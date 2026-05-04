// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

// This is the module that implements atomic operations
module uncacheable_flb
    # ( parameter NUM_NEIGH = `NUM_NEIGH
    )
    (
        // System signals
        input  logic                                         clock,
        input  logic                                         reset,
        input  uc_esr_flb_enable_sigs                        esr_enables,
        input [`ESR_APB_D_WIDTH-1:0]                         esr_wdata,
        output uc_esr_barrier_values_t                       esr_barrier_values,
        // Fast Local Barrier
        input  logic [NUM_NEIGH-1:0]                        flb_neigh_l2_req_valid,
        input  logic [NUM_NEIGH-1:0][`CSR_FL_BARRIER_RANGE] flb_neigh_l2_req_data,
        output logic [NUM_NEIGH-1:0]                        flb_l2_neigh_resp_valid,
        output logic [NUM_NEIGH-1:0]                        flb_l2_neigh_resp_data
        // Operation request bus from minions
        );

    ///////////////////////////////////
    // Clock gater
    ///////////////////////////////////
    logic clkgt;
    logic clock_enable;

    ////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////
    // Fast Local Barrier implementation
    ////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////////
    // Flop neigh signals to meet timing
    ////////////////////////////////////////////////////////////////////////////////
    logic [NUM_NEIGH-1:0]                        flb_neigh_l2_req_valid_reg;
    logic [NUM_NEIGH-1:0][`CSR_FL_BARRIER_RANGE] flb_neigh_l2_req_data_reg;

    generate for(genvar gen_it = 0; gen_it < NUM_NEIGH; gen_it++) begin : NEIGH_FF
      `FF(clock, flb_neigh_l2_req_valid_reg[gen_it], flb_neigh_l2_req_valid[gen_it])
      `EN_FF(clock, flb_neigh_l2_req_valid[gen_it], flb_neigh_l2_req_data_reg[gen_it],  flb_neigh_l2_req_data[gen_it])
    end
    endgenerate

    ////////////////////////////////////////////////////////////////////////////////
    // First the requests are stored in a local FIFO per neigh
    ////////////////////////////////////////////////////////////////////////////////

    logic [NUM_NEIGH-1:0][`CSR_FL_BARRIER_RANGE]  flb_pop_data;  // Next request to be processed per neigh
    logic [NUM_NEIGH-1:0]                         flb_pop_entry; // Pops an entry for a given neigh FIFO
    logic [NUM_NEIGH-1:0]                         flb_pending;   // Pending request to be processed per neigh
    logic [NUM_NEIGH-1:0]                         flb_fifo_full_unused;
    logic [NUM_NEIGH-1:0][`FLB_ELEMS_PER_NEIGH:0] flb_fifo_level_unused;


    generate for(genvar gen_it = 0; gen_it < NUM_NEIGH; gen_it++)
    begin : INPUT_FIFO
        gen_fifo_reg
        #(
            .WIDTH   ( `CSR_FL_BARRIER_SIZE            ),
            .DEPTH   ( `FLB_ELEMS_PER_NEIGH            )
        )
        req_fifo
        (
            // System signals
            .clock        ( clkgt                          ),
            .reset        ( reset                          ),
            // Push data  
            .push         ( flb_neigh_l2_req_valid_reg[gen_it] ),
            .wdata        ( flb_neigh_l2_req_data_reg[gen_it]  ),
            .full         ( flb_fifo_full_unused[gen_it]   ),
            // Pop data
            .pop          ( flb_pop_entry[gen_it]          ),
            .rdata        ( flb_pop_data[gen_it]           ),
            .valid        ( flb_pending[gen_it]            ),
            .level        ( flb_fifo_level_unused[gen_it]         )
            );

      `ASSERT_ALWAYS(ERROR_req_fifo_overflow, clock, reset, !(flb_neigh_l2_req_valid_reg[gen_it] && flb_fifo_full_unused[gen_it]))
    end
    endgenerate
   `ASSERT_END_OF_SIM(ERROR_end_of_sim_fifo_valid_should_be_off, ~(|flb_pending), -1 )
    ////////////////////////////////////////////////////////////////////////////////
    // Second step is arbitrating and selecting the next element to be processed
    ////////////////////////////////////////////////////////////////////////////////

    
    logic [NUM_NEIGH-1:0] flb_arb_grant;  // One hot winner
    logic [$clog2(NUM_NEIGH):0] flb_arb_winner; // Arbiter winner of the FLB


    logic [`CSR_FL_BARRIER_NUM_RANGE]   flb_barrier_num;        // Barrier number we are working on
    logic [`BARRIER_REGISTERS_R][`CSR_FL_BARRIER_LIMIT_RANGE] barrier_regs;    // Atomic registers that can be operated
    logic [`CSR_FL_BARRIER_LIMIT_RANGE]                      atomic_res;     // Result of the atomic operation
    logic [`BARRIER_REGISTERS_R]                              barrier_regs_en; // Enable bit to update the contents of the registers
    logic                               flb_update_regs;        // FLB update the atomic regs
    logic [`CSR_FL_BARRIER_LIMIT_RANGE] flb_barrier_value_next; // Value written to atomic regs from FLB logic
    logic [`CSR_FL_BARRIER_LIMIT_RANGE] flb_update_barrier_value; // Value written to atomic regs from FLB logic

    

    generate for(genvar i = 0; i < `BARRIER_REGISTERS; i++)
    begin : BARRIER_REG
        //          CLK    RST    EN                                      DOUT             DIN
        `RST_EN_FF(clkgt, reset, esr_enables[i] | barrier_regs_en[i], barrier_regs[i], esr_enables[i]? esr_wdata[`FLB_REG_RANGE] : atomic_res, '0)
        assign esr_barrier_values[i] = barrier_regs[i];
    end
    endgenerate

    always_comb begin
        atomic_res = 0;
        barrier_regs_en = 0;
        if(flb_update_regs)
        begin
            barrier_regs_en[flb_barrier_num] = 1'b1;
            atomic_res                      = flb_barrier_value_next;
        end

    end

    generate if (NUM_NEIGH < 2) 
    begin : NO_ARB_LOGIC
      assign flb_arb_grant = flb_pending;
      assign flb_arb_winner = '0;
    end
    else 
    begin : WITH_ARB_LOGIC
    // It is an LRU between the different minions
    arb_lru
    #(
        .NUM_CLIENTS ( NUM_NEIGH     )
    )
    arb_to_l2
    (
        // System signals
        .clock       ( clkgt          ),
        .reset       ( reset          ),
        // Bidding
        .bid         ( flb_pending    ),
        .stall       ( 1'b0           ),
        // Grant     
        .grant       ( flb_arb_grant  ),
        .winner      ( flb_arb_winner )
        );
    end
    endgenerate

    ////////////////////////////////////////////////////////////////////////////////
    // Selects the winner and checks if there are other neighs requesting same
    // barrier and same limit, so we can merge the accesses
    ////////////////////////////////////////////////////////////////////////////////

    logic [`CSR_FL_BARRIER_RANGE]       flb_winner;         // Winner data
    logic [NUM_NEIGH-1:0]               flb_merge_winner;   // Requests from different neighs can be merged with winner
    logic [NUM_NEIGH-1:0]               flb_merge_popcount; // How many requests are processed at same time
    logic [`CSR_FL_BARRIER_LIMIT_RANGE] flb_barrier_value;  // Barrier value
    logic [`CSR_FL_BARRIER_LIMIT_RANGE] flb_barrier_limit;  // Barrier value limit
    logic                               flb_resp_data_prev; // Response from FLB to minions

    always_comb
    begin
        flb_resp_data_prev = 1'b0;
        flb_update_regs    = 0;
        flb_barrier_value_next = 'b0;
        // Selects winner
        flb_winner = flb_pop_data[flb_arb_winner];

        // Generates merge mask to know what can be processed at same time
        for(integer i = 0; i < NUM_NEIGH; i++)
            flb_merge_winner[i] = (flb_winner == flb_pop_data[i]) & flb_pending[i];

        // Popcount of merges
        flb_merge_popcount = 'b0;
        for(integer i = 0; i < NUM_NEIGH; i++)
            if(flb_merge_winner[i]) flb_merge_popcount = flb_merge_popcount + 'b1;

        // Gets the counter value
        flb_barrier_num   = flb_winner[`CSR_FL_BARRIER_NUM_RANGE];
        flb_barrier_limit = flb_winner[`CSR_FL_BARRIER_LIMIT_RANGE];
        flb_barrier_value = barrier_regs[flb_barrier_num];

        flb_pop_entry   = '0;

        if(!esr_enables[flb_barrier_num])begin
            // Checks that the merged guys don't go beyond the limit of the barrier
            // If so, only one element is processed
            flb_update_regs    = |flb_pending;
            flb_update_barrier_value = flb_barrier_value + flb_merge_popcount;
            if(flb_update_barrier_value >= flb_barrier_limit)
            begin
                // Last element (return 1, set 0)
                if(flb_barrier_value == flb_barrier_limit)
                begin
                    flb_barrier_value_next = 'b0;
                    flb_resp_data_prev     = 1'b1;
                end
                else
                    flb_barrier_value_next = flb_barrier_value + 1'b1;
                flb_pop_entry = flb_arb_grant;
            end
            // All elements processed at once, all return 0
            else
            begin
                flb_barrier_value_next = flb_update_barrier_value;
                flb_pop_entry          = flb_merge_winner;
            end
        end
    end

    ////////////////////////////////////////////////////////////////////////////////
    // To meet timing, flops the response
    ////////////////////////////////////////////////////////////////////////////////

    logic flb_resp_data; // Response data


    `RST_FF(clkgt, reset,  flb_l2_neigh_resp_valid, flb_pop_entry,      'b0)

    //      CLK    RST        EN    DOUT                                   DIN                 DEF
    `EN_FF    (clkgt,        |flb_pop_entry,  flb_resp_data,           flb_resp_data_prev)

    assign flb_l2_neigh_resp_data = flb_l2_neigh_resp_valid & {NUM_NEIGH{flb_resp_data}};


   /////////////////////////////
   // Clock gating
   ///////////////////////////

     assign clock_enable = (reset ||
			   (|flb_neigh_l2_req_valid_reg) ||
			   (|esr_enables) ||
			   (|flb_update_regs) ||
			   (|flb_pop_entry) ||
			   (|flb_l2_neigh_resp_valid));
			   

    et_clk_gate ck_gate(.enclk(clkgt), .en(clock_enable), .clk(clock), .te(1'b0));

   /////////////////////////////
   // LINT
   ///////////////////////////


   wire unused_ok = &{
  `ifndef ET_ASCENT_LINT
		   1'b0,
  `endif
		      flb_fifo_full_unused,
		      flb_fifo_level_unused
		      };

endmodule


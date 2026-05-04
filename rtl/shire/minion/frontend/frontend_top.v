// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module frontend_top (
  // System signals
  input  logic                                        clock,
  input  logic                                        reset,
  input  logic                                        reset_debug,
  input  logic                                        chicken_bit,
  // Enable and Reset PC
  input  logic [`CORE_NR_THREADS-1:0]                 f0_thread_enabled,
  input  logic [`VA_RANGE]                            f0_reset_vector,
  // TLB control
  input  minion_vm_status [`CORE_NR_THREADS-1:0]      vm_status,
  // Icache requests
  input  logic                                        f1_icache_req_ready,
  output logic                                        f1_icache_req_valid,
  output frontend_icache_req                          f1_icache_req,
  // Icache response
  input  logic                                        f5_icache_resp_valid,
  input  logic                                        f5_icache_resp_miss,
  input  icache_frontend_resp                         f5_icache_resp,
  input  logic                                        f6_icache_fill_done,
  // Mispredict from core
  input  logic [`CORE_NR_THREADS-1:0]                 f0_core_req_valid,
  input  minion_fe_req [`CORE_NR_THREADS-1:0]         f0_core_req,
  input  [`CORE_NR_THREADS-1:0]                       id_core_stall,
  // Fetch resp
  input  logic                                        id_inst_ready,
  output logic                                        id_inst_valid,
  output logic                                        id_inst_thread_id,
  output frontend_core_resp                           id_inst_data,
  // intpipe decode control
  output minion_control                               id_intpipe_ctrl,
  // VPU decode signals
  output vpu_ctrl_sigs_t                              id_vpu_decoder_sigs,
  output vpu_minion_id_ctrl                           id_vpu_core_ctrl,
  // interface to write program buffer for debug
  input [`MINION_DEBUG_APB_D_WIDTH-1:0]               debug_ffb_wdata,
  input [`MINION_DEBUG_FFB_WORDS-1:0]                 debug_ffb_en,
  input                                               debug_ffb_thread_sel,
  input [`CORE_NR_THREADS-1:0]                        debug_ffb_exec,
  input [`CORE_NR_THREADS-1:0]                        halt,
  input [`CORE_NR_THREADS-1:0]                        halted
);

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// F0 Stage
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

  // detect changes in thread enable
  logic [`CORE_NR_THREADS-1:0]                      f0_thread_enabled_f; // Flopped thread enable due signal coming from outside neigh
  logic [`CORE_NR_THREADS-1:0]                      f1_thread_enabled;
  logic                                             thread_enable_changes;
  logic [`CORE_NR_THREADS-1:0]                      f0_disable_thread;    // Thread is disabled this cycle
  logic [`CORE_NR_THREADS-1:0]                      f0_enable_thread;     // Thread is enabled this cycle
  `EN_FF(clock, thread_enable_changes, f0_thread_enabled_f, f0_thread_enabled)
  `EN_FF(clock, thread_enable_changes, f1_thread_enabled,   f0_thread_enabled_f)
  always_comb begin
    thread_enable_changes = f0_thread_enabled != f0_thread_enabled_f ||  f0_thread_enabled_f != f1_thread_enabled || reset;
    f0_disable_thread = (~f0_thread_enabled_f) &  f1_thread_enabled;
    f0_enable_thread  =  f0_thread_enabled_f & (~f1_thread_enabled);
  end

////////////////////////////////////////////////////////////////////////////////
// Frontend top stores the status for all the threads
// Each thread status its stored in the frontend thread buffer module. The top
// arbitrates the access to the icache in case than more than one thread wants
// to fetch new data.
// It also grants access to the core pipeline in case than more than one
// thread has available instructions to be issued (this is the typical case)
////////////////////////////////////////////////////////////////////////////////

  frontend_icache_req [`CORE_NR_THREADS-1:0] f0_icache_req_arb;       // Per thread icache request going to arbiter
  frontend_operation [`CORE_NR_THREADS-1:0]  f7_tb_inst_data;         // Next instruction read from thread buffer
  logic [`CORE_NR_THREADS-1:0]               f0_icache_req_ready_arb; // Per thread grant signal from arbiter
  logic                                      f1_icache_req_ready_arb; // Stall signal to arbiter
  logic [`CORE_NR_THREADS-1:0]               f0_icache_req_valid_arb; // Per thread valid signal (bid) to arbiter
  logic                                      f6_icache_resp_valid;
  logic                                      f6_icache_resp_miss;
  icache_frontend_resp                       f6_icache_resp;
  logic                                      f7_icache_fill_done;
  logic [`CORE_NR_THREADS-1:0]               f7_tb_inst_ready;        // Ready signal going to thread buffer
  logic [`CORE_NR_THREADS-1:0]               f7_tb_inst_valid;        // Valid signal from thread buffer
  logic [`CORE_NR_THREADS-1:0]               f7_tb_inst_rvc;          // instruction is compressed
  logic [`CORE_NR_THREADS-1:0]               f7_thread_awake; // Qualified valid after taking into account WFI, fence, tensorwait, ...
 

  for(genvar thread = 0; thread < `CORE_NR_THREADS; thread++) begin : FRONTEND_THREAD
    logic [`MINION_DEBUG_FFB_WORDS-1:0]              thread_debug_ffb_en;
    assign thread_debug_ffb_en = debug_ffb_thread_sel == thread ? debug_ffb_en : '0;

    frontend_thread_buffer #(
      .THREAD_ID            ( thread                          )
    ) thread_buffer (
      // System signals
      .clock_aon            ( clock                           ),
      .reset                ( reset                           ),
      .reset_debug          ( reset_debug                     ),
      .chicken_bit          ( chicken_bit                     ),
      // Thread enable and reset PC
      .f0_thread_enabled    ( f0_thread_enabled_f[thread]     ),
      .f0_reset_vector      ( f0_reset_vector                 ),
      .f0_disable_thread    ( f0_disable_thread[thread]       ),
      .f0_enable_thread     ( f0_enable_thread[thread]        ),
      .in_debug_mode        ( halted[thread]                  ),
      // TLB control
      .vm_status            ( vm_status[thread]               ),
      // Icache requests
      .f0_icache_req_ready  ( f0_icache_req_ready_arb[thread] ),
      .f1_icache_req_ready  ( f1_icache_req_ready_arb         ),
      .f0_icache_req_valid  ( f0_icache_req_valid_arb[thread] ),
      .f0_icache_req        ( f0_icache_req_arb[thread]       ),
      // Icache response
      .f5_icache_resp_miss  ( f5_icache_resp_miss             ),
      .f6_icache_resp_valid ( f6_icache_resp_valid            ),
      .f6_icache_resp_miss  ( f6_icache_resp_miss             ),
      .f6_icache_resp       ( f6_icache_resp                  ),
      .f7_icache_fill_done  ( f7_icache_fill_done             ),
      // Mispredict from core
      .f0_core_req_valid    ( f0_core_req_valid[thread]       ),
      .f0_core_req          ( f0_core_req[thread]             ),
      // Fetch resp
      .f7_inst_ready        ( f7_tb_inst_ready[thread]        ),
      .f7_inst_valid        ( f7_tb_inst_valid[thread]        ),
      .f7_inst_rvc          ( f7_tb_inst_rvc[thread]          ),
      .f7_inst_data         ( f7_tb_inst_data[thread]         ),
      // interface to write program buffer for debug
      .debug_ffb_wdata      ( debug_ffb_wdata                 ),
      .debug_ffb_en         ( thread_debug_ffb_en             ),
      .debug_ffb_exec       ( debug_ffb_exec[thread]          ),
      .io_halt              ( halt[thread]                    )
    );
  end

////////////////////////////////////////////////////////////////////////////////
// Arbiter that selects which is the next thread within the minion that gets
// to bid to the icache port. Output of the arbiter is flopped
////////////////////////////////////////////////////////////////////////////////

  assign f1_icache_req_ready_arb = f1_icache_req_ready || !f1_icache_req_valid;

  generate if(`CORE_NR_THREADS == 1) begin : FRONTEND_ICACHE_ARB_DIS
    //          CLK    RST    EN                                                    DOUT                   DIN                         DEF
    `EN_FF     (clock,        f1_icache_req_ready_arb & f0_icache_req_valid_arb[0], f1_icache_req,         f0_icache_req_arb[0])
    `RST_EN_FF (clock, reset, f1_icache_req_ready_arb,                              f1_icache_req_valid,   f0_icache_req_valid_arb[0], 1'b0)
  end
  endgenerate

  generate if(`CORE_NR_THREADS > 1) begin : FRONTEND_ICACHE_ARB
    frontend_icache_req f0_icache_req_winner;
    logic [`CORE_NR_THREADS_L-1:0] icache_req_arb_winner_unused_ok;

    arb_lru_data #(
      .NUM_CLIENTS ( `CORE_NR_THREADS           ),
      .WIDTH       ( $bits(frontend_icache_req) )
    ) icache_req_arb (
      // System signals
      .clock       ( clock                      ),
      .reset       ( reset                      ),
      // Bidding
      .bid         ( f0_icache_req_valid_arb    ),
      .stall       ( !f1_icache_req_ready_arb   ),
      // Data inputs
      .data_in     ( f0_icache_req_arb          ),
      // Grant
      .grant       ( f0_icache_req_ready_arb    ),
      .data_winner ( f0_icache_req_winner       ),
      .winner      ( icache_req_arb_winner_unused_ok )
    );

    //          CLK    RST    EN                                                  DOUT                 DIN                       DEF
    `EN_FF     (clock,        f1_icache_req_ready_arb & |f0_icache_req_valid_arb, f1_icache_req,       f0_icache_req_winner)
    `RST_EN_FF (clock, reset, f1_icache_req_ready_arb,                            f1_icache_req_valid, |f0_icache_req_valid_arb, 1'b0)
  end
  endgenerate

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// F6 Stage
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

  // Flop icache response

  //       CLK    RST    EN                    DOUT                  DIN                   DEF
  `RST_FF (clock, reset,                       f6_icache_resp_valid, f5_icache_resp_valid, 1'b0)
  `EN_FF  (clock,        f5_icache_resp_valid, f6_icache_resp_miss,  f5_icache_resp_miss)
  `EN_FF  (clock,        f5_icache_resp_valid, f6_icache_resp,       f5_icache_resp)

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// F7 Stage
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

  // Flop icache fill response and errors
  //         CLK    RST    EN                   DOUT                   DIN                  DEF
  `RST_FF   (clock, reset,                      f7_icache_fill_done,   f6_icache_fill_done, 1'b0)

////////////////////////////////////////////////////////////////////////////////
// Compose core response
////////////////////////////////////////////////////////////////////////////////

  frontend_core_resp [`CORE_NR_THREADS-1:0] f7_core_inst_data;  // Next instruction assembled by instruction buffer


  always_comb begin
    for(int t = 0; t < `CORE_NR_THREADS; t++) begin
      f7_core_inst_data[t].pc =  f7_tb_inst_data[t].pc;
      f7_core_inst_data[t].inst_bits = f7_tb_inst_data[t].inst_bits;

      f7_core_inst_data[t].rvc = f7_tb_inst_rvc[t];

      // page and acess faults
      f7_core_inst_data[t].pf0 = f7_tb_inst_valid[t] && f7_tb_inst_data[t].page_fault0;
      f7_core_inst_data[t].af0 = f7_tb_inst_valid[t] && f7_tb_inst_data[t].access_fault0;
      f7_core_inst_data[t].pf1 = f7_tb_inst_valid[t] && f7_tb_inst_data[t].page_fault1;
      f7_core_inst_data[t].af1 = f7_tb_inst_valid[t] && f7_tb_inst_data[t].access_fault1;

      // and errors
      f7_core_inst_data[t].bus_error = f7_tb_inst_valid[t] && f7_tb_inst_data[t].bus_err;
      f7_core_inst_data[t].ecc_error = f7_tb_inst_valid[t] && f7_tb_inst_data[t].ecc_err;

      f7_core_inst_data[t].replay = f7_tb_inst_data[t].replay;
    end // for (int t = 0; t < `CORE_NR_THREADS; t++)
  end // always_comb

////////////////////////////////////////////////////////////////////////////////
// Select which thread instruction it is going to be expanded and vpu-decoded.
////////////////////////////////////////////////////////////////////////////////

  frontend_core_resp           f7_arb_inst_data; // Instruction selected by expander picker in F7 stage
  logic                        f7_exp_thread_id; // Thread of the instruction being expanded in F7
  logic [`CORE_NR_THREADS-1:0] inst_fifo_full;
  logic [`CORE_NR_THREADS-1:0] inst_fifo_empty;
  logic [`CORE_NR_THREADS-1:0] inst_fifo_push;

  frontend_thread_data [`CORE_NR_THREADS-1:0] inst_fifo_data;

  generate if(`CORE_NR_THREADS == 1) begin : FRONTEND_EXP_ARB_DIS
    always_comb begin
      f7_exp_thread_id = 1'b0;
      f7_arb_inst_data = f7_core_inst_data[f7_exp_thread_id];
    end
  end
  endgenerate

  generate if(`CORE_NR_THREADS > 1) begin : FRONTEND_EXP_ARB
    always_comb begin
      // select thread if fifo not full, awake and valid
      case (~inst_fifo_full & f7_thread_awake & f7_tb_inst_valid)
        2'b01:   f7_exp_thread_id = 1'b0; // only thread 0 available
        2'b10:   f7_exp_thread_id = 1'b1; // only thread 1 available
        2'b11:   f7_exp_thread_id = ~id_inst_thread_id; //both threads available, select a different one from last cycle
        default: f7_exp_thread_id = ~id_inst_thread_id; // none available,  select a different one ( so that in the last cycle we select the same)
      endcase

      f7_arb_inst_data = f7_core_inst_data[f7_exp_thread_id];
    end // always_comb
  end
  endgenerate


  ////////////////////////////////////////////////////////////////////////////////
  // intpipe decoder
  ////////////////////////////////////////////////////////////////////////////////
  minion_control     f7_intpipe_ctrl;

  intpipe_decode int_dec (
    // Input instruction
    .inst_bits       ( f7_arb_inst_data.inst_bits ),
    // Output decode bits
    .inst_ctrl       ( f7_intpipe_ctrl       )
  );


  ////////////////////////////////////////////////////////////////////////////////
  // VPU decoder and intpipe operand dependencies
  ////////////////////////////////////////////////////////////////////////////////
  vpu_ctrl_sigs_t    f7_vpu_decoder_sigs;
  logic              f7_is_vpu_insn;

  vpu_decoder vpu_dec (
    .id_inst     ( f7_arb_inst_data.inst_bits ),
    .id_sigs     ( f7_vpu_decoder_sigs        ),
    .id_vpu_insn ( f7_is_vpu_insn             )
  );

  // 2-depth instruction fifo per thread
  
   for ( genvar thread = 0; thread < `CORE_NR_THREADS; thread++) begin: INST_FIFOS
     logic [1:0] rd_ptr;
     logic [1:0] wr_ptr;
     logic       pop;


     `RST_FF(clock, reset || f0_core_req_valid[thread], rd_ptr, rd_ptr + pop, 2'b00)
     `RST_FF(clock, reset || f0_core_req_valid[thread], wr_ptr, wr_ptr + inst_fifo_push[thread], 2'b00)

     assign inst_fifo_push[thread] = (thread == f7_exp_thread_id && f7_tb_inst_valid[thread] && !inst_fifo_full[thread]);
     assign pop  = (id_inst_valid && id_inst_ready && id_inst_thread_id == thread);
     assign inst_fifo_full[thread]  = (rd_ptr ^ wr_ptr) == 2'b10;
     assign inst_fifo_empty[thread] = (rd_ptr ^ wr_ptr) == 2'b00;


     // implementing the commented section above, but in a different way to improve timing
     frontend_thread_data pending_data;
     logic       pending_data_is_fp;

     always_ff@(posedge clock) begin
        //fifo empty => inst_fifo_data[thread] is in (clock gated with push)
        if (inst_fifo_empty[thread]) begin
          if (inst_fifo_push[thread]) begin
            inst_fifo_data[thread].core_resp <= f7_arb_inst_data;
            inst_fifo_data[thread].intpipe_ctrl <= f7_intpipe_ctrl;
            if (f7_is_vpu_insn) inst_fifo_data[thread].vpu_ctrl_sigs <= f7_vpu_decoder_sigs;
          end
        end  else if (!inst_fifo_full[thread]) begin

        // 1 entry in the fifo
          if (pop && inst_fifo_push[thread]) begin //reading current entry.. and writing next => out_fifo in the cycle is in data
            inst_fifo_data[thread].core_resp <= f7_arb_inst_data;
            inst_fifo_data[thread].intpipe_ctrl <= f7_intpipe_ctrl;
            if (f7_is_vpu_insn) inst_fifo_data[thread].vpu_ctrl_sigs <= f7_vpu_decoder_sigs;
          end  else if (inst_fifo_push[thread]) begin
            pending_data.core_resp <= f7_arb_inst_data;
            pending_data.intpipe_ctrl <= f7_intpipe_ctrl;
            if (f7_is_vpu_insn) pending_data.vpu_ctrl_sigs <= f7_vpu_decoder_sigs;
            pending_data_is_fp <= f7_is_vpu_insn;
          end
        end
        // 2 entries in the fifo
        else begin
          if (pop) begin
            inst_fifo_data[thread].core_resp <= pending_data.core_resp;
            inst_fifo_data[thread].intpipe_ctrl <= pending_data.intpipe_ctrl;
            if (pending_data_is_fp) inst_fifo_data[thread].vpu_ctrl_sigs <= pending_data.vpu_ctrl_sigs;
          end 
          if (inst_fifo_push[thread]) begin
            pending_data.core_resp <= f7_arb_inst_data;
            pending_data.intpipe_ctrl <= f7_intpipe_ctrl;
            if (f7_is_vpu_insn) pending_data.vpu_ctrl_sigs <= f7_vpu_decoder_sigs;
            pending_data_is_fp <= f7_is_vpu_insn;
          end
        end
      end // always_ff@ (posedge clock)


    end // block: INST_FIFOS
 



////////////////////////////////////////////////////////////////////////////////
// Sends the data to next stage
////////////////////////////////////////////////////////////////////////////////

  logic [`CORE_NR_THREADS-1:0] f7_inst_valid_th; // Valid bit going to ID stage per thread

  always_comb begin
    // New data when the ID buffer is empty or being accepted by core and data
    // available
    for(integer i = 0; i < `CORE_NR_THREADS; i++) begin
      f7_tb_inst_ready[i] = (f7_exp_thread_id == i) && // thread selected to go through expander
                 !inst_fifo_full[i]; // and current data can be written to fifo

      f7_inst_valid_th[i] = (!inst_fifo_empty[i] || inst_fifo_push[i]) && !f0_core_req_valid[i]; // has data to deliver and no new request from core.. or will soon have
    end
  end // always_comb

////////////////////////////////////////////////////////////////////////////////
// Thread scheduler that selects which is the next thread to be sent to the
// intpipe.
////////////////////////////////////////////////////////////////////////////////

  logic f7_thread_id; // Which thread ID is selected in F7 stage

  generate if(`CORE_NR_THREADS == 1) begin : FRONTEND_THREAD_SCHED_DIS
    always_comb begin
      f7_thread_id = 1'b0;
    end
  end
  endgenerate

  generate if(`CORE_NR_THREADS > 1) begin : FRONTEND_THREAD_SCHED
    frontend_thread_sched thread_sched (
      // System signals
      .clock             ( clock               ),
      .reset             ( reset               ),
      // Thread enable and valid signals
      .f7_valid_tid      ( f7_inst_valid_th    ),
      .f0_thread_enabled ( f0_thread_enabled_f ),
      .id_core_stall     ( id_core_stall       ),
      // Picked thread
      .f7_thread_awake   ( f7_thread_awake     ),
      .f7_thread_id      ( f7_thread_id        )
    );
  end
  endgenerate


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// ID Stage
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

  `RST_FF(clock, reset,              id_inst_thread_id,   f7_thread_id,     1'b0)
  always_comb begin
    id_inst_valid = !inst_fifo_empty[id_inst_thread_id];
    id_inst_data = inst_fifo_data[id_inst_thread_id].core_resp;
    id_vpu_decoder_sigs = inst_fifo_data[id_inst_thread_id].vpu_ctrl_sigs;
    id_intpipe_ctrl = inst_fifo_data[id_inst_thread_id].intpipe_ctrl;
  end

  ////////////////////////////////////////////////////////////////////////////////
  // vpu control signals to core
  ////////////////////////////////////////////////////////////////////////////////
  always_comb begin
    id_vpu_core_ctrl = '0;
    id_vpu_core_ctrl.m0ren        = id_vpu_decoder_sigs.m0ren;
    id_vpu_core_ctrl.mallren      = id_vpu_decoder_sigs.mallren;
    id_vpu_core_ctrl.mren1        = id_vpu_decoder_sigs.mren1;
    id_vpu_core_ctrl.mren2        = id_vpu_decoder_sigs.mren2;
    id_vpu_core_ctrl.wen          = id_vpu_decoder_sigs.wen | id_vpu_decoder_sigs.rend;
    id_vpu_core_ctrl.ren1         = id_vpu_decoder_sigs.ren1;
    id_vpu_core_ctrl.ren2         = id_vpu_decoder_sigs.ren2;
    id_vpu_core_ctrl.ren3         = id_vpu_decoder_sigs.ren3;
    id_vpu_core_ctrl.rend         = id_vpu_decoder_sigs.rend;
    id_vpu_core_ctrl.is_trans     = id_vpu_decoder_sigs.trans;
    id_vpu_core_ctrl.fromint      = id_vpu_decoder_sigs.fromint;
  end // always_comb

endmodule


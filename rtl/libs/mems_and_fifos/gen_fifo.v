// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "assert.vh"
`include "macros.vh"
`include "shire_assert.vh"

// This module is a generic fifo
module gen_fifo
#(
    parameter ELEM_SIZE = 32,
    parameter NUM_ELEMS = 32,
    parameter USE_LATCHES = 0,
    parameter NUM_ELEMS_LOG = $clog2(NUM_ELEMS)
)
(
    // System signals
    input  logic                 clock,
    input  logic                 reset,
    // Push data
    input  logic                 push_early,
    input  logic                 push,
    input  logic [ELEM_SIZE-1:0] push_data,
    output logic                 full,
    output logic                 full_next,
    // Pop data
    input  logic                 pop,
    output logic [ELEM_SIZE-1:0] pop_data,
    output logic                 valid,
    output logic                 valid_next
);


logic [NUM_ELEMS_LOG:0]   counter, counter_next; // Counts elements
logic [NUM_ELEMS_LOG-1:0] read_ptr;              // Read pointer
logic [NUM_ELEMS_LOG-1:0] write_ptr;             // Write pointer

`RST_FF(clock, reset, counter, counter_next, 'b0)

always_comb
begin
    counter_next = counter;
    if(pop && !push) counter_next = counter - 'b1;
    if(push && !pop) counter_next = counter + 'b1;
end

//assign full_next  = (counter_next == NUM_ELEMS);
assign full_next = pop ? (push & full) : (full | ((counter == (NUM_ELEMS-1)) & push));
assign valid_next = (counter_next != 'b0);

   generate if ( 1<<NUM_ELEMS_LOG == NUM_ELEMS) begin: PTRS
      // # elems is power of 2
      `RST_EN_FF(clock, reset, pop, read_ptr, read_ptr + 'b1, 'b0)
      `RST_EN_FF(clock, reset, push, write_ptr, write_ptr + 'b1, 'b0)
   end
   else begin
      // # elems is not power of 2
      `RST_EN_FF(clock, reset, pop, read_ptr, read_ptr == NUM_ELEMS-1 ? '0 :read_ptr + 'b1, 'b0)
      `RST_EN_FF(clock, reset, push, write_ptr, write_ptr == NUM_ELEMS-1 ? '0 : write_ptr + 'b1, 'b0)
   end
endgenerate

`RST_FF(clock, reset, full, full_next, 'b0)
`RST_FF(clock, reset, valid, valid_next, 'b0)

// Storage
generate if (USE_LATCHES == 1)
begin : lat
    logic push_early_p2;

   `LATCH_P2 (clock, push_early_p2, push_early)

    rf_latch_1r_1w #(
                     .WIDTH(ELEM_SIZE),
                     .ENTRIES(NUM_ELEMS)
                    )
    mem
    (
      .clk             ( clock         ),
      .test_en         ( 1'b0          ),
      .rd_addr_a       ( read_ptr      ),
      .rd_data_a       ( pop_data      ),
      .wr_en_a         ( push          ),
      .wr_data_a_en_1p ( push_early_p2 ),
      .wr_addr_a       ( write_ptr     ),
      .wr_data_a       ( push_data     )
    ) ;
end
else
begin : ff
    logic [NUM_ELEMS-1:0][ELEM_SIZE-1:0] data_array;
    
    always_ff @ (posedge clock)
    begin
        if(push) data_array[write_ptr] <= push_data; // RTLMIN-4717: convert to functional clock gate
    end
    
    assign pop_data = data_array[read_ptr];

    wire unused_ok = &{
    `ifndef ET_ASCENT_LINT
       1'b0,
    `endif
       push_early
    };
end
endgenerate

`ifndef UPF
// Assertions
// synopsys translate_off
`ASSERT_ALWAYS_MSG(ERROR_empty_fifo_pop, clock, reset, (valid || ~pop), $sformatf("Popping empty FIFO") )
`ASSERT_ALWAYS_MSG(ERROR_fifo_overflow, clock, reset, (~full || ~push || pop), $sformatf("Writing to FIFO when full") )
// synopsys translate_on
`endif
endmodule


// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "assert.vh"
`include "macros.vh"
`include "shire_assert.vh"

module gen_push_pop_fifo
#(
    parameter ELEM_SIZE = 32,
    parameter NUM_ELEMS = 32,
    parameter USE_LATCHES = 0,
    parameter NUM_ELEMS_LOG = $clog2(NUM_ELEMS)
)
(
    input  logic                 clock,
    input  logic                 reset,
    input  logic                 run,

    input  logic                 push,
    input  logic [ELEM_SIZE-1:0] push_data,
    output logic                 push_success,

    input  logic                 pop,
    output logic [ELEM_SIZE-1:0] pop_data,
    output logic                 pop_success,

    output logic                 not_full,
    output logic                 next_not_full,
    output logic                 next_push_success,
    output logic                 next_pop_success,
    output logic [ELEM_SIZE-1:0] next_pop_data,
    output logic                 not_empty,
    output logic                 next_not_empty
);

logic [NUM_ELEMS_LOG:0]          count;
logic [NUM_ELEMS_LOG:0]          next_count;

logic [NUM_ELEMS_LOG-1:0]        read_ptr;
logic [NUM_ELEMS_LOG-1:0]        next_read_ptr;

logic [NUM_ELEMS_LOG-1:0]        write_ptr;
logic [NUM_ELEMS_LOG-1:0]        next_write_ptr;

always_comb
begin
    if (push && pop) begin
        next_push_success = 1;
        next_pop_success = 1;
        next_count = count;
    end
    else if (push && !pop) begin
        if (not_full) begin
            next_push_success = 1;
            next_count = count + 1;
        end
        else begin
            next_push_success = 0;
            next_count = count;
        end
        next_pop_success = 0;
    end
    else if (!push && pop) begin
        next_push_success = 0;
        if (not_empty) begin
            next_pop_success = 1;
            next_count = count - 1;
        end
        else begin
            next_pop_success = 0;
            next_count = count;
        end
    end
    else begin // !push && !pop
        next_push_success = 0;
        next_pop_success = 0;
        next_count = count;
    end

    next_read_ptr = (read_ptr + 1) % NUM_ELEMS;
    next_write_ptr = (write_ptr + 1) % NUM_ELEMS;

    next_not_empty = next_count != 0;

    next_not_full = next_count != NUM_ELEMS;
end

`RST_EN_FF(clock, reset, run, not_full, next_not_full, 'b1)
`RST_EN_FF(clock, reset, run, count, next_count, 'b0)
`RST_EN_FF(clock, reset, run, pop_success, next_pop_success, 'b0)
`RST_EN_FF(clock, reset, run, push_success, next_push_success, 'b0)
`RST_EN_FF(clock, reset, run, not_empty, next_not_empty, 'b0)

// Storage
generate if (USE_LATCHES == 1)
begin : lat
    rf_latch_1r_1w #(
                     .WIDTH(ELEM_SIZE),
                     .ENTRIES(NUM_ELEMS)
                    )
    mem
    (
      .clk             ( clock     ),
      .test_en         ( 1'b0      ),
      .rd_addr_a       ( read_ptr  ),
      .rd_data_a       ( pop_data  ),
      .wr_en_a         ( push      ),
      .wr_data_a_en_1p ( 1'b1      ),
      .wr_addr_a       ( write_ptr ),
      .wr_data_a       ( push_data )
    ) ;

    assign next_pop_data = '0;
end
else
begin : ff
    logic [NUM_ELEMS-1:0][ELEM_SIZE-1:0] data_array;
    
    always_ff @ (posedge clock)
    begin
        if (reset) begin
            write_ptr <= '0;
            read_ptr <= '0;
        end
        else if (run) begin
            if (next_push_success) begin
                data_array[write_ptr] <= push_data;
                write_ptr <= next_write_ptr;
            end
            if (next_pop_success) begin
                pop_data <= next_pop_data;
                read_ptr <= next_read_ptr;
            end
        end
    end

    assign next_pop_data = data_array[read_ptr];

end
endgenerate

endmodule


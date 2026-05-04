// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "assert.vh"
`include "macros.vh"
`include "shire_assert.vh"

/*
gen_valid_ready_fifo
********************

gen_valid_ready_fifo is a FIFO intended for connecting a writer and a reader
module that both follow the "ready/valid" protocol.

The module has three groups of pins:

* Common input pins:

- input clock: input clock signal, common to reader and writer.

- input reset: reset signal, also common to reader and writer.

- input run: whether the design is running. It should be high unless the
  clock is to be ignored because the design is being debugged.

* Writer pins:

- input logic in_valid: whether there is valid input data to be written to
  the FIFO.

- input logic [ELEM_SIZE-1:0] in_data: when in_valid is high, the input data
  to be written. Ignored when in_valid is low.

- output in_ready: set by the FIFO if a valid input would be accepted on the
  positive clock edge.

* Reader pins:

- output logic out_valid: whether the is a valid output to be accepted by the
  reader.

- output logic [ELEM_SIZE-1:0] out_data: when out_valid is high, the data to
  accepted by the reader.

- input logic out_ready: when out_valid is high it signals that the reader
  will accept the FIFO output on the next positive clock edge. The signal is
  ignored when out_valid is low.

When instantiating the module one can select:

- the input and output data buses width in bits (ELEM_SIZE),

- the number of FIFO entries (NUM_ELEMS),

- whether to use latches (USE_LATCHES=1) or flipflos (USE_LATCHES=0)

- whether the flow control mechanism is "non-throlling" (THROTTLE_WHEN_FULL=0)
  or "throttling when full" (THROTTLE_WHEN_FULL=1). See next section for details.


Flow control modes
******************

The FIFO supports two ways of handling a write request when the FIFO is
full, also referred as "flow control mechanisms" because they are strategies
for ensuring that the writing flow does not exceed the reader capacity:

- Non-throttling: If "THROTTLE_WHEN_FULL=0" the in_ready output is a
  combinatorial ready signal that depends on the remaining FIFO capacity and
  also on the out_ready input. When the FIFO is full the FIFO can still
  commit to accept a new input if (and only if) the reader is ready.

  Conceptually, the in_ready signal is computed as the "!full || out_ready"
  logical expression, where "full" is an internal wire signalling whether
  the FIFO is full.

- Throttling when full: If "THROTTLE_WHEN_FULL=1" the in_ready output only
  checks whether the FIFO is full. If full se set then the FIFO is not
  ready, irrespective of whether out_ready is high.

  Conceptually, the in_ready signal is hardwired to the internal "full" signal,
  which is enabled when the FIFO is full.

The non-throttling mode has the advantage that does not introduce throttling
as long as the out_ready signal is high. The drawback of the non-throttling mode
is that the length of the in_ready combinatorial path is not bounded, as it
depends on the length of the critical path on the out_ready input. The length of
the combinatorial path not being bounded means that one may be potentially
chaining logic gates beyond the maximum length that fits in timing.

The "throttling when full" mode on the other hand does exibit throttling, but
has bounded timing. The throttling may in some cases be an acceptable side
effect that one is ready to accept when fixing a timing issue involving the
"ready" signal on a "valid/ready" interface.

Example:

   // Instantiating a 2 elements throttling FIFO for fixing a REGOUT timing
   // violation on a path involving the out ready signal of an "valid/ready"
   // input interface.
   //
   // The "proxy_in_ready", "proxy_in_valid" and "proxy_in_bits" are proxy
   // signals introduced for the design to use instead "in_ready", "in_valid"
   // and "in_bits".
   // 
   // The introduction of "input_fifo" makes the "in_ready" path to be bounded
   // in timing, rather than to be determined by "out_ready".

   gen_valid_ready_fifo
   #(
      .ELEM_SIZE          ( $bits(in_bits) ),
      .NUM_ELEMS          ( 2              ),
      .USE_LATCHES        ( 0              ),
      .THROTTLE_WHEN_FULL ( 1              )
   )
   input_fifo
   (
      .clock       ( clock          ),
      .reset       ( reset          ),
      .run         ( run            ),

      .in_valid    ( in_valid       ),
      .in_data     ( in_bits        ),
      .in_ready    ( proxy_in_ready ),

      .out_valid   ( proxy_in_valid ),
      .out_data    ( proxy_in_bits  ),
      .out_ready   ( in_ready       )
   );

*/

module gen_hybrid_fifo
#(
    parameter ELEM_SIZE = 32,
    parameter NUM_ELEMS = 32,
    parameter USE_LATCHES = 0,
    parameter NUM_ELEMS_LOG = $clog2(NUM_ELEMS),
    parameter THROTTLE_WHEN_FULL = 0
)
(
    input  logic                 clock,
    input  logic                 reset,
    input  logic                 run,

    input  logic                 in_valid,
    input  logic [ELEM_SIZE-1:0] in_data,
    output logic                 in_ready,

    input  logic                 pop,
    output logic [ELEM_SIZE-1:0] pop_data,
    output logic                 pop_success,

    output logic                 next_pop_success,
    output logic [ELEM_SIZE-1:0] next_pop_data,
    output logic                 not_empty,
    output logic                 next_not_empty
);

logic                            not_full;
logic                            next_not_full;
logic                            push;
logic [ELEM_SIZE-1:0]            push_data;
logic                            push_success;
logic                            next_push_success;

generate if (THROTTLE_WHEN_FULL == 1)
begin

   assign in_ready = not_full;
   assign push = in_valid && in_ready;
   assign push_data = in_data;

end
else begin

    assign in_ready = !push || next_push_success;

    always_ff @ (posedge clock)
    begin
        if (reset) begin
            push <= '0;
            push_data <= '0;
        end
        else if (run && in_ready) begin
            push <= in_valid;
            push_data <= in_data;
        end
    end
end
endgenerate

gen_push_pop_fifo
#(
    .ELEM_SIZE(ELEM_SIZE),
    .NUM_ELEMS(NUM_ELEMS),
    .USE_LATCHES(USE_LATCHES),
    .NUM_ELEMS_LOG(NUM_ELEMS_LOG)
)
fifo
(
    .clock(clock),
    .reset(reset),
    .run(run),

    .push(push),
    .push_data(push_data),
    .push_success(push_success),

    .pop(pop),
    .pop_data(pop_data),
    .pop_success(pop_success),

    .not_full(not_full),
    .next_not_full(next_not_full),
    .next_push_success(next_push_success),
    .next_pop_success(next_pop_success),
    .next_pop_data(next_pop_data),
    .not_empty(not_empty),
    .next_not_empty(next_not_empty)
);

endmodule


module gen_valid_ready_fifo
#(
    parameter ELEM_SIZE = 32,
    parameter NUM_ELEMS = 32,
    parameter USE_LATCHES = 0,
    parameter NUM_ELEMS_LOG = $clog2(NUM_ELEMS),
    parameter THROTTLE_WHEN_FULL = 0
)
(
    input  logic                 clock,
    input  logic                 reset,
    input  logic                 run,

    input  logic                 in_valid,
    input  logic [ELEM_SIZE-1:0] in_data,
    output logic                 in_ready,

    output logic                 out_valid,
    output logic [ELEM_SIZE-1:0] out_data,
    input  logic                 out_ready
);

logic                            out_fire;
logic [ELEM_SIZE-1:0]            next_pop_data;
logic                            not_empty;

assign out_valid = not_empty;
assign out_fire = out_valid && out_ready;
assign out_data = next_pop_data;

gen_hybrid_fifo
#(
    .ELEM_SIZE(ELEM_SIZE),
    .NUM_ELEMS(NUM_ELEMS),
    .USE_LATCHES(USE_LATCHES),
    .NUM_ELEMS_LOG(NUM_ELEMS_LOG),
    .THROTTLE_WHEN_FULL(THROTTLE_WHEN_FULL)
)
fifo
(
    .clock(clock),
    .reset(reset),
    .run(run),

    .in_valid(in_valid),
    .in_data(in_data),
    .in_ready(in_ready),

    .pop(out_fire),
    .pop_data(/* Not used*/),
    .pop_success(/* Not used*/),

    .next_pop_success(/*Not used*/),
    .next_pop_data(next_pop_data),
    .not_empty(not_empty),
    .next_not_empty(/* Not used */)
);

endmodule


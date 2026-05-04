// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0


module delay_register
#(
    parameter SIZE = 32,
    parameter NUM = 2
)
(
    // System signals
    input                 clock,
    input                 reset,
    // Push data
    input                 en,
    input[SIZE-1:0]  data_in,
    // Pop data
    output[SIZE-1:0] data_out
);

logic [SIZE-1:0] data_q_r [NUM-1:0];

assign data_out = data_q_r[NUM];


always_ff @ (posedge clock) begin
  if(reset) begin
    for(integer i=0; i<NUM-1; i++) begin
      data_q_r[i] <= {(SIZE){1'b0}};
    end
  end else begin
    if(en) begin
      data_q_r[0] <= data_in;

      for(integer i=0; i<NUM-1; i++) begin
        data_q_r[i+1] <= data_q_r[i];
      end
    end
  end
end

endmodule


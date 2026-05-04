// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module pseudo_lru #(
  parameter int unsigned NWays = 8
) (
  input  logic                     clk_i,
  input  logic                     rst_ni,
  input  logic                     access_i,
  input  logic [$clog2(NWays)-1:0] index_in_i,
  output logic [$clog2(NWays)-1:0] index_out_o
);
  localparam int unsigned IdxWidth = $clog2(NWays);

  logic [NWays-1:1] plru_state_q;
  logic [NWays-1:1] next_plru_state;

  // synthesis translate_off
  initial begin
    assert ($onehot(NWays))
      else $error("NWays has to be a power of 2");
  end
  // synthesis translate_on

  assign next_plru_state[1] = ~index_in_i[IdxWidth-1];

  genvar st_idx;
  generate
    for (st_idx = 2; st_idx < NWays; st_idx++) begin : gen_state_per_way
      localparam int unsigned PtrIdxBits = $clog2(st_idx + 1);

      logic [PtrIdxBits-1:0] ptr_idx;

      assign ptr_idx = {1'b1, index_in_i[IdxWidth-1 -: PtrIdxBits-1]};

      always_comb begin
        next_plru_state[st_idx] = plru_state_q[st_idx];
        if (ptr_idx == st_idx[PtrIdxBits-1:0]) begin
          next_plru_state[st_idx] = ~index_in_i[IdxWidth-PtrIdxBits];
        end
      end
    end
  endgenerate

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      plru_state_q <= '0;
    end else if (access_i) begin
      plru_state_q <= next_plru_state;
    end
  end

  assign index_out_o[IdxWidth-1] = plru_state_q[1];

  genvar i_out;
  generate
    for (i_out = 0; i_out < IdxWidth-1; i_out++) begin : gen_index_out
      localparam int unsigned OutBit     = IdxWidth - 2 - i_out;
      localparam int unsigned PtrIdxBits = IdxWidth - OutBit;

      logic [PtrIdxBits-1:0] ptr_idx;

      assign ptr_idx = {1'b1, index_out_o[IdxWidth-1 -: PtrIdxBits-1]};
      assign index_out_o[OutBit] = plru_state_q[ptr_idx];
    end
  endgenerate
endmodule

// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module pseudo_lru #(
  parameter N_WAYS = 8
) (
  clock,
  reset,
  access,
  index_in,
  index_out
);

  localparam IDX_SZ = $clog2(N_WAYS);

  //synopsys translate_off
  initial begin
    assert($onehot(N_WAYS)) else $error ("N_WAYS has to be a power of 2");
  end
  //synopsys translate_on

  // INPUT/OUTPUT DECLARATION
  input  clock;
  input  reset;
  input  access;
  input  [IDX_SZ-1:0] index_in;
  output [IDX_SZ-1:0] index_out;

  logic [N_WAYS-1:1] plru_state, next_plru_state;

  // Obtain new PLRU state according to accessed index
  assign next_plru_state[1] = ~index_in[IDX_SZ-1];

   
  for (genvar st_idx = 2; st_idx < N_WAYS; st_idx++) begin: STATE_PER_WAY
    localparam PTR_IDX_BITS = $clog2(st_idx+1);

    logic [PTR_IDX_BITS-1:0] ptr_idx;
    assign ptr_idx = {1'b1, index_in[IDX_SZ-1 -: PTR_IDX_BITS-1]};

    always_comb begin
      next_plru_state[st_idx] = plru_state[st_idx];

      if (ptr_idx == st_idx[PTR_IDX_BITS-1:0]) begin
        next_plru_state[st_idx] = ~index_in[IDX_SZ - PTR_IDX_BITS];
      end
    end
  end


  always_ff @(posedge clock) begin
    if (reset) begin
      plru_state <= '0;
    end else if (access) begin
      plru_state <= next_plru_state;
    end
  end

  // Select index to be replaced according to current PLRU state
  assign index_out[IDX_SZ-1] = plru_state[1];


  for (genvar i_out = IDX_SZ-2; i_out >= 0; i_out--) begin: INDEX_OUT
    localparam PTR_IDX_BITS = IDX_SZ-i_out;

    logic [PTR_IDX_BITS-1:0] ptr_idx;
    assign ptr_idx = {1'b1, index_out[IDX_SZ-1 -: PTR_IDX_BITS-1]};

    assign index_out[i_out] = plru_state[ptr_idx];
  end

endmodule

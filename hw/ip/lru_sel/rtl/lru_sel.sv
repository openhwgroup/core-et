// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module lru_sel #(
  parameter int unsigned NumClients = 8
) (
  input  logic                    clk_i,
  input  logic                    rst_ni,
  input  logic [NumClients-1:0]   access_entry_i,
  output logic [NumClients-1:0]   victim_o
);

  logic [NumClients-1:0][NumClients-1:0] prio_bits_d;
  logic [NumClients-1:0][NumClients-1:0] prio_bits_q;
  logic [NumClients-1:0]                 win_per_client;

  /* verilator lint_off SYNCASYNCNET */  // rst_ni is the module async reset and is also reused as the Verilator assertion guard below.
  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      prio_bits_q <= '0;
    end else begin
      prio_bits_q <= prio_bits_d;
    end
  end

  always_comb begin
    for (int i = 0; i < NumClients; i++) begin
      win_per_client[i] = 1'b1;
      for (int j = 0; j < i; j++) begin
        win_per_client[i] &= prio_bits_q[i][j];
      end
      for (int j = i + 1; j < NumClients; j++) begin
        win_per_client[i] &= ~prio_bits_q[j][i];
      end
      victim_o[i] = win_per_client[i];
    end
  end

  always_comb begin
    prio_bits_d = prio_bits_q;
    for (int i = 0; i < NumClients; i++) begin
      if (access_entry_i[i]) begin
        for (int j = 0; j < i; j++) begin
          prio_bits_d[i][j] = 1'b0;
        end
        for (int j = i + 1; j < NumClients; j++) begin
          prio_bits_d[j][i] = 1'b1;
        end
      end
    end
  end

  // synthesis translate_off
`ifdef VERILATOR
  always_ff @(posedge clk_i) begin
    if (rst_ni) begin
      assert ($onehot(victim_o) || (victim_o == '0))
        else $error("lru_sel produced a non-onehot victim");
    end
  end
  /* verilator lint_on SYNCASYNCNET */
`endif
  // synthesis translate_on

endmodule : lru_sel

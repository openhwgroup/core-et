// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module vpu_rf
  import minion_pkg::*;
  import vpu_pkg::*;
#(
  parameter bit UseMmi = UseMmiVpuRfDefault,
  /* verilator lint_off UNUSEDPARAM */  // The netlist/model split is kept explicit for the future hard-macro path.
  parameter bit UseMmiNetlist = UseMmiVpuRfNetlistDefault,
  /* verilator lint_on UNUSEDPARAM */
  localparam int unsigned RfEntries = NrThreadsDefault * VpuRegfileNum,
  localparam int unsigned RfAddrW   = $clog2(RfEntries)
) (
  input  logic [2:0]                     rd_en_i,
  input  logic [2:0]                     rd_thread_id_i,
  input  logic [2:0][FregAddrSize-1:0]   rd_addr_i,
  output logic [2:0][VpuDataSSz-1:0]     rd_data_o,
  input  logic                           clk_i,
  input  logic [1:0]                     wr_en_i,
  input  logic [1:0]                     wr_en_early_i,
  input  logic [1:0]                     wr_thread_id_i,
  input  logic [1:0]                     wr_mask_i,
  input  logic [1:0][FregAddrSize-1:0]   wr_addr_i,
  input  logic [1:0][VpuDataSSz-1:0]     wr_data_i
);

  logic [2:0][RfAddrW-1:0] rd_addr_q;
  logic                    wr_en_masked_0;
  logic                    wr_en_masked_1;
  /* verilator lint_off UNUSEDSIGNAL */  // The original MMI path ignores wr_en_early; keep the port for faithful interface compatibility.
  logic [1:0]              wr_en_early_unused;
  /* verilator lint_on UNUSEDSIGNAL */

  assign wr_en_masked_0 = wr_en_i[0] & wr_mask_i[0];
  assign wr_en_masked_1 = wr_en_i[1] & wr_mask_i[1];
  assign wr_en_early_unused = wr_en_early_i;

  always_ff @(posedge clk_i) begin
    if (rd_en_i[0]) rd_addr_q[0] <= {rd_thread_id_i[0], rd_addr_i[0]};
    if (rd_en_i[1]) rd_addr_q[1] <= {rd_thread_id_i[1], rd_addr_i[1]};
    if (rd_en_i[2]) rd_addr_q[2] <= {rd_thread_id_i[2], rd_addr_i[2]};
  end

  if (UseMmi) begin : gen_mmi
    // Faithful behavioral replacement for the original etcust_*_vpurf model.
    // This path samples write data on the rising edge and lets write port 1 win
    // on same-address collisions, exactly like the original model.
    logic [VpuDataSSz-1:0] rf_q [RfEntries];

    always_ff @(posedge clk_i) begin
      if (wr_en_masked_0) rf_q[{wr_thread_id_i[0], wr_addr_i[0]}] <= wr_data_i[0];
      if (wr_en_masked_1) rf_q[{wr_thread_id_i[1], wr_addr_i[1]}] <= wr_data_i[1];
    end

    assign rd_data_o[0] = rf_q[rd_addr_q[0]];
    assign rd_data_o[1] = rf_q[rd_addr_q[1]];
    assign rd_data_o[2] = rf_q[rd_addr_q[2]];
  end else begin : gen_latch
    prim_rf_3r2w_preview #(
      .Width        (VpuDataSSz),
      .Entries      (RfEntries),
      .Level2CkGate (1)
    ) u_rf (
      .preview_clk_i        (clk_i),
      .rf_clk_i             (clk_i),
      .rd_addr_a_i       (rd_addr_q[0]),
      .rd_data_a_o       (rd_data_o[0]),
      .rd_addr_b_i       (rd_addr_q[1]),
      .rd_data_b_o       (rd_data_o[1]),
      .rd_addr_c_i       (rd_addr_q[2]),
      .rd_data_c_o       (rd_data_o[2]),
      .wr_en_a_i         (wr_en_masked_0),
      .wr_addr_a_i       ({wr_thread_id_i[0], wr_addr_i[0]}),
      .wr_data_a_en_1p_next_i (wr_en_early_i[0]),
      .wr_data_a_i       (wr_data_i[0]),
      .wr_en_b_i         (wr_en_masked_1),
      .wr_addr_b_i       ({wr_thread_id_i[1], wr_addr_i[1]}),
      .wr_data_b_en_1p_next_i (wr_en_early_i[1]),
      .wr_data_b_i       (wr_data_i[1])
    );
  end

endmodule : vpu_rf

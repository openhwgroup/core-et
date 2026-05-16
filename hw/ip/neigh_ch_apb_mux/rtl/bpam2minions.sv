// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module bpam2minions #(
  parameter int unsigned AdInWidth  = 19,
  parameter int unsigned AdOutWidth = 10,
  parameter int unsigned DataWidth  = 32,
  parameter int unsigned NrMinions  = minion_pkg::ShireCoopMinMaskSize
) (
  input  logic                                    clk_i,
  input  logic                                    rst_ni,

  // APB request/response toward BPAM / neighborhood APB mux.
  /* verilator lint_off UNUSEDSIGNAL */  // Original bpam2minions ignores address bit 15 while using the surrounding minion/dest fields.
  input  logic [AdInWidth-1:0]                    req_apb_paddr_i,
  /* verilator lint_on UNUSEDSIGNAL */
  input  logic                                    req_apb_pwrite_i,
  input  logic                                    req_apb_psel_i,
  input  logic                                    req_apb_penable_i,
  input  logic [DataWidth-1:0]                    req_apb_pwdata_i,

  output logic                                    rsp_apb_pready_o,
  output logic [DataWidth-1:0]                    rsp_apb_prdata_o,
  output logic                                    rsp_apb_pslverr_o,

  // APB request/response toward the selected minion.
  output logic [AdOutWidth-1:0]                   minion_apb_paddr_o,
  output logic                                    minion_apb_pwrite_o,
  output logic [NrMinions-1:0]                    minion_apb_psel_o,
  output logic                                    minion_apb_penable_o,
  output logic [DataWidth-1:0]                    minion_apb_pwdata_o,

  input  logic [NrMinions-1:0]                    minion_apb_pready_i,
  input  logic [NrMinions-1:0][DataWidth-1:0]     minion_apb_prdata_i,
  input  logic [NrMinions-1:0]                    minion_apb_pslverr_i
);

  localparam int unsigned MinionSelWidth = (NrMinions > 1) ? $clog2(NrMinions) : 1;

  logic                         req_start;
  logic [MinionSelWidth-1:0]    slave_sel;
  logic [AdOutWidth-1:0]        minion_apb_enc_addr;

  logic [MinionSelWidth-1:0]    minion_sel;
  logic [NrMinions-1:0]         minion_apb_psel_d;
  logic                         minion_done;
  logic                         minion_start_q;
  logic                         rsp_apb_pready_d;
  logic                         minion_apb_penable_d;

  always_comb begin
    req_start = req_apb_psel_i && !req_apb_penable_i;
    slave_sel = req_apb_paddr_i[neigh_ch_apb_mux_pkg::NeighDmApbAddrMinMsb:
                                neigh_ch_apb_mux_pkg::NeighDmApbAddrMinLsb];
    minion_apb_enc_addr = {
      req_apb_paddr_i[neigh_ch_apb_mux_pkg::NeighDmApbAddrDestMsb:
                      neigh_ch_apb_mux_pkg::NeighDmApbAddrDestLsb],
      req_apb_paddr_i[neigh_ch_apb_mux_pkg::NeighDmApbAddrPpMsb:
                      neigh_ch_apb_mux_pkg::NeighDmApbAddrPpLsb],
      req_apb_paddr_i[neigh_ch_apb_mux_pkg::NeighDmApbAddrThreadMsb:
                      neigh_ch_apb_mux_pkg::NeighDmApbAddrThreadLsb],
      req_apb_paddr_i[neigh_ch_apb_mux_pkg::NeighDmApbAddrRegMsb:
                      neigh_ch_apb_mux_pkg::NeighDmApbAddrRegLsb]
    };
  end

  always_comb begin
    minion_sel = req_apb_paddr_i[neigh_ch_apb_mux_pkg::NeighDmApbAddrMinMsb:
                                 neigh_ch_apb_mux_pkg::NeighDmApbAddrMinLsb];
    minion_done = minion_apb_penable_o && minion_apb_pready_i[minion_sel];
    minion_apb_psel_d = '0;
    minion_apb_psel_d[slave_sel] = !minion_done;
  end

  always_comb begin
    if (minion_done) begin
      rsp_apb_pready_d = 1'b1;
    end else if (rsp_apb_pready_o) begin
      rsp_apb_pready_d = 1'b0;
    end else begin
      rsp_apb_pready_d = rsp_apb_pready_o;
    end

    if (minion_start_q) begin
      minion_apb_penable_d = 1'b1;
    end else if (minion_done) begin
      minion_apb_penable_d = 1'b0;
    end else begin
      minion_apb_penable_d = minion_apb_penable_o;
    end
  end

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      minion_apb_psel_o    <= '0;
      rsp_apb_pready_o     <= 1'b0;
      minion_start_q       <= 1'b0;
      minion_apb_penable_o <= 1'b0;
    end else begin
      if (req_start || minion_done) begin
        minion_apb_psel_o <= minion_apb_psel_d;
      end
      rsp_apb_pready_o     <= rsp_apb_pready_d;
      minion_start_q       <= req_start;
      minion_apb_penable_o <= minion_apb_penable_d;
    end
  end

  // Original `EN_FF` instances have no reset; preserve that unreset state.
  always_ff @(posedge clk_i) begin
    if (minion_done) begin
      rsp_apb_prdata_o  <= minion_apb_prdata_i[minion_sel];
      rsp_apb_pslverr_o <= minion_apb_pslverr_i[minion_sel];
    end
    if (req_start) begin
      minion_apb_paddr_o  <= minion_apb_enc_addr;
      minion_apb_pwrite_o <= req_apb_pwrite_i;
      minion_apb_pwdata_o <= req_apb_pwdata_i;
    end
  end

endmodule : bpam2minions

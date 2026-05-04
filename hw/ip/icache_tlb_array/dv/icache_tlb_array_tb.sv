// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Unit-test wrapper for icache_tlb_array.

/* verilator lint_off SYNCASYNCNET */  // Testbench reset drives translated async-assert/sync-deassert flops and original-style sync logic.
module icache_tlb_array_tb
  import minion_pkg::*;
  import icache_pkg::*;
(
  input  logic                             clk_i,
  input  logic                             rst_ni,
  input  logic [1:0]                       vmspagesize_i,
  input  logic                             coop_mode_i,
  input  logic                             req_min_id_i,
  input  logic [1:0]                       req_prv_i,
  input  logic                             req_mprv_i,
  input  logic [1:0]                       req_mpp_i,
  input  logic                             req_sum_i,
  input  logic                             req_mxr_i,
  input  logic                             req_debug_i,
  input  logic [VaSize-1:VaUntransSize]    req_vpn_i,
  input  logic                             req_passthrough_i,
  input  logic                             req_msb_err_i,
  input  logic                             req_valid_i,
  input  logic [CsrSatpModeSz-1:0]         satp_mode0_i,
  input  logic [PaTransSize-1:0]           satp_ppn0_i,
  input  logic [CsrSatpModeSz-1:0]         satp_mode1_i,
  input  logic [PaTransSize-1:0]           satp_ppn1_i,
  input  logic [CsrSatpModeSz-1:0]         matp_mode0_i,
  input  logic [PaTransSize-1:0]           matp_ppn0_i,
  input  logic [CsrSatpModeSz-1:0]         matp_mode1_i,
  input  logic [PaTransSize-1:0]           matp_ppn1_i,
  input  logic [1:0]                       tlb_invalidate_i,
  input  logic                             ptw_req_ready_i,
  input  logic                             ptw_resp_canceled_req_i,
  input  logic                             ptw_resp_access_fault_i,
  input  logic [7:0]                       ptw_resp_rsvd_for_hw_i,
  input  logic [CsrSatpPpnSz-1:0]          ptw_resp_ppn_i,
  input  logic [1:0]                       ptw_resp_rsvd_for_sw_i,
  input  logic                             ptw_resp_d_i,
  input  logic                             ptw_resp_a_i,
  input  logic                             ptw_resp_g_i,
  input  logic                             ptw_resp_u_i,
  input  logic                             ptw_resp_x_i,
  input  logic                             ptw_resp_w_i,
  input  logic                             ptw_resp_r_i,
  input  logic                             ptw_resp_v_i,
  input  logic                             ptw_resp_valid_i,
  output logic                             resp_fill_pending_o,
  output logic                             resp_miss_o,
  output logic [PaSize-1:PaUntransSize]    resp_ppn_o,
  output logic                             resp_xcpt_if_o,
  output logic                             resp_access_fault_o,
  output logic [CsrSatpModeSz-1:0]         ptw_req_satp_mode_o,
  output logic [PaTransSize-1:0]           ptw_req_satp_ppn_o,
  output logic [1:0]                       ptw_req_prv_o,
  output logic [VaSize-1:VaUntransSize]    ptw_req_addr_o,
  output logic                             ptw_req_valid_o,
  output logic                             ptw_invalidate_o
);

  localparam int unsigned NrMinions = 2;

  tlb_entry_type     vmspagesize;
  icache_tlb_req_t   req_data;
  icache_tlb_resp_t  resp_data;
  minion_satp_info   satp_info [NrMinions-1:0];
  minion_satp_info   matp_info [NrMinions-1:0];
  minion_ptw_req     ptw_req_data;
  minion_ptw_pte     ptw_resp_data;
  /* verilator lint_off UNUSEDSIGNAL */  // Lint anchor keeps the generic clock-gate seam reachable when this wrapper includes child tech dependencies that are inactive in this parameterization.
  logic              lint_clk_gate_o;
  /* verilator lint_on UNUSEDSIGNAL */

  prim_clk_gate u_lint_clk_gate (
    .clk_i (clk_i),
    .en_i  (1'b0),
    .dft_i ('0),
    .clk_o (lint_clk_gate_o)
  );

  always_comb begin
    vmspagesize = tlb_entry_type'(vmspagesize_i);

    req_data = '0;
    req_data.status.prv = req_prv_i;
    req_data.status.mprv = req_mprv_i;
    req_data.status.mpp = req_mpp_i;
    req_data.status.sum = req_sum_i;
    req_data.status.mxr = req_mxr_i;
    req_data.status.debug = req_debug_i;
    req_data.vpn = req_vpn_i;
    req_data.passthrough = req_passthrough_i;
    req_data.msb_err = req_msb_err_i;

    satp_info[0] = '{mode: satp_mode0_i, ppn: satp_ppn0_i};
    satp_info[1] = '{mode: satp_mode1_i, ppn: satp_ppn1_i};
    matp_info[0] = '{mode: matp_mode0_i, ppn: matp_ppn0_i};
    matp_info[1] = '{mode: matp_mode1_i, ppn: matp_ppn1_i};

    ptw_resp_data = '0;
    ptw_resp_data.canceled_req = ptw_resp_canceled_req_i;
    ptw_resp_data.access_fault = ptw_resp_access_fault_i;
    ptw_resp_data.rsvd_for_hw = ptw_resp_rsvd_for_hw_i;
    ptw_resp_data.ppn = ptw_resp_ppn_i;
    ptw_resp_data.rsvd_for_sw = ptw_resp_rsvd_for_sw_i;
    ptw_resp_data.d = ptw_resp_d_i;
    ptw_resp_data.a = ptw_resp_a_i;
    ptw_resp_data.g = ptw_resp_g_i;
    ptw_resp_data.u = ptw_resp_u_i;
    ptw_resp_data.x = ptw_resp_x_i;
    ptw_resp_data.w = ptw_resp_w_i;
    ptw_resp_data.r = ptw_resp_r_i;
    ptw_resp_data.v = ptw_resp_v_i;

    resp_fill_pending_o = resp_data.fill_pending;
    resp_miss_o = resp_data.miss;
    resp_ppn_o = resp_data.ppn;
    resp_xcpt_if_o = resp_data.xcpt_if;
    resp_access_fault_o = resp_data.access_fault;
    ptw_req_satp_mode_o = ptw_req_data.satp_mode;
    ptw_req_satp_ppn_o = ptw_req_data.satp_ppn;
    ptw_req_prv_o = ptw_req_data.prv;
    ptw_req_addr_o = ptw_req_data.addr;
  end

  icache_tlb_array #(
    .Entries   (8),
    .NrMinions (NrMinions)
  ) u_dut (
    .clk_i,
    .rst_ni,
    .esr_vmspagesize_i      (vmspagesize),
    .esr_shire_coop_mode_i  (coop_mode_i),
    .req_min_id_i,
    .req_data_i             (req_data),
    .req_valid_i,
    .resp_data_o            (resp_data),
    .satp_info_i            (satp_info),
    .matp_info_i            (matp_info),
    .tlb_invalidate_i,
    .ptw_req_data_o         (ptw_req_data),
    .ptw_req_valid_o,
    .ptw_req_ready_i,
    .ptw_invalidate_o,
    .ptw_resp_valid_i,
    .ptw_resp_data_i        (ptw_resp_data)
  );

endmodule : icache_tlb_array_tb
/* verilator lint_on SYNCASYNCNET */

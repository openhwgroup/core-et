// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation wrapper: original icache_tlb_array vs new icache_tlb_array.

`include "soc.vh"

module cosim_icache_tlb_array_tb
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
  output logic                             new_resp_fill_pending_o,
  output logic                             new_resp_miss_o,
  output logic [PaSize-1:PaUntransSize]    new_resp_ppn_o,
  output logic                             new_resp_xcpt_if_o,
  output logic                             new_resp_access_fault_o,
  output logic [CsrSatpModeSz-1:0]         new_ptw_req_satp_mode_o,
  output logic [PaTransSize-1:0]           new_ptw_req_satp_ppn_o,
  output logic [1:0]                       new_ptw_req_prv_o,
  output logic [VaSize-1:VaUntransSize]    new_ptw_req_addr_o,
  output logic                             new_ptw_req_valid_o,
  output logic                             new_ptw_invalidate_o,
  output logic                             orig_resp_fill_pending_o,
  output logic                             orig_resp_miss_o,
  output logic [PaSize-1:PaUntransSize]    orig_resp_ppn_o,
  output logic                             orig_resp_xcpt_if_o,
  output logic                             orig_resp_access_fault_o,
  output logic [CsrSatpModeSz-1:0]         orig_ptw_req_satp_mode_o,
  output logic [PaTransSize-1:0]           orig_ptw_req_satp_ppn_o,
  output logic [1:0]                       orig_ptw_req_prv_o,
  output logic [VaSize-1:VaUntransSize]    orig_ptw_req_addr_o,
  output logic                             orig_ptw_req_valid_o,
  output logic                             orig_ptw_invalidate_o
);

  localparam int unsigned NrMinions = 2;

  logic             reset_orig;
  tlb_entry_type    new_vmspagesize;
  icache_tlb_req_t  new_req_data;
  icache_tlb_resp_t new_resp_data;
  minion_satp_info  new_satp_info [NrMinions-1:0];
  minion_satp_info  new_matp_info [NrMinions-1:0];
  minion_ptw_req    new_ptw_req_data;
  minion_ptw_pte    new_ptw_resp_data;

  tlb_entry_type    orig_vmspagesize;
  icache_tlb_req    orig_req_data;
  icache_tlb_resp   orig_resp_data;
  minion_satp_info  orig_satp_info [NrMinions-1:0];
  minion_satp_info  orig_matp_info [NrMinions-1:0];
  logic [(NrMinions * $bits(minion_satp_info))-1:0] orig_satp_info_flat;
  logic [(NrMinions * $bits(minion_satp_info))-1:0] orig_matp_info_flat;
  minion_ptw_req    orig_ptw_req_data;
  minion_ptw_pte    orig_ptw_resp_data;

  always_comb begin
    reset_orig = ~rst_ni;

    new_vmspagesize = tlb_entry_type'(vmspagesize_i);
    orig_vmspagesize = tlb_entry_type'(vmspagesize_i);

    new_req_data = '0;
    new_req_data.status.prv = req_prv_i;
    new_req_data.status.mprv = req_mprv_i;
    new_req_data.status.mpp = req_mpp_i;
    new_req_data.status.sum = req_sum_i;
    new_req_data.status.mxr = req_mxr_i;
    new_req_data.status.debug = req_debug_i;
    new_req_data.vpn = req_vpn_i;
    new_req_data.passthrough = req_passthrough_i;
    new_req_data.msb_err = req_msb_err_i;

    orig_req_data = '{
      status: '{
        prv: req_prv_i,
        mprv: req_mprv_i,
        mpp: req_mpp_i,
        sum: req_sum_i,
        mxr: req_mxr_i,
        debug: req_debug_i
      },
      vpn: req_vpn_i,
      passthrough: req_passthrough_i,
      msb_err: req_msb_err_i
    };

    new_satp_info[0] = '{mode: satp_mode0_i, ppn: satp_ppn0_i};
    new_satp_info[1] = '{mode: satp_mode1_i, ppn: satp_ppn1_i};
    new_matp_info[0] = '{mode: matp_mode0_i, ppn: matp_ppn0_i};
    new_matp_info[1] = '{mode: matp_mode1_i, ppn: matp_ppn1_i};
    orig_satp_info[0] = '{mode: satp_mode0_i, ppn: satp_ppn0_i};
    orig_satp_info[1] = '{mode: satp_mode1_i, ppn: satp_ppn1_i};
    orig_matp_info[0] = '{mode: matp_mode0_i, ppn: matp_ppn0_i};
    orig_matp_info[1] = '{mode: matp_mode1_i, ppn: matp_ppn1_i};
    orig_satp_info_flat = {orig_satp_info[1], orig_satp_info[0]};
    orig_matp_info_flat = {orig_matp_info[1], orig_matp_info[0]};

    new_ptw_resp_data = '0;
    new_ptw_resp_data.canceled_req = ptw_resp_canceled_req_i;
    new_ptw_resp_data.access_fault = ptw_resp_access_fault_i;
    new_ptw_resp_data.rsvd_for_hw = ptw_resp_rsvd_for_hw_i;
    new_ptw_resp_data.ppn = ptw_resp_ppn_i;
    new_ptw_resp_data.rsvd_for_sw = ptw_resp_rsvd_for_sw_i;
    new_ptw_resp_data.d = ptw_resp_d_i;
    new_ptw_resp_data.a = ptw_resp_a_i;
    new_ptw_resp_data.g = ptw_resp_g_i;
    new_ptw_resp_data.u = ptw_resp_u_i;
    new_ptw_resp_data.x = ptw_resp_x_i;
    new_ptw_resp_data.w = ptw_resp_w_i;
    new_ptw_resp_data.r = ptw_resp_r_i;
    new_ptw_resp_data.v = ptw_resp_v_i;

    orig_ptw_resp_data = '{
      canceled_req: ptw_resp_canceled_req_i,
      access_fault: ptw_resp_access_fault_i,
      rsvd_for_hw: ptw_resp_rsvd_for_hw_i,
      ppn: ptw_resp_ppn_i,
      rsvd_for_sw: ptw_resp_rsvd_for_sw_i,
      d: ptw_resp_d_i,
      a: ptw_resp_a_i,
      g: ptw_resp_g_i,
      u: ptw_resp_u_i,
      x: ptw_resp_x_i,
      w: ptw_resp_w_i,
      r: ptw_resp_r_i,
      v: ptw_resp_v_i
    };

    new_resp_fill_pending_o = new_resp_data.fill_pending;
    new_resp_miss_o = new_resp_data.miss;
    new_resp_ppn_o = new_resp_data.ppn;
    new_resp_xcpt_if_o = new_resp_data.xcpt_if;
    new_resp_access_fault_o = new_resp_data.access_fault;
    new_ptw_req_satp_mode_o = new_ptw_req_data.satp_mode;
    new_ptw_req_satp_ppn_o = new_ptw_req_data.satp_ppn;
    new_ptw_req_prv_o = new_ptw_req_data.prv;
    new_ptw_req_addr_o = new_ptw_req_data.addr;

    orig_resp_fill_pending_o = orig_resp_data.fill_pending;
    orig_resp_miss_o = orig_resp_data.miss;
    orig_resp_ppn_o = orig_resp_data.ppn;
    orig_resp_xcpt_if_o = orig_resp_data.xcpt_if;
    orig_resp_access_fault_o = orig_resp_data.access_fault;
    orig_ptw_req_satp_mode_o = orig_ptw_req_data.satp_mode;
    orig_ptw_req_satp_ppn_o = orig_ptw_req_data.satp_ppn;
    orig_ptw_req_prv_o = orig_ptw_req_data.prv;
    orig_ptw_req_addr_o = orig_ptw_req_data.addr;
  end

  icache_tlb_array #(
    .Entries   (8),
    .NrMinions (NrMinions)
  ) u_new (
    .clk_i,
    .rst_ni,
    .esr_vmspagesize_i      (new_vmspagesize),
    .esr_shire_coop_mode_i  (coop_mode_i),
    .req_min_id_i,
    .req_data_i             (new_req_data),
    .req_valid_i,
    .resp_data_o            (new_resp_data),
    .satp_info_i            (new_satp_info),
    .matp_info_i            (new_matp_info),
    .tlb_invalidate_i,
    .ptw_req_data_o         (new_ptw_req_data),
    .ptw_req_valid_o        (new_ptw_req_valid_o),
    .ptw_req_ready_i,
    .ptw_invalidate_o       (new_ptw_invalidate_o),
    .ptw_resp_valid_i,
    .ptw_resp_data_i        (new_ptw_resp_data)
  );

  icache_tlb_array_orig #(
    .ENTRIES      (8),
    .NR_MINIONS   (NrMinions),
    .NR_MINIONS_L (1)
  ) u_orig (
    .clock              (clk_i),
    .reset              (reset_orig),
    .esr_vmspagesize    (orig_vmspagesize),
    .esr_shire_coop_mode(coop_mode_i),
    .req_min_id         (req_min_id_i),
    .req_data           (orig_req_data),
    .req_valid          (req_valid_i),
    .resp_data          (orig_resp_data),
    .satp_info          (orig_satp_info_flat),
    .matp_info          (orig_matp_info_flat),
    .tlb_invalidate     (tlb_invalidate_i),
    .ptw_req_data       (orig_ptw_req_data),
    .ptw_req_valid      (orig_ptw_req_valid_o),
    .ptw_req_ready      (ptw_req_ready_i),
    .ptw_invalidate     (orig_ptw_invalidate_o),
    .ptw_resp_valid     (ptw_resp_valid_i),
    .ptw_resp_data      (orig_ptw_resp_data)
  );

endmodule : cosim_icache_tlb_array_tb

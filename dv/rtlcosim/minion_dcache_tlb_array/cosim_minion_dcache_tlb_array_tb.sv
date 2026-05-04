// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation wrapper: original dcache_tlb_array vs new minion_dcache_tlb_array.

`include "soc.vh"

module cosim_minion_dcache_tlb_array_tb
  import dft_pkg::*;
  import minion_pkg::*;
  import minion_dcache_pkg::*;
(
  input  logic                          clk_i,
  input  logic                          rst_ni,
  input  logic                          dft_scanmode_i,
  input  logic [1:0]                    esr_vmspagesize_i,
  input  logic [1:0]                    req_prv_i,
  input  logic                          req_mprv_i,
  input  logic [1:0]                    req_mpp_i,
  input  logic                          req_sum_i,
  input  logic                          req_mxr_i,
  input  logic                          req_debug_i,
  input  logic [VaSize-1:VaUntransSize] req_vpn_i,
  input  logic                          req_passthrough_i,
  input  logic                          req_store_i,
  input  logic                          req_msb_err_i,
  input  logic                          req_valid_i,
  input  logic [CsrSatpModeSz-1:0]      satp_mode_i,
  input  logic [PaTransSize-1:0]        satp_ppn_i,
  input  logic [CsrSatpModeSz-1:0]      matp_mode_i,
  input  logic [PaTransSize-1:0]        matp_ppn_i,
  input  logic                          satp_info_en_i,
  input  logic                          matp_info_en_i,
  input  logic                          tlb_invalidate_i,
  input  logic                          ptw_req_ready_i,
  input  logic                          ptw_resp_canceled_req_i,
  input  logic                          ptw_resp_access_fault_i,
  input  logic [7:0]                    ptw_resp_rsvd_for_hw_i,
  input  logic [CsrSatpPpnSz-1:0]       ptw_resp_ppn_i,
  input  logic [1:0]                    ptw_resp_rsvd_for_sw_i,
  input  logic                          ptw_resp_d_i,
  input  logic                          ptw_resp_a_i,
  input  logic                          ptw_resp_g_i,
  input  logic                          ptw_resp_u_i,
  input  logic                          ptw_resp_x_i,
  input  logic                          ptw_resp_w_i,
  input  logic                          ptw_resp_r_i,
  input  logic                          ptw_resp_v_i,
  input  logic                          ptw_resp_valid_i,
  output logic                          new_resp_fill_pending_o,
  output logic                          new_resp_miss_o,
  output logic [PaSize-1:PaUntransSize] new_resp_ppn_o,
  output logic                          new_resp_xcpt_ld_o,
  output logic                          new_resp_xcpt_st_o,
  output logic                          new_resp_access_fault_o,
  output logic [CsrSatpModeSz-1:0]      new_ptw_req_satp_mode_o,
  output logic [PaTransSize-1:0]        new_ptw_req_satp_ppn_o,
  output logic [1:0]                    new_ptw_req_prv_o,
  output logic [VaSize-1:VaUntransSize] new_ptw_req_addr_o,
  output logic                          new_ptw_req_valid_o,
  output logic                          new_vm_enabled_o,
  output logic                          orig_resp_fill_pending_o,
  output logic                          orig_resp_miss_o,
  output logic [PaSize-1:PaUntransSize] orig_resp_ppn_o,
  output logic                          orig_resp_xcpt_ld_o,
  output logic                          orig_resp_xcpt_st_o,
  output logic                          orig_resp_access_fault_o,
  output logic [CsrSatpModeSz-1:0]      orig_ptw_req_satp_mode_o,
  output logic [PaTransSize-1:0]        orig_ptw_req_satp_ppn_o,
  output logic [1:0]                    orig_ptw_req_prv_o,
  output logic [VaSize-1:VaUntransSize] orig_ptw_req_addr_o,
  output logic                          orig_ptw_req_valid_o,
  output logic                          orig_vm_enabled_o
);

  logic               reset_orig;
  dft_t               dft;
  dcache_tlb_req_t    new_req_data;
  dcache_tlb_resp_t   new_resp_data;
  minion_satp_info    new_satp_info;
  minion_satp_info    new_matp_info;
  minion_ptw_req      new_ptw_req_data;
  minion_ptw_pte      new_ptw_resp_data;
  dcache_tlb_req      orig_req_data;
  dcache_tlb_resp     orig_resp_data;
  minion_satp_info    orig_satp_info;
  minion_satp_info    orig_matp_info;
  minion_ptw_req      orig_ptw_req_data;
  minion_ptw_pte      orig_ptw_resp_data;

  always_comb begin
    dft = '0;
    dft.scanmode = dft_scanmode_i;
    reset_orig = ~rst_ni;

    new_req_data = '0;
    new_req_data.status.prv = req_prv_i;
    new_req_data.status.mprv = req_mprv_i;
    new_req_data.status.mpp = req_mpp_i;
    new_req_data.status.sum = req_sum_i;
    new_req_data.status.mxr = req_mxr_i;
    new_req_data.status.debug = req_debug_i;
    new_req_data.vpn = req_vpn_i;
    new_req_data.passthrough = req_passthrough_i;
    new_req_data.store = req_store_i;
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
      store: req_store_i,
      msb_err: req_msb_err_i
    };

    new_satp_info = '{mode: satp_mode_i, ppn: satp_ppn_i};
    new_matp_info = '{mode: matp_mode_i, ppn: matp_ppn_i};
    orig_satp_info = '{mode: satp_mode_i, ppn: satp_ppn_i};
    orig_matp_info = '{mode: matp_mode_i, ppn: matp_ppn_i};

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
    new_resp_xcpt_ld_o = new_resp_data.xcpt_ld;
    new_resp_xcpt_st_o = new_resp_data.xcpt_st;
    new_resp_access_fault_o = new_resp_data.access_fault;
    new_ptw_req_satp_mode_o = new_ptw_req_data.satp_mode;
    new_ptw_req_satp_ppn_o = new_ptw_req_data.satp_ppn;
    new_ptw_req_prv_o = new_ptw_req_data.prv;
    new_ptw_req_addr_o = new_ptw_req_data.addr;

    orig_resp_fill_pending_o = orig_resp_data.fill_pending;
    orig_resp_miss_o = orig_resp_data.miss;
    orig_resp_ppn_o = orig_resp_data.ppn;
    orig_resp_xcpt_ld_o = orig_resp_data.xcpt_ld;
    orig_resp_xcpt_st_o = orig_resp_data.xcpt_st;
    orig_resp_access_fault_o = orig_resp_data.access_fault;
    orig_ptw_req_satp_mode_o = orig_ptw_req_data.satp_mode;
    orig_ptw_req_satp_ppn_o = orig_ptw_req_data.satp_ppn;
    orig_ptw_req_prv_o = orig_ptw_req_data.prv;
    orig_ptw_req_addr_o = orig_ptw_req_data.addr;
  end

  minion_dcache_tlb_array u_new (
    .clk_i,
    .rst_ni,
    .dft_i             (dft),
    .esr_vmspagesize_i (tlb_entry_type'(esr_vmspagesize_i)),
    .req_data_i        (new_req_data),
    .req_valid_i,
    .resp_data_o       (new_resp_data),
    .satp_info_i       (new_satp_info),
    .matp_info_i       (new_matp_info),
    .satp_info_en_i,
    .matp_info_en_i,
    .tlb_invalidate_i,
    .ptw_req_data_o    (new_ptw_req_data),
    .ptw_req_valid_o   (new_ptw_req_valid_o),
    .ptw_req_ready_i,
    .ptw_resp_data_i   (new_ptw_resp_data),
    .ptw_resp_valid_i,
    .vm_enabled_o      (new_vm_enabled_o)
  );

  dcache_tlb_array u_orig (
    .clock          (clk_i),
    .reset          (reset_orig),
    .esr_vmspagesize(tlb_entry_type'(esr_vmspagesize_i)),
    .req_data       (orig_req_data),
    .req_valid      (req_valid_i),
    .resp_data      (orig_resp_data),
    .satp_info      (orig_satp_info),
    .matp_info      (orig_matp_info),
    .satp_info_en   (satp_info_en_i),
    .matp_info_en   (matp_info_en_i),
    .tlb_invalidate (tlb_invalidate_i),
    .ptw_req_data   (orig_ptw_req_data),
    .ptw_req_valid  (orig_ptw_req_valid_o),
    .ptw_req_ready  (ptw_req_ready_i),
    .ptw_resp_data  (orig_ptw_resp_data),
    .ptw_resp_valid (ptw_resp_valid_i),
    .vm_enabled     (orig_vm_enabled_o)
  );

endmodule

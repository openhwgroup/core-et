// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation wrapper: original tlb_top vs new minion_tlb.

`include "soc.vh"

module cosim_minion_tlb_tb
  import dft_pkg::*;
(
  input  logic                          clk_i,
  input  logic                          rst_ni,
  input  logic                          dft_scanmode_i,
  input  logic [1:0]                    vmspagesize_i,
  input  logic                          coop_mode_i,
  input  logic                          req_min_id_i,
  input  logic [1:0]                    req_prv_i,
  input  logic                          req_mprv_i,
  input  logic [1:0]                    req_mpp_i,
  input  logic                          req_sum_i,
  input  logic                          req_mxr_i,
  input  logic                          req_debug_i,
  input  logic [35:0]                   req_vpn_i,
  input  logic                          req_passthrough_i,
  input  logic                          req_instruction_i,
  input  logic                          req_store_i,
  input  logic                          req_msb_err_i,
  input  logic                          req_valid_i,
  input  logic [3:0]                    satp_mode0_i,
  input  logic [27:0]                   satp_ppn0_i,
  input  logic [3:0]                    satp_mode1_i,
  input  logic [27:0]                   satp_ppn1_i,
  input  logic [3:0]                    matp_mode0_i,
  input  logic [27:0]                   matp_ppn0_i,
  input  logic [3:0]                    matp_mode1_i,
  input  logic [27:0]                   matp_ppn1_i,
  input  logic [1:0]                    tlb_invalidate_i,
  input  logic                          ptw_req_ready_i,
  input  logic                          ptw_resp_canceled_req_i,
  input  logic                          ptw_resp_access_fault_i,
  input  logic [7:0]                    ptw_resp_rsvd_for_hw_i,
  input  logic [43:0]                   ptw_resp_ppn_i,
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
  output logic [27:0]                   new_resp_ppn_o,
  output logic                          new_resp_xcpt_ld_o,
  output logic                          new_resp_xcpt_st_o,
  output logic                          new_resp_xcpt_if_o,
  output logic                          new_resp_access_fault_o,
  output logic [3:0]                    new_ptw_req_satp_mode_o,
  output logic [27:0]                   new_ptw_req_satp_ppn_o,
  output logic [1:0]                    new_ptw_req_prv_o,
  output logic [35:0]                   new_ptw_req_addr_o,
  output logic                          new_ptw_req_valid_o,
  output logic                          new_ptw_invalidate_o,
  output logic                          new_vm_enabled_o,
  output logic                          orig_resp_fill_pending_o,
  output logic                          orig_resp_miss_o,
  output logic [27:0]                   orig_resp_ppn_o,
  output logic                          orig_resp_xcpt_ld_o,
  output logic                          orig_resp_xcpt_st_o,
  output logic                          orig_resp_xcpt_if_o,
  output logic                          orig_resp_access_fault_o,
  output logic [3:0]                    orig_ptw_req_satp_mode_o,
  output logic [27:0]                   orig_ptw_req_satp_ppn_o,
  output logic [1:0]                    orig_ptw_req_prv_o,
  output logic [35:0]                   orig_ptw_req_addr_o,
  output logic                          orig_ptw_req_valid_o,
  output logic                          orig_ptw_invalidate_o,
  output logic                          orig_vm_enabled_o,
  output logic [1:0]                    new_state_o,
  output logic [1:0]                    orig_state_o,
  output logic                          new_ptw_req_valid_q_o,
  output logic                          orig_ptw_req_valid_q_o,
  output logic                          new_ptw_resp_valid_q_o,
  output logic                          orig_ptw_resp_valid_q_o,
  output logic                          new_rf_latch_wr_en_o,
  output logic                          orig_rf_latch_wr_en_o,
  output logic                          new_rf_latch_wr_en_early_o,
  output logic                          orig_rf_latch_wr_en_early_o,
  output logic [0:0]                    new_req_min_id_q_o,
  output logic [0:0]                    orig_req_min_id_q_o,
  output logic [35:0]                   new_req_vpn_q_o,
  output logic [35:0]                   orig_req_vpn_q_o,
  output logic [7:0]                    new_req_tag_q_o,
  output logic [7:0]                    orig_req_tag_q_o,
  output logic [2:0]                    new_tlb_cache_waddr_o,
  output logic [2:0]                    orig_tlb_cache_waddr_o,
  output logic [2:0]                    new_tlb_cache_raddr_o,
  output logic [2:0]                    orig_tlb_cache_raddr_o
);

  localparam int unsigned NrMinions = 2;

  dft_t                        dft;
  logic                        reset_orig;

  minion_pkg::tlb_entry_type   new_vmspagesize;
  minion_pkg::tlb_req          new_req_data;
  minion_pkg::tlb_resp         new_resp_data;
  minion_pkg::minion_satp_info new_satp_info [NrMinions-1:0];
  minion_pkg::minion_satp_info new_matp_info [NrMinions-1:0];
  minion_pkg::minion_ptw_req   new_ptw_req_data;
  minion_pkg::minion_ptw_pte   new_ptw_resp_data;

  tlb_entry_type               orig_vmspagesize;
  tlb_req                      orig_req_data;
  tlb_resp                     orig_resp_data;
  minion_satp_info             orig_satp_info [NrMinions-1:0];
  minion_satp_info             orig_matp_info [NrMinions-1:0];
  logic [(NrMinions * $bits(minion_satp_info))-1:0] orig_satp_info_flat;
  logic [(NrMinions * $bits(minion_satp_info))-1:0] orig_matp_info_flat;
  minion_ptw_req               orig_ptw_req_data;
  minion_ptw_pte               orig_ptw_resp_data;

  always_comb begin
    dft = '0;
    dft.scanmode = dft_scanmode_i;
    reset_orig = ~rst_ni;

    new_vmspagesize = minion_pkg::tlb_entry_type'(vmspagesize_i);
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
    new_req_data.instruction = req_instruction_i;
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
      instruction: req_instruction_i,
      store: req_store_i,
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
    new_resp_xcpt_ld_o = new_resp_data.xcpt_ld;
    new_resp_xcpt_st_o = new_resp_data.xcpt_st;
    new_resp_xcpt_if_o = new_resp_data.xcpt_if;
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
    orig_resp_xcpt_if_o = orig_resp_data.xcpt_if;
    orig_resp_access_fault_o = orig_resp_data.access_fault;
    orig_ptw_req_satp_mode_o = orig_ptw_req_data.satp_mode;
    orig_ptw_req_satp_ppn_o = orig_ptw_req_data.satp_ppn;
    orig_ptw_req_prv_o = orig_ptw_req_data.prv;
    orig_ptw_req_addr_o = orig_ptw_req_data.addr;
  end

  minion_tlb #(
    .Entries   (8),
    .NrMinions (NrMinions)
  ) u_new (
    .clk_i            (clk_i),
    .rst_ni           (rst_ni),
    .dft_i            (dft),
    .vmspagesize_i    (new_vmspagesize),
    .coop_mode_i      (coop_mode_i),
    .req_min_id_i     (req_min_id_i),
    .req_data_i       (new_req_data),
    .req_valid_i      (req_valid_i),
    .resp_data_o      (new_resp_data),
    .satp_info_i      (new_satp_info),
    .matp_info_i      (new_matp_info),
    .tlb_invalidate_i (tlb_invalidate_i),
    .ptw_req_data_o   (new_ptw_req_data),
    .ptw_req_valid_o  (new_ptw_req_valid_o),
    .ptw_req_ready_i  (ptw_req_ready_i),
    .ptw_invalidate_o (new_ptw_invalidate_o),
    .ptw_resp_data_i  (new_ptw_resp_data),
    .ptw_resp_valid_i (ptw_resp_valid_i),
    .vm_enabled_o     (new_vm_enabled_o)
  );

  tlb_top #(
    .ENTRIES    (8),
    .NR_MINIONS (NrMinions)
  ) u_orig (
    .clock          (clk_i),
    .reset          (reset_orig),
    .vmspagesize    (orig_vmspagesize),
    .coop_mode      (coop_mode_i),
    .req_min_id     (req_min_id_i),
    .req_data       (orig_req_data),
    .req_valid      (req_valid_i),
    .resp_data      (orig_resp_data),
    .satp_info      (orig_satp_info_flat),
    .matp_info      (orig_matp_info_flat),
    .tlb_invalidate (tlb_invalidate_i),
    .ptw_req_data   (orig_ptw_req_data),
    .ptw_req_valid  (orig_ptw_req_valid_o),
    .ptw_req_ready  (ptw_req_ready_i),
    .ptw_invalidate (orig_ptw_invalidate_o),
    .ptw_resp_data  (orig_ptw_resp_data),
    .ptw_resp_valid (ptw_resp_valid_i),
    .vm_enabled     (orig_vm_enabled_o)
  );

  assign new_state_o                 = u_new.state_q;
  assign orig_state_o                = u_orig.state;
  assign new_ptw_req_valid_q_o       = u_new.ptw_req_valid_q;
  assign orig_ptw_req_valid_q_o      = u_orig.ptw_req_valid;
  assign new_ptw_resp_valid_q_o      = u_new.ptw_resp_valid_q;
  assign orig_ptw_resp_valid_q_o     = u_orig.ptw_resp_valid_reg;
  assign new_rf_latch_wr_en_o        = u_new.rf_latch_wr_en;
  assign orig_rf_latch_wr_en_o       = u_orig.rf_latch_wr_en;
  assign new_rf_latch_wr_en_early_o  = u_new.rf_latch_wr_en_early;
  assign orig_rf_latch_wr_en_early_o = u_orig.rf_latch_wr_en_early;
  assign new_req_min_id_q_o          = u_new.req_min_id_q;
  assign orig_req_min_id_q_o         = u_orig.req_min_id_reg;
  assign new_req_vpn_q_o             = u_new.req_vpn_q;
  assign orig_req_vpn_q_o            = u_orig.req_vpn_reg;
  assign new_req_tag_q_o             = u_new.req_tag_q;
  assign orig_req_tag_q_o            = u_orig.req_tag_reg;
  assign new_tlb_cache_waddr_o       = u_new.tlb_cache_waddr;
  assign orig_tlb_cache_waddr_o      = u_orig.tlb_cache_waddr;
  assign new_tlb_cache_raddr_o       = u_new.tlb_cache_raddr;
  assign orig_tlb_cache_raddr_o      = u_orig.tlb_cache_raddr;

endmodule : cosim_minion_tlb_tb

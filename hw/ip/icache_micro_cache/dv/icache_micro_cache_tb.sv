// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

/* verilator lint_off SYNCASYNCNET */  // Testbench reset drives translated async-assert/sync-deassert flops and original-style sync logic.
module icache_micro_cache_tb
  import minion_pkg::*;
  import minion_dcache_pkg::*;
  import minion_frontend_pkg::*;
  import icache_geom_pkg::*;
  import icache_pkg::*;
(
  input  logic                          clk_i,
  input  logic                          rst_ni,
  input  logic                          ioshire_i,
  input  logic [1:0]                    prefetch_prv_i,
  input  logic [VaSize-1:6]             prefetch_start_addr_i,
  input  logic [5:0]                    prefetch_num_lines_i,
  input  logic                          prefetch_start_i,
  output logic                          prefetch_done_o,
  input  logic [$bits(esr_mprot_t)-1:0] mprot_i,
  input  logic [1:0]                    vmspagesize_i,
  input  logic                          bypass_icache_i,
  input  logic                          coop_mode_i,
  output logic                          req_ready_o,
  input  logic                          req_valid_i,
  input  logic                          req_min_id_i,
  input  logic [VaSizeExt-1:0]          req_addr_i,
  input  logic [1:0]                    req_prv_i,
  input  logic                          req_mprv_i,
  input  logic [1:0]                    req_mpp_i,
  input  logic                          req_sum_i,
  input  logic                          req_mxr_i,
  input  logic                          req_debug_i,
  output logic                          resp_valid_o,
  output logic                          resp_miss_o,
  output logic [63:0]                   resp_data_0_o,
  output logic [63:0]                   resp_data_1_o,
  output logic [63:0]                   resp_data_2_o,
  output logic [63:0]                   resp_data_3_o,
  output logic                          resp_page_fault_o,
  output logic                          resp_access_fault_o,
  output logic                          resp_cacheable_o,
  output logic                          resp_bus_err_o,
  output logic                          resp_ecc_err_o,
  output logic                          resp_fill_done_o,
  input  logic                          flush_data_i,
  input  logic                          l1_miss_req_ready_i,
  output logic                          l1_miss_req_valid_o,
  output logic [PaSize-1:0]             l1_miss_req_addr_o,
  input  logic                          l1_miss_resp_early_valid_i,
  input  logic                          l1_miss_resp_valid_i,
  input  logic [63:0]                   l1_miss_resp_data_0_i,
  input  logic [63:0]                   l1_miss_resp_data_1_i,
  input  logic [63:0]                   l1_miss_resp_data_2_i,
  input  logic [63:0]                   l1_miss_resp_data_3_i,
  input  logic [63:0]                   l1_miss_resp_data_4_i,
  input  logic [63:0]                   l1_miss_resp_data_5_i,
  input  logic [63:0]                   l1_miss_resp_data_6_i,
  input  logic [63:0]                   l1_miss_resp_data_7_i,
  input  logic                          l1_miss_resp_ecc_err_i,
  input  logic                          l1_miss_resp_l2_err_i,
  input  logic [CsrSatpModeSz-1:0]      satp_mode0_i,
  input  logic [PaTransSize-1:0]        satp_ppn0_i,
  input  logic [CsrSatpModeSz-1:0]      satp_mode1_i,
  input  logic [PaTransSize-1:0]        satp_ppn1_i,
  input  logic [CsrSatpModeSz-1:0]      matp_mode0_i,
  input  logic [PaTransSize-1:0]        matp_ppn0_i,
  input  logic [CsrSatpModeSz-1:0]      matp_mode1_i,
  input  logic [PaTransSize-1:0]        matp_ppn1_i,
  input  logic [1:0]                    tlb_invalidate_i,
  output logic [CsrSatpModeSz-1:0]      ptw_req_satp_mode_o,
  output logic [PaTransSize-1:0]        ptw_req_satp_ppn_o,
  output logic [1:0]                    ptw_req_prv_o,
  output logic [VaSize-1:VaUntransSize] ptw_req_addr_o,
  output logic                          ptw_req_valid_o,
  input  logic                          ptw_req_ready_i,
  output logic                          ptw_invalidate_o,
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
  input  logic [IcacheDbgUcacheAddrWidth-1:0] apb_paddr_i,
  input  logic                          apb_pwrite_i,
  input  logic                          apb_psel_i,
  input  logic                          apb_penable_i,
  input  logic [BpamShireApbDataWidth-1:0] apb_pwdata_i,
  output logic                          apb_pready_o,
  output logic [BpamShireApbDataWidth-1:0] apb_prdata_o,
  output logic                          apb_pslverr_o
);

  localparam int unsigned NrMinions = 2;

  icache_prefetch_conf_t prefetch_conf;
  esr_mprot_t            mprot;
  tlb_entry_type         vmspagesize;
  frontend_icache_req_t  req_data;
  logic [IcacheBlockBits-1:0] l1_miss_resp_data;
  minion_satp_info       satp_info [NrMinions-1:0];
  minion_satp_info       matp_info [NrMinions-1:0];
  minion_ptw_req         ptw_req_data;
  minion_ptw_pte         ptw_resp_data;
  icache_frontend_resp_t resp_data;
  /* verilator lint_off UNUSEDSIGNAL */  // The debug bus is covered by cosim; lint anchors keep inactive generic tech seams from appearing as extra tops.
  icache_dbg_sm_t        dbg_sm_signals;
  logic                  lint_clk_gate_o;
  logic [7:0]            lint_diff_preview_data;
  /* verilator lint_on UNUSEDSIGNAL */

  prim_clk_gate u_lint_clk_gate (
    .clk_i (clk_i),
    .en_i  (1'b0),
    .dft_i ('0),
    .clk_o (lint_clk_gate_o)
  );

  prim_rf_1r1w_diff_preview #(
    .RWidth     (8),
    .RAlignment (8),
    .WWidth     (8),
    .Entries    (2)
  ) u_lint_diff_preview (
    .preview_clk_i        (clk_i),
    .rf_clk_i             (clk_i),
    .wr_data_en_1p_next_i (1'b0),
    .rd_data_o            (lint_diff_preview_data),
    .rd_addr_i            (1'b0),
    .wr_data_i            ('0),
    .wr_addr_i            (1'b0),
    .wr_en_i              (1'b0)
  );

  always_comb begin
    prefetch_conf = '0;
    prefetch_conf.prv = prefetch_prv_i;
    prefetch_conf.start_addr = prefetch_start_addr_i;
    prefetch_conf.num_lines = prefetch_num_lines_i;

    mprot = esr_mprot_t'(mprot_i);
    vmspagesize = tlb_entry_type'(vmspagesize_i);

    req_data = '0;
    req_data.thread_id = req_min_id_i;
    req_data.vm_status.prv = req_prv_i;
    req_data.vm_status.mprv = req_mprv_i;
    req_data.vm_status.mpp = req_mpp_i;
    req_data.vm_status.sum = req_sum_i;
    req_data.vm_status.mxr = req_mxr_i;
    req_data.vm_status.debug = req_debug_i;
    req_data.addr = req_addr_i;

    l1_miss_resp_data = {
      l1_miss_resp_data_7_i,
      l1_miss_resp_data_6_i,
      l1_miss_resp_data_5_i,
      l1_miss_resp_data_4_i,
      l1_miss_resp_data_3_i,
      l1_miss_resp_data_2_i,
      l1_miss_resp_data_1_i,
      l1_miss_resp_data_0_i
    };

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

    ptw_req_satp_mode_o = ptw_req_data.satp_mode;
    ptw_req_satp_ppn_o = ptw_req_data.satp_ppn;
    ptw_req_prv_o = ptw_req_data.prv;
    ptw_req_addr_o = ptw_req_data.addr;

    resp_data_0_o = resp_data.data[63:0];
    resp_data_1_o = resp_data.data[127:64];
    resp_data_2_o = resp_data.data[191:128];
    resp_data_3_o = resp_data.data[255:192];
    resp_page_fault_o = resp_data.page_fault;
    resp_access_fault_o = resp_data.access_fault;
    resp_cacheable_o = resp_data.cacheable;
    resp_bus_err_o = resp_data.bus_err;
    resp_ecc_err_o = resp_data.ecc_err;
  end

  icache_micro_cache #(
    .Id        (0),
    .NrMinions (NrMinions)
  ) u_dut (
    .clk_i,
    .rst_ni,
    .ioshire_i,
    .esr_prefetch_conf_i      (prefetch_conf),
    .esr_prefetch_start_i     (prefetch_start_i),
    .esr_prefetch_done_o      (prefetch_done_o),
    .esr_mprot_i              (mprot),
    .esr_vmspagesize_i        (vmspagesize),
    .esr_bypass_icache_i      (bypass_icache_i),
    .esr_shire_coop_mode_i    (coop_mode_i),
    .f0_req_ready_o           (req_ready_o),
    .f0_req_valid_i           (req_valid_i),
    .f0_req_i                 (req_data),
    .f0_req_min_id_i          (req_min_id_i),
    .f4_resp_valid_o          (resp_valid_o),
    .f4_resp_miss_o           (resp_miss_o),
    .f4_resp_o                (resp_data),
    .f5_resp_fill_done_o      (resp_fill_done_o),
    .f0_flush_data_i          (flush_data_i),
    .f0_l1_miss_req_ready_i   (l1_miss_req_ready_i),
    .f0_l1_miss_req_valid_o   (l1_miss_req_valid_o),
    .f0_l1_miss_req_addr_o    (l1_miss_req_addr_o),
    .f0_l1_miss_resp_early_valid_i(l1_miss_resp_early_valid_i),
    .f0_l1_miss_resp_valid_i  (l1_miss_resp_valid_i),
    .f0_l1_miss_resp_data_i   (l1_miss_resp_data),
    .f0_l1_miss_resp_ecc_err_i(l1_miss_resp_ecc_err_i),
    .f0_l1_miss_resp_l2_err_i (l1_miss_resp_l2_err_i),
    .satp_info_i              (satp_info),
    .matp_info_i              (matp_info),
    .tlb_invalidate_i,
    .ptw_req_data_o           (ptw_req_data),
    .ptw_req_valid_o,
    .ptw_req_ready_i,
    .ptw_invalidate_o,
    .ptw_resp_valid_i,
    .ptw_resp_data_i          (ptw_resp_data),
    .apb_paddr_i,
    .apb_pwrite_i,
    .apb_psel_i,
    .apb_penable_i,
    .apb_pwdata_i,
    .apb_pready_o,
    .apb_prdata_o,
    .apb_pslverr_o,
    .dbg_sm_signals_o         (dbg_sm_signals)
  );

endmodule : icache_micro_cache_tb
/* verilator lint_on SYNCASYNCNET */

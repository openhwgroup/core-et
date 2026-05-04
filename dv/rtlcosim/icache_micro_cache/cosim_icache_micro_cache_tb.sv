// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation wrapper: original icache_micro_cache vs new icache_micro_cache.

`include "soc.vh"

/* verilator lint_off SYNCASYNCNET */  // Testbench reset drives translated async-assert/sync-deassert flops and original-style sync logic.
module cosim_icache_micro_cache_tb
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
  output logic                          new_prefetch_done_o,
  output logic                          orig_prefetch_done_o,
  input  logic [$bits(esr_mprot_t)-1:0] mprot_i,
  input  logic [1:0]                    vmspagesize_i,
  input  logic                          bypass_icache_i,
  input  logic                          coop_mode_i,
  output logic                          new_req_ready_o,
  output logic                          orig_req_ready_o,
  input  logic                          req_valid_i,
  input  logic                          req_min_id_i,
  input  logic [VaSizeExt-1:0]          req_addr_i,
  input  logic [1:0]                    req_prv_i,
  input  logic                          req_mprv_i,
  input  logic [1:0]                    req_mpp_i,
  input  logic                          req_sum_i,
  input  logic                          req_mxr_i,
  input  logic                          req_debug_i,
  output logic                          new_resp_valid_o,
  output logic                          orig_resp_valid_o,
  output logic                          new_resp_miss_o,
  output logic                          orig_resp_miss_o,
  output logic [63:0]                   new_resp_data_0_o,
  output logic [63:0]                   orig_resp_data_0_o,
  output logic [63:0]                   new_resp_data_1_o,
  output logic [63:0]                   orig_resp_data_1_o,
  output logic [63:0]                   new_resp_data_2_o,
  output logic [63:0]                   orig_resp_data_2_o,
  output logic [63:0]                   new_resp_data_3_o,
  output logic [63:0]                   orig_resp_data_3_o,
  output logic                          new_resp_page_fault_o,
  output logic                          orig_resp_page_fault_o,
  output logic                          new_resp_access_fault_o,
  output logic                          orig_resp_access_fault_o,
  output logic                          new_resp_cacheable_o,
  output logic                          orig_resp_cacheable_o,
  output logic                          new_resp_bus_err_o,
  output logic                          orig_resp_bus_err_o,
  output logic                          new_resp_ecc_err_o,
  output logic                          orig_resp_ecc_err_o,
  output logic                          new_resp_fill_done_o,
  output logic                          orig_resp_fill_done_o,
  input  logic                          flush_data_i,
  input  logic                          l1_miss_req_ready_i,
  output logic                          new_l1_miss_req_valid_o,
  output logic                          orig_l1_miss_req_valid_o,
  output logic [PaSize-1:0]             new_l1_miss_req_addr_o,
  output logic [PaSize-1:0]             orig_l1_miss_req_addr_o,
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
  output logic [CsrSatpModeSz-1:0]      new_ptw_req_satp_mode_o,
  output logic [CsrSatpModeSz-1:0]      orig_ptw_req_satp_mode_o,
  output logic [PaTransSize-1:0]        new_ptw_req_satp_ppn_o,
  output logic [PaTransSize-1:0]        orig_ptw_req_satp_ppn_o,
  output logic [1:0]                    new_ptw_req_prv_o,
  output logic [1:0]                    orig_ptw_req_prv_o,
  output logic [VaSize-1:VaUntransSize] new_ptw_req_addr_o,
  output logic [VaSize-1:VaUntransSize] orig_ptw_req_addr_o,
  output logic                          new_ptw_req_valid_o,
  output logic                          orig_ptw_req_valid_o,
  input  logic                          ptw_req_ready_i,
  output logic                          new_ptw_invalidate_o,
  output logic                          orig_ptw_invalidate_o,
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
  output logic                          new_apb_pready_o,
  output logic                          orig_apb_pready_o,
  output logic [BpamShireApbDataWidth-1:0] new_apb_prdata_o,
  output logic [BpamShireApbDataWidth-1:0] orig_apb_prdata_o,
  output logic                          new_apb_pslverr_o,
  output logic                          orig_apb_pslverr_o,
  output logic [63:0]                   new_dbg_chunk0_o,
  output logic [63:0]                   orig_dbg_chunk0_o,
  output logic [63:0]                   new_dbg_chunk1_o,
  output logic [63:0]                   orig_dbg_chunk1_o,
  output logic [63:0]                   new_dbg_chunk2_o,
  output logic [63:0]                   orig_dbg_chunk2_o,
  output logic [63:0]                   new_dbg_chunk3_o,
  output logic [63:0]                   orig_dbg_chunk3_o
);

  localparam int unsigned NrMinions = 2;

  logic                         reset_orig;
  icache_prefetch_conf_t        new_prefetch_conf;
  icache_prefetch_conf_t        orig_prefetch_conf;
  esr_mprot_t                   new_mprot;
  esr_mprot_t                   orig_mprot;
  tlb_entry_type                new_vmspagesize;
  tlb_entry_type                orig_vmspagesize;
  frontend_icache_req_t         new_req_data;
  frontend_icache_req           orig_req_data;
  logic [IcacheBlockBits-1:0]   l1_miss_resp_data;
  minion_satp_info              new_satp_info [NrMinions-1:0];
  minion_satp_info              new_matp_info [NrMinions-1:0];
  minion_satp_info              orig_satp_info [NrMinions-1:0];
  minion_satp_info              orig_matp_info [NrMinions-1:0];
  logic [(NrMinions * $bits(minion_satp_info))-1:0] orig_satp_info_flat;
  logic [(NrMinions * $bits(minion_satp_info))-1:0] orig_matp_info_flat;
  minion_ptw_req                new_ptw_req_data;
  minion_ptw_req                orig_ptw_req_data;
  minion_ptw_pte                new_ptw_resp_data;
  minion_ptw_pte                orig_ptw_resp_data;
  icache_frontend_resp_t        new_resp_data;
  icache_frontend_resp          orig_resp_data;
  icache_dbg_sm_t               new_dbg_sm_signals;
  icache_dbg_sm_t               orig_dbg_sm_signals;
  logic [255:0]                 new_dbg_flat;
  logic [255:0]                 orig_dbg_flat;

  always_comb begin
    reset_orig = ~rst_ni;

    new_prefetch_conf = '0;
    new_prefetch_conf.prv = prefetch_prv_i;
    new_prefetch_conf.start_addr = prefetch_start_addr_i;
    new_prefetch_conf.num_lines = prefetch_num_lines_i;

    orig_prefetch_conf = '{
      prv: prefetch_prv_i,
      start_addr: prefetch_start_addr_i,
      num_lines: prefetch_num_lines_i
    };

    new_mprot = esr_mprot_t'(mprot_i);
    orig_mprot = esr_mprot_t'(mprot_i);
    new_vmspagesize = tlb_entry_type'(vmspagesize_i);
    orig_vmspagesize = tlb_entry_type'(vmspagesize_i);

    new_req_data = '0;
    new_req_data.thread_id = req_min_id_i;
    new_req_data.vm_status.prv = req_prv_i;
    new_req_data.vm_status.mprv = req_mprv_i;
    new_req_data.vm_status.mpp = req_mpp_i;
    new_req_data.vm_status.sum = req_sum_i;
    new_req_data.vm_status.mxr = req_mxr_i;
    new_req_data.vm_status.debug = req_debug_i;
    new_req_data.addr = req_addr_i;

    orig_req_data = '{
      thread_id: req_min_id_i,
      vm_status: '{
        prv: req_prv_i,
        mprv: req_mprv_i,
        mpp: req_mpp_i,
        sum: req_sum_i,
        mxr: req_mxr_i,
        debug: req_debug_i
      },
      addr: req_addr_i
    };

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

    new_ptw_req_satp_mode_o = new_ptw_req_data.satp_mode;
    new_ptw_req_satp_ppn_o = new_ptw_req_data.satp_ppn;
    new_ptw_req_prv_o = new_ptw_req_data.prv;
    new_ptw_req_addr_o = new_ptw_req_data.addr;
    orig_ptw_req_satp_mode_o = orig_ptw_req_data.satp_mode;
    orig_ptw_req_satp_ppn_o = orig_ptw_req_data.satp_ppn;
    orig_ptw_req_prv_o = orig_ptw_req_data.prv;
    orig_ptw_req_addr_o = orig_ptw_req_data.addr;

    new_resp_data_0_o = new_resp_data.data[63:0];
    new_resp_data_1_o = new_resp_data.data[127:64];
    new_resp_data_2_o = new_resp_data.data[191:128];
    new_resp_data_3_o = new_resp_data.data[255:192];
    orig_resp_data_0_o = orig_resp_data.data[63:0];
    orig_resp_data_1_o = orig_resp_data.data[127:64];
    orig_resp_data_2_o = orig_resp_data.data[191:128];
    orig_resp_data_3_o = orig_resp_data.data[255:192];
    new_resp_page_fault_o = new_resp_data.page_fault;
    new_resp_access_fault_o = new_resp_data.access_fault;
    new_resp_cacheable_o = new_resp_data.cacheable;
    new_resp_bus_err_o = new_resp_data.bus_err;
    new_resp_ecc_err_o = new_resp_data.ecc_err;
    orig_resp_page_fault_o = orig_resp_data.page_fault;
    orig_resp_access_fault_o = orig_resp_data.access_fault;
    orig_resp_cacheable_o = orig_resp_data.cacheable;
    orig_resp_bus_err_o = orig_resp_data.bus_err;
    orig_resp_ecc_err_o = orig_resp_data.ecc_err;

    new_dbg_flat = '0;
    new_dbg_flat[$bits(icache_dbg_sm_t)-1:0] = new_dbg_sm_signals;
    orig_dbg_flat = '0;
    orig_dbg_flat[$bits(icache_dbg_sm_t)-1:0] = orig_dbg_sm_signals;
    new_dbg_chunk0_o = new_dbg_flat[63:0];
    new_dbg_chunk1_o = new_dbg_flat[127:64];
    new_dbg_chunk2_o = new_dbg_flat[191:128];
    new_dbg_chunk3_o = new_dbg_flat[255:192];
    orig_dbg_chunk0_o = orig_dbg_flat[63:0];
    orig_dbg_chunk1_o = orig_dbg_flat[127:64];
    orig_dbg_chunk2_o = orig_dbg_flat[191:128];
    orig_dbg_chunk3_o = orig_dbg_flat[255:192];
  end

  icache_micro_cache #(
    .Id        (0),
    .NrMinions (NrMinions)
  ) u_new (
    .clk_i,
    .rst_ni,
    .ioshire_i,
    .esr_prefetch_conf_i      (new_prefetch_conf),
    .esr_prefetch_start_i     (prefetch_start_i),
    .esr_prefetch_done_o      (new_prefetch_done_o),
    .esr_mprot_i              (new_mprot),
    .esr_vmspagesize_i        (new_vmspagesize),
    .esr_bypass_icache_i      (bypass_icache_i),
    .esr_shire_coop_mode_i    (coop_mode_i),
    .f0_req_ready_o           (new_req_ready_o),
    .f0_req_valid_i           (req_valid_i),
    .f0_req_i                 (new_req_data),
    .f0_req_min_id_i          (req_min_id_i),
    .f4_resp_valid_o          (new_resp_valid_o),
    .f4_resp_miss_o           (new_resp_miss_o),
    .f4_resp_o                (new_resp_data),
    .f5_resp_fill_done_o      (new_resp_fill_done_o),
    .f0_flush_data_i          (flush_data_i),
    .f0_l1_miss_req_ready_i   (l1_miss_req_ready_i),
    .f0_l1_miss_req_valid_o   (new_l1_miss_req_valid_o),
    .f0_l1_miss_req_addr_o    (new_l1_miss_req_addr_o),
    .f0_l1_miss_resp_early_valid_i(l1_miss_resp_early_valid_i),
    .f0_l1_miss_resp_valid_i  (l1_miss_resp_valid_i),
    .f0_l1_miss_resp_data_i   (l1_miss_resp_data),
    .f0_l1_miss_resp_ecc_err_i(l1_miss_resp_ecc_err_i),
    .f0_l1_miss_resp_l2_err_i (l1_miss_resp_l2_err_i),
    .satp_info_i              (new_satp_info),
    .matp_info_i              (new_matp_info),
    .tlb_invalidate_i,
    .ptw_req_data_o           (new_ptw_req_data),
    .ptw_req_valid_o          (new_ptw_req_valid_o),
    .ptw_req_ready_i,
    .ptw_invalidate_o         (new_ptw_invalidate_o),
    .ptw_resp_valid_i,
    .ptw_resp_data_i          (new_ptw_resp_data),
    .apb_paddr_i,
    .apb_pwrite_i,
    .apb_psel_i,
    .apb_penable_i,
    .apb_pwdata_i,
    .apb_pready_o             (new_apb_pready_o),
    .apb_prdata_o             (new_apb_prdata_o),
    .apb_pslverr_o            (new_apb_pslverr_o),
    .dbg_sm_signals_o         (new_dbg_sm_signals)
  );

  icache_micro_cache_orig #(
    .ID            (0),
    .NR_MINIONS    (NrMinions),
    .NR_MINIONS_L  (1)
  ) u_orig (
    .clock                     (clk_i),
    .reset                     (reset_orig),
    .ioshire                   (ioshire_i),
    .esr_prefetch_conf         (orig_prefetch_conf),
    .esr_prefetch_start        (prefetch_start_i),
    .esr_prefetch_done         (orig_prefetch_done_o),
    .esr_mprot                 (orig_mprot),
    .esr_vmspagesize           (orig_vmspagesize),
    .esr_bypass_icache         (bypass_icache_i),
    .esr_shire_coop_mode       (coop_mode_i),
    .f0_req_ready              (orig_req_ready_o),
    .f0_req_valid              (req_valid_i),
    .f0_req                    (orig_req_data),
    .f0_req_min_id             (req_min_id_i),
    .f4_resp_valid             (orig_resp_valid_o),
    .f4_resp_miss              (orig_resp_miss_o),
    .f4_resp                   (orig_resp_data),
    .f5_resp_fill_done         (orig_resp_fill_done_o),
    .f0_flush_data             (flush_data_i),
    .f0_l1_miss_req_ready      (l1_miss_req_ready_i),
    .f0_l1_miss_req_valid      (orig_l1_miss_req_valid_o),
    .f0_l1_miss_req_addr       (orig_l1_miss_req_addr_o),
    .f0_l1_miss_resp_early_valid(l1_miss_resp_early_valid_i),
    .f0_l1_miss_resp_valid     (l1_miss_resp_valid_i),
    .f0_l1_miss_resp_data      (l1_miss_resp_data),
    .f0_l1_miss_resp_ecc_err   (l1_miss_resp_ecc_err_i),
    .f0_l1_miss_resp_l2_err    (l1_miss_resp_l2_err_i),
    .satp_info                 (orig_satp_info_flat),
    .matp_info                 (orig_matp_info_flat),
    .tlb_invalidate            (tlb_invalidate_i),
    .ptw_req_data              (orig_ptw_req_data),
    .ptw_req_valid             (orig_ptw_req_valid_o),
    .ptw_req_ready             (ptw_req_ready_i),
    .ptw_invalidate            (orig_ptw_invalidate_o),
    .ptw_resp_valid            (ptw_resp_valid_i),
    .ptw_resp_data             (orig_ptw_resp_data),
    .apb_paddr                 (apb_paddr_i),
    .apb_pwrite                (apb_pwrite_i),
    .apb_psel                  (apb_psel_i),
    .apb_penable               (apb_penable_i),
    .apb_pwdata                (apb_pwdata_i),
    .apb_pready                (orig_apb_pready_o),
    .apb_prdata                (orig_apb_prdata_o),
    .apb_pslverr               (orig_apb_pslverr_o),
    .dbg_sm_signals            (orig_dbg_sm_signals)
  );

endmodule : cosim_icache_micro_cache_tb
/* verilator lint_on SYNCASYNCNET */

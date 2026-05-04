// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

/* verilator lint_off SYNCASYNCNET */  // Testbench reset drives translated async-assert/sync-deassert flops and original-style sync logic.
module icache_top_tb
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
  input  logic                          err_rsp_enable_i,
  input  logic [2:0]                    err_interrupt_enable_i,
  output logic                          err_log_sbe_o,
  output logic                          err_log_dbe_o,
  input  logic [$bits(esr_mprot_t)-1:0] mprot_i,
  input  logic [1:0]                    vmspagesize_i,
  input  logic                          bypass_icache_i,
  input  logic                          coop_mode_i,
  output logic                          req0_ready_o,
  output logic                          req1_ready_o,
  input  logic                          req0_valid_i,
  input  logic                          req0_min_id_i,
  input  logic [VaSizeExt-1:0]          req0_addr_i,
  input  logic [1:0]                    req0_prv_i,
  input  logic                          req0_mprv_i,
  input  logic [1:0]                    req0_mpp_i,
  input  logic                          req0_sum_i,
  input  logic                          req0_mxr_i,
  input  logic                          req0_debug_i,
  input  logic                          req1_valid_i,
  input  logic                          req1_min_id_i,
  input  logic [VaSizeExt-1:0]          req1_addr_i,
  input  logic [1:0]                    req1_prv_i,
  input  logic                          req1_mprv_i,
  input  logic [1:0]                    req1_mpp_i,
  input  logic                          req1_sum_i,
  input  logic                          req1_mxr_i,
  input  logic                          req1_debug_i,
  output logic                          resp0_valid_o,
  output logic                          resp0_miss_o,
  output logic [63:0]                   resp0_data_0_o,
  output logic [63:0]                   resp0_data_1_o,
  output logic [63:0]                   resp0_data_2_o,
  output logic [63:0]                   resp0_data_3_o,
  output logic                          resp0_page_fault_o,
  output logic                          resp0_access_fault_o,
  output logic                          resp0_cacheable_o,
  output logic                          resp0_bus_err_o,
  output logic                          resp0_ecc_err_o,
  output logic                          resp0_fill_done_o,
  output logic                          resp1_valid_o,
  output logic                          resp1_miss_o,
  output logic [63:0]                   resp1_data_0_o,
  output logic [63:0]                   resp1_data_1_o,
  output logic [63:0]                   resp1_data_2_o,
  output logic [63:0]                   resp1_data_3_o,
  output logic                          resp1_page_fault_o,
  output logic                          resp1_access_fault_o,
  output logic                          resp1_cacheable_o,
  output logic                          resp1_bus_err_o,
  output logic                          resp1_ecc_err_o,
  output logic                          resp1_fill_done_o,
  input  logic                          flush_data_i,
  input  logic                          l2_miss_req_disable_next_i,
  input  logic                          l2_miss_req_ready_i,
  output logic                          l2_miss_req_valid_o,
  output logic [PaSize-1:0]             l2_miss_req_addr_o,
  output logic [4:0]                    l2_miss_req_opcode_o,
  output logic [2:0]                    l2_miss_req_size_o,
  output logic [6:0]                    l2_miss_req_subopcode_o,
  output logic                          l2_miss_resp_ready_o,
  input  logic                          l2_miss_resp_valid_i,
  input  logic [1:0]                    l2_miss_resp_opcode_i,
  input  logic [CsrSatpModeSz-1:0]      satp_mode0_i,
  input  logic [PaTransSize-1:0]        satp_ppn0_i,
  input  logic [CsrSatpModeSz-1:0]      satp_mode1_i,
  input  logic [PaTransSize-1:0]        satp_ppn1_i,
  input  logic [CsrSatpModeSz-1:0]      matp_mode0_i,
  input  logic [PaTransSize-1:0]        matp_ppn0_i,
  input  logic [CsrSatpModeSz-1:0]      matp_mode1_i,
  input  logic [PaTransSize-1:0]        matp_ppn1_i,
  input  logic [1:0]                    tlb_invalidate_i,
  output logic [CsrSatpModeSz-1:0]      ptw0_req_satp_mode_o,
  output logic [PaTransSize-1:0]        ptw0_req_satp_ppn_o,
  output logic [1:0]                    ptw0_req_prv_o,
  output logic [VaSize-1:VaUntransSize] ptw0_req_addr_o,
  output logic                          ptw0_req_valid_o,
  input  logic                          ptw0_req_ready_i,
  output logic                          ptw0_invalidate_o,
  input  logic                          ptw0_resp_canceled_req_i,
  input  logic                          ptw0_resp_access_fault_i,
  input  logic [7:0]                    ptw0_resp_rsvd_for_hw_i,
  input  logic [CsrSatpPpnSz-1:0]       ptw0_resp_ppn_i,
  input  logic [1:0]                    ptw0_resp_rsvd_for_sw_i,
  input  logic                          ptw0_resp_d_i,
  input  logic                          ptw0_resp_a_i,
  input  logic                          ptw0_resp_g_i,
  input  logic                          ptw0_resp_u_i,
  input  logic                          ptw0_resp_x_i,
  input  logic                          ptw0_resp_w_i,
  input  logic                          ptw0_resp_r_i,
  input  logic                          ptw0_resp_v_i,
  input  logic                          ptw0_resp_valid_i,
  output logic [CsrSatpModeSz-1:0]      ptw1_req_satp_mode_o,
  output logic [PaTransSize-1:0]        ptw1_req_satp_ppn_o,
  output logic [1:0]                    ptw1_req_prv_o,
  output logic [VaSize-1:VaUntransSize] ptw1_req_addr_o,
  output logic                          ptw1_req_valid_o,
  input  logic                          ptw1_req_ready_i,
  output logic                          ptw1_invalidate_o,
  input  logic                          ptw1_resp_canceled_req_i,
  input  logic                          ptw1_resp_access_fault_i,
  input  logic [7:0]                    ptw1_resp_rsvd_for_hw_i,
  input  logic [CsrSatpPpnSz-1:0]       ptw1_resp_ppn_i,
  input  logic [1:0]                    ptw1_resp_rsvd_for_sw_i,
  input  logic                          ptw1_resp_d_i,
  input  logic                          ptw1_resp_a_i,
  input  logic                          ptw1_resp_g_i,
  input  logic                          ptw1_resp_u_i,
  input  logic                          ptw1_resp_x_i,
  input  logic                          ptw1_resp_w_i,
  input  logic                          ptw1_resp_r_i,
  input  logic                          ptw1_resp_v_i,
  input  logic                          ptw1_resp_valid_i,
  output logic                          sram_req_write_o,
  output logic [IcacheSramAddrWidth-1:0] sram_req_addr_o,
  output logic                          sram_req_valid_o,
  input  logic                          sram_req_ready_i,
  input  logic                          sram_resp_valid_i,
  input  logic [63:0]                   sram_resp_qw0_i,
  input  logic [63:0]                   sram_resp_qw1_i,
  input  logic [63:0]                   sram_resp_qw2_i,
  input  logic [63:0]                   sram_resp_qw3_i,
  input  logic [63:0]                   sram_resp_qw4_i,
  input  logic [63:0]                   sram_resp_qw5_i,
  input  logic [63:0]                   sram_resp_qw6_i,
  input  logic [63:0]                   sram_resp_qw7_i,
  output logic                          sram_resp_ready_o,
  input  logic [IcacheNeighApbAddrWidth-1:0] apb_paddr_i,
  input  logic                          apb_pwrite_i,
  input  logic                          apb_psel_i,
  input  logic                          apb_penable_i,
  input  logic [BpamShireApbDataWidth-1:0] apb_pwdata_i,
  output logic                          apb_pready_o,
  output logic [BpamShireApbDataWidth-1:0] apb_prdata_o,
  output logic                          apb_pslverr_o,
  output logic [63:0]                   dbg_chunk0_o,
  output logic [63:0]                   dbg_chunk1_o,
  output logic [63:0]                   dbg_chunk2_o,
  output logic [63:0]                   dbg_chunk3_o
);

  localparam int unsigned NrMinions = 2;
  localparam int unsigned NrRequestors = 2;

  icache_prefetch_conf_t        prefetch_conf;
  esr_icache_err_log_ctl_t      err_log_ctl;
  esr_mprot_t                   mprot;
  tlb_entry_type                vmspagesize;
  frontend_icache_req_t         req_data [NrRequestors-1:0];
  icache_frontend_resp_t        resp_data [NrRequestors-1:0];
  minion_satp_info              satp_info [NrMinions-1:0];
  minion_satp_info              matp_info [NrMinions-1:0];
  minion_ptw_req                ptw_req_data [NrRequestors-1:0];
  minion_ptw_pte                ptw_resp_data [NrRequestors-1:0];
  etlink_pkg::rsp_t             l2_miss_resp;
  logic [7:0]                   sram_resp_ecc [7:0];
  logic [7:0][71:0]             sram_resp_data_and_ecc;
  logic [IcacheNrMemsPerRow-1:0][IcacheMemWidth-1:0] sram_resp_dout;
  icache_dbg_sm_t               dbg_sm_signals;
  logic [$bits(icache_dbg_sm_t)-1:0] dbg_flat;
  logic [255:0]                 dbg_flat_padded;

  always_comb begin
    prefetch_conf = '0;
    prefetch_conf.prv = prefetch_prv_i;
    prefetch_conf.start_addr = prefetch_start_addr_i;
    prefetch_conf.num_lines = prefetch_num_lines_i;

    err_log_ctl = '0;
    err_log_ctl.err_rsp_enable = err_rsp_enable_i;
    err_log_ctl.err_interrupt_enable = err_interrupt_enable_i;

    mprot = esr_mprot_t'(mprot_i);
    vmspagesize = tlb_entry_type'(vmspagesize_i);

    req_data[0] = '0;
    req_data[0].thread_id = req0_min_id_i;
    req_data[0].vm_status.prv = req0_prv_i;
    req_data[0].vm_status.mprv = req0_mprv_i;
    req_data[0].vm_status.mpp = req0_mpp_i;
    req_data[0].vm_status.sum = req0_sum_i;
    req_data[0].vm_status.mxr = req0_mxr_i;
    req_data[0].vm_status.debug = req0_debug_i;
    req_data[0].addr = req0_addr_i;

    req_data[1] = '0;
    req_data[1].thread_id = req1_min_id_i;
    req_data[1].vm_status.prv = req1_prv_i;
    req_data[1].vm_status.mprv = req1_mprv_i;
    req_data[1].vm_status.mpp = req1_mpp_i;
    req_data[1].vm_status.sum = req1_sum_i;
    req_data[1].vm_status.mxr = req1_mxr_i;
    req_data[1].vm_status.debug = req1_debug_i;
    req_data[1].addr = req1_addr_i;

    satp_info[0] = '{mode: satp_mode0_i, ppn: satp_ppn0_i};
    satp_info[1] = '{mode: satp_mode1_i, ppn: satp_ppn1_i};
    matp_info[0] = '{mode: matp_mode0_i, ppn: matp_ppn0_i};
    matp_info[1] = '{mode: matp_mode1_i, ppn: matp_ppn1_i};

    ptw_resp_data[0] = '0;
    ptw_resp_data[0].canceled_req = ptw0_resp_canceled_req_i;
    ptw_resp_data[0].access_fault = ptw0_resp_access_fault_i;
    ptw_resp_data[0].rsvd_for_hw = ptw0_resp_rsvd_for_hw_i;
    ptw_resp_data[0].ppn = ptw0_resp_ppn_i;
    ptw_resp_data[0].rsvd_for_sw = ptw0_resp_rsvd_for_sw_i;
    ptw_resp_data[0].d = ptw0_resp_d_i;
    ptw_resp_data[0].a = ptw0_resp_a_i;
    ptw_resp_data[0].g = ptw0_resp_g_i;
    ptw_resp_data[0].u = ptw0_resp_u_i;
    ptw_resp_data[0].x = ptw0_resp_x_i;
    ptw_resp_data[0].w = ptw0_resp_w_i;
    ptw_resp_data[0].r = ptw0_resp_r_i;
    ptw_resp_data[0].v = ptw0_resp_v_i;

    ptw_resp_data[1] = '0;
    ptw_resp_data[1].canceled_req = ptw1_resp_canceled_req_i;
    ptw_resp_data[1].access_fault = ptw1_resp_access_fault_i;
    ptw_resp_data[1].rsvd_for_hw = ptw1_resp_rsvd_for_hw_i;
    ptw_resp_data[1].ppn = ptw1_resp_ppn_i;
    ptw_resp_data[1].rsvd_for_sw = ptw1_resp_rsvd_for_sw_i;
    ptw_resp_data[1].d = ptw1_resp_d_i;
    ptw_resp_data[1].a = ptw1_resp_a_i;
    ptw_resp_data[1].g = ptw1_resp_g_i;
    ptw_resp_data[1].u = ptw1_resp_u_i;
    ptw_resp_data[1].x = ptw1_resp_x_i;
    ptw_resp_data[1].w = ptw1_resp_w_i;
    ptw_resp_data[1].r = ptw1_resp_r_i;
    ptw_resp_data[1].v = ptw1_resp_v_i;

    l2_miss_resp = '0;
    l2_miss_resp.opcode = etlink_pkg::rsp_opcode_e'(l2_miss_resp_opcode_i);
    l2_miss_resp.size = etlink_pkg::SizeLine;

    sram_resp_dout[0] = {sram_resp_ecc[1], sram_resp_ecc[0], sram_resp_qw1_i, sram_resp_qw0_i};
    sram_resp_dout[1] = {sram_resp_ecc[3], sram_resp_ecc[2], sram_resp_qw3_i, sram_resp_qw2_i};
    sram_resp_dout[2] = {sram_resp_ecc[5], sram_resp_ecc[4], sram_resp_qw5_i, sram_resp_qw4_i};
    sram_resp_dout[3] = {sram_resp_ecc[7], sram_resp_ecc[6], sram_resp_qw7_i, sram_resp_qw6_i};

    req0_ready_o = f0_req_ready[0];
    req1_ready_o = f0_req_ready[1];

    resp0_valid_o = f4_resp_valid[0];
    resp0_miss_o = f4_resp_miss[0];
    resp0_data_0_o = resp_data[0].data[63:0];
    resp0_data_1_o = resp_data[0].data[127:64];
    resp0_data_2_o = resp_data[0].data[191:128];
    resp0_data_3_o = resp_data[0].data[255:192];
    resp0_page_fault_o = resp_data[0].page_fault;
    resp0_access_fault_o = resp_data[0].access_fault;
    resp0_cacheable_o = resp_data[0].cacheable;
    resp0_bus_err_o = resp_data[0].bus_err;
    resp0_ecc_err_o = resp_data[0].ecc_err;
    resp0_fill_done_o = f5_resp_fill_done[0];

    resp1_valid_o = f4_resp_valid[1];
    resp1_miss_o = f4_resp_miss[1];
    resp1_data_0_o = resp_data[1].data[63:0];
    resp1_data_1_o = resp_data[1].data[127:64];
    resp1_data_2_o = resp_data[1].data[191:128];
    resp1_data_3_o = resp_data[1].data[255:192];
    resp1_page_fault_o = resp_data[1].page_fault;
    resp1_access_fault_o = resp_data[1].access_fault;
    resp1_cacheable_o = resp_data[1].cacheable;
    resp1_bus_err_o = resp_data[1].bus_err;
    resp1_ecc_err_o = resp_data[1].ecc_err;
    resp1_fill_done_o = f5_resp_fill_done[1];

    ptw0_req_satp_mode_o = ptw_req_data[0].satp_mode;
    ptw0_req_satp_ppn_o = ptw_req_data[0].satp_ppn;
    ptw0_req_prv_o = ptw_req_data[0].prv;
    ptw0_req_addr_o = ptw_req_data[0].addr;
    ptw1_req_satp_mode_o = ptw_req_data[1].satp_mode;
    ptw1_req_satp_ppn_o = ptw_req_data[1].satp_ppn;
    ptw1_req_prv_o = ptw_req_data[1].prv;
    ptw1_req_addr_o = ptw_req_data[1].addr;

    l2_miss_req_addr_o = f0_l2_miss_req.address;
    l2_miss_req_opcode_o = f0_l2_miss_req.opcode;
    l2_miss_req_size_o = f0_l2_miss_req.size;
    l2_miss_req_subopcode_o = f0_l2_miss_req.subopcode;

    dbg_flat = dbg_sm_signals;
    dbg_flat_padded = {{(256 - $bits(icache_dbg_sm_t)){1'b0}}, dbg_flat};
    dbg_chunk0_o = dbg_flat_padded[63:0];
    dbg_chunk1_o = dbg_flat_padded[127:64];
    dbg_chunk2_o = dbg_flat_padded[191:128];
    dbg_chunk3_o = dbg_flat_padded[255:192];
  end

  for (genvar g = 0; g < 8; g++) begin : gen_sram_ecc
    prim_ecc_enc #(
      .DataWidth(64)
    ) u_sram_resp_ecc (
      .data_i((g == 0) ? sram_resp_qw0_i :
              (g == 1) ? sram_resp_qw1_i :
              (g == 2) ? sram_resp_qw2_i :
              (g == 3) ? sram_resp_qw3_i :
              (g == 4) ? sram_resp_qw4_i :
              (g == 5) ? sram_resp_qw5_i :
              (g == 6) ? sram_resp_qw6_i : sram_resp_qw7_i),
      .data_and_ecc_o(sram_resp_data_and_ecc[g])
    );

    assign sram_resp_ecc[g] = sram_resp_data_and_ecc[g][71:64];
  end

  logic [NrRequestors-1:0]       f0_req_ready;
  logic [NrRequestors-1:0]       f0_req_valid;
  logic [NrRequestors-1:0][0:0]  f0_req_min_id;
  logic [NrRequestors-1:0]       f4_resp_valid;
  logic [NrRequestors-1:0]       f4_resp_miss;
  logic [NrRequestors-1:0]       f5_resp_fill_done;
  /* verilator lint_off UNUSEDSIGNAL */  // The unit test checks selected fields explicitly; lint anchors keep inactive generic tech seams from appearing as extra tops.
  etlink_pkg::req_nodata_t       f0_l2_miss_req;
  logic                          lint_clk_gate_o;
  logic [7:0]                    lint_diff_preview_data;
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
    f0_req_valid[0] = req0_valid_i;
    f0_req_valid[1] = req1_valid_i;
    f0_req_min_id[0] = req0_min_id_i;
    f0_req_min_id[1] = req1_min_id_i;
  end

  icache_top #(
    .NrMinions   (NrMinions),
    .NrRequestors(NrRequestors)
  ) u_dut (
    .clk_i,
    .rst_ni,
    .ioshire_i,
    .esr_prefetch_conf_i         (prefetch_conf),
    .esr_prefetch_start_i        (prefetch_start_i),
    .esr_prefetch_done_o         (prefetch_done_o),
    .esr_err_log_ctl_i           (err_log_ctl),
    .esr_err_log_sbe_o           (err_log_sbe_o),
    .esr_err_log_dbe_o           (err_log_dbe_o),
    /* verilator lint_off PINCONNECTEMPTY */  // The unit test checks the error pulses directly; detailed log payload is covered by standalone cosim.
    .esr_err_log_info_o          (),
    /* verilator lint_on PINCONNECTEMPTY */
    .esr_mprot_i                 (mprot),
    .esr_vmspagesize_i           (vmspagesize),
    .esr_bypass_icache_i         (bypass_icache_i),
    .esr_shire_coop_mode_i       (coop_mode_i),
    .f0_req_ready_o              (f0_req_ready),
    .f0_req_valid_i              (f0_req_valid),
    .f0_req_i                    (req_data),
    .f0_req_min_id_i             (f0_req_min_id),
    .f4_resp_valid_o             (f4_resp_valid),
    .f4_resp_miss_o              (f4_resp_miss),
    .f4_resp_o                   (resp_data),
    .f5_resp_fill_done_o         (f5_resp_fill_done),
    .f0_flush_data_i             (flush_data_i),
    .f0_l2_miss_req_disable_next_i(l2_miss_req_disable_next_i),
    .f0_l2_miss_req_ready_i      (l2_miss_req_ready_i),
    .f0_l2_miss_req_valid_o      (l2_miss_req_valid_o),
    .f0_l2_miss_req_o            (f0_l2_miss_req),
    .f0_l2_miss_resp_ready_o     (l2_miss_resp_ready_o),
    .f0_l2_miss_resp_i           (l2_miss_resp),
    .f0_l2_miss_resp_valid_i     (l2_miss_resp_valid_i),
    .satp_info_i                 (satp_info),
    .matp_info_i                 (matp_info),
    .tlb_invalidate_i            (tlb_invalidate_i),
    .ptw_req_data_o              (ptw_req_data),
    .ptw_req_valid_o             ({ptw1_req_valid_o, ptw0_req_valid_o}),
    .ptw_req_ready_i             ({ptw1_req_ready_i, ptw0_req_ready_i}),
    .ptw_invalidate_o            ({ptw1_invalidate_o, ptw0_invalidate_o}),
    .ptw_resp_valid_i            ({ptw1_resp_valid_i, ptw0_resp_valid_i}),
    .ptw_resp_data_i             (ptw_resp_data),
    .f2_sram_req_write_o         (sram_req_write_o),
    .f2_sram_req_addr_o          (sram_req_addr_o),
    .f2_sram_req_valid_o         (sram_req_valid_o),
    .f2_sram_req_ready_i         (sram_req_ready_i),
    .f0_sram_resp_dout_i         (sram_resp_dout),
    .f0_sram_resp_valid_i        (sram_resp_valid_i),
    .f0_sram_resp_ready_o        (sram_resp_ready_o),
    .apb_paddr_i,
    .apb_pwrite_i,
    .apb_psel_i,
    .apb_penable_i,
    .apb_pwdata_i,
    .apb_pready_o,
    .apb_prdata_o,
    .apb_pslverr_o,
    .dbg_sm_signals_o            (dbg_sm_signals)
  );

endmodule : icache_top_tb
/* verilator lint_on SYNCASYNCNET */

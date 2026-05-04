// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

/* verilator lint_off SYNCASYNCNET */  // Testbench reset drives translated async-assert/sync-deassert flops and original-style sync logic.
module cosim_icache_top_tb
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
  input  logic                          err_rsp_enable_i,
  input  logic [2:0]                    err_interrupt_enable_i,
  output logic                          new_err_log_sbe_o,
  output logic                          orig_err_log_sbe_o,
  output logic                          new_err_log_dbe_o,
  output logic                          orig_err_log_dbe_o,
  output logic [IcacheWayAddrWidth-1:0] new_err_log_way_o,
  output logic [IcacheWayAddrWidth-1:0] orig_err_log_way_o,
  output logic [IcacheSetAddrWidth-1:0] new_err_log_set_o,
  output logic [IcacheSetAddrWidth-1:0] orig_err_log_set_o,
  output logic [IcacheEccBlocks-1:0]    new_err_log_sbe_bits_o,
  output logic [IcacheEccBlocks-1:0]    orig_err_log_sbe_bits_o,
  output logic [IcacheEccBlocks-1:0]    new_err_log_dbe_bits_o,
  output logic [IcacheEccBlocks-1:0]    orig_err_log_dbe_bits_o,
  output logic [PaSize-IcacheBlockAddrSize-1:0] new_err_log_addr_o,
  output logic [PaSize-IcacheBlockAddrSize-1:0] orig_err_log_addr_o,
  input  logic [$bits(esr_mprot_t)-1:0] mprot_i,
  input  logic [1:0]                    vmspagesize_i,
  input  logic                          bypass_icache_i,
  input  logic                          coop_mode_i,
  output logic                          new_req0_ready_o,
  output logic                          orig_req0_ready_o,
  output logic                          new_req1_ready_o,
  output logic                          orig_req1_ready_o,
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
  output logic                          new_resp0_valid_o,
  output logic                          orig_resp0_valid_o,
  output logic                          new_resp0_miss_o,
  output logic                          orig_resp0_miss_o,
  output logic [63:0]                   new_resp0_data_0_o,
  output logic [63:0]                   orig_resp0_data_0_o,
  output logic [63:0]                   new_resp0_data_1_o,
  output logic [63:0]                   orig_resp0_data_1_o,
  output logic [63:0]                   new_resp0_data_2_o,
  output logic [63:0]                   orig_resp0_data_2_o,
  output logic [63:0]                   new_resp0_data_3_o,
  output logic [63:0]                   orig_resp0_data_3_o,
  output logic                          new_resp0_page_fault_o,
  output logic                          orig_resp0_page_fault_o,
  output logic                          new_resp0_access_fault_o,
  output logic                          orig_resp0_access_fault_o,
  output logic                          new_resp0_cacheable_o,
  output logic                          orig_resp0_cacheable_o,
  output logic                          new_resp0_bus_err_o,
  output logic                          orig_resp0_bus_err_o,
  output logic                          new_resp0_ecc_err_o,
  output logic                          orig_resp0_ecc_err_o,
  output logic                          new_resp0_fill_done_o,
  output logic                          orig_resp0_fill_done_o,
  output logic                          new_resp1_valid_o,
  output logic                          orig_resp1_valid_o,
  output logic                          new_resp1_miss_o,
  output logic                          orig_resp1_miss_o,
  output logic [63:0]                   new_resp1_data_0_o,
  output logic [63:0]                   orig_resp1_data_0_o,
  output logic [63:0]                   new_resp1_data_1_o,
  output logic [63:0]                   orig_resp1_data_1_o,
  output logic [63:0]                   new_resp1_data_2_o,
  output logic [63:0]                   orig_resp1_data_2_o,
  output logic [63:0]                   new_resp1_data_3_o,
  output logic [63:0]                   orig_resp1_data_3_o,
  output logic                          new_resp1_page_fault_o,
  output logic                          orig_resp1_page_fault_o,
  output logic                          new_resp1_access_fault_o,
  output logic                          orig_resp1_access_fault_o,
  output logic                          new_resp1_cacheable_o,
  output logic                          orig_resp1_cacheable_o,
  output logic                          new_resp1_bus_err_o,
  output logic                          orig_resp1_bus_err_o,
  output logic                          new_resp1_ecc_err_o,
  output logic                          orig_resp1_ecc_err_o,
  output logic                          new_resp1_fill_done_o,
  output logic                          orig_resp1_fill_done_o,
  input  logic                          flush_data_i,
  input  logic                          l2_miss_req_disable_next_i,
  input  logic                          l2_miss_req_ready_i,
  output logic                          new_l2_miss_req_valid_o,
  output logic                          orig_l2_miss_req_valid_o,
  output logic [PaSize-1:0]             new_l2_miss_req_addr_o,
  output logic [PaSize-1:0]             orig_l2_miss_req_addr_o,
  output logic [4:0]                    new_l2_miss_req_opcode_o,
  output logic [4:0]                    orig_l2_miss_req_opcode_o,
  output logic [2:0]                    new_l2_miss_req_size_o,
  output logic [2:0]                    orig_l2_miss_req_size_o,
  output logic [6:0]                    new_l2_miss_req_subopcode_o,
  output logic [6:0]                    orig_l2_miss_req_subopcode_o,
  output logic                          new_l2_miss_resp_ready_o,
  output logic                          orig_l2_miss_resp_ready_o,
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
  output logic [CsrSatpModeSz-1:0]      new_ptw0_req_satp_mode_o,
  output logic [CsrSatpModeSz-1:0]      orig_ptw0_req_satp_mode_o,
  output logic [PaTransSize-1:0]        new_ptw0_req_satp_ppn_o,
  output logic [PaTransSize-1:0]        orig_ptw0_req_satp_ppn_o,
  output logic [1:0]                    new_ptw0_req_prv_o,
  output logic [1:0]                    orig_ptw0_req_prv_o,
  output logic [VaSize-1:VaUntransSize] new_ptw0_req_addr_o,
  output logic [VaSize-1:VaUntransSize] orig_ptw0_req_addr_o,
  output logic                          new_ptw0_req_valid_o,
  output logic                          orig_ptw0_req_valid_o,
  input  logic                          ptw0_req_ready_i,
  output logic                          new_ptw0_invalidate_o,
  output logic                          orig_ptw0_invalidate_o,
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
  output logic [CsrSatpModeSz-1:0]      new_ptw1_req_satp_mode_o,
  output logic [CsrSatpModeSz-1:0]      orig_ptw1_req_satp_mode_o,
  output logic [PaTransSize-1:0]        new_ptw1_req_satp_ppn_o,
  output logic [PaTransSize-1:0]        orig_ptw1_req_satp_ppn_o,
  output logic [1:0]                    new_ptw1_req_prv_o,
  output logic [1:0]                    orig_ptw1_req_prv_o,
  output logic [VaSize-1:VaUntransSize] new_ptw1_req_addr_o,
  output logic [VaSize-1:VaUntransSize] orig_ptw1_req_addr_o,
  output logic                          new_ptw1_req_valid_o,
  output logic                          orig_ptw1_req_valid_o,
  input  logic                          ptw1_req_ready_i,
  output logic                          new_ptw1_invalidate_o,
  output logic                          orig_ptw1_invalidate_o,
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
  output logic                          new_sram_req_write_o,
  output logic                          orig_sram_req_write_o,
  output logic [IcacheSramAddrWidth-1:0] new_sram_req_addr_o,
  output logic [IcacheSramAddrWidth-1:0] orig_sram_req_addr_o,
  output logic                          new_sram_req_valid_o,
  output logic                          orig_sram_req_valid_o,
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
  output logic                          new_sram_resp_ready_o,
  output logic                          orig_sram_resp_ready_o,
  input  logic [IcacheNeighApbAddrWidth-1:0] apb_paddr_i,
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
  localparam int unsigned NrRequestors = 2;

  logic                         reset_orig;
  icache_prefetch_conf_t        new_prefetch_conf;
  icache_prefetch_conf_t        orig_prefetch_conf;
  esr_icache_err_log_ctl_t      new_err_log_ctl;
  esr_icache_err_log_ctl_t      orig_err_log_ctl;
  esr_mprot_t                   new_mprot;
  esr_mprot_t                   orig_mprot;
  tlb_entry_type                new_vmspagesize;
  tlb_entry_type                orig_vmspagesize;
  frontend_icache_req_t         new_req_data [NrRequestors-1:0];
  frontend_icache_req [NrRequestors-1:0] orig_req_data;
  logic [NrRequestors-1:0]      req_valid_vec;
  logic [NrRequestors-1:0][0:0] req_min_id_vec;
  minion_satp_info              new_satp_info [NrMinions-1:0];
  minion_satp_info              new_matp_info [NrMinions-1:0];
  minion_satp_info              orig_satp_info [NrMinions-1:0];
  minion_satp_info              orig_matp_info [NrMinions-1:0];
  logic [(NrMinions * $bits(minion_satp_info))-1:0] orig_satp_info_flat;
  logic [(NrMinions * $bits(minion_satp_info))-1:0] orig_matp_info_flat;
  minion_ptw_pte                new_ptw_resp_data [NrRequestors-1:0];
  minion_ptw_pte [NrRequestors-1:0] orig_ptw_resp_data;
  minion_ptw_req                new_ptw_req_data [NrRequestors-1:0];
  minion_ptw_req [NrRequestors-1:0] orig_ptw_req_data;
  icache_frontend_resp_t        new_resp_data [NrRequestors-1:0];
  icache_frontend_resp [NrRequestors-1:0] orig_resp_data;
  logic [NrRequestors-1:0]      new_req_ready;
  logic [NrRequestors-1:0]      orig_req_ready;
  logic [NrRequestors-1:0]      new_resp_valid;
  logic [NrRequestors-1:0]      orig_resp_valid;
  logic [NrRequestors-1:0]      new_resp_miss;
  logic [NrRequestors-1:0]      orig_resp_miss;
  logic [NrRequestors-1:0]      new_fill_done;
  logic [NrRequestors-1:0]      orig_fill_done;
  logic [NrRequestors-1:0]      new_ptw_req_valid;
  logic [NrRequestors-1:0]      orig_ptw_req_valid;
  logic [NrRequestors-1:0]      new_ptw_invalidate;
  logic [NrRequestors-1:0]      orig_ptw_invalidate;
  etlink_pkg::rsp_t             new_l2_miss_resp;
  et_link_rsp_info_t            orig_l2_miss_resp;
  etlink_pkg::req_nodata_t      new_l2_miss_req;
  et_link_nodata_req_info_t     orig_l2_miss_req;
  logic [7:0][71:0]             sram_resp_data_and_ecc;
  logic [7:0]                   sram_resp_ecc [7:0];
  logic [IcacheNrMemsPerRow-1:0][IcacheMemWidth-1:0] new_sram_resp_dout;
  logic [IcacheSramDataWidth-1:0] orig_sram_resp_dout_flat;
  icache_err_log_info_t         new_err_log_info;
  icache_err_log_info_t         orig_err_log_info;
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

    new_err_log_ctl = '0;
    new_err_log_ctl.err_rsp_enable = err_rsp_enable_i;
    new_err_log_ctl.err_interrupt_enable = err_interrupt_enable_i;
    orig_err_log_ctl = '{
      err_rsp_enable: err_rsp_enable_i,
      err_interrupt_enable: err_interrupt_enable_i
    };

    new_mprot = esr_mprot_t'(mprot_i);
    orig_mprot = esr_mprot_t'(mprot_i);
    new_vmspagesize = tlb_entry_type'(vmspagesize_i);
    orig_vmspagesize = tlb_entry_type'(vmspagesize_i);

    req_valid_vec[0] = req0_valid_i;
    req_valid_vec[1] = req1_valid_i;
    req_min_id_vec[0] = req0_min_id_i;
    req_min_id_vec[1] = req1_min_id_i;

    new_req_data[0] = '0;
    new_req_data[0].thread_id = req0_min_id_i;
    new_req_data[0].vm_status.prv = req0_prv_i;
    new_req_data[0].vm_status.mprv = req0_mprv_i;
    new_req_data[0].vm_status.mpp = req0_mpp_i;
    new_req_data[0].vm_status.sum = req0_sum_i;
    new_req_data[0].vm_status.mxr = req0_mxr_i;
    new_req_data[0].vm_status.debug = req0_debug_i;
    new_req_data[0].addr = req0_addr_i;
    new_req_data[1] = '0;
    new_req_data[1].thread_id = req1_min_id_i;
    new_req_data[1].vm_status.prv = req1_prv_i;
    new_req_data[1].vm_status.mprv = req1_mprv_i;
    new_req_data[1].vm_status.mpp = req1_mpp_i;
    new_req_data[1].vm_status.sum = req1_sum_i;
    new_req_data[1].vm_status.mxr = req1_mxr_i;
    new_req_data[1].vm_status.debug = req1_debug_i;
    new_req_data[1].addr = req1_addr_i;

    orig_req_data[0] = '{
      thread_id: req0_min_id_i,
      vm_status: '{
        prv: req0_prv_i,
        mprv: req0_mprv_i,
        mpp: req0_mpp_i,
        sum: req0_sum_i,
        mxr: req0_mxr_i,
        debug: req0_debug_i
      },
      addr: req0_addr_i
    };
    orig_req_data[1] = '{
      thread_id: req1_min_id_i,
      vm_status: '{
        prv: req1_prv_i,
        mprv: req1_mprv_i,
        mpp: req1_mpp_i,
        sum: req1_sum_i,
        mxr: req1_mxr_i,
        debug: req1_debug_i
      },
      addr: req1_addr_i
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

    new_ptw_resp_data[0] = '0;
    new_ptw_resp_data[0].canceled_req = ptw0_resp_canceled_req_i;
    new_ptw_resp_data[0].access_fault = ptw0_resp_access_fault_i;
    new_ptw_resp_data[0].rsvd_for_hw = ptw0_resp_rsvd_for_hw_i;
    new_ptw_resp_data[0].ppn = ptw0_resp_ppn_i;
    new_ptw_resp_data[0].rsvd_for_sw = ptw0_resp_rsvd_for_sw_i;
    new_ptw_resp_data[0].d = ptw0_resp_d_i;
    new_ptw_resp_data[0].a = ptw0_resp_a_i;
    new_ptw_resp_data[0].g = ptw0_resp_g_i;
    new_ptw_resp_data[0].u = ptw0_resp_u_i;
    new_ptw_resp_data[0].x = ptw0_resp_x_i;
    new_ptw_resp_data[0].w = ptw0_resp_w_i;
    new_ptw_resp_data[0].r = ptw0_resp_r_i;
    new_ptw_resp_data[0].v = ptw0_resp_v_i;
    new_ptw_resp_data[1] = '0;
    new_ptw_resp_data[1].canceled_req = ptw1_resp_canceled_req_i;
    new_ptw_resp_data[1].access_fault = ptw1_resp_access_fault_i;
    new_ptw_resp_data[1].rsvd_for_hw = ptw1_resp_rsvd_for_hw_i;
    new_ptw_resp_data[1].ppn = ptw1_resp_ppn_i;
    new_ptw_resp_data[1].rsvd_for_sw = ptw1_resp_rsvd_for_sw_i;
    new_ptw_resp_data[1].d = ptw1_resp_d_i;
    new_ptw_resp_data[1].a = ptw1_resp_a_i;
    new_ptw_resp_data[1].g = ptw1_resp_g_i;
    new_ptw_resp_data[1].u = ptw1_resp_u_i;
    new_ptw_resp_data[1].x = ptw1_resp_x_i;
    new_ptw_resp_data[1].w = ptw1_resp_w_i;
    new_ptw_resp_data[1].r = ptw1_resp_r_i;
    new_ptw_resp_data[1].v = ptw1_resp_v_i;
    orig_ptw_resp_data[0] = new_ptw_resp_data[0];
    orig_ptw_resp_data[1] = new_ptw_resp_data[1];

    new_l2_miss_resp = '0;
    new_l2_miss_resp.opcode = etlink_pkg::rsp_opcode_e'(l2_miss_resp_opcode_i);
    new_l2_miss_resp.size = etlink_pkg::SizeLine;
    orig_l2_miss_resp = '0;
    orig_l2_miss_resp.opcode = et_link_rsp_opcode_t'(l2_miss_resp_opcode_i);
    orig_l2_miss_resp.size = ET_LINK_Line;

    new_sram_resp_dout[0] = {sram_resp_ecc[1], sram_resp_ecc[0], sram_resp_qw1_i, sram_resp_qw0_i};
    new_sram_resp_dout[1] = {sram_resp_ecc[3], sram_resp_ecc[2], sram_resp_qw3_i, sram_resp_qw2_i};
    new_sram_resp_dout[2] = {sram_resp_ecc[5], sram_resp_ecc[4], sram_resp_qw5_i, sram_resp_qw4_i};
    new_sram_resp_dout[3] = {sram_resp_ecc[7], sram_resp_ecc[6], sram_resp_qw7_i, sram_resp_qw6_i};
    orig_sram_resp_dout_flat = {
      new_sram_resp_dout[3],
      new_sram_resp_dout[2],
      new_sram_resp_dout[1],
      new_sram_resp_dout[0]
    };

    new_req0_ready_o = new_req_ready[0];
    orig_req0_ready_o = orig_req_ready[0];
    new_req1_ready_o = new_req_ready[1];
    orig_req1_ready_o = orig_req_ready[1];

    new_resp0_valid_o = new_resp_valid[0];
    orig_resp0_valid_o = orig_resp_valid[0];
    new_resp0_miss_o = new_resp_miss[0];
    orig_resp0_miss_o = orig_resp_miss[0];
    new_resp0_data_0_o = new_resp_data[0].data[63:0];
    orig_resp0_data_0_o = orig_resp_data[0].data[63:0];
    new_resp0_data_1_o = new_resp_data[0].data[127:64];
    orig_resp0_data_1_o = orig_resp_data[0].data[127:64];
    new_resp0_data_2_o = new_resp_data[0].data[191:128];
    orig_resp0_data_2_o = orig_resp_data[0].data[191:128];
    new_resp0_data_3_o = new_resp_data[0].data[255:192];
    orig_resp0_data_3_o = orig_resp_data[0].data[255:192];
    new_resp0_page_fault_o = new_resp_data[0].page_fault;
    orig_resp0_page_fault_o = orig_resp_data[0].page_fault;
    new_resp0_access_fault_o = new_resp_data[0].access_fault;
    orig_resp0_access_fault_o = orig_resp_data[0].access_fault;
    new_resp0_cacheable_o = new_resp_data[0].cacheable;
    orig_resp0_cacheable_o = orig_resp_data[0].cacheable;
    new_resp0_bus_err_o = new_resp_data[0].bus_err;
    orig_resp0_bus_err_o = orig_resp_data[0].bus_err;
    new_resp0_ecc_err_o = new_resp_data[0].ecc_err;
    orig_resp0_ecc_err_o = orig_resp_data[0].ecc_err;
    new_resp0_fill_done_o = new_fill_done[0];
    orig_resp0_fill_done_o = orig_fill_done[0];

    new_resp1_valid_o = new_resp_valid[1];
    orig_resp1_valid_o = orig_resp_valid[1];
    new_resp1_miss_o = new_resp_miss[1];
    orig_resp1_miss_o = orig_resp_miss[1];
    new_resp1_data_0_o = new_resp_data[1].data[63:0];
    orig_resp1_data_0_o = orig_resp_data[1].data[63:0];
    new_resp1_data_1_o = new_resp_data[1].data[127:64];
    orig_resp1_data_1_o = orig_resp_data[1].data[127:64];
    new_resp1_data_2_o = new_resp_data[1].data[191:128];
    orig_resp1_data_2_o = orig_resp_data[1].data[191:128];
    new_resp1_data_3_o = new_resp_data[1].data[255:192];
    orig_resp1_data_3_o = orig_resp_data[1].data[255:192];
    new_resp1_page_fault_o = new_resp_data[1].page_fault;
    orig_resp1_page_fault_o = orig_resp_data[1].page_fault;
    new_resp1_access_fault_o = new_resp_data[1].access_fault;
    orig_resp1_access_fault_o = orig_resp_data[1].access_fault;
    new_resp1_cacheable_o = new_resp_data[1].cacheable;
    orig_resp1_cacheable_o = orig_resp_data[1].cacheable;
    new_resp1_bus_err_o = new_resp_data[1].bus_err;
    orig_resp1_bus_err_o = orig_resp_data[1].bus_err;
    new_resp1_ecc_err_o = new_resp_data[1].ecc_err;
    orig_resp1_ecc_err_o = orig_resp_data[1].ecc_err;
    new_resp1_fill_done_o = new_fill_done[1];
    orig_resp1_fill_done_o = orig_fill_done[1];

    new_l2_miss_req_addr_o = new_l2_miss_req.address;
    orig_l2_miss_req_addr_o = orig_l2_miss_req.address;
    new_l2_miss_req_opcode_o = new_l2_miss_req.opcode;
    orig_l2_miss_req_opcode_o = orig_l2_miss_req.opcode;
    new_l2_miss_req_size_o = new_l2_miss_req.size;
    orig_l2_miss_req_size_o = orig_l2_miss_req.size;
    new_l2_miss_req_subopcode_o = new_l2_miss_req.subopcode;
    orig_l2_miss_req_subopcode_o = orig_l2_miss_req.subopcode;

    new_ptw0_req_satp_mode_o = new_ptw_req_data[0].satp_mode;
    orig_ptw0_req_satp_mode_o = orig_ptw_req_data[0].satp_mode;
    new_ptw0_req_satp_ppn_o = new_ptw_req_data[0].satp_ppn;
    orig_ptw0_req_satp_ppn_o = orig_ptw_req_data[0].satp_ppn;
    new_ptw0_req_prv_o = new_ptw_req_data[0].prv;
    orig_ptw0_req_prv_o = orig_ptw_req_data[0].prv;
    new_ptw0_req_addr_o = new_ptw_req_data[0].addr;
    orig_ptw0_req_addr_o = orig_ptw_req_data[0].addr;
    new_ptw0_req_valid_o = new_ptw_req_valid[0];
    orig_ptw0_req_valid_o = orig_ptw_req_valid[0];
    new_ptw0_invalidate_o = new_ptw_invalidate[0];
    orig_ptw0_invalidate_o = orig_ptw_invalidate[0];

    new_ptw1_req_satp_mode_o = new_ptw_req_data[1].satp_mode;
    orig_ptw1_req_satp_mode_o = orig_ptw_req_data[1].satp_mode;
    new_ptw1_req_satp_ppn_o = new_ptw_req_data[1].satp_ppn;
    orig_ptw1_req_satp_ppn_o = orig_ptw_req_data[1].satp_ppn;
    new_ptw1_req_prv_o = new_ptw_req_data[1].prv;
    orig_ptw1_req_prv_o = orig_ptw_req_data[1].prv;
    new_ptw1_req_addr_o = new_ptw_req_data[1].addr;
    orig_ptw1_req_addr_o = orig_ptw_req_data[1].addr;
    new_ptw1_req_valid_o = new_ptw_req_valid[1];
    orig_ptw1_req_valid_o = orig_ptw_req_valid[1];
    new_ptw1_invalidate_o = new_ptw_invalidate[1];
    orig_ptw1_invalidate_o = orig_ptw_invalidate[1];

    new_err_log_way_o = new_err_log_info.way;
    orig_err_log_way_o = orig_err_log_info.way;
    new_err_log_set_o = new_err_log_info.set;
    orig_err_log_set_o = orig_err_log_info.set;
    new_err_log_sbe_bits_o = new_err_log_info.sbe_per_block;
    orig_err_log_sbe_bits_o = orig_err_log_info.sbe_per_block;
    new_err_log_dbe_bits_o = new_err_log_info.dbe_per_block;
    orig_err_log_dbe_bits_o = orig_err_log_info.dbe_per_block;
    new_err_log_addr_o = new_err_log_info.address;
    orig_err_log_addr_o = orig_err_log_info.address;

    new_dbg_flat = '0;
    new_dbg_flat[$bits(icache_dbg_sm_t)-1:0] = new_dbg_sm_signals;
    orig_dbg_flat = '0;
    orig_dbg_flat[$bits(icache_dbg_sm_t)-1:0] = orig_dbg_sm_signals;
    new_dbg_chunk0_o = new_dbg_flat[63:0];
    orig_dbg_chunk0_o = orig_dbg_flat[63:0];
    new_dbg_chunk1_o = new_dbg_flat[127:64];
    orig_dbg_chunk1_o = orig_dbg_flat[127:64];
    new_dbg_chunk2_o = new_dbg_flat[191:128];
    orig_dbg_chunk2_o = orig_dbg_flat[191:128];
    new_dbg_chunk3_o = new_dbg_flat[255:192];
    orig_dbg_chunk3_o = orig_dbg_flat[255:192];
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

  icache_top #(
    .NrMinions   (NrMinions),
    .NrRequestors(NrRequestors)
  ) u_new (
    .clk_i,
    .rst_ni,
    .ioshire_i,
    .esr_prefetch_conf_i         (new_prefetch_conf),
    .esr_prefetch_start_i        (prefetch_start_i),
    .esr_prefetch_done_o         (new_prefetch_done_o),
    .esr_err_log_ctl_i           (new_err_log_ctl),
    .esr_err_log_sbe_o           (new_err_log_sbe_o),
    .esr_err_log_dbe_o           (new_err_log_dbe_o),
    .esr_err_log_info_o          (new_err_log_info),
    .esr_mprot_i                 (new_mprot),
    .esr_vmspagesize_i           (new_vmspagesize),
    .esr_bypass_icache_i         (bypass_icache_i),
    .esr_shire_coop_mode_i       (coop_mode_i),
    .f0_req_ready_o              (new_req_ready),
    .f0_req_valid_i              (req_valid_vec),
    .f0_req_i                    (new_req_data),
    .f0_req_min_id_i             (req_min_id_vec),
    .f4_resp_valid_o             (new_resp_valid),
    .f4_resp_miss_o              (new_resp_miss),
    .f4_resp_o                   (new_resp_data),
    .f5_resp_fill_done_o         (new_fill_done),
    .f0_flush_data_i             (flush_data_i),
    .f0_l2_miss_req_disable_next_i(l2_miss_req_disable_next_i),
    .f0_l2_miss_req_ready_i      (l2_miss_req_ready_i),
    .f0_l2_miss_req_valid_o      (new_l2_miss_req_valid_o),
    .f0_l2_miss_req_o            (new_l2_miss_req),
    .f0_l2_miss_resp_ready_o     (new_l2_miss_resp_ready_o),
    .f0_l2_miss_resp_i           (new_l2_miss_resp),
    .f0_l2_miss_resp_valid_i     (l2_miss_resp_valid_i),
    .satp_info_i                 (new_satp_info),
    .matp_info_i                 (new_matp_info),
    .tlb_invalidate_i            (tlb_invalidate_i),
    .ptw_req_data_o              (new_ptw_req_data),
    .ptw_req_valid_o             (new_ptw_req_valid),
    .ptw_req_ready_i             ({ptw1_req_ready_i, ptw0_req_ready_i}),
    .ptw_invalidate_o            (new_ptw_invalidate),
    .ptw_resp_valid_i            ({ptw1_resp_valid_i, ptw0_resp_valid_i}),
    .ptw_resp_data_i             (new_ptw_resp_data),
    .f2_sram_req_write_o         (new_sram_req_write_o),
    .f2_sram_req_addr_o          (new_sram_req_addr_o),
    .f2_sram_req_valid_o         (new_sram_req_valid_o),
    .f2_sram_req_ready_i         (sram_req_ready_i),
    .f0_sram_resp_dout_i         (new_sram_resp_dout),
    .f0_sram_resp_valid_i        (sram_resp_valid_i),
    .f0_sram_resp_ready_o        (new_sram_resp_ready_o),
    .apb_paddr_i,
    .apb_pwrite_i,
    .apb_psel_i,
    .apb_penable_i,
    .apb_pwdata_i,
    .apb_pready_o                (new_apb_pready_o),
    .apb_prdata_o                (new_apb_prdata_o),
    .apb_pslverr_o               (new_apb_pslverr_o),
    .dbg_sm_signals_o            (new_dbg_sm_signals)
  );

  icache_top_orig #(
    .NR_MINIONS       (NrMinions),
    .NR_REQUESTORS    (NrRequestors),
    .NR_MIN_PER_REQ   (1),
    .NR_MIN_PER_REQ_L (1)
  ) u_orig (
    .clock                    (clk_i),
    .reset                    (reset_orig),
    .ioshire                  (ioshire_i),
    .esr_prefetch_conf        (orig_prefetch_conf),
    .esr_prefetch_start       (prefetch_start_i),
    .esr_prefetch_done        (orig_prefetch_done_o),
    .esr_err_log_ctl          (orig_err_log_ctl),
    .esr_err_log_sbe          (orig_err_log_sbe_o),
    .esr_err_log_dbe          (orig_err_log_dbe_o),
    .esr_err_log_info         (orig_err_log_info),
    .esr_mprot                (orig_mprot),
    .esr_vmspagesize          (orig_vmspagesize),
    .esr_bypass_icache        (bypass_icache_i),
    .esr_shire_coop_mode      (coop_mode_i),
    .f0_req_ready             (orig_req_ready),
    .f0_req_valid             (req_valid_vec),
    .f0_req                   (orig_req_data),
    .f0_req_min_id            (req_min_id_vec),
    .f4_resp_valid            (orig_resp_valid),
    .f4_resp_miss             (orig_resp_miss),
    .f4_resp                  (orig_resp_data),
    .f5_resp_fill_done        (orig_fill_done),
    .f0_flush_data            (flush_data_i),
    .f0_l2_miss_req_disable_next(l2_miss_req_disable_next_i),
    .f0_l2_miss_req_ready     (l2_miss_req_ready_i),
    .f0_l2_miss_req_valid     (orig_l2_miss_req_valid_o),
    .f0_l2_miss_req           (orig_l2_miss_req),
    .f0_l2_miss_resp_ready    (orig_l2_miss_resp_ready_o),
    .f0_l2_miss_resp_valid    (l2_miss_resp_valid_i),
    .f0_l2_miss_resp          (orig_l2_miss_resp),
    .satp_info                (orig_satp_info_flat),
    .matp_info                (orig_matp_info_flat),
    .tlb_invalidate           (tlb_invalidate_i),
    .ptw_req_data             (orig_ptw_req_data),
    .ptw_req_valid            (orig_ptw_req_valid),
    .ptw_req_ready            ({ptw1_req_ready_i, ptw0_req_ready_i}),
    .ptw_invalidate           (orig_ptw_invalidate),
    .ptw_resp_valid           ({ptw1_resp_valid_i, ptw0_resp_valid_i}),
    .ptw_resp_data            (orig_ptw_resp_data),
    .f2_sram_req_write        (orig_sram_req_write_o),
    .f2_sram_req_addr         (orig_sram_req_addr_o),
    .f2_sram_req_valid        (orig_sram_req_valid_o),
    .f2_sram_req_ready        (sram_req_ready_i),
    .f0_sram_resp_dout        (orig_sram_resp_dout_flat),
    .f0_sram_resp_valid       (sram_resp_valid_i),
    .f0_sram_resp_ready       (orig_sram_resp_ready_o),
    .apb_paddr                (apb_paddr_i),
    .apb_pwrite               (apb_pwrite_i),
    .apb_psel                 (apb_psel_i),
    .apb_penable              (apb_penable_i),
    .apb_pwdata               (apb_pwdata_i),
    .apb_pready               (orig_apb_pready_o),
    .apb_prdata               (orig_apb_prdata_o),
    .apb_pslverr              (orig_apb_pslverr_o),
    .dbg_sm_signals           (orig_dbg_sm_signals)
  );

endmodule : cosim_icache_top_tb
/* verilator lint_on SYNCASYNCNET */

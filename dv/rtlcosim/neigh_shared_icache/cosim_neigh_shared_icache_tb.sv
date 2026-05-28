// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

/* verilator lint_off SYNCASYNCNET */  // Testbench reset feeds original sync reset and translated async reset.
module cosim_neigh_shared_icache_tb
  import minion_pkg::*;
  import minion_dcache_pkg::*;
  import minion_frontend_pkg::*;
  import icache_geom_pkg::*;
  import icache_pkg::*;
(
  input  logic                          clk_i,
  input  logic                          rst_ni,

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

  input  logic [7:0]                    req_valid_i,
  input  logic [7:0]                    req_thread_id_i,
  input  logic [63:0]                   req_addr0_i,
  input  logic [63:0]                   req_addr1_i,
  input  logic [63:0]                   req_addr2_i,
  input  logic [63:0]                   req_addr3_i,
  input  logic [63:0]                   req_addr4_i,
  input  logic [63:0]                   req_addr5_i,
  input  logic [63:0]                   req_addr6_i,
  input  logic [63:0]                   req_addr7_i,
  input  logic [1:0]                    req_prv_i,
  input  logic                          req_mprv_i,
  input  logic [1:0]                    req_mpp_i,
  input  logic                          req_sum_i,
  input  logic                          req_mxr_i,
  input  logic                          req_debug_i,
  output logic [7:0]                    new_req_ready_o,
  output logic [7:0]                    orig_req_ready_o,

  output logic [1:0]                    new_resp_valid_o,
  output logic [1:0]                    orig_resp_valid_o,
  output logic [1:0]                    new_resp_miss_o,
  output logic [1:0]                    orig_resp_miss_o,
  output logic [63:0]                   new_resp0_data_0_o,
  output logic [63:0]                   orig_resp0_data_0_o,
  output logic [63:0]                   new_resp0_data_1_o,
  output logic [63:0]                   orig_resp0_data_1_o,
  output logic [63:0]                   new_resp0_data_2_o,
  output logic [63:0]                   orig_resp0_data_2_o,
  output logic [63:0]                   new_resp0_data_3_o,
  output logic [63:0]                   orig_resp0_data_3_o,
  output logic [4:0]                    new_resp0_flags_o,
  output logic [4:0]                    orig_resp0_flags_o,
  output logic [63:0]                   new_resp1_data_0_o,
  output logic [63:0]                   orig_resp1_data_0_o,
  output logic [63:0]                   new_resp1_data_1_o,
  output logic [63:0]                   orig_resp1_data_1_o,
  output logic [63:0]                   new_resp1_data_2_o,
  output logic [63:0]                   orig_resp1_data_2_o,
  output logic [63:0]                   new_resp1_data_3_o,
  output logic [63:0]                   orig_resp1_data_3_o,
  output logic [4:0]                    new_resp1_flags_o,
  output logic [4:0]                    orig_resp1_flags_o,
  output logic [1:0]                    new_fill_done_o,
  output logic [1:0]                    orig_fill_done_o,

  input  logic [7:0]                    flush_data_i,
  input  logic                          l2_miss_req_disable_next_i,
  input  logic                          l2_miss_req_ready_i,
  output logic                          new_l2_miss_req_valid_o,
  output logic                          orig_l2_miss_req_valid_o,
  output logic [7:0]                    new_l2_miss_req_id_o,
  output logic [7:0]                    orig_l2_miss_req_id_o,
  output logic [3:0]                    new_l2_miss_req_source_o,
  output logic [3:0]                    orig_l2_miss_req_source_o,
  output logic                          new_l2_miss_req_wdata_o,
  output logic                          orig_l2_miss_req_wdata_o,
  output logic [4:0]                    new_l2_miss_req_opcode_o,
  output logic [4:0]                    orig_l2_miss_req_opcode_o,
  output logic [PaSize-1:0]             new_l2_miss_req_addr_o,
  output logic [PaSize-1:0]             orig_l2_miss_req_addr_o,
  output logic [2:0]                    new_l2_miss_req_size_o,
  output logic [2:0]                    orig_l2_miss_req_size_o,
  output logic [3:0]                    new_l2_miss_req_qwen_o,
  output logic [3:0]                    orig_l2_miss_req_qwen_o,
  output logic [6:0]                    new_l2_miss_req_subopcode_o,
  output logic [6:0]                    orig_l2_miss_req_subopcode_o,
  output logic                          new_l2_miss_resp_ready_o,
  output logic                          orig_l2_miss_resp_ready_o,
  input  logic                          l2_miss_resp_valid_i,
  input  logic [1:0]                    l2_miss_resp_opcode_i,
  input  logic [63:0]                   l2_resp_qw0_i,
  input  logic [63:0]                   l2_resp_qw1_i,
  input  logic [63:0]                   l2_resp_qw2_i,
  input  logic [63:0]                   l2_resp_qw3_i,
  input  logic [63:0]                   l2_resp_qw4_i,
  input  logic [63:0]                   l2_resp_qw5_i,
  input  logic [63:0]                   l2_resp_qw6_i,
  input  logic [63:0]                   l2_resp_qw7_i,

  input  logic [31:0]                   satp_mode_pack_i,
  input  logic [63:0]                   satp_ppn_seed_i,
  input  logic [31:0]                   matp_mode_pack_i,
  input  logic [63:0]                   matp_ppn_seed_i,
  input  logic [7:0]                    tlb_invalidate_i,
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
  output logic                          new_ptw0_invalidate_o,
  output logic                          orig_ptw0_invalidate_o,
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
  output logic                          new_ptw1_invalidate_o,
  output logic                          orig_ptw1_invalidate_o,
  input  logic [1:0]                    ptw_req_ready_i,
  input  logic [1:0]                    ptw_resp_valid_i,
  input  logic [31:0]                   ptw_resp_flags_i,
  input  logic [63:0]                   ptw0_resp_ppn_i,
  input  logic [63:0]                   ptw1_resp_ppn_i,

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

  localparam int unsigned NrMinions = 8;
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
  frontend_icache_req_t         new_req_data [NrMinions-1:0];
  frontend_icache_req [NrMinions-1:0] orig_req_data;
  logic [NrMinions-1:0][VaSizeExt-1:0] req_addr;
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
  logic [NrRequestors-1:0]      new_resp_valid;
  logic [NrRequestors-1:0]      orig_resp_valid;
  logic [NrRequestors-1:0]      new_resp_miss;
  logic [NrRequestors-1:0]      orig_resp_miss;
  etlink_pkg::rsp_t             new_l2_miss_resp;
  et_link_rsp_info_t            orig_l2_miss_resp;
  etlink_pkg::req_nodata_t      new_l2_miss_req;
  et_link_nodata_req_info_t     orig_l2_miss_req;
  logic [7:0][71:0]             sram_resp_data_and_ecc;
  logic [7:0][7:0]              sram_resp_ecc;
  logic [IcacheNrMemsPerRow-1:0][IcacheMemWidth-1:0] new_sram_resp_dout;
  logic [IcacheSramDataWidth-1:0] orig_sram_resp_dout_flat;
  icache_err_log_info_t         new_err_log_info;
  icache_err_log_info_t         orig_err_log_info;
  icache_dbg_sm_t               new_dbg_sm_signals;
  icache_dbg_sm_t               orig_dbg_sm_signals;
  logic [255:0]                 new_dbg_flat;
  logic [255:0]                 orig_dbg_flat;

  assign req_addr[0] = req_addr0_i[VaSizeExt-1:0];
  assign req_addr[1] = req_addr1_i[VaSizeExt-1:0];
  assign req_addr[2] = req_addr2_i[VaSizeExt-1:0];
  assign req_addr[3] = req_addr3_i[VaSizeExt-1:0];
  assign req_addr[4] = req_addr4_i[VaSizeExt-1:0];
  assign req_addr[5] = req_addr5_i[VaSizeExt-1:0];
  assign req_addr[6] = req_addr6_i[VaSizeExt-1:0];
  assign req_addr[7] = req_addr7_i[VaSizeExt-1:0];

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

    for (int unsigned minion = 0; minion < NrMinions; minion++) begin
      new_req_data[minion] = '0;
      new_req_data[minion].thread_id = req_thread_id_i[minion];
      new_req_data[minion].vm_status.prv = req_prv_i;
      new_req_data[minion].vm_status.mprv = req_mprv_i;
      new_req_data[minion].vm_status.mpp = req_mpp_i;
      new_req_data[minion].vm_status.sum = req_sum_i;
      new_req_data[minion].vm_status.mxr = req_mxr_i;
      new_req_data[minion].vm_status.debug = req_debug_i;
      new_req_data[minion].addr = req_addr[minion];

      orig_req_data[minion] = '{
        thread_id: req_thread_id_i[minion],
        vm_status: '{
          prv: req_prv_i,
          mprv: req_mprv_i,
          mpp: req_mpp_i,
          sum: req_sum_i,
          mxr: req_mxr_i,
          debug: req_debug_i
        },
        addr: req_addr[minion]
      };

      new_satp_info[minion] = '{
        mode: satp_mode_pack_i[minion*4 +: CsrSatpModeSz],
        ppn: (satp_ppn_seed_i[PaTransSize-1:0] ^ minion[PaTransSize-1:0])
      };
      new_matp_info[minion] = '{
        mode: matp_mode_pack_i[minion*4 +: CsrSatpModeSz],
        ppn: (matp_ppn_seed_i[PaTransSize-1:0] ^ {minion[PaTransSize-2:0], 1'b1})
      };
      orig_satp_info[minion] = new_satp_info[minion];
      orig_matp_info[minion] = new_matp_info[minion];
    end
    orig_satp_info_flat = {orig_satp_info[7], orig_satp_info[6], orig_satp_info[5], orig_satp_info[4],
                           orig_satp_info[3], orig_satp_info[2], orig_satp_info[1], orig_satp_info[0]};
    orig_matp_info_flat = {orig_matp_info[7], orig_matp_info[6], orig_matp_info[5], orig_matp_info[4],
                           orig_matp_info[3], orig_matp_info[2], orig_matp_info[1], orig_matp_info[0]};

    for (int unsigned req_idx = 0; req_idx < NrRequestors; req_idx++) begin
      new_ptw_resp_data[req_idx] = '0;
      new_ptw_resp_data[req_idx].canceled_req = ptw_resp_flags_i[req_idx*16 + 0];
      new_ptw_resp_data[req_idx].access_fault = ptw_resp_flags_i[req_idx*16 + 1];
      new_ptw_resp_data[req_idx].rsvd_for_hw = ptw_resp_flags_i[req_idx*16 + 9 -: 8];
      new_ptw_resp_data[req_idx].rsvd_for_sw = ptw_resp_flags_i[req_idx*16 + 11 -: 2];
      new_ptw_resp_data[req_idx].d = ptw_resp_flags_i[req_idx*16 + 12];
      new_ptw_resp_data[req_idx].a = ptw_resp_flags_i[req_idx*16 + 13];
      new_ptw_resp_data[req_idx].g = ptw_resp_flags_i[req_idx*16 + 14];
      new_ptw_resp_data[req_idx].u = ptw_resp_flags_i[req_idx*16 + 15];
      new_ptw_resp_data[req_idx].x = ptw_resp_flags_i[req_idx*16 + 4];
      new_ptw_resp_data[req_idx].w = ptw_resp_flags_i[req_idx*16 + 5];
      new_ptw_resp_data[req_idx].r = ptw_resp_flags_i[req_idx*16 + 6];
      new_ptw_resp_data[req_idx].v = ptw_resp_flags_i[req_idx*16 + 7];
    end
    new_ptw_resp_data[0].ppn = ptw0_resp_ppn_i[CsrSatpPpnSz-1:0];
    new_ptw_resp_data[1].ppn = ptw1_resp_ppn_i[CsrSatpPpnSz-1:0];
    orig_ptw_resp_data[0] = new_ptw_resp_data[0];
    orig_ptw_resp_data[1] = new_ptw_resp_data[1];

    new_l2_miss_resp = '0;
    new_l2_miss_resp.opcode = etlink_pkg::rsp_opcode_e'(l2_miss_resp_opcode_i);
    new_l2_miss_resp.data = {l2_resp_qw7_i, l2_resp_qw6_i, l2_resp_qw5_i, l2_resp_qw4_i,
                             l2_resp_qw3_i, l2_resp_qw2_i, l2_resp_qw1_i, l2_resp_qw0_i};
    new_l2_miss_resp.size = etlink_pkg::SizeLine;
    orig_l2_miss_resp = '0;
    orig_l2_miss_resp.opcode = et_link_rsp_opcode_t'(l2_miss_resp_opcode_i);
    orig_l2_miss_resp.data = new_l2_miss_resp.data;
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

    new_resp_valid_o = new_resp_valid;
    orig_resp_valid_o = orig_resp_valid;
    new_resp_miss_o = new_resp_miss;
    orig_resp_miss_o = orig_resp_miss;
    new_resp0_data_0_o = new_resp_data[0].data[63:0];
    orig_resp0_data_0_o = orig_resp_data[0].data[63:0];
    new_resp0_data_1_o = new_resp_data[0].data[127:64];
    orig_resp0_data_1_o = orig_resp_data[0].data[127:64];
    new_resp0_data_2_o = new_resp_data[0].data[191:128];
    orig_resp0_data_2_o = orig_resp_data[0].data[191:128];
    new_resp0_data_3_o = new_resp_data[0].data[255:192];
    orig_resp0_data_3_o = orig_resp_data[0].data[255:192];
    new_resp0_flags_o = {new_resp_data[0].page_fault, new_resp_data[0].access_fault,
                         new_resp_data[0].cacheable, new_resp_data[0].bus_err,
                         new_resp_data[0].ecc_err};
    orig_resp0_flags_o = {orig_resp_data[0].page_fault, orig_resp_data[0].access_fault,
                          orig_resp_data[0].cacheable, orig_resp_data[0].bus_err,
                          orig_resp_data[0].ecc_err};
    new_resp1_data_0_o = new_resp_data[1].data[63:0];
    orig_resp1_data_0_o = orig_resp_data[1].data[63:0];
    new_resp1_data_1_o = new_resp_data[1].data[127:64];
    orig_resp1_data_1_o = orig_resp_data[1].data[127:64];
    new_resp1_data_2_o = new_resp_data[1].data[191:128];
    orig_resp1_data_2_o = orig_resp_data[1].data[191:128];
    new_resp1_data_3_o = new_resp_data[1].data[255:192];
    orig_resp1_data_3_o = orig_resp_data[1].data[255:192];
    new_resp1_flags_o = {new_resp_data[1].page_fault, new_resp_data[1].access_fault,
                         new_resp_data[1].cacheable, new_resp_data[1].bus_err,
                         new_resp_data[1].ecc_err};
    orig_resp1_flags_o = {orig_resp_data[1].page_fault, orig_resp_data[1].access_fault,
                          orig_resp_data[1].cacheable, orig_resp_data[1].bus_err,
                          orig_resp_data[1].ecc_err};

    new_l2_miss_req_id_o = new_l2_miss_req.id;
    orig_l2_miss_req_id_o = orig_l2_miss_req.id;
    new_l2_miss_req_source_o = new_l2_miss_req.source;
    orig_l2_miss_req_source_o = orig_l2_miss_req.source;
    new_l2_miss_req_wdata_o = new_l2_miss_req.wdata;
    orig_l2_miss_req_wdata_o = orig_l2_miss_req.wdata;
    new_l2_miss_req_opcode_o = new_l2_miss_req.opcode;
    orig_l2_miss_req_opcode_o = orig_l2_miss_req.opcode;
    new_l2_miss_req_addr_o = new_l2_miss_req.address;
    orig_l2_miss_req_addr_o = orig_l2_miss_req.address;
    new_l2_miss_req_size_o = new_l2_miss_req.size;
    orig_l2_miss_req_size_o = orig_l2_miss_req.size;
    new_l2_miss_req_qwen_o = new_l2_miss_req.qwen;
    orig_l2_miss_req_qwen_o = orig_l2_miss_req.qwen;
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
    new_ptw1_req_satp_mode_o = new_ptw_req_data[1].satp_mode;
    orig_ptw1_req_satp_mode_o = orig_ptw_req_data[1].satp_mode;
    new_ptw1_req_satp_ppn_o = new_ptw_req_data[1].satp_ppn;
    orig_ptw1_req_satp_ppn_o = orig_ptw_req_data[1].satp_ppn;
    new_ptw1_req_prv_o = new_ptw_req_data[1].prv;
    orig_ptw1_req_prv_o = orig_ptw_req_data[1].prv;
    new_ptw1_req_addr_o = new_ptw_req_data[1].addr;
    orig_ptw1_req_addr_o = orig_ptw_req_data[1].addr;

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

  neigh_shared_icache #(
    .NrMinions    (NrMinions),
    .NrRequestors (NrRequestors)
  ) u_new (
    .clk_i,
    .rst_ni,
    .esr_prefetch_conf_i             (new_prefetch_conf),
    .esr_prefetch_start_i            (prefetch_start_i),
    .esr_prefetch_done_o             (new_prefetch_done_o),
    .esr_err_log_ctl_i               (new_err_log_ctl),
    .esr_err_log_sbe_o               (new_err_log_sbe_o),
    .esr_err_log_dbe_o               (new_err_log_dbe_o),
    .esr_err_log_info_o              (new_err_log_info),
    .esr_mprot_i                     (new_mprot),
    .esr_vmspagesize_i               (new_vmspagesize),
    .esr_bypass_icache_i             (bypass_icache_i),
    .esr_shire_coop_mode_i           (coop_mode_i),
    .f0_req_ready_o                  (new_req_ready_o),
    .f0_req_valid_i                  (req_valid_i),
    .f0_req_i                        (new_req_data),
    .f4_resp_valid_o                 (new_resp_valid),
    .f4_resp_miss_o                  (new_resp_miss),
    .f4_resp_o                       (new_resp_data),
    .f5_resp_fill_done_o             (new_fill_done_o),
    .f0_flush_data_i                 (flush_data_i),
    .f0_l2_miss_req_disable_next_i   (l2_miss_req_disable_next_i),
    .f0_l2_miss_req_ready_i          (l2_miss_req_ready_i),
    .f0_l2_miss_req_valid_o          (new_l2_miss_req_valid_o),
    .f0_l2_miss_req_o                (new_l2_miss_req),
    .f0_l2_miss_resp_ready_o         (new_l2_miss_resp_ready_o),
    .f0_l2_miss_resp_valid_i         (l2_miss_resp_valid_i),
    .f0_l2_miss_resp_i               (new_l2_miss_resp),
    .satp_info_i                     (new_satp_info),
    .matp_info_i                     (new_matp_info),
    .tlb_invalidate_i                (tlb_invalidate_i),
    .ptw_req_data_o                  (new_ptw_req_data),
    .ptw_req_valid_o                 ({new_ptw1_req_valid_o, new_ptw0_req_valid_o}),
    .ptw_req_ready_i                 (ptw_req_ready_i),
    .ptw_invalidate_o                ({new_ptw1_invalidate_o, new_ptw0_invalidate_o}),
    .ptw_resp_valid_i                (ptw_resp_valid_i),
    .ptw_resp_data_i                 (new_ptw_resp_data),
    .icache_f2_sram_req_write_o      (new_sram_req_write_o),
    .icache_f2_sram_req_addr_o       (new_sram_req_addr_o),
    .icache_f2_sram_req_valid_o      (new_sram_req_valid_o),
    .icache_f2_sram_req_ready_i      (sram_req_ready_i),
    .icache_f0_sram_resp_dout_i      (orig_sram_resp_dout_flat),
    .icache_f0_sram_resp_valid_i     (sram_resp_valid_i),
    .icache_f0_sram_resp_ready_o     (new_sram_resp_ready_o),
    .apb_paddr_i,
    .apb_pwrite_i,
    .apb_psel_i,
    .apb_penable_i,
    .apb_pwdata_i,
    .apb_pready_o                    (new_apb_pready_o),
    .apb_prdata_o                    (new_apb_prdata_o),
    .apb_pslverr_o                   (new_apb_pslverr_o),
    .dbg_sm_signals_o                (new_dbg_sm_signals)
  );

  neigh_shared_icache_orig #(
    .NR_MINIONS    (NrMinions),
    .NR_REQUESTORS (NrRequestors)
  ) u_orig (
    .clock                       (clk_i),
    .reset                       (reset_orig),
    .esr_prefetch_conf           (orig_prefetch_conf),
    .esr_prefetch_start          (prefetch_start_i),
    .esr_prefetch_done           (orig_prefetch_done_o),
    .esr_err_log_ctl             (orig_err_log_ctl),
    .esr_err_log_sbe             (orig_err_log_sbe_o),
    .esr_err_log_dbe             (orig_err_log_dbe_o),
    .esr_err_log_info            (orig_err_log_info),
    .esr_mprot                   (orig_mprot),
    .esr_vmspagesize             (orig_vmspagesize),
    .esr_bypass_icache           (bypass_icache_i),
    .esr_shire_coop_mode         (coop_mode_i),
    .f0_req_ready                (orig_req_ready_o),
    .f0_req_valid                (req_valid_i),
    .f0_req                      (orig_req_data),
    .f4_resp_valid               (orig_resp_valid),
    .f4_resp_miss                (orig_resp_miss),
    .f4_resp                     (orig_resp_data),
    .f5_resp_fill_done           (orig_fill_done_o),
    .f0_flush_data               (flush_data_i),
    .f0_l2_miss_req_disable_next (l2_miss_req_disable_next_i),
    .f0_l2_miss_req_ready        (l2_miss_req_ready_i),
    .f0_l2_miss_req_valid        (orig_l2_miss_req_valid_o),
    .f0_l2_miss_req              (orig_l2_miss_req),
    .f0_l2_miss_resp_ready       (orig_l2_miss_resp_ready_o),
    .f0_l2_miss_resp_valid       (l2_miss_resp_valid_i),
    .f0_l2_miss_resp             (orig_l2_miss_resp),
    .satp_info                   (orig_satp_info_flat),
    .matp_info                   (orig_matp_info_flat),
    .tlb_invalidate              (tlb_invalidate_i),
    .ptw_req_data                (orig_ptw_req_data),
    .ptw_req_valid               ({orig_ptw1_req_valid_o, orig_ptw0_req_valid_o}),
    .ptw_req_ready               (ptw_req_ready_i),
    .ptw_invalidate              ({orig_ptw1_invalidate_o, orig_ptw0_invalidate_o}),
    .ptw_resp_valid              (ptw_resp_valid_i),
    .ptw_resp_data               (orig_ptw_resp_data),
    .icache_f2_sram_req_write    (orig_sram_req_write_o),
    .icache_f2_sram_req_addr     (orig_sram_req_addr_o),
    .icache_f2_sram_req_valid    (orig_sram_req_valid_o),
    .icache_f2_sram_req_ready    (sram_req_ready_i),
    .icache_f0_sram_resp_dout    (orig_sram_resp_dout_flat),
    .icache_f0_sram_resp_valid   (sram_resp_valid_i),
    .icache_f0_sram_resp_ready   (orig_sram_resp_ready_o),
    .apb_paddr                   (apb_paddr_i),
    .apb_pwrite                  (apb_pwrite_i),
    .apb_psel                    (apb_psel_i),
    .apb_penable                 (apb_penable_i),
    .apb_pwdata                  (apb_pwdata_i),
    .apb_pready                  (orig_apb_pready_o),
    .apb_prdata                  (orig_apb_prdata_o),
    .apb_pslverr                 (orig_apb_pslverr_o),
    .dbg_sm_signals              (orig_dbg_sm_signals)
  );

endmodule : cosim_neigh_shared_icache_tb
/* verilator lint_on SYNCASYNCNET */

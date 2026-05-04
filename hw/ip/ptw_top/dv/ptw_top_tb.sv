// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module ptw_top_tb
  import etlink_pkg::*;
  import minion_pkg::*;
  import minion_dcache_pkg::*;
  import neigh_pkg::*;
(
  input  logic                         clk_i,
  input  logic                         rst_ni,
  input  logic                         ioshire_i,
  input  logic [CsrSatpModeSz-1:0]     req_satp_mode_i,
  input  logic [PaTransSize-1:0]       req_satp_ppn_i,
  input  logic [1:0]                   req_prv_i,
  input  logic [VaSize-1:VaUntransSize] req_addr_i,
  input  logic                         req_valid_i,
  output logic                         req_ready_o,
  input  logic                         invalidate_i,
  input  logic                         mprot_enable_secure_memory_i,
  input  logic [1:0]                   mprot_dram_size_i,
  input  logic                         mprot_disable_osbox_access_i,
  input  logic                         mprot_disable_pcie_access_i,
  input  logic [1:0]                   mprot_io_access_mode_i,
  input  logic [$bits(tlb_entry_type)-1:0] vmspagesize_i,
  output logic [63:0]                  resp_data_o,
  output logic                         resp_valid_o,
  input  logic                         mem_miss_req_disable_next_i,
  output logic [IdSize-1:0]            mem_miss_req_id_o,
  output logic [SourceSize-1:0]        mem_miss_req_source_o,
  output logic                         mem_miss_req_wdata_o,
  output logic [$bits(req_opcode_e)-1:0] mem_miss_req_opcode_o,
  output logic [PaSize-1:0]            mem_miss_req_address_o,
  output logic [63:0]                  mem_miss_req_data_0_o,
  output logic [63:0]                  mem_miss_req_data_1_o,
  output logic [63:0]                  mem_miss_req_data_2_o,
  output logic [63:0]                  mem_miss_req_data_3_o,
  output logic [$bits(size_e)-1:0]     mem_miss_req_size_o,
  output logic [QwenSize-1:0]          mem_miss_req_qwen_o,
  output logic [SubopcodeSize-1:0]     mem_miss_req_subopcode_o,
  output logic                         mem_miss_req_valid_o,
  input  logic                         mem_miss_req_ready_i,
  input  logic [IdSize-1:0]            mem_fill_req_id_i,
  input  logic [SourceSize-1:0]        mem_fill_req_dest_i,
  input  logic                         mem_fill_req_wdata_i,
  input  logic [$bits(rsp_opcode_e)-1:0] mem_fill_req_opcode_i,
  input  logic [63:0]                  mem_fill_req_data_0_i,
  input  logic [63:0]                  mem_fill_req_data_1_i,
  input  logic [63:0]                  mem_fill_req_data_2_i,
  input  logic [63:0]                  mem_fill_req_data_3_i,
  input  logic [63:0]                  mem_fill_req_data_4_i,
  input  logic [63:0]                  mem_fill_req_data_5_i,
  input  logic [63:0]                  mem_fill_req_data_6_i,
  input  logic [63:0]                  mem_fill_req_data_7_i,
  input  logic [$bits(size_e)-1:0]     mem_fill_req_size_i,
  input  logic [QwenSize-1:0]          mem_fill_req_qwen_i,
  input  logic                         mem_fill_req_valid_i,
  output logic                         mem_fill_req_ready_o,
  output logic [$bits(ptw_dbg_sm_t)-1:0] dbg_sm_signals_o
);

  minion_ptw_req         req_data;
  minion_ptw_pte         resp_data;
  esr_mprot_t            mprot;
  tlb_entry_type         vmspagesize;
  neigh_req_t            mem_miss_req_info;
  rsp_t                  mem_fill_req_info;
  ptw_dbg_sm_t           dbg_sm_signals;

  always_comb begin
    req_data = '0;
    req_data.satp_mode = req_satp_mode_i;
    req_data.satp_ppn  = req_satp_ppn_i;
    req_data.prv       = req_prv_i;
    req_data.addr      = req_addr_i;

    mprot = '0;
    mprot.enable_secure_memory = mprot_enable_secure_memory_i;
    mprot.dram_size            = mprot_dram_size_i;
    mprot.disable_osbox_access = mprot_disable_osbox_access_i;
    mprot.disable_pcie_access  = mprot_disable_pcie_access_i;
    mprot.io_access_mode       = mprot_io_access_mode_i;

    vmspagesize = tlb_entry_type'(vmspagesize_i);

    mem_fill_req_info = '0;
    mem_fill_req_info.id     = mem_fill_req_id_i;
    mem_fill_req_info.dest   = mem_fill_req_dest_i;
    mem_fill_req_info.wdata  = mem_fill_req_wdata_i;
    mem_fill_req_info.opcode = rsp_opcode_e'(mem_fill_req_opcode_i);
    mem_fill_req_info.data   = {
      mem_fill_req_data_7_i,
      mem_fill_req_data_6_i,
      mem_fill_req_data_5_i,
      mem_fill_req_data_4_i,
      mem_fill_req_data_3_i,
      mem_fill_req_data_2_i,
      mem_fill_req_data_1_i,
      mem_fill_req_data_0_i
    };
    mem_fill_req_info.size   = size_e'(mem_fill_req_size_i);
    mem_fill_req_info.qwen   = mem_fill_req_qwen_i;

    resp_data_o              = resp_data;
    dbg_sm_signals_o         = dbg_sm_signals;
    mem_miss_req_id_o        = mem_miss_req_info.id;
    mem_miss_req_source_o    = mem_miss_req_info.source;
    mem_miss_req_wdata_o     = mem_miss_req_info.wdata;
    mem_miss_req_opcode_o    = mem_miss_req_info.opcode;
    mem_miss_req_address_o   = mem_miss_req_info.address;
    mem_miss_req_data_0_o    = mem_miss_req_info.data[63:0];
    mem_miss_req_data_1_o    = mem_miss_req_info.data[127:64];
    mem_miss_req_data_2_o    = mem_miss_req_info.data[191:128];
    mem_miss_req_data_3_o    = mem_miss_req_info.data[255:192];
    mem_miss_req_size_o      = mem_miss_req_info.size;
    mem_miss_req_qwen_o      = mem_miss_req_info.qwen;
    mem_miss_req_subopcode_o = mem_miss_req_info.subopcode;
  end

  ptw_top u_dut (
    .clk_i,
    .rst_ni,
    .ioshire_i,
    .req_data_i(req_data),
    .req_valid_i,
    .req_ready_o,
    .invalidate_i,
    .mprot_i(mprot),
    .vmspagesize_i(vmspagesize),
    .resp_data_o(resp_data),
    .resp_valid_o,
    .mem_miss_req_disable_next_i,
    .mem_miss_req_info_o(mem_miss_req_info),
    .mem_miss_req_valid_o,
    .mem_miss_req_ready_i,
    .mem_fill_req_info_i(mem_fill_req_info),
    .mem_fill_req_valid_i,
    .mem_fill_req_ready_o,
    .dbg_sm_signals_o(dbg_sm_signals)
  );

endmodule

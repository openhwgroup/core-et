// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module cosim_neigh_shared_ptw_tb
  import etlink_pkg::*;
  import minion_pkg::*;
  import minion_dcache_pkg::*;
(
  input  logic                         clk_i,
  input  logic                         rst_ni,
  input  logic                         ioshire_i,
  input  logic [CsrSatpModeSz-1:0]     req_satp_mode_i [5],
  input  logic [PaTransSize-1:0]       req_satp_ppn_i [5],
  input  logic [1:0]                   req_prv_i [5],
  input  logic [VaSize-1:VaUntransSize] req_addr_i [5],
  input  logic [4:0]                   req_valid_i,
  input  logic [4:0]                   invalidate_i,
  input  logic                         mprot_enable_secure_memory_i,
  input  logic [1:0]                   mprot_dram_size_i,
  input  logic                         mprot_disable_osbox_access_i,
  input  logic                         mprot_disable_pcie_access_i,
  input  logic [1:0]                   mprot_io_access_mode_i,
  input  logic [$bits(tlb_entry_type)-1:0] vmspagesize_i,
  input  logic                         mem_miss_req_disable_next_i,
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

  output logic [4:0]                   orig_req_ready_o,
  output logic [63:0]                  orig_resp_data_o,
  output logic [4:0]                   orig_resp_valid_o,
  output logic [IdSize-1:0]            orig_mem_miss_req_id_o,
  output logic [SourceSize-1:0]        orig_mem_miss_req_source_o,
  output logic                         orig_mem_miss_req_wdata_o,
  output logic [4:0]                   orig_mem_miss_req_opcode_o,
  output logic [PaSize-1:0]            orig_mem_miss_req_address_o,
  output logic [63:0]                  orig_mem_miss_req_data_0_o,
  output logic [63:0]                  orig_mem_miss_req_data_1_o,
  output logic [63:0]                  orig_mem_miss_req_data_2_o,
  output logic [63:0]                  orig_mem_miss_req_data_3_o,
  output logic [2:0]                   orig_mem_miss_req_size_o,
  output logic [QwenSize-1:0]          orig_mem_miss_req_qwen_o,
  output logic [SubopcodeSize-1:0]     orig_mem_miss_req_subopcode_o,
  output logic                         orig_mem_miss_req_valid_o,
  output logic                         orig_mem_fill_req_ready_o,
  output logic [$bits(neigh_pkg::ptw_dbg_sm_t)-1:0] orig_dbg_sm_signals_o,

  output logic [4:0]                   new_req_ready_o,
  output logic [63:0]                  new_resp_data_o,
  output logic [4:0]                   new_resp_valid_o,
  output logic [IdSize-1:0]            new_mem_miss_req_id_o,
  output logic [SourceSize-1:0]        new_mem_miss_req_source_o,
  output logic                         new_mem_miss_req_wdata_o,
  output logic [4:0]                   new_mem_miss_req_opcode_o,
  output logic [PaSize-1:0]            new_mem_miss_req_address_o,
  output logic [63:0]                  new_mem_miss_req_data_0_o,
  output logic [63:0]                  new_mem_miss_req_data_1_o,
  output logic [63:0]                  new_mem_miss_req_data_2_o,
  output logic [63:0]                  new_mem_miss_req_data_3_o,
  output logic [2:0]                   new_mem_miss_req_size_o,
  output logic [QwenSize-1:0]          new_mem_miss_req_qwen_o,
  output logic [SubopcodeSize-1:0]     new_mem_miss_req_subopcode_o,
  output logic                         new_mem_miss_req_valid_o,
  output logic                         new_mem_fill_req_ready_o,
  output logic [$bits(neigh_pkg::ptw_dbg_sm_t)-1:0] new_dbg_sm_signals_o
);

  logic                             reset;
  minion_ptw_req                    new_req_data [5];
  esr_mprot_t                       new_mprot;
  tlb_entry_type                    new_vmspagesize;
  neigh_req_t                       new_mem_miss_req_info;
  rsp_t                             new_mem_fill_req_info;
  minion_ptw_pte                    new_resp_data;
  neigh_pkg::ptw_dbg_sm_t           new_dbg_sm_signals;

  minion_ptw_req [4:0]              orig_req_data;
  esr_mprot_t                       orig_mprot;
  tlb_entry_type                    orig_vmspagesize;
  et_link_neigh_req_info_t          orig_mem_miss_req_info;
  et_link_rsp_info_t                orig_mem_fill_req_info;
  minion_ptw_pte                    orig_resp_data;
  ptw_dbg_sm_t                      orig_dbg_sm_signals;

  assign reset = !rst_ni;

  always_comb begin
    for (int i = 0; i < 5; i++) begin
      new_req_data[i] = '0;
      new_req_data[i].satp_mode = req_satp_mode_i[i];
      new_req_data[i].satp_ppn  = req_satp_ppn_i[i];
      new_req_data[i].prv       = req_prv_i[i];
      new_req_data[i].addr      = req_addr_i[i];

      orig_req_data[i] = '0;
      orig_req_data[i].satp_mode = req_satp_mode_i[i];
      orig_req_data[i].satp_ppn  = req_satp_ppn_i[i];
      orig_req_data[i].prv       = req_prv_i[i];
      orig_req_data[i].addr      = req_addr_i[i];
    end

    new_mprot = '0;
    new_mprot.enable_secure_memory = mprot_enable_secure_memory_i;
    new_mprot.dram_size            = mprot_dram_size_i;
    new_mprot.disable_osbox_access = mprot_disable_osbox_access_i;
    new_mprot.disable_pcie_access  = mprot_disable_pcie_access_i;
    new_mprot.io_access_mode       = mprot_io_access_mode_i;
    new_vmspagesize = tlb_entry_type'(vmspagesize_i);

    orig_mprot = '0;
    orig_mprot.enable_secure_memory = mprot_enable_secure_memory_i;
    orig_mprot.dram_size            = mprot_dram_size_i;
    orig_mprot.disable_osbox_access = mprot_disable_osbox_access_i;
    orig_mprot.disable_pcie_access  = mprot_disable_pcie_access_i;
    orig_mprot.io_access_mode       = mprot_io_access_mode_i;
    orig_vmspagesize = tlb_entry_type'(vmspagesize_i);

    new_mem_fill_req_info = '0;
    new_mem_fill_req_info.id     = mem_fill_req_id_i;
    new_mem_fill_req_info.dest   = mem_fill_req_dest_i;
    new_mem_fill_req_info.wdata  = mem_fill_req_wdata_i;
    new_mem_fill_req_info.opcode = rsp_opcode_e'(mem_fill_req_opcode_i);
    new_mem_fill_req_info.data   = {
      mem_fill_req_data_7_i,
      mem_fill_req_data_6_i,
      mem_fill_req_data_5_i,
      mem_fill_req_data_4_i,
      mem_fill_req_data_3_i,
      mem_fill_req_data_2_i,
      mem_fill_req_data_1_i,
      mem_fill_req_data_0_i
    };
    new_mem_fill_req_info.size = size_e'(mem_fill_req_size_i);
    new_mem_fill_req_info.qwen = mem_fill_req_qwen_i;

    orig_mem_fill_req_info = '0;
    orig_mem_fill_req_info.id     = mem_fill_req_id_i;
    orig_mem_fill_req_info.dest   = mem_fill_req_dest_i;
    orig_mem_fill_req_info.wdata  = mem_fill_req_wdata_i;
    orig_mem_fill_req_info.opcode = et_link_rsp_opcode_t'(mem_fill_req_opcode_i);
    orig_mem_fill_req_info.data   = {
      mem_fill_req_data_7_i,
      mem_fill_req_data_6_i,
      mem_fill_req_data_5_i,
      mem_fill_req_data_4_i,
      mem_fill_req_data_3_i,
      mem_fill_req_data_2_i,
      mem_fill_req_data_1_i,
      mem_fill_req_data_0_i
    };
    orig_mem_fill_req_info.size = et_link_size_t'(mem_fill_req_size_i);
    orig_mem_fill_req_info.qwen = mem_fill_req_qwen_i;

    orig_resp_data_o              = orig_resp_data;
    orig_dbg_sm_signals_o         = orig_dbg_sm_signals;
    orig_mem_miss_req_id_o        = orig_mem_miss_req_info.id;
    orig_mem_miss_req_source_o    = orig_mem_miss_req_info.source;
    orig_mem_miss_req_wdata_o     = orig_mem_miss_req_info.wdata;
    orig_mem_miss_req_opcode_o    = orig_mem_miss_req_info.opcode;
    orig_mem_miss_req_address_o   = orig_mem_miss_req_info.address;
    orig_mem_miss_req_data_0_o    = orig_mem_miss_req_info.data[63:0];
    orig_mem_miss_req_data_1_o    = orig_mem_miss_req_info.data[127:64];
    orig_mem_miss_req_data_2_o    = orig_mem_miss_req_info.data[191:128];
    orig_mem_miss_req_data_3_o    = orig_mem_miss_req_info.data[255:192];
    orig_mem_miss_req_size_o      = orig_mem_miss_req_info.size;
    orig_mem_miss_req_qwen_o      = orig_mem_miss_req_info.qwen;
    orig_mem_miss_req_subopcode_o = orig_mem_miss_req_info.subopcode;

    new_resp_data_o               = new_resp_data;
    new_dbg_sm_signals_o          = new_dbg_sm_signals;
    new_mem_miss_req_id_o         = new_mem_miss_req_info.id;
    new_mem_miss_req_source_o     = new_mem_miss_req_info.source;
    new_mem_miss_req_wdata_o      = new_mem_miss_req_info.wdata;
    new_mem_miss_req_opcode_o     = new_mem_miss_req_info.opcode;
    new_mem_miss_req_address_o    = new_mem_miss_req_info.address;
    new_mem_miss_req_data_0_o     = new_mem_miss_req_info.data[63:0];
    new_mem_miss_req_data_1_o     = new_mem_miss_req_info.data[127:64];
    new_mem_miss_req_data_2_o     = new_mem_miss_req_info.data[191:128];
    new_mem_miss_req_data_3_o     = new_mem_miss_req_info.data[255:192];
    new_mem_miss_req_size_o       = new_mem_miss_req_info.size;
    new_mem_miss_req_qwen_o       = new_mem_miss_req_info.qwen;
    new_mem_miss_req_subopcode_o  = new_mem_miss_req_info.subopcode;
  end

  neigh_shared_ptw_orig u_orig (
    .clock                     (clk_i),
    .reset                     (reset),
    .ioshire                   (ioshire_i),
    .req_data                  (orig_req_data),
    .req_valid                 (req_valid_i),
    .req_ready                 (orig_req_ready_o),
    .invalidate                (invalidate_i),
    .mprot                     (orig_mprot),
    .vmspagesize               (orig_vmspagesize),
    .resp_data                 (orig_resp_data),
    .resp_valid                (orig_resp_valid_o),
    .mem_miss_req_disable_next (mem_miss_req_disable_next_i),
    .mem_miss_req_info         (orig_mem_miss_req_info),
    .mem_miss_req_valid        (orig_mem_miss_req_valid_o),
    .mem_miss_req_ready        (mem_miss_req_ready_i),
    .mem_fill_req_info         (orig_mem_fill_req_info),
    .mem_fill_req_valid        (mem_fill_req_valid_i),
    .mem_fill_req_ready        (orig_mem_fill_req_ready_o),
    .dbg_sm_signals            (orig_dbg_sm_signals)
  );

  neigh_shared_ptw u_new (
    .clk_i,
    .rst_ni,
    .ioshire_i,
    .req_data_i                 (new_req_data),
    .req_valid_i,
    .req_ready_o                (new_req_ready_o),
    .invalidate_i,
    .mprot_i                    (new_mprot),
    .vmspagesize_i              (new_vmspagesize),
    .resp_data_o                (new_resp_data),
    .resp_valid_o               (new_resp_valid_o),
    .mem_miss_req_disable_next_i,
    .mem_miss_req_info_o        (new_mem_miss_req_info),
    .mem_miss_req_valid_o       (new_mem_miss_req_valid_o),
    .mem_miss_req_ready_i,
    .mem_fill_req_info_i        (new_mem_fill_req_info),
    .mem_fill_req_valid_i,
    .mem_fill_req_ready_o       (new_mem_fill_req_ready_o),
    .dbg_sm_signals_o           (new_dbg_sm_signals)
  );

endmodule

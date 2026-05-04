// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation: original shire_cache_pipe_idx_cop_sm vs new shirecache_pipe_idx_cop_sm.
//
// Both modules receive identical stimulus. Outputs are compared cycle-by-cycle.
// The original uses active-high synchronous reset; the new uses active-low async.

`include "soc.vh"

module cosim_idx_cop_sm_tb
  import shirecache_pkg::*;
  import dft_pkg::*;
(
  input  logic                        clk_i,
  input  logic                        rst_ni,

  // MBIST
  input  logic                        mbist_on_i,

  // ESR write interface
  input  logic                        wr_en_idx_cop_sm_ctl_i,
  input  logic                        wr_en_idx_cop_sm_data0_i,
  input  logic                        wr_en_idx_cop_sm_data1_i,
  input  logic                        wr_en_idx_cop_sm_ecc_i,
  input  logic                        wr_en_idx_cop_sm_physical_index_i,
  input  logic [DwDataSize-1:0]       wr_data_i,

  // ESR config
  input  logic [SetIdSize-1:0]        esr_sc_l2_set_base_i,
  input  logic [SetAllocSize-1:0]     esr_sc_l2_set_size_i,
  input  logic [SetIdSize-1:0]        esr_sc_l3_set_base_i,
  input  logic [SetAllocSize-1:0]     esr_sc_l3_set_size_i,
  input  logic [SetIdSize-1:0]        esr_sc_scp_set_base_i,
  input  logic [SetAllocSize-1:0]     esr_sc_scp_set_size_i,

  // Reqq handshake
  input  logic                        pipe_idx_cop_req_ready_i,

  // Cbuf state
  input  logic [CbufSize-1:0]         cbuf_mem_valid_i,

  // IDs
  input  logic [BankIdSize-1:0]       my_bank_id_i,
  input  logic [ScpShireIdSize-1:0]   my_scp_virtual_shire_id_i,

  // New module outputs
  output logic                        new_aborted_o,
  output logic                        new_l2_busy_o,
  output logic                        new_l3_busy_o,
  output logic                        new_req_valid_o,
  output logic [5:0]                  new_req_opcode_o,
  output logic [PaSize-1:0]           new_req_address_o,
  output logic                        new_req_sync_flush_o,
  output logic                        new_is_active_o,
  output logic [EsrStatusSize-1:0]    new_esr_ctl_o,
  output logic [EsrStatusSize-1:0]    new_esr_pidx_o,
  output logic [EsrStatusSize-1:0]    new_esr_data0_o,
  output logic [EsrStatusSize-1:0]    new_esr_data1_o,
  output logic [EsrStatusSize-1:0]    new_esr_ecc_o,
  output logic                        new_dbg_wr_tag_state_en_o,
  output logic                        new_dbg_wr_tags_en_o,
  output logic                        new_dbg_wr_data_en_o,
  output logic                        new_dbg_wr_ecc_wr_en_o,
  output logic [LineQwSize-1:0]       new_dbg_wr_data_qwen_o,

  // Original module outputs
  output logic                        orig_aborted_o,
  output logic                        orig_l2_busy_o,
  output logic                        orig_l3_busy_o,
  output logic                        orig_req_valid_o,
  output logic [5:0]                  orig_req_opcode_o,
  output logic [PaSize-1:0]           orig_req_address_o,
  output logic                        orig_req_sync_flush_o,
  output logic                        orig_is_active_o,
  output logic [EsrStatusSize-1:0]    orig_esr_ctl_o,
  output logic [EsrStatusSize-1:0]    orig_esr_pidx_o,
  output logic [EsrStatusSize-1:0]    orig_esr_data0_o,
  output logic [EsrStatusSize-1:0]    orig_esr_data1_o,
  output logic [EsrStatusSize-1:0]    orig_esr_ecc_o,
  output logic                        orig_dbg_wr_tag_state_en_o,
  output logic                        orig_dbg_wr_tags_en_o,
  output logic                        orig_dbg_wr_data_en_o,
  output logic                        orig_dbg_wr_ecc_wr_en_o,
  output logic [LineQwSize-1:0]       orig_dbg_wr_data_qwen_o
);

  // Active-high synchronous reset for original
  wire reset = !rst_ni;

  // ── Shared cbuf state with deterministic addresses ────────
  // cbuf_mem[i] = {i[7:0], bank_id, 6'b0} — line-aligned, unique per entry
  cbuf_state_t shared_cbuf_state;
  always_comb begin
    shared_cbuf_state.cbuf_mem_valid = cbuf_mem_valid_i;
    for (int i = 0; i < CbufSize; i++) begin
      shared_cbuf_state.cbuf_mem[i] = PaSize'({8'(i), my_bank_id_i, {BankLsb{1'b0}}});
    end
  end
  sc_pipe_cbuf_state_t orig_cbuf_state;
  always_comb begin
    orig_cbuf_state.cbuf_mem_valid = cbuf_mem_valid_i;
    for (int i = 0; i < `SC_CBUF_SIZE; i++) begin
      orig_cbuf_state.cbuf_mem[i] = `ZX(`PA_SIZE, {8'(i), my_bank_id_i, {`SC_BANK_LSB{1'b0}}});
    end
  end

  // ── New module ────────────────────────────────────────────
  bank_esr_ctl_t new_esr_ctl;
  always_comb begin
    new_esr_ctl = '0;
    new_esr_ctl.wr_en_idx_cop_sm_ctl            = wr_en_idx_cop_sm_ctl_i;
    new_esr_ctl.wr_en_idx_cop_sm_data0          = wr_en_idx_cop_sm_data0_i;
    new_esr_ctl.wr_en_idx_cop_sm_data1          = wr_en_idx_cop_sm_data1_i;
    new_esr_ctl.wr_en_idx_cop_sm_ecc            = wr_en_idx_cop_sm_ecc_i;
    new_esr_ctl.wr_en_idx_cop_sm_physical_index = wr_en_idx_cop_sm_physical_index_i;
    new_esr_ctl.wr_data                         = wr_data_i;
    new_esr_ctl.esr_sc_l2_set_base              = esr_sc_l2_set_base_i;
    new_esr_ctl.esr_sc_l2_set_size              = esr_sc_l2_set_size_i;
    new_esr_ctl.esr_sc_l3_set_base              = esr_sc_l3_set_base_i;
    new_esr_ctl.esr_sc_l3_set_size              = esr_sc_l3_set_size_i;
    new_esr_ctl.esr_sc_scp_set_base             = esr_sc_scp_set_base_i;
    new_esr_ctl.esr_sc_scp_set_size             = esr_sc_scp_set_size_i;
  end

  dft_t new_dft;
  assign new_dft = '0;

  idx_cop_sm_dbg_read_t new_dbg_rd;
  assign new_dbg_rd = '0;

  idx_cop_req_t new_req_info;
  idx_cop_sm_esr_status_t new_esr_status;
  idx_cop_sm_dbg_write_t new_dbg_wr;

  shirecache_pipe_idx_cop_sm #(
    .SkipPowerUpAllInv (1'b0)
  ) u_new (
    .clk_i                       (clk_i),
    .rst_ni                      (rst_ni),
    .rst_c_ni                    (rst_ni),  // same as warm reset for cosim
    .dft_i                       (new_dft),
    .mbist_on_i                  (mbist_on_i),
    .pipe_idx_cop_aborted_o      (new_aborted_o),
    .pipe_idx_cop_l2_busy_o      (new_l2_busy_o),
    .pipe_idx_cop_l3_busy_o      (new_l3_busy_o),
    .pipe_idx_cop_req_ready_i    (pipe_idx_cop_req_ready_i),
    .pipe_idx_cop_req_valid_o    (new_req_valid_o),
    .pipe_idx_cop_req_info_o     (new_req_info),
    .pipe_idx_cop_is_active_o    (new_is_active_o),
    .pipe_cbuf_state_info_i      (shared_cbuf_state),
    .esr_dbg_wr_info_o           (new_dbg_wr),
    .dbg_rd_info_i               (new_dbg_rd),
    .esr_ctl_i                   (new_esr_ctl),
    .pipe_idx_cop_sm_esr_status_o(new_esr_status),
    .my_bank_id_i                (my_bank_id_i),
    .my_scp_virtual_shire_id_i   (my_scp_virtual_shire_id_i)
  );

  assign new_req_opcode_o     = new_req_info.opcode;
  assign new_req_address_o    = new_req_info.address;
  assign new_req_sync_flush_o = new_req_info.sync_flush_victims;
  assign new_esr_ctl_o        = new_esr_status.ctl;
  assign new_esr_pidx_o       = new_esr_status.physical_index;
  assign new_esr_data0_o      = new_esr_status.data0;
  assign new_esr_data1_o      = new_esr_status.data1;
  assign new_esr_ecc_o        = new_esr_status.ecc;
  assign new_dbg_wr_tag_state_en_o = new_dbg_wr.tag_state_en;
  assign new_dbg_wr_tags_en_o      = new_dbg_wr.tags_en;
  assign new_dbg_wr_data_en_o      = new_dbg_wr.data_en;
  assign new_dbg_wr_ecc_wr_en_o    = new_dbg_wr.ecc_wr_en;
  assign new_dbg_wr_data_qwen_o    = new_dbg_wr.data_qwen;

  // ── Original module ───────────────────────────────────────
  sc_bank_esr_ctl_t orig_esr_ctl;
  always_comb begin
    orig_esr_ctl = '0;
    orig_esr_ctl.wr_en_idx_cop_sm_ctl            = wr_en_idx_cop_sm_ctl_i;
    orig_esr_ctl.wr_en_idx_cop_sm_data0          = wr_en_idx_cop_sm_data0_i;
    orig_esr_ctl.wr_en_idx_cop_sm_data1          = wr_en_idx_cop_sm_data1_i;
    orig_esr_ctl.wr_en_idx_cop_sm_ecc            = wr_en_idx_cop_sm_ecc_i;
    orig_esr_ctl.wr_en_idx_cop_sm_physical_index = wr_en_idx_cop_sm_physical_index_i;
    orig_esr_ctl.wr_data                         = wr_data_i;
    orig_esr_ctl.esr_sc_l2_set_base              = esr_sc_l2_set_base_i;
    orig_esr_ctl.esr_sc_l2_set_size              = esr_sc_l2_set_size_i;
    orig_esr_ctl.esr_sc_l3_set_base              = esr_sc_l3_set_base_i;
    orig_esr_ctl.esr_sc_l3_set_size              = esr_sc_l3_set_size_i;
    orig_esr_ctl.esr_sc_scp_set_base             = esr_sc_scp_set_base_i;
    orig_esr_ctl.esr_sc_scp_set_size             = esr_sc_scp_set_size_i;
  end

  sc_pipe_idx_cop_req_t orig_req_info;
  sc_pipe_idx_cop_sm_esr_status_t orig_esr_status;
  sc_idx_cop_sm_dbg_write_t orig_dbg_wr;
  sc_idx_cop_sm_dbg_read_t orig_dbg_rd;
  assign orig_dbg_rd = '0;

  shire_cache_pipe_idx_cop_sm u_orig (
    .clock                        (clk_i),
    .reset_in                     (reset),
    .reset_c_in                   (reset),   // same as warm reset for cosim
    .mbist_on                     (mbist_on_i),
    .pipe_idx_cop_aborted         (orig_aborted_o),
    .pipe_idx_cop_l2_busy         (orig_l2_busy_o),
    .pipe_idx_cop_l3_busy         (orig_l3_busy_o),
    .pipe_idx_cop_req_ready       (pipe_idx_cop_req_ready_i),
    .pipe_idx_cop_req_valid       (orig_req_valid_o),
    .pipe_idx_cop_req_info        (orig_req_info),
    .pipe_idx_cop_is_active       (orig_is_active_o),
    .pipe_cbuf_state_info         (orig_cbuf_state),
    .esr_dbg_wr_info              (orig_dbg_wr),
    .dbg_rd_info                  (orig_dbg_rd),
    .dft__reset_byp_hv            (1'b0),
    .dft__reset_hv                (1'b0),
    .esr_ctl                      (orig_esr_ctl),
    .pipe_idx_cop_sm_esr_status   (orig_esr_status),
    .my_bank_id                   (my_bank_id_i),
    .my_scp_virtual_shire_id      (my_scp_virtual_shire_id_i)
  );

  assign orig_req_opcode_o     = orig_req_info.opcode;
  assign orig_req_address_o    = orig_req_info.address;
  assign orig_req_sync_flush_o = orig_req_info.sync_flush_victims;
  assign orig_esr_ctl_o        = orig_esr_status.ctl;
  assign orig_esr_pidx_o       = orig_esr_status.physical_index;
  assign orig_esr_data0_o      = orig_esr_status.data0;
  assign orig_esr_data1_o      = orig_esr_status.data1;
  assign orig_esr_ecc_o        = orig_esr_status.ecc;
  assign orig_dbg_wr_tag_state_en_o = orig_dbg_wr.tag_state_en;
  assign orig_dbg_wr_tags_en_o      = orig_dbg_wr.tags_en;
  assign orig_dbg_wr_data_en_o      = orig_dbg_wr.data_en;
  assign orig_dbg_wr_ecc_wr_en_o    = orig_dbg_wr.ecc_wr_en;
  assign orig_dbg_wr_data_qwen_o    = orig_dbg_wr.data_qwen;

endmodule : cosim_idx_cop_sm_tb

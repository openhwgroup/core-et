// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Testbench wrapper for shirecache_pipe_idx_cop_sm.
// Breaks packed struct ports into scalars for easy C++ driving.

// TB exposes only selected struct fields; unused fields expected.
/* verilator lint_off UNUSEDSIGNAL */  // TB scalar ports drive struct subfields; remaining struct fields intentionally unused
module shirecache_idx_cop_sm_tb
  import shirecache_pkg::*;
  import dft_pkg::*;
#(
  parameter bit SkipPowerUpAllInv = 1'b0
) (
  input  logic                        clk_i,
  input  logic                        rst_ni,
  input  logic                        rst_c_ni,

  // MBIST
  input  logic                        mbist_on_i,

  // ESR write interface (scalar)
  input  logic                        wr_en_idx_cop_sm_ctl_i,
  input  logic                        wr_en_idx_cop_sm_data0_i,
  input  logic                        wr_en_idx_cop_sm_data1_i,
  input  logic                        wr_en_idx_cop_sm_ecc_i,
  input  logic                        wr_en_idx_cop_sm_physical_index_i,
  input  logic [DwDataSize-1:0]       wr_data_i,

  // ESR config (scalar)
  input  logic [SetIdSize-1:0]        esr_sc_l2_set_base_i,
  input  logic [SetAllocSize-1:0]     esr_sc_l2_set_size_i,
  input  logic [SetIdSize-1:0]        esr_sc_l3_set_base_i,
  input  logic [SetAllocSize-1:0]     esr_sc_l3_set_size_i,
  input  logic [SetIdSize-1:0]        esr_sc_scp_set_base_i,
  input  logic [SetAllocSize-1:0]     esr_sc_scp_set_size_i,

  // Reqq handshake
  input  logic                        pipe_idx_cop_req_ready_i,

  // Cbuf state (simplified — all invalid for basic tests)
  input  logic [CbufSize-1:0]         cbuf_mem_valid_i,

  // IDs
  input  logic [BankIdSize-1:0]       my_bank_id_i,
  input  logic [ScpShireIdSize-1:0]   my_scp_virtual_shire_id_i,

  // Outputs
  output logic                        pipe_idx_cop_aborted_o,
  output logic                        pipe_idx_cop_l2_busy_o,
  output logic                        pipe_idx_cop_l3_busy_o,
  output logic                        pipe_idx_cop_req_valid_o,
  output logic [5:0]                  pipe_idx_cop_req_opcode_o,
  output logic [PaSize-1:0]           pipe_idx_cop_req_address_o,
  output logic                        pipe_idx_cop_req_sync_flush_o,
  output logic                        pipe_idx_cop_is_active_o,
  output logic [EsrStatusSize-1:0]    esr_status_ctl_o
);

  // Construct bank_esr_ctl_t from scalar inputs
  bank_esr_ctl_t esr_ctl;
  always_comb begin
    esr_ctl = '0;
    esr_ctl.wr_en_idx_cop_sm_ctl            = wr_en_idx_cop_sm_ctl_i;
    esr_ctl.wr_en_idx_cop_sm_data0          = wr_en_idx_cop_sm_data0_i;
    esr_ctl.wr_en_idx_cop_sm_data1          = wr_en_idx_cop_sm_data1_i;
    esr_ctl.wr_en_idx_cop_sm_ecc            = wr_en_idx_cop_sm_ecc_i;
    esr_ctl.wr_en_idx_cop_sm_physical_index = wr_en_idx_cop_sm_physical_index_i;
    esr_ctl.wr_data                         = wr_data_i;
    esr_ctl.esr_sc_l2_set_base              = esr_sc_l2_set_base_i;
    esr_ctl.esr_sc_l2_set_size              = esr_sc_l2_set_size_i;
    esr_ctl.esr_sc_l3_set_base              = esr_sc_l3_set_base_i;
    esr_ctl.esr_sc_l3_set_size              = esr_sc_l3_set_size_i;
    esr_ctl.esr_sc_scp_set_base             = esr_sc_scp_set_base_i;
    esr_ctl.esr_sc_scp_set_size             = esr_sc_scp_set_size_i;
  end

  // Construct cbuf_state_t
  cbuf_state_t cbuf_state;
  assign cbuf_state.cbuf_mem_valid = cbuf_mem_valid_i;
  assign cbuf_state.cbuf_mem       = '0;

  // DFT off
  dft_t dft;
  assign dft = '0;

  // DUT
  idx_cop_req_t              req_info;
  idx_cop_sm_esr_status_t    esr_status;
  idx_cop_sm_dbg_write_t     dbg_wr;
  idx_cop_sm_dbg_read_t      dbg_rd;
  assign dbg_rd = '0;

  shirecache_pipe_idx_cop_sm #(
    .SkipPowerUpAllInv (SkipPowerUpAllInv)
  ) u_dut (
    .clk_i                       (clk_i),
    .rst_ni                      (rst_ni),
    .rst_c_ni                    (rst_c_ni),
    .dft_i                       (dft),
    .mbist_on_i                  (mbist_on_i),
    .pipe_idx_cop_aborted_o      (pipe_idx_cop_aborted_o),
    .pipe_idx_cop_l2_busy_o      (pipe_idx_cop_l2_busy_o),
    .pipe_idx_cop_l3_busy_o      (pipe_idx_cop_l3_busy_o),
    .pipe_idx_cop_req_ready_i    (pipe_idx_cop_req_ready_i),
    .pipe_idx_cop_req_valid_o    (pipe_idx_cop_req_valid_o),
    .pipe_idx_cop_req_info_o     (req_info),
    .pipe_idx_cop_is_active_o    (pipe_idx_cop_is_active_o),
    .pipe_cbuf_state_info_i      (cbuf_state),
    .esr_dbg_wr_info_o           (dbg_wr),
    .dbg_rd_info_i               (dbg_rd),
    .esr_ctl_i                   (esr_ctl),
    .pipe_idx_cop_sm_esr_status_o(esr_status),
    .my_bank_id_i                (my_bank_id_i),
    .my_scp_virtual_shire_id_i   (my_scp_virtual_shire_id_i)
  );

  assign pipe_idx_cop_req_opcode_o     = req_info.opcode;
  assign pipe_idx_cop_req_address_o    = req_info.address;
  assign pipe_idx_cop_req_sync_flush_o = req_info.sync_flush_victims;
  assign esr_status_ctl_o              = esr_status.ctl;

endmodule : shirecache_idx_cop_sm_tb

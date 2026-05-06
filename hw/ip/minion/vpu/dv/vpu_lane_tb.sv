// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

/* verilator lint_off IMPORTSTAR */
import dft_pkg::*;
import etlink_pkg::*;
import minion_pkg::*;
import minion_dcache_pkg::*;
import vpu_pkg::*;
import vpu_defs_pkg::*;
/* verilator lint_on IMPORTSTAR */

module vpu_lane_tb (
  input  logic clk_i,
  input  logic rst_ni,
  input  logic fromint_valid_i,
  input  logic [VPU_DATA_S_SZ-1:0] fromint_data_i,
  input  logic f4_load_bypass_i,
  input  logic [FREG_ADDR_SIZE-1:0] bypass_addr_i,
  input  logic [VPU_DATA_S_SZ-1:0] bypass_data_i,
  input  logic sh_sw_valid_i,
  input  logic sh_sw_clock_valid_i,
  input  logic sh_sw_add_i,
  input  logic [VPU_DATA_S_SZ-1:0] ex_in1_i,
  input  logic [VPU_DATA_S_SZ-1:0] ex_in3_i,
  input  logic bypass_pipe_en_i,
  input  logic tima_valid_i,
  input  logic tima_valid_unqual_i,
  input  logic txfma_valid_i,
  input  logic txfma_clock_valid_i,
  input  logic rom_valid_i,
  input  logic rom_clock_valid_i,
  input  logic chicken_vpulane_i,
  input  logic chicken_vputima_i,
  output logic [VPU_DATA_S_SZ-1:0] ex_regfile_rdata_bypass0_o,
  output logic [VPU_DATA_S_SZ-1:0] ex_regfile_rdata_bypass1_o,
  output logic [VPU_DATA_S_SZ-1:0] f2_fswizz_rdata_o,
  output logic f3_maskop_bit_o,
  output logic f8_txfma_comp_res_o,
  output logic [VPU_DATA_S_SZ-1:0] f8_wdata_o,
  output logic [CORE_FCSR_FLAG_BITS_SZ-1:0] f8_wflags_o,
  output logic [VPU_DATA_S_SZ-1:0] f8_sh_sw_wdata_o,
  output logic [1:0] txfma_trans_dbg_o
);

  vpu_input_t ex_req_lane;
  vpu_bypass_force_ctrl ex_bypass_force_ctrl;
  dcache_vpu_scp_resp dcache_scp_resp;
  logic [1:0][VPU_DATA_S_SZ-1:0] ex_regfile_rdata_bypass;

  assign ex_regfile_rdata_bypass0_o = ex_regfile_rdata_bypass[0];
  assign ex_regfile_rdata_bypass1_o = ex_regfile_rdata_bypass[1];

  always_comb begin
    ex_req_lane = '0;
    ex_req_lane.sigs.fromint = fromint_valid_i;
    ex_req_lane.in1 = ex_in1_i;
    ex_req_lane.in2 = 32'h3333_4444;
    ex_req_lane.in3 = ex_in3_i;
    ex_req_lane.rm = 3'd0;
    ex_req_lane.typ = 2'd0;
    ex_req_lane.sigs.dtype = VpuDtypeF32;
    ex_req_lane.sigs.cmd = sh_sw_add_i ? VpuCmdAdd : VpuCmdMvXf;

    ex_bypass_force_ctrl = '0;
    dcache_scp_resp = '0;
  end

  vpu_lane dut (
    .lane_id                 ('0),
    .clock                   (clk_i),
    .reset                   (~rst_ni),
    .chicken_bit_vpulane     (chicken_vpulane_i),
    .chicken_bit_vputima     (chicken_vputima_i),
    .mem_ctrl_override       ('0),
    .id_regfile_raddr1       (bypass_addr_i),
    .id_regfile_raddr2       ('0),
    .id_regfile_raddr3       ('0),
    .id_regfile_ren          (3'b001),
    .id_regfile_thread_id    (3'b000),
    .id_trans_busy           (1'b0),
    .ex_regfile_raddr1       (bypass_addr_i),
    .ex_regfile_raddr2       ('0),
    .ex_regfile_raddr3       ('0),
    .ex_regfile_thread_id    (3'b000),
    .ex_mask_in1             (1'b1),
    .ex_tena_regfile_bypass_en(1'b0),
    .ex_tenb_regfile_bypass_en(1'b0),
    .ex_req_lane             (ex_req_lane),
    .ex_fromint_data         (fromint_data_i),
    .ex_bypass_force_ctrl    (ex_bypass_force_ctrl),
    .ex_tima_valid           ({VPU_NUM_TIMA{tima_valid_i}}),
    .ex_tima_valid_unqual    (tima_valid_unqual_i),
    .ex_tima_sa              (1'b0),
    .ex_tima_sb              (1'b0),
    .ex_tima_tenb_raddr      ('0),
    .ex_tena_rf_rdata        ('0),
    .ex_tena_rf_tima_rdata   ('0),
    .ex_rom_valid            (rom_valid_i),
    .ex_sh_sw_valid          (sh_sw_valid_i),
    .ex_txfma_valid          (txfma_valid_i),
    .ex_txfma_clock_valid    (txfma_clock_valid_i),
    .ex_rom_clock_valid      (rom_clock_valid_i),
    .ex_sh_sw_clock_valid    (sh_sw_clock_valid_i),
    .f2_tima_ren3            (1'b0),
    .f2_tima_tenc_raddr      ('0),
    .f2_tima_tenc_wen        ('0),
    .f2_tima_rf_wen          ('0),
    .f2_fswizz_rdata1        ('0),
    .f2_fswizz_rdata2        ('0),
    .f2_fswizz_rdata3        ('0),
    .f2_tenb_regfile_wen_l   (1'b0),
    .dcache_scp_resp         (dcache_scp_resp),
    .f3_regfile_wen_l        (1'b0),
    .f3_tenb_regfile_wen_l   (1'b0),
    .f3_tenb_regfile_wdata_l ('0),
    .f3_tenb_regfile_waddr_l ('0),
    .f3_tima_tenc_wen        ('0),
    .f3_tima_tenc_waddr      ('0),
    .f3_tima_rf_wen          ('0),
    .f3_thread_id            (1'b0),
    .f3_regfile_waddr        ('0),
    .f3_regfile_wmask        (1'b0),
    .f3_bypass_clk_en        (bypass_pipe_en_i),
    .f3_data_vector          (1'b1),
    .f4_regfile_wen_l        (f4_load_bypass_i),
    .f4_regfile_waddr_l      (bypass_addr_i),
    .f4_regfile_wdata_l      (bypass_data_i),
    .f4_regfile_wmask_l      ({1'b0, f4_load_bypass_i}),
    .f4_regfile_thrid_l      (1'b0),
    .f4_thread_id            (1'b0),
    .f4_regfile_waddr        ('0),
    .f4_regfile_wmask        (1'b0),
    .f4_bypass_clk_en        (bypass_pipe_en_i),
    .f5_thread_id            (1'b0),
    .f5_regfile_wmask        (1'b0),
    .f5_regfile_waddr        ('0),
    .f5_bypass_clk_en        (bypass_pipe_en_i),
    .f6_thread_id            (1'b0),
    .f6_regfile_wmask        (1'b0),
    .f6_regfile_waddr        ('0),
    .f6_bypass_clk_en        (bypass_pipe_en_i),
    .f7_thread_id            (1'b0),
    .f7_regfile_wmask        (1'b0),
    .f7_regfile_waddr        ('0),
    .f7_bypass_clk_en        (bypass_pipe_en_i),
    .f7_regfile_wen          (1'b0),
    .f8_thread_id            (1'b0),
    .f8_regfile_wmask        (1'b0),
    .f8_regfile_waddr        ('0),
    .f8_regfile_wen          (1'b0),
    .f8_txfma_op             (1'b0),
    .f8_data_vector          (1'b1),
    .ex_regfile_rdata_bypass (ex_regfile_rdata_bypass),
    .f2_fswizz_rdata         (f2_fswizz_rdata_o),
    .f3_maskop_bit           (f3_maskop_bit_o),
    .f8_txfma_comp_res       (f8_txfma_comp_res_o),
    .f8_wdata                (f8_wdata_o),
    .f8_wflags               (f8_wflags_o),
    .f8_sh_sw_wdata          (f8_sh_sw_wdata_o),
    .txfma_trans_dbg         (txfma_trans_dbg_o)
  );

endmodule

// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

/* verilator lint_off UNUSED */  // rst_ni is kept for the standard harness; the DUT has no reset.
module cosim_vpu_bypass_tb (
  input  logic                        clk_i,
  input  logic                        rst_ni,
  input  logic [`CORE_GSC_CNT_RANGE]  lane_id_i,
  input  logic [`FREG_ADDR_RANGE]     ex_rf_raddr0_i,
  input  logic [`FREG_ADDR_RANGE]     ex_rf_raddr1_i,
  input  logic [`FREG_ADDR_RANGE]     ex_rf_raddr2_i,
  input  logic                        ex_regfile_thread_id0_i,
  input  logic                        ex_regfile_thread_id1_i,
  input  logic                        ex_regfile_thread_id2_i,
  input  logic [31:0]                 ex_rf_rdata0_i,
  input  logic [31:0]                 ex_rf_rdata1_i,
  input  logic [31:0]                 ex_rf_rdata2_i,
  input  logic                        ex_req_use_prev_sigs_i,
  input  logic [$bits(minion_pkg::vpu_ctrl_sigs_t)-1:0] ex_req_sigs_i,
  input  logic                        ex_req_fromint_i,
  input  logic                        ex_req_txfma_i,
  input  logic [19:0]                 ex_req_imm_i,
  input  logic [2:0]                  ex_req_rm_i,
  input  logic [1:0]                  ex_req_typ_i,
  input  logic [31:0]                 ex_fromint_data_i,
  input  logic [31:0]                 ex_tena_rf_rdata_i,
  input  logic [31:0]                 ex_tenb_rf_rdata_i,
  input  logic                        ex_tena_regfile_bypass_en_i,
  input  logic                        ex_tenb_regfile_bypass_en_i,
  input  logic                        ex_bypass_force_txfma_in0_i,
  input  logic                        ex_bypass_force_txfma_in1_i,
  input  logic                        ex_bypass_force_txfma_in2_i,
  input  logic                        ex_bypass_force_shsw_in1_i,
  input  logic                        ex_bypass_force_shsw_in2_i,
  input  logic [31:0]                 f3_sh_sw_data_i,
  input  logic [5:0]                  f3_sh_sw_exc_i,
  input  logic [$clog2(vpu_pkg::VpuRegfileNum)-1:0] f3_regfile_waddr_i,
  input  logic                        f3_regfile_wmask_i,
  input  logic                        f3_thread_id_i,
  input  logic                        f3_bypass_clk_en_i,
  input  logic                        f3_data_vector_i,
  input  logic [$clog2(vpu_pkg::VpuRegfileNum)-1:0] f4_regfile_waddr_i,
  input  logic                        f4_regfile_wmask_i,
  input  logic                        f4_thread_id_i,
  input  logic                        f4_bypass_clk_en_i,
  input  logic [$clog2(vpu_pkg::VpuRegfileNum)-1:0] f4_regfile_waddr_l_i,
  input  logic [1:0]                  f4_regfile_wmask_l_i,
  input  logic                        f4_regfile_thread_id_l_i,
  input  logic [31:0]                 f4_regfile_wdata_l_i,
  input  logic [$clog2(vpu_pkg::VpuRegfileNum)-1:0] f5_regfile_waddr_i,
  input  logic                        f5_regfile_wmask_i,
  input  logic                        f5_thread_id_i,
  input  logic                        f5_bypass_clk_en_i,
  input  logic [$clog2(vpu_pkg::VpuRegfileNum)-1:0] f6_regfile_waddr_i,
  input  logic                        f6_regfile_wmask_i,
  input  logic                        f6_thread_id_i,
  input  logic                        f6_bypass_clk_en_i,
  input  logic [$clog2(vpu_pkg::VpuRegfileNum)-1:0] f7_regfile_waddr_i,
  input  logic                        f7_regfile_wmask_i,
  input  logic                        f7_thread_id_i,
  input  logic                        f7_bypass_clk_en_i,
  input  logic [$clog2(vpu_pkg::VpuRegfileNum)-1:0] f8_regfile_waddr_i,
  input  logic                        f8_regfile_wmask_i,
  input  logic                        f8_thread_id_i,
  input  logic [31:0]                 f8_txfma_data_i,
  input  logic [5:0]                  f8_txfma_exc_i,
  input  logic                        f8_data_vector_i,
  input  logic                        f8_txfma_op_i,
  output logic                        orig_ex_req_use_prev_sigs_o,
  output logic                        new_ex_req_use_prev_sigs_o,
  output logic [$bits(minion_pkg::vpu_ctrl_sigs_t)-1:0] orig_ex_req_sigs_o,
  output logic [$bits(minion_pkg::vpu_ctrl_sigs_t)-1:0] new_ex_req_sigs_o,
  output logic [19:0]                 orig_ex_req_imm_o,
  output logic [19:0]                 new_ex_req_imm_o,
  output logic [2:0]                  orig_ex_req_rm_o,
  output logic [2:0]                  new_ex_req_rm_o,
  output logic [1:0]                  orig_ex_req_typ_o,
  output logic [1:0]                  new_ex_req_typ_o,
  output logic [31:0]                 orig_ex_req_in1_o,
  output logic [31:0]                 new_ex_req_in1_o,
  output logic [31:0]                 orig_ex_req_in2_o,
  output logic [31:0]                 new_ex_req_in2_o,
  output logic [31:0]                 orig_ex_req_in3_o,
  output logic [31:0]                 new_ex_req_in3_o,
  output logic                        orig_f3_maskop_bit_o,
  output logic                        new_f3_maskop_bit_o,
  output logic [5:0]                  orig_f8_wflags_o,
  output logic [5:0]                  new_f8_wflags_o,
  output logic [31:0]                 orig_f8_wdata_o,
  output logic [31:0]                 new_f8_wdata_o,
  output logic [31:0]                 orig_f8_sh_sw_wdata_o,
  output logic [31:0]                 new_f8_sh_sw_wdata_o
);
/* verilator lint_on UNUSED */

  logic [2:0][4:0] ex_rf_raddr;
  logic [2:0] ex_regfile_thread_id;
  logic [2:0][31:0] ex_rf_rdata;

  vpu_input_t orig_ex_req_lane;
  vpu_bypass_force_ctrl orig_ex_bypass_force_ctrl;
  vpu_output_t orig_f3_sh_sw_bits;
  vpu_output_t orig_f8_txfma_res;
  vpu_input_t orig_ex_req;

  vpu_pkg::vpu_input_t new_ex_req_lane;
  vpu_pkg::vpu_bypass_force_ctrl_t new_ex_bypass_force_ctrl;
  vpu_pkg::vpu_output_t new_f3_sh_sw_bits;
  vpu_pkg::vpu_output_t new_f8_txfma_res;
  vpu_pkg::vpu_input_t new_ex_req;

  always_comb begin
    ex_rf_raddr[0] = ex_rf_raddr0_i;
    ex_rf_raddr[1] = ex_rf_raddr1_i;
    ex_rf_raddr[2] = ex_rf_raddr2_i;
    ex_regfile_thread_id[0] = ex_regfile_thread_id0_i;
    ex_regfile_thread_id[1] = ex_regfile_thread_id1_i;
    ex_regfile_thread_id[2] = ex_regfile_thread_id2_i;
    ex_rf_rdata[0] = ex_rf_rdata0_i;
    ex_rf_rdata[1] = ex_rf_rdata1_i;
    ex_rf_rdata[2] = ex_rf_rdata2_i;

    orig_ex_req_lane = '0;
    orig_ex_req_lane.use_prev_sigs = ex_req_use_prev_sigs_i;
    orig_ex_req_lane.sigs = vpu_ctrl_sigs_t'(ex_req_sigs_i);
    orig_ex_req_lane.sigs.fromint = ex_req_fromint_i;
    orig_ex_req_lane.sigs.txfma = ex_req_txfma_i;
    orig_ex_req_lane.imm = ex_req_imm_i;
    orig_ex_req_lane.rm = ex_req_rm_i;
    orig_ex_req_lane.typ = ex_req_typ_i;

    new_ex_req_lane = '0;
    new_ex_req_lane.use_prev_sigs = ex_req_use_prev_sigs_i;
    new_ex_req_lane.sigs = minion_pkg::vpu_ctrl_sigs_t'(ex_req_sigs_i);
    new_ex_req_lane.sigs.fromint = ex_req_fromint_i;
    new_ex_req_lane.sigs.txfma = ex_req_txfma_i;
    new_ex_req_lane.imm = ex_req_imm_i;
    new_ex_req_lane.rm = ex_req_rm_i;
    new_ex_req_lane.typ = ex_req_typ_i;

    orig_ex_bypass_force_ctrl = '0;
    orig_ex_bypass_force_ctrl.txfma_in0 = ex_bypass_force_txfma_in0_i;
    orig_ex_bypass_force_ctrl.txfma_in1 = ex_bypass_force_txfma_in1_i;
    orig_ex_bypass_force_ctrl.txfma_in2 = ex_bypass_force_txfma_in2_i;
    orig_ex_bypass_force_ctrl.shsw_in1 = ex_bypass_force_shsw_in1_i;
    orig_ex_bypass_force_ctrl.shsw_in2 = ex_bypass_force_shsw_in2_i;

    new_ex_bypass_force_ctrl = '0;
    new_ex_bypass_force_ctrl.txfma_in0 = ex_bypass_force_txfma_in0_i;
    new_ex_bypass_force_ctrl.txfma_in1 = ex_bypass_force_txfma_in1_i;
    new_ex_bypass_force_ctrl.txfma_in2 = ex_bypass_force_txfma_in2_i;
    new_ex_bypass_force_ctrl.shsw_in1 = ex_bypass_force_shsw_in1_i;
    new_ex_bypass_force_ctrl.shsw_in2 = ex_bypass_force_shsw_in2_i;

    orig_f3_sh_sw_bits = '{data: f3_sh_sw_data_i, exc: f3_sh_sw_exc_i};
    new_f3_sh_sw_bits = '{data: f3_sh_sw_data_i, exc: f3_sh_sw_exc_i};
    orig_f8_txfma_res = '{data: f8_txfma_data_i, exc: f8_txfma_exc_i};
    new_f8_txfma_res = '{data: f8_txfma_data_i, exc: f8_txfma_exc_i};
  end

  vpu_bypass_orig u_orig (
    .lane_id                   (lane_id_i),
    .clock                     (clk_i),
    .ex_rf_raddr               (ex_rf_raddr),
    .ex_regfile_thread_id      (ex_regfile_thread_id),
    .ex_rf_rdata               (ex_rf_rdata),
    .ex_req_lane               (orig_ex_req_lane),
    .ex_fromint_data           (ex_fromint_data_i),
    .ex_tena_rf_rdata          (ex_tena_rf_rdata_i),
    .ex_tenb_rf_rdata          (ex_tenb_rf_rdata_i),
    .ex_tena_regfile_bypass_en (ex_tena_regfile_bypass_en_i),
    .ex_tenb_regfile_bypass_en (ex_tenb_regfile_bypass_en_i),
    .ex_bypass_force_ctrl      (orig_ex_bypass_force_ctrl),
    .f3_sh_sw_bits             (orig_f3_sh_sw_bits),
    .f3_regfile_waddr          (f3_regfile_waddr_i),
    .f3_regfile_wmask          (f3_regfile_wmask_i),
    .f3_thread_id              (f3_thread_id_i),
    .f3_bypass_clk_en          (f3_bypass_clk_en_i),
    .f3_data_vector            (f3_data_vector_i),
    .f4_regfile_waddr          (f4_regfile_waddr_i),
    .f4_regfile_wmask          (f4_regfile_wmask_i),
    .f4_thread_id              (f4_thread_id_i),
    .f4_bypass_clk_en          (f4_bypass_clk_en_i),
    .f4_regfile_waddr_l        (f4_regfile_waddr_l_i),
    .f4_regfile_wmask_l        (f4_regfile_wmask_l_i),
    .f4_regfile_thrid_l        (f4_regfile_thread_id_l_i),
    .f4_regfile_wdata_l        (f4_regfile_wdata_l_i),
    .f5_regfile_waddr          (f5_regfile_waddr_i),
    .f5_regfile_wmask          (f5_regfile_wmask_i),
    .f5_thread_id              (f5_thread_id_i),
    .f5_bypass_clk_en          (f5_bypass_clk_en_i),
    .f6_regfile_waddr          (f6_regfile_waddr_i),
    .f6_regfile_wmask          (f6_regfile_wmask_i),
    .f6_thread_id              (f6_thread_id_i),
    .f6_bypass_clk_en          (f6_bypass_clk_en_i),
    .f7_regfile_waddr          (f7_regfile_waddr_i),
    .f7_regfile_wmask          (f7_regfile_wmask_i),
    .f7_thread_id              (f7_thread_id_i),
    .f7_bypass_clk_en          (f7_bypass_clk_en_i),
    .f8_regfile_waddr          (f8_regfile_waddr_i),
    .f8_regfile_wmask          (f8_regfile_wmask_i),
    .f8_thread_id              (f8_thread_id_i),
    .f8_txfma_res              (orig_f8_txfma_res),
    .f8_data_vector            (f8_data_vector_i),
    .f8_txfma_op               (f8_txfma_op_i),
    .ex_req                    (orig_ex_req),
    .f3_maskop_bit             (orig_f3_maskop_bit_o),
    .f8_wflags                 (orig_f8_wflags_o),
    .f8_wdata                  (orig_f8_wdata_o),
    .f8_sh_sw_wdata            (orig_f8_sh_sw_wdata_o)
  );

  vpu_bypass u_new (
    .lane_id_i                 (lane_id_i),
    .clk_i                     (clk_i),
    .ex_rf_raddr_i             (ex_rf_raddr),
    .ex_regfile_thread_id_i        (ex_regfile_thread_id),
    .ex_rf_rdata_i             (ex_rf_rdata),
    .ex_req_lane_i             (new_ex_req_lane),
    .ex_fromint_data_i         (ex_fromint_data_i),
    .ex_tena_rf_rdata_i        (ex_tena_rf_rdata_i),
    .ex_tenb_rf_rdata_i        (ex_tenb_rf_rdata_i),
    .ex_tena_regfile_bypass_en_i,
    .ex_tenb_regfile_bypass_en_i,
    .ex_bypass_force_ctrl_i    (new_ex_bypass_force_ctrl),
    .f3_sh_sw_bits_i           (new_f3_sh_sw_bits),
    .f3_regfile_waddr_i        (f3_regfile_waddr_i),
    .f3_regfile_wmask_i        (f3_regfile_wmask_i),
    .f3_thread_id_i            (f3_thread_id_i),
    .f3_bypass_clk_en_i        (f3_bypass_clk_en_i),
    .f3_data_vector_i          (f3_data_vector_i),
    .f4_regfile_waddr_i        (f4_regfile_waddr_i),
    .f4_regfile_wmask_i        (f4_regfile_wmask_i),
    .f4_thread_id_i            (f4_thread_id_i),
    .f4_bypass_clk_en_i        (f4_bypass_clk_en_i),
    .f4_regfile_waddr_l_i      (f4_regfile_waddr_l_i),
    .f4_regfile_wmask_l_i      (f4_regfile_wmask_l_i),
    .f4_regfile_thread_id_l_i  (f4_regfile_thread_id_l_i),
    .f4_regfile_wdata_l_i      (f4_regfile_wdata_l_i),
    .f5_regfile_waddr_i        (f5_regfile_waddr_i),
    .f5_regfile_wmask_i        (f5_regfile_wmask_i),
    .f5_thread_id_i            (f5_thread_id_i),
    .f5_bypass_clk_en_i        (f5_bypass_clk_en_i),
    .f6_regfile_waddr_i        (f6_regfile_waddr_i),
    .f6_regfile_wmask_i        (f6_regfile_wmask_i),
    .f6_thread_id_i            (f6_thread_id_i),
    .f6_bypass_clk_en_i        (f6_bypass_clk_en_i),
    .f7_regfile_waddr_i        (f7_regfile_waddr_i),
    .f7_regfile_wmask_i        (f7_regfile_wmask_i),
    .f7_thread_id_i            (f7_thread_id_i),
    .f7_bypass_clk_en_i        (f7_bypass_clk_en_i),
    .f8_regfile_waddr_i        (f8_regfile_waddr_i),
    .f8_regfile_wmask_i        (f8_regfile_wmask_i),
    .f8_thread_id_i            (f8_thread_id_i),
    .f8_txfma_res_i            (new_f8_txfma_res),
    .f8_data_vector_i          (f8_data_vector_i),
    .f8_txfma_op_i             (f8_txfma_op_i),
    .ex_req_o                  (new_ex_req),
    .f3_maskop_bit_o           (new_f3_maskop_bit_o),
    .f8_wflags_o               (new_f8_wflags_o),
    .f8_wdata_o                (new_f8_wdata_o),
    .f8_sh_sw_wdata_o          (new_f8_sh_sw_wdata_o)
  );

  assign orig_ex_req_use_prev_sigs_o = orig_ex_req.use_prev_sigs;
  assign new_ex_req_use_prev_sigs_o = new_ex_req.use_prev_sigs;
  assign orig_ex_req_sigs_o = orig_ex_req.sigs;
  assign new_ex_req_sigs_o = new_ex_req.sigs;
  assign orig_ex_req_imm_o = orig_ex_req.imm;
  assign new_ex_req_imm_o = new_ex_req.imm;
  assign orig_ex_req_rm_o = orig_ex_req.rm;
  assign new_ex_req_rm_o = new_ex_req.rm;
  assign orig_ex_req_typ_o = orig_ex_req.typ;
  assign new_ex_req_typ_o = new_ex_req.typ;
  assign orig_ex_req_in1_o = orig_ex_req.in1;
  assign new_ex_req_in1_o = new_ex_req.in1;
  assign orig_ex_req_in2_o = orig_ex_req.in2;
  assign new_ex_req_in2_o = new_ex_req.in2;
  assign orig_ex_req_in3_o = orig_ex_req.in3;
  assign new_ex_req_in3_o = new_ex_req.in3;

endmodule

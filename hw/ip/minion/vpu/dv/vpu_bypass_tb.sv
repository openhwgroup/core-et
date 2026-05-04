// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

/* verilator lint_off UNUSED */  // rst_ni is only here for the standard sim_ctrl harness; vpu_bypass has no reset.
module vpu_bypass_tb
  import minion_pkg::*;
  import vpu_pkg::*;
(
  input  logic                        clk_i,
  input  logic                        rst_ni,
  input  logic [CoreGscCntBits-1:0]   lane_id_i,
  input  logic [FregAddrSize-1:0]     ex_rf_raddr0_i,
  input  logic [FregAddrSize-1:0]     ex_rf_raddr1_i,
  input  logic [FregAddrSize-1:0]     ex_rf_raddr2_i,
  input  logic                        ex_regfile_thread_id0_i,
  input  logic                        ex_regfile_thread_id1_i,
  input  logic                        ex_regfile_thread_id2_i,
  input  logic [VpuDataSSz-1:0]       ex_rf_rdata0_i,
  input  logic [VpuDataSSz-1:0]       ex_rf_rdata1_i,
  input  logic [VpuDataSSz-1:0]       ex_rf_rdata2_i,
  input  logic                        ex_req_use_prev_sigs_i,
  input  logic [$bits(vpu_ctrl_sigs_t)-1:0] ex_req_sigs_i,
  input  logic                        ex_req_fromint_i,
  input  logic                        ex_req_txfma_i,
  input  logic [19:0]                 ex_req_imm_i,
  input  logic [2:0]                  ex_req_rm_i,
  input  logic [1:0]                  ex_req_typ_i,
  input  logic [VpuDataSSz-1:0]       ex_fromint_data_i,
  input  logic [VpuDataSSz-1:0]       ex_tena_rf_rdata_i,
  input  logic [VpuDataSSz-1:0]       ex_tenb_rf_rdata_i,
  input  logic                        ex_tena_regfile_bypass_en_i,
  input  logic                        ex_tenb_regfile_bypass_en_i,
  input  logic                        ex_bypass_force_txfma_in0_i,
  input  logic                        ex_bypass_force_txfma_in1_i,
  input  logic                        ex_bypass_force_txfma_in2_i,
  input  logic                        ex_bypass_force_shsw_in1_i,
  input  logic                        ex_bypass_force_shsw_in2_i,
  input  logic [VpuDataSSz-1:0]       f3_sh_sw_data_i,
  input  logic [CoreFcsrFlagBitsSz-1:0] f3_sh_sw_exc_i,
  input  logic [$clog2(VpuRegfileNum)-1:0] f3_regfile_waddr_i,
  input  logic                        f3_regfile_wmask_i,
  input  logic                        f3_thread_id_i,
  input  logic                        f3_bypass_clk_en_i,
  input  logic                        f3_data_vector_i,
  input  logic [$clog2(VpuRegfileNum)-1:0] f4_regfile_waddr_i,
  input  logic                        f4_regfile_wmask_i,
  input  logic                        f4_thread_id_i,
  input  logic                        f4_bypass_clk_en_i,
  input  logic [$clog2(VpuRegfileNum)-1:0] f4_regfile_waddr_l_i,
  input  logic [1:0]                  f4_regfile_wmask_l_i,
  input  logic                        f4_regfile_thread_id_l_i,
  input  logic [VpuDataSSz-1:0]       f4_regfile_wdata_l_i,
  input  logic [$clog2(VpuRegfileNum)-1:0] f5_regfile_waddr_i,
  input  logic                        f5_regfile_wmask_i,
  input  logic                        f5_thread_id_i,
  input  logic                        f5_bypass_clk_en_i,
  input  logic [$clog2(VpuRegfileNum)-1:0] f6_regfile_waddr_i,
  input  logic                        f6_regfile_wmask_i,
  input  logic                        f6_thread_id_i,
  input  logic                        f6_bypass_clk_en_i,
  input  logic [$clog2(VpuRegfileNum)-1:0] f7_regfile_waddr_i,
  input  logic                        f7_regfile_wmask_i,
  input  logic                        f7_thread_id_i,
  input  logic                        f7_bypass_clk_en_i,
  input  logic [$clog2(VpuRegfileNum)-1:0] f8_regfile_waddr_i,
  input  logic                        f8_regfile_wmask_i,
  input  logic                        f8_thread_id_i,
  input  logic [VpuDataSSz-1:0]       f8_txfma_data_i,
  input  logic [CoreFcsrFlagBitsSz-1:0] f8_txfma_exc_i,
  input  logic                        f8_data_vector_i,
  input  logic                        f8_txfma_op_i,
  output logic                        ex_req_use_prev_sigs_o,
  output logic [$bits(vpu_ctrl_sigs_t)-1:0] ex_req_sigs_o,
  output logic [19:0]                 ex_req_imm_o,
  output logic [2:0]                  ex_req_rm_o,
  output logic [1:0]                  ex_req_typ_o,
  output logic [VpuDataSSz-1:0]       ex_req_in1_o,
  output logic [VpuDataSSz-1:0]       ex_req_in2_o,
  output logic [VpuDataSSz-1:0]       ex_req_in3_o,
  output logic                        f3_maskop_bit_o,
  output logic [CoreFcsrFlagBitsSz-1:0] f8_wflags_o,
  output logic [VpuDataSSz-1:0]       f8_wdata_o,
  output logic [VpuDataSSz-1:0]       f8_sh_sw_wdata_o
);
/* verilator lint_on UNUSED */

  logic [2:0][FregAddrSize-1:0] ex_rf_raddr;
  logic [2:0] ex_regfile_thread_id;
  logic [2:0][VpuDataSSz-1:0] ex_rf_rdata;
  vpu_input_t ex_req_lane_i;
  vpu_input_t ex_req_lane_o;
  vpu_bypass_force_ctrl_t ex_bypass_force_ctrl;
  vpu_output_t f3_sh_sw_bits;
  vpu_output_t f8_txfma_res;

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

    ex_req_lane_i = '0;
    ex_req_lane_i.use_prev_sigs = ex_req_use_prev_sigs_i;
    ex_req_lane_i.sigs = vpu_ctrl_sigs_t'(ex_req_sigs_i);
    ex_req_lane_i.sigs.fromint = ex_req_fromint_i;
    ex_req_lane_i.sigs.txfma = ex_req_txfma_i;
    ex_req_lane_i.imm = ex_req_imm_i;
    ex_req_lane_i.rm = ex_req_rm_i;
    ex_req_lane_i.typ = ex_req_typ_i;

    ex_bypass_force_ctrl = '0;
    ex_bypass_force_ctrl.txfma_in0 = ex_bypass_force_txfma_in0_i;
    ex_bypass_force_ctrl.txfma_in1 = ex_bypass_force_txfma_in1_i;
    ex_bypass_force_ctrl.txfma_in2 = ex_bypass_force_txfma_in2_i;
    ex_bypass_force_ctrl.shsw_in1 = ex_bypass_force_shsw_in1_i;
    ex_bypass_force_ctrl.shsw_in2 = ex_bypass_force_shsw_in2_i;

    f3_sh_sw_bits = '{data: f3_sh_sw_data_i, exc: f3_sh_sw_exc_i};
    f8_txfma_res = '{data: f8_txfma_data_i, exc: f8_txfma_exc_i};
  end

  vpu_bypass u_dut (
    .lane_id_i,
    .clk_i,
    .ex_rf_raddr_i               (ex_rf_raddr),
    .ex_regfile_thread_id_i        (ex_regfile_thread_id),
    .ex_rf_rdata_i               (ex_rf_rdata),
    .ex_req_lane_i               (ex_req_lane_i),
    .ex_fromint_data_i,
    .ex_tena_rf_rdata_i,
    .ex_tenb_rf_rdata_i,
    .ex_tena_regfile_bypass_en_i,
    .ex_tenb_regfile_bypass_en_i,
    .ex_bypass_force_ctrl_i      (ex_bypass_force_ctrl),
    .f3_sh_sw_bits_i             (f3_sh_sw_bits),
    .f3_regfile_waddr_i,
    .f3_regfile_wmask_i,
    .f3_thread_id_i,
    .f3_bypass_clk_en_i,
    .f3_data_vector_i,
    .f4_regfile_waddr_i,
    .f4_regfile_wmask_i,
    .f4_thread_id_i,
    .f4_bypass_clk_en_i,
    .f4_regfile_waddr_l_i,
    .f4_regfile_wmask_l_i,
    .f4_regfile_thread_id_l_i,
    .f4_regfile_wdata_l_i,
    .f5_regfile_waddr_i,
    .f5_regfile_wmask_i,
    .f5_thread_id_i,
    .f5_bypass_clk_en_i,
    .f6_regfile_waddr_i,
    .f6_regfile_wmask_i,
    .f6_thread_id_i,
    .f6_bypass_clk_en_i,
    .f7_regfile_waddr_i,
    .f7_regfile_wmask_i,
    .f7_thread_id_i,
    .f7_bypass_clk_en_i,
    .f8_regfile_waddr_i,
    .f8_regfile_wmask_i,
    .f8_thread_id_i,
    .f8_txfma_res_i              (f8_txfma_res),
    .f8_data_vector_i,
    .f8_txfma_op_i,
    .ex_req_o                    (ex_req_lane_o),
    .f3_maskop_bit_o,
    .f8_wflags_o,
    .f8_wdata_o,
    .f8_sh_sw_wdata_o
  );

  assign ex_req_use_prev_sigs_o = ex_req_lane_o.use_prev_sigs;
  assign ex_req_sigs_o = ex_req_lane_o.sigs;
  assign ex_req_imm_o = ex_req_lane_o.imm;
  assign ex_req_rm_o = ex_req_lane_o.rm;
  assign ex_req_typ_o = ex_req_lane_o.typ;
  assign ex_req_in1_o = ex_req_lane_o.in1;
  assign ex_req_in2_o = ex_req_lane_o.in2;
  assign ex_req_in3_o = ex_req_lane_o.in3;

endmodule

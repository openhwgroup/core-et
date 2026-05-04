// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module vpu_bypass
  import minion_pkg::*;
  import vpu_pkg::*;
(
  input  logic [CoreGscCntBits-1:0]   lane_id_i,
  input  logic                        clk_i,
  input  logic [2:0][FregAddrSize-1:0] ex_rf_raddr_i,
  input  logic [2:0]                  ex_regfile_thread_id_i,
  input  logic [2:0][VpuDataSSz-1:0]  ex_rf_rdata_i,
  input  vpu_input_t                  ex_req_lane_i,
  input  logic [VpuDataSSz-1:0]       ex_fromint_data_i,
  input  logic [VpuDataSSz-1:0]       ex_tena_rf_rdata_i,
  input  logic [VpuDataSSz-1:0]       ex_tenb_rf_rdata_i,
  input  logic                        ex_tena_regfile_bypass_en_i,
  input  logic                        ex_tenb_regfile_bypass_en_i,
  input  vpu_bypass_force_ctrl_t      ex_bypass_force_ctrl_i,
  input  vpu_output_t                 f3_sh_sw_bits_i,
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
  input  vpu_output_t                 f8_txfma_res_i,
  input  logic                        f8_data_vector_i,
  input  logic                        f8_txfma_op_i,
  output vpu_input_t                  ex_req_o,
  output logic                        f3_maskop_bit_o,
  output logic [CoreFcsrFlagBitsSz-1:0] f8_wflags_o,
  output logic [VpuDataSSz-1:0]       f8_wdata_o,
  output logic [VpuDataSSz-1:0]       f8_sh_sw_wdata_o
);

  vpu_output_t                  f3_bits_mux;
  logic [VpuDataSSz-1:0]        f3_wdata;
  vpu_output_t                  f4_bits_q;
  logic [VpuDataSSz-1:0]        f4_wdata;
  vpu_output_t                  f5_bits_q;
  logic [VpuDataSSz-1:0]        f5_wdata;
  vpu_output_t                  f6_bits_q;
  logic [VpuDataSSz-1:0]        f6_wdata;
  vpu_output_t                  f7_bits_q;
  logic [VpuDataSSz-1:0]        f7_wdata;
  vpu_output_t                  f8_bits_q;
  vpu_output_t                  f8_bits_mux;
  logic [2:0][VpuDataSSz-1:0]   ex_regfile_rdata;

  always_comb begin
    for (int i = 0; i < 3; i++) begin
      logic f4_regfile_wmask_l_muxed;

      f4_regfile_wmask_l_muxed = (i == 0) ? f4_regfile_wmask_l_i[0] : f4_regfile_wmask_l_i[1];

      ex_regfile_rdata[i] =
          ((i == 0) && ex_tena_regfile_bypass_en_i) ? ex_tena_rf_rdata_i :
          ((i == 0) && ex_bypass_force_ctrl_i.txfma_in0) ? f8_txfma_res_i.data :
          ((i == 0) && ex_req_lane_i.sigs.fromint) ? ex_fromint_data_i :
          ((i == 1) && ex_tenb_regfile_bypass_en_i) ? ex_tenb_rf_rdata_i :
          ((i == 1) && ex_bypass_force_ctrl_i.shsw_in1) ? f8_bits_q.data :
          ((i == 1) && ex_bypass_force_ctrl_i.txfma_in1) ? f8_txfma_res_i.data :
          ((i == 2) && ex_bypass_force_ctrl_i.shsw_in2) ? f8_bits_q.data :
          ((i == 2) && ex_bypass_force_ctrl_i.txfma_in2) ? f8_txfma_res_i.data :
          ((i == 2) && ex_req_lane_i.sigs.fromint && ex_req_lane_i.sigs.txfma) ? ex_fromint_data_i :
          (ex_rf_raddr_i[i] == f4_regfile_waddr_l_i) && f4_regfile_wmask_l_muxed &&
              (ex_regfile_thread_id_i[i] == f4_regfile_thread_id_l_i) ? f4_regfile_wdata_l_i :
          (ex_rf_raddr_i[i] == f3_regfile_waddr_i) && f3_regfile_wmask_i &&
              (ex_regfile_thread_id_i[i] == f3_thread_id_i) ? f3_wdata :
          (ex_rf_raddr_i[i] == f4_regfile_waddr_i) && f4_regfile_wmask_i &&
              (ex_regfile_thread_id_i[i] == f4_thread_id_i) ? f4_wdata :
          (ex_rf_raddr_i[i] == f5_regfile_waddr_i) && f5_regfile_wmask_i &&
              (ex_regfile_thread_id_i[i] == f5_thread_id_i) ? f5_wdata :
          (ex_rf_raddr_i[i] == f6_regfile_waddr_i) && f6_regfile_wmask_i &&
              (ex_regfile_thread_id_i[i] == f6_thread_id_i) ? f6_wdata :
          (ex_rf_raddr_i[i] == f7_regfile_waddr_i) && f7_regfile_wmask_i &&
              (ex_regfile_thread_id_i[i] == f7_thread_id_i) ? f7_wdata :
          (ex_rf_raddr_i[i] == f8_regfile_waddr_i) && f8_regfile_wmask_i &&
              (ex_regfile_thread_id_i[i] == f8_thread_id_i) ? f8_wdata_o :
          ex_rf_rdata_i[i];
    end
  end

  always_comb begin
    ex_req_o = ex_req_lane_i;
    ex_req_o.in1 = ex_regfile_rdata[0];
    ex_req_o.in2 = ex_regfile_rdata[1];
    ex_req_o.in3 = ex_regfile_rdata[2];
  end

  always_comb begin
    f3_bits_mux = f3_sh_sw_bits_i;
    if ((lane_id_i != '0) && !f3_data_vector_i) begin
      f3_bits_mux = '0;
    end
  end

  assign f3_wdata = f3_bits_mux.data;
  assign f3_maskop_bit_o = f3_bits_mux.data[0];

  always_ff @(posedge clk_i) begin
    if (f3_bypass_clk_en_i) begin
      f4_bits_q <= f3_bits_mux;
    end
  end

  assign f4_wdata = f4_bits_q.data;

  always_ff @(posedge clk_i) begin
    if (f4_bypass_clk_en_i) begin
      f5_bits_q <= f4_bits_q;
    end
  end

  assign f5_wdata = f5_bits_q.data;

  always_ff @(posedge clk_i) begin
    if (f5_bypass_clk_en_i) begin
      f6_bits_q <= f5_bits_q;
    end
  end

  assign f6_wdata = f6_bits_q.data;

  always_ff @(posedge clk_i) begin
    if (f6_bypass_clk_en_i) begin
      f7_bits_q <= f6_bits_q;
    end
  end

  assign f7_wdata = f7_bits_q.data;

  always_ff @(posedge clk_i) begin
    if (f7_bypass_clk_en_i) begin
      f8_bits_q <= f7_bits_q;
    end
  end

  always_comb begin
    f8_bits_mux = f8_bits_q;

    if (f8_txfma_op_i) begin
      f8_bits_mux.data = f8_txfma_res_i.data;
      f8_bits_mux.exc = f8_txfma_res_i.exc;
    end

    if ((lane_id_i != '0) && !f8_data_vector_i) begin
      f8_bits_mux.data = '0;
    end
  end

  assign f8_sh_sw_wdata_o = f8_bits_q.data;
  assign f8_wdata_o       = f8_bits_mux.data;
  assign f8_wflags_o      = f8_bits_mux.exc;

endmodule

// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module vpu_mask
  import minion_pkg::*;
  import vpu_pkg::*;
(
  input  logic                           clk_i,
  input  logic                           rst_ni,

  input  logic                           ex_in_valid_i,
  input  logic                           ex_inst_valid_i,
  input  logic [MregAddrSize-1:0]        ex_regfile_raddr1_i,
  input  logic [MregAddrSize-1:0]        ex_regfile_raddr2_i,
  input  vpu_cmd_e                       ex_cmd_i,
  input  logic [VpuLaneNum-1:0]          ex_imm_i,
  input  logic                           ex_fromint_i,
  input  logic [XregSize-1:0]            ex_fromint_data_i,
  input  logic                           ex_thread_id_i,
  input  logic                           ex_ignore_mask_i,

  input  logic                           f2_thread_id_i,
  input  logic                           f2_wen_i,
  input  logic                           f2_core_kill_i,
  input  logic [MregAddrSize-1:0]        f2_waddr_i,
  input  logic                           f2_maskop_i,
  input  logic                           f2_tointm_i,

  input  logic                           f3_thread_id_i,
  input  logic                           f3_regmask_from_txfma_i,
  input  logic                           f3_regmask_from_short_i,
  input  logic                           f3_core_kill_i,
  input  logic [VpuLaneNum-1:0]          f3_regmask_wdata_lane_i,

  input  logic                           f4_regmask_from_txfma_i,
  input  logic                           f4_core_kill_i,

  input  logic                           f5_thread_id_i,
  input  logic                           f6_thread_id_i,
  input  logic                           f7_thread_id_i,

  input  logic                           f8_thread_id_i,
  input  logic [VpuLaneNum-1:0]          f8_regmask_comp_res_i,

  output logic [VpuLaneNum-1:0]          ex_mask_rf0_o,
  output logic [VpuLaneNum-1:0]          ex_mask_in1_o,
  output logic [VpuLaneNum-1:0]          f2_mask_rf0_o,
  output logic [XregSize-1:0]            f8_toint_data_o,
  output logic [VpuLaneNum-1:0]          f8_regmask_store_o,
  output logic [VpuMaskScoreboardSize-1:0] wb_mask_valid_o,
  output minion_mreg_dest_t [VpuMaskScoreboardSize-1:0] wb_mask_dest_o
);

  typedef logic [VpuRegmaskNum-1:0][VpuLaneNum-1:0] regmask_thread_t;
  typedef logic [NrThreadsDefault-1:0][VpuRegmaskNum-1:0][VpuLaneNum-1:0] regmask_rf_t;

  regmask_rf_t                           ex_mask_rf_q;
  regmask_rf_t                           f8_mask_rf_wdata_d;
  logic [1:0][VpuLaneNum-1:0]           ex_mask_rf_bypass_d;
  logic [VpuLaneNum-1:0]                mask_rf0_bypass_d;
  logic                                 f8_mask_rf_clk_en_d;

  regmask_thread_t                      f2_regmask_wdata_d;
  logic                                 f2_valid_q;
  vpu_cmd_e                             f2_cmd_q;
  logic [VpuDataSSz-1:0]                f2_in1_q;
  logic [VpuDataSSz-1:0]                f2_in2_q;
  logic [VpuLaneNum-1:0]                f2_imm_q;
  logic [VpuLaneNum-1:0]                f2_mask_rf0_q;

  logic [4:0]                           f2_toint_data_d;
  logic                                 f2_mova_x_m_d;
  logic [VpuLaneNum-1:0]                f2_regmask_store_to_mask_d;
  logic [VpuLaneNum-1:0]                f2_regmask_store_d;
  logic [VpuLaneNum-1:0]                f2_regmask_store_bypass_d;
  logic                                 f2_regmask_store_all_d;
  logic [MregAddrSize-1:0]              f2_regmask_waddr_d;

  logic                                 f3_valid_q;
  logic [VpuLaneNum-1:0]                f3_regmask_store_q;
  logic                                 f3_regmask_store_all_q;
  regmask_thread_t                      f3_regmask_wdata_q;
  logic [4:0]                           f3_toint_data_q;
  logic [MregAddrSize-1:0]              f3_regmask_waddr_q;
  logic                                 f3_mova_x_m_q;
  regmask_thread_t                      f3_regmask_wdata_sel_d;
  logic [VpuLaneNum-1:0]                f3_regmask_store_bypass_d;
  logic [VpuLaneNum-1:0]                f3_regmask_store_to_f4_d;
  logic                                 f3_regmask_store_all_qual_d;
  logic                                 f3_regmask_store_qual_d;

  logic                                 f4_valid_q;
  logic [VpuLaneNum-1:0]                f4_regmask_store_q;
  logic [4:0]                           f4_toint_data_q;
  logic [MregAddrSize-1:0]              f4_regmask_waddr_q;
  logic                                 f4_mova_x_m_q;
  logic [VpuLaneNum-1:0]                f4_regmask_store_to_f5_d;

  logic                                 f5_valid_q;
  logic [VpuLaneNum-1:0]                f5_regmask_store_q;
  logic [4:0]                           f5_toint_data_q;
  logic [MregAddrSize-1:0]              f5_regmask_waddr_q;
  logic                                 f5_mova_x_m_q;

  logic                                 f6_valid_q;
  logic [VpuLaneNum-1:0]                f6_regmask_store_q;
  logic [4:0]                           f6_toint_data_q;
  logic [MregAddrSize-1:0]              f6_regmask_waddr_q;
  logic                                 f6_mova_x_m_q;

  logic                                 f7_valid_q;
  logic [VpuLaneNum-1:0]                f7_regmask_store_q;
  logic [4:0]                           f7_toint_data_q;
  logic [MregAddrSize-1:0]              f7_regmask_waddr_q;
  logic                                 f7_mova_x_m_q;

  logic [VpuLaneNum-1:0]                f8_regmask_store_q;
  logic [4:0]                           f8_toint_data_pre_q;
  logic [MregAddrSize-1:0]              f8_regmask_waddr_q;
  logic                                 f8_mova_x_m_q;

  logic [15:0]                          mpopc_mask_d;
  logic [15:0]                          mpopc_rast_d;
  logic [VpuDataSSz-1:0]                ex_in1_d;
  logic [VpuDataSSz-1:0]                ex_in2_d;

  always_ff @(posedge clk_i) begin
    if (f8_mask_rf_clk_en_d) begin
      ex_mask_rf_q <= f8_mask_rf_wdata_d;
    end
  end

  always_comb begin
    ex_mask_rf_bypass_d = '0;
    mask_rf0_bypass_d = '0;
    f2_regmask_store_bypass_d = f2_regmask_store_to_mask_d;
    f3_regmask_store_bypass_d = f3_regmask_store_q;

    for (int lane = 0; lane < VpuLaneNum; lane++) begin
      logic [MregAddrSize-1:0] ex_regmask_raddr;
      logic [VpuLaneNum-1:0] ex_regmask_data;

      ex_regmask_raddr = ex_regfile_raddr1_i;
      ex_regmask_data = ex_mask_rf_q[ex_thread_id_i][ex_regmask_raddr];
      ex_mask_rf_bypass_d[0][lane] =
          (ex_regmask_raddr == f2_regmask_waddr_d) && (ex_thread_id_i == f2_thread_id_i) &&
              f2_regmask_store_bypass_d[lane] ? f2_regmask_wdata_d[0][lane] :
          (ex_regmask_raddr == f3_regmask_waddr_q) && (ex_thread_id_i == f3_thread_id_i) &&
              f3_regmask_store_bypass_d[lane] ? f3_regmask_wdata_sel_d[0][lane] :
          (ex_regmask_raddr == f8_regmask_waddr_q) && (ex_thread_id_i == f8_thread_id_i) &&
              f8_regmask_store_q[lane] ? f8_regmask_comp_res_i[lane] :
          ex_regmask_data[lane];

      ex_regmask_raddr = ex_regfile_raddr2_i;
      ex_regmask_data = ex_mask_rf_q[ex_thread_id_i][ex_regmask_raddr];
      ex_mask_rf_bypass_d[1][lane] =
          (ex_regmask_raddr == f2_regmask_waddr_d) && (ex_thread_id_i == f2_thread_id_i) &&
              f2_regmask_store_bypass_d[lane] ? f2_regmask_wdata_d[0][lane] :
          (ex_regmask_raddr == f3_regmask_waddr_q) && (ex_thread_id_i == f3_thread_id_i) &&
              f3_regmask_store_bypass_d[lane] ? f3_regmask_wdata_sel_d[0][lane] :
          (ex_regmask_raddr == f8_regmask_waddr_q) && (ex_thread_id_i == f8_thread_id_i) &&
              f8_regmask_store_q[lane] ? f8_regmask_comp_res_i[lane] :
          ex_regmask_data[lane];

      mask_rf0_bypass_d[lane] =
          (f2_regmask_waddr_d == '0) && (ex_thread_id_i == f2_thread_id_i) &&
              f2_regmask_store_bypass_d[lane] ? f2_regmask_wdata_d[0][lane] :
          (f3_regmask_waddr_q == '0) && (ex_thread_id_i == f3_thread_id_i) &&
              f3_regmask_store_bypass_d[lane] ? f3_regmask_wdata_sel_d[0][lane] :
          (f8_regmask_waddr_q == '0) && (ex_thread_id_i == f8_thread_id_i) &&
              f8_regmask_store_q[lane] ? f8_regmask_comp_res_i[lane] :
          ex_mask_rf_q[ex_thread_id_i][0][lane];
    end
  end

  always_comb begin
    if (ex_ignore_mask_i) begin
      ex_mask_rf0_o = '1;
    end else begin
      ex_mask_rf0_o = mask_rf0_bypass_d;
    end

    ex_mask_in1_o = mask_rf0_bypass_d;
    ex_in1_d = ex_fromint_i ? ex_fromint_data_i[VpuDataSSz-1:0]
                            : {{(VpuDataSSz - VpuLaneNum){1'b0}}, ex_mask_rf_bypass_d[0]};
    ex_in2_d = ex_fromint_i ? ex_fromint_data_i[XregSize-1 -: VpuDataSSz]
                            : {{(VpuDataSSz - VpuLaneNum){1'b0}}, ex_mask_rf_bypass_d[1]};
  end

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      f2_valid_q <= 1'b0;
    end else begin
      f2_valid_q <= ex_in_valid_i;
    end
  end

  always_ff @(posedge clk_i) begin
    f2_cmd_q <= ex_cmd_i;
    if (ex_in_valid_i) begin
      f2_in1_q <= ex_in1_d;
      f2_in2_q <= ex_in2_d;
      f2_imm_q <= ex_imm_i;
    end
    if (ex_inst_valid_i) begin
      f2_mask_rf0_q <= ex_mask_rf0_o;
    end
  end

  assign f2_mask_rf0_o = f2_mask_rf0_q;

  always_comb begin
    f2_regmask_wdata_d = {f2_in2_q, f2_in1_q};

    if (f2_cmd_q == VpuCmdMovMX) begin
      f2_regmask_wdata_d = {{(VpuRegmaskNum * VpuLaneNum - VpuLaneNum){1'b0}},
                            (f2_in1_q[VpuLaneNum-1:0] | f2_imm_q)};
    end
    if (f2_cmd_q == VpuCmdMand) begin
      f2_regmask_wdata_d = {{(VpuRegmaskNum * VpuLaneNum - VpuDataSSz){1'b0}}, (f2_in1_q & f2_in2_q)};
    end
    if (f2_cmd_q == VpuCmdMor) begin
      f2_regmask_wdata_d = {{(VpuRegmaskNum * VpuLaneNum - VpuDataSSz){1'b0}}, (f2_in1_q | f2_in2_q)};
    end
    if (f2_cmd_q == VpuCmdMxor) begin
      f2_regmask_wdata_d = {{(VpuRegmaskNum * VpuLaneNum - VpuDataSSz){1'b0}}, (f2_in1_q ^ f2_in2_q)};
    end
    if (f2_cmd_q == VpuCmdMnot) begin
      f2_regmask_wdata_d = {{(VpuRegmaskNum * VpuLaneNum - VpuDataSSz){1'b0}}, ~f2_in1_q};
    end

    f2_mova_x_m_d = (f2_cmd_q == VpuCmdMovaXM);

    f2_toint_data_d = '0;
    for (int n = 0; n < VpuLaneNum; n++) begin
      f2_toint_data_d += f2_in1_q[n] ? 5'd1 : 5'd0;
    end

    if (f2_cmd_q == VpuCmdMpopcz) begin
      f2_toint_data_d = '0;
      for (int n = 0; n < VpuLaneNum; n++) begin
        f2_toint_data_d += !f2_in1_q[n] ? 5'd1 : 5'd0;
      end
    end

    mpopc_mask_d = '1;
    mpopc_rast_d = '0;
    if (f2_cmd_q == VpuCmdMpocRast) begin
      unique case (f2_imm_q[3:0])
        4'd0:    mpopc_mask_d = 16'h0f0f;
        4'd1:    mpopc_mask_d = 16'h3c3c;
        4'd2:    mpopc_mask_d = 16'hf0f0;
        default: mpopc_mask_d = 16'hffff;
      endcase

      mpopc_rast_d = {f2_in2_q[7:0], f2_in1_q[7:0]} & mpopc_mask_d;
      f2_toint_data_d = '0;
      for (int n = 0; n < 16; n++) begin
        f2_toint_data_d += mpopc_rast_d[n] ? 5'd1 : 5'd0;
      end
    end
  end

  always_comb begin
    f2_regmask_store_to_mask_d = '0;

    if (f2_wen_i && (f2_cmd_q != VpuCmdMovaMX)) begin
      if (f2_maskop_i) begin
        f2_regmask_store_to_mask_d = '1;
      end
      if (f2_tointm_i) begin
        f2_regmask_store_to_mask_d = f2_mask_rf0_q;
      end
    end

    f2_regmask_store_d = f2_regmask_store_to_mask_d & {VpuLaneNum{!f2_core_kill_i}};
    f2_regmask_store_all_d = f2_wen_i && f2_maskop_i && (f2_cmd_q == VpuCmdMovaMX) && !f2_core_kill_i;
    f2_regmask_waddr_d = f2_waddr_i & {MregAddrSize{|f2_regmask_store_to_mask_d}};
  end

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      f3_valid_q <= 1'b0;
      f3_regmask_store_q <= '0;
      f3_regmask_store_all_q <= 1'b0;
    end else begin
      f3_valid_q <= f2_valid_q;
      if ((|f3_regmask_store_q) || (|f2_regmask_store_d)) begin
        f3_regmask_store_q <= f2_regmask_store_d;
      end
      f3_regmask_store_all_q <= f2_regmask_store_all_d;
    end
  end

  always_ff @(posedge clk_i) begin
    if ((|f2_regmask_store_to_mask_d) || f2_regmask_store_all_d) begin
      f3_regmask_wdata_q <= f2_regmask_wdata_d;
    end
    if (f2_valid_q) begin
      f3_toint_data_q <= f2_toint_data_d;
      f3_mova_x_m_q <= f2_mova_x_m_d;
    end
    if (|f2_regmask_store_to_mask_d) begin
      f3_regmask_waddr_q <= f2_regmask_waddr_d;
    end
  end

  assign f3_regmask_wdata_sel_d = f3_regmask_from_short_i
      ? {{(VpuRegmaskNum * VpuLaneNum - VpuLaneNum){1'b0}}, f3_regmask_wdata_lane_i}
      : f3_regmask_wdata_q;

  assign f3_regmask_store_to_f4_d = f3_regmask_store_q &
      {VpuLaneNum{((!f3_core_kill_i) && f3_regmask_from_txfma_i) || f3_regmask_from_short_i}};

  assign f3_regmask_store_all_qual_d = f3_regmask_store_all_q && !f3_core_kill_i;
  assign f3_regmask_store_qual_d = (|f3_regmask_store_q) && !f3_regmask_from_txfma_i && !f3_core_kill_i;

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      f4_valid_q <= 1'b0;
      f4_regmask_store_q <= '0;
    end else begin
      f4_valid_q <= f3_valid_q;
      if ((|f4_regmask_store_q) || (|f3_regmask_store_to_f4_d)) begin
        f4_regmask_store_q <= f3_regmask_store_to_f4_d;
      end
    end
  end

  always_ff @(posedge clk_i) begin
    if (f3_valid_q) begin
      f4_toint_data_q <= f3_toint_data_q;
      f4_mova_x_m_q <= f3_mova_x_m_q;
    end
    if (|f3_regmask_store_q) begin
      f4_regmask_waddr_q <= f3_regmask_waddr_q;
    end
  end

  assign f4_regmask_store_to_f5_d =
      f4_regmask_store_q & {VpuLaneNum{(!f4_core_kill_i) && f4_regmask_from_txfma_i}};

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      f5_valid_q <= 1'b0;
      f5_regmask_store_q <= '0;
    end else begin
      f5_valid_q <= f4_valid_q;
      if ((|f5_regmask_store_q) || (|f4_regmask_store_to_f5_d)) begin
        f5_regmask_store_q <= f4_regmask_store_to_f5_d;
      end
    end
  end

  always_ff @(posedge clk_i) begin
    if (f4_valid_q) begin
      f5_toint_data_q <= f4_toint_data_q;
      f5_mova_x_m_q <= f4_mova_x_m_q;
    end
    if (|f4_regmask_store_q) begin
      f5_regmask_waddr_q <= f4_regmask_waddr_q;
    end
  end

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      f6_valid_q <= 1'b0;
      f6_regmask_store_q <= '0;
    end else begin
      f6_valid_q <= f5_valid_q;
      if ((|f6_regmask_store_q) || (|f5_regmask_store_q)) begin
        f6_regmask_store_q <= f5_regmask_store_q;
      end
    end
  end

  always_ff @(posedge clk_i) begin
    if (f5_valid_q) begin
      f6_toint_data_q <= f5_toint_data_q;
      f6_mova_x_m_q <= f5_mova_x_m_q;
    end
    if (|f5_regmask_store_q) begin
      f6_regmask_waddr_q <= f5_regmask_waddr_q;
    end
  end

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      f7_valid_q <= 1'b0;
      f7_regmask_store_q <= '0;
    end else begin
      f7_valid_q <= f6_valid_q;
      if ((|f7_regmask_store_q) || (|f6_regmask_store_q)) begin
        f7_regmask_store_q <= f6_regmask_store_q;
      end
    end
  end

  always_ff @(posedge clk_i) begin
    if (f6_valid_q) begin
      f7_toint_data_q <= f6_toint_data_q;
      f7_mova_x_m_q <= f6_mova_x_m_q;
    end
    if (|f6_regmask_store_q) begin
      f7_regmask_waddr_q <= f6_regmask_waddr_q;
    end
  end

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      f8_regmask_store_q <= '0;
    end else begin
      if ((|f8_regmask_store_q) || (|f7_regmask_store_q)) begin
        f8_regmask_store_q <= f7_regmask_store_q;
      end
    end
  end

  always_ff @(posedge clk_i) begin
    if (f7_valid_q) begin
      f8_toint_data_pre_q <= f7_toint_data_q;
      f8_mova_x_m_q <= f7_mova_x_m_q;
    end
    if (|f7_regmask_store_q) begin
      f8_regmask_waddr_q <= f7_regmask_waddr_q;
    end
  end

  always_comb begin
    f8_mask_rf_wdata_d = ex_mask_rf_q;

    if (f3_regmask_store_all_qual_d) begin
      f8_mask_rf_wdata_d[f3_thread_id_i] = f3_regmask_wdata_sel_d;
    end

    if (f3_regmask_store_qual_d) begin
      for (int i = 0; i < VpuLaneNum; i++) begin
        if (f3_regmask_store_q[i]) begin
          f8_mask_rf_wdata_d[f3_thread_id_i][f3_regmask_waddr_q][i] = f3_regmask_wdata_sel_d[0][i];
        end
      end
    end

    if (|f8_regmask_store_q) begin
      for (int i = 0; i < VpuLaneNum; i++) begin
        if (f8_regmask_store_q[i]) begin
          f8_mask_rf_wdata_d[f8_thread_id_i][f8_regmask_waddr_q][i] = f8_regmask_comp_res_i[i];
        end
      end
    end

    f8_mask_rf_clk_en_d = (((|f3_regmask_store_q) && !f3_core_kill_i) ||
                           (f3_regmask_store_all_q && !f3_core_kill_i) ||
                           (|f8_regmask_store_q));
  end

  always_comb begin
    f8_toint_data_o = {{(XregSize - 5){1'b0}}, f8_toint_data_pre_q};
    if (f8_mova_x_m_q) begin
      f8_toint_data_o = ex_mask_rf_q[f8_thread_id_i];
    end
  end

  assign f8_regmask_store_o = f8_regmask_store_q;

  always_comb begin
    wb_mask_valid_o[0] = |f5_regmask_store_q;
    wb_mask_dest_o[0].addr = f5_regmask_waddr_q;
    wb_mask_dest_o[0].thread_id = f5_thread_id_i;

    wb_mask_valid_o[1] = |f6_regmask_store_q;
    wb_mask_dest_o[1].addr = f6_regmask_waddr_q;
    wb_mask_dest_o[1].thread_id = f6_thread_id_i;

    wb_mask_valid_o[2] = |f7_regmask_store_q;
    wb_mask_dest_o[2].addr = f7_regmask_waddr_q;
    wb_mask_dest_o[2].thread_id = f7_thread_id_i;

    wb_mask_valid_o[3] = |f8_regmask_store_q;
    wb_mask_dest_o[3].addr = f8_regmask_waddr_q;
    wb_mask_dest_o[3].thread_id = f8_thread_id_i;
  end

endmodule

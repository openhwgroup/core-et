// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module vpu_sh_sw
  import minion_pkg::*;
  import vpu_pkg::*;
(
  input  logic [$clog2(VpuLaneNum)-1:0] lane_id_i,
  input  logic                          clk_i,
  input  logic                          rst_ni,
  input  logic                          ex_in_valid_i,
  input  vpu_cmd_e                      ex_cmd_i,
  input  logic [VpuDataSSz-1:0]         ex_in1_i,
  input  logic [VpuDataSSz-1:0]         ex_in2_i,
  input  logic [VpuDataSSz-1:0]         ex_in3_i,
  input  logic [19:0]                   ex_imm_i,
  input  vpu_dtype_e                    ex_dtype_i,
  input  logic                          ex_mask_in1_i,
  input  logic [VpuDataSSz-1:0]         f2_swizz_in1_i,
  input  logic [VpuDataSSz-1:0]         f2_swizz_in2_i,
  input  logic [VpuDataSSz-1:0]         f2_swizz_in3_i,
  output logic [VpuDataSSz-1:0]         f2_fswizz_rdata_o,
  output vpu_output_t                   f3_out_bits_o
);

  localparam int unsigned VpuFbciLowSz   = 4;
  localparam int unsigned VpuFbciLow12Sz = 12;
  localparam int unsigned VpuFswizzSelSz = 2;

  logic [31:0]              gfx_out;
  logic [31:0]              f2_data;
  logic [VpuFlagsSz-1:0]    f2_flags;
  logic [31:0]              f3_data_q;
  logic [VpuFlagsSz-1:0]    f3_flags_q;
  logic [31:0]              in1_pip_sll;
  logic [31:0]              in2_pip_sll;
  logic [31:0]              in2_pip_sll1;
  logic [31:0]              in1_pip_feq;
  logic [31:0]              in2_pip_feq;
  logic signed [31:0]       in1_pip_srl_sra;
  logic signed [31:0]       in2_pip_srl_sra;
  logic signed [31:0]       in1_pip_flt;
  logic signed [31:0]       in2_pip_flt;
  logic signed [31:0]       in1_pip_min_max;
  logic signed [31:0]       in2_pip_min_max;
  logic [31:0]              unsigned_in1_pip_min_max;
  logic [31:0]              unsigned_in2_pip_min_max;
  logic [31:0]              in1_pip_and;
  logic [31:0]              in2_pip_and;
  logic [31:0]              in2_pip_and1;
  logic [31:0]              in1_pip_or;
  logic [31:0]              in2_pip_or;
  logic [31:0]              in1_pip_not;
  logic [31:0]              in1_pip_xor;
  logic [31:0]              in2_pip_xor;
  logic [31:0]              unsigned_in1_pip_flt;
  logic [31:0]              unsigned_in2_pip_flt;
  logic [9:0]               imm_pip_andi;
  logic [4:0]               imm_pip_slli;
  logic [19:0]              imm_pip_fbci;
  logic [31:0]              srl_sra_out;
  /* verilator lint_off UNUSEDSIGNAL */  // The 33-bit arithmetic-shift path matches the original; bit 32 is intentionally discarded after the shift.
  logic signed [32:0]       srl_sra_sign;
  /* verilator lint_on UNUSEDSIGNAL */
  logic [31:0]              sll_out;
  logic [31:0]              feq_out;
  logic [31:0]              fle_out;
  logic [31:0]              flt_out;
  logic [31:0]              fbci_out;
  logic signed [31:0]       min_max_sign;
  logic [31:0]              min_max_out;
  logic [31:0]              min_max_unsign;
  logic [31:0]              and_out;
  logic [31:0]              or_out;
  logic [31:0]              not_out;
  logic [31:0]              xor_out;
  logic [31:0]              fbci_ps_out;
  logic [31:0]              fcmov_out;
  logic [31:0]              fcmovm_out;
  logic [31:0]              mvxf_out;
  logic [31:0]              swizz_lane_0_4;
  logic [31:0]              swizz_lane_1_5;
  logic [31:0]              swizz_lane_2_6;
  logic [31:0]              swizz_lane_3_7;
  logic [31:0]              swizz_out;
  logic                     min_max_enable;
  logic                     and_enable;
  logic                     or_enable;
  logic                     not_enable;
  logic                     xor_enable;
  logic                     swizz_enable;
  logic                     packrepb_enable;
  logic                     packreph_enable;
  logic [31:0]              fsgnj_out;
  logic [31:0]              fsetm_out;
  logic [31:0]              packrepb_out;
  logic [31:0]              packreph_out;
  logic                     sll_enable;
  logic                     srl_sra_enable;
  logic                     feq_enable;
  logic                     fle_enable;
  logic                     flt_enable;
  logic                     fbci_enable;
  logic                     fsetm_enable;
  logic                     fbci_ps_enable;
  logic                     fcmov_enable;
  logic                     fcmovm_enable;
  logic                     is_mvxf;
  logic                     mvxf_enable;
  logic                     mvfx_enable;
  logic                     fsgnj_enable;
  logic                     fsat8_enable;
  logic                     fsatu8_enable;
  logic                     add_enable;
  logic                     cubeface_enable;
  logic                     cubeface_idx_enable;
  logic                     cubesgnsc_enable;
  logic                     cubesgntc_enable;
  logic [31:0]              cubeface_out;
  logic [31:0]              cubeface_idx_out;
  logic [31:0]              cubesgnsc_out;
  logic [31:0]              cubesgntc_out;
  logic [31:0]              fsat8_out;
  logic [31:0]              fsatu8_out;
  logic [31:0]              add_out;
  logic [VpuFlagsSz-1:0]    cubeface_idx_flag_out;
  logic [20:0]              gfx_out_valids_packed;
  logic [7:0]               out_valids_packed;
  logic                     gfx_out_valids_ored;
  logic                     out_valids_ored;
  logic                     extension;
  logic                     equal;
  logic                     less;
  logic [VpuFbciLowSz-1:0]  imm_ps_low_fbci;
  logic [VpuFbciLow12Sz-1:0] low12_ps_fbci;
  logic [VpuFbciLowSz-1:0]  imm_ps_low_fbci_inc;
  logic [19:0]              imm_ps_fbci;
  logic [31:0]              mvfx_out;
  logic signed [31:0]       in1_fsat8;
  logic [VpuFswizzSelSz-1:0] swizz_sel;
  logic                     gsgnj_sign;
  logic                     f2_in_valid_q;
  vpu_cmd_e                 f2_cmd_q;
  logic [VpuDataSSz-1:0]    f2_in1_q;
  logic [VpuDataSSz-1:0]    f2_in2_q;
  logic [VpuDataSSz-1:0]    f2_in3_q;
  logic [19:0]              f2_imm_q;
  vpu_dtype_e               f2_dtype_q;
  logic                     f2_mask_in1_q;
  logic [31:0]              add_in1;
  logic [31:0]              add_in3;
  logic [31:0]              add_cin;
  logic                     min_max_unsigned_op;
  logic                     max_op;

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      f2_in_valid_q <= 1'b0;
    end else begin
      f2_in_valid_q <= ex_in_valid_i;
    end
  end

  always_ff @(posedge clk_i) begin
    if (ex_in_valid_i) begin
      f2_cmd_q      <= ex_cmd_i;
      f2_in1_q      <= ex_in1_i;
      f2_in2_q      <= ex_in2_i;
      f2_in3_q      <= ex_in3_i;
      f2_imm_q      <= ex_imm_i;
      f2_dtype_q    <= ex_dtype_i;
      f2_mask_in1_q <= ex_mask_in1_i;
    end
  end

  assign sll_enable   = f2_in_valid_q && ((f2_cmd_q == VpuCmdSll) || (f2_cmd_q == VpuCmdSlli));
  assign in1_pip_sll  = f2_in1_q;
  assign in2_pip_sll  = f2_in2_q;
  assign imm_pip_slli = f2_imm_q[4:0];
  assign in2_pip_sll1 = (f2_cmd_q == VpuCmdSlli) ? {27'b0, imm_pip_slli} : in2_pip_sll;
  assign sll_out      = in1_pip_sll << in2_pip_sll1;

  assign srl_sra_enable = f2_in_valid_q &&
      ((f2_cmd_q == VpuCmdSrl) || (f2_cmd_q == VpuCmdSra) ||
       (f2_cmd_q == VpuCmdSrli) || (f2_cmd_q == VpuCmdSrai));
  assign in1_pip_srl_sra = $signed(f2_in1_q);
  assign in2_pip_srl_sra =
      ((f2_cmd_q == VpuCmdSrl) || (f2_cmd_q == VpuCmdSra)) ? $signed(f2_in2_q) :
      $signed({27'b0, f2_imm_q[4:0]});
  assign extension   = ((f2_cmd_q == VpuCmdSra) || (f2_cmd_q == VpuCmdSrai)) ? in1_pip_srl_sra[31] : 1'b0;
  assign srl_sra_sign = $signed({extension, in1_pip_srl_sra}) >>> in2_pip_srl_sra;
  assign srl_sra_out  = srl_sra_sign[31:0];

  assign feq_enable  = f2_in_valid_q && (f2_cmd_q == VpuCmdFeq);
  assign in1_pip_feq = f2_in1_q;
  assign in2_pip_feq = f2_in2_q;
  assign equal       = in1_pip_feq == in2_pip_feq;
  assign feq_out     = equal ? 32'hffff_ffff : 32'b0;

  assign flt_enable = f2_in_valid_q && ((f2_cmd_q == VpuCmdFlt) || (f2_cmd_q == VpuCmdFltu));
  assign in1_pip_flt = $signed(f2_in1_q);
  assign in2_pip_flt = $signed(f2_in2_q);
  assign unsigned_in1_pip_flt = f2_in1_q;
  assign unsigned_in2_pip_flt = f2_in2_q;
  assign less = (f2_cmd_q == VpuCmdFltu) ? (unsigned_in1_pip_flt < unsigned_in2_pip_flt) :
                                           (in1_pip_flt < in2_pip_flt);
  assign flt_out = less ? 32'hffff_ffff : 32'b0;

  assign fle_enable = f2_in_valid_q && (f2_cmd_q == VpuCmdFle);
  assign fle_out    = (equal || less) ? 32'hffff_ffff : 32'b0;

  assign fbci_enable  = f2_in_valid_q && (f2_cmd_q == VpuCmdFbci) && (f2_dtype_q == VpuDtypeInt);
  assign imm_pip_fbci = f2_imm_q[19:0];
  assign fbci_out     = {{12{imm_pip_fbci[19]}}, imm_pip_fbci};

  assign min_max_enable = f2_in_valid_q &&
      ((f2_cmd_q == VpuCmdMin) || (f2_cmd_q == VpuCmdMax) ||
       (f2_cmd_q == VpuCmdMinu) || (f2_cmd_q == VpuCmdMaxu));
  assign min_max_unsigned_op = f2_in_valid_q && ((f2_cmd_q == VpuCmdMinu) || (f2_cmd_q == VpuCmdMaxu));
  assign max_op = f2_in_valid_q && ((f2_cmd_q == VpuCmdMax) || (f2_cmd_q == VpuCmdMaxu));
  assign in1_pip_min_max = $signed(f2_in1_q);
  assign in2_pip_min_max = $signed(f2_in2_q);
  assign unsigned_in1_pip_min_max = f2_in1_q;
  assign unsigned_in2_pip_min_max = f2_in2_q;
  assign min_max_unsign = ((unsigned_in1_pip_min_max > unsigned_in2_pip_min_max) == max_op) ?
      unsigned_in1_pip_min_max : unsigned_in2_pip_min_max;
  assign min_max_sign = ((in1_pip_min_max > in2_pip_min_max) == max_op) ?
      in1_pip_min_max : in2_pip_min_max;
  assign min_max_out = min_max_unsigned_op ? min_max_unsign : $unsigned(min_max_sign);

  assign and_enable   = f2_in_valid_q && ((f2_cmd_q == VpuCmdAnd) || (f2_cmd_q == VpuCmdAndi));
  assign in1_pip_and  = f2_in1_q;
  assign in2_pip_and  = f2_in2_q;
  assign imm_pip_andi = f2_imm_q[9:0];
  assign in2_pip_and1 = (f2_cmd_q == VpuCmdAndi) ? {{22{imm_pip_andi[9]}}, imm_pip_andi} : in2_pip_and;
  assign and_out      = in1_pip_and & in2_pip_and1;

  assign or_enable  = f2_in_valid_q && (f2_cmd_q == VpuCmdOr);
  assign in1_pip_or = f2_in1_q;
  assign in2_pip_or = f2_in2_q;
  assign or_out     = in1_pip_or | in2_pip_or;

  assign xor_enable  = f2_in_valid_q && (f2_cmd_q == VpuCmdXor);
  assign in1_pip_xor = f2_in1_q;
  assign in2_pip_xor = f2_in2_q;
  assign xor_out     = in1_pip_xor ^ in2_pip_xor;

  assign not_enable  = f2_in_valid_q && (f2_cmd_q == VpuCmdNot);
  assign in1_pip_not = f2_in1_q;
  assign not_out     = ~in1_pip_not;

  assign fcmov_enable  = f2_in_valid_q && (f2_cmd_q == VpuCmdFcmov);
  assign fcmov_out     = (f2_in1_q == '0) ? f2_in3_q : f2_in2_q;

  assign fcmovm_enable = f2_in_valid_q && (f2_cmd_q == VpuCmdFcmovm);
  assign fcmovm_out    = f2_mask_in1_q ? f2_in3_q : f2_in2_q;

  assign swizz_enable    = f2_in_valid_q && (f2_cmd_q == VpuCmdFswizz);
  assign packrepb_enable = f2_in_valid_q && (f2_cmd_q == VpuCmdPackrepb);
  assign packreph_enable = f2_in_valid_q && (f2_cmd_q == VpuCmdPackreph);
  assign swizz_sel = ((lane_id_i == 3'd0) || (lane_id_i == 3'd4)) ? f2_imm_q[1:0] :
                     ((lane_id_i == 3'd1) || (lane_id_i == 3'd5)) ? f2_imm_q[3:2] :
                     ((lane_id_i == 3'd2) || (lane_id_i == 3'd6)) ? f2_imm_q[5:4] :
                                                                     f2_imm_q[7:6];

  assign swizz_lane_0_4 = (swizz_sel == 2'd0) ? f2_in1_q :
                          (swizz_sel == 2'd1) ? f2_swizz_in1_i :
                          (swizz_sel == 2'd2) ? f2_swizz_in2_i :
                                                f2_swizz_in3_i;

  assign swizz_lane_1_5 = (swizz_sel == 2'd0) ? f2_swizz_in1_i :
                          (swizz_sel == 2'd1) ? f2_in1_q :
                          (swizz_sel == 2'd2) ? f2_swizz_in2_i :
                                                f2_swizz_in3_i;

  assign swizz_lane_2_6 = (swizz_sel == 2'd0) ? f2_swizz_in1_i :
                          (swizz_sel == 2'd1) ? f2_swizz_in2_i :
                          (swizz_sel == 2'd2) ? f2_in1_q :
                                                f2_swizz_in3_i;

  assign swizz_lane_3_7 = (swizz_sel == 2'd0) ? f2_swizz_in1_i :
                          (swizz_sel == 2'd1) ? f2_swizz_in2_i :
                          (swizz_sel == 2'd2) ? f2_swizz_in3_i :
                                                f2_in1_q;

  assign swizz_out = ((lane_id_i == 3'd0) || (lane_id_i == 3'd4)) ? swizz_lane_0_4 :
                     ((lane_id_i == 3'd1) || (lane_id_i == 3'd5)) ? swizz_lane_1_5 :
                     ((lane_id_i == 3'd2) || (lane_id_i == 3'd6)) ? swizz_lane_2_6 :
                                                                     swizz_lane_3_7;

  assign packrepb_out = f2_swizz_in1_i;
  assign packreph_out = f2_swizz_in1_i;

  assign fsetm_enable = f2_in_valid_q && (f2_cmd_q == VpuCmdFsetm);
  assign fsetm_out    = (f2_in1_q == '0) ? 32'b0 : 32'b1;

  assign cubeface_enable = f2_in_valid_q && (f2_cmd_q == VpuCmdCubeface);
  assign cubeface_out    = f2_in3_q[0] ? (f2_in2_q[0] ? 32'b0 : 32'd1) :
                                         (f2_in1_q[0] ? 32'b0 : 32'd2);

  assign cubeface_idx_enable = f2_in_valid_q && (f2_cmd_q == VpuCmdCubefaceIdx);

  always_comb begin
    cubeface_idx_out = '0;
    cubeface_idx_flag_out = '0;

    case ({f2_in2_q[31], f2_in1_q[1:0]})
      3'b000: cubeface_idx_out = 32'h0000_0000;
      3'b001: cubeface_idx_out = 32'h4000_0000;
      3'b010: cubeface_idx_out = 32'h4080_0000;
      3'b100: cubeface_idx_out = 32'h3f80_0000;
      3'b101: cubeface_idx_out = 32'h4040_0000;
      3'b110: cubeface_idx_out = 32'h40a0_0000;
      3'b011, 3'b111: begin
        cubeface_idx_out = Float32PosCqnan;
        cubeface_idx_flag_out[VpuFlagInvalid] = 1'b1;
      end
      default: begin
      end
    endcase
  end

  assign cubesgnsc_enable = f2_in_valid_q && (f2_cmd_q == VpuCmdCubesgnsc);
  assign cubesgnsc_out    = {(f2_in1_q[2:0] == 3'd0) || (f2_in1_q[2:0] == 3'd5), f2_in2_q[30:0]};

  assign cubesgntc_enable = f2_in_valid_q && (f2_cmd_q == VpuCmdCubesgntc);
  assign cubesgntc_out    = {(f2_in1_q[2:0] != 3'd2), f2_in2_q[30:0]};

  assign gfx_out_valids_packed = {
    packrepb_enable, cubeface_enable, cubeface_idx_enable, cubesgnsc_enable, cubesgntc_enable,
    packreph_enable, fsetm_enable, swizz_enable, fcmovm_enable, fcmov_enable, sll_enable,
    srl_sra_enable, feq_enable, fle_enable, flt_enable, fbci_enable, min_max_enable,
    and_enable, or_enable, not_enable, xor_enable
  };
  assign gfx_out_valids_ored = |gfx_out_valids_packed;

  assign fbci_ps_enable  = f2_in_valid_q && (f2_cmd_q == VpuCmdFbci) && (f2_dtype_q == VpuDtypeF32);
  assign imm_ps_fbci     = f2_imm_q[19:0];
  assign imm_ps_low_fbci = f2_imm_q[3:0];
  assign imm_ps_low_fbci_inc = imm_ps_low_fbci + 4'd1;
  assign low12_ps_fbci   = (imm_ps_low_fbci < 4'd8) ?
      {imm_ps_low_fbci, imm_ps_low_fbci, imm_ps_low_fbci} :
      {imm_ps_low_fbci, imm_ps_low_fbci, imm_ps_low_fbci_inc};
  assign fbci_ps_out     = {imm_ps_fbci, low12_ps_fbci};

  assign mvfx_enable = f2_in_valid_q &&
      ((f2_cmd_q == VpuCmdMvsFx) || (f2_cmd_q == VpuCmdMvzFx) || (f2_cmd_q == VpuCmdFbc));
  assign mvfx_out = f2_in1_q;

  always_comb begin
    case (f2_cmd_q)
      VpuCmdMvXf, VpuTransLogFma1, VpuTransLogFma2, VpuTransLogMul: is_mvxf = 1'b1;
      default: is_mvxf = 1'b0;
    endcase
  end

  assign mvxf_enable = f2_in_valid_q && is_mvxf;
  assign mvxf_out    = (f2_cmd_q == VpuCmdMvXf) ? f2_in1_q : f2_in3_q;

  assign fsgnj_enable = f2_in_valid_q &&
      ((f2_cmd_q == VpuCmdFsgnj) || (f2_cmd_q == VpuCmdFsgnjn) || (f2_cmd_q == VpuCmdFsgnjx));
  assign gsgnj_sign   = (f2_cmd_q == VpuCmdFsgnj)  ? f2_in2_q[31] :
                        (f2_cmd_q == VpuCmdFsgnjn) ? ~f2_in2_q[31] :
                                                     (f2_in1_q[31] ^ f2_in2_q[31]);
  assign fsgnj_out    = {gsgnj_sign, f2_in1_q[30:0]};

  assign fsat8_enable = f2_in_valid_q && (f2_cmd_q == VpuCmdFsat8);
  assign in1_fsat8    = $signed(f2_in1_q);
  assign fsat8_out    = (in1_fsat8 > 32'sd127)  ? 32'h0000_007f :
                        (in1_fsat8 < -32'sd128) ? 32'h0000_0080 :
                                                  {24'b0, f2_in1_q[7:0]};
  assign fsatu8_enable = f2_in_valid_q && (f2_cmd_q == VpuCmdFsatu8);
  assign fsatu8_out    = (in1_fsat8 > 32'sd255) ? 32'h0000_00ff :
                         (in1_fsat8 < 32'sd0)   ? 32'h0000_0000 :
                                                   {24'b0, f2_in1_q[7:0]};

  assign add_enable = f2_in_valid_q &&
      ((f2_cmd_q == VpuCmdAdd) || (f2_cmd_q == VpuCmdAddi) || (f2_cmd_q == VpuCmdSub));
  assign add_in1 = f2_in1_q & {32{add_enable}};
  assign add_in3 = (f2_cmd_q == VpuCmdAddi) ? ({{22{f2_imm_q[9]}}, f2_imm_q[9:0]} & {32{add_enable}}) :
                  ((f2_in3_q ^ {32{(f2_cmd_q == VpuCmdSub)}}) & {32{add_enable}});
  assign add_cin = {31'b0, (f2_cmd_q == VpuCmdSub)};
  assign add_out = add_in1 + add_in3 + add_cin;

  assign out_valids_packed = {
    gfx_out_valids_ored, mvfx_enable, fbci_ps_enable, mvxf_enable,
    fsgnj_enable, fsat8_enable, fsatu8_enable, add_enable
  };
  assign out_valids_ored = |out_valids_packed;

  always_comb begin
    gfx_out = 32'b0;

    if (gfx_out_valids_ored) begin
      case (gfx_out_valids_packed)
        21'b100000000000000000000: gfx_out = packrepb_out;
        21'b010000000000000000000: gfx_out = cubeface_out;
        21'b001000000000000000000: gfx_out = cubeface_idx_out;
        21'b000100000000000000000: gfx_out = cubesgnsc_out;
        21'b000010000000000000000: gfx_out = cubesgntc_out;
        21'b000001000000000000000: gfx_out = packreph_out;
        21'b000000100000000000000: gfx_out = fsetm_out;
        21'b000000010000000000000: gfx_out = swizz_out;
        21'b000000001000000000000: gfx_out = fcmovm_out;
        21'b000000000100000000000: gfx_out = fcmov_out;
        21'b000000000010000000000: gfx_out = sll_out;
        21'b000000000001000000000: gfx_out = srl_sra_out;
        21'b000000000000100000000: gfx_out = feq_out;
        21'b000000000000010000000: gfx_out = fle_out;
        21'b000000000000001000000: gfx_out = flt_out;
        21'b000000000000000100000: gfx_out = fbci_out;
        21'b000000000000000010000: gfx_out = min_max_out;
        21'b000000000000000001000: gfx_out = and_out;
        21'b000000000000000000100: gfx_out = or_out;
        21'b000000000000000000010: gfx_out = not_out;
        21'b000000000000000000001: gfx_out = xor_out;
        default: begin
        end
      endcase
    end
  end

  always_comb begin
    f2_data = 32'b0;

    if (out_valids_ored) begin
      case (out_valids_packed)
        8'b10000000: f2_data = gfx_out;
        8'b01000000: f2_data = mvfx_out;
        8'b00100000: f2_data = fbci_ps_out;
        8'b00010000: f2_data = mvxf_out;
        8'b00001000: f2_data = fsgnj_out;
        8'b00000100: f2_data = fsat8_out;
        8'b00000010: f2_data = fsatu8_out;
        8'b00000001: f2_data = add_out;
        default: f2_data = 32'b0;
      endcase
    end
  end

  assign f2_flags = cubeface_idx_flag_out & {VpuFlagsSz{cubeface_idx_enable}};
  assign f2_fswizz_rdata_o = f2_in1_q;

  always_ff @(posedge clk_i) begin
    if (f2_in_valid_q) begin
      f3_data_q  <= f2_data;
      f3_flags_q <= f2_flags;
    end
  end

  assign f3_out_bits_o.data = f3_data_q;
  assign f3_out_bits_o.exc  = f3_flags_q;

endmodule

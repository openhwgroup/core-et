// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module vpu_sh_sw (
  input  [$clog2(`VPU_LANE_NUM)-1:0]   lane_id,
  input                                clock,
  input                                reset,
  input                                ex_in_valid,
  input  [`VPU_FCMD_SZ-1:0]            ex_cmd,
  input  [`VPU_DATA_S_SZ-1:0]          ex_in1,
  input  [`VPU_DATA_S_SZ-1:0]          ex_in2,
  input  [`VPU_DATA_S_SZ-1:0]          ex_in3,
  input  [19:0]                        ex_imm,
  input  [`VPU_CTRL_SIGS_DTYPE_SZ-1:0] ex_dtype,
  input                                ex_mask_in1,
  input  [`VPU_DATA_S_SZ-1:0]          f2_swizz_in1,
  input  [`VPU_DATA_S_SZ-1:0]          f2_swizz_in2,
  input  [`VPU_DATA_S_SZ-1:0]          f2_swizz_in3,
  output logic [`VPU_DATA_S_SZ-1:0]    f2_fswizz_rdata,
  output vpu_output_t                  f3_out_bits
);

  // internal logic
  logic [31:0]                        gfx_out, f2_data, f3_data;
  logic [`VPU_FLAGS_SZ-1:0]           f2_flags, f3_flags;
  logic [31:0]                        in1_pip_sll, in2_pip_sll, in2_pip_sll1;
  logic [31:0]                        in1_pip_feq, in2_pip_feq;
  logic signed [31:0]                 in1_pip_srl_sra, in2_pip_srl_sra;
  logic signed [31:0]                 in1_pip_flt, in2_pip_flt;
  logic signed [31:0]                 in1_pip_min_max, in2_pip_min_max;
  logic [31:0]                        unsigned_in1_pip_min_max, unsigned_in2_pip_min_max;
  logic [31:0]                        in1_pip_and, in2_pip_and, in2_pip_and1, in1_pip_or, in2_pip_or;
  logic [31:0]                        in1_pip_not, in1_pip_xor, in2_pip_xor;
  logic [31:0]                        unsigned_in1_pip_flt, unsigned_in2_pip_flt;
  logic [9:0]                         imm_pip_andi;
  logic [4:0]                         imm_pip_slli;
  logic [19:0]                        imm_pip_fbci;
  logic [31:0]                        srl_sra_out;
  logic signed [32:0]                 srl_sra_sign;
  logic [31:0]                        sll_out, feq_out, fle_out, flt_out, fbci_out;
  logic signed [31:0]                 min_max_sign;
  logic [31:0]                        min_max_out, min_max_unsign, and_out, or_out, not_out, xor_out;
  logic [31:0]                        fbci_ps_out, fcmov_out, fcmovm_out, mvxf_out;
  logic [31:0]                        swizz_lane_0_4, swizz_lane_1_5, swizz_lane_2_6, swizz_lane_3_7, swizz_out;
  logic                               min_max_enable, and_enable, or_enable, not_enable, xor_enable, swizz_enable, packrepb_enable, packreph_enable;
  logic [31:0]                        fsgnj_out, fsetm_out, packrepb_out, packreph_out;
  logic                               sll_enable, srl_sra_enable, feq_enable, fle_enable, flt_enable, fbci_enable, fsetm_enable;
  logic                               fbci_ps_enable, fcmov_enable, fcmovm_enable, is_mvxf, mvxf_enable, mvfx_enable, fsgnj_enable, fsat8_enable, fsatu8_enable, add_enable;
  logic                               cubeface_enable, cubeface_idx_enable, cubesgnsc_enable, cubesgntc_enable;
  logic [31:0]                        cubeface_out, cubeface_idx_out, cubesgnsc_out, cubesgntc_out, fsat8_out, fsatu8_out, add_out;
  logic [`VPU_FLAGS_SZ-1:0]           cubeface_idx_flag_out;
  logic [20:0]                        gfx_out_valids_packed;
  logic [7:0]                         out_valids_packed;
  logic                               gfx_out_valids_ORed;
  logic                               out_valids_ORed;
  logic                               extension;
  logic                               equal, less;
  logic [`VPU_FBCI_LOW_SZ-1:0]        imm_ps_low_fbci;
  logic [`VPU_FBCI_LOW12_SZ-1:0]      low12_ps_fbci;
  logic [19:0]                        imm_ps_fbci;
  logic [31:0]                        mvfx_out;
  logic signed [31:0]                 in1fsat8;
  logic [`VPU_FSWIZZ_SEL-1:0]         swizz_sel;
  logic                               gsgnj_sign;
  logic                               f2_in_valid;
  logic [`VPU_FCMD_SZ-1:0]            f2_cmd;
  logic [`VPU_DATA_S_SZ-1:0]          f2_in1;
  logic [`VPU_DATA_S_SZ-1:0]          f2_in2;
  logic [`VPU_DATA_S_SZ-1:0]          f2_in3;
  logic [19:0]                        f2_imm;
  logic [`VPU_CTRL_SIGS_DTYPE_SZ-1:0] f2_dtype;
  logic                               f2_mask_in1;
  logic [31:0]                        add_in1, add_in3, add_cin;
  logic                               min_max_unsigned_op, max_op;


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// F2 Stage
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

//      CLK    RST    DOUT         DIN          DEF
`RST_FF(clock, reset, f2_in_valid, ex_in_valid, 1'b0)

//      CLK    EN           DOUT         DIN
`EN_FF (clock, ex_in_valid, f2_cmd,      ex_cmd)
`EN_FF (clock, ex_in_valid, f2_in1,      ex_in1)
`EN_FF (clock, ex_in_valid, f2_in2,      ex_in2)
`EN_FF (clock, ex_in_valid, f2_in3,      ex_in3)
`EN_FF (clock, ex_in_valid, f2_imm,      ex_imm)
`EN_FF (clock, ex_in_valid, f2_dtype,    ex_dtype)
`EN_FF (clock, ex_in_valid, f2_mask_in1, ex_mask_in1)


  //graphics instructions
//`ifdef GFX

  // SLL / SLLI
  assign sll_enable = f2_in_valid & ((f2_cmd == `VPU_FCMD_SLL) || (f2_cmd == `VPU_FCMD_SLLI));
  assign in1_pip_sll = f2_in1;
  assign in2_pip_sll = f2_in2;
  assign imm_pip_slli = f2_imm[4:0];
  assign in2_pip_sll1 = (f2_cmd == `VPU_FCMD_SLLI) ? {27'b0, imm_pip_slli} : in2_pip_sll;
  assign sll_out = in1_pip_sll << in2_pip_sll1;

  // SRL / SRA
  assign srl_sra_enable = f2_in_valid & ((f2_cmd == `VPU_FCMD_SRL) || (f2_cmd == `VPU_FCMD_SRA) || (f2_cmd == `VPU_FCMD_SRLI) || (f2_cmd == `VPU_FCMD_SRAI));
  assign in1_pip_srl_sra = $signed(f2_in1);
  assign in2_pip_srl_sra = (f2_cmd == `VPU_FCMD_SRL) || (f2_cmd == `VPU_FCMD_SRA) ? $signed(f2_in2) : $signed({27'b0, f2_imm[4:0]});
  assign extension = ((f2_cmd == `VPU_FCMD_SRA) || (f2_cmd == `VPU_FCMD_SRAI)) ? in1_pip_srl_sra[31] : 0;
  assign srl_sra_sign = ($signed({extension, in1_pip_srl_sra}) >>> in2_pip_srl_sra);
  assign srl_sra_out  = srl_sra_sign[31:0];

  // FEQ / FLE
  assign feq_enable = f2_in_valid & (f2_cmd == `VPU_FCMD_FEQ);
  assign in1_pip_feq = f2_in1;
  assign in2_pip_feq = f2_in2;
  assign equal = in1_pip_feq == in2_pip_feq;
  assign feq_out = equal ? 32'hffffffff : 0;

  // FLT / FLE
  assign flt_enable = f2_in_valid & ((f2_cmd == `VPU_FCMD_FLT) | (f2_cmd == `VPU_FCMD_FLTU));
  assign in1_pip_flt = $signed(f2_in1);
  assign in2_pip_flt = $signed(f2_in2);
  assign unsigned_in1_pip_flt = f2_in1;
  assign unsigned_in2_pip_flt = f2_in2;
  assign less = (f2_cmd == `VPU_FCMD_FLTU) ? (unsigned_in1_pip_flt < unsigned_in2_pip_flt) : in1_pip_flt < in2_pip_flt;
  assign flt_out = less ? 32'hffffffff : 0;

  // FLE
  assign fle_enable = f2_in_valid & (f2_cmd == `VPU_FCMD_FLE);
  assign fle_out = (equal | less) ? 32'hffffffff : 0;

  // FBCI PI
  assign fbci_enable = f2_in_valid && (f2_cmd == `VPU_FCMD_FBCI) && (f2_dtype == `VPU_DTYPE_INT);
  assign imm_pip_fbci = f2_imm[19:0];
  assign fbci_out = `SX(32, imm_pip_fbci);

  // MIN / MAX
  assign min_max_enable = f2_in_valid && (f2_cmd == `VPU_FCMD_MIN || f2_cmd == `VPU_FCMD_MAX || f2_cmd == `VPU_FCMD_MINU || f2_cmd == `VPU_FCMD_MAXU);
  assign min_max_unsigned_op = f2_in_valid && (f2_cmd == `VPU_FCMD_MINU || f2_cmd == `VPU_FCMD_MAXU);
  assign max_op = f2_in_valid && (f2_cmd == `VPU_FCMD_MAX || f2_cmd == `VPU_FCMD_MAXU);
  assign in1_pip_min_max = $signed(f2_in1);
  assign in2_pip_min_max = $signed(f2_in2);
  assign unsigned_in1_pip_min_max = f2_in1;
  assign unsigned_in2_pip_min_max = f2_in2;
  assign min_max_unsign = ((unsigned_in1_pip_min_max > unsigned_in2_pip_min_max) == max_op ? unsigned_in1_pip_min_max : unsigned_in2_pip_min_max);
  assign min_max_sign   = ((in1_pip_min_max > in2_pip_min_max) == max_op ? in1_pip_min_max : in2_pip_min_max);
  assign min_max_out  = min_max_unsigned_op ? min_max_unsign
                      :                       $unsigned(min_max_sign);

  // AND / ANDI
  assign and_enable = f2_in_valid & ((f2_cmd == `VPU_FCMD_AND) || (f2_cmd == `VPU_FCMD_ANDI));
  assign in1_pip_and = f2_in1;
  assign in2_pip_and = f2_in2;
  assign imm_pip_andi = f2_imm[9:0];
  assign in2_pip_and1 = (f2_cmd == `VPU_FCMD_ANDI) ? `SX(32, imm_pip_andi) : in2_pip_and;
  assign and_out = in1_pip_and & in2_pip_and1;

  // OR
  assign or_enable = f2_in_valid & (f2_cmd == `VPU_FCMD_OR);
  assign in1_pip_or = f2_in1;
  assign in2_pip_or = f2_in2;
  assign or_out = in1_pip_or | in2_pip_or;

  // XOR
  assign xor_enable = f2_in_valid & (f2_cmd == `VPU_FCMD_XOR);
  assign in1_pip_xor = f2_in1;
  assign in2_pip_xor = f2_in2;
  assign xor_out = in1_pip_xor ^ in2_pip_xor;

  // NOT
  assign not_enable = f2_in_valid & (f2_cmd == `VPU_FCMD_NOT);
  assign in1_pip_not = f2_in1;
  assign not_out = ~in1_pip_not;

  // FCMOV
  assign fcmov_enable = f2_in_valid && (f2_cmd == `VPU_FCMD_FCMOV);
  assign fcmov_out    = (f2_in1 == 0) ? f2_in3 : f2_in2;

  // FCMOVM
  assign fcmovm_enable = f2_in_valid && (f2_cmd == `VPU_FCMD_FCMOVM);
  assign fcmovm_out    = f2_mask_in1 ? f2_in3 : f2_in2;

  // SWIZZ
  assign swizz_enable = f2_in_valid && (f2_cmd == `VPU_FCMD_FSWIZZ);
  assign packrepb_enable = f2_in_valid && (f2_cmd == `VPU_FCMD_PACKREPB);
  assign packreph_enable = f2_in_valid && (f2_cmd == `VPU_FCMD_PACKREPH);
  assign swizz_sel = ((lane_id == 0) || (lane_id == 4)) ? (f2_imm) & `VPU_FSWIZZ_SEL'd3 :
                     ((lane_id == 1) || (lane_id == 5)) ? (f2_imm >> 2) & `VPU_FSWIZZ_SEL'd3 :
                     ((lane_id == 2) || (lane_id == 6)) ? (f2_imm >> 4) & `VPU_FSWIZZ_SEL'd3 :
                                                          (f2_imm >> 6) & `VPU_FSWIZZ_SEL'd3;

  assign swizz_lane_0_4 = ((swizz_sel == 0) ? f2_in1 :
                           (swizz_sel == 1) ? f2_swizz_in1 :
                           (swizz_sel == 2) ? f2_swizz_in2 :
                                              f2_swizz_in3);

  assign swizz_lane_1_5 = ((swizz_sel == 0) ? f2_swizz_in1 :
                           (swizz_sel == 1) ? f2_in1 :
                           (swizz_sel == 2) ? f2_swizz_in2 :
                                              f2_swizz_in3);

  assign swizz_lane_2_6 = ((swizz_sel == 0) ? f2_swizz_in1 :
                           (swizz_sel == 1) ? f2_swizz_in2 :
                           (swizz_sel == 2) ? f2_in1 :
                                              f2_swizz_in3);

  assign swizz_lane_3_7 = ((swizz_sel == 0) ? f2_swizz_in1 :
                           (swizz_sel == 1) ? f2_swizz_in2 :
                           (swizz_sel == 2) ? f2_swizz_in3 :
                                              f2_in1);

  assign swizz_out = ((lane_id == 0) || (lane_id == 4)) ? swizz_lane_0_4 :
                     ((lane_id == 1) || (lane_id == 5)) ? swizz_lane_1_5 :
                     ((lane_id == 2) || (lane_id == 6)) ? swizz_lane_2_6 :
                                                          swizz_lane_3_7;

  // muxes done at vpu_ctrl
  assign packrepb_out = f2_swizz_in1;
  assign packreph_out = f2_swizz_in1;

  // FSETM
  assign fsetm_enable = f2_in_valid && (f2_cmd == `VPU_FCMD_FSETM);
  assign fsetm_out    = (f2_in1 == 0) ? 32'b0 : 32'b1;

  // CUBEFACE
  assign cubeface_enable = f2_in_valid & (f2_cmd == `VPU_FCMD_CUBEFACE);
  assign cubeface_out    = (f2_in3[0]) ? ((f2_in2[0]) ? 32'b0 : 32'd1) :
                                         ((f2_in1[0]) ? 32'b0 : 32'd2);

  // CUBEFACE_IDX
  assign cubeface_idx_enable = f2_in_valid & (f2_cmd == `VPU_FCMD_CUBEFACE_IDX);

  always_comb begin
    cubeface_idx_out     = '0;
    cubeface_idx_flag_out = '0;

    // (f2_in2 < 0) ? float32(f2_in1[2:0] * 2 + 1) : float32(f2_in1[2:0] * 2)
    case ({f2_in2[31], f2_in1[1:0]})
      3'b000:        cubeface_idx_out = 32'h00000000;
      3'b001:        cubeface_idx_out = 32'h40000000;
      3'b010:        cubeface_idx_out = 32'h40800000;
      3'b100:        cubeface_idx_out = 32'h3f800000;
      3'b101:        cubeface_idx_out = 32'h40400000;
      3'b110:        cubeface_idx_out = 32'h40a00000;
      3'b011, 3'b111: begin
        cubeface_idx_out = `FLOAT32_POS_CQNAN;
        cubeface_idx_flag_out[`VPU_FLAG_INVALID] = 1'b1;
      end
    endcase
  end

  // CUBESGNSC
  assign cubesgnsc_enable = f2_in_valid & (f2_cmd == `VPU_FCMD_CUBESGNSC);
  assign cubesgnsc_out    = {(f2_in1[2:0] == 0 || f2_in1[2:0] == 5), f2_in2[30:0]};

  // CUBESGNTC
  assign cubesgntc_enable = f2_in_valid & (f2_cmd == `VPU_FCMD_CUBESGNTC);
  assign cubesgntc_out    = {(f2_in1[2:0] != 2), f2_in2[30:0]};

  // outputs
  assign gfx_out_valids_packed = {packrepb_enable, cubeface_enable, cubeface_idx_enable, cubesgnsc_enable, cubesgntc_enable,
                                  packreph_enable, fsetm_enable, swizz_enable,
                                  fcmovm_enable, fcmov_enable, sll_enable,    srl_sra_enable,
                                  feq_enable,  fle_enable,  flt_enable,  fbci_enable,
                                  min_max_enable,  and_enable,  or_enable,   not_enable,  xor_enable};

  assign gfx_out_valids_ORed   = |gfx_out_valids_packed;

  // FBCI PS
  assign fbci_ps_enable = f2_in_valid && (f2_cmd == `VPU_FCMD_FBCI) && (f2_dtype == `VPU_DTYPE_F32);
  assign imm_ps_fbci    = f2_imm[19:0];
  assign imm_ps_low_fbci = f2_imm[3:0];
  assign low12_ps_fbci  = imm_ps_low_fbci < 8 ? ((imm_ps_low_fbci << 8) | (imm_ps_low_fbci << 4) | imm_ps_low_fbci) :
                                                ((imm_ps_low_fbci << 8) | (imm_ps_low_fbci << 4) | (imm_ps_low_fbci + 1));
  assign fbci_ps_out    = {imm_ps_fbci, low12_ps_fbci};

  // MVS_FX/MVZ_FX
  assign mvfx_enable = f2_in_valid && ((f2_cmd == `VPU_FCMD_MVS_FX) | (f2_cmd == `VPU_FCMD_MVZ_FX) | (f2_cmd == `VPU_FCMD_FBC));
  assign mvfx_out    = f2_in1;

  // MVS_XF
  always_comb begin
    case (f2_cmd)
      `VPU_FCMD_MV_XF:     is_mvxf = `Y;
      `VPU_TRANS_LOG_FMA1: is_mvxf = `Y;
      `VPU_TRANS_LOG_FMA2: is_mvxf = `Y;
      `VPU_TRANS_LOG_MUL:  is_mvxf = `Y;
      default:             is_mvxf = `N;
    endcase
  end

  assign mvxf_enable = f2_in_valid && is_mvxf;
  assign mvxf_out    = (f2_cmd == `VPU_FCMD_MV_XF) ? f2_in1 : f2_in3;

  // FSGNJ/FSGNJN/FSGNX
  assign fsgnj_enable = f2_in_valid && (f2_cmd == `VPU_FCMD_FSGNJ  || f2_cmd == `VPU_FCMD_FSGNJN || f2_cmd == `VPU_FCMD_FSGNJX);
  assign gsgnj_sign   = (f2_cmd == `VPU_FCMD_FSGNJ)  ? f2_in2[31] :
                        (f2_cmd == `VPU_FCMD_FSGNJN) ? ~f2_in2[31] :
                                                       (f2_in1[31] ^ f2_in2[31]);
  assign fsgnj_out    = {gsgnj_sign, f2_in1[30:0]};

  // FSAT8
  assign fsat8_enable = f2_in_valid && (f2_cmd == `VPU_FCMD_FSAT8);
  assign in1fsat8     = $signed(f2_in1);
  assign fsat8_out    = (in1fsat8 > 127)  ? 32'h7f :
                        (in1fsat8 < -128) ? 32'h80 :
                                            {24'b0, f2_in1[7:0]};
  assign fsatu8_enable = f2_in_valid && (f2_cmd == `VPU_FCMD_FSATU8);
  assign fsatu8_out    = (in1fsat8 > 255)  ? 32'hff :
                         (in1fsat8 < 0)    ? 32'h0 :
                                             {24'b0, f2_in1[7:0]};

  // ADD/ADDI/SUB
  assign add_enable = f2_in_valid && ((f2_cmd == `VPU_FCMD_ADD) | (f2_cmd == `VPU_FCMD_ADDI) | (f2_cmd == `VPU_FCMD_SUB));
  assign add_in1 = f2_in1 & {32{add_enable}};
  assign add_in3 = (f2_cmd == `VPU_FCMD_ADDI) ? {{22{f2_imm[9]}}, f2_imm[9:0]} & {32{add_enable}} :
                                                (f2_in3 ^ {32{(f2_cmd == `VPU_FCMD_SUB)}}) & {32{add_enable}};

  assign add_cin = {31'b0, (f2_cmd == `VPU_FCMD_SUB)};
  assign add_out = add_in1 + add_in3 + add_cin;

  // outputs
  assign out_valids_packed = {gfx_out_valids_ORed, mvfx_enable, fbci_ps_enable, mvxf_enable, fsgnj_enable, fsat8_enable, fsatu8_enable, add_enable};
  assign out_valids_ORed   = |out_valids_packed;



///////////////////////////////////////////////////////////////////////////////
// Result mux
///////////////////////////////////////////////////////////////////////////////

    //select functional output
  // select functional output
  always_comb begin
    gfx_out = 32'b0;

    if (gfx_out_valids_ORed) begin
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
        default:                   gfx_out = 32'b0;
      endcase
    end
  end

  // output data
  always_comb begin
    f2_data = 32'b0;

    if (out_valids_ORed) begin
      case (out_valids_packed)
        8'b10000000: f2_data = gfx_out;
        8'b01000000: f2_data = mvfx_out;
        8'b00100000: f2_data = fbci_ps_out;
        8'b00010000: f2_data = mvxf_out;
        8'b00001000: f2_data = fsgnj_out;
        8'b00000100: f2_data = fsat8_out;
        8'b00000010: f2_data = fsatu8_out;
        8'b00000001: f2_data = add_out;
        default:     f2_data = 32'b0;
      endcase
    end
  end

    //output flags
  assign f2_flags = cubeface_idx_flag_out & {`VPU_FLAGS_SZ{cubeface_idx_enable}};

    //f2 swizz data out
  assign f2_fswizz_rdata = f2_in1;

  
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// F3 Stage
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

  //   CLK    EN           DOUT      DIN
  `EN_FF(clock, f2_in_valid, f3_data,  f2_data)
  `EN_FF(clock, f2_in_valid, f3_flags, f2_flags)

  // output assignation
  assign f3_out_bits.data = f3_data;
  assign f3_out_bits.exc  = f3_flags;

endmodule

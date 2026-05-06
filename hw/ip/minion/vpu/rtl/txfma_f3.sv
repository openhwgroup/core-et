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

module txfma_f3
(
  input                                       ctrl_f2_clk,
  input                                       ctrl_f3_clk,
  input [TXFMA_A3:TXFMA_B27]                fa_f3a_h,
  input [TXFMA_A3:TXFMA_B27]                fc_f3a_h,
  input [TXFMA_A3:0]                         prodsum_f3a_h,
  input [6:0]                                 exp_res_f2a_h,
  input [5:0]                                 ediff_abs_diff_hi_f2a_h,
  input [4:0]                                 align2_rshamt_c3_f2a_h,
  input [1:0]                                 op3p3spc_sel_f2a_h,
  input [2:0]                                 op3p2c_sel_f2a_h,
  input [1:0]                                 imxc_sel_f3a_h,
  input [1:0]                                 ugfxrplc_sel_f2a_h,
  input [1:0]                                 sgfxrplc_sel_f2a_h,
  input [1:0]                                 icgfxc_sel_f2a_h,
  input [1:0]                                 al1znp2c_sel_f2a_h,
  input [1:0]                                 al2c_sel_f2a_h,
  input [1:0]                                 al1p2c_sel_f2a_h,
  input                                       al1p1c_sel_f2a_h,
  input                                       op3p1c_sel_r_f2a_h,
  input                                       op3p1c_sel_p_f2a_h,
  input                                       op3g_sel_f2a_h,
  input                                       op2_sxtto_g_f2a_h,
  input                                       rm_rne_f2a_h,
  input                                       rm_rmm_f2a_h,
  input                                       frnd_rupnsc_f2a_h,
  input                                       frnd_rdnsc_f2a_h,
  input                                       frnd_t1_f2a_h,
  input                                       frnd_t2_f2a_h,
  input                                       al1c_zap_f2a_h,
  input                                       op1c_log_mul_clr_f2a_h,
  input                                       negop1c_int_logmul_clr_f2a_h,
  input                                       psc_op1_clr_f2a_h,
  input                                       align2_zap_f2a_h,
  input                                       align2c_neg_f2a_h,
  input                                       al2c3_neg_cin_ps_f3a_h,
  output logic                                fround_rbit_f3a_h,
  output logic                                prodsum_a2_f3a_h,
  output logic                                op2_sxtto_g_f3a_h,
  output logic [TXFMA_A3:TXFMA_B47]         comb_s_f4a_h,
  output logic [TXFMA_A3:TXFMA_B47]         comb_c_f4a_h,
  output logic [TXFMA_A3:TXFMA_B27]         int_minmax_res_f4a_h
);


////////////////////////////////////////////////////////////////////////////
// Internal Signals
////////////////////////////////////////////////////////////////////////////
logic [TXFMA_A1:TXFMA_B45]      align_in_48b_f3a_h;
logic [TXFMA_A1:TXFMA_B47]      aligned_48b_f3a_h;
logic [TXFMA_A1:TXFMA_B23]      opd3_special_in_f3a_h;
logic [TXFMA_A3:TXFMA_B45]      ugfxrpl1_in_f3a_h;
logic [TXFMA_A3:TXFMA_B45]      sgfxrpl1_in_f3a_h;
logic [TXFMA_A3:TXFMA_B45]      int_gfxrpl1_out_f3a_h;
logic [TXFMA_A0:TXFMA_B22]      fc_mask_f3a_h;
logic                             aligned_48b_int_r_f3a_h;
logic                             aligned_48b_int_l_f3a_h;
logic [5:0]                       ediff_abs_diff_hi_f3a_h;
logic [4:0]                       align2_rshamt_c3_f3a_h;
logic [6:0]                       exp_res_f3a_h;
logic [TXFMA_A1:TXFMA_B22]      align_in_25b_f3a_h;
logic [TXFMA_A1:TXFMA_B24]      aligned_25b_f3a_h;
logic [TXFMA_A1:TXFMA_B47]      zapped_op1_f3a_h;
logic [TXFMA_A1:TXFMA_B24]      zapped_op2_f3a_h;
logic [TXFMA_A3:TXFMA_B47]      opd3_f3a_h;
logic [TXFMA_A3:TXFMA_B47]      opd3_prod_f3a_h;
logic [TXFMA_A3:TXFMA_B47]      align1_gfx_f3a_h;
logic [TXFMA_A2:TXFMA_B24]      negated_op2_f3a_h;
logic [TXFMA_A3:TXFMA_B47]      negated_op2_ext_f3a_h;
logic [1:0]                       op3p3spc_sel_f3a_h;
logic [2:0]                       op3p2c_sel_f3a_h;
logic                             op3p1c_sel_p_f3a_h;
logic                             op3p1c_sel_r_f3a_h;
logic                             op3g_sel_f3a_h;
logic [1:0]                       ugfxrplc_sel_f3a_h;
logic [1:0]                       sgfxrplc_sel_f3a_h;
logic [1:0]                       icgfxc_sel_f3a_h;
logic [1:0]                       al1p2c_sel_f3a_h;
logic                             al1p1c_sel_f3a_h;
logic                             rm_rne_f3a_h;
logic                             rm_rmm_f3a_h;
logic                             frnd_rupnsc_f3a_h;
logic                             frnd_rdnsc_f3a_h;
logic                             frnd_t1_f3a_h;
logic                             frnd_t2_f3a_h;
logic                             round_rbit_f3a_h;
logic                             rnet1_f3a_h;
logic [1:0]                       al1znp2c_sel_f3a_h;
logic                             al1c_zap_f3a_h;
logic                             op1c_log_mul_clr_f3a_h;
logic [TXFMA_A0:TXFMA_B5]       ext_op1c_a0b5_log_mul_clr_f3a_h;
logic                             negop1c_int_logmul_clr_f3a_h;
logic [TXFMA_A3:TXFMA_A1]       ext_negop1c_a3a1_int_logmul_clr_f3a_h;
logic                             psc_op1_clr_f3a_h;
logic [TXFMA_B25:TXFMA_B47]     ext_psc_op1_b24b45_clr_f3a_h;
logic [1:0]                       al2c_sel_f3a_h;
logic                             align2_zap_f3a_h;
logic                             align2c_neg_f3a_h;

logic [TXFMA_A3:TXFMA_B47]      csa_in1_f3a_h;
logic [TXFMA_A3:TXFMA_B47]      csa_in2_f3a_h;
logic [TXFMA_A3:TXFMA_B47]      csa_in3_f3a_h;
logic [TXFMA_A3:TXFMA_B47]      comb_s_f3a_h;
logic [TXFMA_A3:TXFMA_B47]      comb_c_f3a_h;
logic [TXFMA_A3:TXFMA_B27]      int_minmax_res_f3a_h;
logic                             fround_l_f3a_h;


///////////////////////////////////////////////////////////////////////////////
// F4 flops; Moved from txfma_top
///////////////////////////////////////////////////////////////////////////////

always_ff @(posedge ctrl_f3_clk) begin
  comb_s_f4a_h <= comb_s_f3a_h;
  comb_c_f4a_h <= comb_c_f3a_h;
  int_minmax_res_f4a_h <= int_minmax_res_f3a_h;
end

// Locally flopped control signals I/P from txfma_c2/txfma_e2
always_ff @(posedge ctrl_f2_clk) begin
  exp_res_f3a_h <= exp_res_f2a_h;
  ediff_abs_diff_hi_f3a_h <= ediff_abs_diff_hi_f2a_h;
  align2_rshamt_c3_f3a_h <= align2_rshamt_c3_f2a_h;
  op3p3spc_sel_f3a_h <= op3p3spc_sel_f2a_h;
  op3p2c_sel_f3a_h <= op3p2c_sel_f2a_h;
  op3p1c_sel_p_f3a_h <= op3p1c_sel_p_f2a_h;
  op3p1c_sel_r_f3a_h <= op3p1c_sel_r_f2a_h;
  op3g_sel_f3a_h <= op3g_sel_f2a_h;
  ugfxrplc_sel_f3a_h <= ugfxrplc_sel_f2a_h;
  sgfxrplc_sel_f3a_h <= sgfxrplc_sel_f2a_h;
  icgfxc_sel_f3a_h <= icgfxc_sel_f2a_h;
  al1p1c_sel_f3a_h <= al1p1c_sel_f2a_h;
  al1p2c_sel_f3a_h <= al1p2c_sel_f2a_h;
  rm_rne_f3a_h <= rm_rne_f2a_h;
  rm_rmm_f3a_h <= rm_rmm_f2a_h;
  frnd_rupnsc_f3a_h <= frnd_rupnsc_f2a_h;
  frnd_rdnsc_f3a_h <= frnd_rdnsc_f2a_h;
  frnd_t1_f3a_h <= frnd_t1_f2a_h;
  frnd_t2_f3a_h <= frnd_t2_f2a_h;
  al1znp2c_sel_f3a_h <= al1znp2c_sel_f2a_h;
  al1c_zap_f3a_h <= al1c_zap_f2a_h;
  op1c_log_mul_clr_f3a_h <= op1c_log_mul_clr_f2a_h;
  negop1c_int_logmul_clr_f3a_h <= negop1c_int_logmul_clr_f2a_h;
  psc_op1_clr_f3a_h <= psc_op1_clr_f2a_h;
  al2c_sel_f3a_h <= al2c_sel_f2a_h;
  align2_zap_f3a_h <= align2_zap_f2a_h;
  align2c_neg_f3a_h <= align2c_neg_f2a_h;
  op2_sxtto_g_f3a_h <= op2_sxtto_g_f2a_h;
end

///////////////////////////////////////////////////////////////////////////////
// 48bit Align High Mux 1
///////////////////////////////////////////////////////////////////////////////
always_comb
begin

  casez (al1p2c_sel_f3a_h)
    2'b00:     align_in_48b_f3a_h[TXFMA_A1:TXFMA_B30] = {1'b0, fc_f3a_h[TXFMA_A3:TXFMA_B27]};
    2'b01:     align_in_48b_f3a_h[TXFMA_A1:TXFMA_B30] = {1'b0, fa_f3a_h[TXFMA_A3:TXFMA_B27]};
    2'b1?:     align_in_48b_f3a_h[TXFMA_A1:TXFMA_B30] = prodsum_f3a_h[TXFMA_A1:TXFMA_B30];
  endcase

  align_in_48b_f3a_h[TXFMA_B31:TXFMA_B45] = prodsum_f3a_h[TXFMA_B31:TXFMA_B45] & {($bits(prodsum_f3a_h[TXFMA_B31:TXFMA_B45])){al1p1c_sel_f3a_h}};

end

assign prodsum_a2_f3a_h = prodsum_f3a_h[TXFMA_A2];


///////////////////////////////////////////////////////////////////////////////
// 48 bit Alignment shifter
///////////////////////////////////////////////////////////////////////////////

txfma_align_shf
#(.left_bit  (TXFMA_A1                     ),
  .right_bit (TXFMA_B47                    ),
  .shft_size ($bits(ediff_abs_diff_hi_f3a_h))
)
align_shf_48
(
  .op_i      ( {align_in_48b_f3a_h, 2'b0}  ),
  .shfamnt_i ( ediff_abs_diff_hi_f3a_h     ),
  .op_align_o( aligned_48b_f3a_h           )
);

assign aligned_48b_int_r_f3a_h = zapped_op1_f3a_h[TXFMA_INT_R];
assign aligned_48b_int_l_f3a_h = zapped_op1_f3a_h[TXFMA_INT_L];



///////////////////////////////////////////////////////////////////////////////
// 48 bit ZAP
///////////////////////////////////////////////////////////////////////////////
assign zapped_op1_f3a_h = {($bits(aligned_48b_f3a_h)){!al1c_zap_f3a_h}} & aligned_48b_f3a_h;



///////////////////////////////////////////////////////////////////////////////
// 25bit Align Low Mux 2
///////////////////////////////////////////////////////////////////////////////
always_comb
begin
  case (al2c_sel_f3a_h)
    2'b00:      align_in_25b_f3a_h = 25'b0;
    2'b01:      align_in_25b_f3a_h = {1'b0, fa_f3a_h[TXFMA_A3:TXFMA_B19]};
    2'b10:      align_in_25b_f3a_h = {prodsum_f3a_h[TXFMA_B36:TXFMA_B57], 3'b0};
    2'b11:      align_in_25b_f3a_h = {prodsum_f3a_h[TXFMA_A1:TXFMA_B19], 3'b0};
  endcase
end


///////////////////////////////////////////////////////////////////////////////
// 27 bit Alignment shifter
///////////////////////////////////////////////////////////////////////////////

txfma_align_shf
#(.left_bit  (TXFMA_A1),
  .right_bit (TXFMA_B24),
  .shft_size ($bits(align2_rshamt_c3_f3a_h))
)
align_shf_27
(
  .op_i      ( {align_in_25b_f3a_h, 2'b0}),
  .shfamnt_i ( align2_rshamt_c3_f3a_h   ),
  .op_align_o( aligned_25b_f3a_h        )
);


///////////////////////////////////////////////////////////////////////////////
// 26 bit ZAP
///////////////////////////////////////////////////////////////////////////////
assign zapped_op2_f3a_h = {($bits(aligned_25b_f3a_h)){!align2_zap_f3a_h}} & aligned_25b_f3a_h;


///////////////////////////////////////////////////////////////////////////////
// 27 bit Negation
///////////////////////////////////////////////////////////////////////////////
assign negated_op2_f3a_h[TXFMA_A2:TXFMA_B24]  = {($bits({1'b0, zapped_op2_f3a_h[TXFMA_A1:TXFMA_B24]})){align2c_neg_f3a_h}} ^ {1'b0, zapped_op2_f3a_h[TXFMA_A1:TXFMA_B24]};
assign negated_op2_ext_f3a_h = {negated_op2_f3a_h[TXFMA_A2], negated_op2_f3a_h, {(TXFMA_B25-TXFMA_B47+1){op2_sxtto_g_f3a_h}}};


///////////////////////////////////////////////////////////////////////////////
// FROUND R control bit
//   We convert the single-precision FP number into an Integer; round and
//   then convert back to SP FP. Due to the timing, we cannot use the main
//   datapath rounding logic. For that reason, we calculate the round bit
//   here for the FROUND instruction only to meet critical timing.
///////////////////////////////////////////////////////////////////////////////

always_comb begin

  round_rbit_f3a_h = ~(aligned_48b_int_r_f3a_h & (rm_rmm_f3a_h | frnd_rdnsc_f3a_h | frnd_rupnsc_f3a_h));
  rnet1_f3a_h      = ~((aligned_48b_int_l_f3a_h & aligned_48b_int_r_f3a_h & rm_rne_f3a_h) | (aligned_48b_int_r_f3a_h & frnd_t1_f3a_h));

  fround_rbit_f3a_h = aligned_48b_int_r_f3a_h;

  fround_l_f3a_h   = ~(round_rbit_f3a_h & !frnd_t2_f3a_h & rnet1_f3a_h);
end


////////////////////////////////////////////////////////////////////////////////
// Oprand3 Muxes
////////////////////////////////////////////////////////////////////////////////

//assign fc_mask_f3a_h = (fc_f3a_h[TXFMA_A3:TXFMA_B19] & aligned_48b_f3a_h[TXFMA_A0:TXFMA_B22]);
assign fc_mask_f3a_h = (fc_f3a_h[TXFMA_A3:TXFMA_B19] & zapped_op1_f3a_h[TXFMA_A0:TXFMA_B22]);


// Special Cases
always_comb
begin
  case (op3p3spc_sel_f3a_h)
    2'b00:     opd3_special_in_f3a_h = 26'b0;
    2'b01:     opd3_special_in_f3a_h = {1'b0, exp_res_f3a_h, 18'b0};
    2'b10:     opd3_special_in_f3a_h = {1'b0, fa_f3a_h[TXFMA_A3:TXFMA_B20]};
    2'b11:     opd3_special_in_f3a_h = {1'b0, fc_f3a_h[TXFMA_A3:TXFMA_B20]};
  endcase
end


// Bits [A1:B23]
always_comb
begin
  case (op3p2c_sel_f3a_h[2])
    1'b0:     opd3_prod_f3a_h[TXFMA_A3:TXFMA_B47] = {2'b0, prodsum_f3a_h[TXFMA_A1:TXFMA_B45],2'b0};
    1'b1:     opd3_prod_f3a_h[TXFMA_A3:TXFMA_B47] = {3'b0, prodsum_f3a_h[TXFMA_A3:TXFMA_B42],2'b0};
  endcase
end

always_comb
begin
  case (op3p2c_sel_f3a_h[1:0])
    2'b00:     opd3_f3a_h[TXFMA_A3:TXFMA_B23] = {2'b0, opd3_special_in_f3a_h};
    2'b01:     opd3_f3a_h[TXFMA_A3:TXFMA_B23] = {2'b0, prodsum_f3a_h[TXFMA_B36:TXFMA_B57], 4'b0};
    2'b10:     opd3_f3a_h[TXFMA_A3:TXFMA_B23] = {3'b0, fc_mask_f3a_h[TXFMA_A0:TXFMA_B22], 1'b0};
    2'b11:     opd3_f3a_h[TXFMA_A3:TXFMA_B23] = opd3_prod_f3a_h[TXFMA_A3:TXFMA_B23];
  endcase

  casez ({op3p1c_sel_p_f3a_h, op3g_sel_f3a_h})
    2'b00:     opd3_f3a_h[TXFMA_B24] = 1'b0;
    2'b01:     opd3_f3a_h[TXFMA_B24] = al2c3_neg_cin_ps_f3a_h;
    2'b1?:     opd3_f3a_h[TXFMA_B24] = opd3_prod_f3a_h[TXFMA_B24];
  endcase

  // Bits [B25:B29]
  opd3_f3a_h[TXFMA_B25:TXFMA_B29] = opd3_prod_f3a_h[TXFMA_B25:TXFMA_B29]
                                  & {($bits(opd3_prod_f3a_h[TXFMA_B25:TXFMA_B29])){op3p1c_sel_p_f3a_h}};

  // Bits [B30]
  opd3_f3a_h[TXFMA_B30] = (opd3_prod_f3a_h[TXFMA_B30] & op3p1c_sel_p_f3a_h)
                        | (fround_l_f3a_h & op3p1c_sel_r_f3a_h);

  // Bits [B31]
  opd3_f3a_h[TXFMA_B31] = opd3_prod_f3a_h[TXFMA_B31] & op3p1c_sel_p_f3a_h;

  // Bits [B32:B47]
  opd3_f3a_h[TXFMA_B32:TXFMA_B47] = opd3_prod_f3a_h[TXFMA_B32:TXFMA_B47]
                                  & {($bits(opd3_prod_f3a_h[TXFMA_B32:TXFMA_B47])){op3p1c_sel_p_f3a_h}};
end



//////////////////////////////////////////////////////////////////////////////
// Integer MinMax Mux
//////////////////////////////////////////////////////////////////////////////
always_comb
begin
  case (imxc_sel_f3a_h)
    2'b00:     int_minmax_res_f3a_h = fa_f3a_h[TXFMA_A3:TXFMA_B27];
    2'b01:     int_minmax_res_f3a_h = fc_f3a_h[TXFMA_A3:TXFMA_B27];
    2'b10:     int_minmax_res_f3a_h = prodsum_f3a_h[31:0];
    2'b11:     int_minmax_res_f3a_h = prodsum_f3a_h[TXFMA_A3:TXFMA_B27];
  endcase
end


//////////////////////////////////////////////////////////////////////////////
// Graphics/NORM (Signed & Unsigned) Muxes
// NOTE: GFX Replication is done here
//////////////////////////////////////////////////////////////////////////////
always_comb
begin
  case (ugfxrplc_sel_f3a_h)
    2'b00:     ugfxrpl1_in_f3a_h = {2'b0, {6{align_in_48b_f3a_h[TXFMA_A0:TXFMA_B6]}}};
    2'b01:     ugfxrpl1_in_f3a_h = {2'b0, {4{align_in_48b_f3a_h[TXFMA_A0:TXFMA_B8]}}, 8'b0};
    2'b10:     ugfxrpl1_in_f3a_h = {2'b0, {3{align_in_48b_f3a_h[TXFMA_A0:TXFMA_B14]}}};
    2'b11:     ugfxrpl1_in_f3a_h = {2'b0, {2{align_in_48b_f3a_h[TXFMA_A0:TXFMA_B22]}}};
  endcase
end

always_comb
begin
  casez (sgfxrplc_sel_f3a_h)
    2'b00:     sgfxrpl1_in_f3a_h = {2'b0, {6{align_in_48b_f3a_h[TXFMA_B0:TXFMA_B6]}}, 6'b0};
    2'b01:     sgfxrpl1_in_f3a_h = {2'b0, {3{align_in_48b_f3a_h[TXFMA_B0:TXFMA_B14]}}, 3'b0};
    2'b1?:     sgfxrpl1_in_f3a_h = {2'b0, {24{align_in_48b_f3a_h[TXFMA_A0:TXFMA_B0]}}};
  endcase
end

always_comb
begin
  casez (icgfxc_sel_f3a_h)
    2'b00:     int_gfxrpl1_out_f3a_h = 50'b0;
    2'b01:     int_gfxrpl1_out_f3a_h = sgfxrpl1_in_f3a_h;
    2'b1?:     int_gfxrpl1_out_f3a_h = ugfxrpl1_in_f3a_h;
  endcase
end


//////////////////////////////////////////////////////////////////////////////
// Mux between 48-bit Alignment Shifter O/P and Graphics
// NOTE: Negation/Zapping of the alignment shifter O/P is handled here
//////////////////////////////////////////////////////////////////////////////

// Extend the control signals to a matching bit-width
assign ext_op1c_a0b5_log_mul_clr_f3a_h       = {($bits(align1_gfx_f3a_h[TXFMA_A0:TXFMA_B5])){op1c_log_mul_clr_f3a_h}};
assign ext_negop1c_a3a1_int_logmul_clr_f3a_h = {($bits(align1_gfx_f3a_h[TXFMA_A3:TXFMA_A1])){negop1c_int_logmul_clr_f3a_h}};
assign ext_psc_op1_b24b45_clr_f3a_h          = {($bits(align1_gfx_f3a_h[TXFMA_B25:TXFMA_B47])){psc_op1_clr_f3a_h}};

// Bits [A3:A1]
always_comb
begin
  casez (al1znp2c_sel_f3a_h)
    2'b00:     align1_gfx_f3a_h[TXFMA_A3:TXFMA_A1] = int_gfxrpl1_out_f3a_h[TXFMA_A3:TXFMA_A1];
    2'b01:     align1_gfx_f3a_h[TXFMA_A3:TXFMA_A1] = {2'b0, zapped_op1_f3a_h[TXFMA_A1]} & ~ext_negop1c_a3a1_int_logmul_clr_f3a_h;
    2'b1?:     align1_gfx_f3a_h[TXFMA_A3:TXFMA_A1] = ~({2'b0, zapped_op1_f3a_h[TXFMA_A1]} | ext_negop1c_a3a1_int_logmul_clr_f3a_h);
  endcase
end

// Bits [A0:B5]
always_comb
begin
  casez (al1znp2c_sel_f3a_h)
    2'b00:     align1_gfx_f3a_h[TXFMA_A0:TXFMA_B5] = int_gfxrpl1_out_f3a_h[TXFMA_A0:TXFMA_B5];
    2'b01:     align1_gfx_f3a_h[TXFMA_A0:TXFMA_B5] = zapped_op1_f3a_h[TXFMA_A0:TXFMA_B5] & ~ext_op1c_a0b5_log_mul_clr_f3a_h;
    2'b1?:     align1_gfx_f3a_h[TXFMA_A0:TXFMA_B5] = ~((zapped_op1_f3a_h[TXFMA_A0:TXFMA_B5]) | ext_op1c_a0b5_log_mul_clr_f3a_h);
  endcase
end

// Bits [B6:B24]
always_comb
begin
  casez (al1znp2c_sel_f3a_h)
    2'b00:     align1_gfx_f3a_h[TXFMA_B6:TXFMA_B24] = int_gfxrpl1_out_f3a_h[TXFMA_B6:TXFMA_B24];
    2'b01:     align1_gfx_f3a_h[TXFMA_B6:TXFMA_B24] = zapped_op1_f3a_h[TXFMA_B6:TXFMA_B24];
    2'b1?:     align1_gfx_f3a_h[TXFMA_B6:TXFMA_B24] = ~(zapped_op1_f3a_h[TXFMA_B6:TXFMA_B24]);
  endcase
end

// Bits [B25:B45]
always_comb
begin
  casez (al1znp2c_sel_f3a_h)
    2'b00:     align1_gfx_f3a_h[TXFMA_B25:TXFMA_B47] = {int_gfxrpl1_out_f3a_h[TXFMA_B25:TXFMA_B45], 2'b0};
    2'b01:     align1_gfx_f3a_h[TXFMA_B25:TXFMA_B47] = zapped_op1_f3a_h[TXFMA_B25:TXFMA_B47] & ext_psc_op1_b24b45_clr_f3a_h;
    2'b1?:     align1_gfx_f3a_h[TXFMA_B25:TXFMA_B47] = ~((zapped_op1_f3a_h[TXFMA_B25:TXFMA_B47]) & ext_psc_op1_b24b45_clr_f3a_h);
  endcase
end


////////////////////////////////////////////////////////////////////////////////
// CSA ADDER - Combining Adder is in txfma_f4
////////////////////////////////////////////////////////////////////////////////

assign csa_in1_f3a_h = opd3_f3a_h;
assign csa_in2_f3a_h = negated_op2_ext_f3a_h;
assign csa_in3_f3a_h = align1_gfx_f3a_h;

txfma_csa
#(
  .Bits(TXFMA_A3-TXFMA_B47+1)
)
f3_32csa
 (
  .a_i  ( csa_in1_f3a_h ),
  .b_i  ( csa_in2_f3a_h ),
  .cin_i( csa_in3_f3a_h ),
  .cout_o( comb_c_f3a_h  ),
  .sum_o( comb_s_f3a_h  )
);

endmodule

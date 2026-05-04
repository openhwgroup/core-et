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

/* verilator lint_off UNUSEDSIGNAL */  // Imported original TXFMA exponent stage preserves packed signal fields even when some bits are not consumed here.
module txfma_e6
(
  input  [TXFMA_EXP_SEXT_PS_SZ-1:0]  exp_res_in_f6a_h,
  input                               ps_rndovf_f6a_h,
  input                               ph_rndovf_f6a_h,
  input  [VPU_FCMD_SZ-1:0]           cmd_f6a_h,
  input [TXFMA_SIGS_SZ-1:0]          sigs_f6a_h,
  input [VPU_DTYPE_SZ-1:0]           op_dtype_f6a_h,
  output logic                        def_ovf_ps_f6a_h,
  output logic                        pot_ovf_ps_f6a_h,
  output logic                        def_unf_ps_f6a_h,
  output logic                        pot_unf_ps_f6a_h,
  output logic                        def_ovf_ph_f6a_h,
  output logic                        pot_ovf_ph_f6a_h,
  output logic                        def_unf_ph_f6a_h,
  output logic                        pot_unf_ph_f6a_h,

  output [TXFMA_EXP_PS_SZ-1:0]       exp_res_f6a_h,
  output                              exp_res_zero_f6a_h
);

logic [TXFMA_EXP_PS_SZ-1:0] exp_res_inc_dec_f6a_h;
logic [TXFMA_EXP_PS_SZ-1:0] e_res_adder_in2_mux_f6a_h;

always_comb
begin

  pot_ovf_ps_f6a_h =  1'b0;
  def_ovf_ps_f6a_h =  1'b0;
  pot_unf_ps_f6a_h =  1'b0;
  def_unf_ps_f6a_h =  1'b0;
  pot_ovf_ph_f6a_h =  1'b0;
  def_ovf_ph_f6a_h =  1'b0;
  pot_unf_ph_f6a_h =  1'b0;
  def_unf_ph_f6a_h =  1'b0;

  if ((op_dtype_f6a_h == VPU_DTYPE_F32) & ~(sigs_f6a_h[TXFMA_FX_F32])) begin
    pot_ovf_ps_f6a_h = (exp_res_in_f6a_h == 10'hfe);
    def_ovf_ps_f6a_h = $signed(exp_res_in_f6a_h) > $signed(10'h0fe);
    pot_unf_ps_f6a_h = (exp_res_in_f6a_h == 10'h0);
    def_unf_ps_f6a_h = $signed(exp_res_in_f6a_h) < $signed(10'h0);
  end else if (op_dtype_f6a_h == VPU_DTYPE_F16_F32) begin
    pot_ovf_ps_f6a_h = (exp_res_in_f6a_h == 10'hfe) & !sigs_f6a_h[TXFMA_MUL];
    def_ovf_ps_f6a_h = $signed(exp_res_in_f6a_h) > $signed(10'h0fe) & !sigs_f6a_h[TXFMA_MUL];
    pot_unf_ps_f6a_h = (exp_res_in_f6a_h == 10'h0)  & !sigs_f6a_h[TXFMA_MUL];
    def_unf_ps_f6a_h = $signed(exp_res_in_f6a_h) < $signed(10'h0)  & !sigs_f6a_h[TXFMA_MUL];
  end else begin
    pot_ovf_ph_f6a_h = (exp_res_in_f6a_h == FLOAT16_32_MAX);
    def_ovf_ph_f6a_h = $signed(exp_res_in_f6a_h) > $signed(FLOAT16_32_MAX);
    pot_unf_ph_f6a_h = (exp_res_in_f6a_h == TXFMA_BIAS_DIFF) & (exp_res_in_f6a_h!=0);
    def_unf_ph_f6a_h = ($signed(exp_res_in_f6a_h) < $signed(TXFMA_BIAS_DIFF)) & (exp_res_in_f6a_h!=0);
  end
end

  //adder in mux
assign e_res_adder_in2_mux_f6a_h = (cmd_f6a_h==VPU_TRANS_RSQRT_FMA2) ? 8'hfe //substract 2 to compensate cin
                                    :                                   8'b0;

///////////////////////////////////////////////////////////////////////////////
// exp hi res add hi
///////////////////////////////////////////////////////////////////////////////
  // Do incrementing anyway in order to shorten the timing
txfma_adder
#(
  .Width(TXFMA_EXP_PS_SZ)
)
exp_hi_res_adder_hi
(
  .a_i  ( exp_res_in_f6a_h[TXFMA_EXP_PS_SZ-1:0] ),
  .b_i  ( e_res_adder_in2_mux_f6a_h              ),
  .cin_i( 1'b1                                   ),
  .sum_o( exp_res_inc_dec_f6a_h                  )
);


///////////////////////////////////////////////////////////////////////////////
// exp hi res mux
///////////////////////////////////////////////////////////////////////////////
assign exp_res_f6a_h = (ps_rndovf_f6a_h || ph_rndovf_f6a_h || (cmd_f6a_h==VPU_TRANS_RSQRT_FMA2)) ?  exp_res_inc_dec_f6a_h[TXFMA_EXP_PS_SZ-1:0]
                        :                                                                            exp_res_in_f6a_h[TXFMA_EXP_PS_SZ-1:0];

assign exp_res_zero_f6a_h = (exp_res_f6a_h==8'b00000000);

endmodule
/* verilator lint_on UNUSEDSIGNAL */

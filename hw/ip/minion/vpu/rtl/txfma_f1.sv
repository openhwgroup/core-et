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

module txfma_f1
(
  input                                   ctrl_f0_clk,
  input                                   multype_f0_clk,
  input                                   ctrl_f1_clk,
  input                                   multype_f1_clk,
  input  [VPU_DATA_S_SZ-1:0]             fa_f0a_h,
  input  [VPU_DATA_S_SZ-1:0]             fb_f0a_h,
  input  [VPU_DATA_S_SZ-1:0]             fc_f0a_h,
  input                                   dtps_f0a_h,
  input                                   mul_sign_f0a_h,
  input                                   mul_hp_mode_f0a_h,
  output logic                            fc_b0_f2a_h,
  output logic [TXFMA_A3:TXFMA_B27]     fc_f2a_h,
  output logic [TXFMA_A3:TXFMA_B27]     fa_f2a_h,
  output logic [TXFMA_A3:TXFMA_B59]     wallace_fC21_f2a_h,
  output logic [TXFMA_A3:TXFMA_B59]     wallace_fC22_f2a_h,
  output logic [TXFMA_A3:TXFMA_B59]     wallace_fS21_f2a_h,
  output logic [TXFMA_A3:TXFMA_B59]     wallace_fS22_f2a_h,
  output logic [VPU_DATA_S_SZ-1:0]       booth_fCin_f2a_h,
  output logic [VPU_DATA_S_SZ-1:0]       booth_fSxt_f2a_h,
  output logic [32:0]                     booth_fpp17_f2a_h,
  output logic [3:0]                      etrz_fa_hi_ph_f1a_h,
  output logic [4:0]                      etrz_fa_lo_ps_f1a_h,
  output logic [3:0]                      etrz_fb_hi_ph_f1a_h,
  output logic [4:0]                      etrz_fb_lo_ps_f1a_h,
  output logic [4:0]                      etrz_fc_f1a_h
);

///////////////////////////////////////////////////////////////////////////////
// Internal Signals
///////////////////////////////////////////////////////////////////////////////

logic                         mul_sign_f1a_h;
logic                         mul_hp_mode_f1a_h;
logic [VPU_DATA_S_SZ-1:0]    fa_f1a_h;
logic [VPU_DATA_S_SZ-1:0]    fb_f1a_h;
logic [VPU_DATA_S_SZ-1:0]    fc_f1a_h;
logic                         dtps_f1a_h;
logic [32:0]                  pp1_f1a_h;
logic [32:0]                  pp2_f1a_h;
logic [32:0]                  pp3_f1a_h;
logic [32:0]                  pp4_f1a_h;
logic [32:0]                  pp5_f1a_h;
logic [32:0]                  pp6_f1a_h;
logic [32:0]                  pp7_f1a_h;
logic [32:0]                  pp8_f1a_h;
logic [32:0]                  pp9_f1a_h;
logic [32:0]                  pp10_f1a_h;
logic [32:0]                  pp11_f1a_h;
logic [32:0]                  pp12_f1a_h;
logic [32:0]                  pp13_f1a_h;
logic [32:0]                  pp14_f1a_h;
logic [32:0]                  pp15_f1a_h;
logic [32:0]                  pp16_f1a_h;
logic [FLOAT32_M:0]          fa_lo_ps_trz_frac_mux;
logic [FLOAT32_M:0]          fb_lo_ps_trz_frac_mux;
logic [FLOAT16_M:0]          fa_hi_ph_trz_frac_mux;
logic [FLOAT16_M:0]          fb_hi_ph_trz_frac_mux;
logic [TXFMA_A3:TXFMA_B59]  wallace_fC21_f1a_h;
logic [TXFMA_A3:TXFMA_B59]  wallace_fC22_f1a_h;
logic [TXFMA_A3:TXFMA_B59]  wallace_fS21_f1a_h;
logic [TXFMA_A3:TXFMA_B59]  wallace_fS22_f1a_h;
logic [VPU_DATA_S_SZ-1:0]    booth_fCin_f1a_h;
logic [VPU_DATA_S_SZ-1:0]    booth_fSxt_f1a_h;
logic [32:0]                  booth_fpp17_f1a_h;
logic [TXFMA_A3:TXFMA_B27]  fc_shift_f1a_h;
logic [TXFMA_A3:TXFMA_B27]  fa_shift_f1a_h;


///////////////////////////////////////////////////////////////////////////////
// Local Flops - Needed to guarantee timing
///////////////////////////////////////////////////////////////////////////////

always_ff @(posedge ctrl_f0_clk) begin
  fa_f1a_h <= fa_f0a_h;
  fb_f1a_h <= fb_f0a_h;
  fc_f1a_h <= fc_f0a_h;
  dtps_f1a_h <= dtps_f0a_h;
end

always_ff @(posedge multype_f0_clk) begin
  mul_sign_f1a_h <= mul_sign_f0a_h;
  mul_hp_mode_f1a_h <= mul_hp_mode_f0a_h;
end

always_ff @(posedge ctrl_f1_clk) begin
  fa_f2a_h <= fa_shift_f1a_h;
  fc_f2a_h <= fc_shift_f1a_h;
end

always_ff @(posedge multype_f1_clk) begin
  wallace_fC21_f2a_h <= wallace_fC21_f1a_h;
  wallace_fC22_f2a_h <= wallace_fC22_f1a_h;
  wallace_fS21_f2a_h <= wallace_fS21_f1a_h;
  wallace_fS22_f2a_h <= wallace_fS22_f1a_h;
  booth_fCin_f2a_h <= booth_fCin_f1a_h;
  booth_fSxt_f2a_h <= booth_fSxt_f1a_h;
  booth_fpp17_f2a_h <= booth_fpp17_f1a_h;
end


///////////////////////////////////////////////////////////////////////////////
// Local manipulation of Operands
///////////////////////////////////////////////////////////////////////////////
assign fa_shift_f1a_h = fa_f1a_h;
assign fc_shift_f1a_h = fc_f1a_h;
assign fc_b0_f2a_h    = fc_f2a_h[TXFMA_B0+3]; // b0 is now in bit 62


///////////////////////////////////////////////////////////////////////////////
// Booth PPG 32R4 +  Sign extend correction + 2'complement correction
///////////////////////////////////////////////////////////////////////////////
txfma_booth_ppg_32r4
ppg
(
  .mode_i   ( mul_hp_mode_f1a_h    ),
  .mplr_i   ( fa_f1a_h             ),
  .mpcnd_i  ( fb_f1a_h             ),
  .sign_i   ( mul_sign_f1a_h       ),
  .sxt_o    ( booth_fSxt_f1a_h     ),
  .cin_o    ( booth_fCin_f1a_h     ),
  .pp1_o    ( pp1_f1a_h            ),
  .pp2_o    ( pp2_f1a_h            ),
  .pp3_o    ( pp3_f1a_h            ),
  .pp4_o    ( pp4_f1a_h            ),
  .pp5_o    ( pp5_f1a_h            ),
  .pp6_o    ( pp6_f1a_h            ),
  .pp7_o    ( pp7_f1a_h            ),
  .pp8_o    ( pp8_f1a_h            ),
  .pp9_o    ( pp9_f1a_h            ),
  .pp10_o   ( pp10_f1a_h           ),
  .pp11_o   ( pp11_f1a_h           ),
  .pp12_o   ( pp12_f1a_h           ),
  .pp13_o   ( pp13_f1a_h           ),
  .pp14_o   ( pp14_f1a_h           ),
  .pp15_o   ( pp15_f1a_h           ),
  .pp16_o   ( pp16_f1a_h           ),
  .pp17_o   ( booth_fpp17_f1a_h    )
);


///////////////////////////////////////////////////////////////////////////////
// Wallace1
///////////////////////////////////////////////////////////////////////////////
txfma_wallace1
wallace1
(
  .pp1_i    ( pp1_f1a_h          ),
  .pp2_i    ( pp2_f1a_h          ),
  .pp3_i    ( pp3_f1a_h          ),
  .pp4_i    ( pp4_f1a_h          ),
  .pp5_i    ( pp5_f1a_h          ),
  .pp6_i    ( pp6_f1a_h          ),
  .pp7_i    ( pp7_f1a_h          ),
  .pp8_i    ( pp8_f1a_h          ),
  .pp9_i    ( pp9_f1a_h          ),
  .pp10_i   ( pp10_f1a_h         ),
  .pp11_i   ( pp11_f1a_h         ),
  .pp12_i   ( pp12_f1a_h         ),
  .pp13_i   ( pp13_f1a_h         ),
  .pp14_i   ( pp14_f1a_h         ),
  .pp15_i   ( pp15_f1a_h         ),
  .pp16_i   ( pp16_f1a_h         ),
  .mode_i   ( mul_hp_mode_f1a_h  ),
  .c21_o    ( wallace_fC21_f1a_h ),
  .c22_o    ( wallace_fC22_f1a_h ),
  .s21_o    ( wallace_fS21_f1a_h ),
  .s22_o    ( wallace_fS22_f1a_h )
);

///////////////////////////////////////////////////////////////////////////////
// TRZ
///////////////////////////////////////////////////////////////////////////////

assign fa_lo_ps_trz_frac_mux =  dtps_f1a_h ? fa_f1a_h[FLOAT32_W-1:FLOAT32_W-FLOAT32_M-1]
                             :               {13'b0,fa_f1a_h[11:1]};

// For PS and PH_LOW
txfma_trz
#(
  .TrzOut ( 5                   ),
  .Max    ( FLOAT32_M          )
)
fa_lo_ps_trz
(
  .frac_i ( fa_lo_ps_trz_frac_mux ),
  .trz_o  ( etrz_fa_lo_ps_f1a_h   )
);

  //mux to select frac

always_comb begin

  fb_lo_ps_trz_frac_mux = {13'h0, fb_f1a_h[11:1]};

  if (dtps_f1a_h)
    fb_lo_ps_trz_frac_mux = fb_f1a_h[FLOAT32_W-3:FLOAT32_W-3-FLOAT32_M];

end

txfma_trz
#(
  .TrzOut ( 5                   ),
  .Max    ( FLOAT32_M          )
)
fb_lo_ps_trz
(
  .frac_i ( fb_lo_ps_trz_frac_mux ),
  .trz_o  ( etrz_fb_lo_ps_f1a_h   )
);


txfma_trz
#(
  .TrzOut ( 5                 ),
  .Max    ( FLOAT32_M        )
)
fc_lo_ps_trz
(
  .frac_i ( fc_f1a_h[FLOAT32_W-1:FLOAT32_W-FLOAT32_M-1] ),
  .trz_o  ( etrz_fc_f1a_h                                  )
);


assign fa_hi_ph_trz_frac_mux = fa_f1a_h[31:21];


// For PH_HIGH
txfma_trz
#(
  .TrzOut ( 4                 ),
  .Max    ( FLOAT16_M        )
)
fa_hi_ph_trz
(
  .frac_i ( fa_hi_ph_trz_frac_mux ),
  .trz_o  ( etrz_fa_hi_ph_f1a_h   )
);

assign fb_hi_ph_trz_frac_mux = fb_f1a_h[29:19];

txfma_trz
#(
  .TrzOut ( 4                 ),
  .Max    ( FLOAT16_M        )
)
fb_hi_ph_trz
(
  .frac_i ( fb_hi_ph_trz_frac_mux ),
  .trz_o  ( etrz_fb_hi_ph_f1a_h   )
);

endmodule

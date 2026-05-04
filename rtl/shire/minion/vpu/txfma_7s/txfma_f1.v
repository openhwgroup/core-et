// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module txfma_f1 
(
  input                                   ctrl_f0_clk,
  input                                   multype_f0_clk,  
  input                                   ctrl_f1_clk,
  input                                   multype_f1_clk,
  input  [`VPU_DATA_S_SZ-1:0]             fa_f0a_h,
  input  [`VPU_DATA_S_SZ-1:0]             fb_f0a_h,
  input  [`VPU_DATA_S_SZ-1:0]             fc_f0a_h,
  input                                   dtps_f0a_h,
  input                                   mul_sign_f0a_h,
  input                                   mul_hp_mode_f0a_h,
  output logic                            fc_b0_f2a_h,
  output logic [`TXFMA_A3:`TXFMA_B27]     fc_f2a_h,
  output logic [`TXFMA_A3:`TXFMA_B27]     fa_f2a_h,
  output logic [`TXFMA_A3:`TXFMA_B59]     wallace_fC21_f2a_h,
  output logic [`TXFMA_A3:`TXFMA_B59]     wallace_fC22_f2a_h,
  output logic [`TXFMA_A3:`TXFMA_B59]     wallace_fS21_f2a_h,
  output logic [`TXFMA_A3:`TXFMA_B59]     wallace_fS22_f2a_h,
  output logic [`VPU_DATA_S_SZ-1:0]       booth_fCin_f2a_h,
  output logic [`VPU_DATA_S_SZ-1:0]       booth_fSxt_f2a_h,
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
logic [`VPU_DATA_S_SZ-1:0]    fa_f1a_h;
logic [`VPU_DATA_S_SZ-1:0]    fb_f1a_h;
logic [`VPU_DATA_S_SZ-1:0]    fc_f1a_h;
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
logic [`FLOAT32_M:0]          fa_lo_ps_trz_frac_mux;
logic [`FLOAT32_M:0]          fb_lo_ps_trz_frac_mux;
logic [`FLOAT16_M:0]          fa_hi_ph_trz_frac_mux;
logic [`FLOAT16_M:0]          fb_hi_ph_trz_frac_mux;
logic [`TXFMA_A3:`TXFMA_B59]  wallace_fC21_f1a_h;
logic [`TXFMA_A3:`TXFMA_B59]  wallace_fC22_f1a_h;
logic [`TXFMA_A3:`TXFMA_B59]  wallace_fS21_f1a_h;
logic [`TXFMA_A3:`TXFMA_B59]  wallace_fS22_f1a_h;
logic [`VPU_DATA_S_SZ-1:0]    booth_fCin_f1a_h;
logic [`VPU_DATA_S_SZ-1:0]    booth_fSxt_f1a_h;
logic [32:0]                  booth_fpp17_f1a_h;
logic [`TXFMA_A3:`TXFMA_B27]  fc_shift_f1a_h;
logic [`TXFMA_A3:`TXFMA_B27]  fa_shift_f1a_h;


///////////////////////////////////////////////////////////////////////////////
// Local Flops - Needed to guarantee timing 
///////////////////////////////////////////////////////////////////////////////

//     CLK             DOUT                 DIN
  `FF (ctrl_f0_clk,    fa_f1a_h,            fa_f0a_h               )
  `FF (ctrl_f0_clk,    fb_f1a_h,            fb_f0a_h               )
  `FF (ctrl_f0_clk,    fc_f1a_h,            fc_f0a_h               )
  `FF (ctrl_f0_clk,    dtps_f1a_h,          dtps_f0a_h             )
  `FF (multype_f0_clk, mul_sign_f1a_h,      mul_sign_f0a_h         )
  `FF (multype_f0_clk, mul_hp_mode_f1a_h,   mul_hp_mode_f0a_h      )
  `FF (ctrl_f1_clk,    fa_f2a_h,            fa_shift_f1a_h         )
  `FF (ctrl_f1_clk,    fc_f2a_h,            fc_shift_f1a_h         )
  `FF (multype_f1_clk, wallace_fC21_f2a_h,  wallace_fC21_f1a_h     )
  `FF (multype_f1_clk, wallace_fC22_f2a_h,  wallace_fC22_f1a_h     )
  `FF (multype_f1_clk, wallace_fS21_f2a_h,  wallace_fS21_f1a_h     )
  `FF (multype_f1_clk, wallace_fS22_f2a_h,  wallace_fS22_f1a_h     )
  `FF (multype_f1_clk, booth_fCin_f2a_h,    booth_fCin_f1a_h       )
  `FF (multype_f1_clk, booth_fSxt_f2a_h,    booth_fSxt_f1a_h       )
  `FF (multype_f1_clk, booth_fpp17_f2a_h,   booth_fpp17_f1a_h      )


///////////////////////////////////////////////////////////////////////////////
// Local manipulation of Operands 
///////////////////////////////////////////////////////////////////////////////
assign fa_shift_f1a_h = fa_f1a_h;
assign fc_shift_f1a_h = fc_f1a_h;
assign fc_b0_f2a_h    = fc_f2a_h[`TXFMA_B0+3]; // b0 is now in bit 62


///////////////////////////////////////////////////////////////////////////////
// Booth PPG 32R4 +  Sign extend correction + 2'complement correction
///////////////////////////////////////////////////////////////////////////////
txfma_booth_ppg_32r4 
ppg
(
  .mode     ( mul_hp_mode_f1a_h    ),
  .mplr     ( fa_f1a_h             ), 
  .mpcnd    ( fb_f1a_h             ), 
  .sign     ( mul_sign_f1a_h       ),
  .Sxt      ( booth_fSxt_f1a_h     ),
  .Cin      ( booth_fCin_f1a_h     ), 
  .pp1      ( pp1_f1a_h            ), 
  .pp2      ( pp2_f1a_h            ), 
  .pp3      ( pp3_f1a_h            ),
  .pp4      ( pp4_f1a_h            ), 
  .pp5      ( pp5_f1a_h            ), 
  .pp6      ( pp6_f1a_h            ), 
  .pp7      ( pp7_f1a_h            ),
  .pp8      ( pp8_f1a_h            ), 
  .pp9      ( pp9_f1a_h            ), 
  .pp10     ( pp10_f1a_h           ), 
  .pp11     ( pp11_f1a_h           ),
  .pp12     ( pp12_f1a_h           ), 
  .pp13     ( pp13_f1a_h           ), 
  .pp14     ( pp14_f1a_h           ), 
  .pp15     ( pp15_f1a_h           ),
  .pp16     ( pp16_f1a_h           ),
  .pp17     ( booth_fpp17_f1a_h    )
);


///////////////////////////////////////////////////////////////////////////////
// Wallace1
///////////////////////////////////////////////////////////////////////////////
txfma_wallace1 
wallace1
(
  .pp1      ( pp1_f1a_h          ),
  .pp2      ( pp2_f1a_h          ),
  .pp3      ( pp3_f1a_h          ),
  .pp4      ( pp4_f1a_h          ),
  .pp5      ( pp5_f1a_h          ),
  .pp6      ( pp6_f1a_h          ),
  .pp7      ( pp7_f1a_h          ),
  .pp8      ( pp8_f1a_h          ),
  .pp9      ( pp9_f1a_h          ),
  .pp10     ( pp10_f1a_h         ),
  .pp11     ( pp11_f1a_h         ),
  .pp12     ( pp12_f1a_h         ),
  .pp13     ( pp13_f1a_h         ),
  .pp14     ( pp14_f1a_h         ),
  .pp15     ( pp15_f1a_h         ),
  .pp16     ( pp16_f1a_h         ),
  .mode     ( mul_hp_mode_f1a_h  ),  
  .C21      ( wallace_fC21_f1a_h ),
  .C22      ( wallace_fC22_f1a_h ),  
  .S21      ( wallace_fS21_f1a_h ),
  .S22      ( wallace_fS22_f1a_h )  
);

///////////////////////////////////////////////////////////////////////////////
// TRZ
///////////////////////////////////////////////////////////////////////////////

assign fa_lo_ps_trz_frac_mux =  dtps_f1a_h ? fa_f1a_h[`FLOAT32_W-1:`FLOAT32_W-`FLOAT32_M-1] 
                             :               {13'b0,fa_f1a_h[11:1]};

// For PS and PH_LOW
txfma_trz
#(
  .trz_out  ( 5                   ),
  .max      ( `FLOAT32_M          )
)
fa_lo_ps_trz
(
  .frac   ( fa_lo_ps_trz_frac_mux ),
  .trz    ( etrz_fa_lo_ps_f1a_h   )
);

  //mux to select frac

always_comb begin

  fb_lo_ps_trz_frac_mux = {13'h0, fb_f1a_h[11:1]}; 

  if (dtps_f1a_h)
    fb_lo_ps_trz_frac_mux = fb_f1a_h[`FLOAT32_W-3:`FLOAT32_W-3-`FLOAT32_M];

end

txfma_trz 
#(
  .trz_out  ( 5                   ),
  .max      ( `FLOAT32_M          )
)
fb_lo_ps_trz
(
  .frac   ( fb_lo_ps_trz_frac_mux ), 
  .trz    ( etrz_fb_lo_ps_f1a_h   )
);


txfma_trz 
#(
  .trz_out  ( 5                 ),
  .max      ( `FLOAT32_M        )
)
fc_lo_ps_trz
(
  .frac   ( fc_f1a_h[`FLOAT32_W-1:`FLOAT32_W-`FLOAT32_M-1] ),
  .trz    ( etrz_fc_f1a_h                                  )
);


assign fa_hi_ph_trz_frac_mux = fa_f1a_h[31:21];


// For PH_HIGH
txfma_trz
#(
  .trz_out  ( 4                 ),
  .max      ( `FLOAT16_M        )
)
fa_hi_ph_trz
(
  .frac   ( fa_hi_ph_trz_frac_mux ),
  .trz    ( etrz_fa_hi_ph_f1a_h   )
);

assign fb_hi_ph_trz_frac_mux = fb_f1a_h[29:19];

txfma_trz 
#(
  .trz_out  ( 4                 ),
  .max      ( `FLOAT16_M        )
)
fb_hi_ph_trz
(
  .frac   ( fb_hi_ph_trz_frac_mux ),
  .trz    ( etrz_fb_hi_ph_f1a_h   )
);

endmodule

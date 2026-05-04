// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module txfma_f1 
(
  input  [`VPU_DTYPE_SZ-1:0]              op_dtype_f1a_h,  
  input                                   mul_sign_f1a_h,
  input  [`VPU_DATA_S_SZ-1:0]             fa_f1a_h,
  input  [`VPU_DATA_S_SZ-1:0]             fb_f1a_h,
  input  [`VPU_DATA_S_SZ-1:0]             fc_f1a_h,
  output logic [`TXFMA_A3:0]              wallace_fC21_f1a_h,
  output logic [`TXFMA_A3:0]              wallace_fC22_f1a_h,
  output logic [`TXFMA_A3:0]              wallace_fS21_f1a_h,
  output logic [`TXFMA_A3:0]              wallace_fS22_f1a_h,
  output logic [`VPU_DATA_S_SZ-1:0]       booth_fCin_f1a_h,
  output logic [`VPU_DATA_S_SZ-1:0]       booth_fSxt_f1a_h,
  output logic [32:0]                     booth_fpp17_f1a_h,
  output logic [`TXFMA_ETRZ_PH_SZ-1:0]    etrz_fa_hi_ph_f1a_h,
  output logic [`TXFMA_ETRZ_PS_SZ-1:0]    etrz_fa_lo_ps_f1a_h,
  output logic [`TXFMA_ETRZ_PH_SZ-1:0]    etrz_fb_hi_ph_f1a_h,
  output logic [`TXFMA_ETRZ_PS_SZ-1:0]    etrz_fb_lo_ps_f1a_h,
  output logic [`TXFMA_ETRZ_PS_SZ-1:0]    etrz_fc_lo_ps_f1a_h
);

///////////////////////////////////////////////////////////////////////////////
// Internal logic
///////////////////////////////////////////////////////////////////////////////
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


///////////////////////////////////////////////////////////////////////////////
// Booth PPG 32R4 +  Sign extend correction + 2'complement correction
///////////////////////////////////////////////////////////////////////////////
txfma_booth_ppg_32r4 
ppg
(
  .op_dtype ( op_dtype_f1a_h    ),
  .mplr     ( fa_f1a_h          ), 
  .mpcnd    ( fb_f1a_h          ), 
  .sign     ( mul_sign_f1a_h    ),
  .Sxt      ( booth_fSxt_f1a_h  ),
  .Cin      ( booth_fCin_f1a_h  ), 
  .pp1      ( pp1_f1a_h         ), 
  .pp2      ( pp2_f1a_h         ), 
  .pp3      ( pp3_f1a_h         ),
  .pp4      ( pp4_f1a_h         ), 
  .pp5      ( pp5_f1a_h         ), 
  .pp6      ( pp6_f1a_h         ), 
  .pp7      ( pp7_f1a_h         ),
  .pp8      ( pp8_f1a_h         ), 
  .pp9      ( pp9_f1a_h         ), 
  .pp10     ( pp10_f1a_h        ), 
  .pp11     ( pp11_f1a_h        ),
  .pp12     ( pp12_f1a_h        ), 
  .pp13     ( pp13_f1a_h        ), 
  .pp14     ( pp14_f1a_h        ), 
  .pp15     ( pp15_f1a_h        ),
  .pp16     ( pp16_f1a_h        ),
  .pp17     ( booth_fpp17_f1a_h )
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
  .op_dtype ( op_dtype_f1a_h     ),  
  .C21      ( wallace_fC21_f1a_h ),
  .C22      ( wallace_fC22_f1a_h ),  
  .S21      ( wallace_fS21_f1a_h ),
  .S22      ( wallace_fS22_f1a_h )  
);

///////////////////////////////////////////////////////////////////////////////
// TRZ
///////////////////////////////////////////////////////////////////////////////

  //mux to select frac 
assign fa_lo_ps_trz_frac_mux = (op_dtype_f1a_h==`VPU_DTYPE_F32) ? fa_f1a_h[`FLOAT32_W-1:`FLOAT32_W-`FLOAT32_M-1]
                             :                                    fa_f1a_h[11:1]; 

// For PS and PH_LOW
txfma_trz
#(
  .trz_out  ( `TXFMA_ETRZ_PS_SZ ),
  .max      ( `FLOAT32_M        )
)
fa_lo_ps_trz
(
  .frac   ( fa_lo_ps_trz_frac_mux ),
  .trz    ( etrz_fa_lo_ps_f1a_h   )
);

  //mux to select frac
always_comb begin

  fb_lo_ps_trz_frac_mux = {13'h0, fb_f1a_h[11:1]}; 

    //undo 2 bits shift
  if (op_dtype_f1a_h==`VPU_DTYPE_F32)
    fb_lo_ps_trz_frac_mux = fb_f1a_h[`FLOAT32_W-3:`FLOAT32_W-3-`FLOAT32_M];
end

txfma_trz 
#(
  .trz_out  ( `TXFMA_ETRZ_PS_SZ ),
  .max      ( `FLOAT32_M        )
)
fb_lo_ps_trz
(
  .frac   ( fb_lo_ps_trz_frac_mux ), 
  .trz    ( etrz_fb_lo_ps_f1a_h   )
);


txfma_trz 
#(
  .trz_out  ( `TXFMA_ETRZ_PS_SZ ),
  .max      ( `FLOAT32_M        )
)
fc_lo_ps_trz
(
  .frac   ( fc_f1a_h[`FLOAT32_W-1:`FLOAT32_W-`FLOAT32_M-1] ),
  .trz    ( etrz_fc_lo_ps_f1a_h                            )
);


  //mux to select frac
assign fa_hi_ph_trz_frac_mux = (op_dtype_f1a_h==`VPU_DTYPE_F32) ? fa_f1a_h[`FLOAT32_W-1:`FLOAT32_W-`FLOAT32_M-1]
                             :                                    fa_f1a_h[31:21]; 

// For PH_HIGH
txfma_trz
#(
  .trz_out  ( `TXFMA_ETRZ_PH_SZ ),
  .max      ( `FLOAT16_M        )
)
fa_hi_ph_trz
(
  .frac   ( fa_hi_ph_trz_frac_mux ),
  .trz    ( etrz_fa_hi_ph_f1a_h   )
);

  //mux to select frac
assign fb_hi_ph_trz_frac_mux = (op_dtype_f1a_h==`VPU_DTYPE_F32) ? fb_f1a_h[`FLOAT32_W-1:`FLOAT32_W-`FLOAT32_M-1]
                             :                                    fb_f1a_h[29:19]; 

txfma_trz 
#(
  .trz_out  ( `TXFMA_ETRZ_PH_SZ ),
  .max      ( `FLOAT16_M        )
)
fb_hi_ph_trz
(
  .frac   ( fb_hi_ph_trz_frac_mux ),
  .trz    ( etrz_fb_hi_ph_f1a_h   )
);

endmodule

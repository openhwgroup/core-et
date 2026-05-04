// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module txfma_f5 
(
  input                                     a_inf_f5a_h,
  input                                     eff_sub1_f5a_h,
  input         [`TXFMA_RM_SZ-1:0]          rm_f5a_h,
  input                                     cmp_invalid_f5a_h,
  input                                     norm_f32_ovf_f5a_h,
  input                                     norm_one_out_f5a_h,
  input                                     res_unorm_one_f5a_h,
  input                                     res_snorm_one_f5a_h,  
  input                                     ec_zero_f5a_h,
  input                                     ediff1_lt_31_f5a_h,
  input                                     cvt_fui_pos_ovf_f5a_h,
  input                                     res_uint_nan_f5a_h,
  input                                     res_zero_f5a_h,
  input                                     res_one_f5a_h,
  input                                     cvt_fi_pos_ovf_f5a_h,
  input                                     cvt_fi_neg_ovf_f5a_h,
  input                                     cvt_fi_min_neg_f5a_h,
  input                                     res_pos_int_nan_f5a_h,
  input                                     res_neg_int_nan_f5a_h, 
  input                                     minmax_op_f5a_h,
  input                                     rmode_f5a_h,
  input                                     comp_op_f5a_h,
  input                                     rnd_ovf_f5a_h,
  input                                     pot_ovf_f5a_h,
  input                                     def_ovf_f5a_h,  
  input                                     pot_unf_f5a_h,
  input                                     def_unf_f5a_h,
  input         [`VPU_FLAGS_SZ-1:0]         in_flags_f5a_h,
  input         [`VPU_FCMD_SZ-1:0]          cmd_f5a_h,
  input                                     res_inf_f5a_h,
  input                                     res_snan_f5a_h,
  input                                     res_qnan_f5a_h,
  input         [`FP_CLASS_BITS-1:0]        classify_f5a_h,
  input         [`VPU_DTYPE_SZ-1:0]         op_dtype_f5a_h,
  input         [`TXFMA_SIGS_SZ-1:0]        sigs_f5a_h,    
  input         [`TXFMA_A0:`TXFMA_B31]      mergd_frac_res_f5a_h,
  input         [`TXFMA_EXP_SEXT_PS_SZ-1:0] e_res_f5a_h,
  input         [`TXFMA_RND_CTL_SZ-1:0]     rnd_ctl_f5a_h,
  input                                     sticky_until_l_f5a_h,
  input         [`TXFMA_FTZ_SZ-1:0]         ftz_f5a_h,
  input                                     s_res_f5a_h,
  input                                     flags_en_f5a_h,
  input                                     spec_res_en_f5a_h,
  input                                     sel_gmaxu24_f5a_h,
  input                                     sel_gmaxu16_f5a_h,
  input                                     sel_gmaxu10_f5a_h,
  input                                     sel_gmaxu8_f5a_h,
  input                                     sel_gmaxu2_f5a_h,
  input                                     sel_gminany_f5a_h,
  input                                     sel_gmaxs16_f5a_h,
  input                                     sel_gmins16_f5a_h,
  input                                     sel_gmaxs8_f5a_h,
  input                                     sel_gmins8_f5a_h,
  output logic  [`VPU_DATA_S_SZ-1:0]        fma_data_f5a_h,
  output logic  [`VPU_FLAGS_SZ-1:0]         fma_flags_f5a_h
);

///////////////////////////////////////////////////////////////////////////////
// Internal logic
///////////////////////////////////////////////////////////////////////////////
logic [`TXFMA_A0:`TXFMA_B30]  rnd_frac_res_f5a_h;
logic frac_zero_hi_f5a_h;
logic frac_zero_hi_1_f5a_h;
logic frac_zero_hi_2_f5a_h;

///////////////////////////////////////////////////////////////////////////////
// round adder hi
///////////////////////////////////////////////////////////////////////////////
txfma_rnd_adder
#(
  .left_bit     ( `TXFMA_A0            ),
  .right_bit    ( `TXFMA_B31           ),
  .rnd_ctl_sz   ( `TXFMA_RND_CTL_HI_SZ ),
  .ftz_sz       ( `TXFMA_FTZ_HI_SZ     ),
  .rnd_ps       ( 1                    )
)
rnd_adder_hi
(
  .in               ( mergd_frac_res_f5a_h ),
  .rnd_ctl          ( rnd_ctl_f5a_h           ),
  .ftz              ( ftz_f5a_h               ),
  .out              ( rnd_frac_res_f5a_h   )
);


///////////////////////////////////////////////////////////////////////////////
// result mux 
///////////////////////////////////////////////////////////////////////////////
txfma_result_mux result_mux 
(
  .a_inf                ( a_inf_f5a_h                           ),
  .eff_sub1             ( eff_sub1_f5a_h                        ),
  .rm                   ( rm_f5a_h                              ),
  .cmp_invalid          ( cmp_invalid_f5a_h                     ),
  .norm_f32_ovf         ( norm_f32_ovf_f5a_h                   	),
  .norm_one_out       	( norm_one_out_f5a_h                   	),
  .res_unorm_one_in   	( res_unorm_one_f5a_h                  	),
  .res_snorm_one_in   	( res_snorm_one_f5a_h                  	),  
  .cmd                	( cmd_f5a_h                            	),
  .op_dtype           	( op_dtype_f5a_h                       	),
  .classify        	    ( classify_f5a_h                    	),
  .ec_zero_hi         	( ec_zero_f5a_h                     	),
  .ediff_lt_31_hi     	( ediff1_lt_31_f5a_h                   	),
  .in_flags        	    ( in_flags_f5a_h                    	),
  .res_uint_nan       	( res_uint_nan_f5a_h                	),
  .res_zero    	        ( res_zero_f5a_h                	    ),
  .res_one              ( res_one_f5a_h                         ),  
  .cvt_fui_pos_ovf_hi 	( cvt_fui_pos_ovf_f5a_h                	),
  .cvt_fi_pos_ovf_hi  	( cvt_fi_pos_ovf_f5a_h                 	),
  .cvt_fi_neg_ovf_hi  	( cvt_fi_neg_ovf_f5a_h                 	),
  .cvt_fi_min_neg_hi  	( cvt_fi_min_neg_f5a_h                 	),
  .res_pos_int_nan 	    ( res_pos_int_nan_f5a_h             	),
  .res_neg_int_nan    	( res_neg_int_nan_f5a_h             	),
  .frac_zero_hi       	( frac_zero_hi_f5a_h                   	),
  .minmax_op          	( minmax_op_f5a_h                      	),
  .comp_op            	( comp_op_f5a_h                        	),
  .res_inf            	( res_inf_f5a_h                     	),
  .sigs               	( sigs_f5a_h                           	),
  .s_res             	( s_res_f5a_h                       	),
  .e_res            	( e_res_f5a_h[`TXFMA_EXP_PS_SZ-1:0] 	),
  .res_snan         	( res_snan_f5a_h                    	),
  .res_qnan         	( res_qnan_f5a_h                    	),
  .rmode              	( rmode_f5a_h                          	),
  .pot_ovf         	    ( pot_ovf_f5a_h                     	),
  .def_ovf         	    ( def_ovf_f5a_h                     	), 
  .pot_unf              ( pot_unf_f5a_h                     	),
  .def_unf              ( def_unf_f5a_h                     	),   
  .rnd_frac_res    	    ( rnd_frac_res_f5a_h                	),
  .sticky_until_l  	    ( sticky_until_l_f5a_h              	),
  .rnd_ovf_hi         	( rnd_ovf_f5a_h                     	),
  .flags_en           	( flags_en_f5a_h                       	),
  .spec_res_en        	( spec_res_en_f5a_h                    	),
  .sel_gmaxu24          ( sel_gmaxu24_f5a_h                     ),
  .sel_gmaxu16          ( sel_gmaxu16_f5a_h                     ),
  .sel_gmaxu10          ( sel_gmaxu10_f5a_h                     ),
  .sel_gmaxu8           ( sel_gmaxu8_f5a_h                      ),
  .sel_gmaxu2           ( sel_gmaxu2_f5a_h                      ),
  .sel_gminany          ( sel_gminany_f5a_h                     ),
  .sel_gmaxs16          ( sel_gmaxs16_f5a_h                     ),
  .sel_gmins16          ( sel_gmins16_f5a_h                     ),
  .sel_gmaxs8           ( sel_gmaxs8_f5a_h                      ),
  .sel_gmins8           ( sel_gmins8_f5a_h                      ),
  .fma_res            	( fma_data_f5a_h                       	),
  .out_flags          	( fma_flags_f5a_h                      	)
);

///////////////////////////////////////////////////////////////////////////////
// Zero frac detect
//////////////////////////////////////////////////////////////////////////////

txfma_frac_zero_detect
#(
  .left_bit     ( `TXFMA_A0  ),
  .right_bit    ( `TXFMA_B10 )
)
frac_zero_detect_hi_1
(
  .in_data ( mergd_frac_res_f5a_h[`TXFMA_A0:`TXFMA_B10] ),
  .zero    ( frac_zero_hi_1_f5a_h                          )
);

txfma_frac_zero_detect
#(
  .left_bit     ( `TXFMA_B11 ),
  .right_bit    ( `TXFMA_B31 )
)
frac_zero_detect_hi_2
(
  .in_data ( mergd_frac_res_f5a_h[`TXFMA_B11:`TXFMA_B31] ),
  .zero    ( frac_zero_hi_2_f5a_h                           )
);

assign  frac_zero_hi_f5a_h = frac_zero_hi_1_f5a_h & frac_zero_hi_2_f5a_h;   

endmodule

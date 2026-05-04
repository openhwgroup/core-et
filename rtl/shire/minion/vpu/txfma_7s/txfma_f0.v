// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module txfma_f0 
(
  input                                 opa_frf_clk,
  input                                 opb_frf_clk,
  input                                 opc_frf_clk,
  input                                 trans_frf_clk,
  input                                 ctrl_frf_clk,
  input [`VPU_DATA_S_SZ-1:0]            opa_byp_frfa_h,
  input [`VPU_DATA_S_SZ-1:0]            opc_byp_frfa_h,
  input [`VPU_DATA_S_SZ-1:0]            opb_byp_frfa_h,
  input [`TRANS_C0_MAX_SIZE-1:0]        xrom_c1c0k_ra_h, 
  input [`TRANS_C2_MAX_SIZE-1:0]        xrom_c2_ra_h,    
  input [`TRANS_MASK_SIZE-1:0]          trans_mask_ra_h,     
  input [5:0]                           eb_hi_k_exa_h,
  input [1:0]                           sel_ebin_hi_exa_h,
  input [1:0]                           sel_eainmux_exa_h,
  input [1:0]                           sel_ec5b_exa_h,
  input [1:0]                           sel_ecin_exa_h,
  input	             			        sel_ec5bdet_exa_h,
  input [1:0]                           sel_sc_exa_h,
  input [1:0]                           sel_opcgfx_exa_h,
  input [1:0]                           sel_xopc_exa_h,
  input [1:0]                           sel_opcfmt_exa_h,
  input [1:0]                           sel_transfa_exa_h,
  input [1:0]                           sel_transfb_exa_h,
  input [1:0]                           sel_opafmt_exa_h,
  input [1:0]                           sel_opbfmt_exa_h,
  input                                 sel_eblo_ph_exa_h,
  input                                 elo_eb_en_exa_h,
  input                                 ehi_eb_en_exa_h,
  input                                 elo_ea_en_exa_h,
  input                                 fa_en_exa_h,
  input                                 fb_en_exa_h,
  input                                 fc_en_exa_h,
  input                                 sel_ealo_ph_exa_h,
  input                                 sa_hi_en_exa_h,
  input                                 sa_lo_en_exa_h,
  input                                 sb_hi_en_exa_h,
  input                                 sb_lo_en_exa_h,
  input                                 sc_en_exa_h,
  input                                 ehi_ea_en_exa_h,
  input                                 ec_en_exa_h,
  input                                 neg_fmaop_exa_h,
  input                                 trans_op2_exa_l,
  input                                 sa1_nr2_frcpfxp_exa_h,
  input                                 sel_sign_sinp2_exa_h,
  input                                 sa_zap_exa_l,
  input                                 sb_zap_exa_l,
  input                                 sel_f10_exa_h,
  input                                 sel_f11_exa_h,
  input                                 fc_dtps_exa_h,
  input                                 sel_snorm_exa_h,
  input                                 sel_g1sk2_exa_h,
  input                                 sel_g1sk8_exa_h,
  input                                 sel_g1sk16_exa_h,
  input                                 sel_g1sk24_exa_h,
  input                                 sel_xexpfrac_exa_h,
  output logic                         	sa_hi_f0a_h,
  output logic                         	sa_lo_f0a_h,
  output logic                         	sb_hi_f0a_h,
  output logic                         	sb_lo_f0a_h,
  output logic                         	sc_f0a_h,
  output logic [`TXFMA_EXP_PS_SZ-1:0]  	ea_hi_f0a_h,
  output logic [`TXFMA_EXP_PH_SZ-1:0]  	ea_lo_f0a_h,
  output logic [`TXFMA_EXP_PS_SZ-1:0]  	eb_hi_f0a_h,
  output logic [`TXFMA_EXP_PH_SZ-1:0]  	eb_lo_f0a_h,
  output logic [`TXFMA_EXP_PS_SZ-1:0]  	ec_f0a_h,
  output logic [`VPU_DATA_S_SZ-1:0]    	fa_f0a_h,
  output logic [`VPU_DATA_S_SZ-1:0]    	fb_f0a_h,
  output logic [`VPU_DATA_S_SZ-1:0]    	fc_f0a_h,
  output logic				            ea_hi_zero_f0a_l,
  output logic                         	ea_hi_all1_f0a_h,
  output logic				            ea_lo_zero_f0a_l,
  output logic                         	ea_lo_all1_f0a_h,
  output logic				            eb_hi_zero_f0a_l,
  output logic                         	eb_hi_all1_f0a_h,
  output logic				            eb_lo_zero_f0a_l,
  output logic                         	eb_lo_all1_f0a_h,
  output logic				            ec_zero_f0a_l,
  output logic                         	ec_all1_f0a_h,
  output logic                          fcf10_zero_f0a_l,
  output logic                          eb_eql_127_f0a_h,
  output logic                          eb_eql_126_f0a_h,
  output logic                          eb_eql_125_f0a_h,
  output logic                          fcf11_zero_f0a_l,
  output logic                          fcps_zero_f0a_h,
  output logic                          fcph_zero_f0a_l,
  output logic                          fcnorm_zero_29_8_f0a_h,
  output logic                          fcnorm16_eq_1_f0a_h,
  output logic                          fcnorm8_eq_1_f0a_h,
  output logic                          fcn8_one_30_24_f0a_h,
  output logic                          fcn10_one_30_22_f0a_h,
  output logic                          fcn16_one_30_16_f0a_h,
  output logic                          fcn24_one_30_8_f0a_h,
  output logic                          faps_zero_f0a_l,
  output logic                          fbps_zero_f0a_l,
  output logic                          faph_hi_zero_f0a_l,
  output logic                          fbph_hi_zero_f0a_l,
  output logic                          faph_lo_zero_f0a_l,
  output logic                          fbph_lo_zero_f0a_l,
  output logic                          faph_lo_b0_f0a_h,
  output logic                          fbph_lo_b0_f0a_h,
  output logic                          faps_b0_f0a_h,
  output logic                          fbps_b0_f0a_h,
  output logic                          fc_31_f0a_h,
  output logic                          fcnorm_30_f0a_h,
  output logic                          faph_hi_b0_f0a_h,
  output logic                          fbph_hi_b0_f0a_h,
  output logic                          fcph_b0_f0a_h,
  output logic                          fcf11_b0_f0a_h,
  output logic                          fcf10_b0_f0a_h,
  output logic                          fcps_b0_f0a_h
);

//======================================================================================
// Local signals 
//====================================================================================== 

logic                           sa_raw_hi_f0a_h;
logic                           sa_raw_lo_f0a_h;
logic                           sb_raw_hi_f0a_h;
logic                           sb_raw_lo_f0a_h;
logic                           sc_raw_f0a_h;

logic                           sel_ec5bdet_f0a_h;
logic [5:0]                     eb_hi_k_f0a_h;
logic [1:0]                     sel_ebin_hi_f0a_h;
logic [1:0]                     sel_eainmux_f0a_h;
logic [1:0]                     sel_ec5b_f0a_h;
logic [1:0]                     sel_ecin_f0a_h;
logic [1:0]                     sel_sc_f0a_h;
logic [1:0]                     sel_opcgfx_f0a_h;
logic [1:0]                     sel_xopc_f0a_h;
logic [1:0]                     sel_opcfmt_f0a_h;
logic [1:0]                     sel_transfa_f0a_h;
logic [1:0]                     sel_transfb_f0a_h;
logic [1:0]                     sel_opafmt_f0a_h;
logic [1:0]                     sel_opbfmt_f0a_h;
logic                           sel_eblo_ph_f0a_h;
logic                           elo_eb_en_f0a_h;
logic                           ehi_eb_en_f0a_h;
logic                           elo_ea_en_f0a_h;
logic                           fa_en_f0a_h;
logic                           fb_en_f0a_h;
logic                           fc_en_f0a_h;
logic                           sel_ealo_ph_f0a_h;
logic                           sa_hi_en_f0a_h;
logic                           sa_lo_en_f0a_h;
logic                           sb_hi_en_f0a_h;
logic                           sb_lo_en_f0a_h;
logic                           sc_en_f0a_h;
logic                           ehi_ea_en_f0a_h;
logic                           ec_en_f0a_h;
logic                           neg_fmaop_f0a_h;
logic                           trans_op2_f0a_l;
logic                           sa1_nr2_frcpfxp_f0a_h;
logic                           sel_sign_sinp2_f0a_h;
logic                           sa_zap_f0a_l;
logic                           sb_zap_f0a_l;
logic                           sel_f10_f0a_h;
logic                           sel_f11_f0a_h;
logic                           fc_dtps_f0a_h;
logic                           sel_snorm_f0a_h;
logic                           sel_g1sk2_f0a_h;
logic                           sel_g1sk8_f0a_h;
logic                           sel_g1sk16_f0a_h;
logic                           sel_g1sk24_f0a_h;
logic                           sel_xexpfrac_f0a_h;

logic [`VPU_DATA_S_SZ-1:0]    	opa_f0a_h;
logic [`VPU_DATA_S_SZ-1:0]    	opa_int32_nr2frcp_f0a_h;
logic [`VPU_DATA_S_SZ-1:0]    	transfa_f0a_h;
logic [22:0]                    opa_ndnps_f0a_h;
logic [25:16]                   opa_ndnhi_f0a_h;
logic [9:0]                     opa_ndnlo_f0a_h;
logic [`VPU_DATA_S_SZ-1:0]    	fa_raw_f0a_h;

logic [`VPU_DATA_S_SZ-1:0]    	opb_f0a_h;
logic [`VPU_DATA_S_SZ-1:0]    	opb_int32_nr2frcp_f0a_h;
logic [`VPU_DATA_S_SZ-1:5]    	transfb_f0a_h;
logic [22:0]                    opb_ndnps_f0a_h;
logic [25:16]                   opb_ndnhi_f0a_h;
logic [9:0]                     opb_ndnlo_f0a_h;
logic [`VPU_DATA_S_SZ-1:0]    	fb_raw_f0a_h;

logic [`VPU_DATA_S_SZ-1:0]    	opc_f0a_h;
logic [`VPU_DATA_S_SZ-1:0]    	transfc_f0a_h;
logic [31:16]                   opc_gfx_f0a_h;
logic [22:0]                    opc_ndnpsu24_f0a_h;
logic [9:0]                     opc_ndnph_f0a_h;
logic [5:0]                     f10f11frac_f0a_h;
logic [5:0]                     f10f11fracraw_f0a_h;
logic                           opc_un23_ps31_f0a_h;
logic [`VPU_DATA_S_SZ-1:0]    	fc_raw_f0a_h;

logic [`VPU_DATA_S_SZ-1:0]      xrom_c1c0k_f0a_h;
logic [16:0]                    xrom_c2_f0a_h;
logic [22:16]                   trans_mask_f0a_h;

logic [7:0]                     ea_raw_hi_f0a_h;
logic [4:0]                     ea_raw_lo_f0a_h;
logic [7:0]                     eb_raw_hi_f0a_h;
logic [4:0]                     eb_raw_lo_f0a_h;
logic [7:0]                     ec_raw_hi_f0a_h;
logic [4:0]                     ec5b_f0a_h; 

// Local Control Signals
logic                           opa_ps_den_l;
logic                           opa_ph_hi_den_l;
logic                           opa_ph_lo_den_l;
logic                           opa_ps_dnclr_f0a_l;
logic                           opa_phhi_dnclr_f0a_l;
logic                           opa_phlo_dnclr_f0a_l;
logic                           opb_ps_dnclr_f0a_l;
logic                           opb_phhi_dnclr_f0a_l;
logic                           opb_phlo_dnclr_f0a_l;
logic                           opb_ps_den_l;
logic                           opb_ph_hi_den_l;
logic                           opb_ph_lo_den_l;
logic                           opc_ps_den_l;
logic                           opc_ph_den_l;
logic                           neg_sa_f0a_h;
logic                           neg_sb_f0a_h;
logic                           neg_sc_f0a_h;
logic                           opcf10f11_expz_l;
logic                           fc_dnclrps_f0a_l;
logic                           fc_dnclrph_f0a_l;
logic                           sel_opa_nr2frcpfx_f0a_h;
logic                           sel_opb_nr2frcpfx_f0a_h;
logic [28:13]                   gfxk_mpnd_f0a_h;
logic [22:16]                   trans_opbmasked_f0a_h;


///////////////////////////////////////////////////////////////////////////////
// Local Flops - Needed to guarantee timing 
///////////////////////////////////////////////////////////////////////////////

 //    CLK                 DOUT                   DIN
 // Flop Data Inputs from ROM/Bypass
  `FF (opa_frf_clk,   opa_f0a_h,             opa_byp_frfa_h            )
  `FF (opb_frf_clk,   opb_f0a_h,             opb_byp_frfa_h            )
  `FF (opc_frf_clk,   opc_f0a_h,             opc_byp_frfa_h            )
  `FF (trans_frf_clk, xrom_c1c0k_f0a_h,      xrom_c1c0k_ra_h           )
  `FF (trans_frf_clk, xrom_c2_f0a_h,         xrom_c2_ra_h              )
  `FF (trans_frf_clk, trans_mask_f0a_h,      trans_mask_ra_h           )
 // Flop control inputs from txfma_c0
  `FF (ctrl_frf_clk,  eb_hi_k_f0a_h,         eb_hi_k_exa_h             )
  `FF (ctrl_frf_clk,  sel_eblo_ph_f0a_h,     sel_eblo_ph_exa_h         )
  `FF (ctrl_frf_clk,  sel_ebin_hi_f0a_h,     sel_ebin_hi_exa_h         )
  `FF (ctrl_frf_clk,  elo_eb_en_f0a_h,       elo_eb_en_exa_h           )
  `FF (ctrl_frf_clk,  ehi_eb_en_f0a_h,       ehi_eb_en_exa_h           )
  `FF (ctrl_frf_clk,  elo_ea_en_f0a_h,       elo_ea_en_exa_h           )
  `FF (ctrl_frf_clk,  fa_en_f0a_h,           fa_en_exa_h               )
  `FF (ctrl_frf_clk,  fb_en_f0a_h,           fb_en_exa_h               )
  `FF (ctrl_frf_clk,  fc_en_f0a_h,           fc_en_exa_h               )
  `FF (ctrl_frf_clk,  sel_ealo_ph_f0a_h,     sel_ealo_ph_exa_h         )
  `FF (ctrl_frf_clk,  sel_eainmux_f0a_h,     sel_eainmux_exa_h         )
  `FF (ctrl_frf_clk,  sel_ec5b_f0a_h,        sel_ec5b_exa_h            )
  `FF (ctrl_frf_clk,  sel_ecin_f0a_h,        sel_ecin_exa_h            )
  `FF (ctrl_frf_clk,  sel_ec5bdet_f0a_h,     sel_ec5bdet_exa_h         )
  `FF (ctrl_frf_clk,  sa_hi_en_f0a_h,        sa_hi_en_exa_h            )
  `FF (ctrl_frf_clk,  sa_lo_en_f0a_h,        sa_lo_en_exa_h            )
  `FF (ctrl_frf_clk,  sb_hi_en_f0a_h,        sb_hi_en_exa_h            )
  `FF (ctrl_frf_clk,  sb_lo_en_f0a_h,        sb_lo_en_exa_h            )
  `FF (ctrl_frf_clk,  sc_en_f0a_h,           sc_en_exa_h               )
  `FF (ctrl_frf_clk,  ehi_ea_en_f0a_h,       ehi_ea_en_exa_h           )
  `FF (ctrl_frf_clk,  ec_en_f0a_h,           ec_en_exa_h               )
  `FF (ctrl_frf_clk,  neg_fmaop_f0a_h,       neg_fmaop_exa_h           )
  `FF (ctrl_frf_clk,  trans_op2_f0a_l,       trans_op2_exa_l           )
  `FF (ctrl_frf_clk,  sa1_nr2_frcpfxp_f0a_h, sa1_nr2_frcpfxp_exa_h     )
  `FF (ctrl_frf_clk,  sel_sc_f0a_h,          sel_sc_exa_h              )
  `FF (ctrl_frf_clk,  sel_sign_sinp2_f0a_h,  sel_sign_sinp2_exa_h      )
  `FF (ctrl_frf_clk,  sa_zap_f0a_l,          sa_zap_exa_l              )
  `FF (ctrl_frf_clk,  sb_zap_f0a_l,          sb_zap_exa_l              )
  `FF (ctrl_frf_clk,  sel_f10_f0a_h,         sel_f10_exa_h             )
  `FF (ctrl_frf_clk,  sel_f11_f0a_h,         sel_f11_exa_h             )
  `FF (ctrl_frf_clk,  sel_opcgfx_f0a_h,      sel_opcgfx_exa_h          )
  `FF (ctrl_frf_clk,  fc_dtps_f0a_h,         fc_dtps_exa_h             )
  `FF (ctrl_frf_clk,  sel_xopc_f0a_h,        sel_xopc_exa_h            )
  `FF (ctrl_frf_clk,  sel_opcfmt_f0a_h,      sel_opcfmt_exa_h          )
  `FF (ctrl_frf_clk,  sel_snorm_f0a_h,       sel_snorm_exa_h           )
  `FF (ctrl_frf_clk,  sel_g1sk2_f0a_h,       sel_g1sk2_exa_h           ) 
  `FF (ctrl_frf_clk,  sel_g1sk8_f0a_h,       sel_g1sk8_exa_h           )
  `FF (ctrl_frf_clk,  sel_g1sk16_f0a_h,      sel_g1sk16_exa_h          )
  `FF (ctrl_frf_clk,  sel_g1sk24_f0a_h,      sel_g1sk24_exa_h          )
  `FF (ctrl_frf_clk,  sel_xexpfrac_f0a_h,    sel_xexpfrac_exa_h        )
  `FF (ctrl_frf_clk,  sel_transfa_f0a_h,     sel_transfa_exa_h         )
  `FF (ctrl_frf_clk,  sel_transfb_f0a_h,     sel_transfb_exa_h         )     
  `FF (ctrl_frf_clk,  sel_opafmt_f0a_h,      sel_opafmt_exa_h          )
  `FF (ctrl_frf_clk,  sel_opbfmt_f0a_h,      sel_opbfmt_exa_h          )


///////////////////////////////////////////////////////////////////////////////
// Local Control Logic 
///////////////////////////////////////////////////////////////////////////////

assign opa_ps_den_l    = |opa_f0a_h[30:23]; 
assign opa_ph_hi_den_l = |opa_f0a_h[30:26]; 
assign opa_ph_lo_den_l = |opa_f0a_h[14:10];
assign opb_ps_den_l    = |opb_f0a_h[30:23]; 
assign opb_ph_hi_den_l = |opb_f0a_h[30:26]; 
assign opb_ph_lo_den_l = |opb_f0a_h[14:10];

assign opc_ps_den_l    = |opc_f0a_h[30:23]; 
assign opc_ph_den_l    = |opc_f0a_h[14:10]; 

assign opa_ps_dnclr_f0a_l   = opa_ps_den_l;
assign opa_phhi_dnclr_f0a_l = opa_ph_hi_den_l;
assign opa_phlo_dnclr_f0a_l = opa_ph_lo_den_l;

assign opb_ps_dnclr_f0a_l   = opb_ps_den_l;
assign opb_phhi_dnclr_f0a_l = opb_ph_hi_den_l;
assign opb_phlo_dnclr_f0a_l = opb_ph_lo_den_l;

assign fc_dnclrps_f0a_l = ~(~opc_ps_den_l & fc_dtps_f0a_h);
assign fc_dnclrph_f0a_l = opc_ph_den_l;

assign sel_opa_nr2frcpfx_f0a_h = sa1_nr2_frcpfxp_f0a_h;
assign sel_opb_nr2frcpfx_f0a_h = sa1_nr2_frcpfxp_f0a_h;


///////////////////////////////////////////////////////////////////////////////
// Building the Operands based on Control from txfma_f0 
///////////////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////////////
  // OperandA
  ///////////////////////////////////////////////////////////////////////////////
 
  // Sign
assign neg_sa_f0a_h    = (neg_fmaop_f0a_h ^ opa_f0a_h[31]) & trans_op2_f0a_l;
assign sa_raw_hi_f0a_h = (neg_sa_f0a_h & sa_zap_f0a_l);
assign sa_raw_lo_f0a_h = opa_f0a_h[15]; 

  // Sign O/P
assign sa_hi_f0a_h = sa_raw_hi_f0a_h &  sa_hi_en_f0a_h;
assign sa_lo_f0a_h = sa_raw_lo_f0a_h &  sa_lo_en_f0a_h;


  // Exponent
always_comb
begin
  case (sel_eainmux_f0a_h)
    2'b00:  ea_raw_hi_f0a_h = {1'b1, 6'b0, 1'b1};
    2'b01:  ea_raw_hi_f0a_h = opa_f0a_h[30:23];
    2'b10:  ea_raw_hi_f0a_h = 8'b10000101; 
    2'b11:  ea_raw_hi_f0a_h = {3'b0, opa_f0a_h[30:26]};
  endcase
end

assign ea_raw_lo_f0a_h = opa_f0a_h[14:10] & {5{sel_ealo_ph_f0a_h}}; 

  // Exponent Output
assign ea_hi_f0a_h = ea_raw_hi_f0a_h & {8{ehi_ea_en_f0a_h}};
assign ea_lo_f0a_h = ea_raw_lo_f0a_h & {5{elo_ea_en_f0a_h}};


  // Mantissa 
always_comb
begin
  case(sel_transfa_f0a_h)  
    2'b00:     transfa_f0a_h = {xrom_c2_f0a_h, 15'b0};
    2'b01:     transfa_f0a_h = {2'b01, opa_f0a_h[24:0],5'b0}; 
    2'b10:     transfa_f0a_h = {1'b1, {5{sel_xexpfrac_f0a_h}}, 26'b0};
    2'b11:     transfa_f0a_h = {1'b1, opa_f0a_h[23:0], 7'b0}; 
  endcase
end


assign opa_ndnps_f0a_h = opa_f0a_h[22:0] & {23{opa_ps_dnclr_f0a_l}};
assign opa_ndnhi_f0a_h = opa_f0a_h[25:16] & {10{opa_phhi_dnclr_f0a_l}};
assign opa_ndnlo_f0a_h = opa_f0a_h[9:0] & {10{opa_phlo_dnclr_f0a_l}};


always_comb
begin
  case(sel_opa_nr2frcpfx_f0a_h)  
    1'b0:     opa_int32_nr2frcp_f0a_h = opa_f0a_h; 
    1'b1:     opa_int32_nr2frcp_f0a_h = {2'b0, opa_f0a_h[31:2]};
  endcase
end

always_comb
begin
  case (sel_opafmt_f0a_h)
    2'b00:  fa_raw_f0a_h = opa_int32_nr2frcp_f0a_h; 
    2'b01:  fa_raw_f0a_h = {opa_ps_den_l, opa_ndnps_f0a_h, 8'b0};
    2'b10:  fa_raw_f0a_h = {opa_ph_hi_den_l, opa_ndnhi_f0a_h, 9'b0, opa_ph_lo_den_l, opa_ndnlo_f0a_h, 1'b0}; 
    2'b11:  fa_raw_f0a_h = transfa_f0a_h;
  endcase
end

  // Output Mantissa
assign fa_f0a_h = fa_raw_f0a_h & {32{fa_en_f0a_h}};


  ///////////////////////////////////////////////////////////////////////////////
  // OperandB
  ///////////////////////////////////////////////////////////////////////////////

  // Sign
assign neg_sb_f0a_h = (sa1_nr2_frcpfxp_f0a_h ^ opb_f0a_h[31]);

assign sb_raw_hi_f0a_h = (neg_sb_f0a_h & sb_zap_f0a_l) | sel_sign_sinp2_f0a_h;
assign sb_raw_lo_f0a_h = opb_f0a_h[15]; 

  // Sign O/P
assign sb_hi_f0a_h = sb_raw_hi_f0a_h &  sb_hi_en_f0a_h;
assign sb_lo_f0a_h = sb_raw_lo_f0a_h &  sb_lo_en_f0a_h;

  
  // Exponent
always_comb
begin
  case (sel_ebin_hi_f0a_h)
    2'b00:  eb_raw_hi_f0a_h = {eb_hi_k_f0a_h[5], 2'b0, eb_hi_k_f0a_h[4:0]};
    2'b01:  eb_raw_hi_f0a_h = opb_f0a_h[30:23];
    2'b10:  eb_raw_hi_f0a_h = {3'b0, opb_f0a_h[30:26]};
    2'b11:  eb_raw_hi_f0a_h = opb_f0a_h[31:24]; 
  endcase
end

assign eb_raw_lo_f0a_h = opb_f0a_h[14:10] & {$bits(opb_f0a_h[14:10]){sel_eblo_ph_f0a_h}}; 

  // Exponent Output
assign eb_hi_f0a_h = eb_raw_hi_f0a_h & {8{ehi_eb_en_f0a_h}};
assign eb_lo_f0a_h = eb_raw_lo_f0a_h & {5{elo_eb_en_f0a_h}};


  // Mantissa
  
assign gfxk_mpnd_f0a_h[28]    = ~sel_snorm_f0a_h;
assign gfxk_mpnd_f0a_h[27]    = 1'b1; 
assign gfxk_mpnd_f0a_h[26:21] = {$bits(gfxk_mpnd_f0a_h[26:21]){~sel_g1sk2_f0a_h}}; 
assign gfxk_mpnd_f0a_h[20:19] = ~({$bits(gfxk_mpnd_f0a_h[20:19]){sel_g1sk2_f0a_h}} |
                                  {$bits(gfxk_mpnd_f0a_h[20:19]){sel_g1sk8_f0a_h}}); 
assign gfxk_mpnd_f0a_h[18:13] = ({$bits(gfxk_mpnd_f0a_h[18:13]){sel_g1sk16_f0a_h}} |
                                 {$bits(gfxk_mpnd_f0a_h[18:13]){sel_g1sk24_f0a_h}}); 
  
assign trans_opbmasked_f0a_h[22:16] = opb_f0a_h[22:16] & trans_mask_f0a_h[22:16];

always_comb
begin
  case(sel_transfb_f0a_h)  
    2'b00:     transfb_f0a_h = {3'b0, trans_opbmasked_f0a_h, opb_f0a_h[15:0], 1'b0};
    2'b01:     transfb_f0a_h = {3'b0, gfxk_mpnd_f0a_h, {8{sel_g1sk24_f0a_h}}};
    2'b10:     transfb_f0a_h = {2'b0, opb_f0a_h[22:0], 2'b0};
    2'b11:     transfb_f0a_h = 27'b001010010101011101111001110; 
  endcase
end

assign opb_ndnps_f0a_h = opb_f0a_h[22:0] & {23{opb_ps_dnclr_f0a_l}};
assign opb_ndnhi_f0a_h = opb_f0a_h[25:16] & {10{opb_phhi_dnclr_f0a_l}};
assign opb_ndnlo_f0a_h = opb_f0a_h[9:0] & {10{opb_phlo_dnclr_f0a_l}};


always_comb
begin
  case(sel_opb_nr2frcpfx_f0a_h)  
    1'b0:     opb_int32_nr2frcp_f0a_h = opb_f0a_h; 
    1'b1:     opb_int32_nr2frcp_f0a_h = {2'b0, opb_f0a_h[29:0]};
  endcase
end


always_comb
begin
  case (sel_opbfmt_f0a_h)
    2'b00:  fb_raw_f0a_h = opb_int32_nr2frcp_f0a_h; 
    2'b01:  fb_raw_f0a_h = {2'b0, opb_ps_den_l, opb_ndnps_f0a_h, 6'b0}; 
    2'b10:  fb_raw_f0a_h = {2'b0, opb_ph_hi_den_l, opb_ndnhi_f0a_h, 7'b0, opb_ph_lo_den_l, opb_ndnlo_f0a_h, 1'b0};
    2'b11:  fb_raw_f0a_h = {transfb_f0a_h, 5'b0};
  endcase
end


  // Output Mantissa
assign fb_f0a_h = fb_raw_f0a_h & {32{fb_en_f0a_h}};


  ///////////////////////////////////////////////////////////////////////////////
  // OperandC
  ///////////////////////////////////////////////////////////////////////////////

  // Sign
assign neg_sc_f0a_h    = (neg_fmaop_f0a_h ^ opc_f0a_h[31]); 

always_comb
begin
  case(sel_sc_f0a_h)
    2'b00:  sc_raw_f0a_h = neg_sc_f0a_h;              // All others
    2'b01:  sc_raw_f0a_h = opc_f0a_h[7];              // CVT SNORM8 to PS
    2'b10:  sc_raw_f0a_h = opc_f0a_h[15];             // CVT SNORM16/PH to PS 
    2'b11:  sc_raw_f0a_h = 1'b0;                      // Zap SignC 
  endcase
end

  // Sign O/P
assign sc_f0a_h = sc_raw_f0a_h &  sc_en_f0a_h;


  // Exponent
always_comb
begin
  case(sel_ec5b_f0a_h)
    2'b00:  ec5b_f0a_h = opc_f0a_h[14:10];          // PH/LO
    2'b01:  ec5b_f0a_h = opc_f0a_h[10:6];           // F11
    2'b10:  ec5b_f0a_h = opc_f0a_h[9:5];            // F10
    2'b11:  ec5b_f0a_h = 5'b0; 
  endcase
end

always_comb
begin
  case (sel_ecin_f0a_h)
    2'b00:  ec_raw_hi_f0a_h = 8'b0; 
    2'b01:  ec_raw_hi_f0a_h = opc_f0a_h[30:23];
    2'b10:  ec_raw_hi_f0a_h = {1'b1, 6'b0, 1'b1};  
    2'b11:  ec_raw_hi_f0a_h = {3'b0, ec5b_f0a_h}; 
  endcase
end

  // Exponent Output
assign ec_f0a_h[7:0] = ec_raw_hi_f0a_h[7:0] & {$bits(ec_f0a_h[7:0]) {ec_en_f0a_h}};


  // Mantissa
always_comb
begin
  case(sel_opcgfx_f0a_h)  
    2'b00:     opc_gfx_f0a_h = opc_f0a_h[15:0];               // NORM16
    2'b01:     opc_gfx_f0a_h = {opc_f0a_h[9:0],  6'b0};       // UNORM10
    2'b10:     opc_gfx_f0a_h = {opc_f0a_h[7:0],  8'b0};       // NORM8
    2'b11:     opc_gfx_f0a_h = {opc_f0a_h[1:0], 14'b0};       // UNORM2
  endcase
end

assign opcf10f11_expz_l = (sel_f11_f0a_h & opc_f0a_h[10]) | (sel_f10_f0a_h & opc_f0a_h[5]) | (|opc_f0a_h[9:6]); 

always_comb
begin
  case(fc_dtps_f0a_h)  
    1'b0:     opc_un23_ps31_f0a_h = opc_f0a_h[23]; 
    1'b1:     opc_un23_ps31_f0a_h = opc_ps_den_l; 
  endcase
end

always_comb
begin
  case(sel_f11_f0a_h)  
    1'b0:     f10f11fracraw_f0a_h = {opc_f0a_h[4:0], 1'b0};     // F10
    1'b1:     f10f11fracraw_f0a_h = opc_f0a_h[5:0];             // F11
  endcase
end

assign f10f11frac_f0a_h = {6{opcf10f11_expz_l}} & f10f11fracraw_f0a_h ;

assign opc_ndnpsu24_f0a_h = opc_f0a_h[22:0] & {$bits(opc_ndnpsu24_f0a_h){fc_dnclrps_f0a_l}};
assign opc_ndnph_f0a_h    = opc_f0a_h[9:0] & {$bits(opc_ndnph_f0a_h){fc_dnclrph_f0a_l}};


always_comb
begin
  case(sel_xopc_f0a_h)  
    2'b00:     transfc_f0a_h = xrom_c1c0k_f0a_h;
    2'b01:     transfc_f0a_h = {opcf10f11_expz_l, f10f11frac_f0a_h, 25'b0};
    2'b10:     transfc_f0a_h = {opc_f0a_h[30:0], 1'b0};
    2'b11:     transfc_f0a_h = {opc_un23_ps31_f0a_h, opc_ndnpsu24_f0a_h, 8'b0}; 
  endcase
end


always_comb
begin
  case(sel_opcfmt_f0a_h)  
    2'b00:     fc_raw_f0a_h = opc_f0a_h;
    2'b01:     fc_raw_f0a_h = {opc_gfx_f0a_h, 16'b0};
    2'b10:     fc_raw_f0a_h = transfc_f0a_h; 
    2'b11:     fc_raw_f0a_h = {opc_ph_den_l, opc_ndnph_f0a_h, 21'b0}; 
  endcase
end

  // Output Mantissa
assign fc_f0a_h = fc_raw_f0a_h & {32{fc_en_f0a_h}};


//===================================================================================================================
// Exponent Special Detect:
//      exp hi greather than 126 used for rcp special case
//===================================================================================================================

assign eb_eql_127_f0a_h  = ((opb_f0a_h[`FLOAT32_W-2:`FLOAT32_W-`FLOAT32_E-1])==8'b11111110);
assign eb_eql_126_f0a_h  = ((opb_f0a_h[`FLOAT32_W-2:`FLOAT32_W-`FLOAT32_E-1])==8'b11111101);
assign eb_eql_125_f0a_h  = ((opb_f0a_h[`FLOAT32_W-2:`FLOAT32_W-`FLOAT32_E-1])==8'b01111101);

assign ea_hi_all1_f0a_h    = &({ea_hi_f0a_h[7:5] | {3{sel_ealo_ph_f0a_h}}, ea_hi_f0a_h[4:0]} );
assign ea_hi_zero_f0a_l    = (|ea_hi_f0a_h);
assign ea_lo_all1_f0a_h    = &ea_lo_f0a_h;
assign ea_lo_zero_f0a_l    = (|ea_lo_f0a_h);

assign eb_hi_all1_f0a_h    = &({eb_hi_f0a_h[7:5] | {3{sel_eblo_ph_f0a_h}}, eb_hi_f0a_h[4:0]} );
assign eb_hi_zero_f0a_l    = (|eb_hi_f0a_h);
assign eb_lo_all1_f0a_h    = &eb_lo_f0a_h;
assign eb_lo_zero_f0a_l    = (|eb_lo_f0a_h);

assign ec_all1_f0a_h    = &({ec_f0a_h[7:5] | {3{sel_ec5bdet_f0a_h}}, ec_f0a_h[4:0]} );
assign ec_zero_f0a_l    = (|ec_f0a_h);


//===================================================================================================================
// Signals needed for the Mantissa Special Detect logic which is now in txfma_c0 
//===================================================================================================================
//
// SS: We are using opa, opb, opc instead of fa, fb, fc
// LHS corresponds to formatted fa, fb, fc bits, for example, bit 30 is b0 bit.
//
assign faph_lo_b0_f0a_h = opa_f0a_h[9];
assign faps_b0_f0a_h = opa_f0a_h[22];
assign fbph_lo_b0_f0a_h = opb_f0a_h[9];
assign fbps_b0_f0a_h = opb_f0a_h[22];
assign fcps_b0_f0a_h = opc_f0a_h[22];
assign fc_31_f0a_h = fc_f0a_h[31];
assign fcnorm_30_f0a_h = fc_f0a_h[30];
assign faph_hi_b0_f0a_h = opa_f0a_h[25];
assign fbph_hi_b0_f0a_h = opb_f0a_h[25];
assign fcph_b0_f0a_h = opc_f0a_h[9];   // ph to ps operand is right justified
assign fcf11_b0_f0a_h = opc_f0a_h[5];   // needed for graphics datatypes
assign fcf10_b0_f0a_h = opc_f0a_h[4];   // needed for graphics datatypes


assign faph_lo_zero_f0a_l  = (|opa_f0a_h[8:0]);
assign fbph_lo_zero_f0a_l  = (|opb_f0a_h[8:0]); 
assign faph_hi_zero_f0a_l  = (|opa_f0a_h[24:16]);
assign fbph_hi_zero_f0a_l  = (|opb_f0a_h[24:16]);
assign faps_zero_f0a_l     = (|opa_f0a_h[21:0]);
assign fbps_zero_f0a_l     = (|opb_f0a_h[21:0]);

assign fcf10_zero_f0a_l   = (|opc_f0a_h[3:0]);
assign fcf11_zero_f0a_l   = (|opc_f0a_h[4:0]);
assign fcps_zero_f0a_h     = !(|opc_f0a_h[21:0]);
assign fcph_zero_f0a_l = (|opc_f0a_h[8:0]);
assign fcnorm_zero_29_8_f0a_h       = !(|fc_f0a_h[29:8]);
assign fcnorm16_eq_1_f0a_h  = !(|fc_f0a_h[30:17]) & fc_f0a_h[16];
assign fcnorm8_eq_1_f0a_h  = !(|fc_f0a_h[30:25]) & fc_f0a_h[24];

assign fcn8_one_30_24_f0a_h      = (&fc_f0a_h[30:24]);
assign fcn10_one_30_22_f0a_h     = (&fc_f0a_h[30:22]);
assign fcn16_one_30_16_f0a_h     = (&fc_f0a_h[30:16]);
assign fcn24_one_30_8_f0a_h      = (&fc_f0a_h[30:8]);


endmodule

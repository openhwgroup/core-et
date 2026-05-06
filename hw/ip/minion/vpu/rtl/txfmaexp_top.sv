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

module txfmaexp_top
(
  input                                          clock,
  input                                          ec_zero_f1a_h,
  input                                          ec_all1_f1a_h,
  input                                          ctrl_valid_f1a_h,
  input                                          ctrl_valid_f2a_h,
  input                                          ctrl_valid_f3a_h,
  input                                          ctrl_valid_f4a_h,
  input                                          ctrl_valid_f5a_h,
  input  [VPU_DTYPE_SZ-1:0]                     op_dtype_f1a_h,
  input  [VPU_DTYPE_SZ-1:0]                     op_dtype_f2a_h,
  input  [VPU_DTYPE_SZ-1:0]                     op_dtype_f5a_h,
  input  [VPU_DTYPE_SZ-1:0]                     op_dtype_f6a_h,
  input  [TXFMA_SIGS_SZ-1:0]                    sigs_f1a_h,
  input  [TXFMA_SIGS_SZ-1:0]                    sigs_f2a_h,
  input  [TXFMA_SIGS_SZ-1:0]                    sigs_f5a_h,
  input  [TXFMA_SIGS_SZ-1:0]                    sigs_f6a_h,
  input  [VPU_FCMD_SZ-1:0]                      cmd_f1a_h,
  input  [VPU_FCMD_SZ-1:0]                      cmd_f2a_h,
  input  [VPU_FCMD_SZ-1:0]                      cmd_f5a_h,
  input  [VPU_FCMD_SZ-1:0]                      cmd_f6a_h,
  input  [TXFMA_EXP_PS_SZ-1:0]                  ea_hi_f1a_h,
  input  [TXFMA_EXP_PH_SZ-1:0]                  ea_lo_f1a_h,
  input  [TXFMA_EXP_PS_SZ-1:0]                  eb_hi_f1a_h,
  input  [TXFMA_EXP_PH_SZ-1:0]                  eb_lo_f1a_h,
  input  [TXFMA_EXP_PS_SZ-1:0]                  ec_f1a_h,
  input  [3:0]                                   etrz_fa_hi_ph_f1a_h,
  input  [4:0]                                   etrz_fa_lo_ps_f1a_h,
  input  [3:0]                                   etrz_fb_hi_ph_f1a_h,
  input  [4:0]                                   etrz_fb_lo_ps_f1a_h,
  input  [4:0]                                   etrz_fc_f1a_h,
  input                                          sin_ediff2_f2a_h,
  input  [TXFMA_LXD_PS_SZ-1:0]                  elxd_f4a_h,
  input  [TXFMA_WSPS_ETRZ_SZ-1:0]               etrz50_f4a_h,
  input  [TXFMA_RM_SZ-1:0]                      rm_f5a_h,
  input  [TXFMA_EXP_PS_SZ-1:0]                  ea_hi_f5a_h,
  input                                          ea_hi_zero_f1a_h,
  input                                          fc_zero30_8_f1a_h,
  input                                          eprod_hi_zero_f2a_h,
  input                                          eprod_lo_zero_f2a_h,
  input                                          ec_zero_f2a_h,
  input                                          exp_fma2_exc_f2a_h,
  input                                          eb_hi_lsb_f4a_h,
  input                                          use_elxd_f4a_h,
  input                                          elxd_add_in2_mux_sel_f4a_h,
  input                                          fb_hi_zero_f4a_h,
  input                                          comp_op_f5a_h,
  input                                          comp_op_true_f5a_h,
  input                                          int_minmax_mux_sel_a_f5a_h,
  input                                          int_minmax_mux_sel_c_f5a_h,
  input                                          nshc_f5a_h,
  input                                          s_res_f5a_h,
  input                                          e_force_to_zero_f5a_h,
  input                                          ps_rndovf_f6a_h,
  input                                          ph_rndovf_f6a_h,
  output logic [TXFMA_EXP_PS_SZ-1:0]            ec_f2a_h,
  output logic [TXFMA_EXP_PS_SZ-1:0]            ec_f3a_h,
  output                                         e_sticky1_f2a_h,
  output                                         e_sticky2_f2a_h,
  output logic                                   e_sticky1_f3a_h,
  output logic [TXFMA_EDIFF_OPD_SEL_SZ-1:0]     align1_mux_sel_f2a_h,
  output logic [TXFMA_EDIFF_OPD_SEL_SZ-1:0]     align2_mux_sel_f2a_h,
  output logic [TXFMA_EDIFF_OPD_SEL_SZ-1:0]     opd3_mux_sel_f2a_h,
  output logic [TXFMA_EXP_SEXT_PS_SZ-1:0]       ediff1_f2a_h,
  output logic [TXFMA_EXP_SEXT_PS_SZ-1:0]       ediff2_f3a_h,
  output logic [TXFMA_EDIFF_OPD_SEL_SZ-1:0]     ediff1_sel_f2a_h,
  output logic [TXFMA_EDIFF_OPD_SEL_SZ-1:0]     ediff2_sel_f2a_h,
  output logic [TXFMA_EXP_SEXT_PS_SZ-1:0]       ediff1_abs_f2a_h,
  output logic [TXFMA_EXP_SEXT_PS_SZ-1:0]       ediff2_abs_f2a_h,
  output logic [5:0]                             ediff_abs_diff_hi_f2a_h,
  output logic [4:0]                             align2_rshamt_c3_f2a_h,
  output logic                                   ph_comb_stiky_f4a_h,
  output logic [6:0]                             exp_res_2f3_f2a_h,
  output logic                                   stky2g_f5a_h,
  output logic [TXFMA_EXP_PS_SZ-1:0]            exp_res_f6a_h,
  output                                         exp_res_zero_f6a_h,
  output                                         rmode_f5a_h,
  output                                         ediff1_zap_f5a_h,
  output                                         def_ovf_ps_f6a_h,
  output                                         pot_ovf_ps_f6a_h,
  output                                         def_unf_ps_f6a_h,
  output                                         pot_unf_ps_f6a_h,
  output                                         def_ovf_ph_f6a_h,
  output                                         pot_ovf_ph_f6a_h,
  output                                         def_unf_ph_f6a_h,
  output                                         pot_unf_ph_f6a_h
);


///////////////////////////////////////////////////////////////////////////////
// Internal logic
///////////////////////////////////////////////////////////////////////////////

logic [TXFMA_EXP_PS_SZ-1:0]                 eb_hi_f2a_h;
logic [TXFMA_EXP_PS_SZ-1:0]                 ec_out_f1a_h;
logic [TXFMA_EXP_PS_SZ-1:0]                 ec_f4a_h;
logic [TXFMA_EXP_PS_SZ-1:0]                 ec_f5a_h;
logic [TXFMA_EXP_SEXT_PS_SZ-1:0]            ediff_ec_eph_f1a_h;
logic [TXFMA_EXP_SEXT_PS_SZ-1:0]            ediff_ec_epl_f1a_h;
logic [TXFMA_EXP_SEXT_PS_SZ-1:0]            ediff_eph_epl_f1a_h;
logic [TXFMA_EXP_SEXT_PS_SZ-1:0]            ediff_ec_eph_f2a_h;
logic [TXFMA_EXP_SEXT_PS_SZ-1:0]            ediff_ec_epl_f2a_h;
logic [TXFMA_EXP_SEXT_PS_SZ-1:0]            ediff_eph_epl_f2a_h;
logic [TXFMA_EXP_SEXT_PS_SZ-1:0]            eprod_hi_f1a_h;
logic [TXFMA_EXP_SEXT_PS_SZ-1:0]            eprod_lo_f1a_h;
logic [TXFMA_EXP_SEXT_PS_SZ-1:0]            eprod_hi_f2a_h;
logic [TXFMA_EXP_SEXT_PS_SZ-1:0]            eprod_lo_f2a_h;
logic [4:0]                                  etrz_fa_lo_ps_f2a_h;
logic [4:0]                                  etrz_fc_f2a_h;
logic [TXFMA_ETRZ_PROD_PH_SZ-1:0]           etrz_prod_hi_ph_f1a_h;
logic [TXFMA_ETRZ_PROD_PS_SZ-1:0]           etrz_prod_lo_ps_f1a_h;
logic [TXFMA_ETRZ_PROD_PH_SZ-1:0]           etrz_prod_hi_ph_f2a_h;
logic [TXFMA_ETRZ_PROD_PS_SZ-1:0]           etrz_prod_lo_ps_f2a_h;
logic                                        ediff1_zap_f2a_h;
logic                                        ediff1_zap_f3a_h;
logic                                        ediff1_zap_f4a_h;
logic [TXFMA_EXP_SEXT_PS_SZ-1:0]            exp_res_f2a_h;
logic [TXFMA_EXP_SEXT_PS_SZ-1:0]            exp_res_f3a_h;
logic [TXFMA_EXP_SEXT_PS_SZ-1:0]            exp_res_f4a_h;
logic [TXFMA_EXP_SEXT_PS_SZ-1:0]            exp_res_in_f5a_h;
logic [TXFMA_EXP_SEXT_PS_SZ-1:0]            exp_res_f5a_h;
logic [TXFMA_EXP_SEXT_PS_SZ-1:0]            exp_res_in_f6a_h;
logic                                        e_sticky1_f4a_h;
logic                                        e_sticky2_f4a_h;
logic [TXFMA_EXP_SEXT_PS_SZ-1:0]            ediff2_f2a_h;
logic					                     e_sticky1_f5a_h;
logic					                     e_sticky2_f5a_h;
logic  [TXFMA_LXD_PS_SZ-1:0]		         elxd_f5a_h;
logic  [TXFMA_WSPS_ETRZ_SZ-1:0]	         etrz50_f5a_h;
logic                                        use_elxd_f5a_h;
logic                                        elxd_add_in2_mux_sel_f5a_h;
logic                                        fb_hi_zero_f5a_h;
logic                                        eb_hi_lsb_f5a_h;
logic                                        e_sticky2_f3a_h;


///////////////////////////////////////////////////////////////////////////////
// E1 component
///////////////////////////////////////////////////////////////////////////////
txfma_e1 txfma0_e1
(
  .op_dtype_f1a_h          ( op_dtype_f1a_h          ),
  .sigs_f1a_h              ( sigs_f1a_h              ),
  .cmd_f1a_h               ( cmd_f1a_h               ),
  .ea_hi_f1a_h             ( ea_hi_f1a_h             ),
  .ea_lo_f1a_h             ( ea_lo_f1a_h             ),
  .eb_hi_f1a_h             ( eb_hi_f1a_h             ),
  .eb_lo_f1a_h             ( eb_lo_f1a_h             ),
  .ec_f1a_h                ( ec_f1a_h                ),
  .ec_zero_f1a_h           ( ec_zero_f1a_h           ),
  .ec_all1_f1a_h           ( ec_all1_f1a_h           ),
  .ea_hi_zero_f1a_h        ( ea_hi_zero_f1a_h        ),
  .etrz_fa_hi_ph_f1a_h     ( etrz_fa_hi_ph_f1a_h     ),
  .etrz_fa_lo_ps_f1a_h     ( etrz_fa_lo_ps_f1a_h     ),
  .etrz_fb_hi_ph_f1a_h     ( etrz_fb_hi_ph_f1a_h     ),
  .etrz_fb_lo_ps_f1a_h     ( etrz_fb_lo_ps_f1a_h     ),
  .fc_zero30_8_f1a_h       ( fc_zero30_8_f1a_h       ),
  .ediff_ec_eph_f1a_h      ( ediff_ec_eph_f1a_h      ),
  .ediff_ec_epl_f1a_h      ( ediff_ec_epl_f1a_h      ),
  .ediff_eph_epl_f1a_h     ( ediff_eph_epl_f1a_h     ),
  .eprod_hi_f1a_h          ( eprod_hi_f1a_h          ),
  .eprod_lo_f1a_h          ( eprod_lo_f1a_h          ),
  .etrz_prod_hi_ph_f1a_h   ( etrz_prod_hi_ph_f1a_h   ),
  .etrz_prod_lo_ps_f1a_h   ( etrz_prod_lo_ps_f1a_h   ),
  .ec_out_f1a_h            ( ec_out_f1a_h            )
);

///////////////////////////////////////////////////////////////////////////////
// E2FF
///////////////////////////////////////////////////////////////////////////////
always_ff @(posedge clock) begin
  if (ctrl_valid_f1a_h) begin
    {ec_f2a_h,
     eb_hi_f2a_h,
     ediff_ec_eph_f2a_h,
     ediff_ec_epl_f2a_h,
     ediff_eph_epl_f2a_h,
     eprod_hi_f2a_h,
     eprod_lo_f2a_h,
     etrz_prod_hi_ph_f2a_h,
     etrz_prod_lo_ps_f2a_h,
     etrz_fa_lo_ps_f2a_h,
     etrz_fc_f2a_h} <= {ec_out_f1a_h,
                        eb_hi_f1a_h,
                        ediff_ec_eph_f1a_h,
                        ediff_ec_epl_f1a_h,
                        ediff_eph_epl_f1a_h,
                        eprod_hi_f1a_h,
                        eprod_lo_f1a_h,
                        etrz_prod_hi_ph_f1a_h,
                        etrz_prod_lo_ps_f1a_h,
                        etrz_fa_lo_ps_f1a_h,
                        etrz_fc_f1a_h};
  end
end

///////////////////////////////////////////////////////////////////////////////
// E2 component
///////////////////////////////////////////////////////////////////////////////
txfma_e2 txfma0_e2
(
  .cmd_f2a_h                    ( cmd_f2a_h                ),
  .op_dtype_f2a_h               ( op_dtype_f2a_h           ),
  .sigs_f2a_h                   ( sigs_f2a_h               ),
  .ediff_ec_eph_f2a_h           ( ediff_ec_eph_f2a_h       ),
  .ediff_ec_epl_f2a_h           ( ediff_ec_epl_f2a_h       ),
  .ediff_eph_epl_f2a_h          ( ediff_eph_epl_f2a_h      ),
  .sin_ediff2_f2a_h             ( sin_ediff2_f2a_h         ),
  .eprod_hi_f2a_h               ( eprod_hi_f2a_h           ),
  .eprod_lo_f2a_h               ( eprod_lo_f2a_h           ),
  .ec_f2a_h                     ( ec_f2a_h                 ),
  .eb_hi_f2a_h                  ( eb_hi_f2a_h              ),
  .etrz_fa_lo_ps_f2a_h          ( etrz_fa_lo_ps_f2a_h      ),
  .etrz_fc_f2a_h                ( etrz_fc_f2a_h            ),
  .etrz_prod_hi_ph_f2a_h        ( etrz_prod_hi_ph_f2a_h    ),
  .etrz_prod_lo_ps_f2a_h        ( etrz_prod_lo_ps_f2a_h    ),
  .eprod_hi_zero_f2a_h          ( eprod_hi_zero_f2a_h      ),
  .eprod_lo_zero_f2a_h          ( eprod_lo_zero_f2a_h      ),
  .ec_zero_f2a_h                ( ec_zero_f2a_h            ),
  .exp_fma2_exc_f2a_h           ( exp_fma2_exc_f2a_h       ),
  .e_sticky1_f2a_h              ( e_sticky1_f2a_h          ),
  .e_sticky2_f2a_h              ( e_sticky2_f2a_h          ),
  .ediff_abs_diff_hi_f2a_h      ( ediff_abs_diff_hi_f2a_h  ),
  .align2_rshamt_c3_f2a_h       ( align2_rshamt_c3_f2a_h   ),
  .align1_mux_sel_f2a_h         ( align1_mux_sel_f2a_h     ),
  .align2_mux_sel_f2a_h         ( align2_mux_sel_f2a_h     ),
  .opd3_mux_sel_f2a_h           ( opd3_mux_sel_f2a_h       ),
  .ediff1_f2a_h                 ( ediff1_f2a_h             ),
  .ediff2_f2a_h                 ( ediff2_f2a_h             ),
  .ediff1_sel_f2a_h             ( ediff1_sel_f2a_h         ),
  .ediff2_sel_f2a_h             ( ediff2_sel_f2a_h         ),
  .ediff1_abs_f2a_h             ( ediff1_abs_f2a_h         ),
  .ediff2_abs_f2a_h             ( ediff2_abs_f2a_h         ),
  .exp_res_f2a_h                ( exp_res_f2a_h            ),
  .exp_res_2f3_f2a_h            ( exp_res_2f3_f2a_h        ),
  .ediff1_zap_f2a_h             ( ediff1_zap_f2a_h         )
);


///////////////////////////////////////////////////////////////////////////////
// E3FF
///////////////////////////////////////////////////////////////////////////////
always_ff @(posedge clock) begin
  if (ctrl_valid_f2a_h) begin
    {e_sticky1_f3a_h,
     e_sticky2_f3a_h,
     ediff2_f3a_h,
     exp_res_f3a_h,
     ec_f3a_h,
     ediff1_zap_f3a_h} <= {e_sticky1_f2a_h,
                           e_sticky2_f2a_h,
                           ediff2_f2a_h,
                           exp_res_f2a_h,
                           ec_f2a_h,
                           ediff1_zap_f2a_h};
  end
end

///////////////////////////////////////////////////////////////////////////////
// E4FF
///////////////////////////////////////////////////////////////////////////////
always_ff @(posedge clock) begin
  if (ctrl_valid_f3a_h) begin
    {e_sticky1_f4a_h,
     e_sticky2_f4a_h,
     exp_res_f4a_h,
     ec_f4a_h,
     ediff1_zap_f4a_h} <= {e_sticky1_f3a_h,
                           e_sticky2_f3a_h,
                           exp_res_f3a_h,
                           ec_f3a_h,
                           ediff1_zap_f3a_h};
  end
end

///////////////////////////////////////////////////////////////////////////////
// E4 component
///////////////////////////////////////////////////////////////////////////////
txfma_e4 txfma0_e4
(
  .e_sticky1_f4a_h                  ( e_sticky1_f4a_h                  ),
  .e_sticky2_f4a_h                  ( e_sticky2_f4a_h                  ),
  .ph_comb_stiky_f4a_h              ( ph_comb_stiky_f4a_h              )
);

///////////////////////////////////////////////////////////////////////////////
// E5FF
///////////////////////////////////////////////////////////////////////////////
always_ff @(posedge clock) begin
  if (ctrl_valid_f4a_h) begin
    {exp_res_in_f5a_h,
     ec_f5a_h,
     ediff1_zap_f5a_h,
     e_sticky1_f5a_h,
     e_sticky2_f5a_h,
     elxd_f5a_h,
     etrz50_f5a_h,
     use_elxd_f5a_h,
     elxd_add_in2_mux_sel_f5a_h,
     fb_hi_zero_f5a_h,
     eb_hi_lsb_f5a_h} <= {exp_res_f4a_h,
                          ec_f4a_h,
                          ediff1_zap_f4a_h,
                          e_sticky1_f4a_h,
                          e_sticky2_f4a_h,
                          elxd_f4a_h,
                          etrz50_f4a_h,
                          use_elxd_f4a_h,
                          elxd_add_in2_mux_sel_f4a_h,
                          fb_hi_zero_f4a_h,
                          eb_hi_lsb_f4a_h};
  end
end


///////////////////////////////////////////////////////////////////////////////
// E5 component
///////////////////////////////////////////////////////////////////////////////


txfma_e5 txfma0_e5
(
  .cmd_f5a_h                  ( cmd_f5a_h                 ),
  .s_res_f5a_h                ( s_res_f5a_h               ),
  .rm_f5a_h                   ( rm_f5a_h                  ),
  .op_dtype_f5a_h             ( op_dtype_f5a_h            ),
  .sigs_f5a_h                 ( sigs_f5a_h                ),
  .exp_res_in_f5a_h           ( exp_res_in_f5a_h          ),
  .ea_hi_f5a_h                ( ea_hi_f5a_h               ),
  .ec_f5a_h                   ( ec_f5a_h                  ),
  .comp_op_f5a_h              ( comp_op_f5a_h             ),
  .comp_op_true_f5a_h         ( comp_op_true_f5a_h        ),
  .nshc_f5a_h                 ( nshc_f5a_h                ),
  .e_force_to_zero_f5a_h      ( e_force_to_zero_f5a_h     ),
  .ediff1_zap_f5a_h           ( ediff1_zap_f5a_h          ),
  .e_sticky1_f5a_h            ( e_sticky1_f5a_h            ),
  .e_sticky2_f5a_h            ( e_sticky2_f5a_h            ),
  .elxd_f5a_h                 ( elxd_f5a_h                 ),
  .etrz50_f5a_h               ( etrz50_f5a_h               ),
  .use_elxd_f5a_h             ( use_elxd_f5a_h             ),
  .elxd_add_in2_mux_sel_f5a_h ( elxd_add_in2_mux_sel_f5a_h ),
  .fb_hi_zero_f5a_h           ( fb_hi_zero_f5a_h           ),
  .eb_hi_lsb_f5a_h            ( eb_hi_lsb_f5a_h            ),
  .stky2g_f5a_h               ( stky2g_f5a_h               ),
  .int_minmax_mux_sel_a_f5a_h ( int_minmax_mux_sel_a_f5a_h ),
  .int_minmax_mux_sel_c_f5a_h ( int_minmax_mux_sel_c_f5a_h ),
  .rmode_f5a_h                ( rmode_f5a_h                ),
  .exp_res_f5a_h              ( exp_res_f5a_h              )
);


///////////////////////////////////////////////////////////////////////////////
// E6FF
///////////////////////////////////////////////////////////////////////////////
always_ff @(posedge clock) begin
  if (ctrl_valid_f5a_h) begin
    exp_res_in_f6a_h <= exp_res_f5a_h;
  end
end

///////////////////////////////////////////////////////////////////////////////
// E6 component
///////////////////////////////////////////////////////////////////////////////
txfma_e6 txfma0_e6
(
  .cmd_f6a_h          ( cmd_f6a_h          ),
  .sigs_f6a_h         ( sigs_f6a_h         ),
  .op_dtype_f6a_h     ( op_dtype_f6a_h     ),
  .exp_res_in_f6a_h   ( exp_res_in_f6a_h   ),
  .ps_rndovf_f6a_h    ( ps_rndovf_f6a_h    ),
  .ph_rndovf_f6a_h    ( ph_rndovf_f6a_h    ),
  .exp_res_f6a_h      ( exp_res_f6a_h      ),
  .exp_res_zero_f6a_h ( exp_res_zero_f6a_h ),
  .def_ovf_ps_f6a_h   ( def_ovf_ps_f6a_h   ),
  .pot_ovf_ps_f6a_h   ( pot_ovf_ps_f6a_h   ),
  .def_unf_ps_f6a_h   ( def_unf_ps_f6a_h   ),
  .pot_unf_ps_f6a_h   ( pot_unf_ps_f6a_h   ),
  .def_ovf_ph_f6a_h   ( def_ovf_ph_f6a_h   ),
  .pot_ovf_ph_f6a_h   ( pot_ovf_ph_f6a_h   ),
  .def_unf_ph_f6a_h   ( def_unf_ph_f6a_h   ),
  .pot_unf_ph_f6a_h   ( pot_unf_ph_f6a_h   )
);

endmodule

`include "soc.vh"

module memshire_top_stub
   #(parameter [7:0] SHIRE_INST_ID = 255,
     parameter int   MEMSHIRE_ID   = 0,
     parameter bit   DV_MEMSHIRE_STUB = 1'b0)
(
  ///////////////////////////////////////////////////////////////
  //  Conenction to memory controllers (high voltage)
  ///////////////////////////////////////////////////////////////

  // Resets
  output wire                                    rst_sms_vmc,

  ///////////////////////////////////////////////////////////////
  //  Conenction to top level (low voltage)
  ///////////////////////////////////////////////////////////////

  // Resets
  input  wire                                    rst_sms_mc,

        //global
      input logic clock,
      input logic reset_w,
      input logic reset_c,
      input logic reset_d,

      output logic   enable_clock_debug // clock gater for debug logic      
    );

   ////////////////////////////////////////////////////////////////////////////////////////
   // Wires declared for DV interfaces that drill for things inside memshire.
   ////////////////////////////////////////////////////////////////////////////////////////
   // debug wires
   // ust_ is UltraSocTechnologies
   // smb is System Memory Buffer - an AXI master that can spool debug data to an AXI memory
   `EVL_WIRE sc_slave_axi_AR_channel_t         ust_smb_axi_AR;
   wire                                        ust_smb_axi_ARVALID;
   wire                                        ust_smb_axi_ARREADY;
   `EVL_WIRE sc_slave_axi_AW_channel_t         ust_smb_axi_AW;
   wire                                        ust_smb_axi_AWVALID;
   wire                                        ust_smb_axi_AWREADY;
   `EVL_WIRE sc_slave_axi_W_channel_t          ust_smb_axi_W;
   wire                                        ust_smb_axi_WVALID;
   wire                                        ust_smb_axi_WREADY;
   `EVL_WIRE sc_slave_axi_B_channel_t          ust_smb_axi_B;
   wire                                        ust_smb_axi_BVALID;
   wire                                        ust_smb_axi_BREADY;
   `EVL_WIRE sc_slave_axi_R_channel_t          ust_smb_axi_R;
   wire                                        ust_smb_axi_RVALID;
   wire                                        ust_smb_axi_RREADY;

   wire  unused_ok = &{ 1'b0,
                        clock,
                        reset_w,
                        reset_c,
                        reset_d,
                        1'b0 };

   `ifdef EVL_SIMULATION
      wor  sc_slave_axi_AR_channel_t      ifc_ust_smb_axi_AR;
      wor                                 ifc_ust_smb_axi_ARVALID;
      wand                                ifc_ust_smb_axi_ARREADY;
      wor  sc_slave_axi_AW_channel_t      ifc_ust_smb_axi_AW;
      wor                                 ifc_ust_smb_axi_AWVALID;
      wand                                ifc_ust_smb_axi_AWREADY;
      wor  sc_slave_axi_W_channel_t       ifc_ust_smb_axi_W;
      wor                                 ifc_ust_smb_axi_WVALID;
      wand                                ifc_ust_smb_axi_WREADY;

      wor                                 ms_dv_force_dmactive;

      wor                                 u0_umctl2_ecc_uncorrected_err_intr_vmc;
      wor                                 u0_umctl2_dfi_alert_err_intr_vmc;
      wor                                 u0_umctl2_ecc_corrected_err_intr_vmc;
      wor                                 u0_umctl2_sbr_done_intr_vmc;
      wor                                 u0_umctl2_derate_temp_limit_intr_vmc;
      wor                                 dfi0_error_vmc;
      wor  [`DDR_DFI_ERR_SIZE-1:0]        dfi0_error_info_vmc;
      wor                                 u1_umctl2_ecc_uncorrected_err_intr_vmc;
      wor                                 u1_umctl2_dfi_alert_err_intr_vmc;
      wor                                 u1_umctl2_ecc_corrected_err_intr_vmc;
      wor                                 u1_umctl2_sbr_done_intr_vmc;
      wor                                 u1_umctl2_derate_temp_limit_intr_vmc;
      wor                                 dfi1_error_vmc;
      wor  [`DDR_DFI_ERR_SIZE-1:0]        dfi1_error_info_vmc;
      wand                                dwc_ddrphy_int_n_vmc;
      wor                                 p0_cntr_overflow_intr;
      wor                                 p1_cntr_overflow_intr;
      wor  [`MEMSHIRE_NOC_ERR_WIDTH-1:0]  noc_err_int_hv;
      wor                                 memshire_critical_int_gv;
      wor                                 memshire_normal_int_gv;
   `endif
   

   wire                                           ms_auto_init_enable;
   wire   [`MS_AUTO_CONFIG_WIDTH-1:0]             ms_auto_init_config;
   wire                                           ms_auto_init_inprog;
   wire                                           dv_ms_auto_init_done;

  vclevel_shft_l2h ls_l2h_rst_sms            (.out_hv(rst_sms_vmc              ), .inp_lv(rst_sms_mc               ));   // goes to rst_sms_vmc_pre in real code, to get gated by internal reset


   `ifdef EVL_SIMULATION
      assign ifc_ust_smb_axi_AR                     = '0;
      assign ifc_ust_smb_axi_ARVALID                = 1'b0;
      assign ifc_ust_smb_axi_ARREADY                = 1'b1;
      assign ifc_ust_smb_axi_AW                     = '0;
      assign ifc_ust_smb_axi_AWVALID                = 1'b0;
      assign ifc_ust_smb_axi_AWREADY                = 1'b1;
      assign ifc_ust_smb_axi_W                      = '0;
      assign ifc_ust_smb_axi_WVALID                 = 1'b0;
      assign ifc_ust_smb_axi_WREADY                 = 1'b1;
      assign ms_dv_force_dmactive                   = 1'b0;
      assign u0_umctl2_ecc_uncorrected_err_intr_vmc = 1'b0;
      assign u0_umctl2_dfi_alert_err_intr_vmc       = 1'b0;
      assign u0_umctl2_ecc_corrected_err_intr_vmc   = 1'b0;
      assign u0_umctl2_sbr_done_intr_vmc            = 1'b0;
      assign u0_umctl2_derate_temp_limit_intr_vmc   = 1'b0;
      assign dfi0_error_vmc                         = 1'b0;
      assign dfi0_error_info_vmc                    = {`DDR_DFI_ERR_SIZE{1'b0}};
      assign u1_umctl2_ecc_uncorrected_err_intr_vmc = 1'b0;
      assign u1_umctl2_dfi_alert_err_intr_vmc       = 1'b0;
      assign u1_umctl2_ecc_corrected_err_intr_vmc   = 1'b0;
      assign u1_umctl2_sbr_done_intr_vmc            = 1'b0;
      assign u1_umctl2_derate_temp_limit_intr_vmc   = 1'b0;
      assign dfi1_error_vmc                         = 1'b0;
      assign dfi1_error_info_vmc                    = {`DDR_DFI_ERR_SIZE{1'b0}};
      assign dwc_ddrphy_int_n_vmc                   = 1'b1;
      assign p0_cntr_overflow_intr                  = 1'b0;
      assign p1_cntr_overflow_intr                  = 1'b0;
      assign noc_err_int_hv                         = {`MEMSHIRE_NOC_ERR_WIDTH{1'b0}};
      assign memshire_critical_int_gv               = 1'b0;
      assign memshire_normal_int_gv                 = 1'b0;
      assign enable_clock_debug                     = ~reset_c;
   `endif

endmodule

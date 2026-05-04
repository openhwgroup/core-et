
module evl_memshire_perfmon_itf #(
   parameter
   ESR_PERFMON_NUM_PERF_CNTR   = 2,
   ESR_PERFMON_STATUS_LSB      = 32,
   ESR_PERFMON_STATUS_SIZE     = 6,
   ESR_PERFMON_CSR_SIZE        = ESR_PERFMON_STATUS_LSB+ESR_PERFMON_STATUS_SIZE
)(
   input  logic                        clock,
   input  logic                        reset,

   input logic [ESR_PERFMON_NUM_PERF_CNTR-1:0]   perfmon_enable,
   input logic [ESR_PERFMON_CSR_SIZE-1:0]        esr_perfmon_ctl_status,

   // Performance monitor for resource and event
   //output sc_perfmon_req_info_t        perfmon_p0_req_info, 
   //input  sc_perfmon_rsp_info_t        perfmon_p0_rsp_info, 
   //output sc_perfmon_req_info_t        perfmon_p1_req_info, 
   //input  sc_perfmon_rsp_info_t        perfmon_p1_rsp_info, 

   // Performance monitor saturation errors 
   // output logic                                       cyc_cntr_overflow_intr,
   // output logic                                       p0_cntr_overflow_intr,
   // output logic                                       p1_cntr_overflow_intr,

   // ESRs

   input  ddrc_esr_ctl_t                               esr_ddrc_ctl_vmc,
   input  memshire_esr_ctl_t                           esr_memshire_ctl,
   // output ms_perfmon_status_t                          esr_perfmon_status,

  // interface with debug logic
   input  logic                                         trace_en,
   // output logic [`MEMSHIRE_PERF_OP_SIGS_WIDTH-1:0]      perf_op_sigs,
   // output logic                                         perf_op_sigs_vld,
   // output logic [`MEMSHIRE_MEM_DEBUG_SIGS_WIDTH-1:0]    mem_debug_sigs,

 // memory controller signals
   input  logic [`MEMSHIRE_PERF_OP_SIGS_WIDTH-1:0]      mem_debug_sigs_op,

 // memory controller signals
   input  logic                                         u0_umctl2_perf_op_is_rd_vmc,
   input  logic                                         u0_umctl2_perf_op_is_wr_vmc,
   input  logic                                         u0_umctl2_perf_op_is_mwr_vmc,
   input  logic                                         u0_umctl2_perf_op_is_activate_vmc,
   input  logic                                         u0_umctl2_perf_op_is_precharge_vmc,
   input  logic                                         u0_umctl2_perf_op_is_refresh_vmc,
   input  logic  [`DDR_BANKS_SIZE-1:0]                  u0_umctl2_perf_bank_vmc,
   input  logic                                         u0_umctl2_perf_precharge_for_rdwr_vmc,
   input  logic                                         u0_umctl2_perf_precharge_for_other_vmc,
   input  logic                                         u0_umctl2_perf_op_is_crit_ref_vmc,
   input  logic                                         u0_umctl2_perf_op_is_spec_ref_vmc,
   input  logic                                         u0_umctl2_perf_op_is_load_mode_vmc,
   input  logic                                         u0_umctl2_perf_op_is_zqcl_vmc,
   input  logic                                         u0_umctl2_perf_op_is_zqcs_vmc,
   input  logic                                         u0_umctl2_perf_op_is_rd_activate_vmc,
   input  logic                                         u0_umctl2_perf_op_is_enter_selfref_vmc,
   input  logic                                         u0_umctl2_perf_op_is_enter_powerdown_vmc,
   input  logic                                         u0_umctl2_perf_op_is_enter_deeppowerdown_vmc,
   input  logic                                         u1_umctl2_perf_op_is_rd_vmc,
   input  logic                                         u1_umctl2_perf_op_is_wr_vmc,
   input  logic                                         u1_umctl2_perf_op_is_mwr_vmc,
   input  logic                                         u1_umctl2_perf_op_is_activate_vmc,
   input  logic                                         u1_umctl2_perf_op_is_precharge_vmc,
   input  logic                                         u1_umctl2_perf_op_is_refresh_vmc,
   input  logic  [`DDR_BANKS_SIZE-1:0]                  u1_umctl2_perf_bank_vmc,
   input  logic                                         u1_umctl2_perf_precharge_for_rdwr_vmc,
   input  logic                                         u1_umctl2_perf_precharge_for_other_vmc,
   input  logic                                         u1_umctl2_perf_op_is_crit_ref_vmc,
   input  logic                                         u1_umctl2_perf_op_is_spec_ref_vmc,
   input  logic                                         u1_umctl2_perf_op_is_load_mode_vmc,
   input  logic                                         u1_umctl2_perf_op_is_zqcl_vmc,
   input  logic                                         u1_umctl2_perf_op_is_zqcs_vmc,
   input  logic                                         u1_umctl2_perf_op_is_rd_activate_vmc,
   input  logic                                         u1_umctl2_perf_op_is_enter_selfref_vmc,
   input  logic                                         u1_umctl2_perf_op_is_enter_powerdown_vmc,
   input  logic                                         u1_umctl2_perf_op_is_enter_deeppowerdown_vmc,
   input  logic [`DDR_DBG_DFI_IE_SIZE-1:0]              u0_umctl2_dbg_dfi_ie_cmd_type_vmc,
   input  logic                                         u0_umctl2_perf_dfi_wr_data_cycles_vmc,
   input  logic                                         u0_umctl2_perf_dfi_rd_data_cycles_vmc,
   input  logic                                         u0_umctl2_ctl_idle_vmc,
   input  logic                                         u0_umctl2_perf_rdwr_transitions_vmc,
   input  logic                                         u0_umctl2_perf_write_combine_vmc,
   input  logic                                         u0_umctl2_perf_war_hazard_vmc,
   input  logic                                         u0_umctl2_perf_raw_hazard_vmc,
   input  logic                                         u0_umctl2_perf_waw_hazard_vmc,
   input  logic                                         u0_umctl2_perf_ie_blk_hazard_vmc,
   input  logic [`DDR_DFI_CS_SIZE-1:0]                  u0_umctl2_dfi_cs_vmc,
   input  logic                                         u0_umctl2_perf_hif_rd_or_wr_vmc,
   input  logic                                         u0_umctl2_perf_hif_wr_vmc,
   input  logic                                         u0_umctl2_perf_hif_rd_vmc,
   input  logic                                         u0_umctl2_perf_hif_rmw_vmc,
   input  logic                                         u0_umctl2_perf_hif_hi_pri_rd_vmc,
   input  logic [`DDR_BANKS_SIZE-1:0]                   u0_umctl2_hif_refresh_req_bank_vmc,
   input  logic                                         u0_umctl2_perf_hpr_req_with_nocredit_vmc,
   input  logic                                         u0_umctl2_perf_lpr_req_with_nocredit_vmc,
   input  logic                                         u0_umctl2_perf_hpr_xact_when_critical_vmc,
   input  logic                                         u0_umctl2_perf_lpr_xact_when_critical_vmc,
   input  logic                                         u0_umctl2_perf_wr_xact_when_critical_vmc,
   input  logic                                         u0_umctl2_perf_selfref_mode_vmc,
   input  logic                                         u0_umctl2_waq_pop_0_vmc,
   input  logic                                         u0_umctl2_waq_push_0_vmc,
   input  logic                                         u0_umctl2_raq_pop_0_vmc,
   input  logic                                         u0_umctl2_raq_push_0_vmc,
   input  logic                                         u0_umctl2_waq_pop_1_vmc,
   input  logic                                         u0_umctl2_waq_push_1_vmc,
   input  logic                                         u0_umctl2_raq_pop_1_vmc,
   input  logic                                         u0_umctl2_raq_push_1_vmc,
   input  logic [`DDR_CRD_CNT_SIZE-1:0]                 u0_umctl2_lpr_credit_cnt_vmc,
   input  logic [`DDR_CRD_CNT_SIZE-1:0]                 u0_umctl2_hpr_credit_cnt_vmc,
   input  logic [`DDR_CRD_CNT_SIZE-1:0]                 u0_umctl2_wr_credit_cnt_vmc,
   input  logic [`DDR_CRD_CNT_SIZE-1:0]                 u0_umctl2_wrecc_credit_cnt_vmc,
   input  logic [`DDR_XAQ_WCOUNT_SIZE-1:0]              u0_umctl2_waq_wcount_0_vmc,
   input  logic [`DDR_XAQ_WCOUNT_SIZE-1:0]              u0_umctl2_raq_wcount_0_vmc,
   input  logic [`DDR_XAQ_WCOUNT_SIZE-1:0]              u0_umctl2_waq_wcount_1_vmc,
   input  logic [`DDR_XAQ_WCOUNT_SIZE-1:0]              u0_umctl2_raq_wcount_1_vmc,
   input  logic                                         u0_umctl2_sbr_done_intr_vmc,
   input  logic                                         u0_umctl2_dfi_alert_err_intr_vmc,
   input  logic                                         u0_umctl2_ecc_corrected_err_intr_vmc,
   input  logic                                         u0_umctl2_ecc_corrected_err_intr_fault_vmc,
   input  logic                                         u0_umctl2_ecc_uncorrected_err_intr_vmc,
   input  logic                                         u0_umctl2_ecc_uncorrected_err_intr_fault_vmc,
   input  logic                                         u0_umctl2_derate_temp_limit_intr_vmc,
   input  logic                                         u0_umctl2_derate_temp_limit_intr_fault_vmc,
   input  logic                                         dwc_ddrphy_int_n_vmc,
   input  logic                                         dfi0_error_vmc,
   input  logic                                         dfi1_error_vmc,
   input  logic                                         u1_umctl2_perf_dfi_wr_data_cycles_vmc,
   input  logic [`DDR_DBG_DFI_IE_SIZE-1:0]              u1_umctl2_dbg_dfi_ie_cmd_type_vmc,
   input  logic                                         u1_umctl2_perf_dfi_rd_data_cycles_vmc,
   input  logic                                         u1_umctl2_ctl_idle_vmc,
   input  logic                                         u1_umctl2_perf_rdwr_transitions_vmc,
   input  logic                                         u1_umctl2_perf_write_combine_vmc,
   input  logic                                         u1_umctl2_perf_war_hazard_vmc,
   input  logic                                         u1_umctl2_perf_raw_hazard_vmc,
   input  logic                                         u1_umctl2_perf_waw_hazard_vmc,
   input  logic                                         u1_umctl2_perf_ie_blk_hazard_vmc,
   input  logic [`DDR_DFI_CS_SIZE-1:0]                  u1_umctl2_dfi_cs_vmc,
   input  logic                                         u1_umctl2_perf_hif_rd_or_wr_vmc,
   input  logic                                         u1_umctl2_perf_hif_wr_vmc,
   input  logic                                         u1_umctl2_perf_hif_rd_vmc,
   input  logic                                         u1_umctl2_perf_hif_rmw_vmc,
   input  logic                                         u1_umctl2_perf_hif_hi_pri_rd_vmc,
   input  logic [`DDR_BANKS_SIZE-1:0]                   u1_umctl2_hif_refresh_req_bank_vmc,
   input  logic                                         u1_umctl2_perf_hpr_req_with_nocredit_vmc,
   input  logic                                         u1_umctl2_perf_lpr_req_with_nocredit_vmc,
   input  logic                                         u1_umctl2_perf_hpr_xact_when_critical_vmc,
   input  logic                                         u1_umctl2_perf_lpr_xact_when_critical_vmc,
   input  logic                                         u1_umctl2_perf_wr_xact_when_critical_vmc,
   input  logic                                         u1_umctl2_perf_selfref_mode_vmc,
   input  logic                                         u1_umctl2_waq_pop_0_vmc,
   input  logic                                         u1_umctl2_waq_push_0_vmc,
   input  logic                                         u1_umctl2_raq_pop_0_vmc,
   input  logic                                         u1_umctl2_raq_push_0_vmc,
   input  logic                                         u1_umctl2_waq_pop_1_vmc,
   input  logic                                         u1_umctl2_waq_push_1_vmc,
   input  logic                                         u1_umctl2_raq_pop_1_vmc,
   input  logic                                         u1_umctl2_raq_push_1_vmc,
   input  logic [`DDR_CRD_CNT_SIZE-1:0]                 u1_umctl2_lpr_credit_cnt_vmc,
   input  logic [`DDR_CRD_CNT_SIZE-1:0]                 u1_umctl2_hpr_credit_cnt_vmc,
   input  logic [`DDR_CRD_CNT_SIZE-1:0]                 u1_umctl2_wr_credit_cnt_vmc,
   input  logic [`DDR_CRD_CNT_SIZE-1:0]                 u1_umctl2_wrecc_credit_cnt_vmc,
   input  logic [`DDR_XAQ_WCOUNT_SIZE-1:0]              u1_umctl2_waq_wcount_0_vmc,
   input  logic [`DDR_XAQ_WCOUNT_SIZE-1:0]              u1_umctl2_raq_wcount_0_vmc,
   input  logic [`DDR_XAQ_WCOUNT_SIZE-1:0]              u1_umctl2_waq_wcount_1_vmc,
   input  logic [`DDR_XAQ_WCOUNT_SIZE-1:0]              u1_umctl2_raq_wcount_1_vmc,
   input  logic                                         u1_umctl2_sbr_done_intr_vmc,
   input  logic                                         u1_umctl2_dfi_alert_err_intr_vmc,
   input  logic                                         u1_umctl2_ecc_corrected_err_intr_vmc,
   input  logic                                         u1_umctl2_ecc_corrected_err_intr_fault_vmc,
   input  logic                                         u1_umctl2_ecc_uncorrected_err_intr_vmc,
   input  logic                                         u1_umctl2_ecc_uncorrected_err_intr_fault_vmc,
   input  logic                                         u1_umctl2_derate_temp_limit_intr_vmc,
   input  logic                                         u1_umctl2_derate_temp_limit_intr_fault_vmc,

   input  sc_slave_axi_AR_channel_t                     mem_mesh_slave_axi_AR_debug_bm,
   input  logic                                         mem_mesh_slave_axi_ARVALID_debug_bm,
   input  logic                                         mem_mesh_slave_axi_ARREADY_debug_bm,
   input  sc_slave_axi_AW_channel_t                     mem_mesh_slave_axi_AW_debug_bm,
   input  logic                                         mem_mesh_slave_axi_AWVALID_debug_bm,
   input  logic                                         mem_mesh_slave_axi_AWREADY_debug_bm,
   input  sc_slave_axi_B_channel_t                      mem_mesh_slave_axi_B_debug_bm,
   input  logic                                         mem_mesh_slave_axi_BVALID_debug_bm,
   input  logic                                         mem_mesh_slave_axi_BREADY_debug_bm,
   input  sc_slave_axi_R_channel_t                      mem_mesh_slave_axi_R_debug_bm,
   input  logic                                         mem_mesh_slave_axi_RVALID_debug_bm,
   input  logic                                         mem_mesh_slave_axi_RREADY_debug_bm

);

import uvm_pkg::*;
import evl_base_pkg::*;

   localparam ESR_PERFMON_P0_MODE         = 9;
   localparam ESR_PERFMON_P1_MODE         = 22;
   localparam ESR_PERFMON_MODE_SIZE       = 8;

   string s;
   string sinfo;
   string sinfo2;

   memshire_esr_ctl_t   esr_memshire_ctl_r;
   logic [1:0]          perfmon_enable_r;
   logic [38-1:0]       esr_perfmon_ctl_status_r;

   always @(posedge clock) begin
     if (reset) begin
        esr_memshire_ctl_r <= '0;
        perfmon_enable_r  <= '0;
        esr_perfmon_ctl_status_r  <= '0;
     end
     else begin
        if (esr_memshire_ctl_r != esr_memshire_ctl) begin
           esr_memshire_ctl_r <= esr_memshire_ctl;
           s = "esr_memshire_ctl" ; 
           sinfo = $sformatf("esr_ms_mc_en %0x esr_ms_addr_mc_bit_sel %0x", esr_memshire_ctl.esr_ms_mc_en, esr_memshire_ctl.esr_ms_addr_mc_bit_sel);
           `evl_log_no_name(UVM_MEDIUM, VMOD_CONFIG, $sformatf("Memshire Perfmon %0s %s",s, sinfo )) 
        end
        if (perfmon_enable_r != perfmon_enable) begin
           perfmon_enable_r  <= perfmon_enable;
           s = "esr_memshire_ctl" ; 
           sinfo = $sformatf("perfmon_enable %0x", perfmon_enable);
           `evl_log_no_name(UVM_MEDIUM, VMOD_CONFIG, $sformatf("Memshire Perfmon %0s %s",s, sinfo )) 
        end
        if (esr_perfmon_ctl_status_r != esr_perfmon_ctl_status) begin
           esr_perfmon_ctl_status_r  <= esr_perfmon_ctl_status;
           s = "esr_memshire_ctl" ; 
           sinfo = $sformatf("perfmon_mode0 0x%0x perfmon_mode1 0x%0x", esr_perfmon_ctl_status[ESR_PERFMON_P0_MODE+:ESR_PERFMON_MODE_SIZE], esr_perfmon_ctl_status[ESR_PERFMON_P1_MODE+:ESR_PERFMON_MODE_SIZE]);
           `evl_log_no_name(UVM_MEDIUM, VMOD_CONFIG, $sformatf("Memshire Perfmon %0s %s",s, sinfo )) 
        end
     end
   end

   logic u1_umctl2_perf_op_is_enter_deeppowerdown_p1;
   logic u1_umctl2_perf_op_is_enter_powerdown_p1;
   logic u1_umctl2_perf_op_is_enter_selfref_p1;
   logic u1_umctl2_perf_op_is_rd_activate_p1;
   logic u1_umctl2_perf_op_is_zqcs_p1;
   logic u1_umctl2_perf_op_is_zqcl_p1;
   logic u1_umctl2_perf_op_is_load_mode_p1;
   logic u1_umctl2_perf_op_is_spec_ref_p1;
   logic u1_umctl2_perf_op_is_crit_ref_p1;
   logic u1_umctl2_perf_precharge_for_other_p1;
   logic u1_umctl2_perf_precharge_for_rdwr_p1;
   logic [`DDR_BANKS_SIZE-1:0]  u1_umctl2_perf_bank_p1;
   logic u1_umctl2_perf_op_is_refresh_p1;
   logic u1_umctl2_perf_op_is_precharge_p1;
   logic u1_umctl2_perf_op_is_activate_p1;
   logic u1_umctl2_perf_op_is_mwr_p1;
   logic u1_umctl2_perf_op_is_wr_p1;
   logic u1_umctl2_perf_op_is_rd_p1;
                                     
   logic u0_umctl2_perf_op_is_enter_deeppowerdown_p1;
   logic u0_umctl2_perf_op_is_enter_powerdown_p1;
   logic u0_umctl2_perf_op_is_enter_selfref_p1;
   logic u0_umctl2_perf_op_is_rd_activate_p1;
   logic u0_umctl2_perf_op_is_zqcs_p1;
   logic u0_umctl2_perf_op_is_zqcl_p1;
   logic u0_umctl2_perf_op_is_load_mode_p1;
   logic u0_umctl2_perf_op_is_spec_ref_p1;
   logic u0_umctl2_perf_op_is_crit_ref_p1;
   logic u0_umctl2_perf_precharge_for_other_p1;
   logic u0_umctl2_perf_precharge_for_rdwr_p1;
   logic [`DDR_BANKS_SIZE-1:0]  u0_umctl2_perf_bank_p1;
   logic u0_umctl2_perf_op_is_refresh_p1;
   logic u0_umctl2_perf_op_is_precharge_p1;
   logic u0_umctl2_perf_op_is_activate_p1;
   logic u0_umctl2_perf_op_is_mwr_p1;
   logic u0_umctl2_perf_op_is_wr_p1;
   logic u0_umctl2_perf_op_is_rd_p1;

   assign { u1_umctl2_perf_op_is_enter_deeppowerdown_p1,
            u1_umctl2_perf_op_is_enter_powerdown_p1,
            u1_umctl2_perf_op_is_enter_selfref_p1,
            u1_umctl2_perf_op_is_rd_activate_p1,
            u1_umctl2_perf_op_is_zqcs_p1,
            u1_umctl2_perf_op_is_zqcl_p1,
            u1_umctl2_perf_op_is_load_mode_p1,
            u1_umctl2_perf_op_is_spec_ref_p1,
            u1_umctl2_perf_op_is_crit_ref_p1,
            u1_umctl2_perf_precharge_for_other_p1,
            u1_umctl2_perf_precharge_for_rdwr_p1,
            u1_umctl2_perf_bank_p1,
            u1_umctl2_perf_op_is_refresh_p1,
            u1_umctl2_perf_op_is_precharge_p1,
            u1_umctl2_perf_op_is_activate_p1,
            u1_umctl2_perf_op_is_mwr_p1,
            u1_umctl2_perf_op_is_wr_p1,
            u1_umctl2_perf_op_is_rd_p1,
                                     
            u0_umctl2_perf_op_is_enter_deeppowerdown_p1,
            u0_umctl2_perf_op_is_enter_powerdown_p1,
            u0_umctl2_perf_op_is_enter_selfref_p1,
            u0_umctl2_perf_op_is_rd_activate_p1,
            u0_umctl2_perf_op_is_zqcs_p1,
            u0_umctl2_perf_op_is_zqcl_p1,
            u0_umctl2_perf_op_is_load_mode_p1,
            u0_umctl2_perf_op_is_spec_ref_p1,
            u0_umctl2_perf_op_is_crit_ref_p1,
            u0_umctl2_perf_precharge_for_other_p1,
            u0_umctl2_perf_precharge_for_rdwr_p1,
            u0_umctl2_perf_bank_p1,
            u0_umctl2_perf_op_is_refresh_p1,
            u0_umctl2_perf_op_is_precharge_p1,
            u0_umctl2_perf_op_is_activate_p1,
            u0_umctl2_perf_op_is_mwr_p1,
            u0_umctl2_perf_op_is_wr_p1,
            u0_umctl2_perf_op_is_rd_p1} = mem_debug_sigs_op;

   always @(posedge clock) begin
      if (dfi0_error_vmc) begin s = "dfi0_error_vmc" ; `evl_log_no_name(UVM_MEDIUM, VMOD_CONFIG, $sformatf("Memshire Perfmon %0s",s )) end
      if (dfi1_error_vmc) begin s = "dfi1_error_vmc" ; `evl_log_no_name(UVM_MEDIUM, VMOD_CONFIG, $sformatf("Memshire Perfmon %0s",s )) end
      // if (dwc_ddrphy_int_n_vmc) begin s = "dwc_ddrphy_int_n_vmc" ; `evl_log_no_name(UVM_MEDIUM, VMOD_CONFIG, $sformatf("Memshire Perfmon %0s",s )) end
      if (dfi1_error_vmc) begin s = "dfi1_error_vmc" ; `evl_log_no_name(UVM_MEDIUM, VMOD_CONFIG, $sformatf("Memshire Perfmon %0s",s )) end
      // if (dwc_ddrphy_int_n_vmc) begin s = "dwc_ddrphy_int_n_vmc" ; `evl_log_no_name(UVM_MEDIUM, VMOD_CONFIG, $sformatf("Memshire Perfmon %0s",s )) end

      // outstd request needs to count time when there is no axi activites and the outstd is within range
      if (~(mem_mesh_slave_axi_ARREADY_debug_bm & mem_mesh_slave_axi_ARVALID_debug_bm) &
          ~(mem_mesh_slave_axi_RREADY_debug_bm & mem_mesh_slave_axi_RVALID_debug_bm) &
          ~(mem_mesh_slave_axi_AWREADY_debug_bm & mem_mesh_slave_axi_AWVALID_debug_bm) &
          ~(mem_mesh_slave_axi_BREADY_debug_bm & mem_mesh_slave_axi_BVALID_debug_bm)) begin
          s = "mem_mesh_slave_axi_AXI_notvalid_debug_bm" ; 
          `evl_log_no_name(UVM_MEDIUM, VMOD_CONFIG, $sformatf("Memshire Perfmon %0s",s)) 
      end
      if (~(mem_mesh_slave_axi_ARREADY_debug_bm & mem_mesh_slave_axi_ARVALID_debug_bm) &
          ~(mem_mesh_slave_axi_RREADY_debug_bm & mem_mesh_slave_axi_RVALID_debug_bm)) begin
          s = "mem_mesh_slave_axi_AR_notvalid_debug_bm" ; 
          `evl_log_no_name(UVM_MEDIUM, VMOD_CONFIG, $sformatf("Memshire Perfmon %0s",s)) 
      end
      if (~(mem_mesh_slave_axi_AWREADY_debug_bm & mem_mesh_slave_axi_AWVALID_debug_bm) &
          ~(mem_mesh_slave_axi_BREADY_debug_bm & mem_mesh_slave_axi_BVALID_debug_bm)) begin
          s = "mem_mesh_slave_axi_AW_notvalid_debug_bm" ; 
          `evl_log_no_name(UVM_MEDIUM, VMOD_CONFIG, $sformatf("Memshire Perfmon %0s",s)) 
      end
      if (mem_mesh_slave_axi_ARREADY_debug_bm & mem_mesh_slave_axi_ARVALID_debug_bm) begin
          s = "mem_mesh_slave_axi_AR_debug_bm" ; 
          sinfo = $sformatf("ID %0x ADDR %0x QOS %0x LEN %0x SIZE %0x BURST %0x LOCK %0x CACHE %0x PROT %0x USER %0x",
             mem_mesh_slave_axi_AR_debug_bm.ID, mem_mesh_slave_axi_AR_debug_bm.ADDR, 
             mem_mesh_slave_axi_AR_debug_bm.QOS,
             mem_mesh_slave_axi_AR_debug_bm.LEN, mem_mesh_slave_axi_AR_debug_bm.SIZE, mem_mesh_slave_axi_AR_debug_bm.BURST,
             mem_mesh_slave_axi_AR_debug_bm.LOCK, mem_mesh_slave_axi_AR_debug_bm.CACHE, mem_mesh_slave_axi_AR_debug_bm.PROT, 
             mem_mesh_slave_axi_AR_debug_bm.USER);
          `evl_log_no_name(UVM_MEDIUM, VMOD_CONFIG, $sformatf("Memshire Perfmon %0s %s",s,sinfo )) 
      end
      if (mem_mesh_slave_axi_RREADY_debug_bm & mem_mesh_slave_axi_RVALID_debug_bm) begin 
          s = "mem_mesh_slave_axi_R_debug_bm" ; 
          sinfo = $sformatf("ID %0x RESP %0x LAST %0x DATA %0x", mem_mesh_slave_axi_R_debug_bm.ID, mem_mesh_slave_axi_R_debug_bm.RESP, mem_mesh_slave_axi_R_debug_bm.LAST, mem_mesh_slave_axi_R_debug_bm.DATA);
          `evl_log_no_name(UVM_MEDIUM, VMOD_CONFIG, $sformatf("Memshire Perfmon %0s %s",s,sinfo )) 
      end
      if (mem_mesh_slave_axi_AWREADY_debug_bm & mem_mesh_slave_axi_AWVALID_debug_bm) begin
          s = "mem_mesh_slave_axi_AW_debug_bm" ; 
          sinfo = $sformatf("ID %0x ADDR %0x QOS %0x LEN %0x SIZE %0x BURST %0x LOCK %0x CACHE %0x PROT %0x USER %0x",
             mem_mesh_slave_axi_AW_debug_bm.ID, mem_mesh_slave_axi_AW_debug_bm.ADDR, 
             mem_mesh_slave_axi_AW_debug_bm.QOS,
             mem_mesh_slave_axi_AW_debug_bm.LEN, mem_mesh_slave_axi_AW_debug_bm.SIZE, mem_mesh_slave_axi_AW_debug_bm.BURST,
             mem_mesh_slave_axi_AW_debug_bm.LOCK, mem_mesh_slave_axi_AW_debug_bm.CACHE, mem_mesh_slave_axi_AW_debug_bm.PROT, 
             mem_mesh_slave_axi_AW_debug_bm.USER);
          `evl_log_no_name(UVM_MEDIUM, VMOD_CONFIG, $sformatf("Memshire Perfmon %0s %s",s,sinfo )) 
      end
      if (mem_mesh_slave_axi_BREADY_debug_bm & mem_mesh_slave_axi_BVALID_debug_bm) begin 
          s = "mem_mesh_slave_axi_B_debug_bm" ; 
          sinfo = $sformatf("ID %0x RESP %0x", mem_mesh_slave_axi_B_debug_bm.ID, mem_mesh_slave_axi_B_debug_bm.RESP);
          `evl_log_no_name(UVM_MEDIUM, VMOD_CONFIG, $sformatf("Memshire Perfmon %0s %s",s,sinfo )) 
      end


      if (u0_umctl2_ctl_idle_vmc) begin s = "u0_umctl2_ctl_idle_vmc" ; `evl_log_no_name(UVM_MEDIUM, VMOD_CONFIG, $sformatf("Memshire Perfmon %0s",s )) end
      if (u0_umctl2_derate_temp_limit_intr_fault_vmc) begin s = "u0_umctl2_derate_temp_limit_intr_fault_vmc" ; `evl_log_no_name(UVM_MEDIUM, VMOD_CONFIG, $sformatf("Memshire Perfmon %0s",s )) end
      if (u0_umctl2_derate_temp_limit_intr_vmc) begin s = "u0_umctl2_derate_temp_limit_intr_vmc" ; `evl_log_no_name(UVM_MEDIUM, VMOD_CONFIG, $sformatf("Memshire Perfmon %0s",s )) end
      if (u0_umctl2_dfi_alert_err_intr_vmc) begin s = "u0_umctl2_dfi_alert_err_intr_vmc" ; `evl_log_no_name(UVM_MEDIUM, VMOD_CONFIG, $sformatf("Memshire Perfmon %0s",s )) end
      if (u0_umctl2_ecc_corrected_err_intr_fault_vmc) begin s = "u0_umctl2_ecc_corrected_err_intr_fault_vmc" ; `evl_log_no_name(UVM_MEDIUM, VMOD_CONFIG, $sformatf("Memshire Perfmon %0s",s )) end
      if (u0_umctl2_ecc_corrected_err_intr_vmc) begin s = "u0_umctl2_ecc_corrected_err_intr_vmc" ; `evl_log_no_name(UVM_MEDIUM, VMOD_CONFIG, $sformatf("Memshire Perfmon %0s",s )) end
      if (u0_umctl2_ecc_uncorrected_err_intr_fault_vmc) begin s = "u0_umctl2_ecc_uncorrected_err_intr_fault_vmc" ; `evl_log_no_name(UVM_MEDIUM, VMOD_CONFIG, $sformatf("Memshire Perfmon %0s",s )) end
      if (u0_umctl2_ecc_uncorrected_err_intr_vmc) begin s = "u0_umctl2_ecc_uncorrected_err_intr_vmc" ; `evl_log_no_name(UVM_MEDIUM, VMOD_CONFIG, $sformatf("Memshire Perfmon %0s",s )) end
      if (u0_umctl2_perf_dfi_rd_data_cycles_vmc) begin s = "u0_umctl2_perf_dfi_rd_data_cycles_vmc" ; `evl_log_no_name(UVM_MEDIUM, VMOD_CONFIG, $sformatf("Memshire Perfmon %0s",s )) end
      if (u0_umctl2_perf_dfi_wr_data_cycles_vmc) begin s = "u0_umctl2_perf_dfi_wr_data_cycles_vmc" ; `evl_log_no_name(UVM_MEDIUM, VMOD_CONFIG, $sformatf("Memshire Perfmon %0s",s )) end
      if (u0_umctl2_perf_hif_hi_pri_rd_vmc) begin s = "u0_umctl2_perf_hif_hi_pri_rd_vmc" ; `evl_log_no_name(UVM_MEDIUM, VMOD_CONFIG, $sformatf("Memshire Perfmon %0s",s )) end
      if (u0_umctl2_perf_hif_rd_or_wr_vmc) begin s = "u0_umctl2_perf_hif_rd_or_wr_vmc" ; `evl_log_no_name(UVM_MEDIUM, VMOD_CONFIG, $sformatf("Memshire Perfmon %0s",s )) end
      if (u0_umctl2_perf_hif_rd_vmc) begin s = "u0_umctl2_perf_hif_rd_vmc" ; `evl_log_no_name(UVM_MEDIUM, VMOD_CONFIG, $sformatf("Memshire Perfmon %0s",s )) end
      if (u0_umctl2_perf_hif_rmw_vmc) begin s = "u0_umctl2_perf_hif_rmw_vmc" ; `evl_log_no_name(UVM_MEDIUM, VMOD_CONFIG, $sformatf("Memshire Perfmon %0s",s )) end
      if (u0_umctl2_perf_hif_wr_vmc) begin s = "u0_umctl2_perf_hif_wr_vmc" ; `evl_log_no_name(UVM_MEDIUM, VMOD_CONFIG, $sformatf("Memshire Perfmon %0s",s )) end
      if (u0_umctl2_perf_hpr_req_with_nocredit_vmc) begin s = "u0_umctl2_perf_hpr_req_with_nocredit_vmc" ; `evl_log_no_name(UVM_MEDIUM, VMOD_CONFIG, $sformatf("Memshire Perfmon %0s",s )) end
      if (u0_umctl2_perf_hpr_xact_when_critical_vmc) begin s = "u0_umctl2_perf_hpr_xact_when_critical_vmc" ; `evl_log_no_name(UVM_MEDIUM, VMOD_CONFIG, $sformatf("Memshire Perfmon %0s",s )) end
      if (u0_umctl2_perf_ie_blk_hazard_vmc) begin s = "u0_umctl2_perf_ie_blk_hazard_vmc" ; `evl_log_no_name(UVM_MEDIUM, VMOD_CONFIG, $sformatf("Memshire Perfmon %0s",s )) end
      if (u0_umctl2_perf_lpr_req_with_nocredit_vmc) begin s = "u0_umctl2_perf_lpr_req_with_nocredit_vmc" ; `evl_log_no_name(UVM_MEDIUM, VMOD_CONFIG, $sformatf("Memshire Perfmon %0s",s )) end
      if (u0_umctl2_perf_lpr_xact_when_critical_vmc) begin s = "u0_umctl2_perf_lpr_xact_when_critical_vmc" ; `evl_log_no_name(UVM_MEDIUM, VMOD_CONFIG, $sformatf("Memshire Perfmon %0s",s )) end
      if (u0_umctl2_perf_op_is_activate_vmc) begin s = "u0_umctl2_perf_op_is_activate_vmc" ; `evl_log_no_name(UVM_MEDIUM, VMOD_CONFIG, $sformatf("Memshire Perfmon %0s",s )) end
      if (u0_umctl2_perf_op_is_crit_ref_vmc) begin s = "u0_umctl2_perf_op_is_crit_ref_vmc" ; `evl_log_no_name(UVM_MEDIUM, VMOD_CONFIG, $sformatf("Memshire Perfmon %0s",s )) end
      if (u0_umctl2_perf_op_is_enter_deeppowerdown_vmc) begin s = "u0_umctl2_perf_op_is_enter_deeppowerdown_vmc" ; `evl_log_no_name(UVM_MEDIUM, VMOD_CONFIG, $sformatf("Memshire Perfmon %0s",s )) end
      if (u0_umctl2_perf_op_is_enter_powerdown_vmc) begin s = "u0_umctl2_perf_op_is_enter_powerdown_vmc" ; `evl_log_no_name(UVM_MEDIUM, VMOD_CONFIG, $sformatf("Memshire Perfmon %0s",s )) end
      if (u0_umctl2_perf_op_is_enter_selfref_vmc) begin s = "u0_umctl2_perf_op_is_enter_selfref_vmc" ; `evl_log_no_name(UVM_MEDIUM, VMOD_CONFIG, $sformatf("Memshire Perfmon %0s",s )) end
      if (u0_umctl2_perf_op_is_load_mode_vmc) begin s = "u0_umctl2_perf_op_is_load_mode_vmc" ; `evl_log_no_name(UVM_MEDIUM, VMOD_CONFIG, $sformatf("Memshire Perfmon %0s",s )) end
      // if (u0_umctl2_perf_op_is_mwr_vmc) begin s = "u0_umctl2_perf_op_is_mwr_vmc" ; `evl_log_no_name(UVM_MEDIUM, VMOD_CONFIG, $sformatf("Memshire Perfmon %0s %0x",s, u0_umctl2_perf_bank_vmc )) end
      // if (u0_umctl2_perf_op_is_rd_vmc) begin s = "u0_umctl2_perf_op_is_rd_vmc" ; `evl_log_no_name(UVM_MEDIUM, VMOD_CONFIG, $sformatf("Memshire Perfmon %0s %0x",s, u0_umctl2_perf_bank_vmc )) end
      // if (u0_umctl2_perf_op_is_wr_vmc) begin s = "u0_umctl2_perf_op_is_wr_vmc" ; `evl_log_no_name(UVM_MEDIUM, VMOD_CONFIG, $sformatf("Memshire Perfmon %0s %0x",s, u0_umctl2_perf_bank_vmc )) end
      if (u0_umctl2_perf_op_is_mwr_p1) begin s = "u0_umctl2_perf_op_is_mwr_p1" ; `evl_log_no_name(UVM_MEDIUM, VMOD_CONFIG, $sformatf("Memshire Perfmon %0s %0x %0x",s, u0_umctl2_perf_bank_p1,u0_umctl2_dbg_dfi_ie_cmd_type_vmc )) end
      if (u0_umctl2_perf_op_is_rd_p1) begin s = "u0_umctl2_perf_op_is_rd_p1" ; `evl_log_no_name(UVM_MEDIUM, VMOD_CONFIG, $sformatf("Memshire Perfmon %0s %0x %0x",s, u0_umctl2_perf_bank_p1,u0_umctl2_dbg_dfi_ie_cmd_type_vmc )) end
      if (u0_umctl2_perf_op_is_wr_p1) begin s = "u0_umctl2_perf_op_is_wr_p1" ; `evl_log_no_name(UVM_MEDIUM, VMOD_CONFIG, $sformatf("Memshire Perfmon %0s %0x %0x",s, u0_umctl2_perf_bank_p1,u0_umctl2_dbg_dfi_ie_cmd_type_vmc )) end
      if (u0_umctl2_perf_op_is_precharge_vmc) begin s = "u0_umctl2_perf_op_is_precharge_vmc" ; `evl_log_no_name(UVM_MEDIUM, VMOD_CONFIG, $sformatf("Memshire Perfmon %0s",s )) end
      if (u0_umctl2_perf_op_is_rd_activate_vmc) begin s = "u0_umctl2_perf_op_is_rd_activate_vmc" ; `evl_log_no_name(UVM_MEDIUM, VMOD_CONFIG, $sformatf("Memshire Perfmon %0s",s )) end
      if (u0_umctl2_perf_op_is_refresh_vmc) begin s = "u0_umctl2_perf_op_is_refresh_vmc" ; `evl_log_no_name(UVM_MEDIUM, VMOD_CONFIG, $sformatf("Memshire Perfmon %0s",s )) end
      if (u0_umctl2_perf_op_is_spec_ref_vmc) begin s = "u0_umctl2_perf_op_is_spec_ref_vmc" ; `evl_log_no_name(UVM_MEDIUM, VMOD_CONFIG, $sformatf("Memshire Perfmon %0s",s )) end
      if (u0_umctl2_perf_op_is_zqcl_vmc) begin s = "u0_umctl2_perf_op_is_zqcl_vmc" ; `evl_log_no_name(UVM_MEDIUM, VMOD_CONFIG, $sformatf("Memshire Perfmon %0s",s )) end
      if (u0_umctl2_perf_op_is_zqcs_vmc) begin s = "u0_umctl2_perf_op_is_zqcs_vmc" ; `evl_log_no_name(UVM_MEDIUM, VMOD_CONFIG, $sformatf("Memshire Perfmon %0s",s )) end
      if (u0_umctl2_perf_precharge_for_other_vmc) begin s = "u0_umctl2_perf_precharge_for_other_vmc" ; `evl_log_no_name(UVM_MEDIUM, VMOD_CONFIG, $sformatf("Memshire Perfmon %0s",s )) end
      if (u0_umctl2_perf_precharge_for_rdwr_vmc) begin s = "u0_umctl2_perf_precharge_for_rdwr_vmc" ; `evl_log_no_name(UVM_MEDIUM, VMOD_CONFIG, $sformatf("Memshire Perfmon %0s",s )) end
      if (u0_umctl2_perf_raw_hazard_vmc) begin s = "u0_umctl2_perf_raw_hazard_vmc" ; `evl_log_no_name(UVM_MEDIUM, VMOD_CONFIG, $sformatf("Memshire Perfmon %0s",s )) end
      if (u0_umctl2_perf_rdwr_transitions_vmc) begin s = "u0_umctl2_perf_rdwr_transitions_vmc" ; `evl_log_no_name(UVM_MEDIUM, VMOD_CONFIG, $sformatf("Memshire Perfmon %0s",s )) end
      if (u0_umctl2_perf_selfref_mode_vmc) begin s = "u0_umctl2_perf_selfref_mode_vmc" ; `evl_log_no_name(UVM_MEDIUM, VMOD_CONFIG, $sformatf("Memshire Perfmon %0s",s )) end
      if (u0_umctl2_perf_war_hazard_vmc) begin s = "u0_umctl2_perf_war_hazard_vmc" ; `evl_log_no_name(UVM_MEDIUM, VMOD_CONFIG, $sformatf("Memshire Perfmon %0s",s )) end
      if (u0_umctl2_perf_waw_hazard_vmc) begin s = "u0_umctl2_perf_waw_hazard_vmc" ; `evl_log_no_name(UVM_MEDIUM, VMOD_CONFIG, $sformatf("Memshire Perfmon %0s",s )) end
      if (u0_umctl2_perf_write_combine_vmc) begin s = "u0_umctl2_perf_write_combine_vmc" ; `evl_log_no_name(UVM_MEDIUM, VMOD_CONFIG, $sformatf("Memshire Perfmon %0s",s )) end
      if (u0_umctl2_perf_wr_xact_when_critical_vmc) begin s = "u0_umctl2_perf_wr_xact_when_critical_vmc" ; `evl_log_no_name(UVM_MEDIUM, VMOD_CONFIG, $sformatf("Memshire Perfmon %0s",s )) end
      if (u0_umctl2_raq_pop_0_vmc) begin s = "u0_umctl2_raq_pop_0_vmc" ; `evl_log_no_name(UVM_MEDIUM, VMOD_CONFIG, $sformatf("Memshire Perfmon %0s",s )) end
      if (u0_umctl2_raq_pop_1_vmc) begin s = "u0_umctl2_raq_pop_1_vmc" ; `evl_log_no_name(UVM_MEDIUM, VMOD_CONFIG, $sformatf("Memshire Perfmon %0s",s )) end
      if (u0_umctl2_raq_push_0_vmc) begin s = "u0_umctl2_raq_push_0_vmc" ; `evl_log_no_name(UVM_MEDIUM, VMOD_CONFIG, $sformatf("Memshire Perfmon %0s",s )) end
      if (u0_umctl2_raq_push_1_vmc) begin s = "u0_umctl2_raq_push_1_vmc" ; `evl_log_no_name(UVM_MEDIUM, VMOD_CONFIG, $sformatf("Memshire Perfmon %0s",s )) end
      if (u0_umctl2_sbr_done_intr_vmc) begin s = "u0_umctl2_sbr_done_intr_vmc" ; `evl_log_no_name(UVM_MEDIUM, VMOD_CONFIG, $sformatf("Memshire Perfmon %0s",s )) end
      if (u0_umctl2_waq_pop_0_vmc) begin s = "u0_umctl2_waq_pop_0_vmc" ; `evl_log_no_name(UVM_MEDIUM, VMOD_CONFIG, $sformatf("Memshire Perfmon %0s",s )) end
      if (u0_umctl2_waq_pop_1_vmc) begin s = "u0_umctl2_waq_pop_1_vmc" ; `evl_log_no_name(UVM_MEDIUM, VMOD_CONFIG, $sformatf("Memshire Perfmon %0s",s )) end
      if (u0_umctl2_waq_push_0_vmc) begin s = "u0_umctl2_waq_push_0_vmc" ; `evl_log_no_name(UVM_MEDIUM, VMOD_CONFIG, $sformatf("Memshire Perfmon %0s",s )) end
      if (u0_umctl2_waq_push_1_vmc) begin s = "u0_umctl2_waq_push_1_vmc" ; `evl_log_no_name(UVM_MEDIUM, VMOD_CONFIG, $sformatf("Memshire Perfmon %0s",s )) end
      if (u1_umctl2_ctl_idle_vmc) begin s = "u1_umctl2_ctl_idle_vmc" ; `evl_log_no_name(UVM_MEDIUM, VMOD_CONFIG, $sformatf("Memshire Perfmon %0s",s )) end
      if (u1_umctl2_derate_temp_limit_intr_fault_vmc) begin s = "u1_umctl2_derate_temp_limit_intr_fault_vmc" ; `evl_log_no_name(UVM_MEDIUM, VMOD_CONFIG, $sformatf("Memshire Perfmon %0s",s )) end
      if (u1_umctl2_derate_temp_limit_intr_vmc) begin s = "u1_umctl2_derate_temp_limit_intr_vmc" ; `evl_log_no_name(UVM_MEDIUM, VMOD_CONFIG, $sformatf("Memshire Perfmon %0s",s )) end
      if (u1_umctl2_dfi_alert_err_intr_vmc) begin s = "u1_umctl2_dfi_alert_err_intr_vmc" ; `evl_log_no_name(UVM_MEDIUM, VMOD_CONFIG, $sformatf("Memshire Perfmon %0s",s )) end
      if (u1_umctl2_ecc_corrected_err_intr_fault_vmc) begin s = "u1_umctl2_ecc_corrected_err_intr_fault_vmc" ; `evl_log_no_name(UVM_MEDIUM, VMOD_CONFIG, $sformatf("Memshire Perfmon %0s",s )) end
      if (u1_umctl2_ecc_corrected_err_intr_vmc) begin s = "u1_umctl2_ecc_corrected_err_intr_vmc" ; `evl_log_no_name(UVM_MEDIUM, VMOD_CONFIG, $sformatf("Memshire Perfmon %0s",s )) end
      if (u1_umctl2_ecc_uncorrected_err_intr_fault_vmc) begin s = "u1_umctl2_ecc_uncorrected_err_intr_fault_vmc" ; `evl_log_no_name(UVM_MEDIUM, VMOD_CONFIG, $sformatf("Memshire Perfmon %0s",s )) end
      if (u1_umctl2_ecc_uncorrected_err_intr_vmc) begin s = "u1_umctl2_ecc_uncorrected_err_intr_vmc" ; `evl_log_no_name(UVM_MEDIUM, VMOD_CONFIG, $sformatf("Memshire Perfmon %0s",s )) end
      if (u1_umctl2_perf_dfi_rd_data_cycles_vmc) begin s = "u1_umctl2_perf_dfi_rd_data_cycles_vmc" ; `evl_log_no_name(UVM_MEDIUM, VMOD_CONFIG, $sformatf("Memshire Perfmon %0s",s )) end
      if (u1_umctl2_perf_dfi_wr_data_cycles_vmc) begin s = "u1_umctl2_perf_dfi_wr_data_cycles_vmc" ; `evl_log_no_name(UVM_MEDIUM, VMOD_CONFIG, $sformatf("Memshire Perfmon %0s",s )) end
      if (u1_umctl2_perf_hif_hi_pri_rd_vmc) begin s = "u1_umctl2_perf_hif_hi_pri_rd_vmc" ; `evl_log_no_name(UVM_MEDIUM, VMOD_CONFIG, $sformatf("Memshire Perfmon %0s",s )) end
      if (u1_umctl2_perf_hif_rd_or_wr_vmc) begin s = "u1_umctl2_perf_hif_rd_or_wr_vmc" ; `evl_log_no_name(UVM_MEDIUM, VMOD_CONFIG, $sformatf("Memshire Perfmon %0s",s )) end
      if (u1_umctl2_perf_hif_rd_vmc) begin s = "u1_umctl2_perf_hif_rd_vmc" ; `evl_log_no_name(UVM_MEDIUM, VMOD_CONFIG, $sformatf("Memshire Perfmon %0s",s )) end
      if (u1_umctl2_perf_hif_rmw_vmc) begin s = "u1_umctl2_perf_hif_rmw_vmc" ; `evl_log_no_name(UVM_MEDIUM, VMOD_CONFIG, $sformatf("Memshire Perfmon %0s",s )) end
      if (u1_umctl2_perf_hif_wr_vmc) begin s = "u1_umctl2_perf_hif_wr_vmc" ; `evl_log_no_name(UVM_MEDIUM, VMOD_CONFIG, $sformatf("Memshire Perfmon %0s",s )) end
      if (u1_umctl2_perf_hpr_req_with_nocredit_vmc) begin s = "u1_umctl2_perf_hpr_req_with_nocredit_vmc" ; `evl_log_no_name(UVM_MEDIUM, VMOD_CONFIG, $sformatf("Memshire Perfmon %0s",s )) end
      if (u1_umctl2_perf_hpr_xact_when_critical_vmc) begin s = "u1_umctl2_perf_hpr_xact_when_critical_vmc" ; `evl_log_no_name(UVM_MEDIUM, VMOD_CONFIG, $sformatf("Memshire Perfmon %0s",s )) end
      if (u1_umctl2_perf_ie_blk_hazard_vmc) begin s = "u1_umctl2_perf_ie_blk_hazard_vmc" ; `evl_log_no_name(UVM_MEDIUM, VMOD_CONFIG, $sformatf("Memshire Perfmon %0s",s )) end
      if (u1_umctl2_perf_lpr_req_with_nocredit_vmc) begin s = "u1_umctl2_perf_lpr_req_with_nocredit_vmc" ; `evl_log_no_name(UVM_MEDIUM, VMOD_CONFIG, $sformatf("Memshire Perfmon %0s",s )) end
      if (u1_umctl2_perf_lpr_xact_when_critical_vmc) begin s = "u1_umctl2_perf_lpr_xact_when_critical_vmc" ; `evl_log_no_name(UVM_MEDIUM, VMOD_CONFIG, $sformatf("Memshire Perfmon %0s",s )) end
      if (u1_umctl2_perf_op_is_activate_vmc) begin s = "u1_umctl2_perf_op_is_activate_vmc" ; `evl_log_no_name(UVM_MEDIUM, VMOD_CONFIG, $sformatf("Memshire Perfmon %0s",s )) end
      if (u1_umctl2_perf_op_is_crit_ref_vmc) begin s = "u1_umctl2_perf_op_is_crit_ref_vmc" ; `evl_log_no_name(UVM_MEDIUM, VMOD_CONFIG, $sformatf("Memshire Perfmon %0s",s )) end
      if (u1_umctl2_perf_op_is_enter_deeppowerdown_vmc) begin s = "u1_umctl2_perf_op_is_enter_deeppowerdown_vmc" ; `evl_log_no_name(UVM_MEDIUM, VMOD_CONFIG, $sformatf("Memshire Perfmon %0s",s )) end
      if (u1_umctl2_perf_op_is_enter_powerdown_vmc) begin s = "u1_umctl2_perf_op_is_enter_powerdown_vmc" ; `evl_log_no_name(UVM_MEDIUM, VMOD_CONFIG, $sformatf("Memshire Perfmon %0s",s )) end
      if (u1_umctl2_perf_op_is_enter_selfref_vmc) begin s = "u1_umctl2_perf_op_is_enter_selfref_vmc" ; `evl_log_no_name(UVM_MEDIUM, VMOD_CONFIG, $sformatf("Memshire Perfmon %0s",s )) end
      if (u1_umctl2_perf_op_is_load_mode_vmc) begin s = "u1_umctl2_perf_op_is_load_mode_vmc" ; `evl_log_no_name(UVM_MEDIUM, VMOD_CONFIG, $sformatf("Memshire Perfmon %0s",s )) end
      // if (u1_umctl2_perf_op_is_mwr_vmc) begin s = "u1_umctl2_perf_op_is_mwr_vmc" ; `evl_log_no_name(UVM_MEDIUM, VMOD_CONFIG, $sformatf("Memshire Perfmon %0s %0x",s, u1_umctl2_perf_bank_vmc )) end
      // if (u1_umctl2_perf_op_is_rd_vmc) begin s = "u1_umctl2_perf_op_is_rd_vmc" ; `evl_log_no_name(UVM_MEDIUM, VMOD_CONFIG, $sformatf("Memshire Perfmon %0s %0x",s, u1_umctl2_perf_bank_vmc )) end
      // if (u1_umctl2_perf_op_is_wr_vmc) begin s = "u1_umctl2_perf_op_is_wr_vmc" ; `evl_log_no_name(UVM_MEDIUM, VMOD_CONFIG, $sformatf("Memshire Perfmon %0s %0x",s, u1_umctl2_perf_bank_vmc )) end
      if (u1_umctl2_perf_op_is_mwr_p1) begin s = "u1_umctl2_perf_op_is_mwr_p1" ; `evl_log_no_name(UVM_MEDIUM, VMOD_CONFIG, $sformatf("Memshire Perfmon %0s %0x %0x",s, u1_umctl2_perf_bank_p1,u1_umctl2_dbg_dfi_ie_cmd_type_vmc )) end
      if (u1_umctl2_perf_op_is_rd_p1) begin s = "u1_umctl2_perf_op_is_rd_p1" ; `evl_log_no_name(UVM_MEDIUM, VMOD_CONFIG, $sformatf("Memshire Perfmon %0s %0x %0x",s, u1_umctl2_perf_bank_p1,u1_umctl2_dbg_dfi_ie_cmd_type_vmc )) end
      if (u1_umctl2_perf_op_is_wr_p1) begin s = "u1_umctl2_perf_op_is_wr_p1" ; `evl_log_no_name(UVM_MEDIUM, VMOD_CONFIG, $sformatf("Memshire Perfmon %0s %0x %0x",s, u1_umctl2_perf_bank_p1,u1_umctl2_dbg_dfi_ie_cmd_type_vmc )) end
      if (u1_umctl2_perf_op_is_precharge_vmc) begin s = "u1_umctl2_perf_op_is_precharge_vmc" ; `evl_log_no_name(UVM_MEDIUM, VMOD_CONFIG, $sformatf("Memshire Perfmon %0s",s )) end
      if (u1_umctl2_perf_op_is_rd_activate_vmc) begin s = "u1_umctl2_perf_op_is_rd_activate_vmc" ; `evl_log_no_name(UVM_MEDIUM, VMOD_CONFIG, $sformatf("Memshire Perfmon %0s",s )) end
      if (u1_umctl2_perf_op_is_refresh_vmc) begin s = "u1_umctl2_perf_op_is_refresh_vmc" ; `evl_log_no_name(UVM_MEDIUM, VMOD_CONFIG, $sformatf("Memshire Perfmon %0s",s )) end
      if (u1_umctl2_perf_op_is_spec_ref_vmc) begin s = "u1_umctl2_perf_op_is_spec_ref_vmc" ; `evl_log_no_name(UVM_MEDIUM, VMOD_CONFIG, $sformatf("Memshire Perfmon %0s",s )) end
      if (u1_umctl2_perf_op_is_zqcl_vmc) begin s = "u1_umctl2_perf_op_is_zqcl_vmc" ; `evl_log_no_name(UVM_MEDIUM, VMOD_CONFIG, $sformatf("Memshire Perfmon %0s",s )) end
      if (u1_umctl2_perf_op_is_zqcs_vmc) begin s = "u1_umctl2_perf_op_is_zqcs_vmc" ; `evl_log_no_name(UVM_MEDIUM, VMOD_CONFIG, $sformatf("Memshire Perfmon %0s",s )) end
      if (u1_umctl2_perf_precharge_for_other_vmc) begin s = "u1_umctl2_perf_precharge_for_other_vmc" ; `evl_log_no_name(UVM_MEDIUM, VMOD_CONFIG, $sformatf("Memshire Perfmon %0s",s )) end
      if (u1_umctl2_perf_precharge_for_rdwr_vmc) begin s = "u1_umctl2_perf_precharge_for_rdwr_vmc" ; `evl_log_no_name(UVM_MEDIUM, VMOD_CONFIG, $sformatf("Memshire Perfmon %0s",s )) end
      if (u1_umctl2_perf_raw_hazard_vmc) begin s = "u1_umctl2_perf_raw_hazard_vmc" ; `evl_log_no_name(UVM_MEDIUM, VMOD_CONFIG, $sformatf("Memshire Perfmon %0s",s )) end
      if (u1_umctl2_perf_rdwr_transitions_vmc) begin s = "u1_umctl2_perf_rdwr_transitions_vmc" ; `evl_log_no_name(UVM_MEDIUM, VMOD_CONFIG, $sformatf("Memshire Perfmon %0s",s )) end
      if (u1_umctl2_perf_selfref_mode_vmc) begin s = "u1_umctl2_perf_selfref_mode_vmc" ; `evl_log_no_name(UVM_MEDIUM, VMOD_CONFIG, $sformatf("Memshire Perfmon %0s",s )) end
      if (u1_umctl2_perf_war_hazard_vmc) begin s = "u1_umctl2_perf_war_hazard_vmc" ; `evl_log_no_name(UVM_MEDIUM, VMOD_CONFIG, $sformatf("Memshire Perfmon %0s",s )) end
      if (u1_umctl2_perf_waw_hazard_vmc) begin s = "u1_umctl2_perf_waw_hazard_vmc" ; `evl_log_no_name(UVM_MEDIUM, VMOD_CONFIG, $sformatf("Memshire Perfmon %0s",s )) end
      if (u1_umctl2_perf_write_combine_vmc) begin s = "u1_umctl2_perf_write_combine_vmc" ; `evl_log_no_name(UVM_MEDIUM, VMOD_CONFIG, $sformatf("Memshire Perfmon %0s",s )) end
      if (u1_umctl2_perf_wr_xact_when_critical_vmc) begin s = "u1_umctl2_perf_wr_xact_when_critical_vmc" ; `evl_log_no_name(UVM_MEDIUM, VMOD_CONFIG, $sformatf("Memshire Perfmon %0s",s )) end
      if (u1_umctl2_raq_pop_0_vmc) begin s = "u1_umctl2_raq_pop_0_vmc" ; `evl_log_no_name(UVM_MEDIUM, VMOD_CONFIG, $sformatf("Memshire Perfmon %0s",s )) end
      if (u1_umctl2_raq_pop_1_vmc) begin s = "u1_umctl2_raq_pop_1_vmc" ; `evl_log_no_name(UVM_MEDIUM, VMOD_CONFIG, $sformatf("Memshire Perfmon %0s",s )) end
      if (u1_umctl2_raq_push_0_vmc) begin s = "u1_umctl2_raq_push_0_vmc" ; `evl_log_no_name(UVM_MEDIUM, VMOD_CONFIG, $sformatf("Memshire Perfmon %0s",s )) end
      if (u1_umctl2_raq_push_1_vmc) begin s = "u1_umctl2_raq_push_1_vmc" ; `evl_log_no_name(UVM_MEDIUM, VMOD_CONFIG, $sformatf("Memshire Perfmon %0s",s )) end
      if (u1_umctl2_sbr_done_intr_vmc) begin s = "u1_umctl2_sbr_done_intr_vmc" ; `evl_log_no_name(UVM_MEDIUM, VMOD_CONFIG, $sformatf("Memshire Perfmon %0s",s )) end
      if (u1_umctl2_waq_pop_0_vmc) begin s = "u1_umctl2_waq_pop_0_vmc" ; `evl_log_no_name(UVM_MEDIUM, VMOD_CONFIG, $sformatf("Memshire Perfmon %0s",s )) end
      if (u1_umctl2_waq_pop_1_vmc) begin s = "u1_umctl2_waq_pop_1_vmc" ; `evl_log_no_name(UVM_MEDIUM, VMOD_CONFIG, $sformatf("Memshire Perfmon %0s",s )) end
      if (u1_umctl2_waq_push_0_vmc) begin s = "u1_umctl2_waq_push_0_vmc" ; `evl_log_no_name(UVM_MEDIUM, VMOD_CONFIG, $sformatf("Memshire Perfmon %0s",s )) end
      if (u1_umctl2_waq_push_1_vmc) begin s = "u1_umctl2_waq_push_1_vmc" ; `evl_log_no_name(UVM_MEDIUM, VMOD_CONFIG, $sformatf("Memshire Perfmon %0s",s )) end
   end


endmodule

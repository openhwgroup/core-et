//--------------------------------------------------------------------------------------------------
//
// Class: evl_noc_scoreboard
//
//
class evl_noc_scoreboard extends evl_scoreboard_mem;

   `uvm_component_utils(evl_noc_scoreboard)

   typedef evl_req_desc evl_sc_req_desc_q[$];
   typedef evl_paddr_t  evl_paddr_q[$];


   //-----------------------------------------------------------------------------------------------
   //
   // Static Member Variables and Objects
   //
   `evl_param_key(pk_noc_sb_error_intensity)


   //-----------------------------------------------------------------------------------------------
   //
   // Member Variables and Objects
   //

   //evl_soc_rtl_port m_soc_rtl_port;

   uvm_tlm_analysis_fifo #(evl_req_desc) m_l3_mesh_req_fifo[35];
   uvm_tlm_analysis_fifo #(evl_req_desc) m_l3_mesh_rsp_fifo[35];
   uvm_tlm_analysis_fifo #(evl_req_desc) m_mesh_l3_rsp_fifo[35];

   uvm_tlm_analysis_fifo #(evl_req_desc) m_l3b_mesh_req_fifo[35];
   uvm_tlm_analysis_fifo #(evl_req_desc) m_l3b_mesh_rsp_fifo[35];
   uvm_tlm_analysis_fifo #(evl_req_desc) m_mesh_l3b_rsp_fifo[35];

   uvm_tlm_analysis_fifo #(evl_req_desc) m_l3c_mesh_req_fifo[35];
   uvm_tlm_analysis_fifo #(evl_req_desc) m_l3c_mesh_rsp_fifo[35];
   uvm_tlm_analysis_fifo #(evl_req_desc) m_mesh_l3c_rsp_fifo[35];

   uvm_tlm_analysis_fifo #(evl_req_desc) m_l3d_mesh_req_fifo[35];
   uvm_tlm_analysis_fifo #(evl_req_desc) m_l3d_mesh_rsp_fifo[35];
   uvm_tlm_analysis_fifo #(evl_req_desc) m_mesh_l3d_rsp_fifo[35];

   uvm_tlm_analysis_fifo #(evl_req_desc) m_sys_mesh_req_fifo[35];
   uvm_tlm_analysis_fifo #(evl_req_desc) m_sys_mesh_rsp_fifo[35];
   uvm_tlm_analysis_fifo #(evl_req_desc) m_mesh_sbm_rsp_fifo[35];
   uvm_tlm_analysis_fifo #(evl_req_desc) m_uctol3_mesh_req_fifo[35];
   uvm_tlm_analysis_fifo #(evl_req_desc) m_uctol3_mesh_rsp_fifo[35];
   uvm_tlm_analysis_fifo #(evl_req_desc) m_uctosys_mesh_req_fifo[35];
   uvm_tlm_analysis_fifo #(evl_req_desc) m_uctosys_mesh_rsp_fifo[35];

   uvm_analysis_export #(evl_req_desc) m_l3_mesh_req_export[35];
   uvm_analysis_export #(evl_req_desc) m_l3_mesh_rsp_export[35];
   uvm_analysis_export #(evl_req_desc) m_mesh_l3_rsp_export[35];

   uvm_analysis_export #(evl_req_desc) m_l3b_mesh_req_export[35];
   uvm_analysis_export #(evl_req_desc) m_l3b_mesh_rsp_export[35];
   uvm_analysis_export #(evl_req_desc) m_mesh_l3b_rsp_export[35];

   uvm_analysis_export #(evl_req_desc) m_l3c_mesh_req_export[35];
   uvm_analysis_export #(evl_req_desc) m_l3c_mesh_rsp_export[35];
   uvm_analysis_export #(evl_req_desc) m_mesh_l3c_rsp_export[35];

   uvm_analysis_export #(evl_req_desc) m_l3d_mesh_req_export[35];
   uvm_analysis_export #(evl_req_desc) m_l3d_mesh_rsp_export[35];
   uvm_analysis_export #(evl_req_desc) m_mesh_l3d_rsp_export[35];

   uvm_analysis_export #(evl_req_desc) m_sys_mesh_req_export[35];
   uvm_analysis_export #(evl_req_desc) m_sys_mesh_rsp_export[35];
   uvm_analysis_export #(evl_req_desc) m_mesh_sbm_rsp_export[35];
   uvm_analysis_export #(evl_req_desc) m_uctol3_mesh_req_export[35];
   uvm_analysis_export #(evl_req_desc) m_uctol3_mesh_rsp_export[35];
   uvm_analysis_export #(evl_req_desc) m_uctosys_mesh_req_export[35];
   uvm_analysis_export #(evl_req_desc) m_uctosys_mesh_rsp_export[35];

   uvm_tlm_analysis_fifo #(evl_req_desc) m_mctosys_mesh_req_fifo[232:239];
   uvm_tlm_analysis_fifo #(evl_req_desc) m_mctosys_mesh_rsp_fifo[232:239];
   uvm_tlm_analysis_fifo #(evl_req_desc) m_mc_mesh_sbm_rsp_fifo[232:239];
   uvm_tlm_analysis_fifo #(evl_req_desc) m_mesh_mem_rsp_fifo[232:239];

   uvm_analysis_export #(evl_req_desc) m_mctosys_mesh_req_export[232:239];
   uvm_analysis_export #(evl_req_desc) m_mctosys_mesh_rsp_export[232:239];
   uvm_analysis_export #(evl_req_desc) m_mc_mesh_sbm_req_export[232:239];
   uvm_analysis_export #(evl_req_desc) m_mc_mesh_sbm_rsp_export[232:239];
   uvm_analysis_export #(evl_req_desc) m_mesh_mem_rsp_export[232:239];

   uvm_tlm_analysis_fifo #(evl_req_desc) m_io_tol3_mesh_req_fifo[253:254];
   uvm_tlm_analysis_fifo #(evl_req_desc) m_io_tol3_mesh_rsp_fifo[253:254];

   uvm_tlm_analysis_fifo #(evl_req_desc) m_io_tol3b_mesh_req_fifo[253:254];
   uvm_tlm_analysis_fifo #(evl_req_desc) m_io_tol3b_mesh_rsp_fifo[253:254];

   uvm_tlm_analysis_fifo #(evl_req_desc) m_io_tol3c_mesh_req_fifo[253:254];
   uvm_tlm_analysis_fifo #(evl_req_desc) m_io_tol3c_mesh_rsp_fifo[253:254];

   uvm_tlm_analysis_fifo #(evl_req_desc) m_io_tol3d_mesh_req_fifo[253:254];
   uvm_tlm_analysis_fifo #(evl_req_desc) m_io_tol3d_mesh_rsp_fifo[253:254];

   uvm_tlm_analysis_fifo #(evl_req_desc) m_io_pm_req_fifo[253:254];
   uvm_tlm_analysis_fifo #(evl_req_desc) m_io_pm_rsp_fifo[253:254];
   uvm_tlm_analysis_fifo #(evl_req_desc) m_io_ps_rsp_fifo[253:254];
   uvm_tlm_analysis_fifo #(evl_req_desc) m_io_tosys_mesh_req_fifo[253:254];
   uvm_tlm_analysis_fifo #(evl_req_desc) m_io_tosys_mesh_rsp_fifo[253:254];

   uvm_tlm_analysis_fifo #(evl_req_desc) m_io_main_noc_rbm_req_fifo[253:254];
   uvm_tlm_analysis_fifo #(evl_req_desc) m_io_main_noc_rbm_rsp_fifo[253:254];

   uvm_analysis_export #(evl_req_desc) m_io_tol3_mesh_req_export[253:254];
   uvm_analysis_export #(evl_req_desc) m_io_tol3_mesh_rsp_export[253:254];

   uvm_analysis_export #(evl_req_desc) m_io_tol3b_mesh_req_export[253:254];
   uvm_analysis_export #(evl_req_desc) m_io_tol3b_mesh_rsp_export[253:254];

   uvm_analysis_export #(evl_req_desc) m_io_tol3c_mesh_req_export[253:254];
   uvm_analysis_export #(evl_req_desc) m_io_tol3c_mesh_rsp_export[253:254];

   uvm_analysis_export #(evl_req_desc) m_io_tol3d_mesh_req_export[253:254];
   uvm_analysis_export #(evl_req_desc) m_io_tol3d_mesh_rsp_export[253:254];

   uvm_analysis_export #(evl_req_desc) m_io_pm_req_export[253:254];
   uvm_analysis_export #(evl_req_desc) m_io_pm_rsp_export[253:254];
   uvm_analysis_export #(evl_req_desc) m_io_ps_rsp_export[253:254];
   uvm_analysis_export #(evl_req_desc) m_io_tosys_mesh_req_export[253:254];
   uvm_analysis_export #(evl_req_desc) m_io_tosys_mesh_rsp_export[253:254];

   uvm_analysis_export #(evl_req_desc) m_io_main_noc_rbm_req_export[253:254];
   uvm_analysis_export #(evl_req_desc) m_io_main_noc_rbm_rsp_export[253:254];

   // REQUEST Qs
   evl_req_desc  m_active_l3_mesh_req_descs[evl_shire_id_t][evl_shire_id_t][$];
   evl_req_desc  m_active_l3b_mesh_req_descs[evl_shire_id_t][evl_shire_id_t][$];
   evl_req_desc  m_active_l3c_mesh_req_descs[evl_shire_id_t][evl_shire_id_t][$];
   evl_req_desc  m_active_l3d_mesh_req_descs[evl_shire_id_t][evl_shire_id_t][$];
   evl_req_desc  m_active_uctol3_mesh_req_descs[evl_shire_id_t][evl_shire_id_t][$];
   evl_req_desc  m_active_sys_mesh_req_descs[evl_shire_id_t][evl_shire_id_t][$];
   evl_req_desc  m_active_uctosys_mesh_req_descs[evl_shire_id_t][evl_shire_id_t][$];

   evl_req_desc  m_active_mctosys_mesh_req_descs[evl_shire_id_t][evl_shire_id_t][$];

   evl_req_desc  m_active_io_l2tol3_mesh_req_descs[evl_shire_id_t][evl_shire_id_t][$];
   evl_req_desc  m_active_io_pm_mesh_req_descs[evl_shire_id_t][evl_shire_id_t][$];
   evl_req_desc  m_active_io_tosys_mesh_req_descs[evl_shire_id_t][evl_shire_id_t][$];

   evl_req_desc  m_active_ps_tosys_mesh_req_descs[evl_shire_id_t][evl_shire_id_t][$];

   // RESPONSE Qs
   evl_req_desc  m_active_mesh_l3_rsp_descs[evl_shire_id_t][evl_shire_id_t][$];
   evl_req_desc  m_active_mesh_sbm_rsp_descs[evl_shire_id_t][evl_shire_id_t][$];

   evl_req_desc  m_active_mesh_mem_rsp_descs[evl_shire_id_t][evl_shire_id_t][$];
   evl_req_desc  m_active_mc_mesh_sbm_rsp_descs[evl_shire_id_t][evl_shire_id_t][$];

   evl_req_desc  m_active_io_ps_rsp_descs[evl_shire_id_t][evl_shire_id_t][$];

   evl_req_desc  m_active_ps_ps_rsp_descs[evl_shire_id_t][evl_shire_id_t][$];

   evl_req_desc  m_active_l3_mesh_rsp_descs[evl_shire_id_t][evl_shire_id_t][$];
   evl_req_desc  m_active_l3b_mesh_rsp_descs[evl_shire_id_t][evl_shire_id_t][$];
   evl_req_desc  m_active_l3c_mesh_rsp_descs[evl_shire_id_t][evl_shire_id_t][$];
   evl_req_desc  m_active_l3d_mesh_rsp_descs[evl_shire_id_t][evl_shire_id_t][$];
   evl_req_desc  m_active_uctol3_mesh_rsp_descs[evl_shire_id_t][evl_shire_id_t][$];
   evl_req_desc  m_active_sys_mesh_rsp_descs[evl_shire_id_t][evl_shire_id_t][$];
   evl_req_desc  m_active_uctosys_mesh_rsp_descs[evl_shire_id_t][evl_shire_id_t][$];

   evl_req_desc  m_active_mctosys_mesh_rsp_descs[evl_shire_id_t][evl_shire_id_t][$];

   evl_req_desc  m_active_io_l2tol3_mesh_rsp_descs[evl_shire_id_t][evl_shire_id_t][$];
   evl_req_desc  m_active_io_pm_mesh_rsp_descs[evl_shire_id_t][evl_shire_id_t][$];
   evl_req_desc  m_active_io_tosys_mesh_rsp_descs[evl_shire_id_t][evl_shire_id_t][$];

   evl_req_desc  m_active_ps_tosys_mesh_rsp_descs[evl_shire_id_t][evl_shire_id_t][$];

   `ifdef EVL_COVERAGE_ON
      evl_noc_sb_cvg    m_noc_cov;
   `endif

   int memshire_master_ids [7:0]       = '{`MEMSHIRE_MASTER_IDS};
   int tol3_sib_master_ids [33:0]      = '{`TOL3_SIB_MASTER_IDS};
   int tosys_sib_master_ids [33:0]     = '{`TOSYS_SIB_MASTER_IDS};
   int l2tol3_master_ids [3:0] [33:0]  = '{'{`L2TOL3D_MASTER_IDS}, '{`L2TOL3C_MASTER_IDS}, '{`L2TOL3B_MASTER_IDS}, '{`TOL3_SIB_MASTER_IDS}};

   int m_min2maxscp_err;

   bit m_shires_in_build[evl_shire_id_t] = '{default:0};

   //int m_current_shire_ids [33:0];

   `include "get_shire_id_functions.vh"
   `include "evl_noc_regmap.svh"
   //------------------------------------------------------------------------------------------------
   //
   // build_phase
   //
   virtual function void build_phase(uvm_phase phase);
      super.build_phase(phase);

      `ifdef EVL_COVERAGE_ON
         `evl_log(UVM_HIGH, VMOD_NOC_SB, get_abstract_name(), $sformatf("Building NoC Coverage Object..."))
         m_noc_cov = new();
         `evl_log(UVM_HIGH, VMOD_NOC_SB, get_abstract_name(), $sformatf("Done Building NoC Coverage Object..."))
      `endif

      foreach (m_l3_mesh_req_fifo[ii]) begin
         m_l3_mesh_req_fifo[ii]        = new($sformatf("m_l3_mesh_req_fifo_%0d", ii), this);
         m_l3_mesh_rsp_fifo[ii]        = new($sformatf("m_l3_mesh_rsp_fifo_%0d", ii), this);
         m_mesh_l3_rsp_fifo[ii]        = new($sformatf("m_mesh_l3_rsp_fifo_%0d", ii), this);
         m_l3b_mesh_req_fifo[ii]       = new($sformatf("m_l3b_mesh_req_fifo_%0d", ii), this);
         m_l3b_mesh_rsp_fifo[ii]       = new($sformatf("m_l3b_mesh_rsp_fifo_%0d", ii), this);
         m_mesh_l3b_rsp_fifo[ii]       = new($sformatf("m_mesh_l3b_rsp_fifo_%0d", ii), this);
         m_l3c_mesh_req_fifo[ii]       = new($sformatf("m_l3c_mesh_req_fifo_%0d", ii), this);
         m_l3c_mesh_rsp_fifo[ii]       = new($sformatf("m_l3c_mesh_rsp_fifo_%0d", ii), this);
         m_mesh_l3c_rsp_fifo[ii]       = new($sformatf("m_mesh_l3c_rsp_fifo_%0d", ii), this);
         m_l3d_mesh_req_fifo[ii]       = new($sformatf("m_l3d_mesh_req_fifo_%0d", ii), this);
         m_l3d_mesh_rsp_fifo[ii]       = new($sformatf("m_l3d_mesh_rsp_fifo_%0d", ii), this);
         m_mesh_l3d_rsp_fifo[ii]       = new($sformatf("m_mesh_l3d_rsp_fifo_%0d", ii), this);
         m_sys_mesh_req_fifo[ii]       = new($sformatf("m_sys_mesh_req_fifo_%0d", ii), this);
         m_sys_mesh_rsp_fifo[ii]       = new($sformatf("m_sys_mesh_rsp_fifo_%0d", ii), this);
         m_mesh_sbm_rsp_fifo[ii]       = new($sformatf("m_mesh_sbm_rsp_fifo_%0d", ii), this);
         m_uctol3_mesh_req_fifo[ii]    = new($sformatf("m_uctol3_mesh_req_fifo_%0d", ii), this);
         m_uctol3_mesh_rsp_fifo[ii]    = new($sformatf("m_uctol3_mesh_rsp_fifo_%0d", ii), this);
         m_uctosys_mesh_req_fifo[ii]   = new($sformatf("m_uctosys_mesh_req_fifo_%0d", ii), this);
         m_uctosys_mesh_rsp_fifo[ii]   = new($sformatf("m_uctosys_mesh_rsp_fifo_%0d", ii), this);
         m_l3_mesh_req_export[ii]      = new($sformatf("m_l3_mesh_req_export_%0d", ii), this);
         m_l3_mesh_rsp_export[ii]      = new($sformatf("m_l3_mesh_rsp_export_%0d", ii), this);
         m_mesh_l3_rsp_export[ii]      = new($sformatf("m_mesh_l3_rsp_export_%0d", ii), this);
         m_l3b_mesh_req_export[ii]     = new($sformatf("m_l3b_mesh_req_export_%0d", ii), this);
         m_l3b_mesh_rsp_export[ii]     = new($sformatf("m_l3b_mesh_rsp_export_%0d", ii), this);
         m_mesh_l3b_rsp_export[ii]     = new($sformatf("m_mesh_l3b_rsp_export_%0d", ii), this);
         m_l3c_mesh_req_export[ii]     = new($sformatf("m_l3c_mesh_req_export_%0d", ii), this);
         m_l3c_mesh_rsp_export[ii]     = new($sformatf("m_l3c_mesh_rsp_export_%0d", ii), this);
         m_mesh_l3c_rsp_export[ii]     = new($sformatf("m_mesh_l3c_rsp_export_%0d", ii), this);
         m_l3d_mesh_req_export[ii]     = new($sformatf("m_l3d_mesh_req_export_%0d", ii), this);
         m_l3d_mesh_rsp_export[ii]     = new($sformatf("m_l3d_mesh_rsp_export_%0d", ii), this);
         m_mesh_l3d_rsp_export[ii]     = new($sformatf("m_mesh_l3d_rsp_export_%0d", ii), this);
         m_sys_mesh_req_export[ii]     = new($sformatf("m_sys_mesh_req_export_%0d", ii), this);
         m_sys_mesh_rsp_export[ii]     = new($sformatf("m_sys_mesh_rsp_export_%0d", ii), this);
         m_mesh_sbm_rsp_export[ii]     = new($sformatf("m_mesh_sbm_rsp_export_%0d", ii), this);
         m_uctol3_mesh_req_export[ii]  = new($sformatf("m_uctol3_mesh_req_export_%0d", ii), this);
         m_uctol3_mesh_rsp_export[ii]  = new($sformatf("m_uctol3_mesh_rsp_export_%0d", ii), this);
         m_uctosys_mesh_req_export[ii] = new($sformatf("m_uctosys_mesh_req_export_%0d", ii), this);
         m_uctosys_mesh_rsp_export[ii] = new($sformatf("m_uctosys_mesh_rsp_export_%0d", ii), this);
      end

      foreach (m_mctosys_mesh_req_fifo[ii]) begin
         m_mctosys_mesh_req_fifo[ii]   = new($sformatf("m_mctosys_mesh_req_fifo_%0d", ii), this);
         m_mctosys_mesh_rsp_fifo[ii]   = new($sformatf("m_mctosys_mesh_rsp_fifo_%0d", ii), this);
         m_mc_mesh_sbm_rsp_fifo[ii]    = new($sformatf("m_mc_mesh_sbm_rsp_fifo_%0d", ii), this);
         m_mesh_mem_rsp_fifo[ii]       = new($sformatf("m_mesh_mem_rsp_fifo_%0d", ii), this);
         m_mctosys_mesh_req_export[ii] = new($sformatf("m_mctosys_mesh_req_export_%0d", ii), this);
         m_mctosys_mesh_rsp_export[ii] = new($sformatf("m_mctosys_mesh_rsp_export_%0d", ii), this);
         m_mc_mesh_sbm_rsp_export[ii]  = new($sformatf("m_mc_mesh_sbm_rsp_export_%0d", ii), this);
         m_mesh_mem_rsp_export[ii]     = new($sformatf("m_mesh_mem_rsp_export_%0d", ii), this);
      end

      foreach (m_io_tol3_mesh_req_fifo[ii]) begin
         m_io_tol3_mesh_req_fifo[ii]      = new($sformatf("m_io_tol3_mesh_req_fifo_%0d", ii), this);
         m_io_tol3_mesh_rsp_fifo[ii]      = new($sformatf("m_io_tol3_mesh_rsp_fifo_%0d", ii), this);
         m_io_tol3b_mesh_req_fifo[ii]     = new($sformatf("m_io_tol3b_mesh_req_fifo_%0d", ii), this);
         m_io_tol3b_mesh_rsp_fifo[ii]     = new($sformatf("m_io_tol3b_mesh_rsp_fifo_%0d", ii), this);
         m_io_tol3c_mesh_req_fifo[ii]     = new($sformatf("m_io_tol3c_mesh_req_fifo_%0d", ii), this);
         m_io_tol3c_mesh_rsp_fifo[ii]     = new($sformatf("m_io_tol3c_mesh_rsp_fifo_%0d", ii), this);
         m_io_tol3d_mesh_req_fifo[ii]     = new($sformatf("m_io_tol3d_mesh_req_fifo_%0d", ii), this);
         m_io_tol3d_mesh_rsp_fifo[ii]     = new($sformatf("m_io_tol3d_mesh_rsp_fifo_%0d", ii), this);
         m_io_pm_req_fifo[ii]             = new($sformatf("m_io_pm_req_fifo_%0d", ii), this);
         m_io_pm_rsp_fifo[ii]             = new($sformatf("m_io_pm_rsp_fifo_%0d", ii), this);
         m_io_ps_rsp_fifo[ii]             = new($sformatf("m_io_ps_rsp_fifo_%0d", ii), this);
         m_io_tosys_mesh_req_fifo[ii]     = new($sformatf("m_io_tosys_mesh_req_fifo_%0d", ii), this);
         m_io_tosys_mesh_rsp_fifo[ii]     = new($sformatf("m_io_tosys_mesh_rsp_fifo_%0d", ii), this);
         m_io_main_noc_rbm_req_fifo[ii]   = new($sformatf("m_io_main_noc_rbm_req_fifo_%0d", ii), this);
         m_io_main_noc_rbm_rsp_fifo[ii]   = new($sformatf("m_io_main_noc_rbm_rsp_fifo_%0d", ii), this);
         m_io_tol3_mesh_req_export[ii]    = new($sformatf("m_io_tol3_mesh_req_export_%0d", ii), this);
         m_io_tol3_mesh_rsp_export[ii]    = new($sformatf("m_io_tol3_mesh_rsp_export_%0d", ii), this);
         m_io_tol3b_mesh_req_export[ii]   = new($sformatf("m_io_tol3b_mesh_req_export_%0d", ii), this);
         m_io_tol3b_mesh_rsp_export[ii]   = new($sformatf("m_io_tol3b_mesh_rsp_export_%0d", ii), this);
         m_io_tol3c_mesh_req_export[ii]   = new($sformatf("m_io_tol3c_mesh_req_export_%0d", ii), this);
         m_io_tol3c_mesh_rsp_export[ii]   = new($sformatf("m_io_tol3c_mesh_rsp_export_%0d", ii), this);
         m_io_tol3d_mesh_req_export[ii]   = new($sformatf("m_io_tol3d_mesh_req_export_%0d", ii), this);
         m_io_tol3d_mesh_rsp_export[ii]   = new($sformatf("m_io_tol3d_mesh_rsp_export_%0d", ii), this);
         m_io_pm_req_export[ii]           = new($sformatf("m_io_pm_req_export_%0d", ii), this);
         m_io_pm_rsp_export[ii]           = new($sformatf("m_io_pm_rsp_export_%0d", ii), this);
         m_io_ps_rsp_export[ii]           = new($sformatf("m_io_ps_rsp_export_%0d", ii), this);
         m_io_tosys_mesh_req_export[ii]   = new($sformatf("m_io_tosys_mesh_req_export_%0d", ii), this);
         m_io_tosys_mesh_rsp_export[ii]   = new($sformatf("m_io_tosys_mesh_rsp_export_%0d", ii), this);
         m_io_main_noc_rbm_req_export[ii] = new($sformatf("m_io_main_noc_rbm_req_export_%0d", ii), this);
         m_io_main_noc_rbm_rsp_export[ii] = new($sformatf("m_io_main_noc_rbm_rsp_export_%0d", ii), this);
      end

      //foreach (m_current_shire_ids[ii]) begin
      //   m_current_shire_ids[ii] = ii;
      //end

      initialize_rbm_addr();

   endfunction : build_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Function: connect_phase
   //
   virtual function void connect_phase(uvm_phase phase);
      super.connect_phase(phase);
      foreach(m_l3_mesh_req_export[ii]) begin
         m_l3_mesh_req_export[ii].connect(m_l3_mesh_req_fifo[ii].analysis_export);
         m_l3_mesh_rsp_export[ii].connect(m_l3_mesh_rsp_fifo[ii].analysis_export);
         m_mesh_l3_rsp_export[ii].connect(m_mesh_l3_rsp_fifo[ii].analysis_export);
         m_l3b_mesh_req_export[ii].connect(m_l3b_mesh_req_fifo[ii].analysis_export);
         m_l3b_mesh_rsp_export[ii].connect(m_l3b_mesh_rsp_fifo[ii].analysis_export);
         m_mesh_l3b_rsp_export[ii].connect(m_mesh_l3b_rsp_fifo[ii].analysis_export);
         m_l3c_mesh_req_export[ii].connect(m_l3c_mesh_req_fifo[ii].analysis_export);
         m_l3c_mesh_rsp_export[ii].connect(m_l3c_mesh_rsp_fifo[ii].analysis_export);
         m_mesh_l3c_rsp_export[ii].connect(m_mesh_l3c_rsp_fifo[ii].analysis_export);
         m_l3d_mesh_req_export[ii].connect(m_l3d_mesh_req_fifo[ii].analysis_export);
         m_l3d_mesh_rsp_export[ii].connect(m_l3d_mesh_rsp_fifo[ii].analysis_export);
         m_mesh_l3d_rsp_export[ii].connect(m_mesh_l3d_rsp_fifo[ii].analysis_export);
         m_sys_mesh_req_export[ii].connect(m_sys_mesh_req_fifo[ii].analysis_export);
         m_sys_mesh_rsp_export[ii].connect(m_sys_mesh_rsp_fifo[ii].analysis_export);
         m_mesh_sbm_rsp_export[ii].connect(m_mesh_sbm_rsp_fifo[ii].analysis_export);
         m_uctol3_mesh_req_export[ii].connect(m_uctol3_mesh_req_fifo[ii].analysis_export);
         m_uctol3_mesh_rsp_export[ii].connect(m_uctol3_mesh_rsp_fifo[ii].analysis_export);
         m_uctosys_mesh_req_export[ii].connect(m_uctosys_mesh_req_fifo[ii].analysis_export);
         m_uctosys_mesh_rsp_export[ii].connect(m_uctosys_mesh_rsp_fifo[ii].analysis_export);
      end
      foreach(m_mctosys_mesh_req_export[ii]) begin
         m_mctosys_mesh_req_export[ii].connect(m_mctosys_mesh_req_fifo[ii].analysis_export);
         m_mctosys_mesh_rsp_export[ii].connect(m_mctosys_mesh_rsp_fifo[ii].analysis_export);
         m_mc_mesh_sbm_rsp_export[ii].connect(m_mc_mesh_sbm_rsp_fifo[ii].analysis_export);
         m_mesh_mem_rsp_export[ii].connect(m_mesh_mem_rsp_fifo[ii].analysis_export);
      end
      foreach (m_io_tol3_mesh_req_export[ii]) begin
         m_io_tol3_mesh_req_export[ii].connect(m_io_tol3_mesh_req_fifo[ii].analysis_export);
         m_io_tol3_mesh_rsp_export[ii].connect(m_io_tol3_mesh_rsp_fifo[ii].analysis_export);
         m_io_tol3b_mesh_req_export[ii].connect(m_io_tol3b_mesh_req_fifo[ii].analysis_export);
         m_io_tol3b_mesh_rsp_export[ii].connect(m_io_tol3b_mesh_rsp_fifo[ii].analysis_export);
         m_io_tol3c_mesh_req_export[ii].connect(m_io_tol3c_mesh_req_fifo[ii].analysis_export);
         m_io_tol3c_mesh_rsp_export[ii].connect(m_io_tol3c_mesh_rsp_fifo[ii].analysis_export);
         m_io_tol3d_mesh_req_export[ii].connect(m_io_tol3d_mesh_req_fifo[ii].analysis_export);
         m_io_tol3d_mesh_rsp_export[ii].connect(m_io_tol3d_mesh_rsp_fifo[ii].analysis_export);
         m_io_pm_req_export[ii].connect(m_io_pm_req_fifo[ii].analysis_export);
         m_io_pm_rsp_export[ii].connect(m_io_pm_rsp_fifo[ii].analysis_export);
         m_io_ps_rsp_export[ii].connect(m_io_ps_rsp_fifo[ii].analysis_export);
         m_io_tosys_mesh_req_export[ii].connect(m_io_tosys_mesh_req_fifo[ii].analysis_export);
         m_io_tosys_mesh_rsp_export[ii].connect(m_io_tosys_mesh_rsp_fifo[ii].analysis_export);
         m_io_main_noc_rbm_req_export[ii].connect(m_io_main_noc_rbm_req_fifo[ii].analysis_export);
         m_io_main_noc_rbm_rsp_export[ii].connect(m_io_main_noc_rbm_rsp_fifo[ii].analysis_export);
      end
   endfunction : connect_phase


   //----------------------------------------------------------------------------------------------
   //
   // Task: run_phase
   //
   virtual task run_phase(uvm_phase phase);
      fork
         super.run_phase(phase);
      join_none;
   endtask: run_phase

   //-----------------------------------------------------------------------------------------------
   //
   // Task: watch_shire_axi_operations
   //
   virtual task watch_shire_axi_operations(input evl_shire_id_t shire);
      watch_for_sent_requests(m_l3_mesh_req_fifo[shire],      shire, L2TOL3_M);
      watch_for_sent_requests(m_l3b_mesh_req_fifo[shire],     shire, L2TOL3_M, 1);
      watch_for_sent_requests(m_l3c_mesh_req_fifo[shire],     shire, L2TOL3_M, 2);
      watch_for_sent_requests(m_l3d_mesh_req_fifo[shire],     shire, L2TOL3_M, 3);
      watch_for_sent_requests(m_sys_mesh_req_fifo[shire],     shire, L3TOSYS_M);
      watch_for_sent_requests(m_uctol3_mesh_req_fifo[shire],  shire, UCTOL3_M);
      watch_for_sent_requests(m_uctosys_mesh_req_fifo[shire], shire, UCTOSYS_M);

      watch_for_sent_responses(m_mesh_l3_rsp_fifo[shire],  shire, TOL3_S);
      watch_for_sent_responses(m_mesh_l3b_rsp_fifo[shire], shire, TOL3_S);
      watch_for_sent_responses(m_mesh_l3c_rsp_fifo[shire], shire, TOL3_S);
      watch_for_sent_responses(m_mesh_l3d_rsp_fifo[shire], shire, TOL3_S);
      watch_for_sent_responses(m_mesh_sbm_rsp_fifo[shire], shire, TOSYS_S);

      watch_for_received_responses(m_l3_mesh_rsp_fifo[shire],      shire, L2TOL3_M);
      watch_for_received_responses(m_l3b_mesh_rsp_fifo[shire],     shire, L2TOL3_M, 1);
      watch_for_received_responses(m_l3c_mesh_rsp_fifo[shire],     shire, L2TOL3_M, 2);
      watch_for_received_responses(m_l3d_mesh_rsp_fifo[shire],     shire, L2TOL3_M, 3);
      watch_for_received_responses(m_sys_mesh_rsp_fifo[shire],     shire, L3TOSYS_M);
      watch_for_received_responses(m_uctol3_mesh_rsp_fifo[shire],  shire, UCTOL3_M);
      watch_for_received_responses(m_uctosys_mesh_rsp_fifo[shire], shire, UCTOSYS_M);

   endtask : watch_shire_axi_operations

   //-----------------------------------------------------------------------------------------------
   //
   // Task: watch_memshire_axi_operations
   //
   virtual task watch_memshire_axi_operations(input evl_shire_id_t shire);
      watch_for_sent_requests(m_mctosys_mesh_req_fifo[shire], shire, MCTOSYS_M);

      watch_for_sent_responses(m_mesh_mem_rsp_fifo[shire],    shire, TOMEM_S);
      watch_for_sent_responses(m_mc_mesh_sbm_rsp_fifo[shire], shire, MC_SB_S);

      watch_for_received_responses(m_mctosys_mesh_rsp_fifo[shire], shire, MCTOSYS_M);

   endtask : watch_memshire_axi_operations

   //-----------------------------------------------------------------------------------------------
   //
   // Task: watch_ioshire_axi_operations
   //
   virtual task watch_ioshire_axi_operations(input evl_shire_id_t shire);
      watch_for_sent_requests(m_io_tol3_mesh_req_fifo[shire],  shire, IO_TOL3_M);
      watch_for_sent_requests(m_io_tol3b_mesh_req_fifo[shire], shire, IO_TOL3_M, 1);
      watch_for_sent_requests(m_io_tol3c_mesh_req_fifo[shire], shire, IO_TOL3_M, 2);
      watch_for_sent_requests(m_io_tol3d_mesh_req_fifo[shire], shire, IO_TOL3_M, 3);
      watch_for_sent_requests(m_io_pm_req_fifo[shire],         shire, IO_PM);
      watch_for_sent_requests(m_io_tosys_mesh_req_fifo[shire], shire, IO_TOSYS_M);

      watch_for_sent_responses(m_io_ps_rsp_fifo[shire], shire, IO_PS);

      watch_for_received_responses(m_io_pm_rsp_fifo[shire],         shire, IO_PM);
      watch_for_received_responses(m_io_tol3_mesh_rsp_fifo[shire],  shire, IO_TOL3_M);
      watch_for_received_responses(m_io_tol3b_mesh_rsp_fifo[shire], shire, IO_TOL3_M, 1);
      watch_for_received_responses(m_io_tol3c_mesh_rsp_fifo[shire], shire, IO_TOL3_M, 2);
      watch_for_received_responses(m_io_tol3d_mesh_rsp_fifo[shire], shire, IO_TOL3_M, 3);
      watch_for_received_responses(m_io_tosys_mesh_rsp_fifo[shire], shire, IO_TOSYS_M);

      watch_for_rbm_transactions(m_io_main_noc_rbm_req_fifo[shire]);

   endtask : watch_ioshire_axi_operations

   //-----------------------------------------------------------------------------------------------
   //
   // Task: watch_pshire_axi_operations
   //
   virtual task watch_pshire_axi_operations(input evl_shire_id_t shire);
      watch_for_sent_requests(m_io_tol3_mesh_req_fifo[shire],  shire, PSHIRE_TOL3_M);
      watch_for_sent_requests(m_io_tol3b_mesh_req_fifo[shire], shire, PSHIRE_TOL3_M, 1);
      watch_for_sent_requests(m_io_tol3c_mesh_req_fifo[shire], shire, PSHIRE_TOL3_M, 2);
      watch_for_sent_requests(m_io_tol3d_mesh_req_fifo[shire], shire, PSHIRE_TOL3_M, 3);
      watch_for_sent_requests(m_io_tosys_mesh_req_fifo[shire], shire, PSHIRE_TOSYS_M);

      watch_for_sent_responses(m_io_ps_rsp_fifo[shire], shire, PSHIRE_PS);

      watch_for_received_responses(m_io_tol3_mesh_rsp_fifo[shire],  shire, PSHIRE_TOL3_M);
      watch_for_received_responses(m_io_tol3b_mesh_rsp_fifo[shire], shire, PSHIRE_TOL3_M, 1);
      watch_for_received_responses(m_io_tol3c_mesh_rsp_fifo[shire], shire, PSHIRE_TOL3_M, 2);
      watch_for_received_responses(m_io_tol3d_mesh_rsp_fifo[shire], shire, PSHIRE_TOL3_M, 3);
      watch_for_received_responses(m_io_tosys_mesh_rsp_fifo[shire], shire, PSHIRE_TOSYS_M);

   endtask : watch_pshire_axi_operations

   //-----------------------------------------------------------------------------------------------
   //
   // Task: watch_for_sent_requests
   //
   task watch_for_sent_requests(input uvm_tlm_analysis_fifo #(evl_req_desc) req_fifo, input evl_shire_id_t rst_shire_id, input evl_noc_master_type_t master_type, input int noc_layer = 0);
      evl_req_desc         rcv_req_desc;
      evl_req_desc         tgt_req_desc;
      evl_shire_id_t       target_shire;
      evl_shire_id_t       shire;
      evl_bus_req_id_t     tgt_req_id;
      evl_paddr_t          rcv_req_addr;
      evl_noc_slave_type_t tgt_slave_type;
      evl_bus_cache_attr_t rcv_req_cache_attr;

      if (req_fifo == null) begin
         return;
      end
      fork
         forever begin
            req_fifo.get(rcv_req_desc);
            if (rcv_req_desc != null) begin
               if (rst_shire_id <= 33) begin
                  `evl_log_mdesc(UVM_DEBUG, VMOD_NOC_SB, get_abstract_name(), rcv_req_desc, $sformatf("S%0d: rst_shire_id: %0d getting current shire_id...", rst_shire_id, rst_shire_id))
                  //shire = m_current_shire_ids[rst_shire_id];
                  shire = m_global_attrs.get_current_shire_id(rst_shire_id);
                  `evl_log_mdesc(UVM_DEBUG, VMOD_NOC_SB, get_abstract_name(), rcv_req_desc, $sformatf("S%0d: rst_shire_id: %0d, current shire_id: %0d...", rst_shire_id, rst_shire_id, shire))
               end
               else begin
                  `evl_log_mdesc(UVM_DEBUG, VMOD_NOC_SB, get_abstract_name(), rcv_req_desc, $sformatf("S%0d: rst_shire_id: %0d getting current shire_id...", rst_shire_id, rst_shire_id))
                  shire = rst_shire_id;
                  `evl_log_mdesc(UVM_DEBUG, VMOD_NOC_SB, get_abstract_name(), rcv_req_desc, $sformatf("S%0d: rst_shire_id: %0d, current shire_id: %0d...", rst_shire_id, rst_shire_id, shire))
               end
               rcv_req_addr = rcv_req_desc.get_paddr();
               target_shire = get_shire_id_from_addr(rcv_req_addr, master_type, tgt_slave_type, shire);
               `ifdef EVL_COVERAGE_ON
                  m_noc_cov.sample_master(shire, target_shire, rcv_req_addr, master_type, noc_layer);
               `endif
               if (target_shire == ~evl_shire_id_t'(0)) begin
                  `dut_warning(get_abstract_name(), $sformatf("0x%0x is illegal address for S%0d %p (%0s)", rcv_req_addr, shire, master_type, rcv_req_desc.sprint_obj()))
               end
               else if (m_shires_in_build[target_shire] == 1'b0) begin
                  `dut_warning(get_abstract_name(), $sformatf("Target shire S%0d is not connected to the NoC Scoreboard. Ignoring this transaction. (%0s)", target_shire, rcv_req_desc.sprint_obj()))
               end
               else if ((master_type == IO_PM) && (rcv_req_desc.bus_cmd_is_write() == 1'b1) && (tgt_slave_type == TOL3_S)) begin
                  `dut_warning(get_abstract_name(), $sformatf("S%0d %p can only read from S%0d %p (%0s)", shire, master_type, target_shire, tgt_slave_type, rcv_req_desc.sprint_obj()))
               end
               else if ((master_type == PSHIRE_TOSYS_M) && (rcv_req_desc.bus_cmd_is_write() == 1'b1) && (tgt_slave_type == TOL3_S)) begin
                  `dut_warning(get_abstract_name(), $sformatf("S%0d %p can only read from S%0d %p (%0s)", shire, master_type, target_shire, tgt_slave_type, rcv_req_desc.sprint_obj()))
               end
               else if ((master_type == MCTOSYS_M) && (rcv_req_desc.bus_cmd_is_write() == 1'b0) && (tgt_slave_type != IO_PS)) begin
                  `dut_warning(get_abstract_name(), $sformatf("S%0d %p is write only (%0s)", shire, master_type, rcv_req_desc.sprint_obj()))
               end
               else if ((master_type == IO_TOSYS_M) && (rcv_req_desc.bus_cmd_is_write() == 1'b0) && (tgt_slave_type == TOSYS_S)) begin
                  `dut_warning(get_abstract_name(), $sformatf("S%0d %p can only write to S%0d %p (%0s)", shire, master_type, target_shire, tgt_slave_type, rcv_req_desc.sprint_obj()))
               end
               else begin
                  `evl_log_mdesc(UVM_DEBUG, VMOD_NOC_SB, get_abstract_name(), rcv_req_desc, $sformatf("S%0d: %p AXI REQ, %0s sent to S%0d %p...", shire, master_type, rcv_req_desc.sprint_obj(), target_shire, tgt_slave_type))
                  rcv_req_desc.set_observed();
                  $cast(tgt_req_desc, rcv_req_desc.clone());
                  tgt_req_id = predict_req_id(rst_shire_id, rcv_req_desc.get_bus_req_id(), master_type, noc_layer);
                  tgt_req_desc.set_bus_req_id(tgt_req_id);
                  rcv_req_cache_attr = rcv_req_desc.get_bus_cache_attr();
                  // The NoC changes the AxSIZE field of txn to match the destination bus width.
                  // Need to predict that here.
                  if (rcv_req_cache_attr[1] == 1'b1) begin
                     case (tgt_slave_type)
                        TOL3_S, TOMEM_S, IO_PS, PSHIRE_PS : tgt_req_desc.set_bus_req_size($clog2((`SC_MESH_MASTER_AXI_DATA_SIZE/8)));
                        TOSYS_S                           : tgt_req_desc.set_bus_req_size($clog2((`SYS_MESH_SLAVE_AXI_DATA_SIZE/8)));
                        MC_SB_S                           : tgt_req_desc.set_bus_req_size($clog2((`MEM_SBM_MESH_SLAVE_AXI_DATA_SIZE/8)));
                     endcase
                  end
                  rcv_req_desc.add_child_desc(tgt_req_desc);
                  if (rcv_req_desc.bus_cmd_is_write() == 1'b1) begin
                     void'(rcv_req_desc.cascade_req_data_to_dst(tgt_req_desc));
                  end
                  `evl_log_mdesc(UVM_HIGH, VMOD_NOC_SB, get_abstract_name(), tgt_req_desc, $sformatf("S%0d: Putting %0s in S%0d queue... (S%0d %p 0x%0x 0x%0x)", shire, tgt_req_desc.sprint_obj(), target_shire, shire, master_type, tgt_req_id, rcv_req_addr))
                  case (master_type)
                     L2TOL3_M, IO_TOL3_M, PSHIRE_TOL3_M  : begin
                                                            if (noc_layer == 3) begin
                                                               m_active_l3d_mesh_req_descs[shire][target_shire].push_back(tgt_req_desc);
                                                            end
                                                            else if (noc_layer == 2) begin
                                                               m_active_l3c_mesh_req_descs[shire][target_shire].push_back(tgt_req_desc);
                                                            end
                                                            else if (noc_layer == 1) begin
                                                               m_active_l3b_mesh_req_descs[shire][target_shire].push_back(tgt_req_desc);
                                                            end
                                                            else begin
                                                               m_active_l3_mesh_req_descs[shire][target_shire].push_back(tgt_req_desc);
                                                            end
                                                           end
                     L3TOSYS_M                           : m_active_sys_mesh_req_descs[shire][target_shire].push_back(tgt_req_desc);
                     UCTOL3_M                            : m_active_uctol3_mesh_req_descs[shire][target_shire].push_back(tgt_req_desc);
                     UCTOSYS_M                           : m_active_uctosys_mesh_req_descs[shire][target_shire].push_back(tgt_req_desc);
                     MCTOSYS_M                           : m_active_mctosys_mesh_req_descs[shire][target_shire].push_back(tgt_req_desc);
                     IO_PM                               : m_active_io_pm_mesh_req_descs[shire][target_shire].push_back(tgt_req_desc);
                     IO_TOSYS_M                          : m_active_io_tosys_mesh_req_descs[shire][target_shire].push_back(tgt_req_desc);
                     PSHIRE_TOSYS_M                      : m_active_ps_tosys_mesh_req_descs[shire][target_shire].push_back(tgt_req_desc);
                  endcase
               end
            end
            else begin
               $display("S%0d %p has received a null req_desc. Not great!!!", shire, master_type);
            end
         end
      join_none

   endtask : watch_for_sent_requests


   //-----------------------------------------------------------------------------------------------
   //
   // Task: watch_for_sent_responses
   //
   task watch_for_sent_responses(input uvm_tlm_analysis_fifo #(evl_req_desc) req_fifo, input evl_shire_id_t rst_shire_id, input evl_noc_slave_type_t slave_type);
      evl_req_desc            rcv_req_desc;
      evl_req_desc            src_req_desc;
      evl_shire_id_t          src_shire;
      evl_shire_id_t          rst_src_shire;
      evl_shire_id_t          rcv_req_src_shire;
      evl_shire_id_t          shire;
      evl_noc_master_type_t   src_master;
      int                     src_noc_layer;

      if (req_fifo == null) begin
         return;
      end
      fork
         forever begin
            req_fifo.get(rcv_req_desc);
            if (rcv_req_desc != null) begin
               if (rst_shire_id <= 33) begin
                  //shire = m_current_shire_ids[rst_shire_id];
                  shire = m_global_attrs.get_current_shire_id(rst_shire_id);
               end
               else begin
                  shire = rst_shire_id;
               end
               `evl_log_mdesc(UVM_DEBUG, VMOD_NOC_SB, get_abstract_name(), rcv_req_desc, $sformatf("S%0d: %p sending %s to S%0d", shire, slave_type, rcv_req_desc.sprint_obj(), rcv_req_desc.get_shire_id()))
               src_req_desc = rcv_req_desc.get_parent_req_desc();
               decode_master_id(rcv_req_desc.get_bus_req_id(), rcv_req_src_shire, src_master, src_noc_layer);
               if (m_shires_in_build[rcv_req_src_shire] == 1'b0) begin
               end
               else if (src_req_desc == null) begin
                  `dut_error(get_abstract_name(), $sformatf("S%0d %p AXI RSP %s has no parent descriptor", shire, slave_type, rcv_req_desc.sprint_obj()), pk_noc_sb_error_intensity)
               end
               else begin
                  src_req_desc.set_bus_rsp_valid();
                  src_req_desc.replicate_rsp_info(rcv_req_desc, 0, 1'b1);
                  src_req_desc.set_bus_rsp_valid(0, get_cycle_count());
                  rst_src_shire = src_req_desc.get_shire_id();
                  if (rst_src_shire <= 33) begin
                     //src_shire = m_current_shire_ids[rst_src_shire];
                     src_shire = m_global_attrs.get_current_shire_id(rst_src_shire);
                  end
                  else begin
                     src_shire = rst_src_shire;
                  end

                  `evl_log_mdesc(UVM_HIGH, VMOD_NOC_SB, get_abstract_name(), rcv_req_desc, $sformatf("S%0d: Putting %0s in S%0d queue... ", shire, rcv_req_desc.sprint_obj(), shire))

                  case (src_master)
                     L2TOL3_M, IO_TOL3_M, PSHIRE_TOL3_M  : begin
                                                            if (src_noc_layer == 3) begin
                                                               m_active_l3d_mesh_rsp_descs[shire][src_shire].push_back(rcv_req_desc);
                                                            end
                                                            else if (src_noc_layer == 2) begin
                                                               m_active_l3c_mesh_rsp_descs[shire][src_shire].push_back(rcv_req_desc);
                                                            end
                                                            else if (src_noc_layer == 1) begin
                                                               m_active_l3b_mesh_rsp_descs[shire][src_shire].push_back(rcv_req_desc);
                                                            end
                                                            else begin
                                                               m_active_l3_mesh_rsp_descs[shire][src_shire].push_back(rcv_req_desc);
                                                            end
                                                           end
                     L3TOSYS_M                           : m_active_sys_mesh_rsp_descs[shire][src_shire].push_back(rcv_req_desc);
                     UCTOL3_M                            : m_active_uctol3_mesh_rsp_descs[shire][src_shire].push_back(rcv_req_desc);
                     UCTOSYS_M                           : m_active_uctosys_mesh_rsp_descs[shire][src_shire].push_back(rcv_req_desc);
                     MCTOSYS_M                           : m_active_mctosys_mesh_rsp_descs[shire][src_shire].push_back(rcv_req_desc);
                     IO_PM                               : m_active_io_pm_mesh_rsp_descs[shire][src_shire].push_back(rcv_req_desc);
                     IO_TOSYS_M                          : m_active_io_tosys_mesh_rsp_descs[shire][src_shire].push_back(rcv_req_desc);
                     PSHIRE_TOSYS_M                      : m_active_ps_tosys_mesh_rsp_descs[shire][src_shire].push_back(rcv_req_desc);
                  endcase
               end
            end
         end
      join_none
   endtask : watch_for_sent_responses


   //-----------------------------------------------------------------------------------------------
   //
   // Task: watch_for_received_responses
   //
   task watch_for_received_responses(input uvm_tlm_analysis_fifo #(evl_req_desc) req_fifo, input evl_shire_id_t rst_shire_id, input evl_noc_master_type_t master_type, input int noc_layer = 0);
      evl_req_desc         rcv_req_desc;
      evl_bus_req_id_t     rcv_bus_req_id;
      evl_paddr_t          rcv_req_addr;
      evl_bus_cmd_t        rcv_bus_cmd;
      evl_trans_id_t       rcv_req_tid;
      evl_bus_qos_t        rcv_req_qos;
      evl_req_desc         active_req_desc;
      evl_bus_req_id_t     active_bus_req_id;
      bit                  rsp_match_found;
      bit                  req_match_found;
      evl_shire_id_t       target_shire;
      evl_shire_id_t       shire;
      evl_noc_slave_type_t tgt_slave_type;
      evl_req_desc         filter_req_descs[$];
      int                  filter_req_count;

      if (req_fifo == null) begin
         return;
      end
      fork
         forever begin
            req_fifo.get(rcv_req_desc);
            if (rcv_req_desc != null) begin
               //if (rcv_req_desc.get_bus_rsp_name() != "AXI_RESP_DECERR") begin
               //if (rcv_req_desc.bus_rsp_is_valid() == 1'b1) begin
               if (rst_shire_id <= 33) begin
                  //shire = m_current_shire_ids[rst_shire_id];
                  shire = m_global_attrs.get_current_shire_id(rst_shire_id);
               end
               else begin
                  shire = rst_shire_id;
               end
               rsp_match_found = 1'b0;
               `evl_log_mdesc(UVM_HIGH, VMOD_NOC_SB, get_abstract_name(), rcv_req_desc, $sformatf("S%0d: %p AXI RESP with ID 0x%0x seen!", shire, master_type, rcv_req_desc.get_bus_req_id()))
               rcv_req_addr   = rcv_req_desc.get_paddr();
               rcv_bus_req_id = rcv_req_desc.get_bus_req_id();
               rcv_bus_cmd    = rcv_req_desc.get_bus_cmd();
               rcv_req_tid    = rcv_req_desc.get_trans_id();
               rcv_req_qos    = rcv_req_desc.get_bus_qos();
               target_shire   = get_shire_id_from_addr(rcv_req_addr, master_type, tgt_slave_type, shire);


               if ((master_type == IO_PM) && (rcv_req_desc.bus_cmd_is_write() == 1'b1) && (tgt_slave_type == TOL3_S)) begin
               end
               else if ((master_type == PSHIRE_TOSYS_M) && (rcv_req_desc.bus_cmd_is_write() == 1'b1) && (tgt_slave_type == TOL3_S)) begin
               end
               else if ((master_type == MCTOSYS_M) && (rcv_req_desc.bus_cmd_is_write() == 1'b0) && (tgt_slave_type != IO_PS)) begin
               end
               else if ((master_type == IO_TOSYS_M) && (rcv_req_desc.bus_cmd_is_write() == 1'b0) && (tgt_slave_type == TOSYS_S)) begin
               end
               else begin

                  if ((target_shire == ~evl_shire_id_t'(0)) && (rcv_req_desc.get_bus_rsp_name() != "AXI_RESP_DECERR")) begin
                     `dut_warning(get_abstract_name(), $sformatf("S%0d %p received AXI RSP from unknown slave (%0s)", shire, master_type, rcv_req_desc.sprint_obj()))
                  end
                  if ((target_shire == ~evl_shire_id_t'(0)) && (rcv_req_desc.get_bus_rsp_name() == "AXI_RESP_DECERR")) begin
                  end
                  else if (m_shires_in_build[target_shire] == 1'b0) begin
                  end
                  else begin
                     `evl_log_mdesc(UVM_DEBUG, VMOD_NOC_SB, get_abstract_name(), rcv_req_desc, $sformatf("S%0d %p: checking queue from S%0d %p Slave", shire, master_type, target_shire, tgt_slave_type))

                     if (rcv_req_desc.bus_cmd_is_write() == 1'b1) begin
                        case (master_type)
                           L2TOL3_M, IO_TOL3_M, PSHIRE_TOL3_M  : begin
                                                                  if (noc_layer == 3) begin
                                                                     filter_req_count = filter_writes(m_active_l3d_mesh_rsp_descs[target_shire][shire], filter_req_descs, rcv_req_qos);
                                                                  end
                                                                  else if (noc_layer == 2) begin
                                                                     filter_req_count = filter_writes(m_active_l3c_mesh_rsp_descs[target_shire][shire], filter_req_descs, rcv_req_qos);
                                                                  end
                                                                  else if (noc_layer == 1) begin
                                                                     filter_req_count = filter_writes(m_active_l3b_mesh_rsp_descs[target_shire][shire], filter_req_descs, rcv_req_qos);
                                                                  end
                                                                  else begin
                                                                     filter_req_count = filter_writes(m_active_l3_mesh_rsp_descs[target_shire][shire], filter_req_descs, rcv_req_qos);
                                                                  end
                                                                 end
                           L3TOSYS_M                           : filter_req_count = filter_writes(m_active_sys_mesh_rsp_descs[target_shire][shire], filter_req_descs, rcv_req_qos);
                           UCTOL3_M                            : filter_req_count = filter_writes(m_active_uctol3_mesh_rsp_descs[target_shire][shire], filter_req_descs, rcv_req_qos);
                           UCTOSYS_M                           : filter_req_count = filter_writes(m_active_uctosys_mesh_rsp_descs[target_shire][shire], filter_req_descs, rcv_req_qos);
                           MCTOSYS_M                           : filter_req_count = filter_writes(m_active_mctosys_mesh_rsp_descs[target_shire][shire], filter_req_descs, rcv_req_qos);
                           IO_PM                               : begin
                                                                  if (rcv_req_addr[39:32] == 8'h01) begin
                                                                     filter_req_count = filter_writes(m_active_io_pm_mesh_rsp_descs[target_shire][shire], filter_req_descs, rcv_req_qos, 1);
                                                                  end
                                                                  else if ((rcv_req_addr[39] == 1'b1) || (rcv_req_addr[39:31] == 9'h01)) begin
                                                                     filter_req_count = filter_writes(m_active_io_pm_mesh_rsp_descs[target_shire][shire], filter_req_descs, rcv_req_qos, 0);
                                                                  end
                                                                  else begin
                                                                     filter_req_count = filter_writes(m_active_io_pm_mesh_rsp_descs[target_shire][shire], filter_req_descs, rcv_req_qos);
                                                                  end
                                                                 end
                           IO_TOSYS_M                          : filter_req_count = filter_writes(m_active_io_tosys_mesh_rsp_descs[target_shire][shire], filter_req_descs, rcv_req_qos);
                           PSHIRE_TOSYS_M                      : filter_req_count = filter_writes(m_active_ps_tosys_mesh_rsp_descs[target_shire][shire], filter_req_descs, rcv_req_qos);
                        endcase
                     end
                     else begin
                        case (master_type)
                           L2TOL3_M, IO_TOL3_M, PSHIRE_TOL3_M  : begin
                                                                  if (noc_layer == 3) begin
                                                                     filter_req_count = filter_reads(m_active_l3d_mesh_rsp_descs[target_shire][shire], filter_req_descs, rcv_req_qos);
                                                                  end
                                                                  else if (noc_layer == 2) begin
                                                                     filter_req_count = filter_reads(m_active_l3c_mesh_rsp_descs[target_shire][shire], filter_req_descs, rcv_req_qos);
                                                                  end
                                                                  else if (noc_layer == 1) begin
                                                                     filter_req_count = filter_reads(m_active_l3b_mesh_rsp_descs[target_shire][shire], filter_req_descs, rcv_req_qos);
                                                                  end
                                                                  else begin
                                                                     filter_req_count = filter_reads(m_active_l3_mesh_rsp_descs[target_shire][shire], filter_req_descs, rcv_req_qos);
                                                                  end
                                                                 end
                           L3TOSYS_M                           : filter_req_count = filter_reads(m_active_sys_mesh_rsp_descs[target_shire][shire], filter_req_descs, rcv_req_qos);
                           UCTOL3_M                            : filter_req_count = filter_reads(m_active_uctol3_mesh_rsp_descs[target_shire][shire], filter_req_descs, rcv_req_qos);
                           UCTOSYS_M                           : filter_req_count = filter_reads(m_active_uctosys_mesh_rsp_descs[target_shire][shire], filter_req_descs, rcv_req_qos);
                           MCTOSYS_M                           : filter_req_count = filter_reads(m_active_mctosys_mesh_rsp_descs[target_shire][shire], filter_req_descs, rcv_req_qos);
                           IO_PM                               : begin
                                                                  if (rcv_req_addr[39:32] == 8'h01) begin
                                                                     filter_req_count = filter_reads(m_active_io_pm_mesh_rsp_descs[target_shire][shire], filter_req_descs, rcv_req_qos, 1);
                                                                  end
                                                                  else if ((rcv_req_addr[39] == 1'b1) || (rcv_req_addr[39:31] == 9'h01)) begin
                                                                     filter_req_count = filter_reads(m_active_io_pm_mesh_rsp_descs[target_shire][shire], filter_req_descs, rcv_req_qos, 0);
                                                                  end
                                                                  else begin
                                                                     filter_req_count = filter_reads(m_active_io_pm_mesh_rsp_descs[target_shire][shire], filter_req_descs, rcv_req_qos);
                                                                  end
                                                                 end
                           IO_TOSYS_M                          : filter_req_count = filter_reads(m_active_io_tosys_mesh_rsp_descs[target_shire][shire], filter_req_descs, rcv_req_qos);
                           PSHIRE_TOSYS_M                      : filter_req_count = filter_reads(m_active_ps_tosys_mesh_rsp_descs[target_shire][shire], filter_req_descs, rcv_req_qos);
                        endcase
                     end

                     if (filter_req_count > 0) begin
                        foreach (filter_req_descs[ii]) begin
                           active_req_desc = filter_req_descs[ii];
                           active_bus_req_id = active_req_desc.get_bus_req_id();
                           if ((active_req_desc.get_paddr() == rcv_req_addr) && (active_bus_req_id[8:0] == rcv_bus_req_id) && (active_req_desc.get_trans_id() == rcv_req_tid)) begin
                              `evl_log_mdesc(UVM_DEBUG, VMOD_NOC_SB, get_abstract_name(), active_req_desc, $sformatf("S%0d %p: AXI RSP matches response sent from S%0d %p", shire, master_type, target_shire, tgt_slave_type))
                              if (ii != 0) begin
                                 `dut_error(get_abstract_name(), $sformatf("S%0d %p: received response from S%0d %p with index %0d (expected 0) (%0s)", shire, master_type, target_shire, tgt_slave_type, ii, rcv_req_desc.sprint_obj()))
                                 for(int jj = 0; jj < ii; jj++) begin
                                    `evl_log_mdesc(UVM_DEBUG, VMOD_NOC_SB, get_abstract_name(), filter_req_descs[jj], $sformatf("S%0d %p: S%0d %p response queue element %0d: %0s", shire, master_type, target_shire, tgt_slave_type, jj, filter_req_descs[jj].sprint_obj()))
                                 end
                              end
                              //filter_req_descs.delete(ii);
                              rsp_match_found = 1'b1;
                              break;
                           end
                        end
                     end
                     else begin
                        `dut_error(get_abstract_name(), $sformatf("S%0d %p filtered queue is empty", shire, master_type))
                     end


                     if (rsp_match_found == 1'b1) begin
                        //`evl_log(UVM_DEBUG, VMOD_NOC_SB, get_abstract_name(), $sformatf("S%0d %p: AXI REQ matches request from S%0d %p Master", shire, slave_type, obs_origin_shire, obs_origin_master_type))

                        case (master_type)
                           L2TOL3_M, IO_TOL3_M, PSHIRE_TOL3_M  : begin
                                                                  if (noc_layer == 3) begin
                                                                     foreach (m_active_l3d_mesh_rsp_descs[target_shire][shire][ii]) begin
                                                                        if (m_active_l3d_mesh_rsp_descs[target_shire][shire][ii] == active_req_desc) begin
                                                                           m_active_l3d_mesh_rsp_descs[target_shire][shire].delete(ii);
                                                                        end
                                                                     end
                                                                  end
                                                                  else if (noc_layer == 2) begin
                                                                     foreach (m_active_l3c_mesh_rsp_descs[target_shire][shire][ii]) begin
                                                                        if (m_active_l3c_mesh_rsp_descs[target_shire][shire][ii] == active_req_desc) begin
                                                                           m_active_l3c_mesh_rsp_descs[target_shire][shire].delete(ii);
                                                                        end
                                                                     end
                                                                  end
                                                                  else if (noc_layer == 1) begin
                                                                     foreach (m_active_l3b_mesh_rsp_descs[target_shire][shire][ii]) begin
                                                                        if (m_active_l3b_mesh_rsp_descs[target_shire][shire][ii] == active_req_desc) begin
                                                                           m_active_l3b_mesh_rsp_descs[target_shire][shire].delete(ii);
                                                                        end
                                                                     end
                                                                  end
                                                                  else begin
                                                                     foreach (m_active_l3_mesh_rsp_descs[target_shire][shire][ii]) begin
                                                                        if (m_active_l3_mesh_rsp_descs[target_shire][shire][ii] == active_req_desc) begin
                                                                           m_active_l3_mesh_rsp_descs[target_shire][shire].delete(ii);
                                                                        end
                                                                     end
                                                                  end
                                                                 end
                           L3TOSYS_M                           : begin
                                                                  foreach (m_active_sys_mesh_rsp_descs[target_shire][shire][ii]) begin
                                                                     if (m_active_sys_mesh_rsp_descs[target_shire][shire][ii] == active_req_desc) begin
                                                                        m_active_sys_mesh_rsp_descs[target_shire][shire].delete(ii);
                                                                     end
                                                                  end
                                                                 end
                           UCTOL3_M                            : begin
                                                                  foreach (m_active_uctol3_mesh_rsp_descs[target_shire][shire][ii]) begin
                                                                     if (m_active_uctol3_mesh_rsp_descs[target_shire][shire][ii] == active_req_desc) begin
                                                                        m_active_uctol3_mesh_rsp_descs[target_shire][shire].delete(ii);
                                                                     end
                                                                  end
                                                                 end
                           UCTOSYS_M                           : begin
                                                                  foreach (m_active_uctosys_mesh_rsp_descs[target_shire][shire][ii]) begin
                                                                     if (m_active_uctosys_mesh_rsp_descs[target_shire][shire][ii] == active_req_desc) begin
                                                                        m_active_uctosys_mesh_rsp_descs[target_shire][shire].delete(ii);
                                                                     end
                                                                  end
                                                                 end
                           MCTOSYS_M                           : begin
                                                                  foreach (m_active_mctosys_mesh_rsp_descs[target_shire][shire][ii]) begin
                                                                     if (m_active_mctosys_mesh_rsp_descs[target_shire][shire][ii] == active_req_desc) begin
                                                                        m_active_mctosys_mesh_rsp_descs[target_shire][shire].delete(ii);
                                                                     end
                                                                  end
                                                                 end
                           IO_PM                               : begin
                                                                  foreach (m_active_io_pm_mesh_rsp_descs[target_shire][shire][ii]) begin
                                                                     if (m_active_io_pm_mesh_rsp_descs[target_shire][shire][ii] == active_req_desc) begin
                                                                        m_active_io_pm_mesh_rsp_descs[target_shire][shire].delete(ii);
                                                                     end
                                                                  end
                                                                 end
                           IO_TOSYS_M                          : begin
                                                                  foreach (m_active_io_tosys_mesh_rsp_descs[target_shire][shire][ii]) begin
                                                                     if (m_active_io_tosys_mesh_rsp_descs[target_shire][shire][ii] == active_req_desc) begin
                                                                        m_active_io_tosys_mesh_rsp_descs[target_shire][shire].delete(ii);
                                                                     end
                                                                  end
                                                                 end
                           PSHIRE_TOSYS_M                      : begin
                                                                  foreach (m_active_ps_tosys_mesh_rsp_descs[target_shire][shire][ii]) begin
                                                                     if (m_active_ps_tosys_mesh_rsp_descs[target_shire][shire][ii] == active_req_desc) begin
                                                                        m_active_ps_tosys_mesh_rsp_descs[target_shire][shire].delete(ii);
                                                                     end
                                                                  end
                                                                 end
                        endcase
                     end
                     else begin
                        `dut_error(get_abstract_name(), $sformatf("S%0d %p received unexpected AXI RSP (%0s)", shire, master_type, rcv_req_desc.sprint_obj()), pk_noc_sb_error_intensity)
                     end

                  end
               end
            end
         end
      join_none
   endtask : watch_for_received_responses

   //-----------------------------------------------------------------------------------------------
   //
   // Task: watch_for_rbm_transactions
   //
   task watch_for_rbm_transactions(input uvm_tlm_analysis_fifo #(evl_req_desc) req_fifo);
      evl_req_desc         rcv_req_desc;
      evl_req_desc         tmp_req_desc;
      evl_shire_id_t       tgt_shire_id;
      evl_shire_id_t       tgt_shire_new_id;
      evl_paddr_t          new_base_addr;
      evl_paddr_t          rcv_req_addr;
      //evl_line_data        rcv_req_data;
      evl_word_t           rcv_req_data;
      evl_xword_t          rcv_req_xword;
      evl_noc_slave_type_t tgt_slave_type;
      int                  addr_found;
      int                  rcv_req_addr_offset;

      if (req_fifo == null) begin
         return;
      end
      fork
         forever begin
            req_fifo.get(rcv_req_desc);
            `evl_log_mdesc(UVM_HIGH, VMOD_NOC_SB, get_abstract_name(), rcv_req_desc, $sformatf("S254 RBM: AXI REQ seen!, %0s", rcv_req_desc.sprint_obj()))
            addr_found = -1;
            if ((rcv_req_desc != null) && (rcv_req_desc.bus_cmd_is_write() == 1'b1)) begin
               rcv_req_addr = rcv_req_desc.get_paddr();
               foreach (m_noc_sh0_tosys_adbase_reg[ii]) begin
                  if (rcv_req_addr == m_noc_sh0_tosys_adbase_reg[ii]) begin
                     tgt_shire_id = get_minion_shire_virt_id(ii);
                     `evl_log_mdesc(UVM_HIGH, VMOD_NOC_SB, get_abstract_name(), rcv_req_desc, $sformatf("S254 RBM seems to be reprogramming S%0d!", tgt_shire_id))
                     addr_found = 1;
                     rcv_req_addr_offset = rcv_req_addr[5:2];
                     break;
                  end
               end
               if (addr_found == 1) begin
                  `evl_log_mdesc(UVM_HIGH, VMOD_NOC_SB, get_abstract_name(), rcv_req_desc, $sformatf("getting req_data from rcv_req_desc!"))
                  //wait (rcv_req_desc.get_req_data() != null);
                  //wait (rcv_req_desc.get_req_word(rcv_req_addr_offset) != null);
                  $cast(tmp_req_desc, rcv_req_desc.clone());
                  void'(rcv_req_desc.cascade_req_data_to_dst(tmp_req_desc));
                  //wait(rcv_req_desc.cascade_req_data_to_dst(tmp_req_desc));
                  //rcv_req_data = tmp_req_desc.get_req_data();
                  //rcv_req_data = rcv_req_desc.get_req_data();
                  rcv_req_data = rcv_req_desc.get_req_word(rcv_req_addr_offset);
                  `evl_log_mdesc(UVM_HIGH, VMOD_NOC_SB, get_abstract_name(), rcv_req_desc, $sformatf("getting data from req_data!"))
                  //rcv_req_xword = rcv_req_data.get_xword(0);
                  `evl_log_mdesc(UVM_HIGH, VMOD_NOC_SB, get_abstract_name(), rcv_req_desc, $sformatf("S254 RBM: req_data: 0x%0x", rcv_req_data))
                  //new_base_addr = {1'b1, rcv_req_xword[31:0]};
                  new_base_addr = {1'b1, rcv_req_data};
                  `evl_log_mdesc(UVM_HIGH, VMOD_NOC_SB, get_abstract_name(), rcv_req_desc, $sformatf("new_base_addr: 0x%0x", new_base_addr))
                  tgt_shire_new_id  = get_shire_id_from_addr(new_base_addr, L3TOSYS_M, tgt_slave_type, evl_shire_id_t'(0));
                  `evl_log_mdesc(UVM_HIGH, VMOD_NOC_SB, get_abstract_name(), rcv_req_desc, $sformatf("New shire ID: %0d", tgt_shire_new_id))
                  if (tgt_shire_new_id != tgt_shire_id) begin
                     `evl_log_mdesc(UVM_HIGH, VMOD_NOC_SB, get_abstract_name(), rcv_req_desc, $sformatf("S254: RBM seems to be reprogramming S%0d to S%0d!", tgt_shire_id, tgt_shire_new_id))
                     //change_shire_id(tgt_shire_id, tgt_shire_new_id);
                     m_global_attrs.change_current_shire_id(tgt_shire_id, tgt_shire_new_id);
                  end
               end
            end
         end
      join_none

   endtask : watch_for_rbm_transactions

   //-----------------------------------------------------------------------------------------------
   //
   // Task: match_observed_request
   //
   function evl_req_desc match_observed_request(input evl_req_desc obs_req_desc, input evl_shire_id_t rst_shire_id, input evl_noc_slave_type_t slave_type);
      evl_bus_req_id_t      obs_bus_req_id;
      evl_paddr_t           obs_req_addr;
      evl_bus_cmd_t         obs_bus_cmd;
      evl_bus_qos_t         obs_req_qos;
      evl_req_desc          active_req_desc;
      evl_noc_master_type_t obs_origin_master_type;
      evl_shire_id_t        obs_origin_shire;
      evl_shire_id_t        rst_origin_shire;
      evl_shire_id_t        shire;
      int                   noc_layer;
      bit                   match_found;
      evl_req_desc          filter_req_descs[$];
      int                   filter_req_count;

      obs_req_addr = obs_req_desc.get_paddr();
      obs_bus_req_id = obs_req_desc.get_bus_req_id();
      obs_bus_cmd = obs_req_desc.get_bus_cmd();
      obs_req_qos = obs_req_desc.get_bus_qos();

      if (rst_shire_id <= 33) begin
         //shire = m_current_shire_ids[rst_shire_id];
         shire = m_global_attrs.get_current_shire_id(rst_shire_id);
      end
      else begin
         shire = rst_shire_id;
      end

      if (check_slave_address(shire, obs_req_addr, slave_type) != 1'b1) begin
         `dut_error(get_abstract_name(), $sformatf("S%0d %0p: Received AXI REQ with ID 0x%0x has illegal address 0x%0x", shire, slave_type, obs_bus_req_id, obs_req_addr), pk_noc_sb_error_intensity)
      end

      `evl_log_mdesc(UVM_DEBUG, VMOD_NOC_SB, get_abstract_name(), obs_req_desc, $sformatf("S%0d %p: decoding Master ID 0x%0x (%0s)", shire, slave_type, obs_bus_req_id, obs_req_desc.sprint_obj()))
      decode_master_id(obs_bus_req_id, rst_origin_shire, obs_origin_master_type, noc_layer);
      if (rst_origin_shire <= 33) begin
         //obs_origin_shire = m_current_shire_ids[rst_origin_shire];
         obs_origin_shire = m_global_attrs.get_current_shire_id(rst_origin_shire);
      end
      else begin
         obs_origin_shire = rst_origin_shire;
      end
      `evl_log_mdesc(UVM_DEBUG, VMOD_NOC_SB, get_abstract_name(), obs_req_desc, $sformatf("S%0d %p: Master ID 0x%0x, decoded to S%0d %p (%0s)", shire, slave_type, obs_bus_req_id, obs_origin_shire, obs_origin_master_type, obs_req_desc.sprint_obj()))

      if (obs_origin_shire == ~evl_shire_id_t'(0)) begin
         `dut_error(get_abstract_name(), $sformatf("S%0d %p: Could not decode Master Bridge ID 0x%0x (%0s)", shire, slave_type, obs_bus_req_id, obs_req_desc.sprint_obj()))
      end
      else if (m_shires_in_build[obs_origin_shire] == 1'b0) begin
         `dut_warning(get_abstract_name(), $sformatf("S%0d %p: Received transaction from S%0d, which is not connected to the NoC Scoreboard. Ignoring this transaction. (%0s)", shire, slave_type, obs_origin_shire, obs_req_desc.sprint_obj()))
      end
      else begin
         match_found = 1'b0;

         if (obs_req_desc.bus_cmd_is_write() == 1'b1) begin
            case (obs_origin_master_type)
               L2TOL3_M, IO_TOL3_M, PSHIRE_TOL3_M  : begin
                                                      if (noc_layer == 3) begin
                                                         filter_req_count = filter_writes(m_active_l3d_mesh_req_descs[obs_origin_shire][shire], filter_req_descs, obs_req_qos);
                                                      end
                                                      else if (noc_layer == 2) begin
                                                         filter_req_count = filter_writes(m_active_l3c_mesh_req_descs[obs_origin_shire][shire], filter_req_descs, obs_req_qos);
                                                      end
                                                      else if (noc_layer == 1) begin
                                                         filter_req_count = filter_writes(m_active_l3b_mesh_req_descs[obs_origin_shire][shire], filter_req_descs, obs_req_qos);
                                                      end
                                                      else begin
                                                         filter_req_count = filter_writes(m_active_l3_mesh_req_descs[obs_origin_shire][shire], filter_req_descs, obs_req_qos);
                                                      end
                                                     end
               L3TOSYS_M                           : filter_req_count = filter_writes(m_active_sys_mesh_req_descs[obs_origin_shire][shire], filter_req_descs, obs_req_qos);
               UCTOL3_M                            : filter_req_count = filter_writes(m_active_uctol3_mesh_req_descs[obs_origin_shire][shire], filter_req_descs, obs_req_qos);
               UCTOSYS_M                           : filter_req_count = filter_writes(m_active_uctosys_mesh_req_descs[obs_origin_shire][shire], filter_req_descs, obs_req_qos);
               MCTOSYS_M                           : filter_req_count = filter_writes(m_active_mctosys_mesh_req_descs[obs_origin_shire][shire], filter_req_descs, obs_req_qos);
               IO_PM                               : begin
                                                      if (obs_req_addr[39:32] == 8'h01) begin
                                                         filter_req_count = filter_writes(m_active_io_pm_mesh_req_descs[obs_origin_shire][shire], filter_req_descs, obs_req_qos, 1);
                                                      end
                                                      else if ((obs_req_addr[39] == 1'b1) || (obs_req_addr[39:31] == 9'h01)) begin
                                                         filter_req_count = filter_writes(m_active_io_pm_mesh_req_descs[obs_origin_shire][shire], filter_req_descs, obs_req_qos, 0);
                                                      end
                                                      else begin
                                                         filter_req_count = filter_writes(m_active_io_pm_mesh_req_descs[obs_origin_shire][shire], filter_req_descs, obs_req_qos);
                                                      end
                                                     end
               IO_TOSYS_M                          : filter_req_count = filter_writes(m_active_io_tosys_mesh_req_descs[obs_origin_shire][shire], filter_req_descs, obs_req_qos);
               PSHIRE_TOSYS_M                      : filter_req_count = filter_writes(m_active_ps_tosys_mesh_req_descs[obs_origin_shire][shire], filter_req_descs, obs_req_qos);
            endcase
         end
         else begin
            case (obs_origin_master_type)
               L2TOL3_M, IO_TOL3_M, PSHIRE_TOL3_M  : begin
                                                      if (noc_layer == 3) begin
                                                         filter_req_count = filter_reads(m_active_l3d_mesh_req_descs[obs_origin_shire][shire], filter_req_descs, obs_req_qos);
                                                      end
                                                      else if (noc_layer == 2) begin
                                                         filter_req_count = filter_reads(m_active_l3c_mesh_req_descs[obs_origin_shire][shire], filter_req_descs, obs_req_qos);
                                                      end
                                                      else if (noc_layer == 1) begin
                                                         filter_req_count = filter_reads(m_active_l3b_mesh_req_descs[obs_origin_shire][shire], filter_req_descs, obs_req_qos);
                                                      end
                                                      else begin
                                                         filter_req_count = filter_reads(m_active_l3_mesh_req_descs[obs_origin_shire][shire], filter_req_descs, obs_req_qos);
                                                      end
                                                     end
               L3TOSYS_M                           : filter_req_count = filter_reads(m_active_sys_mesh_req_descs[obs_origin_shire][shire], filter_req_descs, obs_req_qos);
               UCTOL3_M                            : filter_req_count = filter_reads(m_active_uctol3_mesh_req_descs[obs_origin_shire][shire], filter_req_descs, obs_req_qos);
               UCTOSYS_M                           : filter_req_count = filter_reads(m_active_uctosys_mesh_req_descs[obs_origin_shire][shire], filter_req_descs, obs_req_qos);
               MCTOSYS_M                           : filter_req_count = filter_reads(m_active_mctosys_mesh_req_descs[obs_origin_shire][shire], filter_req_descs, obs_req_qos);
               IO_PM                               : begin
                                                      if (obs_req_addr[39:32] == 8'h01) begin
                                                         filter_req_count = filter_reads(m_active_io_pm_mesh_req_descs[obs_origin_shire][shire], filter_req_descs, obs_req_qos, 1);
                                                      end
                                                      else if ((obs_req_addr[39] == 1'b1) || (obs_req_addr[39:31] == 9'h01)) begin
                                                         filter_req_count = filter_reads(m_active_io_pm_mesh_req_descs[obs_origin_shire][shire], filter_req_descs, obs_req_qos, 0);
                                                      end
                                                      else begin
                                                         filter_req_count = filter_reads(m_active_io_pm_mesh_req_descs[obs_origin_shire][shire], filter_req_descs, obs_req_qos);
                                                      end
                                                     end
               IO_TOSYS_M                          : filter_req_count = filter_reads(m_active_io_tosys_mesh_req_descs[obs_origin_shire][shire], filter_req_descs, obs_req_qos);
               PSHIRE_TOSYS_M                      : filter_req_count = filter_reads(m_active_ps_tosys_mesh_req_descs[obs_origin_shire][shire], filter_req_descs, obs_req_qos);
            endcase
         end

         `evl_log_mdesc(UVM_DEBUG, VMOD_NOC_SB, get_abstract_name(), obs_req_desc, $sformatf("S%0d %p: checking queue from S%0d %p Master", shire, slave_type, obs_origin_shire, obs_origin_master_type))

         if (filter_req_count > 0) begin
            foreach (filter_req_descs[ii]) begin
               if ((filter_req_descs[ii].m_child_desc_queues.size() == 0) && (match_found == 1'b0)) begin
                  active_req_desc = filter_req_descs[ii];
                  if ((active_req_desc.get_bus_req_id() == obs_bus_req_id) && (active_req_desc.get_paddr() == obs_req_addr) && (active_req_desc.get_bus_cmd() == obs_bus_cmd)) begin
                     if (ii != 0) begin
                        `dut_error(get_abstract_name(), $sformatf("S%0d %p: received transaction from S%0d %p with index %0d (expected 0) (%0s)", shire, slave_type, obs_origin_shire, obs_origin_master_type, ii, obs_req_desc.sprint_obj()))
                        for(int jj = 0; jj < ii; jj++) begin
                           `evl_log_mdesc(UVM_DEBUG, VMOD_NOC_SB, get_abstract_name(), obs_req_desc, $sformatf("S%0d %p: S%0d %p transaction queue element %0d: %0s", shire, slave_type, obs_origin_shire, obs_origin_master_type, jj, filter_req_descs[jj].sprint_obj()))
                        end
                     end
                     //filter_req_descs.delete(ii);
                     match_found = 1'b1;
                     break;
                  end
               end
            end
         end
         else begin
            `dut_error(get_abstract_name(), $sformatf("S%0d %p filtered queue is empty", shire, slave_type))
         end

      end

      if (match_found == 1'b1) begin
         `evl_log_mdesc(UVM_DEBUG, VMOD_NOC_SB, get_abstract_name(), obs_req_desc, $sformatf("S%0d %p: AXI REQ matches request from S%0d %p Master (%0s)", shire, slave_type, obs_origin_shire, obs_origin_master_type, active_req_desc.sprint_obj()))
         `ifdef EVL_COVERAGE_ON
            m_noc_cov.sample_slave(shire, obs_origin_shire, slave_type, noc_layer);
         `endif

         case (obs_origin_master_type)
            L2TOL3_M, IO_TOL3_M, PSHIRE_TOL3_M  : begin
                                                   if (noc_layer == 3) begin
                                                      foreach (m_active_l3d_mesh_req_descs[obs_origin_shire][shire][ii]) begin
                                                         if (m_active_l3d_mesh_req_descs[obs_origin_shire][shire][ii] == active_req_desc) begin
                                                            m_active_l3d_mesh_req_descs[obs_origin_shire][shire].delete(ii);
                                                         end
                                                      end
                                                   end
                                                   else if (noc_layer == 2) begin
                                                      foreach (m_active_l3c_mesh_req_descs[obs_origin_shire][shire][ii]) begin
                                                         if (m_active_l3c_mesh_req_descs[obs_origin_shire][shire][ii] == active_req_desc) begin
                                                            m_active_l3c_mesh_req_descs[obs_origin_shire][shire].delete(ii);
                                                         end
                                                      end
                                                   end
                                                   else if (noc_layer == 1) begin
                                                      foreach (m_active_l3b_mesh_req_descs[obs_origin_shire][shire][ii]) begin
                                                         if (m_active_l3b_mesh_req_descs[obs_origin_shire][shire][ii] == active_req_desc) begin
                                                            m_active_l3b_mesh_req_descs[obs_origin_shire][shire].delete(ii);
                                                         end
                                                      end
                                                   end
                                                   else begin
                                                      foreach (m_active_l3_mesh_req_descs[obs_origin_shire][shire][ii]) begin
                                                         if (m_active_l3_mesh_req_descs[obs_origin_shire][shire][ii] == active_req_desc) begin
                                                            m_active_l3_mesh_req_descs[obs_origin_shire][shire].delete(ii);
                                                         end
                                                      end
                                                   end
                                                  end
            L3TOSYS_M                           : begin
                                                   foreach (m_active_sys_mesh_req_descs[obs_origin_shire][shire][ii]) begin
                                                      if (m_active_sys_mesh_req_descs[obs_origin_shire][shire][ii] == active_req_desc) begin
                                                         m_active_sys_mesh_req_descs[obs_origin_shire][shire].delete(ii);
                                                      end
                                                   end
                                                  end
            UCTOL3_M                            : begin
                                                   foreach (m_active_uctol3_mesh_req_descs[obs_origin_shire][shire][ii]) begin
                                                      if (m_active_uctol3_mesh_req_descs[obs_origin_shire][shire][ii] == active_req_desc) begin
                                                         m_active_uctol3_mesh_req_descs[obs_origin_shire][shire].delete(ii);
                                                      end
                                                   end
                                                  end
            UCTOSYS_M                           : begin
                                                   foreach (m_active_uctosys_mesh_req_descs[obs_origin_shire][shire][ii]) begin
                                                      if (m_active_uctosys_mesh_req_descs[obs_origin_shire][shire][ii] == active_req_desc) begin
                                                         m_active_uctosys_mesh_req_descs[obs_origin_shire][shire].delete(ii);
                                                      end
                                                   end
                                                  end
            MCTOSYS_M                           : begin
                                                   foreach (m_active_mctosys_mesh_req_descs[obs_origin_shire][shire][ii]) begin
                                                      if (m_active_mctosys_mesh_req_descs[obs_origin_shire][shire][ii] == active_req_desc) begin
                                                         m_active_mctosys_mesh_req_descs[obs_origin_shire][shire].delete(ii);
                                                      end
                                                   end
                                                  end
            IO_PM                               : begin
                                                   foreach (m_active_io_pm_mesh_req_descs[obs_origin_shire][shire][ii]) begin
                                                      if (m_active_io_pm_mesh_req_descs[obs_origin_shire][shire][ii] == active_req_desc) begin
                                                         m_active_io_pm_mesh_req_descs[obs_origin_shire][shire].delete(ii);
                                                      end
                                                   end
                                                  end
            IO_TOSYS_M                          : begin
                                                   foreach (m_active_io_tosys_mesh_req_descs[obs_origin_shire][shire][ii]) begin
                                                      if (m_active_io_tosys_mesh_req_descs[obs_origin_shire][shire][ii] == active_req_desc) begin
                                                         m_active_io_tosys_mesh_req_descs[obs_origin_shire][shire].delete(ii);
                                                      end
                                                   end
                                                  end
            PSHIRE_TOSYS_M                      : begin
                                                   foreach (m_active_ps_tosys_mesh_req_descs[obs_origin_shire][shire][ii]) begin
                                                      if (m_active_ps_tosys_mesh_req_descs[obs_origin_shire][shire][ii] == active_req_desc) begin
                                                         m_active_ps_tosys_mesh_req_descs[obs_origin_shire][shire].delete(ii);
                                                      end
                                                   end
                                                  end
         endcase

         return active_req_desc;
      end
      else if (m_shires_in_build[obs_origin_shire] == 1'b0) begin
      end
      else begin
         `dut_error(get_abstract_name(), $sformatf("S%0d %0p: Unable to find matching REQ. ID 0x%0x, CMD %0p, ADDR 0x%0x", shire, slave_type, obs_bus_req_id, obs_bus_cmd, obs_req_addr), pk_noc_sb_error_intensity)
         return null;
      end

   endfunction : match_observed_request

   //-----------------------------------------------------------------------------------------------
   //
   // Function: connect_port_to_my_export
   //
   virtual function void connect_port_to_my_export(input uvm_analysis_port#(evl_req_desc) port_in,
                                                   input string                           port_name      = "unknown",
                                                   input int                              port_number    = 0,
                                                   input int                              subport_number = 0);

      uvm_analysis_export #(evl_req_desc) tlm_port;
      if (port_in == null) begin
         return;
      end
      case (port_name)
         "m_l3_mesh_req_export"           : tlm_port = m_l3_mesh_req_export[port_number];
         "m_l3_mesh_rsp_export"           : tlm_port = m_l3_mesh_rsp_export[port_number];
         "m_mesh_l3_rsp_export"           : tlm_port = m_mesh_l3_rsp_export[port_number];
         "m_l3b_mesh_req_export"          : tlm_port = m_l3b_mesh_req_export[port_number];
         "m_l3b_mesh_rsp_export"          : tlm_port = m_l3b_mesh_rsp_export[port_number];
         "m_mesh_l3b_rsp_export"          : tlm_port = m_mesh_l3b_rsp_export[port_number];
         "m_l3c_mesh_req_export"          : tlm_port = m_l3c_mesh_req_export[port_number];
         "m_l3c_mesh_rsp_export"          : tlm_port = m_l3c_mesh_rsp_export[port_number];
         "m_mesh_l3c_rsp_export"          : tlm_port = m_mesh_l3c_rsp_export[port_number];
         "m_l3d_mesh_req_export"          : tlm_port = m_l3d_mesh_req_export[port_number];
         "m_l3d_mesh_rsp_export"          : tlm_port = m_l3d_mesh_rsp_export[port_number];
         "m_mesh_l3d_rsp_export"          : tlm_port = m_mesh_l3d_rsp_export[port_number];
         "m_sys_mesh_req_export"          : tlm_port = m_sys_mesh_req_export[port_number];
         "m_sys_mesh_rsp_export"          : tlm_port = m_sys_mesh_rsp_export[port_number];
         "m_mesh_sbm_rsp_export"          : tlm_port = m_mesh_sbm_rsp_export[port_number];
         "m_uctol3_mesh_req_export"       : tlm_port = m_uctol3_mesh_req_export[port_number];
         "m_uctol3_mesh_rsp_export"       : tlm_port = m_uctol3_mesh_rsp_export[port_number];
         "m_uctosys_mesh_req_export"      : tlm_port = m_uctosys_mesh_req_export[port_number];
         "m_uctosys_mesh_rsp_export"      : tlm_port = m_uctosys_mesh_rsp_export[port_number];
         "m_mesh_mem_rsp_export"          : tlm_port = m_mesh_mem_rsp_export[port_number];
         "m_mc_mesh_sbm_rsp_export"       : tlm_port = m_mc_mesh_sbm_rsp_export[port_number];
         "m_mctosys_mesh_req_export"      : tlm_port = m_mctosys_mesh_req_export[port_number];
         "m_mctosys_mesh_rsp_export"      : tlm_port = m_mctosys_mesh_rsp_export[port_number];
         "m_io_tol3_mesh_req_export"      : tlm_port = m_io_tol3_mesh_req_export[port_number];
         "m_io_tol3_mesh_rsp_export"      : tlm_port = m_io_tol3_mesh_rsp_export[port_number];
         "m_io_tol3b_mesh_req_export"     : tlm_port = m_io_tol3b_mesh_req_export[port_number];
         "m_io_tol3b_mesh_rsp_export"     : tlm_port = m_io_tol3b_mesh_rsp_export[port_number];
         "m_io_tol3c_mesh_req_export"     : tlm_port = m_io_tol3c_mesh_req_export[port_number];
         "m_io_tol3c_mesh_rsp_export"     : tlm_port = m_io_tol3c_mesh_rsp_export[port_number];
         "m_io_tol3d_mesh_req_export"     : tlm_port = m_io_tol3d_mesh_req_export[port_number];
         "m_io_tol3d_mesh_rsp_export"     : tlm_port = m_io_tol3d_mesh_rsp_export[port_number];
         "m_io_pm_req_export"             : tlm_port = m_io_pm_req_export[port_number];
         "m_io_pm_rsp_export"             : tlm_port = m_io_pm_rsp_export[port_number];
         "m_io_ps_rsp_export"             : tlm_port = m_io_ps_rsp_export[port_number];
         "m_io_tosys_mesh_req_export"     : tlm_port = m_io_tosys_mesh_req_export[port_number];
         "m_io_tosys_mesh_rsp_export"     : tlm_port = m_io_tosys_mesh_rsp_export[port_number];
         "m_io_main_noc_rbm_req_export"   : tlm_port = m_io_main_noc_rbm_req_export[port_number];
         "m_io_main_noc_rbm_rsp_export"   : tlm_port = m_io_main_noc_rbm_rsp_export[port_number];
         default                          : return;
      endcase

      if (tlm_port != null) begin
         `evl_log(UVM_MEDIUM, VMOD_CONFIG, get_abstract_name(), $sformatf("connecting %0s -> %0s", port_in.get_full_name(), tlm_port.get_full_name()))
         port_in.connect(tlm_port);
      end
      return;
   endfunction : connect_port_to_my_export


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_shire_id_from_addr
   //
   function evl_shire_id_t get_shire_id_from_addr (input evl_paddr_t bus_addr, input evl_noc_master_type_t master_type, output evl_noc_slave_type_t slave_type, input evl_shire_id_t shire);
      evl_shire_id_t scp_shire_id;
      evl_shire_id_t csr_shire_id;


      if ((master_type == L2TOL3_M) || (master_type == UCTOL3_M) || (master_type == IO_TOL3_M) || (master_type == PSHIRE_TOL3_M)) begin
         if (bus_addr[39] == 1'b1) begin
            if (bus_addr[38:35] == 'h0) begin
               slave_type = TOL3_S;
               return m_global_attrs.get_l3_shire_from_address(bus_addr);
            end
            else begin
               return ~evl_shire_id_t'(0);
            end
         end
         else if (bus_addr[39:31] == 9'h001) begin
            scp_shire_id = m_global_attrs.get_shire_id_from_address(bus_addr, 23);
            //`evl_log(UVM_DEBUG, VMOD_NOC_SB, get_abstract_name(), $sformatf("S%0d %p: get_shire_id_from_addr, scp_shire_id = %0d ", shire, master_type, scp_shire_id))
            if (m_min2maxscp_err == 1) begin
               if ((master_type == UCTOL3_M) && (scp_shire_id == evl_shire_id_t'('d254))) begin
                  `dut_error(get_abstract_name(), $sformatf("S%0d %p: Minion Shire to Maxion SCP traffic not supported!", shire, master_type), pk_noc_sb_error_intensity)  // This check is due to the bug in RTLMIN-5704. Atomic responses from Maxion SCP to Minion Shire hang due to
               end                                                                                                                                                         // a bug in the PU NoC. This will not be fixed for A0 so creating this Error to fail quickly instead of timeout.
            end
            if (scp_shire_id < evl_shire_id_t'('d34)) begin
               slave_type = TOL3_S;
               return scp_shire_id;
            end
            else if ((scp_shire_id == evl_shire_id_t'('d254)) && (shire != evl_shire_id_t'('d254)) && (shire != evl_shire_id_t'('d253))) begin
               slave_type = IO_PS;
               return scp_shire_id;
            end
            else begin
               return ~evl_shire_id_t'(0);
            end
         end
      end
      //if ((master_type == L2TOL3_M) || (master_type == UCTOL3_M)) begin
      //   if ((bus_addr inside {['h40000000:'h5fffffff]}) || (bus_addr inside {['h10000000:'h1fffffff]})) begin
      //      slave_type = IO_PS;
      //      return 'd254;
      //   end
      //   else begin
      //      return ~evl_shire_id_t'(0);
      //   end
      //end
      if ((master_type == L3TOSYS_M) || (master_type == UCTOSYS_M)) begin
         if (bus_addr[39:35] == 'b10000) begin
            slave_type = TOMEM_S;
            return m_global_attrs.get_memshire_virt_id_from_address(bus_addr);
         end
         else if (bus_addr[39:31] == 9'h001) begin
            scp_shire_id = m_global_attrs.get_shire_id_from_address(bus_addr, 23);
            //`evl_log(UVM_DEBUG, VMOD_NOC_SB, get_abstract_name(), $sformatf("S%0d %p: get_shire_id_from_addr, scp_shire_id = %0d ", shire, master_type, scp_shire_id))
            if (scp_shire_id == evl_shire_id_t'('d254)) begin
               slave_type = IO_PS;
               return scp_shire_id;
            end
            else begin
               return ~evl_shire_id_t'(0);
            end
         end
         else if (bus_addr inside {['h0000000000:'h002fffffff]}) begin
            slave_type = IO_PS;
            return 'd254;
         end
         else if (bus_addr inside {['h4000000000:'h7fffffffff]}) begin
            slave_type = PSHIRE_PS;
            return 'd253;
         end
      end
      if ((master_type == L3TOSYS_M) || (master_type == UCTOSYS_M)) begin
         if (bus_addr[39:32] == 8'h01) begin
            slave_type = TOSYS_S;
            csr_shire_id = m_global_attrs.get_shire_id_from_address(bus_addr, 22);
            //`evl_log(UVM_DEBUG, VMOD_NOC_SB, get_abstract_name(), $sformatf("S%0d %p: get_shire_id_from_addr, csr_shire_id = %0d ", shire, master_type, csr_shire_id))
            if (csr_shire_id < evl_shire_id_t'('d34)) begin
               slave_type = TOSYS_S;
               return csr_shire_id;
            end
            else if ((csr_shire_id >= evl_shire_id_t'('d232)) && (csr_shire_id <= evl_shire_id_t'('d239))) begin
               slave_type = MC_SB_S;
               return csr_shire_id;
            end
            else if (csr_shire_id == evl_shire_id_t'('d254)) begin
               slave_type = IO_PS;
               return csr_shire_id;
            end
            else begin
               return ~evl_shire_id_t'(0);
            end
         end
      end
      if (master_type == MCTOSYS_M) begin
         if (bus_addr[39:32] == 8'h01) begin
            slave_type = TOSYS_S;
            csr_shire_id = m_global_attrs.get_shire_id_from_address(bus_addr, 22);
            //`evl_log(UVM_DEBUG, VMOD_NOC_SB, get_abstract_name(), $sformatf("S%0d %p: get_shire_id_from_addr, csr_shire_id = %0d ", shire, master_type, csr_shire_id))
            if (csr_shire_id < evl_shire_id_t'('d34)) begin
               slave_type = TOSYS_S;
               return csr_shire_id;
            end
            else if (csr_shire_id == evl_shire_id_t'('d254)) begin
               slave_type = IO_PS;
               return csr_shire_id;
            end
            else begin
               return ~evl_shire_id_t'(0);
            end
         end
      end
      if (master_type == IO_PM) begin
         if (bus_addr[39] == 1'b1) begin
            if (bus_addr[38:35] == 'h0) begin
               slave_type = TOL3_S;
               //`evl_log(UVM_DEBUG, VMOD_NOC_SB, get_abstract_name(), $sformatf("S%0d %p: get_shire_id_from_addr, bus_addr[39] == 1'b1, bus_addr[38:35] == 'h0", shire, master_type))
               return m_global_attrs.get_l3_shire_from_address(bus_addr);
            end
            else begin
               //`evl_log(UVM_DEBUG, VMOD_NOC_SB, get_abstract_name(), $sformatf("S%0d %p: get_shire_id_from_addr, bus_addr[39] == 1'b1, bus_addr[38:35] != 'h0", shire, master_type))
               return ~evl_shire_id_t'(0);
            end
         end
         else if (bus_addr[39:32] == 8'h01) begin
            csr_shire_id = m_global_attrs.get_shire_id_from_address(bus_addr, 22);
            //`evl_log(UVM_DEBUG, VMOD_NOC_SB, get_abstract_name(), $sformatf("S%0d %p: get_shire_id_from_addr, csr_shire_id = %0d ", shire, master_type, csr_shire_id))
            if (csr_shire_id < evl_shire_id_t'('d34)) begin
               slave_type = TOSYS_S;
               return csr_shire_id;
            end
            else if ((csr_shire_id >= evl_shire_id_t'('d232)) && (csr_shire_id <= evl_shire_id_t'('d239))) begin
               slave_type = MC_SB_S;
               return csr_shire_id;
            end
            else begin
               return ~evl_shire_id_t'(0);
            end
         end
         else if (bus_addr[39:31] == 9'h001) begin
            scp_shire_id = m_global_attrs.get_shire_id_from_address(bus_addr, 23);
            //`evl_log(UVM_DEBUG, VMOD_NOC_SB, get_abstract_name(), $sformatf("S%0d %p: get_shire_id_from_addr, scp_shire_id = %0d ", shire, master_type, scp_shire_id))
            if ((scp_shire_id == evl_shire_id_t'('d32)) || (scp_shire_id == evl_shire_id_t'('d33))) begin
               slave_type = TOL3_S;
               return scp_shire_id;
            end
            else begin
               return ~evl_shire_id_t'(0);
            end
         end
         else if (bus_addr[39:29] == 11'h003) begin
            slave_type = MC_SB_S;
            return m_global_attrs.get_memshire_virt_id_from_address(bus_addr, 26);
         end
         else if (bus_addr inside {['h4000000000:'h7fffffffff]}) begin
            slave_type = PSHIRE_PS;
            return 'd253;
         end
      end
      if (master_type == IO_TOSYS_M) begin
         if ((bus_addr[39] == 1'b1) && (bus_addr[37:35] == 3'b0)) begin
            slave_type = TOMEM_S;
            return m_global_attrs.get_memshire_virt_id_from_address(bus_addr);
         end
         else if (bus_addr[39:32] == 8'h01) begin
            csr_shire_id = m_global_attrs.get_shire_id_from_address(bus_addr, 22);
            if (csr_shire_id < evl_shire_id_t'('d34)) begin
               slave_type = TOSYS_S;
               return csr_shire_id;
            end
            else begin
               return ~evl_shire_id_t'(0);
            end
         end
         else if (bus_addr inside {['h4000000000:'h7fffffffff]}) begin
            slave_type = PSHIRE_PS;
            return 'd253;
         end
      end
      if (master_type == PSHIRE_TOSYS_M) begin
         if (bus_addr[39:35] == 'b11000) begin
            slave_type = TOMEM_S;
            return m_global_attrs.get_memshire_virt_id_from_address(bus_addr);
         end
         else if (bus_addr[39:35] == 'b10000) begin
            slave_type = TOL3_S;
            return m_global_attrs.get_l3_shire_from_address(bus_addr);
         end
         else if (bus_addr[39:32] == 8'h01) begin
            slave_type = TOSYS_S;
            csr_shire_id = m_global_attrs.get_shire_id_from_address(bus_addr, 22);
            //`evl_log(UVM_DEBUG, VMOD_NOC_SB, get_abstract_name(), $sformatf("S%0d %p: get_shire_id_from_addr, csr_shire_id = %0d ", shire, master_type, csr_shire_id))
            if (csr_shire_id < evl_shire_id_t'('d34)) begin
               return csr_shire_id;
            end
            else if ((csr_shire_id >= evl_shire_id_t'('d232)) && (csr_shire_id <= evl_shire_id_t'('d239))) begin
               slave_type = MC_SB_S;
               return csr_shire_id;
            end
            else begin
               return ~evl_shire_id_t'(0);
            end
         end
         else if (bus_addr[39:31] == 9'h001) begin
            scp_shire_id = m_global_attrs.get_shire_id_from_address(bus_addr, 23);
            //`evl_log(UVM_DEBUG, VMOD_NOC_SB, get_abstract_name(), $sformatf("S%0d %p: get_shire_id_from_addr, scp_shire_id = %0d ", shire, master_type, scp_shire_id))
            if ((scp_shire_id == evl_shire_id_t'('d32)) || (scp_shire_id == evl_shire_id_t'('d33))) begin
               slave_type = TOL3_S;
               return scp_shire_id;
            end
            else begin
               return ~evl_shire_id_t'(0);
            end
         end
         else if (bus_addr[39:29] == 11'h003) begin
            slave_type = MC_SB_S;
            return m_global_attrs.get_memshire_virt_id_from_address(bus_addr, 26);
         end
         else if (bus_addr inside {['h0000000000:'h003fffffff]}) begin
            slave_type = IO_PS;
            return 'd254;
         end
         else begin
            return ~evl_shire_id_t'(0);
         end
      end

      return ~evl_shire_id_t'(0);
   endfunction : get_shire_id_from_addr


   //-----------------------------------------------------------------------------------------------
   //
   // Function: check_slave_address
   //
   function bit check_slave_address (input evl_shire_id_t shire, input evl_paddr_t bus_addr, input evl_noc_slave_type_t slave_type);

      if (slave_type == TOL3_S) begin
         return m_global_attrs.address_is_in_my_l3_slave(shire, bus_addr);
      end
      else if (slave_type == TOSYS_S) begin
         if (bus_addr[39:32] == 8'h01) begin
            return m_global_attrs.address_is_in_my_shire(shire, bus_addr, 22);
         end
      end
      else if (slave_type == TOMEM_S) begin
         if (bus_addr[39] == 1'b1) begin
            return m_global_attrs.address_is_in_my_memshire(shire, bus_addr);
         end
      end
      else if (slave_type == MC_SB_S) begin
         if (bus_addr[39:32] == 8'h01) begin
            return m_global_attrs.address_is_in_my_shire(shire, bus_addr, 22);
         end
         if (bus_addr[39:29] == 11'h003) begin
            return m_global_attrs.address_is_in_my_memshire(shire, bus_addr, 26);
         end
      end
      else if (slave_type == IO_PS) begin
         if (bus_addr[39:32] == 8'h01) begin
            return m_global_attrs.address_is_in_my_shire(shire, bus_addr, 22);
         end
         else if (bus_addr[39:31] == 9'h001) begin
            return m_global_attrs.address_is_in_my_shire(shire, bus_addr, 23);
         end
         else if (bus_addr inside {['h0000000000:'h003fffffff]}) begin
            return 1'b1;
         end
      end
      else if (slave_type == PSHIRE_PS) begin
         if (bus_addr inside {['h4000000000:'h7fffffffff]}) begin
            return 1'b1;
         end
      end

      return 1'b0;
   endfunction : check_slave_address

   //-----------------------------------------------------------------------------------------------
   //
   // Function: decode_master_id
   //
   function void decode_master_id (input evl_bus_req_id_t req_id, output evl_shire_id_t shire, output evl_noc_master_type_t master_type, output int noc_layer);
      int            master_id = req_id[19:10];
      bit            sib_bit = req_id[9];
      evl_shire_id_t phys_shire_id;
      bit            match_found;

      noc_layer = 0;
      match_found = 0;

      if (master_id <= 13) begin
         if ((master_id >= 4) && (master_id <= 11)) begin
            foreach (memshire_master_ids[ii]) begin
               if (memshire_master_ids[ii] == master_id) begin
                  shire = get_memshire_virt_id_from_index(ii);
                  master_type = MCTOSYS_M;
                  break;
               end
            end
         end
         else begin
            case (master_id)
               `IOSHIRE_PM_MASTER_ID    : begin
                                           shire = 'd254;
                                           master_type = IO_PM;
                                          end
               `IOSHIRE_TOL3_MASTER_ID  : begin
                                           shire = 'd254;
                                           master_type = IO_TOL3_M;
                                          end
               `IOSHIRE_TOL3B_MASTER_ID : begin
                                           shire = 'd254;
                                           master_type = IO_TOL3_M;
                                           noc_layer = 1;
                                          end
               `IOSHIRE_TOSYS_MASTER_ID : begin
                                           shire = 'd254;
                                           master_type = IO_TOSYS_M;
                                          end
               `PSHIRE_TOL3_MASTER_ID   : begin
                                           shire = 'd253;
                                           master_type = PSHIRE_TOL3_M;
                                          end
               `PSHIRE_TOSYS_MASTER_ID  : begin
                                           shire = 'd253;
                                           master_type = PSHIRE_TOSYS_M;
                                          end
            endcase
         end
      end
      else begin
         if ((master_id >= 252) && (master_id <= 285)) begin
            foreach (tol3_sib_master_ids[ii]) begin
               if (tol3_sib_master_ids[ii] == master_id) begin
                  phys_shire_id = ii;
                  master_type = (sib_bit == 1) ? UCTOL3_M : L2TOL3_M;
                  break;
               end
            end
         end
         else if ((master_id >= 'd286) && (master_id <= 'd319)) begin
            foreach (tosys_sib_master_ids[ii]) begin
               if (tosys_sib_master_ids[ii] == master_id) begin
                  phys_shire_id = ii;
                  master_type = (sib_bit == 1) ? UCTOSYS_M : L3TOSYS_M;
                  break;
               end
            end
         end
         else begin
            foreach (l2tol3_master_ids[ii]) begin
               foreach (l2tol3_master_ids[ii][jj]) begin
                  if (l2tol3_master_ids[ii][jj] == master_id) begin
                     phys_shire_id = jj;
                     master_type = L2TOL3_M;
                     noc_layer = ii;
                     match_found = 1'b1;
                     break;
                  end
               end
               if (match_found == 1'b1) begin
                  break;
               end
            end
         end
         shire = get_minion_shire_virt_id(phys_shire_id);
      end

   endfunction : decode_master_id

   //-----------------------------------------------------------------------------------------------
   //
   // Function: predict_req_id
   //
   function evl_bus_req_id_t predict_req_id (input evl_shire_id_t shire, input evl_bus_req_id_t source_req_id, input evl_noc_master_type_t master_type, input int noc_layer = 0);
      bit [8:0]      master_id;
      bit            sib_bit;
      evl_shire_id_t phys_shire_id;
      evl_shire_id_t memory_shire_id;

      if (shire <= 33) begin
         phys_shire_id = get_minion_shire_bridge_id(shire);
      end
      else begin
         phys_shire_id = shire;
      end

      if ((master_type == UCTOL3_M) || (master_type == UCTOSYS_M)) begin
         sib_bit = 1'b1;
      end
      else begin
         sib_bit = 1'b0;
      end

      if ((phys_shire_id == 253) || (phys_shire_id == 254)) begin
         case (master_type)
            IO_PM          : master_id = `IOSHIRE_PM_MASTER_ID;
            IO_TOL3_M      : master_id = (noc_layer == 1) ? `IOSHIRE_TOL3B_MASTER_ID : `IOSHIRE_TOL3_MASTER_ID;
            IO_TOSYS_M     : master_id = `IOSHIRE_TOSYS_MASTER_ID;
            PSHIRE_TOL3_M  : master_id = `PSHIRE_TOL3_MASTER_ID;
            PSHIRE_TOSYS_M : master_id = `PSHIRE_TOSYS_MASTER_ID;
         endcase
      end
      else if ((phys_shire_id >= 232) && (phys_shire_id <= 239)) begin
         memory_shire_id = get_memory_shire_from_virt_id(phys_shire_id);
         master_id = memshire_master_ids[memory_shire_id];
      end
      else begin
         case (master_type)
            L2TOL3_M             : master_id = l2tol3_master_ids[noc_layer][phys_shire_id];
            UCTOL3_M             : master_id = tol3_sib_master_ids[phys_shire_id];
            L3TOSYS_M, UCTOSYS_M : master_id = tosys_sib_master_ids[phys_shire_id];
         endcase
      end

      return evl_bus_req_id_t'({master_id, sib_bit, source_req_id[8:0]});

   endfunction : predict_req_id

   //-------------------------------------------------------------------------
   //
   // Function: filter_writes
   //
   virtual function int filter_writes(input evl_req_desc req_desc_queue[$], output evl_req_desc write_descs[$], input evl_bus_qos_t filter_qos, input int is_esr_addr = -1);
      int count;
      evl_paddr_t req_desc_addr;

      count = 0;
      if (is_esr_addr == 1) begin
         for (int ii = 0; ii < req_desc_queue.size(); ii++) begin
            req_desc_addr = req_desc_queue[ii].get_paddr();
            if ((req_desc_queue[ii].bus_cmd_is_write() == 1'b1) && (req_desc_queue[ii].get_bus_qos() == filter_qos) && (req_desc_addr[39:32] == 8'h01)) begin
               write_descs.push_back(req_desc_queue[ii]);
               count++;
            end
         end
      end
      else if (is_esr_addr == 0) begin
         for (int ii = 0; ii < req_desc_queue.size(); ii++) begin
            req_desc_addr = req_desc_queue[ii].get_paddr();
            if ((req_desc_queue[ii].bus_cmd_is_write() == 1'b1) && (req_desc_queue[ii].get_bus_qos() == filter_qos) && ((req_desc_addr[39] == 1'b1) || (req_desc_addr[39:31] == 9'h01))) begin
               write_descs.push_back(req_desc_queue[ii]);
               count++;
            end
         end
      end
      else begin
         for (int ii = 0; ii < req_desc_queue.size(); ii++) begin
            if ((req_desc_queue[ii].bus_cmd_is_write() == 1'b1) && (req_desc_queue[ii].get_bus_qos() == filter_qos)) begin
               write_descs.push_back(req_desc_queue[ii]);
               count++;
            end
         end
      end
      return count;
   endfunction : filter_writes

   //-------------------------------------------------------------------------
   //
   // Function: filter_reads
   //
   virtual function int filter_reads(input evl_req_desc req_desc_queue[$], output evl_req_desc read_descs[$], input evl_bus_qos_t filter_qos, input int is_esr_addr = -1);
      int count;
      evl_paddr_t req_desc_addr;

      count = 0;
      if (is_esr_addr == 1) begin
         for (int ii = 0; ii < req_desc_queue.size(); ii++) begin
            req_desc_addr = req_desc_queue[ii].get_paddr();
            if ((req_desc_queue[ii].bus_cmd_is_read() == 1'b1) && (req_desc_queue[ii].get_bus_qos() == filter_qos) && (req_desc_addr[39:32] == 8'h01)) begin
               read_descs.push_back(req_desc_queue[ii]);
               count++;
            end
         end
      end
      else if (is_esr_addr == 0) begin
         for (int ii = 0; ii < req_desc_queue.size(); ii++) begin
            req_desc_addr = req_desc_queue[ii].get_paddr();
            if ((req_desc_queue[ii].bus_cmd_is_read() == 1'b1) && (req_desc_queue[ii].get_bus_qos() == filter_qos) && ((req_desc_addr[39] == 1'b1) || (req_desc_addr[39:31] == 9'h01))) begin
               read_descs.push_back(req_desc_queue[ii]);
               count++;
            end
         end
      end
      else begin
         for (int ii = 0; ii < req_desc_queue.size(); ii++) begin
            if ((req_desc_queue[ii].bus_cmd_is_read() == 1'b1) && (req_desc_queue[ii].get_bus_qos() == filter_qos)) begin
               read_descs.push_back(req_desc_queue[ii]);
               count++;
            end
         end
      end
      return count;
   endfunction : filter_reads

   //-----------------------------------------------------------------------------------------------
   //
   // Function: shire_exists
   //
   virtual function void shire_exists(input evl_shire_id_t shire);
      `evl_log(UVM_DEBUG, VMOD_NOC_SB, get_abstract_name(), $sformatf("Setting S%0d to enabled", shire))
      m_shires_in_build[shire] = 1'b1;
   endfunction : shire_exists

   //-----------------------------------------------------------------------------------------------
   //
   // Function: is_req_checked
   //
   virtual function bit is_req_checked(input evl_req_desc req_desc_in);
      evl_shire_id_t          src_shire;
      evl_noc_master_type_t   src_master;
      int                     src_noc_lyr;

      decode_master_id(req_desc_in.get_bus_req_id(), src_shire, src_master, src_noc_lyr);
      return m_shires_in_build[src_shire];
   endfunction : is_req_checked

   //-------------------------------------------------------------------------
   //
   // Function: change_shire_id
   //
   //virtual function void change_shire_id(input evl_shire_id_t reset_shire_id, input evl_shire_id_t new_shire_id);
   //   m_current_shire_ids[reset_shire_id] = new_shire_id;
   //endfunction : change_shire_id

   //-----------------------------------------------------------------------------------------------
   //
   // Function: post_shutdown_phase
   //
   virtual task post_shutdown_phase(uvm_phase phase);
      phase.raise_objection(this);
      fork
         begin
            super.post_shutdown_phase(phase);
         end
      join
      phase.drop_objection(this);
   endtask : post_shutdown_phase


   //----------------------------------------------------------------------------------------------
   //
   // Task: check_phase
   //
   virtual function void check_phase(uvm_phase phase);
      bit err_found;

      super.check_phase(phase);

      `evl_log(UVM_NONE, VMOD_NOC_SB, get_abstract_name(), "Checking for active Mesh transactions")
      foreach (m_active_l3_mesh_req_descs[ii]) begin
         err_found = 1'b0;
         foreach (m_active_l3_mesh_req_descs[ii][jj]) begin
            if (m_active_l3_mesh_req_descs[ii][jj].size() != 0) begin
               if (err_found == 1'b0) begin
	 `ifndef IOSHIRE_GATESIM
                  `dut_error(get_abstract_name(), $sformatf("S%0d L3 Master has active REQs unnacounted for:", ii))
	 `endif
                  err_found = 1'b1;
               end
               `evl_log(UVM_NONE, VMOD_NOC_SB, get_abstract_name(), $sformatf("  Sent to S%0d:", jj))
               foreach (m_active_l3_mesh_req_descs[ii][jj][kk]) begin
                  `evl_log(UVM_NONE, VMOD_NOC_SB, get_abstract_name(), $sformatf("     %0s", m_active_l3_mesh_req_descs[ii][jj][kk].sprint_obj()))
               end
            end
         end
      end
      foreach (m_active_l3b_mesh_req_descs[ii]) begin
         err_found = 1'b0;
         foreach (m_active_l3b_mesh_req_descs[ii][jj]) begin
            if (m_active_l3b_mesh_req_descs[ii][jj].size() != 0) begin
               if (err_found == 1'b0) begin
                  `dut_error(get_abstract_name(), $sformatf("S%0d L3_B Master has active REQs unnacounted for:", ii))
                  err_found = 1'b1;
               end
               `evl_log(UVM_NONE, VMOD_NOC_SB, get_abstract_name(), $sformatf("  Sent to S%0d:", jj))
               foreach (m_active_l3b_mesh_req_descs[ii][jj][kk]) begin
                  `evl_log_mdesc(UVM_NONE, VMOD_NOC_SB, get_abstract_name(), m_active_l3b_mesh_req_descs[ii][jj][kk], $sformatf("     %0s", m_active_l3b_mesh_req_descs[ii][jj][kk].sprint_obj()))
               end
            end
         end
      end
      foreach (m_active_l3c_mesh_req_descs[ii]) begin
         err_found = 1'b0;
         foreach (m_active_l3c_mesh_req_descs[ii][jj]) begin
            if (m_active_l3c_mesh_req_descs[ii][jj].size() != 0) begin
               if (err_found == 1'b0) begin
                  `dut_error(get_abstract_name(), $sformatf("S%0d L3_C Master has active REQs unnacounted for:", ii))
                  err_found = 1'b1;
               end
               `evl_log(UVM_NONE, VMOD_NOC_SB, get_abstract_name(), $sformatf("  Sent to S%0d:", jj))
               foreach (m_active_l3c_mesh_req_descs[ii][jj][kk]) begin
                  `evl_log_mdesc(UVM_NONE, VMOD_NOC_SB, get_abstract_name(), m_active_l3c_mesh_req_descs[ii][jj][kk], $sformatf("     %0s", m_active_l3c_mesh_req_descs[ii][jj][kk].sprint_obj()))
               end
            end
         end
      end
      foreach (m_active_l3d_mesh_req_descs[ii]) begin
         err_found = 1'b0;
         foreach (m_active_l3d_mesh_req_descs[ii][jj]) begin
            if (m_active_l3d_mesh_req_descs[ii][jj].size() != 0) begin
               if (err_found == 1'b0) begin
                  `dut_error(get_abstract_name(), $sformatf("S%0d L3_D Master has active REQs unnacounted for:", ii))
                  err_found = 1'b1;
               end
               `evl_log(UVM_NONE, VMOD_NOC_SB, get_abstract_name(), $sformatf("  Sent to S%0d:", jj))
               foreach (m_active_l3d_mesh_req_descs[ii][jj][kk]) begin
                  `evl_log_mdesc(UVM_NONE, VMOD_NOC_SB, get_abstract_name(), m_active_l3d_mesh_req_descs[ii][jj][kk], $sformatf("     %0s", m_active_l3d_mesh_req_descs[ii][jj][kk].sprint_obj()))
               end
            end
         end
      end
      foreach (m_active_uctol3_mesh_req_descs[ii]) begin
         err_found = 1'b0;
         foreach (m_active_uctol3_mesh_req_descs[ii][jj]) begin
            if (m_active_uctol3_mesh_req_descs[ii][jj].size() != 0) begin
               if (err_found == 1'b0) begin
                  `dut_error(get_abstract_name(), $sformatf("S%0d UCTOL3 Master has active REQs unnacounted for:", ii))
                  err_found = 1'b1;
               end
               `evl_log(UVM_NONE, VMOD_NOC_SB, get_abstract_name(), $sformatf("  Sent to S%0d:", jj))
               foreach (m_active_uctol3_mesh_req_descs[ii][jj][kk]) begin
                  `evl_log_mdesc(UVM_NONE, VMOD_NOC_SB, get_abstract_name(), m_active_uctol3_mesh_req_descs[ii][jj][kk], $sformatf("     %0s", m_active_uctol3_mesh_req_descs[ii][jj][kk].sprint_obj()))
               end
            end
         end
      end
      foreach (m_active_sys_mesh_req_descs[ii]) begin
         err_found = 1'b0;
         foreach (m_active_sys_mesh_req_descs[ii][jj]) begin
            if (m_active_sys_mesh_req_descs[ii][jj].size() != 0) begin
               if (err_found == 1'b0) begin
                  `dut_error(get_abstract_name(), $sformatf("S%0d TOSYS Master has active REQs unnacounted for:", ii))
                  err_found = 1'b1;
               end
               `evl_log(UVM_NONE, VMOD_NOC_SB, get_abstract_name(), $sformatf("  Sent to S%0d:", jj))
               foreach (m_active_sys_mesh_req_descs[ii][jj][kk]) begin
                  `evl_log_mdesc(UVM_NONE, VMOD_NOC_SB, get_abstract_name(), m_active_sys_mesh_req_descs[ii][jj][kk], $sformatf("     %0s", m_active_sys_mesh_req_descs[ii][jj][kk].sprint_obj()))
               end
            end
         end
      end
      foreach (m_active_uctosys_mesh_req_descs[ii]) begin
         err_found = 1'b0;
         foreach (m_active_uctosys_mesh_req_descs[ii][jj]) begin
            if (m_active_uctosys_mesh_req_descs[ii][jj].size() != 0) begin
               if (err_found == 1'b0) begin
                  `dut_error(get_abstract_name(), $sformatf("S%0d UCTOSYS Master has active REQs unnacounted for:", ii))
                  err_found = 1'b1;
               end
               `evl_log(UVM_NONE, VMOD_NOC_SB, get_abstract_name(), $sformatf("  Sent to S%0d:", jj))
               foreach (m_active_uctosys_mesh_req_descs[ii][jj][kk]) begin
                  `evl_log_mdesc(UVM_NONE, VMOD_NOC_SB, get_abstract_name(), m_active_uctosys_mesh_req_descs[ii][jj][kk], $sformatf("     %0s", m_active_uctosys_mesh_req_descs[ii][jj][kk].sprint_obj()))
               end
            end
         end
      end
      foreach (m_active_mctosys_mesh_req_descs[ii]) begin
         err_found = 1'b0;
         foreach (m_active_mctosys_mesh_req_descs[ii][jj]) begin
            if (m_active_mctosys_mesh_req_descs[ii][jj].size() != 0) begin
               if (err_found == 1'b0) begin
                  `dut_error(get_abstract_name(), $sformatf("S%0d MCTOSYS Master has active REQs unnacounted for:", ii))
                  err_found = 1'b1;
               end
               `evl_log(UVM_NONE, VMOD_NOC_SB, get_abstract_name(), $sformatf("  Sent to S%0d:", jj))
               foreach (m_active_mctosys_mesh_req_descs[ii][jj][kk]) begin
                  `evl_log_mdesc(UVM_NONE, VMOD_NOC_SB, get_abstract_name(), m_active_mctosys_mesh_req_descs[ii][jj][kk], $sformatf("     %0s", m_active_mctosys_mesh_req_descs[ii][jj][kk].sprint_obj()))
               end
            end
         end
      end
      foreach (m_active_io_pm_mesh_req_descs[ii]) begin
         err_found = 1'b0;
         foreach (m_active_io_pm_mesh_req_descs[ii][jj]) begin
            if (m_active_io_pm_mesh_req_descs[ii][jj].size() != 0) begin
               if (err_found == 1'b0) begin
                  `dut_error(get_abstract_name(), $sformatf("IOShire PM Master has active REQs unnacounted for:"))
                  err_found = 1'b1;
               end
               `evl_log(UVM_NONE, VMOD_NOC_SB, get_abstract_name(), $sformatf("  Sent to S%0d:", jj))
               foreach (m_active_io_pm_mesh_req_descs[ii][jj][kk]) begin
                  `evl_log_mdesc(UVM_NONE, VMOD_NOC_SB, get_abstract_name(), m_active_io_pm_mesh_req_descs[ii][jj][kk], $sformatf("     %0s", m_active_io_pm_mesh_req_descs[ii][jj][kk].sprint_obj()))
               end
            end
         end
      end
      foreach (m_active_l3_mesh_rsp_descs[ii]) begin
         err_found = 1'b0;
         foreach (m_active_l3_mesh_rsp_descs[ii][jj]) begin
            if (m_active_l3_mesh_rsp_descs[ii][jj].size() != 0) begin
               if (err_found == 1'b0) begin
                  `dut_error(get_abstract_name(), $sformatf("S%0d L3 Master has active RSPs unnacounted for:", ii))
                  err_found = 1'b1;
               end
               `evl_log(UVM_NONE, VMOD_NOC_SB, get_abstract_name(), $sformatf("  Sent from S%0d:", jj))
               foreach (m_active_l3_mesh_rsp_descs[ii][jj][kk]) begin
                  `evl_log_mdesc(UVM_NONE, VMOD_NOC_SB, get_abstract_name(), m_active_l3_mesh_rsp_descs[ii][jj][kk], $sformatf("     %0s", m_active_l3_mesh_rsp_descs[ii][jj][kk].sprint_obj()))
               end
            end
         end
      end
      foreach (m_active_l3b_mesh_rsp_descs[ii]) begin
         err_found = 1'b0;
         foreach (m_active_l3b_mesh_rsp_descs[ii][jj]) begin
            if (m_active_l3b_mesh_rsp_descs[ii][jj].size() != 0) begin
               if (err_found == 1'b0) begin
                  `dut_error(get_abstract_name(), $sformatf("S%0d L3_B Master has active RSPs unnacounted for:", ii))
                  err_found = 1'b1;
               end
               `evl_log(UVM_NONE, VMOD_NOC_SB, get_abstract_name(), $sformatf("  Sent from S%0d:", jj))
               foreach (m_active_l3b_mesh_rsp_descs[ii][jj][kk]) begin
                  `evl_log_mdesc(UVM_NONE, VMOD_NOC_SB, get_abstract_name(), m_active_l3b_mesh_rsp_descs[ii][jj][kk], $sformatf("     %0s", m_active_l3b_mesh_rsp_descs[ii][jj][kk].sprint_obj()))
               end
            end
         end
      end
      foreach (m_active_l3c_mesh_rsp_descs[ii]) begin
         err_found = 1'b0;
         foreach (m_active_l3c_mesh_rsp_descs[ii][jj]) begin
            if (m_active_l3c_mesh_rsp_descs[ii][jj].size() != 0) begin
               if (err_found == 1'b0) begin
                  `dut_error(get_abstract_name(), $sformatf("S%0d L3_C Master has active RSPs unnacounted for:", ii))
                  err_found = 1'b1;
               end
               `evl_log(UVM_NONE, VMOD_NOC_SB, get_abstract_name(), $sformatf("  Sent from S%0d:", jj))
               foreach (m_active_l3c_mesh_rsp_descs[ii][jj][kk]) begin
                  `evl_log_mdesc(UVM_NONE, VMOD_NOC_SB, get_abstract_name(), m_active_l3c_mesh_rsp_descs[ii][jj][kk], $sformatf("     %0s", m_active_l3c_mesh_rsp_descs[ii][jj][kk].sprint_obj()))
               end
            end
         end
      end
      foreach (m_active_l3d_mesh_rsp_descs[ii]) begin
         err_found = 1'b0;
         foreach (m_active_l3d_mesh_rsp_descs[ii][jj]) begin
            if (m_active_l3d_mesh_rsp_descs[ii][jj].size() != 0) begin
               if (err_found == 1'b0) begin
                  `dut_error(get_abstract_name(), $sformatf("S%0d L3_D Master has active RSPs unnacounted for:", ii))
                  err_found = 1'b1;
               end
               `evl_log(UVM_NONE, VMOD_NOC_SB, get_abstract_name(), $sformatf("  Sent from S%0d:", jj))
               foreach (m_active_l3d_mesh_rsp_descs[ii][jj][kk]) begin
                  `evl_log_mdesc(UVM_NONE, VMOD_NOC_SB, get_abstract_name(), m_active_l3d_mesh_rsp_descs[ii][jj][kk], $sformatf("     %0s", m_active_l3d_mesh_rsp_descs[ii][jj][kk].sprint_obj()))
               end
            end
         end
      end
      foreach (m_active_uctol3_mesh_rsp_descs[ii]) begin
         err_found = 1'b0;
         foreach (m_active_uctol3_mesh_rsp_descs[ii][jj]) begin
            if (m_active_uctol3_mesh_rsp_descs[ii][jj].size() != 0) begin
               if (err_found == 1'b0) begin
                  `dut_error(get_abstract_name(), $sformatf("S%0d UCTOL3 Master has active RSPs unnacounted for:", ii))
                  err_found = 1'b1;
               end
               `evl_log(UVM_NONE, VMOD_NOC_SB, get_abstract_name(), $sformatf("  Sent from S%0d:", jj))
               foreach (m_active_uctol3_mesh_rsp_descs[ii][jj][kk]) begin
                  `evl_log_mdesc(UVM_NONE, VMOD_NOC_SB, get_abstract_name(), m_active_uctol3_mesh_rsp_descs[ii][jj][kk], $sformatf("     %0s", m_active_uctol3_mesh_rsp_descs[ii][jj][kk].sprint_obj()))
               end
            end
         end
      end
      foreach (m_active_sys_mesh_rsp_descs[ii]) begin
         err_found = 1'b0;
         foreach (m_active_sys_mesh_rsp_descs[ii][jj]) begin
            if (m_active_sys_mesh_rsp_descs[ii][jj].size() != 0) begin
               if (err_found == 1'b0) begin
                  `dut_error(get_abstract_name(), $sformatf("S%0d TOSYS Master has active RSPs unnacounted for:", ii))
                  err_found = 1'b1;
               end
               `evl_log(UVM_NONE, VMOD_NOC_SB, get_abstract_name(), $sformatf("  Sent from S%0d:", jj))
               foreach (m_active_sys_mesh_rsp_descs[ii][jj][kk]) begin
                  `evl_log_mdesc(UVM_NONE, VMOD_NOC_SB, get_abstract_name(), m_active_sys_mesh_rsp_descs[ii][jj][kk], $sformatf("     %0s", m_active_sys_mesh_rsp_descs[ii][jj][kk].sprint_obj()))
               end
            end
         end
      end
      foreach (m_active_uctosys_mesh_rsp_descs[ii]) begin
         err_found = 1'b0;
         foreach (m_active_uctosys_mesh_rsp_descs[ii][jj]) begin
            if (m_active_uctosys_mesh_rsp_descs[ii][jj].size() != 0) begin
               if (err_found == 1'b0) begin
                  `dut_error(get_abstract_name(), $sformatf("S%0d UCTOSYS Master has active RSPs unnacounted for:", ii))
                  err_found = 1'b1;
               end
               `evl_log(UVM_NONE, VMOD_NOC_SB, get_abstract_name(), $sformatf("  Sent from S%0d:", jj))
               foreach (m_active_uctosys_mesh_rsp_descs[ii][jj][kk]) begin
                  `evl_log_mdesc(UVM_NONE, VMOD_NOC_SB, get_abstract_name(), m_active_uctosys_mesh_rsp_descs[ii][jj][kk], $sformatf("     %0s", m_active_uctosys_mesh_rsp_descs[ii][jj][kk].sprint_obj()))
               end
            end
         end
      end
      foreach (m_active_mctosys_mesh_rsp_descs[ii]) begin
         err_found = 1'b0;
         foreach (m_active_mctosys_mesh_rsp_descs[ii][jj]) begin
            if (m_active_mctosys_mesh_rsp_descs[ii][jj].size() != 0) begin
               if (err_found == 1'b0) begin
                  `dut_error(get_abstract_name(), $sformatf("S%0d MCTOSYS Master has active RSPs unnacounted for:", ii))
                  err_found = 1'b1;
               end
               `evl_log(UVM_NONE, VMOD_NOC_SB, get_abstract_name(), $sformatf("  Sent from S%0d:", jj))
               foreach (m_active_mctosys_mesh_rsp_descs[ii][jj][kk]) begin
                  `evl_log_mdesc(UVM_NONE, VMOD_NOC_SB, get_abstract_name(), m_active_mctosys_mesh_rsp_descs[ii][jj][kk], $sformatf("     %0s", m_active_mctosys_mesh_rsp_descs[ii][jj][kk].sprint_obj()))
               end
            end
         end
      end
      foreach (m_active_io_pm_mesh_rsp_descs[ii]) begin
         err_found = 1'b0;
         foreach (m_active_io_pm_mesh_rsp_descs[ii][jj]) begin
            if (m_active_io_pm_mesh_rsp_descs[ii][jj].size() != 0) begin
               if (err_found == 1'b0) begin
                  `dut_error(get_abstract_name(), $sformatf("IOShire PM Master has active RSPs unnacounted for:"))
                  err_found = 1'b1;
               end
               `evl_log(UVM_NONE, VMOD_NOC_SB, get_abstract_name(), $sformatf("  Sent from S%0d:", jj))
               foreach (m_active_io_pm_mesh_rsp_descs[ii][jj][kk]) begin
                  `evl_log_mdesc(UVM_NONE, VMOD_NOC_SB, get_abstract_name(), m_active_io_pm_mesh_rsp_descs[ii][jj][kk], $sformatf("     %0s", m_active_io_pm_mesh_rsp_descs[ii][jj][kk].sprint_obj()))
               end
            end
         end
      end

   endfunction : check_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string name_in = "evl_noc_scoreboard", input uvm_component parent_in = null);
      super.new(name_in, parent_in);
      set_abstract_name("NOC_MESH(SB)");

      if (`et_test_plusargs("disableNOCChecks") != 0) begin
         `evl_set_error_intensity(pk_noc_sb_error_intensity, UVM_HIGH)
      end
      else begin
         `evl_set_error_intensity(pk_noc_sb_error_intensity, -1)
      end
      if (`et_test_plusargs("disableMaxionScpErr") != 0) begin
         m_min2maxscp_err = 0;
      end
      else begin
         m_min2maxscp_err = 1;
      end

   endfunction : new

endclass : evl_noc_scoreboard

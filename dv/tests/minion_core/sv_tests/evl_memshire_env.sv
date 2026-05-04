//--------------------------------------------------------------------------------------------------
//
// Class: evl_memshire_env
//
class evl_memshire_env extends evl_verif_comp;

   `uvm_component_utils(evl_memshire_env)

   //-----------------------------------------------------------------------------------------------
   //
   // Debug NoC Bus Types
   //
   // The following enumerated types are used to identify Debug NoC bus agents in an associative array.
   // These bus agents can then be connected to master or slave stimulus agents as necessary during
   // the connect phase.
   //
   //typedef enum int {
   //      MEM_DBG_DN_EVT,
   //      MEM_DBG_DN_MSG,
   //      MEM_DBG_LF_EVT,
   //      MEM_DBG_LF_MSG
   //   } memshire_dbg_noc_bus_t;

   //-----------------------------------------------------------------------------------------------
   //
   // Static Member Variables and Objects
   //
   `evl_param_key(pk_atomic_slave_count)
   `evl_param_key(pk_dbgnoc_stim)
   `evl_param_key(pk_debug_no_inst)
   `evl_param_key(pk_memshire_use_pll_beh)
   `evl_param_key(pk_mesh_mem_stim_count)
   `evl_param_key(pk_mesh_sbm_stim_count)
   `evl_param_key(pk_noc_stub)
   `evl_param_key(pk_shire_stub)
   `evl_param_key(pk_ultrasoc_en)
   `evl_param_key(pk_upstream_req_port_count)
   `evl_param_key(pk_ust_mem_stim_count)


   //-----------------------------------------------------------------------------------------------
   //
   // Member Variables and Objects
   //
   evl_verif_comp                        m_shire_bus_agents[evl_agent_id_t];
   evl_verif_comp                        m_shire_stim_agents[evl_agent_id_t];
   evl_verif_comp                        m_dbgnoc_bus_agents[evl_agent_id_t];
   evl_verif_comp                        m_dbgnoc_stim_agents[evl_agent_id_t];
   uvm_analysis_export   #(evl_req_desc) m_mesh_to_mem_req_export;
   uvm_tlm_analysis_fifo #(evl_req_desc) m_mesh_to_mem_req_fifo;
   uvm_analysis_export   #(evl_req_desc) m_ust_to_mem_req_export;
   uvm_tlm_analysis_fifo #(evl_req_desc) m_ust_to_mem_req_fifo;
   uvm_analysis_export   #(evl_req_desc) m_mesh_to_sbm_req_export;
   uvm_tlm_analysis_fifo #(evl_req_desc) m_mesh_to_sbm_req_fifo;
   uvm_analysis_export   #(evl_req_desc) m_ddr_rsp_export[int];
   uvm_tlm_analysis_fifo #(evl_req_desc) m_ddr_rsp_fifo[int];
   uvm_analysis_export   #(evl_req_desc) m_apb_req_export;
   uvm_tlm_analysis_fifo #(evl_req_desc) m_apb_req_fifo;
   uvm_analysis_export   #(evl_req_desc) m_apb_rsp_export;
   uvm_tlm_analysis_fifo #(evl_req_desc) m_apb_rsp_fifo;
   evl_memshire_scoreboard               m_scoreboard;
   evl_memshire_rtl_port                 m_memshire_rtl_port;
   int                                   m_shire_stub;
   int                                   m_noc_stub;
   int                                   m_debug_no_inst;
   evl_verif_comp                        m_reg_bus_agent;
   evl_verif_comp                        m_reg_stim_agent;
   evl_reg_adapter                       m_reg_adapter;
   uvm_reg_predictor#(evl_req_desc)      m_reg_predictor;
   evl_memshire_reg_block                m_memshire_regs;
   bit                                   m_disable_ms_sb_esr;


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_req_desc_export
   //
   virtual function uvm_analysis_export#(evl_req_desc) get_req_desc_export(input string port_name      = "unknown",
                                                                           input int    port_number    = 0,
                                                                           input int    subport_number = 0);
      if ((port_name == "m_ddr_rsp_export") && m_ddr_rsp_export.exists(port_number)) begin
         return m_ddr_rsp_export[port_number];
      end
      if ((port_name == "m_ddr_rsp_export_lo_even") && m_ddr_rsp_export.exists(`EVL_DDR_LO_EVEN_PORT)) begin
         return m_ddr_rsp_export[`EVL_DDR_LO_EVEN_PORT];
      end
      if ((port_name == "m_ddr_rsp_export_lo_odd") && m_ddr_rsp_export.exists(`EVL_DDR_LO_ODD_PORT)) begin
         return m_ddr_rsp_export[`EVL_DDR_LO_ODD_PORT];
      end
      if ((port_name == "m_ddr_rsp_export_hi_even") && m_ddr_rsp_export.exists(`EVL_DDR_HI_EVEN_PORT)) begin
         return m_ddr_rsp_export[`EVL_DDR_HI_EVEN_PORT];
      end
      if ((port_name == "m_ddr_rsp_export_hi_odd") && m_ddr_rsp_export.exists(`EVL_DDR_HI_ODD_PORT)) begin
         return m_ddr_rsp_export[`EVL_DDR_HI_ODD_PORT];
      end
      return null;
   endfunction : get_req_desc_export


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_req_desc_port
   //
   virtual function uvm_analysis_port#(evl_req_desc) get_req_desc_port(input string port_name      = "unknown",
                                                                       input int    port_number    = 0,
                                                                       input int    subport_number = 0);
      if ((port_name == "m_dbgnoc_tx_req_port") || (port_name == "m_dbgnoc_rx_req_port")) begin
         if (!m_dbgnoc_bus_agents.exists(port_number)) begin
            return null;
         end
         if (port_name == "m_dbgnoc_tx_req_port") begin
            return m_dbgnoc_bus_agents[port_number].get_req_desc_port("m_obs_tx_req_port");
         end
         if (port_name == "m_dbgnoc_rx_req_port") begin
            return m_dbgnoc_bus_agents[port_number].get_req_desc_port("m_obs_rx_req_port");
         end
      end
      else begin
         if ((port_name == "m_axi_req_port") && m_shire_bus_agents.exists(port_number)) begin
            return m_shire_bus_agents[port_number].get_req_desc_port("m_obs_req_port");
         end
         if ((port_name == "m_axi_rsp_port") && m_shire_bus_agents.exists(port_number)) begin
            return m_shire_bus_agents[port_number].get_req_desc_port("m_obs_rsp_port");
         end
      end
      return null;
   endfunction : get_req_desc_port


   //-----------------------------------------------------------------------------------------------
   //
   // Task: watch_for_incoming_reqs_task
   //
   task watch_for_incoming_reqs_task(input uvm_tlm_analysis_fifo #(evl_req_desc) req_fifo, input evl_bus_req_id_t port_req_id, input bit use_qos, input bit use_sb);
      if (req_fifo == null) begin
         return;
      end
      forever begin
         evl_req_desc req_desc;

         req_fifo.get(req_desc);
         if (use_sb == 1'b1) begin
            m_scoreboard.process_bus_request(req_desc, port_req_id, use_qos);
         end
      end
   endtask : watch_for_incoming_reqs_task


   //-----------------------------------------------------------------------------------------------
   //
   // Function: watch_for_incoming_reqs
   //
   function void watch_for_incoming_reqs(input uvm_tlm_analysis_fifo #(evl_req_desc) req_fifo, input evl_bus_req_id_t port_req_id, input bit use_qos, input bit use_sb);
      if (req_fifo == null) begin
         return;
      end
      fork
         watch_for_incoming_reqs_task(req_fifo, port_req_id, use_qos, use_sb);
      join_none
   endfunction : watch_for_incoming_reqs


   //-----------------------------------------------------------------------------------------------
   //
   // Task: watch_for_register_setup_task
   //
   task watch_for_register_setup_task(input uvm_tlm_analysis_fifo #(evl_req_desc) req_fifo);
      if (req_fifo == null) begin
         return;
      end
      forever begin
         evl_req_desc req_desc;

         req_fifo.get(req_desc);

         m_scoreboard.process_register_setup(req_desc);
      end
   endtask : watch_for_register_setup_task


   //-----------------------------------------------------------------------------------------------
   //
   // Function: watch_for_register_setup
   //
   function void watch_for_register_setup(input uvm_tlm_analysis_fifo #(evl_req_desc) req_fifo);
      if (req_fifo == null) begin
         return;
      end
      fork
         watch_for_register_setup_task(req_fifo);
      join_none
   endfunction : watch_for_register_setup


   //-----------------------------------------------------------------------------------------------
   //
   // Task: watch_for_register_access_task
   //
   task watch_for_register_access_task(input uvm_tlm_analysis_fifo #(evl_req_desc) req_fifo);
      if (req_fifo == null) begin
         return;
      end
      forever begin
         evl_req_desc req_desc;

         req_fifo.get(req_desc);

         m_scoreboard.process_register_access(req_desc);
      end
   endtask : watch_for_register_access_task


   //-----------------------------------------------------------------------------------------------
   //
   // Function: watch_for_register_access
   //
   function void watch_for_register_access(input uvm_tlm_analysis_fifo #(evl_req_desc) req_fifo);
      if (req_fifo == null) begin
         return;
      end
      fork
         watch_for_register_access_task(req_fifo);
      join_none
   endfunction : watch_for_register_access


   //-----------------------------------------------------------------------------------------------
   //
   // Task: watch_for_ddr_rsps_task
   //
   task watch_for_ddr_rsps_task(input int port_number);
      if (!(m_ddr_rsp_fifo.exists(port_number))) begin
         return;
      end
      forever begin
         evl_req_desc req_desc;

         m_ddr_rsp_fifo[port_number].get(req_desc);
         m_scoreboard.process_bus_response(req_desc, port_number);
      end
   endtask : watch_for_ddr_rsps_task


   //-----------------------------------------------------------------------------------------------
   //
   // Function: watch_for_ddr_rsps
   //
   function void watch_for_ddr_rsps(input int port_number);
      if (!(m_ddr_rsp_fifo.exists(port_number))) begin
         return;
      end
      fork
         watch_for_ddr_rsps_task(port_number);
      join_none
   endfunction : watch_for_ddr_rsps


   //-----------------------------------------------------------------------------------------------
   //
   // Task: watch_for_interrupts_task
   //
   task watch_for_interrupts_task(input string param_name_in, input int interrupt_val = 1);
      string param_name;
      string param_name_ignore;

      param_name        = $sformatf("pk_%0s_mon", param_name_in);
      param_name_ignore = $sformatf("pk_%0s_ignore", param_name_in);

      forever begin
         m_memshire_rtl_port.wait_for_int_param_by_name(param_name);

         if (m_memshire_rtl_port.get_int_param_by_name(param_name_ignore, -1) != 1) begin
            if (m_memshire_rtl_port.get_int_param_by_name(param_name, -1) == interrupt_val) begin
               `dut_error(get_abstract_name(), $sformatf("Unexpected intterupt on signal %0s!", param_name_in))
            end
         end
      end
   endtask : watch_for_interrupts_task


   //-----------------------------------------------------------------------------------------------
   //
   // Function: watch_for_interrupts
   //
   function void watch_for_interrupts();
      fork
         watch_for_interrupts_task("u0_umctl2_ecc_uncorrected_err_intr_vmc");
         watch_for_interrupts_task("u0_umctl2_dfi_alert_err_intr_vmc");
         watch_for_interrupts_task("u0_umctl2_ecc_corrected_err_intr_vmc");
         watch_for_interrupts_task("u0_umctl2_sbr_done_intr_vmc");
         watch_for_interrupts_task("u0_umctl2_derate_temp_limit_intr_vmc");
         watch_for_interrupts_task("dfi0_error_vmc");
         watch_for_interrupts_task("u1_umctl2_ecc_uncorrected_err_intr_vmc");
         watch_for_interrupts_task("u1_umctl2_dfi_alert_err_intr_vmc");
         watch_for_interrupts_task("u1_umctl2_ecc_corrected_err_intr_vmc");
         watch_for_interrupts_task("u1_umctl2_sbr_done_intr_vmc");
         watch_for_interrupts_task("u1_umctl2_derate_temp_limit_intr_vmc");
         watch_for_interrupts_task("dfi1_error_vmc");
         watch_for_interrupts_task("dwc_ddrphy_int_n_vmc", 0);   // active-low
         watch_for_interrupts_task("p0_cntr_overflow_intr");
         watch_for_interrupts_task("p1_cntr_overflow_intr");
         watch_for_interrupts_task("noc_err_int_or_hv");
         watch_for_interrupts_task("memshire_critical_int_gv");
         watch_for_interrupts_task("memshire_normal_int_gv");
      join_none
   endfunction : watch_for_interrupts


   //-----------------------------------------------------------------------------------------------
   //
   // Function: process_rtl_events
   //
   task process_rtl_events();
      evl_memshire_packet rtl_event;

      if (m_memshire_rtl_port == null) begin
         return;
      end
      forever begin
         m_memshire_rtl_port.get_obs_packet(rtl_event);
         m_scoreboard.process_rtl_event(rtl_event);
      end
   endtask : process_rtl_events


   //-----------------------------------------------------------------------------------------------
   //
   // Function: find_observed_request
   //
   virtual function evl_req_desc find_observed_request(input evl_agent_type_t     port_type_in,
                                                       input int                  port_in,
                                                       input int                  subport_in,
                                                       input evl_req_desc         paired_req_desc,
                                                       input evl_bus_cmd_t        bus_cmd_in   = `EVL_Bus_Idle,
                                                       input evl_paddr_t          address_in   = ~evl_paddr_t'(0),
                                                       input evl_bus_req_id_t     req_id_in    = ~evl_bus_req_id_t'(0),
                                                       input evl_bus_req_source_t source_id_in = ~evl_bus_req_source_t'(0),
                                                       input evl_trans_id_t       trans_id_in  = ~evl_trans_id_t'(0));
      if ((paired_req_desc == null) || (m_scoreboard == null)) begin
         return null;
      end
      return m_scoreboard.find_observed_request(port_type_in, port_in, subport_in, paired_req_desc, bus_cmd_in, address_in, req_id_in, source_id_in, trans_id_in);
   endfunction : find_observed_request


   //-----------------------------------------------------------------------------------------------
   //
   // Function: build_phase
   //
   virtual function void build_phase(uvm_phase phase);
      uvm_factory            factory;
      uvm_component          component;
      evl_verif_comp         verif_comp;
      evl_rtl_instance       instance_list[];
      evl_verif_comp         parent_comp;
      evl_verif_comp         parent_comps[string];
      int                    gen_mesh_mem_stim_agent;
      int                    gen_mesh_sbm_stim_agent;
      int                    gen_ust_mem_stim_agent;
      int                    gen_atomic_slave_agent;
      int                    gen_reg_slave_agent;
      evl_shire_id_t         shire_id;
      int                    shire_offset;
      int                    axi_bus_agent_id;
      int                    dbgnoc_comp_id;
      int                    m_dbgnoc_stim;
      evl_memshire_reg_model reg_model;

      super.build_phase(phase);

      if (m_rtl_port != null) begin
         m_shire_stub    = m_rtl_port.get_int_param(pk_shire_stub, 0);
         m_noc_stub      = m_rtl_port.get_int_param(pk_noc_stub, 0);
         m_debug_no_inst = m_rtl_port.get_int_param(pk_debug_no_inst, 0);
         m_dbgnoc_stim   = m_rtl_port.get_int_param(pk_dbgnoc_stim, 0);
      end
      else begin
         `dut_error(get_abstract_name(), $sformatf("%0s: m_rtl_port is null, SAD!", get_abstract_name()))
         return;
      end

      $cast(m_memshire_rtl_port, m_rtl_port);

      if (m_noc_stub == 1) begin
         set_int_param(pk_mesh_mem_stim_count, 1);
         set_int_param(pk_mesh_sbm_stim_count, 1);
         set_int_param(pk_ust_mem_stim_count, 1);
         set_int_param(pk_atomic_slave_count, 1);
      end

      factory          = uvm_factory::get();
      shire_id         = get_shire_id();
      shire_offset     = (m_shire_stub == 1) ? (shire_id*6) : 0;
      axi_bus_agent_id = 0;
      evl_rtl_instance_map::get_my_children_list(instance_list, get_full_name());
      foreach (instance_list[ii]) begin
         string inst_name;
         string parent_names[$];

         parent_comp = this;
         inst_name   = evl_base_pkg::extract_parent_list(parent_names, get_full_name(), instance_list[ii].get_full_name());
         foreach (parent_names[jj]) begin
            string full_parent_name;

            full_parent_name = $sformatf("%0s.%0s", parent_comp.get_full_name(), parent_names[jj]);
            if (!parent_comps.exists(full_parent_name)) begin
               parent_comp = evl_verif_comp::type_id::create(parent_names[jj], parent_comp);
               parent_comps[full_parent_name] = parent_comp;
            end
            else begin
               parent_comp = parent_comps[full_parent_name];
            end
         end
         component = factory.create_component_by_name(instance_list[ii].get_sv_comp(), "", inst_name, parent_comp);
         if (component != null) begin
            evl_agent_id_t agent_id;

            $cast(verif_comp, component);
            agent_id = instance_list[ii].get_agent_pid();
            verif_comp.set_shire_id(get_shire_id());
            gen_mesh_mem_stim_agent = -1;
            gen_mesh_sbm_stim_agent = -1;
            gen_ust_mem_stim_agent  = -1;
            gen_atomic_slave_agent  = -1;
            gen_reg_slave_agent     = -1;
            if (verif_comp.is_bus_vc() == 1'b1) begin
               if (verif_comp.get_agent_type() == AXI_BUS_AGENT) begin
                  verif_comp.set_agent_id(axi_bus_agent_id++);
                  if (instance_list[ii].get_inst_name() == "m_mesh_to_mem_axi") begin
                     verif_comp.set_port_id(shire_offset+(int'(MESH_TO_MEM_ID)));
                     verif_comp.set_port_type(evl_agent_id_t'(MESH_TO_MEM_ID));
                     verif_comp.set_abstract_name($sformatf("MESH_MEM(AXI)"));
                     m_shire_bus_agents[int'(MESH_TO_MEM_ID)] = verif_comp;
                     if ((get_int_param(pk_mesh_mem_stim_count) > 0) || (m_shire_stub == 1)) begin
                        gen_mesh_mem_stim_agent = int'(MESH_TO_MEM_ID);
                     end
                  end
                  else if (instance_list[ii].get_inst_name() == "m_mesh_to_mem_sbm_axi") begin
                     verif_comp.set_port_id(shire_offset+(int'(MESH_TO_SBM_ID)));
                     verif_comp.set_port_type(int'(MESH_TO_SBM_ID));
                     verif_comp.set_abstract_name($sformatf("MESH_SBM(AXI)"));
                     m_shire_bus_agents[int'(MESH_TO_SBM_ID)] = verif_comp;
                     if ((get_int_param(pk_mesh_sbm_stim_count) > 0) || (m_shire_stub == 1)) begin
                        gen_mesh_sbm_stim_agent = int'(MESH_TO_SBM_ID);
                     end
                  end
                  else if (instance_list[ii].get_inst_name() == "m_ust_smb_to_mem_axi") begin
                     verif_comp.set_port_id(shire_offset+(int'(UST_TO_MEM_ID)));
                     verif_comp.set_port_type(int'(UST_TO_MEM_ID));
                     verif_comp.set_abstract_name($sformatf("UST_MEM(AXI)"));
                     m_shire_bus_agents[int'(UST_TO_MEM_ID)] = verif_comp;
                     if ((get_int_param(pk_ust_mem_stim_count) > 0) || (m_shire_stub == 1)) begin
                        gen_ust_mem_stim_agent = int'(UST_TO_MEM_ID);
                     end
                  end
                  else if (instance_list[ii].get_inst_name() == "m_mem_atomic_to_mesh_axi") begin
                     verif_comp.set_port_id(shire_offset+(int'(ATOMIC_TO_MESH_ID)));
                     verif_comp.set_port_type(int'(ATOMIC_TO_MESH_ID));
                     verif_comp.set_abstract_name($sformatf("MEM_ATOMIC(AXI)"));
                     m_shire_bus_agents[int'(ATOMIC_TO_MESH_ID)] = verif_comp;
                     if ((get_int_param(pk_atomic_slave_count) > 0) || (m_shire_stub == 1)) begin
                        gen_atomic_slave_agent = int'(ATOMIC_TO_MESH_ID);
                     end
                  end
               end
               else if (verif_comp.get_agent_type() == APB_BUS_AGENT) begin
                  verif_comp.set_port_id(agent_id);
                  verif_comp.set_agent_id(evl_agent_id_t'(agent_id));
                  verif_comp.set_abstract_name($sformatf("MS_ESR(APB)"));
                  m_reg_bus_agent = verif_comp;
                  gen_reg_slave_agent = 1;
               end
               else if (verif_comp.get_agent_type() == DBG_NOC_BUS_AGENT) begin
                  verif_comp.set_agent_id(get_shire_id());
                  if (inst_name == "m_dbg_dn_evt") begin
                     dbgnoc_comp_id = int'(MEM_DBG_DN_EVT);
                     verif_comp.set_port_id(dbgnoc_comp_id);
                     verif_comp.set_agent_id(dbgnoc_comp_id);
                     verif_comp.set_abstract_name($sformatf("DN_EVT(DBGNOC)"));
                     m_dbgnoc_bus_agents[dbgnoc_comp_id] = verif_comp;
                  end
                  else if (inst_name == "m_dbg_dn_msg") begin
                     dbgnoc_comp_id = int'(MEM_DBG_DN_MSG);
                     verif_comp.set_port_id(dbgnoc_comp_id);
                     verif_comp.set_agent_id(dbgnoc_comp_id);
                     verif_comp.set_abstract_name($sformatf("DN_MSG(DBGNOC)"));
                     m_dbgnoc_bus_agents[dbgnoc_comp_id] = verif_comp;
                  end
                  else if (inst_name == "m_dbg_lf_evt") begin
                     dbgnoc_comp_id = int'(MEM_DBG_LF_EVT);
                     verif_comp.set_port_id(dbgnoc_comp_id);
                     verif_comp.set_agent_id(dbgnoc_comp_id);
                     verif_comp.set_abstract_name($sformatf("LF_EVT(DBGNOC)"));
                     m_dbgnoc_bus_agents[dbgnoc_comp_id] = verif_comp;
                  end
                  else if (inst_name == "m_dbg_lf_msg") begin
                     dbgnoc_comp_id = int'(MEM_DBG_LF_MSG);
                     verif_comp.set_port_id(dbgnoc_comp_id);
                     verif_comp.set_agent_id(dbgnoc_comp_id);
                     verif_comp.set_abstract_name($sformatf("LF_MSG(DBGNOC)"));
                     m_dbgnoc_bus_agents[dbgnoc_comp_id] = verif_comp;
                  end
               end

               add_vc_child(verif_comp);

               //
               // Create stimulus agents as necessary.
               //
               if (m_dbgnoc_stim == 1) begin
                  if (verif_comp.get_agent_type() == DBG_NOC_BUS_AGENT) begin
                     if (dbgnoc_comp_id == MEM_DBG_DN_EVT) begin
                        component = factory.create_component_by_name("evl_stim_agent_dbg_noc", "", $sformatf("io_dn_evt_%0d", dbgnoc_comp_id), this);
                        if (component != null) begin
                           verif_comp.set_active_master();
                           $cast(verif_comp, component);
                           m_dbgnoc_stim_agents[dbgnoc_comp_id] = verif_comp;
                           verif_comp.set_abstract_name($sformatf("DBGNOC_DN_EVT(STIM)"));
                           verif_comp.set_agent_id(dbgnoc_comp_id);
                           verif_comp.set_shire_id(get_shire_id());
                           verif_comp.set_rtl_port(m_dbgnoc_bus_agents[dbgnoc_comp_id].get_rtl_port());
                           add_vc_child(verif_comp);
                        end
                     end
                     else if (dbgnoc_comp_id == MEM_DBG_DN_MSG) begin
                        component = factory.create_component_by_name("evl_stim_agent_dbg_noc", "", $sformatf("io_dn_msg_%0d", dbgnoc_comp_id), this);
                        if (component != null) begin
                           verif_comp.set_active_master();
                           $cast(verif_comp, component);
                           m_dbgnoc_stim_agents[dbgnoc_comp_id] = verif_comp;
                           verif_comp.set_abstract_name($sformatf("DBGNOC_DN_MSG(STIM)"));
                           verif_comp.set_agent_id(dbgnoc_comp_id);
                           verif_comp.set_shire_id(get_shire_id());
                           verif_comp.set_rtl_port(m_dbgnoc_bus_agents[dbgnoc_comp_id].get_rtl_port());
                           add_vc_child(verif_comp);
                        end
                     end
                     else if (dbgnoc_comp_id == MEM_DBG_LF_EVT) begin
                        component = factory.create_component_by_name("evl_stim_agent_dbg_noc", "", $sformatf("io_lf_evt_%0d", dbgnoc_comp_id), this);
                        if (component != null) begin
                           verif_comp.set_active_master();
                           $cast(verif_comp, component);
                           m_dbgnoc_stim_agents[dbgnoc_comp_id] = verif_comp;
                           verif_comp.set_abstract_name($sformatf("DBGNOC_LF_EVT(STIM)"));
                           verif_comp.set_agent_id(dbgnoc_comp_id);
                           verif_comp.set_shire_id(get_shire_id());
                           verif_comp.set_rtl_port(m_dbgnoc_bus_agents[dbgnoc_comp_id].get_rtl_port());
                           add_vc_child(verif_comp);
                        end
                     end
                     // LF_MSG interface of Memshire is Rx only
                     //else if (dbgnoc_comp_id == MEM_DBG_LF_MSG) begin
                     //   component = factory.create_component_by_name("evl_stim_agent_dbg_noc", "", $sformatf("io_lf_msg_%0d", dbgnoc_comp_id), this);
                     //   if (component != null) begin
                     //      verif_comp.set_active_master();
                     //      $cast(verif_comp, component);
                     //      m_dbgnoc_stim_agents[dbgnoc_comp_id] = verif_comp;
                     //      verif_comp.set_abstract_name($sformatf("DBGNOC_LF_MSG(STIM)"));
                     //      verif_comp.set_agent_id(dbgnoc_comp_id);
                     //      verif_comp.set_shire_id(get_shire_id());
                     //      verif_comp.set_rtl_port(m_dbgnoc_bus_agents[dbgnoc_comp_id].get_rtl_port());
                     //      add_vc_child(verif_comp);
                     //   end
                     //end
                  end
               end
               if (m_shire_stub == 1) begin
                  if (gen_mesh_mem_stim_agent >= 0) begin
                     m_shire_bus_agents[gen_mesh_mem_stim_agent].set_active_slave();
                     component = factory.create_component_by_name("evl_shire_slave_agent_axi", "", $sformatf("mesh_mem_slave"), this);
                  end
                  else if (gen_mesh_sbm_stim_agent >= 0) begin
                     m_shire_bus_agents[gen_mesh_sbm_stim_agent].set_active_slave();
                     component = factory.create_component_by_name("evl_shire_slave_agent_axi", "", $sformatf("mesh_sbm_slave"), this);
                  end
                  else if (gen_ust_mem_stim_agent >= 0) begin
                     m_shire_bus_agents[gen_ust_mem_stim_agent].set_active_slave();
                     component = factory.create_component_by_name("evl_shire_slave_agent_axi", "", $sformatf("ust_mem_slave"), this);
                  end
                  else if (gen_atomic_slave_agent >= 0) begin
                     m_shire_bus_agents[gen_atomic_slave_agent].set_active_master();
                     component = factory.create_component_by_name("evl_atomic_rsp_master_agent_axi", "", $sformatf("atomic_master"), this);
                  end
                  else begin
                     component = null;
                  end
               end
               else begin
                  if (gen_mesh_mem_stim_agent >= 0) begin
                     m_shire_bus_agents[gen_mesh_mem_stim_agent].set_active_master();
                     component = factory.create_component_by_name("evl_mem_master_agent_axi", "", $sformatf("mesh_mem_master"), this);
                  end
                  else if (gen_mesh_sbm_stim_agent >= 0) begin
                     m_shire_bus_agents[gen_mesh_sbm_stim_agent].set_active_master();
                     component = factory.create_component_by_name("evl_sbm_master_agent_axi", "", $sformatf("mesh_sbm_master"), this);
                  end
                  else if (gen_ust_mem_stim_agent >= 0) begin
                     if (m_debug_no_inst != 0) begin
                        m_shire_bus_agents[gen_ust_mem_stim_agent].set_active_master();
                        component = factory.create_component_by_name("evl_ust_master_agent_axi", "", $sformatf("ust_mem_master"), this);
                     end
                     else begin
                        m_shire_bus_agents[gen_ust_mem_stim_agent].set_passive_slave();
                        component = factory.create_component_by_name("evl_ust_slave_agent_axi", "", $sformatf("ust_mem_passive"), this);
                     end
                  end
                  else if (gen_atomic_slave_agent >= 0) begin
                     m_shire_bus_agents[gen_atomic_slave_agent].set_active_slave();
                     component = factory.create_component_by_name("evl_mesh_slave_agent_axi", "", $sformatf("atomic_slave"), this);
                  end
                  else if (gen_reg_slave_agent >= 0) begin
                     m_reg_bus_agent.set_passive_slave();
                     component = factory.create_component_by_name("evl_reg_slave_agent_apb", "", $sformatf("reg_agent_%0d", agent_id), this);
                  end
                  else begin
                     component = null;
                  end
               end
               if (component != null) begin
                  $cast(verif_comp, component);
                  verif_comp.set_shire_id(get_shire_id());
                  if (gen_mesh_mem_stim_agent >= 0) begin
                     verif_comp.set_port_id(shire_offset+gen_mesh_mem_stim_agent);
                     verif_comp.set_abstract_name($sformatf("MESH_MEM"));
                     verif_comp.set_rtl_port(m_shire_bus_agents[gen_mesh_mem_stim_agent].get_rtl_port());
                     m_shire_stim_agents[gen_mesh_mem_stim_agent] = verif_comp;
                  end
                  else if (gen_mesh_sbm_stim_agent >= 0) begin
                     verif_comp.set_port_id(shire_offset+gen_mesh_sbm_stim_agent);
                     verif_comp.set_abstract_name($sformatf("MESH_SBM"));
                     verif_comp.set_rtl_port(m_shire_bus_agents[gen_mesh_sbm_stim_agent].get_rtl_port());
                     m_shire_stim_agents[gen_mesh_sbm_stim_agent] = verif_comp;
                  end
                  else if (gen_ust_mem_stim_agent >= 0) begin
                     verif_comp.set_port_id(shire_offset+gen_ust_mem_stim_agent);
                     verif_comp.set_abstract_name($sformatf("UST_MEM"));
                     verif_comp.set_rtl_port(m_shire_bus_agents[gen_ust_mem_stim_agent].get_rtl_port());
                     m_shire_stim_agents[gen_ust_mem_stim_agent] = verif_comp;
                  end
                  else if (gen_atomic_slave_agent >= 0) begin
                     verif_comp.set_port_id(shire_offset+gen_atomic_slave_agent);
                     verif_comp.set_abstract_name($sformatf("MEM_ATOMIC"));
                     verif_comp.set_rtl_port(m_shire_bus_agents[gen_atomic_slave_agent].get_rtl_port());
                     m_shire_stim_agents[gen_atomic_slave_agent] = verif_comp;
                  end
                  else if (gen_reg_slave_agent >= 0) begin
                     verif_comp.set_port_id(agent_id);
                     verif_comp.set_abstract_name($sformatf("MS_ESR"));
                     verif_comp.set_rtl_port(m_reg_bus_agent.get_rtl_port());
                     m_reg_stim_agent = verif_comp;
                  end
                  add_vc_child(verif_comp);
               end
            end
         end
      end
      if (m_shire_stub == 0) begin
         m_mesh_to_mem_req_export = new("m_mesh_to_mem_req_export", this);
         m_mesh_to_mem_req_fifo   = new("m_mesh_to_mem_req_fifo", this);
         m_ust_to_mem_req_export  = new("m_ust_to_mem_req_export", this);
         m_ust_to_mem_req_fifo    = new("m_ust_to_mem_req_fifo", this);
         m_mesh_to_sbm_req_export = new("m_mesh_to_sbm_req_export", this);
         m_mesh_to_sbm_req_fifo   = new("m_mesh_to_sbm_req_fifo", this);
         m_apb_req_export         = new("m_apb_req_export", this);
         m_apb_req_fifo           = new("m_apb_req_fifo", this);
         m_apb_rsp_export         = new("m_apb_rsp_export", this);
         m_apb_rsp_fifo           = new("m_apb_rsp_fifo", this);
         m_ddr_rsp_export[int'(MEM_LO_P0_ID)] = new($sformatf("m_ddr_rsp_export_%0d", int'(MEM_LO_P0_ID)), this);
         m_ddr_rsp_export[int'(MEM_HI_P0_ID)] = new($sformatf("m_ddr_rsp_export_%0d", int'(MEM_HI_P0_ID)), this);
         m_ddr_rsp_export[int'(MEM_LO_P1_ID)] = new($sformatf("m_ddr_rsp_export_%0d", int'(MEM_LO_P1_ID)), this);
         m_ddr_rsp_export[int'(MEM_HI_P1_ID)] = new($sformatf("m_ddr_rsp_export_%0d", int'(MEM_HI_P1_ID)), this);
         m_ddr_rsp_fifo[int'(MEM_LO_P0_ID)]   = new($sformatf("m_ddr_rsp_fifo_%0d",   int'(MEM_LO_P0_ID)), this);
         m_ddr_rsp_fifo[int'(MEM_HI_P0_ID)]   = new($sformatf("m_ddr_rsp_fifo_%0d",   int'(MEM_HI_P0_ID)), this);
         m_ddr_rsp_fifo[int'(MEM_LO_P1_ID)]   = new($sformatf("m_ddr_rsp_fifo_%0d",   int'(MEM_LO_P1_ID)), this);
         m_ddr_rsp_fifo[int'(MEM_HI_P1_ID)]   = new($sformatf("m_ddr_rsp_fifo_%0d",   int'(MEM_HI_P1_ID)), this);

         m_reg_adapter = evl_reg_adapter::type_id::create("m_ms_reg_adapter", this);
         m_reg_adapter.set_abstract_name($sformatf("%0s(RegAdapt)", get_root_abstract_name()));

         //
         // Create a root register model.
         //
         reg_model = new($sformatf("S%0d", get_shire_id()));
         reg_model.set_agent(this);
         set_evl_reg_block(reg_model);

         //
         // Create a root register model.
         //
         m_memshire_regs = new("memshire");
         m_memshire_regs.set_agent(this);
         m_memshire_regs.set_shire_id(get_shire_id());
         reg_model.add_evl_reg_block(0, m_memshire_regs, "memshire_top.memshire_top.reg_ctl");
         reg_model.build(0);

         m_reg_predictor = new($sformatf("m_ms_reg_predictor"), this);

         m_scoreboard = new($sformatf("%0s(SB)", get_root_abstract_name()), this);
         m_scoreboard.set_agent(this);
         m_scoreboard.set_rtl_port(m_rtl_port);
         m_scoreboard.set_shire_id(get_shire_id());
      end
   endfunction : build_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Function: connect_phase
   //
   virtual function void connect_phase(uvm_phase phase);
      super.connect_phase(phase);
      foreach (m_shire_stim_agents[ii]) begin
         m_shire_stim_agents[ii].connect_my_bus_ports(m_shire_bus_agents[ii]);
      end
      foreach (m_dbgnoc_bus_agents[ii]) begin
         if ((m_dbgnoc_bus_agents[ii] != null) && m_dbgnoc_stim_agents.exists(ii) && (m_dbgnoc_stim_agents[ii] != null)) begin
            m_dbgnoc_stim_agents[ii].connect_my_bus_ports(m_dbgnoc_bus_agents[ii]);
         end
      end
      if (m_shire_stub == 0) begin
         m_mesh_to_mem_req_export.connect(m_mesh_to_mem_req_fifo.analysis_export);
         m_ust_to_mem_req_export.connect(m_ust_to_mem_req_fifo.analysis_export);
         m_mesh_to_sbm_req_export.connect(m_mesh_to_sbm_req_fifo.analysis_export);
         m_apb_req_export.connect(m_apb_req_fifo.analysis_export);
         m_apb_rsp_export.connect(m_apb_rsp_fifo.analysis_export);
         foreach (m_ddr_rsp_export[ii]) begin
            m_ddr_rsp_export[ii].connect(m_ddr_rsp_fifo[ii].analysis_export);
         end

         if (m_shire_bus_agents.exists(`EVL_MESH_TO_MEM_CTL_PORT)) begin
            uvm_analysis_port #(evl_req_desc) tlm_port;

            tlm_port = m_shire_bus_agents[`EVL_MESH_TO_MEM_CTL_PORT].get_req_desc_port("m_obs_req_port");
            if (tlm_port != null) begin
               `evl_log(UVM_MEDIUM, VMOD_CONFIG, get_abstract_name(), $sformatf("connecting %0s -> %0s", tlm_port.get_full_name(), m_mesh_to_mem_req_export.get_full_name()))
               tlm_port.connect(m_mesh_to_mem_req_export);
            end
         end

         if (m_shire_bus_agents.exists(`EVL_UST_TO_MEM_CTL_PORT)) begin
            uvm_analysis_port #(evl_req_desc) tlm_port;

            tlm_port = m_shire_bus_agents[`EVL_UST_TO_MEM_CTL_PORT].get_req_desc_port("m_obs_req_port");
            if (tlm_port != null) begin
               `evl_log(UVM_MEDIUM, VMOD_CONFIG, get_abstract_name(), $sformatf("connecting %0s -> %0s", tlm_port.get_full_name(), m_ust_to_mem_req_export.get_full_name()))
               tlm_port.connect(m_ust_to_mem_req_export);
            end
         end

         if (m_shire_bus_agents.exists(`EVL_MESH_TO_SBM_PORT)) begin
            uvm_analysis_port #(evl_req_desc) tlm_port;

            tlm_port = m_shire_bus_agents[`EVL_MESH_TO_SBM_PORT].get_req_desc_port("m_obs_req_port");
            if (tlm_port != null) begin
               `evl_log(UVM_MEDIUM, VMOD_CONFIG, get_abstract_name(), $sformatf("connecting %0s -> %0s", tlm_port.get_full_name(), m_mesh_to_sbm_req_export.get_full_name()))
               tlm_port.connect(m_mesh_to_sbm_req_export);
            end
         end

         if (m_reg_bus_agent != null) begin
            uvm_analysis_port #(evl_req_desc) tlm_port;

            tlm_port = m_reg_bus_agent.get_req_desc_port("m_obs_req_port");
            if (tlm_port != null) begin
               `evl_log(UVM_MEDIUM, VMOD_CONFIG, get_abstract_name(), $sformatf("connecting %0s -> %0s", tlm_port.get_full_name(), m_apb_req_export.get_full_name()))
               tlm_port.connect(m_apb_req_export);
            end
            tlm_port = m_reg_bus_agent.get_req_desc_port("m_obs_rsp_port");
            if (tlm_port != null) begin
               `evl_log(UVM_MEDIUM, VMOD_CONFIG, get_abstract_name(), $sformatf("connecting %0s -> %0s", tlm_port.get_full_name(), m_apb_rsp_export.get_full_name()))
               tlm_port.connect(m_apb_rsp_export);
            end
         end

         // reg connections
         m_reg_stim_agent.connect_my_bus_ports(m_reg_bus_agent);
         m_reg_stim_agent.connect_reg_predictor_to_monitor(m_reg_predictor);
         m_reg_bus_agent.connect_reg_predictor_to_monitor(m_reg_predictor);

         if (m_reg_bus_agent.get_slave_sequencer() != null) begin
            m_memshire_regs.default_map.set_sequencer(m_reg_bus_agent.get_slave_sequencer(), m_reg_adapter);
         end
         m_reg_predictor.map     = m_memshire_regs.get_map_by_name("APB_ESR");
         m_reg_predictor.adapter = m_reg_adapter;
         m_reg_predictor.map.set_auto_predict(0);
         if (m_disable_ms_sb_esr == 1'b0) begin
            m_reg_predictor.map.set_check_on_read(1);
         end
         m_scoreboard.set_memshire_reg_block(m_memshire_regs);

         //
         // There is a special case for the APB register maps.  Registers off of register APB
         // interfaces are not system-wide unique addresses (the register addresses are only unique
         // per APB bus).  If you look at the UVM code, there is really no particularly convenient
         // way to have hierarchical register blocks while at the same time having a per-subblock
         // address map (available only to that subblock) and a system address map (available to
         // everyone).
         //
         // What happens in the UVM code is that a register map within a register block will only
         // get initialized if there is no parent register block.  So even though there is no
         // parent register map for the bank's APB map, it will not get initialized unless we do
         // it outside of the normal processes.
         //
         m_memshire_regs.lock_model();
         m_reg_predictor.map.Xinit_address_mapX();
      end
   endfunction : connect_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Function: start_of_simulation_phase
   //
   virtual function void start_of_simulation_phase(uvm_phase phase);
      super.start_of_simulation_phase(phase);
      foreach (m_dbgnoc_bus_agents[ii]) begin
         if (m_dbgnoc_stim_agents.exists(ii) && (m_dbgnoc_stim_agents[ii] != null)) begin
            m_dbgnoc_bus_agents[ii].set_master_port_agent(m_dbgnoc_stim_agents[ii]);
         end
      end
      if (m_shire_stub == 1) begin
         foreach (m_shire_bus_agents[ii]) begin
            if (m_shire_stim_agents.exists(ii) && m_shire_stim_agents[ii].is_active_master()) begin
               m_shire_bus_agents[ii].set_master_port_agent(m_shire_stim_agents[ii]);
            end
            if (m_shire_stim_agents.exists(ii) && m_shire_stim_agents[ii].is_active_slave() && (m_master_port_agent != null)) begin
               m_shire_bus_agents[ii].set_slave_port_agent(m_shire_stim_agents[ii]);
               m_shire_bus_agents[ii].set_master_port_agent(m_master_port_agent);
            end
            else if (m_shire_stim_agents.exists(ii) && m_shire_stim_agents[ii].is_active_slave()) begin
               m_shire_bus_agents[ii].set_slave_port_agent(m_shire_stim_agents[ii]);
            end
         end
      end
      else begin
         foreach (m_shire_stim_agents[ii]) begin
            if (m_shire_stim_agents.exists(ii) && (m_shire_stim_agents[ii] != null)) begin
               m_shire_bus_agents[ii].set_slave_port_agent(m_shire_stim_agents[ii]);
            end
         end
         if (m_shire_bus_agents.exists(int'(MESH_TO_MEM_ID))) begin
            m_shire_bus_agents[int'(MESH_TO_MEM_ID)].set_master_port_agent(m_master_port_agent);
         end
         if (m_shire_bus_agents.exists(int'(MESH_TO_SBM_ID))) begin
            m_shire_bus_agents[int'(MESH_TO_SBM_ID)].set_master_port_agent(m_master_port_agent);
         end
         if (m_shire_bus_agents.exists(int'(UST_TO_MEM_ID))) begin
            m_shire_bus_agents[int'(UST_TO_MEM_ID)].set_master_port_agent(m_master_port_agent);
         end
         if (m_shire_stim_agents.exists(int'(ATOMIC_TO_MESH_ID)) && (m_shire_stim_agents[int'(ATOMIC_TO_MESH_ID)] != null)) begin
            m_shire_bus_agents[int'(ATOMIC_TO_MESH_ID)].set_master_port_agent(this);
         end
         watch_for_incoming_reqs(m_mesh_to_mem_req_fifo, `EVL_MEMSHIRE_REQ_ID_FOR_MESH, 1'b1, 1'b1);
         watch_for_incoming_reqs(m_ust_to_mem_req_fifo, `EVL_MEMSHIRE_REQ_ID_FOR_UST, 1'b0, 1'b1);
         watch_for_incoming_reqs(m_mesh_to_sbm_req_fifo, `EVL_MEMSHIRE_REQ_ID_FOR_SBM, 1'b0, 1'b1);
         watch_for_register_setup(m_apb_req_fifo);
         watch_for_register_access(m_apb_rsp_fifo);
         watch_for_ddr_rsps(`EVL_DDR_LO_EVEN_PORT);
         watch_for_ddr_rsps(`EVL_DDR_LO_ODD_PORT);
         watch_for_ddr_rsps(`EVL_DDR_HI_EVEN_PORT);
         watch_for_ddr_rsps(`EVL_DDR_HI_ODD_PORT);

         if (`et_test_plusargs("issue_dut_error_on_interrupt") == 1) begin
            watch_for_interrupts();
         end

         fork
            process_rtl_events();
         join_none
      end
   endfunction : start_of_simulation_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string name_in = "evl_memshire_env", input uvm_component parent_in = null);
      int value;

      super.new(name_in, parent_in, MEM_SHIRE_RTL_AGENT);

      //
      // Memory shire agents are children of memory side agents, but this information should not
      // be used when generating IDs ... we should be assuming a memory shire agent is a child of
      // an SOC agent.
      //
      m_alias_parent_count = 2;
      set_abstract_name("MEM_SHIRE");
      add_agent_alias(MEM_SHIRE_AGENT);
      if (`et_value_plusargs("ms_sb_esr_en=%d", value) == 0) begin
         value = 1;
      end
      m_disable_ms_sb_esr = (value == 0) ? 1'b1 : 1'b0;
   endfunction : new

endclass : evl_memshire_env

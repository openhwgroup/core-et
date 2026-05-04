//--------------------------------------------------------------------------------------------------
//
// Class: evl_soc_env
//
class evl_soc_env extends evl_verif_comp;

   `uvm_component_utils(evl_soc_env)


   //-----------------------------------------------------------------------------------------------
   //
   // Static Member Variables and Objects
   //
   `evl_param_key(pk_memshire_id_max)
   `evl_param_key(pk_memshire_id_min)
   `evl_param_key(pk_memshire_stub)
   `evl_param_key(pk_memshire_use_pll_beh)
   `evl_param_key(pk_minion_shire_count)
   `evl_param_key(pk_minion_shire_stub_hi)
   `evl_param_key(pk_minion_shire_stub_lo)
   `evl_param_key(pk_noc_stub)
   `evl_param_key(pk_use_apb_stub)
   `evl_param_key(pk_use_ddr_stub)


   //-----------------------------------------------------------------------------------------------
   //
   // Local Variables
   //
   int             m_noc_stub;
   int             m_minion_shire_count;
   longint         m_minion_shire_stub;
   evl_soc_noc_env m_noc;
   evl_verif_comp  m_memshire_side_agents[$];
   evl_verif_comp  m_memshire_agents[evl_shire_id_t];
   evl_verif_comp  m_ddr_agents[evl_shire_id_t];
   evl_verif_comp  m_shire_agents[evl_shire_id_t];
   evl_verif_comp  m_ioshire_agents[evl_shire_id_t];
   evl_verif_comp  m_pshire_agents[evl_shire_id_t];


   //-----------------------------------------------------------------------------------------------
   //
   // Function: build_phase
   //
   virtual function void build_phase(uvm_phase phase);
      int              ddr_agent_id = 0;
      uvm_factory      factory;
      uvm_component    component;
      evl_verif_comp   verif_comp;
      evl_rtl_instance instance_list[];
      evl_verif_comp   parent_comp;
      evl_verif_comp   parent_comps[string];

      super.build_phase(phase);
      factory = uvm_factory::get();

      //
      // If there is a real NoC, create one.
      //
      m_noc_stub = m_rtl_port.get_int_param(pk_noc_stub);
      if (m_noc_stub == 0) begin
         m_noc = evl_soc_noc_env::type_id::create("m_noc", this);
         m_noc.set_rtl_port(m_rtl_port);
         m_noc.set_agent_id(~evl_agent_id_t'(0));
         m_noc.set_shire_id(~evl_shire_id_t'(0));
         add_vc_child(m_noc);
      end
      else begin
         m_noc = null;
      end

      evl_rtl_instance_map::get_my_children_list(instance_list, get_full_name());
      m_minion_shire_count = m_rtl_port.get_int_param(pk_minion_shire_count);
      m_minion_shire_stub  = longint'(m_rtl_port.get_int_param(pk_minion_shire_stub_hi));
      m_minion_shire_stub  = (m_minion_shire_stub << 32) | longint'(m_rtl_port.get_int_param(pk_minion_shire_stub_lo));

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
            $cast(verif_comp, component);
            if (instance_list[ii].get_agent_pid() != ~evl_agent_id_t'(0)) begin
               instance_list[ii].propagate_param_strings(verif_comp);
               verif_comp.set_int_param(pk_noc_stub, m_noc_stub);
               if (verif_comp.get_agent_type() inside { MEM_SHIRE_SIDE_RTL_AGENT }) begin
                  verif_comp.set_agent_id(instance_list[ii].get_agent_pid());
                  verif_comp.set_shire_id(~evl_shire_id_t'(0));
                  verif_comp.set_abstract_name($sformatf("%0s-%0d", verif_comp.get_root_abstract_name(), verif_comp.get_agent_id()));
                  m_memshire_side_agents.push_back(verif_comp);
               end
               else if (verif_comp.get_agent_type() inside { DDR_SHIRE_RTL_AGENT }) begin
                  verif_comp.set_agent_id(ddr_agent_id);
                  verif_comp.set_shire_id(instance_list[ii].get_agent_pid());
//                if (instance_list[ii].get_int_param(pk_use_ddr_stub, 0) > 0) begin
//                   verif_comp.set_int_param(pk_use_ddr_stub, 1);
//                end
//                else begin
//                   verif_comp.set_int_param(pk_use_ddr_stub, 0);
//                end
//                if (instance_list[ii].get_int_param(pk_use_apb_stub, 0) > 0) begin
//                   verif_comp.set_int_param(pk_use_apb_stub, 1);
//                end
//                else begin
//                   verif_comp.set_int_param(pk_use_apb_stub, 0);
//                end
                  verif_comp.set_abstract_name($sformatf("%0s-%0d", verif_comp.get_root_abstract_name(), verif_comp.get_agent_id()));
                  m_ddr_agents[verif_comp.get_shire_id()] = verif_comp;
                  ddr_agent_id++;
               end
               else if (verif_comp.get_agent_type() inside { IO_SHIRE_RTL_AGENT }) begin
                  verif_comp.set_agent_id(~evl_agent_id_t'(0));
                  verif_comp.set_shire_id(instance_list[ii].get_agent_pid());
                  verif_comp.set_abstract_name($sformatf("%0s", verif_comp.get_root_abstract_name()));
                  m_ioshire_agents[verif_comp.get_shire_id()] = verif_comp;
               end
               else if (verif_comp.get_agent_type() inside { PSHIRE_RTL_AGENT }) begin
                  verif_comp.set_agent_id(~evl_agent_id_t'(0));
                  verif_comp.set_shire_id(instance_list[ii].get_agent_pid());
                  verif_comp.set_abstract_name($sformatf("%0s", verif_comp.get_root_abstract_name()));
                  m_pshire_agents[verif_comp.get_shire_id()] = verif_comp;
               end
               else if (verif_comp.get_agent_type() inside { SHIRE_RTL_AGENT }) begin
                  verif_comp.set_agent_id(instance_list[ii].get_agent_pid());
                  verif_comp.set_shire_id(instance_list[ii].get_agent_pid());
                  verif_comp.set_abstract_name($sformatf("%0s-%0d", verif_comp.get_root_abstract_name(), verif_comp.get_agent_id()));
                  m_shire_agents[verif_comp.get_shire_id()] = verif_comp;
               end
               else begin
                  verif_comp.set_agent_id(instance_list[ii].get_agent_pid());
                  verif_comp.set_abstract_name($sformatf("%0s-%0d", verif_comp.get_root_abstract_name(), verif_comp.get_agent_id()));
               end
            end
            add_vc_child(verif_comp);
         end
      end
   endfunction : build_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Function: connect_phase
   //
   virtual function void connect_phase(uvm_phase phase);
      evl_verif_comp agent_list[];

      super.connect_phase(phase);

      //
      // To support gate simulations, we have added a memshire side environment.  Because of that,
      // we need gather the list of memshire agents that are available.
      //
      void'(evl_verif_comp_map::get_verif_comp_list(agent_list, MEM_SHIRE_RTL_AGENT));
      foreach (agent_list[ii]) begin
         m_memshire_agents[agent_list[ii].get_shire_id()] = agent_list[ii];
      end
      foreach (m_ddr_agents[shire_id]) begin
         foreach (m_memshire_side_agents[ii]) begin
            if ( (int'(shire_id) >= m_memshire_side_agents[ii].get_int_param(pk_memshire_id_min)) &&
                 (int'(shire_id) <= m_memshire_side_agents[ii].get_int_param(pk_memshire_id_max)) ) begin
               m_ddr_agents[shire_id].set_int_param(pk_memshire_use_pll_beh, m_memshire_side_agents[ii].get_int_param(pk_memshire_use_pll_beh));
            end
         end
      end
      foreach (m_memshire_agents[ii]) begin
         if ((m_ddr_agents.exists(ii)) && (m_memshire_agents[ii] != null))  begin
            m_ddr_agents[ii].connect_my_port_to_export(m_memshire_agents[ii].get_req_desc_export("m_ddr_rsp_export_lo_even"), "m_obs_rsp_port_lo_even");
            m_ddr_agents[ii].connect_my_port_to_export(m_memshire_agents[ii].get_req_desc_export("m_ddr_rsp_export_lo_odd"),  "m_obs_rsp_port_lo_odd");
            m_ddr_agents[ii].connect_my_port_to_export(m_memshire_agents[ii].get_req_desc_export("m_ddr_rsp_export_hi_even"), "m_obs_rsp_port_hi_even");
            m_ddr_agents[ii].connect_my_port_to_export(m_memshire_agents[ii].get_req_desc_export("m_ddr_rsp_export_hi_odd"),  "m_obs_rsp_port_hi_odd");
            m_ddr_agents[ii].set_master_port_agent(m_memshire_agents[ii]);
         end
      end
      if ((m_noc_stub == 0) && (m_noc != null) && (m_noc.m_noc_sb != null)) begin
         foreach (m_shire_agents[ii]) begin
            m_noc.m_noc_sb.shire_exists(ii);
            m_noc.m_noc_sb.connect_port_to_my_export(m_shire_agents[ii].get_req_desc_port("m_axi_req_port", int'(SHIRE_L2TOL3_MESH_AXI)), "m_l3_mesh_req_export", ii);
            m_noc.m_noc_sb.connect_port_to_my_export(m_shire_agents[ii].get_req_desc_port("m_axi_rsp_port", int'(SHIRE_L2TOL3_MESH_AXI)), "m_l3_mesh_rsp_export", ii);
            m_noc.m_noc_sb.connect_port_to_my_export(m_shire_agents[ii].get_req_desc_port("m_axi_rsp_port", int'(SHIRE_MESH_TOL3_AXI)), "m_mesh_l3_rsp_export", ii);
            m_noc.m_noc_sb.connect_port_to_my_export(m_shire_agents[ii].get_req_desc_port("m_axi_req_port", int'(SHIRE_L2TOL3_MESH_AXI), 1), "m_l3b_mesh_req_export", ii);
            m_noc.m_noc_sb.connect_port_to_my_export(m_shire_agents[ii].get_req_desc_port("m_axi_rsp_port", int'(SHIRE_L2TOL3_MESH_AXI), 1), "m_l3b_mesh_rsp_export", ii);
            m_noc.m_noc_sb.connect_port_to_my_export(m_shire_agents[ii].get_req_desc_port("m_axi_rsp_port", int'(SHIRE_MESH_TOL3_AXI), 1), "m_mesh_l3b_rsp_export", ii);
            m_noc.m_noc_sb.connect_port_to_my_export(m_shire_agents[ii].get_req_desc_port("m_axi_req_port", int'(SHIRE_L2TOL3_MESH_AXI), 2), "m_l3c_mesh_req_export", ii);
            m_noc.m_noc_sb.connect_port_to_my_export(m_shire_agents[ii].get_req_desc_port("m_axi_rsp_port", int'(SHIRE_L2TOL3_MESH_AXI), 2), "m_l3c_mesh_rsp_export", ii);
            m_noc.m_noc_sb.connect_port_to_my_export(m_shire_agents[ii].get_req_desc_port("m_axi_rsp_port", int'(SHIRE_MESH_TOL3_AXI), 2), "m_mesh_l3c_rsp_export", ii);
            m_noc.m_noc_sb.connect_port_to_my_export(m_shire_agents[ii].get_req_desc_port("m_axi_req_port", int'(SHIRE_L2TOL3_MESH_AXI), 3), "m_l3d_mesh_req_export", ii);
            m_noc.m_noc_sb.connect_port_to_my_export(m_shire_agents[ii].get_req_desc_port("m_axi_rsp_port", int'(SHIRE_L2TOL3_MESH_AXI), 3), "m_l3d_mesh_rsp_export", ii);
            m_noc.m_noc_sb.connect_port_to_my_export(m_shire_agents[ii].get_req_desc_port("m_axi_rsp_port", int'(SHIRE_MESH_TOL3_AXI), 3), "m_mesh_l3d_rsp_export", ii);
            m_noc.m_noc_sb.connect_port_to_my_export(m_shire_agents[ii].get_req_desc_port("m_axi_req_port", int'(SHIRE_L3TOSYS_MESH_AXI)), "m_sys_mesh_req_export", ii);
            m_noc.m_noc_sb.connect_port_to_my_export(m_shire_agents[ii].get_req_desc_port("m_axi_rsp_port", int'(SHIRE_L3TOSYS_MESH_AXI)), "m_sys_mesh_rsp_export", ii);
            m_noc.m_noc_sb.connect_port_to_my_export(m_shire_agents[ii].get_req_desc_port("m_axi_rsp_port", int'(SHIRE_FROMSYS_MESH_AXI)), "m_mesh_sbm_rsp_export", ii);
            if (m_shire_agents[ii].get_req_desc_port("m_axi_req_port", int'(SHIRE_UCTOL3_MESH_AXI)) != null) begin
               m_noc.m_noc_sb.connect_port_to_my_export(m_shire_agents[ii].get_req_desc_port("m_axi_req_port", int'(SHIRE_UCTOL3_MESH_AXI)), "m_uctol3_mesh_req_export", ii);
               m_noc.m_noc_sb.connect_port_to_my_export(m_shire_agents[ii].get_req_desc_port("m_axi_rsp_port", int'(SHIRE_UCTOL3_MESH_AXI)), "m_uctol3_mesh_rsp_export", ii);
            end
            if (m_shire_agents[ii].get_req_desc_port("m_axi_req_port", int'(SHIRE_UCTOSYS_MESH_AXI)) != null) begin
               m_noc.m_noc_sb.connect_port_to_my_export(m_shire_agents[ii].get_req_desc_port("m_axi_req_port", int'(SHIRE_UCTOSYS_MESH_AXI)), "m_uctosys_mesh_req_export", ii);
               m_noc.m_noc_sb.connect_port_to_my_export(m_shire_agents[ii].get_req_desc_port("m_axi_rsp_port", int'(SHIRE_UCTOSYS_MESH_AXI)), "m_uctosys_mesh_rsp_export", ii);
            end
            if (m_noc.m_dbgnoc_sb != null) begin
               m_noc.m_dbgnoc_sb.connect_port_to_my_export(m_shire_agents[ii].get_req_desc_port("m_dbgnoc_tx_req_port", int'(MIN_DBG_DN_EVT)), "m_dn_evt_tx_req_export", ii);
               m_noc.m_dbgnoc_sb.connect_port_to_my_export(m_shire_agents[ii].get_req_desc_port("m_dbgnoc_rx_req_port", int'(MIN_DBG_DN_EVT)), "m_dn_evt_rx_req_export", ii);
               m_noc.m_dbgnoc_sb.connect_port_to_my_export(m_shire_agents[ii].get_req_desc_port("m_dbgnoc_tx_req_port", int'(MIN_DBG_DN_MSG)), "m_dn_msg_tx_req_export", ii);
               m_noc.m_dbgnoc_sb.connect_port_to_my_export(m_shire_agents[ii].get_req_desc_port("m_dbgnoc_rx_req_port", int'(MIN_DBG_DN_MSG)), "m_dn_msg_rx_req_export", ii);
            end
            m_shire_agents[ii].set_master_port_agent(m_noc);
         end
         foreach (m_memshire_agents[ii]) begin
            if (m_memshire_agents[ii] != null)  begin
               m_noc.m_noc_sb.shire_exists(ii);
               m_noc.m_noc_sb.connect_port_to_my_export(m_memshire_agents[ii].get_req_desc_port("m_axi_rsp_port", 0), "m_mesh_mem_rsp_export", ii);
               m_noc.m_noc_sb.connect_port_to_my_export(m_memshire_agents[ii].get_req_desc_port("m_axi_rsp_port", 1), "m_mc_mesh_sbm_rsp_export", ii);
               m_noc.m_noc_sb.connect_port_to_my_export(m_memshire_agents[ii].get_req_desc_port("m_axi_req_port", 3), "m_mctosys_mesh_req_export", ii);
               m_noc.m_noc_sb.connect_port_to_my_export(m_memshire_agents[ii].get_req_desc_port("m_axi_rsp_port", 3), "m_mctosys_mesh_rsp_export", ii);
               if (m_noc.m_dbgnoc_sb != null) begin
                  m_noc.m_dbgnoc_sb.connect_port_to_my_export(m_memshire_agents[ii].get_req_desc_port("m_dbgnoc_tx_req_port", int'(MEM_DBG_DN_EVT)), "m_dn_evt_tx_req_export", ii);
                  m_noc.m_dbgnoc_sb.connect_port_to_my_export(m_memshire_agents[ii].get_req_desc_port("m_dbgnoc_rx_req_port", int'(MEM_DBG_DN_EVT)), "m_dn_evt_rx_req_export", ii);
                  m_noc.m_dbgnoc_sb.connect_port_to_my_export(m_memshire_agents[ii].get_req_desc_port("m_dbgnoc_tx_req_port", int'(MEM_DBG_DN_MSG)), "m_dn_msg_tx_req_export", ii);
                  m_noc.m_dbgnoc_sb.connect_port_to_my_export(m_memshire_agents[ii].get_req_desc_port("m_dbgnoc_rx_req_port", int'(MEM_DBG_DN_MSG)), "m_dn_msg_rx_req_export", ii);
                  m_noc.m_dbgnoc_sb.connect_port_to_my_export(m_memshire_agents[ii].get_req_desc_port("m_dbgnoc_tx_req_port", int'(MEM_DBG_LF_EVT)), "m_lf_evt_tx_req_export", ii);
                  m_noc.m_dbgnoc_sb.connect_port_to_my_export(m_memshire_agents[ii].get_req_desc_port("m_dbgnoc_rx_req_port", int'(MEM_DBG_LF_EVT)), "m_lf_evt_rx_req_export", ii);
                  m_noc.m_dbgnoc_sb.connect_port_to_my_export(m_memshire_agents[ii].get_req_desc_port("m_dbgnoc_tx_req_port", int'(MEM_DBG_LF_MSG)), "m_lf_msg_tx_req_export", ii);
                  m_noc.m_dbgnoc_sb.connect_port_to_my_export(m_memshire_agents[ii].get_req_desc_port("m_dbgnoc_rx_req_port", int'(MEM_DBG_LF_MSG)), "m_lf_msg_rx_req_export", ii);
               end
               m_memshire_agents[ii].set_master_port_agent(m_noc);
            end
         end
         foreach (m_ioshire_agents[ii]) begin
            m_noc.m_noc_sb.shire_exists(ii);
            m_noc.m_noc_sb.connect_port_to_my_export(m_ioshire_agents[ii].get_req_desc_port("m_axi_req_port", int'(IO_TOL3_AXI)), "m_io_tol3_mesh_req_export", ii);
            m_noc.m_noc_sb.connect_port_to_my_export(m_ioshire_agents[ii].get_req_desc_port("m_axi_rsp_port", int'(IO_TOL3_AXI)), "m_io_tol3_mesh_rsp_export", ii);
            m_noc.m_noc_sb.connect_port_to_my_export(m_ioshire_agents[ii].get_req_desc_port("m_axi_req_port", int'(IO_TOL3_AXI), 1), "m_io_tol3b_mesh_req_export", ii);
            m_noc.m_noc_sb.connect_port_to_my_export(m_ioshire_agents[ii].get_req_desc_port("m_axi_rsp_port", int'(IO_TOL3_AXI), 1), "m_io_tol3b_mesh_rsp_export", ii);
            m_noc.m_noc_sb.connect_port_to_my_export(m_ioshire_agents[ii].get_req_desc_port("m_axi_req_port", int'(IO_TOL3_AXI), 2), "m_io_tol3c_mesh_req_export", ii);
            m_noc.m_noc_sb.connect_port_to_my_export(m_ioshire_agents[ii].get_req_desc_port("m_axi_rsp_port", int'(IO_TOL3_AXI), 2), "m_io_tol3c_mesh_rsp_export", ii);
            m_noc.m_noc_sb.connect_port_to_my_export(m_ioshire_agents[ii].get_req_desc_port("m_axi_req_port", int'(IO_TOL3_AXI), 3), "m_io_tol3d_mesh_req_export", ii);
            m_noc.m_noc_sb.connect_port_to_my_export(m_ioshire_agents[ii].get_req_desc_port("m_axi_rsp_port", int'(IO_TOL3_AXI), 3), "m_io_tol3d_mesh_rsp_export", ii);
            m_noc.m_noc_sb.connect_port_to_my_export(m_ioshire_agents[ii].get_req_desc_port("m_axi_req_port", int'(IO_PM_AXI)), "m_io_pm_req_export", ii);
            m_noc.m_noc_sb.connect_port_to_my_export(m_ioshire_agents[ii].get_req_desc_port("m_axi_rsp_port", int'(IO_PM_AXI)), "m_io_pm_rsp_export", ii);
            m_noc.m_noc_sb.connect_port_to_my_export(m_ioshire_agents[ii].get_req_desc_port("m_axi_rsp_port", int'(IO_PS_AXI)), "m_io_ps_rsp_export", ii);
            m_noc.m_noc_sb.connect_port_to_my_export(m_ioshire_agents[ii].get_req_desc_port("m_axi_req_port", int'(IO_TOSYS_AXI)), "m_io_tosys_mesh_req_export", ii);
            m_noc.m_noc_sb.connect_port_to_my_export(m_ioshire_agents[ii].get_req_desc_port("m_axi_rsp_port", int'(IO_TOSYS_AXI)), "m_io_tosys_mesh_rsp_export", ii);
            m_noc.m_noc_sb.connect_port_to_my_export(m_ioshire_agents[ii].get_req_desc_port("m_axi_req_port", int'(IO_MAIN_NOC_RBM)), "m_io_main_noc_rbm_req_export", ii);
            m_noc.m_noc_sb.connect_port_to_my_export(m_ioshire_agents[ii].get_req_desc_port("m_axi_rsp_port", int'(IO_MAIN_NOC_RBM)), "m_io_main_noc_rbm_rsp_export", ii);
            if (m_noc.m_dbgnoc_sb != null) begin
               m_noc.m_dbgnoc_sb.connect_port_to_my_export(m_ioshire_agents[ii].get_req_desc_port("m_dbgnoc_tx_req_port", int'(IO_DBG_RT_EVT)), "m_rt_evt_tx_req_export", ii);
               m_noc.m_dbgnoc_sb.connect_port_to_my_export(m_ioshire_agents[ii].get_req_desc_port("m_dbgnoc_rx_req_port", int'(IO_DBG_RT_EVT)), "m_rt_evt_rx_req_export", ii);
               m_noc.m_dbgnoc_sb.connect_port_to_my_export(m_ioshire_agents[ii].get_req_desc_port("m_dbgnoc_tx_req_port", int'(IO_DBG_RT_MSG)), "m_rt_msg_tx_req_export", ii);
               m_noc.m_dbgnoc_sb.connect_port_to_my_export(m_ioshire_agents[ii].get_req_desc_port("m_dbgnoc_rx_req_port", int'(IO_DBG_RT_MSG)), "m_rt_msg_rx_req_export", ii);
               m_noc.m_dbgnoc_sb.connect_port_to_my_export(m_ioshire_agents[ii].get_req_desc_port("m_dbgnoc_tx_req_port", int'(IO_DBG_LF_EVT)), "m_lf_evt_tx_req_export", ii);
               m_noc.m_dbgnoc_sb.connect_port_to_my_export(m_ioshire_agents[ii].get_req_desc_port("m_dbgnoc_rx_req_port", int'(IO_DBG_LF_EVT)), "m_lf_evt_rx_req_export", ii);
               m_noc.m_dbgnoc_sb.connect_port_to_my_export(m_ioshire_agents[ii].get_req_desc_port("m_dbgnoc_tx_req_port", int'(IO_DBG_LF_MSG)), "m_lf_msg_tx_req_export", ii);
               m_noc.m_dbgnoc_sb.connect_port_to_my_export(m_ioshire_agents[ii].get_req_desc_port("m_dbgnoc_rx_req_port", int'(IO_DBG_LF_MSG)), "m_lf_msg_rx_req_export", ii);
            end
            m_ioshire_agents[ii].set_master_port_agent(m_noc);
         end
         foreach (m_pshire_agents[ii]) begin
            m_noc.m_noc_sb.shire_exists(ii);
            m_noc.m_noc_sb.connect_port_to_my_export(m_pshire_agents[ii].get_req_desc_port("m_axi_req_port", int'(PSHIRE_TOL3_AXI)), "m_io_tol3_mesh_req_export", ii);
            m_noc.m_noc_sb.connect_port_to_my_export(m_pshire_agents[ii].get_req_desc_port("m_axi_rsp_port", int'(PSHIRE_TOL3_AXI)), "m_io_tol3_mesh_rsp_export", ii);
            m_noc.m_noc_sb.connect_port_to_my_export(m_pshire_agents[ii].get_req_desc_port("m_axi_req_port", int'(PSHIRE_TOL3_AXI), 1), "m_io_tol3b_mesh_req_export", ii);
            m_noc.m_noc_sb.connect_port_to_my_export(m_pshire_agents[ii].get_req_desc_port("m_axi_rsp_port", int'(PSHIRE_TOL3_AXI), 1), "m_io_tol3b_mesh_rsp_export", ii);
            m_noc.m_noc_sb.connect_port_to_my_export(m_pshire_agents[ii].get_req_desc_port("m_axi_req_port", int'(PSHIRE_TOL3_AXI), 2), "m_io_tol3c_mesh_req_export", ii);
            m_noc.m_noc_sb.connect_port_to_my_export(m_pshire_agents[ii].get_req_desc_port("m_axi_rsp_port", int'(PSHIRE_TOL3_AXI), 2), "m_io_tol3c_mesh_rsp_export", ii);
            m_noc.m_noc_sb.connect_port_to_my_export(m_pshire_agents[ii].get_req_desc_port("m_axi_req_port", int'(PSHIRE_TOL3_AXI), 3), "m_io_tol3d_mesh_req_export", ii);
            m_noc.m_noc_sb.connect_port_to_my_export(m_pshire_agents[ii].get_req_desc_port("m_axi_rsp_port", int'(PSHIRE_TOL3_AXI), 3), "m_io_tol3d_mesh_rsp_export", ii);
            m_noc.m_noc_sb.connect_port_to_my_export(m_pshire_agents[ii].get_req_desc_port("m_axi_rsp_port", int'(PSHIRE_PS_AXI)), "m_io_ps_rsp_export", ii);
            m_noc.m_noc_sb.connect_port_to_my_export(m_pshire_agents[ii].get_req_desc_port("m_axi_req_port", int'(PSHIRE_TOSYS_AXI)), "m_io_tosys_mesh_req_export", ii);
            m_noc.m_noc_sb.connect_port_to_my_export(m_pshire_agents[ii].get_req_desc_port("m_axi_rsp_port", int'(PSHIRE_TOSYS_AXI)), "m_io_tosys_mesh_rsp_export", ii);
            if (m_noc.m_dbgnoc_sb != null) begin
               m_noc.m_dbgnoc_sb.connect_port_to_my_export(m_pshire_agents[ii].get_req_desc_port("m_dbgnoc_tx_req_port", int'(PS_DBG_DN_EVT)), "m_dn_evt_tx_req_export", ii);
               m_noc.m_dbgnoc_sb.connect_port_to_my_export(m_pshire_agents[ii].get_req_desc_port("m_dbgnoc_rx_req_port", int'(PS_DBG_DN_EVT)), "m_dn_evt_rx_req_export", ii);
               m_noc.m_dbgnoc_sb.connect_port_to_my_export(m_pshire_agents[ii].get_req_desc_port("m_dbgnoc_tx_req_port", int'(PS_DBG_DN_MSG)), "m_dn_msg_tx_req_export", ii);
               m_noc.m_dbgnoc_sb.connect_port_to_my_export(m_pshire_agents[ii].get_req_desc_port("m_dbgnoc_rx_req_port", int'(PS_DBG_DN_MSG)), "m_dn_msg_rx_req_export", ii);
            end
            m_pshire_agents[ii].set_master_port_agent(m_noc);
         end
      end
   endfunction : connect_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Task: run_phase
   //
   virtual task run_phase(uvm_phase phase);
      fork
         super.run_phase(phase);
         if ((m_noc != null) && (m_noc.m_noc_sb != null)) begin
            foreach (m_shire_agents[ii]) begin
               m_noc.m_noc_sb.watch_shire_axi_operations(ii);
               if (m_noc.m_dbgnoc_sb != null) begin
                  m_noc.m_dbgnoc_sb.watch_shire_dbgnoc_operations(ii);
               end
            end
            foreach (m_memshire_agents[ii]) begin
               if (m_memshire_agents[ii] != null) begin
                  m_noc.m_noc_sb.watch_memshire_axi_operations(ii);
                  if (m_noc.m_dbgnoc_sb != null) begin
                     m_noc.m_dbgnoc_sb.watch_memshire_dbgnoc_operations(ii);
                  end
               end
            end
            foreach (m_ioshire_agents[ii]) begin
               m_noc.m_noc_sb.watch_ioshire_axi_operations(ii);
               if (m_noc.m_dbgnoc_sb != null) begin
                  m_noc.m_dbgnoc_sb.watch_ioshire_dbgnoc_operations(ii);
               end
            end
            foreach (m_pshire_agents[ii]) begin
               m_noc.m_noc_sb.watch_pshire_axi_operations(ii);
               if (m_noc.m_dbgnoc_sb != null) begin
                  m_noc.m_dbgnoc_sb.watch_pshire_dbgnoc_operations(ii);
               end
            end
         end
      join_none
   endtask : run_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string name_in = "evl_soc_env", input uvm_component parent_in = null);
      super.new(name_in, parent_in, SOC_RTL_AGENT);
      set_abstract_name("SOC");
   endfunction : new

endclass : evl_soc_env

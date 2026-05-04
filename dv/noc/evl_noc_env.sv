//--------------------------------------------------------------------------------------------------
//
// Class: evl_noc_env
//
class evl_noc_env extends evl_verif_comp;

   `uvm_component_utils(evl_noc_env)


   //-----------------------------------------------------------------------------------------------
   //
   // Member Variables and Objects
   //
   evl_verif_comp m_memshire_agents[evl_shire_id_t];
   evl_verif_comp m_ddr_agents[evl_shire_id_t];
   evl_verif_comp m_axi_shire_agents[evl_shire_id_t];
   evl_verif_comp m_axi_memshire_agents[evl_shire_id_t];

   evl_noc_scoreboard m_noc_sb;


   //-----------------------------------------------------------------------------------------------
   //
   // Function: build_phase
   //
   virtual function void build_phase(uvm_phase phase);
      uvm_factory      factory;
      uvm_component    component;
      evl_verif_comp   verif_comp;
      evl_rtl_instance instance_list[];
      evl_verif_comp   parent_comp;
      evl_verif_comp   parent_comps[string];

      super.build_phase(phase);
      factory = uvm_factory::get();

      m_noc_sb = evl_noc_scoreboard::type_id::create("m_noc_sb", this);
      m_noc_sb.set_agent(this);
      m_noc_sb.set_rtl_port(m_rtl_port);
      m_noc_sb.set_int_param_by_name("pk_noc_sb_err_intensity", 0); // error intensity param for NoC connectivity checks
                                                                    // set to 0 for errors, 1 for warnings


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
            $cast(verif_comp, component);
            if (instance_list[ii].get_agent_pid() != ~evl_agent_id_t'(0)) begin
               verif_comp.set_agent_id(instance_list[ii].get_agent_pid());
               verif_comp.set_abstract_name($sformatf("%0s-%0d", verif_comp.get_root_abstract_name(), instance_list[ii].get_agent_pid()));
               if (verif_comp.get_agent_type() inside { SHIRE_RTL_AGENT, MEM_SHIRE_RTL_AGENT, DDR_SHIRE_RTL_AGENT }) begin
                  verif_comp.set_shire_id(instance_list[ii].get_agent_pid());
                  if (verif_comp.get_agent_type() inside { SHIRE_RTL_AGENT }) begin
                     if ((verif_comp.m_rtl_port.get_int_param_by_name("pk_sc_stub", 0) == 1) && (verif_comp.m_rtl_port.get_int_param_by_name("pk_noc_stub", 0) == 0)) begin
                        m_axi_shire_agents[verif_comp.get_shire_id()] = verif_comp;
                     end
                  end
                  else if (verif_comp.get_agent_type() inside { MEM_SHIRE_RTL_AGENT }) begin
                     if (verif_comp.m_rtl_port.get_int_param_by_name("pk_shire_stub", 0) == 1) begin
                        m_axi_memshire_agents[verif_comp.get_shire_id()] = verif_comp;
                     end
                     else begin
                        m_memshire_agents[verif_comp.get_shire_id()] = verif_comp;
                     end
                  end
                  else if (verif_comp.get_agent_type() inside { DDR_SHIRE_RTL_AGENT }) begin
                     m_ddr_agents[verif_comp.get_shire_id()] = verif_comp;
                  end
               end
            end
            add_vc_child(verif_comp);
         end
      end
   endfunction : build_phase

   //-----------------------------------------------------------------------------------------------
   //
   // Task: run_phase
   //
   virtual task run_phase(uvm_phase phase);
      fork
         super.run_phase(phase);
         foreach (m_axi_shire_agents[ii]) begin
            m_noc_sb.watch_shire_axi_operations(ii);
         end
         foreach (m_axi_memshire_agents[ii]) begin
            m_noc_sb.watch_memshire_axi_operations(ii);
         end
      join_none
   endtask : run_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Function: connect_phase
   //
   virtual function void connect_phase(uvm_phase phase);
      super.connect_phase(phase);
      foreach (m_memshire_agents[ii]) begin
         if (m_ddr_agents.exists(ii)) begin
            m_ddr_agents[ii].connect_my_port_to_export(m_memshire_agents[ii].get_req_desc_export("m_ddr_rsp_export_lo_even"), "m_obs_rsp_port_lo_even");
            m_ddr_agents[ii].connect_my_port_to_export(m_memshire_agents[ii].get_req_desc_export("m_ddr_rsp_export_lo_odd"),  "m_obs_rsp_port_lo_odd");
            m_ddr_agents[ii].connect_my_port_to_export(m_memshire_agents[ii].get_req_desc_export("m_ddr_rsp_export_hi_even"), "m_obs_rsp_port_hi_even");
            m_ddr_agents[ii].connect_my_port_to_export(m_memshire_agents[ii].get_req_desc_export("m_ddr_rsp_export_hi_odd"),  "m_obs_rsp_port_hi_odd");
            m_ddr_agents[ii].set_master_port_agent(m_memshire_agents[ii]);
         end
      end
      foreach (m_axi_shire_agents[ii]) begin
         m_noc_sb.connect_port_to_my_export(m_axi_shire_agents[ii].get_req_desc_port("m_axi_req_port", 0), "m_l3_mesh_req_export", ii);
         m_noc_sb.connect_port_to_my_export(m_axi_shire_agents[ii].get_req_desc_port("m_axi_rsp_port", 0), "m_l3_mesh_rsp_export", ii);
         m_noc_sb.connect_port_to_my_export(m_axi_shire_agents[ii].get_req_desc_port("m_axi_req_port", 1), "m_sys_mesh_req_export", ii);
         m_noc_sb.connect_port_to_my_export(m_axi_shire_agents[ii].get_req_desc_port("m_axi_rsp_port", 1), "m_sys_mesh_rsp_export", ii);
         m_noc_sb.connect_port_to_my_export(m_axi_shire_agents[ii].get_req_desc_port("m_axi_req_port", 2), "m_mesh_l3_req_export", ii);
         m_noc_sb.connect_port_to_my_export(m_axi_shire_agents[ii].get_req_desc_port("m_axi_rsp_port", 2), "m_mesh_l3_rsp_export", ii);
         m_noc_sb.connect_port_to_my_export(m_axi_shire_agents[ii].get_req_desc_port("m_axi_req_port", 3), "m_mesh_sbm_req_export", ii);
         m_noc_sb.connect_port_to_my_export(m_axi_shire_agents[ii].get_req_desc_port("m_axi_rsp_port", 3), "m_mesh_sbm_rsp_export", ii);
         m_noc_sb.connect_port_to_my_export(m_axi_shire_agents[ii].get_req_desc_port("m_axi_req_port", 4), "m_uctol3_mesh_req_export", ii);
         m_noc_sb.connect_port_to_my_export(m_axi_shire_agents[ii].get_req_desc_port("m_axi_rsp_port", 4), "m_uctol3_mesh_rsp_export", ii);
         m_noc_sb.connect_port_to_my_export(m_axi_shire_agents[ii].get_req_desc_port("m_axi_req_port", 5), "m_uctosys_mesh_req_export", ii);
         m_noc_sb.connect_port_to_my_export(m_axi_shire_agents[ii].get_req_desc_port("m_axi_rsp_port", 5), "m_uctosys_mesh_rsp_export", ii);
         m_axi_shire_agents[ii].set_master_port_agent(this);
      end
      foreach (m_axi_memshire_agents[ii]) begin
         m_noc_sb.connect_port_to_my_export(m_axi_memshire_agents[ii].get_req_desc_port("m_axi_req_port", 0), "m_mesh_mem_req_export", ii);
         m_noc_sb.connect_port_to_my_export(m_axi_memshire_agents[ii].get_req_desc_port("m_axi_rsp_port", 0), "m_mesh_mem_rsp_export", ii);
         m_noc_sb.connect_port_to_my_export(m_axi_memshire_agents[ii].get_req_desc_port("m_axi_req_port", 1), "m_mc_mesh_sbm_req_export", ii);
         m_noc_sb.connect_port_to_my_export(m_axi_memshire_agents[ii].get_req_desc_port("m_axi_rsp_port", 1), "m_mc_mesh_sbm_rsp_export", ii);
         m_noc_sb.connect_port_to_my_export(m_axi_memshire_agents[ii].get_req_desc_port("m_axi_req_port", 3), "m_mctosys_mesh_req_export", ii);
         m_noc_sb.connect_port_to_my_export(m_axi_memshire_agents[ii].get_req_desc_port("m_axi_rsp_port", 3), "m_mctosys_mesh_rsp_export", ii);
         m_axi_memshire_agents[ii].set_master_port_agent(this);
      end

   endfunction : connect_phase

   //-----------------------------------------------------------------------------------------------
   //
   // Function: find_observed_request
   //
   virtual function evl_req_desc find_observed_request(input evl_agent_type_t     port_type_in,
                                                       input int                  subport_in,
                                                       input int                  port_in,
                                                       input evl_req_desc         paired_req_desc,
                                                       input evl_bus_cmd_t        bus_cmd_in   = `EVL_Bus_Idle,
                                                       input evl_paddr_t          address_in   = ~evl_paddr_t'(0),
                                                       input evl_bus_req_id_t     req_id_in    = ~evl_bus_req_id_t'(0),
                                                       input evl_bus_req_source_t source_id_in = ~evl_bus_req_source_t'(0),
                                                       input evl_trans_id_t       trans_id_in  = ~evl_trans_id_t'(0));

      int              obs_req_shire_id;
      int              obs_req_port_id;
      evl_req_desc     match_req_desc;

      //`evl_log(UVM_DEBUG, VMOD_NOC_SB, get_abstract_name(), $sformatf("NOC_ENV: Request with ID 0x%0x observed on port %0d", paired_req_desc.get_bus_req_id(), port_in))
      obs_req_port_id = port_in % 6;
      obs_req_shire_id = port_in/6;
      //`evl_log(UVM_DEBUG, VMOD_NOC_SB, get_abstract_name(), $sformatf("NOC_ENV: Request with ID 0x%0x observed on port %0d of shire %0d", paired_req_desc.get_bus_req_id(), obs_req_port_id, obs_req_shire_id))

      if (port_type_in == AXI_BUS_AGENT) begin
         if (obs_req_shire_id >= 36) begin
            if (obs_req_port_id == 0) begin
               `evl_log_mdesc(UVM_DEBUG, VMOD_NOC_SB, get_abstract_name(), paired_req_desc, $sformatf("NOC_ENV: Request with ID 0x%0x observed on TO MEM Slave port of shire %0d", paired_req_desc.get_bus_req_id(), obs_req_shire_id))
               match_req_desc = m_noc_sb.match_observed_request(paired_req_desc, obs_req_shire_id, TOMEM_S);
            end
            else if (obs_req_port_id == 1) begin
               `evl_log_mdesc(UVM_DEBUG, VMOD_NOC_SB, get_abstract_name(), paired_req_desc, $sformatf("NOC_ENV: Request with ID 0x%0x observed on SYS Slave port of shire %0d", paired_req_desc.get_bus_req_id(), obs_req_shire_id))
               match_req_desc = m_noc_sb.match_observed_request(paired_req_desc, obs_req_shire_id, MC_SB_S);
            end
         end
         else begin
            if (obs_req_port_id == 2) begin
               `evl_log_mdesc(UVM_DEBUG, VMOD_NOC_SB, get_abstract_name(), paired_req_desc, $sformatf("NOC_ENV: Request with ID 0x%0x observed on L3 Slave port of shire %0d", paired_req_desc.get_bus_req_id(), obs_req_shire_id))
               match_req_desc = m_noc_sb.match_observed_request(paired_req_desc, obs_req_shire_id, TOL3_S);
            end
            else if (obs_req_port_id == 3) begin
               `evl_log_mdesc(UVM_DEBUG, VMOD_NOC_SB, get_abstract_name(), paired_req_desc, $sformatf("NOC_ENV: Request with ID 0x%0x observed on SYS Slave port of shire %0d", paired_req_desc.get_bus_req_id(), obs_req_shire_id))
               match_req_desc = m_noc_sb.match_observed_request(paired_req_desc, obs_req_shire_id, TOSYS_S);
            end
         end
         if (match_req_desc != null) begin
            match_req_desc.add_child_desc(paired_req_desc);
         end
         return match_req_desc;
      end

      return null;
   endfunction : find_observed_request

   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string name_in = "evl_noc_env", input uvm_component parent_in = null);
      super.new(name_in, parent_in, SOC_RTL_AGENT);
      set_abstract_name("SOC");
   endfunction : new

endclass : evl_noc_env

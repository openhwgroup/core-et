//--------------------------------------------------------------------------------------------------
//
// Class: evl_standalone_minion_env
//

// Note: For IO Shire SP minion, we only need our m_core_agent (L1SB) to connect to the following:
// - m_core_bus_miss_agents
// - m_core_bus_evict_agents
// - m_core_bus_ptw_agents
// - m_core_bus_dbg_agents

class evl_standalone_minion_env extends evl_verif_comp;
   
   `uvm_component_utils(evl_standalone_minion_env)


   //-----------------------------------------------------------------------------------------------
   //
   // Static Member Variables and Objects
   //


   //-----------------------------------------------------------------------------------------------
   //
   // Member Variables and Objects
   //

   // Children core RTL agents
   evl_verif_comp m_core_agents[evl_agent_id_t];            // actually only one for IO_Shire

   // Core ETLink bus agents
   evl_verif_comp m_core_bus_miss_agents[evl_agent_id_t];   // actually only one for IO_Shire
   evl_verif_comp m_core_bus_evict_agents[evl_agent_id_t];
   evl_verif_comp m_core_bus_ptw_agents[evl_agent_id_t];
   evl_verif_comp m_core_bus_dbg_agents[evl_agent_id_t];


   //-----------------------------------------------------------------------------------------------
   //
   // Function: build_phase
   //
   virtual function void build_phase(uvm_phase phase);
      uvm_factory      factory;
      uvm_component    component;
      uvm_component    stim_agent;
      evl_verif_comp   verif_comp;
      evl_rtl_instance instance_list[];
      evl_verif_comp   parent_comp;
      evl_verif_comp   parent_comps[string];

      super.build_phase(phase);

      factory = uvm_factory::get();
      evl_rtl_instance_map::get_my_children_list(instance_list, get_full_name());

      //
      // To manipulate the names, set the core ID to be invalid for now.
      //
      set_core_id(~evl_core_id_t'(0), 1'b1);
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

            if (verif_comp.is_bus_vc() == 1'b1) begin
               if (verif_comp.get_agent_type() == evl_base_pkg::ETLINK_BUS_AGENT) begin
                  verif_comp.set_agent_id(agent_id);
                  if (instance_list[ii].get_inst_name() == "m_miss_req") begin: CORE_MISS_BUS_AGENT
                     verif_comp.set_core_id(evl_core_id_t'(0));
                     verif_comp.set_abstract_name($sformatf("C0(MISS)"));
                     m_core_bus_miss_agents[agent_id] = verif_comp;
                     m_core_bus_miss_agents[agent_id].set_port_id(0);
                     `evl_log(UVM_DEBUG, VMOD_CONFIG, get_abstract_name(), $sformatf("ETLINK_BUS_AGENT:m_miss_req is found"))
                  end: CORE_MISS_BUS_AGENT
                  else if (instance_list[ii].get_inst_name() == "m_evict_req") begin: CORE_EVICT_BUS_AGENT
                     verif_comp.set_core_id(evl_core_id_t'(0));
                     verif_comp.set_abstract_name($sformatf("C0(EVICT)"));
                     m_core_bus_evict_agents[agent_id] = verif_comp;
                     m_core_bus_evict_agents[agent_id].set_port_id(1);
                     `evl_log(UVM_DEBUG, VMOD_CONFIG, get_abstract_name(), $sformatf("ETLINK_BUS_AGENT:m_evict_req is found"))
                  end: CORE_EVICT_BUS_AGENT
               end
               else if (verif_comp.get_agent_type() == evl_base_pkg::PTW_BUS_AGENT) begin: PTW_BUS_AGENT
                  verif_comp.set_agent_id(agent_id);
                  if (instance_list[ii].get_inst_name() == "m_ptw_req") begin
                     verif_comp.set_core_id(evl_core_id_t'(0));
                     verif_comp.set_abstract_name($sformatf("C0(PTW)"));
                     m_core_bus_ptw_agents[agent_id] = verif_comp;
                     `evl_log(UVM_DEBUG, VMOD_CONFIG, get_abstract_name(), $sformatf("PTW_BUS_AGENT:m_ptw_req is found"))
                  end
               end: PTW_BUS_AGENT
               else if (verif_comp.get_agent_type() == evl_base_pkg::APB_BUS_AGENT) begin: APB_BUS_AGENT
                  if (instance_list[ii].get_inst_name() == "m_debug_itf") begin
                     verif_comp.set_core_id(evl_core_id_t'(0));
                     verif_comp.set_agent_id(agent_id);
                     verif_comp.set_abstract_name($sformatf("C0(DBG)"));
                     m_core_bus_dbg_agents[agent_id] = verif_comp;
                  end
               end: APB_BUS_AGENT
               add_vc_child(verif_comp);
            end
            else begin
               if (verif_comp.get_agent_type() == evl_base_pkg::CORE_RTL_AGENT) begin
                  verif_comp.set_core_id(evl_core_id_t'(0));
                  verif_comp.set_agent_id(agent_id);
                  verif_comp.set_abstract_name($sformatf("C%0d", agent_id));
                  m_core_agents[agent_id] = verif_comp;
                  `evl_log(UVM_DEBUG, VMOD_CONFIG, get_abstract_name(), $sformatf("CORE_RTL_AGENT is found"))
               end
               else if (agent_id != ~evl_agent_id_t'(0)) begin
                  verif_comp.set_agent_id(agent_id);
                  verif_comp.set_abstract_name($sformatf("%0s-%0d", verif_comp.get_root_abstract_name(), agent_id));
               end
               add_vc_child(verif_comp);
            end
         end
      end
   endfunction : build_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Function: connect_phase
   //
   virtual function void connect_phase(uvm_phase phase);
      super.connect_phase(phase);

      //
      // Connect ETLink Bus agent ports to each core dcache scoreboard:
      // note:  m_neigh_miss_req_export, m_neigh_evict_req_export, and m_neigh_ptw_req_export are defined in evl_minion_dcache_scoreboard.sv;
      //
      foreach (m_core_agents[ii]) begin
         if (m_core_bus_miss_agents[ii] != null) begin
            m_core_agents[ii].connect_port_to_my_export(m_core_bus_miss_agents[ii].get_req_desc_port("m_obs_req_port"), "m_neigh_miss_req_export");
            m_core_agents[ii].connect_port_to_my_export(m_core_bus_miss_agents[ii].get_req_desc_port("m_obs_rsp_port"), "m_neigh_miss_rsp_export");
         end
         if (m_core_bus_evict_agents[ii] != null) begin
            m_core_agents[ii].connect_port_to_my_export(m_core_bus_evict_agents[ii].get_req_desc_port("m_obs_req_port"), "m_neigh_evict_req_export");
            m_core_agents[ii].connect_port_to_my_export(m_core_bus_evict_agents[ii].get_req_desc_port("m_obs_rsp_port"), "m_neigh_evict_rsp_export");
         end

         if (m_core_bus_ptw_agents[ii] != null) begin
            m_core_agents[ii].connect_port_to_my_ptw_export(m_core_bus_ptw_agents[ii].get_ptw_desc_port("m_obs_req_port"), "m_neigh_ptw_req_export");
            m_core_agents[ii].connect_port_to_my_ptw_export(m_core_bus_ptw_agents[ii].get_ptw_desc_port("m_obs_rsp_port"), "m_neigh_ptw_rsp_export");
         end
      end
   endfunction : connect_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Function: start_of_simulation_phase
   //
   virtual function void start_of_simulation_phase(uvm_phase phase);
      super.start_of_simulation_phase(phase);

      foreach (m_core_agents[ii]) begin
         if (m_core_bus_miss_agents.exists(ii) && (m_core_bus_miss_agents[ii] != null)) begin
            m_core_bus_miss_agents[ii].set_master_port_agent(m_core_agents[ii]);
         end
         if (m_core_bus_evict_agents.exists(ii) && (m_core_bus_evict_agents[ii] != null)) begin
            m_core_bus_evict_agents[ii].set_master_port_agent(m_core_agents[ii]);
            m_core_bus_evict_agents[ii].set_slave_port_agent(this);
         end

         if (m_core_bus_ptw_agents.exists(ii) && (m_core_bus_ptw_agents[ii] != null)) begin
            m_core_bus_ptw_agents[ii].set_master_port_agent(m_core_agents[ii]);
         end
         if (m_core_bus_dbg_agents.exists(ii) && (m_core_bus_dbg_agents[ii] != null)) begin
            m_core_bus_dbg_agents[ii].set_slave_port_agent(m_core_agents[ii]);
         end
      end

      //
      // Now reset the core ID appropriately.
      //
      set_core_id(evl_core_id_t'(0), 1'b1);
   endfunction : start_of_simulation_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string name_in = "evl_standalone_minion_env", input uvm_component parent_in = null);
      super.new(name_in, parent_in);
      m_agent_type = STANDALONE_MINION_RTL_AGENT;
      add_agent_alias(STANDALONE_MINION_AGENT);
      `evl_log(UVM_HIGH, VMOD_CONFIG, get_abstract_name(), $sformatf("evl_standalone_minion_env: new() is called"))
   endfunction : new

endclass : evl_standalone_minion_env

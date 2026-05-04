//--------------------------------------------------------------------------------------------------
//
// Class: evl_neigh_env
//

class evl_neigh_env extends evl_verif_comp;

   `uvm_component_utils(evl_neigh_env)


   //-----------------------------------------------------------------------------------------------
   //
   // Static Member Variables and Objects
   //
   `evl_param_key(pk_core_req_port_count)
   `evl_param_key(pk_no_error_response)
   `evl_param_key(pk_shire_agent)
   `evl_param_key(pk_shire_channel_agent)


   //-----------------------------------------------------------------------------------------------
   //
   // Member Variables and Objects
   //

   // Neighborhood scoreboard
   evl_neigh_scoreboard m_neigh_scoreboard;

   // Children core RTL agents
   evl_verif_comp       m_core_agents[evl_agent_id_t];
   `ifdef L1SB_EVL_MINION_ENV
      evl_minion_env    my_m_core_agents[evl_agent_id_t];
   `endif

   `ifndef L1SB_EVL_MINION_ENV
      // Core ETLink bus agents
      evl_verif_comp    m_core_bus_miss_agents[evl_agent_id_t];
      evl_verif_comp    m_core_bus_evict_agents[evl_agent_id_t];
      evl_verif_comp    m_core_bus_ptw_agents[evl_agent_id_t];
      evl_verif_comp    m_core_bus_dbg_agents[evl_agent_id_t];
   `endif // ifndef L1SB_EVL_MINION_ENV

   // Core ETLink stim agents
   evl_verif_comp       m_minion_stim_agents[evl_agent_id_t];

   // ESR APB bus agent
   evl_verif_comp       m_apb_esr_bus_agent;

   // Shire agent instantiating this neighborhood agent
   evl_verif_comp       m_shire_agent;

   // Shire channel agent associated with this neighborhood agent
   evl_verif_comp       m_shire_channel_agent;

   // export for observed requests to the downstream shire cache
   uvm_analysis_export #(evl_req_desc) m_shire_req_export;

   // export for observed responses from the downstream shire cache
   uvm_analysis_export #(evl_req_desc) m_shire_rsp_export;

   // Number of stubbed minions
   int m_minion_stub;


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

      // Retrieve minion stub mask from the neigh interface
      m_minion_stub = m_rtl_port.get_int_param_by_name("pk_minion_stub", 0);

      factory = uvm_factory::get();
      evl_rtl_instance_map::get_my_children_list(instance_list, get_full_name());

      foreach (instance_list[ii]) begin
         string inst_name;
         string parent_names[$];

         parent_comp = this;
         inst_name   = evl_base_pkg::extract_parent_list(parent_names, get_full_name(), instance_list[ii].get_full_name());
         //`evl_log(UVM_DEBUG, VMOD_CONFIG, get_abstract_name(), $sformatf("(CYE DEBUG: evl_neigh_env) foreach(instance_list[]) loop: ii=%0d inst_name=%s", ii, inst_name))

         foreach (parent_names[jj]) begin
            string full_parent_name;

            full_parent_name = $sformatf("%0s.%0s", parent_comp.get_full_name(), parent_names[jj]);
            //`evl_log(UVM_DEBUG, VMOD_CONFIG, get_abstract_name(), $sformatf("(CYE DEBUG: evl_neigh_env) foreach(parent_names[]) loop: jj=%0d full_parent_name=%s", jj, full_parent_name))

            if (!parent_comps.exists(full_parent_name)) begin
               parent_comp = evl_verif_comp::type_id::create(parent_names[jj], parent_comp);
               parent_comps[full_parent_name] = parent_comp;
            end
            else begin
               parent_comp = parent_comps[full_parent_name];
            end
         end

         component = factory.create_component_by_name(instance_list[ii].get_sv_comp(), "", inst_name, parent_comp);
         `evl_log(UVM_DEBUG, VMOD_CONFIG, get_abstract_name(), $sformatf("(CYE DEBUG: evl_neigh_env) foreach(instance_list[]) loop: ii=%0d inst_name=%s: create component ...", ii, inst_name))

         if (component != null) begin
            evl_agent_id_t agent_id;

            $cast(verif_comp, component);
            agent_id = instance_list[ii].get_agent_pid();
            verif_comp.set_shire_id(get_shire_id());
            verif_comp.set_neighborhood_id(get_neighborhood_id());
            if (verif_comp.is_bus_vc() == 1'b1) begin

            `ifndef L1SB_EVL_MINION_ENV

               if (verif_comp.get_agent_type() == evl_base_pkg::ETLINK_BUS_AGENT) begin
                  if (instance_list[ii].get_inst_name() == "m_miss_req") begin:CORE_MISS_BUS_AGENT
                     verif_comp.use_my_core_id();
                     verif_comp.set_agent_id(0);
                     verif_comp.set_core_id(agent_id);
                     verif_comp.set_abstract_name($sformatf("C%0d(MISS)", agent_id));
                     m_core_bus_miss_agents[agent_id] = verif_comp;
                     // port_id is made of {minion_id, 0:miss|1:evict}
                     m_core_bus_miss_agents[agent_id].set_port_id((agent_id << 1) | 0);
                     `evl_log(UVM_DEBUG, VMOD_CONFIG, get_abstract_name(), $sformatf("(CYE DEBUG: evl_neigh_env) ETLINK_BUS_AGENT:m_miss_req is found and assigned to m_core_bus_miss_agents[] (agent_id=%0d)", agent_id))

                     //If we stub the minions then the bus agent has to be set as master
                     if (m_minion_stub >0) begin
                        m_core_bus_miss_agents[agent_id].set_active_master();
                     end

                     if (get_int_param(pk_no_error_response, 0) != 0) begin
                        verif_comp.set_int_param(pk_no_error_response, 1);
                     end
                  end:CORE_MISS_BUS_AGENT
                  else if (instance_list[ii].get_inst_name() == "m_evict_req") begin:CORE_EVICT_BUS_AGENT
                     verif_comp.use_my_core_id();
                     verif_comp.set_agent_id(1);
                     verif_comp.set_core_id(agent_id);
                     verif_comp.set_abstract_name($sformatf("C%0d(EVICT)", agent_id));
                     m_core_bus_evict_agents[agent_id] = verif_comp;
                     // port_id is made of {minion_id, 0:miss|1:evict}
                     m_core_bus_evict_agents[agent_id].set_port_id((agent_id << 1) | 1);
                     `evl_log(UVM_DEBUG, VMOD_CONFIG, get_abstract_name(), $sformatf("(CYE DEBUG: evl_neigh_env) ETLINK_BUS_AGENT:m_evict_req is found and assigned to m_core_bus_evict_agents[] (agent_id=%0d)", agent_id))

                     //If we stub the minions then the bus agent has to be set as master
                     if (m_minion_stub >0) begin
                        m_core_bus_evict_agents[agent_id].set_active_master();
                     end

                     if (get_int_param(pk_no_error_response, 0) != 0) begin
                        verif_comp.set_int_param(pk_no_error_response, 1);
                     end
                  end:CORE_EVICT_BUS_AGENT
               end
               else if (verif_comp.get_agent_type() == evl_base_pkg::PTW_BUS_AGENT) begin:PTW_BUS_AGENT
                  verif_comp.set_agent_id(agent_id);
                  if (instance_list[ii].get_inst_name() == "m_ptw_req") begin
                     verif_comp.set_abstract_name($sformatf("C%0d(PTW)", agent_id));
                     m_core_bus_ptw_agents[agent_id] = verif_comp;
                     `evl_log(UVM_DEBUG, VMOD_CONFIG, get_abstract_name(), $sformatf("(CYE DEBUG: evl_neigh_env) PTW_BUS_AGENT:m_ptw_req is found and assigned to m_core_bus_ptw_agents[] (agent_id=%0d)", agent_id))
                  end
               end:PTW_BUS_AGENT
               else if (verif_comp.get_agent_type() == evl_base_pkg::APB_BUS_AGENT) begin:APB_BUS_AGENT
                  if (instance_list[ii].get_inst_name() == "m_debug_itf") begin
                     verif_comp.set_agent_id(agent_id);
                     verif_comp.set_abstract_name($sformatf("C%0d(DBG)", agent_id));
                     m_core_bus_dbg_agents[agent_id] = verif_comp;
                     `evl_log(UVM_DEBUG, VMOD_CONFIG, get_abstract_name(), $sformatf("(CYE DEBUG: evl_neigh_env) APB_BUS_AGENT:m_debug_itf is found and assigned to m_core_bus_dbg_agents[] (agent_id=%0d)", agent_id))
                  end
                  else if (instance_list[ii].get_inst_name() == "m_apb_esr") begin
                     verif_comp.set_agent_id(8);
                     verif_comp.set_abstract_name($sformatf("ESR(APB)"));
                     m_apb_esr_bus_agent = verif_comp;
                     `evl_log(UVM_DEBUG, VMOD_CONFIG, get_abstract_name(), $sformatf("(CYE DEBUG: evl_neigh_env) APB_BUS_AGENT:m_apb_esr is found and assigned to m_apb_esr_bus_agent"))
                  end
               end:APB_BUS_AGENT

               add_vc_child(verif_comp);

            `endif // ifndef L1SB_EVL_MINION_ENV

            end
            else begin
               if (verif_comp.get_agent_type() == evl_base_pkg::CORE_RTL_AGENT) begin
                  verif_comp.set_core_id(agent_id);
                  verif_comp.set_agent_id(agent_id);
                  verif_comp.set_abstract_name($sformatf("C%0d", agent_id));
                  m_core_agents[agent_id] = verif_comp;
                  `evl_log(UVM_DEBUG, VMOD_CONFIG, get_abstract_name(), $sformatf("(CYE DEBUG: evl_neigh_env) CORE_RTL_AGENT is found and assigned to m_core_agents[] (agent_id=%0d)", agent_id))
               end
               else if (agent_id != ~evl_agent_id_t'(0)) begin
                  verif_comp.set_agent_id(agent_id);
                  verif_comp.set_abstract_name($sformatf("%0s-%0d", verif_comp.get_root_abstract_name(), agent_id));
               end
               add_vc_child(verif_comp);
            end
         end
      end

      `ifdef L1SB_EVL_MINION_ENV
         for (int i=0; i<`MIN_PER_N; i++) begin
            $cast(my_m_core_agents[i], m_core_agents[i]);
         end
      `endif // ifdef L1SB_EVL_MINION_ENV

      for (int i=0; i<`MIN_PER_N; i++)
      begin
         if (m_minion_stub > 0) begin:CORE_STIM_AGENT
            stim_agent = factory.create_component_by_name("evl_minion_agent_etlink", "", $sformatf("core%0d_stim_agent", i), this);
            if (stim_agent != null) begin
               $cast(verif_comp, stim_agent);
               m_minion_stim_agents[i] = verif_comp;
               verif_comp.set_agent_id(i);
               verif_comp.set_abstract_name($sformatf("C%0d(STIM)", i));
               verif_comp.set_shire_id(get_shire_id());
               //
               // Assign handles for the RTL ports of both ET link bus agents to the stimulus agent.
               //
               `ifdef L1SB_EVL_MINION_ENV
                  //$cast(my_m_core_agents[i], m_core_agents[i]);
                  verif_comp.set_rtl_port(my_m_core_agents[i].m_core_bus_miss_agents.get_rtl_port(), 0);
                  verif_comp.set_rtl_port(my_m_core_agents[i].m_core_bus_evict_agents.get_rtl_port(), 1);
               `else
                  verif_comp.set_rtl_port(m_core_bus_miss_agents[i].get_rtl_port(), 0);
                  verif_comp.set_rtl_port(m_core_bus_evict_agents[i].get_rtl_port(), 1);
               `endif // ifdef L1SB_EVL_MINION_ENV

               //
               // Notify the ET link bus agents that they are now active masters.
               //
               `ifdef L1SB_EVL_MINION_ENV
                  //$cast(my_m_core_agents[i], m_core_agents[i]);
                  my_m_core_agents[i].m_core_bus_miss_agents.set_active_master();
                  my_m_core_agents[i].m_core_bus_evict_agents.set_active_master();
               `else
                  m_core_bus_miss_agents[i].set_active_master();
                  m_core_bus_evict_agents[i].set_active_master();
               `endif // ifdef L1SB_EVL_MINION_ENV

               add_vc_child(verif_comp);
            end
         end:CORE_STIM_AGENT
      end

      m_shire_req_export = new("m_shire_req_export", this);
      m_shire_rsp_export = new("m_shire_rsp_export", this);

      //
      // Create the neighbourhood scoreboard
      //
      m_neigh_scoreboard = evl_neigh_scoreboard::type_id::create("m_neigh_scoreboard", this);
      m_neigh_scoreboard.set_agent(this);
      m_neigh_scoreboard.set_shire_id(get_shire_id());
      m_neigh_scoreboard.set_neighborhood_id(get_neighborhood_id());
      m_neigh_scoreboard.set_rtl_port(m_rtl_port);
      m_neigh_scoreboard.set_abstract_name($sformatf("%0s(NeighSB)", get_abstract_name()));
      `ifdef L1SB_EVL_MINION_ENV
         m_neigh_scoreboard.set_int_param(pk_core_req_port_count, m_core_agents.size());
      `else
         m_neigh_scoreboard.set_int_param(pk_core_req_port_count, m_core_bus_miss_agents.size());
      `endif // ifdef L1SB_EVL_MINION_ENV
   endfunction : build_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Function: connect_phase
   //
   virtual function void connect_phase(uvm_phase phase);
      super.connect_phase(phase);

      // Connect stimulus agents to the actual bus agents that drive the stimulus to the DUT
      //FUTURE Chech m_minion_stub[ii] and connect to the TLM port only then
      if (m_minion_stub > 0) begin
         foreach (m_minion_stim_agents[i]) begin
            `ifdef L1SB_EVL_MINION_ENV
               //$cast(my_m_core_agents[i], m_core_agents[i]);
               m_minion_stim_agents[i].connect_my_bus_ports(my_m_core_agents[i].m_core_bus_miss_agents,  "", 0);
               m_minion_stim_agents[i].connect_my_bus_ports(my_m_core_agents[i].m_core_bus_evict_agents, "", 1);
               m_minion_stim_agents[i].connect_my_bus_ports(my_m_core_agents[i].m_core_bus_miss_agents,  "", 2);
            `else
               m_minion_stim_agents[i].connect_my_bus_ports(m_core_bus_miss_agents[i],  "", 0);
               m_minion_stim_agents[i].connect_my_bus_ports(m_core_bus_evict_agents[i], "", 1);
               m_minion_stim_agents[i].connect_my_bus_ports(m_core_bus_miss_agents[i],  "", 2);
            `endif // ifdef L1SB_EVL_MINION_ENV
         end
      end

      //
      // Connect ETLink Bus agent ports to each core dcache scoreboard (and to the neigh scoreboard)
      //
      `ifndef L1SB_EVL_MINION_ENV
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
               // FUTURE: Emmanuel Marie use port wrappers to avoid having separate connect methods
               m_core_agents[ii].connect_port_to_my_ptw_export(m_core_bus_ptw_agents[ii].get_ptw_desc_port("m_obs_req_port"), "m_neigh_ptw_req_export");
               m_core_agents[ii].connect_port_to_my_ptw_export(m_core_bus_ptw_agents[ii].get_ptw_desc_port("m_obs_rsp_port"), "m_neigh_ptw_rsp_export");
               // m_core_bus_ptw_agents[ii].get_ptw_desc_port("m_obs_req_port").connect(m_neigh_scoreboard.get_req_desc_export("m_core_ptw_req_export", ii));
               // m_core_bus_ptw_agents[ii].get_ptw_desc_port("m_obs_rsp_port").connect(m_neigh_scoreboard.get_req_desc_export("m_core_ptw_rsp_export", ii));
            end
         end
      `endif // ifndef L1SB_EVL_MINION_ENV

      //
      // Connect shire side exports to the neigh scoreboard exports
      //
      m_shire_req_export.connect(m_neigh_scoreboard.get_req_desc_export("m_shire_req_export"));
      m_shire_rsp_export.connect(m_neigh_scoreboard.get_req_desc_export("m_shire_rsp_export"));

      foreach(m_core_agents[ii]) begin
         m_core_agents[ii].get_pmu_desc_port("m_obs_req_port").connect(m_neigh_scoreboard.get_pmu_desc_export("m_core_pmu_req_export"));
      end

      // Connect Miss/Evict interface to neigh scoreboard
      `ifdef L1SB_EVL_MINION_ENV
         foreach(m_core_agents[i]) begin
            my_m_core_agents[i].m_core_bus_miss_agents.get_req_desc_port("m_obs_req_port").connect(m_neigh_scoreboard.get_req_desc_export("m_core_miss_req_export", i));
            my_m_core_agents[i].m_core_bus_miss_agents.get_req_desc_port("m_obs_rsp_port").connect(m_neigh_scoreboard.get_req_desc_export("m_core_miss_rsp_export", i));
            my_m_core_agents[i].m_core_bus_evict_agents.get_req_desc_port("m_obs_req_port").connect(m_neigh_scoreboard.get_req_desc_export("m_core_evict_req_export", i));
            my_m_core_agents[i].m_core_bus_evict_agents.get_req_desc_port("m_obs_rsp_port").connect(m_neigh_scoreboard.get_req_desc_export("m_core_evict_rsp_export", i));
         end
      `else
         foreach(m_core_bus_miss_agents[i]) begin
            m_core_bus_miss_agents[i].get_req_desc_port("m_obs_req_port").connect(m_neigh_scoreboard.get_req_desc_export("m_core_miss_req_export", i));
            m_core_bus_miss_agents[i].get_req_desc_port("m_obs_rsp_port").connect(m_neigh_scoreboard.get_req_desc_export("m_core_miss_rsp_export", i));
         end
         foreach(m_core_bus_evict_agents[i]) begin
            m_core_bus_evict_agents[i].get_req_desc_port("m_obs_req_port").connect(m_neigh_scoreboard.get_req_desc_export("m_core_evict_req_export", i));
            m_core_bus_evict_agents[i].get_req_desc_port("m_obs_rsp_port").connect(m_neigh_scoreboard.get_req_desc_export("m_core_evict_rsp_export", i));
         end
      `endif // ifdef L1SB_EVL_MINION_ENV
   endfunction : connect_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Function: start_of_simulation_phase
   //
   virtual function void start_of_simulation_phase(uvm_phase phase);
      uvm_void parent_agent;

      super.start_of_simulation_phase(phase);

      foreach (m_core_agents[ii]) begin
         `ifdef L1SB_EVL_MINION_ENV
            if ((my_m_core_agents[ii].m_core_bus_miss_agents != null)) begin
               my_m_core_agents[ii].m_core_bus_miss_agents.set_master_port_agent(my_m_core_agents[ii]);
            end
            if ((my_m_core_agents[ii].m_core_bus_evict_agents != null)) begin
               my_m_core_agents[ii].m_core_bus_evict_agents.set_master_port_agent(my_m_core_agents[ii]);
               my_m_core_agents[ii].m_core_bus_evict_agents.set_slave_port_agent(this);
            end
            if ((my_m_core_agents[ii].m_core_bus_ptw_agents != null)) begin
               my_m_core_agents[ii].m_core_bus_ptw_agents.set_master_port_agent(my_m_core_agents[ii]);
            end
            if ((my_m_core_agents[ii].m_core_bus_dbg_agents != null)) begin
               my_m_core_agents[ii].m_core_bus_dbg_agents.set_slave_port_agent(my_m_core_agents[ii]);
            end
         `else
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
         `endif // ifdef L1SB_EVL_MINION_ENV
      end

      parent_agent = get_object_param(pk_shire_agent);
      if (parent_agent == null) begin
         m_shire_agent = null;
      end
      else begin
         $cast(m_shire_agent, parent_agent);
      end

      parent_agent = get_object_param(pk_shire_channel_agent);
      if (parent_agent == null) begin
         m_shire_channel_agent = null;
      end
      else begin
         $cast(m_shire_channel_agent, parent_agent);
      end
   endfunction : start_of_simulation_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Function: connect_port_to_my_export
   //
   virtual function void connect_port_to_my_export(input uvm_analysis_port#(evl_req_desc) port_in,
                                                   input string                           port_name      = "unknown",
                                                   input int                              port_number    = 0,
                                                   input int                              subport_number = 0);
      if (port_in == null) begin
         return;
      end
      if ((port_name == "m_shire_req_export") || (port_name == "shire_req_export")) begin
         if (m_shire_req_export != null) begin
            `evl_log(UVM_MEDIUM, VMOD_CONFIG, get_abstract_name(), $sformatf("connecting %0s -> %0s", port_in.get_full_name(), m_shire_req_export.get_full_name()))
            port_in.connect(m_shire_req_export);
         end
      end
      else if ((port_name == "m_shire_rsp_export") || (port_name == "shire_rsp_export")) begin
         if (m_shire_rsp_export != null) begin
            `evl_log(UVM_MEDIUM, VMOD_CONFIG, get_abstract_name(), $sformatf("connecting %0s -> %0s", port_in.get_full_name(), m_shire_rsp_export.get_full_name()))
            port_in.connect(m_shire_rsp_export);
         end
      end
      return;
   endfunction : connect_port_to_my_export


   //-----------------------------------------------------------------------------------------------
   //
   // Function: is_req_checked
   //
   //    A bus VC might ask a scoreboard if requests are to be checked.
   //    Useful for scoreboards that don't model the whole behaviour of a block, like neigh SB
   //    which needs to exclude request checking based on source (icache/ptw), or is a scoreboard
   //    is disabled
   //
   virtual function bit is_req_checked(input evl_req_desc req_desc_in);
      return m_neigh_scoreboard.is_req_checked(req_desc_in);
   endfunction : is_req_checked


   //-----------------------------------------------------------------------------------------------
   //
   // Function: is_rsp_checked
   //
   //    A bus VC might ask a scoreboard if responses are to be checked.
   //
   virtual function bit is_rsp_checked(input evl_req_desc req_desc_in);
      return m_neigh_scoreboard.is_rsp_checked(req_desc_in);
   endfunction : is_rsp_checked


   //-----------------------------------------------------------------------------------------------
   //
   // Function: find_observed_request
   //
   // This function is used by ET link bus agents to find parent message request descriptors for
   // received requests.
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
      if (m_neigh_scoreboard == null) begin
         return null;
      end
      return m_neigh_scoreboard.find_observed_request(port_type_in, port_in, subport_in, paired_req_desc, bus_cmd_in, address_in, req_id_in, source_id_in, trans_id_in);
   endfunction : find_observed_request


   //-----------------------------------------------------------------------------------------------
   //
   // Function: find_observed_response
   //
   // This function is used by ET link bus agents to find parent message request descriptors for
   // received message responses.
   //
   virtual function evl_req_desc find_observed_response(input int                 port_in,
                                                        input evl_req_desc        paired_req_desc,
                                                        input bit                 compare_fields = 1'b0,
                                                        input byte                qualifier      = 0,
                                                        input evl_bus_rsp_type_t  rsp_type_in    = `EVL_Rsp_Null,
                                                        input evl_bus_req_id_t    req_id_in      = ~evl_bus_req_id_t'(0),
                                                        input evl_bus_rsp_dest_t  dest_id_in     = ~evl_bus_rsp_dest_t'(0),
                                                        input evl_xword_byte_en_t rsp_byte_en_in = evl_xword_byte_en_t'(0),
                                                        input evl_xword_t         rsp_data_in    = evl_xword_t'(0));
      if (m_neigh_scoreboard == null) begin
         return null;
      end
      return m_neigh_scoreboard.find_observed_response(port_in, paired_req_desc, compare_fields, qualifier, rsp_type_in, req_id_in, dest_id_in, rsp_byte_en_in, rsp_data_in);
   endfunction : find_observed_response


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_cache_tag
   //
   virtual function evl_paddr_t get_cache_tag(input int cindex,
                                              input int cway  = 0,
                                              input int ctype = 0);
      if (evl_cache_ram_type_t'(ctype) inside { SC_RAM_TYPE_ICACHE_TAG, SC_RAM_TYPE_ICACHE0_TAG, SC_RAM_TYPE_ICACHE1_TAG, SC_RAM_TYPE_ICACHE2_TAG, SC_RAM_TYPE_ICACHE3_TAG }) begin
         if (m_rtl_port == null) begin
            return ~evl_paddr_t'(0);
         end
         return m_rtl_port.get_cache_tag(cindex, cway, ctype);
      end
      if (m_shire_agent == null) begin
         return ~evl_paddr_t'(0);
      end
      return m_shire_agent.get_cache_tag(cindex, cway, ctype);
   endfunction : get_cache_tag


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_cache_data
   //
   virtual function int get_cache_data(output evl_dword_t cdata[],
                                       input  int         cindex,
                                       input  int         cway  = 0,
                                       input  int         ctype = 0);
      if (m_shire_agent == null) begin
         cdata = new[`EVL_LINE_DWORDS];
         for (int ii = 0; ii < `EVL_LINE_DWORDS; ii++) begin
            cdata[ii] = ~evl_dword_t'(0);
         end
         return -1;
      end
      if (evl_cache_ram_type_t'(ctype) inside { SC_RAM_TYPE_ICACHE_DATA, SC_RAM_TYPE_ICACHE0_DATA, SC_RAM_TYPE_ICACHE1_DATA, SC_RAM_TYPE_ICACHE2_DATA, SC_RAM_TYPE_ICACHE3_DATA }) begin
         case (get_neighborhood_id())
            0: return m_shire_agent.get_cache_data(cdata, cindex, cway, SC_RAM_TYPE_ICACHE0_DATA);
            1: return m_shire_agent.get_cache_data(cdata, cindex, cway, SC_RAM_TYPE_ICACHE1_DATA);
            2: return m_shire_agent.get_cache_data(cdata, cindex, cway, SC_RAM_TYPE_ICACHE2_DATA);
            3: return m_shire_agent.get_cache_data(cdata, cindex, cway, SC_RAM_TYPE_ICACHE3_DATA);
         endcase
         cdata = new[`EVL_LINE_DWORDS];
         for (int ii = 0; ii < `EVL_LINE_DWORDS; ii++) begin
            cdata[ii] = ~evl_dword_t'(0);
         end
         return -1;
      end
      return m_shire_agent.get_cache_data(cdata, cindex, cway, ctype);
   endfunction : get_cache_data


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_cache_bit_err
   //
   virtual function int set_cache_bit_err(input int cindex,
                                          input int cbit0,
                                          input int cbit1 = -1,
                                          input int cway  = 0,
                                          input int ctype = 0,
                                          input int cerr  = int'(EVL_ECC_ERR_EPHEMERAL));
      if (m_shire_agent == null) begin
         return -1;
      end
      if (evl_cache_ram_type_t'(ctype) inside { SC_RAM_TYPE_ICACHE_DATA, SC_RAM_TYPE_ICACHE0_DATA, SC_RAM_TYPE_ICACHE1_DATA, SC_RAM_TYPE_ICACHE2_DATA, SC_RAM_TYPE_ICACHE3_DATA }) begin
         case (get_neighborhood_id())
            0: return m_shire_agent.set_cache_bit_err(cindex, cbit0, cbit1, cway, SC_RAM_TYPE_ICACHE0_DATA, cerr);
            1: return m_shire_agent.set_cache_bit_err(cindex, cbit0, cbit1, cway, SC_RAM_TYPE_ICACHE1_DATA, cerr);
            2: return m_shire_agent.set_cache_bit_err(cindex, cbit0, cbit1, cway, SC_RAM_TYPE_ICACHE2_DATA, cerr);
            3: return m_shire_agent.set_cache_bit_err(cindex, cbit0, cbit1, cway, SC_RAM_TYPE_ICACHE3_DATA, cerr);
         endcase
         return -1;
      end
      return m_shire_agent.set_cache_bit_err(cindex, cbit0, cbit1, cway, ctype, cerr);
   endfunction : set_cache_bit_err


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string name_in = "evl_neigh_env", input uvm_component parent_in = null);
      super.new(name_in, parent_in);
      m_agent_type = NEIGH_RTL_AGENT;
      add_agent_alias(NEIGH_AGENT);
      if (`et_test_plusargs("noNeighErrorResponse") != 0) begin
         set_int_param(pk_no_error_response, 1);
      end
   endfunction : new

endclass : evl_neigh_env

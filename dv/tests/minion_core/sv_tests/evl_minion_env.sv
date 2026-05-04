//--------------------------------------------------------------------------------------------------
//
// Class: evl_minion_env
//
class evl_minion_env extends evl_verif_comp;

   `uvm_component_utils(evl_minion_env)


   //-----------------------------------------------------------------------------------------------
   //
   // Static Member Variables and Objects
   //


   //-----------------------------------------------------------------------------------------------
   //
   // Member Variables and Objects
   //

   // Dcache scoreboard
   evl_minion_l1d_scoreboard m_minion_l1d_scoreboard;
   evl_minion_fe_scoreboard  m_minion_fe_scoreboard;
   evl_minion_pmu_monitor    m_minion_pmu_monitor;

   // export for observed requests to the downstream neighbourhood
   uvm_analysis_export #(evl_req_desc) m_neigh_miss_req_export;
   uvm_analysis_export #(evl_req_desc) m_neigh_evict_req_export;
   uvm_analysis_export #(evl_ptw_desc) m_neigh_ptw_req_export;

   // export for observed responses from the downstream neighbourhood
   uvm_analysis_export #(evl_req_desc) m_neigh_miss_rsp_export;
   uvm_analysis_export #(evl_req_desc) m_neigh_evict_rsp_export;
   uvm_analysis_export #(evl_ptw_desc) m_neigh_ptw_rsp_export;

   uvm_analysis_export #(evl_pmu_desc) m_pmu_req_export;
   uvm_analysis_export #(evl_pmu_desc) m_pmu_rsp_export;

`ifdef L1SB_EVL_MINION_ENV
   // (Charles) new: testing ...
   // Core ETLink bus agents
   evl_verif_comp       m_core_bus_miss_agents;   // actually only one set for each minion 
   evl_verif_comp       m_core_bus_evict_agents;
   evl_verif_comp       m_core_bus_ptw_agents;
   evl_verif_comp       m_core_bus_dbg_agents;
`endif


   //-----------------------------------------------------------------------------------------------
   //
   // Function: build_phase
   //
   virtual function void build_phase(uvm_phase phase);
      uvm_factory         factory;
      uvm_component       component;
      evl_verif_comp      verif_comp;
      evl_rtl_instance    instance_list[];
      evl_verif_comp      parent_comp;
      evl_verif_comp      parent_comps[string];
      evl_minion_rtl_port minion_rtl_port;

      super.build_phase(phase);

      factory = uvm_factory::get();
      evl_rtl_instance_map::get_my_children_list(instance_list, get_full_name());

      foreach (instance_list[ii]) begin
         string inst_name;
         string parent_names[$];

         parent_comp = this;
         inst_name   = evl_base_pkg::extract_parent_list(parent_names, get_full_name(), instance_list[ii].get_full_name());
         //`evl_log(UVM_DEBUG, VMOD_CONFIG, get_abstract_name(), $sformatf("(CYE DEBUG: evl_minion_env) foreach(instance_list[]) loop: ii=%0d inst_name=%s", ii, inst_name))

         foreach (parent_names[jj]) begin
            string full_parent_name;

            full_parent_name = $sformatf("%0s.%0s", parent_comp.get_full_name(), parent_names[jj]);
            //`evl_log(UVM_DEBUG, VMOD_CONFIG, get_abstract_name(), $sformatf("(CYE DEBUG: evl_minion_env) foreach(parent_names[]) loop: jj=%0d full_parent_name=%s", jj, full_parent_name))

            if (!parent_comps.exists(full_parent_name)) begin
               parent_comp = evl_verif_comp::type_id::create(parent_names[jj], parent_comp);
               parent_comps[full_parent_name] = parent_comp;
            end
            else begin
               parent_comp = parent_comps[full_parent_name];
            end
         end

         component = factory.create_component_by_name(instance_list[ii].get_sv_comp(), "", inst_name, parent_comp);
         //`evl_log(UVM_DEBUG, VMOD_CONFIG, get_abstract_name(), $sformatf("(CYE DEBUG: evl_minion_env) foreach(instance_list[]) loop: ii=%0d inst_name=%s: create component ...", ii, inst_name))

         if (component != null) begin
            evl_agent_id_t agent_id;

            $cast(verif_comp, component);
            agent_id = instance_list[ii].get_agent_pid();
            verif_comp.set_shire_id(get_shire_id());
            verif_comp.set_neighborhood_id(get_neighborhood_id());
            verif_comp.set_core_id(get_core_id());

            `evl_log(UVM_DEBUG, VMOD_CONFIG, get_abstract_name(), $sformatf("(CYE DEBUG: evl_minion_env) foreach(instance_list[]) loop: ii=%0d inst_name=%s: (after create component) after cast to verif_comp: is_bus_vc()=%0b get_agent_type()=%s", ii, inst_name, verif_comp.is_bus_vc(), verif_comp.get_agent_type() ))


`ifdef L1SB_EVL_MINION_ENV

            // (Charles) new: testing ...
            if (verif_comp.is_bus_vc() == 1'b1) begin 
               if (verif_comp.get_agent_type() == ETLINK_BUS_AGENT) begin
                  //`evl_log(UVM_DEBUG, VMOD_CONFIG, get_abstract_name(), $sformatf("(CYE DEBUG: evl_minion_env) ETLINK_BUS_AGENT is found"))
                  verif_comp.set_agent_id(get_shire_id()); 

                  if (instance_list[ii].get_inst_name() == "m_miss_req") begin: CORE_MISS_BUS_AGENT
                     //verif_comp.set_abstract_name($sformatf("C%0d(MISS)", agent_id));
                     verif_comp.set_abstract_name($sformatf("(MISS)"));
                     m_core_bus_miss_agents = verif_comp;
                     m_core_bus_miss_agents.set_port_id(0);
                     `evl_log(UVM_DEBUG, VMOD_CONFIG, get_abstract_name(), $sformatf("(CYE DEBUG: evl_minion_env) ETLINK_BUS_AGENT:m_miss_req (%s) is found and assigned to m_core_bus_miss_agents", verif_comp.get_abstract_name() ))

                     ////If we stub the minions then the bus agent has to be set as master
                     //if (m_minion_stub >0) begin
                     //   m_core_bus_miss_agents.set_active_master();
                     //end
                  end: CORE_MISS_BUS_AGENT

                  else if (instance_list[ii].get_inst_name() == "m_evict_req") begin: CORE_EVICT_BUS_AGENT
                     //verif_comp.set_abstract_name($sformatf("C%0d(EVICT)", agent_id));
                     verif_comp.set_abstract_name($sformatf("(EVICT)"));
                     m_core_bus_evict_agents = verif_comp;
                     m_core_bus_evict_agents.set_port_id(1);
                     `evl_log(UVM_DEBUG, VMOD_CONFIG, get_abstract_name(), $sformatf("(CYE DEBUG: evl_minion_env) ETLINK_BUS_AGENT:m_evict_req (%s) is found and assigned to m_core_bus_evict_agents", verif_comp.get_abstract_name() ))

                     ////If we stub the minions then the bus agent has to be set as master
                     //if (m_minion_stub >0) begin
                     //   m_core_bus_evict_agents.set_active_master();
                     //end
                  end: CORE_EVICT_BUS_AGENT
               end

               else if (verif_comp.get_agent_type() == PTW_BUS_AGENT) begin: PTW_BUS_AGENT
                  verif_comp.set_agent_id(agent_id);
                  if (instance_list[ii].get_inst_name() == "m_ptw_req") begin
                     //verif_comp.set_abstract_name($sformatf("C%0d(PTW)", agent_id));
                     verif_comp.set_abstract_name($sformatf("(PTW)"));
                     m_core_bus_ptw_agents = verif_comp;
                     `evl_log(UVM_DEBUG, VMOD_CONFIG, get_abstract_name(), $sformatf("(CYE DEBUG: evl_minion_env) PTW_BUS_AGENT:m_ptw_req (%s) is found and assigned to m_core_bus_ptw_agents", verif_comp.get_abstract_name() ))
                  end
               end: PTW_BUS_AGENT

               else if (verif_comp.get_agent_type() == APB_BUS_AGENT) begin: APB_BUS_AGENT
                  if (instance_list[ii].get_inst_name() == "m_debug_itf") begin
                     verif_comp.set_agent_id(agent_id);
                     //verif_comp.set_abstract_name($sformatf("C%0d(DBG)", agent_id));
                     verif_comp.set_abstract_name($sformatf("(DBG(APB))"));
                     m_core_bus_dbg_agents = verif_comp;
                     `evl_log(UVM_DEBUG, VMOD_CONFIG, get_abstract_name(), $sformatf("(CYE DEBUG: evl_minion_env) APB_BUS_AGENT:m_debug_itf (%s) is found and assigned to m_core_bus_dbg_agents", verif_comp.get_abstract_name() ))
                  end
                  //else if (instance_list[ii].get_inst_name() == "m_apb_esr") begin
                  //   verif_comp.set_agent_id(~evl_agent_id_t'(0));
                  //   verif_comp.set_abstract_name($sformatf("ESR(APB)"));
                  //   m_apb_esr_bus_agent = verif_comp;
                  //end
               end: APB_BUS_AGENT

            end
            //`evl_log(UVM_DEBUG, VMOD_CONFIG, get_abstract_name(), $sformatf("(CYE DEBUG: evl_minion_env) BUS_AGENT: verif_comp.get_abstract_name()=%s", verif_comp.get_abstract_name() ))

`else   // ifndef L1SB_EVL_MINION_ENV

            if (agent_id != ~evl_agent_id_t'(0)) begin
               verif_comp.set_agent_id(agent_id);
               verif_comp.set_abstract_name($sformatf("%0s-%0d", verif_comp.get_root_abstract_name(), agent_id));
            end

`endif  // ifdef L1SB_EVL_MINION_ENV ... else ...

            add_vc_child(verif_comp);
         end
      end

      m_neigh_miss_req_export  = new("m_neigh_miss_req_export",  this);
      m_neigh_miss_rsp_export  = new("m_neigh_miss_rsp_export",  this);
      m_neigh_evict_req_export = new("m_neigh_evict_req_export", this);
      m_neigh_evict_rsp_export = new("m_neigh_evict_rsp_export", this);
      m_neigh_ptw_req_export   = new("m_neigh_ptw_req_export", this);
      m_neigh_ptw_rsp_export   = new("m_neigh_ptw_rsp_export", this);

      //
      // Create the dcache scoreboard
      //
      m_minion_l1d_scoreboard = evl_minion_l1d_scoreboard::type_id::create("m_minion_l1d_scoreboard", this);
      m_minion_l1d_scoreboard.set_agent(this);
      m_minion_l1d_scoreboard.set_shire_id(get_shire_id());
      m_minion_l1d_scoreboard.set_neighborhood_id(get_neighborhood_id());
      m_minion_l1d_scoreboard.set_core_id(get_core_id());
      m_minion_l1d_scoreboard.set_rtl_port(m_rtl_port);
      m_minion_l1d_scoreboard.set_abstract_name($sformatf("%0s(L1SB)", get_abstract_name()));
      

      //
      // Create the fe scoreboard
      //
      m_minion_fe_scoreboard = evl_minion_fe_scoreboard::type_id::create("m_minion_fe_scoreboard", this);
      m_minion_fe_scoreboard.set_agent(this);
      m_minion_fe_scoreboard.set_shire_id(get_shire_id());
      m_minion_fe_scoreboard.set_neighborhood_id(get_neighborhood_id());
      m_minion_fe_scoreboard.set_core_id(get_core_id());
      m_minion_fe_scoreboard.set_rtl_port(m_rtl_port);
      m_minion_fe_scoreboard.set_abstract_name($sformatf("%0s(FESB)", get_abstract_name()));

      //
      // Create the PMU monitor
      //
      m_minion_pmu_monitor = evl_minion_pmu_monitor::type_id::create("m_minion_pmu_monitor", this);
      m_minion_pmu_monitor.set_rtl_port(m_rtl_port);
      m_minion_pmu_monitor.set_abstract_name($sformatf("%0s(PMU_MON)", get_abstract_name()));


      $cast(minion_rtl_port, m_rtl_port);
      minion_rtl_port.set_shire_id(get_shire_id());
      minion_rtl_port.set_neighborhood_id(get_neighborhood_id());
      minion_rtl_port.set_core_id(get_core_id());

   endfunction : build_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Function: connect_phase
   //
   virtual function void connect_phase(uvm_phase phase);
      super.connect_phase(phase);

      //
      // Connect neigh side exports to the dcache scoreboard exports
      //
      m_neigh_miss_req_export.connect(m_minion_l1d_scoreboard.get_req_desc_export("m_neigh_miss_req_export"));
      m_neigh_miss_rsp_export.connect(m_minion_l1d_scoreboard.get_req_desc_export("m_neigh_miss_rsp_export"));
      m_neigh_evict_req_export.connect(m_minion_l1d_scoreboard.get_req_desc_export("m_neigh_evict_req_export"));
      m_neigh_evict_rsp_export.connect(m_minion_l1d_scoreboard.get_req_desc_export("m_neigh_evict_rsp_export"));
      m_neigh_ptw_req_export.connect(m_minion_l1d_scoreboard.get_ptw_desc_export("m_neigh_ptw_req_export"));
      m_neigh_ptw_rsp_export.connect(m_minion_l1d_scoreboard.get_ptw_desc_export("m_neigh_ptw_rsp_export"));


`ifdef L1SB_EVL_MINION_ENV
      //
      // (Charles) new: testing ...???
      // connect m_core_bus_xxx_agents port to m_neigh_xxx_{req|rsp}_export: 
      //
      //foreach(m_core_bus_miss_agents[ii]) begin
         //m_core_bus_miss_agents.m_obs_req_port.connect(m_neigh_miss_req_export);
         //m_core_bus_miss_agents.m_obs_rsp_port.connect(m_neigh_miss_rsp_export);
         m_core_bus_miss_agents.get_req_desc_port("m_obs_req_port").connect(m_neigh_miss_req_export);
         m_core_bus_miss_agents.get_req_desc_port("m_obs_rsp_port").connect(m_neigh_miss_rsp_export);
      //end
      //foreach(m_core_bus_evict_agents[ii]) begin
         //m_core_bus_evict_agents.m_obs_req_port.connect(m_neigh_evict_req_export);
         //m_core_bus_evict_agents.m_obs_rsp_port.connect(m_neigh_evict_rsp_export);
         m_core_bus_evict_agents.get_req_desc_port("m_obs_req_port").connect(m_neigh_evict_req_export);
         m_core_bus_evict_agents.get_req_desc_port("m_obs_rsp_port").connect(m_neigh_evict_rsp_export);
      //end
      //foreach(m_core_bus_ptw_agents[ii]) begin
         //m_core_bus_ptw_agents.m_obs_req_port.connect(m_neigh_ptw_req_export);
         //m_core_bus_ptw_agents.m_obs_rsp_port.connect(m_neigh_ptw_rsp_export);
         m_core_bus_ptw_agents.get_ptw_desc_port("m_obs_req_port").connect(m_neigh_ptw_req_export);
         m_core_bus_ptw_agents.get_ptw_desc_port("m_obs_rsp_port").connect(m_neigh_ptw_rsp_export);
      //end
`endif // ifdef L1SB_EVL_MINION_ENV


   endfunction : connect_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Function: connect_port_to_my_export
   //
   virtual function void connect_port_to_my_export(input uvm_analysis_port#(evl_req_desc) port_in,
                                                   input string                           port_name      = "unknown",
                                                   input int                              port_number    = 0,
                                                   input int                              subport_number = 0);
      if (port_in == null) return;

      if ((port_name == "m_neigh_miss_req_export") || (port_name == "neigh_miss_req_export")) begin
         if (m_neigh_miss_req_export != null) begin
            `evl_log(UVM_MEDIUM, VMOD_CONFIG, get_abstract_name(), $sformatf("connecting %0s -> %0s", port_in.get_full_name(), m_neigh_miss_req_export.get_full_name()))
            port_in.connect(m_neigh_miss_req_export);
         end
      end
      else if ((port_name == "m_neigh_miss_rsp_export") || (port_name == "neigh_miss_rsp_export")) begin
         if (m_neigh_miss_rsp_export != null) begin
            `evl_log(UVM_MEDIUM, VMOD_CONFIG, get_abstract_name(), $sformatf("connecting %0s -> %0s", port_in.get_full_name(), m_neigh_miss_rsp_export.get_full_name()))
            port_in.connect(m_neigh_miss_rsp_export);
         end
      end
      if ((port_name == "m_neigh_evict_req_export") || (port_name == "neigh_evict_req_export")) begin
         if (m_neigh_evict_req_export != null) begin
            `evl_log(UVM_MEDIUM, VMOD_CONFIG, get_abstract_name(), $sformatf("connecting %0s -> %0s", port_in.get_full_name(), m_neigh_evict_req_export.get_full_name()))
            port_in.connect(m_neigh_evict_req_export);
         end
      end
      else if ((port_name == "m_neigh_evict_rsp_export") || (port_name == "neigh_evict_rsp_export")) begin
         if (m_neigh_evict_rsp_export != null) begin
            `evl_log(UVM_MEDIUM, VMOD_CONFIG, get_abstract_name(), $sformatf("connecting %0s -> %0s", port_in.get_full_name(), m_neigh_evict_rsp_export.get_full_name()))
            port_in.connect(m_neigh_evict_rsp_export);
         end
      end
      return;
   endfunction : connect_port_to_my_export


   //-----------------------------------------------------------------------------------------------
   //
   // Function: connect_port_to_my_ptw_export
   //
   virtual function void connect_port_to_my_ptw_export(input uvm_analysis_port#(evl_ptw_desc) port_in,
                                                       input string                           port_name   = "unknown",
                                                       input int                              port_number = 0);
      if (port_in == null) begin
         `evl_log(UVM_MEDIUM, VMOD_CONFIG, get_abstract_name(), $sformatf("Trying to connect a null port"))
         return;
      end

      if ((port_name == "m_neigh_ptw_req_export") || (port_name == "neigh_ptw_req_export")) begin
         if (m_neigh_ptw_req_export != null) begin
            `evl_log(UVM_MEDIUM, VMOD_CONFIG, get_abstract_name(), $sformatf("connecting %0s -> %0s", port_in.get_full_name(), m_neigh_ptw_req_export.get_full_name()))
            port_in.connect(m_neigh_ptw_req_export);
         end
      end
      else if ((port_name == "m_neigh_ptw_rsp_export") || (port_name == "neigh_ptw_rsp_export")) begin
         if (m_neigh_ptw_rsp_export != null) begin
            `evl_log(UVM_MEDIUM, VMOD_CONFIG, get_abstract_name(), $sformatf("connecting %0s -> %0s", port_in.get_full_name(), m_neigh_ptw_rsp_export.get_full_name()))
            port_in.connect(m_neigh_ptw_rsp_export);
         end
      end
      return;
   endfunction : connect_port_to_my_ptw_export


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_pmu_desc_port
   //
   virtual function uvm_analysis_port#(evl_pmu_desc) get_pmu_desc_port(input string port_name      = "unknown",
                                                                       input int    port_number    = 0);
      if ((port_name == "m_obs_req_port") || (port_name == "obs_req_port") || (port_name == "obs_req")) begin
         return m_minion_pmu_monitor.get_pmu_desc_port("m_obs_req_port");
      end

      return null;
   endfunction : get_pmu_desc_port


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
      return m_minion_l1d_scoreboard.is_req_checked(req_desc_in);
   endfunction : is_req_checked


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
      return m_minion_l1d_scoreboard.find_observed_request(port_type_in, port_in, subport_in, paired_req_desc, bus_cmd_in, address_in, req_id_in, source_id_in, trans_id_in);
   endfunction : find_observed_request


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string name_in = "evl_minion_env", input uvm_component parent_in = null);
      super.new(name_in, parent_in);
      m_agent_type = CORE_RTL_AGENT;
      add_agent_alias(CORE_AGENT);
   endfunction : new

endclass : evl_minion_env

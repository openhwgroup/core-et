//--------------------------------------------------------------------------------------------------
//
// Class: evl_shire_example_env
//
class evl_shire_example_env extends evl_verif_comp;

   `uvm_component_utils(evl_shire_example_env)


   //-----------------------------------------------------------------------------------------------
   //
   // Static Member Variables and Objects
   //


   //-----------------------------------------------------------------------------------------------
   //
   // Member Variables and Objects
   //
   evl_verif_comp m_neigh_agents[evl_agent_id_t];

   evl_verif_comp m_neigh_bus_agents[evl_agent_id_t];
   evl_verif_comp m_neigh_stim_agents[evl_agent_id_t];

   evl_verif_comp m_shire_bus_agents [evl_agent_id_t];
   evl_verif_comp m_shire_stim_agents[evl_agent_id_t];


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
            if (verif_comp.is_bus_vc() == 1'b1) begin
               if (verif_comp.get_agent_type() == ETLINK_BUS_AGENT) begin
                  verif_comp.set_agent_id(agent_id);
                  verif_comp.set_abstract_name($sformatf("N%0d(ETLINK)", agent_id));
                  m_neigh_bus_agents[ii] = verif_comp;
               end
               else if (verif_comp.get_agent_type() == AXI_BUS_AGENT) begin
                  verif_comp.set_agent_id(agent_id);
                  verif_comp.set_abstract_name($sformatf("SHIRE-%0d(AXI)", agent_id));
                  m_shire_bus_agents[ii] = verif_comp;
               end

               add_vc_child(verif_comp);

               //
               // For now, create a neighborhood stimulus agent.
               //
               if (verif_comp.get_agent_type() == ETLINK_BUS_AGENT) begin
                  if (instance_list[ii].get_inst_name() == "m_etlink_master") begin
                     verif_comp.set_active_master();
                     component = factory.create_component_by_name("evl_neigh_agent_etlink", "", $sformatf("neighborhood_master_%0d", agent_id), this);
                  end
                  else begin
                     verif_comp.set_active_slave();
                     component = factory.create_component_by_name("evl_l2_agent_etlink", "", $sformatf("neighborhood_slave_%0d", agent_id), this);
                  end
                  if (component != null) begin
                     $cast(verif_comp, component);
                     m_neigh_stim_agents[ii] = verif_comp;
                     verif_comp.set_agent_id(agent_id);
                     verif_comp.set_abstract_name($sformatf("%0s-%0d", verif_comp.get_root_abstract_name(), agent_id));
                     verif_comp.set_rtl_port(m_neigh_bus_agents[ii].get_rtl_port());
                     add_vc_child(verif_comp);
                  end
               end
               //
               // For now, create a shire stimulus agent.
               //
               else if (verif_comp.get_agent_type() == AXI_BUS_AGENT) begin
                  if (instance_list[ii].get_inst_name() == "m_axi_master") begin
                     verif_comp.set_active_master();
                     component = factory.create_component_by_name("evl_l2_master_agent_axi", "", $sformatf("shire_master_%0d", agent_id), this);
                  end
                  else begin
                     verif_comp.set_active_slave();
                     component = factory.create_component_by_name("evl_mesh_slave_agent_axi", "", $sformatf("shire_slave_%0d", agent_id), this);
                  end
                  if (component != null) begin
                     $cast(verif_comp, component);
                     m_shire_stim_agents[ii] = verif_comp;
                     verif_comp.set_agent_id(agent_id);
                     verif_comp.set_abstract_name($sformatf("%0s-%0d", verif_comp.get_root_abstract_name(), agent_id));
                     verif_comp.set_rtl_port(m_shire_bus_agents[ii].get_rtl_port());
                     add_vc_child(verif_comp);
                  end
               end
            end
            else begin
               if (verif_comp.get_agent_type() == NEIGH_RTL_AGENT) begin
                  m_neigh_agents[agent_id] = verif_comp;
                  verif_comp.set_shire_id(get_shire_id());
                  verif_comp.set_neighborhood_id(agent_id);
                  verif_comp.set_abstract_name($sformatf("N%0d", agent_id));
               end
               else if (agent_id != ~evl_agent_id_t'(0)) begin
                  verif_comp.set_shire_id(get_shire_id());
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
      foreach (m_neigh_agents[ii]) begin
         m_neigh_agents[ii].connect_port_to_my_export(m_neigh_bus_agents[ii].get_req_desc_port("m_obs_req_port"), "m_shire_req_export", ii);
         m_neigh_agents[ii].connect_port_to_my_export(m_neigh_bus_agents[ii].get_req_desc_port("m_obs_rsp_port"), "m_shire_rsp_export", ii);
      end
      foreach (m_neigh_stim_agents[ii]) begin
         m_neigh_stim_agents[ii].connect_my_bus_ports(m_neigh_bus_agents[ii]);
      end
      foreach (m_shire_stim_agents[ii]) begin
         m_shire_stim_agents[ii].connect_my_bus_ports(m_shire_bus_agents[ii]);
      end
   endfunction : connect_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string name_in = "evl_shire_example_env", input uvm_component parent_in = null);
      super.new(name_in, parent_in, SHIRE_RTL_AGENT);
      set_abstract_name("SHIRE");
      add_agent_alias(SHIRE_AGENT);
   endfunction : new

endclass : evl_shire_example_env

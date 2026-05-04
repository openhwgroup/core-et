//--------------------------------------------------------------------------------------------------
//
// Class: evl_noc_test_env
//
class evl_noc_test_env extends evl_verif_comp;

   `uvm_component_utils(evl_noc_test_env)


   //-----------------------------------------------------------------------------------------------
   //
   // Static Member Variables and Objects
   //
   `evl_param_key(pk_allow_unique_tags)
   `evl_param_key(pk_downstream_req_port_count)
   `evl_param_key(pk_l3_mesh_slave)
   `evl_param_key(pk_neigh_stim_count)
   `evl_param_key(pk_noc_stim_count)
   `evl_param_key(pk_sc_stim_count)
   `evl_param_key(pk_ultrasoc_en)
   `evl_param_key(pk_upstream_req_port_count)


   //-----------------------------------------------------------------------------------------------
   //
   // Member Variables and Objects
   //
   evl_verif_comp m_shire_channel_agent;

   evl_verif_comp m_neighborhood_agents[evl_agent_id_t];

   evl_verif_comp m_neigh_bus_agents[evl_agent_id_t];
   evl_verif_comp m_neigh_stim_agents[evl_agent_id_t];
   evl_verif_comp m_sc_stim_agents[evl_agent_id_t];

   evl_verif_comp m_axi_bus_agents[evl_agent_id_t];
   evl_verif_comp m_axi_stim_agents[evl_agent_id_t];

   //-----------------------------------------------------------------------------------------------
   //
   // Function: build_phase
   //
   virtual function void build_phase(uvm_phase phase);

      super.build_phase(phase);
   endfunction : build_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Function: connect_phase
   //
   virtual function void connect_phase(uvm_phase phase);
      super.connect_phase(phase);
   endfunction : connect_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Function: start_of_simulation_phase
   //
   virtual function void start_of_simulation_phase(uvm_phase phase);
      uvm_factory      factory;
      uvm_component    component;
      evl_verif_comp   verif_comp;
      evl_rtl_instance instance_list[];
      int shire_offset;

      super.start_of_simulation_phase(phase);

      factory = uvm_factory::get();

      evl_rtl_instance_map::get_full_list(instance_list);

      $display("========Printing RTL Inst Map=======");
      foreach (instance_list[ii]) begin
         $display("Full Name:     %0s", instance_list[ii].get_full_name());
         $display("Instance Name: %0s", instance_list[ii].get_inst_name());
         $display("Parent Name:   %0s", instance_list[ii].get_parent_name());
      end

   endfunction : start_of_simulation_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string name_in = "evl_noc_env", input uvm_component parent_in = null);
      super.new(name_in, parent_in, NOC_RTL_AGENT);
      set_abstract_name("SOC");
   endfunction : new

endclass : evl_noc_test_env

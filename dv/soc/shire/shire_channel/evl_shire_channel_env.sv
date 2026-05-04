//--------------------------------------------------------------------------------------------------
//
// Class: evl_shire_channel_env
//
class evl_shire_channel_env extends evl_verif_comp;

   `uvm_component_utils(evl_shire_channel_env)


   //-----------------------------------------------------------------------------------------------
   //
   // Static Member Variables and Objects
   //
   `evl_param_key(pk_axi_port_mask)
   `evl_param_key(pk_default_cache_attr)
   `evl_param_key(pk_etlink_master_count)
   `evl_param_key(pk_neigh_agent_0)
   `evl_param_key(pk_neigh_agent_1)
   `evl_param_key(pk_neigh_agent_2)
   `evl_param_key(pk_neigh_agent_3)
   `evl_param_key(pk_no_uc_sb)
   `evl_param_key(pk_num_banks)
   `evl_param_key(pk_num_icache)
   `evl_param_key(pk_num_l3_master_ports)
   `evl_param_key(pk_num_l3_ports)
   `evl_param_key(pk_num_l3_slave_ports)
   `evl_param_key(pk_num_rbox)
   `evl_param_key(pk_shire_cache_enabled)
   `evl_param_key(pk_uc_scoreboard_disable)
   `evl_param_key(pk_upstream_req_port_count)


   //-----------------------------------------------------------------------------------------------
   //
   // Member Variables and Objects
   //
   evl_verif_comp         m_shire_cache_agent;
   evl_verif_comp         m_reg_bus_agents[int];
   evl_verif_comp         m_neighborhood_agents[int];
   int                    m_num_banks;
   int                    m_num_rbox;
   int                    m_num_icache;
   int                    m_no_uc_sb;
   evl_ucblock_scoreboard m_ucblock_scoreboard;


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_req_desc_export
   //
   virtual function uvm_analysis_export#(evl_req_desc) get_req_desc_export(input string port_name      = "unknown",
                                                                           input int    port_number    = 0,
                                                                           input int    subport_number = 0);
      if (m_ucblock_scoreboard != null) begin
         if (port_name == "m_ucblock_etlink_req_export") begin
            return m_ucblock_scoreboard.get_req_desc_export("m_ucblock_etlink_req_export", port_number);
         end
         else if (port_name == "m_ucblock_etlink_rsp_export") begin
            return m_ucblock_scoreboard.get_req_desc_export("m_ucblock_etlink_rsp_export", port_number);
         end
         else if (port_name == "m_ucblock_sysaxi_req_export") begin
            return m_ucblock_scoreboard.get_req_desc_export("m_ucblock_sysaxi_req_export", port_number);
         end
         else if (port_name == "m_ucblock_sysaxi_rsp_export") begin
            return m_ucblock_scoreboard.get_req_desc_export("m_ucblock_sysaxi_rsp_export", port_number);
         end
         else if (port_name == "m_ucblock_l3axi_req_export") begin
            return m_ucblock_scoreboard.get_req_desc_export("m_ucblock_l3axi_req_export", port_number);
         end
         else if (port_name == "m_ucblock_l3axi_rsp_export") begin
            return m_ucblock_scoreboard.get_req_desc_export("m_ucblock_l3axi_rsp_export", port_number);
         end
         else if (port_name == "m_ucblock_fromsysaxi_req_export") begin
            return m_ucblock_scoreboard.get_req_desc_export("m_ucblock_fromsysaxi_req_export", port_number);
         end
         else if (port_name == "m_ucblock_fromsysaxi_rsp_export") begin
            return m_ucblock_scoreboard.get_req_desc_export("m_ucblock_fromsysaxi_rsp_export", port_number);
         end
      end
      return null;
   endfunction : get_req_desc_export


   //-----------------------------------------------------------------------------------------------
   //
   // Function: find_observed_request
   //
   // This function is used by bus agents to find parent message request descriptors for
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
      if (m_ucblock_scoreboard == null) begin
         return null;
      end
      if ((port_in == int'(SHIRE_UCTOSYS_MESH_AXI)) || (port_in == int'(SHIRE_UCTOL3_MESH_AXI))) begin
         return m_ucblock_scoreboard.find_observed_request(port_type_in, port_in, subport_in, paired_req_desc, bus_cmd_in, address_in, req_id_in, source_id_in, trans_id_in);
      end
   endfunction : find_observed_request


   //-----------------------------------------------------------------------------------------------
   //
   // Function: find_observed_response
   //
   // This function is used by ET link bus agents to find parent message request descriptors for
   // received message responses.  The L2 scoreboard is the only tie between these two events.
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
      evl_req_desc req_desc = null;

      if (compare_fields == 1'b1) begin
         if (qualifier[1:0] inside { 2'b00 }) begin
            if (m_shire_cache_agent != null) begin
               req_desc = m_shire_cache_agent.find_observed_response(port_in, paired_req_desc, compare_fields, qualifier, rsp_type_in, req_id_in, dest_id_in, rsp_byte_en_in, rsp_data_in);
            end
            if ((req_desc == null) && (m_ucblock_scoreboard != null)) begin
               req_desc = m_ucblock_scoreboard.find_observed_response(port_in, paired_req_desc, compare_fields, qualifier, rsp_type_in, req_id_in, dest_id_in, rsp_byte_en_in, rsp_data_in);
            end
         end
         else if (qualifier[1:0] inside { 2'b10 }) begin
            if (m_shire_cache_agent != null) begin
               req_desc = m_shire_cache_agent.find_observed_response(port_in, paired_req_desc, compare_fields, qualifier, rsp_type_in, req_id_in, dest_id_in, rsp_byte_en_in, rsp_data_in);
            end
         end
         else if (qualifier[1:0] inside { 2'b11 }) begin
            if (m_ucblock_scoreboard != null) begin
               req_desc = m_ucblock_scoreboard.find_observed_response(port_in, paired_req_desc, compare_fields, qualifier, rsp_type_in, req_id_in, dest_id_in, rsp_byte_en_in, rsp_data_in);
            end
         end
         if (req_desc != null) begin
            return req_desc;
         end
      end
      if (qualifier[1:0] inside { 2'b00, 2'b10 }) begin
         if (m_shire_cache_agent != null) begin
            return m_shire_cache_agent.find_observed_response(port_in, paired_req_desc, 1'b0, qualifier, rsp_type_in, req_id_in, dest_id_in, rsp_byte_en_in, rsp_data_in);
         end
      end
      else begin
         if (m_ucblock_scoreboard != null) begin
            return m_ucblock_scoreboard.find_observed_response(port_in, paired_req_desc, 1'b0, qualifier, rsp_type_in, req_id_in, dest_id_in, rsp_byte_en_in, rsp_data_in);
         end
      end
      return null;
   endfunction : find_observed_response


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

      if (m_rtl_port != null) begin
         m_num_banks  = m_rtl_port.get_int_param(pk_num_banks, 0);
         m_num_rbox   = m_rtl_port.get_int_param(pk_num_rbox, 0);
         m_num_icache = m_rtl_port.get_int_param(pk_num_icache, 0);
         m_no_uc_sb   = m_rtl_port.get_int_param(pk_no_uc_sb, 0);
      end
      else begin
         m_num_banks  = `SC_BANKS;
         m_num_rbox   = 0;
         m_num_icache = 0;
         m_no_uc_sb   = 0;
      end

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
            if (verif_comp.get_agent_type() == SHIRE_CACHE_RTL_AGENT) begin
               m_shire_cache_agent = verif_comp;
               verif_comp.set_int_param(pk_upstream_req_port_count, get_int_param(pk_upstream_req_port_count));
               verif_comp.set_int_param(pk_axi_port_mask, get_int_param(pk_axi_port_mask, 0));
               verif_comp.set_int_param(pk_num_l3_ports, get_int_param(pk_num_l3_ports, 1));
               verif_comp.set_int_param(pk_num_l3_master_ports, get_int_param(pk_num_l3_master_ports, 1));
               verif_comp.set_int_param(pk_num_l3_slave_ports, get_int_param(pk_num_l3_slave_ports, 1));
            end
            if ((verif_comp.is_bus_vc() == 1'b1) && (verif_comp.get_agent_type() == APB_BUS_AGENT)) begin
               if (agent_id == 0) begin
                  verif_comp.set_agent_id(agent_id + m_num_banks);
                  verif_comp.set_port_id(agent_id);
                  verif_comp.set_abstract_name($sformatf("OTHER_ESR(APB)"));
                  m_reg_bus_agents[agent_id] = verif_comp;
               end
               else if ((m_num_rbox > 0) && (agent_id < (1 + m_num_rbox))) begin
                  verif_comp.set_agent_id(agent_id + m_num_banks);
                  verif_comp.set_port_id(agent_id);
                  if (m_num_rbox > 1) begin
                     verif_comp.set_abstract_name($sformatf("RBOX_ESR-%0d(APB)", agent_id - 1));
                  end
                  else begin
                     verif_comp.set_abstract_name($sformatf("RBOX_ESR(APB)"));
                  end
                  m_reg_bus_agents[agent_id] = verif_comp;
               end
               else if ((m_num_icache > 0) && (agent_id < (1 + m_num_rbox + m_num_icache))) begin
                  verif_comp.set_agent_id(agent_id + m_num_banks);
                  verif_comp.set_port_id(agent_id);
                  if (m_num_icache > 1) begin
                     verif_comp.set_abstract_name($sformatf("ICACHE_ESR-%0d(APB)", agent_id - 1 - m_num_rbox));
                  end
                  else begin
                     verif_comp.set_abstract_name($sformatf("ICACHE_ESR(APB)"));
                  end
                  m_reg_bus_agents[agent_id] = verif_comp;
               end
               else begin
                  verif_comp.set_agent_id(agent_id + m_num_banks);
                  verif_comp.set_port_id(agent_id);
                  verif_comp.set_abstract_name($sformatf("EXTRA_ESR-%0d(APB)", agent_id - 1 - m_num_rbox - m_num_icache));
                  m_reg_bus_agents[agent_id] = verif_comp;
               end
            end
            else if (agent_id != ~evl_agent_id_t'(0)) begin
               verif_comp.set_agent_id(agent_id);
               verif_comp.set_abstract_name($sformatf("%0s-%0d", verif_comp.get_root_abstract_name(), agent_id));
            end
            add_vc_child(verif_comp);
         end
      end

      if ((get_int_param(pk_uc_scoreboard_disable, 0) == 0) && (m_no_uc_sb == 0)) begin
         m_ucblock_scoreboard = evl_ucblock_scoreboard::type_id::create("m_ucblock_scoreboard", this);
         m_ucblock_scoreboard.set_agent(this);
         m_ucblock_scoreboard.set_shire_id(get_shire_id());
         m_ucblock_scoreboard.set_rtl_port(m_rtl_port);
         m_ucblock_scoreboard.set_abstract_name($sformatf("S%0d:UCBLOCK(SB)", get_shire_id()));
         // get the number of etlink masters for the scoreboard
         m_ucblock_scoreboard.set_int_param(pk_etlink_master_count, get_int_param(pk_etlink_master_count));
         m_ucblock_scoreboard.set_int_param(pk_default_cache_attr, get_int_param(pk_default_cache_attr, 15));
      end
   endfunction : build_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Function: start_of_simulation_phase
   //
   virtual function void start_of_simulation_phase(uvm_phase phase);
      super.start_of_simulation_phase(phase);
      for (int ii = 0; ii < 4; ii++) begin
         uvm_void neigh_object;

         case (ii)
            0:       neigh_object = get_object_param(pk_neigh_agent_0);
            1:       neigh_object = get_object_param(pk_neigh_agent_1);
            2:       neigh_object = get_object_param(pk_neigh_agent_2);
            3:       neigh_object = get_object_param(pk_neigh_agent_3);
            default: neigh_object = null;
         endcase
         m_neighborhood_agents[ii] = null;
         if (neigh_object != null) begin
            $cast(m_neighborhood_agents[ii], neigh_object);
         end
      end
   endfunction : start_of_simulation_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Function: post_reset_phase
   //
   virtual task post_reset_phase(uvm_phase phase);
      evl_shire_id_t shire_id;
      evl_verif_comp system_agent;

      shire_id     = get_shire_id();
      system_agent = evl_verif_comp_map::get_verif_comp(SYSTEM_AGENT);
      fork
         super.post_reset_phase(phase);
      join_none
      #0;
      if ((m_rtl_port != null) && (shire_id != ~evl_shire_id_t'(0)) && (shire_id < `EVL_MAX_MINION_SHIRE_ID)) begin
         int value;

         if (`et_value_plusargs("delay_sc_init=%d", value) == 0) begin
            value = 35000;
         end
         if (`et_test_plusargs("disable_shire_cache_enable_check") != 0) begin
            value = 0;
         end
         if ((value > 0) && (evl_verif_comp_map::get_verif_comp(SHIRE_CACHE_RTL_AGENT, ~evl_agent_id_t'(0), shire_id) != null)) begin
            if (value < 10000) begin
               value = 10000;
            end
            if (m_rtl_port.get_int_param(pk_shire_cache_enabled, 0) == 0) begin
               phase.raise_objection(phase);
               fork
                  begin :wait_for_shire_cache_enabled
                     #0;
                     `evl_log(UVM_LOW, VMOD_GLOBAL, get_abstract_name(), $sformatf("raising post_reset_phase() objection ... the shire cache is not enabled"))
                     m_rtl_port.wait_for_int_param(pk_shire_cache_enabled, 1, 1);
                     `evl_log(UVM_LOW, VMOD_GLOBAL, get_abstract_name(), $sformatf("dropping post_reset_phase() objection ... the shire cache is enabled"))
                     disable wait_for_shire_cache_enabled_timeout;
                  end
                  begin :wait_for_shire_cache_enabled_timeout
                     if (system_agent != null) begin
                        system_agent.wait_cycles(value);
                     end
                     else begin
                        wait_cycles(value);
                     end
                     disable wait_for_shire_cache_enabled;
                     `dut_error(get_abstract_name(), $sformatf("dropping post_reset_phase() objection ... the shire cache is still not enabled"))
                  end
               join
               phase.drop_objection(phase);
            end
         end
      end
   endtask : post_reset_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Function: connect_port_to_my_export
   //
   virtual function void connect_port_to_my_export(input uvm_analysis_port#(evl_req_desc) port_in,
                                                   input string                           port_name      = "unknown",
                                                   input int                              port_number    = 0,
                                                   input int                              subport_number = 0);
      if (m_shire_cache_agent != null) begin
         m_shire_cache_agent.connect_port_to_my_export(port_in, port_name, port_number, subport_number);
      end
   endfunction : connect_port_to_my_export


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_master_port_agent
   //
   virtual function evl_verif_comp get_master_port_agent(input string port_name = "", input int port_id = 0, input int subport_id = 0);
      if (m_shire_cache_agent == null) begin
         return null;
      end
      return m_shire_cache_agent.get_master_port_agent(port_name, port_id, subport_id);;
   endfunction : get_master_port_agent


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_slave_port_agent
   //
   virtual function evl_verif_comp get_slave_port_agent(input string port_name = "", input int port_id = 0, input int subport_id = 0);
      if (m_shire_cache_agent == null) begin
         return null;
      end
      return m_shire_cache_agent.get_slave_port_agent(port_name, port_id, subport_id);;
   endfunction : get_slave_port_agent


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_cache_set
   //
   virtual function int get_cache_set(input evl_paddr_t caddr,
                                      input int cway  = 0,
                                      input int ctype = 0);
      if (evl_cache_ram_type_t'(ctype) inside { SC_RAM_TYPE_SC, SC_RAM_TYPE_L2, SC_RAM_TYPE_L3, SC_RAM_TYPE_SC_DATA, SC_RAM_TYPE_L2_DATA,
                                                SC_RAM_TYPE_L2_TAG, SC_RAM_TYPE_L2_STATE, SC_RAM_TYPE_L3_DATA, SC_RAM_TYPE_L3_TAG,
                                                SC_RAM_TYPE_L3_STATE }) begin
         if (m_shire_cache_agent == null) begin
            return -1;
         end
         return m_shire_cache_agent.get_cache_set(caddr, cway, ctype);
      end
      if (m_rtl_port == null) begin
         return -1;
      end
      return m_rtl_port.get_cache_set(caddr, cway, ctype);
   endfunction : get_cache_set


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_cache_bank
   //
   virtual function int get_cache_bank(input evl_paddr_t caddr,
                                       input int cway  = 0,
                                       input int ctype = 0);
      if (evl_cache_ram_type_t'(ctype) inside { SC_RAM_TYPE_SC, SC_RAM_TYPE_L2, SC_RAM_TYPE_L3, SC_RAM_TYPE_SC_DATA, SC_RAM_TYPE_L2_DATA,
                                                SC_RAM_TYPE_L2_TAG, SC_RAM_TYPE_L2_STATE, SC_RAM_TYPE_L3_DATA, SC_RAM_TYPE_L3_TAG,
                                                SC_RAM_TYPE_L3_STATE }) begin
         if (m_shire_cache_agent == null) begin
            return -1;
         end
         return m_shire_cache_agent.get_cache_bank(caddr, cway, ctype);
      end
      if (m_rtl_port == null) begin
         return -1;
      end
      return m_rtl_port.get_cache_bank(caddr, cway, ctype);
   endfunction : get_cache_bank


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_cache_subbank
   //
   virtual function int get_cache_subbank(input evl_paddr_t caddr,
                                          input int cway  = 0,
                                          input int ctype = 0);
      if (evl_cache_ram_type_t'(ctype) inside { SC_RAM_TYPE_SC, SC_RAM_TYPE_L2, SC_RAM_TYPE_L3, SC_RAM_TYPE_SC_DATA, SC_RAM_TYPE_L2_DATA,
                                                SC_RAM_TYPE_L2_TAG, SC_RAM_TYPE_L2_STATE, SC_RAM_TYPE_L3_DATA, SC_RAM_TYPE_L3_TAG,
                                                SC_RAM_TYPE_L3_STATE }) begin
         if (m_shire_cache_agent == null) begin
            return -1;
         end
         return m_shire_cache_agent.get_cache_subbank(caddr, cway, ctype);
      end
      if (m_rtl_port == null) begin
         return -1;
      end
      return m_rtl_port.get_cache_subbank(caddr, cway, ctype);
   endfunction : get_cache_subbank


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_cache_tag
   //
   virtual function evl_paddr_t get_cache_tag(input int cindex,
                                              input int cway  = 0,
                                              input int ctype = 0);
      if (evl_cache_ram_type_t'(ctype) inside { SC_RAM_TYPE_SC, SC_RAM_TYPE_L2, SC_RAM_TYPE_L3, SC_RAM_TYPE_SC_DATA, SC_RAM_TYPE_L2_DATA,
                                                SC_RAM_TYPE_L2_TAG, SC_RAM_TYPE_L2_STATE, SC_RAM_TYPE_L3_DATA, SC_RAM_TYPE_L3_TAG,
                                                SC_RAM_TYPE_L3_STATE }) begin
         if (m_shire_cache_agent == null) begin
            return ~evl_paddr_t'(0);
         end
         return m_shire_cache_agent.get_cache_tag(cindex, cway, ctype);
      end
      if (evl_cache_ram_type_t'(ctype) inside { SC_RAM_TYPE_ICACHE_TAG, SC_RAM_TYPE_ICACHE0_TAG, SC_RAM_TYPE_ICACHE1_TAG, SC_RAM_TYPE_ICACHE2_TAG, SC_RAM_TYPE_ICACHE3_TAG }) begin
         if (evl_cache_ram_type_t'(ctype) inside { SC_RAM_TYPE_ICACHE_TAG, SC_RAM_TYPE_ICACHE0_TAG }) begin
            if (m_neighborhood_agents.exists(0) && (m_neighborhood_agents[0] != null)) begin
               return m_neighborhood_agents[0].get_cache_tag(cindex, cway, ctype);
            end
         end
         else if (evl_cache_ram_type_t'(ctype) inside { SC_RAM_TYPE_ICACHE1_TAG }) begin
            if (m_neighborhood_agents.exists(1) && (m_neighborhood_agents[1] != null)) begin
               return m_neighborhood_agents[1].get_cache_tag(cindex, cway, ctype);
            end
         end
         else if (evl_cache_ram_type_t'(ctype) inside { SC_RAM_TYPE_ICACHE2_TAG }) begin
            if (m_neighborhood_agents.exists(2) && (m_neighborhood_agents[2] != null)) begin
               return m_neighborhood_agents[2].get_cache_tag(cindex, cway, ctype);
            end
         end
         else if (evl_cache_ram_type_t'(ctype) inside { SC_RAM_TYPE_ICACHE3_TAG }) begin
            if (m_neighborhood_agents.exists(3) && (m_neighborhood_agents[3] != null)) begin
               return m_neighborhood_agents[3].get_cache_tag(cindex, cway, ctype);
            end
         end
         return ~evl_paddr_t'(0);
      end
      if (m_rtl_port == null) begin
         return ~evl_paddr_t'(0);
      end
      return m_rtl_port.get_cache_tag(cindex, cway, ctype);
   endfunction : get_cache_tag


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_cache_data
   //
   virtual function int get_cache_data(output evl_dword_t cdata[],
                                       input  int         cindex,
                                       input  int         cway  = 0,
                                       input  int         ctype = 0);
      if (evl_cache_ram_type_t'(ctype) inside { SC_RAM_TYPE_SC, SC_RAM_TYPE_L2, SC_RAM_TYPE_L3, SC_RAM_TYPE_SC_DATA, SC_RAM_TYPE_L2_DATA,
                                                SC_RAM_TYPE_L2_TAG, SC_RAM_TYPE_L2_STATE, SC_RAM_TYPE_L3_DATA, SC_RAM_TYPE_L3_TAG,
                                                SC_RAM_TYPE_L3_STATE }) begin
         if (m_shire_cache_agent == null) begin
            cdata = new[`EVL_LINE_DWORDS];
            for (int ii = 0; ii < `EVL_LINE_DWORDS; ii++) begin
               cdata[ii] = ~evl_dword_t'(0);
            end
            return -1;
         end
         return m_shire_cache_agent.get_cache_data(cdata, cindex, cway, ctype);
      end
      if (m_rtl_port == null) begin
         cdata = new[`EVL_LINE_DWORDS];
         for (int ii = 0; ii < `EVL_LINE_DWORDS; ii++) begin
            cdata[ii] = ~evl_dword_t'(0);
         end
         return -1;
      end
      return m_rtl_port.get_cache_data(cdata, cindex, cway, ctype);
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
      if (evl_cache_ram_type_t'(ctype) inside { SC_RAM_TYPE_SC, SC_RAM_TYPE_L2, SC_RAM_TYPE_L3, SC_RAM_TYPE_SC_DATA, SC_RAM_TYPE_L2_DATA,
                                                SC_RAM_TYPE_L2_TAG, SC_RAM_TYPE_L2_STATE, SC_RAM_TYPE_L3_DATA, SC_RAM_TYPE_L3_TAG,
                                                SC_RAM_TYPE_L3_STATE }) begin
         if (m_shire_cache_agent == null) begin
            return -1;
         end
         return m_shire_cache_agent.set_cache_bit_err(cindex, cbit0, cbit1, cway, ctype, cerr);
      end
      else if ((evl_cache_ram_type_t'(ctype) inside { SC_RAM_TYPE_ICACHE_DATA, SC_RAM_TYPE_ICACHE0_DATA, SC_RAM_TYPE_ICACHE1_DATA, SC_RAM_TYPE_ICACHE2_DATA, SC_RAM_TYPE_ICACHE3_DATA }) && (m_rtl_port != null)) begin
         return m_rtl_port.set_cache_bit_err(cindex, cbit0, cbit1, cway, ctype, cerr);
      end
      return -1;
   endfunction : set_cache_bit_err


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string name_in = "evl_shire_channel_env", input uvm_component parent_in = null);
      super.new(name_in, parent_in, SHIRE_CHANNEL_RTL_AGENT);
      set_abstract_name("SHIRE_CHANNEL");
   endfunction : new

endclass : evl_shire_channel_env

//--------------------------------------------------------------------------------------------------
//
// Class: evl_shire_env
//
class evl_shire_env extends evl_verif_comp;

   `uvm_component_utils(evl_shire_env)

   //-----------------------------------------------------------------------------------------------
   //
   // Debug NoC Bus Types
   //
   // The following enumerated types are used to identify Debug NoC bus agents in an associative array.
   // These bus agents can then be connected to master or slave stimulus agents as necessary during
   // the connect phase.
   //
   //typedef enum int {
   //      MIN_DBG_DN_EVT,
   //      MIN_DBG_DN_MSG
   //   } minshire_dbg_noc_bus_t;

   //-----------------------------------------------------------------------------------------------
   //
   // Static Member Variables and Objects
   //
   `evl_param_key(pk_allow_loopback)
   `evl_param_key(pk_allow_unique_tags)
   `evl_param_key(pk_axi_port_mask)
   `evl_param_key(pk_dbgnoc_stim)
   `evl_param_key(pk_ignore_low_addr_req_desc)
   `evl_param_key(pk_l3_mesh_slave)
   `evl_param_key(pk_neigh_stim_count)
   `evl_param_key(pk_neigh_stub)
   `evl_param_key(pk_noc_stub)
   `evl_param_key(pk_num_l3_master_ports)
   `evl_param_key(pk_num_l3_ports)
   `evl_param_key(pk_num_l3_slave_ports)
   `evl_param_key(pk_sc_lite)
   `evl_param_key(pk_sc_stim_count)
   `evl_param_key(pk_sc_stub)
   `evl_param_key(pk_upstream_req_port_count)

   // UCBlock scoreboard param
   `evl_param_key(pk_etlink_master_count)
   `evl_param_key(pk_no_error_response)
   `evl_param_key(pk_uc_scoreboard_disable)

   // Agent pointer params
   `evl_param_key(pk_neigh_agent_0)
   `evl_param_key(pk_neigh_agent_1)
   `evl_param_key(pk_neigh_agent_2)
   `evl_param_key(pk_neigh_agent_3)
   `evl_param_key(pk_shire_agent)
   `evl_param_key(pk_shire_channel_agent)


   //-----------------------------------------------------------------------------------------------
   //
   // Member Variables and Objects
   //
   evl_verif_comp   m_shire_channel_agent;

   evl_verif_comp   m_neighborhood_agents[evl_agent_id_t];

   evl_verif_comp   m_neigh_bus_agents[evl_agent_id_t];
   evl_verif_comp   m_neigh_stim_agents[evl_agent_id_t];
   evl_verif_comp   m_sc_stim_agents[evl_agent_id_t];

   evl_verif_comp   m_apb_bus_agents[evl_agent_id_t];
   evl_verif_comp   m_apb_stim_agents[evl_agent_id_t];

   evl_verif_comp_q m_shire_bus_agents[int];
   evl_verif_comp   m_shire_stim_agents[int];

   evl_verif_comp   m_dbgnoc_bus_agents[int];
   evl_verif_comp   m_dbgnoc_stim_agents[int];

   int              m_num_l3_ports;
   int              m_num_l3_master_ports;
   int              m_num_l3_slave_ports;
   int              m_sc_lite;
   int              m_sc_stub;
   int              m_noc_stub;
   int              m_neigh_stub;
   int              m_dbgnoc_stim;
   bit              m_disable_uc_sb;


   //-----------------------------------------------------------------------------------------------
   //
   // Function: configure_evl_reg_block
   //
   virtual function void configure_evl_reg_block(input evl_reg_block reg_block_in = null, input evl_verif_comp agent_in = null);
      if (m_evl_reg_block != null) begin
         if ((agent_in != null) && (reg_block_in != null)) begin
            reg_block_in.add_evl_reg_block(get_shire_id(), m_evl_reg_block, get_derived_hdl_path_name(agent_in));
         end
         foreach (m_vc_children[ii]) begin
            m_vc_children[ii].configure_evl_reg_block(m_evl_reg_block, this);
         end
      end
      else begin
         foreach (m_vc_children[ii]) begin
            m_vc_children[ii].configure_evl_reg_block(reg_block_in, agent_in);
         end
      end
   endfunction : configure_evl_reg_block


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
      return m_shire_channel_agent.find_observed_response(port_in, paired_req_desc, compare_fields, qualifier, rsp_type_in, req_id_in, dest_id_in, rsp_byte_en_in, rsp_data_in);
   endfunction : find_observed_response


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
      evl_shire_id_t      shire_id;
      int                 shire_offset;
      int                 axi_ifc_type;
      int                 num_base_axi_ifc;
      int                 num_ml_axi_ifc;
      int                 layer_num;
      int                 rtl_port_num;
      int                 layer_offset;
      int                 axi_port_mask;
      int                 dbgnoc_comp_id;
      evl_shire_reg_model reg_model;
      evl_rtl_port        bus_rtl_port;

      super.build_phase(phase);
      factory               = uvm_factory::get();
      shire_id              = get_shire_id();
      axi_port_mask         = 0;
      m_num_l3_ports        = 1;
      m_num_l3_master_ports = 1;
      m_num_l3_slave_ports  = 1;
      num_base_axi_ifc      = 4;
      num_ml_axi_ifc        = 2;
      if (m_rtl_port != null) begin
         m_num_l3_ports        = m_rtl_port.get_int_param(pk_num_l3_ports, 1);
         m_num_l3_master_ports = m_rtl_port.get_int_param(pk_num_l3_master_ports, 1);
         m_num_l3_slave_ports  = m_rtl_port.get_int_param(pk_num_l3_slave_ports, 1);
         m_noc_stub            = m_rtl_port.get_int_param(pk_noc_stub, 0);
         m_sc_lite             = m_rtl_port.get_int_param(pk_sc_lite, 0);
         m_sc_stub             = m_rtl_port.get_int_param(pk_sc_stub, 0);
         m_neigh_stub          = m_rtl_port.get_int_param(pk_neigh_stub, 0);
         m_dbgnoc_stim         = m_rtl_port.get_int_param(pk_dbgnoc_stim, 0);
         `ifdef ET_NEIGH_STANDALONE
            m_sc_lite  = 0;
            m_sc_stub  = 1;
            m_noc_stub = 1;
         `else
            if (m_sc_lite != 0) begin
               m_sc_stub = 0;
            end
         `endif
      end
      else begin
         `dut_error(get_abstract_name(), $sformatf("TB_FAIL: cannot create agents in shire %0d ... no access to an RTL port", shire_id))
         return;
      end
      if (m_num_l3_ports < 1) begin
         m_num_l3_ports = 1;
      end
      if (m_num_l3_master_ports < 1) begin
         m_num_l3_master_ports = 1;
      end
      if (m_num_l3_slave_ports < 1) begin
         m_num_l3_slave_ports = 1;
      end
      if (m_num_l3_master_ports > m_num_l3_ports) begin
         m_num_l3_ports = m_num_l3_master_ports;
      end
      if (m_num_l3_slave_ports > m_num_l3_ports) begin
         m_num_l3_ports = m_num_l3_slave_ports;
      end
      shire_offset = shire_id * (num_base_axi_ifc + (num_ml_axi_ifc * m_num_l3_ports));
      evl_rtl_instance_map::get_my_children_list(instance_list, get_full_name());

      //
      // Start by creating null pointers for all of the possible AXI bus agents.
      //
      for (int ii = 0; ii < m_num_l3_master_ports; ii++) begin
         m_shire_bus_agents[int'(SHIRE_L2TOL3_MESH_AXI)].push_back(null);
      end
      for (int ii = 0; ii < m_num_l3_slave_ports; ii++) begin
         m_shire_bus_agents[int'(SHIRE_MESH_TOL3_AXI)].push_back(null);
      end
      m_shire_bus_agents[int'(SHIRE_L3TOSYS_MESH_AXI)].push_back(null);
      m_shire_bus_agents[int'(SHIRE_FROMSYS_MESH_AXI)].push_back(null);
      m_shire_bus_agents[int'(SHIRE_UCTOL3_MESH_AXI)].push_back(null);
      m_shire_bus_agents[int'(SHIRE_UCTOSYS_MESH_AXI)].push_back(null);

      foreach (instance_list[ii]) begin // --------------------------------------------------------{
         string         inst_name;
         string         parent_names[$];
         evl_agent_id_t agent_id;

         parent_comp = this;
         inst_name   = evl_base_pkg::extract_parent_list(parent_names, get_full_name(), instance_list[ii].get_full_name());
         agent_id    = instance_list[ii].get_agent_pid();
         //`evl_log(UVM_DEBUG, VMOD_CONFIG, get_abstract_name(), $sformatf("(CYE DEBUG) foreach(instance_list[]) loop: ii=%0d inst_name=%s", ii, inst_name))

         foreach (parent_names[jj]) begin // ------------------------------------------------------{
            string full_parent_name;

            full_parent_name = $sformatf("%0s.%0s", parent_comp.get_full_name(), parent_names[jj]);
            //`evl_log(UVM_DEBUG, VMOD_CONFIG, get_abstract_name(), $sformatf("(CYE DEBUG) foreach(parent_names[]) loop: jj=%0d full_parent_name=%s", jj, full_parent_name))

            if (!parent_comps.exists(full_parent_name)) begin
               parent_comp = evl_verif_comp::type_id::create(parent_names[jj], parent_comp);
               parent_comps[full_parent_name] = parent_comp;
            end
            else begin
               parent_comp = parent_comps[full_parent_name];
            end
         end // foreach (parent_names[jj]) --------------------------------------------------------}

         `evl_log(UVM_DEBUG, VMOD_CONFIG, get_abstract_name(), $sformatf("(CYE DEBUG) foreach(instance_list[]) loop: ii=%0d inst_name=%s: create component ...", ii, inst_name))
         if ((m_sc_stub == 0) || (instance_list[ii].get_agent_type() != APB_BUS_AGENT) || ((m_neigh_stub & (1 << agent_id)) == 0)) begin
            component = factory.create_component_by_name(instance_list[ii].get_sv_comp(), "", inst_name, parent_comp);
         end
         else begin
            component = null;
         end

         if (component != null) begin // ----------------------------------------------------------{

            $cast(verif_comp, component);
            verif_comp.set_shire_id(get_shire_id());

            //
            // Bus Verification Components
            //
            axi_ifc_type   = -1;
            dbgnoc_comp_id = -1;
            if (verif_comp.is_bus_vc() == 1'b1) begin // ------------------------------------------{
               if (verif_comp.get_agent_type() == ETLINK_BUS_AGENT) begin // ----------------------{
                  verif_comp.set_agent_id(agent_id);
                  verif_comp.set_port_id(agent_id);
                  verif_comp.set_neighborhood_id(agent_id);
                  verif_comp.set_abstract_name($sformatf("N%0d(ETLINK)", agent_id));
                  m_neigh_bus_agents[agent_id] = verif_comp;
                  if (get_int_param(pk_no_error_response, 0) != 0) begin
                     verif_comp.set_int_param(pk_no_error_response, 1);
                  end
                  rtl_port_num = 0;
               end // if (verif_comp.get_agent_type() == ETLINK_BUS_AGENT) ------------------------}
               else if (verif_comp.get_agent_type() == DBG_NOC_BUS_AGENT) begin // ----------------{
                  verif_comp.set_agent_id(get_shire_id());
                  if (inst_name == "m_dbg_dn_evt") begin
                     dbgnoc_comp_id = int'(MIN_DBG_DN_EVT);
                     verif_comp.set_port_id(dbgnoc_comp_id);
                     verif_comp.set_agent_id(dbgnoc_comp_id);
                     verif_comp.set_abstract_name($sformatf("DN_EVT(DBGNOC)"));
                     m_dbgnoc_bus_agents[dbgnoc_comp_id] = verif_comp;
                  end
                  else if (inst_name == "m_dbg_dn_msg") begin
                     dbgnoc_comp_id = int'(MIN_DBG_DN_MSG);
                     verif_comp.set_port_id(dbgnoc_comp_id);
                     verif_comp.set_agent_id(dbgnoc_comp_id);
                     verif_comp.set_abstract_name($sformatf("DN_MSG(DBGNOC)"));
                     m_dbgnoc_bus_agents[dbgnoc_comp_id] = verif_comp;
                  end
               end // if (verif_comp.get_agent_type() == DBG_NOC_BUS_AGENT) begin // --------------}
               else if (verif_comp.get_agent_type() == AXI_BUS_AGENT) begin // --------------------{
                  evl_rtl_port bus_agent_rtl_port;

                  bus_agent_rtl_port = verif_comp.get_rtl_port();
                  if (bus_agent_rtl_port != null) begin
                     layer_num = bus_agent_rtl_port.get_port_id();
                  end
                  else begin
                     layer_num = 0;
                  end
                  rtl_port_num = layer_num;
                  verif_comp.set_agent_id(get_shire_id());
                  if (inst_name == "m_to_l3_mesh_master_axi") begin
                     axi_ifc_type  = int'(SHIRE_L2TOL3_MESH_AXI);
                     axi_port_mask = axi_port_mask | (1 << axi_ifc_type);
                     layer_offset  = m_global_attrs.get_shire_axi_port_index(axi_ifc_type, layer_num, m_num_l3_ports, 0);
                     rtl_port_num  = (m_noc_stub > 0) ? layer_offset : rtl_port_num;
                     verif_comp.set_port_id(shire_offset + layer_offset);
                     verif_comp.set_port_type(axi_ifc_type);
                     verif_comp.set_subport_id(layer_num);
                     verif_comp.set_agent_id(layer_offset);
                     verif_comp.set_int_param(pk_ignore_low_addr_req_desc, 1); // FUTURE: Charles Ye Remove this when an uncached scoreboard is available
                     if (m_num_l3_master_ports > 1) begin
                        verif_comp.set_abstract_name($sformatf("L2TOL3_MESH(AXI%0d)", layer_num));
                     end
                     else begin
                        verif_comp.set_abstract_name($sformatf("L2TOL3_MESH(AXI)"));
                     end
                     m_shire_bus_agents[axi_ifc_type][layer_num] = verif_comp;
                  end
                  else if (inst_name == "m_to_sys_mesh_master_axi") begin
                     axi_ifc_type  = int'(SHIRE_L3TOSYS_MESH_AXI);
                     axi_port_mask = axi_port_mask | (1 << axi_ifc_type);
                     layer_offset  = m_global_attrs.get_shire_axi_port_index(axi_ifc_type, 0, m_num_l3_ports, 0);
                     rtl_port_num  = (m_noc_stub > 0) ? layer_offset : rtl_port_num;
                     verif_comp.set_port_id(shire_offset + layer_offset);
                     verif_comp.set_port_type(axi_ifc_type);
                     verif_comp.set_agent_id(layer_offset);
                     verif_comp.set_abstract_name($sformatf("L3TOSYS_MESH(AXI)"));
                     m_shire_bus_agents[axi_ifc_type][0] = verif_comp;
                  end
                  else if (inst_name == "m_uc_to_l3_mesh_master_axi") begin
                     axi_ifc_type  = int'(SHIRE_UCTOL3_MESH_AXI);
                     axi_port_mask = axi_port_mask | (1 << axi_ifc_type);
                     layer_offset  = m_global_attrs.get_shire_axi_port_index(axi_ifc_type, 0, m_num_l3_ports, 0);
                     rtl_port_num  = (m_noc_stub > 0) ? layer_offset : rtl_port_num;
                     verif_comp.set_port_id(shire_offset + layer_offset);
                     verif_comp.set_port_type(axi_ifc_type);
                     verif_comp.set_agent_id(layer_offset);
                     verif_comp.set_abstract_name($sformatf("UCTOL3_MESH(AXI)"));
                     m_shire_bus_agents[axi_ifc_type][0] = verif_comp;
                  end
                  else if (inst_name == "m_uc_to_sys_mesh_master_axi") begin
                     axi_ifc_type  = int'(SHIRE_UCTOSYS_MESH_AXI);
                     axi_port_mask = axi_port_mask | (1 << axi_ifc_type);
                     layer_offset  = m_global_attrs.get_shire_axi_port_index(axi_ifc_type, 0, m_num_l3_ports, 0);
                     rtl_port_num  = (m_noc_stub > 0) ? layer_offset : rtl_port_num;
                     verif_comp.set_port_id(shire_offset + layer_offset);
                     verif_comp.set_port_type(axi_ifc_type);
                     verif_comp.set_agent_id(layer_offset);
                     verif_comp.set_abstract_name($sformatf("UCTOSYS_MESH(AXI)"));
                     m_shire_bus_agents[axi_ifc_type][0] = verif_comp;
                  end
                  else if (inst_name == "m_l3_mesh_slave_axi") begin
                     axi_ifc_type  = int'(SHIRE_MESH_TOL3_AXI);
                     axi_port_mask = axi_port_mask | (1 << axi_ifc_type);
                     layer_offset  = m_global_attrs.get_shire_axi_port_index(axi_ifc_type, layer_num, m_num_l3_ports, 0);
                     verif_comp.set_port_id(shire_offset + layer_offset);
                     verif_comp.set_port_type(axi_ifc_type);
                     verif_comp.set_subport_id(layer_num);
                     verif_comp.set_agent_id(layer_offset);
                     if (m_num_l3_slave_ports > 1) begin
                        verif_comp.set_abstract_name($sformatf("MESH_TOL3(AXI%0d)", layer_num));
                     end
                     else begin
                        verif_comp.set_abstract_name($sformatf("MESH_TOL3(AXI)"));
                     end
                     m_shire_bus_agents[axi_ifc_type][layer_num] = verif_comp;
                  end
                  else if (inst_name == "m_sys_mesh_slave_axi") begin
                     axi_ifc_type  = int'(SHIRE_FROMSYS_MESH_AXI);
                     axi_port_mask = axi_port_mask | (1 << axi_ifc_type);
                     layer_offset  = m_global_attrs.get_shire_axi_port_index(axi_ifc_type, 0, m_num_l3_ports, 0);
                     verif_comp.set_port_id(shire_offset + layer_offset);
                     verif_comp.set_port_type(axi_ifc_type);
                     verif_comp.set_agent_id(layer_offset);
                     verif_comp.set_abstract_name($sformatf("FROMSYS_MESH(AXI)"));
                     m_shire_bus_agents[axi_ifc_type][0] = verif_comp;
                  end
               end // else if (verif_comp.get_agent_type() == AXI_BUS_AGENT) ----------------------}
               else if (verif_comp.get_agent_type() == APB_BUS_AGENT) begin // --------------------{
                  verif_comp.set_agent_id(agent_id);
                  verif_comp.set_port_id(agent_id);
                  verif_comp.set_neighborhood_id(agent_id);
                  verif_comp.set_abstract_name($sformatf("N%0d:ESR(APB)", agent_id));
                  m_apb_bus_agents[agent_id] = verif_comp;
                  rtl_port_num = 0;
               end // if (verif_comp.get_agent_type() == APB_BUS_AGENT) ---------------------------}

               add_vc_child(verif_comp);

               //
               // Stimulus Verification Components attached to the ETLink Bus Components
               // Create a neighborhood stimulus agent or shire cache stimulus agent.
               //
               if (verif_comp.get_agent_type() == ETLINK_BUS_AGENT) begin // ----------------------{
                  if ((m_sc_stub > 0) && (m_noc_stub > 0)) begin
                     component = factory.create_component_by_name("evl_l2_agent_etlink", "", $sformatf("shire_cache_%0d", agent_id), this);
                     if (component != null) begin
                        verif_comp.set_active_slave();
                        $cast(verif_comp, component);
                        m_sc_stim_agents[agent_id] = verif_comp;
                        verif_comp.set_abstract_name($sformatf("L2"));
                        verif_comp.set_agent_id(agent_id);
                        verif_comp.set_shire_id(get_shire_id());
                        verif_comp.set_rtl_port(m_neigh_bus_agents[agent_id].get_rtl_port());
                        add_vc_child(verif_comp);
                     end
                  end
                  else if (m_neigh_stub[agent_id] != 0) begin
                     component = factory.create_component_by_name("evl_neigh_agent_etlink", "", $sformatf("neighborhood_%0d", agent_id), this);
                     if (component != null) begin
                        verif_comp.set_active_master();
                        $cast(verif_comp, component);
                        m_neigh_stim_agents[agent_id] = verif_comp;
                        verif_comp.set_abstract_name($sformatf("N%0d(STIM)", agent_id));
                        verif_comp.set_agent_id(agent_id);
                        verif_comp.set_shire_id(get_shire_id());
                        verif_comp.set_rtl_port(m_neigh_bus_agents[agent_id].get_rtl_port());
                        add_vc_child(verif_comp);
                     end
                  end
               end // if (verif_comp.get_agent_type() == ETLINK_BUS_AGENT) ------------------------}

               else if (verif_comp.get_agent_type() == DBG_NOC_BUS_AGENT) begin // ----------------{
                  if (m_dbgnoc_stim == 1) begin
                     if (dbgnoc_comp_id == MIN_DBG_DN_EVT) begin
                        component = factory.create_component_by_name("evl_stim_agent_dbg_noc", "", $sformatf("dn_evt_%0d", dbgnoc_comp_id), this);
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
                     else if (dbgnoc_comp_id == MIN_DBG_DN_MSG) begin
                        component = factory.create_component_by_name("evl_stim_agent_dbg_noc", "", $sformatf("dn_msg_%0d", dbgnoc_comp_id), this);
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
                  end
               end // if (verif_comp.get_agent_type() == DBG_NOC_BUS_AGENT) begin // --------------}

               //
               // Stimulus Verification Components attached to the AXI Bus Components
               //
               else if (verif_comp.get_agent_type() == AXI_BUS_AGENT) begin // --------------------{
                  bus_rtl_port = null;
                  if (m_noc_stub > 0) begin // ----------------------------------------------------{
                     if (axi_ifc_type == SHIRE_L2TOL3_MESH_AXI) begin
                        verif_comp.set_active_slave();
                        bus_rtl_port = m_shire_bus_agents[axi_ifc_type][layer_num].get_rtl_port();
                        if (!m_shire_stim_agents.exists(axi_ifc_type)) begin
                           component = factory.create_component_by_name("evl_minion_shire_slave_agent_axi", "", $sformatf("noc_mesh_slave"), this);
                        end
                        else begin
                           m_shire_stim_agents[axi_ifc_type].set_rtl_port(bus_rtl_port, rtl_port_num);
                           component = null;
                        end
                     end
                     else if (axi_ifc_type == SHIRE_L3TOSYS_MESH_AXI) begin
                        verif_comp.set_active_slave();
                        bus_rtl_port = m_shire_bus_agents[axi_ifc_type][0].get_rtl_port();
                        if (!m_shire_stim_agents.exists(axi_ifc_type)) begin
                           component = factory.create_component_by_name("evl_minion_shire_slave_agent_axi", "", $sformatf("noc_mesh_slave"), this);
                        end
                        else begin
                           m_shire_stim_agents[axi_ifc_type].set_rtl_port(bus_rtl_port, rtl_port_num);
                           component = null;
                        end
                     end
                     else if (axi_ifc_type == SHIRE_UCTOL3_MESH_AXI) begin
                        verif_comp.set_active_slave();
                        bus_rtl_port = m_shire_bus_agents[axi_ifc_type][0].get_rtl_port();
                        if (!m_shire_stim_agents.exists(axi_ifc_type)) begin
                           component = factory.create_component_by_name("evl_minion_shire_slave_agent_axi", "", $sformatf("noc_mesh_slave"), this);
                        end
                        else begin
                           m_shire_stim_agents[axi_ifc_type].set_rtl_port(bus_rtl_port, rtl_port_num);
                           component = null;
                        end
                     end
                     else if (axi_ifc_type == SHIRE_UCTOSYS_MESH_AXI) begin
                        verif_comp.set_active_slave();
                        bus_rtl_port = m_shire_bus_agents[axi_ifc_type][0].get_rtl_port();
                        if (!m_shire_stim_agents.exists(axi_ifc_type)) begin
                           component = factory.create_component_by_name("evl_minion_shire_slave_agent_axi", "", $sformatf("noc_mesh_slave"), this);
                        end
                        else begin
                           m_shire_stim_agents[axi_ifc_type].set_rtl_port(bus_rtl_port, rtl_port_num);
                           component = null;
                        end
                     end
                     else if (axi_ifc_type == SHIRE_MESH_TOL3_AXI) begin
                        verif_comp.set_active_master();
                        bus_rtl_port = m_shire_bus_agents[axi_ifc_type][layer_num].get_rtl_port();
                        if (!m_shire_stim_agents.exists(axi_ifc_type)) begin
                           component = factory.create_component_by_name("evl_l3_master_agent_axi", "", $sformatf("l3_mesh_master"), this);
                        end
                        else begin
                           m_shire_stim_agents[axi_ifc_type].set_rtl_port(bus_rtl_port, layer_num);
                           component = null;
                        end
                     end
                     else if (axi_ifc_type == SHIRE_FROMSYS_MESH_AXI) begin
                        verif_comp.set_active_master();
                        bus_rtl_port = m_shire_bus_agents[axi_ifc_type][0].get_rtl_port();
                        if (!m_shire_stim_agents.exists(axi_ifc_type)) begin
                           component = factory.create_component_by_name("evl_fromsys_master_agent_axi", "", $sformatf("fromsys_mesh_master"), this);
                        end
                        else begin
                           m_shire_stim_agents[axi_ifc_type].set_rtl_port(bus_rtl_port);
                           component = null;
                        end
                     end
                     else begin
                        `dut_error(get_abstract_name(), "couldn't find an AXI interface I was looking for!")
                        component = null;
                     end
                  end // if (m_noc_stub > 0) ------------------------------------------------------}
                  else if (m_sc_stub > 0) begin // ------------------------------------------------{
                     if (axi_ifc_type == SHIRE_MESH_TOL3_AXI) begin
                        verif_comp.set_active_slave();
                        bus_rtl_port = m_shire_bus_agents[axi_ifc_type][layer_num].get_rtl_port();
                        if (!m_shire_stim_agents.exists(axi_ifc_type)) begin
                           component = factory.create_component_by_name("evl_shire_slave_agent_axi", "", $sformatf("l3_mesh_slave"), this);
                        end
                        else begin
                           m_shire_stim_agents[axi_ifc_type].set_rtl_port(bus_rtl_port, layer_num);
                           component = null;
                        end
                     end
                     else if (axi_ifc_type == SHIRE_L2TOL3_MESH_AXI) begin
                        verif_comp.set_active_master();
                        bus_rtl_port = m_shire_bus_agents[axi_ifc_type][layer_num].get_rtl_port();
                        if (!m_shire_stim_agents.exists(axi_ifc_type)) begin
                           component = factory.create_component_by_name("evl_l2tol3_master_agent_axi", "", $sformatf("l2_mesh_master"), this);
                        end
                        else begin
                           m_shire_stim_agents[axi_ifc_type].set_rtl_port(bus_rtl_port, layer_num);
                           component = null;
                        end
                     end
                     else if (axi_ifc_type == SHIRE_L3TOSYS_MESH_AXI) begin
                        verif_comp.set_active_master();
                        bus_rtl_port = m_shire_bus_agents[axi_ifc_type][0].get_rtl_port();
                        if (!m_shire_stim_agents.exists(axi_ifc_type)) begin
                           component = factory.create_component_by_name("evl_tosys_master_agent_axi", "", $sformatf("sys_mesh_master"), this);
                        end
                        else begin
                           m_shire_stim_agents[axi_ifc_type].set_rtl_port(bus_rtl_port);
                           component = null;
                        end
                     end
                     else if (axi_ifc_type == SHIRE_UCTOL3_MESH_AXI) begin
                        verif_comp.set_active_master();
                        bus_rtl_port = m_shire_bus_agents[axi_ifc_type][0].get_rtl_port();
                        if (!m_shire_stim_agents.exists(axi_ifc_type)) begin
                           component = factory.create_component_by_name("evl_uctol3_master_agent_axi", "", $sformatf("uctol3_mesh_master"), this);
                        end
                        else begin
                           m_shire_stim_agents[axi_ifc_type].set_rtl_port(bus_rtl_port);
                           component = null;
                        end
                     end
                     else if (axi_ifc_type == SHIRE_UCTOSYS_MESH_AXI) begin
                        verif_comp.set_active_master();
                        bus_rtl_port = m_shire_bus_agents[axi_ifc_type][0].get_rtl_port();
                        if (!m_shire_stim_agents.exists(axi_ifc_type)) begin
                           component = factory.create_component_by_name("evl_uctosys_master_agent_axi", "", $sformatf("uctosys_mesh_master"), this);
                        end
                        else begin
                           m_shire_stim_agents[axi_ifc_type].set_rtl_port(bus_rtl_port);
                           component = null;
                        end
                     end
                     else if (axi_ifc_type == SHIRE_FROMSYS_MESH_AXI) begin
                        verif_comp.set_active_slave();
                        bus_rtl_port = m_shire_bus_agents[axi_ifc_type][0].get_rtl_port();
                        if (!m_shire_stim_agents.exists(axi_ifc_type)) begin
                           component = factory.create_component_by_name("evl_shire_slave_agent_axi", "", $sformatf("fromsys_mesh_slave"), this);
                        end
                        else begin
                           m_shire_stim_agents[axi_ifc_type].set_rtl_port(bus_rtl_port);
                           component = null;
                        end
                     end
                     else begin
                        `dut_error(get_abstract_name(), "couldn't find an AXI interface I was looking for!")
                        component = null;
                     end
                  end // else if (m_sc_stub > 0) --------------------------------------------------}
                  else begin
                     component = null;
                  end

                  if (component != null) begin // -------------------------------------------------{
                     $cast(verif_comp, component);
                     verif_comp.set_shire_id(get_shire_id());
                     verif_comp.set_port_id(shire_offset + axi_ifc_type);
                     case (axi_ifc_type)
                        SHIRE_L2TOL3_MESH_AXI: begin
                           if (m_noc_stub == 0) begin
                              verif_comp.set_abstract_name($sformatf("L2TOL3_MESH(STIM)"));
                           end
                           else begin
                              verif_comp.set_abstract_name($sformatf("NOC_MESH(STIM)"));
                              verif_comp.set_port_id(shire_offset + int'(SHIRE_L2TOL3_MESH_AXI));
                              m_shire_stim_agents[int'(SHIRE_L2TOL3_MESH_AXI)]  = verif_comp;
                              m_shire_stim_agents[int'(SHIRE_L3TOSYS_MESH_AXI)] = verif_comp;
                              m_shire_stim_agents[int'(SHIRE_UCTOL3_MESH_AXI)]  = verif_comp;
                              m_shire_stim_agents[int'(SHIRE_UCTOSYS_MESH_AXI)] = verif_comp;
                           end
                           verif_comp.set_rtl_port(bus_rtl_port, rtl_port_num);
                           verif_comp.set_int_param(pk_num_l3_ports, m_num_l3_ports);
                           verif_comp.set_int_param(pk_num_l3_master_ports, m_num_l3_master_ports);
                           verif_comp.set_int_param(pk_num_l3_slave_ports, m_num_l3_slave_ports);
                        end
                        SHIRE_L3TOSYS_MESH_AXI: begin
                           if (m_noc_stub == 0) begin
                              verif_comp.set_abstract_name($sformatf("L3TOSYS_MESH(STIM)"));
                           end
                           else begin
                              verif_comp.set_abstract_name($sformatf("NOC_MESH(STIM)"));
                              verif_comp.set_port_id(shire_offset + int'(SHIRE_L2TOL3_MESH_AXI));
                              m_shire_stim_agents[int'(SHIRE_L2TOL3_MESH_AXI)]  = verif_comp;
                              m_shire_stim_agents[int'(SHIRE_L3TOSYS_MESH_AXI)] = verif_comp;
                              m_shire_stim_agents[int'(SHIRE_UCTOL3_MESH_AXI)]  = verif_comp;
                              m_shire_stim_agents[int'(SHIRE_UCTOSYS_MESH_AXI)] = verif_comp;
                           end
                           verif_comp.set_rtl_port(bus_rtl_port, rtl_port_num);
                        end
                        SHIRE_UCTOL3_MESH_AXI: begin
                           if (m_noc_stub == 0) begin
                              verif_comp.set_abstract_name($sformatf("UCTOL3_MESH(STIM)"));
                           end
                           else begin
                              verif_comp.set_abstract_name($sformatf("NOC_MESH(STIM)"));
                              verif_comp.set_port_id(shire_offset + int'(SHIRE_L2TOL3_MESH_AXI));
                              m_shire_stim_agents[int'(SHIRE_L2TOL3_MESH_AXI)]  = verif_comp;
                              m_shire_stim_agents[int'(SHIRE_L3TOSYS_MESH_AXI)] = verif_comp;
                              m_shire_stim_agents[int'(SHIRE_UCTOL3_MESH_AXI)]  = verif_comp;
                              m_shire_stim_agents[int'(SHIRE_UCTOSYS_MESH_AXI)] = verif_comp;
                           end
                           verif_comp.set_rtl_port(bus_rtl_port, rtl_port_num);
                        end
                        SHIRE_UCTOSYS_MESH_AXI: begin
                           if (m_noc_stub == 0) begin
                              verif_comp.set_abstract_name($sformatf("UCTOSYS_MESH(STIM)"));
                           end
                           else begin
                              verif_comp.set_abstract_name($sformatf("NOC_MESH(STIM)"));
                              verif_comp.set_port_id(shire_offset + int'(SHIRE_L2TOL3_MESH_AXI));
                              m_shire_stim_agents[int'(SHIRE_L2TOL3_MESH_AXI)]  = verif_comp;
                              m_shire_stim_agents[int'(SHIRE_L3TOSYS_MESH_AXI)] = verif_comp;
                              m_shire_stim_agents[int'(SHIRE_UCTOL3_MESH_AXI)]  = verif_comp;
                              m_shire_stim_agents[int'(SHIRE_UCTOSYS_MESH_AXI)] = verif_comp;
                           end
                           verif_comp.set_rtl_port(bus_rtl_port, rtl_port_num);
                        end
                        SHIRE_MESH_TOL3_AXI: begin
                           if(m_sc_stub > 0) begin
                              verif_comp.set_int_param(pk_allow_loopback, 1);
                           end
                           verif_comp.set_abstract_name($sformatf("MESH_TOL3(STIM)"));
                           verif_comp.set_rtl_port(bus_rtl_port, rtl_port_num);
                           verif_comp.set_int_param(pk_num_l3_ports, m_num_l3_ports);
                           verif_comp.set_int_param(pk_num_l3_master_ports, m_num_l3_master_ports);
                           verif_comp.set_int_param(pk_num_l3_slave_ports, m_num_l3_slave_ports);
                        end
                        SHIRE_FROMSYS_MESH_AXI: begin
                           verif_comp.set_abstract_name($sformatf("FROMSYS_MESH(STIM)"));
                           verif_comp.set_rtl_port(bus_rtl_port, rtl_port_num);
                        end
                        default: begin
                           verif_comp.set_rtl_port(bus_rtl_port, rtl_port_num);
                        end
                     endcase
                     m_shire_stim_agents[axi_ifc_type] = verif_comp;
                     add_vc_child(verif_comp);
                  end // if (component != null) ---------------------------------------------------}
               end // else if (verif_comp.get_agent_type() == AXI_BUS_AGENT) ----------------------}

               //
               // Stimulus Verification Components attached to the APB Bus Components
               //
               else if (verif_comp.get_agent_type() == APB_BUS_AGENT) begin // --------------------{
                  if (m_neigh_stub[agent_id] == 1'b1) begin
                     //
                     // If the neighborhood is a stub, we need to create a slave APB agent to fake
                     // out APB responses.
                     //
                     component = factory.create_component_by_name("evl_reg_slave_agent_apb", "", $sformatf("apb_%0d", agent_id), this);
                     if (component != null) begin
                        verif_comp.set_active_slave();
                        $cast(verif_comp, component);
                        m_apb_stim_agents[agent_id] = verif_comp;
                        verif_comp.set_abstract_name($sformatf("N%0d:ESR(STIM)", agent_id));
                        verif_comp.set_agent_id(agent_id);
                        verif_comp.set_shire_id(get_shire_id());
                        verif_comp.set_neighborhood_id(agent_id);
                        verif_comp.set_rtl_port(m_apb_bus_agents[agent_id].get_rtl_port());
                        add_vc_child(verif_comp);
                     end
                  end
               end // if (verif_comp.get_agent_type() == APB_BUS_AGENT) ---------------------------}
            end // if (verif_comp.is_bus_vc() == 1'b1) --------------------------------------------}

            //
            // RTL Verification Components
            //
            else begin // -------------------------------------------------------------------------{
               if (verif_comp.get_agent_type() == SHIRE_CHANNEL_RTL_AGENT) begin
                  m_shire_channel_agent = verif_comp;
                  verif_comp.set_int_param(pk_num_l3_ports, m_num_l3_ports);
                  verif_comp.set_int_param(pk_num_l3_master_ports, m_num_l3_master_ports);
                  verif_comp.set_int_param(pk_num_l3_slave_ports, m_num_l3_slave_ports);
                  verif_comp.set_shire_id(get_shire_id());
               end
               else if (verif_comp.get_agent_type() == NEIGH_RTL_AGENT) begin
                  m_neighborhood_agents[agent_id] = verif_comp;
                  verif_comp.set_shire_id(get_shire_id());
                  verif_comp.set_neighborhood_id(agent_id);
                  verif_comp.set_agent_id(agent_id);
                  verif_comp.set_abstract_name($sformatf("N%0d", agent_id));
               end
               else if (agent_id != ~evl_agent_id_t'(0)) begin
                  verif_comp.set_shire_id(get_shire_id());
                  verif_comp.set_agent_id(agent_id);
                  verif_comp.set_abstract_name($sformatf("%0s-%0d", verif_comp.get_root_abstract_name(), agent_id));
               end
               add_vc_child(verif_comp);
            end // else if (!(verif_comp.is_bus_vc() == 1'b1)) ------------------------------------}
         end // if (component != null) ------------------------------------------------------------}
      end // foreach (instance_list[ii]) ----------------------------------------------------------}

      if (m_shire_channel_agent != null) begin
         m_shire_channel_agent.set_int_param(pk_upstream_req_port_count, m_neigh_bus_agents.size());
         m_shire_channel_agent.set_int_param(pk_axi_port_mask, axi_port_mask);
         foreach (m_neighborhood_agents[ii]) begin
            case (int'(ii))
               0: m_shire_channel_agent.set_object_param(pk_neigh_agent_0, m_neighborhood_agents[ii]);
               1: m_shire_channel_agent.set_object_param(pk_neigh_agent_1, m_neighborhood_agents[ii]);
               2: m_shire_channel_agent.set_object_param(pk_neigh_agent_2, m_neighborhood_agents[ii]);
               3: m_shire_channel_agent.set_object_param(pk_neigh_agent_3, m_neighborhood_agents[ii]);
            endcase
            m_neighborhood_agents[ii].set_object_param(pk_shire_channel_agent, m_shire_channel_agent);
            m_neighborhood_agents[ii].set_object_param(pk_shire_agent, this);
         end
         m_shire_channel_agent.set_object_param(pk_shire_agent, this);
         m_shire_channel_agent.set_int_param(pk_etlink_master_count, m_neigh_bus_agents.size());
         m_shire_channel_agent.set_int_param(pk_uc_scoreboard_disable, int'(m_disable_uc_sb));
      end

      // If this is an AXI stub Shire, load some dummy values into the Shire Cache config so the top level scoreboard
      // can access address decoding functions in m_global_attributes
      if (((m_sc_stub == 1) && (m_noc_stub == 0)) || ((m_sc_lite == 1) && (m_noc_stub == 0))) begin
         m_global_attrs.set_l3_config(get_shire_id(), 1'b0, 1'b1, 1, evl_paddr_t'(0), ~evl_paddr_t'(0), evl_paddr_t'(0), evl_paddr_t'(0));
         m_global_attrs.set_scp_config(get_shire_id(), 1'b1, 1, evl_paddr_t'(0), ~evl_paddr_t'(0), evl_paddr_t'(0), evl_paddr_t'(0));
      end

      //
      // Create a root register model.
      //
      reg_model = new($sformatf("S%0d", get_shire_id()));
      reg_model.set_agent(this);
      set_evl_reg_block(reg_model);

      //
      // Create an AXI address map of the entire register space.
      //
      void'(reg_model.create_map("AXI", evl_paddr_t'(0), 8, UVM_LITTLE_ENDIAN, 0));
   endfunction : build_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Function: connect_phase
   //
   virtual function void connect_phase(uvm_phase phase);
      super.connect_phase(phase);
      foreach (m_neigh_stim_agents[ii]) begin
         m_neigh_stim_agents[ii].connect_my_bus_ports(m_neigh_bus_agents[ii]);
      end
      foreach (m_sc_stim_agents[ii]) begin
         m_sc_stim_agents[ii].connect_my_bus_ports(m_neigh_bus_agents[ii]);
      end
      foreach (m_apb_stim_agents[ii]) begin
         m_apb_stim_agents[ii].connect_my_bus_ports(m_apb_bus_agents[ii]);
      end
      foreach (m_shire_stim_agents[ii]) begin
         for (int jj = 0; jj < m_shire_bus_agents[ii].size(); jj++) begin
            if (m_shire_bus_agents[ii][jj] != null) begin
               if ( (m_noc_stub > 0) &&
                    ( (ii == int'(SHIRE_L2TOL3_MESH_AXI))  ||
                      (ii == int'(SHIRE_L3TOSYS_MESH_AXI)) ||
                      (ii == int'(SHIRE_UCTOL3_MESH_AXI))  ||
                      (ii == int'(SHIRE_UCTOSYS_MESH_AXI)) ) ) begin
                  evl_shire_axi_ifc_type_t axi_type = evl_shire_axi_ifc_type_t'(ii);

                  m_shire_stim_agents[ii].connect_my_bus_ports(m_shire_bus_agents[ii][jj], $sformatf("%p", axi_type), jj);
               end
               else begin
                  m_shire_stim_agents[ii].connect_my_bus_ports(m_shire_bus_agents[ii][jj], "", jj);
               end
            end
         end
      end
      foreach (m_neighborhood_agents[ii]) begin
         m_neighborhood_agents[ii].connect_port_to_my_export(m_neigh_bus_agents[ii].get_req_desc_port("m_obs_req_port"), "m_shire_req_export", ii);
         m_neighborhood_agents[ii].connect_port_to_my_export(m_neigh_bus_agents[ii].get_req_desc_port("m_obs_rsp_port"), "m_shire_rsp_export", ii);
      end
      if (m_shire_channel_agent != null) begin
         foreach (m_neigh_bus_agents[ii]) begin
            m_shire_channel_agent.connect_port_to_my_export(m_neigh_bus_agents[ii].get_req_desc_port("m_obs_req_port"), "m_upstream_req_export", ii);
         end
         foreach (m_shire_bus_agents[port_type]) begin
            for (int ii = 0; ii < m_shire_bus_agents[port_type].size(); ii++) begin
               if (m_shire_bus_agents[port_type][ii] != null) begin
                  m_shire_channel_agent.connect_port_to_my_export(m_shire_bus_agents[port_type][ii].get_req_desc_port("m_obs_req_port"), "m_axi_req_export", port_type, ii);
                  m_shire_channel_agent.connect_port_to_my_export(m_shire_bus_agents[port_type][ii].get_req_desc_port("m_obs_rsp_port"), "m_axi_rsp_export", port_type, ii);
               end
            end
         end
      end
      foreach (m_dbgnoc_bus_agents[ii]) begin
         if ((m_dbgnoc_bus_agents[ii] != null) && m_dbgnoc_stim_agents.exists(ii) && (m_dbgnoc_stim_agents[ii] != null)) begin
            m_dbgnoc_stim_agents[ii].connect_my_bus_ports(m_dbgnoc_bus_agents[ii]);
         end
      end

      //
      // Setup the agent loopbacks
      //
      if (m_noc_stub > 0) begin
         //
         // There is a special case for an L2-to-L3 mesh interface and its companion mesh-to-L3
         // interface ... to handle the loopback case, we need to forward some L2-to-L3 mesh
         // requests to the mesh-to-L3 agent.
         //
         if (m_shire_stim_agents.exists(int'(SHIRE_L2TOL3_MESH_AXI)) && m_shire_stim_agents.exists(int'(SHIRE_MESH_TOL3_AXI))) begin
            for (int ii = 0; ii < m_num_l3_master_ports; ii++) begin
               m_shire_stim_agents[int'(SHIRE_MESH_TOL3_AXI)].connect_port_to_my_export(m_shire_stim_agents[int'(SHIRE_L2TOL3_MESH_AXI)].get_req_desc_port("SHIRE_L3_LOOPBACK", ii), "m_obs_req_export", ii);
            end
            for (int ii = 0; ii < m_num_l3_slave_ports; ii++) begin
               m_shire_stim_agents[int'(SHIRE_L2TOL3_MESH_AXI)].connect_port_to_my_export(m_shire_stim_agents[int'(SHIRE_MESH_TOL3_AXI)].get_req_desc_port("m_snd_rsp_port", ii), "SHIRE_L3_LOOPBACK", ii);
            end
         end

         //
         // The system mesh slave (which receives requests from the shire cache) receives atomic
         // completion write (QOS 2) from the shire cache ... these need to be forwarded to the
         // system mesh master.
         //
         if (m_shire_stim_agents.exists(int'(SHIRE_L3TOSYS_MESH_AXI)) && m_shire_stim_agents.exists(int'(SHIRE_FROMSYS_MESH_AXI))) begin
            m_shire_stim_agents[int'(SHIRE_FROMSYS_MESH_AXI)].connect_port_to_my_export(m_shire_stim_agents[int'(SHIRE_L3TOSYS_MESH_AXI)].get_req_desc_port("SHIRE_SYS_LOOPBACK", 0), "m_obs_sys_req_export", 0);
            m_shire_stim_agents[int'(SHIRE_L3TOSYS_MESH_AXI)].connect_port_to_my_export(m_shire_stim_agents[int'(SHIRE_FROMSYS_MESH_AXI)].get_req_desc_port("m_snd_sys_rsp_port", 0), "SHIRE_SYS_LOOPBACK", 0);
         end

         //
         // The uncached L3 mesh slave can forward requests to the 0th mesh-to-L3 master.
         //
         if (m_shire_stim_agents.exists(int'(SHIRE_UCTOL3_MESH_AXI)) && m_shire_stim_agents.exists(int'(SHIRE_MESH_TOL3_AXI))) begin
            m_shire_stim_agents[int'(SHIRE_MESH_TOL3_AXI)].connect_port_to_my_export(m_shire_stim_agents[int'(SHIRE_UCTOL3_MESH_AXI)].get_req_desc_port("SHIRE_UC_L3_LOOPBACK", 0), "m_obs_uc_req_export", 0);
            m_shire_stim_agents[int'(SHIRE_UCTOL3_MESH_AXI)].connect_port_to_my_export(m_shire_stim_agents[int'(SHIRE_MESH_TOL3_AXI)].get_req_desc_port("m_snd_uc_rsp_port", 0), "SHIRE_UC_L3_LOOPBACK", 0);
         end

         //
         // The uncached L3 mesh slave can forward atomic "response" requests to the mesh-to-sys master.
         // The uncached system mesh slave can forward requests to the mesh-to-system master.
         //
         if (m_shire_stim_agents.exists(int'(SHIRE_UCTOSYS_MESH_AXI)) && m_shire_stim_agents.exists(int'(SHIRE_FROMSYS_MESH_AXI))) begin
            m_shire_stim_agents[int'(SHIRE_FROMSYS_MESH_AXI)].connect_port_to_my_export(m_shire_stim_agents[int'(SHIRE_UCTOSYS_MESH_AXI)].get_req_desc_port("SHIRE_UC_SYS_LOOPBACK", 0), "m_obs_uc_req_export", 0);
            m_shire_stim_agents[int'(SHIRE_UCTOSYS_MESH_AXI)].connect_port_to_my_export(m_shire_stim_agents[int'(SHIRE_FROMSYS_MESH_AXI)].get_req_desc_port("m_snd_uc_rsp_port", 0), "SHIRE_UC_SYS_LOOPBACK", 0);
         end
      end // if (m_noc_stub > 0)
      else if (m_sc_stub > 0) begin
         //
         // There is a special case for an L2-to-L3 mesh interface and its companion mesh-to-L3
         // interface ... to handle the loopback case, we need to forward some L2-to-L3 mesh
         // requests to the mesh-to-L3 agent.
         //
         if (m_shire_stim_agents.exists(int'(SHIRE_L3TOSYS_MESH_AXI)) && m_shire_stim_agents.exists(int'(SHIRE_MESH_TOL3_AXI))) begin
            m_shire_stim_agents[int'(SHIRE_L3TOSYS_MESH_AXI)].connect_port_to_my_export(m_shire_stim_agents[int'(SHIRE_MESH_TOL3_AXI)].get_req_desc_port("m_loopback_req_port", 0), "m_obs_req_export", 0);
            m_shire_stim_agents[int'(SHIRE_MESH_TOL3_AXI)].connect_port_to_my_export(m_shire_stim_agents[int'(SHIRE_L3TOSYS_MESH_AXI)].get_req_desc_port("m_snd_rsp_port", 0), "m_loopback_rsp_export", 0);
         end
      end

      if ((m_sc_stub == 0) && (m_disable_uc_sb == 1'b0) && (m_shire_channel_agent != null)) begin
         `evl_log(UVM_MEDIUM, VMOD_CONFIG, get_abstract_name(), $sformatf("Connect the UCBlock scoreboard to bus agent ports"))

         // Connect the UC scoreboard to the etlink master agent
         foreach (m_neigh_bus_agents[ii]) begin
            m_neigh_bus_agents[ii].connect_my_port_to_export(m_shire_channel_agent.get_req_desc_export("m_ucblock_etlink_req_export", ii), "m_obs_req_port");
            m_neigh_bus_agents[ii].connect_my_port_to_export(m_shire_channel_agent.get_req_desc_export("m_ucblock_etlink_rsp_export", ii), "m_obs_rsp_port");
         end

         // Connect the UC scoreboard to the AXI to_L3 and to_sys slave agent
         if ( m_shire_bus_agents.exists(int'(SHIRE_UCTOSYS_MESH_AXI)) &&
             (m_shire_bus_agents[int'(SHIRE_UCTOSYS_MESH_AXI)].size() > 0) &&
             (m_shire_bus_agents[int'(SHIRE_UCTOSYS_MESH_AXI)][0] != null) ) begin
            m_shire_bus_agents[int'(SHIRE_UCTOSYS_MESH_AXI)][0].connect_my_port_to_export(m_shire_channel_agent.get_req_desc_export("m_ucblock_sysaxi_req_export", 0), "m_obs_req_port");
            m_shire_bus_agents[int'(SHIRE_UCTOSYS_MESH_AXI)][0].connect_my_port_to_export(m_shire_channel_agent.get_req_desc_export("m_ucblock_sysaxi_rsp_export", 0), "m_obs_rsp_port");
         end
         if ( m_shire_bus_agents.exists(int'(SHIRE_UCTOL3_MESH_AXI)) &&
             (m_shire_bus_agents[int'(SHIRE_UCTOL3_MESH_AXI)].size() > 0) &&
             (m_shire_bus_agents[int'(SHIRE_UCTOL3_MESH_AXI)][0] != null) ) begin
            m_shire_bus_agents[int'(SHIRE_UCTOL3_MESH_AXI)][0].connect_my_port_to_export(m_shire_channel_agent.get_req_desc_export("m_ucblock_l3axi_req_export", 0), "m_obs_req_port");
            m_shire_bus_agents[int'(SHIRE_UCTOL3_MESH_AXI)][0].connect_my_port_to_export(m_shire_channel_agent.get_req_desc_export("m_ucblock_l3axi_rsp_export", 0), "m_obs_rsp_port");
         end
         if ( m_shire_bus_agents.exists(int'(SHIRE_FROMSYS_MESH_AXI)) &&
             (m_shire_bus_agents[int'(SHIRE_FROMSYS_MESH_AXI)].size() > 0) &&
             (m_shire_bus_agents[int'(SHIRE_FROMSYS_MESH_AXI)][0] != null) ) begin
            m_shire_bus_agents[int'(SHIRE_FROMSYS_MESH_AXI)][0].connect_my_port_to_export(m_shire_channel_agent.get_req_desc_export("m_ucblock_fromsysaxi_req_export", 0), "m_obs_req_port");
            m_shire_bus_agents[int'(SHIRE_FROMSYS_MESH_AXI)][0].connect_my_port_to_export(m_shire_channel_agent.get_req_desc_export("m_ucblock_fromsysaxi_rsp_export", 0), "m_obs_rsp_port");
         end
      end // if (m_shire_channel_agent != null)

   endfunction : connect_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Function: start_of_simulation_phase
   //
   virtual function void start_of_simulation_phase(uvm_phase phase);
      super.start_of_simulation_phase(phase);

      //
      // Notify the ET link bus agents who their master and slave port agents are.
      //
      foreach (m_neigh_bus_agents[ii]) begin
         m_neigh_bus_agents[ii].set_int_param(pk_allow_unique_tags, 1);
         if (m_neigh_stim_agents.exists(ii) && (m_neigh_stim_agents[ii] != null)) begin
            m_neigh_bus_agents[ii].set_master_port_agent(m_neigh_stim_agents[ii]);
         end
         else if (m_neighborhood_agents.exists(ii) && (m_neighborhood_agents[ii] != null)) begin
            m_neigh_bus_agents[ii].set_master_port_agent(m_neighborhood_agents[ii]);
         end
         if (m_sc_stim_agents.exists(ii) && (m_sc_stim_agents[ii] != null)) begin
            m_neigh_bus_agents[ii].set_slave_port_agent(m_sc_stim_agents[ii]);
         end
         else begin
            m_neigh_bus_agents[ii].set_slave_port_agent(this);
         end
      end

      foreach (m_dbgnoc_bus_agents[ii]) begin
         if (m_dbgnoc_stim_agents.exists(ii) && (m_dbgnoc_stim_agents[ii] != null)) begin
            m_dbgnoc_bus_agents[ii].set_master_port_agent(m_dbgnoc_stim_agents[ii]);
         end
      end

      //
      // Notify the AXI bus agents who their master and slave port agents are.
      //
      if ((m_sc_stub > 0) && (m_noc_stub == 0)) begin
         foreach (m_shire_bus_agents[port_type]) begin
            if (m_shire_stim_agents.exists(port_type)) begin
               if (m_shire_stim_agents[port_type].is_active_master() == 1'b1) begin
                  for (int ii = 0; ii < m_shire_bus_agents[port_type].size(); ii++) begin
                     if (m_shire_bus_agents[port_type][ii] != null) begin
                        m_shire_bus_agents[port_type][ii].set_master_port_agent(m_shire_stim_agents[port_type]);
                     end
                  end
               end
               if (m_shire_stim_agents[port_type].is_active_slave() == 1'b1) begin
                  if (m_master_port_agent != null) begin
                     for (int ii = 0; ii < m_shire_bus_agents[port_type].size(); ii++) begin
                        if (m_shire_bus_agents[port_type][ii] != null) begin
                           m_shire_bus_agents[port_type][ii].set_slave_port_agent(m_shire_stim_agents[port_type]);
                           m_shire_bus_agents[port_type][ii].set_master_port_agent(m_master_port_agent);
                        end
                     end
                  end
                  else begin
                     for (int ii = 0; ii < m_shire_bus_agents[port_type].size(); ii++) begin
                        if (m_shire_bus_agents[port_type][ii] != null) begin
                           m_shire_bus_agents[port_type][ii].set_slave_port_agent(m_shire_stim_agents[port_type]);
                        end
                     end
                  end
               end
            end
         end
      end
      else begin
         //
         // Notify all of the SHIRE_L2TOL3_MESH_AXI bus agents who their master port agent is.  The
         // master port agent is a shire cache scoreboard (if it exists).
         //
         if ((m_shire_channel_agent != null) && (m_shire_bus_agents.exists(int'(SHIRE_L2TOL3_MESH_AXI)))) begin
            for (int ii = 0; ii < m_shire_bus_agents[int'(SHIRE_L2TOL3_MESH_AXI)].size(); ii++) begin
               if (m_shire_bus_agents[int'(SHIRE_L2TOL3_MESH_AXI)][ii] != null) begin
                  m_shire_bus_agents[int'(SHIRE_L2TOL3_MESH_AXI)][ii].set_master_port_agent(m_shire_channel_agent.get_master_port_agent("m_axi_req_export", int'(SHIRE_L2TOL3_MESH_AXI)));
               end
            end
         end

         //
         // Notify all of the SHIRE_L2TOL3_MESH_AXI bus agents who their slave port agent is.  The
         // slave port agent is a stimulus agent (if it exists).
         //
         if (m_shire_stim_agents.exists(int'(SHIRE_L2TOL3_MESH_AXI)) && (m_shire_stim_agents[int'(SHIRE_L2TOL3_MESH_AXI)] != null)) begin
            for (int ii = 0; ii < m_shire_bus_agents[int'(SHIRE_L2TOL3_MESH_AXI)].size(); ii++) begin
               if (m_shire_bus_agents[int'(SHIRE_L2TOL3_MESH_AXI)][ii] != null) begin
                  m_shire_bus_agents[int'(SHIRE_L2TOL3_MESH_AXI)][ii].set_slave_port_agent(m_shire_stim_agents[int'(SHIRE_L2TOL3_MESH_AXI)]);
               end
            end
         end

         //
         // Notify all of the SHIRE_L3TOSYS_MESH_AXI bus agents who their master port agent is.  The
         // master port agent is a shire cache scoreboard (if it exists).
         //
         if ((m_shire_channel_agent != null) && (m_shire_bus_agents.exists(int'(SHIRE_L3TOSYS_MESH_AXI)))) begin
            if (m_shire_bus_agents[int'(SHIRE_L3TOSYS_MESH_AXI)][0] != null) begin
               m_shire_bus_agents[int'(SHIRE_L3TOSYS_MESH_AXI)][0].set_master_port_agent(m_shire_channel_agent.get_master_port_agent("m_axi_req_export", int'(SHIRE_L3TOSYS_MESH_AXI)));
            end
         end

         //
         // Notify all of the SHIRE_L3TOSYS_MESH_AXI bus agents who their slave port agent is.  The
         // slave port agent is a stimulus agent (if it exists).
         //
         if (m_shire_stim_agents.exists(int'(SHIRE_L3TOSYS_MESH_AXI)) && (m_shire_stim_agents[int'(SHIRE_L3TOSYS_MESH_AXI)] != null)) begin
            if (m_shire_bus_agents[int'(SHIRE_L3TOSYS_MESH_AXI)][0] != null) begin
               m_shire_bus_agents[int'(SHIRE_L3TOSYS_MESH_AXI)][0].set_slave_port_agent(m_shire_stim_agents[int'(SHIRE_L3TOSYS_MESH_AXI)]);
            end
         end

         //
         // Notify all of the SHIRE_MESH_TOL3_AXI bus agents who their slave port agent is.  The
         // slave port agent is a shire cache scoreboard (if it exists).
         //
         if ((m_shire_channel_agent != null) && (m_shire_bus_agents.exists(int'(SHIRE_MESH_TOL3_AXI)))) begin
            for (int ii = 0; ii < m_shire_bus_agents[int'(SHIRE_MESH_TOL3_AXI)].size(); ii++) begin
               if (m_shire_bus_agents[int'(SHIRE_MESH_TOL3_AXI)][ii] != null) begin
                  m_shire_bus_agents[int'(SHIRE_MESH_TOL3_AXI)][ii].set_slave_port_agent(m_shire_channel_agent.get_slave_port_agent("m_axi_req_export", int'(SHIRE_MESH_TOL3_AXI)));
               end
            end
         end

         //
         // Notify all of the SHIRE_MESH_TOL3_AXI bus agents who their master port agent is.  If
         // there is no stimulus agent driving the SHIRE_MESH_TOL3_AXI bus agent, the master port
         // agent becomes m_master_port_agent.
         //
         if (m_shire_stim_agents.exists(int'(SHIRE_MESH_TOL3_AXI))) begin
            for (int ii = 0; ii < m_shire_bus_agents[int'(SHIRE_MESH_TOL3_AXI)].size(); ii++) begin
               if (m_shire_bus_agents[int'(SHIRE_MESH_TOL3_AXI)][ii] != null) begin
                  m_shire_bus_agents[int'(SHIRE_MESH_TOL3_AXI)][ii].set_master_port_agent(m_shire_stim_agents[int'(SHIRE_MESH_TOL3_AXI)]);
               end
            end
         end
         else if ((m_master_port_agent != null) && (m_shire_bus_agents.exists(int'(SHIRE_MESH_TOL3_AXI)))) begin
            for (int ii = 0; ii < m_shire_bus_agents[int'(SHIRE_MESH_TOL3_AXI)].size(); ii++) begin
               if (m_shire_bus_agents[int'(SHIRE_MESH_TOL3_AXI)][ii] != null) begin
                  m_shire_bus_agents[int'(SHIRE_MESH_TOL3_AXI)][ii].set_master_port_agent(m_master_port_agent);
               end
            end
         end

         //
         // Notify all of the SHIRE_FROMSYS_MESH_AXI bus agents who their master port agent is.  The master
         // port agent either a stimulus agent or m_master_port_agent.
         //
         if (m_shire_stim_agents.exists(int'(SHIRE_FROMSYS_MESH_AXI)) && (m_shire_stim_agents[int'(SHIRE_FROMSYS_MESH_AXI)] != null)) begin
            if (m_shire_bus_agents[int'(SHIRE_FROMSYS_MESH_AXI)][0] != null) begin
               m_shire_bus_agents[int'(SHIRE_FROMSYS_MESH_AXI)][0].set_master_port_agent(m_shire_stim_agents[int'(SHIRE_FROMSYS_MESH_AXI)]);
            end
         end
         else if ((m_master_port_agent != null) && (m_shire_bus_agents.exists(int'(SHIRE_FROMSYS_MESH_AXI)))) begin
            if (m_shire_bus_agents[int'(SHIRE_FROMSYS_MESH_AXI)][0] != null) begin
               m_shire_bus_agents[int'(SHIRE_FROMSYS_MESH_AXI)][0].set_master_port_agent(m_master_port_agent);
            end
         end

         // connect the master ports
         if ((m_shire_channel_agent != null) && (m_disable_uc_sb == 0)) begin
            if (m_shire_bus_agents[int'(SHIRE_UCTOSYS_MESH_AXI)][0] != null) begin
               m_shire_bus_agents[int'(SHIRE_UCTOSYS_MESH_AXI)][0].set_master_port_agent(this);
            end
            if (m_shire_bus_agents[int'(SHIRE_UCTOL3_MESH_AXI)][0] != null) begin
               m_shire_bus_agents[int'(SHIRE_UCTOL3_MESH_AXI)][0].set_master_port_agent(this);
            end
         end
      end
   endfunction : start_of_simulation_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_req_desc_port
   //
   // The AXI interfaces may have multiple layers per port.  Different layers for a particular port
   // are chosen by the port name (a "_<number>" suffix identifies the layer).
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
         if ( !m_shire_bus_agents.exists(port_number) ||
              (m_shire_bus_agents[port_number].size() <= subport_number) ||
              (m_shire_bus_agents[port_number][subport_number] == null) ) begin
            return null;
         end
         if (port_name == "m_axi_req_port") begin
            return m_shire_bus_agents[port_number][subport_number].get_req_desc_port("m_obs_req_port");
         end
         if (port_name == "m_axi_rsp_port") begin
            return m_shire_bus_agents[port_number][subport_number].get_req_desc_port("m_obs_rsp_port");
         end
      end
      return null;
   endfunction : get_req_desc_port


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
      if (m_shire_channel_agent == null) begin
         return null;
      end
      if ((port_in == int'(SHIRE_UCTOSYS_MESH_AXI)) || (port_in == int'(SHIRE_UCTOL3_MESH_AXI))) begin
         return m_shire_channel_agent.find_observed_request(port_type_in, port_in, subport_in, paired_req_desc, bus_cmd_in, address_in, req_id_in, source_id_in, trans_id_in);
      end
   endfunction : find_observed_request


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_cache_set
   //
   virtual function int get_cache_set(input evl_paddr_t caddr,
                                      input int         cway  = 0,
                                      input int         ctype = 0);
      if (m_shire_channel_agent == null) begin
         return -1;
      end
      return m_shire_channel_agent.get_cache_set(caddr, cway, ctype);
   endfunction : get_cache_set


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_cache_bank
   //
   virtual function int get_cache_bank(input evl_paddr_t caddr,
                                       input int         cway  = 0,
                                       input int         ctype = 0);
      if (m_shire_channel_agent == null) begin
         return -1;
      end
      return m_shire_channel_agent.get_cache_bank(caddr, cway, ctype);
   endfunction : get_cache_bank


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_cache_subbank
   //
   virtual function int get_cache_subbank(input evl_paddr_t caddr,
                                          input int         cway  = 0,
                                          input int         ctype = 0);
      if (m_shire_channel_agent == null) begin
         return -1;
      end
      return m_shire_channel_agent.get_cache_subbank(caddr, cway, ctype);
   endfunction : get_cache_subbank


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_cache_tag
   //
   virtual function evl_paddr_t get_cache_tag(input int cindex,
                                              input int cway  = 0,
                                              input int ctype = 0);
      if (m_shire_channel_agent == null) begin
         return ~evl_paddr_t'(0);
      end
      return m_shire_channel_agent.get_cache_tag(cindex, cway, ctype);
   endfunction : get_cache_tag


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_cache_data
   //
   virtual function int get_cache_data(output evl_dword_t cdata[],
                                       input  int         cindex,
                                       input  int         cway  = 0,
                                       input  int         ctype = 0);
      if (m_shire_channel_agent == null) begin
         cdata = new[`EVL_LINE_DWORDS];
         for (int ii = 0; ii < `EVL_LINE_DWORDS; ii++) begin
            cdata[ii] = ~evl_dword_t'(0);
         end
         return -1;
      end
      return m_shire_channel_agent.get_cache_data(cdata, cindex, cway, ctype);
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
      if (m_shire_channel_agent == null) begin
         return -1;
      end
      return m_shire_channel_agent.set_cache_bit_err(cindex, cbit0, cbit1, cway, ctype, cerr);
   endfunction : set_cache_bit_err


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string name_in = "evl_shire_env", input uvm_component parent_in = null);
      int value;

      super.new(name_in, parent_in, SHIRE_RTL_AGENT);
      set_abstract_name("SHIRE");
      add_agent_alias(SHIRE_AGENT);
      if (`et_value_plusargs("uc_sb_en=%d", value) == 0) begin
         value = 1;
      end
      if (`et_test_plusargs("noNeighErrorResponse") != 0) begin
         set_int_param(pk_no_error_response, 1);
      end
      m_disable_uc_sb = (value == 0) ? 1'b1 : 1'b0;
   endfunction : new

endclass : evl_shire_env

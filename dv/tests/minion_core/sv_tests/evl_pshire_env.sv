//--------------------------------------------------------------------------------------------------
//
// Class: evl_pshire_env
//
class evl_pshire_env extends evl_verif_comp;

   `uvm_component_utils(evl_pshire_env)


   //-----------------------------------------------------------------------------------------------
   //
   // AXI Bus Types
   //
   // The following enumerated types are used to identify AXI bus agents in an associative array.
   // These bus agents can then be connected to master or slave stimulus agents as necessary during
   // the connect phase.
   //


   //-----------------------------------------------------------------------------------------------
   //
   // APB Bus Types
   //
   // The following enumerated types are used to identify APB bus agents in an associative array.
   // These bus agents can then be connected to master or slave stimulus agents as necessary during
   // the connect phase.
   //
   typedef enum int {
         APBSLV
      } pshire_apb_bus_t;


   //-----------------------------------------------------------------------------------------------
   //
   // ETLink Bus Types
   //
   // The following enumerated types are used to identify ETLink bus agents in an associative array.
   // These bus agents can then be connected to master or slave stimulus agents as necessary during
   // the connect phase.
   //
   typedef enum int {
         SPIO_E2A_ETLINK,
         SPIO_NONE_ETLINK
      } pshire_etlink_bus_t;


   //-----------------------------------------------------------------------------------------------
   //
   // Static Member Variables and Objects
   //
   `evl_param_key(pk_sp_is_stub)
   `evl_param_key(pk_noc_is_stub)
   `evl_param_key(pk_pshire_stim)
   `evl_param_key(pk_num_l3_ports)
   `evl_param_key(pk_pshire_noc_only)
   `evl_param_key(pk_dbgnoc_stim)


   //-----------------------------------------------------------------------------------------------
   //
   // Member Variables and Objects
   //
   evl_pshire_rtl_port m_pshire_rtl_port;
   evl_verif_comp_q    m_axi_bus_agents[int];
   evl_verif_comp      m_axi_stim_agents[int];
   evl_verif_comp      m_apb_bus_agents[evl_agent_id_t];
   evl_verif_comp      m_apb_stim_agents[evl_agent_id_t];
   evl_verif_comp      m_pshire_noc_agent;

   evl_verif_comp      m_dbgnoc_bus_agents[int];
   evl_verif_comp      m_dbgnoc_stim_agents[int];

   int                 m_pshire_stim;
   int                 m_num_l3_ports;
   int                 m_pshire_noc_only;
   int                 m_dbgnoc_stim;


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
      evl_shire_id_t   shire_id;
      int              sp_is_stub;
      int              noc_is_stub;
      int              axi_comp_id;
      int              apb_comp_id;
      int              dbgnoc_comp_id;
      int              layer_num;
      int              layer_offset;

      super.build_phase(phase);
      factory  = uvm_factory::get();
      shire_id = get_shire_id();
      if (m_rtl_port == null) begin
         `dut_error(get_abstract_name(), $sformatf("%0s: m_rtl_port is null, SAD!", get_abstract_name()))
         return;
      end
      $cast(m_pshire_rtl_port, m_rtl_port);

      //
      // If the service processor is a stub, then sp_is_stub will be 1.
      //
      sp_is_stub  = m_rtl_port.get_int_param(pk_sp_is_stub);

      //
      // If the NoC is a stub, then noc_is_stub will be 1.
      //
      noc_is_stub       = m_rtl_port.get_int_param(pk_noc_is_stub);
      m_num_l3_ports    = m_rtl_port.get_int_param(pk_num_l3_ports, 1);
      m_pshire_noc_only = m_rtl_port.get_int_param(pk_pshire_noc_only);
      m_dbgnoc_stim     = m_rtl_port.get_int_param(pk_dbgnoc_stim, 0);

      //
      // Start by creating null pointers for all of the possible AXI bus agents.
      //
      for (int ii = 0; ii < m_num_l3_ports; ii++) begin
         m_axi_bus_agents[int'(PSHIRE_TOL3_AXI)].push_back(null);
      end
      m_axi_bus_agents[int'(PSHIRE_PS_AXI)].push_back(null);
      m_axi_bus_agents[int'(PSHIRE_TOSYS_AXI)].push_back(null);
      /*m_axi_bus_agents[int'(PSHIRE_SPIO_M_AXI)].push_back(null);
      m_axi_bus_agents[int'(PCIE0_MSTR_AXI)].push_back(null);
      m_axi_bus_agents[int'(PCIE0_DBI_AXI)].push_back(null);
      m_axi_bus_agents[int'(PCIE0_SLV_AXI)].push_back(null);
      m_axi_bus_agents[int'(PCIE1_MSTR_AXI)].push_back(null);
      m_axi_bus_agents[int'(PCIE1_DBI_AXI)].push_back(null);
      m_axi_bus_agents[int'(PCIE1_SLV_AXI)].push_back(null);*/

      //
      // If the IOShire is an AXI stub, then pshire_stub will be 1.
      //
      m_pshire_stim = m_rtl_port.get_int_param(pk_pshire_stim);
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

            //
            // Bus Verification Components configuration
            //
            axi_comp_id    = -1;
            apb_comp_id    = -1;
            if (verif_comp.is_bus_vc() == 1'b1) begin
               if (verif_comp.get_agent_type() == DBG_NOC_BUS_AGENT) begin
                  verif_comp.set_agent_id(get_shire_id());
                  if (inst_name == "m_dbg_dn_evt") begin
                     dbgnoc_comp_id = int'(PS_DBG_DN_EVT);
                     verif_comp.set_port_id(dbgnoc_comp_id);
                     verif_comp.set_agent_id(dbgnoc_comp_id);
                     verif_comp.set_abstract_name($sformatf("DN_EVT(DBGNOC)"));
                     m_dbgnoc_bus_agents[dbgnoc_comp_id] = verif_comp;
                  end
                  else if (inst_name == "m_dbg_dn_msg") begin
                     dbgnoc_comp_id = int'(PS_DBG_DN_MSG);
                     verif_comp.set_port_id(dbgnoc_comp_id);
                     verif_comp.set_agent_id(dbgnoc_comp_id);
                     verif_comp.set_abstract_name($sformatf("DN_MSG(DBGNOC)"));
                     m_dbgnoc_bus_agents[dbgnoc_comp_id] = verif_comp;
                  end
               end
               else if (verif_comp.get_agent_type() == AXI_BUS_AGENT) begin
                  evl_rtl_port bus_agent_rtl_port;

                  bus_agent_rtl_port = verif_comp.get_rtl_port();

                  if (bus_agent_rtl_port != null) begin
                     layer_num = bus_agent_rtl_port.get_port_id();
                  end
                  else begin
                     layer_num = 0;
                  end

                  verif_comp.set_agent_id(get_shire_id());
                  //
                  // AXI Bus Agents for Upstream PSHIRE Stub and PShire NOC
                  //
                  if (inst_name == "m_ps_tol3_mesh_master_axi") begin
                     axi_comp_id = int'(PSHIRE_TOL3_AXI);
                     layer_offset = m_global_attrs.get_pshire_axi_port_index(axi_comp_id, layer_num, m_num_l3_ports);
                     verif_comp.set_port_id(layer_offset);
                     verif_comp.set_port_type(axi_comp_id);
                     verif_comp.set_agent_id(layer_offset);
                     if (m_num_l3_ports > 1) begin
                        verif_comp.set_abstract_name($sformatf("PSHIRE_TOL3(AXI%0d)", layer_num));
                     end
                     else begin
                        verif_comp.set_abstract_name($sformatf("PSHIRE_TOL3(AXI)"));
                     end
                     m_axi_bus_agents[axi_comp_id][layer_num] = verif_comp;
                  end
                  else if (inst_name == "m_ps_ps_slave_axi") begin
                     axi_comp_id = int'(PSHIRE_PS_AXI);
                     layer_offset = m_global_attrs.get_pshire_axi_port_index(axi_comp_id, 0, m_num_l3_ports);
                     verif_comp.set_port_id(layer_offset);
                     verif_comp.set_port_type(axi_comp_id);
                     verif_comp.set_agent_id(layer_offset);
                     verif_comp.set_abstract_name($sformatf("PSHIRE_PS(AXI)"));
                     m_axi_bus_agents[axi_comp_id][0] = verif_comp;
                  end
                  else if (inst_name == "m_ps_tosys_mesh_master_axi") begin
                     axi_comp_id = int'(PSHIRE_TOSYS_AXI);
                     layer_offset = m_global_attrs.get_pshire_axi_port_index(axi_comp_id, 0, m_num_l3_ports);
                     verif_comp.set_port_id(layer_offset);
                     verif_comp.set_port_type(axi_comp_id);
                     verif_comp.set_agent_id(layer_offset);
                     verif_comp.set_abstract_name($sformatf("PSHIRE_TOSYS(AXI)"));
                     m_axi_bus_agents[axi_comp_id][0] = verif_comp;
                  end
               end

               add_vc_child(verif_comp);


               //
               // Create stimulus agents as necessary.
               //
               if (m_dbgnoc_stim == 1) begin
                  if (verif_comp.get_agent_type() == DBG_NOC_BUS_AGENT) begin
                     if (dbgnoc_comp_id == PS_DBG_DN_EVT) begin
                        component = factory.create_component_by_name("evl_stim_agent_dbg_noc", "", $sformatf("ps_dn_evt_%0d", dbgnoc_comp_id), this);
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
                     else if (dbgnoc_comp_id == PS_DBG_DN_MSG) begin
                        component = factory.create_component_by_name("evl_stim_agent_dbg_noc", "", $sformatf("ps_dn_msg_%0d", dbgnoc_comp_id), this);
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
               end
               if (m_pshire_stim == 1) begin
                  if (axi_comp_id >= 0) begin
                     if (axi_comp_id == PSHIRE_TOL3_AXI) begin
                        verif_comp.set_active_master();
                        if (!m_axi_stim_agents.exists(axi_comp_id)) begin
                           component = factory.create_component_by_name("evl_l2tol3_master_agent_axi", "", $sformatf("ps_l2tol3_master"), this);
                        end
                        else begin
                           m_axi_stim_agents[axi_comp_id].set_rtl_port(m_axi_bus_agents[axi_comp_id][layer_num].get_rtl_port(), layer_num);
                           component = null;
                        end
                     end
                     else if (axi_comp_id == PSHIRE_PS_AXI) begin
                        verif_comp.set_active_slave();
                        if (!m_axi_stim_agents.exists(axi_comp_id)) begin
                           component = factory.create_component_by_name("evl_shire_slave_agent_axi", "", $sformatf("ps_ps_slave"), this);
                        end
                        else begin
                           component = null;
                        end
                     end
                     else if (axi_comp_id == PSHIRE_TOSYS_AXI) begin
                        verif_comp.set_active_master();
                        if (!m_axi_stim_agents.exists(axi_comp_id)) begin
                           component = factory.create_component_by_name("evl_pshire_tosys_master_agent_axi", "", $sformatf("ps_tosys_master"), this);
                        end
                        else begin
                           m_axi_stim_agents[axi_comp_id].set_rtl_port(m_axi_bus_agents[axi_comp_id][layer_num].get_rtl_port(), layer_num);
                           component = null;
                        end
                     end
                     else begin
                        component = null;
                     end
                     if (component != null) begin
                        $cast(verif_comp, component);
                        verif_comp.set_shire_id(get_shire_id());
                        verif_comp.set_port_id(axi_comp_id);
                        verif_comp.set_port_type(axi_comp_id);
                        verif_comp.set_agent_id(axi_comp_id);
                        case (axi_comp_id)
                           PSHIRE_TOL3_AXI: begin
                              verif_comp.set_abstract_name($sformatf("PSHIRE_TOL3_AXI(STIM)"));
                              verif_comp.set_rtl_port(m_axi_bus_agents[axi_comp_id][layer_num].get_rtl_port(), layer_num);
                              verif_comp.set_int_param(pk_num_l3_ports, m_num_l3_ports);
                           end
                           PSHIRE_PS_AXI: begin
                              verif_comp.set_abstract_name($sformatf("PSHIRE_PS_AXI(STIM)"));
                              verif_comp.set_rtl_port(m_axi_bus_agents[axi_comp_id][0].get_rtl_port(), layer_num);
                           end
                           PSHIRE_TOSYS_AXI: begin
                              verif_comp.set_abstract_name($sformatf("PSHIRE_TOSYS_AXI(STIM)"));
                              verif_comp.set_rtl_port(m_axi_bus_agents[axi_comp_id][0].get_rtl_port(), layer_num);
                           end
                        endcase
                        m_axi_stim_agents[axi_comp_id] = verif_comp;
                        add_vc_child(verif_comp);
                     end
                  end
               end
            end
            else begin
               if (verif_comp.get_agent_type() == PSHIRE_NOC_RTL_AGENT) begin
                  m_pshire_noc_agent = verif_comp;
                  `evl_log(UVM_DEBUG, VMOD_NOC_SB, get_abstract_name(), $sformatf("PSHIRE_NOC_RTL_AGENT found."))
                  verif_comp.set_int_param(pk_num_l3_ports, m_num_l3_ports);
                  verif_comp.set_shire_id(get_shire_id());
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
      foreach (m_axi_stim_agents[ii]) begin
         for (int jj = 0; jj < m_axi_bus_agents[ii].size(); jj++) begin
            m_axi_stim_agents[ii].connect_my_bus_ports(m_axi_bus_agents[ii][jj], "", jj);
         end
      end
      foreach (m_dbgnoc_bus_agents[ii]) begin
         if ((m_dbgnoc_bus_agents[ii] != null) && m_dbgnoc_stim_agents.exists(ii) && (m_dbgnoc_stim_agents[ii] != null)) begin
            m_dbgnoc_stim_agents[ii].connect_my_bus_ports(m_dbgnoc_bus_agents[ii]);
         end
      end
   endfunction : connect_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Function: start_of_simulation_phase
   //
   virtual function void start_of_simulation_phase(uvm_phase phase);
      super.start_of_simulation_phase(phase);
      if (m_pshire_stim == 1) begin
         foreach (m_axi_bus_agents[ii]) begin
            if (m_axi_stim_agents.exists(ii)) begin
               if (m_axi_stim_agents[ii].is_active_master()) begin
                  for (int jj = 0; jj < m_axi_bus_agents[ii].size(); jj++) begin
                     if (m_axi_bus_agents[ii][jj] != null) begin
                        m_axi_bus_agents[ii][jj].set_master_port_agent(m_axi_stim_agents[ii]);
                     end
                  end
               end
               if (m_axi_stim_agents[ii].is_active_slave() && (m_master_port_agent != null)) begin
                  for (int jj = 0; jj < m_axi_bus_agents[ii].size(); jj++) begin
                     if (m_axi_bus_agents[ii][jj] != null) begin
                        m_axi_bus_agents[ii][jj].set_slave_port_agent(m_axi_stim_agents[ii]);
                        m_axi_bus_agents[ii][jj].set_master_port_agent(m_master_port_agent);
                     end
                  end
               end
               else if (m_axi_stim_agents[ii].is_active_slave()) begin
                  for (int jj = 0; jj < m_axi_bus_agents[ii].size(); jj++) begin
                     if (m_axi_bus_agents[ii][jj] != null) begin
                        m_axi_bus_agents[ii][jj].set_slave_port_agent(m_axi_stim_agents[ii]);
                     end
                  end
               end
            end
         end
      end
      else if (m_pshire_noc_only == 1) begin
         foreach (m_axi_bus_agents[ii]) begin
            if (m_axi_stim_agents.exists(ii)) begin
               if (m_axi_stim_agents[ii].is_active_master()) begin
                  for (int jj = 0; jj < m_axi_bus_agents[ii].size(); jj++) begin
                     if (m_axi_bus_agents[ii][jj] != null) begin
                        m_axi_bus_agents[ii][jj].set_master_port_agent(m_axi_stim_agents[ii]);
                     end
                  end
               end
               else if (m_axi_stim_agents[ii].is_active_slave()) begin
                  for (int jj = 0; jj < m_axi_bus_agents[ii].size(); jj++) begin
                     if (m_axi_bus_agents[ii][jj] != null) begin
                        m_axi_bus_agents[ii][jj].set_slave_port_agent(m_axi_stim_agents[ii]);
                     end
                  end
               end
            end
         end
         if ((m_master_port_agent != null) && (m_axi_bus_agents.exists(int'(PSHIRE_PS_AXI)))) begin
            if (m_axi_bus_agents[int'(PSHIRE_PS_AXI)][0] != null) begin
               m_axi_bus_agents[int'(PSHIRE_PS_AXI)][0].set_master_port_agent(m_master_port_agent);
            end
         end
      end
      else begin
         if (m_axi_stim_agents.exists(int'(PSHIRE_PS_AXI)) && (m_axi_stim_agents[int'(PSHIRE_PS_AXI)] != null)) begin
            if (m_axi_bus_agents[int'(PSHIRE_PS_AXI)][0] != null) begin
               m_axi_bus_agents[int'(PSHIRE_PS_AXI)][0].set_master_port_agent(m_axi_stim_agents[int'(PSHIRE_PS_AXI)]);
            end
         end
         else if ((m_master_port_agent != null) && (m_axi_bus_agents.exists(int'(PSHIRE_PS_AXI)))) begin
            if (m_axi_bus_agents[int'(PSHIRE_PS_AXI)][0] != null) begin
               m_axi_bus_agents[int'(PSHIRE_PS_AXI)][0].set_master_port_agent(m_master_port_agent);
            end
         end
      end

      foreach (m_dbgnoc_bus_agents[ii]) begin
         if (m_dbgnoc_stim_agents.exists(ii) && (m_dbgnoc_stim_agents[ii] != null)) begin
            m_dbgnoc_bus_agents[ii].set_master_port_agent(m_dbgnoc_stim_agents[ii]);
         end
      end

   endfunction : start_of_simulation_phase


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
         if (!m_axi_bus_agents.exists(port_number)) begin
            return null;
         end
         if (port_name == "m_axi_req_port") begin
            if (m_axi_bus_agents[port_number].size() > subport_number) begin
               if (m_axi_bus_agents[port_number][subport_number] != null) begin
                  return m_axi_bus_agents[port_number][subport_number].get_req_desc_port("m_obs_req_port");
               end
            end
         end
         if (port_name == "m_axi_rsp_port") begin
            if (m_axi_bus_agents[port_number].size() > subport_number) begin
               if (m_axi_bus_agents[port_number][subport_number] != null) begin
                  return m_axi_bus_agents[port_number][subport_number].get_req_desc_port("m_obs_rsp_port");
               end
            end
         end
      end
      return null;
   endfunction : get_req_desc_port


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string name_in = "evl_pshire_env", input uvm_component parent_in = null);
      super.new(name_in, parent_in, PSHIRE_RTL_AGENT);
      set_abstract_name("PSHIRE");
      add_agent_alias(PSHIRE_AGENT);
   endfunction : new

endclass : evl_pshire_env

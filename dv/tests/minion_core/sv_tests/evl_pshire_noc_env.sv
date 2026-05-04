//--------------------------------------------------------------------------------------------------
//
// Class: evl_pshire_noc_env
//
class evl_pshire_noc_env extends evl_verif_comp;

   `uvm_component_utils(evl_pshire_noc_env)

   //-----------------------------------------------------------------------------------------------
   //
   // APB Bus Types
   //
   // The following enumerated types are used to identify APB bus agents in an associative array.
   // These bus agents can then be connected to master or slave stimulus agents as necessary during
   // the connect phase.
   //
   typedef enum int {
         APBSLV_APB,
         PLL0SLV_APB,
         ESRSLV_APB
      } pshire_apb_bus_t;

   //-----------------------------------------------------------------------------------------------
   //
   // Static Member Variables and Objects
   //
   `evl_param_key(pk_pshire_noc_only)
   `evl_param_key(pk_pshire_no_esr)
   `evl_param_key(pk_zebu_driver)


   //-----------------------------------------------------------------------------------------------
   //
   // Member Variables and Objects
   //
   evl_pshire_noc_rtl_port m_pshire_noc_rtl_port;
   evl_verif_comp_q        m_axi_bus_agents[int];
   evl_verif_comp          m_axi_stim_agents[int];
   evl_verif_comp          m_apb_bus_agents[evl_agent_id_t];
   evl_verif_comp          m_apb_stim_agents[evl_agent_id_t];

   int                 m_num_l3_ports;
   int                 m_pshire_noc_only;
   int                 m_pshire_no_esr;
   int                 m_zebu_driver;


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
      int              noc_is_stub;
      int              axi_comp_id;
      int              apb_comp_id;
      int              etlink_comp_id;
      int              layer_num;
      int              layer_offset;

      super.build_phase(phase);
      factory  = uvm_factory::get();
      shire_id = get_shire_id();
      if (m_rtl_port == null) begin
         `dut_error(get_abstract_name(), $sformatf("%0s: m_rtl_port is null, SAD!", get_abstract_name()))
         return;
      end
      $cast(m_pshire_noc_rtl_port, m_rtl_port);

      //
      // If the PShire is built with NoC only, then noc_is_stub will be 1.
      //
      m_pshire_noc_only = m_rtl_port.get_int_param(pk_pshire_noc_only, 0);
      m_pshire_no_esr   = m_rtl_port.get_int_param(pk_pshire_no_esr, 0);
      m_zebu_driver     = m_rtl_port.get_int_param(pk_zebu_driver, 0);

      //
      // Start by creating null pointers for all of the possible AXI bus agents.
      //
      m_axi_bus_agents[int'(PSHIRE_SPIO_M_AXI)].push_back(null);
      m_axi_bus_agents[int'(PCIE0_MSTR_AXI)].push_back(null);
      m_axi_bus_agents[int'(PCIE0_DBI_AXI)].push_back(null);
      m_axi_bus_agents[int'(PCIE0_SLV_AXI)].push_back(null);
      m_axi_bus_agents[int'(PCIE1_MSTR_AXI)].push_back(null);
      m_axi_bus_agents[int'(PCIE1_DBI_AXI)].push_back(null);
      m_axi_bus_agents[int'(PCIE1_SLV_AXI)].push_back(null);
      m_axi_bus_agents[int'(PSHIRE_ESR_AXI)].push_back(null);
      m_axi_bus_agents[int'(PSHIRE_DBG_AXI)].push_back(null);

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
            etlink_comp_id = -1;
            if (verif_comp.is_bus_vc() == 1'b1) begin
               if (verif_comp.get_agent_type() == APB_BUS_AGENT) begin
                  if (inst_name == "m_apbslv") begin
                     agent_id = int'(APBSLV_APB);
                     verif_comp.set_agent_id(agent_id);
                     verif_comp.set_port_id(agent_id);
                     verif_comp.set_abstract_name($sformatf("APBSLV_APB(APB)"));
                     m_apb_bus_agents[agent_id] = verif_comp;
                  end
                  else if (inst_name == "m_pshr_pll0_slave_apb") begin
                     agent_id = int'(PLL0SLV_APB);
                     verif_comp.set_agent_id(agent_id);
                     verif_comp.set_port_id(agent_id);
                     verif_comp.set_abstract_name($sformatf("PLL0SLV_APB(APB)"));
                     m_apb_bus_agents[agent_id] = verif_comp;
                  end
                  else if (inst_name == "m_pshr_esr_slave_apb") begin
                     agent_id = int'(ESRSLV_APB);
                     verif_comp.set_agent_id(agent_id);
                     verif_comp.set_port_id(agent_id);
                     verif_comp.set_abstract_name($sformatf("ESRSLV_APB(APB)"));
                     m_apb_bus_agents[agent_id] = verif_comp;
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
                  if (inst_name == "m_p0_spio_master_axi") begin
                     axi_comp_id = int'(PSHIRE_SPIO_M_AXI);
                     layer_offset = m_global_attrs.get_pshire_axi_port_index(axi_comp_id, 0, m_num_l3_ports);
                     verif_comp.set_port_id(layer_offset);
                     verif_comp.set_port_type(axi_comp_id);
                     verif_comp.set_agent_id(layer_offset);
                     verif_comp.set_abstract_name($sformatf("PSHIRE_SPIO_M_AXI(AXI)"));
                     m_axi_bus_agents[axi_comp_id][0] = verif_comp;
                  end
                  else if (inst_name == "m_pcie0_mstr_master_axi") begin
                     axi_comp_id = int'(PCIE0_MSTR_AXI);
                     layer_offset = m_global_attrs.get_pshire_axi_port_index(axi_comp_id, 0, m_num_l3_ports);
                     verif_comp.set_port_id(layer_offset);
                     verif_comp.set_port_type(axi_comp_id);
                     verif_comp.set_agent_id(layer_offset);
                     verif_comp.set_abstract_name($sformatf("PCIE0_MSTR_AXI(AXI)"));
                     m_axi_bus_agents[axi_comp_id][0] = verif_comp;
                  end
                  else if (inst_name == "m_pcie0_dbi_slave_axi") begin
                     axi_comp_id = int'(PCIE0_DBI_AXI);
                     layer_offset = m_global_attrs.get_pshire_axi_port_index(axi_comp_id, 0, m_num_l3_ports);
                     verif_comp.set_port_id(layer_offset);
                     verif_comp.set_port_type(axi_comp_id);
                     verif_comp.set_agent_id(layer_offset);
                     verif_comp.set_abstract_name($sformatf("PCIE0_DBI_AXI(AXI)"));
                     m_axi_bus_agents[axi_comp_id][0] = verif_comp;
                  end
                  else if (inst_name == "m_pcie0_slv_slave_axi") begin
                     axi_comp_id = int'(PCIE0_SLV_AXI);
                     layer_offset = m_global_attrs.get_pshire_axi_port_index(axi_comp_id, 0, m_num_l3_ports);
                     verif_comp.set_port_id(layer_offset);
                     verif_comp.set_port_type(axi_comp_id);
                     verif_comp.set_agent_id(layer_offset);
                     verif_comp.set_abstract_name($sformatf("PCIE0_SLV_AXI(AXI)"));
                     m_axi_bus_agents[axi_comp_id][0] = verif_comp;
                  end
                  else if (inst_name == "m_pcie1_mstr_master_axi") begin
                     axi_comp_id = int'(PCIE1_MSTR_AXI);
                     layer_offset = m_global_attrs.get_pshire_axi_port_index(axi_comp_id, 0, m_num_l3_ports);
                     verif_comp.set_port_id(layer_offset);
                     verif_comp.set_port_type(axi_comp_id);
                     verif_comp.set_agent_id(layer_offset);
                     verif_comp.set_abstract_name($sformatf("PCIE1_MSTR_AXI(AXI)"));
                     m_axi_bus_agents[axi_comp_id][0] = verif_comp;
                  end
                  else if (inst_name == "m_pcie1_dbi_slave_axi") begin
                     axi_comp_id = int'(PCIE1_DBI_AXI);
                     layer_offset = m_global_attrs.get_pshire_axi_port_index(axi_comp_id, 0, m_num_l3_ports);
                     verif_comp.set_port_id(layer_offset);
                     verif_comp.set_port_type(axi_comp_id);
                     verif_comp.set_agent_id(layer_offset);
                     verif_comp.set_abstract_name($sformatf("PCIE1_DBI_AXI(AXI)"));
                     m_axi_bus_agents[axi_comp_id][0] = verif_comp;
                  end
                  else if (inst_name == "m_pcie1_slv_slave_axi") begin
                     axi_comp_id = int'(PCIE1_SLV_AXI);
                     layer_offset = m_global_attrs.get_pshire_axi_port_index(axi_comp_id, 0, m_num_l3_ports);
                     verif_comp.set_port_id(layer_offset);
                     verif_comp.set_port_type(axi_comp_id);
                     verif_comp.set_agent_id(layer_offset);
                     verif_comp.set_abstract_name($sformatf("PCIE1_SLV_AXI(AXI)"));
                     m_axi_bus_agents[axi_comp_id][0] = verif_comp;
                  end
                  else if (inst_name == "m_pshr_esr_pshr_noc_master_axi") begin
                     axi_comp_id = int'(PSHIRE_ESR_AXI);
                     layer_offset = m_global_attrs.get_pshire_axi_port_index(axi_comp_id, 0, m_num_l3_ports);
                     verif_comp.set_port_id(layer_offset);
                     verif_comp.set_port_type(axi_comp_id);
                     verif_comp.set_agent_id(layer_offset);
                     verif_comp.set_abstract_name($sformatf("PSHIRE_ESR_AXI(AXI)"));
                     m_axi_bus_agents[axi_comp_id][0] = verif_comp;
                  end
                  else if (inst_name == "m_pshr_dbg_dma_am_master_axi") begin
                     axi_comp_id = int'(PSHIRE_DBG_AXI);
                     layer_offset = m_global_attrs.get_pshire_axi_port_index(axi_comp_id, 0, m_num_l3_ports);
                     verif_comp.set_port_id(layer_offset);
                     verif_comp.set_port_type(axi_comp_id);
                     verif_comp.set_agent_id(layer_offset);
                     verif_comp.set_abstract_name($sformatf("PSHIRE_DBG_AXI(AXI)"));
                     m_axi_bus_agents[axi_comp_id][0] = verif_comp;
                  end
               end

               add_vc_child(verif_comp);


               //
               // Create stimulus agents as necessary.
               //
               if ((m_pshire_noc_only == 1) && (m_pshire_no_esr == 1)) begin
                  if (verif_comp.get_agent_type() == APB_BUS_AGENT) begin
                     if (agent_id == APBSLV_APB) begin
                        component = factory.create_component_by_name("evl_reg_slave_agent_apb", "", $sformatf("ps_apb_slv"), this);
                        if (component != null) begin
                           verif_comp.set_active_slave();
                           $cast(verif_comp, component);
                           m_apb_stim_agents[agent_id] = verif_comp;
                           verif_comp.set_agent_id(agent_id);
                           verif_comp.set_abstract_name($sformatf("APB_SLV"));
                           verif_comp.set_shire_id(get_shire_id());
                           verif_comp.set_rtl_port(m_apb_bus_agents[agent_id].get_rtl_port());
                           add_vc_child(verif_comp);
                        end
                     end
                     else if (agent_id == ESRSLV_APB) begin
                        component = factory.create_component_by_name("evl_reg_slave_agent_apb", "", $sformatf("ps_apb_esr_slv"), this);
                        if (component != null) begin
                           verif_comp.set_active_slave();
                           $cast(verif_comp, component);
                           m_apb_stim_agents[agent_id] = verif_comp;
                           verif_comp.set_agent_id(agent_id);
                           verif_comp.set_abstract_name($sformatf("APB_ESR_SLV"));
                           verif_comp.set_shire_id(get_shire_id());
                           verif_comp.set_rtl_port(m_apb_bus_agents[agent_id].get_rtl_port());
                           add_vc_child(verif_comp);
                        end
                     end
                  end
               end
               if (m_pshire_noc_only == 1) begin
                  if (verif_comp.get_agent_type() == AXI_BUS_AGENT) begin
                     if (axi_comp_id >= 0) begin
                        if ((axi_comp_id == PCIE0_MSTR_AXI) && (m_zebu_driver == 0)) begin
                           verif_comp.set_active_master();
                           if (!m_axi_stim_agents.exists(axi_comp_id)) begin
                              component = factory.create_component_by_name("evl_pshire_pcie_master_agent_axi", "", $sformatf("ps_pcie0_master"), this);
                           end
                           else begin
                              component = null;
                           end
                        end
                        else if (axi_comp_id == PCIE0_DBI_AXI) begin
                           verif_comp.set_active_slave();
                           if (!m_axi_stim_agents.exists(axi_comp_id)) begin
                              component = factory.create_component_by_name("evl_shire_slave_agent_axi", "", $sformatf("ps_pcie0_dbi_slave"), this);
                           end
                           else begin
                              component = null;
                           end
                        end
                        else if (axi_comp_id == PCIE0_SLV_AXI) begin
                           verif_comp.set_active_slave();
                           if (!m_axi_stim_agents.exists(axi_comp_id)) begin
                              component = factory.create_component_by_name("evl_shire_slave_agent_axi", "", $sformatf("ps_pcie0_slave"), this);
                           end
                           else begin
                              component = null;
                           end
                        end
                        else if (axi_comp_id == PCIE1_MSTR_AXI) begin
                           verif_comp.set_active_master();
                           if (!m_axi_stim_agents.exists(axi_comp_id)) begin
                              component = factory.create_component_by_name("evl_pshire_pcie_master_agent_axi", "", $sformatf("ps_pcie1_master"), this);
                           end
                           else begin
                              component = null;
                           end
                        end
                        else if (axi_comp_id == PCIE1_DBI_AXI) begin
                           verif_comp.set_active_slave();
                           if (!m_axi_stim_agents.exists(axi_comp_id)) begin
                              component = factory.create_component_by_name("evl_shire_slave_agent_axi", "", $sformatf("ps_pcie1_dbi_slave"), this);
                           end
                           else begin
                              component = null;
                           end
                        end
                        else if (axi_comp_id == PCIE1_SLV_AXI) begin
                           verif_comp.set_active_slave();
                           if (!m_axi_stim_agents.exists(axi_comp_id)) begin
                              component = factory.create_component_by_name("evl_shire_slave_agent_axi", "", $sformatf("ps_pcie1_slave"), this);
                           end
                           else begin
                              component = null;
                           end
                        end
                        else if (axi_comp_id == PSHIRE_ESR_AXI) begin
                           if (m_pshire_no_esr == 1) begin
                              verif_comp.set_active_master();
                              if (!m_axi_stim_agents.exists(axi_comp_id)) begin
                                 component = factory.create_component_by_name("evl_pshire_esr_master_agent_axi", "", $sformatf("ps_pshire_esr_master"), this);
                              end
                              else begin
                                 component = null;
                              end
                           end
                        end
                        /*else if (axi_comp_id == PSHIRE_DBG_AXI) begin
                           verif_comp.set_active_master();
                           if (!m_axi_stim_agents.exists(axi_comp_id)) begin
                              component = factory.create_component_by_name("evl_pshire_dbg_master_agent_axi", "", $sformatf("ps_pshire_dbg_master"), this);
                           end
                           else begin
                              component = null;
                           end
                        end*/ // For now PShire NoC build always contains pshr_dbg block.
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
                              PCIE0_MSTR_AXI: begin
                                 verif_comp.set_abstract_name($sformatf("PCIE0_MSTR_AXI(STIM)"));
                                 verif_comp.set_rtl_port(m_axi_bus_agents[axi_comp_id][layer_num].get_rtl_port(), layer_num);
                              end
                              PCIE0_DBI_AXI: begin
                                 verif_comp.set_abstract_name($sformatf("PCIE0_DBI_AXI(STIM)"));
                                 verif_comp.set_rtl_port(m_axi_bus_agents[axi_comp_id][0].get_rtl_port(), layer_num);
                              end
                              PCIE0_SLV_AXI: begin
                                 verif_comp.set_abstract_name($sformatf("PCIE0_SLV_AXI(STIM)"));
                                 verif_comp.set_rtl_port(m_axi_bus_agents[axi_comp_id][0].get_rtl_port(), layer_num);
                              end
                              PCIE1_MSTR_AXI: begin
                                 verif_comp.set_abstract_name($sformatf("PCIE1_MSTR_AXI(STIM)"));
                                 verif_comp.set_rtl_port(m_axi_bus_agents[axi_comp_id][layer_num].get_rtl_port(), layer_num);
                              end
                              PCIE1_DBI_AXI: begin
                                 verif_comp.set_abstract_name($sformatf("PCIE1_DBI_AXI(STIM)"));
                                 verif_comp.set_rtl_port(m_axi_bus_agents[axi_comp_id][0].get_rtl_port(), layer_num);
                              end
                              PCIE1_SLV_AXI: begin
                                 verif_comp.set_abstract_name($sformatf("PCIE1_SLV_AXI(STIM)"));
                                 verif_comp.set_rtl_port(m_axi_bus_agents[axi_comp_id][0].get_rtl_port(), layer_num);
                              end
                              PSHIRE_ESR_AXI: begin
                                 verif_comp.set_abstract_name($sformatf("PSHIRE_ESR_AXI(STIM)"));
                                 verif_comp.set_rtl_port(m_axi_bus_agents[axi_comp_id][0].get_rtl_port(), layer_num);
                              end
                              PSHIRE_DBG_AXI: begin
                                 verif_comp.set_abstract_name($sformatf("PSHIRE_DBG_AXI(STIM)"));
                                 verif_comp.set_rtl_port(m_axi_bus_agents[axi_comp_id][0].get_rtl_port(), layer_num);
                              end
                           endcase
                           m_axi_stim_agents[axi_comp_id] = verif_comp;
                           add_vc_child(verif_comp);
                        end
                     end
                  end
               end
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
      foreach (m_apb_stim_agents[ii]) begin
         m_apb_stim_agents[ii].connect_my_bus_ports(m_apb_bus_agents[ii]);
      end
   endfunction : connect_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Function: start_of_simulation_phase
   //
   virtual function void start_of_simulation_phase(uvm_phase phase);
      super.start_of_simulation_phase(phase);
      if ((m_pshire_noc_only == 1)) begin
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
      /*else begin
         if (m_axi_stim_agents.exists(int'(PSHIRE_PS_AXI)) && (m_axi_stim_agents[int'(IO_PS_AXI)] != null)) begin
            if (m_axi_bus_agents[int'(PSHIRE_PS_AXI)][0] != null) begin
               m_axi_bus_agents[int'(PSHIRE_PS_AXI)][0].set_master_port_agent(m_axi_stim_agents[int'(IO_PS_AXI)]);
            end
         end
         else if ((m_master_port_agent != null) && (m_axi_bus_agents.exists(int'(IO_PS_AXI)))) begin
            if (m_axi_bus_agents[int'(PSHIRE_PS_AXI)][0] != null) begin
               m_axi_bus_agents[int'(PSHIRE_PS_AXI)][0].set_master_port_agent(m_master_port_agent);
            end
         end
      end*/
   endfunction : start_of_simulation_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_req_desc_port
   //
   virtual function uvm_analysis_port#(evl_req_desc) get_req_desc_port(input string port_name      = "unknown",
                                                                       input int    port_number    = 0,
                                                                       input int    subport_number = 0);

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
      return null;
   endfunction : get_req_desc_port


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string name_in = "evl_pshire_noc_env", input uvm_component parent_in = null);
      super.new(name_in, parent_in, PSHIRE_NOC_RTL_AGENT);
      set_abstract_name("PSHIRE_NOC");
      add_agent_alias(PSHIRE_NOC_AGENT);
   endfunction : new

endclass : evl_pshire_noc_env

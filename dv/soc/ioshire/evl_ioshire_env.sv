//--------------------------------------------------------------------------------------------------
//
// Class: evl_ioshire_env
//
class evl_ioshire_env extends evl_verif_comp;

   `uvm_component_utils(evl_ioshire_env)


   //-----------------------------------------------------------------------------------------------
   //
   // AXI Bus Types
   //
   // Refer to dv/common/base/evl_base_pkg.sv for the AXI bus types.
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
         SPIO_NOC_APB0,
         SPIO_NOC_APB1,
         SPIO_NONE_APB
      } ioshire_apb_bus_t;


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
      } ioshire_etlink_bus_t;


   //-----------------------------------------------------------------------------------------------
   //
   // Debug NoC Bus Types
   //
   // The following enumerated types are used to identify Debug NoC bus agents in an associative array.
   // These bus agents can then be connected to master or slave stimulus agents as necessary during
   // the connect phase.
   //
   //typedef enum int {
   //      IO_DBG_RT_EVT,
   //      IO_DBG_RT_MSG,
   //      IO_DBG_LF_EVT,
   //      IO_DBG_LF_MSG
   //   } ioshire_dbg_noc_bus_t;

   //-----------------------------------------------------------------------------------------------
   //
   // Static Member Variables and Objects
   //
   `evl_param_key(pk_allow_loopback)
   `evl_param_key(pk_sp_is_stub)
   `evl_param_key(pk_noc_is_stub)
   `evl_param_key(pk_ioshire_stim)
   `evl_param_key(pk_num_l3_ports)
   `evl_param_key(pk_num_l3_master_ports)
   `evl_param_key(pk_num_l3_slave_ports)
   `evl_param_key(pk_io_noc_only)
   `evl_param_key(pk_io_no_pu)


   //-----------------------------------------------------------------------------------------------
   //
   // Member Variables and Objects
   //
   evl_ioshire_rtl_port m_ioshire_rtl_port;
   evl_verif_comp_q     m_axi_bus_agents[int];
   evl_verif_comp       m_apb_bus_agents[int];
   evl_verif_comp       m_etlink_bus_agents[int];
   evl_verif_comp       m_dbgnoc_bus_agents[int];
   evl_verif_comp       m_axi_stim_agents[int];
   evl_verif_comp       m_apb_stim_agents[int];
   evl_verif_comp       m_etlink_stim_agents[int];
   evl_verif_comp       m_dbgnoc_stim_agents[int];
   evl_verif_comp       m_shire_channel_agent;
//   evl_verif_comp       m_sp_agent;

`ifdef L1SB_EVL_MINION_ENV
   evl_verif_comp       m_core_agent;   // only one instance in ioshire;
`else    // when using new evl_standalone_minion_env
   evl_verif_comp       m_standalone_minion_agent;
`endif

   int                  m_ioshire_stim;
   int                  m_num_l3_ports;
   int                  m_num_l3_master_ports;
   int                  m_num_l3_slave_ports;
   int                  m_io_noc_only;
   int                  m_io_no_pu;
   int                  m_noc_is_stub;


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
      int              axi_comp_id;
      int              apb_comp_id;
      int              etlink_comp_id;
      int              dbgnoc_comp_id;
      int              layer_num;
      int              rtl_port_num;
      int              layer_offset;
      evl_rtl_port     bus_rtl_port;

      super.build_phase(phase);
      factory  = uvm_factory::get();
      shire_id = get_shire_id();
      if (m_rtl_port == null) begin
         `dut_error(get_abstract_name(), $sformatf("%0s: m_rtl_port is null, SAD!", get_abstract_name()))
         return;
      end
      $cast(m_ioshire_rtl_port, m_rtl_port);

      //
      // If the service processor is a stub, then sp_is_stub will be 1.
      //
      sp_is_stub  = m_rtl_port.get_int_param(pk_sp_is_stub, 0);

      //
      // If the NoC is a stub, then noc_is_stub will be 1.
      //
//    m_noc_is_stub = get_int_param(pk_noc_stub, 0);
      m_noc_is_stub = m_rtl_port.get_int_param(pk_noc_is_stub, 0);

      m_num_l3_ports        = m_rtl_port.get_int_param(pk_num_l3_ports, 1);
      m_num_l3_master_ports = m_rtl_port.get_int_param(pk_num_l3_master_ports, 1);
      m_num_l3_slave_ports  = m_rtl_port.get_int_param(pk_num_l3_slave_ports, 1);

      //
      // Start by creating null pointers for all of the possible AXI bus agents.
      //
      for (int ii = 0; ii < m_num_l3_ports; ii++) begin
         m_axi_bus_agents[int'(IO_TOL3_AXI)].push_back(null);
      end
      for (int ii = 0; ii < m_num_l3_master_ports; ii++) begin
         m_axi_bus_agents[int'(MAXION_TOL3_AXI)].push_back(null);
      end
      for (int ii = 0; ii < m_num_l3_slave_ports; ii++) begin
         m_axi_bus_agents[int'(MAXION_L3_SLV_AXI)].push_back(null);
      end
      m_axi_bus_agents[int'(SPIO_ROM_AXI)].push_back(null);
      m_axi_bus_agents[int'(SPIO_SCRAM_AXI)].push_back(null);
      m_axi_bus_agents[int'(SPIO_NOC_PU_NOC_AXI)].push_back(null);
      m_axi_bus_agents[int'(SPIO_E2A_AXI)].push_back(null);
      m_axi_bus_agents[int'(SPIO_DBG_COMM_AXI)].push_back(null);
      m_axi_bus_agents[int'(SPIO_AM_AXI)].push_back(null);
      m_axi_bus_agents[int'(SPIO_DMA_AXI)].push_back(null);
      m_axi_bus_agents[int'(SPIO_VAULT_AXI)].push_back(null);
      m_axi_bus_agents[int'(SPIO_PCIE_AXI)].push_back(null);
      m_axi_bus_agents[int'(IO_PM_AXI)].push_back(null);
      m_axi_bus_agents[int'(IO_PS_AXI)].push_back(null);
      m_axi_bus_agents[int'(IO_TOSYS_AXI)].push_back(null);
      m_axi_bus_agents[int'(IO_MAIN_NOC_RBM)].push_back(null);
      m_axi_bus_agents[int'(IO_PU_NOC_RBM)].push_back(null);
      m_axi_bus_agents[int'(IO_PCIE_RBM)].push_back(null);
      m_axi_bus_agents[int'(IO_DBG_NOC_RT_EVT)].push_back(null);
      m_axi_bus_agents[int'(IO_DBG_NOC_RT_MSG)].push_back(null);
      m_axi_bus_agents[int'(IO_DBG_NOC_LF_EVT)].push_back(null);
      m_axi_bus_agents[int'(IO_DBG_NOC_LF_MSG)].push_back(null);
      m_axi_bus_agents[int'(MAXION_UCTOL3_AXI)].push_back(null);
      m_axi_bus_agents[int'(MAXION_TOSYS_AXI)].push_back(null);
      m_axi_bus_agents[int'(MAXION_UCTOSYS_AXI)].push_back(null);
      m_axi_bus_agents[int'(MAXION_SYS_SLV_AXI)].push_back(null);
      m_axi_bus_agents[int'(PU_NOC_SRAM_AXI)].push_back(null);
      m_axi_bus_agents[int'(PU_NOC_DMA_AXI)].push_back(null);
      m_axi_bus_agents[int'(PU_NOC_DMA_AM_AXI)].push_back(null);
      m_axi_bus_agents[int'(PU_NOC_EMMC_AXI)].push_back(null);
      m_axi_bus_agents[int'(PU_NOC_USB0_AXI)].push_back(null);
      m_axi_bus_agents[int'(PU_NOC_USB1_AXI)].push_back(null);
      m_axi_bus_agents[int'(PU_NOC_STATIC_AXI)].push_back(null);

      //
      // If the IOShire is an AXI stimulus stub, then ioshire_stim will be 1.
      //
      m_ioshire_stim = m_rtl_port.get_int_param(pk_ioshire_stim);
      m_io_noc_only  = m_rtl_port.get_int_param(pk_io_noc_only);
      m_io_no_pu     = m_rtl_port.get_int_param(pk_io_no_pu);
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

         if (instance_list[ii].get_agent_type() != SHIRE_CHANNEL_RTL_AGENT) begin
            component = factory.create_component_by_name(instance_list[ii].get_sv_comp(), "", inst_name, parent_comp);
         end
         else begin
            component = null;
         end

         if (component != null) begin
            evl_agent_id_t agent_id;

            $cast(verif_comp, component);
            agent_id = instance_list[ii].get_agent_pid();
            verif_comp.set_shire_id(get_shire_id());

            `evl_log(UVM_DEBUG, VMOD_CONFIG, get_abstract_name(), $sformatf("(CYE DEBUG: evl_ioshire_env) foreach(instance_list[]) loop: ii=%0d inst_name=%s: (after create component) after cast to verif_comp: is_bus_vc()=%0b get_agent_type()=%s", ii, inst_name, verif_comp.is_bus_vc(), verif_comp.get_agent_type() ))

            //
            // Bus Verification Components configuration
            //
            axi_comp_id    = -1;
            apb_comp_id    = -1;
            etlink_comp_id = -1;
            dbgnoc_comp_id = -1;
            rtl_port_num   = 0;
            if (verif_comp.is_bus_vc() == 1'b1) begin // ------------------------------------------{
               bus_rtl_port = null;
               if (verif_comp.get_agent_type() == ETLINK_BUS_AGENT) begin
                  verif_comp.set_agent_id(get_shire_id());
                  if (instance_list[ii].get_inst_name() == "m_etlink_e2a_agent") begin
                     etlink_comp_id = int'(SPIO_E2A_ETLINK);
                     verif_comp.set_port_id(etlink_comp_id);
                     verif_comp.set_agent_id(etlink_comp_id);
                     verif_comp.set_abstract_name($sformatf("E2A(ETLINK)"));
                     m_etlink_bus_agents[etlink_comp_id] = verif_comp;
                     `evl_log(UVM_DEBUG, VMOD_CONFIG, get_abstract_name(), $sformatf("(CYE DEBUG: evl_ioshire_env) ETLINK_BUS_AGENT:m_etlink_e2a_agent is found and assigned to m_etlink_bus_agents[]"))  
                  end
               end
               else if (verif_comp.get_agent_type() == DBG_NOC_BUS_AGENT) begin
                  verif_comp.set_agent_id(get_shire_id());
                  if (inst_name == "m_dbg_rt_evt") begin
                     dbgnoc_comp_id = int'(IO_DBG_RT_EVT);
                     verif_comp.set_port_id(dbgnoc_comp_id);
                     verif_comp.set_agent_id(dbgnoc_comp_id);
                     verif_comp.set_abstract_name($sformatf("RT_EVT(DBGNOC)"));
                     m_dbgnoc_bus_agents[dbgnoc_comp_id] = verif_comp;
                  end
                  else if (inst_name == "m_dbg_rt_msg") begin
                     dbgnoc_comp_id = int'(IO_DBG_RT_MSG);
                     verif_comp.set_port_id(dbgnoc_comp_id);
                     verif_comp.set_agent_id(dbgnoc_comp_id);
                     verif_comp.set_abstract_name($sformatf("RT_MSG(DBGNOC)"));
                     m_dbgnoc_bus_agents[dbgnoc_comp_id] = verif_comp;
                  end
                  else if (inst_name == "m_dbg_lf_evt") begin
                     dbgnoc_comp_id = int'(IO_DBG_LF_EVT);
                     verif_comp.set_port_id(dbgnoc_comp_id);
                     verif_comp.set_agent_id(dbgnoc_comp_id);
                     verif_comp.set_abstract_name($sformatf("LF_EVT(DBGNOC)"));
                     m_dbgnoc_bus_agents[dbgnoc_comp_id] = verif_comp;
                  end
                  else if (inst_name == "m_dbg_lf_msg") begin
                     dbgnoc_comp_id = int'(IO_DBG_LF_MSG);
                     verif_comp.set_port_id(dbgnoc_comp_id);
                     verif_comp.set_agent_id(dbgnoc_comp_id);
                     verif_comp.set_abstract_name($sformatf("LF_MSG(DBGNOC)"));
                     m_dbgnoc_bus_agents[dbgnoc_comp_id] = verif_comp;
                  end
               end
               else if (verif_comp.get_agent_type() == AXI_BUS_AGENT) begin
                  bus_rtl_port = verif_comp.get_rtl_port();

                  if (bus_rtl_port != null) begin
                     layer_num = bus_rtl_port.get_port_id();
                  end
                  else begin
                     layer_num = 0;
                  end

                  verif_comp.set_agent_id(get_shire_id());
                  if (inst_name == "m_spio_rom_axi") begin
                     axi_comp_id = int'(SPIO_ROM_AXI);
                     layer_offset = m_global_attrs.get_ioshire_axi_port_index(axi_comp_id, 0, m_num_l3_ports, m_num_l3_master_ports, m_num_l3_slave_ports);
                     verif_comp.set_port_id(layer_offset);
                     verif_comp.set_port_type(axi_comp_id);
                     verif_comp.set_agent_id(layer_offset);
                     verif_comp.set_abstract_name($sformatf("ROM(AXI)"));
                     m_axi_bus_agents[axi_comp_id][0] = verif_comp;
                  end
                  else if (inst_name == "m_spio_scram_axi") begin
                     axi_comp_id = int'(SPIO_SCRAM_AXI);
                     layer_offset = m_global_attrs.get_ioshire_axi_port_index(axi_comp_id, 0, m_num_l3_ports, m_num_l3_master_ports, m_num_l3_slave_ports);
                     verif_comp.set_port_id(layer_offset);
                     verif_comp.set_port_type(axi_comp_id);
                     verif_comp.set_agent_id(layer_offset);
                     verif_comp.set_abstract_name($sformatf("SCRAM(AXI)"));
                     m_axi_bus_agents[axi_comp_id][0] = verif_comp;
                  end
                  else if (inst_name == "m_spio_noc_pu_noc_axi") begin
                     axi_comp_id = int'(SPIO_NOC_PU_NOC_AXI);
                     layer_offset = m_global_attrs.get_ioshire_axi_port_index(axi_comp_id, 0, m_num_l3_ports, m_num_l3_master_ports, m_num_l3_slave_ports);
                     verif_comp.set_port_id(layer_offset);
                     verif_comp.set_port_type(axi_comp_id);
                     verif_comp.set_agent_id(layer_offset);
                     verif_comp.set_abstract_name($sformatf("SPIO_NOC_PU_NOC(AXI)"));
                     m_axi_bus_agents[axi_comp_id][0] = verif_comp;
                  end
                  else if (inst_name == "m_spio_dma_spio_noc_axi") begin
                     axi_comp_id = int'(SPIO_DMA_AXI);
                     layer_offset = m_global_attrs.get_ioshire_axi_port_index(axi_comp_id, 0, m_num_l3_ports, m_num_l3_master_ports, m_num_l3_slave_ports);
                     verif_comp.set_port_id(layer_offset);
                     verif_comp.set_port_type(axi_comp_id);
                     verif_comp.set_agent_id(layer_offset);
                     verif_comp.set_abstract_name($sformatf("SPIO_DMA(AXI)"));
                     m_axi_bus_agents[axi_comp_id][0] = verif_comp;
                  end
                  else if (inst_name == "m_spio_am_spio_noc_axi") begin
                     axi_comp_id = int'(SPIO_AM_AXI);
                     layer_offset = m_global_attrs.get_ioshire_axi_port_index(axi_comp_id, 0, m_num_l3_ports, m_num_l3_master_ports, m_num_l3_slave_ports);
                     verif_comp.set_port_id(layer_offset);
                     verif_comp.set_port_type(axi_comp_id);
                     verif_comp.set_agent_id(layer_offset);
                     verif_comp.set_abstract_name($sformatf("SPIO_AM(AXI)"));
                     m_axi_bus_agents[axi_comp_id][0] = verif_comp;
                  end
                  else if (inst_name == "m_spio_noc_dbg_comm_axi") begin
                     axi_comp_id = int'(SPIO_DBG_COMM_AXI);
                     layer_offset = m_global_attrs.get_ioshire_axi_port_index(axi_comp_id, 0, m_num_l3_ports, m_num_l3_master_ports, m_num_l3_slave_ports);
                     verif_comp.set_port_id(layer_offset);
                     verif_comp.set_port_type(axi_comp_id);
                     verif_comp.set_agent_id(layer_offset);
                     verif_comp.set_abstract_name($sformatf("SPIO_DBG_COMM(AXI)"));
                     m_axi_bus_agents[axi_comp_id][0] = verif_comp;
                  end
                  else if (inst_name == "m_spio_sp_spio_noc_axi") begin
                     axi_comp_id = int'(SPIO_E2A_AXI);
                     layer_offset = m_global_attrs.get_ioshire_axi_port_index(axi_comp_id, 0, m_num_l3_ports, m_num_l3_master_ports, m_num_l3_slave_ports);
                     verif_comp.set_port_id(layer_offset);
                     verif_comp.set_port_type(axi_comp_id);
                     verif_comp.set_subport_id(axi_comp_id);
                     verif_comp.set_agent_id(layer_offset);
                     verif_comp.set_abstract_name($sformatf("E2A(AXI)"));
                     m_axi_bus_agents[axi_comp_id][0] = verif_comp;
                  end
                  else if (inst_name == "m_spio_vault_spio_noc_axi") begin
                     axi_comp_id = int'(SPIO_VAULT_AXI);
                     layer_offset = m_global_attrs.get_ioshire_axi_port_index(axi_comp_id, 0, m_num_l3_ports, m_num_l3_master_ports, m_num_l3_slave_ports);
                     verif_comp.set_port_id(layer_offset);
                     verif_comp.set_port_type(axi_comp_id);
                     verif_comp.set_subport_id(axi_comp_id);
                     verif_comp.set_agent_id(layer_offset);
                     verif_comp.set_abstract_name($sformatf("SPIO_VAULT(AXI)"));
                     m_axi_bus_agents[axi_comp_id][0] = verif_comp;
                  end
                  else if (inst_name == "m_spio_p10_pcie_s_axi") begin
                     axi_comp_id = int'(SPIO_PCIE_AXI);
                     layer_offset = m_global_attrs.get_ioshire_axi_port_index(axi_comp_id, 0, m_num_l3_ports, m_num_l3_master_ports, m_num_l3_slave_ports);
                     verif_comp.set_port_id(layer_offset);
                     verif_comp.set_port_type(axi_comp_id);
                     verif_comp.set_subport_id(axi_comp_id);
                     verif_comp.set_agent_id(layer_offset);
                     verif_comp.set_abstract_name($sformatf("SPIO_PCIE(AXI)"));
                     m_axi_bus_agents[axi_comp_id][0] = verif_comp;
                  end
                  else if (inst_name == "m_maxion_to_l3_master_axi") begin
                     axi_comp_id = int'(MAXION_TOL3_AXI);
                     layer_offset = m_global_attrs.get_ioshire_axi_port_index(axi_comp_id, layer_num, m_num_l3_ports, m_num_l3_master_ports, m_num_l3_slave_ports);
                     verif_comp.set_port_id(layer_offset);
                     verif_comp.set_port_type(axi_comp_id);
                     verif_comp.set_subport_id(layer_num);
                     verif_comp.set_agent_id(layer_offset);
                     if (m_num_l3_master_ports > 1) begin
                        verif_comp.set_abstract_name($sformatf("IO_MAXION_TOL3(AXI%0d)", layer_num));
                     end
                     else begin
                        verif_comp.set_abstract_name($sformatf("IO_MAXION_TOL3(AXI)"));
                     end
                     m_axi_bus_agents[axi_comp_id][layer_num] = verif_comp;
                  end
                  else if (inst_name == "m_maxion_l3_slave_axi") begin
                     axi_comp_id = int'(MAXION_L3_SLV_AXI);
                     layer_offset = m_global_attrs.get_ioshire_axi_port_index(axi_comp_id, layer_num, m_num_l3_ports, m_num_l3_master_ports, m_num_l3_slave_ports);
                     verif_comp.set_port_id(layer_offset);
                     verif_comp.set_port_type(axi_comp_id);
                     verif_comp.set_subport_id(layer_num);
                     verif_comp.set_agent_id(layer_offset);
                     if (m_num_l3_slave_ports > 1) begin
                        verif_comp.set_abstract_name($sformatf("IO_MAXION_L3_SLV(AXI%0d)", layer_num));
                     end
                     else begin
                        verif_comp.set_abstract_name($sformatf("IO_MAXION_L3_SLV(AXI)"));
                     end
                     m_axi_bus_agents[axi_comp_id][layer_num] = verif_comp;
                  end
                  else if (inst_name == "m_maxion_uc_to_l3_master_axi") begin
                     axi_comp_id = int'(MAXION_UCTOL3_AXI);
                     layer_offset = m_global_attrs.get_ioshire_axi_port_index(axi_comp_id, 0, m_num_l3_ports, m_num_l3_master_ports, m_num_l3_slave_ports);
                     verif_comp.set_port_id(layer_offset);
                     verif_comp.set_port_type(axi_comp_id);
                     verif_comp.set_subport_id(axi_comp_id);
                     verif_comp.set_agent_id(layer_offset);
                     verif_comp.set_abstract_name($sformatf("MAXION_UCTOL3(AXI)"));
                     m_axi_bus_agents[axi_comp_id][0] = verif_comp;
                  end
                  else if (inst_name == "m_maxion_uc_to_sys_master_axi") begin
                     axi_comp_id = int'(MAXION_UCTOSYS_AXI);
                     layer_offset = m_global_attrs.get_ioshire_axi_port_index(axi_comp_id, 0, m_num_l3_ports, m_num_l3_master_ports, m_num_l3_slave_ports);
                     verif_comp.set_port_id(layer_offset);
                     verif_comp.set_port_type(axi_comp_id);
                     verif_comp.set_subport_id(axi_comp_id);
                     verif_comp.set_agent_id(layer_offset);
                     verif_comp.set_abstract_name($sformatf("MAXION_UCTOSYS(AXI)"));
                     m_axi_bus_agents[axi_comp_id][0] = verif_comp;
                  end
                  else if (inst_name == "m_maxion_to_sys_master_axi") begin
                     axi_comp_id = int'(MAXION_TOSYS_AXI);
                     layer_offset = m_global_attrs.get_ioshire_axi_port_index(axi_comp_id, 0, m_num_l3_ports, m_num_l3_master_ports, m_num_l3_slave_ports);
                     verif_comp.set_port_id(layer_offset);
                     verif_comp.set_port_type(axi_comp_id);
                     verif_comp.set_subport_id(axi_comp_id);
                     verif_comp.set_agent_id(layer_offset);
                     verif_comp.set_abstract_name($sformatf("MAXION_TOSYS(AXI)"));
                     m_axi_bus_agents[axi_comp_id][0] = verif_comp;
                  end
                  else if (inst_name == "m_maxion_sys_slave_axi") begin
                     axi_comp_id = int'(MAXION_SYS_SLV_AXI);
                     layer_offset = m_global_attrs.get_ioshire_axi_port_index(axi_comp_id, 0, m_num_l3_ports, m_num_l3_master_ports, m_num_l3_slave_ports);
                     verif_comp.set_port_id(layer_offset);
                     verif_comp.set_port_type(axi_comp_id);
                     verif_comp.set_subport_id(axi_comp_id);
                     verif_comp.set_agent_id(layer_offset);
                     verif_comp.set_abstract_name($sformatf("MAXION_SYS_SLV(AXI)"));
                     m_axi_bus_agents[axi_comp_id][0] = verif_comp;
                  end
                  else if (inst_name == "m_pu_noc_pu_scram_axi") begin
                     axi_comp_id = int'(PU_NOC_SRAM_AXI);
                     layer_offset = m_global_attrs.get_ioshire_axi_port_index(axi_comp_id, 0, m_num_l3_ports, m_num_l3_master_ports, m_num_l3_slave_ports);
                     verif_comp.set_port_id(layer_offset);
                     verif_comp.set_port_type(axi_comp_id);
                     verif_comp.set_subport_id(axi_comp_id);
                     verif_comp.set_agent_id(layer_offset);
                     verif_comp.set_abstract_name($sformatf("PU_NOC_PU(AXI)"));
                     m_axi_bus_agents[axi_comp_id][0] = verif_comp;
                  end
                  else if (inst_name == "m_pu_dma_pu_noc_axi") begin
                     axi_comp_id = int'(PU_NOC_DMA_AXI);
                     layer_offset = m_global_attrs.get_ioshire_axi_port_index(axi_comp_id, 0, m_num_l3_ports, m_num_l3_master_ports, m_num_l3_slave_ports);
                     verif_comp.set_port_id(layer_offset);
                     verif_comp.set_port_type(axi_comp_id);
                     verif_comp.set_subport_id(axi_comp_id);
                     verif_comp.set_agent_id(layer_offset);
                     verif_comp.set_abstract_name($sformatf("PU_NOC_DMA(AXI)"));
                     m_axi_bus_agents[axi_comp_id][0] = verif_comp;
                  end
                  else if (inst_name == "m_pu_dma_am_pu_noc_axi") begin
                     axi_comp_id = int'(PU_NOC_DMA_AM_AXI);
                     layer_offset = m_global_attrs.get_ioshire_axi_port_index(axi_comp_id, 0, m_num_l3_ports, m_num_l3_master_ports, m_num_l3_slave_ports);
                     verif_comp.set_port_id(layer_offset);
                     verif_comp.set_port_type(axi_comp_id);
                     verif_comp.set_subport_id(axi_comp_id);
                     verif_comp.set_agent_id(layer_offset);
                     verif_comp.set_abstract_name($sformatf("PU_NOC_DMA_AM(AXI)"));
                     m_axi_bus_agents[axi_comp_id][0] = verif_comp;
                  end
                  else if (inst_name == "m_pu_emmc_pu_noc_axi") begin
                     axi_comp_id = int'(PU_NOC_EMMC_AXI);
                     layer_offset = m_global_attrs.get_ioshire_axi_port_index(axi_comp_id, 0, m_num_l3_ports, m_num_l3_master_ports, m_num_l3_slave_ports);
                     verif_comp.set_port_id(layer_offset);
                     verif_comp.set_port_type(axi_comp_id);
                     verif_comp.set_subport_id(axi_comp_id);
                     verif_comp.set_agent_id(layer_offset);
                     verif_comp.set_abstract_name($sformatf("PU_NOC_EMMC(AXI)"));
                     m_axi_bus_agents[axi_comp_id][0] = verif_comp;
                  end
                  else if (inst_name == "m_pu_usb0_pu_noc_axi") begin
                     axi_comp_id = int'(PU_NOC_USB0_AXI);
                     layer_offset = m_global_attrs.get_ioshire_axi_port_index(axi_comp_id, 0, m_num_l3_ports, m_num_l3_master_ports, m_num_l3_slave_ports);
                     verif_comp.set_port_id(layer_offset);
                     verif_comp.set_port_type(axi_comp_id);
                     verif_comp.set_subport_id(axi_comp_id);
                     verif_comp.set_agent_id(layer_offset);
                     verif_comp.set_abstract_name($sformatf("PU_NOC_USB0(AXI)"));
                     m_axi_bus_agents[axi_comp_id][0] = verif_comp;
                  end
                  else if (inst_name == "m_pu_usb1_pu_noc_axi") begin
                     axi_comp_id = int'(PU_NOC_USB1_AXI);
                     layer_offset = m_global_attrs.get_ioshire_axi_port_index(axi_comp_id, 0, m_num_l3_ports, m_num_l3_master_ports, m_num_l3_slave_ports);
                     verif_comp.set_port_id(layer_offset);
                     verif_comp.set_port_type(axi_comp_id);
                     verif_comp.set_subport_id(axi_comp_id);
                     verif_comp.set_agent_id(layer_offset);
                     verif_comp.set_abstract_name($sformatf("PU_NOC_USB1(AXI)"));
                     m_axi_bus_agents[axi_comp_id][0] = verif_comp;
                  end
                  else if (inst_name == "m_pu_static_pu_noc_axi") begin
                     axi_comp_id = int'(PU_NOC_STATIC_AXI);
                     layer_offset = m_global_attrs.get_ioshire_axi_port_index(axi_comp_id, 0, m_num_l3_ports, m_num_l3_master_ports, m_num_l3_slave_ports);
                     verif_comp.set_port_id(layer_offset);
                     verif_comp.set_port_type(axi_comp_id);
                     verif_comp.set_subport_id(axi_comp_id);
                     verif_comp.set_agent_id(layer_offset);
                     verif_comp.set_abstract_name($sformatf("PU_NOC_STATIC(AXI)"));
                     m_axi_bus_agents[axi_comp_id][0] = verif_comp;
                  end
                  else if (inst_name == "m_ioshire_tol3_mesh_master_axi") begin
                     axi_comp_id = int'(IO_TOL3_AXI);
                     layer_offset = m_global_attrs.get_ioshire_axi_port_index(axi_comp_id, 0, m_num_l3_ports, m_num_l3_master_ports, m_num_l3_slave_ports);
                     verif_comp.set_port_id(layer_offset);
                     verif_comp.set_port_type(axi_comp_id);
                     verif_comp.set_subport_id(layer_num);
                     verif_comp.set_agent_id(layer_offset);
                     rtl_port_num = 0;
                     if (m_num_l3_ports > 1) begin
                        verif_comp.set_abstract_name($sformatf("IO_NOC_TOL3(AXI%0d)", layer_num));
                     end
                     else begin
                        verif_comp.set_abstract_name($sformatf("IO_NOC_TOL3(AXI)"));
                     end
                     m_axi_bus_agents[axi_comp_id][0] = verif_comp;
                  end
                  else if (inst_name == "m_ioshire_tol3b_mesh_master_axi") begin
                     axi_comp_id = int'(IO_TOL3_AXI);
                     layer_offset = m_global_attrs.get_ioshire_axi_port_index(axi_comp_id, 1, m_num_l3_ports, m_num_l3_master_ports, m_num_l3_slave_ports);
                     verif_comp.set_port_id(layer_offset);
                     verif_comp.set_port_type(axi_comp_id);
                     verif_comp.set_subport_id(layer_num);
                     verif_comp.set_agent_id(layer_offset);
                     rtl_port_num = 1;
                     if (m_num_l3_ports > 1) begin
                        verif_comp.set_abstract_name($sformatf("IO_NOC_TOL3(AXI%0d)", layer_num));
                     end
                     else begin
                        verif_comp.set_abstract_name($sformatf("IO_NOC_TOL3(AXI)"));
                     end
                     m_axi_bus_agents[axi_comp_id][1] = verif_comp;
                  end
                  else if (inst_name == "m_io_pm_master_axi") begin
                     axi_comp_id = int'(IO_PM_AXI);
                     layer_offset = m_global_attrs.get_ioshire_axi_port_index(axi_comp_id, 0, m_num_l3_ports, m_num_l3_master_ports, m_num_l3_slave_ports);
                     verif_comp.set_port_id(layer_offset);
                     verif_comp.set_port_type(axi_comp_id);
                     verif_comp.set_agent_id(layer_offset);
                     verif_comp.set_abstract_name($sformatf("IO_PU_NOC_PM(AXI)"));
                     rtl_port_num = 4;
                     m_axi_bus_agents[axi_comp_id][0] = verif_comp;
                  end
                  else if (inst_name == "m_io_ps_slave_axi") begin
                     axi_comp_id = int'(IO_PS_AXI);
                     layer_offset = m_global_attrs.get_ioshire_axi_port_index(axi_comp_id, 0, m_num_l3_ports, m_num_l3_master_ports, m_num_l3_slave_ports);
                     verif_comp.set_port_id(layer_offset);
                     verif_comp.set_port_type(axi_comp_id);
                     verif_comp.set_agent_id(layer_offset);
                     verif_comp.set_abstract_name($sformatf("IO_NOC_PS(AXI)"));
                     m_axi_bus_agents[axi_comp_id][0] = verif_comp;
                  end
                  else if (inst_name == "m_io_tosys_mesh_master_axi") begin
                     axi_comp_id = int'(IO_TOSYS_AXI);
                     layer_offset = m_global_attrs.get_ioshire_axi_port_index(axi_comp_id, 0, m_num_l3_ports, m_num_l3_master_ports, m_num_l3_slave_ports);
                     verif_comp.set_port_id(layer_offset);
                     verif_comp.set_port_type(axi_comp_id);
                     verif_comp.set_agent_id(layer_offset);
                     verif_comp.set_abstract_name($sformatf("IO_NOC_TOSYS(AXI)"));
                     rtl_port_num = 2;
                     m_axi_bus_agents[axi_comp_id][0] = verif_comp;
                  end
                  else if (inst_name == "m_spio_sp_main0_main_noc_rbm_s") begin
                     axi_comp_id = int'(IO_MAIN_NOC_RBM);
                     layer_offset = m_global_attrs.get_ioshire_axi_port_index(axi_comp_id, 0, m_num_l3_ports, m_num_l3_master_ports, m_num_l3_slave_ports);
                     verif_comp.set_port_id(layer_offset);
                     verif_comp.set_port_type(axi_comp_id);
                     verif_comp.set_agent_id(layer_offset);
                     verif_comp.set_abstract_name($sformatf("IO_NOC_RBM(AXI)"));
                     rtl_port_num = 3;
                     m_axi_bus_agents[axi_comp_id][0] = verif_comp;
                  end
                  else if (inst_name == "m_spio_p10_pcie_rbm_s") begin
                     axi_comp_id = int'(IO_PCIE_RBM);
                     layer_offset = m_global_attrs.get_ioshire_axi_port_index(axi_comp_id, 0, m_num_l3_ports, m_num_l3_master_ports, m_num_l3_slave_ports);
                     verif_comp.set_port_id(layer_offset);
                     verif_comp.set_port_type(axi_comp_id);
                     verif_comp.set_agent_id(layer_offset);
                     verif_comp.set_abstract_name($sformatf("IO_PCIE_RBM(AXI)"));
                     m_axi_bus_agents[axi_comp_id][0] = verif_comp;
                  end
                  else if (inst_name == "m_spio_noc_pu_noc_regbus_axi") begin
                     axi_comp_id = int'(IO_PU_NOC_RBM);
                     layer_offset = m_global_attrs.get_ioshire_axi_port_index(axi_comp_id, 0, m_num_l3_ports, m_num_l3_master_ports, m_num_l3_slave_ports);
                     verif_comp.set_port_id(layer_offset);
                     verif_comp.set_port_type(axi_comp_id);
                     verif_comp.set_agent_id(layer_offset);
                     verif_comp.set_abstract_name($sformatf("IO_PU_NOC_RBM(AXI)"));
                     m_axi_bus_agents[axi_comp_id][0] = verif_comp;
                  end
               end
               else if (verif_comp.get_agent_type() == APB_BUS_AGENT) begin
                  verif_comp.set_agent_id(get_shire_id());
                  if (instance_list[ii].get_inst_name() == "m_spio_noc_apb0") begin
                     apb_comp_id = int'(SPIO_NOC_APB0);
                     verif_comp.set_port_id(apb_comp_id);
                     verif_comp.set_agent_id(apb_comp_id);
                     verif_comp.set_abstract_name($sformatf("SPIO_NOC(APB0)"));
                     m_apb_bus_agents[apb_comp_id] = verif_comp;
                  end
                  else if (instance_list[ii].get_inst_name() == "m_spio_noc_apb1") begin
                     apb_comp_id = int'(SPIO_NOC_APB1);
                     verif_comp.set_port_id(apb_comp_id);
                     verif_comp.set_agent_id(apb_comp_id);
                     verif_comp.set_abstract_name($sformatf("SPIO_NOC(APB1)"));
                     m_apb_bus_agents[apb_comp_id] = verif_comp;
                  end
               end

               add_vc_child(verif_comp);

               //
               // Create stimulus agents as necessary.
               //
               if (m_ioshire_stim == 1) begin // --------------------------------------------------{
                  if (axi_comp_id >= 0) begin
                     if (axi_comp_id == IO_TOL3_AXI) begin
                        verif_comp.set_active_master();
                        if (!m_axi_stim_agents.exists(axi_comp_id)) begin
                           component = factory.create_component_by_name("evl_l2tol3_master_agent_axi", "", $sformatf("io_main_noc_l2tol3_master"), this);
                        end
                        else begin
                           m_axi_stim_agents[axi_comp_id].set_rtl_port(m_axi_bus_agents[axi_comp_id][layer_num].get_rtl_port(), layer_num);
                           component = null;
                        end
                     end
                     else if (axi_comp_id == IO_PM_AXI) begin
                        verif_comp.set_active_master();
                        if (!m_axi_stim_agents.exists(axi_comp_id)) begin
                           component = factory.create_component_by_name("evl_io_pm_master_agent_axi", "", $sformatf("io_main_noc_pm_master"), this);
                        end
                        else begin
                           m_axi_stim_agents[axi_comp_id].set_rtl_port(m_axi_bus_agents[axi_comp_id][layer_num].get_rtl_port(), layer_num);
                           component = null;
                        end
                     end
                     else if (axi_comp_id == IO_PS_AXI) begin
                        verif_comp.set_active_slave();
                        if (!m_axi_stim_agents.exists(axi_comp_id)) begin
                           component = factory.create_component_by_name("evl_shire_slave_agent_axi", "", $sformatf("io_main_noc_ps_slave"), this);
                        end
                        else begin
                           component = null;
                        end
                     end
                     else if (axi_comp_id == IO_TOSYS_AXI) begin
                        verif_comp.set_active_master();
                        if (!m_axi_stim_agents.exists(axi_comp_id)) begin
                           component = factory.create_component_by_name("evl_iotosys_master_agent_axi", "", $sformatf("io_main_noc_tosys_master"), this);
                        end
                        else begin
                           m_axi_stim_agents[axi_comp_id].set_rtl_port(m_axi_bus_agents[axi_comp_id][layer_num].get_rtl_port(), layer_num);
                           component = null;
                        end
                     end
                     else if (axi_comp_id == IO_MAIN_NOC_RBM) begin
                        verif_comp.set_active_master();
                        if (!m_axi_stim_agents.exists(axi_comp_id)) begin
                           component = factory.create_component_by_name("evl_rbm_master_agent_axi", "", $sformatf("io_main_noc_rbm_master"), this);
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
                        case (axi_comp_id)
                           IO_TOL3_AXI: begin
                              verif_comp.set_abstract_name($sformatf("IO_TOL3_AXI(STIM)"));
                              verif_comp.set_rtl_port(m_axi_bus_agents[axi_comp_id][layer_num].get_rtl_port(), layer_num);
                              verif_comp.set_int_param(pk_num_l3_ports, m_num_l3_ports);
                           end
                           IO_PM_AXI: begin
                              verif_comp.set_abstract_name($sformatf("IO_PM_AXI(STIM)"));
                              verif_comp.set_rtl_port(m_axi_bus_agents[axi_comp_id][0].get_rtl_port(), layer_num);
                           end
                           IO_PS_AXI: begin
                              verif_comp.set_abstract_name($sformatf("IO_PS_AXI(STIM)"));
                              verif_comp.set_rtl_port(m_axi_bus_agents[axi_comp_id][0].get_rtl_port(), layer_num);
                              verif_comp.set_int_param(pk_allow_loopback, 1);
                           end
                           IO_TOSYS_AXI: begin
                              verif_comp.set_abstract_name($sformatf("IO_TOSYS_AXI(STIM)"));
                              verif_comp.set_rtl_port(m_axi_bus_agents[axi_comp_id][0].get_rtl_port(), layer_num);
                              verif_comp.set_int_param(pk_allow_loopback, 1);
                           end
                           IO_MAIN_NOC_RBM: begin
                              verif_comp.set_abstract_name($sformatf("IO_NOC_RBM(STIM)"));
                              verif_comp.set_rtl_port(m_axi_bus_agents[axi_comp_id][0].get_rtl_port(), layer_num);
                           end
                        endcase
                        m_axi_stim_agents[axi_comp_id] = verif_comp;
                        add_vc_child(verif_comp);
                     end
                  end
               end
               else if (m_io_noc_only == 1) begin // }---------------------------------------------{
                  if (axi_comp_id >= 0) begin
                     if (axi_comp_id == MAXION_TOL3_AXI) begin
                        verif_comp.set_active_master();
                        if (!m_axi_stim_agents.exists(axi_comp_id)) begin
                           component = factory.create_component_by_name("evl_l2tol3_master_agent_axi", "", $sformatf("io_maxion_l2tol3_master"), this);
                        end
                        else begin
                           m_axi_stim_agents[axi_comp_id].set_rtl_port(m_axi_bus_agents[axi_comp_id][layer_num].get_rtl_port(), layer_num);
                           component = null;
                        end
                     end
                     else if (axi_comp_id == MAXION_UCTOL3_AXI) begin
                        verif_comp.set_active_master();
                        if (!m_axi_stim_agents.exists(axi_comp_id)) begin
                           component = factory.create_component_by_name("evl_uctol3_master_agent_axi", "", $sformatf("io_maxion_uctol3_master"), this);
                        end
                        else begin
                           m_axi_stim_agents[axi_comp_id].set_rtl_port(m_axi_bus_agents[axi_comp_id][layer_num].get_rtl_port(), layer_num);
                           component = null;
                        end
                     end
                     else if (axi_comp_id == MAXION_TOSYS_AXI) begin
                        verif_comp.set_active_master();
                        if (!m_axi_stim_agents.exists(axi_comp_id)) begin
                           component = factory.create_component_by_name("evl_tosys_master_agent_axi", "", $sformatf("io_maxion_tosys_master"), this);
                        end
                        else begin
                           m_axi_stim_agents[axi_comp_id].set_rtl_port(m_axi_bus_agents[axi_comp_id][layer_num].get_rtl_port(), layer_num);
                           component = null;
                        end
                     end
                     else if (axi_comp_id == MAXION_UCTOSYS_AXI) begin
                        verif_comp.set_active_master();
                        if (!m_axi_stim_agents.exists(axi_comp_id)) begin
                           component = factory.create_component_by_name("evl_uctosys_master_agent_axi", "", $sformatf("io_maxion_uctosys_master"), this);
                        end
                        else begin
                           m_axi_stim_agents[axi_comp_id].set_rtl_port(m_axi_bus_agents[axi_comp_id][layer_num].get_rtl_port(), layer_num);
                           component = null;
                        end
                     end
                     else if (axi_comp_id == MAXION_L3_SLV_AXI) begin
                        verif_comp.set_active_slave();
                        if (!m_axi_stim_agents.exists(axi_comp_id)) begin
                           component = factory.create_component_by_name("evl_shire_slave_agent_axi", "", $sformatf("io_maxion_l3_mesh_slave"), this);
                        end
                        else begin
                           m_axi_stim_agents[axi_comp_id].set_rtl_port(m_axi_bus_agents[axi_comp_id][layer_num].get_rtl_port(), layer_num);
                           component = null;
                        end
                     end
                     else if (axi_comp_id == MAXION_SYS_SLV_AXI) begin
                        verif_comp.set_active_slave();
                        if (!m_axi_stim_agents.exists(axi_comp_id)) begin
                           component = factory.create_component_by_name("evl_shire_slave_agent_axi", "", $sformatf("io_maxion_sys_slave"), this);
                        end
                        else begin
                           component = null;
                        end
                     end
                     else if (axi_comp_id == SPIO_ROM_AXI) begin
                        verif_comp.set_active_slave();
                        if (!m_axi_stim_agents.exists(axi_comp_id)) begin
                           component = factory.create_component_by_name("evl_shire_slave_agent_axi", "", $sformatf("sp_rom_slave"), this);
                        end
                        else begin
                           component = null;
                        end
                     end
                     else if (axi_comp_id == SPIO_SCRAM_AXI) begin
                        verif_comp.set_active_slave();
                        if (!m_axi_stim_agents.exists(axi_comp_id)) begin
                           component = factory.create_component_by_name("evl_shire_slave_agent_axi", "", $sformatf("sp_ram_slave"), this);
                        end
                        else begin
                           component = null;
                        end
                     end
                     else if (axi_comp_id == SPIO_E2A_AXI) begin
                        verif_comp.set_active_master();
                        if (!m_axi_stim_agents.exists(axi_comp_id)) begin
                           component = factory.create_component_by_name("evl_spio_e2a_master_agent_axi", "", $sformatf("sp_e2a_master"), this);
                        end
                        else begin
                           component = null;
                        end
                     end
                     else if (axi_comp_id == SPIO_DBG_COMM_AXI) begin
                        verif_comp.set_active_slave();
                        if (!m_axi_stim_agents.exists(axi_comp_id)) begin
                           component = factory.create_component_by_name("evl_shire_slave_agent_axi", "", $sformatf("sp_dbg_slave"), this);
                        end
                        else begin
                           component = null;
                        end
                     end
                     else if (axi_comp_id == SPIO_AM_AXI) begin
                        verif_comp.set_active_master();
                        if (!m_axi_stim_agents.exists(axi_comp_id)) begin
                           component = factory.create_component_by_name("evl_spio_am_master_agent_axi", "", $sformatf("sp_dma_am_master"), this);
                        end
                        else begin
                           component = null;
                        end
                     end
                     else if (axi_comp_id == SPIO_DMA_AXI) begin
                        verif_comp.set_active_master();
                        if (!m_axi_stim_agents.exists(axi_comp_id)) begin
                           component = factory.create_component_by_name("evl_spio_dma_master_agent_axi", "", $sformatf("sp_dma_master"), this);
                        end
                        else begin
                           component = null;
                        end
                     end
                     else if (axi_comp_id == SPIO_VAULT_AXI) begin
                        verif_comp.set_active_master();
                        if (!m_axi_stim_agents.exists(axi_comp_id)) begin
                           component = factory.create_component_by_name("evl_spio_vault_master_agent_axi", "", $sformatf("sp_vault_master"), this);
                        end
                        else begin
                           component = null;
                        end
                     end
                     if (m_io_no_pu == 1'b1) begin
                        if (axi_comp_id == PU_NOC_STATIC_AXI) begin
                           verif_comp.set_active_slave();
                           if (!m_axi_stim_agents.exists(axi_comp_id)) begin
                              component = factory.create_component_by_name("evl_shire_slave_agent_axi", "", $sformatf("pu_static_slave"), this);
                           end
                           else begin
                              component = null;
                           end
                        end
                        else if (axi_comp_id == PU_NOC_SRAM_AXI) begin
                           verif_comp.set_active_slave();
                           if (!m_axi_stim_agents.exists(axi_comp_id)) begin
                              component = factory.create_component_by_name("evl_shire_slave_agent_axi", "", $sformatf("pu_sram_slave"), this);
                           end
                           else begin
                              component = null;
                           end
                        end
                        else if (axi_comp_id == PU_NOC_DMA_AXI) begin
                           verif_comp.set_active_master();
                           if (!m_axi_stim_agents.exists(axi_comp_id)) begin
                              component = factory.create_component_by_name("evl_pu_dma_master_agent_axi", "", $sformatf("pu_dma_master"), this);
                           end
                           else begin
                              component = null;
                           end
                        end
                        else if (axi_comp_id == PU_NOC_DMA_AM_AXI) begin
                           verif_comp.set_active_master();
                           if (!m_axi_stim_agents.exists(axi_comp_id)) begin
                              component = factory.create_component_by_name("evl_pu_dma_am_master_agent_axi", "", $sformatf("pu_dma_am_master"), this);
                           end
                           else begin
                              component = null;
                           end
                        end
                        else if (axi_comp_id == PU_NOC_EMMC_AXI) begin
                           verif_comp.set_active_master();
                           if (!m_axi_stim_agents.exists(axi_comp_id)) begin
                              component = factory.create_component_by_name("evl_pu_emmc_master_agent_axi", "", $sformatf("pu_emmc_master"), this);
                           end
                           else begin
                              component = null;
                           end
                        end
                        else if (axi_comp_id == PU_NOC_USB0_AXI) begin
                           verif_comp.set_active_master();
                           if (!m_axi_stim_agents.exists(axi_comp_id)) begin
                              component = factory.create_component_by_name("evl_pu_usb_master_agent_axi", "", $sformatf("pu_usb0_master"), this);
                           end
                           else begin
                              component = null;
                           end
                        end
                        else if (axi_comp_id == PU_NOC_USB1_AXI) begin
                           verif_comp.set_active_master();
                           if (!m_axi_stim_agents.exists(axi_comp_id)) begin
                              component = factory.create_component_by_name("evl_pu_usb_master_agent_axi", "", $sformatf("pu_usb1_master"), this);
                           end
                           else begin
                              component = null;
                           end
                        end
                     end
                     if (component != null) begin
                        $cast(verif_comp, component);
                        verif_comp.set_shire_id(get_shire_id());
                        verif_comp.set_port_id(axi_comp_id);
                        case (axi_comp_id)
                           MAXION_TOL3_AXI: begin
                              verif_comp.set_abstract_name($sformatf("MAXION_TOL3_AXI(STIM)"));
                              verif_comp.set_rtl_port(m_axi_bus_agents[axi_comp_id][layer_num].get_rtl_port(), layer_num);
                              verif_comp.set_int_param(pk_num_l3_ports, m_num_l3_master_ports);
                           end
                           MAXION_L3_SLV_AXI: begin
                              verif_comp.set_abstract_name($sformatf("MAXION_L3_SLV_AXI(STIM)"));
                              verif_comp.set_rtl_port(m_axi_bus_agents[axi_comp_id][layer_num].get_rtl_port(), layer_num);
                              verif_comp.set_int_param(pk_num_l3_ports, m_num_l3_slave_ports);
                           end
                           MAXION_UCTOL3_AXI: begin
                              verif_comp.set_abstract_name($sformatf("MAXION_UCTOL3_AXI(STIM)"));
                              verif_comp.set_rtl_port(m_axi_bus_agents[axi_comp_id][0].get_rtl_port(), layer_num);
                           end
                           MAXION_TOSYS_AXI: begin
                              verif_comp.set_abstract_name($sformatf("MAXION_TOSYS_AXI(STIM)"));
                              verif_comp.set_rtl_port(m_axi_bus_agents[axi_comp_id][0].get_rtl_port(), layer_num);
                           end
                           MAXION_UCTOSYS_AXI: begin
                              verif_comp.set_abstract_name($sformatf("MAXION_UCTOSYS_AXI(STIM)"));
                              verif_comp.set_rtl_port(m_axi_bus_agents[axi_comp_id][0].get_rtl_port(), layer_num);
                           end
                           MAXION_SYS_SLV_AXI: begin
                              verif_comp.set_abstract_name($sformatf("MAXION_SYS_SLV_AXI(STIM)"));
                              verif_comp.set_rtl_port(m_axi_bus_agents[axi_comp_id][0].get_rtl_port(), layer_num);
                           end
                           SPIO_ROM_AXI: begin
                              verif_comp.set_abstract_name($sformatf("SPIO_ROM_AXI(STIM)"));
                              verif_comp.set_rtl_port(m_axi_bus_agents[axi_comp_id][0].get_rtl_port(), layer_num);
                           end
                           SPIO_SCRAM_AXI: begin
                              verif_comp.set_abstract_name($sformatf("SPIO_SCRAM_AXI(STIM)"));
                              verif_comp.set_rtl_port(m_axi_bus_agents[axi_comp_id][0].get_rtl_port(), layer_num);
                           end
                           SPIO_E2A_AXI: begin
                              verif_comp.set_abstract_name($sformatf("SPIO_E2A_AXI(STIM)"));
                              verif_comp.set_rtl_port(m_axi_bus_agents[axi_comp_id][0].get_rtl_port(), layer_num);
                              verif_comp.set_int_param_by_name("pk_observe_descs", 0);
                           end
                           SPIO_DBG_COMM_AXI: begin
                              verif_comp.set_abstract_name($sformatf("SPIO_DBG_COMM_AXI(STIM)"));
                              verif_comp.set_rtl_port(m_axi_bus_agents[axi_comp_id][0].get_rtl_port(), layer_num);
                           end
                           SPIO_AM_AXI: begin
                              verif_comp.set_abstract_name($sformatf("SPIO_AM_AXI(STIM)"));
                              verif_comp.set_rtl_port(m_axi_bus_agents[axi_comp_id][0].get_rtl_port(), layer_num);
                           end
                           SPIO_DMA_AXI: begin
                              verif_comp.set_abstract_name($sformatf("SPIO_DMA_AXI(STIM)"));
                              verif_comp.set_rtl_port(m_axi_bus_agents[axi_comp_id][0].get_rtl_port(), layer_num);
                           end
                           SPIO_VAULT_AXI: begin
                              verif_comp.set_abstract_name($sformatf("SPIO_VAULT_AXI(STIM)"));
                              verif_comp.set_rtl_port(m_axi_bus_agents[axi_comp_id][0].get_rtl_port(), layer_num);
                           end
                           PU_NOC_SRAM_AXI: begin
                              verif_comp.set_abstract_name($sformatf("PU_NOC_SRAM_AXI(STIM)"));
                              verif_comp.set_rtl_port(m_axi_bus_agents[axi_comp_id][0].get_rtl_port(), layer_num);
                           end
                           PU_NOC_DMA_AXI: begin
                              verif_comp.set_abstract_name($sformatf("PU_NOC_DMA_AXI(STIM)"));
                              verif_comp.set_rtl_port(m_axi_bus_agents[axi_comp_id][0].get_rtl_port(), layer_num);
                           end
                           PU_NOC_DMA_AM_AXI: begin
                              verif_comp.set_abstract_name($sformatf("PU_NOC_DMA_AM_AXI(STIM)"));
                              verif_comp.set_rtl_port(m_axi_bus_agents[axi_comp_id][0].get_rtl_port(), layer_num);
                           end
                           PU_NOC_EMMC_AXI: begin
                              verif_comp.set_abstract_name($sformatf("PU_NOC_EMMC_AXI(STIM)"));
                              verif_comp.set_rtl_port(m_axi_bus_agents[axi_comp_id][0].get_rtl_port(), layer_num);
                           end
                           PU_NOC_USB0_AXI: begin
                              verif_comp.set_abstract_name($sformatf("PU_NOC_USB0_AXI(STIM)"));
                              verif_comp.set_rtl_port(m_axi_bus_agents[axi_comp_id][0].get_rtl_port(), layer_num);
                           end
                           PU_NOC_USB1_AXI: begin
                              verif_comp.set_abstract_name($sformatf("PU_NOC_USB1_AXI(STIM)"));
                              verif_comp.set_rtl_port(m_axi_bus_agents[axi_comp_id][0].get_rtl_port(), layer_num);
                           end
                           PU_NOC_STATIC_AXI: begin
                              verif_comp.set_abstract_name($sformatf("PU_NOC_STATIC_AXI(STIM)"));
                              verif_comp.set_rtl_port(m_axi_bus_agents[axi_comp_id][0].get_rtl_port(), layer_num);
                           end
                        endcase
                        m_axi_stim_agents[axi_comp_id] = verif_comp;
                        add_vc_child(verif_comp);
                     end
                  end
               end
               else begin // }---------------------------------------------------------------------{
                  if (sp_is_stub != 0) begin // ---------------------------------------------------{
                     if (axi_comp_id >= 0) begin
                        if (axi_comp_id == SPIO_E2A_AXI) begin
                           verif_comp.set_active_master();
                           if (!m_axi_stim_agents.exists(axi_comp_id)) begin
                              component = factory.create_component_by_name("evl_spio_e2a_master_agent_axi", "", $sformatf("sp_e2a_master"), this);
                           end
                           else begin
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
                           verif_comp.set_abstract_name($sformatf("E2A_AXI(STIM)"));
                           verif_comp.set_rtl_port(m_axi_bus_agents[axi_comp_id][0].get_rtl_port());
                           m_axi_stim_agents[axi_comp_id] = verif_comp;
                           add_vc_child(verif_comp);
                        end
                     end
                     else if (apb_comp_id >= 0) begin
                        component = null;
                        if (component != null) begin
                           $cast(verif_comp, component);
                           verif_comp.set_shire_id(get_shire_id());
                           verif_comp.set_port_id(apb_comp_id);
                           verif_comp.set_agent_id(apb_comp_id);
                           verif_comp.set_abstract_name($sformatf("%0s(STIM)", m_apb_bus_agents[apb_comp_id].get_root_abstract_name()));
                           verif_comp.set_rtl_port(m_apb_bus_agents[apb_comp_id].get_rtl_port());
                           m_apb_stim_agents[apb_comp_id] = verif_comp;
                           add_vc_child(verif_comp);
                        end
                     end
                     else if (etlink_comp_id >= 0) begin
                        component = null;
                        if (component != null) begin
                           $cast(verif_comp, component);
                           verif_comp.set_shire_id(get_shire_id());
                           verif_comp.set_port_id(etlink_comp_id);
                           verif_comp.set_agent_id(etlink_comp_id);
                           verif_comp.set_abstract_name($sformatf("%0s(STIM)", m_etlink_bus_agents[etlink_comp_id].get_root_abstract_name()));
                           verif_comp.set_rtl_port(m_etlink_bus_agents[etlink_comp_id].get_rtl_port());
                           m_etlink_stim_agents[etlink_comp_id] = verif_comp;
                           add_vc_child(verif_comp);
                        end
                     end
                  end // if (sp_is_stub != 0) ... -------------------------------------------------}
                  if (m_noc_is_stub != 0) begin // ------------------------------------------------{
                     bus_rtl_port = null;
                     if (axi_comp_id >= 0) begin
                        if (axi_comp_id == IO_TOL3_AXI) begin
                           verif_comp.set_active_slave();
                           bus_rtl_port = m_axi_bus_agents[axi_comp_id][layer_num].get_rtl_port();
                           if (!m_axi_stim_agents.exists(axi_comp_id)) begin
                              component = factory.create_component_by_name("evl_io_shire_slave_agent_axi", "", $sformatf("io_mesh_slave"), this);
                           end
                           else begin
                              m_axi_stim_agents[axi_comp_id].set_rtl_port(bus_rtl_port, rtl_port_num);
                              component = null;
                           end
                        end
                        else if (axi_comp_id == IO_TOSYS_AXI) begin
                           verif_comp.set_active_slave();
                           bus_rtl_port = m_axi_bus_agents[axi_comp_id][layer_num].get_rtl_port();
                           if (!m_axi_stim_agents.exists(axi_comp_id)) begin
                              component = factory.create_component_by_name("evl_io_shire_slave_agent_axi", "", $sformatf("io_mesh_slave"), this);
                           end
                           else begin
                              m_axi_stim_agents[axi_comp_id].set_rtl_port(bus_rtl_port, rtl_port_num);
                              component = null;
                           end
                        end
                        else if (axi_comp_id == IO_MAIN_NOC_RBM) begin
                           verif_comp.set_active_slave();
                           bus_rtl_port = m_axi_bus_agents[axi_comp_id][layer_num].get_rtl_port();
                           if (!m_axi_stim_agents.exists(axi_comp_id)) begin
                              component = factory.create_component_by_name("evl_io_shire_slave_agent_axi", "", $sformatf("io_mesh_slave"), this);
                           end
                           else begin
                              m_axi_stim_agents[axi_comp_id].set_rtl_port(bus_rtl_port, rtl_port_num);
                              component = null;
                           end
                        end
                        else if (axi_comp_id == IO_PM_AXI) begin
                           verif_comp.set_active_slave();
                           bus_rtl_port = m_axi_bus_agents[axi_comp_id][layer_num].get_rtl_port();
                           if (!m_axi_stim_agents.exists(axi_comp_id)) begin
                              component = factory.create_component_by_name("evl_io_shire_slave_agent_axi", "", $sformatf("io_mesh_slave"), this);
                           end
                           else begin
                              m_axi_stim_agents[axi_comp_id].set_rtl_port(bus_rtl_port, rtl_port_num);
                              component = null;
                           end
                        end
                        else if (axi_comp_id == IO_PS_AXI) begin
                           verif_comp.set_active_master();
                           if (!m_axi_stim_agents.exists(axi_comp_id)) begin
                              component = factory.create_component_by_name("evl_generic_master_agent_axi", "", $sformatf("io_master"), this);
                           end
                           else begin
                              component = null;
                           end
                        end
//                      if (axi_comp_id == SPIO_ROM_AXI) begin
//                         verif_comp.set_active_master();
//                         if (!m_axi_stim_agents.exists(axi_comp_id)) begin
//                            component = factory.create_component_by_name("evl_spio_rom_master_agent_axi", "", $sformatf("sp_rom_master"), this);
//                         end
//                         else begin
//                            component = null;
//                         end
//                      end
//                      else if (axi_comp_id == SPIO_SCRAM_AXI) begin
//                         verif_comp.set_active_master();
//                         if (!m_axi_stim_agents.exists(axi_comp_id)) begin
//                            component = factory.create_component_by_name("evl_spio_ram_master_agent_axi", "", $sformatf("sp_ram_master"), this);
//                         end
//                         else begin
//                            component = null;
//                         end
//                      end
//                      else if (axi_comp_id == SPIO_NOC_PU_NOC_AXI) begin
//                         verif_comp.set_active_master();
//                         if (!m_axi_stim_agents.exists(axi_comp_id)) begin
//                            component = factory.create_component_by_name("evl_spio_ram_master_agent_axi", "", $sformatf("spio_noc_master"), this);
//                         end
//                         else begin
//                            component = null;
//                         end
//                      end
                        else begin
                           component = null;
                        end
                        if (component != null) begin
                           $cast(verif_comp, component);
                           verif_comp.set_shire_id(get_shire_id());
                           verif_comp.set_port_id(axi_comp_id);
                           if (axi_comp_id inside { IO_TOL3_AXI, IO_TOSYS_AXI, IO_MAIN_NOC_RBM, IO_PM_AXI }) begin
                              verif_comp.set_abstract_name($sformatf("NOC_MESH(STIM)"));
                              verif_comp.set_rtl_port(bus_rtl_port, rtl_port_num);
                              verif_comp.set_int_param(pk_num_l3_ports, m_num_l3_ports);
                              verif_comp.set_int_param(pk_num_l3_master_ports, m_num_l3_ports);
                              verif_comp.set_int_param(pk_num_l3_slave_ports, m_num_l3_ports);
                              m_axi_stim_agents[int'(IO_TOL3_AXI)]     = verif_comp;
                              m_axi_stim_agents[int'(IO_TOSYS_AXI)]    = verif_comp;
                              m_axi_stim_agents[int'(IO_MAIN_NOC_RBM)] = verif_comp;
                              m_axi_stim_agents[int'(IO_PM_AXI)]       = verif_comp;
                           end
                           else begin
                              verif_comp.set_agent_id(axi_comp_id);
                              verif_comp.set_rtl_port(m_axi_bus_agents[axi_comp_id][0].get_rtl_port());
                              verif_comp.set_abstract_name($sformatf("%0s(STIM)", m_axi_bus_agents[axi_comp_id][0].get_root_abstract_name()));
                              m_axi_stim_agents[axi_comp_id] = verif_comp;
                           end
                           add_vc_child(verif_comp);
                        end
                     end
                  end // if (m_noc_is_stub != 0) ... ----------------------------------------------}
               end // if (m_io_noc_only == 1) ... else ... ----------------------------------------}
               if ((m_io_no_pu == 1) && (m_io_noc_only == 1) || (m_ioshire_stim == 1)) begin
                  if (dbgnoc_comp_id >= 0) begin
                     if (dbgnoc_comp_id == IO_DBG_RT_EVT) begin
                        component = factory.create_component_by_name("evl_stim_agent_dbg_noc", "", $sformatf("io_rt_evt_%0d", dbgnoc_comp_id), this);
                        if (component != null) begin
                           verif_comp.set_active_master();
                           $cast(verif_comp, component);
                           m_dbgnoc_stim_agents[dbgnoc_comp_id] = verif_comp;
                           verif_comp.set_abstract_name($sformatf("DBGNOC_RT_EVT(STIM)"));
                           verif_comp.set_agent_id(dbgnoc_comp_id);
                           verif_comp.set_shire_id(get_shire_id());
                           verif_comp.set_rtl_port(m_dbgnoc_bus_agents[dbgnoc_comp_id].get_rtl_port());
                           add_vc_child(verif_comp);
                        end
                     end
                     else if (dbgnoc_comp_id == IO_DBG_RT_MSG) begin
                        component = factory.create_component_by_name("evl_stim_agent_dbg_noc", "", $sformatf("io_rt_msg_%0d", dbgnoc_comp_id), this);
                        if (component != null) begin
                           verif_comp.set_active_master();
                           $cast(verif_comp, component);
                           m_dbgnoc_stim_agents[dbgnoc_comp_id] = verif_comp;
                           verif_comp.set_abstract_name($sformatf("DBGNOC_RT_MSG(STIM)"));
                           verif_comp.set_agent_id(dbgnoc_comp_id);
                           verif_comp.set_shire_id(get_shire_id());
                           verif_comp.set_rtl_port(m_dbgnoc_bus_agents[dbgnoc_comp_id].get_rtl_port());
                           add_vc_child(verif_comp);
                        end
                     end
                     else if (dbgnoc_comp_id == IO_DBG_LF_EVT) begin
                        component = factory.create_component_by_name("evl_stim_agent_dbg_noc", "", $sformatf("io_lf_evt_%0d", dbgnoc_comp_id), this);
                        if (component != null) begin
                           verif_comp.set_active_master();
                           $cast(verif_comp, component);
                           m_dbgnoc_stim_agents[dbgnoc_comp_id] = verif_comp;
                           verif_comp.set_abstract_name($sformatf("DBGNOC_LF_EVT(STIM)"));
                           verif_comp.set_agent_id(dbgnoc_comp_id);
                           verif_comp.set_shire_id(get_shire_id());
                           verif_comp.set_rtl_port(m_dbgnoc_bus_agents[dbgnoc_comp_id].get_rtl_port());
                           add_vc_child(verif_comp);
                        end
                     end
                     else if (dbgnoc_comp_id == IO_DBG_LF_MSG) begin
                        component = factory.create_component_by_name("evl_stim_agent_dbg_noc", "", $sformatf("io_lf_msg_%0d", dbgnoc_comp_id), this);
                        if (component != null) begin
                           verif_comp.set_active_master();
                           $cast(verif_comp, component);
                           m_dbgnoc_stim_agents[dbgnoc_comp_id] = verif_comp;
                           verif_comp.set_abstract_name($sformatf("DBGNOC_LF_MSG(STIM)"));
                           verif_comp.set_agent_id(dbgnoc_comp_id);
                           verif_comp.set_shire_id(get_shire_id());
                           verif_comp.set_rtl_port(m_dbgnoc_bus_agents[dbgnoc_comp_id].get_rtl_port());
                           add_vc_child(verif_comp);
                        end
                     end
                  end
               end
            end // if (verif_comp.is_bus_vc() == 1'b1) ... ----------------------------------------}
            //
            // RTL Verification Components
            //
            else begin // -------------------------------------------------------------------------{
               if (verif_comp.get_agent_type() == SHIRE_CHANNEL_RTL_AGENT) begin
                  m_shire_channel_agent = verif_comp;
                  verif_comp.set_int_param(pk_num_l3_ports, m_num_l3_ports);
                  verif_comp.set_int_param(pk_num_l3_master_ports, m_num_l3_ports);
                  verif_comp.set_int_param(pk_num_l3_slave_ports, m_num_l3_ports);
                  verif_comp.set_shire_id(get_shire_id());
               end

               `ifdef L1SB_EVL_MINION_ENV
                  else if (verif_comp.get_agent_type() == CORE_RTL_AGENT) begin
                     //m_sp_agent = verif_comp;
                     m_core_agent = verif_comp;
                     verif_comp.set_shire_id(get_shire_id());
                     verif_comp.set_neighborhood_id(0);
                     verif_comp.set_core_id(agent_id);
                     verif_comp.set_agent_id(agent_id);
                     verif_comp.set_abstract_name($sformatf("C%0d", agent_id));
                     `evl_log(UVM_DEBUG, VMOD_CONFIG, get_abstract_name(), $sformatf("(CYE DEBUG: evl_ioshire_env) CORE_RTL_AGENT is found and assigned to m_core_agent"))
                  end
               `else // when using new evl_standalone_minion_env
                  else if (verif_comp.get_agent_type() == STANDALONE_MINION_RTL_AGENT) begin
                     m_standalone_minion_agent = verif_comp;
                     verif_comp.set_shire_id(get_shire_id());
                     verif_comp.set_neighborhood_id(0);       // there is no "neigh" in io_shire 
                     verif_comp.set_core_id(agent_id);
                     verif_comp.set_agent_id(agent_id);
                     verif_comp.set_abstract_name("N0:C0");
                     `evl_log(UVM_DEBUG, VMOD_CONFIG, get_abstract_name(), $sformatf("(CYE DEBUG: evl_ioshire_env) STANDALONE_MINION_RTL_AGENT is found and assigned to m_standalone_minion_agent"))
                  end
               `endif

               else if (agent_id != ~evl_agent_id_t'(0)) begin
                  verif_comp.set_shire_id(get_shire_id());
                  verif_comp.set_agent_id(agent_id);
                  verif_comp.set_abstract_name($sformatf("%0s-%0d", verif_comp.get_root_abstract_name(), agent_id));
               end
               add_vc_child(verif_comp);
            end // else if (!(verif_comp.is_bus_vc() == 1'b1)) ------------------------------------}

         end // if (component != null) begin
      end // foreach (instance_list[ii]) begin

   endfunction : build_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Function: connect_phase
   //
   virtual function void connect_phase(uvm_phase phase);
      super.connect_phase(phase);
      foreach (m_axi_stim_agents[ii]) begin
         evl_ioshire_axi_ifc_type_t axi_type = evl_ioshire_axi_ifc_type_t'(ii);

         for (int jj = 0; jj < m_axi_bus_agents[ii].size(); jj++) begin
            if (m_axi_bus_agents[ii][jj] != null) begin
               if ( (m_noc_is_stub > 0) &&
                    ( (ii == int'(IO_TOL3_AXI))  ||
                      (ii == int'(IO_TOSYS_AXI)) ||
                      (ii == int'(IO_PM_AXI))    ||
                      (ii == int'(IO_MAIN_NOC_RBM)) ) ) begin
                  m_axi_stim_agents[ii].connect_my_bus_ports(m_axi_bus_agents[ii][jj], $sformatf("%p", axi_type), jj);
               end
               else begin
                  m_axi_stim_agents[ii].connect_my_bus_ports(m_axi_bus_agents[ii][jj], "", jj);
               end
            end
         end
      end
      foreach (m_etlink_bus_agents[ii]) begin
         if ((m_etlink_bus_agents[ii] != null) && m_etlink_stim_agents.exists(ii) && (m_etlink_stim_agents[ii] != null)) begin
            m_etlink_stim_agents[ii].connect_my_bus_ports(m_etlink_bus_agents[ii]);
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
      if (m_ioshire_stim == 1) begin
         //
         // There is a special case for an AXI mesh slave interface and its companion AXI mesh
         // master interface ... to handle the loopback case, we need to forward some AXI mesh slave
         // requests to the AXI mesh master agent.
         //
         if (m_axi_stim_agents.exists(int'(IO_PS_AXI)) && m_axi_stim_agents.exists(int'(IO_TOSYS_AXI))) begin
            for (int ii = 0; ii < 1; ii++) begin
               m_axi_stim_agents[int'(IO_TOSYS_AXI)].connect_port_to_my_export(m_axi_stim_agents[int'(IO_PS_AXI)].get_req_desc_port("m_loopback_req_port", ii), "m_loopback_req_export", ii);
            end
            for (int ii = 0; ii < 1; ii++) begin
               m_axi_stim_agents[int'(IO_PS_AXI)].connect_port_to_my_export(m_axi_stim_agents[int'(IO_TOSYS_AXI)].get_req_desc_port("m_loopback_rsp_port", ii), "m_loopback_rsp_export", ii);
            end
         end
      end
   endfunction : connect_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Function: start_of_simulation_phase
   //
   virtual function void start_of_simulation_phase(uvm_phase phase);
      super.start_of_simulation_phase(phase);
      if ((m_ioshire_stim == 1)) begin
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
      else if ((m_io_noc_only == 1)) begin
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
         if ((m_master_port_agent != null) && (m_axi_bus_agents.exists(int'(IO_PS_AXI)))) begin
            if (m_axi_bus_agents[int'(IO_PS_AXI)][0] != null) begin
               m_axi_bus_agents[int'(IO_PS_AXI)][0].set_master_port_agent(m_master_port_agent);
            end
         end
         foreach (m_dbgnoc_bus_agents[ii]) begin
            if (m_dbgnoc_stim_agents.exists(ii) && (m_dbgnoc_stim_agents[ii] != null)) begin
               m_dbgnoc_bus_agents[ii].set_master_port_agent(m_dbgnoc_stim_agents[ii]);
            end
         end
      end
      else begin
         if (m_axi_stim_agents.exists(int'(IO_PS_AXI)) && (m_axi_stim_agents[int'(IO_PS_AXI)] != null)) begin
            if (m_axi_bus_agents[int'(IO_PS_AXI)][0] != null) begin
               m_axi_bus_agents[int'(IO_PS_AXI)][0].set_master_port_agent(m_axi_stim_agents[int'(IO_PS_AXI)]);
            end
         end
         else if ((m_master_port_agent != null) && (m_axi_bus_agents.exists(int'(IO_PS_AXI)))) begin
            if (m_axi_bus_agents[int'(IO_PS_AXI)][0] != null) begin
               m_axi_bus_agents[int'(IO_PS_AXI)][0].set_master_port_agent(m_master_port_agent);
            end
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
   function new(input string name_in = "evl_ioshire_env", input uvm_component parent_in = null);
      super.new(name_in, parent_in, IO_SHIRE_RTL_AGENT);
      set_abstract_name("IO_SHIRE");
      add_agent_alias(IO_SHIRE_AGENT);
   endfunction : new

endclass : evl_ioshire_env

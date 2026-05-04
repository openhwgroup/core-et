//--------------------------------------------------------------------------------------------------
//
// Class: evl_ddr_env
//
class evl_ddr_env extends evl_verif_comp;

   `uvm_component_utils(evl_ddr_env)


   //-----------------------------------------------------------------------------------------------
   //
   // Static Member Variables and Objects
   //
   `evl_param_key(pk_use_apb_stub)
   `evl_param_key(pk_use_ddr_stub)


   //-----------------------------------------------------------------------------------------------
   //
   // Member Variables and Objects
   //
   evl_verif_comp m_shire_bus_agents[evl_agent_id_t];
   evl_verif_comp m_shire_stim_agents[evl_agent_id_t];


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_master_port_agent
   //
   virtual function void set_master_port_agent(input evl_verif_comp agent_in, input string port_name = "", input int port_id = 0);
      foreach (m_shire_bus_agents[ii]) begin
         m_shire_bus_agents[ii].set_master_port_agent(agent_in);
      end
   endfunction : set_master_port_agent


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_slave_port_agent
   //
   virtual function void set_slave_port_agent(input evl_verif_comp agent_in, input string port_name = "", input int port_id = 0);
      foreach (m_shire_bus_agents[ii]) begin
         m_shire_bus_agents[ii].set_slave_port_agent(agent_in);
      end
   endfunction : set_slave_port_agent


   //-----------------------------------------------------------------------------------------------
   //
   // Function: connect_my_port_to_export
   //
   virtual function void connect_my_port_to_export(input uvm_analysis_export#(evl_req_desc) export_in,
                                                   input string                             port_name      = "unknown",
                                                   input int                                port_number    = 0,
                                                   input int                                subport_number = 0);
      uvm_analysis_port #(evl_req_desc) tlm_port;

      if (export_in == null) begin
         return;
      end
      if ((port_name == "m_obs_req_port") && m_shire_bus_agents.exists(port_number)) begin
         tlm_port = m_shire_bus_agents[port_number].get_req_desc_port("m_obs_req_port");
         if (tlm_port != null) begin
            `evl_log(UVM_MEDIUM, VMOD_CONFIG, get_abstract_name(), $sformatf("connecting %0s -> %0s", tlm_port.get_full_name(), export_in.get_full_name()))
            tlm_port.connect(export_in);
         end
      end
      else if (port_name == "m_obs_rsp_port") begin
         tlm_port = m_shire_bus_agents[port_number].get_req_desc_port("m_obs_rsp_port");
         if (tlm_port != null) begin
            `evl_log(UVM_MEDIUM, VMOD_CONFIG, get_abstract_name(), $sformatf("connecting %0s -> %0s", tlm_port.get_full_name(), export_in.get_full_name()))
            tlm_port.connect(export_in);
         end
      end
      else if ((port_name == "m_obs_req_port_lo_even") && m_shire_bus_agents.exists(`EVL_DDR_LO_EVEN_PORT)) begin
         tlm_port = m_shire_bus_agents[`EVL_DDR_LO_EVEN_PORT].get_req_desc_port("m_obs_req_port");
         if (tlm_port != null) begin
            `evl_log(UVM_MEDIUM, VMOD_CONFIG, get_abstract_name(), $sformatf("connecting %0s -> %0s", tlm_port.get_full_name(), export_in.get_full_name()))
            tlm_port.connect(export_in);
         end
      end
      else if ((port_name == "m_obs_req_port_lo_odd") && m_shire_bus_agents.exists(`EVL_DDR_LO_ODD_PORT)) begin
         tlm_port = m_shire_bus_agents[`EVL_DDR_LO_ODD_PORT].get_req_desc_port("m_obs_req_port");
         if (tlm_port != null) begin
            `evl_log(UVM_MEDIUM, VMOD_CONFIG, get_abstract_name(), $sformatf("connecting %0s -> %0s", tlm_port.get_full_name(), export_in.get_full_name()))
            tlm_port.connect(export_in);
         end
      end
      else if ((port_name == "m_obs_req_port_hi_even") && m_shire_bus_agents.exists(`EVL_DDR_HI_EVEN_PORT)) begin
         tlm_port = m_shire_bus_agents[`EVL_DDR_HI_EVEN_PORT].get_req_desc_port("m_obs_req_port");
         if (tlm_port != null) begin
            `evl_log(UVM_MEDIUM, VMOD_CONFIG, get_abstract_name(), $sformatf("connecting %0s -> %0s", tlm_port.get_full_name(), export_in.get_full_name()))
            tlm_port.connect(export_in);
         end
      end
      else if ((port_name == "m_obs_req_port_hi_odd") && m_shire_bus_agents.exists(`EVL_DDR_HI_ODD_PORT)) begin
         tlm_port = m_shire_bus_agents[`EVL_DDR_HI_ODD_PORT].get_req_desc_port("m_obs_req_port");
         if (tlm_port != null) begin
            `evl_log(UVM_MEDIUM, VMOD_CONFIG, get_abstract_name(), $sformatf("connecting %0s -> %0s", tlm_port.get_full_name(), export_in.get_full_name()))
            tlm_port.connect(export_in);
         end
      end
      else if ((port_name == "m_obs_rsp_port_lo_even") && m_shire_bus_agents.exists(`EVL_DDR_LO_EVEN_PORT)) begin
         tlm_port = m_shire_bus_agents[`EVL_DDR_LO_EVEN_PORT].get_req_desc_port("m_obs_rsp_port");
         if (tlm_port != null) begin
            `evl_log(UVM_MEDIUM, VMOD_CONFIG, get_abstract_name(), $sformatf("connecting %0s -> %0s", tlm_port.get_full_name(), export_in.get_full_name()))
            tlm_port.connect(export_in);
         end
      end
      else if ((port_name == "m_obs_rsp_port_lo_odd") && m_shire_bus_agents.exists(`EVL_DDR_LO_ODD_PORT)) begin
         tlm_port = m_shire_bus_agents[`EVL_DDR_LO_ODD_PORT].get_req_desc_port("m_obs_rsp_port");
         if (tlm_port != null) begin
            `evl_log(UVM_MEDIUM, VMOD_CONFIG, get_abstract_name(), $sformatf("connecting %0s -> %0s", tlm_port.get_full_name(), export_in.get_full_name()))
            tlm_port.connect(export_in);
         end
      end
      else if ((port_name == "m_obs_rsp_port_hi_even") && m_shire_bus_agents.exists(`EVL_DDR_HI_EVEN_PORT)) begin
         tlm_port = m_shire_bus_agents[`EVL_DDR_HI_EVEN_PORT].get_req_desc_port("m_obs_rsp_port");
         if (tlm_port != null) begin
            `evl_log(UVM_MEDIUM, VMOD_CONFIG, get_abstract_name(), $sformatf("connecting %0s -> %0s", tlm_port.get_full_name(), export_in.get_full_name()))
            tlm_port.connect(export_in);
         end
      end
      else if ((port_name == "m_obs_rsp_port_hi_odd") && m_shire_bus_agents.exists(`EVL_DDR_HI_ODD_PORT)) begin
         tlm_port = m_shire_bus_agents[`EVL_DDR_HI_ODD_PORT].get_req_desc_port("m_obs_rsp_port");
         if (tlm_port != null) begin
            `evl_log(UVM_MEDIUM, VMOD_CONFIG, get_abstract_name(), $sformatf("connecting %0s -> %0s", tlm_port.get_full_name(), export_in.get_full_name()))
            tlm_port.connect(export_in);
         end
      end
   endfunction : connect_my_port_to_export


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
      evl_verif_comp   ddr_slave_agent_0;
      evl_verif_comp   ddr_slave_agent_1;
      int              gen_apb_slave_agent;
      int              gen_ddr_slave_agent;

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
            gen_apb_slave_agent = -1;
            gen_ddr_slave_agent = -1;
            if (verif_comp.is_bus_vc() == 1'b1) begin
               if (verif_comp.get_agent_type() == AXI_BUS_AGENT) begin
                  verif_comp.set_shire_id(evl_agent_id_t'(get_shire_id()));
                  if (instance_list[ii].get_inst_name() == "m_mem_to_ddr0_lo_axi") begin
                     verif_comp.set_port_id(int'(MEM_LO_P0_ID));
                     verif_comp.set_port_type(int'(MEM_LO_P0_ID));
                     verif_comp.set_agent_id(evl_agent_id_t'(4));
                     verif_comp.set_abstract_name($sformatf("MEM_DDR_LO-0(AXI)"));
                     m_shire_bus_agents[int'(MEM_LO_P0_ID)] = verif_comp;
                     if (get_int_param(pk_use_ddr_stub, 0) > 0) begin
                        gen_ddr_slave_agent = int'(MEM_LO_P0_ID);
                     end
                  end
                  else if (instance_list[ii].get_inst_name() == "m_mem_to_ddr1_lo_axi") begin
                     verif_comp.set_port_id(int'(MEM_LO_P1_ID));
                     verif_comp.set_port_type(int'(MEM_LO_P1_ID));
                     verif_comp.set_agent_id(evl_agent_id_t'(5));
                     verif_comp.set_abstract_name($sformatf("MEM_DDR_LO-1(AXI)"));
                     m_shire_bus_agents[int'(MEM_LO_P1_ID)] = verif_comp;
                     if (get_int_param(pk_use_ddr_stub, 0) > 0) begin
                        gen_ddr_slave_agent = int'(MEM_LO_P1_ID);
                     end
                  end
                  else if (instance_list[ii].get_inst_name() == "m_mem_to_ddr0_hi_axi") begin
                     verif_comp.set_port_id(int'(MEM_HI_P0_ID));
                     verif_comp.set_port_type(int'(MEM_HI_P0_ID));
                     verif_comp.set_agent_id(evl_agent_id_t'(6));
                     verif_comp.set_abstract_name($sformatf("MEM_DDR_HI-0(AXI)"));
                     m_shire_bus_agents[int'(MEM_HI_P0_ID)] = verif_comp;
                     if (get_int_param(pk_use_ddr_stub, 0) > 0) begin
                        gen_ddr_slave_agent = int'(MEM_HI_P0_ID);
                     end
                  end
                  else if (instance_list[ii].get_inst_name() == "m_mem_to_ddr1_hi_axi") begin
                     verif_comp.set_port_id(int'(MEM_HI_P1_ID));
                     verif_comp.set_port_type(int'(MEM_HI_P1_ID));
                     verif_comp.set_agent_id(evl_agent_id_t'(7));
                     verif_comp.set_abstract_name($sformatf("MEM_DDR_HI-1(AXI)"));
                     m_shire_bus_agents[int'(MEM_HI_P1_ID)] = verif_comp;
                     if (get_int_param(pk_use_ddr_stub, 0) > 0) begin
                        gen_ddr_slave_agent = int'(MEM_HI_P1_ID);
                     end
                  end
               end
               else if (verif_comp.get_agent_type() == APB_BUS_AGENT) begin
                  verif_comp.set_shire_id(get_shire_id());
                  verif_comp.set_port_id(0);
                  verif_comp.set_agent_id(evl_agent_id_t'(1));
                  verif_comp.set_abstract_name($sformatf("DDR_ESR(APB)"));
                  m_shire_bus_agents[int'(DDR_APB_ID)] = verif_comp;
                  if (get_int_param(pk_use_apb_stub, 0) > 0) begin
                     gen_apb_slave_agent = int'(DDR_APB_ID);
                  end
               end

               add_vc_child(verif_comp);

               //
               // Create stimulus agents as necessary.
               //
               if (gen_ddr_slave_agent >= 0) begin
                  m_shire_bus_agents[gen_ddr_slave_agent].set_active_slave();
                  if (evl_memshire_port_t'(gen_ddr_slave_agent) inside { MEM_LO_P0_ID, MEM_HI_P0_ID }) begin
                     if (ddr_slave_agent_0 == null) begin
                        component = factory.create_component_by_name("evl_ddr_slave_agent_axi", "", $sformatf("ddr_slave_0"), this);
                        $cast(ddr_slave_agent_0, component);
                     end
                     else begin
                        component = ddr_slave_agent_0;
                     end
                  end
                  else if (evl_memshire_port_t'(gen_ddr_slave_agent) inside { MEM_LO_P1_ID, MEM_HI_P1_ID }) begin
                     if (ddr_slave_agent_1 == null) begin
                        component = factory.create_component_by_name("evl_ddr_slave_agent_axi", "", $sformatf("ddr_slave_1"), this);
                        $cast(ddr_slave_agent_1, component);
                     end
                     else begin
                        component = ddr_slave_agent_1;
                     end
                  end
               end
               else if (gen_apb_slave_agent >= 0) begin
                  m_shire_bus_agents[gen_apb_slave_agent].set_active_slave();
                  component = factory.create_component_by_name("evl_reg_slave_agent_apb", "", $sformatf("apb_slave"), this);
               end
               else begin
                  component = null;
               end
               if (component != null) begin
                  $cast(verif_comp, component);
                  verif_comp.set_shire_id(get_shire_id());
                  if (gen_ddr_slave_agent >= 0) begin
                     if (evl_memshire_port_t'(gen_ddr_slave_agent) inside { MEM_LO_P0_ID, MEM_HI_P0_ID }) begin
                        verif_comp.set_port_id(0);
                        verif_comp.set_agent_id(evl_agent_id_t'(0));
                        verif_comp.set_abstract_name($sformatf("MEM_DDR-0"));
                     end
                     else begin
                        verif_comp.set_port_id(1);
                        verif_comp.set_agent_id(evl_agent_id_t'(1));
                        verif_comp.set_abstract_name($sformatf("MEM_DDR-1"));
                     end
                     if (int'(gen_ddr_slave_agent) inside { MEM_LO_P0_ID, MEM_LO_P1_ID }) begin
                        verif_comp.set_rtl_port(m_shire_bus_agents[gen_ddr_slave_agent].get_rtl_port(), 0);
                     end
                     else begin
                        verif_comp.set_rtl_port(m_shire_bus_agents[gen_ddr_slave_agent].get_rtl_port(), 1);
                     end
                     m_shire_stim_agents[gen_ddr_slave_agent] = verif_comp;
                  end
                  else if (gen_apb_slave_agent >= 0) begin
                     verif_comp.set_agent_id(~evl_agent_id_t'(0));
                     verif_comp.set_port_id(0);
                     verif_comp.set_abstract_name($sformatf("DDR_ESR"));
                     verif_comp.set_rtl_port(m_shire_bus_agents[gen_apb_slave_agent].get_rtl_port(), 0);
                     m_shire_stim_agents[gen_apb_slave_agent] = verif_comp;
                  end
                  add_vc_child(verif_comp);
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
      foreach (m_shire_stim_agents[ii]) begin
         uvm_analysis_port #(evl_req_desc) tlm_port;

         if (ii inside { MEM_LO_P0_ID, MEM_HI_P0_ID }) begin
            m_shire_stim_agents[ii].connect_my_bus_ports(m_shire_bus_agents[ii], "", ii - int'(MEM_LO_P0_ID));
         end
         else if (ii inside { MEM_LO_P1_ID, MEM_HI_P1_ID }) begin
            m_shire_stim_agents[ii].connect_my_bus_ports(m_shire_bus_agents[ii], "", ii - int'(MEM_LO_P1_ID));
         end
         else begin
            m_shire_stim_agents[ii].connect_my_bus_ports(m_shire_bus_agents[ii]);
         end
      end
   endfunction : connect_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Function: start_of_simulation_phase
   //
   virtual function void start_of_simulation_phase(uvm_phase phase);
      super.start_of_simulation_phase(phase);
      foreach (m_shire_stim_agents[ii]) begin
         if (m_shire_stim_agents.exists(ii) && (m_shire_stim_agents[ii] != null)) begin
            m_shire_bus_agents[ii].set_slave_port_agent(m_shire_stim_agents[ii]);
            if (ii inside { MEM_LO_P0_ID }) begin
               m_shire_stim_agents[ii].add_addr_template(evl_paddr_t'(3) << (`EVL_PADDR_MSB - 1), evl_paddr_t'(0), 1'b1, 1'b1);
               m_shire_stim_agents[ii].add_addr_template(evl_paddr_t'(15) << `EVL_LINE_ADDR_LSB, evl_paddr_t'(get_shire_id() & 7) << `EVL_LINE_ADDR_LSB, 1'b0);
            end
            else if (ii inside { MEM_LO_P1_ID }) begin
               m_shire_stim_agents[ii].add_addr_template(evl_paddr_t'(3) << (`EVL_PADDR_MSB - 1), evl_paddr_t'(0), 1'b1, 1'b1);
               m_shire_stim_agents[ii].add_addr_template(evl_paddr_t'(15) << `EVL_LINE_ADDR_LSB, evl_paddr_t'(8 | (get_shire_id() & 7)) << `EVL_LINE_ADDR_LSB, 1'b0);
            end
         end
      end
   endfunction : start_of_simulation_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string name_in = "evl_ddr_env", input uvm_component parent_in = null);
      super.new(name_in, parent_in, DDR_SHIRE_RTL_AGENT);
      set_abstract_name("DDR_SHIRE");
   endfunction : new

endclass : evl_ddr_env

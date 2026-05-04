//--------------------------------------------------------------------------------------------------
//
// Class: evl_shire_cache_env
//
class evl_shire_cache_env extends evl_verif_comp;

   `uvm_component_utils(evl_shire_cache_env)


   //-----------------------------------------------------------------------------------------------
   //
   // Static Member Variables and Objects
   //
   // pk_upstream_req_port_count: This is the integer parameter key to access the number of ET
   //                             link ports.
   //                             ------------------------------------------------------------------
   // pk_axi_port_mask:           This is the integer parameter key to access the bit mask of AXI
   //                             ports.
   //
   `evl_param_key(pk_upstream_req_port_count)
   `evl_param_key(pk_axi_port_mask)
   `evl_param_key(pk_sc_scoreboard_type)
   `evl_param_key(pk_mesh_l3_port_num)
   `evl_param_key(pk_mesh_l3_axi_port)
   `evl_param_key(pk_num_l3_ports)
   `evl_param_key(pk_num_l3_master_ports)
   `evl_param_key(pk_num_l3_slave_ports)


   //-----------------------------------------------------------------------------------------------
   //
   // Member Variables and Objects
   //
   // m_shire_cache_rtl_port:        This is a handle to the RTL port attached to the shire cache.
   //                                This handle is identical to m_rtl_port, but it has been cast to
   //                                the correct class type.
   //                                ---------------------------------------------------------------
   // m_l2_scoreboard:               This is a handle to the L2 scoreboard.
   //                                ---------------------------------------------------------------
   // m_scp_scoreboard:              This is a handle to the scratchpad RAM scoreboard.
   //                                ---------------------------------------------------------------
   // m_l3_scoreboard:               This is a handle to the L3 scoreboard.
   //                                ---------------------------------------------------------------
   // m_active_l3_mesh_req_descs[]:  x
   //                                ---------------------------------------------------------------
   // m_active_sys_mesh_req_descs[]: x
   //                                ---------------------------------------------------------------
   // m_upstream_req_export[]:       This is a dynamic array of TLM ports used to connect this
   //                                component to the observed request ports of ET link bus agents.
   //                                It is accessed by ET link port number (0 for neighborhood 0, 1
   //                                for neighborhood 1, etc.).
   //                                ---------------------------------------------------------------
   // m_upstream_req_fifo[]:         This is a dynamic array of TLM FIFOs used to field incoming
   //                                observed ET link bus requests.
   //                                ---------------------------------------------------------------
   // m_axi_req_export[]:            This is a dynamic array of TLM ports used to connect this
   //                                component to the observed request ports AXI bus agents.
   //                                ---------------------------------------------------------------
   // m_axi_req_fifo[]:              This is a dynamic array of TLM FIFOs used to field incoming
   //                                observed AXI bus requests.
   //                                ---------------------------------------------------------------
   // m_axi_rsp_export[]:            This is a dynamic array of TLM ports used to connect this
   //                                component to the observed response ports AXI bus agents.
   //                                ---------------------------------------------------------------
   // m_axi_rsp_fifo[]:              This is a dynamic array of TLM FIFOs used to field incoming
   //                                obsrved AXI bus responses.
   //                                ---------------------------------------------------------------
   // m_reg_bus_agents[]:            This is an associative array of bus agents for the register
   //                                interfaces
   //                                ---------------------------------------------------------------
   // m_reg_stim_agents[]:           This is an associative array of stim agents for the register
   //                                bus agents
   //
   int                                   m_shutdown_objection  = 0;
   int                                   m_num_l3_ports        = 1;
   int                                   m_num_l3_master_ports = 1;
   int                                   m_num_l3_slave_ports  = 1;
   bit                                   m_disable_sc_sb       = 1'b0;
   bit                                   m_disable_sc_sb_esr   = 1'b0;
   int                                   m_axi_feature_select  = 0;
   evl_shire_cache_rtl_port              m_shire_cache_rtl_port;
   evl_shire_cache_reqq                  m_shire_cache_reqq;
   evl_shire_cache_axi_meshq             m_shire_cache_axi_meshq;
   evl_l2_scoreboard                     m_l2_scoreboard;
   evl_scp_scoreboard                    m_scp_scoreboard;
   evl_l3_scoreboard                     m_l3_scoreboard;
   evl_req_desc                          m_active_l3_mesh_req_descs[$];
   evl_req_desc                          m_active_sys_mesh_req_descs[$];
   uvm_analysis_export   #(evl_req_desc) m_upstream_req_export[];
   uvm_tlm_analysis_fifo #(evl_req_desc) m_upstream_req_fifo[];
   uvm_analysis_export   #(evl_req_desc) m_axi_req_export[int];
   uvm_tlm_analysis_fifo #(evl_req_desc) m_axi_req_fifo[int];
   uvm_analysis_export   #(evl_req_desc) m_axi_rsp_export[int];
   uvm_tlm_analysis_fifo #(evl_req_desc) m_axi_rsp_fifo[int];
   evl_verif_comp                        m_reg_bus_agents[evl_agent_id_t];
// evl_verif_comp                        m_reg_stim_agents[evl_agent_id_t];
   evl_reg_adapter                       m_reg_adapter;
   uvm_reg_predictor#(evl_req_desc)      m_reg_predictor[evl_agent_id_t];
   evl_shire_cache_reg_block             m_shire_cache_regs[evl_agent_id_t];
   mailbox #(evl_req_desc)               m_l3_mesh_rsp_queue;
   mailbox #(evl_req_desc)               m_mesh_l3_req_queue;


   //-----------------------------------------------------------------------------------------------
   //
   // Function: init_progeny
   //
   // This virtual function is intended to be redefined as necessary to initialize any child
   // components after setting the shire ID, abstract name, etc.
   //
   // Input Variables:
   //
   // type_in: This integer is made available to pass information in as necessary.
   //
   virtual function void init_progeny(input int type_in = 0);
      if (m_shire_cache_reqq != null) begin
         m_shire_cache_reqq.set_abstract_name($sformatf("%0s(REQQ)", get_root_abstract_name()));
      end
      if (m_shire_cache_axi_meshq != null) begin
         m_shire_cache_axi_meshq.set_abstract_name($sformatf("%0s(MESHQ)", get_root_abstract_name()));
      end
      if (m_l2_scoreboard != null) begin
         m_l2_scoreboard.set_abstract_name($sformatf("%0s(L2SB)", get_root_abstract_name()));
      end
      if (m_scp_scoreboard != null) begin
         m_scp_scoreboard.set_abstract_name($sformatf("%0s(ScpSB)", get_root_abstract_name()));
      end
      if (m_l3_scoreboard != null) begin
         m_l3_scoreboard.set_abstract_name($sformatf("%0s(L3SB)", get_root_abstract_name()));
      end
   endfunction : init_progeny


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_master_port_agent
   //
   virtual function evl_verif_comp get_master_port_agent(input string port_name = "", input int port_id = 0, input int subport_id = 0);
      if (m_disable_sc_sb == 1'b1) begin
         return null;
      end
      return this;
   endfunction : get_master_port_agent


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_slave_port_agent
   //
   virtual function evl_verif_comp get_slave_port_agent(input string port_name = "", input int port_id = 0, input int subport_id = 0);
      if (m_disable_sc_sb == 1'b1) begin
         return null;
      end
      return this;
   endfunction : get_slave_port_agent


   //-----------------------------------------------------------------------------------------------
   //
   // Function: connect_port_to_my_export
   //
   virtual function void connect_port_to_my_export(input uvm_analysis_port#(evl_req_desc) port_in,
                                                   input string                           port_name      = "unknown",
                                                   input int                              port_number    = 0,
                                                   input int                              subport_number = 0);
      int port_index;

      if (port_in == null) begin
         return;
      end
      if ((port_name == "m_upstream_req_export") || (port_name == "upstream_req_export")) begin
         if ((port_number >= 0) && (port_number < m_upstream_req_export.size()) && (m_upstream_req_export[port_number] != null)) begin
            `evl_log(UVM_MEDIUM, VMOD_CONFIG, get_abstract_name(), $sformatf("connecting %0s -> %0s", port_in.get_full_name(), m_upstream_req_export[port_number].get_full_name()))
            port_in.connect(m_upstream_req_export[port_number]);
         end
      end
      else if ((port_name == "m_axi_req_export") || (port_name == "axi_req_export")) begin
         port_index = m_global_attrs.get_shire_axi_port_index(port_number, subport_number, m_num_l3_ports, m_axi_feature_select);
         if (m_axi_req_export.exists(port_index) && (m_axi_req_export[port_index] != null)) begin
            `evl_log(UVM_MEDIUM, VMOD_CONFIG, get_abstract_name(), $sformatf("connecting %0s -> %0s", port_in.get_full_name(), m_axi_req_export[port_index].get_full_name()))
            port_in.connect(m_axi_req_export[port_index]);
         end
      end
      else if ((port_name == "m_axi_rsp_export") || (port_name == "axi_rsp_export")) begin
         port_index = m_global_attrs.get_shire_axi_port_index(port_number, subport_number, m_num_l3_ports, m_axi_feature_select);
         if (m_axi_rsp_export.exists(port_index) && (m_axi_rsp_export[port_index] != null)) begin
            `evl_log(UVM_MEDIUM, VMOD_CONFIG, get_abstract_name(), $sformatf("connecting %0s -> %0s", port_in.get_full_name(), m_axi_rsp_export[port_index].get_full_name()))
            port_in.connect(m_axi_rsp_export[port_index]);
         end
      end
   endfunction : connect_port_to_my_export


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
      if (m_shire_cache_axi_meshq == null) begin
         return paired_req_desc;
      end
      return m_shire_cache_axi_meshq.find_observed_request(port_type_in, port_in, subport_in, paired_req_desc, bus_cmd_in, address_in, req_id_in, source_id_in, trans_id_in);
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
      evl_req_desc l2_req_desc;
      if ((m_l2_scoreboard == null) && (m_scp_scoreboard == null)) begin
         return paired_req_desc;
      end
      if (m_l2_scoreboard == null) begin
         l2_req_desc = null;
      end
      else begin
         l2_req_desc = m_l2_scoreboard.find_observed_response(port_in, paired_req_desc, compare_fields, qualifier, rsp_type_in, req_id_in, dest_id_in, rsp_byte_en_in, rsp_data_in);
      end
      if ((m_scp_scoreboard != null) && (l2_req_desc == null)) begin
         return m_scp_scoreboard.find_observed_response(port_in, paired_req_desc, compare_fields, qualifier, rsp_type_in, req_id_in, dest_id_in, rsp_byte_en_in, rsp_data_in);
      end
      else begin
         return l2_req_desc;
      end
   endfunction : find_observed_response


   //-----------------------------------------------------------------------------------------------
   //
   // Function: build_phase
   //
   virtual function void build_phase(uvm_phase phase);
      uvm_factory               factory;
      uvm_component             component;
      evl_verif_comp            verif_comp;
      evl_rtl_instance          instance_list[];
      evl_verif_comp            parent_comp;
      evl_verif_comp            parent_comps[string];
      evl_shire_cache_reg_model reg_model;

      super.build_phase(phase);

      if (m_rtl_port == null) begin
         `dut_error(get_abstract_name(), "shire_cache_scoreboard: m_rtl_port is null, SAD!")
      end

      $cast(m_shire_cache_rtl_port, m_rtl_port);

      m_num_l3_ports        = get_int_param(pk_num_l3_ports, 1);
      m_num_l3_master_ports = get_int_param(pk_num_l3_master_ports, 1);
      m_num_l3_slave_ports  = get_int_param(pk_num_l3_slave_ports, 1);

      if (get_shire_id() >= 128) begin
         m_axi_feature_select = get_shire_id();
      end

      if (m_disable_sc_sb == 1'b1) begin
         return;
      end

      if (get_int_param(pk_upstream_req_port_count) > 0) begin
         m_upstream_req_export = new[get_int_param(pk_upstream_req_port_count)];
         m_upstream_req_fifo   = new[get_int_param(pk_upstream_req_port_count)];
      end
      foreach (m_upstream_req_export[ii]) begin
         m_upstream_req_export[ii] = new($sformatf("m_upstream_req_export[%0d]", ii), this);
         m_upstream_req_fifo[ii]   = new($sformatf("m_upstream_req_fifo[%0d]", ii), this);
      end

      if (get_int_param(pk_axi_port_mask, 0) > 0) begin
         int axi_port_mask;
         int axi_port_index;

         axi_port_index = 0;
         axi_port_mask  = get_int_param(pk_axi_port_mask, 0);
         while (axi_port_mask != 0) begin
            evl_shire_axi_ifc_type_t port_type;

            port_type = evl_shire_axi_ifc_type_t'(axi_port_index);
            if ((axi_port_mask[0:0] == 1'b1) && (axi_port_index inside { int'(SHIRE_L2TOL3_MESH_AXI), int'(SHIRE_L3TOSYS_MESH_AXI), int'(SHIRE_MESH_TOL3_AXI) })) begin
               for (int ii = 0; ii < m_num_l3_ports; ii++) begin
                  int port_index;

                  port_index = m_global_attrs.get_shire_axi_port_index(axi_port_index, ii, m_num_l3_ports, m_axi_feature_select);
                  if (port_index < 0) begin
                     break;
                  end
                  if (m_axi_req_export.exists(port_index)) begin
                     `dut_error(get_abstract_name(), $sformatf("cannot create m_axi_req_export[%0d] ... axi_port_index:%0d, ii:%0d, ports:%0d (type %0p)", port_index, axi_port_index, ii, m_num_l3_ports, port_type))
                  end
                  else begin
                     m_axi_req_export[port_index] = new($sformatf("m_axi_req_export[%0d]", port_index), this);
                     m_axi_req_fifo[port_index]   = new($sformatf("m_axi_req_fifo[%0d]", port_index), this);
                  end
               end
            end
            axi_port_index++;
            axi_port_mask = axi_port_mask >> 1;
         end

         axi_port_index = 0;
         axi_port_mask  = get_int_param(pk_axi_port_mask, 0);
         while (axi_port_mask != 0) begin
            evl_shire_axi_ifc_type_t port_type;

            port_type = evl_shire_axi_ifc_type_t'(axi_port_index);
            if ((axi_port_mask[0:0] == 1'b1) && (axi_port_index inside { int'(SHIRE_L2TOL3_MESH_AXI), int'(SHIRE_L3TOSYS_MESH_AXI) })) begin
               for (int ii = 0; ii < m_num_l3_ports; ii++) begin
                  int port_index;

                  port_index = m_global_attrs.get_shire_axi_port_index(axi_port_index, ii, m_num_l3_ports, m_axi_feature_select);
                  if (port_index < 0) begin
                     break;
                  end
                  if (m_axi_rsp_export.exists(port_index)) begin
                     `dut_error(get_abstract_name(), $sformatf("cannot create m_axi_rsp_export[%0d] ... axi_port_index:%0d, ii:%0d, ports:%0d (type %0p)", port_index, axi_port_index, ii, m_num_l3_ports, port_type))
                  end
                  else begin
                     m_axi_rsp_export[port_index] = new($sformatf("m_axi_rsp_export[%0d]", port_index), this);
                     m_axi_rsp_fifo[port_index]   = new($sformatf("m_axi_rsp_fifo[%0d]", port_index), this);
                  end
               end
            end
            axi_port_index++;
            axi_port_mask = axi_port_mask >> 1;
         end
      end

      m_l3_mesh_rsp_queue = new();
      m_mesh_l3_req_queue = new();

      m_shire_cache_reqq      = new();
      m_shire_cache_axi_meshq = new();

      m_shire_cache_reqq.set_int_param(pk_num_l3_ports, m_num_l3_ports);
      m_shire_cache_reqq.set_shire_id(get_shire_id());

      m_shire_cache_axi_meshq.set_int_param(pk_num_l3_ports, m_num_l3_ports);
      m_shire_cache_axi_meshq.set_shire_id(get_shire_id());

      m_l2_scoreboard = evl_l2_scoreboard::type_id::create("m_l2_scoreboard", this);
      m_l2_scoreboard.set_agent(this);
      m_l2_scoreboard.set_rtl_port(m_rtl_port);
      m_l2_scoreboard.set_shire_id(get_shire_id());
      m_l2_scoreboard.set_shire_cache_reqq(m_shire_cache_reqq);
      m_l2_scoreboard.set_shire_cache_axi_meshq(m_shire_cache_axi_meshq);
      m_l2_scoreboard.set_int_param(pk_num_l3_ports, m_num_l3_ports);

      m_scp_scoreboard = evl_scp_scoreboard::type_id::create("m_scp_scoreboard", this);
      m_scp_scoreboard.set_agent(this);
      m_scp_scoreboard.set_rtl_port(m_rtl_port);
      m_scp_scoreboard.set_shire_id(get_shire_id());
      m_scp_scoreboard.set_shire_cache_reqq(m_shire_cache_reqq);
      m_scp_scoreboard.set_shire_cache_axi_meshq(m_shire_cache_axi_meshq);
      m_scp_scoreboard.set_int_param(pk_num_l3_ports, m_num_l3_ports);

      m_l3_scoreboard = evl_l3_scoreboard::type_id::create("m_l3_scoreboard", this);
      m_l3_scoreboard.set_agent(this);
      m_l3_scoreboard.set_rtl_port(m_rtl_port);
      m_l3_scoreboard.set_shire_id(get_shire_id());
      m_l3_scoreboard.set_shire_cache_reqq(m_shire_cache_reqq);
      m_l3_scoreboard.set_shire_cache_axi_meshq(m_shire_cache_axi_meshq);
      m_l3_scoreboard.set_int_param(pk_num_l3_ports, m_num_l3_ports);

      init_progeny();


      // set up the register agent
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
               if (verif_comp.get_agent_type() == APB_BUS_AGENT) begin
                  verif_comp.set_agent_id(agent_id);
                  verif_comp.set_port_id(agent_id);
                  verif_comp.set_abstract_name($sformatf("SC_ESR-%0d(APB)", agent_id));
                  m_reg_bus_agents[agent_id] = verif_comp;
               end

               add_vc_child(verif_comp);

               //
               // create a register agent
               //
//             if (verif_comp.get_agent_type() == APB_BUS_AGENT) begin
//                component = factory.create_component_by_name("evl_reg_slave_agent_apb", "", $sformatf("reg_agent_%0d", agent_id), this);
//                if (component != null) begin
//                   verif_comp.set_passive_slave();
//                   $cast(verif_comp, component);
//                   m_reg_stim_agents[agent_id] = verif_comp;
//                   verif_comp.set_agent_id(agent_id);
//                   verif_comp.set_abstract_name($sformatf("SC_ESR-%0d", agent_id));
//                   verif_comp.set_shire_id(get_shire_id());
//                   verif_comp.set_rtl_port(m_reg_bus_agents[agent_id].get_rtl_port());
//                   add_vc_child(verif_comp);
//                end
//             end
            end
         end
      end

      m_reg_adapter = evl_reg_adapter::type_id::create("m_reg_adapter", this);
      m_reg_adapter.set_abstract_name($sformatf("%0s(RegAdapt)", get_root_abstract_name()));

      //
      // Create a root register model.
      //
      reg_model = new($sformatf("shire_cache"));
      reg_model.set_agent(this);
      reg_model.set_abstract_name($sformatf("%0s(RegModel)", get_root_abstract_name()));
      set_evl_reg_block(reg_model);

      foreach (m_reg_bus_agents[ii]) begin
         m_reg_predictor[ii] = new($sformatf("m_reg_predictor_bank_%0d", ii), this);
         m_shire_cache_regs[ii] = new($sformatf("bank_%0d", ii));
         m_shire_cache_regs[ii].set_agent(this);
         m_shire_cache_regs[ii].set_agent_id(ii);
         reg_model.add_evl_reg_block(ii, m_shire_cache_regs[ii], $sformatf("banks[%0d].bank.esr", ii));
      end
      reg_model.build(`EVL_SHIRE_CACHE_BASE_ESR & evl_paddr_t'(40'h00003fffff));
   endfunction : build_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Function: connect_phase
   //
   virtual function void connect_phase(uvm_phase phase);
      super.connect_phase(phase);
      foreach (m_upstream_req_export[ii]) begin
         m_upstream_req_export[ii].connect(m_upstream_req_fifo[ii].analysis_export);
      end
      foreach (m_axi_req_export[ii]) begin
         m_axi_req_export[ii].connect(m_axi_req_fifo[ii].analysis_export);
      end
      foreach (m_axi_rsp_export[ii]) begin
         m_axi_rsp_export[ii].connect(m_axi_rsp_fifo[ii].analysis_export);
      end
      foreach (m_reg_bus_agents[ii]) begin
         m_reg_bus_agents[ii].connect_reg_predictor_to_monitor(m_reg_predictor[ii]);
      end
//    foreach (m_reg_stim_agents[ii]) begin
//       m_reg_stim_agents[ii].connect_my_bus_ports(m_reg_bus_agents[ii]);
//       m_reg_stim_agents[ii].connect_reg_predictor_to_monitor(m_reg_predictor[ii]);
//    end

      foreach (m_shire_cache_regs[ii]) begin
         m_reg_predictor[ii].map     = m_shire_cache_regs[ii].get_map_by_name("APB_ESR");
         m_reg_predictor[ii].adapter = m_reg_adapter;
         m_reg_predictor[ii].map.set_auto_predict(0);
         if (m_disable_sc_sb_esr == 1'b0) begin
            m_reg_predictor[ii].map.set_check_on_read(1);
         end
         if (m_reg_bus_agents[ii].get_slave_sequencer() != null) begin
            m_reg_predictor[ii].map.set_sequencer(m_reg_bus_agents[ii].get_slave_sequencer(), m_reg_adapter);
         end

         //
         // There is a special case for the APB register maps.  Registers off of register APB
         // interfaces are not system-wide unique addresses (the register addresses are only unique
         // per APB bus).  If you look at the UVM code, there is really no particularly convenient
         // way to have hierarchical register blocks while at the same time having a per-subblock
         // address map (available only to that subblock) and a system address map (available to
         // everyone).
         //
         // What happens in the UVM code is that a register map within a register block will only
         // get initialized if there is no parent register block.  So even though there is no
         // parent register map for the bank's APB map, it will not get initialized unless we do
         // it outside of the normal processes.
         //
         m_shire_cache_regs[ii].lock_model();
         m_reg_predictor[ii].map.Xinit_address_mapX();
      end
      if (m_l2_scoreboard != null) begin
         m_l2_scoreboard.set_shire_cache_reg_block(m_shire_cache_regs);
      end
      if (m_l3_scoreboard != null) begin
         m_l3_scoreboard.set_shire_cache_reg_block(m_shire_cache_regs);
      end
      if (m_scp_scoreboard != null) begin
         m_scp_scoreboard.set_shire_cache_reg_block(m_shire_cache_regs);
      end
   endfunction : connect_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Task: shutdown_phase
   //
   virtual task shutdown_phase(uvm_phase phase);
      fork
         begin
            super.shutdown_phase(phase);
         end
         if (m_disable_sc_sb == 1'b0) begin
            begin :reqq_wait_block
               if (m_global_attrs.get_error_count() <= 0) begin
                  int active_count;
                  bit start_timeout_count;
                  bit disable_inner_loop;;

                  phase.raise_objection(this);
                  m_shutdown_objection++;
                  start_timeout_count = 1'b0;
                  disable_inner_loop  = 1'b0;
                  fork
                     forever begin :reqq_wait_loop
                        if (m_shire_cache_reqq.reqq_is_empty() == 1'b0) begin
                           start_timeout_count = 1'b1;
                           disable_inner_loop  = 1'b0;
                           m_shire_cache_reqq.wait_for_reqq_empty();
                           disable_inner_loop  = 1'b1;
                        end
                        wait_cycles(10);
                        if (m_shire_cache_reqq.reqq_is_empty() == 1'b1) begin
                           m_shutdown_objection--;
                           phase.drop_objection(this);
                           m_shire_cache_reqq.wait_for_reqq_not_empty();
                           #0 if (m_global_attrs.get_cur_phase() != ShutdownPhase) begin
                              disable reqq_count_outer_loop;
                              break;
                           end
                           phase.raise_objection(this);
                           m_shutdown_objection++;
                        end
                     end
                     forever begin :reqq_count_outer_loop
                        #0 wait (start_timeout_count == 1'b1);
                        start_timeout_count = 1'b0;
                        #0 if (m_global_attrs.get_cur_phase() != ShutdownPhase) begin
                           disable reqq_wait_loop;
                           break;
                        end
                        fork
                           forever begin
                              wait (disable_inner_loop == 1'b1);
                              disable reqq_count_inner_loop;
                              break;
                           end
                           forever begin :reqq_count_inner_loop
                              active_count = m_shire_cache_reqq.get_reqq_active_count();
                              wait_cycles(m_global_attrs.get_timeout_value(ShutdownPhase));
                              if ((active_count > 0) && (active_count <= m_shire_cache_reqq.get_reqq_active_count())) begin
                                 `dut_error(get_abstract_name(), $sformatf("the active REQQ count is not decrementing (%0d -> %0d). Shutdown timeout is %d", active_count, m_shire_cache_reqq.get_reqq_active_count(),m_global_attrs.get_timeout_value(ShutdownPhase)))
                                 disable reqq_wait_loop;
                                 if (m_shutdown_objection-- > 0) begin
                                    phase.drop_objection(this);
                                 end
                                 disable reqq_count_outer_loop;
                              end
                           end
                        join
                     end
                  join
               end
            end
            begin :phase_block
               m_global_attrs.wait_for_end_of_phase(ShutdownPhase);
               disable reqq_wait_block;
               disable error_block;
               if (m_shutdown_objection-- > 0) begin
                  m_shutdown_objection = 0;
                  phase.drop_objection(this);
               end
            end
            begin :error_block
               m_global_attrs.wait_for_error_finish();
               disable reqq_wait_block;
               disable phase_block;
               if (m_shutdown_objection-- > 0) begin
                  m_shutdown_objection = 0;
                  phase.drop_objection(this);
               end
            end
         end
      join
   endtask : shutdown_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Task: post_shutdown_phase
   //
   virtual task post_shutdown_phase(uvm_phase phase);
      fork
         begin
            super.post_shutdown_phase(phase);
         end
         begin
            if ((m_shire_cache_reqq != null) && (m_shire_cache_reqq.get_reqq_active_count() > 0)) begin
               m_shire_cache_reqq.print_active_reqq_entries();
            end
         end
      join
   endtask : post_shutdown_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Task: run_phase
   //
   virtual task run_phase(uvm_phase phase);
      fork
         super.run_phase(phase);
         process_rtl_events();
         watch_bus_operations();
      join_none
   endtask : run_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Task: watch_for_port_requests
   //
   task watch_for_port_requests(input int port_num, input uvm_tlm_analysis_fifo #(evl_req_desc) req_fifo);
      evl_req_desc rcv_req_desc;

      if (req_fifo == null) begin
         return;
      end
      fork
         forever begin
            req_fifo.get(rcv_req_desc);
            if (rcv_req_desc != null) begin
               if (rcv_req_desc.get_bus_bank() < `SC_BANKS) begin
                  if (rcv_req_desc.get_bus_cmd() == evl_bus_cmd_t'(evl_etlink_types_pkg::ET_LINK_REQ_MsgSendData)) begin
                     m_shire_cache_reqq.enqueue_port_request(port_num, (rcv_req_desc.get_paddr() >> `SC_MSG_NEIGH_ID_START) & ((1 << `SC_BANK_ID_SIZE) - 1), rcv_req_desc);
                  end
                  else begin
                     m_shire_cache_reqq.enqueue_port_request(port_num, (rcv_req_desc.get_paddr() >> `SC_BANK_LSB) & ((1 << `SC_BANK_ID_SIZE) - 1), rcv_req_desc);
                  end
               end
            end
         end
      join_none
   endtask : watch_for_port_requests


   //-----------------------------------------------------------------------------------------------
   //
   // Task: watch_for_sent_l3_mesh_requests
   //
   task watch_for_sent_l3_mesh_requests(input uvm_tlm_analysis_fifo #(evl_req_desc) req_fifo, input int port_num);
      evl_req_desc rcv_req_desc;

      if (req_fifo == null) begin
         return;
      end
      fork
         forever begin
            evl_req_desc selected_req_desc;

            req_fifo.get(rcv_req_desc);
            if (rcv_req_desc != null) begin
               selected_req_desc = m_shire_cache_axi_meshq.enqueue_active_l3_mesh_request(rcv_req_desc, port_num);
               if (selected_req_desc != null) begin
                  m_active_l3_mesh_req_descs.push_back(selected_req_desc);
               end
            end
         end
      join_none
   endtask : watch_for_sent_l3_mesh_requests


   //-----------------------------------------------------------------------------------------------
   //
   // Task: watch_for_l3_mesh_responses
   //
   task watch_for_l3_mesh_port_responses(input uvm_tlm_analysis_fifo #(evl_req_desc) req_fifo, input int port_num);
      evl_req_desc rcv_req_desc;

      if (req_fifo == null) begin
         return;
      end
      fork
         forever begin
            req_fifo.get(rcv_req_desc);
            m_l3_mesh_rsp_queue.put(rcv_req_desc);
         end
      join_none
   endtask : watch_for_l3_mesh_port_responses


   //-----------------------------------------------------------------------------------------------
   //
   // Task: watch_for_l3_mesh_responses
   //
   task watch_for_l3_mesh_responses();
      if (m_l3_mesh_rsp_queue == null) begin
         return;
      end
      fork
         forever begin
            evl_req_desc rcv_req_desc;

            m_l3_mesh_rsp_queue.get(rcv_req_desc);
            if (rcv_req_desc != null) begin
               foreach (m_active_l3_mesh_req_descs[ii]) begin
                  if (m_active_l3_mesh_req_descs[ii] == rcv_req_desc) begin
                     int sb_type = rcv_req_desc.get_int_param(pk_sc_scoreboard_type);

                     m_active_l3_mesh_req_descs.delete(ii);
                     if (sb_type == int'(SCP_SCOREBOARD)) begin
                        m_scp_scoreboard.process_mesh_response(rcv_req_desc);
                     end
                     else if (sb_type == int'(L3_SCOREBOARD)) begin
                        m_l3_scoreboard.process_mesh_response(rcv_req_desc);
                     end
                     else begin
                        m_l2_scoreboard.process_mesh_response(rcv_req_desc);
                     end
                     break;
                  end
               end
            end
         end
      join_none
   endtask : watch_for_l3_mesh_responses


   //-----------------------------------------------------------------------------------------------
   //
   // Task: watch_for_received_mesh_l3_requests
   //
   task watch_for_received_mesh_l3_requests();
      evl_req_desc rcv_req_desc;

      if (m_mesh_l3_req_queue == null) begin
         return;
      end
      fork
         forever begin
            m_mesh_l3_req_queue.get(rcv_req_desc);
            if (rcv_req_desc != null) begin
               bit         select_scp = 1'b0;
               evl_paddr_t addr       = rcv_req_desc.get_paddr();
               int         axi_port_num;
               int         bank_num;

               axi_port_num = rcv_req_desc.get_int_param(pk_mesh_l3_axi_port, -1);
               if (addr[`SC_SCP_REGION_SEL_ADDR_RANGE] == `SC_SCP_REGION_ID) begin
                  select_scp = 1'b1;
               end
               if (select_scp == 1'b1) begin
                  bank_num = int'(addr >> `SC_BANK_LSB) & ((1 << `SC_BANK_ID_SIZE) - 1);
               end
               else begin
                  bank_num = m_global_attrs.get_l3_bank_from_address(addr);
               end
               if ((m_num_l3_ports <= 1) || (bank_num == 0)) begin
                  axi_port_num = bank_num;
               end
               else if ((axi_port_num == 0) && (axi_port_num != bank_num)) begin
                  axi_port_num = `SC_BANKS + bank_num;
               end
               else begin
                  axi_port_num = bank_num;
               end
               m_shire_cache_reqq.enqueue_port_request(rcv_req_desc.get_int_param(pk_mesh_l3_port_num), axi_port_num, rcv_req_desc);
            end
         end
      join_none
   endtask : watch_for_received_mesh_l3_requests


   //-----------------------------------------------------------------------------------------------
   //
   // Task: watch_for_received_mesh_l3_port_requests
   //
   task watch_for_received_mesh_l3_port_requests(input uvm_tlm_analysis_fifo #(evl_req_desc) req_fifo, input int port_num);
      evl_req_desc rcv_req_desc;

      if (req_fifo == null) begin
         return;
      end
      fork
         forever begin
            req_fifo.get(rcv_req_desc);
            rcv_req_desc.set_int_param(pk_mesh_l3_port_num, `EVL_BASE_PORT_NUM_AXI);
            rcv_req_desc.set_int_param(pk_mesh_l3_axi_port, port_num);
            m_mesh_l3_req_queue.put(rcv_req_desc);
         end
      join_none
   endtask : watch_for_received_mesh_l3_port_requests


   //-----------------------------------------------------------------------------------------------
   //
   // Task: watch_for_sent_sys_mesh_requests
   //
   task watch_for_sent_sys_mesh_requests(input uvm_tlm_analysis_fifo #(evl_req_desc) req_fifo);
      evl_req_desc rcv_req_desc;

      if (req_fifo == null) begin
         return;
      end
      fork
         forever begin
            evl_req_desc selected_req_desc;

            req_fifo.get(rcv_req_desc);
            if (rcv_req_desc != null) begin
               selected_req_desc = m_shire_cache_axi_meshq.enqueue_active_sys_mesh_request(rcv_req_desc);
               if (selected_req_desc != null) begin
                  m_active_sys_mesh_req_descs.push_back(selected_req_desc);
               end
            end
         end
      join_none
   endtask : watch_for_sent_sys_mesh_requests


   //-----------------------------------------------------------------------------------------------
   //
   // Task: watch_for_sys_mesh_responses
   //
   task watch_for_sys_mesh_responses(input uvm_tlm_analysis_fifo #(evl_req_desc) req_fifo);
      evl_req_desc rcv_req_desc;

      if (req_fifo == null) begin
         return;
      end
      fork
         forever begin
            req_fifo.get(rcv_req_desc);
            if (rcv_req_desc != null) begin
               foreach (m_active_sys_mesh_req_descs[ii]) begin
                  if (m_active_sys_mesh_req_descs[ii] == rcv_req_desc) begin
                     int sb_type = rcv_req_desc.get_int_param(pk_sc_scoreboard_type);

                     m_active_sys_mesh_req_descs.delete(ii);
                     if (sb_type == int'(SCP_SCOREBOARD)) begin
                        m_scp_scoreboard.process_mesh_response(rcv_req_desc);
                     end
                     else if (sb_type == int'(L3_SCOREBOARD)) begin
                        m_l3_scoreboard.process_mesh_response(rcv_req_desc);
                     end
                     else begin
                        m_l2_scoreboard.process_mesh_response(rcv_req_desc);
                     end
                     break;
                  end
               end
            end
         end
      join_none
   endtask : watch_for_sys_mesh_responses


   //-----------------------------------------------------------------------------------------------
   //
   // Task: watch_bus_operations
   //
   task watch_bus_operations();
      int port_index;

      foreach (m_upstream_req_fifo[ii]) begin
         watch_for_port_requests(ii, m_upstream_req_fifo[ii]);
      end

      watch_for_l3_mesh_responses();
      for (int ii = 0; ii < m_num_l3_ports; ii++) begin
         port_index = m_global_attrs.get_shire_axi_port_index(int'(SHIRE_L2TOL3_MESH_AXI), ii, m_num_l3_ports, m_axi_feature_select);
         watch_for_sent_l3_mesh_requests(m_axi_req_fifo[port_index], ii);
      end
      for (int ii = 0; ii < m_num_l3_ports; ii++) begin
         port_index = m_global_attrs.get_shire_axi_port_index(int'(SHIRE_L2TOL3_MESH_AXI), ii, m_num_l3_ports, m_axi_feature_select);
         watch_for_l3_mesh_port_responses(m_axi_rsp_fifo[port_index], ii);
      end

      port_index = m_global_attrs.get_shire_axi_port_index(int'(SHIRE_L3TOSYS_MESH_AXI), 0, m_num_l3_ports, m_axi_feature_select);
      watch_for_sent_sys_mesh_requests(m_axi_req_fifo[port_index]);
      watch_for_sys_mesh_responses(m_axi_rsp_fifo[port_index]);

      watch_for_received_mesh_l3_requests();
      for (int ii = 0; ii < m_num_l3_ports; ii++) begin
         port_index = m_global_attrs.get_shire_axi_port_index(int'(SHIRE_MESH_TOL3_AXI), ii, m_num_l3_ports, m_axi_feature_select);
         watch_for_received_mesh_l3_port_requests(m_axi_req_fifo[port_index], ii);
      end
   endtask : watch_bus_operations


   //-----------------------------------------------------------------------------------------------
   //
   // Function: process_sc_l3_index_config
   //
   function void process_sc_l3_index_config(input evl_sc_l3_index_config_t rtl_info);
      m_global_attrs.set_l3_bank_shift(0, rtl_info.bank_0_bit);
      m_global_attrs.set_l3_bank_shift(1, rtl_info.bank_1_bit);
      m_global_attrs.set_l3_bank_shift(2, rtl_info.bank_2_bit);
      m_global_attrs.set_l3_subbank_shift(0, rtl_info.subbank_0_bit);
      m_global_attrs.set_l3_subbank_shift(1, rtl_info.subbank_1_bit);
      m_global_attrs.set_l3_subbank_shift(2, rtl_info.subbank_2_bit);
      m_global_attrs.set_l3_shire_shift(rtl_info.shire_0_bit);
   endfunction : process_sc_l3_index_config


   //-----------------------------------------------------------------------------------------------
   //
   // Function: process_rtl_events
   //
   task process_rtl_events();
      evl_shire_cache_packet rtl_event;
      evl_trans_id_t         trid;
      int                    bank_id;
      evl_sc_packet_u        packet_info;

      if (m_shire_cache_rtl_port == null) begin
         return;
      end
      forever begin
         m_shire_cache_rtl_port.get_obs_sc(rtl_event);
         if (m_disable_sc_sb == 1'b0) begin
            if (rtl_event.get_packet_type() == SHIRE_CACHE_L3_INDEX_CONFIG) begin
               packet_info = rtl_event.get_info();
               process_sc_l3_index_config(packet_info.l3_index_config_info.info);
            end
            else begin
               if (rtl_event.get_packet_source() == int'(SC_Source_L2)) begin
                  m_l2_scoreboard.process_rtl_event(rtl_event);
               end
               else if (rtl_event.get_packet_source() == int'(SC_Source_L3)) begin
                  m_l3_scoreboard.process_rtl_event(rtl_event);
               end
               else if (rtl_event.get_packet_source() == int'(SC_Source_Scp)) begin
                  m_scp_scoreboard.process_rtl_event(rtl_event);
               end
            end
         end
      end
   endtask : process_rtl_events


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string name_in = "evl_shire_cache_env", input uvm_component parent_in = null);
      int value;

      super.new(name_in, parent_in, SHIRE_CACHE_RTL_AGENT);
      set_abstract_name("SHIRE_CACHE");
      add_agent_alias(SHIRE_CACHE_AGENT);
      if (`et_value_plusargs("sc_sb_en=%d", value) == 0) begin
         value = 1;
      end
      m_disable_sc_sb = (value == 0) ? 1'b1 : 1'b0;
      if (`et_value_plusargs("sc_sb_esr_en=%d", value) == 0) begin
         value = 1;
      end
      m_disable_sc_sb_esr = (value == 0) ? 1'b1 : 1'b0;
   endfunction : new

endclass : evl_shire_cache_env

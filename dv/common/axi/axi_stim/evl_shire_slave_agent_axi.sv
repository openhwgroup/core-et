//--------------------------------------------------------------------------------------------------
//
// Class: evl_shire_slave_agent_axi
//
class evl_shire_slave_agent_axi extends evl_stim_agent_axi;

   `uvm_component_utils(evl_shire_slave_agent_axi)

   //-----------------------------------------------------------------------------------------------
   //
   // Member Variables and Objects
   //
   bit m_allow_loopback = 1'b1;


   //-----------------------------------------------------------------------------------------------
   //
   // Function: build_tables
   //
   virtual function void build_tables();

      //              |------------------------ Table Inputs -------------------------||---------------- Table Outputs ----------------|
      //               Cmd                  Model State  Qual               Source      Tag Action  Data Action  Bus Rsp    Bus Command
      //               -------------------  -----------  -----------------  ----------  ----------  -----------  ---------  -----------
      add_bus_req_act( `EVL_AXI_REQ_READ,   I,           AXQ_Local,         AXI_NoSrc,  St_Ignore,  Data_MemRd,  RspWData,  BusIdle     );
      add_bus_req_act( `EVL_AXI_REQ_WRITE,  I,           AXQ_Local,         AXI_NoSrc,  St_Ignore,  Data_MemWr,  RspAck,    BusIdle     );

      add_bus_req_act( `EVL_AXI_REQ_READ,   I,           AXQ_AtomicLocal,   AXI_NoSrc,  St_Ignore,  Data_MemRd,  RspWData,  BusIdle     );

      add_bus_req_act( `EVL_AXI_REQ_READ,   I,           AXQ_AtomicRemote,  AXI_NoSrc,  St_Ignore,  Data_MemRd,  RspWData,  BusIdle     );
      add_bus_req_act( `EVL_AXI_REQ_WRITE,  I,           AXQ_AtomicRemote,  AXI_NoSrc,  St_Ignore,  Data_MemWr,  RspAck,    BusIdle     );

      add_bus_req_act( `EVL_AXI_REQ_READ,   I,           AXQ_MsgLocal,      AXI_NoSrc,  St_Ignore,  Data_MemRd,  RspWData,  BusIdle     );
      add_bus_req_act( `EVL_AXI_REQ_WRITE,  I,           AXQ_MsgLocal,      AXI_NoSrc,  St_Ignore,  Data_MemWr,  RspAck,    BusIdle     );
      add_bus_req_act( `EVL_AXI_REQ_READ,   I,           AXQ_MsgRemote,     AXI_NoSrc,  St_Ignore,  Data_MemRd,  RspWData,  BusIdle     );
      add_bus_req_act( `EVL_AXI_REQ_WRITE,  I,           AXQ_MsgRemote,     AXI_NoSrc,  St_Ignore,  Data_MemWr,  RspAck,    BusIdle     );

      add_bus_req_act( `EVL_AXI_REQ_READ,   I,           AXQ_None,          AXI_NoSrc,  St_Ignore,  Data_MemRd,  RspWData,  BusIdle     );
      add_bus_req_act( `EVL_AXI_REQ_WRITE,  I,           AXQ_None,          AXI_NoSrc,  St_Ignore,  Data_MemWr,  RspAck,    BusIdle     );

      if (m_allow_loopback == 1'b1) begin
         //              |------------------------ Table Inputs -------------------------||--------------------- Table Outputs ---------------------|
         //               Cmd                  Model State  Qual               Source      Tag Action  Data Action         Bus Rsp    Bus Command
         //               -------------------  -----------  -----------------  ----------  ----------  ------------------  ---------  --------------
         add_bus_req_act( `EVL_AXI_REQ_READ,   I,           AXQ_Remote,        AXI_NoSrc,  St_Ignore,  Data_None,          RspNull,   BusRead        );
         add_bus_req_act( `EVL_AXI_REQ_WRITE,  I,           AXQ_Remote,        AXI_NoSrc,  St_Ignore,  Data_FwdWr,         RspNull,   BusWrite       );
         add_bus_req_act( `EVL_AXI_REQ_WRITE,  I,           AXQ_AtomicLocal,   AXI_NoSrc,  St_Ignore,  Data_FwdAtomicRsp,  RspAck,    BusWriteAtomic );
      end
      else begin
         add_bus_req_act( `EVL_AXI_REQ_READ,   I,           AXQ_Remote,        AXI_NoSrc,  St_Ignore,  Data_MemRd,         RspWData,  BusIdle        );
         add_bus_req_act( `EVL_AXI_REQ_WRITE,  I,           AXQ_Remote,        AXI_NoSrc,  St_Ignore,  Data_MemWr,         RspAck,    BusIdle        );
         add_bus_req_act( `EVL_AXI_REQ_WRITE,  I,           AXQ_AtomicLocal,   AXI_NoSrc,  St_Ignore,  Data_MemWr,         RspAck,    BusIdle        );
      end

      // Actions to take when a response comes back:
      //
      //              |--------------- Table Inputs ----------------||--------- Table Outputs ---------|
      //               Cmd                  Model State  Qual         Tag Action  Data Action  Bus Rsp
      //               -------------------  -----------  -----------  ----------  -----------  --------
      add_bus_rsp_act( `EVL_AXI_REQ_READ,   I,           AXQ_Remote,  St_Ignore,  Data_FwdRd,  RspWData );
      add_bus_rsp_act( `EVL_AXI_REQ_WRITE,  I,           AXQ_Remote,  St_Ignore,  Data_FwdWr,  RspAck   );

   endfunction : build_tables


// //-----------------------------------------------------------------------------------------------
// //
// //
// //
// virtual function evl_axi_source_t get_axi_source(input int req_port);
//    if ((req_port >= m_l2_to_l3_first_rcv_port) && (req_port <= m_l2_to_l3_last_rcv_port)) begin
//       return AXI_L2_to_L3;
//    end
//    if ((req_port >= m_l3_to_sys_first_rcv_port) && (req_port <= m_l3_to_sys_last_rcv_port)) begin
//       return AXI_L3_to_Sys;
//    end
//    if ((req_port >= m_uc_to_l3_first_rcv_port) && (req_port <= m_uc_to_l3_last_rcv_port)) begin
//       return AXI_UC_to_L3;
//    end
//    if ((req_port >= m_uc_to_sys_first_rcv_port) && (req_port <= m_uc_to_sys_last_rcv_port)) begin
//       return AXI_UC_to_Sys;
//    end
//    return AXI_NoSrc;
// endfunction : get_axi_source


// //-----------------------------------------------------------------------------------------------
// //
// // Function: select_req_port
// //
// virtual function int select_req_port(input evl_req_desc req_desc, input evl_axi_source_t axi_source);
//    int snd_req_port = req_desc.get_int_param(pk_tlm_req_port, 0);

//    if (axi_source == AXI_L2_to_L3) begin
//       snd_req_port = snd_req_port - m_l2_to_l3_first_rcv_port;
//       if ((snd_req_port < 0) || (snd_req_port >= m_l3_loopback_snd_ports.size())) begin
//          `dut_error(get_abstract_name(), $sformatf("TB_ISSUE: attempting to send a request to a non-existent loopback port"))
//          return m_l3_loopback_snd_ports[0];
//       end
//       return m_l3_loopback_snd_ports[snd_req_port];
//    end
//    else if (axi_source == AXI_L3_to_Sys) begin
//       if (req_desc.get_bus_qos() == `AXI_QOS_ATOMIC_RSP) begin
//          return m_uc_sys_loopback_snd_ports[0];
//       end
//       return m_sys_loopback_snd_ports[0];
//    end
//    else if (axi_source == AXI_UC_to_L3) begin
//       if (req_desc.get_bus_qos() == `AXI_QOS_ATOMIC_RSP) begin
//          return m_uc_sys_loopback_snd_ports[0];
//       end
//       return m_uc_l3_loopback_snd_ports[0];
//    end
//    else if (axi_source == AXI_UC_to_Sys) begin
//       return m_uc_sys_loopback_snd_ports[0];
//    end
//    return 0;
// endfunction : select_req_port


   //-----------------------------------------------------------------------------------------------
   //
   //
   //
   virtual function evl_axi_qual_t preprocess_qualifier(input evl_req_desc req_desc, input evl_axi_qual_t axi_qual_in, input evl_axi_source_t axi_source);
      evl_trans_desc trans_desc;

      trans_desc = req_desc.get_root_trans_desc();
      if ((trans_desc != null) && (trans_desc.get_int_param(pk_from_axi_stim_agent, 0) > 0)) begin
         if (trans_desc.get_int_param_by_name("pk_noc_l3_miss", 0) == 1) begin
            return AXQ_Remote;
         end
         return axi_qual_in;
      end
      if (trans_desc.get_int_param_by_name("pk_noc_l3_miss", 0) == 1) begin
         return AXQ_Remote;
      end
      return axi_qual_in;
   endfunction : preprocess_qualifier


   //-----------------------------------------------------------------------------------------------
   //
   // Function: select_rsp_port
   //
   virtual function int select_rsp_port(input evl_req_desc req_desc);
      return req_desc.get_int_param(pk_tlm_req_port, 0);
   endfunction : select_rsp_port


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_req_desc_port
   //
   virtual function uvm_analysis_port#(evl_req_desc) get_req_desc_port(input string port_name      = "unknown",
                                                                       input int    port_number    = 0,
                                                                       input int    subport_number = 0);
      if ((port_name == "m_snd_rsp_port") || (port_name == "sa_snd_rsp") || (port_name == "snd_rsp")) begin
         if (port_number < m_snd_rsp_ports.size()) begin
            return m_snd_rsp_ports[port_number];
         end
      end
      else if (port_name == "m_loopback_req_port") begin
         if (port_number < m_snd_req_ports.size()) begin
            return m_snd_req_ports[port_number];
         end
      end
      return null;
   endfunction : get_req_desc_port


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_req_desc_export
   //
   virtual function uvm_analysis_export#(evl_req_desc) get_req_desc_export(input string port_name      = "unknown",
                                                                           input int    port_number    = 0,
                                                                           input int    subport_number = 0);
      if ((port_name == "m_obs_req_export") || (port_name == "obs_req")) begin
         if (port_number < m_obs_req_exports.size()) begin
            return m_obs_req_exports[port_number];
         end
      end
      else if (port_name == "m_loopback_rsp_export") begin
         if (port_number < m_rcv_rsp_exports.size()) begin
            return m_rcv_rsp_exports[port_number];
         end
      end
      return null;
   endfunction : get_req_desc_export


   //-----------------------------------------------------------------------------------------------
   //
   // Function: connect_my_bus_ports
   //
   // This function is used to connect all of the ports that go between this stimulus agent and its
   // bus agent.  Because this stimulus agent supports loopbacks, we need to redefine this method
   // to only connect to the appropriate bus agent ports.
   //
   virtual function void connect_my_bus_ports(input evl_verif_comp bus_verif_comp,
                                              input string         port_name      = "",
                                              input int            port_number    = 0,
                                              input int            subport_number = 0);
      uvm_analysis_port   #(evl_req_desc) bus_port;
      uvm_analysis_export #(evl_req_desc) bus_export;

      bus_export = bus_verif_comp.get_req_desc_export("sa_snd_rsp", port_number);
      if ((port_number < m_snd_rsp_ports.size()) && (m_snd_rsp_ports[port_number] != null) && (bus_export != null)) begin
         `evl_log(UVM_MEDIUM, VMOD_CONFIG, get_abstract_name(), $sformatf("connecting %0s -> %0s", m_snd_rsp_ports[port_number].get_full_name(), bus_export.get_full_name()))
         m_snd_rsp_ports[port_number].connect(bus_export);
      end

      bus_port = bus_verif_comp.get_req_desc_port("m_obs_req_port", port_number);
      if ((port_number < m_obs_req_exports.size()) && (m_obs_req_exports[port_number] != null) && (bus_port != null)) begin
         `evl_log(UVM_MEDIUM, VMOD_CONFIG, get_abstract_name(), $sformatf("connecting %0s -> %0s", bus_port.get_full_name(), m_obs_req_exports[port_number].get_full_name()))
         bus_port.connect(m_obs_req_exports[port_number]);
      end
   endfunction : connect_my_bus_ports


   //-----------------------------------------------------------------------------------------------
   //
   // Function: build_phase
   //
   virtual function void build_phase(uvm_phase phase);
      super.build_phase(phase);
      //m_allow_loopback = (get_int_param(pk_allow_loopback, 0) <= 0) ? 1'b0 : 1'b1;
      if (get_int_param(pk_allow_loopback, 0) <= 0) begin
         m_allow_loopback = 1'b0;
      end
      else begin
         m_allow_loopback = 1'b1;
      end

      //
      // Incoming AXI slave port connections
      //
      for (int ii = 0; ii < m_num_l3_ports; ii++) begin
         m_snd_rsp_ports.push_back(null);
         m_obs_req_exports.push_back(null);
         m_obs_req_fifos.push_back(null);
         m_snd_rsp_ports[ii]   = new($sformatf("m_snd_rsp_port_%0d", ii), this);
         m_obs_req_exports[ii] = new($sformatf("m_obs_req_export_%0d", ii), this);
         m_obs_req_fifos[ii]   = new($sformatf("m_obs_req_fifo_%0d", ii), this);
      end

      if (m_allow_loopback == 1'b1) begin
         //
         // Loopback port connections
         //
         m_snd_req_ports.push_back(null);
         m_rcv_rsp_exports.push_back(null);
         m_rcv_rsp_fifos.push_back(null);
         m_snd_req_ports[0]   = new($sformatf("m_snd_req_port"), this);
         m_rcv_rsp_exports[0] = new($sformatf("m_rcv_rsp_export"), this);
         m_rcv_rsp_fifos[0]   = new($sformatf("m_rcv_rsp_fifo"), this);
      end
   endfunction : build_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string           name_in       = "evl_shire_slave_agent_axi",
                input uvm_component    parent_in     = null,
                input evl_agent_type_t agent_type_in = SHIRE_AXI_SLAVE_STIM_AGENT);
      evl_axi_bridge_id_t bridge_id = evl_axi_bridge_id_t'(16);
      evl_axi_reqq_id_t   reqq_id   = evl_axi_reqq_id_t'(0);
      evl_axi_bank_id_t   bank_id   = evl_axi_bank_id_t'(0);
      int                 value;

      super.new(name_in, parent_in, agent_type_in);
      if (`et_value_plusargs("allow_l3_loopback=%d", value) == 0) begin
         value = 1;
      end
      m_memory_agent         = 1'b1;
      m_allow_mem_load       = 1'b1;
      bridge_id              = evl_axi_bridge_id_t'(0);
      bank_id                = evl_axi_bank_id_t'(0);
      reqq_id                = evl_axi_reqq_id_t'(1);
      m_bus_req_fwd_id       = evl_bus_req_id_t'({ bridge_id, bank_id, reqq_id });
      bridge_id              = evl_axi_bridge_id_t'(15);
      bank_id                = ~evl_axi_bank_id_t'(0);
      reqq_id                = ~evl_axi_reqq_id_t'(0);
      //m_bus_req_fwd_id_limit = evl_bus_req_id_t'({ bridge_id, bank_id, reqq_id });
      m_bus_req_fwd_id_limit = evl_bus_req_id_t'(511);
      set_abstract_name("SHIRE(STIM)");
      add_agent_alias(MEM_STIM_AGENT);
      add_agent_alias(AXI_SLAVE_STIM_AGENT);
      add_agent_alias(MEM_AGENT);
      set_active_slave();
   endfunction : new

endclass : evl_shire_slave_agent_axi

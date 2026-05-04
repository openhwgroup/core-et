//--------------------------------------------------------------------------------------------------
//
// Class: evl_fromsys_master_agent_axi
//
// This class is an extension of evl_stim_agent_axi to
// define a master stim agent that acts like the mesh driving
// the request to the SBM/UC box
//
class evl_fromsys_master_agent_axi extends evl_stim_agent_axi;

   `uvm_component_utils(evl_fromsys_master_agent_axi)


   //-----------------------------------------------------------------------------------------------
   //
   // Additional Member TLM Ports
   //
   uvm_analysis_port     #(evl_req_desc) m_snd_uc_rsp_port;
   uvm_analysis_export   #(evl_req_desc) m_obs_uc_req_export;
   uvm_tlm_analysis_fifo #(evl_req_desc) m_obs_uc_req_fifo;

   uvm_analysis_port     #(evl_req_desc) m_snd_sys_rsp_port;
   uvm_analysis_export   #(evl_req_desc) m_obs_sys_req_export;
   uvm_tlm_analysis_fifo #(evl_req_desc) m_obs_sys_req_fifo;

   uvm_analysis_port     #(evl_req_desc) m_snd_l3_rsp_port;
   uvm_analysis_export   #(evl_req_desc) m_obs_l3_req_export;
   uvm_tlm_analysis_fifo #(evl_req_desc) m_obs_l3_req_fifo;


   //-----------------------------------------------------------------------------------------------
   //
   // Function: build_tables
   //
   virtual function void build_tables();
      //
      // Actions to take when we receive a request from the simulus.
      //
      // Int Req Table Definitions:
      //   Cmd:        The command from the stimulus world. These are generic because they will be
      //               used across many types of interfaces.
      //   Cached:     The request is cached or uncached.
      //   ModelState: The current cache state of the cache model inside this agent.
      //   Qual:       Optional qualifier that is currently unused.
      //   TagAction:  What needs to happen to the state of the cache based on this transaction.
      //   DataAction: What needs to happen to the Data based on this transaction.
      //   BusCmd:     What (if anything) needs to be driven on the bus based on this transaction.
      //
      // DataAction Definitions:
      //   Data_ReqWr: Copy write data from the trans_desc to the req_desc that is being sent to the
      //               driver.
      //
      //              |-------------- Table Inputs --------------||---------- Table Outputs -----------|
      //               Cmd         Cached  Model State  Qual       Tag Action  Data Action  Bus Command
      //               ----------  ------  -----------  ---------  ----------  -----------  -----------
      add_int_req_act( LD,         1'b1,   I,           AXQ_None,  St_Ignore,  Data_None,   BusRead     );
      add_int_req_act( ST,         1'b1,   I,           AXQ_None,  St_Ignore,  Data_ReqWr,  BusWrite    );
      add_int_req_act( LD_NO_MOD,  1'b1,   I,           AXQ_None,  St_Ignore,  Data_None,   BusRead     );
      add_int_req_act( ST_NO_MOD,  1'b1,   I,           AXQ_None,  St_Ignore,  Data_ReqWr,  BusWrite    );
      add_int_req_act( LD_COOP,    1'b1,   I,           AXQ_None,  St_Ignore,  Data_None,   BusRead     );
      add_int_req_act( ST_WA,      1'b1,   I,           AXQ_None,  St_Ignore,  Data_ReqWr,  BusWrite    );

      add_int_req_act( LD,         1'b0,   I,           AXQ_None,  St_Ignore,  Data_None,   BusRead     );
      add_int_req_act( ST,         1'b0,   I,           AXQ_None,  St_Ignore,  Data_ReqWr,  BusWrite    );
      add_int_req_act( LD_NO_MOD,  1'b0,   I,           AXQ_None,  St_Ignore,  Data_None,   BusRead     );
      add_int_req_act( ST_NO_MOD,  1'b0,   I,           AXQ_None,  St_Ignore,  Data_ReqWr,  BusWrite    );
      add_int_req_act( LD_COOP,    1'b0,   I,           AXQ_None,  St_Ignore,  Data_None,   BusRead     );
      add_int_req_act( ST_WA,      1'b0,   I,           AXQ_None,  St_Ignore,  Data_ReqWr,  BusWrite    );

      //
      // Actions to take when an incoming request occurs:
      //
      //              |------------------------- Table Inputs --------------------------||--------------- Table Outputs ----------------|
      //               Cmd                  Model State  Qual                 Source      Tag Action  Data Action  Bus Rsp   Bus Command
      //               -------------------  -----------  -------------------  ----------  ----------  -----------  --------  -----------
      add_bus_req_act( `EVL_AXI_REQ_READ,   I,           AXQ_Remote,          AXI_NoSrc,  St_Ignore,  Data_None,   RspNull,  BusRead     );
      add_bus_req_act( `EVL_AXI_REQ_WRITE,  I,           AXQ_Remote,          AXI_NoSrc,  St_Ignore,  Data_FwdWr,  RspNull,  BusWrite    );
      add_bus_req_act( `EVL_AXI_REQ_WRITE,  I,           AXQ_MsgRemote,       AXI_NoSrc,  St_Ignore,  Data_FwdWr,  RspNull,  BusWrite    );
      add_bus_req_act( `EVL_AXI_REQ_WRITE,  I,           AXQ_AtomicComplete,  AXI_NoSrc,  St_Ignore,  Data_FwdWr,  RspNull,  BusWrite    );

      //
      // Actions to take when a response comes back:
      //
      // Bus Response Table Definitions:
      //   Cmd:        The response opcode that was observed on the bus.
      //   ModelState: The current state of the cache model inside this agent.
      //   Qual:       Optional qualifier that is currently unused.
      //   TagAction:  What needs to happen to the state of the cache based on this response.
      //   DataAction: What needs to happen to the data based on this response.
      //
      // DataAction Definitions:
      //   Data_ReqRd: Copy the data from the req_desc to the trans_desc so that the original test has access to it.
      //
      //              |---------------- Table Inputs ---------------||--------- Table Outputs ---------|
      //               Cmd                  Model State  Qual         Tag Action  Data Action  Bus Rsp
      //               -------------------  -----------  -----------  ----------  -----------  --------
      add_bus_rsp_act( `EVL_AXI_REQ_READ,   I,           AXQ_None,    St_Ignore,  Data_ReqRd,  RspNull  );
      add_bus_rsp_act( `EVL_AXI_REQ_WRITE,  I,           AXQ_None,    St_Ignore,  Data_None,   RspNull  );
      add_bus_rsp_act( `EVL_AXI_REQ_READ,   I,           AXQ_Local,   St_Ignore,  Data_ReqRd,  RspNull  );
      add_bus_rsp_act( `EVL_AXI_REQ_WRITE,  I,           AXQ_Local,   St_Ignore,  Data_None,   RspNull  );
      add_bus_rsp_act( `EVL_AXI_REQ_READ,   I,           AXQ_Remote,  St_Ignore,  Data_FwdRd,  RspWData );
      add_bus_rsp_act( `EVL_AXI_REQ_WRITE,  I,           AXQ_Remote,  St_Ignore,  Data_FwdWr,  RspAck   );
   endfunction : build_tables


   //----------------------------------------------------------------------------------------------
   //
   // Function: final_fixup
   //
   virtual function void final_fixup(input evl_trans_desc trans_desc, input evl_req_desc req_desc);
      if (req_desc.get_vc() == ~evl_bus_req_vc_t'(0)) begin
         trans_desc.set_vc(`SBM_SYS_SLAVE_VSI_ID);
         req_desc.set_vc(`SBM_SYS_SLAVE_VSI_ID);
      end

      case (trans_desc.get_cmd())
         LD_NO_MOD, ST_NO_MOD, ATOMIC, MESSAGE: req_desc.set_bus_cache_attr(evl_bus_cache_attr_t'(4'b1101));
      endcase
   endfunction : final_fixup


   //----------------------------------------------------------------------------------------------
   //
   // Function: post_process_loopback_req
   //
   //   Requests sent by the UCTOSYS through the loopback need to be driven either to the UC block
   //   or the SBM.  This is defined by the value of the AXI QOS field.  See UC spec Section 5.2
   //   and 5.6
   //
   virtual function void post_process_loopback_req(input evl_trans_desc trans_desc, input evl_req_desc req_desc);
      if (req_desc.get_bus_qos() == 3) begin
         `evl_log_mdesc(UVM_HIGH, VMOD_AXI_STIM, get_abstract_name(), req_desc, $sformatf("loopback of %0s - driving towards SBM", req_desc.sprint_obj()))
         trans_desc.set_vc(`SBM_SYS_SLAVE_VSI_ID);
         req_desc.set_vc(`SBM_SYS_SLAVE_VSI_ID);
      end
      else if (req_desc.get_bus_qos() == 2) begin
         `evl_log_mdesc(UVM_HIGH, VMOD_AXI_STIM, get_abstract_name(), req_desc, $sformatf("loopback of %0s - driving towards UC", req_desc.sprint_obj()))
         trans_desc.set_vc(`UC_SYS_SLAVE_VSI_ID);
         req_desc.set_vc(`UC_SYS_SLAVE_VSI_ID);
      end
      else begin
         `dut_error_mdesc(get_abstract_name(), req_desc, $sformatf("Received loopback request with unexpected QOS field %0d", req_desc.get_bus_qos()))
      end
   endfunction : post_process_loopback_req


   //-----------------------------------------------------------------------------------------------
   //
   // Function: build_phase
   //
   virtual function void build_phase(uvm_phase phase);
      super.build_phase(phase);

      m_snd_req_ports.push_back(null);
      m_rcv_rsp_exports.push_back(null);
      m_rcv_rsp_fifos.push_back(null);

      m_snd_req_ports[0]   = new("m_snd_req_port", this);
      m_rcv_rsp_exports[0] = new("m_rcv_rsp_export", this);
      m_rcv_rsp_fifos[0]   = new("m_rcv_rsp_fifo", this);

      //
      // Incoming system mesh requests from the UC block.
      //
      m_uc_port = m_snd_rsp_ports.size();

      m_snd_uc_rsp_port    = new("m_snd_uc_rsp_port", this);
      m_obs_uc_req_export  = new("m_obs_uc_req_export", this);
      m_obs_uc_req_fifo    = new("m_obs_uc_req_fifo", this);

      m_snd_rsp_ports.push_back(m_snd_uc_rsp_port);
      m_obs_req_exports.push_back(m_obs_uc_req_export);
      m_obs_req_fifos.push_back(m_obs_uc_req_fifo);

      //
      // Incoming atomic completion requests from the shire cache sys port.
      //
      m_sys_port = m_snd_rsp_ports.size();

      m_snd_sys_rsp_port   = new("m_snd_sys_rsp_port", this);
      m_obs_sys_req_export = new("m_obs_sys_req_export", this);
      m_obs_sys_req_fifo   = new("m_obs_sys_req_fifo", this);

      m_snd_rsp_ports.push_back(m_snd_sys_rsp_port);
      m_obs_req_exports.push_back(m_obs_sys_req_export);
      m_obs_req_fifos.push_back(m_obs_sys_req_fifo);

      //
      // Incoming atomic completion requests from the UC L3 port.
      //
      m_l3_port = m_snd_rsp_ports.size();

      m_snd_l3_rsp_port   = new("m_snd_l3_rsp_port", this);
      m_obs_l3_req_export = new("m_obs_l3_req_export", this);
      m_obs_l3_req_fifo   = new("m_obs_l3_req_fifo", this);

      m_snd_rsp_ports.push_back(m_snd_l3_rsp_port);
      m_obs_req_exports.push_back(m_obs_l3_req_export);
      m_obs_req_fifos.push_back(m_obs_l3_req_fifo);
   endfunction : build_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Function: connect_my_bus_ports
   //
   // This function is used to connect all of the ports that go between this stimulus agent and its
   // bus agent.  Because the fromsys_mesh stimulus agent supports loopbacks, we need to redefine this
   // function to only connect the request side to the bus agents.
   //
   virtual function void connect_my_bus_ports(input evl_verif_comp bus_verif_comp,
                                              input string         port_name      = "",
                                              input int            port_number    = 0,
                                              input int            subport_number = 0);
      uvm_analysis_port   #(evl_req_desc) bus_port;
      uvm_analysis_export #(evl_req_desc) bus_export;

      bus_export = bus_verif_comp.get_req_desc_export("sa_snd_req", port_number);
      if ((port_number < m_snd_req_ports.size()) && (m_snd_req_ports[port_number] != null) && (bus_export != null)) begin
         `evl_log(UVM_MEDIUM, VMOD_CONFIG, get_abstract_name(), $sformatf("connecting %0s -> %0s", m_snd_req_ports[port_number].get_full_name(), bus_export.get_full_name()))
         m_snd_req_ports[port_number].connect(bus_export);
      end

      bus_port = bus_verif_comp.get_req_desc_port("m_rcv_rsp_port", port_number);
      if ((port_number < m_rcv_rsp_exports.size()) && (m_rcv_rsp_exports[port_number] != null) && (bus_port != null)) begin
         `evl_log(UVM_MEDIUM, VMOD_CONFIG, get_abstract_name(), $sformatf("connecting %0s -> %0s", bus_port.get_full_name(), m_rcv_rsp_exports[port_number].get_full_name()))
         bus_port.connect(m_rcv_rsp_exports[port_number]);
      end
   endfunction : connect_my_bus_ports


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string name_in = "evl_fromsys_master_agent_axi", input uvm_component parent_in = null);
      super.new(name_in, parent_in, FROMSYS_MASTER_STIM_AGENT);
      m_stimulus_agent = 1'b1;
      set_abstract_name("FROMSYS_MESH(STIM)");
      add_agent_alias(AXI_MASTER_STIM_AGENT);
      set_active_master();
   endfunction : new

endclass : evl_fromsys_master_agent_axi

//--------------------------------------------------------------------------------------------------
//
// Class: evl_tosys_master_agent_axi
//
// This class is an extension of evl_stim_agent_axi to define a master stim agent that acts like a
// _TOSYS master driving requests to the mesh.
//
class evl_tosys_master_agent_axi extends evl_stim_agent_axi;

   `uvm_component_utils(evl_tosys_master_agent_axi)


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
      //              |------------------ Table Inputs -------------------||------------------------ Table Outputs -------------------------|
      //               Cmd                  Cached  Model State  Qual       Tag Action  Data Action  Bus Command
      //               -------------------  ------  -----------  ---------  ----------  -----------  ---------------------------------------
      add_int_req_act( LD,                  1'b1,   I,           AXQ_None,  St_Ignore,  Data_None,   BusRead                                 );
      add_int_req_act( ST,                  1'b1,   I,           AXQ_None,  St_Ignore,  Data_ReqWr,  BusWrite                                );
      add_int_req_act( LD_NO_MOD,           1'b1,   I,           AXQ_None,  St_Ignore,  Data_None,   BusRead                                 );
      add_int_req_act( ST_NO_MOD,           1'b1,   I,           AXQ_None,  St_Ignore,  Data_ReqWr,  BusWrite                                );
      add_int_req_act( LD_COOP,             1'b1,   I,           AXQ_None,  St_Ignore,  Data_None,   BusRead                                 );
      add_int_req_act( ST_WA,               1'b1,   I,           AXQ_None,  St_Ignore,  Data_ReqWr,  BusWrite                                );

      add_int_req_act( LD,                  1'b0,   I,           AXQ_None,  St_Ignore,  Data_None,   BusRead                                 );
      add_int_req_act( ST,                  1'b0,   I,           AXQ_None,  St_Ignore,  Data_ReqWr,  BusWrite                                );
      add_int_req_act( LD_NO_MOD,           1'b0,   I,           AXQ_None,  St_Ignore,  Data_None,   BusRead                                 );
      add_int_req_act( ST_NO_MOD,           1'b0,   I,           AXQ_None,  St_Ignore,  Data_ReqWr,  BusWrite                                );
      add_int_req_act( LD_COOP,             1'b0,   I,           AXQ_None,  St_Ignore,  Data_None,   BusRead                                 );
      add_int_req_act( ST_WA,               1'b0,   I,           AXQ_None,  St_Ignore,  Data_ReqWr,  BusWrite                                );

      add_int_req_act( CACHE_EVICT,         1'b1,   I,           AXQ_None,  St_Ignore,  Data_Create, BusWrite                                );
      add_int_req_act( CACHE_EVICT_TO_MEM,  1'b1,   I,           AXQ_None,  St_Ignore,  Data_ReqWr,  BusWrite                                );
      add_int_req_act( CACHE_FLUSH,         1'b1,   I,           AXQ_None,  St_Ignore,  Data_Create, BusWrite                                );
      add_int_req_act( CACHE_FLUSH_TO_MEM,  1'b1,   I,           AXQ_None,  St_Ignore,  Data_ReqWr,  BusWrite                                );
      add_int_req_act( CACHE_PREFETCH,      1'b1,   I,           AXQ_None,  St_Ignore,  Data_Create, BusWrite                                );

      add_int_req_act( ATOMIC,              1'b1,   I,           AXQ_None,  St_Ignore,  Data_ReqWr,  BusWrite                                );
      add_int_req_act( MESSAGE,             1'b1,   I,           AXQ_None,  St_Ignore,  Data_ReqWr,  evl_bus_cmd_t'(ET_LINK_REQ_MsgSendData) );

      add_int_req_act( ATOMIC,              1'b0,   I,           AXQ_None,  St_Ignore,  Data_ReqWr,  BusWrite                                );
      add_int_req_act( MESSAGE,             1'b0,   I,           AXQ_None,  St_Ignore,  Data_ReqWr,  evl_bus_cmd_t'(ET_LINK_REQ_MsgSendData) );

      //
      // Actions to take when an incoming request occurs:
      //
      //              |------------------------ Table Inputs -------------------------||--------------- Table Outputs ----------------|
      //               Cmd                  Model State  Qual               Source      Tag Action  Data Action  Bus Rsp   Bus Command
      //               -------------------  -----------  -----------------  ----------  ----------  -----------  --------  -----------
      add_bus_req_act( `EVL_AXI_REQ_READ,   I,           AXQ_Remote,        AXI_NoSrc,  St_Ignore,  Data_None,   RspNull,  BusRead     );
      add_bus_req_act( `EVL_AXI_REQ_WRITE,  I,           AXQ_Remote,        AXI_NoSrc,  St_Ignore,  Data_FwdWr,  RspNull,  BusWrite    );

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
      //   Data_ReqRd: Copy the data from the req_desc to the trans_desc so that the original test
      //               has access to it.
      //
      //              |--------------- Table Inputs ---------------||-------- Table Outputs ---------|
      //               Cmd                  Model State  Qual        Tag Action  Data Action  Bus Rsp
      //               -------------------  -----------  ----------  ----------  -----------  -------
      add_bus_rsp_act( `EVL_AXI_REQ_READ,   I,           AXQ_Local,  St_Ignore,  Data_ReqRd,  RspNull  );
      add_bus_rsp_act( `EVL_AXI_REQ_WRITE,  I,           AXQ_Local,  St_Ignore,  Data_None,   RspNull  );
      add_bus_rsp_act( `EVL_AXI_REQ_READ,   I,           AXQ_Remote, St_Ignore,  Data_FwdRd,  RspWData );
      add_bus_rsp_act( `EVL_AXI_REQ_WRITE,  I,           AXQ_Remote, St_Ignore,  Data_FwdWr,  RspAck   );
   endfunction : build_tables


   //----------------------------------------------------------------------------------------------
   //
   // Function: final_fixup
   //
   virtual function void final_fixup(input evl_trans_desc trans_desc, input evl_req_desc req_desc);
      evl_cmd_t            cmd = trans_desc.get_cmd();
      evl_paddr_t          trans_addr;
      et_link_req_opcode_t etl_opcodes[5:0] = { evl_etlink_types_pkg::ET_LINK_REQ_Write, 
                                                evl_etlink_types_pkg::ET_LINK_REQ_Evict, 
                                                evl_etlink_types_pkg::ET_LINK_REQ_EvictToMem, 
                                                evl_etlink_types_pkg::ET_LINK_REQ_Flush, 
                                                evl_etlink_types_pkg::ET_LINK_REQ_FlushToMem, 
                                                evl_etlink_types_pkg::ET_LINK_REQ_Prefetch };

      trans_addr = trans_desc.get_paddr();
      if (trans_addr >= 40'h8000000000) begin
         if (trans_addr[20] == 1'b1) begin
            req_desc.set_bus_qos(`AXI_QOS_MEM_LOW_PRIORITY);
         end
         else begin
            req_desc.set_bus_qos(`AXI_QOS_MEM_HIGH_PRIORITY);
         end
      end
      else if (cmd == MESSAGE) begin
         req_desc.set_bus_qos(`AXI_QOS_MESSAGE);
      end
      else begin
         req_desc.set_bus_qos(`AXI_QOS_ESR);
      end
      req_desc.set_bus_prot(3'b100);

      if (cmd inside { LD_NO_MOD, ST_NO_MOD, ATOMIC, MESSAGE }) begin
         req_desc.set_bus_cache_attr(evl_bus_cache_attr_t'(4'b1101));
      end
      else begin
         req_desc.set_bus_cache_attr(evl_bus_cache_attr_t'(4'b1111));
      end

      if (req_desc.bus_cmd_is_write() == 1'b1) begin
         evl_bus_req_user_t user_cmd;
         int                etl_idx;

         case (cmd)
            LD, LD_NO_MOD, LD_COOP: user_cmd = evl_bus_req_user_t'(evl_etlink_types_pkg::ET_LINK_REQ_Read);
            ATOMIC:                 user_cmd = evl_bus_req_user_t'($urandom_range(32)); // evl_bus_req_user_t'(evl_etlink_types_pkg::ET_LINK_REQ_Atomic);
            MESSAGE:                user_cmd = evl_bus_req_user_t'(evl_etlink_types_pkg::ET_LINK_REQ_MsgSendData);
            default:                begin
                                       etl_idx  = $urandom_range(5);
                                       user_cmd = etl_opcodes[etl_idx];
                                    end
         endcase
         req_desc.set_bus_req_user(evl_bus_req_user_t'(user_cmd));
      end
      else begin
         req_desc.set_bus_req_user(evl_bus_req_user_t'(0));
      end

      //
      // For atomics, we need to put additional information in the appropriate data quadword.
      //
      if (cmd == ATOMIC) begin
         evl_qword_t atomic_qword;

         atomic_qword = ((evl_qword_t'(req_desc.get_bus_atomic_conf())    & evl_qword_t'(`EVL_AXI_ATOMIC_CONF_MASK))    << `EVL_AXI_ATOMIC_CONF_SHIFT)   |
                        ((evl_qword_t'(`EVL_BUS_ATOMIC_CONF_L3_OP)        & evl_qword_t'(`EVL_AXI_ATOMIC_CONF_MASK))    << `EVL_AXI_ATOMIC_CONF_SHIFT)   |
                        ((evl_qword_t'(req_desc.get_bus_atomic_offset())  & evl_qword_t'(`EVL_AXI_ATOMIC_ADDR_MASK))    << `EVL_AXI_ATOMIC_ADDR_SHIFT)   |
                        ((evl_qword_t'(get_shire_id())                    & evl_qword_t'(`EVL_AXI_ATOMIC_SOURCE_MASK))  << `EVL_AXI_ATOMIC_SOURCE_SHIFT) |
                        ((evl_qword_t'(req_desc.get_bus_atomic_transid()) & evl_qword_t'(`EVL_AXI_ATOMIC_TRANSID_MASK)) << `EVL_AXI_ATOMIC_TRANSID_SHIFT);
         for (int ii = 0; ii < (`SC_ATOMIC_DATA_SIZE / (`EVL_QWORD_MSB + 1)); ii++) begin
            req_desc.set_req_qword(ii, req_desc.get_req_qword(ii));
         end
         req_desc.set_req_qword(`SC_ATOMIC_DATA_SIZE / (`EVL_QWORD_MSB + 1), atomic_qword);
         req_desc.set_paddr(req_desc.get_paddr() & const_line_paddr_mask);
         req_desc.set_bus_req_size(`EVL_LINE_ADDR_LSB);
      end
   endfunction : final_fixup


   //-----------------------------------------------------------------------------------------------
   //
   // Function: connect_my_bus_ports
   //
   // This function is used to connect all of the ports that go between this stimulus agent and its
   // bus agent.  Because the L3 stimulus agent supports loopbacks, we need to redefine this
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

      m_snd_rsp_ports.push_back(null);
      m_obs_req_exports.push_back(null);
      m_obs_req_fifos.push_back(null);
      m_snd_rsp_ports[0]   = new($sformatf("m_snd_rsp_port"), this);
      m_obs_req_exports[0] = new($sformatf("m_obs_req_export"), this);
      m_obs_req_fifos[0]   = new($sformatf("m_obs_req_fifo"), this);

      set_neighborhood_id(get_agent_id());
   endfunction : build_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string name_in = "evl_tosys_master_agent_axi", input uvm_component parent_in = null);
      super.new(name_in, parent_in, TOSYS_MASTER_STIM_AGENT);
      m_stimulus_agent = 1'b1;
      set_abstract_name("TOSYS_MASTER(STIM)");
      add_agent_alias(AXI_MASTER_STIM_AGENT);
      set_active_master();
   endfunction : new

endclass : evl_tosys_master_agent_axi

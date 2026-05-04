//--------------------------------------------------------------------------------------------------
//
// Class: evl_spio_e2a_master_agent_axi
//
// This class is an extension of evl_stim_agent_axi to define a master stim agent that acts like an
// SBM driving requests to the mesh.
//
class evl_spio_e2a_master_agent_axi extends evl_stim_agent_axi;

   `uvm_component_utils(evl_spio_e2a_master_agent_axi)


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
      add_int_req_act( ST_WA,               1'b1,   I,           AXQ_None,  St_Ignore,  Data_ReqWr,  BusWrite                                );

      add_int_req_act( LD,                  1'b0,   I,           AXQ_None,  St_Ignore,  Data_None,   BusRead                                 );
      add_int_req_act( ST,                  1'b0,   I,           AXQ_None,  St_Ignore,  Data_ReqWr,  BusWrite                                );
      add_int_req_act( LD_NO_MOD,           1'b0,   I,           AXQ_None,  St_Ignore,  Data_None,   BusRead                                 );
      add_int_req_act( ST_NO_MOD,           1'b0,   I,           AXQ_None,  St_Ignore,  Data_ReqWr,  BusWrite                                );
      add_int_req_act( ST_WA,               1'b0,   I,           AXQ_None,  St_Ignore,  Data_ReqWr,  BusWrite                                );

      add_int_req_act( ATOMIC,              1'b1,   I,           AXQ_None,  St_Ignore,  Data_ReqWr,  BusWrite                                );
      add_int_req_act( MESSAGE,             1'b1,   I,           AXQ_None,  St_Ignore,  Data_ReqWr,  evl_bus_cmd_t'(ET_LINK_REQ_MsgSendData) );

      add_int_req_act( ATOMIC,              1'b0,   I,           AXQ_None,  St_Ignore,  Data_ReqWr,  BusWrite                                );
      add_int_req_act( MESSAGE,             1'b0,   I,           AXQ_None,  St_Ignore,  Data_ReqWr,  evl_bus_cmd_t'(ET_LINK_REQ_MsgSendData) );

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
      add_bus_rsp_act( `EVL_AXI_REQ_READ,   I,           AXQ_None,   St_Ignore,  Data_ReqRd,  RspNull );
      add_bus_rsp_act( `EVL_AXI_REQ_WRITE,  I,           AXQ_None,   St_Ignore,  Data_None,   RspNull );
      add_bus_rsp_act( `EVL_AXI_REQ_READ,   I,           AXQ_Local,  St_Ignore,  Data_ReqRd,  RspNull );
      add_bus_rsp_act( `EVL_AXI_REQ_WRITE,  I,           AXQ_Local,  St_Ignore,  Data_None,   RspNull );
   endfunction : build_tables


   //----------------------------------------------------------------------------------------------
   //
   // Function: final_fixup
   //
   virtual function void final_fixup(input evl_trans_desc trans_desc, input evl_req_desc req_desc);
      evl_cmd_t cmd = trans_desc.get_cmd();

      req_desc.set_bus_prot(3'b100);

      if (cmd inside { LD_NO_MOD, ST_NO_MOD, ATOMIC, MESSAGE }) begin
         req_desc.set_bus_cache_attr(evl_bus_cache_attr_t'(4'b1101));
      end
      else begin
         req_desc.set_bus_cache_attr(evl_bus_cache_attr_t'(4'b1111));
      end

      if (req_desc.bus_cmd_is_write() == 1'b1) begin
         case (cmd)
            LD, LD_NO_MOD, LD_COOP: req_desc.set_bus_req_user(evl_bus_req_user_t'(evl_etlink_types_pkg::ET_LINK_REQ_Read));
            CACHE_EVICT:            req_desc.set_bus_req_user(evl_bus_req_user_t'(evl_etlink_types_pkg::ET_LINK_REQ_Evict));
            CACHE_EVICT_TO_MEM:     req_desc.set_bus_req_user(evl_bus_req_user_t'(evl_etlink_types_pkg::ET_LINK_REQ_EvictToMem));
            CACHE_FLUSH:            req_desc.set_bus_req_user(evl_bus_req_user_t'(evl_etlink_types_pkg::ET_LINK_REQ_Flush));
            CACHE_FLUSH_TO_MEM:     req_desc.set_bus_req_user(evl_bus_req_user_t'(evl_etlink_types_pkg::ET_LINK_REQ_FlushToMem));
            CACHE_PREFETCH:         req_desc.set_bus_req_user(evl_bus_req_user_t'(evl_etlink_types_pkg::ET_LINK_REQ_Prefetch));
            ATOMIC:                 req_desc.set_bus_req_user(evl_bus_req_user_t'(evl_etlink_types_pkg::ET_LINK_REQ_Atomic));
            MESSAGE:                req_desc.set_bus_req_user(evl_bus_req_user_t'(evl_etlink_types_pkg::ET_LINK_REQ_MsgSendData));
            default:                req_desc.set_bus_req_user(evl_bus_req_user_t'(evl_etlink_types_pkg::ET_LINK_REQ_Write));
         endcase
      end
      else begin
         req_desc.set_bus_req_user(evl_bus_req_user_t'(0));
      end
   endfunction : final_fixup


   //-----------------------------------------------------------------------------------------------
   //
   // Function: add_bank_to_req_id
   //
   virtual function evl_bus_req_id_t add_bank_to_req_id(input evl_bus_req_id_t bus_req_id, input evl_axi_bank_id_t bank_id);
       return bus_req_id;
   endfunction


   //-----------------------------------------------------------------------------------------------
   //
   // Function: build_phase
   //
   virtual function void build_phase(uvm_phase phase);
      super.build_phase(phase);
      m_snd_req_ports.push_back(null);
      m_obs_req_exports.push_back(null);
      m_obs_req_fifos.push_back(null);
      m_rcv_rsp_exports.push_back(null);
      m_rcv_rsp_fifos.push_back(null);
      m_snd_req_ports[0]   = new("m_snd_req_port", this);
      m_obs_req_exports[0] = new("m_obs_req_export", this);
      m_obs_req_fifos[0]   = new("m_obs_req_fifo", this);
      m_rcv_rsp_exports[0] = new("m_rcv_rsp_export", this);
      m_rcv_rsp_fifos[0]   = new("m_rcv_rsp_fifo", this);
   endfunction : build_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string name_in = "evl_spio_e2a_master_agent_axi", input uvm_component parent_in = null);
      super.new(name_in, parent_in, SPIO_E2A_MASTER_STIM_AGENT);
      m_stimulus_agent   = 1'b1;
      m_bus_req_id_limit = 15;
      m_active_limit     = 15;
      m_issue_limit      = 15;
      set_abstract_name("SPIO_E2A(STIM)");
      add_agent_alias(AXI_MASTER_STIM_AGENT);
      set_active_master();
   endfunction : new

endclass : evl_spio_e2a_master_agent_axi

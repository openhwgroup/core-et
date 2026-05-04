//--------------------------------------------------------------------------------------------------
//
// Class: evl_ust_master_agent_axi
//
// This class is an extension of evl_stim_agent_axi to define a master stim agent that acts like an
// L3 driving requests to the mesh.
//
class evl_ust_master_agent_axi extends evl_stim_agent_axi;

   `uvm_component_utils(evl_ust_master_agent_axi)


   //-----------------------------------------------------------------------------------------------
   //
   // Function: build_tag_list
   //
   virtual function void build_req_id_list();
      evl_axi_bank_id_t   bank_id = evl_axi_bank_id_t'(0);
      evl_axi_reqq_id_t   reqq_id = evl_axi_reqq_id_t'(0);
      evl_axi_bridge_id_t bridge_ids[$];
      int                 min_shire_masters[`MEM_MINSHIRE_SYS_MASTER_RANGE_ID];

      // Bridge ID = random
      //  No restriction on what the bits are supposed to be, only that the
      //  total width of the AXI_ID port is 17 bits so the upper 2 bits of the
      //  bridge_id here need to be 0
      // the mesh_mem master agent generates ~70 bridge_ids so let's do the same here
      repeat (70) begin
         evl_axi_bridge_id_t bridge_id;

         bridge_id = evl_axi_bridge_id_t'($urandom());
         bridge_id >>= 2;
         bridge_ids.push_back(bridge_id);
      end

      do begin
         reqq_id = evl_axi_reqq_id_t'(0);
         m_bus_req_id_queues[bank_id] = new();
         do begin
            for (int ii = 0; ii < bridge_ids.size(); ii++) begin
               evl_axi_bridge_id_t bridge_id;

               bridge_id = bridge_ids[ii];
               if (bridge_id != evl_axi_bridge_id_t'(m_shire_id)) begin
                  m_bus_req_id_queues[bank_id].push(evl_bus_req_id_t'({ bridge_id, reqq_id }));
               end
            end
            reqq_id += evl_axi_reqq_id_t'(1);
         end while (reqq_id != evl_axi_reqq_id_t'(0));
         bank_id += evl_axi_bank_id_t'(1);
      end while (bank_id != evl_axi_bank_id_t'(0));
   endfunction : build_req_id_list


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
      //              |--------------- Table Inputs ----------------||--------- Table Outputs --------|
      //               Cmd                  Model State  Qual         Tag Action  Data Action  Bus Rsp
      //               -------------------  -----------  -----------  ----------  -----------  -------
      add_bus_rsp_act( `EVL_AXI_REQ_READ,   I,           AXQ_Local,   St_Ignore,  Data_ReqRd,  RspNull );
      add_bus_rsp_act( `EVL_AXI_REQ_WRITE,  I,           AXQ_Local,   St_Ignore,  Data_None,   RspNull );
      add_bus_rsp_act( `EVL_AXI_REQ_READ,   I,           AXQ_Remote,  St_Ignore,  Data_ReqRd,  RspNull );
      add_bus_rsp_act( `EVL_AXI_REQ_WRITE,  I,           AXQ_Remote,  St_Ignore,  Data_None,   RspNull );
   endfunction : build_tables


   //----------------------------------------------------------------------------------------------
   //
   // Function: final_fixup
   //
   virtual function void final_fixup(input evl_trans_desc trans_desc, input evl_req_desc req_desc);
      evl_cmd_t cmd = trans_desc.get_cmd();

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
   function new(input string name_in = "evl_ust_master_agent_axi", input uvm_component parent_in = null);
      super.new(name_in, parent_in, UST_MASTER_STIM_AGENT);
      // ecc slows bandwidth so throttle the requests more
      if (`et_test_plusargs("ddr_ecc_en=1") != 0) begin
         m_active_limit = 8;
         m_issue_limit  = 8;
      end
      else begin
         m_active_limit = 16;
         m_issue_limit  = 16;
      end
      m_stimulus_agent = 1'b1;
      m_bank_mask      = ~evl_axi_bank_id_t'(0);
      set_abstract_name("UST_MASTER(STIM)");
      add_agent_alias(AXI_MASTER_STIM_AGENT);
      set_active_master();
   endfunction : new

endclass : evl_ust_master_agent_axi

//--------------------------------------------------------------------------------------------------
//
// Class: evl_l2tol3_master_agent_axi
//
// This class is an extension of evl_stim_agent_axi to define a master stim agent that acts like a
// TOL3 master driving requests to the mesh.
//
class evl_l2tol3_master_agent_axi extends evl_stim_agent_axi;

   `uvm_component_utils(evl_l2tol3_master_agent_axi)


   //-----------------------------------------------------------------------------------------------
   //
   // Member Variables and Objects
   //
   bit m_use_req_id_mods = 1'b0;


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
      //              |------------------ Table Inputs -------------------||------------------------- Table Outputs -------------------------|
      //               Cmd                  Cached  Model State  Qual       Tag Action  Data Action  Bus Command
      //               -------------------  ------  -----------  ---------  ----------  -----------  ----------------------------------------
      add_int_req_act( LD,                  1'b1,   I,           AXQ_None,  St_Ignore,  Data_None,   BusRead                                  );
      add_int_req_act( ST,                  1'b1,   I,           AXQ_None,  St_Ignore,  Data_ReqWr,  BusWrite                                 );
      add_int_req_act( LD_NO_MOD,           1'b1,   I,           AXQ_None,  St_Ignore,  Data_None,   BusRead                                  );
      add_int_req_act( ST_NO_MOD,           1'b1,   I,           AXQ_None,  St_Ignore,  Data_ReqWr,  BusWrite                                 );
      add_int_req_act( LD_COOP,             1'b1,   I,           AXQ_None,  St_Ignore,  Data_None,   BusRead                                  );
      add_int_req_act( ST_WA,               1'b1,   I,           AXQ_None,  St_Ignore,  Data_ReqWr,  BusWrite                                 );

      add_int_req_act( LD,                  1'b0,   I,           AXQ_None,  St_Ignore,  Data_None,   BusRead                                  );
      add_int_req_act( ST,                  1'b0,   I,           AXQ_None,  St_Ignore,  Data_ReqWr,  BusWrite                                 );
      add_int_req_act( LD_NO_MOD,           1'b0,   I,           AXQ_None,  St_Ignore,  Data_None,   BusRead                                  );
      add_int_req_act( ST_NO_MOD,           1'b0,   I,           AXQ_None,  St_Ignore,  Data_ReqWr,  BusWrite                                 );
      add_int_req_act( LD_COOP,             1'b0,   I,           AXQ_None,  St_Ignore,  Data_None,   BusRead                                  );
      add_int_req_act( ST_WA,               1'b0,   I,           AXQ_None,  St_Ignore,  Data_ReqWr,  BusWrite                                 );

      add_int_req_act( CACHE_EVICT,         1'b1,   I,           AXQ_None,  St_Ignore,  Data_Create,  BusWrite                                );
      add_int_req_act( CACHE_EVICT_TO_MEM,  1'b1,   I,           AXQ_None,  St_Ignore,  Data_ReqWr,   BusWrite                                );
      add_int_req_act( CACHE_FLUSH,         1'b1,   I,           AXQ_None,  St_Ignore,  Data_Create,  BusWrite                                );
      add_int_req_act( CACHE_FLUSH_TO_MEM,  1'b1,   I,           AXQ_None,  St_Ignore,  Data_ReqWr,   BusWrite                                );
      add_int_req_act( CACHE_PREFETCH,      1'b1,   I,           AXQ_None,  St_Ignore,  Data_Create,  BusWrite                                );

      add_int_req_act( ATOMIC,              1'b1,   I,           AXQ_None,  St_Ignore,  Data_ReqWr,   BusWrite                                );
      add_int_req_act( MESSAGE,             1'b1,   I,           AXQ_None,  St_Ignore,  Data_ReqWr,   evl_bus_cmd_t'(ET_LINK_REQ_MsgSendData) );

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
      add_bus_rsp_act( `EVL_AXI_REQ_READ,   I,           AXQ_Local,  St_Ignore,  Data_ReqRd,  RspNull );
      add_bus_rsp_act( `EVL_AXI_REQ_WRITE,  I,           AXQ_Local,  St_Ignore,  Data_None,   RspNull );
   endfunction : build_tables


   //----------------------------------------------------------------------------------------------
   //
   // Function: final_fixup
   //
   virtual function void final_fixup(input evl_trans_desc trans_desc, input evl_req_desc req_desc);
      evl_cmd_t            cmd = trans_desc.get_cmd();
      et_link_req_opcode_t etl_opcodes[5:0] = { evl_etlink_types_pkg::ET_LINK_REQ_Write, 
                                                evl_etlink_types_pkg::ET_LINK_REQ_Evict, 
                                                evl_etlink_types_pkg::ET_LINK_REQ_EvictToMem, 
                                                evl_etlink_types_pkg::ET_LINK_REQ_Flush, 
                                                evl_etlink_types_pkg::ET_LINK_REQ_FlushToMem, 
                                                evl_etlink_types_pkg::ET_LINK_REQ_Prefetch };

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

      if (get_int_param(pk_noc_l3_miss, 0) > 0) begin
         trans_desc.set_int_param_by_name("pk_noc_l3_miss", 1);
      end

   endfunction : final_fixup


   //-----------------------------------------------------------------------------------------------
   //
   // Function: build_phase
   //
   virtual function void build_phase(uvm_phase phase);
      super.build_phase(phase);
      for (int ii = 0; ii < m_num_l3_ports; ii++) begin
         m_snd_req_ports.push_back(null);
         m_snd_rsp_ports.push_back(null);
         m_obs_req_exports.push_back(null);
         m_obs_req_fifos.push_back(null);
         m_rcv_rsp_exports.push_back(null);
         m_rcv_rsp_fifos.push_back(null);
         m_snd_req_ports[ii]   = new($sformatf("m_snd_req_port_%0d", ii), this);
         m_snd_rsp_ports[ii]   = new($sformatf("m_snd_rsp_port_%0d", ii), this);
         m_obs_req_exports[ii] = new($sformatf("m_obs_req_export_%0d", ii), this);
         m_obs_req_fifos[ii]   = new($sformatf("m_obs_req_fifo_%0d", ii), this);
         m_rcv_rsp_exports[ii] = new($sformatf("m_rcv_rsp_export_%0d", ii), this);
         m_rcv_rsp_fifos[ii]   = new($sformatf("m_rcv_rsp_fifo_%0d", ii), this);
      end
      set_neighborhood_id(get_agent_id());
   endfunction : build_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Function: select_req_port
   //
   virtual function int select_req_port(input evl_req_desc req_desc, input evl_axi_source_t axi_source);
      if (m_num_l3_port_mask <= 0) begin
         return 0;
      end
      return (m_num_l3_port_mask & m_global_attrs.get_axi_port_index_from_address(req_desc.get_paddr()));
   endfunction : select_req_port


   //-----------------------------------------------------------------------------------------------
   //
   // Function: select_rsp_port
   //
   virtual function int select_rsp_port(input evl_req_desc req_desc);
      return req_desc.get_int_param(pk_tlm_req_port, 0);
   endfunction : select_rsp_port


   //-----------------------------------------------------------------------------------------------
   //
   //
   //
   virtual function bit check_active_req_ids(input evl_bus_req_id_t bus_req_id, input evl_axi_bank_id_t bank_id);
      bank_id = bank_id % m_num_l3_ports;
      if (m_allow_duplicate_sent_ids == 1'b1) begin
         if (m_use_req_id_mods == 1'b1) begin
            return (m_active_bus_req_ids[bank_id].exists(bus_req_id) && (&m_active_bus_req_ids[bank_id][bus_req_id] == 1'b1));
         end
         else begin
            return (m_active_bus_req_ids[0].exists(bus_req_id) && (&m_active_bus_req_ids[0][bus_req_id] == 1'b1));
         end
      end
      else begin
         if (m_use_req_id_mods == 1'b1) begin
            return (m_active_bus_req_ids[bank_id].exists(bus_req_id) && (m_active_bus_req_ids[bank_id][bus_req_id][0] == 1'b1));
         end
         else begin
            return (m_active_bus_req_ids[0].exists(bus_req_id) && (m_active_bus_req_ids[0][bus_req_id][0] == 1'b1));
         end
      end
   endfunction


   //-----------------------------------------------------------------------------------------------
   //
   // Function: add_bank_to_req_id
   //
   virtual function evl_bus_req_id_t add_bank_to_req_id(input evl_bus_req_id_t bus_req_id, input evl_axi_bank_id_t bank_id);
      evl_axi_reqq_id_t   reqq_id;
      evl_axi_bridge_id_t bridge_id;

      if (m_use_req_id_mods == 1'b1) begin
         return bus_req_id;
      end
      else begin
         { bridge_id, reqq_id } = bus_req_id[CONCAT_AXI_SHORT_REQID_SIZE - 1:0];
         return evl_bus_req_id_t'({ bridge_id, bank_id, reqq_id });
      end
   endfunction

   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_bus_req_id_bit
   //
   virtual function void set_bus_req_id_bit(input evl_bus_req_id_t bus_req_id, input evl_axi_bank_id_t bank_id);
      bank_id = bank_id % m_num_l3_ports;
      if (m_allow_duplicate_sent_ids == 1'b1) begin
         if (m_use_req_id_mods == 1'b1) begin
            m_active_bus_req_ids[bank_id][bus_req_id] = {m_active_bus_req_ids[bank_id][bus_req_id][6:0], 1'b1};
         end
         else begin
            m_active_bus_req_ids[0][bus_req_id] = {m_active_bus_req_ids[0][bus_req_id][6:0], 1'b1};
         end
      end
      else begin
         if (m_use_req_id_mods == 1'b1) begin
            m_active_bus_req_ids[bank_id][bus_req_id] = 1'b1;
         end
         else begin
            m_active_bus_req_ids[0][bus_req_id] = 1'b1;
         end
      end
   endfunction


   //-----------------------------------------------------------------------------------------------
   //
   // Function: unset_bus_req_id_bit
   //
   virtual function void unset_bus_req_id_bit(input evl_bus_req_id_t bus_req_id, input evl_axi_bank_id_t bank_id);
      bank_id = bank_id % m_num_l3_ports;
      if (m_allow_duplicate_sent_ids == 1'b1) begin
         if (m_use_req_id_mods == 1'b1) begin
            m_active_bus_req_ids[bank_id][bus_req_id] = {1'b0, m_active_bus_req_ids[bank_id][bus_req_id][7:1]};
         end
         else begin
            m_active_bus_req_ids[0][bus_req_id] = {1'b0, m_active_bus_req_ids[0][bus_req_id][7:1]};
         end
      end
      else begin
         if (m_use_req_id_mods == 1'b1) begin
            m_active_bus_req_ids[bank_id][bus_req_id] = 1'b0;
         end
         else begin
            m_active_bus_req_ids[0][bus_req_id] = 1'b0;
         end
      end
   endfunction


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

      if (bus_verif_comp == null) begin
         return;
      end
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
   function new(input string name_in = "evl_l2tol3_master_agent_axi", input uvm_component parent_in = null);
      super.new(name_in, parent_in, L2TOL3_MASTER_STIM_AGENT);
      if (`et_test_plusargs("l3_perf_traffic") != 0) begin
         m_active_limit      = 8192;
         m_issue_limit       = 8192;
         m_bus_req_id_limit  = 512;
         m_use_req_id_mods   = 1'b1;
         m_allow_duplicate_sent_ids = 1'b1;
      end
      m_stimulus_agent = 1'b1;
      set_abstract_name("L2TOL3_MASTER(STIM)");
      add_agent_alias(AXI_MASTER_STIM_AGENT);
      set_active_master();
   endfunction : new

endclass : evl_l2tol3_master_agent_axi

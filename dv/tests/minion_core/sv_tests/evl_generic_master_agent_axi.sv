//--------------------------------------------------------------------------------------------------
//
// Class: evl_generic_master_agent_axi
//
// This class is an extension of evl_stim_agent_axi to define a master stim agent that acts like an
// L3 driving requests to the mesh.
//
class evl_generic_master_agent_axi extends evl_stim_agent_axi;

   `uvm_component_utils(evl_generic_master_agent_axi)


   //-----------------------------------------------------------------------------------------------
   //
   // Function: build_tag_list
   //
   virtual function void build_req_id_list();
      evl_axi_bank_id_t   bank_id   = evl_axi_bank_id_t'(0);
      evl_axi_reqq_id_t   reqq_id   = evl_axi_reqq_id_t'(0);
      evl_axi_bridge_id_t bridge_id = evl_axi_bridge_id_t'(0);

      do begin
         reqq_id = evl_axi_reqq_id_t'(0);
         m_bus_req_id_queues[bank_id] = new();
         do begin
            bridge_id = evl_axi_bridge_id_t'(0);
            do begin
               //
               // There is a special case here to prevent overlap with forwarded IDs from the
               // evl_minion_shire_slave_agent_axi slave agent.  Bridge IDs 0x08 to 0x0f are
               // reserved for loopbacks.
               //
               if ( (bridge_id != evl_axi_bridge_id_t'(m_shire_id)) &&
                    (bridge_id != ~evl_axi_bridge_id_t'(0)) &&
                    ((bridge_id < evl_axi_bridge_id_t'(8)) || (bridge_id > evl_axi_bridge_id_t'(15))) ) begin
                  m_bus_req_id_queues[bank_id].push(evl_bus_req_id_t'({ bridge_id, reqq_id }));
               end
               bridge_id += evl_axi_bridge_id_t'(1);
            end while (bridge_id != evl_axi_bridge_id_t'(0));
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
      //              |--------------- Table Inputs ----------------||--------- Table Outputs ---------|
      //               Cmd                  Model State  Qual         Tag Action  Data Action  Bus Rsp
      //               -------------------  -----------  -----------  ----------  -----------  --------
      add_bus_rsp_act( `EVL_AXI_REQ_READ,   I,           AXQ_Local,   St_Ignore,  Data_ReqRd,  RspNull  );
      add_bus_rsp_act( `EVL_AXI_REQ_WRITE,  I,           AXQ_Local,   St_Ignore,  Data_None,   RspNull  );
   endfunction : build_tables


   //----------------------------------------------------------------------------------------------
   //
   // Function: final_fixup
   //
   //
   virtual function void final_fixup(input evl_trans_desc trans_desc, input evl_req_desc req_desc);
      evl_cmd_t cmd = trans_desc.get_cmd();

      if (req_desc.bus_cmd_is_write() == 1'b1) begin
         if (cmd inside { LD, LD_NO_MOD }) begin
            req_desc.set_bus_req_user(evl_bus_req_user_t'(evl_etlink_types_pkg::ET_LINK_REQ_Read));
         end
         else begin
            req_desc.set_bus_req_user(evl_bus_req_user_t'(evl_etlink_types_pkg::ET_LINK_REQ_Write));
         end
      end
      else begin
         req_desc.set_bus_req_user(evl_bus_req_user_t'(0));
      end

      if (cmd inside { LD_NO_MOD, ST_NO_MOD, ATOMIC, MESSAGE }) begin
         req_desc.set_bus_cache_attr(evl_bus_cache_attr_t'(4'b1101));
      end
   endfunction : final_fixup


   //-----------------------------------------------------------------------------------------------
   //
   // Function: build_phase
   //
   virtual function void build_phase(uvm_phase phase);
      super.build_phase(phase);
      m_snd_req_ports.push_back(null);
      m_rcv_rsp_exports.push_back(null);
      m_rcv_rsp_fifos.push_back(null);
      m_snd_req_ports[0]   = new("m_snd_req_port",   this);
      m_rcv_rsp_exports[0] = new("m_rcv_rsp_export", this);
      m_rcv_rsp_fifos[0]   = new("m_rcv_rsp_fifo",   this);
   endfunction : build_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string name_in = "evl_generic_master_agent_axi", input uvm_component parent_in = null);
      super.new(name_in, parent_in, GENERIC_AXI_MASTER_STIM_AGENT);
      m_stimulus_agent = 1'b1;
      set_abstract_name("MASTER(STIM)");
      add_agent_alias(AXI_MASTER_STIM_AGENT);
      set_active_master();
   endfunction : new

endclass : evl_generic_master_agent_axi

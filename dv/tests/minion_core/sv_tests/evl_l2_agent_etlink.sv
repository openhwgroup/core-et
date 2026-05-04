//--------------------------------------------------------------------------------------------------
//
// Class: evl_l2_agent_etlink
//
class evl_l2_agent_etlink extends evl_stim_agent_etlink;

   `uvm_component_utils(evl_l2_agent_etlink)

   //-----------------------------------------------------------------------------------------------
   //
   // Static Member Variables and Objects
   //
   `include "dv/common/base/evl_mem_map_pk_inc.svh"


   //-----------------------------------------------------------------------------------------------
   //
   // Member Variables and Objects
   //


   //-----------------------------------------------------------------------------------------------
   //
   // Function: build_tables
   //
   virtual function void build_tables();

      //  cmd: LD, ST, ST_WA, EVICT, CACHE_LOCK, CACHE_UNLOCK, CACHE_EVICT, CACHE_FLUSH, CACHE_PREFETCH, CACHE_L3_FILL, MESSAGE, ATOMIC
      //  model_st:   I, S, E, M, IE_Bsy, IM_Bsy, EI_Bsy, MI_Bsy, St_Conflict, St_Ignore
      //  tag_action: I, S, E, M, IE_Bsy, IM_Bsy, EI_Bsy, MI_Bsy, St_Conflict, St_Ignore
      //  data_action: Data_None, Data_ReqWr, Data_ReqWrBack, Data_ReqRd, Data_CacheFill
      //
      //             |--------------------------------- Table Inputs ------------------------------||-------------- Table Outputs --------------------|
      //               cmd                                                       model_st     qual   tag_action   data_action        cmd
      add_bus_req_act( {`EVL_ETLINK_REQ_READ },                                { St_Ignore }, { 1 }, St_Ignore,   Data_MemRd,        `EVL_ETLINK_RSP_ACK_DATA     );
      add_bus_req_act( {`EVL_ETLINK_REQ_WRITE, `EVL_ETLINK_REQ_WRITE_AROUND }, { St_Ignore }, { 1 }, St_Ignore,   Data_MemWr,        `EVL_ETLINK_RSP_ACK          );
      add_bus_req_act( {`EVL_ETLINK_REQ_FLUSH, `EVL_ETLINK_REQ_EVICT },        { St_Ignore }, { 1 }, St_Ignore,   Data_None,         `EVL_ETLINK_RSP_ACK          );
      add_bus_req_act( {`EVL_ETLINK_REQ_LOCK, `EVL_ETLINK_REQ_UNLOCK },        { St_Ignore }, { 1 }, St_Ignore,   Data_None,         `EVL_ETLINK_RSP_ACK          );
      add_bus_req_act( {`EVL_ETLINK_REQ_SCP_FILL, `EVL_ETLINK_REQ_PREFETCH },  { St_Ignore }, { 1 }, St_Ignore,   Data_None,         `EVL_ETLINK_RSP_ACK          );
      // FUTURE: Dimitris Tsaliagos Probably will have to get the sideband information (1 bit) to send to the local minion (in case the message source is the TBOX/RBOX)
      add_bus_req_act( {`EVL_ETLINK_REQ_MSG_SEND_DATA },                       { St_Ignore }, { 0 }, St_Ignore,   Data_None,         `EVL_ETLINK_RSP_NONE         );
      // FUTURE: Dimitris Tsaliagos Check with someone that the following data action is indeed correct and just forwards the data along with the qwen to the response
      add_bus_req_act( {`EVL_ETLINK_REQ_MSG_SEND_DATA },                       { St_Ignore }, { 1 }, St_Ignore,   Data_ForwardMsg,   `EVL_ETLINK_RSP_MSG_RCV_DATA );
      add_bus_req_act( {`EVL_ETLINK_REQ_ATOMIC },                              { St_Ignore }, { 0 }, St_Ignore,   Data_MemWrRdNorm,  `EVL_ETLINK_RSP_ACK          );
      add_bus_req_act( {`EVL_ETLINK_REQ_ATOMIC },                              { St_Ignore }, { 1 }, St_Ignore,   Data_MemWrRd,      `EVL_ETLINK_RSP_ACK_DATA     );
      // FUTURE: figure out how we want to do message responses - perhaps as some reaction to receiving messages

   endfunction : build_tables


   //-----------------------------------------------------------------------------------------------
   //
   // Function: build_phase
   //
   virtual function void build_phase(uvm_phase phase);
      super.build_phase(phase);
      m_snd_rsp_ports.push_back(null);
      m_obs_req_exports.push_back(null);
      m_obs_req_fifos.push_back(null);
      m_snd_rsp_ports[0]   = new("m_snd_rsp_port", this);
      // Since we're a slave agent, we need to receive observed requests:
      m_obs_req_exports[0] = new("m_obs_req_export", this);
      m_obs_req_fifos[0]   = new("m_obs_req_fifo", this);
      set_neighborhood_id(get_agent_id());
   endfunction : build_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Function: post_process_received_request()
   //
   virtual function void post_process_received_request(inout evl_req_desc req_desc);
      bit is_bus_err    = 1'b0;
      evl_paddr_t paddr = req_desc.get_paddr();
      if (addr_in_region(pk_scp_region, paddr)) begin
         if (paddr[22:0] >= 23'h280000) is_bus_err = 1'b1;
      end
      else if (addr_in_region(pk_esr_region, paddr)) begin
         // PRM-19: 2.4 The ESR Region (4G to 8G)
         if ((paddr[29:22] >= 0) && (paddr[29:22] <= 34)) is_bus_err = 1'b0;
         else if ((paddr[29:22] >= 232) && (paddr[29:22] <= 239)) is_bus_err = 1'b0;
         else if  (paddr[29:22] == 253) is_bus_err = 1'b0;
         else if  (paddr[29:22] == 254) is_bus_err = 1'b0;
         else if  (paddr[29:22] == 255) is_bus_err = 1'b0;
         else is_bus_err = 1'b1;
      end
      else if (addr_in_region(pk_mram_region, paddr)) begin
         is_bus_err = (paddr >= 64'h08800000000) ? 1 : 0;
      end
      else if (addr_in_regions({pk_io_region, pk_reserved_region}, paddr)) begin
         is_bus_err = 1'b1;
      end

      if (is_bus_err == 1'b1) begin
         req_desc.set_abstract_rsp_type(RspErr);
         req_desc.set_bus_rsp_valid(0, get_cycle_count());
         //req_desc.set_rsp_data('0);
         req_desc.set_bus_rsp_qword_en(4'b0000);
      end
   endfunction : post_process_received_request


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string name_in = "evl_l2_agent_etlink", input uvm_component parent_in = null);
      super.new(name_in, parent_in, SHIRE_CACHE_STIM_AGENT);
      m_memory_agent = 1'b1;
      add_agent_alias(MEM_STIM_AGENT);
      add_agent_alias(MEM_AGENT);
      add_agent_alias(ETLINK_SLAVE_STIM_AGENT);
      set_abstract_name("L2(STIM)");
   endfunction : new

endclass : evl_l2_agent_etlink


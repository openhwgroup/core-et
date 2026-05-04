//--------------------------------------------------------------------------------------------------
//
// Class: evl_neigh_agent_etlink
//
// This class is an extension of evl_stim_agent_etlink to define a master stim agent that acts like
// a single neighborhood.
//
class evl_neigh_agent_etlink extends evl_stim_agent_etlink;

   `uvm_component_utils(evl_neigh_agent_etlink)

   `evl_param_key(pk_coop_rsp_exp)

   //-----------------------------------------------------------------------------------------------
   //
   // Static Member Variables and Objects
   //


   //-----------------------------------------------------------------------------------------------
   //
   // Member Variables and Objects
   //


   //-----------------------------------------------------------------------------------------------
   //
   // Function: build_tables
   //
   virtual function void build_tables();

      // Actions to take when we receive a request from the simulus.
      //
      // Int Req Table Definitions:
      // Cmd: The command from the stimulus world. These are generic because they will be used
      //      across many types of interfaces.
      // Cached: Is this request cached or uncached.
      // CacheState: The current cache state of the cache model inside this agent.
      // Qual: optional qualifier that is currently unused.
      // TagAction: What needs to happen to the state of the cache based on this transaction.
      // DataAction: What needs to happen to the Data based on this transaction.
      // BusCmd: What (if anything) needs to be driven on the bus based on this transaction.
      //
      // DataAction Definitions:
      // Data_ReqWr: Copy write data from the trans_desc to the req_desc that is being sent to the driver.
      //
      //             |--------------- Table Inputs ---------------||------- Table Outputs ----------------|
      //               Cmd           Cached     CacheState   Qual    TagAction  DataAction   BusCmd
      add_int_req_act( { LD },       { 1'b1 },  { I },       { 0 }, I,         Data_None,  `EVL_ETLINK_REQ_READ  );
      add_int_req_act( { LD },       { 1'b1 },  { I },       { 1 }, I,         Data_None,  `EVL_ETLINK_REQ_READ  );
      add_int_req_act( { LD },       { 1'b1 },  { E, M },    { 0 }, St_Ignore, Data_None,  `EVL_Bus_Idle  );
      add_int_req_act( { LD },       { 1'b1 },  { E, M },    { 1 }, St_Ignore, Data_None,  `EVL_Bus_Idle  );
      add_int_req_act( { LD_COOP },  { 1'b1 },  { I },       { 0 }, I,         Data_None,  `EVL_ETLINK_REQ_READCOOP  );
      add_int_req_act( { LD_COOP },  { 1'b1 },  { I },       { 1 }, I,         Data_None,  `EVL_ETLINK_REQ_READCOOP  );
      add_int_req_act( { LD_COOP },  { 1'b1 },  { E, M },    { 0 }, St_Ignore, Data_None,  `EVL_Bus_Idle  );
      add_int_req_act( { LD_COOP },  { 1'b1 },  { E, M },    { 1 }, St_Ignore, Data_None,  `EVL_Bus_Idle  );
      add_int_req_act( { ST },       { 1'b1 },  { I },       { 0 }, I,         Data_ReqWr, `EVL_ETLINK_REQ_WRITE );
      add_int_req_act( { ST },       { 1'b1 },  { I },       { 1 }, I,         Data_ReqWr, `EVL_ETLINK_REQ_WRITE );
      add_int_req_act( { ST },       { 1'b1 },  { E, M },    { 0 }, St_Ignore, Data_None,  `EVL_Bus_Idle  );
      add_int_req_act( { ST },       { 1'b1 },  { E, M },    { 1 }, St_Ignore, Data_None,  `EVL_Bus_Idle  );
      add_int_req_act( { EVICT },    { 1'b1 },  { E, I },    { 0 }, I,         Data_None,  `EVL_Bus_Idle  );
      add_int_req_act( { EVICT },    { 1'b1 },  { E, I },    { 1 }, I,         Data_None,  `EVL_Bus_Idle  );
      add_int_req_act( { EVICT },    { 1'b1 },  { M },       { 0 }, I,         Data_ReqWr, `EVL_ETLINK_REQ_WRITE );
      add_int_req_act( { EVICT },    { 1'b1 },  { M },       { 1 }, I,         Data_ReqWr, `EVL_ETLINK_REQ_WRITE );
      add_int_req_act( { ST_WA },    { 1'b1 },  { I },       { 0 }, I,         Data_ReqWr, `EVL_ETLINK_REQ_WRITE_AROUND);
      add_int_req_act( { ST_WA },    { 1'b1 },  { I },       { 1 }, I,         Data_ReqWr, `EVL_ETLINK_REQ_WRITE_AROUND);
      add_int_req_act( { ST_WA },    { 1'b1 },  { E, M },    { 0 }, St_Ignore, Data_None,  `EVL_Bus_Idle  );
      add_int_req_act( { ST_WA },    { 1'b1 },  { E, M },    { 1 }, St_Ignore, Data_None,  `EVL_Bus_Idle  );

      add_int_req_act( { LD },       { 1'b0 },  { I, E, M }, { 0 }, St_Ignore, Data_None,  `EVL_ETLINK_REQ_READ  );
      add_int_req_act( { LD },       { 1'b0 },  { I, E, M }, { 1 }, St_Ignore, Data_None,  `EVL_ETLINK_REQ_READ  );
      add_int_req_act( { ST },       { 1'b0 },  { I, E, M }, { 0 }, St_Ignore, Data_ReqWr, `EVL_ETLINK_REQ_WRITE );
      add_int_req_act( { ST },       { 1'b0 },  { I, E, M }, { 1 }, St_Ignore, Data_ReqWr, `EVL_ETLINK_REQ_WRITE );

      add_int_req_act( { CACHE_LOCK },          { 1'b1 }, { I, E, M },    { 0 }, St_Ignore, Data_None,   `EVL_ETLINK_REQ_LOCK);
      add_int_req_act( { CACHE_LOCK },          { 1'b1 }, { I, E, M },    { 1 }, St_Ignore, Data_None,   `EVL_ETLINK_REQ_LOCK);
      add_int_req_act( { CACHE_UNLOCK },        { 1'b1 }, { I, E, M },    { 0 }, St_Ignore, Data_None,   `EVL_ETLINK_REQ_UNLOCK);
      add_int_req_act( { CACHE_UNLOCK },        { 1'b1 }, { I, E, M },    { 1 }, St_Ignore, Data_None,   `EVL_ETLINK_REQ_UNLOCK);
      add_int_req_act( { CACHE_EVICT },         { 1'b1 }, { I, E, M },    { 0 }, St_Ignore, Data_None,   `EVL_ETLINK_REQ_EVICT);
      add_int_req_act( { CACHE_EVICT },         { 1'b1 }, { I, E, M },    { 1 }, St_Ignore, Data_None,   `EVL_ETLINK_REQ_EVICT);
      add_int_req_act( { CACHE_EVICT_TO_MEM },  { 1'b1 }, { I, E, M },    { 0 }, St_Ignore, Data_None,   `EVL_ETLINK_REQ_EVICT);
      add_int_req_act( { CACHE_EVICT_TO_MEM },  { 1'b1 }, { I, E, M },    { 1 }, St_Ignore, Data_None,   `EVL_ETLINK_REQ_EVICT);
      add_int_req_act( { CACHE_FLUSH },         { 1'b1 }, { I, E, M },    { 0 }, St_Ignore, Data_None,   `EVL_ETLINK_REQ_FLUSH);
      add_int_req_act( { CACHE_FLUSH },         { 1'b1 }, { I, E, M },    { 1 }, St_Ignore, Data_None,   `EVL_ETLINK_REQ_FLUSH);
      add_int_req_act( { CACHE_FLUSH_TO_MEM },  { 1'b1 }, { I, E, M },    { 0 }, St_Ignore, Data_None,   `EVL_ETLINK_REQ_FLUSH);
      add_int_req_act( { CACHE_FLUSH_TO_MEM },  { 1'b1 }, { I, E, M },    { 1 }, St_Ignore, Data_None,   `EVL_ETLINK_REQ_FLUSH);
      add_int_req_act( { CACHE_PREFETCH },      { 1'b1 }, { I, E, M },    { 0 }, St_Ignore, Data_None,   `EVL_ETLINK_REQ_PREFETCH);
      add_int_req_act( { CACHE_PREFETCH },      { 1'b1 }, { I, E, M },    { 1 }, St_Ignore, Data_None,   `EVL_ETLINK_REQ_PREFETCH);
      add_int_req_act( { CACHE_SCP_FILL },      { 1'b1 }, { I, E, M },    { 0 }, St_Ignore, Data_None,   `EVL_ETLINK_REQ_SCP_FILL);
      add_int_req_act( { CACHE_SCP_FILL },      { 1'b1 }, { I, E, M },    { 1 }, St_Ignore, Data_None,   `EVL_ETLINK_REQ_SCP_FILL);

      add_int_req_act( { MESSAGE },       { 1'b0, 1'b1 }, { I, E, M },    { 0 }, St_Ignore, Data_ReqWr,  `EVL_ETLINK_REQ_MSG_SEND_DATA);
      add_int_req_act( { MESSAGE },       { 1'b0, 1'b1 }, { I, E, M },    { 1 }, St_Ignore, Data_ReqWr,  `EVL_ETLINK_REQ_MSG_SEND_DATA);

      add_int_req_act( { ATOMIC },        { 1'b0, 1'b1 }, { I, E, M },    { 0 }, St_Ignore, Data_ReqWr,  `EVL_ETLINK_REQ_ATOMIC);
      add_int_req_act( { ATOMIC },        { 1'b0, 1'b1 }, { I, E, M },    { 1 }, St_Ignore, Data_ReqWr,  `EVL_ETLINK_REQ_ATOMIC);


      // Actions to take when a response comes back:
      //
      // Bus Response Table Definitions:
      // Cmd: The response opcode that was observed on the bus
      // CacheState: The current state of the cache model inside this agent.
      // Qual: option qualifier that is currently unused
      // TagAction: What needs to happen to the state of the cache based on this resposne?
      // DataAction: What needs to happen to the Data based on this response?
      //
      // DataAction Definitions:
      // Data_ReqRd: Copy the data from the req_desc to the trans_desc so that the original test has access to it.
      //
      //              |---------------------------- Table Inputs -----------------------------||----- Table Outputs ------|
      //               ET Link Command                                  Model State     Qual    Tag Action  Data Action
      //               -----------------------------------------------  --------------  ------  ----------  --------------
      add_bus_rsp_act( {evl_bus_cmd_t'(`EVL_ETLINK_REQ_READ)},          { St_Ignore },  { 0 },  St_Ignore,  Data_ReqRd     );
      add_bus_rsp_act( {evl_bus_cmd_t'(`EVL_ETLINK_REQ_READCOOP)},      { St_Ignore },  { 0 },  St_Ignore,  Data_ReqRd     );
      add_bus_rsp_act( {evl_bus_cmd_t'(`EVL_ETLINK_REQ_ATOMIC)},        { St_Ignore },  { 0 },  St_Ignore,  Data_ReqRdNorm );

      add_bus_rsp_act( {evl_bus_cmd_t'(`EVL_ETLINK_REQ_WRITE)},         { St_Ignore },  { 0 },  St_Ignore,  Data_None      );
      add_bus_rsp_act( {evl_bus_cmd_t'(`EVL_ETLINK_REQ_WRITE_AROUND)},  { St_Ignore },  { 0 },  St_Ignore,  Data_None      );
      add_bus_rsp_act( {evl_bus_cmd_t'(`EVL_ETLINK_REQ_FLUSH)},         { St_Ignore },  { 0 },  St_Ignore,  Data_None      );
      add_bus_rsp_act( {evl_bus_cmd_t'(`EVL_ETLINK_REQ_EVICT)},         { St_Ignore },  { 0 },  St_Ignore,  Data_None      );
      add_bus_rsp_act( {evl_bus_cmd_t'(`EVL_ETLINK_REQ_LOCK)},          { St_Ignore },  { 0 },  St_Ignore,  Data_None      );
      add_bus_rsp_act( {evl_bus_cmd_t'(`EVL_ETLINK_REQ_UNLOCK)},        { St_Ignore },  { 0 },  St_Ignore,  Data_None      );
      add_bus_rsp_act( {evl_bus_cmd_t'(`EVL_ETLINK_REQ_SCP_FILL)},      { St_Ignore },  { 0 },  St_Ignore,  Data_None      );
      add_bus_rsp_act( {evl_bus_cmd_t'(`EVL_ETLINK_REQ_PREFETCH)},      { St_Ignore },  { 0 },  St_Ignore,  Data_None      );
   endfunction : build_tables


   //----------------------------------------------------------------------------------------------
   //
   // Function: preprocess_trans_desc
   //
   virtual function void preprocess_trans_desc(input evl_trans_desc trans_desc);
      super.preprocess_trans_desc(trans_desc);
      if (trans_desc.get_cmd() == LD_COOP) begin
         evl_bus_neigh_coop_mask_t neigh_coop_mask;
         int                       num_responses;

         // "coop_target" is what is generated by the stimulus world, but it needs to
         // be copied over to become the "neigh_coop_mask" since that's what is interesting to the shire cache
         // Also, the neigh that is requesting the ReadCoop must always get a response
         // (the rtl has an assertion assuring that the neigh does this)
         neigh_coop_mask = trans_desc.get_bus_coop_target() | (1'b1 << get_agent_id());
         trans_desc.set_bus_neigh_coop_mask(neigh_coop_mask);
         // while we're here, use the neigh coop mask to find out how many
         // responses will be expected by the various neighs
         for (int ii = 0; ii < `NUM_NEIGH; ii++) begin
            if (neigh_coop_mask[ii] == 1'b1) begin
               num_responses++;
            end
         end
         `evl_log_mdesc(UVM_HIGH, VMOD_GLOBAL, get_abstract_name(), trans_desc, $sformatf("setting coop_rsp_exp to %0d for this coop read", num_responses))
         trans_desc.set_int_param(pk_coop_rsp_exp, num_responses);
      end
   endfunction : preprocess_trans_desc


   //-----------------------------------------------------------------------------------------------
   //
   // Function: build_req_id_list
   //
   // Overwrite this function to make the req_ids for ReadCoop requests unique for each neighborhood.
   // Otherwise a neighborhood might receive 2 identical responses.
   virtual function void build_req_id_list();
      evl_bus_req_source_t source_id = evl_bus_req_source_t'(0);
      evl_bus_req_id_t     req_id;

      // Random ids up to 255 for all sources except for 12 (Cooperative)
      do begin
         if (source_id != 12) begin
            for (int ii = 0; ii < int'(m_bus_req_id_limit); ii++) begin
               m_bus_req_id_queues[source_id].push_back(ii);
            end
         end
         source_id += evl_bus_req_source_t'(1);
      end while (source_id != evl_bus_req_source_t'(0));

      // For Cooperative source, a couple of bits are the unique agent id.
      for (int jj = 0; jj < 31; jj++) begin
         req_id[4:0] = jj;
         req_id[7:5] = get_agent_id();
         m_bus_req_id_queues[12].push_back(req_id);
      end
   endfunction : build_req_id_list


   //----------------------------------------------------------------------------------------------
   //
   // Function: final_fixup
   //
   //
   virtual function void final_fixup(input evl_trans_desc trans_desc, input evl_req_desc req_desc);
      if (trans_desc.get_cmd() == MESSAGE) begin
         req_desc.set_bus_bank(m_global_attrs.get_bank_from_message(get_shire_id(), req_desc.get_paddr()));
      end
      else if (trans_desc.is_cached() == 1'b0) begin
         req_desc.set_bus_bank(`SC_BANKS);
      end
      else begin
         req_desc.set_bus_bank(m_global_attrs.get_bank_from_address(req_desc.get_paddr()));
      end
      if (req_desc.get_bus_bank() != ~evl_sc_bank_id_t'(0)) begin
         if (req_desc.get_bus_bank() >= `SC_BANKS) begin
            req_desc.set_bus_cache_attr(evl_bus_cache_attr_t'(0));
         end
         else begin
            req_desc.set_bus_cache_attr(evl_bus_cache_attr_t'(1));
         end
      end
      req_desc.set_bus_req_qword_en(trans_desc.get_write_qword_en());
   endfunction : final_fixup


   //----------------------------------------------------------------------------------------------
   //
   // Function: reuse_rsp_id
   //
   // A specific agent may want to use this function to do some response cleanup.
   //
   // If we receive a ReadCoop response and the request came from this port, then
   // we can put the ID back into circulation for reuse.
   virtual function void reuse_rsp_id(input evl_trans_desc trans_desc, input evl_req_desc rcv_rsp_desc);
      evl_bus_req_id_t     bus_rsp_id;
      evl_bus_req_source_t source_id;

      if (rcv_rsp_desc.get_bus_cmd() inside { evl_bus_cmd_t'(`EVL_ETLINK_REQ_READCOOP) }) begin
         if (trans_desc != null) begin
            int coop_rsp_exp;

            `evl_log_mdesc(UVM_HIGH, VMOD_GLOBAL, get_abstract_name(), rcv_rsp_desc, $sformatf("got a READCOOP with trans_desc neighborhood id 0x%x coop_rsp_exp = %0d", trans_desc.get_neighborhood_id(), trans_desc.get_int_param(pk_coop_rsp_exp)))
            coop_rsp_exp = trans_desc.decr_int_param(pk_coop_rsp_exp, 0);
            if (trans_desc.get_neighborhood_id() == get_agent_id()) begin
               if (trans_desc.get_int_param(pk_coop_rsp_exp) == 0) begin // this was the last response
                  bus_rsp_id = rcv_rsp_desc.get_bus_req_id();
                  source_id  = rcv_rsp_desc.get_bus_req_source();
                  m_bus_req_id_queues[source_id].push_back(bus_rsp_id);
                  `evl_log_mdesc(UVM_HIGH, VMOD_GLOBAL, get_abstract_name(), trans_desc, $sformatf("This Readcoop response is the last one. Go ahead and reuse ID 0x%x", bus_rsp_id))
               end
               else begin
                  // Wait until the responses to other neighs are done before reusing the id
                  fork
                     begin
                        bus_rsp_id = rcv_rsp_desc.get_bus_req_id();
                        source_id  = rcv_rsp_desc.get_bus_req_source();
                        `evl_log_mdesc(UVM_HIGH, VMOD_GLOBAL, get_abstract_name(), trans_desc, $sformatf("Waiting for my responses to show up at other neighs"))
                        trans_desc.wait_for_int_param(pk_coop_rsp_exp, 0, 0);
                        `evl_log_mdesc(UVM_HIGH, VMOD_GLOBAL, get_abstract_name(), trans_desc, $sformatf("All of my responses have shown up at other neighs! Safe to reuse id 0x%x for source 0x%x now", bus_rsp_id, source_id))
                        m_bus_req_id_queues[source_id].push_back(bus_rsp_id);
                     end
                  join_none
               end
            end
         end
      end // if (rcv_rsp_desc.get_bus_cmd() inside { evl_bus_cmd_t'(`EVL_ETLINK_REQ_READCOOP) })
      else begin
         bus_rsp_id = rcv_rsp_desc.get_bus_req_id();
         source_id  = rcv_rsp_desc.get_bus_req_source();
         m_bus_req_id_queues[source_id].push_back(bus_rsp_id);
      end
   endfunction : reuse_rsp_id


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
      // Since we're a master agent, we need to receive observed responses:
      m_rcv_rsp_exports[0] = new("m_rcv_rsp_export", this);
      m_rcv_rsp_fifos[0]   = new("m_rcv_rsp_fifo", this);
      set_neighborhood_id(get_agent_id());
   endfunction : build_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string name_in = "evl_neigh_agent_etlink", input uvm_component parent_in = null);
      super.new(name_in, parent_in, NEIGH_STIM_AGENT);
      add_agent_alias(ETLINK_MASTER_STIM_AGENT);
      m_stimulus_agent = 1'b1;
      m_neigh_message  = 1'b1;
      if (`et_test_plusargs("allow_duplicate_id") != 0) begin
         m_allow_duplicate_id = 1'b1;
      end
   endfunction : new

endclass : evl_neigh_agent_etlink

//--------------------------------------------------------------------------------------------------
//
// Class: evl_minion_agent_etlink
//
// This class is an extension of evl_stim_agent_etlink to
// define a master stim agent that acts like a single
// minion. 
class evl_minion_agent_etlink extends evl_stim_agent_etlink;

   `uvm_component_utils(evl_minion_agent_etlink)


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

      add_int_req_act( { MESSAGE },   { 1'b0,1'b1 }, { I, E, M },         { 0 }, St_Ignore, Data_ReqWr,  `EVL_ETLINK_REQ_MSG_SEND_DATA);
      add_int_req_act( { MESSAGE },   { 1'b0,1'b1 }, { I, E, M },         { 1 }, St_Ignore, Data_ReqWr,  `EVL_ETLINK_REQ_MSG_SEND_DATA);

      add_int_req_act( { ATOMIC },    { 1'b0,1'b1 }, { I, E, M },         { 0 }, St_Ignore, Data_ReqWr,  `EVL_ETLINK_REQ_ATOMIC);
      add_int_req_act( { ATOMIC },    { 1'b0,1'b1 }, { I, E, M },         { 1 }, St_Ignore, Data_ReqWr,  `EVL_ETLINK_REQ_ATOMIC);
      
      
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


   //-----------------------------------------------------------------------------------------------
   //
   // Function: select_req_port
   //
   // This function is used to select the correct stimulus port on which to send a request (either
   // the evict port or the miss port).
   //
   virtual function int select_req_port(input evl_req_desc req_desc);
       // FUTURE: Dimitris Tsaliagos add support for TENB loads which goes to a different interface also
       return (req_desc.get_bus_cmd() inside {`EVL_ETLINK_REQ_WRITE_AROUND, `EVL_ETLINK_REQ_WRITE,
                                              `EVL_ETLINK_REQ_EVICT, `EVL_ETLINK_REQ_MSG_SEND_DATA, 
                                              `EVL_ETLINK_REQ_FLUSH, `EVL_ETLINK_REQ_ATOMIC,
                                              `EVL_ETLINK_REQ_PREFETCH
                                             });
   endfunction : select_req_port


   //-----------------------------------------------------------------------------------------------
   //
   // Function: select_rsp_port
   //
   virtual function int select_rsp_port(input evl_req_desc req_desc);
       return 0;

       //return (req_desc.get_bus_cmd() inside {`EVL_ETLINK_REQ_WRITE_AROUND, `EVL_ETLINK_REQ_WRITE,
       //                                       `EVL_ETLINK_REQ_EVICT, `EVL_ETLINK_REQ_MSG_SEND_DATA, 
       //                                       `EVL_ETLINK_REQ_FLUSH, `EVL_ETLINK_REQ_ATOMIC,
       //                                       `EVL_ETLINK_REQ_PREFETCH
       //                                      });
   endfunction : select_rsp_port


   //----------------------------------------------------------------------------------------------
   //
   // Function: final_fixup
   //
   //
   virtual function void final_fixup(input evl_trans_desc trans_desc, input evl_req_desc req_desc);
   endfunction : final_fixup


   //-----------------------------------------------------------------------------------------------
   //
   // Function: build_phase
   //
   // There are 2 request ports for driving stimulus, but the funky thing is that both request
   // ET link interfaces receive the same response, so there is really only one response port.
   //
   virtual function void build_phase(uvm_phase phase);
      super.build_phase(phase);
      m_snd_req_ports.push_back(null);
      m_snd_req_ports.push_back(null);
      m_rcv_rsp_exports.push_back(null);
      m_rcv_rsp_fifos.push_back(null);
      m_snd_req_ports[0]   = new("m_snd_req_port[0]", this);
      m_snd_req_ports[1]   = new("m_snd_req_port[1]", this);
      // Since we're a master agent, we need to receive observed responses:
      m_rcv_rsp_exports[0] = new("m_rcv_rsp_export[0]", this);

      m_rcv_rsp_fifos[0]   = new("m_rcv_rsp_fifo[0]", this);

      set_neighborhood_id(get_agent_id());
      
      // The width of the et-link interface in the minion cores is just 5 bits
      // Chaning the limit of the bus_req_id since the default is set to 255 and it
      // it will cause an incosistency between the driver and the agent (which expects an id of 8bits)
      m_bus_req_id_limit = 2**5; 

   endfunction : build_phase

   virtual function void connect_phase(uvm_phase phase);
       super.connect_phase(phase);
       // Temporary hack to solve the issue with the shared response interface
       // of the 2 et-link agents (evict/miss) 
       m_rcv_rsp_exports.push_back(m_rcv_rsp_exports[0]);
   endfunction: connect_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string name_in = "evl_minion_agent_etlink", input uvm_component parent_in = null);
      super.new(name_in, parent_in, CORE_STIM_AGENT);
      add_agent_alias(CORE_STIM_AGENT);
      m_stimulus_agent = 1'b1;
   endfunction : new

endclass : evl_minion_agent_etlink

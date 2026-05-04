//--------------------------------------------------------------------------------------------------
//
// Class: evl_minion_l1d_scoreboard
//
//
class evl_minion_l1d_scoreboard extends evl_minion_dcache_scoreboard #(evl_minion_l1d_scoreboard);

   `uvm_component_utils(evl_minion_l1d_scoreboard)


   function void build_phase(uvm_phase phase);
      super.build_phase(phase);

      m_tlb = new("m_tlb", this);
      m_tlb.set_abstract_name($sformatf("%s(dTLB)", get_root_abstract_name()));
      m_tlb.set_cache_type("TLB");

      foreach (m_cache[ii]) begin
         m_cache[ii] = new($sformatf("l1dcache_%0d", ii), this);
         m_cache[ii].set_parent_sb(this);
         m_cache[ii].set_abstract_name($sformatf("%s(L1D$)", get_root_abstract_name()));
         m_cache[ii].set_cache_type("L1D$");
         m_cache[ii].enable_coverage();
      end


      //
      // Requests from integer pipe
      //
      //          |---------------- Inputs ---------------| |------------------------------ Outputs -------------------------|
      //           Command      Model State     Qualifier    Tag Action   Data Action   Command          Rsp Type   Err Code
      //           -----------  --------------  ----------   -----------  ------------  ---------------  ---------  ---------

      add_req_act( { LD, ST },  { St_Ignore },  DCQ_Replay,  St_Ignore,   Data_None,    BusIdle,         RspNull,   EC_Replay );
      add_req_act( { LD },      { I },          DCQ_None,    IE_Bsy,      Data_None,    BusRead,         RspNull,   EC_Replay );
      add_req_act( { ST },      { I },          DCQ_None,    IM_Bsy,      Data_None,    BusRead,         RspNull,   EC_Replay );
      add_req_act( { LD, ST },  { IE_Bsy },     DCQ_None,    St_Ignore,   Data_None,    BusIdle,         RspNull,   EC_Replay );
      add_req_act( { LD, ST },  { IM_Bsy },     DCQ_None,    St_Ignore,   Data_None,    BusIdle,         RspNull,   EC_Replay );
      add_req_act( { LD, ST },  { EM_Bsy },     DCQ_None,    St_Ignore,   Data_None,    BusIdle,         RspNull,   EC_Replay );
      add_req_act( { LD, ST },  { LD_Bsy },     DCQ_None,    St_Ignore,   Data_None,    BusIdle,         RspNull,   EC_Replay );
      add_req_act( { LD, ST },  { CT_Bsy },     DCQ_None,    St_Ignore,   Data_None,    BusIdle,         RspNull,   EC_Replay );
      add_req_act( { LD },      { E, M },       DCQ_None,    St_Ignore,   Data_MemRd,   BusIdle,         RspWData,  EC_None   );
      add_req_act( { LD },      { L, D },       DCQ_None,    St_Ignore,   Data_MemRd,   BusIdle,         RspWData,  EC_None   );
      add_req_act( { LD },      { C, T },       DCQ_None,    St_Ignore,   Data_MemRd,   BusIdle,         RspWData,  EC_None   );
      add_req_act( { ST },      { E },          DCQ_None,    EM_Bsy,      Data_None,    BusIdle,         RspNull,   EC_Replay ); // RTL forces store to replay when hitting exclusive by sending it to miss handler, which is the guy to update the state to M
      add_req_act( { ST },      { L },          DCQ_None,    LD_Bsy,      Data_None,    BusIdle,         RspNull,   EC_Replay );
      add_req_act( { ST },      { C },          DCQ_None,    CT_Bsy,      Data_None,    BusIdle,         RspNull,   EC_Replay );
      add_req_act( { ST },      { M, D, T },    DCQ_None,    St_Ignore,   Data_MemWr,   BusIdle,         RspAck,    EC_None   );

      //
      // Uncached accesses
      //
      //          |------------------- Inputs ------------------|   |------------------------------ Outputs -------------------------|
      //           Command            Model State     Qualifier      Tag Action   Data Action   Command          Rsp Type   Err Code
      //           -----------------  --------------  -------------  -----------  ------------  ---------------  ---------  ---------
      add_req_act( { LD, LD_RA },     { St_Ignore },  DCQ_Uncached,  St_Ignore,   Data_None,    BusRead,         RspNull,   EC_Replay );
      add_req_act( { LD, LD_RA },     { St_Ignore },  DCQ_Resp,      St_Ignore,   Data_None,    BusIdle,         RspWData,  EC_None   );
      add_req_act( { ST, ST_WA },     { St_Ignore },  DCQ_Uncached,  St_Ignore,   Data_FwdWr,   BusWrite,        RspNull,   EC_None   );
      add_req_act( { ST, ST_WA },     { St_Ignore },  DCQ_Resp,      St_Ignore,   Data_None,    BusIdle,         RspAck,    EC_None   );
      add_req_act( { ST_WA, LD_RA },  { St_Ignore },  DCQ_Replay,    St_Ignore,   Data_None,    BusIdle,         RspNull,   EC_Replay );
      add_req_act( { ATOMIC },        { St_Ignore },  DCQ_Replay,    St_Ignore,   Data_None,    BusIdle,         RspNull,   EC_Replay );
      add_req_act( { ATOMIC },        { St_Ignore },  DCQ_Uncached,  St_Ignore,   Data_FwdWr,   BusWriteAtomic,  RspNull,   EC_Replay );
      add_req_act( { ATOMIC },        { St_Ignore },  DCQ_Resp,      St_Ignore,   Data_None,    BusIdle,         RspWData,  EC_None   ); // Atomics response

      //
      // Port Message requests
      //
      //          |---------------- Inputs ----------------| |------------------------------ Outputs -------------------------|
      //           Command      Model State     Qualifier     Tag Action   Data Action   Command          Rsp Type   Err Code
      //           -----------  --------------  -----------   -----------  ------------  ---------------  ---------  ---------
      add_req_act( { MESSAGE }, { St_Ignore },  DCQ_Uncached, St_Ignore,   Data_FwdWr,   BusWriteMsg,     RspAck,    EC_None   );
      add_req_act( { MESSAGE }, { St_Ignore },  DCQ_Replay  , St_Ignore,   Data_None,    BusIdle,         RspNull,   EC_Replay );

      //
      // Requests from CSR (cache ops)
      //
      //           Command              Model State      Qualifier    Tag Action   Data Action   Command        Rsp Type   Err Code
      //           -------------------  ---------------  -----------  -----------  ------------  -------------  ---------  ----------
      add_req_act( { CACHE_PREFETCH },  { St_Ignore },   DCQ_Replay,  St_Ignore,   Data_None,    BusIdle,       RspNull,   EC_Replay );
      add_req_act( { CACHE_PREFETCH },  { I },           DCQ_None,    IE_Bsy,      Data_None,    BusRead,       RspAck,    EC_None   );
      add_req_act( { CACHE_PREFETCH },  { IE_Bsy },      DCQ_None,    St_Ignore,   Data_None,    BusIdle,       RspAck,    EC_None   );
      add_req_act( { CACHE_PREFETCH },  { IM_Bsy },      DCQ_None,    St_Ignore,   Data_None,    BusIdle,       RspAck,    EC_None   );
      add_req_act( { CACHE_PREFETCH },  { E, M },        DCQ_None,    St_Ignore,   Data_None,    BusIdle,       RspAck,    EC_None   );
      add_req_act( { CACHE_PREFETCH },  { L, D },        DCQ_None,    St_Ignore,   Data_None,    BusIdle,       RspAck,    EC_None   );
      add_req_act( { CACHE_PREFETCH },  { C, T },        DCQ_None,    St_Ignore,   Data_None,    BusIdle,       RspAck,    EC_None   );

      add_req_act( { CACHE_PREFETCH },  { St_Ignore },   DCQ_ToL2,    St_Ignore,   Data_None,    BusPrefetch,   RspNull,   EC_None   );
      add_req_act( { CACHE_PREFETCH },  { St_Ignore },   DCQ_ToL3,    St_Ignore,   Data_None,    BusPrefetch,   RspNull,   EC_None   );

      //           Command                           Model State     Qualifier    Tag Action   Data Action      Command    Rsp Type   Err Code
      //           --------------------------------  ------------    -----------  -----------  ---------------  --------   ---------  ---------
      add_req_act( { CACHE_EVICT_SW, CACHE_EVICT },  { I, L, D },    DCQ_ToL2,    St_Ignore,   Data_None,       BusIdle,   RspAck,    EC_None   );
      add_req_act( { CACHE_EVICT_SW, CACHE_EVICT },  { E, C },       DCQ_ToL2,    I,           Data_None,       BusIdle,   RspAck,    EC_None   );
      add_req_act( { CACHE_EVICT_SW, CACHE_EVICT },  { M, T },       DCQ_ToL2,    I,           Data_ReqWrBack,  BusWrite,  RspAck,    EC_None   );

      add_req_act( { CACHE_EVICT_SW },               { I },          DCQ_ToL3,    St_Ignore,   Data_None,       BusIdle,   RspAck,    EC_None   );
      add_req_act( { CACHE_EVICT },                  { I },          DCQ_ToL3,    St_Ignore,   Data_None,       BusIdle,   RspAck,    EC_FwdReq );
      add_req_act( { CACHE_EVICT_SW, CACHE_EVICT },  { L, D },       DCQ_ToL3,    St_Ignore,   Data_None,       BusIdle,   RspAck,    EC_None   );
      add_req_act( { CACHE_EVICT_SW, CACHE_EVICT },  { E, C },       DCQ_ToL3,    I,           Data_None,       BusIdle,   RspAck,    EC_FwdReq );
      add_req_act( { CACHE_EVICT_SW, CACHE_EVICT },  { M, T },       DCQ_ToL3,    I,           Data_ReqWrBack,  BusWrite,  RspAck,    EC_FwdReq );

      add_req_act( { CACHE_EVICT_SW },               { I },          DCQ_ToMem,   St_Ignore,   Data_None,       BusIdle,   RspAck,    EC_None   );
      add_req_act( { CACHE_EVICT },                  { I },          DCQ_ToMem,   St_Ignore,   Data_None,       BusIdle,   RspAck,    EC_FwdReq );
      add_req_act( { CACHE_EVICT_SW, CACHE_EVICT },  { L, D },       DCQ_ToMem,   St_Ignore,   Data_None,       BusIdle,   RspAck,    EC_None   );
      add_req_act( { CACHE_EVICT_SW, CACHE_EVICT },  { E, C },       DCQ_ToMem,   I,           Data_None,       BusIdle,   RspAck,    EC_FwdReq );
      add_req_act( { CACHE_EVICT_SW, CACHE_EVICT },  { M, T },       DCQ_ToMem,   I,           Data_ReqWrBack,  BusWrite,  RspAck,    EC_FwdReq );

      //           Command                           Model State      Qualifier    Tag Action   Data Action      Command    Rsp Type   Err Code
      //           --------------------------------  ------------     -----------  -----------  ---------------  --------   ---------  ---------
      add_req_act( { CACHE_FLUSH_SW, CACHE_FLUSH },  { I, E, L, C },  DCQ_ToL2,    St_Ignore,   Data_None,       BusIdle,   RspAck,    EC_None   );
      add_req_act( { CACHE_FLUSH_SW, CACHE_FLUSH },  { M },           DCQ_ToL2,    E,           Data_ReqWrBack,  BusWrite,  RspAck,    EC_None   );
      add_req_act( { CACHE_FLUSH_SW, CACHE_FLUSH },  { D },           DCQ_ToL2,    L,           Data_ReqWrBack,  BusWrite,  RspAck,    EC_None   );
      add_req_act( { CACHE_FLUSH_SW, CACHE_FLUSH },  { T },           DCQ_ToL2,    C,           Data_ReqWrBack,  BusWrite,  RspAck,    EC_None   );

      add_req_act( { CACHE_FLUSH_SW },               { I },           DCQ_ToL3,    St_Ignore,   Data_None,       BusIdle,   RspAck,    EC_None   );
      add_req_act( { CACHE_FLUSH },                  { I },           DCQ_ToL3,    St_Ignore,   Data_None,       BusIdle,   RspAck,    EC_FwdReq );
      add_req_act( { CACHE_FLUSH_SW, CACHE_FLUSH },  { E, L, C },     DCQ_ToL3,    St_Ignore,   Data_None,       BusIdle,   RspAck,    EC_FwdReq );
      add_req_act( { CACHE_FLUSH_SW, CACHE_FLUSH },  { M },           DCQ_ToL3,    E,           Data_ReqWrBack,  BusWrite,  RspAck,    EC_FwdReq );
      add_req_act( { CACHE_FLUSH_SW, CACHE_FLUSH },  { D },           DCQ_ToL3,    L,           Data_ReqWrBack,  BusWrite,  RspAck,    EC_FwdReq );
      add_req_act( { CACHE_FLUSH_SW, CACHE_FLUSH },  { T },           DCQ_ToL3,    C,           Data_ReqWrBack,  BusWrite,  RspAck,    EC_FwdReq );

      add_req_act( { CACHE_FLUSH_SW },               { I },           DCQ_ToMem,   St_Ignore,   Data_None,       BusIdle,   RspAck,    EC_None   );
      add_req_act( { CACHE_FLUSH },                  { I },           DCQ_ToMem,   St_Ignore,   Data_None,       BusIdle,   RspAck,    EC_FwdReq );
      add_req_act( { CACHE_FLUSH_SW, CACHE_FLUSH },  { E, L, C },     DCQ_ToMem,   St_Ignore,   Data_None,       BusIdle,   RspAck,    EC_FwdReq );
      add_req_act( { CACHE_FLUSH_SW, CACHE_FLUSH },  { M },           DCQ_ToMem,   E,           Data_ReqWrBack,  BusWrite,  RspAck,    EC_FwdReq );
      add_req_act( { CACHE_FLUSH_SW, CACHE_FLUSH },  { D },           DCQ_ToMem,   L,           Data_ReqWrBack,  BusWrite,  RspAck,    EC_FwdReq );
      add_req_act( { CACHE_FLUSH_SW, CACHE_FLUSH },  { T },           DCQ_ToMem,   C,           Data_ReqWrBack,  BusWrite,  RspAck,    EC_FwdReq );

      //           Command               Model State   Qualifier         Tag Action   Data Action      Command    Rsp Type   Err Code
      //           -----------------     ------------  -----------       -----------  --------------   --------   ---------  ---------
      add_req_act( { CACHE_LOCK },       { I, E },     DCQ_None,         T,           Data_ZeroFill,   BusIdle,   RspAck,    EC_None  );
      add_req_act( { CACHE_LOCK },       { M },        DCQ_None,         T,           Data_ZeroFill,   BusIdle,   RspAck,    EC_None  );
      add_req_act( { CACHE_LOCK },       { C },        DCQ_None,         T,           Data_ZeroFill,   BusIdle,   RspAck,    EC_None  );
      add_req_act( { CACHE_LOCK },       { T },        DCQ_None,         St_Ignore,   Data_ZeroFill,   BusIdle,   RspAck,    EC_None  );
      add_req_act( { CACHE_LOCK },       { L },        DCQ_None,         D,           Data_ZeroFill,   BusIdle,   RspAck,    EC_None  );
      add_req_act( { CACHE_LOCK },       { D },        DCQ_None,         St_Ignore,   Data_ZeroFill,   BusIdle,   RspAck,    EC_None  );
      add_req_act( { CACHE_UNLOCK },     { C },        DCQ_None,         E,           Data_None,       BusIdle,   RspAck,    EC_None  );
      add_req_act( { CACHE_UNLOCK },     { T },        DCQ_None,         M,           Data_None,       BusIdle,   RspAck,    EC_None  );
      add_req_act( { CACHE_UNLOCK },     { I, E, M },  DCQ_None,         St_Ignore,   Data_None,       BusIdle,   RspAck,    EC_None  );
      add_req_act( { CACHE_UNLOCK },     { L, D },     DCQ_None,         St_Ignore,   Data_None,       BusIdle,   RspAck,    EC_None  );

      add_req_act( { CACHE_LOCK_SW },    { I, E, M },  DCQ_None,         D,           Data_ZeroFill,   BusIdle,   RspAck,    EC_None  );
      add_req_act( { CACHE_LOCK_SW },    { C, T },     DCQ_None,         D,           Data_ZeroFill,   BusIdle,   RspAck,    EC_None  );
      add_req_act( { CACHE_UNLOCK_SW },  { I, E, M },  DCQ_None,         St_Ignore,   Data_None,       BusIdle,   RspAck,    EC_None  );
      add_req_act( { CACHE_UNLOCK_SW },  { C, L },     DCQ_None,         E,           Data_None,       BusIdle,   RspAck,    EC_None  );
      add_req_act( { CACHE_UNLOCK_SW },  { T, D },     DCQ_None,         M,           Data_None,       BusIdle,   RspAck,    EC_None  );


      //           Command              Model State      Qualifier    Tag Action   Data Action   Command          Rsp Type   Err Code
      //           -------------------  ---------------  -----------  -----------  ------------  ---------------  ---------  ----------
      add_req_act( { CACHE_SCP_FILL },  { St_Ignore },   DCQ_None,    St_Ignore,   Data_None,    BusSysPrefetch,  RspNull,   EC_None   );

      //
      // Tag updates from the miss handler
      //
      //          |--------------- Inputs --------------| |---------------------------- Outputs ---------------------|
      //           Command      Model State   Qualifier    Tag Action   Data Action   Command    Rsp Type   Err Code
      //           -----------  ------------  -----------  -----------  ------------  --------   ---------  ---------
      add_req_act( { NOCMD },   { IE_Bsy },   DCQ_None,    St_Ignore,   Data_None,    BusIdle,   RspNull,   EC_None  );
      add_req_act( { NOCMD },   { IM_Bsy },   DCQ_None,    St_Ignore,   Data_None,    BusIdle,   RspNull,   EC_None  );
      add_req_act( { NOCMD },   { EM_Bsy },   DCQ_None,    M,           Data_None,    BusIdle,   RspNull,   EC_None  );
      add_req_act( { NOCMD },   { LD_Bsy },   DCQ_None,    D,           Data_None,    BusIdle,   RspNull,   EC_None  );
      add_req_act( { NOCMD },   { CT_Bsy },   DCQ_None,    T,           Data_None,    BusIdle,   RspNull,   EC_None  );
      //
      // updates after error response
      //
      add_req_act( { NOCMD },   { EI_Bsy },   DCQ_None,    I,           Data_None,    BusIdle,   RspNull,   EC_None  );
      add_req_act( { NOCMD },   { MI_Bsy },   DCQ_None,    I,           Data_None,    BusIdle,   RspNull,   EC_None  );


      //
      // Replayed error responses (turn off coverage for now)
      //
      //             |---------------- Inputs ----------------| |------------------------------ Outputs -------------------------|
      //              Command      Model State     Qualifier     Tag Action   Data Action   Command          Rsp Type   Err Code
      //              -----------  --------------  -----------   -----------  ------------  ---------------  ---------  ---------
      add_req_act_nc( { LD, ST },  { St_Ignore },  DCQ_BusErr,   St_Ignore,   Data_None,    BusIdle,         RspAck,    EC_None  );
      add_req_act_nc( { ST_WA  },  { St_Ignore },  DCQ_BusErr,   St_Ignore,   Data_None,    BusIdle,         RspAck,    EC_None  );
      add_req_act_nc( { LD_RA  },  { St_Ignore },  DCQ_BusErr,   St_Ignore,   Data_None,    BusIdle,         RspAck,    EC_None  );
      add_req_act_nc( { ATOMIC },  { St_Ignore },  DCQ_BusErr,   St_Ignore,   Data_None,    BusIdle,         RspAck,    EC_None  );


      //
      // Responses from ETLink
      //
      //              |-------------------------------------- Inputs ------------------------------------------| |------------- Outputs ------------------|
      //               Command              ETLink command             Model State     Bus response   Qualifier   Tag Action   Data Action      Err Code
      //               -------------------  ------------------------   --------------  -------------  ---------   -----------  ---------------  -----------
      add_bus_rsp_act( { LD },              { ET_LINK_REQ_Read },      { IE_Bsy },     { RspWData },  DCQ_None,   E,           Data_MemWrInst,  EC_Replay  );
      add_bus_rsp_act( { ST },              { ET_LINK_REQ_Read },      { IM_Bsy },     { RspWData },  DCQ_None,   M,           Data_MemWrInst,  EC_Replay  );
      add_bus_rsp_act( { CACHE_PREFETCH },  { ET_LINK_REQ_Read },      { IE_Bsy },     { RspWData },  DCQ_None,   E,           Data_MemWrInst,  EC_None    );
      add_bus_rsp_act( { EVICT },           { ET_LINK_REQ_Write },     { St_Ignore },  { RspAck },    DCQ_None,   St_Ignore,   Data_None,       EC_None    );
      add_bus_rsp_act( { ST },              { ET_LINK_REQ_Write },     { St_Ignore },  { RspAck },    DCQ_None,   St_Ignore,   Data_None,       EC_None    );

      add_bus_rsp_act( { ST },              { ET_LINK_REQ_Write },     { St_Ignore },  { RspAck },    DCQ_Resp,   St_Ignore,   Data_None,       EC_None    );
      add_bus_rsp_act( { ST_WA },           { ET_LINK_REQ_Write },     { St_Ignore },  { RspAck },    DCQ_Resp,   St_Ignore,   Data_None,       EC_None    );
      add_bus_rsp_act( { LD },              { ET_LINK_REQ_Read },      { St_Ignore },  { RspWData },  DCQ_Resp,   St_Ignore,   Data_None,       EC_Replay  );
      add_bus_rsp_act( { LD_RA },           { ET_LINK_REQ_Read },      { St_Ignore },  { RspWData },  DCQ_Resp,   St_Ignore,   Data_None,       EC_Replay  );
      add_bus_rsp_act( { ATOMIC },          { ET_LINK_REQ_Atomic },    { St_Ignore },  { RspWData },  DCQ_Resp,   St_Ignore,   Data_None,       EC_Replay  );

      add_bus_rsp_act( { CACHE_EVICT },     { ET_LINK_REQ_Evict },     { St_Ignore },  { RspAck },    DCQ_None,   St_Ignore,   Data_None,       EC_None    );
      add_bus_rsp_act( { CACHE_EVICT_SW },  { ET_LINK_REQ_Evict },     { St_Ignore },  { RspAck },    DCQ_None,   St_Ignore,   Data_None,       EC_None    );
      add_bus_rsp_act( { CACHE_FLUSH },     { ET_LINK_REQ_Flush },     { St_Ignore },  { RspAck },    DCQ_None,   St_Ignore,   Data_None,       EC_None    );
      add_bus_rsp_act( { CACHE_FLUSH_SW },  { ET_LINK_REQ_Flush },     { St_Ignore },  { RspAck },    DCQ_None,   St_Ignore,   Data_None,       EC_None    );
      add_bus_rsp_act( { CACHE_EVICT },     { ET_LINK_REQ_Write },     { St_Ignore },  { RspAck },    DCQ_None,   St_Ignore,   Data_None,       EC_None    );
      add_bus_rsp_act( { CACHE_EVICT_SW },  { ET_LINK_REQ_Write },     { St_Ignore },  { RspAck },    DCQ_None,   St_Ignore,   Data_None,       EC_None    );
      add_bus_rsp_act( { CACHE_FLUSH },     { ET_LINK_REQ_Write },     { St_Ignore },  { RspAck },    DCQ_None,   St_Ignore,   Data_None,       EC_None    );
      add_bus_rsp_act( { CACHE_FLUSH_SW },  { ET_LINK_REQ_Write },     { St_Ignore },  { RspAck },    DCQ_None,   St_Ignore,   Data_None,       EC_None    );
      add_bus_rsp_act( { CACHE_LOCK },      { ET_LINK_REQ_Write },     { St_Ignore },  { RspAck },    DCQ_None,   St_Ignore,   Data_None,       EC_None    );
      add_bus_rsp_act( { CACHE_LOCK_SW },   { ET_LINK_REQ_Write },     { St_Ignore },  { RspAck },    DCQ_None,   St_Ignore,   Data_None,       EC_None    );
      add_bus_rsp_act( { CACHE_PREFETCH },  { ET_LINK_REQ_Prefetch },  { St_Ignore },  { RspAck },    DCQ_None,   St_Ignore,   Data_None,       EC_None    );
      add_bus_rsp_act( { CACHE_SCP_FILL },  { ET_LINK_REQ_ScpFill },   { St_Ignore },  { RspAck },    DCQ_None,   St_Ignore,   Data_None,       EC_None    );

      add_bus_rsp_act( { TENSOR_LOAD },     { ET_LINK_REQ_Read },      { St_Ignore },  { RspWData },  DCQ_None,   St_Ignore,   Data_None,       EC_None    );
      add_bus_rsp_act( { TENSOR_LOAD },     { ET_LINK_REQ_ReadCoop },  { St_Ignore },  { RspWData },  DCQ_None,   St_Ignore,   Data_None,       EC_None    );

      // (CYE) TENSOR_STORE: change to DCQ_None (for Cached, same as TENSOR_LOAD)
      add_bus_rsp_act( { TENSOR_STORE },    { ET_LINK_REQ_WriteAround },{ St_Ignore }, { RspAck },    DCQ_None,   St_Ignore,   Data_None,       EC_None    );
      add_bus_rsp_act( { TENSOR_STORE_SCP },{ ET_LINK_REQ_WriteAround },{ St_Ignore }, { RspAck },    DCQ_None,   St_Ignore,   Data_None,       EC_None    );


      //
      // Error Responses from ETLink (turn off coverage for now)
      //
      add_bus_rsp_act_nc( { LD },              { ET_LINK_REQ_Read },      { IE_Bsy },     { RspErr },    DCQ_None,   EI_Bsy,      Data_None,       EC_ErrRsp  );
      add_bus_rsp_act_nc( { ST },              { ET_LINK_REQ_Read },      { IM_Bsy },     { RspErr },    DCQ_None,   MI_Bsy,      Data_None,       EC_ErrRsp  );
      add_bus_rsp_act_nc( { CACHE_PREFETCH },  { ET_LINK_REQ_Read },      { IE_Bsy },     { RspErr },    DCQ_None,   EI_Bsy,      Data_None,       EC_None    );
      add_bus_rsp_act_nc( { EVICT },           { ET_LINK_REQ_Write },     { St_Ignore },  { RspErr },    DCQ_None,   St_Ignore,   Data_None,       EC_None    );
      add_bus_rsp_act_nc( { ST },              { ET_LINK_REQ_Write },     { St_Ignore },  { RspErr },    DCQ_None,   St_Ignore,   Data_None,       EC_None    );

      add_bus_rsp_act_nc( { ST },              { ET_LINK_REQ_Write },     { St_Ignore },  { RspErr },    DCQ_Resp,   St_Ignore,   Data_None,       EC_ErrRsp  );
      add_bus_rsp_act_nc( { ST_WA },           { ET_LINK_REQ_Write },     { St_Ignore },  { RspErr },    DCQ_Resp,   St_Ignore,   Data_None,       EC_ErrRsp  );
      add_bus_rsp_act_nc( { LD },              { ET_LINK_REQ_Read },      { St_Ignore },  { RspErr },    DCQ_Resp,   St_Ignore,   Data_None,       EC_ErrRsp  );
      add_bus_rsp_act_nc( { LD_RA },           { ET_LINK_REQ_Read },      { St_Ignore },  { RspErr },    DCQ_Resp,   St_Ignore,   Data_None,       EC_ErrRsp  );
      add_bus_rsp_act_nc( { ATOMIC },          { ET_LINK_REQ_Atomic },    { St_Ignore },  { RspErr },    DCQ_Resp,   St_Ignore,   Data_None,       EC_ErrRsp  );

      add_bus_rsp_act_nc( { CACHE_EVICT },     { ET_LINK_REQ_Evict },     { St_Ignore },  { RspErr },    DCQ_None,   St_Ignore,   Data_None,       EC_None    );
      add_bus_rsp_act_nc( { CACHE_EVICT_SW },  { ET_LINK_REQ_Evict },     { St_Ignore },  { RspErr },    DCQ_None,   St_Ignore,   Data_None,       EC_None    );
      add_bus_rsp_act_nc( { CACHE_FLUSH },     { ET_LINK_REQ_Flush },     { St_Ignore },  { RspErr },    DCQ_None,   St_Ignore,   Data_None,       EC_None    );
      add_bus_rsp_act_nc( { CACHE_FLUSH_SW },  { ET_LINK_REQ_Flush },     { St_Ignore },  { RspErr },    DCQ_None,   St_Ignore,   Data_None,       EC_None    );
      add_bus_rsp_act_nc( { CACHE_EVICT },     { ET_LINK_REQ_Write },     { St_Ignore },  { RspErr },    DCQ_None,   St_Ignore,   Data_None,       EC_None    );
      add_bus_rsp_act_nc( { CACHE_EVICT_SW },  { ET_LINK_REQ_Write },     { St_Ignore },  { RspErr },    DCQ_None,   St_Ignore,   Data_None,       EC_None    );
      add_bus_rsp_act_nc( { CACHE_FLUSH },     { ET_LINK_REQ_Write },     { St_Ignore },  { RspErr },    DCQ_None,   St_Ignore,   Data_None,       EC_None    );
      add_bus_rsp_act_nc( { CACHE_FLUSH_SW },  { ET_LINK_REQ_Write },     { St_Ignore },  { RspErr },    DCQ_None,   St_Ignore,   Data_None,       EC_None    );
      add_bus_rsp_act_nc( { CACHE_LOCK },      { ET_LINK_REQ_Write },     { St_Ignore },  { RspErr },    DCQ_None,   St_Ignore,   Data_None,       EC_None    );
      add_bus_rsp_act_nc( { CACHE_LOCK_SW },   { ET_LINK_REQ_Write },     { St_Ignore },  { RspErr },    DCQ_None,   St_Ignore,   Data_None,       EC_None    );
      add_bus_rsp_act_nc( { CACHE_PREFETCH },  { ET_LINK_REQ_Prefetch },  { St_Ignore },  { RspErr },    DCQ_None,   St_Ignore,   Data_None,       EC_None    );
      add_bus_rsp_act_nc( { CACHE_SCP_FILL },  { ET_LINK_REQ_ScpFill },   { St_Ignore },  { RspErr },    DCQ_None,   St_Ignore,   Data_None,       EC_None    );

      add_bus_rsp_act_nc( { TENSOR_LOAD },     { ET_LINK_REQ_Read },      { St_Ignore },  { RspErr },    DCQ_None,   St_Ignore,   Data_None,       EC_None    );
      add_bus_rsp_act_nc( { TENSOR_LOAD },     { ET_LINK_REQ_ReadCoop },  { St_Ignore },  { RspErr },    DCQ_None,   St_Ignore,   Data_None,       EC_None    );

      // (CYE) TENSOR_STORE: change to DCQ_None (for Cached, same as TENSOR_LOAD)
      add_bus_rsp_act_nc( { TENSOR_STORE },    { ET_LINK_REQ_WriteAround },{ St_Ignore }, { RspErr },    DCQ_None,   St_Ignore,   Data_None,       EC_None    );
      add_bus_rsp_act_nc( { TENSOR_STORE_SCP },{ ET_LINK_REQ_WriteAround },{ St_Ignore }, { RspErr },    DCQ_None,   St_Ignore,   Data_None,       EC_None    );


      foreach (m_cache[ii]) begin
         m_cache[ii].build_coverage();
      end
   endfunction : build_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function void start_of_simulation_phase(uvm_phase phase);
      super.start_of_simulation_phase(phase);

      foreach (m_cache[ii]) begin
         if (m_cache[ii] != null) begin
            m_cache[ii].set_abstract_name($sformatf("%s(L1D$)", get_root_abstract_name()));
         end
      end
   endfunction : start_of_simulation_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string name_in = "evl_minion_l1d_scoreboard", input uvm_component parent_in = null);
      super.new(name_in, parent_in);
      set_abstract_name("MINION(L1D$SB)");
   endfunction : new

endclass : evl_minion_l1d_scoreboard


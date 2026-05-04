//--------------------------------------------------------------------------------------------------
//
// Class: evl_l2_scoreboard
//
//
// This l2_scoreboard extends the shire cache scoreboard and defines a table which
// describes the actions the shire cache scoreboard should take to model
// and L2.
class evl_l2_scoreboard extends evl_shire_cache_scoreboard #(evl_l2_scoreboard);

   `uvm_component_utils(evl_l2_scoreboard)


   //-----------------------------------------------------------------------------------------------
   //
   // Static Member Variables and Objects
   //
   `evl_param_key(pk_l2_index_msb)
   `evl_param_key(pk_l2_set_size)
   `evl_param_key(pk_random_sc_test)


   bit m_no_cover;

   //-----------------------------------------------------------------------------------------------
   //
   // Function: process_sc_bank_config
   //
   virtual function void process_sc_bank_config(input evl_sc_bank_config_t rtl_info);
      super.process_sc_bank_config(rtl_info);
      m_global_attrs.set_l2_config(get_shire_id(), m_bypass, m_mem_size, m_set_base_addr, m_set_base_addr, m_stim_index_mask, m_alias_mask);
      m_agent.set_int_param(pk_l2_set_size, m_set_size);
      m_agent.set_int_param(pk_l2_index_msb, m_index_msb);
   endfunction : process_sc_bank_config


   //-----------------------------------------------------------------------------------------------
   //
   // Function: convert_index_cacheop_address
   //
   virtual function evl_paddr_t convert_index_cacheop_address(input evl_paddr_t cacheop_address);
      evl_paddr_t addr_in;
      evl_paddr_t addr_out;
      evl_paddr_t set_mask;
      evl_paddr_t set_top;
      
      int  bank;
      int  subbank;
      int  set;
      int  shire = 0;

     
      set_mask = (1 << $clog2(m_raw_set_size)) - 1;
      set_top = m_set_base_addr + m_raw_set_size - 1;
      addr_in = cacheop_address;
      addr_in = addr_in >> `EVL_LINE_ADDR_LSB;
      bank = addr_in[`SC_BANK_ID_SIZE-1:0];
      addr_in = addr_in >> `SC_BANK_ID_SIZE;
      subbank = addr_in[`SC_SUB_BANK_ID_SIZE-1:0];
      addr_in = addr_in >> (`SC_SUB_BANK_ID_SIZE + `SC_WAY_ID_SIZE);
      set = addr_in;
    
      // If the set is between set_base and set_top (which it should be!) then just use the set mask
      if ((set <= set_top) && (set >= m_set_base_addr)) begin
         set = set & set_mask;
      end
      else begin
         `dut_error(get_abstract_name(), $sformatf("TB_ERROR: L2 scoreboard not expecting an index cacheop set outside of the base and top. set = %x base = %x top = %x",set,m_set_base_addr,set_top))
      end
      
      addr_out = set << (`SC_SUB_BANK_ID_SIZE + `SC_BANK_ID_SIZE + `EVL_LINE_ADDR_LSB) | subbank << (`SC_BANK_ID_SIZE + `EVL_LINE_ADDR_LSB) | bank << (`EVL_LINE_ADDR_LSB);
      `evl_log(UVM_HIGH,VMOD_SC_SB, get_abstract_name(), $sformatf("converting index cacheop address %x to %x before table lookup",cacheop_address,addr_out))
      return addr_out;
   endfunction : convert_index_cacheop_address


   //-----------------------------------------------------------------------------------------------
   function void build_phase(uvm_phase phase);
      super.build_phase(phase);

      m_cache = new("m_cache", this);
      m_cache.set_abstract_name($sformatf("%s(L2$)", get_root_abstract_name()));
      m_cache.set_cache_type("L2$");
      m_cache.set_verbosity_mods(VMOD_SC_SB, VMOD_L2_SB_WARN);
      m_cache.enable_coverage();

      m_merge_partial_opcodes.push_back(SC_L2_WriteAround);
      m_check_partial_opcodes.push_back(SC_L2_Read);
      m_check_partial_opcodes.push_back(SC_L2_Atomic);
      m_check_partial_opcodes.push_back(SC_L2_Prefetch);

      m_index_cacheop_opcodes.push_back(SC_Idx_L2_Inv);
      m_index_cacheop_opcodes.push_back(SC_Idx_L2_Flush);
      m_index_cacheop_opcodes.push_back(SC_Idx_L2_Evict);

      m_no_cover = 1'b1;
      
      //============================================================================================
      //============================================================================================
      //
      // Lookups Performed at Request Queue Allocation
      //
      //============================================================================================
      //============================================================================================

      // Notes about Scoreboard Cache States:
      //
      // IE_Bsy: We have sent a read request downstram and we're waiting for response data, which
      //         we're going to install.
      // WI_Bsy
      // MI_Bsy: These are transitional states typically only used from "tc req" time to "tag rsp" time
      //         to mark a request as "we expect to be evicting this"
      // WM_Bsy:
      // WD_Bsy:
      //
      // ME_MemBsy
      // WI_MemBsy: "MemBsy" designates that instead of a regular Write, Flush, or Evict, the RTL needs
      //            to send a FlushToMem or FlushToEvict downstream
      //
      // A1_I_Bsy
      // A2_I_Bsy The A1 or A2 indicates that we have sent one or more writes (or flush) downstream for this
      // A1_E_Bsy request and are waiting for that number of Acks. The "Bsy" simply indicates a transitional
      // A2_E_Bsy state and is redundant to the A1/A2. The I/E/M/D in the middle indicates the state
      // A1_M_Bsy of the data in the cache.
      //
      // A1_MI_Bsy: We may also end up in a transitional state while still waiting for an Ack
      //
      // ET Link Messages (local to shire)
      //                                                              Obs   Model                                                      Bus
      //               ET-link Opcode            L2 Opcode           State  State           Qual         Tag Action  Data Action       Command       Rsp Type  Err Code
      //               ------------------------  ------------------  -----  --------------  -----------  ----------  ----------------  ------------  --------  --------------
      add_reqq_req_act(ET_LINK_REQ_MsgSendData,  SC_L2_MsgSendData,  I,     { St_Ignore },  `SCQ_None,  St_Ignore,  Data_ForwardMsg,  BusLocalMsg,  RspNull,  EC_DeallocReqq  );
      // Message sent in Bypass still goes downstream.
      add_reqq_req_act(ET_LINK_REQ_MsgSendData,  SC_L2_MsgSendData,  I,     { St_Ignore },  `SCQ_Bypass,  St_Ignore,  Data_ForwardMsg,  BusLocalMsg,  RspNull,  EC_DeallocReqq  );
      // Message with invalid dest port 
      add_reqq_req_act(ET_LINK_REQ_MsgSendData,  SC_L2_MsgSendData,  I,     { St_Ignore },  `SCQ_DecErr,  St_Ignore,  Data_None,  BusIdle,  RspNull,  EC_None  );
      add_reqq_req_act(ET_LINK_REQ_MsgSendData,  SC_L2_MsgSendData,  I,     { St_Ignore },  (`SCQ_DecErr | `SCQ_Bypass),  St_Ignore,  Data_None,  BusIdle,  RspNull,  EC_None  );
      
      
      
      //
      // A Flush, Evict, or Prefetch that doesn't apply to the L2 will bypass the pipe
      //                                                        Obs   Model                                                      Bus
      //               ET-link Opcode         L2 Opcode        State  State           Qual          Tag Action  Data Action      Command          Rsp Type  Err Code
      //               ---------------------  ---------------  -----  --------------  ------------  ----------  ---------------  ---------------  --------  --------------
      add_reqq_req_act(ET_LINK_REQ_Flush,     SC_L2_Flush,     I,     { St_Ignore },  `SCQ_L3_Mem,  Stale,      Data_ReqWrNull,  BusFlush,        RspAck,   EC_None        );
      add_reqq_req_act(ET_LINK_REQ_Evict,     SC_L2_Evict,     I,     { St_Ignore },  `SCQ_L3_Mem,  Stale,      Data_ReqWrNull,  BusEvict,        RspAck,   EC_None        );
      add_reqq_req_act(ET_LINK_REQ_Prefetch,  SC_L2_Prefetch,  I,     { St_Ignore },  `SCQ_L3,      Stale,      Data_ReqWrNull,  BusPrefetch,     RspNull,  EC_None        );
      // mem shire doesn't want a prefetch
      add_reqq_req_act(ET_LINK_REQ_Prefetch,  SC_L2_Prefetch,  I,     { St_Ignore },  `SCQ_Mem,     Stale,      Data_None,       BusIdle,         RspAck,   EC_None        );
      // mem shire doesn't want a prefetch when remote l3 is disabled
      add_reqq_req_act(ET_LINK_REQ_Prefetch,  SC_L2_Prefetch,  I,     { St_Ignore },  (`SCQ_L3 |`SCQ_DownDis),  Stale,  Data_None,  BusIdle,      RspAck,   EC_None        );

      // L3->Mem when remote L3 is disabled doesn't do anything.
      add_reqq_req_act(ET_LINK_REQ_Flush,     SC_L2_Flush,     I,     { St_Ignore },  (`SCQ_L3_Mem|`SCQ_DownDis),  Stale,  Data_ReqWrNull,  BusIdle,  RspAck,   EC_None     );
      add_reqq_req_act(ET_LINK_REQ_Evict,     SC_L2_Evict,     I,     { St_Ignore },  (`SCQ_L3_Mem|`SCQ_DownDis),  Stale,  Data_ReqWrNull,  BusIdle,  RspAck,   EC_None     );

      // Remote Scratchpad Evict L2->L3 is legal but all other incantations are errors
      add_reqq_req_act(ET_LINK_REQ_Evict,     SC_L2_Evict,     I,     { St_Ignore },  (`SCQ_Remote|`SCQ_L1_L3),              Stale,      Data_None,  BusIdle,  RspAck,   EC_MaybeErrRsp  );
      add_reqq_req_act(ET_LINK_REQ_Evict,     SC_L2_Evict,     I,     { St_Ignore },  (`SCQ_Remote|`SCQ_L2_L3),              Stale,      Data_None,  BusIdle,  RspAck,   EC_None  );
      add_reqq_req_act(ET_LINK_REQ_Evict,     SC_L2_Evict,     I,     { St_Ignore },  (`SCQ_Remote|`SCQ_L1_Mem),             Stale,      Data_None,  BusIdle,  RspAck,   EC_MaybeErrRsp  );
      add_reqq_req_act(ET_LINK_REQ_Evict,     SC_L2_Evict,     I,     { St_Ignore },  (`SCQ_Remote|`SCQ_L2_Mem),             Stale,      Data_None,  BusIdle,  RspAck,   EC_MaybeErrRsp  ); 
      add_reqq_req_act(ET_LINK_REQ_Evict,     SC_L2_Evict,     I,     { St_Ignore },  (`SCQ_Remote|`SCQ_L3_Mem),             Stale,      Data_None,  BusIdle,  RspAck,   EC_MaybeErrRsp  );
      add_reqq_req_act(ET_LINK_REQ_Evict,     SC_L2_Evict,     I,     { St_Ignore },  (`SCQ_Remote),                         Stale,      Data_None,  BusIdle,  RspAck,   EC_MaybeErrRsp  );

      //CLC Aug2019: These cases don't show up here, they show up below with L2 Opcode SC_Reqq_Initial instead.
      //add_reqq_req_act(ET_LINK_REQ_Evict,     SC_L2_Evict,     I,     { St_Ignore },  (`SCQ_Remote|`SCQ_L1_L3|`SCQ_Bypass), Stale,      Data_None,  BusIdle,  RspAck,   EC_MaybeErrRsp  );
      //add_reqq_req_act(ET_LINK_REQ_Evict,     SC_L2_Evict,     I,     { St_Ignore },  (`SCQ_Remote|`SCQ_L2_L3|`SCQ_Bypass), Stale,      Data_None,  BusIdle,  RspAck,   EC_None  );
      //add_reqq_req_act(ET_LINK_REQ_Evict,     SC_L2_Evict,     I,     { St_Ignore },  (`SCQ_Remote|`SCQ_L1_Mem|`SCQ_Bypass), Stale,      Data_None,  BusIdle,  RspAck,   EC_MaybeErrRsp  );
      //add_reqq_req_act(ET_LINK_REQ_Evict,     SC_L2_Evict,     I,     { St_Ignore },  (`SCQ_Remote|`SCQ_L2_Mem|`SCQ_Bypass), Stale,      Data_None,  BusIdle,  RspAck,   EC_MaybeErrRsp  );
      //add_reqq_req_act(ET_LINK_REQ_Evict,     SC_L2_Evict,     I,     { St_Ignore },  (`SCQ_Remote|`SCQ_L3_Mem|`SCQ_Bypass), Stale,      Data_None,  BusIdle,  RspAck,   EC_MaybeErrRsp  );
      
      // ReadForward
      add_reqq_req_act(ET_LINK_REQ_Read,   SC_L2_Read,         I,     { St_Ignore },   `SCQ_L3,     Stale,      Data_None,       BusRead,      RspNull,     EC_None);
      // WriteForward
      add_reqq_req_act(ET_LINK_REQ_Write,  SC_L2_Write,        I,     { St_Ignore},    `SCQ_L3,     Stale,      Data_FwdWr,      BusWrite,     RspNull,     EC_None);
      
      //
      // What to do in Bypass:
      //                                                          Obs   Model                                                      Bus
      //               ET-link Opcode           L2 Opcode        State  State           Qual          Tag Action  Data Action      Command          Rsp Type  Err Code
      //               ---------------------    ---------------  -----  --------------  ------------  ----------  ---------------  ---------------  --------  --------------
      add_reqq_req_act(ET_LINK_REQ_Write,       SC_Reqq_Initial,    I,    { St_Ignore },  `SCQ_Bypass,  Stale,    Data_FwdWr,       BusWrite,       RspNull,   EC_None        );
      add_reqq_req_act(ET_LINK_REQ_WriteAround, SC_Reqq_Initial,    I,    { St_Ignore },  `SCQ_Bypass,  Stale,    Data_FwdWr,       BusWrite,       RspNull,   EC_None        );
      add_reqq_req_act(ET_LINK_REQ_Read,        SC_Reqq_Initial,    I,    { St_Ignore },  `SCQ_Bypass,  Stale,    Data_None,        BusRead,        RspNull,   EC_None        );
      add_reqq_req_act(ET_LINK_REQ_ReadCoop,    SC_Reqq_Initial,    I,    { St_Ignore },  `SCQ_Bypass,  Stale,    Data_None,        BusRead,        RspNull,   EC_None        );
      add_reqq_req_act(ET_LINK_REQ_Lock,        SC_Reqq_Initial,    I,    { St_Ignore },  `SCQ_Bypass,  Stale,    Data_None,        BusIdle,        RspAck,    EC_None        );
      add_reqq_req_act(ET_LINK_REQ_Unlock,      SC_Reqq_Initial,    I,    { St_Ignore },  `SCQ_Bypass,  Stale,    Data_None,        BusIdle,        RspAck,    EC_None        );

      add_reqq_req_act(ET_LINK_REQ_Flush,     SC_Reqq_Initial,    I,    { St_Ignore },  (`SCQ_Bypass | `SCQ_L1_L3),  Stale,    Data_None,       BusIdle,       RspAck,   EC_None);
      add_reqq_req_act(ET_LINK_REQ_Flush,     SC_Reqq_Initial,    I,    { St_Ignore },  (`SCQ_Bypass | `SCQ_L2_L3),  Stale,    Data_None,       BusIdle,       RspAck,   EC_None);
      add_reqq_req_act(ET_LINK_REQ_Flush,     SC_Reqq_Initial,    I,    { St_Ignore },  (`SCQ_Bypass | `SCQ_L1_Mem),  Stale,    Data_ReqWrNull, BusFlush,       RspNull,   EC_None);
      add_reqq_req_act(ET_LINK_REQ_Flush,     SC_Reqq_Initial,    I,    { St_Ignore },  (`SCQ_Bypass | `SCQ_L2_Mem),  Stale,    Data_ReqWrNull, BusFlush,       RspNull,   EC_None);
      add_reqq_req_act(ET_LINK_REQ_Flush,     SC_Reqq_Initial,    I,    { St_Ignore },  (`SCQ_Bypass | `SCQ_L3_Mem),  Stale,    Data_ReqWrNull, BusFlush,       RspNull,   EC_None);
      // this one covers strange combinations for src/dest for the L2 to receive (like L1->L2)
      add_reqq_req_act(ET_LINK_REQ_Flush,     SC_Reqq_Initial,    I,    { St_Ignore },  (`SCQ_Bypass ),               Stale,    Data_None,       BusIdle,       RspAck,   EC_None);
      // L2 is bypassed and we're not sending anything to L3
      add_reqq_req_act(ET_LINK_REQ_Flush,     SC_Reqq_Initial,    I,    { St_Ignore },  (`SCQ_Bypass|`SCQ_DownDis|`SCQ_L1_L3),  Stale,    Data_None,    BusIdle,     RspAck,   EC_None);
      add_reqq_req_act(ET_LINK_REQ_Flush,     SC_Reqq_Initial,    I,    { St_Ignore },  (`SCQ_Bypass|`SCQ_DownDis|`SCQ_L2_L3),  Stale,    Data_None,    BusIdle,     RspAck,   EC_None);
      add_reqq_req_act(ET_LINK_REQ_Flush,     SC_Reqq_Initial,    I,    { St_Ignore },  (`SCQ_Bypass|`SCQ_DownDis|`SCQ_L1_Mem), Stale,    Data_None,    BusIdle,     RspAck,   EC_None);
      add_reqq_req_act(ET_LINK_REQ_Flush,     SC_Reqq_Initial,    I,    { St_Ignore },  (`SCQ_Bypass|`SCQ_DownDis|`SCQ_L2_Mem), Stale,    Data_None,    BusIdle,     RspAck,   EC_None);
      add_reqq_req_act(ET_LINK_REQ_Flush,     SC_Reqq_Initial,    I,    { St_Ignore },  (`SCQ_Bypass|`SCQ_DownDis|`SCQ_L3_Mem), Stale,    Data_None,    BusIdle,     RspAck,   EC_None);
      add_reqq_req_act(ET_LINK_REQ_Flush,     SC_Reqq_Initial,    I,    { St_Ignore },  (`SCQ_Bypass|`SCQ_DownDis), Stale,    Data_None,    BusIdle,     RspAck,   EC_None);

      add_reqq_req_act(ET_LINK_REQ_Evict,     SC_Reqq_Initial,    I,    { St_Ignore },  (`SCQ_Bypass | `SCQ_L1_L3),  Stale,    Data_None,       BusIdle,       RspAck,   EC_None);
      add_reqq_req_act(ET_LINK_REQ_Evict,     SC_Reqq_Initial,    I,    { St_Ignore },  (`SCQ_Bypass | `SCQ_L2_L3),  Stale,    Data_None,       BusIdle,       RspAck,   EC_None);
      add_reqq_req_act(ET_LINK_REQ_Evict,     SC_Reqq_Initial,    I,    { St_Ignore },  (`SCQ_Bypass | `SCQ_L1_Mem),  Stale,    Data_ReqWrNull,  BusEvict,       RspNull,   EC_None);
      add_reqq_req_act(ET_LINK_REQ_Evict,     SC_Reqq_Initial,    I,    { St_Ignore },  (`SCQ_Bypass | `SCQ_L2_Mem),  Stale,    Data_ReqWrNull,  BusEvict,       RspNull,   EC_None);
      add_reqq_req_act(ET_LINK_REQ_Evict,     SC_Reqq_Initial,    I,    { St_Ignore },  (`SCQ_Bypass | `SCQ_L3_Mem),  Stale,    Data_ReqWrNull,  BusEvict,       RspNull,   EC_None);
      // this one covers strange combinations of src/dest for the L2 to receive like L1->L2
      add_reqq_req_act(ET_LINK_REQ_Evict,     SC_Reqq_Initial,    I,    { St_Ignore },  (`SCQ_Bypass),                Stale,    Data_None,       BusIdle,       RspAck,   EC_None);
      
      // Remote Scp Evict but we're in bypass (L2-L3 does nothing. The rest are errors just like they are in non-bypass)
      add_reqq_req_act(ET_LINK_REQ_Evict,     SC_Reqq_Initial,    I,    { St_Ignore },  (`SCQ_Bypass|`SCQ_Remote|`SCQ_L1_L3),   Stale,      Data_None,  BusIdle,        RspAck,   EC_MaybeErrRsp );
      add_reqq_req_act(ET_LINK_REQ_Evict,     SC_Reqq_Initial,    I,    { St_Ignore },  (`SCQ_Bypass|`SCQ_Remote|`SCQ_L2_L3),   Stale,      Data_None,  BusIdle,        RspAck,   EC_None        );
      add_reqq_req_act(ET_LINK_REQ_Evict,     SC_Reqq_Initial,    I,    { St_Ignore },  (`SCQ_Bypass|`SCQ_Remote|`SCQ_L1_Mem),  Stale,      Data_None,  BusIdle,        RspAck,   EC_MaybeErrRsp );
      add_reqq_req_act(ET_LINK_REQ_Evict,     SC_Reqq_Initial,    I,    { St_Ignore },  (`SCQ_Bypass|`SCQ_Remote|`SCQ_L2_Mem),  Stale,      Data_None,  BusIdle,        RspAck,   EC_MaybeErrRsp );
      add_reqq_req_act(ET_LINK_REQ_Evict,     SC_Reqq_Initial,    I,    { St_Ignore },  (`SCQ_Bypass|`SCQ_Remote|`SCQ_L3_Mem),  Stale,      Data_None,  BusIdle,        RspAck,   EC_MaybeErrRsp );
      add_reqq_req_act(ET_LINK_REQ_Evict,     SC_Reqq_Initial,    I,    { St_Ignore },  (`SCQ_Bypass|`SCQ_Remote|`SCQ_DownDis|`SCQ_L1_L3),   Stale,      Data_None,  BusIdle,     RspAck,   EC_MaybeErrRsp );
      add_reqq_req_act(ET_LINK_REQ_Evict,     SC_Reqq_Initial,    I,    { St_Ignore },  (`SCQ_Bypass|`SCQ_Remote|`SCQ_DownDis|`SCQ_L2_L3),   Stale,      Data_None,  BusIdle,     RspAck,   EC_None        );
      add_reqq_req_act(ET_LINK_REQ_Evict,     SC_Reqq_Initial,    I,    { St_Ignore },  (`SCQ_Bypass|`SCQ_Remote|`SCQ_DownDis|`SCQ_L1_Mem),  Stale,      Data_None,  BusIdle,     RspAck,   EC_MaybeErrRsp );
      add_reqq_req_act(ET_LINK_REQ_Evict,     SC_Reqq_Initial,    I,    { St_Ignore },  (`SCQ_Bypass|`SCQ_Remote|`SCQ_DownDis|`SCQ_L2_Mem),  Stale,      Data_None,  BusIdle,     RspAck,   EC_MaybeErrRsp );
      add_reqq_req_act(ET_LINK_REQ_Evict,     SC_Reqq_Initial,    I,    { St_Ignore },  (`SCQ_Bypass|`SCQ_Remote|`SCQ_DownDis|`SCQ_L3_Mem),  Stale,      Data_None,  BusIdle,     RspAck,   EC_MaybeErrRsp );
      add_reqq_req_act(ET_LINK_REQ_Evict,     SC_Reqq_Initial,    I,    { St_Ignore },  (`SCQ_Bypass|`SCQ_Remote|`SCQ_DownDis),              Stale,      Data_None,  BusIdle,     RspAck,   EC_MaybeErrRsp );
      
      // L2 is bypassed and we're not sending anything to L3
      add_reqq_req_act(ET_LINK_REQ_Evict,     SC_Reqq_Initial,    I,    { St_Ignore },  (`SCQ_Bypass|`SCQ_DownDis|`SCQ_L1_L3),  Stale,    Data_None,    BusIdle,     RspAck,   EC_None);
      add_reqq_req_act(ET_LINK_REQ_Evict,     SC_Reqq_Initial,    I,    { St_Ignore },  (`SCQ_Bypass|`SCQ_DownDis|`SCQ_L2_L3),  Stale,    Data_None,    BusIdle,     RspAck,   EC_None);
      add_reqq_req_act(ET_LINK_REQ_Evict,     SC_Reqq_Initial,    I,    { St_Ignore },  (`SCQ_Bypass|`SCQ_DownDis|`SCQ_L1_Mem), Stale,    Data_None,    BusIdle,     RspAck,   EC_None);
      add_reqq_req_act(ET_LINK_REQ_Evict,     SC_Reqq_Initial,    I,    { St_Ignore },  (`SCQ_Bypass|`SCQ_DownDis|`SCQ_L2_Mem), Stale,    Data_None,    BusIdle,     RspAck,   EC_None);
      add_reqq_req_act(ET_LINK_REQ_Evict,     SC_Reqq_Initial,    I,    { St_Ignore },  (`SCQ_Bypass|`SCQ_DownDis|`SCQ_L3_Mem), Stale,    Data_None,    BusIdle,     RspAck,   EC_None);
      add_reqq_req_act(ET_LINK_REQ_Evict,     SC_Reqq_Initial,    I,    { St_Ignore },  (`SCQ_Bypass|`SCQ_DownDis),             Stale,    Data_None,    BusIdle,     RspAck,   EC_None);

      add_reqq_req_act(ET_LINK_REQ_Prefetch,  SC_Reqq_Initial,    I,    { St_Ignore },  `SCQ_Bypass,                        Stale,  Data_None,      BusIdle,     RspAck,  EC_None, m_no_cover);
      add_reqq_req_act(ET_LINK_REQ_Prefetch,  SC_Reqq_Initial,    I,    { St_Ignore },  (`SCQ_Bypass|`SCQ_L2),              Stale,  Data_None,      BusIdle,     RspAck,  EC_None );
      add_reqq_req_act(ET_LINK_REQ_Prefetch,  SC_Reqq_Initial,    I,    { St_Ignore },  (`SCQ_Bypass|`SCQ_L2|`SCQ_DownDis), Stale,  Data_None,      BusIdle,     RspAck,  EC_None );
      add_reqq_req_act(ET_LINK_REQ_Prefetch,  SC_Reqq_Initial,    I,    { St_Ignore },  (`SCQ_Bypass|`SCQ_L3),              Stale,  Data_ReqWrNull, BusPrefetch, RspNull, EC_DeallocReqq );
      add_reqq_req_act(ET_LINK_REQ_Prefetch,  SC_Reqq_Initial,    I,    { St_Ignore },  (`SCQ_Bypass|`SCQ_L3|`SCQ_DownDis), Stale,  Data_None,      BusIdle,     RspAck,  EC_None );
      add_reqq_req_act(ET_LINK_REQ_Prefetch,  SC_Reqq_Initial,    I,    { St_Ignore },  (`SCQ_Bypass|`SCQ_Mem),             Stale,  Data_None,      BusIdle,     RspAck,  EC_None );
      add_reqq_req_act(ET_LINK_REQ_Prefetch,  SC_Reqq_Initial,    I,    { St_Ignore },  (`SCQ_Bypass|`SCQ_Mem|`SCQ_DownDis),Stale,  Data_None,      BusIdle,     RspAck,  EC_None );
      

      //add_reqq_req_act(ET_LINK_REQ_MsgSendData,  SC_Reqq_Initial,  I,     { St_Ignore },  `SCQ_Bypass,  St_Ignore,  Data_ForwardMsg,  BusLocalMsg,  RspNull,  EC_DeallocReqq  );
      add_reqq_req_act(ET_LINK_REQ_Atomic,       SC_Reqq_Initial,  I,     { St_Ignore },  `SCQ_Bypass,  St_Ignore,  Data_ZeroRsp,     BusIdle,      RspWData, EC_MaybeErrRsp  ); 

      // Another special case for when the coalescing buffer is disabled:
      add_reqq_req_act(ET_LINK_REQ_WriteAround, SC_L2_WriteAround,  I,    { St_Ignore },  `SCQ_None,  Stale,    Data_FwdWr,       BusWrite,       RspNull,   EC_None        );

      // Scp fill to L2 is an error:
      add_reqq_req_act(ET_LINK_REQ_ScpFill,     SC_Scp_Fill,        I,    { St_Ignore },  `SCQ_None,  Stale,    Data_None,        BusIdle,        RspAck,    EC_MaybeErrRsp  );
      
      //============================================================================================
      //============================================================================================
      //
      // Lookups Performed at the Read Buffer
      //
      //============================================================================================
      //============================================================================================

      //
      // ET Link Reads
      //                                                Obs   Model                                              Bus
      //               ET-link Opcode     L2 Opcode    State  State         Qual        Tag Action  Data Action  Command   Rsp Type   Err Code
      //               -----------------  -----------  -----  ------------  ----------  ----------  -----------  --------  ---------  --------
      add_rbuf_req_act(ET_LINK_REQ_Read,  SC_L2_Read,  E,     { E },        `SCQ_None,  St_Ignore,  Data_MemRd,  BusIdle,  RspWData,  EC_None  );
      add_rbuf_req_act(ET_LINK_REQ_Read,  SC_L2_Read,  E,     { A1_E_Bsy }, `SCQ_None,  St_Ignore,  Data_MemRd,  BusIdle,  RspWData,  EC_None, m_no_cover );
      add_rbuf_req_act(ET_LINK_REQ_Read,  SC_L2_Read,  E,     { L },        `SCQ_None,  St_Ignore,  Data_MemRd,  BusIdle,  RspWData,  EC_None  );
      add_rbuf_req_act(ET_LINK_REQ_Read,  SC_L2_Read,  E,     { M },        `SCQ_None,  St_Ignore,  Data_MemRd,  BusIdle,  RspWData,  EC_None  );
      add_rbuf_req_act(ET_LINK_REQ_Read,  SC_L2_Read,  E,     { D },        `SCQ_None,  St_Ignore,  Data_MemRd,  BusIdle,  RspWData,  EC_None  );
      add_rbuf_req_act(ET_LINK_REQ_Read,  SC_L2_Read,  E,     { EI_Pend },  `SCQ_None,  St_Ignore,  Data_MemRd,  BusIdle,  RspWData,  EC_None  );

      //
      // ET Link Cooperative Reads
      //                                                    Obs   Model                                              Bus
      //               ET-link Opcode         L2 Opcode    State  State         Qual        Tag Action  Data Action  Command   Rsp Type   Err Code
      //               ---------------------  -----------  -----  ------------  ----------  ----------  -----------  --------  ---------  --------
      add_rbuf_req_act(ET_LINK_REQ_ReadCoop,  SC_L2_Read,  E,     { E },        `SCQ_None,  St_Ignore,  Data_MemRd,  BusIdle,  RspWData,  EC_None  );
      add_rbuf_req_act(ET_LINK_REQ_ReadCoop,  SC_L2_Read,  E,     { A1_E_Bsy},  `SCQ_None,  St_Ignore,  Data_MemRd,  BusIdle,  RspWData,  EC_None, m_no_cover);
      add_rbuf_req_act(ET_LINK_REQ_ReadCoop,  SC_L2_Read,  E,     { L },        `SCQ_None,  St_Ignore,  Data_MemRd,  BusIdle,  RspWData,  EC_None  );
      add_rbuf_req_act(ET_LINK_REQ_ReadCoop,  SC_L2_Read,  E,     { M },        `SCQ_None,  St_Ignore,  Data_MemRd,  BusIdle,  RspWData,  EC_None  );
      add_rbuf_req_act(ET_LINK_REQ_ReadCoop,  SC_L2_Read,  E,     { D },        `SCQ_None,  St_Ignore,  Data_MemRd,  BusIdle,  RspWData,  EC_None  );
      add_rbuf_req_act(ET_LINK_REQ_ReadCoop,  SC_L2_Read,  E,     { EI_Pend },  `SCQ_None,  St_Ignore,  Data_MemRd,  BusIdle,  RspWData,  EC_None  );


      //============================================================================================
      //============================================================================================
      //
      // Lookups Performed at the TC Request Stage
      //
      //============================================================================================
      //============================================================================================

      // CLC May2019: A note about A1_W_Bsy: My assumption is that the RTL will never send another request to the same line down the pipe when we're in this state.
      // CLC June2019: Turns out we can hit this, at least for read requests: State is M. Index cacheop invalidates it (A1_I_Bsy) then WriteAround comes along (A1_W_Bsy).
      // CLC Oct2019: Ok so there's a fun way to hit anything in A1_W_Bsy state: If an Index cacheop invalidates a line (A1_I_Bsy) and then the index cop sm gets
      //              an abort, it won't send a sync. So the REQQ has no issue with sending a WriteAround followed by whatever. It's rare to see happen randomly though.
      
      // CLC May2019: A note about A1_L_Bsy: The only way to get into this state is a Flush. The RTL won't send anything else after a flush until it gets a response
      // Therefore, we can't hit A1_L_Bsy in TC REQ. This also makes A2_L_Bsy impossible to reach.
      // CLC July 2019: I thought that A1_E_Bsy wouldn't get hit either but that will get hit after an index cacheop (though hitting all the cases is pretty rare)
      
      //
      // ET Link Reads
      //                                              Obs   Model                                                          Bus
      //             ET-link Opcode     L2 Opcode    State  State           Qual           Tag Action     Data Action      Command     Rsp Type   Err Code
      //             -----------------  -----------  -----  --------------  -------------  ----------     ---------------  ----------  ---------  ---------------
      add_tc_req_act(ET_LINK_REQ_Read,  SC_L2_Read,  I,     { I },          `SCQ_None,     IE_Bsy,        Data_None,       BusLineRd,  RspNull,   EC_None         );
      add_tc_req_act(ET_LINK_REQ_Read,  SC_L2_Read,  I,     { A1_I_Bsy },   `SCQ_None,     A1_IE_Bsy,     Data_None,       BusLineRd,  RspNull,   EC_None         );
      add_tc_req_act(ET_LINK_REQ_Read,  SC_L2_Read,  I,     { A2_I_Bsy },   `SCQ_None,     A2_IE_Bsy,     Data_None,       BusLineRd,  RspNull,   EC_None, m_no_cover );
      add_tc_req_act(ET_LINK_REQ_Read,  SC_L2_Read,  E,     { E },          `SCQ_None,     St_Ignore,     Data_MemRd,      BusIdle,    RspWData,  EC_None         );
      add_tc_req_act(ET_LINK_REQ_Read,  SC_L2_Read,  E,     { A1_E_Bsy },   `SCQ_None,     St_Ignore,     Data_MemRd,      BusIdle,    RspWData,  EC_None, m_no_cover );
      add_tc_req_act(ET_LINK_REQ_Read,  SC_L2_Read,  E,     { L },          `SCQ_None,     St_Ignore,     Data_MemRd,      BusIdle,    RspWData,  EC_None         );
      add_tc_req_act(ET_LINK_REQ_Read,  SC_L2_Read,  E,     { A1_L_Bsy },   `SCQ_None,     St_Ignore,     Data_MemRd,      BusIdle,    RspWData,  EC_None, m_no_cover );
      add_tc_req_act(ET_LINK_REQ_Read,  SC_L2_Read,  E,     { M },          `SCQ_None,     St_Ignore,     Data_MemRd,      BusIdle,    RspWData,  EC_None         );
      add_tc_req_act(ET_LINK_REQ_Read,  SC_L2_Read,  E,     { A1_M_Bsy },   `SCQ_None,     St_Ignore,     Data_MemRd,      BusIdle,    RspWData,  EC_None         );
      add_tc_req_act(ET_LINK_REQ_Read,  SC_L2_Read,  E,     { A2_M_Bsy },   `SCQ_None,     St_Ignore,     Data_MemRd,      BusIdle,    RspWData,  EC_None, m_no_cover );
      add_tc_req_act(ET_LINK_REQ_Read,  SC_L2_Read,  E,     { D },          `SCQ_None,     St_Ignore,     Data_MemRd,      BusIdle,    RspWData,  EC_None         );
      add_tc_req_act(ET_LINK_REQ_Read,  SC_L2_Read,  E,     { A1_D_Bsy },   `SCQ_None,     St_Ignore,     Data_MemRd,      BusIdle,    RspWData,  EC_None         );
      add_tc_req_act(ET_LINK_REQ_Read,  SC_L2_Read,  E,     { W },          `SCQ_Partial,  WIE_Bsy,       Data_ReqWrBack,  BusVictim,  RspNull,   EC_Victim_ClrQW );
      add_tc_req_act(ET_LINK_REQ_Read,  SC_L2_Read,  E,     { A1_W_Bsy },   `SCQ_Partial,  A1_WIE_Bsy,    Data_ReqWrBack,  BusVictim,  RspNull,   EC_Victim_ClrQW, m_no_cover );
      add_tc_req_act(ET_LINK_REQ_Read,  SC_L2_Fill,  I,     { IE_Bsy },     `SCQ_None,     E,             Data_CacheFill,  BusIdle,    RspNull,   EC_ClrQW        );
      add_tc_req_act(ET_LINK_REQ_Read,  SC_L2_Fill,  I,     { A1_IE_Bsy },  `SCQ_None,     A1_E_Bsy,      Data_CacheFill,  BusIdle,    RspNull,   EC_ClrQW        , m_no_cover );

      // No L2:
      add_tc_req_act(ET_LINK_REQ_Read,  SC_L2_Read,  I,     { I },          `SCQ_NoRegion, St_Ignore,     Data_ZeroRsp,    BusIdle,    RspWData,  EC_MaybeErrRsp  );

      //
      // ET Link ReadCoops
      //                                                  Obs   Model                                                          Bus
      //             ET-link Opcode         L2 Opcode    State  State           Qual           Tag Action     Data Action      Command     Rsp Type   Err Code
      //             ---------------------  -----------  -----  --------------  -------------  ----------     ---------------  ----------  ---------  ---------------
      add_tc_req_act(ET_LINK_REQ_ReadCoop,  SC_L2_Read,  I,     { I },          `SCQ_None,     IE_Bsy,        Data_None,       BusLineRd,  RspNull,   EC_None         );
      add_tc_req_act(ET_LINK_REQ_ReadCoop,  SC_L2_Read,  I,     { A1_I_Bsy },   `SCQ_None,     A1_IE_Bsy,     Data_None,       BusLineRd,  RspNull,   EC_None         );
      add_tc_req_act(ET_LINK_REQ_ReadCoop,  SC_L2_Read,  I,     { A2_I_Bsy },   `SCQ_None,     A2_IE_Bsy,     Data_None,       BusLineRd,  RspNull,   EC_None, m_no_cover );
      add_tc_req_act(ET_LINK_REQ_ReadCoop,  SC_L2_Read,  E,     { E },          `SCQ_None,     St_Ignore,     Data_MemRd,      BusIdle,    RspWData,  EC_None         );
      add_tc_req_act(ET_LINK_REQ_ReadCoop,  SC_L2_Read,  E,     { A1_E_Bsy },   `SCQ_None,     St_Ignore,     Data_MemRd,      BusIdle,    RspWData,  EC_None, m_no_cover );
      add_tc_req_act(ET_LINK_REQ_ReadCoop,  SC_L2_Read,  E,     { L },          `SCQ_None,     St_Ignore,     Data_MemRd,      BusIdle,    RspWData,  EC_None         );
      add_tc_req_act(ET_LINK_REQ_ReadCoop,  SC_L2_Read,  E,     { A1_L_Bsy },   `SCQ_None,     St_Ignore,     Data_MemRd,      BusIdle,    RspWData,  EC_None, m_no_cover );
      add_tc_req_act(ET_LINK_REQ_ReadCoop,  SC_L2_Read,  E,     { M },          `SCQ_None,     St_Ignore,     Data_MemRd,      BusIdle,    RspWData,  EC_None         );
      add_tc_req_act(ET_LINK_REQ_ReadCoop,  SC_L2_Read,  E,     { A1_M_Bsy },   `SCQ_None,     St_Ignore,     Data_MemRd,      BusIdle,    RspWData,  EC_None         );
      add_tc_req_act(ET_LINK_REQ_ReadCoop,  SC_L2_Read,  E,     { A2_M_Bsy },   `SCQ_None,     St_Ignore,     Data_MemRd,      BusIdle,    RspWData,  EC_None, m_no_cover );
      add_tc_req_act(ET_LINK_REQ_ReadCoop,  SC_L2_Read,  E,     { D },          `SCQ_None,     St_Ignore,     Data_MemRd,      BusIdle,    RspWData,  EC_None         );
      add_tc_req_act(ET_LINK_REQ_ReadCoop,  SC_L2_Read,  E,     { A1_D_Bsy },   `SCQ_None,     St_Ignore,     Data_MemRd,      BusIdle,    RspWData,  EC_None         );
      add_tc_req_act(ET_LINK_REQ_ReadCoop,  SC_L2_Read,  E,     { W },          `SCQ_Partial,  WIE_Bsy,       Data_ReqWrBack,  BusVictim,  RspNull,   EC_Victim_ClrQW );
      add_tc_req_act(ET_LINK_REQ_ReadCoop,  SC_L2_Read,  E,     { A1_W_Bsy },   `SCQ_Partial,  A1_WIE_Bsy,    Data_ReqWrBack,  BusVictim,  RspNull,   EC_Victim_ClrQW, m_no_cover );
      add_tc_req_act(ET_LINK_REQ_ReadCoop,  SC_L2_Fill,  I,     { IE_Bsy },     `SCQ_None,     E,             Data_CacheFill,  BusIdle,    RspNull,   EC_ClrQW        );
      add_tc_req_act(ET_LINK_REQ_ReadCoop,  SC_L2_Fill,  I,     { A1_IE_Bsy },  `SCQ_None,     A1_E_Bsy,      Data_CacheFill,  BusIdle,    RspNull,   EC_ClrQW       , m_no_cover );

      // No L2:
      add_tc_req_act(ET_LINK_REQ_ReadCoop,  SC_L2_Read,  I,     { I },          `SCQ_NoRegion, St_Ignore,     Data_ZeroRsp,    BusIdle,    RspWData,  EC_MaybeErrRsp  );

      //
      // ET Link Atomics
      //
      //                                                  Obs   Model                                                           Bus
      //             ET-link Opcode       L2 Opcode      State  State          Qual           Tag Action     Data Action        Command     Rsp Type   Err Code
      //             -------------------  -------------  -----  -------------  -------------  -------------  -----------------  ----------  ---------  ---------------
      add_tc_req_act(ET_LINK_REQ_Atomic,  SC_L2_Atomic,  I,    { I },          `SCQ_None,     IE_Bsy,        Data_None,         BusLineRd,  RspNull,   EC_None         );
      add_tc_req_act(ET_LINK_REQ_Atomic,  SC_L2_Atomic,  I,    { A1_I_Bsy },   `SCQ_None,     A1_IE_Bsy,     Data_None,         BusLineRd,  RspNull,   EC_None         );
      add_tc_req_act(ET_LINK_REQ_Atomic,  SC_L2_Atomic,  I,    { A2_I_Bsy },   `SCQ_None,     A2_IE_Bsy,     Data_None,         BusLineRd,  RspNull,   EC_None, m_no_cover );
      add_tc_req_act(ET_LINK_REQ_Atomic,  SC_L2_Atomic,  E,    { E },          `SCQ_None,     M,             Data_MemWrRd,      BusIdle,    RspWData,  EC_SetQW        );
      add_tc_req_act(ET_LINK_REQ_Atomic,  SC_L2_Atomic,  E,    { A1_E_Bsy},    `SCQ_None,     A1_M_Bsy,      Data_MemWrRd,      BusIdle,    RspWData,  EC_SetQW,  m_no_cover);
      add_tc_req_act(ET_LINK_REQ_Atomic,  SC_L2_Atomic,  E,    { L },          `SCQ_None,     D,             Data_MemWrRd,      BusIdle,    RspWData,  EC_SetQW        );
      add_tc_req_act(ET_LINK_REQ_Atomic,  SC_L2_Atomic,  E,    { A1_L_Bsy },   `SCQ_None,     A1_D_Bsy,      Data_MemWrRd,      BusIdle,    RspWData,  EC_SetQW, m_no_cover);
      add_tc_req_act(ET_LINK_REQ_Atomic,  SC_L2_Atomic,  E,    { M },          `SCQ_None,     St_Ignore,     Data_MemWrRd,      BusIdle,    RspWData,  EC_SetQW        );
      add_tc_req_act(ET_LINK_REQ_Atomic,  SC_L2_Atomic,  E,    { A1_M_Bsy },   `SCQ_None,     St_Ignore,     Data_MemWrRd,      BusIdle,    RspWData,  EC_SetQW        );
      //add_tc_req_act(ET_LINK_REQ_Atomic,  SC_L2_Atomic,  E,    { A2_M_Bsy },   `SCQ_None,     St_Ignore,     Data_MemWrRd,      BusIdle,    RspWData,  EC_SetQW        );
      add_tc_req_act(ET_LINK_REQ_Atomic,  SC_L2_Atomic,  E,    { D },          `SCQ_None,     St_Ignore,     Data_MemWrRd,      BusIdle,    RspWData,  EC_SetQW        );
      add_tc_req_act(ET_LINK_REQ_Atomic,  SC_L2_Atomic,  E,    { A1_D_Bsy },   `SCQ_None,     St_Ignore,     Data_MemWrRd,      BusIdle,    RspWData,  EC_SetQW        );
      add_tc_req_act(ET_LINK_REQ_Atomic,  SC_L2_Atomic,  E,    { W },          `SCQ_Partial,  WIE_Bsy,       Data_ReqWrBack,    BusVictim,  RspNull,   EC_Victim_ClrQW );
      add_tc_req_act(ET_LINK_REQ_Atomic,  SC_L2_Atomic,  E,    { A1_W_Bsy },   `SCQ_Partial,  A1_WIE_Bsy,    Data_ReqWrBack,    BusVictim,  RspNull,   EC_Victim_ClrQW, m_no_cover );

      add_tc_req_act(ET_LINK_REQ_Atomic,  SC_L2_Fill,    I,    { IE_Bsy },     `SCQ_None,     E,             Data_CacheFill,    BusIdle,    RspNull,   EC_ClrQW        );
      add_tc_req_act(ET_LINK_REQ_Atomic,  SC_L2_Fill,    I,    { A1_IE_Bsy },  `SCQ_None,     A1_E_Bsy,      Data_CacheFill,    BusIdle,    RspNull,   EC_ClrQW        , m_no_cover );

      // No L2:
      add_tc_req_act(ET_LINK_REQ_Atomic,  SC_L2_Atomic,  I,     { I },         `SCQ_NoRegion, St_Ignore,     Data_ZeroRsp,      BusIdle,    RspWData,  EC_MaybeErrRsp  );

      //
      // ET Link Writes
      //                                                Obs   Model                                            Bus
      //             ET-link Opcode      L2 Opcode     State  State   Qual        Tag Action  Data Action      Command   Rsp Type  Err Code
      //             ------------------  ------------  -----  ------  ----------  ----------  ---------------  --------  --------  --------------
      add_tc_req_act(ET_LINK_REQ_Write,  SC_L2_Write,  I,     { I },  `SCQ_None,  M,          Data_MemWrInst,  BusIdle,  RspAck,   EC_SetQW       );
      add_tc_req_act(ET_LINK_REQ_Write,  SC_L2_Write,  E,     { E },  `SCQ_None,  M,          Data_MemWr,      BusIdle,  RspAck,   EC_SetQW       );
      add_tc_req_act(ET_LINK_REQ_Write,  SC_L2_Write,  E,     { L },  `SCQ_None,  D,          Data_MemWr,      BusIdle,  RspAck,   EC_SetQW       );
      add_tc_req_act(ET_LINK_REQ_Write,  SC_L2_Write,  E,     { M },  `SCQ_None,  St_Ignore,  Data_MemWr,      BusIdle,  RspAck,   EC_SetQW       );
      add_tc_req_act(ET_LINK_REQ_Write,  SC_L2_Write,  E,     { D },  `SCQ_None,  St_Ignore,  Data_MemWr,      BusIdle,  RspAck,   EC_SetQW       );
      add_tc_req_act(ET_LINK_REQ_Write,  SC_L2_Write,  E,     { W },  `SCQ_None,  WM_Bsy,     Data_ReqWrBack_MemWr, BusVictim,RspAck,   EC_Victim_SetQW);

      //
      // ET Link Writes to Evicted Lines
      //                                                Obs   Model                                                        Bus
      //             ET-link Opcode      L2 Opcode     State  State             Qual        Tag Action    Data Action      Command   Rsp Type  Err Code
      //             ------------------  ------------  -----  ----------------  ----------  ------------  ---------------  --------  --------  --------------
      add_tc_req_act(ET_LINK_REQ_Write,  SC_L2_Write,  I,     { A1_I_Bsy },       `SCQ_None,  A1_M_Bsy,     Data_MemWrInst,  BusIdle,  RspAck,   EC_SetQW       );
      add_tc_req_act(ET_LINK_REQ_Write,  SC_L2_Write,  I,     { A2_I_Bsy },       `SCQ_None,  A2_M_Bsy,     Data_MemWrInst,  BusIdle,  RspAck,   EC_SetQW, m_no_cover );
      add_tc_req_act(ET_LINK_REQ_Write,  SC_L2_Write,  E,     { A1_E_Bsy },       `SCQ_None,  A1_M_Bsy,     Data_MemWrInst,  BusIdle,  RspAck,   EC_SetQW, m_no_cover );
      add_tc_req_act(ET_LINK_REQ_Write,  SC_L2_Write,  E,     { A1_M_Bsy },     `SCQ_None,  St_Ignore,    Data_MemWr,      BusIdle,  RspAck,   EC_SetQW       );
      //add_tc_req_act(ET_LINK_REQ_Write,  SC_L2_Write,  E,     { A2_M_Bsy },     `SCQ_None,  St_Ignore,    Data_MemWr,      BusIdle,  RspAck,   EC_SetQW       );
      //add_tc_req_act(ET_LINK_REQ_Write,  SC_L2_Write,  E,     { A1_W_Bsy },     `SCQ_None,  A1_WM_Bsy,    Data_ReqWrBack_MemWr, BusVictim,RspAck,   EC_Victim_SetQW);
      add_tc_req_act(ET_LINK_REQ_Write,  SC_L2_Write,  E,     { A1_D_Bsy },     `SCQ_None,  St_Ignore,    Data_MemWr,      BusIdle,  RspAck,   EC_SetQW       );


      // No L2:
      add_tc_req_act(ET_LINK_REQ_Write,  SC_L2_Write,  I,     { I },        `SCQ_NoRegion, St_Ignore,  Data_None,       BusIdle,  RspAck,   EC_MaybeErrRsp );

      //
      // WritePartial
      //                                                      Obs   Model                                                      Bus
      //             ET-link Opcode      L2 Opcode           State  State         Qual           Tag Action  Data Action      Command    Rsp Type  Err Code
      //             ------------------  ------------        -----  -----------   ----------     ----------  ---------------  --------   --------  --------
      add_tc_req_act(ET_LINK_REQ_Write,  SC_L2_WritePartial,  I,     { I },       `SCQ_None,     IEM_Bsy,    Data_None,       BusLineRd,   RspAck,   EC_None );
      add_tc_req_act(ET_LINK_REQ_Write,  SC_L2_WritePartial,  E,     { E },       `SCQ_None,     M,          Data_MemWr,      BusIdle,   RspAck,   EC_SetQW );
      add_tc_req_act(ET_LINK_REQ_Write,  SC_L2_WritePartial,  E,     { L },       `SCQ_None,     D,          Data_MemWr,      BusIdle,   RspAck,   EC_SetQW );
      add_tc_req_act(ET_LINK_REQ_Write,  SC_L2_WritePartial,  E,     { M },       `SCQ_None,     St_Ignore,  Data_MemWr,      BusIdle,   RspAck,   EC_None  );
      add_tc_req_act(ET_LINK_REQ_Write,  SC_L2_WritePartial,  E,     { D },       `SCQ_None,     St_Ignore,  Data_MemWr,      BusIdle,   RspAck,   EC_None  );
      // The 'write' part of the read-modify-write
      add_tc_req_act(ET_LINK_REQ_Write,  SC_L2_WritePartial,  E,     { EM_Bsy },  `SCQ_None,     M,          Data_MemWr,      BusIdle,   RspAck,   EC_SetQW );
      add_tc_req_act(ET_LINK_REQ_Write,  SC_L2_WritePartial,  E,     { A1_EM_Bsy},`SCQ_None,     A1_M_Bsy,   Data_MemWr,      BusIdle,   RspAck,   EC_SetQW, m_no_cover );
      add_tc_req_act(ET_LINK_REQ_Write,  SC_L2_WritePartial,  E,     { W },       `SCQ_None,     WEM_Bsy,    Data_ReqWrBack,  BusVictim, RspAck,   EC_Victim );

      // WritePartial to lines in transition
      add_tc_req_act(ET_LINK_REQ_Write,  SC_L2_WritePartial,  I,     { A1_I_Bsy },`SCQ_None,     A1_IEM_Bsy, Data_None,       BusLineRd,   RspAck,   EC_None );
      add_tc_req_act(ET_LINK_REQ_Write,  SC_L2_WritePartial,  I,     { A2_I_Bsy },`SCQ_None,     A2_IEM_Bsy, Data_None,       BusLineRd,   RspAck,   EC_None, m_no_cover );
      add_tc_req_act(ET_LINK_REQ_Write,  SC_L2_WritePartial,  E,     { A1_E_Bsy },`SCQ_None,     A1_M_Bsy,   Data_MemWr,      BusIdle,   RspAck,     EC_SetQW, m_no_cover);
      add_tc_req_act(ET_LINK_REQ_Write,  SC_L2_WritePartial,  E,     { A1_M_Bsy },`SCQ_None,     St_Ignore,  Data_MemWr,      BusIdle,   RspAck,   EC_SetQW );
      //add_tc_req_act(ET_LINK_REQ_Write,  SC_L2_WritePartial,  E,     { A1_L_Bsy },`SCQ_None,     A1_D_Bsy,   Data_MemWr,      BusIdle,   RspAck,   EC_SetQW );
      add_tc_req_act(ET_LINK_REQ_Write,  SC_L2_WritePartial,  E,     { A1_D_Bsy },`SCQ_None,     St_Ignore,  Data_MemWr,      BusIdle,   RspAck,   EC_None  );

      // No L2:
      add_tc_req_act(ET_LINK_REQ_Write,  SC_L2_WritePartial, I,     { I },        `SCQ_NoRegion, St_Ignore,  Data_None,       BusIdle,  RspAck,   EC_MaybeErrRsp );

      // WritePartial sent out a read and here's the fill
      add_tc_req_act(ET_LINK_REQ_Write,  SC_L2_Fill,          I,     { IEM_Bsy }, `SCQ_None,     EM_Bsy,     Data_CacheFill,  BusIdle,   RspAck,   EC_None );
      add_tc_req_act(ET_LINK_REQ_Write,  SC_L2_Fill,          I,     { A1_IEM_Bsy }, `SCQ_None,  A1_EM_Bsy,  Data_CacheFill,  BusIdle,   RspAck,   EC_None, m_no_cover );
      
      //
      // ET Link WriteArounds
      //                                                            Obs   Model                                               Bus
      //             ET-link Opcode            L2 Opcode           State  State   Qual           Tag Action  Data Action      Command     Rsp Type  Err Code
      //             ------------------------  ------------------  -----  ------  -------------  ----------  ---------------  ----------  --------  ---------------
      add_tc_req_act(ET_LINK_REQ_WriteAround,  SC_L2_WriteAround,  I,     { I },  `SCQ_Partial,  W,          Data_MemWrInst,  BusIdle,    RspAck,   EC_EnqWA        );
      add_tc_req_act(ET_LINK_REQ_WriteAround,  SC_L2_WriteAround,  I,     { I },  `SCQ_None,     WI_Bsy,     Data_MrgWrBack,  BusVictim,  RspAck,   EC_Victim_ClrQW );
      add_tc_req_act(ET_LINK_REQ_WriteAround,  SC_L2_WriteAround,  E,     { E },  `SCQ_Partial,  W,          Data_MemWr,      BusIdle,    RspAck,   EC_EnqWA        );
      add_tc_req_act(ET_LINK_REQ_WriteAround,  SC_L2_WriteAround,  E,     { E },  `SCQ_None,     WI_Bsy,     Data_MrgWrBack,  BusVictim,  RspAck,   EC_Victim_ClrQW );
      add_tc_req_act(ET_LINK_REQ_WriteAround,  SC_L2_WriteAround,  E,     { L },  `SCQ_Partial,  W,          Data_MemWr,      BusIdle,    RspAck,   EC_EnqWA        );
      add_tc_req_act(ET_LINK_REQ_WriteAround,  SC_L2_WriteAround,  E,     { L },  `SCQ_None,     WI_Bsy,     Data_MrgWrBack,  BusVictim,  RspAck,   EC_Victim_ClrQW );
      //add_tc_req_act(ET_LINK_REQ_WriteAround,  SC_L2_WriteAround,  E,     { M },  `SCQ_Partial,  W,          Data_MemWr,      BusIdle,    RspAck,   EC_EnqWA        ); no partial here.
      add_tc_req_act(ET_LINK_REQ_WriteAround,  SC_L2_WriteAround,  E,     { M },  `SCQ_None,     WI_Bsy,     Data_MrgWrBack,  BusVictim,  RspAck,   EC_Victim_ClrQW );
      add_tc_req_act(ET_LINK_REQ_WriteAround,  SC_L2_WriteAround,  E,     { D },  `SCQ_None,     WI_Bsy,     Data_MrgWrBack,  BusVictim,  RspAck,   EC_Victim_ClrQW );
      add_tc_req_act(ET_LINK_REQ_WriteAround,  SC_L2_WriteAround,  E,     { W },  `SCQ_Partial,  St_Ignore,  Data_MemWr,      BusIdle,    RspAck,   EC_UpdWAMru     );
      add_tc_req_act(ET_LINK_REQ_WriteAround,  SC_L2_WriteAround,  E,     { W },  `SCQ_None,     WI_Bsy,     Data_MrgWrBack,  BusVictim,  RspAck,   EC_Victim_ClrQW );

      // Remote Scratchpad WriteAround
      add_tc_req_act(ET_LINK_REQ_WriteAround,  SC_L2_WriteAround,  I,     { I },  (`SCQ_Remote|`SCQ_Partial),  W,          Data_MemWrInst,  BusIdle,    RspAck,   EC_EnqWA        );
      add_tc_req_act(ET_LINK_REQ_WriteAround,  SC_L2_WriteAround,  I,     { I },  (`SCQ_Remote),               WI_Bsy,     Data_MrgWrBack,  BusVictim,  RspAck,   EC_Victim_ClrQW );
      add_tc_req_act(ET_LINK_REQ_WriteAround,  SC_L2_WriteAround,  E,     { W },  (`SCQ_Remote|`SCQ_Partial),  St_Ignore,  Data_MemWr,      BusIdle,    RspAck,   EC_UpdWAMru     );
      add_tc_req_act(ET_LINK_REQ_WriteAround,  SC_L2_WriteAround,  E,     { W },  (`SCQ_Remote),               WI_Bsy,     Data_MrgWrBack,  BusVictim,  RspAck,   EC_Victim_ClrQW );

      // No L2:
      add_tc_req_act(ET_LINK_REQ_WriteAround,  SC_L2_WriteAround,  I,     { I },  (`SCQ_NoRegion | `SCQ_Partial),  I,     Data_None,  BusIdle,  RspAck,   EC_MaybeErrRsp  );
      add_tc_req_act(ET_LINK_REQ_WriteAround,  SC_L2_WriteAround,  I,     { I },  (`SCQ_NoRegion),                 I,     Data_None,  BusIdle,  RspAck,   EC_MaybeErrRsp );
      add_tc_req_act(ET_LINK_REQ_WriteAround,  SC_L2_WriteAround,  I,     { I },  (`SCQ_NoRegion |`SCQ_Remote| `SCQ_Partial),  I,     Data_None,  BusIdle,  RspAck,   EC_MaybeErrRsp );
      add_tc_req_act(ET_LINK_REQ_WriteAround,  SC_L2_WriteAround,  I,     { I },  (`SCQ_NoRegion |`SCQ_Remote),                I,     Data_None,  BusIdle,  RspAck,   EC_MaybeErrRsp );

      //
      // ET Link WriteAround Coalescing Eviction (already gone)
      //                                                      Obs   Model                                             Bus
      //             ET-link Opcode            L2 Opcode     State  State        Qual        Tag Action  Data Action  Command   Rsp Type  Err Code
      //             ------------------------  ------------  -----  -----------  ----------  ----------  -----------  --------  --------  --------
      add_tc_req_act(ET_LINK_REQ_WriteAround,  SC_L2_Evict,  I,     { I },       `SCQ_None,  St_Ignore,  Data_None,   BusIdle,  RspNull,  EC_None  );
      //add_tc_req_act(ET_LINK_REQ_WriteAround,  SC_L2_Evict,  I,     { WI_Bsy },  `SCQ_None,  St_Ignore,  Data_None,   BusIdle,  RspNull,  EC_None  );
      add_tc_req_act(ET_LINK_REQ_WriteAround,  SC_L2_Evict,  I,     { A1_I_Bsy },`SCQ_None,  St_Ignore,  Data_None,   BusIdle,  RspNull,  EC_None  );
      add_tc_req_act(ET_LINK_REQ_WriteAround,  SC_L2_Evict,  I,     { A2_I_Bsy },`SCQ_None,  St_Ignore,  Data_None,   BusIdle,  RspNull,  EC_None, m_no_cover  );

      //
      // ET Link WriteAround Coalescing Eviction (already gone and read back in)
      //                                                      Obs   Model                                        Bus
      //             ET-link Opcode            L2 Opcode     State  State   Qual        Tag Action  Data Action  Command   Rsp Type  Err Code
      //             ------------------------  ------------  -----  ------  ----------  ----------  -----------  --------  --------  ---------------
      add_tc_req_act(ET_LINK_REQ_WriteAround,  SC_L2_Evict,  E,     { E },  `SCQ_None,  EI_Pend,    Data_None,   BusIdle,  RspNull,  EC_Victim_ClrQW );

      //
      // ET Link WriteAround Coalescing Eviction
      //                                                      Obs   Model                                                   Bus
      //             ET-link Opcode            L2 Opcode     State  State          Qual        Tag Action  Data Action      Command     Rsp Type  Err Code
      //             ------------------------  ------------  -----  -------------  ----------  ----------  ---------------  ----------  --------  ---------------
      add_tc_req_act(ET_LINK_REQ_WriteAround,  SC_L2_Evict,  E,     { W },         `SCQ_None,  WI_Bsy,     Data_ReqWrBack,  BusVictim,  RspNull,  EC_Victim_ClrQW );
      //add_tc_req_act(ET_LINK_REQ_WriteAround,  SC_L2_Evict,  E,     { A1_W_Bsy },  `SCQ_None,  A1_WI_Bsy,  Data_ReqWrBack,  BusVictim,  RspNull,  EC_Victim_ClrQW );
      add_tc_req_act(ET_LINK_REQ_WriteAround,  SC_L2_Evict,  E,     { A1_M_Bsy },  `SCQ_None,  A1_MI_Bsy,  Data_ReqWrBack,  BusVictim,  RspNull,  EC_Victim_ClrQW );
      add_tc_req_act(ET_LINK_REQ_WriteAround,  SC_L2_Evict,  E,     { M },         `SCQ_None,  MI_Bsy,     Data_ReqWrBack,  BusVictim,  RspNull,  EC_Victim_ClrQW );
      add_tc_req_act(ET_LINK_REQ_WriteAround,  SC_L2_Evict,  E,     { D },         `SCQ_None,  St_Ignore,  Data_None,       BusIdle,    RspNull,  EC_None );
      add_tc_req_act(ET_LINK_REQ_WriteAround,  SC_L2_Evict,  E,     { A1_D_Bsy },  `SCQ_None,  St_Ignore,  Data_None,       BusIdle,    RspNull,  EC_None );
      
      //
      // ET Link WriteArounds to Evicted Lines
      //                                                            Obs   Model                                                        Bus
      //             ET-link Opcode            L2 Opcode           State  State         Qual           Tag Action     Data Action      Command     Rsp Type  Err Code
      //             ------------------------  ------------------  -----  ------------  -------------  -------------  ---------------  ----------  --------  ---------------
      add_tc_req_act(ET_LINK_REQ_WriteAround,  SC_L2_WriteAround,  I,    { A1_I_Bsy },  `SCQ_Partial,  A1_W_Bsy,      Data_MemWrInst,  BusIdle,    RspAck,   EC_EnqWA        );
      add_tc_req_act(ET_LINK_REQ_WriteAround,  SC_L2_WriteAround,  I,    { A1_I_Bsy },  `SCQ_None,     A1_WI_Bsy,     Data_MrgWrBack,  BusVictim,  RspAck,   EC_Victim_ClrQW );
      add_tc_req_act(ET_LINK_REQ_WriteAround,  SC_L2_WriteAround,  I,    { A2_I_Bsy },  `SCQ_Partial,  A2_W_Bsy,      Data_MemWrInst,  BusIdle,    RspAck,   EC_EnqWA,        m_no_cover );
      add_tc_req_act(ET_LINK_REQ_WriteAround,  SC_L2_WriteAround,  I,    { A2_I_Bsy },  `SCQ_None,     A2_WI_Bsy,     Data_MrgWrBack,  BusVictim,  RspAck,   EC_Victim_ClrQW, m_no_cover );
      add_tc_req_act(ET_LINK_REQ_WriteAround,  SC_L2_WriteAround,  E,    { A1_M_Bsy },  `SCQ_None,     A1_WI_Bsy,     Data_MrgWrBack,  BusVictim,  RspAck,   EC_Victim_ClrQW );
      add_tc_req_act(ET_LINK_REQ_WriteAround,  SC_L2_WriteAround,  E,    { A1_W_Bsy },  `SCQ_Partial,  St_Ignore,     Data_MemWr,      BusIdle,    RspAck,   EC_UpdWAMru    , m_no_cover );
      add_tc_req_act(ET_LINK_REQ_WriteAround,  SC_L2_WriteAround,  E,    { A1_W_Bsy },  `SCQ_None,     A1_WI_Bsy,     Data_MrgWrBack,  BusVictim,  RspAck,   EC_Victim_ClrQW, m_no_cover );
      add_tc_req_act(ET_LINK_REQ_WriteAround,  SC_L2_WriteAround,  E,    { A1_L_Bsy },  `SCQ_Partial,  A1_W_Bsy,      Data_MemWr,      BusIdle,    RspAck,   EC_EnqWA,        m_no_cover );
      //add_tc_req_act(ET_LINK_REQ_WriteAround,  SC_L2_WriteAround,  E,    { A1_L_Bsy },  `SCQ_None,      this one should come up eventually...
      

      add_tc_req_act(ET_LINK_REQ_WriteAround,  SC_L2_WriteAround,  E,    { A1_D_Bsy },  `SCQ_None,     A1_WI_Bsy,     Data_MrgWrBack,  BusVictim,  RspAck,   EC_Victim_ClrQW );
      // An index cachop evict, followed by a read, and now a WriteAround
      add_tc_req_act(ET_LINK_REQ_WriteAround,  SC_L2_WriteAround,  E,    { A1_E_Bsy },  `SCQ_Partial,  A1_W_Bsy,      Data_MemWr,  BusIdle,   RspAck,   EC_EnqWA, m_no_cover );
      add_tc_req_act(ET_LINK_REQ_WriteAround,  SC_L2_WriteAround,  E,    { A1_E_Bsy },  `SCQ_None,     A1_WI_Bsy,     Data_MrgWrBack,  BusVictim, RspAck,  EC_Victim_ClrQW, m_no_cover );     

      // Remote Scratchpad Writearound
      add_tc_req_act(ET_LINK_REQ_WriteAround,  SC_L2_WriteAround,  I,    { A1_I_Bsy },  (`SCQ_Remote|`SCQ_Partial),  A1_W_Bsy,      Data_MemWrInst,  BusIdle,    RspAck,   EC_EnqWA        );
      add_tc_req_act(ET_LINK_REQ_WriteAround,  SC_L2_WriteAround,  I,    { A1_I_Bsy },  `SCQ_Remote,                 A1_WI_Bsy,     Data_MrgWrBack,  BusVictim,  RspAck,   EC_Victim_ClrQW );

      //
      // ET Link Cacheops (Flush)
      //                                                Obs   Model                                                         Bus
      //             ET-link Opcode      L2 Opcode     State  State           Qual          Tag Action     Data Action      Command     Rsp Type  Err Code
      //             ------------------  ------------  -----  --------------  ------------  -------------  ---------------  ----------  --------  ---------------
      add_tc_req_act(ET_LINK_REQ_Flush,  SC_L2_Flush,  I,     { I },          `SCQ_L1_L3,   St_Ignore,     Data_None,       BusIdle,    RspAck,  EC_None         );
      add_tc_req_act(ET_LINK_REQ_Flush,  SC_L2_Flush,  I,     { A1_I_Bsy },   `SCQ_L1_L3,   St_Ignore,     Data_None,       BusIdle,    RspAck,  EC_None         );
      add_tc_req_act(ET_LINK_REQ_Flush,  SC_L2_Flush,  I,     { A2_I_Bsy },   `SCQ_L1_L3,   St_Ignore,     Data_None,       BusIdle,    RspAck,  EC_None,        m_no_cover );
      add_tc_req_act(ET_LINK_REQ_Flush,  SC_L2_Flush,  E,     { E },          `SCQ_L1_L3,   St_Ignore,     Data_None,       BusIdle,    RspAck,  EC_None         );
      add_tc_req_act(ET_LINK_REQ_Flush,  SC_L2_Flush,  E,     { A1_E_Bsy },   `SCQ_L1_L3,   St_Ignore,     Data_None,       BusIdle,    RspAck,  EC_None,        m_no_cover );
      add_tc_req_act(ET_LINK_REQ_Flush,  SC_L2_Flush,  E,     { L },          `SCQ_L1_L3,   St_Ignore,     Data_None,       BusIdle,    RspAck,  EC_None         );
      add_tc_req_act(ET_LINK_REQ_Flush,  SC_L2_Flush,  E,     { M },          `SCQ_L1_L3,   ME_Bsy,        Data_ReqWrBack,  BusVictim,  RspAck,  EC_Victim_ClrQW );
      add_tc_req_act(ET_LINK_REQ_Flush,  SC_L2_Flush,  E,     { A1_M_Bsy },   `SCQ_L1_L3,   A1_ME_Bsy,     Data_ReqWrBack,  BusVictim,  RspAck,  EC_Victim_ClrQW );
      add_tc_req_act(ET_LINK_REQ_Flush,  SC_L2_Flush,  E,     { D },          `SCQ_L1_L3,   St_Ignore,     Data_None,       BusIdle,    RspAck,  EC_None         );
      add_tc_req_act(ET_LINK_REQ_Flush,  SC_L2_Flush,  E,     { A1_D_Bsy },   `SCQ_L1_L3,   St_Ignore,     Data_None,       BusIdle,    RspAck,  EC_None         );
      add_tc_req_act(ET_LINK_REQ_Flush,  SC_L2_Flush,  E,     { W },          `SCQ_L1_L3,   WI_Bsy,        Data_ReqWrBack,  BusVictim,  RspAck,  EC_Victim_ClrQW );
      add_tc_req_act(ET_LINK_REQ_Flush,  SC_L2_Flush,  E,     { A1_W_Bsy },   `SCQ_L1_L3,   A1_WI_Bsy,     Data_ReqWrBack,  BusVictim,  RspAck,  EC_Victim_ClrQW, m_no_cover );

      // Flush to L3 but we're not sending anything to the L3
      add_tc_req_act(ET_LINK_REQ_Flush,  SC_L2_Flush,  I,     { I },          (`SCQ_L1_L3 | `SCQ_DownDis),   St_Ignore,     Data_None,       BusIdle,    RspAck,  EC_None  );
      add_tc_req_act(ET_LINK_REQ_Flush,  SC_L2_Flush,  I,     { A1_I_Bsy },   (`SCQ_L1_L3 | `SCQ_DownDis),   St_Ignore,     Data_None,       BusIdle,    RspAck,  EC_None  );
      add_tc_req_act(ET_LINK_REQ_Flush,  SC_L2_Flush,  I,     { A2_I_Bsy },   (`SCQ_L1_L3 | `SCQ_DownDis),   St_Ignore,     Data_None,       BusIdle,    RspAck,  EC_None, m_no_cover );
      add_tc_req_act(ET_LINK_REQ_Flush,  SC_L2_Flush,  E,     { E },          (`SCQ_L1_L3 | `SCQ_DownDis),   St_Ignore,     Data_None,       BusIdle,    RspAck,  EC_None  );
      add_tc_req_act(ET_LINK_REQ_Flush,  SC_L2_Flush,  E,     { A1_E_Bsy },   (`SCQ_L1_L3 | `SCQ_DownDis),   St_Ignore,     Data_None,       BusIdle,    RspAck,  EC_None, m_no_cover );
      add_tc_req_act(ET_LINK_REQ_Flush,  SC_L2_Flush,  E,     { L },          (`SCQ_L1_L3 | `SCQ_DownDis),   St_Ignore,     Data_None,       BusIdle,    RspAck,  EC_None  );
      add_tc_req_act(ET_LINK_REQ_Flush,  SC_L2_Flush,  E,     { M },          (`SCQ_L1_L3 | `SCQ_DownDis),   ME_Bsy,        Data_ReqWrBack,  BusVictim,  RspAck,  EC_Victim_ClrQW );
      add_tc_req_act(ET_LINK_REQ_Flush,  SC_L2_Flush,  E,     { A1_M_Bsy },   (`SCQ_L1_L3 | `SCQ_DownDis),   A1_ME_Bsy,     Data_ReqWrBack,  BusVictim,  RspAck,  EC_Victim_ClrQW );
      add_tc_req_act(ET_LINK_REQ_Flush,  SC_L2_Flush,  E,     { D },          (`SCQ_L1_L3 | `SCQ_DownDis),   St_Ignore,     Data_None,       BusIdle,    RspAck,  EC_None  );
      add_tc_req_act(ET_LINK_REQ_Flush,  SC_L2_Flush,  E,     { A1_D_Bsy },   (`SCQ_L1_L3 | `SCQ_DownDis),   St_Ignore,     Data_None,       BusIdle,    RspAck,  EC_None  );
      add_tc_req_act(ET_LINK_REQ_Flush,  SC_L2_Flush,  E,     { W },          (`SCQ_L1_L3 | `SCQ_DownDis),   WI_Bsy,        Data_ReqWrBack,  BusVictim,  RspAck,  EC_Victim_ClrQW );
      add_tc_req_act(ET_LINK_REQ_Flush,  SC_L2_Flush,  E,     { A1_W_Bsy },   (`SCQ_L1_L3 | `SCQ_DownDis),   A1_WI_Bsy,     Data_ReqWrBack,  BusVictim,  RspAck,  EC_Victim_ClrQW, m_no_cover );

      add_tc_req_act(ET_LINK_REQ_Flush,  SC_L2_Flush,  I,     { I },          `SCQ_L2_L3,   St_Ignore,     Data_None,       BusIdle,    RspAck,   EC_None         );
      add_tc_req_act(ET_LINK_REQ_Flush,  SC_L2_Flush,  I,     { A1_I_Bsy },   `SCQ_L2_L3,   St_Ignore,     Data_None,       BusIdle,    RspAck,   EC_None         );
      add_tc_req_act(ET_LINK_REQ_Flush,  SC_L2_Flush,  I,     { A2_I_Bsy },   `SCQ_L2_L3,   St_Ignore,     Data_None,       BusIdle,    RspAck,   EC_None,       m_no_cover );
      add_tc_req_act(ET_LINK_REQ_Flush,  SC_L2_Flush,  E,     { E },          `SCQ_L2_L3,   St_Ignore,     Data_None,       BusIdle,    RspAck,  EC_None         );
      add_tc_req_act(ET_LINK_REQ_Flush,  SC_L2_Flush,  E,     { A1_E_Bsy },   `SCQ_L2_L3,   St_Ignore,     Data_None,       BusIdle,    RspAck,  EC_None,        m_no_cover );
      add_tc_req_act(ET_LINK_REQ_Flush,  SC_L2_Flush,  E,     { L },          `SCQ_L2_L3,   St_Ignore,     Data_None,       BusIdle,    RspAck,  EC_None         );
      add_tc_req_act(ET_LINK_REQ_Flush,  SC_L2_Flush,  E,     { M },          `SCQ_L2_L3,   ME_Bsy,        Data_ReqWrBack,  BusVictim,  RspAck,  EC_Victim_ClrQW );
      add_tc_req_act(ET_LINK_REQ_Flush,  SC_L2_Flush,  E,     { A1_M_Bsy },   `SCQ_L2_L3,   A1_ME_Bsy,     Data_ReqWrBack,  BusVictim,  RspAck,  EC_Victim_ClrQW );
      add_tc_req_act(ET_LINK_REQ_Flush,  SC_L2_Flush,  E,     { D },          `SCQ_L2_L3,   St_Ignore,     Data_None,       BusIdle,    RspAck,  EC_None         );
      add_tc_req_act(ET_LINK_REQ_Flush,  SC_L2_Flush,  E,     { A1_D_Bsy },   `SCQ_L2_L3,   St_Ignore,     Data_None,       BusIdle,    RspAck,  EC_None         );
      add_tc_req_act(ET_LINK_REQ_Flush,  SC_L2_Flush,  E,     { W },          `SCQ_L2_L3,   WI_Bsy,        Data_ReqWrBack,  BusVictim,  RspAck,  EC_Victim_ClrQW );
      add_tc_req_act(ET_LINK_REQ_Flush,  SC_L2_Flush,  E,     { A1_W_Bsy },   `SCQ_L2_L3,   A1_WI_Bsy,     Data_ReqWrBack,  BusVictim,  RspAck,  EC_Victim_ClrQW, m_no_cover );

      // Flush to L3 but we're not sending anything to the L3
      add_tc_req_act(ET_LINK_REQ_Flush,  SC_L2_Flush,  I,     { I },          (`SCQ_L2_L3 | `SCQ_DownDis),   St_Ignore,     Data_None,       BusIdle,    RspAck,  EC_None  );
      add_tc_req_act(ET_LINK_REQ_Flush,  SC_L2_Flush,  I,     { A1_I_Bsy },   (`SCQ_L2_L3 | `SCQ_DownDis),   St_Ignore,     Data_None,       BusIdle,    RspAck,  EC_None  );
      add_tc_req_act(ET_LINK_REQ_Flush,  SC_L2_Flush,  I,     { A2_I_Bsy },   (`SCQ_L2_L3 | `SCQ_DownDis),   St_Ignore,     Data_None,       BusIdle,    RspAck,  EC_None, m_no_cover );
      add_tc_req_act(ET_LINK_REQ_Flush,  SC_L2_Flush,  E,     { E },          (`SCQ_L2_L3 | `SCQ_DownDis),   St_Ignore,     Data_None,       BusIdle,    RspAck,  EC_None  );
      add_tc_req_act(ET_LINK_REQ_Flush,  SC_L2_Flush,  E,     { A1_E_Bsy },   (`SCQ_L2_L3 | `SCQ_DownDis),   St_Ignore,     Data_None,       BusIdle,    RspAck,  EC_None, m_no_cover );
      add_tc_req_act(ET_LINK_REQ_Flush,  SC_L2_Flush,  E,     { L },          (`SCQ_L2_L3 | `SCQ_DownDis),   St_Ignore,     Data_None,       BusIdle,    RspAck,  EC_None  );
      add_tc_req_act(ET_LINK_REQ_Flush,  SC_L2_Flush,  E,     { M },          (`SCQ_L2_L3 | `SCQ_DownDis),   ME_Bsy,        Data_ReqWrBack,  BusVictim,  RspAck,  EC_Victim_ClrQW );
      add_tc_req_act(ET_LINK_REQ_Flush,  SC_L2_Flush,  E,     { A1_M_Bsy },   (`SCQ_L2_L3 | `SCQ_DownDis),   A1_ME_Bsy,     Data_ReqWrBack,  BusVictim,  RspAck,  EC_Victim_ClrQW );
      add_tc_req_act(ET_LINK_REQ_Flush,  SC_L2_Flush,  E,     { D },          (`SCQ_L2_L3 | `SCQ_DownDis),   St_Ignore,     Data_None,       BusIdle,    RspAck,  EC_None  );
      add_tc_req_act(ET_LINK_REQ_Flush,  SC_L2_Flush,  E,     { A1_D_Bsy },   (`SCQ_L2_L3 | `SCQ_DownDis),   St_Ignore,     Data_None,       BusIdle,    RspAck,  EC_None  );
      add_tc_req_act(ET_LINK_REQ_Flush,  SC_L2_Flush,  E,     { W },          (`SCQ_L2_L3 | `SCQ_DownDis),   WI_Bsy,        Data_ReqWrBack,  BusVictim,  RspAck,  EC_Victim_ClrQW );
      add_tc_req_act(ET_LINK_REQ_Flush,  SC_L2_Flush,  E,     { A1_W_Bsy },   (`SCQ_L2_L3 | `SCQ_DownDis),   A1_WI_Bsy,     Data_ReqWrBack,  BusVictim,  RspAck,  EC_Victim_ClrQW, m_no_cover );

      add_tc_req_act(ET_LINK_REQ_Flush,  SC_L2_Flush,  I,     { I },          `SCQ_L1_Mem,  A1_I_Bsy,     Data_ReqWrNull,  BusFlush,   RspAck,  EC_None         );
      add_tc_req_act(ET_LINK_REQ_Flush,  SC_L2_Flush,  I,     { A1_I_Bsy },   `SCQ_L1_Mem,  A2_I_Bsy,     Data_ReqWrNull,  BusFlush,   RspAck,  EC_None         );
      add_tc_req_act(ET_LINK_REQ_Flush,  SC_L2_Flush,  I,     { A2_I_Bsy },   `SCQ_L1_Mem,  A3_I_Bsy,     Data_ReqWrNull,  BusFlush,   RspAck,  EC_None, m_no_cover );
      add_tc_req_act(ET_LINK_REQ_Flush,  SC_L2_Flush,  E,     { E },          `SCQ_L1_Mem,  A1_E_Bsy,     Data_ReqWrNull,  BusFlush,   RspAck,  EC_None         );
      add_tc_req_act(ET_LINK_REQ_Flush,  SC_L2_Flush,  E,     { A1_E_Bsy },   `SCQ_L1_Mem,  A2_E_Bsy,     Data_ReqWrNull,  BusFlush,   RspAck,  EC_None, m_no_cover );
      add_tc_req_act(ET_LINK_REQ_Flush,  SC_L2_Flush,  E,     { M },          `SCQ_L1_Mem,  ME_MemBsy,     Data_ReqWrBack,  BusVictim,  RspAck,  EC_Victim_ClrQW );
      add_tc_req_act(ET_LINK_REQ_Flush,  SC_L2_Flush,  E,     { A1_M_Bsy },   `SCQ_L1_Mem,  A1_ME_MemBsy,  Data_ReqWrBack,  BusVictim,  RspAck,  EC_Victim_ClrQW );
      add_tc_req_act(ET_LINK_REQ_Flush,  SC_L2_Flush,  E,     { L },          `SCQ_L1_Mem,  A1_L_Bsy,      Data_ReqWrNull,  BusFlush,   RspAck,  EC_None         );
      //add_tc_req_act(ET_LINK_REQ_Flush,  SC_L2_Flush,  E,     { A1_L_Bsy },   `SCQ_L1_Mem,  A2_L_Bsy,      Data_ReqWrNull,  BusFlush,   RspAck,  EC_None         );
      add_tc_req_act(ET_LINK_REQ_Flush,  SC_L2_Flush,  E,     { D },          `SCQ_L1_Mem,  A1_D_Bsy,      Data_ReqWrNull,  BusFlush,   RspAck,  EC_None         );
      add_tc_req_act(ET_LINK_REQ_Flush,  SC_L2_Flush,  E,     { A1_D_Bsy },   `SCQ_L1_Mem,  A2_D_Bsy,      Data_ReqWrNull,  BusFlush,    RspAck,  EC_None         );
      add_tc_req_act(ET_LINK_REQ_Flush,  SC_L2_Flush,  E,     { W },          `SCQ_L1_Mem,  WI_MemBsy,     Data_ReqWrBack,  BusVictim,  RspAck,  EC_Victim_ClrQW );
      add_tc_req_act(ET_LINK_REQ_Flush,  SC_L2_Flush,  E,     { A1_W_Bsy },   `SCQ_L1_Mem,  A1_WI_MemBsy,  Data_ReqWrBack,  BusVictim,  RspAck,  EC_Victim_ClrQW, m_no_cover );

      // flush to Mem but we're not sending anything to the L3
      add_tc_req_act(ET_LINK_REQ_Flush,  SC_L2_Flush,  I,     { I },          (`SCQ_L1_Mem | `SCQ_DownDis),  St_Ignore,     Data_None,       BusIdle,    RspAck,  EC_None  );
      add_tc_req_act(ET_LINK_REQ_Flush,  SC_L2_Flush,  I,     { A1_I_Bsy },   (`SCQ_L1_Mem | `SCQ_DownDis),  St_Ignore,     Data_None,       BusIdle,    RspAck,  EC_None  );
      add_tc_req_act(ET_LINK_REQ_Flush,  SC_L2_Flush,  I,     { A2_I_Bsy },   (`SCQ_L1_Mem | `SCQ_DownDis),  St_Ignore,     Data_None,       BusIdle,    RspAck,  EC_None, m_no_cover );
      add_tc_req_act(ET_LINK_REQ_Flush,  SC_L2_Flush,  E,     { E },          (`SCQ_L1_Mem | `SCQ_DownDis),  St_Ignore,     Data_None,       BusIdle,    RspAck,  EC_None  );
      add_tc_req_act(ET_LINK_REQ_Flush,  SC_L2_Flush,  E,     { A1_E_Bsy },   (`SCQ_L1_Mem | `SCQ_DownDis),  St_Ignore,     Data_None,       BusIdle,    RspAck,  EC_None, m_no_cover );
      add_tc_req_act(ET_LINK_REQ_Flush,  SC_L2_Flush,  E,     { M },          (`SCQ_L1_Mem | `SCQ_DownDis),  ME_MemBsy,     Data_ReqWrBack,  BusVictim,  RspAck,  EC_Victim_ClrQW );
      add_tc_req_act(ET_LINK_REQ_Flush,  SC_L2_Flush,  E,     { A1_M_Bsy },   (`SCQ_L1_Mem | `SCQ_DownDis),  A1_ME_MemBsy,  Data_ReqWrBack,  BusVictim,  RspAck,  EC_Victim_ClrQW );
      add_tc_req_act(ET_LINK_REQ_Flush,  SC_L2_Flush,  E,     { L },          (`SCQ_L1_Mem | `SCQ_DownDis),  St_Ignore,     Data_None,       BusIdle,    RspAck,  EC_None  );
      add_tc_req_act(ET_LINK_REQ_Flush,  SC_L2_Flush,  E,     { D },          (`SCQ_L1_Mem | `SCQ_DownDis),  St_Ignore,     Data_None,       BusIdle,    RspAck,  EC_None  );
      add_tc_req_act(ET_LINK_REQ_Flush,  SC_L2_Flush,  E,     { A1_D_Bsy },   (`SCQ_L1_Mem | `SCQ_DownDis),  St_Ignore,     Data_None,       BusIdle,    RspAck,  EC_None  );
      add_tc_req_act(ET_LINK_REQ_Flush,  SC_L2_Flush,  E,     { W },          (`SCQ_L1_Mem | `SCQ_DownDis),  WI_MemBsy,     Data_ReqWrBack,  BusVictim,  RspAck,  EC_Victim_ClrQW );
      add_tc_req_act(ET_LINK_REQ_Flush,  SC_L2_Flush,  E,     { A1_W_Bsy },   (`SCQ_L1_Mem | `SCQ_DownDis),  A1_WI_MemBsy,  Data_ReqWrBack,  BusVictim,  RspAck,  EC_Victim_ClrQW, m_no_cover );
      

      add_tc_req_act(ET_LINK_REQ_Flush,  SC_L2_Flush,  I,     { I },          `SCQ_L2_Mem,  A1_I_Bsy,      Data_ReqWrNull,  BusFlush,   RspAck,  EC_None         );
      add_tc_req_act(ET_LINK_REQ_Flush,  SC_L2_Flush,  I,     { A1_I_Bsy },   `SCQ_L2_Mem,  A2_I_Bsy,      Data_ReqWrNull,  BusFlush,   RspAck,  EC_None         );
      add_tc_req_act(ET_LINK_REQ_Flush,  SC_L2_Flush,  I,     { A2_I_Bsy },   `SCQ_L2_Mem,  A3_I_Bsy,      Data_ReqWrNull,  BusFlush,   RspAck,  EC_None, m_no_cover );

      add_tc_req_act(ET_LINK_REQ_Flush,  SC_L2_Flush,  E,     { E },          `SCQ_L2_Mem,  A1_E_Bsy,      Data_ReqWrNull,  BusFlush,   RspAck,  EC_None         );
      add_tc_req_act(ET_LINK_REQ_Flush,  SC_L2_Flush,  E,     { A1_E_Bsy },   `SCQ_L2_Mem,  A2_E_Bsy,      Data_ReqWrNull,  BusFlush,   RspAck,  EC_None, m_no_cover );
      add_tc_req_act(ET_LINK_REQ_Flush,  SC_L2_Flush,  E,     { M },          `SCQ_L2_Mem,  ME_MemBsy,     Data_ReqWrBack,  BusVictim,  RspAck,  EC_Victim_ClrQW );
      add_tc_req_act(ET_LINK_REQ_Flush,  SC_L2_Flush,  E,     { A1_M_Bsy },   `SCQ_L2_Mem,  A1_ME_MemBsy,  Data_ReqWrBack,  BusVictim,  RspAck,  EC_Victim_ClrQW );
      add_tc_req_act(ET_LINK_REQ_Flush,  SC_L2_Flush,  E,     { L },          `SCQ_L2_Mem,  A1_L_Bsy,      Data_ReqWrNull,  BusFlush,   RspAck,  EC_None         );
      //add_tc_req_act(ET_LINK_REQ_Flush,  SC_L2_Flush,  E,     { A1_L_Bsy },   `SCQ_L2_Mem,  A2_L_Bsy,      Data_ReqWrNull,  BusFlush,   RspAck,  EC_None         );
      add_tc_req_act(ET_LINK_REQ_Flush,  SC_L2_Flush,  E,     { D },          `SCQ_L2_Mem,  A1_D_Bsy,      Data_ReqWrNull,  BusFlush,   RspAck,  EC_None         );
      add_tc_req_act(ET_LINK_REQ_Flush,  SC_L2_Flush,  E,     { A1_D_Bsy },   `SCQ_L2_Mem,  A2_D_Bsy,      Data_ReqWrNull,  BusFlush,   RspAck,  EC_None         );
      add_tc_req_act(ET_LINK_REQ_Flush,  SC_L2_Flush,  E,     { W },          `SCQ_L2_Mem,  WI_MemBsy,     Data_ReqWrBack,  BusVictim,  RspAck,  EC_Victim_ClrQW );
      add_tc_req_act(ET_LINK_REQ_Flush,  SC_L2_Flush,  E,     { A1_W_Bsy },   `SCQ_L2_Mem,  A1_WI_MemBsy,  Data_ReqWrBack,  BusVictim,  RspAck,  EC_Victim_ClrQW, m_no_cover );

      // flush to mem but we're not sending anything to the L3
      add_tc_req_act(ET_LINK_REQ_Flush,  SC_L2_Flush,  I,     { I },          (`SCQ_L2_Mem | `SCQ_DownDis),  St_Ignore,     Data_None,       BusIdle,    RspAck,  EC_None  );
      add_tc_req_act(ET_LINK_REQ_Flush,  SC_L2_Flush,  I,     { A1_I_Bsy },   (`SCQ_L2_Mem | `SCQ_DownDis),  St_Ignore,     Data_None,       BusIdle,    RspAck,  EC_None  );
      add_tc_req_act(ET_LINK_REQ_Flush,  SC_L2_Flush,  I,     { A2_I_Bsy },   (`SCQ_L2_Mem | `SCQ_DownDis),  St_Ignore,     Data_None,       BusIdle,    RspAck,  EC_None, m_no_cover );
      add_tc_req_act(ET_LINK_REQ_Flush,  SC_L2_Flush,  E,     { E },          (`SCQ_L2_Mem | `SCQ_DownDis),  St_Ignore,     Data_None,       BusIdle,    RspAck,  EC_None  );
      add_tc_req_act(ET_LINK_REQ_Flush,  SC_L2_Flush,  E,     { A1_E_Bsy },   (`SCQ_L2_Mem | `SCQ_DownDis),  St_Ignore,     Data_None,       BusIdle,    RspAck,  EC_None, m_no_cover );
      add_tc_req_act(ET_LINK_REQ_Flush,  SC_L2_Flush,  E,     { M },          (`SCQ_L2_Mem | `SCQ_DownDis),  ME_MemBsy,     Data_ReqWrBack,  BusVictim,  RspAck,  EC_Victim_ClrQW );
      add_tc_req_act(ET_LINK_REQ_Flush,  SC_L2_Flush,  E,     { A1_M_Bsy },   (`SCQ_L2_Mem | `SCQ_DownDis),  A1_ME_MemBsy,  Data_ReqWrBack,  BusVictim,  RspAck,  EC_Victim_ClrQW );
      add_tc_req_act(ET_LINK_REQ_Flush,  SC_L2_Flush,  E,     { L },          (`SCQ_L2_Mem | `SCQ_DownDis),  St_Ignore,     Data_None,       BusIdle,    RspAck,  EC_None  );
      add_tc_req_act(ET_LINK_REQ_Flush,  SC_L2_Flush,  E,     { D },          (`SCQ_L2_Mem | `SCQ_DownDis),  St_Ignore,     Data_None,       BusIdle,    RspAck,  EC_None  );
      add_tc_req_act(ET_LINK_REQ_Flush,  SC_L2_Flush,  E,     { A1_D_Bsy },   (`SCQ_L2_Mem | `SCQ_DownDis),  St_Ignore,     Data_None,       BusIdle,    RspAck,  EC_None  );
      add_tc_req_act(ET_LINK_REQ_Flush,  SC_L2_Flush,  E,     { W },          (`SCQ_L2_Mem | `SCQ_DownDis),  WI_MemBsy,     Data_ReqWrBack,  BusVictim,  RspAck,  EC_Victim_ClrQW );
      add_tc_req_act(ET_LINK_REQ_Flush,  SC_L2_Flush,  E,     { A1_W_Bsy },   (`SCQ_L2_Mem | `SCQ_DownDis),  A1_WI_MemBsy,  Data_ReqWrBack,  BusVictim,  RspAck,  EC_Victim_ClrQW, m_no_cover );
      

// The following entries should not really exist, but we can have illegal source and destination fields:
      // For coverage, only worry about the one that's covered in the limited directed testing.
      add_tc_req_act(ET_LINK_REQ_Flush,  SC_L2_Flush,  I,     { I },          `SCQ_None,    St_Ignore,     Data_None,       BusIdle,    RspAck,  EC_None,         m_no_cover );
      add_tc_req_act(ET_LINK_REQ_Flush,  SC_L2_Flush,  I,     { A1_I_Bsy },   `SCQ_None,    St_Ignore,     Data_None,       BusIdle,    RspAck,  EC_None,         m_no_cover );
      add_tc_req_act(ET_LINK_REQ_Flush,  SC_L2_Flush,  I,     { A2_I_Bsy },   `SCQ_None,    St_Ignore,     Data_None,       BusIdle,    RspAck,  EC_None,         m_no_cover );
      add_tc_req_act(ET_LINK_REQ_Flush,  SC_L2_Flush,  E,     { E },          `SCQ_None,    St_Ignore,     Data_None,       BusIdle,    RspAck,  EC_None,         m_no_cover );
      add_tc_req_act(ET_LINK_REQ_Flush,  SC_L2_Flush,  E,     { A1_E_Bsy },   `SCQ_None,    St_Ignore,     Data_None,       BusIdle,    RspAck,  EC_None,         m_no_cover );
      add_tc_req_act(ET_LINK_REQ_Flush,  SC_L2_Flush,  E,     { M },          `SCQ_None,    ME_Bsy,        Data_ReqWrBack,  BusVictim,  RspAck,  EC_Victim_ClrQW );
      add_tc_req_act(ET_LINK_REQ_Flush,  SC_L2_Flush,  E,     { A1_M_Bsy },   `SCQ_None,    A1_ME_Bsy,     Data_ReqWrBack,  BusVictim,  RspAck,  EC_Victim_ClrQW, m_no_cover );
      add_tc_req_act(ET_LINK_REQ_Flush,  SC_L2_Flush,  E,     { L },          `SCQ_None,    St_Ignore,     Data_None,       BusIdle,    RspAck,  EC_None,         m_no_cover );
      add_tc_req_act(ET_LINK_REQ_Flush,  SC_L2_Flush,  E,     { D },          `SCQ_None,    St_Ignore,     Data_None,       BusIdle,    RspAck,  EC_None,         m_no_cover );
      add_tc_req_act(ET_LINK_REQ_Flush,  SC_L2_Flush,  E,     { A1_D_Bsy },   `SCQ_None,    St_Ignore,     Data_None,       BusIdle,    RspAck,  EC_None,         m_no_cover );
      add_tc_req_act(ET_LINK_REQ_Flush,  SC_L2_Flush,  E,     { W },          `SCQ_None,    WI_Bsy,        Data_ReqWrBack,  BusVictim,  RspAck,  EC_Victim_ClrQW, m_no_cover );
      add_tc_req_act(ET_LINK_REQ_Flush,  SC_L2_Flush,  E,     { A1_W_Bsy },   `SCQ_None,    A1_WI_Bsy,     Data_ReqWrBack,  BusVictim,  RspAck,  EC_Victim_ClrQW, m_no_cover );

      add_tc_req_act(ET_LINK_REQ_Flush,  SC_L2_Flush,  I,     { I },          (`SCQ_None | `SCQ_DownDis),    St_Ignore,     Data_None,       BusIdle,    RspAck,  EC_None,         m_no_cover );
      add_tc_req_act(ET_LINK_REQ_Flush,  SC_L2_Flush,  I,     { A1_I_Bsy },   (`SCQ_None | `SCQ_DownDis),    St_Ignore,     Data_None,       BusIdle,    RspAck,  EC_None,         m_no_cover );
      add_tc_req_act(ET_LINK_REQ_Flush,  SC_L2_Flush,  I,     { A2_I_Bsy },   (`SCQ_None | `SCQ_DownDis),    St_Ignore,     Data_None,       BusIdle,    RspAck,  EC_None,         m_no_cover );
      add_tc_req_act(ET_LINK_REQ_Flush,  SC_L2_Flush,  E,     { E },          (`SCQ_None | `SCQ_DownDis),    St_Ignore,     Data_None,       BusIdle,    RspAck,  EC_None,         m_no_cover );
      add_tc_req_act(ET_LINK_REQ_Flush,  SC_L2_Flush,  E,     { A1_E_Bsy },   (`SCQ_None | `SCQ_DownDis),    St_Ignore,     Data_None,       BusIdle,    RspAck,  EC_None,         m_no_cover );
      add_tc_req_act(ET_LINK_REQ_Flush,  SC_L2_Flush,  E,     { M },          (`SCQ_None | `SCQ_DownDis),    ME_Bsy,        Data_ReqWrBack,  BusVictim,  RspAck,  EC_Victim_ClrQW );
      add_tc_req_act(ET_LINK_REQ_Flush,  SC_L2_Flush,  E,     { A1_M_Bsy },   (`SCQ_None | `SCQ_DownDis),    A1_ME_Bsy,     Data_ReqWrBack,  BusVictim,  RspAck,  EC_Victim_ClrQW, m_no_cover );
      add_tc_req_act(ET_LINK_REQ_Flush,  SC_L2_Flush,  E,     { L },          (`SCQ_None | `SCQ_DownDis),    St_Ignore,     Data_None,       BusIdle,    RspAck,  EC_None,         m_no_cover );
      add_tc_req_act(ET_LINK_REQ_Flush,  SC_L2_Flush,  E,     { D },          (`SCQ_None | `SCQ_DownDis),    St_Ignore,     Data_None,       BusIdle,    RspAck,  EC_None,         m_no_cover );
      add_tc_req_act(ET_LINK_REQ_Flush,  SC_L2_Flush,  E,     { A1_D_Bsy },   (`SCQ_None | `SCQ_DownDis),    St_Ignore,     Data_None,       BusIdle,    RspAck,  EC_None,         m_no_cover );
      add_tc_req_act(ET_LINK_REQ_Flush,  SC_L2_Flush,  E,     { W },          (`SCQ_None | `SCQ_DownDis),    WI_Bsy,        Data_ReqWrBack,  BusVictim,  RspAck,  EC_Victim_ClrQW, m_no_cover );
      add_tc_req_act(ET_LINK_REQ_Flush,  SC_L2_Flush,  E,     { A1_W_Bsy },   (`SCQ_None | `SCQ_DownDis),    A1_WI_Bsy,     Data_ReqWrBack,  BusVictim,  RspAck,  EC_Victim_ClrQW, m_no_cover );
      

      // Note: The L3->Mem Flushes are a special case taken care of in the REQQ alloc portion of the table.

      //Flush when there's No L2:
      add_tc_req_act(ET_LINK_REQ_Flush,  SC_L2_Flush,  I,     { I },  (`SCQ_NoRegion | `SCQ_L1_L3),   St_Ignore,     Data_None,       BusIdle,    RspAck,  EC_MaybeErrRsp  );
      add_tc_req_act(ET_LINK_REQ_Flush,  SC_L2_Flush,  I,     { I },  (`SCQ_NoRegion | `SCQ_L2_L3),   St_Ignore,     Data_None,       BusIdle,    RspAck,  EC_MaybeErrRsp  );
      add_tc_req_act(ET_LINK_REQ_Flush,  SC_L2_Flush,  I,     { I },  (`SCQ_NoRegion | `SCQ_L1_Mem),  St_Ignore,     Data_None,       BusIdle,    RspAck,  EC_MaybeErrRsp  );
      add_tc_req_act(ET_LINK_REQ_Flush,  SC_L2_Flush,  I,     { I },  (`SCQ_NoRegion | `SCQ_L2_Mem),  St_Ignore,     Data_None,       BusIdle,    RspAck,  EC_MaybeErrRsp  );
      add_tc_req_act(ET_LINK_REQ_Flush,  SC_L2_Flush,  I,     { I },  (`SCQ_NoRegion),                St_Ignore,     Data_None,       BusIdle,    RspAck,  EC_MaybeErrRsp  );

      add_tc_req_act(ET_LINK_REQ_Flush,  SC_L2_Flush,  I,     { I },  (`SCQ_NoRegion | `SCQ_DownDis | `SCQ_L1_L3),   St_Ignore,     Data_None,       BusIdle,    RspAck,  EC_MaybeErrRsp );
      add_tc_req_act(ET_LINK_REQ_Flush,  SC_L2_Flush,  I,     { I },  (`SCQ_NoRegion | `SCQ_DownDis | `SCQ_L2_L3),   St_Ignore,     Data_None,       BusIdle,    RspAck,  EC_MaybeErrRsp );
      add_tc_req_act(ET_LINK_REQ_Flush,  SC_L2_Flush,  I,     { I },  (`SCQ_NoRegion | `SCQ_DownDis | `SCQ_L1_Mem),  St_Ignore,     Data_None,       BusIdle,    RspAck,  EC_MaybeErrRsp );
      add_tc_req_act(ET_LINK_REQ_Flush,  SC_L2_Flush,  I,     { I },  (`SCQ_NoRegion | `SCQ_DownDis | `SCQ_L2_Mem),  St_Ignore,     Data_None,       BusIdle,    RspAck,  EC_MaybeErrRsp );
      add_tc_req_act(ET_LINK_REQ_Flush,  SC_L2_Flush,  I,     { I },  (`SCQ_NoRegion | `SCQ_DownDis),                St_Ignore,     Data_None,       BusIdle,    RspAck,  EC_MaybeErrRsp );
      
      //
      // ET Link Cacheops (Evict)
      //                                                Obs   Model                                                         Bus
      //             ET-link Opcode      L2 Opcode     State  State           Qual          Tag Action     Data Action      Command     Rsp Type  Err Code
      //             ------------------  ------------  -----  --------------  ------------  -------------  ---------------  ----------  --------  ---------------
      add_tc_req_act(ET_LINK_REQ_Evict,  SC_L2_Evict,  I,     { I },          `SCQ_L1_L3,   St_Ignore,     Data_None,       BusIdle,    RspAck,  EC_None         );
      add_tc_req_act(ET_LINK_REQ_Evict,  SC_L2_Evict,  I,     { A1_I_Bsy },   `SCQ_L1_L3,   St_Ignore,     Data_None,       BusIdle,    RspAck,  EC_None         );
      add_tc_req_act(ET_LINK_REQ_Evict,  SC_L2_Evict,  I,     { A2_I_Bsy },   `SCQ_L1_L3,   St_Ignore,     Data_None,       BusIdle,    RspAck,  EC_None, m_no_cover );
      add_tc_req_act(ET_LINK_REQ_Evict,  SC_L2_Evict,  E,     { E },          `SCQ_L1_L3,   I,             Data_None,       BusIdle,    RspAck,  EC_Victim       );
      add_tc_req_act(ET_LINK_REQ_Evict,  SC_L2_Evict,  E,     { A1_E_Bsy },   `SCQ_L1_L3,   A1_I_Bsy,      Data_None,       BusIdle,    RspAck,  EC_Victim, m_no_cover );
      add_tc_req_act(ET_LINK_REQ_Evict,  SC_L2_Evict,  E,     { M },          `SCQ_L1_L3,   MI_Bsy,        Data_ReqWrBack,  BusVictim,  RspAck,  EC_Victim_ClrQW );
      add_tc_req_act(ET_LINK_REQ_Evict,  SC_L2_Evict,  E,     { A1_M_Bsy },   `SCQ_L1_L3,   A1_MI_Bsy,     Data_ReqWrBack,  BusVictim,  RspAck,  EC_Victim_ClrQW );
      add_tc_req_act(ET_LINK_REQ_Evict,  SC_L2_Evict,  E,     { L },          `SCQ_L1_L3,   St_Ignore,     Data_None,       BusIdle,    RspAck,  EC_None         );
      add_tc_req_act(ET_LINK_REQ_Evict,  SC_L2_Evict,  E,     { D },          `SCQ_L1_L3,   St_Ignore,     Data_None,       BusIdle,    RspAck,  EC_None         );
      add_tc_req_act(ET_LINK_REQ_Evict,  SC_L2_Evict,  E,     { A1_D_Bsy },   `SCQ_L1_L3,   St_Ignore,     Data_None,       BusIdle,    RspAck,  EC_None         );
      add_tc_req_act(ET_LINK_REQ_Evict,  SC_L2_Evict,  E,     { W },          `SCQ_L1_L3,   WI_Bsy,        Data_ReqWrBack,  BusVictim,  RspAck,  EC_Victim_ClrQW );
      add_tc_req_act(ET_LINK_REQ_Evict,  SC_L2_Evict,  E,     { A1_W_Bsy },   `SCQ_L1_L3,   A1_WI_Bsy,     Data_ReqWrBack,  BusVictim,  RspAck,  EC_Victim_ClrQW, m_no_cover );

      //Evict to L3 but we're not sending anything to L3 (remote_l3_enable=0)
      add_tc_req_act(ET_LINK_REQ_Evict,  SC_L2_Evict,  I,     { I },          (`SCQ_L1_L3 | `SCQ_DownDis),   St_Ignore,     Data_None,       BusIdle,    RspAck,  EC_None  );
      add_tc_req_act(ET_LINK_REQ_Evict,  SC_L2_Evict,  I,     { A1_I_Bsy },   (`SCQ_L1_L3 | `SCQ_DownDis),   St_Ignore,     Data_None,       BusIdle,    RspAck,  EC_None  );
      add_tc_req_act(ET_LINK_REQ_Evict,  SC_L2_Evict,  I,     { A2_I_Bsy },   (`SCQ_L1_L3 | `SCQ_DownDis),   St_Ignore,     Data_None,       BusIdle,    RspAck,  EC_None, m_no_cover );
      add_tc_req_act(ET_LINK_REQ_Evict,  SC_L2_Evict,  E,     { E },          (`SCQ_L1_L3 | `SCQ_DownDis),   I,             Data_None,       BusIdle,    RspAck,  EC_Victim  );
      add_tc_req_act(ET_LINK_REQ_Evict,  SC_L2_Evict,  E,     { A1_E_Bsy },   (`SCQ_L1_L3 | `SCQ_DownDis),   A1_I_Bsy,      Data_None,       BusIdle,    RspAck,  EC_Victim, m_no_cover );
      add_tc_req_act(ET_LINK_REQ_Evict,  SC_L2_Evict,  E,     { M },          (`SCQ_L1_L3 | `SCQ_DownDis),   MI_Bsy,        Data_ReqWrBack,  BusVictim,  RspAck,  EC_Victim_ClrQW );
      add_tc_req_act(ET_LINK_REQ_Evict,  SC_L2_Evict,  E,     { A1_M_Bsy },   (`SCQ_L1_L3 | `SCQ_DownDis),   A1_MI_Bsy,     Data_ReqWrBack,  BusVictim,  RspAck,  EC_Victim_ClrQW );
      add_tc_req_act(ET_LINK_REQ_Evict,  SC_L2_Evict,  E,     { L },          (`SCQ_L1_L3 | `SCQ_DownDis),   St_Ignore,     Data_None,       BusIdle,    RspAck,  EC_None  );
      add_tc_req_act(ET_LINK_REQ_Evict,  SC_L2_Evict,  E,     { D },          (`SCQ_L1_L3 | `SCQ_DownDis),   St_Ignore,     Data_None,       BusIdle,    RspAck,  EC_None  );
      add_tc_req_act(ET_LINK_REQ_Evict,  SC_L2_Evict,  E,     { A1_D_Bsy },   (`SCQ_L1_L3 | `SCQ_DownDis),   St_Ignore,     Data_None,       BusIdle,    RspAck,  EC_None  );
      add_tc_req_act(ET_LINK_REQ_Evict,  SC_L2_Evict,  E,     { W },          (`SCQ_L1_L3 | `SCQ_DownDis),   WI_Bsy,        Data_ReqWrBack,  BusVictim,  RspAck,  EC_Victim_ClrQW );
      add_tc_req_act(ET_LINK_REQ_Evict,  SC_L2_Evict,  E,     { A1_W_Bsy },   (`SCQ_L1_L3 | `SCQ_DownDis),   A1_WI_Bsy,     Data_ReqWrBack,  BusVictim,  RspAck,  EC_Victim_ClrQW, m_no_cover );

      add_tc_req_act(ET_LINK_REQ_Evict,  SC_L2_Evict,  I,     { I },          `SCQ_L2_L3,   St_Ignore,     Data_None,       BusIdle,    RspAck,  EC_None         );
      add_tc_req_act(ET_LINK_REQ_Evict,  SC_L2_Evict,  I,     { A1_I_Bsy },   `SCQ_L2_L3,   St_Ignore,     Data_None,       BusIdle,    RspAck,  EC_None         );
      add_tc_req_act(ET_LINK_REQ_Evict,  SC_L2_Evict,  I,     { A2_I_Bsy },   `SCQ_L2_L3,   St_Ignore,     Data_None,       BusIdle,    RspAck,  EC_None,        m_no_cover );
      add_tc_req_act(ET_LINK_REQ_Evict,  SC_L2_Evict,  E,     { E },          `SCQ_L2_L3,   I,             Data_None,       BusIdle,    RspAck,  EC_Victim       );
      add_tc_req_act(ET_LINK_REQ_Evict,  SC_L2_Evict,  E,     { A1_E_Bsy },   `SCQ_L2_L3,   A1_I_Bsy,      Data_None,       BusIdle,    RspAck,  EC_Victim, m_no_cover );
      add_tc_req_act(ET_LINK_REQ_Evict,  SC_L2_Evict,  E,     { M },          `SCQ_L2_L3,   MI_Bsy,        Data_ReqWrBack,  BusVictim,  RspAck,  EC_Victim_ClrQW );
      add_tc_req_act(ET_LINK_REQ_Evict,  SC_L2_Evict,  E,     { A1_M_Bsy },   `SCQ_L2_L3,   A1_MI_Bsy,     Data_ReqWrBack,  BusVictim,  RspAck,  EC_Victim_ClrQW );
      add_tc_req_act(ET_LINK_REQ_Evict,  SC_L2_Evict,  E,     { L },          `SCQ_L2_L3,   St_Ignore,     Data_None,       BusIdle,    RspAck,  EC_None         );
      add_tc_req_act(ET_LINK_REQ_Evict,  SC_L2_Evict,  E,     { D },          `SCQ_L2_L3,   St_Ignore,     Data_None,       BusIdle,    RspAck,  EC_None         );
      add_tc_req_act(ET_LINK_REQ_Evict,  SC_L2_Evict,  E,     { A1_D_Bsy },   `SCQ_L2_L3,   St_Ignore,     Data_None,       BusIdle,    RspAck,  EC_None         );
      add_tc_req_act(ET_LINK_REQ_Evict,  SC_L2_Evict,  E,     { W },          `SCQ_L2_L3,   WI_Bsy,        Data_ReqWrBack,  BusVictim,  RspAck,  EC_Victim_ClrQW );
      add_tc_req_act(ET_LINK_REQ_Evict,  SC_L2_Evict,  E,     { A1_W_Bsy },   `SCQ_L2_L3,   A1_WI_Bsy,     Data_ReqWrBack,  BusVictim,  RspAck,  EC_Victim_ClrQW, m_no_cover );

      //Evict to L3 but remote_l3_enable=0
      // For this case dirty data must still be sent to memory
      add_tc_req_act(ET_LINK_REQ_Evict,  SC_L2_Evict,  I,     { I },          (`SCQ_L2_L3 | `SCQ_DownDis),   St_Ignore,     Data_None,       BusIdle,    RspAck,  EC_None  );
      add_tc_req_act(ET_LINK_REQ_Evict,  SC_L2_Evict,  I,     { A1_I_Bsy },   (`SCQ_L2_L3 | `SCQ_DownDis),   St_Ignore,     Data_None,       BusIdle,    RspAck,  EC_None  );
      add_tc_req_act(ET_LINK_REQ_Evict,  SC_L2_Evict,  I,     { A2_I_Bsy },   (`SCQ_L2_L3 | `SCQ_DownDis),   St_Ignore,     Data_None,       BusIdle,    RspAck,  EC_None,       m_no_cover );
      add_tc_req_act(ET_LINK_REQ_Evict,  SC_L2_Evict,  E,     { E },          (`SCQ_L2_L3 | `SCQ_DownDis),   I,             Data_None,       BusIdle,    RspAck,  EC_Victim );
      add_tc_req_act(ET_LINK_REQ_Evict,  SC_L2_Evict,  E,     { A1_E_Bsy },   (`SCQ_L2_L3 | `SCQ_DownDis),   A1_I_Bsy,      Data_None,       BusIdle,    RspAck,  EC_Victim, m_no_cover );
      add_tc_req_act(ET_LINK_REQ_Evict,  SC_L2_Evict,  E,     { M },          (`SCQ_L2_L3 | `SCQ_DownDis),   MI_Bsy,        Data_ReqWrBack,  BusVictim,  RspAck,  EC_Victim_ClrQW  );
      add_tc_req_act(ET_LINK_REQ_Evict,  SC_L2_Evict,  E,     { A1_M_Bsy },   (`SCQ_L2_L3 | `SCQ_DownDis),   A1_MI_Bsy,     Data_ReqWrBack,  BusVictim,  RspAck,  EC_Victim_ClrQW  );
      add_tc_req_act(ET_LINK_REQ_Evict,  SC_L2_Evict,  E,     { L },          (`SCQ_L2_L3 | `SCQ_DownDis),   St_Ignore,     Data_None,       BusIdle,    RspAck,  EC_None  );
      add_tc_req_act(ET_LINK_REQ_Evict,  SC_L2_Evict,  E,     { D },          (`SCQ_L2_L3 | `SCQ_DownDis),   St_Ignore,     Data_None,       BusIdle,    RspAck,  EC_None  );
      add_tc_req_act(ET_LINK_REQ_Evict,  SC_L2_Evict,  E,     { A1_D_Bsy },   (`SCQ_L2_L3 | `SCQ_DownDis),   St_Ignore,     Data_None,       BusIdle,    RspAck,  EC_None  );
      add_tc_req_act(ET_LINK_REQ_Evict,  SC_L2_Evict,  E,     { W },          (`SCQ_L2_L3 | `SCQ_DownDis),   WI_Bsy,        Data_ReqWrBack,  BusVictim,  RspAck,  EC_Victim_ClrQW  );
      add_tc_req_act(ET_LINK_REQ_Evict,  SC_L2_Evict,  E,     { A1_W_Bsy },   (`SCQ_L2_L3 | `SCQ_DownDis),   A1_WI_Bsy,     Data_ReqWrBack,  BusVictim,  RspAck,  EC_Victim_ClrQW, m_no_cover );

      add_tc_req_act(ET_LINK_REQ_Evict,  SC_L2_Evict,  I,     { I },          `SCQ_L1_Mem,  A1_I_Bsy,         Data_ReqWrNull,  BusEvict,   RspAck,  EC_None         );
      add_tc_req_act(ET_LINK_REQ_Evict,  SC_L2_Evict,  I,     { A1_I_Bsy },   `SCQ_L1_Mem,  A2_I_Bsy,      Data_ReqWrNull,  BusEvict,   RspAck,  EC_None         );
      add_tc_req_act(ET_LINK_REQ_Evict,  SC_L2_Evict,  I,     { A2_I_Bsy },   `SCQ_L1_Mem,  A3_I_Bsy,      Data_ReqWrNull,  BusEvict,   RspAck,  EC_None         , m_no_cover );
      add_tc_req_act(ET_LINK_REQ_Evict,  SC_L2_Evict,  E,     { E },          `SCQ_L1_Mem,  EI_Bsy,        Data_ReqWrNull,  BusEvict,   RspAck,  EC_Victim_ClrQW );
      add_tc_req_act(ET_LINK_REQ_Evict,  SC_L2_Evict,  E,     { A1_E_Bsy },   `SCQ_L1_Mem,  A1_EI_Bsy,     Data_ReqWrNull,  BusEvict,   RspAck,  EC_Victim_ClrQW, m_no_cover );
      add_tc_req_act(ET_LINK_REQ_Evict,  SC_L2_Evict,  E,     { M },          `SCQ_L1_Mem,  MI_MemBsy,     Data_ReqWrBack,  BusVictim,  RspAck,  EC_Victim_ClrQW );
      add_tc_req_act(ET_LINK_REQ_Evict,  SC_L2_Evict,  E,     { A1_M_Bsy },   `SCQ_L1_Mem,  A1_MI_MemBsy,  Data_ReqWrBack,  BusVictim,  RspAck,  EC_Victim_ClrQW );
      add_tc_req_act(ET_LINK_REQ_Evict,  SC_L2_Evict,  E,     { L },          `SCQ_L1_Mem,  A1_L_Bsy,      Data_ReqWrNull,  BusEvict,   RspAck,  EC_None         );
      add_tc_req_act(ET_LINK_REQ_Evict,  SC_L2_Evict,  E,     { A1_L_Bsy },   `SCQ_L1_Mem,  A2_L_Bsy,      Data_ReqWrNull,  BusEvict,   RspAck,  EC_None, m_no_cover );
      add_tc_req_act(ET_LINK_REQ_Evict,  SC_L2_Evict,  E,     { D },          `SCQ_L1_Mem,  A1_D_Bsy,      Data_ReqWrNull,  BusEvict,   RspAck,  EC_None         );
      add_tc_req_act(ET_LINK_REQ_Evict,  SC_L2_Evict,  E,     { A1_D_Bsy },   `SCQ_L1_Mem,  A2_D_Bsy,      Data_ReqWrNull,  BusEvict,   RspAck,  EC_None         );
      add_tc_req_act(ET_LINK_REQ_Evict,  SC_L2_Evict,  E,     { W },          `SCQ_L1_Mem,  WI_MemBsy,     Data_ReqWrBack,  BusVictim,  RspAck,  EC_Victim_ClrQW );
      add_tc_req_act(ET_LINK_REQ_Evict,  SC_L2_Evict,  E,     { A1_W_Bsy },   `SCQ_L1_Mem,  A1_WI_MemBsy,  Data_ReqWrBack,  BusVictim,  RspAck,  EC_Victim_ClrQW, m_no_cover );

      //Evict to Mem but we're not sending anything to L3 (remote_l3_enable=0)
      add_tc_req_act(ET_LINK_REQ_Evict,  SC_L2_Evict,  I,     { I },          (`SCQ_L1_Mem | `SCQ_DownDis),  St_Ignore,     Data_None,       BusIdle,    RspAck,  EC_None  );
      add_tc_req_act(ET_LINK_REQ_Evict,  SC_L2_Evict,  I,     { A1_I_Bsy },   (`SCQ_L1_Mem | `SCQ_DownDis),  St_Ignore,     Data_None,       BusIdle,    RspAck,  EC_None  );
      add_tc_req_act(ET_LINK_REQ_Evict,  SC_L2_Evict,  I,     { A2_I_Bsy },   (`SCQ_L1_Mem | `SCQ_DownDis),  St_Ignore,     Data_None,       BusIdle,    RspAck,  EC_None,  m_no_cover );
      add_tc_req_act(ET_LINK_REQ_Evict,  SC_L2_Evict,  E,     { E },          (`SCQ_L1_Mem | `SCQ_DownDis),  I,             Data_None,       BusIdle,    RspAck,  EC_Victim  );
      add_tc_req_act(ET_LINK_REQ_Evict,  SC_L2_Evict,  E,     { A1_E_Bsy },   (`SCQ_L1_Mem | `SCQ_DownDis),  A1_I_Bsy,      Data_None,       BusIdle,    RspAck,  EC_Victim, m_no_cover );
      add_tc_req_act(ET_LINK_REQ_Evict,  SC_L2_Evict,  E,     { M },          (`SCQ_L1_Mem | `SCQ_DownDis),  MI_MemBsy,     Data_ReqWrBack,  BusVictim,  RspAck,  EC_Victim_ClrQW );
      add_tc_req_act(ET_LINK_REQ_Evict,  SC_L2_Evict,  E,     { A1_M_Bsy },   (`SCQ_L1_Mem | `SCQ_DownDis),  A1_MI_MemBsy,  Data_ReqWrBack,  BusVictim,  RspAck,  EC_Victim_ClrQW );
      add_tc_req_act(ET_LINK_REQ_Evict,  SC_L2_Evict,  E,     { L },          (`SCQ_L1_Mem | `SCQ_DownDis),  St_Ignore,     Data_None,       BusIdle,    RspAck,  EC_None  );
      add_tc_req_act(ET_LINK_REQ_Evict,  SC_L2_Evict,  E,     { D },          (`SCQ_L1_Mem | `SCQ_DownDis),  St_Ignore,     Data_None,       BusIdle,    RspAck,  EC_None  );
      add_tc_req_act(ET_LINK_REQ_Evict,  SC_L2_Evict,  E,     { A1_D_Bsy },   (`SCQ_L1_Mem | `SCQ_DownDis),  St_Ignore,     Data_None,       BusIdle,    RspAck,  EC_None  );
      add_tc_req_act(ET_LINK_REQ_Evict,  SC_L2_Evict,  E,     { W },          (`SCQ_L1_Mem | `SCQ_DownDis),  WI_MemBsy,     Data_ReqWrBack,  BusVictim,  RspAck,  EC_Victim_ClrQW );
      add_tc_req_act(ET_LINK_REQ_Evict,  SC_L2_Evict,  E,     { A1_W_Bsy },   (`SCQ_L1_Mem | `SCQ_DownDis),  A1_WI_MemBsy,  Data_ReqWrBack,  BusVictim,  RspAck,  EC_Victim_ClrQW, m_no_cover );

      add_tc_req_act(ET_LINK_REQ_Evict,  SC_L2_Evict,  I,     { I },          `SCQ_L2_Mem,  A1_I_Bsy,      Data_ReqWrNull,  BusEvict,   RspAck,  EC_None         );
      add_tc_req_act(ET_LINK_REQ_Evict,  SC_L2_Evict,  I,     { A1_I_Bsy },   `SCQ_L2_Mem,  A2_I_Bsy,      Data_ReqWrNull,  BusEvict,   RspAck,  EC_None         );
      add_tc_req_act(ET_LINK_REQ_Evict,  SC_L2_Evict,  I,     { A2_I_Bsy },   `SCQ_L2_Mem,  A3_I_Bsy,      Data_ReqWrNull,  BusEvict,   RspAck,  EC_None         , m_no_cover );
      add_tc_req_act(ET_LINK_REQ_Evict,  SC_L2_Evict,  E,     { E },          `SCQ_L2_Mem,  EI_Bsy,        Data_ReqWrNull,  BusEvict,   RspAck,  EC_Victim_ClrQW );
      add_tc_req_act(ET_LINK_REQ_Evict,  SC_L2_Evict,  E,     { A1_E_Bsy },   `SCQ_L2_Mem,  A1_EI_Bsy,     Data_ReqWrNull,  BusEvict,   RspAck,  EC_Victim_ClrQW, m_no_cover );
      add_tc_req_act(ET_LINK_REQ_Evict,  SC_L2_Evict,  E,     { M },          `SCQ_L2_Mem,  MI_MemBsy,     Data_ReqWrBack,  BusVictim,  RspAck,  EC_Victim_ClrQW );
      add_tc_req_act(ET_LINK_REQ_Evict,  SC_L2_Evict,  E,     { A1_M_Bsy },   `SCQ_L2_Mem,  A1_MI_MemBsy,  Data_ReqWrBack,  BusVictim,  RspAck,  EC_Victim_ClrQW );
      add_tc_req_act(ET_LINK_REQ_Evict,  SC_L2_Evict,  E,     { L },          `SCQ_L2_Mem,  A1_L_Bsy,      Data_ReqWrNull,  BusEvict,   RspAck,  EC_None         );
      //add_tc_req_act(ET_LINK_REQ_Evict,  SC_L2_Evict,  E,     { A1_L_Bsy },   `SCQ_L2_Mem,  A2_L_Bsy,      Data_ReqWrNull,  BusEvict,   RspAck,  EC_None         );
      add_tc_req_act(ET_LINK_REQ_Evict,  SC_L2_Evict,  E,     { D },          `SCQ_L2_Mem,  A1_D_Bsy,      Data_ReqWrNull,  BusEvict,   RspAck,  EC_None         );
      add_tc_req_act(ET_LINK_REQ_Evict,  SC_L2_Evict,  E,     { A1_D_Bsy },   `SCQ_L2_Mem,  A2_D_Bsy,      Data_ReqWrNull,  BusEvict,   RspAck,  EC_None         );
      add_tc_req_act(ET_LINK_REQ_Evict,  SC_L2_Evict,  E,     { W },          `SCQ_L2_Mem,  WI_MemBsy,     Data_ReqWrBack,  BusVictim,  RspAck,  EC_Victim_ClrQW );
      add_tc_req_act(ET_LINK_REQ_Evict,  SC_L2_Evict,  E,     { A1_W_Bsy },   `SCQ_L2_Mem,  A1_WI_MemBsy,     Data_ReqWrBack,  BusVictim,  RspAck,  EC_Victim_ClrQW, m_no_cover );

      //Evict to Mem but we're not sending anything to L3 (remote_l3_enable=0)
      add_tc_req_act(ET_LINK_REQ_Evict,  SC_L2_Evict,  I,     { I },          (`SCQ_L2_Mem | `SCQ_DownDis),  St_Ignore,     Data_None,       BusIdle,    RspAck,  EC_None  );
      add_tc_req_act(ET_LINK_REQ_Evict,  SC_L2_Evict,  I,     { A1_I_Bsy },   (`SCQ_L2_Mem | `SCQ_DownDis),  St_Ignore,     Data_None,       BusIdle,    RspAck,  EC_None  );
      add_tc_req_act(ET_LINK_REQ_Evict,  SC_L2_Evict,  I,     { A2_I_Bsy },   (`SCQ_L2_Mem | `SCQ_DownDis),  St_Ignore,     Data_None,       BusIdle,    RspAck,  EC_None  , m_no_cover);
      add_tc_req_act(ET_LINK_REQ_Evict,  SC_L2_Evict,  E,     { E },          (`SCQ_L2_Mem | `SCQ_DownDis),  I,             Data_None,       BusIdle,    RspAck,  EC_Victim  );
      add_tc_req_act(ET_LINK_REQ_Evict,  SC_L2_Evict,  E,     { A1_E_Bsy },   (`SCQ_L2_Mem | `SCQ_DownDis),  A1_I_Bsy,      Data_None,       BusIdle,    RspAck,  EC_Victim, m_no_cover );
      add_tc_req_act(ET_LINK_REQ_Evict,  SC_L2_Evict,  E,     { M },          (`SCQ_L2_Mem | `SCQ_DownDis),  MI_MemBsy,     Data_ReqWrBack,  BusVictim,  RspAck,  EC_Victim_ClrQW );
      add_tc_req_act(ET_LINK_REQ_Evict,  SC_L2_Evict,  E,     { A1_M_Bsy },   (`SCQ_L2_Mem | `SCQ_DownDis),  A1_MI_MemBsy,  Data_ReqWrBack,  BusVictim,  RspAck,  EC_Victim_ClrQW );
      add_tc_req_act(ET_LINK_REQ_Evict,  SC_L2_Evict,  E,     { L },          (`SCQ_L2_Mem | `SCQ_DownDis),  St_Ignore,     Data_None,       BusIdle,    RspAck,  EC_None  );
      add_tc_req_act(ET_LINK_REQ_Evict,  SC_L2_Evict,  E,     { D },          (`SCQ_L2_Mem | `SCQ_DownDis),  St_Ignore,     Data_None,       BusIdle,    RspAck,  EC_None  );
      add_tc_req_act(ET_LINK_REQ_Evict,  SC_L2_Evict,  E,     { A1_D_Bsy },   (`SCQ_L2_Mem | `SCQ_DownDis),  St_Ignore,     Data_None,       BusIdle,    RspAck,  EC_None  );
      add_tc_req_act(ET_LINK_REQ_Evict,  SC_L2_Evict,  E,     { W },          (`SCQ_L2_Mem | `SCQ_DownDis),  WI_MemBsy,     Data_ReqWrBack,  BusVictim,  RspAck,  EC_Victim_ClrQW );
      add_tc_req_act(ET_LINK_REQ_Evict,  SC_L2_Evict,  E,     { A1_W_Bsy },   (`SCQ_L2_Mem | `SCQ_DownDis),  A1_WI_MemBsy,  Data_ReqWrBack,  BusVictim,  RspAck,  EC_Victim_ClrQW, m_no_cover );

      // Illegal source/dest combinations end up here (only one in coverage... it's hit in a directed test)
      add_tc_req_act(ET_LINK_REQ_Evict,  SC_L2_Evict,  I,     { I },          `SCQ_None,    St_Ignore,     Data_None,       BusIdle,    RspAck,  EC_None         , m_no_cover);
      add_tc_req_act(ET_LINK_REQ_Evict,  SC_L2_Evict,  I,     { A1_I_Bsy },   `SCQ_None,    St_Ignore,     Data_None,       BusIdle,    RspAck,  EC_None         , m_no_cover);
      add_tc_req_act(ET_LINK_REQ_Evict,  SC_L2_Evict,  I,     { A2_I_Bsy },   `SCQ_None,    St_Ignore,     Data_None,       BusIdle,    RspAck,  EC_None         , m_no_cover);
      add_tc_req_act(ET_LINK_REQ_Evict,  SC_L2_Evict,  E,     { E },          `SCQ_None,    I,             Data_None,       BusIdle,    RspAck,  EC_Victim       , m_no_cover);
      add_tc_req_act(ET_LINK_REQ_Evict,  SC_L2_Evict,  E,     { A1_E_Bsy },   `SCQ_None,    A1_I_Bsy,      Data_None,       BusIdle,    RspAck,  EC_Victim       , m_no_cover);
      add_tc_req_act(ET_LINK_REQ_Evict,  SC_L2_Evict,  E,     { M },          `SCQ_None,    MI_Bsy,        Data_ReqWrBack,  BusVictim,  RspAck,  EC_Victim_ClrQW );
      add_tc_req_act(ET_LINK_REQ_Evict,  SC_L2_Evict,  E,     { A1_M_Bsy },   `SCQ_None,    A1_MI_Bsy,     Data_ReqWrBack,  BusVictim,  RspAck,  EC_Victim_ClrQW , m_no_cover);
      add_tc_req_act(ET_LINK_REQ_Evict,  SC_L2_Evict,  E,     { L },          `SCQ_None,    St_Ignore,     Data_None,       BusIdle,    RspAck,  EC_None         , m_no_cover);
      add_tc_req_act(ET_LINK_REQ_Evict,  SC_L2_Evict,  E,     { D },          `SCQ_None,    St_Ignore,     Data_None,       BusIdle,    RspAck,  EC_None         , m_no_cover);
      add_tc_req_act(ET_LINK_REQ_Evict,  SC_L2_Evict,  E,     { A1_D_Bsy },   `SCQ_None,    St_Ignore,     Data_None,       BusIdle,    RspAck,  EC_None         , m_no_cover);
      add_tc_req_act(ET_LINK_REQ_Evict,  SC_L2_Evict,  E,     { W },          `SCQ_None,    WI_Bsy,        Data_ReqWrBack,  BusVictim,  RspAck,  EC_Victim_ClrQW , m_no_cover);
      add_tc_req_act(ET_LINK_REQ_Evict,  SC_L2_Evict,  E,     { A1_W_Bsy },   `SCQ_None,    A1_WI_Bsy,     Data_ReqWrBack,  BusVictim,  RspAck,  EC_Victim_ClrQW , m_no_cover);

      //but we're not sending anything to L3 (remote_l3_enable=0)
      add_tc_req_act(ET_LINK_REQ_Evict,  SC_L2_Evict,  I,     { I },          (`SCQ_None | `SCQ_DownDis),    St_Ignore,     Data_None,       BusIdle,    RspAck,  EC_None         , m_no_cover);
      add_tc_req_act(ET_LINK_REQ_Evict,  SC_L2_Evict,  I,     { A1_I_Bsy },   (`SCQ_None | `SCQ_DownDis),    St_Ignore,     Data_None,       BusIdle,    RspAck,  EC_None         , m_no_cover);
      add_tc_req_act(ET_LINK_REQ_Evict,  SC_L2_Evict,  I,     { A2_I_Bsy },   (`SCQ_None | `SCQ_DownDis),    St_Ignore,     Data_None,       BusIdle,    RspAck,  EC_None         , m_no_cover);
      add_tc_req_act(ET_LINK_REQ_Evict,  SC_L2_Evict,  E,     { E },          (`SCQ_None | `SCQ_DownDis),    I,             Data_None,       BusIdle,    RspAck,  EC_Victim       , m_no_cover);
      add_tc_req_act(ET_LINK_REQ_Evict,  SC_L2_Evict,  E,     { A1_E_Bsy },   (`SCQ_None | `SCQ_DownDis),    A1_I_Bsy,      Data_None,       BusIdle,    RspAck,  EC_Victim       , m_no_cover);
      add_tc_req_act(ET_LINK_REQ_Evict,  SC_L2_Evict,  E,     { M },          (`SCQ_None | `SCQ_DownDis),    MI_Bsy,        Data_ReqWrBack,  BusVictim,  RspAck,  EC_Victim_ClrQW );
      add_tc_req_act(ET_LINK_REQ_Evict,  SC_L2_Evict,  E,     { A1_M_Bsy },   (`SCQ_None | `SCQ_DownDis),    A1_MI_Bsy,     Data_ReqWrBack,  BusVictim,  RspAck,  EC_Victim_ClrQW , m_no_cover);
      add_tc_req_act(ET_LINK_REQ_Evict,  SC_L2_Evict,  E,     { L },          (`SCQ_None | `SCQ_DownDis),    St_Ignore,     Data_None,       BusIdle,    RspAck,  EC_None         , m_no_cover);
      add_tc_req_act(ET_LINK_REQ_Evict,  SC_L2_Evict,  E,     { D },          (`SCQ_None | `SCQ_DownDis),    St_Ignore,     Data_None,       BusIdle,    RspAck,  EC_None         , m_no_cover);
      add_tc_req_act(ET_LINK_REQ_Evict,  SC_L2_Evict,  E,     { A1_D_Bsy },   (`SCQ_None | `SCQ_DownDis),    St_Ignore,     Data_None,       BusIdle,    RspAck,  EC_None         , m_no_cover);
      add_tc_req_act(ET_LINK_REQ_Evict,  SC_L2_Evict,  E,     { W },          (`SCQ_None | `SCQ_DownDis),    WI_Bsy,        Data_ReqWrBack,  BusVictim,  RspAck,  EC_Victim_ClrQW , m_no_cover);
      add_tc_req_act(ET_LINK_REQ_Evict,  SC_L2_Evict,  E,     { A1_W_Bsy },   (`SCQ_None | `SCQ_DownDis),    A1_WI_Bsy,     Data_ReqWrBack,  BusVictim,  RspAck,  EC_Victim_ClrQW , m_no_cover);


      // Note: The L3->Mem Evictions are a special case taken care of in the REQQ alloc portion of the table.

      // No L2:
      add_tc_req_act(ET_LINK_REQ_Evict,  SC_L2_Evict,  I,     { I }, (`SCQ_NoRegion | `SCQ_L1_L3),   St_Ignore,     Data_None,       BusIdle,    RspAck,  EC_MaybeErrRsp  );
      add_tc_req_act(ET_LINK_REQ_Evict,  SC_L2_Evict,  I,     { I }, (`SCQ_NoRegion | `SCQ_L2_L3),   St_Ignore,     Data_None,       BusIdle,    RspAck,  EC_MaybeErrRsp  );
      add_tc_req_act(ET_LINK_REQ_Evict,  SC_L2_Evict,  I,     { I }, (`SCQ_NoRegion | `SCQ_L1_Mem),  St_Ignore,     Data_None,       BusIdle,    RspAck,  EC_MaybeErrRsp  );
      add_tc_req_act(ET_LINK_REQ_Evict,  SC_L2_Evict,  I,     { I }, (`SCQ_NoRegion | `SCQ_L2_Mem),  St_Ignore,     Data_None,       BusIdle,    RspAck,  EC_MaybeErrRsp  );
      add_tc_req_act(ET_LINK_REQ_Evict,  SC_L2_Evict,  I,     { I }, (`SCQ_NoRegion),                St_Ignore,     Data_None,       BusIdle,    RspAck,  EC_MaybeErrRsp  );

      add_tc_req_act(ET_LINK_REQ_Evict,  SC_L2_Evict,  I,     { I }, (`SCQ_NoRegion |`SCQ_Remote | `SCQ_L2_L3),   St_Ignore,     Data_None,       BusIdle,    RspAck,  EC_MaybeErrRsp   );
      add_tc_req_act(ET_LINK_REQ_Evict,  SC_L2_Evict,  I,     { I }, (`SCQ_NoRegion |`SCQ_Remote | `SCQ_L2_L3 |`SCQ_DownDis),   St_Ignore,     Data_None,       BusIdle,    RspAck,  EC_MaybeErrRsp );

      add_tc_req_act(ET_LINK_REQ_Evict,  SC_L2_Evict,  I,     { I }, (`SCQ_NoRegion | `SCQ_DownDis | `SCQ_L1_L3),   St_Ignore,     Data_None,       BusIdle,    RspAck,  EC_MaybeErrRsp );
      add_tc_req_act(ET_LINK_REQ_Evict,  SC_L2_Evict,  I,     { I }, (`SCQ_NoRegion | `SCQ_DownDis | `SCQ_L2_L3),   St_Ignore,     Data_None,       BusIdle,    RspAck,  EC_MaybeErrRsp );
      add_tc_req_act(ET_LINK_REQ_Evict,  SC_L2_Evict,  I,     { I }, (`SCQ_NoRegion | `SCQ_DownDis | `SCQ_L1_Mem),  St_Ignore,     Data_None,       BusIdle,    RspAck,  EC_MaybeErrRsp );
      add_tc_req_act(ET_LINK_REQ_Evict,  SC_L2_Evict,  I,     { I }, (`SCQ_NoRegion | `SCQ_DownDis | `SCQ_L2_Mem),  St_Ignore,     Data_None,       BusIdle,    RspAck,  EC_MaybeErrRsp );
      add_tc_req_act(ET_LINK_REQ_Evict,  SC_L2_Evict,  I,     { I }, (`SCQ_NoRegion | `SCQ_DownDis),                St_Ignore,     Data_None,       BusIdle,    RspAck,  EC_MaybeErrRsp );

      // Evict to Remote Scratchpad
      add_tc_req_act(ET_LINK_REQ_Evict,  SC_L2_Evict,  I,     { I },         (`SCQ_Remote|`SCQ_L2_L3),   St_Ignore,     Data_None,       BusIdle,    RspAck,  EC_None         );
      add_tc_req_act(ET_LINK_REQ_Evict,  SC_L2_Evict,  I,     { A1_I_Bsy },  (`SCQ_Remote|`SCQ_L2_L3),   St_Ignore,     Data_None,       BusIdle,    RspAck,  EC_None         );
      add_tc_req_act(ET_LINK_REQ_Evict,  SC_L2_Evict,  E,     { W },         (`SCQ_Remote|`SCQ_L2_L3),   WI_Bsy,        Data_ReqWrBack,  BusVictim,  RspAck,  EC_Victim_ClrQW );

      add_tc_req_act(ET_LINK_REQ_Evict,  SC_L2_Evict,  I,     { I },         (`SCQ_Remote|`SCQ_DownDis|`SCQ_L2_L3),   St_Ignore,     Data_None,       BusIdle,    RspAck,  EC_None         );
      add_tc_req_act(ET_LINK_REQ_Evict,  SC_L2_Evict,  I,     { A1_I_Bsy },  (`SCQ_Remote|`SCQ_DownDis|`SCQ_L2_L3),   St_Ignore,     Data_None,       BusIdle,    RspAck,  EC_None         );
      add_tc_req_act(ET_LINK_REQ_Evict,  SC_L2_Evict,  E,     { W },         (`SCQ_Remote|`SCQ_DownDis|`SCQ_L2_L3),   WI_Bsy,        Data_ReqWrBack,  BusVictim,  RspAck,  EC_Victim_ClrQW );
      
      //
      // ET Link Cacheops (Lock)
      //                                              Obs   Model                                                       Bus
      //             ET-link Opcode     L2 Opcode    State  State          Qual        Tag Action  Data Action          Command   Rsp Type  Err Code
      //             -----------------  -----------  -----  -------------  ----------  ----------  -------------------  --------  --------  --------------
      add_tc_req_act(ET_LINK_REQ_Lock,  SC_L2_Lock,  I,     { I        },  `SCQ_None,  D,          Data_ZeroFillAlloc,  BusIdle,  RspAck,  EC_SetQW       );
      add_tc_req_act(ET_LINK_REQ_Lock,  SC_L2_Lock,  I,     { A1_I_Bsy },  `SCQ_None,  A1_D_Bsy,   Data_ZeroFillAlloc,  BusIdle,  RspAck,  EC_SetQW       );
      add_tc_req_act(ET_LINK_REQ_Lock,  SC_L2_Lock,  E,     { E        },  `SCQ_None,  D,          Data_ZeroFill,       BusIdle,  RspAck,  EC_SetQW       );
      add_tc_req_act(ET_LINK_REQ_Lock,  SC_L2_Lock,  E,     { L        },  `SCQ_None,  D,          Data_ZeroFill,       BusIdle,  RspAck,  EC_SetQW       );
      add_tc_req_act(ET_LINK_REQ_Lock,  SC_L2_Lock,  E,     { A1_L_Bsy },  `SCQ_None,  A1_D_Bsy,   Data_ZeroFill,       BusIdle,  RspAck,  EC_SetQW, m_no_cover );
      add_tc_req_act(ET_LINK_REQ_Lock,  SC_L2_Lock,  E,     { M        },  `SCQ_None,  D,          Data_ZeroFill,       BusIdle,  RspAck,  EC_SetQW       );
      add_tc_req_act(ET_LINK_REQ_Lock,  SC_L2_Lock,  E,     { A1_M_Bsy },  `SCQ_None,  A1_D_Bsy,   Data_ZeroFill,       BusIdle,  RspAck,  EC_SetQW       );
      add_tc_req_act(ET_LINK_REQ_Lock,  SC_L2_Lock,  E,     { D        },  `SCQ_None,  St_Ignore,  Data_ZeroFill,       BusIdle,  RspAck,  EC_SetQW       );
      add_tc_req_act(ET_LINK_REQ_Lock,  SC_L2_Lock,  E,     { A1_D_Bsy },  `SCQ_None,  St_Ignore,  Data_ZeroFill,       BusIdle,  RspAck,  EC_SetQW       );
      add_tc_req_act(ET_LINK_REQ_Lock,  SC_L2_Lock,  E,     { W        },  `SCQ_None,  WD_Bsy,     Data_ReqWrBack_ZeroFill, BusVictim,RspAck,  EC_Victim_SetQW );

      // You can't lock the last way:
      add_tc_req_act(ET_LINK_REQ_Lock,  SC_L2_Lock,  I,     { I        },  `SCQ_AllLocked,  St_Ignore,  Data_None,      BusIdle,  RspAck,  EC_MaybeErrRsp );
      add_tc_req_act(ET_LINK_REQ_Lock,  SC_L2_Lock,  I,     { A1_I_Bsy },  `SCQ_AllLocked,  St_Ignore,  Data_None,      BusIdle,  RspAck,  EC_MaybeErrRsp );
      add_tc_req_act(ET_LINK_REQ_Lock,  SC_L2_Lock,  I,     { A2_I_Bsy },  `SCQ_AllLocked,  St_Ignore,  Data_None,      BusIdle,  RspAck,  EC_MaybeErrRsp, m_no_cover ); 
      add_tc_req_act(ET_LINK_REQ_Lock,  SC_L2_Lock,  E,     { E        },  `SCQ_AllLocked,  St_Ignore,  Data_None,      BusIdle,  RspAck,  EC_MaybeErrRsp );
      add_tc_req_act(ET_LINK_REQ_Lock,  SC_L2_Lock,  E,     { M        },  `SCQ_AllLocked,  St_Ignore,  Data_None,      BusIdle,  RspAck,  EC_MaybeErrRsp );
      add_tc_req_act(ET_LINK_REQ_Lock,  SC_L2_Lock,  E,     { A1_M_Bsy },  `SCQ_AllLocked,  St_Ignore,  Data_None,      BusIdle,  RspAck,  EC_MaybeErrRsp );
      add_tc_req_act(ET_LINK_REQ_Lock,  SC_L2_Lock,  E,     { W        },  `SCQ_AllLocked,  St_Ignore,  Data_None,      BusIdle,  RspAck,  EC_MaybeErrRsp );

      // No L2:
      add_tc_req_act(ET_LINK_REQ_Lock,  SC_L2_Lock,  I,     { I        },  `SCQ_NoRegion,  St_Ignore,  Data_None,      BusIdle,  RspAck,  EC_MaybeErrRsp );
      
      //
      // ET Link Cacheops (Unlock)
      //                                                     Obs   Model                                              Bus
      //             ET-link Opcode       L2 Opcode         State  State         Qual        Tag Action  Data Action  Command   Rsp Type  Err Code
      //             -------------------  ----------------  -----  -----------   ----------  ----------  -----------  --------  --------  --------
      add_tc_req_act(ET_LINK_REQ_Unlock,  SC_L2_Unlock,     I,     { I },        `SCQ_None,  St_Ignore,  Data_None,   BusIdle,  RspAck,  EC_ClrQW );
      add_tc_req_act(ET_LINK_REQ_Unlock,  SC_L2_Unlock,     I,     { A1_I_Bsy }, `SCQ_None,  St_Ignore,  Data_None,   BusIdle,  RspAck,  EC_ClrQW );
      add_tc_req_act(ET_LINK_REQ_Unlock,  SC_L2_Unlock,     I,     { A2_I_Bsy }, `SCQ_None,  St_Ignore,  Data_None,   BusIdle,  RspAck,  EC_ClrQW, m_no_cover );
      add_tc_req_act(ET_LINK_REQ_Unlock,  SC_L2_Unlock,     E,     { E },        `SCQ_None,  St_Ignore,  Data_None,   BusIdle,  RspAck,  EC_ClrQW );
      add_tc_req_act(ET_LINK_REQ_Unlock,  SC_L2_Unlock,     E,     { A1_E_Bsy }, `SCQ_None,  St_Ignore,  Data_None,   BusIdle,  RspAck,  EC_ClrQW, m_no_cover );
      add_tc_req_act(ET_LINK_REQ_Unlock,  SC_L2_Unlock,     E,     { L },        `SCQ_None,  E,          Data_None,   BusIdle,  RspAck,  EC_ClrQW );
      add_tc_req_act(ET_LINK_REQ_Unlock,  SC_L2_Unlock,     E,     { M },        `SCQ_None,  St_Ignore,  Data_None,   BusIdle,  RspAck,  EC_None  );
      add_tc_req_act(ET_LINK_REQ_Unlock,  SC_L2_Unlock,     E,     { A1_M_Bsy }, `SCQ_None,  St_Ignore,  Data_None,   BusIdle,  RspAck,  EC_None  );
      add_tc_req_act(ET_LINK_REQ_Unlock,  SC_L2_Unlock,     E,     { D },        `SCQ_None,  M,          Data_None,   BusIdle,  RspAck,  EC_None  );
      add_tc_req_act(ET_LINK_REQ_Unlock,  SC_L2_Unlock,     E,     { A1_D_Bsy }, `SCQ_None,  A1_M_Bsy,   Data_None,   BusIdle,  RspAck,  EC_None  );
      add_tc_req_act(ET_LINK_REQ_Unlock,  SC_L2_Unlock,     E,     { W },        `SCQ_None,  St_Ignore,  Data_None,   BusIdle,  RspAck,  EC_None  );
      add_tc_req_act(ET_LINK_REQ_Unlock,  SC_L2_Unlock,     E,     { A1_W_Bsy }, `SCQ_None,  St_Ignore,  Data_None,   BusIdle,  RspAck,  EC_None, m_no_cover  );


      add_tc_req_act(ET_LINK_REQ_Unlock,  SC_L2_UnlockInv,  I,     { I },        `SCQ_None,  St_Ignore,  Data_None,   BusIdle,  RspAck,  EC_ClrQW );
      add_tc_req_act(ET_LINK_REQ_Unlock,  SC_L2_UnlockInv,  I,     { A1_I_Bsy }, `SCQ_None,  St_Ignore,  Data_None,   BusIdle,  RspAck,  EC_ClrQW );
      add_tc_req_act(ET_LINK_REQ_Unlock,  SC_L2_UnlockInv,  I,     { A2_I_Bsy }, `SCQ_None,  St_Ignore,  Data_None,   BusIdle,  RspAck,  EC_ClrQW, m_no_cover );
      add_tc_req_act(ET_LINK_REQ_Unlock,  SC_L2_UnlockInv,  E,     { E },        `SCQ_None,  St_Ignore,  Data_None,   BusIdle,  RspAck,  EC_ClrQW );
      add_tc_req_act(ET_LINK_REQ_Unlock,  SC_L2_UnlockInv,  E,     { A1_E_Bsy }, `SCQ_None,  St_Ignore,  Data_None,   BusIdle,  RspAck,  EC_ClrQW, m_no_cover );
      add_tc_req_act(ET_LINK_REQ_Unlock,  SC_L2_UnlockInv,  E,     { L },        `SCQ_None,  I,          Data_None,   BusIdle,  RspAck,  EC_ClrQW );
      add_tc_req_act(ET_LINK_REQ_Unlock,  SC_L2_UnlockInv,  E,     { M },        `SCQ_None,  St_Ignore,  Data_None,   BusIdle,  RspAck,  EC_None  );
      add_tc_req_act(ET_LINK_REQ_Unlock,  SC_L2_UnlockInv,  E,     { A1_M_Bsy }, `SCQ_None,  St_Ignore,  Data_None,   BusIdle,  RspAck,  EC_None  );
      add_tc_req_act(ET_LINK_REQ_Unlock,  SC_L2_UnlockInv,  E,     { D },        `SCQ_None,  I,          Data_None,   BusIdle,  RspAck,  EC_ClrQW );
      add_tc_req_act(ET_LINK_REQ_Unlock,  SC_L2_UnlockInv,  E,     { A1_D_Bsy }, `SCQ_None,  A1_I_Bsy,   Data_None,   BusIdle,  RspAck,  EC_ClrQW );
      add_tc_req_act(ET_LINK_REQ_Unlock,  SC_L2_UnlockInv,  E,     { W },        `SCQ_None,  St_Ignore,  Data_None,   BusIdle,  RspAck,  EC_None  );
      add_tc_req_act(ET_LINK_REQ_Unlock,  SC_L2_UnlockInv,  E,     { A1_W_Bsy }, `SCQ_None,  St_Ignore,  Data_None,   BusIdle,  RspAck,  EC_None, m_no_cover  );



      // No L2:
      add_tc_req_act(ET_LINK_REQ_Unlock,  SC_L2_Unlock,     I,     { I       },  `SCQ_NoRegion,  St_Ignore,  Data_None,      BusIdle,  RspAck,  EC_MaybeErrRsp );
      add_tc_req_act(ET_LINK_REQ_Unlock,  SC_L2_UnlockInv,  I,     { I       },  `SCQ_NoRegion,  St_Ignore,  Data_None,      BusIdle,  RspAck,  EC_MaybeErrRsp );
      
      //
      // ET Link Cacheops (Prefetch)
      //                                                      Obs   Model                                                    Bus
      //             ET-link Opcode         L2 Opcode        State  State        Qual           Tag Action  Data Action      Command     Rsp Type  Err Code
      //             ---------------------  ---------------  -----  -----------  -------------  ----------  ---------------  ----------  --------  ---------------
      add_tc_req_act(ET_LINK_REQ_Prefetch,  SC_L2_Prefetch,  I,     { I },       `SCQ_None,     IE_Bsy,     Data_None,       BusLineRd,    RspNull,  EC_ClrQW        );
      add_tc_req_act(ET_LINK_REQ_Prefetch,  SC_L2_Prefetch,  I,     { A1_I_Bsy },`SCQ_None,     A1_IE_Bsy,  Data_None,       BusLineRd,    RspNull,  EC_ClrQW        );
      add_tc_req_act(ET_LINK_REQ_Prefetch,  SC_L2_Prefetch,  I,     { A2_I_Bsy },`SCQ_None,     A2_IE_Bsy,  Data_None,       BusLineRd,    RspNull,  EC_ClrQW, m_no_cover );
      add_tc_req_act(ET_LINK_REQ_Prefetch,  SC_L2_Prefetch,  E,     { E },       `SCQ_None,     St_Ignore,  Data_None,       BusIdle,    RspAck,   EC_ClrQW        );
      add_tc_req_act(ET_LINK_REQ_Prefetch,  SC_L2_Prefetch,  E,     { A1_E_Bsy },`SCQ_None,     St_Ignore,  Data_None,       BusIdle,    RspAck,   EC_ClrQW, m_no_cover );
      add_tc_req_act(ET_LINK_REQ_Prefetch,  SC_L2_Prefetch,  E,     { L },       `SCQ_None,     St_Ignore,  Data_None,       BusIdle,    RspAck,   EC_ClrQW        );
      add_tc_req_act(ET_LINK_REQ_Prefetch,  SC_L2_Prefetch,  E,     { M },       `SCQ_None,     St_Ignore,  Data_None,       BusIdle,    RspAck,   EC_None         );
      add_tc_req_act(ET_LINK_REQ_Prefetch,  SC_L2_Prefetch,  E,     { A1_M_Bsy },`SCQ_None,     St_Ignore,  Data_None,       BusIdle,    RspAck,   EC_None         );
      add_tc_req_act(ET_LINK_REQ_Prefetch,  SC_L2_Prefetch,  E,     { D },       `SCQ_None,     St_Ignore,  Data_None,       BusIdle,    RspAck,   EC_None         );
      add_tc_req_act(ET_LINK_REQ_Prefetch,  SC_L2_Prefetch,  E,     { A1_D_Bsy },`SCQ_None,     St_Ignore,  Data_None,       BusIdle,    RspAck,   EC_None         );
      add_tc_req_act(ET_LINK_REQ_Prefetch,  SC_L2_Prefetch,  E,     { W },       `SCQ_Partial,  WIE_Bsy,   Data_ReqWrBack,  BusVictim,  RspAck,   EC_Victim_ClrQW );

      add_tc_req_act(ET_LINK_REQ_Prefetch,  SC_L2_Fill,      I,     { IE_Bsy },  `SCQ_None,     E,          Data_CacheFill,  BusIdle,    RspNull,  EC_ClrQW        );

      // No L2:
      add_tc_req_act(ET_LINK_REQ_Prefetch,  SC_L2_Prefetch,  I,     { I },       `SCQ_NoRegion, St_Ignore,  Data_None,       BusIdle,    RspAck,   EC_MaybeErrRsp  );

      
      //
      // Index Cacheops
      //                                                      Obs   Model                                                    Bus
      //             ET-link Opcode         L2 Opcode        State  State        Qual           Tag Action  Data Action      Command     Rsp Type  Err Code
      //             ---------------------  ---------------  -----  -----------  -------------  ----------  ---------------  ----------  --------  ---------------
      add_tc_req_act(ET_LINK_REQ_Write,  SC_Idx_L2_Evict, I,    { I },      `SCQ_None,        I,           Data_None,       BusIdle,         RspAck,    EC_None        );
      add_tc_req_act(ET_LINK_REQ_Write,  SC_Idx_L2_Evict, E,    { E },      `SCQ_None,        I,           Data_None,       BusIdle,         RspAck,    EC_Victim_ClrQW );
      add_tc_req_act(ET_LINK_REQ_Write,  SC_Idx_L2_Evict, E,    { L },      `SCQ_None,        I,           Data_None,       BusIdle,         RspAck,    EC_Victim_ClrQW );
      add_tc_req_act(ET_LINK_REQ_Write,  SC_Idx_L2_Evict, E,    { M },      `SCQ_None,        MI_Bsy,      Data_ReqWrBack,  BusVictim,       RspAck,    EC_Victim_ClrQW );
      add_tc_req_act(ET_LINK_REQ_Write,  SC_Idx_L2_Evict, E,    { W },      `SCQ_None,        WI_Bsy,      Data_ReqWrBack,  BusVictim,       RspAck,    EC_Victim_ClrQW );
      add_tc_req_act(ET_LINK_REQ_Write,  SC_Idx_L2_Evict, E,    { D },      `SCQ_None,        DI_Bsy,      Data_ReqWrBack,  BusVictim,       RspAck,    EC_Victim_ClrQW );

      add_tc_req_act(ET_LINK_REQ_Write,  SC_Idx_L2_Flush, I,    { I },      `SCQ_None,        St_Ignore,   Data_None,       BusIdle,         RspAck,    EC_None        );
      add_tc_req_act(ET_LINK_REQ_Write,  SC_Idx_L2_Flush, E,    { E },      `SCQ_None,        St_Ignore,   Data_None,       BusIdle,         RspAck,    EC_None        );
      add_tc_req_act(ET_LINK_REQ_Write,  SC_Idx_L2_Flush, E,    { M },      `SCQ_None,        ME_Bsy,      Data_ReqWrBack,  BusVictim,       RspAck,    EC_Victim_ClrQW );
      add_tc_req_act(ET_LINK_REQ_Write,  SC_Idx_L2_Flush, E,    { W },      `SCQ_None,        WI_Bsy,      Data_ReqWrBack,  BusVictim,       RspAck,    EC_Victim_ClrQW );
      add_tc_req_act(ET_LINK_REQ_Write,  SC_Idx_L2_Flush, E,    { D },      `SCQ_None,        DL_Bsy,      Data_ReqWrBack,  BusVictim,       RspAck,    EC_Victim_ClrQW );
      add_tc_req_act(ET_LINK_REQ_Write,  SC_Idx_L2_Flush, E,    { L },      `SCQ_None,        St_Ignore,   Data_None,       BusIdle,        RspAck,     EC_None       );

      add_tc_req_act(ET_LINK_REQ_Write,  SC_Idx_L2_Inv,   I,    { I },      `SCQ_None,        I,           Data_None,       BusIdle,         RspAck,    EC_None        );
      add_tc_req_act(ET_LINK_REQ_Write,  SC_Idx_L2_Inv,   E,    { E },      `SCQ_None,        I,           Data_None,       BusIdle,         RspAck,    EC_None        );
      add_tc_req_act(ET_LINK_REQ_Write,  SC_Idx_L2_Inv,   E,    { M },      `SCQ_None,        I,           Data_None,       BusIdle,         RspAck,    EC_None        );
      add_tc_req_act(ET_LINK_REQ_Write,  SC_Idx_L2_Inv,   E,    { W },      `SCQ_None,        I,           Data_None,       BusIdle,         RspAck,    EC_DeqWA       );
      add_tc_req_act(ET_LINK_REQ_Write,  SC_Idx_L2_Inv,   E,    { D },      `SCQ_None,        I,           Data_None,       BusIdle,         RspAck,    EC_None        );
      add_tc_req_act(ET_LINK_REQ_Write,  SC_Idx_L2_Inv,   E,    { L },      `SCQ_None,        I,           Data_None,       BusIdle,         RspAck,    EC_None        );

      add_tc_req_act(ET_LINK_REQ_Write,  SC_Idx_All_Inv,  I,    { I },      `SCQ_None,        I,           Data_None,       BusIdle,         RspAck,    EC_None        );

      //A CB_Inv cacheop from the cacheop state machine sends a non-index Evict
      add_tc_req_act(ET_LINK_REQ_Write,  SC_L2_Evict,     E,    { W },      `SCQ_None,        WI_Bsy,      Data_ReqWrBack,  BusVictim,       RspAck,    EC_Victim_ClrQW );
      //The state may have changed by the time the Evict from the CB_Inv gets to the pipe
      add_tc_req_act(ET_LINK_REQ_Write,  SC_L2_Evict,     I,    { I },      `SCQ_None,        I,           Data_None,       BusIdle,         RspAck,    EC_None );
      add_tc_req_act(ET_LINK_REQ_Write,  SC_L2_Evict,     I,    { A1_I_Bsy },`SCQ_None,       A1_I_Bsy,    Data_None,       BusIdle,         RspAck,    EC_None , m_no_cover );
      add_tc_req_act(ET_LINK_REQ_Write,  SC_L2_Evict,     E,    { M },      `SCQ_None,        MI_Bsy,      Data_ReqWrBack,  BusVictim,       RspAck,    EC_Victim_ClrQW );
      add_tc_req_act(ET_LINK_REQ_Write,  SC_L2_Evict,     E,    { A1_M_Bsy },`SCQ_None,       A1_MI_Bsy,   Data_ReqWrBack,  BusVictim,       RspAck,    EC_Victim_ClrQW, m_no_cover );
      add_tc_req_act(ET_LINK_REQ_Write,  SC_L2_Evict,     E,    { E },      `SCQ_None,        I,           Data_None,       BusIdle,         RspAck,    EC_Victim_ClrQW, m_no_cover );
      add_tc_req_act(ET_LINK_REQ_Write,  SC_L2_Evict,     E,    { L },      `SCQ_None,        St_Ignore,   Data_None,       BusIdle,         RspAck,    EC_None , m_no_cover );
      add_tc_req_act(ET_LINK_REQ_Write,  SC_L2_Evict,     E,    { D },      `SCQ_None,        St_Ignore,   Data_None,       BusIdle,         RspAck,    EC_None , m_no_cover );
      
      // index cachops are sent even in Bypass
      add_tc_req_act(ET_LINK_REQ_Write,  SC_Idx_All_Inv,  I,    { I },      `SCQ_Bypass,      I,           Data_None,       BusIdle,         RspAck,    EC_None        );
      add_tc_req_act(ET_LINK_REQ_Write,  SC_Idx_L2_Flush, I,    { I },      `SCQ_Bypass,      I,           Data_None,       BusIdle,         RspAck,    EC_None        );
      add_tc_req_act(ET_LINK_REQ_Write,  SC_Idx_L2_Evict, I,    { I },      `SCQ_Bypass,      I,           Data_None,       BusIdle,         RspAck,    EC_None        );
      add_tc_req_act(ET_LINK_REQ_Write,  SC_Idx_L2_Inv,   I,    { I },      `SCQ_Bypass,      I,           Data_None,       BusIdle,         RspAck,    EC_None        );
      // because of how Idx_All_Invs are sent to all scoreboards:
      add_tc_req_act(ET_LINK_REQ_Write,  SC_Idx_All_Inv,  I,    { I },      `SCQ_NoRegion,    I,           Data_None,       BusIdle,         RspAck,    EC_MaybeErrRsp );

      // This is some bad luck where we're running a full chip test and we put the L2 into bypass while read requests are being sent to memory, and the
      // L2 happens to get thrown into bypass as a) a read request is outstanding, and b) another read request has gotten to reqq.
      //add_tc_req_act(ET_LINK_REQ_Read, SC_L2_Read, I, {IE_Bsy}, `SCQ_Bypass, I, Data_None, BusIdle, 

      //============================================================================================
      //============================================================================================
      //
      // Lookups Performed at the TC Tag Response Stage
      //
      //============================================================================================
      //============================================================================================

      //
      // Capacity Evictions
      //                                                             Obs   Model                                                     Bus
      //              ET-link Opcode            L2 Opcode           State  State          Qual          Tag Action  Data Action      Command    Rsp Type  Err Code
      //              ------------------------  ------------------  -----  -------------  ------------  ----------  ---------------  ---------  --------  --------
      add_tag_rsp_act(ET_LINK_REQ_Read,         SC_L2_Fill,         M,     { M },         `SCQ_Victim,  A1_I_Bsy,   Data_ReqWrBack,  BusWrite,  RspNull,  EC_ClrQW );
      add_tag_rsp_act(ET_LINK_REQ_Read,         SC_L2_Fill,         M,     { A1_M_Bsy },  `SCQ_Victim,  A2_I_Bsy,   Data_ReqWrBack,  BusWrite,  RspNull,  EC_ClrQW );
      add_tag_rsp_act(ET_LINK_REQ_Read,         SC_L2_Fill,         M,     { A2_M_Bsy },  `SCQ_Victim,  A3_I_Bsy,   Data_ReqWrBack,  BusWrite,  RspNull,  EC_ClrQW, m_no_cover );
      add_tag_rsp_act(ET_LINK_REQ_Read,         SC_L2_Fill,         M,     { W },         `SCQ_Victim,  A1_I_Bsy,   Data_ReqWrBack,  BusWrite,  RspNull,  EC_ClrQW );
      add_tag_rsp_act(ET_LINK_REQ_Read,         SC_L2_Fill,         M,     { A1_W_Bsy },  `SCQ_Victim,  A2_I_Bsy,   Data_ReqWrBack,  BusWrite,  RspNull,  EC_ClrQW );
      add_tag_rsp_act(ET_LINK_REQ_Read,         SC_L2_Fill,         M,     { A2_W_Bsy },  `SCQ_Victim,  A3_I_Bsy,   Data_ReqWrBack,  BusWrite,  RspNull,  EC_ClrQW, m_no_cover );

      // CLC I believe that at some point the RTL changed and these just all show up as Read now:
      //add_tag_rsp_act(ET_LINK_REQ_ReadCoop,     SC_L2_Fill,         M,     { M },         `SCQ_Victim,  A1_I_Bsy,     Data_ReqWrBack,  BusWrite,  RspNull,  EC_ClrQW );
      //add_tag_rsp_act(ET_LINK_REQ_ReadCoop,     SC_L2_Fill,         M,     { A1_M_Bsy },  `SCQ_Victim,  A2_I_Bsy,     Data_ReqWrBack,  BusWrite,  RspNull,  EC_ClrQW );
      //add_tag_rsp_act(ET_LINK_REQ_ReadCoop,     SC_L2_Fill,         M,     { A2_M_Bsy },  `SCQ_Victim,  A3_I_Bsy,     Data_ReqWrBack,  BusWrite,  RspNull,  EC_ClrQW );
      //add_tag_rsp_act(ET_LINK_REQ_ReadCoop,     SC_L2_Fill,         M,     { W },         `SCQ_Victim,  A1_I_Bsy,     Data_ReqWrBack,  BusWrite,  RspNull,  EC_ClrQW );
      //add_tag_rsp_act(ET_LINK_REQ_ReadCoop,     SC_L2_Fill,         M,     { A1_W_Bsy },  `SCQ_Victim,  A2_I_Bsy,     Data_ReqWrBack,  BusWrite,  RspNull,  EC_ClrQW );
      //add_tag_rsp_act(ET_LINK_REQ_ReadCoop,     SC_L2_Fill,         M,     { A2_W_Bsy },  `SCQ_Victim,  A3_I_Bsy,     Data_ReqWrBack,  BusWrite,  RspNull,  EC_ClrQW );
      //add_tag_rsp_act(ET_LINK_REQ_Atomic,       SC_L2_Fill,         M,     { M },         `SCQ_Victim,  A1_I_Bsy,     Data_ReqWrBack,  BusWrite,  RspNull,  EC_ClrQW );
      //add_tag_rsp_act(ET_LINK_REQ_Atomic,       SC_L2_Fill,         M,     { A1_M_Bsy },  `SCQ_Victim,  A2_I_Bsy,     Data_ReqWrBack,  BusWrite,  RspNull,  EC_ClrQW );
      //add_tag_rsp_act(ET_LINK_REQ_Atomic,       SC_L2_Fill,         M,     { A2_M_Bsy },  `SCQ_Victim,  A3_I_Bsy,     Data_ReqWrBack,  BusWrite,  RspNull,  EC_ClrQW );
      //add_tag_rsp_act(ET_LINK_REQ_Atomic,       SC_L2_Fill,         M,     { W },         `SCQ_Victim,  A1_I_Bsy,     Data_ReqWrBack,  BusWrite,  RspNull,  EC_ClrQW );
      //add_tag_rsp_act(ET_LINK_REQ_Atomic,       SC_L2_Fill,         M,     { A1_W_Bsy },  `SCQ_Victim,  A2_I_Bsy,     Data_ReqWrBack,  BusWrite,  RspNull,  EC_ClrQW );
      //add_tag_rsp_act(ET_LINK_REQ_Atomic,       SC_L2_Fill,         M,     { A2_W_Bsy },  `SCQ_Victim,  A3_I_Bsy,     Data_ReqWrBack,  BusWrite,  RspNull,  EC_ClrQW );


      add_tag_rsp_act(ET_LINK_REQ_Write,        SC_L2_Write,        M,     { M },         `SCQ_Victim,  A1_I_Bsy,     Data_ReqWrBack,  BusWrite,  RspNull,  EC_ClrQW );
      add_tag_rsp_act(ET_LINK_REQ_Write,        SC_L2_Write,        M,     { A1_M_Bsy },  `SCQ_Victim,  A2_I_Bsy,     Data_ReqWrBack,  BusWrite,  RspNull,  EC_ClrQW );
      add_tag_rsp_act(ET_LINK_REQ_Write,        SC_L2_Write,        M,     { A2_M_Bsy },  `SCQ_Victim,  A3_I_Bsy,     Data_ReqWrBack,  BusWrite,  RspNull,  EC_ClrQW );
      add_tag_rsp_act(ET_LINK_REQ_Write,        SC_L2_Write,        M,     { W },         `SCQ_Victim,  A1_I_Bsy,     Data_ReqWrBack,  BusWrite,  RspNull,  EC_ClrQW );
      add_tag_rsp_act(ET_LINK_REQ_Write,        SC_L2_Write,        M,     { A1_W_Bsy },  `SCQ_Victim,  A2_I_Bsy,     Data_ReqWrBack,  BusWrite,  RspNull,  EC_ClrQW );
      add_tag_rsp_act(ET_LINK_REQ_Write,        SC_L2_Write,        M,     { A2_W_Bsy },  `SCQ_Victim,  A3_I_Bsy,     Data_ReqWrBack,  BusWrite,  RspNull,  EC_ClrQW );

      // Write hit a partial, so we have a victim and a write at the same time.
      add_tag_rsp_act(ET_LINK_REQ_Write,        SC_L2_Write,        M,     { WM_Bsy },    `SCQ_Victim,  A1_M_Bsy,  Data_ReqWrBack,  BusWrite,  RspNull,  EC_None );

      // WritePartial hit some WriteAround data, so we have to write back, send out a read, then write the writePartial data.
      add_tag_rsp_act(ET_LINK_REQ_Write,        SC_L2_WritePartial, M,     { WEM_Bsy },   `SCQ_Victim,  A1_IEM_Bsy, Data_ReqWrBack, BusWrite,  RspNull,  EC_LateRead );


      add_tag_rsp_act(ET_LINK_REQ_WriteAround,  SC_L2_WriteAround,  M,     { M },         `SCQ_Victim,  A1_I_Bsy,   Data_ReqWrBack,  BusWrite,  RspNull,  EC_ClrQW );
      add_tag_rsp_act(ET_LINK_REQ_WriteAround,  SC_L2_WriteAround,  M,     { A1_M_Bsy },  `SCQ_Victim,  A2_I_Bsy,   Data_ReqWrBack,  BusWrite,  RspNull,  EC_ClrQW );
      add_tag_rsp_act(ET_LINK_REQ_WriteAround,  SC_L2_WriteAround,  M,     { A2_M_Bsy },  `SCQ_Victim,  A3_I_Bsy,   Data_ReqWrBack,  BusWrite,  RspNull,  EC_ClrQW, m_no_cover );
      add_tag_rsp_act(ET_LINK_REQ_WriteAround,  SC_L2_WriteAround,  M,     { W },         `SCQ_Victim,  A1_I_Bsy,   Data_ReqWrBack,  BusWrite,  RspNull,  EC_ClrQW );
      add_tag_rsp_act(ET_LINK_REQ_WriteAround,  SC_L2_WriteAround,  M,     { A1_W_Bsy },  `SCQ_Victim,  A2_I_Bsy,   Data_ReqWrBack,  BusWrite,  RspNull,  EC_ClrQW );
      add_tag_rsp_act(ET_LINK_REQ_WriteAround,  SC_L2_WriteAround,  M,     { A2_W_Bsy },  `SCQ_Victim,  A3_I_Bsy,   Data_ReqWrBack,  BusWrite,  RspNull,  EC_ClrQW, m_no_cover );

      add_tag_rsp_act(ET_LINK_REQ_Lock,         SC_L2_Lock,         M,     { M },         `SCQ_Victim,  A1_I_Bsy,     Data_ReqWrBack,  BusWrite,  RspNull,  EC_ClrQW );
      add_tag_rsp_act(ET_LINK_REQ_Lock,         SC_L2_Lock,         M,     { A1_M_Bsy },  `SCQ_Victim,  A2_I_Bsy,     Data_ReqWrBack,  BusWrite,  RspNull,  EC_ClrQW );
      add_tag_rsp_act(ET_LINK_REQ_Lock,         SC_L2_Lock,         M,     { A2_M_Bsy },  `SCQ_Victim,  A3_I_Bsy,     Data_ReqWrBack,  BusWrite,  RspNull,  EC_ClrQW, m_no_cover );
      add_tag_rsp_act(ET_LINK_REQ_Lock,         SC_L2_Lock,         M,     { W },         `SCQ_Victim,  A1_I_Bsy,     Data_ReqWrBack,  BusWrite,  RspNull,  EC_ClrQW );
      add_tag_rsp_act(ET_LINK_REQ_Lock,         SC_L2_Lock,         M,     { A1_W_Bsy },  `SCQ_Victim,  A2_I_Bsy,     Data_ReqWrBack,  BusWrite,  RspNull,  EC_ClrQW );
      add_tag_rsp_act(ET_LINK_REQ_Lock,         SC_L2_Lock,         M,     { A2_W_Bsy },  `SCQ_Victim,  A3_I_Bsy,     Data_ReqWrBack,  BusWrite,  RspNull,  EC_ClrQW, m_no_cover );

      add_tag_rsp_act(ET_LINK_REQ_Lock,         SC_L2_Lock,         M,     { WD_Bsy },    `SCQ_Victim,  A1_D_Bsy,    Data_ReqWrBack,  BusWrite,  RspNull,  EC_None  );
  
      
      //
      // Forced Partial Eviction due to a partial read/atomic/prefetch
      //                                                       Obs   Model                                                           Bus
      //              ET-link Opcode         L2 Opcode        State  State             Qual          Tag Action     Data Action      Command    Rsp Type  Err Code
      //              ---------------------  ---------------  -----  ----------------  ------------  -------------  ---------------  ---------  --------  -----------
      add_tag_rsp_act(ET_LINK_REQ_Read,      SC_L2_Read,      M,     { WIE_Bsy },      `SCQ_Victim,  A1_IE_Bsy,     Data_ReqWrBack,  BusWrite,  RspNull,  EC_LateRead );
      add_tag_rsp_act(ET_LINK_REQ_Read,      SC_L2_Read,      M,     { A1_WIE_Bsy },   `SCQ_Victim,  A2_IE_Bsy,     Data_ReqWrBack,  BusWrite,  RspNull,  EC_LateRead, m_no_cover );
      add_tag_rsp_act(ET_LINK_REQ_ReadCoop,  SC_L2_Read,      M,     { WIE_Bsy },      `SCQ_Victim,  A1_IE_Bsy,     Data_ReqWrBack,  BusWrite,  RspNull,  EC_LateRead );
      add_tag_rsp_act(ET_LINK_REQ_ReadCoop,  SC_L2_Read,      M,     { A1_WIE_Bsy },   `SCQ_Victim,  A2_IE_Bsy,     Data_ReqWrBack,  BusWrite,  RspNull,  EC_LateRead, m_no_cover );
      add_tag_rsp_act(ET_LINK_REQ_Atomic,    SC_L2_Atomic,    M,     { WIE_Bsy },      `SCQ_Victim,  A1_IE_Bsy,     Data_ReqWrBack,  BusWrite,  RspNull,  EC_LateRead );
      add_tag_rsp_act(ET_LINK_REQ_Atomic,    SC_L2_Atomic,    M,     { A1_WIE_Bsy },   `SCQ_Victim,  A2_IE_Bsy,     Data_ReqWrBack,  BusWrite,  RspNull,  EC_LateRead, m_no_cover );
      add_tag_rsp_act(ET_LINK_REQ_Prefetch,  SC_L2_Prefetch,  M,     { WIE_Bsy },      `SCQ_Victim,  A1_IE_Bsy,     Data_ReqWrBack,  BusWrite,  RspNull,  EC_LateRead );
      add_tag_rsp_act(ET_LINK_REQ_Prefetch,  SC_L2_Prefetch,  M,     { A1_WIE_Bsy },   `SCQ_Victim,  A2_IE_Bsy,     Data_ReqWrBack,  BusWrite,  RspNull,  EC_LateRead, m_no_cover );
      
      //
      // Coalescing Buffer Evictions
      //                                                       Obs   Model                                                     Bus
      //              ET-link Opcode            L2 Opcode     State  State          Qual          Tag Action  Data Action      Command    Rsp Type  Err Code
      //              ------------------------  ------------  -----  -------------  ------------  ----------  ---------------  ---------  --------  --------

      add_tag_rsp_act(ET_LINK_REQ_WriteAround,  SC_L2_Evict,  M,    { WI_Bsy },     `SCQ_Victim,  A1_I_Bsy,   Data_ReqWrBack,  BusWrite,  RspNull,  EC_ClrQW );
      add_tag_rsp_act(ET_LINK_REQ_WriteAround,  SC_L2_Evict,  M,    { MI_Bsy },     `SCQ_Victim,  A1_I_Bsy,   Data_ReqWrBack,  BusWrite,  RspNull,  EC_ClrQW );
      add_tag_rsp_act(ET_LINK_REQ_WriteAround,  SC_L2_Evict,  M,    { A1_WI_Bsy },  `SCQ_Victim,  A2_I_Bsy,   Data_ReqWrBack,  BusWrite,  RspNull,  EC_ClrQW );
      add_tag_rsp_act(ET_LINK_REQ_WriteAround,  SC_L2_Evict,  M,    { A1_MI_Bsy },  `SCQ_Victim,  A2_I_Bsy,   Data_ReqWrBack,  BusWrite,  RspNull,  EC_ClrQW );


      //
      // Forced Eviction due to a filled write-around
      //                                                             Obs   Model                                                         Bus
      //              ET-link Opcode            L2 Opcode           State  State              Qual          Tag Action  Data Action      Command    Rsp Type  Err Code
      //              ------------------------  ------------------  -----  -----------------  ------------  ----------  ---------------  ---------  --------  --------
      add_tag_rsp_act(ET_LINK_REQ_WriteAround,  SC_L2_WriteAround,  M,     { MI_Bsy },        `SCQ_Victim,  A1_I_Bsy,   Data_ReqWrBack,  BusWrite,  RspNull,  EC_ClrQW );
      add_tag_rsp_act(ET_LINK_REQ_WriteAround,  SC_L2_WriteAround,  M,     { WI_Bsy },        `SCQ_Victim,  A1_I_Bsy,   Data_ReqWrBack,  BusWrite,  RspNull,  EC_ClrQW );
      add_tag_rsp_act(ET_LINK_REQ_WriteAround,  SC_L2_WriteAround,  M,     { A1_WI_Bsy },     `SCQ_Victim,  A2_I_Bsy,   Data_ReqWrBack,  BusWrite,  RspNull,  EC_ClrQW );
      add_tag_rsp_act(ET_LINK_REQ_WriteAround,  SC_L2_WriteAround,  M,     { A2_WI_Bsy },     `SCQ_Victim,  A3_I_Bsy,   Data_ReqWrBack,  BusWrite,  RspNull,  EC_ClrQW, m_no_cover );

      
      //
      // A cacheop creates an eviction
      //                                                 Obs   Model                                                         Bus
      //              ET-link Opcode      L2 Opcode     State  State              Qual          Tag Action  Data Action      Command         Rsp Type   Err Code
      //              ------------------  ------------  -----  -----------------  ------------  ----------  ---------------  --------------  ---------  --------
      add_tag_rsp_act(ET_LINK_REQ_Flush,  SC_L2_Flush,  M,     { ME_Bsy },        `SCQ_Victim,  A1_E_Bsy,   Data_ReqWrBack,  BusWrite,       RspNull,   EC_None  );
      add_tag_rsp_act(ET_LINK_REQ_Flush,  SC_L2_Flush,  M,     { A1_ME_Bsy },     `SCQ_Victim,  A2_E_Bsy,   Data_ReqWrBack,  BusWrite,       RspNull,   EC_None  );
      add_tag_rsp_act(ET_LINK_REQ_Flush,  SC_L2_Flush,  M,     { WI_Bsy },        `SCQ_Victim,  A1_I_Bsy,   Data_ReqWrBack,  BusWrite,       RspNull,   EC_ClrQW );
      add_tag_rsp_act(ET_LINK_REQ_Flush,  SC_L2_Flush,  M,     { A1_WI_Bsy },     `SCQ_Victim,  A2_I_Bsy,   Data_ReqWrBack,  BusWrite,       RspNull,   EC_ClrQW, m_no_cover );
      add_tag_rsp_act(ET_LINK_REQ_Flush,  SC_L2_Flush,  M,     { ME_MemBsy },     `SCQ_Victim,  A1_E_Bsy,   Data_ReqWrBack,  BusFlushToMem,  RspNull,   EC_None  );
      //add_tag_rsp_act(ET_LINK_REQ_Flush,  SC_L2_Flush,  M,     { MI_MemBsy },     `SCQ_Victim,  A1_I_Bsy,   Data_ReqWrBack,  BusFlushToMem,  RspNull,   EC_ClrQW );
      add_tag_rsp_act(ET_LINK_REQ_Flush,  SC_L2_Flush,  M,     { WI_MemBsy },     `SCQ_Victim,  A1_I_Bsy,   Data_ReqWrBack,  BusFlushToMem,  RspNull,   EC_ClrQW );
      //add_tag_rsp_act(ET_LINK_REQ_Flush,  SC_L2_Flush,  M,     { A1_MI_MemBsy },  `SCQ_Victim,  A2_I_Bsy,   Data_ReqWrBack,  BusFlushToMem,  RspNull,   EC_ClrQW, m_no_cover );
      add_tag_rsp_act(ET_LINK_REQ_Flush,  SC_L2_Flush,  M,     { A1_ME_MemBsy },  `SCQ_Victim,  A2_E_Bsy,   Data_ReqWrBack,  BusFlushToMem,  RspNull,   EC_ClrQW );
      //add_tag_rsp_act(ET_LINK_REQ_Flush,  SC_L2_Flush,  M,     { A1_WI_MemBsy },  `SCQ_Victim,  A2_I_Bsy,   Data_ReqWrBack,  BusFlushToMem,  RspNull,   EC_ClrQW, m_no_cover );

      add_tag_rsp_act(ET_LINK_REQ_Flush,  SC_L2_Flush,  M,     { ME_Bsy },        (`SCQ_Victim | `SCQ_DownDis),  A1_E_Bsy,   Data_ReqWrBack,  BusWrite,  RspNull,   EC_None  );
      add_tag_rsp_act(ET_LINK_REQ_Flush,  SC_L2_Flush,  M,     { A1_ME_Bsy },     (`SCQ_Victim | `SCQ_DownDis),  A2_E_Bsy,   Data_ReqWrBack,  BusWrite,  RspNull,   EC_None  );
      add_tag_rsp_act(ET_LINK_REQ_Flush,  SC_L2_Flush,  M,     { WI_Bsy },        (`SCQ_Victim | `SCQ_DownDis),  A1_I_Bsy,   Data_ReqWrBack,  BusWrite,  RspNull,   EC_ClrQW );
      add_tag_rsp_act(ET_LINK_REQ_Flush,  SC_L2_Flush,  M,     { A1_WI_Bsy },     (`SCQ_Victim | `SCQ_DownDis),  A2_I_Bsy,   Data_ReqWrBack,  BusWrite,  RspNull,   EC_ClrQW, m_no_cover );
      add_tag_rsp_act(ET_LINK_REQ_Flush,  SC_L2_Flush,  M,     { ME_MemBsy },     (`SCQ_Victim | `SCQ_DownDis),  A1_E_Bsy,   Data_ReqWrBack,  BusWrite,  RspNull,   EC_None  );
      //add_tag_rsp_act(ET_LINK_REQ_Flush,  SC_L2_Flush,  M,     { MI_MemBsy },     (`SCQ_Victim | `SCQ_DownDis),  A1_I_Bsy,   Data_ReqWrBack,  BusWrite,  RspNull,   EC_ClrQW );
      add_tag_rsp_act(ET_LINK_REQ_Flush,  SC_L2_Flush,  M,     { WI_MemBsy },     (`SCQ_Victim | `SCQ_DownDis),  A1_I_Bsy,   Data_ReqWrBack,  BusWrite,  RspNull,   EC_ClrQW );
      //add_tag_rsp_act(ET_LINK_REQ_Flush,  SC_L2_Flush,  M,     { A1_MI_MemBsy },  (`SCQ_Victim | `SCQ_DownDis),  A2_I_Bsy,   Data_ReqWrBack,  BusWrite,  RspNull,   EC_ClrQW, m_no_cover );
      add_tag_rsp_act(ET_LINK_REQ_Flush,  SC_L2_Flush,  M,     { A1_ME_MemBsy },  (`SCQ_Victim | `SCQ_DownDis),  A2_E_Bsy,   Data_ReqWrBack,  BusWrite,  RspNull,   EC_ClrQW );
      //add_tag_rsp_act(ET_LINK_REQ_Flush,  SC_L2_Flush,  M,     { A1_WI_MemBsy },  (`SCQ_Victim | `SCQ_DownDis),  A2_I_Bsy,   Data_ReqWrBack,  BusWrite,  RspNull,   EC_ClrQW, m_no_cover );

      add_tag_rsp_act(ET_LINK_REQ_Evict,  SC_L2_Evict,  M,     { MI_Bsy },        `SCQ_Victim,  A1_I_Bsy,   Data_ReqWrBack,  BusWrite,       RspNull,   EC_ClrQW );
      add_tag_rsp_act(ET_LINK_REQ_Evict,  SC_L2_Evict,  M,     { WI_Bsy },        `SCQ_Victim,  A1_I_Bsy,   Data_ReqWrBack,  BusWrite,       RspNull,   EC_ClrQW );
      add_tag_rsp_act(ET_LINK_REQ_Evict,  SC_L2_Evict,  M,     { A1_MI_Bsy },     `SCQ_Victim,  A2_I_Bsy,   Data_ReqWrBack,  BusWrite,       RspNull,   EC_ClrQW );
      add_tag_rsp_act(ET_LINK_REQ_Evict,  SC_L2_Evict,  M,     { A1_WI_Bsy },     `SCQ_Victim,  A2_I_Bsy,   Data_ReqWrBack,  BusWrite,       RspNull,   EC_ClrQW, m_no_cover );
      add_tag_rsp_act(ET_LINK_REQ_Evict,  SC_L2_Evict,  M,     { MI_MemBsy },     `SCQ_Victim,  A1_I_Bsy,   Data_ReqWrBack,  BusEvictToMem,  RspNull,   EC_ClrQW );
      add_tag_rsp_act(ET_LINK_REQ_Evict,  SC_L2_Evict,  M,     { WI_MemBsy },     `SCQ_Victim,  A1_I_Bsy,   Data_ReqWrBack,  BusEvictToMem,  RspNull,   EC_ClrQW );
      add_tag_rsp_act(ET_LINK_REQ_Evict,  SC_L2_Evict,  M,     { A1_MI_MemBsy },  `SCQ_Victim,  A2_I_Bsy,   Data_ReqWrBack,  BusEvictToMem,  RspNull,   EC_ClrQW );
      //add_tag_rsp_act(ET_LINK_REQ_Evict,  SC_L2_Evict,  M,     { A1_WI_MemBsy },  `SCQ_Victim,  A2_I_Bsy,   Data_ReqWrBack,  BusEvictToMem,  RspNull,   EC_ClrQW, m_no_cover );

      add_tag_rsp_act(ET_LINK_REQ_Evict,  SC_L2_Evict,  M,     { MI_Bsy },        (`SCQ_Victim | `SCQ_DownDis),  A1_I_Bsy,   Data_ReqWrBack,  BusWrite,  RspNull,   EC_ClrQW );
      add_tag_rsp_act(ET_LINK_REQ_Evict,  SC_L2_Evict,  M,     { WI_Bsy },        (`SCQ_Victim | `SCQ_DownDis),  A1_I_Bsy,   Data_ReqWrBack,  BusWrite,  RspNull,   EC_ClrQW );
      add_tag_rsp_act(ET_LINK_REQ_Evict,  SC_L2_Evict,  M,     { A1_MI_Bsy },     (`SCQ_Victim | `SCQ_DownDis),  A2_I_Bsy,   Data_ReqWrBack,  BusWrite,  RspNull,   EC_ClrQW );
      add_tag_rsp_act(ET_LINK_REQ_Evict,  SC_L2_Evict,  M,     { A1_WI_Bsy },     (`SCQ_Victim | `SCQ_DownDis),  A2_I_Bsy,   Data_ReqWrBack,  BusWrite,  RspNull,   EC_ClrQW, m_no_cover );
      add_tag_rsp_act(ET_LINK_REQ_Evict,  SC_L2_Evict,  M,     { MI_MemBsy },     (`SCQ_Victim | `SCQ_DownDis),  A1_I_Bsy,   Data_ReqWrBack,  BusWrite,  RspNull,   EC_ClrQW );
      add_tag_rsp_act(ET_LINK_REQ_Evict,  SC_L2_Evict,  M,     { WI_MemBsy },     (`SCQ_Victim | `SCQ_DownDis),  A1_I_Bsy,   Data_ReqWrBack,  BusWrite,  RspNull,   EC_ClrQW );
      add_tag_rsp_act(ET_LINK_REQ_Evict,  SC_L2_Evict,  M,     { A1_MI_MemBsy },  (`SCQ_Victim | `SCQ_DownDis),  A2_I_Bsy,   Data_ReqWrBack,  BusWrite,  RspNull,   EC_ClrQW );
      //add_tag_rsp_act(ET_LINK_REQ_Evict,  SC_L2_Evict,  M,     { A1_WI_MemBsy },  (`SCQ_Victim | `SCQ_DownDis),  A2_I_Bsy,   Data_ReqWrBack,  BusWrite,  RspNull,   EC_ClrQW, m_no_cover );

      //
      // Index Cache Ops
      //                                                    Obs     Model                                                       Bus
      //             ET-link Opcode       L2 Opcode         State   State       Qual              Tag Action   Data Action      Command          Rsp Type   Err Code
      //             -------------------  -------------     -----   ----------  ----------------  ----------   ---------------  ---------------  ---------  ------------
      add_tag_rsp_act(ET_LINK_REQ_Write,   SC_Idx_L2_Evict,  M,      {MI_Bsy},   `SCQ_Victim,       A1_I_Bsy,   Data_ReqWrBack,   BusWrite,       RspNull,   EC_None  );
      add_tag_rsp_act(ET_LINK_REQ_Write,   SC_Idx_L2_Evict,  M,      {DI_Bsy},   `SCQ_Victim,       A1_I_Bsy,   Data_ReqWrBack,   BusWrite,       RspNull,   EC_None  );
      add_tag_rsp_act(ET_LINK_REQ_Write,   SC_Idx_L2_Evict,  M,      {WI_Bsy},   `SCQ_Victim,       A1_I_Bsy,   Data_ReqWrBack,   BusWrite,       RspNull,   EC_None  );
      
      add_tag_rsp_act(ET_LINK_REQ_Write,   SC_Idx_L2_Flush,  M,      {ME_Bsy},   `SCQ_Victim,       A1_E_Bsy,   Data_ReqWrBack,   BusWrite,       RspNull,   EC_None  );
      add_tag_rsp_act(ET_LINK_REQ_Write,   SC_Idx_L2_Flush,  M,      {DL_Bsy},   `SCQ_Victim,       A1_L_Bsy,   Data_ReqWrBack,   BusWrite,       RspNull,   EC_None  );
      add_tag_rsp_act(ET_LINK_REQ_Write,   SC_Idx_L2_Flush,  M,      {WI_Bsy},   `SCQ_Victim,       A1_I_Bsy,   Data_ReqWrBack,   BusWrite,       RspNull,   EC_None  );

      //A CB_Inv cacheop from the cacheop state machine sends a non-index Evict.
      add_tag_rsp_act(ET_LINK_REQ_Write,   SC_L2_Evict,      M,      {WI_Bsy},   `SCQ_Victim,       A1_I_Bsy,   Data_ReqWrBack,   BusWrite,       RspNull,   EC_None  );
      add_tag_rsp_act(ET_LINK_REQ_Write,   SC_L2_Evict,      M,      {MI_Bsy},   `SCQ_Victim,       A1_I_Bsy,   Data_ReqWrBack,   BusWrite,       RspNull,   EC_None  );
      add_tag_rsp_act(ET_LINK_REQ_Write,   SC_L2_Evict,      M,      {A1_MI_Bsy},`SCQ_Victim,       A2_I_Bsy,   Data_ReqWrBack,   BusWrite,       RspNull,   EC_None  );
      //============================================================================================
      //============================================================================================
      //
      // Lookups Performed for AXI Bus Responses
      //
      //============================================================================================
      //============================================================================================

      //
      // Response to a Read Miss
      //                                     AXI            Model                                                                                   Bus
      //              ET-link Opcode         Command        State       AXI Response      Qual        Tag Action  Data Action            Rsp Type   Command   Err Code
      //              ---------------------  -------------  -------     ----------------  ----------  ----------  ---------------------  ---------  --------  --------
      add_bus_rsp_act(ET_LINK_REQ_Read,      AXI_REQ_READ,  IE_Bsy,     AXI_RESP_OKAY,    `SCQ_None,  St_Ignore,  Data_CascadeFill,      RspWData,  BusIdle,  EC_None  );
      add_bus_rsp_act(ET_LINK_REQ_Read,      AXI_REQ_READ,  A1_IE_Bsy,  AXI_RESP_OKAY,    `SCQ_None,  St_Ignore,  Data_CascadeFill,      RspWData,  BusIdle,  EC_None, m_no_cover );
      add_bus_rsp_act(ET_LINK_REQ_ReadCoop,  AXI_REQ_READ,  IE_Bsy,     AXI_RESP_OKAY,    `SCQ_None,  St_Ignore,  Data_CascadeFill,      RspWData,  BusIdle,  EC_None  );
      add_bus_rsp_act(ET_LINK_REQ_ReadCoop,  AXI_REQ_READ,  A1_IE_Bsy,  AXI_RESP_OKAY,    `SCQ_None,  St_Ignore,  Data_CascadeFill,      RspWData,  BusIdle,  EC_None, m_no_cover );
      add_bus_rsp_act(ET_LINK_REQ_Atomic,    AXI_REQ_READ,  IE_Bsy,     AXI_RESP_OKAY,    `SCQ_None,  St_Ignore,  Data_EnqueueFill,      RspNull,   BusIdle,  EC_None  );
      add_bus_rsp_act(ET_LINK_REQ_Atomic,    AXI_REQ_READ,  A1_IE_Bsy,  AXI_RESP_OKAY,    `SCQ_None,  St_Ignore,  Data_EnqueueFill,      RspNull,   BusIdle,  EC_None, m_no_cover );
      add_bus_rsp_act(ET_LINK_REQ_Read,      AXI_REQ_READ,  IE_Bsy,     AXI_RESP_DECERR,  `SCQ_None,  St_Ignore,  Data_CascadeFillZero,  RspErr,    BusIdle,  EC_None, m_no_cover );
      add_bus_rsp_act(ET_LINK_REQ_Read,      AXI_REQ_READ,  IE_Bsy,     AXI_RESP_SLVERR,  `SCQ_None,  St_Ignore,  Data_CascadeFillZero,  RspErr,    BusIdle,  EC_None, m_no_cover );
      add_bus_rsp_act(ET_LINK_REQ_Read,      AXI_REQ_READ,  A1_IE_Bsy,  AXI_RESP_DECERR,  `SCQ_None,  St_Ignore,  Data_CascadeFillZero,  RspErr,    BusIdle,  EC_None, m_no_cover );
      add_bus_rsp_act(ET_LINK_REQ_Read,      AXI_REQ_READ,  A1_IE_Bsy,  AXI_RESP_SLVERR,  `SCQ_None,  St_Ignore,  Data_CascadeFillZero,  RspErr,    BusIdle,  EC_None, m_no_cover );

      //
      // Response to a Capacity Eviction
      //                                   AXI             Model                                                                         Bus
      //              ET-link Opcode       Command         State          AXI Response    Qual        Tag Action  Data Action  Rsp Type  Command   Err Code
      //              -------------------  --------------  -------------  --------------  ----------  ----------  -----------  --------  --------  --------
      // Note CLC May2019: Once the pipe is dealing with a capacity eviction, the REQQ will not send another request to that line to the pipe. (though it
      // may have already sent one request). So I'm commenting out the ones that should not be possible
      add_bus_rsp_act(ET_LINK_REQ_Read,    AXI_REQ_WRITE,  A1_I_Bsy,      AXI_RESP_OKAY,  `SCQ_None,  I,          Data_None,   RspNull,  BusIdle,  EC_None  );
      add_bus_rsp_act(ET_LINK_REQ_Read,    AXI_REQ_WRITE,  A2_I_Bsy,      AXI_RESP_OKAY,  `SCQ_None,  A1_I_Bsy,   Data_None,   RspNull,  BusIdle,  EC_None  );
      //add_bus_rsp_act(ET_LINK_REQ_Read,    AXI_REQ_WRITE,  A3_I_Bsy,      AXI_RESP_OKAY,  `SCQ_None,  A2_I_Bsy,   Data_None,   RspNull,  BusIdle,  EC_None  );
      //add_bus_rsp_act(ET_LINK_REQ_Read,    AXI_REQ_WRITE,  A1_E_Bsy,      AXI_RESP_OKAY,  `SCQ_None,  E,          Data_None,   RspNull,  BusIdle,  EC_None  );
      //add_bus_rsp_act(ET_LINK_REQ_Read,    AXI_REQ_WRITE,  A2_E_Bsy,      AXI_RESP_OKAY,  `SCQ_None,  A1_E_Bsy,   Data_None,   RspNull,  BusIdle,  EC_None  );
      add_bus_rsp_act(ET_LINK_REQ_Read,    AXI_REQ_WRITE,  A1_M_Bsy,      AXI_RESP_OKAY,  `SCQ_None,  M,          Data_None,   RspNull,  BusIdle,  EC_None  );
      //add_bus_rsp_act(ET_LINK_REQ_Read,    AXI_REQ_WRITE,  A2_M_Bsy,      AXI_RESP_OKAY,  `SCQ_None,  A1_M_Bsy,   Data_None,   RspNull,  BusIdle,  EC_None  );
      add_bus_rsp_act(ET_LINK_REQ_Read,    AXI_REQ_WRITE,  A1_D_Bsy,      AXI_RESP_OKAY,  `SCQ_None,  D,          Data_None,   RspNull,  BusIdle,  EC_None  );
      //add_bus_rsp_act(ET_LINK_REQ_Read,    AXI_REQ_WRITE,  A2_D_Bsy,      AXI_RESP_OKAY,  `SCQ_None,  A1_D_Bsy,   Data_None,   RspNull,  BusIdle,  EC_None  );
      add_bus_rsp_act(ET_LINK_REQ_Read,    AXI_REQ_WRITE,  A1_W_Bsy,      AXI_RESP_OKAY,  `SCQ_None,  W,          Data_None,   RspNull,  BusIdle,  EC_None  );
      //add_bus_rsp_act(ET_LINK_REQ_Read,    AXI_REQ_WRITE,  A2_W_Bsy,      AXI_RESP_OKAY,  `SCQ_None,  A1_W_Bsy,   Data_None,   RspNull,  BusIdle,  EC_None  );
      add_bus_rsp_act(ET_LINK_REQ_Read,    AXI_REQ_WRITE,  A1_IE_Bsy,     AXI_RESP_OKAY,  `SCQ_None,  IE_Bsy,     Data_None,   RspNull,  BusIdle,  EC_None  );
      //add_bus_rsp_act(ET_LINK_REQ_Read,    AXI_REQ_WRITE,  A2_IE_Bsy,     AXI_RESP_OKAY,  `SCQ_None,  A1_IE_Bsy,  Data_None,   RspNull,  BusIdle,  EC_None  );
      add_bus_rsp_act(ET_LINK_REQ_Read,    AXI_REQ_WRITE,  A1_IEM_Bsy,    AXI_RESP_OKAY,  `SCQ_None,  IEM_Bsy,    Data_None,   RspNull,  BusIdle,  EC_None  );

      // Note CLC May2019: Once the pipe is dealing with a capacity eviction, the REQQ will not send another request to that line to the pipe. (though it
      // may have already sent one request). So I'm commenting out the ones that should not be possible
      add_bus_rsp_act(ET_LINK_REQ_ReadCoop,    AXI_REQ_WRITE,  A1_I_Bsy,      AXI_RESP_OKAY,  `SCQ_None,  I,          Data_None,   RspNull,  BusIdle,  EC_None  );
      add_bus_rsp_act(ET_LINK_REQ_ReadCoop,    AXI_REQ_WRITE,  A2_I_Bsy,      AXI_RESP_OKAY,  `SCQ_None,  A1_I_Bsy,   Data_None,   RspNull,  BusIdle,  EC_None  );
      //add_bus_rsp_act(ET_LINK_REQ_ReadCoop,    AXI_REQ_WRITE,  A3_I_Bsy,      AXI_RESP_OKAY,  `SCQ_None,  A2_I_Bsy,   Data_None,   RspNull,  BusIdle,  EC_None  );
      //add_bus_rsp_act(ET_LINK_REQ_ReadCoop,    AXI_REQ_WRITE,  A1_E_Bsy,      AXI_RESP_OKAY,  `SCQ_None,  E,          Data_None,   RspNull,  BusIdle,  EC_None  );
      //add_bus_rsp_act(ET_LINK_REQ_ReadCoop,    AXI_REQ_WRITE,  A2_E_Bsy,      AXI_RESP_OKAY,  `SCQ_None,  A1_E_Bsy,   Data_None,   RspNull,  BusIdle,  EC_None  );
      add_bus_rsp_act(ET_LINK_REQ_ReadCoop,    AXI_REQ_WRITE,  A1_M_Bsy,      AXI_RESP_OKAY,  `SCQ_None,  M,          Data_None,   RspNull,  BusIdle,  EC_None  );
      //add_bus_rsp_act(ET_LINK_REQ_ReadCoop,    AXI_REQ_WRITE,  A2_M_Bsy,      AXI_RESP_OKAY,  `SCQ_None,  A1_M_Bsy,   Data_None,   RspNull,  BusIdle,  EC_None  );
      add_bus_rsp_act(ET_LINK_REQ_ReadCoop,    AXI_REQ_WRITE,  A1_D_Bsy,      AXI_RESP_OKAY,  `SCQ_None,  D,          Data_None,   RspNull,  BusIdle,  EC_None  );
      //add_bus_rsp_act(ET_LINK_REQ_ReadCoop,    AXI_REQ_WRITE,  A2_D_Bsy,      AXI_RESP_OKAY,  `SCQ_None,  A1_D_Bsy,   Data_None,   RspNull,  BusIdle,  EC_None  );
      add_bus_rsp_act(ET_LINK_REQ_ReadCoop,    AXI_REQ_WRITE,  A1_W_Bsy,      AXI_RESP_OKAY,  `SCQ_None,  W,          Data_None,   RspNull,  BusIdle,  EC_None  );
      //add_bus_rsp_act(ET_LINK_REQ_ReadCoop,    AXI_REQ_WRITE,  A2_W_Bsy,      AXI_RESP_OKAY,  `SCQ_None,  A1_W_Bsy,   Data_None,   RspNull,  BusIdle,  EC_None  );
      add_bus_rsp_act(ET_LINK_REQ_ReadCoop,    AXI_REQ_WRITE,  A1_IE_Bsy,     AXI_RESP_OKAY,  `SCQ_None,  IE_Bsy,     Data_None,   RspNull,  BusIdle,  EC_None  );
      //add_bus_rsp_act(ET_LINK_REQ_ReadCoop,    AXI_REQ_WRITE,  A2_IE_Bsy,     AXI_RESP_OKAY,  `SCQ_None,  A1_IE_Bsy,  Data_None,   RspNull,  BusIdle,  EC_None );
      add_bus_rsp_act(ET_LINK_REQ_ReadCoop,    AXI_REQ_WRITE,  A1_IEM_Bsy,    AXI_RESP_OKAY,  `SCQ_None,  IEM_Bsy,    Data_None,   RspNull,  BusIdle,  EC_None  );
      
      // Note CLC May2019: Once the pipe is dealing with a capacity eviction, the REQQ will not send another request to that line to the pipe. (though it
      // may have already sent one request). So I'm commenting out the ones that should not be possible
      add_bus_rsp_act(ET_LINK_REQ_Atomic,    AXI_REQ_WRITE,  A1_I_Bsy,      AXI_RESP_OKAY,  `SCQ_None,  I,          Data_None,   RspNull,  BusIdle,  EC_None  );
      add_bus_rsp_act(ET_LINK_REQ_Atomic,    AXI_REQ_WRITE,  A2_I_Bsy,      AXI_RESP_OKAY,  `SCQ_None,  A1_I_Bsy,   Data_None,   RspNull,  BusIdle,  EC_None  );
      //add_bus_rsp_act(ET_LINK_REQ_Atomic,    AXI_REQ_WRITE,  A3_I_Bsy,      AXI_RESP_OKAY,  `SCQ_None,  A2_I_Bsy,   Data_None,   RspNull,  BusIdle,  EC_None  );
      //add_bus_rsp_act(ET_LINK_REQ_Atomic,    AXI_REQ_WRITE,  A1_E_Bsy,      AXI_RESP_OKAY,  `SCQ_None,  E,          Data_None,   RspNull,  BusIdle,  EC_None  );
      //add_bus_rsp_act(ET_LINK_REQ_Atomic,    AXI_REQ_WRITE,  A2_E_Bsy,      AXI_RESP_OKAY,  `SCQ_None,  A1_E_Bsy,   Data_None,   RspNull,  BusIdle,  EC_None  );
      add_bus_rsp_act(ET_LINK_REQ_Atomic,    AXI_REQ_WRITE,  A1_M_Bsy,      AXI_RESP_OKAY,  `SCQ_None,  M,          Data_None,   RspNull,  BusIdle,  EC_None  );
      //add_bus_rsp_act(ET_LINK_REQ_Atomic,    AXI_REQ_WRITE,  A2_M_Bsy,      AXI_RESP_OKAY,  `SCQ_None,  A1_M_Bsy,   Data_None,   RspNull,  BusIdle,  EC_None  );
      add_bus_rsp_act(ET_LINK_REQ_Atomic,    AXI_REQ_WRITE,  A1_D_Bsy,      AXI_RESP_OKAY,  `SCQ_None,  D,          Data_None,   RspNull,  BusIdle,  EC_None  );
      //add_bus_rsp_act(ET_LINK_REQ_Atomic,    AXI_REQ_WRITE,  A2_D_Bsy,      AXI_RESP_OKAY,  `SCQ_None,  A1_D_Bsy,   Data_None,   RspNull,  BusIdle,  EC_None  );
      add_bus_rsp_act(ET_LINK_REQ_Atomic,    AXI_REQ_WRITE,  A1_W_Bsy,      AXI_RESP_OKAY,  `SCQ_None,  W,          Data_None,   RspNull,  BusIdle,  EC_None  );
      //add_bus_rsp_act(ET_LINK_REQ_Atomic,    AXI_REQ_WRITE,  A2_W_Bsy,      AXI_RESP_OKAY,  `SCQ_None,  A1_W_Bsy,   Data_None,   RspNull,  BusIdle,  EC_None  );
      add_bus_rsp_act(ET_LINK_REQ_Atomic,    AXI_REQ_WRITE,  A1_IE_Bsy,     AXI_RESP_OKAY,  `SCQ_None,  IE_Bsy,     Data_None,   RspNull,  BusIdle,  EC_None  );
      //add_bus_rsp_act(ET_LINK_REQ_Atomic,    AXI_REQ_WRITE,  A2_IE_Bsy,     AXI_RESP_OKAY,  `SCQ_None,  A1_IE_Bsy,  Data_None,   RspNull,  BusIdle,  EC_None  );
      

      
      add_bus_rsp_act(ET_LINK_REQ_Write,     AXI_REQ_WRITE,  A1_I_Bsy,      AXI_RESP_OKAY,  `SCQ_None,  I,          Data_None,   RspNull,  BusIdle,  EC_None  );
      add_bus_rsp_act(ET_LINK_REQ_Write,     AXI_REQ_WRITE,  A2_I_Bsy,      AXI_RESP_OKAY,  `SCQ_None,  A1_I_Bsy,   Data_None,   RspNull,  BusIdle,  EC_None  );
      add_bus_rsp_act(ET_LINK_REQ_Write,     AXI_REQ_WRITE,  A3_I_Bsy,      AXI_RESP_OKAY,  `SCQ_None,  A2_I_Bsy,   Data_None,   RspNull,  BusIdle,  EC_None  );
      add_bus_rsp_act(ET_LINK_REQ_Write,     AXI_REQ_WRITE,  A1_E_Bsy,      AXI_RESP_OKAY,  `SCQ_None,  E,          Data_None,   RspNull,  BusIdle,  EC_None  );
      add_bus_rsp_act(ET_LINK_REQ_Write,     AXI_REQ_WRITE,  A2_E_Bsy,      AXI_RESP_OKAY,  `SCQ_None,  A1_E_Bsy,   Data_None,   RspNull,  BusIdle,  EC_None  );
      add_bus_rsp_act(ET_LINK_REQ_Write,     AXI_REQ_WRITE,  A1_M_Bsy,      AXI_RESP_OKAY,  `SCQ_None,  M,          Data_None,   RspNull,  BusIdle,  EC_None  );
      add_bus_rsp_act(ET_LINK_REQ_Write,     AXI_REQ_WRITE,  A2_M_Bsy,      AXI_RESP_OKAY,  `SCQ_None,  A1_M_Bsy,   Data_None,   RspNull,  BusIdle,  EC_None, m_no_cover );
      add_bus_rsp_act(ET_LINK_REQ_Write,     AXI_REQ_WRITE,  A1_D_Bsy,      AXI_RESP_OKAY,  `SCQ_None,  D,          Data_None,   RspNull,  BusIdle,  EC_None  );
      add_bus_rsp_act(ET_LINK_REQ_Write,     AXI_REQ_WRITE,  A2_D_Bsy,      AXI_RESP_OKAY,  `SCQ_None,  A1_D_Bsy,   Data_None,   RspNull,  BusIdle,  EC_None, m_no_cover );
      add_bus_rsp_act(ET_LINK_REQ_Write,     AXI_REQ_WRITE,  A1_W_Bsy,      AXI_RESP_OKAY,  `SCQ_None,  W,          Data_None,   RspNull,  BusIdle,  EC_None  );
      add_bus_rsp_act(ET_LINK_REQ_Write,     AXI_REQ_WRITE,  A2_W_Bsy,      AXI_RESP_OKAY,  `SCQ_None,  A1_W_Bsy,   Data_None,   RspNull,  BusIdle,  EC_None  );
      add_bus_rsp_act(ET_LINK_REQ_Write,     AXI_REQ_WRITE,  A1_IE_Bsy,     AXI_RESP_OKAY,  `SCQ_None,  IE_Bsy,     Data_None,   RspNull,  BusIdle,  EC_None  );
      add_bus_rsp_act(ET_LINK_REQ_Write,     AXI_REQ_WRITE,  A2_IE_Bsy,     AXI_RESP_OKAY,  `SCQ_None,  A1_IE_Bsy,  Data_None,   RspNull,  BusIdle,  EC_None  );
      // This one was caused by an Idx_L2_Flush:
      add_bus_rsp_act(ET_LINK_REQ_Write,     AXI_REQ_WRITE,  A1_L_Bsy,      AXI_RESP_OKAY,  `SCQ_None,  L,          Data_None,   RspNull,  BusIdle,  EC_None  );
      add_bus_rsp_act(ET_LINK_REQ_Write,     AXI_REQ_WRITE,  A2_L_Bsy,      AXI_RESP_OKAY,  `SCQ_None,  A1_L_Bsy,   Data_None,   RspNull,  BusIdle,  EC_None, m_no_cover  );
      // Flush followed by an Evict. Now the flush response returns:
      add_bus_rsp_act(ET_LINK_REQ_Write,     AXI_REQ_WRITE,  A1_EI_Bsy,     AXI_RESP_OKAY,  `SCQ_None,  EI_Bsy,     Data_None,   RspNull,  BusIdle,  EC_None  );

   
      // Note CLC May2019: Once the pipe is dealing with a capacity eviction, the REQQ will not send another request to that line to the pipe. (though it
      // may have already sent one request). So I'm commenting out the ones that should not be possible
      // *Lock is a bit different than the others because it can leave the line in state A1_D_Bsy, which means it's response can see A2_D_Bsy.
      add_bus_rsp_act(ET_LINK_REQ_Lock,    AXI_REQ_WRITE,  A1_I_Bsy,      AXI_RESP_OKAY,  `SCQ_None,  I,          Data_None,   RspNull,  BusIdle,  EC_None  );
      add_bus_rsp_act(ET_LINK_REQ_Lock,    AXI_REQ_WRITE,  A2_I_Bsy,      AXI_RESP_OKAY,  `SCQ_None,  A1_I_Bsy,   Data_None,   RspNull,  BusIdle,  EC_None  );
      //add_bus_rsp_act(ET_LINK_REQ_Lock,    AXI_REQ_WRITE,  A3_I_Bsy,      AXI_RESP_OKAY,  `SCQ_None,  A2_I_Bsy,   Data_None,   RspNull,  BusIdle,  EC_None  );
      //add_bus_rsp_act(ET_LINK_REQ_Lock,    AXI_REQ_WRITE,  A1_E_Bsy,      AXI_RESP_OKAY,  `SCQ_None,  E,          Data_None,   RspNull,  BusIdle,  EC_None  );
      //add_bus_rsp_act(ET_LINK_REQ_Lock,    AXI_REQ_WRITE,  A2_E_Bsy,      AXI_RESP_OKAY,  `SCQ_None,  A1_E_Bsy,   Data_None,   RspNull,  BusIdle,  EC_None  );
      add_bus_rsp_act(ET_LINK_REQ_Lock,    AXI_REQ_WRITE,  A1_M_Bsy,      AXI_RESP_OKAY,  `SCQ_None,  M,          Data_None,   RspNull,  BusIdle,  EC_None  );
      //add_bus_rsp_act(ET_LINK_REQ_Lock,    AXI_REQ_WRITE,  A2_M_Bsy,      AXI_RESP_OKAY,  `SCQ_None,  A1_M_Bsy,   Data_None,   RspNull,  BusIdle,  EC_None  );
      add_bus_rsp_act(ET_LINK_REQ_Lock,    AXI_REQ_WRITE,  A1_D_Bsy,      AXI_RESP_OKAY,  `SCQ_None,  D,          Data_None,   RspNull,  BusIdle,  EC_None  );
      add_bus_rsp_act(ET_LINK_REQ_Lock,    AXI_REQ_WRITE,  A2_D_Bsy,      AXI_RESP_OKAY,  `SCQ_None,  A1_D_Bsy,   Data_None,   RspNull,  BusIdle,  EC_None  );
      add_bus_rsp_act(ET_LINK_REQ_Lock,    AXI_REQ_WRITE,  A1_W_Bsy,      AXI_RESP_OKAY,  `SCQ_None,  W,          Data_None,   RspNull,  BusIdle,  EC_None  );
      //add_bus_rsp_act(ET_LINK_REQ_Lock,    AXI_REQ_WRITE,  A2_W_Bsy,      AXI_RESP_OKAY,  `SCQ_None,  A1_W_Bsy,   Data_None,   RspNull,  BusIdle,  EC_None  );
      add_bus_rsp_act(ET_LINK_REQ_Lock,    AXI_REQ_WRITE,  A1_IE_Bsy,     AXI_RESP_OKAY,  `SCQ_None,  IE_Bsy,     Data_None,   RspNull,  BusIdle,  EC_None  );
      //add_bus_rsp_act(ET_LINK_REQ_Lock,    AXI_REQ_WRITE,  A2_IE_Bsy,     AXI_RESP_OKAY,  `SCQ_None,  A1_IE_Bsy,  Data_None,   RspNull,  BusIdle,  EC_None  );
      add_bus_rsp_act(ET_LINK_REQ_Lock,    AXI_REQ_WRITE,  A1_IEM_Bsy,    AXI_RESP_OKAY,  `SCQ_None,  IEM_Bsy,    Data_None,   RspNull,  BusIdle,  EC_None  );
      
      //
      // Response to a Forced Partial Eviction due to a partial read/atomic
      //                                     AXI             Model                                                                          Bus
      //              ET-link Opcode         Command         State       AXI Response    Qual            Tag Action  Data Action  Rsp Type  Command         Err Code
      //              ---------------------  --------------  ----------  --------------  --------------  ----------  -----------  --------  --------------  --------
      add_bus_rsp_act(ET_LINK_REQ_Read,      AXI_REQ_WRITE,  A1_IE_Bsy,  AXI_RESP_OKAY,  `SCQ_LateRead,  IE_Bsy,     Data_None,   RspNull,  BusReadParent,  EC_None  );
      add_bus_rsp_act(ET_LINK_REQ_ReadCoop,  AXI_REQ_WRITE,  A1_IE_Bsy,  AXI_RESP_OKAY,  `SCQ_LateRead,  IE_Bsy,     Data_None,   RspNull,  BusReadParent,  EC_None  );
      add_bus_rsp_act(ET_LINK_REQ_Atomic,    AXI_REQ_WRITE,  A1_IE_Bsy,  AXI_RESP_OKAY,  `SCQ_LateRead,  IE_Bsy,     Data_None,   RspNull,  BusReadParent,  EC_None  );
      add_bus_rsp_act(ET_LINK_REQ_Prefetch,  AXI_REQ_WRITE,  A1_IE_Bsy,  AXI_RESP_OKAY,  `SCQ_LateRead,  IE_Bsy,     Data_None,   RspNull,  BusReadParent,  EC_None  );
      add_bus_rsp_act(ET_LINK_REQ_Read,      AXI_REQ_WRITE,  A2_IE_Bsy,  AXI_RESP_OKAY,  `SCQ_LateRead,  A1_IE_Bsy,  Data_None,   RspNull,  BusReadParent,  EC_None, m_no_cover );
      add_bus_rsp_act(ET_LINK_REQ_ReadCoop,  AXI_REQ_WRITE,  A2_IE_Bsy,  AXI_RESP_OKAY,  `SCQ_LateRead,  A1_IE_Bsy,  Data_None,   RspNull,  BusReadParent,  EC_None, m_no_cover );
      add_bus_rsp_act(ET_LINK_REQ_Atomic,    AXI_REQ_WRITE,  A2_IE_Bsy,  AXI_RESP_OKAY,  `SCQ_LateRead,  A1_IE_Bsy,  Data_None,   RspNull,  BusReadParent,  EC_None, m_no_cover );
      add_bus_rsp_act(ET_LINK_REQ_Prefetch,  AXI_REQ_WRITE,  A2_IE_Bsy,  AXI_RESP_OKAY,  `SCQ_LateRead,  A1_IE_Bsy,  Data_None,   RspNull,  BusReadParent,  EC_None, m_no_cover );

      //
      // Response to a Forced Eviction due to either a filled write-around or a capacity eviction
      //                                        AXI             Model                                                                         Bus
      //              ET-link Opcode            Command         State          AXI Response    Qual        Tag Action  Data Action  Rsp Type  Command   Err Code
      //              ------------------------  --------------  -------------  --------------  ----------  ----------  -----------  --------  --------  --------
      // Note CLC May2019: Once the pipe is dealing with a capacity eviction, the REQQ will not send another request to that line to the pipe. (though it
      // may have already sent one request). So I'm commenting out the ones that should not be possible
      add_bus_rsp_act(ET_LINK_REQ_WriteAround,     AXI_REQ_WRITE,  A1_I_Bsy,      AXI_RESP_OKAY,  `SCQ_None,  I,          Data_None,   RspNull,  BusIdle,  EC_None  );
      add_bus_rsp_act(ET_LINK_REQ_WriteAround,     AXI_REQ_WRITE,  A2_I_Bsy,      AXI_RESP_OKAY,  `SCQ_None,  A1_I_Bsy,   Data_None,   RspNull,  BusIdle,  EC_None  );
      //add_bus_rsp_act(ET_LINK_REQ_WriteAround,     AXI_REQ_WRITE,  A3_I_Bsy,      AXI_RESP_OKAY,  `SCQ_None,  A2_I_Bsy,   Data_None,   RspNull,  BusIdle,  EC_None  );
      //add_bus_rsp_act(ET_LINK_REQ_WriteAround,     AXI_REQ_WRITE,  A1_E_Bsy,      AXI_RESP_OKAY,  `SCQ_None,  E,          Data_None,   RspNull,  BusIdle,  EC_None  );
      //add_bus_rsp_act(ET_LINK_REQ_WriteAround,     AXI_REQ_WRITE,  A2_E_Bsy,      AXI_RESP_OKAY,  `SCQ_None,  A1_E_Bsy,   Data_None,   RspNull,  BusIdle,  EC_None  );
      add_bus_rsp_act(ET_LINK_REQ_WriteAround,     AXI_REQ_WRITE,  A1_M_Bsy,      AXI_RESP_OKAY,  `SCQ_None,  M,          Data_None,   RspNull,  BusIdle,  EC_None  );
      //add_bus_rsp_act(ET_LINK_REQ_WriteAround,     AXI_REQ_WRITE,  A2_M_Bsy,      AXI_RESP_OKAY,  `SCQ_None,  A1_M_Bsy,   Data_None,   RspNull,  BusIdle,  EC_None  );
      add_bus_rsp_act(ET_LINK_REQ_WriteAround,     AXI_REQ_WRITE,  A1_D_Bsy,      AXI_RESP_OKAY,  `SCQ_None,  D,          Data_None,   RspNull,  BusIdle,  EC_None  );
      //add_bus_rsp_act(ET_LINK_REQ_WriteAround,     AXI_REQ_WRITE,  A2_D_Bsy,      AXI_RESP_OKAY,  `SCQ_None,  A1_D_Bsy,   Data_None,   RspNull,  BusIdle,  EC_None  );
      add_bus_rsp_act(ET_LINK_REQ_WriteAround,     AXI_REQ_WRITE,  A1_W_Bsy,      AXI_RESP_OKAY,  `SCQ_None,  W,          Data_None,   RspNull,  BusIdle,  EC_None  );
      //add_bus_rsp_act(ET_LINK_REQ_WriteAround,     AXI_REQ_WRITE,  A2_W_Bsy,      AXI_RESP_OKAY,  `SCQ_None,  A1_W_Bsy,   Data_None,   RspNull,  BusIdle,  EC_None  );
      add_bus_rsp_act(ET_LINK_REQ_WriteAround,     AXI_REQ_WRITE,  A1_IE_Bsy,     AXI_RESP_OKAY,  `SCQ_None,  IE_Bsy,     Data_None,   RspNull,  BusIdle,  EC_None  );
      //add_bus_rsp_act(ET_LINK_REQ_WriteAround,     AXI_REQ_WRITE,  A2_IE_Bsy,     AXI_RESP_OKAY,  `SCQ_None,  A1_IE_Bsy,  Data_None,   RspNull,  BusIdle,  EC_None  );

      
      //
      // Response to a Cacheop (Flush)
      //                                  AXI             Model                                                                    Bus
      //              ET-link Opcode      Command         State      AXI Response    Qual         Tag Action  Data Action  Rsp Type  Command   Err Code
      //              ------------------  --------------  -------    --------------  -----------  ----------  -----------  --------  --------  --------
      add_bus_rsp_act(ET_LINK_REQ_Flush,  AXI_REQ_WRITE,  A1_I_Bsy,   AXI_RESP_OKAY,  `SCQ_None,   I,          Data_None,   RspAck,   BusIdle,  EC_None  );
      add_bus_rsp_act(ET_LINK_REQ_Flush,  AXI_REQ_WRITE,  A1_E_Bsy,   AXI_RESP_OKAY,  `SCQ_None,   E,          Data_None,   RspAck,   BusIdle,  EC_None  );
      add_bus_rsp_act(ET_LINK_REQ_Flush,  AXI_REQ_WRITE,  A1_M_Bsy,   AXI_RESP_OKAY,  `SCQ_None,   M,          Data_None,   RspAck,   BusIdle,  EC_None, m_no_cover );
      add_bus_rsp_act(ET_LINK_REQ_Flush,  AXI_REQ_WRITE,  A1_L_Bsy,   AXI_RESP_OKAY,  `SCQ_None,   L,          Data_None,   RspAck,   BusIdle,  EC_None  );
      add_bus_rsp_act(ET_LINK_REQ_Flush,  AXI_REQ_WRITE,  A1_D_Bsy,   AXI_RESP_OKAY,  `SCQ_None,   D,          Data_None,   RspAck,   BusIdle,  EC_None  );
      add_bus_rsp_act(ET_LINK_REQ_Flush,  AXI_REQ_WRITE,  Stale,      AXI_RESP_OKAY,  `SCQ_Stale,  St_Ignore,  Data_None,   RspAck,   BusIdle,  EC_None  );

      // No L2: "but I thought if there's no L2 it won't send a flush downstream?"  "Don't worry, this is a flush that doesn't apply to l2 (ie: L3->Mem)"
      // CLC July2019: This actually shows up as the "Stale" case above.
      //add_bus_rsp_act(ET_LINK_REQ_Flush,  AXI_REQ_WRITE,  I,       AXI_RESP_OKAY,  `SCQ_NoRegion,   I,      Data_None,   RspAck,   BusIdle,  EC_None  );

      //
      // Response to a Cacheop (Evict)
      //                                  AXI             Model                                                                    Bus
      //              ET-link Opcode      Command         State       AXI Response    Qual         Tag Action  Data Action  Rsp Type  Command   Err Code
      //              ------------------  --------------  -------     --------------  -----------  ----------  -----------  --------  --------  --------
      add_bus_rsp_act(ET_LINK_REQ_Evict,  AXI_REQ_WRITE,  EI_Bsy,     AXI_RESP_OKAY,  `SCQ_None,   I,          Data_None,   RspNull,  BusIdle,  EC_None  );
      add_bus_rsp_act(ET_LINK_REQ_Evict,  AXI_REQ_WRITE,  MI_Bsy,     AXI_RESP_OKAY,  `SCQ_None,   I,          Data_None,   RspNull,  BusIdle,  EC_None  );
      add_bus_rsp_act(ET_LINK_REQ_Evict,  AXI_REQ_WRITE,  A1_I_Bsy,   AXI_RESP_OKAY,  `SCQ_None,   I,          Data_None,   RspAck,   BusIdle,  EC_None  );
      add_bus_rsp_act(ET_LINK_REQ_Evict,  AXI_REQ_WRITE,  A1_E_Bsy,   AXI_RESP_OKAY,  `SCQ_None,   E,          Data_None,   RspAck,   BusIdle,  EC_None, m_no_cover );
      add_bus_rsp_act(ET_LINK_REQ_Evict,  AXI_REQ_WRITE,  A1_M_Bsy,   AXI_RESP_OKAY,  `SCQ_None,   M,          Data_None,   RspAck,   BusIdle,  EC_None, m_no_cover );
      add_bus_rsp_act(ET_LINK_REQ_Evict,  AXI_REQ_WRITE,  A1_L_Bsy,   AXI_RESP_OKAY,  `SCQ_None,   L,          Data_None,   RspAck,   BusIdle,  EC_None  );
      add_bus_rsp_act(ET_LINK_REQ_Evict,  AXI_REQ_WRITE,  A1_D_Bsy,   AXI_RESP_OKAY,  `SCQ_None,   D,          Data_None,   RspAck,   BusIdle,  EC_None  );
      add_bus_rsp_act(ET_LINK_REQ_Evict,  AXI_REQ_WRITE,  Stale,      AXI_RESP_OKAY,  `SCQ_Stale,  St_Ignore,  Data_None,   RspAck,   BusIdle,  EC_None  );

      // No L2: "but I thought if there's no L2 it won't send a flush downstream?"  "Don't worry, this is a flush that doesn't apply to l2 (ie: L3->Mem)"
      // CLC July2019: This actually shows up as the "Stale" case above.
      //add_bus_rsp_act(ET_LINK_REQ_Evict,  AXI_REQ_WRITE,  I,       AXI_RESP_OKAY,  `SCQ_NoRegion,   I,      Data_None,   RspAck,   BusIdle,  EC_None  );

      //
      // Response to a Cacheop (Prefetch)
      //                                     AXI             Model                                                                          Bus
      //              ET-link Opcode         Command         State       AXI Response    Qual         Tag Action  Data Action        Rsp Type  Command   Err Code
      //              ---------------------  --------------  -------     --------------  -----------  ----------  -----------------  --------  --------  --------
      add_bus_rsp_act(ET_LINK_REQ_Prefetch,  AXI_REQ_READ,   IE_Bsy,     AXI_RESP_OKAY,  `SCQ_None,   St_Ignore,  Data_EnqueueFill,  RspAck,   BusIdle,  EC_None  );

      add_bus_rsp_act(ET_LINK_REQ_Prefetch,  AXI_REQ_WRITE,  Stale,      AXI_RESP_OKAY,  `SCQ_Stale,  St_Ignore,  Data_None,         RspAck,   BusIdle,  EC_None  );

      // No L2:
      //CLC July2019: this actually shows up as the "Stale" case above.
      //add_bus_rsp_act(ET_LINK_REQ_Prefetch,  AXI_REQ_WRITE,  I,       AXI_RESP_OKAY,  `SCQ_NoRegion,   I,      Data_None,         RspAck,   BusIdle,  EC_None  );

      //
      // Response to a Capacity Eviction
      //                                     AXI             Model                                                                         Bus
      //              ET-link Opcode         Command         State          AXI Response    Qual        Tag Action  Data Action  Rsp Type  Command   Err Code
      //              ---------------------  --------------  -------------  --------------  ----------  ----------  -----------  --------  --------  --------
      // Note CLC May2019: Once the pipe is dealing with a capacity eviction, the REQQ will not send another request to that line to the pipe. (though it
      // may have already sent one request). So I'm commenting out the ones that should not be possible
      add_bus_rsp_act(ET_LINK_REQ_Prefetch,  AXI_REQ_WRITE,  A1_I_Bsy,      AXI_RESP_OKAY,  `SCQ_None,  I,          Data_None,   RspNull,  BusIdle,  EC_None  );
      add_bus_rsp_act(ET_LINK_REQ_Prefetch,  AXI_REQ_WRITE,  A2_I_Bsy,      AXI_RESP_OKAY,  `SCQ_None,  A1_I_Bsy,   Data_None,   RspNull,  BusIdle,  EC_None  );
      //add_bus_rsp_act(ET_LINK_REQ_Prefetch,  AXI_REQ_WRITE,  A3_I_Bsy,      AXI_RESP_OKAY,  `SCQ_None,  A2_I_Bsy,   Data_None,   RspNull,  BusIdle,  EC_None  );
      //add_bus_rsp_act(ET_LINK_REQ_Prefetch,  AXI_REQ_WRITE,  A1_E_Bsy,      AXI_RESP_OKAY,  `SCQ_None,  E,          Data_None,   RspNull,  BusIdle,  EC_None  );
      //add_bus_rsp_act(ET_LINK_REQ_Prefetch,  AXI_REQ_WRITE,  A2_E_Bsy,      AXI_RESP_OKAY,  `SCQ_None,  A1_E_Bsy,   Data_None,   RspNull,  BusIdle,  EC_None  );
      add_bus_rsp_act(ET_LINK_REQ_Prefetch,  AXI_REQ_WRITE,  A1_M_Bsy,      AXI_RESP_OKAY,  `SCQ_None,  M,          Data_None,   RspNull,  BusIdle,  EC_None  );
      //add_bus_rsp_act(ET_LINK_REQ_Prefetch,  AXI_REQ_WRITE,  A2_M_Bsy,      AXI_RESP_OKAY,  `SCQ_None,  A1_M_Bsy,   Data_None,   RspNull,  BusIdle,  EC_None  );
      add_bus_rsp_act(ET_LINK_REQ_Prefetch,  AXI_REQ_WRITE,  A1_D_Bsy,      AXI_RESP_OKAY,  `SCQ_None,  D,          Data_None,   RspNull,  BusIdle,  EC_None  );
      //add_bus_rsp_act(ET_LINK_REQ_Prefetch,  AXI_REQ_WRITE,  A2_D_Bsy,      AXI_RESP_OKAY,  `SCQ_None,  A1_D_Bsy,   Data_None,   RspNull,  BusIdle,  EC_None  );
      add_bus_rsp_act(ET_LINK_REQ_Prefetch,  AXI_REQ_WRITE,  A1_W_Bsy,      AXI_RESP_OKAY,  `SCQ_None,  W,          Data_None,   RspNull,  BusIdle,  EC_None  );
      //add_bus_rsp_act(ET_LINK_REQ_Prefetch,  AXI_REQ_WRITE,  A2_W_Bsy,      AXI_RESP_OKAY,  `SCQ_None,  A1_W_Bsy,   Data_None,   RspNull,  BusIdle,  EC_None  );
      add_bus_rsp_act(ET_LINK_REQ_Prefetch,  AXI_REQ_WRITE,  A1_IE_Bsy,     AXI_RESP_OKAY,  `SCQ_None,  IE_Bsy,     Data_None,   RspNull,  BusIdle,  EC_None  );
      //add_bus_rsp_act(ET_LINK_REQ_Prefetch,  AXI_REQ_WRITE,  A2_IE_Bsy,     AXI_RESP_OKAY,  `SCQ_None,  A1_IE_Bsy,  Data_None,   RspNull,  BusIdle,  EC_None  );
      add_bus_rsp_act(ET_LINK_REQ_Prefetch,  AXI_REQ_WRITE,  A1_IEM_Bsy,    AXI_RESP_OKAY,  `SCQ_None,  IEM_Bsy,    Data_None,   RspNull,  BusIdle,  EC_None  );
      
      //
      // Response to a evict/flush Write to mem due to an Index cacheop.
      //
      //                                       AXI             Model                                                                   Bus
      //              ET-link Opcode           Command         State   AXI Response    Qual            Tag Action  Data Action  Rsp Type   Command   Err Code
      //              -----------------------  -------------   ------  --------------  ----------      ----------  -----------  ---------  --------  --------
      //CLC July2019: These are old and not used anymore 
      //add_bus_rsp_act(ET_LINK_REQ_Write,       AXI_REQ_WRITE,  I_Bsy,  AXI_RESP_OKAY,  `SCQ_None,      I,          Data_None,   RspNull,   BusIdle,  EC_None  );
      //add_bus_rsp_act(ET_LINK_REQ_Write,       AXI_REQ_WRITE,  E_Bsy,  AXI_RESP_OKAY,  `SCQ_None,      E,          Data_None,   RspNull,   BusIdle,  EC_None  );


      //
      // Fun with partial writes.
      //
      //                                       AXI             Model                                                                   Bus
      //              ET-link Opcode           Command         State       AXI Response    Qual         Tag Action  Data Action        Rsp Type   Command   Err Code
      //              -----------------------  -------------   ------      --------------  ----------   ---------  -----------       ---------  --------  --------
      // Response to a read that was sent for a read-modify-write due to a partial write
      add_bus_rsp_act(ET_LINK_REQ_Write,       AXI_REQ_READ,   IEM_Bsy,    AXI_RESP_OKAY,  `SCQ_None,   St_Ignore,  Data_EnqueueFill,   RspAck,   BusIdle,  EC_None );
      // Same as above but there was an eviction shortly before the fun started
      add_bus_rsp_act(ET_LINK_REQ_Write,       AXI_REQ_READ,   A1_IEM_Bsy, AXI_RESP_OKAY,  `SCQ_None,   St_Ignore,  Data_EnqueueFill,   RspAck,   BusIdle,  EC_None, m_no_cover );

      // a partial write on top of writeAroundData
      add_bus_rsp_act(ET_LINK_REQ_Write,       AXI_REQ_WRITE,  A1_IEM_Bsy, AXI_RESP_OKAY, `SCQ_LateRead,IEM_Bsy,    Data_None,          RspAck,   BusLineRd, EC_None );
      // a writePartial after an eviction
      add_bus_rsp_act(ET_LINK_REQ_Write,       AXI_REQ_WRITE,  A1_IEM_Bsy, AXI_RESP_OKAY, `SCQ_None,    IEM_Bsy,    Data_None,          RspAck,   BusIdle, EC_None );
      add_bus_rsp_act(ET_LINK_REQ_Write,       AXI_REQ_WRITE,  A2_IEM_Bsy, AXI_RESP_OKAY, `SCQ_None,    A1_IEM_Bsy,    Data_None,          RspAck,   BusIdle, EC_None, m_no_cover );
      
      // A WriteAround which caused a writeBack was followed by a WritePartial.
      add_bus_rsp_act(ET_LINK_REQ_WriteAround, AXI_REQ_WRITE,  A1_IEM_Bsy, AXI_RESP_OKAY, `SCQ_None,    IEM_Bsy,    Data_None,          RspAck,   BusIdle, EC_None ); 

      
      //
      // Responses while in Bypass
      //
      //                                       AXI             Model                                                                   Bus
      //              ET-link Opcode           Command         State    AXI Response    Qual            Tag Action  Data Action        Rsp Type   Command   Err Code
      //              -----------------------  -------------   ------   --------------  ----------      ----------  -----------       ---------  --------  --------
      add_bus_rsp_act(ET_LINK_REQ_Write,       AXI_REQ_WRITE,  I,       AXI_RESP_OKAY,  `SCQ_Bypass,     St_Ignore,  Data_None,        RspAck,   BusIdle,  EC_None );
      add_bus_rsp_act(ET_LINK_REQ_WriteAround, AXI_REQ_WRITE,  I,       AXI_RESP_OKAY,  `SCQ_Bypass,     St_Ignore,  Data_None,        RspAck,   BusIdle,  EC_None );
      add_bus_rsp_act(ET_LINK_REQ_Prefetch,    AXI_REQ_WRITE,  I,       AXI_RESP_OKAY,  `SCQ_Bypass,     St_Ignore,  Data_None,        RspAck,   BusIdle,  EC_None );
      add_bus_rsp_act(ET_LINK_REQ_Evict,       AXI_REQ_WRITE,  I,       AXI_RESP_OKAY,  `SCQ_Bypass,     St_Ignore,  Data_None,        RspAck,   BusIdle,  EC_None );
      add_bus_rsp_act(ET_LINK_REQ_Flush,       AXI_REQ_WRITE,  I,       AXI_RESP_OKAY,  `SCQ_Bypass,     St_Ignore,  Data_None,        RspAck,   BusIdle,  EC_None );
      add_bus_rsp_act(ET_LINK_REQ_Read,        AXI_REQ_READ,   I,       AXI_RESP_OKAY,  `SCQ_Bypass,     St_Ignore,  Data_CascadeRsp,  RspWData, BusIdle,  EC_None );
      add_bus_rsp_act(ET_LINK_REQ_ReadCoop,    AXI_REQ_READ,   I,       AXI_RESP_OKAY,  `SCQ_Bypass,     St_Ignore,  Data_CascadeRsp,  RspWData, BusIdle,  EC_None );
      add_bus_rsp_act(ET_LINK_REQ_Write,       AXI_REQ_WRITE,  E,       AXI_RESP_OKAY,  `SCQ_Bypass,     St_Ignore,  Data_None,        RspAck,   BusIdle,  EC_None );
      add_bus_rsp_act(ET_LINK_REQ_WriteAround, AXI_REQ_WRITE,  E,       AXI_RESP_OKAY,  `SCQ_Bypass,     St_Ignore,  Data_None,        RspAck,   BusIdle,  EC_None );
      add_bus_rsp_act(ET_LINK_REQ_Prefetch,    AXI_REQ_WRITE,  E,       AXI_RESP_OKAY,  `SCQ_Bypass,     St_Ignore,  Data_None,        RspAck,   BusIdle,  EC_None );
      add_bus_rsp_act(ET_LINK_REQ_Evict,       AXI_REQ_WRITE,  E,       AXI_RESP_OKAY,  `SCQ_Bypass,     St_Ignore,  Data_None,        RspAck,   BusIdle,  EC_None );
      add_bus_rsp_act(ET_LINK_REQ_Flush,       AXI_REQ_WRITE,  E,       AXI_RESP_OKAY,  `SCQ_Bypass,     St_Ignore,  Data_None,        RspAck,   BusIdle,  EC_None );
      add_bus_rsp_act(ET_LINK_REQ_Read,        AXI_REQ_READ,   E,       AXI_RESP_OKAY,  `SCQ_Bypass,     St_Ignore,  Data_CascadeRsp,  RspWData, BusIdle,  EC_None );
      add_bus_rsp_act(ET_LINK_REQ_ReadCoop,    AXI_REQ_READ,   E,       AXI_RESP_OKAY,  `SCQ_Bypass,     St_Ignore,  Data_CascadeRsp,  RspWData, BusIdle,  EC_None );
      // These came up in bring-up testing when the L2 was put in bypass while reads to memory were happening.               
      add_bus_rsp_act(ET_LINK_REQ_Write,       AXI_REQ_WRITE,  IE_Bsy,       AXI_RESP_OKAY,  `SCQ_Bypass,     St_Ignore,  Data_None,        RspAck,   BusIdle,  EC_None );
      add_bus_rsp_act(ET_LINK_REQ_WriteAround, AXI_REQ_WRITE,  IE_Bsy,       AXI_RESP_OKAY,  `SCQ_Bypass,     St_Ignore,  Data_None,        RspAck,   BusIdle,  EC_None );
      add_bus_rsp_act(ET_LINK_REQ_Prefetch,    AXI_REQ_WRITE,  IE_Bsy,       AXI_RESP_OKAY,  `SCQ_Bypass,     St_Ignore,  Data_None,        RspAck,   BusIdle,  EC_None );
      add_bus_rsp_act(ET_LINK_REQ_Evict,       AXI_REQ_WRITE,  IE_Bsy,       AXI_RESP_OKAY,  `SCQ_Bypass,     St_Ignore,  Data_None,        RspAck,   BusIdle,  EC_None );
      add_bus_rsp_act(ET_LINK_REQ_Flush,       AXI_REQ_WRITE,  IE_Bsy,       AXI_RESP_OKAY,  `SCQ_Bypass,     St_Ignore,  Data_None,        RspAck,   BusIdle,  EC_None );
      add_bus_rsp_act(ET_LINK_REQ_Read,        AXI_REQ_READ,   IE_Bsy,       AXI_RESP_OKAY,  `SCQ_Bypass,     St_Ignore,  Data_CascadeRsp,  RspWData, BusIdle,  EC_None );
      add_bus_rsp_act(ET_LINK_REQ_ReadCoop,    AXI_REQ_READ,   IE_Bsy,       AXI_RESP_OKAY,  `SCQ_Bypass,     St_Ignore,  Data_CascadeRsp,  RspWData, BusIdle,  EC_None );
      
      // Response to a WriteAround when cbuf was disabled
      add_bus_rsp_act(ET_LINK_REQ_WriteAround, AXI_REQ_WRITE,  Stale,     AXI_RESP_OKAY,  `SCQ_Stale,  St_Ignore,  Data_None,        RspAck,   BusIdle,  EC_None );

      // Response to ReadForward and WriteForward
      add_bus_rsp_act(ET_LINK_REQ_Read,       AXI_REQ_READ,  Stale, AXI_RESP_OKAY, `SCQ_Stale, St_Ignore, Data_CascadeRsp, RspWData, BusIdle, EC_None);
      add_bus_rsp_act(ET_LINK_REQ_Write,      AXI_REQ_WRITE, Stale, AXI_RESP_OKAY, `SCQ_Stale, St_Ignore, Data_None,       RspAck,   BusIdle, EC_None);
      
      m_cache.build_coverage();
   endfunction : build_phase


   virtual function void final_axi_req_fixup(input evl_req_desc axi_req_desc, input evl_abstract_bus_cmd_t abstract_bus_cmd);
      evl_sc_bank_id_t bank_id;
      evl_dword_t esr_axi_qos;
      evl_shire_axi_ifc_type_t axi_ifc_type;
      
      bank_id = axi_req_desc.get_bus_bank();
      esr_axi_qos = m_regs[bank_id].sc_reqq_ctl.ESR_SC_AXI_QOS.get();
    
      `evl_log_mdesc(UVM_HIGH, VMOD_SC_SB, get_abstract_name(), axi_req_desc, $sformatf("final_axi_req_fixup: get_bus_qos = 0x%x, m_remote_l3_enable = 0x%x esr_axi_qos = %x",axi_req_desc.get_bus_qos(),m_remote_l3_enable,esr_axi_qos))

      if (esr_axi_qos == `AXI_QOS_MEM_HIGH_PRIORITY) begin
         axi_req_desc.set_bus_qos(`AXI_QOS_MEM_HIGH_PRIORITY);
      end
      else begin
         axi_req_desc.set_bus_qos(`AXI_QOS_MEM_LOW_PRIORITY);
      end

      //What port to we expect this to show up on ?
      if (m_remote_l3_enable == 0) begin
         axi_ifc_type = SHIRE_L3TOSYS_MESH_AXI;
      end
      else begin
         axi_ifc_type = SHIRE_L2TOL3_MESH_AXI;
      end
      axi_req_desc.set_int_param(pk_sc_downstream_port,int'(axi_ifc_type));
   endfunction: final_axi_req_fixup

   //-----------------------------------------------------------------------------------------------
   //
   // Function: fix_qualifier_at_reqq_alloc
   //
   virtual function evl_sc_qual_t fix_qualifier_at_reqq_alloc(input evl_sc_qual_t qualifier_in, input evl_req_desc req_desc);
      evl_bus_cmd_t bus_cmd;
   
      // The L2 behaves differently for some requests if we're not sending requests to the L3
      if (m_remote_l3_enable == 0) begin
         bus_cmd = evl_bus_cmd_t'(req_desc.get_int_param(pk_etlink_cmd));
         `evl_log_mdesc(UVM_HIGH, VMOD_SC_SB, get_abstract_name(), req_desc, $sformatf(" fix_qualifier_at_reqq_alloc. remote_l3_enable =0. bus_cmd = %0p",bus_cmd))
         if (bus_cmd inside {ET_LINK_REQ_Flush, ET_LINK_REQ_Evict, ET_LINK_REQ_Prefetch}) begin
            return (qualifier_in | `SCQ_DownDis);
         end
      end
      return qualifier_in;
   endfunction : fix_qualifier_at_reqq_alloc
   

   //-----------------------------------------------------------------------------------------------
   //
   // Function: fix_qualifier_at_tc_req
   //
   virtual function evl_sc_qual_t fix_qualifier_at_tc_req(input evl_sc_qual_t qualifier_in, input evl_req_desc req_desc);
      evl_bus_cmd_t bus_cmd;
      evl_sc_qual_t qualifier_out;
      bus_cmd = evl_bus_cmd_t'(req_desc.get_int_param(pk_etlink_cmd));
      qualifier_out = qualifier_in;
      
      // The L2 behaves differently for some requests if we're not sending requests to the L3
      if (m_remote_l3_enable == 0) begin
         `evl_log_mdesc(UVM_HIGH, VMOD_SC_SB, get_abstract_name(), req_desc, $sformatf(" fix_qualifier_at_tc_req. remote_l3_enable =0. bus_cmd = %0p",bus_cmd))
         if (bus_cmd inside {ET_LINK_REQ_Flush, ET_LINK_REQ_Evict}) begin
            qualifier_out = qualifier_out | `SCQ_DownDis;
         end
      end

      // Qualifier for remote scp WriteAround and Evict so that we can keep them separate in the table for better coverage.
      if ((bus_cmd inside {ET_LINK_REQ_WriteAround,ET_LINK_REQ_Evict}) && (req_desc.get_int_param(pk_remote_shire) == 1)) begin
         qualifier_out = qualifier_out | `SCQ_Remote;
      end
      return qualifier_out;
   endfunction : fix_qualifier_at_tc_req

   //-----------------------------------------------------------------------------------------------
   //
   // Function: fix_qualifier_at_tag_rsp
   //
   virtual function evl_sc_qual_t fix_qualifier_at_tag_rsp(input evl_sc_qual_t qualifier_in, input evl_req_desc req_desc);
      evl_bus_cmd_t bus_cmd;
   
      // The L2 behaves differently for some requests if we're not sending requests to the L3
      if (m_remote_l3_enable == 0) begin
         bus_cmd = evl_bus_cmd_t'(req_desc.get_int_param(pk_etlink_cmd));
         `evl_log_mdesc(UVM_HIGH, VMOD_SC_SB, get_abstract_name(), req_desc, $sformatf(" fix_qualifier_at_tag_rsp. remote_l3_enable =0. bus_cmd = %0p",bus_cmd))
         if (bus_cmd inside {ET_LINK_REQ_Flush, ET_LINK_REQ_Evict}) begin
            return qualifier_in | `SCQ_DownDis;
         end
      end
      return qualifier_in;
   endfunction : fix_qualifier_at_tag_rsp


   //-----------------------------------------------------------------------------------------------
   //
   // Function: start_of_simulation_phase
   //
   function void start_of_simulation_phase(uvm_phase phase);
      super.start_of_simulation_phase(phase);

      if (m_cache != null) begin
         m_cache.set_abstract_name($sformatf("%s(L2$)", get_root_abstract_name()));
      end
   endfunction : start_of_simulation_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Task: pre_main_phase
   //
   task pre_main_phase(uvm_phase phase);
      capture_configuration();
      super.pre_main_phase(phase);
   endtask : pre_main_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string name_in = "evl_l2_scoreboard", input uvm_component parent_in = null);
      super.new(name_in, parent_in);
      set_abstract_name("SHIRE_CACHE(L2SB)");
      m_perform_qword_en_check = 1'b1;
      m_sb_type                = int'(L2_SCOREBOARD);
      m_verbosity_mod          = VMOD_L2_SB_WARN;
   endfunction : new

endclass : evl_l2_scoreboard

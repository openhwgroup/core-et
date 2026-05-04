//--------------------------------------------------------------------------------------------------
//
// Class: evl_scp_scoreboard
//
// This scp_scoreboard extends the shire cache scoreboard and defines a table which
// describes the actions the shire cache scoreboard should take to model
// and scp.
//
class evl_scp_scoreboard extends evl_shire_cache_scoreboard #(evl_scp_scoreboard);

   `uvm_component_utils(evl_scp_scoreboard)


   //-----------------------------------------------------------------------------------------------
   //
   // Static Member Variables and Objects
   //
   `evl_param_key(pk_scp_index_msb)
   `evl_param_key(pk_scp_set_size)


   //-----------------------------------------------------------------------------------------------
   //
   // Function: process_sc_bank_config
   //
   virtual function void process_sc_bank_config(input evl_sc_bank_config_t rtl_info);
      int my_index_msb;

      super.process_sc_bank_config(rtl_info);
      if (m_mem_size > 0) begin
         m_stim_index_mask = ((evl_paddr_t'(1) << ($clog2(m_mem_size))) - 1) & (~evl_paddr_t'(`EVL_LINE_BYTES - 1));;
      end
      m_global_attrs.set_scp_config(get_shire_id(), m_remote_enable, m_mem_size, m_set_base_addr, ((m_set_base_addr + m_mem_size - 1) & m_stim_index_mask) | (`EVL_LINE_BYTES - 1), m_stim_index_mask, m_alias_mask);
      m_agent.set_int_param(pk_scp_set_size, m_set_size);
      m_agent.set_int_param(pk_scp_index_msb, m_index_msb);
   endfunction : process_sc_bank_config


   //-----------------------------------------------------------------------------------------------
   //
   // Function: process_error_code
   //
   virtual function void process_error_code(input evl_req_desc req_desc, input evl_error_code error_code_in);
      if (error_code_in == EC_Error01) begin
         evl_line_desc line_desc = req_desc.get_line_desc();

         `dut_error_mdesc(get_abstract_name(), req_desc, $sformatf("scratchpad address 0x%x should be a hit (%0s)", (line_desc.get_paddr() & const_line_paddr_mask), req_desc.sprint_obj()), pk_sc_sb_error_intensity, { VMOD_SC_SB_WARN, m_verbosity_mod })
      end
   endfunction : process_error_code


   virtual function void final_axi_req_fixup(input evl_req_desc axi_req_desc, input evl_abstract_bus_cmd_t abstract_bus_cmd);
      evl_sc_bank_id_t bank_id;
      evl_dword_t esr_axi_qos;
      evl_shire_axi_ifc_type_t axi_ifc_type;
      evl_paddr_t req_addr;
      bank_id = axi_req_desc.get_bus_bank();
      esr_axi_qos = m_regs[bank_id].sc_reqq_ctl.ESR_SC_AXI_QOS.get();
      req_addr = axi_req_desc.get_paddr();
     
      `evl_log_mdesc(UVM_HIGH, VMOD_SC_SB, get_abstract_name(), axi_req_desc, $sformatf("final_axi_req_fixup: get_bus_qos = 0x%x, m_remote_enable = 0x%x, m_remote_l3_enable =0x%x, get_bus_req_user = 0x%x esr_axi_qos = %x",axi_req_desc.get_bus_qos(),m_remote_enable,m_remote_l3_enable,axi_req_desc.get_bus_req_user(),esr_axi_qos))
     
      //document: "If the request comes from the l2 and the esr is AXI_QOS_MEM_HIGH_PRIORITY, then the output qos to the mesh will be AXI_QOS_MEM_HIGH_PRIORITY, else AXI_QOS_MEM_LOW_PRIORITY"
      if (axi_req_desc.get_bus_qos() == 0) begin // If we haven't set it to something (ex: ATOMIC_RSP) already:
         if (esr_axi_qos == `AXI_QOS_MEM_HIGH_PRIORITY) begin
            axi_req_desc.set_bus_qos(`AXI_QOS_MEM_HIGH_PRIORITY);
         end
         else begin
            axi_req_desc.set_bus_qos(`AXI_QOS_MEM_LOW_PRIORITY);
         end
      end
      
      //What port to we expect this to show up on ?
      if (abstract_bus_cmd == BusWriteAtomic) begin
         axi_ifc_type = SHIRE_L3TOSYS_MESH_AXI;
      end
      // ScpFill is affected by remote_l3_enable=1 if the request is to L3 space (not scratchpad space)
      else if ((m_remote_l3_enable == 0) && (axi_req_desc.get_int_param(pk_etlink_cmd, `EVL_Bus_Idle) == int'(ET_LINK_REQ_ScpFill)) && (req_addr[39:31] != 9'h1)) begin
         axi_ifc_type = SHIRE_L3TOSYS_MESH_AXI;
      end
      // scratchpad requests still go to TOL3 even if remote_l3_enable=0
      else begin
         axi_ifc_type = SHIRE_L2TOL3_MESH_AXI;
      end
      axi_req_desc.set_int_param(pk_sc_downstream_port,int'(axi_ifc_type));
   endfunction: final_axi_req_fixup


   //-----------------------------------------------------------------------------------------------
   //
   // Function: fix_qualifier_at_tc_req
   //
   virtual function evl_sc_qual_t fix_qualifier_at_tc_req(input evl_sc_qual_t qualifier_in, input evl_req_desc req_desc);
      evl_sc_qual_t qualifier_out;
      qualifier_out = qualifier_in;
      
      if ((req_desc.get_bus_type() == EVL_BUS_TYPE_AXI)) begin
         qualifier_out = qualifier_out | `SCQ_FromMesh | `SCQ_CheckSize;
      end
      else begin
         qualifier_out = qualifier_out | `SCQ_CheckSize;
      end
      
      return qualifier_out;
   endfunction : fix_qualifier_at_tc_req

   //-----------------------------------------------------------------------------------------------
   //
   // Function: fix_qualifier_at_reqq_alloc
   //
   virtual function evl_sc_qual_t fix_qualifier_at_reqq_alloc(input evl_sc_qual_t qualifier_in, input evl_req_desc req_desc);
      evl_sc_qual_t qualifier_out;
      
      if ((req_desc.get_bus_type() == EVL_BUS_TYPE_AXI)) begin
         qualifier_out = qualifier_in | `SCQ_FromMesh | `SCQ_CheckSize;
      end
      else begin
         qualifier_out = qualifier_in | `SCQ_CheckSize;
      end

  
      return qualifier_out;
   endfunction : fix_qualifier_at_reqq_alloc


   //-----------------------------------------------------------------------------------------------
   function void build_phase(uvm_phase phase);
      super.build_phase(phase);

      m_cache = new("m_cache", this);
      m_cache.set_abstract_name($sformatf("%s(Scp$)", get_root_abstract_name()));
      m_cache.set_cache_type("Scp$");
      m_cache.set_verbosity_mods(VMOD_SC_SB, VMOD_SCP_SB_WARN);
      m_cache.enable_coverage();


      //============================================================================================
      //============================================================================================
      //
      // Lookups Performed at the Read Buffer
      //
      //============================================================================================
      //============================================================================================

      //
      // ET Link Scratchpad Reads
      //                                                    Obs   Model                                                  Bus
      //               ET-link Opcode        Scp Opcode    State  State           Qual          Tag Action  Data Action  Command   Rsp Type   Err Code
      //               --------------------  ------------  -----  --------------  ------------  ----------  -----------  --------  ---------  --------
      add_rbuf_req_act(ET_LINK_REQ_Read,     SC_Scp_Read,  E,     { St_Ignore },  `SCQ_None,     St_Ignore,  Data_MemRd,  BusIdle,  RspWData,  EC_None  );
      add_rbuf_req_act(ET_LINK_REQ_Read,     SC_Scp_Read,  E,     { St_Ignore },  `SCQ_FromMesh, St_Ignore,  Data_MemRd,  BusIdle,  RspWData,  EC_None  );
      add_rbuf_req_act(ET_LINK_REQ_ReadCoop, SC_Scp_Read,  E,     { St_Ignore },  `SCQ_None,     St_Ignore,  Data_MemRd,  BusIdle,  RspWData,  EC_None  );
      //add_rbuf_req_act(ET_LINK_REQ_ReadCoop, SC_Scp_Read,  E,     { St_Ignore },  `SCQ_FromMesh, St_Ignore,  Data_MemRd,  BusIdle,  RspWData,  EC_None  ); no readcoop from mesh

      //============================================================================================
      //============================================================================================
      //
      // Lookups Performed at the TC Request Stage
      //
      //============================================================================================
      //============================================================================================

      // It looks like we only ever see observed state "E" for SCP ops

      //
      // ET Link Scratchpad Reads
      //                                                  Obs   Model                                                  Bus
      //             ET-link Opcode        Scp Opcode    State  State           Qual          Tag Action  Data Action  Command   Rsp Type   Err Code
      //             --------------------  ------------  -----  --------------  ------------  ----------  -----------  --------  ---------  --------
      add_tc_req_act(ET_LINK_REQ_Read,     SC_Scp_Read,  E,     { St_Ignore },  `SCQ_None,    St_Ignore,  Data_MemRd,  BusIdle,  RspWData,  EC_None  );
      add_tc_req_act(ET_LINK_REQ_Read,     SC_Scp_Read,  E,     { St_Ignore },  `SCQ_FromMesh,St_Ignore,  Data_MemRd,  BusIdle,  RspWData,  EC_None  );
      add_tc_req_act(ET_LINK_REQ_ReadCoop, SC_Scp_Read,  E,     { St_Ignore },  `SCQ_None,    St_Ignore,  Data_MemRd,  BusIdle,  RspWData,  EC_None  );
      //add_tc_req_act(ET_LINK_REQ_ReadCoop, SC_Scp_Read,  E,     { St_Ignore },  `SCQ_FromMesh,St_Ignore,  Data_MemRd,  BusIdle,  RspWData,  EC_None  ); No readcoop from mesh

      // No Scp:
      add_tc_req_act(ET_LINK_REQ_Read,     SC_Scp_Read,  I,     { St_Ignore },  (`SCQ_NoRegion),               St_Ignore,  Data_ZeroRsp,  BusIdle,  RspWData,  EC_MaybeErrRsp  );
      add_tc_req_act(ET_LINK_REQ_Read,     SC_Scp_Read,  I,     { St_Ignore },  (`SCQ_FromMesh|`SCQ_NoRegion), St_Ignore,  Data_ZeroRsp,  BusIdle,  RspWData,  EC_MaybeErrRsp  );
      add_tc_req_act(ET_LINK_REQ_ReadCoop, SC_Scp_Read,  I,     { St_Ignore },  (`SCQ_NoRegion),               St_Ignore,  Data_ZeroRsp,  BusIdle,  RspWData,  EC_MaybeErrRsp  );
      // Out of bounds scp read:
      add_tc_req_act(ET_LINK_REQ_Read,     SC_Scp_Read,  I,     { St_Ignore },  (`SCQ_DecErr),                 St_Ignore,  Data_ZeroRsp,  BusIdle,  RspWData,  EC_MaybeErrRsp  );
      add_tc_req_act(ET_LINK_REQ_Read,     SC_Scp_Read,  I,     { St_Ignore },  (`SCQ_FromMesh|`SCQ_DecErr),           St_Ignore,  Data_ZeroRsp,  BusIdle,  RspWData,  EC_MaybeErrRsp  );
      //add_tc_req_act(ET_LINK_REQ_ReadCoop, SC_Scp_Read,  I,     { St_Ignore },  (`SCQ_FromMesh|`SCQ_NoRegion), St_Ignore,  Data_ZeroRsp,  BusIdle,  RspWData,  EC_None  ); No readcoop from mesh

      //
      // ET Link Scratchpad Atomics
      //                                                   Obs   Model                                                        Bus
      //             ET-link Opcode       Scp Opcode      State  State           Qual          Tag Action  Data Action        Command          Rsp Type   Err Code
      //             -------------------  --------------  -----  --------------  ------------  ----------  -----------------  ---------------  ---------  --------
      //add_tc_req_act(ET_LINK_REQ_Atomic,  SC_Scp_Atomic,  E,     { St_Ignore },  `SCQ_None,    St_Ignore,  Data_MemWrRdNorm,  BusIdle,         RspWData,  EC_None  ); No atomic from etlink

      // This is an atomic that came from the AXI interface.
      //Since it can't be responded to in the write response, it sends out a new write to a magic address.
      add_tc_req_act(ET_LINK_REQ_Atomic,  SC_Scp_Atomic,  E,     { St_Ignore },  `SCQ_FromMesh, Stale,      Data_ReqWr,    BusWriteAtomic,  RspAck,    EC_None  );

      // No Scp:
      //add_tc_req_act(ET_LINK_REQ_Atomic,  SC_Scp_Atomic,  I,     { St_Ignore },  (`SCQ_NoRegion),             St_Ignore,  Data_FwdWr,  BusWriteAtomic,  RspAck,  EC_None  ); No atomic from etlink
      // This Atomic came in from the AXI interface. from the shire cache doc:
      // "If the L3 region (or SCP region) does not exist, the Acknowledge response as already been sent and the error can't be sent with the AXI ESR write because
      // we don't yet support a WRite Error opcode. A normal atomic ESR write occurs with uncomputed results and we rely on the error being reported as an interrupt."
      add_tc_req_act(ET_LINK_REQ_Atomic,  SC_Scp_Atomic,  I,     { St_Ignore },  (`SCQ_FromMesh|`SCQ_NoRegion), St_Ignore,  Data_FwdWr,  BusWriteAtomic,  RspAck,  EC_None );

      //
      // ET Link Scratchpad Writes
      //                                                      Obs   Model                                                    Bus
      //             ET-link Opcode      Scp Opcode          State  State           Qual            Tag Action  Data Action  Command   Rsp Type  Err Code
      //             ------------------  -------------       -----  --------------  --------------  ----------  -----------  --------  --------  ---------
      add_tc_req_act(ET_LINK_REQ_Write,  SC_Scp_Write,        E,    { St_Ignore },  `SCQ_None,      St_Ignore,  Data_MemWr,  BusIdle,  RspAck,   EC_None   );
      add_tc_req_act(ET_LINK_REQ_Write,  SC_Scp_Write,        E,    { St_Ignore },  `SCQ_FromMesh,  St_Ignore,  Data_MemWr,  BusIdle,  RspAck,   EC_None   );
      add_tc_req_act(ET_LINK_REQ_Write,  SC_Scp_WritePartial, E,    { St_Ignore },  `SCQ_None,      St_Ignore,  Data_MemWr,  BusIdle,  RspAck,   EC_None   );
      add_tc_req_act(ET_LINK_REQ_Write,  SC_Scp_WritePartial, E,    { St_Ignore },  `SCQ_FromMesh,  St_Ignore,  Data_MemWr,  BusIdle,  RspAck,   EC_None   );

      // No Scp:
      add_tc_req_act(ET_LINK_REQ_Write,  SC_Scp_Write,        I,     { St_Ignore },  `SCQ_NoRegion,  St_Ignore,  Data_None,  BusIdle,  RspAck,   EC_MaybeErrRsp  );
      add_tc_req_act(ET_LINK_REQ_Write,  SC_Scp_Write,        I,     { St_Ignore },  (`SCQ_FromMesh|`SCQ_NoRegion),  St_Ignore,  Data_None,  BusIdle,  RspAck,   EC_MaybeErrRsp  );
      add_tc_req_act(ET_LINK_REQ_Write,  SC_Scp_WritePartial, I,     { St_Ignore },  `SCQ_NoRegion,  St_Ignore,  Data_None,  BusIdle,  RspAck,   EC_MaybeErrRsp  );
      add_tc_req_act(ET_LINK_REQ_Write,  SC_Scp_WritePartial, I,     { St_Ignore },  (`SCQ_FromMesh|`SCQ_NoRegion),  St_Ignore,  Data_None,  BusIdle,  RspAck,   EC_MaybeErrRsp  );
      // This is out of bounds:
      add_tc_req_act(ET_LINK_REQ_Write,  SC_Scp_Write,  I,     { St_Ignore },  (`SCQ_DecErr),               St_Ignore,  Data_None,  BusIdle,  RspAck,   EC_MaybeErrRsp );
      add_tc_req_act(ET_LINK_REQ_Write,  SC_Scp_Write,  I,     { St_Ignore },  (`SCQ_FromMesh|`SCQ_DecErr),  St_Ignore,  Data_None,  BusIdle,  RspAck,   EC_MaybeErrRsp );
      //add_tc_req_act(ET_LINK_REQ_Write,  SC_Scp_Write,  I,     { St_Ignore },  `SCQ_FromMesh,    St_Ignore,  Data_None,  BusIdle,  RspAck,   EC_Error01 ); Stimulus error if we hit uninitialized scp
      //add_tc_req_act(ET_LINK_REQ_Write,  SC_Scp_WritePartial,  I,     { St_Ignore },  `SCQ_None,      St_Ignore,  Data_None,  BusIdle,  RspAck,   EC_Error01 ); Stimulus error if we hit uninitialized scp
      //add_tc_req_act(ET_LINK_REQ_Write,  SC_Scp_WritePartial,  I,     { St_Ignore },  `SCQ_FromMesh,  St_Ignore,  Data_None,  BusIdle,  RspAck,   EC_Error01 ); Stimulus error if we hit uninitialized scp
      
      //
      // ET Link Remote Scratchpad Operations
      //                                                           Obs   Model                                                  Bus
      //               ET-link Opcode        Scp Opcode           State  State           Qual          Tag Action  Data Action  Command    Rsp Type  Err Code
      //               --------------------  --------------       -----  --------------  ------------  ----------  -----------  ---------  --------  --------
      add_reqq_req_act(ET_LINK_REQ_Read,     SC_Scp_Read,         I,     { St_Ignore },  `SCQ_Remote,  St_Ignore,  Data_None,   BusRead,   RspNull,  EC_None  );
      add_reqq_req_act(ET_LINK_REQ_ReadCoop, SC_Scp_Read,         I,     { St_Ignore },  `SCQ_Remote,  St_Ignore,  Data_None,   BusRead,   RspNull,  EC_None  );
      add_reqq_req_act(ET_LINK_REQ_Write,    SC_Scp_Write,        I,     { St_Ignore },  `SCQ_Remote,  St_Ignore,  Data_FwdWr,  BusWrite,  RspNull,  EC_None  );
      add_reqq_req_act(ET_LINK_REQ_Write,    SC_Scp_WritePartial, I,     { St_Ignore },  `SCQ_Remote,  St_Ignore,  Data_FwdWr,  BusWrite,  RspNull,  EC_None  );
      // Error: Atomics go through the uc block
      add_reqq_req_act(ET_LINK_REQ_Atomic,   SC_Scp_Atomic,       I,     { St_Ignore },  `SCQ_Remote,  St_Ignore,  Data_None,   BusIdle,   RspAck,  EC_MaybeErrRsp  ); 
      // ScpFill to a remote scratchpad address is an error
      add_reqq_req_act(ET_LINK_REQ_ScpFill,  SC_Scp_Fill,         I,     { St_Ignore },  `SCQ_Remote,  St_Ignore,  Data_None,   BusIdle,   RspAck,  EC_MaybeErrRsp  );
      add_reqq_req_act(ET_LINK_REQ_Prefetch, SC_L2_Prefetch,      I,     { St_Ignore },  `SCQ_Remote,  St_Ignore,  Data_None,   BusIdle,   RspAck,  EC_MaybeErrRsp  );
      add_reqq_req_act(ET_LINK_REQ_Flush,    SC_L2_Flush,         I,     { St_Ignore },  `SCQ_Remote,  St_Ignore,  Data_None,   BusIdle,   RspAck,  EC_MaybeErrRsp  );

      // ET Link Remote Scratchpad Operations when remote scratchpad is disabled (esr_sc_remote_scp_enable = 0)
      add_reqq_req_act(ET_LINK_REQ_Read,     SC_Scp_Read,         I,     { St_Ignore },  (`SCQ_Remote|`SCQ_DownDis),  St_Ignore,  Data_ZeroRsp,  BusIdle,  RspWData,  EC_MaybeErrRsp  );
      add_reqq_req_act(ET_LINK_REQ_ReadCoop, SC_Scp_Read,         I,     { St_Ignore },  (`SCQ_Remote|`SCQ_DownDis),  St_Ignore,  Data_ZeroRsp,  BusIdle,  RspWData,  EC_MaybeErrRsp  );
      add_reqq_req_act(ET_LINK_REQ_Write,    SC_Scp_Write,        I,     { St_Ignore },  (`SCQ_Remote|`SCQ_DownDis),  St_Ignore,  Data_None,  BusIdle,  RspAck,  EC_MaybeErrRsp  );
      add_reqq_req_act(ET_LINK_REQ_Write,    SC_Scp_WritePartial, I,     { St_Ignore },  (`SCQ_Remote|`SCQ_DownDis),  St_Ignore,  Data_None,  BusIdle,  RspAck,  EC_MaybeErrRsp  );
      add_reqq_req_act(ET_LINK_REQ_ScpFill,  SC_Scp_Fill,         I,     { St_Ignore },  (`SCQ_Remote|`SCQ_DownDis),  St_Ignore,  Data_None,  BusIdle,  RspAck,  EC_MaybeErrRsp  );
      add_reqq_req_act(ET_LINK_REQ_Atomic,   SC_Scp_Atomic,       I,     { St_Ignore },  (`SCQ_Remote|`SCQ_DownDis),  St_Ignore,  Data_None,  BusIdle,  RspAck,  EC_MaybeErrRsp  );

      // This is an error case. Local Scratchpad Evict. (Remote scratchpad evicts are taken care of in L2 scoreboard)
      add_reqq_req_act(ET_LINK_REQ_Evict,    SC_L2_Evict,    I, { St_Ignore }, `SCQ_L3_Mem,                St_Ignore, Data_None, BusIdle, RspAck, EC_MaybeErrRsp );
      add_reqq_req_act(ET_LINK_REQ_Evict,    SC_L2_Evict,    I, { St_Ignore }, (`SCQ_L3_Mem|`SCQ_DownDis), St_Ignore, Data_None, BusIdle, RspAck, EC_MaybeErrRsp );
      add_reqq_req_act(ET_LINK_REQ_Flush,    SC_L2_Flush,    I, { St_Ignore }, `SCQ_None,                  St_Ignore, Data_None, BusIdle, RspAck, EC_MaybeErrRsp );
      add_reqq_req_act(ET_LINK_REQ_Evict,    SC_L2_Evict,    I, { St_Ignore }, `SCQ_None,                  St_Ignore, Data_None, BusIdle, RspAck, EC_MaybeErrRsp );
      add_reqq_req_act(ET_LINK_REQ_Lock,     SC_L2_Lock,     I, { St_Ignore }, `SCQ_None,                  St_Ignore, Data_None, BusIdle, RspAck, EC_MaybeErrRsp );
      add_reqq_req_act(ET_LINK_REQ_Unlock,   SC_L2_Unlock,   I, { St_Ignore }, `SCQ_None,                  St_Ignore, Data_None, BusIdle, RspAck, EC_MaybeErrRsp );
      add_reqq_req_act(ET_LINK_REQ_Prefetch, SC_L2_Prefetch, I, { St_Ignore }, `SCQ_None,                  St_Ignore, Data_None, BusIdle, RspAck, EC_MaybeErrRsp );
      add_reqq_req_act(ET_LINK_REQ_Atomic,   SC_L2_Atomic,   I, { St_Ignore }, `SCQ_None,                  St_Ignore, Data_None, BusIdle, RspAck, EC_MaybeErrRsp );
      // Atomic from the mesh is expected and will go down the pipe, so just predict the ack:
      add_reqq_req_act(ET_LINK_REQ_Atomic,   SC_L2_Atomic,   I, { St_Ignore }, `SCQ_FromMesh,              St_Ignore, Data_None, BusIdle, RspAck, EC_None );
      // Local Scratchpad Writearound. Error case
      add_reqq_req_act(ET_LINK_REQ_WriteAround, SC_L2_WriteAround, I, { St_Ignore }, `SCQ_None, St_Ignore, Data_None, BusIdle, RspAck, EC_MaybeErrRsp );
      // Now allowed from the mesh:
      add_reqq_req_act(ET_LINK_REQ_Flush,    SC_L2_Flush,         I,     { St_Ignore },  `SCQ_FromMesh,                St_Ignore, Data_None, BusIdle, RspAck, EC_MaybeErrRsp );
      add_reqq_req_act(ET_LINK_REQ_Evict,    SC_L2_Evict,         I,     { St_Ignore },  `SCQ_FromMesh,                St_Ignore,  Data_None,   BusIdle,  RspAck,  EC_MaybeErrRsp  );
      add_reqq_req_act(ET_LINK_REQ_Prefetch, SC_L2_Prefetch,      I,     { St_Ignore },  `SCQ_FromMesh,                St_Ignore,  Data_None,   BusIdle,  RspAck,  EC_MaybeErrRsp  );
      // Remote stuff from mesh not allowed
      add_reqq_req_act(ET_LINK_REQ_Read,     SC_Scp_Read,         I,     { St_Ignore },  (`SCQ_Remote|`SCQ_FromMesh),  St_Ignore,  Data_ZeroRsp,   BusIdle,   RspWData,  EC_MaybeErrRsp  );
      add_reqq_req_act(ET_LINK_REQ_Write,    SC_Scp_Write,        I,     { St_Ignore },  (`SCQ_Remote|`SCQ_FromMesh),  St_Ignore,  Data_None,   BusIdle,   RspAck,  EC_MaybeErrRsp  );
      add_reqq_req_act(ET_LINK_REQ_Write,    SC_Scp_WritePartial, I,     { St_Ignore },  (`SCQ_Remote|`SCQ_FromMesh),  St_Ignore,  Data_None,   BusIdle,   RspAck,  EC_MaybeErrRsp  );
      add_reqq_req_act(ET_LINK_REQ_Atomic,   SC_Scp_Atomic,       I,     { St_Ignore },  (`SCQ_Remote|`SCQ_FromMesh),  St_Ignore,  Data_None,   BusIdle,  RspAck,  EC_MaybeErrRsp  );
      add_reqq_req_act(ET_LINK_REQ_Flush,    SC_L2_Flush,         I,     { St_Ignore },  (`SCQ_Remote|`SCQ_FromMesh),  St_Ignore,  Data_None,   BusIdle,  RspAck,  EC_MaybeErrRsp  );
      add_reqq_req_act(ET_LINK_REQ_Evict,    SC_L2_Evict,         I,     { St_Ignore },  (`SCQ_Remote|`SCQ_FromMesh),  St_Ignore,  Data_None,   BusIdle,  RspAck,  EC_MaybeErrRsp  );
      add_reqq_req_act(ET_LINK_REQ_Prefetch, SC_L2_Prefetch,      I,     { St_Ignore },  (`SCQ_Remote|`SCQ_FromMesh),  St_Ignore,  Data_None,   BusIdle,  RspAck,  EC_MaybeErrRsp  );
      
      //
      // ET Link Scratchpad Fills
      //
      //  The first entry (Obs State "I") is for the lookup at reqq stage so the axi request can be issued
      //  The second entry (Obs State "E") is for the lookup at tc stage to actually perform the fill
      //
      //                                                     Obs   Model                                                         Bus
      //               ET-link Opcode        Scp Opcode     State  State           Qual             Tag Action  Data Action      Command    Rsp Type  Err Code
      //               --------------------  -------------  -----  --------------  ---------------  ----------  ---------------  ---------  --------  --------
      add_reqq_req_act(ET_LINK_REQ_ScpFill,  SC_Scp_Fill,   I,     { St_Ignore },  `SCQ_None,       St_Ignore,  Data_None,       BusRead,   RspNull,  EC_None  );
      // this decode error won't really have any effect until the fill happens:
      //CLC July2019 Actually we won't set the DecErr in REQQ.
      //add_reqq_req_act(ET_LINK_REQ_ScpFill,  SC_Scp_Fill,   I,     { St_Ignore },  `SCQ_DecErr,     St_Ignore,  Data_None,       BusRead,   RspNull,  EC_None  );

      // The fill itself came from the mesh
      add_tc_req_act  (ET_LINK_REQ_ScpFill,  SC_Scp_Fill,   E,     { St_Ignore },  (`SCQ_UseSrcAddr|`SCQ_FromMesh), St_Ignore,  Data_CacheFill,  BusIdle,   RspAck,  EC_None  );
      // Scratchpad out of range error:
      add_tc_req_act  (ET_LINK_REQ_ScpFill,  SC_Scp_Fill,   I,     { St_Ignore },  (`SCQ_UseSrcAddr|`SCQ_FromMesh|`SCQ_DecErr), St_Ignore,  Data_CacheFill,  BusIdle,   RspAck,  EC_MaybeErrRsp  );

      // This is an error case for when the Scratchpad does not exist.
      add_tc_req_act  (ET_LINK_REQ_ScpFill,  SC_Scp_Fill,   I,     { St_Ignore },  `SCQ_NoRegion,   St_Ignore,  Data_CacheFill,  BusIdle,   RspAck,  EC_MaybeErrRsp  );
      add_tc_req_act  (ET_LINK_REQ_ScpFill,  SC_Scp_Fill,   I,     { St_Ignore }, (`SCQ_UseSrcAddr | `SCQ_NoRegion), St_Ignore,  Data_CacheFill,  BusIdle,   RspAck,  EC_MaybeErrRsp  );
      add_tc_req_act  (ET_LINK_REQ_ScpFill,  SC_Scp_Fill,   I,     { St_Ignore }, (`SCQ_UseSrcAddr|`SCQ_NoRegion|`SCQ_FromMesh), St_Ignore,  Data_CacheFill,  BusIdle,   RspAck,  EC_MaybeErrRsp  );
      
      //
      // Index Cacheops
      //                                                      Obs   Model                                                    Bus
      //             ET-link Opcode        L2 Opcode        State  State         Qual           Tag Action  Data Action      Command     Rsp Type  Err Code
      //             -------------------  ---------------  -----  -----------   -------------  ----------  ---------------  ----------  --------  ---------------
      add_tc_req_act(ET_LINK_REQ_Write,    SC_Scp_Zero,     E,    { St_Ignore }, `SCQ_None,     St_Ignore,  Data_ZeroFill,   BusIdle,    RspAck,    EC_None        );
      
      //============================================================================================
      //============================================================================================
      //
      // Lookups Performed for AXI Bus Responses
      //
      //============================================================================================
      //============================================================================================

      //
      // Response to a SCP fill read
      //                                    AXI             Model                                                                            Bus
      //              ET-link Opcode        Command         State  AXI Response    Qual            Tag Action  Data Action        Rsp Type   Command   Err Code
      //              --------------------  --------------  -----  --------------  --------------  ----------  -----------------  ---------  --------  --------
      add_bus_rsp_act(ET_LINK_REQ_Read,     AXI_REQ_READ,   I,     AXI_RESP_OKAY,  `SCQ_None,      St_Ignore,  Data_CascadeRsp,   RspWData,  BusIdle,  EC_None  );
      add_bus_rsp_act(ET_LINK_REQ_ReadCoop, AXI_REQ_READ,   I,     AXI_RESP_OKAY,  `SCQ_None,      St_Ignore,  Data_CascadeRsp,   RspWData,  BusIdle,  EC_None  );
      add_bus_rsp_act(ET_LINK_REQ_Write,    AXI_REQ_WRITE,  I,     AXI_RESP_OKAY,  `SCQ_None,      St_Ignore,  Data_None,         RspAck,    BusIdle,  EC_None  );
      add_bus_rsp_act(ET_LINK_REQ_ScpFill,  AXI_REQ_READ,   I,     AXI_RESP_OKAY,  `SCQ_None,      St_Ignore,  Data_EnqueueFill,  RspNull,    BusIdle,  EC_None  );

      add_bus_rsp_act(ET_LINK_REQ_Read,     AXI_REQ_READ,   I,     AXI_RESP_OKAY,  `SCQ_NoRegion,  St_Ignore,  Data_CascadeRsp,   RspWData,  BusIdle,  EC_None  );
      add_bus_rsp_act(ET_LINK_REQ_ReadCoop, AXI_REQ_READ,   I,     AXI_RESP_OKAY,  `SCQ_NoRegion,  St_Ignore,  Data_CascadeRsp,   RspWData,  BusIdle,  EC_None  );
      add_bus_rsp_act(ET_LINK_REQ_Write,    AXI_REQ_WRITE,  I,     AXI_RESP_OKAY,  `SCQ_NoRegion,  St_Ignore,  Data_None,         RspAck,    BusIdle,  EC_None  );
      add_bus_rsp_act(ET_LINK_REQ_ScpFill,  AXI_REQ_READ,   I,     AXI_RESP_OKAY,  `SCQ_NoRegion,  St_Ignore,  Data_EnqueueFill,  RspNull,    BusIdle,  EC_None  );

      //
      // Response to an SCP remote atomic
      //                                   AXI             Model                                                                    Bus
      //              ET-link Opcode       Command         State   AXI Response    Qual         Tag Action  Data Action  Rsp Type   Command   Err Code
      //              -------------------  --------------  ------  --------------  -----------  ----------  -----------  ---------  --------  --------
      add_bus_rsp_act(ET_LINK_REQ_Atomic,  AXI_REQ_WRITE,  Stale,  AXI_RESP_OKAY,  `SCQ_Stale,  St_Ignore,  Data_None,   RspNull,   BusIdle,  EC_None  );

      // No Scp:
      add_bus_rsp_act(ET_LINK_REQ_Atomic,  AXI_REQ_WRITE,  I,      AXI_RESP_OKAY,  `SCQ_NoRegion, St_Ignore,  Data_None,   RspNull,   BusIdle,  EC_None  );
      m_cache.build_coverage();
   endfunction : build_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function void start_of_simulation_phase(uvm_phase phase);
      super.start_of_simulation_phase(phase);

      if (m_cache != null) begin
         m_cache.set_abstract_name($sformatf("%s(Scp$)", get_root_abstract_name()));
      end
   endfunction : start_of_simulation_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string name_in = "evl_scp_scoreboard", input uvm_component parent_in = null);
      super.new(name_in, parent_in);
      set_abstract_name("SHIRE_CACHE(SCPSB)");
      m_sb_type                  = int'(SCP_SCOREBOARD);
      m_mask_with_base_and_limit = 1'b1;
      m_verbosity_mod            = VMOD_SCP_SB_WARN;
   endfunction : new

endclass : evl_scp_scoreboard

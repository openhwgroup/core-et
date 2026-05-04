//--------------------------------------------------------------------------------------------------
//
// Class: evl_l3_scoreboard
//
// This l3_scoreboard extends the shire cache scoreboard and defines a table which
// describes the actions the shire cache scoreboard should take to model
// and l3.
//
class evl_l3_scoreboard extends evl_shire_cache_scoreboard #(evl_l3_scoreboard);

   `uvm_component_utils(evl_l3_scoreboard)


   //-----------------------------------------------------------------------------------------------
   //
   // Static Member Variables and Objects
   //
   `evl_param_key(pk_l3_index_msb)
   `evl_param_key(pk_l3_set_size)


   bit m_no_cover;
   
   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_index_mask
   //
   virtual function evl_paddr_t get_index_mask();
      evl_paddr_t addr;

      addr = m_global_attrs.convert_to_l3_address(0, 0, ((1 << `SC_L3_SHIRE_ID_SIZE) - 1));
      return (((evl_paddr_t'(1) << (m_index_msb + `SC_L3_SHIRE_ID_SIZE)) - 1) & (~addr) & (~evl_paddr_t'(`EVL_LINE_BYTES - 1)));
   endfunction : get_index_mask


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_index_alias_mask
   //
   virtual function evl_paddr_t get_index_alias_mask();
      evl_paddr_t addr;

      return evl_paddr_t'(1) << (m_index_msb + `SC_L3_SHIRE_ID_SIZE - 1);
   endfunction : get_index_alias_mask


   //-----------------------------------------------------------------------------------------------
   //
   // Function: process_sc_bank_config
   //
   virtual function void process_sc_bank_config(input evl_sc_bank_config_t rtl_info);
      int my_index_msb;

      super.process_sc_bank_config(rtl_info);
      m_global_attrs.set_l3_config(get_shire_id(), m_bypass, m_remote_enable, m_mem_size, m_set_base_addr, m_set_base_addr, m_stim_index_mask, m_alias_mask);
      m_agent.set_int_param(pk_l3_set_size, m_set_size);
      m_agent.set_int_param(pk_l3_index_msb, m_index_msb);
   endfunction : process_sc_bank_config


   //-----------------------------------------------------------------------------------------------
   //
   // Function: final_axi_req_fixup
   //
   virtual function void final_axi_req_fixup(input evl_req_desc axi_req_desc, input evl_abstract_bus_cmd_t abstract_bus_cmd);
      evl_req_desc src_req_desc;
      evl_sc_reqq_opcode_t opcode;
      evl_dword_t esr_axi_qos;
      evl_sc_bank_id_t bank_id;
      evl_shire_axi_ifc_type_t axi_ifc_type;
      // from spec: "If the request comes from the L3 and the incoming qos from the L3 slave is AXI_QOS_MEM_HIGH_PRIORITY, then the output qos to the next level mesh will be AXI_QOS_MEM_HIGH_PRIORITY, else AXI_QOS_MEM_LOW_PRIORITY"
      // But also: "Index cacheops get their QOS from the esr"
      src_req_desc = axi_req_desc.get_parent_req_desc();
      opcode = evl_sc_reqq_opcode_t'(src_req_desc.get_int_param(pk_l2_opcode));
      bank_id = axi_req_desc.get_bus_bank();
      esr_axi_qos = m_regs[bank_id].sc_reqq_ctl.ESR_SC_AXI_QOS.get();
      
      `evl_log_mdesc(UVM_HIGH, VMOD_SC_SB, get_abstract_name(), axi_req_desc, $sformatf("final_axi_req_fixup: get_bus_qos = 0x%x src_req_desc qos = %x opcode = %0p",axi_req_desc.get_bus_qos(),src_req_desc.get_bus_qos(),opcode))

      if (opcode inside { m_index_cacheop_opcodes }) begin
         if (esr_axi_qos == `AXI_QOS_MEM_HIGH_PRIORITY) begin
            axi_req_desc.set_bus_qos(`AXI_QOS_MEM_HIGH_PRIORITY);
         end
         else begin
            axi_req_desc.set_bus_qos(`AXI_QOS_MEM_LOW_PRIORITY);
         end
      end
      else begin
         if (axi_req_desc.get_bus_qos() == 0) begin // If we haven't set it to something (ex: ATOMIC_RSP) already:
            if (src_req_desc.get_bus_qos() == `AXI_QOS_MEM_HIGH_PRIORITY) begin
               axi_req_desc.set_bus_qos(`AXI_QOS_MEM_HIGH_PRIORITY);
            end
            else begin
               axi_req_desc.set_bus_qos(`AXI_QOS_MEM_LOW_PRIORITY);
            end
         end
      end // else: !if(opcode inside { m_index_cacheop_opcodes })

      //What port to we expect this to show up on ?
      axi_ifc_type = SHIRE_L3TOSYS_MESH_AXI;
      axi_req_desc.set_int_param(pk_sc_downstream_port,int'(axi_ifc_type));
   endfunction: final_axi_req_fixup

   
   //-----------------------------------------------------------------------------------------------
   //
   // Function: fix_qualifier_at_tc_req
   //
   virtual function evl_sc_qual_t fix_qualifier_at_tc_req(input evl_sc_qual_t qualifier_in, input evl_req_desc req_desc);
      evl_sc_qual_t qualifier_out;
      evl_paddr_t addr;
      evl_shire_id_t addr_shire_id;
      
      qualifier_out = qualifier_in;

      //If we already have a NoRegion case, don't bother, but otherwise see if this is a bad shire id:
      // Also don't do this if the address is an index address
      if (!(m_cache.has_sub_qualifier(qualifier_out,`SCQ_NoRegion)) && !(m_cache.has_sub_qualifier(qualifier_out,`SCQ_IdxAddr))) begin
         if (m_all_shire_aliasing == 1'b1) begin
            //All shire ids are good
         end
         else begin
            evl_shire_id_t alternate_shire_id;
            if (m_two_shire_aliasing_use_lsb == 1'b1) begin
               alternate_shire_id = get_shire_id();
               alternate_shire_id[0] = ~alternate_shire_id[0];
            end
            else begin
               alternate_shire_id = get_shire_id();
               alternate_shire_id[`SC_L3_SHIRE_ID_SIZE-1] = ~alternate_shire_id[`SC_L3_SHIRE_ID_SIZE-1];
            end
            addr = req_desc.get_paddr();
            addr_shire_id = m_global_attrs.get_l3_shire_from_address(addr);
            if (!(addr_shire_id inside {get_shire_id(),alternate_shire_id})) begin
               qualifier_out = qualifier_out | `SCQ_DecErr;
            end
         end
      end // if (!`SCQ_NoRegion)
      
      return qualifier_out;
   endfunction : fix_qualifier_at_tc_req
   
   
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
         `dut_error(get_abstract_name(), $sformatf("TB_ERROR: L3 scoreboard not expecting an index cacheop set outside of the base and top. set = %x base = %x top = %x",set,m_set_base_addr,set_top))
      end

      
      addr_out = m_global_attrs.convert_to_l3_address(bank,subbank,shire,set);
      `evl_log(UVM_HIGH,VMOD_SC_SB, get_abstract_name(), $sformatf("converting index cacheop address %x to %x before table lookup",cacheop_address,addr_out))
      return addr_out;
      
     
   endfunction : convert_index_cacheop_address

   //-----------------------------------------------------------------------------------------------
   function void build_phase(uvm_phase phase);
      super.build_phase(phase);

      m_cache = new("m_cache", this);
      m_cache.set_abstract_name($sformatf("%s(L3$)", get_root_abstract_name()));
      m_cache.set_cache_type("L3$");
      m_cache.set_verbosity_mods(VMOD_SC_SB, VMOD_L3_SB_WARN);
      m_cache.enable_coverage();

      m_merge_partial_opcodes.push_back(SC_L3_FlushWData);

      m_check_partial_opcodes.push_back(SC_L3_Read);
      m_check_partial_opcodes.push_back(SC_L3_Atomic);
      m_check_partial_opcodes.push_back(SC_L3_Prefetch);
      m_check_partial_opcodes.push_back(SC_L3_Flush);
      m_check_partial_opcodes.push_back(SC_Idx_L3_Flush);
      m_check_partial_opcodes.push_back(SC_L3_WritePartial);

      m_index_cacheop_opcodes.push_back(SC_Idx_L3_Inv);
      m_index_cacheop_opcodes.push_back(SC_Idx_L3_Flush);
      m_index_cacheop_opcodes.push_back(SC_Idx_L3_Evict);

      m_no_cover = 1'b1;

      //============================================================================================
      //============================================================================================
      //
      // Lookups Performed at the REQQ Stage
      //
      //============================================================================================
      //============================================================================================
      // If the L3 is in Bypass requests will get processed in REQQ
      add_reqq_req_act(ET_LINK_REQ_Write,       SC_Reqq_Initial,    I,    { St_Ignore },  `SCQ_Bypass,  Stale,    Data_FwdWr,       BusWrite,       RspNull,   EC_None        );
      add_reqq_req_act(ET_LINK_REQ_Read,        SC_Reqq_Initial,    I,    { St_Ignore },  `SCQ_Bypass,  Stale,    Data_None,        BusRead,        RspNull,   EC_None        );
      add_reqq_req_act(ET_LINK_REQ_Evict,       SC_Reqq_Initial,    I,    { St_Ignore },  `SCQ_Bypass,  Stale,    Data_None,        BusIdle,       RspAck,   EC_None        );
      add_reqq_req_act(ET_LINK_REQ_Flush,       SC_Reqq_Initial,    I,    { St_Ignore },  `SCQ_Bypass,  Stale,    Data_None,        BusIdle,       RspAck,   EC_None        );
      add_reqq_req_act(ET_LINK_REQ_EvictToMem,  SC_Reqq_Initial,    I,    { St_Ignore },  `SCQ_Bypass,  Stale,    Data_FwdWr,       BusWrite,      RspNull,  EC_None        );
      add_reqq_req_act(ET_LINK_REQ_FlushToMem,  SC_Reqq_Initial,    I,    { St_Ignore },  `SCQ_Bypass,  Stale,    Data_FwdWr,       BusWrite,      RspNull,  EC_None        );
      add_reqq_req_act(ET_LINK_REQ_Prefetch,  SC_Reqq_Initial,    I,    { St_Ignore },  (`SCQ_Bypass | `SCQ_Mem),     Stale,    Data_None,  BusIdle,  RspAck,  EC_None );

      // We need to send back the AXI response right away for the Atomic:
      add_reqq_req_act(ET_LINK_REQ_Atomic,    SC_L3_Atomic,     I,     { St_Ignore },  `SCQ_None,    St_Ignore,  Data_None,  BusIdle,  RspAck,  EC_None );
      // When L3 is in Bypass atomic is a nop with an error response
      add_reqq_req_act(ET_LINK_REQ_Atomic,    SC_Reqq_Initial,  I,     { St_Ignore },  `SCQ_Bypass,  St_Ignore,  Data_None,  BusIdle,  RspAck,  EC_MaybeErrRsp );
      
      //============================================================================================
      //============================================================================================
      //
      // Lookups Performed at the TC Request Stage
      //
      //============================================================================================
      //============================================================================================

      //
      // Reads (note that all AXI lookups use the request descriptor's AWUSER bits for an ET link command)
      //                                              Obs   Model                                                    Bus
      //             ET-link Opcode     L3 Opcode    State  State        Qual           Tag Action  Data Action      Command     Rsp Type   Err Code
      //             -----------------  -----------  -----  -----------  -------------  ----------  ---------------  ----------  ---------  ---------
      add_tc_req_act(ET_LINK_REQ_Read,  SC_L3_Read,  I,     { I },       `SCQ_None,     IE_Bsy,     Data_None,       BusLineRd,    RspNull,   EC_None   );
      add_tc_req_act(ET_LINK_REQ_Read,  SC_L3_Read,  I,     { A1_I_Bsy },  `SCQ_None,     A1_IE_Bsy,  Data_None,       BusLineRd,    RspNull,   EC_None   );
      add_tc_req_act(ET_LINK_REQ_Read,  SC_L3_Read,  E,     { E },       `SCQ_None,     St_Ignore,  Data_MemRd,      BusIdle,    RspWData,  EC_None   );
      add_tc_req_act(ET_LINK_REQ_Read,  SC_L3_Read,  E,     { A1_E_Bsy },`SCQ_None,     St_Ignore,  Data_MemRd,      BusIdle,    RspWData,  EC_None, m_no_cover );
      add_tc_req_act(ET_LINK_REQ_Read,  SC_L3_Read,  E,     { M },       `SCQ_None,     St_Ignore,  Data_MemRd,      BusIdle,    RspWData,  EC_None   );
      add_tc_req_act(ET_LINK_REQ_Read,  SC_L3_Read,  E,     { M },       `SCQ_Partial,  WIE_Bsy,   Data_ReqWrBack,  BusVictim,  RspNull,   EC_Victim_ClrQW );

      add_tc_req_act(ET_LINK_REQ_Read,  SC_L3_Fill,  I,     { IE_Bsy },  `SCQ_None,     E,          Data_CacheFill,  BusIdle,    RspNull,   EC_None   );
      add_tc_req_act(ET_LINK_REQ_Read,  SC_L3_Fill,  I,     { A1_IE_Bsy },`SCQ_None,    A1_E_Bsy,   Data_CacheFill,  BusIdle,    RspNull,   EC_None, m_no_cover );

      // No L3:
      add_tc_req_act(ET_LINK_REQ_Read,  SC_L3_Read,  I,     { I },       `SCQ_NoRegion, St_Ignore,  Data_ZeroRsp,    BusIdle,    RspWData,  EC_MaybeErrRsp );
      // Bad Shire id:
      add_tc_req_act(ET_LINK_REQ_Read,  SC_L3_Read,  I,     { I },       `SCQ_DecErr,   St_Ignore,  Data_ZeroRsp,    BusIdle,    RspWData,  EC_MaybeErrRsp );

      //
      // Writes
      //                                                Obs   Model                                                  Bus
      //             ET-link Opcode      L2 Opcode     State  State         Qual           Tag Action  Data Action      Command   Rsp Type  Err Code
      //             ------------------  ------------  -----  -----------   ----------     ----------  ---------------  --------  --------  --------
      add_tc_req_act(ET_LINK_REQ_Write,  SC_L3_Write,  I,     { I },        `SCQ_None,     M,          Data_MemWrInst,  BusIdle,  RspAck,   EC_None );
      add_tc_req_act(ET_LINK_REQ_Write,  SC_L3_Write,  E,     { E },        `SCQ_None,     M,          Data_MemWr,      BusIdle,  RspAck,   EC_SetQW );
      add_tc_req_act(ET_LINK_REQ_Write,  SC_L3_Write,  E,     { M },        `SCQ_None,     St_Ignore,  Data_MemWr,      BusIdle,  RspAck,   EC_None );

      // Writes to Evicted Lines
      add_tc_req_act(ET_LINK_REQ_Write,  SC_L3_Write,  I,     { A1_I_Bsy }, `SCQ_None,     A1_M_Bsy,   Data_MemWrInst,  BusIdle,  RspAck,   EC_None );
      add_tc_req_act(ET_LINK_REQ_Write,  SC_L3_Write,  E,     { A1_E_Bsy }, `SCQ_None,     A1_M_Bsy,   Data_MemWr,      BusIdle,  RspAck,   EC_SetQW, m_no_cover );
      add_tc_req_act(ET_LINK_REQ_Write,  SC_L3_Write,  E,     { A1_M_Bsy }, `SCQ_None,     St_Ignore,  Data_MemWr,      BusIdle,  RspAck,   EC_SetQW, m_no_cover );

      // No L3 region:
      add_tc_req_act(ET_LINK_REQ_Write,  SC_L3_Write,  I,     { I },        `SCQ_NoRegion, St_Ignore,  Data_None,       BusIdle,  RspAck,   EC_MaybeErrRsp );
      // Bad Shire ID:
      add_tc_req_act(ET_LINK_REQ_Write,  SC_L3_Write,  I,     { I },        `SCQ_DecErr,   St_Ignore,  Data_None,       BusIdle,  RspAck,   EC_MaybeErrRsp );

      //
      // WritePartial
      //                                                      Obs   Model                                                      Bus
      //             ET-link Opcode      L2 Opcode           State  State         Qual           Tag Action  Data Action      Command   Rsp Type  Err Code
      //             ------------------  ------------        -----  -----------   ----------     ----------  ---------------  --------  --------  --------
      add_tc_req_act(ET_LINK_REQ_Write,  SC_L3_WritePartial,  I,     { I },       `SCQ_None,     IEM_Bsy,    Data_None,       BusLineRd,  RspAck,   EC_None );
      add_tc_req_act(ET_LINK_REQ_Write,  SC_L3_WritePartial,  I,     { A1_I_Bsy },`SCQ_None,     A1_IEM_Bsy, Data_None,       BusLineRd,  RspAck,   EC_None );
      add_tc_req_act(ET_LINK_REQ_Write,  SC_L3_WritePartial,  E,     { E },       `SCQ_None,     M,          Data_MemWr,      BusIdle,  RspAck,   EC_SetQW );
      add_tc_req_act(ET_LINK_REQ_Write,  SC_L3_WritePartial,  E,     { A1_E_Bsy },`SCQ_None,     A1_M_Bsy,   Data_MemWr,      BusIdle,  RspAck,   EC_SetQW, m_no_cover );
      add_tc_req_act(ET_LINK_REQ_Write,  SC_L3_WritePartial,  E,     { M },       `SCQ_None,     St_Ignore,  Data_MemWr,      BusIdle,  RspAck,   EC_None  );
      add_tc_req_act(ET_LINK_REQ_Write,  SC_L3_WritePartial,  E,     { A1_M_Bsy },`SCQ_None,     St_Ignore,  Data_MemWr,      BusIdle,  RspAck,   EC_None, m_no_cover  );
      add_tc_req_act(ET_LINK_REQ_Write,  SC_L3_WritePartial,  E,     { M },       `SCQ_Partial,  WEM_Bsy,    Data_ReqWrBack,  BusVictim, RspAck,   EC_Victim_ClrQW  );
      add_tc_req_act(ET_LINK_REQ_Write,  SC_L3_WritePartial,  E,     { EM_Bsy },  `SCQ_None,     M,          Data_MemWr,      BusIdle,  RspAck,   EC_SetQW );

      // No L3 region:
      add_tc_req_act(ET_LINK_REQ_Write,  SC_L3_WritePartial,  I,     { I },        `SCQ_NoRegion, St_Ignore,  Data_None,       BusIdle,  RspAck,   EC_MaybeErrRsp );
      // Bad Shire id:
      add_tc_req_act(ET_LINK_REQ_Write,  SC_L3_WritePartial,  I,     { I },        `SCQ_DecErr,   St_Ignore,  Data_None,       BusIdle,  RspAck,   EC_MaybeErrRsp );

      // WritePartial sent out a read and here's the fill
      add_tc_req_act(ET_LINK_REQ_Write,  SC_L3_Fill,          I,     { IEM_Bsy }, `SCQ_None,     EM_Bsy,     Data_CacheFill,  BusIdle, RspAck, EC_None );
      
      //
      // Evict
      //                                                          Obs   Model                                                Bus
      //             ET-link Opcode           L2 Opcode          State  State       Qual           Tag Action  Data Action      Command     Rsp Type  Err Code
      //             -----------------------  -----------------  -----  ----------  ----------     ----------  ---------------  ----------  --------  ---------
      add_tc_req_act(ET_LINK_REQ_Evict,       SC_L3_Evict,       I,    { I },       `SCQ_None,     St_Ignore,  Data_None,       BusIdle,    RspAck,   EC_None   );
      add_tc_req_act(ET_LINK_REQ_Evict,       SC_L3_Evict,       I,    { A1_I_Bsy },  `SCQ_None,     St_Ignore,  Data_None,       BusIdle,    RspAck,   EC_None   );
      add_tc_req_act(ET_LINK_REQ_Evict,       SC_L3_Evict,       E,    { E },       `SCQ_None,     I,          Data_None,       BusIdle,    RspAck,   EC_Victim );
      add_tc_req_act(ET_LINK_REQ_Evict,       SC_L3_Evict,       E,    { A1_E_Bsy },`SCQ_None,     A1_I_Bsy,   Data_None,       BusIdle,    RspAck,   EC_Victim, m_no_cover );
      add_tc_req_act(ET_LINK_REQ_Evict,       SC_L3_Evict,       E,    { M },       `SCQ_None,     MI_Bsy,     Data_ReqWrBack,  BusVictim,  RspAck,   EC_Victim );

      // No L3:
      add_tc_req_act(ET_LINK_REQ_Evict,       SC_L3_Evict,       I,    { I },       `SCQ_NoRegion, St_Ignore,  Data_None,       BusIdle,    RspAck,  EC_MaybeErrRsp );
      // Bad shire id:
      add_tc_req_act(ET_LINK_REQ_Evict,       SC_L3_Evict,       I,    { I },       `SCQ_DecErr,   St_Ignore,  Data_None,       BusIdle,    RspAck,  EC_MaybeErrRsp );

      // The EvictToMem comes with Data so that counts as having dirty data
      add_tc_req_act(ET_LINK_REQ_EvictToMem,  SC_L3_EvictWData,  I,    { I },       `SCQ_None,     A1_I_Bsy,      Data_FwdWr,      BusWrite,   RspNull, EC_None   );
      add_tc_req_act(ET_LINK_REQ_EvictToMem,  SC_L3_EvictWData,  I,    { A1_I_Bsy },`SCQ_None,     A2_I_Bsy,   Data_FwdWr,      BusWrite,   RspNull,  EC_None   );
      add_tc_req_act(ET_LINK_REQ_EvictToMem,  SC_L3_EvictWData,  E,    { E },       `SCQ_None,     EI_Bsy,     Data_MrgWrBack,  BusVictim,  RspAck,  EC_Victim );
      add_tc_req_act(ET_LINK_REQ_EvictToMem,  SC_L3_EvictWData,  E,    { A1_E_Bsy },`SCQ_None,     A1_EI_Bsy,  Data_MrgWrBack,  BusVictim,  RspAck,  EC_Victim, m_no_cover );
      add_tc_req_act(ET_LINK_REQ_EvictToMem,  SC_L3_EvictWData,  E,    { M },       `SCQ_None,     MI_Bsy,     Data_MrgWrBack,  BusVictim,  RspAck,  EC_Victim );
      add_tc_req_act(ET_LINK_REQ_EvictToMem,  SC_L3_EvictWData,  E,    { A1_M_Bsy },`SCQ_None,     A1_MI_Bsy,  Data_MrgWrBack,  BusVictim,  RspAck,  EC_Victim, m_no_cover );

      // No L3:
      add_tc_req_act(ET_LINK_REQ_EvictToMem,  SC_L3_EvictWData,  I,    { I },       `SCQ_NoRegion, St_Ignore,  Data_None,       BusIdle,    RspAck,  EC_MaybeErrRsp );


      //
      // Flush
      //                                                          Obs   Model                                                    Bus
      //             ET-link Opcode           L2 Opcode          State  State        Qual           Tag Action  Data Action      Command     Rsp Type  Err Code
      //             -----------------------  -----------------  -----  -----------  -------------  ----------  ---------------  ----------  --------  ---------------
      add_tc_req_act(ET_LINK_REQ_Flush,       SC_L3_Flush,       I,     { I },       `SCQ_None,     St_Ignore,  Data_None,       BusIdle,    RspAck,  EC_None         );
      add_tc_req_act(ET_LINK_REQ_Flush,       SC_L3_Flush,       I,     { A1_I_Bsy },  `SCQ_None,     St_Ignore,  Data_None,       BusIdle,    RspAck,  EC_None         );
      add_tc_req_act(ET_LINK_REQ_Flush,       SC_L3_Flush,       E,     { E },       `SCQ_None,     St_Ignore,  Data_None,       BusIdle,    RspAck,  EC_None         );
      add_tc_req_act(ET_LINK_REQ_Flush,       SC_L3_Flush,       E,     { A1_E_Bsy },`SCQ_None,     St_Ignore,  Data_None,       BusIdle,    RspAck,  EC_None,  m_no_cover );
      add_tc_req_act(ET_LINK_REQ_Flush,       SC_L3_Flush,       E,     { M },       `SCQ_None,     ME_Bsy,     Data_ReqWrBack,  BusVictim,  RspAck,  EC_Victim       );
      add_tc_req_act(ET_LINK_REQ_Flush,       SC_L3_Flush,       E,     { M },       `SCQ_Partial,  MI_Bsy,     Data_ReqWrBack,  BusVictim,  RspAck,  EC_Victim       );

      // No L3:
      add_tc_req_act(ET_LINK_REQ_Flush,       SC_L3_Flush,       I,     { I },       `SCQ_NoRegion, St_Ignore,  Data_None,       BusIdle,    RspAck,  EC_MaybeErrRsp );
      // bad shire id:
      add_tc_req_act(ET_LINK_REQ_Flush,       SC_L3_Flush,       I,     { I },       `SCQ_DecErr,   St_Ignore,  Data_None,       BusIdle,    RspAck,  EC_MaybeErrRsp );

      add_tc_req_act(ET_LINK_REQ_FlushToMem,  SC_L3_FlushWData,  I,     { I },       `SCQ_None,     A1_I_Bsy,      Data_FwdWr,      BusWrite,   RspNull,  EC_None         );
      add_tc_req_act(ET_LINK_REQ_FlushToMem,  SC_L3_FlushWData,  I,     { I },       `SCQ_Partial,  A1_I_Bsy,      Data_FwdWr,      BusWrite,   RspNull,  EC_None         );
      add_tc_req_act(ET_LINK_REQ_FlushToMem,  SC_L3_FlushWData,  I,     { A1_I_Bsy },  `SCQ_None,     A2_I_Bsy,   Data_FwdWr,      BusWrite,   RspNull,  EC_None         );
      add_tc_req_act(ET_LINK_REQ_FlushToMem,  SC_L3_FlushWData,  I,     { A1_I_Bsy },  `SCQ_Partial,  A2_I_Bsy,   Data_FwdWr,      BusWrite,   RspNull,  EC_None         );
      // if a flushtomem hits a clean line it just forwards the data
      add_tc_req_act(ET_LINK_REQ_FlushToMem,  SC_L3_FlushWData,  E,     { E },       `SCQ_None,     E_Bsy,      Data_MrgWrBack,  BusVictim,  RspAck,   EC_Victim       );
      add_tc_req_act(ET_LINK_REQ_FlushToMem,  SC_L3_FlushWData,  E,     { A1_E_Bsy },`SCQ_None,     A1_E_Bsy,   Data_MrgWrBack,  BusVictim,  RspAck,   EC_Victim, m_no_cover );
      add_tc_req_act(ET_LINK_REQ_FlushToMem,  SC_L3_FlushWData,  E,     { E },       `SCQ_Partial,  ME_Bsy,     Data_MrgWrBack,  BusVictim,  RspAck,   EC_Victim_ClrQW );
      // if a flushtomem hits a modified line and it's full after the merge then flush it downstream and keep the line.
      add_tc_req_act(ET_LINK_REQ_FlushToMem,  SC_L3_FlushWData,  E,     { M },       `SCQ_None,     ME_Bsy,     Data_MrgWrBack,  BusVictim,  RspAck,   EC_Victim_ClrQW );
      // if a flushtomem hits a modified line and it's partial after the merge the don't keep a copy (evict it)
      add_tc_req_act(ET_LINK_REQ_FlushToMem,  SC_L3_FlushWData,  E,     { M },       `SCQ_Partial,  MI_Bsy,     Data_MrgWrBack,  BusVictim,  RspAck,   EC_Victim       );


      // No L3:
      add_tc_req_act(ET_LINK_REQ_FlushToMem,  SC_L3_FlushWData,  I,     { I },   `SCQ_NoRegion,                  St_Ignore,  Data_None,  BusIdle,   RspAck,  EC_MaybeErrRsp );
      add_tc_req_act(ET_LINK_REQ_FlushToMem,  SC_L3_FlushWData,  I,     { I },   (`SCQ_NoRegion | `SCQ_Partial), St_Ignore,  Data_None,  BusIdle,   RspAck,  EC_MaybeErrRsp );


      //
      // Prefetch
      //                                                      Obs   Model                                                    Bus
      //             ET-link Opcode         L2 Opcode        State  State         Qual           Tag Action  Data Action      Command     Rsp Type  Err Code
      //             ---------------------  ---------------  -----  -----------   ----------     ----------  ---------------  ----------  --------  ---------------
      add_tc_req_act(ET_LINK_REQ_Prefetch,  SC_L3_Prefetch,  I,     { I },        `SCQ_None,     IE_Bsy,     Data_None,       BusLineRd,    RspNull,  EC_None         );
      add_tc_req_act(ET_LINK_REQ_Prefetch,  SC_L3_Prefetch,  I,     { A1_I_Bsy }, `SCQ_None,     A1_IE_Bsy,  Data_None,       BusLineRd,    RspNull,  EC_None         );
      add_tc_req_act(ET_LINK_REQ_Prefetch,  SC_L3_Prefetch,  E,     { E },        `SCQ_None,     St_Ignore,  Data_None,       BusIdle,    RspAck,   EC_None         );
      add_tc_req_act(ET_LINK_REQ_Prefetch,  SC_L3_Prefetch,  E,     { A1_E_Bsy }, `SCQ_None,     St_Ignore,  Data_None,       BusIdle,    RspAck,   EC_None, m_no_cover );
      add_tc_req_act(ET_LINK_REQ_Prefetch,  SC_L3_Prefetch,  E,     { M },        `SCQ_None,     St_Ignore,  Data_None,       BusIdle,    RspAck,   EC_None         );
      add_tc_req_act(ET_LINK_REQ_Prefetch,  SC_L3_Prefetch,  E,     { M },        `SCQ_Partial,  WIE_Bsy,    Data_ReqWrBack,  BusVictim,  RspAck,   EC_Victim_ClrQW );

      add_tc_req_act(ET_LINK_REQ_Prefetch,  SC_L3_Fill,      I,     { IE_Bsy },   `SCQ_None,     E,          Data_CacheFill,  BusIdle,    RspAck,  EC_None         );
      add_tc_req_act(ET_LINK_REQ_Prefetch,  SC_L3_Fill,      I,     { A1_IE_Bsy },`SCQ_None,     A1_E_Bsy,   Data_CacheFill,  BusIdle,    RspAck,  EC_None, m_no_cover  );

      // No L3:
      add_tc_req_act(ET_LINK_REQ_Prefetch,  SC_L3_Prefetch,  I,     { I },        `SCQ_NoRegion, St_Ignore,  Data_None,       BusIdle,    RspAck,  EC_MaybeErrRsp );
      // Bad shire id:
      add_tc_req_act(ET_LINK_REQ_Prefetch,  SC_L3_Prefetch,  I,     { I },        `SCQ_DecErr,   St_Ignore,  Data_None,       BusIdle,    RspAck,  EC_MaybeErrRsp );

      //
      // Atomics
      //                                                  Obs   Model                                                       Bus
      //             ET-link Opcode       L2 Opcode      State  State       Qual              Tag Action     Data Action      Command          Rsp Type   Err Code
      //             -------------------  -------------  -----  ----------  ----------------  ----------     ---------------  ---------------  ---------  ---------------
      add_tc_req_act(ET_LINK_REQ_Atomic,  SC_L3_Atomic,  I,    { I },       `SCQ_None,        IE_Bsy,        Data_None,       BusLineRd,         RspAck,    EC_None         );
      add_tc_req_act(ET_LINK_REQ_Atomic,  SC_L3_Atomic,  I,    { A1_I_Bsy },`SCQ_None,        A1_IE_Bsy,     Data_None,       BusLineRd,         RspAck,    EC_None         );
      add_tc_req_act(ET_LINK_REQ_Atomic,  SC_L3_Atomic,  E,    { E },       `SCQ_None,        Stale_M,       Data_ReqWr,      BusWriteAtomic,  RspAck,    EC_SetQW        );
      add_tc_req_act(ET_LINK_REQ_Atomic,  SC_L3_Atomic,  E,    { A1_E_Bsy },`SCQ_None,        A1_Stale_M_Bsy,Data_ReqWr,      BusWriteAtomic,  RspAck,    EC_SetQW, m_no_cover);
      add_tc_req_act(ET_LINK_REQ_Atomic,  SC_L3_Atomic,  E,    { E },       `SCQ_SecondPass,  Stale_M,       Data_ReqWr,      BusWriteAtomic,  RspNull,   EC_SetQW        );
      add_tc_req_act(ET_LINK_REQ_Atomic,  SC_L3_Atomic,  E,    { A1_E_Bsy },`SCQ_SecondPass,  A1_Stale_M_Bsy,Data_ReqWr,      BusWriteAtomic,  RspNull,   EC_SetQW, m_no_cover  );
      add_tc_req_act(ET_LINK_REQ_Atomic,  SC_L3_Atomic,  E,    { M },       `SCQ_None,        Stale_M,       Data_ReqWr,      BusWriteAtomic,  RspAck,    EC_SetQW        );
      add_tc_req_act(ET_LINK_REQ_Atomic,  SC_L3_Atomic,  E,    { M },       `SCQ_Partial,     WIE_Bsy,       Data_ReqWrBack,  BusVictim,       RspAck,    EC_Victim_ClrQW );
      add_tc_req_act(ET_LINK_REQ_Atomic,  SC_L3_Atomic,  E,    { M },       `SCQ_SecondPass,  Stale_M,       Data_ReqWr,      BusWriteAtomic,  RspNull,   EC_SetQW        );

      add_tc_req_act(ET_LINK_REQ_Atomic,  SC_L3_Fill,    I,    { IE_Bsy },  `SCQ_None,        E,             Data_CacheFill,  BusIdle,         RspNull,   EC_None         );
      add_tc_req_act(ET_LINK_REQ_Atomic,  SC_L3_Fill,    I,    { A1_IE_Bsy },`SCQ_None,       A1_E_Bsy,      Data_CacheFill,  BusIdle,         RspNull,   EC_None, m_no_cover  );
      
      // No L3:

      add_tc_req_act(ET_LINK_REQ_Atomic,  SC_L3_Atomic,  I,    { I },       `SCQ_NoRegion,    St_Ignore,     Data_FwdWr,      BusWriteAtomic,  RspAck,    EC_None );
      // Bad shire id:
      add_tc_req_act(ET_LINK_REQ_Atomic,  SC_L3_Atomic,  I,    { I },       `SCQ_DecErr,      Stale,         Data_FwdWr,      BusWriteAtomic,  RspAck,    EC_None );

      //
      // Index Cache Ops
      //                                                  Obs   Model                                                       Bus
      //             ET-link Opcode       L2 Opcode      State  State        Qual              Tag Action   Data Action      Command          Rsp Type   Err Code
      //             -------------------  -------------  -----  ----------   ----------------  ----------   ---------------  ---------------  ---------  ---------------
      add_tc_req_act(ET_LINK_REQ_Write,  SC_Idx_L3_Evict, I,    { I },       `SCQ_None,        I,           Data_None,       BusIdle,         RspAck,    EC_None        );
      add_tc_req_act(ET_LINK_REQ_Write,  SC_Idx_L3_Evict, I,    { A1_I_Bsy },`SCQ_None,        St_Ignore,   Data_None,       BusIdle,         RspAck,    EC_None, m_no_cover );
      add_tc_req_act(ET_LINK_REQ_Write,  SC_Idx_L3_Evict, E,    { E },       `SCQ_None,        I,           Data_None,       BusIdle,         RspAck,    EC_Victim_ClrQW );
      add_tc_req_act(ET_LINK_REQ_Write,  SC_Idx_L3_Evict, E,    { M },       `SCQ_None,        MI_Bsy,      Data_ReqWrBack,  BusVictim,       RspAck,    EC_Victim_ClrQW );

      add_tc_req_act(ET_LINK_REQ_Write,  SC_Idx_L3_Flush, I,    { I },       `SCQ_None,        St_Ignore,   Data_None,       BusIdle,         RspAck,    EC_None        );
      add_tc_req_act(ET_LINK_REQ_Write,  SC_Idx_L3_Flush, I,    { A1_I_Bsy },`SCQ_None,        St_Ignore,   Data_None,       BusIdle,         RspAck,    EC_None, m_no_cover  );
      add_tc_req_act(ET_LINK_REQ_Write,  SC_Idx_L3_Flush, E,    { E },       `SCQ_None,        St_Ignore,   Data_None,       BusIdle,         RspAck,    EC_None        );
      add_tc_req_act(ET_LINK_REQ_Write,  SC_Idx_L3_Flush, E,    { M },       `SCQ_None,        ME_Bsy,      Data_ReqWrBack,  BusVictim,       RspAck,    EC_Victim      );
      add_tc_req_act(ET_LINK_REQ_Write,  SC_Idx_L3_Flush, E,    { M },       `SCQ_Partial,     MI_Bsy,      Data_ReqWrBack,  BusVictim,       RspAck,    EC_Victim      );

      add_tc_req_act(ET_LINK_REQ_Write,  SC_Idx_L3_Inv,   I,    { I },       `SCQ_None,        I,           Data_None,       BusIdle,         RspAck,    EC_None        );
      add_tc_req_act(ET_LINK_REQ_Write,  SC_Idx_L3_Inv,   E,    { E },       `SCQ_None,        I,           Data_None,       BusIdle,         RspAck,    EC_None        );
      add_tc_req_act(ET_LINK_REQ_Write,  SC_Idx_L3_Inv,   E,    { M },       `SCQ_None,        I,           Data_None,       BusIdle,         RspAck,    EC_None        );

      // index cachops are sent even in Bypass
      add_tc_req_act(ET_LINK_REQ_Write,  SC_Idx_L3_Flush, I,    { I },       `SCQ_Bypass,      I,           Data_None,       BusIdle,         RspAck,    EC_None        );
      add_tc_req_act(ET_LINK_REQ_Write,  SC_Idx_L3_Evict, I,    { I },       `SCQ_Bypass,      I,           Data_None,       BusIdle,         RspAck,    EC_None        );
      add_tc_req_act(ET_LINK_REQ_Write,  SC_Idx_L3_Inv,   I,    { I },       `SCQ_Bypass,      I,           Data_None,       BusIdle,         RspAck,    EC_None        );

      //============================================================================================
      //============================================================================================
      //
      // Lookups Performed at the TC Tag Response Stage
      //
      //============================================================================================
      //============================================================================================

      //
      // Capacity Evictions
      //                                                       Obs   Model                                                     Bus
      //              ET-link Opcode         L2 Opcode        State  State          Qual          Tag Action  Data Action      Command    Rsp Type  Err Code
      //              ---------------------  ---------------  -----  -------------  ------------  ----------  ---------------  ---------  --------  -----------
      add_tag_rsp_act(ET_LINK_REQ_Read,      SC_L3_Fill,      M,     { M },         `SCQ_Victim,  A1_I_Bsy,  Data_ReqWrBack,  BusWrite,  RspNull,  EC_None     );
      add_tag_rsp_act(ET_LINK_REQ_Read,      SC_L3_Fill,      M,     { A1_M_Bsy },  `SCQ_Victim,  A2_I_Bsy,  Data_ReqWrBack,  BusWrite,  RspNull,  EC_None     );

      add_tag_rsp_act(ET_LINK_REQ_Atomic,    SC_L3_Fill,      M,     { M },         `SCQ_Victim,  A1_I_Bsy,  Data_ReqWrBack,  BusWrite,  RspNull,  EC_None     );
      add_tag_rsp_act(ET_LINK_REQ_Atomic,    SC_L3_Fill,      M,     { A1_M_Bsy },  `SCQ_Victim,  A2_I_Bsy,  Data_ReqWrBack,  BusWrite,  RspNull,  EC_None     );

      add_tag_rsp_act(ET_LINK_REQ_Write,     SC_L3_Write,     M,     { M },         `SCQ_Victim,  A1_I_Bsy,  Data_ReqWrBack,  BusWrite,  RspNull,  EC_None     );
      add_tag_rsp_act(ET_LINK_REQ_Write,     SC_L3_Write,     M,     { A1_M_Bsy },  `SCQ_Victim,  A2_I_Bsy,  Data_ReqWrBack,  BusWrite,  RspNull,  EC_None     );


      // Forced Partial Eviction due to a read of a partial line
      add_tag_rsp_act(ET_LINK_REQ_Read,      SC_L3_Read,      M,     { WIE_Bsy },  `SCQ_Victim,  A1_IE_Bsy,  Data_ReqWrBack,  BusWrite,  RspNull,  EC_LateRead );
      add_tag_rsp_act(ET_LINK_REQ_Atomic,    SC_L3_Atomic,    M,     { WIE_Bsy },  `SCQ_Victim,  A1_IE_Bsy,  Data_ReqWrBack,  BusWrite,  RspNull,  EC_LateRead );
      add_tag_rsp_act(ET_LINK_REQ_Prefetch,  SC_L3_Prefetch,  M,     { WIE_Bsy },  `SCQ_Victim,  A1_IE_Bsy,  Data_ReqWrBack,  BusWrite,  RspNull,  EC_LateRead );

      // Also will do a partial eviction due to a writePartial of a partial line
      add_tag_rsp_act(ET_LINK_REQ_Write,     SC_L3_WritePartial, M,     { WEM_Bsy },  `SCQ_Victim,  A1_IEM_Bsy,  Data_ReqWrBack,  BusWrite,  RspNull,  EC_LateRead );

      //
      // Evict / Flush
      //                                                           Obs   Model                                                   Bus
      //              ET-link Opcode           L2 Opcode          State  State        Qual          Tag Action  Data Action      Command    Rsp Type  Err Code
      //              -----------------------  -----------------  -----  -----------  ------------  ----------  ---------------  ---------  --------  --------
      add_tag_rsp_act(ET_LINK_REQ_Evict,       SC_L3_Evict,       M,     { MI_Bsy },  `SCQ_Victim,  A1_I_Bsy,      Data_ReqWrBack,  BusWrite,  RspNull,  EC_None  );

      add_tag_rsp_act(ET_LINK_REQ_EvictToMem,  SC_L3_EvictWData,  M,     { EI_Bsy },  `SCQ_Victim,  A1_I_Bsy,      Data_FwdWr,      BusWrite,  RspNull,  EC_None  );
      add_tag_rsp_act(ET_LINK_REQ_EvictToMem,  SC_L3_EvictWData,  M,     { A1_EI_Bsy },`SCQ_Victim, A2_I_Bsy,      Data_FwdWr,      BusWrite,  RspNull,  EC_None  , m_no_cover);
      add_tag_rsp_act(ET_LINK_REQ_EvictToMem,  SC_L3_EvictWData,  M,     { MI_Bsy },  `SCQ_Victim,  A1_I_Bsy,      Data_ReqWrBack,  BusWrite,  RspNull,  EC_None  );
      add_tag_rsp_act(ET_LINK_REQ_EvictToMem,  SC_L3_EvictWData,  M,     { A1_MI_Bsy },`SCQ_Victim, A2_I_Bsy,      Data_ReqWrBack,  BusWrite,  RspNull,  EC_None  , m_no_cover);

      add_tag_rsp_act(ET_LINK_REQ_Flush,       SC_L3_Flush,       M,     { ME_Bsy },  `SCQ_Victim,  A1_E_Bsy,      Data_ReqWrBack,  BusWrite,  RspNull,  EC_None  );
      add_tag_rsp_act(ET_LINK_REQ_Flush,       SC_L3_Flush,       M,     { MI_Bsy },  `SCQ_Victim,  A1_I_Bsy,      Data_ReqWrBack,  BusWrite,  RspNull,  EC_None  );
      add_tag_rsp_act(ET_LINK_REQ_FlushToMem,  SC_L3_FlushWData,  M,     { ME_Bsy },  `SCQ_Victim,  A1_E_Bsy,      Data_ReqWrBack,  BusWrite,  RspNull,  EC_None  );

      add_tag_rsp_act(ET_LINK_REQ_FlushToMem,  SC_L3_FlushWData,  M,     { MI_Bsy },  `SCQ_Victim,  A1_I_Bsy,      Data_ReqWrBack,  BusWrite,  RspNull,  EC_None  );
      // If the FlushToMem hit clean data then it just forwards the incoming data to mem.
      add_tag_rsp_act(ET_LINK_REQ_FlushToMem,  SC_L3_FlushWData,  M,     { E_Bsy },   `SCQ_Victim,  A1_E_Bsy,      Data_FwdWr,      BusWrite,  RspNull,  EC_None  );
      add_tag_rsp_act(ET_LINK_REQ_FlushToMem,  SC_L3_FlushWData,  M,     { A1_E_Bsy },`SCQ_Victim,  A2_E_Bsy,      Data_FwdWr,      BusWrite,  RspNull,  EC_None, m_no_cover );
      
      //
      // Index Cache Ops
      //                                                    Obs     Model                                                       Bus
      //             ET-link Opcode       L2 Opcode         State   State       Qual              Tag Action   Data Action      Command          Rsp Type   Err Code
      //             -------------------  -------------     -----   ----------  ----------------  ----------   ---------------  ---------------  ---------  ------------
      add_tag_rsp_act(ET_LINK_REQ_Write,   SC_Idx_L3_Evict,  M,      {MI_Bsy},   `SCQ_Victim,      A1_I_Bsy,     Data_ReqWrBack,   BusWrite,       RspNull,   EC_None  );
      add_tag_rsp_act(ET_LINK_REQ_Write,   SC_Idx_L3_Flush,  M,      {ME_Bsy},   `SCQ_Victim,      A1_E_Bsy,     Data_ReqWrBack,   BusWrite,       RspNull,   EC_None  );
      // A flush might actually evict if the data was partial
      add_tag_rsp_act(ET_LINK_REQ_Write,   SC_Idx_L3_Flush,  M,      {MI_Bsy},   `SCQ_Victim,      A1_I_Bsy,     Data_ReqWrBack,   BusWrite,       RspNull,   EC_None  );

      //============================================================================================
      //============================================================================================
      //
      // Lookups Performed for AXI Bus Responses
      //
      //============================================================================================
      //============================================================================================

      //
      // Response to an AXI read, atomic or prefetch
      //                                     AXI            Model                                                                          Bus
      //              ET-link Opcode         Command        State      AXI Response    Qual          Tag Action  Data Action        Rsp Type   Command   Err Code
      //              ---------------------  -------------  -------    --------------  ----------    ----------  -----------------  ---------  --------  --------
      add_bus_rsp_act(ET_LINK_REQ_Read,      AXI_REQ_READ,  IE_Bsy,    AXI_RESP_OKAY,  `SCQ_None,    St_Ignore,  Data_CascadeFill,  RspWData,  BusIdle,  EC_None  );
      add_bus_rsp_act(ET_LINK_REQ_Read,      AXI_REQ_READ,  A1_IE_Bsy, AXI_RESP_OKAY,  `SCQ_None,    St_Ignore,  Data_CascadeFill,  RspWData,  BusIdle,  EC_None, m_no_cover );
      add_bus_rsp_act(ET_LINK_REQ_Atomic,    AXI_REQ_READ,  IE_Bsy,    AXI_RESP_OKAY,  `SCQ_None,    St_Ignore,  Data_EnqueueFill,  RspAck,    BusIdle,  EC_None  );
      add_bus_rsp_act(ET_LINK_REQ_Atomic,    AXI_REQ_READ,  A1_IE_Bsy, AXI_RESP_OKAY,  `SCQ_None,    St_Ignore,  Data_EnqueueFill,  RspAck,    BusIdle,  EC_None, m_no_cover );
      add_bus_rsp_act(ET_LINK_REQ_Prefetch,  AXI_REQ_READ,  IE_Bsy,    AXI_RESP_OKAY,  `SCQ_None,    St_Ignore,  Data_EnqueueFill,  RspAck,    BusIdle,  EC_None  );
      add_bus_rsp_act(ET_LINK_REQ_Prefetch,  AXI_REQ_READ,  A1_IE_Bsy, AXI_RESP_OKAY,  `SCQ_None,    St_Ignore,  Data_EnqueueFill,  RspAck,    BusIdle,  EC_None, m_no_cover );

      //
      // Response to a Capacity Eviction
      //                                     AXI             Model                                                                           Bus
      //              ET-link Opcode         Command         State       AXI Response    Qual            Tag Action  Data Action  Rsp Type   Command         Err Code
      //              ---------------------  --------------  ----------  --------------  --------------  ----------  -----------  ---------  --------------  --------
      add_bus_rsp_act(ET_LINK_REQ_Read,      AXI_REQ_WRITE,  A1_I_Bsy,     AXI_RESP_OKAY,  `SCQ_None,      I,          Data_None,   RspNull,   BusIdle,        EC_None  );
      add_bus_rsp_act(ET_LINK_REQ_Atomic,    AXI_REQ_WRITE,  A1_I_Bsy,     AXI_RESP_OKAY,  `SCQ_None,      I,          Data_None,   RspNull,   BusIdle,        EC_None  );
      add_bus_rsp_act(ET_LINK_REQ_Prefetch,  AXI_REQ_WRITE,  A1_I_Bsy,     AXI_RESP_OKAY,  `SCQ_None,      I,          Data_None,   RspNull,   BusIdle,        EC_None  );
      // capacity eviction was followed by a rd
      add_bus_rsp_act(ET_LINK_REQ_Read,      AXI_REQ_WRITE,  A1_IE_Bsy,  AXI_RESP_OKAY,  `SCQ_None,      IE_Bsy,     Data_None,   RspNull,   BusIdle,        EC_None  );
      add_bus_rsp_act(ET_LINK_REQ_Atomic,    AXI_REQ_WRITE,  A1_IE_Bsy,  AXI_RESP_OKAY,  `SCQ_None,      IE_Bsy,     Data_None,   RspNull,   BusIdle,        EC_None  );
      add_bus_rsp_act(ET_LINK_REQ_Prefetch,  AXI_REQ_WRITE,  A1_IE_Bsy,  AXI_RESP_OKAY,  `SCQ_None,      IE_Bsy,     Data_None,   RspNull,   BusIdle,        EC_None  );
      // capacity eviction was followed by a wr
      add_bus_rsp_act(ET_LINK_REQ_Read,      AXI_REQ_WRITE,  A1_M_Bsy,   AXI_RESP_OKAY,  `SCQ_None,      M,          Data_None,   RspNull,   BusIdle,        EC_None  );
      add_bus_rsp_act(ET_LINK_REQ_Atomic,    AXI_REQ_WRITE,  A1_M_Bsy,   AXI_RESP_OKAY,  `SCQ_None,      M,          Data_None,   RspNull,   BusIdle,        EC_None  );
      add_bus_rsp_act(ET_LINK_REQ_Prefetch,  AXI_REQ_WRITE,  A1_M_Bsy,   AXI_RESP_OKAY,  `SCQ_None,      M,          Data_None,   RspNull,   BusIdle,        EC_None  );
      // read of a partial caused an eviction (followed by a read)
      add_bus_rsp_act(ET_LINK_REQ_Read,      AXI_REQ_WRITE,  A1_IE_Bsy,  AXI_RESP_OKAY,  `SCQ_LateRead,  IE_Bsy,     Data_None,   RspNull,   BusReadParent,  EC_None  );
      add_bus_rsp_act(ET_LINK_REQ_Atomic,    AXI_REQ_WRITE,  A1_IE_Bsy,  AXI_RESP_OKAY,  `SCQ_LateRead,  IE_Bsy,     Data_None,   RspNull,   BusReadParent,  EC_None  );
      add_bus_rsp_act(ET_LINK_REQ_Prefetch,  AXI_REQ_WRITE,  A1_IE_Bsy,  AXI_RESP_OKAY,  `SCQ_LateRead,  IE_Bsy,     Data_None,   RspNull,   BusReadParent,  EC_None  );

      // capacity eviction was followed by a flushtomem
      add_bus_rsp_act(ET_LINK_REQ_Read,      AXI_REQ_WRITE,  A2_I_Bsy,   AXI_RESP_OKAY,  `SCQ_None,      A1_I_Bsy,      Data_None,   RspNull,   BusIdle,        EC_None  );
      add_bus_rsp_act(ET_LINK_REQ_Atomic,    AXI_REQ_WRITE,  A2_I_Bsy,   AXI_RESP_OKAY,  `SCQ_None,      A1_I_Bsy,      Data_None,   RspNull,   BusIdle,        EC_None  );
      add_bus_rsp_act(ET_LINK_REQ_Prefetch,  AXI_REQ_WRITE,  A2_I_Bsy,   AXI_RESP_OKAY,  `SCQ_None,      A1_I_Bsy,      Data_None,   RspNull,   BusIdle,        EC_None  );

      // No L3: An atomic still sends a write downstream
      add_bus_rsp_act(ET_LINK_REQ_Atomic,  AXI_REQ_WRITE,  I,      AXI_RESP_OKAY,  `SCQ_NoRegion, St_Ignore,  Data_None,   RspNull,   BusIdle,  EC_None  );
     

      // WritePartial causing more fun.
      add_bus_rsp_act(ET_LINK_REQ_Read,    AXI_REQ_WRITE,  A1_IEM_Bsy,   AXI_RESP_OKAY,  `SCQ_None,  IEM_Bsy,    Data_None,   RspNull,  BusIdle,  EC_None  );
      add_bus_rsp_act(ET_LINK_REQ_Prefetch,AXI_REQ_WRITE,  A1_IEM_Bsy,   AXI_RESP_OKAY,  `SCQ_None,  IEM_Bsy,    Data_None,   RspNull,  BusIdle,  EC_None  );
      
      //
      // Response to an AXI Write
      //                                  AXI             Model                                                                      Bus
      //              ET-link Opcode      Command         State       AXI Response    Qual        Tag Action  Data Action  Rsp Type  Command   Err Code
      //              ------------------  --------------  ----------  --------------  ----------  ----------  -----------  --------  --------  --------
      add_bus_rsp_act(ET_LINK_REQ_Write,  AXI_REQ_WRITE,  A1_I_Bsy,   AXI_RESP_OKAY,  `SCQ_None,  I,          Data_None,   RspNull,  BusIdle,  EC_None  );
      add_bus_rsp_act(ET_LINK_REQ_Write,  AXI_REQ_WRITE,  A2_I_Bsy,   AXI_RESP_OKAY,  `SCQ_None,  A1_I_Bsy,   Data_None,   RspNull,  BusIdle,  EC_None  );
      add_bus_rsp_act(ET_LINK_REQ_Write,  AXI_REQ_WRITE,  A2_E_Bsy,   AXI_RESP_OKAY,  `SCQ_None,  A1_E_Bsy,   Data_None,   RspNull,  BusIdle,  EC_None, m_no_cover );
      add_bus_rsp_act(ET_LINK_REQ_Write,  AXI_REQ_WRITE,  A1_M_Bsy,   AXI_RESP_OKAY,  `SCQ_None,  M,          Data_None,   RspNull,  BusIdle,  EC_None  );
      add_bus_rsp_act(ET_LINK_REQ_Write,  AXI_REQ_WRITE,  A1_IE_Bsy,  AXI_RESP_OKAY,  `SCQ_None,  IE_Bsy,     Data_None,   RspNull,  BusIdle,  EC_None  );

      // This was a writePartial over partial data (or an evict followed by writePartial
      add_bus_rsp_act(ET_LINK_REQ_Write,  AXI_REQ_WRITE,  A1_IEM_Bsy, AXI_RESP_OKAY, `SCQ_None,    IEM_Bsy,    Data_None,  RspAck,   BusIdle, EC_None ); 
      add_bus_rsp_act(ET_LINK_REQ_Write,  AXI_REQ_WRITE,  A1_IEM_Bsy, AXI_RESP_OKAY, `SCQ_LateRead,IEM_Bsy,    Data_None,  RspAck,   BusRead, EC_None ); 
      
      //
      // Response to a Flush or Evict
      //                                       AXI             Model                                                                   Bus
      //              ET-link Opcode           Command         State   AXI Response    Qual            Tag Action  Data Action  Rsp Type   Command   Err Code
      //              -----------------------  -------------   ------  --------------  ----------      ----------  -----------  ---------  --------  --------
      add_bus_rsp_act(ET_LINK_REQ_Flush,       AXI_REQ_WRITE,  A1_I_Bsy,  AXI_RESP_OKAY,  `SCQ_None,      I,          Data_None,   RspNull,   BusIdle,  EC_None  );
      add_bus_rsp_act(ET_LINK_REQ_Flush,       AXI_REQ_WRITE,  A1_E_Bsy,  AXI_RESP_OKAY,  `SCQ_None,      E,          Data_None,   RspNull,   BusIdle,  EC_None  );
      add_bus_rsp_act(ET_LINK_REQ_FlushToMem,  AXI_REQ_WRITE,  A1_I_Bsy,  AXI_RESP_OKAY,  `SCQ_None,      I,          Data_None,   RspAck,   BusIdle,  EC_None  );
      add_bus_rsp_act(ET_LINK_REQ_FlushToMem,  AXI_REQ_WRITE,  A1_E_Bsy,  AXI_RESP_OKAY,  `SCQ_None,      E,          Data_None,   RspAck,   BusIdle,  EC_None  );


      add_bus_rsp_act(ET_LINK_REQ_Evict,       AXI_REQ_WRITE,  A1_I_Bsy,  AXI_RESP_OKAY,  `SCQ_None,      I,          Data_None,   RspNull,   BusIdle,  EC_None  );
      add_bus_rsp_act(ET_LINK_REQ_EvictToMem,  AXI_REQ_WRITE,  A1_I_Bsy,  AXI_RESP_OKAY,  `SCQ_None,      I,          Data_None,   RspAck,    BusIdle,   EC_None  );


      //
      // Atomic Response sent to the Mesh
      //                                   AXI             Model                                                                    Bus
      //              ET-link Opcode       Command         State   AXI Response    Qual         Tag Action  Data Action  Rsp Type   Command   Err Code
      //              -------------------  --------------  ------  --------------  -----------  ----------  -----------  ---------  --------  --------
      add_bus_rsp_act(ET_LINK_REQ_Atomic,  AXI_REQ_WRITE,  Stale,  AXI_RESP_OKAY,  `SCQ_Stale,  St_Ignore,  Data_None,   RspNull,   BusIdle,  EC_None  );

      //
      // Response to a evict/flush Write to mem due to an Index cacheop.
      //
      //                                       AXI             Model                                                                   Bus
      //              ET-link Opcode           Command         State     AXI Response    Qual            Tag Action  Data Action  Rsp Type   Command   Err Code
      //              -----------------------  -------------   ------    --------------  ----------      ----------  -----------  ---------  --------  --------
      add_bus_rsp_act(ET_LINK_REQ_Write,       AXI_REQ_WRITE,  A1_E_Bsy, AXI_RESP_OKAY,  `SCQ_None,      E,          Data_None,   RspNull,   BusIdle,  EC_None  );

      
      //
      // Response to a read that was sent for a read-modify-write due to a partial write
      //
      //                                       AXI             Model                                                                   Bus
      //              ET-link Opcode           Command         State    AXI Response    Qual            Tag Action  Data Action        Rsp Type   Command   Err Code
      //              -----------------------  -------------   ------   --------------  ----------      ----------  -----------       ---------  --------  --------
      add_bus_rsp_act(ET_LINK_REQ_Write,       AXI_REQ_READ,   IEM_Bsy, AXI_RESP_OKAY,  `SCQ_None,      St_Ignore,  Data_EnqueueFill,   RspAck,   BusIdle,  EC_None );

      //
      // Responses while in Bypass
      //
      //                                       AXI             Model                                                                   Bus
      //              ET-link Opcode           Command         State    AXI Response    Qual            Tag Action  Data Action        Rsp Type   Command   Err Code
      //              -----------------------  -------------   ------   --------------  ----------      ----------  -----------       ---------  --------  --------
      add_bus_rsp_act(ET_LINK_REQ_Write,       AXI_REQ_WRITE,  I,       AXI_RESP_OKAY,  `SCQ_Bypass,     St_Ignore,  Data_None,        RspAck,   BusIdle,  EC_None );
      add_bus_rsp_act(ET_LINK_REQ_EvictToMem,  AXI_REQ_WRITE,  I,       AXI_RESP_OKAY,  `SCQ_Bypass,     St_Ignore,  Data_None,        RspAck,   BusIdle,  EC_None );
      add_bus_rsp_act(ET_LINK_REQ_FlushToMem,  AXI_REQ_WRITE,  I,       AXI_RESP_OKAY,  `SCQ_Bypass,     St_Ignore,  Data_None,        RspAck,   BusIdle,  EC_None );
      add_bus_rsp_act(ET_LINK_REQ_Read,        AXI_REQ_READ,   I,       AXI_RESP_OKAY,  `SCQ_Bypass,     St_Ignore,  Data_CascadeRsp,  RspWData, BusIdle,  EC_None );
   
      
      m_cache.build_coverage();
   endfunction : build_phase


   //-----------------------------------------------------------------------------------------------
   function void start_of_simulation_phase(uvm_phase phase);
      super.start_of_simulation_phase(phase);

      if (m_cache != null) begin
         m_cache.set_abstract_name($sformatf("%s(L3$)", get_root_abstract_name()));
      end
      m_template_req_data_axi.set_abstract_name($sformatf("S%0d:SYS_MESH(AXI)", get_shire_id()));
      m_template_line_desc_axi.set_abstract_name($sformatf("S%0d:SYS_MESH(AXI)", get_shire_id()));
      m_template_trans_desc_axi.set_abstract_name($sformatf("S%0d:SYS_MESH(AXI)", get_shire_id()));
      m_template_req_desc_axi.set_abstract_name($sformatf("S%0d:SYS_MESH(AXI)", get_shire_id()));
   endfunction : start_of_simulation_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string name_in = "evl_l3_scoreboard", input uvm_component parent_in = null);
      super.new(name_in, parent_in);
      set_abstract_name("SHIRE_CACHE(L3SB)");
      m_extract_etlink_from_awuser = 1'b1;
      m_sb_type                    = int'(L3_SCOREBOARD);
      m_base_port_num              = 16;
      m_verbosity_mod              = VMOD_L3_SB_WARN;
   endfunction : new

endclass : evl_l3_scoreboard

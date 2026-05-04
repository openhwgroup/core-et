//--------------------------------------------------------------------------------------------------
//
// Class: evl_tlink_bus_parser
//
class evl_tlink_bus_parser extends evl_bus_parser;

   //-----------------------------------------------------------------------------------------------
   //
   // Static Member Variables and Objects
   //
   static evl_tlink_bus_parser m_tlink_bus_parser = evl_tlink_bus_parser::get_inst();
// static string               m_source_names[$];


   //--------------------------------------------------------------------------------------------------
   //
   // Function: evl_tlink_bus_parser::get_inst
   //
   static function evl_tlink_bus_parser get_inst();
      if (m_tlink_bus_parser == null) begin
         evl_tlink_bus_parser tlink_bus_parser;

         tlink_bus_parser = new();
      end
      return m_tlink_bus_parser;
   endfunction : get_inst


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_bus_cmd_name
   //
   virtual function string get_bus_cmd_name(input evl_bus_cmd_t bus_cmd_in, input evl_bus_type_t bus_type_in = EVL_BUS_TYPE_UNKNOWN);
      evl_tlink_req_opcode_t req_opcode = evl_tlink_req_opcode_t'(bus_cmd_in & `EVL_TL_REQ_OPCODE_MASK);

      return $sformatf("%p", req_opcode);
   endfunction : get_bus_cmd_name


   //-----------------------------------------------------------------------------------------------
   //
   // Function: bus_cmd_requires_rsp
   //
   virtual function bit bus_cmd_requires_rsp(input evl_bus_cmd_t bus_cmd_in, input evl_bus_type_t bus_type_in = EVL_BUS_TYPE_UNKNOWN);
      return 1'b1;
   endfunction : bus_cmd_requires_rsp


   //-----------------------------------------------------------------------------------------------
   //
   // Function: bus_cmd_is_read
   //
   virtual function bit bus_cmd_is_read(input evl_bus_cmd_t bus_cmd_in, input evl_bus_type_t bus_type_in = EVL_BUS_TYPE_UNKNOWN);
      if (evl_tlink_req_opcode_t'(bus_cmd_in & `EVL_TL_REQ_OPCODE_MASK) inside { TLA_AtomicArith, TLA_AtomicLogic, TLA_Get, TLA_AcquireBlock,
                                                                                 TLB_AtomicArith, TLB_AtomicLogic, TLB_Get, TLB_ProbeBlock }) begin
         return 1'b1;
      end
      return 1'b0;
   endfunction : bus_cmd_is_read


   //-----------------------------------------------------------------------------------------------
   //
   // Function: bus_cmd_is_write
   //
   virtual function bit bus_cmd_is_write(input evl_bus_cmd_t bus_cmd_in, input evl_bus_type_t bus_type_in = EVL_BUS_TYPE_UNKNOWN);
      if (evl_tlink_req_opcode_t'(bus_cmd_in & `EVL_TL_REQ_OPCODE_MASK) inside { TLA_PutFull, TLA_PutPartial, TLA_AtomicArith, TLA_AtomicLogic,
                                                                                 TLB_PutFull, TLB_PutPartial, TLB_AtomicArith, TLB_AtomicLogic,
                                                                                 TLC_Release, TLC_ReleaseData }) begin
         return 1'b1;
      end
      return 1'b0;
   endfunction : bus_cmd_is_write


   //-----------------------------------------------------------------------------------------------
   //
   // Function: bus_cmd_is_scratchpad
   //
   virtual function bit bus_cmd_is_scratchpad(input evl_bus_cmd_t bus_cmd_in, input evl_bus_type_t bus_type_in = EVL_BUS_TYPE_UNKNOWN);
      return 1'b0;
   endfunction : bus_cmd_is_scratchpad


   //-----------------------------------------------------------------------------------------------
   //
   // Function: bus_cmd_is_cache_op
   //
   virtual function bit bus_cmd_is_cache_op(input evl_bus_cmd_t bus_cmd_in, input evl_bus_type_t bus_type_in = EVL_BUS_TYPE_UNKNOWN);
      return 1'b0;
   endfunction : bus_cmd_is_cache_op


   //-----------------------------------------------------------------------------------------------
   //
   // Function: bus_cmd_is_atomic
   //
   virtual function bit bus_cmd_is_atomic(input evl_bus_cmd_t bus_cmd_in, input evl_bus_type_t bus_type_in = EVL_BUS_TYPE_UNKNOWN);
      if (evl_tlink_req_opcode_t'(bus_cmd_in & `EVL_TL_REQ_OPCODE_MASK) inside { TLA_AtomicArith, TLA_AtomicLogic,
                                                                                 TLB_AtomicArith, TLB_AtomicLogic }) begin
         return 1'b1;
      end
      return 1'b0;
   endfunction : bus_cmd_is_atomic


   //-----------------------------------------------------------------------------------------------
   //
   // Function: bus_cmd_is_acquire
   //
   virtual function bit bus_cmd_is_acquire(input evl_bus_cmd_t bus_cmd_in, input evl_bus_type_t bus_type_in = EVL_BUS_TYPE_UNKNOWN);
      if (evl_tlink_req_opcode_t'(bus_cmd_in & `EVL_TL_REQ_OPCODE_MASK) inside { TLA_AcquireBlock, TLA_AcquirePerm }) begin
         return 1'b1;
      end
      return 1'b0;
   endfunction : bus_cmd_is_acquire


   //-----------------------------------------------------------------------------------------------
   //
   // Function: bus_cmd_has_read_data
   //
   virtual function bit bus_cmd_has_read_data(input evl_bus_cmd_t bus_cmd_in, input evl_bus_type_t bus_type_in = EVL_BUS_TYPE_UNKNOWN);
      if (evl_tlink_req_opcode_t'(bus_cmd_in & `EVL_TL_REQ_OPCODE_MASK) inside { TLA_AtomicArith, TLA_AtomicLogic, TLA_Get, TLA_AcquireBlock,
                                                                                 TLB_AtomicArith, TLB_AtomicLogic, TLB_Get, TLB_ProbeBlock }) begin
         return 1'b1;
      end
      return 1'b0;
   endfunction : bus_cmd_has_read_data


   //-----------------------------------------------------------------------------------------------
   //
   // Function: bus_cmd_has_write_data
   //
   virtual function bit bus_cmd_has_write_data(input evl_bus_cmd_t bus_cmd_in, input evl_bus_type_t bus_type_in = EVL_BUS_TYPE_UNKNOWN);
      if (evl_tlink_req_opcode_t'(bus_cmd_in & `EVL_TL_REQ_OPCODE_MASK) inside { TLA_PutFull, TLA_PutPartial, TLA_AtomicArith, TLA_AtomicLogic,
                                                                                 TLB_PutFull, TLB_PutPartial, TLB_AtomicArith, TLB_AtomicLogic,
                                                                                 TLC_ReleaseData }) begin
         return 1'b1;
      end
      return 1'b0;
   endfunction : bus_cmd_has_write_data


   //-----------------------------------------------------------------------------------------------
   //
   // Function: bus_cmd_has_unique_tag
   //
   virtual function bit bus_cmd_has_unique_tag(input evl_bus_cmd_t bus_cmd_in, input evl_bus_type_t bus_type_in = EVL_BUS_TYPE_UNKNOWN);
      return 1'b1;
   endfunction : bus_cmd_has_unique_tag


   //-----------------------------------------------------------------------------------------------
   //
   // Function: convert_bus_cmd
   //
   virtual function int convert_bus_cmd(input evl_abstract_bus_cmd_t bus_cmd_in, input evl_bus_type_t bus_type_in = EVL_BUS_TYPE_UNKNOWN);
//    case (bus_cmd_in)
//       BusRead:        return int'(ET_LINK_REQ_Read);
//       BusReadCoop:    return int'(ET_LINK_REQ_ReadCoop);
//       BusLineRd:      return int'(ET_LINK_REQ_Read);
//       BusWrite:       return int'(ET_LINK_REQ_Write);
//       BusWriteAround: return int'(ET_LINK_REQ_WriteAround);
//       BusWriteAtomic: return int'(ET_LINK_REQ_Atomic);
//       BusLocalMsg:    return int'(ET_LINK_REQ_MsgSendData);
//       BusWriteMsg:    return int'(ET_LINK_REQ_MsgSendData);
//       BusFlush:       return int'(ET_LINK_REQ_Flush);
//       BusFlushToMem:  return int'(ET_LINK_REQ_FlushToMem);
//       BusEvict:       return int'(ET_LINK_REQ_Evict);
//       BusEvictToMem:  return int'(ET_LINK_REQ_EvictToMem);
//       BusPrefetch:    return int'(ET_LINK_REQ_Prefetch);
//       BusSysPrefetch: return int'(ET_LINK_REQ_ScpFill);
//    endcase
      return `EVL_Bus_Idle;
   endfunction : convert_bus_cmd


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_bus_rsp_name
   //
   virtual function string get_bus_rsp_name(input evl_bus_rsp_type_t bus_rsp_in, input evl_bus_type_t bus_type_in = EVL_BUS_TYPE_UNKNOWN);
      evl_tlink_rsp_opcode_t rsp_opcode = evl_tlink_rsp_opcode_t'(bus_rsp_in & `EVL_TL_RSP_OPCODE_MASK);

      if ((bus_rsp_in & evl_bus_rsp_type_t'(`EVL_TL_RSP_DENIED)) != 0) begin
         return $sformatf("%p (denied)", rsp_opcode);
      end
      if ((bus_rsp_in & evl_bus_rsp_type_t'(`EVL_TL_RSP_CORRUPT)) != 0) begin
         return $sformatf("%p (corrupt)", rsp_opcode);
      end
      return $sformatf("%p", rsp_opcode);
   endfunction : get_bus_rsp_name


   //-----------------------------------------------------------------------------------------------
   //
   // Function: bus_rsp_has_data
   //
   virtual function bit bus_rsp_has_data(input evl_bus_rsp_type_t bus_rsp_in, input evl_bus_type_t bus_type_in = EVL_BUS_TYPE_UNKNOWN);
      if (evl_tlink_rsp_opcode_t'(bus_rsp_in & `EVL_TL_RSP_OPCODE_MASK) inside { TLC_AccessAckData, TLC_ProbeAckData,
                                                                                 TLD_AccessAckData, TLD_GrantData }) begin
         return 1'b1;
      end
      return 1'b0;
   endfunction : bus_rsp_has_data


   //-----------------------------------------------------------------------------------------------
   //
   // Function: bus_rsp_has_error
   //
   virtual function bit bus_rsp_has_error(input evl_bus_rsp_type_t bus_rsp_in, input evl_bus_type_t bus_type_in = EVL_BUS_TYPE_UNKNOWN);
      if ((bus_rsp_in & evl_bus_rsp_type_t'(`EVL_TL_RSP_CORRUPT)) != 0) begin
         return 1'b1;
      end
      return 1'b0;
   endfunction : bus_rsp_has_error


   //-----------------------------------------------------------------------------------------------
   //
   // Function: convert_bus_rsp
   //
   virtual function int convert_bus_rsp(input evl_abstract_rsp_type_t bus_rsp_in, input evl_bus_type_t bus_type_in = EVL_BUS_TYPE_UNKNOWN);
      if (bus_rsp_in == RspAck) begin
         return int'(TLD_AccessAck);
      end
      else if (bus_rsp_in == RspWData) begin
         return int'(TLD_AccessAckData);
      end
      return `EVL_Rsp_Null;
   endfunction : convert_bus_rsp


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_source_name
   //
   virtual function string get_source_name(input evl_bus_cmd_t        bus_cmd_in,
                                           input evl_bus_req_source_t bus_source_in,
                                           input evl_bus_type_t       bus_type_in = EVL_BUS_TYPE_UNKNOWN);
//    return m_source_names[bus_source_in];
      return $sformatf("0x%0x", bus_source_in);
   endfunction : get_source_name


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_msg_dest_name
   //
   virtual function string get_msg_dest_name(input evl_req_desc req_desc);
      return "";
   endfunction : get_msg_dest_name


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_rcv_msg_dest_name
   //
   virtual function string get_rcv_msg_dest_name(input evl_req_desc req_desc);
      return "";
   endfunction : get_rcv_msg_dest_name


   //-----------------------------------------------------------------------------------------------
   //
   // Function: sprint_obj
   //
   virtual function string sprint_obj(input evl_req_desc   req_desc,
                                      input int            style_in        = 0,
                                      input int            transfer_idx_in = -1,
                                      input evl_bus_type_t bus_type_in     = EVL_BUS_TYPE_UNKNOWN);
      int    req_size;
      string req_source;
      string message;
//    evl_subopcode_dest_t my_subopcode = evl_subopcode_dest_t'(req_desc.get_bus_subopcode_dest());

      req_size   = (1 << req_desc.get_bus_req_size());
      if (req_desc.get_bus_req_param() == 0) begin
         req_source = $sformatf("%s, %0d byte%0s", get_source_name(req_desc.get_bus_cmd(), req_desc.get_bus_req_source()), req_size, ((req_size == 1) ? "" : "s"));
      end
      else begin
         evl_tlink_param_t param = evl_tlink_param_t'(req_desc.get_bus_req_param());

         req_source = $sformatf("%p, %s, %0d byte%0s", param, get_source_name(req_desc.get_bus_cmd(), req_desc.get_bus_req_source()), req_size, ((req_size == 1) ? "" : "s"));
      end
      if ((req_desc.bus_cmd_has_write_data() == 1'b1) && (style_in == 1)) begin
         message = $sformatf("%0s 0x%x (%0s, ID:0x%02x, TrID 0x%x, id %0d%0s)%0s <- %0s",
                      req_desc.get_bus_cmd_name(), req_desc.get_paddr(), req_source, req_desc.get_bus_req_id(), req_desc.get_trans_id(), req_desc.get_inst_id(), ((req_desc.is_done() == 1'b1) ? ", done" : ""), ((req_desc.is_bus_child() == 1'b1) ? "*" : ""), req_desc.format_req_data());
//       if (evl_tlink_req_opcode_t'(req_desc.get_bus_cmd()) == ET_LINK_REQ_Atomic) begin
//          message = $sformatf("%0s conf:0x%2x", message, req_desc.get_bus_atomic_conf());
//       end
      end
      else begin
         message = $sformatf("%0s 0x%x (%0s, ID:0x%02x, TrID 0x%x, id %0d%0s)%0s",
                      req_desc.get_bus_cmd_name(), req_desc.get_paddr(), req_source, req_desc.get_bus_req_id(), req_desc.get_trans_id(), req_desc.get_inst_id(), ((req_desc.is_done() == 1'b1) ? ", done" : ""), ((req_desc.is_bus_child() == 1'b1) ? "*" : ""));
      end
      return message;
   endfunction : sprint_obj


   //-----------------------------------------------------------------------------------------------
   //
   // Function: sprint_rsp
   //
   virtual function string sprint_rsp(input evl_req_desc   req_desc,
                                      input int            dword_start = 0,
                                      input int            dword_end   = `EVL_LINE_DWORDS - 1,
                                      input evl_bus_type_t bus_type_in = EVL_BUS_TYPE_UNKNOWN);
      string message;

      if (req_desc.get_bus_rsp_param() == 0) begin
         message = $sformatf("%0s", req_desc.get_bus_rsp_name());
      end
      else begin
         evl_tlink_param_t param = evl_tlink_param_t'(req_desc.get_bus_rsp_param());

         message = $sformatf("%0s (%p)", req_desc.get_bus_rsp_name(), param);
      end
      if (req_desc.bus_rsp_has_data() == 1'b1) begin
         message = $sformatf("%0s 0x%0s", message, req_desc.format_rsp_data(dword_start, dword_end));
      end
      return message;
   endfunction : sprint_rsp


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new();
      super.new("TileLinkParser", EVL_BUS_TYPE_TLINK);
      if (m_tlink_bus_parser == null) begin
         m_tlink_bus_parser = this;
         evl_bus_parser::add_bus_parser(this);
      end
   endfunction : new

endclass : evl_tlink_bus_parser

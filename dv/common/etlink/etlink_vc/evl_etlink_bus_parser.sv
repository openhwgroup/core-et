//--------------------------------------------------------------------------------------------------
//
// Class: evl_etlink_bus_parser
//
class evl_etlink_bus_parser extends evl_bus_parser;

   //-----------------------------------------------------------------------------------------------
   //
   // Static Member Variables and Objects
   //
   static evl_etlink_bus_parser m_etlink_bus_parser = evl_etlink_bus_parser::get_inst();
   static string                m_source_names[$];
   static string                m_maxion_source_names[$];


   //--------------------------------------------------------------------------------------------------
   //
   // Function: evl_etlink_bus_parser::get_inst
   //
   static function evl_etlink_bus_parser get_inst();
      if (m_etlink_bus_parser == null) begin
         evl_etlink_bus_parser etlink_bus_parser;

         etlink_bus_parser = new();
      end
      return m_etlink_bus_parser;
   endfunction : get_inst


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_bus_cmd_name
   //
   virtual function string get_bus_cmd_name(input evl_bus_cmd_t bus_cmd_in, input evl_bus_type_t bus_type_in = EVL_BUS_TYPE_UNKNOWN);
      evl_etlink_req_opcode_t req_opcode;

      req_opcode = evl_etlink_req_opcode_t'(bus_cmd_in);
      return $sformatf("%p", req_opcode);
   endfunction : get_bus_cmd_name


   //-----------------------------------------------------------------------------------------------
   //
   // Function: bus_cmd_requires_rsp
   //
   virtual function bit bus_cmd_requires_rsp(input evl_bus_cmd_t bus_cmd_in, input evl_bus_type_t bus_type_in = EVL_BUS_TYPE_UNKNOWN);
      evl_etlink_req_opcode_t req_opcode;

      req_opcode = evl_etlink_req_opcode_t'(bus_cmd_in);
      return ((req_opcode inside { ET_LINK_REQ_MsgSendData }) ? 1'b0 : 1'b1);
   endfunction : bus_cmd_requires_rsp


   //-----------------------------------------------------------------------------------------------
   //
   // Function: bus_cmd_is_read
   //
   virtual function bit bus_cmd_is_read(input evl_bus_cmd_t bus_cmd_in, input evl_bus_type_t bus_type_in = EVL_BUS_TYPE_UNKNOWN);
      evl_etlink_req_opcode_t req_opcode;

      req_opcode = evl_etlink_req_opcode_t'(bus_cmd_in);
      return ((req_opcode inside { ET_LINK_REQ_Read, ET_LINK_REQ_ReadCoop, ET_LINK_REQ_Atomic }) ? 1'b1 : 1'b0);
   endfunction : bus_cmd_is_read


   //-----------------------------------------------------------------------------------------------
   //
   // Function: bus_cmd_is_write
   //
   virtual function bit bus_cmd_is_write(input evl_bus_cmd_t bus_cmd_in, input evl_bus_type_t bus_type_in = EVL_BUS_TYPE_UNKNOWN);
      evl_etlink_req_opcode_t req_opcode;

      req_opcode = evl_etlink_req_opcode_t'(bus_cmd_in);
      return ((req_opcode inside { ET_LINK_REQ_Write, ET_LINK_REQ_WriteAround, ET_LINK_REQ_Atomic, ET_LINK_REQ_MsgSendData }) ? 1'b1 : 1'b0);
   endfunction : bus_cmd_is_write


   //-----------------------------------------------------------------------------------------------
   //
   // Function: bus_cmd_is_scratchpad
   //
   virtual function bit bus_cmd_is_scratchpad(input evl_bus_cmd_t bus_cmd_in, input evl_bus_type_t bus_type_in = EVL_BUS_TYPE_UNKNOWN);
      evl_etlink_req_opcode_t req_opcode;

      req_opcode = evl_etlink_req_opcode_t'(bus_cmd_in);
      return ((req_opcode inside { ET_LINK_REQ_ScpFill }) ? 1'b1 : 1'b0);
   endfunction : bus_cmd_is_scratchpad


   //-----------------------------------------------------------------------------------------------
   //
   // Function: bus_cmd_is_cache_op
   //
   virtual function bit bus_cmd_is_cache_op(input evl_bus_cmd_t bus_cmd_in, input evl_bus_type_t bus_type_in = EVL_BUS_TYPE_UNKNOWN);
      evl_etlink_req_opcode_t req_opcode;

      req_opcode = evl_etlink_req_opcode_t'(bus_cmd_in);
      return ((req_opcode inside { ET_LINK_REQ_Flush, ET_LINK_REQ_FlushToMem, ET_LINK_REQ_Evict, ET_LINK_REQ_EvictToMem, ET_LINK_REQ_Lock, ET_LINK_REQ_Unlock, ET_LINK_REQ_ScpFill, ET_LINK_REQ_Prefetch }) ? 1'b1 : 1'b0);
   endfunction : bus_cmd_is_cache_op


   //-----------------------------------------------------------------------------------------------
   //
   // Function: bus_cmd_is_atomic
   //
   virtual function bit bus_cmd_is_atomic(input evl_bus_cmd_t bus_cmd_in, input evl_bus_type_t bus_type_in = EVL_BUS_TYPE_UNKNOWN);
      evl_etlink_req_opcode_t req_opcode;

      req_opcode = evl_etlink_req_opcode_t'(bus_cmd_in);
      return ((req_opcode inside { ET_LINK_REQ_Atomic }) ? 1'b1 : 1'b0);
   endfunction : bus_cmd_is_atomic


   //-----------------------------------------------------------------------------------------------
   //
   // Function: bus_cmd_has_read_data
   //
   virtual function bit bus_cmd_has_read_data(input evl_bus_cmd_t bus_cmd_in, input evl_bus_type_t bus_type_in = EVL_BUS_TYPE_UNKNOWN);
      evl_etlink_req_opcode_t req_opcode;

      req_opcode = evl_etlink_req_opcode_t'(bus_cmd_in);
      return ((req_opcode inside { ET_LINK_REQ_Read, ET_LINK_REQ_ReadCoop, ET_LINK_REQ_Atomic }) ? 1'b1 : 1'b0);
   endfunction : bus_cmd_has_read_data


   //-----------------------------------------------------------------------------------------------
   //
   // Function: bus_cmd_has_write_data
   //
   virtual function bit bus_cmd_has_write_data(input evl_bus_cmd_t bus_cmd_in, input evl_bus_type_t bus_type_in = EVL_BUS_TYPE_UNKNOWN);
      evl_etlink_req_opcode_t req_opcode;

      req_opcode = evl_etlink_req_opcode_t'(bus_cmd_in);
      return ((req_opcode inside { ET_LINK_REQ_Write, ET_LINK_REQ_WriteAround, ET_LINK_REQ_Flush, ET_LINK_REQ_FlushToMem, ET_LINK_REQ_Evict, ET_LINK_REQ_EvictToMem, ET_LINK_REQ_Lock, ET_LINK_REQ_Unlock, ET_LINK_REQ_ScpFill, ET_LINK_REQ_Prefetch, ET_LINK_REQ_Atomic, ET_LINK_REQ_MsgSendData }) ? 1'b1 : 1'b0);
   endfunction : bus_cmd_has_write_data


   //-----------------------------------------------------------------------------------------------
   //
   // Function: bus_cmd_has_unique_tag
   //
   virtual function bit bus_cmd_has_unique_tag(input evl_bus_cmd_t bus_cmd_in, input evl_bus_type_t bus_type_in = EVL_BUS_TYPE_UNKNOWN);
      evl_etlink_req_opcode_t req_opcode;

      req_opcode = evl_etlink_req_opcode_t'(bus_cmd_in);
      return ((req_opcode inside { ET_LINK_REQ_Write, ET_LINK_REQ_WriteAround, ET_LINK_REQ_Flush, ET_LINK_REQ_FlushToMem, ET_LINK_REQ_Evict, ET_LINK_REQ_EvictToMem, ET_LINK_REQ_Lock, ET_LINK_REQ_Unlock, ET_LINK_REQ_ScpFill, ET_LINK_REQ_Prefetch }) ? 1'b0 : 1'b1);
   endfunction : bus_cmd_has_unique_tag


   //-----------------------------------------------------------------------------------------------
   //
   // Function: convert_bus_cmd
   //
   virtual function int convert_bus_cmd(input evl_abstract_bus_cmd_t bus_cmd_in, input evl_bus_type_t bus_type_in = EVL_BUS_TYPE_UNKNOWN);
      case (bus_cmd_in)
         BusRead:        return int'(ET_LINK_REQ_Read);
         BusReadCoop:    return int'(ET_LINK_REQ_ReadCoop);
         BusLineRd:      return int'(ET_LINK_REQ_Read);
         BusWrite:       return int'(ET_LINK_REQ_Write);
         BusWriteAround: return int'(ET_LINK_REQ_WriteAround);
         BusWriteAtomic: return int'(ET_LINK_REQ_Atomic);
         BusLocalMsg:    return int'(ET_LINK_REQ_MsgSendData);
         BusWriteMsg:    return int'(ET_LINK_REQ_MsgSendData);
         BusFlush:       return int'(ET_LINK_REQ_Flush);
         BusFlushToMem:  return int'(ET_LINK_REQ_FlushToMem);
         BusEvict:       return int'(ET_LINK_REQ_Evict);
         BusEvictToMem:  return int'(ET_LINK_REQ_EvictToMem);
         BusPrefetch:    return int'(ET_LINK_REQ_Prefetch);
         BusSysPrefetch: return int'(ET_LINK_REQ_ScpFill);
      endcase
      return `EVL_Bus_Idle;
   endfunction : convert_bus_cmd


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_bus_rsp_name
   //
   virtual function string get_bus_rsp_name(input evl_bus_rsp_type_t bus_rsp_in, input evl_bus_type_t bus_type_in = EVL_BUS_TYPE_UNKNOWN);
      evl_etlink_rsp_opcode_t rsp_opcode;

      if (bus_rsp_in == `EVL_Rsp_Null) begin
         return "EVL_Rsp_Null";
      end
      rsp_opcode = evl_etlink_rsp_opcode_t'(bus_rsp_in);
      return $sformatf("%p", rsp_opcode);
   endfunction : get_bus_rsp_name


   //-----------------------------------------------------------------------------------------------
   //
   // Function: bus_rsp_has_data
   //
   virtual function bit bus_rsp_has_data(input evl_bus_rsp_type_t bus_rsp_in, input evl_bus_type_t bus_type_in = EVL_BUS_TYPE_UNKNOWN);
      evl_etlink_rsp_opcode_t rsp_opcode;

      rsp_opcode = evl_etlink_rsp_opcode_t'(bus_rsp_in);
      return ((rsp_opcode inside { ET_LINK_RSP_AckData, ET_LINK_RSP_MsgRcvData }) ? 1'b1 : 1'b0);
   endfunction : bus_rsp_has_data


   //-----------------------------------------------------------------------------------------------
   //
   // Function: bus_rsp_has_error
   //
   virtual function bit bus_rsp_has_error(input evl_bus_rsp_type_t bus_rsp_in, input evl_bus_type_t bus_type_in = EVL_BUS_TYPE_UNKNOWN);
      evl_etlink_rsp_opcode_t rsp_opcode;

      rsp_opcode = evl_etlink_rsp_opcode_t'(bus_rsp_in);
      return ((rsp_opcode inside { ET_LINK_RSP_Err }) ? 1'b1 : 1'b0);
   endfunction : bus_rsp_has_error


   //-----------------------------------------------------------------------------------------------
   //
   // Function: convert_bus_rsp
   //
   virtual function int convert_bus_rsp(input evl_abstract_rsp_type_t bus_rsp_in, input evl_bus_type_t bus_type_in = EVL_BUS_TYPE_UNKNOWN);
      if (bus_rsp_in == RspAck) begin
         return int'(ET_LINK_RSP_Ack);
      end
      else if (bus_rsp_in inside { RspWData }) begin
         return int'(ET_LINK_RSP_AckData);
      end
      else if (bus_rsp_in == RspMsgData) begin
         return int'(ET_LINK_RSP_MsgRcvData);
      end
      else if (bus_rsp_in == RspErr) begin
         return int'(ET_LINK_RSP_Err);
      end
      return `EVL_Rsp_Null;
   endfunction : convert_bus_rsp


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_source_name
   //
   virtual function string get_source_name(input evl_bus_cmd_t        bus_cmd_in,
                                           input evl_bus_req_source_t bus_source_in,
                                           input evl_core_type_t      core_type_in = EVL_MinionCore,
                                           input evl_bus_type_t       bus_type_in  = EVL_BUS_TYPE_UNKNOWN);
      if (core_type_in == EVL_MaxionCore) begin
         return m_maxion_source_names[bus_source_in];
      end
      else if (evl_etlink_req_opcode_t'(bus_cmd_in) inside { ET_LINK_REQ_MsgSendData }) begin
         case (bus_source_in)
            0:       return "Minion0, Thread0";
            1:       return "Minion0, Thread1";
            2:       return "Minion1, Thread0";
            3:       return "Minion1, Thread1";
            4:       return "Minion2, Thread0";
            5:       return "Minion2, Thread1";
            6:       return "Minion3, Thread0";
            7:       return "Minion3, Thread1";
            8:       return "Minion4, Thread0";
            9:       return "Minion4, Thread1";
            10:      return "Minion5, Thread0";
            11:      return "Minion5, Thread1";
            12:      return "Minion6, Thread0";
            13:      return "Minion6, Thread1";
            14:      return "Minion7, Thread0";
            default: return "Minion7, Thread1";
         endcase
      end
      return m_source_names[bus_source_in];
   endfunction : get_source_name


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_msg_dest_name
   //
   virtual function string get_msg_dest_name(input evl_req_desc req_desc);
      // Extract destination from the message physical adddress
      evl_paddr_t    dest_p_addr   = req_desc.get_paddr();
      int            dest_neigh_id = int'(dest_p_addr[`EVL_BUS_MSG_DST_NEIGH_MSB:`EVL_BUS_MSG_DST_NEIGH_LSB]);
      int            dest_core_id  = int'(dest_p_addr[`EVL_BUS_MSG_DST_MINION_MSB:`EVL_BUS_MSG_DST_MINION_LSB]);
      int            dest_hart_id  = int'(dest_p_addr[`EVL_BUS_MSG_DST_HART_MSB]);
      //evl_shire_id_t dest_shire_id = evl_shire_id_t'(dest_p_addr[`EVL_BUS_MSG_SHIRE_ID_MSB:`EVL_BUS_MSG_SHIRE_ID_LSB]);
      evl_shire_id_t dest_shire_id = 0;
      int            port_id       = int'(dest_p_addr[`EVL_BUS_MSG_PORT_MSB:`EVL_BUS_MSG_PORT_LSB]);

      if ((dest_shire_id == ~evl_shire_id_t'(0)) && (req_desc != null)) begin
         return $sformatf("S%0dN%0dM%0dH%0d P%0d (shire 0xff)", req_desc.get_shire_id(), dest_neigh_id, dest_core_id, dest_hart_id, port_id);
      end
      else begin
         return $sformatf("S%0dN%0dM%0dH%0d P%0d", dest_shire_id, dest_neigh_id, dest_core_id, dest_hart_id, port_id);
      end
   endfunction : get_msg_dest_name


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_rcv_msg_dest_name
   //
   virtual function string get_rcv_msg_dest_name(input evl_req_desc req_desc);
      if (req_desc == null) begin
         return "";
      end
      else begin
         // Extract destination from the message physical adddress
         int dest_neigh_id = int'(req_desc.get_neighborhood_id());
         int dest_core_id  = int'(req_desc.get_bus_rsp_dest()) >> 1;
         int dest_hart_id  = int'(req_desc.get_bus_rsp_dest()) & 1;
         int dest_shire_id = int'(req_desc.get_shire_id());

         return $sformatf("S%0dN%0dM%0dH%0d", dest_shire_id, dest_neigh_id, dest_core_id, dest_hart_id);
      end
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

      req_size = (1 << req_desc.get_bus_req_size());
      if (et_link_req_opcode_t'(req_desc.get_bus_cmd()) inside { ET_LINK_REQ_Read, ET_LINK_REQ_Write }) begin
         evl_subopcode_dest_t my_subopcode = evl_subopcode_dest_t'(req_desc.get_bus_subopcode_dest());

         req_source = $sformatf("%s, %p, %0d byte%0s", get_source_name(req_desc.get_bus_cmd(), req_desc.get_bus_req_source(), req_desc.get_core_type()), my_subopcode, req_size, ((req_size == 1) ? "" : "s"));
      end
      else begin
         req_source = $sformatf("%s, %0d byte%0s", get_source_name(req_desc.get_bus_cmd(), req_desc.get_bus_req_source(), req_desc.get_core_type()), req_size, ((req_size == 1) ? "" : "s"));
      end

      // Decode Message packets and append the destination hart after the source
      if (et_link_req_opcode_t'(req_desc.get_bus_cmd()) inside {ET_LINK_REQ_MsgSendData}) begin
         req_source = $sformatf("%s, DST %s", req_source, get_msg_dest_name(req_desc));
      end

      if (req_desc.bus_cmd_is_cache_op()) begin
         string start_level;
         string dest_level;

         case (req_desc.get_bus_cop_start_level())
            0:       start_level = "L1";
            1:       start_level = "L2";
            2:       start_level = "L3";
            default: start_level = "--";
         endcase

         case (req_desc.get_bus_cop_dest_level())
            0:       dest_level = "--";
            1:       dest_level = "L2";
            2:       dest_level = "L3";
            default: dest_level = "Mem";
         endcase // case (req_desc.get_bus_cop_dest_level())

         case (et_link_req_opcode_t'(req_desc.get_bus_cmd()))
            ET_LINK_REQ_ReadCoop:   begin
                                       message = $sformatf("%0s neigh mask %b minion_mask %b 0x%x (ID:0x%02x, TrID 0x%x, id %0d%0s)%0s",
                                                    req_desc.get_bus_cmd_name(),req_desc.get_bus_neigh_coop_mask(), req_desc.get_bus_minion_coop_mask(), req_desc.get_paddr(), req_desc.get_bus_req_id(), req_desc.get_trans_id(), req_desc.get_inst_id(), ((req_desc.is_done() == 1'b1) ? ", done" : ""), ((req_desc.is_bus_child() == 1'b1) ? "*" : ""));
                                    end
            ET_LINK_REQ_Flush,
            ET_LINK_REQ_Evict:      begin
                                       message = $sformatf("%0s (%0s -> %0s) 0x%x (ID:0x%02x, TrID 0x%x, id %0d%0s)%0s",
                                                    req_desc.get_bus_cmd_name(), start_level, dest_level, req_desc.get_paddr(), req_desc.get_bus_req_id(), req_desc.get_trans_id(), req_desc.get_inst_id(), ((req_desc.is_done() == 1'b1) ? ", done" : ""), ((req_desc.is_bus_child() == 1'b1) ? "*" : ""));
                                    end

            ET_LINK_REQ_FlushToMem,
            ET_LINK_REQ_EvictToMem: if (style_in == 1) begin
                                       message = $sformatf("%0s 0x%x (%0s, ID:0x%02x, TrID 0x%x, id %0d%0s)%0s <- 0x%0s",
                                                    req_desc.get_bus_cmd_name(), req_desc.get_paddr(), req_source, req_desc.get_bus_req_id(), req_desc.get_trans_id(), req_desc.get_inst_id(), ((req_desc.is_done() == 1'b1) ? ", done" : ""), ((req_desc.is_bus_child() == 1'b1) ? "*" : ""), req_desc.format_req_data());
                                    end
                                    else begin
                                       message = $sformatf("%0s 0x%x (%0s, ID:0x%02x, TrID 0x%x, id %0d%0s)%0s",
                                                    req_desc.get_bus_cmd_name(), req_desc.get_paddr(), req_source, req_desc.get_bus_req_id(), req_desc.get_trans_id(), req_desc.get_inst_id(), ((req_desc.is_done() == 1'b1) ? ", done" : ""), ((req_desc.is_bus_child() == 1'b1) ? "*" : ""));
                                    end

            ET_LINK_REQ_Lock:       begin
                                       message = $sformatf("%0s 0x%x (%0s, ID:0x%02x, TrID 0x%x, id %0d%0s)%0s",
                                                    req_desc.get_bus_cmd_name(), req_desc.get_paddr(), req_source, req_desc.get_bus_req_id(), req_desc.get_trans_id(), req_desc.get_inst_id(), ((req_desc.is_done() == 1'b1) ? ", done" : ""), ((req_desc.is_bus_child() == 1'b1) ? "*" : ""));
                                    end

            ET_LINK_REQ_Unlock:     begin
                                       message = $sformatf("%0s (%0s) 0x%x (ID:0x%02x, TrID 0x%x, id %0d%0s)%0s",
                                                    req_desc.get_bus_cmd_name(), ((req_desc.get_bus_cop_final_state() == 1'b1) ? "preserve" : "invalidate"), req_desc.get_paddr(), req_desc.get_bus_req_id(), req_desc.get_trans_id(), req_desc.get_inst_id(), ((req_desc.is_done() == 1'b1) ? ", done" : ""), ((req_desc.is_bus_child() == 1'b1) ? "*" : ""));
                                    end

            ET_LINK_REQ_ScpFill:    begin
                                       message = $sformatf("%0s 0x%x -> 0x%x (ID:0x%02x, TrID 0x%x, id %0d%0s)%0s",
                                                    req_desc.get_bus_cmd_name(), req_desc.get_bus_scp_fill_addr(), req_desc.get_paddr(), req_desc.get_bus_req_id(), req_desc.get_trans_id(), req_desc.get_inst_id(), ((req_desc.is_done() == 1'b1) ? ", done" : ""), ((req_desc.is_bus_child() == 1'b1) ? "*" : ""));
                                    end

            ET_LINK_REQ_Prefetch:   begin
                                       message = $sformatf("%0s (%0s) 0x%x (ID:0x%02x, TrID 0x%x, id %0d%0s)%0s",
                                                    req_desc.get_bus_cmd_name(), dest_level, req_desc.get_paddr(), req_desc.get_bus_req_id(), req_desc.get_trans_id(), req_desc.get_inst_id(), ((req_desc.is_done() == 1'b1) ? ", done" : ""), ((req_desc.is_bus_child() == 1'b1) ? "*" : ""));
                                    end

            default:                begin
                                       message = $sformatf("%0s (UNKNOWN) 0x%x (ID:0x%02x, TrID 0x%x, id %0d%0s)%0s",
                                                    req_desc.get_bus_cmd_name(), req_desc.get_paddr(), req_desc.get_bus_req_id(), req_desc.get_trans_id(), req_desc.get_inst_id(), ((req_desc.is_done() == 1'b1) ? ", done" : ""), ((req_desc.is_bus_child() == 1'b1) ? "*" : ""));
                                    end
         endcase
      end
      else if ((req_desc.bus_cmd_has_write_data() == 1'b1) && (style_in == 1)) begin
         message = $sformatf("%0s 0x%x (%0s, ID:0x%02x, TrID 0x%x, id %0d%0s)%0s <- %0s",
                      req_desc.get_bus_cmd_name(), req_desc.get_paddr(), req_source, req_desc.get_bus_req_id(), req_desc.get_trans_id(), req_desc.get_inst_id(), ((req_desc.is_done() == 1'b1) ? ", done" : ""), ((req_desc.is_bus_child() == 1'b1) ? "*" : ""), req_desc.format_req_data());
         if (et_link_req_opcode_t'(req_desc.get_bus_cmd()) == ET_LINK_REQ_Atomic) begin
            message = $sformatf("%0s conf:0x%2x", message, req_desc.get_bus_atomic_conf());
         end
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

      message = $sformatf("%0s", req_desc.get_bus_rsp_name());
      if (req_desc.bus_rsp_has_data() == 1'b1) begin
         message = $sformatf("%0s 0x%0s", message, req_desc.format_rsp_data(dword_start, dword_end));
      end
      if (req_desc.get_bus_rsp_type() == evl_bus_rsp_type_t'(ET_LINK_RSP_MsgRcvData)) begin
         string rsp_dest = get_rcv_msg_dest_name(req_desc);

         if (rsp_dest != "") begin
            message = $sformatf("%0s (DST %0s)", message, rsp_dest);
         end
      end
      return message;
   endfunction : sprint_rsp


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new();
      evl_bus_req_source_t      req_source;
      evl_etlink_neigh_source_t neigh_source;
      int                       index;
      string                    source_name;

      super.new("ETLinkParser", EVL_BUS_TYPE_ETLINK);
      if (m_etlink_bus_parser == null) begin
         m_etlink_bus_parser = this;
         evl_bus_parser::add_bus_parser(this);
      end
      if (m_source_names.size() == 0) begin
         m_source_names.push_back("Src0x0");
         req_source = evl_bus_req_source_t'(1);
         while (req_source != evl_bus_req_source_t'(0)) begin
            m_source_names.push_back($sformatf("Src0x%0x", req_source));
            req_source += evl_bus_req_source_t'(1);
         end
         neigh_source = neigh_source.first();
         while (neigh_source != neigh_source.last()) begin
            source_name  = neigh_source.name();
            index        = source_name.len() - 1;
            while (index > 0) begin
               if (source_name.getc(index) == byte'("_")) begin
                  index++;
                  break;
               end
               index--;
            end
            if (index > 0) begin
               source_name = source_name.substr(index, source_name.len() - 1);
            end
            m_source_names[int'(neigh_source)] = source_name;
            neigh_source = neigh_source.next();
         end
         source_name  = neigh_source.name();
         index        = source_name.len() - 1;
         while (index > 0) begin
            if (source_name.getc(index) == byte'("_")) begin
               index++;
               break;
            end
            index--;
         end
         if (index > 0) begin
            source_name = source_name.substr(index, source_name.len() - 1);
         end
         m_source_names[int'(neigh_source)] = source_name;
      end
      if (m_maxion_source_names.size() == 0) begin
         m_maxion_source_names.push_back("Src0x0");
         req_source = evl_bus_req_source_t'(1);
         while (req_source != evl_bus_req_source_t'(0)) begin
            m_maxion_source_names.push_back($sformatf("Src0x%0x", req_source));
            req_source += evl_bus_req_source_t'(1);
         end
         m_maxion_source_names[0] = "SC_Mem";
         m_maxion_source_names[1] = "SC_MMIO";
         m_maxion_source_names[2] = "SC_GAG";
      end
   endfunction : new

endclass : evl_etlink_bus_parser

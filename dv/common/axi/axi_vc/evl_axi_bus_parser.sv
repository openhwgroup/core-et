//--------------------------------------------------------------------------------------------------
//
// Class: evl_axi_bus_parser
//
class evl_axi_bus_parser extends evl_bus_parser;

   //-----------------------------------------------------------------------------------------------
   //
   // Static Member Variables and Objects
   //
   static evl_axi_bus_parser m_axi_bus_parser = evl_axi_bus_parser::get_inst();


   //-----------------------------------------------------------------------------------------------
   //
   // Function: evl_axi_bus_parser::get_inst
   //
   static function evl_axi_bus_parser get_inst();
      if (m_axi_bus_parser == null) begin
         evl_axi_bus_parser axi_bus_parser;

         axi_bus_parser = new();
      end
      return m_axi_bus_parser;
   endfunction : get_inst


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_bus_cmd_name
   //
   virtual function string get_bus_cmd_name(input evl_bus_cmd_t bus_cmd_in, input evl_bus_type_t bus_type_in = EVL_BUS_TYPE_UNKNOWN);
      evl_axi_req_type_t req_type;

      req_type = evl_axi_req_type_t'(bus_cmd_in);
      return $sformatf("%p", req_type);
   endfunction : get_bus_cmd_name


   //-----------------------------------------------------------------------------------------------
   //
   // Function: bus_cmd_is_read
   //
   virtual function bit bus_cmd_is_read(input evl_bus_cmd_t bus_cmd_in, input evl_bus_type_t bus_type_in = EVL_BUS_TYPE_UNKNOWN);
      evl_axi_req_type_t req_type;

      req_type = evl_axi_req_type_t'(bus_cmd_in);
      return ((req_type inside { AXI_REQ_READ }) ? 1'b1 : 1'b0);
   endfunction : bus_cmd_is_read


   //-----------------------------------------------------------------------------------------------
   //
   // Function: bus_cmd_is_write
   //
   virtual function bit bus_cmd_is_write(input evl_bus_cmd_t bus_cmd_in, input evl_bus_type_t bus_type_in = EVL_BUS_TYPE_UNKNOWN);
      evl_axi_req_type_t req_type;

      req_type = evl_axi_req_type_t'(bus_cmd_in);
      return ((req_type inside { AXI_REQ_WRITE }) ? 1'b1 : 1'b0);
   endfunction : bus_cmd_is_write


   //-----------------------------------------------------------------------------------------------
   //
   // Function: bus_cmd_is_cache_op
   //
   virtual function bit bus_cmd_is_cache_op(input evl_bus_cmd_t bus_cmd_in, input evl_bus_type_t bus_type_in = EVL_BUS_TYPE_UNKNOWN);

      return 0;
   endfunction : bus_cmd_is_cache_op


   //-----------------------------------------------------------------------------------------------
   //
   // Function: bus_cmd_is_atomic
   //
   virtual function bit bus_cmd_is_atomic(input evl_bus_cmd_t bus_cmd_in, input evl_bus_type_t bus_type_in = EVL_BUS_TYPE_UNKNOWN);

      return 0;
   endfunction : bus_cmd_is_atomic


   //-----------------------------------------------------------------------------------------------
   //
   // Function: bus_cmd_has_read_data
   //
   virtual function bit bus_cmd_has_read_data(input evl_bus_cmd_t bus_cmd_in, input evl_bus_type_t bus_type_in = EVL_BUS_TYPE_UNKNOWN);
      evl_axi_req_type_t req_type;

      req_type = evl_axi_req_type_t'(bus_cmd_in);
      return ((req_type inside { AXI_REQ_READ }) ? 1'b1 : 1'b0);
   endfunction : bus_cmd_has_read_data


   //-----------------------------------------------------------------------------------------------
   //
   // Function: bus_cmd_has_write_data
   //
   virtual function bit bus_cmd_has_write_data(input evl_bus_cmd_t bus_cmd_in, input evl_bus_type_t bus_type_in = EVL_BUS_TYPE_UNKNOWN);
      evl_axi_req_type_t req_type;

      req_type = evl_axi_req_type_t'(bus_cmd_in);
      return ((req_type inside { AXI_REQ_WRITE }) ? 1'b1 : 1'b0);
   endfunction : bus_cmd_has_write_data


   //-----------------------------------------------------------------------------------------------
   //
   // Function: convert_bus_cmd
   //
   virtual function int convert_bus_cmd(input evl_abstract_bus_cmd_t bus_cmd_in, input evl_bus_type_t bus_type_in = EVL_BUS_TYPE_UNKNOWN);
      if (bus_cmd_in inside { BusRead, BusReadCoop, BusLineRd, BusReadParent }) begin
         return int'(AXI_REQ_READ);
      end
      else if (bus_cmd_in inside { BusWrite, BusWriteAround, BusWriteAtomic, BusWriteMsg, BusLocalMsg, BusVictim, BusFlush, BusFlushToMem, BusEvict, BusEvictToMem, BusPrefetch, BusSysPrefetch }) begin
         return int'(AXI_REQ_WRITE);
      end
      return `EVL_Bus_Idle;
   endfunction : convert_bus_cmd


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_bus_rsp_name
   //
   virtual function string get_bus_rsp_name(input evl_bus_rsp_type_t bus_rsp_in, input evl_bus_type_t bus_type_in = EVL_BUS_TYPE_UNKNOWN);
      evl_axi_resp_t rsp_type;

      rsp_type = evl_axi_resp_t'(bus_rsp_in);
      return $sformatf("%p", rsp_type);
   endfunction : get_bus_rsp_name


   //-----------------------------------------------------------------------------------------------
   //
   // Function: bus_rsp_has_data
   //
   virtual function bit bus_rsp_has_data(input evl_bus_rsp_type_t bus_rsp_in, input evl_bus_type_t bus_type_in = EVL_BUS_TYPE_UNKNOWN);
      return 1;
   endfunction : bus_rsp_has_data


   //-----------------------------------------------------------------------------------------------
   //
   // Function: bus_rsp_has_error
   //
   virtual function bit bus_rsp_has_error(input evl_bus_rsp_type_t bus_rsp_in, input evl_bus_type_t bus_type_in = EVL_BUS_TYPE_UNKNOWN);
      evl_axi_resp_t rsp_type;

      rsp_type = evl_axi_resp_t'(bus_rsp_in);
      return ((rsp_type inside { AXI_RESP_SLVERR, AXI_RESP_DECERR }) ? 1'b1 : 1'b0);
   endfunction : bus_rsp_has_error


   //-----------------------------------------------------------------------------------------------
   //
   // Function: convert_bus_rsp
   //
   virtual function int convert_bus_rsp(input evl_abstract_rsp_type_t bus_rsp_in, input evl_bus_type_t bus_type_in = EVL_BUS_TYPE_UNKNOWN);
      if (bus_rsp_in == RspAck) begin
         return int'(AXI_RESP_OKAY);
      end
      else if (bus_rsp_in inside { RspWData }) begin
         return int'(AXI_RESP_OKAY);
      end
      else if (bus_rsp_in inside { RspErr }) begin
         return int'(AXI_RESP_SLVERR);
      end
      else if (bus_rsp_in inside { RspDecErr }) begin
         return int'(AXI_RESP_DECERR);
      end
      return `EVL_Rsp_Null;
   endfunction : convert_bus_rsp


   //-----------------------------------------------------------------------------------------------
   //
   // Function: sprint_obj
   //
   virtual function string sprint_obj(input evl_req_desc   req_desc,
                                      input int            style_in        = 0,
                                      input int            transfer_idx_in = -1,
                                      input evl_bus_type_t bus_type_in     = EVL_BUS_TYPE_UNKNOWN);
      int    byte_count = (1 << req_desc.get_bus_req_size()) * (req_desc.get_bus_req_len() + 1);
      string message;

      message = "";
      if (req_desc.get_vc() == ~evl_bus_req_vc_t'(0)) begin
         if (req_desc.get_bus_lock() == 1'b1) begin
            if (req_desc.get_bus_req_len() == 0) begin
               message = $sformatf("%0s 0x%x (%0d byte%0s, user 0x%x, cache 0x%x, QOS 0x%0x, locked, req ID 0x%02x, TrID 0x%x, id %0d%0s)%0s", req_desc.get_bus_cmd_name(), req_desc.get_paddr(), byte_count, ((byte_count == 1) ? "" : "s"), req_desc.get_bus_req_user(), req_desc.get_bus_cache_attr(), req_desc.get_bus_qos(), req_desc.get_bus_req_id(), req_desc.get_trans_id(), req_desc.get_inst_id(), ((req_desc.is_done() == 1'b1) ? ", done" : ""), ((req_desc.is_bus_child() == 1'b1) ? "*" : ""));
            end
            else begin
               message = $sformatf("%0s 0x%x (%0d byte%0s (%0d bursts), user 0x%x, cache 0x%x, QOS 0x%0x, locked, req ID 0x%02x, TrID 0x%x, id %0d%0s)%0s", req_desc.get_bus_cmd_name(), req_desc.get_paddr(), byte_count, ((byte_count == 1) ? "" : "s"), req_desc.get_bus_req_len() + 1, req_desc.get_bus_req_user(), req_desc.get_bus_cache_attr(), req_desc.get_bus_qos(), req_desc.get_bus_req_id(), req_desc.get_trans_id(), req_desc.get_inst_id(), ((req_desc.is_done() == 1'b1) ? ", done" : ""), ((req_desc.is_bus_child() == 1'b1) ? "*" : ""));
            end
         end
         else begin
            if (req_desc.get_bus_req_len() == 0) begin
               message = $sformatf("%0s 0x%x (%0d byte%0s, user 0x%x, cache 0x%x, QOS 0x%0x, req ID 0x%02x, TrID 0x%x, id %0d%0s)%0s", req_desc.get_bus_cmd_name(), req_desc.get_paddr(), byte_count, ((byte_count == 1) ? "" : "s"), req_desc.get_bus_req_user(), req_desc.get_bus_cache_attr(), req_desc.get_bus_qos(), req_desc.get_bus_req_id(), req_desc.get_trans_id(), req_desc.get_inst_id(), ((req_desc.is_done() == 1'b1) ? ", done" : ""), ((req_desc.is_bus_child() == 1'b1) ? "*" : ""));
            end
            else begin
               message = $sformatf("%0s 0x%x (%0d byte%0s (%0d bursts), user 0x%x, cache 0x%x, QOS 0x%0x, req ID 0x%02x, TrID 0x%x, id %0d%0s)%0s", req_desc.get_bus_cmd_name(), req_desc.get_paddr(), byte_count, ((byte_count == 1) ? "" : "s"), req_desc.get_bus_req_len() + 1, req_desc.get_bus_req_user(), req_desc.get_bus_cache_attr(), req_desc.get_bus_qos(), req_desc.get_bus_req_id(), req_desc.get_trans_id(), req_desc.get_inst_id(), ((req_desc.is_done() == 1'b1) ? ", done" : ""), ((req_desc.is_bus_child() == 1'b1) ? "*" : ""));
            end
         end
      end
      else begin
         if (req_desc.get_bus_lock() == 1'b1) begin
            if (req_desc.get_bus_req_len() == 0) begin
               message = $sformatf("%0s 0x%x (VC %0d, %0d byte%0s, user 0x%x, cache 0x%x, QOS 0x%0x, locked, req ID 0x%02x, TrID 0x%x, id %0d%0s)%0s", req_desc.get_bus_cmd_name(), req_desc.get_paddr(), req_desc.get_vc(), byte_count, ((byte_count == 1) ? "" : "s"), req_desc.get_bus_req_user(), req_desc.get_bus_cache_attr(), req_desc.get_bus_qos(), req_desc.get_bus_req_id(), req_desc.get_trans_id(), req_desc.get_inst_id(), ((req_desc.is_done() == 1'b1) ? ", done" : ""), ((req_desc.is_bus_child() == 1'b1) ? "*" : ""));
            end
            else begin
               message = $sformatf("%0s 0x%x (VC %0d, %0d byte%0s (%0d bursts), user 0x%x, cache 0x%x, QOS 0x%0x, locked, req ID 0x%02x, TrID 0x%x, id %0d%0s)%0s", req_desc.get_bus_cmd_name(), req_desc.get_paddr(), req_desc.get_vc(), byte_count, ((byte_count == 1) ? "" : "s"), req_desc.get_bus_req_len() + 1, req_desc.get_bus_req_user(), req_desc.get_bus_cache_attr(), req_desc.get_bus_qos(), req_desc.get_bus_req_id(), req_desc.get_trans_id(), req_desc.get_inst_id(), ((req_desc.is_done() == 1'b1) ? ", done" : ""), ((req_desc.is_bus_child() == 1'b1) ? "*" : ""));
            end
         end
         else begin
            if (req_desc.get_bus_req_len() == 0) begin
               message = $sformatf("%0s 0x%x (VC %0d, %0d byte%0s, user 0x%x, cache 0x%x, QOS 0x%0x, req ID 0x%02x, TrID 0x%x, id %0d%0s)%0s", req_desc.get_bus_cmd_name(), req_desc.get_paddr(), req_desc.get_vc(), byte_count, ((byte_count == 1) ? "" : "s"), req_desc.get_bus_req_user(), req_desc.get_bus_cache_attr(), req_desc.get_bus_qos(), req_desc.get_bus_req_id(), req_desc.get_trans_id(), req_desc.get_inst_id(), ((req_desc.is_done() == 1'b1) ? ", done" : ""), ((req_desc.is_bus_child() == 1'b1) ? "*" : ""));
            end
            else begin
               message = $sformatf("%0s 0x%x (VC %0d, %0d byte%0s (%0d bursts), user 0x%x, cache 0x%x, QOS 0x%0x, req ID 0x%02x, TrID 0x%x, id %0d%0s)%0s", req_desc.get_bus_cmd_name(), req_desc.get_paddr(), req_desc.get_vc(), byte_count, ((byte_count == 1) ? "" : "s"), req_desc.get_bus_req_len() + 1, req_desc.get_bus_req_user(), req_desc.get_bus_cache_attr(), req_desc.get_bus_qos(), req_desc.get_bus_req_id(), req_desc.get_trans_id(), req_desc.get_inst_id(), ((req_desc.is_done() == 1'b1) ? ", done" : ""), ((req_desc.is_bus_child() == 1'b1) ? "*" : ""));
            end
         end
      end
      if ((req_desc.bus_cmd_has_write_data() == 1'b1) && (style_in == 1)) begin
         message = $sformatf("%0s <- 0x%0s", message, req_desc.format_req_data());
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
      if (req_desc.bus_cmd_has_read_data() == 1'b1) begin
         message = $sformatf("%0s 0x%0s", message, req_desc.format_rsp_data(dword_start, dword_end));
      end
      return message;
   endfunction : sprint_rsp


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new();
      super.new("AxiParser", EVL_BUS_TYPE_AXI);
      if (m_axi_bus_parser == null) begin
         m_axi_bus_parser = this;
         evl_bus_parser::add_bus_parser(this);
      end
   endfunction : new

endclass : evl_axi_bus_parser

//--------------------------------------------------------------------------------------------------
//
// Class: evl_apb_bus_parser
//
class evl_apb_bus_parser extends evl_bus_parser;

   //-----------------------------------------------------------------------------------------------
   //
   // Static Member Variables and Objects
   //
   static evl_apb_bus_parser m_apb_bus_parser = evl_apb_bus_parser::get_inst();


   //-----------------------------------------------------------------------------------------------
   //
   // Function: evl_apb_bus_parser::get_inst
   //
   static function evl_apb_bus_parser get_inst();
      if (m_apb_bus_parser == null) begin
         evl_apb_bus_parser apb_bus_parser;

         apb_bus_parser = new();
      end
      return m_apb_bus_parser;
   endfunction : get_inst


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_bus_cmd_name
   //
   virtual function string get_bus_cmd_name(input evl_bus_cmd_t bus_cmd_in, input evl_bus_type_t bus_type_in = EVL_BUS_TYPE_UNKNOWN);
      evl_apb_req_type_t req_type;

      req_type = evl_apb_req_type_t'(bus_cmd_in);
      return $sformatf("%p", req_type);
   endfunction : get_bus_cmd_name


   //-----------------------------------------------------------------------------------------------
   //
   // Function: bus_cmd_is_read
   //
   virtual function bit bus_cmd_is_read(input evl_bus_cmd_t bus_cmd_in, input evl_bus_type_t bus_type_in = EVL_BUS_TYPE_UNKNOWN);
      evl_apb_req_type_t req_type;

      req_type = evl_apb_req_type_t'(bus_cmd_in);
      return ((req_type inside { APB_REQ_READ }) ? 1'b1 : 1'b0);
   endfunction : bus_cmd_is_read


   //-----------------------------------------------------------------------------------------------
   //
   // Function: bus_cmd_is_write
   //
   virtual function bit bus_cmd_is_write(input evl_bus_cmd_t bus_cmd_in, input evl_bus_type_t bus_type_in = EVL_BUS_TYPE_UNKNOWN);
      evl_apb_req_type_t req_type;

      req_type = evl_apb_req_type_t'(bus_cmd_in);
      return ((req_type inside { APB_REQ_WRITE }) ? 1'b1 : 1'b0);
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
      evl_apb_req_type_t req_type;

      req_type = evl_apb_req_type_t'(bus_cmd_in);
      return ((req_type inside { APB_REQ_READ }) ? 1'b1 : 1'b0);
   endfunction : bus_cmd_has_read_data


   //-----------------------------------------------------------------------------------------------
   //
   // Function: bus_cmd_has_write_data
   //
   virtual function bit bus_cmd_has_write_data(input evl_bus_cmd_t bus_cmd_in, input evl_bus_type_t bus_type_in = EVL_BUS_TYPE_UNKNOWN);
      evl_apb_req_type_t req_type;

      req_type = evl_apb_req_type_t'(bus_cmd_in);
      return ((req_type inside { APB_REQ_WRITE }) ? 1'b1 : 1'b0);
   endfunction : bus_cmd_has_write_data


   //-----------------------------------------------------------------------------------------------
   //
   // Function: convert_bus_cmd
   //
   virtual function int convert_bus_cmd(input evl_abstract_bus_cmd_t bus_cmd_in, input evl_bus_type_t bus_type_in = EVL_BUS_TYPE_UNKNOWN);
      if (bus_cmd_in inside { BusRead, BusReadParent }) begin
         return int'(APB_REQ_READ);
      end
      else if (bus_cmd_in inside { BusWrite, BusWriteAtomic, BusVictim, BusFlush, BusFlushToMem, BusEvict, BusEvictToMem, BusPrefetch, BusSysPrefetch }) begin
         return int'(APB_REQ_WRITE);
      end
      return `EVL_Bus_Idle;
   endfunction : convert_bus_cmd


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_bus_rsp_name
   //
   virtual function string get_bus_rsp_name(input evl_bus_rsp_type_t bus_rsp_in, input evl_bus_type_t bus_type_in = EVL_BUS_TYPE_UNKNOWN);
      evl_apb_resp_type_t rsp_type;

      rsp_type = evl_apb_resp_type_t'(bus_rsp_in);
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
      evl_apb_resp_type_t rsp_type;

      rsp_type = evl_apb_resp_type_t'(bus_rsp_in);
      return ((rsp_type inside { APB_RESP_SLVERR}) ? 1'b1 : 1'b0);
   endfunction : bus_rsp_has_error


   //-----------------------------------------------------------------------------------------------
   //
   // Function: convert_bus_rsp
   //
   virtual function int convert_bus_rsp(input evl_abstract_rsp_type_t bus_rsp_in, input evl_bus_type_t bus_type_in = EVL_BUS_TYPE_UNKNOWN);
      if (bus_rsp_in == RspAck) begin
         return int'(APB_RESP_OKAY);
      end
      else if (bus_rsp_in inside { RspWData }) begin
         return int'(APB_RESP_OKAY);
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
      string message;

      message = $sformatf("%0s 0x%x (TrID 0x%x, id %0d%0s)", req_desc.get_bus_cmd_name(), req_desc.get_paddr(), req_desc.get_trans_id(), req_desc.get_inst_id(), ((req_desc.is_done() == 1'b1) ? ", done" : ""));
      if ((req_desc.bus_cmd_has_write_data() == 1'b1) && (style_in == 1)) begin
         int byte_count;
         int dword_start;
         int dword_end;

         byte_count  = 1 << req_desc.get_bus_req_size();
         dword_start = (int'(req_desc.get_paddr()) & (`EVL_LINE_BYTES - 1) & (~(byte_count - 1))) >> `EVL_DWORD_ADDR_LSB;
         dword_end   = dword_start + ((byte_count + `EVL_DWORD_BYTES - 1) >> `EVL_DWORD_ADDR_LSB) - 1;
         message = $sformatf("%0s <- 0x%0s", message, req_desc.format_req_data(dword_start, dword_end));
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
      int    byte_count;
      int    dword_offset_start;
      int    dword_offset_end;
      string message;

      byte_count         = 1 << req_desc.get_bus_req_size();
      dword_offset_start = (int'(req_desc.get_paddr()) & (`EVL_LINE_BYTES - 1) & (~(byte_count - 1))) >> `EVL_DWORD_ADDR_LSB;
      dword_offset_end   = dword_offset_start + ((byte_count + `EVL_DWORD_BYTES - 1) >> `EVL_DWORD_ADDR_LSB) - 1;
      if (req_desc.bus_cmd_has_write_data() == 1'b1) begin
         message = $sformatf("W Data 0x%0s", req_desc.format_req_data(dword_offset_start, dword_offset_end));
      end
      else begin
         message = $sformatf("R Data 0x%0s", req_desc.format_rsp_data(dword_offset_start, dword_offset_end));
      end
      return message;
   endfunction : sprint_rsp


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new();
      super.new("ApbParser", EVL_BUS_TYPE_APB);
      if (m_apb_bus_parser == null) begin
         m_apb_bus_parser = this;
         evl_bus_parser::add_bus_parser(this);
      end
   endfunction : new

endclass : evl_apb_bus_parser


//--------------------------------------------------------------------------------------------------
//
// Class: evl_jtag_bus_parser
//
class evl_jtag_bus_parser extends evl_bus_parser;

   //-----------------------------------------------------------------------------------------------
   //
   // Static Member Variables and Objects
   //
   static evl_jtag_bus_parser m_jtag_bus_parser = evl_jtag_bus_parser::get_inst();


   //--------------------------------------------------------------------------------------------------
   //
   // Function: evl_jtag_bus_parser::get_inst
   //
   static function evl_jtag_bus_parser get_inst();
      if (m_jtag_bus_parser == null) begin
         evl_jtag_bus_parser jtag_bus_parser;

         jtag_bus_parser = new();
      end
      return m_jtag_bus_parser;
   endfunction : get_inst


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_bus_cmd_name
   //
   virtual function string get_bus_cmd_name(input evl_bus_cmd_t bus_cmd_in, input evl_bus_type_t bus_type_in = EVL_BUS_TYPE_UNKNOWN);
      evl_cmd_t req_type;

      req_type = evl_cmd_t'(bus_cmd_in);
      return $sformatf("%p", req_type);
   endfunction : get_bus_cmd_name


   //-----------------------------------------------------------------------------------------------
   //
   // Function: bus_cmd_is_read
   //
   virtual function bit bus_cmd_is_read(input evl_bus_cmd_t bus_cmd_in, input evl_bus_type_t bus_type_in = EVL_BUS_TYPE_UNKNOWN);
      return 1'b1;
   endfunction : bus_cmd_is_read


   //-----------------------------------------------------------------------------------------------
   //
   // Function: bus_cmd_is_write
   //
   virtual function bit bus_cmd_is_write(input evl_bus_cmd_t bus_cmd_in, input evl_bus_type_t bus_type_in = EVL_BUS_TYPE_UNKNOWN);
      return 1'b1;
   endfunction : bus_cmd_is_write


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
      return 1'b0;
   endfunction : bus_cmd_is_atomic


   //-----------------------------------------------------------------------------------------------
   //
   // Function: bus_cmd_has_read_data
   //
   virtual function bit bus_cmd_has_read_data(input evl_bus_cmd_t bus_cmd_in, input evl_bus_type_t bus_type_in = EVL_BUS_TYPE_UNKNOWN);
      return 1'b1;
   endfunction : bus_cmd_has_read_data


   //-----------------------------------------------------------------------------------------------
   //
   // Function: bus_cmd_has_write_data
   //
   virtual function bit bus_cmd_has_write_data(input evl_bus_cmd_t bus_cmd_in, input evl_bus_type_t bus_type_in = EVL_BUS_TYPE_UNKNOWN);
      return 1'b1;
   endfunction : bus_cmd_has_write_data


   //-----------------------------------------------------------------------------------------------
   //
   // Function: convert_bus_cmd
   //
   virtual function int convert_bus_cmd(input evl_abstract_bus_cmd_t bus_cmd_in, input evl_bus_type_t bus_type_in = EVL_BUS_TYPE_UNKNOWN);
      if (bus_cmd_in inside { BusRead, BusWrite }) begin
         return int'(JTAG_WR);
      end
      return `EVL_Bus_Idle;
   endfunction : convert_bus_cmd


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_bus_rsp_name
   //
   virtual function string get_bus_rsp_name(input evl_bus_rsp_type_t bus_rsp_in, input evl_bus_type_t bus_type_in = EVL_BUS_TYPE_UNKNOWN);
      return "Scan Data";
   endfunction : get_bus_rsp_name


   //-----------------------------------------------------------------------------------------------
   //
   // Function: bus_rsp_has_data
   //
   virtual function bit bus_rsp_has_data(input evl_bus_rsp_type_t bus_rsp_in, input evl_bus_type_t bus_type_in = EVL_BUS_TYPE_UNKNOWN);
      return 1'b1;
   endfunction : bus_rsp_has_data


   //-----------------------------------------------------------------------------------------------
   //
   // Function: bus_rsp_has_error
   //
   virtual function bit bus_rsp_has_error(input evl_bus_rsp_type_t bus_rsp_in, input evl_bus_type_t bus_type_in = EVL_BUS_TYPE_UNKNOWN);
      return 1'b0;
   endfunction : bus_rsp_has_error


   //-----------------------------------------------------------------------------------------------
   //
   // Function: sprint_obj
   //
   virtual function string sprint_obj(input evl_req_desc   req_desc,
                                      input int            style_in        = 0,
                                      input int            transfer_idx_in = -1,
                                      input evl_bus_type_t bus_type_in     = EVL_BUS_TYPE_UNKNOWN);
      evl_cmd_t cmd;

      cmd = evl_cmd_t'(req_desc.get_bus_cmd());
      if (cmd inside { JTAG_RESET, JTAG_RUN_TEST }) begin
         return $sformatf("%p (id %0d)", cmd, req_desc.get_inst_id());
      end
      else if (cmd inside { JTAG_STEP }) begin
         return $sformatf("%0p tms:%0d tdo:%0d (id %0d)", cmd, req_desc.get_paddr() & evl_paddr_t'(1), req_desc.get_req_xword(0) & evl_xword_t'(1), req_desc.get_inst_id());
      end
      else if (cmd inside { JTAG_RD, JTAG_RD_PARTIAL }) begin
         return $sformatf("%0p %0d (id %0d)", cmd, req_desc.get_paddr(), req_desc.get_inst_id());
      end
      return $sformatf("%0p %0d 0x%x (id %0d)", cmd, req_desc.get_paddr(), req_desc.get_req_xword(0), req_desc.get_inst_id());
   endfunction : sprint_obj


   //-----------------------------------------------------------------------------------------------
   //
   // Function: sprint_rsp
   //
   virtual function string sprint_rsp(input evl_req_desc   req_desc,
                                      input int            dword_start = 0,
                                      input int            dword_end   = `EVL_LINE_DWORDS - 1,
                                      input evl_bus_type_t bus_type_in = EVL_BUS_TYPE_UNKNOWN);
//    string message;

//    message = $sformatf("%0s", req_desc.get_bus_rsp_name());
//    if (req_desc.bus_cmd_has_read_data() == 1'b1) begin
//       message = $sformatf("%0s 0x%0s", message, req_desc.format_rsp_data(dword_start, dword_end));
//    end
//    return message;
   endfunction : sprint_rsp


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new();
      super.new("JtagParser", EVL_BUS_TYPE_JTAG);
      if (m_jtag_bus_parser == null) begin
         m_jtag_bus_parser = this;
         evl_bus_parser::add_bus_parser(this);
      end
   endfunction : new

endclass : evl_jtag_bus_parser

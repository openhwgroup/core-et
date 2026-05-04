//--------------------------------------------------------------------------------------------------
//
// Class: evl_dbg_noc_bus_parser
//
class evl_dbg_noc_bus_parser extends evl_bus_parser;

   //-----------------------------------------------------------------------------------------------
   //
   // Static Member Variables and Objects
   //
   static evl_dbg_noc_bus_parser m_dbg_noc_bus_parser = evl_dbg_noc_bus_parser::get_inst();


   //--------------------------------------------------------------------------------------------------
   //
   // Function: evl_dbg_noc_bus_parser::get_inst
   //
   static function evl_dbg_noc_bus_parser get_inst();
      if (m_dbg_noc_bus_parser == null) begin
         evl_dbg_noc_bus_parser dbg_noc_bus_parser;

         dbg_noc_bus_parser = new();
      end
      return m_dbg_noc_bus_parser;
   endfunction : get_inst


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_bus_cmd_name
   //
   virtual function string get_bus_cmd_name(input evl_bus_cmd_t bus_cmd_in, input evl_bus_type_t bus_type_in = EVL_BUS_TYPE_UNKNOWN);
      return $sformatf("DBG_NOC_WRITE");
   endfunction : get_bus_cmd_name


   //----------------------------------------------------------------------------------------------
   //
   // Function: bus_cmd_is_read
   //
   virtual function bit bus_cmd_is_read(input evl_bus_cmd_t bus_cmd_in, input evl_bus_type_t bus_type_in = EVL_BUS_TYPE_UNKNOWN);

      return 1'b0;
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

      return 0;
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
      return `EVL_Bus_Idle;
   endfunction : convert_bus_cmd


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_bus_rsp_name
   //
   virtual function string get_bus_rsp_name(input evl_bus_rsp_type_t bus_rsp_in, input evl_bus_type_t bus_type_in = EVL_BUS_TYPE_UNKNOWN);
      return $sformatf("0x%x", bus_rsp_in);
   endfunction : get_bus_rsp_name


   //-----------------------------------------------------------------------------------------------
   //
   // Function: bus_rsp_has_data
   //
   virtual function bit bus_rsp_has_data(input evl_bus_rsp_type_t bus_rsp_in, input evl_bus_type_t bus_type_in = EVL_BUS_TYPE_UNKNOWN);
      return 0;
   endfunction : bus_rsp_has_data


   //-----------------------------------------------------------------------------------------------
   //
   // Function: bus_rsp_has_error
   //
   virtual function bit bus_rsp_has_error(input evl_bus_rsp_type_t bus_rsp_in, input evl_bus_type_t bus_type_in = EVL_BUS_TYPE_UNKNOWN);
      return 0;
   endfunction : bus_rsp_has_error


   //-----------------------------------------------------------------------------------------------
   //
   // Function: convert_bus_rsp
   //
   virtual function int convert_bus_rsp(input evl_abstract_rsp_type_t bus_rsp_in, input evl_bus_type_t bus_type_in = EVL_BUS_TYPE_UNKNOWN);
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

      message = $sformatf("0x%x (%0d byte%0s, Dest Int 0x%0x, QOS 0x%0x, TrID 0x%x, id %0d%0s)%0s", req_desc.get_paddr(), byte_count, ((byte_count == 1) ? "" : "s"), req_desc.get_vc(), req_desc.get_bus_qos(), req_desc.get_trans_id(), req_desc.get_inst_id(), ((req_desc.is_done() == 1'b1) ? ", done" : ""), ((req_desc.is_bus_child() == 1'b1) ? "*" : ""));
      message = $sformatf("%0s <- 0x%0s", message, req_desc.format_req_data());
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

      return "???";
   endfunction : sprint_rsp


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new();
      super.new("DBGNoCParser", EVL_BUS_TYPE_DBG_NOC);
      if (m_dbg_noc_bus_parser == null) begin
         m_dbg_noc_bus_parser = this;
         evl_bus_parser::add_bus_parser(this);
      end
   endfunction : new

endclass : evl_dbg_noc_bus_parser

//--------------------------------------------------------------------------------------------------
//
// Class: evl_bus_parser
//
// This class is used to support bus-agnostic request descriptors.  What this means is that bus
// commands and bus response types are simply bit vectors (rather than enumerated types).  Specific
// checking (like "is this bus command a read" or "is this bus command a write") is performed in an
// extended bus parser class for that specific bus type.  Each request descriptor has a variable
// with its bus type, and that information is used to select the appropriate bus parser to use.
//
// This class (evl_bus_parser) is the root of all bus parsers and is a singleton.  Each bus
// verification component has a companion bus parser class; when the bus verification package is
// loaded, an instance of its companion bus parser class is added as a child to this singleton:
//
//    evl_bus_parser                evl_etlink_bus_parser
//    +----------------------+      +----------------------+
//    |                      |      |                      |
//    | m_bus_parsers[] -+-->|----->|                      |
//    |                  |   |      |                      |
//    |                  +-->|--+   +----------------------+
//    |                      |  |
//    +----------------------+  |   evl_axi_bus_parser
//                              |   +----------------------+
//                              |   |                      |
//                              +-->|                      |
//                                  |                      |
//                                  +----------------------+
//
// A request descriptor can always get access to its bus parser either through a handle to this
// singleton, or through a handle to its bus parser.
//
class evl_bus_parser extends evl_void;

   //-----------------------------------------------------------------------------------------------
   //
   // Static Member Variables and Objects
   //
   // m_bus_parser:       This is a handle the singleton of this class.
   //                     --------------------------------------------------------------------------
   // m_bus_parsers[]:    This associative array is an array of bus parsers indexed by bus type.  It
   //                     is intended to be filled out by the root bus parser.
   //
   static evl_bus_parser m_bus_parser = evl_bus_parser::get_inst();
   static evl_bus_parser m_bus_parsers[evl_bus_type_t];


   //-----------------------------------------------------------------------------------------------
   //
   // Member Variables and Objects
   //
   // m_bus_type: This variable is the bus type for this bus parser.
   //
   evl_bus_type_t m_bus_type;


   //-----------------------------------------------------------------------------------------------
   //
   // Static Function: evl_bus_parser::get_inst
   //
   // This static function is used to get a handle to the singleton instance of the root bus
   // parser.
   //
   // Return Value:
   //
   // evl_bus_parser: This is a handle to the singleton instance of the root bus parser.
   //
   static function evl_bus_parser get_inst();
      if (m_bus_parser == null) begin
         evl_bus_parser bus_parser;

         bus_parser = new();
      end
      return m_bus_parser;
   endfunction : get_inst


   //-----------------------------------------------------------------------------------------------
   //
   // Static Function: evl_bus_parser::add_bus_parser
   //
   // This static function is used to add a bus-specific bus parser handle to the list of bus
   // parsers in the singleton instance of the root bus parser.
   //
   // Input Variables:
   //
   // bus_parser_in: This is handle to the bus parser to be added.
   //
   static function void add_bus_parser(input evl_bus_parser bus_parser_in);
      if (m_bus_parser == null) begin
         void'(evl_bus_parser::get_inst());
      end
      if ((bus_parser_in == null) || (bus_parser_in.get_bus_type() == EVL_BUS_TYPE_UNKNOWN)) begin
         return;
      end
      m_bus_parsers[bus_parser_in.get_bus_type()] = bus_parser_in;
   endfunction : add_bus_parser


   //-----------------------------------------------------------------------------------------------
   //
   // Function: add_bus_parser_local
   //
   function void add_bus_parser_local(input evl_bus_parser bus_parser_in);
      if ((bus_parser_in == null) || (bus_parser_in.get_bus_type() == EVL_BUS_TYPE_UNKNOWN)) begin
         return;
      end
      m_bus_parsers[bus_parser_in.get_bus_type()] = bus_parser_in;
   endfunction : add_bus_parser_local


   //-----------------------------------------------------------------------------------------------
   //
   // Static Function: evl_bus_parser::get_bus_parser
   //
   static function evl_bus_parser get_bus_parser(input evl_bus_type_t bus_type_in);
      if (m_bus_parser == null) begin
         void'(evl_bus_parser::get_inst());
      end
      if (!(m_bus_parsers.exists(bus_type_in))) begin
         return m_bus_parser;
      end
      return m_bus_parsers[bus_type_in];
   endfunction : get_bus_parser


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_bus_parser_local
   //
   function evl_bus_parser get_bus_parser_local(input evl_bus_type_t bus_type_in);
      if (!(m_bus_parsers.exists(bus_type_in))) begin
         return this;
      end
      return m_bus_parsers[bus_type_in];
   endfunction : get_bus_parser_local


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_bus_type
   //
   function evl_bus_type_t get_bus_type();
      return m_bus_type;
   endfunction : get_bus_type


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_bus_cmd_name
   //
   virtual function string get_bus_cmd_name(input evl_bus_cmd_t bus_cmd_in, input evl_bus_type_t bus_type_in = EVL_BUS_TYPE_UNKNOWN);
      if (m_bus_parsers.exists(bus_type_in)) begin
         return m_bus_parsers[bus_type_in].get_bus_cmd_name(bus_cmd_in);
      end
      return $sformatf("0x%x", bus_cmd_in);
   endfunction : get_bus_cmd_name


   //-----------------------------------------------------------------------------------------------
   //
   // Function: bus_cmd_requires_rsp
   //
   virtual function bit bus_cmd_requires_rsp(input evl_bus_cmd_t bus_cmd_in, input evl_bus_type_t bus_type_in = EVL_BUS_TYPE_UNKNOWN);
      if (m_bus_parsers.exists(bus_type_in)) begin
         return m_bus_parsers[bus_type_in].bus_cmd_requires_rsp(bus_cmd_in);
      end
      return 1'b1;
   endfunction : bus_cmd_requires_rsp


   //-----------------------------------------------------------------------------------------------
   //
   // Function: bus_cmd_is_read
   //
   virtual function bit bus_cmd_is_read(input evl_bus_cmd_t bus_cmd_in, input evl_bus_type_t bus_type_in = EVL_BUS_TYPE_UNKNOWN);
      if (m_bus_parsers.exists(bus_type_in)) begin
         return m_bus_parsers[bus_type_in].bus_cmd_is_read(bus_cmd_in);
      end
      return 1'b0;
   endfunction : bus_cmd_is_read


   //-----------------------------------------------------------------------------------------------
   //
   // Function: bus_cmd_is_write
   //
   virtual function bit bus_cmd_is_write(input evl_bus_cmd_t bus_cmd_in, input evl_bus_type_t bus_type_in = EVL_BUS_TYPE_UNKNOWN);
      if (m_bus_parsers.exists(bus_type_in)) begin
         return m_bus_parsers[bus_type_in].bus_cmd_is_write(bus_cmd_in);
      end
      return 1'b0;
   endfunction : bus_cmd_is_write


   //-----------------------------------------------------------------------------------------------
   //
   // Function: bus_cmd_is_cache_op
   //
   virtual function bit bus_cmd_is_cache_op(input evl_bus_cmd_t bus_cmd_in, input evl_bus_type_t bus_type_in = EVL_BUS_TYPE_UNKNOWN);
      if (m_bus_parsers.exists(bus_type_in)) begin
         return m_bus_parsers[bus_type_in].bus_cmd_is_cache_op(bus_cmd_in);
      end
      return 1'b0;
   endfunction : bus_cmd_is_cache_op


   //-----------------------------------------------------------------------------------------------
   //
   // Function: bus_cmd_is_scratchpad
   //
   virtual function bit bus_cmd_is_scratchpad(input evl_bus_cmd_t bus_cmd_in, input evl_bus_type_t bus_type_in = EVL_BUS_TYPE_UNKNOWN);
      if (m_bus_parsers.exists(bus_type_in)) begin
         return m_bus_parsers[bus_type_in].bus_cmd_is_scratchpad(bus_cmd_in);
      end
      return 1'b0;
   endfunction : bus_cmd_is_scratchpad


   //-----------------------------------------------------------------------------------------------
   //
   // Function: bus_cmd_is_atomic
   //
   virtual function bit bus_cmd_is_atomic(input evl_bus_cmd_t bus_cmd_in, input evl_bus_type_t bus_type_in = EVL_BUS_TYPE_UNKNOWN);
      if (m_bus_parsers.exists(bus_type_in)) begin
         return m_bus_parsers[bus_type_in].bus_cmd_is_atomic(bus_cmd_in);
      end
      return 1'b0;
   endfunction : bus_cmd_is_atomic


   //-----------------------------------------------------------------------------------------------
   //
   // Function: bus_cmd_is_acquire
   //
   virtual function bit bus_cmd_is_acquire(input evl_bus_cmd_t bus_cmd_in, input evl_bus_type_t bus_type_in = EVL_BUS_TYPE_UNKNOWN);
      if (m_bus_parsers.exists(bus_type_in)) begin
         return m_bus_parsers[bus_type_in].bus_cmd_is_acquire(bus_cmd_in);
      end
      return 1'b0;
   endfunction : bus_cmd_is_acquire


   //-----------------------------------------------------------------------------------------------
   //
   // Function: bus_cmd_has_read_data
   //
   virtual function bit bus_cmd_has_read_data(input evl_bus_cmd_t bus_cmd_in, input evl_bus_type_t bus_type_in = EVL_BUS_TYPE_UNKNOWN);
      if (m_bus_parsers.exists(bus_type_in)) begin
         return m_bus_parsers[bus_type_in].bus_cmd_has_read_data(bus_cmd_in);
      end
      return 1'b0;
   endfunction : bus_cmd_has_read_data


   //-----------------------------------------------------------------------------------------------
   //
   // Function: bus_cmd_has_write_data
   //
   virtual function bit bus_cmd_has_write_data(input evl_bus_cmd_t bus_cmd_in, input evl_bus_type_t bus_type_in = EVL_BUS_TYPE_UNKNOWN);
      if (m_bus_parsers.exists(bus_type_in)) begin
         return m_bus_parsers[bus_type_in].bus_cmd_has_write_data(bus_cmd_in);
      end
      return 1'b0;
   endfunction : bus_cmd_has_write_data


   //-----------------------------------------------------------------------------------------------
   //
   // Function: bus_cmd_has_unique_tag
   //
   virtual function bit bus_cmd_has_unique_tag(input evl_bus_cmd_t bus_cmd_in, input evl_bus_type_t bus_type_in = EVL_BUS_TYPE_UNKNOWN);
      if (m_bus_parsers.exists(bus_type_in)) begin
         return m_bus_parsers[bus_type_in].bus_cmd_has_unique_tag(bus_cmd_in);
      end
      return 1'b1;
   endfunction : bus_cmd_has_unique_tag


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_bus_rsp_name
   //
   virtual function string get_bus_rsp_name(input evl_bus_rsp_type_t bus_rsp_in, input evl_bus_type_t bus_type_in = EVL_BUS_TYPE_UNKNOWN);
      if (m_bus_parsers.exists(bus_type_in)) begin
         return m_bus_parsers[bus_type_in].get_bus_rsp_name(bus_rsp_in);
      end
      return $sformatf("0x%x", bus_rsp_in);
   endfunction : get_bus_rsp_name


   //-----------------------------------------------------------------------------------------------
   //
   // Function: bus_rsp_has_data
   //
   virtual function bit bus_rsp_has_data(input evl_bus_rsp_type_t bus_rsp_in, input evl_bus_type_t bus_type_in = EVL_BUS_TYPE_UNKNOWN);
      if (m_bus_parsers.exists(bus_type_in)) begin
         return m_bus_parsers[bus_type_in].bus_rsp_has_data(bus_rsp_in);
      end
      return 1'b0;
   endfunction : bus_rsp_has_data


   //-----------------------------------------------------------------------------------------------
   //
   // Function: bus_rsp_has_error
   //
   virtual function bit bus_rsp_has_error(input evl_bus_rsp_type_t bus_rsp_in, input evl_bus_type_t bus_type_in = EVL_BUS_TYPE_UNKNOWN);
      if (m_bus_parsers.exists(bus_type_in)) begin
         return m_bus_parsers[bus_type_in].bus_rsp_has_error(bus_rsp_in);
      end
      return 1'b0;
   endfunction : bus_rsp_has_error


   //-----------------------------------------------------------------------------------------------
   //
   // Function: convert_bus_cmd
   //
   virtual function int convert_bus_cmd(input evl_abstract_bus_cmd_t bus_cmd_in, input evl_bus_type_t bus_type_in = EVL_BUS_TYPE_UNKNOWN);
      return `EVL_Bus_Idle;
   endfunction : convert_bus_cmd


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
      if (m_bus_parsers.exists(bus_type_in)) begin
         return m_bus_parsers[bus_type_in].sprint_obj(req_desc, style_in, transfer_idx_in, bus_type_in);
      end
      return "???";
   endfunction : sprint_obj


   //-----------------------------------------------------------------------------------------------
   //
   // Function: sprint_rsp
   //
   virtual function string sprint_rsp(input evl_req_desc   req_desc,
                                      input int            dword_start = 0,
                                      input int            dword_end   = `EVL_LINE_DWORDS - 1,
                                      input evl_bus_type_t bus_type_in = EVL_BUS_TYPE_UNKNOWN);
      if (m_bus_parsers.exists(bus_type_in)) begin
         return m_bus_parsers[bus_type_in].sprint_rsp(req_desc, dword_start, dword_end, bus_type_in);
      end
      return "???";
   endfunction : sprint_rsp


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   protected function new(input string name_in = "BusParser", input evl_bus_type_t bus_type = EVL_BUS_TYPE_UNKNOWN);
      super.new(name_in);
      m_bus_type = bus_type;
      if (bus_type == EVL_BUS_TYPE_UNKNOWN) begin
         if (m_bus_parser == null) begin
            m_bus_parser = this;
            void'(evl_base_pkg::evl_log_create());
         end
         else begin
            `dut_error(get_abstract_name(), $sformatf("a new copy of %0s has been created (id %0d ... singleton id %0d)", "evl_bus_parser", get_inst_id(), m_bus_parser.get_inst_id()))
         end
      end
      else begin
         if (m_bus_parser == null) begin
            void'(evl_bus_parser::get_inst());
            void'(evl_base_pkg::evl_log_create());
         end
      end
      `evl_log(UVM_HIGH, VMOD_CONFIG, get_abstract_name(), $sformatf("created bus parser of type %p (id %0d)", m_bus_type, get_inst_id()))
   endfunction : new

endclass : evl_bus_parser

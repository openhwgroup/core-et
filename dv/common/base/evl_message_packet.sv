//--------------------------------------------------------------------------------------------------
//
// Class: evl_message_packet
//
// This class is intended to be used to pass message information to some class object.
//
class evl_message_packet extends evl_object;

   `uvm_object_utils(evl_message_packet)


   //-----------------------------------------------------------------------------------------------
   //
   // Member Variables and Objects
   //
   // m_packet_source: This is the source of the packet.
   //                  -----------------------------------------------------------------------------
   // m_packet_type:   This is the type of monitor packet.  It is completely context-dependent ...
   //                  each sender can have its own encoding for packet type.
   //                  -----------------------------------------------------------------------------
   // m_packet_value:  This is the packet value.
   //
   int         m_packet_source = 0;
   int         m_packet_type   = 0;
   evl_dword_t m_packet_value  = evl_trans_id_t'(0);


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_packet_source
   //
   // This function is used to set the packet source for this message packet.
   //
   // Input Variables:
   //
   // packet_source_in: This is the packet source identifier to use.
   //
   function void set_packet_source(input int packet_source_in);
      m_packet_source = packet_source_in;
   endfunction : set_packet_source


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_packet_source
   //
   // This function is used to get the packet source for this message packet.
   //
   // Return Value:
   //
   // int: This is the packet source identifier for this packet.
   //
   function int get_packet_source();
      return m_packet_source;
   endfunction : get_packet_source


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_packet_type
   //
   // This function is used to set the packet type for this message packet.
   //
   // Input Variables:
   //
   // packet_type_in: This is the packet type to use.
   //
   function void set_packet_type(input int packet_type_in);
      m_packet_type = packet_type_in;
   endfunction : set_packet_type


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_packet_type
   //
   // This function is used to get the packet type for this message packet.
   //
   // Return Value:
   //
   // int: This is the packet type for this packet.
   //
   function int get_packet_type();
      return m_packet_type;
   endfunction : get_packet_type


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_packet_value
   //
   // This function is used to set the value for this message packet.
   //
   // Input Variables:
   //
   // packet_value_in: This is the packet value.
   //
   function void set_packet_value(input evl_dword_t packet_value_in);
      m_packet_value = packet_value_in;
   endfunction : set_packet_value


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_packet_value
   //
   // This function is used to get the packet value for this message packet.
   //
   // Return Value:
   //
   // evl_dword_t: This is the packet value.
   //
   function evl_dword_t get_packet_value();
      return m_packet_value;
   endfunction : get_packet_value


   //-----------------------------------------------------------------------------------------------
   //
   // Function: sprint_obj
   //
   // This function is used to print object information consistently.  It is intended to be
   // redefined in extended classes to print useful, concise information for requests.
   //
   // Return Value:
   //
   // string: This is a formatted string to be used for logging.
   //
   virtual function string sprint_obj();
      if (evl_param_xref::param_key_exists(m_packet_source) == 1'b1) begin
         return $sformatf("%0s::0x%0x 0x%x", evl_param_xref::get_param_name(m_packet_source), m_packet_type, m_packet_value);
      end
      return $sformatf("0x%0x::0x%0x 0x%x", m_packet_source, m_packet_type, m_packet_value);
   endfunction : sprint_obj


   //-----------------------------------------------------------------------------------------------
   //
   // Function: do_copy
   //
   virtual function void do_copy(uvm_object rhs);
      evl_message_packet source_obj;

      super.do_copy(rhs);
      $cast(source_obj, rhs);
      m_packet_source   = source_obj.m_packet_source;
      m_packet_type     = source_obj.m_packet_type;
      m_packet_value    = source_obj.m_packet_value;
   endfunction : do_copy


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string      name_in          = "evl_message_packet",
                input int         packet_source_in = 0,
                input int         packet_type_in   = 0,
                input evl_dword_t packet_value_in  = evl_dword_t'(0));
      super.new(name_in);
      m_packet_source = packet_source_in;
      m_packet_type   = packet_type_in;
      m_packet_value  = packet_value_in;
   endfunction : new

endclass : evl_message_packet

//--------------------------------------------------------------------------------------------------
//
// Class: evl_monitor_packet
//
// This class is intended to be used to pass RTL monitor information to some class object (like a
// scoreboard), although there is nothing preventing it from being used to drive information as
// well.
//
class evl_monitor_packet extends evl_object;

   `uvm_object_utils(evl_monitor_packet)


   //-----------------------------------------------------------------------------------------------
   //
   // Member Variables and Objects
   //
   // m_packet_source:   This is the source of the packet.  As new monitor types are added, they
   //                    should use evl_param_xref::get_param_key() to get a unique integer to use
   //                    for this value.  There is no convention that needs to be followed here ...
   //                    any string can be used (the simplest may be the verification component's
   //                    type name, or something like that).
   //                    ---------------------------------------------------------------------------
   // m_packet_type:     This is the type of monitor packet.  It is completely context-dependent ...
   //                    each monitor can have its own encoding for packet type.
   //                    ---------------------------------------------------------------------------
   // m_packet_priority: This is the packet priority.  Higher numbers reflect lower priority (and
   //                    thus a delay in processing).  Normally a value of 0 is used.
   //                    ---------------------------------------------------------------------------
   // m_trans_id:        This is the transaction ID for this packet.
   //                    ---------------------------------------------------------------------------
   // m_cycle_count:     This is the current cycle count for this packet.
   //
   int            m_packet_source   = 0;
   int            m_packet_type     = 0;
   int            m_packet_priority = 0;
   evl_trans_id_t m_trans_id        = ~evl_trans_id_t'(0);
   int            m_cycle_count     = 0;


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_packet_source
   //
   // This function is used to set the packet source for this RTL packet.
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
   // This function is used to get the packet source for this RTL packet.
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
   // This function is used to set the packet type for this RTL packet.
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
   // This function is used to get the packet type for this RTL packet.
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
   // Function: set_packet_priority
   //
   // This function is used to set the packet priority for this RTL packet.
   //
   // Input Variables:
   //
   // packet_priority_in: This is the packet priority to use.
   //
   function void set_packet_priority(input int packet_priority_in);
      m_packet_priority = packet_priority_in;
   endfunction : set_packet_priority


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_packet_priority
   //
   // This function is used to get the packet priority for this RTL packet.
   //
   // Return Value:
   //
   // int: This is the packet priority for this packet.
   //
   function int get_packet_priority();
      return m_packet_priority;
   endfunction : get_packet_priority


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_trans_id
   //
   // This function is used to set the transaction ID for this RTL packet.
   //
   // Input Variables:
   //
   // packet_trans_id_in: This is the transaction ID to use.
   //
   function void set_trans_id(input evl_trans_id_t trans_id_in);
      m_trans_id = trans_id_in;
   endfunction : set_trans_id


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_trans_id
   //
   // This function is used to get the transaction ID for this RTL packet.
   //
   // Return Value:
   //
   // int: This is the transaction ID for this packet.
   //
   function evl_trans_id_t get_trans_id();
      return m_trans_id;
   endfunction : get_trans_id


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_cycle_count
   //
   // This function is used to set the cycle count for this RTL packet.
   //
   // Input Variables:
   //
   // cycle_count_in: This is the cycle count to use.
   //
   function void set_cycle_count(input int cycle_count_in);
      m_cycle_count = cycle_count_in;
   endfunction : set_cycle_count


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_cycle_count
   //
   // This function is used to get the cycle count for this RTL packet.
   //
   // Return Value:
   //
   // int: This is the cycle count for this packet.
   //
   function int get_cycle_count();
      return m_cycle_count;
   endfunction : get_cycle_count


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
         return $sformatf("%0s::0x%0x", evl_param_xref::get_param_name(m_packet_source), m_packet_type);
      end
      return $sformatf("0x%0x::0x%0x", m_packet_source, m_packet_type);
   endfunction : sprint_obj


   //-----------------------------------------------------------------------------------------------
   //
   // Function: do_copy
   //
   virtual function void do_copy(uvm_object rhs);
      evl_monitor_packet source_obj;

      super.do_copy(rhs);
      $cast(source_obj, rhs);
      m_packet_source   = source_obj.m_packet_source;
      m_packet_type     = source_obj.m_packet_type;
      m_packet_priority = source_obj.m_packet_priority;
      m_cycle_count     = source_obj.m_cycle_count;
   endfunction : do_copy


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string         name_in            = "evl_monitor_packet",
                input int            packet_source_in   = 0,
                input int            packet_type_in     = 0,
                input int            packet_priority_in = 0,
                input evl_trans_id_t trans_id_in        = ~evl_trans_id_t'(0),
                input int            cycle_count_in     = 0);
      super.new(name_in);
      m_packet_source   = packet_source_in;
      m_packet_type     = packet_type_in;
      m_packet_priority = packet_priority_in;
      m_trans_id        = trans_id_in;
      m_cycle_count     = cycle_count_in;
   endfunction : new

endclass : evl_monitor_packet

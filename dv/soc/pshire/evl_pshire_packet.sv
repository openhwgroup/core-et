class evl_pshire_packet extends evl_monitor_packet;

   evl_pshire_packet_u m_info;


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_info
   //    This function is used to set the info of this bus request
   //
   // Input Variables:
   //    info_in: This is the info of the request
   //
   // Return Value:
   //    none
   //
   function void set_info(input evl_pshire_packet_u info_in);
      m_info = info_in;
   endfunction : set_info


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_info
   //    This function is used to get the info of this bus request
   //
   // Input Variables:
   //    none
   //
   // Return Value:
   //    evl_pshire_packet_u: This is the info of the request
   //
   function evl_pshire_packet_u get_info();
      return m_info;
   endfunction : get_info


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string               name_in,
                input int                  packet_type_in,
                input evl_pshire_packet_u info_in);
      super.new(name_in, 0, packet_type_in, 0, evl_trans_id_t'(0), 0);
      m_info = info_in;
   endfunction : new

endclass : evl_pshire_packet

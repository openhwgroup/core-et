class evl_minion_packet extends evl_monitor_packet;

   evl_minion_packet_u m_info;

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
   function void set_info(input evl_minion_packet_u info_in);
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
   //    evl_minion_packet_u: This is the info of the request
   //
   function evl_minion_packet_u get_info();
      return m_info;
   endfunction : get_info

endclass

class evl_neigh_packet extends evl_monitor_packet;

   evl_trans_id_t     m_trans_id;    // dv-related-ID
   int                m_core_id;
   evl_neigh_packet_u m_info;


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_core_id
   //    This function is used to set the core_id of this bus request
   //
   // Input Variables:
   //    core_id_in: This is the core_id of the request
   //
   // Return Value:
   //    none
   //
   function void set_core_id(evl_core_id_t core_id_in);
      m_core_id = core_id_in;
   endfunction : set_core_id


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_core_id
   //    This function is used to get the core_id of this bus request
   //
   // Input Variables:
   //    none
   //
   // Return Value:
   //    evl_core_id_t: This is the core_id of the request
   //
   function evl_core_id_t get_core_id();
      return m_core_id;
   endfunction : get_core_id


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_trans_id
   //    This function is used to set the trans_id of this bus request
   //
   // Input Variables:
   //    trans_id_in: This is the trans_id of the request
   //
   // Return Value:
   //    none
   //
   function void set_trans_id(evl_trans_id_t trans_id_in);
      m_trans_id = trans_id_in;
   endfunction : set_trans_id


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_trans_id
   //    This function is used to get the trans_id of this bus request
   //
   // Input Variables:
   //    none
   //
   // Return Value:
   //    evl_trans_id_t: This is the trans_id of the request
   //
   function evl_trans_id_t get_trans_id();
      return m_trans_id;
   endfunction : get_trans_id


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
   function void set_info(input evl_neigh_packet_u info_in);
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
   //    evl_neigh_packet_u: This is the info of the request
   //
   function evl_neigh_packet_u get_info();
      return m_info;
   endfunction : get_info

endclass

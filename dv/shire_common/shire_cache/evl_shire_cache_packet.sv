class evl_shire_cache_packet extends evl_monitor_packet;

   int             m_bank_id;
   bit             m_use_remote_shire_id;
   evl_shire_id_t  m_remote_shire_id;
   evl_sc_packet_u m_info;


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_bank_id
   //    This function is used to set the bank_id of this bus request
   //
   // Input Variables:
   //    bank_id_in: This is the bank_id of the request
   //
   // Return Value:
   //    none
   //
   function void set_bank_id(int unsigned bank_id_in);
      m_bank_id = bank_id_in;
   endfunction : set_bank_id


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_bank_id
   //    This function is used to get the bank_id of this bus request
   //
   // Input Variables:
   //    none
   //
   // Return Value:
   //    evl_bank_id_t: This is the bank_id of the request
   //
   function int unsigned get_bank_id();
      return m_bank_id;
   endfunction : get_bank_id


   //-----------------------------------------------------------------------------------------------
   //
   // Function: use_remote_shire_id
   //    This function is used to BLAH.
   //
   function bit use_remote_shire_id();
      return m_use_remote_shire_id;
   endfunction : use_remote_shire_id


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_remote_shire_id
   //    This function is used to BLAH.
   //
   function evl_shire_id_t get_remote_shire_id();
      return m_remote_shire_id;
   endfunction : get_remote_shire_id


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
   function void set_info(input evl_sc_packet_u info_in);
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
   //    evl_sc_packet_u: This is the info of the request
   //
   function evl_sc_packet_u get_info();
      return m_info;
   endfunction : get_info


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string          name_in,
                input int             bank_in,
                input int             packet_source_in,
                input int             packet_type_in,
                input evl_trans_id_t  trans_id_in,
                input int             cycle_count_in,
                input bit             use_remote_shire_id_in,
                input evl_shire_id_t  remote_shire_id_in,
                input evl_sc_packet_u info_in);
      super.new(name_in, packet_source_in, packet_type_in, 0, trans_id_in, cycle_count_in);
      m_bank_id             = bank_in;
      m_use_remote_shire_id = use_remote_shire_id_in;
      m_remote_shire_id     = remote_shire_id_in;
      m_info                = info_in;
   endfunction : new

endclass

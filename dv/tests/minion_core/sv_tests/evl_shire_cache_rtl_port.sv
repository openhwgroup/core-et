//--------------------------------------------------------------------------------------------------
//
// Class: evl_shire_cache_rtl_port
//
class evl_shire_cache_rtl_port extends evl_rtl_port;

   evl_shire_cache_packet            m_bank_info;
   mailbox #(evl_shire_cache_packet) m_obs_sc_mb;
   string                            m_packet_type_names[int];
   string                            m_addr_space_names[int];


   //-----------------------------------------------------------------------------------------------
   //
   //
   //
   task get_obs_sc(output evl_shire_cache_packet packet);
      m_obs_sc_mb.get(packet);
   endtask


   //-----------------------------------------------------------------------------------------------
   //
   //
   //
   function send_packet(input int             packet_type,
                        input int             destination_in,
                        input int             bank_num,
                        input evl_trans_id_t  trans_id,
                        input bit             use_remote_shire_id,
                        input evl_shire_id_t  remote_shire_id,
                        input int             cycle_count,
                        input evl_sc_packet_u info);
      int destination = destination_in;

      if (destination inside { int'(SC_Address_L2), int'(SC_Address_Msg) }) begin
         m_bank_info = new(get_root_abstract_name(), bank_num, int'(SC_Source_L2), packet_type, trans_id, cycle_count, use_remote_shire_id, remote_shire_id, info);
      end
      else if (destination inside { int'(SC_Address_L3) }) begin
         m_bank_info = new(get_root_abstract_name(), bank_num, int'(SC_Source_L3), packet_type, trans_id, cycle_count, use_remote_shire_id, remote_shire_id, info);
      end
      else if (destination inside { int'(SC_Address_Scp) }) begin
         m_bank_info = new(get_root_abstract_name(), bank_num, int'(SC_Source_Scp), packet_type, trans_id, cycle_count, use_remote_shire_id, remote_shire_id, info);
      end
      else if (destination inside { int'(SC_Address_Idx) }) begin
         m_bank_info = new(get_root_abstract_name(), bank_num, int'(SC_Source_L2), packet_type, trans_id, cycle_count, use_remote_shire_id, remote_shire_id, info);
      end
      else begin
         m_bank_info = new(get_root_abstract_name(), bank_num, int'(SC_Source_L2), packet_type, trans_id, cycle_count, use_remote_shire_id, remote_shire_id, info);
      end
      void'(m_obs_sc_mb.try_put(m_bank_info));
   endfunction : send_packet


   //-----------------------------------------------------------------------------------------------
   //
   //
   //
   function new(input string full_name_in = "evl_shire_cache_rtl_port", input int skip_levels_in[] = {});
      evl_sc_event_t         packet_type;
      evl_sc_address_space_t addr_space;

      super.new(full_name_in, SHIRE_CACHE_RTL_AGENT, ~evl_agent_id_t'(0), "evl_shire_cache_env", "", "", "", skip_levels_in);
      m_obs_sc_mb = new();

      packet_type = packet_type.first();
      while (packet_type != packet_type.last()) begin
          m_packet_type_names[int'(packet_type)] = packet_type.name();
          packet_type = packet_type.next();
      end
      m_packet_type_names[int'(packet_type)] = packet_type.name();

      addr_space = addr_space.first();
      while (addr_space != addr_space.last()) begin
          m_addr_space_names[int'(addr_space)] = addr_space.name();
          addr_space = addr_space.next();
      end
      m_addr_space_names[int'(addr_space)] = addr_space.name();
   endfunction : new

endclass

//--------------------------------------------------------------------------------------------------
//
// Class: evl_memshire_rtl_port
//
class evl_memshire_rtl_port extends evl_rtl_port;

   mailbox #(evl_memshire_packet) m_obs_memshire_mb;


   //-----------------------------------------------------------------------------------------------
   //
   //
   //
   task get_obs_packet(output evl_memshire_packet packet);
      m_obs_memshire_mb.get(packet);
   endtask : get_obs_packet


   //-----------------------------------------------------------------------------------------------
   //
   //
   //
   function send_packet(input int packet_type, input evl_memshire_packet_u info);
      evl_memshire_packet packet;

      packet = new(get_root_abstract_name(), packet_type, info);
      void'(m_obs_memshire_mb.try_put(packet));
   endfunction : send_packet


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string         full_name_in     = "evl_memshire_rtl_port",
                input evl_agent_id_t agent_pid_in     = ~evl_agent_id_t'(0),
                input string         param_list_in    = "",
                input int            skip_levels_in[] = {});
      super.new(full_name_in, MEM_SHIRE_RTL_AGENT, agent_pid_in, "evl_memshire_env", param_list_in, "", "", skip_levels_in);
      m_obs_memshire_mb = new();
   endfunction : new

endclass : evl_memshire_rtl_port

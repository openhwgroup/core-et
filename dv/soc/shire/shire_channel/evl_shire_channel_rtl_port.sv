//--------------------------------------------------------------------------------------------------
//
// Class: evl_shire_rtl_channel_port
//

class evl_shire_channel_rtl_port extends evl_rtl_port;

   mailbox #(evl_monitor_packet) m_obs_event;

   //-----------------------------------------------------------------------------------------------
   //
   // Task: get_event
   //
   virtual task get_event(output evl_monitor_packet packet);
      m_obs_event.get(packet);
   endtask


   //-----------------------------------------------------------------------------------------------
   //
   // Function: send_event
   //
   function send_event(input int packet_type, input int core_id, input evl_trans_id_t trans_id, input evl_ucblock_packet_u info);
      evl_ucblock_packet ucblock_packet;

      ucblock_packet = new();
      ucblock_packet.set_packet_type (packet_type);
      ucblock_packet.set_trans_id    (trans_id);
      ucblock_packet.set_core_id     (core_id);
      ucblock_packet.set_info        (info);
      void'(m_obs_event.try_put(ucblock_packet));
   endfunction : send_event


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string full_name_in     = "evl_shire_channel_rtl_port",
                input string param_list_in    = "",
                input int    skip_levels_in[] = {});
      super.new(full_name_in, SHIRE_CHANNEL_RTL_AGENT, ~evl_agent_id_t'(0), "evl_shire_channel_env", param_list_in, "", "", skip_levels_in);
      m_obs_event = new();
   endfunction : new

endclass : evl_shire_channel_rtl_port

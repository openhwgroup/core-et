//-----------------------------------------------------------------------------------------------
//
// Class: evl_standalone_minion_rtl_port
//
class evl_standalone_minion_rtl_port extends evl_rtl_port;

   mailbox #(evl_standalone_minion_packet) m_obs_event;

   task get_rtl_event(output evl_standalone_minion_packet packet);
      m_obs_event.get(packet);
   endtask


   function send_event(input int packet_type, input int core_id, input evl_trans_id_t trans_id, input evl_standalone_minion_packet_u info);
      evl_standalone_minion_packet standalone_minion_packet;
      standalone_minion_packet = new();
      standalone_minion_packet.set_packet_type (packet_type);
      standalone_minion_packet.set_trans_id    (trans_id);
      standalone_minion_packet.set_core_id     (core_id);
      standalone_minion_packet.set_info        (info);
      void'(m_obs_event.try_put(standalone_minion_packet));
   endfunction : send_event


   //--------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string         full_name_in = "standalone_minion_rtl_port",
                input evl_agent_id_t agent_pid_in = ~evl_agent_id_t'(0),
                input int            skip_levels_in[] = {});
      super.new(full_name_in, STANDALONE_MINION_RTL_AGENT, agent_pid_in, "evl_standalone_minion_env", "", "", "", skip_levels_in);
      m_obs_event = new();
   endfunction : new

endclass : evl_standalone_minion_rtl_port




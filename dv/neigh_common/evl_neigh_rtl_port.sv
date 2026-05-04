//-----------------------------------------------------------------------------------------------
//
// Class: evl_neigh_rtl_port
//
class evl_neigh_rtl_port extends evl_rtl_port;

   mailbox #(evl_neigh_packet) m_obs_event;

   mailbox #(evl_pmu_desc) m_obs_pmu_event;

   bit m_is_shutdown;

   task get_event_neigh(output evl_neigh_packet packet);
      m_obs_event.get(packet);
   endtask


   function send_event(input int packet_type, input int core_id, input evl_trans_id_t trans_id, input evl_neigh_packet_u info);
      evl_neigh_packet neigh_packet;

      neigh_packet = new();
      neigh_packet.set_packet_type (packet_type);
      neigh_packet.set_trans_id    (trans_id);
      neigh_packet.set_core_id     (core_id);
      neigh_packet.set_info        (info);
      void'(m_obs_event.try_put(neigh_packet));
   endfunction : send_event


   function send_pmu_event(input int packet_type, input evl_neigh_packet_u info);
      evl_pmu_desc pmu_desc;
      evl_pmu_event_type_t pmu_event_type;
      pmu_desc = new();

      pmu_event_type = evl_pmu_event_type_t'(info.pmu_event.info.pmu_event_type);
      pmu_desc.set_event_type(pmu_event_type);
      pmu_desc.set_cntr_id(info.pmu_event.info.pmu_cntr_id);
      pmu_desc.set_thread_id(info.pmu_event.info.pmu_thread_id);
      pmu_desc.set_txn_type(evl_pmu_txn_type_t'(packet_type));
      m_obs_pmu_event.try_put(pmu_desc);
   endfunction : send_pmu_event


   task get_pmu_event(output evl_pmu_desc packet);
      m_obs_pmu_event.get(packet);
   endtask : get_pmu_event


   //-----------------------------------------------------------------------------------------------
   //
   // Function: shutdown_started
   //
   function void shutdown_started();
      m_is_shutdown = 1'b1;
   endfunction


   //-----------------------------------------------------------------------------------------------
   //
   // Function: is_shutdown
   //
   function bit is_shutdown();
      return m_is_shutdown;
   endfunction


   //-----------------------------------------------------------------------------------------------
   //
   // Function: power_on
   //
   virtual function void power_on();
      m_global_attrs.set_neigh_power(1'b1, m_shire_id, evl_neighborhood_id_t'(m_agent_id));
      super.power_on();
   endfunction : power_on


   //-----------------------------------------------------------------------------------------------
   //
   // Function: power_off
   //
   virtual function void power_off();
      m_global_attrs.set_neigh_power(1'b0, m_shire_id, evl_neighborhood_id_t'(m_agent_id));
      super.power_off();
   endfunction : power_off


   //--------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string         full_name_in = "neigh_rtl_port",
                input evl_agent_id_t agent_pid_in = ~evl_agent_id_t'(0),
                input int            skip_levels_in[] = {});
      super.new(full_name_in, NEIGH_RTL_AGENT, agent_pid_in, "evl_neigh_env", "", "", "", skip_levels_in);
      m_obs_event = new();
      m_obs_pmu_event = new();
      m_is_shutdown = 1'b0;
   endfunction : new

endclass : evl_neigh_rtl_port

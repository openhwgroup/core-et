//--------------------------------------------------------------------------------------------------
//
// Class: evl_pshire_noc_rtl_port
//
class evl_pshire_noc_rtl_port extends evl_rtl_port;

   mailbox #(evl_pshire_noc_packet) m_obs_pshire_noc_mb;


   //-----------------------------------------------------------------------------------------------
   //
   //
   //
   task get_obs_packet(output evl_pshire_noc_packet packet);
      m_obs_pshire_noc_mb.get(packet);
   endtask : get_obs_packet


   //-----------------------------------------------------------------------------------------------
   //
   //
   //
   function send_packet(input int packet_type, input evl_pshire_noc_packet_u info);
      evl_pshire_noc_packet packet;

      packet = new(get_root_abstract_name(), packet_type, info);
      void'(m_obs_pshire_noc_mb.try_put(packet));
   endfunction : send_packet


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string         full_name_in     = "evl_pshire_noc_rtl_port",
                input evl_agent_id_t agent_pid_in     = ~evl_agent_id_t'(0),
                input string         param_list_in    = "",
                input int            skip_levels_in[] = {});
      super.new(full_name_in, PSHIRE_NOC_RTL_AGENT, agent_pid_in, "evl_pshire_noc_env", param_list_in, "", "", skip_levels_in);
      if (agent_pid_in == ~evl_agent_id_t'(0)) begin
         set_abstract_name($sformatf("PSHIRE_NOC(RTL)"));
      end
      else begin
         set_abstract_name($sformatf("PSHIRE_NOC-%0d(RTL)", agent_pid_in));
      end
      m_obs_pshire_noc_mb = new();
   endfunction : new

endclass : evl_pshire_noc_rtl_port

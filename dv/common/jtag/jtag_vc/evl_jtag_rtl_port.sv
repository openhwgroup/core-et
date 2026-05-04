//--------------------------------------------------------------------------------------------------
//
// Class: evl_jtag_rtl_port
//
class evl_jtag_rtl_port extends evl_rtl_port;

   mailbox #(evl_jtag_req_info_t) m_snd_req_mb;
   mailbox #(evl_jtag_rsp_info_t) m_obs_rsp_mb;


   function void put_jtag_req(input evl_jtag_req_info_t jtag_req);
      void'(m_snd_req_mb.try_put(jtag_req));
   endfunction : put_jtag_req


   function void put_jtag_rsp(input evl_jtag_rsp_info_t jtag_rsp);
      void'(m_obs_rsp_mb.try_put(jtag_rsp));
   endfunction : put_jtag_rsp


   task get_jtag_req(output evl_jtag_req_info_t jtag_req);
      m_snd_req_mb.get(jtag_req);
   endtask : get_jtag_req


   task get_jtag_rsp(output evl_jtag_rsp_info_t jtag_rsp);
      m_obs_rsp_mb.get(jtag_rsp);
   endtask : get_jtag_rsp


   function new(input string           full_name_in     = "JTAG",
                input evl_agent_type_t agent_type_in    = JTAG_BUS_AGENT,
                input evl_agent_id_t   agent_pid_in     = ~evl_agent_id_t'(0),
                input string           sv_comp_in       = "evl_jtag_verif_comp",
                input string           param_list_in    = "",
                input string           name_prefix_in   = "",
                input string           name_postfix_in  = "",
                input int              skip_levels_in[] = {},
                input int              port_id_in       = 0,
                input int              max_port_id_in   = 0,
                input int              additional_hier  = 0);
      super.new(full_name_in, agent_type_in, agent_pid_in, sv_comp_in, param_list_in, name_prefix_in, name_postfix_in, skip_levels_in, port_id_in, max_port_id_in, additional_hier);
      m_obs_rsp_mb = new();
      m_snd_req_mb = new();
   endfunction : new

endclass : evl_jtag_rtl_port

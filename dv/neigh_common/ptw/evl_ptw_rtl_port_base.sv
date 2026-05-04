//--------------------------------------------------------------------------------------------------
//
// Class: evl_ptw_rtl_port_base
//
class evl_ptw_rtl_port_base extends evl_rtl_port;

   virtual task get_obs_rsp(output evl_ptw_rsp bus_rsp);
   endtask : get_obs_rsp

   virtual task get_obs_req(output evl_ptw_req bus_req);
   endtask : get_obs_req

   function new(input string           full_name_in     = "evl_rtl_port",
                input evl_agent_type_t agent_type_in    = NO_AGENT,
                input evl_agent_id_t   agent_pid_in     = ~evl_agent_id_t'(0),
                input string           sv_comp_in       = "",
                input string           param_list_in    = "",
                input string           name_prefix_in   = "",
                input string           name_postfix_in  = "",
                input int              skip_levels_in[] = {});
      super.new(full_name_in, agent_type_in, agent_pid_in, sv_comp_in, param_list_in, name_prefix_in, name_postfix_in, skip_levels_in);
   endfunction : new

endclass



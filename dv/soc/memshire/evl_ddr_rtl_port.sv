//--------------------------------------------------------------------------------------------------
//
// Class: evl_ddr_rtl_port
//
class evl_ddr_rtl_port extends evl_rtl_port;

   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string         full_name_in     = "evl_ddr_rtl_port",
                input evl_agent_id_t agent_pid_in     = ~evl_agent_id_t'(0),
                input string         param_list_in    = "",
                input int            skip_levels_in[] = {});
      super.new(full_name_in, DDR_SHIRE_RTL_AGENT, agent_pid_in, "evl_ddr_env", param_list_in, "", "", skip_levels_in);
   endfunction : new

endclass : evl_ddr_rtl_port

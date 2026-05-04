//--------------------------------------------------------------------------------------------------
//
// Class: evl_noc_rtl_port
//
class evl_noc_rtl_port extends evl_rtl_port;

   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string full_name_in     = "evl_noc_rtl_port",
                input int    skip_levels_in[] = {});
      super.new(full_name_in, SOC_RTL_AGENT, ~evl_agent_id_t'(0), "evl_noc_env", "", "", "", skip_levels_in);
   endfunction : new

endclass : evl_noc_rtl_port

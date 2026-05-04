//==================================================================================================
//
// Class: evl_param_ping
//
class evl_param_ping extends uvm_void;

   event m_event;


   //-----------------------------------------------------------------------------------------------
   //
   // Function: ping
   //
   function void ping();
      -> m_event;
   endfunction : ping


   //-----------------------------------------------------------------------------------------------
   //
   // Static Function: get_param_name
   //
   task wait_for_ping();
      #0 @m_event;
   endtask : wait_for_ping

endclass : evl_param_ping

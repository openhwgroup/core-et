//--------------------------------------------------------------------------------------------------
//
//
// Class: evl_axi_bus_rsp
//
// This class object is used to transfer information between the AXI RTL port and UVM monitors and
// drivers.  The actual pin monitor/driver is the evl_axi_interface module that is instantiated in
// the testbench.  The evl_axi_interface module and its companion monitor (and driver) must have a
// handle to the same AXI RTL port object instance.
//
// When a response is observed, the evl_axi_interface module sends monitored response information to
// the AXI RTL port by filling out a packed structure and then sending that packed structure to the
// RTL port.  When a particular structure is received by the RTL port, the information is
// transferred to an instance of this class object, and that class object instance is then sent to
// the UVM monitor.
//
// Similarly, a UVM driver sends an instance of this class object to the RTL port, and then that
// information is transferred to a packed structure that is then consumed by the evl_axi_interface
// module.
//
// By dealing only with packed objects in the evl_axi_interface module, it is possible to create
// either synthesizeable modules or DPI calls to be used in an emulation platform while maintaining
// essentially the same evl_axi_interface module code.
//
class evl_axi_bus_rsp;

   //-----------------------------------------------------------------------------------------------
   //
   // Variables for Monitoring and Driving
   //
   // m_trans_id:      This is the EVL transaction ID (it is only used if the Esperanto Verification
   //                  Library is fully included).
   //                  -----------------------------------------------------------------------------
   // m_rsp_for_write: This bit is set to 1'b1 if the response is for a write request.
   //                  -----------------------------------------------------------------------------
   // m_xref_valid:    This bit is set to 1'b1 if this class object contains complete response
   //                  information.
   //                  -----------------------------------------------------------------------------
   // m_x_resp:        This is the AXI response type.
   //                  -----------------------------------------------------------------------------
   // m_x_data:        This is the 64-byte data associated with the response.  When sending single
   //                  beats, this variable is normalized (i.e., bit 0 is always the least
   //                  significant bit of the data beat based on the bus width).  When holding data
   //                  for a full read response (i.e., all data beats have been received and
   //                  m_xref_valid is set to 1'b1), the data in this variable is cache-line aligned
   //                  (i.e., bit 0 always corresponds to bit 0 of the cache line).
   //                  -----------------------------------------------------------------------------
   // m_x_user:        This is the user field for the AXI response data transfer (typically parity).
   //                  -----------------------------------------------------------------------------
   // m_x_last:        This bit is set to 1'b1 for the last transfer of a response.
   //
   evl_trans_id_t     m_trans_id      = evl_trans_id_t'(0);
   bit                m_rsp_for_write = 1'b0;
   bit                m_xref_valid    = 1'b0;
   evl_bus_req_id_t   m_x_id          = evl_bus_req_id_t'(0);
   evl_axi_resp_t     m_x_resp        = evl_axi_resp_t'(0);
   evl_xword_t        m_x_data        = evl_xword_t'(0);
   evl_bus_req_user_t m_x_user        = evl_bus_req_user_t'(0);
   bit                m_x_last        = 1'b0;


   //-----------------------------------------------------------------------------------------------
   //
   // Variables for Driving Only
   //
   // m_bus_rsp_delay: This is the delay (in cycles) before B/R is placed onto the signal pins.
   //                  This variable is only used for driving responses onto an AXI bus.
   //                  -----------------------------------------------------------------------------
   // m_cycle:         This is the cycle number in which to drive the response information.  This
   //                  variable is only used for driving responses onto an AXI bus.
   //
   int         m_cycle         = 0;
   evl_delay_t m_bus_rsp_delay = evl_delay_t'(0);


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_trans_id
   //
   // This function is used to set the EVL DV transaction ID of this bus request.
   //
   // Input Variables:
   //
   // trans_id_in: This is the DV transaction ID of the request.
   //
   function void set_trans_id(evl_trans_id_t trans_id_in);
      m_trans_id = trans_id_in;
   endfunction : set_trans_id


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_trans_id
   //
   // This function is used to get the EVL transaction ID of this bus request.
   //
   // Return Value:
   //
   // evl_trans_id_t: This is the EVL transaction ID of the request.
   //
   function evl_trans_id_t get_trans_id();
      return m_trans_id;
   endfunction : get_trans_id


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_rsp_for_write
   //    This function is used to set the response type (read/write) of this bus response
   //
   // Return Value:
   //    none
   //
   function void set_rsp_for_write();
      m_rsp_for_write = 1'b1;
      m_x_last        = 1'b1;
   endfunction : set_rsp_for_write


   //-----------------------------------------------------------------------------------------------
   //
   // Function: rsp_is_for_write
   //
   // This function returns 1'b1 if this response is an AXI B response (i.e., a response for a
   // write).
   //
   // Return Value:
   //
   // bit: This function returns 1'b1 for AXI B responses.
   //
   function bit rsp_is_for_write();
      return m_rsp_for_write;
   endfunction : rsp_is_for_write


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_mem_bus_rsp_delay
   //    This function is used to set the bus response (i.e. B or R channel) delay of this response
   //
   // Input Variables:
   //    delay_in: This is the bus delay of the response
   //
   // Return Value:
   //    none
   //
   function void set_mem_bus_rsp_delay(evl_delay_t delay_in);
      m_bus_rsp_delay = delay_in;
   endfunction


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_mem_bus_rsp_delay
   //    This function is used to get the bus response (i.e. B or R channel) delay of this response
   //
   // Input Variables:
   //    none
   //
   // Return Value:
   //    evl_delay_t: This is the bus delay of the response
   //
   function evl_delay_t get_mem_bus_rsp_delay();
      return m_bus_rsp_delay;
   endfunction


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_x_id
   //    This function is used to set the x_id of this bus response
   //
   // Input Variables:
   //    x_id_in: This is the ID of the response
   //
   // Return Value:
   //    none
   //
   function void set_x_id(input evl_bus_rsp_id_t x_id_in);
      m_x_id = x_id_in;
   endfunction : set_x_id


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_x_id
   //    This function is used to get the x_id of this bus response
   //
   // Input Variables:
   //    none
   //
   // Return Value:
   //    evl_bus_rsp_id_t: This is the ID of the response
   //
   function evl_bus_rsp_id_t get_x_id();
      return m_x_id;
   endfunction : get_x_id


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_x_resp
   //    This function is used to set the x_resp of this bus response
   //
   // Input Variables:
   //    x_resp_in: This is the RESP of the response
   //
   // Return Value:
   //    none
   //
   function void set_x_resp(input evl_axi_resp_t x_resp_in);
      m_x_resp = x_resp_in;
   endfunction : set_x_resp


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_x_resp
   //    This function is used to get the x_resp of this bus response
   //
   // Input Variables:
   //    none
   //
   // Return Value:
   //    evl_axi_resp_t: This is the RESP of the response
   //
   function evl_axi_resp_t get_x_resp();
      return m_x_resp;
   endfunction : get_x_resp


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_x_user
   //    This function is used to set the x_user of this bus response
   //
   // Input Variables:
   //    x_user_in: This is the USER of the response
   //
   // Return Value:
   //    none
   //
   function void set_x_user(input evl_bus_rsp_user_t x_user_in);
      m_x_user = x_user_in;
   endfunction : set_x_user


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_x_user
   //    This function is used to get the x_user of this bus response
   //
   // Input Variables:
   //    none
   //
   // Return Value:
   //    evl_bus_rsp_user_t: This is the USER of the response
   //
   function evl_bus_rsp_user_t get_x_user();
      return m_x_user;
   endfunction : get_x_user


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_x_data
   //    This function is used to set the x_data of this bus response
   //
   // Input Variables:
   //    x_data_in: This is the DATA of the response
   //
   // Return Value:
   //    none
   //
   function void set_x_data(input evl_xword_t x_data_in);
      m_x_data = x_data_in;
   endfunction : set_x_data


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_x_data
   //    This function is used to get the x_data of this bus response
   //
   // Input Variables:
   //    none
   //
   // Return Value:
   //    evl_xword_t: This is the DATA of the response
   //
   function evl_xword_t get_x_data();
      return m_x_data;
   endfunction : get_x_data


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_x_last
   //
   function void set_x_last();
      m_x_last = 1'b1;
   endfunction : set_x_last


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_x_last
   //
   function bit get_x_last();
      return m_x_last;
   endfunction : get_x_last


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_cycle
   //
   function void set_cycle(input int cycle_in);
      m_cycle = cycle_in;
   endfunction : set_cycle


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_cycle
   //
   function int get_cycle();
      return m_cycle;
   endfunction : get_cycle

endclass

//--------------------------------------------------------------------------------------------------
//
//
// Class: usr_apb_rtl_port
//
// This class object is used by the evl_apb_interface to capture APB requests and responses.  It can
// be used simply to print requests and responses as they are observed, or it can be used to pass
// packaged requests and responses to an external monitor (the external monitor would need to have a
// handle to the same object instantiated in the evl_apb_interface module; the get_obs_req() and
// get_obs_rsp() task calls could then be used get captured requests and responses).
//
// This class can either be modified or extended by users of this library; any modifications are the
// sole responsibility of the modifier.
//
class usr_apb_rtl_port;

   //
   // Create a couple of strings to identify messages from this particular object.  In addition,
   // define an agent ID that can be used to abstractly identify the bus for this object.
   //
   evl_agent_id_t  m_agent_id      = ~evl_agent_id_t'(0);
   string          m_abstract_name = "APB";
   string          m_rtl_name      = "unknown";


   //
   // Variables for clocks and resets, etc.
   //
   int   m_cycle_count = -1;
   bit   m_power_on    = 1'b1;
   bit   m_reset_asserted[];
   bit   m_reset_deasserted[];
   event m_reset_change;
   event m_clock_event;


   //
   // Define the request and response data width in bytes.  The default width is currently 64 bytes.
   //
   int m_data_bus_width;
   int m_data_bus_clog;


   //
   // The following variables are used to monitor requests and responses.
   //
   evl_apb_bus_req m_bus_req_mon;
   evl_apb_bus_rsp m_bus_rsp_mon;


   //
   // Mailboxes for collecting bus requests and responses from the interface to send to an external
   // monitor.
   //
   mailbox #(evl_apb_bus_req) m_obs_req_mb;
   mailbox #(evl_apb_bus_rsp) m_obs_rsp_mb;


   //
   // Mailboxes for collecting bus requests and responses from an external driver to send to the
   // interface.
   //
   mailbox #(evl_apb_bus_req) m_snd_req_mb;
   mailbox #(evl_apb_bus_rsp) m_snd_rsp_mb;


   //
   // Variables for enabling drivers.
   //
   bit m_master_driver       = 1'b0;
   bit m_slave_driver        = 1'b0;
   int m_delays_are_disabled = 0;


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_abstract_name
   //
   // This function is used to set the abstract name of this object.
   //
   virtual function void set_abstract_name(input string abstract_name_in);
      m_abstract_name = $sformatf("%s", abstract_name_in);
   endfunction : set_abstract_name


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_abstract_name
   //
   // This function is used to get the abstract name for this object.
   //
   virtual function string get_abstract_name();
      return m_abstract_name;
   endfunction : get_abstract_name


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_rtl_name
   //
   // This function is used to set the RTL name of this object.
   //
   virtual function void set_rtl_name(input string rtl_name_in);
      m_rtl_name = $sformatf("%s", rtl_name_in);
      log_message($sformatf("these messages map to %0s", m_rtl_name));
   endfunction : set_rtl_name


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_rtl_name
   //
   // This function is used to get the RTL name for this object.
   //
   virtual function string get_rtl_name();
      return m_rtl_name;
   endfunction : get_rtl_name


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_agent_id
   //
   // This function is used to set the agent ID for this object.  The agent ID can be used to
   // identify a particular APB bus within a specific module hierarchy.
   //
   virtual function void set_agent_id(input evl_agent_id_t agent_id_in);
      m_agent_id = agent_id_in;
   endfunction : set_agent_id


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_agent_id
   //
   // This function is used to get the agent ID for this object.
   //
   virtual function evl_agent_id_t get_agent_id();
      return m_agent_id;
   endfunction : get_agent_id


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_bus_data_width
   //
   // This function is used to set the bus data width (in bytes) for bus write requests.
   //
   virtual function void set_bus_data_width(input int bus_data_width_in);
      m_data_bus_width = bus_data_width_in;
      m_data_bus_clog  = $clog2(m_data_bus_width);
   endfunction : set_bus_data_width


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_bus_data_width
   //
   // This function is used to get the bus data width (in bytes) for bus write requests.
   //
   virtual function int get_bus_data_width();
      return m_data_bus_width;
   endfunction : get_bus_data_width


   //-----------------------------------------------------------------------------------------------
   //
   // Function: power_on
   //
   virtual function void power_on();
      if (m_power_on == 1'b0) begin
         `evl_log(`UVM_LOW, VMOD_GLOBAL, get_abstract_name(), $sformatf("turning power on (%s)", m_rtl_name))
      end
      m_power_on = 1'b1;
   endfunction : power_on


   //-----------------------------------------------------------------------------------------------
   //
   // Function: power_off
   //
   virtual function void power_off();
      if (m_power_on == 1'b1) begin
         `evl_log(`UVM_LOW, VMOD_GLOBAL, get_abstract_name(), $sformatf("turning power off (%s)", m_rtl_name))
      end
      m_power_on = 1'b0;
   endfunction : power_off


   //-----------------------------------------------------------------------------------------------
   //
   // Function: power_is_on
   //
   function bit power_is_on();
      return m_power_on;
   endfunction : power_is_on


   //-----------------------------------------------------------------------------------------------
   //
   // Function: power_is_off
   //
   function bit power_is_off();
      return ~m_power_on;
   endfunction : power_is_off


   //-----------------------------------------------------------------------------------------------
   //
   // Task: wait_for_power_on
   //
   task wait_for_power_on();
      #0 wait (m_power_on == 1'b1);
   endtask : wait_for_power_on


   //-----------------------------------------------------------------------------------------------
   //
   // Task: wait_for_power_off
   //
   task wait_for_power_off();
      #0 wait (m_power_on == 1'b0);
   endtask : wait_for_power_off


   //-----------------------------------------------------------------------------------------------
   //
   // Function: assert_reset
   //
   function void assert_reset(input int unsigned reset_index_in = 0);
      if (reset_index_in < m_reset_asserted.size()) begin
         m_reset_asserted[reset_index_in]   = 1'b1;
         m_reset_deasserted[reset_index_in] = 1'b0;
         -> m_reset_change;
      end
   endfunction : assert_reset


   //-----------------------------------------------------------------------------------------------
   //
   // Function: deassert_reset
   //
   function void deassert_reset(input int unsigned reset_index_in = 0);
      if (reset_index_in < m_reset_asserted.size()) begin
         m_reset_asserted[reset_index_in]   = 1'b0;
         m_reset_deasserted[reset_index_in] = 1'b1;
         -> m_reset_change;
      end
   endfunction : deassert_reset


   //-----------------------------------------------------------------------------------------------
   //
   // Function: reset_is_deasserted
   //
   function bit reset_is_deasserted(input int unsigned reset_index_in = 0);
      if (reset_index_in < m_reset_asserted.size()) begin
         return (((m_reset_asserted[reset_index_in] == 1'b0) && (m_reset_deasserted[reset_index_in] == 1'b1)) ? 1'b1 : 1'b0);
      end
      return 1'b0;
   endfunction : reset_is_deasserted


   //-----------------------------------------------------------------------------------------------
   //
   // Function: reset_is_asserted
   //
   function bit reset_is_asserted(input int unsigned reset_index_in = 0);
      if (reset_index_in < m_reset_asserted.size()) begin
         return (((m_reset_asserted[reset_index_in] == 1'b1) && (m_reset_deasserted[reset_index_in] == 1'b0)) ? 1'b1 : 1'b0);
      end
      return 1'b0;
   endfunction : reset_is_asserted


   //-----------------------------------------------------------------------------------------------
   //
   // Function: wait_for_reset_deassertion
   //
   task wait_for_reset_deassertion(input int unsigned reset_index_in = 0);
      if (reset_index_in < m_reset_asserted.size()) begin
         #0;
         while (reset_is_deasserted(reset_index_in) == 1'b0) begin
            @(m_reset_change);
         end
         #0;
      end
      return;
   endtask : wait_for_reset_deassertion


   //-----------------------------------------------------------------------------------------------
   //
   // Function: wait_for_reset_assertion
   //
   task wait_for_reset_assertion(input int unsigned reset_index_in = 0);
      if (reset_index_in < m_reset_asserted.size()) begin
         #0;
         while (reset_is_asserted(reset_index_in) == 1'b0) begin
            @(m_reset_change);
         end
         #0;
      end
      return;
   endtask : wait_for_reset_assertion


   //-----------------------------------------------------------------------------------------------
   //
   // Function: ping_clock
   //
   virtual function void ping_clock(input int clock_index_in = 0);
      ->m_clock_event;
      m_cycle_count++;
   endfunction : ping_clock


   //-----------------------------------------------------------------------------------------------
   //
   // Function: check_verbosity
   //
   // This function is used to check verbosity for this object before printing information.  A
   // value of 1'b1 should be returned if printing is enabled.
   //
   virtual function bit check_verbosity();
      return `evl_log_check_verbosity(`UVM_MEDIUM, VMOD_GLOBAL);
   endfunction : check_verbosity


   //-----------------------------------------------------------------------------------------------
   //
   // Function: log_message
   //
   // This function is used to print a log message.
   //
   virtual function void log_message(input string message);
      `evl_log(`UVM_MEDIUM, VMOD_GLOBAL, get_abstract_name(), message)
   endfunction : log_message


   //-----------------------------------------------------------------------------------------------
   //
   // Function: sprint_obj
   //
   // This function is used to format an APB request in a user-friendly form.  It returns a string
   // that is the formatted output.
   //
   // Input Variables:
   //
   // bus_req: This is the bus request structure that contains essential request information.
   //
   // Return Value:
   //
   // string: This function returns a formatted string with the request information.
   //
   virtual function string sprint_obj(evl_apb_req_s bus_req);
      if (bus_req.pwrite == 1'b1) begin
         return $sformatf("APB_REQ_WRITE 0x%x <- 0x%x", bus_req.paddr, bus_req.pwdata);
      end
      return $sformatf("APB_REQ_READ 0x%x", bus_req.paddr);
   endfunction : sprint_obj


   //-----------------------------------------------------------------------------------------------
   //
   // Function: sprint_rsp
   //
   // This function is used to format an APB response in a user-friendly form.  It returns a
   // string that is the formatted output.
   //
   // Input Variables:
   //
   // bus_rsp: This is the bus response structure that contains essential response information.
   //          -------------------------------------------------------------------------------------
   // bus_req: This is the bus request handle that contains essential request information.
   //
   // Return Value:
   //
   // string: This function returns a formatted string with the response information.
   //
   virtual function string sprint_rsp(input evl_apb_rsp_s bus_rsp,
                                      input evl_apb_req_s bus_req);
      if (bus_req.pwrite == 1'b1) begin
         return $sformatf("%0s (APB_REQ_WRITE 0x%x)", ((bus_rsp.pslverr == 1'b1) ? "APB_RESP_SLVERR" : "APB_RESP_OKAY"), bus_req.paddr);
      end
      if (bus_rsp.pslverr == 1'b1) begin
         return $sformatf("APB_RESP_SLVERR (APB_REQ_READ 0x%x)", bus_req.paddr);
      end
      return $sformatf("APB_RESP_OKAY 0x%x (APB_REQ_READ 0x%x)", bus_rsp.prdata, bus_req.paddr);
   endfunction : sprint_rsp


   //-----------------------------------------------------------------------------------------------
   //
   // Function: capture_req
   //
   // This function is called by the evl_apb_interface module when a request is detected.
   //
   // Input Variables:
   //
   // req_info: This is the bus request structure that contains essential request information.
   //
   virtual function void capture_req(input evl_apb_req_s req_info);
      //
      // Transfer request information from the packed structure to a class object.
      //
      m_bus_req_mon.m_bus_req = req_info;

      if (check_verbosity() == 1'b1) begin
         log_message(sprint_obj(m_bus_req_mon.m_bus_req));
      end
      if (m_obs_req_mb != null) begin
         m_obs_req_mb.try_put(m_bus_req_mon);
      end
   endfunction : capture_req


   //-----------------------------------------------------------------------------------------------
   //
   // Function: capture_rsp
   //
   // This function is called by the evl_apb_interface module when a response is detected.
   //
   // Input Variables:
   //
   // bus_rsp: This is the bus response structure that contains essential response information.
   //
   virtual function void capture_rsp(input evl_apb_rsp_s rsp_info);
      //
      // Transfer response information from the packed structure to a class object.
      //
      m_bus_rsp_mon.m_bus_rsp = rsp_info;

      if (check_verbosity() == 1'b1) begin
         log_message(sprint_rsp(m_bus_rsp_mon.m_bus_rsp, m_bus_req_mon.m_bus_req));
      end
      if (m_obs_rsp_mb != null) begin
         void'(m_obs_rsp_mb.try_put(m_bus_rsp_mon));
      end
   endfunction : capture_rsp


   //-----------------------------------------------------------------------------------------------
   //
   // Function: create_request_mailbox
   //
   // This function is to create a request mailbox and should only be used if there is an external
   // driver or monitor that intends to use it (via get_obs_req() calls).
   //
   virtual function create_request_mailbox();
      if (m_obs_req_mb == null) begin
         m_obs_req_mb = new();
      end
   endfunction : create_request_mailbox


   //-----------------------------------------------------------------------------------------------
   //
   // Function: create_response_mailbox
   //
   // This function is to create a response mailbox and should only be used if there is an external
   // driver or monitor that intends to use it (via get_obs_rsp() calls).
   //
   virtual function create_response_mailbox();
      if (m_obs_rsp_mb == null) begin
         m_obs_rsp_mb = new();
      end
   endfunction : create_response_mailbox


   //-----------------------------------------------------------------------------------------------
   //
   // Function: create_mailboxes
   //
   // This function is to create request and response mailboxes and should only be used if there is
   // an external monitor that intends to use them (via get_obs_req() and get_obs_rsp() calls).
   //
   virtual function create_mailboxes();
      create_request_mailbox();
      create_response_mailbox();
   endfunction : create_mailboxes


   //-----------------------------------------------------------------------------------------------
   //
   // Function: put_req
   //
   // This function receives a bus request object from the driver and keeps it in the mailbox
   // until it is ready to be placed on the bus interface.
   //
   virtual function void put_req(input evl_apb_bus_req bus_req);
      void'(m_snd_req_mb.try_put(bus_req));
   endfunction : put_req


   //-----------------------------------------------------------------------------------------------
   //
   // Function: put_rsp
   //
   // This function receives a bus response object from the driver and keeps it in the mailbox
   // until it is ready to be placed on the bus interface.
   //
   virtual function void put_rsp(input evl_apb_bus_rsp bus_rsp);
      void'(m_snd_rsp_mb.try_put(bus_rsp));
   endfunction : put_rsp


   //-----------------------------------------------------------------------------------------------
   //
   // Function: init_master
   //
   virtual function void init_master();
      m_master_driver = 1'b1;
   endfunction : init_master


   //-----------------------------------------------------------------------------------------------
   //
   // Function: init_slave
   //
   virtual function void init_slave();
      m_slave_driver = 1'b1;
   endfunction : init_slave


   //-----------------------------------------------------------------------------------------------
   //
   // Task: wait_for_master_driver
   //
   virtual task wait_for_master_driver();
      #0 wait (m_master_driver == 1'b1);
   endtask : wait_for_master_driver


   //-----------------------------------------------------------------------------------------------
   //
   // Task: wait_for_slave_driver
   //
   virtual task wait_for_slave_driver();
      #0 wait (m_slave_driver == 1'b1);
   endtask : wait_for_slave_driver


   //-----------------------------------------------------------------------------------------------
   //
   // Function: disable_delays
   //
   function void disable_delays(input int disable_type = `EVL_MASTER_DISABLE);
      m_delays_are_disabled |= disable_type;
   endfunction : disable_delays


   //-----------------------------------------------------------------------------------------------
   //
   // Function: enable_delays
   //
   function void enable_delays(input int disable_type = `EVL_MASTER_DISABLE);
      m_delays_are_disabled &= ~disable_type;
   endfunction : enable_delays


   //-----------------------------------------------------------------------------------------------
   //
   // Function: delays_are_enabled
   //
   function bit delays_are_enabled();
      return ((m_delays_are_disabled == 0) ? 1'b1 : 1'b0);
   endfunction : delays_are_enabled


   //-----------------------------------------------------------------------------------------------
   //
   // Function: delays_are_disabled
   //
   function bit delays_are_disabled();
      return ((m_delays_are_disabled == 0) ? 1'b0 : 1'b1);
   endfunction : delays_are_disabled


   //-----------------------------------------------------------------------------------------------
   //
   // Task: wait_for_delay_enable
   //
   task wait_for_delay_enable();
      if (m_delays_are_disabled != 0) begin
         #0 wait (m_delays_are_disabled == 0);
      end
   endtask : wait_for_delay_enable


   //-----------------------------------------------------------------------------------------------
   //
   // Task: wait_for_delay_disable
   //
   task wait_for_delay_disable();
      if (m_delays_are_disabled == 0) begin
         #0 wait (m_delays_are_disabled != 0);
      end
   endtask : wait_for_delay_disable


   //-----------------------------------------------------------------------------------------------
   //
   // Task: get_obs_req
   //
   // This task retrieves a bus request object from the mailbox.  This task is intended to be used
   // by an external monitor (should it exist).  Note that this task is blocking; it will not return
   // until there is something to observe.  A bus request structure is returned as an output
   // variable upon completion.
   //
   virtual task get_obs_req(output evl_apb_bus_req bus_req);
      if (m_obs_req_mb != null) begin
         m_obs_req_mb.get(bus_req);
      end
      else begin
         wait (0);
      end
   endtask : get_obs_req


   //-----------------------------------------------------------------------------------------------
   //
   // Task: get_obs_rsp
   //
   // This task retrieves a bus response object from the mailbox.  This task is intended to be used
   // by an external monitor (should it exist).  Note that this task is blocking; it will not return
   // until there is something to observe.  A bus response structure is returned as an output
   // variable upon completion.
   //
   virtual task get_obs_rsp(output evl_apb_bus_rsp bus_rsp);
      if (m_obs_rsp_mb != null) begin
         m_obs_rsp_mb.get(bus_rsp);
      end
      else begin
         wait (0);
      end
   endtask : get_obs_rsp


   //-----------------------------------------------------------------------------------------------
   //
   // Task: get_next_req
   //
   // This task retrieves the next bus request object to issue from the mailbox.  This task is used
   // by the evl_apb_interface module to get a request to issue.  Requests to issue should be sent
   // by a driver of some sort using the send_req() task.
   //
   virtual task get_next_req(output evl_apb_req_s apb_req_drv);
      evl_apb_bus_req bus_req;

      m_snd_req_mb.get(bus_req);
      apb_req_drv = bus_req.m_bus_req;
   endtask : get_next_req


   //-----------------------------------------------------------------------------------------------
   //
   // Task: get_next_rsp
   //
   // This task retrieves the next bus response object to issue from the mailbox.  This task is used
   // by the evl_apb_interface module to get a response to issue.  Responses to issue should be sent
   // by a driver of some sort using the send_rsp() task.
   //
   virtual task get_next_rsp(output evl_apb_rsp_s apb_rsp_drv);
      evl_apb_bus_rsp bus_rsp;

      m_snd_rsp_mb.get(bus_rsp);
      apb_rsp_drv = bus_rsp.m_bus_rsp;
   endtask : get_next_rsp


   //-----------------------------------------------------------------------------------------------
   //
   // Function: send_req
   //
   // This function is used to enqueue an APB request to send from an external APB request driver.
   // Before sending any requests, the function zero_req() should be issued once (to initialize APB
   // pins).
   //
   virtual function void send_req(input evl_apb_bus_req bus_req);
      bus_req.m_bus_req.valid = 1'b1;
      m_snd_req_mb.try_put(bus_req);
   endfunction : send_req


   //-----------------------------------------------------------------------------------------------
   //
   // Function: zero_req
   //
   // This function should be used by an external APB request driver at the start of a simulation.
   // It initializes the APB request pins (to prevent X propagation).
   //
   virtual function void zero_req();
      evl_apb_bus_req bus_req;

      bus_req = new();
      bus_req.m_bus_req.valid = 1'b0;
      m_snd_req_mb.try_put(bus_req);
   endfunction : zero_req


   //-----------------------------------------------------------------------------------------------
   //
   // Function: send_rsp
   //
   // This function is used to enqueue an APB response to send from an external APB response driver.
   // Before sending any responses, the function zero_rsp() should be issued once (to initialize APB
   // pins).
   //
   function void send_rsp(input evl_apb_bus_rsp bus_rsp);
      bus_rsp.m_bus_rsp.valid = 1'b1;
      m_snd_rsp_mb.try_put(bus_rsp);
   endfunction : send_rsp


   //-----------------------------------------------------------------------------------------------
   //
   // Function: zero_rsp
   //
   // This function should be used by an external APB response driver at the start of a simulation.
   // It initializes the APB response pins (to prevent X propagation).
   //
   function void zero_rsp();
      evl_apb_bus_rsp bus_rsp;

      bus_rsp = new();
      bus_rsp.m_bus_rsp.valid = 1'b0;
      m_snd_rsp_mb.try_put(bus_rsp);
   endfunction : zero_rsp


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string         full_name_in = "usr_apb_rtl_port",
                input evl_agent_id_t agent_id_in  = ~evl_agent_id_t'(0));
      m_agent_id       = agent_id_in;
      m_bus_req_mon    = new();
      m_bus_rsp_mon    = new();
      m_data_bus_width = 8;
      m_data_bus_clog  = $clog2(m_data_bus_width);

      m_snd_req_mb = new();
      m_snd_rsp_mb = new();

      //
      // If there is an external monitor available, mailboxes can be created by a call to
      // create_mailboxes().
      //
      // create_mailboxes();
   endfunction : new

endclass : usr_apb_rtl_port

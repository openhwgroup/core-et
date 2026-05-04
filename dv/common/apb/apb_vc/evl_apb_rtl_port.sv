//--------------------------------------------------------------------------------------------------
//
// Class: evl_apb_rtl_port
//
class evl_apb_rtl_port extends evl_rtl_port;

   // mailbox is for collecting bus requests/responses from the driver
   mailbox #(evl_apb_bus_req) m_snd_req_mb;
   mailbox #(evl_apb_bus_rsp) m_snd_rsp_mb;

   // mailboxes for monitoring/collecting bus requests from the interface to send to the monitor
   mailbox #(evl_apb_bus_req) m_obs_req_mb;
   mailbox #(evl_apb_bus_rsp) m_obs_rsp_mb;

   evl_apb_bus_req m_bus_req_mon;
   evl_apb_bus_rsp m_bus_rsp_mon;

   bit m_master_driver = 1'b0;
   bit m_slave_driver  = 1'b0;


   //-----------------------------------------------------------------------------------------------
   //
   // Function: capture_req
   //
   function void capture_req(input evl_apb_req_s apb_req_mon);
      m_bus_req_mon.m_bus_req = apb_req_mon;
      m_obs_req_mb.try_put(m_bus_req_mon);
   endfunction : capture_req


   //-----------------------------------------------------------------------------------------------
   //
   // Function: capture_rsp
   //
   function void capture_rsp(input evl_apb_rsp_s apb_rsp_mon);
      m_bus_rsp_mon.m_bus_rsp = apb_rsp_mon;
      m_obs_rsp_mb.try_put(m_bus_rsp_mon);
   endfunction : capture_rsp


   //-----------------------------------------------------------------------------------------------
   //
   // Task: get_obs_req
   //
   task get_obs_req(output evl_apb_bus_req bus_req);
      m_obs_req_mb.get(bus_req);
   endtask : get_obs_req


   //-----------------------------------------------------------------------------------------------
   //
   // Task: get_obs_rsp
   //
   task get_obs_rsp(output evl_apb_bus_rsp bus_rsp);
      m_obs_rsp_mb.get(bus_rsp);
   endtask : get_obs_rsp


   //-----------------------------------------------------------------------------------------------
   //
   // Task: get_next_req
   //
   task get_next_req(output evl_apb_req_s apb_req_drv);
      evl_apb_bus_req bus_req;

      m_snd_req_mb.get(bus_req);
      apb_req_drv = bus_req.m_bus_req;
   endtask : get_next_req


   //-----------------------------------------------------------------------------------------------
   //
   // Task: get_next_rsp
   //
   task get_next_rsp(output evl_apb_rsp_s apb_rsp_drv);
      evl_apb_bus_rsp bus_rsp;

      m_snd_rsp_mb.get(bus_rsp);
      apb_rsp_drv = bus_rsp.m_bus_rsp;
   endtask : get_next_rsp


   //-----------------------------------------------------------------------------------------------
   //
   // Function: send_req
   //
   function void send_req(input evl_apb_bus_req bus_req);
      bus_req.m_bus_req.valid = 1'b1;
      m_snd_req_mb.try_put(bus_req);
   endfunction : send_req


   //-----------------------------------------------------------------------------------------------
   //
   // Function: zero_req
   //
   function void zero_req();
      evl_apb_bus_req bus_req;

      bus_req = new();
      bus_req.m_bus_req.valid = 1'b0;
      m_snd_req_mb.try_put(bus_req);
   endfunction : zero_req


   //-----------------------------------------------------------------------------------------------
   //
   // Function: send_rsp
   //
   function void send_rsp(input evl_apb_bus_rsp bus_rsp);
      bus_rsp.m_bus_rsp.valid = 1'b1;
      m_snd_rsp_mb.try_put(bus_rsp);
   endfunction : send_rsp


   //-----------------------------------------------------------------------------------------------
   //
   // Function: zero_rsp
   //
   function void zero_rsp();
      evl_apb_bus_rsp bus_rsp;

      bus_rsp = new();
      bus_rsp.m_bus_rsp.valid = 1'b0;
      m_snd_rsp_mb.try_put(bus_rsp);
   endfunction : zero_rsp


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
   // Constructor
   //
   function new(input string         full_name_in     = "apb_rtl_port",
                input evl_agent_id_t agent_pid_in     = ~evl_agent_id_t'(0),
                input int            skip_levels_in[] = {});
      super.new(full_name_in, APB_BUS_AGENT, agent_pid_in, "evl_apb_verif_comp", "", "", "", skip_levels_in);
      m_snd_req_mb  = new();
      m_snd_rsp_mb  = new();
      m_obs_req_mb  = new();
      m_obs_rsp_mb  = new();
      m_bus_req_mon = new();
      m_bus_rsp_mon = new();
   endfunction : new

endclass : evl_apb_rtl_port

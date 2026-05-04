//--------------------------------------------------------------------------------------------------
//
// Class: evl_axi_rtl_port_base
//
class evl_axi_rtl_port_base extends evl_rtl_port;
   int m_data_bytes;
   int m_data_bytes_log2;

   mailbox #(evl_axi_bus_req) m_snd_req_mb;
   mailbox #(evl_axi_bus_rsp) m_snd_rsp_mb;

   //
   // Observed requests and responses
   //
   mailbox #(evl_axi_bus_req) m_obs_req_mb;
   mailbox #(evl_axi_bus_rsp) m_obs_rsp_mb;
   evl_axi_bus_req            ar_bus_req;
   evl_axi_bus_req            aw_bus_req;
   evl_axi_bus_rsp            r_bus_rsp;
   evl_axi_bus_rsp            b_bus_rsp;
   bit                        m_master_driver        = 1'b0;
   bit                        m_slave_driver         = 1'b0;
// bit                        m_master_driver_enable = 1'b0;
// bit                        m_slave_driver_enable  = 1'b0;


   //-----------------------------------------------------------------------------------------------
   //
   // Function: capture_req
   //
   function void capture_req(ref evl_axi_req_info_t req_info);
      evl_axi_bus_req bus_req;

      //
      // Because we can get only one read or write request per cycle, we can play a game here and
      // reuse the class object over and over.  The monitor consumes class objects immediately.
      //
      if (req_info.ax_is_read == 1'b1) begin
         bus_req = ar_bus_req;
      end
      else begin
         bus_req = aw_bus_req;
      end
      bus_req.m_data_only  = 1'b0;
      bus_req.m_xref_valid = 1'b0;
      bus_req.m_ax_id      = req_info.ax_id;
      bus_req.m_ax_addr    = req_info.ax_addr;
      bus_req.m_ax_len     = req_info.ax_len;
      bus_req.m_ax_size    = req_info.ax_size;
      bus_req.m_ax_burst   = req_info.ax_burst;
      bus_req.m_ax_lock    = req_info.ax_lock;
      bus_req.m_ax_cache   = req_info.ax_cache;
      bus_req.m_ax_prot    = req_info.ax_prot;
      bus_req.m_ax_qos     = req_info.ax_qos;
      bus_req.m_ax_region  = req_info.ax_region;
      bus_req.m_ax_user    = req_info.ax_user;
      bus_req.m_ax_vc      = req_info.ax_vc;
      bus_req.m_trans_id   = req_info.trans_id;
      void'(m_obs_req_mb.try_put(bus_req));
   endfunction : capture_req


   //-----------------------------------------------------------------------------------------------
   //
   // Function: capture_req_data
   //
   function void capture_req_data(ref evl_axi_w_data_info_t req_data);
      evl_axi_bus_req bus_req;

      //
      // Write data is different ... the monitor does NOT necessarily consume the class object
      // immediately, so we need to create a new one each time.
      //
      bus_req = new();
      bus_req.m_data_only  = 1'b1;
      bus_req.m_xref_valid = 1'b0;
      bus_req.m_req_type   = AXI_REQ_WRITE;
      bus_req.m_w_data     = req_data.w_data;
      bus_req.m_w_strb     = req_data.w_strb;
      bus_req.m_w_user     = req_data.w_user;
      bus_req.m_w_last     = req_data.w_last;
      bus_req.m_trans_id   = req_data.trans_id;
      void'(m_obs_req_mb.try_put(bus_req));
   endfunction : capture_req_data


   //-----------------------------------------------------------------------------------------------
   //
   // Function: capture_rsp
   //
   function void capture_rsp(ref evl_axi_rsp_info_t rsp_info);
      evl_axi_bus_rsp bus_rsp;

      if (rsp_info.x_is_read == 1'b1) begin
         bus_rsp = r_bus_rsp;
      end
      else begin
         bus_rsp = b_bus_rsp;
      end
      bus_rsp.m_x_id     = rsp_info.x_id;
      bus_rsp.m_x_resp   = rsp_info.x_resp;
      bus_rsp.m_x_data   = rsp_info.x_data;
      bus_rsp.m_x_last   = rsp_info.x_last;
      bus_rsp.m_x_user   = rsp_info.x_user;
      bus_rsp.m_trans_id = rsp_info.trans_id;
      void'(m_obs_rsp_mb.try_put(bus_rsp));
   endfunction : capture_rsp


   //-----------------------------------------------------------------------------------------------
   //
   // Task: get_obs_req
   //
   task get_obs_req(output evl_axi_bus_req bus_req);
      m_obs_req_mb.get(bus_req);
   endtask : get_obs_req


   //-----------------------------------------------------------------------------------------------
   //
   // Task: get_obs_rsp
   //
   task get_obs_rsp(output evl_axi_bus_rsp bus_rsp);
      m_obs_rsp_mb.get(bus_rsp);
   endtask : get_obs_rsp


   //-----------------------------------------------------------------------------------------------
   //
   // Function: put_req
   //
   // This function receives a bus request object from the driver and keeps it in the mailbox
   // until it is ready to be placed on the bus interface.
   //
   virtual function void put_req(input evl_axi_bus_req bus_req);
      void'(m_snd_req_mb.try_put(bus_req));
   endfunction : put_req


   //-----------------------------------------------------------------------------------------------
   //
   // Task: get_req
   //
   virtual task get_req(output evl_axi_bus_req bus_req);
      m_snd_req_mb.get(bus_req);
   endtask : get_req


   //-----------------------------------------------------------------------------------------------
   //
   // Function: put_rsp
   //
   // This function receives a bus response object from the driver and keeps it in the mailbox
   // until it is ready to be placed on the bus interface.
   //
   virtual function void put_rsp(input evl_axi_bus_rsp bus_rsp);
      void'(m_snd_rsp_mb.try_put(bus_rsp));
   endfunction : put_rsp


   //-----------------------------------------------------------------------------------------------
   //
   // Task: get_rsp
   //
   virtual task get_rsp(output evl_axi_bus_rsp bus_rsp);
      m_snd_rsp_mb.get(bus_rsp);
   endtask : get_rsp


   //-----------------------------------------------------------------------------------------------
   //
   // Function: init_master
   //
   virtual function void init_master();
      m_master_driver = 1'b1;
   endfunction : init_master


// //-----------------------------------------------------------------------------------------------
// //
// // Function: enable_master
// //
// virtual function void enable_master();
//    m_master_driver_enable = 1'b1;
// endfunction : enable_master


   //-----------------------------------------------------------------------------------------------
   //
   // Function: init_slave
   //
   virtual function void init_slave();
      m_slave_driver = 1'b1;
   endfunction : init_slave


// //-----------------------------------------------------------------------------------------------
// //
// // Function: enable_slave
// //
// virtual function void enable_slave();
//    m_slave_driver_enable = 1'b1;
// endfunction : enable_slave


   //-----------------------------------------------------------------------------------------------
   //
   // Task: wait_for_master_driver
   //
   virtual task wait_for_master_driver();
      #0 wait (m_master_driver == 1'b1);
   endtask : wait_for_master_driver


// //-----------------------------------------------------------------------------------------------
// //
// // Function: wait_for_master_driver_enable
// //
// virtual task wait_for_master_driver_enable();
//    #0 wait (m_master_driver_enable == 1'b1);
// endtask : wait_for_master_driver_enable


   //-----------------------------------------------------------------------------------------------
   //
   // Task: wait_for_slave_driver
   //
   virtual task wait_for_slave_driver();
      #0 wait (m_slave_driver == 1'b1);
   endtask : wait_for_slave_driver


// //-----------------------------------------------------------------------------------------------
// //
// // Function: wait_for_slave_driver_enable
// //
// virtual task wait_for_slave_driver_enable();
//    #0 wait (m_slave_driver_enable == 1'b1);
// endtask : wait_for_slave_driver_enable


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string           full_name_in     = "evl_rtl_port",
                input evl_agent_type_t agent_type_in    = NO_AGENT,
                input evl_agent_id_t   agent_pid_in     = ~evl_agent_id_t'(0),
                input string           sv_comp_in       = "",
                input string           param_list_in    = "",
                input string           name_prefix_in   = "",
                input string           name_postfix_in  = "",
                input int              skip_levels_in[] = {},
                input int              port_id_in       = 0,
                input int              max_port_id_in   = 0,
                input int              additional_hier  = 0);
      super.new(full_name_in, agent_type_in, agent_pid_in, sv_comp_in, param_list_in, name_prefix_in, name_postfix_in, skip_levels_in, port_id_in, max_port_id_in, additional_hier);
      m_obs_req_mb = new();
      m_obs_rsp_mb = new();
      m_snd_req_mb = new();
      m_snd_rsp_mb = new();
      ar_bus_req   = new();
      aw_bus_req   = new();
      r_bus_rsp    = new();
      b_bus_rsp    = new();
      ar_bus_req.m_data_only    = 1'b0;
      ar_bus_req.m_req_type     = AXI_REQ_READ;
      aw_bus_req.m_data_only    = 1'b0;
      aw_bus_req.m_req_type     = AXI_REQ_WRITE;
      r_bus_rsp.m_rsp_for_write = 1'b0;
      b_bus_rsp.m_rsp_for_write = 1'b1;
   endfunction : new

endclass : evl_axi_rtl_port_base

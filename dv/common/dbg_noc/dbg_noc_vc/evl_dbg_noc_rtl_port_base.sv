//--------------------------------------------------------------------------------------------------
//
// Class: evl_dbg_noc_rtl_port_base
//
class evl_dbg_noc_rtl_port_base extends evl_rtl_port;
   int m_data_bytes;
   int m_data_bytes_log2;

   mailbox #(evl_dbg_noc_bus_req) m_snd_tx_req_mb;

   //
   // Observed requests and responses
   //
   mailbox #(evl_dbg_noc_bus_req) m_obs_tx_req_mb;
   mailbox #(evl_dbg_noc_bus_req) m_obs_rx_req_mb;
   mailbox #(bit)                 m_obs_rsp_mb;
   evl_dbg_noc_bus_req            m_bus_tx_req;
   evl_dbg_noc_bus_req            m_bus_rx_req;


   function void put_mon_tx_req(ref evl_dbg_noc_req_info_t tx_req);
      evl_dbg_noc_bus_req bus_req;

      //
      // Because we can get only one read or write request per cycle, we can play a game here and
      // reuse the class object over and over.  The monitor consumes class objects immediately.
      //
      bus_req = m_bus_tx_req;
      bus_req.m_req_addr      = tx_req.req_addr;
      bus_req.m_req_dest_int  = tx_req.req_dest_int;;
      bus_req.m_req_qos       = tx_req.req_qos;
      bus_req.m_req_eop       = tx_req.req_eop;
      bus_req.m_req_data      = tx_req.req_data;
      bus_req.m_trans_id      = tx_req.trans_id;
      void'(m_obs_tx_req_mb.try_put(bus_req));
   endfunction : put_mon_tx_req

   function void put_mon_rx_req(ref evl_dbg_noc_req_info_t rx_req);
      evl_dbg_noc_bus_req bus_req;

      //
      // Because we can get only one read or write request per cycle, we can play a game here and
      // reuse the class object over and over.  The monitor consumes class objects immediately.
      //
      bus_req = m_bus_rx_req;
      bus_req.m_req_addr      = rx_req.req_addr;
      bus_req.m_req_dest_int  = rx_req.req_dest_int;;
      bus_req.m_req_qos       = rx_req.req_qos;
      bus_req.m_req_eop       = rx_req.req_eop;
      bus_req.m_req_data      = rx_req.req_data;
      bus_req.m_trans_id      = rx_req.trans_id;
      void'(m_obs_rx_req_mb.try_put(bus_req));
   endfunction : put_mon_rx_req

   function void put_mon_resp(bit rsp);
      void'(m_obs_rsp_mb.try_put(rsp));
   endfunction : put_mon_resp

   task get_obs_tx_req(output evl_dbg_noc_bus_req bus_req);
      m_obs_tx_req_mb.get(bus_req);
   endtask : get_obs_tx_req

   task get_obs_rx_req(output evl_dbg_noc_bus_req bus_req);
      m_obs_rx_req_mb.get(bus_req);
   endtask : get_obs_rx_req

   task get_obs_rsp(output bit bus_rsp);
      m_obs_rsp_mb.get(bus_rsp);
   endtask : get_obs_rsp

   //--------------------------------------------------------------------------------------------
   //
   // Function: put_tx_req
   //
   // This function receives a bus request object from the driver and keeps it in the mailbox
   // until it is ready to be placed on the bus interface.
   //
   virtual function void put_tx_req(input evl_dbg_noc_bus_req bus_req);
      void'(m_snd_tx_req_mb.try_put(bus_req));
   endfunction : put_tx_req


   //-----------------------------------------------------------------------------------------------
   //
   // Task: get_tx_req
   //
   virtual task get_tx_req(output evl_dbg_noc_bus_req bus_req);
      m_snd_tx_req_mb.get(bus_req);
   endtask : get_tx_req


   virtual task zero_req_values();
   endtask : zero_req_values

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
      m_obs_tx_req_mb   = new();
      m_obs_rx_req_mb   = new();
      m_obs_rsp_mb      = new();
      m_snd_tx_req_mb   = new();
      m_bus_tx_req      = new();
      m_bus_rx_req      = new();
   endfunction : new

endclass : evl_dbg_noc_rtl_port_base

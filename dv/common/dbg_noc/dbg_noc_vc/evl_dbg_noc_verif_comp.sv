//--------------------------------------------------------------------------------------------------
//
// Class: evl_dbg_noc_verif_comp
//
// The DBG_NOC Verification component contains a master driver, a slave driver, and a monitor.
// The drivers are not always present. For example, if this verification component is being used as an
// active master, then it will contain a master driver and a monitor, and it will receive
// requests to drive on the DBG_NOC bus from a master stim agent. Likewise if it is being used as
// an active slave, then it will contain a slave driver and a monitor, and it will send observed
// requests to a slave stim agent (which contains a memory model), and the slave stim agent will
// send this verification component a response to drive on the bus.
//
// This verification component may also be passive, which means it won't drive anything onto the
// bus. In that case it will just contain a monitor.
//
class evl_dbg_noc_verif_comp extends evl_verif_comp;

   `uvm_component_utils(evl_dbg_noc_verif_comp)

   //-----------------------------------------------------------------------------------------------
   //
   // Static Member Variables and Objects
   //
   `evl_param_key(pk_data_printed)
   `evl_param_key(pk_data_width)
   `evl_param_key(pk_ignore_low_addr_req_desc)
   `evl_param_key(pk_no_active_cpu)
   `evl_param_key(pk_sc_dbg_noc_error_intensity)


   //-----------------------------------------------------------------------------------------------
   //
   // Member Variables and Objects
   //
   bit                   m_saw_request;
   bit                   m_saw_response;

   int                   m_data_width;
   int                   m_data_width_clog2;
   int                   m_data_width_in_dwords;
   int                   m_req_dword_start;
   evl_req_desc          m_last_obs_write_req;
   int                   m_active_reads;
   int                   m_active_tx_reqs;
   bit                   m_issue_dut_error_on_error_rsp;

   evl_dbg_noc_master_agent  m_master_agent;
   evl_dbg_noc_slave_agent   m_slave_agent;
   evl_dbg_noc_monitor       m_monitor;

   // port for receiving requests from stimulus agent.
   uvm_analysis_export   #(evl_req_desc) m_sa_snd_tx_req_export;
   uvm_tlm_analysis_fifo #(evl_req_desc) m_sa_snd_tx_req_fifo;
   mailbox               #(evl_req_desc) m_snd_tx_req_queue;

   // port for receiving responses from stimulus agent.
   uvm_analysis_export   #(evl_req_desc) m_sa_snd_rsp_export;

   // port for sending responses back to the stimulus agent.
   uvm_analysis_port     #(evl_req_desc) m_sa_rcv_req_port;
   uvm_analysis_port     #(evl_req_desc) m_sa_rcv_rsp_port;

   // ports for sending observed requests and responses from monitor to the rest of the world
   uvm_analysis_port     #(evl_req_desc) m_obs_tx_req_port;
   uvm_analysis_port     #(evl_req_desc) m_obs_rx_req_port;
   uvm_analysis_port     #(evl_req_desc) m_obs_rsp_port;
   uvm_tlm_analysis_fifo #(evl_req_desc) m_obs_tx_req_fifo;
   uvm_tlm_analysis_fifo #(evl_req_desc) m_obs_rx_req_fifo;
   mailbox               #(evl_req_desc) m_obs_tx_req_queue;
   mailbox               #(evl_req_desc) m_obs_rx_req_queue;

   uvm_tlm_analysis_fifo #(evl_req_desc) m_exp_rsp_fifo;


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_abstract_name
   //
   virtual function void set_abstract_name(input string name_in);
      super.set_abstract_name(name_in);
      if (m_monitor != null) begin
         m_monitor.set_abstract_name($sformatf("%0s(DBG_NOC-MON)", get_root_abstract_name()));
      end
      if (m_master_agent != null) begin
         m_master_agent.set_abstract_name($sformatf("%0s(DBG_NOC)", get_root_abstract_name()));
      end
      if (m_slave_agent != null) begin
         m_slave_agent.set_abstract_name($sformatf("%0s(DBG_NOC)", get_root_abstract_name()));
      end
   endfunction : set_abstract_name


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_req_desc_port
   //
   virtual function uvm_analysis_port#(evl_req_desc) get_req_desc_port(input string port_name      = "unknown",
                                                                       input int    port_number    = 0,
                                                                       input int    subport_number = 0);
      if ((port_name == "m_obs_req_port") || (port_name == "obs_req_port") || (port_name == "obs_req")) begin
         `dut_warning(get_abstract_name(), $sformatf("%s is an invalid port_name", port_name), pk_sc_dbg_noc_error_intensity, { VMOD_DBG_NOC_SB }, m_rtl_port.get_rtl_name())
         return m_obs_tx_req_port;
      end
      else if ((port_name == "m_obs_tx_req_port") || (port_name == "obs_tx_req_port") || (port_name == "obs_tx_req")) begin
         return m_obs_tx_req_port;
      end
      else if ((port_name == "m_obs_rx_req_port") || (port_name == "obs_rx_req_port") || (port_name == "obs_rx_req")) begin
         return m_obs_rx_req_port;
      end
      else if ((port_name == "m_obs_rsp_port") || (port_name == "obs_rsp_port") || (port_name == "obs_rsp")) begin
         return m_obs_rsp_port;
      end
      else if ((port_name == "m_sa_rcv_rsp_port") || (port_name == "sa_rcv_rsp") || (port_name == "m_rcv_rsp_port")) begin
         return m_sa_rcv_rsp_port;
      end
      return null;
   endfunction : get_req_desc_port


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_req_desc_export
   //
   virtual function uvm_analysis_export#(evl_req_desc) get_req_desc_export(input string port_name      = "unknown",
                                                                           input int    port_number    = 0,
                                                                           input int    subport_number = 0);
      if ((port_name == "sa_snd_req") || (port_name == "m_sa_snd_req_export")) begin
         return m_sa_snd_tx_req_export;
      end
      if ((port_name == "sa_snd_rsp") || (port_name == "m_sa_snd_rsp_export")) begin
         return m_sa_snd_rsp_export;
      end
      return null;
   endfunction : get_req_desc_export


   //-----------------------------------------------------------------------------------------------
   //
   // sort_sent_and_obs_req_descs
   //
   // The purpose of this task is to sort the incoming stimulus agent request descriptors and
   // incoming observed request descriptors into separate read & write fifos.  Since DBG_NOC doesn't
   // guarantee ordering between AR and AW, we need these sorted to be able to match up stimulus
   // request descriptors and observed request descriptors in the join_sent_and_obs_req_descs task
   // below.
   //
   task sort_sent_and_obs_req_descs;
      evl_req_desc snd_req;
      evl_req_desc obs_tx_req;
      evl_req_desc obs_rx_req;

      fork
         if (m_sa_snd_tx_req_fifo != null) begin
            forever begin
               m_sa_snd_tx_req_fifo.get(snd_req);
               `evl_log_mdesc(UVM_HIGH, VMOD_DBG_NOC_MON, get_abstract_name(), snd_req, $sformatf("sorting sent write request (%0s)", snd_req.sprint_obj()))
               m_snd_tx_req_queue.put(snd_req);
            end
         end
         if (m_obs_tx_req_fifo != null) begin
            forever begin
               m_obs_tx_req_fifo.get(obs_tx_req);
               `evl_log_mdesc(UVM_HIGH, VMOD_DBG_NOC_MON, get_abstract_name(), obs_tx_req, $sformatf("sorting observed Tx request (%0s)", obs_tx_req.sprint_obj()))
               m_obs_tx_req_queue.put(obs_tx_req);
            end
         end
         if (m_obs_rx_req_fifo != null) begin
            forever begin
               m_obs_rx_req_fifo.get(obs_rx_req);
               `evl_log_mdesc(UVM_HIGH, VMOD_DBG_NOC_MON, get_abstract_name(), obs_rx_req, $sformatf("sorting observed Rx request (%0s)", obs_rx_req.sprint_obj()))
               m_obs_rx_req_queue.put(obs_rx_req);
            end
         end
      join
   endtask : sort_sent_and_obs_req_descs


   //-----------------------------------------------------------------------------------------------
   //
   // process_observed_tx_requests
   //
   // The purpose of this task is to connect up the request descriptor from the stimulus agent with
   // the corresponding observed request descriptor from the monitor.  They need to be linked so
   // that when the observed response gets back to the master stimulus agent, it can put the data in
   // the right place and set done and all that.  It turns out the observed request is going to be a
   // child of the sent request.
   //
   virtual task process_observed_tx_requests;
      evl_req_desc obs_req;
      evl_req_desc exp_req;

      fork
         forever begin
            bit              matched;

            m_obs_tx_req_queue.get(obs_req);
            exp_req = null;
            matched       = 1'b1;
            m_saw_request = 1'b1;
            `evl_log_mdesc(UVM_HIGH, VMOD_DBG_NOC_MON, get_abstract_name(), obs_req, $sformatf("saw this observed Tx request: %s", obs_req.sprint_obj()))
            if (is_active_master() == 1'b0) begin

               `evl_log_mdesc(UVM_MEDIUM, VMOD_GLOBAL, get_abstract_name(), obs_req, obs_req.sprint_obj())
               if (obs_req.do_parent_check() == 1'b1) begin
                  exp_req = obs_req.get_parent_req_desc();
                  if (exp_req == null) begin
                     if (get_int_param(pk_ignore_low_addr_req_desc, 0) == 0) begin
                        `dut_error_mdesc(get_abstract_name(), obs_req, $sformatf("%0s is an unexpected request", obs_req.sprint_obj()), pk_sc_dbg_noc_error_intensity, { VMOD_DBG_NOC_SB }, m_rtl_port.get_rtl_name())
                     end
                     else begin
                        `dut_warning_mdesc(get_abstract_name(), obs_req, $sformatf("%0s is an unexpected request", obs_req.sprint_obj()), pk_sc_dbg_noc_error_intensity, { VMOD_DBG_NOC_SB }, m_rtl_port.get_rtl_name())
                        obs_req.set_int_param(pk_ignore_low_addr_req_desc, 1);
                     end
                  end
                  else begin
                     if ( (exp_req.get_bus_cmd() != obs_req.get_bus_cmd()) ||
                          (exp_req.get_paddr()   != obs_req.get_paddr()) ) begin
                        `dut_error_mdesc(get_abstract_name(), obs_req, $sformatf("expected %0s, not %0s", exp_req.sprint_obj(), obs_req.sprint_obj()), pk_sc_dbg_noc_error_intensity, { VMOD_DBG_NOC_SB }, m_rtl_port.get_rtl_name())
                        matched = 1'b0;
                     end
                     if (exp_req.get_bus_qos() != obs_req.get_bus_qos()) begin
                        `dut_error_mdesc(get_abstract_name(), obs_req, $sformatf("expected DBG_NOC QOS of 0x%x for %0s, not 0x%x", exp_req.get_bus_qos(), obs_req.sprint_obj(), obs_req.get_bus_qos()), pk_sc_dbg_noc_error_intensity, { VMOD_DBG_NOC_SB }, m_rtl_port.get_rtl_name())
                     end
                  end
               end
            end
            else if (m_snd_tx_req_queue.num() == 0) begin
               `evl_log_mdesc(UVM_MEDIUM, VMOD_GLOBAL, get_abstract_name(), obs_req, obs_req.sprint_obj())
               `dut_error_mdesc(get_abstract_name(), obs_req, $sformatf("TB_ISSUE: process_observed_tx_requests() not expecting m_snd_tx_req_queue to be empty (%0s, line %0d)", `__FILE__, `__LINE__))
            end
            else begin
               m_snd_tx_req_queue.get(exp_req);

               // These are supposed to match. Do a couple of quick checks.
               if ((exp_req.get_paddr() == obs_req.get_paddr())) begin
                  // The observed request is going to be the child of the sent request
                  void'(exp_req.add_child_desc(obs_req));
                  obs_req.set_correlated();
                  `evl_log_mdesc(UVM_MEDIUM, VMOD_GLOBAL, get_abstract_name(), obs_req, obs_req.sprint_obj())
               end
               else begin
                  `evl_log_mdesc(UVM_MEDIUM, VMOD_GLOBAL, get_abstract_name(), obs_req, obs_req.sprint_obj())
                  `dut_error(get_abstract_name(), $sformatf("process_observed_tx_requests() expected sent request and observed request to match but they didn't: sent: %s ... observed: %s", exp_req.sprint_obj(), obs_req.sprint_obj()))
                  matched = 1'b0;
               end
            end
            if (matched == 1'b0) begin
               exp_req = null;
            end
            if (exp_req != null) begin
               exp_req.set_issued();
               if (is_active_master() == 1'b1) begin
                  evl_mem_desc root_desc = exp_req.get_root_desc();

                  // Reset the timeout value now that the request has been issued:
                  if (root_desc != null) begin
                     root_desc.reset_timeout_value();
                  end
               end
            end
            if (obs_req != null) begin
               obs_req.set_issued();
            end
            m_obs_tx_req_port.write(obs_req);
            m_active_tx_reqs++;
         end
      join
   endtask : process_observed_tx_requests

   //-----------------------------------------------------------------------------------------------
   //
   // process_observed_rx_requests
   //
   // The purpose of this task is to connect up the request descriptor from the stimulus agent with
   // the corresponding observed request descriptor from the monitor.  They need to be linked so
   // that when the observed response gets back to the master stimulus agent, it can put the data in
   // the right place and set done and all that.  It turns out the observed request is going to be a
   // child of the sent request.
   //
   virtual task process_observed_rx_requests;
      evl_req_desc obs_req;

      fork
         forever begin
            m_obs_rx_req_queue.get(obs_req);
            `evl_log_mdesc(UVM_HIGH, VMOD_DBG_NOC_MON, get_abstract_name(), obs_req, $sformatf("saw this observed Rx request: %s", obs_req.sprint_obj()))
            if (obs_req != null) begin
               obs_req.set_issued();
            end
            m_obs_rx_req_port.write(obs_req);
         end
      join
   endtask : process_observed_rx_requests


   //-----------------------------------------------------------------------------------------------
   //
   // Function: process_observed_responses
   //
   // The purpose of this task is to receive observed responses from the monitor, transfer
   // information (ex: rsp data) to the original request descriptor sent by the stimulus agent, and
   // send the request descriptor back to the stimulus agent.
   //
   task process_observed_responses;
      evl_req_desc exp_req_desc;
      evl_req_desc obs_req_desc;
      int          dword_start;
      int          dword_end;
      bit          x_last;
      string       err_messages[$];

      fork
         forever begin
            err_messages = {};
            m_exp_rsp_fifo.get(exp_req_desc);
            m_saw_response = 1'b1;
            if (exp_req_desc != null) begin
               if (exp_req_desc.bus_cmd_is_write() == 1'b1) begin
                  if (m_active_tx_reqs > 0) begin
                     m_active_tx_reqs--;
                  end
               end
               else begin
                  if (m_active_reads > 0) begin
                     m_active_reads--;
                  end
               end
            end
            `evl_log_mdesc(UVM_HIGH, VMOD_DBG_NOC_MON, get_abstract_name(), exp_req_desc, $sformatf("process_observed_responses received this exp_req_desc %s", exp_req_desc.sprint_obj()))

            obs_req_desc = exp_req_desc.get_parent_req_desc();
            if (obs_req_desc == null) begin
               obs_req_desc = exp_req_desc;
            end

            //
            // Print the observed response.
            //
            //`evl_log_mdesc(UVM_MEDIUM, VMOD_GLOBAL, get_abstract_name(), obs_req_desc, $sformatf("%s", obs_req_desc.sprint_rsp()))

            //
            // Pop the current burst value for read data.
            //
            if (obs_req_desc.bus_cmd_is_write() == 1'b0) begin
               x_last = obs_req_desc.pop_cur_burst();
            end
            else begin
               x_last = 1'b1;
            end

            //
            // If this is the last response as expected, remove the descriptor from the active list.
            //
            if (x_last == 1'b1) begin
               delete_active_mem_desc(exp_req_desc);
            end

            if ((obs_req_desc.get_x_last() != x_last) && (obs_req_desc.bus_cmd_is_read() == 1'b1)) begin
               `dut_error_mdesc(get_abstract_name(), obs_req_desc, $sformatf("expected RLAST to be %d, not %b (%s)", x_last, obs_req_desc.get_x_last(), obs_req_desc.sprint_obj()), pk_sc_dbg_noc_error_intensity, { VMOD_DBG_NOC_SB }, m_rtl_port.get_rtl_name())
            end

            //
            // If this is the last response, we can perform bus checks, etc.
            //
            if (x_last == 1'b1) begin
               if (exp_req_desc.has_been_observed() == 1'b1) begin
                  evl_line_data      obs_rsp_data;
                  evl_line_data      exp_rsp_data;
                  evl_bus_rsp_type_t obs_rsp_type;
                  evl_bus_rsp_type_t exp_rsp_type;

                  obs_rsp_type = obs_req_desc.get_bus_rsp_type();
                  exp_rsp_type = exp_req_desc.get_bus_rsp_type();
                  if (obs_rsp_type != exp_rsp_type) begin
                     `dut_error_mdesc(get_abstract_name(), exp_req_desc, $sformatf("exp_req_desc (%s) has rsp_type 0x%x but obs_req_desc (%s) has rsp_type 0x%x", exp_req_desc.sprint_obj(), exp_rsp_type, obs_req_desc.sprint_obj(), obs_rsp_type), pk_sc_dbg_noc_error_intensity, { VMOD_DBG_NOC_SB }, m_rtl_port.get_rtl_name())
                     exp_req_desc.print_from_root();
                  end

                  if ((m_issue_dut_error_on_error_rsp == 1'b1) && (obs_req_desc.bus_rsp_has_error() == 1'b1)) begin
                     `dut_error(get_abstract_name(), "Received an unexpected Error response")
                  end

                  obs_rsp_data = obs_req_desc.get_rsp_data();
                  exp_rsp_data = exp_req_desc.get_rsp_data();
                  if ((obs_rsp_data != null) && (exp_rsp_data != null)) begin
                     string line_err_messages[];

                     if (obs_rsp_data.compare_data(exp_rsp_data, line_err_messages) == 1'b1) begin
                        foreach (line_err_messages[ii]) begin
                           err_messages.push_back(line_err_messages[ii]);
                        end
                     end
                  end
                  else if ((obs_rsp_data != null) && (exp_rsp_data == null)) begin
                     `dut_error_mdesc(get_abstract_name(), exp_req_desc, $sformatf("obs_rsp_data (%s) is not null but exp_rsp_data (%s) is and that's bad", obs_req_desc.sprint_obj(), exp_req_desc.sprint_obj()), pk_sc_dbg_noc_error_intensity, { VMOD_DBG_NOC_SB }, m_rtl_port.get_rtl_name())
                  end
                  else if ((obs_rsp_data == null) && (exp_rsp_data != null)) begin
                      `dut_error_mdesc(get_abstract_name(), exp_req_desc, $sformatf("obs_rsp_data (%s) is null but exp_rsp_data (%s) is not and that's bad", obs_req_desc.sprint_obj(), exp_req_desc.sprint_obj()), pk_sc_dbg_noc_error_intensity, { VMOD_DBG_NOC_SB }, m_rtl_port.get_rtl_name())
                  end
               end

               foreach (err_messages[ii]) begin
                  `dut_error_mdesc(get_abstract_name(), obs_req_desc, err_messages[ii], pk_sc_dbg_noc_error_intensity, { VMOD_DBG_NOC_SB }, m_rtl_port.get_rtl_name())
               end
               if (m_obs_rsp_port != null) begin
                  if (obs_req_desc.get_int_param(pk_ignore_low_addr_req_desc, 0) == 0) begin
                     m_obs_rsp_port.write(obs_req_desc);
                  end
               end
               if (m_sa_rcv_rsp_port != null) begin
                  m_sa_rcv_rsp_port.write(obs_req_desc);
               end
            end
         end
      join
   endtask: process_observed_responses


   //-----------------------------------------------------------------------------------------------
   //
   // Function: connect_my_port_to_export
   //
   virtual function void connect_my_port_to_export(input uvm_analysis_export#(evl_req_desc) export_in,
                                                   input string                             port_name      = "unknown",
                                                   input int                                port_number    = 0,
                                                   input int                                subport_number = 0);
      uvm_analysis_port#(evl_req_desc) req_desc_port;

      req_desc_port = get_req_desc_port(port_name);
      if ((req_desc_port != null) && (export_in != null)) begin
         `evl_log(UVM_MEDIUM, VMOD_CONFIG, get_abstract_name(), $sformatf("connecting %0s -> %0s", req_desc_port.get_full_name(), export_in.get_full_name()))
         req_desc_port.connect(export_in);
      end
   endfunction : connect_my_port_to_export


   //------------------------------------------------------------------------------------------------
   //
   // build_phase
   //
   virtual function void build_phase(uvm_phase phase);
      super.build_phase(phase);

      if (m_rtl_port == null) begin
         `dut_error(get_abstract_name(), "dbg_noc_verif_comp: m_rtl_port is null, SAD!")
         m_data_width          = 64;
      end
      else begin
         m_data_width          = m_rtl_port.get_int_param_by_name("pk_data_width", 64);
         if (m_data_width < 1) begin
            m_data_width = 1;
         end
      end
      m_data_width_clog2     = $clog2(m_data_width);
      m_data_width_in_dwords = m_data_width / `EVL_DWORD_BYTES;

      m_monitor = evl_dbg_noc_monitor::type_id::create("m_monitor", this);
      m_monitor.m_verif_comp = this;
      m_monitor.m_port_type  = get_port_type();
      m_monitor.m_subport_id = get_subport_id();
      m_monitor.set_rtl_port(get_rtl_port());

      if (is_active_master() == 1'b1) begin
         m_master_agent = evl_dbg_noc_master_agent::type_id::create("m_master_agent", this);
         m_master_agent.set_rtl_port(get_rtl_port());
      end
      else if (is_active_slave() == 1'b1) begin
         m_slave_agent = evl_dbg_noc_slave_agent::type_id::create("m_slave_agent", this);
         m_slave_agent.set_rtl_port(get_rtl_port());
      end

      m_snd_tx_req_queue = new();
      m_obs_tx_req_queue = new();
      m_obs_rx_req_queue = new();

      m_obs_tx_req_port = new("m_obs_tx_req_port", this);
      m_obs_rx_req_port = new("m_obs_rx_req_port", this);
      m_obs_rsp_port = new("m_obs_rsp_port", this);
      m_exp_rsp_fifo = new("m_exp_rsp_fifo", this);

      if (is_active_master() == 1'b1) begin
         m_obs_tx_req_fifo       = new("m_obs_tx_req_fifo", this);
         m_obs_rx_req_fifo       = new("m_obs_rx_req_fifo", this);
         m_sa_snd_tx_req_export  = new("m_sa_snd_tx_req_export", this);
         m_sa_rcv_rsp_port       = new("m_sa_rcv_rsp_port", this);
         m_sa_snd_tx_req_fifo    = new("m_sa_snd_tx_req_fifo", this);
      end
      else begin
         if (this_is_master_port_agent() == 1'b1) begin
            m_obs_tx_req_fifo = new("m_obs_tx_req_fifo", this);
            m_obs_rx_req_fifo = new("m_obs_rx_req_fifo", this);
         end
         if (is_active_slave() == 1'b1) begin
            m_sa_snd_rsp_export = new("m_sa_snd_rsp_export", this);
         end
      end

      if (m_master_agent != null) begin
         m_master_agent.set_abstract_name($sformatf("%0s(DBG_NOC)", get_root_abstract_name()));
      end
      if (m_slave_agent != null) begin
         m_slave_agent.set_abstract_name($sformatf("%0s(DBG_NOC)", get_root_abstract_name()));
      end
      if (m_monitor != null) begin
         m_monitor.set_abstract_name($sformatf("%0s(DBG_NOC-MON)", get_root_abstract_name()));
      end
   endfunction : build_phase


   //-----------------------------------------------------------------------------------------------
   //
   // connect_phase
   //
   virtual function void connect_phase(uvm_phase phase);
      super.connect_phase(phase);
      if (m_master_agent != null) begin
         m_sa_snd_tx_req_export.connect(m_master_agent.m_sa_snd_req_export);
      end
      if (m_slave_agent != null) begin
         m_sa_snd_rsp_export.connect(m_slave_agent.m_sa_snd_rsp_export);
      end
      if (m_sa_snd_tx_req_fifo != null) begin
         m_sa_snd_tx_req_export.connect(m_sa_snd_tx_req_fifo.analysis_export);
      end
      if (m_obs_tx_req_fifo != null) begin
         m_monitor.m_obs_tx_req_port.connect(m_obs_tx_req_fifo.analysis_export);
      end
      if (m_obs_rx_req_fifo != null) begin
         m_monitor.m_obs_rx_req_port.connect(m_obs_rx_req_fifo.analysis_export);
      end
      if (m_exp_rsp_fifo != null) begin
         m_monitor.m_exp_rsp_port.connect(m_exp_rsp_fifo.analysis_export);
      end
   endfunction: connect_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Task: run_phase
   //
   virtual task run_phase(uvm_phase phase);
      if (m_rtl_port == null) begin
         return;
      end
      if (m_monitor != null) begin
         m_monitor.set_master_port_agent(get_master_port_agent());
         m_monitor.set_slave_port_agent(get_slave_port_agent());
      end
      fork
         super.run_phase(phase);
         sort_sent_and_obs_req_descs();
         process_observed_tx_requests();
         process_observed_rx_requests();
         process_observed_responses();
      join_none
   endtask: run_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Function: shutdown_phase
   //
   virtual task shutdown_phase(uvm_phase phase);
      fork
         begin
            super.shutdown_phase(phase);
         end
         begin
            phase.raise_objection(this);
            wait_for_active_mem_desc(m_global_attrs.get_timeout_value(ShutdownPhase), 1'b1, 1'b0, 1'b1);
            phase.drop_objection(this);
         end
      join
   endtask : shutdown_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string name_in = "evl_verif_comp", input uvm_component parent_in = null);
      super.new(name_in, parent_in, DBG_NOC_BUS_AGENT);
      if (`et_test_plusargs("disableDBG_NOCChecks") != 0) begin
         `evl_set_error_intensity(pk_sc_dbg_noc_error_intensity, UVM_HIGH)
      end
      if (`et_test_plusargs("enableDBG_NOCChecks") != 0) begin
         `evl_set_error_intensity(pk_sc_dbg_noc_error_intensity, -1)
      end

      m_active_tx_reqs     = 0;
      m_req_dword_start    = 0;
      m_last_obs_write_req = null;
      m_err_pk             = pk_sc_dbg_noc_error_intensity;
   endfunction : new

endclass : evl_dbg_noc_verif_comp

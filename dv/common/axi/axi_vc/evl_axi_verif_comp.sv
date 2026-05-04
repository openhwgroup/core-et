//--------------------------------------------------------------------------------------------------
//
// Class: evl_axi_verif_comp
//
// The AXI Verification component contains a master driver, a slave driver, and a monitor.
// The drivers are not always present. For example, if this verification component is being used as an
// active master, then it will contain a master driver and a monitor, and it will receive
// requests to drive on the AXI bus from a master stim agent. Likewise if it is being used as
// an active slave, then it will contain a slave driver and a monitor, and it will send observed
// requests to a slave stim agent (which contains a memory model), and the slave stim agent will
// send this verification component a response to drive on the bus.
//
// This verification component may also be passive, which means it won't drive anything onto the
// bus. In that case it will just contain a monitor.
//
class evl_axi_verif_comp extends evl_verif_comp;

   `uvm_component_utils(evl_axi_verif_comp)

   //-----------------------------------------------------------------------------------------------
   //
   // Static Member Variables and Objects
   //
   `evl_param_key(pk_allow_duplicate_outstanding_ids)
   `evl_param_key(pk_data_printed)
   `evl_param_key(pk_data_width)
   `evl_param_key(pk_ignore_low_addr_req_desc)
   `evl_param_key(pk_ignore_data_on_err)
   `evl_param_key(pk_no_active_cpu)
   `evl_param_key(pk_read_vc_credits)
   `evl_param_key(pk_sc_axi_error_intensity)
   `evl_param_key(pk_write_vc_credits)


   //-----------------------------------------------------------------------------------------------
   //
   // Member Variables and Objects
   //
   bit                   m_saw_request;
   bit                   m_saw_response;

   int                   m_data_width;
   int                   m_data_width_clog2;
   int                   m_data_width_in_dwords;
   int                   m_read_channel_count;
   int                   m_write_channel_count;
   int                   m_req_dword_start;
   evl_req_desc          m_last_obs_write_req;
   int                   m_active_reads;
   int                   m_active_writes;
   bit                   m_issue_dut_error_on_error_rsp;

   evl_axi_master_agent  m_master_agent;
   evl_axi_slave_agent   m_slave_agent;
   evl_axi_monitor       m_monitor;
   evl_axi_cov_base      m_axi_cov;

   // port for receiving requests from stimulus agent.
   uvm_analysis_export   #(evl_req_desc) m_sa_snd_req_export;
   uvm_tlm_analysis_fifo #(evl_req_desc) m_sa_snd_req_fifo;
   mailbox               #(evl_req_desc) m_snd_write_req_queues[];
   mailbox               #(evl_req_desc) m_snd_read_req_queues[];

   // port for receiving responses from stimulus agent.
   uvm_analysis_export   #(evl_req_desc) m_sa_snd_rsp_export;

   // port for sending responses back to the stimulus agent.
   uvm_analysis_port     #(evl_req_desc) m_sa_rcv_req_port;
   uvm_analysis_port     #(evl_req_desc) m_sa_rcv_rsp_port;

   // ports for sending observed requests and responses from monitor to the rest of the world
   uvm_analysis_port     #(evl_req_desc) m_obs_req_port;
   uvm_analysis_port     #(evl_req_desc) m_obs_rsp_port;
   uvm_tlm_analysis_fifo #(evl_req_desc) m_obs_req_fifo;
   mailbox               #(evl_req_desc) m_obs_write_req_queue;
   mailbox               #(evl_req_desc) m_obs_write_data_queue[int];
   mailbox               #(evl_req_desc) m_obs_read_req_queue;

   uvm_tlm_analysis_fifo #(evl_req_desc) m_exp_rsp_fifo;


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_abstract_name
   //
   virtual function void set_abstract_name(input string name_in);
      super.set_abstract_name(name_in);
      if (m_monitor != null) begin
         m_monitor.set_abstract_name($sformatf("%0s(AXI-MON)", get_root_abstract_name()));
      end
      if (m_master_agent != null) begin
         m_master_agent.set_abstract_name($sformatf("%0s(AXI)", get_root_abstract_name()));
      end
      if (m_slave_agent != null) begin
         m_slave_agent.set_abstract_name($sformatf("%0s(AXI)", get_root_abstract_name()));
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
         return m_obs_req_port;
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
         return m_sa_snd_req_export;
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
   // incoming observed request descriptors into separate read & write fifos.  Since AXI doesn't
   // guarantee ordering between AR and AW, we need these sorted to be able to match up stimulus
   // request descriptors and observed request descriptors in the join_sent_and_obs_req_descs task
   // below.
   //
   task sort_sent_and_obs_req_descs;
      evl_req_desc snd_req;
      evl_req_desc obs_req;

      fork
         if (m_sa_snd_req_fifo != null) begin
            forever begin
               m_sa_snd_req_fifo.get(snd_req);
               if (snd_req.get_bus_cmd() == AXI_REQ_WRITE) begin
                  evl_bus_req_vc_t vc;

                  `evl_log_mdesc(UVM_HIGH, VMOD_AXI_MON, get_abstract_name(), snd_req, $sformatf("sorting sent write request (%0s)", snd_req.sprint_obj()))
                  vc = snd_req.get_vc();
                  if (int'(vc) >= m_write_channel_count) begin
                     `evl_log_mdesc(UVM_HIGH, VMOD_AXI_MON, get_abstract_name(), snd_req, $sformatf("VC for write request, %0d, is higher than write channel count, %0d. Setting VC to %0d (%0s)", vc, m_write_channel_count, evl_bus_req_vc_t'(0), snd_req.sprint_obj()))
                     vc = evl_bus_req_vc_t'(0);
                  end
                  else begin
                     `evl_log_mdesc(UVM_HIGH, VMOD_AXI_MON, get_abstract_name(), snd_req, $sformatf("write request sent on VC %0d (%0s)", vc, snd_req.sprint_obj()))
                  end
                  m_snd_write_req_queues[vc].put(snd_req);
               end
               else begin
                  evl_bus_req_vc_t vc;

                  `evl_log_mdesc(UVM_HIGH, VMOD_AXI_MON, get_abstract_name(), snd_req, $sformatf("sorting sent read request (%0s)", snd_req.sprint_obj()))
                  vc = snd_req.get_vc();
                  if (int'(vc) >= m_read_channel_count) begin
                     `evl_log_mdesc(UVM_HIGH, VMOD_AXI_MON, get_abstract_name(), snd_req, $sformatf("VC for read request, %0d, is higher than read channel count, %0d. Setting VC to %0d (%0s)", vc, m_read_channel_count, evl_bus_req_vc_t'(0), snd_req.sprint_obj()))
                     vc = evl_bus_req_vc_t'(0);
                  end
                  else begin
                     `evl_log_mdesc(UVM_HIGH, VMOD_AXI_MON, get_abstract_name(), snd_req, $sformatf("read request sent on VC %0d (%0s)", vc, snd_req.sprint_obj()))
                  end
                  m_snd_read_req_queues[vc].put(snd_req);
               end
            end
         end
         if (m_obs_req_fifo != null) begin
            forever begin
               m_obs_req_fifo.get(obs_req);
               if (obs_req.bus_cmd_is_write() == 1'b1) begin
                  `evl_log_mdesc(UVM_HIGH, VMOD_AXI_MON, get_abstract_name(), obs_req, $sformatf("sorting observed write request (%0s)", obs_req.sprint_obj()))
                  m_obs_write_req_queue.put(obs_req);
               end
               else begin
                  `evl_log_mdesc(UVM_HIGH, VMOD_AXI_MON, get_abstract_name(), obs_req, $sformatf("sorting observed read request (%0s)", obs_req.sprint_obj()))
                  m_obs_read_req_queue.put(obs_req);
               end
            end
         end
      join
   endtask : sort_sent_and_obs_req_descs


   //-----------------------------------------------------------------------------------------------
   //
   // process_observed_write_data_task
   //
   task process_observed_write_data_task(input int vc);
      evl_req_desc  obs_write_req;
      evl_req_desc  exp_write_req;
      int           cur_burst;
      int           req_size;
      int           dword_start;
      int           dword_end;
      evl_line_data exp_req_data;
      evl_line_data obs_req_data;

      forever begin
         //
         // Get the next write request descriptor.
         //
         m_obs_write_data_queue[vc].get(obs_write_req);

         cur_burst   = 0;
         dword_start = ((obs_write_req.get_paddr() & (`EVL_LINE_BYTES - 1)) / m_data_width) * m_data_width_in_dwords;
         while (1) begin
            dword_end = dword_start + m_data_width_in_dwords - 1;
            obs_write_req.wait_for_burst_data(cur_burst);
            `evl_log_mdesc(UVM_MEDIUM, VMOD_GLOBAL, get_abstract_name(), obs_write_req, $sformatf("W Data: 0x%0s%0s", obs_write_req.format_req_data(dword_start, dword_end), ((cur_burst < obs_write_req.get_bus_req_len()) ? "" : " (last)")))

            //
            // Once the first data chunk is available, perform an initial check to make sure
            // there is no address/size/byte_en mismatch.
            //
            if (cur_burst == 0) begin
               req_size = int'(1 << obs_write_req.get_bus_req_size());
               if (req_size < m_data_width) begin
                  int                 byte_en_shift = (obs_write_req.get_paddr() & (`EVL_LINE_BYTES - 1));
                  evl_xword_byte_en_t obs_byte_ens;
                  evl_xword_byte_en_t exp_byte_ens;

                  obs_req_data = obs_write_req.get_req_data(0);
                  obs_byte_ens = obs_req_data.get_xword_valids(0);
                  exp_byte_ens = ((evl_xword_byte_en_t'(1) << req_size) - evl_xword_byte_en_t'(1)) << byte_en_shift;
                  if ((~exp_byte_ens & obs_byte_ens) != evl_xword_byte_en_t'(0)) begin
                     `dut_error_mdesc(get_abstract_name(), obs_write_req, $sformatf("address/size/byte_en mismatch for %0s (0x%x, %0d byte%0s, 0x%x)", obs_write_req.sprint_obj(), obs_write_req.get_paddr(), req_size, ((req_size == 1) ? "" : "s"), obs_byte_ens), pk_sc_axi_error_intensity, { VMOD_AXI_SB }, m_rtl_port.get_rtl_name())
                  end
               end
            end
            dword_start = dword_end + 1;
            cur_burst++;
            if (cur_burst > obs_write_req.get_bus_req_len()) begin
               obs_write_req.set_have_all_data();
               break;
            end
         end

         if (obs_write_req.do_parent_check() == 1'b1) begin
            exp_write_req = obs_write_req.get_parent_req_desc();
            if (exp_write_req != null) begin
               int limit;

               limit = int'(obs_write_req.get_max_line_burst_idx());
               for (int ii = 0; ii <= limit; ii++) begin
                  exp_req_data = exp_write_req.get_req_data(ii);
                  obs_req_data = obs_write_req.get_req_data(ii);
                  if (exp_req_data == null) begin
                     `dut_error_mdesc(get_abstract_name(), exp_write_req, $sformatf("TB_FAIL: request data for expected request is null (%0s, %0s, line %0d)", exp_write_req.sprint_obj(), `__FILE__, `__LINE__), pk_sc_axi_error_intensity, { VMOD_AXI_SB }, m_rtl_port.get_rtl_name())
                     exp_write_req.print_from_root();
                  end
                  else if (obs_req_data == null) begin
                     `dut_error_mdesc(get_abstract_name(), obs_write_req, $sformatf("request data for %0s is null", obs_write_req.sprint_obj()), pk_sc_axi_error_intensity, { VMOD_AXI_SB }, m_rtl_port.get_rtl_name())
                     obs_write_req.print_from_root();
                  end
                  else begin
                     string err_messages[$];

                     err_messages = {};
                     if (obs_req_data.compare_data_and_byte_ens(exp_req_data, err_messages) == 1'b1) begin
                        foreach (err_messages[jj]) begin
                           `dut_error_mdesc(get_abstract_name(), obs_write_req, err_messages[jj], pk_sc_axi_error_intensity, { VMOD_AXI_SB }, m_rtl_port.get_rtl_name())
                        end
                     end
                  end
               end
            end
         end
      end
   endtask : process_observed_write_data_task


   //-----------------------------------------------------------------------------------------------
   //
   // process_observed_write_data
   //
   function void process_observed_write_data(input int vc);
      fork
         process_observed_write_data_task(vc);
      join_none
   endfunction : process_observed_write_data


   //-----------------------------------------------------------------------------------------------
   //
   // process_observed_requests
   //
   // The purpose of this task is to connect up the request descriptor from the stimulus agent with
   // the corresponding observed request descriptor from the monitor.  They need to be linked so
   // that when the observed response gets back to the master stimulus agent, it can put the data in
   // the right place and set done and all that.  It turns out the observed request is going to be a
   // child of the sent request.
   //
   virtual task process_observed_requests;
      evl_req_desc obs_write_req;
      evl_req_desc exp_write_req;
      evl_req_desc obs_read_req;
      evl_req_desc exp_read_req;
      evl_req_desc obs_write_data_req;
      int          source_id;

      source_id = get_inst_id();
      for (int ii = 0; ii < m_write_channel_count; ii++) begin
         m_obs_write_data_queue[ii] = new();
         process_observed_write_data(ii);
      end
      fork
         //
         // All write data is in order, so this simplifies printing a bit ... we don't need to pair
         // up data with an ID and handle any out-of-order cases.
         //

         forever begin
            bit              matched;
            evl_bus_req_vc_t vc;

            m_obs_write_req_queue.get(obs_write_req);
            vc = obs_write_req.get_vc();
            if (int'(vc) >= m_write_channel_count) begin
               vc = evl_bus_req_vc_t'(0);
            end
            exp_write_req = null;
            matched       = 1'b1;
            m_saw_request = 1'b1;
            `evl_log_mdesc(UVM_HIGH, VMOD_AXI_MON, get_abstract_name(), obs_write_req, $sformatf("saw this observed write request: %s", obs_write_req.sprint_obj()))
            if (is_active_master() == 1'b0) begin
               evl_paddr_t req_mask = (evl_paddr_t'(1) << obs_write_req.get_bus_req_size()) - 1;

               `evl_log_mdesc(UVM_MEDIUM, VMOD_GLOBAL, get_abstract_name(), obs_write_req, obs_write_req.sprint_obj())
               if ((req_mask & obs_write_req.get_paddr()) != evl_paddr_t'(0)) begin
                  `dut_warning_mdesc(get_abstract_name(), obs_write_req, $sformatf("AXI AWSIZE of %0d (%0d byte%0s) is invalid for an address of 0x%x (%0s)", obs_write_req.get_bus_req_size(), (1 << obs_write_req.get_bus_req_size()), ((obs_write_req.get_bus_req_size() == 0) ? "" : "s"), obs_write_req.get_paddr(), obs_write_req.sprint_obj()), pk_sc_axi_error_intensity, { VMOD_AXI_SB }, m_rtl_port.get_rtl_name())
               end
               if (obs_write_req.do_parent_check() == 1'b1) begin
                  exp_write_req = obs_write_req.get_parent_req_desc();
                  if (exp_write_req == null) begin
                     if (get_int_param(pk_ignore_low_addr_req_desc, 0) == 0) begin
                        `dut_error_mdesc(get_abstract_name(), obs_write_req, $sformatf("%0s is an unexpected request", obs_write_req.sprint_obj()), pk_sc_axi_error_intensity, { VMOD_AXI_SB }, m_rtl_port.get_rtl_name())
                     end
                     else begin
                        `dut_warning_mdesc(get_abstract_name(), obs_write_req, $sformatf("%0s is an unexpected request", obs_write_req.sprint_obj()), pk_sc_axi_error_intensity, { VMOD_AXI_SB }, m_rtl_port.get_rtl_name())
                        obs_write_req.set_int_param(pk_ignore_low_addr_req_desc, 1);
                     end
                  end
                  else begin
                     if ( (exp_write_req.get_bus_cmd() != obs_write_req.get_bus_cmd()) ||
                          (exp_write_req.get_paddr()   != obs_write_req.get_paddr()) ) begin
                        `dut_error_mdesc(get_abstract_name(), obs_write_req, $sformatf("expected %0s, not %0s", exp_write_req.sprint_obj(), obs_write_req.sprint_obj()), pk_sc_axi_error_intensity, { VMOD_AXI_SB }, m_rtl_port.get_rtl_name())
                        matched = 1'b0;
                     end
                     if (exp_write_req.get_bus_req_size() != obs_write_req.get_bus_req_size()) begin
                        `dut_error_mdesc(get_abstract_name(), obs_write_req, $sformatf("expected AXI AWSIZE of %0d (%0d byte%0s) for %0s, not %0d (%0d byte%0s)", exp_write_req.get_bus_req_size(), (1 << exp_write_req.get_bus_req_size()), ((exp_write_req.get_bus_req_size() == 0) ? "" : "s"), obs_write_req.sprint_obj(), obs_write_req.get_bus_req_size(), (1 << obs_write_req.get_bus_req_size()), ((obs_write_req.get_bus_req_size() == 0) ? "" : "s")), pk_sc_axi_error_intensity, { VMOD_AXI_SB }, m_rtl_port.get_rtl_name())
                     end
                     else if (int'(obs_write_req.get_bus_req_size()) > m_data_width_clog2) begin
                        `dut_error_mdesc(get_abstract_name(), obs_write_req, $sformatf("AXI AWSIZE of %0d (%0d byte%0s) for %0s is wider than the bus", obs_write_req.get_bus_req_size(), (1 << obs_write_req.get_bus_req_size()), ((obs_write_req.get_bus_req_size() == 0) ? "" : "s"), obs_write_req.sprint_obj()), pk_sc_axi_error_intensity, { VMOD_AXI_SB }, m_rtl_port.get_rtl_name())
                     end
                     if (exp_write_req.get_bus_req_len() != obs_write_req.get_bus_req_len()) begin
                        `dut_error_mdesc(get_abstract_name(), obs_write_req, $sformatf("expected AXI AWLEN of 0x%x for %0s, not 0x%x", exp_write_req.get_bus_req_len(), obs_write_req.sprint_obj(), obs_write_req.get_bus_req_len()), pk_sc_axi_error_intensity, { VMOD_AXI_SB }, m_rtl_port.get_rtl_name())
                     end
                     if (exp_write_req.get_bus_lock() != obs_write_req.get_bus_lock()) begin
                        `dut_error_mdesc(get_abstract_name(), obs_write_req, $sformatf("expected AXI AWLOCK of %0d for %0s, not %0d", exp_write_req.get_bus_lock(), obs_write_req.sprint_obj(), obs_write_req.get_bus_lock()), pk_sc_axi_error_intensity, { VMOD_AXI_SB }, m_rtl_port.get_rtl_name())
                     end
                     if (exp_write_req.get_bus_req_id() != obs_write_req.get_bus_req_id()) begin
                        `dut_error_mdesc(get_abstract_name(), obs_write_req, $sformatf("expected AXI AWID of 0x%x for %0s, not 0x%x", exp_write_req.get_bus_req_id(), obs_write_req.sprint_obj(), obs_write_req.get_bus_req_id()), pk_sc_axi_error_intensity, { VMOD_AXI_SB }, m_rtl_port.get_rtl_name())
                     end
                     if (exp_write_req.get_bus_req_user() != obs_write_req.get_bus_req_user()) begin
                        `dut_error_mdesc(get_abstract_name(), obs_write_req, $sformatf("expected AXI AWUSER of 0x%x for %0s, not 0x%x", exp_write_req.get_bus_req_user(), obs_write_req.sprint_obj(), obs_write_req.get_bus_req_user()), pk_sc_axi_error_intensity, { VMOD_AXI_SB }, m_rtl_port.get_rtl_name())
                     end
                     if (exp_write_req.get_bus_qos() != obs_write_req.get_bus_qos()) begin
                        `dut_error_mdesc(get_abstract_name(), obs_write_req, $sformatf("expected AXI AWQOS of 0x%x for %0s, not 0x%x", exp_write_req.get_bus_qos(), obs_write_req.sprint_obj(), obs_write_req.get_bus_qos()), pk_sc_axi_error_intensity, { VMOD_AXI_SB }, m_rtl_port.get_rtl_name())
                     end
                  end
               end
               m_global_attrs.bus_trigger(obs_write_req.get_paddr(), obs_write_req.get_bus_req_id(), source_id);
            end
            else if (m_snd_write_req_queues[vc].num() == 0) begin
               `evl_log_mdesc(UVM_MEDIUM, VMOD_GLOBAL, get_abstract_name(), obs_write_req, obs_write_req.sprint_obj())
               `dut_error_mdesc(get_abstract_name(), obs_write_req, $sformatf("TB_ISSUE: process_observed_requests() not expecting m_snd_write_req_queues[%0d] to be empty (%0s, line %0d)", vc, `__FILE__, `__LINE__))
            end
            else begin
               m_snd_write_req_queues[vc].get(exp_write_req);

               // These are supposed to match. Do a couple of quick checks.
               if ((exp_write_req.get_bus_cmd()    == obs_write_req.get_bus_cmd()) &&
                   (exp_write_req.get_paddr()      == obs_write_req.get_paddr())   &&
                   (exp_write_req.get_bus_req_id() == obs_write_req.get_bus_req_id())) begin
                  // The observed request is going to be the child of the sent request
                  void'(exp_write_req.add_child_desc(obs_write_req));
                  obs_write_req.set_correlated();
                  `evl_log_mdesc(UVM_MEDIUM, VMOD_GLOBAL, get_abstract_name(), obs_write_req, obs_write_req.sprint_obj())
               end
               else begin
                  `evl_log_mdesc(UVM_MEDIUM, VMOD_GLOBAL, get_abstract_name(), obs_write_req, obs_write_req.sprint_obj())
                  `dut_error(get_abstract_name(), $sformatf("process_observed_requests() expected sent request and observed request to match but they didn't: sent: %s ... observed: %s", exp_write_req.sprint_obj(), obs_write_req.sprint_obj()))
                  matched = 1'b0;
               end
            end
            if (obs_write_req != null) begin
               m_obs_write_data_queue[vc].put(obs_write_req);
            end
            if (matched == 1'b0) begin
               exp_write_req = null;
            end
            if (exp_write_req != null) begin
               exp_write_req.set_issued();
               if (is_active_master() == 1'b1) begin
                  evl_mem_desc root_desc = exp_write_req.get_root_desc();

                  // Reset the timeout value now that the request has been issued:
                  if (root_desc != null) begin
                     root_desc.reset_timeout_value();
                  end
               end
            end
            if (obs_write_req != null) begin
               obs_write_req.set_issued();
            end
            m_obs_req_port.write(obs_write_req);
            m_active_writes++;
            if ((m_active_writes < 255) && (m_axi_cov != null)) begin
               #0 m_axi_cov.sample_aw_active_count(m_active_writes);
            end
         end

         forever begin
            bit              matched;
            evl_bus_req_vc_t vc;

            m_obs_read_req_queue.get(obs_read_req);
            vc = obs_read_req.get_vc();
            if (int'(vc) >= m_read_channel_count) begin
               vc = evl_bus_req_vc_t'(0);
            end
            exp_read_req  = null;
            matched       = 1'b1;
            m_saw_request = 1'b1;
            `evl_log_mdesc(UVM_HIGH, VMOD_AXI_MON, get_abstract_name(), obs_read_req, $sformatf("saw this observed read request: %s", obs_read_req.sprint_obj()))
            if (is_active_master() == 1'b0) begin
               evl_paddr_t req_mask = (evl_paddr_t'(1) << obs_read_req.get_bus_req_size()) - 1;

               `evl_log_mdesc(UVM_MEDIUM, VMOD_GLOBAL, get_abstract_name(), obs_read_req, obs_read_req.sprint_obj())
               if ((req_mask & obs_read_req.get_paddr()) != evl_paddr_t'(0)) begin
                  `dut_warning_mdesc(get_abstract_name(), obs_read_req, $sformatf("AXI ARSIZE of %0d (%0d byte%0s) is invalid for an address of 0x%x (%0s)", obs_read_req.get_bus_req_size(), (1 << obs_read_req.get_bus_req_size()), ((obs_read_req.get_bus_req_size() == 0) ? "" : "s"), obs_read_req.get_paddr(), obs_read_req.sprint_obj()), pk_sc_axi_error_intensity, { VMOD_AXI_SB }, m_rtl_port.get_rtl_name())
               end
               if (obs_read_req.do_parent_check() == 1'b1) begin
                  exp_read_req = obs_read_req.get_parent_req_desc();
                  if (exp_read_req == null) begin
                     if (get_int_param(pk_ignore_low_addr_req_desc, 0) == 0) begin
                        `dut_error_mdesc(get_abstract_name(), obs_read_req, $sformatf("%0s is an unexpected request", obs_read_req.sprint_obj()), pk_sc_axi_error_intensity, { VMOD_AXI_SB }, m_rtl_port.get_rtl_name())
                     end
                     else begin
                        `dut_warning_mdesc(get_abstract_name(), obs_read_req, $sformatf("%0s is an unexpected request", obs_read_req.sprint_obj()), pk_sc_axi_error_intensity, { VMOD_AXI_SB }, m_rtl_port.get_rtl_name())
                        obs_read_req.set_int_param(pk_ignore_low_addr_req_desc, 1);
                     end
                  end
                  else begin
                     if ( (exp_read_req.get_bus_cmd() != obs_read_req.get_bus_cmd()) ||
                          (exp_read_req.get_paddr()   != obs_read_req.get_paddr()) ) begin
                        `dut_error_mdesc(get_abstract_name(), obs_read_req, $sformatf("expected %0s, not %0s", exp_read_req.sprint_obj(), obs_read_req.sprint_obj()), pk_sc_axi_error_intensity, { VMOD_AXI_SB }, m_rtl_port.get_rtl_name())
                        matched = 1'b0;
                     end
                     if (exp_read_req.get_bus_req_size() != obs_read_req.get_bus_req_size()) begin
                        `dut_error_mdesc(get_abstract_name(), obs_read_req, $sformatf("expected AXI ARSIZE of %0d (%0d byte%0s) for %0s, not %0d (%0d byte%0s)", exp_read_req.get_bus_req_size(), (1 << exp_read_req.get_bus_req_size()), ((exp_read_req.get_bus_req_size() == 0) ? "" : "s"), obs_read_req.sprint_obj(), obs_read_req.get_bus_req_size(), (1 << obs_read_req.get_bus_req_size()), ((obs_read_req.get_bus_req_size() == 0) ? "" : "s")), pk_sc_axi_error_intensity, { VMOD_AXI_SB }, m_rtl_port.get_rtl_name())
                     end
                     else if (int'(obs_read_req.get_bus_req_size()) > m_data_width_clog2) begin
                        `dut_error_mdesc(get_abstract_name(), obs_read_req, $sformatf("AXI ARSIZE of %0d (%0d byte%0s) for %0s is wider than the bus", obs_read_req.get_bus_req_size(), (1 << obs_read_req.get_bus_req_size()), ((obs_read_req.get_bus_req_size() == 0) ? "" : "s"), obs_read_req.sprint_obj()), pk_sc_axi_error_intensity, { VMOD_AXI_SB }, m_rtl_port.get_rtl_name())
                     end
                     if (exp_read_req.get_bus_req_len() != obs_read_req.get_bus_req_len()) begin
                        `dut_error_mdesc(get_abstract_name(), obs_read_req, $sformatf("expected AXI ARLEN of 0x%x for %0s, not 0x%x", exp_read_req.get_bus_req_len(), obs_read_req.sprint_obj(), obs_read_req.get_bus_req_len()), pk_sc_axi_error_intensity, { VMOD_AXI_SB }, m_rtl_port.get_rtl_name())
                     end
                     if (exp_read_req.get_bus_lock() != obs_read_req.get_bus_lock()) begin
                        `dut_error_mdesc(get_abstract_name(), obs_read_req, $sformatf("expected AXI ARLOCK of %0d for %0s, not %0d", exp_read_req.get_bus_lock(), obs_read_req.sprint_obj(), obs_read_req.get_bus_lock()), pk_sc_axi_error_intensity, { VMOD_AXI_SB }, m_rtl_port.get_rtl_name())
                     end
                     if (exp_read_req.get_bus_req_id() != obs_read_req.get_bus_req_id()) begin
                        `dut_error_mdesc(get_abstract_name(), obs_read_req, $sformatf("expected AXI ARID of 0x%x for %0s, not 0x%x", exp_read_req.get_bus_req_id(), obs_read_req.sprint_obj(), obs_read_req.get_bus_req_id()), pk_sc_axi_error_intensity, { VMOD_AXI_SB }, m_rtl_port.get_rtl_name())
                     end
                     if (exp_read_req.get_bus_req_user() != obs_read_req.get_bus_req_user()) begin
                        `dut_error_mdesc(get_abstract_name(), obs_read_req, $sformatf("expected AXI ARUSER of 0x%x for %0s, not 0x%x", exp_read_req.get_bus_req_user(), obs_read_req.sprint_obj(), obs_read_req.get_bus_req_user()), pk_sc_axi_error_intensity, { VMOD_AXI_SB }, m_rtl_port.get_rtl_name())
                     end
                     if (exp_read_req.get_bus_qos() != obs_read_req.get_bus_qos()) begin
                        `dut_error_mdesc(get_abstract_name(), obs_read_req, $sformatf("expected AXI ARQOS of 0x%x for %0s, not 0x%x", exp_read_req.get_bus_qos(), obs_read_req.sprint_obj(), obs_read_req.get_bus_qos()), pk_sc_axi_error_intensity, { VMOD_AXI_SB }, m_rtl_port.get_rtl_name())
                     end
                  end
               end
               m_global_attrs.bus_trigger(obs_read_req.get_paddr(), obs_read_req.get_bus_req_id(), source_id);
            end
            else if (m_snd_read_req_queues[vc].num() == 0) begin
               `evl_log_mdesc(UVM_MEDIUM, VMOD_GLOBAL, get_abstract_name(), obs_read_req, obs_read_req.sprint_obj())
               `dut_error_mdesc(get_abstract_name(), obs_read_req, $sformatf("TB_ISSUE: process_observed_requests() not expecting m_snd_read_req_queues[%0d] to be empty (%0s, line %0d)", vc, `__FILE__, `__LINE__))
            end
            else begin
               m_snd_read_req_queues[vc].get(exp_read_req);

               // These are supposed to match. Do a couple of quick checks.
               if ((exp_read_req.get_bus_cmd()    == obs_read_req.get_bus_cmd()) &&
                   (exp_read_req.get_paddr()      == obs_read_req.get_paddr())   &&
                   (exp_read_req.get_bus_req_id() == obs_read_req.get_bus_req_id())) begin
                  // The observed request is going to be the child of the sent request
                  void'(exp_read_req.add_child_desc(obs_read_req));
                  obs_read_req.set_correlated();
                  `evl_log_mdesc(UVM_MEDIUM, VMOD_GLOBAL, get_abstract_name(), obs_read_req, obs_read_req.sprint_obj())
               end
               else begin
                  `evl_log_mdesc(UVM_MEDIUM, VMOD_GLOBAL, get_abstract_name(), obs_read_req, obs_read_req.sprint_obj())
                  `dut_error(get_abstract_name(), $sformatf("process_observed_requests() expected exp_read_req and obs_read_req to match but they didn't: exp_read_req: %s 0x%x (id 0x%x) ... obs_read_req: %s 0x%x (id 0x%x, %0s)", exp_read_req.get_bus_cmd_name(), exp_read_req.get_paddr(), exp_read_req.get_bus_req_id(), obs_read_req.get_bus_cmd_name(), obs_read_req.get_paddr(), obs_read_req.get_bus_req_id(), m_rtl_port.get_rtl_name()))
                  matched = 1'b0;
               end
            end
            if (matched == 1'b0) begin
               exp_read_req = null;
            end
            if (exp_read_req != null) begin
               exp_read_req.set_issued();
               if (is_active_master() == 1'b1) begin
                  evl_mem_desc root_desc = exp_read_req.get_root_desc();

                  // Reset the timeout value now that the request has been issued:
                  if (root_desc != null) begin
                     root_desc.reset_timeout_value();
                  end
               end
            end
            if (obs_read_req != null) begin
               obs_read_req.set_issued();
            end
            m_obs_req_port.write(obs_read_req);
            m_active_reads++;
            if ((m_active_reads < 255) && (m_axi_cov != null)) begin
               #0 m_axi_cov.sample_ar_active_count(m_active_reads);
            end
         end
      join
   endtask : process_observed_requests


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
                  if (m_active_writes > 0) begin
                     m_active_writes--;
                  end
               end
               else begin
                  if (m_active_reads > 0) begin
                     m_active_reads--;
                  end
               end
            end
            `evl_log_mdesc(UVM_HIGH, VMOD_AXI_MON, get_abstract_name(), exp_req_desc, $sformatf("process_observed_responses received this exp_req_desc %s", exp_req_desc.sprint_obj()))

            obs_req_desc = exp_req_desc.get_parent_req_desc();
            if (obs_req_desc == null) begin
               obs_req_desc = exp_req_desc;
            end

            //
            // Print the observed response.
            //
            if (exp_req_desc.bus_cmd_is_write() == 1'b1) begin
               `evl_log_mdesc(UVM_MEDIUM, VMOD_GLOBAL, get_abstract_name(), obs_req_desc, $sformatf("%s", obs_req_desc.sprint_rsp()))
            end
            else begin
               dword_start = (((obs_req_desc.get_paddr() & (`EVL_LINE_BYTES - 1)) / m_data_width) + obs_req_desc.get_cur_burst()) * m_data_width_in_dwords;
               dword_end   = dword_start + m_data_width_in_dwords - 1;
               `evl_log_mdesc(UVM_MEDIUM, VMOD_GLOBAL, get_abstract_name(), obs_req_desc, $sformatf("%s%0s", obs_req_desc.sprint_rsp(dword_start, dword_end), ((obs_req_desc.get_x_last() == 1'b1) ? " (last)" : "")))
            end

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
               `dut_error_mdesc(get_abstract_name(), obs_req_desc, $sformatf("expected RLAST to be %d, not %b (%s)", x_last, obs_req_desc.get_x_last(), obs_req_desc.sprint_obj()), pk_sc_axi_error_intensity, { VMOD_AXI_SB }, m_rtl_port.get_rtl_name())
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
                     `dut_error_mdesc(get_abstract_name(), exp_req_desc, $sformatf("exp_req_desc (%s) has rsp_type 0x%x but obs_req_desc (%s) has rsp_type 0x%x", exp_req_desc.sprint_obj(), exp_rsp_type, obs_req_desc.sprint_obj(), obs_rsp_type), pk_sc_axi_error_intensity, { VMOD_AXI_SB }, m_rtl_port.get_rtl_name())
                     exp_req_desc.print_from_root();
                  end

                  // FUTURE: We also need to check the error state of the response
                  // if we aren't expecting error responses then issue an error message
                  if ((m_issue_dut_error_on_error_rsp == 1'b1) && (obs_req_desc.bus_rsp_has_error() == 1'b1)) begin
                     `dut_error(get_abstract_name(), "Received an unexpected Error response")
                  end

                  obs_rsp_data = obs_req_desc.get_rsp_data();
                  exp_rsp_data = exp_req_desc.get_rsp_data();
                  if ((obs_req_desc.bus_rsp_has_error() == 1'b0) || (get_int_param(pk_ignore_data_on_err, 0) == 0)) begin
                     if ((obs_rsp_data != null) && (exp_rsp_data != null)) begin
                        string line_err_messages[];

                        if (obs_rsp_data.compare_data(exp_rsp_data, line_err_messages) == 1'b1) begin
                           foreach (line_err_messages[ii]) begin
                              err_messages.push_back(line_err_messages[ii]);
                           end
                        end
                     end
                     else if ((obs_rsp_data != null) && (exp_rsp_data == null)) begin
                        `dut_error_mdesc(get_abstract_name(), exp_req_desc, $sformatf("obs_rsp_data (%s) is not null but exp_rsp_data (%s) is and that's bad", obs_req_desc.sprint_obj(), exp_req_desc.sprint_obj()), pk_sc_axi_error_intensity, { VMOD_AXI_SB }, m_rtl_port.get_rtl_name())
                     end
                     else if ((obs_rsp_data == null) && (exp_rsp_data != null)) begin
                         `dut_error_mdesc(get_abstract_name(), exp_req_desc, $sformatf("obs_rsp_data (%s) is null but exp_rsp_data (%s) is not and that's bad", obs_req_desc.sprint_obj(), exp_req_desc.sprint_obj()), pk_sc_axi_error_intensity, { VMOD_AXI_SB }, m_rtl_port.get_rtl_name())
                     end
                  end
               end

               foreach (err_messages[ii]) begin
                  `dut_error_mdesc(get_abstract_name(), obs_req_desc, err_messages[ii], pk_sc_axi_error_intensity, { VMOD_AXI_SB }, m_rtl_port.get_rtl_name())
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


   //-----------------------------------------------------------------------------------------------
   //
   // build_phase
   //
   virtual function void build_phase(uvm_phase phase);
      super.build_phase(phase);

      if (m_rtl_port == null) begin
         `dut_error(get_abstract_name(), "axi_verif_comp: m_rtl_port is null, SAD!")
         m_data_width          = 64;
         m_read_channel_count  = 1;
         m_write_channel_count = 1;
      end
      else begin
         m_data_width          = m_rtl_port.get_int_param_by_name("pk_data_width", 64);
         m_read_channel_count  = m_rtl_port.get_int_param_by_name("pk_read_vchannels", 1);
         m_write_channel_count = m_rtl_port.get_int_param_by_name("pk_write_vchannels", 1);
         if (m_data_width < 1) begin
            m_data_width = 1;
         end
         if (m_read_channel_count < 1) begin
            m_read_channel_count = 1;
         end
         if (m_write_channel_count < 1) begin
            m_write_channel_count = 1;
         end
      end
      m_data_width_clog2     = $clog2(m_data_width);
      m_data_width_in_dwords = m_data_width / `EVL_DWORD_BYTES;

      m_monitor = evl_axi_monitor::type_id::create("m_monitor", this);
      m_monitor.m_verif_comp = this;
      m_monitor.m_port_type  = get_port_type();
      m_monitor.m_subport_id = get_subport_id();
      m_monitor.set_rtl_port(get_rtl_port());

      if (is_active_master() == 1'b1) begin
         m_master_agent = evl_axi_master_agent::type_id::create("m_master_agent", this);
         m_master_agent.set_rtl_port(get_rtl_port());
      end
      else if (is_active_slave() == 1'b1) begin
         m_slave_agent = evl_axi_slave_agent::type_id::create("m_slave_agent", this);
         m_slave_agent.set_rtl_port(get_rtl_port());
      end

      m_snd_read_req_queues = new[m_read_channel_count];
      for (int ii = 0; ii < m_read_channel_count; ii++) begin
         m_snd_read_req_queues[ii] = new();
      end
      m_snd_write_req_queues = new[m_write_channel_count];
      for (int ii = 0; ii < m_write_channel_count; ii++) begin
         m_snd_write_req_queues[ii] = new();
      end
      m_obs_write_req_queue = new();
      m_obs_read_req_queue  = new();

      m_obs_req_port = new("m_obs_req_port", this);
      m_obs_rsp_port = new("m_obs_rsp_port", this);
      m_exp_rsp_fifo = new("m_exp_rsp_fifo", this);

      if (is_active_master() == 1'b1) begin
         m_obs_req_fifo      = new("m_obs_req_fifo", this);
         m_sa_snd_req_export = new("m_sa_snd_req_export", this);
         m_sa_rcv_rsp_port   = new("m_sa_rcv_rsp_port", this);
         m_sa_snd_req_fifo   = new("m_sa_snd_req_fifo", this);
      end
      else begin
         if (this_is_master_port_agent() == 1'b1) begin
            m_obs_req_fifo = new("m_obs_req_fifo", this);
         end
         if (is_active_slave() == 1'b1) begin
            m_sa_snd_rsp_export = new("m_sa_snd_rsp_export", this);
         end
      end

      if (m_master_agent != null) begin
         m_master_agent.set_abstract_name($sformatf("%0s(AXI)", get_root_abstract_name()));
      end
      if (m_slave_agent != null) begin
         m_slave_agent.set_abstract_name($sformatf("%0s(AXI)", get_root_abstract_name()));
      end
      if (m_monitor != null) begin
         m_monitor.set_abstract_name($sformatf("%0s(AXI-MON)", get_root_abstract_name()));
      end
   endfunction : build_phase


   //-----------------------------------------------------------------------------------------------
   //
   // connect_phase
   //
   virtual function void connect_phase(uvm_phase phase);
      super.connect_phase(phase);
      if (m_master_agent != null) begin
         m_sa_snd_req_export.connect(m_master_agent.m_sa_snd_req_export);
      end
      if (m_slave_agent != null) begin
         m_sa_snd_rsp_export.connect(m_slave_agent.m_sa_snd_rsp_export);
      end
      if (m_sa_snd_req_fifo != null) begin
         m_sa_snd_req_export.connect(m_sa_snd_req_fifo.analysis_export);
      end
      if (m_obs_req_fifo != null) begin
         m_monitor.m_obs_req_port.connect(m_obs_req_fifo.analysis_export);
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
         evl_verif_comp master_port_agent;

         master_port_agent = get_master_port_agent();
         m_monitor.set_int_param(pk_allow_duplicate_outstanding_ids, m_rtl_port.get_int_param(pk_allow_duplicate_outstanding_ids, 0));
         m_monitor.set_master_port_agent(master_port_agent);
         m_monitor.set_slave_port_agent(get_slave_port_agent());
         if (master_port_agent != null) begin
            master_port_agent.set_int_param(pk_read_vc_credits, m_rtl_port.get_int_param(pk_read_vc_credits, 0));
            master_port_agent.set_int_param(pk_write_vc_credits, m_rtl_port.get_int_param(pk_write_vc_credits, 0));
         end
      end
      fork
         super.run_phase(phase);
         sort_sent_and_obs_req_descs();
         process_observed_requests();
         process_observed_responses();
         begin
            uvm_void axi_cov;

            m_rtl_port.wait_for_coverage_enabled();
            #0 axi_cov = m_rtl_port.get_object_param_by_name("pk_axi_cov_object");
            if (axi_cov != null) begin
               $cast(m_axi_cov, axi_cov);
            end
         end
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
            if (`evl_get_error_intensity(pk_sc_axi_error_intensity) >= 0) begin
               //
               // If we are disabling errors, the error intensity will be non-negative.  In this
               // case, set the timeout value to something relatively low and deal with the
               // consequences.
               //
               if (m_global_attrs.get_timeout_value(ShutdownPhase) < 1000) begin
                  wait_for_active_mem_desc(m_global_attrs.get_timeout_value(ShutdownPhase), 1'b1, 1'b0, 1'b1);
               end
               else begin
                  wait_for_active_mem_desc(1000, 1'b1, 1'b0, 1'b1);
               end
            end
            else begin
               wait_for_active_mem_desc(m_global_attrs.get_timeout_value(ShutdownPhase), 1'b1, 1'b0, 1'b1);
            end
            phase.drop_objection(this);
         end
      join
   endtask : shutdown_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string name_in = "evl_verif_comp", input uvm_component parent_in = null);
      super.new(name_in, parent_in, AXI_BUS_AGENT);
      if (`et_test_plusargs("disableAXIChecks") != 0) begin
         `evl_set_error_intensity(pk_sc_axi_error_intensity, UVM_HIGH)
      end
      if (`et_test_plusargs("enableAXIChecks") != 0) begin
         `evl_set_error_intensity(pk_sc_axi_error_intensity, -1)
      end

      if (`et_value_plusargs("issue_dut_error_on_error_rsp=%d", m_issue_dut_error_on_error_rsp) == 0) begin
         m_issue_dut_error_on_error_rsp = `EVL_ISSUE_DUT_ERROR_ON_AXI_ERR_RSP_DEFAULT;
      end

      m_active_reads       = 0;
      m_active_writes      = 0;
      m_req_dword_start    = 0;
      m_last_obs_write_req = null;
      m_err_pk             = pk_sc_axi_error_intensity;
      m_vmod               = VMOD_AXI_SB;
   endfunction : new

endclass : evl_axi_verif_comp

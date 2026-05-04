//--------------------------------------------------------------------------------------------------
//
// Class: evl_axi_monitor
//
class evl_axi_monitor extends evl_monitor;

   `uvm_component_utils(evl_axi_monitor)

   `evl_param_key(pk_allow_duplicate_outstanding_ids)
   `evl_param_key(pk_data_bytes)
   `evl_param_key(pk_sc_axi_error_intensity)

   //-----------------------------------------------------------------------------------------------
   //
   // This following variable is a handle to the RTL port (which is instantiated in the
   // evl_axi_interface module).
   //
   evl_axi_rtl_port_base m_axi_rtl_port;

   bit                     m_allow_duplicate_sent_ids = 1'b0;

   //-----------------------------------------------------------------------------------------------
   //
   // The following variables are intended to uniquely identify this interface.
   //
   int                   m_port_type  = 0;
   int                   m_subport_id = 0;

   //-----------------------------------------------------------------------------------------------
   //
   // The following variables are handles back to the bus verification component and the master and
   // slave components for this interface.
   //
   evl_verif_comp        m_verif_comp        = null;
   evl_verif_comp        m_master_port_agent = null;
   evl_verif_comp        m_slave_port_agent  = null;

   //-----------------------------------------------------------------------------------------------
   //
   // The following variable is a handle to the AXI coverage object.  Certain coverage items require
   // history for sampling; the coverage object is created within the evl_axi_interface module (most
   // coverage can be obtained directly from bus signals), and a handle to that object is stored in
   // the RTL port so the monitor can also have access to it.
   //
   evl_axi_cov_base      m_axi_cov;

   //-----------------------------------------------------------------------------------------------
   //
   // This following variable is a request descriptor queue to hold pending requests that can then
   // be matched with incoming responses.
   //
   evl_req_desc_queue    m_request_queue;

   //-----------------------------------------------------------------------------------------------
   //
   // This following variable is a request descriptor queue to hold write requests that are waiting
   // for data (the write request and write data pipes operate at independent rates).
   //
   evl_req_desc_queue    m_write_waiting_for_data_queue;

   //-----------------------------------------------------------------------------------------------
   //
   // The following queue is used to hold write data that is still waiting for a write request (it
   // is possible for write data to be sent before the request is observed).
   //
   evl_axi_bus_req       m_data_waiting_for_write_queue[$];

   //-----------------------------------------------------------------------------------------------
   //
   // The following variables are templates from which to create new requests.  Each request
   // observed on the bus spawns a new request descriptor ... the following templates are used to
   // create the spawned descriptor.
   //
   evl_line_data         m_template_req_data;
   evl_line_desc         m_template_line_desc;
   evl_trans_desc        m_template_trans_desc;
   evl_req_desc          m_template_req_desc;

   //-----------------------------------------------------------------------------------------------
   //
   // The following variables define the data width of the bus for this inteface.
   int                   m_data_bytes;
   int                   m_data_bytes_log2;

   //-----------------------------------------------------------------------------------------------
   //
   // The following analysis ports are used to send observed requests to a downstream object and
   // observed responses to an upstream object.
   //
   uvm_analysis_port #(evl_req_desc) m_obs_req_port;
   uvm_analysis_port #(evl_req_desc) m_exp_rsp_port;


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_master_port_agent
   //
   virtual function void set_master_port_agent(input evl_verif_comp agent_in, input string port_name = "", input int port_id = 0);
      m_master_port_agent = agent_in;
   endfunction : set_master_port_agent


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_master_port_agent
   //
   virtual function evl_verif_comp get_master_port_agent(input string port_name = "", input int port_id = 0, input int subport_id = 0);
      return m_master_port_agent;
   endfunction : get_master_port_agent


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_slave_port_agent
   //
   virtual function void set_slave_port_agent(input evl_verif_comp agent_in, input string port_name = "", input int port_id = 0);
      m_slave_port_agent = agent_in;
   endfunction : set_slave_port_agent


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_slave_port_agent
   //
   virtual function evl_verif_comp get_slave_port_agent(input string port_name = "", input int port_id = 0, input int subport_id = 0);
      return m_slave_port_agent;
   endfunction : get_slave_port_agent


   //-----------------------------------------------------------------------------------------------
   //
   // Task: process_requests
   //
   // Get request objects from the interface, transfer the information into a request descriptor,
   // then send it on the m_obs_req_port.
   //
   task process_requests();
      evl_req_desc        req_desc;
      evl_req_desc        exp_req_desc;
      evl_line_data       line_data;
      evl_xword_t         xword_data;
      evl_xword_byte_en_t xword_byte_en;
      evl_axi_bus_req     bus_req;

      if (m_axi_rtl_port == null) begin
         return;
      end
      forever begin
         //
         // Get the next observed bus cycle.
         //
         m_axi_rtl_port.get_obs_req(bus_req);

         if (bus_req.get_data_only() == 1'b0) begin // --------------------------------------------{
            //
            // The interface is sending us an AW or AR, not a W.  Create an observed request
            // descriptor and fill in the fields.
            //
            $cast(req_desc, m_template_req_desc.clone());
            req_desc.mark_as_bus_child();
            req_desc.set_bus_cmd(evl_bus_cmd_t'(bus_req.get_req_type()));
            req_desc.set_bus_cache_attr(bus_req.get_ax_cache());
            req_desc.set_paddr(bus_req.get_ax_addr());
            req_desc.set_bus_req_size(bus_req.get_ax_size());
            req_desc.set_bus_req_len(bus_req.get_ax_len());
            req_desc.set_bus_req_id(bus_req.get_ax_id());
            req_desc.set_bus_req_user(bus_req.get_ax_user());
            req_desc.set_bus_prot(bus_req.get_ax_prot());
            req_desc.set_bus_qos(bus_req.get_ax_qos());
            if (bus_req.get_ax_vc() != ~evl_bus_req_vc_t'(0)) begin
               req_desc.set_vc(bus_req.get_ax_vc());
            end
            if (!(bus_req.get_trans_id() inside { evl_trans_id_t'(0), ~evl_trans_id_t'(0) })) begin
               req_desc.set_trans_id(bus_req.get_trans_id());
            end

            //
            // The following code fragment attempts to link the observed request to a predicted
            // request.  If the handle to upstream verification component (m_master_port_agent) is
            // null, no connection can be made.
            //
            if (m_master_port_agent != null) begin
               exp_req_desc = m_master_port_agent.find_observed_request(AXI_BUS_AGENT, m_port_type, m_subport_id, req_desc);
               if (m_master_port_agent.is_req_checked(req_desc)) begin
                  req_desc.set_parent_check();
               end
               if (exp_req_desc != null) begin
                  exp_req_desc.set_bus_req_desc(req_desc);
                  req_desc.reset_create_time();
               end
               else if ((req_desc.get_parent_req_desc() == null) && (!(req_desc.get_trans_id() inside { evl_trans_id_t'(0), ~evl_trans_id_t'(0) }))) begin
                  req_desc.set_trans_id();
               end
            end

            //
            // If no match for the observed request descriptor was made and it has no parent, we
            // need to create a fake transaction descriptor for it and set the transaction
            // descriptor as the parent.  This way, the downstream object (which will get a handle
            // to the observed request descriptor) can assume there is a valid transaction
            // descriptor associated with the observed request descriptor.
            //
            if ((exp_req_desc == null) && (req_desc.get_parent_req_desc() == null)) begin
               if (req_desc.get_root_trans_desc() == null) begin
                  evl_trans_desc trans_desc;

                  $cast(trans_desc, m_template_trans_desc.clone());
                  if (req_desc.bus_cmd_is_read() == 1'b1) begin
                     trans_desc.set_cmd(LD);
                  end
                  else begin
                     trans_desc.set_cmd(ST);
                  end
                  trans_desc.set_paddr(req_desc.get_paddr());
                  if (req_desc.get_trans_id() inside { evl_trans_id_t'(0), ~evl_trans_id_t'(0) }) begin
                     trans_desc.set_trans_id();
                  end
                  else begin
                     trans_desc.set_trans_id(req_desc.get_trans_id());
                  end
                  trans_desc.set_created_by_bus();
                  trans_desc.add_child_desc(req_desc);
               end
            end

            //
            // One final check to make sure there is a unique transaction ID for the observed
            // request descriptor.
            //
            if (req_desc.get_trans_id() inside { evl_trans_id_t'(0), ~evl_trans_id_t'(0) }) begin
               req_desc.set_trans_id();
            end

            `evl_log_mdesc(UVM_HIGH, VMOD_AXI_MON, get_abstract_name(), req_desc, $sformatf("saw a request with type %p, id 0x%x, addr 0x%x", bus_req.get_req_type(), bus_req.get_ax_id(), bus_req.get_ax_addr()))

            //
            // If this is a write, see if the data arrived before the request.  Otherwise, stash
            // away this request descriptor until its data shows up.
            //
            if (req_desc.bus_cmd_is_write() == 1'b1) begin
               bit         w_last = 1'b0;
               evl_paddr_t addr;

               //
               // Sample how many Ws are waiting for an AW.
               //
               if (m_axi_cov != null) begin
                  m_axi_cov.sample_aw_vs_w(0, m_data_waiting_for_write_queue.size());
               end

               while ((m_data_waiting_for_write_queue.size() != 0) && (w_last == 1'b0)) begin
                  evl_axi_bus_req data_bus_req;

                  //
                  // Data arrives in the same order as requests, so just take the first available.
                  // Note that there may be a number of data bursts to capture.
                  //
                  data_bus_req = m_data_waiting_for_write_queue.pop_front();
                  if (req_desc.get_cur_burst() == 0) begin
                     addr = req_desc.get_paddr();
                  end
                  else begin
                     addr = (req_desc.get_paddr() + (evl_paddr_t'(m_data_bytes) * req_desc.get_cur_burst())) & ~(evl_paddr_t'(m_data_bytes) - 1);
                  end
                  copy_bus_req_data_to_desc(data_bus_req, req_desc, addr);
                  w_last = req_desc.pop_cur_burst();
                  if (w_last != data_bus_req.get_w_last()) begin
                     `dut_error(get_abstract_name(), $sformatf("expected WLAST to be %b, not %b", w_last, data_bus_req.get_w_last()), pk_sc_axi_error_intensity, { VMOD_AXI_SB }, m_rtl_port.get_rtl_name())
                  end
               end
               if (w_last == 1'b0) begin
                  m_write_waiting_for_data_queue.push(req_desc);
               end
            end

            //
            // Put the observed request descriptor onto the request queue so we can match its
            // response later.
            //
            m_request_queue.push(req_desc);

            //
            // Add the observed request descriptor to the list that is checked for timeouts.
            //
            m_verif_comp.add_active_mem_desc(req_desc);

            //
            // Send the observed request descriptor off to any downstream component.
            //
            m_obs_req_port.write(req_desc);
         end
         else begin // }---------------------------------------------------------------------------{
            //
            // The interface is sending us a write data cycle.  Either this write data matches up
            // with an AW that was previously observed, or this write data has arrived first.
            //

            //
            // Sample how many AWs are waiting for a W.
            //
            if (m_axi_cov != null) begin
               m_axi_cov.sample_aw_vs_w(1, m_write_waiting_for_data_queue.size());
            end

            //
            // Check for an outstanding AW request and, if found, match this data to it.
            //
            if ((m_write_waiting_for_data_queue.size() == 0) || (m_data_waiting_for_write_queue.size() > 0)) begin
               //
               // There are currently no pending write requests waiting for data (or we are already
               // backed up).
               //
               `evl_log(UVM_HIGH, VMOD_AXI_MON, get_abstract_name(), $sformatf("observed a write data cycle (0x%x) ... waiting for an AW request", bus_req.get_w_data()))
               m_data_waiting_for_write_queue.push_back(bus_req);
            end
            else begin
               bit         w_last;
               evl_paddr_t addr;

               //
               // If this is the last chunk of write data, pop the request descriptor from the queue
               // of writes waiting for data.  If this is not the last chunk of write data, leave
               // the request descriptor on the queue of writes waiting for data.
               //
               if (bus_req.get_w_last() == 1'b1) begin
                  req_desc = m_write_waiting_for_data_queue.pop();
               end
               else begin
                  req_desc = m_write_waiting_for_data_queue.peek();
               end
               `evl_log_mdesc(UVM_HIGH, VMOD_AXI_MON, get_abstract_name(), req_desc, $sformatf("found the request matching the write data just observed on the bus (0x%x)", bus_req.get_w_data()))

               if (req_desc.get_cur_burst() == 0) begin
                  addr = req_desc.get_paddr();
               end
               else begin
                  addr = (req_desc.get_paddr() + (evl_paddr_t'(m_data_bytes) * req_desc.get_cur_burst())) & ~(evl_paddr_t'(m_data_bytes) - 1);
               end
               copy_bus_req_data_to_desc(bus_req, req_desc, addr);
               w_last = req_desc.pop_cur_burst();
               if (w_last != bus_req.get_w_last()) begin
                  `dut_error(get_abstract_name(), $sformatf("expected WLAST to be %b, not %b", w_last, bus_req.get_w_last()), pk_sc_axi_error_intensity, { VMOD_AXI_SB }, m_rtl_port.get_rtl_name())
               end
            end
         end // -----------------------------------------------------------------------------------}
      end
   endtask : process_requests


   //-----------------------------------------------------------------------------------------------
   //
   // Task: process_responses
   //
   // Get response objects from the interface, transfer the information into a request descriptor,
   // then send it on the m_exp_rsp_port.
   //
   task process_responses();
      evl_req_desc        exp_req_desc;
      evl_req_desc        obs_req_desc;
      evl_line_data       line_data;
      evl_axi_bus_rsp     bus_rsp;
      bit                 found_request;
      evl_req_desc_queue  filter_queue;
      evl_paddr_t         req_addr;
      bit                 r_last;

      if (m_axi_rtl_port == null) begin
         return;
      end
      forever begin
         m_axi_rtl_port.get_obs_rsp(bus_rsp);

         `evl_log(UVM_HIGH, VMOD_AXI_MON, get_abstract_name(), $sformatf("received a response type %0s, id 0x%x", ((bus_rsp.rsp_is_for_write() == 1'b1) ? "AXI_BRESP" : "AXI_RRESP"), bus_rsp.get_x_id()))

         //
         // Find this ID in the request queue.
         //
         if (bus_rsp.rsp_is_for_write() == 1'b1) begin
            filter_queue = m_request_queue.filter_write_bus_req_id(bus_rsp.get_x_id());
         end
         else begin
            filter_queue = m_request_queue.filter_read_bus_req_id(bus_rsp.get_x_id());
         end

         //
         // Unless this monitor is configured otherwise, there should be only 1 request outstanding
         // with the specified ID.
         //
         found_request = 1'b0;
         if (filter_queue == null) begin
            `dut_error(get_abstract_name(), $sformatf("received a response with id 0x%x, but no requests in m_request_queue with that id", bus_rsp.get_x_id), pk_sc_axi_error_intensity, { VMOD_AXI_SB }, m_rtl_port.get_rtl_name())
         end
         else begin
            found_request = 1'b1;
            if ((get_int_param(pk_allow_duplicate_outstanding_ids, 0) == 0) && (m_allow_duplicate_sent_ids == 1'b0)) begin
               if (filter_queue.size() != 1) begin
                  `dut_error(get_abstract_name(), $sformatf("received a response with id 0x%x, but found more than one request with that id:", bus_rsp.get_x_id()), pk_sc_axi_error_intensity, { VMOD_AXI_SB }, m_rtl_port.get_rtl_name())
                  filter_queue.print_obj();
               end
            end
            exp_req_desc = filter_queue.pop();
            if (bus_rsp.get_x_last() == 1'b1) begin
               void'(m_request_queue.pop(exp_req_desc));
            end
            `evl_log_mdesc(UVM_HIGH, VMOD_AXI_MON, get_abstract_name(), exp_req_desc, $sformatf("found the request matching for the response just observed on the bus"))
         end

         // if we didn't find the request, then create a req_desc ... be sure to create a transaction descriptor, too.
         if (found_request == 1'b0) begin
            evl_trans_desc trans_desc;

            $cast(exp_req_desc, m_template_req_desc.clone());
            $cast(trans_desc, m_template_trans_desc.clone());
            if (bus_rsp.rsp_is_for_write() == 1'b0) begin
               trans_desc.set_cmd(LD);
            end
            else begin
               trans_desc.set_cmd(ST);
            end
            trans_desc.set_created_by_bus();
            trans_desc.add_child_desc(exp_req_desc);
         end

         //
         // The expected request descriptor should be the previously observed request descriptor.
         // The request descriptor that spawned this observed request descriptor should be its
         // parent; if it isn't, make them the same.
         //
         // So, obs_req_desc is supposed to be the descriptor that sent the original request (from
         // the upstream object) and exp_req_desc is the spawned descriptor (from the downstream
         // object).  If the downstream object has a scoreboard, the data in it would have been
         // filled in by the scoreboard.  If it is from a stimulus agent, it would have been filled
         // in by the stimulus agent.
         //
         obs_req_desc = exp_req_desc.get_parent_req_desc();
         if (obs_req_desc == null) begin
            obs_req_desc = exp_req_desc;
         end

         if (bus_rsp.rsp_is_for_write() == 1'b1) begin
            obs_req_desc.set_bus_rsp_type(evl_bus_rsp_type_t'(bus_rsp.get_x_resp()));
            obs_req_desc.set_bus_rsp_user(evl_bus_rsp_user_t'(bus_rsp.get_x_user()));
            obs_req_desc.set_x_last();
            obs_req_desc.set_bus_rsp_valid(0, m_axi_rtl_port.get_cycle_count());
         end
         else begin
            if (obs_req_desc.get_cur_burst() == 0) begin
               obs_req_desc.set_bus_rsp_type(evl_bus_rsp_type_t'(bus_rsp.get_x_resp()));
               obs_req_desc.set_bus_rsp_user(evl_bus_rsp_user_t'(bus_rsp.get_x_user()));
               req_addr = obs_req_desc.get_paddr();
            end
            else begin
               req_addr = (obs_req_desc.get_paddr() + (evl_paddr_t'(m_data_bytes) * obs_req_desc.get_cur_burst())) & ~(evl_paddr_t'(m_data_bytes) - 1);
            end
            obs_req_desc.set_obs_rsp_data(req_addr, m_data_bytes, bus_rsp.get_x_data(), ((1 << m_data_bytes) - 1));

            //
            // Set the observed value of RLAST.  Note that the current burst count is incremented in
            // evl_axi_verif_comp.sv, so there is no need to increment it here.
            //
            if (bus_rsp.get_x_last() == 1'b1) begin
               obs_req_desc.set_x_last();
               obs_req_desc.set_bus_rsp_valid(0, m_axi_rtl_port.get_cycle_count());
            end
         end

         m_exp_rsp_port.write(exp_req_desc);
      end
   endtask : process_responses


   //-----------------------------------------------------------------------------------------------
   function void copy_bus_req_data_to_desc(input evl_axi_bus_req bus_req, input evl_req_desc req_desc, input evl_paddr_t req_addr);
      req_desc.set_obs_req_data(req_addr, m_data_bytes, bus_req.get_w_data(), bus_req.get_w_strb());
   endfunction : copy_bus_req_data_to_desc


   //-----------------------------------------------------------------------------------------------
   //
   // Task: watch_resets
   //
   task watch_resets();
      if (m_rtl_port == null) begin
         return;
      end
      forever begin
         m_rtl_port.wait_for_reset_assertion();
         m_rtl_port.wait_cycles(1);
         m_request_queue.delete_all_descs();
         m_write_waiting_for_data_queue.delete_all_descs();
         if (m_verif_comp != null) begin
            m_verif_comp.delete_all_active_mem_desc();
         end
         m_rtl_port.wait_for_reset_deassertion();
         m_request_queue.delete_all_descs();
         m_write_waiting_for_data_queue.delete_all_descs();
         if (m_verif_comp != null) begin
            m_verif_comp.delete_all_active_mem_desc();
         end
      end
   endtask : watch_resets


   //-----------------------------------------------------------------------------------------------
   virtual function void build_phase(uvm_phase phase);
      super.build_phase(phase);
      $cast(m_axi_rtl_port, m_rtl_port);
   endfunction : build_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Function: start_of_simulation_phase
   //
   virtual function void start_of_simulation_phase(uvm_phase phase);
      super.start_of_simulation_phase(phase);

      m_template_req_data   = evl_line_data::type_id::create($sformatf("%s(AXI)", get_root_abstract_name()));
      m_template_line_desc  = evl_line_desc::type_id::create($sformatf("%s(AXI)", get_root_abstract_name()));
      m_template_trans_desc = evl_trans_desc::type_id::create($sformatf("%s(AXI)", get_root_abstract_name()));
      m_template_req_desc   = evl_req_desc::type_id::create($sformatf("%s(AXI)", get_root_abstract_name()));

      m_template_trans_desc.set_shire_id(m_verif_comp.get_shire_id());
      m_template_trans_desc.set_neighborhood_id(m_verif_comp.get_neighborhood_id());
      m_template_trans_desc.set_core_id(m_verif_comp.get_core_id());

      m_template_req_desc.set_shire_id(m_verif_comp.get_shire_id());
      m_template_req_desc.set_neighborhood_id(m_verif_comp.get_neighborhood_id());
      m_template_req_desc.set_core_id(m_verif_comp.get_core_id());

      m_template_line_desc.set_data_template(m_template_req_data);
      m_template_trans_desc.set_data_template(m_template_req_data);
      m_template_req_desc.set_data_template(m_template_req_data);
      m_template_req_desc.set_bus_type(EVL_BUS_TYPE_AXI);

      m_template_trans_desc.set_line_template(m_template_line_desc);
      m_template_req_desc.set_line_template(m_template_line_desc);

      fork
         watch_resets();
      join_none
   endfunction : start_of_simulation_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Task: run_phase
   //
   virtual task run_phase(uvm_phase phase);
      super.run_phase(phase);
      m_request_queue.set_abstract_name($sformatf("%s(REQQ)", get_root_abstract_name(get_abstract_name())));
      m_write_waiting_for_data_queue.set_abstract_name($sformatf("%s(WDQ)", get_root_abstract_name(get_abstract_name())));
      fork
         process_requests();
         process_responses();
         begin
            uvm_void axi_cov;

            m_rtl_port.wait_for_coverage_enabled();
            #0 axi_cov = m_rtl_port.get_object_param_by_name("pk_axi_cov_object");
            if (axi_cov != null) begin
               $cast(m_axi_cov, axi_cov);
            end
         end
      join_none;

      m_data_bytes      = m_rtl_port.get_int_param(pk_data_bytes);
      m_data_bytes_log2 = $clog2(m_data_bytes);
   endtask : run_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string name_in = "evl_axi_monitor", input uvm_component parent_in = null);
      super.new(name_in, parent_in);
      m_obs_req_port  = new("m_obs_req_port", this);
      m_exp_rsp_port  = new("m_exp_rsp_port", this);
      m_request_queue = new("axi_mon_request_queue");
      m_write_waiting_for_data_queue = new("write_waiting_for_data");
      if (`et_test_plusargs("axi_allow_duplicate_sent_ids") != 0) begin
         m_allow_duplicate_sent_ids = 1'b1;
      end
   endfunction : new

endclass: evl_axi_monitor

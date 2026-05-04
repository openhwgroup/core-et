//--------------------------------------------------------------------------------------------------
//
// Class: evl_tlink_verif_comp
//
// The TileLink Verification component contains a master driver, a slave driver, and a monitor.  The
// drivers are not always present.  For example, if this verification component is being used as an
// active master, then it will contain a master driver and a monitor, and it will receive requests
// to drive on the TileLink bus from a master stim agent.  Likewise if it is being used as an active
// slave, then it will contain a slave driver and a monitor, and it will send observed requests to a
// slave stim agent (which contains a memory model), and the slave stim agent will send this
// verification component a response to drive on the bus.
//
// This verification component may also be passive, which means it won't drive anything onto the
// bus.  In that case it will just contain a monitor.
//
class evl_tlink_verif_comp extends evl_verif_comp;

   `uvm_component_utils(evl_tlink_verif_comp)

   //-----------------------------------------------------------------------------------------------
   //
   // Local Parameters
   //


   //-----------------------------------------------------------------------------------------------
   //
   // Static Member Variables and Objects
   //
   `evl_param_key(pk_bus_rsp_count)
   `evl_param_key(pk_no_error_response)
   `evl_param_key(pk_tlink_error_intensity)


   //-----------------------------------------------------------------------------------------------
   //
   // Member Variables and Objects
   //
   bit                    m_saw_request;
   bit                    m_saw_response;
   string                 m_req_channel_names[int];
   evl_tlink_rtl_port     m_tlink_rtl_port;
   evl_verif_comp         m_master_port_agents[$];
   evl_tlink_master_agent m_tlink_master_agent;
   evl_tlink_slave_agent  m_tlink_slave_agent;
   evl_tlink_monitor      m_tlink_monitor;

   //
   // Members for supporting stimulus agents:
   //
   // m_sa_snd_req_export:        This is the TLM port used to connect this bus agent to a stimulus
   //                             agent.
   // m_sa_snd_req_fifo:          This is the TLM FIFO used to receive request descriptors from the
   //                             stimulus agent.
   // m_sa_snd_req_desc_queues[]: This array of queues contains in-order lists of request
   //                             descriptors per independent port (up to 16 independent ports are
   //                             currently supported).
   //
   uvm_analysis_export   #(evl_req_desc) m_sa_snd_req_export;
   uvm_tlm_analysis_fifo #(evl_req_desc) m_sa_snd_req_fifo;
   evl_req_desc_queue                    m_sa_req_desc_queues[4];

   // Ports for sending requests and responses back to the stimulus agent.
   uvm_analysis_port #(evl_req_desc) m_sa_rcv_req_port;
   uvm_analysis_port #(evl_req_desc) m_sa_rcv_rsp_port;

   // Ports for sending observed requests and responses from the monitor to the rest of the world
   uvm_analysis_port #(evl_req_desc) m_obs_areq_port;
   uvm_analysis_port #(evl_req_desc) m_obs_breq_port;
   uvm_analysis_port #(evl_req_desc) m_obs_creq_port;
   uvm_analysis_port #(evl_req_desc) m_obs_crsp_port;
   uvm_analysis_port #(evl_req_desc) m_obs_drsp_port;
   uvm_analysis_port #(evl_req_desc) m_obs_ersp_port;

   // FIFOs for receiving observed requests and responses from the monitor.
   uvm_tlm_analysis_fifo #(evl_req_desc) m_mon_req_fifo;
   uvm_tlm_analysis_fifo #(evl_req_desc) m_mon_rsp_fifo;


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_req_desc_port
   //
   virtual function uvm_analysis_port#(evl_req_desc) get_req_desc_port(input string port_name      = "unknown",
                                                                       input int    port_number    = 0,
                                                                       input int    subport_number = 0);
      if (port_name == "obs_areq") begin
         return m_obs_areq_port;
      end
      else if (port_name == "obs_breq") begin
         return m_obs_breq_port;
      end
      else if (port_name == "obs_creq") begin
         return m_obs_creq_port;
      end
      else if (port_name == "obs_crsp") begin
         return m_obs_crsp_port;
      end
      else if (port_name == "obs_drsp") begin
         return m_obs_drsp_port;
      end
      else if (port_name == "obs_ersp") begin
         return m_obs_ersp_port;
      end
      else if (port_name == "sa_rcv_rsp") begin
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
      if (port_name == "sa_snd_req") begin
         return m_sa_snd_req_export;
      end
      return null;
   endfunction : get_req_desc_export


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_abstract_name
   //
   virtual function void set_abstract_name(input string name_in);
      super.set_abstract_name(name_in);
      if (m_tlink_master_agent != null) begin
         m_tlink_master_agent.set_abstract_name($sformatf("%0s(TLINK)", get_root_abstract_name()));
      end
      if (m_tlink_slave_agent != null) begin
         m_tlink_slave_agent.set_abstract_name($sformatf("%0s(TLINK)", get_root_abstract_name()));
      end
      if (m_tlink_monitor != null) begin
         m_tlink_monitor.set_abstract_name($sformatf("%0s(TLINK-MON)", get_root_abstract_name()));
      end
   endfunction : set_abstract_name


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_int_param
   //
   virtual function void set_int_param(input int param_key, input int param_value);
      if (m_tlink_monitor != null) begin
         m_tlink_monitor.set_int_param(param_key, param_value);
      end
      super.set_int_param(param_key, param_value);
   endfunction : set_int_param


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_master_port_agent
   //
   virtual function void set_master_port_agent(input evl_verif_comp agent_in, input string port_name = "", input int port_id = 0);
      if ((port_id >= 0) && (port_id < m_master_port_agents.size())) begin
         m_master_port_agents[port_id] = agent_in;
         if ((m_master_port_agents[port_id] != this) && (m_master_port_agents[port_id] != null)) begin
            `evl_log(UVM_MEDIUM, VMOD_CONFIG, get_abstract_name(), $sformatf("setting %0s as a master agent for %0s channel %0s (%0d)", m_master_port_agents[port_id].get_abstract_name(), get_abstract_name(), m_req_channel_names[port_id], port_id))
         end
      end
   endfunction : set_master_port_agent


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_master_port_agent
   //
   virtual function evl_verif_comp get_master_port_agent(input string port_name = "", input int port_id = 0, input int subport_id = 0);
      if ((port_id < 0) || (port_id >= m_master_port_agents.size())) begin
         return null;
      end
      return m_master_port_agents[port_id];
   endfunction : get_master_port_agent


   //-----------------------------------------------------------------------------------------------
   //
   // Function: this_is_master_port_agent
   //
   virtual function bit this_is_master_port_agent(input int port_id = 0);
      if ((port_id < 0) || (port_id >= m_master_port_agents.size())) begin
         return 1'b0;
      end
      return ((m_master_port_agents[port_id] == this) || (m_master_port_agents[port_id] == null)) ? 1'b1 : 1'b0;
   endfunction : this_is_master_port_agent


   //-----------------------------------------------------------------------------------------------
   //
   // Function: find_observed_request
   //
   virtual function evl_req_desc find_observed_request(input evl_agent_type_t     port_type_in,
                                                       input int                  port_in,
                                                       input int                  subport_in,
                                                       input evl_req_desc         paired_req_desc,
                                                       input evl_bus_cmd_t        bus_cmd_in   = `EVL_Bus_Idle,
                                                       input evl_paddr_t          address_in   = ~evl_paddr_t'(0),
                                                       input evl_bus_req_id_t     req_id_in    = ~evl_bus_req_id_t'(0),
                                                       input evl_bus_req_source_t source_id_in = ~evl_bus_req_source_t'(0),
                                                       input evl_trans_id_t       trans_id_in  = ~evl_trans_id_t'(0));
      int          channel;
      evl_req_desc snd_req;

      if ((is_active_master() == 1'b0) || (paired_req_desc == null)) begin
         return null;
      end
      if (bus_cmd_in == `EVL_Bus_Idle) begin
         channel = (int'(paired_req_desc.get_bus_cmd()) & `EVL_TL_REQ_CHANNEL_MASK) >> `EVL_TL_REQ_CHANNEL_SHIFT;
      end
      else begin
         channel = (int'(bus_cmd_in) & `EVL_TL_REQ_CHANNEL_MASK) >> `EVL_TL_REQ_CHANNEL_SHIFT;
      end
      if (m_sa_req_desc_queues[channel].size() > 0) begin
         snd_req = m_sa_req_desc_queues[channel].peek();
         if ((snd_req.get_bus_cmd()    == paired_req_desc.get_bus_cmd()) &&
             (snd_req.get_paddr()      == paired_req_desc.get_paddr())   &&
             (snd_req.get_bus_req_id() == paired_req_desc.get_bus_req_id())) begin
            if (paired_req_desc.get_parent_desc() == null) begin
               snd_req.add_child_desc(paired_req_desc);
               paired_req_desc.reset_create_time();
            end
            return snd_req;
         end
      end
      return null;
   endfunction : find_observed_request


   //-----------------------------------------------------------------------------------------------
   //
   // Function: process_stim_agent_requests
   //
   task process_stim_agent_requests;
      evl_req_desc req_desc;

      if (m_sa_snd_req_fifo == null) begin
         return;
      end
      forever begin
         m_sa_snd_req_fifo.get(req_desc);
         if (req_desc != null) begin
            int channel;

            channel = (int'(req_desc.get_bus_cmd()) & `EVL_TL_REQ_CHANNEL_MASK) >> `EVL_TL_REQ_CHANNEL_SHIFT;
            m_sa_req_desc_queues[channel].push(req_desc);
         end
      end
   endtask : process_stim_agent_requests


   //-----------------------------------------------------------------------------------------------
   //
   // Function: process_observed_requests
   //
   // The purpose of this task is to connect up the request descriptor from the stimulus agent with
   // the corresponding observed request descriptor from the monitor.  They need to be linked so
   // that when the observed response gets back to the master stimulus agent, it can put the data in
   // the right place and set done and all that.  It turns out the observed request is going to be a
   // child of the sent request.
   //
   virtual task process_observed_requests();
      evl_req_desc obs_req;

      if (m_mon_req_fifo == null) begin
         return;
      end
      forever begin
         int          channel;
         evl_req_desc exp_req;

         m_mon_req_fifo.get(obs_req);
         m_saw_request = 1'b1;
         exp_req       = null;
         channel       = (int'(obs_req.get_bus_cmd()) & `EVL_TL_REQ_CHANNEL_MASK) >> `EVL_TL_REQ_CHANNEL_SHIFT;

         `evl_log_mdesc(UVM_HIGH, VMOD_TLINK_MON, get_abstract_name(), obs_req, $sformatf("saw this observed request: %s", obs_req.sprint_obj()))

         //
         // If this interface isn't driven by a stimulus agent, check against scoreboard
         // prediction.
         //
         if (is_active_master() == 1'b0) begin
            `evl_log_mdesc(UVM_MEDIUM, VMOD_GLOBAL, get_abstract_name(), obs_req, obs_req.sprint_obj(1))
            if (obs_req.do_parent_check() == 1'b1) begin
               bit mismatch = 1'b0;

               exp_req = obs_req.get_parent_req_desc();
               if (exp_req == null) begin
                  mismatch = 1'b1;
                  `dut_error_mdesc(get_abstract_name(), obs_req, $sformatf("%0s is an unexpected request", obs_req.sprint_obj()), pk_tlink_error_intensity, { VMOD_TLINK_SB })
               end
               else begin
                  evl_line_data exp_req_data;
                  evl_line_data obs_req_data;

                  if ( (exp_req.get_bus_cmd() != obs_req.get_bus_cmd()) ||
                       (exp_req.get_paddr()   != obs_req.get_paddr()) ) begin
                     mismatch = 1'b1;
                     `dut_error_mdesc(get_abstract_name(), obs_req, $sformatf("expected %0s, not %0s", exp_req.sprint_obj(), obs_req.sprint_obj()), pk_tlink_error_intensity, { VMOD_TLINK_SB })
                  end
                  if (exp_req.get_bus_req_id() != obs_req.get_bus_req_id()) begin
                     mismatch = 1'b1;
                     `dut_error_mdesc(get_abstract_name(), obs_req, $sformatf("expected TileLink request ID of 0x%x for %0s, not 0x%x", exp_req.get_bus_req_id(), obs_req.sprint_obj(), obs_req.get_bus_req_id()), pk_tlink_error_intensity, { VMOD_TLINK_SB })
                  end
                  if (exp_req.bus_cmd_is_write() == 1'b1) begin
                     exp_req_data = exp_req.get_req_data();
                     obs_req_data = obs_req.get_req_data();
                     if (exp_req_data == null) begin
                        `dut_error_mdesc(get_abstract_name(), exp_req, $sformatf("TB_FAIL: request data for expected request is null (%0s, line %0d)", `__FILE__, `__LINE__), pk_tlink_error_intensity, { VMOD_TLINK_SB })
                     end
                     else if (obs_req_data == null) begin
                        `dut_error_mdesc(get_abstract_name(), obs_req, $sformatf("request data for %0s is null", obs_req.sprint_obj()), pk_tlink_error_intensity, { VMOD_TLINK_SB })
                     end
                     else begin
                        string err_messages[$];

                        err_messages = {};
                        if (obs_req_data.compare_data_and_byte_ens(exp_req_data, err_messages) == 1'b1) begin
                           foreach (err_messages[ii]) begin
                              `dut_error_mdesc(get_abstract_name(), obs_req, err_messages[ii], pk_tlink_error_intensity, { VMOD_TLINK_SB })
                           end
                        end
                     end
                     if (mismatch == 1'b1) begin
                        exp_req = null;
                     end
                  end
               end
            end
            m_global_attrs.bus_trigger(obs_req.get_paddr(), obs_req.get_bus_req_id(), get_inst_id());
         end
         //
         // This is driven by a stimulus agent ... just link the descriptors.
         //
         else begin
            if (m_sa_req_desc_queues[channel].size() == 0) begin
               `evl_log_mdesc(UVM_MEDIUM, VMOD_GLOBAL, get_abstract_name(), obs_req, obs_req.sprint_obj(1))
               `dut_error_mdesc(get_abstract_name(), obs_req, $sformatf("TB_ISSUE: process_observed_requests not expecting m_sa_req_desc_queues[%0d] to be empty (%0s, line %0d)", channel, `__FILE__, `__LINE__))
               continue;
            end

            //
            // These are supposed to match ... do a couple of quick checks and make the observed
            // request a child of the sent request.
            //
            exp_req = m_sa_req_desc_queues[channel].pop();
            if ((exp_req.get_bus_cmd()    == obs_req.get_bus_cmd()) &&
                (exp_req.get_paddr()      == obs_req.get_paddr())   &&
                (exp_req.get_bus_req_id() == obs_req.get_bus_req_id())) begin
               void'(exp_req.add_child_desc(obs_req));
               exp_req.set_bus_req_desc(obs_req);
               obs_req.reset_create_time();
               obs_req.set_correlated();
               `evl_log_mdesc(UVM_MEDIUM, VMOD_GLOBAL, get_abstract_name(), obs_req, obs_req.sprint_obj(1))
            end
            else begin
               `evl_log_mdesc(UVM_MEDIUM, VMOD_GLOBAL, get_abstract_name(), obs_req, obs_req.sprint_obj(1))
               `dut_error(get_abstract_name(), $sformatf("process_observed_requests() expected sent request and observed request to match but they didn't: sent: %s ... observed: %s", exp_req.sprint_obj(), obs_req.sprint_obj()))
               exp_req = null;
            end
         end

         //
         // Reset the timeout value now that the request has been issued.
         //
         if (exp_req != null) begin
            evl_mem_desc root_desc = exp_req.get_root_desc();

            exp_req.set_issued();
            if (root_desc != null) begin
               root_desc.reset_timeout_value();
            end
         end
         if (obs_req != null) begin
            obs_req.set_issued();
            if (channel == (`EVL_TL_REQ_CHANNEL_A >> `EVL_TL_REQ_CHANNEL_SHIFT)) begin
               m_obs_areq_port.write(obs_req);
            end
            else if (channel == (`EVL_TL_REQ_CHANNEL_B >> `EVL_TL_REQ_CHANNEL_SHIFT)) begin
               m_obs_breq_port.write(obs_req);
            end
            else if (channel == (`EVL_TL_REQ_CHANNEL_C >> `EVL_TL_REQ_CHANNEL_SHIFT)) begin
               m_obs_creq_port.write(obs_req);
            end
         end
      end
   endtask : process_observed_requests


   //-----------------------------------------------------------------------------------------------
   //
   // Function: process_observed_responses
   //
   // The purpose of this task is to receive observed responses from the monitor, transfer
   // information (ex: rsp data) to the original request descriptor sent by the stimulus agent, and
   // send the request descriptor back to the stimulus agent.
   //
   task process_observed_responses();
      evl_req_desc obs_rsp;
      evl_req_desc rcv_req;

      forever begin
         m_mon_rsp_fifo.get(obs_rsp);
         m_saw_response = 1'b1;
         `evl_log_mdesc(UVM_HIGH, VMOD_TLINK_MON, get_abstract_name(), obs_rsp, $sformatf("observed response for %s", obs_rsp.sprint_obj()))
         if (obs_rsp.is_correlated() == 1'b1) begin
            rcv_req = obs_rsp.get_parent_req_desc();
            if (rcv_req != null) begin
               if (evl_tlink_rsp_opcode_t'(obs_rsp.get_bus_rsp_type()) inside { TLD_AccessAckData, TLD_GrantData, TLC_AccessAckData, TLC_ProbeAckData }) begin
                  rcv_req.replicate_rsp_info(obs_rsp);
               end
               else begin
                  rcv_req.replicate_rsp_info(obs_rsp, -1, 1'b0, 1'b1);
               end
               rcv_req.set_bus_rsp_valid(0, get_cycle_count());
            end
         end
         else begin
            rcv_req = obs_rsp;
         end

         if (obs_rsp.get_int_param(pk_bus_rsp_count) > 1) begin
            m_obs_ersp_port.write(rcv_req);
         end
         else begin
            int channel = int'(obs_rsp.get_bus_rsp_type()) & `EVL_TL_RSP_CHANNEL_MASK;

            if (channel == `EVL_TL_RSP_CHANNEL_D) begin
               m_obs_drsp_port.write(rcv_req);
            end
            else if (channel == `EVL_TL_RSP_CHANNEL_C) begin
               m_obs_crsp_port.write(rcv_req);
            end
         end

         if (m_sa_rcv_rsp_port != null) begin
            m_sa_rcv_rsp_port.write(rcv_req);
         end
      end
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

      $cast(m_tlink_rtl_port, m_rtl_port);
      if (m_rtl_port == null) begin
         `dut_error(get_abstract_name(), "tlink_verif_comp: m_rtl_port is null, SAD!")
      end

      m_tlink_monitor = evl_tlink_monitor::type_id::create("m_tlink_monitor", this);
      m_tlink_monitor.m_verif_comp     = this;
      m_tlink_monitor.m_rtl_port       = m_tlink_rtl_port;
      m_tlink_monitor.m_tlink_rtl_port = m_tlink_rtl_port;

      if (is_active_master() == 1'b1) begin
         m_tlink_master_agent = evl_tlink_master_agent::type_id::create("m_tlink_master_agent", this);
         m_tlink_master_agent.m_tlink_rtl_port = m_tlink_rtl_port;
      end
      else if (is_active_slave() == 1'b1) begin
         m_tlink_slave_agent = evl_tlink_slave_agent::type_id::create("m_tlink_slave_agent", this);
         m_tlink_slave_agent.m_tlink_rtl_port = m_tlink_rtl_port;
      end

      //
      // The following ports are used to send requests from this VC to external VCs.
      //
      m_obs_areq_port = new("m_obs_areq_port", this);
      m_obs_breq_port = new("m_obs_breq_port", this);
      m_obs_creq_port = new("m_obs_creq_port", this);
      m_obs_crsp_port = new("m_obs_crsp_port", this);
      m_obs_drsp_port = new("m_obs_drsp_port", this);
      m_obs_ersp_port = new("m_obs_ersp_port", this);

      //
      // The following FIFOs are used to send requests and responses from the monitor to this VC.
      //
      m_mon_req_fifo = new("m_mon_req_fifo", this);
      m_mon_rsp_fifo = new("m_mon_rsp_fifo", this);


      if (is_active_master() == 1'b1) begin
         m_sa_snd_req_export = new("m_sa_snd_req_export", this);
         m_sa_rcv_rsp_port   = new("m_sa_rcv_rsp_port", this);
         m_sa_snd_req_fifo   = new("m_sa_snd_req_fifo", this);
      end
      else begin
         if (is_active_slave() == 1'b1) begin
            m_sa_snd_req_export = new("m_sa_snd_req_export", this);
         end
      end

      if (m_tlink_master_agent != null) begin
         m_tlink_master_agent.set_abstract_name($sformatf("%0s(TLINK)", get_root_abstract_name()));
      end
      if (m_tlink_slave_agent != null) begin
         m_tlink_slave_agent.set_abstract_name($sformatf("%0s(TLINK)", get_root_abstract_name()));
      end
      if (m_tlink_monitor != null) begin
         m_tlink_monitor.set_abstract_name($sformatf("%0s(TLINK-MON)", get_root_abstract_name()));
      end
   endfunction : build_phase


   //-----------------------------------------------------------------------------------------------
   //
   // connect_phase
   //
   virtual function void connect_phase(uvm_phase phase);
      super.connect_phase(phase);
//    if (m_tlink_master_agent != null) begin
//       m_sa_snd_req_export.connect(m_tlink_master_agent.m_sa_snd_req_export);
//    end
//    if (m_tlink_slave_agent != null) begin
//       m_sa_snd_req_export.connect(m_tlink_slave_agent.m_sa_snd_req_export);
//    end
//    if (m_sa_snd_req_fifo != null) begin
//       m_sa_snd_req_export.connect(m_sa_snd_req_fifo.analysis_export);
//    end
      if (m_mon_req_fifo != null) begin
         m_tlink_monitor.m_obs_req_port.connect(m_mon_req_fifo.analysis_export);
      end
      if (m_mon_rsp_fifo != null) begin
         m_tlink_monitor.m_obs_rsp_port.connect(m_mon_rsp_fifo.analysis_export);
      end
      m_tlink_monitor.m_port_id = get_port_id();
   endfunction: connect_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Task: watch_resets
   //
   task watch_resets();
      forever begin
         wait_for_reset_assertion();
         wait_cycles(1);
         delete_all_active_mem_desc();
         foreach (m_sa_req_desc_queues[ii]) begin
            if (m_sa_req_desc_queues[ii] != null) begin
               m_sa_req_desc_queues[ii].delete_all_descs();
            end
         end
         wait_for_reset_deassertion();
         delete_all_active_mem_desc();
         foreach (m_sa_req_desc_queues[ii]) begin
            if (m_sa_req_desc_queues[ii] != null) begin
               m_sa_req_desc_queues[ii].delete_all_descs();
            end
         end
      end
   endtask : watch_resets


   //-----------------------------------------------------------------------------------------------
   //
   // Function: start_of_simulation_phase
   //
   virtual function void start_of_simulation_phase(uvm_phase phase);
      super.start_of_simulation_phase(phase);
      if (is_active_master() == 1'b1) begin
         set_master_port_agent(this, "", 0);
         set_master_port_agent(this, "", 2);
      end
      if (is_active_slave() == 1'b1) begin
         set_master_port_agent(this, "", 1);
      end
      fork
         watch_resets();
      join_none
   endfunction : start_of_simulation_phase


   //----------------------------------------------------------------------------------------------
   //
   // Task: run_phase
   //
   virtual task run_phase(uvm_phase phase);
      if (m_tlink_monitor != null) begin
         foreach (m_master_port_agents[ii]) begin
            m_tlink_monitor.set_master_port_agent(get_master_port_agent("", ii), "", ii);
         end
      end
      fork
         super.run_phase(phase);
         watch_for_timeouts();
         process_stim_agent_requests();
         process_observed_requests();
         process_observed_responses();
      join_none;
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
            if (`evl_get_error_intensity(pk_tlink_error_intensity) >= 0) begin
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
      super.new(name_in, parent_in, TLINK_BUS_AGENT);
      foreach (m_sa_req_desc_queues[ii]) begin
         m_sa_req_desc_queues[ii] = new();
      end
      if (`et_test_plusargs("disableTLinkChecks") != 0) begin
         `evl_set_error_intensity(pk_tlink_error_intensity, UVM_HIGH)
      end
      m_req_channel_names[`EVL_TL_REQ_CHANNEL_A] = "A";
      m_req_channel_names[`EVL_TL_REQ_CHANNEL_B] = "B";
      m_req_channel_names[`EVL_TL_REQ_CHANNEL_C] = "C";
      m_req_channel_names[(`EVL_TL_REQ_CHANNEL_A >> `EVL_TL_REQ_CHANNEL_SHIFT)] = "A";
      m_req_channel_names[(`EVL_TL_REQ_CHANNEL_B >> `EVL_TL_REQ_CHANNEL_SHIFT)] = "B";
      m_req_channel_names[(`EVL_TL_REQ_CHANNEL_C >> `EVL_TL_REQ_CHANNEL_SHIFT)] = "C";
      m_master_port_agents.push_back(null);
      m_master_port_agents.push_back(null);
      m_master_port_agents.push_back(null);
      m_vmod = VMOD_TLINK_SB;
   endfunction : new

endclass : evl_tlink_verif_comp

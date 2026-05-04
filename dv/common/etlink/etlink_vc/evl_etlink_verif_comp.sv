//--------------------------------------------------------------------------------------------------
//
// Class: evl_etlink_verif_comp
//
// The ETLINK Verification component contains a master driver, a slave driver, and a monitor.  The
// drivers are not always present.  For example, if this verification component is being used as an
// active master, then it will contain a master driver and a monitor, and it will receive requests
// to drive on the ETLINK bus from a master stim agent.  Likewise if it is being used as an active
// slave, then it will contain a slave driver and a monitor, and it will send observed requests to a
// slave stim agent (which contains a memory model), and the slave stim agent will send this
// verification component a response to drive on the bus.
//
// This verification component may also be passive, which means it won't drive anything onto the
// bus. In that case it will just contain a monitor.
//
class evl_etlink_verif_comp extends evl_verif_comp;

   `uvm_component_utils(evl_etlink_verif_comp)

   //-----------------------------------------------------------------------------------------------
   //
   // Local Parameters
   //
   localparam int UNCACHED_PORT = 15;


   //-----------------------------------------------------------------------------------------------
   //
   // Static Member Variables and Objects
   //
   `evl_param_key(pk_num_banks)
   `evl_param_key(pk_sc_etlink_error_intensity)
   `evl_param_key(pk_no_error_response)


   //-----------------------------------------------------------------------------------------------
   //
   // Member Variables and Objects
   //
   bit                      m_saw_request;
   bit                      m_saw_response;
   int                      m_bank_mask;
   evl_etlink_rtl_port      m_etlink_rtl_port;

   evl_etlink_master_agent  m_etlink_master_agent;
   evl_etlink_slave_agent   m_etlink_slave_agent;
   evl_etlink_monitor       m_etlink_monitor;

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
   evl_req_desc_queue                    m_sa_req_desc_queues[16];

   // ports for sending requests and responses back to the stimulus agent.
   uvm_analysis_port #(evl_req_desc) m_sa_rcv_req_port;
   uvm_analysis_port #(evl_req_desc) m_sa_rcv_rsp_port;

   // Ports for sending observed requests and responses from the monitor to the rest of the world
   uvm_analysis_port     #(evl_req_desc) m_obs_req_port;
   uvm_analysis_port     #(evl_req_desc) m_obs_rsp_port;
   uvm_tlm_analysis_fifo #(evl_req_desc) m_obs_req_fifo;

   // port and fifo for receiving observed responses from the monitor, so we can
   // put the observed response data/status into the original driven req_desc
   // from the stim agent and send it back.
   uvm_tlm_analysis_fifo #(evl_req_desc) m_obs_rsp_fifo;


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
      return null;
   endfunction : get_req_desc_export


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_abstract_name
   //
   virtual function void set_abstract_name(input string name_in);
      super.set_abstract_name(name_in);
      if (m_etlink_master_agent != null) begin
         m_etlink_master_agent.set_abstract_name($sformatf("%0s(ETLINK)", get_root_abstract_name()));
      end
      if (m_etlink_slave_agent != null) begin
         m_etlink_slave_agent.set_abstract_name($sformatf("%0s(ETLINK)", get_root_abstract_name()));
      end
      if (m_etlink_monitor != null) begin
         m_etlink_monitor.set_abstract_name($sformatf("%0s(ETLINK-MON)", get_root_abstract_name()));
      end
   endfunction : set_abstract_name


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_int_param
   //
   virtual function void set_int_param(input int param_key, input int param_value);
      if (m_etlink_monitor != null) begin
         m_etlink_monitor.set_int_param(param_key, param_value);
      end
      super.set_int_param(param_key, param_value);
   endfunction : set_int_param


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
            if (m_bank_mask == 0) begin
               m_sa_req_desc_queues[0].push(req_desc);
            end
            else begin
               evl_paddr_t addr;
               int         port;

               addr = req_desc.get_paddr();
               if (evl_etlink_req_opcode_t'(req_desc.get_bus_cmd()) == ET_LINK_REQ_MsgSendData) begin
                  port = m_global_attrs.get_bank_from_message(m_shire_id, addr);
               end
               else if (req_desc.is_cached() == 1'b1) begin
                  port = addr[`SC_BANK_SEL_ADDR_RANGE] & m_bank_mask;
               end
               else begin
                  port = UNCACHED_PORT;
               end
               m_sa_req_desc_queues[port].push(req_desc);
            end
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
      int          source_id;

      if (m_obs_req_fifo == null) begin
         return;
      end
      source_id = get_inst_id();
      forever begin
         int          port;
         evl_req_desc exp_req;

         m_obs_req_fifo.get(obs_req);
         m_saw_request = 1'b1;
         exp_req       = null;
         `evl_log_mdesc(UVM_HIGH, VMOD_ETLINK_MON, get_abstract_name(), obs_req, $sformatf("saw this observed request: %s%0s", obs_req.sprint_obj(), ((obs_req.is_cached() == 1'b1) ? "" : " (uncached)")))

         if (m_bank_mask <= 0) begin
            port = 0;
         end
         else if (evl_etlink_req_opcode_t'(obs_req.get_bus_cmd()) == ET_LINK_REQ_MsgSendData) begin
            port = m_global_attrs.get_bank_from_message(m_shire_id, obs_req.get_paddr());
         end
         else if (obs_req.is_cached() == 1'b1) begin
            evl_paddr_t addr = obs_req.get_paddr();

            port = addr[`SC_BANK_SEL_ADDR_RANGE] & m_bank_mask;
         end
         else begin
            port = UNCACHED_PORT;
         end

         //
         // If this interface isn't driven by a stimulus agent, check against scoreboard prediction
         //
         if (is_active_master() == 1'b0) begin
            `evl_log_mdesc(UVM_MEDIUM, VMOD_GLOBAL, get_abstract_name(), obs_req, obs_req.sprint_obj(1))
            if (obs_req.do_parent_check() == 1'b1) begin
               bit            mismatch = 1'b0;
               evl_verif_comp master_port_agent;

               master_port_agent = get_master_port_agent();
               exp_req = obs_req.get_parent_req_desc();
               if ((exp_req == null) && (master_port_agent != this) && (master_port_agent.is_req_checked(obs_req) == 1'b1)) begin
                  mismatch = 1'b1;
                  `dut_error_mdesc(get_abstract_name(), obs_req, $sformatf("%0s is an unexpected request", obs_req.sprint_obj()), pk_sc_etlink_error_intensity, { VMOD_ETLINK_SB })
               end
               if ((exp_req != null) && (master_port_agent != this) && (master_port_agent.is_req_checked(obs_req) == 1'b1)) begin
                  evl_line_data exp_req_data;
                  evl_line_data obs_req_data;

                  if ( (exp_req.get_bus_cmd() != obs_req.get_bus_cmd()) ||
                       (exp_req.get_paddr()   != obs_req.get_paddr()) ) begin
                     mismatch = 1'b1;
                     `dut_error_mdesc(get_abstract_name(), obs_req, $sformatf("expected %0s, not %0s", exp_req.sprint_obj(), obs_req.sprint_obj()), pk_sc_etlink_error_intensity, { VMOD_ETLINK_SB })
                  end
                  if (exp_req.get_bus_req_id() != obs_req.get_bus_req_id()) begin
                     mismatch = 1'b1;
                     `dut_error_mdesc(get_abstract_name(), obs_req, $sformatf("expected ET link request ID of 0x%x for %0s, not 0x%x", exp_req.get_bus_req_id(), obs_req.sprint_obj(), obs_req.get_bus_req_id()), pk_sc_etlink_error_intensity, { VMOD_ETLINK_SB })
                  end
                  if (exp_req.bus_cmd_is_write() == 1'b1) begin
                     exp_req_data = exp_req.get_req_data();
                     obs_req_data = obs_req.get_req_data();
                     if (exp_req_data == null) begin
                        `dut_error_mdesc(get_abstract_name(), exp_req, $sformatf("TB_FAIL: request data for expected request is null (%0s, line %0d)", `__FILE__, `__LINE__), pk_sc_etlink_error_intensity, { VMOD_ETLINK_SB })
                     end
                     else if (obs_req_data == null) begin
                        `dut_error_mdesc(get_abstract_name(), obs_req, $sformatf("request data for %0s is null", obs_req.sprint_obj()), pk_sc_etlink_error_intensity, { VMOD_ETLINK_SB })
                     end
                     else begin
                        string err_messages[$];

                        err_messages = {};
                        if (obs_req_data.compare_data_and_byte_ens(exp_req_data, err_messages) == 1'b1) begin
                           foreach (err_messages[ii]) begin
                              `dut_error_mdesc(get_abstract_name(), obs_req, err_messages[ii], pk_sc_etlink_error_intensity, { VMOD_ETLINK_SB })
                           end
                        end
                     end
                     if (mismatch == 1'b1) begin
                        exp_req = null;
                     end
                  end
               end
            end
            m_global_attrs.bus_trigger(obs_req.get_paddr(), obs_req.get_bus_req_id(), source_id);
         end
         //
         // This is driven by a stimulus agent, just link descriptors
         //
         else begin
            if (m_sa_req_desc_queues[port].size() == 0) begin
               `evl_log_mdesc(UVM_MEDIUM, VMOD_GLOBAL, get_abstract_name(), obs_req, obs_req.sprint_obj(1))
               `dut_error_mdesc(get_abstract_name(), obs_req, $sformatf("TB_ISSUE: process_observed_requests not expecting m_sa_req_desc_queues[%0d] to be empty (%0s, line %0d)", port, `__FILE__, `__LINE__))
               continue;
            end
            exp_req = m_sa_req_desc_queues[port].pop();

            // These are supposed to match. Do a couple of quick checks.
            if ((exp_req.get_bus_cmd()    == obs_req.get_bus_cmd()) &&
                (exp_req.get_paddr()      == obs_req.get_paddr())   &&
                (exp_req.get_bus_req_id() == obs_req.get_bus_req_id())) begin
               // The observed request is going to be the child of the sent request
               void'(exp_req.add_child_desc(obs_req));
               exp_req.set_bus_req_desc(obs_req);
               obs_req.reset_create_time();
               obs_req.set_correlated();
               `evl_log_mdesc(UVM_MEDIUM, VMOD_GLOBAL, get_abstract_name(), obs_req, obs_req.sprint_obj(1))
               // If we don't expect a response, mark it all as done.
               if (exp_req.bus_cmd_requires_rsp() == 1'b0) begin
                  exp_req.set_done();
               end
            end
            else begin
               `evl_log_mdesc(UVM_MEDIUM, VMOD_GLOBAL, get_abstract_name(), obs_req, obs_req.sprint_obj(1))
               `dut_error(get_abstract_name(), $sformatf("process_observed_requests() expected sent request and observed request to match but they didn't: sent: %s ... observed: %s", exp_req.sprint_obj(), obs_req.sprint_obj()))
               exp_req = null;
            end
         end
         if (exp_req != null) begin
            evl_mem_desc root_desc = exp_req.get_root_desc();

            exp_req.set_issued();
            // Reset the timeout value now that the request has been issued:
            if (root_desc != null) begin
               root_desc.reset_timeout_value();
            end
         end
         if (obs_req != null) begin
            obs_req.set_issued();
         end
         m_obs_req_port.write(obs_req);
      end
   endtask : process_observed_requests


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
      evl_paddr_t  addr;
      int          port;
      evl_req_desc snd_req;

      if ((is_active_master() == 1'b0) || (paired_req_desc == null)) begin
         return null;
      end
      addr = paired_req_desc.get_paddr();
      if (m_bank_mask <= 0) begin
         port = 0;
      end
      if (evl_etlink_req_opcode_t'(paired_req_desc.get_bus_cmd()) == ET_LINK_REQ_MsgSendData) begin
         port = m_global_attrs.get_bank_from_message(m_shire_id, addr);
      end
      else if (paired_req_desc.is_cached() == 1'b1) begin
         port = addr[`SC_BANK_SEL_ADDR_RANGE] & m_bank_mask;
      end
      else begin
         port = UNCACHED_PORT;
      end
      if (m_sa_req_desc_queues[port].size() > 0) begin
         snd_req = m_sa_req_desc_queues[port].peek();
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
   // Function: find_observed_response
   //
   // The purpose of this task is to receive observed responses from the monitor, transfer
   // information (ex: rsp data) to the original request descriptor sent by the stimulus agent, and
   // send the request descriptor back to the stimulus agent.
   //
   virtual function evl_req_desc find_observed_response(input int                 port_in,
                                                        input evl_req_desc        paired_req_desc,
                                                        input bit                 compare_fields = 1'b0,
                                                        input byte                qualifier      = 0,
                                                        input evl_bus_rsp_type_t  rsp_type_in    = `EVL_Rsp_Null,
                                                        input evl_bus_req_id_t    req_id_in      = ~evl_bus_req_id_t'(0),
                                                        input evl_bus_rsp_dest_t  dest_id_in     = ~evl_bus_rsp_dest_t'(0),
                                                        input evl_xword_byte_en_t rsp_byte_en_in = evl_xword_byte_en_t'(0),
                                                        input evl_xword_t         rsp_data_in    = evl_xword_t'(0));
      evl_bus_rsp_type_t  rsp_type;
      evl_bus_req_id_t    req_id;
      evl_bus_rsp_dest_t  dest_id;
      evl_xword_t         rsp_data;
      evl_xword_byte_en_t rsp_byte_en;
      evl_req_desc        req_desc;

      if (paired_req_desc != null) begin
         rsp_type    = (rsp_type_in    == `EVL_Rsp_Null)           ? paired_req_desc.get_bus_rsp_type() : rsp_type_in;
         req_id      = (req_id_in      == ~evl_bus_req_id_t'(0))   ? paired_req_desc.get_bus_req_id()   : req_id_in;
         dest_id     = (dest_id_in     == ~evl_bus_rsp_dest_t'(0)) ? paired_req_desc.get_bus_rsp_dest() : dest_id_in;
         rsp_byte_en = (rsp_byte_en_in == evl_xword_byte_en_t'(0)) ? ~evl_xword_byte_en_t'(0)           : rsp_byte_en_in;
         rsp_data    = (rsp_data_in    == evl_xword_t'(0))         ? paired_req_desc.get_rsp_xword(0)   : rsp_data_in;
         if (paired_req_desc.is_correlated() == 1'b1) begin
            return paired_req_desc.get_parent_req_desc();
         end
      end
      else begin
         rsp_type    = rsp_type_in;
         req_id      = req_id_in;
         dest_id     = dest_id_in;
         rsp_byte_en = rsp_byte_en_in;
         rsp_data    = rsp_data_in;
      end
      if (int'(rsp_type) == int'(ET_LINK_RSP_MsgRcvData)) begin
         if (m_slave_port_agent != null) begin
            req_desc = m_slave_port_agent.find_observed_response(get_agent_id(), paired_req_desc, 1'b1, qualifier, rsp_type, req_id, dest_id, rsp_byte_en, rsp_data);
            if (req_desc != null) begin
               req_desc.set_correlated();
               `evl_log_mdesc(UVM_MEDIUM, VMOD_ETLINK_MON, get_abstract_name(), paired_req_desc, $sformatf("received a message ... found %0s", req_desc.sprint_obj()))
            end
         end
      end
      return req_desc;
   endfunction: find_observed_response


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
         m_obs_rsp_fifo.get(obs_rsp);
         m_saw_response = 1'b1;
         `evl_log_mdesc(UVM_HIGH, VMOD_ETLINK_MON, get_abstract_name(), obs_rsp, $sformatf("observed response for %s", obs_rsp.sprint_obj()))
         if (obs_rsp.is_correlated() == 1'b1) begin
            rcv_req = obs_rsp.get_parent_req_desc();
            if (rcv_req != null) begin
               rcv_req.replicate_rsp_info(obs_rsp);
               rcv_req.set_bus_rsp_valid(0, get_cycle_count());
            end
         end
         else begin
            rcv_req = obs_rsp;
         end

         // Mark responses done (Not messages)
         if (int'(obs_rsp.get_bus_rsp_type()) != int'(ET_LINK_RSP_MsgRcvData)) begin
            if ((int'(obs_rsp.get_bus_rsp_type()) == int'(ET_LINK_RSP_Err)) && (get_int_param(pk_no_error_response, 0) != 0)) begin
               `dut_error_mdesc(get_abstract_name(), obs_rsp, $sformatf("ETLink response errors are not expected (%0s)", obs_rsp.sprint_obj()), pk_sc_etlink_error_intensity, { VMOD_ETLINK_SB })
            end
            obs_rsp.set_done();
         end

         m_obs_rsp_port.write(rcv_req);

         if (m_sa_rcv_rsp_port != null) begin
            if (int'(obs_rsp.get_bus_rsp_type()) != int'(ET_LINK_RSP_MsgRcvData)) begin
               m_sa_rcv_rsp_port.write(rcv_req);
            end
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
         `evl_log(UVM_MEDIUM, VMOD_CONFIG, get_abstract_name(), $sformatf("connecting %0s -> %0s", req_desc_port.get_full_name(),export_in.get_full_name()))
         req_desc_port.connect(export_in);
      end
   endfunction : connect_my_port_to_export


   //------------------------------------------------------------------------------------------------
   //
   // build_phase
   //
   virtual function void build_phase(uvm_phase phase);
      super.build_phase(phase);

      $cast(m_etlink_rtl_port, m_rtl_port);
      if (m_rtl_port == null) begin
         `dut_error(get_abstract_name(), "etlink_verif_comp: m_rtl_port is null, SAD!")
      end

      m_etlink_monitor = evl_etlink_monitor::type_id::create("m_etlink_monitor", this);
      m_etlink_monitor.m_verif_comp      = this;
      m_etlink_monitor.m_port_id         = get_port_id();
      m_etlink_monitor.m_rtl_port        = m_etlink_rtl_port;
      m_etlink_monitor.m_etlink_rtl_port = m_etlink_rtl_port;

      if (is_active_master() == 1'b1) begin
         m_etlink_master_agent = evl_etlink_master_agent::type_id::create("m_etlink_master_agent", this);
         m_etlink_master_agent.m_etlink_rtl_port = m_etlink_rtl_port;
      end
      else if (is_active_slave() == 1'b1) begin
         m_etlink_slave_agent = evl_etlink_slave_agent::type_id::create("m_etlink_slave_agent", this);
         m_etlink_slave_agent.m_etlink_rtl_port = m_etlink_rtl_port;
      end

      m_obs_req_port = new("m_obs_req_port", this);
      m_obs_rsp_port = new("m_obs_rsp_port", this);
      m_obs_rsp_fifo = new("m_obs_rsp_fifo", this);

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
            m_sa_snd_req_export = new("m_sa_snd_req_export", this);
         end
      end

      if (m_etlink_master_agent != null) begin
         m_etlink_master_agent.set_abstract_name($sformatf("%0s(ETLINK)", get_root_abstract_name()));
      end
      if (m_etlink_slave_agent != null) begin
         m_etlink_slave_agent.set_abstract_name($sformatf("%0s(ETLINK)", get_root_abstract_name()));
      end
      if (m_etlink_monitor != null) begin
         m_etlink_monitor.set_abstract_name($sformatf("%0s(ETLINK-MON)", get_root_abstract_name()));
      end
   endfunction : build_phase


   //-----------------------------------------------------------------------------------------------
   //
   // connect_phase
   //
   virtual function void connect_phase(uvm_phase phase);
      super.connect_phase(phase);
      if (m_etlink_master_agent != null) begin
         m_sa_snd_req_export.connect(m_etlink_master_agent.m_sa_snd_req_export);
      end
      if (m_etlink_slave_agent != null) begin
         m_sa_snd_req_export.connect(m_etlink_slave_agent.m_sa_snd_req_export);
      end
      if (m_sa_snd_req_fifo != null) begin
         m_sa_snd_req_export.connect(m_sa_snd_req_fifo.analysis_export);
      end
      if (m_obs_req_fifo != null) begin
         m_etlink_monitor.m_obs_req_port.connect(m_obs_req_fifo.analysis_export);
      end
      if (m_obs_rsp_fifo != null) begin
         m_etlink_monitor.m_obs_rsp_port.connect(m_obs_rsp_fifo.analysis_export);
      end
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
      if (m_etlink_rtl_port != null) begin
         if (m_etlink_rtl_port.get_int_param(pk_num_banks, 0) > 1) begin
            m_bank_mask = m_etlink_rtl_port.get_int_param(pk_num_banks) - 1;
         end
      end
      if (is_active_master() == 1'b1) begin
         set_master_port_agent(this);
      end
      if (is_active_slave() == 1'b1) begin
         set_slave_port_agent(this);
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
      if (m_etlink_monitor != null) begin
         m_etlink_monitor.set_master_port_agent(get_master_port_agent());
         m_etlink_monitor.set_slave_port_agent(get_slave_port_agent());
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
            if (`evl_get_error_intensity(pk_sc_etlink_error_intensity) >= 0) begin
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
      super.new(name_in, parent_in, ETLINK_BUS_AGENT);
      foreach (m_sa_req_desc_queues[ii]) begin
         m_sa_req_desc_queues[ii] = new();
      end
      if (`et_test_plusargs("disableETLinkChecks") != 0) begin
         `evl_set_error_intensity(pk_sc_etlink_error_intensity, UVM_HIGH)
      end
      m_bank_mask = 0;
      m_err_pk    = pk_sc_etlink_error_intensity;
      m_vmod      = VMOD_ETLINK_SB;
   endfunction : new

endclass : evl_etlink_verif_comp

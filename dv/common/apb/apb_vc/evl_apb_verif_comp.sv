//--------------------------------------------------------------------------------------------------
//
// Class: evl_apb_verif_comp
//
// The APB Verification component contains a master driver, a slave driver, and a monitor.
// The drivers are not always present. For example, if this verification component is being used as an
// active master, then it will contain a master driver and a monitor, and it will receive
// requests to drive on the APB bus from a master stim agent. Likewise if it is being used as
// an active slave, then it will contain a slave driver and a monitor, and it will send observed
// requests to a slave stim agent (which contains a memory model), and the slave stim agent will
// send this verification component a response to drive on the bus.
//
// This verification component may also be passive, which means it won't drive anything onto the
// bus. In that case it will just contain a monitor.
//
// FUTURE: initially implementing only the passive mode.  A lot of code for
// active/passive master/slave is commented out and untested
//
class evl_apb_verif_comp extends evl_verif_comp;

   `uvm_component_utils(evl_apb_verif_comp)


   //-----------------------------------------------------------------------------------------------
   //
   // Member Variables and Objects
   //
   evl_apb_rtl_port     m_apb_rtl_port;
   evl_apb_master_agent m_apb_master_agent;
   evl_apb_slave_agent  m_apb_slave_agent;
   evl_apb_monitor      m_apb_monitor;

   // Port for receiving requests from stimulus agent.
   uvm_analysis_export   #(evl_req_desc) m_sa_snd_req_export;
   uvm_analysis_export   #(evl_req_desc) m_sa_snd_rsp_export;
   uvm_tlm_analysis_fifo #(evl_req_desc) m_sa_snd_req_fifo;

   // Ports for sending requests and responses back to the stimulus agent.
   uvm_analysis_port #(evl_req_desc) m_sa_rcv_req_port;
   uvm_analysis_port #(evl_req_desc) m_sa_rcv_rsp_port;

   // Ports for sending observed requests and responses from the monitor to the rest of the world.
   uvm_analysis_port     #(evl_req_desc) m_obs_req_port;
   uvm_tlm_analysis_fifo #(evl_req_desc) m_obs_req_fifo;
   uvm_analysis_port     #(evl_req_desc) m_obs_rsp_port;
   uvm_tlm_analysis_fifo #(evl_req_desc) m_obs_rsp_fifo;

   evl_req_desc m_sa_req_desc_queue[$];

   bit m_issue_dut_error_on_error_rsp;


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
   // Function: get_slave_sequencer
   //
   virtual function evl_push_sequencer#(evl_req_desc) get_slave_sequencer();
      if (m_apb_slave_agent != null) begin
         return m_apb_slave_agent.m_slave_sequencer;
      end
      return null;
   endfunction : get_slave_sequencer


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_abstract_name
   //
   virtual function void set_abstract_name(input string name_in);
      super.set_abstract_name(name_in);
      if (m_apb_master_agent != null) begin
         m_apb_master_agent.set_abstract_name($sformatf("%0s(APB)", get_root_abstract_name()));
      end
      if (m_apb_slave_agent != null) begin
         m_apb_slave_agent.set_abstract_name($sformatf("%0s(APB)", get_root_abstract_name()));
      end
      if (m_apb_monitor != null) begin
         m_apb_monitor.set_abstract_name($sformatf("%0s(APB-MON)", get_root_abstract_name()));
      end
   endfunction : set_abstract_name


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_int_param
   //
   virtual function void set_int_param(input int param_key, input int param_value);
      if (m_apb_monitor != null) begin
         m_apb_monitor.set_int_param(param_key, param_value);
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
      fork
         forever begin
            m_sa_snd_req_fifo.get(req_desc);
            if (req_desc != null) begin
               m_sa_req_desc_queue.push_back(req_desc);
            end
         end
      join_none
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
   virtual task process_observed_requests;
      evl_req_desc obs_req_desc;
      int          source_id;

      if (m_obs_req_fifo == null) begin
         return;
      end
      source_id = get_inst_id();
      forever begin
         m_obs_req_fifo.get(obs_req_desc);
         if (is_active_master() == 1'b0) begin
            `evl_log_mdesc(UVM_MEDIUM, VMOD_GLOBAL, get_abstract_name(), obs_req_desc, obs_req_desc.sprint_obj())
            if (is_active_slave() == 1'b1) begin
            end
         end
         else begin
            evl_req_desc exp_req_desc;

            if (m_sa_req_desc_queue.size() == 0) begin
               exp_req_desc = null;
            end
            else begin
               exp_req_desc = m_sa_req_desc_queue.pop_front();
            end
            if (exp_req_desc == null) begin
               `evl_log_mdesc(UVM_MEDIUM, VMOD_GLOBAL, get_abstract_name(), obs_req_desc, obs_req_desc.sprint_obj())
               `dut_error(get_abstract_name(), $sformatf("process_observed_requests(): unexpected request observed: %s", obs_req_desc.sprint_obj()))
            end
            else if ((exp_req_desc.get_bus_cmd() == obs_req_desc.get_bus_cmd()) &&
                     (exp_req_desc.get_paddr()   == obs_req_desc.get_paddr())) begin
               evl_mem_desc root_desc;

               // The observed request is going to be the child of the sent request.
               void'(exp_req_desc.add_child_desc(obs_req_desc));
               obs_req_desc.set_correlated();
               `evl_log_mdesc(UVM_MEDIUM, VMOD_GLOBAL, get_abstract_name(), obs_req_desc, obs_req_desc.sprint_obj())
               exp_req_desc.set_issued();
               root_desc = exp_req_desc.get_root_desc();

               // Reset the timeout value now that the request has been issued.
               if (root_desc != null) begin
                  root_desc.reset_timeout_value();
               end
            end
            else begin
               `evl_log_mdesc(UVM_MEDIUM, VMOD_GLOBAL, get_abstract_name(), obs_req_desc, obs_req_desc.sprint_obj())
               `dut_error(get_abstract_name(), $sformatf("process_observed_requests(): expected the sent request and observed request to match but they did not: sent: %s ... observed: %s", exp_req_desc.sprint_obj(), obs_req_desc.sprint_obj()))
               exp_req_desc = null;
            end
         end
         m_global_attrs.bus_trigger(obs_req_desc.get_paddr(), obs_req_desc.get_bus_req_id(), source_id);
         m_obs_req_port.write(obs_req_desc);
      end
   endtask : process_observed_requests


   //-----------------------------------------------------------------------------------------------
   //
   // Function: process_observed_responses
   //
   virtual task process_observed_responses;
      evl_req_desc obs_req_desc;
      evl_req_desc exp_req_desc;

      if (m_obs_rsp_fifo == null) begin
         return;
      end
      forever begin
         //
         // Get the next observed response from the monitor.
         //
         m_obs_rsp_fifo.get(obs_req_desc);
         `evl_log_mdesc(UVM_MEDIUM, VMOD_GLOBAL, get_abstract_name(), obs_req_desc, obs_req_desc.sprint_rsp())
         if ((m_issue_dut_error_on_error_rsp == 1'b1) && (obs_req_desc.bus_rsp_has_error() == 1'b1)) begin
            `dut_error(get_abstract_name(), "Received an unexpected APB error response")
         end

         //
         // The request descriptor returned as the observed response is the child descriptor that
         // was created as an observed request.  Get its parent (if it exists) and copy response
         // information into it (ideally, we would compare observed and expected response
         // information here, but the APB VC is never currently attached to a scoreboard).
         //
         if (obs_req_desc.is_correlated() == 1'b1) begin
            exp_req_desc = obs_req_desc.get_parent_req_desc();
         end
         if (exp_req_desc != null) begin
            exp_req_desc.replicate_rsp_info(obs_req_desc);
         end
         else begin
            exp_req_desc = obs_req_desc;
         end
         if (m_sa_rcv_rsp_port != null) begin
            m_sa_rcv_rsp_port.write(exp_req_desc);
         end
      end
   endtask : process_observed_responses


   //-----------------------------------------------------------------------------------------------
   //
   // build_phase
   //
   virtual function void build_phase(uvm_phase phase);
      super.build_phase(phase);

      $cast(m_apb_rtl_port, m_rtl_port);
      if (m_rtl_port == null) begin
         `dut_error(get_abstract_name(), "apb_verif_comp: m_rtl_port is null, SAD!")
      end

      m_apb_monitor = evl_apb_monitor::type_id::create("m_apb_monitor", this);
      m_apb_monitor.m_verif_comp = this;
      m_apb_monitor.m_port_id    = get_port_id();
      m_apb_monitor.set_rtl_port(m_apb_rtl_port);

      if (is_active_master() == 1'b1) begin
         m_apb_master_agent = evl_apb_master_agent::type_id::create("m_apb_master_agent", this);
         m_apb_master_agent.set_rtl_port(m_apb_rtl_port);
      end
      else if (is_active_slave() == 1'b1) begin
         m_apb_slave_agent = evl_apb_slave_agent::type_id::create("m_apb_slave_agent", this);
         m_apb_slave_agent.set_rtl_port(m_apb_rtl_port);
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
            m_sa_snd_rsp_export = new("m_sa_snd_rsp_export", this);
         end
      end

      if (m_apb_master_agent != null) begin
         m_apb_master_agent.set_abstract_name($sformatf("%0s(APB)", get_root_abstract_name()));
      end
      if (m_apb_slave_agent != null) begin
         m_apb_slave_agent.set_abstract_name($sformatf("%0s(APB)", get_root_abstract_name()));
      end
      if (m_apb_monitor != null) begin
         m_apb_monitor.set_abstract_name($sformatf("%0s(APB-MON)", get_root_abstract_name()));
      end
   endfunction : build_phase


   //-----------------------------------------------------------------------------------------------
   //
   // connect_phase
   //
   virtual function void connect_phase(uvm_phase phase);
      super.connect_phase(phase);
      m_apb_monitor.m_obs_req_port.connect(m_obs_req_port);
      m_apb_monitor.m_obs_rsp_port.connect(m_obs_rsp_port);
      if (m_apb_master_agent != null) begin
         m_sa_snd_req_export.connect(m_apb_master_agent.m_sa_snd_req_export);
      end
      if (m_apb_slave_agent != null) begin
         if (is_active_slave() == 1'b1) begin
            m_sa_snd_rsp_export.connect(m_apb_slave_agent.m_sa_snd_rsp_export);
         end
      end
      if (m_sa_snd_req_fifo != null) begin
         m_sa_snd_req_export.connect(m_sa_snd_req_fifo.analysis_export);
      end
      if (m_obs_req_fifo != null) begin
         m_apb_monitor.m_obs_req_port.connect(m_obs_req_fifo.analysis_export);
      end
      if (m_obs_rsp_fifo != null) begin
         m_apb_monitor.m_obs_rsp_port.connect(m_obs_rsp_fifo.analysis_export);
      end
   endfunction: connect_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Function: connect_reg_predictor_to_monitor
   //
   virtual function void connect_reg_predictor_to_monitor(uvm_reg_predictor #(evl_req_desc) predictor_in);
      m_apb_monitor.m_obs_rsp_port.connect(predictor_in.bus_in);
   endfunction : connect_reg_predictor_to_monitor


   //-----------------------------------------------------------------------------------------------
   //
   // Task: run_phase
   //
   virtual task run_phase(uvm_phase phase);
      if (m_apb_monitor != null) begin
         m_apb_monitor.set_master_port_agent(get_master_port_agent());
         m_apb_monitor.set_slave_port_agent(get_slave_port_agent());
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
      super.new(name_in, parent_in, APB_BUS_AGENT);

      if (`et_value_plusargs("issue_dut_error_on_error_rsp=%d", m_issue_dut_error_on_error_rsp) == 0) begin
         m_issue_dut_error_on_error_rsp = `EVL_ISSUE_DUT_ERROR_ON_APB_ERR_RSP_DEFAULT;
      end
   endfunction : new

endclass : evl_apb_verif_comp

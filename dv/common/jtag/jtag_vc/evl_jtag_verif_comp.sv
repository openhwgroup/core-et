//--------------------------------------------------------------------------------------------------
//
// Class: evl_jtag_verif_comp
//
// The JTAG Verification component contains a master driver, a slave driver, and a monitor.
// The drivers are not always present. For example, if this verification component is being used as an
// active master, then it will contain a master driver and a monitor, and it will receive
// requests to drive on the JTAG bus from a master stim agent. Likewise if it is being used as
// an active slave, then it will contain a slave driver and a monitor, and it will send observed
// requests to a slave stim agent (which contains a memory model), and the slave stim agent will
// send this verification component a response to drive on the bus.
//
// This verification component may also be passive, which means it won't drive anything onto the
// bus. In that case it will just contain a monitor.
//
class evl_jtag_verif_comp extends evl_verif_comp;

   `uvm_component_utils(evl_jtag_verif_comp)

   //-----------------------------------------------------------------------------------------------
   //
   // Static Member Variables and Objects
   //


   //-----------------------------------------------------------------------------------------------
   //
   // Member Variables and Objects
   //
   evl_jtag_master_agent  m_master_agent;
   evl_jtag_monitor       m_monitor;

   // port for receiving requests from stimulus agent.
   uvm_analysis_export   #(evl_req_desc) m_sa_snd_req_export;
   uvm_tlm_analysis_fifo #(evl_req_desc) m_sa_snd_req_fifo;

   // port for sending responses back to the stimulus agent.
   uvm_analysis_port     #(evl_req_desc) m_sa_rcv_rsp_port;

   // ports for sending observed requests and responses from monitor to the rest of the world
   uvm_tlm_analysis_fifo #(evl_req_desc) m_exp_rsp_fifo;


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_abstract_name
   //
   virtual function void set_abstract_name(input string name_in);
      super.set_abstract_name(name_in);
      if (m_monitor != null) begin
         m_monitor.set_abstract_name($sformatf("%0s(JTAG-MON)", get_root_abstract_name()));
      end
      if (m_master_agent != null) begin
         m_master_agent.set_abstract_name($sformatf("%0s(JTAG)", get_root_abstract_name()));
      end
   endfunction : set_abstract_name


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_req_desc_port
   //
   virtual function uvm_analysis_port#(evl_req_desc) get_req_desc_port(input string port_name      = "unknown",
                                                                       input int    port_number    = 0,
                                                                       input int    subport_number = 0);
      if ((port_name == "m_sa_rcv_rsp_port") || (port_name == "sa_rcv_rsp") || (port_name == "m_rcv_rsp_port")) begin
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
   // Function: process_observed_responses
   //
   task process_observed_responses;
      evl_req_desc exp_req_desc;

      forever begin
         m_exp_rsp_fifo.get(exp_req_desc);
         if (m_sa_rcv_rsp_port != null) begin
            m_sa_rcv_rsp_port.write(exp_req_desc);
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

      if (m_rtl_port == null) begin
         `dut_error(get_abstract_name(), "jtag_verif_comp: m_rtl_port is null, SAD!")
      end

      if (is_active_master() == 1'b1) begin
         m_monitor = evl_jtag_monitor::type_id::create("m_monitor", this);
         m_monitor.m_verif_comp = this;
         m_monitor.set_rtl_port(get_rtl_port());

         m_master_agent = evl_jtag_master_agent::type_id::create("m_master_agent", this);
         m_master_agent.set_rtl_port(get_rtl_port());
      end

      if (is_active_master() == 1'b1) begin
         m_exp_rsp_fifo      = new("m_exp_rsp_fifo", this);
         m_sa_snd_req_export = new("m_sa_snd_req_export", this);
         m_sa_rcv_rsp_port   = new("m_sa_rcv_rsp_port", this);
         m_sa_snd_req_fifo   = new("m_sa_snd_req_fifo", this);
      end

      if (m_master_agent != null) begin
         m_master_agent.set_abstract_name($sformatf("%0s(JTAG)", get_root_abstract_name()));
      end
      if (m_monitor != null) begin
         m_monitor.set_abstract_name($sformatf("%0s(JTAG-MON)", get_root_abstract_name()));
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
         m_sa_snd_req_export.connect(m_monitor.m_snd_req_fifo.analysis_export);
         m_monitor.m_obs_rsp_port.connect(m_exp_rsp_fifo.analysis_export);
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
      fork
         super.run_phase(phase);
         if (m_master_agent != null) begin
            process_observed_responses();
         end
      join_none
   endtask: run_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Function: shutdown_phase
   //
// virtual task shutdown_phase(uvm_phase phase);
//    fork
//       begin
//          super.shutdown_phase(phase);
//       end
//       begin
//          phase.raise_objection(this);
//          wait_for_active_mem_desc(m_global_attrs.get_timeout_value(ShutdownPhase), 1'b1, 1'b0, 1'b1);
//          phase.drop_objection(this);
//       end
//    join
// endtask : shutdown_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string name_in = "evl_verif_comp", input uvm_component parent_in = null);
      super.new(name_in, parent_in, JTAG_BUS_AGENT);
      m_allow_sys_clk = 1'b0;
   endfunction : new

endclass : evl_jtag_verif_comp

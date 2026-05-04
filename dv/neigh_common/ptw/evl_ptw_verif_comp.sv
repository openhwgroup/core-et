//--------------------------------------------------------------------------------------------------
//
// Class: evl_ptw_verif_comp
//
// The PTW Verification component only contains a monitor and is always used passive (for now)
//
class evl_ptw_verif_comp extends evl_verif_comp;

   `uvm_component_utils(evl_ptw_verif_comp)

   //-----------------------------------------------------------------------------------------------
   //
   // Static Member Variables and Objects
   //


   //-----------------------------------------------------------------------------------------------
   //
   // Member Variables and Objects
   //
   evl_ptw_monitor       m_monitor;

   // ports for sending observed requests and responses from monitor to the rest of the world
   uvm_analysis_port     #(evl_ptw_desc) m_obs_req_port;
   uvm_analysis_port     #(evl_ptw_desc) m_obs_rsp_port;
   uvm_tlm_analysis_fifo #(evl_ptw_desc) m_obs_req_fifo;
   uvm_tlm_analysis_fifo #(evl_ptw_desc) m_obs_rsp_fifo;


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_abstract_name
   //
   virtual function void set_abstract_name(input string name_in);
      super.set_abstract_name(name_in);
      if (m_monitor != null) begin
         m_monitor.set_abstract_name($sformatf("%0s(PTW-MON)", get_root_abstract_name()));
      end
   endfunction : set_abstract_name


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_ptw_desc_port
   //
   virtual function uvm_analysis_port#(evl_ptw_desc) get_ptw_desc_port(input string port_name   = "unknown",
                                                                       input int    port_number = 0);
      if ((port_name == "m_obs_req_port") || (port_name == "obs_req_port") || (port_name == "obs_req")) begin
         return m_obs_req_port;
      end
      else if ((port_name == "m_obs_rsp_port") || (port_name == "obs_rsp_port") || (port_name == "obs_rsp")) begin
         return m_obs_rsp_port;
      end
      return null;
   endfunction : get_ptw_desc_port


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_ptw_desc_export
   //
   virtual function uvm_analysis_export#(evl_ptw_desc) get_ptw_desc_export(input string port_name   = "unknown",
                                                                           input int    port_number = 0);
      return null;
   endfunction : get_ptw_desc_export


   //-----------------------------------------------------------------------------------------------
   //
   // process_observed_requests
   //
   virtual task process_observed_requests;
      evl_ptw_desc obs_req;
      fork
         forever begin

            m_obs_req_fifo.get(obs_req);
            add_active_mem_desc(obs_req);
            // `evl_log_mdesc(UVM_HIGH, VMOD_PTW_MON, get_abstract_name(), obs_req, $sformatf("observed this PTW request: %s", obs_req.sprint_obj()))
            m_obs_req_port.write(obs_req);
         end

      join_none
   endtask : process_observed_requests


   //-----------------------------------------------------------------------------------------------
   //
   // Function: process_observed_responses
   //
   task process_observed_responses;
      evl_ptw_desc obs_rsp;
      fork
         forever begin
            m_obs_rsp_fifo.get(obs_rsp);
            delete_active_mem_desc(obs_rsp);
            // `evl_log_mdesc(UVM_HIGH, VMOD_PTW_MON, get_abstract_name(), obs_req, $sformatf("observed this PTW response: %s", obs_rsp.sprint_obj()))
            m_obs_rsp_port.write(obs_rsp);
         end
      join_none
   endtask: process_observed_responses


   //------------------------------------------------------------------------------------------------
   //
   // build_phase
   //
   virtual function void build_phase(uvm_phase phase);
      super.build_phase(phase);

      if (m_rtl_port == null) begin
         `dut_error(get_abstract_name(), "ptw_verif_comp: m_rtl_port is null, SAD!")
      end
      m_monitor = evl_ptw_monitor::type_id::create("m_monitor", this);
      m_monitor.m_verif_comp = this;
      m_monitor.set_rtl_port(get_rtl_port());

      m_obs_req_port = new("m_obs_req_port", this);
      m_obs_rsp_port = new("m_obs_rsp_port", this);

      m_obs_req_fifo = new("m_obs_req_fifo", this);
      m_obs_rsp_fifo = new("m_obs_rsp_fifo", this);

      if (m_monitor != null) begin
         m_monitor.set_abstract_name($sformatf("%0s(PTW-MON)", get_root_abstract_name()));
      end
   endfunction : build_phase


   //-----------------------------------------------------------------------------------------------
   //
   // connect_phase
   //
   virtual function void connect_phase(uvm_phase phase);
      super.connect_phase(phase);
      if (m_obs_req_fifo != null) begin
         m_monitor.m_obs_req_port.connect(m_obs_req_fifo.analysis_export);
      end
      if (m_obs_rsp_fifo != null) begin
         m_monitor.m_obs_rsp_port.connect(m_obs_rsp_fifo.analysis_export);
      end
   endfunction: connect_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Task: run_phase
   //
   virtual task run_phase(uvm_phase phase);
      fork
         super.run_phase(phase);
         process_observed_requests();
         process_observed_responses();
      join_none;
   endtask: run_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Function: shutdown_phase
   //
   virtual task shutdown_phase(uvm_phase phase);
      // fork
      //    begin
      //       super.shutdown_phase(phase);
      //    end
      //    begin
      //       phase.raise_objection(this);
      //       wait_for_active_mem_desc(5000, 1'b1);
      //       phase.drop_objection(this);
      //    end
      // join
   endtask : shutdown_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string name_in = "evl_verif_comp", input uvm_component parent_in = null);
      super.new(name_in, parent_in, PTW_BUS_AGENT);
      set_abstract_name("PTW");
   endfunction : new

endclass : evl_ptw_verif_comp


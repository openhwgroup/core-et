//--------------------------------------------------------------------------------------------------
//
// Class: evl_apb_monitor
//
class evl_apb_monitor extends evl_monitor;

   `uvm_component_utils(evl_apb_monitor)

   // This is the handle to the RTL port
   evl_apb_rtl_port m_apb_rtl_port;

   int              m_bus_data_width    = `EVL_DWORD_BYTES;
   int              m_bus_data_width_log2;

   // This is a handle back to the verification component
   int              m_port_id           = 0;
   evl_verif_comp   m_verif_comp        = null;
   evl_verif_comp   m_master_port_agent = null;
   evl_verif_comp   m_slave_port_agent  = null;


   //-----------------------------------------------------------------------------------------------
   //
   // analysis port, to send the transactions to scoreboard/monitor/etc...
   //
   uvm_analysis_port #(evl_req_desc) m_obs_req_port;
   uvm_analysis_port #(evl_req_desc) m_obs_rsp_port;

   // This is the queue to hold requests so we can match up the responses
   evl_line_data  m_template_req_data;
   evl_line_desc  m_template_line_desc;
   evl_trans_desc m_template_trans_desc;
   evl_req_desc   m_template_req_desc;
   evl_req_desc   m_last_req_desc;


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
   // Get request objects from the interface, transfer the information
   // into a request descriptor, then send it on the obs_req_port.
   //
   task process_requests();
      evl_req_desc    req_desc;
      evl_apb_bus_req bus_req;

      forever begin
         m_apb_rtl_port.get_obs_req(bus_req);
         // Create a new req_desc and fill in the fields.
         $cast(req_desc, m_template_req_desc.clone());
         req_desc.mark_as_bus_child();
         if (bus_req.get_write() == 1'b1) begin
            req_desc.set_bus_cmd(evl_bus_cmd_t'(APB_REQ_WRITE));
            req_desc.set_paddr(bus_req.get_addr());
            req_desc.set_bus_req_size(m_bus_data_width_log2);
            copy_bus_req_data_to_desc(bus_req, req_desc);
         end
         else begin
            req_desc.set_bus_cmd(evl_bus_cmd_t'(APB_REQ_READ));
            req_desc.set_bus_req_size(m_bus_data_width_log2);
            req_desc.set_paddr(bus_req.get_addr());
         end
         m_last_req_desc = req_desc;

         req_desc.reset_create_time();
         m_obs_req_port.write(req_desc);
      end
   endtask : process_requests


   //-----------------------------------------------------------------------------------------------
   //
   // Task: process_responses
   //
   // Get request objects from the interface, transfer the information
   // into a request descriptor, then send it on the obs_req_port.
   //
   task process_responses();
      evl_apb_bus_rsp bus_rsp;

      forever begin
         m_apb_rtl_port.get_obs_rsp(bus_rsp);
         if (m_last_req_desc != null) begin
            if (m_last_req_desc.bus_cmd_is_read() == 1'b1) begin
               copy_bus_rsp_data_to_desc(bus_rsp, m_last_req_desc);
            end
            m_last_req_desc.set_bus_rsp_type(evl_bus_rsp_type_t'(bus_rsp.get_slv_err()));
            m_last_req_desc.set_bus_rsp_valid(0, m_apb_rtl_port.get_cycle_count());
            m_obs_rsp_port.write(m_last_req_desc);
         end
         m_last_req_desc = null;
      end
   endtask : process_responses


   //-----------------------------------------------------------------------------------------------
   function void copy_bus_req_data_to_desc(input evl_apb_bus_req bus_req, input evl_req_desc req_desc);
      req_desc.invalidate_req_data();
      if (m_bus_data_width == 4) begin
         req_desc.set_req_word(-1, evl_word_t'(bus_req.get_wdata()));
      end
      else begin
         req_desc.set_req_dword(-1, evl_dword_t'(bus_req.get_wdata()));
      end
   endfunction : copy_bus_req_data_to_desc


   //-----------------------------------------------------------------------------------------------
   function void copy_bus_rsp_data_to_desc(input evl_apb_bus_rsp bus_rsp, input evl_req_desc req_desc);
      req_desc.invalidate_rsp_data();
      if (m_bus_data_width == 4) begin
         req_desc.set_rsp_word(-1, evl_word_t'(bus_rsp.get_rdata()));
      end
      else begin
         req_desc.set_rsp_dword(-1, evl_dword_t'(bus_rsp.get_rdata()));
      end
   endfunction : copy_bus_rsp_data_to_desc


   //-----------------------------------------------------------------------------------------------
   virtual function void build_phase(uvm_phase phase);
      super.build_phase(phase);
      $cast(m_apb_rtl_port, m_rtl_port);
   endfunction : build_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Function: start_of_simulation_phase
   //
   virtual function void start_of_simulation_phase(uvm_phase phase);
      super.start_of_simulation_phase(phase);

      m_template_req_data   = evl_line_data::type_id::create($sformatf("%s(APB)", get_root_abstract_name()));
      m_template_line_desc  = evl_line_desc::type_id::create($sformatf("%s(APB)", get_root_abstract_name()));
      m_template_trans_desc = evl_trans_desc::type_id::create($sformatf("%s(APB)", get_root_abstract_name()));
      m_template_req_desc   = evl_req_desc::type_id::create($sformatf("%s(APB)", get_root_abstract_name()));

      m_template_trans_desc.set_shire_id(m_verif_comp.get_shire_id());
      m_template_trans_desc.set_neighborhood_id(m_verif_comp.get_neighborhood_id());
      m_template_trans_desc.set_core_id(m_verif_comp.get_core_id());

      m_template_req_desc.set_shire_id(m_verif_comp.get_shire_id());
      m_template_req_desc.set_neighborhood_id(m_verif_comp.get_neighborhood_id());
      m_template_req_desc.set_core_id(m_verif_comp.get_core_id());

      m_template_line_desc.set_data_template(m_template_req_data);
      m_template_trans_desc.set_data_template(m_template_req_data);
      m_template_req_desc.set_data_template(m_template_req_data);
      m_template_req_desc.set_bus_type(EVL_BUS_TYPE_APB);

      m_template_trans_desc.set_line_template(m_template_line_desc);
      m_template_req_desc.set_line_template(m_template_line_desc);
   endfunction : start_of_simulation_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Task: run_phase
   //
   virtual task run_phase(uvm_phase phase);
      super.run_phase(phase);
      if (m_rtl_port != null) begin
         m_bus_data_width = m_rtl_port.get_bus_data_width();
      end
      m_bus_data_width_log2 = $clog2(m_bus_data_width);
      fork
         process_requests();
         process_responses();
      join_none;
   endtask : run_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string name_in = "evl_apb_monitor", input uvm_component parent_in = null);
      super.new(name_in, parent_in);
      m_obs_req_port = new("m_obs_req_port", this);
      m_obs_rsp_port = new("m_obs_rsp_port", this);
   endfunction : new

endclass: evl_apb_monitor


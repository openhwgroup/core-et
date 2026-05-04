//--------------------------------------------------------------------------------------------------
//
// Class: evl_jtag_monitor
//
class evl_jtag_monitor extends evl_monitor;

   `uvm_component_utils(evl_jtag_monitor)

   //-----------------------------------------------------------------------------------------------
   //
   // This following variable is a handle to the RTL port (which is instantiated in the
   // evl_jtag_interface module).
   //
   evl_jtag_rtl_port  m_jtag_rtl_port;

   //-----------------------------------------------------------------------------------------------
   //
   // The following variable is a handle back to the bus verification component.
   //
   evl_verif_comp     m_verif_comp = null;

   //-----------------------------------------------------------------------------------------------
   //
   // This following variable is a request descriptor queue to hold pending requests that can then
   // be matched with incoming responses.
   //
   evl_req_desc_queue m_request_queue;

   //-----------------------------------------------------------------------------------------------
   //
   // The following variables are templates from which to create new requests.  Each request
   // observed on the bus spawns a new request descriptor ... the following templates are used to
   // create the spawned descriptor.
   //
   evl_line_data      m_template_req_data;
   evl_line_desc      m_template_line_desc;
   evl_trans_desc     m_template_trans_desc;
   evl_req_desc       m_template_req_desc;

   //-----------------------------------------------------------------------------------------------
   //
   // The following analysis ports are used to send observed requests to a downstream object and
   // observed responses to an upstream object.
   //
   uvm_tlm_analysis_fifo #(evl_req_desc) m_snd_req_fifo;
   uvm_analysis_port     #(evl_req_desc) m_obs_rsp_port;


   //-----------------------------------------------------------------------------------------------
   //
   // Task: process_responses
   //
   // Get response objects from the interface, transfer the information into a request descriptor,
   // then send it on the m_exp_rsp_port.
   //
   task process_responses();
      evl_req_desc        req_desc;
      evl_jtag_rsp_info_t jtag_rsp;

      if (m_jtag_rtl_port == null) begin
         return;
      end
      forever begin
         m_snd_req_fifo.get(req_desc);
         m_jtag_rtl_port.get_jtag_rsp(jtag_rsp);
         req_desc.set_rsp_xword(0, jtag_rsp.data);
         m_obs_rsp_port.write(req_desc);
      end
   endtask : process_responses


   //-----------------------------------------------------------------------------------------------
   virtual function void build_phase(uvm_phase phase);
      super.build_phase(phase);
      m_snd_req_fifo = new("m_snd_req_fifo", this);
      m_obs_rsp_port = new("m_obs_rsp_port", this);
   endfunction : build_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Function: start_of_simulation_phase
   //
   virtual function void start_of_simulation_phase(uvm_phase phase);
      super.start_of_simulation_phase(phase);

      $cast(m_jtag_rtl_port, m_rtl_port);

      m_template_req_data   = evl_line_data::type_id::create($sformatf("%s(JTAG)", get_root_abstract_name()));
      m_template_line_desc  = evl_line_desc::type_id::create($sformatf("%s(JTAG)", get_root_abstract_name()));
      m_template_trans_desc = evl_trans_desc::type_id::create($sformatf("%s(JTAG)", get_root_abstract_name()));
      m_template_req_desc   = evl_req_desc::type_id::create($sformatf("%s(JTAG)", get_root_abstract_name()));

      m_template_trans_desc.set_shire_id(m_verif_comp.get_shire_id());
      m_template_trans_desc.set_neighborhood_id(m_verif_comp.get_neighborhood_id());
      m_template_trans_desc.set_core_id(m_verif_comp.get_core_id());

      m_template_req_desc.set_shire_id(m_verif_comp.get_shire_id());
      m_template_req_desc.set_neighborhood_id(m_verif_comp.get_neighborhood_id());
      m_template_req_desc.set_core_id(m_verif_comp.get_core_id());

      m_template_line_desc.set_data_template(m_template_req_data);
      m_template_trans_desc.set_data_template(m_template_req_data);
      m_template_req_desc.set_data_template(m_template_req_data);
      m_template_req_desc.set_bus_type(EVL_BUS_TYPE_JTAG);

      m_template_trans_desc.set_line_template(m_template_line_desc);
      m_template_req_desc.set_line_template(m_template_line_desc);
   endfunction : start_of_simulation_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Task: run_phase
   //
   virtual task run_phase(uvm_phase phase);
      super.run_phase(phase);
      fork
         process_responses();
      join_none;
   endtask : run_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string name_in = "evl_jtag_monitor", input uvm_component parent_in = null);
      super.new(name_in, parent_in);
   endfunction : new

endclass: evl_jtag_monitor

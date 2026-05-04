//--------------------------------------------------------------------------------------------------
//
// Class: evl_etlink_slave_agent
//
class evl_etlink_slave_agent extends evl_agent;

   `uvm_component_utils(evl_etlink_slave_agent)

   evl_etlink_rtl_port m_etlink_rtl_port;

   evl_etlink_slave_sequencer m_etlink_slave_sequencer;
   evl_etlink_slave_driver    m_etlink_slave_driver;

   // port for receiving requests from the stimulus agent.
   uvm_analysis_export #(evl_req_desc) m_sa_snd_req_export;


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_abstract_name
   //
   virtual function void set_abstract_name(input string name_in);
      super.set_abstract_name(name_in);
      if (m_etlink_slave_driver != null) begin
         m_etlink_slave_driver.set_abstract_name($sformatf("%0s(ETLINK-SDRV)", get_root_abstract_name()));
      end
      if (m_etlink_slave_sequencer != null) begin
         m_etlink_slave_sequencer.set_abstract_name($sformatf("%0s(ETLINK-SSEQ)", get_root_abstract_name()));
      end
   endfunction : set_abstract_name


   //-----------------------------------------------------------------------------------------------
   //
   // Function: build_phase
   //
   virtual function void build_phase(uvm_phase phase);
      super.build_phase(phase);

      m_etlink_slave_driver = evl_etlink_slave_driver::type_id::create("m_etlink_slave_driver",this);
      m_etlink_slave_sequencer = evl_etlink_slave_sequencer::type_id::create("m_etlink_slave_sequencer",this);

      uvm_config_db#(uvm_object_wrapper)::set(this, "m_etlink_slave_sequencer.run_phase", "default_sequence", evl_etlink_slave_seq::get_type());

      //Give the slave driver a pointer to the rtl port (the interface)
      if (m_etlink_rtl_port == null) begin
         `dut_error(get_abstract_name(), "evl_etlink_slave: m_etlink_rtl_port is null, sad!")
      end
      else begin
         m_etlink_slave_driver.m_etlink_rtl_port = m_etlink_rtl_port;
      end

      m_sa_snd_req_export = new("m_sa_snd_req_export",this);

      if (m_etlink_slave_driver != null) begin
         m_etlink_slave_driver.set_abstract_name($sformatf("%0s(ETLINK-SDRV)", get_root_abstract_name()));
      end
      if (m_etlink_slave_sequencer != null) begin
         m_etlink_slave_sequencer.set_abstract_name($sformatf("%0s(ETLINK-SSEQ)", get_root_abstract_name()));
      end
   endfunction : build_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Function: connect_phase
   //
   virtual function void connect_phase(uvm_phase phase);
      super.connect_phase(phase);
      m_sa_snd_req_export.connect(m_etlink_slave_sequencer.m_sa_snd_req_export);
      m_etlink_slave_sequencer.req_port.connect(m_etlink_slave_driver.req_export);
   endfunction :connect_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string name_in = "evl_etlink_slave_agent", input uvm_component parent_in = null);
      super.new(name_in, parent_in);
   endfunction : new

endclass : evl_etlink_slave_agent

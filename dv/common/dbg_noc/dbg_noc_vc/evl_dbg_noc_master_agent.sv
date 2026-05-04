//--------------------------------------------------------------------------------------------------
//
// Class: evl_dbg_noc_master_agent
//
class evl_dbg_noc_master_agent extends evl_agent;

   `uvm_component_utils(evl_dbg_noc_master_agent)

   evl_dbg_noc_master_sequencer m_master_sequencer;
   evl_dbg_noc_master_driver    m_master_driver;

   // port for receiving requests from the stimulus agent.
   uvm_analysis_export #(evl_req_desc) m_sa_snd_req_export;


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_abstract_name
   //
   virtual function void set_abstract_name(input string name_in);
      super.set_abstract_name(name_in);
      if (m_master_driver != null) begin
         m_master_driver.set_abstract_name($sformatf("%0s(DBG_NOC-MDRV)", get_root_abstract_name()));
      end
      if (m_master_sequencer != null) begin
         m_master_sequencer.set_abstract_name($sformatf("%0s(DBG_NOC-MSEQ)", get_root_abstract_name()));
      end
   endfunction : set_abstract_name


   //-----------------------------------------------------------------------------------------------
   //
   // build_phase
   //
   virtual function void build_phase(uvm_phase phase);
      super.build_phase(phase);

      m_master_driver    = evl_dbg_noc_master_driver::type_id::create("m_master_driver", this);
      m_master_sequencer = evl_dbg_noc_master_sequencer::type_id::create("m_master_sequencer", this);

      uvm_config_db#(uvm_object_wrapper)::set(this, "m_master_sequencer.run_phase", "default_sequence", evl_dbg_noc_master_seq::get_type());

      // Give the master driver a pointer to the rtl port (the interface)
      m_master_driver.set_rtl_port(get_rtl_port());

      m_sa_snd_req_export = new("m_sa_snd_req_export", this);
      if (m_master_driver != null) begin
         m_master_driver.set_abstract_name($sformatf("%0s(DBG_NOC-MDRV)", get_root_abstract_name()));
      end
      if (m_master_sequencer != null) begin
         m_master_sequencer.set_abstract_name($sformatf("%0s(DBG_NOC-MSEQ)", get_root_abstract_name()));
      end
   endfunction : build_phase


   //-----------------------------------------------------------------------------------------------
   //
   // connect_phase
   //
   virtual function void connect_phase(uvm_phase phase);
      super.connect_phase(phase);

      //
      // Connect the stimulus agent analysis port to the sequencer, and connect the sequencer's
      // analysis port to the driver.
      //
      if (m_sa_snd_req_export != null) begin
         m_sa_snd_req_export.connect(m_master_sequencer.m_sa_snd_req_export);
      end
      m_master_sequencer.req_port.connect(m_master_driver.req_export);
   endfunction :connect_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string name_in = "evl_dbg_noc_master_agent", input uvm_component parent_in = null);
      super.new(name_in, parent_in);
   endfunction : new

endclass : evl_dbg_noc_master_agent

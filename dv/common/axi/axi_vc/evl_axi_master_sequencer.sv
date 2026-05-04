//--------------------------------------------------------------------------------------------------
//
// Class: evl_axi_master_sequencer
//
class evl_axi_master_sequencer extends evl_push_sequencer#(evl_req_desc);


   `uvm_component_utils(evl_axi_master_sequencer)

   uvm_analysis_export   #(evl_req_desc) m_sa_snd_req_export;
   uvm_tlm_analysis_fifo #(evl_req_desc) m_sa_snd_req_fifo;

   //-----------------------------------------------------------------------------------------------
   //
   // Function: build_phase
   //
   virtual function void build_phase(uvm_phase phase);
      super.build_phase(phase);
      m_sa_snd_req_export = new("m_sa_snd_req_export", this);
      m_sa_snd_req_fifo   = new("m_sa_snd_req_fifo", this);
   endfunction : build_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Function: connect_phase
   //
   virtual function void connect_phase(uvm_phase phase);
      super.connect_phase(phase);
      m_sa_snd_req_export.connect(m_sa_snd_req_fifo.analysis_export);
   endfunction : connect_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string name_in = "evl_axi_master_sequencer", input uvm_component parent_in = null);
      super.new(name_in, parent_in);
   endfunction : new

endclass : evl_axi_master_sequencer

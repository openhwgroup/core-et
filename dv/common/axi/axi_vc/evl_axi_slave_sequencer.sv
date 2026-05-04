//--------------------------------------------------------------------------------------------------
//
// Class: evl_axi_slave_sequencer
//
class evl_axi_slave_sequencer extends evl_push_sequencer#(evl_req_desc);


   `uvm_component_utils(evl_axi_slave_sequencer)

   uvm_analysis_export   #(evl_req_desc) m_sa_snd_rsp_export;
   uvm_tlm_analysis_fifo #(evl_req_desc) m_sa_snd_rsp_fifo;

   //-----------------------------------------------------------------------------------------------
   //
   // Function: build_phase
   //
   virtual function void build_phase(uvm_phase phase);
      super.build_phase(phase);
      m_sa_snd_rsp_export = new("m_sa_snd_rsp_export", this);
      m_sa_snd_rsp_fifo   = new("m_sa_snd_rsp_fifo", this);
   endfunction : build_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Function: connect_phase
   //
   virtual function void connect_phase(uvm_phase phase);
      super.connect_phase(phase);
      m_sa_snd_rsp_export.connect(m_sa_snd_rsp_fifo.analysis_export);
   endfunction : connect_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string name_in = "evl_axi_slave_sequencer", input uvm_component parent_in = null);
      super.new(name_in, parent_in);
   endfunction : new

endclass : evl_axi_slave_sequencer


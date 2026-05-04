//--------------------------------------------------------------------------------------------------
//
// Class: evl_ust_slave_agent_axi
//
// This class is an extension of evl_stim_agent_axi to define a slave stim agent that acts like an
// L3 driving requests to the mesh.
//
class evl_ust_slave_agent_axi extends evl_stim_agent_axi;

   `uvm_component_utils(evl_ust_slave_agent_axi)



   //-----------------------------------------------------------------------------------------------
   //
   // Function: build_tables
   //
   virtual function void build_tables();

      // FUTURE maybe?  make some table entries!   (for now this is only used as a passive agent)

   endfunction : build_tables



   //-----------------------------------------------------------------------------------------------
   //
   // Function: build_phase
   //
   virtual function void build_phase(uvm_phase phase);
      super.build_phase(phase);
      m_snd_rsp_ports.push_back(null);
      m_obs_req_exports.push_back(null);
      m_obs_req_fifos.push_back(null);
      m_snd_rsp_ports[0]   = new("m_snd_req_port", this);
      m_obs_req_exports[0] = new("m_obs_req_export", this);
      m_obs_req_fifos[0]   = new("m_obs_req_fifo", this);
   endfunction : build_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string name_in = "evl_ust_slave_agent_axi", input uvm_component parent_in = null);
      super.new(name_in, parent_in, UST_SLAVE_STIM_AGENT);
      set_abstract_name("UST_SLAVE(STIM)");
      add_agent_alias(AXI_SLAVE_STIM_AGENT);
      set_active_slave();
   endfunction : new

endclass : evl_ust_slave_agent_axi

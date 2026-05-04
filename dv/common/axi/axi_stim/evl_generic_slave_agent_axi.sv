//--------------------------------------------------------------------------------------------------
//
// Class: evl_generic_slave_agent_axi
//
class evl_generic_slave_agent_axi extends evl_stim_agent_axi;

   `uvm_component_utils(evl_generic_slave_agent_axi)


   //-----------------------------------------------------------------------------------------------
   //
   // Function: build_tables
   //
   virtual function void build_tables();
      //
      //              |--------------------- Table Inputs ----------------------||---------------- Table Outputs ----------------|
      //               Cmd                  Model State  Qual         Source      Tag Action  Data Action  Bus Rsp    Bus Command
      //               -------------------  -----------  -----------  ----------  ----------  -----------  ---------  -----------
      add_bus_req_act( `EVL_AXI_REQ_READ,   I,           AXQ_Local,   AXI_NoSrc,  St_Ignore,  Data_MemRd,  RspWData,  BusIdle     );
      add_bus_req_act( `EVL_AXI_REQ_WRITE,  I,           AXQ_Local,   AXI_NoSrc,  St_Ignore,  Data_MemWr,  RspAck,    BusIdle     );
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
      m_snd_rsp_ports[0]   = new("m_snd_rsp_port",   this);
      m_obs_req_exports[0] = new("m_obs_req_export", this);
      m_obs_req_fifos[0]   = new("m_obs_req_fifo",   this);
   endfunction : build_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string name_in = "evl_generic_slave_agent_axi", input uvm_component parent_in = null);
      super.new(name_in, parent_in, GENERIC_AXI_SLAVE_STIM_AGENT);
      m_memory_agent   = 1'b1;
      m_allow_mem_load = 1'b1;
      set_abstract_name("SLAVE(STIM)");
      add_agent_alias(MEM_STIM_AGENT);
      add_agent_alias(AXI_SLAVE_STIM_AGENT);
      add_agent_alias(MEM_AGENT);
      set_active_slave();
   endfunction : new

endclass : evl_generic_slave_agent_axi

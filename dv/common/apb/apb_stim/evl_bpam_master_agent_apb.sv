//--------------------------------------------------------------------------------------------------
//
// Class: evl_bpam_master_agent_apb
//
// This class is an extension of evl_stim_agent_apb to define a master stim agent that acts like an
// L3 driving requests to the mesh.
//
class evl_bpam_master_agent_apb extends evl_stim_agent_apb;

   `uvm_component_utils(evl_bpam_master_agent_apb)


   //-----------------------------------------------------------------------------------------------
   //
   // Function: build_tables
   //
   virtual function void build_tables();
      //
      // Actions to take when we receive a request from the simulus.
      //
      //              |------- Table Inputs --------||---------- Table Outputs -----------|
      //               Cmd         Model State  Qual  Tag Action  Data Action  Bus Command
      //               ----------  -----------  ----  ----------  -----------  -----------
      add_int_req_act( LD,         I,           0,    St_Ignore,  Data_None,   BusRead     );
      add_int_req_act( ST,         I,           0,    St_Ignore,  Data_ReqWr,  BusWrite    );
      add_int_req_act( LD_NO_MOD,  I,           0,    St_Ignore,  Data_None,   BusRead     );
      add_int_req_act( ST_NO_MOD,  I,           0,    St_Ignore,  Data_ReqWr,  BusWrite    );
      add_int_req_act( LD_COOP,    I,           0,    St_Ignore,  Data_None,   BusRead     );
      add_int_req_act( ST_WA,      I,           0,    St_Ignore,  Data_ReqWr,  BusWrite    );

      //
      // Actions to take when a response comes back:
      //
      //              |------------ Table Inputs ------------||---- Table Outputs ----|
      //               Cmd                  Model State  Qual  Tag Action  Data Action
      //               -------------------  -----------  ----  ----------  -----------
      add_bus_rsp_act( `EVL_APB_REQ_READ,   I,           0,    St_Ignore,  Data_ReqRd  );
      add_bus_rsp_act( `EVL_APB_REQ_WRITE,  I,           0,    St_Ignore,  Data_None   );
   endfunction : build_tables


   //-----------------------------------------------------------------------------------------------
   //
   // Function: build_phase
   //
   virtual function void build_phase(uvm_phase phase);
      super.build_phase(phase);
      m_snd_req_port   = new("m_snd_req_port",   this);
      m_rcv_rsp_export = new("m_rcv_rsp_export", this);
      m_rcv_rsp_fifo   = new("m_rcv_rsp_fifo",   this);
   endfunction : build_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string name_in = "evl_bpam_master_agent_apb", input uvm_component parent_in = null);
      super.new(name_in, parent_in, BPAM_MASTER_STIM_AGENT);
      m_stimulus_agent = 1'b1;
      set_abstract_name("BPAM(STIM)");
      add_agent_alias(APB_MASTER_STIM_AGENT);
      set_active_master();
   endfunction : new

endclass : evl_bpam_master_agent_apb

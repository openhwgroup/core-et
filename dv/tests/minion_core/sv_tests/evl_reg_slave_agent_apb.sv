//--------------------------------------------------------------------------------------------------
//
// Class: evl_reg_slave_agent_apb
//
class evl_reg_slave_agent_apb extends evl_stim_agent_apb;

   `uvm_component_utils(evl_reg_slave_agent_apb)

   //-----------------------------------------------------------------------------------------------
   //
   // Function: build_tables
   //
   virtual function void build_tables();
      //                               Model
      //              Bus Cmd          State  Qual  Tag Action  Data Action  Bus Rsp
      //              ---------------  -----  ----  ----------  -----------  --------
      add_bus_req_act(`EVL_APB_READ,   I,     0,    St_Ignore,  Data_MemRd,  RspWData );
      add_bus_req_act(`EVL_APB_WRITE,  I,     0,    St_Ignore,  Data_MemWr,  RspAck   );
   endfunction : build_tables


   //-----------------------------------------------------------------------------------------------
   //
   // Function: build_phase
   //
   virtual function void build_phase(uvm_phase phase);
      super.build_phase(phase);
      m_snd_rsp_port   = new("m_snd_rsp_port", this);
      m_obs_req_export = new("m_obs_req_export",this);
      m_obs_req_fifo   = new("m_obs_req_fifo",this);
   endfunction : build_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Function: connect_my_bus_ports
   //
   // This function is used to connect all of the ports that go between this stimulus agent and its
   // bus agent.
   //
   virtual function void connect_my_bus_ports(input evl_verif_comp bus_verif_comp,
                                              input string         port_name      = "",
                                              input int            port_number    = 0,
                                              input int            subport_number = 0);
      uvm_analysis_port   #(evl_req_desc) bus_port;
      uvm_analysis_export #(evl_req_desc) bus_export;

      bus_export = bus_verif_comp.get_req_desc_export("m_sa_snd_rsp_export", port_number);
      if ((m_snd_rsp_port != null) && (bus_export != null)) begin
         `evl_log(UVM_MEDIUM, VMOD_CONFIG, get_abstract_name(), $sformatf("connecting %0s -> %0s", m_snd_rsp_port.get_full_name(), bus_export.get_full_name()))
         m_snd_rsp_port.connect(bus_export);
      end

      bus_port = bus_verif_comp.get_req_desc_port("m_obs_req_port", port_number);
      if ((m_obs_req_export != null) && (bus_port != null)) begin
         `evl_log(UVM_MEDIUM, VMOD_CONFIG, get_abstract_name(), $sformatf("connecting %0s -> %0s", bus_port.get_full_name(), m_obs_req_export.get_full_name()))
         bus_port.connect(m_obs_req_export);
      end
   endfunction : connect_my_bus_ports


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string name_in = "evl_reg_slave_agent_apb", input uvm_component parent_in = null);
      super.new(name_in, parent_in, REG_SLAVE_STIM_AGENT);
      m_memory_agent = 1'b0;
      add_agent_alias(APB_SLAVE_STIM_AGENT);
      add_agent_alias(MEM_AGENT);
      set_active_slave();
   endfunction : new

endclass : evl_reg_slave_agent_apb

//--------------------------------------------------------------------------------------------------
//
// Class: evl_mesh_slave_agent_axi
//
class evl_mesh_slave_agent_axi extends evl_stim_agent_axi;

   `uvm_component_utils(evl_mesh_slave_agent_axi)

   bit m_allow_l3_loopback = 1'b1;

   //-----------------------------------------------------------------------------------------------
   //
   // Function: build_tables
   //
   virtual function void build_tables();

      //              |--------------------- Table Inputs ----------------------||---------------- Table Outputs ----------------|
      //               Cmd                  Model State  Qual         Source      Tag Action  Data Action  Bus Rsp    Bus Command
      //               -------------------  -----------  -----------  ----------  ----------  -----------  ---------  -----------
      add_bus_req_act( `EVL_AXI_REQ_READ,   I,           AXQ_Local,   AXI_NoSrc,  St_Ignore,  Data_MemRd,  RspWData,  BusIdle     );
      add_bus_req_act( `EVL_AXI_REQ_WRITE,  I,           AXQ_Local,   AXI_NoSrc,  St_Ignore,  Data_MemWr,  RspAck,    BusIdle     );
      add_bus_req_act( `EVL_AXI_REQ_READ,   I,           AXQ_Remote,  AXI_NoSrc,  St_Ignore,  Data_None,   RspNull,   BusRead     );
      add_bus_req_act( `EVL_AXI_REQ_WRITE,  I,           AXQ_Remote,  AXI_NoSrc,  St_Ignore,  Data_FwdWr,  RspNull,   BusWrite    );

      // Actions to take when a response comes back:
      //
      // DataAction Definitions:
      // Data_ReqRd: Copy the data from the req_desc to the trans_desc so that the original test has access to it.
      //
      //              |--------------- Table Inputs ----------------||--------- Table Outputs ---------|
      //               Cmd                  Model State  Qual         Tag Action  Data Action  Bus Rsp
      //               -------------------  -----------  -----------  ----------  -----------  --------
      add_bus_rsp_act( `EVL_AXI_REQ_READ,   I,           AXQ_Remote,  St_Ignore,  Data_FwdRd,  RspWData );
      add_bus_rsp_act( `EVL_AXI_REQ_WRITE,  I,           AXQ_Remote,  St_Ignore,  Data_FwdWr,  RspAck   );

   endfunction : build_tables


   //-----------------------------------------------------------------------------------------------
   //
   // Function: address_is_mine
   //
   virtual function bit address_is_mine(input evl_paddr_t addr_in, input evl_axi_source_t axi_source, input evl_etlink_req_opcode_t opcode_in);
      if (int'(addr_in[`SC_SCP_REGION_SEL_ADDR_RANGE]) == int'(`SC_SCP_REGION_ID)) begin
         if ( (evl_shire_id_t'(addr_in[`SC_SCP_SHIRE_SEL_ADDR_RANGE]) == m_shire_id) ||
              (m_addr_regions.exists(pk_loopback_region) && (m_addr_regions[pk_loopback_region].addr_in_region(addr_in) == 1'b1)) ) begin
            return ~m_allow_l3_loopback;
         end
         else begin
            return 1'b1;
         end
      end
      if (m_addr_regions.exists(pk_loopback_region) && (m_addr_regions[pk_loopback_region].addr_in_region(addr_in) == 1'b1)) begin
         return 1'b0;
      end
      return ((m_global_attrs.address_is_in_my_l3(m_shire_id, addr_in) == 1'b1) ? ~m_allow_l3_loopback : 1'b1);
   endfunction : address_is_mine


   //-----------------------------------------------------------------------------------------------
   //
   // Function: build_phase
   //
   virtual function void build_phase(uvm_phase phase);
      super.build_phase(phase);
      for (int ii = 0; ii < m_num_l3_ports; ii++) begin
         m_snd_rsp_ports.push_back(null);
         m_obs_req_exports.push_back(null);
         m_obs_req_fifos.push_back(null);
         m_snd_rsp_ports[ii]   = new($sformatf("m_snd_rsp_port_%0d", ii), this);
         m_obs_req_exports[ii] = new($sformatf("m_obs_req_export_%0d", ii), this);
         m_obs_req_fifos[ii]   = new($sformatf("m_obs_req_fifo_%0d", ii), this);
      end
   endfunction : build_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string name_in = "evl_mesh_slave_agent_axi", input uvm_component parent_in = null, input evl_agent_type_t agent_type_in = MESH_SLAVE_STIM_AGENT);
      super.new(name_in, parent_in, agent_type_in);
      m_memory_agent   = 1'b1;
      m_allow_mem_load = 1'b1;
      set_abstract_name("MESH(STIM)");
      add_agent_alias(MEM_STIM_AGENT);
      add_agent_alias(AXI_SLAVE_STIM_AGENT);
      add_agent_alias(MEM_AGENT);
      set_active_slave();
   endfunction : new

endclass : evl_mesh_slave_agent_axi

//--------------------------------------------------------------------------------------------------
//
// Class: evl_minion_shire_slave_agent_axi
//
// This class is an extension of evl_stim_agent_axi to define a slave stimulus agent that acts like
// NoC on the L2-to-L3, L3-to-sys, UC-to-L3, and UC-to-sys AXI ports.
//
class evl_minion_shire_slave_agent_axi extends evl_stim_agent_axi;

   `uvm_component_utils(evl_minion_shire_slave_agent_axi)


   //-----------------------------------------------------------------------------------------------
   //
   // Member Variables and Objects
   //
   bit m_allow_l3_loopback  = 1'b1;
   bit m_check_sys_requests = 1'b0;
   int m_l2_to_l3_first_rcv_port;
   int m_l3_to_sys_first_rcv_port;
   int m_uc_to_l3_first_rcv_port;
   int m_uc_to_sys_first_rcv_port;
   int m_l2_to_l3_last_rcv_port;
   int m_l3_to_sys_last_rcv_port;
   int m_uc_to_l3_last_rcv_port;
   int m_uc_to_sys_last_rcv_port;

   int m_l2_to_l3_rcv_ports[$];
   int m_l3_to_sys_rcv_ports[$];
   int m_uc_to_l3_rcv_ports[$];
   int m_uc_to_sys_rcv_ports[$];
   int m_l3_loopback_snd_ports[$];
   int m_sys_loopback_snd_ports[$];
   int m_uc_l3_loopback_snd_ports[$];
   int m_uc_sys_loopback_snd_ports[$];


   //-----------------------------------------------------------------------------------------------
   //
   // Function: build_tables
   //
   virtual function void build_tables();

      //
      // Requests from the L2-to-L3 mesh AXI port
      //
      //              |----------------------- Table Inputs -----------------------||---------------- Table Outputs ----------------|
      //               Cmd                  Model State  Qual         Source         Tag Action  Data Action  Bus Rsp    Bus Command
      //               -------------------  -----------  -----------  -------------  ----------  -----------  ---------  -----------
      add_bus_req_act( `EVL_AXI_REQ_READ,   I,           AXQ_Local,   AXI_L2_to_L3,  St_L3ChkE,  Data_MemRd,  RspWData,  BusIdle     );
      add_bus_req_act( `EVL_AXI_REQ_READ,   E,           AXQ_Local,   AXI_L2_to_L3,  St_Ignore,  Data_MemRd,  RspWData,  BusIdle     );
      add_bus_req_act( `EVL_AXI_REQ_READ,   M,           AXQ_Local,   AXI_L2_to_L3,  St_Ignore,  Data_MemRd,  RspWData,  BusIdle     );
      add_bus_req_act( `EVL_AXI_REQ_WRITE,  I,           AXQ_Local,   AXI_L2_to_L3,  St_L3ChkM,  Data_MemWr,  RspAck,    BusIdle     );
      add_bus_req_act( `EVL_AXI_REQ_WRITE,  E,           AXQ_Local,   AXI_L2_to_L3,  M,          Data_MemWr,  RspAck,    BusIdle     );
      add_bus_req_act( `EVL_AXI_REQ_WRITE,  M,           AXQ_Local,   AXI_L2_to_L3,  St_Ignore,  Data_MemWr,  RspAck,    BusIdle     );
      add_bus_req_act( `EVL_AXI_REQ_READ,   I,           AXQ_Remote,  AXI_L2_to_L3,  St_Ignore,  Data_None,   RspNull,   BusRead     );
      add_bus_req_act( `EVL_AXI_REQ_WRITE,  I,           AXQ_Remote,  AXI_L2_to_L3,  St_Ignore,  Data_FwdWr,  RspNull,   BusWrite    );

      //
      // Requests from the L3-to-system mesh AXI port
      //
      //              |----------------------- Table Inputs ------------------------||----------------- Table Outputs -----------------|
      //               Cmd                  Model State  Qual         Source          Tag Action    Data Action  Bus Rsp    Bus Command
      //               -------------------  -----------  -----------  --------------  ------------  -----------  ---------  -----------
      add_bus_req_act( `EVL_AXI_REQ_READ,   I,           AXQ_Local,   AXI_L3_to_Sys,  St_SysChkI,   Data_MemRd,  RspWData,  BusIdle     );
      add_bus_req_act( `EVL_AXI_REQ_READ,   E,           AXQ_Local,   AXI_L3_to_Sys,  St_SysChkI,   Data_MemRd,  RspWData,  BusIdle     );
      add_bus_req_act( `EVL_AXI_REQ_READ,   M,           AXQ_Local,   AXI_L3_to_Sys,  St_SysChkI,   Data_MemRd,  RspWData,  BusIdle     );
      add_bus_req_act( `EVL_AXI_REQ_WRITE,  I,           AXQ_Local,   AXI_L3_to_Sys,  St_SysChkI,   Data_MemWr,  RspAck,    BusIdle     );
      add_bus_req_act( `EVL_AXI_REQ_WRITE,  E,           AXQ_Local,   AXI_L3_to_Sys,  St_SysChkI,   Data_MemWr,  RspAck,    BusIdle     );
      add_bus_req_act( `EVL_AXI_REQ_WRITE,  M,           AXQ_Local,   AXI_L3_to_Sys,  St_SysChkI,   Data_MemWr,  RspAck,    BusIdle     );
      add_bus_req_act( `EVL_AXI_REQ_READ,   I,           AXQ_Remote,  AXI_L3_to_Sys,  St_SysChkI,   Data_MemRd,  RspWData,  BusIdle     );
      add_bus_req_act( `EVL_AXI_REQ_WRITE,  I,           AXQ_Remote,  AXI_L3_to_Sys,  St_SysChkI,   Data_MemWr,  RspAck,    BusIdle     );

      if (evl_verif_comp_map::get_verif_comp(SHIRE_CACHE_RTL_AGENT, ~evl_agent_id_t'(0), get_shire_id()) == null) begin
         //
         // We are using a lite shire cache.
         //
         //              |-------------------------- Table Inputs --------------------------||---------------- Table Outputs -----------------|
         //               Cmd                  Model State  Qual              Source          Tag Action   Data Action  Bus Rsp    Bus Command
         //               -------------------  -----------  ----------------  --------------  -----------  -----------  ---------  -----------
         add_bus_req_act( `EVL_AXI_REQ_WRITE,  I,           AXQ_AtomicLocal,  AXI_L3_to_Sys,  St_SysChkI,  Data_None,   RspAck,    BusIdle     );
         add_bus_req_act( `EVL_AXI_REQ_WRITE,  E,           AXQ_AtomicLocal,  AXI_L3_to_Sys,  St_SysChkI,  Data_None,   RspAck,    BusIdle     );
         add_bus_req_act( `EVL_AXI_REQ_WRITE,  M,           AXQ_AtomicLocal,  AXI_L3_to_Sys,  St_SysChkI,  Data_None,   RspAck,    BusIdle     );
      end
      else begin
         //
         // We are using a real shire cache.
         //
         //              |--------------------------- Table Inputs ----------------------------||---------------- Table Outputs ----------------|
         //               Cmd                  Model State  Qual                 Source          Tag Action  Data Action  Bus Rsp    Bus Command
         //               -------------------  -----------  -------------------  --------------  ----------  -----------  ---------  -----------
         add_bus_req_act( `EVL_AXI_REQ_WRITE,  I,           AXQ_AtomicComplete,  AXI_L3_to_Sys,  St_Ignore,  Data_FwdWr,  RspNull,   BusWrite    );
         add_bus_req_act( `EVL_AXI_REQ_WRITE,  E,           AXQ_AtomicComplete,  AXI_L3_to_Sys,  St_Ignore,  Data_FwdWr,  RspNull,   BusWrite    );
         add_bus_req_act( `EVL_AXI_REQ_WRITE,  M,           AXQ_AtomicComplete,  AXI_L3_to_Sys,  St_Ignore,  Data_FwdWr,  RspNull,   BusWrite    );
      end

      //
      // Requests from the UC-to-L3 mesh AXI port
      //
      //              |-------------------------- Table Inputs --------------------------||--------------------- Table Outputs ----------------------|
      //               Cmd                  Model State  Qual               Source         Tag Action   Data Action         Bus Rsp    Bus Command
      //               -------------------  -----------  -----------------  -------------  -----------  ------------------  ---------  --------------
      add_bus_req_act( `EVL_AXI_REQ_READ,   I,           AXQ_Local,         AXI_UC_to_L3,  St_L3ChkE,   Data_MemRd,         RspWData,  BusIdle        );
      add_bus_req_act( `EVL_AXI_REQ_READ,   E,           AXQ_Local,         AXI_UC_to_L3,  St_Ignore,   Data_MemRd,         RspWData,  BusIdle        );
      add_bus_req_act( `EVL_AXI_REQ_READ,   M,           AXQ_Local,         AXI_UC_to_L3,  St_Ignore,   Data_MemRd,         RspWData,  BusIdle        );
      add_bus_req_act( `EVL_AXI_REQ_WRITE,  I,           AXQ_Local,         AXI_UC_to_L3,  St_L3ChkM,   Data_MemWr,         RspAck,    BusIdle        );
      add_bus_req_act( `EVL_AXI_REQ_WRITE,  E,           AXQ_Local,         AXI_UC_to_L3,  M,           Data_MemWr,         RspAck,    BusIdle        );
      add_bus_req_act( `EVL_AXI_REQ_WRITE,  M,           AXQ_Local,         AXI_UC_to_L3,  St_Ignore,   Data_MemWr,         RspAck,    BusIdle        );
      add_bus_req_act( `EVL_AXI_REQ_READ,   I,           AXQ_Remote,        AXI_UC_to_L3,  St_Ignore,   Data_None,          RspNull,   BusRead        );
      add_bus_req_act( `EVL_AXI_REQ_WRITE,  I,           AXQ_Remote,        AXI_UC_to_L3,  St_Ignore,   Data_FwdWr,         RspNull,   BusWrite       );
      add_bus_req_act( `EVL_AXI_REQ_WRITE,  I,           AXQ_AtomicLocal,   AXI_UC_to_L3,  St_L3ChkEM,  Data_FwdAtomicRsp,  RspAck,    BusWriteAtomic );
      add_bus_req_act( `EVL_AXI_REQ_WRITE,  E,           AXQ_AtomicLocal,   AXI_UC_to_L3,  M,           Data_FwdAtomicRsp,  RspAck,    BusWriteAtomic );
      add_bus_req_act( `EVL_AXI_REQ_WRITE,  M,           AXQ_AtomicLocal,   AXI_UC_to_L3,  St_Ignore,   Data_FwdAtomicRsp,  RspAck,    BusWriteAtomic );
      add_bus_req_act( `EVL_AXI_REQ_WRITE,  I,           AXQ_AtomicRemote,  AXI_UC_to_L3,  St_Ignore,   Data_FwdWr,         RspNull,   BusWrite       );

      //
      // Requests from the UC-to-system mesh AXI port
      //
      //              |-------------------------- Table Inputs --------------------------||--------------------- Table Outputs ----------------------|
      //               Cmd                  Model State  Qual              Source          Tag Action   Data Action         Bus Rsp    Bus Command
      //               -------------------  -----------  ----------------  --------------  -----------  ------------------  ---------  --------------
      add_bus_req_act( `EVL_AXI_REQ_READ,   I,           AXQ_Local,        AXI_UC_to_Sys,  St_SysChkI,  Data_MemRd,         RspWData,  BusIdle        );
      add_bus_req_act( `EVL_AXI_REQ_READ,   E,           AXQ_Local,        AXI_UC_to_Sys,  St_SysChkI,  Data_MemRd,         RspWData,  BusIdle        );
      add_bus_req_act( `EVL_AXI_REQ_READ,   M,           AXQ_Local,        AXI_UC_to_Sys,  St_SysChkI,  Data_MemRd,         RspWData,  BusIdle        );
      add_bus_req_act( `EVL_AXI_REQ_WRITE,  I,           AXQ_Local,        AXI_UC_to_Sys,  St_SysChkI,  Data_MemWr,         RspAck,    BusIdle        );
      add_bus_req_act( `EVL_AXI_REQ_WRITE,  E,           AXQ_Local,        AXI_UC_to_Sys,  St_SysChkI,  Data_MemWr,         RspAck,    BusIdle        );
      add_bus_req_act( `EVL_AXI_REQ_WRITE,  M,           AXQ_Local,        AXI_UC_to_Sys,  St_SysChkI,  Data_MemWr,         RspAck,    BusIdle        );
      add_bus_req_act( `EVL_AXI_REQ_READ,   I,           AXQ_Remote,       AXI_UC_to_Sys,  St_Ignore,   Data_None,          RspNull,   BusRead        );
      add_bus_req_act( `EVL_AXI_REQ_WRITE,  I,           AXQ_Remote,       AXI_UC_to_Sys,  St_Ignore,   Data_FwdWr,         RspNull,   BusWrite       );
      add_bus_req_act( `EVL_AXI_REQ_WRITE,  I,           AXQ_MsgLocal,     AXI_UC_to_Sys,  St_Ignore,   Data_None,          RspAck,    BusIdle        );
      add_bus_req_act( `EVL_AXI_REQ_WRITE,  I,           AXQ_MsgRemote,    AXI_UC_to_Sys,  St_Ignore,   Data_FwdWr,         RspNull,   BusWrite       );
      add_bus_req_act( `EVL_AXI_REQ_WRITE,  I,           AXQ_AtomicLocal,  AXI_UC_to_Sys,  St_SysChkI,  Data_FwdAtomicRsp,  RspAck,    BusWriteAtomic );
      add_bus_req_act( `EVL_AXI_REQ_WRITE,  E,           AXQ_AtomicLocal,  AXI_UC_to_Sys,  St_SysChkI,  Data_FwdAtomicRsp,  RspAck,    BusWriteAtomic );
      add_bus_req_act( `EVL_AXI_REQ_WRITE,  M,           AXQ_AtomicLocal,  AXI_UC_to_Sys,  St_SysChkI,  Data_FwdAtomicRsp,  RspAck,    BusWriteAtomic );

      // Actions to take when a response comes back:
      //
      //              |--------------- Table Inputs ----------------||--------- Table Outputs ---------|
      //               Cmd                  Model State  Qual         Tag Action  Data Action  Bus Rsp
      //               -------------------  -----------  -----------  ----------  -----------  --------
      add_bus_rsp_act( `EVL_AXI_REQ_READ,   I,           AXQ_Remote,  St_Ignore,  Data_FwdRd,  RspWData );
      add_bus_rsp_act( `EVL_AXI_REQ_WRITE,  I,           AXQ_Remote,  St_Ignore,  Data_FwdWr,  RspAck   );

      //
      // Default Requests
      //
      //              |------------------ Table Inputs ------------------||------------------- Table Outputs --------------------|
      //               Cmd             Model State  Qual       Source      Tag Action  Data Action  Bus Rsp   Bus Cmd   Err Code
      //               --------------  -----------  ---------  ----------  ----------  -----------  --------  --------  ---------
      add_bus_req_act( `EVL_Bus_Idle,  St_Ignore,   AXQ_None,  AXI_NoSrc,  St_Ignore,  Data_None,   RspNull,  BusIdle,  1         );

   endfunction : build_tables


   //-----------------------------------------------------------------------------------------------
   //
   // Function: address_is_mine
   //
   virtual function bit address_is_mine(input evl_paddr_t addr_in, input evl_axi_source_t axi_source, input evl_etlink_req_opcode_t opcode_in);
      if (axi_source inside { AXI_L2_to_L3, AXI_UC_to_L3 }) begin
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
      end
      if (axi_source == AXI_UC_to_Sys) begin
         if (opcode_in inside { ET_LINK_REQ_MsgSendData }) begin
            evl_shire_id_t shire_id;

            shire_id = evl_shire_id_t'(addr_in >> `EVL_MSG_SHIRE_ID_LSB);
            if ((shire_id == get_shire_id()) || (shire_id == ~evl_shire_id_t'(0))) begin
                return ~m_allow_l3_loopback;
            end
             return 1'b1;
         end
         else if ((addr_in >= evl_paddr_t'(40'h01_0000_0000)) && (addr_in <= evl_paddr_t'(40'h01_ffff_ffff))) begin
            evl_paddr_t addr    = addr_in & evl_paddr_t'(40'h01_3fc0_0000);
            evl_paddr_t my_addr = evl_paddr_t'(40'h1_0000_0000) | (evl_paddr_t'(m_shire_id) << 22);

            if ((addr == evl_paddr_t'(40'h01_3fc0_0000)) || (addr == my_addr)) begin
               return ~m_allow_l3_loopback;
            end
         end
      end
       return 1'b1;
   endfunction : address_is_mine


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_axi_source
   //
   // This function is used to determine the source AXI bus of the request.
   //
   virtual function evl_axi_source_t get_axi_source(input int req_port);
      if ((req_port >= m_l2_to_l3_first_rcv_port) && (req_port <= m_l2_to_l3_last_rcv_port)) begin
         return AXI_L2_to_L3;
      end
      if ((req_port >= m_l3_to_sys_first_rcv_port) && (req_port <= m_l3_to_sys_last_rcv_port)) begin
         return AXI_L3_to_Sys;
      end
      if ((req_port >= m_uc_to_l3_first_rcv_port) && (req_port <= m_uc_to_l3_last_rcv_port)) begin
         return AXI_UC_to_L3;
      end
      if ((req_port >= m_uc_to_sys_first_rcv_port) && (req_port <= m_uc_to_sys_last_rcv_port)) begin
         return AXI_UC_to_Sys;
      end
      return AXI_NoSrc;
   endfunction : get_axi_source


   //-----------------------------------------------------------------------------------------------
   //
   // Function: select_req_port
   //
   virtual function int select_req_port(input evl_req_desc req_desc, input evl_axi_source_t axi_source);
      int snd_req_port = req_desc.get_int_param(pk_tlm_req_port, 0);

      if (axi_source == AXI_L2_to_L3) begin
         snd_req_port = snd_req_port - m_l2_to_l3_first_rcv_port;
         if ((snd_req_port < 0) || (snd_req_port >= m_l3_loopback_snd_ports.size())) begin
            `dut_error(get_abstract_name(), $sformatf("TB_ISSUE: attempting to send a request to a non-existent loopback port"))
            return m_l3_loopback_snd_ports[0];
         end
         return m_l3_loopback_snd_ports[snd_req_port];
      end
      else if (axi_source == AXI_L3_to_Sys) begin
         if (req_desc.get_bus_qos() == `AXI_QOS_ATOMIC_RSP) begin
            return m_uc_sys_loopback_snd_ports[0];
         end
         return m_sys_loopback_snd_ports[0];
      end
      else if (axi_source == AXI_UC_to_L3) begin
         if (req_desc.get_bus_qos() == `AXI_QOS_ATOMIC_RSP) begin
            return m_uc_sys_loopback_snd_ports[0];
         end
         return m_uc_l3_loopback_snd_ports[0];
      end
      else if (axi_source == AXI_UC_to_Sys) begin
         return m_uc_sys_loopback_snd_ports[0];
      end
      return 0;
   endfunction : select_req_port


   //-----------------------------------------------------------------------------------------------
   //
   // Function: select_rsp_port
   //
   virtual function int select_rsp_port(input evl_req_desc req_desc);
      return req_desc.get_int_param(pk_tlm_req_port, 0);
   endfunction : select_rsp_port


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_req_desc_port
   //
   virtual function uvm_analysis_port#(evl_req_desc) get_req_desc_port(input string port_name      = "unknown",
                                                                       input int    port_number    = 0,
                                                                       input int    subport_number = 0);
      if (port_name == "SHIRE_L2TOL3_MESH_AXI") begin
         if ((port_number < 0) || (port_number >= m_l2_to_l3_rcv_ports.size())) begin
            return null;
         end
         return m_snd_rsp_ports[m_l2_to_l3_rcv_ports[port_number]];
      end
      if (port_name == "SHIRE_L3TOSYS_MESH_AXI") begin
         if ((port_number < 0) || (port_number >= m_l3_to_sys_rcv_ports.size())) begin
            return null;
         end
         return m_snd_rsp_ports[m_l3_to_sys_rcv_ports[port_number]];
      end
      if (port_name == "SHIRE_UCTOL3_MESH_AXI") begin
         if ((port_number < 0) || (port_number >= m_uc_to_l3_rcv_ports.size())) begin
            return null;
         end
         return m_snd_rsp_ports[m_uc_to_l3_rcv_ports[port_number]];
      end
      if (port_name == "SHIRE_UCTOSYS_MESH_AXI") begin
         if ((port_number < 0) || (port_number >= m_uc_to_sys_rcv_ports.size())) begin
            return null;
         end
         return m_snd_rsp_ports[m_uc_to_sys_rcv_ports[port_number]];
      end
      if (port_name == "SHIRE_L3_LOOPBACK") begin
         if ((port_number < 0) || (port_number >= m_l3_loopback_snd_ports.size())) begin
            return null;
         end
         return m_snd_req_ports[m_l3_loopback_snd_ports[port_number]];
      end
      if (port_name == "SHIRE_SYS_LOOPBACK") begin
         if ((port_number < 0) || (port_number >= m_sys_loopback_snd_ports.size())) begin
            return null;
         end
         return m_snd_req_ports[m_sys_loopback_snd_ports[port_number]];
      end
      if (port_name == "SHIRE_UC_L3_LOOPBACK") begin
         if ((port_number < 0) || (port_number >= m_uc_l3_loopback_snd_ports.size())) begin
            return null;
         end
         return m_snd_req_ports[m_uc_l3_loopback_snd_ports[port_number]];
      end
      if (port_name == "SHIRE_UC_SYS_LOOPBACK") begin
         if ((port_number < 0) || (port_number >= m_uc_sys_loopback_snd_ports.size())) begin
            return null;
         end
         return m_snd_req_ports[m_uc_sys_loopback_snd_ports[port_number]];
      end
      return null;
   endfunction : get_req_desc_port


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_req_desc_export
   //
   virtual function uvm_analysis_export#(evl_req_desc) get_req_desc_export(input string port_name      = "unknown",
                                                                           input int    port_number    = 0,
                                                                           input int    subport_number = 0);
      if (port_name == "SHIRE_L2TOL3_MESH_AXI") begin
         if ((port_number < 0) || (port_number >= m_l2_to_l3_rcv_ports.size())) begin
            return null;
         end
         return m_obs_req_exports[m_l2_to_l3_rcv_ports[port_number]];
      end
      if (port_name == "SHIRE_L3TOSYS_MESH_AXI") begin
         if ((port_number < 0) || (port_number >= m_l3_to_sys_rcv_ports.size())) begin
            return null;
         end
         return m_obs_req_exports[m_l3_to_sys_rcv_ports[port_number]];
      end
      if (port_name == "SHIRE_UCTOL3_MESH_AXI") begin
         if ((port_number < 0) || (port_number >= m_uc_to_l3_rcv_ports.size())) begin
            return null;
         end
         return m_obs_req_exports[m_uc_to_l3_rcv_ports[port_number]];
      end
      if (port_name == "SHIRE_UCTOSYS_MESH_AXI") begin
         if ((port_number < 0) || (port_number >= m_uc_to_sys_rcv_ports.size())) begin
            return null;
         end
         return m_obs_req_exports[m_uc_to_sys_rcv_ports[port_number]];
      end
      if (port_name == "SHIRE_L3_LOOPBACK") begin
         if ((port_number < 0) || (port_number >= m_l3_loopback_snd_ports.size())) begin
            return null;
         end
         return m_rcv_rsp_exports[m_l3_loopback_snd_ports[port_number]];
      end
      if (port_name == "SHIRE_SYS_LOOPBACK") begin
         if ((port_number < 0) || (port_number >= m_sys_loopback_snd_ports.size())) begin
            return null;
         end
         return m_rcv_rsp_exports[m_sys_loopback_snd_ports[port_number]];
      end
      if (port_name == "SHIRE_UC_L3_LOOPBACK") begin
         if ((port_number < 0) || (port_number >= m_uc_l3_loopback_snd_ports.size())) begin
            return null;
         end
         return m_rcv_rsp_exports[m_uc_l3_loopback_snd_ports[port_number]];
      end
      if (port_name == "SHIRE_UC_SYS_LOOPBACK") begin
         if ((port_number < 0) || (port_number >= m_uc_sys_loopback_snd_ports.size())) begin
            return null;
         end
         return m_rcv_rsp_exports[m_uc_sys_loopback_snd_ports[port_number]];
      end
      return null;
   endfunction : get_req_desc_export


   //-----------------------------------------------------------------------------------------------
   //
   // Function: connect_my_bus_ports
   //
   // This function is used to connect all of the ports that go between this stimulus agent and its
   // bus agent.  Because the evl_uc_l3mesh_slave_agent_axi stimulus agent supports loopbacks, we
   // need to redefine this function to only connect the response side to the bus agents.
   //
   virtual function void connect_my_bus_ports(input evl_verif_comp bus_verif_comp,
                                              input string         port_name      = "",
                                              input int            port_number    = 0,
                                              input int            subport_number = 0);
      uvm_analysis_port   #(evl_req_desc) bus_port;
      uvm_analysis_export #(evl_req_desc) bus_export;
      uvm_analysis_port   #(evl_req_desc) stim_port;
      uvm_analysis_export #(evl_req_desc) stim_export;

      bus_export = bus_verif_comp.get_req_desc_export("sa_snd_rsp", port_number);
      stim_port  = get_req_desc_port(port_name, port_number);
      if ((bus_export != null) && (stim_port != null)) begin
         `evl_log(UVM_MEDIUM, VMOD_CONFIG, get_abstract_name(), $sformatf("connecting %0s -> %0s", stim_port.get_full_name(), bus_export.get_full_name()))
         stim_port.connect(bus_export);
      end

      bus_port    = bus_verif_comp.get_req_desc_port("m_obs_req_port", port_number);
      stim_export = get_req_desc_export(port_name, port_number);
      if ((bus_port != null) && (stim_export != null)) begin
         `evl_log(UVM_MEDIUM, VMOD_CONFIG, get_abstract_name(), $sformatf("connecting %0s -> %0s", bus_port.get_full_name(), stim_export.get_full_name()))
         bus_port.connect(stim_export);
      end
   endfunction : connect_my_bus_ports


   //-----------------------------------------------------------------------------------------------
   //
   // Function: build_phase
   //
   virtual function void build_phase(uvm_phase phase);
      int cur_snd_port = 0;
      int cur_rcv_port = 0;

      super.build_phase(phase);

      //
      // L2-to-L3 slave port connections
      //
      m_l2_to_l3_first_rcv_port = m_snd_rsp_ports.size();
      for (int ii = 0; ii < m_num_l3_ports; ii++) begin
         m_l2_to_l3_rcv_ports.push_back(m_snd_rsp_ports.size());
         m_snd_rsp_ports.push_back(null);
         m_obs_req_exports.push_back(null);
         m_obs_req_fifos.push_back(null);
         m_obs_req_mb_map.push_back(0);
         m_snd_rsp_ports[cur_rcv_port]   = new($sformatf("l2_to_l3_snd_rsp_port_%0d", ii), this);
         m_obs_req_exports[cur_rcv_port] = new($sformatf("l2_to_l3_obs_req_export_%0d", ii), this);
         m_obs_req_fifos[cur_rcv_port]   = new($sformatf("l2_to_l3_obs_req_fifo_%0d", ii), this);
         cur_rcv_port++;
      end
      m_l2_to_l3_last_rcv_port = m_snd_rsp_ports.size() - 1;

      //
      // L3-to-sys slave port connections
      //
      m_l3_to_sys_first_rcv_port = m_snd_rsp_ports.size();
      m_l3_to_sys_rcv_ports.push_back(m_snd_rsp_ports.size());
      m_snd_rsp_ports.push_back(null);
      m_obs_req_exports.push_back(null);
      m_obs_req_fifos.push_back(null);
      m_obs_req_mb_map.push_back(1);
      m_snd_rsp_ports[cur_rcv_port]   = new($sformatf("l3_to_sys_snd_rsp_port"), this);
      m_obs_req_exports[cur_rcv_port] = new($sformatf("l3_to_sys_obs_req_export"), this);
      m_obs_req_fifos[cur_rcv_port]   = new($sformatf("l3_to_sys_obs_req_fifo"), this);
      cur_rcv_port++;
      m_l3_to_sys_last_rcv_port = m_snd_rsp_ports.size() - 1;

      //
      // UC-to-L3 slave port connections
      //
      m_uc_to_l3_first_rcv_port = m_snd_rsp_ports.size();
      m_uc_to_l3_rcv_ports.push_back(m_snd_rsp_ports.size());
      m_snd_rsp_ports.push_back(null);
      m_obs_req_exports.push_back(null);
      m_obs_req_fifos.push_back(null);
      m_obs_req_mb_map.push_back(2);
      m_snd_rsp_ports[cur_rcv_port]   = new($sformatf("uc_to_l3_snd_rsp_port"), this);
      m_obs_req_exports[cur_rcv_port] = new($sformatf("uc_to_l3_obs_req_export"), this);
      m_obs_req_fifos[cur_rcv_port]   = new($sformatf("uc_to_l3_obs_req_fifo"), this);
      cur_rcv_port++;
      m_uc_to_l3_last_rcv_port = m_snd_rsp_ports.size() - 1;

      //
      // UC-to-sys slave port connections
      //
      m_uc_to_sys_first_rcv_port = m_snd_rsp_ports.size();
      m_uc_to_sys_rcv_ports.push_back(m_snd_rsp_ports.size());
      m_snd_rsp_ports.push_back(null);
      m_obs_req_exports.push_back(null);
      m_obs_req_fifos.push_back(null);
      m_obs_req_mb_map.push_back(3);
      m_snd_rsp_ports[cur_rcv_port]   = new($sformatf("uc_to_sys_snd_rsp_port"), this);
      m_obs_req_exports[cur_rcv_port] = new($sformatf("uc_to_sys_obs_req_export"), this);
      m_obs_req_fifos[cur_rcv_port]   = new($sformatf("uc_to_sys_obs_req_fifo"), this);
      cur_rcv_port++;
      m_uc_to_sys_last_rcv_port = m_snd_rsp_ports.size() - 1;

      //
      // L3-to-sys loopback port connections
      //
      for (int ii = 0; ii < m_num_l3_ports; ii++) begin
         m_l3_loopback_snd_ports.push_back(m_snd_req_ports.size());
         m_snd_req_ports.push_back(null);
         m_rcv_rsp_exports.push_back(null);
         m_rcv_rsp_fifos.push_back(null);
         m_snd_req_ports[cur_snd_port]   = new($sformatf("l3_loopback_snd_req_port_%0d", cur_snd_port), this);
         m_rcv_rsp_exports[cur_snd_port] = new($sformatf("l3_loopback_rcv_rsp_export_%0d", cur_snd_port), this);
         m_rcv_rsp_fifos[cur_snd_port]   = new($sformatf("l3_loopback_rcv_rsp_fifo_%0d", cur_snd_port), this);
         cur_snd_port++;
      end

      //
      // Sys loopback port connections
      //
      m_sys_loopback_snd_ports.push_back(m_snd_req_ports.size());
      m_snd_req_ports.push_back(null);
      m_rcv_rsp_exports.push_back(null);
      m_rcv_rsp_fifos.push_back(null);
      m_snd_req_ports[cur_snd_port]   = new($sformatf("sys_loopback_snd_req_port"), this);
      m_rcv_rsp_exports[cur_snd_port] = new($sformatf("sys_loopback_rcv_rsp_export"), this);
      m_rcv_rsp_fifos[cur_snd_port]   = new($sformatf("sys_loopback_rcv_rsp_fifo"), this);
      cur_snd_port++;

      //
      // UC L3 loopback port connections
      //
      m_uc_l3_loopback_snd_ports.push_back(m_snd_req_ports.size());
      m_snd_req_ports.push_back(null);
      m_rcv_rsp_exports.push_back(null);
      m_rcv_rsp_fifos.push_back(null);
      m_snd_req_ports[cur_snd_port]   = new($sformatf("uc_l3_loopback_snd_req_port"), this);
      m_rcv_rsp_exports[cur_snd_port] = new($sformatf("uc_l3_loopback_rcv_rsp_export"), this);
      m_rcv_rsp_fifos[cur_snd_port]   = new($sformatf("uc_l3_loopback_rcv_rsp_fifo"), this);
      cur_snd_port++;

      //
      // UC sys loopback port connections
      //
      m_uc_sys_loopback_snd_ports.push_back(m_snd_req_ports.size());
      m_snd_req_ports.push_back(null);
      m_rcv_rsp_exports.push_back(null);
      m_rcv_rsp_fifos.push_back(null);
      m_snd_req_ports[cur_snd_port]   = new($sformatf("uc_sys_loopback_snd_req_port"), this);
      m_rcv_rsp_exports[cur_snd_port] = new($sformatf("uc_sys_loopback_rcv_rsp_export"), this);
      m_rcv_rsp_fifos[cur_snd_port]   = new($sformatf("uc_sys_loopback_rcv_rsp_fifo"), this);
      cur_snd_port++;
   endfunction : build_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string name_in = "evl_minion_shire_slave_agent_axi", input uvm_component parent_in = null);
      evl_axi_bridge_id_t bridge_id = evl_axi_bridge_id_t'(16);
      evl_axi_reqq_id_t   reqq_id   = evl_axi_reqq_id_t'(0);
      evl_axi_bank_id_t   bank_id   = evl_axi_bank_id_t'(0);
      int                 value;

      super.new(name_in, parent_in, MESH_SLAVE_STIM_AGENT);
      if (`et_value_plusargs("allow_l3_loopback=%d", value) == 0) begin
         value = 1;
      end
      if (`et_value_plusargs("check_l3_ways=%d", m_check_l3_ways) == 0) begin
         m_check_l3_ways = 0;
      end
      m_allow_l3_loopback    = (value != 0) ? 1'b1 : 1'b0;
      m_memory_agent         = 1'b1;
      m_allow_mem_load       = 1'b1;
      bridge_id              = evl_axi_bridge_id_t'(8);
      bank_id                = evl_axi_bank_id_t'(0);
      reqq_id                = evl_axi_reqq_id_t'(0);
      m_bus_req_fwd_id       = evl_bus_req_id_t'({ bridge_id, bank_id, reqq_id });
      bridge_id              = evl_axi_bridge_id_t'(15);
      bank_id                = ~evl_axi_bank_id_t'(0);
      reqq_id                = ~evl_axi_reqq_id_t'(0);
      m_bus_req_fwd_id_limit = evl_bus_req_id_t'({ bridge_id, bank_id, reqq_id });
      set_abstract_name("NOC_MESH(STIM)");
      add_agent_alias(MEM_STIM_AGENT);
      add_agent_alias(AXI_SLAVE_STIM_AGENT);
      add_agent_alias(MEM_AGENT);
      set_active_slave();
   endfunction : new

endclass : evl_minion_shire_slave_agent_axi

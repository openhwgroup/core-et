//--------------------------------------------------------------------------------------------------
//
// Class: evl_maxion_shire_slave_agent_axi
//
// This class is an extension of evl_stim_agent_axi to define a slave stimulus agent that acts like
// NoC on the L2-to-L3, L3-to-sys, UC-to-L3, and UC-to-sys AXI ports.
//
class evl_maxion_shire_slave_agent_axi extends evl_stim_agent_axi;

   `uvm_component_utils(evl_maxion_shire_slave_agent_axi)


   //-----------------------------------------------------------------------------------------------
   //
   // Member Variables and Objects
   //
   bit m_allow_l3_loopback  = 1'b1;
   bit m_check_sys_requests = 1'b0;
   bit m_have_scratchpad    = 1'b0;
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
               return ~(m_allow_l3_loopback & m_have_scratchpad);
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
         if ((addr_in >= evl_paddr_t'(40'h01_0000_0000)) && (addr_in <= evl_paddr_t'(40'h01_ffff_ffff))) begin
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
   // Task: post_process_received_request()
   //
   // This task will get called from process_received_request() in order to post-process the request
   // descriptor (e.g. bus_error).
   //
   virtual function void post_process_received_request(input evl_req_desc req_desc);
      if (m_axi_mem_map_err == 1'b1) begin
         bit         is_bus_err = 1'b0;
         evl_paddr_t paddr      = req_desc.get_paddr();

         if (addr_in_region(pk_scp_region, paddr)) begin
            //
            // If the scratchpad offset is 2560K or greater, signal a decode error.
            //
            if (paddr[22:0] >= 23'h280000) begin
               is_bus_err = 1'b1;
            end
         end
         else if (addr_in_region(pk_esr_region, paddr)) begin
            //
            // PRM-19: 2.4 The ESR Region (4G to 8G)
            //
            // If the ESR address is not for shires 0-33, memory shires 0-7, the IO shire, or the
            // PCI shire, signal a decode error.
            //
            if ((paddr[29:22] >= 0) && (paddr[29:22] < 34)) begin
               is_bus_err = 1'b0;
            end
            else if ((paddr[29:22] >= 232) && (paddr[29:22] <= 239)) begin
               is_bus_err = 1'b0;
            end
            else if (paddr[29:22] == 253) begin
               is_bus_err = 1'b0;
            end
            else if (paddr[29:22] == 254) begin
               is_bus_err = 1'b0;
            end
            else if (paddr[29:22] == 255) begin
               is_bus_err = 1'b0;
            end
            else begin
               is_bus_err = 1'b1;
            end
         end
         else if (addr_in_region(pk_mram_region, paddr)) begin
            //
            // If the DDR address is not for the L3 16GB space (0x8000000000-0x83ffffffff) or for
            // the system 16GB space (0xc000000000-0xc3ffffffff), signal a decode error.
            //
            is_bus_err = (paddr[39:32] >= 8'hc4) ? 1'b1 :
                         (paddr[39:32] >= 8'hc0) ? 1'b0 :
                         (paddr[39:32] >= 8'h84) ? 1'b1 :
                         (paddr[39:32] >= 8'h80) ? 1'b0 : 1'b1;
         end
         else if ( ((paddr >= evl_paddr_t'(40'h0000001000)) && (paddr <= evl_paddr_t'(40'h0000002fff))) || // within the Maxion region
                   ((paddr >= evl_paddr_t'(40'h0000004000)) && (paddr <= evl_paddr_t'(40'h000000ffff))) || // within the Maxion region
                   ((paddr >= evl_paddr_t'(40'h0000012000)) && (paddr <= evl_paddr_t'(40'h00000fffff))) || // within the Maxion region
                   ((paddr >= evl_paddr_t'(40'h0000108000)) && (paddr <= evl_paddr_t'(40'h00001fffff))) || // within the Maxion region
                   ((paddr >= evl_paddr_t'(40'h0000204000)) && (paddr <= evl_paddr_t'(40'h00002fffff))) || // within the Maxion region
                   ((paddr >= evl_paddr_t'(40'h0000304000)) && (paddr <= evl_paddr_t'(40'h0001ffffff))) || // within the Maxion region
                   ((paddr >= evl_paddr_t'(40'h0002010000)) && (paddr <= evl_paddr_t'(40'h000fffffff))) || // within the Maxion region
                   ((paddr >= evl_paddr_t'(40'h001200b000)) && (paddr <= evl_paddr_t'(40'h0013ffffff))) || // undefined PU space (> PU_DMA_RELOC, < PU_Static)
                   ((paddr >= evl_paddr_t'(40'h0014040000)) && (paddr <= evl_paddr_t'(40'h0017ffffff))) || // undefined PU space (> PU_Static, < PU_USB0)
                   ((paddr >= evl_paddr_t'(40'h0018083000)) && (paddr <= evl_paddr_t'(40'h001fffffff))) || // undefined PU space (> PU_DMA_CFG, < PU_SRAM_MM_MX)
                   ((paddr >= evl_paddr_t'(40'h0020040000)) && (paddr <= evl_paddr_t'(40'h007fffffff))) || // undefined PU space (> PU_SRAM_HI, < PU_MBOX_MX_SP)
                   ((paddr >= evl_paddr_t'(40'h003000c000)) && (paddr <= evl_paddr_t'(40'h003fffffff))) || // undefined PU space (> PU_TRG_PCIE_SP)
                   ((paddr >= evl_paddr_t'(40'h0040000000)) && (paddr <= evl_paddr_t'(40'h007fffffff))) ) begin // SP region
            is_bus_err = 1'b1;
         end

         if (is_bus_err == 1'b1) begin
            req_desc.set_abstract_rsp_type(RspDecErr);
            req_desc.set_bus_rsp_qword_en(4'b0000);
         end
      end
   endfunction : post_process_received_request


   //-----------------------------------------------------------------------------------------------
   //
   // Function: perform_load_mem
   //
   virtual function void perform_load_mem(input string file_name);
      string line;

      if (file_name == "") begin
         return;
      end
      super.perform_load_mem(file_name);
      if (`et_value_plusargs("maxion_config_dword=%s", line) != 0) begin
         evl_line_desc line_desc;
         evl_dword_t   config_dword;

         line = line.toupper();
         if (line.getc(0) == byte'("0")) begin
            if (line.getc(1) == byte'("D")) begin
               line = line.substr(2, line.len() - 1);
               config_dword = evl_dword_t'(line.atoi());
            end
            else if (line.getc(1) == byte'("X")) begin
               line = line.substr(2, line.len() - 1);
               config_dword = evl_dword_t'(line.atohex());
            end
            else if (line.getc(1) == byte'("B")) begin
               line = line.substr(2, line.len() - 1);
               config_dword = evl_dword_t'(line.atobin());
            end
            else if (line.getc(1) == byte'("O")) begin
               line = line.substr(2, line.len() - 1);
               config_dword = evl_dword_t'(line.atooct());
            end
            else begin
               line = line.substr(2, line.len() - 1);
               config_dword = evl_dword_t'(line.atooct());
            end
         end
         else begin
            config_dword = evl_dword_t'(line.atoi());
         end

         // Scratch address 0 is used for the configuration address.
         line_desc = m_cache.get_line_desc(evl_paddr_t'(40'h0080000000));
         if (line_desc != null) begin
            // Create zero-filled data if the line does not exist.
            if (line_desc.all_bytes_are_valid() == 1'b0) begin
               line_desc.zero_all_line_data();
            end
            if (line_desc.get_dword(0) != config_dword) begin
               `evl_log(UVM_LOW, VMOD_LOAD_MEM, get_abstract_name(), $sformatf("changing data at address 0x0080000000 from 0x%x to 0x%x", line_desc.get_dword(0), config_dword))
               line_desc.set_dword(0, config_dword);
            end
         end
      end
   endfunction : perform_load_mem


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_req_desc_port
   //
   virtual function uvm_analysis_port#(evl_req_desc) get_req_desc_port(input string port_name      = "unknown",
                                                                       input int    port_number    = 0,
                                                                       input int    subport_number = 0);
      if (port_name == "MAXSHIRE_TOL3_AXI") begin
         if ((port_number < 0) || (port_number >= m_l2_to_l3_rcv_ports.size())) begin
            return null;
         end
         return m_snd_rsp_ports[m_l2_to_l3_rcv_ports[port_number]];
      end
      if (port_name == "MAXSHIRE_TOSYS_AXI") begin
         if ((port_number < 0) || (port_number >= m_l3_to_sys_rcv_ports.size())) begin
            return null;
         end
         return m_snd_rsp_ports[m_l3_to_sys_rcv_ports[port_number]];
      end
      if (port_name == "MAXSHIRE_UCTOL3_AXI") begin
         if ((port_number < 0) || (port_number >= m_uc_to_l3_rcv_ports.size())) begin
            return null;
         end
         return m_snd_rsp_ports[m_uc_to_l3_rcv_ports[port_number]];
      end
      if (port_name == "MAXSHIRE_UCTOSYS_AXI") begin
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
      if (port_name == "MAXSHIRE_TOL3_AXI") begin
         if ((port_number < 0) || (port_number >= m_l2_to_l3_rcv_ports.size())) begin
            return null;
         end
         return m_obs_req_exports[m_l2_to_l3_rcv_ports[port_number]];
      end
      if (port_name == "MAXSHIRE_TOSYS_AXI") begin
         if ((port_number < 0) || (port_number >= m_l3_to_sys_rcv_ports.size())) begin
            return null;
         end
         return m_obs_req_exports[m_l3_to_sys_rcv_ports[port_number]];
      end
      if (port_name == "MAXSHIRE_UCTOL3_AXI") begin
         if ((port_number < 0) || (port_number >= m_uc_to_l3_rcv_ports.size())) begin
            return null;
         end
         return m_obs_req_exports[m_uc_to_l3_rcv_ports[port_number]];
      end
      if (port_name == "MAXSHIRE_UCTOSYS_AXI") begin
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
   function new(input string name_in = "evl_maxion_shire_slave_agent_axi", input uvm_component parent_in = null);
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
      if (`et_test_plusargs("axi_mem_map_err=") != 0) begin
         if (`et_value_plusargs("axi_mem_map_err=%d", value) != 0) begin
            m_axi_mem_map_err = (value == 0) ? 1'b0 : 1'b1;
         end
      end
      else begin
         m_axi_mem_map_err = 1'b1;
      end
      m_allow_l3_loopback        = (value != 0) ? 1'b1 : 1'b0;
      m_memory_agent             = 1'b1;
      m_allow_mem_load           = 1'b1;
      bridge_id                  = evl_axi_bridge_id_t'(14);
      bank_id                    = evl_axi_bank_id_t'(0);
      reqq_id                    = evl_axi_reqq_id_t'(0);
      m_bus_req_fwd_id           = evl_bus_req_id_t'({ bridge_id, bank_id, reqq_id });
      bridge_id                  = evl_axi_bridge_id_t'(15);
      bank_id                    = ~evl_axi_bank_id_t'(0);
      reqq_id                    = ~evl_axi_reqq_id_t'(0);
      m_bus_req_fwd_id_limit     = evl_bus_req_id_t'({ bridge_id, bank_id, reqq_id });
      bridge_id                  = evl_axi_bridge_id_t'(0);
      bank_id                    = ~evl_axi_bank_id_t'(0);
      reqq_id                    = ~evl_axi_reqq_id_t'(0);
      m_atomic_rsp_bus_req_mask  = evl_bus_req_id_t'({ bridge_id, bank_id, reqq_id });
      bridge_id                  = evl_axi_bridge_id_t'(10);
      bank_id                    = evl_axi_bank_id_t'(0);
      reqq_id                    = evl_axi_reqq_id_t'(0);
      m_atomic_rsp_bus_req_base  = evl_bus_req_id_t'({ bridge_id, bank_id, reqq_id });
      bridge_id                  = ~evl_axi_bridge_id_t'(0);
      bank_id                    = ~evl_axi_bank_id_t'(0);
      reqq_id                    = ~evl_axi_reqq_id_t'(0);
      m_atomic_rsp_bus_req_limit = evl_bus_req_id_t'(4);
      m_atomic_rsp_bus_req_shift = $bits(bank_id) + $bits(reqq_id);

      set_abstract_name("NOC_MESH(STIM)");
      add_agent_alias(MEM_STIM_AGENT);
      add_agent_alias(AXI_SLAVE_STIM_AGENT);
      add_agent_alias(MEM_AGENT);
      set_active_slave();
   endfunction : new

endclass : evl_maxion_shire_slave_agent_axi

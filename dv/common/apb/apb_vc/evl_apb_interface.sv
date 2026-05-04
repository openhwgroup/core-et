//--------------------------------------------------------------------------------------------------
//
//
// Interface: evl_apb_interface
//
`include "rtl/inc/soc.vh"

`include "dv/common/apb/apb_vc/evl_apb_pkg.sv"

`ifndef ET_MONITOR_IFCS
   `ifdef EVL_SIMULATION
      `define ET_MONITOR_IFCS 1
   `else
      `ifdef ET_IP_SIMULATION
         `define ET_MONITOR_IFCS 1
      `endif
   `endif
`endif

`ifdef EVL_SIMULATION
   `ifdef ET_INCLUDE_USR_PORT
      `define ET_APB_INCLUDE_USR_PORT
   `endif
`endif

module evl_apb_interface #(parameter int    SHIRE_ID      = 0,
                           parameter int    AGENT_ID      = 0,
                           parameter string ABSTRACT_NAME = "",
                           parameter int    ADDR_WIDTH    = 40,
                           parameter int    DATA_WIDTH    = 64,
                           parameter int    SKIP_LEVELS[] = { 0 }) (
      input wire                      isolate,
      input wire                      pclk,
      input wire                      presetn,
      inout wire [(ADDR_WIDTH - 1):0] paddr,
      inout wire [(DATA_WIDTH - 1):0] pwdata,
      inout wire                      pwrite,
      inout wire                      psel,
      inout wire                      penable,
      inout wire                      pready,
      inout wire [(DATA_WIDTH - 1):0] prdata,
      inout wire                      pslverr
   );

   `ifdef ET_MONITOR_IFCS // ----------------------------------------------------------------------{

      import evl_base_pkg::*;
      import evl_apb_types_pkg::*;
      import evl_apb_pkg::*;


      reg                      presetn_mon;

      reg [(ADDR_WIDTH - 1):0] paddr_drv;
      reg [(DATA_WIDTH - 1):0] pwdata_drv;
      reg                      pwrite_drv;
      reg                      psel_drv;
      reg                      penable_drv;
      reg                      pready_drv;
      reg [(DATA_WIDTH - 1):0] prdata_drv;
      reg                      pslverr_drv;

      evl_apb_req_s            apb_req_mon;
      evl_apb_rsp_s            apb_rsp_mon;

      evl_apb_req_s            apb_req_drv;
      evl_apb_rsp_s            apb_rsp_drv;

      reg                      req_to_send;
      reg                      rsp_to_send;
      bit                      drive_req;
      bit                      drive_rsp;
      wire                     have_req;
      wire                     have_rsp;
      reg                      have_req_mon;

      int                      m_agent_id;
      bit                      m_power_off = 1'b0;
      bit                      m_power_on  = 1'b0;

      assign paddr    = paddr_drv;
      assign pwdata   = pwdata_drv;
      assign pwrite   = pwrite_drv;
      assign psel     = psel_drv;
      assign penable  = penable_drv;
      assign pready   = pready_drv;
      assign prdata   = prdata_drv;
      assign pslverr  = pslverr_drv;

      assign have_req = psel & (~have_req_mon);
      assign have_rsp = psel & penable & pready;


      `ifdef EVL_SIMULATION
         bit              m_register_me = evl_rtl_instance_map::add_instance_info($sformatf("%m"), APB_BUS_AGENT, AGENT_ID, "evl_apb_verif_comp", "", "", "", { SKIP_LEVELS });
         evl_apb_rtl_port m_rtl_port    = new($sformatf("%m"), AGENT_ID, { SKIP_LEVELS });

         `ifdef ET_APB_INCLUDE_USR_PORT
            usr_apb_rtl_port m_rtl_port2 = new($sformatf("%m"), AGENT_ID);
         `endif // ifdef ET_APB_INCLUDE_USR_PORT
      `else // ifdef EVL_SIMULATION
         usr_apb_rtl_port m_rtl_port = new(ABSTRACT_NAME, AGENT_ID);
      `endif // !ifdef EVL_SIMULATION


      //--------------------------------------------------------------------------------------------
      //
      // Monitor Loop
      //
      initial begin
         paddr_drv    = {ADDR_WIDTH{1'bz}};
         pwdata_drv   = {DATA_WIDTH{1'bz}};
         pwrite_drv   = 1'bz;
         psel_drv     = 1'bz;
         penable_drv  = 1'bz;
         pready_drv   = 1'bz;
         prdata_drv   = {DATA_WIDTH{1'bz}};
         pslverr_drv  = 1'bz;
         have_req_mon = 1'b0;
         m_agent_id   = AGENT_ID;
         if (ABSTRACT_NAME != "") begin
            m_rtl_port.set_abstract_name(ABSTRACT_NAME);
         end
         m_rtl_port.set_rtl_name($sformatf("%m"));
         m_rtl_port.set_bus_data_width(DATA_WIDTH / 8);
         `ifdef ET_APB_INCLUDE_USR_PORT
            if (ABSTRACT_NAME != "") begin
               m_rtl_port2.set_abstract_name(ABSTRACT_NAME);
            end
            m_rtl_port2.set_rtl_name($sformatf("%m"));
            m_rtl_port2.set_bus_data_width(DATA_WIDTH / 8);
         `endif // ifdef ET_APB_INCLUDE_USR_PORT
         fork
            begin
               #1ns;
               forever begin
                  #0 wait ((isolate === 1'b1) && (m_power_off === 1'b0));
                  m_rtl_port.power_off();
                  m_power_off = 1'b1;
                  m_power_on  = 1'b0;
                  #0 wait (isolate !== 1'b1);
               end
            end
            begin
               #1ns;
               forever begin
                  #0 wait ((isolate === 1'b0) && (m_power_on === 1'b0));
                  m_rtl_port.power_on();
                  m_power_on  = 1'b1;
                  m_power_off = 1'b0;
                  #0 wait (isolate !== 1'b0);
               end
            end
            begin
               forever @(posedge pclk) begin
                  if (presetn === 1'b0) begin
                     if (presetn_mon !== presetn) begin
                        m_rtl_port.assert_reset();
                        `ifdef ET_APB_INCLUDE_USR_PORT
                           m_rtl_port2.assert_reset();
                        `endif // ifdef ET_APB_INCLUDE_USR_PORT
                     end
                  end
                  else if (presetn === 1'b1) begin
                     if (presetn_mon === 1'b0) begin
                        m_rtl_port.deassert_reset();
                        `ifdef ET_APB_INCLUDE_USR_PORT
                           m_rtl_port2.deassert_reset();
                        `endif // ifdef ET_APB_INCLUDE_USR_PORT
                     end
                     if (isolate !== 1'b1) begin
                        if (have_req === 1'b1) begin
                           apb_req_mon.pwrite = pwrite;
                           apb_req_mon.paddr  = evl_paddr_t'(paddr);
                           apb_req_mon.pwdata = evl_dword_t'(pwdata);
                           m_rtl_port.capture_req(apb_req_mon);
                           `ifdef ET_APB_INCLUDE_USR_PORT
                              m_rtl_port2.capture_req(apb_req_mon);
                           `endif // ifdef ET_APB_INCLUDE_USR_PORT
                        end
                        if (have_rsp === 1'b1) begin
                           apb_rsp_mon.prdata  = evl_dword_t'(prdata);
                           apb_rsp_mon.pslverr = pslverr;
                           m_rtl_port.capture_rsp(apb_rsp_mon);
                           `ifdef ET_APB_INCLUDE_USR_PORT
                              m_rtl_port2.capture_rsp(apb_rsp_mon);
                           `endif // ifdef ET_APB_INCLUDE_USR_PORT
                        end
                     end
                  end
                  presetn_mon  <= presetn;
                  have_req_mon <= presetn & (have_req | (~have_rsp & have_req_mon)) & ~isolate;
                  m_rtl_port.ping_clock();
                  `ifdef ET_APB_INCLUDE_USR_PORT
                     m_rtl_port2.ping_clock();
                  `endif // ifdef ET_APB_INCLUDE_USR_PORT
               end
            end
         join
      end


      initial begin
         #0 m_rtl_port.wait_for_master_driver();
         paddr_drv   <= {ADDR_WIDTH{1'b0}};
         pwdata_drv  <= {DATA_WIDTH{1'b0}};
         pwrite_drv  <= 1'b0;
         psel_drv    <= 1'b0;
         penable_drv <= 1'b0;
      end


      initial begin
         #0 m_rtl_port.wait_for_slave_driver();
         pready_drv  <= 1'b0;
         prdata_drv  <= {DATA_WIDTH{1'b0}};
         pslverr_drv <= 1'b0;
      end


      //--------------------------------------------------------------------------------------------
      //
      // Request Loop
      //
      initial begin
         drive_req   = 1'b0;
         req_to_send = 1'b0;
         wait (drive_req === 1'b1);
         forever @(posedge pclk) begin
            if ((req_to_send === 1'b1) && ((psel === 1'b0) || (pready === 1'b0))) begin
               psel_drv    <= `ET_TB_HOLD apb_req_drv.valid & presetn;
               penable_drv <= `ET_TB_HOLD apb_req_drv.valid & presetn;
               pwrite_drv  <= `ET_TB_HOLD apb_req_drv.pwrite;
               paddr_drv   <= `ET_TB_HOLD apb_req_drv.paddr[(ADDR_WIDTH - 1):0];
               pwdata_drv  <= `ET_TB_HOLD apb_req_drv.pwdata[(DATA_WIDTH - 1):0];
            end
            else begin
               psel_drv    <= `ET_TB_HOLD 1'b0;
               penable_drv <= `ET_TB_HOLD 1'b0;
               pwrite_drv  <= `ET_TB_HOLD 1'b0;
               paddr_drv   <= `ET_TB_HOLD {ADDR_WIDTH{1'b0}};
               pwdata_drv  <= `ET_TB_HOLD {DATA_WIDTH{1'b0}};
            end
         end
      end


      //--------------------------------------------------------------------------------------------
      //
      // Response Loop
      //
      initial begin
         drive_rsp   = 1'b0;
         rsp_to_send = 1'b0;
         wait (drive_rsp === 1'b1);
         forever @(posedge pclk) begin
            if (rsp_to_send === 1'b1) begin
               pready_drv  <= `ET_TB_HOLD apb_rsp_drv.valid & psel & penable & presetn;
               prdata_drv  <= `ET_TB_HOLD apb_rsp_drv.prdata[(DATA_WIDTH - 1):0];
               pslverr_drv <= `ET_TB_HOLD apb_rsp_drv.pslverr;
            end
            else begin
               pready_drv  <= `ET_TB_HOLD 1'b0;
               prdata_drv  <= `ET_TB_HOLD {DATA_WIDTH{1'b0}};
               pslverr_drv <= `ET_TB_HOLD 1'b0;
            end
         end
      end


      //--------------------------------------------------------------------------------------------
      //
      // Extract New Requests
      //
      always begin
         m_rtl_port.get_next_req(apb_req_drv);
         while (presetn === 1'b0) begin
            @(negedge pclk);
         end
         if (apb_req_drv.valid === 1'b1) begin
            drive_req   <= 1'b1;
            req_to_send <= 1'b1;
            while ((have_rsp !== 1'b1) && (presetn === 1'b1)) begin
               @(posedge pclk);
            end
            @(negedge pclk);
            req_to_send <= 1'b0;
         end
      end


      //--------------------------------------------------------------------------------------------
      //
      // Extract New Responses
      //
      always begin
         m_rtl_port.get_next_rsp(apb_rsp_drv);
         while (presetn === 1'b0) begin
            @(negedge pclk);
         end
         drive_rsp   <= 1'b1;
         rsp_to_send <= 1'b1;
         @(negedge pclk);
         rsp_to_send <= 1'b0;
      end

   `endif // ifdef ET_MONITOR_IFCS ----------------------------------------------------------------}

endmodule : evl_apb_interface

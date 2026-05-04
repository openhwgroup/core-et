//--------------------------------------------------------------------------------------------------
//
// Interface: evl_dbg_noc_interface
//
`include "rtl/inc/soc.vh"

`ifdef ET_SIMULATION
   `include "dv/common/dbg_noc/dbg_noc_vc/evl_dbg_noc_pkg.sv"
`endif // ifdef ET_SIMULATION

`ifndef EVL_IFC_PORT
   `ifdef EVL_SIMULATION
      `define EVL_IFC_PORT inout
   `else // ifdef EVL_SIMULATION
      `define EVL_IFC_PORT input
   `endif // !ifdef EVL_SIMULATION
`endif // ifndef EVL_IFC_PORT

module evl_dbg_noc_interface #(parameter int    SHIRE_ID                           = 0,
                               parameter int    PORT_ID                            = 0,
                               parameter int    MAX_PORT_ID                        = 0,
                               parameter int    PORT_TYPE                          = `EVL_DBGNOC_EVT_IFC_ID,
                               parameter int    ADDR_WIDTH                         = `EVL_PADDR_WIDTH,
                               parameter int    DATA_WIDTH                         = `EVL_XWORD_WIDTH,
                               parameter int    QOS_WIDTH                          = 4,
                               parameter int    VC_CREDITS                         = 1,
                               parameter int    NO_TRANS_ID_REQ_DRIVER             = 0,
                               parameter int    NO_TRANS_ID_RSP_DRIVER             = 0,
                               parameter int    SKIP_LEVELS[]                      = {0},
                               parameter int    REQ_LIMIT                          = 8
                              ) (
      input         wire                        reset,
      input         wire                        clk,

      // Req channel
      `EVL_IFC_PORT wire [ADDR_WIDTH-1:0]       tx_req_addr,
      `EVL_IFC_PORT wire [1:0]                  tx_req_dest_int,
      `EVL_IFC_PORT wire [QOS_WIDTH-1:0]        tx_req_qos,
      `EVL_IFC_PORT wire                        tx_req_valid,
      `EVL_IFC_PORT wire [DATA_WIDTH-1:0]       tx_req_data,
      `EVL_IFC_PORT wire                        tx_req_sop,
      `EVL_IFC_PORT wire                        tx_req_eop,
      `EVL_IFC_PORT wire [`EVL_TRANS_ID_MSB:0]  tx_req_trans_id,

      // Rsp channel
      `EVL_IFC_PORT wire [DATA_WIDTH-1:0]       rx_req_data,
      `EVL_IFC_PORT wire                        rx_req_valid,
      `EVL_IFC_PORT wire                        rx_req_sop,
      `EVL_IFC_PORT wire                        rx_req_eop,

      // Credits
      `EVL_IFC_PORT wire                        host_credit_inc,
      `EVL_IFC_PORT wire                        noc_credit_inc

   );

   localparam int TRUE_VC_CREDITS = ((VC_CREDITS > 0) && (VC_CREDITS < 255)) ? VC_CREDITS : 0;

   import evl_base_pkg::*;
   import evl_dbg_noc_pkg::evl_dbg_noc_req_info_t;

   bit [31:0] m_cycle_count;
   bit [31:0] m_cycle_count_posedge;

   initial begin
      m_cycle_count         <= 32'h0;
      m_cycle_count_posedge <= 32'h0;
   end

   `ifdef EVL_SIMULATION // -----------------------------------------------------------------------{

      typedef class evl_dbg_noc_rtl_port;

      import uvm_pkg::*;
      import evl_dbg_noc_pkg::evl_dbg_noc_bus_req;
      import evl_dbg_noc_pkg::evl_dbg_noc_rtl_port_base;

      // Tx channel
      reg [ADDR_WIDTH-1:0]       tx_req_addr_out;
      reg [1:0]                  tx_req_dest_int_out;
      reg [QOS_WIDTH-1:0]        tx_req_qos_out;
      reg                        tx_req_valid_out;
      reg [DATA_WIDTH-1:0]       tx_req_data_out;
      reg                        tx_req_sop_out;
      reg                        tx_req_eop_out;
      reg [`EVL_TRANS_ID_MSB:0]  tx_req_trans_id_out;

      // Rx channel
      reg [DATA_WIDTH-1:0]       rx_req_data_out;
      reg                        rx_req_valid_out;
      reg                        rx_req_sop_out;
      reg                        rx_req_eop_out;

      // Credits
      reg                        host_credit_inc_out;
      reg                        noc_credit_inc_out;


      bit              m_register_me = evl_rtl_instance_map::add_instance_info($sformatf("%m"), DBG_NOC_BUS_AGENT, SHIRE_ID, "evl_dbg_noc_verif_comp", "", "", "", { SKIP_LEVELS });
      evl_dbg_noc_rtl_port m_rtl_port    = new($sformatf("%m"), SHIRE_ID, { SKIP_LEVELS });

      //`evl_param_key(pk_data_bytes)
      static int pk_data_bytes = evl_param_xref::get_param_key("pk_data_bytes");

      initial begin
         m_rtl_port.set_int_param(pk_data_bytes, (DATA_WIDTH/8));
         m_rtl_port.m_data_bytes      = (DATA_WIDTH/8);
         m_rtl_port.m_data_bytes_log2 = $clog2(DATA_WIDTH/8);
      end

      initial begin
         tx_req_addr_out         = {ADDR_WIDTH{1'bz}};
         tx_req_dest_int_out     = 2'bz;
         tx_req_qos_out          = {QOS_WIDTH{1'bz}};
         tx_req_valid_out        = 1'bz;
         tx_req_data_out         = {DATA_WIDTH{1'bz}};
         tx_req_sop_out          = 1'bz;
         tx_req_eop_out          = 1'bz;
         rx_req_data_out        = {DATA_WIDTH{1'bz}};
         rx_req_valid_out       = 1'bz; 
         rx_req_sop_out         = 1'bz;
         rx_req_eop_out         = 1'bz;
         host_credit_inc_out  = 1'bz;
         noc_credit_inc_out   = 1'bz;
      end


      //--------------------------------------------------------------------------------------------
      //
      // monitor clock & reset
      //
      string                  m_abstract_name;
      reg                     m_request_master;
      bit                     m_delays_enabled;
      bit                     m_reset_asserted;
      bit                     m_reset_deasserted;
      bit [7:0]               m_credits;

      // Tx channel
      assign tx_req_addr         = tx_req_addr_out;
      assign tx_req_dest_int     = tx_req_dest_int_out;
      //assign tx_req_dest_int     = (m_request_master !== 1'b1) ? 2'bz : (tx_req_valid) ? 2'b01 : 2'bx;
      //assign tx_req_dest_int     = (tx_req_valid) ? tx_req_dest_int_out : 2'bx;
      assign tx_req_qos          = tx_req_qos_out;
      //assign tx_req_qos          = (m_request_master !== 1'b1) ? 4'bz : (tx_req_valid) ? 4'b01 : 4'bx;
      assign tx_req_valid        = tx_req_valid_out;
      assign tx_req_data         = tx_req_data_out;
      assign tx_req_sop          = tx_req_sop_out;
      assign tx_req_eop          = tx_req_eop_out;

      // Rx channel
      assign rx_req_data        = rx_req_data_out;
      assign rx_req_valid       = rx_req_valid_out;
      assign rx_req_sop         = rx_req_sop_out;
      assign rx_req_eop         = rx_req_eop_out;

      // Credits
      assign host_credit_inc  = (m_request_master === 1'b1) ? host_credit_inc_out : 1'bz;
      assign noc_credit_inc   = noc_credit_inc_out;

      always @ (posedge clk) begin
         host_credit_inc_out <= rx_req_valid;
      end

      initial begin
         m_reset_asserted   = 1'b0;
         m_reset_deasserted = 1'b0;
         m_request_master   = 1'b0;
         m_credits          = VC_CREDITS;
         #0;
         if (MAX_PORT_ID > 0) begin
            m_abstract_name = $sformatf("%0s(DBG_NOC-IFC%0d)", m_rtl_port.get_root_abstract_name(), PORT_ID);
         end
         else begin
            m_abstract_name = $sformatf("%0s(DBG_NOC-IFC)", m_rtl_port.get_root_abstract_name());
         end
         fork
            begin
               forever begin
                  wait (reset === 1'b1);
                  m_rtl_port.assert_reset();
                  m_reset_asserted   = 1'b1;
                  m_reset_deasserted = 1'b0;
                  if (MAX_PORT_ID > 0) begin
                     m_abstract_name = $sformatf("%0s(DBG_NOC-IFC%0d)", m_rtl_port.get_root_abstract_name(), PORT_ID);
                  end
                  else begin
                     m_abstract_name = $sformatf("%0s(DBG_NOC-IFC)", m_rtl_port.get_root_abstract_name());
                  end
                  m_credits = VC_CREDITS;
                  wait (reset === 1'b0);
                  m_rtl_port.deassert_reset();
                  m_reset_asserted   = 1'b0;
                  m_reset_deasserted = 1'b1;
                  if (MAX_PORT_ID > 0) begin
                     m_abstract_name = $sformatf("%0s(DBG_NOC-IFC%0d)", m_rtl_port.get_root_abstract_name(), PORT_ID);
                  end
                  else begin
                     m_abstract_name = $sformatf("%0s(DBG_NOC-IFC)", m_rtl_port.get_root_abstract_name());
                  end
               end
            end
            begin
               wait (clk === 1'b0);
               forever begin
                  @(posedge clk);
                  // Capture pin values here
                  if (TRUE_VC_CREDITS > 0) begin
                     if (tx_req_valid == 1'b1) begin
                        if (m_credits == 8'h00) begin
                           `dut_warning($sformatf("%0s(DBG_NOC-IFC)", m_rtl_port.get_root_abstract_name()), $sformatf("credit underflow!"))
                           m_credits = TRUE_VC_CREDITS;
                        end
                        else begin
                           m_credits = m_credits - 8'h01;
                        end
                     end
                  end
                  if (TRUE_VC_CREDITS > 0) begin
                     if (noc_credit_inc == 1'b1) begin
                        if (m_credits == TRUE_VC_CREDITS) begin
                           `dut_warning($sformatf("%0s(DBG_NOC-IFC)", m_rtl_port.get_root_abstract_name()), $sformatf("credit overflow!"))
                           m_credits = 8'h00;
                        end
                        else begin
                           m_credits = m_credits + 8'h01;
                        end
                     end
                  end
               end
            end
         join
      end

      //--------------------------------------------------------------------------------------------
      //
      // m_tx_req:     This bit vector contains request bits for Tx bus requests on a per VC
      //               basis.  In addition, the most significant bit will be set if there is a
      //               request in progress.
      // m_cycle:      This bit vector contains cycle count information for delaying AW bus
      //               requests on a per VC basis.  Cycle count vectors awe currently 32 bits
      //               wide.
      // m_bus_tx_reqs[]: This memory awray contains the bus request infomation per VC.
      //
      bit                            m_tx_req             = 1'b0;
      bit [31:0]                     m_tx_req_cycle_delay = 0;
      mailbox #(evl_dbg_noc_bus_req) m_bus_tx_req_mb;

      evl_dbg_noc_bus_req      bus_tx_req;
      bit [31:0]               req_cycle;
      bit [31:0]               req_cycle_delay;

      initial begin
         m_bus_tx_req_mb = new();
         forever begin
            m_bus_tx_req_mb.get(bus_tx_req);
            m_tx_req = 1'b1;
            wait (m_tx_req === 1'b0);
         end
      end

      always begin
         wait (reset === 1'b0);
         #0 wait ((m_tx_req === 1'b1) || (reset !== 1'b0));
         req_cycle_delay = 0;
         if (m_tx_req === 1'b1) begin
            req_cycle = bus_tx_req.get_cycle();
            if ((m_delays_enabled == 1'b1) && (req_cycle > m_cycle_count_posedge[31:0])) begin
               req_cycle_delay = req_cycle - m_cycle_count_posedge[31:0];
            end
         end
         #0 wait ( (reset !== 1'b0) ||
                   ( ((m_delays_enabled == 1'b0) || (req_cycle <= m_cycle_count_posedge[31:0])) &&
                     ((TRUE_VC_CREDITS <= 0) || (m_credits > 8'h00)) ) );
         if (reset === 1'b0) begin
            #0 wait (m_tx_req);
            if ((m_delays_enabled == 1'b1) && (req_cycle <= m_cycle_count_posedge[31:0])) begin
               m_tx_req_cycle_delay <= req_cycle_delay;
            end
            tx_req_addr_out      <= `ET_TB_HOLD bus_tx_req.get_req_addr();
            tx_req_dest_int_out  <= `ET_TB_HOLD bus_tx_req.get_req_dest_int();
            tx_req_qos_out       <= `ET_TB_HOLD bus_tx_req.get_req_qos();
            tx_req_data_out      <= `ET_TB_HOLD bus_tx_req.get_req_data();
            tx_req_eop_out       <= `ET_TB_HOLD bus_tx_req.get_req_eop();
            tx_req_trans_id_out  <= `ET_TB_HOLD bus_tx_req.get_trans_id();
            tx_req_valid_out     <= `ET_TB_HOLD 1'b1;
            tx_req_sop_out       <= `ET_TB_HOLD 1'b1;
            @(posedge clk);
            tx_req_valid_out     <= `ET_TB_HOLD 1'b0;
            tx_req_sop_out       <= `ET_TB_HOLD 1'b0;
            tx_req_eop_out       <= `ET_TB_HOLD 1'b0;
            m_tx_req             = 1'b0;
         end
      end

      //--------------------------------------------------------------------------------------------
      //
      // Class: evl_dbg_noc_rtl_port
      //
      class evl_dbg_noc_rtl_port extends evl_dbg_noc_rtl_port_base;


         //-----------------------------------------------------------------------------------------
         //
         // Task: zero_tx_req_values
         //
         // This task zeros the signals on the Tx interface.
         //
         virtual task zero_req_values();
            tx_req_valid_out        <= 1'b0;
            tx_req_addr_out         <= {ADDR_WIDTH{1'b0}};
            tx_req_qos_out          <= {QOS_WIDTH{1'b0}};
            tx_req_data_out         <= {DATA_WIDTH{1'b0}};
            tx_req_sop_out          <= 1'b0;
            tx_req_eop_out          <= 1'b0;
            tx_req_trans_id_out     <= {(`EVL_TRANS_ID_MSB+1){1'b0}};
            tx_req_dest_int_out     <= 2'b0;
            //tx_req_dest_int_out     <= (PORT_TYPE == `EVL_DBGNOC_EVT_IFC_ID) ? 2'b01 : 2'b0;
            //host_credit_inc_out  <= 1'b0;
            m_request_master     <= 1'b1;
         endtask : zero_req_values


         //-----------------------------------------------------------------------------------------
         //
         // Constructor
         //
         function new(input string         full_name_in     = "dbg_noc_rtl_port",
                      input evl_agent_id_t agent_pid_in     = ~evl_agent_id_t'(0),
                      input int            skip_levels_in[] = {});
            super.new(full_name_in, DBG_NOC_BUS_AGENT, agent_pid_in, "evl_dbg_noc_verif_comp", "", "", "", skip_levels_in, ((MAX_PORT_ID > 0) ? PORT_ID : 0), MAX_PORT_ID);
            set_int_param_by_name("pk_addr_width", ADDR_WIDTH / 8);
            set_int_param_by_name("pk_data_width", DATA_WIDTH / 8);
            set_int_param_by_name("pk_vc_credits", TRUE_VC_CREDITS);
         endfunction : new
      endclass : evl_dbg_noc_rtl_port


      //--------------------------------------------------------------------------------------------
      //
      // Task: get_and_send_tx_reqs
      //
      task get_and_send_tx_reqs;
         evl_dbg_noc_bus_req  bus_tx_req;
         evl_delay_t          req_delay;
         int unsigned         next_req_cycle;

         next_req_cycle = 0;
         forever begin
            m_rtl_port.get_tx_req(bus_tx_req);

            req_delay = bus_tx_req.get_bus_req_delay();
            // figure out when we can send the req
            next_req_cycle = ((m_cycle_count[31:0] + req_delay) > next_req_cycle) ? m_cycle_count[31:0] + req_delay : next_req_cycle + 1;
            bus_tx_req.set_cycle(next_req_cycle);

            m_bus_tx_req_mb.put(bus_tx_req);
         end
      endtask : get_and_send_tx_reqs


      //--------------------------------------------------------------------------------------------
      //
      // Request and Response Stimulus Processing
      //
      initial begin
         #0;
         fork
            begin
               get_and_send_tx_reqs;
            end
         join
      end

   `endif // ifdef EVL_SIMULATION -----------------------------------------------------------------}

   `ifdef EVL_SIMULATION // -----------------------------------------------------------------------{
      initial begin
         #0;
         m_delays_enabled = 1'b0;
         forever begin
            m_rtl_port.wait_for_delay_enable();
            #0 m_delays_enabled = 1'b1;
            m_rtl_port.wait_for_delay_disable();
            #0 m_delays_enabled = 1'b0;
         end
      end
   `endif // EVL_SIMULATION -----------------------------------------------------------------------}


   //-----------------------------------------------------------------------------------------------
   evl_dbg_noc_req_info_t  tx_req;
   evl_dbg_noc_req_info_t  rx_req;
   bit                     valid_tx_req;
   bit                     valid_rx_req;
   bit                     valid_rsp;
   bit                     mask_warning;
   bit                     valid_assert;

   always @(posedge clk) begin
      valid_tx_req   = 1'b0;
      valid_rx_req   = 1'b0;
      valid_rsp      = 1'b0;
      if (reset === 1'b1) begin
         mask_warning   <= 1'b1;
         valid_assert   <= 1'b0;
      end
      if (reset === 1'b0) begin
         if (tx_req_valid === 1'b1) begin
            valid_tx_req         = 1'b1;
            tx_req.req_addr      = tx_req_addr;
            tx_req.req_dest_int  = tx_req_dest_int;
            tx_req.req_qos       = tx_req_qos;
            tx_req.req_data      = tx_req_data;
            tx_req.req_eop       = tx_req_eop;
            tx_req.trans_id      = tx_req_trans_id;
         end

         if (rx_req_valid === 1'b1) begin
            valid_rx_req      = 1'b1;
            rx_req.req_data   = rx_req_data;
            rx_req.req_eop    = rx_req_eop;
         end

         if (noc_credit_inc === 1'b1) begin
            valid_rsp = 1'b1;
         end

         `ifdef EVL_SIMULATION // -----------------------------------------------------------------{

            if (valid_tx_req == 1'b1) begin
               m_rtl_port.put_mon_tx_req(tx_req);
            end
            if (valid_rx_req == 1'b1) begin
               m_rtl_port.put_mon_rx_req(rx_req);
            end
            if (valid_rsp == 1'b1) begin
               m_rtl_port.put_mon_resp(valid_rsp);
            end

         `endif // EVL_SIMULATION -----------------------------------------------------------------}
      end

      `ifdef EVL_SIMULATION // --------------------------------------------------------------------{
         m_rtl_port.ping_clock();
      `endif // EVL_SIMULATION --------------------------------------------------------------------}
   end

   always @(posedge clk) begin
      m_cycle_count_posedge <= m_cycle_count_posedge + 32'h1;
   end


   always @(negedge clk) begin
      m_cycle_count <= m_cycle_count_posedge;
   end

endmodule

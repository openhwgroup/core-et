`include "soc_defines.vh"
`include "dv/common/axi/axi_vc/evl_axi_pkg.sv"

module evl_ioshire_meshstop_monitor
   #(
   parameter int IOSHIRE_ID = 254,
   parameter int NUM_L3_PORTS = 1
   )
   (
   input       reset_n_system,
   input       clk__noc,
   input wire  io0_ps_ARVALID,
   input wire  io0_ps_ARREADY,
   input wire  io0_ps_AWVALID,
   input wire  io0_ps_AWREADY,
   input wire  io0_ps_WVALID,
   input wire  io0_ps_WREADY,
   input wire  io0_ps_BVALID,
   input wire  io0_ps_BREADY,
   input wire  io0_ps_RVALID,
   input wire  io0_ps_RREADY,

   input wire  io0_pm_ARVALID,
   input wire  io0_pm_ARREADY,
   input wire  io0_pm_AWVALID,
   input wire  io0_pm_AWREADY,
   input wire  io0_pm_WVALID,
   input wire  io0_pm_WREADY,
   input wire  io0_pm_BVALID,
   input wire  io0_pm_BREADY,
   input wire  io0_pm_RVALID,
   input wire  io0_pm_RREADY,

   input wire  io0_tosys_m_ARVALID,
   input wire  io0_tosys_m_ARREADY,
   input wire  io0_tosys_m_AWVALID,
   input wire  io0_tosys_m_AWREADY,
   input wire  io0_tosys_m_WVALID,
   input wire  io0_tosys_m_WREADY,
   input wire  io0_tosys_m_BVALID,
   input wire  io0_tosys_m_BREADY,
   input wire  io0_tosys_m_RVALID,
   input wire  io0_tosys_m_RREADY,

   input wire  [NUM_L3_PORTS-1:0] io0_tol3_m_ARVALID,
   input wire  [NUM_L3_PORTS-1:0] io0_tol3_m_ARREADY,
   input wire  [NUM_L3_PORTS-1:0] io0_tol3_m_AWVALID,
   input wire  [NUM_L3_PORTS-1:0] io0_tol3_m_AWREADY,
   input wire  [NUM_L3_PORTS-1:0] io0_tol3_m_WVALID,
   input wire  [NUM_L3_PORTS-1:0] io0_tol3_m_WREADY,
   input wire  [NUM_L3_PORTS-1:0] io0_tol3_m_BVALID,
   input wire  [NUM_L3_PORTS-1:0] io0_tol3_m_BREADY,
   input wire  [NUM_L3_PORTS-1:0] io0_tol3_m_RVALID,
   input wire  [NUM_L3_PORTS-1:0] io0_tol3_m_RREADY

   );

   import uvm_pkg::*;
   import evl_base_pkg::*;
   import evl_noc_pkg::*;
   import evl_axi_types_pkg::*;

   `include "get_shire_id_functions.vh"

   localparam  int NUM_NOC_LAYERS      = 8;
   localparam  int NUM_VIRT_CHANNELS   = 4;
   localparam  int NUM_PORT = 8;
   localparam  int MAX_CREDIT_WIDTH = 8;

   logic [NUM_NOC_LAYERS-1:0][NUM_PORT-1:0][NUM_VIRT_CHANNELS-1:0][MAX_CREDIT_WIDTH-1:0] inport_rd_pntr_bin_q;
   logic [NUM_NOC_LAYERS-1:0][NUM_PORT-1:0][NUM_VIRT_CHANNELS-1:0][MAX_CREDIT_WIDTH-1:0] inport_wr_pntr_bin_q;
   logic [NUM_NOC_LAYERS-1:0][NUM_PORT-1:0][NUM_VIRT_CHANNELS-1:0][MAX_CREDIT_WIDTH-1:0] inport_fifo_depth;
   logic [NUM_NOC_LAYERS-1:0][NUM_PORT-1:0][NUM_VIRT_CHANNELS-1:0][MAX_CREDIT_WIDTH-1:0] outport_credit_cntr_q;
   logic [NUM_NOC_LAYERS-1:0][NUM_PORT-1:0][NUM_VIRT_CHANNELS-1:0][MAX_CREDIT_WIDTH-1:0] outport_fifo_depth;
   int  input_fifo_depth_histogram[NUM_NOC_LAYERS-1:0][NUM_PORT-1:0][NUM_VIRT_CHANNELS-1:0][MAX_CREDIT_WIDTH];
   int output_fifo_depth_histogram[NUM_NOC_LAYERS-1:0][NUM_PORT-1:0][NUM_VIRT_CHANNELS-1:0][MAX_CREDIT_WIDTH];
   int clk_cnt = 0 ;


   assign inport_fifo_depth =  '{ '{ '{ 4'b110, 4'b0, 4'b0, 4'b110 }, '{ 4'b0, 4'b0, 4'b0, 4'b0 }, '{ 4'b101, 4'b0, 4'b10, 4'b101 }, '{ 4'b0, 4'b0, 4'b0, 4'b0 }, '{ 4'b0, 4'b0, 4'b0, 4'b0 }, '{ 4'b10, 4'b1, 4'b0, 4'b0 }, '{ 4'b0, 4'b0, 4'b0, 4'b0 }, '{ 4'b0, 4'b0, 4'b0, 4'b0 } }, '{ '{ 4'b110, 4'b0, 4'b0, 4'b110 }, '{ 4'b0, 4'b0, 4'b0, 4'b0 }, '{ 4'b101, 4'b1, 4'b0, 4'b101 }, '{ 4'b0, 4'b0, 4'b0, 4'b0 }, '{ 4'b0, 4'b0, 4'b0, 4'b0 }, '{ 4'b10, 4'b0, 4'b0, 4'b0 }, '{ 4'b0, 4'b0, 4'b0, 4'b10 }, '{ 4'b0, 4'b0, 4'b0, 4'b0 } }, '{ '{ 4'b110, 4'b0, 4'b0, 4'b110 }, '{ 4'b0, 4'b0, 4'b0, 4'b0 }, '{ 4'b0, 4'b0, 4'b0, 4'b101 }, '{ 4'b0, 4'b0, 4'b0, 4'b0 }, '{ 4'b0, 4'b0, 4'b10, 4'b0 }, '{ 4'b10, 4'b10, 4'b0, 4'b0 }, '{ 4'b0, 4'b0, 4'b0, 4'b0 }, '{ 4'b0, 4'b0, 4'b0, 4'b0 } }, '{ '{ 4'b110, 4'b0, 4'b0, 4'b110 }, '{ 4'b0, 4'b0, 4'b0, 4'b0 }, '{ 4'b0, 4'b0, 4'b0, 4'b101 }, '{ 4'b0, 4'b0, 4'b0, 4'b0 }, '{ 4'b0, 4'b0, 4'b10, 4'b0 }, '{ 4'b10, 4'b0, 4'b0, 4'b0 }, '{ 4'b0, 4'b0, 4'b0, 4'b0 }, '{ 4'b0, 4'b0, 4'b0, 4'b0 } }, '{ '{ 4'b110, 4'b0, 4'b0, 4'b110 }, '{ 4'b0, 4'b0, 4'b0, 4'b0 }, '{ 4'b101, 4'b101, 4'b0, 4'b101 }, '{ 4'b0, 4'b0, 4'b0, 4'b0 }, '{ 4'b0, 4'b0, 4'b0, 4'b0 }, '{ 4'b10, 4'b0, 4'b0, 4'b0 }, '{ 4'b0, 4'b10, 4'b0, 4'b0 }, '{ 4'b0, 4'b0, 4'b0, 4'b10 } }, '{ '{ 4'b110, 4'b0, 4'b110, 4'b110 }, '{ 4'b0, 4'b0, 4'b0, 4'b0 }, '{ 4'b101, 4'b101, 4'b101, 4'b101 }, '{ 4'b0, 4'b0, 4'b0, 4'b0 }, '{ 4'b0, 4'b0, 4'b0, 4'b0 }, '{ 4'b10, 4'b0, 4'b0, 4'b0 }, '{ 4'b0, 4'b10, 4'b0, 4'b0 }, '{ 4'b0, 4'b0, 4'b0, 4'b10 } }, '{ '{ 4'b110, 4'b110, 4'b0, 4'b110 }, '{ 4'b0, 4'b0, 4'b0, 4'b0 }, '{ 4'b101, 4'b101, 4'b0, 4'b101 }, '{ 4'b0, 4'b0, 4'b0, 4'b0 }, '{ 4'b0, 4'b10, 4'b0, 4'b0 }, '{ 4'b10, 4'b0, 4'b0, 4'b0 }, '{ 4'b0, 4'b0, 4'b0, 4'b0 }, '{ 4'b0, 4'b0, 4'b0, 4'b10 } }, '{ '{ 4'b110, 4'b110, 4'b0, 4'b110 }, '{ 4'b0, 4'b0, 4'b0, 4'b0 }, '{ 4'b101, 4'b101, 4'b0, 4'b101 }, '{ 4'b0, 4'b0, 4'b0, 4'b0 }, '{ 4'b0, 4'b10, 4'b0, 4'b0 }, '{ 4'b10, 4'b0, 4'b0, 4'b0 }, '{ 4'b0, 4'b0, 4'b0, 4'b0 }, '{ 4'b0, 4'b0, 4'b0, 4'b10 } } } ;
   assign outport_fifo_depth =  '{ '{ '{ 4'b110, 4'b0, 4'b0, 4'b110 }, '{ 4'b0, 4'b0, 4'b0, 4'b0 }, '{ 4'b101, 4'b1, 4'b0, 4'b0 }, '{ 4'b0, 4'b0, 4'b0, 4'b0 }, '{ 4'b10, 4'b0, 4'b0, 4'b0 }, '{ 4'b0, 4'b0, 4'b10, 4'b10 }, '{ 4'b10, 4'b0, 4'b0, 4'b0 }, '{ 4'b0, 4'b0, 4'b0, 4'b0 } }, '{ '{ 4'b110, 4'b0, 4'b0, 4'b110 }, '{ 4'b0, 4'b0, 4'b0, 4'b0 }, '{ 4'b101, 4'b0, 4'b0, 4'b101 }, '{ 4'b0, 4'b0, 4'b0, 4'b0 }, '{ 4'b10, 4'b0, 4'b0, 4'b0 }, '{ 4'b0, 4'b1, 4'b0, 4'b10 }, '{ 4'b0, 4'b0, 4'b0, 4'b0 }, '{ 4'b0, 4'b0, 4'b0, 4'b0 } }, '{ '{ 4'b110, 4'b0, 4'b0, 4'b110 }, '{ 4'b0, 4'b0, 4'b0, 4'b0 }, '{ 4'b101, 4'b10, 4'b101, 4'b0 }, '{ 4'b0, 4'b0, 4'b0, 4'b0 }, '{ 4'b0, 4'b0, 4'b0, 4'b0 }, '{ 4'b0, 4'b0, 4'b0, 4'b10 }, '{ 4'b0, 4'b0, 4'b0, 4'b0 }, '{ 4'b0, 4'b0, 4'b0, 4'b0 } }, '{ '{ 4'b110, 4'b0, 4'b0, 4'b110 }, '{ 4'b0, 4'b0, 4'b0, 4'b0 }, '{ 4'b101, 4'b0, 4'b101, 4'b0 }, '{ 4'b0, 4'b0, 4'b0, 4'b0 }, '{ 4'b0, 4'b0, 4'b0, 4'b0 }, '{ 4'b0, 4'b0, 4'b0, 4'b10 }, '{ 4'b0, 4'b0, 4'b0, 4'b0 }, '{ 4'b0, 4'b0, 4'b0, 4'b0 } }, '{ '{ 4'b110, 4'b110, 4'b0, 4'b110 }, '{ 4'b0, 4'b0, 4'b0, 4'b0 }, '{ 4'b101, 4'b101, 4'b0, 4'b101 }, '{ 4'b0, 4'b0, 4'b0, 4'b0 }, '{ 4'b0, 4'b0, 4'b0, 4'b0 }, '{ 4'b0, 4'b0, 4'b0, 4'b10 }, '{ 4'b0, 4'b0, 4'b0, 4'b0 }, '{ 4'b10, 4'b0, 4'b0, 4'b0 } }, '{ '{ 4'b110, 4'b110, 4'b0, 4'b110 }, '{ 4'b0, 4'b0, 4'b0, 4'b0 }, '{ 4'b101, 4'b101, 4'b101, 4'b101 }, '{ 4'b0, 4'b0, 4'b0, 4'b0 }, '{ 4'b0, 4'b0, 4'b0, 4'b0 }, '{ 4'b0, 4'b0, 4'b0, 4'b10 }, '{ 4'b0, 4'b0, 4'b11, 4'b0 }, '{ 4'b10, 4'b0, 4'b0, 4'b0 } }, '{ '{ 4'b110, 4'b110, 4'b0, 4'b110 }, '{ 4'b0, 4'b0, 4'b0, 4'b0 }, '{ 4'b101, 4'b0, 4'b0, 4'b101 }, '{ 4'b0, 4'b0, 4'b0, 4'b0 }, '{ 4'b10, 4'b0, 4'b0, 4'b0 }, '{ 4'b0, 4'b10, 4'b0, 4'b10 }, '{ 4'b0, 4'b0, 4'b0, 4'b0 }, '{ 4'b10, 4'b0, 4'b0, 4'b0 } }, '{ '{ 4'b110, 4'b110, 4'b0, 4'b110 }, '{ 4'b0, 4'b0, 4'b0, 4'b0 }, '{ 4'b101, 4'b0, 4'b0, 4'b101 }, '{ 4'b0, 4'b0, 4'b0, 4'b0 }, '{ 4'b10, 4'b0, 4'b0, 4'b0 }, '{ 4'b0, 4'b10, 4'b0, 4'b10 }, '{ 4'b0, 4'b0, 4'b0, 4'b0 }, '{ 4'b10, 4'b0, 4'b0, 4'b0 } } } ;

`ifndef  IOSHIRE_MESHSTOP_WRAPPER_GATESIM
   assign  outport_credit_cntr_q = '{ '{ '{  u_ns_io_meshstop.u_ns_router_7_14.u_ns_router.u_west_outblk.G_OUT_PORT_ENB.G_OVCSTS[3].G_OVC_ENABLED.u_ns_ovcsts.credit_cntr_q[4:0], 1'b0, 1'b0, u_ns_io_meshstop.u_ns_router_7_14.u_ns_router.u_west_outblk.G_OUT_PORT_ENB.G_OVCSTS[0].G_OVC_ENABLED.u_ns_ovcsts.credit_cntr_q[4:0] }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  u_ns_io_meshstop.u_ns_router_7_14.u_ns_router.u_south_outblk.G_OUT_PORT_ENB.G_OVCSTS[3].G_OVC_ENABLED.u_ns_ovcsts.credit_cntr_q[4:0], u_ns_io_meshstop.u_ns_router_7_14.u_ns_router.u_south_outblk.G_OUT_PORT_ENB.G_OVCSTS[2].G_OVC_ENABLED.u_ns_ovcsts.credit_cntr_q[4:0], 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  u_ns_io_meshstop.u_ns_router_7_14.u_ns_router.u_host3_outblk.G_OUT_PORT_ENB.G_OVCSTS[3].G_OVC_ENABLED.u_ns_ovcsts.credit_cntr_q[4:0], 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, u_ns_io_meshstop.u_ns_router_7_14.u_ns_router.u_host2_outblk.G_OUT_PORT_ENB.G_OVCSTS[1].G_OVC_ENABLED.u_ns_ovcsts.credit_cntr_q[4:0], u_ns_io_meshstop.u_ns_router_7_14.u_ns_router.u_host2_outblk.G_OUT_PORT_ENB.G_OVCSTS[0].G_OVC_ENABLED.u_ns_ovcsts.credit_cntr_q[4:0] }, '{  u_ns_io_meshstop.u_ns_router_7_14.u_ns_router.u_host1_outblk.G_OUT_PORT_ENB.G_OVCSTS[3].G_OVC_ENABLED.u_ns_ovcsts.credit_cntr_q[4:0], 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }  }, '{ '{  u_ns_io_meshstop.u_ns_router_6_14.u_ns_router.u_west_outblk.G_OUT_PORT_ENB.G_OVCSTS[3].G_OVC_ENABLED.u_ns_ovcsts.credit_cntr_q[4:0], 1'b0, 1'b0, u_ns_io_meshstop.u_ns_router_6_14.u_ns_router.u_west_outblk.G_OUT_PORT_ENB.G_OVCSTS[0].G_OVC_ENABLED.u_ns_ovcsts.credit_cntr_q[4:0] }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  u_ns_io_meshstop.u_ns_router_6_14.u_ns_router.u_south_outblk.G_OUT_PORT_ENB.G_OVCSTS[3].G_OVC_ENABLED.u_ns_ovcsts.credit_cntr_q[4:0], 1'b0, 1'b0, u_ns_io_meshstop.u_ns_router_6_14.u_ns_router.u_south_outblk.G_OUT_PORT_ENB.G_OVCSTS[0].G_OVC_ENABLED.u_ns_ovcsts.credit_cntr_q[4:0] }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  u_ns_io_meshstop.u_ns_router_6_14.u_ns_router.u_host3_outblk.G_OUT_PORT_ENB.G_OVCSTS[3].G_OVC_ENABLED.u_ns_ovcsts.credit_cntr_q[4:0], 1'b0, 1'b0, 1'b0 }, '{  1'b0, u_ns_io_meshstop.u_ns_router_6_14.u_ns_router.u_host2_outblk.G_OUT_PORT_ENB.G_OVCSTS[2].G_OVC_ENABLED.u_ns_ovcsts.credit_cntr_q[4:0], 1'b0, u_ns_io_meshstop.u_ns_router_6_14.u_ns_router.u_host2_outblk.G_OUT_PORT_ENB.G_OVCSTS[0].G_OVC_ENABLED.u_ns_ovcsts.credit_cntr_q[4:0] }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }  }, '{ '{  u_ns_io_meshstop.u_ns_router_5_14.u_ns_router.u_west_outblk.G_OUT_PORT_ENB.G_OVCSTS[3].G_OVC_ENABLED.u_ns_ovcsts.credit_cntr_q[4:0], 1'b0, 1'b0, u_ns_io_meshstop.u_ns_router_5_14.u_ns_router.u_west_outblk.G_OUT_PORT_ENB.G_OVCSTS[0].G_OVC_ENABLED.u_ns_ovcsts.credit_cntr_q[4:0] }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  u_ns_io_meshstop.u_ns_router_5_14.u_ns_router.u_south_outblk.G_OUT_PORT_ENB.G_OVCSTS[3].G_OVC_ENABLED.u_ns_ovcsts.credit_cntr_q[4:0], u_ns_io_meshstop.u_ns_router_5_14.u_ns_router.u_south_outblk.G_OUT_PORT_ENB.G_OVCSTS[2].G_OVC_ENABLED.u_ns_ovcsts.credit_cntr_q[4:0], u_ns_io_meshstop.u_ns_router_5_14.u_ns_router.u_south_outblk.G_OUT_PORT_ENB.G_OVCSTS[1].G_OVC_ENABLED.u_ns_ovcsts.credit_cntr_q[4:0], 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, u_ns_io_meshstop.u_ns_router_5_14.u_ns_router.u_host2_outblk.G_OUT_PORT_ENB.G_OVCSTS[0].G_OVC_ENABLED.u_ns_ovcsts.credit_cntr_q[4:0] }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }  }, '{ '{  u_ns_io_meshstop.u_ns_router_4_14.u_ns_router.u_west_outblk.G_OUT_PORT_ENB.G_OVCSTS[3].G_OVC_ENABLED.u_ns_ovcsts.credit_cntr_q[4:0], 1'b0, 1'b0, u_ns_io_meshstop.u_ns_router_4_14.u_ns_router.u_west_outblk.G_OUT_PORT_ENB.G_OVCSTS[0].G_OVC_ENABLED.u_ns_ovcsts.credit_cntr_q[4:0] }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  u_ns_io_meshstop.u_ns_router_4_14.u_ns_router.u_south_outblk.G_OUT_PORT_ENB.G_OVCSTS[3].G_OVC_ENABLED.u_ns_ovcsts.credit_cntr_q[4:0], 1'b0, u_ns_io_meshstop.u_ns_router_4_14.u_ns_router.u_south_outblk.G_OUT_PORT_ENB.G_OVCSTS[1].G_OVC_ENABLED.u_ns_ovcsts.credit_cntr_q[4:0], 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, u_ns_io_meshstop.u_ns_router_4_14.u_ns_router.u_host2_outblk.G_OUT_PORT_ENB.G_OVCSTS[0].G_OVC_ENABLED.u_ns_ovcsts.credit_cntr_q[4:0] }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }  }, '{ '{  u_ns_io_meshstop.u_ns_router_3_14.u_ns_router.u_west_outblk.G_OUT_PORT_ENB.G_OVCSTS[3].G_OVC_ENABLED.u_ns_ovcsts.credit_cntr_q[4:0], u_ns_io_meshstop.u_ns_router_3_14.u_ns_router.u_west_outblk.G_OUT_PORT_ENB.G_OVCSTS[2].G_OVC_ENABLED.u_ns_ovcsts.credit_cntr_q[4:0], 1'b0, u_ns_io_meshstop.u_ns_router_3_14.u_ns_router.u_west_outblk.G_OUT_PORT_ENB.G_OVCSTS[0].G_OVC_ENABLED.u_ns_ovcsts.credit_cntr_q[4:0] }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  u_ns_io_meshstop.u_ns_router_3_14.u_ns_router.u_south_outblk.G_OUT_PORT_ENB.G_OVCSTS[3].G_OVC_ENABLED.u_ns_ovcsts.credit_cntr_q[4:0], u_ns_io_meshstop.u_ns_router_3_14.u_ns_router.u_south_outblk.G_OUT_PORT_ENB.G_OVCSTS[2].G_OVC_ENABLED.u_ns_ovcsts.credit_cntr_q[4:0], 1'b0, u_ns_io_meshstop.u_ns_router_3_14.u_ns_router.u_south_outblk.G_OUT_PORT_ENB.G_OVCSTS[0].G_OVC_ENABLED.u_ns_ovcsts.credit_cntr_q[4:0] }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, u_ns_io_meshstop.u_ns_router_3_14.u_ns_router.u_host2_outblk.G_OUT_PORT_ENB.G_OVCSTS[0].G_OVC_ENABLED.u_ns_ovcsts.credit_cntr_q[4:0] }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  u_ns_io_meshstop.u_ns_router_3_14.u_ns_router.u_host0_outblk.G_OUT_PORT_ENB.G_OVCSTS[3].G_OVC_ENABLED.u_ns_ovcsts.credit_cntr_q[4:0], 1'b0, 1'b0, 1'b0 }  }, '{ '{  u_ns_io_meshstop.u_ns_router_2_14.u_ns_router.u_west_outblk.G_OUT_PORT_ENB.G_OVCSTS[3].G_OVC_ENABLED.u_ns_ovcsts.credit_cntr_q[4:0], u_ns_io_meshstop.u_ns_router_2_14.u_ns_router.u_west_outblk.G_OUT_PORT_ENB.G_OVCSTS[2].G_OVC_ENABLED.u_ns_ovcsts.credit_cntr_q[4:0], 1'b0, u_ns_io_meshstop.u_ns_router_2_14.u_ns_router.u_west_outblk.G_OUT_PORT_ENB.G_OVCSTS[0].G_OVC_ENABLED.u_ns_ovcsts.credit_cntr_q[4:0] }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  u_ns_io_meshstop.u_ns_router_2_14.u_ns_router.u_south_outblk.G_OUT_PORT_ENB.G_OVCSTS[3].G_OVC_ENABLED.u_ns_ovcsts.credit_cntr_q[4:0], u_ns_io_meshstop.u_ns_router_2_14.u_ns_router.u_south_outblk.G_OUT_PORT_ENB.G_OVCSTS[2].G_OVC_ENABLED.u_ns_ovcsts.credit_cntr_q[4:0], u_ns_io_meshstop.u_ns_router_2_14.u_ns_router.u_south_outblk.G_OUT_PORT_ENB.G_OVCSTS[1].G_OVC_ENABLED.u_ns_ovcsts.credit_cntr_q[4:0], u_ns_io_meshstop.u_ns_router_2_14.u_ns_router.u_south_outblk.G_OUT_PORT_ENB.G_OVCSTS[0].G_OVC_ENABLED.u_ns_ovcsts.credit_cntr_q[4:0] }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, u_ns_io_meshstop.u_ns_router_2_14.u_ns_router.u_host2_outblk.G_OUT_PORT_ENB.G_OVCSTS[0].G_OVC_ENABLED.u_ns_ovcsts.credit_cntr_q[4:0] }, '{  1'b0, 1'b0, u_ns_io_meshstop.u_ns_router_2_14.u_ns_router.u_host1_outblk.G_OUT_PORT_ENB.G_OVCSTS[1].G_OVC_ENABLED.u_ns_ovcsts.credit_cntr_q[4:0], 1'b0 }, '{  u_ns_io_meshstop.u_ns_router_2_14.u_ns_router.u_host0_outblk.G_OUT_PORT_ENB.G_OVCSTS[3].G_OVC_ENABLED.u_ns_ovcsts.credit_cntr_q[4:0], 1'b0, 1'b0, 1'b0 }  }, '{ '{  u_ns_io_meshstop.u_ns_router_1_14.u_ns_router.u_west_outblk.G_OUT_PORT_ENB.G_OVCSTS[3].G_OVC_ENABLED.u_ns_ovcsts.credit_cntr_q[4:0], u_ns_io_meshstop.u_ns_router_1_14.u_ns_router.u_west_outblk.G_OUT_PORT_ENB.G_OVCSTS[2].G_OVC_ENABLED.u_ns_ovcsts.credit_cntr_q[4:0], 1'b0, u_ns_io_meshstop.u_ns_router_1_14.u_ns_router.u_west_outblk.G_OUT_PORT_ENB.G_OVCSTS[0].G_OVC_ENABLED.u_ns_ovcsts.credit_cntr_q[4:0] }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  u_ns_io_meshstop.u_ns_router_1_14.u_ns_router.u_south_outblk.G_OUT_PORT_ENB.G_OVCSTS[3].G_OVC_ENABLED.u_ns_ovcsts.credit_cntr_q[4:0], 1'b0, 1'b0, u_ns_io_meshstop.u_ns_router_1_14.u_ns_router.u_south_outblk.G_OUT_PORT_ENB.G_OVCSTS[0].G_OVC_ENABLED.u_ns_ovcsts.credit_cntr_q[4:0] }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  u_ns_io_meshstop.u_ns_router_1_14.u_ns_router.u_host3_outblk.G_OUT_PORT_ENB.G_OVCSTS[3].G_OVC_ENABLED.u_ns_ovcsts.credit_cntr_q[4:0], 1'b0, 1'b0, 1'b0 }, '{  1'b0, u_ns_io_meshstop.u_ns_router_1_14.u_ns_router.u_host2_outblk.G_OUT_PORT_ENB.G_OVCSTS[2].G_OVC_ENABLED.u_ns_ovcsts.credit_cntr_q[4:0], 1'b0, u_ns_io_meshstop.u_ns_router_1_14.u_ns_router.u_host2_outblk.G_OUT_PORT_ENB.G_OVCSTS[0].G_OVC_ENABLED.u_ns_ovcsts.credit_cntr_q[4:0] }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  u_ns_io_meshstop.u_ns_router_1_14.u_ns_router.u_host0_outblk.G_OUT_PORT_ENB.G_OVCSTS[3].G_OVC_ENABLED.u_ns_ovcsts.credit_cntr_q[4:0], 1'b0, 1'b0, 1'b0 }  }, '{ '{  u_ns_io_meshstop.u_ns_router_0_14.u_ns_router.u_west_outblk.G_OUT_PORT_ENB.G_OVCSTS[3].G_OVC_ENABLED.u_ns_ovcsts.credit_cntr_q[4:0], u_ns_io_meshstop.u_ns_router_0_14.u_ns_router.u_west_outblk.G_OUT_PORT_ENB.G_OVCSTS[2].G_OVC_ENABLED.u_ns_ovcsts.credit_cntr_q[4:0], 1'b0, u_ns_io_meshstop.u_ns_router_0_14.u_ns_router.u_west_outblk.G_OUT_PORT_ENB.G_OVCSTS[0].G_OVC_ENABLED.u_ns_ovcsts.credit_cntr_q[4:0] }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  u_ns_io_meshstop.u_ns_router_0_14.u_ns_router.u_south_outblk.G_OUT_PORT_ENB.G_OVCSTS[3].G_OVC_ENABLED.u_ns_ovcsts.credit_cntr_q[4:0], 1'b0, 1'b0, u_ns_io_meshstop.u_ns_router_0_14.u_ns_router.u_south_outblk.G_OUT_PORT_ENB.G_OVCSTS[0].G_OVC_ENABLED.u_ns_ovcsts.credit_cntr_q[4:0] }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  u_ns_io_meshstop.u_ns_router_0_14.u_ns_router.u_host3_outblk.G_OUT_PORT_ENB.G_OVCSTS[3].G_OVC_ENABLED.u_ns_ovcsts.credit_cntr_q[4:0], 1'b0, 1'b0, 1'b0 }, '{  1'b0, u_ns_io_meshstop.u_ns_router_0_14.u_ns_router.u_host2_outblk.G_OUT_PORT_ENB.G_OVCSTS[2].G_OVC_ENABLED.u_ns_ovcsts.credit_cntr_q[4:0], 1'b0, u_ns_io_meshstop.u_ns_router_0_14.u_ns_router.u_host2_outblk.G_OUT_PORT_ENB.G_OVCSTS[0].G_OVC_ENABLED.u_ns_ovcsts.credit_cntr_q[4:0] }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  u_ns_io_meshstop.u_ns_router_0_14.u_ns_router.u_host0_outblk.G_OUT_PORT_ENB.G_OVCSTS[3].G_OVC_ENABLED.u_ns_ovcsts.credit_cntr_q[4:0], 1'b0, 1'b0, 1'b0 }  }  } ;


   assign  inport_rd_pntr_bin_q = '{ '{ '{  u_ns_io_meshstop.u_ns_router_7_14.u_ns_router.u_west_inblk.G_IN_PORT_ENB.G_IVCBUF_IVCCTRL[3].G_IVC_ENB.G_IVCBUF_SYNC.u_ivcbuf_sync_dat.rd_pntr_bin_q[3:0], 1'b0, 1'b0, u_ns_io_meshstop.u_ns_router_7_14.u_ns_router.u_west_inblk.G_IN_PORT_ENB.G_IVCBUF_IVCCTRL[0].G_IVC_ENB.G_IVCBUF_SYNC.u_ivcbuf_sync_dat.rd_pntr_bin_q[3:0] }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  u_ns_io_meshstop.u_ns_router_7_14.u_ns_router.u_south_inblk.G_IN_PORT_ENB.G_IVCBUF_IVCCTRL[3].G_IVC_ENB.G_IVCBUF_SYNC.u_ivcbuf_sync_dat.rd_pntr_bin_q[3:0], 1'b0, u_ns_io_meshstop.u_ns_router_7_14.u_ns_router.u_south_inblk.G_IN_PORT_ENB.G_IVCBUF_IVCCTRL[1].G_IVC_ENB.G_IVCBUF_SYNC.u_ivcbuf_sync_dat.rd_pntr_bin_q[1:0], u_ns_io_meshstop.u_ns_router_7_14.u_ns_router.u_south_inblk.G_IN_PORT_ENB.G_IVCBUF_IVCCTRL[0].G_IVC_ENB.G_IVCBUF_SYNC.u_ivcbuf_sync_dat.rd_pntr_bin_q[3:0] }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  u_ns_io_meshstop.u_ns_router_7_14.u_ns_router.u_host2_inblk.G_IN_PORT_ENB.G_IVCBUF_IVCCTRL[3].G_IVC_ENB.G_IVCBUF_SYNC.u_ivcbuf_sync_dat.rd_pntr_bin_q[1:0], u_ns_io_meshstop.u_ns_router_7_14.u_ns_router.u_host2_inblk.G_IN_PORT_ENB.G_IVCBUF_IVCCTRL[2].G_IVC_ENB.G_IVCBUF_SYNC.u_ivcbuf_sync_dat.rd_pntr_bin_q[1:0], 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }  }, '{ '{  u_ns_io_meshstop.u_ns_router_6_14.u_ns_router.u_west_inblk.G_IN_PORT_ENB.G_IVCBUF_IVCCTRL[3].G_IVC_ENB.G_IVCBUF_SYNC.u_ivcbuf_sync_dat.rd_pntr_bin_q[3:0], 1'b0, 1'b0, u_ns_io_meshstop.u_ns_router_6_14.u_ns_router.u_west_inblk.G_IN_PORT_ENB.G_IVCBUF_IVCCTRL[0].G_IVC_ENB.G_IVCBUF_SYNC.u_ivcbuf_sync_dat.rd_pntr_bin_q[3:0] }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  u_ns_io_meshstop.u_ns_router_6_14.u_ns_router.u_south_inblk.G_IN_PORT_ENB.G_IVCBUF_IVCCTRL[3].G_IVC_ENB.G_IVCBUF_SYNC.u_ivcbuf_sync_dat.rd_pntr_bin_q[3:0], u_ns_io_meshstop.u_ns_router_6_14.u_ns_router.u_south_inblk.G_IN_PORT_ENB.G_IVCBUF_IVCCTRL[2].G_IVC_ENB.G_IVCBUF_SYNC.u_ivcbuf_sync_dat.rd_pntr_bin_q[1:0], 1'b0, u_ns_io_meshstop.u_ns_router_6_14.u_ns_router.u_south_inblk.G_IN_PORT_ENB.G_IVCBUF_IVCCTRL[0].G_IVC_ENB.G_IVCBUF_SYNC.u_ivcbuf_sync_dat.rd_pntr_bin_q[3:0] }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  u_ns_io_meshstop.u_ns_router_6_14.u_ns_router.u_host2_inblk.G_IN_PORT_ENB.G_IVCBUF_IVCCTRL[3].G_IVC_ENB.G_IVCBUF_SYNC.u_ivcbuf_sync_dat.rd_pntr_bin_q[1:0], 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, u_ns_io_meshstop.u_ns_router_6_14.u_ns_router.u_host1_inblk.G_IN_PORT_ENB.G_IVCBUF_IVCCTRL[0].G_IVC_ENB.G_IVCBUF_SYNC.u_ivcbuf_sync_dat.rd_pntr_bin_q[1:0] }, '{  1'b0, 1'b0, 1'b0, 1'b0 }  }, '{ '{  u_ns_io_meshstop.u_ns_router_5_14.u_ns_router.u_west_inblk.G_IN_PORT_ENB.G_IVCBUF_IVCCTRL[3].G_IVC_ENB.G_IVCBUF_SYNC.u_ivcbuf_sync_dat.rd_pntr_bin_q[3:0], 1'b0, 1'b0, u_ns_io_meshstop.u_ns_router_5_14.u_ns_router.u_west_inblk.G_IN_PORT_ENB.G_IVCBUF_IVCCTRL[0].G_IVC_ENB.G_IVCBUF_SYNC.u_ivcbuf_sync_dat.rd_pntr_bin_q[3:0] }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, u_ns_io_meshstop.u_ns_router_5_14.u_ns_router.u_south_inblk.G_IN_PORT_ENB.G_IVCBUF_IVCCTRL[0].G_IVC_ENB.G_IVCBUF_SYNC.u_ivcbuf_sync_dat.rd_pntr_bin_q[3:0] }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, u_ns_io_meshstop.u_ns_router_5_14.u_ns_router.u_host3_inblk.G_IN_PORT_ENB.G_IVCBUF_IVCCTRL[1].G_IVC_ENB.G_IVCBUF_SYNC.u_ivcbuf_sync_dat.rd_pntr_bin_q[1:0], 1'b0 }, '{  u_ns_io_meshstop.u_ns_router_5_14.u_ns_router.u_host2_inblk.G_IN_PORT_ENB.G_IVCBUF_IVCCTRL[3].G_IVC_ENB.G_IVCBUF_SYNC.u_ivcbuf_sync_dat.rd_pntr_bin_q[1:0], u_ns_io_meshstop.u_ns_router_5_14.u_ns_router.u_host2_inblk.G_IN_PORT_ENB.G_IVCBUF_IVCCTRL[2].G_IVC_ENB.G_IVCBUF_SYNC.u_ivcbuf_sync_dat.rd_pntr_bin_q[1:0], 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }  }, '{ '{  u_ns_io_meshstop.u_ns_router_4_14.u_ns_router.u_west_inblk.G_IN_PORT_ENB.G_IVCBUF_IVCCTRL[3].G_IVC_ENB.G_IVCBUF_SYNC.u_ivcbuf_sync_dat.rd_pntr_bin_q[3:0], 1'b0, 1'b0, u_ns_io_meshstop.u_ns_router_4_14.u_ns_router.u_west_inblk.G_IN_PORT_ENB.G_IVCBUF_IVCCTRL[0].G_IVC_ENB.G_IVCBUF_SYNC.u_ivcbuf_sync_dat.rd_pntr_bin_q[3:0] }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, u_ns_io_meshstop.u_ns_router_4_14.u_ns_router.u_south_inblk.G_IN_PORT_ENB.G_IVCBUF_IVCCTRL[0].G_IVC_ENB.G_IVCBUF_SYNC.u_ivcbuf_sync_dat.rd_pntr_bin_q[3:0] }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, u_ns_io_meshstop.u_ns_router_4_14.u_ns_router.u_host3_inblk.G_IN_PORT_ENB.G_IVCBUF_IVCCTRL[1].G_IVC_ENB.G_IVCBUF_SYNC.u_ivcbuf_sync_dat.rd_pntr_bin_q[1:0], 1'b0 }, '{  u_ns_io_meshstop.u_ns_router_4_14.u_ns_router.u_host2_inblk.G_IN_PORT_ENB.G_IVCBUF_IVCCTRL[3].G_IVC_ENB.G_IVCBUF_SYNC.u_ivcbuf_sync_dat.rd_pntr_bin_q[1:0], 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }  }, '{ '{  u_ns_io_meshstop.u_ns_router_3_14.u_ns_router.u_west_inblk.G_IN_PORT_ENB.G_IVCBUF_IVCCTRL[3].G_IVC_ENB.G_IVCBUF_SYNC.u_ivcbuf_sync_dat.rd_pntr_bin_q[3:0], 1'b0, 1'b0, u_ns_io_meshstop.u_ns_router_3_14.u_ns_router.u_west_inblk.G_IN_PORT_ENB.G_IVCBUF_IVCCTRL[0].G_IVC_ENB.G_IVCBUF_SYNC.u_ivcbuf_sync_dat.rd_pntr_bin_q[3:0] }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  u_ns_io_meshstop.u_ns_router_3_14.u_ns_router.u_south_inblk.G_IN_PORT_ENB.G_IVCBUF_IVCCTRL[3].G_IVC_ENB.G_IVCBUF_SYNC.u_ivcbuf_sync_dat.rd_pntr_bin_q[3:0], u_ns_io_meshstop.u_ns_router_3_14.u_ns_router.u_south_inblk.G_IN_PORT_ENB.G_IVCBUF_IVCCTRL[2].G_IVC_ENB.G_IVCBUF_SYNC.u_ivcbuf_sync_dat.rd_pntr_bin_q[3:0], 1'b0, u_ns_io_meshstop.u_ns_router_3_14.u_ns_router.u_south_inblk.G_IN_PORT_ENB.G_IVCBUF_IVCCTRL[0].G_IVC_ENB.G_IVCBUF_SYNC.u_ivcbuf_sync_dat.rd_pntr_bin_q[3:0] }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  u_ns_io_meshstop.u_ns_router_3_14.u_ns_router.u_host2_inblk.G_IN_PORT_ENB.G_IVCBUF_IVCCTRL[3].G_IVC_ENB.G_IVCBUF_SYNC.u_ivcbuf_sync_dat.rd_pntr_bin_q[1:0], 1'b0, 1'b0, 1'b0 }, '{  1'b0, u_ns_io_meshstop.u_ns_router_3_14.u_ns_router.u_host1_inblk.G_IN_PORT_ENB.G_IVCBUF_IVCCTRL[2].G_IVC_ENB.G_IVCBUF_SYNC.u_ivcbuf_sync_dat.rd_pntr_bin_q[1:0], 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, u_ns_io_meshstop.u_ns_router_3_14.u_ns_router.u_host0_inblk.G_IN_PORT_ENB.G_IVCBUF_IVCCTRL[0].G_IVC_ENB.G_IVCBUF_SYNC.u_ivcbuf_sync_dat.rd_pntr_bin_q[1:0] }  }, '{ '{  u_ns_io_meshstop.u_ns_router_2_14.u_ns_router.u_west_inblk.G_IN_PORT_ENB.G_IVCBUF_IVCCTRL[3].G_IVC_ENB.G_IVCBUF_SYNC.u_ivcbuf_sync_dat.rd_pntr_bin_q[3:0], 1'b0, u_ns_io_meshstop.u_ns_router_2_14.u_ns_router.u_west_inblk.G_IN_PORT_ENB.G_IVCBUF_IVCCTRL[1].G_IVC_ENB.G_IVCBUF_SYNC.u_ivcbuf_sync_dat.rd_pntr_bin_q[3:0], u_ns_io_meshstop.u_ns_router_2_14.u_ns_router.u_west_inblk.G_IN_PORT_ENB.G_IVCBUF_IVCCTRL[0].G_IVC_ENB.G_IVCBUF_SYNC.u_ivcbuf_sync_dat.rd_pntr_bin_q[3:0] }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  u_ns_io_meshstop.u_ns_router_2_14.u_ns_router.u_south_inblk.G_IN_PORT_ENB.G_IVCBUF_IVCCTRL[3].G_IVC_ENB.G_IVCBUF_SYNC.u_ivcbuf_sync_dat.rd_pntr_bin_q[3:0], u_ns_io_meshstop.u_ns_router_2_14.u_ns_router.u_south_inblk.G_IN_PORT_ENB.G_IVCBUF_IVCCTRL[2].G_IVC_ENB.G_IVCBUF_SYNC.u_ivcbuf_sync_dat.rd_pntr_bin_q[3:0], u_ns_io_meshstop.u_ns_router_2_14.u_ns_router.u_south_inblk.G_IN_PORT_ENB.G_IVCBUF_IVCCTRL[1].G_IVC_ENB.G_IVCBUF_SYNC.u_ivcbuf_sync_dat.rd_pntr_bin_q[3:0], u_ns_io_meshstop.u_ns_router_2_14.u_ns_router.u_south_inblk.G_IN_PORT_ENB.G_IVCBUF_IVCCTRL[0].G_IVC_ENB.G_IVCBUF_SYNC.u_ivcbuf_sync_dat.rd_pntr_bin_q[3:0] }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  u_ns_io_meshstop.u_ns_router_2_14.u_ns_router.u_host2_inblk.G_IN_PORT_ENB.G_IVCBUF_IVCCTRL[3].G_IVC_ENB.G_IVCBUF_SYNC.u_ivcbuf_sync_dat.rd_pntr_bin_q[1:0], 1'b0, 1'b0, 1'b0 }, '{  1'b0, u_ns_io_meshstop.u_ns_router_2_14.u_ns_router.u_host1_inblk.G_IN_PORT_ENB.G_IVCBUF_IVCCTRL[2].G_IVC_ENB.G_IVCBUF_SYNC.u_ivcbuf_sync_dat.rd_pntr_bin_q[1:0], 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, u_ns_io_meshstop.u_ns_router_2_14.u_ns_router.u_host0_inblk.G_IN_PORT_ENB.G_IVCBUF_IVCCTRL[0].G_IVC_ENB.G_IVCBUF_SYNC.u_ivcbuf_sync_dat.rd_pntr_bin_q[1:0] }  }, '{ '{  u_ns_io_meshstop.u_ns_router_1_14.u_ns_router.u_west_inblk.G_IN_PORT_ENB.G_IVCBUF_IVCCTRL[3].G_IVC_ENB.G_IVCBUF_SYNC.u_ivcbuf_sync_dat.rd_pntr_bin_q[3:0], u_ns_io_meshstop.u_ns_router_1_14.u_ns_router.u_west_inblk.G_IN_PORT_ENB.G_IVCBUF_IVCCTRL[2].G_IVC_ENB.G_IVCBUF_SYNC.u_ivcbuf_sync_dat.rd_pntr_bin_q[3:0], 1'b0, u_ns_io_meshstop.u_ns_router_1_14.u_ns_router.u_west_inblk.G_IN_PORT_ENB.G_IVCBUF_IVCCTRL[0].G_IVC_ENB.G_IVCBUF_SYNC.u_ivcbuf_sync_dat.rd_pntr_bin_q[3:0] }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  u_ns_io_meshstop.u_ns_router_1_14.u_ns_router.u_south_inblk.G_IN_PORT_ENB.G_IVCBUF_IVCCTRL[3].G_IVC_ENB.G_IVCBUF_SYNC.u_ivcbuf_sync_dat.rd_pntr_bin_q[3:0], u_ns_io_meshstop.u_ns_router_1_14.u_ns_router.u_south_inblk.G_IN_PORT_ENB.G_IVCBUF_IVCCTRL[2].G_IVC_ENB.G_IVCBUF_SYNC.u_ivcbuf_sync_dat.rd_pntr_bin_q[3:0], 1'b0, u_ns_io_meshstop.u_ns_router_1_14.u_ns_router.u_south_inblk.G_IN_PORT_ENB.G_IVCBUF_IVCCTRL[0].G_IVC_ENB.G_IVCBUF_SYNC.u_ivcbuf_sync_dat.rd_pntr_bin_q[3:0] }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, u_ns_io_meshstop.u_ns_router_1_14.u_ns_router.u_host3_inblk.G_IN_PORT_ENB.G_IVCBUF_IVCCTRL[2].G_IVC_ENB.G_IVCBUF_SYNC.u_ivcbuf_sync_dat.rd_pntr_bin_q[1:0], 1'b0, 1'b0 }, '{  u_ns_io_meshstop.u_ns_router_1_14.u_ns_router.u_host2_inblk.G_IN_PORT_ENB.G_IVCBUF_IVCCTRL[3].G_IVC_ENB.G_IVCBUF_SYNC.u_ivcbuf_sync_dat.rd_pntr_bin_q[1:0], 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, u_ns_io_meshstop.u_ns_router_1_14.u_ns_router.u_host0_inblk.G_IN_PORT_ENB.G_IVCBUF_IVCCTRL[0].G_IVC_ENB.G_IVCBUF_SYNC.u_ivcbuf_sync_dat.rd_pntr_bin_q[1:0] }  }, '{ '{  u_ns_io_meshstop.u_ns_router_0_14.u_ns_router.u_west_inblk.G_IN_PORT_ENB.G_IVCBUF_IVCCTRL[3].G_IVC_ENB.G_IVCBUF_SYNC.u_ivcbuf_sync_dat.rd_pntr_bin_q[3:0], u_ns_io_meshstop.u_ns_router_0_14.u_ns_router.u_west_inblk.G_IN_PORT_ENB.G_IVCBUF_IVCCTRL[2].G_IVC_ENB.G_IVCBUF_SYNC.u_ivcbuf_sync_dat.rd_pntr_bin_q[3:0], 1'b0, u_ns_io_meshstop.u_ns_router_0_14.u_ns_router.u_west_inblk.G_IN_PORT_ENB.G_IVCBUF_IVCCTRL[0].G_IVC_ENB.G_IVCBUF_SYNC.u_ivcbuf_sync_dat.rd_pntr_bin_q[3:0] }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  u_ns_io_meshstop.u_ns_router_0_14.u_ns_router.u_south_inblk.G_IN_PORT_ENB.G_IVCBUF_IVCCTRL[3].G_IVC_ENB.G_IVCBUF_SYNC.u_ivcbuf_sync_dat.rd_pntr_bin_q[3:0], u_ns_io_meshstop.u_ns_router_0_14.u_ns_router.u_south_inblk.G_IN_PORT_ENB.G_IVCBUF_IVCCTRL[2].G_IVC_ENB.G_IVCBUF_SYNC.u_ivcbuf_sync_dat.rd_pntr_bin_q[3:0], 1'b0, u_ns_io_meshstop.u_ns_router_0_14.u_ns_router.u_south_inblk.G_IN_PORT_ENB.G_IVCBUF_IVCCTRL[0].G_IVC_ENB.G_IVCBUF_SYNC.u_ivcbuf_sync_dat.rd_pntr_bin_q[3:0] }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, u_ns_io_meshstop.u_ns_router_0_14.u_ns_router.u_host3_inblk.G_IN_PORT_ENB.G_IVCBUF_IVCCTRL[2].G_IVC_ENB.G_IVCBUF_SYNC.u_ivcbuf_sync_dat.rd_pntr_bin_q[1:0], 1'b0, 1'b0 }, '{  u_ns_io_meshstop.u_ns_router_0_14.u_ns_router.u_host2_inblk.G_IN_PORT_ENB.G_IVCBUF_IVCCTRL[3].G_IVC_ENB.G_IVCBUF_SYNC.u_ivcbuf_sync_dat.rd_pntr_bin_q[1:0], 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, u_ns_io_meshstop.u_ns_router_0_14.u_ns_router.u_host0_inblk.G_IN_PORT_ENB.G_IVCBUF_IVCCTRL[0].G_IVC_ENB.G_IVCBUF_SYNC.u_ivcbuf_sync_dat.rd_pntr_bin_q[1:0] }  }  } ;

   assign  inport_wr_pntr_bin_q = '{ '{ '{  u_ns_io_meshstop.u_ns_router_7_14.u_ns_router.u_west_inblk.G_IN_PORT_ENB.G_IVCBUF_IVCCTRL[3].G_IVC_ENB.G_IVCBUF_SYNC.u_ivcbuf_sync_dat.wr_pntr_bin_q[3:0], 1'b0, 1'b0, u_ns_io_meshstop.u_ns_router_7_14.u_ns_router.u_west_inblk.G_IN_PORT_ENB.G_IVCBUF_IVCCTRL[0].G_IVC_ENB.G_IVCBUF_SYNC.u_ivcbuf_sync_dat.wr_pntr_bin_q[3:0] }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  u_ns_io_meshstop.u_ns_router_7_14.u_ns_router.u_south_inblk.G_IN_PORT_ENB.G_IVCBUF_IVCCTRL[3].G_IVC_ENB.G_IVCBUF_SYNC.u_ivcbuf_sync_dat.wr_pntr_bin_q[3:0], 1'b0, u_ns_io_meshstop.u_ns_router_7_14.u_ns_router.u_south_inblk.G_IN_PORT_ENB.G_IVCBUF_IVCCTRL[1].G_IVC_ENB.G_IVCBUF_SYNC.u_ivcbuf_sync_dat.wr_pntr_bin_q[1:0], u_ns_io_meshstop.u_ns_router_7_14.u_ns_router.u_south_inblk.G_IN_PORT_ENB.G_IVCBUF_IVCCTRL[0].G_IVC_ENB.G_IVCBUF_SYNC.u_ivcbuf_sync_dat.wr_pntr_bin_q[3:0] }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  u_ns_io_meshstop.u_ns_router_7_14.u_ns_router.u_host2_inblk.G_IN_PORT_ENB.G_IVCBUF_IVCCTRL[3].G_IVC_ENB.G_IVCBUF_SYNC.u_ivcbuf_sync_dat.wr_pntr_bin_q[1:0], u_ns_io_meshstop.u_ns_router_7_14.u_ns_router.u_host2_inblk.G_IN_PORT_ENB.G_IVCBUF_IVCCTRL[2].G_IVC_ENB.G_IVCBUF_SYNC.u_ivcbuf_sync_dat.wr_pntr_bin_q[1:0], 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }  }, '{ '{  u_ns_io_meshstop.u_ns_router_6_14.u_ns_router.u_west_inblk.G_IN_PORT_ENB.G_IVCBUF_IVCCTRL[3].G_IVC_ENB.G_IVCBUF_SYNC.u_ivcbuf_sync_dat.wr_pntr_bin_q[3:0], 1'b0, 1'b0, u_ns_io_meshstop.u_ns_router_6_14.u_ns_router.u_west_inblk.G_IN_PORT_ENB.G_IVCBUF_IVCCTRL[0].G_IVC_ENB.G_IVCBUF_SYNC.u_ivcbuf_sync_dat.wr_pntr_bin_q[3:0] }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  u_ns_io_meshstop.u_ns_router_6_14.u_ns_router.u_south_inblk.G_IN_PORT_ENB.G_IVCBUF_IVCCTRL[3].G_IVC_ENB.G_IVCBUF_SYNC.u_ivcbuf_sync_dat.wr_pntr_bin_q[3:0], u_ns_io_meshstop.u_ns_router_6_14.u_ns_router.u_south_inblk.G_IN_PORT_ENB.G_IVCBUF_IVCCTRL[2].G_IVC_ENB.G_IVCBUF_SYNC.u_ivcbuf_sync_dat.wr_pntr_bin_q[1:0], 1'b0, u_ns_io_meshstop.u_ns_router_6_14.u_ns_router.u_south_inblk.G_IN_PORT_ENB.G_IVCBUF_IVCCTRL[0].G_IVC_ENB.G_IVCBUF_SYNC.u_ivcbuf_sync_dat.wr_pntr_bin_q[3:0] }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  u_ns_io_meshstop.u_ns_router_6_14.u_ns_router.u_host2_inblk.G_IN_PORT_ENB.G_IVCBUF_IVCCTRL[3].G_IVC_ENB.G_IVCBUF_SYNC.u_ivcbuf_sync_dat.wr_pntr_bin_q[1:0], 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, u_ns_io_meshstop.u_ns_router_6_14.u_ns_router.u_host1_inblk.G_IN_PORT_ENB.G_IVCBUF_IVCCTRL[0].G_IVC_ENB.G_IVCBUF_SYNC.u_ivcbuf_sync_dat.wr_pntr_bin_q[1:0] }, '{  1'b0, 1'b0, 1'b0, 1'b0 }  }, '{ '{  u_ns_io_meshstop.u_ns_router_5_14.u_ns_router.u_west_inblk.G_IN_PORT_ENB.G_IVCBUF_IVCCTRL[3].G_IVC_ENB.G_IVCBUF_SYNC.u_ivcbuf_sync_dat.wr_pntr_bin_q[3:0], 1'b0, 1'b0, u_ns_io_meshstop.u_ns_router_5_14.u_ns_router.u_west_inblk.G_IN_PORT_ENB.G_IVCBUF_IVCCTRL[0].G_IVC_ENB.G_IVCBUF_SYNC.u_ivcbuf_sync_dat.wr_pntr_bin_q[3:0] }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, u_ns_io_meshstop.u_ns_router_5_14.u_ns_router.u_south_inblk.G_IN_PORT_ENB.G_IVCBUF_IVCCTRL[0].G_IVC_ENB.G_IVCBUF_SYNC.u_ivcbuf_sync_dat.wr_pntr_bin_q[3:0] }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, u_ns_io_meshstop.u_ns_router_5_14.u_ns_router.u_host3_inblk.G_IN_PORT_ENB.G_IVCBUF_IVCCTRL[1].G_IVC_ENB.G_IVCBUF_SYNC.u_ivcbuf_sync_dat.wr_pntr_bin_q[1:0], 1'b0 }, '{  u_ns_io_meshstop.u_ns_router_5_14.u_ns_router.u_host2_inblk.G_IN_PORT_ENB.G_IVCBUF_IVCCTRL[3].G_IVC_ENB.G_IVCBUF_SYNC.u_ivcbuf_sync_dat.wr_pntr_bin_q[1:0], u_ns_io_meshstop.u_ns_router_5_14.u_ns_router.u_host2_inblk.G_IN_PORT_ENB.G_IVCBUF_IVCCTRL[2].G_IVC_ENB.G_IVCBUF_SYNC.u_ivcbuf_sync_dat.wr_pntr_bin_q[1:0], 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }  }, '{ '{  u_ns_io_meshstop.u_ns_router_4_14.u_ns_router.u_west_inblk.G_IN_PORT_ENB.G_IVCBUF_IVCCTRL[3].G_IVC_ENB.G_IVCBUF_SYNC.u_ivcbuf_sync_dat.wr_pntr_bin_q[3:0], 1'b0, 1'b0, u_ns_io_meshstop.u_ns_router_4_14.u_ns_router.u_west_inblk.G_IN_PORT_ENB.G_IVCBUF_IVCCTRL[0].G_IVC_ENB.G_IVCBUF_SYNC.u_ivcbuf_sync_dat.wr_pntr_bin_q[3:0] }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, u_ns_io_meshstop.u_ns_router_4_14.u_ns_router.u_south_inblk.G_IN_PORT_ENB.G_IVCBUF_IVCCTRL[0].G_IVC_ENB.G_IVCBUF_SYNC.u_ivcbuf_sync_dat.wr_pntr_bin_q[3:0] }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, u_ns_io_meshstop.u_ns_router_4_14.u_ns_router.u_host3_inblk.G_IN_PORT_ENB.G_IVCBUF_IVCCTRL[1].G_IVC_ENB.G_IVCBUF_SYNC.u_ivcbuf_sync_dat.wr_pntr_bin_q[1:0], 1'b0 }, '{  u_ns_io_meshstop.u_ns_router_4_14.u_ns_router.u_host2_inblk.G_IN_PORT_ENB.G_IVCBUF_IVCCTRL[3].G_IVC_ENB.G_IVCBUF_SYNC.u_ivcbuf_sync_dat.wr_pntr_bin_q[1:0], 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }  }, '{ '{  u_ns_io_meshstop.u_ns_router_3_14.u_ns_router.u_west_inblk.G_IN_PORT_ENB.G_IVCBUF_IVCCTRL[3].G_IVC_ENB.G_IVCBUF_SYNC.u_ivcbuf_sync_dat.wr_pntr_bin_q[3:0], 1'b0, 1'b0, u_ns_io_meshstop.u_ns_router_3_14.u_ns_router.u_west_inblk.G_IN_PORT_ENB.G_IVCBUF_IVCCTRL[0].G_IVC_ENB.G_IVCBUF_SYNC.u_ivcbuf_sync_dat.wr_pntr_bin_q[3:0] }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  u_ns_io_meshstop.u_ns_router_3_14.u_ns_router.u_south_inblk.G_IN_PORT_ENB.G_IVCBUF_IVCCTRL[3].G_IVC_ENB.G_IVCBUF_SYNC.u_ivcbuf_sync_dat.wr_pntr_bin_q[3:0], u_ns_io_meshstop.u_ns_router_3_14.u_ns_router.u_south_inblk.G_IN_PORT_ENB.G_IVCBUF_IVCCTRL[2].G_IVC_ENB.G_IVCBUF_SYNC.u_ivcbuf_sync_dat.wr_pntr_bin_q[3:0], 1'b0, u_ns_io_meshstop.u_ns_router_3_14.u_ns_router.u_south_inblk.G_IN_PORT_ENB.G_IVCBUF_IVCCTRL[0].G_IVC_ENB.G_IVCBUF_SYNC.u_ivcbuf_sync_dat.wr_pntr_bin_q[3:0] }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  u_ns_io_meshstop.u_ns_router_3_14.u_ns_router.u_host2_inblk.G_IN_PORT_ENB.G_IVCBUF_IVCCTRL[3].G_IVC_ENB.G_IVCBUF_SYNC.u_ivcbuf_sync_dat.wr_pntr_bin_q[1:0], 1'b0, 1'b0, 1'b0 }, '{  1'b0, u_ns_io_meshstop.u_ns_router_3_14.u_ns_router.u_host1_inblk.G_IN_PORT_ENB.G_IVCBUF_IVCCTRL[2].G_IVC_ENB.G_IVCBUF_SYNC.u_ivcbuf_sync_dat.wr_pntr_bin_q[1:0], 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, u_ns_io_meshstop.u_ns_router_3_14.u_ns_router.u_host0_inblk.G_IN_PORT_ENB.G_IVCBUF_IVCCTRL[0].G_IVC_ENB.G_IVCBUF_SYNC.u_ivcbuf_sync_dat.wr_pntr_bin_q[1:0] }  }, '{ '{  u_ns_io_meshstop.u_ns_router_2_14.u_ns_router.u_west_inblk.G_IN_PORT_ENB.G_IVCBUF_IVCCTRL[3].G_IVC_ENB.G_IVCBUF_SYNC.u_ivcbuf_sync_dat.wr_pntr_bin_q[3:0], 1'b0, u_ns_io_meshstop.u_ns_router_2_14.u_ns_router.u_west_inblk.G_IN_PORT_ENB.G_IVCBUF_IVCCTRL[1].G_IVC_ENB.G_IVCBUF_SYNC.u_ivcbuf_sync_dat.wr_pntr_bin_q[3:0], u_ns_io_meshstop.u_ns_router_2_14.u_ns_router.u_west_inblk.G_IN_PORT_ENB.G_IVCBUF_IVCCTRL[0].G_IVC_ENB.G_IVCBUF_SYNC.u_ivcbuf_sync_dat.wr_pntr_bin_q[3:0] }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  u_ns_io_meshstop.u_ns_router_2_14.u_ns_router.u_south_inblk.G_IN_PORT_ENB.G_IVCBUF_IVCCTRL[3].G_IVC_ENB.G_IVCBUF_SYNC.u_ivcbuf_sync_dat.wr_pntr_bin_q[3:0], u_ns_io_meshstop.u_ns_router_2_14.u_ns_router.u_south_inblk.G_IN_PORT_ENB.G_IVCBUF_IVCCTRL[2].G_IVC_ENB.G_IVCBUF_SYNC.u_ivcbuf_sync_dat.wr_pntr_bin_q[3:0], u_ns_io_meshstop.u_ns_router_2_14.u_ns_router.u_south_inblk.G_IN_PORT_ENB.G_IVCBUF_IVCCTRL[1].G_IVC_ENB.G_IVCBUF_SYNC.u_ivcbuf_sync_dat.wr_pntr_bin_q[3:0], u_ns_io_meshstop.u_ns_router_2_14.u_ns_router.u_south_inblk.G_IN_PORT_ENB.G_IVCBUF_IVCCTRL[0].G_IVC_ENB.G_IVCBUF_SYNC.u_ivcbuf_sync_dat.wr_pntr_bin_q[3:0] }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  u_ns_io_meshstop.u_ns_router_2_14.u_ns_router.u_host2_inblk.G_IN_PORT_ENB.G_IVCBUF_IVCCTRL[3].G_IVC_ENB.G_IVCBUF_SYNC.u_ivcbuf_sync_dat.wr_pntr_bin_q[1:0], 1'b0, 1'b0, 1'b0 }, '{  1'b0, u_ns_io_meshstop.u_ns_router_2_14.u_ns_router.u_host1_inblk.G_IN_PORT_ENB.G_IVCBUF_IVCCTRL[2].G_IVC_ENB.G_IVCBUF_SYNC.u_ivcbuf_sync_dat.wr_pntr_bin_q[1:0], 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, u_ns_io_meshstop.u_ns_router_2_14.u_ns_router.u_host0_inblk.G_IN_PORT_ENB.G_IVCBUF_IVCCTRL[0].G_IVC_ENB.G_IVCBUF_SYNC.u_ivcbuf_sync_dat.wr_pntr_bin_q[1:0] }  }, '{ '{  u_ns_io_meshstop.u_ns_router_1_14.u_ns_router.u_west_inblk.G_IN_PORT_ENB.G_IVCBUF_IVCCTRL[3].G_IVC_ENB.G_IVCBUF_SYNC.u_ivcbuf_sync_dat.wr_pntr_bin_q[3:0], u_ns_io_meshstop.u_ns_router_1_14.u_ns_router.u_west_inblk.G_IN_PORT_ENB.G_IVCBUF_IVCCTRL[2].G_IVC_ENB.G_IVCBUF_SYNC.u_ivcbuf_sync_dat.wr_pntr_bin_q[3:0], 1'b0, u_ns_io_meshstop.u_ns_router_1_14.u_ns_router.u_west_inblk.G_IN_PORT_ENB.G_IVCBUF_IVCCTRL[0].G_IVC_ENB.G_IVCBUF_SYNC.u_ivcbuf_sync_dat.wr_pntr_bin_q[3:0] }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  u_ns_io_meshstop.u_ns_router_1_14.u_ns_router.u_south_inblk.G_IN_PORT_ENB.G_IVCBUF_IVCCTRL[3].G_IVC_ENB.G_IVCBUF_SYNC.u_ivcbuf_sync_dat.wr_pntr_bin_q[3:0], u_ns_io_meshstop.u_ns_router_1_14.u_ns_router.u_south_inblk.G_IN_PORT_ENB.G_IVCBUF_IVCCTRL[2].G_IVC_ENB.G_IVCBUF_SYNC.u_ivcbuf_sync_dat.wr_pntr_bin_q[3:0], 1'b0, u_ns_io_meshstop.u_ns_router_1_14.u_ns_router.u_south_inblk.G_IN_PORT_ENB.G_IVCBUF_IVCCTRL[0].G_IVC_ENB.G_IVCBUF_SYNC.u_ivcbuf_sync_dat.wr_pntr_bin_q[3:0] }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, u_ns_io_meshstop.u_ns_router_1_14.u_ns_router.u_host3_inblk.G_IN_PORT_ENB.G_IVCBUF_IVCCTRL[2].G_IVC_ENB.G_IVCBUF_SYNC.u_ivcbuf_sync_dat.wr_pntr_bin_q[1:0], 1'b0, 1'b0 }, '{  u_ns_io_meshstop.u_ns_router_1_14.u_ns_router.u_host2_inblk.G_IN_PORT_ENB.G_IVCBUF_IVCCTRL[3].G_IVC_ENB.G_IVCBUF_SYNC.u_ivcbuf_sync_dat.wr_pntr_bin_q[1:0], 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, u_ns_io_meshstop.u_ns_router_1_14.u_ns_router.u_host0_inblk.G_IN_PORT_ENB.G_IVCBUF_IVCCTRL[0].G_IVC_ENB.G_IVCBUF_SYNC.u_ivcbuf_sync_dat.wr_pntr_bin_q[1:0] }  }, '{ '{  u_ns_io_meshstop.u_ns_router_0_14.u_ns_router.u_west_inblk.G_IN_PORT_ENB.G_IVCBUF_IVCCTRL[3].G_IVC_ENB.G_IVCBUF_SYNC.u_ivcbuf_sync_dat.wr_pntr_bin_q[3:0], u_ns_io_meshstop.u_ns_router_0_14.u_ns_router.u_west_inblk.G_IN_PORT_ENB.G_IVCBUF_IVCCTRL[2].G_IVC_ENB.G_IVCBUF_SYNC.u_ivcbuf_sync_dat.wr_pntr_bin_q[3:0], 1'b0, u_ns_io_meshstop.u_ns_router_0_14.u_ns_router.u_west_inblk.G_IN_PORT_ENB.G_IVCBUF_IVCCTRL[0].G_IVC_ENB.G_IVCBUF_SYNC.u_ivcbuf_sync_dat.wr_pntr_bin_q[3:0] }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  u_ns_io_meshstop.u_ns_router_0_14.u_ns_router.u_south_inblk.G_IN_PORT_ENB.G_IVCBUF_IVCCTRL[3].G_IVC_ENB.G_IVCBUF_SYNC.u_ivcbuf_sync_dat.wr_pntr_bin_q[3:0], u_ns_io_meshstop.u_ns_router_0_14.u_ns_router.u_south_inblk.G_IN_PORT_ENB.G_IVCBUF_IVCCTRL[2].G_IVC_ENB.G_IVCBUF_SYNC.u_ivcbuf_sync_dat.wr_pntr_bin_q[3:0], 1'b0, u_ns_io_meshstop.u_ns_router_0_14.u_ns_router.u_south_inblk.G_IN_PORT_ENB.G_IVCBUF_IVCCTRL[0].G_IVC_ENB.G_IVCBUF_SYNC.u_ivcbuf_sync_dat.wr_pntr_bin_q[3:0] }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, u_ns_io_meshstop.u_ns_router_0_14.u_ns_router.u_host3_inblk.G_IN_PORT_ENB.G_IVCBUF_IVCCTRL[2].G_IVC_ENB.G_IVCBUF_SYNC.u_ivcbuf_sync_dat.wr_pntr_bin_q[1:0], 1'b0, 1'b0 }, '{  u_ns_io_meshstop.u_ns_router_0_14.u_ns_router.u_host2_inblk.G_IN_PORT_ENB.G_IVCBUF_IVCCTRL[3].G_IVC_ENB.G_IVCBUF_SYNC.u_ivcbuf_sync_dat.wr_pntr_bin_q[1:0], 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, u_ns_io_meshstop.u_ns_router_0_14.u_ns_router.u_host0_inblk.G_IN_PORT_ENB.G_IVCBUF_IVCCTRL[0].G_IVC_ENB.G_IVCBUF_SYNC.u_ivcbuf_sync_dat.wr_pntr_bin_q[1:0] }  }  } ;

   `endif

   typedef enum int {north, east, south, west} evl_noc_flit_dir_t;

   logic [NUM_NOC_LAYERS-1:0] [NUM_VIRT_CHANNELS-1:0] n_valid_in, e_valid_in, s_valid_out, w_valid_out;
   logic [NUM_NOC_LAYERS-1:0] n_valid_lyrs_in, e_valid_lyrs_in, s_valid_lyrs_out, w_valid_lyrs_out;
   logic n_valid_reduc_in, e_valid_reduc_in, s_valid_reduc_out, w_valid_reduc_out;

   evl_paddr_t [NUM_NOC_LAYERS-1:0]       n_addr_in, e_addr_in, s_addr_out, w_addr_out;

   evl_bus_req_id_t [NUM_NOC_LAYERS-1:0]  n_req_id_in, e_req_id_in, s_req_id_out, w_req_id_out;
   evl_bus_req_id_t [NUM_NOC_LAYERS-1:0]  n_rsp_id_in, e_rsp_id_in, s_rsp_id_out, w_rsp_id_out;

   string      inst_name = $sformatf("S254:MESH_MONITOR");

   int memshire_master_ids [7:0]       = '{`MEMSHIRE_MASTER_IDS};
   int tol3_sib_master_ids [33:0]      = '{`TOL3_SIB_MASTER_IDS};
   int tosys_sib_master_ids [33:0]     = '{`TOSYS_SIB_MASTER_IDS};
   int l2tol3_master_ids [3:0] [33:0]  = '{'{`L2TOL3D_MASTER_IDS}, '{`L2TOL3C_MASTER_IDS}, '{`L2TOL3B_MASTER_IDS}, '{`TOL3_SIB_MASTER_IDS}};

   assign n_valid_in    = '{'{u_ns_io_meshstop.ns_o_north_flit_valid_7_22_3, 1'b0, u_ns_io_meshstop.ns_o_north_flit_valid_7_22_1, u_ns_io_meshstop.ns_o_north_flit_valid_7_22_0},
                            '{u_ns_io_meshstop.ns_o_north_flit_valid_6_22_3, 1'b0, 1'b0, u_ns_io_meshstop.ns_o_north_flit_valid_6_22_0},
                            '{1'b0, 1'b0, 1'b0, u_ns_io_meshstop.ns_o_north_flit_valid_5_22_0},
                            '{1'b0, 1'b0, 1'b0, u_ns_io_meshstop.ns_o_north_flit_valid_4_22_0},
                            '{u_ns_io_meshstop.ns_o_north_flit_valid_3_22_3, u_ns_io_meshstop.ns_o_north_flit_valid_3_22_2, 1'b0, u_ns_io_meshstop.ns_o_north_flit_valid_3_22_0},
                            '{u_ns_io_meshstop.ns_o_north_flit_valid_2_22_3, u_ns_io_meshstop.ns_o_north_flit_valid_2_22_2, u_ns_io_meshstop.ns_o_north_flit_valid_2_22_1, u_ns_io_meshstop.ns_o_north_flit_valid_2_22_0},
                            '{u_ns_io_meshstop.ns_o_north_flit_valid_1_22_3, u_ns_io_meshstop.ns_o_north_flit_valid_1_22_2, 1'b0, u_ns_io_meshstop.ns_o_north_flit_valid_1_22_0},
                            '{u_ns_io_meshstop.ns_o_north_flit_valid_0_22_3, u_ns_io_meshstop.ns_o_north_flit_valid_0_22_2, 1'b0, u_ns_io_meshstop.ns_o_north_flit_valid_0_22_0}};

   assign n_valid_lyrs_in     = '{|n_valid_in[7], |n_valid_in[6], |n_valid_in[5], |n_valid_in[4], |n_valid_in[3], |n_valid_in[2], |n_valid_in[1], |n_valid_in[0]};
   assign n_valid_reduc_in    = |n_valid_lyrs_in;

   assign n_addr_in     = '{u_ns_io_meshstop.ns_o_north_flit_usrsb_7_22[73:34], u_ns_io_meshstop.ns_o_north_flit_data_6_22[68:29], u_ns_io_meshstop.ns_o_north_flit_usrsb_5_22[73:34], u_ns_io_meshstop.ns_o_north_flit_data_4_22[68:29],
                            u_ns_io_meshstop.ns_o_north_flit_usrsb_3_22[73:34], u_ns_io_meshstop.ns_o_north_flit_data_2_22[68:29], u_ns_io_meshstop.ns_o_north_flit_usrsb_1_22[73:34], u_ns_io_meshstop.ns_o_north_flit_data_0_22[68:29]};

   assign n_req_id_in   = '{u_ns_io_meshstop.ns_o_north_flit_usrsb_7_22[92:74], u_ns_io_meshstop.ns_o_north_flit_data_6_22[87:69], u_ns_io_meshstop.ns_o_north_flit_usrsb_5_22[92:74], u_ns_io_meshstop.ns_o_north_flit_data_4_22[87:69],
                            u_ns_io_meshstop.ns_o_north_flit_usrsb_3_22[92:74], u_ns_io_meshstop.ns_o_north_flit_data_2_22[87:69], u_ns_io_meshstop.ns_o_north_flit_usrsb_1_22[92:74], u_ns_io_meshstop.ns_o_north_flit_data_0_22[87:69]};

   assign n_rsp_id_in   = '{u_ns_io_meshstop.ns_o_north_flit_usrsb_7_22[9:0], u_ns_io_meshstop.ns_o_north_flit_data_6_22[11:2], u_ns_io_meshstop.ns_o_north_flit_usrsb_5_22[9:0], u_ns_io_meshstop.ns_o_north_flit_data_4_22[11:2],
                            u_ns_io_meshstop.ns_o_north_flit_usrsb_3_22[9:0], u_ns_io_meshstop.ns_o_north_flit_data_2_22[11:2], u_ns_io_meshstop.ns_o_north_flit_usrsb_1_22[9:0], u_ns_io_meshstop.ns_o_north_flit_data_0_22[11:2]};

`ifdef  IOSHIRE_MESHSTOP_WRAPPER_GATESIM
`elsif  IOSHIRE_WRAPPER_ONLY_GATESIM
`else
   assign w_valid_out   = '{'{u_ns_io_meshstop.ns_o_west_flit_valid_7_14_3_PPLN_R7_13E_0, 1'b0, 1'b0, u_ns_io_meshstop.ns_o_west_flit_valid_7_14_0_PPLN_R7_13E_0},
                            '{u_ns_io_meshstop.ns_o_west_flit_valid_6_14_3_PPLN_R6_13E_0, 1'b0, 1'b0, u_ns_io_meshstop.ns_o_west_flit_valid_6_14_0_PPLN_R6_13E_0},
                            '{u_ns_io_meshstop.ns_o_west_flit_valid_5_14_3_PPLN_R5_13E_0, 1'b0, 1'b0, u_ns_io_meshstop.ns_o_west_flit_valid_5_14_0_PPLN_R5_13E_0},
                            '{u_ns_io_meshstop.ns_o_west_flit_valid_4_14_3_PPLN_R4_13E_0, 1'b0, 1'b0, u_ns_io_meshstop.ns_o_west_flit_valid_4_14_0_PPLN_R4_13E_0},
                            '{u_ns_io_meshstop.ns_o_west_flit_valid_3_14_3_PPLN_R3_13E_0, u_ns_io_meshstop.ns_o_west_flit_valid_3_14_2_PPLN_R3_13E_0, 1'b0, u_ns_io_meshstop.ns_o_west_flit_valid_3_14_0_PPLN_R3_13E_0},
                            '{u_ns_io_meshstop.ns_o_west_flit_valid_2_14_3_PPLN_R2_13E_0, u_ns_io_meshstop.ns_o_west_flit_valid_2_14_2_PPLN_R2_13E_0, 1'b0, u_ns_io_meshstop.ns_o_west_flit_valid_2_14_0_PPLN_R2_13E_0},
                            '{u_ns_io_meshstop.ns_o_west_flit_valid_1_14_3_PPLN_R1_13E_0, u_ns_io_meshstop.ns_o_west_flit_valid_1_14_2_PPLN_R1_13E_0, 1'b0, u_ns_io_meshstop.ns_o_west_flit_valid_1_14_0_PPLN_R1_13E_0},
                            '{u_ns_io_meshstop.ns_o_west_flit_valid_0_14_3_PPLN_R0_13E_0, u_ns_io_meshstop.ns_o_west_flit_valid_0_14_2_PPLN_R0_13E_0, 1'b0, u_ns_io_meshstop.ns_o_west_flit_valid_0_14_0_PPLN_R0_13E_0}};
`endif

   assign w_valid_lyrs_out    = '{|w_valid_out[7], |w_valid_out[6], |w_valid_out[5], |w_valid_out[4], |w_valid_out[3], |w_valid_out[2], |w_valid_out[1], |w_valid_out[0]};
   assign w_valid_reduc_out   = |w_valid_lyrs_out;

`ifndef  IOSHIRE_MESHSTOP_WRAPPER_GATESIM
   assign w_addr_out    = '{u_ns_io_meshstop.ns_o_west_flit_usrsb_7_14_PPLN_R7_13E_0[73:34], u_ns_io_meshstop.ns_o_west_flit_data_6_14_PPLN_R6_13E_0[68:29], u_ns_io_meshstop.ns_o_west_flit_usrsb_5_14_PPLN_R5_13E_0[73:34], u_ns_io_meshstop.ns_o_west_flit_data_4_14_PPLN_R4_13E_0[68:29],
                            u_ns_io_meshstop.ns_o_west_flit_usrsb_3_14_PPLN_R3_13E_0[73:34], u_ns_io_meshstop.ns_o_west_flit_data_2_14_PPLN_R2_13E_0[68:29], u_ns_io_meshstop.ns_o_west_flit_usrsb_1_14_PPLN_R1_13E_0[73:34], u_ns_io_meshstop.ns_o_west_flit_data_0_14_PPLN_R0_13E_0[68:29]};

   assign w_req_id_out  = '{u_ns_io_meshstop.ns_o_west_flit_usrsb_7_14_PPLN_R7_13E_0[92:74], u_ns_io_meshstop.ns_o_west_flit_data_6_14_PPLN_R6_13E_0[87:69], u_ns_io_meshstop.ns_o_west_flit_usrsb_5_14_PPLN_R5_13E_0[92:74], u_ns_io_meshstop.ns_o_west_flit_data_4_14_PPLN_R4_13E_0[87:69],
                            u_ns_io_meshstop.ns_o_west_flit_usrsb_3_14_PPLN_R3_13E_0[92:74], u_ns_io_meshstop.ns_o_west_flit_data_2_14_PPLN_R2_13E_0[87:69], u_ns_io_meshstop.ns_o_west_flit_usrsb_1_14_PPLN_R1_13E_0[92:74], u_ns_io_meshstop.ns_o_west_flit_data_0_14_PPLN_R0_13E_0[87:69]};

   assign w_rsp_id_out  = '{u_ns_io_meshstop.ns_o_west_flit_usrsb_7_14_PPLN_R7_13E_0[9:0], u_ns_io_meshstop.ns_o_west_flit_data_6_14_PPLN_R6_13E_0[11:2], u_ns_io_meshstop.ns_o_west_flit_usrsb_5_14_PPLN_R5_13E_0[9:0], u_ns_io_meshstop.ns_o_west_flit_data_4_14_PPLN_R4_13E_0[11:2],
                            u_ns_io_meshstop.ns_o_west_flit_usrsb_3_14_PPLN_R3_13E_0[9:0], u_ns_io_meshstop.ns_o_west_flit_data_2_14_PPLN_R2_13E_0[11:2], u_ns_io_meshstop.ns_o_west_flit_usrsb_1_14_PPLN_R1_13E_0[9:0], u_ns_io_meshstop.ns_o_west_flit_data_0_14_PPLN_R0_13E_0[11:2]};

   assign s_valid_out   = '{'{u_ns_io_meshstop.ns_o_south_flit_valid_7_14_3_PPLN_R7_22N_0, 1'b0, 1'b0, 1'b0},
                            '{u_ns_io_meshstop.ns_o_south_flit_valid_6_14_3_PPLN_R6_22N_0, 1'b0, 1'b0, u_ns_io_meshstop.ns_o_south_flit_valid_6_14_0_PPLN_R6_22N_0},
                            '{u_ns_io_meshstop.ns_o_south_flit_valid_5_14_3_PPLN_R5_22N_0, u_ns_io_meshstop.ns_o_south_flit_valid_5_14_2_PPLN_R5_22N_0, u_ns_io_meshstop.ns_o_south_flit_valid_5_14_1_PPLN_R5_22N_0, 1'b0},
                            '{u_ns_io_meshstop.ns_o_south_flit_valid_4_14_3_PPLN_R4_22N_0, 1'b0, u_ns_io_meshstop.ns_o_south_flit_valid_4_14_1_PPLN_R4_22N_0, 1'b0},
                            '{u_ns_io_meshstop.ns_o_south_flit_valid_3_14_3_PPLN_R3_22N_0, u_ns_io_meshstop.ns_o_south_flit_valid_3_14_2_PPLN_R3_22N_0, 1'b0, u_ns_io_meshstop.ns_o_south_flit_valid_3_14_0_PPLN_R3_22N_0},
                            '{u_ns_io_meshstop.ns_o_south_flit_valid_2_14_3_PPLN_R2_22N_0, u_ns_io_meshstop.ns_o_south_flit_valid_2_14_2_PPLN_R2_22N_0, u_ns_io_meshstop.ns_o_south_flit_valid_2_14_1_PPLN_R2_22N_0, u_ns_io_meshstop.ns_o_south_flit_valid_2_14_0_PPLN_R2_22N_0},
                            '{u_ns_io_meshstop.ns_o_south_flit_valid_1_14_3_PPLN_R1_22N_0, u_ns_io_meshstop.ns_o_south_flit_valid_1_14_2_PPLN_R1_22N_0, 1'b0, u_ns_io_meshstop.ns_o_south_flit_valid_1_14_0_PPLN_R1_22N_0},
                            '{u_ns_io_meshstop.ns_o_south_flit_valid_0_14_3_PPLN_R0_22N_0, 1'b0, 1'b0, u_ns_io_meshstop.ns_o_south_flit_valid_0_14_0_PPLN_R0_22N_0}};
`endif //  `ifndef IOSHIRE_MESHSTOP_WRAPPER_GATESIM

   assign s_valid_lyrs_out    = '{|s_valid_out[7], |s_valid_out[6], |s_valid_out[5], |s_valid_out[4], |s_valid_out[3], |s_valid_out[2], |s_valid_out[1], |s_valid_out[0]};
   assign s_valid_reduc_out   = |s_valid_lyrs_out;

   assign s_addr_out    = '{'b0, u_ns_io_meshstop.ns_o_south_flit_data_6_14_PPLN_R6_22N_0[68:29], u_ns_io_meshstop.ns_o_south_flit_usrsb_5_14_PPLN_R5_22N_0[73:34], u_ns_io_meshstop.ns_o_south_flit_data_4_14_PPLN_R4_22N_0[68:29],
                            u_ns_io_meshstop.ns_o_south_flit_usrsb_3_14_PPLN_R3_22N_0[73:34], u_ns_io_meshstop.ns_o_south_flit_data_2_14_PPLN_R2_22N_0[68:29], u_ns_io_meshstop.ns_o_south_flit_usrsb_1_14_PPLN_R1_22N_0[73:34], u_ns_io_meshstop.ns_o_south_flit_data_0_14_PPLN_R0_22N_0[68:29]};

   assign s_req_id_out  = '{'b0, u_ns_io_meshstop.ns_o_south_flit_data_6_14_PPLN_R6_22N_0[87:69], u_ns_io_meshstop.ns_o_south_flit_usrsb_5_14_PPLN_R5_22N_0[92:74], u_ns_io_meshstop.ns_o_south_flit_data_4_14_PPLN_R4_22N_0[87:69],
                            u_ns_io_meshstop.ns_o_south_flit_usrsb_3_14_PPLN_R3_22N_0[92:74], u_ns_io_meshstop.ns_o_south_flit_data_2_14_PPLN_R2_22N_0[87:69], u_ns_io_meshstop.ns_o_south_flit_usrsb_1_14_PPLN_R1_22N_0[92:74], u_ns_io_meshstop.ns_o_south_flit_data_0_14_PPLN_R0_22N_0[87:69]};

   assign s_rsp_id_out  = '{u_ns_io_meshstop.ns_o_south_flit_usrsb_7_14_PPLN_R7_22N_0[9:0], u_ns_io_meshstop.ns_o_south_flit_data_6_14_PPLN_R6_22N_0[11:2], u_ns_io_meshstop.ns_o_south_flit_usrsb_5_14_PPLN_R5_22N_0[9:0], u_ns_io_meshstop.ns_o_south_flit_data_4_14_PPLN_R4_22N_0[11:2],
                            u_ns_io_meshstop.ns_o_south_flit_usrsb_3_14_PPLN_R3_22N_0[9:0], u_ns_io_meshstop.ns_o_south_flit_data_2_14_PPLN_R2_22N_0[11:2], u_ns_io_meshstop.ns_o_south_flit_usrsb_1_14_PPLN_R1_22N_0[9:0], u_ns_io_meshstop.ns_o_south_flit_data_0_14_PPLN_R0_22N_0[11:2]};

   assign e_valid_in    = '{'{u_ns_io_meshstop.ns_o_east_flit_valid_7_13_3, 1'b0, 1'b0, u_ns_io_meshstop.ns_o_east_flit_valid_7_13_0},
                            '{u_ns_io_meshstop.ns_o_east_flit_valid_6_13_3, 1'b0, 1'b0, u_ns_io_meshstop.ns_o_east_flit_valid_6_13_0},
                            '{u_ns_io_meshstop.ns_o_east_flit_valid_5_13_3, 1'b0, 1'b0, u_ns_io_meshstop.ns_o_east_flit_valid_5_13_0},
                            '{u_ns_io_meshstop.ns_o_east_flit_valid_4_13_3, 1'b0, 1'b0, u_ns_io_meshstop.ns_o_east_flit_valid_4_13_0},
                            '{u_ns_io_meshstop.ns_o_east_flit_valid_3_13_3, 1'b0, 1'b0, u_ns_io_meshstop.ns_o_east_flit_valid_3_13_0},
                            '{u_ns_io_meshstop.ns_o_east_flit_valid_2_13_3, 1'b0, u_ns_io_meshstop.ns_o_east_flit_valid_2_13_1, u_ns_io_meshstop.ns_o_east_flit_valid_2_13_0},
                            '{u_ns_io_meshstop.ns_o_east_flit_valid_1_13_3, u_ns_io_meshstop.ns_o_east_flit_valid_1_13_2, 1'b0, u_ns_io_meshstop.ns_o_east_flit_valid_1_13_0},
                            '{u_ns_io_meshstop.ns_o_east_flit_valid_0_13_3, u_ns_io_meshstop.ns_o_east_flit_valid_0_13_2, 1'b0, u_ns_io_meshstop.ns_o_east_flit_valid_0_13_0}};

   assign e_valid_lyrs_in     = '{|e_valid_in[7], |e_valid_in[6], |e_valid_in[5], |e_valid_in[4], |e_valid_in[3], |e_valid_in[2], |e_valid_in[1], |e_valid_in[0]};
   assign e_valid_reduc_in    = |e_valid_lyrs_in;

   assign e_addr_in     = '{u_ns_io_meshstop.ns_o_east_flit_usrsb_7_13[73:34], u_ns_io_meshstop.ns_o_east_flit_data_6_13[68:29], u_ns_io_meshstop.ns_o_east_flit_usrsb_5_13[73:34], u_ns_io_meshstop.ns_o_east_flit_data_4_13[68:29],
                            u_ns_io_meshstop.ns_o_east_flit_usrsb_3_13[73:34], u_ns_io_meshstop.ns_o_east_flit_data_2_13[68:29], u_ns_io_meshstop.ns_o_east_flit_usrsb_1_13[73:34], u_ns_io_meshstop.ns_o_east_flit_data_0_13[68:29]};

   assign e_req_id_in   = '{u_ns_io_meshstop.ns_o_east_flit_usrsb_7_13[92:74], u_ns_io_meshstop.ns_o_east_flit_data_6_13[87:69], u_ns_io_meshstop.ns_o_east_flit_usrsb_5_13[92:74], u_ns_io_meshstop.ns_o_east_flit_data_4_13[87:69],
                            u_ns_io_meshstop.ns_o_east_flit_usrsb_3_13[92:74], u_ns_io_meshstop.ns_o_east_flit_data_2_13[87:69], u_ns_io_meshstop.ns_o_east_flit_usrsb_1_13[92:74], u_ns_io_meshstop.ns_o_east_flit_data_0_13[87:69]};

   assign e_rsp_id_in   = '{u_ns_io_meshstop.ns_o_east_flit_usrsb_7_13[9:0], u_ns_io_meshstop.ns_o_east_flit_data_6_13[11:2], u_ns_io_meshstop.ns_o_east_flit_usrsb_5_13[9:0], u_ns_io_meshstop.ns_o_east_flit_data_4_13[11:2],
                            u_ns_io_meshstop.ns_o_east_flit_usrsb_3_13[9:0], u_ns_io_meshstop.ns_o_east_flit_data_2_13[11:2], u_ns_io_meshstop.ns_o_east_flit_usrsb_1_13[9:0], u_ns_io_meshstop.ns_o_east_flit_data_0_13[11:2]};



   initial begin
      wait (clk__noc === 1'b0);
      fork
         begin
            forever begin
               @(posedge clk__noc)
               if (reset_n_system ) begin
                   if ( (clk_cnt % 1000) == 0 ) begin
                       print_histogram(inport_fifo_depth,outport_fifo_depth,input_fifo_depth_histogram,output_fifo_depth_histogram);
                       initialize_histogram(input_fifo_depth_histogram,output_fifo_depth_histogram);
                   end
                   watch_fifo_interface(inport_rd_pntr_bin_q,inport_wr_pntr_bin_q,inport_fifo_depth,outport_credit_cntr_q,outport_fifo_depth,input_fifo_depth_histogram,output_fifo_depth_histogram);
               end
               else if (clk_cnt ==0) begin
                   initialize_histogram(input_fifo_depth_histogram,output_fifo_depth_histogram);
               end
               clk_cnt = clk_cnt + 1 ;
               if (n_valid_reduc_in == 1'b1) begin
                  watch_interface(n_valid_in, n_addr_in, n_req_id_in, n_rsp_id_in, north, 1'b1);
               end
               if (e_valid_reduc_in == 1'b1) begin
                  watch_interface(e_valid_in, e_addr_in, e_req_id_in, e_rsp_id_in, east, 1'b1);
               end
               if (s_valid_reduc_out == 1'b1) begin
                  watch_interface(s_valid_out, s_addr_out, s_req_id_out, s_rsp_id_out, south, 1'b0);
               end
               if (w_valid_reduc_out == 1'b1) begin
                  watch_interface(w_valid_out, w_addr_out, w_req_id_out, w_rsp_id_out, west, 1'b0);
               end
            end
         end
      join_none
   end

   `include "dv/noc/meshstop_monitor_functions.svh"

   `ifdef EVL_COVERAGE_ON
      ioshire_mesh_cvg m_ioshire_mesh_cov = new();
      always @ (posedge clk__noc) begin
         if (reset_n_system) begin
            if (n_valid_reduc_in || e_valid_reduc_in || s_valid_reduc_out || w_valid_reduc_out) begin
               m_ioshire_mesh_cov.sample_flits(n_valid_lyrs_in, e_valid_lyrs_in, s_valid_lyrs_out, w_valid_lyrs_out);
            end
            if ((io0_tol3_m_AWVALID[0] & io0_tol3_m_AWREADY[0]) || (io0_tol3_m_WVALID[0] & io0_tol3_m_WREADY[0]) || (io0_tol3_m_BVALID[0] & io0_tol3_m_BREADY[0]) ||
                (io0_tol3_m_ARVALID[0] & io0_tol3_m_ARREADY[0]) || (io0_tol3_m_RVALID[0] & io0_tol3_m_RREADY[0]) ||
                (io0_tol3_m_AWVALID[1] & io0_tol3_m_AWREADY[1]) || (io0_tol3_m_WVALID[1] & io0_tol3_m_WREADY[1]) || (io0_tol3_m_BVALID[1] & io0_tol3_m_BREADY[1]) ||
                (io0_tol3_m_ARVALID[1] & io0_tol3_m_ARREADY[1]) || (io0_tol3_m_RVALID[1] & io0_tol3_m_RREADY[1]) ||
                (io0_tosys_m_AWVALID & io0_tosys_m_AWREADY) || (io0_tosys_m_WVALID & io0_tosys_m_WREADY) || (io0_tosys_m_BVALID & io0_tosys_m_BREADY) ||
                (io0_tosys_m_ARVALID & io0_tosys_m_ARREADY) || (io0_tosys_m_RVALID & io0_tosys_m_RREADY) ||
                (io0_pm_AWVALID & io0_pm_AWREADY) || (io0_pm_WVALID & io0_pm_WREADY) || (io0_pm_BVALID & io0_pm_BREADY) ||
                (io0_pm_ARVALID & io0_pm_ARREADY) || (io0_pm_RVALID & io0_pm_RREADY) ||
                (io0_ps_AWVALID & io0_ps_AWREADY) || (io0_ps_WVALID & io0_ps_WREADY) || (io0_ps_BVALID & io0_ps_BREADY) ||
                (io0_ps_ARVALID & io0_ps_ARREADY) || (io0_ps_RVALID & io0_ps_RREADY)) begin

                  m_ioshire_mesh_cov.sample_axi (io0_tol3_m_AWVALID[0], io0_tol3_m_AWREADY[0], io0_tol3_m_WVALID[0], io0_tol3_m_WREADY[0], io0_tol3_m_BVALID[0], io0_tol3_m_BREADY[0],
                                                 io0_tol3_m_ARVALID[0], io0_tol3_m_ARREADY[0], io0_tol3_m_RVALID[0], io0_tol3_m_RREADY[0],
                                                 io0_tol3_m_AWVALID[1], io0_tol3_m_AWREADY[1], io0_tol3_m_WVALID[1], io0_tol3_m_WREADY[1], io0_tol3_m_BVALID[1], io0_tol3_m_BREADY[1],
                                                 io0_tol3_m_ARVALID[1], io0_tol3_m_ARREADY[1], io0_tol3_m_RVALID[1], io0_tol3_m_RREADY[1],
                                                 io0_tosys_m_AWVALID, io0_tosys_m_AWREADY, io0_tosys_m_WVALID, io0_tosys_m_WREADY, io0_tosys_m_BVALID, io0_tosys_m_BREADY,
                                                 io0_tosys_m_ARVALID, io0_tosys_m_ARREADY, io0_tosys_m_RVALID, io0_tosys_m_RREADY,
                                                 io0_pm_AWVALID, io0_pm_AWREADY, io0_pm_WVALID, io0_pm_WREADY, io0_pm_BVALID, io0_pm_BREADY,
                                                 io0_pm_ARVALID, io0_pm_ARREADY, io0_pm_RVALID, io0_pm_RREADY,
                                                 io0_ps_AWVALID, io0_ps_AWREADY, io0_ps_WVALID, io0_ps_WREADY, io0_ps_BVALID, io0_ps_BREADY,
                                                 io0_ps_ARVALID, io0_ps_ARREADY, io0_ps_RVALID, io0_ps_RREADY);
            end
         end
      end
   `endif

   `include "dv/noc/cov/ioshire_mesh_assert.svh"

   wire unused_ok = &{ 1'b0,
                       io0_ps_ARVALID,
                       io0_ps_ARREADY,
                       io0_ps_AWVALID,
                       io0_ps_AWREADY,
                       io0_ps_WVALID,
                       io0_ps_WREADY,
                       io0_ps_BVALID,
                       io0_ps_BREADY,
                       io0_ps_RVALID,
                       io0_ps_RREADY,
                       io0_pm_ARVALID,
                       io0_pm_ARREADY,
                       io0_pm_AWVALID,
                       io0_pm_AWREADY,
                       io0_pm_WVALID,
                       io0_pm_WREADY,
                       io0_pm_BVALID,
                       io0_pm_BREADY,
                       io0_pm_RVALID,
                       io0_pm_RREADY,
                       io0_tosys_m_ARVALID,
                       io0_tosys_m_ARREADY,
                       io0_tosys_m_AWVALID,
                       io0_tosys_m_AWREADY,
                       io0_tosys_m_WVALID,
                       io0_tosys_m_WREADY,
                       io0_tosys_m_BVALID,
                       io0_tosys_m_BREADY,
                       io0_tosys_m_RVALID,
                       io0_tosys_m_RREADY,
                       io0_tol3_m_ARVALID,
                       io0_tol3_m_ARREADY,
                       io0_tol3_m_AWVALID,
                       io0_tol3_m_AWREADY,
                       io0_tol3_m_WVALID,
                       io0_tol3_m_WREADY,
                       io0_tol3_m_BVALID,
                       io0_tol3_m_BREADY,
                       io0_tol3_m_RVALID,
                       io0_tol3_m_RREADY,
                       1'b0 };

endmodule // evl_minion_meshstop_monitor

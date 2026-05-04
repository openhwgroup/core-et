`include "soc_defines.vh"
`include "dv/common/axi/axi_vc/evl_axi_pkg.sv"

module evl_mem_west_meshstop_monitor
   #(
   parameter [7:0] SHIRE_INST_ID = 255
   )
   (
   input       reset_n_system,
   input       clk__noc,
   input wire  mem_mesh_slave_axi_ARVALID,
   input wire  mem_mesh_slave_axi_ARREADY,
   input wire  mem_mesh_slave_axi_AWVALID,
   input wire  mem_mesh_slave_axi_AWREADY,
   input wire  mem_mesh_slave_axi_WVALID,
   input wire  mem_mesh_slave_axi_WREADY,
   input wire  mem_mesh_slave_axi_BVALID,
   input wire  mem_mesh_slave_axi_BREADY,
   input wire  mem_mesh_slave_axi_RVALID,
   input wire  mem_mesh_slave_axi_RREADY,

   input wire  atomic_rsp_mesh_master_axi_ARVALID,
   input wire  atomic_rsp_mesh_master_axi_ARREADY,
   input wire  atomic_rsp_mesh_master_axi_AWVALID,
   input wire  atomic_rsp_mesh_master_axi_AWREADY,
   input wire  atomic_rsp_mesh_master_axi_WVALID,
   input wire  atomic_rsp_mesh_master_axi_WREADY,
   input wire  atomic_rsp_mesh_master_axi_BVALID,
   input wire  atomic_rsp_mesh_master_axi_BREADY,
   input wire  atomic_rsp_mesh_master_axi_RVALID,
   input wire  atomic_rsp_mesh_master_axi_RREADY,

   input wire  sbm_mesh_slave_axi_ARVALID,
   input wire  sbm_mesh_slave_axi_ARREADY,
   input wire  sbm_mesh_slave_axi_AWVALID,
   input wire  sbm_mesh_slave_axi_AWREADY,
   input wire  sbm_mesh_slave_axi_WVALID,
   input wire  sbm_mesh_slave_axi_WREADY,
   input wire  sbm_mesh_slave_axi_BVALID,
   input wire  sbm_mesh_slave_axi_BREADY,
   input wire  sbm_mesh_slave_axi_RVALID,
   input wire  sbm_mesh_slave_axi_RREADY
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


   assign inport_fifo_depth =  '{ '{ '{ 4'b0, 4'b0, 4'b0, 4'b0 }, '{ 4'b0, 4'b1, 4'b0, 4'b0 }, '{ 4'b0, 4'b0, 4'b0, 4'b0 }, '{ 4'b0, 4'b0, 4'b0, 4'b0 }, '{ 4'b0, 4'b0, 4'b10, 4'b0 }, '{ 4'b0, 4'b0, 4'b0, 4'b0 }, '{ 4'b0, 4'b0, 4'b0, 4'b0 }, '{ 4'b10, 4'b0, 4'b0, 4'b0 } }, '{ '{ 4'b0, 4'b0, 4'b0, 4'b0 }, '{ 4'b0, 4'b0, 4'b0, 4'b0 }, '{ 4'b0, 4'b0, 4'b0, 4'b0 }, '{ 4'b0, 4'b0, 4'b0, 4'b0 }, '{ 4'b0, 4'b1, 4'b0, 4'b0 }, '{ 4'b0, 4'b0, 4'b0, 4'b0 }, '{ 4'b0, 4'b0, 4'b0, 4'b0 }, '{ 4'b10, 4'b0, 4'b0, 4'b0 } }, '{ '{ 4'b0, 4'b0, 4'b0, 4'b0 }, '{ 4'b0, 4'b10, 4'b110, 4'b0 }, '{ 4'b0, 4'b0, 4'b0, 4'b0 }, '{ 4'b0, 4'b0, 4'b0, 4'b0 }, '{ 4'b0, 4'b0, 4'b0, 4'b0 }, '{ 4'b0, 4'b0, 4'b0, 4'b0 }, '{ 4'b0, 4'b0, 4'b0, 4'b0 }, '{ 4'b0, 4'b0, 4'b0, 4'b0 } }, '{ '{ 4'b0, 4'b0, 4'b0, 4'b0 }, '{ 4'b0, 4'b0, 4'b110, 4'b0 }, '{ 4'b0, 4'b0, 4'b0, 4'b0 }, '{ 4'b0, 4'b0, 4'b0, 4'b0 }, '{ 4'b0, 4'b0, 4'b0, 4'b0 }, '{ 4'b0, 4'b0, 4'b0, 4'b0 }, '{ 4'b0, 4'b0, 4'b0, 4'b0 }, '{ 4'b0, 4'b0, 4'b0, 4'b0 } }, '{ '{ 4'b0, 4'b0, 4'b0, 4'b0 }, '{ 4'b0, 4'b110, 4'b0, 4'b0 }, '{ 4'b0, 4'b0, 4'b0, 4'b0 }, '{ 4'b0, 4'b0, 4'b0, 4'b0 }, '{ 4'b0, 4'b0, 4'b0, 4'b0 }, '{ 4'b0, 4'b0, 4'b0, 4'b0 }, '{ 4'b0, 4'b0, 4'b0, 4'b0 }, '{ 4'b0, 4'b0, 4'b0, 4'b0 } }, '{ '{ 4'b0, 4'b0, 4'b0, 4'b0 }, '{ 4'b0, 4'b110, 4'b0, 4'b0 }, '{ 4'b0, 4'b0, 4'b0, 4'b0 }, '{ 4'b0, 4'b0, 4'b0, 4'b0 }, '{ 4'b0, 4'b0, 4'b0, 4'b0 }, '{ 4'b0, 4'b0, 4'b0, 4'b0 }, '{ 4'b0, 4'b0, 4'b10, 4'b0 }, '{ 4'b0, 4'b0, 4'b0, 4'b0 } }, '{ '{ 4'b0, 4'b0, 4'b0, 4'b0 }, '{ 4'b0, 4'b0, 4'b0, 4'b0 }, '{ 4'b0, 4'b0, 4'b0, 4'b0 }, '{ 4'b0, 4'b0, 4'b0, 4'b0 }, '{ 4'b0, 4'b0, 4'b0, 4'b0 }, '{ 4'b0, 4'b0, 4'b0, 4'b0 }, '{ 4'b0, 4'b0, 4'b0, 4'b0 }, '{ 4'b0, 4'b0, 4'b0, 4'b0 } }, '{ '{ 4'b0, 4'b0, 4'b0, 4'b0 }, '{ 4'b0, 4'b0, 4'b0, 4'b0 }, '{ 4'b0, 4'b0, 4'b0, 4'b0 }, '{ 4'b0, 4'b0, 4'b0, 4'b0 }, '{ 4'b0, 4'b0, 4'b0, 4'b0 }, '{ 4'b0, 4'b0, 4'b0, 4'b0 }, '{ 4'b0, 4'b0, 4'b0, 4'b0 }, '{ 4'b0, 4'b0, 4'b0, 4'b0 } } } ;
   assign outport_fifo_depth =  '{ '{ '{ 4'b0, 4'b0, 4'b0, 4'b0 }, '{ 4'b110, 4'b0, 4'b10, 4'b0 }, '{ 4'b0, 4'b0, 4'b0, 4'b0 }, '{ 4'b0, 4'b0, 4'b0, 4'b0 }, '{ 4'b0, 4'b1, 4'b0, 4'b0 }, '{ 4'b0, 4'b0, 4'b0, 4'b0 }, '{ 4'b0, 4'b0, 4'b0, 4'b0 }, '{ 4'b0, 4'b0, 4'b0, 4'b0 } }, '{ '{ 4'b0, 4'b0, 4'b0, 4'b0 }, '{ 4'b110, 4'b1, 4'b0, 4'b0 }, '{ 4'b0, 4'b0, 4'b0, 4'b0 }, '{ 4'b0, 4'b0, 4'b0, 4'b0 }, '{ 4'b0, 4'b0, 4'b0, 4'b0 }, '{ 4'b0, 4'b0, 4'b0, 4'b0 }, '{ 4'b0, 4'b0, 4'b0, 4'b0 }, '{ 4'b0, 4'b0, 4'b0, 4'b0 } }, '{ '{ 4'b0, 4'b0, 4'b0, 4'b0 }, '{ 4'b0, 4'b0, 4'b0, 4'b0 }, '{ 4'b0, 4'b0, 4'b0, 4'b0 }, '{ 4'b0, 4'b0, 4'b0, 4'b0 }, '{ 4'b0, 4'b10, 4'b0, 4'b0 }, '{ 4'b0, 4'b0, 4'b0, 4'b0 }, '{ 4'b0, 4'b0, 4'b0, 4'b0 }, '{ 4'b0, 4'b0, 4'b10, 4'b0 } }, '{ '{ 4'b0, 4'b0, 4'b0, 4'b0 }, '{ 4'b0, 4'b0, 4'b0, 4'b0 }, '{ 4'b0, 4'b0, 4'b0, 4'b0 }, '{ 4'b0, 4'b0, 4'b0, 4'b0 }, '{ 4'b0, 4'b0, 4'b0, 4'b0 }, '{ 4'b0, 4'b0, 4'b0, 4'b0 }, '{ 4'b0, 4'b0, 4'b0, 4'b0 }, '{ 4'b0, 4'b0, 4'b10, 4'b0 } }, '{ '{ 4'b0, 4'b0, 4'b0, 4'b0 }, '{ 4'b0, 4'b0, 4'b0, 4'b0 }, '{ 4'b0, 4'b0, 4'b0, 4'b0 }, '{ 4'b0, 4'b0, 4'b0, 4'b0 }, '{ 4'b0, 4'b0, 4'b0, 4'b0 }, '{ 4'b0, 4'b0, 4'b0, 4'b0 }, '{ 4'b0, 4'b10, 4'b0, 4'b0 }, '{ 4'b0, 4'b0, 4'b0, 4'b0 } }, '{ '{ 4'b0, 4'b0, 4'b0, 4'b0 }, '{ 4'b0, 4'b0, 4'b110, 4'b0 }, '{ 4'b0, 4'b0, 4'b0, 4'b0 }, '{ 4'b0, 4'b0, 4'b0, 4'b0 }, '{ 4'b0, 4'b0, 4'b0, 4'b0 }, '{ 4'b0, 4'b0, 4'b0, 4'b0 }, '{ 4'b0, 4'b10, 4'b0, 4'b0 }, '{ 4'b0, 4'b0, 4'b0, 4'b0 } }, '{ '{ 4'b0, 4'b0, 4'b0, 4'b0 }, '{ 4'b0, 4'b0, 4'b0, 4'b0 }, '{ 4'b0, 4'b0, 4'b0, 4'b0 }, '{ 4'b0, 4'b0, 4'b0, 4'b0 }, '{ 4'b0, 4'b0, 4'b0, 4'b0 }, '{ 4'b0, 4'b0, 4'b0, 4'b0 }, '{ 4'b0, 4'b0, 4'b0, 4'b0 }, '{ 4'b0, 4'b0, 4'b0, 4'b0 } }, '{ '{ 4'b0, 4'b0, 4'b0, 4'b0 }, '{ 4'b0, 4'b0, 4'b0, 4'b0 }, '{ 4'b0, 4'b0, 4'b0, 4'b0 }, '{ 4'b0, 4'b0, 4'b0, 4'b0 }, '{ 4'b0, 4'b0, 4'b0, 4'b0 }, '{ 4'b0, 4'b0, 4'b0, 4'b0 }, '{ 4'b0, 4'b0, 4'b0, 4'b0 }, '{ 4'b0, 4'b0, 4'b0, 4'b0 } } } ;

   assign  outport_credit_cntr_q = '{ '{ '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  u_ns_mem_west_meshstop.u_ns_router_defer_7_16.u_ns_router.u_east_outblk.G_OUT_PORT_ENB.G_OVCSTS[3].G_OVC_ENABLED.u_ns_ovcsts.credit_cntr_q[4:0], 1'b0, u_ns_mem_west_meshstop.u_ns_router_defer_7_16.u_ns_router.u_east_outblk.G_OUT_PORT_ENB.G_OVCSTS[1].G_OVC_ENABLED.u_ns_ovcsts.credit_cntr_q[4:0], 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, u_ns_mem_west_meshstop.u_ns_router_defer_7_16.u_ns_router.u_host3_outblk.G_OUT_PORT_ENB.G_OVCSTS[2].G_OVC_ENABLED.u_ns_ovcsts.credit_cntr_q[4:0], 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }  }, '{ '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  u_ns_mem_west_meshstop.u_ns_router_defer_6_16.u_ns_router.u_east_outblk.G_OUT_PORT_ENB.G_OVCSTS[3].G_OVC_ENABLED.u_ns_ovcsts.credit_cntr_q[4:0], u_ns_mem_west_meshstop.u_ns_router_defer_6_16.u_ns_router.u_east_outblk.G_OUT_PORT_ENB.G_OVCSTS[2].G_OVC_ENABLED.u_ns_ovcsts.credit_cntr_q[4:0], 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }  }, '{ '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, u_ns_mem_west_meshstop.u_ns_router_defer_5_16.u_ns_router.u_host3_outblk.G_OUT_PORT_ENB.G_OVCSTS[2].G_OVC_ENABLED.u_ns_ovcsts.credit_cntr_q[4:0], 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, u_ns_mem_west_meshstop.u_ns_router_defer_5_16.u_ns_router.u_host0_outblk.G_OUT_PORT_ENB.G_OVCSTS[1].G_OVC_ENABLED.u_ns_ovcsts.credit_cntr_q[4:0], 1'b0 }  }, '{ '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, u_ns_mem_west_meshstop.u_ns_router_defer_4_16.u_ns_router.u_host0_outblk.G_OUT_PORT_ENB.G_OVCSTS[1].G_OVC_ENABLED.u_ns_ovcsts.credit_cntr_q[4:0], 1'b0 }  }, '{ '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, u_ns_mem_west_meshstop.u_ns_router_defer_3_16.u_ns_router.u_host1_outblk.G_OUT_PORT_ENB.G_OVCSTS[2].G_OVC_ENABLED.u_ns_ovcsts.credit_cntr_q[4:0], 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }  }, '{ '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, u_ns_mem_west_meshstop.u_ns_router_defer_2_16.u_ns_router.u_east_outblk.G_OUT_PORT_ENB.G_OVCSTS[1].G_OVC_ENABLED.u_ns_ovcsts.credit_cntr_q[4:0], 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, u_ns_mem_west_meshstop.u_ns_router_defer_2_16.u_ns_router.u_host1_outblk.G_OUT_PORT_ENB.G_OVCSTS[2].G_OVC_ENABLED.u_ns_ovcsts.credit_cntr_q[4:0], 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }  }, '{ '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }  }, '{ '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }  }  } ;

   assign  inport_rd_pntr_bin_q = '{ '{ '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, u_ns_mem_west_meshstop.u_ns_router_defer_7_16.u_ns_router.u_east_inblk.G_IN_PORT_ENB.G_IVCBUF_IVCCTRL[2].G_IVC_ENB.G_IVCBUF_SYNC.u_ivcbuf_sync_dat.rd_pntr_bin_q[1:0], 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, u_ns_mem_west_meshstop.u_ns_router_defer_7_16.u_ns_router.u_host3_inblk.G_IN_PORT_ENB.G_IVCBUF_IVCCTRL[1].G_IVC_ENB.G_IVCBUF_SYNC.u_ivcbuf_sync_dat.rd_pntr_bin_q[1:0], 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  u_ns_mem_west_meshstop.u_ns_router_defer_7_16.u_ns_router.u_host0_inblk.G_IN_PORT_ENB.G_IVCBUF_IVCCTRL[3].G_IVC_ENB.G_IVCBUF_SYNC.u_ivcbuf_sync_dat.rd_pntr_bin_q[1:0], 1'b0, 1'b0, 1'b0 }  }, '{ '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, u_ns_mem_west_meshstop.u_ns_router_defer_6_16.u_ns_router.u_host3_inblk.G_IN_PORT_ENB.G_IVCBUF_IVCCTRL[2].G_IVC_ENB.G_IVCBUF_SYNC.u_ivcbuf_sync_dat.rd_pntr_bin_q[1:0], 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  u_ns_mem_west_meshstop.u_ns_router_defer_6_16.u_ns_router.u_host0_inblk.G_IN_PORT_ENB.G_IVCBUF_IVCCTRL[3].G_IVC_ENB.G_IVCBUF_SYNC.u_ivcbuf_sync_dat.rd_pntr_bin_q[1:0], 1'b0, 1'b0, 1'b0 }  }, '{ '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, u_ns_mem_west_meshstop.u_ns_router_defer_5_16.u_ns_router.u_east_inblk.G_IN_PORT_ENB.G_IVCBUF_IVCCTRL[2].G_IVC_ENB.G_IVCBUF_SYNC.u_ivcbuf_sync_dat.rd_pntr_bin_q[1:0], u_ns_mem_west_meshstop.u_ns_router_defer_5_16.u_ns_router.u_east_inblk.G_IN_PORT_ENB.G_IVCBUF_IVCCTRL[1].G_IVC_ENB.G_IVCBUF_SYNC.u_ivcbuf_sync_dat.rd_pntr_bin_q[3:0], 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }  }, '{ '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, u_ns_mem_west_meshstop.u_ns_router_defer_4_16.u_ns_router.u_east_inblk.G_IN_PORT_ENB.G_IVCBUF_IVCCTRL[1].G_IVC_ENB.G_IVCBUF_SYNC.u_ivcbuf_sync_dat.rd_pntr_bin_q[3:0], 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }  }, '{ '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, u_ns_mem_west_meshstop.u_ns_router_defer_3_16.u_ns_router.u_east_inblk.G_IN_PORT_ENB.G_IVCBUF_IVCCTRL[2].G_IVC_ENB.G_IVCBUF_SYNC.u_ivcbuf_sync_dat.rd_pntr_bin_q[3:0], 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }  }, '{ '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, u_ns_mem_west_meshstop.u_ns_router_defer_2_16.u_ns_router.u_east_inblk.G_IN_PORT_ENB.G_IVCBUF_IVCCTRL[2].G_IVC_ENB.G_IVCBUF_SYNC.u_ivcbuf_sync_dat.rd_pntr_bin_q[3:0], 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, u_ns_mem_west_meshstop.u_ns_router_defer_2_16.u_ns_router.u_host1_inblk.G_IN_PORT_ENB.G_IVCBUF_IVCCTRL[1].G_IVC_ENB.G_IVCBUF_SYNC.u_ivcbuf_sync_dat.rd_pntr_bin_q[1:0], 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }  }, '{ '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }  }, '{ '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }  }  } ;

   assign  inport_wr_pntr_bin_q = '{ '{ '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, u_ns_mem_west_meshstop.u_ns_router_defer_7_16.u_ns_router.u_east_inblk.G_IN_PORT_ENB.G_IVCBUF_IVCCTRL[2].G_IVC_ENB.G_IVCBUF_SYNC.u_ivcbuf_sync_dat.wr_pntr_bin_q[1:0], 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, u_ns_mem_west_meshstop.u_ns_router_defer_7_16.u_ns_router.u_host3_inblk.G_IN_PORT_ENB.G_IVCBUF_IVCCTRL[1].G_IVC_ENB.G_IVCBUF_SYNC.u_ivcbuf_sync_dat.wr_pntr_bin_q[1:0], 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  u_ns_mem_west_meshstop.u_ns_router_defer_7_16.u_ns_router.u_host0_inblk.G_IN_PORT_ENB.G_IVCBUF_IVCCTRL[3].G_IVC_ENB.G_IVCBUF_SYNC.u_ivcbuf_sync_dat.wr_pntr_bin_q[1:0], 1'b0, 1'b0, 1'b0 }  }, '{ '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, u_ns_mem_west_meshstop.u_ns_router_defer_6_16.u_ns_router.u_host3_inblk.G_IN_PORT_ENB.G_IVCBUF_IVCCTRL[2].G_IVC_ENB.G_IVCBUF_SYNC.u_ivcbuf_sync_dat.wr_pntr_bin_q[1:0], 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  u_ns_mem_west_meshstop.u_ns_router_defer_6_16.u_ns_router.u_host0_inblk.G_IN_PORT_ENB.G_IVCBUF_IVCCTRL[3].G_IVC_ENB.G_IVCBUF_SYNC.u_ivcbuf_sync_dat.wr_pntr_bin_q[1:0], 1'b0, 1'b0, 1'b0 }  }, '{ '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, u_ns_mem_west_meshstop.u_ns_router_defer_5_16.u_ns_router.u_east_inblk.G_IN_PORT_ENB.G_IVCBUF_IVCCTRL[2].G_IVC_ENB.G_IVCBUF_SYNC.u_ivcbuf_sync_dat.wr_pntr_bin_q[1:0], u_ns_mem_west_meshstop.u_ns_router_defer_5_16.u_ns_router.u_east_inblk.G_IN_PORT_ENB.G_IVCBUF_IVCCTRL[1].G_IVC_ENB.G_IVCBUF_SYNC.u_ivcbuf_sync_dat.wr_pntr_bin_q[3:0], 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }  }, '{ '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, u_ns_mem_west_meshstop.u_ns_router_defer_4_16.u_ns_router.u_east_inblk.G_IN_PORT_ENB.G_IVCBUF_IVCCTRL[1].G_IVC_ENB.G_IVCBUF_SYNC.u_ivcbuf_sync_dat.wr_pntr_bin_q[3:0], 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }  }, '{ '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, u_ns_mem_west_meshstop.u_ns_router_defer_3_16.u_ns_router.u_east_inblk.G_IN_PORT_ENB.G_IVCBUF_IVCCTRL[2].G_IVC_ENB.G_IVCBUF_SYNC.u_ivcbuf_sync_dat.wr_pntr_bin_q[3:0], 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }  }, '{ '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, u_ns_mem_west_meshstop.u_ns_router_defer_2_16.u_ns_router.u_east_inblk.G_IN_PORT_ENB.G_IVCBUF_IVCCTRL[2].G_IVC_ENB.G_IVCBUF_SYNC.u_ivcbuf_sync_dat.wr_pntr_bin_q[3:0], 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, u_ns_mem_west_meshstop.u_ns_router_defer_2_16.u_ns_router.u_host1_inblk.G_IN_PORT_ENB.G_IVCBUF_IVCCTRL[1].G_IVC_ENB.G_IVCBUF_SYNC.u_ivcbuf_sync_dat.wr_pntr_bin_q[1:0], 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }  }, '{ '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }  }, '{ '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }, '{  1'b0, 1'b0, 1'b0, 1'b0 }  }  } ;

   typedef enum int {north, east, south, west} evl_noc_flit_dir_t;

   logic [NUM_NOC_LAYERS-1:0] [NUM_VIRT_CHANNELS-1:0] e_valid_out, w_valid_in;
   logic [NUM_NOC_LAYERS-1:0] e_valid_lyrs_out, w_valid_lyrs_in;
   logic e_valid_reduc_out, w_valid_reduc_in;

   evl_paddr_t [NUM_NOC_LAYERS-1:0]       e_addr_out, w_addr_in;

   evl_bus_req_id_t [NUM_NOC_LAYERS-1:0]  e_req_id_out, w_req_id_in;
   evl_bus_req_id_t [NUM_NOC_LAYERS-1:0]  e_rsp_id_out, w_rsp_id_in;

   //string      shire_name = evl_base_pkg::parse_hier_levels($sformatf("%m"), "", "", { 1, 2, 3, 4}, 1'b1);
   string      inst_name = $sformatf("S%0d:MESH_MONITOR", SHIRE_INST_ID);
   //int         shire_inst = shire_top.SHIRE_INST_ID;
   //int         shire_inst = shire_top.shire_id_reset_val;
   //string      inst_name = $sformatf("S%0d.meshstop_monitor", shire_inst);

   int memshire_master_ids [7:0]       = '{`MEMSHIRE_MASTER_IDS};
   int tol3_sib_master_ids [33:0]      = '{`TOL3_SIB_MASTER_IDS};
   int tosys_sib_master_ids [33:0]     = '{`TOSYS_SIB_MASTER_IDS};
   int l2tol3_master_ids [3:0] [33:0]  = '{'{`L2TOL3D_MASTER_IDS}, '{`L2TOL3C_MASTER_IDS}, '{`L2TOL3B_MASTER_IDS}, '{`TOL3_SIB_MASTER_IDS}};

   assign w_valid_in    = '{'{1'b0, 1'b0, 1'b0, 1'b0},
                            '{1'b0, 1'b0, 1'b0, 1'b0},
                            '{1'b0, u_ns_mem_west_meshstop.ns_o_west_flit_valid_5_17_2, u_ns_mem_west_meshstop.ns_o_west_flit_valid_5_17_1, 1'b0},
                            '{1'b0, 1'b0, u_ns_mem_west_meshstop.ns_o_west_flit_valid_4_17_1, 1'b0},
                            '{1'b0, u_ns_mem_west_meshstop.ns_o_west_flit_valid_3_17_2, 1'b0, 1'b0},
                            '{1'b0, u_ns_mem_west_meshstop.ns_o_west_flit_valid_2_17_2, 1'b0, 1'b0},
                            '{1'b0, 1'b0, 1'b0, 1'b0},
                            '{1'b0, 1'b0, 1'b0, 1'b0}};

   assign w_valid_lyrs_in     = '{|w_valid_in[7], |w_valid_in[6], |w_valid_in[5], |w_valid_in[4], |w_valid_in[3], |w_valid_in[2], |w_valid_in[1], |w_valid_in[0]};
   assign w_valid_reduc_in    = |w_valid_lyrs_in;

   assign w_addr_in     = '{'b0, 'b0, u_ns_mem_west_meshstop.ns_o_west_flit_usrsb_5_17[73:34], u_ns_mem_west_meshstop.ns_o_west_flit_data_4_17[68:29],
                            u_ns_mem_west_meshstop.ns_o_west_flit_usrsb_3_17[73:34], u_ns_mem_west_meshstop.ns_o_west_flit_data_2_17[68:29], 'b0, 'b0};

   assign w_req_id_in   = '{'b0, 'b0, u_ns_mem_west_meshstop.ns_o_west_flit_usrsb_5_17[92:74], u_ns_mem_west_meshstop.ns_o_west_flit_data_4_17[87:69],
                            u_ns_mem_west_meshstop.ns_o_west_flit_usrsb_3_17[92:74], u_ns_mem_west_meshstop.ns_o_west_flit_data_2_17[87:69], 'b0, 'b0};

   assign w_rsp_id_in   = '{u_ns_mem_west_meshstop.ns_o_west_flit_usrsb_7_17[9:0], 'b0, u_ns_mem_west_meshstop.ns_o_west_flit_usrsb_5_17[9:0], u_ns_mem_west_meshstop.ns_o_west_flit_data_4_17[11:2],
                            u_ns_mem_west_meshstop.ns_o_west_flit_usrsb_3_17[9:0], u_ns_mem_west_meshstop.ns_o_west_flit_data_2_17[11:2], 'b0, 'b0};

   assign e_valid_out   = '{'{u_ns_mem_west_meshstop.ns_o_east_flit_valid_7_16_3, 1'b0, u_ns_mem_west_meshstop.ns_o_east_flit_valid_7_16_1, 1'b0},
                            '{u_ns_mem_west_meshstop.ns_o_east_flit_valid_6_16_3, 1'b0, 1'b0, 1'b0},
                            '{1'b0, 1'b0, 1'b0, 1'b0},
                            '{1'b0, 1'b0, 1'b0, 1'b0},
                            '{1'b0, 1'b0, 1'b0, 1'b0},
                            '{1'b0, 1'b0, u_ns_mem_west_meshstop.ns_o_east_flit_valid_2_16_1, 1'b0},
                            '{1'b0, 1'b0, 1'b0, 1'b0},
                            '{1'b0, 1'b0, 1'b0, 1'b0}};

   assign e_valid_lyrs_out    = '{|e_valid_out[7], |e_valid_out[6], |e_valid_out[5], |e_valid_out[4], |e_valid_out[3], |e_valid_out[2], |e_valid_out[1], |e_valid_out[0]};
   assign e_valid_reduc_out   = |e_valid_lyrs_out;

   assign e_addr_out    = '{u_ns_mem_west_meshstop.ns_o_east_flit_usrsb_7_16[73:34], u_ns_mem_west_meshstop.ns_o_east_flit_data_6_16[68:29], 1'b0, 1'b0,
                            'b0, 'b0, 'b0, 'b0};

   assign e_req_id_out  = '{u_ns_mem_west_meshstop.ns_o_east_flit_usrsb_7_16[92:74], u_ns_mem_west_meshstop.ns_o_east_flit_data_6_16[87:69], 'b0, 'b0,
                            'b0, 'b0, 'b0, 'b0};

   assign e_rsp_id_out  = '{u_ns_mem_west_meshstop.ns_o_east_flit_usrsb_7_16[9:0], u_ns_mem_west_meshstop.ns_o_east_flit_data_6_16[11:2], 'b0, 'b0,
                            'b0, 'b0, 'b0, 'b0};



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
               if (w_valid_reduc_in == 1'b1) begin
                  watch_interface(w_valid_in, w_addr_in, w_req_id_in, w_rsp_id_in, west, 1'b1);
               end
               if (e_valid_reduc_out == 1'b1) begin
                  watch_interface(e_valid_out, e_addr_out, e_req_id_out, e_rsp_id_out, east, 1'b0);
               end
            end
         end
      join_none
   end

   `include "dv/noc/meshstop_monitor_functions.svh"


   `ifdef EVL_COVERAGE_ON
      memshire_west_mesh_cvg m_memshire_west_mesh_cov = new();
      always @ (posedge clk__noc) begin
         if (reset_n_system) begin
            if (w_valid_reduc_in) begin
               m_memshire_west_mesh_cov.memshire_west_mesh_parallel_flit_lyrs_in_cg.sample(w_valid_lyrs_in);
            end
            if (e_valid_reduc_out) begin
               m_memshire_west_mesh_cov.memshire_west_mesh_parallel_flit_lyrs_out_cg.sample(e_valid_lyrs_out);
            end
            if ((atomic_rsp_mesh_master_axi_AWVALID & atomic_rsp_mesh_master_axi_AWREADY) || (atomic_rsp_mesh_master_axi_WVALID & atomic_rsp_mesh_master_axi_WREADY) ||
                (atomic_rsp_mesh_master_axi_BVALID & atomic_rsp_mesh_master_axi_BREADY) || (atomic_rsp_mesh_master_axi_ARVALID & atomic_rsp_mesh_master_axi_ARREADY) ||
                (atomic_rsp_mesh_master_axi_RVALID & atomic_rsp_mesh_master_axi_RREADY) || (mem_mesh_slave_axi_AWVALID & mem_mesh_slave_axi_AWREADY) ||
                (mem_mesh_slave_axi_WVALID & mem_mesh_slave_axi_WREADY) || (mem_mesh_slave_axi_BVALID & mem_mesh_slave_axi_BREADY) ||
                (mem_mesh_slave_axi_ARVALID & mem_mesh_slave_axi_ARREADY) || (mem_mesh_slave_axi_RVALID & mem_mesh_slave_axi_RREADY) ||
                (sbm_mesh_slave_axi_AWVALID & sbm_mesh_slave_axi_AWREADY) || (sbm_mesh_slave_axi_WVALID & sbm_mesh_slave_axi_WREADY) ||
                (sbm_mesh_slave_axi_BVALID & sbm_mesh_slave_axi_BREADY) || (sbm_mesh_slave_axi_ARVALID & sbm_mesh_slave_axi_ARREADY) ||
                (sbm_mesh_slave_axi_RVALID & sbm_mesh_slave_axi_RREADY)) begin
               m_memshire_west_mesh_cov.sample_axi(atomic_rsp_mesh_master_axi_AWVALID, atomic_rsp_mesh_master_axi_AWREADY,
                                                   atomic_rsp_mesh_master_axi_WVALID, atomic_rsp_mesh_master_axi_WREADY,
                                                   atomic_rsp_mesh_master_axi_BVALID, atomic_rsp_mesh_master_axi_BREADY,
                                                   atomic_rsp_mesh_master_axi_ARVALID, atomic_rsp_mesh_master_axi_ARREADY,
                                                   atomic_rsp_mesh_master_axi_RVALID, atomic_rsp_mesh_master_axi_RREADY,
                                                   mem_mesh_slave_axi_AWVALID, mem_mesh_slave_axi_AWREADY,
                                                   mem_mesh_slave_axi_WVALID, mem_mesh_slave_axi_WREADY,
                                                   mem_mesh_slave_axi_BVALID, mem_mesh_slave_axi_BREADY,
                                                   mem_mesh_slave_axi_ARVALID, mem_mesh_slave_axi_ARREADY,
                                                   mem_mesh_slave_axi_RVALID, mem_mesh_slave_axi_RREADY,
                                                   sbm_mesh_slave_axi_AWVALID, sbm_mesh_slave_axi_AWREADY,
                                                   sbm_mesh_slave_axi_WVALID, sbm_mesh_slave_axi_WREADY,
                                                   sbm_mesh_slave_axi_BVALID, sbm_mesh_slave_axi_BREADY,
                                                   sbm_mesh_slave_axi_ARVALID, sbm_mesh_slave_axi_ARREADY,
                                                   sbm_mesh_slave_axi_RVALID, sbm_mesh_slave_axi_RREADY);
            end
         end
      end
   `endif

   `include "dv/noc/cov/memshire_west_mesh_assert.svh"

   wire unused_ok = &{ 1'b0,
                       mem_mesh_slave_axi_ARVALID,
                       mem_mesh_slave_axi_ARREADY,
                       mem_mesh_slave_axi_AWVALID,
                       mem_mesh_slave_axi_AWREADY,
                       mem_mesh_slave_axi_WVALID,
                       mem_mesh_slave_axi_WREADY,
                       mem_mesh_slave_axi_BVALID,
                       mem_mesh_slave_axi_BREADY,
                       mem_mesh_slave_axi_RVALID,
                       mem_mesh_slave_axi_RREADY,
                       atomic_rsp_mesh_master_axi_ARVALID,
                       atomic_rsp_mesh_master_axi_ARREADY,
                       atomic_rsp_mesh_master_axi_AWVALID,
                       atomic_rsp_mesh_master_axi_AWREADY,
                       atomic_rsp_mesh_master_axi_WVALID,
                       atomic_rsp_mesh_master_axi_WREADY,
                       atomic_rsp_mesh_master_axi_BVALID,
                       atomic_rsp_mesh_master_axi_BREADY,
                       atomic_rsp_mesh_master_axi_RVALID,
                       atomic_rsp_mesh_master_axi_RREADY,
                       sbm_mesh_slave_axi_ARVALID,
                       sbm_mesh_slave_axi_ARREADY,
                       sbm_mesh_slave_axi_AWVALID,
                       sbm_mesh_slave_axi_AWREADY,
                       sbm_mesh_slave_axi_WVALID,
                       sbm_mesh_slave_axi_WREADY,
                       sbm_mesh_slave_axi_BVALID,
                       sbm_mesh_slave_axi_BREADY,
                       sbm_mesh_slave_axi_RVALID,
                       sbm_mesh_slave_axi_RREADY,
                       1'b0 };

endmodule // evl_mem_west_meshstop_monitor

`ifndef __EVL_NOC_PKG

   `define __EVL_NOC_PKG 1

   `define AXI_TOP_NOC_SHIRE_PORT(id, port) \
    .id``_AWADDR(port.AW.ADDR),\
    .id``_AWLEN(port.AW.LEN),\
    .id``_AWID(port.AW.ID),\
    .id``_AWSIZE(port.AW.SIZE),\
    .id``_AWBURST(port.AW.BURST),\
    .id``_AWLOCK(port.AW.LOCK),\
    .id``_AWCACHE(port.AW.CACHE),\
    .id``_AWPROT(port.AW.PROT),\
    .id``_AWQOS(port.AW.QOS),\
    .id``_AWUSER(port.AW.USER),\
    .id``_AWVALID(port.AWVALID),\
    .id``_AWREADY(port.AWREADY),\
    .id``_WDATA(port.W.DATA),\
    .id``_WSTRB(port.W.STRB),\
    .id``_WLAST(port.W.LAST),\
    .id``_WVALID(port.WVALID),\
    .id``_WREADY(port.WREADY),\
    .id``_BREADY(port.BREADY),\
    .id``_BRESP(port.B.RESP),\
    .id``_BID(port.B.ID),\
    .id``_BVALID(port.BVALID),\
    .id``_ARADDR(port.AR.ADDR),\
    .id``_ARLEN(port.AR.LEN),\
    .id``_ARID(port.AR.ID),\
    .id``_ARSIZE(port.AR.SIZE),\
    .id``_ARBURST(port.AR.BURST),\
    .id``_ARLOCK(port.AR.LOCK),\
    .id``_ARCACHE(port.AR.CACHE),\
    .id``_ARPROT(port.AR.PROT),\
    .id``_ARQOS(port.AR.QOS),\
    .id``_ARVALID(port.ARVALID),\
    .id``_ARREADY(port.ARREADY),\
    .id``_RREADY(port.RREADY),\
    .id``_RDATA(port.R.DATA),\
    .id``_RRESP(port.R.RESP),\
    .id``_RID(port.R.ID),\
    .id``_RLAST(port.R.LAST),\
    .id``_RVALID(port.RVALID)

   `define AXI_TOP_NOC_SHIRE_PORT_NO_AWUSER(id, port) \
    .id``_AWADDR(port.AW.ADDR),\
    .id``_AWLEN(port.AW.LEN),\
    .id``_AWID(port.AW.ID),\
    .id``_AWSIZE(port.AW.SIZE),\
    .id``_AWBURST(port.AW.BURST),\
    .id``_AWLOCK(port.AW.LOCK),\
    .id``_AWCACHE(port.AW.CACHE),\
    .id``_AWPROT(port.AW.PROT),\
    .id``_AWQOS(port.AW.QOS),\
    .id``_AWVALID(port.AWVALID),\
    .id``_AWREADY(port.AWREADY),\
    .id``_WDATA(port.W.DATA),\
    .id``_WSTRB(port.W.STRB),\
    .id``_WLAST(port.W.LAST),\
    .id``_WVALID(port.WVALID),\
    .id``_WREADY(port.WREADY),\
    .id``_BREADY(port.BREADY),\
    .id``_BRESP(port.B.RESP),\
    .id``_BID(port.B.ID),\
    .id``_BVALID(port.BVALID),\
    .id``_ARADDR(port.AR.ADDR),\
    .id``_ARLEN(port.AR.LEN),\
    .id``_ARID(port.AR.ID),\
    .id``_ARSIZE(port.AR.SIZE),\
    .id``_ARBURST(port.AR.BURST),\
    .id``_ARLOCK(port.AR.LOCK),\
    .id``_ARCACHE(port.AR.CACHE),\
    .id``_ARPROT(port.AR.PROT),\
    .id``_ARQOS(port.AR.QOS),\
    .id``_ARVALID(port.ARVALID),\
    .id``_ARREADY(port.ARREADY),\
    .id``_RREADY(port.RREADY),\
    .id``_RDATA(port.R.DATA),\
    .id``_RRESP(port.R.RESP),\
    .id``_RID(port.R.ID),\
    .id``_RLAST(port.R.LAST),\
    .id``_RVALID(port.RVALID)


   `define AXI_TOP_SHIRE_NOC_PORT(id, port)\
    .id``_AR(port.AR),\
    .id``_ARVALID(port.ARVALID),\
    .id``_ARREADY(port.ARREADY),\
    .id``_AW(port.AW),\
    .id``_AWVALID(port.AWVALID),\
    .id``_AWREADY(port.AWREADY),\
    .id``_W(port.W),\
    .id``_WVALID(port.WVALID),\
    .id``_WREADY(port.WREADY),\
    .id``_B(port.B),\
    .id``_BVALID(port.BVALID),\
    .id``_BREADY(port.BREADY),\
    .id``_R(port.R),\
    .id``_RVALID(port.RVALID),\
    .id``_RREADY(port.RREADY)

   `define AXI_NOC_TOP_MASTER_PORT(id)\
    output  wire sc_master_axi_AR_channel_t    id``_AR,\
    output  wire                               id``_ARVALID,\
    input   wire                               id``_ARREADY,\
    output  wire sc_master_axi_AW_channel_t    id``_AW,\
    output  wire                               id``_AWVALID,\
    input   wire                               id``_AWREADY,\
    output  wire sc_master_axi_W_channel_t     id``_W,\
    output  wire                               id``_WVALID,\
    input   wire                               id``_WREADY,\
    input   wire sc_master_axi_B_channel_t     id``_B,\
    input   wire                               id``_BVALID,\
    output  wire                               id``_BREADY,\
    input   wire sc_master_axi_R_channel_t     id``_R,\
    input   wire                               id``_RVALID,\
    output  wire                               id``_RREADY

    `define AXI_NOC_TOP_SLAVE_PORT(id)\
    input  wire sc_slave_axi_AR_channel_t      id``_AR,\
    input  wire                                id``_ARVALID,\
    output wire                                id``_ARREADY,\
    input  wire sc_slave_axi_AW_channel_t      id``_AW,\
    input  wire                                id``_AWVALID,\
    output wire                                id``_AWREADY,\
    input  wire sc_slave_axi_W_channel_t       id``_W,\
    input  wire                                id``_WVALID,\
    output wire                                id``_WREADY,\
    output wire sc_slave_axi_B_channel_t       id``_B,\
    output wire                                id``_BVALID,\
    input  wire                                id``_BREADY,\
    output wire sc_slave_axi_R_channel_t       id``_R,\
    output wire                                id``_RVALID,\
    input  wire                                id``_RREADY

   `include "dv/common/base/evl_base_pkg.sv"
   //`include "dv/soc/evl_soc_pkg.sv"

   package evl_noc_pkg;

      import uvm_pkg::*;
      import evl_base_pkg::*;
      //import evl_soc_pkg::*;

      typedef enum int {
            L2TOL3_M,
            UCTOL3_M,
            L3TOSYS_M,
            UCTOSYS_M,
            MCTOSYS_M,
            IO_TOL3_M,
            IO_TOSYS_M,
            IO_PM,
            PSHIRE_TOL3_M,
            PSHIRE_TOSYS_M
         } evl_noc_master_type_t;

      typedef enum int {
            TOL3_S,
            TOSYS_S,
            TOMEM_S,
            MC_SB_S,
            IO_PS,
            PSHIRE_PS
         } evl_noc_slave_type_t;

      typedef enum int {
            DBG_RT_EVT,
            DBG_RT_MSG,
            DBG_LF_EVT,
            DBG_LF_MSG,
            DBG_DN_EVT,
            DBG_DN_MSG
         } evl_dbgnoc_ifc_type_t;

      `define EVL_DBGNOC_EVT_IFC DBG_RT_EVT, DBG_LF_EVT, DBG_DN_EVT
      `define EVL_DBGNOC_MSG_IFC DBG_RT_MSG, DBG_LF_MSG, DBG_DN_MSG

      //typedef class evl_noc_scoreboard;
      //typedef class evl_noc_rtl_port;
      //typedef class evl_noc_cvg;

      //`include "dv/noc/evl_noc_rtl_port.sv"
      //`include "dv/noc/evl_noc_env.sv"
      //`include "dv/noc/evl_noc_test_env.sv"
      //`include "dv/noc/evl_noc_axi_meshq.sv"
      //`include "dv/noc/cov/evl_noc_sb_cvg.svh"
      //`include "dv/noc/cov/minion_mesh_cvg.svh"
      //`include "dv/noc/cov/ioshire_mesh_cvg.svh"
     //`include "dv/noc/cov/pshire_mesh_cvg.svh"
      //`include "dv/noc/cov/memshire_west_mesh_cvg.svh"
      //`include "dv/noc/cov/memshire_east_mesh_cvg.svh"
      `include "dv/noc/evl_noc_regmap.svh"
      `include "dv/noc/evl_noc_scoreboard.sv"
      `include "dv/noc/evl_dbgnoc_scoreboard.sv"

   endpackage : evl_noc_pkg

   `ifndef NOC_DISABLE
      `include "dv/noc/evl_minion_meshstop_monitor.sv"
      `include "dv/noc/evl_ioshire_meshstop_monitor.sv"
      `include "dv/noc/evl_pshire_meshstop_monitor.sv"
      `include "dv/noc/evl_mem_west_meshstop_monitor.sv"
      `include "dv/noc/evl_mem_east_meshstop_monitor.sv"
   `endif

`endif // !__EVL_NOC_PKG

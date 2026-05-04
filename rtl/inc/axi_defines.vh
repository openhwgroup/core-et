// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`ifndef _AXI_DEFINES_
`define _AXI_DEFINES_


// **************************************************************************
// `defines
// **************************************************************************

// AXI protocol required widths
`define AXI_AXLEN_SIZE    8
`define AXI_AXSIZE_SIZE   3
`define AXI_AXBURST_SIZE  2
`define AXI_AXCACHE_SIZE  4
`define AXI_AXPROT_SIZE   3
`define AXI_AXQOS_SIZE    4
`define AXI_BRESP_SIZE    2
`define AXI_RRESP_SIZE    2

// AXI QOS levels - per usage
`define AXI_QOS_MESSAGE           2
`define AXI_QOS_ATOMIC_RSP        2
`define AXI_QOS_ESR               3
`define AXI_QOS_MEM_LOW_PRIORITY  0
`define AXI_QOS_MEM_HIGH_PRIORITY 1
`define AXI_QOS_DONT_CARE         0

// Design specific widths
//
// (Not included prefix to specify Minion vs Maxion L2/L3)
// 1st item:  L2_ or L3_  ... connects to...
// 2nd item:  MESH    ... with direction ...
// 3rd item:  MASTER_ or SLAVE_  - widths are different - ... with protocol ...
// 4th item:  AXI_  - protocol and where it is (Mesh connector)
`define SC_MESH_MASTER_AXI_ADDR_SIZE   40
// ID size = 2-3 bank + 6 ID
`define SC_MESH_MASTER_AXI_ID_SIZE     9
`define SC_MESH_MASTER_AXI_AXUSER_SIZE 5   
`define SC_MESH_MASTER_AXI_ARUSER_SIZE 1
`define SC_MESH_MASTER_AXI_AWUSER_SIZE `SC_MESH_MASTER_AXI_AXUSER_SIZE 
`define SC_MESH_MASTER_AXI_DATA_SIZE   512
`define SC_MESH_MASTER_AXI_WSTRB_SIZE  (`SC_MESH_MASTER_AXI_DATA_SIZE/8)
`define SC_MESH_MASTER_AXI_WUSER_SIZE  0
`define SC_MESH_MASTER_AXI_BUSER_SIZE  0
`define SC_MESH_MASTER_AXI_RUSER_SIZE  0

`define MEM_SBM_MESH_SLAVE_AXI_DATA_SIZE   64
`define MEM_ATOMIC_RSP_AXI_DATA_SIZE      256

// Defines for debug
`define DEBUG_AXI_DATA_SIZE                64
`define DEBUG_AXI_STRB_SIZE                (`DEBUG_AXI_DATA_SIZE/8)

//Defines for sys slave port
`define SYS_MESH_SLAVE_AXI_DATA_SIZE 256
`define SYS_MESH_SLAVE_AXI_WSTRB_SIZE  (`SYS_MESH_SLAVE_AXI_DATA_SIZE/8)

// Constant values for unused parts of AXI
`define SC_MESH_MASTER_AXI_AXLEN   8'b0
`define SC_MESH_MASTER_AXI_AXBURST 2'b01    // INCR
`define SC_MESH_MASTER_AXI_AXCACHE 4'b0000  // ARCACHE {Other Allocate, Allocate, Modifiable, Bufferable}
`define SC_MESH_MASTER_AXI_AXPROT  3'b010   // {Data, Non-secure, Unprivileged}
`define SC_MESH_MASTER_AXI_AXQOS   4'b0


// Mesh adds more bits of ID to track where transaction came from
`define MESH_SOURCE_BRIDGE_ID_SIZE    10
// L2_AXI_SLAVE_ID_SIZE will be `L2_AXI_MASTER_ID_SIZE + encoded bits of all masters to ease return paths and avoid ordering
`define SC_MESH_SLAVE_AXI_ID_SIZE     (`MESH_SOURCE_BRIDGE_ID_SIZE + `SC_MESH_MASTER_AXI_ID_SIZE)

// assert valid ports macros for AXI structs
// assert all channels are valid (except for data)
`define ASSERT_VALID_PORTS_AXI( BASE, CLK, RESET) \
   `ASSERT_VALID_PORTS(BASE``_ARID,     CLK, RESET, BASE``_ARVALID, BASE``_AR.ID) \
   `ASSERT_VALID_PORTS(BASE``_ARADDR,   CLK, RESET, BASE``_ARVALID, BASE``_AR.ADDR) \
   `ASSERT_VALID_PORTS(BASE``_ARLEN,    CLK, RESET, BASE``_ARVALID, BASE``_AR.LEN) \
   `ASSERT_VALID_PORTS(BASE``_ARSIZE,   CLK, RESET, BASE``_ARVALID, BASE``_AR.SIZE) \
   `ASSERT_VALID_PORTS(BASE``_ARBURST,  CLK, RESET, BASE``_ARVALID, BASE``_AR.BURST) \
   `ASSERT_VALID_PORTS(BASE``_ARLOCK,   CLK, RESET, BASE``_ARVALID, BASE``_AR.LOCK) \
   `ASSERT_VALID_PORTS(BASE``_ARCACHE,  CLK, RESET, BASE``_ARVALID, BASE``_AR.CACHE) \
   `ASSERT_VALID_PORTS(BASE``_ARPROT,   CLK, RESET, BASE``_ARVALID, BASE``_AR.PROT) \
   `ASSERT_VALID_PORTS(BASE``_ARQOS,    CLK, RESET, BASE``_ARVALID, BASE``_AR.QOS) \
   `ASSERT_VALID_PORTS(BASE``_ARUSER,   CLK, RESET, BASE``_ARVALID, BASE``_AR.USER) \
 \
   `ASSERT_VALID_PORTS(BASE``_AWID,     CLK, RESET, BASE``_AWVALID, BASE``_AW.ID) \
   `ASSERT_VALID_PORTS(BASE``_AWADDR,   CLK, RESET, BASE``_AWVALID, BASE``_AW.ADDR) \
   `ASSERT_VALID_PORTS(BASE``_AWLEN,    CLK, RESET, BASE``_AWVALID, BASE``_AW.LEN) \
   `ASSERT_VALID_PORTS(BASE``_AWSIZE,   CLK, RESET, BASE``_AWVALID, BASE``_AW.SIZE) \
   `ASSERT_VALID_PORTS(BASE``_AWBURST,  CLK, RESET, BASE``_AWVALID, BASE``_AW.BURST) \
   `ASSERT_VALID_PORTS(BASE``_AWLOCK,   CLK, RESET, BASE``_AWVALID, BASE``_AW.LOCK) \
   `ASSERT_VALID_PORTS(BASE``_AWCACHE,  CLK, RESET, BASE``_AWVALID, BASE``_AW.CACHE) \
   `ASSERT_VALID_PORTS(BASE``_AWPROT,   CLK, RESET, BASE``_AWVALID, BASE``_AW.PROT) \
   `ASSERT_VALID_PORTS(BASE``_AWQOS,    CLK, RESET, BASE``_AWVALID, BASE``_AW.QOS) \
   `ASSERT_VALID_PORTS(BASE``_AWUSER,   CLK, RESET, BASE``_AWVALID, BASE``_AW.USER) \
 \
   `ASSERT_VALID_PORTS(BASE``_WSTRB,    CLK, RESET, BASE``_WVALID, BASE``_W.STRB) \
   `ASSERT_VALID_PORTS(BASE``_WLAST,    CLK, RESET, BASE``_WVALID, BASE``_W.LAST) \
 \
   `ASSERT_VALID_PORTS(BASE``_BID,    CLK, RESET, BASE``_BVALID,  BASE``_B.ID) \
   `ASSERT_VALID_PORTS(BASE``_BRESP,  CLK, RESET, BASE``_BVALID,  BASE``_B.RESP) \
 \
   `ASSERT_VALID_PORTS(BASE``_RID,    CLK, RESET, BASE``_RVALID,  BASE``_R.ID) \
   `ASSERT_VALID_PORTS(BASE``_RRESP,  CLK, RESET, BASE``_RVALID,  BASE``_R.RESP) \
   `ASSERT_VALID_PORTS(BASE``_RLAST,  CLK, RESET, BASE``_RVALID,  BASE``_R.LAST) \


// Constant values for QOS depending on the request sent
`define ET_AXI_QOS_MEM 4'b0001
`define ET_AXI_QOS_MESSAGE 4'b0010
`define ET_AXI_QOS_ESR 4'b0011
`define ET_AXI_QOS_ATOMIC_L3_REQ 4'b0000
`define ET_AXI_QOS_ATOMIC_REQ 4'b0001

// Debug - number of bits debug block uses to select axi port
`define DEBUG_AXI_PORT_SEL_SIZE 4


`endif // _AXI_DEFINES_

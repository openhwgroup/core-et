// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`ifndef __MESH_DEFINES__

    `define IOSHIRE_PM_MASTER_ID    0
    `define IOSHIRE_TOL3_MASTER_ID  1
    `define IOSHIRE_TOL3B_MASTER_ID 2
    `define IOSHIRE_TOSYS_MASTER_ID 3

    `define PSHIRE_TOL3_MASTER_ID   12
    `define PSHIRE_TOSYS_MASTER_ID  13

    `define MEMSHIRE_MASTER_IDS  11, 10, 9, 8, 7, 6, 5, 4

    `define TOL3_SIB_MASTER_IDS  279, 278, 277, 276, 274, 273, 272, 271, 270, 269, 268, 267, 266, 265, 263, 262, 261, 260, 259, 258, 257, 256, 255, 254, 285, 284, 283, 282, 281, 280, 275, 264, 253, 252
    `define TOSYS_SIB_MASTER_IDS 313, 312, 311, 310, 308, 307, 306, 305, 304, 303, 302, 301, 300, 299, 297, 296, 295, 294, 293, 292, 291, 290, 289, 288, 319, 318, 317, 316, 315, 314, 309, 298, 287, 286

    `define L2TOL3B_MASTER_IDS   204, 197, 190, 183, 169, 162, 155, 148, 141, 134, 127, 120, 113, 106, 92, 85, 78, 71, 64, 57, 50, 43, 36, 29, 246, 239, 232, 225, 218, 211, 176, 99, 22, 15
    `define L2TOL3C_MASTER_IDS   205, 198, 191, 184, 170, 163, 156, 149, 142, 135, 128, 121, 114, 107, 93, 86, 79, 72, 65, 58, 51, 44, 37, 30, 247, 240, 233, 226, 219, 212, 177, 100, 23, 16
    `define L2TOL3D_MASTER_IDS   206, 199, 192, 185, 171, 164, 157, 150, 143, 136, 129, 122, 115, 108, 94, 87, 80, 73, 66, 59, 52, 45, 38, 31, 248, 241, 234, 227, 220, 213, 178, 101, 24, 17
    `define MEM_MINSHIRE_SYS_MASTER_RANGE_ID 286:319

    //RTLMIN-2200: Defines used for id compression at memshire
    `define MEM_IOSHIRE_SYS_MASTER_ID `IOSHIRE_PM_MASTER_ID
    `define MEM_PSHIRE_SYS_MASTER_ID `IOSHIRE_TOSYS_MASTER_ID
    `define MEM_CONTROL_SHIRE_SYS_MASTER_ID `PSHIRE_TOSYS_MASTER_ID
    //`define MEM_MINSHIRE_SYS_MASTER_RANGE_ID 290:323


    `define __MESH_DEFINES__
    `define NOC_SHIRE_PORT(id, port) \
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

    `define SHIRE_NOC_PORT(id, port)\
    .id``_AR(port``_AR),\
    .id``_ARVALID(port``_ARVALID),\
    .id``_ARREADY(port``_ARREADY),\
    .id``_AW(port``_AW),\
    .id``_AWVALID(port``_AWVALID),\
    .id``_AWREADY(port``_AWREADY),\
    .id``_W(port``_W),\
    .id``_WVALID(port``_WVALID),\
    .id``_WREADY(port``_WREADY),\
    .id``_B(port``_B),\
    .id``_BVALID(port``_BVALID),\
    .id``_BREADY(port``_BREADY),\
    .id``_R(port``_R),\
    .id``_RVALID(port``_RVALID),\
    .id``_RREADY(port``_RREADY)


    `define SHIRE_NOC_PORT_INDEXED(id, port,index)\
    .id``_AR(port``_AR[index]),\
    .id``_ARVALID(port``_ARVALID[index]),\
    .id``_ARREADY(port``_ARREADY[index]),\
    .id``_AW(port``_AW[index]),\
    .id``_AWVALID(port``_AWVALID[index]),\
    .id``_AWREADY(port``_AWREADY[index]),\
    .id``_W(port``_W[index]),\
    .id``_WVALID(port``_WVALID[index]),\
    .id``_WREADY(port``_WREADY[index]),\
    .id``_B(port``_B[index]),\
    .id``_BVALID(port``_BVALID[index]),\
    .id``_BREADY(port``_BREADY[index]),\
    .id``_R(port``_R[index]),\
    .id``_RVALID(port``_RVALID[index]),\
    .id``_RREADY(port``_RREADY[index])

    `define SBM_SYS_SLAVE_VSI_ID 1
    `define UC_SYS_SLAVE_VSI_ID 0

    `define MC_BUFFER_SIZE 16

    `define AXI_MASTER_PORT(id)\
    wire sc_master_axi_AR_channel_t    id``_AR;\
    wire                               id``_ARVALID;\
    wire                               id``_ARREADY;\
    wire sc_master_axi_AW_channel_t    id``_AW;\
    wire                               id``_AWVALID;\
    wire                               id``_AWREADY;\
    wire sc_master_axi_W_channel_t     id``_W;\
    wire                               id``_WVALID;\
    wire                               id``_WREADY;\
    wire sc_master_axi_B_channel_t     id``_B;\
    wire                               id``_BVALID;\
    wire                               id``_BREADY;\
    wire sc_master_axi_R_channel_t     id``_R;\
    wire                               id``_RVALID;\
    wire                               id``_RREADY;

    `define AXI_SLAVE_PORT(id)\
    wire sc_slave_axi_AR_channel_t     id``_AR;\
    wire                               id``_ARVALID;\
    wire                               id``_ARREADY;\
    wire sc_slave_axi_AW_channel_t     id``_AW;\
    wire                               id``_AWVALID;\
    wire                               id``_AWREADY;\
    wire sc_slave_axi_W_channel_t      id``_W;\
    wire                               id``_WVALID;\
    wire                               id``_WREADY;\
    wire sc_slave_axi_B_channel_t      id``_B;\
    wire                               id``_BVALID;\
    wire                               id``_BREADY;\
    wire sc_slave_axi_R_channel_t      id``_R;\
    wire                               id``_RVALID;\
    wire                               id``_RREADY;

    `define VECTOR_TRACK(module_id, id, name, excluded)\
    int id``_desc;\
    initial id``_desc = $save_stimuli_open(name, module_id, excluded);\
    always @(negedge clock) $save_stimuli_log(id``_desc);

    `define UNPLUG_AXI_MASTER_PORT(port)\
    assign port``_ARVALID = 'b0;\
    assign port``_AWVALID = 'b0;\
    assign port``_WVALID = 'b0;\
    assign port``_BREADY = 'b0;\
    assign port``_RREADY = 'b0;

    `define UNPLUG_AXI_MASTER_PORT_INDEXED(port, index)\
    assign port``_ARVALID[index] = 'b0;\
    assign port``_AWVALID[index] = 'b0;\
    assign port``_WVALID[index] = 'b0;\
    assign port``_BREADY[index] = 'b0;\
    assign port``_RREADY[index] = 'b0;

    `define UNPLUG_AXI_SLAVE_PORT(port)\
    assign port``_ARREADY = 'b0;\
    assign port``_AWREADY = 'b0;\
    assign port``_WREADY = 'b0;\
    assign port``_BVALID = 'b0;\
    assign port``_RVALID = 'b0;

`endif

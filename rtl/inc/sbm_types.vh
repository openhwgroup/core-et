// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

typedef struct packed 
{
    logic                                       valid; //active
    logic   [3:0]                               counter; //rsp counter
    logic                                       error; //error found
    logic   [`SC_MESH_SLAVE_AXI_ID_SIZE-1:0]    axi_id; //id of axi req
} bcast_struct_sbm;

`ifndef SBM_AXI_Ax_CHANNEL_TYPE_DEF
`define SBM_AXI_Ax_CHANNEL_TYPE_DEF(ID_SIZE) \
struct packed { \
    logic [ID_SIZE-1:0]                         ID; \
    logic [`SC_MESH_MASTER_AXI_ADDR_SIZE-1:0]   ADDR; \
} 
`endif

typedef `SBM_AXI_Ax_CHANNEL_TYPE_DEF(`SC_MESH_SLAVE_AXI_ID_SIZE) sbm_slave_axi_AR_channel_t;
typedef `SBM_AXI_Ax_CHANNEL_TYPE_DEF(`SC_MESH_SLAVE_AXI_ID_SIZE) sbm_slave_axi_AW_channel_t;

typedef struct packed{ 
   logic [`ESR_APB_D_WIDTH-1:0]   DATA; 
} SBM_master_axi_W_channel_t;
typedef SBM_master_axi_W_channel_t sbm_slave_axi_W_channel_t;

typedef struct packed 
{
    logic                           reserved_pos;
    logic                           ARVALID;
    sbm_slave_axi_AR_channel_t      AR;
    logic                           AWVALID;
    sbm_slave_axi_AW_channel_t      AW;
    logic [`ESR_APB_D_WIDTH-1:0]    DATA;
} sbm_buffer_struct_t;

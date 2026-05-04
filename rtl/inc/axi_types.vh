// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

// axi_types.vh

////////////////////////////////////////////////////////////////////////////////
// ENUMS
////////////////////////////////////////////////////////////////////////////////
typedef enum logic [`AXI_RRESP_SIZE-1:0] {
   AXI_RESP_OKAY   = 2'b00,
   AXI_RESP_EXOKAY = 2'b01,
   AXI_RESP_SLVERR = 2'b10,
   AXI_RESP_DECERR = 2'b11
} axi_resp_t;


// DEBUG: axi port select enums per shire type
// shire channel axi port select
typedef enum logic [`DEBUG_AXI_PORT_SEL_SIZE-1:0] {
   SC_AXI_PORT_to_l3_mesh_master_port0 = 4'h0,
   SC_AXI_PORT_to_l3_mesh_master_port1 = 4'h1,
   SC_AXI_PORT_to_l3_mesh_master_port2 = 4'h2,
   SC_AXI_PORT_to_l3_mesh_master_port3 = 4'h3,
   SC_AXI_PORT_l3_mesh_slave_port0     = 4'h4,
   SC_AXI_PORT_l3_mesh_slave_port1     = 4'h5,
   SC_AXI_PORT_l3_mesh_slave_port2     = 4'h6,
   SC_AXI_PORT_l3_mesh_slave_port3     = 4'h7,
   SC_AXI_PORT_to_sys_mesh_master      = 4'h8,
   SC_AXI_PORT_uc_to_l3_mesh_master    = 4'hC,
   SC_AXI_PORT_uc_to_sys_mesh_master   = 4'hD,
   SC_AXI_PORT_sys_mesh_slave_uc       = 4'hE,
   SC_AXI_PORT_sys_mesh_slave_sbm      = 4'hF
} sc_axi_port_sel_t;

////////////////////////////////////////////////////////////////////////////////
// STRUCTS
////////////////////////////////////////////////////////////////////////////////


// AXI structs to contain channels.
// sc_master - widths for shire_cache (and shire_channel) master ports, including to_l3 and to_sys
// sc_slave  - widths for shire_cache (and shire_channel) slave  ports, including l3_slave
`ifndef SC_AXI_Ax_CHANNEL_TYPE_DEF
`define SC_AXI_Ax_CHANNEL_TYPE_DEF(ID_SIZE, ADDR_SIZE, USER_SIZE) \
struct packed { \
    logic [ID_SIZE-1:0]                         ID; \
    logic [ADDR_SIZE-1:0]                       ADDR; \
    logic [`AXI_AXLEN_SIZE-1:0]                 LEN; \
    logic [`AXI_AXSIZE_SIZE-1:0]                SIZE; \
    logic [`AXI_AXBURST_SIZE-1:0]               BURST; \
    logic                                       LOCK; \
    logic [`AXI_AXCACHE_SIZE-1:0]               CACHE; \
    logic [`AXI_AXPROT_SIZE-1:0]                PROT; \
    logic [`AXI_AXQOS_SIZE-1:0]                 QOS; \
    logic [USER_SIZE-1:0]                       USER; \
} 
`endif
typedef `SC_AXI_Ax_CHANNEL_TYPE_DEF(`SC_MESH_MASTER_AXI_ID_SIZE, `SC_MESH_MASTER_AXI_ADDR_SIZE, `SC_MESH_MASTER_AXI_ARUSER_SIZE) sc_master_axi_AR_channel_t;
typedef `SC_AXI_Ax_CHANNEL_TYPE_DEF(`SC_MESH_SLAVE_AXI_ID_SIZE,  `SC_MESH_MASTER_AXI_ADDR_SIZE, `SC_MESH_MASTER_AXI_ARUSER_SIZE) sc_slave_axi_AR_channel_t;
typedef sc_slave_axi_AR_channel_t                                                                                                sys_slave_axi_AR_channel_t;
typedef sc_slave_axi_AR_channel_t                                                                                                debug_axi_AR_channel_t;
typedef sc_master_axi_AR_channel_t                                                                                               mem_atomic_rsp_axi_AR_channel_t;
typedef `SC_AXI_Ax_CHANNEL_TYPE_DEF(`MEM_MASTER_AXI_ID_SIZE,     `MEM_MASTER_AXI_ADDR_SIZE,     1)                               mem_master_axi_AR_channel_t;

typedef `SC_AXI_Ax_CHANNEL_TYPE_DEF(`SC_MESH_MASTER_AXI_ID_SIZE, `SC_MESH_MASTER_AXI_ADDR_SIZE, `SC_MESH_MASTER_AXI_AWUSER_SIZE) sc_master_axi_AW_channel_t;
typedef `SC_AXI_Ax_CHANNEL_TYPE_DEF(`SC_MESH_SLAVE_AXI_ID_SIZE,  `SC_MESH_MASTER_AXI_ADDR_SIZE, `SC_MESH_MASTER_AXI_AWUSER_SIZE) sc_slave_axi_AW_channel_t;
typedef sc_slave_axi_AW_channel_t                                                                                                sys_slave_axi_AW_channel_t;
typedef sc_slave_axi_AW_channel_t                                                                                                debug_axi_AW_channel_t;
typedef sc_master_axi_AW_channel_t                                                                                               mem_atomic_rsp_axi_AW_channel_t;
typedef `SC_AXI_Ax_CHANNEL_TYPE_DEF(`MEM_MASTER_AXI_ID_SIZE,     `MEM_MASTER_AXI_ADDR_SIZE,     1)                               mem_master_axi_AW_channel_t;


`ifndef SC_AXI_W_CHANNEL_TYPE_DEF
`define SC_AXI_W_CHANNEL_TYPE_DEF(DATA_SIZE) \
struct packed{ \
   logic [DATA_SIZE-1:0]    DATA; \
   logic [DATA_SIZE/8-1:0]  STRB; \
   logic                    LAST; \
} 
`endif
typedef `SC_AXI_W_CHANNEL_TYPE_DEF(`SC_MESH_MASTER_AXI_DATA_SIZE)     sc_master_axi_W_channel_t;
typedef sc_master_axi_W_channel_t                                     sc_slave_axi_W_channel_t;
typedef `SC_AXI_W_CHANNEL_TYPE_DEF(`SYS_MESH_SLAVE_AXI_DATA_SIZE)     sys_slave_axi_W_channel_t;
typedef `SC_AXI_W_CHANNEL_TYPE_DEF(`DEBUG_AXI_DATA_SIZE)              debug_axi_W_channel_t;
typedef `SC_AXI_W_CHANNEL_TYPE_DEF(`MEM_ATOMIC_RSP_AXI_DATA_SIZE)     mem_atomic_rsp_axi_W_channel_t;
typedef sc_master_axi_W_channel_t                                     mem_master_axi_W_channel_t;
typedef `SC_AXI_W_CHANNEL_TYPE_DEF(`MEM_SBM_MESH_SLAVE_AXI_DATA_SIZE) mem_sbm_slave_axi_W_channel_t;

`ifndef SC_AXI_R_CHANNEL_TYPE_DEF
`define SC_AXI_R_CHANNEL_TYPE_DEF(ID_SIZE, DATA_SIZE) \
struct packed { \
   logic [ID_SIZE-1:0]                         ID; \
   logic [`AXI_RRESP_SIZE-1:0]                 RESP; \
   logic                                       LAST; \
   logic [DATA_SIZE-1:0]                       DATA; \
} 
`endif
typedef `SC_AXI_R_CHANNEL_TYPE_DEF(`SC_MESH_MASTER_AXI_ID_SIZE, `SC_MESH_MASTER_AXI_DATA_SIZE)     sc_master_axi_R_channel_t;
typedef `SC_AXI_R_CHANNEL_TYPE_DEF(`SC_MESH_SLAVE_AXI_ID_SIZE,  `SC_MESH_MASTER_AXI_DATA_SIZE)     sc_slave_axi_R_channel_t;
typedef `SC_AXI_R_CHANNEL_TYPE_DEF(`SC_MESH_SLAVE_AXI_ID_SIZE,  `SYS_MESH_SLAVE_AXI_DATA_SIZE)     sys_slave_axi_R_channel_t;
typedef `SC_AXI_R_CHANNEL_TYPE_DEF(`SC_MESH_SLAVE_AXI_ID_SIZE,  `DEBUG_AXI_DATA_SIZE)              debug_axi_R_channel_t;
typedef `SC_AXI_R_CHANNEL_TYPE_DEF(`SC_MESH_MASTER_AXI_ID_SIZE, `MEM_ATOMIC_RSP_AXI_DATA_SIZE)     mem_atomic_rsp_axi_R_channel_t;
typedef `SC_AXI_R_CHANNEL_TYPE_DEF(`MEM_MASTER_AXI_ID_SIZE,     `SC_MESH_MASTER_AXI_DATA_SIZE)     mem_master_axi_R_channel_t;
typedef `SC_AXI_R_CHANNEL_TYPE_DEF(`SC_MESH_SLAVE_AXI_ID_SIZE,  `MEM_SBM_MESH_SLAVE_AXI_DATA_SIZE) mem_sbm_slave_axi_R_channel_t;


`ifndef SC_AXI_B_CHANNEL_TYPE_DEF
`define SC_AXI_B_CHANNEL_TYPE_DEF(ID_SIZE) \
struct packed { \
   logic [ID_SIZE-1:0]                         ID; \
   logic [`AXI_RRESP_SIZE-1:0]                 RESP; \
} 
`endif
typedef `SC_AXI_B_CHANNEL_TYPE_DEF(`SC_MESH_MASTER_AXI_ID_SIZE) sc_master_axi_B_channel_t;
typedef `SC_AXI_B_CHANNEL_TYPE_DEF(`SC_MESH_SLAVE_AXI_ID_SIZE)  sc_slave_axi_B_channel_t;
typedef sc_slave_axi_B_channel_t                                sys_slave_axi_B_channel_t;
typedef sc_slave_axi_B_channel_t                                debug_axi_B_channel_t;
typedef sc_master_axi_B_channel_t                               mem_atomic_rsp_axi_B_channel_t;
typedef `SC_AXI_B_CHANNEL_TYPE_DEF(`MEM_MASTER_AXI_ID_SIZE)     mem_master_axi_B_channel_t;


typedef struct packed{
   logic                      ARREADY;
   logic                      ARVALID;
   sc_master_axi_AR_channel_t AR;
   logic                      AWREADY;
   logic                      AWVALID;
   sc_master_axi_AW_channel_t AW;
   logic                      WREADY;
   logic                      WVALID;
   sc_master_axi_W_channel_t  W;
   logic                      RREADY;
   logic                      RVALID;
   sc_master_axi_R_channel_t  R;
   logic                      BREADY;
   logic                      BVALID;
   sc_master_axi_B_channel_t  B;
} sc_master_axi_port_t;

typedef struct packed{
   logic                     ARREADY;
   logic                     ARVALID;
   sc_slave_axi_AR_channel_t AR;
   logic                     AWREADY;
   logic                     AWVALID;
   sc_slave_axi_AW_channel_t AW;
   logic                     WREADY;
   logic                     WVALID;
   sc_slave_axi_W_channel_t  W;
   logic                     RREADY;
   logic                     RVALID;
   sc_slave_axi_R_channel_t  R;
   logic                     BREADY;
   logic                     BVALID;
   sc_slave_axi_B_channel_t  B;
} sc_slave_axi_port_t;

typedef struct packed{
   logic                      ARREADY;
   logic                      ARVALID;
   sys_slave_axi_AR_channel_t AR;
   logic                      AWREADY;
   logic                      AWVALID;
   sys_slave_axi_AW_channel_t AW;
   logic                      WREADY;
   logic                      WVALID;
   sys_slave_axi_W_channel_t  W;
   logic                      RREADY;
   logic                      RVALID;
   sys_slave_axi_R_channel_t  R;
   logic                      BREADY;
   logic                      BVALID;
   sys_slave_axi_B_channel_t  B;
} sys_slave_axi_port_t;


typedef struct packed{ 
    sc_master_axi_AW_channel_t  AW;
    sc_master_axi_W_channel_t   W;
} write_req_axi_master_t;
typedef write_req_axi_master_t wr_req_axi_master_t;


typedef struct packed{
   logic                       ARREADY;
   logic                       ARVALID;
   mem_master_axi_AR_channel_t AR;
   logic                       AWREADY;
   logic                       AWVALID;
   mem_master_axi_AW_channel_t AW;
   logic                       WREADY;
   logic                       WVALID;
   mem_master_axi_W_channel_t  W;
   logic                       RREADY;
   logic                       RVALID;
   mem_master_axi_R_channel_t  R;
   logic                       BREADY;
   logic                       BVALID;
   mem_master_axi_B_channel_t  B;
} mem_master_axi_port_t;

// debug
typedef struct packed{
   logic                      ARREADY;
   logic                      ARVALID;
   debug_axi_AR_channel_t     AR;
   logic                      AWREADY;
   logic                      AWVALID;
   debug_axi_AW_channel_t     AW;
   logic                      WREADY;
   logic                      WVALID;
   debug_axi_W_channel_t      W;
   logic                      RREADY;
   logic                      RVALID;
   debug_axi_R_channel_t      R;
   logic                      BREADY;
   logic                      BVALID;
   debug_axi_B_channel_t      B;
} debug_axi_port_t;

typedef struct packed{
   logic [`DEBUG_AXI_PORT_SEL_SIZE-1:0] axi_port_sel;  // allows 16 unique axi port mux selection per shire type (minionshire, memshire, ioshire, etc) - cast to one of the enum types below
   logic [`SHIRE_BM_DWORD_MUX-1:0]      axi_dword_sel; // selects dword of data from each channel that carries data
   logic                                axi_trace_en;  // indicates tracing axi ports, use to clkgate relay stages  -> connect this to dm_active
} debug_axi_port_ctl_t;


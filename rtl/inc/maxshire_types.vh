// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

////////////////////////////////////////////////////////////////////////////////
// ENUMS
////////////////////////////////////////////////////////////////////////////////

typedef enum logic [3:0] {
    ET_LINK_MAXNEIGH_SRC_MEM    = ET_LINK_NEIGH_SRC_Minion0,
    ET_LINK_MAXNEIGH_SRC_MMIO   = ET_LINK_NEIGH_SRC_Minion1,
    ET_LINK_MAXNEIGH_SRC_GAG    = ET_LINK_NEIGH_SRC_Minion2
} et_link_maxneigh_source_t;


////////////////////////////////////////////////////////////////////////////////
// STRUCTS
////////////////////////////////////////////////////////////////////////////////

// APB STRUCT FOR ESR

`ifndef APB_ESR_PSTRB_TO_SLAVE_TYPE_DEF
`define APB_ESR_PSTRB_TO_SLAVE_TYPE_DEF(ADDR_WIDTH,DATA_SIZE) \
struct packed { \
    logic    [ADDR_WIDTH - 1:0]     apb_paddr;  \
    logic                           apb_penable; \
    logic                           apb_psel; \
    logic    [DATA_SIZE - 1:0]      apb_pwdata; \
    logic    [(DATA_SIZE/8) - 1:0]  apb_pstrb;  \
    logic                           apb_pwrite; \
}
`endif

typedef `APB_ESR_PSTRB_TO_SLAVE_TYPE_DEF( `MAXSHIRE_APB_AD_WIDTH, `MAXSHIRE_APB_D_WIDTH ) APB_to_maxshire_ESRs_t;

typedef `APB_ESR_TO_MASTER_TYPE_DEF( `MAXSHIRE_APB_D_WIDTH ) APB_from_maxshire_ESRs_t;

// Create a W channel type for the maxshire bus master and axi2apb bridge.
typedef `SC_AXI_W_CHANNEL_TYPE_DEF(`MAXSHIRE_SBM_APB_D_WIDTH)     mxn_sbm_slave_axi_W_channel_t;

// Extend sbm buffer struct to support new W channel type.
typedef struct packed
{
    logic                           reserved_pos;
    logic                           ARVALID;
    sbm_slave_axi_AR_channel_t      AR;
    logic                           AWVALID;
    sbm_slave_axi_AW_channel_t      AW;
    mxn_sbm_slave_axi_W_channel_t   W;
} mxn_sbm_buffer_struct_t;

// Maxion AXI channel types used in axi2etl bridge.
typedef `SC_AXI_Ax_CHANNEL_TYPE_DEF(`MXN_AXI_ID_SIZE, `MXN_AXI_ADDR_SIZE, `MXN_AXI_USER_SIZE) mxn_axi_AR_channel_t;
typedef `SC_AXI_Ax_CHANNEL_TYPE_DEF(`MXN_AXI_ID_SIZE, `MXN_AXI_ADDR_SIZE, `MXN_AXI_USER_SIZE) mxn_axi_AW_channel_t;

typedef struct packed{
   logic [`SC_MESH_MASTER_AXI_DATA_SIZE-1:0]   DATA;
   logic [`SC_MESH_MASTER_AXI_WSTRB_SIZE-1:0]  STRB;
   logic                                       LAST;
} mxn_axi_W_channel_t;


typedef `SC_AXI_R_CHANNEL_TYPE_DEF(`MXN_AXI_ID_SIZE, `SC_MESH_MASTER_AXI_DATA_SIZE) mxn_axi_R_channel_t;
typedef `SC_AXI_B_CHANNEL_TYPE_DEF(`MXN_AXI_ID_SIZE)                                mxn_axi_B_channel_t;

// Struct-of-structs for maxneigh TDR
// Includes SRAM configs 
// based on compiler type.  All SRAMs from the same
// compiler will use the same configs
typedef struct packed {
    logic [`MAXSHIRE_DFT_SCAN_DUMP_CNTL_WIDTH-1:0]  dft__scan_dump_cntl;
    logic                                           dft__ram_mei;
    logic                                           dft__ram_wei;
    ioshire_sram2p_cfg_t                            sadrl;
    ioshire_sram1p_cfg_t                            sadul;
    ioshire_sram1p_cfg_t                            sadcl;
    ioshire_sram1p_cfg_t                            sacrl;
} maxneigh_tdr_t;

// Struct-of-structs for maxshire TDR
// Doesn't have anything right now, but this
// is added in anticipation that it will be needed
typedef struct packed {
    logic [`MAXSHIRE_DFT_SCAN_DUMP_CNTL_WIDTH-1:0]  dft__scan_dump_cntl;
} maxshire_tdr_t;

// JTAG interface for RAM config
typedef sc_jtag_ram_cfg_t           mxn_jtag_tdr_cfg_t;

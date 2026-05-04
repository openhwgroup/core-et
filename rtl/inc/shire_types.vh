// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0


////////////////////////////////////////////////////////////////////////////////
// STRUCTS
////////////////////////////////////////////////////////////////////////////////
// DLL Delay Estimation
typedef struct packed {
   logic ctl_enable;
   logic ctl_init;
   logic ctl_start;
   logic [7:0] ctl_txn;
} esr_dll_dly_est_ctl_t;

typedef struct packed {
   logic [7:0] sts_errn;
   logic sts_done;
} esr_dll_dly_est_sts_t;

// Cooperative TLoad slave request
typedef struct packed {
    logic [`NUM_NEIGH_R]             master_neigh;
    logic [`SHIRE_COOP_ID_RANGE]     coop_id;
    logic [`SHIRE_COOP_SEQ_ID_RANGE] coop_seq_id;
    logic [`MIN_PER_N-1:0]           coop_min_mask;
} coop_tload_slv_req_t;

// Cooperative TLoad slave ready bus request
typedef struct packed {
    logic [`NUM_NEIGH_R]         dest_neigh;
    logic [`SHIRE_COOP_ID_RANGE] coop_id;
} coop_tload_slv_rdy_req_t;

// Cooperative TLoad tag table data
typedef struct packed {
    logic [`MIN_PER_N-1:0]                            coop_min_mask;
    logic [`MIN_PER_N-1:0][`SHIRE_COOP_MISS_ID_RANGE] min_miss_id;
} coop_tload_tag_table_data_t;

// Cooperative TLoad tag table request
typedef struct packed {
    logic [`SHIRE_COOP_LOAD_ID_RANGE] tag;
    coop_tload_tag_table_data_t       data;
} coop_tload_tag_table_req_t;

// Fill FIFO data
typedef struct packed {
    et_link_neigh_source_t                       dest;
    logic                                        is_coop;
    logic [`MIN_PER_N-1:0]                       coop_min_mask;
    logic [`MIN_PER_N-1:0][`ET_LINK_ID_SIZE-1:0] coop_min_id;
    et_link_rsp_info_t                           rsp;
} fill_fifo_data_t;

// APB STRUCT FOR ESR

`ifndef APB_ESR_TO_SLAVE_TYPE_DEF
`define APB_ESR_TO_SLAVE_TYPE_DEF(ADDR_WIDTH,DATA_SIZE) \
struct packed { \
    logic    [ADDR_WIDTH - 1:0]     apb_paddr;  \
    logic                           apb_penable; \
    logic                           apb_psel; \
    logic    [DATA_SIZE - 1:0]      apb_pwdata; \
    logic                           apb_pwrite; \
}
`endif

typedef `APB_ESR_TO_SLAVE_TYPE_DEF( `MIN_DM_APB_ADDR_WIDTH          , `ESR_APB_D_WIDTH )   APB_to_min_t;
typedef `APB_ESR_TO_SLAVE_TYPE_DEF( `NEIGH_DM_APB_ADDR_WIDTH        , `ESR_APB_D_WIDTH )   APB_to_neigh_t;
typedef `APB_ESR_TO_SLAVE_TYPE_DEF( `NEIGH_CH_DM_APB_ADDR_WIDTH     , `ESR_APB_D_WIDTH )   APB_to_neigh_channel_t;
typedef `APB_ESR_TO_SLAVE_TYPE_DEF( `ESR_NEIGH_APB_AD_PP_WIDTH      , `ESR_APB_D_WIDTH )   APB_to_neigh_esrs_t;
typedef `APB_ESR_TO_SLAVE_TYPE_DEF( `ICACHE_NEIGH_APB_ADDR_WIDTH    , `ESR_APB_D_WIDTH )   APB_to_neigh_icache_t;
typedef `APB_ESR_TO_SLAVE_TYPE_DEF( `TBOX_NEIGH_APB_ADDR_WIDTH      , `ESR_APB_D_WIDTH )   APB_to_neigh_tbox_t;
typedef `APB_ESR_TO_SLAVE_TYPE_DEF( `ESR_CACHE_BANK_APB_AD_PP_WIDTH , `ESR_APB_D_WIDTH )   APB_to_bank_ESRs_t;
typedef `APB_ESR_TO_SLAVE_TYPE_DEF( `RBOX_SHIRE_APB_ADDR_WIDTH      , `ESR_APB_D_WIDTH )   APB_to_rbox_ESRs_t;
typedef `APB_ESR_TO_SLAVE_TYPE_DEF( `ICACHE_MEMS_APB_ADDR_WIDTH     , `ESR_APB_D_WIDTH )   APB_to_shire_icache_t; 
typedef `APB_ESR_TO_SLAVE_TYPE_DEF( `APB_AD_SHIRE_TOP_WIDTH         , `ESR_APB_D_WIDTH )   APB_to_all_ESRs_t;
typedef `APB_ESR_TO_SLAVE_TYPE_DEF( `ICACHE_DATA_APB_ADDR_WIDTH     , `ESR_APB_D_WIDTH )   APB_to_cpu_icache_t; 
typedef `APB_ESR_TO_SLAVE_TYPE_DEF( `ESR_SHIRE_OTHER_APB_AD_PP_WIDTH, `ESR_APB_D_WIDTH )   APB_to_cpu_reg_t;
typedef `APB_ESR_TO_SLAVE_TYPE_DEF( `PLIC_APB_ADDR_WIDTH            , `PLIC_DATA_WIDTH )   APB_to_plic_t;

`ifndef APB_ESR_TO_MASTER_TYPE_DEF
`define APB_ESR_TO_MASTER_TYPE_DEF(DATA_SIZE) \
struct packed { \
    logic    [DATA_SIZE - 1:0]      apb_prdata; \
    logic                           apb_pready; \
    logic                           apb_pslverr; \
}
`endif

typedef `APB_ESR_TO_MASTER_TYPE_DEF( `ESR_APB_D_WIDTH ) APB_from_min_t;
typedef `APB_ESR_TO_MASTER_TYPE_DEF( `ESR_APB_D_WIDTH ) APB_from_neigh_t;
typedef `APB_ESR_TO_MASTER_TYPE_DEF( `ESR_APB_D_WIDTH ) APB_from_neigh_channel_t;
typedef `APB_ESR_TO_MASTER_TYPE_DEF( `ESR_APB_D_WIDTH ) APB_from_bank_ESRs_t;
typedef `APB_ESR_TO_MASTER_TYPE_DEF( `ESR_APB_D_WIDTH ) APB_from_rbox_ESRs_t;
typedef `APB_ESR_TO_MASTER_TYPE_DEF( `ESR_APB_D_WIDTH ) APB_from_all_ESRs_t;
typedef `APB_ESR_TO_MASTER_TYPE_DEF( `ESR_APB_D_WIDTH ) APB_from_cpu_icache_t;
typedef `APB_ESR_TO_MASTER_TYPE_DEF( `ESR_APB_D_WIDTH ) APB_from_cpu_reg_t;
typedef `APB_ESR_TO_MASTER_TYPE_DEF( `PLIC_DATA_WIDTH ) APB_from_plic_t;

//typedef struct packed {
//   logic       info_ov    ; 
//   logic [3:0] info_ram   ; 
//   logic       err_code   ; 
//   logic       imprecise  ; 
//   logic       enabled    ; 
//   logic       multiple   ; 
//   logic       valid	    ; 
//} esr_icache_err_log_t;
typedef `APB_ESR_TO_MASTER_TYPE_DEF( `ESR_APB_D_WIDTH ) APB_from_shire_icache_t;

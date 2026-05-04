// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`ifndef _DEBUG_DEFINES_
`define _DEBUG_DEFINES_

// Number of cycles of delay that a signal suffers when is sent from the
// neighborhood to the shire and viceversa
`define DBG_SIGNALS_NEIGH_VC_DELAY  5
`define DBG_SIGNALS_NEIGH_MIN_DELAY 2
`define DBG_SIGNALS_NEIGH_TBOX_DELAY 2
`define DBG_SIGNALS_NEIGH_CH_DELAY 2

`define DBG_SC_NUM_PORTS 4

// Number of elements for APB sync in IOShire (RTLMIN-4404)
`define APB_CDC_NUM_ELEMS 6

///////////////////////////////////////////////////
// DEBUG FABRIC INSTANCE IDs
///////////////////////////////////////////////////

///////////////////////////////////////
// Each debug fabric module has a port named instance_id, we should follow
// a convention to specify its value: {Shire ID, subgroup, module ID}
//////////////////////////////////////
//
// Shire ID
`define DEBUG_ID_IOSHIRE        4'h0
`define DEBUG_ID_MINIONSHIRE    4'h1
`define DEBUG_ID_MEMSHIRE       4'h2
`define DEBUG_ID_PSHIRE         4'h3

// IOShire subgroup
`define DEBUG_IO_ID_ROOT        6'h0
`define DEBUG_IO_ID_SPIO        6'h1
`define DEBUG_IO_ID_PU          6'h2
`define DEBUG_IO_ID_MAXION      6'h3

// Minion Shire subgroup must be shire_phy_id
// MemShire subgtoup should also be shire_phy_id

//Analytic modules ID
`define DEBUG_ME_ID     6'h00
`define DEBUG_BPAM_ID   6'h01
`define DEBUG_TE_ID     6'h02
`define DEBUG_SMN_ID    4'h3  //special for minion shire neighborhoods
`define DEBUG_SM0_ID    6'h03
`define DEBUG_SM1_ID    6'h04
`define DEBUG_SM2_ID    6'h05
`define DEBUG_BM0_ID    6'h06
`define DEBUG_BM1_ID    6'h07
`define DEBUG_DMA_ID    6'h08
`define DEBUG_SMB_ID    6'h09
`define DEBUG_SI_ID     6'h0A
`define DEBUG_JPAM_ID   6'h0B
`define DEBUG_USB2_ID   6'h0C
`define DEBUG_JTAG_ID   6'h0D

///////////////////////////////////////////////////
// DEBUG RESET CONF
///////////////////////////////////////////////////
`define DEBUG_RST_PULSE_CYCLES 32

///////////////////////////////////////////////////
// COMMON CONFIGURATION THROUGH DEBUG FABRIC MODULES
///////////////////////////////////////////////////

// Pipelining
`define neigh_sm_pipeline        1 //Allowed values for SM 0,1
`define shire_etlink_sm_pipeline 1
`define shire_cache_sm_pipeline  1
`define te_pipeline              2 //Allowed values for TE 0,1,2
`define bm_pipeline              1
`define me_pipeline              1

//Index length
`define US_INDEX_LENGTH     10

//Clock disable --> Allows to apply clock gating to the ports that are not
//used
`define us_clk_disable 1

//Allocation --> Allows to claim for specific resources of a module, this is
//used in case there are more than one system for debug
`define us_allocation   0

// GPIO configuration
`define neigh_sm_gpio_en            1
`define shire_etlink_sm_gpio_en     1
`define shire_cache_sm_gpio_en      1
`define shire_bm_gpio_en            1
`define shire_te_gpio_en            1
`define shire_bpam_gpio_en          1

`define shire_cache_sm_gpio_width   `SC_BANK_ID_SIZE
`define SHIRE_TE_GPIO_WIDTH         (`NUM_NEIGH_L + `MIN_PER_N_L + 1)
`define shire_bpam_gpio_out_width    9
`define shire_bpam_gpio_in_width     1

`define us_gpio_rst_value '0

//Level shifters
`define neigh_sm_lvl_shifter        0 //Signals between hi/lo voltage
`define shire_cache_sm_lvl_shifter  0 //All signals in hi voltage
`define shire_etlink_sm_lvl_shifter 0 //All signals in hi voltage
`define shire_bm_lvl_shifter        0 //All signals in hi voltage

//Test parameters
`define sm_test_mem_type    1
`define sm_test_in_width    1
`define sm_test_out_width   1
`define bm_test_mem_type    1
`define bm_test_in_width    1
`define bm_test_out_width   1
`define te_test_mem_type    1
`define te_test_in_width    1
`define te_test_out_width   1

///////////////////////////////////////////////////
// NEIGHBORHOOD STATUS MONITOR
///////////////////////////////////////////////////

// mux config
`define NEIGH_DEBUG_FILTER_WIDTH      200
`define NEIGH_DEBUG_DATA_WIDTH        128
`define NEIGH_DEBUG_MATCH_WIDTH        64
`define NEIGH_DEBUG_SM_MUX_DATA_WIDTH   2
`define NEIGH_DEBUG_SM_MUX_MOD_WIDTH    5
`define NEIGH_DEBUG_SM_MUX_WIDTH      (`NEIGH_DEBUG_SM_MUX_DATA_WIDTH + `NEIGH_DEBUG_SM_MUX_MOD_WIDTH)

// Target selector
`define NEIGH_SM_TARGET_SEL      4
`define NEIGH_SM_TARGET_MIN0     `NEIGH_SM_TARGET_SEL'd0
`define NEIGH_SM_TARGET_MIN1     `NEIGH_SM_TARGET_SEL'd1
`define NEIGH_SM_TARGET_MIN2     `NEIGH_SM_TARGET_SEL'd2
`define NEIGH_SM_TARGET_MIN3     `NEIGH_SM_TARGET_SEL'd3
`define NEIGH_SM_TARGET_MIN4     `NEIGH_SM_TARGET_SEL'd4
`define NEIGH_SM_TARGET_MIN5     `NEIGH_SM_TARGET_SEL'd5
`define NEIGH_SM_TARGET_MIN6     `NEIGH_SM_TARGET_SEL'd6
`define NEIGH_SM_TARGET_MIN7     `NEIGH_SM_TARGET_SEL'd7
`define NEIGH_SM_TARGET_TBOX     `NEIGH_SM_TARGET_SEL'd8
`define NEIGH_SM_TARGET_NEIGH_CH `NEIGH_SM_TARGET_SEL'd9

`define neigh_sm_gpio_width      (`NEIGH_DEBUG_SM_MUX_WIDTH + `NEIGH_SM_TARGET_SEL)

// Neighborhood channel debug signals
`define NEIGH_CH_DEBUG_SM_MOD_MIN0   `NEIGH_DEBUG_SM_MUX_MOD_WIDTH'd0
`define NEIGH_CH_DEBUG_SM_MOD_MIN1   `NEIGH_DEBUG_SM_MUX_MOD_WIDTH'd1
`define NEIGH_CH_DEBUG_SM_MOD_MIN2   `NEIGH_DEBUG_SM_MUX_MOD_WIDTH'd2
`define NEIGH_CH_DEBUG_SM_MOD_MIN3   `NEIGH_DEBUG_SM_MUX_MOD_WIDTH'd3
`define NEIGH_CH_DEBUG_SM_MOD_MIN4   `NEIGH_DEBUG_SM_MUX_MOD_WIDTH'd4
`define NEIGH_CH_DEBUG_SM_MOD_MIN5   `NEIGH_DEBUG_SM_MUX_MOD_WIDTH'd5
`define NEIGH_CH_DEBUG_SM_MOD_MIN6   `NEIGH_DEBUG_SM_MUX_MOD_WIDTH'd6
`define NEIGH_CH_DEBUG_SM_MOD_MIN7   `NEIGH_DEBUG_SM_MUX_MOD_WIDTH'd7
`define NEIGH_CH_DEBUG_SM_MOD_ICACHE `NEIGH_DEBUG_SM_MUX_MOD_WIDTH'd8
`define NEIGH_CH_DEBUG_SM_MOD_PTW    `NEIGH_DEBUG_SM_MUX_MOD_WIDTH'd9
`define NEIGH_CH_DEBUG_SM_MOD_TLOAD  `NEIGH_DEBUG_SM_MUX_MOD_WIDTH'd10
`define NEIGH_CH_DEBUG_SM_MOD_TSTORE `NEIGH_DEBUG_SM_MUX_MOD_WIDTH'd11
`define NEIGH_CH_DEBUG_SM_MOD_FLN    `NEIGH_DEBUG_SM_MUX_MOD_WIDTH'd12
`define NEIGH_CH_DEBUG_SM_MOD_TBOX   `NEIGH_DEBUG_SM_MUX_MOD_WIDTH'd13
`define NEIGH_CH_DEBUG_SM_MOD_DPATH  `NEIGH_DEBUG_SM_MUX_MOD_WIDTH'd14
`define NEIGH_CH_DEBUG_SM_MOD_FLB    `NEIGH_DEBUG_SM_MUX_MOD_WIDTH'd15

`define NEIGH_CH_DEBUG_SM_TLOAD_DATA_EXT 1'b0
`define NEIGH_CH_DEBUG_SM_TLOAD_DATA_INT 1'b1

// module config
`define neigh_sm_upstream             3
`define neigh_sm_downstream           6
`define neigh_sm_sync_stages          0

`define neigh_sm_trace_en             2
`define neigh_sm_sequencer_en         1
`define neigh_sm_event_en             0

`define neigh_sm_bypass               0 //message inputs registered when 0
`define neigh_sm_sts_bypass           0 //signals inputs registered when 0

`define neigh_sm_qual_number         10
`define neigh_sm_comp_number          8
`define neigh_sm_filter_number        4
`define neigh_sm_counter_number       3
`define neigh_sm_accum_number         0
`define neigh_sm_accum_minmax         0

`define neigh_sm_match_width        `NEIGH_DEBUG_MATCH_WIDTH
`define neigh_sm_data_trace_width   `NEIGH_DEBUG_DATA_WIDTH
`define neigh_sm_filter_width       `NEIGH_DEBUG_FILTER_WIDTH
`define neigh_sm_counter_width       32
`define neigh_sm_accum_width         16
`define neigh_sm_event_width          1
`define neigh_sm_timer_width         32

`define neigh_sm_match_thresh        16
`define neigh_sm_filter_thresh       16
`define neigh_sm_counter_thresh       8
`define neigh_sm_sequencer_thresh     8

`define neigh_sm_trace_depth          8
`define neigh_sm_ups_depth            0
`define neigh_sm_ds_depth             4

///////////////////////////////////////////////////
// SHIRE ET-LINK AND RBOX STATUS MONITOR
///////////////////////////////////////////////////


`define SHIRE_ETLINK_FILTER_REQ_GPIO          3
`define SHIRE_ETLINK_FILTER_RSP_GPIO          3
`define SHIRE_ETLINK_DATA_WORD_REQ_GPIO       6
`define SHIRE_ETLINK_DATA_WORD_RSP_GPIO       5
`define SHIRE_ETLINK_DATA_WORD_GPIO          (`SHIRE_ETLINK_DATA_WORD_REQ_GPIO +\
                                              `SHIRE_ETLINK_DATA_WORD_RSP_GPIO)

`define SHIRE_ETLINK_SM_GPIO_MUX             (`SHIRE_ETLINK_FILTER_REQ_GPIO + \
                                              `SHIRE_ETLINK_FILTER_RSP_GPIO + \
                                              `SHIRE_ETLINK_DATA_WORD_GPIO )

`define SHIRE_ETLINK_SM_GPIO_WIDTH           (`SHIRE_ETLINK_SM_GPIO_MUX + 1) //+1(MSB) to choose between rbox and et-link

`define SHIRE_ETLINK_DATA_WORD_REQ_START     (`SHIRE_ETLINK_FILTER_REQ_GPIO+`SHIRE_ETLINK_FILTER_RSP_GPIO)
`define SHIRE_ETLINK_DATA_WORD_RSP_START     (`SHIRE_ETLINK_DATA_WORD_REQ_START+`SHIRE_ETLINK_DATA_WORD_REQ_GPIO)

`define SHIRE_ETLINK_SM_FILTER_REQ          77
`define SHIRE_ETLINK_SM_FILTER_RSP          23

`define SHIRE_ETLINK_SM_DATA_REQ            88
`define SHIRE_ETLINK_SM_DATA_RSP            38

`define RBOX_SM_GPIO_WIDTH                 `SHIRE_ETLINK_SM_GPIO_MUX
`define RBOX_SM_MATCH_WIDTH                `SHIRE_ETLINK_SM_MATCH_WIDTH
`define RBOX_SM_DATA_TRACE_WIDTH           `SHIRE_ETLINK_SM_DATA_TRACE_WIDTH
`define RBOX_SM_FILTER_WIDTH               `SHIRE_ETLINK_SM_FILTER_WIDTH

`define shire_etlink_sm_upstream              3
`define shire_etlink_sm_downstream            6
`define shire_etlink_sm_sync_stages           0

`define shire_etlink_sm_trace_en              2
`define shire_etlink_sm_sequencer_en          1
`define shire_etlink_sm_event_en              0

`define shire_etlink_sm_bypass                0 //message inputs registered when 0
`define shire_etlink_sm_sts_bypass            0 //signals inputs registered when 0

`define shire_etlink_sm_qual_number           4
`define shire_etlink_sm_comp_number           8
`define shire_etlink_sm_filter_number         4
`define shire_etlink_sm_counter_number        3
`define shire_etlink_sm_accum_number          0
`define shire_etlink_sm_accum_minmax          0

`define SHIRE_ETLINK_SM_MATCH_WIDTH          30
`define SHIRE_ETLINK_SM_DATA_TRACE_WIDTH    (`SHIRE_ETLINK_SM_DATA_REQ + `SHIRE_ETLINK_SM_DATA_RSP)
`define SHIRE_ETLINK_SM_FILTER_WIDTH        (`SHIRE_ETLINK_SM_FILTER_REQ + `SHIRE_ETLINK_SM_FILTER_RSP)
`define shire_etlink_sm_counter_width        32
`define shire_etlink_sm_accum_width          16
`define shire_etlink_sm_event_width           1
`define shire_etlink_sm_timer_width          32

`define shire_etlink_sm_match_thresh         16
`define shire_etlink_sm_filter_thresh        16
`define shire_etlink_sm_counter_thresh        8
`define shire_etlink_sm_sequencer_thresh      8

`define shire_etlink_sm_trace_depth           8
`define shire_etlink_sm_ups_depth             0
`define shire_etlink_sm_ds_depth              4

///////////////////////////////////////////////////
// SHIRE BUS MONITOR
///////////////////////////////////////////////////

// General parameters
`define shire_bm_upstream                3
`define shire_bm_downstream              6
`define shire_bm_timer_width            32
`define shire_bm_sync_stages             0
`define shire_bm_ups_depth               0
`define shire_bm_ds_depth                8

// Capabilities
`define shire_bm_tracker_type            0
`define shire_bm_conc_read               64
`define shire_bm_conc_write              64

// Bypass parameters
`define shire_bm_bypass                  0
`define shire_bm_axi_bypass              0

// Trace parameters
`define shire_bm_trace                   2
`define shire_bm_trace_shared            0
`define shire_bm_trace_ptime_width       0
`define shire_bm_trace_addr_depth        8
`define shire_bm_trace_data_depth        4
`define shire_bm_trace_resp_depth        8

// Monitor parameters
`define shire_bm_itas                    1
`define shire_bm_num_count               2
`define shire_bm_num_filter              2
`define shire_bm_num_grp_filters         1
`define shire_bm_write_data_notvalid     1

`define shire_bm_itas_width             16
`define shire_bm_counter_width          32
`define shire_bm_couter_minmax           1
`define shire_bm_grp_thrhld_width       11

// Filter configuration parameters
`define shire_bm_filter_addr          4'hF
`define shire_bm_filter_beat          3'h0 //3'h1
`define shire_bm_filter_burst         4'h0 //4'hD
`define shire_bm_filter_cache         4'h0 //4'h5
`define shire_bm_filter_data          3'h0 //3'h5
`define shire_bm_filter_id            4'hF
`define shire_bm_filter_len           4'h0 //4'h5
`define shire_bm_filter_lock          4'h0 //4'h5
`define shire_bm_filter_prot          4'h0 //4'h5
`define shire_bm_filter_qos           4'h0 //4'hF
`define shire_bm_filter_region        4'h0 //4'h0
`define shire_bm_filter_resp          4'h7
`define shire_bm_filter_size          4'hF
`define shire_bm_filter_awuser        4'h7
`define shire_bm_filter_aruser        4'h0
`define shire_bm_filter_wuser         3'h7
`define shire_bm_filter_ruser         3'h0
`define shire_bm_filter_buser         3'h0

// AXI parameters
`define shire_bm_axi_enable              1
`define shire_bm_axi_addr_width         40
`define SHIRE_BM_AXI_DATA_WIDTH         `DEBUG_AXI_DATA_SIZE
`define SHIRE_BM_AXI_ID_WIDTH           `SC_MESH_SLAVE_AXI_ID_SIZE
`define shire_bm_axi_id_range           (`SHIRE_BM_AXI_ID_WIDTH-1):0
`define shire_bm_axi_strb_width         `DEBUG_AXI_STRB_SIZE

`define shire_bm_axi_awregion            4
`define shire_bm_axi_arregion            4
`define shire_bm_axi_awuser_width        `SC_MESH_MASTER_AXI_AWUSER_SIZE
`define shire_bm_axi_wuser_width         `DEBUG_AXI_STRB_SIZE
`define shire_bm_axi_aruser_width        1
`define shire_bm_axi_ruser_width         1
`define shire_bm_axi_buser_width         1

// Counters support
`define shire_bm_count_duration          1
`define shire_bm_count_dlatency          1
`define shire_bm_count_rlatency          1
`define shire_bm_count_tlatency          1
`define shire_bm_count_ahesitancy        1
`define shire_bm_count_dhesitancy        1
`define shire_bm_count_rhesitancy        1
`define shire_bm_count_thesitancy        1
`define shire_bm_count_beats             1
`define shire_bm_count_bytes             1

`define SHIRE_BM_DWORD_MUX_SC            3 // SC AXI data interfaces is 512b wide
`define SHIRE_BM_DWORD_MUX_UC_TO_NOC     3 // uc to l3/sys AXI data interfaces are 512b wide
`define SHIRE_BM_DWORD_MUX_SYS_SLAVE     2 // sys mesh slave AXI Data interface is 256b wide

`define SHIRE_BM_DWORD_MUX               `MAX(`SHIRE_BM_DWORD_MUX_SC, \
                                              `MAX(`SHIRE_BM_DWORD_MUX_UC_TO_NOC, \
                                                   `SHIRE_BM_DWORD_MUX_SYS_SLAVE) )

`define SHIRE_BM_GPIO_WIDTH              (`DEBUG_AXI_PORT_SEL_SIZE + `SHIRE_BM_DWORD_MUX )

///////////////////////////////////////////////////
// SHIRE CACHE STATUS MONITOR
///////////////////////////////////////////////////

`define shire_cache_sm_upstream               3
`define shire_cache_sm_downstream             6
`define shire_cache_sm_sync_stages            0

`define shire_cache_sm_trace_en               2
`define shire_cache_sm_sequencer_en           1
`define shire_cache_sm_event_en               0

`define shire_cache_sm_bypass                 0 //message inputs registered when 0
`define shire_cache_sm_sts_bypass             0 //signals inputs registered when 0

`define shire_cache_sm_qual_number           10
`define shire_cache_sm_comp_number            8
`define shire_cache_sm_filter_number          4
`define shire_cache_sm_counter_number         3
`define shire_cache_sm_accum_number           0
`define shire_cache_sm_accum_minmax           0

`define shire_cache_sm_match_width           55
`define shire_cache_sm_data_trace_width     165
`define shire_cache_sm_filter_width          55
`define shire_cache_sm_counter_width         32
`define shire_cache_sm_accum_width           16
`define shire_cache_sm_event_width            1
`define shire_cache_sm_timer_width           32

`define shire_cache_sm_match_thresh          16
`define shire_cache_sm_filter_thresh         16
`define shire_cache_sm_counter_thresh         8
`define shire_cache_sm_sequencer_thresh       8

`define shire_cache_sm_trace_depth            8
`define shire_cache_sm_ups_depth              0
`define shire_cache_sm_ds_depth               4

///////////////////////////////////////////////////
// SHIRE TRACE ENCODER
///////////////////////////////////////////////////

// General parameters
`define shire_te_upstream                 3
`define shire_te_downstream               5
`define shire_te_timer_width             32
`define shire_te_sync_stages              0
`define shire_te_bypass                   0
`define shire_te_auth_supp                0

// TE config
`define shire_te_rv32                     0
`define shire_te_retires_per_cycle        1
`define shire_te_itrace_size             48
`define shire_te_itrace_fast              0
`define shire_te_no_context               1
`define shire_te_no_trap_val              0
`define shire_te_instr_lsb                1
`define shire_te_priv_rst                4'hF

// Monitor signals
`define shire_te_comparators_number       8
`define shire_te_filter_number            5
`define shire_te_counter_number           0
`define shire_te_exceptions_number        5

`define shire_te_counter_width           16
`define shire_te_context_width            1
`define shire_te_ecause_width             5
`define shire_te_privilege_width          2
`define shire_te_status_width             1
`define shire_te_user_width               1
`define shire_te_instr_addr_width        `VA_SIZE_EXT
`define shire_te_instr_width             32
//`define shire_te_val_width                8

// Filter configuration parameters
`define shire_te_filter_context           0
`define shire_te_filter_ecause            2
`define shire_te_filter_interrupt         1
`define shire_te_filter_privilege         1
`define shire_te_filter_trap_val          1

// Fifo depth parameters
`define shire_te_ups_depth                0
`define shire_te_ds_depth                 4
`define shire_te_fetch_depth              0

///////////////////////////////////////////////////
// SHIRE BPAM
///////////////////////////////////////////////////
// General parameters
`define shire_bpam_upstream                  6
`define shire_bpam_downstream                6
`define shire_bpam_timer_width              32
`define shire_bpam_sync_stages               0
`define shire_bpam_bypass                    0

`define shire_bpam_aux_domain                0
`define shire_bpam_notify_depth              4

`define shire_bpam_apb4                      0
`define shire_bpam_dma_ch                    1

`define SHIRE_BPAM_APB_ADDR_WIDTH           25
`define shire_bpam_apb_data_width           `ESR_APB_D_WIDTH  //64b

`define shire_bpam_event_in_en               0
`define shire_bpam_event_in_width            1
`define shire_bpam_event_out_en              1
`define shire_bpam_event_out_width           7
`define shire_bpam_event_cross_ret           1

`define shire_bpam_ds_depth                  4

`define clog2min1(x) (x > 1 ? $clog2(x) : 1)

///////////////////////////////////////////////////
// SHIRE MESSAGE ENGINE
///////////////////////////////////////////////////
`define SHIRE_ME_TIME_ENABLE    1
`define shire_me_upstream       7
`define shire_me_downstream     7
`define shire_me_gpio_width     (3*`SHIRE_NOC_BRIDGE_HP_WIDTH) // 3*8
`define shire_me_gpio_en        1
`define ME_SHIRE_EVENT_MASK_W  16
///////////////////////////////////////////////////
// Debug NoC bridge --> Values depend on Hannan spreadsheet
///////////////////////////////////////////////////
//Ports with constant values
`define SHIRE_NOC_BRIDGE_DOWNSTREAM `shire_me_downstream
`define SHIRE_NOC_BRIDGE_UPSTREAM   `shire_me_upstream

`define shire_noc_bridge_index_adjust `US_INDEX_LENGTH'h00

`define shire_noc_bridge_bypass     '0
`define shire_noc_bridge_root        1
`define shire_noc_bridge_leaf        0
`define shire_noc_bridge_event_tiers `MEMSHIRE_NORMAL_BRIDGE_EVENT_TIERS
`define shire_noc_bridge_groups      1
`define shire_noc_bridge_grp_index   0
`define shire_noc_bridge_grp_mask    0

`define DBG_NOC_BRIDGE_EVENT_BITS  (`SHIRE_NOC_BRIDGE_HP_WIDTH * `ust_psum4(`shire_noc_bridge_event_tiers, 8))

// Synchronization
`define shire_noc_bridge_sync_hostid    0
`define shire_noc_bridge_udb2noc_sync   0 // Between the debug NoC and the bridge we have voltage(+clock) crossing FIFOS,
`define shire_noc_bridge_noc2udb_sync   0 // so both interfaces have the same clock

//NSIP interface
`define shire_noc_bridge_data_width    128
`define SHIRE_NOC_BRIDGE_HP_WIDTH        8
`define shire_noc_bridge_flow_depth      9
`define shire_noc_bridge_flow_event      9

///////////////////////////////////////////////////
// DEBUG MEMORY MAP
///////////////////////////////////////////////////
// (w/o 3 LSB which are 0)

`define debug_apb_addr_width        22

// Minion APB address
`define MIN_REG_APB_ADDR_WIDTH      12 // {PP, thread, @[11:3] }
`define MIN_DCACHE_APB_ADDR_WIDTH   10
`define MIN_DM_APB_ADDR_WIDTH       (`MAX(`MIN_REG_APB_ADDR_WIDTH, `MIN_DCACHE_APB_ADDR_WIDTH) + 1) //+1b(Debug Mode)

// Neighborhood defines
`define TBOX_NEIGH_APB_ADDR_WIDTH    12
`define ICACHE_NEIGH_APB_ADDR_WIDTH  10
`define NEIGH_CH_DM_APB_ADDR_WIDTH   (1 + (`MAX(`TBOX_NEIGH_APB_ADDR_WIDTH,\
                                                `MAX(`ESR_NEIGH_APB_AD_PP_WIDTH,\
                                                     `ICACHE_NEIGH_APB_ADDR_WIDTH) ) ) ) //+1b(Debug Mode)

`define NEIGH_DM_APB_ADDR_WIDTH (`MAX(`NEIGH_CH_DM_APB_ADDR_WIDTH, `MIN_DM_APB_ADDR_WIDTH ) + 1) //+1b(Debug Mode)

`define NEIGH_DM_APB_ADDR_REG_RANGE 8:0
`define NEIGH_DM_APB_ADDR_THREAD_RANGE 9:9
`define NEIGH_DM_APB_ADDR_DEST_RANGE `NEIGH_DM_APB_ADDR_WIDTH-1:`NEIGH_DM_APB_ADDR_WIDTH-1
`define NEIGH_DM_APB_ADDR_PP_RANGE `NEIGH_DM_APB_ADDR_WIDTH-3:`NEIGH_DM_APB_ADDR_WIDTH-4
`define NEIGH_DM_APB_ADDR_MIN_RANGE 12:10



//Shire BPAM
`define RBOX_SHIRE_APB_ADDR_WIDTH   17
`define ICACHE_DATA_APB_ADDR_WIDTH  13
`define ICACHE_MEMS_APB_NEIGH_WIDTH  3
`define ICACHE_MEMS_APB_ADDR_WIDTH  (`ICACHE_MEMS_APB_NEIGH_WIDTH + 3 + `ICACHE_DATA_APB_ADDR_WIDTH)

//Shire APB width
`define APB_AD_SHIRE_TOP_WIDTH `MAX(`ESR_APB_AD_PP_WIDTH, \
                                 `MAX(`ICACHE_MEMS_APB_ADDR_WIDTH,`NEIGH_DM_APB_ADDR_WIDTH ) )


///////////////////////////////////////////////////
// MEMSHIRE DEBUG
///////////////////////////////////////////////////



`define DEBUG_INDEX_LENGTH           10
`define DEBUG_UST_IDCODE             32'h0000_0000
`define DEBUG_UST_CHIP_INFO          8'h0
`define DEBUG_NSIP_MSG_DATA_WIDTH    128
`define DEBUG_NSIP_NSP_EVT_DATA_WIDTH    32
`define DEBUG_NSIP_EVT_DATA_WIDTH    32
`define DEBUG_NSIP_HOST_PORT_WIDTH   8

`define MEMSHIRE_MSG_LOCK_US_RESET   `ME_MSG_LOCK_US_RESET_P
`define MEMSHIRE_MSG_LOCK_DS_RESET   `ME_MSG_LOCK_DS_RESET_P
`define MEMSHIRE_MSG_LOCK_POLARITY   `ME_MSG_LOCK_POLARITY

///////////////////////////////////////////////////
// MEMSHIRE
///////////////////////////////////////////////////
`define MEMSHIRE_INDEX_LENGTH              `US_INDEX_LENGTH
`define MEMSHIRE_CLK_DISABLE               `us_clk_disable
`define MEMSHIRE_ALLOCATION                `us_allocation

///////////////////////////////////////////////////
// MEMSHIRE BPAM
///////////////////////////////////////////////////
// General parameters
`define MEMSHIRE_BPAM_UPSTREAM                  6    // 64b ESR
`define MEMSHIRE_BPAM_DOWNSTREAM                6
`define MEMSHIRE_BPAM_TIMER_WIDTH              32
`define MEMSHIRE_BPAM_SYNC_STAGES               0
`define MEMSHIRE_BPAM_BYPASS                    0

`define MEMSHIRE_BPAM_AUX_DOMAIN                0
`define MEMSHIRE_BPAM_NOTIFY_DEPTH              1

`define MEMSHIRE_BPAM_APB4                      0
`define MEMSHIRE_BPAM_DMA_CH                    1

`define MEMSHIRE_BPAM_EVENT_IN_EN               0
`define MEMSHIRE_BPAM_EVENT_IN_WIDTH            1
`define MEMSHIRE_BPAM_EVENT_OUT_EN              0
`define MEMSHIRE_BPAM_EVENT_OUT_WIDTH           1
`define MEMSHIRE_BPAM_EVENT_CROSS_RET           0

`define MEMSHIRE_BPAM_DS_DEPTH                  1

`define MEMSHIRE_BPAM_GPIO_EN                   1
`define MEMSHIRE_BPAM_GPIO_OUT_WIDTH            1
`define MEMSHIRE_BPAM_GPIO_IN_WIDTH             1
`define MEMSHIRE_BPAM_GPIO_RST_VALUE            '0

///////////////////////////////////////////////////
// MEMSHIRE STATUS MONITOR
///////////////////////////////////////////////////

`define MEMSHIRE_SM_UPSTREAM               3
`define MEMSHIRE_SM_DOWNSTREAM             7
`define MEMSHIRE_SM_SYNC_STAGES            0

`define MEMSHIRE_SM_TRACE_EN               2
`define MEMSHIRE_SM_SEQUENCER_EN           0
`define MEMSHIRE_SM_EVENT_EN               0

`define MEMSHIRE_SM_BYPASS                 0 //message inputs registered when 0
`define MEMSHIRE_SM_STS_BYPASS             0 //signals inputs registered when 0
`define MEMSHIRE_SM_PIPELINE               0

`define MEMSHIRE_SM_QUAL_NUMBER            4
`define MEMSHIRE_SM_COMP_NUMBER            0
`define MEMSHIRE_SM_FILTER_NUMBER          0
`define MEMSHIRE_SM_COUNTER_NUMBER         1
`define MEMSHIRE_SM_ACCUM_NUMBER           0
`define MEMSHIRE_SM_ACCUM_MINMAX           0
`define MEMSHIRE_SM_GPIO_WIDTH             1
`define MEMSHIRE_SM_GPIO_EN                0
`define MEMSHIRE_SM_GPIO_RST_VALUE         '0

`define MEMSHIRE_SM_MATCH_WIDTH            8
`define MEMSHIRE_SM_FILTER_WIDTH           1
`define MEMSHIRE_SM_COUNTER_WIDTH         32
`define MEMSHIRE_SM_ACCUM_WIDTH            1
`define MEMSHIRE_SM_EVENT_WIDTH            1
`define MEMSHIRE_SM_TIMER_WIDTH           32

`define MEMSHIRE_SM_MATCH_THRESH           4
`define MEMSHIRE_SM_FILTER_THRESH          0
`define MEMSHIRE_SM_COUNTER_THRESH         8
`define MEMSHIRE_SM_SEQUENCER_THRESH       0

`define MEMSHIRE_SM_TRACE_DEPTH            8
`define MEMSHIRE_SM_US_DEPTH               0
`define MEMSHIRE_SM_DS_DEPTH               0

`define MEMSHIRE_SM_TEST_MEM_TYPE          1
`define MEMSHIRE_SM_TEST_IN_WIDTH          1
`define MEMSHIRE_SM_TEST_OUT_WIDTH         1

`define MEMSHIRE_SM_LVL_SHIFTER            0

///////////////////////////////////////////////////
// MEMSHIRE BUS MONITOR
///////////////////////////////////////////////////

// General parameters
`define MEMSHIRE_BM_UPSTREAM                3
`define MEMSHIRE_BM_DOWNSTREAM              7
`define MEMSHIRE_BM_TIMER_WIDTH            32
`define MEMSHIRE_BM_SYNC_STAGES             0
`define MEMSHIRE_BM_US_DEPTH                0
`define MEMSHIRE_BM_DS_DEPTH                8
`define MEMSHIRE_BM_GPIO_WIDTH              1
`define MEMSHIRE_BM_GPIO_EN                 1
`define MEMSHIRE_BM_GPIO_RST_VALUE          0
`define MEMSHIRE_BM_PIPELINE                2
`define MEMSHIRE_BM_LVL_SHIFTER             0

// Capabilities
`define MEMSHIRE_BM_TRACKER_TYPE            0
`define MEMSHIRE_BM_CONC_READ              86
`define MEMSHIRE_BM_CONC_WRITE             54

// Bypass parameters
`define MEMSHIRE_BM_BYPASS                  0
`define MEMSHIRE_BM_AXI_BYPASS              0

// Trace parameters
`define MEMSHIRE_BM_TRACE                   2
`define MEMSHIRE_BM_TRACE_SHARED            0
`define MEMSHIRE_BM_TRACE_PTIME_WIDTH       0
`define MEMSHIRE_BM_TRACE_ADDR_DEPTH        8
`define MEMSHIRE_BM_TRACE_DATA_DEPTH        4
`define MEMSHIRE_BM_TRACE_RESP_DEPTH        8

// Monitor parameters
`define MEMSHIRE_BM_ITAS                    1
`define MEMSHIRE_BM_NUM_COUNT               2
`define MEMSHIRE_BM_NUM_FILTER              4
`define MEMSHIRE_BM_NUM_GRP_FILTERS         1
`define MEMSHIRE_BM_WRITE_DATA_NOTVALID     1

`define MEMSHIRE_BM_ITAS_WIDTH             12
`define MEMSHIRE_BM_COUNTER_WIDTH          32
`define MEMSHIRE_BM_COUNTER_MINMAX          1
`define MEMSHIRE_BM_GRP_THRHLD_WIDTH        8

// Filter configuration parameters
`define MEMSHIRE_BM_FILTER_ADDR          4'hF
`define MEMSHIRE_BM_FILTER_BEAT          3'h0
`define MEMSHIRE_BM_FILTER_BURST         4'h0
`define MEMSHIRE_BM_FILTER_CACHE         4'h0
`define MEMSHIRE_BM_FILTER_DATA          3'h5
`define MEMSHIRE_BM_FILTER_ID            4'hF
`define MEMSHIRE_BM_FILTER_LEN           4'h0
`define MEMSHIRE_BM_FILTER_LOCK          4'h0
`define MEMSHIRE_BM_FILTER_PROT          4'h0
`define MEMSHIRE_BM_FILTER_QOS           4'hF
`define MEMSHIRE_BM_FILTER_REGION        4'h0
`define MEMSHIRE_BM_FILTER_RESP          4'h7
`define MEMSHIRE_BM_FILTER_SIZE          4'hF
`define MEMSHIRE_BM_FILTER_AWUSER        4'h7
`define MEMSHIRE_BM_FILTER_ARUSER        4'h0
`define MEMSHIRE_BM_FILTER_WUSER         3'h0
`define MEMSHIRE_BM_FILTER_RUSER         3'h0
`define MEMSHIRE_BM_FILTER_BUSER         3'h0

// AXI parameters
`define MEMSHIRE_BM_AXI_ENABLE              1
`define MEMSHIRE_BM_AXI_ADDR_WIDTH         `SC_MESH_MASTER_AXI_ADDR_SIZE
`define MEMSHIRE_BM_AXI_DATA_WIDTH         `SC_MESH_MASTER_AXI_DATA_SIZE
`define MEMSHIRE_BM_AXI_ID_WIDTH           `SC_MESH_SLAVE_AXI_ID_SIZE

`define MEMSHIRE_BM_AXI_AWUSER_WIDTH        `SC_MESH_MASTER_AXI_AWUSER_SIZE
`define MEMSHIRE_BM_AXI_WUSER_WIDTH         1
`define MEMSHIRE_BM_AXI_ARUSER_WIDTH        `SC_MESH_MASTER_AXI_ARUSER_SIZE
`define MEMSHIRE_BM_AXI_RUSER_WIDTH         1
`define MEMSHIRE_BM_AXI_BUSER_WIDTH         1

// Counters support
`define MEMSHIRE_BM_COUNT_DURATION          1
`define MEMSHIRE_BM_COUNT_DLATENCY          1
`define MEMSHIRE_BM_COUNT_RLATENCY          1
`define MEMSHIRE_BM_COUNT_TLATENCY          1
`define MEMSHIRE_BM_COUNT_AHESITANCY        1
`define MEMSHIRE_BM_COUNT_DHESITANCY        1
`define MEMSHIRE_BM_COUNT_RHESITANCY        1
`define MEMSHIRE_BM_COUNT_THESITANCY        1
`define MEMSHIRE_BM_COUNT_BEATS             1
`define MEMSHIRE_BM_COUNT_BYTES             1

// TEST
`define MEMSHIRE_BM_TEST_MEM_TYPE           1
`define MEMSHIRE_BM_TEST_IN_WIDTH           1
`define MEMSHIRE_BM_TEST_OUT_WIDTH          1

///////////////////////////////////////////////////
// MEMSHIRE SMB
///////////////////////////////////////////////////
// General parameters
`define MEMSHIRE_SMB_INDEX                  1
`define MEMSHIRE_SMB_DOUBLE_ENDED           1
`define MEMSHIRE_SMB_WR_FIFO_DEPTH          2
`define MEMSHIRE_SMB_RD_FIFO_DEPTH          1
`define MEMSHIRE_SMB_INT_POLARITY           1   // unused in this configuration
`define MEMSHIRE_SMB_NUM_BUFFERS            1
`define MEMSHIRE_SMB_ID_MODE                2   // writes have unique ID, reads have single ID
`define MEMSHIRE_SMB_DOWNSTREAM             7
`define MEMSHIRE_SMB_UPSTREAM               7
`define MEMSHIRE_SMB_BYPASS                 0
`define MEMSHIRE_SMB_CONC_READ              1   // only 1 read at a time
`define MEMSHIRE_SMB_CONC_WRITE             4

`define MEMSHIRE_SMB_SYNC_STAGES            0   // synchronous


`define MEMSHIRE_SMB_AXIM4                  1
`define MEMSHIRE_SMB_AXI_ID_WIDTH           `MEMSHIRE_AXI_SLAVE_CONVERTED_ID_SIZE
`define MEMSHIRE_SMB_AXI_ADDR_WIDTH         `SC_MESH_MASTER_AXI_ADDR_SIZE
`define MEMSHIRE_SMB_AXI_DATA_WIDTH         `SC_MESH_MASTER_AXI_DATA_SIZE

`define MEMSHIRE_SMB_TEST_MEM_TYPE          1  // default value, don't care
`define MEMSHIRE_SMB_TEST_IN_WIDTH          1
`define MEMSHIRE_SMB_TEST_OUT_WIDTH         1

///////////////////////////////////////////////////
// MEMSHIRE ME
///////////////////////////////////////////////////
// General parameters
`define MEMSHIRE_ME_PIPELINE                 1
`define MEMSHIRE_ME_CM_SUPPORT               2  // Its value is two because the ME has an SMB and the Dbg NoC as lower paths
`define MEMSHIRE_ME_GPIO_IN_WIDTH            1
`define MEMSHIRE_ME_GPIO_OUT_WIDTH           24 // 3*8
`define MEMSHIRE_ME_GPIO_EN                  1
`define MEMSHIRE_ME_GPIO_RST_VALUE           '0

`define MEMSHIRE_ME_NORMAL_BRIDGE_IDX        10'h000
`define MEMSHIRE_ME_SMB_IDX                  10'h001
`define MEMSHIRE_ME_SMB_BRIDGE_IDX           10'h000
`define MEMSHIRE_ME_BPAM_IDX                 10'h001
`define MEMSHIRE_ME_BM_IDX                   10'h001
`define MEMSHIRE_ME_SM_IDX                   10'h001
`define MEMSHIRE_ME_MSG_RSVD                 10'h003

///////////////////////////////////////////////////
// MEMSHIRE LEAF NSIP BRIDGE
///////////////////////////////////////////////////
// General parameters
`define MEMSHIRE_SMB_BRIDGE_DOWNSTREAM           7
`define MEMSHIRE_SMB_BRIDGE_UPSTREAM             3
`define MEMSHIRE_SMB_BRIDGE_BYPASS               0

`define MEMSHIRE_SMB_BRIDGE_ROOT_BRIDGE          0
`define MEMSHIRE_SMB_BRIDGE_EVENT_TIERS          32'h00020303
`define MEMSHIRE_SMB_BRIDGE_GROUPS               1  // Unused by non-root bridges by non-root bridges by non-root bridges by non-root bridges
`define MEMSHIRE_SMB_BRIDGE_GROUP_INDEXES        0  // Unused by non-root bridges by non-root bridges by non-root bridges by non-root bridges
`define MEMSHIRE_SMB_BRIDGE_GROUP_MASKS          0  // Unused by non-root bridges by non-root bridges by non-root bridges by non-root bridges
`define MEMSHIRE_SMB_BRIDGE_SYNC_STAGES          0
`define MEMSHIRE_SMB_BRIDGE_SYNC_HOSTID          0  // RTLMIN-3375  no functional path in that direction through the SMB bridge
`define MEMSHIRE_SMB_BRIDGE_DATA_WIDTH           `DEBUG_NSIP_MSG_DATA_WIDTH
`define MEMSHIRE_SMB_BRIDGE_HP_WIDTH             `DEBUG_NSIP_HOST_PORT_WIDTH
`define MEMSHIRE_SMB_BRIDGE_FLOW_DEPTH           9
`define MEMSHIRE_SMB_BRIDGE_FLOW_DEPTH_EVENT     9
`define MEMSHIRE_SMB_BRIDGE_NOC_FLOW_DEPTH       9  // Look in noc/reference_manual/noc_reference_manual.htlm Chapter 2.5 Bridges, Interface Properties, column "fifo_depth_host"
`define MEMSHIRE_SMB_BRIDGE_NOC_FLOW_DEPTH_EVENT 9  // Look in noc/reference_manual/noc_reference_manual.htlm Chapter 2.5 Bridges, Interface Properties, column "fifo_depth_host"

///////////////////////////////////////////////////
// MEMSHIRE ROOT NSIP BRIDGE
///////////////////////////////////////////////////
// General parameters
`define MEMSHIRE_NORMAL_BRIDGE_DOWNSTREAM           7
`define MEMSHIRE_NORMAL_BRIDGE_UPSTREAM             6
`define MEMSHIRE_NORMAL_BRIDGE_BYPASS               0
`define MEMSHIRE_NORMAL_BRIDGE_ROOT_BRIDGE          0
`define MEMSHIRE_NORMAL_BRIDGE_EVENT_TIERS          32'h00020303
`define MEMSHIRE_NORMAL_BRIDGE_GROUPS               1
`define MEMSHIRE_NORMAL_BRIDGE_GROUP_INDEXES        0
`define MEMSHIRE_NORMAL_BRIDGE_GROUP_MASKS          0
`define MEMSHIRE_NORMAL_BRIDGE_SYNC_STAGES          0
`define MEMSHIRE_NORMAL_BRIDGE_SYNC_HOSTID          0  // RTLMIN-3373 not used by any shire
`define MEMSHIRE_NORMAL_BRIDGE_DATA_WIDTH           `DEBUG_NSIP_MSG_DATA_WIDTH
`define MEMSHIRE_NORMAL_BRIDGE_HP_WIDTH             `DEBUG_NSIP_HOST_PORT_WIDTH
`define MEMSHIRE_NORMAL_BRIDGE_FLOW_DEPTH           9
`define MEMSHIRE_NORMAL_BRIDGE_FLOW_DEPTH_EVENT     9
`define MEMSHIRE_NORMAL_BRIDGE_NOC_FLOW_DEPTH       9  // Look in noc/reference_manual/noc_reference_manual.htlm Chapter 2.5 Bridges, Interface Properties, column "fifo_depth_host"
`define MEMSHIRE_NORMAL_BRIDGE_NOC_FLOW_DEPTH_EVENT 9  // Look in noc/reference_manual/noc_reference_manual.htlm Chapter 2.5 Bridges, Interface Properties, column "fifo_depth_host"

`define MEMSHIRE_NORMAL_BRIDGE_INDEX_ADJUST      10'd6



///////////////////////////////////////////////////
// SPIO DEBUG
///////////////////////////////////////////////////
`define SPIO_DBG_DFT_TESTMODE_WIDTH          1
`define SPIO_DBG_DFT_MBIST_IN_WIDTH          6
`define SPIO_DBG_DFT_MBIST_OUT_WIDTH         6
`define SPIO_DBG_DFT_SCAN_SHIFT_WIDTH        2

`define SPIO_BPAM_NOC_MUX_INTERFACES         6 // 5 PLLs + CRU
`define SPIO_APB_MUX_DATA_WIDTH             32 // SPIO NoC has 32b for addr
`define SPIO_APB_MUX_ADDR_WIDTH             32 // SPIO NoC has 32b for addr
`define SPIO_CRU_ADDR_WIDTH                 12 // CRU endpoint is 12b on addr

`define SPIO_DBG_ME_DOWNSTREAM_WIDTH          7
`define SPIO_DBG_ME_UPSTREAM_WIDTH            6
`define SPIO_DBG_ME_PIPELINE                  1
`define SPIO_DBG_ME_TIME                      1
`define SPIO_DBG_ME_BYPASS                    0
`define SPIO_DBG_ME_INGRESS_EVENT_DEPTH       6
`define SPIO_DBG_ME_MSG_DEPTH                 0
`define SPIO_DBG_ME_ME_EVENT_MASK_WIDTH       4

`define SPIO_DBG_JPAM_UPSTREAM_WIDTH          3
`define SPIO_DBG_JPAM_DOWNSTREAM_WIDTH        5
`define SPIO_DBG_JPAM_BYPASS                  0
`define SPIO_DBG_JPAM_DATA_LENGTH           128
`define SPIO_DBG_JPAM_EVENT_CROSS_RETIME      0
`define SPIO_DBG_JPAM_EVENT_IN                0
`define SPIO_DBG_JPAM_EVENT_OUT               0
`define SPIO_DBG_JPAM_GPIO                    0
`define SPIO_DBG_JPAM_DS_EVENT_FIFO_DEPTH     1
`define SPIO_DBG_JPAM_FALLING_EDGE            1
`define SPIO_DBG_JPAM_SYNC_STAGES             0

`define SPIO_DBG_TE_DOWNSTREAM_WIDTH          6
`define SPIO_DBG_TE_UPSTREAM_WIDTH            6
`define SPIO_DBG_TE_GPIO_WIDTH                1

`define SPIO_DBG_APB_SUBGROUPS_WIDTH          5
`define SPIO_DBG_BPAM_APB_ADDR_WIDTH          21

`define SPIO_DBG_BPAM_APB_DATA_WIDTH          `ESR_APB_D_WIDTH
`define SPIO_DBG_BPAM_DOWNSTREAM_WIDTH        6
`define SPIO_DBG_BPAM_UPSTREAM_WIDTH          6
`define SPIO_DBG_BPAM_GPIO_WIDTH              4
`define SPIO_DBG_BPAM_EVENT_WIDTH             3

`define SPIO_DBG_SM_DOWNSTREAM_WIDTH          7
`define SPIO_DBG_SM_UPSTREAM_WIDTH            3
`define SPIO_DBG_SM_GPIO_WIDTH                (`STDALMIN_SM_MUX_WIDTH + 1)
`define SPIO_DBG_SM_FILTERS                   8
`define SPIO_DBG_SM_TRACE_DEPTH              16
`define SPIO_DBG_SM_FILTER_NOC_WIDTH         90
`define SPIO_DBG_SM_MATCH_NOC_WIDTH           0
`define SPIO_DBG_SM_DATA_NOC_WIDTH           90

`define SPIO_DBG_SM_FILTER_WIDTH             `MAX(`SPIO_DBG_SM_FILTER_NOC_WIDTH, \
                                                  `NEIGH_DEBUG_FILTER_WIDTH )

`define SPIO_DBG_SM_MATCH_WIDTH              `MAX(`SPIO_DBG_SM_MATCH_NOC_WIDTH, \
                                                  `NEIGH_DEBUG_MATCH_WIDTH )

`define SPIO_DBG_SM_DATA_WIDTH               `MAX(`SPIO_DBG_SM_DATA_NOC_WIDTH, \
                                                  `NEIGH_DEBUG_DATA_WIDTH )

`define SPIO_DBG_DMA_UPSTREAM_WIDTH           6
`define SPIO_DBG_DMA_DOWNSTREAM_WIDTH         6
`define SPIO_DBG_DMA_GPIO_EN                  1
`define SPIO_DBG_DMA_GPIO_WIDTH              16 //8LSB for QOS, 8MSB for CACHE
`define SPIO_DBG_DMA_AXI4_ENABLE              1
`define SPIO_DBG_DMA_SYNC_STAGES              0
`define SPIO_DBG_DMA_AXIM_ADDR_WIDTH         40
`define SPIO_DBG_DMA_AXIM_DATA_WIDTH         64
`define SPIO_DBG_DMA_AXIM_ID_WIDTH            4
`define SPIO_DBG_DMA_BYPASS                   0
`define SPIO_DBG_DMA_FIFO_DEPTH_READ_BURST    1
`define SPIO_DBG_DMA_FIFO_DEPTH_WRITE_BURST   1

`define SPIO_DBG_AXIC_DOWNSTREAM_WIDTH        7
`define SPIO_DBG_AXIC_UPSTREAM_WIDTH          6
`define SPIO_DBG_AXIC_AXI4_ENABLE             1
`define SPIO_DBG_AXIC_NUM_EVT_FILTER          2
`define SPIO_DBG_AXIC_SYNC_STAGES             0

///////////////////////////////////////////////////
// PU DEBUG
///////////////////////////////////////////////////
`define PU_DBG_BRIDGE_INDEX_ADJUST       `US_INDEX_LENGTH'h3EF
`define PU_DBG_NOC_BRIDGE_DOWNSTREAM      7
`define PU_DBG_NOC_BRIDGE_UPSTREAM        7

`define PU_DBG_ME_GPIO_IN_WIDTH           1
`define PU_DBG_ME_GPIO_OUT_WIDTH         24 // (3*8)
`define PU_DBG_ME_GPIO                    1
`define PU_DBG_ME_PIPELINE                1
`define PU_DBG_ME_TIME                    1
`define PU_DBG_ME_BYPASS                  0
`define PU_DBG_ME_INGRESS_EVENT_DEPTH     6
`define PU_DBG_ME_INGRESS_MSG_DEPTH       0
`define PU_DBG_ME_CM_SUPPORT              1
`define PU_DBG_ME_FLOW_ROUTE              3

`define PU_DBG_USB_UPSTREAM_WIDTH         3
`define PU_DBG_USB_DOWNSTREAM_WIDTH       5

`define PU_DBG_JTAG_UPSTREAM_WIDTH        3
`define PU_DBG_JTAG_DOWNSTREAM_WIDTH      5
`define PU_DBG_JTAG_PIPELINE              1
`define PU_DBG_JTAG_IDCODE                32'h01001ACB
`define PU_DBG_DMA_UPSTREAM_WIDTH         5
`define PU_DBG_DMA_DOWNSTREAM_WIDTH       7
`define PU_DBG_DMA_AXI4_ENABLE            1
`define PU_DBG_DMA_AXIM2UDB_SYNC_STAGES   0
`define PU_DBG_DMA_AXIM_ADDR_WIDTH       40
`define PU_DBG_DMA_AXIM_DATA_WIDTH       64
`define PU_DBG_DMA_AXIM_ID_WIDTH          5
`define PU_DBG_DMA_BYPASS                 0
`define PU_DBG_DMA_GPIO_EN                1
`define PU_DBG_DMA_GPIO_OUT_WIDTH         8
`define PU_DBG_DMA_UDB2AXIM_SYNC_STAGES   0

`define PU_DBG_SI_UPSTREAM_WIDTH          3
`define PU_DBG_SI_DOWNSTREAM_WIDTH        7
`define PU_DBG_SI_PIPELINE                1
`define PU_DBG_SI_BYPASS                  0
`define PU_DBG_SI_UDB2AXI_SYNC_STAGES     0
`define PU_DBG_SI_AXI2UDB_SYNC_STAGES     0
`define PU_DBG_SI_AXI4_ENABLE             1
`define PU_DBG_SI_AXI_ID_WIDTH            4

///////////////////////////////////////////////////
// MAX DEBUG
///////////////////////////////////////////////////
`define MAX_DBG_DFT_TESTMODE_WIDTH          1
`define MAX_DBG_DFT_SCAN_SHIFT_WIDTH        2

`define MAX_DBG_MSGLOCK_US_LOCK_AT_RESET        `ME_MSG_LOCK_US_RESET_P
`define MAX_DBG_MSGLOCK_DS_LOCK_AT_RESET        `ME_MSG_LOCK_DS_RESET_P
`define MAX_DBG_MSGLOCK_LOCK_IP_POLARITY        `ME_MSG_LOCK_POLARITY

`define MAX_DBG_ME_DOWNSTREAM_WIDTH             7
`define MAX_DBG_ME_UPSTREAM_WIDTH               6
`define MAX_DBG_ME_PIPELINE                     1
`define MAX_DBG_ME_TIME                         1
`define MAX_DBG_ME_BYPASS                       0
`define MAX_DBG_ME_INGRESS_EVENT_DEPTH          6
`define MAX_DBG_ME_INGRESS_MSG_DEPTH            0
`define MAX_DBG_ME_LOWER_0_EVENT_MASK_WIDTH     4
`define MAX_DBG_ME_CDC_DEPTH                    0

`define MAX_DBG_SC_SM_DS_WIDTH                  7
`define MAX_DBG_SC_SM_US_WIDTH                  3
`define MAX_DBG_SC_SM_TRACE_DEPTH              32

`define MAX_DBG_MAX_SM_DS_WIDTH                 7
`define MAX_DBG_MAX_SM_US_WIDTH                 3
`define MAX_DBG_MAX_SM_EVENT                    0
`define MAX_DBG_MAX_SM_STS_BYPASS               0
`define MAX_DBG_MAX_SM_MATCH_WIDTH             64
`define MAX_DBG_MAX_SM_FILTER_WIDTH           128
`define MAX_DBG_MAX_SM_DATA_WIDTH             200
`define MAX_DBG_MAX_SM_GPIO_WIDTH               ((1+`NUM_MAXION_CORES_LOG2)+`MAXNEIGH_DBG_SM_CTRL_WIDTH)
`define MAX_DBG_MAX_SM_COMPARATORS              8
`define MAX_DBG_MAX_SM_QUALIFIERS               4
`define MAX_DBG_MAX_SM_FILTERS                  4
`define MAX_DBG_MAX_SM_COUNTERS                 3
`define MAX_DBG_MAX_SM_COUNTER_WIDTH           32
`define MAX_DBG_MAX_SM_ACCUMULATORS             0
`define MAX_DBG_MAX_SM_ACCUM_MINMAX             0
`define MAX_DBG_MAX_SM_TRACE                    2
`define MAX_DBG_MAX_SM_TRACE_DEPTH            256
`define MAX_DBG_MAX_SM_PIPELINE                 1

`define MAX_DBG_BPAM_DS_WIDTH                   6
`define MAX_DBG_BPAM_US_WIDTH                   6
`define MAX_DBG_BPAM_APB4                       1
`define MAXSHIRE_BPAM_APB_ADDR_WIDTH           29
`define MAXSHIRE_BPAM_APB_DATA_WIDTH           64
`define MAX_DBG_BPAM_BYPASS                     0
`define MAX_DBG_BPAM_EVENT_IN                   0
`define MAX_DBG_BPAM_EVENT_OUT_WIDTH            1
`define MAX_DBG_BPAM_EVENT_IN_WIDTH             1
`define MAX_DBG_BPAM_GPIO                       1
`define MAX_DBG_BPAM_GPIO_IN_WIDTH              1
`define MAX_DBG_BPAM_GPIO_OUT_WIDTH             1

`define MAX_DBG_BM_DS_WIDTH                     7
`define MAX_DBG_BM_US_WIDTH                     3
`define MAX_DBG_BM_PIPELINE_STAGES              2

`define MAX_DBG_JPAM_DS_WIDTH                   5
`define MAX_DBG_JPAM_US_WIDTH                   3
`define MAX_DBG_JPAM_BYPASS                     0
`define MAX_DBG_JPAM_DATA_LENGTH              128
`define MAX_DBG_JPAM_EVENT_CROSS_RETIME         0
`define MAX_DBG_JPAM_EVENT_IN                   0
`define MAX_DBG_JPAM_EVENT_OUT                  0
`define MAX_DBG_JPAM_GPIO                       0
`define MAX_DBG_JPAM_DS_EVENT_FIFO_DEPTH        1
`define MAX_DBG_JPAM_FALLING_EDGE               1

/////////////////////////////////////////////////
// START OF TEMPORAL DEFINES
// FOR MAXION, MEMSHIRE AND IOSHIRE DEBUG
// REMOVE WHEN DEBUG MODULES ARE CONFIGURED PROPERLY
///////////////////////////////////////////////

`define us_index_length     `US_INDEX_LENGTH

//Trace Encoder
`define te_gpio_out_width_p `SHIRE_TE_GPIO_WIDTH

`define te_retires_per_cycle             `shire_te_retires_per_cycle
`define te_comparators_number            `shire_te_comparators_number
`define te_filter_number                 `shire_te_filter_number
`define te_counter_number                `shire_te_counter_number
`define te_exceptions_number             `shire_te_exceptions_number

`define te_counter_width                 `shire_te_counter_width
`define te_context_width                 `shire_te_context_width
`define te_ecause_width                  `shire_te_ecause_width
`define te_privilege_width               `shire_te_privilege_width
`define te_status_width                  `shire_te_status_width
`define te_user_width                    `shire_te_user_width
`define te_intr_addr_bus_width           `shire_te_instr_addr_width
`define te_instruction_bus_width         `shire_te_instr_width
`define te_val_width                     `shire_te_val_width

`define te_filter_context                `shire_te_filter_context
`define te_filter_ecause                 `shire_te_filter_ecause
`define te_filter_interrupt              `shire_te_filter_interrupt
`define te_filter_privilege              `shire_te_filter_privilege
`define te_filter_trap_val               `shire_te_filter_trap_val
`define te_fetch_depth                   `shire_te_fetch_depth

//BPAM
`define bpam_gpio_out_width_p `shire_bpam_gpio_out_width
`define bpam_gpio_in_width_p `shire_bpam_gpio_in_width

`define bpam_shire_apb_addr_width `SHIRE_BPAM_APB_ADDR_WIDTH
`define bpam_shire_apb_data_width `shire_bpam_apb_data_width

`define bpam_shire_from_rtl_event_width     3
`define bpam_shire_to_rtl_event_width       3

//Shire cache status monitor
`define sm2_gpio_out_width_p 32

// Debug message infrastructure width
`define analytic_lower_us_width     3
`define analytic_lower_ds_width     5
`define analytic_disable_us_width   0
`define analytic_disable_ds_width   0

`define ME_MSG_LOCK_US_RESET_P 0
`define ME_MSG_LOCK_DS_RESET_P 0
`define ME_MSG_LOCK_POLARITY   0 // 1 - means access denied when lock signal is 1
                                 // 0 - means access denied when lock signal is 0

//BUS MONITOR
`define shire_bm_lower_us_width 3
`define shire_bm_lower_ds_width 7

`define shire_bm_axi_id_width           `SC_MESH_SLAVE_AXI_ID_SIZE
`define bm_gpio_enable 1
`define bm_gpio_out_width_p 3

`define bm_index_len              10
`define bm_itas                    1
`define bm_itas_width              9
`define bm_shire_counter_width    32
`define bm_shire_couter_minmax     1
`define bm_shire_num_count         2
`define bm_shire_num_filter        4
`define bm_shire_conc_read        64
`define bm_shire_conc_write       64
`define bm_num_grp_filters         1
`define bm_grp_thrhld_width        8
`define bm_trace                   1
`define bm_trace_addr_depth        8
`define bm_trace_data_depth        8
`define bm_trace_resp_depth        8
`define bm_write_data_notvalid     0

// Bus monitor filter parameters
`define bm_filter_addr          4'hF
`define bm_filter_beat          3'h7
`define bm_filter_burst         4'hD
`define bm_filter_cache         4'h5
`define bm_filter_data          3'h5 // This still not defined properly (5 or 0)
`define bm_filter_id            4'hF
`define bm_filter_len           4'h5
`define bm_filter_lock          4'h5
`define bm_filter_prot          4'h5
`define bm_filter_qos           4'hF
`define bm_filter_region        4'h0
`define bm_filter_resp          3'h7
`define bm_filter_size          4'hF
`define bm_filter_awuser        4'hF
`define bm_filter_aruser        4'h0
`define bm_filter_wuser         3'h0
`define bm_filter_ruser         3'h0
`define bm_filter_buser         3'h0

// Shire Bus monitor AXI widths
`define bm_shire_axi_addr_width 40
`define bm_shire_axi_data_width 512
`define bm_shire_axi_enable    1
`define bm_shire_axi_id_width   16
`define bm_shire_axi_id_range  (`bm_shire_axi_id_width-1):0
`define bm_shire_axi_strb_width (`bm_shire_axi_data_width/8)

`define bm_shire_axi_awregion 4
`define bm_shire_axi_arregion 4
`define bm_shire_axi_wuser_width 1

`define sm0_gpio_enable 1
`define sm1_gpio_enable 1
`define sm2_gpio_enable 1
`define te_gpio_enable 1
`define bpam_gpio_enable 0

`define sm0_gpio_out_width_p 32
`define sm1_gpio_out_width_p 32

`define sm0_neigh_instance_id 4'b0111

//Neighborhood Status Monitor 2
`define sm2_qualifiers_number 10
`define sm2_comparators_number 12
`define sm2_filter_number 4
`define sm2_match_width 55
`define sm2_filter_width 55
`define sm2_data_trace_width 165
`define sm2_counter_number 3
`define sm2_counter_width 32
`define sm2_trace_enable 1

//Neighborhood Status Monitor 0
`define sm0_qualifiers_number 10
`define sm0_comparators_number 12
`define sm0_filter_number 4
`define sm0_match_width 256
`define sm0_filter_width 200
`define sm0_data_trace_width 200
`define sm0_counter_number 3
`define sm0_counter_width 32
`define sm0_trace_enable 1

//Neighborhood Status Monitor 1
`define sm1_qualifiers_number 10
`define sm1_comparators_number 12
`define sm1_filter_number 4
`define sm1_match_width 1
`define sm1_filter_width 178
`define sm1_data_trace_width 178
`define sm1_counter_number 3
`define sm1_counter_width 32
`define sm1_trace_enable 1


/////////////////////////////////////////////////
// END OF TEMPORAL DEFINES
///////////////////////////////////////////////
`endif //_DEBUG_DEFINES_

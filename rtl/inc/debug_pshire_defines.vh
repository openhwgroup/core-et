// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`ifndef _DEBUG_PSHIRE_DEFINES_
`define _DEBUG_PSHIRE_DEFINES_

//////////////////////////////////////////////////////////////////////////
// Pshire uses macro defines in the debug_defines.vh whenever applicable.
// The macros defined in this file are unique for pshire or they have higher
// probability to diverge from minion shire counterparts.
//////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////
// ME
///////////////////////////////////////////////////

//// `define pshire_me_gpio_width         1
`define pshire_me_gpio_in_width         1
`define pshire_me_gpio_out_width        `MEMSHIRE_ME_GPIO_OUT_WIDTH
`define pshire_me_gpio_en            1

`define me_pshire_lower0_us_width    3
`define me_pshire_lower0_ds_width    7

`define pshire_me_cm_support_p            0 // Communicator routing support. 0: no cm routing, 1: cm2cm, 2: cm2cm and SMB
`define pshire_me_flow0_route_p           0 // Reset value for flow0_route
`define pshire_me_flow1_route_p           0 // Reset value for flow1_route
`define pshire_me_flow2_route_p           0 // Reset value for flow2_route

`define pshire_me_hysteresis_width_p      0 // Width of hysteresis byte counter
`define pshire_me_info_width_p            8 // Chip indentification input width
`define pshire_me_time_p                  0 // Time message generation supported when set

`define pshire_me_bypass_p                0 // Retime message interface when 0
`define pshire_me_ingress_event_depth_p   6 // Event FIFO depth
`define pshire_me_ingress_msg_depth_p     0 // Ingress message FIFO depth
`define pshire_me_egress_msg_depth_p      0 // Message FIFO depth
`define pshire_me_lo_event_mask_width_p   4 // lower_0 Event mask width
`define pshire_me_event_mask_width_p      0 // Event mask width


///////////////////////////////////////////////////
// Bridge
///////////////////////////////////////////////////

`define pshire_noc_bridge_us_width       3
`define pshire_noc_bridge_ds_width       7
`define pshire_noc_bridge_bypass         0 //message inputs registered when 0
`define pshire_noc_bridge_bypass_leaf    1
`define pshire_noc_bridge_bypass_root    0


`define pshire_noc_bridge_index_adjust `us_index_length'h06 

`define pshire_noc_bridge_leaf          0 
`define pshire_noc_bridge_root          1    // not used; will retire after ioshire stops sharing this parameter;

`define pshire_noc_bridge_event_tiers `MEMSHIRE_NORMAL_BRIDGE_EVENT_TIERS

`define pshire_noc_bridge_groups         1 
`define pshire_noc_bridge_groups_leaf    1 
`define pshire_noc_bridge_groups_root    2 
`define pshire_noc_bridge_grp_index      0 
`define pshire_noc_bridge_grp_index_leaf 0 
`define pshire_noc_bridge_grp_index_root {16'd544,16'd0}
`define pshire_noc_bridge_grp_mask       0
`define pshire_noc_bridge_grp_mask_leaf  0
`define pshire_noc_bridge_grp_mask_root  {16'hFFF8,16'hFFF0}

// Synchronization
`define pshire_noc_bridge_sync_hostid    0 
`define pshire_noc_bridge_udb2noc_sync   0 // Between the debug NoC and the bridge we have voltage(+clock) crossing FIFOS,
`define pshire_noc_bridge_noc2udb_sync   0 // so both interfaces have the same clock

//NSIP interface
`define pshire_noc_bridge_msg_data_width   128 
`define pshire_noc_bridge_evt_data_width   32 
`define pshire_noc_bridge_hp_width       8  
`define pshire_noc_bridge_flow_depth     9
`define pshire_noc_bridge_flow_event     9


///////////////////////////////////////////////////
// Status Monitor
///////////////////////////////////////////////////

`define pshire_sm_pipeline             1 //Allowed values for SM 0,1
`define pshire_sm_gpio_en              1
`define pshire_sm_gpio_width           1 
`define pshire_sm_lvl_shifter          0 //Signals between hi/lo voltage
`define pshire_sm_upstream             3
`define pshire_sm_downstream           7
`define pshire_sm_sync_stages          2
`define pshire_sm_trace_en             2 //Trace functionality (0: no trace, 1: basic trace, 2: trace with XOR compression
`define pshire_sm_sequencer_en         1 
`define pshire_sm_event_en             0
`define pshire_sm_bypass               0 //message inputs registered when 0
`define pshire_sm_sts_bypass           0 //signals inputs registered when 0
`define pshire_sm_qual_number         10
`define pshire_sm_comp_number          8
`define pshire_sm_filter_number        4
`define pshire_sm_counter_number       3
`define pshire_sm_accum_number         0
`define pshire_sm_accum_minmax         0
`define pshire_sm_match_width        180
`define pshire_sm_data_trace_width   180
`define pshire_sm_filter_width       180
`define pshire_sm_counter_width       32
`define pshire_sm_accum_width         16
`define pshire_sm_event_width          1 
`define pshire_sm_timer_width         32
`define pshire_sm_match_thresh        16
`define pshire_sm_filter_thresh       16
`define pshire_sm_counter_thresh       8
`define pshire_sm_sequencer_thresh     8
`define pshire_sm_trace_depth          16
`define pshire_sm_ups_depth            0
`define pshire_sm_ds_depth             8

///////////////////////////////////////////////////
// DMA AM
///////////////////////////////////////////////////

`define pshire_dma_am_upstream               3
`define pshire_dma_am_downstream             5
 
`define pshire_dma_am_axi4_enable            1

`define pshire_dma_am_sync_stages            0   // DMA AM and AXI bus are synchronous
`define pshire_dma_am_axi_addr_width         40
`define pshire_dma_am_axi_data_width         64
`define pshire_dma_am_axi_id_width           4
`define pshire_dma_am_bypass                 0   //message inputs registered when 0
`define pshire_dma_am_fifo_depth_read_burst  16
`define pshire_dma_am_fifo_depth_write_burst 16
`define pshire_dma_am_gpio_en                1
`define pshire_dma_am_gpio_width             8
`define pshire_dma_am_num_dma_chan           1
`define pshire_dma_am_r_concurrent           1   
`define pshire_dma_am_id_mode                0
`define pshire_dma_timer_width               32
`define pshire_dma_am_w_concurrent           1   

`define pshire_dma_am_test_mem_type          1
`define pshire_dma_am_test_in_width          1
`define pshire_dma_am_test_out_width         1

///////////////////////////////////////////////////
// Bus Monitor
///////////////////////////////////////////////////

`define pshire_bm_bypass                 0   //message inputs registered when 0
`define pshire_bm_upstream               3
`define pshire_bm_downstream             7
`define pshire_bm_gpio_en                1
`define pshire_bm_gpio_width             3
`define pshire_bm_gpio_reset             '0

`define pshire_bm_conc_read              32
`define pshire_bm_conc_write             32

`define pshire_bm_trace                  2

// AXI parameters
`define pshire_bm_axi4_enable            1
`define pshire_bm_axi_addr_width         40
`define pshire_bm_axi_data_width         256
`define pshire_bm_axi_id_width           16

//// `define pshire_bm_axi_id_range           (`pshire_bm_axi_id_width-1):0
//// `define pshire_bm_axi_strb_width         (`pshire_bm_axi_data_width/8)

//// `define pshire_bm_axi_awregion            4
//// `define pshire_bm_axi_arregion            4

`define pshire_bm_axi_awuser_width        1
`define pshire_bm_axi_wuser_width         1
`define pshire_bm_axi_aruser_width        1
`define pshire_bm_axi_ruser_width         1
`define pshire_bm_axi_buser_width         1

`define pshire_bm_filter_awuser           4'h0 
`define pshire_bm_filter_aruser           4'h0
`define pshire_bm_filter_wuser            3'h0
`define pshire_bm_filter_ruser            3'h0
`define pshire_bm_filter_buser            3'h0

`define pshire_bm_sync_stages             0     // BM and AXI bus are synchronous

`endif // _DEBUG_PSHIRE_DEFINES_

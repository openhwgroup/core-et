// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`ifndef _DEBUG_IOSHIRE_DEFINES_
`define _DEBUG_IOSHIRE_DEFINES_

//////////////////////////////////////////////////////////////////////////
// IOShire uses macro defines in the debug_defines.vh whenever applicable.
// The macros defined in this file are unique for ioshire or they are
// judged likely to diverge from minion shire counterparts.
//////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////
// ULTRASOC INSTACE IDs
///////////////////////////////////////////////////

// Shire Modules instance IDs
`define ioshire_me_id      6'h00
`define ioshire_sm0_id     6'h01
`define ioshire_sm1_id     6'h02
`define ioshire_dma_am_id  6'h03
`define ioshire_bm0_id     6'h04
`define ioshire_bm1_id     6'h05

///////////////////////////////////////////////////
// ME, spio
///////////////////////////////////////////////////

`define ioshire_spio_me_gpio_width         1
`define ioshire_spio_me_gpio_en            1

`define ioshire_spio_me_lower0_us_width    3
`define ioshire_spio_me_lower0_ds_width    7

///////////////////////////////////////////////////
// JPAM
///////////////////////////////////////////////////

`define ioshire_spio_jpam_ds_width         5
`define ioshire_spio_jpam_us_width         3

///////////////////////////////////////////////////
// Bridge
///////////////////////////////////////////////////

`define ioshire_noc_bridge_us_width       3
`define ioshire_noc_bridge_ds_width       7
`define ioshire_noc_bridge_bypass         0 //message inputs registered when 0


`define ioshire_noc_bridge_index_adjust `us_index_length'h06 

`define ioshire_noc_bridge_root        1 
`define ioshire_noc_bridge_event_tiers `us_index_length 
`define ioshire_noc_bridge_groups      1 
`define ioshire_noc_bridge_grp_index   0 
`define ioshire_noc_bridge_grp_mask    0 

// Synchronization
`define ioshire_noc_bridge_sync_hostid    0 
`define ioshire_noc_bridge_udb2noc_sync   0 // Between the debug NoC and the bridge we have voltage(+clock) crossing FIFOS,
`define ioshire_noc_bridge_noc2udb_sync   0 // so both interfaces have the same clock

//NSIP interface
`define ioshire_noc_bridge_hp_width       8  
`define ioshire_noc_bridge_flow_depth     9
`define ioshire_noc_bridge_flow_event     9


///////////////////////////////////////////////////
// Status Monitor, spio
///////////////////////////////////////////////////

`define ioshire_spio_sm_us_width           3
`define ioshire_spio_sm_ds_width           7

`define ioshire_spio_sm_gpio_out_width    2

`define ioshire_sm_pipeline             1 //Allowed values for SM 0,1
`define ioshire_sm_gpio_en              1
`define ioshire_sm_gpio_width           1 
`define ioshire_sm_lvl_shifter          0 //Signals between hi/lo voltage
`define ioshire_sm_sync_stages          0
`define ioshire_sm_trace_en             2
`define ioshire_sm_event_en             0
`define ioshire_sm_bypass               0 //message inputs registered when 0
`define ioshire_sm_sts_bypass           0 //signals inputs registered when 0
`define ioshire_sm_qual_number         10
`define ioshire_sm_comp_number          8
`define ioshire_sm_filter_number        4
`define ioshire_sm_counter_number       3
`define ioshire_sm_accum_number         0
`define ioshire_sm_accum_minmax         0
`define ioshire_sm_counter_width       32
`define ioshire_sm_accum_width         16
`define ioshire_sm_event_width          1 
`define ioshire_sm_timer_width         32
`define ioshire_sm_match_thresh        16
`define ioshire_sm_filter_thresh       16
`define ioshire_sm_counter_thresh       8
`define ioshire_sm_sequencer_thresh     8
`define ioshire_sm_trace_depth          4
`define ioshire_sm_ups_depth            0
`define ioshire_sm_ds_depth             8

///////////////////////////////////////////////////
// Trace Encoder, spio
///////////////////////////////////////////////////

`define ioshire_spio_te_us_width           3
`define ioshire_spio_te_ds_width           7

`define ioshire_spio_te_gpio_out_width     2

///////////////////////////////////////////////////
// BPAM, spio
///////////////////////////////////////////////////

`define ioshire_spio_bpam_us_width        3
`define ioshire_spio_bpam_ds_width        6
 
///////////////////////////////////////////////////
// DMA AM
///////////////////////////////////////////////////

`define ioshire_spio_dma_us_width        3
`define ioshire_spio_dma_ds_width        5
 
`define ioshire_dma_am_axi4_enable            1

`define ioshire_dma_am_sync_stages            0   // DMA AM and AXI bus are synchronous
`define ioshire_dma_am_axi_addr_width         40
`define ioshire_dma_am_axi_data_width         32
`define ioshire_dma_am_axi_id_width           4
`define ioshire_dma_am_bypass                 0   //message inputs registered when 0
`define ioshire_dma_am_fifo_depth_read_burst  16
`define ioshire_dma_am_fifo_depth_write_burst 16
`define ioshire_dma_am_gpio_en                1
`define ioshire_dma_am_gpio_width             1
`define ioshire_dma_am_num_dma_chan           1
`define ioshire_dma_am_id_mode                0
`define ioshire_dma_timer_width               32

`define ioshire_dma_am_test_mem_type          1
`define ioshire_dma_am_test_in_width          1
`define ioshire_dma_am_test_out_width         1

///////////////////////////////////////////////////
// AXI Communicator, spio
///////////////////////////////////////////////////

`define ioshire_spio_axi_us_width           3
`define ioshire_spio_axi_ds_width           7
 
///////////////////////////////////////////////////
// Static Instrumentation, pu
///////////////////////////////////////////////////

`define ioshire_pu_si_axi_id_width              4
`define ioshire_pu_si_axi4_enable               1

`endif // _DEBUG_IOSHIRE_DEFINES_

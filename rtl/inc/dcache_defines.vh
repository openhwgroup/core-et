// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`ifndef _DCACHE_DEFINES_
`define _DCACHE_DEFINES_

// Mem response type (short name)
`define MT_X        4'bx
`define MT_B        dcache_type_B
`define MT_H        dcache_type_H
`define MT_W        dcache_type_W
`define MT_D        dcache_type_D
`define MT_BU       dcache_type_BU
`define MT_HU       dcache_type_HU
`define MT_WU       dcache_type_WU
`define MT_PS       dcache_type_PS
`define MT_PS_BR    dcache_type_PS_BR
`define MT_PS_GS8   dcache_type_PS_GS8
`define MT_PS_GS16  dcache_type_PS_GS16
`define MT_PS_GS32  dcache_type_PS_GS32

////////////////////////////////////////////////////////////////////////////////
// MACROS
////////////////////////////////////////////////////////////////////////////////

// Cache line size
`define DCACHE_LINE_BYTES     64
`define DCACHE_LINE_ADDR_BITS $clog2(`DCACHE_LINE_BYTES)

// LRU
`define DCACHE_LRU_SET_SIZE  (`DCACHE_WAYS*`DCACHE_WAYS)
`define DCACHE_LRU_SET_RANGE (`DCACHE_LRU_SET_SIZE-1):0

// Tag generic
`define VA_TAG_MSB          `VA_MSB
`define PA_TAG_MSB          `PA_MSB

// Tag with regular bits for full set 
`define PA_TAG_REG_RANGE    `PA_TAG_MSB:($clog2(`DCACHE_SETS)+6)
`define PA_TAG_REG_SIZE     (`PA_SIZE-$clog2(`DCACHE_SETS)-6)

// Tag with extended bits for set reduction
`define VA_TAG_EXT_SIZE     (`VA_SIZE-$clog2(`DCACHE_SETS_RED)-6)
`define PA_TAG_EXT_SIZE     (`PA_SIZE-$clog2(`DCACHE_SETS_RED)-6)
`define PA_TAG_EXT_RANGE    `PA_TAG_MSB:($clog2(`DCACHE_SETS_RED)+6)
`define PA_NOTAG_EXT_RANGE  ($clog2(`DCACHE_SETS_RED)+6-1):0
`define PA_NOTAG_EXT_BITS  ($clog2(`DCACHE_SETS_RED)+6)

// LRAM read ports
`define DCACHE_LRAM_DATA_SIZE  64
`define DCACHE_LRAM_DATA_RANGE (`DCACHE_LRAM_DATA_SIZE-1):0
`define DCACHE_LRAM_NUM_BANKS 4
`define DCACHE_LRAM_BANK_RANGE (`DCACHE_LRAM_NUM_BANKS-1):0

// LRAM rows
`define DCACHE_LRAM_ROWS           (`DCACHE_SETS*`DCACHE_WAYS*2)
`define DCACHE_LRAM_ROW_ADDR_BITS  $clog2(`DCACHE_LRAM_ROWS)
`define DCACHE_LRAM_ROWS_SPLIT     (`DCACHE_SETS_SCP_SPLIT*`DCACHE_WAYS*2)

// Load reserved / store conditional
`define LRSC_CYCLES      32
`define LRSC_COUNT_SIZE  $clog2(`LRSC_CYCLES)
`define LRSC_COUNT_RANGE (`LRSC_COUNT_SIZE-1):0

// UC region
`define DCACHE_UC_REGION 32'hfff0_0000

// Replay queue
`define DCACHE_REPLAYQ_RANGE            (`DCACHE_REPLAYQ_SIZE-1):0
`define DCACHE_REPLAYQ_ADDR_RANGE       ($clog2(`DCACHE_REPLAYQ_SIZE)-1):0
`define DCACHE_REPLAYQ_AGE_ID_MASK_SIZE (`DCACHE_REPLAYQ_SIZE*2)               // We want to have a window of up to 2 times the number of entries in the replayQ
`define DCACHE_REPLAYQ_AGE_ID_MASK      (`DCACHE_REPLAYQ_AGE_ID_MASK_SIZE-1):0 // We want to have a window of up to 2 times the number of entries in the replayQ
`define DCACHE_REPLAYQ_AGE_ID           ($clog2(`DCACHE_REPLAYQ_SIZE)+1):0     // We need an extra bit vs the previous entry to store ERA
`define DCACHE_REPLAYQ_AGE_ID_CMP       $clog2(`DCACHE_REPLAYQ_SIZE):0         // Compare bits
`define DCACHE_REPLAYQ_AGE_ID_ERA       ($clog2(`DCACHE_REPLAYQ_SIZE)+1)       // Era bit
`define DCACHE_REPLAYQ_ADDR_CONF_BITS   $clog2(`DCACHE_DATA_SIZE/8) +: 10      // Check for addr hazards using 5 bits

// Miss handler
`define DCACHE_MH_FILE_SIZE  2
`define DCACHE_MH_FILE_RANGE $clog2(`DCACHE_MH_FILE_SIZE)-1:0

// Buffer array
`define DCACHE_BUFFER_SIZE     8
`define DCACHE_BUFFER_RANGE    `DCACHE_BUFFER_SIZE-1:0
`define DCACHE_BUFFER_ID_RANGE $clog2(`DCACHE_BUFFER_SIZE)-1:0
`define DCACHE_BUFFER_ALMOST_FULL_LEVEL 2

// Reduce
`define DCACHE_REDUCE_LEVEL_BITS     4
`define DCACHE_REDUCE_LEVEL_RANGE    (`DCACHE_REDUCE_LEVEL_BITS-1):0
`define DCACHE_REDUCE_MSG_DATA_BITS  5
`define DCACHE_REDUCE_MSG_DATA_RANGE (`DCACHE_REDUCE_MSG_DATA_BITS-1):0
`define DCACHE_REDUCE_MSG_DATA_MSB   (`DCACHE_REDUCE_MSG_DATA_BITS-1)
`define DCACHE_REDUCE_ACTION_BITS    2
`define DCACHE_REDUCE_ACTION_RANGE   (`DCACHE_REDUCE_ACTION_BITS-1):0

`define DCACHE_REDUCE_DO_NOTHING    2'b10
`define DCACHE_REDUCE_SEND          2'b00
`define DCACHE_REDUCE_RECV          2'b01

`define DCACHE_REDUCE_MAX_LEVEL      2
`define DCACHE_REDUCE_MAX_PARTNER_ID 7

// TLB
`define DCACHE_TLB_ENTRIES 8

// Tensol Load
`define DCACHE_TL_L2_TRANSFERS           4
`define DCACHE_TL_L2_TRANSFERS_RANGE     (`DCACHE_TL_L2_TRANSFERS-1):0
`define DCACHE_TL_L2_TRANSFERS_SEL_RANGE ($clog2(`DCACHE_TL_L2_TRANSFERS)-1):0

`define DCACHE_TENSOR_CTRL_NLINES_BITS   4
`define DCACHE_TENSOR_CTRL_NLINES_RANGE  (`DCACHE_TENSOR_CTRL_NLINES_BITS-1):0

`define DCACHE_TENSOR_CTRL_DEST_BITS     $clog2(`DCACHE_WAYS*`DCACHE_SETS)
`define DCACHE_TENSOR_CTRL_DEST_RANGE    (`DCACHE_TENSOR_CTRL_DEST_BITS-1):0

// Tensro Load Errors
`define DCACHE_TL_ERROR_BITS             5
`define DCACHE_TL_ERROR_UNLOCK_LINE      0
`define DCACHE_TL_ERROR_TRANS_VALID      1
`define DCACHE_TL_ERROR_WRONG_OFFST      2
`define DCACHE_TL_ERROR_ACCESS_FAULT_0   3
`define DCACHE_TL_ERROR_ACCESS_FAULT_1   4

// Other tensor errors
`define DCACHE_TX_ERROR_SCPBT_UNSET      4

// Maximum entry index for scratchpad
`define DCACHE_TL_SCP_MAX_IDX            48
`define DCACHE_TL_SCP_MOD                12        

// Number of loads that we send look ahead when doing TENB
`define DCACHE_TL_TENB_LOOK_AHEAD        2
`define DCACHE_TL_TENB_CREDITS           ((`VPU_TENB_SIZE/2)+`DCACHE_TL_TENB_LOOK_AHEAD)
`define DCACHE_TENB_BITS                 $clog2(`VPU_TENB_SIZE/2)
`define DCACHE_TENB_R                    (`DCACHE_TENB_BITS-1):0

`define DCACHE_L1_SCP_DST_BITS           `DCACHE_TENSOR_CTRL_DEST_BITS
`define DCACHE_L1_SCP_DST_RANGE          (`DCACHE_L1_SCP_DST_BITS-1):0

`define DCACHE_L2_SCP_DST_BITS           17
`define DCACHE_L2_SCP_DST_RANGE          (`DCACHE_L2_SCP_DST_BITS-1):0

// Address translation buffer for TensorStore
`define DCACHE_TS_PHYS_ADDR_NUM       2
`define DCACHE_TS_TRANS_ADDR_CNT_BITS 2

// Rate control for TensorStore
`define DCACHE_TS_RATE_BITS           4
`define DCACHE_TS_RATE_COUNTER_BITS   6

// Backoff control for TensorStore/TensorReduce
`define DCACHE_TS_BACKOFF_COUNTER_BITS  4

// Cache Ops
`define DCACHE_CO_ERROR_LOCK_VA        5
`define DCACHE_CO_L2_MAX_OPS_BITS      5
`define DCACHE_CO_L2_REPRATE_BITS      3
`define DCACHE_CO_L2_CYCLE_BITS        9
`define DCACHE_CO_L2_GRANT_BITS        2 

// Flags carried in cache_ops_err_flags
`define DCACHE_ERR_FLAG_PARTNER_ID     0
`define DCACHE_ERR_FLAG_CO_LOCK        0
`define DCACHE_ERR_FLAG_MEM_FAULT      1
`define DCACHE_ERR_FLAG_RANGE          1:0

// Tbox
`define DCACHE_TEXSND_MAX_REQ_ALIVE      8
`define DCACHE_TEXSND_REQ_COUNT_BITS     4
`define DCACHE_TEXSND_WB_COUNT_BITS      3
`define DCACHE_TEXSND_PULL_SIZE_BITS     3

// VM status selection indexes
`define DCACHE_VM_SEL_BITS  3
`define DCACHE_VM_SEL_RANGE (`DCACHE_VM_SEL_BITS-1):0
`define DCACHE_VM_SEL_IP    `DCACHE_VM_SEL_BITS'd0
`define DCACHE_VM_SEL_CO    `DCACHE_VM_SEL_BITS'd1
`define DCACHE_VM_SEL_TL0   `DCACHE_VM_SEL_BITS'd2
`define DCACHE_VM_SEL_TL1   `DCACHE_VM_SEL_BITS'd3
`define DCACHE_VM_SEL_TS    `DCACHE_VM_SEL_BITS'd4
`define DCACHE_VM_SEL_L2    `DCACHE_VM_SEL_BITS'd5

// Debug
`define DCACHE_DBG_ADDR_BITS 10
`define DCACHE_DBG_ADDR_RANGE (`DCACHE_DBG_ADDR_BITS-1):0

`define DCACHE_DBG_MD_DATA_RANGE ($bits(dcache_meta_data)-1):0
`define DCACHE_DBG_MD_ADDR_RANGE `DCACHE_SET_WAY_RANGE

`define DCACHE_DBG_DA_DATA_RANGE (`bpam_shire_apb_data_width-1):0  // Should be 64 bits
`define DCACHE_DBG_DA_ADDR_BITS  9                               // Addressing 4k bytes = 1/2k Dword 
`define DCACHE_DBG_DA_ADDR_RANGE (`DCACHE_DBG_DA_ADDR_BITS-1):0

/* RTLMIN-5542
`define DCACHE_DBG_CSR0_TL_SIZE 53
`define DCACHE_DBG_CSR1_TL_SIZE 16
*/
`define DCACHE_DBG_CSR0_TL_SIZE 3
`define DCACHE_DBG_CSR1_TL_SIZE 0
`define DCACHE_DBG_CSR_TL_SIZE  (`DCACHE_DBG_CSR0_TL_SIZE + `DCACHE_DBG_CSR1_TL_SIZE)
`define DCACHE_DBG_CSR0_TL_RANGE (`DCACHE_DBG_CSR0_TL_SIZE-1):0
`define DCACHE_DBG_CSR1_TL_RANGE `DCACHE_DBG_CSR0_TL_SIZE +:`DCACHE_DBG_CSR1_TL_SIZE

/* RTLMIN-5542
`define DCACHE_DBG_CSR0_TS_SIZE 41
`define DCACHE_DBG_CSR1_TS_SIZE 38
*/
`define DCACHE_DBG_CSR0_TS_SIZE 4
`define DCACHE_DBG_CSR1_TS_SIZE 0
`define DCACHE_DBG_CSR_TS_SIZE (`DCACHE_DBG_CSR0_TS_SIZE + `DCACHE_DBG_CSR1_TS_SIZE)
`define DCACHE_DBG_CSR0_TS_RANGE (`DCACHE_DBG_CSR0_TS_SIZE-1):0
`define DCACHE_DBG_CSR1_TS_RANGE `DCACHE_DBG_CSR0_TS_SIZE +:`DCACHE_DBG_CSR1_TS_SIZE

/* RTLMIN-5542
`define DCACHE_DBG_CSR0_COU_SIZE 44
`define DCACHE_DBG_CSR1_COU_SIZE 64
*/
`define DCACHE_DBG_CSR0_COU_SIZE 4
`define DCACHE_DBG_CSR1_COU_SIZE 0
`define DCACHE_DBG_CSR_COU_SIZE (`DCACHE_DBG_CSR0_COU_SIZE + `DCACHE_DBG_CSR1_COU_SIZE)
`define DCACHE_DBG_CSR0_COU_RANGE (`DCACHE_DBG_CSR0_COU_SIZE-1):0
`define DCACHE_DBG_CSR1_COU_RANGE `DCACHE_DBG_CSR0_COU_SIZE +:`DCACHE_DBG_CSR1_COU_SIZE

/* RTLMIN-5542
`define DCACHE_DBG_CSR0_COUL2_SIZE 64
`define DCACHE_DBG_CSR1_COUL2_SIZE 18
*/
`define DCACHE_DBG_CSR0_COUL2_SIZE 3
`define DCACHE_DBG_CSR1_COUL2_SIZE 0
`define DCACHE_DBG_CSR_COUL2_SIZE (`DCACHE_DBG_CSR0_COUL2_SIZE + `DCACHE_DBG_CSR1_COUL2_SIZE)
`define DCACHE_DBG_CSR0_COUL2_RANGE (`DCACHE_DBG_CSR0_COUL2_SIZE-1):0
`define DCACHE_DBG_CSR1_COUL2_RANGE `DCACHE_DBG_CSR0_COUL2_SIZE+:`DCACHE_DBG_CSR1_COUL2_SIZE

/* RTLMIN-5542
`define DCACHE_DBG_CSR0_TXSND_SIZE 38
*/
`define DCACHE_DBG_CSR0_TXSND_SIZE 2
`define DCACHE_DBG_CSR_TXSND_SIZE (`DCACHE_DBG_CSR0_TXSND_SIZE)
`define DCACHE_DBG_CSR0_TXSND_RANGE (`DCACHE_DBG_CSR0_TXSND_SIZE-1):0

/* RTLMIN-5542
`define DCACHE_DBG_CSR0_MH_SIZE 31
*/
`define DCACHE_DBG_CSR0_MH_SIZE 4
`define DCACHE_DBG_CSR_MH_SIZE (`DCACHE_DBG_CSR0_MH_SIZE)
`define DCACHE_DBG_CSR0_MH_RANGE (`DCACHE_DBG_CSR0_MH_SIZE-1):0


// Status Monitor connections
`define DCACHE_DBG_SM_MATCH_BITS      64
`define DCACHE_DBG_SM_FILTER_BITS     200
`define DCACHE_DBG_SM_DATA_GROUP_BITS 128
`define DCACHE_DBG_SM_DATA_BITS       (4*`DCACHE_DBG_SM_DATA_GROUP_BITS)
`define DCACHE_DBG_SM_DATA_GROUP(G)   (G*`DCACHE_DBG_SM_DATA_GROUP_BITS)+:`DCACHE_DBG_SM_DATA_GROUP_BITS

// Gather/Scater progress control
`define DCACHE_GSC_BLK_IDX_BITS  2
`define DCACHE_GSC_BLK_IDX_RANGE (`DCACHE_GSC_BLK_IDX_BITS-1):0

`endif // _DCACHE_DEFINES_


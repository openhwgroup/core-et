// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`ifndef _CPU_DEFINES_
`define _CPU_DEFINES_

////////////////////////////////////////////////////////////////////////////////
// MACROS
////////////////////////////////////////////////////////////////////////////////

//===================================================================
// AXI
//===================================================================
`define CPU_AXI_ID_WIDTH    8
`define CPU_AXI_ADDR_WIDTH 32
`define CPU_AXI_LEN_WIDTH   8
`define CPU_AXI_SIZE_WIDTH  3
`define CPU_AXI_BURST_WIDTH 2
`define CPU_AXI_CACHE_WIDTH 4
`define CPU_AXI_PROT_WIDTH  3
`define CPU_AXI_DATA_WIDTH  `ET_LINE_DATA_SIZE
`define CPU_AXI_STRB_WIDTH  (`CPU_AXI_DATA_WIDTH/8)
`define CPU_AXI_RESP_WIDTH  2

`define CPU_AXI_ADDR_OFFSET $clog2(`CPU_AXI_STRB_WIDTH)

//===================================================================
// Floating point
//===================================================================
`define CPU_FP32_SIGN       31
`define CPU_FP32_EXP_MSB    30
`define CPU_FP32_EXP_LSB    23
`define CPU_FP32_EXP_RANGE  `CPU_FP32_EXP_MSB:`CPU_FP32_EXP_LSB
`define CPU_FP32_MANT_MSB   22
`define CPU_FP32_MANT_LSB    0
`define CPU_FP32_MANT_RANGE `CPU_FP32_MANT_MSB:`CPU_FP32_MANT_LSB

//===================================================================
// ETL2AXI BRIDGE
//===================================================================
`define ETL2AXI_TT_BANK_SIZE       8
`define ETL2AXI_TT_BANK_SIZE_L     $clog2(`ETL2AXI_TT_BANK_SIZE)
`define ETL2AXI_TT_BANK_WR_ENTRIES 4
`define ETL2AXI_TT_BANK_AT_ENTRIES 2

`define ETL2AXI_TT_BANK_DATA_ENTRIES   4
`define ETL2AXI_TT_BANK_DATA_ENTRIES_L $clog2(`ETL2AXI_TT_BANK_DATA_ENTRIES)

`define ETL2AXI_TT_SIZE            (`ETL2AXI_TT_BANK_SIZE*`MIN_PER_N)
`define ETL2AXI_TT_SIZE_L          $clog2(`ETL2AXI_TT_SIZE)

//===================================================================
// APB
//===================================================================
`define CPU_APB_ADDR_WIDTH 32
`define CPU_APB_ADDR_MSB   31
`define CPU_APB_DATA_WIDTH 64

`define CPU_APB_ADDR_DEST_START    11
`define CPU_APB_ADDR_DEST_WIDTH    20
`define CPU_APB_ADDR_DEST_RANGE    `CPU_APB_ADDR_DEST_START +: `CPU_APB_ADDR_DEST_WIDTH
`define CPU_APB_ADDR_HART_ESR      `CPU_APB_ADDR_DEST_WIDTH'b0000000??000000????0
`define CPU_APB_ADDR_NEIGH_ESR     `CPU_APB_ADDR_DEST_WIDTH'b0000000??010000?????
`define CPU_APB_ADDR_CPU_REG       `CPU_APB_ADDR_DEST_WIDTH'b0000000??11010??????
`define CPU_APB_ADDR_MINION_DCACHE `CPU_APB_ADDR_DEST_WIDTH'b0000001??00000???0??
`define CPU_APB_ADDR_NEIGH_ICACHE  `CPU_APB_ADDR_DEST_WIDTH'b0000001??010000000??
`define CPU_APB_ADDR_L1_ICACHE     `CPU_APB_ADDR_DEST_WIDTH'b0000001??110000?????
`define CPU_APB_ADDR_PLIC          `CPU_APB_ADDR_DEST_WIDTH'b010000??????????????
`define CPU_APB_ADDR_PP_START      22
`define CPU_APB_ADDR_PP_RANGE      `CPU_APB_ADDR_PP_START +: 2
`define CPU_APB_ADDR_MIN_OR_NEIGH_BIT 17
`define CPU_APB_ADDR_DBG_BIT       24

//===================================================================
// PLIC
//===================================================================
`define PLIC_APB_ADDR_WIDTH  23
`define PLIC_DATA_WIDTH      32
`define PLIC_NEXTINT_TARGETS (2*`THREADS_PER_N) // 2 prv modes per thread
`define PLIC_NEXTINT_SOURCES 6

`endif // _CPU_DEFINES_

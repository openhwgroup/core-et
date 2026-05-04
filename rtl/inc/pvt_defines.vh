// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

// pvt_defines.vh
//
// Defines for PVT (Process, Voltage, Temperature) monitoring.
//
// Contains project-specific PVT configuration defines and overrides
//
// defines only

`ifndef _PVT_DEFINES_
`define _PVT_DEFINES_

// SOC level number of items
// 35 pd/ts - each of 6x6 array of shires with the exception of the pshire
// 8 vm     - near each memshire
`define PVT_TS_NUM 35
`define PVT_PD_NUM `PVT_TS_NUM
`define PVT_VM_NUM 8
`define PVT_PVTC_NUM  5

// shire positioning within ts/pd
`define PVT_MINION_SHIRE_RANGE  `SOCTOP_MINION_SHIRES-1:0

// Pshire no longer has ts/pd (RTLMIN-3619)
//`define PVT_PSHIRE_RANGE        `SOCTOP_MINION_SHIRES
//`define PVT_IOSHIRE_RANGE       `SOCTOP_MINION_SHIRES+1

`define PVT_IOSHIRE_RANGE       `SOCTOP_MINION_SHIRES
`define PVT_UNUSED_RANGE        (`PVT_PVTC_NUM*`MRS_TS_NUM)-1:`PVT_IOSHIRE_RANGE+1

// Override vendor-default defines
`define PVT_USER_DEFINED

// Number of elements per PVTC.
// Configured for each PVTC 8PD / 8TS / 2VM
// Desire 36 PD, 36 TS (one per core shire)
// 8 VM (one per memshire)
// Use 5 PVTC
// PVTC 0-3 fully connected 8PD/8TS/2VM
// PVTC 4 partial connected 4PD/4TS/0VM
`define MRS_TS_NUM 8
`define MRS_VM_NUM 2
`define MRS_CH_NUM 16
`define MRS_PD_NUM 8

// Bit position of IO shire sensor signals within PVTC4 buses.
`define PVT_IOSHIRE_TS_LOCAL_RANGE    `SOCTOP_MINION_SHIRES-(4*`MRS_TS_NUM)
`define PVT_IOSHIRE_PD_LOCAL_RANGE    `SOCTOP_MINION_SHIRES-(4*`MRS_PD_NUM)

// Unused bits in PVTC4 buses.
`define PVT_IOSHIRE_TS_LOCAL_UNUSED_RANGE `MRS_TS_NUM-1:`PVT_IOSHIRE_TS_LOCAL_RANGE+1
`define PVT_IOSHIRE_PD_LOCAL_UNUSED_RANGE `MRS_PD_NUM-1:`PVT_IOSHIRE_PD_LOCAL_RANGE+1

// Used bits in PVTC4 buses (excluding the IO shire sensor bit).
`define PVT_IOSHIRE_TS_LOCAL_USED_RANGE `PVT_IOSHIRE_TS_LOCAL_RANGE-1:0
`define PVT_IOSHIRE_PD_LOCAL_USED_RANGE `PVT_IOSHIRE_PD_LOCAL_RANGE-1:0



`endif //  `ifndef _PVT_DEFINES_

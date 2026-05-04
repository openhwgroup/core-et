// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

// maxshire_defines.vh
//
// defines only

`ifndef _MAXSHIRE_DEFINES_
`define _MAXSHIRE_DEFINES_

/////////////////////////////////////////////////////
// Maxion neighborhood defines
/////////////////////////////////////////////////////

// Number of maxion cores
`define NUM_MAXION_CORES                    4
`define NUM_MAXION_CORES_LOG2               2

// Number of PLLs
`define MAXSHIRE_NUM_PLL                    2

// PLL wrapper APB parameters
`define ET_HPDPLL_WRAP_APB_AD_WIDTH         12
`define ET_HPDPLL_WRAP_APB_D_WIDTH          32

// Maxion neighborhood ESR parameters
`define ESR_MAXNEIGH_APB_AD_WIDTH           28
`define ESR_MAXNEIGH_APB_D_WIDTH            64

// Maxion SM control signal width
`define MAXNEIGH_DBG_SM_CTRL_WIDTH          8

// Maxion DM JTAG Manufacturer ID
`define MAXNEIGH_DM_JTAG_MFR_ID             11'b10101100101

// DFT
`define MAXSHIRE_DFT_SCAN_DUMP_CNTL_WIDTH   `SHIRE_DFT_SCAN_DUMP_CNTL_WIDTH   // extra scam dump control bits in addition to wei/mei

/////////////////////////////////////////////////////
// Maxion CRU defines
/////////////////////////////////////////////////////

// APB interface index assignment, must match SBM and shire level assignments below
`define MAXSHIRE_CRU_PLL_CORE_RANGE_LO      0
`define MAXSHIRE_CRU_PLL_CORE_RANGE_HI      `MAXSHIRE_CRU_PLL_CORE_RANGE_LO
`define MAXSHIRE_CRU_PLL_CORE_RANGE         `MAXSHIRE_CRU_PLL_CORE_RANGE_HI:`MAXSHIRE_CRU_PLL_CORE_RANGE_LO

`define MAXSHIRE_CRU_PLL_UNCORE_RANGE_LO    (`MAXSHIRE_CRU_PLL_CORE_RANGE_HI+1)
`define MAXSHIRE_CRU_PLL_UNCORE_RANGE_HI    `MAXSHIRE_CRU_PLL_UNCORE_RANGE_LO
`define MAXSHIRE_CRU_PLL_UNCORE_RANGE       `MAXSHIRE_CRU_PLL_UNCORE_RANGE_HI:`MAXSHIRE_CRU_PLL_UNCORE_RANGE_LO

/////////////////////////////////////////////////////
// Shire channel parameters
/////////////////////////////////////////////////////

`define MAXSHIRE_SC_APB_SLAVES              (`SC_BANKS + 1) //banks, shire esr
`define MAXSHIRE_SC_NUM_NEIGH               2             // maxion neighborhood and l2 hpf
`define MAXSHIRE_SC_NUM_RBOX                0
`define MAXSHIRE_SC_NUM_BANKS               `SC_BANKS
`define MAXSHIRE_SC_NUM_L3_MASTER_PORTS     2
`define MAXSHIRE_SC_NUM_L3_SLAVE_PORTS      1
`define MAXSHIRE_SC_BANK_ID_SIZE            `SC_BANK_ID_SIZE
`define MAXSHIRE_SC_BANK_SEL_ADDR_RANGE     `SC_BANK_SEL_ADDR_RANGE
`define MAXSHIRE_SC_NUM_BANK_REQQ_ENTRIES   16            // 16 per bank is 64 outstanding requests
`define MAXSHIRE_SC_SETS_PER_SUB_BANK       `SC_SETS_PER_SUB_BANK
`define MAXSHIRE_SC_ICACHE_MEMS_IMPLEMENTED 0
`define MAXSHIRE_L2HPF_IMPLEMENTED          1
`define MAXSHIRE_SC_NUM_UC                  1

// APB interface index assignment referenced to MAXSHIRE_SC_APB_SLAVES
`define MAXSHIRE_SC_BANKS_RANGE_LO          0
`define MAXSHIRE_SC_BANKS_RANGE_HI          (`MAXSHIRE_SC_BANKS_RANGE_LO+(`MAXSHIRE_SC_NUM_BANKS-1))
`define MAXSHIRE_SC_BANKS_RANGE             `MAXSHIRE_SC_BANKS_RANGE_HI:`MAXSHIRE_SC_BANKS_RANGE_LO

`define MAXSHIRE_SC_SHIRE_RANGE_LO          (`MAXSHIRE_SC_BANKS_RANGE_HI+1)
`define MAXSHIRE_SC_SHIRE_RANGE_HI          `MAXSHIRE_SC_SHIRE_RANGE_LO
`define MAXSHIRE_SC_SHIRE_RANGE             `MAXSHIRE_SC_SHIRE_RANGE_HI:`MAXSHIRE_SC_SHIRE_RANGE_LO

/////////////////////////////////////////////////////
// Shire channel neighborhood assignment
/////////////////////////////////////////////////////

`define MAXSHIRE_SC_MAX_NEIGH               0
`define MAXSHIRE_SC_L2PF_NEIGH              1

/////////////////////////////////////////////////////
// Shire bus master parameters
/////////////////////////////////////////////////////

`define MAXSHIRE_SBM_APB_SLAVES             (`MAXSHIRE_SC_NUM_BANKS + 2 + `MAXSHIRE_NUM_PLL) // shire cache banks, shire esr, maxneigh apb slave, core and uncore PLLs
`define MAXSHIRE_SBM_APB_AD_WIDTH           `MAX(`ESR_CACHE_BANK_APB_AD_PP_WIDTH, \
                                                 `MAX(`ESR_SHIRE_OTHER_APB_AD_PP_WIDTH, \
                                                      `MAX(`ESR_MAXNEIGH_APB_AD_WIDTH, \
                                                           `ET_HPDPLL_WRAP_APB_AD_WIDTH)))
`define MAXSHIRE_SBM_APB_D_WIDTH            `MAX(`ESR_APB_D_WIDTH, \
                                                 `MAX(`ESR_MAXNEIGH_APB_D_WIDTH, \
                                                      `ET_HPDPLL_WRAP_APB_D_WIDTH))

`define MAXSHIRE_SBM_BCAST_DSTS             1   // shire cache banks
`define MAXSHIRE_SBM_BCAST_BANKS_IDX        0

// APB interface index assignment referenced to MAXSHIRE_SBM_APB_SLAVES
`define MAXSHIRE_SBM_BANKS_RANGE_LO         0
`define MAXSHIRE_SBM_BANKS_RANGE_HI         (`MAXSHIRE_SBM_BANKS_RANGE_LO+(`MAXSHIRE_SC_NUM_BANKS-1))
`define MAXSHIRE_SBM_BANKS_RANGE            `MAXSHIRE_SBM_BANKS_RANGE_HI:`MAXSHIRE_SBM_BANKS_RANGE_LO

`define MAXSHIRE_SBM_SHIRE_RANGE_LO         (`MAXSHIRE_SBM_BANKS_RANGE_HI+1)
`define MAXSHIRE_SBM_SHIRE_RANGE_HI         `MAXSHIRE_SBM_SHIRE_RANGE_LO
`define MAXSHIRE_SBM_SHIRE_RANGE            `MAXSHIRE_SBM_SHIRE_RANGE_HI:`MAXSHIRE_SBM_SHIRE_RANGE_LO

`define MAXSHIRE_SBM_NEIGH_RANGE_LO         (`MAXSHIRE_SBM_SHIRE_RANGE_HI+1)
`define MAXSHIRE_SBM_NEIGH_RANGE_HI         `MAXSHIRE_SBM_NEIGH_RANGE_LO
`define MAXSHIRE_SBM_NEIGH_RANGE            `MAXSHIRE_SBM_NEIGH_RANGE_HI:`MAXSHIRE_SBM_NEIGH_RANGE_LO

`define MAXSHIRE_SBM_PLL_RANGE_LO           (`MAXSHIRE_SBM_NEIGH_RANGE_HI+1)
`define MAXSHIRE_SBM_PLL_RANGE_HI           (`MAXSHIRE_SBM_PLL_RANGE_LO+(`MAXSHIRE_NUM_PLL-1))
`define MAXSHIRE_SBM_PLL_RANGE              `MAXSHIRE_SBM_PLL_RANGE_HI:`MAXSHIRE_SBM_PLL_RANGE_LO

/////////////////////////////////////////////////////
// NoC defines
/////////////////////////////////////////////////////

`define MAXSHIRE_SYS_SLAVE_CLIENTS          2

`define MAXSHIRE_UC_SYS_SLAVE_CLIENT_ID     0
`define MAXSHIRE_SBM_SYS_SLAVE_CLIENT_ID    1

/////////////////////////////////////////////////////
// General Maxion shire defines
/////////////////////////////////////////////////////

`define MAXSHIRE_APB_SLAVES                 `MAXSHIRE_SBM_APB_SLAVES // shire cache banks, shire esr, maxneigh apb slave, core and uncore PLLs
`define MAXSHIRE_APB_AD_WIDTH               `MAX(`MAXSHIRE_SBM_APB_AD_WIDTH, `MAXSHIRE_BPAM_APB_ADDR_WIDTH)
`define MAXSHIRE_APB_D_WIDTH                `MAX(`MAXSHIRE_SBM_APB_D_WIDTH, `MAXSHIRE_BPAM_APB_DATA_WIDTH)

// APB interface index assignment referenced to MAXSHIRE_APB_SLAVES
`define MAXSHIRE_APB_BANKS_RANGE_LO         `MAXSHIRE_SBM_BANKS_RANGE_LO
`define MAXSHIRE_APB_BANKS_RANGE_HI         `MAXSHIRE_SBM_BANKS_RANGE_HI
`define MAXSHIRE_APB_BANKS_RANGE            `MAXSHIRE_SBM_BANKS_RANGE

`define MAXSHIRE_APB_SHIRE_RANGE_LO         `MAXSHIRE_SBM_SHIRE_RANGE_LO
`define MAXSHIRE_APB_SHIRE_RANGE_HI         `MAXSHIRE_SBM_SHIRE_RANGE_HI
`define MAXSHIRE_APB_SHIRE_RANGE            `MAXSHIRE_SBM_SHIRE_RANGE

`define MAXSHIRE_APB_NEIGH_RANGE_LO         `MAXSHIRE_SBM_NEIGH_RANGE_LO
`define MAXSHIRE_APB_NEIGH_RANGE_HI         `MAXSHIRE_SBM_NEIGH_RANGE_HI
`define MAXSHIRE_APB_NEIGH_RANGE            `MAXSHIRE_SBM_NEIGH_RANGE

`define MAXSHIRE_APB_PLL_RANGE_LO           `MAXSHIRE_SBM_PLL_RANGE_LO
`define MAXSHIRE_APB_PLL_RANGE_HI           `MAXSHIRE_SBM_PLL_RANGE_HI
`define MAXSHIRE_APB_PLL_RANGE              `MAXSHIRE_SBM_PLL_RANGE

// Defines for Maxion AXI channels used in axi2etl bridge.
`define MXN_AXI_ID_SIZE       6
`define MXN_AXI_ADDR_SIZE    40
`define MXN_AXI_USER_SIZE     1

`define MXN_AXI_DATA_SIZE   512
`define MXN_AXI_WSTRB_SIZE  (`MXN_AXI_DATA_SIZE/8)


`endif

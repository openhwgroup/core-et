// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`ifndef _IOSHIRE_DEFINES_
`define _IOSHIRE_DEFINES_

//
// NoC defines
//
`define IOSHIRE_NUM_L3_PORTS                2

//
// SP defines
//

// Error information, single and double bit error indicator, overflow bit
`define SP_ICACHE_ECC_CAUSE_WIDTH            ($bits(icache_err_log_info_t)+3)

//
// PLIC defines
//

`define IOS_N_IRQ_PU_PLIC            2       // number of interrupts from pins to PU PLIC
`define IOS_N_IRQ_SPIO_PLIC          2       // number of interrupts from pins to SPIO PLIC
`define IOS_N_IRQ_MXN_PLIC           4       // number of interrupts from pins to Maxion

`define IOS_SPIO_PLIC_N_INT_SRC_PU_PLIC      2          // interrupt source from pu plic
`define IOS_SPIO_PLIC_N_INT_SRC_I            (5+10)     // interrupt source from internal of spio
                                                        // TBR: place holder
                                                        //      5: from PVTC
                                                        //     10: from other IO IPs
`define IOS_SPIO_PLIC_N_INT_SRC              (`IOS_SPIO_PLIC_N_INT_SRC_I + `IOS_SPIO_PLIC_N_INT_SRC_PU_PLIC + `IOS_N_IRQ_SPIO_PLIC)
`define IOS_SPIO_PLIC_N_INT_TARGETS          2          // spio PLIC targets only to sp_int_seip and sp_int_meip

//
// ECO defines
//

`define ET_ECO_IOSHIRE_LRG_INPUT_WIDTH                  32
`define ET_ECO_IOSHIRE_LRG_OUTPUT_WIDTH                 32
`define ET_ECO_IOSHIRE_MED_INPUT_WIDTH                  16
`define ET_ECO_IOSHIRE_MED_OUTPUT_WIDTH                 16
`define ET_ECO_IOSHIRE_SML_INPUT_WIDTH                  8
`define ET_ECO_IOSHIRE_SML_OUTPUT_WIDTH                 8

//
// DFT
//
`define IOSHIRE_DFT_MODE_WIDTH                          `SHIRE_DFT_MODE_WIDTH
`define IOSHIRE_DFT_BC_CTL_WIDTH                        `SHIRE_DFT_BC_CTL_WIDTH
`define IOSHIRE_DFT_CNTL_WIDTH                          `SHIRE_DFT_CNTL_WIDTH
`define IOSHIRE_DFT_SCAN_DUMP_CNTL_WIDTH                `SHIRE_DFT_SCAN_DUMP_CNTL_WIDTH

`define IOSHIRE_DFT_SCANIN_E2W_WIDTH                    `SHIRE_DFT_SCANIN_SCANOUT_SHIRE_TOP_WIDTH
`define IOSHIRE_DFT_SCANIN_W2E_WIDTH                    `SHIRE_DFT_SCANIN_SCANOUT_SHIRE_TOP_WIDTH
`define IOSHIRE_DFT_SCANIN_MISC_WIDTH                   16
`define IOSHIRE_DFT_SCANIN_MESHSTOP_WIDTH               16
`define IOSHIRE_DFT_SCANIN_SPIO_WIDTH                   16
`define IOSHIRE_DFT_SCANIN_PU_WIDTH                     16
`define IOSHIRE_DFT_SCANIN_SPIO_NOC_WIDTH               16
`define IOSHIRE_DFT_SCANIN_PU_NOC_WIDTH                 16
`define IOSHIRE_DFT_SCANIN_MAXSHIRE_WIDTH               32
`define IOSHIRE_DFT_SCANIN_MAXSHIRE_SC_WIDTH            16

`define IOSHIRE_DFT_SCANOUT_E2W_WIDTH                   `SHIRE_DFT_SCANIN_SCANOUT_SHIRE_TOP_WIDTH
`define IOSHIRE_DFT_SCANOUT_W2E_WIDTH                   `SHIRE_DFT_SCANIN_SCANOUT_SHIRE_TOP_WIDTH
`define IOSHIRE_DFT_SCANOUT_MISC_WIDTH                  16
`define IOSHIRE_DFT_SCANOUT_MESHSTOP_WIDTH              16
`define IOSHIRE_DFT_SCANOUT_SPIO_WIDTH                  16
`define IOSHIRE_DFT_SCANOUT_PU_WIDTH                    16
`define IOSHIRE_DFT_SCANOUT_SPIO_NOC_WIDTH              16
`define IOSHIRE_DFT_SCANOUT_PU_NOC_WIDTH                16
`define IOSHIRE_DFT_SCANOUT_MAXSHIRE_WIDTH              32
`define IOSHIRE_DFT_SCANOUT_MAXSHIRE_SC_WIDTH           16

`endif // _IOSHIRE_DEFINES_

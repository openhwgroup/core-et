// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`ifndef _SOCTOP_DEFINES_
`define _SOCTOP_DEFINES_

// --------------------------------------------------------------------------
// Synchronizer stages
// --------------------------------------------------------------------------
// NOTE: per Snps, N=SYNC_STAGES=2 for all synchronizers >= 650mV (all syncs moved to _pd_hv domain)
`define SYNC_STAGES_400mV_500Mhz  2     // NOC
`define SYNC_STAGES_400mV_1000Mhz 2     // Minion
`define SYNC_STAGES_650mV_1000Mhz 2     // Shire cache
`define SYNC_STAGES_850mV_1066Mhz 2     // Memshire
`define SYNC_STAGES_750mV_500Mhz  2     // Pshire, IOShire
`define SYNC_STAGES_750mV_250Mhz  2     // Pshire
`define SYNC_STAGES_750mV_1000Mhz 2     // Maxshire
`define SYNC_STAGES_750mV_1500Mhz 2     // Maxshire
`define SYNC_STAGES_SYNC_RST      2     // number of sync stages for all sync rst modules - set to max of all sync stages

`define SYNC_STAGES_SHIRE_TO_CLKREF  2    
`define SYNC_STAGES_PLL_DLL_TO_SHIRE 2    
`define SYNC_STAGES_DMCTRL_CAPTURE 19    
// --------------------------------------------------------------------------
// General macros
`define NIBBLE0_R           7:0
`define NIBBLE1_R           15:8
`define NIBBLE2_R           23:16
`define NIBBLE3_R           31:24


// Shire array size.
`define SOCTOP_ROWS         6
`define SOCTOP_COLS         6
`define SOCTOP_ROWS_L       $clog2(`SOCTOP_ROWS)
`define SOCTOP_COLS_L       $clog2(`SOCTOP_COLS)
`define SOCTOP_ROWS_R       (`SOCTOP_ROWS_L-1):0
`define SOCTOP_COLS_R       (`SOCTOP_COLS_L-1):0
`define SOCTOP_ROWS_MAX     (2 ** `SOCTOP_ROWS_L)
`define SOCTOP_COLS_MAX     (2 ** `SOCTOP_COLS_L)


// If we ever change the value of SOCTOP_MINION_SHIRES or SOCTOP_MEMSHIRES_PER_SIDE,
// please update the file $RTLROOT/ioshire/common/database/ioshire.py.  That file
// contains embedded python used to generate the ioshire verilog.  Unfortunately,
// the embedded python cannot evaluate verilog macros so I had to hardcode the values
// 34 and 4 into this file.  Search for SOCTOP_MINION_SHIRES or SOCTOP_MEMSHIRES_PER_SIDE
// to find the spot that needs to be modified.

`define SOCTOP_MINION_SHIRES              34
`define SOCTOP_MEMSHIRES_PER_SIDE          4
`define SOCTOP_MEMSHIRES_PER_SIDE_L        $clog2(`SOCTOP_MEMSHIRES_PER_SIDE)
`define SOCTOP_MEMSHIRES_PER_SIDE_R        (`SOCTOP_MEMSHIRES_PER_SIDE_L-1):0
`define SOCTOP_IO_SHIRES                   1
`define SOCTOP_PCI_SHIRES                  1

`define SOCTOP_N_MXN               4

// IO-shire(IOS) location in the shire array. The rows and columns are 0-based
// number. The Row 0 is the top row. The column 0 is the left-most column, at
// the east side.
`define SOCTOP_IOS_ROW_IDX         5
`define SOCTOP_IOS_COL_IDX         3
`define SOCTOP_PSHR_ROW_IDX        0
`define SOCTOP_PSHR_COL_IDX        3

//`define SOCTOP_STRAPS              16
//`define SOCTOP_STRAPS_R            (`SOCTOP_STRAPS-1):0
//`define SOCTOP_GPIOS               64
//`define SOCTOP_GPIOS_R             (`SOCTOP_GPIOS-1):0
//`define SOCTOP_GPIOS_HALF          (`SOCTOP_GPIOS/2)
//`define SOCTOP_GPIOS_HALF_R        (`SOCTOP_GPIOS_HALF-1):0
//`define SOCTOP_GPIOS_HI_R          (`SOCTOP_GPIOS-1):`SOCTOP_GPIOS_HALF
//`define SOCTOP_GPIOS_LO_R          (`SOCTOP_GPIOS_HALF-1):0

`define SOCTOP_MAX_MEM_FP_DATA_SZ  512

// GPIO ports to match the port macro defines in $RTLROOT/ip/synopsys/DW_apb_gpio/DW_apb_gpio_cc_constants.v
`define SOCTOP_GPIO_PWIDTH_A 8
`define SOCTOP_GPIO_PWIDTH_B 8
`define SOCTOP_GPIO_PWIDTH_C 8
`define SOCTOP_GPIO_PWIDTH_D 8

// TIMERS: macro defines must match these in $RTLROOT/ip/synopsys/DW_apb_timers/DW_apb_timers_cc_constants.v
`define SOCTOP_NUM_TIMERS 2


// These defines are no longer used.  The number of interrupt targets for the platform PLIC
// is now automatically derived from the platform PLIC interrupt source table in
// $RTLROOT/ioshire/common/database/ioshire.py.

// Number of interrupts from minion shires
//`define SOCTOP_N_INT_SHIRE      (2*`SOCTOP_MINION_SHIRES)
// Number of interrupt targets for platform PLIC
//`define SOCTOP_N_EXTINT         (`SOCTOP_N_INT_SHIRE+`IOS_N_IRQ_PU_PLIC)


`define SOCTOP_N_TARGETS_MNN    2                                                              // number of interrupts to master minion shire
`define SOCTOP_N_TARGETS_MXN    (`NUM_MAXION_CORES*2)                                          // number of interrupts to maxion shire

// Number of targets for platform PLIC
`define SOCTOP_N_TARGETS        (`SOCTOP_N_TARGETS_MNN + `SOCTOP_N_TARGETS_MXN + `IOS_SPIO_PLIC_N_INT_SRC_PU_PLIC)

// Layout of interrupt target vector from platform PLIC
`define SOCTOP_PU_PLIC_TARG_SP_R     (`IOS_SPIO_PLIC_N_INT_SRC_PU_PLIC-1):0
`define SOCTOP_PU_PLIC_TARG_MXN_R    (`IOS_SPIO_PLIC_N_INT_SRC_PU_PLIC+`SOCTOP_N_TARGETS_MXN-1):`IOS_SPIO_PLIC_N_INT_SRC_PU_PLIC
`define SOCTOP_PU_PLIC_TARG_MNN_R    (`IOS_SPIO_PLIC_N_INT_SRC_PU_PLIC+`SOCTOP_N_TARGETS_MXN+`SOCTOP_N_TARGETS_MNN-1):(`IOS_SPIO_PLIC_N_INT_SRC_PU_PLIC+`SOCTOP_N_TARGETS_MXN)


`define SOCTOP_DDR_CA_WIDTH    6
`define SOCTOP_DDR_CKE_WIDTH   2
`define SOCTOP_DDR_CS_WIDTH    2
`define SOCTOP_DDR_DQ_WIDTH   16
`define SOCTOP_DDR_DMI_WIDTH   2
`define SOCTOP_DDR_DSQ_WIDTH   2

// DFT related global defines

`define SOCTOP_DFT_BC_CTL_WIDTH     32  // dft broadcast control bits to all top-level blocks, sourced from DCU, terminates on each *_dft_hub module
`define SOCTOP_DFT_SI_SO_TOP_WIDTH  44  // scan in/out fabric bus width.


// Voltage monitors
`define SOCTOP_VM_NUMBER          `PVT_VM_NUM
`define SOCTOP_VM_POINTS          16
`define SOCTOP_VM_MINSHIRE_POINTS 3
`define SOCTOP_VM_MEMSHIRE_POINTS 2

// pad cell types
// These macros are used in soc_top.v for port declarations to give hints on what types of pad cells
// the ports are connected to, given the configuration of the pad cells through the connections and the tie-offs
// of the pad cells' ports. However, the values of the macros are all keyword "inout" to make it friendly 
// to the design tools, such as linter, etc., for connecting to the inout port of the pad cells two-level of hierarchy
// below soc_top. These macros are used for the soc_top ports connected to pad cells. The ports connected directly
// to other IPs may be treated differently depending on the IPs. 
`ifdef ZEBU
   `define INPUT  input
   `define OUTPUT output
`else
   `define INPUT  inout
   `define OUTPUT inout
`endif
`define INOUT     inout

`endif // _SOCTOP_DEFINES_

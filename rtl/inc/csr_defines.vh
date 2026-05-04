// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`ifndef _CSR_DEFINES_VH_
`define _CSR_DEFINES_VH_

`define CSR_VENDOR_ID 64'h565 // bank 11, code=0xE5 (0x65 without parity) => ((11-1)<<7) | ( 0xe5 & 0x7f)
`define CSR_ARCH_ID   64'h4554_0000_0000_0001
`define CSR_IMP_ID    64'h1_00_00

`define CSR_REG_NR_SZ 12

`define CSR_PRV_LEVELS 4
`define CSR_PRV_SZ     $clog2(`CSR_PRV_LEVELS)
`define CSR_PRV_U      (`CSR_PRV_SZ)'(unsigned'(0))
`define CSR_PRV_S      (`CSR_PRV_SZ)'(unsigned'(1))
`define CSR_PRV_H      (`CSR_PRV_SZ)'(unsigned'(2))
`define CSR_PRV_M      (`CSR_PRV_SZ)'(unsigned'(3))

`define CSR_ISA_MAX    ((1'b1 << 2)  | /* Compressed extension */                      \
                        (1'b1 << 5)  | /* Single-precision floating-point extension */ \
                        (1'b1 << 8)  | /* RV32I/64I/128I base ISA */                   \
                        (1'b1 << 12) | /* Integer Multiply/Divide extension */         \
                        (1'b1 << 18) | /* Supervisor mode implemented */               \
                        (1'b1 << 20) | /* User mode implemented */                     \
                        (1'b1 << 23) | /* Non-standard extensions present */           \
                        (2'h2 << 62))  /* XLEN = 64-bit */

`define CSR_FIRST_COUNTER csr_ad_CYCLE
`define CSR_FIRST_HPM 3
`define CSR_LAST_CORE_HPM  6
`define CSR_LAST_HPM  8
`define CSR_NR_HPM (`CSR_LAST_HPM - `CSR_FIRST_HPM + 1)
`define CSR_NR_COUNTERS (`CSR_LAST_HPM + 1) // number of implemented counters

`define CSR_MAX_NR_COUNTERS `CSR_NR_COUNTERS // not 32, because counters 9 to 31 are not implemented
`define CSR_MAX_NR_COUNTERS_L $clog2(`CSR_MAX_NR_COUNTERS)
`define CSR_MAX_NR_COUNTERS_R (`CSR_MAX_NR_COUNTERS_L-1):0

`define FCSR_RESET_VAL '{flags: '0, rm: 3'h7}
`define MSTATUS_RESET_VAL '{ mpp: `CSR_PRV_M, fs: 2'b00, sxl: 2'b10, uxl: 2'b10, default: '0}
`define CSR_NR_EVENTS 32
`define CSR_NR_EVENTS_DCACHE 12
`define CSR_NR_EVENTS_VPU    11
`define CSR_NR_EVENTS_BRANCH 2
`define CSR_NR_EVENTS_EXT    (`CSR_NR_EVENTS_DCACHE+`CSR_NR_EVENTS_VPU+`CSR_NR_EVENTS_BRANCH)
`define CSR_NR_EVENTS_DCACHE_VPU (`CSR_NR_EVENTS_DCACHE+`CSR_NR_EVENTS_VPU)
`define CSR_NR_EVENTS_L $clog2(`CSR_NR_EVENTS)
`define CSR_NR_EVENTS_R (`CSR_NR_EVENTS_L-1):0 
`define AMOFENCE_CTRL_RESET_VAL '{ af_rl: 1'b0, fa_aq: 1'b0, fa_rl: 1'b1, af_aq: 1'b1}
`define DCSR_RESET_VAL '{ prv: 2'b11, xdebugver: 4'h4, default: '0}
`define TDATA1_RESET_VAL '{ ttype: 4'h2, maskmax: 6'h6, default: '0}

`define CORE_MSG_PORT_ADDR_SZ 24
`define CORE_MSG_PORT_ADDR_RANGE (`CORE_MSG_PORT_ADDR_SZ-1):0
`define CORE_MSG_PORT_REDIRECT_OFFSET_SZ 30
`define CORE_MSG_PORT_REDIRECT_OFFSET_RANGE (`CORE_MSG_PORT_REDIRECT_OFFSET_SZ-1):0

`define CORE_MSG_PORT_MAX_MSGS_SZ 4 

`define CORE_MSG_ENABLE_R   4'b0111
`define CORE_MSG_DISABLE    4'b?011
`define CORE_MSG_PGET       4'b??00
`define CORE_MSG_PGETNB     4'b??01


`define CSR_SET_REPLAY( WHAT, EN, COND) \
     if ( (EN) && (COND) ) wb_csr_replay[WHAT] = 1'b1
`define CSR_SET_TENSOR_WAIT_REPLAY(WHAT, WAIT_TYPE, COND) \
     if ( (|reg_tensor_wait_en) && (COND) && ( wb_replay_tensor_wait_cond == (WAIT_TYPE) ) )  \
          wb_csr_replay[WHAT] = 1'b1
`define CSR_CLEAR_STALL( WHAT,COND) \
     if ( COND ) clear_stall[thread][WHAT] = 1'b1


`define TENSOR_ERROR_SZ 10

// debug defines
`define CSR_DEBUG_ONLY_AD_MSB 8'h7B // regs with ad=0x7BX are only accessible in debug mode


`endif


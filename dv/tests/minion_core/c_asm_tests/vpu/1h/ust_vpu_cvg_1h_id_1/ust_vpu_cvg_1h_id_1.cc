/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>
#include "minion.h"
#include "macros.h"
#define VSNIP_RSV           : :  : "f31","x21","x24","x29","x30","x31"
int main() {
		__asm__ __volatile__ ( 
			"j LBL_TEST_START\n"          
			VSNIP_RSV     
		);
		__asm__ __volatile__ (
			".equ EXCP_INSTRUCTION_ADDRESS_MISALIGN,    0\n"
			".equ EXCP_INSTRUCTION_ACCESS_FAULT,        1\n"
			".equ EXCP_ILLEGAL_INSTRUCTION,             2\n"
			".equ EXCP_BREAKPOINT,                      3\n"
			".equ EXCP_LOAD_ADDRESS_MISALIGNED,         4\n"
			".equ EXCP_LOAD_ACCESS_FAULT,               5\n"
			".equ EXCP_STORE_OR_AMO_ADDRESS_MISALIGNED, 6\n"
			".equ EXCP_STORE_OR_AMO_ACCESS_FAULT,       7\n"
			".equ EXCP_ECALL_FROM_UMODE,                8\n"
			".equ EXCP_ECALL_FROM_SMODE,                9\n"
			".equ EXCP_ECALL_FROM_MMODE,                11\n"
			".equ EXCP_INSTRUCTION_PAGE_FAULT,          12\n"
			".equ EXCP_LOAD_PAGE_FAULT,                 13\n"
			".equ EXCP_STORE_OR_AMO_PAGE_FAULT,         15\n"
			".equ EXCP_INSTRUCTION_BUS_ERROR,           25\n"
			".equ EXCP_INSTRUCTION_ECC_ERROR,           26\n"
			".equ EXCP_LOAD_SPLIT_PAGE_FAULT,           27\n"
			".equ EXCP_STORE_SPLIT_PAGE_FAULT,          28\n"
			".equ EXCP_MCODE_EMULATION,                 30\n"
			".equ TENSOR_WAIT_TENSORLOAD_0,             0\n"
			".equ TENSOR_WAIT_TENSORLOAD_1,             1\n"
			".equ TENSOR_WAIT_TENSORLOADL2_0,           2\n"
			".equ TENSOR_WAIT_TENSORLOADL2_1,           3\n"
			".equ TENSOR_WAIT_PREFETCH_0,               4\n"
			".equ TENSOR_WAIT_PREFETCH_1,               5\n"
			".equ TENSOR_WAIT_CACHEOP,                  6\n"
			".equ TENSOR_WAIT_TENSORFMA,                7\n"
			".equ TENSOR_WAIT_TENSORSTORE,              8\n"
			".equ TENSOR_WAIT_TENSORREDUCE,             9\n"
			".equ TENSOR_WAIT_TENSORQUANT,              10\n"
			".equ TENSOR_REDUCE_SEND_START_REG,                       57\n"
			".equ TENSOR_REDUCE_SEND_NUM_REG,                         16\n"
			".equ TENSOR_REDUCE_SEND_PARTNERID,                       3\n"
			".equ TENSOR_REDUCE_SEND_OPC_00,                          0\n"
			".equ TENSOR_REDUCE_RCV_START_REG,                        57\n"
			".equ TENSOR_REDUCE_RCV_OPERATION,                        24\n"
			".equ TENSOR_REDUCE_RCV_NUM_REG,                          16\n"
			".equ TENSOR_REDUCE_RCV_PARTNERID,                        3\n"
			".equ TENSOR_REDUCE_RCV_OPC_01,                           0\n"
			".equ TENSOR_REDUCE_AUTO_START_REG,                       57\n"
			".equ TENSOR_REDUCE_AUTO_OPERATION,                       24\n"
			".equ TENSOR_REDUCE_AUTO_NUM_REG,                         16\n"
			".equ TENSOR_REDUCE_AUTO_DEPTH,                           3\n"
			".equ TENSOR_REDUCE_AUTO_OPC_11,                          0\n"
			".equ TENSOR_REDUCE_BCAST_START_REG,                      57\n"
			".equ TENSOR_REDUCE_BCAST_OPERATION,                      24\n"
			".equ TENSOR_REDUCE_BCAST_NUM_REG,                        16\n"
			".equ TENSOR_REDUCE_BCAST_DEPTH,                          3\n"
			".equ TENSOR_REDUCE_BCAST_OPC_10,                         0\n"
			".equ TENSOR_FMA16_USE_TMASK,                             63\n"
			".equ TENSOR_FMA16_B_NUM_COLS,                            55\n"
			".equ TENSOR_FMA16_A_NUM_ROWS,                            51\n"
			".equ TENSOR_FMA16_A_NUM_COLS,                            47\n"
			".equ TENSOR_FMA16_OFFSET,                                43\n"
			".equ TENSOR_FMA16_TEN_B_IN_MEM,                          20\n"
			".equ TENSOR_FMA16_SCP_LOC_B,                             12\n"
			".equ TENSOR_FMA16_SCP_LOC_A,                             4\n"
			".equ TENSOR_FMA16_OPC_001,                               1\n"
			".equ TENSOR_FMA16_FIRST_PASS,                            0\n"
			".equ TENSOR_IMA_USE_TMASK,                               63\n"
			".equ TENSOR_IMA_B_NUM_COLS,                              55\n"
			".equ TENSOR_IMA_A_NUM_ROWS,                              51\n"
			".equ TENSOR_IMA_A_NUM_COLS,                              47\n"
			".equ TENSOR_IMA_OFFSET,                                  43\n"
			".equ TENSOR_IMA_TO_VRF,                                  23\n"
			".equ TENSOR_IMA_U_B,                                     22\n"
			".equ TENSOR_IMA_U_A,                                     21\n"
			".equ TENSOR_IMA_TEN_B_IN_MEM,                            20\n"
			".equ TENSOR_IMA_SCP_LOC_B,                               12\n"
			".equ TENSOR_IMA_SCP_LOC_A,                               4\n"
			".equ TENSOR_IMA_OPC_011,                                 1\n"
			".equ TENSOR_IMA_RESET_TENC,                              0\n"
			".equ TENSOR_FMA32_USE_TMASK,                             63\n"
			".equ TENSOR_FMA32_B_NUM_COLS,                            55\n"
			".equ TENSOR_FMA32_A_NUM_ROWS,                            51\n"
			".equ TENSOR_FMA32_A_NUM_COLS,                            47\n"
			".equ TENSOR_FMA32_OFFSET,                                43\n"
			".equ TENSOR_FMA32_TEN_B_IN_MEM,                          20\n"
			".equ TENSOR_FMA32_SCP_LOC_B,                             12\n"
			".equ TENSOR_FMA32_SCP_LOC_A,                             4\n"
			".equ TENSOR_FMA32_OPC_000,                               1\n"
			".equ TENSOR_FMA32_FIRST_PASS,                            0\n"
			".equ TENSOR_CONV_SIZE_SROW,                              56\n"
			".equ TENSOR_CONV_SIZE_NROW,                              32\n"
			".equ TENSOR_CONV_SIZE_SCOL,                              24\n"
			".equ TENSOR_CONV_SIZE_NCOL,                              0\n"
			".equ TENSOR_CONV_CTRL_ROWSTART,                          32\n"
			".equ TENSOR_CONV_CTRL_COLSTART,                          0\n"
			".equ TENSOR_COOP_TIMEOUT,                                16\n"
			".equ TENSOR_COOP_COOPMASK,                               8\n"
			".equ TENSOR_COOP_COOPID,                                 0\n"
			".equ TENSOR_MASK_MASK,                                   0\n"
			".equ TENSOR_QUANT_START_REG,                             57\n"
			".equ TENSOR_QUANT_QUANT_COL,                             55\n"
			".equ TENSOR_QUANT_QUANT_ROW,                             51\n"
			".equ TENSOR_QUANT_QUANT_SCP_LOC,                         45\n"
			".equ TENSOR_QUANT_TRANSF9,                               36\n"
			".equ TENSOR_QUANT_TRANSF8,                               32\n"
			".equ TENSOR_QUANT_TRANSF7,                               28\n"
			".equ TENSOR_QUANT_TRANSF6,                               24\n"
			".equ TENSOR_QUANT_TRANSF5,                               20\n"
			".equ TENSOR_QUANT_TRANSF4,                               16\n"
			".equ TENSOR_QUANT_TRANSF3,                               12\n"
			".equ TENSOR_QUANT_TRANSF2,                               8\n"
			".equ TENSOR_QUANT_TRANSF1,                               4\n"
			".equ TENSOR_QUANT_TRANSF0,                               0\n"
			".equ TENSOR_ERROR_BAD_PARTNER,                           9\n"
			".equ TENSOR_ERROR_COOPSIZE_INV,                          8\n"
			".equ TENSOR_ERROR_MEM_FAULT,                             7\n"
			".equ TENSOR_ERROR_TENB_TFMA_BAD_PAIR,                    6\n"
			".equ TENSOR_ERROR_LOCK_VA_SW_FAIL,                       5\n"
			".equ TENSOR_ERROR_L1_SCP_DIS,                            4\n"
			".equ TENSOR_ERROR_FCC_OVFL,                              3\n"
			".equ TENSOR_ERROR_TLD_BYTE_OFFSET_ERR,                   2\n"
			".equ TENSOR_ERROR_TLD_XFORM_ERR,                         1\n"
			".equ TENSOR_ERROR_WARL0,                                 0\n"
			".equ TENSOR_LOAD_USE_TMASK,                              63\n"
			".equ TENSOR_LOAD_USE_COOP,                               62\n"
			".equ TENSOR_LOAD_OPC_000,                                59\n"
			".equ TENSOR_LOAD_DST_START,                              53\n"
			".equ TENSOR_LOAD_OPC_0,                                  52\n"
			".equ TENSOR_LOAD_VADDR,                                  6\n"
			".equ TENSOR_LOAD_NUM_LINES,                              0\n"
			".equ TENSOR_LOAD_SETUP_B_USE_COOP,                       62\n"
			".equ TENSOR_LOAD_SETUP_B_OPC_000,                        59\n"
			".equ TENSOR_LOAD_SETUP_B_OPC_1,                          52\n"
			".equ TENSOR_LOAD_SETUP_B_VADDR,                          6\n"
			".equ TENSOR_LOAD_SETUP_B_NUM_LINES,                      0\n"
			".equ TENSOR_LOAD_I8_USE_TMASK,                           63\n"
			".equ TENSOR_LOAD_I8_USE_COOP,                            62\n"
			".equ TENSOR_LOAD_I8_OPC_001,                             59\n"
			".equ TENSOR_LOAD_I8_DST_START,                           53\n"
			".equ TENSOR_LOAD_I8_VADDR,                               6\n"
			".equ TENSOR_LOAD_I8_OFFSET,                              4\n"
			".equ TENSOR_LOAD_I8_NUM_LINES,                           0\n"
			".equ TENSOR_LOAD_I16_USE_TMASK,                          63\n"
			".equ TENSOR_LOAD_I16_USE_COOP,                           62\n"
			".equ TENSOR_LOAD_I16_OPC_010,                            59\n"
			".equ TENSOR_LOAD_I16_DST_START,                          53\n"
			".equ TENSOR_LOAD_I16_VADDR,                              6\n"
			".equ TENSOR_LOAD_I16_OFFSET,                             5\n"
			".equ TENSOR_LOAD_I16_NUM_LINES,                          0\n"
			".equ TENSOR_LOAD_T8_USE_TMASK,                           63\n"
			".equ TENSOR_LOAD_T8_USE_COOP,                            62\n"
			".equ TENSOR_LOAD_T8_OPC_101,                             59\n"
			".equ TENSOR_LOAD_T8_DST_START,                           53\n"
			".equ TENSOR_LOAD_T8_VADDR,                               6\n"
			".equ TENSOR_LOAD_T8_OFFSET,                              4\n"
			".equ TENSOR_LOAD_T8_NUM_LINES,                           0\n"
			".equ TENSOR_LOAD_T16_USE_TMASK,                          63\n"
			".equ TENSOR_LOAD_T16_USE_COOP,                           62\n"
			".equ TENSOR_LOAD_T16_OPC_110,                            59\n"
			".equ TENSOR_LOAD_T16_DST_START,                          53\n"
			".equ TENSOR_LOAD_T16_VADDR,                              6\n"
			".equ TENSOR_LOAD_T16_OFFSET,                             5\n"
			".equ TENSOR_LOAD_T16_NUM_LINES,                          0\n"
			".equ TENSOR_LOAD_T32_USE_TMASK,                          63\n"
			".equ TENSOR_LOAD_T32_USE_COOP,                           62\n"
			".equ TENSOR_LOAD_T32_OPC_111,                            59\n"
			".equ TENSOR_LOAD_T32_DST_START,                          53\n"
			".equ TENSOR_LOAD_T32_VADDR,                              6\n"
			".equ TENSOR_LOAD_T32_NUM_LINES,                          0\n"
			".equ TENSOR_LOAD_L2SCP_USE_TMASK,                        63\n"
			".equ TENSOR_LOAD_L2SCP_START_22_8,                       48\n"
			".equ TENSOR_LOAD_L2SCP_VADDR,                            6\n"
			".equ TENSOR_LOAD_L2SCP_START_7_6,                        4\n"
			".equ TENSOR_LOAD_L2SCP_NUM_LINES,                        0\n"
			".equ TENSOR_STORE_SCP_ENTRY_STRIDE,                      62\n"
			".equ TENSOR_STORE_SCP_START_SCP_ENTRY,                   56\n"
			".equ TENSOR_STORE_SCP_AROWS,                             51\n"
			".equ TENSOR_STORE_SCP_OPC_1,                             48\n"
			".equ TENSOR_STORE_SCP_VADDR,                             6\n"
			".equ TENSOR_STORE_REG_STRIDE,                            62\n"
			".equ TENSOR_STORE_START_REG,                             57\n"
			".equ TENSOR_STORE_STORE_COLS,                            55\n"
			".equ TENSOR_STORE_AROWS,                                 51\n"
			".equ TENSOR_STORE_COOP_STORE,                            49\n"
			".equ TENSOR_STORE_OPC_0,                                 48\n"
			".equ TENSOR_STORE_VADDR,                                 4\n"
			".equ EVICT_SW_USE_TMASK,                                 63\n"
			".equ EVICT_SW_DEST_LVL,                                  58\n"
			".equ EVICT_SW_SET,                                       14\n"
			".equ EVICT_SW_WAY,                                       6\n"
			".equ EVICT_SW_NUM_LINES,                                 0\n"
			".equ FLUSH_SW_USE_TMASK,                                 63\n"
			".equ FLUSH_SW_DEST_LVL,                                  58\n"
			".equ FLUSH_SW_SET,                                       14\n"
			".equ FLUSH_SW_WAY,                                       6\n"
			".equ FLUSH_SW_NUM_LINES,                                 0\n"
			".equ LOCK_SW_WAY,                                        55\n"
			".equ LOCK_SW_PA_39_6,                                    6\n"
			".equ UNLOCK_SW_WAY,                                      55\n"
			".equ UNLOCK_SW_SET,                                      6\n"
			".equ EVICT_VA_USE_TMASK,                                 63\n"
			".equ EVICT_VA_DEST_LVL,                                  58\n"
			".equ EVICT_VA_VA,                                        6\n"
			".equ EVICT_VA_NUM_LINES,                                 0\n"
			".equ FLUSH_VA_USE_TMASK,                                 63\n"
			".equ FLUSH_VA_DEST_LVL,                                  58\n"
			".equ FLUSH_VA_VA,                                        6\n"
			".equ FLUSH_VA_NUM_LINES,                                 0\n"
			".equ PREFETCH_VA_USE_TMASK,                              63\n"
			".equ PREFETCH_VA_DEST_LVL,                               58\n"
			".equ PREFETCH_VA_VA,                                     6\n"
			".equ PREFETCH_VA_NUM_LINES,                              0\n"
			".equ LOCK_VA_USE_TMASK,                                  63\n"
			".equ LOCK_VA_VA,                                         6\n"
			".equ LOCK_VA_NUM_LINES,                                  0\n"
			".equ UNLOCK_VA_USE_TMASK,                                63\n"
			".equ UNLOCK_VA_VA,                                       6\n"
			".equ UNLOCK_VA_NUM_LINES,                                0\n"
			".equ MCACHE_CONTROL_SCP_EN,                              1\n"
			".equ MCACHE_CONTROL_D1_SPLIT,                            0\n"
			".equ UCACHE_CONTROL_CACHEOP_MAX,                         6\n"
			".equ UCACHE_CONTROL_REP_RATE,                            2\n"
			".equ UCACHE_CONTROL_SCP_EN,                              1\n"
			".equ UCACHE_CONTROL_D1_SPLIT,                            0\n"
			VSNIP_RSV
		);
		__asm__ __volatile__ (
			".align 12\n"
			"LBL_M_MODE_E_H:\n"
			".global LBL_M_MODE_E_H\n"
			"addi sp, sp, -8\n"
			"sd x5, (sp)\n"
			"csrr x5, mcause\n"      
			"bltz x5, LBL_INTERRUPT_TABLE\n"
			"LBL_EXCEPTION_TABLE:\n"
			".global LBL_EXCEPTION_TABLE\n"
			"li x31, -1\n"
			"srli x31, x31, 1\n"
			"and x5, x5, x31\n"
			"li x31, 64\n"
			"mulw x31, x5, x31\n"
			"la x5, LBL_EXCP_INSTRUCTION_ADDRESS_MISALIGN_0\n"
			"add x31, x5, x31\n"
			"jr x31\n"
			".align 6\n"
			"LBL_EXCP_INSTRUCTION_ADDRESS_MISALIGN_0:\n"
			"j LBL_GOTO_TEST_FAIL\n"
			".align 6\n"
			"LBL_EXCP_INSTRUCTION_ACCESS_FAULT_1:\n"
			"j LBL_GOTO_TEST_FAIL\n"
			".align 6\n"
			"LBL_EXCP_ILLEGAL_INSTRUCTION_2:\n"
			"j LBL_GOTO_TEST_FAIL\n"
			".align 6\n"
			"LBL_EXCP_BREAKPOINT_3:\n"
			"csrr x5, mepc\n"      
			"addi x5, x5, 4\n" 
			"csrw mepc, x5\n" 
			"ld x5, (sp)\n"
			"addi sp, sp, 8\n"
			"mret\n"
			".align 6\n"
			"LBL_EXCP_LOAD_ADDRESS_MISALIGNED_4:\n"
			"csrr x5, mepc\n"      
			"addi x5, x5, 4\n" 
			"csrw mepc, x5\n" 
			"ld x5, (sp)\n"
			"addi sp, sp, 8\n"
			"mret\n"
			".align 6\n"
			"LBL_EXCP_LOAD_ACCESS_FAULT_5:\n"
			"csrr x5, mepc\n"      
			"addi x5, x5, 4\n" 
			"csrw mepc, x5\n" 
			"ld x5, (sp)\n"
			"addi sp, sp, 8\n"
			"mret\n"
			".align 6\n"
			"LBL_EXCP_STORE_OR_AMO_ADDRESS_MISALIGNED_6:\n"
			"csrr x5, mepc\n"      
			"addi x5, x5, 4\n" 
			"csrw mepc, x5\n" 
			"ld x5, (sp)\n"
			"addi sp, sp, 8\n"
			"mret\n"
			".align 6\n"
			"LBL_EXCP_STORE_OR_AMO_ACCESS_FAULT_7:\n"
			"csrr x5, mepc\n"      
			"addi x5, x5, 4\n" 
			"csrw mepc, x5\n" 
			"ld x5, (sp)\n"
			"addi sp, sp, 8\n"
			"mret\n"
			".align 6\n"
			"LBL_EXCP_ECALL_FROM_UMODE_8:\n"
			"lui	x5,0x2\n"      
			"addiw	x5,x5,-2048\n" 
			"csrrs   zero, mstatus, x5\n" 
			"j LBL_GOTO_TEST_FAIL\n"
			".align 6\n"
			"LBL_EXCP_ECALL_FROM_SMODE_9:\n"
			"lui	x5,0x2\n"      
			"addiw	x5,x5,-2048\n" 
			"csrrs   zero, mstatus, x5\n" 
			"j LBL_GOTO_TEST_FAIL\n"
			".align 6\n"
			"LBL_DUMMY_10:\n"
			"j LBL_GOTO_TEST_FAIL\n"
			".align 6\n"
			"LBL_EXCP_ECALL_FROM_MMODE_11:\n"
			"csrr x5, mepc\n"      
			"addi x5, x5, 4\n" 
			"csrw mepc, x5\n" 
			"ld x5, (sp)\n"
			"addi sp, sp, 8\n"
			"mret\n"
			".align 6\n"
			"LBL_EXCP_INSTRUCTION_PAGE_FAULT_12:\n"
			"j LBL_GOTO_TEST_FAIL\n"
			".align 6\n"
			"LBL_EXCP_LOAD_PAGE_FAULT_13:\n"
			"j LBL_GOTO_TEST_FAIL\n"
			".align 6\n"
			"LBL_DUMMY_14:\n"
			"j LBL_GOTO_TEST_FAIL\n"
			".align 6\n"
			"LBL_EXCP_STORE_OR_AMO_PAGE_FAULT_15:\n"
			"j LBL_GOTO_TEST_FAIL\n"
			".align 6\n"
			"LBL_DUMMY_16:\n"
			"j LBL_GOTO_TEST_FAIL\n"
			".align 6\n"
			"LBL_DUMMY_17:\n"
			"j LBL_GOTO_TEST_FAIL\n"
			".align 6\n"
			"LBL_DUMMY_18:\n"
			"j LBL_GOTO_TEST_FAIL\n"
			".align 6\n"
			"LBL_DUMMY_19:\n"
			"j LBL_GOTO_TEST_FAIL\n"
			".align 6\n"
			"LBL_DUMMY_20:\n"
			"j LBL_GOTO_TEST_FAIL\n"
			".align 6\n"
			"LBL_DUMMY_21:\n"
			"j LBL_GOTO_TEST_FAIL\n"
			".align 6\n"
			"LBL_DUMMY_22:\n"
			"j LBL_GOTO_TEST_FAIL\n"
			".align 6\n"
			"LBL_DUMMY_23:\n"
			"j LBL_GOTO_TEST_FAIL\n"
			".align 6\n"
			"LBL_DUMMY_24:\n"
			"j LBL_GOTO_TEST_FAIL\n"
			".align 6\n"
			"LBL_EXCP_INSTRUCTION_BUS_ERROR_25:\n"
			"j LBL_GOTO_TEST_FAIL\n"
			".align 6\n"
			"LBL_EXCP_INSTRUCTION_ECC_ERROR_26:\n"
			"j LBL_GOTO_TEST_FAIL\n"
			".align 6\n"
			"LBL_EXCP_LOAD_SPLIT_PAGE_FAULT_27:\n"
			"j LBL_GOTO_TEST_FAIL\n"
			".align 6\n"
			"LBL_EXCP_STORE_SPLIT_PAGE_FAULT_28:\n"
			"j LBL_GOTO_TEST_FAIL\n"
			".align 6\n"
			"LBL_DUMMY_29:\n"
			"j LBL_GOTO_TEST_FAIL\n"
			".align 6\n"
			"LBL_EXCP_MCODE_EMULATION_30:\n"
			"csrr x5, mepc\n"      
			"addi x5, x5, 4\n" 
			"csrw mepc, x5\n" 
			"ld x5, (sp)\n"
			"addi sp, sp, 8\n"
			"mret\n"
			"LBL_INTERRUPT_TABLE:\n"
			"nop\n"
			"nop\n"
			"nop\n"
			"nop\n"
			"nop\n"
			"nop\n"
			"nop\n"
			"nop\n"
			"nop\n"
			"nop\n"
			"nop\n"
			"nop\n"
			"nop\n"
			"nop\n"
			"nop\n"
			"j LBL_GOTO_TEST_FAIL\n"
			"LBL_GOTO_TEST_FAIL:\n"
			".global LBL_GOTO_TEST_FAIL\n"
			"csrw mcause, x0\n"
			"csrw mtval, x0\n"
			"csrr x5, mscratch\n"
			"csrw mepc, x5\n"       
			"li x5, 0xDEADDEAD\n"            
			"mret\n"                
			VSNIP_RSV     
		);
		__asm__ __volatile__ ( 
			"LBL_TEST_START:\n"          
			".global LBL_TEST_START\n"          
			VSNIP_RSV     
		);
	uint64_t hid;
	hid = get_hart_id();
	if ( hid%2 == 0 ) { 
		C_TEST_START;
		__asm__ __volatile__ (
			"LBL_PREAMBLE_HID_0:\n"          
			"la x5, LBL_M_MODE_E_H\n"
			"csrw mtvec, x5\n"
			"la x5, LBL_FAIL_HID_0\n"
			"csrw mscratch, x5\n"
			"mov.m.x m0, x0, 0xff\n"
			"csrwi menable_shadows, 0xf\n"
			"li x5, -1\n"
			"csrw mcounteren, x5\n"
			"csrw scounteren, x5\n"
			"add x21, x0, x0\n"
			"add x24, x0, x0\n"
			"add x29, x0, x0\n"
			"add x30, x0, x0\n"
			"add x31, x0, x0\n"
			"csrwi mhpmcounter3, 0\n"
			"csrwi mhpmcounter4, 0\n"
			"csrwi mhpmcounter5, 0\n"
			"csrwi mhpmcounter6, 0\n"
			"csrwi mhpmcounter7, 0\n"
			"csrwi mhpmcounter8, 0\n"
			"csrwi mhpmevent3, 19\n"   
			"csrwi mhpmevent4, 20\n"   
			"csrwi mhpmevent5, 4\n"   
			"csrwi mhpmevent6, 18\n"   
			"csrwi mhpmevent7, 6\n"   
			"csrwi mhpmevent8, 4\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f25,  96 (x5)\n"
			"flw.ps f1,  512 (x5)\n"
			"flw.ps f11,  800 (x5)\n"
			"flw.ps f4,  768 (x5)\n"
			"flw.ps f0,  416 (x5)\n"
			"flw.ps f15,  64 (x5)\n"
			"flw.ps f21,  576 (x5)\n"
			"flw.ps f28,  928 (x5)\n"
			"flw.ps f31,  672 (x5)\n"
			"flw.ps f22,  736 (x5)\n"
			"flw.ps f30,  832 (x5)\n"
			"flw.ps f17,  352 (x5)\n"
			"flw.ps f2,  32 (x5)\n"
			"flw.ps f26,  992 (x5)\n"
			"flw.ps f23,  896 (x5)\n"
			"flw.ps f6,  256 (x5)\n"
			"flw.ps f19,  608 (x5)\n"
			"flw.ps f7,  480 (x5)\n"
			"flw.ps f13,  224 (x5)\n"
			"flw.ps f24,  320 (x5)\n"
			"flw.ps f18,  384 (x5)\n"
			"flw.ps f14,  160 (x5)\n"
			"flw.ps f3,  128 (x5)\n"
			"flw.ps f16,  192 (x5)\n"
			"flw.ps f29,  288 (x5)\n"
			"flw.ps f9,  864 (x5)\n"
			"flw.ps f12,  0 (x5)\n"
			"flw.ps f5,  640 (x5)\n"
			"flw.ps f20,  960 (x5)\n"
			"flw.ps f8,  448 (x5)\n"
			"flw.ps f10,  704 (x5)\n"
			"flw.ps f27,  544 (x5)\n"
			"csrw fflags, zero\n"
			"csrwi frm, 0x0\n"
			"LBL_KERNEL_HID_0:\n"          
			"li x5, 0xFEEDFEED\n"
			"li x0, 0xc8a\n" 
			"slti x0, x0, 0x7f2\n"
			"la x31, check_sum_hid_0\n"
			"li x5, 0x4\n" 
			"sw x5, (x31)\n"
			"li x29, 0x4\n"
			"LBL_OUTER_LOOP_SEQ_HID_0:\n"
			"nop \n"
			"nop \n"
			"li x30, 0\n"
			"addi x30, x30, 1\n"
			"la x18, rand_ieee754_32\n"
			"flw.ps f27, 768(x18)\n"
			"flw.ps f18, 160(x18)\n"
			"fcmovm.ps f26, f27, f18\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f8, 928(x18)\n"
			"flw.ps f25, 192(x18)\n"
			"flw.ps f18, 640(x18)\n"
			"fnmadd.ps f14, f8, f25, f18, rdn\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f18, 544(x18)\n"
			"fcvt.lu.s x16, f18, rmm\n" 
			"la x25, rand_int_32\n"
			"flw.ps f30, 288(x25)\n"
			"fmulhu.pi f25, f30, f30\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f1, 896(x26)\n"
			"flw.ps f10, 480(x26)\n"
			"fcmov.ps f19, f1, f1, f10\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f4, 864(x25)\n"
			"fswizz.ps f25, f4, 0x3\n" 
			"la x17, rand_int_32\n"
			"flw.ps f20, 832(x17)\n"
			"fpackreph.pi f27, f20\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f7, 0(x9)\n"
			"fcvt.wu.s x26, f7, rdn\n" 
			"la x16, rand_int_32\n"
			"flw.ps f1, 896(x16)\n"
			"fle.pi f27, f1, f1\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f26, 832(x25)\n"
			"flw.ps f27, 864(x25)\n"
			"fsub.ps f7, f26, f27, rtz\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f14, 768(x16)\n"
			"flw.ps f13, 992(x16)\n"
			"fsgnjn.ps f0, f14, f13\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f27, 320(x15)\n"
			"fcvt.f16.ps f4, f27\n" 
			"la x25, rand_int_32\n"
			"flw.ps f10, 32(x25)\n"
			"flw.ps f20, 256(x25)\n"
			"fmulh.pi f19, f10, f20\n" 
			"la x15, rand_int_32\n"
			"flw.ps f1, 320(x15)\n"
			"flw.ps f0, 256(x15)\n"
			"fmaxu.pi f4, f1, f0\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_0:\n"
			"maskor m2, m4, m4\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x17, rand_ieee754_32\n"
			"flw.ps f27, 832(x17)\n"
			"fround.ps f7, f27, rne\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_0:\n"
			"maskand m4, m0, m4\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x9, rand_ieee754_32\n"
			"flw.ps f8, 992(x9)\n"
			"flw.ps f19, 352(x9)\n"
			"fsgnj.ps f18, f8, f19\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f20, 736(x26)\n"
			"fle.s x19, f20, f20\n" 
			"la x28, rand_int_32\n"
			"flw.ps f19, 928(x28)\n"
			"fcvt.ps.pwu f5, f19, rne\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f14, 992(x16)\n"
			"fsin.ps f13, f14\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f25, 320(x25)\n"
			"flw.ps f30, 992(x25)\n"
			"flw.ps f1, 608(x25)\n"
			"fmsub.ps f4, f25, f30, f1, rtz\n" 
			"LBL_SEQID_0_M0_WRITE_ID_2_HID_0:\n"
			"masknot m0, m5\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0x4958a27ace\n"
			"fsw.ps f0, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x17, rand_ieee754_32\n"
			"flw.ps f5, 96(x17)\n"
			"flw.ps f7, 288(x17)\n"
			"flt.ps f8, f5, f7\n" 
			"LBL_SEQID_0_M0_WRITE_ID_3_HID_0:\n"
			"maskand m2, m5, m0\n" 
			"mov.m.x m0, x0, 0xff\n"
			"li x5, 0x803400D0\n"
			"ld x5, 0(x5)\n"
			"la x22, rand_int_32\n"
			"flw.ps f18, 224(x22)\n"
			"flw.ps f26, 672(x22)\n"
			"flt.pi f30, f18, f26\n" 
			"la x15, rand_int_32\n"
			"flw.ps f26, 32(x15)\n"
			"flw.ps f0, 416(x15)\n"
			"feq.pi f27, f26, f0\n" 
			"LBL_SEQID_0_M0_WRITE_ID_4_HID_0:\n"
			"maskxor m4, m0, m4\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x26, rand_ieee754_32\n"
			"flw.ps f26, 672(x26)\n"
			"fcvt.wu.s x22, f26, rdn\n" 
			"LBL_SEQID_0_M0_WRITE_ID_5_HID_0:\n"
			"fltm.ps m0, f0, f14\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x25, rand_int_32\n"
			"flw.ps f25, 896(x25)\n"
			"flw.ps f20, 576(x25)\n"
			"for.pi f30, f25, f20\n" 
			"LBL_SEQID_0_M0_WRITE_ID_6_HID_0:\n"
			"mova.m.x x15\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_0\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_1_M0_WRITE_ID_7_HID_0:\n"
			"feqm.ps m0, f25, f20\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x22, rand_ieee754_32\n"
			"flw.ps f1, 512(x22)\n"
			"flw.ps f13, 352(x22)\n"
			"fmul.ps f14, f1, f13, rdn\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f20, 672(x25)\n"
			"fclass.ps f1, f20\n" 
			"li x19, 0x563cd0269b118d9c\n"
			"fcvt.s.wu f8, x19, rup\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f5, 928(x15)\n"
			"flw.ps f27, 128(x15)\n"
			"flw.ps f18, 32(x15)\n"
			"fmsub.s f10, f5, f27, f18, rmm\n" 
			"LBL_SEQID_1_M0_WRITE_ID_8_HID_0:\n"
			"mov.m.x m0, x17, 0x3c\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0x8c125b02bb\n"
			"flw.ps f5, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x28, rand_int_32\n"
			"flw.ps f19, 224(x28)\n"
			"flw.ps f30, 0(x28)\n"
			"for.pi f8, f19, f30\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f14, 416(x26)\n"
			"frsq.ps f13, f14\n" 
			"la x15, rand_int_32\n"
			"flw.ps f18, 256(x15)\n"
			"flw.ps f1, 384(x15)\n"
			"fmulh.pi f1, f18, f1\n" 
			"li x16, 0x3a9d100a6e42ff69\n"
			"fcvt.s.wu f10, x16, rup\n" 
			"LBL_SEQID_1_M0_WRITE_ID_9_HID_0:\n"
			"fsetm.pi m5, f19\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x18, rand_ieee754_32\n"
			"flw.ps f14, 352(x18)\n"
			"flw.ps f18, 448(x18)\n"
			"fmul.s f1, f14, f18, rtz\n" 
			"la x18, rand_int_32\n"
			"flw.ps f13, 832(x18)\n"
			"fcvt.ps.pw f13, f13, rtz\n" 
			"LBL_SEQID_1_FP_TRANS_ID_10_HID_0:\n"
			"la x22, rand_ieee754_32\n"
			"flw.ps f1, 704(x22)\n"
			"fexp.ps f0, f1\n" 
			"LBL_SEQID_1_M0_WRITE_ID_11_HID_0:\n"
			"fltm.pi m5, f14, f14\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x17, rand_ieee754_16\n"
			"flw.ps f20, 800(x17)\n"
			"fcvt.ps.f16 f19, f20\n" 
			"la x26, rand_int_32\n"
			"flw.ps f10, 160(x26)\n"
			"flw.ps f13, 672(x26)\n"
			"fmaxu.pi f1, f10, f13\n" 
			"la x25, rand_int_32\n"
			"flw.ps f27, 352(x25)\n"
			"fsat8.pi f13, f27\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f7, 608(x17)\n"
			"fclass.ps f8, f7\n" 
			"LBL_SEQID_1_M0_WRITE_ID_12_HID_0:\n"
			"maskxor m5, m2, m4\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x25, rand_ieee754_32\n"
			"flw.ps f5, 832(x25)\n"
			"fcvt.w.s x22, f5, rdn\n" 
			"mova.x.m x28\n" 
			"LBL_SEQID_1_FP_TRANS_ID_13_HID_0:\n"
			"la x9, rand_ieee754_32\n"
			"flw.ps f0, 672(x9)\n"
			"flog.ps f14, f0\n" 
			"la x16, rand_int_32\n"
			"flw.ps f14, 480(x16)\n"
			"flw.ps f20, 160(x16)\n"
			"feq.pi f1, f14, f20\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f4, 288(x18)\n"
			"flw.ps f20, 192(x18)\n"
			"flt.ps f1, f4, f20\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f7, 128(x25)\n"
			"fclass.s x26, f7\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f7, 768(x22)\n"
			"flw.ps f26, 192(x22)\n"
			"fsgnjx.s f10, f7, f26\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f10, 736(x18)\n"
			"flw.ps f25, 192(x18)\n"
			"fsub.ps f5, f10, f25, rdn\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f4, 704(x15)\n"
			"flw.ps f7, 576(x15)\n"
			"fsub.s f7, f4, f7, rdn\n" 
			"LBL_SEQID_1_M0_WRITE_ID_14_HID_0:\n"
			"fsetm.pi m5, f10\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x18, rand_int_32\n"
			"flw.ps f7, 832(x18)\n"
			"flw.ps f26, 608(x18)\n"
			"fsrl.pi f30, f7, f26\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f1, 864(x17)\n"
			"fswizz.ps f26, f1, 0x1e\n" 
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_0\n"
			"addi x30, x30, 1\n"
			"fbci.ps f18, 0x19cf3\n" 
			"la x22, rand_int_32\n"
			"flw.ps f20, 704(x22)\n"
			"flw.ps f8, 192(x22)\n"
			"fsrl.pi f5, f20, f8\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f14, 352(x22)\n"
			"fcvt.pwu.ps f5, f14, rup\n" 
			"la x28, rand_int_32\n"
			"flw.ps f19, 864(x28)\n"
			"fsat8.pi f30, f19\n" 
			"la x25, rand_int_32\n"
			"flw.ps f20, 480(x25)\n"
			"fsatu8.pi f27, f20\n" 
			"la x26, rand_int_32\n"
			"flw.ps f7, 288(x26)\n"
			"flw.ps f26, 0(x26)\n"
			"fsll.pi f19, f7, f26\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f7, 416(x17)\n"
			"flw.ps f10, 544(x17)\n"
			"flw.ps f1, 224(x17)\n"
			"fnmsub.ps f4, f7, f10, f1, rup\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f20, 800(x15)\n"
			"flw.ps f18, 704(x15)\n"
			"flw.ps f13, 800(x15)\n"
			"fmadd.ps f14, f20, f18, f13, rtz\n" 
			"la x18, rand_int_32\n"
			"flw.ps f1, 992(x18)\n"
			"flw.ps f13, 704(x18)\n"
			"fmaxu.pi f1, f1, f13\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f18, 128(x25)\n"
			"flw.ps f10, 992(x25)\n"
			"flt.s x17, f18, f10\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f18, 32(x9)\n"
			"flw.ps f20, 864(x9)\n"
			"fsgnjx.s f13, f18, f20\n" 
			"la x18, rand_int_32\n"
			"flw.ps f14, 864(x18)\n"
			"flw.ps f4, 448(x18)\n"
			"fmul.pi f5, f14, f4\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f10, 384(x22)\n"
			"flw.ps f27, 896(x22)\n"
			"fsub.ps f19, f10, f27, rmm\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f8, 928(x18)\n"
			"fround.ps f20, f8, rdn\n" 
			"la x19, rand_int_32\n"
			"flw.ps f4, 640(x19)\n"
			"fand.pi f13, f4, f4\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f4, 768(x9)\n"
			"flw.ps f19, 256(x9)\n"
			"fadd.s f18, f4, f19, rup\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f7, 352(x26)\n"
			"flw.ps f19, 192(x26)\n"
			"fmax.ps f19, f7, f19\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f30, 384(x22)\n"
			"flw.ps f8, 544(x22)\n"
			"fmsub.ps f14, f30, f8, f30, rdn\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f4, 608(x25)\n"
			"flw.ps f20, 992(x25)\n"
			"fsgnj.s f8, f4, f20\n" 
			"LBL_SEQID_2_M0_WRITE_ID_15_HID_0:\n"
			"masknot m0, m0\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x9, rand_int_32\n"
			"flw.ps f0, 672(x9)\n"
			"fsll.pi f4, f0, f0\n" 
			"la x19, rand_int_32\n"
			"flw.ps f30, 640(x19)\n"
			"flw.ps f13, 192(x19)\n"
			"fadd.pi f14, f30, f13\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f14, 800(x9)\n"
			"fcvt.pw.ps f19, f14, rtz\n" 
			"LBL_SEQID_2_FP_TRANS_ID_16_HID_0:\n"
			"la x28, rand_ieee754_32\n"
			"flw.ps f1, 448(x28)\n"
			"frcp.ps f19, f1\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f13, 928(x26)\n"
			"flw.ps f0, 288(x26)\n"
			"fmax.s f25, f13, f0\n" 
			"la x9, rand_int_32\n"
			"flw.ps f10, 448(x9)\n"
			"flw.ps f4, 128(x9)\n"
			"fand.pi f26, f10, f4\n" 
			"la x19, rand_int_32\n"
			"flw.ps f7, 32(x19)\n"
			"fsat8.pi f13, f7\n" 
			"la x22, rand_int_32\n"
			"flw.ps f4, 0(x22)\n"
			"fnot.pi f5, f4\n" 
			"la x22, rand_int_32\n"
			"flw.ps f14, 224(x22)\n"
			"flw.ps f19, 672(x22)\n"
			"fmulhu.pi f1, f14, f19\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f1, 896(x25)\n"
			"fcmovm.ps f30, f1, f1\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f18, 448(x19)\n"
			"fcvt.wu.s x19, f18, rtz\n" 
			"la x22, rand_int_32\n"
			"flw.ps f25, 640(x22)\n"
			"flw.ps f5, 320(x22)\n"
			"fmulh.pi f25, f25, f5\n" 
			"li x5, 3\n" 
			"bne x30, x5, LBL_FAIL_HID_0\n"
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f20,  736 (x5)\n"
			"flw.ps f14,  704 (x5)\n"
			"flw.ps f18,  0 (x5)\n"
			"flw.ps f11,  384 (x5)\n"
			"flw.ps f22,  864 (x5)\n"
			"flw.ps f2,  320 (x5)\n"
			"flw.ps f25,  480 (x5)\n"
			"flw.ps f23,  992 (x5)\n"
			"flw.ps f5,  256 (x5)\n"
			"flw.ps f1,  960 (x5)\n"
			"flw.ps f3,  768 (x5)\n"
			"flw.ps f15,  576 (x5)\n"
			"flw.ps f4,  608 (x5)\n"
			"flw.ps f21,  96 (x5)\n"
			"flw.ps f12,  128 (x5)\n"
			"flw.ps f7,  448 (x5)\n"
			"flw.ps f16,  800 (x5)\n"
			"flw.ps f8,  512 (x5)\n"
			"flw.ps f24,  928 (x5)\n"
			"flw.ps f29,  288 (x5)\n"
			"flw.ps f31,  160 (x5)\n"
			"flw.ps f13,  352 (x5)\n"
			"flw.ps f28,  544 (x5)\n"
			"flw.ps f17,  192 (x5)\n"
			"flw.ps f26,  224 (x5)\n"
			"flw.ps f30,  32 (x5)\n"
			"flw.ps f0,  832 (x5)\n"
			"flw.ps f10,  896 (x5)\n"
			"flw.ps f27,  672 (x5)\n"
			"flw.ps f19,  640 (x5)\n"
			"flw.ps f9,  416 (x5)\n"
			"flw.ps f6,  64 (x5)\n"
			"addi x30, x30, 1\n"
			"maskpopc x25, m0\n" 
			"mov.m.x m0, x0, 0xff\n"
			"la x17, rand_ieee754_32\n"
			"flw.ps f5, 800(x17)\n"
			"fround.ps f14, f5, rne\n" 
			"LBL_SEQID_3_M0_WRITE_ID_17_HID_0:\n"
			"mova.m.x x28\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x18, rand_ieee754_32\n"
			"flw.ps f18, 96(x18)\n"
			"flw.ps f14, 0(x18)\n"
			"fsgnjn.s f13, f18, f14\n" 
			"maskpopcz x16, m2\n" 
			"la x15, rand_ieee754_16\n"
			"flw.ps f1, 224(x15)\n"
			"fcvt.ps.f16 f7, f1\n" 
			"LBL_SEQID_3_M0_WRITE_ID_18_HID_0:\n"
			"fltm.ps m5, f4, f26\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x9, rand_ieee754_32\n"
			"flw.ps f20, 736(x9)\n"
			"fcvt.pwu.ps f26, f20, rne\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f30, 640(x28)\n"
			"flw.ps f27, 224(x28)\n"
			"fsub.s f25, f30, f27, rne\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f19, 832(x17)\n"
			"flw.ps f0, 800(x17)\n"
			"fmul.s f14, f19, f0, rdn\n" 
			"la x22, rand_int_32\n"
			"flw.ps f7, 672(x22)\n"
			"flw.ps f20, 288(x22)\n"
			"fremu.pi f30, f7, f20\n" 
			"la x15, rand_int_32\n"
			"flw.ps f27, 960(x15)\n"
			"flw.ps f14, 320(x15)\n"
			"fsrl.pi f5, f27, f14\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f5, 896(x16)\n"
			"flw.ps f30, 512(x16)\n"
			"flw.ps f10, 160(x16)\n"
			"fnmsub.s f20, f5, f30, f10, rdn\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f18, 864(x15)\n"
			"flw.ps f27, 928(x15)\n"
			"fsgnjx.ps f14, f18, f27\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f4, 0(x25)\n"
			"flw.ps f13, 864(x25)\n"
			"fle.ps f20, f4, f13\n" 
			"la x26, rand_int_32\n"
			"flw.ps f10, 832(x26)\n"
			"flw.ps f18, 928(x26)\n"
			"fmul.pi f20, f10, f18\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f25, 544(x16)\n"
			"frsq.ps f10, f25\n" 
			"la x28, rand_int_32\n"
			"flw.ps f10, 512(x28)\n"
			"fsrli.pi f27, f10, 0x3\n" 
			"li x16, 0xbace2d5e478394a6\n"
			"fcvt.s.w f1, x16, rtz\n" 
			"LBL_SEQID_3_M0_WRITE_ID_19_HID_0:\n"
			"fltm.pi m4, f19, f0\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x15, rand_ieee754_32\n"
			"flw.ps f0, 768(x15)\n"
			"flw.ps f13, 32(x15)\n"
			"fsub.ps f5, f0, f13, rmm\n" 
			"maskpopc x17, m4\n" 
			"li x28, 0x52f3d1e87c42b168\n"
			"fcvt.s.w f1, x28, rmm\n" 
			"la x25, rand_int_32\n"
			"flw.ps f14, 96(x25)\n"
			"flw.ps f7, 64(x25)\n"
			"fltu.pi f0, f14, f7\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f7, 96(x28)\n"
			"flw.ps f25, 992(x28)\n"
			"fadd.s f10, f7, f25, rmm\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f1, 96(x25)\n"
			"flw.ps f26, 704(x25)\n"
			"fle.s x9, f1, f26\n" 
			"la x9, rand_int_32\n"
			"flw.ps f0, 736(x9)\n"
			"fcvt.ps.pw f10, f0, rne\n" 
			"la x17, rand_int_32\n"
			"flw.ps f18, 416(x17)\n"
			"flw.ps f30, 768(x17)\n"
			"fle.pi f18, f18, f30\n" 
			"fbci.ps f8, 0x1707c\n" 
			"la x16, rand_int_32\n"
			"flw.ps f14, 480(x16)\n"
			"fsrli.pi f18, f14, 0x5\n" 
			"LBL_SEQID_3_M0_WRITE_ID_20_HID_0:\n"
			"maskand m4, m2, m5\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x26, rand_ieee754_32\n"
			"flw.ps f10, 448(x26)\n"
			"flw.ps f18, 416(x26)\n"
			"fsub.ps f5, f10, f18, rup\n" 
			"li x5, 4\n" 
			"bne x30, x5, LBL_FAIL_HID_0\n"
			"nop \n"
			"nop \n"
			"addi x29, x29, -1\n"
			"bne x29, x0, LBL_OUTER_LOOP_SEQ_HID_0\n"
			"la x31, check_sum_hid_0\n"
			"lw x5, (x31)\n"
			"bne x30, x5, LBL_FAIL_HID_0\n"
			"LBL_POSTAMBLE_HID_0:\n"          
			"fence\n"
			"csrwi excl_mode, 0\n"
			"j LBL_PASS_HID_0\n"          
			VSNIP_RSV
		);
		__asm__ __volatile__ ( 
			"LBL_FAIL_HID_0:\n"          
			".global LBL_FAIL_HID_0\n"          
			"la x31, check_sum_hid_0\n"
			"li x5, 0xDEADDEADDEADDEAD\n"
			"sd x5, 24(x31)\n"
			"LBL_HPM_3_CORE_TIMA_OPS_FAIL_HID_0:\n"          
			"csrr x5, mhpmcounter3\n"
			"sd x5, 32 (x31)\n"
			"LBL_HPM_4_CORE_TXFMA_OPS16_FAIL_HID_0:\n"          
			"csrr x5, mhpmcounter4\n"
			"sd x5, 40 (x31)\n"
			"LBL_HPM_5_CORE_BRANCHES0_FAIL_HID_0:\n"          
			"csrr x5, mhpmcounter5\n"
			"sd x5, 48 (x31)\n"
			"LBL_HPM_6_CORE_TFMA_WAIT_TENB_FAIL_HID_0:\n"          
			"csrr x5, mhpmcounter6\n"
			"sd x5, 56 (x31)\n"
			"LBL_HPM_7_NEIGH_TX_COOP_TST_REQ_FAIL_HID_0:\n"          
			"csrr x5, mhpmcounter7\n"
			"sd x5, 64 (x31)\n"
			"LBL_HPM_8_NEIGH_TX_NEIGH_COOP_TLD_REQ_FAIL_HID_0:\n"          
			"csrr x5, mhpmcounter8\n"
			"sd x5, 72 (x31)\n"
			"slti x0, x0, 0x7ff\n"
			VSNIP_RSV
		);
		C_TEST_FAIL;
		__asm__ __volatile__ ( 
			"j LBL_HALT_HID_0\n"          
			VSNIP_RSV
		);
		__asm__ __volatile__ ( 
			"LBL_PASS_HID_0:\n"          
			".global LBL_PASS_HID_0\n"          
			"la x31, check_sum_hid_0\n"
			"li x5, 0xACEDACEDACEDACED\n"
			"sd x5, 24(x31)\n"
			"LBL_HPM_3_CORE_TIMA_OPS_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter3\n"
			"sd x5, 32 (x31)\n"
			"LBL_HPM_4_CORE_TXFMA_OPS16_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter4\n"
			"sd x5, 40 (x31)\n"
			"LBL_HPM_5_CORE_BRANCHES0_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter5\n"
			"sd x5, 48 (x31)\n"
			"LBL_HPM_6_CORE_TFMA_WAIT_TENB_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter6\n"
			"sd x5, 56 (x31)\n"
			"LBL_HPM_7_NEIGH_TX_COOP_TST_REQ_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter7\n"
			"sd x5, 64 (x31)\n"
			"LBL_HPM_8_NEIGH_TX_NEIGH_COOP_TLD_REQ_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter8\n"
			"sd x5, 72 (x31)\n"
			"slti x0, x0, 0x7fe\n"
			VSNIP_RSV
		);
		C_TEST_PASS;
		__asm__ __volatile__ ( 
			"LBL_HALT_HID_0:\n"          
			".global LBL_HALT_HID_0\n"          
			"wfi\n"
			"j LBL_HALT_HID_0\n"          
			VSNIP_RSV
		);
		return 0;       
	};
}
volatile uint32_t rand_int_32[256] __attribute__ ((aligned (2048))) = {
		0x5a191afa, 
		0x96dd2433, 
		0x6db34945, 
		0x6531b7af, 
		0x7e89a9c6, 
		0x0da5990a, 
		0x7879b559, 
		0xc30ae95c, 
		0x3f864de5, 
		0x72080f05, 
		0x656d3663, 
		0x87380285, 
		0xd08d98f2, 
		0x3e49472e, 
		0xf2f125eb, 
		0x9db52d79, 
		0x118166d7, 
		0xb50bdc38, 
		0x2fb53743, 
		0x07d38e18, 
		0x073bdfcf, 
		0x357a6d03, 
		0x3c116737, 
		0x12019ad5, 
		0xd3afa48d, 
		0x17473d8d, 
		0x703349bc, 
		0xb25a7abd, 
		0x0c312010, 
		0x4392e61d, 
		0x135182ee, 
		0x0094ddfd, 
		0xc4e824fb, 
		0x14749f1f, 
		0xe8b144b9, 
		0x1c7d4cb8, 
		0xbbcad543, 
		0x3901cde9, 
		0x11e29a5c, 
		0xdbc55250, 
		0x5c19b43b, 
		0x48730adf, 
		0x779c168d, 
		0x97643a31, 
		0x7004f8ae, 
		0x9c9d828c, 
		0x13a76d0a, 
		0xe00c81cb, 
		0x297afa22, 
		0xbe66c40d, 
		0x82a990ec, 
		0xd25d5046, 
		0x58253aae, 
		0x75baa9bc, 
		0xc0b5d7f2, 
		0x15bb2b9b, 
		0xaebf9665, 
		0xde70f90c, 
		0xf0eb3a17, 
		0x46dbbfae, 
		0x051edd7b, 
		0x27fbf65a, 
		0x3d4bdb81, 
		0x4df5aee1, 
		0x3881724e, 
		0xb90996e9, 
		0x2758aeb6, 
		0x010b1bdc, 
		0x1210dc52, 
		0xfddb1cf4, 
		0x7dba24dd, 
		0x891baaeb, 
		0xd966fa76, 
		0x980ca87c, 
		0xdd793f43, 
		0x9a31d11b, 
		0xd9019862, 
		0x9ca56578, 
		0x07ea56ea, 
		0x24e45201, 
		0x86f4ef73, 
		0xa108037c, 
		0x5cc2ea79, 
		0x1c34f56a, 
		0xf33a2dc2, 
		0xefd9d013, 
		0xdb688232, 
		0xea0b9be3, 
		0xaf9f00c4, 
		0xd24f29d5, 
		0xd39d29fb, 
		0x65987349, 
		0x20b5fb1d, 
		0x22658cfd, 
		0x0601573d, 
		0x109fd21a, 
		0xd3e222e2, 
		0x16cdd126, 
		0x452ddfd1, 
		0xbead04b0, 
		0x0f3cba9b, 
		0xb7a71f0b, 
		0x390a2388, 
		0x606534c6, 
		0xd202ff40, 
		0x6bd38924, 
		0xf461e6b6, 
		0x499fbb6f, 
		0xebb1c891, 
		0x8455f362, 
		0xda1ae3c2, 
		0xc41b1c06, 
		0xcf88b5a8, 
		0x80f289bf, 
		0x13c49ae1, 
		0x18338a2e, 
		0xc024eea3, 
		0x320050d1, 
		0x6404ba7c, 
		0xcba82f9e, 
		0x71eb7a4a, 
		0x9feef00e, 
		0x5899b988, 
		0x71a8ea8f, 
		0xfaf7002b, 
		0x1dc19879, 
		0xd7fe4742, 
		0x126cba7b, 
		0xd303bf86, 
		0x1570f6fb, 
		0x019ce9d8, 
		0x9ed83402, 
		0xd54886c5, 
		0x710bb155, 
		0x1836180c, 
		0xeadb07d5, 
		0x2518ca0d, 
		0x94d1eef1, 
		0x86b0a999, 
		0x07d49b35, 
		0x3b954e53, 
		0xc0aca946, 
		0xcbc5630e, 
		0x1ab13a5e, 
		0xb0d7bc47, 
		0xbc4f4c4a, 
		0x0109bac7, 
		0x72d33074, 
		0x179c0d17, 
		0xa4de2783, 
		0x7dd7dbbe, 
		0xff5f0d8b, 
		0xae31e432, 
		0x68cf40f0, 
		0x1ee29534, 
		0x23b2f20a, 
		0x0a73a749, 
		0x2bb23d1e, 
		0xb663fc75, 
		0xed6147cd, 
		0x7ccf80ad, 
		0xe9bf4db4, 
		0x9e995b97, 
		0x75d3f43d, 
		0xd6521a72, 
		0x6a2cd294, 
		0x39097a19, 
		0xdfe52c5d, 
		0x28e3ad55, 
		0x23c787cb, 
		0x8fd8ede6, 
		0x18ce5ad0, 
		0x06a6ccea, 
		0x4c58202f, 
		0x546b1756, 
		0xef64ba09, 
		0xb55ea5c6, 
		0x553b7c71, 
		0xa0ce8d0d, 
		0xacab62b7, 
		0xd7267d0d, 
		0x30891713, 
		0xc193bd51, 
		0xae615323, 
		0x86d29773, 
		0x25252fb8, 
		0x0ad8fe47, 
		0xde3d5cbb, 
		0x00d57c64, 
		0x65a78f26, 
		0xa8a67f91, 
		0xc43424dd, 
		0x636a1a6d, 
		0xebf935b9, 
		0x0e5f2371, 
		0xb17ec5ba, 
		0xe7e529a7, 
		0x3d132380, 
		0x83c55cdf, 
		0x48caee57, 
		0xe5e2c8fa, 
		0x7a7ca5f4, 
		0xb01908fb, 
		0x497677c1, 
		0x72f29ba9, 
		0x67159722, 
		0xfe361f58, 
		0x2dd23198, 
		0x98c65335, 
		0xbe37e486, 
		0xa3672351, 
		0x38c9d2a1, 
		0x64a398a1, 
		0x01095014, 
		0x717ba843, 
		0xe6e889e6, 
		0x5ab00564, 
		0x4464b5b0, 
		0xf5530480, 
		0x0ec429d7, 
		0x50025baf, 
		0x6b40b30f, 
		0xf11e59e0, 
		0x64f9421c, 
		0x2c33065e, 
		0xb9e2d0f6, 
		0x4cb02e1f, 
		0x6fa39a1a, 
		0xff5ef1ef, 
		0xf753f43b, 
		0xbe135af0, 
		0x5e4db74b, 
		0xae59bbb3, 
		0x3630e769, 
		0x745ffd1b, 
		0x7e4ed102, 
		0x817306f6, 
		0x5612ff8d, 
		0xd9515456, 
		0x530619c3, 
		0xe8346e52, 
		0xacdb4d36, 
		0x53e01ba9, 
		0xb21ab5f9, 
		0xf42145aa, 
		0x6ebc2123, 
		0x5e6bc0df, 
		0x9547c858, 
		0x01a9bb14, 
		0x4065b045, 
		0x92d78c1c, 
		0x7b143a7e, 
		0xe8dcb203, 
		0x902d9048, 
		0xdcb79b04, 
		0x19c519ab  
};
volatile uint16_t rand_ieee754_16[1536] __attribute__ ((aligned (2048))) = {
		0xc800, 
		0x5040, 
		0xd3a0, 
		0x0000, 
		0xf59d, 
		0xd393, 
		0x19a7, 
		0xfc00, 
		0xd260, 
		0xa1e6, 
		0x5120, 
		0xb97f, 
		0x53e0, 
		0x4f00, 
		0x51c0, 
		0xd200, 
		0x79bb, 
		0x5100, 
		0x0000, 
		0xccc0, 
		0x5100, 
		0xca37, 
		0xd0c0, 
		0x5100, 
		0x52c0, 
		0xf910, 
		0x5300, 
		0x1616, 
		0x8000, 
		0x52e0, 
		0xd1c0, 
		0x4400, 
		0x4d40, 
		0xd1c0, 
		0x4c80, 
		0x4900, 
		0xd240, 
		0x7f55, 
		0x4980, 
		0xb50d, 
		0x559d, 
		0x6343, 
		0xcc00, 
		0x5a93, 
		0x5000, 
		0x5180, 
		0x7c00, 
		0x4c80, 
		0xc200, 
		0xd040, 
		0x5180, 
		0x6f94, 
		0xd2a0, 
		0xd220, 
		0xc400, 
		0x5180, 
		0x4dc0, 
		0x3c00, 
		0x062f, 
		0x8b9b, 
		0x9320, 
		0x5120, 
		0x4e69, 
		0xd3c0, 
		0x709f, 
		0xc880, 
		0x5100, 
		0xcd40, 
		0x7f04, 
		0x5985, 
		0xbd08, 
		0xd2e0, 
		0x944b, 
		0x4200, 
		0x7c00, 
		0xd020, 
		0x4e80, 
		0x5060, 
		0x5320, 
		0x4cc0, 
		0xede2, 
		0x4dc0, 
		0x977f, 
		0xce70, 
		0x8000, 
		0x8000, 
		0xc500, 
		0x7f19, 
		0x5300, 
		0xf0a6, 
		0xd080, 
		0xc400, 
		0x339f, 
		0x5160, 
		0x4e00, 
		0x6eff, 
		0xbc00, 
		0x4a80, 
		0xc800, 
		0x50e0, 
		0x2015, 
		0x7c00, 
		0xd280, 
		0xbc00, 
		0x9e06, 
		0x3246, 
		0x4c80, 
		0xb388, 
		0x4400, 
		0x5220, 
		0xd1a0, 
		0xcfc0, 
		0x77ec, 
		0xcc40, 
		0xa358, 
		0x6bc0, 
		0x4500, 
		0xec40, 
		0x5060, 
		0x4e00, 
		0x5360, 
		0xc600, 
		0x5200, 
		0xcf80, 
		0x4b80, 
		0x284d, 
		0x4d40, 
		0x6176, 
		0x4d00, 
		0xd0c0, 
		0x8000, 
		0xcc80, 
		0xcd40, 
		0xb1e9, 
		0xd060, 
		0x7266, 
		0x01d1, 
		0x4500, 
		0xcc80, 
		0x4b00, 
		0xd240, 
		0xd080, 
		0xcd40, 
		0x5320, 
		0x4fe7, 
		0xd040, 
		0xcec0, 
		0x4cc0, 
		0x52a0, 
		0x5340, 
		0x5200, 
		0x5000, 
		0xd3c0, 
		0x0269, 
		0x4cc0, 
		0x4400, 
		0xc700, 
		0xd9fa, 
		0xcd80, 
		0x8b39, 
		0x53a0, 
		0x5240, 
		0x4e00, 
		0x4b00, 
		0x5300, 
		0xd140, 
		0xbc00, 
		0x36e7, 
		0x4c80, 
		0x51e0, 
		0xe9f3, 
		0x4c40, 
		0x5bd5, 
		0xce5a, 
		0x7c50, 
		0xce40, 
		0x5380, 
		0x302d, 
		0x5622, 
		0x0033, 
		0x5132, 
		0xfc00, 
		0xce40, 
		0xfc4d, 
		0x8000, 
		0xcc40, 
		0xd360, 
		0x5160, 
		0x2504, 
		0x4dc0, 
		0xd340, 
		0x8bb4, 
		0xd100, 
		0x7c00, 
		0x50e0, 
		0x51c0, 
		0x4e00, 
		0xcdc0, 
		0xfc00, 
		0x5000, 
		0x4400, 
		0xc980, 
		0x8000, 
		0x7c00, 
		0x6848, 
		0x5bbf, 
		0x4d40, 
		0xc980, 
		0xd0a0, 
		0x50a0, 
		0x5080, 
		0x4d80, 
		0xd320, 
		0xd180, 
		0x7004, 
		0x4a80, 
		0xc600, 
		0xd040, 
		0x7c00, 
		0x0e79, 
		0x5320, 
		0x4dc0, 
		0x1b6d, 
		0x4e80, 
		0xd1a0, 
		0xd320, 
		0xfc00, 
		0x11cd, 
		0xd280, 
		0xca80, 
		0xd360, 
		0x8ae3, 
		0x5251, 
		0xd1e0, 
		0x4400, 
		0x4700, 
		0xc500, 
		0xd2a0, 
		0xcb00, 
		0xd160, 
		0x52c0, 
		0xcd00, 
		0xfc00, 
		0x0000, 
		0x4f40, 
		0x53c0, 
		0x5320, 
		0xd3e0, 
		0xbc00, 
		0x011c, 
		0xcec0, 
		0xd160, 
		0xf5c4, 
		0xfdf5, 
		0xca80, 
		0xcfc0, 
		0xd2c0, 
		0x4f00, 
		0x4600, 
		0x5220, 
		0x4e80, 
		0xfd5a, 
		0x3866, 
		0xce00, 
		0xd120, 
		0xca00, 
		0xcb00, 
		0xd2a0, 
		0xd1c0, 
		0xfd95, 
		0x81ce, 
		0x51c0, 
		0x50a0, 
		0xa24b, 
		0xd080, 
		0x0000, 
		0x5100, 
		0x5200, 
		0xd210, 
		0xcd00, 
		0x4c40, 
		0x8217, 
		0x52a0, 
		0xea3f, 
		0x6ffa, 
		0x4b80, 
		0x5140, 
		0xce00, 
		0x4fc0, 
		0x8e5a, 
		0xbc00, 
		0xccc0, 
		0x51a0, 
		0xd020, 
		0x62d0, 
		0x51c0, 
		0x8000, 
		0xd0a0, 
		0x69ea, 
		0xd280, 
		0xf146, 
		0x79c0, 
		0x4dc0, 
		0x4f40, 
		0xce80, 
		0x3c00, 
		0x9094, 
		0xcf80, 
		0xd3e0, 
		0xbc3c, 
		0x7c00, 
		0x4b80, 
		0xcc00, 
		0xcec0, 
		0x7a42, 
		0x53a0, 
		0x1786, 
		0x50e0, 
		0xd100, 
		0xd060, 
		0xd320, 
		0x6177, 
		0xac12, 
		0x51c0, 
		0x3d9e, 
		0x5140, 
		0x6127, 
		0x51c0, 
		0xc800, 
		0xcf00, 
		0x6285, 
		0xda61, 
		0x1f9c, 
		0x5180, 
		0x4ec0, 
		0x8662, 
		0x4000, 
		0x8b31, 
		0xc600, 
		0x7df9, 
		0x51c0, 
		0x5260, 
		0xcd40, 
		0x5180, 
		0xd140, 
		0xd000, 
		0xd2e0, 
		0x6b03, 
		0xfc00, 
		0x795c, 
		0xd340, 
		0x4a80, 
		0x4a00, 
		0x5020, 
		0xd1e0, 
		0xa669, 
		0x8000, 
		0x0000, 
		0xfb4a, 
		0x768f, 
		0x5200, 
		0xca00, 
		0xd2a0, 
		0x0000, 
		0x5180, 
		0xdc6f, 
		0x5e2d, 
		0x0000, 
		0x0000, 
		0x4d40, 
		0x0000, 
		0x53c0, 
		0x5160, 
		0xfadc, 
		0xd280, 
		0x4900, 
		0xd340, 
		0xd020, 
		0x2986, 
		0x119d, 
		0x0700, 
		0xfb27, 
		0x47b2, 
		0x5260, 
		0xce80, 
		0xc800, 
		0x4f00, 
		0xc900, 
		0x1de9, 
		0x10d9, 
		0xd3a0, 
		0xd320, 
		0x4980, 
		0x50e0, 
		0xc980, 
		0xe436, 
		0xc400, 
		0x7c00, 
		0x5060, 
		0x4800, 
		0xcf00, 
		0xcd00, 
		0xd3e0, 
		0x3c00, 
		0x49c7, 
		0x6980, 
		0x50a0, 
		0x0000, 
		0xce00, 
		0xc000, 
		0x5040, 
		0xfc00, 
		0x4111, 
		0xcd00, 
		0xd33d, 
		0x7722, 
		0x4000, 
		0x5180, 
		0x1895, 
		0xc880, 
		0x4700, 
		0x4fc0, 
		0x51c0, 
		0xcc80, 
		0x4f80, 
		0x152c, 
		0xd060, 
		0x5220, 
		0xd160, 
		0x05c6, 
		0x52e0, 
		0xcc00, 
		0xd3a0, 
		0xcf95, 
		0xcc80, 
		0xc700, 
		0xd2e0, 
		0xce40, 
		0x4d40, 
		0xd3a0, 
		0x9753, 
		0x4900, 
		0x812a, 
		0x3508, 
		0xd080, 
		0xd2a0, 
		0xc500, 
		0x3c00, 
		0xc500, 
		0x23f0, 
		0xfc00, 
		0x0000, 
		0x7ce5, 
		0x4a1b, 
		0xfc00, 
		0x79f4, 
		0xcf80, 
		0xcec0, 
		0xc900, 
		0x52c0, 
		0xe245, 
		0x4e00, 
		0xcd80, 
		0x4a80, 
		0xd0c0, 
		0x4b80, 
		0xbc00, 
		0xbc00, 
		0x4c80, 
		0x8000, 
		0xd240, 
		0xb0c0, 
		0x4e80, 
		0xca80, 
		0x6ab0, 
		0x4600, 
		0xd3c0, 
		0x8799, 
		0x5100, 
		0x1051, 
		0x5200, 
		0x4800, 
		0x4d40, 
		0x4400, 
		0xd903, 
		0x797c, 
		0xd300, 
		0xd300, 
		0xcec0, 
		0x4900, 
		0xcc40, 
		0xd080, 
		0x8000, 
		0xd3a0, 
		0x4980, 
		0x2186, 
		0xcfc0, 
		0x52a0, 
		0xd0a0, 
		0xbc09, 
		0x50e0, 
		0x4800, 
		0x4fc0, 
		0xfcf2, 
		0xc3fa, 
		0xd100, 
		0x090f, 
		0xd0c0, 
		0xcefe, 
		0x5300, 
		0xf331, 
		0x4e00, 
		0x5280, 
		0x4a00, 
		0x4d40, 
		0x0000, 
		0x464f, 
		0x5100, 
		0xcdc0, 
		0x7b73, 
		0x69c0, 
		0xc400, 
		0xcec0, 
		0xa71b, 
		0xcd00, 
		0xd120, 
		0xd2a0, 
		0xce80, 
		0x1ce4, 
		0x7c00, 
		0x68f4, 
		0x4f00, 
		0x5120, 
		0xeaf0, 
		0xd040, 
		0x5260, 
		0x5398, 
		0x53c0, 
		0xc857, 
		0xdb1f, 
		0x5040, 
		0xd180, 
		0x8000, 
		0xd220, 
		0x4b80, 
		0xd1a0, 
		0xd020, 
		0xfd0d, 
		0xd120, 
		0x8000, 
		0xd240, 
		0xcb00, 
		0xd220, 
		0xd040, 
		0xc900, 
		0x38f1, 
		0x4900, 
		0x0b88, 
		0xc986, 
		0xcd00, 
		0x723b, 
		0xfc00, 
		0xce00, 
		0x3c00, 
		0x28dd, 
		0x5000, 
		0xecc2, 
		0x4980, 
		0xeeed, 
		0x4dc0, 
		0x5180, 
		0x8f6f, 
		0x8000, 
		0xd0e0, 
		0xcb00, 
		0xce40, 
		0x2711, 
		0xd100, 
		0x9e8c, 
		0x4f77, 
		0xde8f, 
		0xd1e0, 
		0x7c00, 
		0x1712, 
		0x2b04, 
		0x50a0, 
		0x03bc, 
		0x5340, 
		0x8750, 
		0x4b80, 
		0x4b80, 
		0x151c, 
		0xd3c0, 
		0xcf40, 
		0x53a0, 
		0xd020, 
		0x3c00, 
		0xd1e0, 
		0x7c09, 
		0x0ca3, 
		0x4200, 
		0x5140, 
		0x4e80, 
		0x884b, 
		0xfa57, 
		0x0000, 
		0xdb1d, 
		0x580d, 
		0xccc0, 
		0x4a00, 
		0xb1d4, 
		0xc980, 
		0xc980, 
		0xc400, 
		0x4e40, 
		0x6be0, 
		0x8000, 
		0x0000, 
		0x43f1, 
		0xd2a0, 
		0xcd80, 
		0x902b, 
		0xcc00, 
		0x5060, 
		0x4c80, 
		0x810f, 
		0xcec0, 
		0x8000, 
		0x4b00, 
		0x5340, 
		0xce00, 
		0xfc00, 
		0x512d, 
		0xd040, 
		0xebaa, 
		0x4e00, 
		0xd240, 
		0xd180, 
		0xd1e0, 
		0x52a0, 
		0x53a0, 
		0x1e79, 
		0xa235, 
		0xc000, 
		0xd340, 
		0x52a0, 
		0xa0d9, 
		0x3395, 
		0x4980, 
		0x51a0, 
		0x53a0, 
		0xd180, 
		0x50c0, 
		0xef4b, 
		0x4ebc, 
		0xd320, 
		0x5240, 
		0xd1c0, 
		0xfb84, 
		0x51a0, 
		0xd0c0, 
		0x2f0a, 
		0x12b4, 
		0xd000, 
		0xd380, 
		0x8000, 
		0xd060, 
		0xfc72, 
		0x8122, 
		0x48df, 
		0x9a02, 
		0x6b53, 
		0xd240, 
		0x0000, 
		0xf05a, 
		0x93a0, 
		0x3de3, 
		0xd160, 
		0xc52c, 
		0xfc00, 
		0xfcb8, 
		0x5140, 
		0xbc00, 
		0xc600, 
		0xc400, 
		0xd0e0, 
		0x4fc0, 
		0x12f5, 
		0x3cc9, 
		0xfc52, 
		0xe502, 
		0xc200, 
		0xcf00, 
		0xc900, 
		0xb947, 
		0x53c0, 
		0xfc00, 
		0x4880, 
		0x4e80, 
		0xd220, 
		0x288c, 
		0x5db0, 
		0xd000, 
		0x9029, 
		0x4a80, 
		0x0d90, 
		0x4e40, 
		0x4400, 
		0xcc40, 
		0x5160, 
		0xfc82, 
		0xd140, 
		0x8000, 
		0xcdc0, 
		0xd080, 
		0x0990, 
		0x4af3, 
		0xd3e0, 
		0x5200, 
		0x5360, 
		0xd1a0, 
		0x153f, 
		0x7ddb, 
		0x163e, 
		0x5160, 
		0xcc40, 
		0xd360, 
		0x250c, 
		0x1aa4, 
		0x2e45, 
		0x5040, 
		0x5240, 
		0xcc00, 
		0xd180, 
		0x7989, 
		0x5320, 
		0xc600, 
		0xd1c0, 
		0xd160, 
		0xc800, 
		0x0000, 
		0x4600, 
		0x51e0, 
		0x8000, 
		0xd000, 
		0x0000, 
		0xbc00, 
		0x32b6, 
		0x211a, 
		0x5160, 
		0x4400, 
		0x4d00, 
		0x4b80, 
		0xb582, 
		0x1584, 
		0xeb7f, 
		0x4f00, 
		0x9eee, 
		0xd000, 
		0xd280, 
		0xd3a0, 
		0x4e40, 
		0xd8a8, 
		0xd040, 
		0xcd00, 
		0x0efc, 
		0x96be, 
		0x52c0, 
		0xc980, 
		0x6957, 
		0x52c0, 
		0x4ec0, 
		0x5d51, 
		0xd180, 
		0x52e0, 
		0xfcb4, 
		0x147c, 
		0x5100, 
		0x7bfa, 
		0xf84b, 
		0x5360, 
		0x95db, 
		0xd2e0, 
		0x50e0, 
		0xd220, 
		0xd040, 
		0xdb7a, 
		0xccc0, 
		0x5300, 
		0xd3c0, 
		0x4880, 
		0xe1d5, 
		0xfcbb, 
		0x4fc0, 
		0x4fc0, 
		0x8000, 
		0xce40, 
		0x50a0, 
		0xcd40, 
		0xcf00, 
		0x5260, 
		0xbec5, 
		0xc980, 
		0xd6fe, 
		0xcd30, 
		0x4c40, 
		0x2439, 
		0x50c0, 
		0xcec0, 
		0x4fc0, 
		0x4c40, 
		0x51a0, 
		0xcb00, 
		0xd8cb, 
		0x53e0, 
		0x3067, 
		0x5180, 
		0xe2dc, 
		0xf258, 
		0x52f3, 
		0x7787, 
		0x5040, 
		0x72c2, 
		0xc900, 
		0x5360, 
		0x4a80, 
		0xc200, 
		0xa5bb, 
		0xcf00, 
		0xab55, 
		0x9fae, 
		0x5180, 
		0x47e4, 
		0xcec0, 
		0xce40, 
		0x5ac5, 
		0x3be1, 
		0xbdbd, 
		0xe567, 
		0x50a0, 
		0xc200, 
		0xcd00, 
		0xd0c0, 
		0xd060, 
		0x9e1c, 
		0xea18, 
		0xcf00, 
		0x7c00, 
		0xbcbc, 
		0x2dee, 
		0x5280, 
		0x8000, 
		0x3c00, 
		0x4c57, 
		0x84b3, 
		0xd280, 
		0x1f9d, 
		0xc880, 
		0x0000, 
		0xd160, 
		0x870f, 
		0x8bc6, 
		0x505d, 
		0xd320, 
		0x27c6, 
		0x52a0, 
		0x5220, 
		0xc261, 
		0x9545, 
		0x4f80, 
		0x5220, 
		0xcd40, 
		0x95c5, 
		0xd2a0, 
		0xd340, 
		0x5360, 
		0x0000, 
		0x4b80, 
		0xfd3c, 
		0x0000, 
		0xea2c, 
		0x4d81, 
		0x5160, 
		0xcc40, 
		0xd280, 
		0x273d, 
		0xc400, 
		0xcec0, 
		0x4500, 
		0x7c00, 
		0xd120, 
		0x8982, 
		0x0000, 
		0xd2c0, 
		0xfc00, 
		0xd3a0, 
		0xc600, 
		0xc3f9, 
		0x5100, 
		0xf4ed, 
		0xff34, 
		0xf737, 
		0xdced, 
		0xcdc0, 
		0x0000, 
		0xd0a0, 
		0x0000, 
		0xef19, 
		0xd5aa, 
		0x4980, 
		0x9cee, 
		0x5180, 
		0xccc0, 
		0xce80, 
		0xc200, 
		0x4200, 
		0x50a0, 
		0x7c00, 
		0xb6d9, 
		0xbc45, 
		0xc3ab, 
		0x3c00, 
		0xd220, 
		0x6250, 
		0xa0e0, 
		0xd0c0, 
		0xcb80, 
		0x5daf, 
		0xda54, 
		0x0000, 
		0xce00, 
		0x677d, 
		0xc900, 
		0x14ff, 
		0x4900, 
		0x5120, 
		0xd30f, 
		0x4500, 
		0x0000, 
		0x7e83, 
		0x8000, 
		0x4b00, 
		0x4700, 
		0xa514, 
		0x7c00, 
		0x21f8, 
		0x398e, 
		0x4d40, 
		0xc247, 
		0xd1e0, 
		0xcfc0, 
		0xd599, 
		0xd0c0, 
		0xd000, 
		0x4400, 
		0xc980, 
		0xaa1f, 
		0x5120, 
		0x52e0, 
		0x50a0, 
		0x39c5, 
		0xd320, 
		0x4700, 
		0x5060, 
		0xd060, 
		0x7c00, 
		0xd5fd, 
		0xc1b1, 
		0x4d00, 
		0x81a7, 
		0xd1e0, 
		0xd140, 
		0x5280, 
		0x4b80, 
		0x5040, 
		0x53c0, 
		0x5100, 
		0xeef1, 
		0x5100, 
		0xd0a0, 
		0x5340, 
		0xf189, 
		0x4f80, 
		0x51c0, 
		0xeaba, 
		0xbc00, 
		0x96b9, 
		0x2bab, 
		0xb695, 
		0x9814, 
		0xd380, 
		0x0000, 
		0x4cc0, 
		0xdb2d, 
		0x5340, 
		0xbc00, 
		0xce00, 
		0x4800, 
		0xd3e0, 
		0xa24a, 
		0x9467, 
		0x8000, 
		0x4d00, 
		0xcfc0, 
		0x0000, 
		0x2249, 
		0xbd30, 
		0x4b5f, 
		0xfde9, 
		0x9cdf, 
		0x4c00, 
		0xc800, 
		0xe046, 
		0xd040, 
		0xcdc0, 
		0xd160, 
		0x8000, 
		0x4e00, 
		0x4979, 
		0xc800, 
		0x00b3, 
		0x0e70, 
		0xd280, 
		0x52e0, 
		0x1bce, 
		0x51e0, 
		0xc400, 
		0xbc00, 
		0xcfc0, 
		0x2e1e, 
		0x5280, 
		0x4880, 
		0x86c4, 
		0x2c2e, 
		0x5080, 
		0xc200, 
		0x52c0, 
		0x50e0, 
		0xe413, 
		0xac22, 
		0xd060, 
		0x53e0, 
		0xd340, 
		0x5060, 
		0x5020, 
		0x5360, 
		0x53a0, 
		0xd31b, 
		0xd0a0, 
		0x4e80, 
		0x4ca1, 
		0x5060, 
		0xd0e0, 
		0x5380, 
		0x5280, 
		0x5140, 
		0xd060, 
		0x2ee0, 
		0x50e0, 
		0x2c42, 
		0xcf80, 
		0x76c1, 
		0x7df9, 
		0x4700, 
		0xfc38, 
		0xcd00, 
		0xd0a0, 
		0x2f2d, 
		0xcb80, 
		0xcd80, 
		0x8000, 
		0xcdc0, 
		0xcec0, 
		0xcec0, 
		0x39b3, 
		0x52e0, 
		0x50a0, 
		0xd1a0, 
		0xd280, 
		0x8000, 
		0x53c0, 
		0x4cc0, 
		0xcf80, 
		0xbc00, 
		0x7d0f, 
		0x52c0, 
		0xb945, 
		0xd0a0, 
		0xd1a0, 
		0x4c80, 
		0xce00, 
		0x0000, 
		0x9987, 
		0x4d40, 
		0x3467, 
		0x8000, 
		0x4c40, 
		0xdfe3, 
		0x7d3a, 
		0xc800, 
		0xc500, 
		0x8c3d, 
		0xd2e0, 
		0x4200, 
		0xcc40, 
		0xd3c0, 
		0x09b4, 
		0x4b00, 
		0x4d40, 
		0xd380, 
		0x1ef0, 
		0xce00, 
		0x5380, 
		0xd3e0, 
		0xc200, 
		0x4980, 
		0x1b18, 
		0xca80, 
		0xcc80, 
		0x0000, 
		0xfc00, 
		0xeda8, 
		0x0000, 
		0x5240, 
		0x53c0, 
		0x8000, 
		0x4a00, 
		0xce00, 
		0xd200, 
		0xacc2, 
		0x5340, 
		0xcdd6, 
		0x8000, 
		0xfde7, 
		0x52a0, 
		0x7c00, 
		0x53e0, 
		0xea0e, 
		0xd340, 
		0x4f40, 
		0x84b2, 
		0x75b0, 
		0xd0c0, 
		0xd360, 
		0xca80, 
		0xfc8d, 
		0xd0a0, 
		0x62d0, 
		0xb511, 
		0x5320, 
		0x5140, 
		0xbe20, 
		0xcc40, 
		0x5180, 
		0x0000, 
		0x5060, 
		0x9101, 
		0x51e0, 
		0xc800, 
		0x6a31, 
		0x7b9f, 
		0x4980, 
		0x4f80, 
		0x4e4a, 
		0x0da1, 
		0x4d80, 
		0x5080, 
		0x4d40, 
		0x53e0, 
		0x5160, 
		0x5280, 
		0xd220, 
		0x78b4, 
		0x52a0, 
		0x52e0, 
		0xcd40, 
		0xca03, 
		0xbd29, 
		0xd3a0, 
		0xc880, 
		0xc900, 
		0x4200, 
		0x52c0, 
		0x4000, 
		0x8348, 
		0x266b, 
		0x14b3, 
		0xd000, 
		0x5380, 
		0x50e0, 
		0x0000, 
		0xc800, 
		0xd080, 
		0xd200, 
		0xcc80, 
		0xd040, 
		0x51c0, 
		0x7c00, 
		0x6bda, 
		0xc200, 
		0x0fc9, 
		0x8000, 
		0x4c80, 
		0x8000, 
		0xcc00, 
		0x5360, 
		0xd380, 
		0xd0a0, 
		0xb009, 
		0xd2c0, 
		0xd120, 
		0x5340, 
		0x0f13, 
		0x8000, 
		0x4a00, 
		0x5380, 
		0x5160, 
		0x4be6, 
		0xd340, 
		0xd140, 
		0x4980, 
		0x51a0, 
		0x0000, 
		0xd020, 
		0x5280, 
		0x0000, 
		0xd360, 
		0x0000, 
		0xcf40, 
		0x8000, 
		0x6afa, 
		0x8000, 
		0xc700, 
		0xca80, 
		0xd300, 
		0x6960, 
		0xc9a6, 
		0xd120, 
		0x53e0, 
		0x4d40, 
		0xd0a0, 
		0xd200, 
		0xd120, 
		0x2ebd, 
		0x5d2f, 
		0xfc00, 
		0x4d00, 
		0xdefd, 
		0x6555, 
		0xfc00, 
		0x5140, 
		0xc700, 
		0xd100, 
		0xf050, 
		0x51c0, 
		0x3241, 
		0xb1fe, 
		0x50c0, 
		0x95ed, 
		0x5120, 
		0xd000, 
		0xc900, 
		0xc494, 
		0x5320, 
		0xcb00, 
		0x4f00, 
		0x6369, 
		0x0000, 
		0x5360, 
		0xcc00, 
		0xc5e4, 
		0x0000, 
		0xa119, 
		0x4980, 
		0x4900, 
		0xca80, 
		0xd260, 
		0x0000, 
		0xcc00, 
		0x9a74, 
		0x50e0, 
		0x52a0, 
		0xd0e0, 
		0x0177, 
		0xcf00, 
		0x8000, 
		0x52e0, 
		0xd080, 
		0x50c0, 
		0xeb2c, 
		0x80ec, 
		0x90ad, 
		0x4c80, 
		0x5878, 
		0x53e0, 
		0x516e, 
		0x4500, 
		0x8000, 
		0xcf80, 
		0xdcbe, 
		0x4a00, 
		0xe42b, 
		0xfb87, 
		0x0d3e, 
		0x14e3, 
		0xdf6d, 
		0x4200, 
		0x5220, 
		0xc900, 
		0x5020, 
		0x51e0, 
		0x4980, 
		0x1211, 
		0x261e, 
		0x5160, 
		0x4a00, 
		0xf7a1, 
		0x6c7e, 
		0xcd40, 
		0x0000, 
		0xc600, 
		0x51a0, 
		0x475d, 
		0x3c00, 
		0x4500, 
		0x4d00, 
		0x4800, 
		0xd240, 
		0xd260, 
		0x0000, 
		0xe0fd, 
		0x61e4, 
		0x2f6f, 
		0x7c01, 
		0x8000, 
		0x8355, 
		0xf743, 
		0x4600, 
		0x8000, 
		0x04fe, 
		0xcec0, 
		0x1d1e, 
		0x4b00, 
		0x5380, 
		0x75ee, 
		0x4d80, 
		0x5180, 
		0x4d40, 
		0x8000, 
		0x5360, 
		0x4dc0, 
		0xbec5, 
		0x7c03, 
		0x8000, 
		0x112d, 
		0x7765, 
		0xd2e0, 
		0x4b80, 
		0xcf00, 
		0x4d80, 
		0x3a9f, 
		0x58ef, 
		0x50a0, 
		0x52a0, 
		0x8000, 
		0x8919, 
		0xcf80, 
		0x5380, 
		0x5280, 
		0x50e0, 
		0x8e75, 
		0x5220, 
		0x4880, 
		0x3d70, 
		0xcd40, 
		0xd3e0, 
		0xbc00, 
		0xd000, 
		0xcd80, 
		0xbd6a, 
		0x52c0, 
		0x0000, 
		0x7dba, 
		0xd300, 
		0xabce, 
		0x6e98, 
		0xc82c, 
		0x4e00, 
		0x57df, 
		0x3aab, 
		0x53e0, 
		0xd100, 
		0x4f00, 
		0x4d40, 
		0x50e0, 
		0xcfc0, 
		0x3c3b, 
		0xcf80, 
		0xf8d7, 
		0xcf80, 
		0xbaaa, 
		0xf1f4, 
		0xcd40, 
		0x50a0, 
		0xca00, 
		0xd605, 
		0xce80, 
		0xcd00, 
		0x4b80, 
		0x7c00, 
		0x651b, 
		0x5240, 
		0x4d40, 
		0x945a, 
		0xcd40, 
		0x7c42, 
		0x5a50, 
		0x5040, 
		0x4e40, 
		0x011c, 
		0x84c3, 
		0x4400, 
		0xd0e0, 
		0xc500, 
		0xd360, 
		0xce00, 
		0x8814, 
		0x53a0, 
		0xc56b, 
		0xe4da, 
		0x69b2, 
		0xc980, 
		0xcec0, 
		0x8a6f, 
		0x8b26, 
		0x5320, 
		0xcc80, 
		0x89bc, 
		0xc000, 
		0x78d2, 
		0x8000, 
		0xd040, 
		0xfc39, 
		0xe209, 
		0xc200, 
		0x5200, 
		0xf9da, 
		0xd180, 
		0x4cc0, 
		0x7c68, 
		0x8000, 
		0x1d78, 
		0x4a00, 
		0x8000, 
		0x8000, 
		0x522a, 
		0xd040, 
		0x4b00, 
		0x5300, 
		0xc900, 
		0x4400, 
		0x7c00, 
		0x4700, 
		0x8137, 
		0x2933, 
		0xd300, 
		0xd2c0, 
		0x5380, 
		0x4200, 
		0x4980, 
		0xd160, 
		0xd080, 
		0x38d3, 
		0x211a, 
		0x5983, 
		0x6782, 
		0x51e0, 
		0x20c0, 
		0xd300, 
		0xf2bb, 
		0xd120, 
		0x4880, 
		0xccc0, 
		0x8000, 
		0xd320, 
		0xe228, 
		0x4d80, 
		0x4f40, 
		0xd1c0, 
		0xd200, 
		0xd0e0, 
		0xcf44, 
		0xd040, 
		0xc980, 
		0xd080, 
		0x2965, 
		0x4400, 
		0xd3c0, 
		0x4f80, 
		0x8c89, 
		0x5080, 
		0x8000, 
		0xcf80, 
		0x4b00, 
		0xa0be, 
		0x53a0, 
		0x53e0, 
		0x2380, 
		0xccc0, 
		0xb6c3, 
		0x4635, 
		0x0000, 
		0x23e6, 
		0xcc80, 
		0xd1c0, 
		0xcc00, 
		0x4f40, 
		0xd3c0, 
		0x0000, 
		0xd340, 
		0x7c00, 
		0x5340, 
		0xbf3b, 
		0x4e00, 
		0x0000, 
		0x9103, 
		0x6484, 
		0xbcae, 
		0xcd40, 
		0xbc00, 
		0x9e9b, 
		0xcfc0, 
		0x3c00, 
		0x0000, 
		0x5000, 
		0xa78a  
};
volatile uint32_t rand_ieee754_32[1536] __attribute__ ((aligned (2048))) = {
		0x7f800000, 
		0xc1a00000, 
		0x0c7fff80, 
		0xc2680000, 
		0x423c0000, 
		0xc2100000, 
		0x52fa5ed9, 
		0x0f7fffff, 
		0x42700000, 
		0x00001000, 
		0x1fa24c3b, 
		0xe255b13c, 
		0x42700000, 
		0x4ed8f15e, 
		0x80000800, 
		0x0c7ffff0, 
		0xc1b00000, 
		0x00000800, 
		0x0aa9df06, 
		0x970efd49, 
		0x0c7fffe0, 
		0x00800001, 
		0x0e003fff, 
		0x41200000, 
		0x00000000, 
		0x1e5ddfaa, 
		0x80000000, 
		0x49ce63a3, 
		0xc2580000, 
		0x00000020, 
		0x2086f113, 
		0xc0e00000, 
		0x0b516e34, 
		0xff800000, 
		0xa45ba1ad, 
		0x80ffffff, 
		0x00800000, 
		0x59ada889, 
		0x00800001, 
		0x807ffffe, 
		0x80800001, 
		0xf4ae02d2, 
		0xdfd21688, 
		0x00000000, 
		0xecf7564a, 
		0x41e80000, 
		0xff7ffffe, 
		0xae3f47a0, 
		0x80040000, 
		0x80800002, 
		0xa1cf8d96, 
		0x41800000, 
		0x7f800000, 
		0x43df5481, 
		0x9797a308, 
		0xaa83f7e8, 
		0x41500000, 
		0xc1f80000, 
		0x6fb36684, 
		0x0c600000, 
		0xff800000, 
		0x00000004, 
		0x42540000, 
		0xc946e802, 
		0xffa7534e, 
		0x00000000, 
		0x80000800, 
		0x42140000, 
		0x80000800, 
		0xbb7d1a31, 
		0x80100000, 
		0xff000000, 
		0x1437fde7, 
		0x42380000, 
		0x0e0001ff, 
		0x80000000, 
		0x00000010, 
		0xc22c0000, 
		0x80000080, 
		0x7fe4816b, 
		0xaaaaaaaa, 
		0x8012f797, 
		0x80000000, 
		0x00008000, 
		0xff800000, 
		0x805a7d58, 
		0x9ca50f0d, 
		0x80800001, 
		0xc0c00000, 
		0x7f7fffff, 
		0x2669e6fd, 
		0x80800001, 
		0x0c7fffc0, 
		0x5e4ca35c, 
		0x7faa8015, 
		0xc2380000, 
		0x80000000, 
		0x41e80000, 
		0x00040000, 
		0x3cebbcfc, 
		0xff800000, 
		0xc1300000, 
		0x7fffffff, 
		0xbf028f5c, 
		0x00000000, 
		0x80800000, 
		0x00000000, 
		0x7f800000, 
		0xef9b7596, 
		0x42300000, 
		0xf64588a0, 
		0xc2200000, 
		0x7eb86f1e, 
		0x6ede6c6d, 
		0x8f7ffffd, 
		0x41a80000, 
		0x00008000, 
		0xa0f8f55c, 
		0x01a26afe, 
		0x41880000, 
		0x00800001, 
		0x0f38a639, 
		0x0f7ffffc, 
		0x415da7a6, 
		0x7fbfffff, 
		0xc378b05c, 
		0x41b80000, 
		0xc2700000, 
		0x3666c13b, 
		0x707bc60a, 
		0xe656a6c8, 
		0x3f5eb7ea, 
		0x066c7758, 
		0xfd86fdf2, 
		0x42600000, 
		0x50713527, 
		0x342a58b3, 
		0x80000000, 
		0x9044f875, 
		0xe7b75678, 
		0x069e206c, 
		0x6ceb6e29, 
		0x8b97ad57, 
		0x7f800000, 
		0x0c7ffffc, 
		0x42700000, 
		0xe1307bd7, 
		0xff800000, 
		0x8dd722b5, 
		0xe6040dae, 
		0x00001000, 
		0xff800000, 
		0x11ed6bfd, 
		0x0e1fffff, 
		0x41400000, 
		0x86d3c478, 
		0x0e007fff, 
		0xff800000, 
		0xc22c0000, 
		0xc2440000, 
		0x8039a14b, 
		0x00100000, 
		0xc2240000, 
		0xbcdcc671, 
		0x1700487e, 
		0xffc00000, 
		0x7f000000, 
		0xff800000, 
		0x09d2ff9a, 
		0x0c7ff800, 
		0x3fe13939, 
		0x0c7fffe0, 
		0xc1600000, 
		0x1b8e96c1, 
		0x42300000, 
		0xff9fcdf5, 
		0xff800000, 
		0x4bffa5f3, 
		0x30eee8fe, 
		0x80000400, 
		0x42100000, 
		0x807ffffe, 
		0x41700000, 
		0xc0400000, 
		0xcb8c4b40, 
		0x41980000, 
		0x0e0000ff, 
		0x40e00000, 
		0x42780000, 
		0x0e00ffff, 
		0x7fc00000, 
		0x0e0003ff, 
		0x33ef34e3, 
		0x41d80000, 
		0x40800000, 
		0x80020000, 
		0xc2200000, 
		0x128c3ab1, 
		0x007ffffe, 
		0x0c7ffff8, 
		0x0e07ffff, 
		0x3f800001, 
		0x00000020, 
		0xff800000, 
		0x80008000, 
		0x42080000, 
		0x0c7fffc0, 
		0x5e51e6b8, 
		0x00800000, 
		0x42540000, 
		0xc1e00000, 
		0xc0000000, 
		0x5b33ffed, 
		0x00400000, 
		0x0d00fff0, 
		0x40c00000, 
		0xc2740000, 
		0x007fffff, 
		0xcb56e2b1, 
		0xcbe95f52, 
		0xc2240000, 
		0xff800000, 
		0x79444217, 
		0xff233111, 
		0xf92bec79, 
		0x42740000, 
		0xf07f38cb, 
		0x13697e2c, 
		0x4b000000, 
		0x961156f2, 
		0x0f7ffffc, 
		0xc25c0000, 
		0x7f800001, 
		0x80000008, 
		0x834b4a73, 
		0x00343c53, 
		0x3190ca0f, 
		0x4fa16a31, 
		0x00080000, 
		0xff000000, 
		0xc1400000, 
		0x55555555, 
		0x5b21224a, 
		0x7f800000, 
		0x0c780000, 
		0x0c7ffc00, 
		0x00000000, 
		0x4b000000, 
		0xd2bf324b, 
		0x00191094, 
		0x41f00000, 
		0x422c0000, 
		0x00000010, 
		0x41300000, 
		0x42b349d7, 
		0x0c7f0000, 
		0x0e000007, 
		0xcac64721, 
		0x2e7fc181, 
		0x43241f24, 
		0xff800000, 
		0x33333333, 
		0xf3efaa27, 
		0x8f7fffff, 
		0x80800003, 
		0x7f800000, 
		0xcf7ec4dd, 
		0x00000000, 
		0x0c7ff800, 
		0x00000080, 
		0x00000020, 
		0x42380000, 
		0x00800001, 
		0xc1e80000, 
		0x0e7fffff, 
		0x95c3f4ef, 
		0xf628ebb9, 
		0x0c7ffffc, 
		0x1845ced2, 
		0xc1900000, 
		0xd9891908, 
		0x7fc00000, 
		0xdf5addae, 
		0xc1f80000, 
		0x00000040, 
		0x80800000, 
		0x891566c0, 
		0xc1800000, 
		0x80000002, 
		0x00000000, 
		0xff7ffffe, 
		0x00008000, 
		0x7f800000, 
		0x80000000, 
		0xaaaaaaaa, 
		0x9bd64914, 
		0xa40d8b97, 
		0x00800000, 
		0x00800001, 
		0x0e0fffff, 
		0x422c0000, 
		0xab8211b1, 
		0xc0e00000, 
		0xc2400000, 
		0xe5f72a47, 
		0xcb87d166, 
		0xc2040000, 
		0x0e0003ff, 
		0x00000008, 
		0x42300000, 
		0xc25c0000, 
		0x802a20d9, 
		0xc2440000, 
		0x80000000, 
		0x00000000, 
		0x80000000, 
		0x7f800000, 
		0x00080000, 
		0x0c7ffff8, 
		0x80040000, 
		0xc2440000, 
		0x422c0000, 
		0x4f5784a1, 
		0xaf32924d, 
		0x4abc117d, 
		0x7fc00001, 
		0x7f7ffffe, 
		0x0e07ffff, 
		0x0f7ffffc, 
		0xcb8c4b40, 
		0x85f2d4e7, 
		0x7f7fffff, 
		0xc1f80000, 
		0x42000000, 
		0x42480000, 
		0x423c0000, 
		0xff800000, 
		0x7fc00001, 
		0x800bb7ce, 
		0x0e0007ff, 
		0x7f800000, 
		0xc2340000, 
		0x0e003fff, 
		0x00000000, 
		0xc2000000, 
		0x79614a45, 
		0x00001000, 
		0x00000000, 
		0x7f7ffffe, 
		0x0e00000f, 
		0x80000001, 
		0x0c7fff80, 
		0xfc3c8909, 
		0x3d702705, 
		0xc1600000, 
		0x0e000003, 
		0x00002000, 
		0x00800000, 
		0x0e00003f, 
		0x00004000, 
		0x80800001, 
		0x80100000, 
		0xc1800000, 
		0x0d00fff0, 
		0xffc00000, 
		0xedd506f8, 
		0xa7fc9fc8, 
		0x7f7bfb97, 
		0xbf028f5c, 
		0x40000000, 
		0x00000001, 
		0xff800000, 
		0xe3f946cd, 
		0x927857ea, 
		0xce61c5e4, 
		0x0e000003, 
		0x358303a6, 
		0xff800000, 
		0x00200000, 
		0x80000001, 
		0xa8af6748, 
		0xb2bd61ef, 
		0x424c0000, 
		0x40e00000, 
		0xff800000, 
		0xcfc5e819, 
		0x9f50dd82, 
		0x78fdb1bd, 
		0x80800003, 
		0xff800000, 
		0x923bf4b9, 
		0x80800002, 
		0x80000000, 
		0x42340000, 
		0xc1b00000, 
		0x3f800001, 
		0xcb3616a3, 
		0x6927c5d8, 
		0x0d00fff0, 
		0x0e00ffff, 
		0x00000000, 
		0x7f000000, 
		0x3d6dc174, 
		0x9930569e, 
		0x41c00000, 
		0xc2140000, 
		0xc621d4c9, 
		0x31eeba92, 
		0x426c0000, 
		0x42200000, 
		0x0f7ffffe, 
		0xc0a00000, 
		0x8f7fffff, 
		0xc2080000, 
		0xff800000, 
		0x80518d61, 
		0xe2144fb2, 
		0x0e00007f, 
		0x0e00003f, 
		0x7f7ffffe, 
		0x2a4fd95e, 
		0x9952b938, 
		0x807ffffe, 
		0x0c700000, 
		0xc0e00000, 
		0x185e0c4a, 
		0x00080000, 
		0x00000000, 
		0x0c7fffff, 
		0x0c7fff80, 
		0x80000008, 
		0x00000200, 
		0x80800002, 
		0x42200000, 
		0x3756c7e4, 
		0x41800000, 
		0x41400000, 
		0xf7f17cc5, 
		0x0c7ffff8, 
		0x42300000, 
		0x0e00ffff, 
		0x007fffff, 
		0x80000000, 
		0x034945d0, 
		0xc2240000, 
		0xad85a8a0, 
		0x70b03f37, 
		0x281b80c1, 
		0xff800000, 
		0x7f7ffffe, 
		0x805b66c2, 
		0x6f347b94, 
		0x0c7c0000, 
		0xc2000000, 
		0x00000000, 
		0x006b1d81, 
		0xc1f80000, 
		0x7b617e87, 
		0x85bbb089, 
		0x80000000, 
		0x42140000, 
		0xd5ee6fe5, 
		0x80000000, 
		0x41900000, 
		0x00000000, 
		0xc1f80000, 
		0x0e0007ff, 
		0x40000000, 
		0xc2340000, 
		0xc1e00000, 
		0x55792f4e, 
		0xc45aebdb, 
		0xa3b41a86, 
		0x41b00000, 
		0x489ca5e4, 
		0x80800001, 
		0xfb8da52f, 
		0xb2201264, 
		0x80000001, 
		0x5fff145a, 
		0x40633212, 
		0xff800000, 
		0x0c7ffff8, 
		0x41e00000, 
		0xe855d2c5, 
		0x40400000, 
		0x00200000, 
		0x80561558, 
		0xc6e41c40, 
		0x2ca69e73, 
		0xc25c0000, 
		0x7f800000, 
		0xc2100000, 
		0x00000004, 
		0x42140000, 
		0x80000001, 
		0x37ea2aa6, 
		0x42640000, 
		0xb28709c0, 
		0x0053111d, 
		0x80000004, 
		0x80000080, 
		0xc1f159ec, 
		0x41500000, 
		0x7f7fffff, 
		0x80000000, 
		0xbf028f5c, 
		0x0c7ffc00, 
		0xfdc84986, 
		0xc1400000, 
		0xc7e82a6e, 
		0x00000000, 
		0x0d00fff0, 
		0xc2080000, 
		0x00000002, 
		0x0c7fffff, 
		0x9e231f3a, 
		0x41800000, 
		0xff7fffff, 
		0x398bdc63, 
		0x3a59ea65, 
		0x80200000, 
		0x0f9cdcc5, 
		0x110471ba, 
		0xe7874e23, 
		0x42140000, 
		0xc2400000, 
		0xef8b9dc1, 
		0xd563a66c, 
		0x002a32f6, 
		0x7f9c5255, 
		0x00800001, 
		0x5641f418, 
		0x0e000fff, 
		0xff7ffffe, 
		0x7f800000, 
		0x4b000000, 
		0x007fffff, 
		0x80000002, 
		0x80000100, 
		0x059df4ec, 
		0x41e80000, 
		0xd1b9b59e, 
		0x0c600000, 
		0x00000002, 
		0x3b5529c1, 
		0x0c400000, 
		0x0c7c0000, 
		0x7fa4d97e, 
		0xb9f7a75f, 
		0x80002000, 
		0x80000100, 
		0x0e001fff, 
		0x42540000, 
		0xc67cbb20, 
		0x7f9bf927, 
		0xc2240000, 
		0x2b865c9e, 
		0x00000004, 
		0x0e0007ff, 
		0x00000000, 
		0x80004000, 
		0xff8a6f04, 
		0x80010000, 
		0x0e000fff, 
		0x0e00000f, 
		0x80100000, 
		0xc1d80000, 
		0x80100000, 
		0x80800003, 
		0x39286a3c, 
		0x0e00000f, 
		0x00000000, 
		0x143f910b, 
		0x9522efe0, 
		0x80800003, 
		0x80004000, 
		0xc1200000, 
		0x520ab790, 
		0xfe72c0a4, 
		0x3f800000, 
		0x80000008, 
		0x00000000, 
		0xc1880000, 
		0x0e03ffff, 
		0x00008000, 
		0x80000400, 
		0x7f800000, 
		0xa318a507, 
		0x0e0003ff, 
		0x7fffffff, 
		0x573a0a87, 
		0x81be5f83, 
		0xebaebeac, 
		0x80581dbc, 
		0xe3aba1a2, 
		0x9863daf3, 
		0xe2451229, 
		0x00000000, 
		0x24b283ac, 
		0x7f800000, 
		0x2c0dcc9e, 
		0x42300000, 
		0xffffffff, 
		0x80000000, 
		0xd246fa5a, 
		0x95569234, 
		0x00000004, 
		0xc25c0000, 
		0xc1800000, 
		0x0c7fffe0, 
		0x7f7fffff, 
		0x7f000000, 
		0xcb8c4b40, 
		0xa29d8260, 
		0x00000000, 
		0x0059b084, 
		0xc7ca96c2, 
		0x0e00000f, 
		0x4fb332f1, 
		0xf390c6e5, 
		0xc2700000, 
		0xabfa92a2, 
		0x7f8c9cc6, 
		0x00000000, 
		0xc2661a26, 
		0x420c0000, 
		0x54f5141c, 
		0x0c7fff00, 
		0x80000000, 
		0x41900000, 
		0xc2540000, 
		0x1b0cf517, 
		0x00000200, 
		0x80000000, 
		0x40c00000, 
		0x00000200, 
		0x42540000, 
		0x7f000000, 
		0x80dd24bc, 
		0x41880000, 
		0xe01ea7f7, 
		0xea974eee, 
		0x0e00000f, 
		0x7f800000, 
		0xff000000, 
		0x423c0000, 
		0x41200000, 
		0x7f800000, 
		0xff800000, 
		0xff7ffffe, 
		0xa46484d2, 
		0x7d990394, 
		0x80000001, 
		0x80000002, 
		0x55555555, 
		0x00000000, 
		0x00400000, 
		0x6b16bb68, 
		0xc1100000, 
		0x7e6a6ea4, 
		0x422c0000, 
		0xc1c00000, 
		0xc2240000, 
		0x1a7d9fd5, 
		0x8f7ffffe, 
		0x0e01ffff, 
		0x42740000, 
		0x0c780000, 
		0x7f800000, 
		0x00000000, 
		0x00000800, 
		0x00020000, 
		0x0f7fffff, 
		0x423c0000, 
		0xc26c0000, 
		0x46f4f2eb, 
		0x3f800000, 
		0x0c7ff000, 
		0x41e80000, 
		0x3bf6bdf0, 
		0xc1800000, 
		0x00040000, 
		0x41000000, 
		0x0d2543b4, 
		0x1c68d41a, 
		0xc543366c, 
		0x6472b0b9, 
		0x37e0c955, 
		0x41500000, 
		0x41b80000, 
		0x0fea627d, 
		0x6b8c4588, 
		0x426c0000, 
		0xff800000, 
		0xff800000, 
		0xef2d345c, 
		0xffbfffff, 
		0xc1500000, 
		0x40000000, 
		0x00800000, 
		0xc25c0000, 
		0x42500000, 
		0x9a2e5dc3, 
		0x42100000, 
		0x00800001, 
		0xe9232f5d, 
		0xe042c14a, 
		0x80000000, 
		0x5dabdec0, 
		0xc2280000, 
		0x00040000, 
		0x00a9f5ad, 
		0x0c700000, 
		0x0e01ffff, 
		0x288c6eec, 
		0x00000002, 
		0xc26c0000, 
		0x007fffff, 
		0xd47f4564, 
		0x9091042c, 
		0xff800000, 
		0x80000000, 
		0xba433e8a, 
		0x80000200, 
		0x7f000000, 
		0x0c7c0000, 
		0xf9cc1773, 
		0xa0c80e4c, 
		0x7f800000, 
		0x41f80000, 
		0x80000000, 
		0x0e7fffff, 
		0x09b300c8, 
		0x667235a3, 
		0x00040000, 
		0x7f800000, 
		0x00800000, 
		0x7f800000, 
		0xc2140000, 
		0x4b42a087, 
		0x0e00000f, 
		0x80000200, 
		0x314b8739, 
		0xc1500000, 
		0x427c0000, 
		0x7fbfffff, 
		0x00000000, 
		0x00000000, 
		0x41100000, 
		0xcb000000, 
		0xf9cc1e8b, 
		0xaa28284e, 
		0x00002000, 
		0x40800000, 
		0xbc639e6d, 
		0x5a11215c, 
		0x427c0000, 
		0xb53a0196, 
		0x9c85d799, 
		0x00000800, 
		0x0009268b, 
		0x55555555, 
		0x0d0f7f28, 
		0x80800003, 
		0xc1600000, 
		0x31ab8bb9, 
		0x0e0fffff, 
		0x2a6f06cd, 
		0x0e00001f, 
		0xdf19af46, 
		0xff800000, 
		0xc27c0000, 
		0xc1b80000, 
		0xc874b786, 
		0xc1200000, 
		0xc2600000, 
		0xaaf8ed62, 
		0x6c3d5551, 
		0xc2700000, 
		0x41d00000, 
		0xc1c00000, 
		0x80000080, 
		0x422c0000, 
		0x80000004, 
		0xc22c0000, 
		0x80010000, 
		0xb04e0847, 
		0x7f8e46f3, 
		0x41000000, 
		0xc1a00000, 
		0xeaeae66d, 
		0x0c7fc000, 
		0xffbfffff, 
		0xa90c9100, 
		0xb0d6b690, 
		0x00002000, 
		0x8006f28c, 
		0x0e00000f, 
		0x0f7ffffe, 
		0x00000000, 
		0xbf800000, 
		0x00000010, 
		0x425c0000, 
		0x420c0000, 
		0x80800003, 
		0x42140000, 
		0x420c0000, 
		0x00800003, 
		0xffb07f88, 
		0xc0800000, 
		0x41c80000, 
		0x42700000, 
		0x80800001, 
		0x42500000, 
		0x041b05dc, 
		0xa901e7a0, 
		0xffdc5bf0, 
		0x0ef0e2d9, 
		0x80000002, 
		0xc2640000, 
		0x7fc00001, 
		0x00000200, 
		0x41880000, 
		0xb00fd658, 
		0x0c7ff800, 
		0x06f7882a, 
		0x0e0000ff, 
		0xa53575a3, 
		0x00aed6bd, 
		0x7f800000, 
		0xa3ad7123, 
		0x40800000, 
		0xc1980000, 
		0x7fc00001, 
		0x433a9e25, 
		0x0e00003f, 
		0x80000000, 
		0x0c780000, 
		0xc25c0000, 
		0x422c0000, 
		0xc26c0000, 
		0xc20c0000, 
		0x0e03ffff, 
		0x0f7ffffc, 
		0x807ffffe, 
		0x37a218f9, 
		0x423c0000, 
		0x80000000, 
		0x00000000, 
		0x3f800001, 
		0x80000000, 
		0xc0400000, 
		0x0c7ffffc, 
		0x0e0fffff, 
		0xc1800000, 
		0x0e0001ff, 
		0x8f7fffff, 
		0x42640000, 
		0x41600000, 
		0xc2040000, 
		0x0c00e0e9, 
		0x0e00001f, 
		0x00011111, 
		0x80004000, 
		0x00000000, 
		0x0e0007ff, 
		0x7f800000, 
		0x007ffffe, 
		0x41900000, 
		0x42500000, 
		0xff7ffffe, 
		0x0e001fff, 
		0x7f8de12b, 
		0x48840cbc, 
		0x8f7ffffc, 
		0x0e3fffff, 
		0x00020000, 
		0x422c0000, 
		0x42340000, 
		0x8f7ffffd, 
		0xb9af337b, 
		0xc25c0000, 
		0xc2100000, 
		0xc1b80000, 
		0x09442f9c, 
		0xffbbed47, 
		0x80379eae, 
		0x7f7fffff, 
		0x80000000, 
		0x0c600000, 
		0x19b804f3, 
		0x0c7ffc00, 
		0x00000000, 
		0xab0b482b, 
		0x42680000, 
		0xc1800000, 
		0xee367ebf, 
		0xc22c0000, 
		0x3a6977e5, 
		0xc1100000, 
		0xb0f4484a, 
		0xcccccccc, 
		0x40400000, 
		0x80800003, 
		0xbf800000, 
		0x02a71341, 
		0x7fc00000, 
		0x92eb5209, 
		0xb2b60482, 
		0x0c7ff800, 
		0x00000400, 
		0x4b8c4b40, 
		0x41c00000, 
		0xc2000000, 
		0x95417c1a, 
		0xff800000, 
		0x80000040, 
		0xea1cc7ed, 
		0x234ffdce, 
		0x42100000, 
		0x3d19b690, 
		0xb561efbb, 
		0x00000000, 
		0xc1000000, 
		0x305a2236, 
		0xece4d23b, 
		0xff7ffffe, 
		0x42780000, 
		0x9579a1c3, 
		0x0c7ff000, 
		0x80000004, 
		0x80000000, 
		0x80000020, 
		0xb5c7963b, 
		0x0c600000, 
		0x80010000, 
		0x80040000, 
		0xbf800000, 
		0x7fc4933d, 
		0x00000800, 
		0xcb28a04d, 
		0x30ac38ab, 
		0xd2339816, 
		0xecaf5ce8, 
		0x0e000fff, 
		0x80000000, 
		0x42080000, 
		0xff7ffffe, 
		0xff7ffffe, 
		0x33333333, 
		0x80008000, 
		0xc5c7a7b1, 
		0x1a85bee7, 
		0xe955bab7, 
		0xb3b1fc24, 
		0x5951476d, 
		0x42700000, 
		0xaaaaaaaa, 
		0x41c00000, 
		0x67ae21bd, 
		0xe9c7018b, 
		0xc1700000, 
		0x0c7ffe00, 
		0xc2400000, 
		0x0e00003f, 
		0xc25c0000, 
		0xc1a00000, 
		0xe24050eb, 
		0xc1100000, 
		0x9fd3da8b, 
		0x3f028f5c, 
		0x41f00000, 
		0x41d80000, 
		0x00008000, 
		0xc3364377, 
		0x0e000001, 
		0x12199491, 
		0x0d00fff0, 
		0x2f834ff0, 
		0x00040000, 
		0x7fb12ea0, 
		0xd1b25a0f, 
		0x4c829163, 
		0x7bf4a350, 
		0x0f7ffffc, 
		0x7cd425fb, 
		0x752b9faa, 
		0x0c780000, 
		0x80000001, 
		0x41c00000, 
		0xcb000000, 
		0x67a11bda, 
		0x7f000000, 
		0x7f800000, 
		0x00000001, 
		0xff7fffff, 
		0x0c600000, 
		0x0e00000f, 
		0xff800000, 
		0x7212aa82, 
		0x0028fe4b, 
		0x26a923be, 
		0xff82cb9a, 
		0x98468c9b, 
		0x00000100, 
		0x75094f61, 
		0x40e00000, 
		0x80000002, 
		0xff800000, 
		0x4df0ced0, 
		0x42240000, 
		0xc1f80000, 
		0xffa732ae, 
		0x1d72e2de, 
		0x42700000, 
		0xbf17dbfb, 
		0xc1000000, 
		0x00800001, 
		0x00000000, 
		0x0e0001ff, 
		0x80000800, 
		0x422c0000, 
		0x423c0000, 
		0x3f28e609, 
		0x80800002, 
		0x7f800000, 
		0x42500000, 
		0x0c7f8000, 
		0x40e00000, 
		0x30504d20, 
		0x00100000, 
		0x00000000, 
		0x649d12a2, 
		0xcb8c4b40, 
		0x4e047797, 
		0xab1fc9d1, 
		0x801a4c69, 
		0xc1700000, 
		0x80000000, 
		0x80800003, 
		0xe63e4411, 
		0xa1328f22, 
		0x0c7ffe00, 
		0x58e73795, 
		0x41c80000, 
		0x0f7ffffd, 
		0x40c00000, 
		0x80c6a858, 
		0x41a80000, 
		0xc0a00000, 
		0xc1a80000, 
		0x42040000, 
		0x0c7f0000, 
		0x80000000, 
		0x0e00001f, 
		0xc1e80000, 
		0x80000000, 
		0xc1200000, 
		0x0b379f7e, 
		0xc3823cf6, 
		0x0e000fff, 
		0xc2180000, 
		0xd4ec1801, 
		0x00000008, 
		0xc21c0000, 
		0x9ff35890, 
		0xc2440000, 
		0xff800000, 
		0x80800000, 
		0x41800000, 
		0x80000002, 
		0x41700000, 
		0x930ca8ac, 
		0xff800000, 
		0x80000000, 
		0x40400000, 
		0xf046ba53, 
		0xcb000000, 
		0x0f7ffffe, 
		0xc20c0000, 
		0x89baf26a, 
		0x7feb81f7, 
		0x7fbf3e6c, 
		0xc26c0000, 
		0x0c7fc000, 
		0xc2540000, 
		0xf682d8df, 
		0x06aaeb62, 
		0x007fffff, 
		0x42180000, 
		0x0e0fffff, 
		0x5124fe15, 
		0x00000002, 
		0xbb79c5d6, 
		0x6fc25718, 
		0x00004000, 
		0x7f800000, 
		0x80000000, 
		0x0c600000, 
		0x0e0003ff, 
		0x41300000, 
		0x0d6eb53e, 
		0xc2600000, 
		0x32c5f9d2, 
		0xc0c00000, 
		0x41500000, 
		0x80800003, 
		0x41400000, 
		0x00000000, 
		0x00000000, 
		0xc2400000, 
		0x80000000, 
		0x9d963874, 
		0x42100000, 
		0x0c7ffc00, 
		0xffffffff, 
		0x0c7ffff8, 
		0xc24c0000, 
		0xf238ad81, 
		0x60ce7252, 
		0x5ae0d2c7, 
		0xc1900000, 
		0x00400000, 
		0x1b6c0669, 
		0x41c80000, 
		0x7fffffff, 
		0xad24996f, 
		0xc1700000, 
		0x42240000, 
		0xb2ab2730, 
		0xffbfffff, 
		0xc1f00000, 
		0x41700000, 
		0x7fc00000, 
		0x9d64b884, 
		0x41800000, 
		0xc1800000, 
		0x0c7fffe0, 
		0xcd9e675e, 
		0xc1a80000, 
		0x0e0fffff, 
		0x80000000, 
		0x42480000, 
		0x4b000000, 
		0xc23c0000, 
		0xc1f00000, 
		0x41500000, 
		0xc2400000, 
		0x007fffff, 
		0x585e1e0f, 
		0x00800001, 
		0x80800000, 
		0x42100000, 
		0xc23c0000, 
		0xfd31b454, 
		0x42240000, 
		0x00000001, 
		0x00000001, 
		0xaaaaaaaa, 
		0x67b5b107, 
		0x80020000, 
		0x56d7991d, 
		0x0c600000, 
		0x0c7ff800, 
		0x00800003, 
		0x3f800000, 
		0x00000000, 
		0x01c83a00, 
		0x7f7ffffe, 
		0x42300000, 
		0x17cda6b2, 
		0x9efe68e2, 
		0xc1200000, 
		0x3f028f5c, 
		0x057c9ef3, 
		0x80800000, 
		0x7f7ffffe, 
		0xc24c0000, 
		0x53b698ac, 
		0x80000000, 
		0xc88e0442, 
		0x7f800000, 
		0x80000008, 
		0x80800003, 
		0x00000000, 
		0xc3b8861d, 
		0xc185da48, 
		0x80200000, 
		0x41200000, 
		0xc0e00000, 
		0xc1600000, 
		0x42240000, 
		0x8f7ffffe, 
		0x00000080, 
		0x42700000, 
		0x80000004, 
		0x98eb8c50, 
		0x0c7f0000, 
		0xff800000, 
		0x8f7ffffc, 
		0x0c600000, 
		0x41d80000, 
		0x7f800000, 
		0x8ea7dcab, 
		0xbf028f5c, 
		0x80000000, 
		0x42280000, 
		0xc00da837, 
		0x41e00000, 
		0x00000080, 
		0xc0400000, 
		0xc2600000, 
		0x41c00000, 
		0x7f7ffffe, 
		0xf219c792, 
		0x7f800000, 
		0x00000002, 
		0x40800000, 
		0x7f000000, 
		0xff000000, 
		0x0e7fffff, 
		0x41800000, 
		0x00011111, 
		0x0c7ff800, 
		0x80008000, 
		0x80200000, 
		0x80020000, 
		0x76eb0abe, 
		0x841473f2, 
		0x7f800000, 
		0x0f4dbed1, 
		0x80000000, 
		0xf0c15763, 
		0xc2600000, 
		0xff800000, 
		0x63cd8d30, 
		0x80800000, 
		0xaaaaaaaa, 
		0xc2180000, 
		0x42780000, 
		0x80020000, 
		0x09fa693a, 
		0xff7fffff, 
		0x15596c9a, 
		0xbf800000, 
		0x80800000, 
		0x00000000, 
		0x00008000, 
		0xb3b73e07, 
		0x0c7fffff, 
		0x00800000, 
		0x7f800000, 
		0x80040000, 
		0xf16dc55a, 
		0x80020000, 
		0x0e000003, 
		0x80000800, 
		0xaaaaaaaa, 
		0x80100000, 
		0x80200000, 
		0x7bb938f0, 
		0x00011111, 
		0x4b6a1f7c, 
		0x0c7fc000, 
		0x6966cecb, 
		0x00000008, 
		0x8071902a, 
		0x7f800000, 
		0x00000400, 
		0x3f97537c, 
		0x31d6c353, 
		0x00200000, 
		0x00000000, 
		0x80020000, 
		0x6804aaa9, 
		0x3f028f5c, 
		0x007fffff, 
		0xc2040000, 
		0xe63f2627, 
		0x80000000, 
		0x80800003, 
		0x41a80000, 
		0xc1b80000, 
		0x4b8c4b40, 
		0x42540000, 
		0x0c7fffc0, 
		0x5011a1b7, 
		0x7f800000, 
		0x42500000, 
		0x4d4c7d77, 
		0x0c7ff800, 
		0xc2300000, 
		0x0e0003ff, 
		0x21bead28, 
		0x80100000, 
		0x877c8ebd, 
		0xc2780000, 
		0x7f800000, 
		0x40a00000, 
		0x80000002, 
		0x00000100, 
		0x8f7ffffe, 
		0x0c400000, 
		0x8827cae3, 
		0x211da847, 
		0x8dcf9723, 
		0x493e9ba9, 
		0x80000001, 
		0xff800000, 
		0x388292a5, 
		0x007fffff, 
		0x0c7ff800, 
		0x7fc00001, 
		0x00000100, 
		0x3f800000, 
		0xae6d028e, 
		0x40c00000, 
		0x6db326ec, 
		0x497aaa3b, 
		0x00100000, 
		0x0e003fff, 
		0xbf028f5c, 
		0x80000000, 
		0x00000000, 
		0xe1a7d02a, 
		0x424c0000, 
		0x80ffffff, 
		0x41a00000, 
		0x00000000, 
		0x26e8c0ad, 
		0xffbdbe8c, 
		0x32947990, 
		0x42140000, 
		0x42640000, 
		0x8f7ffffe, 
		0xc2100000, 
		0xc1b00000, 
		0x42740000, 
		0x9394808e, 
		0xc21c0000, 
		0x00000001, 
		0x43a0b41e, 
		0x421db5b4, 
		0xcb3204d6, 
		0x41c00000, 
		0xc1200000, 
		0x7fc00001, 
		0xff800000, 
		0x713e80f5, 
		0x41a00000, 
		0x3f800001, 
		0x426c3010, 
		0x80200000, 
		0x7f869eb4, 
		0x42740000, 
		0x00000000, 
		0x41f80000, 
		0x9cb66e4e, 
		0x79e01817, 
		0x00ffffff, 
		0xc2200000, 
		0x483b7257, 
		0x1c2b9571, 
		0x0c7e0000, 
		0x80000000, 
		0x80000008, 
		0x63b3a4c0, 
		0x8351402f, 
		0xc1000000, 
		0x0d02b22b, 
		0xbf800001, 
		0x3f800001, 
		0xdceafb8c, 
		0x0e000007, 
		0x40400000, 
		0x65ccfa84, 
		0x0f7ffffe, 
		0xff800000, 
		0x0ec08399, 
		0x964f9b72, 
		0xbf800001, 
		0x42140000, 
		0x420c0000, 
		0x41880000, 
		0xb98d1bf6, 
		0x5ec09f97, 
		0x80000008, 
		0x423c0000, 
		0x0e00003f, 
		0x41e80000, 
		0x00000020, 
		0x41b00000, 
		0x00000000, 
		0xbf028f5c, 
		0xc2180000, 
		0x00800001, 
		0x00040000, 
		0x00000001, 
		0x436f7cd0, 
		0x0e03ffff, 
		0xc1600000, 
		0x00000020, 
		0x00400000, 
		0x12b6f5ea, 
		0x94088ff7, 
		0x80011111, 
		0x00800002, 
		0xd84aa3e3, 
		0x0f7fffff, 
		0xc3bbe3f6, 
		0x80000000, 
		0x18568c77, 
		0x3f028f5c, 
		0x637f26ce, 
		0x80002000, 
		0x3a4c59c2, 
		0xc2180000, 
		0x007fffff, 
		0x40400000, 
		0x42780000, 
		0x420c0000, 
		0x41600000, 
		0xff800000, 
		0x80000400, 
		0x00800001, 
		0xc0e00000, 
		0x80000000, 
		0x40400000, 
		0x7fbfffff, 
		0xc1100000, 
		0x40c00000, 
		0x80002000, 
		0xca53ed8c, 
		0x00000000, 
		0x21e8fc30, 
		0x40800000, 
		0xc1f80000, 
		0x00000800, 
		0x41a80000, 
		0x80800003, 
		0xbe6da9ee, 
		0xc1c00000, 
		0x00800000, 
		0x40400000, 
		0x80000000, 
		0x42080000, 
		0x00020000, 
		0x80000001, 
		0x0c780000, 
		0xc2000000, 
		0x00800003, 
		0x0c7ffffc, 
		0x80004000, 
		0x80000200, 
		0x0e0fffff, 
		0x7f8a08fc, 
		0x80000040, 
		0x4d7783bc, 
		0x7f800000, 
		0x80000040, 
		0x41e80000, 
		0xb4374899, 
		0x00000002, 
		0x80000000, 
		0x80000008, 
		0x00800002, 
		0x41700000, 
		0x00000040, 
		0x80800001, 
		0xc2600000, 
		0x6da8e1e7, 
		0xbe22b9bf, 
		0x0c7ffffe, 
		0xff800000, 
		0xc1c80000, 
		0x42180000, 
		0x42200000, 
		0xbed1ab4f, 
		0x3f800000, 
		0xc2100000, 
		0x41c80000, 
		0x00100000, 
		0x80800001, 
		0x0f7fffff, 
		0x0c7f0000, 
		0xb054a741, 
		0x263786c5, 
		0x8f7ffffc, 
		0x80001000, 
		0xf48fca28, 
		0x6a96ae66, 
		0xcc3020ca, 
		0xc1d80000, 
		0x80000200, 
		0x80800001, 
		0xc1000000, 
		0x57dde947, 
		0xc2100000, 
		0x7f800000, 
		0xc1e80000, 
		0x00001000, 
		0xc2240000, 
		0xff7fffff, 
		0x0d00fff0, 
		0xff8dc866, 
		0xc2140000, 
		0x6e6048da, 
		0x7f855890, 
		0x7f7ffffe, 
		0x80200000, 
		0xd1759114, 
		0x0c400000, 
		0xd3f73b3a, 
		0xff800000, 
		0x3404a0d0, 
		0x00000000, 
		0xc1f00000, 
		0x0c7fe000, 
		0xbbad0214, 
		0xc23c0000, 
		0x42180000, 
		0x974a22e9, 
		0xc2000000, 
		0x00000002, 
		0xc40f602d, 
		0x3840e14b, 
		0x80800001, 
		0xc2700000, 
		0x422c0000, 
		0x85c9a361, 
		0x0c7ffff8, 
		0x80000100, 
		0x80000002, 
		0x0c7c0000, 
		0x0e000fff  
};
volatile uint64_t check_sum_hid_0[16] __attribute__ ((aligned (1024))) = {
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff, 
		0xffffffffffffffff  
};
volatile uint32_t mem_ptr_offset[256] __attribute__ ((aligned (1024))) = {
		0x00000000, 
		0x00000004, 
		0x00000008, 
		0x0000000c, 
		0x00000010, 
		0x00000014, 
		0x00000018, 
		0x0000001c, 
		0x00000020, 
		0x00000024, 
		0x00000028, 
		0x0000002c, 
		0x00000030, 
		0x00000034, 
		0x00000038, 
		0x0000003c, 
		0x00000000, 
		0x00000004, 
		0x00000008, 
		0x0000000c, 
		0x00000010, 
		0x00000014, 
		0x00000018, 
		0x0000001c, 
		0x00000020, 
		0x00000024, 
		0x00000028, 
		0x0000002c, 
		0x00000030, 
		0x00000034, 
		0x00000038, 
		0x0000003c, 
		0x00000000, 
		0x00000004, 
		0x00000008, 
		0x0000000c, 
		0x00000010, 
		0x00000014, 
		0x00000018, 
		0x0000001c, 
		0x00000020, 
		0x00000024, 
		0x00000028, 
		0x0000002c, 
		0x00000030, 
		0x00000034, 
		0x00000038, 
		0x0000003c, 
		0x00000000, 
		0x00000004, 
		0x00000008, 
		0x0000000c, 
		0x00000010, 
		0x00000014, 
		0x00000018, 
		0x0000001c, 
		0x00000020, 
		0x00000024, 
		0x00000028, 
		0x0000002c, 
		0x00000030, 
		0x00000034, 
		0x00000038, 
		0x0000003c, 
		0x00000000, 
		0x00000004, 
		0x00000008, 
		0x0000000c, 
		0x00000010, 
		0x00000014, 
		0x00000018, 
		0x0000001c, 
		0x00000020, 
		0x00000024, 
		0x00000028, 
		0x0000002c, 
		0x00000030, 
		0x00000034, 
		0x00000038, 
		0x0000003c, 
		0x00000000, 
		0x00000004, 
		0x00000008, 
		0x0000000c, 
		0x00000010, 
		0x00000014, 
		0x00000018, 
		0x0000001c, 
		0x00000020, 
		0x00000024, 
		0x00000028, 
		0x0000002c, 
		0x00000030, 
		0x00000034, 
		0x00000038, 
		0x0000003c, 
		0x00000000, 
		0x00000004, 
		0x00000008, 
		0x0000000c, 
		0x00000010, 
		0x00000014, 
		0x00000018, 
		0x0000001c, 
		0x00000020, 
		0x00000024, 
		0x00000028, 
		0x0000002c, 
		0x00000030, 
		0x00000034, 
		0x00000038, 
		0x0000003c, 
		0x00000000, 
		0x00000004, 
		0x00000008, 
		0x0000000c, 
		0x00000010, 
		0x00000014, 
		0x00000018, 
		0x0000001c, 
		0x00000020, 
		0x00000024, 
		0x00000028, 
		0x0000002c, 
		0x00000030, 
		0x00000034, 
		0x00000038, 
		0x0000003c, 
		0x00000000, 
		0x00000004, 
		0x00000008, 
		0x0000000c, 
		0x00000010, 
		0x00000014, 
		0x00000018, 
		0x0000001c, 
		0x00000020, 
		0x00000024, 
		0x00000028, 
		0x0000002c, 
		0x00000030, 
		0x00000034, 
		0x00000038, 
		0x0000003c, 
		0x00000000, 
		0x00000004, 
		0x00000008, 
		0x0000000c, 
		0x00000010, 
		0x00000014, 
		0x00000018, 
		0x0000001c, 
		0x00000020, 
		0x00000024, 
		0x00000028, 
		0x0000002c, 
		0x00000030, 
		0x00000034, 
		0x00000038, 
		0x0000003c, 
		0x00000000, 
		0x00000004, 
		0x00000008, 
		0x0000000c, 
		0x00000010, 
		0x00000014, 
		0x00000018, 
		0x0000001c, 
		0x00000020, 
		0x00000024, 
		0x00000028, 
		0x0000002c, 
		0x00000030, 
		0x00000034, 
		0x00000038, 
		0x0000003c, 
		0x00000000, 
		0x00000004, 
		0x00000008, 
		0x0000000c, 
		0x00000010, 
		0x00000014, 
		0x00000018, 
		0x0000001c, 
		0x00000020, 
		0x00000024, 
		0x00000028, 
		0x0000002c, 
		0x00000030, 
		0x00000034, 
		0x00000038, 
		0x0000003c, 
		0x00000000, 
		0x00000004, 
		0x00000008, 
		0x0000000c, 
		0x00000010, 
		0x00000014, 
		0x00000018, 
		0x0000001c, 
		0x00000020, 
		0x00000024, 
		0x00000028, 
		0x0000002c, 
		0x00000030, 
		0x00000034, 
		0x00000038, 
		0x0000003c, 
		0x00000000, 
		0x00000004, 
		0x00000008, 
		0x0000000c, 
		0x00000010, 
		0x00000014, 
		0x00000018, 
		0x0000001c, 
		0x00000020, 
		0x00000024, 
		0x00000028, 
		0x0000002c, 
		0x00000030, 
		0x00000034, 
		0x00000038, 
		0x0000003c, 
		0x00000000, 
		0x00000004, 
		0x00000008, 
		0x0000000c, 
		0x00000010, 
		0x00000014, 
		0x00000018, 
		0x0000001c, 
		0x00000020, 
		0x00000024, 
		0x00000028, 
		0x0000002c, 
		0x00000030, 
		0x00000034, 
		0x00000038, 
		0x0000003c, 
		0x00000000, 
		0x00000004, 
		0x00000008, 
		0x0000000c, 
		0x00000010, 
		0x00000014, 
		0x00000018, 
		0x0000001c, 
		0x00000020, 
		0x00000024, 
		0x00000028, 
		0x0000002c, 
		0x00000030, 
		0x00000034, 
		0x00000038, 
		0x0000003c  
};
volatile uint32_t mem_ptr_misalign_offset[256] __attribute__ ((aligned (1024))) = {
		0x00000063, 
		0x00000395, 
		0x000000a1, 
		0x00000213, 
		0x00000365, 
		0x00000301, 
		0x00000095, 
		0x00000029, 
		0x000002a3, 
		0x00000257, 
		0x00000263, 
		0x00000013, 
		0x000000bd, 
		0x00000281, 
		0x000002fd, 
		0x00000251, 
		0x0000010b, 
		0x000001fd, 
		0x000003c9, 
		0x0000037b, 
		0x00000053, 
		0x000001ad, 
		0x00000207, 
		0x0000022f, 
		0x000002bb, 
		0x00000189, 
		0x00000217, 
		0x0000037b, 
		0x000001f7, 
		0x0000014d, 
		0x00000337, 
		0x0000034f, 
		0x000003b5, 
		0x0000009d, 
		0x00000337, 
		0x00000321, 
		0x00000257, 
		0x000001b9, 
		0x0000028d, 
		0x000002d5, 
		0x0000023d, 
		0x00000259, 
		0x000001cd, 
		0x0000009f, 
		0x00000375, 
		0x0000025b, 
		0x00000257, 
		0x000001d1, 
		0x00000165, 
		0x00000037, 
		0x00000319, 
		0x000000ab, 
		0x000002a3, 
		0x0000034d, 
		0x00000015, 
		0x00000131, 
		0x00000017, 
		0x0000017b, 
		0x000001ef, 
		0x00000007, 
		0x00000133, 
		0x00000293, 
		0x0000019f, 
		0x00000189, 
		0x000001b9, 
		0x0000038d, 
		0x0000025f, 
		0x0000033b, 
		0x0000013d, 
		0x0000010d, 
		0x000002dd, 
		0x0000001f, 
		0x0000002f, 
		0x00000223, 
		0x00000177, 
		0x0000011d, 
		0x00000015, 
		0x000001d5, 
		0x00000089, 
		0x000000ed, 
		0x00000239, 
		0x00000037, 
		0x0000012d, 
		0x00000039, 
		0x000002ef, 
		0x00000379, 
		0x000003dd, 
		0x00000087, 
		0x000000f1, 
		0x000001ed, 
		0x000003ad, 
		0x000000bf, 
		0x0000037f, 
		0x00000239, 
		0x000001b1, 
		0x00000361, 
		0x000003c7, 
		0x000000f3, 
		0x000001ab, 
		0x00000109, 
		0x00000397, 
		0x0000014f, 
		0x00000281, 
		0x000001fd, 
		0x000002e7, 
		0x00000179, 
		0x00000007, 
		0x00000377, 
		0x00000135, 
		0x00000023, 
		0x00000127, 
		0x00000343, 
		0x0000028b, 
		0x00000157, 
		0x0000022d, 
		0x000003df, 
		0x000002e9, 
		0x00000377, 
		0x000001f7, 
		0x00000361, 
		0x0000027d, 
		0x000002c5, 
		0x0000004b, 
		0x0000020d, 
		0x0000008d, 
		0x00000083, 
		0x000002df, 
		0x00000267, 
		0x000001e7, 
		0x00000049, 
		0x00000223, 
		0x000002cb, 
		0x000003c5, 
		0x00000075, 
		0x00000283, 
		0x000002e9, 
		0x000001bb, 
		0x00000351, 
		0x00000321, 
		0x000003eb, 
		0x00000071, 
		0x00000395, 
		0x0000014b, 
		0x000002d3, 
		0x00000317, 
		0x000003fb, 
		0x00000029, 
		0x00000309, 
		0x00000321, 
		0x0000029f, 
		0x00000287, 
		0x000001cf, 
		0x0000027d, 
		0x00000311, 
		0x00000071, 
		0x0000014f, 
		0x00000385, 
		0x0000013b, 
		0x00000067, 
		0x000003eb, 
		0x00000391, 
		0x0000032d, 
		0x000002f3, 
		0x00000189, 
		0x0000007f, 
		0x000001cf, 
		0x0000005f, 
		0x000002d1, 
		0x00000399, 
		0x00000129, 
		0x000002c3, 
		0x000002d1, 
		0x000001d9, 
		0x000001d1, 
		0x000002c1, 
		0x00000007, 
		0x00000193, 
		0x000002d7, 
		0x000001c5, 
		0x0000009d, 
		0x000001f1, 
		0x0000016d, 
		0x0000017d, 
		0x00000043, 
		0x00000011, 
		0x000003b1, 
		0x000002ad, 
		0x00000327, 
		0x0000015f, 
		0x000003af, 
		0x00000287, 
		0x000001f1, 
		0x00000145, 
		0x00000011, 
		0x00000127, 
		0x00000035, 
		0x00000365, 
		0x00000309, 
		0x00000375, 
		0x00000137, 
		0x000000cb, 
		0x00000057, 
		0x000001f1, 
		0x0000026b, 
		0x000002a5, 
		0x00000177, 
		0x0000037d, 
		0x0000012b, 
		0x000002c5, 
		0x000001ef, 
		0x0000037d, 
		0x00000155, 
		0x00000229, 
		0x000000f7, 
		0x0000028d, 
		0x000001fd, 
		0x000000cf, 
		0x000001bf, 
		0x0000005f, 
		0x00000373, 
		0x000000d9, 
		0x00000187, 
		0x00000041, 
		0x000000f5, 
		0x000002ff, 
		0x000000ff, 
		0x000001c9, 
		0x00000041, 
		0x000001a3, 
		0x00000217, 
		0x00000143, 
		0x0000030b, 
		0x000002a5, 
		0x000003e5, 
		0x00000391, 
		0x0000005b, 
		0x00000261, 
		0x000003e5, 
		0x00000229, 
		0x00000209, 
		0x00000337, 
		0x000001a3, 
		0x00000177, 
		0x000001bd, 
		0x0000001f, 
		0x00000339, 
		0x00000335, 
		0x000001bb, 
		0x0000019d, 
		0x0000036b, 
		0x0000019b, 
		0x00000097, 
		0x000002b9, 
		0x0000026b, 
		0x0000008d, 
		0x0000001f  
};

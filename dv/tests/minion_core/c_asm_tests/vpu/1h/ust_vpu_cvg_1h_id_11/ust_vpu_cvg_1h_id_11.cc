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
			"csrwi mhpmevent3, 18\n"   
			"csrwi mhpmevent4, 19\n"   
			"csrwi mhpmevent5, 26\n"   
			"csrwi mhpmevent6, 2\n"   
			"csrwi mhpmevent7, 6\n"   
			"csrwi mhpmevent8, 3\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f20,  320 (x5)\n"
			"flw.ps f25,  928 (x5)\n"
			"flw.ps f15,  0 (x5)\n"
			"flw.ps f22,  672 (x5)\n"
			"flw.ps f2,  96 (x5)\n"
			"flw.ps f18,  640 (x5)\n"
			"flw.ps f12,  32 (x5)\n"
			"flw.ps f4,  704 (x5)\n"
			"flw.ps f9,  768 (x5)\n"
			"flw.ps f13,  256 (x5)\n"
			"flw.ps f26,  896 (x5)\n"
			"flw.ps f24,  192 (x5)\n"
			"flw.ps f3,  960 (x5)\n"
			"flw.ps f0,  352 (x5)\n"
			"flw.ps f17,  864 (x5)\n"
			"flw.ps f10,  416 (x5)\n"
			"flw.ps f19,  224 (x5)\n"
			"flw.ps f1,  800 (x5)\n"
			"flw.ps f29,  992 (x5)\n"
			"flw.ps f31,  736 (x5)\n"
			"flw.ps f16,  832 (x5)\n"
			"flw.ps f7,  384 (x5)\n"
			"flw.ps f27,  544 (x5)\n"
			"flw.ps f11,  64 (x5)\n"
			"flw.ps f14,  576 (x5)\n"
			"flw.ps f21,  512 (x5)\n"
			"flw.ps f23,  480 (x5)\n"
			"flw.ps f6,  288 (x5)\n"
			"flw.ps f28,  608 (x5)\n"
			"flw.ps f30,  128 (x5)\n"
			"flw.ps f5,  448 (x5)\n"
			"flw.ps f8,  160 (x5)\n"
			"csrw fflags, zero\n"
			"csrwi frm, 0x0\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_mask, zero\n"
			"csrwi tensor_wait, 0x6\n" 
			"fence\n"
			"csrwi tensor_wait, 0x6\n" 
			"csrrs x0, mcache_control, 1\n"
			"csrwi tensor_wait, 0x6\n" 
			"fence\n"
			"csrrs x0, mcache_control, 2\n"
			"csrwi excl_mode, 0\n"
			"fence\n"
			"li x31, 0x40\n"                 
			"la x11, rand_ieee754_32\n"    
			"add x5, x11, zero\n"            
			"li x6,  0xb \n"                 
			"add x5, x6, x5\n"               
			"csrw tensor_load, x5\n"         
			"li x5, 1024\n"                  
			"add x5, x11, x5\n"              
			"li x6, 0x018000000000000b\n"    
			"add x5, x6, x5\n"               
			"csrw tensor_load, x5\n"         
			"li x5, 2048\n"                  
			"add x5, x11, x5\n"              
			"li x6, 0x030000000000000b\n"    
			"add x5, x6, x5\n"               
			"csrw tensor_load, x5\n"         
			"li x5, 3072\n"                  
			"add x5, x11, x5\n"              
			"li x6, 0x048000000000000b\n"    
			"add x5, x6, x5\n"               
			"csrw tensor_load, x5\n"         
			"csrwi tensor_wait, 0x0\n"      
			"fence\n"
			"li x5,  0x1ff800000610007\n" 
			"csrw tensor_fma, x5\n" 
			"csrwi tensor_wait, 7\n" 
			"LBL_KERNEL_HID_0:\n"          
			"li x5, 0xFEEDFEED\n"
			"li x0, 0x4b0\n" 
			"slti x0, x0, 0x7f2\n"
			"la x31, check_sum_hid_0\n"
			"li x5, 0x1\n" 
			"sw x5, (x31)\n"
			"li x30, 0\n"
			"addi x30, x30, 1\n"
			"la x10, rand_int_32\n"
			"flw.ps f13, 0(x10)\n"
			"flw.ps f20, 128(x10)\n"
			"fsub.pi f22, f13, f20\n" 
			"la x22, rand_int_32\n"
			"flw.ps f16, 768(x22)\n"
			"flw.ps f20, 704(x22)\n"
			"fsub.pi f8, f16, f20\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f1, 864(x6)\n"
			"flw.ps f30, 0(x6)\n"
			"fdiv.ps f21, f1, f30, rmm\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f16, 224(x26)\n"
			"flw.ps f5, 64(x26)\n"
			"flt.ps f14, f16, f5\n" 
			"la x11, rand_int_32\n"
			"flw.ps f7, 320(x11)\n"
			"flw.ps f8, 32(x11)\n"
			"fmin.pi f20, f7, f8\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f14, 544(x22)\n"
			"flw.ps f27, 640(x22)\n"
			"flw.ps f8, 672(x22)\n"
			"fmadd.ps f16, f14, f27, f8, rne\n" 
			"la x9, rand_int_32\n"
			"flw.ps f24, 128(x9)\n"
			"flw.ps f21, 448(x9)\n"
			"fsrl.pi f22, f24, f21\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f20, 384(x16)\n"
			"flw.ps f0, 416(x16)\n"
			"flw.ps f24, 160(x16)\n"
			"fmsub.ps f0, f20, f0, f24, rtz\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_0:\n"
			"mova.m.x x16\n" 
			"li x5, 0x803400D0\n"
			"ld x5, 0(x5)\n"
			"li x5, 0x396bc41399052219\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0x71cdbf8d039103e\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"la x6, rand_int_32\n"
			"flw.ps f23, 608(x6)\n"
			"flw.ps f5, 928(x6)\n"
			"fand.pi f21, f23, f5\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f14, 960(x15)\n"
			"fcvt.w.s x7, f14, rne\n" 
			"li x5, 0x69c4\n"
			"csrw tensor_mask, x5\n"	
			"la x17, rand_int_32\n"
			"flw.ps f1, 736(x17)\n"
			"flw.ps f14, 576(x17)\n"
			"fmul.pi f1, f1, f14\n" 
			"li x16, 0x6b7a721786e6fe13\n"
			"fcvt.s.w f22, x16, rup\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x11, rand_ieee754_32\n"
			"flw.ps f21, 320(x11)\n"
			"flw.ps f8, 736(x11)\n"
			"fdiv.ps f5, f21, f8, rmm\n" 
			"LBL_SEQID_0_FP_TRANS_ID_1_HID_0:\n"
			"la x16, rand_ieee754_32\n"
			"flw.ps f20, 832(x16)\n"
			"flog.ps f0, f20\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f8, 448(x26)\n"
			"flt.s x26, f8, f8\n" 
			"la x10, rand_int_32\n"
			"flw.ps f24, 288(x10)\n"
			"flw.ps f21, 704(x10)\n"
			"fmulh.pi f13, f24, f21\n" 
			"la x9, rand_int_32\n"
			"flw.ps f20, 896(x9)\n"
			"flw.ps f30, 96(x9)\n"
			"feq.pi f22, f20, f30\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f13, 576(x11)\n"
			"flw.ps f16, 288(x11)\n"
			"fle.ps f9, f13, f16\n" 
			"LBL_SEQID_0_FP_TRANS_ID_2_HID_0:\n"
			"la x11, rand_ieee754_32\n"
			"flw.ps f14, 736(x11)\n"
			"frcp.ps f8, f14\n" 
			"la x17, rand_int_32\n"
			"flw.ps f7, 608(x17)\n"
			"flw.ps f23, 832(x17)\n"
			"fltu.pi f1, f7, f23\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x15, rand_ieee754_32\n"
			"flw.ps f9, 704(x15)\n"
			"fcvt.f16.ps f24, f9\n" 
			"mova.x.m x10\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f5, 320(x16)\n"
			"flw.ps f13, 160(x16)\n"
			"fdiv.ps f9, f5, f13, rne\n" 
			"li x5, 0x19d3\n"
			"csrw tensor_mask, x5\n"	
			"li x11, 0x531eeb8dc8bddd28\n"
			"fcvt.s.wu f8, x11, rmm\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x10, rand_ieee754_32\n"
			"flw.ps f14, 96(x10)\n"
			"flw.ps f9, 352(x10)\n"
			"flw.ps f23, 672(x10)\n"
			"fmsub.ps f23, f14, f9, f23, rdn\n" 
			"la x16, rand_int_32\n"
			"flw.ps f5, 736(x16)\n"
			"flw.ps f7, 128(x16)\n"
			"fadd.pi f30, f5, f7\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f21, 448(x22)\n"
			"flw.ps f16, 640(x22)\n"
			"fle.ps f13, f21, f16\n" 
			"LBL_SEQID_0_M0_WRITE_ID_3_HID_0:\n"
			"maskand m2, m3, m1\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x22, rand_int_32\n"
			"flw.ps f20, 416(x22)\n"
			"flw.ps f8, 864(x22)\n"
			"fsrl.pi f14, f20, f8\n" 
			"LBL_SEQID_0_M0_WRITE_ID_4_HID_0:\n"
			"fltm.pi m1, f30, f20\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"fbci.pi f30, 0x7ba40\n" 
			"csrw tensor_error, zero\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ID_632126_HID_0:\n"
			"li x5, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x2 << TENSOR_IMA_B_NUM_COLS) | (0xb << TENSOR_IMA_A_NUM_ROWS) | (0x4 << TENSOR_IMA_A_NUM_COLS) | (0xb << TENSOR_IMA_OFFSET) | (0x0 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x92 << TENSOR_IMA_SCP_LOC_B) | (0x5 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x0 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x6, ((0x1d << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0xb << TENSOR_QUANT_QUANT_ROW) | (0x2e << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x1 << TENSOR_QUANT_TRANSF7) | (0x9 << TENSOR_QUANT_TRANSF6) | (0x9 << TENSOR_QUANT_TRANSF5) | (0x4 << TENSOR_QUANT_TRANSF4) | (0xa << TENSOR_QUANT_TRANSF3) | (0x5 << TENSOR_QUANT_TRANSF2) | (0x4 << TENSOR_QUANT_TRANSF1) | (0x9 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 7\n"
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_0\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ITER_0_HID_0:\n"
			"li x5, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x1 << TENSOR_FMA32_B_NUM_COLS) | (0xd << TENSOR_FMA32_A_NUM_ROWS) | (0xf << TENSOR_FMA32_A_NUM_COLS) | (0xf << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x17 << TENSOR_FMA32_SCP_LOC_B) | (0xcd << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ITER_1_HID_0:\n"
			"li x5, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x3 << TENSOR_FMA16_B_NUM_COLS) | (0xe << TENSOR_FMA16_A_NUM_ROWS) | (0x4 << TENSOR_FMA16_A_NUM_COLS) | (0x5 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0xcf << TENSOR_FMA16_SCP_LOC_B) | (0xd4 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x0 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ITER_2_HID_0:\n"
			"li x5, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x0 << TENSOR_IMA_B_NUM_COLS) | (0x7 << TENSOR_IMA_A_NUM_ROWS) | (0xa << TENSOR_IMA_A_NUM_COLS) | (0xb << TENSOR_IMA_OFFSET) | (0x0 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x11 << TENSOR_IMA_SCP_LOC_B) | (0x41 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x0 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ITER_3_HID_0:\n"
			"li x5, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x3 << TENSOR_FMA16_B_NUM_COLS) | (0xa << TENSOR_FMA16_A_NUM_ROWS) | (0x8 << TENSOR_FMA16_A_NUM_COLS) | (0xc << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0xbe << TENSOR_FMA16_SCP_LOC_B) | (0x5e << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x0 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ITER_4_HID_0:\n"
			"li x5, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x3 << TENSOR_FMA16_B_NUM_COLS) | (0x7 << TENSOR_FMA16_A_NUM_ROWS) | (0x8 << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x58 << TENSOR_FMA16_SCP_LOC_B) | (0x7b << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ITER_5_HID_0:\n"
			"li x5, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x2 << TENSOR_IMA_B_NUM_COLS) | (0xd << TENSOR_IMA_A_NUM_ROWS) | (0x2 << TENSOR_IMA_A_NUM_COLS) | (0x3 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x9c << TENSOR_IMA_SCP_LOC_B) | (0xfa << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x0 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_0\n"
			"csrw tensor_error, x0\n"
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_0\n"
			"la x31, check_sum_hid_0\n"
			"lw x5, (x31)\n"
			"bne x30, x5, LBL_FAIL_HID_0\n"
			"LBL_POSTAMBLE_HID_0:\n"          
			"csrwi tensor_wait, 0x0\n" 
			"csrwi tensor_wait, 0x1\n" 
			"csrwi tensor_wait, 0x2\n" 
			"csrwi tensor_wait, 0x3\n" 
			"csrwi tensor_wait, 0x4\n" 
			"csrwi tensor_wait, 0x5\n" 
			"csrwi tensor_wait, 0x6\n" 
			"csrwi tensor_wait, 0x7\n" 
			"csrwi tensor_wait, 0x8\n" 
			"csrwi tensor_wait, 0x9\n" 
			"csrwi tensor_wait, 0xa\n" 
			"fence\n"
			"li x5,  0x1ff800000610007\n" 
			"csrw tensor_fma, x5\n" 
			"csrwi tensor_wait, 7\n" 
			"csrwi tensor_wait, 0x6\n" 
			"fence\n"
			"csrwi tensor_wait, 0x6\n" 
			"csrrci x0, mcache_control, 2\n"
			"csrwi tensor_wait, 0x6\n" 
			"fence\n"
			"csrrci x0, mcache_control, 1\n"
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
			"LBL_HPM_3_CORE_TFMA_WAIT_TENB_FAIL_HID_0:\n"          
			"csrr x5, mhpmcounter3\n"
			"sd x5, 32 (x31)\n"
			"LBL_HPM_4_CORE_TIMA_OPS_FAIL_HID_0:\n"          
			"csrr x5, mhpmcounter4\n"
			"sd x5, 40 (x31)\n"
			"LBL_HPM_5_CORE_TFMA_INST_FAIL_HID_0:\n"          
			"csrr x5, mhpmcounter5\n"
			"sd x5, 48 (x31)\n"
			"LBL_HPM_6_CORE_RETIRED_INST0_FAIL_HID_0:\n"          
			"csrr x5, mhpmcounter6\n"
			"sd x5, 56 (x31)\n"
			"LBL_HPM_7_NEIGH_TX_COOP_TST_REQ_FAIL_HID_0:\n"          
			"csrr x5, mhpmcounter7\n"
			"sd x5, 64 (x31)\n"
			"LBL_HPM_8_NEIGH_TX_COOP_TLD_REQ_FAIL_HID_0:\n"          
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
			"LBL_HPM_3_CORE_TFMA_WAIT_TENB_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter3\n"
			"sd x5, 32 (x31)\n"
			"LBL_HPM_4_CORE_TIMA_OPS_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter4\n"
			"sd x5, 40 (x31)\n"
			"LBL_HPM_5_CORE_TFMA_INST_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter5\n"
			"sd x5, 48 (x31)\n"
			"LBL_HPM_6_CORE_RETIRED_INST0_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter6\n"
			"sd x5, 56 (x31)\n"
			"LBL_HPM_7_NEIGH_TX_COOP_TST_REQ_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter7\n"
			"sd x5, 64 (x31)\n"
			"LBL_HPM_8_NEIGH_TX_COOP_TLD_REQ_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter8\n"
			"sd x5, 72 (x31)\n"
			"slti x0, x0, 0x7fe\n"
			VSNIP_RSV
		);
		__asm__ __volatile__ ( 
			"LBL_HALT_HID_0:\n"          
			".global LBL_HALT_HID_0\n"          
			VSNIP_RSV
		);
	};
		C_TEST_PASS;
		return 0;       
}
volatile uint32_t rand_int_32[256] __attribute__ ((aligned (2048))) = {
		0x1edb3d2f, 
		0xd74b25d7, 
		0xe2fa719e, 
		0x0059d8d4, 
		0x8f19b1c1, 
		0x940e27f5, 
		0xac9ae9bb, 
		0x566cb844, 
		0xf90a51bd, 
		0x3d594a88, 
		0xe6a9bf6e, 
		0xb9559104, 
		0x6a366ed2, 
		0x06c5936a, 
		0xe992370e, 
		0xdbdbe284, 
		0xeeaa32f0, 
		0x44b610f1, 
		0xe06f5e5a, 
		0x96888a69, 
		0x457feca8, 
		0xc94d4b88, 
		0x8cce0795, 
		0xc780b5ab, 
		0xe9c5c565, 
		0xaef4cd53, 
		0x9f714c03, 
		0xde18477d, 
		0xde33281c, 
		0xacb5f039, 
		0x761347ff, 
		0xea2bca79, 
		0x58f87f10, 
		0xc848756e, 
		0x11ea7149, 
		0x53f5e42b, 
		0x4c2bbd7a, 
		0x51e87bf0, 
		0xc78a916d, 
		0xb90e16ec, 
		0x354161d7, 
		0xe6f10b4f, 
		0x6b93bded, 
		0xd73be961, 
		0x06688b96, 
		0xe42da43a, 
		0x2ba58e9f, 
		0xe7dc5ca4, 
		0x4614d70b, 
		0x088bb585, 
		0x27fe6a21, 
		0x10611a25, 
		0x7f062ae4, 
		0x664aaa98, 
		0x5c640541, 
		0x35c1e2d3, 
		0xad650ce2, 
		0xa312f7d9, 
		0x0c748af0, 
		0xc86f6419, 
		0x3835482a, 
		0x37807425, 
		0xec997070, 
		0x253996b4, 
		0x19e7adc8, 
		0x0fa76c11, 
		0x8e71323f, 
		0x898fbb47, 
		0x7745b873, 
		0xeab9355c, 
		0x2d598651, 
		0x7c758bba, 
		0xc5727dab, 
		0x47e54623, 
		0x0839a45b, 
		0x1206a8c0, 
		0x995af166, 
		0x1920dedc, 
		0x32a57c66, 
		0xb2243c0b, 
		0x76681854, 
		0x91aa84c7, 
		0x4245bae6, 
		0x2907f415, 
		0x20123aff, 
		0x12196472, 
		0x8f2810fe, 
		0x37140b03, 
		0x7ac23de5, 
		0x9c5312e2, 
		0x6ac9f6d9, 
		0x36502318, 
		0x47bafc23, 
		0xb90b6dc3, 
		0xe4880028, 
		0xf993a043, 
		0x422edf22, 
		0x15a838bd, 
		0x17320c83, 
		0xb37b6e0f, 
		0x2b9773e6, 
		0x82cba349, 
		0x04eaddce, 
		0xe33056a2, 
		0x0fd8be7b, 
		0x1ff802cf, 
		0xcceb5e2e, 
		0xb0aa19fe, 
		0xa67e3c4d, 
		0x9a05b6b6, 
		0x6cd5f5bb, 
		0xc1e1bebb, 
		0x59c47dde, 
		0xf40302be, 
		0x981e77b6, 
		0xa6b7f1fa, 
		0x63b8b287, 
		0xef530d2c, 
		0x7f72ed51, 
		0xd421e31c, 
		0x5a7b81f5, 
		0xd02f3379, 
		0x401079b4, 
		0x1588c7ea, 
		0x9126cbf0, 
		0x73bd4359, 
		0x28deea2e, 
		0x3b09c4ee, 
		0x63e694e7, 
		0x8a335518, 
		0x8346f6b6, 
		0xa1707996, 
		0x2cac5d25, 
		0x737cae02, 
		0x740c6bde, 
		0x87ff1b78, 
		0x74780c7f, 
		0xe5773ecd, 
		0xa9b33192, 
		0xa459b6c5, 
		0xb45eaad6, 
		0xeb2d1448, 
		0x8e3aefd2, 
		0x21db2dcc, 
		0xb9e95577, 
		0x72292c13, 
		0x3a948e52, 
		0xd4f9203f, 
		0x3d44ae5f, 
		0x93ffd66f, 
		0xeb00b5d7, 
		0xc9992255, 
		0x032e5344, 
		0xb2fd20e1, 
		0x15cd0b39, 
		0xca478a4e, 
		0x3618bbb4, 
		0x3203ef5b, 
		0x74354528, 
		0x4b410f54, 
		0x5ec9edda, 
		0x6e547919, 
		0x18e06b59, 
		0x83f30a47, 
		0x45b54c5f, 
		0x1151b27b, 
		0xcaa9f78d, 
		0x09cd8e2a, 
		0xb4d4bd89, 
		0x7bcd43f7, 
		0xc1e73daa, 
		0x90dd3827, 
		0xa3512a62, 
		0x31792d0a, 
		0x796180dd, 
		0xf0dfd161, 
		0xa9d9dcf5, 
		0x3491c9f3, 
		0x376be4fa, 
		0x147d3815, 
		0xc45c0e56, 
		0xa185b48f, 
		0x7829fb5b, 
		0x417e2d25, 
		0x8275d541, 
		0x22b369b7, 
		0x808c33c0, 
		0xc73156bf, 
		0x2bac0fb6, 
		0x748326f9, 
		0xc57deb18, 
		0x0971b413, 
		0x289da1b2, 
		0x3ce7a77a, 
		0x87d7b95e, 
		0xe43efd64, 
		0x3614f866, 
		0x4504a01b, 
		0x8e241fbe, 
		0x215ffb04, 
		0x57e0f1f2, 
		0x3fc99f68, 
		0x60c0c7a6, 
		0x8a5f574e, 
		0x72d09ce1, 
		0xa4b79fed, 
		0x08f1d79e, 
		0x36a3be6f, 
		0x637dce49, 
		0x8bde3e0e, 
		0x91c2a375, 
		0xf89ce800, 
		0xb219edab, 
		0xa721102f, 
		0xe4b445b7, 
		0x23e97f0f, 
		0x9e314a88, 
		0xfd8ce569, 
		0xf2422577, 
		0xb9f5c8ff, 
		0x2caab63c, 
		0xe7a3b9ef, 
		0xd86eb07e, 
		0x3092f38e, 
		0xa4d3dd28, 
		0xc7e89be1, 
		0x3bbf8ba8, 
		0x836c67ab, 
		0xf327c0c2, 
		0xc8bd4609, 
		0x5c1a2417, 
		0x301420f2, 
		0x99f2c52b, 
		0xec4c71ea, 
		0x903ed977, 
		0xac26334c, 
		0x2ff0e480, 
		0x4a1b8abc, 
		0xc2ac633a, 
		0x57235a5e, 
		0xc83a783a, 
		0x2da4ce7a, 
		0x5577ad2f, 
		0xd559acfe, 
		0xf34e4bfc, 
		0xed78d0c9, 
		0x311009bb, 
		0x45580475, 
		0x6c34a9fa, 
		0xc38854c8, 
		0x0d263fe9, 
		0x7b3a6927, 
		0xf8457c18, 
		0xcc52e655, 
		0x4705ac17, 
		0x1a415af0  
};
volatile uint32_t rand_ieee754_32[1536] __attribute__ ((aligned (2048))) = {
		0x7f7ffffe, 
		0xc22c0000, 
		0xc1f00000, 
		0x00000008, 
		0x7f800000, 
		0xff000000, 
		0x00004000, 
		0x00800000, 
		0xc3d2cc46, 
		0xbc085d11, 
		0x3f800001, 
		0x00000000, 
		0x6654d9df, 
		0x0f7ffffd, 
		0xff7ffffe, 
		0x00175a04, 
		0x2aebf6d5, 
		0xff800000, 
		0x4de6f8b3, 
		0x00ffffff, 
		0x4b000000, 
		0x79c363d1, 
		0x42240000, 
		0x7f7ffffe, 
		0x0f7ffffd, 
		0xc1300000, 
		0x41600000, 
		0x5c7b0336, 
		0x7f95e7dc, 
		0xc2380000, 
		0x42600000, 
		0x00800003, 
		0x80000000, 
		0x00040000, 
		0x4a5abd21, 
		0xd99553ad, 
		0x00000000, 
		0x0c7ff000, 
		0x3f800001, 
		0x0e00001f, 
		0x2cf4a7fc, 
		0x7f7ffffe, 
		0xc1400000, 
		0x19ff0ddc, 
		0xc0400000, 
		0x40c00000, 
		0x42100000, 
		0xb88c33f2, 
		0x007fffff, 
		0x9b863920, 
		0x0c7c0000, 
		0x4e5bc07d, 
		0x46ae45d8, 
		0x00800000, 
		0x80000020, 
		0x80000000, 
		0x8f7ffffd, 
		0x00000000, 
		0x41800000, 
		0xe2d411a8, 
		0x80000001, 
		0x7f7ffffe, 
		0x00000001, 
		0xfd571e76, 
		0x07809b25, 
		0xc1d80000, 
		0x72df9e35, 
		0x41800000, 
		0x4ecf4726, 
		0x1d1a620a, 
		0x502fee8c, 
		0x00800002, 
		0xc1500000, 
		0x00000000, 
		0xfce50ad1, 
		0x00000000, 
		0xc1000000, 
		0xc2140000, 
		0xe33eb861, 
		0xc2580000, 
		0xff880bf3, 
		0x3f390435, 
		0x41900000, 
		0xc2180000, 
		0x7fc00001, 
		0xff800000, 
		0x7f7fffff, 
		0xbf800000, 
		0xff000000, 
		0x0c7fe000, 
		0x80001000, 
		0x00000002, 
		0x0d00fff0, 
		0x80020000, 
		0x0c7fc000, 
		0xc1e00000, 
		0xff7ffffe, 
		0x42ff6577, 
		0x470e1fe5, 
		0xaac366ae, 
		0x420c0000, 
		0x00800002, 
		0xff7ffffe, 
		0x17c5d53c, 
		0x47654ecc, 
		0x50b381cd, 
		0x7f800000, 
		0x800f89e3, 
		0xc0400000, 
		0x40400000, 
		0x80ec931b, 
		0x04771491, 
		0xc2140000, 
		0xeac7cfa8, 
		0x7f7fffff, 
		0x0f7ffffc, 
		0x7f7ffffe, 
		0x424c0000, 
		0x80800003, 
		0x0f7ffffc, 
		0xc1500000, 
		0x0c7f8000, 
		0x41400000, 
		0xc1e00000, 
		0xfdb96987, 
		0x00080000, 
		0x00227820, 
		0x526fd0ce, 
		0xb4215c59, 
		0xf1595a48, 
		0x00ffffff, 
		0xa68378e6, 
		0xd10ba32e, 
		0xbe007490, 
		0xcb4d8a31, 
		0xe1c3366b, 
		0xff800000, 
		0xc1e80000, 
		0x3f800000, 
		0xc1980000, 
		0x80000080, 
		0x118bea45, 
		0xc1400000, 
		0x789c92c0, 
		0x7f800000, 
		0x0c780000, 
		0x7f800000, 
		0x00800001, 
		0x0c7ffff8, 
		0x7fc00000, 
		0x42300000, 
		0x7f928c0c, 
		0x80800000, 
		0xc2000000, 
		0xff7ffffe, 
		0x41e00000, 
		0x0e001fff, 
		0x4d9750d9, 
		0xffc00000, 
		0x80000000, 
		0x0e000fff, 
		0x00000008, 
		0xe503bee9, 
		0x00000002, 
		0x42740000, 
		0xc94b0f1b, 
		0x7d375d12, 
		0x80800002, 
		0xb2aca1e1, 
		0x80080000, 
		0x41a80000, 
		0x0c7fc000, 
		0x00002000, 
		0xc2500000, 
		0x7fbeb7ae, 
		0xa2acb6a1, 
		0xc1efbd10, 
		0x00000002, 
		0xc2300000, 
		0xc1b80000, 
		0xc2140000, 
		0x7f800000, 
		0xc2440000, 
		0xffbfffff, 
		0x41d80000, 
		0x00000000, 
		0x72ff718b, 
		0x3d0ef0e0, 
		0xb617f2c7, 
		0x80000000, 
		0xdc7f3db2, 
		0x80000004, 
		0xc1900000, 
		0x11ac4a59, 
		0x00800002, 
		0x00000800, 
		0xc1880000, 
		0x58d46734, 
		0x80800001, 
		0xdcb3e3e3, 
		0x53d29bf9, 
		0x80010000, 
		0x00000000, 
		0x0c7ffc00, 
		0xc1d80000, 
		0x7fc00001, 
		0x0e00007f, 
		0x41500000, 
		0x00000000, 
		0x3f7229fd, 
		0x7f800000, 
		0x0f7ffffd, 
		0x0e007fff, 
		0xffffffff, 
		0x96daccff, 
		0x40e00000, 
		0x0c7ffff8, 
		0x80001000, 
		0x0e000003, 
		0x0e8de173, 
		0x80200000, 
		0x7f88d66c, 
		0x1a8008ba, 
		0x424c0000, 
		0x0f7ffffe, 
		0xc1000000, 
		0xc2000000, 
		0x0c600000, 
		0xfce2d8f5, 
		0x6b0557b7, 
		0x00000000, 
		0x00000001, 
		0x80000000, 
		0xffbfffff, 
		0xd4f3cab2, 
		0xe45222b4, 
		0x41c80000, 
		0xff800000, 
		0x4f89cf2f, 
		0xc313d233, 
		0xc1500000, 
		0x23486e07, 
		0xc0d18423, 
		0x80000000, 
		0x424c0000, 
		0x0e007fff, 
		0xfd9fb0d9, 
		0x426c0000, 
		0xfff88795, 
		0x42240000, 
		0x80020000, 
		0xc26c0000, 
		0xa3bb841e, 
		0x42400000, 
		0x7fbd3dd8, 
		0x00000000, 
		0xc1100000, 
		0xff7ffffe, 
		0x57c52b19, 
		0xc0400000, 
		0xc1500000, 
		0x80000000, 
		0x80008000, 
		0xa4930776, 
		0x7f800000, 
		0xc2340000, 
		0x0c7ffe00, 
		0xff7fffff, 
		0x80000000, 
		0x8f59d411, 
		0x00004000, 
		0x41900000, 
		0x7f7fffff, 
		0xc1700000, 
		0x65172a72, 
		0x2b2c98a2, 
		0xb44a7a06, 
		0xff800000, 
		0x5e4e6a15, 
		0x3f028f5c, 
		0x6b861907, 
		0xc1600000, 
		0xc2580000, 
		0xc1880000, 
		0x41f80000, 
		0xf24cb3c2, 
		0x00000400, 
		0x41d80000, 
		0x7f000000, 
		0xff800000, 
		0x0c7fc000, 
		0xe32d606e, 
		0xfa48bf41, 
		0x00000000, 
		0x41900000, 
		0xaaf439b5, 
		0x80000000, 
		0xb09a79a3, 
		0x00001000, 
		0x00000002, 
		0x80000004, 
		0xb12fe21b, 
		0x0e3fffff, 
		0x0e1fffff, 
		0x7b771a56, 
		0x0c7e0000, 
		0x80040000, 
		0x00011111, 
		0xc0400000, 
		0x42340000, 
		0x41900000, 
		0x807fffff, 
		0x41980000, 
		0x0c7ffc00, 
		0xc2480000, 
		0xc22c0000, 
		0x00000100, 
		0xc0800000, 
		0x427c0000, 
		0x0f7fffff, 
		0x00000080, 
		0xff800001, 
		0x00755be5, 
		0xc2180000, 
		0x42300000, 
		0x00100000, 
		0xc2400000, 
		0x41100000, 
		0xff000000, 
		0xff800000, 
		0xc332efa2, 
		0x00800002, 
		0xa52f73d7, 
		0x80000000, 
		0xffbfffff, 
		0x0e001fff, 
		0x00000000, 
		0x00800000, 
		0xc2100000, 
		0x40e00000, 
		0xf7b731ab, 
		0xbf028f5c, 
		0x41a80000, 
		0x0e000007, 
		0x80010000, 
		0x42280000, 
		0x0c7fe000, 
		0x00000010, 
		0xffc00000, 
		0x00000000, 
		0x80800001, 
		0x0c400000, 
		0xbd26a9e4, 
		0x0e007fff, 
		0xc1300000, 
		0x80000000, 
		0x80000020, 
		0x9a6545cd, 
		0x80000800, 
		0x7defe8d9, 
		0x00000008, 
		0xc2740000, 
		0x00800001, 
		0xffc00001, 
		0xfab490b4, 
		0xb48ad5f7, 
		0xc1200000, 
		0x324b9ef6, 
		0xc1e80000, 
		0x9239db90, 
		0x0c7fff80, 
		0x80004000, 
		0x80200000, 
		0x00000040, 
		0xc1000000, 
		0x2d720a6d, 
		0xd16010a1, 
		0xc2740000, 
		0x41c00000, 
		0x1f527681, 
		0x67c35b0b, 
		0xff000000, 
		0x6d3025dd, 
		0xc0e00000, 
		0x4a64f06b, 
		0x6ffae7bc, 
		0x80000000, 
		0xfd2649ba, 
		0x42580000, 
		0xc2000000, 
		0x0c7f8000, 
		0x05bd8764, 
		0x39ef3e07, 
		0x00000800, 
		0xff000000, 
		0x00800000, 
		0x0c780000, 
		0x0c7ff000, 
		0x80000000, 
		0x0f550f96, 
		0x80000100, 
		0x80010000, 
		0x41e80000, 
		0x96d9f5cb, 
		0x80000000, 
		0x00000000, 
		0x422c0000, 
		0x2fc81c35, 
		0x00000001, 
		0x41800000, 
		0xae2cfbf8, 
		0x0e000001, 
		0x42140000, 
		0xff869e9c, 
		0x0064c5d8, 
		0x41f00000, 
		0xff800000, 
		0x00800002, 
		0xc0c00000, 
		0xc2000000, 
		0x7fbfffff, 
		0x42280000, 
		0x974a0dca, 
		0x42400000, 
		0x0e00000f, 
		0x031f3d47, 
		0x4b8c4b40, 
		0x00400000, 
		0x407c708f, 
		0x80000020, 
		0x67675104, 
		0x00800003, 
		0x42640000, 
		0x41880000, 
		0xaded2ab7, 
		0x42580000, 
		0x6dfcc2e6, 
		0x42680000, 
		0x43972a28, 
		0x8f7ffffc, 
		0xad6a3bdd, 
		0x41800000, 
		0x80000200, 
		0x7f800000, 
		0x0e00007f, 
		0x00000001, 
		0x9ffb844b, 
		0x80000080, 
		0xc1db2be3, 
		0x80000100, 
		0x0e000fff, 
		0x00000000, 
		0x0e07ffff, 
		0x2f330a83, 
		0xc2780000, 
		0x2f26d88d, 
		0x090f4d98, 
		0xe0b4bc0b, 
		0x00000010, 
		0xc2440000, 
		0xd23b3bca, 
		0x00000010, 
		0xc1400000, 
		0x00000000, 
		0x41b00000, 
		0xc0e00000, 
		0x80800000, 
		0x00200000, 
		0x41600000, 
		0x97d48420, 
		0x41100000, 
		0xff7fffff, 
		0x5eed9d53, 
		0x4b8c4b40, 
		0x0c7ff000, 
		0x41800000, 
		0x73318f61, 
		0xbf028f5c, 
		0xbeefe7eb, 
		0x2fdb476e, 
		0x0c7ff800, 
		0x35df0bfb, 
		0xff800001, 
		0xc2480000, 
		0x0c7ffc00, 
		0xff800000, 
		0x41880000, 
		0x7f800000, 
		0x7f8c1897, 
		0x51c31f7e, 
		0xfae3f0b3, 
		0x352c1001, 
		0x00800003, 
		0x6b6cdf7f, 
		0x3f800000, 
		0x807ffffe, 
		0xcb8c4b40, 
		0xc0c00000, 
		0x00800000, 
		0x6a7d30eb, 
		0x34273ba8, 
		0x422c0000, 
		0x0c400000, 
		0x2959fa0a, 
		0x80000000, 
		0x8f7ffffe, 
		0xc2240000, 
		0xffb8be30, 
		0xc1100000, 
		0x42300000, 
		0x80200000, 
		0xc22c0000, 
		0x99315b22, 
		0x40400000, 
		0x80800000, 
		0x8924d93f, 
		0x00001000, 
		0x7f800000, 
		0x425d13c1, 
		0xc2200000, 
		0x41a80000, 
		0xffbfffff, 
		0xc1600000, 
		0xc1c00000, 
		0x33ffbf3f, 
		0xdfd22d2c, 
		0x38be3077, 
		0x33333333, 
		0xff920062, 
		0x030ef0b3, 
		0xc1d00000, 
		0x01bc0af1, 
		0x56fb86af, 
		0x0c7ffffe, 
		0x42180000, 
		0xc0a00000, 
		0x7f800000, 
		0x80020000, 
		0x00000000, 
		0x7f800000, 
		0x0e01ffff, 
		0xe3b48071, 
		0x8cc7ee40, 
		0x41c80000, 
		0x7f800000, 
		0xff8308ed, 
		0x3062de07, 
		0xa5765836, 
		0x0c7fff80, 
		0x80000080, 
		0x42780000, 
		0xcd6dfbb9, 
		0x7c5c09cc, 
		0x42000000, 
		0x38859e84, 
		0xcb8c4b40, 
		0x80800001, 
		0x4b000000, 
		0x4d517cfd, 
		0x80000000, 
		0x80800003, 
		0x80800000, 
		0xc2780000, 
		0xbbae88ef, 
		0x80000000, 
		0x80000000, 
		0xc2580000, 
		0x00000000, 
		0xffbfffff, 
		0x8a767b7a, 
		0x40a00000, 
		0xb0fb7706, 
		0x40000000, 
		0x40a00000, 
		0xffc00001, 
		0xc1880000, 
		0x41e80000, 
		0xc1e80000, 
		0x80000001, 
		0xff800000, 
		0xcb4aa77a, 
		0x0e000003, 
		0xc1880000, 
		0x80000100, 
		0xc2040000, 
		0xad08dc5c, 
		0x40000000, 
		0x0c7fff80, 
		0x7f7fffff, 
		0x00000001, 
		0x42040000, 
		0x00800001, 
		0x80000000, 
		0x420c0000, 
		0x7f87a893, 
		0x3f028f5c, 
		0x00004000, 
		0x496fc20c, 
		0x41a00000, 
		0x00008000, 
		0x42700000, 
		0x65f7831e, 
		0xc9186baa, 
		0x42500000, 
		0x00000000, 
		0xdf4fd237, 
		0x00000000, 
		0x0e03ffff, 
		0x41c80000, 
		0x23acde11, 
		0x7f800000, 
		0x80000040, 
		0x80000000, 
		0x0c400000, 
		0xcccccccc, 
		0x9fe33df3, 
		0xc1f00000, 
		0xd150ba04, 
		0x7f7fffff, 
		0x7f7ffffe, 
		0x80000040, 
		0x0e00003f, 
		0x00000100, 
		0x88af4dd1, 
		0xc1880000, 
		0x41d80000, 
		0xff7fffff, 
		0x88c059a4, 
		0x80000001, 
		0x42340000, 
		0xc2000000, 
		0x882b1d44, 
		0x7a85ad32, 
		0xc1600000, 
		0x556c86c8, 
		0xffc00000, 
		0x807fffff, 
		0x98d9932a, 
		0xc0a00000, 
		0x42400000, 
		0xff7ffffe, 
		0x4e16e6d1, 
		0x3d1ad4a4, 
		0xc0800000, 
		0x0e00ffff, 
		0xc1300000, 
		0x00000001, 
		0x3eaa2ae5, 
		0x0c7e0000, 
		0x80000010, 
		0xc2200000, 
		0x0e00001f, 
		0x80002000, 
		0x00800001, 
		0x0e0fffff, 
		0x004cc987, 
		0x7fc00001, 
		0x4d083bfd, 
		0x0d5db79b, 
		0x3e2a49ec, 
		0xffbfffff, 
		0x00000010, 
		0x41300000, 
		0x0e00001f, 
		0xe7fc2b63, 
		0x80000080, 
		0x41b80000, 
		0x0d000ff0, 
		0x0c7ff800, 
		0xdd24c92e, 
		0x424c0000, 
		0xbf028f5c, 
		0x00000200, 
		0x8f7ffffd, 
		0xc2340000, 
		0xc22c0000, 
		0x00000080, 
		0xe37d6487, 
		0xc2580000, 
		0x80800003, 
		0xb662ef20, 
		0x41700000, 
		0x00000020, 
		0xff800000, 
		0x807ffffe, 
		0x0c7e0000, 
		0xffbfffff, 
		0x9c40e48f, 
		0xdfebd211, 
		0x7f800000, 
		0x542115c9, 
		0xc2380000, 
		0x7f800000, 
		0x80000000, 
		0x0e00003f, 
		0xd4b81191, 
		0x00800000, 
		0x99b1b19e, 
		0x0e0007ff, 
		0x41600000, 
		0xffa4e37b, 
		0x0c7ffc00, 
		0x0304ff68, 
		0x80000000, 
		0xcccccccc, 
		0xcf5f9e9a, 
		0xa87ed777, 
		0xc941c352, 
		0x00000040, 
		0x14691bce, 
		0xc27c0000, 
		0x0c7fff80, 
		0x80000000, 
		0x00000001, 
		0x0c7ffe00, 
		0x3f800000, 
		0x2b5ea0e4, 
		0x80000100, 
		0x872f7806, 
		0x80000010, 
		0xdc60a381, 
		0x0f7ffffe, 
		0xff7ffffe, 
		0xb2709a35, 
		0x726616f4, 
		0x007ffffe, 
		0x80000800, 
		0x7fc00000, 
		0x42780000, 
		0x42300000, 
		0x806ea0cf, 
		0x41e80000, 
		0x00400000, 
		0xff800000, 
		0x149cfb8c, 
		0x41a80000, 
		0x80020000, 
		0x5561c814, 
		0x951e610a, 
		0x0c7fff00, 
		0xc2380000, 
		0x0c400000, 
		0x3f028f5c, 
		0x80080000, 
		0x00000000, 
		0x8f7fffff, 
		0x80040000, 
		0xd077c2d3, 
		0x0c7fffc0, 
		0x0e000003, 
		0xff7ffffe, 
		0x0c600000, 
		0x80800001, 
		0x00000040, 
		0x007fffff, 
		0xc1f00000, 
		0x0c700000, 
		0x774798b4, 
		0x5dfcaed8, 
		0x41d00000, 
		0x0e0001ff, 
		0x41300000, 
		0x55555555, 
		0x3f800000, 
		0xab2ec75b, 
		0xff877482, 
		0xc2380000, 
		0xc2780000, 
		0x528ae8e5, 
		0xc1400000, 
		0x0e000007, 
		0x7f7ffffe, 
		0xc2140000, 
		0xc2200000, 
		0xbf800000, 
		0x80800000, 
		0x80001000, 
		0xc6140784, 
		0xc1f80000, 
		0xd056c41c, 
		0x8624cd7e, 
		0x42040000, 
		0xff800000, 
		0x5ea33c63, 
		0x4d616aca, 
		0xff815e67, 
		0x0c7ffc00, 
		0x05b143b4, 
		0x00020000, 
		0x0e00ffff, 
		0x007ffffe, 
		0xc1e80000, 
		0x0e003fff, 
		0x7f800000, 
		0x42140000, 
		0x30815541, 
		0x6f795d45, 
		0xbe7b7d19, 
		0x0c7fc000, 
		0xc0a00000, 
		0x1740c0bb, 
		0xc1100000, 
		0x3c8b369f, 
		0x41400000, 
		0xa9acb9aa, 
		0x42580000, 
		0x80000000, 
		0x41a80000, 
		0xff869164, 
		0xae3b2ebb, 
		0xae52a9e2, 
		0x0c7fff80, 
		0x95d5e3f7, 
		0xc2680000, 
		0x41700000, 
		0x42680000, 
		0x0c7ff800, 
		0xc47aa31b, 
		0x7f800000, 
		0x87fa8b3b, 
		0xc2500000, 
		0x80000000, 
		0x00000000, 
		0x41a00000, 
		0x960258e0, 
		0x42640000, 
		0x0e000fff, 
		0xc1f80000, 
		0x8f7ffffd, 
		0x80800002, 
		0xc2580000, 
		0x4b391543, 
		0x7fac9b54, 
		0xc27c0000, 
		0x7f7ffffe, 
		0x7fffffff, 
		0x2eb4c07f, 
		0x3f028f5c, 
		0x007ffffe, 
		0x00000010, 
		0x80004000, 
		0x0e0007ff, 
		0x8d47d1c2, 
		0xc24c0000, 
		0x0e1fffff, 
		0xff7ffffe, 
		0x421c0000, 
		0xc2280000, 
		0xe5820e30, 
		0x00000004, 
		0xe08e9080, 
		0xc2140000, 
		0xf7907f47, 
		0x0e0fffff, 
		0x5a9e27aa, 
		0x80800001, 
		0xc2280000, 
		0xc24c0000, 
		0x41c00000, 
		0x00000000, 
		0x80000001, 
		0x62bc92f2, 
		0x0e01ffff, 
		0xc2540000, 
		0x00200000, 
		0xc2000000, 
		0x41800000, 
		0xe229f91a, 
		0xff800000, 
		0xff84e828, 
		0xe5e7f0c1, 
		0x0c7fc000, 
		0xfa1f36a0, 
		0x8f7ffffc, 
		0x40800000, 
		0x0c7ffff8, 
		0xff800000, 
		0x0e003fff, 
		0xc217d036, 
		0xc1282869, 
		0x41d00000, 
		0xaf6d1797, 
		0x80000400, 
		0x7fc00000, 
		0x69d1cca4, 
		0x41b00000, 
		0x40c00000, 
		0xc2040000, 
		0x0e001fff, 
		0x41f00000, 
		0xc1c00000, 
		0xff7ffffe, 
		0xc2300000, 
		0x0f7ffffc, 
		0x0e000003, 
		0x99a4456d, 
		0x3f800001, 
		0xc2100000, 
		0xc1b00000, 
		0x0e000fff, 
		0x0e01ffff, 
		0x41100000, 
		0x42040000, 
		0x80000004, 
		0xc9863624, 
		0x3f800000, 
		0x41100000, 
		0x7f800001, 
		0x00000010, 
		0x80f37602, 
		0xe2e315df, 
		0x89ebb01a, 
		0x12be8fb3, 
		0xe0102235, 
		0x00000004, 
		0xc2340000, 
		0x0c7fffc0, 
		0x80000000, 
		0x41800000, 
		0x920320c4, 
		0x91ff3ce8, 
		0xc1800000, 
		0x2f5ba406, 
		0xff800000, 
		0x1bd8b766, 
		0x00000000, 
		0x7f800000, 
		0xc2400000, 
		0x8dc11982, 
		0x40e00000, 
		0x8137d27e, 
		0x48db51b1, 
		0xaac3471b, 
		0x80000000, 
		0x80800001, 
		0x7f800000, 
		0x42380000, 
		0xc2600000, 
		0x80000000, 
		0x3f800001, 
		0x00400000, 
		0xc1900000, 
		0x8e05d027, 
		0x3f028f5c, 
		0xcebaaf23, 
		0x4f935058, 
		0xc1f00000, 
		0x0e0007ff, 
		0xaf4b7488, 
		0x0f7ffffc, 
		0xc25c0000, 
		0x00000100, 
		0x9acc16b0, 
		0xffa732b2, 
		0xa06589cb, 
		0xbf800001, 
		0x769f0632, 
		0x6ebeb082, 
		0xc1000000, 
		0x00000000, 
		0x41e80000, 
		0xc1200000, 
		0xc1600000, 
		0x7f800000, 
		0x80800002, 
		0x664d20f6, 
		0xd77a9c9b, 
		0x7f800000, 
		0x926a181e, 
		0x030f31a0, 
		0x807ffffe, 
		0x7f800000, 
		0x42680000, 
		0xc1700000, 
		0x80800000, 
		0xc2200000, 
		0xc1700000, 
		0x41f80000, 
		0x0c7fffe0, 
		0x80000000, 
		0x00000000, 
		0xc22c0000, 
		0x7fc00001, 
		0xbcca0d6d, 
		0xff7ffffe, 
		0x9df96489, 
		0xb1f17c57, 
		0x80000000, 
		0xffc00000, 
		0x4cc3ec06, 
		0x7f800000, 
		0x42240000, 
		0x00000000, 
		0x80000000, 
		0x86c1d111, 
		0x7f800000, 
		0x80000000, 
		0x41f00000, 
		0x80000000, 
		0xbf028f5c, 
		0x0c7fe000, 
		0xffac3697, 
		0x41e80000, 
		0x8676fc94, 
		0x42700000, 
		0x4902d860, 
		0x80000000, 
		0x4bab14e8, 
		0xffc00000, 
		0x00000000, 
		0xdf76ca84, 
		0x80000010, 
		0xc2640000, 
		0xc2040000, 
		0x0c700000, 
		0x522667d7, 
		0x80000002, 
		0x0c7ff000, 
		0x80000000, 
		0x0391d82c, 
		0xc1a00000, 
		0x00000040, 
		0x80010000, 
		0x00400000, 
		0x00400000, 
		0xc2580000, 
		0x782ad4b0, 
		0x0e0000ff, 
		0x7f800000, 
		0x423c0000, 
		0xc4541711, 
		0x1f3f5902, 
		0x8f7ffffc, 
		0xc0400000, 
		0x807fffff, 
		0x3f800000, 
		0x00000002, 
		0x14fbcc7e, 
		0x80011111, 
		0xff7ffffe, 
		0xe1975a7d, 
		0xfd5f5331, 
		0xc2180000, 
		0x3171db92, 
		0xff800000, 
		0x0d00fff0, 
		0x00080000, 
		0xff7ffffe, 
		0xcb03e804, 
		0x80000001, 
		0x0e0000ff, 
		0xc2440000, 
		0xaaaaaaaa, 
		0x80000002, 
		0x7f800000, 
		0x42040000, 
		0xce849f28, 
		0x00040000, 
		0x80000002, 
		0x40400000, 
		0xc1f00000, 
		0x320bc70c, 
		0xc1980000, 
		0x42780000, 
		0x00200000, 
		0x0e007fff, 
		0xcb000000, 
		0x00000000, 
		0x7f800000, 
		0x0fdad992, 
		0x80011111, 
		0xb128d482, 
		0xd28019ad, 
		0xcd0d1740, 
		0x4b8c4b40, 
		0x7fbfffff, 
		0xc2100000, 
		0x0c7fe000, 
		0x001dacb6, 
		0xff7fffff, 
		0x00001000, 
		0x00004000, 
		0x42680000, 
		0x2d377de2, 
		0x29472094, 
		0x00000000, 
		0xa32a2653, 
		0xb105d536, 
		0x00010000, 
		0x1973b212, 
		0xd4231a03, 
		0xb831c31b, 
		0x21d19b92, 
		0xc2440000, 
		0x00800002, 
		0xc4098978, 
		0x0c7ffffe, 
		0xc2580000, 
		0x0e07ffff, 
		0x42140000, 
		0x00000000, 
		0xc2080000, 
		0x42240000, 
		0x13382877, 
		0xff800000, 
		0xc1600000, 
		0x80800000, 
		0x7e17c64d, 
		0xb9613d5e, 
		0x41b80000, 
		0x41d80000, 
		0x0e0fffff, 
		0x7fc00001, 
		0xff800000, 
		0x42740000, 
		0x41d00000, 
		0xadf92bb1, 
		0xe7da5d7c, 
		0xa5bb8a16, 
		0x41e00000, 
		0x0e00000f, 
		0x42540000, 
		0xe20a75a7, 
		0x00000000, 
		0x0c600000, 
		0xc20c0000, 
		0xc0800000, 
		0x8f7ffffe, 
		0xc24c0000, 
		0x0c7ff800, 
		0x00000000, 
		0x0e000fff, 
		0xff800000, 
		0xc1f80000, 
		0x41700000, 
		0x807ffffe, 
		0x20bce516, 
		0xc0c00000, 
		0x42100000, 
		0x55555555, 
		0x0555f1c0, 
		0x80000001, 
		0x80002000, 
		0x00000004, 
		0x7f000000, 
		0xc1e80000, 
		0x5eb41f91, 
		0x00000000, 
		0x00000000, 
		0x0c7fff80, 
		0x00000040, 
		0x40a00000, 
		0x0e0007ff, 
		0x7a0723d0, 
		0x8a473183, 
		0x00000100, 
		0x20a2ddb0, 
		0x00800000, 
		0x42740000, 
		0x80000000, 
		0x7f7ffffe, 
		0xc1f80000, 
		0x00800001, 
		0x41900000, 
		0xfd103493, 
		0x00010000, 
		0xff800000, 
		0x80000010, 
		0x00000000, 
		0x386a080f, 
		0xe99c9c70, 
		0xc1c80000, 
		0x41c00000, 
		0x5cbdee08, 
		0x9bb0909f, 
		0x00ffffff, 
		0x0e03ffff, 
		0x00ffffff, 
		0xc2280000, 
		0x80800000, 
		0xe6539aed, 
		0x940c6cbb, 
		0x0e0000ff, 
		0xc2000000, 
		0xc2540000, 
		0x41300000, 
		0xffd8fa31, 
		0x80800003, 
		0x55555555, 
		0x80000000, 
		0xc1980000, 
		0x0f7ffffe, 
		0x06046ed8, 
		0x41600000, 
		0x41600000, 
		0x80ffffff, 
		0x00000001, 
		0x427c0000, 
		0x7f7ffffe, 
		0xff841f9e, 
		0x3ece61bb, 
		0x8357f58b, 
		0xd0fdad16, 
		0xad12396d, 
		0xffc00000, 
		0x07443167, 
		0x41000000, 
		0x42140000, 
		0xc2440000, 
		0x3e869eb0, 
		0x862ffd3d, 
		0x1dcf6c5e, 
		0x3a83bfea, 
		0x80000010, 
		0x084b87bd, 
		0xff000000, 
		0x80000400, 
		0x00400000, 
		0xc2700000, 
		0x00800001, 
		0x8022e2ab, 
		0x06d2826d, 
		0x42580000, 
		0x0e1fffff, 
		0x80000000, 
		0xc27c0000, 
		0x9584331b, 
		0x0c7f8000, 
		0x000561b3, 
		0x00000020, 
		0x0c7fffff, 
		0x0e00001f, 
		0xc2180000, 
		0x80040000, 
		0x592690b9, 
		0x40000000, 
		0x55555555, 
		0x80000000, 
		0xe47559a5, 
		0x0c7fffe0, 
		0xf56e87d2, 
		0xc1d80000, 
		0xc2180000, 
		0x0e001fff, 
		0x41880000, 
		0x99e05775, 
		0x0e007fff, 
		0x41c80000, 
		0xc21c0000, 
		0x3b33b0a3, 
		0x42540000, 
		0x80000000, 
		0x7f7fffff, 
		0x80000200, 
		0xff7ffffe, 
		0xc0e00000, 
		0xddad4f58, 
		0xc2000000, 
		0x80010000, 
		0xcb8c4b40, 
		0xe09ddb02, 
		0xc1500000, 
		0x42780000, 
		0x42440000, 
		0xc1f80000, 
		0x7f800000, 
		0x95bcf931, 
		0xff7ffffe, 
		0x42540000, 
		0xc1d00000, 
		0x51a19c3c, 
		0x3f028f5c, 
		0xc2300000, 
		0x7e0a7a96, 
		0x50d6a9f2, 
		0xff800000, 
		0xc2440000, 
		0x7f800000, 
		0x7fc00001, 
		0xd8a804b6, 
		0x7f800000, 
		0x00000000, 
		0x7f000000, 
		0xc8d7f599, 
		0x40c00000, 
		0x41880000, 
		0x42400000, 
		0x42400000, 
		0x41d80000, 
		0xc2700000, 
		0xa0b1d45d, 
		0x0e00000f, 
		0x24e88f16, 
		0xcccccccc, 
		0xaa8756f3, 
		0x42440000, 
		0xff8e9ebe, 
		0x238db5b5, 
		0x910b7c9d, 
		0xffc00000, 
		0x80000000, 
		0xffcfeb69, 
		0xc2380000, 
		0xc2740000, 
		0x80000000, 
		0xc23c0000, 
		0x00000000, 
		0xc0400000, 
		0xa455cae6, 
		0x6df45132, 
		0x4fdf7eae, 
		0x40c00000, 
		0x0c700000, 
		0x00000002, 
		0x80000000, 
		0xd13c3bc2, 
		0xbeb8e476, 
		0x00400000, 
		0xff7fffff, 
		0xff800000, 
		0x3be286fd, 
		0x80001000, 
		0xd04aac20, 
		0xc22c0000, 
		0x67bb1f18, 
		0xc21c0000, 
		0x40800000, 
		0xc2280000, 
		0xc0e00000, 
		0x5b901982, 
		0x00000100, 
		0x8c5ce023, 
		0x1eccc3a3, 
		0x7fffffff, 
		0xc2640000, 
		0x4b000000, 
		0x3d1614f4, 
		0x95ea7928, 
		0x00000200, 
		0x42000000, 
		0x42580000, 
		0x00800000, 
		0x4005d5d5, 
		0x80001000, 
		0x523cb335, 
		0xffa36239, 
		0xff939d76, 
		0x6c6c7002, 
		0x007ffffe, 
		0xc1900000, 
		0x0e07ffff, 
		0x41400000, 
		0x00800002, 
		0x8bf17000, 
		0x80000002, 
		0x80800001, 
		0xfae92808, 
		0x426c0000, 
		0xfb53f6de, 
		0xc655421b, 
		0x29a79c24, 
		0xc2700000, 
		0x42580000, 
		0x0e0fffff, 
		0xff7fffff, 
		0x80000040, 
		0xc20c0000, 
		0x00800001, 
		0x0e000001, 
		0xff7ffffe, 
		0xc71548a9, 
		0x42280000, 
		0x7f800000, 
		0x42080000, 
		0x0e007fff, 
		0x0e0003ff, 
		0xc2040000, 
		0xc1b00000, 
		0x5b64f522, 
		0xa79175f6, 
		0xffa91b2e, 
		0x00800001, 
		0x424c0000, 
		0xd1f3aff6, 
		0x00800001, 
		0x34a76300, 
		0x66a6ea75, 
		0x41300000, 
		0xdd6fb4dd, 
		0x3a31b3f5, 
		0x420c0000, 
		0x7f800000, 
		0xffaf8493, 
		0x0e003fff, 
		0xafb8351f, 
		0x0e0001ff, 
		0x4b8c4b40, 
		0x0c7fff80, 
		0xc1b80000, 
		0x80800000, 
		0x41d80000, 
		0x0c400000, 
		0x00002000, 
		0x41b00000, 
		0x00000000, 
		0xc2300000, 
		0x00800000, 
		0x80800003, 
		0x0e007fff, 
		0x7fa1aa8e, 
		0xcccccccc, 
		0x41980000, 
		0x42640000, 
		0x7f800000, 
		0x00000400, 
		0xff7ffffe, 
		0xc1980000, 
		0x007ffffe, 
		0x0c7e0000, 
		0xd686efc2, 
		0x80080000, 
		0x7f7ffffe, 
		0x41900000, 
		0xc20c0000, 
		0x832d712f, 
		0x489425fe, 
		0x80000000, 
		0x0e001fff, 
		0x00000000, 
		0x342c4ed9, 
		0x0e003fff, 
		0x00000000, 
		0x260b9d4d, 
		0xffa2bd74, 
		0x1f2d0691, 
		0xddcf2c5f, 
		0x80000000, 
		0xc1400000, 
		0x0c7ffc00, 
		0x7f7fffff, 
		0xeb702069, 
		0x00000040, 
		0x538188b2, 
		0x434b2d92, 
		0x7f800000, 
		0x0c7ffff0, 
		0xa7e7da79, 
		0x622254df, 
		0x80ffffff, 
		0x725bdf91, 
		0x0f7ffffe, 
		0x135b3511, 
		0x1eec84a4, 
		0xffbfffff, 
		0x80000000, 
		0x42600000, 
		0xc91d37e7, 
		0x0c7fff00, 
		0x83012349, 
		0x63e0fcf7, 
		0x80200000, 
		0x00000000, 
		0xc25c0000, 
		0x7f91bd0a, 
		0x00800001, 
		0x16eaa663, 
		0x42580000, 
		0x00000001, 
		0x00000040, 
		0x240f841f, 
		0xff800000, 
		0x7924040d, 
		0x4a425c99, 
		0xc2700000, 
		0x51527b5d, 
		0x0f7fffff, 
		0x0c400000, 
		0x8f7ffffc, 
		0xd366a1af, 
		0x41e00000, 
		0x797fdc95, 
		0x426c0000, 
		0xff7ffffe, 
		0x5f49206e, 
		0x9f724315, 
		0x0d00fff0, 
		0x80000001, 
		0x427c0000, 
		0x41900000, 
		0xc2640000, 
		0xa2402f47, 
		0x0d00fff0, 
		0xff800000, 
		0x41600000, 
		0x426c0000, 
		0xffd11f75, 
		0x41900000, 
		0x974dcbed, 
		0x41f80000, 
		0x90a1c68c, 
		0x00008000, 
		0x426c0000, 
		0xc20c0000, 
		0x8b07d75c, 
		0x42340000, 
		0xe46735eb, 
		0x80000000, 
		0x1198d3a3, 
		0x7fc6b2b1, 
		0x8f7ffffe, 
		0x1a5bfc31, 
		0x7f7ffffe, 
		0xc1200000, 
		0x42040000, 
		0x0b3bdecd, 
		0x927557e0, 
		0x00100000, 
		0xff800000, 
		0x00000002, 
		0x9d0dd9fa, 
		0xc3da6afe, 
		0x00000020, 
		0x42000000, 
		0xc27c0000, 
		0xb0a7a562, 
		0x40e00000, 
		0xc27c0000, 
		0x7d9bde4d, 
		0xc0c00000, 
		0x41e80000, 
		0x39ffae6b, 
		0x0c7c0000, 
		0xfd2132ce, 
		0x7fc00000, 
		0x8ef197a3, 
		0x7f800000  
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
		0x000001ad, 
		0x000003a1, 
		0x000000ef, 
		0x0000013d, 
		0x000003b7, 
		0x0000011b, 
		0x0000015f, 
		0x0000017b, 
		0x0000028f, 
		0x0000004f, 
		0x00000379, 
		0x00000379, 
		0x0000008d, 
		0x0000026b, 
		0x000001a1, 
		0x000001e1, 
		0x00000159, 
		0x0000016d, 
		0x00000067, 
		0x00000261, 
		0x00000195, 
		0x000000ab, 
		0x000002a3, 
		0x00000203, 
		0x00000353, 
		0x000001f5, 
		0x000002cf, 
		0x0000019d, 
		0x00000119, 
		0x00000341, 
		0x000001bf, 
		0x0000015f, 
		0x00000061, 
		0x00000157, 
		0x00000229, 
		0x00000139, 
		0x00000007, 
		0x000002db, 
		0x0000000b, 
		0x000001b3, 
		0x00000349, 
		0x0000005f, 
		0x00000033, 
		0x0000039f, 
		0x00000381, 
		0x00000269, 
		0x000000f7, 
		0x00000015, 
		0x000001bb, 
		0x000002d3, 
		0x000001a5, 
		0x000002bd, 
		0x000003db, 
		0x0000005b, 
		0x000003c5, 
		0x0000024b, 
		0x000001b1, 
		0x0000000d, 
		0x00000233, 
		0x0000017f, 
		0x0000019b, 
		0x0000021f, 
		0x00000209, 
		0x00000223, 
		0x0000038d, 
		0x0000005f, 
		0x00000135, 
		0x00000021, 
		0x00000015, 
		0x000002bd, 
		0x00000211, 
		0x00000159, 
		0x0000019d, 
		0x00000143, 
		0x0000003d, 
		0x0000038f, 
		0x00000375, 
		0x000002c3, 
		0x0000030d, 
		0x00000375, 
		0x000003ef, 
		0x00000349, 
		0x00000035, 
		0x0000001d, 
		0x00000065, 
		0x000001cf, 
		0x0000010b, 
		0x00000397, 
		0x00000137, 
		0x00000189, 
		0x00000093, 
		0x00000043, 
		0x00000041, 
		0x0000028d, 
		0x000001d3, 
		0x0000028f, 
		0x00000327, 
		0x00000051, 
		0x000001a5, 
		0x000002e5, 
		0x000000e7, 
		0x0000034b, 
		0x000003e5, 
		0x00000331, 
		0x0000032b, 
		0x0000001d, 
		0x0000021d, 
		0x00000233, 
		0x000003bb, 
		0x00000223, 
		0x0000001f, 
		0x0000003f, 
		0x00000031, 
		0x000000e3, 
		0x00000201, 
		0x000000eb, 
		0x000001fb, 
		0x00000371, 
		0x000002ff, 
		0x0000014d, 
		0x000000ad, 
		0x00000217, 
		0x000002df, 
		0x0000023f, 
		0x0000018d, 
		0x000001f3, 
		0x0000039d, 
		0x0000035b, 
		0x00000223, 
		0x00000159, 
		0x0000012d, 
		0x0000039f, 
		0x00000317, 
		0x00000003, 
		0x0000035d, 
		0x0000014f, 
		0x0000038f, 
		0x00000365, 
		0x000000f7, 
		0x000000c5, 
		0x0000026f, 
		0x0000037b, 
		0x000000d1, 
		0x00000045, 
		0x0000031b, 
		0x00000363, 
		0x00000293, 
		0x000003ef, 
		0x00000237, 
		0x000002fb, 
		0x00000365, 
		0x00000265, 
		0x00000313, 
		0x00000061, 
		0x000002af, 
		0x00000021, 
		0x000002cf, 
		0x00000311, 
		0x00000257, 
		0x000003d9, 
		0x000002ef, 
		0x0000012f, 
		0x000002af, 
		0x00000225, 
		0x000002e3, 
		0x0000016b, 
		0x00000243, 
		0x000000d5, 
		0x00000157, 
		0x00000085, 
		0x00000195, 
		0x000002e1, 
		0x000001f9, 
		0x000002d3, 
		0x00000159, 
		0x00000149, 
		0x00000297, 
		0x000000f1, 
		0x0000006b, 
		0x0000000f, 
		0x00000031, 
		0x00000257, 
		0x0000011d, 
		0x0000039b, 
		0x00000365, 
		0x00000061, 
		0x000001e3, 
		0x00000207, 
		0x00000297, 
		0x000003b5, 
		0x00000367, 
		0x0000008b, 
		0x000001e5, 
		0x00000105, 
		0x00000291, 
		0x00000167, 
		0x00000331, 
		0x000001bd, 
		0x000001bd, 
		0x0000003f, 
		0x0000010b, 
		0x000000bb, 
		0x000001a7, 
		0x0000004d, 
		0x0000028b, 
		0x000002bf, 
		0x0000036d, 
		0x00000367, 
		0x00000279, 
		0x00000255, 
		0x00000127, 
		0x0000006d, 
		0x00000051, 
		0x000003a5, 
		0x000000e5, 
		0x00000313, 
		0x000000ad, 
		0x000000e3, 
		0x00000257, 
		0x00000387, 
		0x00000003, 
		0x00000277, 
		0x00000189, 
		0x00000255, 
		0x00000265, 
		0x000002c7, 
		0x00000081, 
		0x000000e1, 
		0x0000000b, 
		0x00000395, 
		0x000003ef, 
		0x00000081, 
		0x0000027b, 
		0x000000ff, 
		0x0000020f, 
		0x00000233, 
		0x0000021b, 
		0x00000175, 
		0x00000117, 
		0x00000285, 
		0x00000223, 
		0x00000253, 
		0x00000009, 
		0x0000026f, 
		0x0000000d, 
		0x00000175, 
		0x000002b1, 
		0x00000343, 
		0x0000011f, 
		0x00000259, 
		0x00000085, 
		0x000001e5, 
		0x00000223, 
		0x0000002f, 
		0x00000031, 
		0x000002df  
};

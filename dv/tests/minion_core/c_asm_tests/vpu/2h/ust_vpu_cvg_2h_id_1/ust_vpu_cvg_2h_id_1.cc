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
			"csrwi mhpmevent4, 15\n"   
			"csrwi mhpmevent5, 28\n"   
			"csrwi mhpmevent6, 22\n"   
			"csrwi mhpmevent7, 6\n"   
			"csrwi mhpmevent8, 3\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f23,  192 (x5)\n"
			"flw.ps f14,  288 (x5)\n"
			"flw.ps f28,  576 (x5)\n"
			"flw.ps f16,  160 (x5)\n"
			"flw.ps f9,  704 (x5)\n"
			"flw.ps f29,  736 (x5)\n"
			"flw.ps f8,  832 (x5)\n"
			"flw.ps f4,  128 (x5)\n"
			"flw.ps f30,  352 (x5)\n"
			"flw.ps f19,  32 (x5)\n"
			"flw.ps f20,  480 (x5)\n"
			"flw.ps f1,  864 (x5)\n"
			"flw.ps f3,  448 (x5)\n"
			"flw.ps f26,  928 (x5)\n"
			"flw.ps f7,  640 (x5)\n"
			"flw.ps f25,  0 (x5)\n"
			"flw.ps f5,  64 (x5)\n"
			"flw.ps f2,  320 (x5)\n"
			"flw.ps f31,  672 (x5)\n"
			"flw.ps f13,  544 (x5)\n"
			"flw.ps f22,  224 (x5)\n"
			"flw.ps f6,  768 (x5)\n"
			"flw.ps f11,  608 (x5)\n"
			"flw.ps f18,  896 (x5)\n"
			"flw.ps f10,  416 (x5)\n"
			"flw.ps f21,  256 (x5)\n"
			"flw.ps f0,  992 (x5)\n"
			"flw.ps f27,  96 (x5)\n"
			"flw.ps f24,  384 (x5)\n"
			"flw.ps f17,  512 (x5)\n"
			"flw.ps f12,  800 (x5)\n"
			"flw.ps f15,  960 (x5)\n"
			"csrw fflags, zero\n"
			"csrwi frm, 0x0\n"
			"LBL_KERNEL_HID_0:\n"          
			"li x5, 0xFEEDFEED\n"
			"li x0, 0xc8\n" 
			"slti x0, x0, 0x7f2\n"
			"la x31, check_sum_hid_0\n"
			"li x5, 0x1\n" 
			"sw x5, (x31)\n"
			"li x30, 0\n"
			"addi x30, x30, 1\n"
			"la x26, rand_int_32\n"
			"flw.ps f29, 96(x26)\n"
			"fpackrepb.pi f20, f29\n" 
			"la x17, rand_int_32\n"
			"flw.ps f21, 128(x17)\n"
			"flw.ps f24, 288(x17)\n"
			"fadd.pi f30, f21, f24\n" 
			"la x26, rand_int_32\n"
			"flw.ps f0, 384(x26)\n"
			"fpackrepb.pi f26, f0\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f8, 448(x16)\n"
			"fcvt.pw.ps f3, f8, rup\n" 
			"la x19, rand_int_32\n"
			"flw.ps f30, 544(x19)\n"
			"fcvt.ps.pw f16, f30, rtz\n" 
			"la x14, rand_int_32\n"
			"flw.ps f18, 128(x14)\n"
			"fslli.pi f16, f18, 0xf\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_0:\n"
			"feqm.ps m3, f15, f25\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x19, rand_ieee754_32\n"
			"flw.ps f10, 704(x19)\n"
			"fcvt.f16.ps f9, f10\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_0:\n"
			"fsetm.pi m7, f14\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x15, rand_ieee754_32\n"
			"flw.ps f1, 672(x15)\n"
			"flw.ps f5, 928(x15)\n"
			"fadd.ps f22, f1, f5, rup\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f26, 160(x6)\n"
			"flw.ps f1, 192(x6)\n"
			"fmul.s f23, f26, f1, rmm\n" 
			"la x18, rand_int_32\n"
			"flw.ps f10, 32(x18)\n"
			"fpackrepb.pi f8, f10\n" 
			"LBL_SEQID_0_FP_TRANS_ID_2_HID_0:\n"
			"la x23, rand_ieee754_32\n"
			"flw.ps f1, 288(x23)\n"
			"flog.ps f31, f1\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f6, 800(x5)\n"
			"flw.ps f7, 576(x5)\n"
			"fsub.ps f16, f6, f7, rtz\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f29, 768(x15)\n"
			"flw.ps f5, 0(x15)\n"
			"flw.ps f15, 864(x15)\n"
			"fnmsub.s f10, f29, f5, f15, rdn\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f6, 32(x26)\n"
			"flw.ps f19, 224(x26)\n"
			"flw.ps f16, 576(x26)\n"
			"fnmsub.ps f16, f6, f19, f16, rup\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_0\n"
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
			"LBL_HPM_4_CORE_TL_OPS_FAIL_HID_0:\n"          
			"csrr x5, mhpmcounter4\n"
			"sd x5, 40 (x31)\n"
			"LBL_HPM_5_CORE_TQUANT_INST_FAIL_HID_0:\n"          
			"csrr x5, mhpmcounter5\n"
			"sd x5, 48 (x31)\n"
			"LBL_HPM_6_CORE_TXFMA_INT_OPS_FAIL_HID_0:\n"          
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
			"LBL_HPM_3_CORE_TIMA_OPS_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter3\n"
			"sd x5, 32 (x31)\n"
			"LBL_HPM_4_CORE_TL_OPS_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter4\n"
			"sd x5, 40 (x31)\n"
			"LBL_HPM_5_CORE_TQUANT_INST_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter5\n"
			"sd x5, 48 (x31)\n"
			"LBL_HPM_6_CORE_TXFMA_INT_OPS_PASS_HID_0:\n"          
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
	if ( hid%2 == 1 ) { 
		C_TEST_START;
		__asm__ __volatile__ (
			"LBL_PREAMBLE_HID_1:\n"          
			"la x5, LBL_M_MODE_E_H\n"
			"csrw mtvec, x5\n"
			"la x5, LBL_FAIL_HID_1\n"
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
			"csrwi mhpmevent3, 8\n"   
			"csrwi mhpmevent4, 25\n"   
			"csrwi mhpmevent5, 13\n"   
			"csrwi mhpmevent6, 24\n"   
			"csrwi mhpmevent7, 2\n"   
			"csrwi mhpmevent8, 1\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f1,  64 (x5)\n"
			"flw.ps f24,  768 (x5)\n"
			"flw.ps f18,  352 (x5)\n"
			"flw.ps f22,  160 (x5)\n"
			"flw.ps f12,  32 (x5)\n"
			"flw.ps f10,  256 (x5)\n"
			"flw.ps f31,  448 (x5)\n"
			"flw.ps f9,  640 (x5)\n"
			"flw.ps f21,  576 (x5)\n"
			"flw.ps f28,  864 (x5)\n"
			"flw.ps f30,  800 (x5)\n"
			"flw.ps f7,  544 (x5)\n"
			"flw.ps f8,  96 (x5)\n"
			"flw.ps f19,  960 (x5)\n"
			"flw.ps f27,  608 (x5)\n"
			"flw.ps f2,  896 (x5)\n"
			"flw.ps f17,  0 (x5)\n"
			"flw.ps f13,  128 (x5)\n"
			"flw.ps f23,  224 (x5)\n"
			"flw.ps f15,  320 (x5)\n"
			"flw.ps f11,  512 (x5)\n"
			"flw.ps f0,  672 (x5)\n"
			"flw.ps f26,  992 (x5)\n"
			"flw.ps f3,  384 (x5)\n"
			"flw.ps f6,  192 (x5)\n"
			"flw.ps f5,  832 (x5)\n"
			"flw.ps f14,  736 (x5)\n"
			"flw.ps f20,  288 (x5)\n"
			"flw.ps f29,  480 (x5)\n"
			"flw.ps f25,  416 (x5)\n"
			"flw.ps f16,  928 (x5)\n"
			"flw.ps f4,  704 (x5)\n"
			"csrw fflags, zero\n"
			"csrwi frm, 0x0\n"
			"LBL_KERNEL_HID_1:\n"          
			"li x5, 0xFEEDFEED\n"
			"slti x0, x0, 0x7f2\n"
			"la x31, check_sum_hid_1\n"
			"li x5, 0x1\n" 
			"sw x5, (x31)\n"
			"li x30, 0\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_1:\n"
			"fltm.pi m7, f24, f4\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x20, rand_int_32\n"
			"flw.ps f31, 192(x20)\n"
			"fslli.pi f16, f31, 0x9\n" 
			"la x27, rand_int_32\n"
			"flw.ps f7, 864(x27)\n"
			"fpackrepb.pi f25, f7\n" 
			"la x5, rand_int_32\n"
			"flw.ps f20, 384(x5)\n"
			"flw.ps f27, 672(x5)\n"
			"fminu.pi f3, f20, f27\n" 
			"maskpopc x9, m0\n" 
			"mov.m.x m0, x0, 0xff\n"
			"la x7, rand_int_32\n"
			"flw.ps f14, 160(x7)\n"
			"flw.ps f18, 224(x7)\n"
			"fminu.pi f29, f14, f18\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_1:\n"
			"maskxor m1, m1, m3\n" 
			"li x5, 0x803401E0\n"
			"ld x5, 0(x5)\n"
			"mova.x.m x28\n" 
			"fbci.pi f16, 0x73f63\n" 
			"la x26, rand_int_32\n"
			"flw.ps f27, 320(x26)\n"
			"flw.ps f1, 832(x26)\n"
			"fadd.pi f4, f27, f1\n" 
			"la x9, rand_int_32\n"
			"flw.ps f28, 256(x9)\n"
			"flw.ps f11, 64(x9)\n"
			"fsll.pi f0, f28, f11\n" 
			"la x9, rand_int_32\n"
			"flw.ps f9, 800(x9)\n"
			"flw.ps f25, 480(x9)\n"
			"fmaxu.pi f19, f9, f25\n" 
			"LBL_SEQID_0_M0_WRITE_ID_2_HID_1:\n"
			"mova.m.x x10\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x28, rand_ieee754_32\n"
			"flw.ps f17, 960(x28)\n"
			"flw.ps f7, 736(x28)\n"
			"feq.s x25, f17, f7\n" 
			"fbci.ps f7, 0x23e71\n" 
			"LBL_SEQID_0_FP_TRANS_ID_3_HID_1:\n"
			"la x6, rand_ieee754_32\n"
			"flw.ps f25, 160(x6)\n"
			"flog.ps f21, f25\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_1\n"
			"la x31, check_sum_hid_1\n"
			"lw x5, (x31)\n"
			"bne x30, x5, LBL_FAIL_HID_1\n"
			"LBL_POSTAMBLE_HID_1:\n"          
			"fence\n"
			"csrwi excl_mode, 0\n"
			"j LBL_PASS_HID_1\n"          
			VSNIP_RSV
		);
		__asm__ __volatile__ ( 
			"LBL_FAIL_HID_1:\n"          
			".global LBL_FAIL_HID_1\n"          
			"la x31, check_sum_hid_1\n"
			"li x5, 0xDEADDEADDEADDEAD\n"
			"sd x5, 24(x31)\n"
			"LBL_HPM_3_CORE_DCACHE_MISSES0_FAIL_HID_1:\n"          
			"csrr x5, mhpmcounter3\n"
			"sd x5, 32 (x31)\n"
			"LBL_HPM_4_CORE_MASK_OPS_FAIL_HID_1:\n"          
			"csrr x5, mhpmcounter4\n"
			"sd x5, 40 (x31)\n"
			"LBL_HPM_5_CORE_L2_EVICT_REQ_REJ_FAIL_HID_1:\n"          
			"csrr x5, mhpmcounter5\n"
			"sd x5, 48 (x31)\n"
			"LBL_HPM_6_CORE_SHORT_OPS_FAIL_HID_1:\n"          
			"csrr x5, mhpmcounter6\n"
			"sd x5, 56 (x31)\n"
			"LBL_HPM_7_NEIGH_ANY_CORE_RX_ETL_RSP_FAIL_HID_1:\n"          
			"csrr x5, mhpmcounter7\n"
			"sd x5, 64 (x31)\n"
			"LBL_HPM_8_NEIGH_ANY_CORE_TX_ETL_REQ_FAIL_HID_1:\n"          
			"csrr x5, mhpmcounter8\n"
			"sd x5, 72 (x31)\n"
			"slti x0, x0, 0x7ff\n"
			VSNIP_RSV
		);
		C_TEST_FAIL;
		__asm__ __volatile__ ( 
			"j LBL_HALT_HID_1\n"          
			VSNIP_RSV
		);
		__asm__ __volatile__ ( 
			"LBL_PASS_HID_1:\n"          
			".global LBL_PASS_HID_1\n"          
			"la x31, check_sum_hid_1\n"
			"li x5, 0xACEDACEDACEDACED\n"
			"sd x5, 24(x31)\n"
			"LBL_HPM_3_CORE_DCACHE_MISSES0_PASS_HID_1:\n"          
			"csrr x5, mhpmcounter3\n"
			"sd x5, 32 (x31)\n"
			"LBL_HPM_4_CORE_MASK_OPS_PASS_HID_1:\n"          
			"csrr x5, mhpmcounter4\n"
			"sd x5, 40 (x31)\n"
			"LBL_HPM_5_CORE_L2_EVICT_REQ_REJ_PASS_HID_1:\n"          
			"csrr x5, mhpmcounter5\n"
			"sd x5, 48 (x31)\n"
			"LBL_HPM_6_CORE_SHORT_OPS_PASS_HID_1:\n"          
			"csrr x5, mhpmcounter6\n"
			"sd x5, 56 (x31)\n"
			"LBL_HPM_7_NEIGH_ANY_CORE_RX_ETL_RSP_PASS_HID_1:\n"          
			"csrr x5, mhpmcounter7\n"
			"sd x5, 64 (x31)\n"
			"LBL_HPM_8_NEIGH_ANY_CORE_TX_ETL_REQ_PASS_HID_1:\n"          
			"csrr x5, mhpmcounter8\n"
			"sd x5, 72 (x31)\n"
			"slti x0, x0, 0x7fe\n"
			VSNIP_RSV
		);
		__asm__ __volatile__ ( 
			"LBL_HALT_HID_1:\n"          
			".global LBL_HALT_HID_1\n"          
			VSNIP_RSV
		);
	};
		C_TEST_PASS;
		return 0;       
}
volatile uint32_t rand_int_32[256] __attribute__ ((aligned (2048))) = {
		0x56a17056, 
		0x559b7988, 
		0xc4423f14, 
		0x64949741, 
		0x22a54847, 
		0x18a03ec4, 
		0xe2acf989, 
		0xb94eeb91, 
		0x89ec1be0, 
		0x17c31354, 
		0xb17ddb2b, 
		0x7f0574ae, 
		0x6b850433, 
		0x957130da, 
		0xced31e41, 
		0xa65798df, 
		0x52e8e509, 
		0x25d57706, 
		0xc0a490e3, 
		0x44a17267, 
		0x8250d796, 
		0x90a0d45d, 
		0xcebb6490, 
		0x6fd3b449, 
		0xdfd4620e, 
		0x5a53b4b6, 
		0x6da5d589, 
		0x4927c914, 
		0x27093bae, 
		0xe3a62c6e, 
		0x06154fa4, 
		0x08332819, 
		0xdfdfdce5, 
		0xa84877de, 
		0xc1aa0173, 
		0xbb85fcea, 
		0x538b2c7e, 
		0x67b16e32, 
		0x71fb9e22, 
		0xb9d0be94, 
		0x5cfaafe5, 
		0x3927777c, 
		0x8a467173, 
		0x16c9afc2, 
		0x2b1d8dfa, 
		0x750b6628, 
		0xe790ab63, 
		0x9bd7df76, 
		0x42b64eea, 
		0x3179418a, 
		0x82f1a419, 
		0x65ce736c, 
		0x5d3a9c6a, 
		0x25af8fd6, 
		0xf69a40ca, 
		0xb837782b, 
		0x2eccb560, 
		0xec2553a7, 
		0xb30d287d, 
		0xd3fbcc5c, 
		0x4e0d25db, 
		0x892f8758, 
		0xdd987c4d, 
		0x66fb8235, 
		0x3739c34f, 
		0x8489a08d, 
		0x246b5cef, 
		0x6db1fdad, 
		0x068de1c1, 
		0x78dac98d, 
		0x0a825377, 
		0x98f19202, 
		0x4a69a4b7, 
		0xf524a9b0, 
		0x149c5b71, 
		0x803e5750, 
		0x04ed4baf, 
		0xcd60ea8c, 
		0x17fa015a, 
		0x979ddec4, 
		0x9a8b9b19, 
		0xccd4520d, 
		0xf03c80da, 
		0x4dd31fa1, 
		0x61fac02f, 
		0xdcd400d5, 
		0xf7af8646, 
		0xff8361c4, 
		0x68526bbf, 
		0x278d0094, 
		0xde15c27d, 
		0x47a6c66a, 
		0xf0c36761, 
		0x4e084da3, 
		0x827c42ad, 
		0x280bb834, 
		0xb30ef607, 
		0x82a4440e, 
		0x6479d222, 
		0xe82c739b, 
		0x74f330fc, 
		0x13ea783e, 
		0xb326c3c2, 
		0x07f527e5, 
		0x0a6d89bc, 
		0x10c1a0b1, 
		0x89d3d29e, 
		0xdc735db9, 
		0xffada3a7, 
		0x628c7633, 
		0xaa877076, 
		0x3424422e, 
		0x1e28f861, 
		0xef02fe9b, 
		0xccfbf6fd, 
		0x65eacfcc, 
		0xac2ff1e5, 
		0x9bb8dd4a, 
		0x995783eb, 
		0xfd99bf15, 
		0xdfdd6cff, 
		0xa5148d73, 
		0xc0261612, 
		0x1d269ec9, 
		0x5a57f1c5, 
		0x6c2b15e5, 
		0x5fd95b55, 
		0x38b55e2d, 
		0x6967a090, 
		0x77749953, 
		0x430a5317, 
		0x6e7164ca, 
		0x8df28a3d, 
		0x70db5aae, 
		0xcad51157, 
		0x0e59c31b, 
		0x9bd80edc, 
		0xa7fe3c0b, 
		0xd6c3504d, 
		0x506d63da, 
		0x770ad612, 
		0x0229a1ee, 
		0x36efda08, 
		0xc5da9fc2, 
		0x84092c98, 
		0x2c4347dc, 
		0x4212ad46, 
		0xf47b2913, 
		0x25f018aa, 
		0x3aec7f1a, 
		0x7eda5022, 
		0xb9ce24a5, 
		0x16d3b65f, 
		0x4c18760f, 
		0x1e84fbea, 
		0x5f24381b, 
		0xc594a8da, 
		0xd3b5b72a, 
		0xf884983b, 
		0x73ab6f19, 
		0x1e1f0781, 
		0x2cf20cae, 
		0x7bef0c94, 
		0x18538985, 
		0x846e869b, 
		0xc01d88f8, 
		0x5eb82b81, 
		0x21692489, 
		0x798576a3, 
		0x619622db, 
		0x694cb2de, 
		0x53b55e65, 
		0x7e9301e5, 
		0x57ba6e10, 
		0x51a0cae1, 
		0x57fef6f2, 
		0x9a62529b, 
		0xa97b958b, 
		0xc1f1f1cd, 
		0x665b1e6f, 
		0xe6fc3222, 
		0xb88bf001, 
		0x5d816b5b, 
		0xb40df746, 
		0x904358b2, 
		0x04975de7, 
		0x60ae01cf, 
		0xf3114809, 
		0x3ef03ec3, 
		0xe108300e, 
		0x1546255c, 
		0xcedd3645, 
		0x3a7a454e, 
		0xbd25901e, 
		0xc340e9a6, 
		0x1a1e1ba5, 
		0xe70f5976, 
		0x783911f1, 
		0x3a32ca10, 
		0x367c0edc, 
		0xd452c7ab, 
		0x8ad07468, 
		0xc557625e, 
		0x940248cd, 
		0xa56447cd, 
		0x54e6e468, 
		0xd2264e7d, 
		0xcd602d8e, 
		0x438410c2, 
		0x4e3c75fd, 
		0x7e5a717a, 
		0x2e4a8bee, 
		0x03829e94, 
		0xe095d236, 
		0xed778ed1, 
		0x1b754b25, 
		0x12a28f57, 
		0xbd72afc5, 
		0x0cd9674f, 
		0x3c7eac19, 
		0xe52c9415, 
		0xd1a615f6, 
		0xac92dc35, 
		0xb5a69d3e, 
		0xa477cedd, 
		0x4bc0b505, 
		0x3a0c1363, 
		0xe51ba55e, 
		0xab8a116b, 
		0x26137638, 
		0x12f2bec3, 
		0x11e71921, 
		0x49d593c9, 
		0xbfb66afd, 
		0x8f98a814, 
		0x5d9a78ad, 
		0x9728baf2, 
		0x354cdf48, 
		0x52699b9d, 
		0xdd5eea61, 
		0x3112571b, 
		0xfd31bf9d, 
		0xb25ef9ef, 
		0x99391d3d, 
		0xa20fa732, 
		0x0e81cace, 
		0x210effc6, 
		0x4f6536e2, 
		0x5689a5b2, 
		0x111dc185, 
		0x9a9f9c13, 
		0xeb71b2d9, 
		0xe3886f5d, 
		0x38d8ae95, 
		0x3c941e85, 
		0xdfabc302  
};
volatile uint32_t rand_ieee754_32[1536] __attribute__ ((aligned (2048))) = {
		0xc2240000, 
		0x80000000, 
		0x9e133c65, 
		0x00008000, 
		0xbf800000, 
		0x421c0000, 
		0xbb3e94d1, 
		0x0e00007f, 
		0x3fd27c96, 
		0x00000020, 
		0x2cbb2737, 
		0x7f800000, 
		0x80200000, 
		0x42580000, 
		0xc2600000, 
		0x41400000, 
		0xa1dcf366, 
		0x0f7ffffc, 
		0x80002000, 
		0x7f800000, 
		0xffbfffff, 
		0xc1f80000, 
		0xc0e00000, 
		0x6a2e3579, 
		0xc0800000, 
		0xcccccccc, 
		0xc1e80000, 
		0x00800000, 
		0x80000020, 
		0x5577557c, 
		0x41600000, 
		0xc2700000, 
		0xa6cad48b, 
		0x4651fb95, 
		0x42300000, 
		0x80040000, 
		0xbd122f80, 
		0x41400000, 
		0x04fff719, 
		0x0052ecb5, 
		0x5f0e8441, 
		0x4b000000, 
		0xa7e42608, 
		0x00040000, 
		0x0c7ffffc, 
		0x41c00000, 
		0x00000800, 
		0x00000000, 
		0xc1c80000, 
		0xff800001, 
		0x40800000, 
		0x807fffff, 
		0xc0400000, 
		0x80080000, 
		0xb0c13854, 
		0x0c7fffff, 
		0x0c700000, 
		0x42740000, 
		0x41e80000, 
		0xbf028f5c, 
		0x42780000, 
		0x41300000, 
		0x8097ad2c, 
		0x1c4d1dd0, 
		0x42500000, 
		0x007ffffe, 
		0x0c7fff00, 
		0x00080000, 
		0x061ce71e, 
		0xc2000000, 
		0xc2640000, 
		0xb731eee3, 
		0x0e000001, 
		0x42300000, 
		0xff800000, 
		0x8f7fffff, 
		0x419297a4, 
		0x00608a1f, 
		0xaf6bdb9a, 
		0x04fc3c19, 
		0x00000080, 
		0x14be293f, 
		0x4b8c4b40, 
		0xc0400000, 
		0x0e7fffff, 
		0xc0a00000, 
		0x41700000, 
		0xc2300000, 
		0x80800000, 
		0xc2340000, 
		0x4c322a1c, 
		0x7fc00000, 
		0x4b8c4b40, 
		0xc24c0000, 
		0x0c7ffc00, 
		0x80800002, 
		0x40800000, 
		0x41100000, 
		0x42540000, 
		0xcb5b47ee, 
		0x028e7d10, 
		0x879e2b92, 
		0x42680000, 
		0x4790c514, 
		0xc1e80000, 
		0xffb4905f, 
		0x007ffffe, 
		0x120bd82d, 
		0x7f800000, 
		0xc2500000, 
		0x97d79163, 
		0xc2180000, 
		0xc1a00000, 
		0x80800003, 
		0x40400000, 
		0x42440000, 
		0x0c7ffff0, 
		0x1560ad1c, 
		0x2b6b003d, 
		0x00000000, 
		0x382781ee, 
		0x0c7ffffe, 
		0x7f7fffff, 
		0x7f7ffffe, 
		0x42580000, 
		0x00000000, 
		0x7af7b227, 
		0xc1600000, 
		0x42700000, 
		0x0e0001ff, 
		0x7f800000, 
		0xc2640000, 
		0xc1100000, 
		0x80000002, 
		0x0e000003, 
		0x688b1bb0, 
		0x8017acc7, 
		0x80080000, 
		0x00000000, 
		0xbf800001, 
		0x00004000, 
		0x41e80000, 
		0xff7ffffe, 
		0x7fc00001, 
		0x80800000, 
		0x80100000, 
		0x7f800001, 
		0xa4c394aa, 
		0x7f7ffffe, 
		0x15b6005e, 
		0xc1a80000, 
		0x42400000, 
		0x431acc9f, 
		0xd95341a2, 
		0x24268da7, 
		0x41e00000, 
		0x35c26fdc, 
		0xf796ab33, 
		0x7f800000, 
		0xc2780000, 
		0x7fc00000, 
		0x00800002, 
		0xc2000000, 
		0xc1400000, 
		0x0e1fffff, 
		0x7f800000, 
		0x00000100, 
		0x00000000, 
		0x5d4baa20, 
		0x7f800000, 
		0x00000000, 
		0x7f800000, 
		0x80001000, 
		0x48892562, 
		0x32af4ac9, 
		0x06e24718, 
		0xce30c0f4, 
		0xabe8075a, 
		0xc0800000, 
		0xae4f09cd, 
		0x3f800001, 
		0x7fa5bf2d, 
		0x80000008, 
		0x5ced18af, 
		0xc2140000, 
		0x41700000, 
		0x04fe27ed, 
		0xf6cd840a, 
		0xc0400000, 
		0x8f7ffffd, 
		0x22641bad, 
		0x00002000, 
		0x41700000, 
		0x4cf65443, 
		0xf4ae25bf, 
		0xc2540000, 
		0x0c7fff00, 
		0x41e00000, 
		0x54cfd20d, 
		0x00011111, 
		0x800f14ba, 
		0x00000080, 
		0x00000000, 
		0xb5586801, 
		0x80800000, 
		0x42440000, 
		0x3f028f5c, 
		0x41980000, 
		0xffaab8c2, 
		0x80000008, 
		0xff800000, 
		0x00000020, 
		0x6313d979, 
		0x80040000, 
		0x23252b23, 
		0x0e0001ff, 
		0xc2380000, 
		0x80800003, 
		0x41e00000, 
		0x41980000, 
		0x41600000, 
		0x496da933, 
		0x7f7ffffe, 
		0xf338d5ed, 
		0xc1700000, 
		0x42200000, 
		0xc1c80000, 
		0xff800000, 
		0x0c7fff00, 
		0xc1f80000, 
		0x7fc00001, 
		0x7fc00000, 
		0x03b9d4fa, 
		0x7f7ffffe, 
		0x7f800001, 
		0xa9073ce0, 
		0xcb000000, 
		0x80000000, 
		0x80800003, 
		0x00000002, 
		0x42580000, 
		0xb70173c4, 
		0x80800002, 
		0xc2500000, 
		0x00000100, 
		0xff800000, 
		0xb7572a44, 
		0x90c12a11, 
		0x00400000, 
		0x004c37ff, 
		0x00000001, 
		0x80000020, 
		0x80000000, 
		0x80000100, 
		0x7f800000, 
		0x8f7ffffe, 
		0x80800002, 
		0xe3ecbaf1, 
		0x6dc13d71, 
		0x00000000, 
		0x034b7e02, 
		0xc2640000, 
		0x40800000, 
		0xc1a00000, 
		0x0c7ffffc, 
		0xff800000, 
		0x00000000, 
		0xc2180000, 
		0xc2680000, 
		0x0c600000, 
		0x41100000, 
		0x421c0000, 
		0xc2640000, 
		0xe2bf0837, 
		0xff800000, 
		0x7fc00000, 
		0xb01fdece, 
		0xeb99e77a, 
		0xeaa1c604, 
		0xc2140000, 
		0x4b000000, 
		0x0c7f8000, 
		0xff800000, 
		0x0f7ffffc, 
		0x007fffff, 
		0xb12a28b9, 
		0x41d00000, 
		0x0c7ffc00, 
		0x9e643ff7, 
		0x40400000, 
		0x743c78f5, 
		0x6bc73458, 
		0x11a70606, 
		0x00004000, 
		0x80800002, 
		0x33333333, 
		0xbcb48ce3, 
		0xc2580000, 
		0xff800000, 
		0x0e3fffff, 
		0xc2280000, 
		0x71697bfd, 
		0x0e00000f, 
		0x42440000, 
		0x80000200, 
		0x0c7fffe0, 
		0xc1400000, 
		0xc2700000, 
		0xd6deb22a, 
		0x41700000, 
		0xff800000, 
		0xcccccccc, 
		0x7cec7f5d, 
		0xc1100000, 
		0x80000001, 
		0x0e00001f, 
		0x68b1f625, 
		0xfa5984c9, 
		0x0323635c, 
		0x7fffffff, 
		0xf2735d8e, 
		0xb5d26c30, 
		0x807ffffe, 
		0xc2640000, 
		0xc8f3959e, 
		0xff83bb0b, 
		0x80002000, 
		0xff800000, 
		0x421c0000, 
		0x80000000, 
		0x59ed31d7, 
		0xc2580000, 
		0xc1980000, 
		0x00400000, 
		0x66372936, 
		0x8e4121a2, 
		0x4a464ec4, 
		0xc1500000, 
		0x00000040, 
		0x0c7e0000, 
		0x0c7fc000, 
		0xc1e00000, 
		0x173f19ce, 
		0xf963787e, 
		0x40c00000, 
		0x4cce3ebc, 
		0xc1880000, 
		0x9985c3e6, 
		0x4b8c4b40, 
		0x0c7ff000, 
		0x41a80000, 
		0x00000004, 
		0xc2040000, 
		0x005f98b4, 
		0x86561d38, 
		0x00000000, 
		0x3f028f5c, 
		0xbf89d0e8, 
		0x0e0001ff, 
		0x492c5896, 
		0x7f800000, 
		0x0c700000, 
		0xff7fffff, 
		0xc2540000, 
		0x80010000, 
		0xc1880000, 
		0x0e007fff, 
		0xff800000, 
		0x0e6b7596, 
		0x80002000, 
		0xffc4b0c5, 
		0x80000000, 
		0x0c7ffffc, 
		0x0c7f0000, 
		0xff7ffffe, 
		0x72c1998b, 
		0x80000000, 
		0x00000010, 
		0x7f800000, 
		0x42480000, 
		0xff87cb97, 
		0x40a00000, 
		0x7f800001, 
		0x7fffffff, 
		0xde2ce380, 
		0x8f7ffffc, 
		0x6da82fc4, 
		0x0048b3c6, 
		0x00000800, 
		0x3f800001, 
		0x422c0000, 
		0x7f800000, 
		0xc2240000, 
		0x52848369, 
		0xff800000, 
		0x11b265b3, 
		0x80000000, 
		0xdaf3e7f0, 
		0x0c7fffe0, 
		0x0e000003, 
		0x19bccf32, 
		0x40e00000, 
		0x001982d3, 
		0xc1100000, 
		0x80008000, 
		0x80000800, 
		0xffae41ad, 
		0x0c7c0000, 
		0x42140000, 
		0x80000000, 
		0xaa20aea3, 
		0x41700000, 
		0xc1100000, 
		0x80004000, 
		0xffbdba3b, 
		0x80004000, 
		0x3a53b884, 
		0x00000400, 
		0xc6cebdd6, 
		0xaaaaaaaa, 
		0x00010000, 
		0x42640000, 
		0x42140000, 
		0x5d036a12, 
		0x3f028f5c, 
		0x54750102, 
		0xc1200000, 
		0xc5dd00cf, 
		0x0e3fffff, 
		0xc2580000, 
		0x42580000, 
		0x39a1486c, 
		0xc25c0000, 
		0x0c7ffffe, 
		0x00000020, 
		0x0c7fffe0, 
		0xeda3bb94, 
		0x1a9cab6d, 
		0xff8e0103, 
		0xf6c1ba5e, 
		0x93603f93, 
		0x8f7ffffc, 
		0x007ffffe, 
		0x41500000, 
		0xc2040000, 
		0x80000001, 
		0x80800001, 
		0x7f800000, 
		0x42440000, 
		0x0e000003, 
		0x41100000, 
		0x7f800000, 
		0xcb8c4b40, 
		0x80000000, 
		0xc1c80000, 
		0x40a00000, 
		0x6b741570, 
		0x422c0000, 
		0xcdec2f47, 
		0x73771f44, 
		0xf560c133, 
		0x3f028f5c, 
		0x056ba17a, 
		0x3d85b3b3, 
		0x80800001, 
		0xc1a80000, 
		0x15085c1f, 
		0xff7fffff, 
		0x80800003, 
		0xc1900000, 
		0x00000000, 
		0x41f80000, 
		0x00010000, 
		0xc1600000, 
		0x0f7ffffd, 
		0x67233a7d, 
		0xc2200000, 
		0xf9d0f6ee, 
		0x423c0000, 
		0x80000000, 
		0xc1100000, 
		0x0c600000, 
		0xc355bdc4, 
		0x42340000, 
		0x7f800000, 
		0x41d80000, 
		0x80000000, 
		0x1f8696c5, 
		0x4b8c4b40, 
		0x8f7ffffc, 
		0xc469274b, 
		0x00000001, 
		0x97b2ed86, 
		0xc20c0000, 
		0xe8d5c853, 
		0xc0e00000, 
		0x0c7fffc0, 
		0x484333f0, 
		0xc2500000, 
		0x0fd0f2b0, 
		0x00200000, 
		0x41600000, 
		0x9de0062e, 
		0x72ce7298, 
		0x80000000, 
		0x43fc522d, 
		0xb84b3f27, 
		0x41b80000, 
		0xc1600000, 
		0x7faccde5, 
		0x80000008, 
		0x0f7ffffe, 
		0xc2540000, 
		0x41d80000, 
		0xc1700000, 
		0xc2540000, 
		0x41d00000, 
		0x0c7fffc0, 
		0x42340000, 
		0x80000000, 
		0x7f7ffffe, 
		0x814834c5, 
		0x0c7fe000, 
		0x206c53dd, 
		0x42400000, 
		0x4cd6ac5c, 
		0x00000000, 
		0x7f800000, 
		0x0c7ffffc, 
		0x5bb9bc19, 
		0xff000000, 
		0xe031919a, 
		0x8ea60c3d, 
		0x00800000, 
		0x426c0000, 
		0x41e80000, 
		0x8b14863d, 
		0xcb8c4b40, 
		0xc27c0000, 
		0xff800000, 
		0x96321309, 
		0x80001000, 
		0x00000008, 
		0x41700000, 
		0x80000000, 
		0x80011111, 
		0xc27c0000, 
		0xf474515a, 
		0xc1400000, 
		0x00040000, 
		0xff800000, 
		0x80000002, 
		0x42140000, 
		0x9d11bc71, 
		0xdae31568, 
		0xc0400000, 
		0x0c600000, 
		0x42540000, 
		0x2dad05ac, 
		0x7f7ffffe, 
		0xfde4368c, 
		0x00002000, 
		0x80000010, 
		0xc0e00000, 
		0xff800000, 
		0x0e0001ff, 
		0xc2040000, 
		0xff7ffffe, 
		0xc1c80000, 
		0xffffffff, 
		0xff800000, 
		0x7f800000, 
		0x37b3d5f1, 
		0xff800000, 
		0xffffffff, 
		0x1a190ec4, 
		0x68dc45b5, 
		0x00000000, 
		0xc2340000, 
		0x0e0000ff, 
		0xc1a80000, 
		0x92bcb23e, 
		0x42000000, 
		0x647609b4, 
		0x10804aff, 
		0x00000000, 
		0xffc00000, 
		0x8f7ffffd, 
		0xc2580000, 
		0x04c26dba, 
		0x0e001fff, 
		0x41200000, 
		0x26fcaac7, 
		0xff7ffffe, 
		0xcb000000, 
		0xff800000, 
		0x00800001, 
		0xb4e36699, 
		0xc0800000, 
		0x98c8c0d1, 
		0xff8e1906, 
		0xc0a00000, 
		0x35cd87f6, 
		0x00000002, 
		0x0c7f0000, 
		0x80000800, 
		0x42726509, 
		0xa2b88c12, 
		0xffc00000, 
		0xff800001, 
		0x80800000, 
		0x80615661, 
		0xcf5467f4, 
		0xc2080000, 
		0x7f800001, 
		0xade422ac, 
		0x7f7ffffe, 
		0xc9fbd155, 
		0x80000000, 
		0x00800001, 
		0x80800001, 
		0x7fc00000, 
		0x0e00007f, 
		0xfb3fb488, 
		0x80800000, 
		0xffc00001, 
		0x9ace0ed3, 
		0x9af3e290, 
		0x40800000, 
		0x11e7e989, 
		0x0c7e0000, 
		0xef79a2cf, 
		0x427c0000, 
		0x00000010, 
		0xc2140000, 
		0x41200000, 
		0x80000000, 
		0x80000000, 
		0x40e00000, 
		0x80000000, 
		0xc2400000, 
		0x0c7f8000, 
		0x42480000, 
		0x80008000, 
		0xc1980000, 
		0xf718d941, 
		0x42200000, 
		0x00001000, 
		0x8f7ffffd, 
		0xfd3e6cab, 
		0x42740000, 
		0x00000000, 
		0xc433c1d5, 
		0xc0400000, 
		0x0e003fff, 
		0x00000000, 
		0x7f800000, 
		0x7fffffff, 
		0xc1a00000, 
		0x20bcab58, 
		0x504ae505, 
		0xc1600000, 
		0xb7bc5eab, 
		0x30fd9a68, 
		0x98e402f8, 
		0x00000200, 
		0x00000000, 
		0xc2500000, 
		0x0c700000, 
		0x8f7ffffc, 
		0x80800002, 
		0xffb63623, 
		0x41980000, 
		0x00800001, 
		0xffffffff, 
		0xff800000, 
		0x0af93a7e, 
		0x42680000, 
		0x9aff0e63, 
		0x80000001, 
		0xadbddf99, 
		0x08204351, 
		0x6e6313a3, 
		0x41c00000, 
		0x00040000, 
		0xcd198341, 
		0x80004000, 
		0x8f7fffff, 
		0x005e5951, 
		0x0f7fffff, 
		0x78a29f19, 
		0xc2540000, 
		0x41f80000, 
		0x80800000, 
		0x41300000, 
		0x63069da2, 
		0x00dbf0cb, 
		0x42780000, 
		0x0e001fff, 
		0x7f800000, 
		0x0c7e0000, 
		0x068697b1, 
		0xc26c0000, 
		0x42640000, 
		0x7f93a826, 
		0x7fbfffff, 
		0xc27c0000, 
		0x5ab2f321, 
		0x80000000, 
		0x80000001, 
		0x44d5465e, 
		0x9a473201, 
		0xc2040000, 
		0x00000004, 
		0xd4797ae9, 
		0xc1700000, 
		0x3f800000, 
		0x00000000, 
		0x40e00000, 
		0x76a77ef9, 
		0x00000000, 
		0xc1880000, 
		0x32fa3bbf, 
		0xc2580000, 
		0x5fddf6e4, 
		0x00000000, 
		0xc2400000, 
		0xc2140000, 
		0xff7fffff, 
		0x00000002, 
		0x40400000, 
		0x00008000, 
		0x80000000, 
		0x7fffffff, 
		0x40c00000, 
		0xc0e00000, 
		0x0e000fff, 
		0xbb5ca540, 
		0x0e01ffff, 
		0xf3dcf46f, 
		0x7fe2d8d5, 
		0x89abc7ee, 
		0x0e000fff, 
		0x42240000, 
		0x00800002, 
		0xbf26f6b3, 
		0x42700000, 
		0x80000800, 
		0x1f1a949e, 
		0x41980000, 
		0x00000000, 
		0x7fc00000, 
		0x0e01ffff, 
		0x78113472, 
		0x80000040, 
		0x7f800000, 
		0x80800001, 
		0x80000200, 
		0x0c780000, 
		0x73e42a97, 
		0xb0f136b3, 
		0xe0177c37, 
		0xad94924c, 
		0xdbaf4a89, 
		0x789a56e4, 
		0x007fffff, 
		0x00000000, 
		0x80000000, 
		0x40800000, 
		0x3f800000, 
		0xc0000000, 
		0x41300000, 
		0x40000000, 
		0x7f8dc0f7, 
		0xcde48df7, 
		0x0e000fff, 
		0x00800002, 
		0x41500000, 
		0x00000010, 
		0x421c0000, 
		0xb34888d7, 
		0xc1300000, 
		0x41a00000, 
		0x80008000, 
		0xffbfffff, 
		0x007ffffe, 
		0x40a00000, 
		0x2248d053, 
		0x80000010, 
		0x599eb24f, 
		0x4b8c4b40, 
		0x41900000, 
		0x00000000, 
		0x007ffffe, 
		0x00000000, 
		0x0f7ffffc, 
		0xf23529f5, 
		0x7f7fffff, 
		0x425c0000, 
		0x00800002, 
		0x5ed7432c, 
		0x00000000, 
		0x42740000, 
		0x0f7ffffc, 
		0x8f7ffffc, 
		0xffa0612b, 
		0x807fffff, 
		0x42680000, 
		0xffffffff, 
		0x0e0003ff, 
		0x7f800000, 
		0x42500000, 
		0x423c0000, 
		0xd9f84c13, 
		0xffa6ac2c, 
		0x0c2f5cde, 
		0xd07c21db, 
		0x5f730713, 
		0x377ba0e4, 
		0xc2540000, 
		0x0c7fff80, 
		0xff800001, 
		0xe7a102f3, 
		0x00800000, 
		0x80ffffff, 
		0x316787d0, 
		0x40400000, 
		0x0e07ffff, 
		0x00000000, 
		0x8f7fffff, 
		0x80001000, 
		0x42380000, 
		0x00000008, 
		0x00000000, 
		0x9072bdeb, 
		0xd42cb6e7, 
		0xc0400000, 
		0xec60fe9c, 
		0x4b000000, 
		0x3a80c63c, 
		0x80000010, 
		0x0d00fff0, 
		0x00000800, 
		0x555da5a2, 
		0x003b5e86, 
		0xc1d00000, 
		0xc2140000, 
		0x80800001, 
		0x0c7fff00, 
		0x80000080, 
		0xc1b00000, 
		0x79648a25, 
		0x0c7f0000, 
		0xff800000, 
		0x0c7ffff8, 
		0x00800001, 
		0x42380000, 
		0x41600000, 
		0x426c0000, 
		0x00800000, 
		0x7015c3d7, 
		0xc2400000, 
		0x40000000, 
		0xc2600000, 
		0xff800000, 
		0xcc4397bc, 
		0x0c780000, 
		0x42640000, 
		0x7f800000, 
		0x00800000, 
		0xc0e00000, 
		0x41900000, 
		0xff800000, 
		0x6895280c, 
		0xc2000000, 
		0xffc00001, 
		0x80000001, 
		0x725f00e2, 
		0x8eed590a, 
		0x0c7ffe00, 
		0xff800000, 
		0x006f199f, 
		0x0e7fffff, 
		0x6a70b5fa, 
		0xda5a5cc7, 
		0x0e0003ff, 
		0x34f35eb0, 
		0xc91932b4, 
		0xffa29090, 
		0xc2580000, 
		0x0c7fc000, 
		0xff7fffff, 
		0x2f6ac84b, 
		0x80000080, 
		0x521e27c5, 
		0x80000000, 
		0xc2180000, 
		0x80000000, 
		0xc1c00000, 
		0x0c700000, 
		0x42740000, 
		0x7fc00000, 
		0x80004000, 
		0xfa7a6281, 
		0x0c7fff00, 
		0xff800000, 
		0x732d1a39, 
		0xd0da7095, 
		0xde0e6eee, 
		0x491f9dcf, 
		0x7fbfffff, 
		0x41600000, 
		0x0c7fffc0, 
		0x0f7ffffc, 
		0xc86bae56, 
		0x33333333, 
		0x807fffff, 
		0x7f800000, 
		0x4b000000, 
		0x41b80000, 
		0x459ff9c9, 
		0x0f7fffff, 
		0x80000100, 
		0xff800000, 
		0xffc00000, 
		0x40800000, 
		0x658c22ae, 
		0x00080000, 
		0x80200000, 
		0x80800002, 
		0x80000100, 
		0xc23c0000, 
		0xffae0a4c, 
		0xbe2b6dbc, 
		0xa9a6a2fc, 
		0x80000000, 
		0x0e03ffff, 
		0x7f7ffffe, 
		0x00000000, 
		0xc1d80000, 
		0x1f218762, 
		0xff800000, 
		0x007fffff, 
		0xc1f80000, 
		0x5a71b4b9, 
		0x0c7ffffc, 
		0xbc8615b3, 
		0xc9a1b420, 
		0x80800001, 
		0x7f800000, 
		0x284df9db, 
		0x00001000, 
		0xc2700000, 
		0xc2700000, 
		0x00000010, 
		0x0c7ffff0, 
		0x0c600000, 
		0xff7fffff, 
		0xe08bdeed, 
		0x80800003, 
		0x00800001, 
		0xc1980000, 
		0xc0c00000, 
		0x42040000, 
		0x0c7ffc00, 
		0x2b8b6d84, 
		0xc20c0000, 
		0x00800002, 
		0x42240000, 
		0x21fbee0e, 
		0x0f7fffff, 
		0x1f8dac8c, 
		0x421c0000, 
		0x42100000, 
		0x42680000, 
		0x7fc00000, 
		0x7f800000, 
		0x42100000, 
		0xc0400000, 
		0x0c7ffff8, 
		0x427c0000, 
		0xc0e00000, 
		0x00000000, 
		0x3c0102a3, 
		0xffa3aada, 
		0x1724be26, 
		0x00100000, 
		0xc0400000, 
		0x80000001, 
		0x80001000, 
		0xcccccccc, 
		0x00400000, 
		0xff9a9d64, 
		0xabc73e27, 
		0x92cae8cf, 
		0x42400000, 
		0x0e0003ff, 
		0xbf800001, 
		0x7f000000, 
		0x59f6f53c, 
		0x749ec02b, 
		0xf5084e5c, 
		0x80000000, 
		0x42400000, 
		0xc1a00000, 
		0xe3e59a06, 
		0x0f7fffff, 
		0x80008000, 
		0x425c0000, 
		0x98eb1ff1, 
		0xa9aef7f6, 
		0xda7d6fd4, 
		0x244b93b2, 
		0x454bbb8f, 
		0x0e000001, 
		0x42a4580a, 
		0x206c3a77, 
		0xc21c0000, 
		0xc1d00000, 
		0xef2ec6e4, 
		0x0c7ffffc, 
		0x9318fe30, 
		0x0e01ffff, 
		0x00000000, 
		0xffbfcc2a, 
		0x42740000, 
		0x40a00000, 
		0xc1980000, 
		0x42700000, 
		0x41667d1e, 
		0x00000001, 
		0x3bd55a3a, 
		0xff800000, 
		0x313375a0, 
		0xff7ffffe, 
		0x474b486f, 
		0x423c0000, 
		0x41e80000, 
		0xe22d8d2c, 
		0x422c0000, 
		0x05752a97, 
		0xf079bad8, 
		0xffc00000, 
		0xc2440000, 
		0x424c0000, 
		0x0c7f0000, 
		0x80000000, 
		0x0c600000, 
		0xf3e89109, 
		0xc1000000, 
		0x80040000, 
		0x0e000003, 
		0x42500000, 
		0xb80cdf79, 
		0xffc00000, 
		0x2d8bdb7b, 
		0xff000000, 
		0xff7fffff, 
		0x0e0003ff, 
		0x0e00003f, 
		0x00020000, 
		0x60d94f2f, 
		0x41e00000, 
		0x805504e8, 
		0x5d99d2c2, 
		0xffb61c76, 
		0x7f800000, 
		0x00800000, 
		0xff800000, 
		0x479911a0, 
		0x04d92be4, 
		0xa1467fef, 
		0x14b2a3e4, 
		0x0c7f0000, 
		0xfa3785ab, 
		0x98ce9c59, 
		0xc2700000, 
		0x00800000, 
		0x80010000, 
		0x8f7fffff, 
		0x80000000, 
		0x7fb0b1aa, 
		0xcc658d12, 
		0x20b11313, 
		0x80000000, 
		0x0c7fffc0, 
		0x8a5270a7, 
		0x80800003, 
		0x7f800001, 
		0xc0800000, 
		0x00800000, 
		0x0c7e0000, 
		0xffb88f95, 
		0x80800000, 
		0x3e32d4c7, 
		0x42080000, 
		0xc1400000, 
		0xc2780000, 
		0x0e000001, 
		0x0f7ffffd, 
		0xc2240000, 
		0xff800000, 
		0xc40719b0, 
		0x00000200, 
		0xc0000000, 
		0x41d00000, 
		0x7f803980, 
		0x00800001, 
		0x66fb4d65, 
		0x00002000, 
		0xc1a00000, 
		0x0c7ff800, 
		0x80000004, 
		0x00000002, 
		0x80000400, 
		0x813fc037, 
		0xc20c0000, 
		0x00000002, 
		0xc0000000, 
		0xffa2438f, 
		0x00000020, 
		0x00800003, 
		0xab134e8c, 
		0x41c00000, 
		0xefc536da, 
		0x247b5e2d, 
		0xcb000000, 
		0x7f7fffff, 
		0xc26c0000, 
		0x00000010, 
		0xef68cc84, 
		0x00800002, 
		0x0e07ffff, 
		0x80620f75, 
		0x0e0000ff, 
		0x41d80000, 
		0xc1c00000, 
		0x8f7ffffd, 
		0x00517aff, 
		0x41300000, 
		0x00000020, 
		0x42500000, 
		0x80000000, 
		0x80000002, 
		0xc1c00000, 
		0x00000000, 
		0xc1d00000, 
		0xc24c0000, 
		0x855e5ee0, 
		0x0f7fffff, 
		0xff7ffffe, 
		0x80100000, 
		0xc2740000, 
		0xcccccccc, 
		0x00800000, 
		0x41880000, 
		0x80000000, 
		0x80000001, 
		0x8005f955, 
		0x7d80561e, 
		0x7378b3b8, 
		0x8062a82f, 
		0x0e000007, 
		0x7891649c, 
		0x67a6f130, 
		0x41300000, 
		0xcfcef509, 
		0x41a80000, 
		0x5daaf0b3, 
		0x42100000, 
		0x41b80000, 
		0x00000000, 
		0x7f000000, 
		0x0a081dca, 
		0xcdb2348a, 
		0x8b4714ca, 
		0xff000000, 
		0x41880000, 
		0xc2440000, 
		0xd57dd35f, 
		0x00000000, 
		0x80008000, 
		0x41600000, 
		0x00800001, 
		0x00000000, 
		0xff7fffff, 
		0xba5ceb0b, 
		0x80000100, 
		0xcf9856b4, 
		0x00000800, 
		0x0c7ffffc, 
		0x10c7e0a3, 
		0x0e7fffff, 
		0x80000100, 
		0x00000000, 
		0x35929ef9, 
		0xff800001, 
		0x80080000, 
		0x00000000, 
		0xe46f038a, 
		0x41a80000, 
		0x0c7ff000, 
		0xe9793dbc, 
		0x80000000, 
		0x4b000000, 
		0xe939b417, 
		0x0c400000, 
		0xdfec2949, 
		0x0c400000, 
		0x80800003, 
		0x42440000, 
		0xc1000000, 
		0xff7ffffe, 
		0x33333333, 
		0xffa292d2, 
		0x95a8f637, 
		0xc2040000, 
		0x41500000, 
		0x79a25616, 
		0xc9014d21, 
		0x42640000, 
		0x42380000, 
		0x623e6c8a, 
		0x0e003fff, 
		0x00000200, 
		0x1616981e, 
		0x0c7fc000, 
		0x00000008, 
		0x0c7fffe0, 
		0x00000000, 
		0xff800000, 
		0xc25c0000, 
		0xc2480000, 
		0x2d062eab, 
		0x58336550, 
		0x0c700000, 
		0x00000000, 
		0x0e000fff, 
		0x007ffffe, 
		0x0c7f8000, 
		0x00000080, 
		0x80080000, 
		0x80001000, 
		0x426c0000, 
		0x3f028f5c, 
		0x422c0000, 
		0xb9a52959, 
		0xc20c0000, 
		0xc2680000, 
		0x2b4773bc, 
		0x422c0000, 
		0x7f546cec, 
		0x0e0001ff, 
		0x0e0000ff, 
		0x0c7fffff, 
		0x00000000, 
		0x0e003fff, 
		0x00000000, 
		0xb9151918, 
		0x75e81914, 
		0x3ed9bea3, 
		0x80800000, 
		0x0e007fff, 
		0x00000400, 
		0x0c7e0000, 
		0x0e0001ff, 
		0x80000000, 
		0x00001000, 
		0x0c400000, 
		0x80000400, 
		0xd1f1593d, 
		0x20f604c2, 
		0x80800001, 
		0xa6c9bbbc, 
		0x00200000, 
		0x41200000, 
		0x7f8d2c89, 
		0x00000040, 
		0xff800001, 
		0x1d2445d6, 
		0x00000000, 
		0x0e000007, 
		0x4dc863f2, 
		0x00400000, 
		0xf863e596, 
		0x7f800000, 
		0xc2300000, 
		0x8e3d8452, 
		0x80000002, 
		0x00000000, 
		0xc1500000, 
		0x4369cb82, 
		0xbbdd97ce, 
		0x41d80000, 
		0x80000100, 
		0x2642b0a7, 
		0x41f00000, 
		0xc2200000, 
		0xa3660f8d, 
		0x00000000, 
		0x0e0fffff, 
		0xc2640000, 
		0xa6f8e341, 
		0x8f7fffff, 
		0x00020000, 
		0xf4891143, 
		0x0e00007f, 
		0x80000001, 
		0x42140000, 
		0x72acdc08, 
		0x0f7ffffc, 
		0x00010000, 
		0x1e2a1710, 
		0x7f800000, 
		0x41c00000, 
		0x00000000, 
		0xbd5f7308, 
		0x7f800000, 
		0x80000000, 
		0x3323b8fc, 
		0x1dd04a94, 
		0xff800000, 
		0x80000000, 
		0x80000000, 
		0x0e0003ff, 
		0xc2380000, 
		0x55acfd3f, 
		0x80000100, 
		0xff800000, 
		0x41f00000, 
		0xff95137c, 
		0x6fb1a304, 
		0x0e00003f, 
		0x41e00000, 
		0x4b8c4b40, 
		0xc1880000, 
		0x41900000, 
		0xc1c80000, 
		0xffc00000, 
		0x41b80000, 
		0x41900000, 
		0x00000800, 
		0xac4d53e1, 
		0x41d80000, 
		0x0c7e0000, 
		0x80000000, 
		0x0f7ffffd, 
		0xc1900000, 
		0xff800000, 
		0x80000002, 
		0x0f7ffffd, 
		0xc1b00000, 
		0x00800003, 
		0x00416134, 
		0x53eb42f3, 
		0xc25c0000, 
		0x42680000, 
		0xc1700000, 
		0xc0e00000, 
		0x83b9a413, 
		0x628e97c7, 
		0xffc00000, 
		0x41f00000, 
		0x00000040, 
		0x0c780000, 
		0x80000000, 
		0xff800000, 
		0xc2400000, 
		0xc2300000, 
		0xff800000, 
		0x41800000, 
		0x74871e4e, 
		0xc1a80000, 
		0x00000000, 
		0xc1000000, 
		0x41d80000, 
		0xc1880000, 
		0xce4c044f, 
		0x42280000, 
		0x80000000, 
		0x5dbd9f5c, 
		0xa6820a33, 
		0x00100000, 
		0x285d2aec, 
		0x0c7ffe00, 
		0x0e007fff, 
		0xff800000, 
		0x80002000, 
		0x80010000, 
		0x6d8ad953, 
		0x00400000, 
		0x00000100, 
		0x169a5e7a, 
		0x8021afba, 
		0x42640000, 
		0x80001000, 
		0xc27c0000, 
		0xffc01391, 
		0x80000020, 
		0x8ccf559d, 
		0x7f800001, 
		0x41200000, 
		0x0e07ffff, 
		0xc1880000, 
		0xe56b9b8d, 
		0x0c7ffffe, 
		0x80000000, 
		0x80004000, 
		0xff70d16e, 
		0x00340035, 
		0x71f761f1, 
		0x0c7ffff0, 
		0x1a0685a7, 
		0xc2400000, 
		0x47fb0a2f, 
		0x933d0d5c, 
		0x42000000, 
		0x4a903405, 
		0x007fffff, 
		0x80000000, 
		0x80000000, 
		0x80800001, 
		0xc2000000, 
		0xc25c0000, 
		0x00000000, 
		0x00000800, 
		0xc1800000, 
		0x0e01ffff, 
		0xff800000, 
		0x5f9ae031, 
		0x00000000, 
		0x7f800000, 
		0x0c7fc000, 
		0x7f7ffffe, 
		0xc27c0000, 
		0x41700000, 
		0x00000000, 
		0xff7fffff, 
		0x80100000, 
		0x0548e655, 
		0x7f7fffff, 
		0xc837eafc, 
		0x382d991a, 
		0x00008000, 
		0x46d1a065, 
		0x663ca36a, 
		0x424c0000, 
		0x00000000, 
		0x420c0000, 
		0x7fc00000, 
		0x0e789a6f, 
		0x42540000, 
		0x41d00000, 
		0x0c7ffc00, 
		0x806bf0ca, 
		0x80153b82, 
		0xd3e5c219, 
		0xffa3e115, 
		0xc1a00000, 
		0x40e00000, 
		0x7f7fffff, 
		0xc1f00000, 
		0xc1c80000, 
		0x3ebd63c3, 
		0x80000000, 
		0x41000000, 
		0x40000000, 
		0x41d00000, 
		0x420c0000, 
		0x203a9884, 
		0x3340d59b, 
		0x4d0aac91, 
		0xff7ffffe, 
		0xc2340000, 
		0x06d43858, 
		0x0e00ffff, 
		0xc0e00000, 
		0xff7ffffe, 
		0xa325539e, 
		0xc2040000, 
		0xc1200000, 
		0x990ffd16, 
		0x5878ffa8, 
		0x00800003, 
		0x5d402b6f, 
		0x00000000, 
		0x0c400000, 
		0xc20c0000, 
		0x80000000, 
		0x3f028f5c, 
		0xff800000, 
		0xf9b39121, 
		0x33333333, 
		0x80800002, 
		0x42080000, 
		0x42040000, 
		0x80000010, 
		0xd957b65b, 
		0x8f7ffffc, 
		0xc1500000, 
		0xc2440000, 
		0xc2040000, 
		0x8035ae49, 
		0x6edb8720, 
		0x4b000000, 
		0x00002000, 
		0xc2480000, 
		0x42680000, 
		0x0f7ffffe, 
		0x42000000, 
		0x42140000, 
		0x0c7fffff, 
		0x42680000, 
		0x0e07ffff, 
		0xc2280000, 
		0x00100000, 
		0x00040000, 
		0xc22c149a, 
		0x0f7ffffd, 
		0xc2040000, 
		0xc23c0000, 
		0xff800000, 
		0x2dd3d7e5, 
		0x0e1fffff, 
		0xff9227b5, 
		0xc2400000, 
		0xc1b00000, 
		0x8fe5db3d, 
		0x6a3f6a88, 
		0x7f800000, 
		0xb54eaee5, 
		0x80200000, 
		0x2baf98f5, 
		0xc1500000, 
		0xc2180000, 
		0x0e003fff  
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
volatile uint64_t check_sum_hid_1[16] __attribute__ ((aligned (1024))) = {
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
		0x0000006b, 
		0x00000057, 
		0x000001cd, 
		0x0000014b, 
		0x00000135, 
		0x00000305, 
		0x0000030b, 
		0x00000261, 
		0x00000365, 
		0x00000137, 
		0x000000c9, 
		0x000003b3, 
		0x00000173, 
		0x0000003b, 
		0x00000273, 
		0x000003ff, 
		0x000000e7, 
		0x00000193, 
		0x000003f3, 
		0x0000029f, 
		0x000003f1, 
		0x00000309, 
		0x0000006f, 
		0x00000113, 
		0x00000061, 
		0x0000039f, 
		0x00000097, 
		0x000001cf, 
		0x000002a9, 
		0x00000115, 
		0x00000033, 
		0x000000eb, 
		0x00000065, 
		0x000002bd, 
		0x0000029d, 
		0x00000361, 
		0x0000008f, 
		0x000002ad, 
		0x0000007f, 
		0x000001e3, 
		0x000002a3, 
		0x00000393, 
		0x00000071, 
		0x00000369, 
		0x000001b5, 
		0x0000012d, 
		0x000002df, 
		0x0000000b, 
		0x000000c5, 
		0x000002d5, 
		0x00000211, 
		0x000002e7, 
		0x00000339, 
		0x00000387, 
		0x00000017, 
		0x0000020f, 
		0x000003d3, 
		0x00000049, 
		0x0000013d, 
		0x000000d1, 
		0x000002b3, 
		0x000001c5, 
		0x00000199, 
		0x00000305, 
		0x00000313, 
		0x0000017f, 
		0x0000032f, 
		0x00000091, 
		0x0000035b, 
		0x00000023, 
		0x00000027, 
		0x000000a1, 
		0x00000345, 
		0x00000351, 
		0x000002d1, 
		0x0000006f, 
		0x00000029, 
		0x00000025, 
		0x00000369, 
		0x000003f9, 
		0x00000103, 
		0x000003eb, 
		0x000002d9, 
		0x0000000d, 
		0x00000069, 
		0x00000171, 
		0x00000345, 
		0x0000035b, 
		0x00000011, 
		0x000003fd, 
		0x00000355, 
		0x00000147, 
		0x000002db, 
		0x0000031d, 
		0x000001f9, 
		0x00000337, 
		0x00000057, 
		0x00000265, 
		0x000003a3, 
		0x0000028f, 
		0x000001e9, 
		0x0000014f, 
		0x00000285, 
		0x000003b3, 
		0x00000371, 
		0x0000005f, 
		0x00000077, 
		0x00000207, 
		0x0000014f, 
		0x000002d7, 
		0x0000039f, 
		0x000002b9, 
		0x00000023, 
		0x00000335, 
		0x00000285, 
		0x00000221, 
		0x00000329, 
		0x00000283, 
		0x00000265, 
		0x000000af, 
		0x000003f3, 
		0x000003d1, 
		0x000000a1, 
		0x000002e1, 
		0x00000373, 
		0x000000e3, 
		0x00000039, 
		0x000002ef, 
		0x000001d9, 
		0x0000018f, 
		0x000002d5, 
		0x000000af, 
		0x00000113, 
		0x000002c9, 
		0x000001f5, 
		0x000000db, 
		0x0000002d, 
		0x0000036b, 
		0x0000036f, 
		0x000002d9, 
		0x000001b7, 
		0x00000121, 
		0x00000091, 
		0x0000009f, 
		0x000002e5, 
		0x0000014d, 
		0x000001a7, 
		0x000000db, 
		0x00000171, 
		0x00000089, 
		0x00000305, 
		0x0000011b, 
		0x00000195, 
		0x00000219, 
		0x0000038f, 
		0x000001f7, 
		0x00000237, 
		0x00000147, 
		0x00000353, 
		0x000000bd, 
		0x000000d9, 
		0x00000047, 
		0x00000157, 
		0x00000387, 
		0x000002f3, 
		0x000002d3, 
		0x000001fd, 
		0x0000017d, 
		0x0000021f, 
		0x00000075, 
		0x00000155, 
		0x0000018b, 
		0x00000065, 
		0x00000241, 
		0x00000263, 
		0x00000117, 
		0x0000039d, 
		0x00000393, 
		0x000003bd, 
		0x0000025b, 
		0x00000051, 
		0x000002fb, 
		0x00000071, 
		0x00000261, 
		0x00000369, 
		0x000002f3, 
		0x00000229, 
		0x00000059, 
		0x00000383, 
		0x000002ed, 
		0x0000033f, 
		0x000001d9, 
		0x000001ef, 
		0x000003a9, 
		0x000003ed, 
		0x000001ab, 
		0x00000293, 
		0x00000111, 
		0x00000279, 
		0x00000217, 
		0x00000105, 
		0x000001d9, 
		0x00000395, 
		0x00000089, 
		0x000002eb, 
		0x0000013f, 
		0x00000267, 
		0x000003bd, 
		0x000003f9, 
		0x0000024b, 
		0x000001a3, 
		0x00000223, 
		0x000000cd, 
		0x000000db, 
		0x000001a5, 
		0x000002c3, 
		0x0000032f, 
		0x000002e3, 
		0x00000297, 
		0x00000163, 
		0x0000001d, 
		0x00000079, 
		0x0000019b, 
		0x00000373, 
		0x000003b9, 
		0x00000099, 
		0x00000347, 
		0x0000021d, 
		0x00000363, 
		0x00000165, 
		0x000002ab, 
		0x0000006b, 
		0x00000185, 
		0x00000143, 
		0x00000171, 
		0x00000229, 
		0x00000341, 
		0x00000137, 
		0x000002dd, 
		0x000002c9, 
		0x000001cd, 
		0x00000261, 
		0x000002b7, 
		0x00000263, 
		0x000000f5, 
		0x00000121, 
		0x0000003f, 
		0x0000016b, 
		0x0000011b, 
		0x00000005, 
		0x000002d3, 
		0x00000259, 
		0x00000247, 
		0x00000281, 
		0x00000363, 
		0x00000097  
};

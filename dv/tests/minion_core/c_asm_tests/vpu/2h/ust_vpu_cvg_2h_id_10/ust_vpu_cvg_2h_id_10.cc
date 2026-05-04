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
			"csrwi mhpmevent3, 14\n"   
			"csrwi mhpmevent4, 26\n"   
			"csrwi mhpmevent5, 4\n"   
			"csrwi mhpmevent6, 18\n"   
			"csrwi mhpmevent7, 6\n"   
			"csrwi mhpmevent8, 3\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f30,  736 (x5)\n"
			"flw.ps f17,  768 (x5)\n"
			"flw.ps f18,  0 (x5)\n"
			"flw.ps f14,  608 (x5)\n"
			"flw.ps f19,  512 (x5)\n"
			"flw.ps f12,  256 (x5)\n"
			"flw.ps f21,  64 (x5)\n"
			"flw.ps f22,  96 (x5)\n"
			"flw.ps f24,  416 (x5)\n"
			"flw.ps f23,  992 (x5)\n"
			"flw.ps f13,  192 (x5)\n"
			"flw.ps f28,  928 (x5)\n"
			"flw.ps f27,  576 (x5)\n"
			"flw.ps f29,  448 (x5)\n"
			"flw.ps f31,  288 (x5)\n"
			"flw.ps f1,  320 (x5)\n"
			"flw.ps f0,  960 (x5)\n"
			"flw.ps f26,  32 (x5)\n"
			"flw.ps f7,  480 (x5)\n"
			"flw.ps f6,  896 (x5)\n"
			"flw.ps f15,  864 (x5)\n"
			"flw.ps f11,  640 (x5)\n"
			"flw.ps f3,  160 (x5)\n"
			"flw.ps f10,  704 (x5)\n"
			"flw.ps f9,  384 (x5)\n"
			"flw.ps f16,  224 (x5)\n"
			"flw.ps f2,  800 (x5)\n"
			"flw.ps f4,  128 (x5)\n"
			"flw.ps f5,  672 (x5)\n"
			"flw.ps f25,  352 (x5)\n"
			"flw.ps f20,  544 (x5)\n"
			"flw.ps f8,  832 (x5)\n"
			"csrw fflags, zero\n"
			"csrwi frm, 0x0\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_mask, zero\n"
			"csrwi tensor_wait, 0x6\n" 
			"fence\n"
			"csrwi excl_mode, 1\n"
			"csrwi tensor_wait, 0x6\n" 
			"fence\n"
			"li x5, 0x80000000004000f\n"
			"csrw   evict_sw, x5\n" 
			"li x5, 0x80000000004004f\n"
			"csrw   evict_sw, x5\n" 
			"li x5, 0x80000000004008f\n"
			"csrw   evict_sw, x5\n" 
			"li x5, 0x8000000000400cf\n"
			"csrw   evict_sw, x5\n" 
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
			"li x0, 0x44c\n" 
			"slti x0, x0, 0x7f2\n"
			"la x31, check_sum_hid_0\n"
			"li x5, 0x2\n" 
			"sw x5, (x31)\n"
			"li x29, 0x8\n"
			"LBL_OUTER_LOOP_SEQ_HID_0:\n"
			"nop \n"
			"nop \n"
			"li x30, 0\n"
			"addi x30, x30, 1\n"
			"la x25, rand_ieee754_32\n"
			"flw.ps f22, 576(x25)\n"
			"flw.ps f28, 704(x25)\n"
			"fmul.ps f25, f22, f28, rdn\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f24, 608(x18)\n"
			"flw.ps f19, 32(x18)\n"
			"fnmsub.ps f16, f24, f24, f19, rmm\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_0:\n"
			"mova.m.x x9\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x10, rand_ieee754_32\n"
			"flw.ps f22, 768(x10)\n"
			"flw.ps f28, 864(x10)\n"
			"fsub.ps f13, f22, f28, rne\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f1, 800(x25)\n"
			"flw.ps f19, 96(x25)\n"
			"fmin.s f12, f1, f19\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f2, 96(x18)\n"
			"flw.ps f12, 96(x18)\n"
			"flw.ps f31, 320(x18)\n"
			"fmsub.ps f15, f2, f12, f31, rne\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f16, 512(x27)\n"
			"flw.ps f22, 832(x27)\n"
			"fmin.ps f16, f16, f22\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f8, 416(x17)\n"
			"fcvt.f16.ps f12, f8\n" 
			"la x15, rand_int_32\n"
			"flw.ps f16, 672(x15)\n"
			"fsrl.pi f24, f16, f16\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f15, 608(x18)\n"
			"flw.ps f28, 128(x18)\n"
			"feq.s x15, f15, f28\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f15, 832(x18)\n"
			"flw.ps f1, 800(x18)\n"
			"flw.ps f12, 160(x18)\n"
			"fnmadd.ps f1, f15, f1, f12, rne\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x18, rand_ieee754_32\n"
			"flw.ps f22, 288(x18)\n"
			"fcvt.l.s x15, f22, rtz\n" 
			"fbci.pi f7, 0x62ff7\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f31, 768(x18)\n"
			"flw.ps f8, 960(x18)\n"
			"flw.ps f13, 128(x18)\n"
			"fmadd.s f17, f31, f8, f13, rmm\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f13, 32(x27)\n"
			"flw.ps f12, 128(x27)\n"
			"fadd.s f17, f13, f12, rdn\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f26, 352(x18)\n"
			"flw.ps f16, 960(x18)\n"
			"flw.ps f24, 160(x18)\n"
			"fnmsub.ps f26, f26, f16, f24, rne\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ITER_0_HID_0:\n"
			"li x5, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x1 << TENSOR_IMA_B_NUM_COLS) | (0xc << TENSOR_IMA_A_NUM_ROWS) | (0x3 << TENSOR_IMA_A_NUM_COLS) | (0x5 << TENSOR_IMA_OFFSET) | (0x0 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0xa << TENSOR_IMA_SCP_LOC_B) | (0x56 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x0 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x6, ((0x12 << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0xc << TENSOR_QUANT_QUANT_ROW) | (0x12 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x1 << TENSOR_QUANT_TRANSF8) | (0xa << TENSOR_QUANT_TRANSF7) | (0x3 << TENSOR_QUANT_TRANSF6) | (0x4 << TENSOR_QUANT_TRANSF5) | (0x8 << TENSOR_QUANT_TRANSF4) | (0x9 << TENSOR_QUANT_TRANSF3) | (0xa << TENSOR_QUANT_TRANSF2) | (0x8 << TENSOR_QUANT_TRANSF1) | (0x9 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ITER_1_HID_0:\n"
			"li x5, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x0 << TENSOR_FMA16_B_NUM_COLS) | (0x5 << TENSOR_FMA16_A_NUM_ROWS) | (0x9 << TENSOR_FMA16_A_NUM_COLS) | (0x3 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0xe1 << TENSOR_FMA16_SCP_LOC_B) | (0xc5 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ITER_2_HID_0:\n"
			"li x5, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x2 << TENSOR_IMA_B_NUM_COLS) | (0x2 << TENSOR_IMA_A_NUM_ROWS) | (0xd << TENSOR_IMA_A_NUM_COLS) | (0x6 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x8e << TENSOR_IMA_SCP_LOC_B) | (0xd6 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x0 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x6, ((0x1d << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0x2 << TENSOR_QUANT_QUANT_ROW) | (0x3 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x1 << TENSOR_QUANT_TRANSF6) | (0x5 << TENSOR_QUANT_TRANSF5) | (0x8 << TENSOR_QUANT_TRANSF4) | (0x4 << TENSOR_QUANT_TRANSF3) | (0x4 << TENSOR_QUANT_TRANSF2) | (0x5 << TENSOR_QUANT_TRANSF1) | (0xa << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ITER_3_HID_0:\n"
			"li x5, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x0 << TENSOR_IMA_B_NUM_COLS) | (0x6 << TENSOR_IMA_A_NUM_ROWS) | (0x9 << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x4c << TENSOR_IMA_SCP_LOC_B) | (0xe0 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x0 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x6, ((0x1f << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0x6 << TENSOR_QUANT_QUANT_ROW) | (0x5 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x1 << TENSOR_QUANT_TRANSF2) | (0x9 << TENSOR_QUANT_TRANSF1) | (0x4 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ITER_4_HID_0:\n"
			"li x5, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x1 << TENSOR_FMA16_B_NUM_COLS) | (0x6 << TENSOR_FMA16_A_NUM_ROWS) | (0x6 << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0xe3 << TENSOR_FMA16_SCP_LOC_B) | (0x5 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ITER_5_HID_0:\n"
			"li x6, ((0x7 << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0xc << TENSOR_QUANT_QUANT_ROW) | (0x1a << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x6 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ITER_6_HID_0:\n"
			"li x5, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x2 << TENSOR_FMA16_B_NUM_COLS) | (0x0 << TENSOR_FMA16_A_NUM_ROWS) | (0x3 << TENSOR_FMA16_A_NUM_COLS) | (0x9 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x5e << TENSOR_FMA16_SCP_LOC_B) | (0x45 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ITER_7_HID_0:\n"
			"li x5, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x0 << TENSOR_IMA_B_NUM_COLS) | (0x9 << TENSOR_IMA_A_NUM_ROWS) | (0x1 << TENSOR_IMA_A_NUM_COLS) | (0x3 << TENSOR_IMA_OFFSET) | (0x0 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x5a << TENSOR_IMA_SCP_LOC_B) | (0x56 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x0 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x6, ((0x17 << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0x9 << TENSOR_QUANT_QUANT_ROW) | (0x33 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x1 << TENSOR_QUANT_TRANSF1) | (0xa << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 7\n"
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_0\n"
			"csrw tensor_error, x0\n"
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_0\n"
			"addi x30, x30, 1\n"
			"la x17, rand_int_32\n"
			"flw.ps f12, 736(x17)\n"
			"fpackreph.pi f26, f12\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f7, 352(x9)\n"
			"flw.ps f13, 896(x9)\n"
			"fmax.ps f17, f7, f13\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f7, 352(x23)\n"
			"flw.ps f12, 64(x23)\n"
			"flw.ps f16, 32(x23)\n"
			"fmsub.ps f28, f7, f12, f16, rne\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f16, 960(x9)\n"
			"flw.ps f17, 128(x9)\n"
			"fsgnjx.s f25, f16, f17\n" 
			"la x18, rand_int_32\n"
			"flw.ps f1, 576(x18)\n"
			"flw.ps f31, 256(x18)\n"
			"fminu.pi f1, f1, f31\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f2, 768(x17)\n"
			"flw.ps f15, 224(x17)\n"
			"fmul.s f1, f2, f15, rne\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f16, 96(x9)\n"
			"flw.ps f25, 192(x9)\n"
			"flt.s x9, f16, f25\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f17, 896(x23)\n"
			"fsin.ps f8, f17\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f12, 672(x28)\n"
			"flw.ps f16, 352(x28)\n"
			"fsgnjx.ps f15, f12, f16\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f13, 672(x18)\n"
			"fclass.s x23, f13\n" 
			"la x9, rand_int_32\n"
			"flw.ps f28, 480(x9)\n"
			"flw.ps f1, 928(x9)\n"
			"fmaxu.pi f31, f28, f1\n" 
			"la x10, rand_int_32\n"
			"flw.ps f7, 352(x10)\n"
			"flw.ps f17, 0(x10)\n"
			"fltu.pi f24, f7, f17\n" 
			"LBL_SEQID_1_M0_WRITE_ID_1_HID_0:\n"
			"mov.m.x m4, x13, 0x45\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_1_M0_WRITE_ID_2_HID_0:\n"
			"fltm.pi m7, f15, f24\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x13, rand_ieee754_32\n"
			"flw.ps f1, 32(x13)\n"
			"flw.ps f28, 64(x13)\n"
			"flw.ps f8, 928(x13)\n"
			"fnmsub.ps f13, f1, f28, f8, rtz\n" 
			"LBL_SEQID_1_M0_WRITE_ID_3_HID_0:\n"
			"mov.m.x m7, x18, 0xf\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0xc1d981073a\n"
			"fsw.ps f0, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ITER_0_HID_0:\n"
			"li x5, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x0 << TENSOR_IMA_B_NUM_COLS) | (0xd << TENSOR_IMA_A_NUM_ROWS) | (0x6 << TENSOR_IMA_A_NUM_COLS) | (0xd << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x7d << TENSOR_IMA_SCP_LOC_B) | (0xb4 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x0 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ITER_1_HID_0:\n"
			"li x5, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x2 << TENSOR_IMA_B_NUM_COLS) | (0x7 << TENSOR_IMA_A_NUM_ROWS) | (0x9 << TENSOR_IMA_A_NUM_COLS) | (0xd << TENSOR_IMA_OFFSET) | (0x0 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0xb6 << TENSOR_IMA_SCP_LOC_B) | (0x67 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x0 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ITER_2_HID_0:\n"
			"li x5, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x0 << TENSOR_IMA_B_NUM_COLS) | (0x3 << TENSOR_IMA_A_NUM_ROWS) | (0x1 << TENSOR_IMA_A_NUM_COLS) | (0xb << TENSOR_IMA_OFFSET) | (0x0 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x30 << TENSOR_IMA_SCP_LOC_B) | (0xbc << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x0 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_0\n"
			"csrw tensor_error, x0\n"
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_0\n"
			"nop \n"
			"nop \n"
			"addi x29, x29, -1\n"
			"bne x29, x0, LBL_OUTER_LOOP_SEQ_HID_0\n"
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
			"csrwi excl_mode, 1\n"
			"csrwi tensor_wait, 0x6\n" 
			"fence\n"
			"li x5, 0x80000000004000f\n"
			"csrw   evict_sw, x5\n" 
			"li x5, 0x80000000004004f\n"
			"csrw   evict_sw, x5\n" 
			"li x5, 0x80000000004008f\n"
			"csrw   evict_sw, x5\n" 
			"li x5, 0x8000000000400cf\n"
			"csrw   evict_sw, x5\n" 
			"csrwi tensor_wait, 0x6\n" 
			"csrrci x0, mcache_control, 2\n"
			"csrwi tensor_wait, 0x6\n" 
			"fence\n"
			"csrrci x0, mcache_control, 1\n"
			"csrwi tensor_wait, 0x6\n" 
			"fence\n"
			"csrwi excl_mode, 0\n"
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
			"LBL_HPM_3_CORE_TL_INST_FAIL_HID_0:\n"          
			"csrr x5, mhpmcounter3\n"
			"sd x5, 32 (x31)\n"
			"LBL_HPM_4_CORE_TFMA_INST_FAIL_HID_0:\n"          
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
			"LBL_HPM_3_CORE_TL_INST_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter3\n"
			"sd x5, 32 (x31)\n"
			"LBL_HPM_4_CORE_TFMA_INST_PASS_HID_0:\n"          
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
			"csrwi mhpmevent3, 13\n"   
			"csrwi mhpmevent4, 9\n"   
			"csrwi mhpmevent5, 23\n"   
			"csrwi mhpmevent6, 3\n"   
			"csrwi mhpmevent7, 2\n"   
			"csrwi mhpmevent8, 12\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f31,  480 (x5)\n"
			"flw.ps f27,  128 (x5)\n"
			"flw.ps f26,  416 (x5)\n"
			"flw.ps f17,  0 (x5)\n"
			"flw.ps f6,  896 (x5)\n"
			"flw.ps f30,  672 (x5)\n"
			"flw.ps f25,  832 (x5)\n"
			"flw.ps f5,  800 (x5)\n"
			"flw.ps f16,  576 (x5)\n"
			"flw.ps f28,  224 (x5)\n"
			"flw.ps f10,  352 (x5)\n"
			"flw.ps f19,  448 (x5)\n"
			"flw.ps f12,  320 (x5)\n"
			"flw.ps f11,  608 (x5)\n"
			"flw.ps f18,  768 (x5)\n"
			"flw.ps f13,  544 (x5)\n"
			"flw.ps f0,  64 (x5)\n"
			"flw.ps f20,  96 (x5)\n"
			"flw.ps f23,  32 (x5)\n"
			"flw.ps f7,  288 (x5)\n"
			"flw.ps f15,  736 (x5)\n"
			"flw.ps f1,  160 (x5)\n"
			"flw.ps f22,  704 (x5)\n"
			"flw.ps f29,  512 (x5)\n"
			"flw.ps f3,  864 (x5)\n"
			"flw.ps f2,  192 (x5)\n"
			"flw.ps f8,  928 (x5)\n"
			"flw.ps f24,  992 (x5)\n"
			"flw.ps f21,  256 (x5)\n"
			"flw.ps f9,  640 (x5)\n"
			"flw.ps f14,  960 (x5)\n"
			"flw.ps f4,  384 (x5)\n"
			"csrw fflags, zero\n"
			"csrwi frm, 0x0\n"
			"LBL_KERNEL_HID_1:\n"          
			"li x5, 0xFEEDFEED\n"
			"slti x0, x0, 0x7f2\n"
			"la x31, check_sum_hid_1\n"
			"li x5, 0x2\n" 
			"sw x5, (x31)\n"
			"li x29, 0x8\n"
			"LBL_OUTER_LOOP_SEQ_HID_1:\n"
			"nop \n"
			"nop \n"
			"li x30, 0\n"
			"addi x30, x30, 1\n"
			"la x15, rand_int_32\n"
			"flw.ps f31, 672(x15)\n"
			"flw.ps f15, 416(x15)\n"
			"fmaxu.pi f15, f31, f15\n" 
			"mova.x.m x13\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f11, 800(x7)\n"
			"flw.ps f22, 576(x7)\n"
			"fmin.s f22, f11, f22\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f3, 960(x25)\n"
			"flw.ps f16, 544(x25)\n"
			"fsub.s f22, f3, f16, rup\n" 
			"LBL_SEQID_0_FP_TRANS_ID_0_HID_1:\n"
			"la x13, rand_ieee754_32\n"
			"flw.ps f3, 160(x13)\n"
			"flog.ps f4, f3\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f13, 320(x6)\n"
			"flw.ps f16, 288(x6)\n"
			"fsgnjn.s f3, f13, f16\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f20, 160(x15)\n"
			"flw.ps f22, 576(x15)\n"
			"flw.ps f17, 736(x15)\n"
			"fnmadd.ps f15, f20, f22, f17, rmm\n" 
			"maskpopc x25, m6\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_1:\n"
			"fsetm.pi m6, f17\n" 
			"li x5, 0x80340108\n"
			"ld x5, 0(x5)\n"
			"la x9, rand_ieee754_32\n"
			"flw.ps f14, 288(x9)\n"
			"flw.ps f15, 608(x9)\n"
			"fsgnjx.s f11, f14, f15\n" 
			"la x13, rand_int_32\n"
			"flw.ps f12, 992(x13)\n"
			"flw.ps f5, 96(x13)\n"
			"fsub.pi f26, f12, f5\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f31, 64(x9)\n"
			"flw.ps f4, 960(x9)\n"
			"flw.ps f3, 416(x9)\n"
			"fnmadd.ps f31, f31, f4, f3, rmm\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f17, 256(x12)\n"
			"flw.ps f31, 160(x12)\n"
			"flw.ps f15, 640(x12)\n"
			"fmadd.ps f19, f17, f31, f15, rdn\n" 
			"la x7, rand_int_32\n"
			"flw.ps f17, 640(x7)\n"
			"faddi.pi f4, f17, 0xe\n" 
			"LBL_SEQID_0_M0_WRITE_ID_2_HID_1:\n"
			"maskand m4, m3, m3\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_0_M0_WRITE_ID_3_HID_1:\n"
			"mova.m.x x11\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"csrw tensor_error, zero\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_1\n"
			"addi x30, x30, 1\n"
			"maskpopc x15, m3\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f5, 640(x15)\n"
			"flw.ps f12, 256(x15)\n"
			"fmax.s f31, f5, f12\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f3, 768(x7)\n"
			"flw.ps f26, 672(x7)\n"
			"flw.ps f16, 512(x7)\n"
			"fmadd.s f22, f3, f26, f16, rtz\n" 
			"la x6, rand_int_32\n"
			"flw.ps f16, 128(x6)\n"
			"fsatu8.pi f19, f16\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f12, 608(x9)\n"
			"flw.ps f4, 768(x9)\n"
			"flw.ps f31, 960(x9)\n"
			"fnmsub.s f19, f12, f4, f31, rmm\n" 
			"LBL_SEQID_1_M0_WRITE_ID_4_HID_1:\n"
			"masknot m6, m4\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_1_M0_WRITE_ID_5_HID_1:\n"
			"mov.m.x m3, x12, 0x1b\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x15, rand_int_32\n"
			"flw.ps f20, 224(x15)\n"
			"faddi.pi f14, f20, 0x9f\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f3, 832(x15)\n"
			"flw.ps f11, 960(x15)\n"
			"fsgnj.s f16, f3, f11\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f5, 832(x6)\n"
			"flw.ps f22, 160(x6)\n"
			"fsub.s f10, f5, f22, rne\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f13, 832(x25)\n"
			"fsin.ps f11, f13\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f20, 32(x15)\n"
			"flw.ps f4, 704(x15)\n"
			"fmin.s f13, f20, f4\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f3, 160(x13)\n"
			"flw.ps f17, 480(x13)\n"
			"fsgnjx.ps f4, f3, f17\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f4, 288(x12)\n"
			"fcvt.w.s x12, f4, rup\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f17, 64(x27)\n"
			"fclass.ps f31, f17\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f31, 672(x11)\n"
			"flw.ps f11, 992(x11)\n"
			"fnmadd.s f26, f31, f11, f11, rup\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_1\n"
			"nop \n"
			"nop \n"
			"addi x29, x29, -1\n"
			"bne x29, x0, LBL_OUTER_LOOP_SEQ_HID_1\n"
			"la x31, check_sum_hid_1\n"
			"lw x5, (x31)\n"
			"bne x30, x5, LBL_FAIL_HID_1\n"
			"LBL_POSTAMBLE_HID_1:\n"          
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
			"LBL_HPM_3_CORE_L2_EVICT_REQ_REJ_FAIL_HID_1:\n"          
			"csrr x5, mhpmcounter3\n"
			"sd x5, 32 (x31)\n"
			"LBL_HPM_4_CORE_DCACHE_MISSES1_FAIL_HID_1:\n"          
			"csrr x5, mhpmcounter4\n"
			"sd x5, 40 (x31)\n"
			"LBL_HPM_5_CORE_FTRANS_OPS_FAIL_HID_1:\n"          
			"csrr x5, mhpmcounter5\n"
			"sd x5, 48 (x31)\n"
			"LBL_HPM_6_CORE_RETIRED_INST1_FAIL_HID_1:\n"          
			"csrr x5, mhpmcounter6\n"
			"sd x5, 56 (x31)\n"
			"LBL_HPM_7_NEIGH_ANY_CORE_RX_ETL_RSP_FAIL_HID_1:\n"          
			"csrr x5, mhpmcounter7\n"
			"sd x5, 64 (x31)\n"
			"LBL_HPM_8_NEIGH_INTER_CORE_MSG_TX_FAIL_HID_1:\n"          
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
			"LBL_HPM_3_CORE_L2_EVICT_REQ_REJ_PASS_HID_1:\n"          
			"csrr x5, mhpmcounter3\n"
			"sd x5, 32 (x31)\n"
			"LBL_HPM_4_CORE_DCACHE_MISSES1_PASS_HID_1:\n"          
			"csrr x5, mhpmcounter4\n"
			"sd x5, 40 (x31)\n"
			"LBL_HPM_5_CORE_FTRANS_OPS_PASS_HID_1:\n"          
			"csrr x5, mhpmcounter5\n"
			"sd x5, 48 (x31)\n"
			"LBL_HPM_6_CORE_RETIRED_INST1_PASS_HID_1:\n"          
			"csrr x5, mhpmcounter6\n"
			"sd x5, 56 (x31)\n"
			"LBL_HPM_7_NEIGH_ANY_CORE_RX_ETL_RSP_PASS_HID_1:\n"          
			"csrr x5, mhpmcounter7\n"
			"sd x5, 64 (x31)\n"
			"LBL_HPM_8_NEIGH_INTER_CORE_MSG_TX_PASS_HID_1:\n"          
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
		0xad93e59c, 
		0x6b8a5a8f, 
		0x31c91718, 
		0xcdee42ff, 
		0xf0e61d51, 
		0xf1c1b3ef, 
		0xcde7b1d5, 
		0x5d229041, 
		0x7aea8812, 
		0x540433f5, 
		0x8f46b13e, 
		0x15cf5a04, 
		0xd0af5e8a, 
		0x4d136a00, 
		0x7b42d187, 
		0x0810c25e, 
		0x11ac16fa, 
		0x96cdc816, 
		0xb5b6a37d, 
		0xd94780f3, 
		0x552d2c37, 
		0x792b80f4, 
		0x8b94d10c, 
		0x7a0938b5, 
		0x432c34ac, 
		0xf0d8bc09, 
		0x90e80a98, 
		0x11fb4567, 
		0x8641fa2b, 
		0x7a86ade9, 
		0x3c52975f, 
		0x86031f26, 
		0xa54bd2ef, 
		0xbb3cdfae, 
		0x9adb1221, 
		0x1ba6ff75, 
		0x62ccb691, 
		0x5f77bf55, 
		0x2fd241f4, 
		0xa0cb1497, 
		0x20c77f68, 
		0xc952c672, 
		0xa13538a7, 
		0x302dee9a, 
		0x6db544ee, 
		0x4be17c72, 
		0xa779b8ab, 
		0x41df3fcd, 
		0x078d1657, 
		0x1140f818, 
		0xdcf08dec, 
		0x5789845c, 
		0xf0d66024, 
		0x6a6e084f, 
		0xf61704e7, 
		0x419d4903, 
		0x8b2d9ede, 
		0x605e5816, 
		0x4fc27572, 
		0x91ff0b68, 
		0x9f23550e, 
		0xb3f5a0e6, 
		0x7f3a30bf, 
		0x130aa79b, 
		0xb13357f9, 
		0x48dce72e, 
		0x17d614bd, 
		0x73c64019, 
		0x8ff1abd0, 
		0x3b4d15da, 
		0xd859cbb7, 
		0x9689f0a1, 
		0xf262f679, 
		0xebd49e7a, 
		0xe9a75007, 
		0xb6887e1e, 
		0xee9d935d, 
		0x00c53b21, 
		0x73f1ba3c, 
		0xcab96f37, 
		0x7d723a76, 
		0x07e5f6ee, 
		0x8465b610, 
		0x42c13dd5, 
		0x6bbc5465, 
		0xfe0b6538, 
		0xce33d04b, 
		0xd7750432, 
		0x22c71e83, 
		0x84428ae0, 
		0x4e0ef8da, 
		0xbd00ff81, 
		0x2cb77e58, 
		0x02328bba, 
		0x3bfcca4a, 
		0x6271062d, 
		0x3833f142, 
		0x3db546b7, 
		0xf6f0245a, 
		0x5eabe0ab, 
		0xc423586f, 
		0xd016c284, 
		0x167e59bf, 
		0x38426c52, 
		0xa1fe090a, 
		0x2633e3da, 
		0x811715ea, 
		0x323a0029, 
		0x0c026532, 
		0x58d19685, 
		0x793ededf, 
		0xcf4a45e1, 
		0x5f1aee5a, 
		0x98d6d12a, 
		0x1ef4f8e7, 
		0x887ab124, 
		0x0e997483, 
		0xd197318e, 
		0x3e1baacb, 
		0xbf5f3bcc, 
		0x3d94efe9, 
		0xb0138f33, 
		0x16ba5e9d, 
		0xdca2e344, 
		0x915b6e35, 
		0x6b53771d, 
		0x1e1a6058, 
		0xe3dadd6f, 
		0xa091003a, 
		0xadd68c42, 
		0x592f908f, 
		0x5c63e58d, 
		0x2d6f645c, 
		0xe4328849, 
		0x77e37d55, 
		0x696c85b6, 
		0x5ba4a642, 
		0x34f1a878, 
		0xa13a6000, 
		0x10a41687, 
		0xab9c3c05, 
		0x2c66821a, 
		0xf41cc264, 
		0xcf4f4e95, 
		0xa40e93af, 
		0xaac222e4, 
		0x68243755, 
		0xffb8f76c, 
		0x3a7d91a9, 
		0xadedfee5, 
		0x9864f167, 
		0x5ba655db, 
		0x309547b9, 
		0x3c9214d6, 
		0x50cb0d6b, 
		0xc19680c4, 
		0xa0071b2e, 
		0x21d1cfa0, 
		0xcf2495cf, 
		0xc343795f, 
		0x4a6dc3b1, 
		0xc9e7be00, 
		0xf55bdf97, 
		0x2bdae841, 
		0x8c098691, 
		0xa0510176, 
		0x6d588bbd, 
		0xd18b34eb, 
		0x126ead5d, 
		0xc79ec821, 
		0x44f3e516, 
		0x1632878c, 
		0xc02eca1f, 
		0x118fc452, 
		0xb165241c, 
		0x884494ac, 
		0xa9aaa39f, 
		0xd25a7c85, 
		0xd27eb7b2, 
		0xe9c7015e, 
		0x0f953493, 
		0xd01b71b5, 
		0x4c60276d, 
		0xe5f69452, 
		0x314f7413, 
		0x69e9b35c, 
		0x1d04ffdd, 
		0x139710f7, 
		0x2b3417b6, 
		0xeb268d93, 
		0x03f0144f, 
		0xd6ab4ed4, 
		0x3bca8e08, 
		0xbafd3869, 
		0x40ddb2e9, 
		0x9b042a67, 
		0x438e710c, 
		0x09f1651e, 
		0x3409ce05, 
		0xbc94281b, 
		0xaf9d5c08, 
		0x57f800e0, 
		0xcc6d33e4, 
		0x6a50021e, 
		0x33a6f2ad, 
		0xb397230a, 
		0x733795af, 
		0x1ec045d0, 
		0x10199e99, 
		0xfc4ad678, 
		0x12d1469d, 
		0x6b285083, 
		0x946812fd, 
		0xc95d368d, 
		0xaf1ec3cf, 
		0x04a8691e, 
		0x53f568a5, 
		0x0856a7d2, 
		0xf5b732e8, 
		0x72a108ce, 
		0x5a2e9b89, 
		0x6e363e3d, 
		0x08c87429, 
		0x3a1d31b6, 
		0x3d7538b6, 
		0x95cde10e, 
		0x5497c427, 
		0x0b8e0579, 
		0xba2e69a0, 
		0x53f6f2af, 
		0x84371d69, 
		0xc57c7365, 
		0x00fcb374, 
		0x68ce1675, 
		0xce429522, 
		0xc3d64ad4, 
		0xe6797249, 
		0xcd02d89b, 
		0x3a0a8b7d, 
		0xd51b3751, 
		0x491c6465, 
		0x1be1f440, 
		0x127a821f, 
		0x79e8454a, 
		0x6f400819, 
		0x629da641, 
		0xb39c5d20, 
		0xcabdfb98, 
		0x9cb18fdf, 
		0x5e1687af, 
		0xea51419e, 
		0x119167d8, 
		0xb223746c, 
		0x3a13221e, 
		0xa72016bc, 
		0xc9fdfa1f  
};
volatile uint32_t rand_ieee754_32[1536] __attribute__ ((aligned (2048))) = {
		0x41600000, 
		0x40000000, 
		0x7f800000, 
		0xc1d80000, 
		0x41900000, 
		0x0c7fffff, 
		0x42040000, 
		0x80010000, 
		0x1f8e800e, 
		0x7f7ffffe, 
		0x7fc00000, 
		0x80800001, 
		0x4979ff5e, 
		0x2abad76c, 
		0x688e51a0, 
		0x41000000, 
		0xcb000000, 
		0x459c5042, 
		0xcf528453, 
		0x80076369, 
		0xff7ffffe, 
		0x44b5b651, 
		0x7fba1ba5, 
		0x0c7fffc0, 
		0x00200000, 
		0xc1100000, 
		0xef2558f9, 
		0x00000010, 
		0x0e0007ff, 
		0x0e000001, 
		0x00000001, 
		0x00000000, 
		0x525a05f4, 
		0x3f028f5c, 
		0x404d0735, 
		0x00000200, 
		0xa1d33bec, 
		0xff800000, 
		0x42080000, 
		0x42540000, 
		0x0e00ffff, 
		0xc1200000, 
		0x6dbaaba0, 
		0x80800000, 
		0x80000000, 
		0xc1b80000, 
		0x734dfbc5, 
		0xc1f00000, 
		0x7fe88e3f, 
		0x80000010, 
		0x7f800000, 
		0x422c0000, 
		0x8be86e47, 
		0x376a4982, 
		0xc24c0000, 
		0xc1c80000, 
		0xc1600000, 
		0x7fbfffff, 
		0x00000000, 
		0x422c0000, 
		0x0c700000, 
		0x0c7ffff0, 
		0x41900000, 
		0x4f996dc1, 
		0x326c3fe6, 
		0x33873ff7, 
		0xffaed67f, 
		0x6b969ccd, 
		0x80800000, 
		0x00800001, 
		0x3f800001, 
		0xcccccccc, 
		0x00800001, 
		0x0c700000, 
		0xaa6f410a, 
		0xc2180000, 
		0x7fbc78a9, 
		0x786faef4, 
		0x0c600000, 
		0x00800003, 
		0x85ded243, 
		0x0c7fff80, 
		0x0c7ff800, 
		0x41c00000, 
		0x80020000, 
		0xc2240000, 
		0x8f7fffff, 
		0x253ef706, 
		0xc0c00000, 
		0x80000002, 
		0x80000080, 
		0x00000000, 
		0x80800001, 
		0x00000000, 
		0x00040000, 
		0xc2080000, 
		0x7f800000, 
		0x10e444a3, 
		0x7f800000, 
		0x68e39f1e, 
		0x4b8c4b40, 
		0x80000000, 
		0x0e0003ff, 
		0x00000000, 
		0xff800000, 
		0x80001000, 
		0x0e000fff, 
		0x41300000, 
		0x176424ee, 
		0xff800001, 
		0x42400000, 
		0xc24c0000, 
		0x8f7fffff, 
		0x00ffffff, 
		0xff800000, 
		0x2080b3ed, 
		0xc2040000, 
		0x80000000, 
		0xff7fffff, 
		0x0c7ffffc, 
		0x80000000, 
		0xf4659cee, 
		0xbf800001, 
		0x41a80000, 
		0x8f7ffffc, 
		0xbc02e63a, 
		0x80200000, 
		0x80800001, 
		0x42340000, 
		0x42540000, 
		0x0e0007ff, 
		0xff7fffff, 
		0x80000800, 
		0xff7fffff, 
		0x40800000, 
		0x9f710ea4, 
		0x0c7fffc0, 
		0x7488835e, 
		0x10467afa, 
		0x1bc4c25e, 
		0x7fbfffff, 
		0x0e1db5f5, 
		0x0c7fff00, 
		0xff800000, 
		0x7f7fffff, 
		0x22f2a97b, 
		0x0c7c0000, 
		0xff800000, 
		0xa9f769a8, 
		0x80000000, 
		0xaaaaaaaa, 
		0xb07fe0d6, 
		0x80249d67, 
		0xff800000, 
		0x0c7fff00, 
		0xd6abad8a, 
		0xc2540000, 
		0x01079792, 
		0x41400000, 
		0x7f800000, 
		0x7f800000, 
		0xfff630c0, 
		0x80800003, 
		0x80000001, 
		0x41500000, 
		0x909eea5d, 
		0x00000001, 
		0xff800000, 
		0xc24c0000, 
		0x3d51bfb8, 
		0xe94ce86f, 
		0x42700000, 
		0x060e062f, 
		0x41700000, 
		0xc21c0000, 
		0xc2080000, 
		0x424c0000, 
		0x0e007fff, 
		0x9bf91048, 
		0x94f302e4, 
		0x4da238a3, 
		0x4482c365, 
		0x55cefb0c, 
		0x80800000, 
		0x423c0000, 
		0x7f800000, 
		0xffc00000, 
		0xff800000, 
		0x7f800000, 
		0x7fffffff, 
		0x80000000, 
		0xbf0b9537, 
		0x80000000, 
		0x392cdab6, 
		0x1dba870a, 
		0xffbfffff, 
		0x227416f4, 
		0xc2340000, 
		0x41d80000, 
		0x0c600000, 
		0xc2640000, 
		0xff800000, 
		0x00000800, 
		0x7f7ffffe, 
		0x80000020, 
		0xc2100000, 
		0x00400000, 
		0x0e03ffff, 
		0x0c780000, 
		0xbf028f5c, 
		0x0c600000, 
		0xb5c7ab80, 
		0x9cbebeca, 
		0x00004000, 
		0x00ffffff, 
		0xffc00001, 
		0x41200000, 
		0x4b8c4b40, 
		0x0053f3f1, 
		0x0e7fffff, 
		0x4c12143a, 
		0xff800000, 
		0x0032f0e7, 
		0x41600000, 
		0x80000000, 
		0x41a80000, 
		0xc2700000, 
		0xff7fffff, 
		0x1431dda1, 
		0x7f7ffffe, 
		0x00001000, 
		0x80000400, 
		0x0e0000ff, 
		0xff800000, 
		0x854f0a21, 
		0x95397568, 
		0x7fc00000, 
		0x0019f646, 
		0x80000020, 
		0x41980000, 
		0xa6b2cbcd, 
		0x41200000, 
		0x80000000, 
		0x00ffffff, 
		0x0548617e, 
		0xab1573f3, 
		0x14372be6, 
		0x0056d4f5, 
		0x8f7ffffc, 
		0x80000000, 
		0x7f800001, 
		0x00001000, 
		0xfea7fc25, 
		0xff853c26, 
		0xc1a80000, 
		0xa5fa0531, 
		0x0e3fffff, 
		0xc1d80000, 
		0x3f028f5c, 
		0x80000010, 
		0x23a7b982, 
		0x41e00000, 
		0x7f800000, 
		0x33333333, 
		0x8f7fffff, 
		0x00800002, 
		0xc1b00000, 
		0x00800001, 
		0x00800003, 
		0x00000000, 
		0x1b7460b6, 
		0x6da402fc, 
		0xc2000000, 
		0x9da8ad8e, 
		0xb0ca1066, 
		0x7fc00001, 
		0xc2300000, 
		0x546816e2, 
		0xbf800001, 
		0xc81281e2, 
		0xf7c5b446, 
		0xb916f763, 
		0xc2040000, 
		0x2b364f91, 
		0xdfc724f8, 
		0x9de72a9d, 
		0x80000000, 
		0x6d5d2346, 
		0x7f7ffffe, 
		0x3f028f5c, 
		0xc09f6ded, 
		0x00080000, 
		0x2b4a14a2, 
		0xff000000, 
		0x41600000, 
		0xc2180000, 
		0x0c7fc000, 
		0x80100000, 
		0x00000000, 
		0xc1f80000, 
		0x00000200, 
		0xc1900000, 
		0xffdc272d, 
		0x41980000, 
		0xff800000, 
		0x42240000, 
		0xae4e4a15, 
		0x80000000, 
		0xea931760, 
		0x80800003, 
		0xc1b80000, 
		0x02426e77, 
		0x423c0000, 
		0xf87f43c0, 
		0xe0fc0117, 
		0x7f800000, 
		0xbf800000, 
		0x7f7ffffe, 
		0x80000400, 
		0x80008000, 
		0x452b2d71, 
		0x5ef3809a, 
		0x42140000, 
		0x41000000, 
		0x41100000, 
		0xc2140000, 
		0xff800000, 
		0x80800000, 
		0x00000000, 
		0xff7fffff, 
		0x0d000ff0, 
		0xb6f32780, 
		0x41980000, 
		0x41a00000, 
		0x80000000, 
		0xffffffff, 
		0x7f800000, 
		0x0c7fffc0, 
		0xc0400000, 
		0x7f800001, 
		0x42380000, 
		0x62747ba5, 
		0x00800001, 
		0xc2500000, 
		0x00000002, 
		0x00000000, 
		0x0c7fff00, 
		0x80000001, 
		0x00000000, 
		0xff800000, 
		0xff800000, 
		0x42200000, 
		0x00001000, 
		0x41a80000, 
		0xc2400000, 
		0xc23c0000, 
		0xc93cdce0, 
		0x6c936954, 
		0x00800003, 
		0x7f800000, 
		0xd1a4684a, 
		0x1cd50a74, 
		0xcc5795be, 
		0x80011111, 
		0x219aa51a, 
		0x68d21fc9, 
		0x9002a953, 
		0xff800000, 
		0x41200000, 
		0x80040000, 
		0xff800001, 
		0xeb992074, 
		0xffbdac85, 
		0x41500000, 
		0xc26c0000, 
		0x427c0000, 
		0xc2000000, 
		0x80800003, 
		0x776bed9c, 
		0x41a80000, 
		0x55555555, 
		0x00000200, 
		0xb5bdd00a, 
		0x80000800, 
		0x41400000, 
		0xc2500000, 
		0x0c780000, 
		0x41300000, 
		0xf5fd899d, 
		0x80000004, 
		0x0e3fffff, 
		0x40e00000, 
		0xc2400000, 
		0x4d89a51f, 
		0xff9647b0, 
		0x00000001, 
		0xa1313694, 
		0x80ffffff, 
		0xed62a252, 
		0x0c7ff800, 
		0x015b3aaa, 
		0x80800000, 
		0x0c7ffe00, 
		0x68060753, 
		0x41a80000, 
		0x9f1eda0b, 
		0x804fc3c4, 
		0x3e4a5b41, 
		0x80000000, 
		0xc759bc03, 
		0x807ffffe, 
		0x9a966f3f, 
		0x42100000, 
		0x7f7ffffe, 
		0x80800002, 
		0x7b891b91, 
		0x95be6d8e, 
		0x41f00000, 
		0x0e7fffff, 
		0x342166f1, 
		0x41f80000, 
		0x8eb56205, 
		0xc1b00000, 
		0x42740000, 
		0xbe47697a, 
		0xffa48653, 
		0x41d80000, 
		0x54b8172d, 
		0xf2a0e2ef, 
		0x80000002, 
		0xed1c71d4, 
		0xc2700000, 
		0x00020000, 
		0x42480000, 
		0x9cee5ceb, 
		0x0d00fff0, 
		0x00080000, 
		0x0e001fff, 
		0x7f800000, 
		0x007fffff, 
		0x40a00000, 
		0x80000000, 
		0xe62c7d87, 
		0x9418093b, 
		0x80000001, 
		0x0e07ffff, 
		0x7fc00000, 
		0x00800003, 
		0x0e0000ff, 
		0xc1b80000, 
		0x19f0545d, 
		0xc1a80000, 
		0x3f38c910, 
		0xc1900000, 
		0x8073afbc, 
		0x7304f5da, 
		0x0e07ffff, 
		0x00800001, 
		0x80000000, 
		0xc1200000, 
		0x7f800000, 
		0xeedef790, 
		0x7f7fffff, 
		0x423c0000, 
		0x7f7fffff, 
		0xc36f2adf, 
		0x33333333, 
		0xa0d1840c, 
		0x0e00003f, 
		0x8f7fffff, 
		0x80200000, 
		0xc27c0000, 
		0xc22c0000, 
		0xcd6b707b, 
		0xc27c0000, 
		0x80100000, 
		0xa30c2c3d, 
		0x41900000, 
		0xf43d0324, 
		0x43b60b7e, 
		0x80000080, 
		0x42200000, 
		0x0c7f0000, 
		0x80000000, 
		0x80000400, 
		0x475c95aa, 
		0x80000008, 
		0x02213e7e, 
		0xc0a00000, 
		0x21a3141c, 
		0x5e6a12d3, 
		0x758937af, 
		0x41f00000, 
		0x42240000, 
		0x83b928e3, 
		0x0f7ffffc, 
		0x0d7daebf, 
		0x65f27713, 
		0x0f7a36c5, 
		0x42680000, 
		0x1160ccb8, 
		0xff800000, 
		0x7fc1f435, 
		0xfb75fb91, 
		0xc1a00000, 
		0xd0538243, 
		0x420c0000, 
		0x0c7fff00, 
		0x6c3bb799, 
		0xff800000, 
		0xae4e97fd, 
		0x80100000, 
		0xe5c3b739, 
		0x880c18a0, 
		0x0c7ffffe, 
		0xc27c0000, 
		0xff800000, 
		0x420c0000, 
		0xc2340000, 
		0x7f7ffffe, 
		0x62ef3b63, 
		0x4bdbb821, 
		0xd0ba98da, 
		0x0c7ff000, 
		0xdcc250e5, 
		0x6288d2a4, 
		0x8f7ffffd, 
		0x7f7fffff, 
		0xc2200000, 
		0x00000000, 
		0x1ab3088a, 
		0x0c700000, 
		0x00200000, 
		0x41e80000, 
		0x0c7fffc0, 
		0x00011111, 
		0x8f7ffffe, 
		0x40400000, 
		0x71985398, 
		0x41500000, 
		0xc1900000, 
		0x7f7ffffe, 
		0x7f800000, 
		0x362925f6, 
		0x427c0000, 
		0xf371c514, 
		0x64ce6857, 
		0x542623b8, 
		0x80ffffff, 
		0x00000000, 
		0x80000020, 
		0xa893c8ed, 
		0x15a3056e, 
		0xc1800000, 
		0xadeef6ec, 
		0x6ed438ac, 
		0x80e3f8b3, 
		0xff800000, 
		0x00000000, 
		0x73b80cb8, 
		0xc52e8e32, 
		0x860c4594, 
		0x0c7ffffc, 
		0xfb0d1d8c, 
		0x2783eb97, 
		0x00000020, 
		0x41000000, 
		0x80000002, 
		0x80800002, 
		0x88430b41, 
		0x426c0000, 
		0x40000000, 
		0xa9b40881, 
		0x0e03ffff, 
		0xc07fc36a, 
		0xff000000, 
		0xdbba13cf, 
		0x4f0ddf4b, 
		0xc2780000, 
		0x13e80678, 
		0xff000000, 
		0x80000100, 
		0x9c71ab78, 
		0xbf028f5c, 
		0x423c0000, 
		0x00000000, 
		0xf5abe019, 
		0x41980000, 
		0x1eb29f68, 
		0x41980000, 
		0x42080000, 
		0x0c7fff00, 
		0xc22c0000, 
		0xa150bb78, 
		0xc2040000, 
		0x80800001, 
		0x427c0000, 
		0xff7ffffe, 
		0xc1100000, 
		0x80011111, 
		0x00001000, 
		0x42200000, 
		0x00000002, 
		0xefb60299, 
		0xbf800001, 
		0xc1c80000, 
		0x00000002, 
		0x134875fa, 
		0x0e00003f, 
		0x00000080, 
		0x00000008, 
		0x52fdf0af, 
		0x42140000, 
		0xc2100000, 
		0x0c7ffff0, 
		0x0c7fff00, 
		0x0c7fe000, 
		0x73e299ef, 
		0x42040000, 
		0x80000000, 
		0xf01522e1, 
		0x0e01ffff, 
		0xff800000, 
		0x0c700000, 
		0x0e00ffff, 
		0x00000100, 
		0xff800000, 
		0x00000001, 
		0x1acc3b86, 
		0x7fc8cd05, 
		0xc25c0000, 
		0xc1d00000, 
		0x7f800000, 
		0xbd0c5289, 
		0xff800000, 
		0x5934326c, 
		0xc1200000, 
		0x00000000, 
		0xcd2d1453, 
		0x0e007fff, 
		0xc27c0000, 
		0xcb5f1a98, 
		0x8f7ffffe, 
		0x00ffffff, 
		0xe7783341, 
		0xc2200000, 
		0x5b687e18, 
		0x0e03ffff, 
		0x00000020, 
		0xc23c0000, 
		0x0f7ffffe, 
		0x40800000, 
		0x0e000fff, 
		0x00010000, 
		0x80000001, 
		0xc2180000, 
		0x0e7fffff, 
		0x7edf0b1f, 
		0xc1b00000, 
		0x422c0000, 
		0x80000000, 
		0xc1a00000, 
		0x42140000, 
		0x0c7ffffc, 
		0xff7ffffe, 
		0xff834b3a, 
		0x9f89bf9d, 
		0xe9979299, 
		0xc1e00000, 
		0x80100000, 
		0xc2340000, 
		0xc2100000, 
		0x0c7ffffc, 
		0x58ffe7a1, 
		0xc2080000, 
		0x80000001, 
		0x0c780000, 
		0x80000000, 
		0x31ba8547, 
		0xbf800001, 
		0x9383cc2f, 
		0xff835141, 
		0xc2000000, 
		0x80000000, 
		0x00000020, 
		0x41600000, 
		0x80080000, 
		0xc26c0000, 
		0xc2480000, 
		0xffcbaa72, 
		0x2c159957, 
		0x00200000, 
		0x00000001, 
		0x18a77d7d, 
		0x4ded62e7, 
		0x0c7e0000, 
		0xdd6f92c2, 
		0x42340000, 
		0xfb72c5eb, 
		0x7f800000, 
		0x7f7fffff, 
		0x60439b09, 
		0x80800001, 
		0x80000000, 
		0x0c7fff00, 
		0xc4e4edf0, 
		0x94d69b33, 
		0xfa8f3da2, 
		0x003d5f52, 
		0x00000020, 
		0x41c00000, 
		0xc58129fb, 
		0xa238f81a, 
		0x00000001, 
		0x00000000, 
		0x00000000, 
		0x2ecf5a79, 
		0xc25c0000, 
		0x42380000, 
		0x80000100, 
		0x00000020, 
		0xff9869af, 
		0xc2240000, 
		0xc0a00000, 
		0xc0000000, 
		0x80000000, 
		0xc9d41e00, 
		0x42480000, 
		0x42400000, 
		0x7fffffff, 
		0x41a00000, 
		0x00000000, 
		0x42140000, 
		0x80000000, 
		0x0010ede5, 
		0x0c7fff80, 
		0x80001000, 
		0xe93c7183, 
		0xee6d1803, 
		0x00000000, 
		0xc0000000, 
		0x416ca06d, 
		0x01093078, 
		0x00000200, 
		0x00080000, 
		0xff800000, 
		0x0c400000, 
		0x80000000, 
		0x0e00007f, 
		0x80800000, 
		0xc0400000, 
		0x427c0000, 
		0x80655493, 
		0xbf800001, 
		0x1c25a413, 
		0x4b000000, 
		0x007ffffe, 
		0x42100000, 
		0x80000000, 
		0x403af14e, 
		0x8d57b12f, 
		0x41100000, 
		0xff7ffffe, 
		0x41a00000, 
		0xb8c3fe16, 
		0x41980000, 
		0x7b02d17d, 
		0x00001000, 
		0x40000000, 
		0xf44ddaa7, 
		0x93e9c81e, 
		0x01a273c3, 
		0x5db55f31, 
		0x00000008, 
		0x0c7fffc0, 
		0xff7fffff, 
		0xff800000, 
		0x9dfe0e9e, 
		0x6c1f7c1e, 
		0x0e00000f, 
		0x7f7ffffe, 
		0x4b000000, 
		0x80000000, 
		0x7fc00000, 
		0x4b8c4b40, 
		0x3eca8ea6, 
		0xcccccccc, 
		0x40000000, 
		0x425c0000, 
		0xc2700000, 
		0x41d80000, 
		0xb3759c21, 
		0xff800000, 
		0x42480000, 
		0xff000000, 
		0x00000000, 
		0xc1a80000, 
		0x714af00b, 
		0x0066d57f, 
		0x0e01ffff, 
		0xbb660b98, 
		0x425c0000, 
		0x748c3e04, 
		0x80800003, 
		0x00000000, 
		0x4a33ab13, 
		0x0c7ff800, 
		0x80000000, 
		0x41b00000, 
		0x5cd029e3, 
		0x00000000, 
		0xe360c039, 
		0xcccccccc, 
		0x80200000, 
		0x42440000, 
		0xec5d6689, 
		0xc0a00000, 
		0xb40d5f9e, 
		0x7f000000, 
		0x00004000, 
		0xff7ffffe, 
		0x41600000, 
		0x0e003fff, 
		0x007ffffe, 
		0x0c7ff800, 
		0xe4095260, 
		0xffc00000, 
		0x7f7fffff, 
		0xff7fffff, 
		0x0c7ffffc, 
		0x80000000, 
		0x47a980da, 
		0x00000000, 
		0x80000000, 
		0x042ee20c, 
		0x70c4c4f8, 
		0x55555555, 
		0x221ef181, 
		0x4b000000, 
		0x807ffffe, 
		0x0c7e0000, 
		0xa08b1374, 
		0x427c0000, 
		0x7ff19df0, 
		0x7fbfffff, 
		0x0c7e0000, 
		0x98e5dc91, 
		0xc1980000, 
		0x00000000, 
		0x0e00007f, 
		0xc1880000, 
		0xb5374325, 
		0x1ec34987, 
		0x51b98b64, 
		0x0e000003, 
		0x00000000, 
		0x00000000, 
		0x420c0000, 
		0xeb5417ee, 
		0x5ea0eb97, 
		0x80000000, 
		0x73db3fe5, 
		0x6b09155c, 
		0xc23c0000, 
		0xebf42e80, 
		0x0c7ffe00, 
		0x6f4aabde, 
		0xc1980000, 
		0xc22c0000, 
		0x42640000, 
		0xc1b80000, 
		0x0e000007, 
		0xc3c9d65e, 
		0x3bd4fa1d, 
		0xff800000, 
		0xc0c00000, 
		0xe3c976b1, 
		0xff7ffffe, 
		0xc0a00000, 
		0x00000000, 
		0x427c0000, 
		0x0c7fff00, 
		0xb726144b, 
		0x41c80000, 
		0x00800000, 
		0x41900000, 
		0xc1880000, 
		0x7f800000, 
		0x8b88373d, 
		0xf45d067e, 
		0xe7be6caa, 
		0x00000008, 
		0x80000000, 
		0x41800000, 
		0xc2000000, 
		0x80000000, 
		0x41c80000, 
		0x41600000, 
		0x41d80000, 
		0x7f7ffffe, 
		0xff800000, 
		0x00800003, 
		0x41d00000, 
		0x1209d16c, 
		0x00000400, 
		0x9be5a1b3, 
		0xc0000000, 
		0x80000400, 
		0x80000000, 
		0xffc00001, 
		0xf0bd82b8, 
		0x42300000, 
		0xc1100000, 
		0x0f7fffff, 
		0xc2140000, 
		0x00000100, 
		0xc2040000, 
		0x52ebdde0, 
		0x0e0000ff, 
		0x007fffff, 
		0x8e7bac68, 
		0xc2580000, 
		0x00000200, 
		0x8f7fffff, 
		0x0e000001, 
		0xd78e852e, 
		0x42440000, 
		0x7f89c3ec, 
		0x80000000, 
		0x12b443ec, 
		0x0e00007f, 
		0xc0000000, 
		0xc1900000, 
		0x80080000, 
		0xc24c0000, 
		0x9ff1823b, 
		0x4d8d4053, 
		0x80008000, 
		0x80010000, 
		0x0c7e0000, 
		0xcccccccc, 
		0x00000000, 
		0x00000000, 
		0x8523a2e7, 
		0x00002000, 
		0x00000080, 
		0x00400000, 
		0x0e000007, 
		0x80800001, 
		0x0c7e0000, 
		0x00001000, 
		0x41900000, 
		0xf19ff147, 
		0x0e000007, 
		0x0c600000, 
		0x80000400, 
		0xd1f548ae, 
		0x0d00fff0, 
		0x00000000, 
		0x40e00000, 
		0x7fffffff, 
		0x3f028f5c, 
		0x4b4369e9, 
		0x41700000, 
		0x7f800000, 
		0x8f7ffffd, 
		0x70f27cfa, 
		0xc2280000, 
		0xc1d00000, 
		0xe8fa7a24, 
		0x0e000003, 
		0x0c780000, 
		0x00000001, 
		0x00800000, 
		0x07e8d494, 
		0x0e1fffff, 
		0xf606d957, 
		0x01d3178b, 
		0x0f7ffffe, 
		0x80000000, 
		0xff7ffffe, 
		0x42500000, 
		0x41b00000, 
		0x00040000, 
		0x804b1f4e, 
		0xb70b53d0, 
		0x00ffffff, 
		0x97615122, 
		0x3f18d7d6, 
		0x0c7f8000, 
		0x00000000, 
		0x425c0000, 
		0x1236fd5f, 
		0x7f800000, 
		0x0c7f0000, 
		0x0c7c0000, 
		0x0e0003ff, 
		0x007fffff, 
		0xc2740000, 
		0x40800000, 
		0x41100000, 
		0x80000000, 
		0xb85c6d1b, 
		0x41d00000, 
		0x42240000, 
		0xc2300000, 
		0x3f800001, 
		0x82f61a7d, 
		0x8f7ffffe, 
		0x9e6bf8fb, 
		0x0c7ffffe, 
		0x00000000, 
		0x3568769c, 
		0x7fa8efd3, 
		0x80000000, 
		0x00080000, 
		0xc24c0000, 
		0x49b912f7, 
		0xc1a80000, 
		0x80200000, 
		0x41500000, 
		0xd298662c, 
		0x00000002, 
		0x00010000, 
		0x00000008, 
		0xbf800000, 
		0x22d6d2d8, 
		0xf56b8265, 
		0xc1c00000, 
		0x42180000, 
		0x0e01ffff, 
		0x6f83c3c0, 
		0x00000100, 
		0xff7ffffe, 
		0xac3137af, 
		0xbaa85873, 
		0x425c0000, 
		0x80000080, 
		0x007a2bb9, 
		0x42700000, 
		0x00000000, 
		0x007fffff, 
		0x00000000, 
		0x5a040c5a, 
		0xf03e7132, 
		0x00800002, 
		0xf3d68eb2, 
		0x00000000, 
		0x80002000, 
		0x913674c1, 
		0x8f7ffffc, 
		0xb479a62a, 
		0x0c7ffc00, 
		0x41f00000, 
		0x5f549b3b, 
		0x7f800000, 
		0x00000100, 
		0xc1e00000, 
		0x2ee5da62, 
		0xc1e80000, 
		0x66ec5d24, 
		0x42700000, 
		0x7e28d048, 
		0x517d0bda, 
		0xd578b56d, 
		0xc2200000, 
		0x7f800000, 
		0x42340000, 
		0x0e7fffff, 
		0xc1300000, 
		0x0e1fffff, 
		0x6727f7b6, 
		0xc1f80000, 
		0x9d4da4b4, 
		0x00800002, 
		0x807ffffe, 
		0x00080000, 
		0x7f800000, 
		0xab27a7a4, 
		0xa837a4c6, 
		0xc1880000, 
		0xc25c0000, 
		0x1a42a111, 
		0x80020000, 
		0x41500000, 
		0x41700000, 
		0xdb8c86c9, 
		0x92dde4c5, 
		0x41d00000, 
		0x0c7e0000, 
		0x7f800000, 
		0xc1600000, 
		0x80100000, 
		0x42240000, 
		0xa8dfe282, 
		0x80800000, 
		0x007f2932, 
		0x0c7fff00, 
		0xff8fadfd, 
		0x0e001fff, 
		0xc1700000, 
		0xffa0e8d7, 
		0x807ffffe, 
		0xc2780000, 
		0xc27c0000, 
		0x80000001, 
		0xffbeb6fc, 
		0x8f7ffffc, 
		0x0e000fff, 
		0x80000008, 
		0x42500000, 
		0x98c71d55, 
		0x00000000, 
		0x0c7fe000, 
		0x7d94d507, 
		0x68f074d5, 
		0x8db7697e, 
		0xc2700000, 
		0x80000008, 
		0x80800002, 
		0x00004000, 
		0x427c0000, 
		0x42700000, 
		0xf75b0e7c, 
		0x00004000, 
		0xbf800001, 
		0x9e6315f4, 
		0x41000000, 
		0x00000020, 
		0xf1a63322, 
		0xc1100000, 
		0x80800001, 
		0x80000000, 
		0x7f7fffff, 
		0x00008000, 
		0x7f800000, 
		0x80000001, 
		0x41880000, 
		0x2857c4f1, 
		0x8f7ffffd, 
		0x80800003, 
		0x80000002, 
		0x16bf9321, 
		0x0c7f8000, 
		0xc2240000, 
		0xc1500000, 
		0xb3f5d6c5, 
		0x00000100, 
		0x33333333, 
		0x00000000, 
		0xc1e80000, 
		0xc1f00000, 
		0x80517b61, 
		0x41900000, 
		0x80000000, 
		0xa984beab, 
		0x424c0000, 
		0x7f800000, 
		0xc1d00000, 
		0x34d94145, 
		0x8f7fffff, 
		0x010462f1, 
		0x00400000, 
		0x00000080, 
		0xff800000, 
		0xc2580000, 
		0x0c7fff80, 
		0xc647c469, 
		0x0e07ffff, 
		0x42580000, 
		0xffc0f389, 
		0xdfad07c8, 
		0xf4ef3b7c, 
		0xf5d36063, 
		0xe03d3eae, 
		0x0e00003f, 
		0x00ffffff, 
		0x41880000, 
		0x94a58f42, 
		0xff96febc, 
		0x41d80000, 
		0x0c7fff80, 
		0x00800002, 
		0x55555555, 
		0x00010000, 
		0x41300000, 
		0x41000000, 
		0xe11a1351, 
		0x80100000, 
		0x0e07ffff, 
		0xc2740000, 
		0xc1400000, 
		0xbf028f5c, 
		0x007fffff, 
		0xa26d1e58, 
		0xea909c62, 
		0xc2000000, 
		0x80000100, 
		0x7f800000, 
		0x7f800000, 
		0x00010000, 
		0x00008000, 
		0x0e000007, 
		0x00080000, 
		0xc1900000, 
		0x188d8ae8, 
		0x42540000, 
		0x00000008, 
		0x7f7ffffe, 
		0xfcfd85ac, 
		0xb79e46fb, 
		0x196f8c8a, 
		0x701188d7, 
		0x5746cf48, 
		0x80000000, 
		0x42080000, 
		0x80000000, 
		0x42140000, 
		0xd59d5926, 
		0xff800000, 
		0x42300000, 
		0x33333333, 
		0x7f7ffffe, 
		0x40c00000, 
		0xc0a00000, 
		0xb5bf5458, 
		0x80000002, 
		0x3f800001, 
		0x1fae2a44, 
		0xc2280000, 
		0x42461767, 
		0x40e00000, 
		0x7fc00001, 
		0xffbfffff, 
		0x18517068, 
		0x0e003fff, 
		0x42380000, 
		0x80000200, 
		0x80080000, 
		0xffc00000, 
		0x4a343993, 
		0x00011111, 
		0x42100000, 
		0x424c0000, 
		0xc2500000, 
		0x4275f278, 
		0xc2300000, 
		0x0c7f0000, 
		0x00000000, 
		0xebe0689e, 
		0x80000001, 
		0x80000000, 
		0x40a00000, 
		0x7f800000, 
		0x7f800000, 
		0x00000000, 
		0x00800001, 
		0xff7fffff, 
		0x7f7fffff, 
		0x00800000, 
		0xff800000, 
		0x00000000, 
		0x42100000, 
		0x7fbfffff, 
		0x0e000007, 
		0xff800000, 
		0x41e80000, 
		0x0d524c85, 
		0xaaaaaaaa, 
		0x80010000, 
		0x9ce61e6a, 
		0xc2240000, 
		0x80000000, 
		0xd0132588, 
		0x41c80000, 
		0x00000010, 
		0x00000800, 
		0x1dffd0c6, 
		0x79a935a6, 
		0x7fc00001, 
		0x00008000, 
		0x99babbf3, 
		0xc26c0000, 
		0xff800000, 
		0x41e80000, 
		0x44307629, 
		0x00000020, 
		0x0c600000, 
		0x0c400000, 
		0xc1c00000, 
		0x3f800000, 
		0x77e536de, 
		0x6d4ca0e6, 
		0xff800000, 
		0xc1d00000, 
		0x41a80000, 
		0x00000000, 
		0x425c0000, 
		0x80800002, 
		0x00000000, 
		0x0e01ffff, 
		0x55555555, 
		0xe16af5e0, 
		0x007a375b, 
		0x7f800001, 
		0xc2740000, 
		0x007fffff, 
		0x2801826f, 
		0x41300000, 
		0x0f7ffffc, 
		0x661a1c7f, 
		0xc2740000, 
		0xaaaaaaaa, 
		0x42340000, 
		0x15b85c14, 
		0xa3e7e287, 
		0xb2de25fb, 
		0x7f800000, 
		0x42780000, 
		0x28a7594a, 
		0xbeb769ed, 
		0x1d55fc4c, 
		0x00800000, 
		0x0e00001f, 
		0x80000000, 
		0x80000000, 
		0x42000000, 
		0x40e00000, 
		0x42780000, 
		0x00800000, 
		0x7fb496a0, 
		0x00000000, 
		0x42480000, 
		0xd258f944, 
		0x00000020, 
		0x2107ac67, 
		0x00800001, 
		0x80000004, 
		0xffb09269, 
		0x80000001, 
		0x9d74449c, 
		0x42240000, 
		0x41e00000, 
		0x5f36ed88, 
		0x00ffffff, 
		0x41d00000, 
		0x1c636ee7, 
		0x427c0000, 
		0x00800001, 
		0xffaa5a9e, 
		0x42180000, 
		0x7f9932af, 
		0x505e632f, 
		0xc2400000, 
		0x1e15025e, 
		0xc1400000, 
		0x80020000, 
		0x80000000, 
		0x7fbc721c, 
		0xc2780000, 
		0xff7ffffe, 
		0x7f8e6231, 
		0x7f800001, 
		0x0e7fffff, 
		0xc7e520fc, 
		0x80000000, 
		0x80200000, 
		0x80000000, 
		0x7fc00000, 
		0x7f7ffffe, 
		0x0c7fff00, 
		0xc2680000, 
		0xff800000, 
		0x55e36500, 
		0xc2040000, 
		0x7f800000, 
		0x00000000, 
		0x41b00000, 
		0xb100ef91, 
		0x7f800000, 
		0x2429923e, 
		0x80800000, 
		0x42340000, 
		0x421c0000, 
		0x42400000, 
		0xc0000000, 
		0x42140000, 
		0x0e0fffff, 
		0x7e150ecc, 
		0x007fffff, 
		0x64186612, 
		0x0c7ff000, 
		0x00800003, 
		0x80000200, 
		0x80200000, 
		0xdcc58890, 
		0xe2d4e0b0, 
		0x41e00000, 
		0x7fc00000, 
		0xf16fd962, 
		0xdbf7546f, 
		0x9b8d28ba, 
		0x41d80000, 
		0x80000000, 
		0x00100000, 
		0x7db9d911, 
		0x80000000, 
		0xc0c00000, 
		0xcace042a, 
		0xc1a80000, 
		0x3e20bff2, 
		0x959a79e7, 
		0x3d590067, 
		0x48b0325b, 
		0xcccccccc, 
		0x7c9c0832, 
		0xc1e80000, 
		0x2ab9642b, 
		0x9e601e88, 
		0x40e00000, 
		0xffffffff, 
		0x00000000, 
		0x00000001, 
		0xc1900000, 
		0xff800000, 
		0x40c00000, 
		0x80800000, 
		0xff7ffffe, 
		0x980cd7f8, 
		0xc2140000, 
		0x42340000, 
		0x7db55340, 
		0x25f1fc6c, 
		0x41b80000, 
		0xc1400000, 
		0x29e595d8, 
		0x58eae806, 
		0x6a03b4a8, 
		0x7fc00001, 
		0x80000000, 
		0xc1800000, 
		0x25ca4414, 
		0x80000001, 
		0xcccccccc, 
		0xffffffff, 
		0xff7ffffe, 
		0xe0b306b5, 
		0xc23c0000, 
		0xc2300000, 
		0x80800002, 
		0xc2240000, 
		0xff7fffff, 
		0xc1980000, 
		0x0e7fffff, 
		0x522cadc2, 
		0xc1a00000, 
		0x42300000, 
		0xc3ecb0e5, 
		0x00000001, 
		0xa82a6c40, 
		0x9a045489, 
		0x80800003, 
		0x978bfd2e, 
		0x0c7ffc00, 
		0x04e8a2e9, 
		0x42100000, 
		0x42780000, 
		0x00000000, 
		0xe49cb82f, 
		0x58fd3d8c, 
		0x6f15bc07, 
		0x03750ef9, 
		0x72368e99, 
		0x80000000, 
		0x0c7ffff0, 
		0x9d95c09c, 
		0x311f9847, 
		0xaa0e448c, 
		0xaaaaaaaa, 
		0x7fab53be, 
		0xb5131415, 
		0x2aaebe22, 
		0x7f800000, 
		0x7f800000, 
		0xa98ff91b, 
		0x42600000, 
		0x62aad91f, 
		0x00000100, 
		0x41a00000, 
		0xa7d9ee60, 
		0x7f800000, 
		0x0c6d8038, 
		0x0e3fffff, 
		0xc20c0000, 
		0xcccccccc, 
		0x8f7fffff, 
		0xc1d00000, 
		0xc20c0000, 
		0x732814c7, 
		0x00000000, 
		0x82e159d4, 
		0xbf800000, 
		0xc2040000, 
		0x6cc048e2, 
		0x42240000, 
		0x3c32860b, 
		0x1c0989c8, 
		0xdf39a6ad, 
		0x0c7ffff8, 
		0x80000000, 
		0x00800002, 
		0x00000000, 
		0x80200000, 
		0x8f7ffffd, 
		0x7f800000, 
		0x0c7fc000, 
		0x80ffffff, 
		0x00020000, 
		0x41500000, 
		0xcb8c4b40, 
		0x30790cad, 
		0xff800000, 
		0x88d4a554, 
		0xd4ff1779, 
		0x6d6f1881, 
		0x00008000, 
		0x00000000, 
		0xc2700000, 
		0x80800000, 
		0x0f7ffffe, 
		0x18ddae99, 
		0x00800001, 
		0xc24c0000, 
		0x80800000, 
		0xc1a00000, 
		0xe6de8578  
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
		0x000001c5, 
		0x000000cb, 
		0x00000019, 
		0x00000255, 
		0x0000008d, 
		0x00000227, 
		0x00000273, 
		0x000002bb, 
		0x00000057, 
		0x000000ff, 
		0x000002fb, 
		0x000000db, 
		0x000001ab, 
		0x000001fd, 
		0x00000127, 
		0x0000037d, 
		0x0000022f, 
		0x0000024d, 
		0x0000002b, 
		0x000000ad, 
		0x0000021d, 
		0x00000047, 
		0x0000007f, 
		0x00000173, 
		0x00000335, 
		0x000003af, 
		0x00000057, 
		0x0000002f, 
		0x000001a3, 
		0x00000085, 
		0x000001bf, 
		0x000003b5, 
		0x000002f3, 
		0x000000b7, 
		0x000000c1, 
		0x00000071, 
		0x000000ad, 
		0x0000024b, 
		0x0000032b, 
		0x0000007d, 
		0x000002a7, 
		0x000000df, 
		0x00000277, 
		0x0000018b, 
		0x000001a1, 
		0x000000ad, 
		0x000002b9, 
		0x000000d7, 
		0x0000035f, 
		0x0000006f, 
		0x000002ab, 
		0x0000038f, 
		0x00000057, 
		0x00000273, 
		0x00000123, 
		0x000001cb, 
		0x0000001b, 
		0x000003d7, 
		0x000000fd, 
		0x00000259, 
		0x00000089, 
		0x0000028f, 
		0x00000375, 
		0x0000031d, 
		0x000000b5, 
		0x00000027, 
		0x000001c5, 
		0x0000026b, 
		0x0000012f, 
		0x0000018d, 
		0x000001bf, 
		0x00000091, 
		0x000001c7, 
		0x00000221, 
		0x00000197, 
		0x0000033f, 
		0x00000325, 
		0x0000007f, 
		0x00000179, 
		0x0000027f, 
		0x00000105, 
		0x00000109, 
		0x0000004b, 
		0x000000bd, 
		0x000001c1, 
		0x00000347, 
		0x000001f7, 
		0x000003a5, 
		0x000001cb, 
		0x000002fd, 
		0x000002bb, 
		0x0000005f, 
		0x0000025f, 
		0x000001c7, 
		0x00000391, 
		0x0000029b, 
		0x000001fd, 
		0x00000001, 
		0x000000df, 
		0x000001c3, 
		0x00000171, 
		0x00000383, 
		0x0000025f, 
		0x00000349, 
		0x00000279, 
		0x000000dd, 
		0x00000049, 
		0x000003e9, 
		0x000001cf, 
		0x000002ef, 
		0x000000e9, 
		0x00000095, 
		0x000000b1, 
		0x000001dd, 
		0x000003a3, 
		0x000001d3, 
		0x00000215, 
		0x00000159, 
		0x00000111, 
		0x000001f5, 
		0x00000091, 
		0x00000325, 
		0x0000034f, 
		0x0000016b, 
		0x00000133, 
		0x000001b1, 
		0x0000012d, 
		0x000001d9, 
		0x000002f3, 
		0x000003d3, 
		0x00000027, 
		0x000003a1, 
		0x00000017, 
		0x0000031b, 
		0x00000245, 
		0x00000389, 
		0x000000f5, 
		0x0000029f, 
		0x00000089, 
		0x0000019d, 
		0x00000273, 
		0x00000043, 
		0x000003f7, 
		0x000000cb, 
		0x0000030d, 
		0x000000e5, 
		0x000002d3, 
		0x00000225, 
		0x000002b5, 
		0x0000038d, 
		0x000001e7, 
		0x00000271, 
		0x00000019, 
		0x000003ef, 
		0x00000139, 
		0x0000026b, 
		0x000002e5, 
		0x0000002f, 
		0x000003f5, 
		0x0000033d, 
		0x00000145, 
		0x000000ff, 
		0x0000025f, 
		0x0000038f, 
		0x0000034f, 
		0x0000020d, 
		0x00000003, 
		0x00000349, 
		0x000001db, 
		0x000000ef, 
		0x00000275, 
		0x0000019b, 
		0x000001ed, 
		0x0000038d, 
		0x000003d3, 
		0x000001dd, 
		0x000002d1, 
		0x000003eb, 
		0x000000c1, 
		0x0000015f, 
		0x0000039d, 
		0x00000027, 
		0x000002ef, 
		0x0000024d, 
		0x00000133, 
		0x000002fb, 
		0x000001c9, 
		0x00000249, 
		0x00000241, 
		0x000002e9, 
		0x00000193, 
		0x00000057, 
		0x00000107, 
		0x000000a3, 
		0x000002c5, 
		0x00000001, 
		0x0000003d, 
		0x00000089, 
		0x00000263, 
		0x00000373, 
		0x00000337, 
		0x000000ad, 
		0x0000012b, 
		0x0000006d, 
		0x00000135, 
		0x000003d5, 
		0x00000083, 
		0x00000179, 
		0x0000001b, 
		0x000000e1, 
		0x000003c3, 
		0x0000031f, 
		0x0000024f, 
		0x00000215, 
		0x0000012d, 
		0x00000049, 
		0x0000026b, 
		0x00000209, 
		0x000000e5, 
		0x00000229, 
		0x000000d3, 
		0x0000005b, 
		0x00000059, 
		0x000002a7, 
		0x000003c9, 
		0x0000004d, 
		0x000003a5, 
		0x00000209, 
		0x000001d9, 
		0x000001b1, 
		0x000002d9, 
		0x00000063, 
		0x000003e9, 
		0x000003f3, 
		0x00000297, 
		0x000000c5, 
		0x000003d5, 
		0x0000033b, 
		0x0000037d, 
		0x00000191, 
		0x00000035, 
		0x000001a5, 
		0x00000227, 
		0x00000033, 
		0x00000083, 
		0x0000024d, 
		0x00000311, 
		0x00000325, 
		0x000000e1, 
		0x000002c5, 
		0x00000017, 
		0x0000015f, 
		0x000003e1, 
		0x000002c5, 
		0x000003eb, 
		0x0000033d  
};

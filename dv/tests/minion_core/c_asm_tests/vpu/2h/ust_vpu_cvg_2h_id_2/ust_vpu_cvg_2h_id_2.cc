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
			"csrwi mhpmevent3, 17\n"   
			"csrwi mhpmevent4, 28\n"   
			"csrwi mhpmevent5, 20\n"   
			"csrwi mhpmevent6, 22\n"   
			"csrwi mhpmevent7, 3\n"   
			"csrwi mhpmevent8, 4\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f27,  672 (x5)\n"
			"flw.ps f1,  96 (x5)\n"
			"flw.ps f3,  288 (x5)\n"
			"flw.ps f10,  256 (x5)\n"
			"flw.ps f5,  768 (x5)\n"
			"flw.ps f29,  416 (x5)\n"
			"flw.ps f31,  736 (x5)\n"
			"flw.ps f8,  0 (x5)\n"
			"flw.ps f7,  320 (x5)\n"
			"flw.ps f14,  128 (x5)\n"
			"flw.ps f24,  832 (x5)\n"
			"flw.ps f18,  192 (x5)\n"
			"flw.ps f21,  32 (x5)\n"
			"flw.ps f28,  576 (x5)\n"
			"flw.ps f19,  800 (x5)\n"
			"flw.ps f6,  160 (x5)\n"
			"flw.ps f17,  608 (x5)\n"
			"flw.ps f26,  512 (x5)\n"
			"flw.ps f12,  896 (x5)\n"
			"flw.ps f30,  864 (x5)\n"
			"flw.ps f15,  704 (x5)\n"
			"flw.ps f4,  960 (x5)\n"
			"flw.ps f22,  544 (x5)\n"
			"flw.ps f11,  352 (x5)\n"
			"flw.ps f16,  384 (x5)\n"
			"flw.ps f23,  640 (x5)\n"
			"flw.ps f9,  448 (x5)\n"
			"flw.ps f20,  992 (x5)\n"
			"flw.ps f0,  224 (x5)\n"
			"flw.ps f13,  64 (x5)\n"
			"flw.ps f25,  928 (x5)\n"
			"flw.ps f2,  480 (x5)\n"
			"csrw fflags, zero\n"
			"csrwi frm, 0x0\n"
			"LBL_KERNEL_HID_0:\n"          
			"li x5, 0xFEEDFEED\n"
			"li x0, 0x12c\n" 
			"slti x0, x0, 0x7f2\n"
			"la x31, check_sum_hid_0\n"
			"li x5, 0x1\n" 
			"sw x5, (x31)\n"
			"li x30, 0\n"
			"addi x30, x30, 1\n"
			"la x22, rand_int_32\n"
			"flw.ps f24, 224(x22)\n"
			"faddi.pi f28, f24, 0x100\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f30, 96(x26)\n"
			"flw.ps f5, 192(x26)\n"
			"fsgnjx.ps f16, f30, f5\n" 
			"la x5, rand_int_32\n"
			"flw.ps f15, 736(x5)\n"
			"fsrli.pi f15, f15, 0x7\n" 
			"li x5, 0x9a8cc6f858fa5657\n"
			"fcvt.s.lu f24, x5, rmm\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f8, 256(x14)\n"
			"fround.ps f3, f8, rdn\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f3, 640(x26)\n"
			"flw.ps f8, 960(x26)\n"
			"fsgnjn.ps f26, f3, f8\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f8, 864(x5)\n"
			"flw.ps f3, 928(x5)\n"
			"flw.ps f15, 416(x5)\n"
			"fmadd.s f28, f8, f3, f15, rdn\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_0:\n"
			"maskor m0, m7, m2\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x15, rand_ieee754_32\n"
			"flw.ps f23, 160(x15)\n"
			"flw.ps f11, 960(x15)\n"
			"fsgnjn.ps f26, f23, f11\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f1, 704(x23)\n"
			"fcvt.wu.s x10, f1, rdn\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_0:\n"
			"flem.ps m7, f23, f16\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x20, rand_int_32\n"
			"flw.ps f30, 896(x20)\n"
			"fsatu8.pi f31, f30\n" 
			"LBL_SEQID_0_M0_WRITE_ID_2_HID_0:\n"
			"fltm.ps m7, f15, f1\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x20, rand_int_32\n"
			"flw.ps f11, 864(x20)\n"
			"fcvt.ps.pw f30, f11, rmm\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f26, 576(x5)\n"
			"flw.ps f30, 384(x5)\n"
			"fsgnj.ps f26, f26, f30\n" 
			"la x10, rand_int_32\n"
			"flw.ps f10, 32(x10)\n"
			"flw.ps f8, 832(x10)\n"
			"fltu.pi f17, f10, f8\n" 
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
			"LBL_HPM_3_CORE_TS_OPS_FAIL_HID_0:\n"          
			"csrr x5, mhpmcounter3\n"
			"sd x5, 32 (x31)\n"
			"LBL_HPM_4_CORE_TQUANT_INST_FAIL_HID_0:\n"          
			"csrr x5, mhpmcounter4\n"
			"sd x5, 40 (x31)\n"
			"LBL_HPM_5_CORE_TXFMA_OPS16_FAIL_HID_0:\n"          
			"csrr x5, mhpmcounter5\n"
			"sd x5, 48 (x31)\n"
			"LBL_HPM_6_CORE_TXFMA_INT_OPS_FAIL_HID_0:\n"          
			"csrr x5, mhpmcounter6\n"
			"sd x5, 56 (x31)\n"
			"LBL_HPM_7_NEIGH_TX_COOP_TLD_REQ_FAIL_HID_0:\n"          
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
			"LBL_HPM_3_CORE_TS_OPS_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter3\n"
			"sd x5, 32 (x31)\n"
			"LBL_HPM_4_CORE_TQUANT_INST_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter4\n"
			"sd x5, 40 (x31)\n"
			"LBL_HPM_5_CORE_TXFMA_OPS16_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter5\n"
			"sd x5, 48 (x31)\n"
			"LBL_HPM_6_CORE_TXFMA_INT_OPS_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter6\n"
			"sd x5, 56 (x31)\n"
			"LBL_HPM_7_NEIGH_TX_COOP_TLD_REQ_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter7\n"
			"sd x5, 64 (x31)\n"
			"LBL_HPM_8_NEIGH_TX_NEIGH_COOP_TLD_REQ_PASS_HID_0:\n"          
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
			"csrwi mhpmevent3, 3\n"   
			"csrwi mhpmevent4, 2\n"   
			"csrwi mhpmevent5, 1\n"   
			"csrwi mhpmevent6, 6\n"   
			"csrwi mhpmevent7, 12\n"   
			"csrwi mhpmevent8, 21\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f4,  288 (x5)\n"
			"flw.ps f28,  768 (x5)\n"
			"flw.ps f2,  416 (x5)\n"
			"flw.ps f16,  384 (x5)\n"
			"flw.ps f26,  832 (x5)\n"
			"flw.ps f0,  224 (x5)\n"
			"flw.ps f27,  128 (x5)\n"
			"flw.ps f17,  960 (x5)\n"
			"flw.ps f30,  320 (x5)\n"
			"flw.ps f18,  192 (x5)\n"
			"flw.ps f31,  928 (x5)\n"
			"flw.ps f8,  992 (x5)\n"
			"flw.ps f21,  0 (x5)\n"
			"flw.ps f5,  160 (x5)\n"
			"flw.ps f24,  544 (x5)\n"
			"flw.ps f11,  672 (x5)\n"
			"flw.ps f22,  864 (x5)\n"
			"flw.ps f29,  352 (x5)\n"
			"flw.ps f3,  800 (x5)\n"
			"flw.ps f25,  448 (x5)\n"
			"flw.ps f7,  32 (x5)\n"
			"flw.ps f9,  480 (x5)\n"
			"flw.ps f12,  736 (x5)\n"
			"flw.ps f10,  640 (x5)\n"
			"flw.ps f15,  96 (x5)\n"
			"flw.ps f19,  64 (x5)\n"
			"flw.ps f20,  704 (x5)\n"
			"flw.ps f6,  608 (x5)\n"
			"flw.ps f23,  576 (x5)\n"
			"flw.ps f14,  512 (x5)\n"
			"flw.ps f1,  256 (x5)\n"
			"flw.ps f13,  896 (x5)\n"
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
			"la x27, rand_ieee754_32\n"
			"flw.ps f14, 352(x27)\n"
			"fclass.ps f31, f14\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f27, 640(x15)\n"
			"flw.ps f20, 224(x15)\n"
			"fsgnjn.s f14, f27, f20\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f12, 608(x10)\n"
			"flw.ps f4, 608(x10)\n"
			"flw.ps f1, 160(x10)\n"
			"fcmov.ps f5, f12, f4, f1\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f8, 448(x27)\n"
			"flw.ps f15, 128(x27)\n"
			"flw.ps f5, 704(x27)\n"
			"fmadd.ps f5, f8, f15, f5, rmm\n" 
			"la x18, rand_int_32\n"
			"flw.ps f2, 416(x18)\n"
			"flw.ps f26, 928(x18)\n"
			"for.pi f12, f2, f26\n" 
			"LBL_SEQID_0_FP_TRANS_ID_0_HID_1:\n"
			"la x9, rand_ieee754_32\n"
			"flw.ps f5, 736(x9)\n"
			"frcp.ps f4, f5\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f20, 896(x23)\n"
			"flw.ps f8, 768(x23)\n"
			"fsub.s f31, f20, f8, rup\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f30, 288(x6)\n"
			"fclass.ps f1, f30\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f2, 96(x9)\n"
			"flw.ps f16, 256(x9)\n"
			"fmul.ps f30, f2, f16, rmm\n" 
			"la x10, rand_int_32\n"
			"flw.ps f31, 128(x10)\n"
			"fslli.pi f1, f31, 0x1\n" 
			"la x23, rand_int_32\n"
			"flw.ps f16, 320(x23)\n"
			"fxor.pi f31, f16, f16\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f5, 800(x23)\n"
			"flw.ps f16, 448(x23)\n"
			"flt.s x9, f5, f16\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f4, 896(x6)\n"
			"fmin.s f28, f4, f4\n" 
			"la x10, rand_int_32\n"
			"flw.ps f21, 416(x10)\n"
			"flw.ps f26, 192(x10)\n"
			"fmaxu.pi f20, f21, f26\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f30, 448(x22)\n"
			"flw.ps f5, 608(x22)\n"
			"fmin.s f14, f30, f5\n" 
			"maskpopc x9, m3\n" 
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
			"LBL_HPM_3_CORE_RETIRED_INST1_FAIL_HID_1:\n"          
			"csrr x5, mhpmcounter3\n"
			"sd x5, 32 (x31)\n"
			"LBL_HPM_4_CORE_RETIRED_INST0_FAIL_HID_1:\n"          
			"csrr x5, mhpmcounter4\n"
			"sd x5, 40 (x31)\n"
			"LBL_HPM_5_CORE_MCYCLES_FAIL_HID_1:\n"          
			"csrr x5, mhpmcounter5\n"
			"sd x5, 48 (x31)\n"
			"LBL_HPM_6_CORE_DCACHE_ACCESS0_FAIL_HID_1:\n"          
			"csrr x5, mhpmcounter6\n"
			"sd x5, 56 (x31)\n"
			"LBL_HPM_7_NEIGH_INTER_CORE_MSG_TX_FAIL_HID_1:\n"          
			"csrr x5, mhpmcounter7\n"
			"sd x5, 64 (x31)\n"
			"LBL_HPM_8_NEIGH_ETL_REQ_TO_INTERMEDIATE_FIFO_FAIL_HID_1:\n"          
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
			"LBL_HPM_3_CORE_RETIRED_INST1_PASS_HID_1:\n"          
			"csrr x5, mhpmcounter3\n"
			"sd x5, 32 (x31)\n"
			"LBL_HPM_4_CORE_RETIRED_INST0_PASS_HID_1:\n"          
			"csrr x5, mhpmcounter4\n"
			"sd x5, 40 (x31)\n"
			"LBL_HPM_5_CORE_MCYCLES_PASS_HID_1:\n"          
			"csrr x5, mhpmcounter5\n"
			"sd x5, 48 (x31)\n"
			"LBL_HPM_6_CORE_DCACHE_ACCESS0_PASS_HID_1:\n"          
			"csrr x5, mhpmcounter6\n"
			"sd x5, 56 (x31)\n"
			"LBL_HPM_7_NEIGH_INTER_CORE_MSG_TX_PASS_HID_1:\n"          
			"csrr x5, mhpmcounter7\n"
			"sd x5, 64 (x31)\n"
			"LBL_HPM_8_NEIGH_ETL_REQ_TO_INTERMEDIATE_FIFO_PASS_HID_1:\n"          
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
		0x39be5fca, 
		0x3b943ce8, 
		0x668fd5a8, 
		0xa2792f86, 
		0x78027437, 
		0xe350b534, 
		0x44c147f7, 
		0x9bd59dee, 
		0x0a42d9b4, 
		0xa6514875, 
		0x7947d7dc, 
		0x95d5ba29, 
		0xcac9076d, 
		0x6f966117, 
		0xb00b5bcc, 
		0x9d27401f, 
		0xe319bfbd, 
		0x770f05d3, 
		0x66bdd2e4, 
		0x4c3ef054, 
		0xfba2ea5d, 
		0x277d716c, 
		0x4afce6ff, 
		0x69ab0179, 
		0x24d3549d, 
		0x73f1f3fa, 
		0xa06ff0b2, 
		0x2a00c671, 
		0xc9ce4b04, 
		0xcc0a81ea, 
		0x40b81a82, 
		0xceb44808, 
		0x6b720635, 
		0x7eed1d8e, 
		0xf12ffa32, 
		0x52f74821, 
		0xc17e8653, 
		0x8e6480ba, 
		0x9a5192b9, 
		0xf0ad07f0, 
		0xb4b62e77, 
		0xc9796636, 
		0x8b0c9fd6, 
		0x7570b9dc, 
		0xf6e37bd3, 
		0x32fba404, 
		0xf8b8a057, 
		0x6cf5c259, 
		0x2e3320f2, 
		0x9e25dd09, 
		0x8b13dd80, 
		0x608b1f8a, 
		0x50cb7d21, 
		0x1eace79b, 
		0x87fdbc33, 
		0xb8705fbe, 
		0x2d971e1c, 
		0x8d3f608d, 
		0xc24de941, 
		0xa033da84, 
		0x43eaad09, 
		0x4d0efec0, 
		0x67e748d6, 
		0xcba6b179, 
		0x02f5aa0b, 
		0xfa9773c5, 
		0xa29bd30f, 
		0x30b04d50, 
		0x09176eb8, 
		0x8f83e835, 
		0x221e01bc, 
		0xc046d1cc, 
		0x03382cad, 
		0x3d1b6057, 
		0xc98a6d05, 
		0x68b09812, 
		0x604db6a3, 
		0xe5a42a99, 
		0xc4c59f77, 
		0x63c2218f, 
		0x9f5b563d, 
		0xfb8b4941, 
		0xa579277c, 
		0xac15a056, 
		0x62f9bf2c, 
		0xcea72f41, 
		0x7c98b30c, 
		0xb5abd2e9, 
		0xe49a7c6c, 
		0x7e6c8008, 
		0x967a3894, 
		0x397af3c3, 
		0x22bcb2c8, 
		0x05b37e08, 
		0x0ef051e0, 
		0xc20ae868, 
		0xb981e800, 
		0xcdad3802, 
		0x9c834108, 
		0xa39cca1a, 
		0x4816fe4b, 
		0x65e44d3e, 
		0x3683fd2f, 
		0x0a33a31e, 
		0xefd278e1, 
		0x1c6c89d4, 
		0x7f61f8a8, 
		0xc4639c1a, 
		0x07bdbf43, 
		0x6695a04e, 
		0xbf198eb7, 
		0x72302cc0, 
		0xc626c961, 
		0x606a550b, 
		0x226a7745, 
		0x4ad97e8c, 
		0x2fb71d8d, 
		0x13470bd3, 
		0xdd0be79f, 
		0x25eb42f7, 
		0x3421017a, 
		0x18c42431, 
		0x8f540895, 
		0x509d9898, 
		0x94c37300, 
		0xce7fd0cf, 
		0x67691b4d, 
		0x3c010f8c, 
		0x25c7b4d1, 
		0x779aa53f, 
		0x86dcb02b, 
		0x5b6bbce4, 
		0x911cc21a, 
		0x54175225, 
		0xd38d5fcc, 
		0x895eb977, 
		0x2a5f1643, 
		0x9a31cf53, 
		0xf3b7f21d, 
		0xd8c93757, 
		0x1acc7023, 
		0x080621bb, 
		0x2610361d, 
		0x1b61000c, 
		0x6e680fb6, 
		0xe59f8b64, 
		0xe6e71aa5, 
		0xb3ef24e6, 
		0x94e2c528, 
		0x84c21f26, 
		0x742f7827, 
		0xa30858a5, 
		0xfc114edc, 
		0x3284b924, 
		0xdb34d060, 
		0xdfd68410, 
		0x845db691, 
		0xe52bc32a, 
		0x881ff1c4, 
		0x2bb59fc4, 
		0x972c0177, 
		0xdf0dbdd2, 
		0xc7616ee7, 
		0xfadd7687, 
		0xfe601cad, 
		0x850ea4d0, 
		0x91a307f9, 
		0xf6c2366e, 
		0x8ed16df3, 
		0x23e53a47, 
		0xfc03ec90, 
		0xac085aed, 
		0x21b3bd89, 
		0xd6af4c86, 
		0xc414e771, 
		0x260d5dd0, 
		0xfd51a5ee, 
		0xe8e1809f, 
		0xb0c247d3, 
		0xa3a5c714, 
		0x4ac08042, 
		0xd09944d5, 
		0x4033b309, 
		0x75346b5c, 
		0x0cd0c362, 
		0x38ec2730, 
		0x940e5360, 
		0x5662cd88, 
		0xcd788cd3, 
		0xe44b5a31, 
		0xad0f90e9, 
		0x9e42e5b8, 
		0x5976a1ef, 
		0xc294b279, 
		0xf389c448, 
		0x834b568f, 
		0x0388ebb6, 
		0xa79e05be, 
		0xbce02da9, 
		0xdb6173fe, 
		0xc2dca91f, 
		0x44021efc, 
		0xfd4b04f8, 
		0xda9fbd54, 
		0xe6da201e, 
		0x8bd25302, 
		0xa30b1d70, 
		0x9b02169b, 
		0x2d5cc0c2, 
		0xf6a1c2b0, 
		0x2dc586a3, 
		0x2123f757, 
		0x010b2b2e, 
		0x3dfe30e9, 
		0x1139edc7, 
		0x71f84dff, 
		0xfc34fd3a, 
		0x91908682, 
		0xa896bf78, 
		0x84937d92, 
		0x62df6276, 
		0x7b865675, 
		0x1f4649cb, 
		0xcc81f0d6, 
		0x18b52e9f, 
		0x8b3c7270, 
		0x66a4029c, 
		0xca486ec2, 
		0x6d243fe9, 
		0xabe92b6f, 
		0x9a23f298, 
		0xa4c36b83, 
		0x0f2773e9, 
		0x0a47dc5d, 
		0x98a6cdd1, 
		0x1f36c3b9, 
		0x7de6841a, 
		0xf1711982, 
		0xd62438be, 
		0x334d1711, 
		0x515b1cdf, 
		0x70be1688, 
		0x475ed2b5, 
		0x1b7cd3be, 
		0xaa9f017f, 
		0x455b9c5f, 
		0x91bca006, 
		0x1ac0187d, 
		0x1fbdcdc3, 
		0xad816696, 
		0x0320db50, 
		0x71b66a1c, 
		0x995fe9b2, 
		0x2766ae73, 
		0x9cfc1c7d, 
		0xc0fdafa5  
};
volatile uint32_t rand_ieee754_32[1536] __attribute__ ((aligned (2048))) = {
		0xc2400000, 
		0x99774d6e, 
		0x80010000, 
		0x59f5a5ee, 
		0x7fa6b97c, 
		0x00000004, 
		0x39262748, 
		0x423c0000, 
		0xf0fd7a94, 
		0x00800001, 
		0xe2e6080d, 
		0x0f7ffffd, 
		0xc1c00000, 
		0xe0f0181d, 
		0x80800003, 
		0xc1a80000, 
		0xc2080000, 
		0x0fd2fb23, 
		0x1caee0c7, 
		0x42200000, 
		0xc2000000, 
		0xd590c35f, 
		0x89f59fcb, 
		0x00040000, 
		0x00800000, 
		0x41f00000, 
		0x454d5be4, 
		0x50b6e20c, 
		0x0c7ffff8, 
		0xc1500000, 
		0x7fffffff, 
		0xc1b80000, 
		0xc2440000, 
		0x42680000, 
		0x488e0780, 
		0x80000000, 
		0x0e000003, 
		0x00000100, 
		0x69654457, 
		0x7f800000, 
		0xc1e00000, 
		0x8ecf44a1, 
		0x427c0000, 
		0x80800001, 
		0x3fb437f7, 
		0x25db910f, 
		0x09926cc6, 
		0x0c7ffff8, 
		0x0f7ffffe, 
		0x00800003, 
		0xc1f00000, 
		0x41a00000, 
		0x00200000, 
		0x144a4992, 
		0xffc00000, 
		0x42200000, 
		0x9e46a794, 
		0xc1e00000, 
		0x42080000, 
		0x0f7ffffd, 
		0x80000010, 
		0x41200000, 
		0x9a5ad77b, 
		0x54f2c605, 
		0x42946d1d, 
		0x42300000, 
		0xe17687c7, 
		0x0c600000, 
		0xc2580000, 
		0xa20e500e, 
		0xff800000, 
		0xc0400000, 
		0x0e01ffff, 
		0x00800002, 
		0xc2180000, 
		0x7f800000, 
		0x571d9f61, 
		0x00000002, 
		0xca1d378f, 
		0x80000000, 
		0xc1d80000, 
		0x42400000, 
		0x00ffffff, 
		0x85bcd7ac, 
		0xe504f376, 
		0x00040000, 
		0x67a8c36a, 
		0x42c1b0b5, 
		0xc2780000, 
		0xc2540000, 
		0x80000000, 
		0x0e07ffff, 
		0x6c8fae00, 
		0x0c7ffff8, 
		0x7f7ffffe, 
		0x80000000, 
		0x41900000, 
		0xc23c0000, 
		0x80000002, 
		0x99cc066c, 
		0x80011111, 
		0x00800000, 
		0xe04ff2b3, 
		0x97fd0316, 
		0x7284d0ca, 
		0xf15cebf8, 
		0x80800003, 
		0x00400000, 
		0x41900000, 
		0x6c314b46, 
		0xc3823a31, 
		0x871e192a, 
		0x006680fd, 
		0x80002000, 
		0x304788f5, 
		0xc0400000, 
		0xc2740000, 
		0xff7ffffe, 
		0xd6262fe3, 
		0xc2000000, 
		0x80b976e0, 
		0xc2340000, 
		0xc1e00000, 
		0xc70c6694, 
		0x7f7ffffe, 
		0x42780000, 
		0x424c0000, 
		0x2c02752f, 
		0x115b3ad3, 
		0xb0b1a5cc, 
		0x00400000, 
		0x0c7fff80, 
		0x00000040, 
		0x0e03ffff, 
		0x00000000, 
		0xff800000, 
		0xc1b80000, 
		0xc0e00000, 
		0x00000000, 
		0x0c7f0000, 
		0x16190cbb, 
		0x47b99cd6, 
		0x613c90e3, 
		0x42340000, 
		0x0e000fff, 
		0x41d00000, 
		0x40400000, 
		0x2817997f, 
		0x36d5a998, 
		0x9cd924f3, 
		0xc1a80000, 
		0x7fc00001, 
		0xff7ffffe, 
		0x50498a9d, 
		0xc1980000, 
		0xd3898efe, 
		0xff800000, 
		0x0e0001ff, 
		0xc1600000, 
		0x6fa77dc5, 
		0x00800001, 
		0x7fbfffff, 
		0x80000000, 
		0x8de3043e, 
		0x6d63e54f, 
		0x2008b402, 
		0x00080000, 
		0x00000000, 
		0x0c7fc000, 
		0x6558ed01, 
		0x80000008, 
		0xad80250d, 
		0x00008000, 
		0x80200000, 
		0x0c7ff800, 
		0x00000000, 
		0x7f7ffffe, 
		0xc2340000, 
		0xba3b8ccd, 
		0x80008000, 
		0x88a6cc39, 
		0x41600000, 
		0xc2740000, 
		0x0c7e0000, 
		0x7f7ffffe, 
		0xc2380000, 
		0x3f800000, 
		0x00010000, 
		0x0e0001ff, 
		0x42640000, 
		0xff80b1f5, 
		0x00800003, 
		0xff800000, 
		0x80011111, 
		0xbf800000, 
		0x3a667169, 
		0xd44239c3, 
		0x7fc00001, 
		0xc2380000, 
		0x09e8e22b, 
		0x0e003fff, 
		0x424c0000, 
		0x80020000, 
		0x0f7ffffc, 
		0x00800001, 
		0xc1980000, 
		0x007c3d64, 
		0x41f00000, 
		0x0c7ffe00, 
		0x6da235b3, 
		0x00400000, 
		0xfd70d259, 
		0x0c7fffff, 
		0x00004000, 
		0xc2280000, 
		0xff800000, 
		0x3f800001, 
		0x0c7ff800, 
		0x42640000, 
		0x00000000, 
		0xcb000000, 
		0x00000800, 
		0x80000000, 
		0xaaaaaaaa, 
		0x86b1f1e9, 
		0x00000000, 
		0xb18e822f, 
		0x82d1d18d, 
		0x42240000, 
		0xf8a248f5, 
		0x33333333, 
		0x0c400000, 
		0xff800000, 
		0x0e001fff, 
		0xb0393c9e, 
		0x0e0000ff, 
		0x80000008, 
		0x41800000, 
		0x1b6b70cc, 
		0x807fffff, 
		0x80000002, 
		0x3f028f5c, 
		0xffb3188f, 
		0x42380000, 
		0x00800000, 
		0x0e03ffff, 
		0x0c7ff800, 
		0x00000001, 
		0x00000000, 
		0x0c7ffffe, 
		0x41f00000, 
		0x0e07ffff, 
		0xc1980000, 
		0xdb72b4c9, 
		0x1a396624, 
		0x00800003, 
		0x0c600000, 
		0x0f7ffffd, 
		0x80000000, 
		0x0c600000, 
		0x7f800000, 
		0xc2480000, 
		0xe41f41d9, 
		0x0c7e0000, 
		0xc2340000, 
		0xffffffff, 
		0x80000004, 
		0x41000000, 
		0x80200000, 
		0x47eb3c0e, 
		0x3f028f5c, 
		0x80004000, 
		0x40800000, 
		0x41a80000, 
		0x799b252c, 
		0x00800001, 
		0x41c00000, 
		0xc1000000, 
		0x9e0399a8, 
		0x7fffffff, 
		0x0e000003, 
		0xb548bbc0, 
		0xff800000, 
		0x00000000, 
		0xf39d1b8b, 
		0xa3bfeb74, 
		0x7fe006df, 
		0x00000001, 
		0x00000001, 
		0x0e1fffff, 
		0x0c7ff000, 
		0x00000040, 
		0x9fb2832b, 
		0x80100000, 
		0x003ddfb8, 
		0x3eccfb30, 
		0x80000000, 
		0x40e00000, 
		0x41400000, 
		0x42280000, 
		0x00400000, 
		0x41900000, 
		0xe965de9b, 
		0xbbf37f54, 
		0x3f800001, 
		0x6d621428, 
		0xff98fddb, 
		0xb06bcb58, 
		0x0e000fff, 
		0x3f800001, 
		0x42440000, 
		0x67bdfe1c, 
		0xf77cba9c, 
		0x11619396, 
		0x293628d1, 
		0x00000001, 
		0x21acdcdd, 
		0x00800000, 
		0x80000008, 
		0x7fc00001, 
		0x80100000, 
		0xc0a00000, 
		0x80432c84, 
		0xc0a00000, 
		0x7f800000, 
		0x5c46b7f8, 
		0xc1880000, 
		0xc24c0000, 
		0xff861665, 
		0x00800002, 
		0x5ebc9a2b, 
		0xff800000, 
		0x00020000, 
		0x7f7ffffe, 
		0x963f7f8b, 
		0x41200000, 
		0x80000100, 
		0x00000002, 
		0xc60709af, 
		0xc2580000, 
		0x3918b4e5, 
		0x2348beb3, 
		0xe25fcd10, 
		0x00000000, 
		0x00800003, 
		0x80000000, 
		0x80000800, 
		0x0e000007, 
		0x41f00000, 
		0xff800000, 
		0xc22c0000, 
		0xff800000, 
		0x7fa28a2b, 
		0x70ade2a3, 
		0x804e50f8, 
		0x7f800000, 
		0x421c0000, 
		0x80000002, 
		0xc2080000, 
		0xff800001, 
		0xc1d80000, 
		0x80000000, 
		0x007fffff, 
		0x532eecc7, 
		0xe7af52e6, 
		0xc2600000, 
		0x00000000, 
		0x41f00000, 
		0x41d00000, 
		0xffc00000, 
		0xff7fffff, 
		0x41800000, 
		0xc2280000, 
		0xad31297f, 
		0xff7ffffe, 
		0xd3e37689, 
		0x0d000ff0, 
		0x76a99c0a, 
		0x00000002, 
		0x65e0593b, 
		0xc2300000, 
		0x00008000, 
		0xffc00000, 
		0x00800002, 
		0xc1b00000, 
		0xc6c7ec9d, 
		0xc2740000, 
		0x80000002, 
		0x41000000, 
		0xc2240000, 
		0xaf0a04da, 
		0x80800003, 
		0x7d920798, 
		0x00000002, 
		0x0c700000, 
		0xee36fe36, 
		0x7f7ffffe, 
		0x802ccc71, 
		0x0c7fc000, 
		0x40e00000, 
		0x80000000, 
		0x42600000, 
		0x80000000, 
		0x42400000, 
		0x42240000, 
		0x0e629b8d, 
		0x40e00000, 
		0x80800000, 
		0x0e7fffff, 
		0x00ffffff, 
		0x80029af5, 
		0x7a7a6d6d, 
		0xc1a80000, 
		0xff800000, 
		0x80800003, 
		0x0d45a280, 
		0x00000004, 
		0x00000000, 
		0xc2580000, 
		0x80000000, 
		0x00000000, 
		0x80800001, 
		0x0e000001, 
		0x00010000, 
		0x80800001, 
		0x80200000, 
		0x0e00000f, 
		0x0c7ffff0, 
		0x420c0000, 
		0x7ff4a498, 
		0xc20b7d59, 
		0x45db7df8, 
		0xc1100000, 
		0x0c700000, 
		0x80800002, 
		0xc36dcecb, 
		0x8f7fffff, 
		0xa497bd7e, 
		0x42580000, 
		0x426c0000, 
		0x557a8fb2, 
		0xfcb74f72, 
		0xff7fffff, 
		0x00800001, 
		0x4bf427c2, 
		0x80000200, 
		0xe5652048, 
		0x41980000, 
		0x80000001, 
		0x0d00fff0, 
		0x42400000, 
		0x80100000, 
		0x80800003, 
		0x80ffffff, 
		0x0c7fff80, 
		0x80000400, 
		0x42340000, 
		0x42580000, 
		0x7f800000, 
		0x80800000, 
		0xf18042e5, 
		0x80800000, 
		0xf58b0c0b, 
		0x8de2a3fc, 
		0x469fe7de, 
		0x0e01ffff, 
		0x0c600000, 
		0xc2440000, 
		0x80000000, 
		0xc1f80000, 
		0x89dc24ed, 
		0x9652b4c0, 
		0xc20c0000, 
		0x42640000, 
		0xeb8e9045, 
		0x8035a1a2, 
		0x72ebb8b4, 
		0x0c7ffffc, 
		0x0e000001, 
		0x00000008, 
		0x0e0000ff, 
		0xc1e80000, 
		0x42480000, 
		0xa9786458, 
		0x0c7ffffc, 
		0x00000000, 
		0x00000020, 
		0x748fbc9f, 
		0xa575983e, 
		0x016d0472, 
		0x7f800000, 
		0x35c5b074, 
		0x7fa08ea4, 
		0xcc1d93ed, 
		0xfb01b66e, 
		0x80000000, 
		0x80002000, 
		0x7949bfad, 
		0x0e00000f, 
		0xab68289a, 
		0xe33e6f4b, 
		0x42040000, 
		0x42080000, 
		0xf5341edf, 
		0x33333333, 
		0x489a8902, 
		0x42240000, 
		0x427c0000, 
		0x00000800, 
		0xc1f80000, 
		0xff7ffffe, 
		0x00100000, 
		0x077364ee, 
		0x00800002, 
		0x0e3fffff, 
		0x5b4e5165, 
		0x56f969c9, 
		0x80000001, 
		0x0e07ffff, 
		0x80001000, 
		0x80020000, 
		0x0c600000, 
		0x0e001fff, 
		0x7f800000, 
		0x6e1ea72d, 
		0x1b9d5f86, 
		0x00000000, 
		0x0c7ffc00, 
		0x42600000, 
		0x00800001, 
		0x0c7ffffc, 
		0x80000000, 
		0x41b00000, 
		0xc1400000, 
		0xc1e80000, 
		0x00000080, 
		0x5bb2229c, 
		0xb54cbd0c, 
		0x007fffff, 
		0x00000000, 
		0xff800000, 
		0x00000000, 
		0x80008000, 
		0x80000000, 
		0x80000000, 
		0x00000002, 
		0x00011111, 
		0xc1b80000, 
		0x00000000, 
		0xce9d7646, 
		0x40400000, 
		0x0c7f0000, 
		0x9bfc7d4c, 
		0x80800001, 
		0x40e00000, 
		0x0c7fff80, 
		0x80000000, 
		0x68a09f9b, 
		0x00000400, 
		0x7f800000, 
		0x3239d5e4, 
		0x7bd352cd, 
		0xc0c00000, 
		0x24e1378e, 
		0x0c700000, 
		0xd33a7503, 
		0xbf028f5c, 
		0xc2440000, 
		0x0e01ffff, 
		0x357849e8, 
		0xff800000, 
		0xb6c78e1b, 
		0x7f9725e3, 
		0x80000001, 
		0xe256fa9a, 
		0xc743fbda, 
		0x420c0000, 
		0x1a86ff0b, 
		0xc25c0000, 
		0x0e0007ff, 
		0x0c7ffff8, 
		0xc2640000, 
		0xff000000, 
		0x42380000, 
		0x0c7ffff8, 
		0xc1200000, 
		0x42180000, 
		0x4b3be1a3, 
		0x41200000, 
		0x7a9b66b5, 
		0x00000000, 
		0x0f7ffffc, 
		0x41800000, 
		0xf5a40c09, 
		0x40400000, 
		0x00000008, 
		0x8f7ffffe, 
		0x7ffcd7ca, 
		0x00733d31, 
		0x80000000, 
		0x00400000, 
		0x2a0da657, 
		0xffc00001, 
		0x0c7fc000, 
		0xffc00001, 
		0xcccccccc, 
		0xff800000, 
		0xff9d2bde, 
		0xb8613e46, 
		0x0c7ffffe, 
		0x00000000, 
		0x7f7ffffe, 
		0x80000000, 
		0x8f7ffffd, 
		0x0f7ffffe, 
		0x00000080, 
		0xbf1d27c6, 
		0x43061026, 
		0x00000000, 
		0x00008000, 
		0x0e0fffff, 
		0x00000000, 
		0x80000000, 
		0xb755f580, 
		0xc0e00000, 
		0x388b447f, 
		0x427c0000, 
		0x7f800000, 
		0xc1c00000, 
		0x41100000, 
		0x42200000, 
		0x00000002, 
		0x80800001, 
		0xc1000000, 
		0x0057fd3d, 
		0x0555c54a, 
		0x7f7ffffe, 
		0xc1f80000, 
		0x00080000, 
		0x0e1fffff, 
		0xff800000, 
		0x00000200, 
		0x8e9698a7, 
		0xff800000, 
		0xc1b80000, 
		0xe16a83ce, 
		0x00800000, 
		0xfc3704f4, 
		0x247061bc, 
		0x170bd310, 
		0x0c7fffff, 
		0x2d8a8170, 
		0x00001000, 
		0xff7ffffe, 
		0x5f6eff14, 
		0x41800000, 
		0x434bcaba, 
		0xff800001, 
		0x45c3fce7, 
		0x23c46755, 
		0x00000000, 
		0x1f0bc919, 
		0x00002000, 
		0x380370d7, 
		0xa13abc24, 
		0x00000001, 
		0x00400000, 
		0x0e00000f, 
		0x0f7ffffe, 
		0xc2600000, 
		0xc2180000, 
		0x0c7ffffc, 
		0x33333333, 
		0x10a303e7, 
		0x80000000, 
		0x0c7c0000, 
		0x8f21d440, 
		0xbf028f5c, 
		0x0e003fff, 
		0xff800000, 
		0xd54a3f74, 
		0x0e7fffff, 
		0x17780b3e, 
		0x42540000, 
		0x9c6e5658, 
		0xc2280000, 
		0x7f7ffffe, 
		0x0c7fffe0, 
		0x728a1f0f, 
		0x00000000, 
		0xff800000, 
		0xadf48f31, 
		0xcd295168, 
		0x007fffff, 
		0x80800001, 
		0xc1f80000, 
		0x42680000, 
		0x80000000, 
		0x7f7fffff, 
		0x0e00ffff, 
		0xff7ffffe, 
		0x59653417, 
		0x80000000, 
		0xba325ab4, 
		0x80000000, 
		0x80000010, 
		0xe4ff741e, 
		0x8d790537, 
		0x80000010, 
		0x00002000, 
		0x00000000, 
		0x41200000, 
		0xffbd2ea5, 
		0x8056e300, 
		0x42440000, 
		0x7f800000, 
		0x40800000, 
		0x9bd97b67, 
		0x42200000, 
		0x3f800000, 
		0x80000000, 
		0x7f7ffffe, 
		0xc27c0000, 
		0x0e00ffff, 
		0x94df29f7, 
		0x423c0000, 
		0xc22c0000, 
		0xc22c0000, 
		0x00000001, 
		0x7f800000, 
		0x0c7e0000, 
		0x42440000, 
		0x0e0fffff, 
		0x80000000, 
		0xffc00001, 
		0x80000800, 
		0x88b64448, 
		0x42380000, 
		0x227f1a2c, 
		0x33333333, 
		0x0c7fffff, 
		0xac2781cc, 
		0x0c400000, 
		0x7fc00001, 
		0x42200000, 
		0x55555555, 
		0xff800000, 
		0x00080000, 
		0xd13fe2aa, 
		0x0c7ffffe, 
		0x00400000, 
		0x00800000, 
		0x0e7fffff, 
		0xffa7de2c, 
		0xef7a2dd9, 
		0x7f7ffffe, 
		0x4deadedc, 
		0x00800003, 
		0x00800003, 
		0x00020000, 
		0xa08b10c3, 
		0x42480000, 
		0xb2ab063d, 
		0x7f8fa84d, 
		0xc25c0000, 
		0x20988488, 
		0x00000040, 
		0xc2500000, 
		0x0c7fffe0, 
		0x09a18957, 
		0x69b36e72, 
		0x0c7ffe00, 
		0x7f800000, 
		0x40890e64, 
		0x00800003, 
		0xc21c0000, 
		0x80000000, 
		0x421c0000, 
		0x80800003, 
		0x253060e8, 
		0xc21c0000, 
		0xff800000, 
		0x7fffffff, 
		0x0e0fffff, 
		0xb3b17801, 
		0x42440000, 
		0x0e07ffff, 
		0x000bd6b0, 
		0x80800002, 
		0xb0867fca, 
		0x42380000, 
		0x40800000, 
		0xff800000, 
		0x7f800000, 
		0x7a9ce19d, 
		0x1f2b328e, 
		0x645080c0, 
		0xc1100000, 
		0x00000001, 
		0x00011111, 
		0x80000000, 
		0xeb7deae6, 
		0x41b00000, 
		0x00000000, 
		0x8053c5c6, 
		0xc0800000, 
		0x7fd363d3, 
		0x80000000, 
		0x00000000, 
		0x80ffffff, 
		0xc2640000, 
		0x80800001, 
		0x0f7ffffe, 
		0xc1b80000, 
		0x3708ef39, 
		0x0e0001ff, 
		0x60caaf7f, 
		0xabad337e, 
		0x4f6b362c, 
		0x0c700000, 
		0x80800000, 
		0x80200000, 
		0xc1800000, 
		0x30585513, 
		0x902b409b, 
		0x2f7a33b6, 
		0x42340000, 
		0x0c7ffffe, 
		0x0ba8587e, 
		0x80080000, 
		0xffaef46f, 
		0xff7ffffe, 
		0x41e00000, 
		0x80010000, 
		0x00010000, 
		0x00000000, 
		0x42580000, 
		0x8e6c5236, 
		0xff7fffff, 
		0x40400000, 
		0xd0a1d541, 
		0x00000004, 
		0x41f80000, 
		0x00000000, 
		0xc2300000, 
		0x80000000, 
		0x900979f7, 
		0x1f5aab79, 
		0x4b8c4b40, 
		0xea3d731f, 
		0x002399be, 
		0x0d5b0d1b, 
		0x00509b5a, 
		0x0c7ffff8, 
		0x0e00001f, 
		0x00000001, 
		0x06f43e78, 
		0xff000000, 
		0x00000020, 
		0x79ab3194, 
		0x6cd0c8c8, 
		0xc9bad133, 
		0xff912490, 
		0xc1700000, 
		0xc2080000, 
		0x6678c64f, 
		0x0c7fff00, 
		0x0c7ff800, 
		0xff800000, 
		0x849439de, 
		0x4b8c4b40, 
		0x0e07ffff, 
		0x4b22ef8a, 
		0x80000000, 
		0xaefaac6d, 
		0xa0e7fe05, 
		0x00800003, 
		0x0e000001, 
		0xc1500000, 
		0x80000000, 
		0x40a00000, 
		0x0e3fffff, 
		0x596ff9dd, 
		0x80800000, 
		0xc1c00000, 
		0xc1880000, 
		0xd13b0657, 
		0x00000100, 
		0x42780000, 
		0xfefb2e76, 
		0xc2500000, 
		0x80000000, 
		0xc2180000, 
		0xff7ffffe, 
		0x426747a5, 
		0x00000002, 
		0x76c5539a, 
		0x86268fd2, 
		0x00100000, 
		0x4b000000, 
		0xefb2fcd0, 
		0x33333333, 
		0x0bc63c47, 
		0x7fc00000, 
		0x40a00000, 
		0xc0e00000, 
		0x80000001, 
		0xff800000, 
		0x080ac2f2, 
		0xb364ca8a, 
		0xb23ac7c9, 
		0x80800000, 
		0x422c0000, 
		0x5e221b8b, 
		0x0c7ffc00, 
		0x0c7f8000, 
		0x80020000, 
		0x0f2a7e14, 
		0x8ba16e50, 
		0xc0c00000, 
		0x7fbfffff, 
		0x2416740b, 
		0x80000800, 
		0x9cebce5d, 
		0x31eca591, 
		0x1b4f6eac, 
		0x0c7fffc0, 
		0xc20c0000, 
		0x7e8a71d5, 
		0x166f5b8e, 
		0x41300000, 
		0xd4107c2a, 
		0x7f800000, 
		0x00000080, 
		0x80000001, 
		0xc55ae383, 
		0x41700000, 
		0x80000020, 
		0x0e07ffff, 
		0xc45c6363, 
		0x80ffffff, 
		0x00000200, 
		0x676fe03c, 
		0xc2400000, 
		0x920dffc7, 
		0x80800001, 
		0x427c0000, 
		0xc2200000, 
		0xc2300000, 
		0x5b8a4082, 
		0x41300000, 
		0x80000001, 
		0xc2680000, 
		0x85b19856, 
		0x4b8c4b40, 
		0x80200000, 
		0xd2cba03c, 
		0x531d9bb9, 
		0xd7748db6, 
		0x00008000, 
		0x80800000, 
		0xff800000, 
		0x00080000, 
		0x30c5c084, 
		0x36048cdb, 
		0xc1500000, 
		0x80000000, 
		0x686163e0, 
		0x97b8a18d, 
		0xff800000, 
		0xc2680000, 
		0x00000000, 
		0xc707d7a0, 
		0xd2216344, 
		0x36150c0d, 
		0x22d5f8e0, 
		0x625da80e, 
		0x0c7c0000, 
		0x86fbf216, 
		0xc1100000, 
		0xb3bfdc50, 
		0xc23c0000, 
		0x0e03ffff, 
		0x80000001, 
		0x00000000, 
		0x00000000, 
		0x00000000, 
		0x80ffffff, 
		0xff800000, 
		0x80000080, 
		0x7fc00001, 
		0x41100000, 
		0x7f800001, 
		0xff800000, 
		0xa9bd9d0b, 
		0xbdd3070d, 
		0x00000001, 
		0x28b3fdff, 
		0x40e00000, 
		0x80000200, 
		0x0e00ffff, 
		0x00011111, 
		0x7f000000, 
		0x85fc8d1d, 
		0x00001000, 
		0xf09b0def, 
		0x80000000, 
		0x6732590f, 
		0xc2600000, 
		0xc2080000, 
		0xc1c80000, 
		0x0c780000, 
		0x80000400, 
		0x80000000, 
		0xc2100000, 
		0x00000020, 
		0xa1b1d6e6, 
		0x80000400, 
		0xb4cc12b4, 
		0x00000100, 
		0x33333333, 
		0x0c7fffc0, 
		0x80001000, 
		0x80000100, 
		0x0c7fffc0, 
		0xc2140000, 
		0x41a80000, 
		0x42000000, 
		0x80000002, 
		0x42100000, 
		0xc0400000, 
		0x0d00fff0, 
		0x0c7f8000, 
		0xff7ffffe, 
		0xc26c0000, 
		0x0f7fffff, 
		0x5504ba69, 
		0x80000000, 
		0x80000000, 
		0x80000020, 
		0x00000000, 
		0x7fc00000, 
		0xa8929b1b, 
		0x0ce717c1, 
		0x40a00000, 
		0x807fffff, 
		0x97475650, 
		0xc2340000, 
		0x0e003fff, 
		0xb37ccbc4, 
		0x80800002, 
		0x42780000, 
		0xc2500000, 
		0x0c7fe000, 
		0x00000000, 
		0x40b5f7eb, 
		0x80000002, 
		0x3f800001, 
		0x0f7ffffe, 
		0x0e0007ff, 
		0xc1880000, 
		0xaa3b67e6, 
		0x894adf76, 
		0x1b051d63, 
		0xb135f054, 
		0xc1300000, 
		0x8046c452, 
		0xc2440000, 
		0xff000000, 
		0x0c7ffffe, 
		0x71bb484c, 
		0x80000200, 
		0xa204267d, 
		0x62051941, 
		0x423c0000, 
		0x80200000, 
		0x42700000, 
		0xff800000, 
		0x7f800001, 
		0x6d76de71, 
		0x00000000, 
		0xc2500000, 
		0x00000010, 
		0x87f1e1ef, 
		0x00000080, 
		0x49030919, 
		0xd8965f7d, 
		0x41980000, 
		0x00008000, 
		0xf2449bc9, 
		0xc2700000, 
		0x0e01ffff, 
		0x00000100, 
		0x00004000, 
		0x0d7a7e8a, 
		0x00800002, 
		0x0e00000f, 
		0x685fc9c6, 
		0x13e10599, 
		0x6f30a211, 
		0xa4f2b8d8, 
		0x80000001, 
		0x80001000, 
		0x0f7ffffe, 
		0x00000000, 
		0x80000002, 
		0x50d1164d, 
		0xf3340c39, 
		0xc27c0000, 
		0x41880000, 
		0x41300000, 
		0x06308b41, 
		0x426c0000, 
		0xc2100000, 
		0x00000000, 
		0xc2440000, 
		0x00000002, 
		0xf93358e0, 
		0x42540000, 
		0x007ffffe, 
		0xffc00000, 
		0x80000000, 
		0xdbb7e47a, 
		0x80ffffff, 
		0x3a6927ea, 
		0x9366674e, 
		0x2bf0f6e8, 
		0x8c6837f7, 
		0x0f7ffffe, 
		0x40800000, 
		0x7f800000, 
		0x80000010, 
		0xad39d3dc, 
		0x80000000, 
		0x0c600000, 
		0x41400000, 
		0xff800000, 
		0x41700000, 
		0xff800000, 
		0x80000000, 
		0x908cf37c, 
		0xc2200000, 
		0x41a00000, 
		0xad3c4e8a, 
		0x7f7fffff, 
		0x41b00000, 
		0x42280000, 
		0xffffffff, 
		0xaaaaaaaa, 
		0x8010db14, 
		0xc2380000, 
		0xc0a00000, 
		0x42340000, 
		0x0e007fff, 
		0x30dc5c8c, 
		0x41980000, 
		0x41b00000, 
		0x00000000, 
		0xc1800000, 
		0x00000080, 
		0xe128fcb6, 
		0xd03e8a9a, 
		0x07767305, 
		0x80000002, 
		0x4b000000, 
		0xc26c0000, 
		0xff8a4880, 
		0xcbb50fb3, 
		0xb2be8146, 
		0xff800000, 
		0xc2140000, 
		0x422c0000, 
		0x0e0003ff, 
		0x8e5dcaf6, 
		0x00000400, 
		0xc116ac24, 
		0x0c7e0000, 
		0xc1400000, 
		0x80000008, 
		0x80000100, 
		0x00000000, 
		0x00200000, 
		0x5602b964, 
		0x0e001fff, 
		0xc2000000, 
		0x42740000, 
		0x00000010, 
		0x42580000, 
		0x5bbd645f, 
		0xc2080000, 
		0x80000000, 
		0x7ea50ca1, 
		0x0e0000ff, 
		0xad7702d5, 
		0xffc00000, 
		0xbf800000, 
		0x42600000, 
		0x00000400, 
		0xae26020a, 
		0x80000008, 
		0x9f2911f4, 
		0xdaf5f8b1, 
		0xb60067bd, 
		0x00800003, 
		0xcb9a0e89, 
		0xc2140000, 
		0xb3457b05, 
		0xffc00000, 
		0xb2c883fc, 
		0x80040000, 
		0x538736d8, 
		0x7f7ffffe, 
		0x7f8e669a, 
		0x198e2e22, 
		0x80800002, 
		0xfa532490, 
		0x00000004, 
		0x80100000, 
		0x42080000, 
		0x7f800000, 
		0x0c7f8000, 
		0xff800000, 
		0x16e4f4a6, 
		0xa6d84654, 
		0x0e0001ff, 
		0x0c7ffc00, 
		0x74902db7, 
		0x0f7ffffd, 
		0x00000000, 
		0x7fffffff, 
		0xc20c0000, 
		0x00000040, 
		0xb778db2a, 
		0xff7ffffe, 
		0x00000000, 
		0x80000001, 
		0x802b3383, 
		0x0c7f0000, 
		0x41b00000, 
		0x80432fb8, 
		0x80000000, 
		0x7ff60616, 
		0xff800000, 
		0xc2640000, 
		0xff800000, 
		0x42540000, 
		0x0c7ffc00, 
		0x80040000, 
		0x0e00000f, 
		0x80000000, 
		0x42100000, 
		0xc1a00000, 
		0x4b000000, 
		0xc0c00000, 
		0x80000000, 
		0x8f7ffffe, 
		0x00800003, 
		0x00800001, 
		0xaaaaaaaa, 
		0x55c4454a, 
		0x41800000, 
		0x0e0fffff, 
		0x7c56baf7, 
		0xf37f49ef, 
		0x00000004, 
		0xc0800000, 
		0xe4b2ac71, 
		0x0e01ffff, 
		0x40800000, 
		0x42380000, 
		0x0f7ffffd, 
		0x00000000, 
		0x42240000, 
		0xc1f00000, 
		0x00000000, 
		0xc2480000, 
		0x0e0001ff, 
		0x3ab7a41b, 
		0x00010000, 
		0x0c7fff00, 
		0x29a651a0, 
		0x80000001, 
		0x0ed66cb5, 
		0xbf028f5c, 
		0x395eedc6, 
		0x5544d78a, 
		0xbc40fcd3, 
		0x0c7fffe0, 
		0x42240000, 
		0x7f7ffffe, 
		0x7fb4bba8, 
		0xc1900000, 
		0xbf800000, 
		0x7faf897a, 
		0x80000100, 
		0x41800000, 
		0xff800000, 
		0x7f800000, 
		0x41a00000, 
		0x7f800000, 
		0x80010000, 
		0x0c780000, 
		0xc2280000, 
		0x7f7ffffe, 
		0xc1900000, 
		0xe15efb42, 
		0xc2500000, 
		0x00000010, 
		0x80000000, 
		0xc2700000, 
		0x80004000, 
		0x27688ad3, 
		0xff9ee2d6, 
		0x80000002, 
		0xd907cdc9, 
		0xc2700000, 
		0xccf0cb98, 
		0xb1de7e34, 
		0x7cf8cbf5, 
		0x80800001, 
		0x4b8c4b40, 
		0x7ffca4b2, 
		0xaa5f2f37, 
		0xc1880000, 
		0xc1900000, 
		0x00000001, 
		0x0e7fffff, 
		0x6946650f, 
		0x80000002, 
		0x80000800, 
		0x7f9ca22c, 
		0x05cf7fb9, 
		0xc1200000, 
		0xcbed9a49, 
		0x40a00000, 
		0x32622faf, 
		0xd8339aa5, 
		0xfabc658f, 
		0x41e80000, 
		0x00000010, 
		0x42680000, 
		0x42700000, 
		0x42440000, 
		0xc22c2a14, 
		0x42780000, 
		0x0e00007f, 
		0x7f800000, 
		0x0482f6cb, 
		0x40e00000, 
		0x00000000, 
		0x00000000, 
		0x0c400000, 
		0x80000000, 
		0xc1a00000, 
		0xa4e7ce12, 
		0x0e003fff, 
		0xc2040000, 
		0xc1300000, 
		0xb2cb1ab9, 
		0x41800000, 
		0x41d00000, 
		0x426c0000, 
		0x00000000, 
		0x7f800000, 
		0x80002000, 
		0xc2380000, 
		0xba18d204, 
		0xa8ecd62a, 
		0x40a00000, 
		0x00008000, 
		0x42400000, 
		0x42480000, 
		0x00800000, 
		0xc1d80000, 
		0x00100000, 
		0xc25c0000, 
		0x00000000, 
		0x3c210bf9, 
		0x00000400, 
		0x80000000, 
		0xc1980000, 
		0x7fc00001, 
		0x41d80000, 
		0xffffffff, 
		0x00800001, 
		0x00000001, 
		0x80000008, 
		0x00593717, 
		0x00000001, 
		0xc1e80000, 
		0xc2500000, 
		0x45c457a7, 
		0x54d40f83, 
		0x486902f8, 
		0x00000001, 
		0x7f800000, 
		0xb17ded91, 
		0x0c7f0000, 
		0x40800000, 
		0xbc1dfe56, 
		0x00004000, 
		0xff800000, 
		0x7f800000, 
		0x80000020, 
		0x41300000, 
		0x80800000, 
		0xbb19c2d3, 
		0xa4b47213, 
		0x242be3a9, 
		0x819c6106, 
		0x734d24e9, 
		0x41f00000, 
		0x00000000, 
		0x3e40d913, 
		0xc1600000, 
		0x00002000, 
		0x90cbc055, 
		0x425c0000, 
		0x0c7e0000, 
		0x7f800000, 
		0x4012cde6, 
		0xc61e0d49, 
		0xd162e0f4, 
		0xbe5c3d90, 
		0xc23c0000, 
		0x298a9d9e, 
		0x00000000, 
		0xc2780000, 
		0x41800000, 
		0x00010000, 
		0xc0000000, 
		0x41f00000, 
		0x80000000, 
		0x00000000, 
		0xc2240000, 
		0x00010000, 
		0xc1100000, 
		0x09e9fe75, 
		0xf4c631d1, 
		0x80ffffff, 
		0x00000001, 
		0x0c7ff800, 
		0x80000800, 
		0x24fe824e, 
		0x7f800000, 
		0xc1900000, 
		0x7a670186, 
		0xffbace94, 
		0x2bdd3975, 
		0xff893cac, 
		0x423774cd, 
		0xec5d485c, 
		0x807fffff, 
		0xff7fffff, 
		0x0e0003ff, 
		0x80800001, 
		0xc2100000, 
		0xcccccccc, 
		0xc2080000, 
		0xcf9741a3, 
		0x9cddaca2, 
		0x00002000, 
		0x00020000, 
		0x80000000, 
		0x0c7ffffc, 
		0xf7ad1912, 
		0x0e0001ff, 
		0x7feeda92, 
		0xcb8c4b40, 
		0x80000008, 
		0xc2780000, 
		0x0e07ffff, 
		0x7f7ffffe, 
		0x00000000, 
		0x80000000, 
		0xa2579a01, 
		0x00001000, 
		0x420c0000, 
		0x2478eb00, 
		0x0d000ff0, 
		0x00004000, 
		0x7f800000, 
		0x77253df7, 
		0x00800002, 
		0x80000000, 
		0x7f800000, 
		0xff800000, 
		0x00000400, 
		0x7f7fffff, 
		0x00000000, 
		0x40a00000, 
		0x3f800000, 
		0x42740000, 
		0x007fffff, 
		0x8e315358, 
		0xc0800000, 
		0x00000000, 
		0x8e536992, 
		0x463bca71, 
		0x7ec5b259, 
		0x8f7ffffe, 
		0xf60b7de7, 
		0x0f7fffff, 
		0x1852f09c, 
		0x7f7fffff, 
		0xc1c00000, 
		0x91f3c4da, 
		0xc23c0000, 
		0xc2240000, 
		0x55e87c63, 
		0x3f800000, 
		0xc3d50a8c, 
		0x0c7ff000, 
		0xc2acb765, 
		0x39e2bc07, 
		0xc1d80000, 
		0x8c3d42a0, 
		0xc0a00000, 
		0x7e8415a1, 
		0x00000000, 
		0x80100000, 
		0x00000080, 
		0x0e3fffff, 
		0x00000000, 
		0x80000200, 
		0x80011111, 
		0x80000001, 
		0xc0800000, 
		0xff800000, 
		0xf214ad07, 
		0x23efde74, 
		0x5cdab9d6, 
		0xc20c0000, 
		0x0e1fffff, 
		0x42240000, 
		0x007fffff  
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
		0x00000147, 
		0x00000349, 
		0x0000031f, 
		0x0000008b, 
		0x000001a5, 
		0x000003f9, 
		0x0000006b, 
		0x000002d7, 
		0x0000021b, 
		0x000002c9, 
		0x000003c3, 
		0x000000a5, 
		0x00000085, 
		0x000001a7, 
		0x000002fb, 
		0x00000263, 
		0x00000381, 
		0x00000143, 
		0x000001c9, 
		0x000001b7, 
		0x0000027f, 
		0x00000261, 
		0x00000201, 
		0x0000015b, 
		0x000003f7, 
		0x0000001f, 
		0x00000351, 
		0x0000018d, 
		0x00000191, 
		0x00000369, 
		0x000001f1, 
		0x000003cb, 
		0x000000b3, 
		0x0000018f, 
		0x00000001, 
		0x0000026f, 
		0x0000007d, 
		0x000003a3, 
		0x00000091, 
		0x000001ed, 
		0x000003a5, 
		0x0000039d, 
		0x0000017d, 
		0x00000159, 
		0x000002a9, 
		0x000003fb, 
		0x00000325, 
		0x00000221, 
		0x000001fd, 
		0x0000026b, 
		0x000001e5, 
		0x0000000d, 
		0x0000037d, 
		0x00000311, 
		0x00000379, 
		0x00000137, 
		0x0000013d, 
		0x00000207, 
		0x000002cf, 
		0x0000004b, 
		0x0000001f, 
		0x000002c9, 
		0x00000195, 
		0x000000d5, 
		0x000001e7, 
		0x000000dd, 
		0x00000113, 
		0x000002ab, 
		0x00000021, 
		0x0000027f, 
		0x00000105, 
		0x00000293, 
		0x0000004b, 
		0x000003bb, 
		0x000002bf, 
		0x000002cb, 
		0x00000229, 
		0x00000087, 
		0x0000016b, 
		0x00000089, 
		0x0000002b, 
		0x0000038f, 
		0x0000035f, 
		0x00000113, 
		0x00000271, 
		0x00000323, 
		0x0000024b, 
		0x0000020f, 
		0x000002bb, 
		0x000003a1, 
		0x00000195, 
		0x0000031d, 
		0x000001d9, 
		0x00000045, 
		0x0000027b, 
		0x00000299, 
		0x00000043, 
		0x00000113, 
		0x00000117, 
		0x00000245, 
		0x00000231, 
		0x000002ef, 
		0x00000297, 
		0x000001db, 
		0x000000fb, 
		0x00000345, 
		0x00000387, 
		0x00000227, 
		0x00000025, 
		0x0000028b, 
		0x000002a9, 
		0x00000063, 
		0x000001dd, 
		0x000003a3, 
		0x000002d9, 
		0x000001e9, 
		0x000003db, 
		0x00000393, 
		0x00000363, 
		0x00000315, 
		0x00000189, 
		0x000003c9, 
		0x000002c5, 
		0x000001b3, 
		0x000003c1, 
		0x00000325, 
		0x00000351, 
		0x000003fb, 
		0x000002e7, 
		0x000002ff, 
		0x000002f3, 
		0x00000385, 
		0x0000013d, 
		0x000000d5, 
		0x0000005d, 
		0x0000000f, 
		0x0000004d, 
		0x00000381, 
		0x00000385, 
		0x000003cb, 
		0x0000025d, 
		0x00000189, 
		0x00000353, 
		0x000002e1, 
		0x00000057, 
		0x000001a3, 
		0x00000365, 
		0x00000225, 
		0x0000039f, 
		0x00000311, 
		0x00000171, 
		0x00000231, 
		0x00000153, 
		0x00000239, 
		0x000001f5, 
		0x00000123, 
		0x00000005, 
		0x000001a1, 
		0x000000bd, 
		0x00000097, 
		0x00000101, 
		0x00000163, 
		0x0000023b, 
		0x0000014d, 
		0x000000ef, 
		0x00000001, 
		0x000003e9, 
		0x000002af, 
		0x00000235, 
		0x000003bf, 
		0x000003bf, 
		0x0000017d, 
		0x0000018f, 
		0x0000026d, 
		0x00000299, 
		0x00000129, 
		0x00000127, 
		0x0000012f, 
		0x000001fb, 
		0x000001a5, 
		0x00000307, 
		0x00000299, 
		0x00000249, 
		0x0000001b, 
		0x0000030d, 
		0x000003b7, 
		0x00000145, 
		0x0000031d, 
		0x00000053, 
		0x000002d5, 
		0x000000ad, 
		0x00000055, 
		0x000003dd, 
		0x000001c7, 
		0x00000343, 
		0x00000113, 
		0x0000013b, 
		0x00000209, 
		0x00000043, 
		0x0000025d, 
		0x000003d1, 
		0x000003f7, 
		0x0000034f, 
		0x000001bb, 
		0x00000053, 
		0x000000cf, 
		0x000003c9, 
		0x0000011f, 
		0x00000379, 
		0x000002b3, 
		0x000000e5, 
		0x00000241, 
		0x00000339, 
		0x00000235, 
		0x00000367, 
		0x00000281, 
		0x000003fb, 
		0x0000019b, 
		0x000003bf, 
		0x00000143, 
		0x00000385, 
		0x00000217, 
		0x00000329, 
		0x00000013, 
		0x000000ad, 
		0x00000031, 
		0x00000205, 
		0x000001bb, 
		0x000003cd, 
		0x000000e5, 
		0x00000247, 
		0x000000a1, 
		0x0000015f, 
		0x00000023, 
		0x000001a5, 
		0x00000255, 
		0x0000016d, 
		0x000003f1, 
		0x00000003, 
		0x0000024f, 
		0x0000033b, 
		0x000001b7, 
		0x00000047, 
		0x00000359, 
		0x000000c5, 
		0x0000013d, 
		0x00000357, 
		0x00000333, 
		0x00000159, 
		0x00000215, 
		0x0000037d, 
		0x00000079, 
		0x0000022f, 
		0x000002a3, 
		0x0000012f, 
		0x0000008b  
};

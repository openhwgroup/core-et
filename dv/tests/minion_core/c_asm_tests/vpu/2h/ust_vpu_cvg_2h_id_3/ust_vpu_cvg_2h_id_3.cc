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
			"csrwi mhpmevent3, 20\n"   
			"csrwi mhpmevent4, 2\n"   
			"csrwi mhpmevent5, 21\n"   
			"csrwi mhpmevent6, 27\n"   
			"csrwi mhpmevent7, 5\n"   
			"csrwi mhpmevent8, 3\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f0,  64 (x5)\n"
			"flw.ps f17,  416 (x5)\n"
			"flw.ps f14,  576 (x5)\n"
			"flw.ps f20,  544 (x5)\n"
			"flw.ps f27,  672 (x5)\n"
			"flw.ps f9,  928 (x5)\n"
			"flw.ps f10,  256 (x5)\n"
			"flw.ps f15,  224 (x5)\n"
			"flw.ps f28,  480 (x5)\n"
			"flw.ps f30,  512 (x5)\n"
			"flw.ps f24,  832 (x5)\n"
			"flw.ps f13,  736 (x5)\n"
			"flw.ps f12,  864 (x5)\n"
			"flw.ps f31,  0 (x5)\n"
			"flw.ps f23,  96 (x5)\n"
			"flw.ps f16,  288 (x5)\n"
			"flw.ps f25,  128 (x5)\n"
			"flw.ps f2,  32 (x5)\n"
			"flw.ps f8,  192 (x5)\n"
			"flw.ps f11,  800 (x5)\n"
			"flw.ps f18,  640 (x5)\n"
			"flw.ps f6,  448 (x5)\n"
			"flw.ps f22,  704 (x5)\n"
			"flw.ps f29,  960 (x5)\n"
			"flw.ps f19,  608 (x5)\n"
			"flw.ps f7,  768 (x5)\n"
			"flw.ps f4,  992 (x5)\n"
			"flw.ps f26,  320 (x5)\n"
			"flw.ps f1,  896 (x5)\n"
			"flw.ps f3,  352 (x5)\n"
			"flw.ps f5,  384 (x5)\n"
			"flw.ps f21,  160 (x5)\n"
			"csrw fflags, zero\n"
			"csrwi frm, 0x0\n"
			"LBL_KERNEL_HID_0:\n"          
			"li x5, 0xFEEDFEED\n"
			"li x0, 0x190\n" 
			"slti x0, x0, 0x7f2\n"
			"la x31, check_sum_hid_0\n"
			"li x5, 0x2\n" 
			"sw x5, (x31)\n"
			"li x29, 0x5\n"
			"LBL_OUTER_LOOP_SEQ_HID_0:\n"
			"nop \n"
			"nop \n"
			"li x30, 0\n"
			"addi x30, x30, 1\n"
			"la x23, rand_ieee754_32\n"
			"flw.ps f3, 640(x23)\n"
			"fsgnjx.s f25, f3, f3\n" 
			"la x23, rand_int_32\n"
			"flw.ps f4, 352(x23)\n"
			"flw.ps f0, 352(x23)\n"
			"frem.pi f20, f4, f0\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f0, 832(x13)\n"
			"fcvt.pw.ps f30, f0, rmm\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f8, 64(x5)\n"
			"flw.ps f31, 928(x5)\n"
			"fsgnj.s f30, f8, f31\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f17, 576(x11)\n"
			"flt.ps f4, f17, f17\n" 
			"LBL_SEQID_0_FP_TRANS_ID_0_HID_0:\n"
			"la x27, rand_ieee754_32\n"
			"flw.ps f12, 960(x27)\n"
			"frcp.ps f30, f12\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f1, 224(x12)\n"
			"fclass.ps f3, f1\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f15, 960(x25)\n"
			"fsgnj.s f31, f15, f15\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f8, 544(x16)\n"
			"fcvt.wu.s x27, f8, rdn\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f30, 864(x11)\n"
			"flw.ps f17, 800(x11)\n"
			"fsgnj.ps f3, f30, f17\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f31, 896(x13)\n"
			"flw.ps f0, 448(x13)\n"
			"fsgnj.s f31, f31, f0\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f4, 640(x12)\n"
			"flw.ps f8, 800(x12)\n"
			"feq.s x11, f4, f8\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f8, 640(x17)\n"
			"flw.ps f12, 672(x17)\n"
			"fle.ps f0, f8, f12\n" 
			"la x28, rand_int_32\n"
			"flw.ps f12, 960(x28)\n"
			"fpackreph.pi f20, f12\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f20, 768(x17)\n"
			"fsin.ps f0, f20\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f31, 768(x25)\n"
			"fmin.ps f4, f31, f31\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_0\n"
			"addi x30, x30, 1\n"
			"la x23, rand_int_32\n"
			"flw.ps f0, 576(x23)\n"
			"flw.ps f6, 576(x23)\n"
			"fltu.pi f15, f0, f6\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f8, 544(x26)\n"
			"fsqrt.s f4, f8, rmm\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f12, 704(x12)\n"
			"fnmsub.ps f30, f12, f12, f12, rne\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f30, 288(x11)\n"
			"fcvt.pwu.ps f25, f30, rmm\n" 
			"la x26, rand_int_32\n"
			"flw.ps f4, 320(x26)\n"
			"flw.ps f6, 960(x26)\n"
			"fmin.pi f12, f4, f6\n" 
			"la x23, rand_int_32\n"
			"flw.ps f30, 384(x23)\n"
			"flw.ps f1, 736(x23)\n"
			"fle.pi f20, f30, f1\n" 
			"LBL_SEQID_1_M0_WRITE_ID_1_HID_0:\n"
			"fsetm.pi m0, f5\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0xFF\n"
			"li x28, 0xe99fac2b7b3a279e\n"
			"fcvt.s.wu f25, x28, rne\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f6, 992(x25)\n"
			"flw.ps f8, 0(x25)\n"
			"flw.ps f25, 288(x25)\n"
			"fmadd.ps f0, f6, f8, f25, rne\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f30, 128(x11)\n"
			"fadd.ps f1, f30, f30, rtz\n" 
			"fbci.ps f5, 0x39342\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f4, 416(x23)\n"
			"flw.ps f12, 256(x23)\n"
			"fmax.ps f6, f4, f12\n" 
			"la x12, rand_int_32\n"
			"flw.ps f8, 64(x12)\n"
			"fslli.pi f30, f8, 0x7\n" 
			"la x27, rand_int_32\n"
			"flw.ps f3, 160(x27)\n"
			"flw.ps f6, 96(x27)\n"
			"fsll.pi f30, f3, f6\n" 
			"LBL_SEQID_1_M0_WRITE_ID_2_HID_0:\n"
			"mov.m.x m1, x27, 0x25\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x26, rand_ieee754_32\n"
			"flw.ps f12, 992(x26)\n"
			"flw.ps f30, 288(x26)\n"
			"fle.s x17, f12, f30\n" 
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
			"LBL_HPM_3_CORE_TXFMA_OPS16_FAIL_HID_0:\n"          
			"csrr x5, mhpmcounter3\n"
			"sd x5, 32 (x31)\n"
			"LBL_HPM_4_CORE_RETIRED_INST0_FAIL_HID_0:\n"          
			"csrr x5, mhpmcounter4\n"
			"sd x5, 40 (x31)\n"
			"LBL_HPM_5_CORE_TXFMA_OPS32_FAIL_HID_0:\n"          
			"csrr x5, mhpmcounter5\n"
			"sd x5, 48 (x31)\n"
			"LBL_HPM_6_CORE_TREDUCE_INST_FAIL_HID_0:\n"          
			"csrr x5, mhpmcounter6\n"
			"sd x5, 56 (x31)\n"
			"LBL_HPM_7_NEIGH_RX_COOP_TLD_RSP_FAIL_HID_0:\n"          
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
			"LBL_HPM_3_CORE_TXFMA_OPS16_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter3\n"
			"sd x5, 32 (x31)\n"
			"LBL_HPM_4_CORE_RETIRED_INST0_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter4\n"
			"sd x5, 40 (x31)\n"
			"LBL_HPM_5_CORE_TXFMA_OPS32_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter5\n"
			"sd x5, 48 (x31)\n"
			"LBL_HPM_6_CORE_TREDUCE_INST_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter6\n"
			"sd x5, 56 (x31)\n"
			"LBL_HPM_7_NEIGH_RX_COOP_TLD_RSP_PASS_HID_0:\n"          
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
			"csrwi mhpmevent3, 24\n"   
			"csrwi mhpmevent4, 11\n"   
			"csrwi mhpmevent5, 7\n"   
			"csrwi mhpmevent6, 1\n"   
			"csrwi mhpmevent7, 12\n"   
			"csrwi mhpmevent8, 23\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f30,  192 (x5)\n"
			"flw.ps f31,  320 (x5)\n"
			"flw.ps f7,  992 (x5)\n"
			"flw.ps f8,  32 (x5)\n"
			"flw.ps f2,  128 (x5)\n"
			"flw.ps f9,  512 (x5)\n"
			"flw.ps f15,  736 (x5)\n"
			"flw.ps f12,  64 (x5)\n"
			"flw.ps f6,  800 (x5)\n"
			"flw.ps f10,  704 (x5)\n"
			"flw.ps f4,  160 (x5)\n"
			"flw.ps f1,  544 (x5)\n"
			"flw.ps f29,  416 (x5)\n"
			"flw.ps f18,  960 (x5)\n"
			"flw.ps f16,  864 (x5)\n"
			"flw.ps f22,  896 (x5)\n"
			"flw.ps f5,  576 (x5)\n"
			"flw.ps f13,  640 (x5)\n"
			"flw.ps f27,  384 (x5)\n"
			"flw.ps f20,  0 (x5)\n"
			"flw.ps f25,  224 (x5)\n"
			"flw.ps f14,  608 (x5)\n"
			"flw.ps f11,  288 (x5)\n"
			"flw.ps f19,  480 (x5)\n"
			"flw.ps f17,  256 (x5)\n"
			"flw.ps f26,  448 (x5)\n"
			"flw.ps f3,  928 (x5)\n"
			"flw.ps f21,  768 (x5)\n"
			"flw.ps f23,  672 (x5)\n"
			"flw.ps f0,  832 (x5)\n"
			"flw.ps f28,  352 (x5)\n"
			"flw.ps f24,  96 (x5)\n"
			"csrw fflags, zero\n"
			"csrwi frm, 0x0\n"
			"LBL_KERNEL_HID_1:\n"          
			"li x5, 0xFEEDFEED\n"
			"slti x0, x0, 0x7f2\n"
			"la x31, check_sum_hid_1\n"
			"li x5, 0x2\n" 
			"sw x5, (x31)\n"
			"li x29, 0x5\n"
			"LBL_OUTER_LOOP_SEQ_HID_1:\n"
			"nop \n"
			"nop \n"
			"li x30, 0\n"
			"addi x30, x30, 1\n"
			"la x7, rand_ieee754_32\n"
			"flw.ps f2, 800(x7)\n"
			"flw.ps f4, 224(x7)\n"
			"fsgnjn.ps f20, f2, f4\n" 
			"la x26, rand_int_32\n"
			"flw.ps f10, 576(x26)\n"
			"flw.ps f15, 736(x26)\n"
			"fsrl.pi f10, f10, f15\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f11, 768(x7)\n"
			"flw.ps f6, 480(x7)\n"
			"feq.ps f20, f11, f6\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f20, 896(x11)\n"
			"flw.ps f26, 768(x11)\n"
			"flt.ps f22, f20, f26\n" 
			"la x11, rand_int_32\n"
			"flw.ps f4, 896(x11)\n"
			"flw.ps f26, 320(x11)\n"
			"fle.pi f27, f4, f26\n" 
			"la x12, rand_int_32\n"
			"flw.ps f16, 608(x12)\n"
			"flw.ps f31, 736(x12)\n"
			"fmulhu.pi f20, f16, f31\n" 
			"la x9, rand_int_32\n"
			"flw.ps f10, 128(x9)\n"
			"flw.ps f27, 960(x9)\n"
			"fremu.pi f11, f10, f27\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f10, 800(x6)\n"
			"flw.ps f15, 160(x6)\n"
			"fsgnjx.s f6, f10, f15\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f4, 352(x7)\n"
			"flw.ps f17, 448(x7)\n"
			"fsgnj.ps f10, f4, f17\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f15, 480(x11)\n"
			"fmin.ps f15, f15, f15\n" 
			"la x7, rand_int_32\n"
			"flw.ps f11, 480(x7)\n"
			"fsrai.pi f26, f11, 0xc\n" 
			"LBL_SEQID_0_FP_TRANS_ID_0_HID_1:\n"
			"la x12, rand_ieee754_32\n"
			"flw.ps f17, 640(x12)\n"
			"flog.ps f22, f17\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f17, 480(x28)\n"
			"flw.ps f10, 384(x28)\n"
			"flw.ps f0, 736(x28)\n"
			"fmsub.s f2, f17, f10, f0, rne\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f22, 64(x7)\n"
			"fround.ps f10, f22, rtz\n" 
			"maskpopcz x11, m7\n" 
			"la x7, rand_int_32\n"
			"flw.ps f13, 832(x7)\n"
			"flw.ps f27, 672(x7)\n"
			"fsra.pi f0, f13, f27\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_1\n"
			"addi x30, x30, 1\n"
			"maskpopcz x6, m4\n" 
			"la x10, rand_ieee754_16\n"
			"flw.ps f11, 864(x10)\n"
			"fcvt.ps.f16 f11, f11\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f10, 192(x12)\n"
			"flw.ps f2, 320(x12)\n"
			"fmul.s f10, f10, f2, rmm\n" 
			"la x23, rand_int_32\n"
			"flw.ps f4, 928(x23)\n"
			"flw.ps f15, 544(x23)\n"
			"fltu.pi f10, f4, f15\n" 
			"la x10, rand_int_32\n"
			"flw.ps f20, 640(x10)\n"
			"flw.ps f4, 768(x10)\n"
			"fmaxu.pi f26, f20, f4\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f11, 672(x12)\n"
			"fswizz.ps f13, f11, 0xf\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f20, 160(x28)\n"
			"fclass.s x6, f20\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f26, 640(x9)\n"
			"flw.ps f27, 352(x9)\n"
			"fadd.s f16, f26, f27, rmm\n" 
			"LBL_SEQID_1_M0_WRITE_ID_1_HID_1:\n"
			"mova.m.x x26\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x6, rand_int_32\n"
			"flw.ps f31, 128(x6)\n"
			"fpackrepb.pi f18, f31\n" 
			"la x26, rand_int_32\n"
			"flw.ps f15, 640(x26)\n"
			"faddi.pi f26, f15, 0x97\n" 
			"LBL_SEQID_1_M0_WRITE_ID_2_HID_1:\n"
			"feqm.ps m4, f26, f15\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x27, rand_int_32\n"
			"flw.ps f2, 96(x27)\n"
			"flw.ps f16, 576(x27)\n"
			"fmin.pi f17, f2, f16\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f22, 320(x12)\n"
			"fclass.ps f31, f22\n" 
			"la x28, rand_int_32\n"
			"flw.ps f4, 320(x28)\n"
			"fsrai.pi f17, f4, 0x3\n" 
			"la x9, rand_int_32\n"
			"flw.ps f13, 672(x9)\n"
			"fmulhu.pi f26, f13, f13\n" 
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
			"LBL_HPM_3_CORE_SHORT_OPS_FAIL_HID_1:\n"          
			"csrr x5, mhpmcounter3\n"
			"sd x5, 32 (x31)\n"
			"LBL_HPM_4_CORE_L2_MISS_REQ_REJ_FAIL_HID_1:\n"          
			"csrr x5, mhpmcounter4\n"
			"sd x5, 40 (x31)\n"
			"LBL_HPM_5_CORE_DCACHE_ACCESS1_FAIL_HID_1:\n"          
			"csrr x5, mhpmcounter5\n"
			"sd x5, 48 (x31)\n"
			"LBL_HPM_6_CORE_MCYCLES_FAIL_HID_1:\n"          
			"csrr x5, mhpmcounter6\n"
			"sd x5, 56 (x31)\n"
			"LBL_HPM_7_NEIGH_INTER_CORE_MSG_TX_FAIL_HID_1:\n"          
			"csrr x5, mhpmcounter7\n"
			"sd x5, 64 (x31)\n"
			"LBL_HPM_8_NEIGH_ETL_RSP_FROM_SC_UC_FAIL_HID_1:\n"          
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
			"LBL_HPM_3_CORE_SHORT_OPS_PASS_HID_1:\n"          
			"csrr x5, mhpmcounter3\n"
			"sd x5, 32 (x31)\n"
			"LBL_HPM_4_CORE_L2_MISS_REQ_REJ_PASS_HID_1:\n"          
			"csrr x5, mhpmcounter4\n"
			"sd x5, 40 (x31)\n"
			"LBL_HPM_5_CORE_DCACHE_ACCESS1_PASS_HID_1:\n"          
			"csrr x5, mhpmcounter5\n"
			"sd x5, 48 (x31)\n"
			"LBL_HPM_6_CORE_MCYCLES_PASS_HID_1:\n"          
			"csrr x5, mhpmcounter6\n"
			"sd x5, 56 (x31)\n"
			"LBL_HPM_7_NEIGH_INTER_CORE_MSG_TX_PASS_HID_1:\n"          
			"csrr x5, mhpmcounter7\n"
			"sd x5, 64 (x31)\n"
			"LBL_HPM_8_NEIGH_ETL_RSP_FROM_SC_UC_PASS_HID_1:\n"          
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
		0xb8f539a3, 
		0xb8f4d182, 
		0xfb316aac, 
		0xf6fdc02e, 
		0xe644f7c6, 
		0x8e1e8e3c, 
		0xf5cc65d4, 
		0x6f86e4e9, 
		0x1934fe94, 
		0xe301767d, 
		0x60917f3f, 
		0xbf3778da, 
		0x6cb32460, 
		0xb83ccb82, 
		0xed13f7f9, 
		0x898ecb52, 
		0xda645761, 
		0x651aff1f, 
		0xb65fd3c8, 
		0x8ecda025, 
		0xa8266995, 
		0x2e3724a2, 
		0x2e8297d4, 
		0x2fa65bc2, 
		0xcafcb80e, 
		0x046f1625, 
		0xbd86dc2d, 
		0x1a8bc4f3, 
		0xc0dd856c, 
		0x42fe5fee, 
		0x35e89735, 
		0x11bcb9b7, 
		0x4b40c61d, 
		0x40397f29, 
		0xb17f96ca, 
		0xc0ccbf12, 
		0x292410d6, 
		0xbeea55b7, 
		0xf0ef5144, 
		0xfa49774d, 
		0x05efe7c6, 
		0x1084285d, 
		0xa79f323e, 
		0xaff43eeb, 
		0x748ed385, 
		0x5f135f72, 
		0x4d3d64de, 
		0x5e84069d, 
		0xf34e3180, 
		0x545458d6, 
		0x4aa28503, 
		0x0a4788ce, 
		0xae2c385e, 
		0xbcf89d07, 
		0xe684eecb, 
		0xb2bded85, 
		0x5eb6070e, 
		0x55b11591, 
		0xe01cb32a, 
		0x18c24073, 
		0x418f18b2, 
		0xdaed49bd, 
		0x4dfa490c, 
		0x2359675b, 
		0xd40b5bc5, 
		0x588213e4, 
		0x81cb5394, 
		0xab0bf2d1, 
		0x92520cd1, 
		0xd2299c11, 
		0xf2d8d8c0, 
		0x7e443982, 
		0x60f46d39, 
		0xe092c1dd, 
		0x6d9eab81, 
		0xa2412955, 
		0x9ce54465, 
		0x2a723351, 
		0x3e792c1f, 
		0xfd41344c, 
		0xe7e1ebbd, 
		0xd1e88382, 
		0x339bf960, 
		0x30a07335, 
		0x5aa6f11f, 
		0x2cb15a19, 
		0x204006f1, 
		0xa289a493, 
		0xf7a6d95b, 
		0xaf7c2359, 
		0x8a4461b3, 
		0x60a42c9a, 
		0x25bc019f, 
		0xeacb301a, 
		0x90128f1b, 
		0xf84a2860, 
		0x49fd63cb, 
		0x9ea2dd2b, 
		0x3b348e8b, 
		0x84ebe9dd, 
		0x92efc530, 
		0x3724f274, 
		0x4a9f22bf, 
		0x3911b8fb, 
		0x88ca16b0, 
		0x20497f6c, 
		0xf7dcacf9, 
		0x4260c991, 
		0xdbbf6993, 
		0x6011bcca, 
		0x24565986, 
		0x3f936365, 
		0x95198029, 
		0x67722919, 
		0xa7d43717, 
		0x97e781b8, 
		0x811bd12a, 
		0x80bc0201, 
		0xaad24e8d, 
		0xf1c724c3, 
		0x2c18d51c, 
		0xc005bb9a, 
		0xac27d8b5, 
		0x854bae44, 
		0x49686f4e, 
		0xa6d1054b, 
		0x6c37731d, 
		0xaa64269d, 
		0x8802954e, 
		0x628e88e1, 
		0x357f6883, 
		0x09e435b3, 
		0x5e6eccb7, 
		0x2ec28fdf, 
		0x4e727c50, 
		0x6fa4829f, 
		0x29a94a68, 
		0x1e62734e, 
		0xc131da75, 
		0x4e36403c, 
		0x9aedb628, 
		0x47713eee, 
		0x5fbb672e, 
		0x8f08e9cc, 
		0xedcff31a, 
		0x306e6332, 
		0xbc7d1f83, 
		0x65b1f1de, 
		0x6391a12b, 
		0xfb451c20, 
		0x7c7adeef, 
		0x3c5fd1d0, 
		0x35ee1209, 
		0xac9f362c, 
		0x0aa0c107, 
		0xb7b8064d, 
		0xeb928723, 
		0xb069dc8e, 
		0x8a2ea55f, 
		0x28bd8263, 
		0x88d9e6b0, 
		0x4a9072f0, 
		0x3c8f4d68, 
		0xc30c296a, 
		0xac14e117, 
		0x8da9a588, 
		0x7e409a91, 
		0xbd696c16, 
		0xd35c4d61, 
		0x05ac6c7e, 
		0x2be8567e, 
		0x55510317, 
		0x0ebd822c, 
		0x1353fc88, 
		0x287e9aec, 
		0x7a9cb0ec, 
		0x3d7b6a84, 
		0xd1b42cab, 
		0xde3da47e, 
		0xd467646b, 
		0x3a34ca01, 
		0x73a132f8, 
		0x7638e397, 
		0x65be760c, 
		0xf33085bc, 
		0x16b01297, 
		0xb7094f22, 
		0x4b425fd1, 
		0xa3adca66, 
		0x41193568, 
		0xf336fc65, 
		0x00b71bde, 
		0x53147e36, 
		0xc386d177, 
		0x017b11ce, 
		0x33ab1dfb, 
		0x7319c0db, 
		0xb6a44b66, 
		0x15960658, 
		0xa0ed5b9a, 
		0x5efd98a6, 
		0xa435ea07, 
		0xe3bf6313, 
		0x41349767, 
		0x1d9cf3c8, 
		0xc5744fe9, 
		0xa0e33cb7, 
		0x59830f27, 
		0xd9f2cb9e, 
		0x4f3342fe, 
		0x8ac0f697, 
		0x6bd827fb, 
		0x95051ea0, 
		0x80547980, 
		0xa15f5bbb, 
		0x6a652cf6, 
		0x2a0c34d2, 
		0xcb39b760, 
		0x36d87845, 
		0x856869f8, 
		0x6d606294, 
		0x8d0dbc8d, 
		0xaa93067b, 
		0x7302227d, 
		0xd5763cde, 
		0x57b45ee3, 
		0xe66eb10f, 
		0xd5985480, 
		0x1a2056b5, 
		0x4f539f77, 
		0xe2d23174, 
		0x89b4205f, 
		0x3d506ac3, 
		0x8e16c1a2, 
		0x19b2c960, 
		0xb20fcc9b, 
		0xb976bc9c, 
		0x74ef75af, 
		0xb8d0c4ca, 
		0x63c0aaaa, 
		0xddb9ba4e, 
		0x0de0f097, 
		0x41d3e392, 
		0xedd8014e, 
		0x8535846c, 
		0x81304a15, 
		0xba04fdb9, 
		0x0ecac38f, 
		0xd36f77f0, 
		0x8a7eba6f, 
		0x0b5c00c2, 
		0x8cd31331, 
		0x27dbf8b6, 
		0xfe521e71, 
		0xaddd3253, 
		0x9b45731b  
};
volatile uint16_t rand_ieee754_16[1536] __attribute__ ((aligned (2048))) = {
		0xd040, 
		0x4560, 
		0xd2a0, 
		0x53c0, 
		0xb62a, 
		0x5240, 
		0x5300, 
		0x2227, 
		0x50a0, 
		0x8000, 
		0x5020, 
		0x53a0, 
		0x4700, 
		0x2688, 
		0xd000, 
		0x0000, 
		0xd2a0, 
		0x9303, 
		0x4d80, 
		0x4ec0, 
		0x7378, 
		0x5040, 
		0x0000, 
		0xa7ab, 
		0x4d40, 
		0x195e, 
		0xd280, 
		0x36a4, 
		0x9cda, 
		0x0382, 
		0x4e40, 
		0xd040, 
		0x4700, 
		0xcfc0, 
		0xd160, 
		0x74df, 
		0xd2e0, 
		0x5320, 
		0xd300, 
		0xd260, 
		0xd300, 
		0xc700, 
		0x0000, 
		0x5040, 
		0x4a00, 
		0x8000, 
		0x3ee1, 
		0x51e0, 
		0x8fef, 
		0xd000, 
		0x0000, 
		0x8de8, 
		0xc000, 
		0x4600, 
		0x4b00, 
		0x5360, 
		0xd0c0, 
		0xd3e0, 
		0xd100, 
		0xa7fc, 
		0xcf40, 
		0xd060, 
		0x27af, 
		0xcd00, 
		0x1c7a, 
		0xd2c0, 
		0x7c00, 
		0x5140, 
		0x4500, 
		0x51c0, 
		0xd3a0, 
		0x5120, 
		0xd200, 
		0x6ad6, 
		0x4e40, 
		0x51a0, 
		0x5060, 
		0x805e, 
		0x53e0, 
		0x51a0, 
		0x4c40, 
		0xaca5, 
		0x5080, 
		0xd220, 
		0xcdc0, 
		0x8000, 
		0xeb0e, 
		0xda43, 
		0xcfc0, 
		0x53c0, 
		0x5180, 
		0xe308, 
		0x4000, 
		0x51e0, 
		0x674f, 
		0x184b, 
		0x0000, 
		0x7689, 
		0x7ceb, 
		0x22a1, 
		0x1827, 
		0xc700, 
		0xce40, 
		0xd180, 
		0xcb80, 
		0xca00, 
		0x4c00, 
		0x8d04, 
		0x06fc, 
		0x8000, 
		0xab6b, 
		0x5180, 
		0x7c00, 
		0xc980, 
		0x8000, 
		0x53a0, 
		0x5200, 
		0xd1e7, 
		0x5100, 
		0x4a80, 
		0x4f00, 
		0xc980, 
		0x4e40, 
		0xe40e, 
		0x4f40, 
		0x4b00, 
		0x3c00, 
		0xd3c0, 
		0x5220, 
		0xd2e0, 
		0x1ba6, 
		0xce40, 
		0x5056, 
		0x50c0, 
		0x559c, 
		0xd1a0, 
		0xc880, 
		0x5020, 
		0x07c0, 
		0x4ec0, 
		0x4fc0, 
		0xfc00, 
		0x5180, 
		0xfc30, 
		0x1edf, 
		0x8000, 
		0xb9d5, 
		0xccc6, 
		0x5020, 
		0xcb00, 
		0x4900, 
		0xcdc0, 
		0x61d9, 
		0xce80, 
		0x5040, 
		0x8000, 
		0x5160, 
		0x4d80, 
		0xcf40, 
		0xd8c7, 
		0xd320, 
		0xa6ac, 
		0x4900, 
		0xc700, 
		0xfc00, 
		0x4900, 
		0x53c0, 
		0xfc4d, 
		0xfc00, 
		0x0000, 
		0xc880, 
		0x8000, 
		0xb740, 
		0x7762, 
		0xcfc0, 
		0x5240, 
		0xd1c0, 
		0xa2cf, 
		0xd080, 
		0x53a0, 
		0x5180, 
		0xcb00, 
		0x2100, 
		0x0000, 
		0xb9dd, 
		0x51a0, 
		0x7c09, 
		0xc2e2, 
		0x5077, 
		0x823f, 
		0xce40, 
		0x0000, 
		0x7c00, 
		0x53e0, 
		0xd120, 
		0x53e0, 
		0x4b80, 
		0x5160, 
		0xe6f8, 
		0xe2fa, 
		0xcf00, 
		0x6392, 
		0x4fc0, 
		0x4f40, 
		0xca80, 
		0x4d00, 
		0x86de, 
		0x4500, 
		0x5060, 
		0x0da3, 
		0x4d80, 
		0x53a0, 
		0xc5a4, 
		0xd120, 
		0x5220, 
		0xd020, 
		0xcfc0, 
		0x5180, 
		0xce40, 
		0xef7e, 
		0xce40, 
		0xc900, 
		0x0000, 
		0x5020, 
		0x4400, 
		0x7825, 
		0x5120, 
		0x8146, 
		0xbdab, 
		0x8453, 
		0xd200, 
		0x4880, 
		0x5280, 
		0x4700, 
		0x6226, 
		0xd220, 
		0xc800, 
		0xd0a0, 
		0xd060, 
		0x7c00, 
		0x6929, 
		0xa0f1, 
		0x52c0, 
		0x4e00, 
		0xd220, 
		0xd1c0, 
		0x0000, 
		0x9398, 
		0xa3db, 
		0xd120, 
		0xcfc0, 
		0xd908, 
		0xd3c0, 
		0xcd80, 
		0x4a00, 
		0xf373, 
		0xce00, 
		0xa259, 
		0x8cec, 
		0x4f80, 
		0x16b9, 
		0x58fc, 
		0x4ec0, 
		0x4900, 
		0xccc0, 
		0xbc00, 
		0xfc00, 
		0x8000, 
		0x8d9d, 
		0x03dc, 
		0x0000, 
		0xd380, 
		0x8000, 
		0xce80, 
		0x5020, 
		0x5340, 
		0xcd40, 
		0x8479, 
		0xcfc0, 
		0x3e2d, 
		0x5300, 
		0x4357, 
		0xb4fb, 
		0xd160, 
		0x8c1e, 
		0x2d48, 
		0xd2e0, 
		0x4cc0, 
		0x4f40, 
		0x58d3, 
		0x4900, 
		0xd360, 
		0x92f0, 
		0x52a0, 
		0xd240, 
		0xc600, 
		0xd0a0, 
		0xfc00, 
		0x6367, 
		0x50e0, 
		0x5300, 
		0x7dce, 
		0x5040, 
		0xbc00, 
		0x53c0, 
		0x4cc0, 
		0xc880, 
		0xc980, 
		0x5300, 
		0x4c80, 
		0x0000, 
		0x53a0, 
		0xd380, 
		0xcd80, 
		0xcc80, 
		0x5e79, 
		0xfd24, 
		0x0879, 
		0x4400, 
		0xd240, 
		0xfc00, 
		0x4d80, 
		0xd3e0, 
		0x5140, 
		0x8020, 
		0x8e1b, 
		0x5080, 
		0xbc27, 
		0xd402, 
		0xc980, 
		0x4700, 
		0xd300, 
		0xc600, 
		0x4b00, 
		0x4576, 
		0xd280, 
		0xd040, 
		0x4700, 
		0xc000, 
		0x0000, 
		0x5220, 
		0xcf0d, 
		0x4c80, 
		0x40d1, 
		0x53c0, 
		0x5120, 
		0xccc0, 
		0x4ec0, 
		0x4dc0, 
		0x52c0, 
		0x4e00, 
		0xd140, 
		0xd140, 
		0x0a73, 
		0x1297, 
		0x51a0, 
		0x4c00, 
		0xd2e0, 
		0x53c0, 
		0x7c00, 
		0x53a0, 
		0x5380, 
		0x7454, 
		0xbb96, 
		0x4e40, 
		0x3ba2, 
		0xccc0, 
		0xa620, 
		0xcd00, 
		0x4e80, 
		0xd320, 
		0x4000, 
		0x0000, 
		0x774f, 
		0xabba, 
		0xd200, 
		0x7c00, 
		0x5eab, 
		0x67f9, 
		0x5488, 
		0xdad0, 
		0xd300, 
		0x4700, 
		0xcec0, 
		0x5280, 
		0xce40, 
		0x4000, 
		0xd380, 
		0xcb00, 
		0xd1e0, 
		0xd220, 
		0xfc72, 
		0xd100, 
		0xe3ac, 
		0xcc40, 
		0xd040, 
		0x8000, 
		0x51a0, 
		0x4000, 
		0xd240, 
		0x1680, 
		0xf225, 
		0x0ff2, 
		0x5320, 
		0xcc40, 
		0x51e0, 
		0x5160, 
		0x4880, 
		0xd320, 
		0xce40, 
		0x5000, 
		0x53a0, 
		0xd298, 
		0xc8f0, 
		0xd200, 
		0x4c80, 
		0xcd40, 
		0xc880, 
		0x4a80, 
		0x50c0, 
		0x5280, 
		0x8000, 
		0xc880, 
		0x4e80, 
		0x08bf, 
		0x4e80, 
		0x5160, 
		0x4f40, 
		0xb89d, 
		0x32e9, 
		0x0e24, 
		0x4d20, 
		0xd100, 
		0xd000, 
		0x865c, 
		0x5100, 
		0x4e00, 
		0x909b, 
		0x52e0, 
		0x4880, 
		0x4b00, 
		0xcd40, 
		0xc000, 
		0x52c0, 
		0x52a0, 
		0x53a0, 
		0x4b80, 
		0x4c00, 
		0x5280, 
		0x4b00, 
		0x5220, 
		0x54e8, 
		0xcb00, 
		0xc700, 
		0x52c0, 
		0x7fc2, 
		0x50a0, 
		0x10b4, 
		0xd2a0, 
		0x5180, 
		0x5300, 
		0x01c6, 
		0x53a0, 
		0x52a0, 
		0x960c, 
		0xbc00, 
		0xcc40, 
		0x8000, 
		0xce00, 
		0x4dc0, 
		0x4a00, 
		0x4e80, 
		0xc200, 
		0x4dc0, 
		0xd080, 
		0xd060, 
		0xb2bb, 
		0x0590, 
		0x4e00, 
		0x5280, 
		0x4e00, 
		0x06c5, 
		0x5100, 
		0xc880, 
		0x4e00, 
		0x0000, 
		0xce00, 
		0x2653, 
		0x4a00, 
		0xbd7e, 
		0xcb80, 
		0xd3e0, 
		0xc200, 
		0x4cc0, 
		0x4600, 
		0x7c00, 
		0x51a0, 
		0x4880, 
		0xd2e0, 
		0x5380, 
		0x52a0, 
		0x5020, 
		0x7ed6, 
		0x4a00, 
		0x53c0, 
		0x7d88, 
		0xcf00, 
		0x5080, 
		0xd280, 
		0x4d40, 
		0xc600, 
		0x4e80, 
		0xcf00, 
		0x5180, 
		0xd140, 
		0xee6f, 
		0xce40, 
		0x4d00, 
		0x4e80, 
		0xfc00, 
		0xcd40, 
		0x4cc0, 
		0x4700, 
		0xc900, 
		0xcd80, 
		0xd240, 
		0x7c00, 
		0x5240, 
		0x0000, 
		0xec1a, 
		0x6540, 
		0x8000, 
		0xd618, 
		0xabf3, 
		0x53e0, 
		0x80e9, 
		0x79eb, 
		0xd160, 
		0xd260, 
		0x4f40, 
		0xc4f7, 
		0x5120, 
		0xfc8a, 
		0x0e3a, 
		0x8299, 
		0x4c00, 
		0x4cc0, 
		0xcc40, 
		0x8000, 
		0xd360, 
		0x4700, 
		0xd360, 
		0xfc00, 
		0x6d6f, 
		0x0000, 
		0xd0e0, 
		0xe7df, 
		0xfc00, 
		0xd380, 
		0x5360, 
		0xd180, 
		0x9268, 
		0xe9f5, 
		0xcd40, 
		0xd636, 
		0xab2e, 
		0x5340, 
		0x8000, 
		0x51c0, 
		0xcfc0, 
		0xaa40, 
		0xd2c0, 
		0x52c0, 
		0x54cf, 
		0x9cee, 
		0xfc00, 
		0xd120, 
		0xb302, 
		0x4e80, 
		0x5360, 
		0xcf00, 
		0x4a00, 
		0x4700, 
		0xc400, 
		0x8000, 
		0x8000, 
		0x38ad, 
		0xd3a0, 
		0x51a0, 
		0x4d00, 
		0xb5c5, 
		0xf5ff, 
		0x5dad, 
		0x5140, 
		0xfb98, 
		0xcf40, 
		0x8e97, 
		0x51e0, 
		0xd020, 
		0x0a5f, 
		0xce00, 
		0xccc0, 
		0xcc80, 
		0xca80, 
		0x545e, 
		0x4b00, 
		0xd413, 
		0x5220, 
		0xcf80, 
		0x50c0, 
		0xce40, 
		0x8000, 
		0x52e0, 
		0xd260, 
		0x4fc0, 
		0xc980, 
		0xfd91, 
		0xd340, 
		0x51e0, 
		0x4cc0, 
		0x4c40, 
		0x5060, 
		0xa73e, 
		0xd2a0, 
		0xd3a0, 
		0xc700, 
		0x1aca, 
		0xd1a0, 
		0x4c80, 
		0x5100, 
		0x7dad, 
		0xc600, 
		0x5020, 
		0x2160, 
		0x1596, 
		0x112e, 
		0x5220, 
		0x4980, 
		0x379b, 
		0xd100, 
		0x4fc0, 
		0x0000, 
		0x53c0, 
		0xcf40, 
		0x81bf, 
		0x53fc, 
		0x8000, 
		0x4980, 
		0xc980, 
		0x53a0, 
		0xcd00, 
		0x8000, 
		0x51e0, 
		0xd2c0, 
		0x5220, 
		0x4200, 
		0xc6b2, 
		0xd2e0, 
		0x4a00, 
		0xd200, 
		0xccc0, 
		0x5280, 
		0xe6a2, 
		0xd060, 
		0xb627, 
		0x4600, 
		0xb1b5, 
		0x4fc0, 
		0xd020, 
		0xfc00, 
		0x5060, 
		0x7f60, 
		0xfc12, 
		0xd3c0, 
		0x109f, 
		0xfdc4, 
		0x5100, 
		0xcfc0, 
		0xd340, 
		0x291c, 
		0x9919, 
		0xd1a0, 
		0x7c00, 
		0x4fc0, 
		0xfc00, 
		0x4500, 
		0xcd40, 
		0xd819, 
		0x1f0b, 
		0xd1e0, 
		0x0000, 
		0x51c0, 
		0x52e0, 
		0x8000, 
		0xd100, 
		0xcf00, 
		0xd140, 
		0x52e0, 
		0x4700, 
		0xd360, 
		0x4600, 
		0x8000, 
		0x5240, 
		0x50e0, 
		0x8000, 
		0xc800, 
		0x4980, 
		0x8a7b, 
		0x8b8f, 
		0xc500, 
		0xc500, 
		0xd280, 
		0xd0a0, 
		0x4700, 
		0x8000, 
		0x8000, 
		0xd360, 
		0xcc80, 
		0x76f1, 
		0xc8ab, 
		0xaca2, 
		0x4cc0, 
		0xe153, 
		0x4400, 
		0x4a45, 
		0x4c80, 
		0x53e0, 
		0xc200, 
		0x52a0, 
		0x5140, 
		0x6b36, 
		0x8000, 
		0x379d, 
		0xd3c0, 
		0x51d4, 
		0x8000, 
		0x3217, 
		0xa3e3, 
		0x5000, 
		0xce60, 
		0x4400, 
		0x7c00, 
		0xfc00, 
		0xb278, 
		0xd1e0, 
		0x5320, 
		0x232e, 
		0x683f, 
		0x2ee6, 
		0xcf80, 
		0xb65f, 
		0xd320, 
		0x8738, 
		0xcd00, 
		0x0000, 
		0x4200, 
		0x8a3f, 
		0xcc80, 
		0xd300, 
		0xd260, 
		0x2cdf, 
		0xd2e0, 
		0xcb00, 
		0xf264, 
		0xd280, 
		0x52c0, 
		0x4fc0, 
		0x9905, 
		0x5260, 
		0x50e0, 
		0x51af, 
		0xaec5, 
		0x5380, 
		0x6961, 
		0xd1e0, 
		0xd1e0, 
		0xda92, 
		0x386a, 
		0xbbee, 
		0x4600, 
		0xd200, 
		0xbc00, 
		0x792f, 
		0xc23b, 
		0x2c68, 
		0x4b00, 
		0xd080, 
		0xcc00, 
		0x4b71, 
		0x4700, 
		0x5360, 
		0x4f00, 
		0x804a, 
		0xd280, 
		0xfe6f, 
		0xd240, 
		0xd120, 
		0xc700, 
		0x4f40, 
		0xcd40, 
		0x8f04, 
		0x53c0, 
		0xb23c, 
		0xc400, 
		0xdb8c, 
		0x4700, 
		0xc000, 
		0x51e0, 
		0xd280, 
		0x5140, 
		0xcec0, 
		0x5300, 
		0x4758, 
		0xb8a4, 
		0x0319, 
		0x20ae, 
		0xd160, 
		0xca00, 
		0xcd80, 
		0xcc00, 
		0x7c00, 
		0x4e40, 
		0x24d9, 
		0xc84f, 
		0x5240, 
		0x0000, 
		0xcd80, 
		0x8000, 
		0x8000, 
		0x4c80, 
		0x15f7, 
		0x8000, 
		0xd3e0, 
		0xca00, 
		0x19ac, 
		0xd3a0, 
		0x53c0, 
		0x5200, 
		0x6e9a, 
		0x7c00, 
		0x4400, 
		0x4c40, 
		0x0000, 
		0x9d40, 
		0x4d00, 
		0x5220, 
		0xd140, 
		0x4700, 
		0x560a, 
		0x8000, 
		0x4200, 
		0x0000, 
		0x5320, 
		0x8000, 
		0x2fe0, 
		0x4dc0, 
		0x4ec0, 
		0xabc5, 
		0xd240, 
		0x4e40, 
		0x111f, 
		0x6dcf, 
		0xcf80, 
		0xee46, 
		0x0618, 
		0x086c, 
		0x4b00, 
		0xd3a0, 
		0x8893, 
		0x5240, 
		0x06e7, 
		0x8000, 
		0x51c0, 
		0xc843, 
		0xd200, 
		0x7c00, 
		0xa3b6, 
		0x0066, 
		0xfc00, 
		0x4cc0, 
		0xcb00, 
		0x5020, 
		0xd2e0, 
		0xd320, 
		0x7c00, 
		0xc900, 
		0x53e0, 
		0x8000, 
		0x5140, 
		0xc159, 
		0x4500, 
		0x4880, 
		0xd020, 
		0x36e7, 
		0x8000, 
		0x5320, 
		0x5f72, 
		0xb16f, 
		0xcdc0, 
		0xd3a0, 
		0xd060, 
		0x4e80, 
		0x5340, 
		0xce40, 
		0x4dc0, 
		0x0000, 
		0x36f2, 
		0xb485, 
		0x4d80, 
		0xca80, 
		0x4c80, 
		0x0bc3, 
		0x5360, 
		0xd1a0, 
		0x06c0, 
		0xf158, 
		0xd280, 
		0x4600, 
		0x417a, 
		0x5300, 
		0xfaa1, 
		0xd180, 
		0x6c89, 
		0x52c0, 
		0x5220, 
		0xbaa4, 
		0xcc40, 
		0xd100, 
		0xd240, 
		0x4fc0, 
		0x0000, 
		0x2947, 
		0xd340, 
		0x4e00, 
		0xd0a0, 
		0xf397, 
		0xcb00, 
		0xc900, 
		0xd0c0, 
		0x2075, 
		0xc800, 
		0x4fc0, 
		0xd320, 
		0x5060, 
		0xd2c0, 
		0x3b72, 
		0x7c2a, 
		0x0000, 
		0xd3a0, 
		0xc000, 
		0xd120, 
		0xd3e0, 
		0x4c40, 
		0x8a49, 
		0xd260, 
		0x51a0, 
		0xc500, 
		0xfc00, 
		0x0000, 
		0xd92a, 
		0x4f80, 
		0x52e0, 
		0xfc00, 
		0x0000, 
		0xd32c, 
		0xcfc0, 
		0x8457, 
		0x026c, 
		0xd060, 
		0x4c40, 
		0x4e80, 
		0x5080, 
		0x4880, 
		0x51e0, 
		0x5040, 
		0x0000, 
		0x43ec, 
		0x0000, 
		0xb536, 
		0xd3e0, 
		0xd1e0, 
		0x0000, 
		0xf8b1, 
		0x4ec0, 
		0x4b80, 
		0xc400, 
		0x50a0, 
		0xd2c0, 
		0x9781, 
		0xcf80, 
		0xd040, 
		0x4b00, 
		0xedd1, 
		0xccc0, 
		0xce00, 
		0xcdc0, 
		0xcfc0, 
		0x4000, 
		0x4d00, 
		0xca00, 
		0xce80, 
		0x5220, 
		0x50c0, 
		0x4f80, 
		0xe246, 
		0x45bd, 
		0xd0c0, 
		0x3126, 
		0xa1bb, 
		0xd2e0, 
		0x0000, 
		0xfc00, 
		0xd2c0, 
		0xfd28, 
		0xd2e0, 
		0x8000, 
		0xc400, 
		0xd0a0, 
		0x5160, 
		0xcc80, 
		0xbc5e, 
		0xd080, 
		0x8000, 
		0xac64, 
		0xcf00, 
		0x7bc1, 
		0xca00, 
		0x4e40, 
		0x50a0, 
		0x98c9, 
		0x42ab, 
		0xd1e8, 
		0xd260, 
		0xcd00, 
		0xcf00, 
		0xe274, 
		0xd100, 
		0x5200, 
		0x8000, 
		0xd220, 
		0x50c0, 
		0xc880, 
		0x082f, 
		0xfc00, 
		0x3534, 
		0x19d0, 
		0x8000, 
		0x8eb4, 
		0xcc00, 
		0xce00, 
		0xd2a0, 
		0x1012, 
		0x5a72, 
		0x51e0, 
		0xd2a0, 
		0x4dc0, 
		0x0000, 
		0x3ea5, 
		0xfcff, 
		0xcb6d, 
		0xd2e0, 
		0xb788, 
		0xd100, 
		0xe923, 
		0xc200, 
		0xd200, 
		0x52e0, 
		0x0000, 
		0x50a0, 
		0x9c89, 
		0xf749, 
		0xd0a0, 
		0xe614, 
		0x4400, 
		0xf9da, 
		0xd2e0, 
		0xd240, 
		0x64f4, 
		0x4800, 
		0xc900, 
		0x4cc0, 
		0xb970, 
		0xfd07, 
		0x9007, 
		0xcf40, 
		0xa573, 
		0x4fc0, 
		0xcb80, 
		0x5120, 
		0x7ba5, 
		0xc500, 
		0xd3a0, 
		0x4a00, 
		0x4e00, 
		0xd9a5, 
		0x4e40, 
		0xd0c0, 
		0xdd64, 
		0xd2a0, 
		0xcdc0, 
		0x5180, 
		0xfcd8, 
		0x7c8d, 
		0x5000, 
		0x6977, 
		0x5360, 
		0xcec0, 
		0xc880, 
		0x832d, 
		0x4d00, 
		0xc900, 
		0xca80, 
		0x525f, 
		0x4e40, 
		0xd3c0, 
		0x1d74, 
		0x4582, 
		0x4f80, 
		0x4b80, 
		0x4d00, 
		0xd220, 
		0x7c00, 
		0x4600, 
		0x51a0, 
		0xd260, 
		0x6b9c, 
		0x8fd6, 
		0xdce6, 
		0x3a24, 
		0xd200, 
		0x6393, 
		0x4568, 
		0x977e, 
		0x185c, 
		0x4200, 
		0xc980, 
		0x5280, 
		0x166b, 
		0xf2bd, 
		0xd3c0, 
		0x51e0, 
		0x4900, 
		0xfc00, 
		0xccb5, 
		0x5604, 
		0xaa9c, 
		0x7dac, 
		0xc088, 
		0xcd80, 
		0x52a0, 
		0xd000, 
		0x5060, 
		0x0000, 
		0x4200, 
		0xc700, 
		0x0e8d, 
		0x54c2, 
		0xd1e0, 
		0x80f4, 
		0xd380, 
		0xfc00, 
		0x4b80, 
		0xd160, 
		0x8000, 
		0xffdf, 
		0xce40, 
		0x1116, 
		0x4e40, 
		0xcc80, 
		0x8000, 
		0x3480, 
		0x0000, 
		0x9278, 
		0x4fc0, 
		0xad85, 
		0x51e0, 
		0xbc04, 
		0x4ef4, 
		0x4f00, 
		0x5202, 
		0xa410, 
		0xc600, 
		0x0000, 
		0x8dff, 
		0xd2a0, 
		0xce00, 
		0xd260, 
		0x51a0, 
		0xcd80, 
		0x519d, 
		0x4f3a, 
		0x94a9, 
		0xd3a0, 
		0x5120, 
		0xbd52, 
		0x5180, 
		0xd320, 
		0xd220, 
		0x4980, 
		0xcf40, 
		0x50e0, 
		0xa582, 
		0xd280, 
		0x55cb, 
		0xd220, 
		0xc200, 
		0x50e0, 
		0x4980, 
		0xea26, 
		0xc1c0, 
		0x9df1, 
		0x3d8e, 
		0xc880, 
		0x0955, 
		0xcc00, 
		0x9eb8, 
		0x53c0, 
		0xc200, 
		0xce23, 
		0xfc00, 
		0x3e48, 
		0x5140, 
		0x8000, 
		0x4a80, 
		0xd320, 
		0x0000, 
		0xd1e0, 
		0xce40, 
		0x0918, 
		0x7c00, 
		0x4b00, 
		0x4b80, 
		0xc900, 
		0xd1e0, 
		0xb98a, 
		0x4500, 
		0x5320, 
		0xcc00, 
		0x81ba, 
		0x51e0, 
		0x4200, 
		0x4880, 
		0xcd80, 
		0xc2e8, 
		0x5220, 
		0x4000, 
		0x43bf, 
		0xcb00, 
		0x6e02, 
		0x4e00, 
		0x5000, 
		0xbce9, 
		0x52a0, 
		0x50cb, 
		0xd360, 
		0x4b00, 
		0x3580, 
		0xdbb1, 
		0x8000, 
		0x7cbd, 
		0x4fc0, 
		0xc700, 
		0xdba7, 
		0xd3da, 
		0x8000, 
		0x1884, 
		0x5260, 
		0xa1ba, 
		0x1c4e, 
		0xfc00, 
		0x0000, 
		0xd0c0, 
		0x4dc0, 
		0x4900, 
		0xd0c0, 
		0x210b, 
		0xc880, 
		0xdd02, 
		0x53e0, 
		0xca00, 
		0x0fb2, 
		0xc980, 
		0x4c00, 
		0xcfc0, 
		0xce00, 
		0xcdc0, 
		0xd300, 
		0xcc40, 
		0xb218, 
		0xc700, 
		0x8beb, 
		0x2e7c, 
		0x50e0, 
		0x4400, 
		0x7acb, 
		0xc980, 
		0xcdc0, 
		0x52c0, 
		0x4a00, 
		0x0198, 
		0x4e40, 
		0xd0e0, 
		0x66b1, 
		0x5160, 
		0x4900, 
		0xccc0, 
		0x4e80, 
		0xc800, 
		0xd0c0, 
		0x3c00, 
		0xd100, 
		0x06d7, 
		0x5360, 
		0xce80, 
		0xd3e0, 
		0x52a0, 
		0xfaf9, 
		0xdcac, 
		0x4ec0, 
		0x4b80, 
		0x12d8, 
		0xe489, 
		0xd020, 
		0x2cff, 
		0x8e8b, 
		0x7c81, 
		0xd304, 
		0x5200, 
		0x0000, 
		0xcb00, 
		0xfc00, 
		0x52e0, 
		0xc853, 
		0xe063, 
		0xe615, 
		0x5220, 
		0x0000, 
		0xcc00, 
		0x4880, 
		0xfaa7, 
		0xe6a3, 
		0xc6f5, 
		0x4e80, 
		0xe160, 
		0xd040, 
		0xccc0, 
		0x80c0, 
		0xc600, 
		0x7c00, 
		0x4a80, 
		0x5120, 
		0x5320, 
		0xcb00, 
		0x203f, 
		0xf8ea, 
		0x2f8e, 
		0xcc80, 
		0x349d, 
		0x5220, 
		0xbe5e, 
		0x9280, 
		0x4b00, 
		0xf4d9, 
		0xca80, 
		0x4c40, 
		0xd320, 
		0x7057, 
		0xd180, 
		0xa36a, 
		0x0000, 
		0xcabd, 
		0xabec, 
		0xdbab, 
		0x18fd, 
		0x4f00, 
		0x0654, 
		0x132d, 
		0x52e0, 
		0xd0e0, 
		0x182d, 
		0xce00, 
		0x4400, 
		0x3beb, 
		0xd2c0, 
		0x5060, 
		0xc900, 
		0xcd80, 
		0xbdd5, 
		0x8000, 
		0xd040, 
		0x7dad, 
		0xd280, 
		0x5070, 
		0xd854, 
		0xbc00, 
		0x01c9, 
		0x3985, 
		0x8000, 
		0x182f, 
		0x0cca, 
		0xb944, 
		0x5c42, 
		0x9f81, 
		0x5320, 
		0x4f80, 
		0x4f80, 
		0x79c7, 
		0x4f00, 
		0x8000, 
		0x51a0, 
		0xd020, 
		0xcf80, 
		0xcf00, 
		0xd3a0, 
		0x5140, 
		0x35c3, 
		0x5320, 
		0x8000, 
		0xef84, 
		0xd320, 
		0x8000, 
		0xd120, 
		0x4f00, 
		0xd1c0, 
		0x7c00, 
		0xcc40, 
		0xca80, 
		0x071d, 
		0xb7ec, 
		0xcc40, 
		0xab52, 
		0xc600, 
		0xc700, 
		0xc980, 
		0xb49f, 
		0xd300, 
		0x5120, 
		0x874f, 
		0x7c52, 
		0xcc40, 
		0xcd80, 
		0xfc00, 
		0xfc00, 
		0x4a00, 
		0x3c6d, 
		0x8000, 
		0x25aa, 
		0x43dc, 
		0xce81, 
		0x5320, 
		0x1f36, 
		0xd0e0, 
		0x0000, 
		0x3c00, 
		0x98eb, 
		0xd020, 
		0x8000, 
		0x5060, 
		0x5180, 
		0xadaf, 
		0xd200, 
		0xc800, 
		0x4700, 
		0xccc0, 
		0x8c6a, 
		0x4700, 
		0xcc80, 
		0x5100, 
		0xd0c0, 
		0xfd98, 
		0x5240, 
		0xa338, 
		0x8000, 
		0x8000, 
		0x2e54, 
		0xd140, 
		0xfc00, 
		0x5240, 
		0xb5f3, 
		0x0000, 
		0xcd00, 
		0xd3c0, 
		0x51a0, 
		0xbbf9, 
		0x4fc0, 
		0xd100, 
		0x4800, 
		0xb633, 
		0xd1c0, 
		0x4b00, 
		0xd2c0, 
		0x51a0, 
		0xd2a0, 
		0x81bc, 
		0x4d00, 
		0xe4d2, 
		0x7c00, 
		0xfc00, 
		0x5200, 
		0x5120, 
		0xd2e0, 
		0x06a2, 
		0xded1, 
		0x0000, 
		0x9df5, 
		0x5280, 
		0xfc00, 
		0x0000, 
		0x7c00, 
		0xd300, 
		0xa148, 
		0xb1b3, 
		0x4c00, 
		0xe97d, 
		0xc500, 
		0xce00, 
		0x4f40, 
		0xd260, 
		0x4e33, 
		0x4700, 
		0x4dc0, 
		0x2015, 
		0xdead, 
		0xd3c0, 
		0xf479, 
		0xf93b, 
		0xd140, 
		0x50e0, 
		0xd100, 
		0xd1c0, 
		0x5380, 
		0xd320, 
		0x5200, 
		0xca80, 
		0xeffe, 
		0xd160  
};
volatile uint32_t rand_ieee754_32[1536] __attribute__ ((aligned (2048))) = {
		0xff800000, 
		0x00800000, 
		0x0e0003ff, 
		0x40400000, 
		0x80000000, 
		0x3af06381, 
		0x00800000, 
		0xcb95bdb2, 
		0x80000000, 
		0x7f7ffffe, 
		0xc0f4682b, 
		0x0c7fffe0, 
		0x00800000, 
		0x0c7c0000, 
		0x00000000, 
		0x7fc00000, 
		0x0e003fff, 
		0xa173ba49, 
		0xff93c276, 
		0xc1500000, 
		0x40c00000, 
		0x42200000, 
		0x0e000007, 
		0x80800003, 
		0x80000000, 
		0xc1a00000, 
		0x80000000, 
		0x0e7fffff, 
		0x18db224d, 
		0x0c7c0000, 
		0x41100000, 
		0x80000000, 
		0x921c7801, 
		0xff800000, 
		0x4e14e88e, 
		0x1a78a2b7, 
		0xbc21949a, 
		0x00011111, 
		0x0e7fffff, 
		0x11a1afa7, 
		0xc1200000, 
		0x00000400, 
		0x0c700000, 
		0x425c0000, 
		0x3c4ff631, 
		0xc2300000, 
		0x64c846e1, 
		0x40800000, 
		0xc2700000, 
		0x80200000, 
		0x7f800000, 
		0x80000001, 
		0xc2140000, 
		0x00800001, 
		0x78602b23, 
		0x6d7747f5, 
		0x40e00000, 
		0x006eb1bb, 
		0x304395be, 
		0x0e03ffff, 
		0xc1700000, 
		0x40000000, 
		0x421c0000, 
		0x41f00000, 
		0xe7c9b043, 
		0x2ed7a0cf, 
		0x00000000, 
		0xfba19831, 
		0xd785832c, 
		0x40800000, 
		0x00008000, 
		0xac33c192, 
		0x0e0001ff, 
		0x51a308ef, 
		0x0c400000, 
		0x80000000, 
		0x80000000, 
		0x7f800000, 
		0xe1408e33, 
		0x00000000, 
		0xaaaaaaaa, 
		0x00800003, 
		0x80000001, 
		0x42340000, 
		0x12264616, 
		0x00000008, 
		0x8f7ffffd, 
		0x7fc00000, 
		0x0e00001f, 
		0x427c0000, 
		0xc1600000, 
		0x80000004, 
		0x41b00000, 
		0x00000002, 
		0x3fa8e03c, 
		0xc1800000, 
		0x8e6e32a9, 
		0xc1980000, 
		0x807fffff, 
		0x0e000007, 
		0x80001000, 
		0x6209eaaa, 
		0x9babcd77, 
		0x0e00ffff, 
		0x7f7fffff, 
		0x0e000fff, 
		0xa2a95c5e, 
		0x0e000007, 
		0x8f7ffffd, 
		0x42040000, 
		0x7f7fffff, 
		0x7fc00001, 
		0x41e00000, 
		0x004947b2, 
		0xc33d2092, 
		0x00000008, 
		0xc1e00000, 
		0x5c241dfa, 
		0xc1c80000, 
		0xc0c00000, 
		0x423c0000, 
		0xc2700000, 
		0xe7efa781, 
		0xcf22c9ae, 
		0x8f7ffffd, 
		0xc0fc7aec, 
		0xf95ad6f0, 
		0x80080000, 
		0x80200000, 
		0x00200000, 
		0xe8af2802, 
		0xad56c2b8, 
		0x42180000, 
		0xff7fffff, 
		0xc1b00000, 
		0xff7fffff, 
		0xc2600000, 
		0xc1a80000, 
		0x75c791c8, 
		0xaaaaaaaa, 
		0xa8440d3d, 
		0x80000100, 
		0x80000000, 
		0x80800001, 
		0xc2580000, 
		0x41000000, 
		0x00000002, 
		0x7fffffff, 
		0x007ffffe, 
		0x9b067dce, 
		0x9ae9ea53, 
		0x9fcf2e13, 
		0x01843553, 
		0x8b5c85a9, 
		0x00000100, 
		0xc2380000, 
		0xdb9fdd36, 
		0x80000400, 
		0x42680000, 
		0xaaaaaaaa, 
		0xc1a80000, 
		0x993efe59, 
		0xc2700000, 
		0xffc00000, 
		0x228ca9f3, 
		0xc2680000, 
		0x3d4b8e7c, 
		0x0e0007ff, 
		0xdc58c555, 
		0xff800000, 
		0x743b2699, 
		0xff7ffffe, 
		0x00000000, 
		0x0c7fffff, 
		0x8aec2910, 
		0x80000000, 
		0xc2280000, 
		0x2161ae82, 
		0x2f933d8f, 
		0x7f800000, 
		0x8eeb0441, 
		0x00000002, 
		0x41c80000, 
		0x4b8c4b40, 
		0x0c7ffff0, 
		0xb71b8156, 
		0xa443badc, 
		0x42480000, 
		0x50d2dd7f, 
		0xc26c0000, 
		0x0c7fffe0, 
		0x7a15d99e, 
		0xff800000, 
		0x41c00000, 
		0x80000010, 
		0xc0000000, 
		0x807fffff, 
		0xc1f00000, 
		0x0e00003f, 
		0x7fc00001, 
		0x00000001, 
		0x282b5fbc, 
		0x0c7ffffe, 
		0x80000080, 
		0x42200000, 
		0xffbfffff, 
		0xff7fffff, 
		0xffc00001, 
		0x67930cbe, 
		0x00000010, 
		0x00800000, 
		0x0c7fffc0, 
		0x0c7fff00, 
		0xb594659e, 
		0xc2000000, 
		0x9aff8092, 
		0x39eaebcb, 
		0x00040000, 
		0x685b3cca, 
		0xc3d11049, 
		0x42340000, 
		0x80080000, 
		0x4b17de40, 
		0xcb8c4b40, 
		0x662199a2, 
		0x0a65ac48, 
		0x80000000, 
		0x00000400, 
		0xff800000, 
		0x80000001, 
		0xc1f80000, 
		0x00000010, 
		0x4de93e33, 
		0x807c0eeb, 
		0x055570a7, 
		0xc0e00000, 
		0x62397072, 
		0x00333256, 
		0x00000001, 
		0xe2a068b4, 
		0x9fefa99b, 
		0x80001000, 
		0x0e000001, 
		0x0058307b, 
		0x42240000, 
		0xff000000, 
		0x0213371f, 
		0x41600000, 
		0x223fbbdf, 
		0x421c0000, 
		0x00004000, 
		0x410d7127, 
		0x42780000, 
		0x80000008, 
		0xc2040000, 
		0x44f33a96, 
		0x00000000, 
		0x7f800000, 
		0xbf800001, 
		0x80000004, 
		0x59a3c0a6, 
		0xc2040000, 
		0x708fe2ee, 
		0xff800000, 
		0x42480000, 
		0x1433b2bc, 
		0x80008000, 
		0xff800000, 
		0xe6cf05aa, 
		0x41800000, 
		0x007ffffe, 
		0x80000000, 
		0x1d18aa7a, 
		0x80000004, 
		0xc0000000, 
		0xc0400000, 
		0x007ffffe, 
		0x404feec6, 
		0xff800000, 
		0x7f800000, 
		0x0018ad3e, 
		0xd50e9abd, 
		0xff7fffff, 
		0x00000002, 
		0x41d80000, 
		0xffc00001, 
		0x0c7e0000, 
		0x4bbd0335, 
		0x00040000, 
		0x0f1ee088, 
		0xc1f00000, 
		0x00000001, 
		0x9da6d3bf, 
		0x805cd3e5, 
		0xc25c0000, 
		0x98bcd34e, 
		0x15cd062c, 
		0x0c7fffff, 
		0x3f800001, 
		0x11bf9941, 
		0x08e46535, 
		0x80000000, 
		0x3f800001, 
		0xff7fffff, 
		0x3bc26dba, 
		0xd965a165, 
		0x7f800000, 
		0x00000000, 
		0x5543ee8c, 
		0x80002000, 
		0x0c7ff800, 
		0xc2100000, 
		0xc2680000, 
		0xcccccccc, 
		0xffbfffff, 
		0x41d00000, 
		0x7f7ffffe, 
		0x00800002, 
		0x0e00000f, 
		0x00400000, 
		0x00000200, 
		0xece3428d, 
		0xc24c0000, 
		0x0e0001ff, 
		0x0c7ffff8, 
		0xc2a4572c, 
		0xd446ea1a, 
		0xc1b80000, 
		0xc1c00000, 
		0x5c65aef1, 
		0x00800002, 
		0x7f7ffffe, 
		0x814c09bb, 
		0x7ae5b161, 
		0x7f800000, 
		0xff800000, 
		0x80800003, 
		0xe8215ff0, 
		0x7f800000, 
		0x42580000, 
		0xc0c00000, 
		0x41980000, 
		0xc1c80000, 
		0x0c7ff800, 
		0x80000020, 
		0xc1700000, 
		0x00000040, 
		0xbf4567ba, 
		0xff800000, 
		0x00100000, 
		0x41d00000, 
		0xe72411a9, 
		0xc2780000, 
		0xff7ffffe, 
		0xc1400000, 
		0x13273e95, 
		0x3f800001, 
		0x80000000, 
		0x55555555, 
		0xffc00001, 
		0x42540000, 
		0x1860146d, 
		0x0e0003ff, 
		0x41e00000, 
		0x00008000, 
		0x0c7e0000, 
		0xf9b0e44a, 
		0x00000000, 
		0xc1100000, 
		0xc1d80000, 
		0x424c0000, 
		0x91139ec2, 
		0x42680000, 
		0xf28f3bdb, 
		0x93741018, 
		0x0c7ffff0, 
		0x0f7fffff, 
		0xff800000, 
		0xc1b00000, 
		0x80000040, 
		0x00000000, 
		0x7fa688d7, 
		0x33bf4760, 
		0xc27c0000, 
		0xc2580000, 
		0x7fffffff, 
		0x00400000, 
		0x7f800000, 
		0x7fc00000, 
		0x00000000, 
		0x42300000, 
		0x80000000, 
		0x00004000, 
		0xc2380000, 
		0x80800003, 
		0x00800000, 
		0x7f924ab7, 
		0xef7480e4, 
		0x355d7fb2, 
		0x0c7c0000, 
		0x42000000, 
		0xc1500000, 
		0x0e00003f, 
		0x74bbc764, 
		0x80800003, 
		0xc1a00000, 
		0xff800000, 
		0x0c7e0000, 
		0x00800000, 
		0x00008000, 
		0x8047b439, 
		0x41c80000, 
		0xd2adce13, 
		0x97ab62af, 
		0x0e1fffff, 
		0xa6164f76, 
		0x1446135a, 
		0x41600000, 
		0x0e0001ff, 
		0x7f800000, 
		0x80000400, 
		0x0f7fffff, 
		0xf9b6c4de, 
		0x7fc00001, 
		0xff800000, 
		0x80000000, 
		0x2174d37f, 
		0xc2040000, 
		0x8bb167bf, 
		0x02a84477, 
		0x40000000, 
		0xe5fa1c15, 
		0x00400000, 
		0x00000080, 
		0x80000000, 
		0x00002000, 
		0x80ffffff, 
		0x1d5662e0, 
		0xd53c28da, 
		0x9b67a108, 
		0x0c7ffe00, 
		0x80000040, 
		0x00000800, 
		0xff800000, 
		0xc2780000, 
		0x25919893, 
		0x7fc00001, 
		0x0e000fff, 
		0x00000000, 
		0xc1880000, 
		0x0c7f8000, 
		0x6003e5b2, 
		0x00000004, 
		0x7f951b21, 
		0x00000010, 
		0x4f3f7d6e, 
		0x80000100, 
		0x00800001, 
		0x00800000, 
		0x7fc00000, 
		0x8f7ffffc, 
		0xc6bce01f, 
		0x41300000, 
		0x00040000, 
		0xb08d6896, 
		0xffa1dc50, 
		0x7f8e39f7, 
		0x80000000, 
		0xac7d1653, 
		0xc2040000, 
		0x80011111, 
		0x0c7fffc0, 
		0xfb3c766a, 
		0x41b80000, 
		0x4abfbd22, 
		0x8041298d, 
		0x211d3dd8, 
		0xc1400000, 
		0x87015866, 
		0xc1000000, 
		0x0051d442, 
		0x3f800000, 
		0x3d7304fa, 
		0xc2640000, 
		0x80261131, 
		0x80000000, 
		0x80000000, 
		0x0c7fff00, 
		0xf1d26a85, 
		0xc1500000, 
		0xc2440000, 
		0x00000100, 
		0xd89017ad, 
		0x41f80000, 
		0x0c7e0000, 
		0x00800000, 
		0xb388ae2c, 
		0xff86639a, 
		0x90694378, 
		0x4b000000, 
		0x421c0000, 
		0xff7ffffe, 
		0x0e03ffff, 
		0x80000000, 
		0x80000020, 
		0xbf800000, 
		0xc2040000, 
		0x41c80000, 
		0xda8bae41, 
		0xc1100000, 
		0x7f85df55, 
		0x80000001, 
		0x7fffffff, 
		0x00080000, 
		0x0bba63a6, 
		0x5d7f45e9, 
		0x80001000, 
		0xc0400000, 
		0x79cfbf0b, 
		0x7147a836, 
		0xc1a00000, 
		0xbafaf37d, 
		0xc1d00000, 
		0x560dec97, 
		0xfffc3a02, 
		0xae54a337, 
		0x00000002, 
		0x76a0c3a2, 
		0x71d0e88c, 
		0xc2440000, 
		0x0c7ffe00, 
		0x0e0003ff, 
		0xc1e00000, 
		0x00000200, 
		0x007fffff, 
		0x0c700000, 
		0x9585eeca, 
		0x0c7fffe0, 
		0x346398c3, 
		0x00080000, 
		0x00000000, 
		0x80000020, 
		0x42240000, 
		0xffd55661, 
		0x0c7fc000, 
		0x80000000, 
		0x00000080, 
		0x80000010, 
		0x0016665f, 
		0x0e3fffff, 
		0xc2100000, 
		0xc2280000, 
		0x80011111, 
		0x42780000, 
		0x0e0001ff, 
		0x80020000, 
		0xc1b80000, 
		0x00000000, 
		0x00000000, 
		0xc23c0000, 
		0x42080000, 
		0x42180000, 
		0xffffffff, 
		0x40c00000, 
		0x80000000, 
		0x00400000, 
		0x7f800000, 
		0x00800000, 
		0x00800000, 
		0xc2440000, 
		0x527b768f, 
		0xc1880000, 
		0x1f64d03a, 
		0xd2e6e63f, 
		0x7f9b2e2b, 
		0xc1000000, 
		0xff000000, 
		0x0e000007, 
		0xc2380000, 
		0xc2440000, 
		0x014b3ce1, 
		0xff800000, 
		0xff841a4e, 
		0x00100000, 
		0xc1f80000, 
		0x0e007fff, 
		0x7f999b7d, 
		0x2513e1b6, 
		0xff891745, 
		0x41e10ef9, 
		0x42480000, 
		0x25d679f9, 
		0x00475127, 
		0x292178ce, 
		0x0c600000, 
		0x74b5039f, 
		0xc0000000, 
		0x00000000, 
		0x31a3789f, 
		0x4a949af9, 
		0x689bb31f, 
		0xbd64f6d6, 
		0x1ab88066, 
		0xff7ffffe, 
		0x0d00fff0, 
		0x7f800000, 
		0x9cfef104, 
		0xc1b80000, 
		0x24114a54, 
		0x80000010, 
		0x80800001, 
		0x80000040, 
		0x007ffffe, 
		0x6c0614f4, 
		0x4ace667a, 
		0x421c0000, 
		0x00000001, 
		0xff7ffffe, 
		0x41880000, 
		0x00000000, 
		0x80000020, 
		0x38285dd2, 
		0xcbc5c331, 
		0xc0800000, 
		0x00400000, 
		0xf0dcefa5, 
		0xecadd955, 
		0x0f7ffffd, 
		0x00000800, 
		0x0889a6a1, 
		0xc0799500, 
		0x0c1285d3, 
		0x42480000, 
		0xff800000, 
		0xc40fff69, 
		0x80100000, 
		0x142e0316, 
		0xd178baaf, 
		0x80000000, 
		0x0e0000ff, 
		0x80000002, 
		0x7ef3bc77, 
		0x80000010, 
		0x891ff3e2, 
		0x00000002, 
		0x0e0000ff, 
		0x00800001, 
		0xdcd70f7e, 
		0x0e000001, 
		0x126a04e1, 
		0x420c0000, 
		0x08636a50, 
		0xe78b0393, 
		0x00000200, 
		0x28867943, 
		0x0c7e0000, 
		0x00000000, 
		0xffe127d8, 
		0x42400000, 
		0x0e000fff, 
		0x42240000, 
		0x3f800001, 
		0x57eaac86, 
		0x33ca5f12, 
		0x42740000, 
		0xc2780000, 
		0xc0c00000, 
		0x4b8c4b40, 
		0x16004831, 
		0x92be1ba2, 
		0x8f7ffffd, 
		0xa11729d7, 
		0x007fffff, 
		0x41600000, 
		0xdded7c08, 
		0x0c7fff80, 
		0x80000800, 
		0x7f800000, 
		0x00000000, 
		0x7fc00000, 
		0x6b1995f0, 
		0x7f800000, 
		0x8502aeb8, 
		0xff949c57, 
		0xffc00000, 
		0xc2440000, 
		0xc1900000, 
		0x00000010, 
		0x00002000, 
		0x0a12db74, 
		0xc2100000, 
		0x00000002, 
		0x7fb69a27, 
		0x00000000, 
		0x80080000, 
		0x0c7fffc0, 
		0x8f7fffff, 
		0xc2300000, 
		0x80000100, 
		0xa357e6e5, 
		0x42380000, 
		0x80000000, 
		0x7c7e7742, 
		0xc1b00000, 
		0x42040000, 
		0x0e01ffff, 
		0x268de928, 
		0xc1900000, 
		0x7978c12c, 
		0xc1b00000, 
		0x42240000, 
		0x7fc00001, 
		0x42000000, 
		0xa98638b0, 
		0x0e7fffff, 
		0x80080000, 
		0xffb1a88a, 
		0x7f7fffff, 
		0xff800000, 
		0x4ee8670c, 
		0x0c7ff000, 
		0x41200000, 
		0x00000400, 
		0xbbe11b41, 
		0xe7f29680, 
		0x0e00003f, 
		0xc0a00000, 
		0x9213878c, 
		0xcb000000, 
		0x0e0003ff, 
		0x20ae4cec, 
		0x0f7ffffc, 
		0x1d0da661, 
		0xb64c6411, 
		0x3dedb02c, 
		0x7f800000, 
		0xf8bdb80c, 
		0x42540000, 
		0xc3c2f090, 
		0x4b8c4b40, 
		0x28d7bfc9, 
		0x007ffffe, 
		0x5c137090, 
		0x00000004, 
		0xc2780000, 
		0x7f800000, 
		0x8eea194a, 
		0x42400000, 
		0x7f800000, 
		0x9bd12287, 
		0x0d000ff0, 
		0xc1c00000, 
		0xdad5c7db, 
		0x45247954, 
		0xf9d039e0, 
		0x6ea4d026, 
		0x5d7e38ec, 
		0x807fffff, 
		0x00000800, 
		0x00400000, 
		0xc77a2e07, 
		0xffb98ca7, 
		0xb9fd62f9, 
		0x0f7ffffc, 
		0x0c7fe000, 
		0x41c80000, 
		0x5c6007c6, 
		0x98a804b8, 
		0x0c7fffff, 
		0x80000000, 
		0x864b9f29, 
		0x0c400000, 
		0x5e15c6f1, 
		0x80020000, 
		0x628a2687, 
		0x0c600000, 
		0x09549d98, 
		0x41800000, 
		0x80000000, 
		0x0c7e0000, 
		0xc2600000, 
		0x00008000, 
		0xff800000, 
		0x80800000, 
		0x80000000, 
		0x004fc6d8, 
		0xff800000, 
		0x00000020, 
		0x40bf17ac, 
		0x00100000, 
		0xc1a00000, 
		0xf97479e3, 
		0xff800000, 
		0x80800000, 
		0x99e92e98, 
		0xd256d503, 
		0xa1ce67ff, 
		0x80000000, 
		0x00020000, 
		0x7f800000, 
		0xff7ffffe, 
		0x0f7fffff, 
		0x80004000, 
		0xd543490e, 
		0xff7fffff, 
		0xa920f75b, 
		0xa999609f, 
		0xc0800000, 
		0xc2180000, 
		0xc1d60da5, 
		0x0e7fffff, 
		0xff7ffffe, 
		0x70f4777b, 
		0x42300000, 
		0x80040000, 
		0xc2680000, 
		0xc1800000, 
		0x4b0b8a7c, 
		0xcb1984a5, 
		0x00000100, 
		0x0d9f5c2f, 
		0x80800000, 
		0x00000001, 
		0x0c7fffe0, 
		0x00000000, 
		0x0c700000, 
		0x0c7fffff, 
		0x007ffffe, 
		0x678eb46e, 
		0xc1200000, 
		0x8d5c0ec7, 
		0xb3aaae45, 
		0xff800001, 
		0x41a80000, 
		0x89e4afd8, 
		0x470a8a41, 
		0xe11a280a, 
		0x80800001, 
		0x55769bcc, 
		0x80800000, 
		0xa3e3beea, 
		0x80800003, 
		0x7e101052, 
		0x7f800000, 
		0xc2440000, 
		0x0c7fffc0, 
		0x3f800000, 
		0x6f692785, 
		0xb9e5523c, 
		0x80000000, 
		0x33333333, 
		0x934e0b2e, 
		0x00800003, 
		0x6308167d, 
		0xc1a00000, 
		0x0e000fff, 
		0x80800000, 
		0x41880000, 
		0x80000400, 
		0x41e80000, 
		0x80004000, 
		0x6bb49ffd, 
		0xa7f00ddf, 
		0x0c7fff80, 
		0x6ce43612, 
		0xd5565b4a, 
		0x7fbfffff, 
		0xc1f00000, 
		0x7507efec, 
		0x425c0000, 
		0xc1b00000, 
		0x80000000, 
		0xc0c6b6c7, 
		0xc1b80000, 
		0xffc00000, 
		0x7f800000, 
		0xc0800000, 
		0x00000002, 
		0x0e7fffff, 
		0x80010000, 
		0xc2780000, 
		0x7f800000, 
		0xc26c0000, 
		0x80800001, 
		0xff800000, 
		0xc21c0000, 
		0x8687430a, 
		0x7f800000, 
		0x6c7fb1c6, 
		0xc1f80000, 
		0x00000020, 
		0x0f7fffff, 
		0x3262c88b, 
		0x41800000, 
		0xcc8ee2e4, 
		0xd5aa580a, 
		0xc1c80000, 
		0x7f800000, 
		0x7f8a02a4, 
		0xd5e929e0, 
		0x40000000, 
		0x426c0000, 
		0x9be52a6f, 
		0x00000008, 
		0x7f800000, 
		0x80000200, 
		0x80000001, 
		0xbeada53d, 
		0xc0800000, 
		0x41980000, 
		0x6d52a68f, 
		0x0c7fff00, 
		0x0e7fffff, 
		0x8f7ffffd, 
		0x00000000, 
		0x7f800000, 
		0xc1f80000, 
		0xffbc730a, 
		0x8f7fffff, 
		0x7f800000, 
		0x0d000ff0, 
		0x7f8550cf, 
		0xc21c0000, 
		0x7fc00000, 
		0x00000010, 
		0x00800003, 
		0x2acaad0f, 
		0x80800000, 
		0x355e11d2, 
		0xc2400000, 
		0x7f800000, 
		0x96783b06, 
		0x0c7fffff, 
		0xe7ac8fef, 
		0x00200000, 
		0x42640000, 
		0x7f7ffffe, 
		0x3f028f5c, 
		0x8dac5dcd, 
		0x42540000, 
		0xc1980000, 
		0x00000080, 
		0x24be967c, 
		0xcecb6fd9, 
		0x41f80000, 
		0x0e000fff, 
		0x80800000, 
		0x179aa651, 
		0x80800000, 
		0x80000008, 
		0xecc6d645, 
		0x0c7ffff0, 
		0x80ffffff, 
		0xc756f1d3, 
		0x45b9e1dd, 
		0xefd0ac5b, 
		0xc1f00000, 
		0xff000000, 
		0x24bc0774, 
		0x0c7ffff8, 
		0x7f800000, 
		0x41d80000, 
		0x56ca7789, 
		0x41d00000, 
		0x80000008, 
		0x5875b7b1, 
		0x00000000, 
		0x80008000, 
		0x00020000, 
		0x6c928699, 
		0x0c7ff800, 
		0xc0400000, 
		0xf03836e6, 
		0xc2000000, 
		0x42440000, 
		0x880a89c8, 
		0x0e003fff, 
		0x41980000, 
		0x4571562c, 
		0xffc00001, 
		0xc1400000, 
		0xc2480000, 
		0xc2380000, 
		0x0c7f8000, 
		0x80040000, 
		0x80020000, 
		0x42080000, 
		0x331da002, 
		0xff7fffff, 
		0x00800000, 
		0xffbb66e8, 
		0x80000000, 
		0xfabc080f, 
		0x569095d4, 
		0x680c8e71, 
		0x54181e18, 
		0x7cf597be, 
		0xff800000, 
		0x8f7ffffe, 
		0x24ad1394, 
		0x0c7c0000, 
		0x007ffffe, 
		0xd6b4366b, 
		0x807ffffe, 
		0x2cb49afc, 
		0x1c8d4b08, 
		0x00011111, 
		0x80000040, 
		0xb0428fad, 
		0x80000001, 
		0x807fffff, 
		0x758189d8, 
		0xcb000000, 
		0x00000002, 
		0x00000002, 
		0xff800000, 
		0x0e001fff, 
		0x00020000, 
		0x2455d250, 
		0x41d80000, 
		0x0c780000, 
		0x00000000, 
		0x376730a9, 
		0x0c7c0000, 
		0x0f7fffff, 
		0x780c028f, 
		0x80000000, 
		0x71111c64, 
		0x8248d48e, 
		0x00200000, 
		0x46627f3c, 
		0x00100000, 
		0x80004000, 
		0x00800001, 
		0x9be7b3ef, 
		0x80001000, 
		0x0d000ff0, 
		0x7fbfffff, 
		0x00000080, 
		0x424c0000, 
		0xffbfffff, 
		0x0e00007f, 
		0x30b53345, 
		0x7f7ffffe, 
		0x00800001, 
		0x42200000, 
		0x391fec8d, 
		0x80000040, 
		0x0c7fff80, 
		0x3f800001, 
		0x2383948a, 
		0x72b13102, 
		0x00000000, 
		0x00000000, 
		0xff800000, 
		0x00000000, 
		0x8f7ffffc, 
		0x530b6f26, 
		0x80000000, 
		0x42600000, 
		0x000aab72, 
		0x007ffffe, 
		0x0c7fe000, 
		0x80800003, 
		0xc2480000, 
		0x4d1fd0a5, 
		0xff9e7d5b, 
		0x41200000, 
		0x4fb28351, 
		0x80800002, 
		0xb7e52efb, 
		0x6ccb74bd, 
		0x2dd1f042, 
		0xb87ba9d0, 
		0x7f800000, 
		0x00001000, 
		0xcf500875, 
		0x7e230565, 
		0xc1200000, 
		0xc2740000, 
		0x7f7ffffe, 
		0x0c7ff000, 
		0xc1c80000, 
		0x00ffffff, 
		0x424c0000, 
		0x0c7fff00, 
		0xc25c0000, 
		0x36e25152, 
		0x80000001, 
		0xc1400000, 
		0xff7ffffe, 
		0xce968983, 
		0x425c0000, 
		0xa576a71c, 
		0x427c0000, 
		0xffbfffff, 
		0x00000040, 
		0x80000000, 
		0x0c7f0000, 
		0x80767586, 
		0xff9ab51f, 
		0x00000200, 
		0x0c7ffff8, 
		0x40c00000, 
		0x51b7e4e4, 
		0x42280000, 
		0x80020000, 
		0x0c7f8000, 
		0x80800000, 
		0x83fb2f81, 
		0x0e000fff, 
		0x40a00000, 
		0x40a00000, 
		0xc2640000, 
		0x541b600c, 
		0xff615b4b, 
		0xc1a00000, 
		0x41900000, 
		0xd0e524aa, 
		0x80000001, 
		0xe26ceceb, 
		0x0e00003f, 
		0x765ec15b, 
		0x08479947, 
		0x0a57ebbd, 
		0xff7ffffe, 
		0x422c0000, 
		0x294fe9e5, 
		0x00000001, 
		0x7f800000, 
		0x6b4002c3, 
		0x00000000, 
		0x52c5b809, 
		0x7fc00001, 
		0x42680000, 
		0x03453fec, 
		0xc25c0000, 
		0x0c7ffff0, 
		0x28e2c78b, 
		0x00010000, 
		0x42700000, 
		0x9e387ed9, 
		0xb6f88f88, 
		0x7f822770, 
		0xc9d7ff0e, 
		0x294d27a7, 
		0x80000002, 
		0x00000002, 
		0x6407d70b, 
		0x4b000000, 
		0x41300000, 
		0x72e33589, 
		0xad8364c1, 
		0x8b15e7bb, 
		0x80040000, 
		0x8026da2d, 
		0x80000000, 
		0x42100000, 
		0x7597e486, 
		0x0c7ffffc, 
		0x80000020, 
		0x0c7ffffc, 
		0x8000e475, 
		0x0c7ffe00, 
		0x80000001, 
		0x80800000, 
		0x0c7ffff8, 
		0xbf800000, 
		0x4d9b033e, 
		0x3f028f5c, 
		0xcb000000, 
		0x42640000, 
		0x80000000, 
		0x7fe9bc21, 
		0x52f64e54, 
		0xffd01b4b, 
		0xc1100000, 
		0x8b2b04c1, 
		0x55946313, 
		0x80200000, 
		0x80020000, 
		0x80000080, 
		0x00000000, 
		0x8f7fffff, 
		0xc1980000, 
		0x7fca3018, 
		0xc26c0000, 
		0x80000001, 
		0xc1a00000, 
		0x7f800000, 
		0x0d000ff0, 
		0x44e44573, 
		0x80000000, 
		0x42600000, 
		0xd57d366e, 
		0x00004000, 
		0xc2500000, 
		0xc2440000, 
		0x41d00000, 
		0xff800000, 
		0xc1a80000, 
		0x3315d1a8, 
		0xbbda6c74, 
		0xc1b80000, 
		0x851c9410, 
		0x42140000, 
		0x7f800000, 
		0xc2600000, 
		0x6510afdb, 
		0xc27c0000, 
		0xa854d28d, 
		0x00010000, 
		0xff7ffffe, 
		0x42340000, 
		0x0e007fff, 
		0xff800000, 
		0x421c0000, 
		0xff7fffff, 
		0x5385cb75, 
		0x80000000, 
		0xc23c0000, 
		0x80001000, 
		0xa200c59e, 
		0x80000200, 
		0x1f1662fd, 
		0x00000000, 
		0x42640000, 
		0x3032b5b8, 
		0xff7ffffe, 
		0x80000002, 
		0x5e10b39f, 
		0x80000000, 
		0x80000020, 
		0xc0a00000, 
		0xc2440000, 
		0x8de5f14f, 
		0x80000000, 
		0x00080000, 
		0x0c7ffffe, 
		0xc2140000, 
		0x41a00000, 
		0xc0800000, 
		0x0c780000, 
		0x00000002, 
		0xc946e552, 
		0x41a00000, 
		0x35ce730a, 
		0x0d000ff0, 
		0x42f9dee4, 
		0xcb8c4b40, 
		0x80000010, 
		0x426c0000, 
		0xc1d00000, 
		0x41300000, 
		0x00000000, 
		0x80008000, 
		0xff800000, 
		0x0e0000ff, 
		0x00000000, 
		0x80000008, 
		0xc1500000, 
		0x0e003fff, 
		0xff7ffffe, 
		0x7f800000, 
		0x80800000, 
		0x40800000, 
		0x7fc00000, 
		0x7d9c56af, 
		0xc1c80000, 
		0x80002000, 
		0x2d3bc373, 
		0x0e000001, 
		0x42740000, 
		0x54088af3, 
		0xedbd845b, 
		0x80ffffff, 
		0x00200000, 
		0x00000001, 
		0x0e01ffff, 
		0xff800000, 
		0x0e001fff, 
		0x9dab4b7a, 
		0xc1f00000, 
		0x38f0738a, 
		0x41400000, 
		0xbf800000, 
		0x80000002, 
		0x7f800000, 
		0x00000080, 
		0x41900000, 
		0x8f7fffff, 
		0x80000001, 
		0x0c7fe000, 
		0xca75caff, 
		0xff7fffff, 
		0x421c0000, 
		0x80001000, 
		0x42200000, 
		0x80800000, 
		0xc1500000, 
		0x9b9b91c1, 
		0x0c7ffffc, 
		0x80000080, 
		0xbf800001, 
		0x42000000, 
		0x00000000, 
		0x7f7fffff, 
		0x6ee09529, 
		0x00080000, 
		0xf03f0849, 
		0x0e000003, 
		0x41300000, 
		0xc2280000, 
		0x427c0000, 
		0x80200000, 
		0x0e118182, 
		0x0c7fe000, 
		0x0e000fff, 
		0x0e0007ff, 
		0x41e00000, 
		0x80800002, 
		0x33333333, 
		0x00004000, 
		0xc1880000, 
		0x3f800001, 
		0x0e007fff, 
		0x0f248b7d, 
		0xc2040000, 
		0xc2700000, 
		0x42780000, 
		0x80000000, 
		0xff7fffff, 
		0x42140000, 
		0x00400000, 
		0x00800000, 
		0x9578a6a2, 
		0xc1f00000, 
		0x0e001fff, 
		0x80000000, 
		0x80000000, 
		0x00000000, 
		0x1c7b0b2d, 
		0x82b46051, 
		0xb798f2b6, 
		0xff905d37, 
		0x6a59dd0b, 
		0x00000100, 
		0x42640000, 
		0x42240000, 
		0x3b8cd1f9, 
		0x426c0000, 
		0x40400000, 
		0xd7cab883, 
		0x00000000, 
		0xc97c6cdb, 
		0x7fadc25c, 
		0x71c18a87, 
		0xde5d1fd4, 
		0x2b8d57b2, 
		0xf73216e4, 
		0x41800000, 
		0xcccccccc, 
		0x80000040, 
		0x54faa6a5, 
		0xc2500000, 
		0xa3c74b97, 
		0x298ea68f, 
		0x80011111, 
		0xfa77d266, 
		0x7f7ffffe, 
		0x00000020, 
		0x7f000000, 
		0xfee3c162, 
		0x40e00000, 
		0x41f80000, 
		0xff7ffffe, 
		0x178c0f8f, 
		0xb6c896df, 
		0x00400000, 
		0xc21c0000, 
		0xff800000, 
		0xc2600000, 
		0x41800000, 
		0x42180000, 
		0xc75240ae, 
		0xff800000, 
		0x007fffff, 
		0x42380000, 
		0x932cd32d, 
		0x00000000, 
		0x41c00000, 
		0xc27c0000, 
		0x80800000, 
		0x00000001, 
		0x00000000, 
		0x80011111, 
		0x26bd68c7, 
		0x26165f84, 
		0x80000004, 
		0x550994ac, 
		0x621551a1, 
		0x0c400000, 
		0x95ae2c06, 
		0x0c7ffffc, 
		0x2bd3642d, 
		0x0c600000, 
		0xc0e00000, 
		0xff7ffffe, 
		0x41d00000, 
		0x40e00000, 
		0x277ea436, 
		0xff7ffffe, 
		0x0e00ffff, 
		0x42740000, 
		0x3d353149, 
		0x80800001, 
		0xff800000, 
		0x0e003fff, 
		0x00000000, 
		0x5b9b7033, 
		0xf3414cef, 
		0x80000800, 
		0xc1400000, 
		0x41f80000, 
		0x0c7f0000, 
		0x11ed8676, 
		0x41a00000, 
		0x2350d235, 
		0x41e00000, 
		0xffbf7ca7, 
		0xa5e816bc, 
		0x458fb5e2, 
		0x8f7ffffe, 
		0xe7493fe3, 
		0x42480000, 
		0x00000000, 
		0x0c7ff000, 
		0x42440000, 
		0xff000000, 
		0x4b8c4b40, 
		0xdfd50ae6, 
		0x290c7622, 
		0x00800001, 
		0x00000200, 
		0xbeae8992, 
		0xc2700000, 
		0x009e63aa, 
		0x0c7c0000, 
		0x3bdc7a57, 
		0x80ffffff, 
		0x0c7ffc00, 
		0xc2200000, 
		0x80000000, 
		0x41a80000, 
		0xc26c0000, 
		0x0003c564, 
		0xff7ffffe, 
		0xfe265809, 
		0x42080000, 
		0xffc00000, 
		0xc2100000, 
		0x41d00000, 
		0x00008000, 
		0x2bfe18bf, 
		0xff7ffffe, 
		0x41c80000, 
		0x0e001fff, 
		0x0c7ff800, 
		0x42500000, 
		0x0c7ffffe, 
		0x7f7ffffe, 
		0x7f9791b8, 
		0x7f9dcae1, 
		0x42500000, 
		0x3661117e, 
		0x3f800000, 
		0x404b5613, 
		0x0e03ffff, 
		0xf1581e02, 
		0x7f7ffffe, 
		0x41c00000, 
		0x42580000, 
		0x00000001, 
		0x0c7fe000, 
		0x00000000, 
		0x0c7fffe0, 
		0x0c7ffff8, 
		0x0c7ffffe, 
		0xc2600000, 
		0x42340000, 
		0x7f7ffffe, 
		0x7f800000, 
		0x7f800000, 
		0xc23c0000, 
		0x491d8cc7, 
		0x00000004, 
		0xc22c0000, 
		0xca662fdf, 
		0x41500000, 
		0xff800001, 
		0x171dc9d2, 
		0xb7aefa0c, 
		0x80000080, 
		0x40000000, 
		0xb299112a, 
		0x80800000, 
		0xffc00001, 
		0x3b67e751, 
		0x42280000, 
		0xedcb6f91, 
		0xb0fd05d8, 
		0x807fffff, 
		0x00800002, 
		0x41f80000, 
		0xc2180000, 
		0x19d29a5c, 
		0x80000000, 
		0xc1d00000, 
		0x780e2e92, 
		0xffc00000, 
		0xff800000, 
		0x7f800000, 
		0xc2200000, 
		0xf7324fa2, 
		0x90b5f015, 
		0xc2440000, 
		0x00020000, 
		0x80000000, 
		0x42600000, 
		0xc1d00000, 
		0x80001000, 
		0x0c7fe000, 
		0x6362e01b, 
		0x42140000, 
		0x80000000, 
		0x42740000  
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
		0x000001a5, 
		0x000003b9, 
		0x000001d1, 
		0x0000028f, 
		0x000002b7, 
		0x0000039b, 
		0x00000119, 
		0x000002cf, 
		0x000002ed, 
		0x000000c3, 
		0x00000309, 
		0x00000141, 
		0x00000113, 
		0x0000001f, 
		0x000002d5, 
		0x00000065, 
		0x00000025, 
		0x00000079, 
		0x00000045, 
		0x00000093, 
		0x0000028b, 
		0x00000227, 
		0x0000033b, 
		0x000001df, 
		0x0000009f, 
		0x00000257, 
		0x0000016d, 
		0x000001bf, 
		0x000003b7, 
		0x0000025f, 
		0x000001b3, 
		0x00000043, 
		0x000003fd, 
		0x0000017d, 
		0x000001f3, 
		0x00000339, 
		0x0000031f, 
		0x000001d5, 
		0x00000355, 
		0x000001d5, 
		0x0000000d, 
		0x000003ab, 
		0x0000003f, 
		0x0000030f, 
		0x00000359, 
		0x00000001, 
		0x0000037d, 
		0x0000009b, 
		0x00000131, 
		0x0000005d, 
		0x00000173, 
		0x00000333, 
		0x000001a7, 
		0x000002f3, 
		0x0000033b, 
		0x00000181, 
		0x0000034f, 
		0x000001ed, 
		0x0000035d, 
		0x00000007, 
		0x000002c3, 
		0x00000135, 
		0x0000026f, 
		0x000000e5, 
		0x000003ab, 
		0x0000038b, 
		0x000001e7, 
		0x00000161, 
		0x000001db, 
		0x00000361, 
		0x0000026b, 
		0x00000131, 
		0x0000011d, 
		0x000000e9, 
		0x00000033, 
		0x000001bd, 
		0x000003c5, 
		0x00000361, 
		0x00000179, 
		0x0000035f, 
		0x000001e9, 
		0x00000289, 
		0x000001fd, 
		0x00000059, 
		0x000002a7, 
		0x000001b3, 
		0x0000029b, 
		0x00000091, 
		0x000000db, 
		0x00000281, 
		0x000002b5, 
		0x0000030b, 
		0x0000002f, 
		0x000002c9, 
		0x00000125, 
		0x000000af, 
		0x0000033d, 
		0x00000347, 
		0x000003cf, 
		0x0000006f, 
		0x00000143, 
		0x0000030d, 
		0x000000b1, 
		0x000001b9, 
		0x000003df, 
		0x000002ef, 
		0x00000079, 
		0x000001ef, 
		0x0000010d, 
		0x00000253, 
		0x000003d1, 
		0x00000117, 
		0x000000cb, 
		0x000001f9, 
		0x00000351, 
		0x0000003d, 
		0x0000005b, 
		0x0000029d, 
		0x000002bf, 
		0x000001a1, 
		0x00000211, 
		0x0000029d, 
		0x00000133, 
		0x0000035f, 
		0x0000035f, 
		0x0000026f, 
		0x0000017b, 
		0x00000031, 
		0x00000343, 
		0x00000349, 
		0x0000034f, 
		0x000003ed, 
		0x0000039b, 
		0x0000018d, 
		0x000001ff, 
		0x000000ab, 
		0x000003d7, 
		0x000003b9, 
		0x00000331, 
		0x00000173, 
		0x000000f7, 
		0x000003bb, 
		0x000001ef, 
		0x000001ed, 
		0x000002dd, 
		0x00000129, 
		0x00000023, 
		0x000001f3, 
		0x000001eb, 
		0x0000037f, 
		0x00000137, 
		0x00000109, 
		0x00000091, 
		0x00000383, 
		0x00000371, 
		0x0000027f, 
		0x000000a1, 
		0x0000013f, 
		0x000003af, 
		0x00000333, 
		0x0000026b, 
		0x00000001, 
		0x00000301, 
		0x0000037b, 
		0x000003e5, 
		0x000002b1, 
		0x000002ef, 
		0x0000018f, 
		0x000003e5, 
		0x000003ab, 
		0x0000009f, 
		0x000000ff, 
		0x000000b7, 
		0x00000155, 
		0x00000281, 
		0x0000009f, 
		0x000003a1, 
		0x000000b5, 
		0x0000000b, 
		0x000001cd, 
		0x0000033f, 
		0x000002b9, 
		0x000003e9, 
		0x0000026b, 
		0x000003bb, 
		0x0000031d, 
		0x00000019, 
		0x00000135, 
		0x0000023b, 
		0x000002d3, 
		0x000000e3, 
		0x00000091, 
		0x000003a3, 
		0x0000018f, 
		0x000000e1, 
		0x0000008f, 
		0x00000395, 
		0x000002a1, 
		0x000002c5, 
		0x000003b1, 
		0x000002f7, 
		0x0000037d, 
		0x000001f7, 
		0x0000002b, 
		0x00000273, 
		0x000003cd, 
		0x000002b3, 
		0x0000024b, 
		0x00000231, 
		0x00000323, 
		0x0000010f, 
		0x00000301, 
		0x000001a7, 
		0x0000018b, 
		0x00000091, 
		0x000001eb, 
		0x0000006f, 
		0x0000037f, 
		0x0000036f, 
		0x0000033f, 
		0x000003bb, 
		0x000000ef, 
		0x00000067, 
		0x000000cf, 
		0x00000367, 
		0x000003c1, 
		0x0000025b, 
		0x00000041, 
		0x00000301, 
		0x000002b7, 
		0x0000035b, 
		0x00000221, 
		0x00000243, 
		0x00000171, 
		0x0000028f, 
		0x00000003, 
		0x000001a7, 
		0x00000347, 
		0x0000036d, 
		0x000001d5, 
		0x000003dd, 
		0x0000005f, 
		0x0000029f, 
		0x00000001, 
		0x000000e1, 
		0x000001c5, 
		0x000003f1, 
		0x00000245, 
		0x000003d3, 
		0x0000016d, 
		0x00000045, 
		0x000003cf, 
		0x000002db, 
		0x0000034b, 
		0x000001af, 
		0x0000029f  
};

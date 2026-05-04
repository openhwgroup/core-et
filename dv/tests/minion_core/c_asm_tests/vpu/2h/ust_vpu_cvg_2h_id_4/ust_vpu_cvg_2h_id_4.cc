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
			"csrwi mhpmevent4, 19\n"   
			"csrwi mhpmevent5, 28\n"   
			"csrwi mhpmevent6, 20\n"   
			"csrwi mhpmevent7, 4\n"   
			"csrwi mhpmevent8, 6\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f2,  64 (x5)\n"
			"flw.ps f12,  128 (x5)\n"
			"flw.ps f4,  928 (x5)\n"
			"flw.ps f20,  832 (x5)\n"
			"flw.ps f30,  0 (x5)\n"
			"flw.ps f23,  992 (x5)\n"
			"flw.ps f18,  896 (x5)\n"
			"flw.ps f3,  704 (x5)\n"
			"flw.ps f14,  320 (x5)\n"
			"flw.ps f7,  512 (x5)\n"
			"flw.ps f25,  864 (x5)\n"
			"flw.ps f15,  448 (x5)\n"
			"flw.ps f16,  608 (x5)\n"
			"flw.ps f29,  192 (x5)\n"
			"flw.ps f8,  96 (x5)\n"
			"flw.ps f19,  352 (x5)\n"
			"flw.ps f11,  768 (x5)\n"
			"flw.ps f21,  800 (x5)\n"
			"flw.ps f28,  576 (x5)\n"
			"flw.ps f0,  32 (x5)\n"
			"flw.ps f10,  960 (x5)\n"
			"flw.ps f9,  256 (x5)\n"
			"flw.ps f22,  384 (x5)\n"
			"flw.ps f31,  160 (x5)\n"
			"flw.ps f26,  224 (x5)\n"
			"flw.ps f27,  672 (x5)\n"
			"flw.ps f17,  288 (x5)\n"
			"flw.ps f24,  480 (x5)\n"
			"flw.ps f1,  416 (x5)\n"
			"flw.ps f5,  544 (x5)\n"
			"flw.ps f6,  640 (x5)\n"
			"flw.ps f13,  736 (x5)\n"
			"csrw fflags, zero\n"
			"csrwi frm, 0x0\n"
			"LBL_KERNEL_HID_0:\n"          
			"li x5, 0xFEEDFEED\n"
			"li x0, 0x1f4\n" 
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
			"la x23, rand_ieee754_32\n"
			"flw.ps f9, 832(x23)\n"
			"fcvt.w.s x12, f9, rtz\n" 
			"la x7, rand_int_32\n"
			"flw.ps f22, 0(x7)\n"
			"fcvt.ps.pw f11, f22, rup\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f7, 960(x14)\n"
			"flw.ps f31, 896(x14)\n"
			"feq.s x14, f7, f31\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f14, 640(x19)\n"
			"fround.ps f26, f14, rdn\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f1, 64(x23)\n"
			"flw.ps f7, 288(x23)\n"
			"flw.ps f30, 192(x23)\n"
			"fmsub.ps f30, f1, f7, f30, rup\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f30, 288(x14)\n"
			"flw.ps f11, 896(x14)\n"
			"flt.ps f22, f30, f11\n" 
			"la x17, rand_int_32\n"
			"flw.ps f17, 672(x17)\n"
			"fcvt.ps.pwu f1, f17, rmm\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f11, 160(x22)\n"
			"flw.ps f22, 800(x22)\n"
			"fsgnj.s f1, f11, f22\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_0:\n"
			"mov.m.x m6, x14, 0x21\n" 
			"li x5, 0x80340150\n"
			"ld x5, 0(x5)\n"
			"la x23, rand_ieee754_32\n"
			"flw.ps f31, 224(x23)\n"
			"fsin.ps f12, f31\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f10, 256(x17)\n"
			"fsqrt.ps f18, f10\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f19, 832(x11)\n"
			"flw.ps f14, 864(x11)\n"
			"fle.s x28, f19, f14\n" 
			"la x11, rand_int_32\n"
			"flw.ps f9, 736(x11)\n"
			"flw.ps f31, 224(x11)\n"
			"fmaxu.pi f21, f9, f31\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_0:\n"
			"maskor m2, m2, m5\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x15, rand_int_32\n"
			"flw.ps f1, 704(x15)\n"
			"fcvt.ps.pwu f7, f1, rmm\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f11, 384(x17)\n"
			"flw.ps f14, 224(x17)\n"
			"flw.ps f10, 576(x17)\n"
			"fnmadd.s f14, f11, f14, f10, rtz\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_0\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_1_M0_WRITE_ID_2_HID_0:\n"
			"fltm.ps m6, f12, f26\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x28, rand_ieee754_32\n"
			"flw.ps f22, 544(x28)\n"
			"flw.ps f14, 736(x28)\n"
			"fsgnj.ps f18, f22, f14\n" 
			"la x7, rand_int_32\n"
			"flw.ps f26, 32(x7)\n"
			"fsrli.pi f19, f26, 0x8\n" 
			"la x11, rand_int_32\n"
			"flw.ps f17, 0(x11)\n"
			"fmulh.pi f14, f17, f17\n" 
			"la x19, rand_int_32\n"
			"flw.ps f14, 160(x19)\n"
			"flw.ps f17, 0(x19)\n"
			"fremu.pi f31, f14, f17\n" 
			"la x17, rand_int_32\n"
			"flw.ps f21, 224(x17)\n"
			"fnot.pi f21, f21\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f31, 928(x15)\n"
			"flw.ps f1, 928(x15)\n"
			"fsgnjx.ps f30, f31, f1\n" 
			"LBL_SEQID_1_M0_WRITE_ID_3_HID_0:\n"
			"fltm.pi m2, f9, f10\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0xbaee473620\n"
			"flw.ps f5, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_1_M0_WRITE_ID_4_HID_0:\n"
			"flem.ps m7, f26, f1\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"maskpopc x11, m6\n" 
			"la x28, rand_int_32\n"
			"flw.ps f10, 320(x28)\n"
			"flw.ps f9, 320(x28)\n"
			"flt.pi f17, f10, f9\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f30, 800(x12)\n"
			"flw.ps f21, 352(x12)\n"
			"fmul.ps f18, f30, f21, rne\n" 
			"la x7, rand_ieee754_16\n"
			"flw.ps f7, 992(x7)\n"
			"fcvt.ps.f16 f22, f7\n" 
			"la x28, rand_int_32\n"
			"flw.ps f1, 288(x28)\n"
			"fnot.pi f26, f1\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f7, 544(x14)\n"
			"flw.ps f18, 288(x14)\n"
			"fsgnj.s f14, f7, f18\n" 
			"la x12, rand_int_32\n"
			"flw.ps f17, 64(x12)\n"
			"fpackrepb.pi f28, f17\n" 
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
			"LBL_HPM_3_CORE_TS_OPS_FAIL_HID_0:\n"          
			"csrr x5, mhpmcounter3\n"
			"sd x5, 32 (x31)\n"
			"LBL_HPM_4_CORE_TIMA_OPS_FAIL_HID_0:\n"          
			"csrr x5, mhpmcounter4\n"
			"sd x5, 40 (x31)\n"
			"LBL_HPM_5_CORE_TQUANT_INST_FAIL_HID_0:\n"          
			"csrr x5, mhpmcounter5\n"
			"sd x5, 48 (x31)\n"
			"LBL_HPM_6_CORE_TXFMA_OPS16_FAIL_HID_0:\n"          
			"csrr x5, mhpmcounter6\n"
			"sd x5, 56 (x31)\n"
			"LBL_HPM_7_NEIGH_TX_NEIGH_COOP_TLD_REQ_FAIL_HID_0:\n"          
			"csrr x5, mhpmcounter7\n"
			"sd x5, 64 (x31)\n"
			"LBL_HPM_8_NEIGH_TX_COOP_TST_REQ_FAIL_HID_0:\n"          
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
			"LBL_HPM_4_CORE_TIMA_OPS_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter4\n"
			"sd x5, 40 (x31)\n"
			"LBL_HPM_5_CORE_TQUANT_INST_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter5\n"
			"sd x5, 48 (x31)\n"
			"LBL_HPM_6_CORE_TXFMA_OPS16_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter6\n"
			"sd x5, 56 (x31)\n"
			"LBL_HPM_7_NEIGH_TX_NEIGH_COOP_TLD_REQ_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter7\n"
			"sd x5, 64 (x31)\n"
			"LBL_HPM_8_NEIGH_TX_COOP_TST_REQ_PASS_HID_0:\n"          
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
			"csrwi mhpmevent3, 6\n"   
			"csrwi mhpmevent4, 3\n"   
			"csrwi mhpmevent5, 1\n"   
			"csrwi mhpmevent6, 2\n"   
			"csrwi mhpmevent7, 21\n"   
			"csrwi mhpmevent8, 22\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f9,  672 (x5)\n"
			"flw.ps f8,  736 (x5)\n"
			"flw.ps f30,  960 (x5)\n"
			"flw.ps f20,  320 (x5)\n"
			"flw.ps f16,  0 (x5)\n"
			"flw.ps f2,  160 (x5)\n"
			"flw.ps f18,  640 (x5)\n"
			"flw.ps f4,  288 (x5)\n"
			"flw.ps f7,  992 (x5)\n"
			"flw.ps f21,  64 (x5)\n"
			"flw.ps f28,  512 (x5)\n"
			"flw.ps f25,  800 (x5)\n"
			"flw.ps f14,  544 (x5)\n"
			"flw.ps f0,  608 (x5)\n"
			"flw.ps f15,  448 (x5)\n"
			"flw.ps f26,  352 (x5)\n"
			"flw.ps f1,  896 (x5)\n"
			"flw.ps f13,  192 (x5)\n"
			"flw.ps f29,  832 (x5)\n"
			"flw.ps f17,  768 (x5)\n"
			"flw.ps f10,  928 (x5)\n"
			"flw.ps f5,  256 (x5)\n"
			"flw.ps f11,  384 (x5)\n"
			"flw.ps f6,  128 (x5)\n"
			"flw.ps f23,  32 (x5)\n"
			"flw.ps f3,  416 (x5)\n"
			"flw.ps f24,  96 (x5)\n"
			"flw.ps f27,  576 (x5)\n"
			"flw.ps f19,  480 (x5)\n"
			"flw.ps f12,  704 (x5)\n"
			"flw.ps f31,  864 (x5)\n"
			"flw.ps f22,  224 (x5)\n"
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
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_1:\n"
			"fltm.pi m5, f7, f8\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x25, rand_int_32\n"
			"flw.ps f24, 864(x25)\n"
			"fslli.pi f27, f24, 0xd\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f21, 448(x13)\n"
			"flw.ps f28, 576(x13)\n"
			"flw.ps f27, 96(x13)\n"
			"fmsub.s f1, f21, f28, f27, rtz\n" 
			"la x5, rand_int_32\n"
			"flw.ps f7, 832(x5)\n"
			"fpackrepb.pi f19, f7\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_1:\n"
			"maskxor m6, m2, m5\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x16, rand_int_32\n"
			"flw.ps f22, 256(x16)\n"
			"fnot.pi f8, f22\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f23, 512(x18)\n"
			"fclass.ps f8, f23\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f16, 416(x13)\n"
			"fclass.ps f7, f16\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f6, 864(x16)\n"
			"flw.ps f8, 896(x16)\n"
			"flw.ps f7, 448(x16)\n"
			"fmadd.ps f8, f6, f8, f7, rtz\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f6, 288(x20)\n"
			"fclass.ps f2, f6\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f8, 640(x16)\n"
			"fcvt.w.s x25, f8, rmm\n" 
			"la x20, rand_int_32\n"
			"flw.ps f1, 384(x20)\n"
			"fsrai.pi f23, f1, 0x6\n" 
			"LBL_SEQID_0_M0_WRITE_ID_2_HID_1:\n"
			"mov.m.x m5, x14, 0x62\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x16, rand_ieee754_32\n"
			"flw.ps f13, 192(x16)\n"
			"flw.ps f8, 288(x16)\n"
			"flw.ps f16, 96(x16)\n"
			"fmadd.s f9, f13, f8, f16, rtz\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f16, 960(x5)\n"
			"flw.ps f25, 512(x5)\n"
			"flw.ps f1, 416(x5)\n"
			"fmsub.ps f2, f16, f25, f1, rtz\n" 
			"la x5, rand_ieee754_16\n"
			"flw.ps f16, 0(x5)\n"
			"fcvt.ps.f16 f19, f16\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_1\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_1_M0_WRITE_ID_3_HID_1:\n"
			"fltm.pi m5, f22, f8\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x13, rand_ieee754_32\n"
			"flw.ps f1, 448(x13)\n"
			"ffrc.ps f9, f1\n" 
			"li x13, 0x188ac2b2e2d7099f\n"
			"fcvt.s.w f27, x13, rdn\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f27, 448(x25)\n"
			"flw.ps f1, 832(x25)\n"
			"flt.ps f23, f27, f1\n" 
			"LBL_SEQID_1_M0_WRITE_ID_4_HID_1:\n"
			"feqm.ps m2, f28, f25\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0xf53ae7cc74\n"
			"fsw.ps f0, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x18, rand_ieee754_32\n"
			"flw.ps f19, 576(x18)\n"
			"flw.ps f8, 256(x18)\n"
			"fcmovm.ps f28, f19, f8\n" 
			"fbci.ps f21, 0x39a3c\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f2, 832(x27)\n"
			"flw.ps f25, 320(x27)\n"
			"fsgnjn.ps f16, f2, f25\n" 
			"la x11, rand_ieee754_16\n"
			"flw.ps f6, 416(x11)\n"
			"fcvt.ps.f16 f22, f6\n" 
			"la x11, rand_int_32\n"
			"flw.ps f25, 704(x11)\n"
			"flw.ps f13, 320(x11)\n"
			"fmulhu.pi f27, f25, f13\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f6, 640(x27)\n"
			"flw.ps f16, 320(x27)\n"
			"fsgnj.ps f2, f6, f16\n" 
			"la x11, rand_int_32\n"
			"flw.ps f9, 896(x11)\n"
			"flw.ps f22, 608(x11)\n"
			"fsub.pi f25, f9, f22\n" 
			"la x20, rand_int_32\n"
			"flw.ps f22, 544(x20)\n"
			"flw.ps f7, 544(x20)\n"
			"fminu.pi f7, f22, f7\n" 
			"LBL_SEQID_1_M0_WRITE_ID_5_HID_1:\n"
			"maskor m2, m6, m2\n" 
			"li x5, 0x803401A0\n"
			"ld x5, 0(x5)\n"
			"maskpopc x18, m5\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f28, 352(x13)\n"
			"flw.ps f8, 224(x13)\n"
			"flt.ps f9, f28, f8\n" 
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
			"LBL_HPM_3_CORE_DCACHE_ACCESS0_FAIL_HID_1:\n"          
			"csrr x5, mhpmcounter3\n"
			"sd x5, 32 (x31)\n"
			"LBL_HPM_4_CORE_RETIRED_INST1_FAIL_HID_1:\n"          
			"csrr x5, mhpmcounter4\n"
			"sd x5, 40 (x31)\n"
			"LBL_HPM_5_CORE_MCYCLES_FAIL_HID_1:\n"          
			"csrr x5, mhpmcounter5\n"
			"sd x5, 48 (x31)\n"
			"LBL_HPM_6_CORE_RETIRED_INST0_FAIL_HID_1:\n"          
			"csrr x5, mhpmcounter6\n"
			"sd x5, 56 (x31)\n"
			"LBL_HPM_7_NEIGH_ETL_REQ_TO_INTERMEDIATE_FIFO_FAIL_HID_1:\n"          
			"csrr x5, mhpmcounter7\n"
			"sd x5, 64 (x31)\n"
			"LBL_HPM_8_NEIGH_ETL_REQ_TO_SC_UC_FIFO_FAIL_HID_1:\n"          
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
			"LBL_HPM_3_CORE_DCACHE_ACCESS0_PASS_HID_1:\n"          
			"csrr x5, mhpmcounter3\n"
			"sd x5, 32 (x31)\n"
			"LBL_HPM_4_CORE_RETIRED_INST1_PASS_HID_1:\n"          
			"csrr x5, mhpmcounter4\n"
			"sd x5, 40 (x31)\n"
			"LBL_HPM_5_CORE_MCYCLES_PASS_HID_1:\n"          
			"csrr x5, mhpmcounter5\n"
			"sd x5, 48 (x31)\n"
			"LBL_HPM_6_CORE_RETIRED_INST0_PASS_HID_1:\n"          
			"csrr x5, mhpmcounter6\n"
			"sd x5, 56 (x31)\n"
			"LBL_HPM_7_NEIGH_ETL_REQ_TO_INTERMEDIATE_FIFO_PASS_HID_1:\n"          
			"csrr x5, mhpmcounter7\n"
			"sd x5, 64 (x31)\n"
			"LBL_HPM_8_NEIGH_ETL_REQ_TO_SC_UC_FIFO_PASS_HID_1:\n"          
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
		0x5cb05f29, 
		0x5baf6601, 
		0x86b75926, 
		0xbba452ab, 
		0x4592430a, 
		0xd25e66fe, 
		0x849b4832, 
		0xd73ceb03, 
		0xc3f812d7, 
		0xcfed1d84, 
		0x38e890f9, 
		0xecc072f2, 
		0x546d432a, 
		0x97ece8c2, 
		0xe4622abe, 
		0xd2964169, 
		0xa2290ca2, 
		0xe00759e3, 
		0xb6e28c51, 
		0x7ae196ee, 
		0x27ed1bc2, 
		0x67d2dc28, 
		0xace3a3e4, 
		0x6926dd32, 
		0x5abeea15, 
		0xa4ac81bc, 
		0x5b30a76d, 
		0xcdf85148, 
		0x3a5a612f, 
		0x8eebf442, 
		0x68b038f9, 
		0xf33c66eb, 
		0x3d32c540, 
		0x17030261, 
		0xbcf4584c, 
		0x069018d4, 
		0x8b56217e, 
		0x6db72e39, 
		0x5de8c57a, 
		0xb4197bff, 
		0x2d2288a7, 
		0xd8ab202e, 
		0xc348ee02, 
		0x0ab57ad7, 
		0x09a3dd77, 
		0x65b162e3, 
		0x4305ba66, 
		0x1d05c8c7, 
		0xcc1b7036, 
		0x0bbc4a19, 
		0xa433ff9d, 
		0xf1a5dc86, 
		0x8bad8042, 
		0xca280b54, 
		0xaa3ad3e8, 
		0xb8e79154, 
		0xed95f78b, 
		0x4aacce8f, 
		0xf41ef1ae, 
		0xb2b5ee6a, 
		0x5521e577, 
		0x16a85669, 
		0x99138704, 
		0xb01cfac2, 
		0x8d604c89, 
		0x09d4a7ba, 
		0x32d47316, 
		0x5bf0153b, 
		0x61aea5b2, 
		0x769b95a3, 
		0x1c579fe1, 
		0x4d70f131, 
		0xe3f335f4, 
		0x4a448de6, 
		0xe9fcaa25, 
		0x67ccc826, 
		0xf4040618, 
		0x9807b0db, 
		0x9ba6ce21, 
		0xd0efa74c, 
		0x7f19a2e5, 
		0xd3dce8b1, 
		0xbe67c8ab, 
		0x74a7c5d7, 
		0x953cd589, 
		0x68efb0ab, 
		0x938b1ee5, 
		0x85652cbc, 
		0x198a9b27, 
		0xc36cc578, 
		0xa5e4f9fd, 
		0x1bef43f6, 
		0x583c09b9, 
		0xfec908d1, 
		0xa6a3c9ee, 
		0xdef5751f, 
		0x7ef3199b, 
		0x67d41d10, 
		0xd9e9150f, 
		0x8e6e4be6, 
		0xffc23c8c, 
		0x3c9e7177, 
		0xf7f8802e, 
		0x3eb342b6, 
		0x1bf3aa08, 
		0xfd5f536f, 
		0xed001b96, 
		0x600753ba, 
		0x4829cc20, 
		0x74970110, 
		0x3beaf3c1, 
		0x86a5f64b, 
		0x969edcc4, 
		0x05064da9, 
		0x5319fcd0, 
		0x451dccf1, 
		0xc1395c52, 
		0x89308059, 
		0x64cb5c44, 
		0xadfdc7cf, 
		0x66bc489a, 
		0x3efe02a2, 
		0x7c0a5fd1, 
		0x44711bcf, 
		0x3a1b5d5c, 
		0x82b6bc00, 
		0x8cd77fdf, 
		0xed76c679, 
		0x71ef65d0, 
		0xfc38c359, 
		0xe92573c3, 
		0x4d453d2a, 
		0x82419e54, 
		0xe2ffa197, 
		0x9ac4cd22, 
		0xb1499c45, 
		0xb9c25725, 
		0xc2f70c9d, 
		0x75929fb3, 
		0x9646ecff, 
		0xedcf25eb, 
		0x6a558450, 
		0x15238380, 
		0x6905dd69, 
		0xabe6997a, 
		0x83ccdc64, 
		0x1a80c618, 
		0xc279d19f, 
		0x9f13c26c, 
		0xbf43a856, 
		0xdf659cb3, 
		0x1a0b1d57, 
		0xc50a48d0, 
		0xf8359f09, 
		0x5ec21aa5, 
		0x5a04d173, 
		0x6192b7e6, 
		0x3a963bf4, 
		0x9201eca1, 
		0xe2037ece, 
		0x6f606482, 
		0xf3139be0, 
		0x2b98626f, 
		0x3273b100, 
		0x5b591b5b, 
		0xa39bd5c0, 
		0x13ad05c7, 
		0x9b42e76b, 
		0x1c3206a2, 
		0x3cc30d71, 
		0x70c079b8, 
		0xd9426473, 
		0x53523356, 
		0xa0428c06, 
		0x6eb96971, 
		0x0a5378ed, 
		0x78fb5f35, 
		0xf7e6826c, 
		0x53d7f5bc, 
		0xb770290c, 
		0x72063e84, 
		0x6d57886d, 
		0x5f6f8b0c, 
		0x052966a5, 
		0x43482c3e, 
		0x8b1c17ff, 
		0x940e876f, 
		0xe7c6e63d, 
		0x613b1c52, 
		0x997f0955, 
		0x5a92652f, 
		0xca21f659, 
		0xc05a7015, 
		0xee5a7196, 
		0x0ba491c6, 
		0x8957d76a, 
		0x80ea1fd7, 
		0x966d9651, 
		0xde019e9b, 
		0xe13f09ae, 
		0xa26438e3, 
		0x05ffa9df, 
		0x6638e6ae, 
		0x1f8b05ef, 
		0xfad57a0b, 
		0xa9bbf293, 
		0x473a46f4, 
		0x4baa60b3, 
		0x9da783dc, 
		0x24609c07, 
		0xe208e79e, 
		0x600936a4, 
		0x51229e36, 
		0x61fba165, 
		0x6dcab9c5, 
		0x89354f92, 
		0xd675987e, 
		0x489515a8, 
		0x57926a0e, 
		0xbf277259, 
		0x7bca3e19, 
		0x3cb656f5, 
		0x8a82835e, 
		0x3623a013, 
		0x18ba7ef8, 
		0x17a80a72, 
		0x67fbbd5c, 
		0xabb63913, 
		0x3962222a, 
		0x15d02591, 
		0xdf1a9d69, 
		0x07592178, 
		0x8e8fc6be, 
		0x61e54fe3, 
		0x9d616068, 
		0x7251bf9c, 
		0xb89860f4, 
		0x2d28b74f, 
		0x25a0b328, 
		0x4fd82abb, 
		0x5997232a, 
		0x4290a008, 
		0xc5498a3e, 
		0x92ed45e7, 
		0xa8524551, 
		0xa3a44556, 
		0x8bafffa1, 
		0xa9d0d079, 
		0x297fd5d5, 
		0x8625739e, 
		0x781312a1, 
		0x52d597ba, 
		0x7c4a0974, 
		0x6d362016, 
		0x5dc720e1, 
		0x85ae4984  
};
volatile uint16_t rand_ieee754_16[1536] __attribute__ ((aligned (2048))) = {
		0xcfc0, 
		0xfc00, 
		0xd280, 
		0x5200, 
		0xf178, 
		0x50a0, 
		0x4c80, 
		0x4cc0, 
		0xd1e0, 
		0xa4e5, 
		0xbc00, 
		0x4dc0, 
		0x93ef, 
		0xccc0, 
		0xcfc0, 
		0xad3b, 
		0xd2e0, 
		0xc600, 
		0x3f63, 
		0x4366, 
		0x5240, 
		0xb761, 
		0x21fa, 
		0xfc00, 
		0x53c0, 
		0x8000, 
		0xd120, 
		0xfc00, 
		0x4c5b, 
		0x50c0, 
		0x5060, 
		0x9395, 
		0x72eb, 
		0xaa3a, 
		0xcc80, 
		0x4a00, 
		0x52a0, 
		0x50c0, 
		0xd240, 
		0xc880, 
		0x4400, 
		0x1669, 
		0x9ade, 
		0x9647, 
		0xd200, 
		0x8000, 
		0x5060, 
		0x50c0, 
		0x3a87, 
		0x5180, 
		0x92c4, 
		0xd100, 
		0x4880, 
		0x7c00, 
		0xd300, 
		0xd19c, 
		0x3c00, 
		0xd140, 
		0xd100, 
		0x5140, 
		0xd6c4, 
		0x99c7, 
		0x51a0, 
		0xcc40, 
		0xcd00, 
		0x5220, 
		0x4a80, 
		0xcd80, 
		0x51e0, 
		0x0000, 
		0xcdc0, 
		0x52e0, 
		0x56d3, 
		0xc700, 
		0x9247, 
		0xcd88, 
		0xebc4, 
		0x4a80, 
		0x4e00, 
		0x685d, 
		0x7c4d, 
		0x7227, 
		0xa4e1, 
		0x8711, 
		0x5280, 
		0x7c00, 
		0xcdc0, 
		0xc200, 
		0x4a00, 
		0xfda7, 
		0xcb00, 
		0x0212, 
		0x775a, 
		0x9673, 
		0x53c0, 
		0x8000, 
		0x50e0, 
		0xcfc0, 
		0x5280, 
		0x8588, 
		0xce47, 
		0x466f, 
		0xcdc0, 
		0xe64a, 
		0xd320, 
		0xc980, 
		0xcf3d, 
		0x2b05, 
		0x87d3, 
		0x4a80, 
		0xc800, 
		0xd300, 
		0xcb80, 
		0xb21e, 
		0x0000, 
		0xd320, 
		0x7e24, 
		0xcf00, 
		0x4200, 
		0x7c00, 
		0xd000, 
		0xe7be, 
		0x4a80, 
		0xc700, 
		0xc880, 
		0x4900, 
		0x9a09, 
		0xfc00, 
		0x5180, 
		0xd200, 
		0x5a2c, 
		0x5260, 
		0xfc00, 
		0xaaf8, 
		0x8000, 
		0xceef, 
		0xd339, 
		0xd0c0, 
		0x0000, 
		0x7c66, 
		0x0000, 
		0xd020, 
		0xce80, 
		0x8000, 
		0x7d7e, 
		0xc500, 
		0x837b, 
		0x1077, 
		0xc400, 
		0x4f80, 
		0x4980, 
		0xcf00, 
		0xd2c0, 
		0x6224, 
		0xd200, 
		0xd3c0, 
		0x5200, 
		0x4f00, 
		0xd020, 
		0x607a, 
		0xbdd3, 
		0x4000, 
		0xd2a0, 
		0x52a0, 
		0x3a54, 
		0x39fe, 
		0x4900, 
		0x4cc0, 
		0xc980, 
		0xd140, 
		0xded5, 
		0xc980, 
		0x8000, 
		0x50c0, 
		0x5040, 
		0xcd80, 
		0xc500, 
		0x4fc0, 
		0x6c64, 
		0xfca3, 
		0x5c23, 
		0x4700, 
		0x53e0, 
		0xc600, 
		0x7c00, 
		0x4400, 
		0x4dc0, 
		0x5080, 
		0xd340, 
		0xd2e0, 
		0xc400, 
		0x4e40, 
		0x0000, 
		0xd2a0, 
		0x5360, 
		0x52e0, 
		0xd92a, 
		0x3dc4, 
		0xd100, 
		0xd220, 
		0xd2e0, 
		0x53c0, 
		0xd280, 
		0x5140, 
		0xf031, 
		0xcb80, 
		0xbc5d, 
		0x349b, 
		0xe947, 
		0x4f80, 
		0x4bc3, 
		0xc880, 
		0x7c00, 
		0x0e6d, 
		0xfc00, 
		0xd1c0, 
		0x4a80, 
		0x4600, 
		0x112e, 
		0xcd80, 
		0x4dc0, 
		0xd1a0, 
		0xde67, 
		0x5200, 
		0xd1e0, 
		0x5200, 
		0x67d0, 
		0x8000, 
		0x53e0, 
		0x93dc, 
		0xcf80, 
		0x4c00, 
		0xfc00, 
		0xd000, 
		0x0000, 
		0xf62a, 
		0x61a7, 
		0xc200, 
		0x2f22, 
		0x2665, 
		0x92e0, 
		0x52e0, 
		0x7c00, 
		0x5380, 
		0xfc00, 
		0xd0a0, 
		0xc800, 
		0x4a80, 
		0x4f00, 
		0x4b00, 
		0x5000, 
		0xcf80, 
		0x4980, 
		0xc500, 
		0xc0eb, 
		0x4e40, 
		0xd360, 
		0xca00, 
		0xe8e2, 
		0x5240, 
		0x31df, 
		0x8896, 
		0x4f80, 
		0xbc00, 
		0x706d, 
		0x7179, 
		0x4e00, 
		0xce20, 
		0x5080, 
		0x8000, 
		0x53e0, 
		0x90ff, 
		0x40cf, 
		0x0000, 
		0x7c00, 
		0x4fc0, 
		0x4980, 
		0x706a, 
		0x5000, 
		0x3eb4, 
		0x4500, 
		0x5220, 
		0x5240, 
		0x5300, 
		0x50a0, 
		0x68f7, 
		0xd3a0, 
		0x51a0, 
		0x4ec0, 
		0x4c80, 
		0x5240, 
		0x5240, 
		0x07e2, 
		0x52a0, 
		0x4500, 
		0xd060, 
		0xcc80, 
		0x4980, 
		0x3dab, 
		0x7f85, 
		0x9fb9, 
		0x8000, 
		0x8000, 
		0xd0e0, 
		0x0715, 
		0x8000, 
		0xd320, 
		0xd280, 
		0xc3e8, 
		0x25d3, 
		0x51c0, 
		0xca80, 
		0x4fc0, 
		0xce00, 
		0x5a86, 
		0xa255, 
		0x50a0, 
		0xbe19, 
		0xccc0, 
		0x4f40, 
		0x7c00, 
		0x5340, 
		0x5100, 
		0x4880, 
		0x5360, 
		0x21a7, 
		0xcb80, 
		0x4a80, 
		0xe0bf, 
		0xd140, 
		0x5120, 
		0x51fb, 
		0x8000, 
		0x5240, 
		0x8000, 
		0x53e0, 
		0x4700, 
		0xc200, 
		0x8000, 
		0xf190, 
		0xd240, 
		0x630b, 
		0x7c00, 
		0x01e8, 
		0xc200, 
		0xc400, 
		0x270c, 
		0x10a5, 
		0x703c, 
		0xd794, 
		0x4600, 
		0x5240, 
		0xd380, 
		0x4f40, 
		0xc980, 
		0x50c0, 
		0xd340, 
		0x5020, 
		0x4600, 
		0xd0a0, 
		0x99ef, 
		0xcab7, 
		0x1a31, 
		0x4abd, 
		0x7c00, 
		0xcec0, 
		0xbf8e, 
		0x7c9f, 
		0xd100, 
		0x4c40, 
		0xfd56, 
		0x4d40, 
		0x5380, 
		0x4ec0, 
		0x515a, 
		0xcc40, 
		0x6bee, 
		0x7935, 
		0x0386, 
		0xd120, 
		0xca00, 
		0xfc00, 
		0xcf80, 
		0xfc00, 
		0x4ec0, 
		0x8e1c, 
		0x4600, 
		0xd0e0, 
		0xbc00, 
		0xd320, 
		0xd3c0, 
		0x0225, 
		0xc800, 
		0x5020, 
		0x100b, 
		0x4b00, 
		0xfca6, 
		0x5100, 
		0xce00, 
		0x4c00, 
		0x4c40, 
		0xb73c, 
		0x8000, 
		0xd380, 
		0x775b, 
		0xc800, 
		0x52d0, 
		0x8b7c, 
		0x4cc0, 
		0xd380, 
		0x5100, 
		0x7c00, 
		0x50e0, 
		0x53a0, 
		0x4d8a, 
		0x4d00, 
		0x5360, 
		0xcfc0, 
		0xc980, 
		0xce00, 
		0x0000, 
		0x0000, 
		0xd180, 
		0x8000, 
		0xcc00, 
		0xe56f, 
		0xd200, 
		0xca00, 
		0x717c, 
		0x3702, 
		0x8000, 
		0xcdc0, 
		0x5020, 
		0xd2c0, 
		0x5360, 
		0x5340, 
		0xf0ed, 
		0x7c00, 
		0x7c00, 
		0xd4a8, 
		0x4a00, 
		0xd280, 
		0x4880, 
		0xd360, 
		0xce00, 
		0x50a0, 
		0xd220, 
		0x5160, 
		0xd2a0, 
		0x3c00, 
		0x52c0, 
		0x5420, 
		0x48a4, 
		0x5100, 
		0x4d40, 
		0xcd80, 
		0xd1e0, 
		0x5060, 
		0xb3ce, 
		0xfc00, 
		0x16c4, 
		0xc500, 
		0x50c0, 
		0x4e40, 
		0xcc00, 
		0x0000, 
		0xf8e9, 
		0x8000, 
		0x8000, 
		0xfd81, 
		0xbff8, 
		0x4200, 
		0xfc00, 
		0xe979, 
		0x4c34, 
		0x4a80, 
		0x4600, 
		0xb769, 
		0x4000, 
		0xeda2, 
		0x081d, 
		0xcd00, 
		0x4f00, 
		0x4900, 
		0x5040, 
		0x1059, 
		0x5060, 
		0xc000, 
		0x4f80, 
		0x8000, 
		0xef89, 
		0x4a8c, 
		0xc900, 
		0xf704, 
		0xd240, 
		0x50e0, 
		0x8000, 
		0xd260, 
		0xcc80, 
		0x547b, 
		0xcd00, 
		0xd300, 
		0x5140, 
		0x43fa, 
		0xbef9, 
		0x4b00, 
		0xd220, 
		0xd340, 
		0xdc96, 
		0x870e, 
		0xc500, 
		0x5180, 
		0xce80, 
		0xd180, 
		0xe711, 
		0x4200, 
		0x8000, 
		0x589c, 
		0xfc00, 
		0x3c00, 
		0xa865, 
		0x8000, 
		0xb368, 
		0xd160, 
		0x74f1, 
		0xb6e9, 
		0x4e40, 
		0xcd80, 
		0x4400, 
		0xa16d, 
		0x2a09, 
		0xc400, 
		0xd0e0, 
		0x7c00, 
		0x4e40, 
		0xd934, 
		0x4900, 
		0x4b00, 
		0x7c00, 
		0xc900, 
		0x43b1, 
		0x8000, 
		0x0000, 
		0xc980, 
		0xbc8a, 
		0xca80, 
		0x7c00, 
		0xd2e0, 
		0xc4fe, 
		0x51c0, 
		0xdf17, 
		0x4600, 
		0xcd28, 
		0x5b95, 
		0x0000, 
		0x5080, 
		0x4400, 
		0xca00, 
		0xd120, 
		0x4b00, 
		0xd0c0, 
		0xa94a, 
		0x4600, 
		0x4700, 
		0xcc80, 
		0xc200, 
		0xc980, 
		0xc880, 
		0xc900, 
		0xd9c3, 
		0xfc00, 
		0xd3c0, 
		0x4b80, 
		0x4c00, 
		0x276f, 
		0xd180, 
		0x0000, 
		0xa787, 
		0xd120, 
		0x4200, 
		0x8000, 
		0xccc0, 
		0xd1a0, 
		0x03cc, 
		0xcd80, 
		0x2592, 
		0xd040, 
		0x8000, 
		0xd080, 
		0x0130, 
		0x493d, 
		0x803a, 
		0xb3ae, 
		0xe6d1, 
		0xd3c0, 
		0xfc00, 
		0xd380, 
		0xfc00, 
		0x4c40, 
		0x4d4e, 
		0xd120, 
		0x4cc0, 
		0xca80, 
		0x8000, 
		0x0000, 
		0x0917, 
		0xd200, 
		0xfc00, 
		0xb7f0, 
		0xc500, 
		0xd8a1, 
		0x50a0, 
		0x4f40, 
		0x50e0, 
		0x35b5, 
		0x151b, 
		0x27de, 
		0xe375, 
		0xfc00, 
		0xd200, 
		0xcf6b, 
		0xcc00, 
		0x5300, 
		0x5060, 
		0x4c40, 
		0xd080, 
		0x3733, 
		0xcdf8, 
		0xa388, 
		0x51c0, 
		0xc800, 
		0xd380, 
		0x9857, 
		0x64f6, 
		0xa7c1, 
		0xd3a0, 
		0x50e0, 
		0xc500, 
		0xd3c0, 
		0x5180, 
		0xc400, 
		0xcc80, 
		0xc700, 
		0xfd6f, 
		0x8000, 
		0x5040, 
		0x52c0, 
		0x0000, 
		0xd360, 
		0x21ec, 
		0xcb00, 
		0xd140, 
		0x06fa, 
		0x52a0, 
		0x4c80, 
		0xd2c0, 
		0x51c0, 
		0xccc0, 
		0xe250, 
		0x90b1, 
		0xd260, 
		0x4700, 
		0xd120, 
		0x4880, 
		0xcd40, 
		0xfb25, 
		0x6b1e, 
		0x88e0, 
		0x1159, 
		0x4b80, 
		0x8b7e, 
		0x5060, 
		0x8000, 
		0x5060, 
		0xd160, 
		0x755a, 
		0x4e80, 
		0x4353, 
		0xcf80, 
		0x9b80, 
		0x4e40, 
		0xc900, 
		0x4b00, 
		0xd040, 
		0x4b00, 
		0xcf80, 
		0x8000, 
		0x4600, 
		0xd0a0, 
		0x5080, 
		0xc854, 
		0x51e0, 
		0x3cfc, 
		0x4fc0, 
		0x52a0, 
		0x5360, 
		0x52a0, 
		0xd120, 
		0x8000, 
		0x4900, 
		0x8000, 
		0x4f80, 
		0xc600, 
		0x5100, 
		0xcb80, 
		0xc500, 
		0xd160, 
		0x300f, 
		0xd160, 
		0x50e0, 
		0xd0a0, 
		0x5020, 
		0x52c0, 
		0xb792, 
		0x53c0, 
		0xc500, 
		0x4d00, 
		0x4d40, 
		0x7c00, 
		0xa1ab, 
		0x37b8, 
		0xc980, 
		0x20d0, 
		0x4b80, 
		0x5100, 
		0x5160, 
		0x0000, 
		0x8000, 
		0x4b00, 
		0x53a0, 
		0xbf28, 
		0x0000, 
		0xfdae, 
		0xc000, 
		0x2fe3, 
		0x5260, 
		0xbd6d, 
		0x674c, 
		0x7203, 
		0x4600, 
		0x5100, 
		0x5260, 
		0x4400, 
		0x4e40, 
		0x52e0, 
		0x8000, 
		0x8000, 
		0x4c00, 
		0x1828, 
		0x4900, 
		0xc200, 
		0xd2a0, 
		0xd320, 
		0x52e0, 
		0x5240, 
		0x5100, 
		0x4e80, 
		0xd3c0, 
		0xc33e, 
		0x13f2, 
		0x0000, 
		0xf758, 
		0xd3eb, 
		0xd2c0, 
		0xcf80, 
		0x1222, 
		0x18eb, 
		0x0000, 
		0x5000, 
		0xce40, 
		0x8000, 
		0xd180, 
		0x4c80, 
		0xc700, 
		0x8827, 
		0x4d00, 
		0xc800, 
		0xc200, 
		0x3c00, 
		0xca80, 
		0x52a0, 
		0x2ac7, 
		0xd3a0, 
		0x9a6b, 
		0x5300, 
		0x4ec0, 
		0xeaac, 
		0x22cd, 
		0x5380, 
		0x50e0, 
		0xd260, 
		0xf84e, 
		0x9664, 
		0xd0e0, 
		0xf155, 
		0xcdc0, 
		0x0000, 
		0xd040, 
		0xd0c0, 
		0x4500, 
		0x50a0, 
		0xce40, 
		0xd3a0, 
		0xfcaa, 
		0x53a0, 
		0xd040, 
		0xa99a, 
		0xfdd4, 
		0x4b00, 
		0xd0c0, 
		0xce40, 
		0xd0c0, 
		0xcf40, 
		0x5200, 
		0x51a0, 
		0xd020, 
		0x4980, 
		0x5040, 
		0x8000, 
		0xcf80, 
		0x8731, 
		0xcdc0, 
		0x7294, 
		0xce32, 
		0x4e40, 
		0x1318, 
		0xd200, 
		0xca00, 
		0x51a0, 
		0xd080, 
		0x825a, 
		0xc200, 
		0x4400, 
		0x4d40, 
		0x5260, 
		0x53c0, 
		0xd180, 
		0xd340, 
		0x3ac2, 
		0x0000, 
		0xd043, 
		0xf84a, 
		0x23e4, 
		0x0000, 
		0x5728, 
		0x37db, 
		0x4cc0, 
		0xd280, 
		0xd020, 
		0xd1c0, 
		0x4e40, 
		0x4200, 
		0x5180, 
		0x1153, 
		0xd2a0, 
		0xd360, 
		0x5300, 
		0xffaa, 
		0xd1a0, 
		0x4c00, 
		0xffde, 
		0xd618, 
		0x7c00, 
		0xd160, 
		0xdb82, 
		0xd140, 
		0x4800, 
		0xd78e, 
		0xd160, 
		0x4d00, 
		0xcdc0, 
		0x5240, 
		0x5040, 
		0x7c2a, 
		0xcf00, 
		0xc880, 
		0x83a1, 
		0x5360, 
		0x4f40, 
		0x7c00, 
		0xfc00, 
		0xce40, 
		0x5160, 
		0xd260, 
		0x5080, 
		0x1178, 
		0x2ef4, 
		0xf1a4, 
		0xe29a, 
		0x8000, 
		0xd280, 
		0x4a80, 
		0xfc00, 
		0x5360, 
		0xd060, 
		0xe702, 
		0x5020, 
		0x11d6, 
		0x4000, 
		0xd340, 
		0x4dc0, 
		0x53c0, 
		0xd1c0, 
		0xfc00, 
		0xc880, 
		0x4c80, 
		0xd180, 
		0xd140, 
		0x4e00, 
		0xcbcb, 
		0xca00, 
		0x4fc0, 
		0x5040, 
		0x0048, 
		0xd220, 
		0x4f80, 
		0xd411, 
		0x302c, 
		0x5000, 
		0xd380, 
		0x8a1b, 
		0x4f80, 
		0x9f40, 
		0x5280, 
		0xc880, 
		0x6d47, 
		0xd160, 
		0x7534, 
		0xcc40, 
		0x7c00, 
		0x7474, 
		0xd1a0, 
		0x5060, 
		0x4f40, 
		0xcf00, 
		0xd040, 
		0x5200, 
		0x9f51, 
		0x7c00, 
		0x05c2, 
		0x8cca, 
		0x7dd7, 
		0x170e, 
		0xd3c0, 
		0xc48e, 
		0xd160, 
		0xc74f, 
		0xc980, 
		0x4880, 
		0x8000, 
		0xcfc0, 
		0x9182, 
		0x4000, 
		0x4cc0, 
		0xc700, 
		0xc52e, 
		0xafea, 
		0xd3a0, 
		0x52a0, 
		0xd060, 
		0x4c80, 
		0x49a6, 
		0x52a0, 
		0x5280, 
		0xd1e0, 
		0x77dd, 
		0xc200, 
		0xd149, 
		0xcd00, 
		0xb086, 
		0xa562, 
		0xc486, 
		0xce00, 
		0x5160, 
		0xcf40, 
		0xc000, 
		0x7c9e, 
		0x044b, 
		0xba8a, 
		0x5060, 
		0x5000, 
		0xd340, 
		0xa878, 
		0xcec0, 
		0xa610, 
		0x4db1, 
		0x4cc0, 
		0xd240, 
		0xccc0, 
		0xfb76, 
		0xc800, 
		0x4900, 
		0xd638, 
		0x50e0, 
		0xab87, 
		0xc200, 
		0xd040, 
		0xd0c0, 
		0x3d66, 
		0x5380, 
		0xca80, 
		0x8000, 
		0x6c7c, 
		0xd260, 
		0xd2c0, 
		0x4b00, 
		0xcd00, 
		0xc200, 
		0xd0a0, 
		0x50fb, 
		0x53c0, 
		0x04ee, 
		0xd1a0, 
		0x2175, 
		0x6c7c, 
		0x1392, 
		0x4d80, 
		0x247c, 
		0x50c0, 
		0xc200, 
		0x52a0, 
		0xcdc0, 
		0x4f40, 
		0x4800, 
		0xd1a0, 
		0x5300, 
		0x4907, 
		0x5220, 
		0x5140, 
		0xfc00, 
		0xfc00, 
		0xc980, 
		0x93ec, 
		0xcdc0, 
		0xe146, 
		0x4dc0, 
		0xde03, 
		0xedd5, 
		0x164e, 
		0x8000, 
		0x4d80, 
		0x9fe6, 
		0xbc1b, 
		0x5380, 
		0xcf00, 
		0x53c0, 
		0xcf80, 
		0xfc00, 
		0xd220, 
		0x7345, 
		0x5060, 
		0xfc00, 
		0xfc00, 
		0xc800, 
		0x5320, 
		0x3216, 
		0x8000, 
		0x899b, 
		0x52c0, 
		0x451e, 
		0xcc40, 
		0x53c0, 
		0x595c, 
		0xcb80, 
		0xd3a0, 
		0xce40, 
		0x4fc0, 
		0x51e0, 
		0xd294, 
		0xcd40, 
		0xd0c0, 
		0x5120, 
		0x3c00, 
		0xb4e9, 
		0x5000, 
		0x1f88, 
		0x4c40, 
		0xcf00, 
		0x3636, 
		0x8000, 
		0xd0c0, 
		0x50c0, 
		0x5080, 
		0x7c00, 
		0x0e7c, 
		0xd2c0, 
		0xcf80, 
		0x5140, 
		0x4baf, 
		0x4e00, 
		0x4200, 
		0x4ec0, 
		0x8000, 
		0x6019, 
		0x7c00, 
		0xfc00, 
		0x4d00, 
		0x4d40, 
		0xccc0, 
		0x033e, 
		0xd120, 
		0x0000, 
		0xd260, 
		0x52c0, 
		0x4cc0, 
		0xe411, 
		0x076f, 
		0x7b02, 
		0x4f81, 
		0x8000, 
		0x50a0, 
		0x4499, 
		0xb100, 
		0xcf80, 
		0xd2e0, 
		0xe47a, 
		0xde37, 
		0x5240, 
		0xb64f, 
		0xcc40, 
		0x944b, 
		0xd1c0, 
		0xd0a0, 
		0x8000, 
		0xfda4, 
		0x80d0, 
		0x1c3f, 
		0x6653, 
		0xe85b, 
		0xc9b1, 
		0xb2d6, 
		0xc800, 
		0x0000, 
		0xd2c0, 
		0x8000, 
		0xb79a, 
		0x0b0a, 
		0x50e0, 
		0x742d, 
		0x4f00, 
		0x4e80, 
		0x53e0, 
		0xc900, 
		0xd2c0, 
		0xba07, 
		0xc77c, 
		0x4c40, 
		0xcd5e, 
		0x4700, 
		0x4e41, 
		0x7c00, 
		0x3c00, 
		0x5160, 
		0x4500, 
		0x49ee, 
		0xd080, 
		0xfd8e, 
		0xb341, 
		0x4e80, 
		0xccc0, 
		0x4a80, 
		0x4ec1, 
		0x5bc8, 
		0x4f40, 
		0x52e0, 
		0xefcd, 
		0x5340, 
		0xcf80, 
		0x4980, 
		0x2071, 
		0x51e0, 
		0x6863, 
		0x88fe, 
		0x1d89, 
		0x4e80, 
		0xd0e0, 
		0x4e80, 
		0x3489, 
		0x4ec0, 
		0xc200, 
		0x52c0, 
		0x75fc, 
		0xd3a0, 
		0x4c00, 
		0xd1c0, 
		0x5160, 
		0x7bb7, 
		0xd340, 
		0xca80, 
		0xcc40, 
		0xb9e2, 
		0x5300, 
		0xd040, 
		0xfc00, 
		0xd280, 
		0x4980, 
		0x4000, 
		0x4a80, 
		0xcf00, 
		0xfadb, 
		0x51c0, 
		0x47e7, 
		0xd3b1, 
		0x5380, 
		0x4400, 
		0xd000, 
		0x8000, 
		0x3fc9, 
		0x5000, 
		0x5140, 
		0xddd1, 
		0x7c00, 
		0x4d40, 
		0x0000, 
		0xd360, 
		0x53c0, 
		0x52c0, 
		0x3c00, 
		0xd040, 
		0xcb80, 
		0x4dc0, 
		0x52e0, 
		0xcd80, 
		0x411b, 
		0xc000, 
		0xcdc0, 
		0xc245, 
		0xd0c0, 
		0xd160, 
		0x108b, 
		0xecf8, 
		0x4d40, 
		0x1dd4, 
		0x4800, 
		0xce00, 
		0x0000, 
		0x69b6, 
		0xcbf9, 
		0xfc00, 
		0xd3c0, 
		0x4e80, 
		0xcb00, 
		0xe015, 
		0xcfc0, 
		0x8000, 
		0xd240, 
		0xce40, 
		0x51e0, 
		0x0d23, 
		0x272b, 
		0xd360, 
		0xe9fa, 
		0xcb80, 
		0xe1ee, 
		0xfa00, 
		0xfc00, 
		0x5360, 
		0x5632, 
		0xd260, 
		0xc6ef, 
		0x4e00, 
		0x5260, 
		0x7c23, 
		0x4600, 
		0x0957, 
		0xfc00, 
		0x03c0, 
		0xd160, 
		0xa24a, 
		0xd220, 
		0x75c9, 
		0xcc00, 
		0x5380, 
		0x0000, 
		0x4600, 
		0x1a7b, 
		0x5866, 
		0xd060, 
		0x0ff7, 
		0xb0cb, 
		0xc109, 
		0x50a0, 
		0xd360, 
		0xca80, 
		0x4500, 
		0xd1c0, 
		0xd040, 
		0xfc00, 
		0x4f00, 
		0x7c00, 
		0x1047, 
		0x4980, 
		0xfc00, 
		0x53a0, 
		0xd120, 
		0xd240, 
		0xaa23, 
		0xc500, 
		0xd100, 
		0xd240, 
		0x8000, 
		0x8000, 
		0x8000, 
		0x40d9, 
		0x4520, 
		0x858c, 
		0xc366, 
		0x9b36, 
		0x5200, 
		0x4500, 
		0xdca8, 
		0xd2c0, 
		0x8000, 
		0xcf00, 
		0x8000, 
		0xfd4f, 
		0xcc80, 
		0x4d40, 
		0x7ccf, 
		0x9d4c, 
		0x52c0, 
		0x5120, 
		0xce40, 
		0xce80, 
		0x4800, 
		0xd100, 
		0x5160, 
		0xc980, 
		0xcc80, 
		0xcc00, 
		0xd0a0, 
		0xd140, 
		0xcf40, 
		0xc475, 
		0xf046, 
		0xd2e0, 
		0x5320, 
		0x5a0e, 
		0xe94e, 
		0x4fc0, 
		0x9bb1, 
		0xcf40, 
		0xfc00, 
		0x7c00, 
		0xcf00, 
		0xc880, 
		0x5120, 
		0xde96, 
		0x4200, 
		0x4400, 
		0xcdc0, 
		0xd200, 
		0x5120, 
		0x905d, 
		0x2d91, 
		0x0511, 
		0x4e00, 
		0x4dc0, 
		0xcb80, 
		0xc200, 
		0x8000, 
		0x4a00, 
		0xd23c, 
		0xaa63, 
		0x086e, 
		0xd000, 
		0x5aa4, 
		0x8000, 
		0xc57a, 
		0xd16e, 
		0x43d6, 
		0x51e0, 
		0x863e, 
		0x0000, 
		0xbbb5, 
		0xd2e0, 
		0x58ff, 
		0x97c9, 
		0x4600, 
		0x0b2f, 
		0xd3c0, 
		0xf53f, 
		0x5340, 
		0x7104, 
		0xfc00, 
		0xd000, 
		0x51a0, 
		0xcec0, 
		0x5220, 
		0x8000, 
		0xd3e0, 
		0x81fe, 
		0x4dc0, 
		0xcf80, 
		0x3c00, 
		0x59ef, 
		0x4d00, 
		0x5cb2, 
		0x4d40, 
		0x5180, 
		0x89a6, 
		0x6d36, 
		0xfc00, 
		0x4500, 
		0x4d80, 
		0xbc00, 
		0x3c00, 
		0x2c85, 
		0x0505, 
		0x4c40, 
		0x4e80, 
		0x0dd2, 
		0x5280, 
		0x8000, 
		0x0501, 
		0x5100, 
		0x4880, 
		0xf1ae, 
		0xe677, 
		0xb6ae, 
		0x8000, 
		0x5260, 
		0x52c0, 
		0xcf40, 
		0x4a80, 
		0x5020, 
		0x4e40, 
		0x7e8f, 
		0xd2e0, 
		0x53c0, 
		0xd360, 
		0x5220, 
		0xd040, 
		0xd260, 
		0x8000, 
		0xbd5a, 
		0x5180, 
		0x297b, 
		0x0000, 
		0x17be, 
		0xfd1a, 
		0xfc00, 
		0x7c00, 
		0xfc00, 
		0x51a0, 
		0x4c00, 
		0x4dc0, 
		0xfd17, 
		0x5280, 
		0x1151, 
		0x5120, 
		0xd080, 
		0xc800, 
		0x4880, 
		0xd0a0, 
		0x5040, 
		0x5380, 
		0xd3c0, 
		0x0000, 
		0xd3e0, 
		0x1e2c, 
		0x4a7b, 
		0x4980, 
		0xd140, 
		0x5140, 
		0x4e80, 
		0xcfc0, 
		0x8000, 
		0x0000, 
		0xd080, 
		0x4f80, 
		0xcb80, 
		0x50c0, 
		0xc600, 
		0x4c40, 
		0x0000, 
		0xce40, 
		0x4cc0, 
		0x4ec0, 
		0xcf80, 
		0x4258, 
		0x4e00, 
		0x8881, 
		0xcc40, 
		0xcd00, 
		0x4023, 
		0xcfc0, 
		0xfc00, 
		0x4e80, 
		0xadf6, 
		0xfc00, 
		0x4000, 
		0xbe59, 
		0xcec0, 
		0x4a80, 
		0x4d00, 
		0x0000, 
		0x4f00, 
		0x5280, 
		0xccc3, 
		0xeda5, 
		0x5260, 
		0xd200, 
		0xc600, 
		0x6f56, 
		0xdc71, 
		0xfc00, 
		0x53e0, 
		0x4e40, 
		0xb459, 
		0xd200, 
		0x4e00, 
		0xd2a0, 
		0x5140, 
		0xcf80, 
		0xd100, 
		0x6b6d, 
		0x53e0, 
		0xc600, 
		0x9432, 
		0xe2ae, 
		0xb4c2, 
		0x53e0, 
		0x51af, 
		0x3182, 
		0xaf9b, 
		0x4fc0, 
		0x4d40, 
		0x3901, 
		0x80d5, 
		0xd300, 
		0x5220, 
		0x4500, 
		0x4000, 
		0xd220, 
		0x4d80  
};
volatile uint32_t rand_ieee754_32[1536] __attribute__ ((aligned (2048))) = {
		0xf073e816, 
		0x00400000, 
		0x0c7fffe0, 
		0x00008000, 
		0x0c7fff80, 
		0x4401ac03, 
		0xc1c00000, 
		0xc2140000, 
		0x8ff86d0a, 
		0x80000008, 
		0xc2040000, 
		0x2aae1391, 
		0x0c7ffff0, 
		0x0e007fff, 
		0x80080000, 
		0x80000000, 
		0x00008000, 
		0x6e190d3b, 
		0x41b80000, 
		0x483c663f, 
		0x4b8c4b40, 
		0xe27e9cec, 
		0xc21c0000, 
		0xc1e80000, 
		0x0f54f254, 
		0xc2340000, 
		0x8ab2a7ec, 
		0x80800003, 
		0x00000080, 
		0x80000000, 
		0xff000000, 
		0x7f9c190b, 
		0xe27512be, 
		0x0c7ffff0, 
		0xffc00001, 
		0xaa2dc5c1, 
		0x0f7fffff, 
		0x00000001, 
		0x00000800, 
		0x00000020, 
		0x8c2f2013, 
		0x00000000, 
		0xc4eb8f0a, 
		0x00000002, 
		0x80800000, 
		0xbdb5dc34, 
		0x41e80000, 
		0x7fc00000, 
		0x80800003, 
		0x80000002, 
		0xa5dba250, 
		0xc1f80000, 
		0x2c662d3b, 
		0x5fa5b830, 
		0x80000020, 
		0xc2640000, 
		0xc1300000, 
		0x0c7ffc00, 
		0x00000001, 
		0x425c0000, 
		0x41880000, 
		0x00000000, 
		0xc2780000, 
		0x1f19a822, 
		0x0c7fff80, 
		0x00001000, 
		0xcccccccc, 
		0x7f800000, 
		0x80000800, 
		0x12b62b31, 
		0x59eafb98, 
		0x00200000, 
		0x0f7ffffe, 
		0x42680000, 
		0x3f800001, 
		0x3ff33711, 
		0x00000000, 
		0x42580000, 
		0x00040000, 
		0x00100000, 
		0x806d55bf, 
		0x28b377b0, 
		0x00000000, 
		0x00000000, 
		0x0e07ffff, 
		0x00000010, 
		0xff800000, 
		0xff000000, 
		0xc2400000, 
		0x42300000, 
		0x80000002, 
		0x41700000, 
		0x00800002, 
		0x00000000, 
		0x1fc9fc5a, 
		0xc2580000, 
		0x7f800000, 
		0xff800000, 
		0x7f7fffff, 
		0x80000000, 
		0xc1b80000, 
		0x423c0000, 
		0x0d000ff0, 
		0x80000040, 
		0xff7fffff, 
		0x7f800001, 
		0x80000000, 
		0x00000000, 
		0x41100000, 
		0xe588df01, 
		0x42380000, 
		0xeecf80d8, 
		0x42380000, 
		0xc1b80000, 
		0x8a0c1c78, 
		0xff800000, 
		0xc1c80000, 
		0x4e6960d2, 
		0xd456b96d, 
		0x40e00000, 
		0x00800001, 
		0x41500000, 
		0x0c7fffe0, 
		0x00000200, 
		0xc1500000, 
		0x0cbcfb09, 
		0x686bf494, 
		0xff800000, 
		0xc2280000, 
		0xc1c00000, 
		0x0e0001ff, 
		0x80000000, 
		0x7f800000, 
		0x309f6b7a, 
		0x01a88897, 
		0x426c0000, 
		0x0e001fff, 
		0x80000040, 
		0x5e76db10, 
		0x42500000, 
		0x41d00000, 
		0x00800001, 
		0x41980000, 
		0xa807ce87, 
		0x83d47cd3, 
		0x292dd55a, 
		0x0f7ffffd, 
		0xc1700000, 
		0x3f800000, 
		0xc0400000, 
		0x6b9a2527, 
		0xcb8c4b40, 
		0xaaaaaaaa, 
		0x00000004, 
		0x80000000, 
		0x7f7fffff, 
		0x2d9de9c1, 
		0x421c0000, 
		0xc1d80000, 
		0x8006c480, 
		0xc2380000, 
		0x80000001, 
		0x00200000, 
		0x0b9ef22f, 
		0xc0400000, 
		0x80800002, 
		0xc2400000, 
		0x00210d38, 
		0x00800000, 
		0x41900000, 
		0xc24c0000, 
		0x80000000, 
		0x997d864e, 
		0x0c7ff800, 
		0xa269be76, 
		0xcccccccc, 
		0x1e199ba3, 
		0x80800000, 
		0x00010000, 
		0x0a7b29d8, 
		0xbf800000, 
		0x41700000, 
		0x41d80000, 
		0x179df226, 
		0x29412d98, 
		0x32809482, 
		0xe407da67, 
		0x0e0001ff, 
		0x425c0000, 
		0x0d00fff0, 
		0x80080000, 
		0x80000001, 
		0x7f800001, 
		0x0e00000f, 
		0xfc5e62b0, 
		0x80000000, 
		0x80000000, 
		0x42780000, 
		0x98fc0453, 
		0x3af78e12, 
		0x26190991, 
		0xa42bc88c, 
		0xe36d102c, 
		0x0f7ffffe, 
		0xa6b020d6, 
		0xd2023788, 
		0x8f7ffffe, 
		0x80000000, 
		0x45d7c5eb, 
		0x7f800000, 
		0x00800000, 
		0x00001000, 
		0xc2740000, 
		0x9890e69e, 
		0x0d00fff0, 
		0xea050d26, 
		0xf12a9cf5, 
		0x0e000fff, 
		0x7f800000, 
		0xc1900000, 
		0x41000000, 
		0xd2a3408c, 
		0x006d6273, 
		0xc1800000, 
		0x426c0000, 
		0x42340000, 
		0x0c780000, 
		0x42640000, 
		0xc0800000, 
		0x80010000, 
		0x80010000, 
		0x80000004, 
		0xc2300000, 
		0xff7ffffe, 
		0x0c400000, 
		0xff9a0481, 
		0xc1100000, 
		0x8e4c29c3, 
		0x7f800000, 
		0x80800002, 
		0x60d56783, 
		0x80000000, 
		0xff800000, 
		0x80800001, 
		0x7fd7731a, 
		0x5998bd51, 
		0x0c7ffe00, 
		0x55039817, 
		0xc1500000, 
		0xff904b39, 
		0x41f00000, 
		0x80000040, 
		0xfcaea9e2, 
		0xc1a00000, 
		0x59794fe9, 
		0x0c7ffe00, 
		0x4af298f6, 
		0x00010000, 
		0x807fffff, 
		0x807ffffe, 
		0xc92f5197, 
		0x42180000, 
		0x41b80000, 
		0xaeded3b8, 
		0xffabdc38, 
		0xc0800000, 
		0x0c7e0000, 
		0x2be38825, 
		0x7f800000, 
		0x40a00000, 
		0x41e80000, 
		0x3479832d, 
		0x80001000, 
		0x0b22b0c6, 
		0x7f96b5d5, 
		0xc1b80000, 
		0xff7ffffe, 
		0x0c7f8000, 
		0xebab0b6c, 
		0x807fffff, 
		0xc1b80000, 
		0x42300000, 
		0x80800001, 
		0x7f7fffff, 
		0xb17ac84b, 
		0x40e00000, 
		0x41b80000, 
		0x80000080, 
		0x41300000, 
		0x0c7ffffe, 
		0x0f7ffffe, 
		0x0e0000ff, 
		0x807ffffe, 
		0xc8bbc30f, 
		0x80000400, 
		0x84c262ce, 
		0xc2340000, 
		0x00000000, 
		0x00400000, 
		0xc2100000, 
		0x00000008, 
		0xb05561e0, 
		0xa0362537, 
		0x80800001, 
		0xd8c9d70f, 
		0x0a4f8bdb, 
		0xc1e00000, 
		0xc1600000, 
		0x42340000, 
		0x80000000, 
		0x42300000, 
		0x41880000, 
		0x134e5022, 
		0xffffffff, 
		0x0c7e0000, 
		0x0c7ffff8, 
		0x80000010, 
		0x0e7fffff, 
		0x00800001, 
		0xc25c0000, 
		0xdf1981cc, 
		0x32230a91, 
		0x80000000, 
		0xc2680000, 
		0x0e00003f, 
		0x9563ec9b, 
		0x80800000, 
		0x80000000, 
		0x80080000, 
		0x73092b59, 
		0x80008000, 
		0x0c7ff000, 
		0xa883c3c8, 
		0x0c7ff800, 
		0x00800001, 
		0x0c7fe000, 
		0xb519e6b0, 
		0xff800000, 
		0xbf028f5c, 
		0xb9fd8def, 
		0xc2680000, 
		0x0e000001, 
		0x8f7ffffc, 
		0xa6932714, 
		0x41800000, 
		0x33336c09, 
		0xc2680000, 
		0x41400000, 
		0xff800000, 
		0xbd48d317, 
		0xb0011d78, 
		0x80000040, 
		0x6b05a0f5, 
		0x00000400, 
		0x42280000, 
		0xbf800000, 
		0x42040000, 
		0x7fb04613, 
		0xc0400000, 
		0xc1900000, 
		0x6fe2a22a, 
		0x41d80000, 
		0x7fffffff, 
		0x42300000, 
		0x42100000, 
		0x00800000, 
		0x80000000, 
		0xadb6435b, 
		0x8f873e28, 
		0x0e1fffff, 
		0x80000800, 
		0x7f800000, 
		0x80000000, 
		0x80299230, 
		0x55555555, 
		0xc2580000, 
		0x40e00000, 
		0xe3d2281d, 
		0x00010000, 
		0x41600000, 
		0x180400f4, 
		0xe8a336c9, 
		0x80000000, 
		0x80000000, 
		0x8a4a031e, 
		0x80009a34, 
		0x98e77215, 
		0xffbfffff, 
		0x0c7ffffe, 
		0xc2680000, 
		0x5ff2f8c7, 
		0xff7ffffe, 
		0x8f7ffffc, 
		0x420c0000, 
		0xc26c0000, 
		0xff7ffffe, 
		0x42440000, 
		0x80010000, 
		0xb3184861, 
		0x7a872b05, 
		0xc0800000, 
		0xff800000, 
		0x2f64fd6f, 
		0x00800003, 
		0x1b7c9bd6, 
		0x1a41e604, 
		0x4888f540, 
		0x00000000, 
		0x1cc67ba8, 
		0x00000100, 
		0xff7ffffe, 
		0x80000000, 
		0x0c400000, 
		0xc2200000, 
		0x3f800001, 
		0x426c0000, 
		0xa1b16b0b, 
		0xc20c0000, 
		0x91b48b7f, 
		0x007fffff, 
		0x422c0000, 
		0x6ceea666, 
		0x00000000, 
		0xff7fffff, 
		0xce6e4226, 
		0x00000000, 
		0x09e0a70a, 
		0x80040000, 
		0x3b1bb9af, 
		0xff800000, 
		0xc1b80000, 
		0xa1139065, 
		0x00000020, 
		0x00011111, 
		0x7f800000, 
		0x0e3fffff, 
		0x80800002, 
		0x41880000, 
		0x427c0000, 
		0x7fc00000, 
		0x49a92546, 
		0x7fc00001, 
		0x471d9372, 
		0x42480000, 
		0xc1c80000, 
		0xc2380000, 
		0x427c0000, 
		0xbf028f5c, 
		0x0c7ffc00, 
		0xf25d4dd3, 
		0x8f7ffffe, 
		0x1e0e9cf2, 
		0xc1e00000, 
		0x42540000, 
		0x00800001, 
		0xffbfffff, 
		0x0c780000, 
		0x6ffa12f8, 
		0x007ffffe, 
		0xf25da3ab, 
		0x69f71cdc, 
		0x00800002, 
		0x0e1fffff, 
		0xbcdfc0dc, 
		0x0e007fff, 
		0xcccccccc, 
		0x7fc00001, 
		0xffaaf16c, 
		0xc2240000, 
		0xbfe7db1c, 
		0x0c7fffe0, 
		0x41000000, 
		0x2ea1472d, 
		0xc2500000, 
		0x00800001, 
		0x7f7ffffe, 
		0xeb0ef20d, 
		0x3e48c0d2, 
		0xb932653e, 
		0xb6b9b5a1, 
		0x00000400, 
		0x8c03b602, 
		0xcd466165, 
		0xc2180000, 
		0x87dc87cc, 
		0x41300000, 
		0x00400000, 
		0xec87b246, 
		0x80000200, 
		0xc1600000, 
		0xc1e00000, 
		0x00000040, 
		0x7f800000, 
		0x032eeafd, 
		0x26f35770, 
		0x00000000, 
		0x0e000001, 
		0x42640000, 
		0x3014cbeb, 
		0x0e000001, 
		0x0f7ffffc, 
		0x41d80000, 
		0x1417ab38, 
		0x7f7ffffe, 
		0xc2080000, 
		0x7cdbd375, 
		0x292c81ae, 
		0x0c7ffe00, 
		0xf53abc9e, 
		0x81376bde, 
		0xc1800000, 
		0xf5841d4b, 
		0x007ffffe, 
		0x544aa728, 
		0x3e016c92, 
		0xb14ef06f, 
		0x00004000, 
		0x0e00003f, 
		0x42200000, 
		0x00ffffff, 
		0x3f800000, 
		0xa357b3c2, 
		0x0c400000, 
		0x42480000, 
		0x0c400000, 
		0xff800000, 
		0xc1b00000, 
		0x00000080, 
		0x33333333, 
		0x7f7ffffe, 
		0x00000000, 
		0x04469081, 
		0x41f80000, 
		0xc1b00000, 
		0xcb000000, 
		0x41d80000, 
		0x0e00000f, 
		0x80000800, 
		0x41e00000, 
		0x00800002, 
		0x9dc98469, 
		0xe3566394, 
		0xc24c0000, 
		0x142de4a6, 
		0x6f30d1b4, 
		0x0c400000, 
		0x7f800000, 
		0x00004000, 
		0x00000000, 
		0x0e001fff, 
		0xc2a15eda, 
		0x40c00000, 
		0x80011111, 
		0xc2140000, 
		0xb4ae4e9e, 
		0xc1e00000, 
		0x7996d88c, 
		0x00011111, 
		0x27b4cdcf, 
		0x42500000, 
		0x00000000, 
		0x40000000, 
		0xa433e52a, 
		0xc1a00000, 
		0x33333333, 
		0xe6a49ca5, 
		0x80800000, 
		0x42000000, 
		0x0c600000, 
		0xab84f7f7, 
		0x0c7ffe00, 
		0x02869792, 
		0x80002000, 
		0xc21c0000, 
		0xc2440000, 
		0xff800000, 
		0x00800001, 
		0x26745751, 
		0xe00e2da0, 
		0x1bc36884, 
		0x80000000, 
		0x830ea2be, 
		0x619303e4, 
		0xffa9717a, 
		0xa8fc9bc3, 
		0xc2000000, 
		0x0c780000, 
		0x00000008, 
		0x40000000, 
		0xc1100000, 
		0xc2540000, 
		0x00800003, 
		0x005a611a, 
		0x7fc00001, 
		0x6beb6fc0, 
		0x41e00000, 
		0x417a138e, 
		0xff800000, 
		0xaaaaaaaa, 
		0x00011111, 
		0x42580000, 
		0x42300000, 
		0x836ec90e, 
		0x7f800000, 
		0x4561aecb, 
		0xc1f00000, 
		0xfe08b1d7, 
		0x00000000, 
		0xbf028f5c, 
		0x00000000, 
		0x060181dc, 
		0x00000000, 
		0x84934b35, 
		0xffa1b4df, 
		0xff800000, 
		0x4b8c4b40, 
		0x00400000, 
		0x00800003, 
		0x0c7e0000, 
		0xc1800000, 
		0x807fffff, 
		0x936be677, 
		0x007ffffe, 
		0x41100000, 
		0x00000000, 
		0x8b95bcb7, 
		0xb1f38ac8, 
		0x99244853, 
		0x00011111, 
		0x80000000, 
		0x00001000, 
		0x0c7f8000, 
		0x7f800000, 
		0x80000000, 
		0xc2240000, 
		0x7fffffff, 
		0x80000080, 
		0xff800001, 
		0x80000020, 
		0x42440000, 
		0x00400000, 
		0x21b271e3, 
		0x80800000, 
		0x0c7fff80, 
		0x80800003, 
		0x42240000, 
		0xc0e00000, 
		0x7fffffff, 
		0x0e00000f, 
		0x0038ad14, 
		0x8ea77aab, 
		0x0e007fff, 
		0xc1e80000, 
		0xa05f6608, 
		0x41400000, 
		0xef1a5edb, 
		0xc5a7a545, 
		0x7fc00000, 
		0x00724241, 
		0xc2340000, 
		0x0c600000, 
		0x9c431e4b, 
		0x00001000, 
		0xff800000, 
		0x307f30ab, 
		0x0c7fffc0, 
		0x3f800000, 
		0x420c0000, 
		0x00800000, 
		0x4524e971, 
		0xbf028f5c, 
		0x22279039, 
		0x4b000000, 
		0x7fc00000, 
		0x00000000, 
		0xc2540000, 
		0x0c7ff800, 
		0x0c7fff80, 
		0x804208b4, 
		0x00002000, 
		0x007fffff, 
		0xc2100000, 
		0x80000000, 
		0xc2740000, 
		0xb870a953, 
		0xc2600000, 
		0x801351b5, 
		0x1035c0c5, 
		0x007fffff, 
		0x42500000, 
		0x3b2cc720, 
		0x235b92a6, 
		0x40400000, 
		0x807fffff, 
		0xc2380000, 
		0xe07ad6f2, 
		0x7fffffff, 
		0x42640000, 
		0x80800000, 
		0xff949f80, 
		0x42100000, 
		0xe01424e4, 
		0x00000100, 
		0x8c43231d, 
		0x0e000001, 
		0x0e00001f, 
		0x0e07ffff, 
		0x7cdc9b22, 
		0xff7ffffe, 
		0x80000000, 
		0x00004000, 
		0x444bbd32, 
		0xbf800000, 
		0xcc1e3e1a, 
		0x00020000, 
		0x7fc00001, 
		0x423c0000, 
		0xee7a4fc6, 
		0x333bc280, 
		0xeee20391, 
		0x55ad9506, 
		0x80001000, 
		0xc2500000, 
		0x0c700000, 
		0xff7ffffe, 
		0x41800000, 
		0x80000100, 
		0x41800000, 
		0x7fad988d, 
		0xc2400000, 
		0x00400000, 
		0x0c7f0000, 
		0x40e00000, 
		0xb8dbb810, 
		0x426c0000, 
		0x8f7ffffe, 
		0xd5b81182, 
		0xc9a0db02, 
		0x80800003, 
		0x00000010, 
		0x7028abd7, 
		0xb807ad12, 
		0x42400000, 
		0x0e000007, 
		0x6e8e8d37, 
		0x7fae713f, 
		0xc19189e3, 
		0x33779614, 
		0x00000004, 
		0x0e003fff, 
		0x80100000, 
		0xc2780000, 
		0x80000000, 
		0x0c7fffff, 
		0xff7fffff, 
		0x41880000, 
		0x0c7ffffe, 
		0x80000000, 
		0x0e7fffff, 
		0x80000010, 
		0x103b1ef3, 
		0x7f7fffff, 
		0x8c5b34d9, 
		0x0e001fff, 
		0x0c600000, 
		0x80800003, 
		0x7fac9956, 
		0x08ec676f, 
		0x906b16cb, 
		0x00000000, 
		0x0e0fffff, 
		0x00000008, 
		0xec87639a, 
		0x241e23c3, 
		0x0c780000, 
		0x41b80000, 
		0x80000001, 
		0xff800000, 
		0xf0c8c24e, 
		0x00000200, 
		0x7f40c2d7, 
		0xc2640000, 
		0x28627363, 
		0x00000001, 
		0x3c26e6b0, 
		0x7fc00001, 
		0x6156f723, 
		0xc1e80000, 
		0x00000000, 
		0x41100000, 
		0x7f800000, 
		0xfeb6bda4, 
		0x4b000000, 
		0x8792354f, 
		0xc2040000, 
		0x80800003, 
		0x41c00000, 
		0xc2500000, 
		0x41400000, 
		0xc20c0000, 
		0x28dca0af, 
		0xd7ae6965, 
		0xffa6d7dd, 
		0x80000000, 
		0x00000002, 
		0x00020000, 
		0xc2480000, 
		0xc2140000, 
		0x80000000, 
		0x5ad7f6b0, 
		0x80800001, 
		0x80004000, 
		0x00800001, 
		0xc2380000, 
		0x49efb17a, 
		0xcef0cae3, 
		0x80000000, 
		0x0f7ffffd, 
		0x00000001, 
		0x0c780000, 
		0x7f800000, 
		0x42700000, 
		0xc24c0000, 
		0x41500000, 
		0x0e03ffff, 
		0xff800000, 
		0x0c7ffffc, 
		0x18fb72bb, 
		0x41980000, 
		0xbace08c2, 
		0xc23c0000, 
		0x80000002, 
		0x0c7fffe0, 
		0xffc00001, 
		0x7fbac574, 
		0xff800000, 
		0x42540000, 
		0x1676e799, 
		0x5ff81652, 
		0x00000040, 
		0x80000000, 
		0xc0c00000, 
		0x7f800000, 
		0x42340000, 
		0x80000000, 
		0x0c7ff800, 
		0x80000400, 
		0x80ffffff, 
		0xffabd1ff, 
		0x2b09c92f, 
		0xbf028f5c, 
		0xc2400000, 
		0x0c7fffc0, 
		0x0e0001ff, 
		0x0e0000ff, 
		0x80000400, 
		0x55555555, 
		0x40800000, 
		0x40000000, 
		0x9fa87f9b, 
		0x7f8eae09, 
		0x576aec11, 
		0x80000008, 
		0x80000000, 
		0x00002000, 
		0x33333333, 
		0x00000020, 
		0xf7338cda, 
		0x42280000, 
		0x00002000, 
		0x7f800000, 
		0xc1300000, 
		0x3f800001, 
		0xcb79d063, 
		0x41400000, 
		0xc1f80000, 
		0xff8fb471, 
		0x7f800000, 
		0xffdf0d6a, 
		0xffd59e18, 
		0x422c0000, 
		0x80000010, 
		0x40e00000, 
		0xc1100000, 
		0x80800003, 
		0x7fffffff, 
		0x4376bb45, 
		0xc2180000, 
		0x7fbfffff, 
		0x41f00000, 
		0xffb025a5, 
		0xd7aa3c1c, 
		0xbad370e2, 
		0x0e01ffff, 
		0x0e000001, 
		0x00200000, 
		0x00000000, 
		0x65a986f5, 
		0x2a8970f2, 
		0x00040000, 
		0xcb8c4b40, 
		0x41700000, 
		0x80011111, 
		0x42000000, 
		0xbf800000, 
		0x427c0000, 
		0x80011111, 
		0x4b71e9b3, 
		0xff800000, 
		0x3f800001, 
		0xc1d80000, 
		0xc1d00000, 
		0xbf028f5c, 
		0x0e000007, 
		0xc2200000, 
		0x7376e9ef, 
		0x00000000, 
		0x00000000, 
		0x80ffffff, 
		0x00000002, 
		0x72e89928, 
		0x00010000, 
		0x00000000, 
		0x007ffffe, 
		0x2b4411dc, 
		0x80000000, 
		0xc26c0000, 
		0x7f800000, 
		0x0e07ffff, 
		0xc2140000, 
		0x7fc00001, 
		0x807fffff, 
		0x00ffffff, 
		0x2d004bba, 
		0xfc111fc1, 
		0x994a03bb, 
		0x424c0000, 
		0x57be39b8, 
		0x3bc78a39, 
		0x841e6d05, 
		0x0c700000, 
		0xf1359d8b, 
		0x1931acc6, 
		0x00675345, 
		0xc2140000, 
		0x00000000, 
		0x41200000, 
		0xa2908493, 
		0xcccccccc, 
		0xc1980000, 
		0x0c7fffc0, 
		0xa3fb99b2, 
		0x0c400000, 
		0x0c7c0000, 
		0x42640000, 
		0x6b9561a8, 
		0x80000080, 
		0x42200000, 
		0x80000000, 
		0x8f523042, 
		0x0e000003, 
		0xedfed25b, 
		0x67b1da77, 
		0xfd0cc700, 
		0xc22c0000, 
		0x01156f4b, 
		0x0c7ff000, 
		0xc813406d, 
		0xc2000000, 
		0x0c400000, 
		0x68ab7630, 
		0x42640000, 
		0xc24c0000, 
		0xff800001, 
		0xc2500000, 
		0x0e0000ff, 
		0x7f800000, 
		0xff7fffff, 
		0x3f1f27df, 
		0x0e0003ff, 
		0xffffffff, 
		0xff7ffffe, 
		0x00200000, 
		0x8d7387bb, 
		0x42640000, 
		0xd8d2e06b, 
		0x80000000, 
		0xc2100000, 
		0x00000000, 
		0x0c7fffc0, 
		0x0e03ffff, 
		0x4bcd5b59, 
		0x80800002, 
		0x41c00000, 
		0xc1b00000, 
		0x7fbe5b11, 
		0x0c7ffff8, 
		0x80000000, 
		0x87e7b193, 
		0x0d000ff0, 
		0x7f800000, 
		0xc20c0000, 
		0x7f800000, 
		0xac50aa2b, 
		0xbf028f5c, 
		0x7ea1903b, 
		0x8f7ffffd, 
		0x41300000, 
		0x1d9fcc2b, 
		0x80011111, 
		0x7f7ffffe, 
		0xffbfffff, 
		0x3f800000, 
		0x0e003fff, 
		0x9b5ecdac, 
		0x807fffff, 
		0x00011111, 
		0xc20c0000, 
		0xc22c0000, 
		0x00000020, 
		0x7fabbb93, 
		0xff8f8530, 
		0xc2640000, 
		0xef0a4260, 
		0x9ce61103, 
		0x25072758, 
		0x0c7ffe00, 
		0xc1300000, 
		0x32dead03, 
		0x34d98aa2, 
		0x7fc00001, 
		0x94d44db9, 
		0xc2340000, 
		0x42340000, 
		0x0c7fff00, 
		0x0e01ffff, 
		0x0c7fc000, 
		0x0e7fffff, 
		0x71de04d6, 
		0x86ccd1d0, 
		0x0c400000, 
		0xd4d3b75e, 
		0x7f800000, 
		0xff800000, 
		0xff800000, 
		0x7f7fffff, 
		0x5aee82df, 
		0xffc00001, 
		0x41800000, 
		0x0c7fe000, 
		0xc1e00000, 
		0x80800002, 
		0x42280000, 
		0x7fc00001, 
		0x0c7fffc0, 
		0x0e0000ff, 
		0x80010000, 
		0xc2640000, 
		0xc1300000, 
		0x00800000, 
		0xff800000, 
		0x7f9617d2, 
		0x42700000, 
		0xff800000, 
		0x420c0000, 
		0xc1e00000, 
		0x41858d74, 
		0xa927d900, 
		0xff7fffff, 
		0xcd334dea, 
		0x002a7875, 
		0xd70c2582, 
		0x0007f913, 
		0x42180000, 
		0x42600000, 
		0x00000000, 
		0x421c0000, 
		0xff800000, 
		0x7f800000, 
		0x00000080, 
		0xc1c80000, 
		0xc8ec4e65, 
		0x00800000, 
		0x00011111, 
		0x42340000, 
		0x13e643c1, 
		0xc2140000, 
		0x80000080, 
		0x0012d6d7, 
		0xddbdb23d, 
		0xba777a65, 
		0x0c7fff00, 
		0x007fffff, 
		0x0cf12d54, 
		0x0e0fffff, 
		0x42740000, 
		0x804c85a6, 
		0x828583ce, 
		0x00000000, 
		0x4e01f85e, 
		0x0cb576f8, 
		0x807ffffe, 
		0xff7ffffe, 
		0xbf800000, 
		0x0f7ffffc, 
		0x60eab3c0, 
		0x80800002, 
		0xa6ab5ccd, 
		0xc0800000, 
		0x80000000, 
		0x0e007fff, 
		0x664b9acd, 
		0xc1880000, 
		0x80000000, 
		0x0e07ffff, 
		0x42080000, 
		0x7f800000, 
		0xff2c2a20, 
		0x0e000003, 
		0x00040000, 
		0x80000000, 
		0x41880000, 
		0x2edbd25c, 
		0xff7ffffe, 
		0x7f800000, 
		0xc2780000, 
		0x0e0000ff, 
		0xc2080000, 
		0xff800000, 
		0x804c1639, 
		0xc0c00000, 
		0xc2080000, 
		0x41800000, 
		0x610c8f3f, 
		0x42680000, 
		0x807ffffe, 
		0x0b7790fd, 
		0x42300000, 
		0xb439da5c, 
		0x80000000, 
		0x42780000, 
		0x00200000, 
		0x80000000, 
		0x4b8c4b40, 
		0x0d000ff0, 
		0x0c7fff80, 
		0x0d8b70b6, 
		0x5a59f0f9, 
		0x80000001, 
		0x0013746e, 
		0xff800000, 
		0x42040000, 
		0x2c954266, 
		0xff800000, 
		0x4b4bfe08, 
		0x41800000, 
		0x0e0001ff, 
		0xd4cbcdd0, 
		0xc25c0000, 
		0x43ea8d08, 
		0x007ffffe, 
		0xce64f8ec, 
		0x0d00fff0, 
		0xd17a2694, 
		0xff800000, 
		0x8e44e8fd, 
		0x7f800000, 
		0x0e00000f, 
		0xe58c73da, 
		0x134fe860, 
		0xd6b9e749, 
		0x41600000, 
		0xc2300000, 
		0xc24c0000, 
		0xc0000000, 
		0xcb8c4b40, 
		0xf049607e, 
		0xc1900000, 
		0xc2480000, 
		0xfc16137b, 
		0x00000000, 
		0x0e00003f, 
		0xc25c0000, 
		0x80000000, 
		0xff7fffff, 
		0x41a00000, 
		0x41e80000, 
		0x7fbffd5e, 
		0x063946ec, 
		0x679eb46f, 
		0xc1000000, 
		0xbf028f5c, 
		0x41f80000, 
		0x36268700, 
		0xc1900000, 
		0xd72376ce, 
		0x95d3e25b, 
		0xc1100000, 
		0xc0896c73, 
		0xc2500000, 
		0x7e222b03, 
		0x7f7fffff, 
		0x7f800000, 
		0x80080000, 
		0x00000100, 
		0x7f7ffffe, 
		0x80800003, 
		0x055d4a99, 
		0x00800001, 
		0x7f800000, 
		0xbf800001, 
		0x543591b7, 
		0xee75b01e, 
		0x80000000, 
		0x42080000, 
		0xbf5118b1, 
		0xe52603ee, 
		0x1f0ddbeb, 
		0xc5ad3ae6, 
		0x80001000, 
		0x00004000, 
		0x3b9ab5f3, 
		0x00002000, 
		0x42440000, 
		0x42140000, 
		0x41d00000, 
		0x007fffff, 
		0x41a00000, 
		0xc353234c, 
		0x0c7e0000, 
		0x58bce5fe, 
		0x36958eae, 
		0x00000000, 
		0x0d8f102e, 
		0x007ffffe, 
		0x7f7ffffe, 
		0xff7ffffe, 
		0xfd7bdfba, 
		0xc80bf4e7, 
		0x943ef306, 
		0x0c7fffe0, 
		0x85dfacc2, 
		0x765cba56, 
		0xc2180000, 
		0xd909ebf7, 
		0x7f800000, 
		0x423c0000, 
		0x04f5f373, 
		0x00011111, 
		0x44acfe7b, 
		0xc2480000, 
		0xc1b80000, 
		0x8f7ffffe, 
		0x80000000, 
		0x84e39954, 
		0x41e00000, 
		0xb931b01b, 
		0x7f7ffffe, 
		0xc1d80000, 
		0x0e03ffff, 
		0xc0000000, 
		0x00000010, 
		0x310de09a, 
		0xdf2c6c5f, 
		0xff800000, 
		0x80000000, 
		0xc2340000, 
		0x0c7fffe0, 
		0xc23c0000, 
		0xc1e80000, 
		0x41800000, 
		0x0ed590e8, 
		0x0e000fff, 
		0x7f800000, 
		0x42500000, 
		0xc2680000, 
		0x00800003, 
		0x007fffff, 
		0x41f80000, 
		0x80000040, 
		0xc1000000, 
		0x0cf2569d, 
		0x3e75f367, 
		0xc2200000, 
		0x80000010, 
		0xbf800000, 
		0xc2740000, 
		0x0c7fffff, 
		0xff7ffffe, 
		0x420c0000, 
		0x886c00de, 
		0xbf800001, 
		0xff800000, 
		0x80ffffff, 
		0xffc07b40, 
		0x7f7fffff, 
		0xc22c0000, 
		0xc0800000, 
		0x5fed64be, 
		0xc1b00000, 
		0xce9410a3, 
		0xc2400000, 
		0x80000020, 
		0x7fc00000, 
		0x425c0000, 
		0xffe54ef9, 
		0xc1600000, 
		0x42680000, 
		0xc0e00000, 
		0xc0c00000, 
		0x42740000, 
		0x00000000, 
		0x2de168b1, 
		0x0f7ffffe, 
		0x42400000, 
		0x80000000, 
		0x4b8c4b40, 
		0x803b55ca, 
		0x0e000003, 
		0xff800000, 
		0x7f7fffff, 
		0x4d90480d, 
		0x42580000, 
		0x0e000003, 
		0xbdef9b69, 
		0xff800000, 
		0xc2540000, 
		0x00000008, 
		0x80ffffff, 
		0x41c80000, 
		0xc21c0000, 
		0xda525538, 
		0x42e213b4, 
		0x0b780832, 
		0xa09b6649, 
		0x41300000, 
		0x23bf731d, 
		0x7f800000, 
		0x7fc00000, 
		0x0e3fffff, 
		0xc2680000, 
		0x0c7fff80, 
		0x7f800000, 
		0x00400000, 
		0xc1e00000, 
		0x80000000, 
		0x5a0ce60d, 
		0x41e80000, 
		0x00800003, 
		0x80100000, 
		0x00800001, 
		0x22699ab2, 
		0x00080000, 
		0x80800003, 
		0xc23c0000, 
		0xc1a80000, 
		0xaaaaaaaa, 
		0x0e00003f, 
		0x00008000, 
		0x41f00000, 
		0x4510b091, 
		0xdda9bce6, 
		0x42140000, 
		0x00000000, 
		0xbf800000, 
		0x41880000, 
		0x0c7ffe00, 
		0x00000001, 
		0x564c2249, 
		0x00000000, 
		0xc1c00000, 
		0xc1700000, 
		0x7f800000, 
		0xd483386a, 
		0x0ba5da1b, 
		0xbd56aed5, 
		0x42480000, 
		0x0a45d722, 
		0xc1a00000, 
		0x5f721d3f, 
		0xc1880000, 
		0x61897b64, 
		0x3ec6ea82, 
		0x0c780000, 
		0x0c7c0000, 
		0x00000001, 
		0xf6f30193, 
		0x49f4fb99, 
		0x0e0000ff, 
		0x0c7e0000, 
		0x533f8427, 
		0x0e0003ff, 
		0x80011111, 
		0xc1e00000, 
		0x427c0000, 
		0x00800003, 
		0x3f028f5c, 
		0x00000080, 
		0x80011111, 
		0x80000100, 
		0x291b2e9e, 
		0x80000000, 
		0x80000000, 
		0x5a02b688, 
		0xc1c00000, 
		0xae718258, 
		0x42340000, 
		0x00000400, 
		0xffbfffff, 
		0x53981011, 
		0x00000010, 
		0x42240000, 
		0x0e00003f, 
		0x426c0000, 
		0xcccccccc, 
		0x16c83ef0, 
		0x80000010, 
		0x7f800000, 
		0x9d855a5a, 
		0x42140000, 
		0x42700000, 
		0xc2540000, 
		0x0c7ffe00, 
		0x80200000, 
		0x7f8ec0ca, 
		0x40400000, 
		0x3f800000, 
		0xc0a00000, 
		0x80000000, 
		0x426c0000, 
		0xb885579d, 
		0xbf800001, 
		0x75a50cfb, 
		0x80000000, 
		0x41e00000, 
		0x00040000, 
		0x4b8c4b40, 
		0x5d0f8cde, 
		0x00004000, 
		0x7f800000, 
		0x427c0000, 
		0x88abdc65, 
		0x41500000, 
		0xbe67af00, 
		0xc2480000, 
		0x00000000, 
		0x80000010, 
		0xe0e16601, 
		0x0c400000, 
		0x42180000, 
		0x4b095821, 
		0x80000000, 
		0xff7ffffe, 
		0x0f7fffff, 
		0x8795c645, 
		0xc2200000, 
		0x320380ee, 
		0x42480000, 
		0x7cc142cb, 
		0x8697d615, 
		0x420c0000, 
		0x7fafe1aa, 
		0x3cf41f14, 
		0x00ffffff, 
		0xa18cedc5, 
		0x7f800001, 
		0xdf2b72de, 
		0xffc00000, 
		0x7fb34093, 
		0x00800003, 
		0xc1d00000, 
		0x5342c23e, 
		0x007fffff, 
		0x00000400, 
		0xbf028f5c, 
		0x00008000, 
		0x6ce72982, 
		0x0221b300, 
		0x80800003, 
		0x807ffffe, 
		0x7f800000, 
		0x80000040, 
		0xc2080000, 
		0x80000000, 
		0x80000001, 
		0x7f800000, 
		0x80000000, 
		0x8f7ffffc, 
		0x7fffd585, 
		0x41400000, 
		0xb48215e3, 
		0x7fc00000, 
		0xf4990e93, 
		0xc26c0000, 
		0x40800000, 
		0x00000000, 
		0x80000010, 
		0xff800000, 
		0x00000000, 
		0x3f800001, 
		0x1397f8a0, 
		0x7f800000, 
		0xdb2013c6, 
		0xcb8c4b40, 
		0xc2740000, 
		0x7f7ffffe, 
		0x3f800001, 
		0xc21c0000, 
		0x42300000, 
		0x42340000, 
		0xd4e82ff4, 
		0xa3ae507b, 
		0x41400000, 
		0x51aa1742, 
		0xffffffff, 
		0x4b63f572, 
		0x42780000, 
		0x006f6dcc, 
		0x7ee1d5fe, 
		0x41100000, 
		0xc1e00000, 
		0x114cbe77, 
		0x4b000000, 
		0x80800000, 
		0x0e000fff, 
		0xc2100000, 
		0x00800000, 
		0xabbc67bc, 
		0x55555555, 
		0x0c7ff000, 
		0x9fa2aca2, 
		0xc1c80000, 
		0x41e00000, 
		0x7227a6c4, 
		0xc0e00000, 
		0xea3ec566  
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
		0x000002c1, 
		0x000002bf, 
		0x000002d1, 
		0x000002af, 
		0x000002ef, 
		0x000000d1, 
		0x000000ad, 
		0x000002ab, 
		0x000001d5, 
		0x00000055, 
		0x00000115, 
		0x00000321, 
		0x0000029d, 
		0x00000187, 
		0x00000189, 
		0x000000db, 
		0x000003e7, 
		0x0000003f, 
		0x000002db, 
		0x0000000d, 
		0x00000317, 
		0x0000006f, 
		0x000001a9, 
		0x00000273, 
		0x00000255, 
		0x000003b3, 
		0x00000363, 
		0x0000024b, 
		0x00000385, 
		0x000000ed, 
		0x00000127, 
		0x00000129, 
		0x00000113, 
		0x000003ff, 
		0x00000317, 
		0x0000035d, 
		0x00000333, 
		0x00000161, 
		0x00000089, 
		0x0000029d, 
		0x0000003d, 
		0x000003bb, 
		0x00000075, 
		0x0000014d, 
		0x000002ad, 
		0x0000033d, 
		0x000001dd, 
		0x00000357, 
		0x000002f5, 
		0x0000010d, 
		0x0000035d, 
		0x0000005f, 
		0x000003bf, 
		0x00000031, 
		0x0000019f, 
		0x0000032b, 
		0x00000313, 
		0x00000177, 
		0x00000119, 
		0x00000295, 
		0x000001e5, 
		0x0000001b, 
		0x000003e9, 
		0x0000036f, 
		0x0000038f, 
		0x000002c9, 
		0x00000359, 
		0x000002bb, 
		0x000000cf, 
		0x0000027d, 
		0x00000335, 
		0x00000325, 
		0x0000021f, 
		0x000002e7, 
		0x000002c1, 
		0x000001af, 
		0x000002af, 
		0x00000017, 
		0x00000121, 
		0x000000b1, 
		0x00000019, 
		0x0000020b, 
		0x0000036d, 
		0x000001b7, 
		0x00000017, 
		0x0000027b, 
		0x00000207, 
		0x000001dd, 
		0x00000091, 
		0x00000273, 
		0x0000010d, 
		0x00000185, 
		0x00000147, 
		0x00000329, 
		0x00000183, 
		0x00000289, 
		0x0000022d, 
		0x000000c7, 
		0x00000399, 
		0x00000169, 
		0x0000029b, 
		0x0000026d, 
		0x000002b3, 
		0x0000027f, 
		0x00000073, 
		0x0000003d, 
		0x000002df, 
		0x000001c3, 
		0x00000381, 
		0x00000287, 
		0x00000061, 
		0x00000323, 
		0x00000355, 
		0x00000129, 
		0x000003e5, 
		0x00000253, 
		0x00000233, 
		0x0000017f, 
		0x0000039d, 
		0x00000279, 
		0x00000031, 
		0x000000fb, 
		0x00000197, 
		0x000003ff, 
		0x0000039f, 
		0x000000ff, 
		0x0000002b, 
		0x00000269, 
		0x000000f7, 
		0x000001bd, 
		0x0000022b, 
		0x0000027d, 
		0x00000055, 
		0x000001ed, 
		0x00000143, 
		0x00000221, 
		0x000001bb, 
		0x00000107, 
		0x0000000b, 
		0x00000147, 
		0x000000a9, 
		0x000000f7, 
		0x000002bf, 
		0x000000a9, 
		0x000002db, 
		0x00000395, 
		0x000003ff, 
		0x000003af, 
		0x000001e9, 
		0x000001cf, 
		0x0000015b, 
		0x000001fb, 
		0x000000ab, 
		0x0000029f, 
		0x000002db, 
		0x000001b7, 
		0x000003e3, 
		0x000002c5, 
		0x000003d1, 
		0x000000f3, 
		0x00000285, 
		0x00000017, 
		0x00000281, 
		0x00000051, 
		0x00000343, 
		0x000003fd, 
		0x00000171, 
		0x000002cb, 
		0x000002d5, 
		0x0000008b, 
		0x000003b1, 
		0x00000031, 
		0x0000020b, 
		0x000002c9, 
		0x00000043, 
		0x0000018f, 
		0x000000bd, 
		0x000001fb, 
		0x000002f1, 
		0x00000209, 
		0x0000008f, 
		0x00000021, 
		0x0000001b, 
		0x000003e3, 
		0x000003e5, 
		0x000002cd, 
		0x00000053, 
		0x0000007b, 
		0x000001b5, 
		0x00000253, 
		0x0000013b, 
		0x000002df, 
		0x00000105, 
		0x000000af, 
		0x00000157, 
		0x0000022d, 
		0x0000017f, 
		0x00000137, 
		0x000002cb, 
		0x0000003d, 
		0x00000333, 
		0x00000301, 
		0x0000017b, 
		0x00000191, 
		0x00000265, 
		0x00000215, 
		0x000002c5, 
		0x00000201, 
		0x000003af, 
		0x00000311, 
		0x000001cb, 
		0x00000195, 
		0x00000009, 
		0x000000b9, 
		0x000002f9, 
		0x00000293, 
		0x00000037, 
		0x000001c1, 
		0x00000313, 
		0x000001a5, 
		0x000000db, 
		0x000002fd, 
		0x00000387, 
		0x0000035f, 
		0x00000165, 
		0x000000d1, 
		0x00000353, 
		0x0000014f, 
		0x000000f1, 
		0x00000173, 
		0x000001b5, 
		0x00000149, 
		0x00000277, 
		0x0000035d, 
		0x000003b3, 
		0x00000349, 
		0x000001fb, 
		0x0000017d, 
		0x000001f1, 
		0x000001ef, 
		0x000003f1, 
		0x00000335, 
		0x0000018f, 
		0x0000023b, 
		0x0000039b, 
		0x000000b5, 
		0x000002d1, 
		0x00000351, 
		0x000003eb, 
		0x000003fb, 
		0x00000221, 
		0x000000f9, 
		0x000000b5, 
		0x0000029f, 
		0x000003d7, 
		0x0000018b  
};

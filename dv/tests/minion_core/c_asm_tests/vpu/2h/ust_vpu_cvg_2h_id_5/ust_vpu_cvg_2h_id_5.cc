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
			"csrwi mhpmevent3, 15\n"   
			"csrwi mhpmevent4, 19\n"   
			"csrwi mhpmevent5, 2\n"   
			"csrwi mhpmevent6, 8\n"   
			"csrwi mhpmevent7, 6\n"   
			"csrwi mhpmevent8, 5\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f23,  672 (x5)\n"
			"flw.ps f11,  128 (x5)\n"
			"flw.ps f13,  864 (x5)\n"
			"flw.ps f10,  224 (x5)\n"
			"flw.ps f2,  384 (x5)\n"
			"flw.ps f15,  992 (x5)\n"
			"flw.ps f31,  928 (x5)\n"
			"flw.ps f19,  768 (x5)\n"
			"flw.ps f1,  896 (x5)\n"
			"flw.ps f29,  64 (x5)\n"
			"flw.ps f18,  448 (x5)\n"
			"flw.ps f26,  704 (x5)\n"
			"flw.ps f12,  544 (x5)\n"
			"flw.ps f28,  320 (x5)\n"
			"flw.ps f9,  160 (x5)\n"
			"flw.ps f30,  416 (x5)\n"
			"flw.ps f27,  832 (x5)\n"
			"flw.ps f17,  256 (x5)\n"
			"flw.ps f14,  192 (x5)\n"
			"flw.ps f5,  32 (x5)\n"
			"flw.ps f16,  0 (x5)\n"
			"flw.ps f6,  576 (x5)\n"
			"flw.ps f0,  800 (x5)\n"
			"flw.ps f22,  640 (x5)\n"
			"flw.ps f24,  96 (x5)\n"
			"flw.ps f7,  960 (x5)\n"
			"flw.ps f4,  608 (x5)\n"
			"flw.ps f21,  288 (x5)\n"
			"flw.ps f25,  480 (x5)\n"
			"flw.ps f20,  352 (x5)\n"
			"flw.ps f3,  512 (x5)\n"
			"flw.ps f8,  736 (x5)\n"
			"csrw fflags, zero\n"
			"csrwi frm, 0x0\n"
			"LBL_KERNEL_HID_0:\n"          
			"li x5, 0xFEEDFEED\n"
			"li x0, 0x258\n" 
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
			"la x9, rand_int_32\n"
			"flw.ps f0, 896(x9)\n"
			"flw.ps f19, 512(x9)\n"
			"fle.pi f7, f0, f19\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f30, 160(x7)\n"
			"fcvt.pwu.ps f11, f30, rup\n" 
			"la x12, rand_int_32\n"
			"flw.ps f7, 64(x12)\n"
			"faddi.pi f19, f7, 0x16a\n" 
			"la x7, rand_int_32\n"
			"flw.ps f18, 288(x7)\n"
			"fminu.pi f18, f18, f18\n" 
			"la x15, rand_int_32\n"
			"flw.ps f12, 416(x15)\n"
			"flw.ps f28, 864(x15)\n"
			"feq.pi f27, f12, f28\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f6, 256(x13)\n"
			"flw.ps f9, 800(x13)\n"
			"fmax.ps f19, f6, f9\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f18, 96(x7)\n"
			"flw.ps f12, 832(x7)\n"
			"fmul.ps f18, f18, f12, rmm\n" 
			"la x28, rand_int_32\n"
			"flw.ps f13, 352(x28)\n"
			"flw.ps f1, 192(x28)\n"
			"fmul.pi f7, f13, f1\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f13, 128(x15)\n"
			"flw.ps f28, 576(x15)\n"
			"fsub.s f30, f13, f28, rne\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f19, 352(x25)\n"
			"flw.ps f6, 416(x25)\n"
			"feq.s x15, f19, f6\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_0:\n"
			"mov.m.x m6, x13, 0x19\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x10, rand_int_32\n"
			"flw.ps f9, 960(x10)\n"
			"flw.ps f13, 736(x10)\n"
			"fsub.pi f30, f9, f13\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f13, 192(x15)\n"
			"flw.ps f6, 832(x15)\n"
			"fle.ps f30, f13, f6\n" 
			"LBL_SEQID_0_FP_TRANS_ID_1_HID_0:\n"
			"la x12, rand_ieee754_32\n"
			"flw.ps f19, 480(x12)\n"
			"fexp.ps f18, f19\n" 
			"LBL_SEQID_0_M0_WRITE_ID_2_HID_0:\n"
			"fsetm.pi m0, f23\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_0_FP_TRANS_ID_3_HID_0:\n"
			"la x20, rand_ieee754_32\n"
			"flw.ps f2, 448(x20)\n"
			"frcp.ps f0, f2\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_0\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_1_M0_WRITE_ID_4_HID_0:\n"
			"masknot m0, m0\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x13, rand_int_32\n"
			"flw.ps f30, 640(x13)\n"
			"flw.ps f2, 0(x13)\n"
			"fltu.pi f2, f30, f2\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f3, 128(x9)\n"
			"ffrc.ps f11, f3\n" 
			"la x22, rand_int_32\n"
			"flw.ps f9, 32(x22)\n"
			"flw.ps f27, 288(x22)\n"
			"fdiv.pi f28, f9, f27\n" 
			"LBL_SEQID_1_M0_WRITE_ID_5_HID_0:\n"
			"maskxor m3, m6, m0\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x20, rand_ieee754_32\n"
			"flw.ps f23, 832(x20)\n"
			"frsq.ps f2, f23\n" 
			"LBL_SEQID_1_M0_WRITE_ID_6_HID_0:\n"
			"masknot m0, m6\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x20, rand_int_32\n"
			"flw.ps f23, 192(x20)\n"
			"fsrai.pi f12, f23, 0x4\n" 
			"la x13, rand_int_32\n"
			"flw.ps f18, 256(x13)\n"
			"flw.ps f11, 768(x13)\n"
			"fxor.pi f30, f18, f11\n" 
			"la x25, rand_int_32\n"
			"flw.ps f27, 160(x25)\n"
			"flw.ps f11, 352(x25)\n"
			"for.pi f19, f27, f11\n" 
			"LBL_SEQID_1_M0_WRITE_ID_7_HID_0:\n"
			"fsetm.pi m2, f3\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0xaf99fac563\n"
			"flw.ps f5, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x13, rand_ieee754_32\n"
			"flw.ps f11, 576(x13)\n"
			"fcvt.w.s x9, f11, rdn\n" 
			"la x28, rand_int_32\n"
			"flw.ps f6, 256(x28)\n"
			"flw.ps f18, 256(x28)\n"
			"fsll.pi f27, f6, f18\n" 
			"la x7, rand_int_32\n"
			"flw.ps f12, 864(x7)\n"
			"fltu.pi f1, f12, f12\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f0, 256(x25)\n"
			"flw.ps f1, 320(x25)\n"
			"fdiv.ps f12, f0, f1, rdn\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f6, 384(x15)\n"
			"fround.ps f12, f6, rdn\n" 
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
			"LBL_HPM_3_CORE_TL_OPS_FAIL_HID_0:\n"          
			"csrr x5, mhpmcounter3\n"
			"sd x5, 32 (x31)\n"
			"LBL_HPM_4_CORE_TIMA_OPS_FAIL_HID_0:\n"          
			"csrr x5, mhpmcounter4\n"
			"sd x5, 40 (x31)\n"
			"LBL_HPM_5_CORE_RETIRED_INST0_FAIL_HID_0:\n"          
			"csrr x5, mhpmcounter5\n"
			"sd x5, 48 (x31)\n"
			"LBL_HPM_6_CORE_DCACHE_MISSES0_FAIL_HID_0:\n"          
			"csrr x5, mhpmcounter6\n"
			"sd x5, 56 (x31)\n"
			"LBL_HPM_7_NEIGH_TX_COOP_TST_REQ_FAIL_HID_0:\n"          
			"csrr x5, mhpmcounter7\n"
			"sd x5, 64 (x31)\n"
			"LBL_HPM_8_NEIGH_RX_COOP_TLD_RSP_FAIL_HID_0:\n"          
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
			"LBL_HPM_3_CORE_TL_OPS_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter3\n"
			"sd x5, 32 (x31)\n"
			"LBL_HPM_4_CORE_TIMA_OPS_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter4\n"
			"sd x5, 40 (x31)\n"
			"LBL_HPM_5_CORE_RETIRED_INST0_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter5\n"
			"sd x5, 48 (x31)\n"
			"LBL_HPM_6_CORE_DCACHE_MISSES0_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter6\n"
			"sd x5, 56 (x31)\n"
			"LBL_HPM_7_NEIGH_TX_COOP_TST_REQ_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter7\n"
			"sd x5, 64 (x31)\n"
			"LBL_HPM_8_NEIGH_RX_COOP_TLD_RSP_PASS_HID_0:\n"          
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
			"csrwi mhpmevent4, 2\n"   
			"csrwi mhpmevent5, 23\n"   
			"csrwi mhpmevent6, 25\n"   
			"csrwi mhpmevent7, 22\n"   
			"csrwi mhpmevent8, 2\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f25,  128 (x5)\n"
			"flw.ps f9,  800 (x5)\n"
			"flw.ps f11,  768 (x5)\n"
			"flw.ps f30,  224 (x5)\n"
			"flw.ps f13,  960 (x5)\n"
			"flw.ps f8,  864 (x5)\n"
			"flw.ps f1,  512 (x5)\n"
			"flw.ps f20,  608 (x5)\n"
			"flw.ps f22,  0 (x5)\n"
			"flw.ps f7,  640 (x5)\n"
			"flw.ps f0,  320 (x5)\n"
			"flw.ps f27,  64 (x5)\n"
			"flw.ps f4,  448 (x5)\n"
			"flw.ps f29,  576 (x5)\n"
			"flw.ps f5,  192 (x5)\n"
			"flw.ps f23,  896 (x5)\n"
			"flw.ps f10,  544 (x5)\n"
			"flw.ps f18,  672 (x5)\n"
			"flw.ps f21,  352 (x5)\n"
			"flw.ps f12,  288 (x5)\n"
			"flw.ps f26,  32 (x5)\n"
			"flw.ps f2,  160 (x5)\n"
			"flw.ps f6,  384 (x5)\n"
			"flw.ps f24,  928 (x5)\n"
			"flw.ps f28,  480 (x5)\n"
			"flw.ps f17,  832 (x5)\n"
			"flw.ps f31,  416 (x5)\n"
			"flw.ps f3,  96 (x5)\n"
			"flw.ps f15,  256 (x5)\n"
			"flw.ps f19,  704 (x5)\n"
			"flw.ps f16,  736 (x5)\n"
			"flw.ps f14,  992 (x5)\n"
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
			"la x27, rand_ieee754_32\n"
			"flw.ps f25, 416(x27)\n"
			"fsin.ps f22, f25\n" 
			"la x9, rand_int_32\n"
			"flw.ps f11, 128(x9)\n"
			"fcvt.ps.pwu f15, f11, rmm\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f15, 384(x9)\n"
			"flw.ps f19, 288(x9)\n"
			"flw.ps f8, 224(x9)\n"
			"fmsub.ps f21, f15, f19, f8, rtz\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_1:\n"
			"mov.m.x m5, x23, 0x4\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x17, rand_ieee754_32\n"
			"flw.ps f20, 768(x17)\n"
			"flw.ps f0, 128(x17)\n"
			"flw.ps f4, 992(x17)\n"
			"fnmadd.ps f13, f20, f0, f4, rup\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_1:\n"
			"fltm.ps m7, f4, f0\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_0_M0_WRITE_ID_2_HID_1:\n"
			"maskxor m5, m5, m1\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_0_M0_WRITE_ID_3_HID_1:\n"
			"maskxor m1, m6, m6\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0x72be8d5f18\n"
			"flw.ps f5, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_0_M0_WRITE_ID_4_HID_1:\n"
			"maskand m5, m1, m7\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x19, rand_int_32\n"
			"flw.ps f15, 256(x19)\n"
			"flw.ps f20, 992(x19)\n"
			"fsub.pi f13, f15, f20\n" 
			"LBL_SEQID_0_M0_WRITE_ID_5_HID_1:\n"
			"feqm.ps m5, f8, f20\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x27, rand_int_32\n"
			"flw.ps f4, 800(x27)\n"
			"flw.ps f21, 320(x27)\n"
			"fmul.pi f15, f4, f21\n" 
			"li x27, 0x2ef4994da1b7204\n"
			"fcvt.s.wu f11, x27, rup\n" 
			"la x15, rand_int_32\n"
			"flw.ps f19, 0(x15)\n"
			"fsat8.pi f22, f19\n" 
			"LBL_SEQID_0_M0_WRITE_ID_6_HID_1:\n"
			"maskxor m5, m6, m7\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x15, rand_int_32\n"
			"flw.ps f15, 448(x15)\n"
			"flw.ps f20, 0(x15)\n"
			"fminu.pi f21, f15, f20\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_1\n"
			"addi x30, x30, 1\n"
			"mova.x.m x27\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f6, 832(x9)\n"
			"flw.ps f21, 896(x9)\n"
			"fmin.ps f8, f6, f21\n" 
			"la x12, rand_int_32\n"
			"flw.ps f29, 64(x12)\n"
			"fsrai.pi f15, f29, 0x9\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f27, 192(x9)\n"
			"flw.ps f22, 384(x9)\n"
			"feq.ps f20, f27, f22\n" 
			"li x11, 0x2736d121512d36b4\n"
			"fcvt.s.w f15, x11, rdn\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f13, 512(x23)\n"
			"flw.ps f27, 576(x23)\n"
			"fsgnj.ps f29, f13, f27\n" 
			"LBL_SEQID_1_M0_WRITE_ID_7_HID_1:\n"
			"fsetm.pi m7, f22\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x15, rand_int_32\n"
			"flw.ps f21, 832(x15)\n"
			"flw.ps f29, 384(x15)\n"
			"fmax.pi f1, f21, f29\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f1, 320(x19)\n"
			"ffrc.ps f29, f1\n" 
			"la x12, rand_int_32\n"
			"flw.ps f6, 0(x12)\n"
			"fpackrepb.pi f1, f6\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f25, 992(x15)\n"
			"flw.ps f29, 768(x15)\n"
			"fadd.ps f8, f25, f29, rne\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f27, 448(x9)\n"
			"flw.ps f0, 64(x9)\n"
			"fsgnjn.s f1, f27, f0\n" 
			"LBL_SEQID_1_M0_WRITE_ID_8_HID_1:\n"
			"maskor m6, m1, m5\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_1_M0_WRITE_ID_9_HID_1:\n"
			"feqm.ps m5, f4, f22\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x9, rand_ieee754_32\n"
			"flw.ps f11, 352(x9)\n"
			"fcvt.f16.ps f22, f11\n" 
			"fbci.ps f11, 0x306b2\n" 
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
			"LBL_HPM_3_CORE_L2_EVICT_REQ_REJ_FAIL_HID_1:\n"          
			"csrr x5, mhpmcounter3\n"
			"sd x5, 32 (x31)\n"
			"LBL_HPM_4_CORE_RETIRED_INST0_FAIL_HID_1:\n"          
			"csrr x5, mhpmcounter4\n"
			"sd x5, 40 (x31)\n"
			"LBL_HPM_5_CORE_FTRANS_OPS_FAIL_HID_1:\n"          
			"csrr x5, mhpmcounter5\n"
			"sd x5, 48 (x31)\n"
			"LBL_HPM_6_CORE_MASK_OPS_FAIL_HID_1:\n"          
			"csrr x5, mhpmcounter6\n"
			"sd x5, 56 (x31)\n"
			"LBL_HPM_7_NEIGH_ETL_REQ_TO_SC_UC_FIFO_FAIL_HID_1:\n"          
			"csrr x5, mhpmcounter7\n"
			"sd x5, 64 (x31)\n"
			"LBL_HPM_8_NEIGH_ANY_CORE_RX_ETL_RSP_FAIL_HID_1:\n"          
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
			"LBL_HPM_4_CORE_RETIRED_INST0_PASS_HID_1:\n"          
			"csrr x5, mhpmcounter4\n"
			"sd x5, 40 (x31)\n"
			"LBL_HPM_5_CORE_FTRANS_OPS_PASS_HID_1:\n"          
			"csrr x5, mhpmcounter5\n"
			"sd x5, 48 (x31)\n"
			"LBL_HPM_6_CORE_MASK_OPS_PASS_HID_1:\n"          
			"csrr x5, mhpmcounter6\n"
			"sd x5, 56 (x31)\n"
			"LBL_HPM_7_NEIGH_ETL_REQ_TO_SC_UC_FIFO_PASS_HID_1:\n"          
			"csrr x5, mhpmcounter7\n"
			"sd x5, 64 (x31)\n"
			"LBL_HPM_8_NEIGH_ANY_CORE_RX_ETL_RSP_PASS_HID_1:\n"          
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
		0xb52fedbd, 
		0x59120c50, 
		0xf08007cf, 
		0x09f77f2b, 
		0xf2873416, 
		0x66f70f87, 
		0x55a20828, 
		0xd6824e94, 
		0x4e306297, 
		0xe520e3d8, 
		0xb4d5d692, 
		0x95842ef8, 
		0xe1d44717, 
		0x6b189cda, 
		0x37bf3a80, 
		0xffcbcf7f, 
		0xeb5089ee, 
		0x1ba98d0a, 
		0x7b11ddfe, 
		0x62e5fa07, 
		0x67b06987, 
		0xbdf4993b, 
		0xc3085c56, 
		0xc111cff3, 
		0xa1e11f52, 
		0x119f5e0d, 
		0x9de3c23e, 
		0xb7b7b35b, 
		0xc61b129b, 
		0xf395fa03, 
		0x49a7fbbd, 
		0x183c6890, 
		0x21ba1017, 
		0xc214a8bb, 
		0x901d51d0, 
		0x5b685225, 
		0xbfe13844, 
		0xd676f1db, 
		0x6a24c5a9, 
		0x931f357a, 
		0x28cad0d2, 
		0x1e2c54d0, 
		0x84ccbfd2, 
		0xc805a4ae, 
		0xfb181778, 
		0x29d1a16b, 
		0x6c58f19d, 
		0x8c11808c, 
		0xf62dd8c2, 
		0x0c47861e, 
		0xe084328d, 
		0xea3f5ec8, 
		0xdf57fb41, 
		0xc8eea6df, 
		0x830d3546, 
		0xa76dc9a2, 
		0x748cb1a1, 
		0xef72f342, 
		0xceb7d573, 
		0x6bf5578d, 
		0xdf181118, 
		0x15d06f30, 
		0x44d58d66, 
		0x6533b7ab, 
		0x5c1b4b50, 
		0xbbec2102, 
		0xa4928caa, 
		0x1ee9e4a9, 
		0x018d60c1, 
		0xd2a7e197, 
		0x82d851a9, 
		0xc2e671c6, 
		0x6b15879e, 
		0x6c6303eb, 
		0xc57dd8b5, 
		0xa4836de2, 
		0xe640a051, 
		0x315ac952, 
		0xfdf04508, 
		0x98d6d248, 
		0x68b74a6a, 
		0x9ee8be78, 
		0xf60d1022, 
		0xa26ee86a, 
		0xeb2855fb, 
		0x0fed7f6c, 
		0xb29e6b4e, 
		0x45241ffc, 
		0xa9f96390, 
		0x9f30ff27, 
		0xcb5aca7a, 
		0x53aab758, 
		0x73b3591f, 
		0xfb91bc64, 
		0x7c6c4218, 
		0x323d8194, 
		0x73d5f848, 
		0xee1e71ae, 
		0x2bd79779, 
		0x92ead8db, 
		0x57a111c4, 
		0x73c3a718, 
		0x3d129050, 
		0x43a52179, 
		0x30a2c2c9, 
		0x928cacc6, 
		0xdef71f44, 
		0x203095ad, 
		0x8edfd985, 
		0x576bbe45, 
		0xa63c9374, 
		0xaf227d57, 
		0xe44af044, 
		0x2f3800f2, 
		0x3a5c71ad, 
		0x17daefb3, 
		0xc7e65efe, 
		0xccf91f57, 
		0x0402f24a, 
		0x182d0a68, 
		0x4db122db, 
		0x09dbb7b5, 
		0x8dc03a5f, 
		0x10c62db7, 
		0xa72bef00, 
		0xdb8bf247, 
		0x2a681708, 
		0x4a8aa72c, 
		0x0ff792e8, 
		0x762d382a, 
		0xa3d4bd7b, 
		0xc89a3729, 
		0xceb28589, 
		0x2dcd9a6d, 
		0xe58f1411, 
		0x0add81e6, 
		0xf7589c27, 
		0x547ce382, 
		0xdd3ad133, 
		0x5c9f965c, 
		0xb80aed6b, 
		0x0d24c616, 
		0x82941d5d, 
		0x769fb819, 
		0x23e68260, 
		0x83b7e0d6, 
		0x9bbd061c, 
		0xcd466933, 
		0x1706de1a, 
		0x1fe13313, 
		0x6e28b887, 
		0xbec60bbc, 
		0x6437f529, 
		0x54a40334, 
		0x3da2ebcb, 
		0xda63dc39, 
		0xde52703b, 
		0x85d5cd5a, 
		0x609b73c1, 
		0x92a684ed, 
		0x9d5e8c42, 
		0x6f072bc4, 
		0x268f6627, 
		0x00d3378c, 
		0xc2b72236, 
		0x4887b0de, 
		0x24c91eec, 
		0x37e3a692, 
		0x3fee7b9d, 
		0x4417a26c, 
		0x237b5b01, 
		0x49e72dd7, 
		0xa6c482fe, 
		0x5f4de14e, 
		0xbf8e909f, 
		0x9b61a5fb, 
		0x8de77100, 
		0xb20c1569, 
		0x601a1c42, 
		0x3f4e3aa7, 
		0x3637eda4, 
		0xaaa67326, 
		0xd3e23d27, 
		0x1b3638dc, 
		0x27c5cec3, 
		0x47518220, 
		0x9eba68a4, 
		0x8eee051c, 
		0x0c471df8, 
		0x0847685d, 
		0x24eb1288, 
		0x1cfbd672, 
		0xa4a888ab, 
		0x004bacfa, 
		0xd911174b, 
		0x3dfa3655, 
		0x0bc92cb1, 
		0xcd053086, 
		0x321140ea, 
		0x2250f1c6, 
		0x5b74c77d, 
		0xae303977, 
		0x7f42657a, 
		0xa8d799d2, 
		0x4343a4e6, 
		0x07c9deed, 
		0x60deb9d5, 
		0x777552b9, 
		0x3c62cb1b, 
		0x9573c2bb, 
		0x1cd40f39, 
		0xe9094712, 
		0x504318a2, 
		0xc78a1b78, 
		0xee4f5bad, 
		0x2a68f6cb, 
		0x14e99121, 
		0x7b3b81ca, 
		0x1ff8ec1d, 
		0xfa96054c, 
		0xe22c8d87, 
		0x00a5f1b2, 
		0xeb003e17, 
		0x5b36c19f, 
		0xdd90e0f8, 
		0x45c217c8, 
		0x4c816264, 
		0xc0ad96d0, 
		0xb716792a, 
		0xa6ed3b66, 
		0xa5763747, 
		0xe6ffdca2, 
		0x6c778469, 
		0x91cb5378, 
		0x475df2f5, 
		0x8ba68b88, 
		0xe1cd8f43, 
		0xc70c3674, 
		0xc6ee8d37, 
		0xe1190a1c, 
		0x2c890258, 
		0xc24f3071, 
		0xcf62fffc, 
		0x09f5901a, 
		0x89296bc5, 
		0x9dd23006, 
		0x58a31c01, 
		0x7f7c7027, 
		0x8835b05d, 
		0x8ae4cc41, 
		0xc18cd337, 
		0x15968efb, 
		0xb879e3a6, 
		0x4dde160c, 
		0x33b6ae77, 
		0x7d0b8a0a  
};
volatile uint32_t rand_ieee754_32[1536] __attribute__ ((aligned (2048))) = {
		0x56fac018, 
		0x1c15cc58, 
		0x0c7ffff0, 
		0x80000000, 
		0xa5078193, 
		0x9d380d97, 
		0xffbfffff, 
		0x00001000, 
		0xe4ddeeb0, 
		0x80000000, 
		0x8be45709, 
		0x41f80000, 
		0x0c7ffe00, 
		0x41e80000, 
		0x80800002, 
		0x00000001, 
		0xc2240000, 
		0x7fb81d36, 
		0xff800000, 
		0x0d6e1dc7, 
		0xc2100000, 
		0x00000000, 
		0xc2480000, 
		0x807ff403, 
		0xff800000, 
		0x41f80000, 
		0x3f800001, 
		0x0e000007, 
		0x42500000, 
		0x8c39a565, 
		0x4d0b2cbb, 
		0xc0c00000, 
		0x41a80000, 
		0x561da2ae, 
		0x00001000, 
		0x9f42f378, 
		0x00800000, 
		0x00800002, 
		0x80000400, 
		0x40c00000, 
		0x8522d055, 
		0xc2440000, 
		0xc2140000, 
		0x1af2f829, 
		0x00008000, 
		0xff800000, 
		0xff800001, 
		0x0117e2b4, 
		0x810af86c, 
		0x9e3b9817, 
		0x007fffff, 
		0x807ffffe, 
		0x80800001, 
		0x02e72fe8, 
		0x0e001fff, 
		0xffa5cb60, 
		0x80000400, 
		0xc2640000, 
		0x80800003, 
		0x42680000, 
		0x00000000, 
		0x0e000fff, 
		0x7a81d161, 
		0x80800003, 
		0x00080000, 
		0x92b7d296, 
		0xcccccccc, 
		0x80000000, 
		0x974aae30, 
		0x41c00000, 
		0x80000000, 
		0x00000000, 
		0x00000000, 
		0x00000000, 
		0xbb5a1456, 
		0x00800001, 
		0xc2140000, 
		0xa4df4295, 
		0x7fc00001, 
		0x5a7c6238, 
		0x40c00000, 
		0x80100000, 
		0x80000000, 
		0xbbc65841, 
		0x3750dd74, 
		0xa8e40fce, 
		0x41c00000, 
		0x0c7fff00, 
		0x7642ef40, 
		0x958d7568, 
		0x80000000, 
		0x0e3fffff, 
		0xe8dcb2ff, 
		0x06288f3a, 
		0x80000020, 
		0x0e07ffff, 
		0x80000000, 
		0x58ed8024, 
		0x0e7fffff, 
		0x41300000, 
		0x00400000, 
		0x5695ce85, 
		0xc1980000, 
		0x00000000, 
		0x753b2d18, 
		0x80ffffff, 
		0x0c7ffff0, 
		0x1dafaf24, 
		0xc2700000, 
		0x42640000, 
		0x223ad641, 
		0x0c7fff00, 
		0xc3aef699, 
		0x80000800, 
		0xfa1ab0dc, 
		0x80000004, 
		0xdd7ac201, 
		0x7f7ffffe, 
		0xf5797521, 
		0x38c024a2, 
		0xff800000, 
		0x51a499d8, 
		0x0c1b21b3, 
		0xc21c0000, 
		0x7fc00000, 
		0xfab278cb, 
		0x4b8c4b40, 
		0x41b00000, 
		0x40fbe6b1, 
		0x00800000, 
		0x4d80cab2, 
		0xa82c2afb, 
		0x42480000, 
		0x877b15f7, 
		0x41000000, 
		0x80011111, 
		0x00000002, 
		0x00000000, 
		0x912f20c2, 
		0x00800003, 
		0x0e00ffff, 
		0x518b9f35, 
		0x80000001, 
		0xe08cfa7c, 
		0xc1d80000, 
		0xc2280000, 
		0xc2100000, 
		0xc2780000, 
		0xc2780000, 
		0x55555555, 
		0x0a50a0a6, 
		0xffbc3f37, 
		0xff800000, 
		0x0e0007ff, 
		0x42180000, 
		0x0c7fe000, 
		0xcccccccc, 
		0xff7ffffe, 
		0x7a1888fd, 
		0xc1600000, 
		0x00000000, 
		0xdb889b9f, 
		0x530f3cbb, 
		0xb20eed90, 
		0x421c0000, 
		0x42440000, 
		0x76f22b75, 
		0x42680000, 
		0x80800000, 
		0xffc00000, 
		0x00ffffff, 
		0x0e000fff, 
		0x7f800001, 
		0x80000000, 
		0x0c7fe000, 
		0x7f7ffffe, 
		0xffbc0a9f, 
		0xecb86f2d, 
		0x0c7fc000, 
		0x7fc00001, 
		0x80008000, 
		0x42540000, 
		0xb2cbab7d, 
		0x80200000, 
		0xc1f80000, 
		0x00000001, 
		0xc2600000, 
		0x00000001, 
		0xc1200000, 
		0x68537ec5, 
		0x80011111, 
		0x1eb29f3b, 
		0xc26c0000, 
		0x00000010, 
		0x7f800000, 
		0xacec8027, 
		0x00000000, 
		0x41a00000, 
		0xa73b6d59, 
		0x7f7ffffe, 
		0xedcc4de7, 
		0xff800000, 
		0x005fd405, 
		0x8aa4363a, 
		0x7f800000, 
		0xffc00000, 
		0x0e001fff, 
		0xc1a80000, 
		0x7fc00000, 
		0x3f028f5c, 
		0x80000002, 
		0x4e5bbe12, 
		0x80ffffff, 
		0x425c0000, 
		0xc1200000, 
		0x80000000, 
		0xc2440000, 
		0x50ad8107, 
		0x420c0000, 
		0xff800001, 
		0x480b30f0, 
		0xd4fd9af6, 
		0xff800000, 
		0x80000200, 
		0x40400000, 
		0x451af8bf, 
		0x0e00000f, 
		0x00800000, 
		0xf41f3954, 
		0xdd8726ac, 
		0xc26c0000, 
		0xc2480000, 
		0x0cb83e92, 
		0xc2280000, 
		0x13df91f9, 
		0x7f800000, 
		0x00000000, 
		0xc25c0000, 
		0xc2500000, 
		0x80000008, 
		0x80000004, 
		0x00020000, 
		0xffcd5500, 
		0x7f800000, 
		0x80100000, 
		0x80000004, 
		0xff800000, 
		0xc1800000, 
		0x981dc696, 
		0x2b63983d, 
		0x0c7fe000, 
		0x7f800000, 
		0x80000004, 
		0x804dbd8d, 
		0x00800003, 
		0xc2780000, 
		0x422c0000, 
		0x826eb26f, 
		0x18d8ba46, 
		0x0e7fffff, 
		0x00040000, 
		0x0799a251, 
		0xe4b63e3e, 
		0x07b9b245, 
		0x80000002, 
		0x328d8ea7, 
		0x802de3dc, 
		0x80000001, 
		0x00000000, 
		0x0e3fffff, 
		0xc2080000, 
		0x7f800000, 
		0xff7fffff, 
		0x42740000, 
		0x0e3fffff, 
		0x0e07ffff, 
		0x42000000, 
		0x0e01ffff, 
		0x278afaf1, 
		0x00800001, 
		0x0c7f8000, 
		0x0f7ffffd, 
		0x0e03ffff, 
		0x7f7ffffe, 
		0x20f280cb, 
		0x0c7fff00, 
		0x00040000, 
		0x503969f5, 
		0xc2380000, 
		0xc2180000, 
		0x92371424, 
		0x3f800000, 
		0x13547e7c, 
		0x00ffffff, 
		0x40800000, 
		0x7bd1297e, 
		0xffc00000, 
		0x0c7e0000, 
		0x80000000, 
		0xcb143da7, 
		0xaaaaaaaa, 
		0x42400000, 
		0x80010000, 
		0x8f7ffffd, 
		0x80020000, 
		0x40c00000, 
		0x53386f5b, 
		0x8506bff2, 
		0xc0e00000, 
		0x8461686b, 
		0x80000000, 
		0xc1100000, 
		0x08cf1f85, 
		0xff800000, 
		0x420c0000, 
		0x6caa7088, 
		0x42580000, 
		0x98217f67, 
		0x44be8994, 
		0xc2580000, 
		0x80800000, 
		0x42500000, 
		0x4c5a186a, 
		0x0c7ffffc, 
		0x3f800000, 
		0xc1000000, 
		0x993d17bc, 
		0xc1400000, 
		0x80080000, 
		0x48857cfe, 
		0xff800000, 
		0x7f800000, 
		0x42340000, 
		0x80011111, 
		0x66070ced, 
		0x18d4f741, 
		0xb5daac0e, 
		0xc0000000, 
		0xfa9a9008, 
		0x42440000, 
		0xbb96dd25, 
		0x42500000, 
		0x80001000, 
		0x00200000, 
		0x00010000, 
		0x67c26375, 
		0xc1200000, 
		0xc1c00000, 
		0x420c0000, 
		0x80000200, 
		0x8ce11c60, 
		0xc2140000, 
		0xbc658250, 
		0x00000000, 
		0xf516219f, 
		0x0c780000, 
		0x41880000, 
		0x0c7fffff, 
		0xbdd20735, 
		0x00800001, 
		0x0e007fff, 
		0x803f812c, 
		0x40c00000, 
		0x7f7ffffe, 
		0x7f800000, 
		0xff800000, 
		0xffc00001, 
		0xef56b814, 
		0xc2580000, 
		0x2c3e38da, 
		0x17d6f3ea, 
		0x3f800000, 
		0x80800000, 
		0x0c700000, 
		0x278d4270, 
		0xc2440000, 
		0xc2480000, 
		0x7fffffff, 
		0x00800003, 
		0x41200000, 
		0x80080000, 
		0xc2100000, 
		0x11e6ba31, 
		0x55555555, 
		0xb3871b25, 
		0x00080000, 
		0x41800000, 
		0x00000100, 
		0x8f7ffffe, 
		0x00000400, 
		0x4b8c4b40, 
		0x7f800000, 
		0x80800001, 
		0x80000010, 
		0x3f028f5c, 
		0x40c00000, 
		0xee35f34f, 
		0x4d9dadec, 
		0x7fbe9620, 
		0x18cd5f07, 
		0xd3d91544, 
		0x09e27a3f, 
		0x80000004, 
		0x41a00000, 
		0x007ffffe, 
		0x80000000, 
		0xc2440000, 
		0x59fdc05a, 
		0xe0cd0db7, 
		0x5979b796, 
		0x00000002, 
		0x0e0fffff, 
		0x7f7fffff, 
		0xc27c0000, 
		0x00000000, 
		0x0e0000ff, 
		0xff800000, 
		0xffffffff, 
		0xff800000, 
		0x0bebad4c, 
		0x80000000, 
		0xbf800001, 
		0x42540000, 
		0x3f028f5c, 
		0xc2680000, 
		0xccd2dbb0, 
		0x8feb5de2, 
		0xc2740000, 
		0x80000001, 
		0x42480000, 
		0x00000000, 
		0x00000000, 
		0x8f7fffff, 
		0x80800000, 
		0xc0400000, 
		0xff7ffffe, 
		0x41e00000, 
		0x2d6dba47, 
		0xc2640000, 
		0x00020000, 
		0xff800000, 
		0x456d6883, 
		0x2d54ba33, 
		0xc1c00000, 
		0x0e1fffff, 
		0x80000400, 
		0x00800002, 
		0x7f800000, 
		0x82f31645, 
		0x35575fd0, 
		0x80000000, 
		0x0c7f8000, 
		0x00000000, 
		0x80000000, 
		0x426c0000, 
		0x55f4c504, 
		0xc1b80000, 
		0x41a80000, 
		0xc1b00000, 
		0x41d00000, 
		0xc1800000, 
		0x538a0d7b, 
		0xc2280000, 
		0x41c80000, 
		0x0c700000, 
		0x41200000, 
		0xc2400000, 
		0x58cd51ae, 
		0xc21c0000, 
		0xff7fffff, 
		0x0c700000, 
		0x9a67c1dd, 
		0x41500000, 
		0x42180000, 
		0x93b12aea, 
		0x2f4d3713, 
		0xc25c0000, 
		0xc0e00000, 
		0x41400000, 
		0x191883bf, 
		0x148f4bba, 
		0x0c7f0000, 
		0xc2540000, 
		0x46ed5635, 
		0x80800003, 
		0x0f7ffffc, 
		0x00008000, 
		0x424c0000, 
		0x00800000, 
		0x3f028f5c, 
		0x3f800000, 
		0x934e0653, 
		0x42000000, 
		0xd91b5848, 
		0x0c7ffc00, 
		0x9eb93c7b, 
		0x0c7f0000, 
		0x7fffffff, 
		0x42000000, 
		0x00800000, 
		0x762b584b, 
		0x7f7ffffe, 
		0x554f7c5c, 
		0xff800000, 
		0x3ad8a851, 
		0xc1d80000, 
		0x589e8015, 
		0x4b8c4b40, 
		0x2dc85f59, 
		0x9dae5624, 
		0xffbfffff, 
		0x41a80000, 
		0x7fbfffff, 
		0x62d3f403, 
		0x5ab77399, 
		0x007fffff, 
		0x42600000, 
		0x42040000, 
		0x0d96579e, 
		0xff800000, 
		0x7fc00000, 
		0xe84b6d8f, 
		0x80000008, 
		0x00000000, 
		0x00400000, 
		0x0e001fff, 
		0x0f7fffff, 
		0x00000001, 
		0xc1f00000, 
		0xff800000, 
		0x1ee9cbf8, 
		0xc997bbb0, 
		0x80000000, 
		0xc22c0000, 
		0xffc00000, 
		0x80001000, 
		0x0f7ffffe, 
		0x80800000, 
		0x3bb7d7c8, 
		0x9ccf60ea, 
		0xc22c0000, 
		0x41300000, 
		0x07f8992a, 
		0x00000000, 
		0xcb000000, 
		0xc3142e0b, 
		0x0fc5b992, 
		0x294f364e, 
		0x80000100, 
		0x6e9d9068, 
		0xb6a3e7fd, 
		0x7f7ffffe, 
		0x0e1fffff, 
		0x42780000, 
		0x80020000, 
		0x0c7f0000, 
		0x0e0fffff, 
		0x0e01ffff, 
		0x7f800000, 
		0xb57cbff2, 
		0x80800001, 
		0x420c0000, 
		0x7f800000, 
		0x41e80000, 
		0x0e003fff, 
		0x05543e7b, 
		0x33333333, 
		0x426c0000, 
		0xc1f80000, 
		0x0e1fffff, 
		0x9106098c, 
		0x505168e1, 
		0x2891f894, 
		0x80000000, 
		0x0c7ffc00, 
		0xff800000, 
		0x41800000, 
		0x421c0000, 
		0xc1100000, 
		0x00000020, 
		0xbf028f5c, 
		0x7f7ffffe, 
		0x3d46cbde, 
		0xc2580000, 
		0x00800002, 
		0x41880000, 
		0xc5da6587, 
		0xf1697745, 
		0x00800002, 
		0x6d3ba8c5, 
		0x42580000, 
		0x42080000, 
		0x00298ab4, 
		0xb98973a6, 
		0x42340000, 
		0x0e1fffff, 
		0xc2480000, 
		0x007ffffe, 
		0x7f800000, 
		0x00ffffff, 
		0x0eb3dab9, 
		0x41700000, 
		0x42200000, 
		0xc2300000, 
		0x80011111, 
		0xbf800001, 
		0x0c7fffc0, 
		0x0c7ff000, 
		0x0d00fff0, 
		0x00400000, 
		0xaaaaaaaa, 
		0x00000000, 
		0x80000200, 
		0x00000002, 
		0x7ac5629b, 
		0x80008000, 
		0x807ffffe, 
		0x80000000, 
		0x80000020, 
		0x0e3fffff, 
		0x905e1473, 
		0x41800000, 
		0x80000000, 
		0xff800000, 
		0xc2740000, 
		0x80800000, 
		0x8b5dd09a, 
		0x80000080, 
		0xc1a00000, 
		0x7fc00001, 
		0x3f800000, 
		0x80000010, 
		0xff800000, 
		0x2fdf1229, 
		0x7f800000, 
		0x80000008, 
		0x73b5cd8c, 
		0xc2700000, 
		0x00000400, 
		0x2b62e41e, 
		0x00000000, 
		0x78f2246a, 
		0x945160b1, 
		0x00ffffff, 
		0x007ffffe, 
		0x14a2c3d1, 
		0x00000000, 
		0x00800002, 
		0x56177ab2, 
		0x00800001, 
		0x000bb814, 
		0x80000000, 
		0x0c7ff800, 
		0xcccccccc, 
		0xc23c0000, 
		0xe54ac8a0, 
		0xfe1c2769, 
		0x42740000, 
		0x8c073a88, 
		0x0c7fc000, 
		0xba77f078, 
		0x40000000, 
		0x8f7ffffc, 
		0xc2400000, 
		0x0c7fe000, 
		0x42340000, 
		0xbd382c8e, 
		0x80000000, 
		0x80000000, 
		0x35faa571, 
		0xc1400000, 
		0xff800000, 
		0x80800003, 
		0x42680000, 
		0x00011111, 
		0x73b9f6d6, 
		0xc2080000, 
		0x00100000, 
		0xff800000, 
		0xc2540000, 
		0x92010a0f, 
		0xc86002ec, 
		0xbf3adc42, 
		0xff9ce1d9, 
		0xc23c0000, 
		0xc0e00000, 
		0x421c0000, 
		0x7f800000, 
		0x41f80000, 
		0xcc7cc1d5, 
		0x0e0003ff, 
		0xff800001, 
		0xc2780000, 
		0x80000000, 
		0x42340000, 
		0xc1400000, 
		0x88a0cea1, 
		0x7fbfffff, 
		0x00800001, 
		0x80080000, 
		0x6d1bbe97, 
		0x42080000, 
		0x6217682c, 
		0x00020000, 
		0x00800002, 
		0x0342c29f, 
		0xc26c0000, 
		0xf952a677, 
		0xbf800001, 
		0xc1000000, 
		0xc1d80000, 
		0x5f88f2a9, 
		0x80000001, 
		0x1b4dbf3b, 
		0x0e0000ff, 
		0xc1100000, 
		0xba326e16, 
		0x0f7ffffd, 
		0x244645c4, 
		0xff000000, 
		0x0c7fff00, 
		0xfffb5f8e, 
		0x42200000, 
		0x42240000, 
		0x00000040, 
		0x7cfd1387, 
		0x0e01ffff, 
		0x8ef58200, 
		0x9169f627, 
		0x0c7fff80, 
		0x8ffd9af0, 
		0x80080000, 
		0xaba7f8a5, 
		0xff800000, 
		0x42040000, 
		0x80000000, 
		0x41300000, 
		0x7f800000, 
		0x0d000ff0, 
		0x7f800000, 
		0x0469c4bd, 
		0x7f800000, 
		0xff800000, 
		0x80008000, 
		0x26e25a89, 
		0x00000000, 
		0xd9605016, 
		0x7f800000, 
		0xfa611c89, 
		0x0e00007f, 
		0x1e8c1159, 
		0x0c7ffff0, 
		0x134149d1, 
		0xd053fb85, 
		0x41d80000, 
		0x80ffffff, 
		0xc2540000, 
		0xc1000000, 
		0xc22c0000, 
		0x37390eae, 
		0x41c80000, 
		0xcdb81e62, 
		0x424c0000, 
		0xc1d00000, 
		0x5d09447b, 
		0x9a45e0f3, 
		0x80463a3a, 
		0xffb46a09, 
		0xfaada6e1, 
		0x80800003, 
		0x7888747b, 
		0x0c7ffff0, 
		0x80000000, 
		0x73cde918, 
		0xc1e00000, 
		0x00ffffff, 
		0xe8801f3c, 
		0x80000100, 
		0x42580000, 
		0xc2400000, 
		0xcda59b5c, 
		0x00800000, 
		0x03d7a15d, 
		0xe7a21a94, 
		0x8e8cef06, 
		0x42480000, 
		0x0c7ff800, 
		0xff800000, 
		0x80040000, 
		0x42780000, 
		0x33333333, 
		0x80011111, 
		0x00080000, 
		0xc2280000, 
		0x80000000, 
		0x00000000, 
		0x41c80000, 
		0xff921e10, 
		0x3e9d3d44, 
		0x0c7ff000, 
		0x67ca3a8a, 
		0x00000100, 
		0x0e01ffff, 
		0x80bec8e9, 
		0x6b5e2118, 
		0xc0800000, 
		0xffa9edcd, 
		0x7f800000, 
		0x80000040, 
		0xe58a8ba0, 
		0x53e156e0, 
		0x0c7ffff8, 
		0x0e01ffff, 
		0x6ead61bf, 
		0x7b670644, 
		0xf9380696, 
		0x00000040, 
		0x353ac8dc, 
		0x41b80000, 
		0x80608c75, 
		0x41600000, 
		0xff800000, 
		0x7fc00001, 
		0x00000000, 
		0xff800000, 
		0xc1a00000, 
		0x00000000, 
		0x7f800000, 
		0x80800001, 
		0xdf0c2003, 
		0x80000020, 
		0x0f7ffffd, 
		0x0c7ffff0, 
		0x75a0112a, 
		0x9ea5a8af, 
		0x291c1121, 
		0x4977cc4c, 
		0xff7ffffe, 
		0x2bc7642c, 
		0xc22c0000, 
		0x3f800001, 
		0x7f800000, 
		0xaaaaaaaa, 
		0x0c7fff80, 
		0x80000040, 
		0x00008000, 
		0xc2280000, 
		0x3a05a4c4, 
		0xc1f00000, 
		0xff800000, 
		0x0f7fffff, 
		0xcb8c4b40, 
		0x42500000, 
		0xc24c0000, 
		0xfd86d47c, 
		0x80000000, 
		0x7fffffff, 
		0x00020000, 
		0xeb1734b2, 
		0xd424657c, 
		0x80011111, 
		0x40c00000, 
		0x0c700000, 
		0xca6ec735, 
		0x80800000, 
		0xeadff551, 
		0x7fa5059d, 
		0xfd52bb2d, 
		0x00011111, 
		0x18cfe547, 
		0xc3e16ba4, 
		0x80001000, 
		0x00000000, 
		0x80000200, 
		0xf34b7bcd, 
		0x00011111, 
		0x7fc00001, 
		0x80800001, 
		0x80000000, 
		0x8f7ffffe, 
		0x4bc9107f, 
		0x5bebd8bc, 
		0x2426301a, 
		0x7f800000, 
		0xffc00000, 
		0x0e0003ff, 
		0x7f800000, 
		0x80000000, 
		0xa6e0713a, 
		0x40e00000, 
		0x41f80000, 
		0x80800003, 
		0x0c7ff000, 
		0x80000000, 
		0x3f800000, 
		0x7927151d, 
		0x3f800001, 
		0x80579c52, 
		0x7f800000, 
		0x80774cda, 
		0x0e0001ff, 
		0xc2700000, 
		0x42740000, 
		0x00004000, 
		0x3492e9c1, 
		0x26223a9a, 
		0xc2640000, 
		0x00800001, 
		0x0c7ffc00, 
		0x88d297c0, 
		0x41e00000, 
		0x6b49857d, 
		0xffb5cb97, 
		0x78147839, 
		0xbbfdb623, 
		0x80000001, 
		0x00000000, 
		0x5500417d, 
		0x7f800000, 
		0xcccccccc, 
		0x0c600000, 
		0xc2700000, 
		0x0c600000, 
		0x40c00000, 
		0xffaf4b34, 
		0x0f35f2ae, 
		0x41800000, 
		0x80000001, 
		0x42180000, 
		0xffefc7bd, 
		0x00011111, 
		0xc0c00000, 
		0xc28300ef, 
		0x41a80000, 
		0x0c7fffc0, 
		0x00000200, 
		0x0c7fffff, 
		0x426c0000, 
		0xffc00001, 
		0x48b711e3, 
		0x80412d0c, 
		0x0c7ffffc, 
		0x80800003, 
		0x00000000, 
		0x00000000, 
		0xffbf8e15, 
		0xc26c0000, 
		0x857c0f1e, 
		0x41d80000, 
		0x41a80000, 
		0x426c0000, 
		0xc1d00000, 
		0x3b163dec, 
		0xddd9eaef, 
		0x7f800000, 
		0x2001c591, 
		0xc2400000, 
		0x0e0003ff, 
		0x0e3fffff, 
		0xff800000, 
		0x730c4a9a, 
		0x80000000, 
		0xff800000, 
		0x80800000, 
		0xe824aec5, 
		0x45b20387, 
		0xf87c1c3d, 
		0x0d00fff0, 
		0x41e00000, 
		0x80000200, 
		0xc1200000, 
		0xc1400000, 
		0x427c0000, 
		0x80800000, 
		0xc2400000, 
		0x00000004, 
		0x426c0000, 
		0x5b7099ac, 
		0x4efed41b, 
		0xffacd8d9, 
		0x33263b76, 
		0x427c0000, 
		0x42580000, 
		0xffbffc74, 
		0xc1c80000, 
		0x0c7f0000, 
		0x80080000, 
		0x9e4081eb, 
		0x7fa006b9, 
		0x0965ef3d, 
		0x0e03ffff, 
		0x7fbfffff, 
		0x59764f60, 
		0x41b00000, 
		0x80000000, 
		0xc1500000, 
		0x42500000, 
		0x80000008, 
		0xf573bb3a, 
		0x421c0000, 
		0x3f800000, 
		0x0e0000ff, 
		0xd29b8ad1, 
		0x42540000, 
		0xc6f4091b, 
		0x80000008, 
		0xfe54f98f, 
		0xbdfabbf9, 
		0x7f800000, 
		0xff7fffff, 
		0x0e001fff, 
		0x8d7f6a98, 
		0xff000000, 
		0xc22c0000, 
		0x41f00000, 
		0xcccccccc, 
		0x55555555, 
		0x7fb46b76, 
		0x52f1351d, 
		0xbb1ef370, 
		0xaf25ce43, 
		0xfb92caf0, 
		0x00000001, 
		0x0e00ffff, 
		0xc1980000, 
		0x00800000, 
		0x6f69932e, 
		0xc2680000, 
		0x807fffff, 
		0x3f800000, 
		0x41880000, 
		0x80000010, 
		0x0e007fff, 
		0xc1980000, 
		0xd6db50a8, 
		0x44fd9519, 
		0x42960182, 
		0x00000000, 
		0x00000001, 
		0x12115ece, 
		0x80000004, 
		0x7f7ffffe, 
		0x7708fba2, 
		0xbf028f5c, 
		0x80000000, 
		0x41880000, 
		0x00000010, 
		0x00000000, 
		0xde61c6db, 
		0x00ffffff, 
		0xc0e00000, 
		0xc1d80000, 
		0x5581e555, 
		0xc1e00000, 
		0x80000008, 
		0xc2480000, 
		0xc1900000, 
		0x7f800001, 
		0xff800000, 
		0xe1bee225, 
		0x80000008, 
		0x42240000, 
		0xf110c1af, 
		0xa450a76e, 
		0x0f7ffffd, 
		0x87761fb6, 
		0x0f7fffff, 
		0xff7ffffe, 
		0x00282035, 
		0x00040000, 
		0x00000004, 
		0x00000000, 
		0xc1b80000, 
		0x00001000, 
		0xfed0ec16, 
		0x7f803f33, 
		0xc1c80000, 
		0xda99d4f1, 
		0xc2000000, 
		0x6d9b02d6, 
		0x42700000, 
		0x0c7f0000, 
		0x0c7fe000, 
		0x007ffffe, 
		0x41c80000, 
		0x42080000, 
		0x80800000, 
		0xc2780000, 
		0x41600000, 
		0x42580000, 
		0x689b5f77, 
		0xc23c0000, 
		0x03490497, 
		0xecd768f7, 
		0x00000000, 
		0x27007aad, 
		0x6aefdafa, 
		0xc2040000, 
		0x7f800000, 
		0x7f7ffffe, 
		0xc41d4156, 
		0x80000200, 
		0xc5088908, 
		0x80800001, 
		0xb0738abb, 
		0xff800000, 
		0xc2380000, 
		0xcf3201b5, 
		0x7f852536, 
		0x8f7ffffe, 
		0xcb8c4b40, 
		0xd5e8ae78, 
		0x38d07a47, 
		0x00000000, 
		0xebeb0515, 
		0x6c0d5329, 
		0xe5b3a232, 
		0xc2640000, 
		0x00000000, 
		0x2d52234f, 
		0x49dd939c, 
		0x42100000, 
		0xcd7fc6bb, 
		0x41a00000, 
		0x5b9c6a98, 
		0xbd1b551e, 
		0x00000020, 
		0x41c00000, 
		0x80000010, 
		0x00000000, 
		0xcb000000, 
		0x4b000000, 
		0x2b3b93f4, 
		0x922eff2c, 
		0x66513f8a, 
		0x80000000, 
		0xbf028f5c, 
		0x2d1fafbe, 
		0xc1d00000, 
		0x41980000, 
		0x3445c993, 
		0x80000000, 
		0x42040000, 
		0xd0756d25, 
		0x8c89b987, 
		0x0f7ffffc, 
		0x421c0000, 
		0xcb000000, 
		0x7fa29c71, 
		0x1c302977, 
		0xe439108a, 
		0x422c0000, 
		0x0e03ffff, 
		0x41e80000, 
		0xc2180000, 
		0xc25c0000, 
		0xeda852f4, 
		0x2d72ee57, 
		0x80000000, 
		0x0e003fff, 
		0x239e9e99, 
		0xc25c0000, 
		0x42200000, 
		0x0e00ffff, 
		0x41d00000, 
		0x00000000, 
		0xc26c0000, 
		0x7fc00000, 
		0x42500000, 
		0x8623bc69, 
		0x41a80000, 
		0xfc471098, 
		0x00400000, 
		0x80800000, 
		0xc2080000, 
		0xbadf226d, 
		0x802b819e, 
		0xc23c0000, 
		0x42480000, 
		0x00800001, 
		0xc2400000, 
		0x00000001, 
		0x2058de88, 
		0x80000001, 
		0x41500000, 
		0x0c7c0000, 
		0x7f800000, 
		0x7f800000, 
		0xc1d80000, 
		0x0c7ff800, 
		0x00020000, 
		0x80000020, 
		0x2540babd, 
		0xfbaddbcc, 
		0x80000001, 
		0x7f7fffff, 
		0x0e0007ff, 
		0x80100000, 
		0xc1400000, 
		0x0c7fe000, 
		0x80001000, 
		0x9cdac1b3, 
		0x00800000, 
		0x62b0d256, 
		0x9544461e, 
		0x00000000, 
		0x42040000, 
		0xc0a00000, 
		0xc1800000, 
		0xc2380000, 
		0x00000200, 
		0xda66ffb9, 
		0x1208f7eb, 
		0x0cfaca35, 
		0x80000100, 
		0x007fffff, 
		0xe258e9a2, 
		0x41f80000, 
		0x1a161077, 
		0xc20c0000, 
		0x5ecdb141, 
		0x42080000, 
		0x80000080, 
		0x422c0000, 
		0x00000000, 
		0x7f800000, 
		0xe42f9f11, 
		0xffc00000, 
		0x329a6486, 
		0x00000200, 
		0xc2380000, 
		0x7f97c6b4, 
		0x2b986806, 
		0xc24c0000, 
		0x00000000, 
		0x01655f96, 
		0xc0e00000, 
		0x80000001, 
		0xf9b9fe20, 
		0x0f7ffffe, 
		0x0d000ff0, 
		0x80ffffff, 
		0x63d24fb5, 
		0x72143113, 
		0x80000000, 
		0x41c00000, 
		0xc2080000, 
		0x0e0fffff, 
		0xd67434fd, 
		0x423c0000, 
		0x0e7fffff, 
		0x42140000, 
		0xc0c00000, 
		0x0e0007ff, 
		0x007fffff, 
		0x26b7337a, 
		0xc1f80000, 
		0x00800001, 
		0x804a9680, 
		0x00000000, 
		0xc2600000, 
		0xdc62fbd3, 
		0x0b03f82b, 
		0xc26c0000, 
		0x423c0000, 
		0x87977c4f, 
		0x0c7ffe00, 
		0xc3168327, 
		0x00000020, 
		0xf750e659, 
		0x1ecf0b5f, 
		0x00000000, 
		0x55555555, 
		0x40c00000, 
		0x42140000, 
		0x80004000, 
		0xc2700000, 
		0x41c00000, 
		0x7f7ffffe, 
		0xb91f570f, 
		0xc1c80000, 
		0xdd8ebc97, 
		0xc1200000, 
		0x80800000, 
		0x80000000, 
		0x7f800000, 
		0x80200000, 
		0xac290dca, 
		0x41900000, 
		0x80000000, 
		0xc1900000, 
		0x0c700000, 
		0xff800000, 
		0x41f80000, 
		0xec759bf2, 
		0x00000000, 
		0x0c780000, 
		0x1545314c, 
		0x01afd09a, 
		0x80000800, 
		0xaf65e99a, 
		0xc1300000, 
		0x8d454e85, 
		0x41c80000, 
		0xff800000, 
		0x41400000, 
		0xc25c0000, 
		0x00000001, 
		0xff800001, 
		0xefe0d17f, 
		0x6574f19f, 
		0x80000800, 
		0x09b12bd4, 
		0x80800002, 
		0x41800000, 
		0x41b00000, 
		0xff000000, 
		0xfc059f4e, 
		0x41500000, 
		0xffb8546c, 
		0x7fc00000, 
		0xc1880000, 
		0x80000002, 
		0x0e000003, 
		0x00000000, 
		0x80100000, 
		0x4f3ef59e, 
		0xff800000, 
		0x80000004, 
		0x42440000, 
		0x0c7e0000, 
		0x00800000, 
		0x80800000, 
		0xc22c0000, 
		0xa49dd0b0, 
		0xc2180000, 
		0x30fd322a, 
		0x42440000, 
		0x42200000, 
		0xff800000, 
		0x0c7ffff0, 
		0x7a33925f, 
		0x42740000, 
		0x7f7ffffe, 
		0x00002000, 
		0x00000040, 
		0x0e0001ff, 
		0x43bbe2de, 
		0x54a8f82b, 
		0xefd8aa97, 
		0x7f7ffffe, 
		0x007fffff, 
		0x00800000, 
		0x80800003, 
		0xe54f5792, 
		0x40c00000, 
		0x9b4bef3b, 
		0xcccccccc, 
		0xe71cb27f, 
		0x007fffff, 
		0x80000000, 
		0xc0e00000, 
		0x41b80000, 
		0xeba65439, 
		0x872805c8, 
		0xf0259364, 
		0x0e03ffff, 
		0x00400000, 
		0x42780000, 
		0x1f5d0a40, 
		0xfff699d3, 
		0x42100000, 
		0x42140000, 
		0xf2e2d7fa, 
		0x7f800000, 
		0x607efdba, 
		0x424c0000, 
		0x00000800, 
		0x0c7fff00, 
		0x0e07ffff, 
		0x9ef14095, 
		0x6efb0592, 
		0x00000000, 
		0x887a6b52, 
		0x0c7ffffc, 
		0xc4001dcf, 
		0x1dc1f861, 
		0x0c700000, 
		0xfe67fc80, 
		0x00040000, 
		0x10895540, 
		0xc2340000, 
		0x2cd45600, 
		0x80000002, 
		0x8a626f7c, 
		0x0b1f0359, 
		0xceff6c94, 
		0xfb1c1660, 
		0x717de348, 
		0xd5c83b66, 
		0x80000000, 
		0x0e07ffff, 
		0x67fb43e6, 
		0x80000000, 
		0x80000001, 
		0x0c7fff00, 
		0x8f7ffffd, 
		0xc2580000, 
		0x80000000, 
		0xa0691920, 
		0x4ffa101b, 
		0xc1a00000, 
		0x423c0000, 
		0xdb705f60, 
		0xc1a00000, 
		0x80000000, 
		0xcb000000, 
		0x04b3d9f9, 
		0xc2700000, 
		0x2af3cfe4, 
		0x41e80000, 
		0x0c7ffff8, 
		0x807fffff, 
		0x0c700000, 
		0x40800000, 
		0x3f028f5c, 
		0x00000000, 
		0x80000100, 
		0x7a9b2bac, 
		0xff800000, 
		0x7f7ffffe, 
		0x42180000, 
		0x33333333, 
		0x80200000, 
		0x7f000000, 
		0x80800001, 
		0xaaaaaaaa, 
		0xa2276473, 
		0x91e4fe07, 
		0xff000000, 
		0x88565dd9, 
		0x42500000, 
		0x004ba040, 
		0x2bab8799, 
		0x0b489b25, 
		0xff9d0abb, 
		0x6d8006d3, 
		0xc1e80000, 
		0x3d28ce73, 
		0x41e00000, 
		0xff800000, 
		0xffbfffff, 
		0x3d976307, 
		0x42480000, 
		0x00000001, 
		0x3f584f83, 
		0x61b96ec5, 
		0xe1e12812, 
		0x7f7fffff, 
		0x80000010, 
		0xac16b6c8, 
		0x80000000, 
		0x7f98b1c1, 
		0xfaff3fdf, 
		0xc2780000, 
		0x0f7ffffc, 
		0xf98075a9, 
		0x00000200, 
		0x80080000, 
		0xc2700000, 
		0xc2780000, 
		0xb53f6b0e, 
		0x42300000, 
		0xa7973da4, 
		0x41a80000, 
		0x7228f9f2, 
		0x0e0003ff, 
		0x3f028f5c, 
		0x7fa1b29e, 
		0x42300000, 
		0xac7f4bd5, 
		0x8109cd2d, 
		0x80000000, 
		0x80800002, 
		0xc2380000, 
		0x589f741f, 
		0xdd6506e4, 
		0xc1f80000, 
		0x80000010, 
		0x7cd29079, 
		0x35ce036c, 
		0x42480000, 
		0xc1100000, 
		0xc2480000, 
		0xb76b876f, 
		0x80080000, 
		0xd19a1f7b, 
		0x42540000, 
		0x41300000, 
		0x42200000, 
		0x80000000, 
		0xc1a00000, 
		0x42300000, 
		0x00800002, 
		0xc0e00000, 
		0x4e0dbe71, 
		0xc2380000, 
		0xf0dca165, 
		0xc2180000, 
		0xc26c0000, 
		0x80000800, 
		0x0e07ffff, 
		0x0e0003ff, 
		0xc2140000, 
		0xbf800000, 
		0x80800001, 
		0x00800003, 
		0xffbfffff, 
		0x00400000, 
		0x2936a3e8, 
		0x80000800, 
		0xaaaaaaaa, 
		0x0c700000, 
		0x7fc00001, 
		0x8f5283c5, 
		0x0e00003f, 
		0x80000040, 
		0x3432d75c, 
		0x41c80000, 
		0x80800001, 
		0xc2440000, 
		0x444c8a08, 
		0x80008000, 
		0x42000000, 
		0xc2080000, 
		0x00000000  
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
		0x0000030d, 
		0x0000027b, 
		0x0000023f, 
		0x00000015, 
		0x000003b1, 
		0x000003c9, 
		0x0000039f, 
		0x00000027, 
		0x0000024d, 
		0x0000001f, 
		0x0000013b, 
		0x000002eb, 
		0x000000a7, 
		0x0000022b, 
		0x00000347, 
		0x0000015d, 
		0x000002a9, 
		0x00000325, 
		0x0000030f, 
		0x00000175, 
		0x0000020b, 
		0x0000032b, 
		0x000001df, 
		0x000003a9, 
		0x000000e7, 
		0x0000005f, 
		0x000003cf, 
		0x000000eb, 
		0x0000039b, 
		0x000002ab, 
		0x000002a9, 
		0x00000175, 
		0x000003c1, 
		0x00000055, 
		0x000000bb, 
		0x0000021d, 
		0x00000023, 
		0x0000023d, 
		0x000002f7, 
		0x000003f3, 
		0x0000027f, 
		0x0000007d, 
		0x00000099, 
		0x00000123, 
		0x00000225, 
		0x000001d7, 
		0x0000006f, 
		0x0000038d, 
		0x0000029b, 
		0x00000083, 
		0x0000018d, 
		0x00000385, 
		0x0000023d, 
		0x000000e5, 
		0x00000291, 
		0x000001ab, 
		0x000002fb, 
		0x000000c1, 
		0x00000263, 
		0x0000012f, 
		0x00000087, 
		0x00000097, 
		0x000002e9, 
		0x0000011d, 
		0x000001c7, 
		0x000003b1, 
		0x0000025d, 
		0x00000087, 
		0x000002f1, 
		0x0000017d, 
		0x0000035b, 
		0x000002dd, 
		0x000003bb, 
		0x000002c7, 
		0x00000315, 
		0x0000026d, 
		0x00000199, 
		0x00000115, 
		0x0000007b, 
		0x000003ff, 
		0x000001a1, 
		0x00000315, 
		0x000003fb, 
		0x0000036d, 
		0x0000015f, 
		0x000003c1, 
		0x00000031, 
		0x0000005f, 
		0x000001a7, 
		0x00000045, 
		0x00000117, 
		0x0000010f, 
		0x000001f7, 
		0x00000203, 
		0x00000357, 
		0x00000305, 
		0x000003f1, 
		0x00000353, 
		0x000003f9, 
		0x000003e3, 
		0x0000010b, 
		0x000003d3, 
		0x00000305, 
		0x00000253, 
		0x00000189, 
		0x0000019f, 
		0x000000b1, 
		0x0000018f, 
		0x0000003f, 
		0x00000093, 
		0x000003bb, 
		0x000003d5, 
		0x000001f7, 
		0x0000025b, 
		0x0000025d, 
		0x000000cb, 
		0x000001e1, 
		0x000001ad, 
		0x00000067, 
		0x00000309, 
		0x000003e7, 
		0x0000014d, 
		0x00000073, 
		0x0000035f, 
		0x000000ed, 
		0x00000347, 
		0x0000026d, 
		0x000002cd, 
		0x0000011f, 
		0x0000007f, 
		0x000002e7, 
		0x00000307, 
		0x000002f1, 
		0x000000c3, 
		0x000002fd, 
		0x0000035d, 
		0x000000c3, 
		0x00000225, 
		0x0000037b, 
		0x0000008f, 
		0x00000341, 
		0x00000363, 
		0x00000259, 
		0x00000163, 
		0x0000011f, 
		0x0000016d, 
		0x000001f5, 
		0x000003f5, 
		0x0000039b, 
		0x0000026f, 
		0x00000127, 
		0x00000239, 
		0x00000379, 
		0x0000022b, 
		0x00000227, 
		0x00000097, 
		0x00000175, 
		0x000002bf, 
		0x00000177, 
		0x0000025f, 
		0x00000025, 
		0x000002a3, 
		0x000000c1, 
		0x0000007f, 
		0x000003eb, 
		0x0000031d, 
		0x0000007b, 
		0x00000299, 
		0x00000165, 
		0x00000261, 
		0x000001ab, 
		0x0000021d, 
		0x000003c3, 
		0x000001b9, 
		0x0000029b, 
		0x00000157, 
		0x00000309, 
		0x00000311, 
		0x00000097, 
		0x000001b5, 
		0x00000051, 
		0x00000359, 
		0x00000327, 
		0x000002d7, 
		0x000000bf, 
		0x0000036f, 
		0x000000cf, 
		0x0000013b, 
		0x0000018f, 
		0x000002dd, 
		0x00000235, 
		0x0000003d, 
		0x00000261, 
		0x00000031, 
		0x00000385, 
		0x00000355, 
		0x000000c3, 
		0x000001eb, 
		0x00000237, 
		0x000000e5, 
		0x00000393, 
		0x0000021f, 
		0x000003f3, 
		0x000001fb, 
		0x0000025b, 
		0x00000311, 
		0x00000089, 
		0x000001af, 
		0x00000367, 
		0x000002b5, 
		0x000001d9, 
		0x000001f3, 
		0x00000279, 
		0x00000195, 
		0x00000227, 
		0x00000311, 
		0x00000083, 
		0x00000047, 
		0x000001af, 
		0x0000035f, 
		0x00000257, 
		0x000002cd, 
		0x000002ff, 
		0x0000022b, 
		0x0000014d, 
		0x00000099, 
		0x00000309, 
		0x000000e7, 
		0x000000cb, 
		0x000003c9, 
		0x000003cf, 
		0x000002bf, 
		0x000003f9, 
		0x00000105, 
		0x000000cf, 
		0x000000fb, 
		0x00000331, 
		0x000000e5, 
		0x000001cb, 
		0x00000397, 
		0x00000001, 
		0x00000205, 
		0x0000027d, 
		0x000003df, 
		0x00000287, 
		0x0000026f, 
		0x000001e3, 
		0x00000177, 
		0x00000255, 
		0x000003a9, 
		0x00000159, 
		0x00000117, 
		0x00000045, 
		0x000002c3, 
		0x00000197, 
		0x00000379  
};

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
			"csrwi mhpmevent4, 26\n"   
			"csrwi mhpmevent5, 19\n"   
			"csrwi mhpmevent6, 15\n"   
			"csrwi mhpmevent7, 5\n"   
			"csrwi mhpmevent8, 4\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f14,  384 (x5)\n"
			"flw.ps f9,  352 (x5)\n"
			"flw.ps f29,  256 (x5)\n"
			"flw.ps f30,  0 (x5)\n"
			"flw.ps f7,  896 (x5)\n"
			"flw.ps f28,  448 (x5)\n"
			"flw.ps f16,  640 (x5)\n"
			"flw.ps f10,  992 (x5)\n"
			"flw.ps f31,  160 (x5)\n"
			"flw.ps f19,  608 (x5)\n"
			"flw.ps f1,  768 (x5)\n"
			"flw.ps f15,  736 (x5)\n"
			"flw.ps f17,  64 (x5)\n"
			"flw.ps f8,  416 (x5)\n"
			"flw.ps f11,  96 (x5)\n"
			"flw.ps f5,  576 (x5)\n"
			"flw.ps f21,  928 (x5)\n"
			"flw.ps f0,  32 (x5)\n"
			"flw.ps f20,  320 (x5)\n"
			"flw.ps f26,  288 (x5)\n"
			"flw.ps f6,  224 (x5)\n"
			"flw.ps f2,  480 (x5)\n"
			"flw.ps f23,  864 (x5)\n"
			"flw.ps f3,  960 (x5)\n"
			"flw.ps f27,  800 (x5)\n"
			"flw.ps f22,  128 (x5)\n"
			"flw.ps f25,  192 (x5)\n"
			"flw.ps f12,  672 (x5)\n"
			"flw.ps f18,  544 (x5)\n"
			"flw.ps f24,  704 (x5)\n"
			"flw.ps f13,  512 (x5)\n"
			"flw.ps f4,  832 (x5)\n"
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
			"li x0, 0x320\n" 
			"slti x0, x0, 0x7f2\n"
			"la x31, check_sum_hid_0\n"
			"li x5, 0x1\n" 
			"sw x5, (x31)\n"
			"li x30, 0\n"
			"addi x30, x30, 1\n"
			"la x15, rand_ieee754_32\n"
			"flw.ps f27, 128(x15)\n"
			"flw.ps f19, 512(x15)\n"
			"feq.ps f27, f27, f19\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_0:\n"
			"maskxor m0, m4, m5\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x15, rand_ieee754_32\n"
			"flw.ps f31, 224(x15)\n"
			"flw.ps f18, 576(x15)\n"
			"fsgnjn.ps f18, f31, f18\n" 
			"la x12, rand_int_32\n"
			"flw.ps f27, 992(x12)\n"
			"flw.ps f6, 256(x12)\n"
			"fmin.pi f15, f27, f6\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f4, 864(x15)\n"
			"fround.ps f21, f4, rtz\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x12, rand_int_32\n"
			"flw.ps f17, 992(x12)\n"
			"for.pi f31, f17, f17\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f9, 928(x9)\n"
			"flw.ps f11, 256(x9)\n"
			"fcmov.ps f21, f9, f11, f11\n" 
			"la x15, rand_int_32\n"
			"flw.ps f9, 896(x15)\n"
			"flw.ps f21, 672(x15)\n"
			"fltu.pi f15, f9, f21\n" 
			"la x17, rand_int_32\n"
			"flw.ps f6, 704(x17)\n"
			"flw.ps f9, 864(x17)\n"
			"fle.pi f8, f6, f9\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f4, 192(x18)\n"
			"fswizz.ps f21, f4, 0x10\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f6, 192(x17)\n"
			"flw.ps f8, 256(x17)\n"
			"fsub.ps f20, f6, f8, rmm\n" 
			"li x5, 0x48ab\n"
			"csrw tensor_mask, x5\n"	
			"fbci.ps f13, 0x8cec\n" 
			"la x12, rand_int_32\n"
			"flw.ps f11, 608(x12)\n"
			"flw.ps f21, 96(x12)\n"
			"fmin.pi f18, f11, f21\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f27, 352(x26)\n"
			"flw.ps f6, 512(x26)\n"
			"flw.ps f18, 288(x26)\n"
			"fnmsub.s f27, f27, f6, f18, rtz\n" 
			"la x17, rand_int_32\n"
			"flw.ps f15, 832(x17)\n"
			"faddi.pi f21, f15, 0x1b9\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f20, 416(x19)\n"
			"fcvt.w.s x19, f20, rdn\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 0xfd14\n"
			"csrw tensor_mask, x5\n"	
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ITER_0_HID_0:\n"
			"li x5, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x0 << TENSOR_FMA32_B_NUM_COLS) | (0x3 << TENSOR_FMA32_A_NUM_ROWS) | (0x6 << TENSOR_FMA32_A_NUM_COLS) | (0x2 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0xc5 << TENSOR_FMA32_SCP_LOC_B) | (0x24 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ITER_1_HID_0:\n"
			"li x5, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x3 << TENSOR_FMA16_B_NUM_COLS) | (0xa << TENSOR_FMA16_A_NUM_ROWS) | (0xb << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x24 << TENSOR_FMA16_SCP_LOC_B) | (0x0 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x0 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ITER_2_HID_0:\n"
			"li x5, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x0 << TENSOR_IMA_B_NUM_COLS) | (0xc << TENSOR_IMA_A_NUM_ROWS) | (0x5 << TENSOR_IMA_A_NUM_COLS) | (0x7 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x84 << TENSOR_IMA_SCP_LOC_B) | (0x59 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x0 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ITER_3_HID_0:\n"
			"li x5, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x0 << TENSOR_FMA32_B_NUM_COLS) | (0xc << TENSOR_FMA32_A_NUM_ROWS) | (0xa << TENSOR_FMA32_A_NUM_COLS) | (0xc << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0xb << TENSOR_FMA32_SCP_LOC_B) | (0xcb << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x0 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ITER_4_HID_0:\n"
			"li x5, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x2 << TENSOR_FMA32_B_NUM_COLS) | (0xc << TENSOR_FMA32_A_NUM_ROWS) | (0x8 << TENSOR_FMA32_A_NUM_COLS) | (0xc << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x26 << TENSOR_FMA32_SCP_LOC_B) | (0x4b << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x0 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ITER_5_HID_0:\n"
			"li x5, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x0 << TENSOR_IMA_B_NUM_COLS) | (0x3 << TENSOR_IMA_A_NUM_ROWS) | (0x0 << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x0 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0xee << TENSOR_IMA_SCP_LOC_B) | (0xe1 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ITER_6_HID_0:\n"
			"li x5, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x3 << TENSOR_IMA_B_NUM_COLS) | (0x3 << TENSOR_IMA_A_NUM_ROWS) | (0xb << TENSOR_IMA_A_NUM_COLS) | (0xd << TENSOR_IMA_OFFSET) | (0x0 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0xb7 << TENSOR_IMA_SCP_LOC_B) | (0xb2 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ITER_7_HID_0:\n"
			"li x5, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x1 << TENSOR_FMA16_B_NUM_COLS) | (0x4 << TENSOR_FMA16_A_NUM_ROWS) | (0xc << TENSOR_FMA16_A_NUM_COLS) | (0x8 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0xad << TENSOR_FMA16_SCP_LOC_B) | (0xe1 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ITER_8_HID_0:\n"
			"li x5, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x1 << TENSOR_IMA_B_NUM_COLS) | (0x2 << TENSOR_IMA_A_NUM_ROWS) | (0x8 << TENSOR_IMA_A_NUM_COLS) | (0x7 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x5a << TENSOR_IMA_SCP_LOC_B) | (0x22 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x0 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
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
			"csrrci x0, mcache_control, 3\n"
			"csrwi tensor_wait, 0x6\n" 
			"fence\n"
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
			"LBL_HPM_3_CORE_TS_OPS_FAIL_HID_0:\n"          
			"csrr x5, mhpmcounter3\n"
			"sd x5, 32 (x31)\n"
			"LBL_HPM_4_CORE_TFMA_INST_FAIL_HID_0:\n"          
			"csrr x5, mhpmcounter4\n"
			"sd x5, 40 (x31)\n"
			"LBL_HPM_5_CORE_TIMA_OPS_FAIL_HID_0:\n"          
			"csrr x5, mhpmcounter5\n"
			"sd x5, 48 (x31)\n"
			"LBL_HPM_6_CORE_TL_OPS_FAIL_HID_0:\n"          
			"csrr x5, mhpmcounter6\n"
			"sd x5, 56 (x31)\n"
			"LBL_HPM_7_NEIGH_RX_COOP_TLD_RSP_FAIL_HID_0:\n"          
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
			"LBL_HPM_4_CORE_TFMA_INST_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter4\n"
			"sd x5, 40 (x31)\n"
			"LBL_HPM_5_CORE_TIMA_OPS_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter5\n"
			"sd x5, 48 (x31)\n"
			"LBL_HPM_6_CORE_TL_OPS_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter6\n"
			"sd x5, 56 (x31)\n"
			"LBL_HPM_7_NEIGH_RX_COOP_TLD_RSP_PASS_HID_0:\n"          
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
			"csrwi mhpmevent4, 11\n"   
			"csrwi mhpmevent5, 2\n"   
			"csrwi mhpmevent6, 25\n"   
			"csrwi mhpmevent7, 1\n"   
			"csrwi mhpmevent8, 2\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f10,  736 (x5)\n"
			"flw.ps f4,  128 (x5)\n"
			"flw.ps f18,  288 (x5)\n"
			"flw.ps f30,  896 (x5)\n"
			"flw.ps f25,  0 (x5)\n"
			"flw.ps f2,  96 (x5)\n"
			"flw.ps f31,  544 (x5)\n"
			"flw.ps f11,  384 (x5)\n"
			"flw.ps f26,  352 (x5)\n"
			"flw.ps f16,  640 (x5)\n"
			"flw.ps f0,  416 (x5)\n"
			"flw.ps f3,  480 (x5)\n"
			"flw.ps f15,  992 (x5)\n"
			"flw.ps f14,  224 (x5)\n"
			"flw.ps f8,  672 (x5)\n"
			"flw.ps f9,  832 (x5)\n"
			"flw.ps f23,  608 (x5)\n"
			"flw.ps f22,  448 (x5)\n"
			"flw.ps f7,  928 (x5)\n"
			"flw.ps f13,  800 (x5)\n"
			"flw.ps f27,  576 (x5)\n"
			"flw.ps f1,  704 (x5)\n"
			"flw.ps f21,  768 (x5)\n"
			"flw.ps f12,  256 (x5)\n"
			"flw.ps f17,  512 (x5)\n"
			"flw.ps f24,  320 (x5)\n"
			"flw.ps f29,  160 (x5)\n"
			"flw.ps f28,  960 (x5)\n"
			"flw.ps f19,  192 (x5)\n"
			"flw.ps f5,  64 (x5)\n"
			"flw.ps f6,  32 (x5)\n"
			"flw.ps f20,  864 (x5)\n"
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
			"la x15, rand_int_32\n"
			"flw.ps f12, 96(x15)\n"
			"flw.ps f25, 800(x15)\n"
			"fmax.pi f6, f12, f25\n" 
			"fbci.pi f17, 0x5795e\n" 
			"la x13, rand_int_32\n"
			"flw.ps f23, 320(x13)\n"
			"faddi.pi f27, f23, 0x14\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f16, 384(x9)\n"
			"flw.ps f0, 672(x9)\n"
			"flt.ps f22, f16, f0\n" 
			"LBL_SEQID_0_FP_TRANS_ID_0_HID_1:\n"
			"la x27, rand_ieee754_32\n"
			"flw.ps f6, 768(x27)\n"
			"fexp.ps f29, f6\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_1:\n"
			"fltm.pi m0, f16, f29\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x9, rand_ieee754_32\n"
			"flw.ps f20, 0(x9)\n"
			"fcvt.pwu.ps f6, f20, rne\n" 
			"la x27, rand_int_32\n"
			"flw.ps f3, 288(x27)\n"
			"fcvt.ps.pw f26, f3, rdn\n" 
			"la x7, rand_int_32\n"
			"flw.ps f0, 896(x7)\n"
			"flw.ps f22, 32(x7)\n"
			"fmul.pi f9, f0, f22\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f3, 128(x25)\n"
			"fcvt.pwu.ps f12, f3, rup\n" 
			"la x9, rand_int_32\n"
			"flw.ps f16, 0(x9)\n"
			"fsat8.pi f28, f16\n" 
			"LBL_SEQID_0_M0_WRITE_ID_2_HID_1:\n"
			"masknot m3, m0\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x27, rand_int_32\n"
			"flw.ps f20, 224(x27)\n"
			"flw.ps f29, 224(x27)\n"
			"fadd.pi f12, f20, f29\n" 
			"la x7, rand_int_32\n"
			"flw.ps f0, 512(x7)\n"
			"flw.ps f9, 576(x7)\n"
			"fmax.pi f17, f0, f9\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f23, 608(x7)\n"
			"flw.ps f17, 832(x7)\n"
			"flt.s x18, f23, f17\n" 
			"li x6, 0x85badcccd59c0f8c\n"
			"fcvt.s.lu f6, x6, rdn\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_1\n"
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
			"LBL_HPM_3_CORE_RETIRED_INST1_FAIL_HID_1:\n"          
			"csrr x5, mhpmcounter3\n"
			"sd x5, 32 (x31)\n"
			"LBL_HPM_4_CORE_L2_MISS_REQ_REJ_FAIL_HID_1:\n"          
			"csrr x5, mhpmcounter4\n"
			"sd x5, 40 (x31)\n"
			"LBL_HPM_5_CORE_RETIRED_INST0_FAIL_HID_1:\n"          
			"csrr x5, mhpmcounter5\n"
			"sd x5, 48 (x31)\n"
			"LBL_HPM_6_CORE_MASK_OPS_FAIL_HID_1:\n"          
			"csrr x5, mhpmcounter6\n"
			"sd x5, 56 (x31)\n"
			"LBL_HPM_7_NEIGH_ANY_CORE_TX_ETL_REQ_FAIL_HID_1:\n"          
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
			"LBL_HPM_3_CORE_RETIRED_INST1_PASS_HID_1:\n"          
			"csrr x5, mhpmcounter3\n"
			"sd x5, 32 (x31)\n"
			"LBL_HPM_4_CORE_L2_MISS_REQ_REJ_PASS_HID_1:\n"          
			"csrr x5, mhpmcounter4\n"
			"sd x5, 40 (x31)\n"
			"LBL_HPM_5_CORE_RETIRED_INST0_PASS_HID_1:\n"          
			"csrr x5, mhpmcounter5\n"
			"sd x5, 48 (x31)\n"
			"LBL_HPM_6_CORE_MASK_OPS_PASS_HID_1:\n"          
			"csrr x5, mhpmcounter6\n"
			"sd x5, 56 (x31)\n"
			"LBL_HPM_7_NEIGH_ANY_CORE_TX_ETL_REQ_PASS_HID_1:\n"          
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
		0x59f174cd, 
		0xb1af115b, 
		0xaae93a2f, 
		0x5bb77a49, 
		0x2703f48e, 
		0x9540917d, 
		0x286a9070, 
		0x370f2f9c, 
		0xe8ef5b7c, 
		0x140812e2, 
		0x9f93c5df, 
		0xb2bf6135, 
		0xefc1ef40, 
		0x1bee2b10, 
		0x7b73c20b, 
		0xb49b195a, 
		0x2b14ce4f, 
		0x438772e2, 
		0x615709a1, 
		0xadf6d48e, 
		0x1ace96d8, 
		0x5dc2b255, 
		0x3633a92d, 
		0x59234b4f, 
		0xa740c6ff, 
		0xf6ec13cd, 
		0xfc69198b, 
		0x79bd5230, 
		0xb35a85c0, 
		0x73757f85, 
		0x402dc9b9, 
		0x82bfaae6, 
		0x00dcc18b, 
		0xb0a31520, 
		0x3c37297d, 
		0x29e75497, 
		0x1ad3342c, 
		0x60e96ec8, 
		0x0ad5a390, 
		0x3c699360, 
		0x0634be6f, 
		0x4d9714ff, 
		0x7ff99a95, 
		0xa29d7a0a, 
		0x83bc8483, 
		0x1274fc35, 
		0x66422df3, 
		0x64156dc6, 
		0x87e171bd, 
		0x055619da, 
		0xefb8ebda, 
		0xc2f15545, 
		0x6d472b44, 
		0xbb0706ca, 
		0x9c75474c, 
		0xdb632173, 
		0x3eda1349, 
		0x2ab51479, 
		0xc9a189c7, 
		0xa03d92e4, 
		0x41442d27, 
		0x8022a78c, 
		0xcf9705d8, 
		0xb58949cc, 
		0x2b738484, 
		0xc55c40ca, 
		0x0a17ed0b, 
		0xc336315d, 
		0x14a1ed25, 
		0xaec22b05, 
		0x0646ea4c, 
		0x1d9089f9, 
		0xf6ca29ee, 
		0xb1a0e5e4, 
		0xc7eda254, 
		0xef6f19ab, 
		0x0387de3c, 
		0x71994aa3, 
		0x75078981, 
		0xdd90cebd, 
		0x7f4602ac, 
		0xdb5c6424, 
		0x213e74cc, 
		0xb30fad4b, 
		0xf03011da, 
		0xe4c9f18c, 
		0x09e857f5, 
		0xf96366a4, 
		0x54cd4026, 
		0x2493e56a, 
		0x69b2cf4e, 
		0x52723a04, 
		0xf39c9509, 
		0xb3d794e5, 
		0xa259ac51, 
		0x0e6ed6f8, 
		0xd6c48528, 
		0x5346e91a, 
		0xf444b1ee, 
		0xd0755a52, 
		0xd2cbf059, 
		0x118afaf6, 
		0x6dce62de, 
		0x0b8a5711, 
		0x9beb1b9b, 
		0x03ad4513, 
		0x7ccd45ef, 
		0x4e7c64d8, 
		0xcb256ec9, 
		0xee0dab46, 
		0x98276582, 
		0x44daca1e, 
		0x63cb4e9e, 
		0x70c2e6d1, 
		0xde95fce7, 
		0x4a22d0bd, 
		0xaf5a5354, 
		0xca0d3473, 
		0xc24311f5, 
		0x820b766a, 
		0x16143dfd, 
		0x52c1599c, 
		0xf2e5597d, 
		0xbd464038, 
		0x7b0bed71, 
		0xa6d1bad8, 
		0x447cf8c3, 
		0xa97a4292, 
		0xfa261499, 
		0xdf8607b6, 
		0x808de932, 
		0x7637753a, 
		0xe445059e, 
		0xe75dd029, 
		0x945f8782, 
		0xdbb66286, 
		0x519dff84, 
		0x2e5034bb, 
		0xb1b76ad5, 
		0xe885a50e, 
		0x24afe35b, 
		0xa4b87ce6, 
		0x6595a0a6, 
		0xdac0bfba, 
		0x37e13a93, 
		0x1cd7f7c6, 
		0x193e47fc, 
		0x0cadf5d3, 
		0x4710daf6, 
		0x84aac6a3, 
		0x8b75697b, 
		0xe0a9b8ef, 
		0xa6f4568d, 
		0xc894e7f4, 
		0x76c418e8, 
		0x2df35a83, 
		0x1bb5fe7b, 
		0x2bf42245, 
		0x48807216, 
		0x6925a039, 
		0x666999eb, 
		0x78b90ab5, 
		0xd1062c7c, 
		0x258d215e, 
		0x7cb67410, 
		0x21e0a742, 
		0x2f8e5218, 
		0x7d09fbfe, 
		0xff957338, 
		0x0e553a9a, 
		0x680e36b8, 
		0x2037ea38, 
		0xf7f75068, 
		0x3cf27836, 
		0x1a485bc2, 
		0x95856a3b, 
		0xee180082, 
		0xb46401d8, 
		0x7e97dcaa, 
		0xd536f590, 
		0x2f9d19f6, 
		0xde22fc96, 
		0x58efaba2, 
		0xfd23a370, 
		0x9685478a, 
		0x7c40fdc3, 
		0x0383263a, 
		0x2fd25013, 
		0xd3bd3880, 
		0x930a00b2, 
		0xe9ab6c0d, 
		0x50478f1f, 
		0xa4657293, 
		0x6f52f9f8, 
		0xe0465b75, 
		0xaca94f59, 
		0x0c0836a2, 
		0x63829c43, 
		0x32981a22, 
		0x0602be81, 
		0xc8b71e56, 
		0xdf27254e, 
		0x97892e9a, 
		0x5a8b8520, 
		0xe8b2b382, 
		0x968f2a77, 
		0xa18975c5, 
		0xad28c49a, 
		0x0447473d, 
		0xdb885101, 
		0x3b623495, 
		0x000856ce, 
		0xa4c79014, 
		0x486b49b3, 
		0xd50f12d5, 
		0xa80fc75c, 
		0x8630091b, 
		0x5ec33c67, 
		0x25c545e8, 
		0x7ed8cd80, 
		0x5631796c, 
		0xb8b7c410, 
		0xdc1abef2, 
		0xbaea22dd, 
		0x763f7749, 
		0x868fed4a, 
		0x6191337a, 
		0xd91cb89e, 
		0xed5f47d5, 
		0x25994afe, 
		0x89bc7893, 
		0xe62cf303, 
		0x8a5782be, 
		0x1998a775, 
		0x67dded33, 
		0x12652e69, 
		0xabbb74c0, 
		0x9db4f642, 
		0x9af9221f, 
		0xd2131231, 
		0x8a333555, 
		0x9f2e70ba, 
		0x12f380f1, 
		0xde6b44d1, 
		0xe2afb557, 
		0x7afed1f9, 
		0x1cc507f5, 
		0x0225eeed, 
		0xca50c4b6, 
		0x3c851125, 
		0x46a20a04, 
		0xb955a94c, 
		0x5556be00, 
		0xf31691bf, 
		0xdaa3d451, 
		0xdbfe63d5  
};
volatile uint32_t rand_ieee754_32[1536] __attribute__ ((aligned (2048))) = {
		0xc1f00000, 
		0x0c5006b6, 
		0x1a3a8836, 
		0x3f028f5c, 
		0x7fe58d47, 
		0x00008000, 
		0xc1200000, 
		0x8f7ffffe, 
		0x06b6899e, 
		0xfee9e649, 
		0x2cc98f74, 
		0x00000000, 
		0x420c0000, 
		0x80000000, 
		0x8c17791e, 
		0xc22c0000, 
		0x42400000, 
		0x80800001, 
		0x7fc00000, 
		0x0e00000f, 
		0x5eeebc01, 
		0x41880000, 
		0x64b13780, 
		0x0e00000f, 
		0x254e82df, 
		0x15ff32f4, 
		0x5e4c8a75, 
		0x42500000, 
		0x442bb6d9, 
		0x41980000, 
		0x42400000, 
		0x804cfdfb, 
		0xbd21b93a, 
		0x00000400, 
		0x0d000ff0, 
		0xff83fbe8, 
		0x80000040, 
		0xc2080000, 
		0x0e01ffff, 
		0x00800000, 
		0xb04911dd, 
		0x0c700000, 
		0xc192c7a8, 
		0x7f73d72a, 
		0xffc00001, 
		0xc1880000, 
		0x8957e7bf, 
		0x80ffffff, 
		0xc0613835, 
		0x41880000, 
		0x41c00000, 
		0x41980000, 
		0x0d000ff0, 
		0x00010000, 
		0x0c7c0000, 
		0xff7ffffe, 
		0xf059b393, 
		0x00000001, 
		0xceade0d3, 
		0x40c00000, 
		0x80800002, 
		0x40800000, 
		0x0e003fff, 
		0x80000000, 
		0x80020000, 
		0x00000020, 
		0x42780000, 
		0xff800000, 
		0x80000000, 
		0x80080000, 
		0x33b5645b, 
		0xc1800000, 
		0xff800000, 
		0xc2540000, 
		0xff7ffffe, 
		0x00ffffff, 
		0x00200000, 
		0x520f567f, 
		0x41000000, 
		0x80000001, 
		0x4b000000, 
		0x00800002, 
		0x42040000, 
		0x42300000, 
		0x80100000, 
		0x00000000, 
		0x00001000, 
		0x247e8faa, 
		0x7f7fffff, 
		0xffc00000, 
		0x00000000, 
		0xc2080000, 
		0x80000000, 
		0xc3b11cea, 
		0xc2000000, 
		0xc25c0000, 
		0x7fc00001, 
		0x00080000, 
		0x0c700000, 
		0xc2380000, 
		0x8f7fffff, 
		0x00001000, 
		0x00800000, 
		0xc2540000, 
		0xc2180000, 
		0x80000008, 
		0xa337cd2f, 
		0x0e427bc8, 
		0xc0e00000, 
		0x7f7ffffe, 
		0x8aad6acc, 
		0xc1980000, 
		0xbf028f5c, 
		0x007ffffe, 
		0x80000000, 
		0xf9178c71, 
		0x00400000, 
		0x42380000, 
		0x7ffd48b1, 
		0xc0c00000, 
		0x0e1fffff, 
		0x0c7fff80, 
		0xc2180000, 
		0x0c7f0000, 
		0xb4998376, 
		0xc2580000, 
		0x00000000, 
		0xc2780000, 
		0x40000000, 
		0x80000001, 
		0xdfb7661d, 
		0xff800000, 
		0x46eda585, 
		0xca4cf4e6, 
		0x426c0000, 
		0x420c0000, 
		0x41a0916b, 
		0x40a00000, 
		0x1bfbf025, 
		0x42200000, 
		0x454a4766, 
		0x80000000, 
		0xf1a9916a, 
		0x0e1fffff, 
		0x34d93a84, 
		0x41c00000, 
		0x41880000, 
		0x7f7ffffe, 
		0xff7fffff, 
		0x80000000, 
		0x0c700000, 
		0x7f800000, 
		0x00000004, 
		0xc24c0000, 
		0xecf5cc62, 
		0x7fbfffff, 
		0x00200000, 
		0x06982b25, 
		0xb1b18e1d, 
		0xc09f0009, 
		0xc1980000, 
		0x0e000003, 
		0x8f7fffff, 
		0x0c7ffff8, 
		0x00400000, 
		0x0e0001ff, 
		0x33ec79eb, 
		0x7f940dea, 
		0x84cdcbce, 
		0x3f800001, 
		0xf8eb1411, 
		0xc1400000, 
		0x0734b4d7, 
		0x807ffffe, 
		0x7f800000, 
		0x16fb6993, 
		0x80695750, 
		0x80000000, 
		0x55555555, 
		0xc2040000, 
		0x7f7fffff, 
		0x00000000, 
		0xe46b580f, 
		0x80000800, 
		0xc0a00000, 
		0x84f08b00, 
		0x723a2e56, 
		0x807fffff, 
		0x00000020, 
		0x49052ecd, 
		0x001d1c5a, 
		0x80ffffff, 
		0x40800000, 
		0x4e59cdb7, 
		0x0c780000, 
		0x807ffffe, 
		0x420c0000, 
		0x0f7ffffe, 
		0x42480000, 
		0x00100000, 
		0xd09fa27c, 
		0x57b56c18, 
		0xb1c7c9db, 
		0x80200000, 
		0x0e000001, 
		0x42080000, 
		0x9c6cc133, 
		0x20d68322, 
		0x5eecce60, 
		0x00000400, 
		0xff800000, 
		0x421c0000, 
		0xff800000, 
		0x80800001, 
		0x42580000, 
		0x40e00000, 
		0xc1800000, 
		0x2be82ea1, 
		0x0c7fffe0, 
		0x80800001, 
		0xc1800000, 
		0x0c7ffffc, 
		0xd8af8a5a, 
		0x00000000, 
		0x00800000, 
		0xc2380000, 
		0xab402868, 
		0x42500000, 
		0x0c7ff000, 
		0x807fffff, 
		0xc1d00000, 
		0xe364fd7e, 
		0xff7ffffe, 
		0x80020000, 
		0xedc4ac80, 
		0x85f508ae, 
		0x41d00000, 
		0x0c7ffffc, 
		0xc1800000, 
		0xff800000, 
		0x0e01ffff, 
		0x4cc6f2d8, 
		0x7f800000, 
		0xc1200000, 
		0x533ab74a, 
		0x80008000, 
		0x00000100, 
		0x80000001, 
		0xe6e9c273, 
		0xc2600000, 
		0xf99e0269, 
		0x00000000, 
		0x0e000fff, 
		0x80800000, 
		0x30d90bd5, 
		0x7f800000, 
		0xab45c37d, 
		0x0c7e0000, 
		0x1d58c27b, 
		0x00008000, 
		0xbf800000, 
		0x8ee12c45, 
		0xcccccccc, 
		0x00000040, 
		0x41c80000, 
		0x692afa34, 
		0x0e0000ff, 
		0x00004000, 
		0x00ffffff, 
		0xc2780000, 
		0xc1a00000, 
		0xff800000, 
		0x00100000, 
		0x00000010, 
		0x00800002, 
		0x42540000, 
		0xc1b00000, 
		0x67e26d49, 
		0x558f4ed6, 
		0x0f7fffff, 
		0x110c84ea, 
		0xdf5cc447, 
		0xd3c9a79c, 
		0x80ffffff, 
		0x8ecf7f9d, 
		0x41400000, 
		0xc2140000, 
		0x7f7fffff, 
		0xc1f00000, 
		0x422c0000, 
		0x80000000, 
		0x7f96cb91, 
		0x80000000, 
		0xc2740000, 
		0xc1900000, 
		0x1935696a, 
		0x41700000, 
		0x68f202bb, 
		0xff800000, 
		0x80000000, 
		0x003da55e, 
		0x0e0000ff, 
		0xcb89a738, 
		0xddef8788, 
		0xed5dffd6, 
		0xfdc70768, 
		0xc2140000, 
		0x73080690, 
		0x4b8c4b40, 
		0x07fa0c70, 
		0xd564b45e, 
		0xc0907aa8, 
		0xa5faa78c, 
		0x6a96d8ff, 
		0x80000000, 
		0x8f7ffffd, 
		0x41200000, 
		0xc1f00000, 
		0x742c26b4, 
		0x0e000fff, 
		0x7c28c1b0, 
		0x80000000, 
		0x42140000, 
		0x426c0000, 
		0x00400000, 
		0x41400000, 
		0x00011111, 
		0x00000008, 
		0x15f7fae4, 
		0x974e9fb6, 
		0xc2580000, 
		0x05378f0d, 
		0x00000000, 
		0x00000004, 
		0xff7fffff, 
		0x7f7fffff, 
		0x00000000, 
		0x00800003, 
		0x41c80000, 
		0xfc3342b5, 
		0xc1c80000, 
		0x41c80000, 
		0x7fc00001, 
		0x80000000, 
		0xc1600000, 
		0xc0e00000, 
		0x6218bc32, 
		0xde740e7b, 
		0xca6ae813, 
		0x7f800000, 
		0x00000001, 
		0x41e00000, 
		0x0d00fff0, 
		0x0e7fffff, 
		0x80d0b591, 
		0x80100000, 
		0x426c0000, 
		0xb12748e3, 
		0xb8865975, 
		0x420c0000, 
		0x42380000, 
		0x536340d5, 
		0xff800000, 
		0x7ff5983f, 
		0x03b179ee, 
		0x80000000, 
		0xc1f80000, 
		0xd60f0702, 
		0x42640000, 
		0x97c26a59, 
		0xc1e80000, 
		0xe642381d, 
		0xa66793f3, 
		0x80800001, 
		0x80200000, 
		0xc1d80000, 
		0x2024a65d, 
		0xc1900000, 
		0x80800001, 
		0xc1200000, 
		0xc2300000, 
		0x38a56ae5, 
		0x67f99961, 
		0x20360fbe, 
		0xc2340000, 
		0x0c600000, 
		0x00000001, 
		0x80000040, 
		0x80002000, 
		0xc1d80000, 
		0xc1c80000, 
		0x80000002, 
		0x54a4554d, 
		0xff800000, 
		0xbbaa85c5, 
		0x80100000, 
		0x401b3025, 
		0x0c7ffc00, 
		0x80800000, 
		0x0e000003, 
		0xc1980000, 
		0x0c7f8000, 
		0xc1100000, 
		0x0c7fffff, 
		0xff7ffffe, 
		0x0f7ffffd, 
		0xff800000, 
		0xff800000, 
		0xff800000, 
		0xffbfffff, 
		0x80000000, 
		0x0e00001f, 
		0x0e000fff, 
		0xc2400000, 
		0x821fd28b, 
		0xc24c0000, 
		0x42080000, 
		0x80000000, 
		0x00000000, 
		0xaaaaaaaa, 
		0x007ffffe, 
		0xffffffff, 
		0x30d0d049, 
		0x00000002, 
		0x41f00000, 
		0xc27c0000, 
		0x0e00000f, 
		0x80000000, 
		0x3f800001, 
		0x80800000, 
		0x00008000, 
		0x8ad70825, 
		0x0c7fe000, 
		0x7fb31b0d, 
		0x41700000, 
		0xc21c0000, 
		0x80008000, 
		0x3c1513ca, 
		0x423c0000, 
		0xc2600000, 
		0xa5544f1e, 
		0xe5a202bd, 
		0xda34e6f4, 
		0x80002000, 
		0x48d6b899, 
		0x00000100, 
		0xb1bb2e09, 
		0x00000000, 
		0x00004000, 
		0x80000000, 
		0x373d7e17, 
		0x00000002, 
		0x7fc00000, 
		0x7fbfffff, 
		0x0e000fff, 
		0x42540000, 
		0x80ffffff, 
		0x40000000, 
		0xfe064ba2, 
		0x00008000, 
		0xc1300000, 
		0xc21c0000, 
		0x7f800000, 
		0x7fbce150, 
		0x0e1fffff, 
		0x80000000, 
		0xc1400000, 
		0x7f7fffff, 
		0xff7ffffe, 
		0xff800000, 
		0x80000000, 
		0x53ad4be5, 
		0xc2700000, 
		0xc2280000, 
		0x00203200, 
		0xb94a3a56, 
		0x0c7f8000, 
		0x42180000, 
		0x0d000ff0, 
		0x41300000, 
		0x47cc947f, 
		0x0e000fff, 
		0x41f00000, 
		0xe717c969, 
		0x6fb6328b, 
		0xcb8c4b40, 
		0xffffffff, 
		0x42780000, 
		0x8e28fe5f, 
		0x6c8fb65c, 
		0xf0f1700a, 
		0x80000040, 
		0x41880000, 
		0x8f7ffffd, 
		0x007fffff, 
		0x9d2118ff, 
		0x55555555, 
		0xc2280000, 
		0x00000000, 
		0xb98f44c4, 
		0x0e00007f, 
		0x7fe1fa45, 
		0x0c7fc000, 
		0xff7fffff, 
		0x80000000, 
		0x0e0003ff, 
		0x0c700000, 
		0xae264c6a, 
		0x41000000, 
		0x80000004, 
		0x0e01ffff, 
		0x79b06f86, 
		0x00800002, 
		0x8574bcd5, 
		0xc2640000, 
		0x00000000, 
		0xc2280000, 
		0xab6c06bd, 
		0xc4bdadad, 
		0x9b345e29, 
		0x0d00fff0, 
		0x0e03ffff, 
		0x0e001fff, 
		0x7f800000, 
		0x0e0fffff, 
		0xc2700000, 
		0x0e1fffff, 
		0x0c7ffffe, 
		0x00000400, 
		0x00080000, 
		0x40800000, 
		0x7df91598, 
		0x46cf66c9, 
		0x00020000, 
		0x80000000, 
		0xc2080000, 
		0x804dfa6e, 
		0x5bad0a45, 
		0x0c7ffffe, 
		0x420c0000, 
		0x42280000, 
		0x00000000, 
		0xb2456e6f, 
		0xcf20045f, 
		0x33333333, 
		0x40400000, 
		0x41200000, 
		0x177863b2, 
		0x0e000007, 
		0x3acf26fd, 
		0x0c7f0000, 
		0x42040000, 
		0x42400000, 
		0x424c0000, 
		0xc0400000, 
		0xff800000, 
		0x0c7ffe00, 
		0x7fc00001, 
		0x61ddd61d, 
		0x6af5c5ae, 
		0x80000080, 
		0x00010000, 
		0xa5aa1cb4, 
		0x41a80000, 
		0x7fe205d0, 
		0x41400000, 
		0x80000008, 
		0x805cfc3c, 
		0x2109116d, 
		0x425c0000, 
		0xff800000, 
		0x80000000, 
		0xc2440000, 
		0x46876191, 
		0xbf028f5c, 
		0x40400000, 
		0xff800000, 
		0x0c7c0000, 
		0x0e022a79, 
		0x4b8c4b40, 
		0x0e0007ff, 
		0xc1a80000, 
		0xbf75d135, 
		0xc145dc70, 
		0x00400000, 
		0x80000000, 
		0x4b8c4b40, 
		0x00100000, 
		0xd69ba319, 
		0xe6522220, 
		0x41700000, 
		0xc2100000, 
		0x0c7ff000, 
		0x80000004, 
		0x0c7fffc0, 
		0x0c7e0000, 
		0x61ede497, 
		0xede43af8, 
		0x00000100, 
		0x41300000, 
		0x80000000, 
		0x80000000, 
		0x0c7e0000, 
		0x2137fc0e, 
		0x42340000, 
		0x0e00000f, 
		0x80000000, 
		0x00002000, 
		0x425c0000, 
		0xc2140000, 
		0x80040000, 
		0xc2700000, 
		0x0c7fff00, 
		0x42500000, 
		0x7b603ad5, 
		0xc1100000, 
		0xaf27020c, 
		0xb76ea19c, 
		0xc2180000, 
		0x00000000, 
		0xfa152998, 
		0x80000000, 
		0x00000000, 
		0xc1300000, 
		0x7fc00000, 
		0x92196c3d, 
		0x00000400, 
		0x80000002, 
		0xc23c0000, 
		0xf32f0681, 
		0x41500000, 
		0x80000000, 
		0xf64f59f6, 
		0x41f80000, 
		0x74a1c2c2, 
		0x41300000, 
		0x99dd2547, 
		0x80000100, 
		0x00000004, 
		0x80800001, 
		0x80000010, 
		0xd40b8a4b, 
		0xff820cbc, 
		0x80002000, 
		0x00800001, 
		0x80800003, 
		0x7f800000, 
		0xffbc6d34, 
		0x0c7fffe0, 
		0x807abf0f, 
		0x425c0000, 
		0xff800001, 
		0x80000000, 
		0x00000010, 
		0x80000001, 
		0x2dfd979c, 
		0x00800002, 
		0x00010000, 
		0x0e0007ff, 
		0x80001000, 
		0xc1b80000, 
		0xc2180000, 
		0x0c7f8000, 
		0x00000000, 
		0x3b02850f, 
		0x41500000, 
		0x00002000, 
		0x27e03cb1, 
		0x2dba5293, 
		0x0c8b208e, 
		0xbf028f5c, 
		0x0387dcef, 
		0x55c357b4, 
		0x42040000, 
		0x00000000, 
		0x8f7fffff, 
		0x0e0007ff, 
		0xec09d982, 
		0x5a734cd7, 
		0x41a00000, 
		0x41c80000, 
		0x80000000, 
		0x383a4f6f, 
		0x0e0000ff, 
		0x7f80dee1, 
		0x80000001, 
		0x7f800000, 
		0xc1c00000, 
		0x80002000, 
		0x007fffff, 
		0x0e00001f, 
		0x930e3f1d, 
		0x0c7ffffc, 
		0x7e44c6a3, 
		0x80000080, 
		0x00000001, 
		0x41a00000, 
		0x8f7fffff, 
		0x80800003, 
		0x0e0007ff, 
		0x804aa96f, 
		0xc1b80000, 
		0x00000004, 
		0x00000000, 
		0x80000010, 
		0x41400000, 
		0x81ae28fe, 
		0x000f9599, 
		0x41900000, 
		0x0c7ffff8, 
		0xff000000, 
		0x41e00000, 
		0xff7ffffe, 
		0x425c0000, 
		0xc39615dd, 
		0x41980000, 
		0x41f00000, 
		0x00743799, 
		0xd67f13b4, 
		0x7f800000, 
		0x0c7ffff0, 
		0x41c80000, 
		0xf499df85, 
		0x0f7ffffe, 
		0xc2540000, 
		0x78064e68, 
		0x239269a5, 
		0x24ec5503, 
		0xc2040000, 
		0x17f86261, 
		0xc2780000, 
		0x2b814f21, 
		0xc1f00000, 
		0x0e0001ff, 
		0xd87eeaf7, 
		0x9c79569d, 
		0x7f800000, 
		0xc2280000, 
		0xb5a611fc, 
		0x48fb5f7c, 
		0x41880000, 
		0x0c7f8000, 
		0x80100000, 
		0x992624f4, 
		0x0e0003ff, 
		0x0e0000ff, 
		0x8216a5bf, 
		0x0f9e1cb5, 
		0x504fc4e0, 
		0x81435781, 
		0x80000000, 
		0xff7fffff, 
		0xd60ce57b, 
		0x80002000, 
		0x1c475acd, 
		0x41c80000, 
		0x80008000, 
		0x00001000, 
		0x444ee176, 
		0x19f4d7fb, 
		0xc2280000, 
		0x00000000, 
		0xc1800000, 
		0x00000004, 
		0x00800002, 
		0x79801980, 
		0x0c7fe000, 
		0x41d80000, 
		0x41f00000, 
		0xdacaa205, 
		0xc25c0000, 
		0x0e7fffff, 
		0x00400000, 
		0x84663f12, 
		0x00400000, 
		0x42200000, 
		0xff800000, 
		0x80000002, 
		0x00000001, 
		0x807ffffe, 
		0x15cffcc7, 
		0x80000000, 
		0x41f00000, 
		0x52ef37e0, 
		0x7f800000, 
		0x15eee5cb, 
		0x80000000, 
		0x427c0000, 
		0x46b40701, 
		0x00000020, 
		0x80ffffff, 
		0xffffffff, 
		0x50dd5198, 
		0x7f7fffff, 
		0x42180000, 
		0x00800000, 
		0xff7ffffe, 
		0xc70f2a2d, 
		0x0e3fffff, 
		0x6e0b7768, 
		0x5b9f63af, 
		0x0e07ffff, 
		0x30f1602b, 
		0xc1f00000, 
		0x80000000, 
		0xc2740000, 
		0xc1200000, 
		0x7f800000, 
		0x59606ca9, 
		0x00080000, 
		0xa9a84397, 
		0xaa8ea749, 
		0x57b2468b, 
		0x80800001, 
		0x0c400000, 
		0x1d5227a5, 
		0x80800003, 
		0xffa048f2, 
		0x00000000, 
		0x80002000, 
		0x00000000, 
		0x0e1fffff, 
		0x00010000, 
		0x80011111, 
		0xaaaaaaaa, 
		0x36c281be, 
		0xc1a00000, 
		0x4fa07cdf, 
		0xc2740000, 
		0x00000000, 
		0x9e2390c5, 
		0x53799e36, 
		0x80020000, 
		0x066eb2b4, 
		0x80002000, 
		0x42600000, 
		0x41700000, 
		0x0c7f0000, 
		0xc1500000, 
		0xbe2bfd38, 
		0x80000000, 
		0xee4a17e5, 
		0xffc00001, 
		0xc1c80000, 
		0x0c7ffe00, 
		0x0d000ff0, 
		0x40400000, 
		0xa5c510cd, 
		0xb9f75978, 
		0xff800000, 
		0x80011111, 
		0x3878d608, 
		0x80000000, 
		0xf5875034, 
		0x002daecb, 
		0x7f800000, 
		0x00020000, 
		0xffc8dd36, 
		0x80000000, 
		0x41800000, 
		0x7f7ffffe, 
		0xcfd7706d, 
		0x41d00000, 
		0x80000080, 
		0xc1b80000, 
		0x1d75b5db, 
		0x00000020, 
		0x60db17ac, 
		0x00000000, 
		0xc2780000, 
		0x40c00000, 
		0xcfd22ddd, 
		0x0c7fffe0, 
		0x42080000, 
		0x00000200, 
		0x80800002, 
		0x0e07ffff, 
		0x00000008, 
		0x80800003, 
		0x7f800000, 
		0x7f800000, 
		0x7f884bf4, 
		0x4356f892, 
		0xd9b08900, 
		0x426c0000, 
		0x42200000, 
		0x7d17e594, 
		0xa6b27c92, 
		0x00239bf5, 
		0xaaaaaaaa, 
		0x4b8c4b40, 
		0x527130ea, 
		0x80800003, 
		0x00020000, 
		0x69ae54b9, 
		0xc1600000, 
		0x41b80000, 
		0x42240000, 
		0x19200956, 
		0x80000000, 
		0x00002000, 
		0x00004000, 
		0xbf800000, 
		0x00100000, 
		0xc27c0000, 
		0x00104f67, 
		0x2f19742c, 
		0x7fa4b7f2, 
		0x00000008, 
		0xc2280000, 
		0x42200000, 
		0x41200000, 
		0x00000000, 
		0x7f800000, 
		0x42700000, 
		0x00000040, 
		0xff800000, 
		0x0c7e0000, 
		0x0c7fffc0, 
		0xa4247131, 
		0xc2400000, 
		0x80800000, 
		0xc1900000, 
		0x40c00000, 
		0x00000008, 
		0xffa9d4ab, 
		0x95466660, 
		0xb19e7de7, 
		0xcb000000, 
		0xff000000, 
		0xcccccccc, 
		0x80800003, 
		0xfde55b68, 
		0x41a80000, 
		0x0c780000, 
		0xc2380000, 
		0x00000000, 
		0x7f800000, 
		0x00800000, 
		0xff7ffffe, 
		0x7cf07d81, 
		0x002f0411, 
		0xffda9c43, 
		0xc1d00000, 
		0x00000000, 
		0x0e0001ff, 
		0xffbfcee1, 
		0x0c7ffe00, 
		0x80000000, 
		0x268a22a6, 
		0x80004000, 
		0xc2440000, 
		0xc0400000, 
		0x00000000, 
		0xb32641d0, 
		0x7f800000, 
		0x2e512c83, 
		0x32cebbcb, 
		0x80020000, 
		0x10d60b59, 
		0xc2500000, 
		0x3f800000, 
		0xffc00001, 
		0xca1503d5, 
		0xc2680000, 
		0x41880000, 
		0x80800000, 
		0x7f800000, 
		0x41c00000, 
		0xcb8c4b40, 
		0x0c7f0000, 
		0x050dbae2, 
		0x80001000, 
		0xc2240000, 
		0xff800000, 
		0x0c400000, 
		0xc1b00000, 
		0x80000000, 
		0x80800001, 
		0x230d6e75, 
		0x7f800001, 
		0xe486983f, 
		0x80000000, 
		0xb30cfe5c, 
		0x40000000, 
		0xc1e80000, 
		0xc25c0000, 
		0x0c7fffe0, 
		0x4aa40bd9, 
		0x42580000, 
		0x0d00fff0, 
		0xffffffff, 
		0x00000080, 
		0xd2b425ed, 
		0x3f028f5c, 
		0x1d8bb43f, 
		0xad0cd350, 
		0xc20c0000, 
		0xc20c0000, 
		0x10d7522c, 
		0x1ab01aa4, 
		0x425c0000, 
		0x00000800, 
		0x80000001, 
		0x3cc5adab, 
		0x00004000, 
		0x41880000, 
		0x596c94cc, 
		0x00800000, 
		0x80011111, 
		0x80800000, 
		0xc1d80000, 
		0x80000001, 
		0xf56af326, 
		0xf03f9397, 
		0x6dc03b0c, 
		0x8659e6f7, 
		0x5b7584d1, 
		0xed0c2d66, 
		0x00100000, 
		0x7fb3afa2, 
		0x42780000, 
		0x00400000, 
		0x0e00001f, 
		0x00000000, 
		0xc1200000, 
		0xc1400000, 
		0xc2400000, 
		0x0e00003f, 
		0x00000000, 
		0xc79e2099, 
		0xc2140000, 
		0xc1400000, 
		0x41e80000, 
		0x807fffff, 
		0x00ffffff, 
		0x0e000003, 
		0x42d432c0, 
		0xbfc3d97a, 
		0x589b6547, 
		0xc1a00000, 
		0x42500000, 
		0x00000000, 
		0xc0e00000, 
		0x00400000, 
		0x00000000, 
		0x3f800000, 
		0x23ce61c8, 
		0xc2180000, 
		0xffffffff, 
		0xc27c0000, 
		0xf95bc236, 
		0x420c0000, 
		0x007ffffe, 
		0x7f800001, 
		0xc1880000, 
		0x0f7ffffc, 
		0x0c7ffff8, 
		0xc1300000, 
		0x41d00000, 
		0x42680000, 
		0x00004000, 
		0x8f7ffffd, 
		0xcccccccc, 
		0xffffffff, 
		0x437b42a1, 
		0x0e007fff, 
		0x80000000, 
		0xaaaaaaaa, 
		0x80800000, 
		0x80800003, 
		0x00400000, 
		0x76d89ba4, 
		0x80800001, 
		0x41900000, 
		0xe44f679b, 
		0xc0000000, 
		0x0c7c0000, 
		0x760d4794, 
		0xc1900000, 
		0x807ffffe, 
		0x80008000, 
		0x80080000, 
		0x41600000, 
		0x0e0001ff, 
		0x248d56fb, 
		0x1ee5c239, 
		0x41600000, 
		0x80008000, 
		0xee19d79a, 
		0x7f000000, 
		0x42540000, 
		0x00000000, 
		0x635d1af7, 
		0x80000000, 
		0xbdfe367f, 
		0xc27c0000, 
		0xb0e61c2d, 
		0xff800000, 
		0xff800000, 
		0xc23c0000, 
		0x4b8c4b40, 
		0xb4350f88, 
		0x42640000, 
		0xc1700000, 
		0x0c7fc000, 
		0x0f7ffffd, 
		0x80020000, 
		0xff7ffffe, 
		0xfc8478af, 
		0xc2180000, 
		0xc1880000, 
		0x80000001, 
		0x00800002, 
		0x804d9d45, 
		0x0e00ffff, 
		0x0c7fffe0, 
		0xc2440000, 
		0xc2700000, 
		0x00800000, 
		0x00000040, 
		0xed5a0677, 
		0x00800001, 
		0x07709b8a, 
		0xc26c0000, 
		0xc0bd60e5, 
		0x0f7ffffe, 
		0xff800001, 
		0x80800003, 
		0xb7ade5ec, 
		0x80000400, 
		0x0d000ff0, 
		0xc1b00000, 
		0xff800000, 
		0x3d3f0dce, 
		0x80080000, 
		0xbf028f5c, 
		0x803a4feb, 
		0x8f7ffffd, 
		0x50dbb061, 
		0xc20c0000, 
		0x1c9d02ce, 
		0x40e00000, 
		0x7f800000, 
		0x41d80000, 
		0xea42dcf6, 
		0xc2200000, 
		0xc1800000, 
		0x7f800000, 
		0xc27c0000, 
		0x40a00000, 
		0xebf471a7, 
		0x0c7c0000, 
		0x42580000, 
		0x7f7ffffe, 
		0x80011111, 
		0x42600000, 
		0x80000080, 
		0xc2200000, 
		0x80000001, 
		0xff7fffff, 
		0xfc48b410, 
		0x00008000, 
		0xc1700000, 
		0x00000400, 
		0x426c0000, 
		0xc1c00000, 
		0xc2600000, 
		0xc25c0000, 
		0xd62e4790, 
		0x7f800000, 
		0x00000002, 
		0x0e0000ff, 
		0xff85b160, 
		0x7f7ffffe, 
		0x80000002, 
		0x0c7fffe0, 
		0x00000004, 
		0xc1200000, 
		0xff7ffffe, 
		0x80000100, 
		0xc1880000, 
		0x80800002, 
		0x41e80000, 
		0x41500000, 
		0x7f800000, 
		0xc1e80000, 
		0x1653799a, 
		0x80734e3b, 
		0x0c7f8000, 
		0xb3b3a40e, 
		0x41400000, 
		0x0c600000, 
		0x7f800000, 
		0x80000000, 
		0x80011111, 
		0xe0167a9a, 
		0xc2180000, 
		0x00000000, 
		0x80000000, 
		0x0c7ffffe, 
		0x40c16b61, 
		0x00004000, 
		0x87232fb2, 
		0x42400000, 
		0x0e000001, 
		0x7f800000, 
		0xc1c80000, 
		0x41980000, 
		0x00000008, 
		0xc1a00000, 
		0x80000001, 
		0x80000000, 
		0x00000000, 
		0x0c7ffffe, 
		0x8e7e57c5, 
		0x7f800000, 
		0x4b000000, 
		0x0c7ffffc, 
		0x61cd1ba4, 
		0xffbfffff, 
		0xc0e00000, 
		0x3f028f5c, 
		0x00001000, 
		0x80000000, 
		0x41e00000, 
		0x00020000, 
		0x00000100, 
		0x425c0000, 
		0xc0c00000, 
		0x80800002, 
		0x80000100, 
		0x41b00000, 
		0x80000002, 
		0x41500000, 
		0x807ffffe, 
		0xcc651d8f, 
		0xc4877f59, 
		0x80000000, 
		0x00000000, 
		0x007ffffe, 
		0x4d20d95f, 
		0x80800000, 
		0x00000080, 
		0x39fa1e06, 
		0x41000000, 
		0x80001000, 
		0x42700000, 
		0x0c7c0000, 
		0x0d00fff0, 
		0xcb000000, 
		0xd56a4ceb, 
		0x42040000, 
		0xff7fffff, 
		0xffc00001, 
		0x7f800000, 
		0x396b154e, 
		0xc2680000, 
		0x41900000, 
		0xc1d80000, 
		0x80ffffff, 
		0x0c7fffe0, 
		0x41200000, 
		0x80000002, 
		0x00800001, 
		0x8982eb60, 
		0x03e7e78b, 
		0xc8fd1bdf, 
		0xffb20df4, 
		0x8f7ffffe, 
		0x0e00ffff, 
		0x556531a1, 
		0xc2500000, 
		0x427c0000, 
		0x91fd7963, 
		0x8b84354d, 
		0x0d00fff0, 
		0x41800000, 
		0xc2240000, 
		0x40c00000, 
		0x80800003, 
		0x3f028f5c, 
		0xc2340000, 
		0x42000000, 
		0xaaaaaaaa, 
		0x42700000, 
		0xbf028f5c, 
		0x7f800000, 
		0x8049fab7, 
		0x00000040, 
		0x0f7fffff, 
		0xffc20bd7, 
		0x00800003, 
		0x7bc130ff, 
		0xc0400000, 
		0x00800002, 
		0x41100000, 
		0x00000002, 
		0x00002000, 
		0x80800001, 
		0xc1d80000, 
		0xcb000000, 
		0x3f05d90b, 
		0x16181169, 
		0x7fc00001, 
		0x00800003, 
		0x80000000, 
		0xc1f00000, 
		0x80000000, 
		0x7f7ffffe, 
		0x3705938d, 
		0xff800000, 
		0x6634117f, 
		0xff8a6f47, 
		0x00000008, 
		0xcb8c4b40, 
		0x422c0000, 
		0x807fffff, 
		0x5e860560, 
		0x80000001, 
		0x42780000, 
		0xff7ffffe, 
		0xbf800001, 
		0xc2680000, 
		0xc2540000, 
		0xc27c0000, 
		0xc1a80000, 
		0x80000000, 
		0x7f800001, 
		0xff418cad, 
		0xc1f80000, 
		0x80000010, 
		0x80000000, 
		0xc1f80000, 
		0xff800000, 
		0x00000800, 
		0xee4fc076, 
		0x00000008, 
		0x0f7ffffd, 
		0x00000008, 
		0xcce9f33b, 
		0x00400000, 
		0x474e8f47, 
		0xc2080000, 
		0xbff3d10d, 
		0x0c7fffc0, 
		0xc2740000, 
		0x8ac36a85, 
		0x469a26f6, 
		0xc1600000, 
		0xa39ce73f, 
		0x9d8183c5, 
		0x80011111, 
		0x00800000, 
		0x002f4f0e, 
		0x00800001, 
		0x80010000, 
		0xc2400000, 
		0x00000400, 
		0x0e1fffff, 
		0xffffffff, 
		0x00000002, 
		0x42600000, 
		0x0e00001f, 
		0x3aa1c744, 
		0xc4296cd3, 
		0x9a878877, 
		0x80080000, 
		0x68d2f325, 
		0xc1f00000, 
		0x7f800000, 
		0x08805809, 
		0xff800000, 
		0x42300000, 
		0x00000000, 
		0x40a00000, 
		0xc1500000, 
		0x41400000, 
		0x80020000, 
		0x52999ee0, 
		0xff800000, 
		0xc2280000, 
		0x00000000, 
		0x8cda0aaf, 
		0x80000000, 
		0x00011111, 
		0x00000000, 
		0x80000000, 
		0x007fffff, 
		0xe041a118, 
		0x80000000, 
		0x26868d1a, 
		0xff7ffffe, 
		0xff7ffffe, 
		0x81e8eb25, 
		0x7f800000, 
		0x6ebd104a, 
		0x00400000, 
		0x41300000, 
		0x2c25ba85, 
		0x0c7ff000, 
		0xff800000, 
		0xc1f80000, 
		0xc88dbc72, 
		0x7fdc2eef, 
		0x18413b76, 
		0xc2740000, 
		0x42140000, 
		0x4dcaab63, 
		0x80010000, 
		0x424c0000, 
		0xc2680000, 
		0x00080000, 
		0x00800001, 
		0x0c600000, 
		0x42577485, 
		0xae68884a, 
		0x41980000, 
		0x53dafdd0, 
		0x75e172da, 
		0xc2180000, 
		0xffac8ae6, 
		0x0c7fff80, 
		0x4b8c4b40, 
		0xef1ea328, 
		0x716435a6, 
		0x41500000, 
		0x7bb6168d, 
		0x0c780000, 
		0x41980000, 
		0x0c7fff80, 
		0x41800000, 
		0xbf800001, 
		0x6e3e2b6d, 
		0xc1100000, 
		0x4b8c4b40, 
		0x69306641, 
		0xc2440000, 
		0x7f800000, 
		0xc1600000, 
		0x7f800000, 
		0x7fbfffff, 
		0x003b1b6f, 
		0x8b297ee9, 
		0xbf028f5c, 
		0x0e00003f, 
		0x10db7872, 
		0x80000000, 
		0x80000001, 
		0x7f800000, 
		0xccd124a0, 
		0xc20c0000, 
		0x007fffff, 
		0x42240000, 
		0xffa1a55d, 
		0xa838dd28, 
		0x0e0000ff, 
		0xdcbf4d21, 
		0xc2300000, 
		0x423c0000, 
		0x3d5842db, 
		0x4160a7fb, 
		0x00400000, 
		0x40e00000, 
		0x80000000, 
		0x7f800000, 
		0x41d00000, 
		0x00000000, 
		0x5b6c74bd, 
		0xcb000000, 
		0x427cd7b7, 
		0x426c0000, 
		0x653c6c99, 
		0x00011111, 
		0x80470986, 
		0x0e000001, 
		0x80000000, 
		0x49035c28, 
		0x40800000, 
		0xff800000, 
		0x33333333, 
		0xff800000, 
		0xc31bdac7, 
		0xeb74ee1d, 
		0x00000000, 
		0xc0a00000, 
		0x423c0000, 
		0x84b48335, 
		0xc27c0000, 
		0xf83b18ac, 
		0x9599f87b, 
		0x62cbb446, 
		0x4dc37218, 
		0xc2180000, 
		0x0e003fff, 
		0x80000080, 
		0xc634d40b, 
		0x0f7fffff, 
		0x4b000000, 
		0x0e0fffff, 
		0xc2700000, 
		0x42540000, 
		0xfe8ddd11, 
		0x80000020, 
		0x80800000, 
		0x7fbfffff, 
		0x80040000, 
		0xc2340000, 
		0x80008000, 
		0x550de3de, 
		0x41b80000, 
		0x42340000, 
		0x00000008, 
		0x806570b8, 
		0xf0f59e4b, 
		0x0c7e0000, 
		0xa107a210, 
		0x80010000, 
		0xba6f4010, 
		0x42300000, 
		0xaae5f5c8, 
		0x41300000, 
		0x00000400, 
		0x80001000, 
		0x00800001, 
		0x0e000001, 
		0x0e003fff, 
		0x0e000003, 
		0x900024de, 
		0x055d6024, 
		0x0c7ff000, 
		0x7f7ffffe, 
		0x007ffffe, 
		0x80080000, 
		0x80000200, 
		0x0fbc85fa  
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
		0x000002d5, 
		0x000001b3, 
		0x00000221, 
		0x000001f5, 
		0x0000002f, 
		0x00000031, 
		0x00000105, 
		0x00000319, 
		0x0000036b, 
		0x000001a5, 
		0x000001ab, 
		0x00000233, 
		0x00000177, 
		0x00000389, 
		0x000001f1, 
		0x000003c5, 
		0x000001e7, 
		0x00000011, 
		0x00000181, 
		0x000002e1, 
		0x00000037, 
		0x0000037b, 
		0x000003b9, 
		0x00000353, 
		0x000001e7, 
		0x000001a9, 
		0x00000027, 
		0x00000059, 
		0x00000259, 
		0x0000016b, 
		0x00000165, 
		0x000003a7, 
		0x00000315, 
		0x00000113, 
		0x00000003, 
		0x00000049, 
		0x000002eb, 
		0x00000241, 
		0x0000038f, 
		0x00000165, 
		0x00000103, 
		0x000003bf, 
		0x0000005d, 
		0x00000069, 
		0x00000053, 
		0x00000235, 
		0x00000231, 
		0x000000f7, 
		0x00000039, 
		0x000002f1, 
		0x000002ad, 
		0x0000017f, 
		0x0000004b, 
		0x000002b5, 
		0x000000ed, 
		0x0000023b, 
		0x0000038f, 
		0x0000007f, 
		0x00000085, 
		0x0000001b, 
		0x000000fd, 
		0x00000063, 
		0x000002db, 
		0x000002a9, 
		0x00000395, 
		0x000002e9, 
		0x00000395, 
		0x00000081, 
		0x000002d7, 
		0x0000028d, 
		0x000002fb, 
		0x0000029f, 
		0x00000123, 
		0x00000011, 
		0x0000023b, 
		0x00000379, 
		0x000002b3, 
		0x00000347, 
		0x000001c7, 
		0x00000045, 
		0x000002a1, 
		0x000003c3, 
		0x000000f1, 
		0x000002d9, 
		0x0000000f, 
		0x0000009f, 
		0x00000067, 
		0x0000036d, 
		0x000003e9, 
		0x000002af, 
		0x0000005b, 
		0x0000004f, 
		0x000003df, 
		0x0000023d, 
		0x000002cb, 
		0x000002a5, 
		0x0000018b, 
		0x00000007, 
		0x000002e7, 
		0x00000047, 
		0x00000177, 
		0x000003e7, 
		0x00000283, 
		0x0000026f, 
		0x00000017, 
		0x0000027b, 
		0x000002d1, 
		0x0000032d, 
		0x000001ad, 
		0x000002cd, 
		0x000001f5, 
		0x0000038d, 
		0x000001b9, 
		0x00000003, 
		0x00000109, 
		0x000000d7, 
		0x000002bd, 
		0x000000eb, 
		0x000003af, 
		0x0000033f, 
		0x000000fb, 
		0x000002b7, 
		0x00000343, 
		0x00000395, 
		0x0000010d, 
		0x000002b7, 
		0x00000239, 
		0x000000a7, 
		0x000002ed, 
		0x00000073, 
		0x0000037d, 
		0x000000bb, 
		0x00000203, 
		0x0000016d, 
		0x000000f5, 
		0x00000201, 
		0x000000d5, 
		0x0000017b, 
		0x000003b9, 
		0x0000027f, 
		0x0000020f, 
		0x0000007b, 
		0x00000015, 
		0x000002f3, 
		0x000002ad, 
		0x000003b1, 
		0x0000039b, 
		0x000000db, 
		0x00000217, 
		0x00000001, 
		0x00000199, 
		0x000000a5, 
		0x0000004d, 
		0x000001b9, 
		0x0000027b, 
		0x00000089, 
		0x00000285, 
		0x00000323, 
		0x0000001b, 
		0x000003dd, 
		0x000001a1, 
		0x0000005b, 
		0x00000321, 
		0x00000009, 
		0x0000029b, 
		0x000002ff, 
		0x0000007b, 
		0x0000016d, 
		0x0000015f, 
		0x00000151, 
		0x00000043, 
		0x0000009b, 
		0x0000028b, 
		0x00000377, 
		0x0000004f, 
		0x000003d5, 
		0x00000275, 
		0x000000d7, 
		0x000000d9, 
		0x000001e7, 
		0x000001ab, 
		0x00000205, 
		0x0000008f, 
		0x0000010d, 
		0x00000301, 
		0x0000027f, 
		0x00000019, 
		0x000000e7, 
		0x00000265, 
		0x00000067, 
		0x00000343, 
		0x000003c9, 
		0x0000029b, 
		0x00000021, 
		0x000002f9, 
		0x000003f3, 
		0x000001ab, 
		0x000002f3, 
		0x00000087, 
		0x000000eb, 
		0x000002cd, 
		0x000001e9, 
		0x0000026b, 
		0x000001b7, 
		0x00000173, 
		0x0000025b, 
		0x0000035b, 
		0x000001f9, 
		0x000000b9, 
		0x0000030b, 
		0x0000035d, 
		0x00000151, 
		0x000001ff, 
		0x00000163, 
		0x000000f9, 
		0x00000291, 
		0x0000010f, 
		0x0000024f, 
		0x000002a3, 
		0x000001ad, 
		0x000003f3, 
		0x000002f5, 
		0x000000f9, 
		0x0000019d, 
		0x0000001b, 
		0x00000377, 
		0x00000293, 
		0x00000257, 
		0x000003f5, 
		0x00000371, 
		0x000002c9, 
		0x000002c3, 
		0x0000012f, 
		0x000001ad, 
		0x000002c1, 
		0x0000035d, 
		0x00000073, 
		0x000003c7, 
		0x00000239, 
		0x000001c7, 
		0x000000d3, 
		0x000002f1, 
		0x0000013d, 
		0x00000101, 
		0x00000343, 
		0x000001df, 
		0x00000065, 
		0x000003af, 
		0x000003eb, 
		0x000003f1, 
		0x000000d5, 
		0x0000030b, 
		0x0000017f, 
		0x000003bb, 
		0x0000038b, 
		0x0000010f  
};

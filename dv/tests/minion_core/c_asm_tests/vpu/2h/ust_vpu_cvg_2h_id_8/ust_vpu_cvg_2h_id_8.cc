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
			"csrwi mhpmevent4, 15\n"   
			"csrwi mhpmevent5, 26\n"   
			"csrwi mhpmevent6, 19\n"   
			"csrwi mhpmevent7, 7\n"   
			"csrwi mhpmevent8, 5\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f25,  64 (x5)\n"
			"flw.ps f27,  160 (x5)\n"
			"flw.ps f12,  0 (x5)\n"
			"flw.ps f26,  992 (x5)\n"
			"flw.ps f5,  704 (x5)\n"
			"flw.ps f8,  96 (x5)\n"
			"flw.ps f29,  384 (x5)\n"
			"flw.ps f18,  576 (x5)\n"
			"flw.ps f20,  224 (x5)\n"
			"flw.ps f22,  192 (x5)\n"
			"flw.ps f17,  480 (x5)\n"
			"flw.ps f6,  864 (x5)\n"
			"flw.ps f28,  832 (x5)\n"
			"flw.ps f23,  256 (x5)\n"
			"flw.ps f16,  416 (x5)\n"
			"flw.ps f14,  640 (x5)\n"
			"flw.ps f11,  288 (x5)\n"
			"flw.ps f10,  736 (x5)\n"
			"flw.ps f4,  448 (x5)\n"
			"flw.ps f1,  928 (x5)\n"
			"flw.ps f21,  128 (x5)\n"
			"flw.ps f15,  960 (x5)\n"
			"flw.ps f13,  320 (x5)\n"
			"flw.ps f2,  672 (x5)\n"
			"flw.ps f3,  32 (x5)\n"
			"flw.ps f30,  800 (x5)\n"
			"flw.ps f31,  608 (x5)\n"
			"flw.ps f7,  512 (x5)\n"
			"flw.ps f9,  352 (x5)\n"
			"flw.ps f0,  544 (x5)\n"
			"flw.ps f24,  768 (x5)\n"
			"flw.ps f19,  896 (x5)\n"
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
			"li x0, 0x384\n" 
			"slti x0, x0, 0x7f2\n"
			"la x31, check_sum_hid_0\n"
			"li x5, 0x1\n" 
			"sw x5, (x31)\n"
			"li x30, 0\n"
			"addi x30, x30, 1\n"
			"la x27, rand_int_32\n"
			"flw.ps f13, 608(x27)\n"
			"flw.ps f10, 992(x27)\n"
			"fsub.pi f20, f13, f10\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_0:\n"
			"fsetm.pi m2, f4\n" 
			"li x5, 0x803400C8\n"
			"ld x5, 0(x5)\n"
			"la x10, rand_ieee754_32\n"
			"flw.ps f15, 416(x10)\n"
			"flw.ps f9, 800(x10)\n"
			"fsgnjn.ps f19, f15, f9\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f19, 416(x12)\n"
			"flw.ps f12, 960(x12)\n"
			"fle.ps f5, f19, f12\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f2, 608(x28)\n"
			"flw.ps f4, 832(x28)\n"
			"fsub.s f16, f2, f4, rne\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x28, rand_ieee754_32\n"
			"flw.ps f3, 288(x28)\n"
			"flw.ps f24, 256(x28)\n"
			"fsgnjx.ps f5, f3, f24\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f18, 896(x20)\n"
			"flw.ps f10, 992(x20)\n"
			"flw.ps f26, 928(x20)\n"
			"fcmov.ps f4, f18, f10, f26\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f18, 928(x12)\n"
			"frsq.ps f12, f18\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f6, 544(x7)\n"
			"fcvt.w.s x27, f6, rup\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f20, 128(x6)\n"
			"flw.ps f10, 832(x6)\n"
			"flw.ps f0, 832(x6)\n"
			"fmsub.ps f30, f20, f10, f0, rdn\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_0:\n"
			"maskor m3, m6, m1\n" 
			"li x5, 0x803400C0\n"
			"ld x5, 0(x5)\n"
			"la x13, rand_int_32\n"
			"flw.ps f24, 288(x13)\n"
			"fandi.pi f26, f24, 0x1b9\n" 
			"la x10, rand_int_32\n"
			"flw.ps f30, 320(x10)\n"
			"faddi.pi f21, f30, 0x1bc\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f10, 384(x22)\n"
			"flw.ps f14, 32(x22)\n"
			"feq.s x22, f10, f14\n" 
			"la x6, rand_int_32\n"
			"flw.ps f10, 64(x6)\n"
			"fpackrepb.pi f28, f10\n" 
			"li x5, 0xb37a\n"
			"csrw tensor_mask, x5\n"	
			"la x15, rand_ieee754_32\n"
			"flw.ps f9, 544(x15)\n"
			"flw.ps f15, 960(x15)\n"
			"fsgnjn.ps f28, f9, f15\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ITER_0_HID_0:\n"
			"li x5, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x1 << TENSOR_FMA32_B_NUM_COLS) | (0xb << TENSOR_FMA32_A_NUM_ROWS) | (0x0 << TENSOR_FMA32_A_NUM_COLS) | (0x7 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0xaa << TENSOR_FMA32_SCP_LOC_B) | (0x6b << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x0 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x6, ((0x3 << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0xb << TENSOR_QUANT_QUANT_ROW) | (0x2 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x6 << TENSOR_QUANT_TRANSF5) | (0x6 << TENSOR_QUANT_TRANSF4) | (0x7 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ITER_1_HID_0:\n"
			"li x6, ((0xd << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0xe << TENSOR_QUANT_QUANT_ROW) | (0x9 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x2 << TENSOR_QUANT_TRANSF4) | (0x7 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ITER_2_HID_0:\n"
			"li x6, ((0x9 << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0xa << TENSOR_QUANT_QUANT_ROW) | (0xa << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x0 << TENSOR_QUANT_TRANSF1) | (0x2 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ITER_3_HID_0:\n"
			"li x5, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x0 << TENSOR_FMA16_B_NUM_COLS) | (0x7 << TENSOR_FMA16_A_NUM_ROWS) | (0x8 << TENSOR_FMA16_A_NUM_COLS) | (0x1 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x9e << TENSOR_FMA16_SCP_LOC_B) | (0xc3 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ITER_4_HID_0:\n"
			"li x6, ((0xc << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0x6 << TENSOR_QUANT_QUANT_ROW) | (0x17 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x6 << TENSOR_QUANT_TRANSF5) | (0x7 << TENSOR_QUANT_TRANSF4) | (0x6 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ITER_5_HID_0:\n"
			"li x5, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x1 << TENSOR_FMA16_B_NUM_COLS) | (0xd << TENSOR_FMA16_A_NUM_ROWS) | (0x7 << TENSOR_FMA16_A_NUM_COLS) | (0x8 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0xcb << TENSOR_FMA16_SCP_LOC_B) | (0x97 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ITER_6_HID_0:\n"
			"li x5, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x3 << TENSOR_FMA16_B_NUM_COLS) | (0x4 << TENSOR_FMA16_A_NUM_ROWS) | (0x0 << TENSOR_FMA16_A_NUM_COLS) | (0x6 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0xe1 << TENSOR_FMA16_SCP_LOC_B) | (0xb6 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ITER_7_HID_0:\n"
			"li x5, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x2 << TENSOR_IMA_B_NUM_COLS) | (0xb << TENSOR_IMA_A_NUM_ROWS) | (0x3 << TENSOR_IMA_A_NUM_COLS) | (0x4 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0xb5 << TENSOR_IMA_SCP_LOC_B) | (0xd8 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x0 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x6, ((0xe << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0xb << TENSOR_QUANT_QUANT_ROW) | (0x27 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x3 << TENSOR_QUANT_TRANSF8) | (0x4 << TENSOR_QUANT_TRANSF7) | (0x3 << TENSOR_QUANT_TRANSF6) | (0x4 << TENSOR_QUANT_TRANSF5) | (0x8 << TENSOR_QUANT_TRANSF4) | (0x3 << TENSOR_QUANT_TRANSF3) | (0x3 << TENSOR_QUANT_TRANSF2) | (0x3 << TENSOR_QUANT_TRANSF1) | (0x8 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ITER_8_HID_0:\n"
			"li x5, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x2 << TENSOR_FMA16_B_NUM_COLS) | (0xe << TENSOR_FMA16_A_NUM_ROWS) | (0x7 << TENSOR_FMA16_A_NUM_COLS) | (0xe << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0xa2 << TENSOR_FMA16_SCP_LOC_B) | (0x69 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x6, ((0x5 << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0xe << TENSOR_QUANT_QUANT_ROW) | (0x3 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x2 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"LBL_HPM_3_CORE_TS_OPS_FAIL_HID_0:\n"          
			"csrr x5, mhpmcounter3\n"
			"sd x5, 32 (x31)\n"
			"LBL_HPM_4_CORE_TL_OPS_FAIL_HID_0:\n"          
			"csrr x5, mhpmcounter4\n"
			"sd x5, 40 (x31)\n"
			"LBL_HPM_5_CORE_TFMA_INST_FAIL_HID_0:\n"          
			"csrr x5, mhpmcounter5\n"
			"sd x5, 48 (x31)\n"
			"LBL_HPM_6_CORE_TIMA_OPS_FAIL_HID_0:\n"          
			"csrr x5, mhpmcounter6\n"
			"sd x5, 56 (x31)\n"
			"LBL_HPM_7_NEIGH_RX_COOP_TST_RSP_FAIL_HID_0:\n"          
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
			"LBL_HPM_3_CORE_TS_OPS_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter3\n"
			"sd x5, 32 (x31)\n"
			"LBL_HPM_4_CORE_TL_OPS_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter4\n"
			"sd x5, 40 (x31)\n"
			"LBL_HPM_5_CORE_TFMA_INST_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter5\n"
			"sd x5, 48 (x31)\n"
			"LBL_HPM_6_CORE_TIMA_OPS_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter6\n"
			"sd x5, 56 (x31)\n"
			"LBL_HPM_7_NEIGH_RX_COOP_TST_RSP_PASS_HID_0:\n"          
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
			"csrwi mhpmevent3, 24\n"   
			"csrwi mhpmevent4, 6\n"   
			"csrwi mhpmevent5, 2\n"   
			"csrwi mhpmevent6, 12\n"   
			"csrwi mhpmevent7, 22\n"   
			"csrwi mhpmevent8, 1\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f22,  480 (x5)\n"
			"flw.ps f18,  320 (x5)\n"
			"flw.ps f27,  832 (x5)\n"
			"flw.ps f28,  896 (x5)\n"
			"flw.ps f8,  64 (x5)\n"
			"flw.ps f11,  128 (x5)\n"
			"flw.ps f29,  384 (x5)\n"
			"flw.ps f17,  576 (x5)\n"
			"flw.ps f12,  224 (x5)\n"
			"flw.ps f9,  288 (x5)\n"
			"flw.ps f15,  96 (x5)\n"
			"flw.ps f4,  448 (x5)\n"
			"flw.ps f23,  992 (x5)\n"
			"flw.ps f5,  704 (x5)\n"
			"flw.ps f7,  960 (x5)\n"
			"flw.ps f13,  0 (x5)\n"
			"flw.ps f21,  736 (x5)\n"
			"flw.ps f6,  672 (x5)\n"
			"flw.ps f1,  160 (x5)\n"
			"flw.ps f14,  512 (x5)\n"
			"flw.ps f30,  928 (x5)\n"
			"flw.ps f3,  256 (x5)\n"
			"flw.ps f0,  192 (x5)\n"
			"flw.ps f19,  768 (x5)\n"
			"flw.ps f24,  608 (x5)\n"
			"flw.ps f16,  544 (x5)\n"
			"flw.ps f25,  32 (x5)\n"
			"flw.ps f10,  352 (x5)\n"
			"flw.ps f31,  416 (x5)\n"
			"flw.ps f26,  640 (x5)\n"
			"flw.ps f2,  800 (x5)\n"
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
			"la x18, rand_ieee754_32\n"
			"flw.ps f6, 992(x18)\n"
			"flw.ps f12, 480(x18)\n"
			"fsgnj.s f28, f6, f12\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_1:\n"
			"maskor m2, m2, m1\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x20, rand_int_32\n"
			"flw.ps f27, 352(x20)\n"
			"flw.ps f12, 352(x20)\n"
			"fmin.pi f11, f27, f12\n" 
			"la x11, rand_int_32\n"
			"flw.ps f24, 576(x11)\n"
			"flw.ps f5, 608(x11)\n"
			"fltu.pi f27, f24, f5\n" 
			"la x25, rand_int_32\n"
			"flw.ps f10, 640(x25)\n"
			"flw.ps f17, 736(x25)\n"
			"fsra.pi f30, f10, f17\n" 
			"fbci.ps f11, 0x604cd\n" 
			"la x28, rand_int_32\n"
			"flw.ps f6, 448(x28)\n"
			"fcvt.ps.pw f23, f6, rne\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f26, 32(x26)\n"
			"flw.ps f21, 32(x26)\n"
			"fmax.s f23, f26, f21\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f20, 480(x6)\n"
			"flw.ps f17, 96(x6)\n"
			"flw.ps f14, 736(x6)\n"
			"fnmsub.s f4, f20, f17, f14, rdn\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f9, 800(x12)\n"
			"flw.ps f3, 704(x12)\n"
			"fsgnjn.s f9, f9, f3\n" 
			"la x25, rand_int_32\n"
			"flw.ps f9, 992(x25)\n"
			"fnot.pi f22, f9\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f21, 448(x20)\n"
			"flw.ps f10, 320(x20)\n"
			"fsgnjx.s f5, f21, f10\n" 
			"la x10, rand_int_32\n"
			"flw.ps f28, 256(x10)\n"
			"fsrai.pi f2, f28, 0xa\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f11, 160(x12)\n"
			"flw.ps f28, 448(x12)\n"
			"fadd.s f13, f11, f28, rdn\n" 
			"la x23, rand_int_32\n"
			"flw.ps f21, 608(x23)\n"
			"flw.ps f6, 416(x23)\n"
			"fltu.pi f22, f21, f6\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_1:\n"
			"maskxor m0, m2, m2\n" 
			"mov.m.x m0, x0, 0xFF\n"
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
			"LBL_HPM_3_CORE_SHORT_OPS_FAIL_HID_1:\n"          
			"csrr x5, mhpmcounter3\n"
			"sd x5, 32 (x31)\n"
			"LBL_HPM_4_CORE_DCACHE_ACCESS0_FAIL_HID_1:\n"          
			"csrr x5, mhpmcounter4\n"
			"sd x5, 40 (x31)\n"
			"LBL_HPM_5_CORE_RETIRED_INST0_FAIL_HID_1:\n"          
			"csrr x5, mhpmcounter5\n"
			"sd x5, 48 (x31)\n"
			"LBL_HPM_6_CORE_L2_EVICT_REQ_FAIL_HID_1:\n"          
			"csrr x5, mhpmcounter6\n"
			"sd x5, 56 (x31)\n"
			"LBL_HPM_7_NEIGH_ETL_REQ_TO_SC_UC_FIFO_FAIL_HID_1:\n"          
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
			"LBL_HPM_3_CORE_SHORT_OPS_PASS_HID_1:\n"          
			"csrr x5, mhpmcounter3\n"
			"sd x5, 32 (x31)\n"
			"LBL_HPM_4_CORE_DCACHE_ACCESS0_PASS_HID_1:\n"          
			"csrr x5, mhpmcounter4\n"
			"sd x5, 40 (x31)\n"
			"LBL_HPM_5_CORE_RETIRED_INST0_PASS_HID_1:\n"          
			"csrr x5, mhpmcounter5\n"
			"sd x5, 48 (x31)\n"
			"LBL_HPM_6_CORE_L2_EVICT_REQ_PASS_HID_1:\n"          
			"csrr x5, mhpmcounter6\n"
			"sd x5, 56 (x31)\n"
			"LBL_HPM_7_NEIGH_ETL_REQ_TO_SC_UC_FIFO_PASS_HID_1:\n"          
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
		0xdd22cba1, 
		0x1d0badba, 
		0x14cc7974, 
		0x32c0af5d, 
		0x20e157a5, 
		0x62134e62, 
		0xc77dab94, 
		0xfbe2488d, 
		0xc7eee89f, 
		0xfa6c03a5, 
		0x16ce26b7, 
		0x96837517, 
		0x85f3fbb6, 
		0x499a3cae, 
		0xf219fefa, 
		0xa7b70d17, 
		0x568215bd, 
		0x65da9f40, 
		0x112e8fb9, 
		0xccaa0d6c, 
		0x001ad593, 
		0x3fe260d7, 
		0x987c8dd9, 
		0x510a7701, 
		0x034e2ea3, 
		0x1b90c7ee, 
		0x7a96a401, 
		0xcffc956a, 
		0x805176dd, 
		0xb9d90e91, 
		0x6fb20694, 
		0xaeac549e, 
		0x0bb4e0ff, 
		0x4c8b5e33, 
		0x349de59a, 
		0x6c03fafc, 
		0xb794aaa6, 
		0x70d8f382, 
		0xcb2b99e2, 
		0x5b906e6c, 
		0x770f6d12, 
		0xa4ad469d, 
		0x4c22a3a6, 
		0x591d3d94, 
		0x8a886f77, 
		0x7613ce64, 
		0x84c63003, 
		0x63ee62ca, 
		0xa5feab68, 
		0xb4e50ba1, 
		0x8acc438c, 
		0x65567d47, 
		0x66a7a562, 
		0x0b867781, 
		0x9eb4e7ec, 
		0xcf38dee4, 
		0x1654d767, 
		0x2f653ae4, 
		0x0aee3677, 
		0xc21643c6, 
		0x5ee86b7f, 
		0x4b461e3e, 
		0xe603b8aa, 
		0xa3238775, 
		0xd4061441, 
		0xb9c2e21b, 
		0x6f3ca401, 
		0x99df6ed1, 
		0xf6d41d9c, 
		0x525de1bc, 
		0x4ef684ba, 
		0x9f10264d, 
		0xb23f69cd, 
		0x9d0d0034, 
		0x092f3358, 
		0xe0d168f3, 
		0xe5c8a371, 
		0x52aa46ac, 
		0xc59c1ac7, 
		0x38a6c462, 
		0x566873b2, 
		0x983f2950, 
		0xb8e696f4, 
		0x0be15d24, 
		0x7e14599c, 
		0xfabc0f05, 
		0xef053fea, 
		0x752ae96b, 
		0x03aae5e8, 
		0x767e5513, 
		0x54b1adf8, 
		0x7ed613ba, 
		0x40c0aeec, 
		0xcecc69af, 
		0x0a310b90, 
		0x97d7b81e, 
		0xa0551dfe, 
		0x5e0ee52d, 
		0xce7df7b3, 
		0x03d76eaa, 
		0x55d380dd, 
		0xae3a3017, 
		0x860f7c66, 
		0x45cc5248, 
		0x27978e61, 
		0x0f3b7b43, 
		0x133dd636, 
		0x8f289900, 
		0xfa9a89b2, 
		0x4192db19, 
		0x025316fe, 
		0x59c7a934, 
		0x8623cc64, 
		0xbeb57ed8, 
		0xd41fad7b, 
		0x974ecf9c, 
		0xee12c4ee, 
		0xfae928fd, 
		0x87cd912a, 
		0x3fb2871c, 
		0x93102cb9, 
		0x102a7c21, 
		0xde004973, 
		0x8969646c, 
		0xa8418a14, 
		0x3e4900f1, 
		0x5203f8e2, 
		0xad63c4a6, 
		0x40edc72d, 
		0x3efad460, 
		0xc6dc52a0, 
		0x34ea892a, 
		0x3bca86b4, 
		0x9bfb7114, 
		0x3d272ab4, 
		0x4b9b944a, 
		0x5d3d983e, 
		0x73a80d9b, 
		0x58b702d2, 
		0xa44ffa4d, 
		0x2ab60335, 
		0x6ef93ca7, 
		0x4111cf54, 
		0xc0d34e80, 
		0x0f9abed9, 
		0xddfb76d6, 
		0xaae39510, 
		0xf2bda40e, 
		0x6131502c, 
		0xdc84d5e4, 
		0xb2d85fed, 
		0x5b438fde, 
		0x1116d69c, 
		0x7e5dcc58, 
		0x83bebc0b, 
		0xe03e0a4f, 
		0x43cd7c82, 
		0xd9a81ebe, 
		0x92b58097, 
		0x3f21ad97, 
		0x9bf0f002, 
		0x3b57a14c, 
		0xbad72728, 
		0x4b3f1a6c, 
		0x2fb9d2fe, 
		0xc37b39d5, 
		0x0fa3c55c, 
		0x04e52fdb, 
		0x177fef84, 
		0xd3cf2c34, 
		0x5c7c6833, 
		0xeea7b3a3, 
		0xd9375c24, 
		0xdc153b59, 
		0xbf4c090e, 
		0x64b01bab, 
		0xb3ccdd9e, 
		0x80b10804, 
		0xca0175c5, 
		0x46079e8d, 
		0x0b4bac84, 
		0x173952c4, 
		0xf17a9774, 
		0xaeb0b8a6, 
		0x179f7508, 
		0x0787ca93, 
		0x154bb941, 
		0x4d9007c8, 
		0x4b59ca54, 
		0x1a6d996d, 
		0xb99aad13, 
		0xa038fc39, 
		0x963ff44b, 
		0xe40f16ab, 
		0xbc3e6f23, 
		0xc8bf0f65, 
		0xff83cb03, 
		0x00feefc2, 
		0x2805d85b, 
		0x3ae4b9e9, 
		0xff118c52, 
		0x6f131d73, 
		0x781ea656, 
		0x5d057057, 
		0xcb544f32, 
		0x2afd4b9c, 
		0xe37b4875, 
		0x914ef0dd, 
		0x529c9212, 
		0x5d8a3559, 
		0xfb4ba998, 
		0x47e17ad3, 
		0xa9d370d6, 
		0xe4103f25, 
		0x901d9404, 
		0xe9c12671, 
		0x4745df3b, 
		0x5c00d867, 
		0x34f94746, 
		0x588aa041, 
		0x0974acbf, 
		0xb862492c, 
		0xc143fa05, 
		0xc2bf2dfb, 
		0xad07b801, 
		0x15f16330, 
		0xf187e64f, 
		0xe487459d, 
		0x8b5c258c, 
		0x32d015d2, 
		0x1b8259a8, 
		0xefa9e651, 
		0x7cd3dcc2, 
		0xc75f5c86, 
		0xb3d2afaa, 
		0x9ff6e70c, 
		0x47ff5037, 
		0x76fdc6f5, 
		0xbe029af5, 
		0xc874c942, 
		0x775ef5d4, 
		0x7281baf9, 
		0xcb5cccbe, 
		0xbe87fb43, 
		0x209aff76, 
		0x9bd5f7a8, 
		0x73fed91f, 
		0x782a2dc6, 
		0x1837a7b7, 
		0x0316dbee, 
		0xe96ebd91, 
		0x31177ce6, 
		0xd4157bee, 
		0x3f89cbde, 
		0x4bb62a2f, 
		0xbc69a982  
};
volatile uint32_t rand_ieee754_32[1536] __attribute__ ((aligned (2048))) = {
		0x00000004, 
		0xc2300000, 
		0x80000800, 
		0xcb8c4b40, 
		0xaaaaaaaa, 
		0x41300000, 
		0x98f01bec, 
		0x0e000007, 
		0x41b00000, 
		0xff800000, 
		0x0c7fc000, 
		0xc1b00000, 
		0x6ad963b7, 
		0x80000002, 
		0x55555555, 
		0x33333333, 
		0x80000000, 
		0xc1f80000, 
		0x0c7ff800, 
		0x00800001, 
		0xc2080000, 
		0x7f800000, 
		0xec0bf6a6, 
		0xc2280000, 
		0x80800001, 
		0x2c195bb7, 
		0xaaaaaaaa, 
		0x80000000, 
		0x79d3db37, 
		0x429773d4, 
		0x0e1fffff, 
		0x7f800000, 
		0xc0800000, 
		0x8f7fffff, 
		0x1d9c5745, 
		0x42480000, 
		0x007ffffe, 
		0x41c00000, 
		0x41f80000, 
		0x0e000007, 
		0x259c6c77, 
		0x40c00000, 
		0x0e007fff, 
		0xbe01ecf8, 
		0x3f800001, 
		0x1989a183, 
		0xc1100000, 
		0x3f800000, 
		0x80800002, 
		0x00000400, 
		0x0e1fffff, 
		0x0e00003f, 
		0xc0e00000, 
		0x0c7ffffe, 
		0x8ba76ae1, 
		0xc2140000, 
		0xc0cf2d6e, 
		0x80011111, 
		0xc1000000, 
		0x42300000, 
		0x80800003, 
		0xa6950eae, 
		0x0c7fffc0, 
		0x4b000000, 
		0xc26c0000, 
		0x0755b8f1, 
		0x00200000, 
		0x42440000, 
		0x80000800, 
		0x002cebe8, 
		0xfb91d143, 
		0x007fffff, 
		0xc2400000, 
		0x1e8d1b07, 
		0x7f800000, 
		0x0c7ff800, 
		0x0e07ffff, 
		0x0e001fff, 
		0x8f7fffff, 
		0x41b80000, 
		0x1911e195, 
		0x41d00000, 
		0x0f7ffffd, 
		0x00000001, 
		0x40400000, 
		0x41a80000, 
		0x80000000, 
		0xc2400000, 
		0x41000000, 
		0xd4dddf2c, 
		0x00040000, 
		0x0e00ffff, 
		0x5bd16ed1, 
		0x41300000, 
		0x36a5debc, 
		0x80800000, 
		0x09a7826d, 
		0x9580ee68, 
		0x00800002, 
		0xffa2d64c, 
		0x807fffff, 
		0x42780000, 
		0xe6198602, 
		0xc1a80000, 
		0x7cdcd197, 
		0xc0400000, 
		0x7fa4c29b, 
		0x7fc00000, 
		0xc2240000, 
		0x0e000001, 
		0x42080000, 
		0xc1000000, 
		0x80000004, 
		0x0d000ff0, 
		0x41d80000, 
		0x80000000, 
		0x443e1682, 
		0xe2172264, 
		0x40c00000, 
		0x807ffffe, 
		0x890e0824, 
		0xc8b76412, 
		0x5e2d7b4d, 
		0x41300000, 
		0xaf1fd0a3, 
		0x80800002, 
		0xc0000000, 
		0xc1900000, 
		0xc2380000, 
		0x7fffffff, 
		0x007ffffe, 
		0x42640000, 
		0x2fe9f22a, 
		0x00008000, 
		0x42640000, 
		0x311addfc, 
		0x3bb887b2, 
		0x1b192896, 
		0x6887fcb0, 
		0xc0a00000, 
		0x0c700000, 
		0x00000000, 
		0x42540000, 
		0xcde1ad8d, 
		0xc1300000, 
		0xc2200000, 
		0x41880000, 
		0xc1400000, 
		0x7f800000, 
		0x0e01ffff, 
		0x00000004, 
		0x00000000, 
		0xff7ffffe, 
		0xd5e91a72, 
		0xc22c0000, 
		0x80000002, 
		0x00080000, 
		0x0f7ffffc, 
		0x7ad22cde, 
		0xc2380000, 
		0x0e0003ff, 
		0xc2340000, 
		0xec541fb7, 
		0x7f7ffffe, 
		0xffc00000, 
		0x4922b741, 
		0x00000000, 
		0x80000001, 
		0x0e000007, 
		0xc2040000, 
		0x1e9000f3, 
		0xc20c0000, 
		0x0e0003ff, 
		0xffbfffff, 
		0x7f825c49, 
		0x40c00000, 
		0x0e45a2d4, 
		0xc0c345c8, 
		0x0e000fff, 
		0x7d394a92, 
		0xabc01110, 
		0x7f800000, 
		0xc2740000, 
		0x00007c80, 
		0x7f86c5d1, 
		0x42440000, 
		0x7f7fffff, 
		0xe6465176, 
		0x8064bba8, 
		0x0e0007ff, 
		0x6f5c27e2, 
		0x3b8fd773, 
		0x00000000, 
		0x80000002, 
		0x1684d40b, 
		0xff000000, 
		0xf2daccd8, 
		0x5db323d5, 
		0x0e00007f, 
		0x7fffffff, 
		0x00004000, 
		0x42140000, 
		0x00080000, 
		0x0d00fff0, 
		0x0e00ffff, 
		0xffc00000, 
		0x00000000, 
		0x7f8d9513, 
		0x3f800000, 
		0xc1f80000, 
		0x41e80000, 
		0xc2740000, 
		0x3f800000, 
		0xc1500000, 
		0xc1337a86, 
		0x00000001, 
		0xc25c0000, 
		0x7f800000, 
		0xa34896c8, 
		0x0e00003f, 
		0x00000000, 
		0x0c7ffff0, 
		0x41a80000, 
		0xc2580000, 
		0x0e000003, 
		0xc2680000, 
		0xff800001, 
		0x0c600000, 
		0x8f7ffffe, 
		0x80800003, 
		0x0c7fe000, 
		0x0e3fffff, 
		0xd0c99d1f, 
		0xc0400000, 
		0x8f7fffff, 
		0x0f7ffffc, 
		0xc24c0000, 
		0x00200000, 
		0x3f028f5c, 
		0xff800000, 
		0x7f800000, 
		0x553977f2, 
		0x0e003fff, 
		0xc0000000, 
		0xc2740000, 
		0x7f800000, 
		0x0e0000ff, 
		0x80800000, 
		0x7fffffff, 
		0xc2780000, 
		0x7fbfffff, 
		0xc2440000, 
		0x16a178b8, 
		0x0e00000f, 
		0x0e00000f, 
		0x42380000, 
		0xc1d00000, 
		0x42280000, 
		0xfe9929ca, 
		0xc2600000, 
		0x5a0d49b5, 
		0x685e428e, 
		0x00000001, 
		0xc0c00000, 
		0xb6bcb447, 
		0x00008000, 
		0xa8dd04bd, 
		0x42480000, 
		0x420c0000, 
		0x41700000, 
		0x0c7f8000, 
		0xee1941b8, 
		0x42240000, 
		0x80000001, 
		0x80000200, 
		0xffc00001, 
		0xc24c0000, 
		0x0c7fffe0, 
		0x80008000, 
		0x00800000, 
		0x00004000, 
		0xc21c0000, 
		0xcb000000, 
		0x175add48, 
		0x42480000, 
		0x4eeff0c6, 
		0x80000040, 
		0x00000001, 
		0xc1a00000, 
		0x41800000, 
		0xa575388c, 
		0x00800002, 
		0x0e00000f, 
		0xaaaaaaaa, 
		0x0c7f0000, 
		0x0c7fff00, 
		0xc2280000, 
		0x7fc00000, 
		0x29bd1eb7, 
		0x42080000, 
		0x33de03e7, 
		0x42400000, 
		0x5c7a9e03, 
		0x800b568e, 
		0xc1b80000, 
		0x33333333, 
		0x42680000, 
		0x7f7fffff, 
		0x80000000, 
		0xff800000, 
		0x80000040, 
		0xc2580000, 
		0x40a00000, 
		0xc2580000, 
		0x00000000, 
		0xc24c0000, 
		0xd2c9a3c6, 
		0xc9229f31, 
		0x2cb6489a, 
		0x0e00007f, 
		0xc2380000, 
		0x8f7ffffd, 
		0x42680000, 
		0x427c0000, 
		0x42080000, 
		0x80008000, 
		0x0c7ffff8, 
		0xbf800001, 
		0x00000000, 
		0x9bf41634, 
		0x0e0007ff, 
		0xd6b758d0, 
		0x00000020, 
		0x0c7fffc0, 
		0x00000008, 
		0xba12c1c2, 
		0x00800000, 
		0x00000040, 
		0x80000001, 
		0x7f7ffffe, 
		0xc2200000, 
		0x00000000, 
		0xc1000000, 
		0xa50c9d53, 
		0x42180000, 
		0xc42d4704, 
		0x80000000, 
		0xffbfffff, 
		0x33d32600, 
		0x7f7ffffe, 
		0x42300000, 
		0xbf028f5c, 
		0xf5e6d0ce, 
		0x80000002, 
		0x0c7fffc0, 
		0x41100000, 
		0x8f7ffffe, 
		0x4e811957, 
		0x80000020, 
		0xac539b75, 
		0x410d5e24, 
		0x41d00000, 
		0xc2600000, 
		0x7f7ffffe, 
		0x80000004, 
		0x7fbfffff, 
		0x3b537b95, 
		0x7f9fff4b, 
		0x41000000, 
		0x0033981e, 
		0x80000000, 
		0xc1800000, 
		0x00000200, 
		0x40a00000, 
		0x42600000, 
		0x0851cc93, 
		0x3f028f5c, 
		0x0c600000, 
		0x00000000, 
		0xc0e00000, 
		0x00800003, 
		0xc2140000, 
		0xff800000, 
		0x807ffffe, 
		0x41c00000, 
		0x70b99406, 
		0x0ad628e1, 
		0x42480000, 
		0x42700000, 
		0x0e001fff, 
		0x7f800000, 
		0x7fc00001, 
		0xb999c1d9, 
		0xb46830f4, 
		0xc1c00000, 
		0xff800000, 
		0xd9a8083e, 
		0x80000000, 
		0x6a6ef5b7, 
		0x0f7ffffc, 
		0x42440000, 
		0xbba19abe, 
		0xbf800001, 
		0x1b3f9a43, 
		0x3a7dc2f3, 
		0x0e0007ff, 
		0x80000000, 
		0xc1ba349c, 
		0x41d80000, 
		0x7202ba85, 
		0xfb355638, 
		0x41d80000, 
		0xc2340000, 
		0xc1300000, 
		0x42340000, 
		0x41980000, 
		0xc2600000, 
		0x42040000, 
		0x00609a12, 
		0xae9497c7, 
		0x0c25999c, 
		0x41f00000, 
		0x1ad49d0f, 
		0x0e000001, 
		0xff800000, 
		0x00800002, 
		0x7f800000, 
		0xc2180000, 
		0x0e0003ff, 
		0xc2040000, 
		0xff7ffffe, 
		0xc1b00000, 
		0x42500000, 
		0xe1ebea00, 
		0x41f80000, 
		0x9d510137, 
		0x8a6b2814, 
		0x41600000, 
		0x80011111, 
		0xc1400000, 
		0x00008000, 
		0x0c400000, 
		0xff9edb3c, 
		0xc576f396, 
		0x00000010, 
		0x80000000, 
		0x0d00fff0, 
		0x80100000, 
		0xc2340000, 
		0x599f253f, 
		0x41000000, 
		0x80800002, 
		0x80008000, 
		0x80000020, 
		0xc0a00000, 
		0x7f84debd, 
		0xd46f135e, 
		0x098ce0f7, 
		0x41600000, 
		0x41b00000, 
		0x3a55a7f6, 
		0xc23c0000, 
		0x42180000, 
		0x42780000, 
		0x424c0000, 
		0xc2600000, 
		0xff7ffffe, 
		0xff800000, 
		0x42640000, 
		0x80000001, 
		0xc1e00000, 
		0x0f7ffffc, 
		0x00000100, 
		0x7f7ffffe, 
		0x41a00000, 
		0x0e1fffff, 
		0x0e003fff, 
		0x9c07e54f, 
		0xc0a00000, 
		0x7fc00000, 
		0x80000000, 
		0xc20c0000, 
		0x00000400, 
		0x0e07ffff, 
		0xcb8b260c, 
		0xc2680000, 
		0x0b64d742, 
		0x0f7ffffe, 
		0x8f7ffffe, 
		0x0e00003f, 
		0x7f7ffffe, 
		0x40c00000, 
		0xbf3b3f21, 
		0x3c46e339, 
		0x0e0000ff, 
		0x80000400, 
		0x59812f4b, 
		0xff7ffffe, 
		0xf8ae13c3, 
		0x5684c930, 
		0x7f800000, 
		0x7f272e23, 
		0x7f800000, 
		0xfce2c457, 
		0x8eb0f4ca, 
		0x7fc00001, 
		0xc4e58be4, 
		0x00800001, 
		0xc8777a54, 
		0xd3c92989, 
		0x9784f85a, 
		0xb95c503f, 
		0xc1400000, 
		0x80000000, 
		0xc1f00000, 
		0x00000080, 
		0x80800000, 
		0xec97e017, 
		0xde04215e, 
		0xff800001, 
		0xcc9c412b, 
		0x0c7ff000, 
		0xebf08a78, 
		0xc1100000, 
		0x41980000, 
		0xff7ffffe, 
		0xae02fa55, 
		0x00020000, 
		0xcb8c4b40, 
		0xe142d999, 
		0x3f028f5c, 
		0x51283258, 
		0x3f800000, 
		0x7f7ffffe, 
		0xc2080000, 
		0x0c7fff00, 
		0xc4efbebc, 
		0x00000080, 
		0x3f800000, 
		0x2d77c67e, 
		0x0c7f0000, 
		0x0d000ff0, 
		0xc0e00000, 
		0x00000008, 
		0xc2100000, 
		0x00000800, 
		0x42040000, 
		0x7f7ffffe, 
		0xc1a80000, 
		0x7f800000, 
		0x80000000, 
		0x0e3fffff, 
		0x00080000, 
		0x41e00000, 
		0xebc891f0, 
		0xc2640000, 
		0xf81ba709, 
		0x0c7ffffc, 
		0x518df47a, 
		0xc2000000, 
		0x80000000, 
		0x0c7fe000, 
		0x7f8c85a4, 
		0xff800000, 
		0x7f88dd58, 
		0x80000040, 
		0xc1200000, 
		0x42740000, 
		0x42140000, 
		0x41400000, 
		0x40c00000, 
		0x7f869ae4, 
		0x270fcf03, 
		0x0e000007, 
		0xe596c23b, 
		0x7f9570a2, 
		0xba3deb42, 
		0x73007a5e, 
		0x00000000, 
		0x42280000, 
		0x00800000, 
		0x41700000, 
		0x7fc00001, 
		0x00400000, 
		0xff800000, 
		0xc1c00000, 
		0x424c0000, 
		0x0e07ffff, 
		0xff967d1d, 
		0xc1c80000, 
		0x00001000, 
		0xbaa24fed, 
		0x20c8fcda, 
		0xc1d00000, 
		0x0a1431d4, 
		0x636cc1b1, 
		0x87073bf8, 
		0x42500000, 
		0xc2400000, 
		0x80000004, 
		0xa0147742, 
		0x41800000, 
		0xc1980000, 
		0x0e1fffff, 
		0xc0800000, 
		0x7f800000, 
		0x0e7fffff, 
		0x6161110b, 
		0x41a80000, 
		0xf06f3ec5, 
		0x00000000, 
		0x40400000, 
		0x0f7ffffd, 
		0xb9a947e3, 
		0x8f7ffffd, 
		0x41880000, 
		0x42100000, 
		0xc2100000, 
		0x0e000003, 
		0xc1800000, 
		0x0c7ffe00, 
		0x42180000, 
		0xc20c0000, 
		0xc2580000, 
		0xaf622103, 
		0x7f7ffffe, 
		0x7f7ffffe, 
		0x0c7ff800, 
		0x4b000000, 
		0x798a3555, 
		0x40800000, 
		0x424c0000, 
		0xe8bb848e, 
		0xc2540000, 
		0xff800001, 
		0x0c7fc000, 
		0xbe19cf33, 
		0x80011111, 
		0x8f7fffff, 
		0xffffffff, 
		0xff7fffff, 
		0x4a527a11, 
		0xc1500000, 
		0x7f800000, 
		0x42140000, 
		0x41e80000, 
		0x7f7ffffe, 
		0x80000001, 
		0xc1900000, 
		0x422c0000, 
		0xc532d520, 
		0x41f80000, 
		0xc1600000, 
		0x00000000, 
		0x8c7b2c9b, 
		0x04e0426c, 
		0x807ffffe, 
		0x4b8c4b40, 
		0xff7ffffe, 
		0x00800000, 
		0xca223b74, 
		0xff7ffffe, 
		0x7fac8aab, 
		0x7fefd40b, 
		0x80100000, 
		0x7fc00000, 
		0xb988a33f, 
		0x40000000, 
		0x233af1c6, 
		0xffb8411d, 
		0x3f800001, 
		0x0c7f8000, 
		0x00011111, 
		0xc1600000, 
		0x60c8d357, 
		0x80000000, 
		0x8004f34f, 
		0x0dc5fc8b, 
		0x41300000, 
		0x8060cab7, 
		0x00800000, 
		0x425c0000, 
		0x0e001fff, 
		0x0f7ffffe, 
		0x00000000, 
		0x80000001, 
		0x0db0e090, 
		0x00000000, 
		0xc1e00000, 
		0xc2200000, 
		0x00004000, 
		0xd4e3434a, 
		0x0e0001ff, 
		0xb441a3aa, 
		0x5bb45cd4, 
		0x0e07ffff, 
		0x2fa158e8, 
		0xff800000, 
		0x3f800001, 
		0x00000002, 
		0x42040000, 
		0x5aae3a19, 
		0x80000002, 
		0x0c7ff000, 
		0x00400000, 
		0x78c69bf4, 
		0x12dadc04, 
		0x69dc1fa0, 
		0x80000200, 
		0xc3b698c3, 
		0x7f800000, 
		0x00000000, 
		0xaaaaaaaa, 
		0xc2480000, 
		0x00020000, 
		0xd784682a, 
		0x7f800000, 
		0x00800000, 
		0x5ee63006, 
		0x0e000001, 
		0x3a4b13f4, 
		0x0e000fff, 
		0x42340000, 
		0xc1c80000, 
		0x88c3e457, 
		0xe0d5ece1, 
		0x80004000, 
		0xc0400000, 
		0x7f800000, 
		0x423c0000, 
		0x9102107a, 
		0x80000010, 
		0xc0000000, 
		0x41c00000, 
		0x3f028f5c, 
		0x41880000, 
		0xf2ae2d4b, 
		0x21563bea, 
		0x41c00000, 
		0x77adb7a8, 
		0x807fffff, 
		0x48dbade5, 
		0x00000004, 
		0xbfc7442b, 
		0xe3b0c288, 
		0x0c7f8000, 
		0x00000000, 
		0x0e7fffff, 
		0x00800003, 
		0xc20c0000, 
		0x7f7ffffe, 
		0xc2540000, 
		0x8d34395b, 
		0xbafd2f03, 
		0x0ff34fe7, 
		0x41f00000, 
		0xe8df4969, 
		0x8a581987, 
		0x80800003, 
		0xbf800000, 
		0xbbbe40f5, 
		0x00000040, 
		0xc2200000, 
		0x2d923a60, 
		0x00010000, 
		0x0e03ffff, 
		0x2652c3d7, 
		0x00000020, 
		0xb5df97a1, 
		0x201dbabf, 
		0x00800001, 
		0x80000000, 
		0x8f7ffffe, 
		0x00076913, 
		0x41b80000, 
		0x00000080, 
		0x0c7c0000, 
		0x41e80000, 
		0xf6a05023, 
		0x7fc00001, 
		0x55555555, 
		0x6bd15158, 
		0x0c7fff00, 
		0x42340000, 
		0x41c80000, 
		0x0e0007ff, 
		0xc1500000, 
		0xc21c0000, 
		0x00000000, 
		0xc6beff29, 
		0x0e0fffff, 
		0x00000000, 
		0xe9ccd6d0, 
		0x34c17e8f, 
		0x80000000, 
		0xff800000, 
		0x9220026b, 
		0x3f800000, 
		0x421c0000, 
		0xc1f00000, 
		0x361a7965, 
		0x80000000, 
		0x0f7ffffd, 
		0x3b5982a8, 
		0x80000000, 
		0xff7ffffe, 
		0x80000001, 
		0x52845bd5, 
		0x80000000, 
		0x7f800000, 
		0xc2380000, 
		0x00000000, 
		0x38f93d57, 
		0x00800002, 
		0x40000000, 
		0x00020000, 
		0x0c7ffffc, 
		0xc2340000, 
		0xc806c7ab, 
		0x425c0000, 
		0xc26c0000, 
		0x7fc00000, 
		0xeba33b9e, 
		0xc24c0000, 
		0x424c0000, 
		0xc1800000, 
		0x422c0000, 
		0x7f800000, 
		0x0e7fffff, 
		0x8013ecc7, 
		0x40800000, 
		0x80069cbf, 
		0x80100000, 
		0x42700000, 
		0xcb8c4b40, 
		0x0e07ffff, 
		0xffa011f2, 
		0xca5b7ae7, 
		0x80020000, 
		0x41900000, 
		0x0043d83e, 
		0x80000200, 
		0x0c7ffffc, 
		0x80100000, 
		0x42700000, 
		0xc1000000, 
		0x42600000, 
		0xc1600000, 
		0xdce1403f, 
		0x423c0000, 
		0xd41f8568, 
		0x7b14f628, 
		0x80000000, 
		0x7f800000, 
		0x8e008eda, 
		0xff800000, 
		0xffc00001, 
		0x60104fe8, 
		0xc2300000, 
		0x0c7f8000, 
		0xc25c0000, 
		0x80800000, 
		0x72e93222, 
		0x3f028f5c, 
		0x42500000, 
		0xc0000000, 
		0x27c18293, 
		0x0c7f8000, 
		0x00000000, 
		0x80080000, 
		0x8c352e35, 
		0xc26c0000, 
		0x40800000, 
		0x0c780000, 
		0x0c7e0000, 
		0x7f800000, 
		0x0d00fff0, 
		0x80000400, 
		0x41500000, 
		0x80100000, 
		0xa2b787a4, 
		0x000d1785, 
		0x7f9691c6, 
		0x80000000, 
		0x80bd9621, 
		0xf0827bbd, 
		0xffffffff, 
		0x41100000, 
		0xff800000, 
		0xc1e00000, 
		0xc23c0000, 
		0x00004000, 
		0xc6b03634, 
		0xc2400000, 
		0x0d4b453e, 
		0xeec762ac, 
		0xc25c0000, 
		0xc26c0000, 
		0x80000020, 
		0xc0c00000, 
		0xd193bdd5, 
		0xff800000, 
		0xc2600000, 
		0x4a7f4bbb, 
		0xab32efe5, 
		0xc2440000, 
		0xf985fc7d, 
		0x41880000, 
		0xc23c0000, 
		0x8c91b095, 
		0x00002000, 
		0x00100000, 
		0x8bf81cf0, 
		0x77a397d3, 
		0x41d80000, 
		0x00800001, 
		0xc24c0000, 
		0x42240000, 
		0xc22c0000, 
		0xc1800000, 
		0x0c7fffc0, 
		0x0c7ffff0, 
		0xd64813c7, 
		0xc1100000, 
		0x007fffff, 
		0x80000000, 
		0x00800002, 
		0x00000002, 
		0x00000000, 
		0xbeb046e0, 
		0x80000000, 
		0x0e00007f, 
		0x42440000, 
		0x0e00ffff, 
		0xd7bb93d2, 
		0xaefa384c, 
		0x41d80000, 
		0xc1300000, 
		0x0e01ffff, 
		0x80289bb4, 
		0x0e001fff, 
		0xb6eeb28d, 
		0x80080000, 
		0x8d54ca6e, 
		0xc23c0000, 
		0xa128bfed, 
		0x3f800001, 
		0xc1d00000, 
		0x0e003fff, 
		0x427a20ee, 
		0xc1200000, 
		0x807ffffe, 
		0x52b4ed15, 
		0xc2040000, 
		0x15611c9d, 
		0x00001000, 
		0xc2300000, 
		0x4b283404, 
		0xfc93d5a4, 
		0x69d2638e, 
		0xc1a80000, 
		0xff800001, 
		0xc2540000, 
		0x56f4e77f, 
		0x42480000, 
		0x1f83d54e, 
		0x3f028f5c, 
		0x55583ca7, 
		0x42740000, 
		0x00000002, 
		0x00000000, 
		0x7fbfffff, 
		0x42780000, 
		0x6925ab32, 
		0x0c7ffc00, 
		0x7f800000, 
		0x7f800000, 
		0x00000000, 
		0x7f800000, 
		0xe15c05c4, 
		0xfb675dfa, 
		0x09d46e97, 
		0x41400000, 
		0xf88775e5, 
		0x80000000, 
		0x180cc54c, 
		0xc1d80000, 
		0x729c5623, 
		0x0e03ffff, 
		0x0e3fffff, 
		0xffa5e539, 
		0x0f7ffffe, 
		0xc24c0000, 
		0xa2b1c0dd, 
		0x127e06f2, 
		0x41e80000, 
		0x52b6a895, 
		0x80000000, 
		0xc2600000, 
		0x7f800000, 
		0x00400000, 
		0x42740000, 
		0x807ffffe, 
		0x00000000, 
		0x1ccfa853, 
		0x5631ad40, 
		0x42700000, 
		0xff7ffffe, 
		0x96046bd4, 
		0xc2080000, 
		0x9ac11d79, 
		0x80000000, 
		0x709e10d5, 
		0x00800000, 
		0x925db132, 
		0xc21c0000, 
		0xa4617c62, 
		0xff800000, 
		0xff800000, 
		0x0c7ffff0, 
		0x21171107, 
		0x7fafb2d5, 
		0xaa1e5b95, 
		0x7f800000, 
		0x0c780000, 
		0x8f7ffffd, 
		0xf0b7a03f, 
		0x80800000, 
		0xe65d02ee, 
		0x0e03ffff, 
		0xf368ddd0, 
		0x0e0fffff, 
		0xaaaaaaaa, 
		0x00011111, 
		0x00000080, 
		0xc2500000, 
		0x426c0000, 
		0x41b00000, 
		0xe86da85e, 
		0x00000008, 
		0x83100203, 
		0x782434da, 
		0x41c00000, 
		0x80800002, 
		0x856fff27, 
		0x1adbfedd, 
		0x80040000, 
		0xbf800000, 
		0x00011111, 
		0x0c7ffffc, 
		0xc2500000, 
		0xa895e99d, 
		0x005fcdcc, 
		0xff81e2fe, 
		0x42380000, 
		0xc2100000, 
		0x7f8c497c, 
		0x69d4d2af, 
		0x41000000, 
		0x80000000, 
		0x41d00000, 
		0x41b80000, 
		0x0e01ffff, 
		0x41300000, 
		0x42400000, 
		0x17e88a93, 
		0x4b000000, 
		0xc1700000, 
		0x5a4f93b6, 
		0x34d86a73, 
		0x7f800001, 
		0x00001000, 
		0xcccccccc, 
		0x0c7e0000, 
		0xffb3e499, 
		0xdbc8dc29, 
		0xc2780000, 
		0xd383d4e9, 
		0x41400000, 
		0x41e80000, 
		0xc1d80000, 
		0xc1300000, 
		0x3c20c8fd, 
		0x27f71970, 
		0x41880000, 
		0x571bb3c1, 
		0x00000001, 
		0x4328895c, 
		0x0e0000ff, 
		0xc24c0000, 
		0xc642bf3b, 
		0x0c7fffe0, 
		0xced26275, 
		0xbf800000, 
		0x0e00003f, 
		0x58d93b90, 
		0x805f5972, 
		0xc0e00000, 
		0x00000000, 
		0x80000000, 
		0x42300000, 
		0x0e0003ff, 
		0xc2040000, 
		0x0c7fe000, 
		0x80800002, 
		0xedfbb73d, 
		0x1de90320, 
		0x42680000, 
		0xff800000, 
		0x00000000, 
		0x0e01ffff, 
		0x0e00001f, 
		0xa712e58a, 
		0x0c7fffe0, 
		0xffc00001, 
		0x7fc00001, 
		0x7f800000, 
		0x0e00000f, 
		0xc23c0000, 
		0x7f800000, 
		0x00000000, 
		0x51817c34, 
		0xca404832, 
		0xde3b1df4, 
		0x3a338f09, 
		0xff800000, 
		0x30d12125, 
		0xc2380000, 
		0x40c00000, 
		0x40800000, 
		0xc2200000, 
		0x0e0001ff, 
		0xc26c0000, 
		0xff7ffffe, 
		0x0e1fffff, 
		0xff800000, 
		0x0e1fffff, 
		0x00000000, 
		0xff000000, 
		0x0c7ffff8, 
		0xffffffff, 
		0x00000040, 
		0x80000002, 
		0xc26c0000, 
		0x7f800000, 
		0xc27c0000, 
		0x00400000, 
		0xc26c0000, 
		0x0e00001f, 
		0x3eeb82fd, 
		0x80010000, 
		0x41e00000, 
		0x41e00000, 
		0x351869f9, 
		0xc2180000, 
		0x80000001, 
		0x7f800001, 
		0xd92089b0, 
		0x8fe0f1b2, 
		0x46ddfde5, 
		0x898d5fbe, 
		0x0c7fffff, 
		0x30d40a45, 
		0xf9cb9fad, 
		0x2c934ae5, 
		0x495e26d6, 
		0x41400000, 
		0x0c7ffffc, 
		0x7fffffff, 
		0x80000000, 
		0x00800001, 
		0xff7fffff, 
		0xb99f3421, 
		0x42300000, 
		0x703cf83d, 
		0x9461ea65, 
		0x80008000, 
		0x80800000, 
		0x0e3fffff, 
		0x0e03ffff, 
		0x0c7fffe0, 
		0x0c7ff800, 
		0xc2580000, 
		0xc2400000, 
		0x00001000, 
		0xffc00000, 
		0x42580000, 
		0x8f7ffffd, 
		0xc0c00000, 
		0x0c7ffff0, 
		0x41e00000, 
		0xb7991070, 
		0xc2140000, 
		0x708999ef, 
		0x00000001, 
		0x0e0003ff, 
		0x42080000, 
		0x40800000, 
		0xc25c0000, 
		0x0e0007ff, 
		0xfff4d83a, 
		0xffab0150, 
		0x00011111, 
		0x0c7f8000, 
		0xc27c0000, 
		0x7f800000, 
		0x704676e4, 
		0xc0c00000, 
		0x1c48121a, 
		0x80000002, 
		0xffbfffff, 
		0x52c07183, 
		0x42140000, 
		0xc1500000, 
		0x42580000, 
		0x0c780000, 
		0xc2680000, 
		0x807ffffe, 
		0x00800000, 
		0x0c780000, 
		0x80000000, 
		0xd0b49a3e, 
		0x88f69e94, 
		0x00000004, 
		0x0e00001f, 
		0x40400000, 
		0x7f7fffff, 
		0x80100000, 
		0x838fe25d, 
		0xad6e0b14, 
		0x0e000003, 
		0x00011111, 
		0x96a2714d, 
		0xff800000, 
		0x00800001, 
		0xff800000, 
		0x7fc00001, 
		0x00400000, 
		0xc2180000, 
		0x00020000, 
		0xe8cb16c3, 
		0xff800000, 
		0x00800001, 
		0xc1b80000, 
		0xc4dfa4ca, 
		0xc24c0000, 
		0xc2500000, 
		0x1e758559, 
		0xc26c0000, 
		0x007d46f8, 
		0x0ac1ae24, 
		0xfbabc6ee, 
		0x00000000, 
		0xc2240000, 
		0x80000000, 
		0x807ffffe, 
		0xb146910f, 
		0x41100000, 
		0xfdc1092e, 
		0x0e0001ff, 
		0x80200000, 
		0x41f80000, 
		0x42180000, 
		0x80800003, 
		0x18f7fd1a, 
		0x41200000, 
		0xc2100000, 
		0xc1900000, 
		0x0c7f8000, 
		0xc1e80000, 
		0x7ae89193, 
		0x847bbe30, 
		0x42280000, 
		0xff800000, 
		0x00002000, 
		0x80800002, 
		0x0e000003, 
		0x330f3945, 
		0x00800001, 
		0x80000000, 
		0x80800001, 
		0x42140000, 
		0xc1e00000, 
		0x0c7f8000, 
		0xfc30e691, 
		0x8ead4746, 
		0x80002000, 
		0xff7ffffe, 
		0x505048db, 
		0x0c400000, 
		0x3236bea5, 
		0x11b5986b, 
		0x40eb1c31, 
		0xc2540000, 
		0x80000008, 
		0xc2480000, 
		0xff800001, 
		0x05b08f91, 
		0x80800001, 
		0x41e00000, 
		0xc1f00000, 
		0x80000000, 
		0xff800000, 
		0x0f7ffffd, 
		0xeb3e1390, 
		0xc1e00000, 
		0x9b8ceeb2, 
		0x0e00000f, 
		0x423c0000, 
		0x80010000, 
		0xff7fffff, 
		0xc2700000, 
		0x7f9a08b5, 
		0x00000000, 
		0xc5c26abe, 
		0x42180000, 
		0xd4317ea4, 
		0x7dc1d394, 
		0x2385a080, 
		0x0c3bf53b, 
		0x7c524c98, 
		0x7f870e88, 
		0x77dde0eb, 
		0x42380000, 
		0x80800003, 
		0x0c7fffc0, 
		0xff800000, 
		0x002fe225, 
		0xc2440000, 
		0x80000000, 
		0xc25c0000, 
		0x00200000, 
		0xc1b00000, 
		0xdf48949f, 
		0xc26c0000, 
		0x6fc9b1bc, 
		0xc1880000, 
		0x541d338e, 
		0x0e0007ff, 
		0x42480000, 
		0xc2600000, 
		0x00800003, 
		0xc2340000, 
		0x40e00000, 
		0x00000000, 
		0x0e001fff, 
		0x0c7ffffc, 
		0x7f800000, 
		0xc1900000, 
		0x80800002, 
		0x80000000, 
		0x80040000, 
		0xd010bf74, 
		0x0a728533, 
		0xff800000, 
		0x00080000, 
		0x62e4675c, 
		0x8dad0c11, 
		0xbd5d91cf, 
		0x0f7ffffc, 
		0xc1600000, 
		0x0c700000, 
		0xc2500000, 
		0x00000000, 
		0x80000000, 
		0x80004000, 
		0x80800001, 
		0x54076e20, 
		0xc2300000, 
		0x0d000ff0, 
		0xc2440000, 
		0x41b00000, 
		0x0c400000, 
		0x0c7ffe00, 
		0x00000040, 
		0x80800001, 
		0x00000000, 
		0x9903e9a4, 
		0x7f800000, 
		0x80800001, 
		0x80000000, 
		0x80000004, 
		0x33333333, 
		0x80002000, 
		0x42640000, 
		0x42780000, 
		0x89fc2ed6, 
		0x00000000, 
		0x801a86d2, 
		0x0e000003, 
		0xc2200000, 
		0x3f028f5c, 
		0x0d000ff0, 
		0x80ffffff, 
		0xc2340000, 
		0xc2740000, 
		0x007ffffe, 
		0x41b00000, 
		0xc1f80000, 
		0x40000000, 
		0xc1600000, 
		0xef15b07f, 
		0xc2700000, 
		0xff800000, 
		0x0c7ffff0, 
		0x0e0001ff, 
		0x80000002, 
		0x00000000, 
		0x41800000, 
		0xab923242, 
		0xff800001, 
		0x421c0000, 
		0x8bd694b0, 
		0x00000200, 
		0x80000000, 
		0x41a80000, 
		0x7f800001, 
		0x80100000, 
		0x4b8c4b40, 
		0x42440000, 
		0x0a472a1a, 
		0xaafac8cb, 
		0xff7ffffe, 
		0x80800002, 
		0x70a7b4da, 
		0x00000000, 
		0x615dcf5a, 
		0xc2740000, 
		0x00000000, 
		0x00000000, 
		0x00800000, 
		0x42540000, 
		0x40e00000, 
		0x4c0e34a7, 
		0x00008000, 
		0x80000020, 
		0xc1880000, 
		0x00400000, 
		0x0c7ff800, 
		0x0d6ed30d, 
		0x801a71fb, 
		0x00400000, 
		0x80800000, 
		0xc1900000, 
		0x00000000, 
		0xc1400000, 
		0xc20c0000, 
		0x85b5247b, 
		0x0c7f0000, 
		0x40a00000, 
		0xf0e5412c, 
		0x7f7ffffe, 
		0x8f7ffffd, 
		0x424c0000, 
		0x0e0000ff, 
		0x41300000, 
		0x0e000007, 
		0x5f1cb415, 
		0xc1c00000, 
		0x80000001, 
		0x00040000, 
		0x41c00000, 
		0xc0a00000, 
		0x80000000, 
		0x86910c1b, 
		0x41a80000, 
		0xc1f00000, 
		0xc1400000, 
		0x41600000, 
		0xc0000000, 
		0x80000100, 
		0x4a681873, 
		0xb46625ba, 
		0x13dac741, 
		0x7f7ffffe, 
		0x67f03316, 
		0x106ec620, 
		0x91b10121, 
		0xc1500000, 
		0x80000000, 
		0x0e00001f, 
		0x0f7ffffd, 
		0x0c7ffc00, 
		0x80000000, 
		0xece801a8, 
		0x80008000, 
		0xcd209210, 
		0x80ffffff, 
		0x02d6c5a4, 
		0xdf176646, 
		0xd4fb11eb, 
		0xff800000, 
		0x0c7ff000, 
		0x0c7ffffc, 
		0x0e03ffff, 
		0xa1366567, 
		0x66f4051e, 
		0x19a315f2, 
		0x00008000, 
		0x7fffffff, 
		0xffc1bf75, 
		0x8f7ffffc, 
		0xc1400000, 
		0x00001000, 
		0x7f800000, 
		0xc27c0000, 
		0x41800000, 
		0x41600000, 
		0x00020000, 
		0x7f854d69, 
		0xc21c0000, 
		0x80011111, 
		0x40400000, 
		0x1df9f436, 
		0xf0bb7bd0, 
		0x40c00000, 
		0x80000000, 
		0x0c7fc000, 
		0x7f7ffffe, 
		0x80000000, 
		0x00800000, 
		0x0b441c3a, 
		0x3c67e17d, 
		0x41800000, 
		0x00100000, 
		0x420c0000, 
		0xc1a00000, 
		0x42540000, 
		0x5160d75b, 
		0xbc334376, 
		0xc23c0000, 
		0x00000008, 
		0x2ad26cf0, 
		0x930d0ab5, 
		0xc1b00000, 
		0x8f7fffff  
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
		0x00000007, 
		0x00000177, 
		0x000002d9, 
		0x000000d5, 
		0x000001b7, 
		0x000002c5, 
		0x000001f5, 
		0x0000020d, 
		0x000000f7, 
		0x000000dd, 
		0x0000009b, 
		0x000003b3, 
		0x00000385, 
		0x00000271, 
		0x000000d7, 
		0x00000059, 
		0x0000027b, 
		0x000003b3, 
		0x0000029b, 
		0x00000115, 
		0x0000019f, 
		0x000001db, 
		0x0000025b, 
		0x00000277, 
		0x000002a7, 
		0x0000014b, 
		0x00000175, 
		0x00000205, 
		0x000000ab, 
		0x00000129, 
		0x000002f3, 
		0x0000000d, 
		0x000001d9, 
		0x000002a3, 
		0x000002fd, 
		0x00000387, 
		0x0000009f, 
		0x000003ff, 
		0x00000123, 
		0x000002d9, 
		0x00000373, 
		0x0000004f, 
		0x00000035, 
		0x000000c5, 
		0x00000375, 
		0x00000127, 
		0x0000029f, 
		0x00000141, 
		0x000002c1, 
		0x0000004d, 
		0x0000025b, 
		0x0000012f, 
		0x0000001d, 
		0x0000021f, 
		0x0000021d, 
		0x00000095, 
		0x00000087, 
		0x0000021b, 
		0x000000bd, 
		0x000000c1, 
		0x000002f1, 
		0x0000020d, 
		0x000001b7, 
		0x000003f1, 
		0x000001b5, 
		0x00000341, 
		0x000001b1, 
		0x000003f7, 
		0x000001a1, 
		0x000000ab, 
		0x0000027b, 
		0x000000db, 
		0x00000163, 
		0x0000004f, 
		0x00000123, 
		0x000003bd, 
		0x000003a1, 
		0x00000219, 
		0x000003c9, 
		0x00000271, 
		0x000002a3, 
		0x000003b3, 
		0x00000071, 
		0x0000036f, 
		0x000001fb, 
		0x00000341, 
		0x00000121, 
		0x000001cf, 
		0x00000059, 
		0x000003c3, 
		0x00000015, 
		0x000000dd, 
		0x00000075, 
		0x0000013b, 
		0x000001cd, 
		0x0000009b, 
		0x00000365, 
		0x00000377, 
		0x00000371, 
		0x000002cf, 
		0x000002d7, 
		0x000003ef, 
		0x00000329, 
		0x00000339, 
		0x000000f3, 
		0x00000313, 
		0x0000036f, 
		0x000002b9, 
		0x00000335, 
		0x000003a3, 
		0x000002b1, 
		0x00000063, 
		0x000001c5, 
		0x0000021b, 
		0x00000395, 
		0x00000331, 
		0x000000e3, 
		0x00000155, 
		0x00000249, 
		0x00000093, 
		0x00000279, 
		0x00000283, 
		0x000003ed, 
		0x00000135, 
		0x000000d3, 
		0x00000123, 
		0x000001ad, 
		0x0000029d, 
		0x00000283, 
		0x00000167, 
		0x00000221, 
		0x000003a1, 
		0x00000159, 
		0x0000036b, 
		0x00000079, 
		0x00000253, 
		0x0000015b, 
		0x000002df, 
		0x000003e5, 
		0x00000281, 
		0x000000a5, 
		0x000002e3, 
		0x00000105, 
		0x00000245, 
		0x00000135, 
		0x000000d5, 
		0x00000045, 
		0x00000359, 
		0x00000121, 
		0x00000345, 
		0x00000191, 
		0x0000027b, 
		0x000001d7, 
		0x000000d5, 
		0x000003c9, 
		0x0000036f, 
		0x00000335, 
		0x000002c7, 
		0x00000377, 
		0x00000245, 
		0x0000021f, 
		0x000000d5, 
		0x0000005b, 
		0x0000004f, 
		0x00000347, 
		0x00000161, 
		0x00000159, 
		0x000003af, 
		0x0000005d, 
		0x00000277, 
		0x00000247, 
		0x000003e7, 
		0x00000175, 
		0x000002d9, 
		0x000002dd, 
		0x0000008b, 
		0x00000097, 
		0x000001a3, 
		0x0000023f, 
		0x000000c1, 
		0x00000049, 
		0x00000295, 
		0x000003bd, 
		0x0000036d, 
		0x000001a5, 
		0x00000253, 
		0x0000002d, 
		0x000003ad, 
		0x00000201, 
		0x0000008b, 
		0x000002e5, 
		0x00000317, 
		0x00000019, 
		0x00000317, 
		0x00000367, 
		0x000001a9, 
		0x000000cd, 
		0x00000019, 
		0x000003f3, 
		0x00000199, 
		0x000002e3, 
		0x0000032f, 
		0x0000028d, 
		0x000000c1, 
		0x000000d1, 
		0x000000eb, 
		0x000001ab, 
		0x000001c1, 
		0x000001d7, 
		0x00000333, 
		0x00000333, 
		0x000000d9, 
		0x000003bb, 
		0x0000011f, 
		0x00000295, 
		0x000001fb, 
		0x000002b5, 
		0x00000271, 
		0x0000027f, 
		0x00000161, 
		0x0000015b, 
		0x000001d1, 
		0x00000251, 
		0x0000026b, 
		0x0000008f, 
		0x000002d7, 
		0x00000005, 
		0x0000003f, 
		0x000002ff, 
		0x00000169, 
		0x00000383, 
		0x00000039, 
		0x00000109, 
		0x0000017f, 
		0x0000025f, 
		0x0000029b, 
		0x000003d1, 
		0x0000028d, 
		0x000001cf, 
		0x0000019d, 
		0x00000233, 
		0x0000014b, 
		0x0000012f, 
		0x00000201, 
		0x00000101, 
		0x000003b3, 
		0x000001bf, 
		0x00000157, 
		0x000002fd, 
		0x000000cb, 
		0x000002f3, 
		0x0000035b, 
		0x0000021d, 
		0x000001f7, 
		0x000002c7, 
		0x00000181  
};

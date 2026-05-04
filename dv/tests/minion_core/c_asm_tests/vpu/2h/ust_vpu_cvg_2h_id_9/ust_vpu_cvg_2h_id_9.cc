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
			"csrwi mhpmevent3, 8\n"   
			"csrwi mhpmevent4, 28\n"   
			"csrwi mhpmevent5, 26\n"   
			"csrwi mhpmevent6, 21\n"   
			"csrwi mhpmevent7, 3\n"   
			"csrwi mhpmevent8, 6\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f25,  0 (x5)\n"
			"flw.ps f28,  192 (x5)\n"
			"flw.ps f8,  640 (x5)\n"
			"flw.ps f1,  800 (x5)\n"
			"flw.ps f7,  864 (x5)\n"
			"flw.ps f29,  224 (x5)\n"
			"flw.ps f22,  160 (x5)\n"
			"flw.ps f17,  416 (x5)\n"
			"flw.ps f0,  384 (x5)\n"
			"flw.ps f24,  608 (x5)\n"
			"flw.ps f15,  64 (x5)\n"
			"flw.ps f31,  832 (x5)\n"
			"flw.ps f16,  768 (x5)\n"
			"flw.ps f10,  32 (x5)\n"
			"flw.ps f9,  992 (x5)\n"
			"flw.ps f26,  704 (x5)\n"
			"flw.ps f19,  736 (x5)\n"
			"flw.ps f30,  96 (x5)\n"
			"flw.ps f2,  288 (x5)\n"
			"flw.ps f21,  512 (x5)\n"
			"flw.ps f13,  448 (x5)\n"
			"flw.ps f23,  672 (x5)\n"
			"flw.ps f18,  320 (x5)\n"
			"flw.ps f11,  352 (x5)\n"
			"flw.ps f4,  480 (x5)\n"
			"flw.ps f5,  256 (x5)\n"
			"flw.ps f27,  576 (x5)\n"
			"flw.ps f20,  128 (x5)\n"
			"flw.ps f3,  544 (x5)\n"
			"flw.ps f6,  896 (x5)\n"
			"flw.ps f12,  960 (x5)\n"
			"flw.ps f14,  928 (x5)\n"
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
			"li x0, 0x3e8\n" 
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
			"la x5, rand_int_32\n"
			"flw.ps f14, 256(x5)\n"
			"fslli.pi f9, f14, 0xd\n" 
			"la x23, rand_int_32\n"
			"flw.ps f22, 384(x23)\n"
			"flw.ps f12, 960(x23)\n"
			"fmax.pi f8, f22, f12\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x12, rand_ieee754_32\n"
			"flw.ps f22, 544(x12)\n"
			"fclass.ps f6, f22\n" 
			"la x12, rand_int_32\n"
			"flw.ps f15, 448(x12)\n"
			"flw.ps f8, 672(x12)\n"
			"fmaxu.pi f30, f15, f8\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f9, 0(x12)\n"
			"feq.ps f22, f9, f9\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f30, 224(x28)\n"
			"flw.ps f21, 512(x28)\n"
			"fmax.ps f8, f30, f21\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f19, 64(x5)\n"
			"flw.ps f7, 96(x5)\n"
			"fmul.s f31, f19, f7, rne\n" 
			"la x20, rand_int_32\n"
			"flw.ps f9, 864(x20)\n"
			"flw.ps f7, 928(x20)\n"
			"frem.pi f8, f9, f7\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_0:\n"
			"flem.ps m4, f28, f8\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"li x5, 0x9b4d\n"
			"csrw tensor_mask, x5\n"	
			"la x22, rand_ieee754_32\n"
			"flw.ps f22, 32(x22)\n"
			"flw.ps f6, 544(x22)\n"
			"fadd.s f9, f22, f6, rtz\n" 
			"li x22, 0x250078cf7e1a62da\n"
			"fcvt.s.lu f12, x22, rtz\n" 
			"li x22, 0xbae9cc43b1b7c4b6\n"
			"fcvt.s.w f14, x22, rup\n" 
			"li x14, 0x3dbbb2489e9af2a\n"
			"fcvt.s.wu f20, x14, rup\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f21, 608(x25)\n"
			"flw.ps f12, 832(x25)\n"
			"fsub.ps f22, f21, f12, rup\n" 
			"la x25, rand_int_32\n"
			"flw.ps f13, 96(x25)\n"
			"fnot.pi f15, f13\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f8, 256(x14)\n"
			"fclass.ps f31, f8\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ID_302625_HID_0:\n"
			"li x5, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x0 << TENSOR_IMA_B_NUM_COLS) | (0xb << TENSOR_IMA_A_NUM_ROWS) | (0xb << TENSOR_IMA_A_NUM_COLS) | (0xf << TENSOR_IMA_OFFSET) | (0x0 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x14 << TENSOR_IMA_SCP_LOC_B) | (0x3 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x0 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ID_262575_HID_0:\n"
			"li x5, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x0 << TENSOR_FMA32_B_NUM_COLS) | (0x4 << TENSOR_FMA32_A_NUM_ROWS) | (0xf << TENSOR_FMA32_A_NUM_COLS) | (0xe << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x8c << TENSOR_FMA32_SCP_LOC_B) | (0x4 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_0\n"
			"csrw tensor_error, x0\n"
			"li x5, 0xb787b4cbc6a6ab24\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0x1160d922775407ec\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ITER_0_HID_0:\n"
			"li x5, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x1 << TENSOR_FMA16_B_NUM_COLS) | (0x6 << TENSOR_FMA16_A_NUM_ROWS) | (0xe << TENSOR_FMA16_A_NUM_COLS) | (0x1 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0xaf << TENSOR_FMA16_SCP_LOC_B) | (0xfc << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x0 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ITER_1_HID_0:\n"
			"li x6, ((0x12 << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0x8 << TENSOR_QUANT_QUANT_ROW) | (0x3c << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x2 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"la x5, rand_ieee754_32\n"
			"flw.ps f7, 128(x5)\n"
			"flw.ps f22, 928(x5)\n"
			"fmul.s f8, f7, f22, rtz\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x25, rand_int_32\n"
			"flw.ps f15, 608(x25)\n"
			"fandi.pi f30, f15, 0xd9\n" 
			"la x14, rand_int_32\n"
			"flw.ps f19, 320(x14)\n"
			"fsatu8.pi f30, f19\n" 
			"mova.x.m x25\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f28, 160(x28)\n"
			"flw.ps f31, 224(x28)\n"
			"fle.s x23, f28, f31\n" 
			"maskpopc x13, m6\n" 
			"la x13, rand_int_32\n"
			"flw.ps f30, 32(x13)\n"
			"flw.ps f19, 288(x13)\n"
			"flt.pi f20, f30, f19\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f15, 704(x12)\n"
			"fcvt.f16.ps f6, f15\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f21, 672(x19)\n"
			"fclass.s x12, f21\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f21, 448(x13)\n"
			"flw.ps f20, 384(x13)\n"
			"flw.ps f8, 800(x13)\n"
			"fnmsub.ps f8, f21, f20, f8, rmm\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f12, 544(x13)\n"
			"fmul.ps f1, f12, f12, rmm\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f22, 64(x22)\n"
			"fsin.ps f19, f22\n" 
			"la x19, rand_int_32\n"
			"flw.ps f12, 128(x19)\n"
			"fsatu8.pi f20, f12\n" 
			"mova.x.m x13\n" 
			"la x28, rand_int_32\n"
			"flw.ps f14, 0(x28)\n"
			"flw.ps f13, 320(x28)\n"
			"fdiv.pi f6, f14, f13\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f1, 416(x5)\n"
			"fcvt.pwu.ps f22, f1, rtz\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ITER_0_HID_0:\n"
			"li x5, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x3 << TENSOR_IMA_B_NUM_COLS) | (0x6 << TENSOR_IMA_A_NUM_ROWS) | (0xc << TENSOR_IMA_A_NUM_COLS) | (0xa << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x34 << TENSOR_IMA_SCP_LOC_B) | (0x9d << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x0 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x6, ((0xa << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0x6 << TENSOR_QUANT_QUANT_ROW) | (0x3c << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x4 << TENSOR_QUANT_TRANSF6) | (0xa << TENSOR_QUANT_TRANSF5) | (0x8 << TENSOR_QUANT_TRANSF4) | (0x4 << TENSOR_QUANT_TRANSF3) | (0x5 << TENSOR_QUANT_TRANSF2) | (0x5 << TENSOR_QUANT_TRANSF1) | (0xa << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ITER_1_HID_0:\n"
			"li x6, ((0x0 << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0x1 << TENSOR_QUANT_QUANT_ROW) | (0x13 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x2 << TENSOR_QUANT_TRANSF5) | (0x7 << TENSOR_QUANT_TRANSF4) | (0x6 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ITER_2_HID_0:\n"
			"li x5, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x1 << TENSOR_FMA16_B_NUM_COLS) | (0x3 << TENSOR_FMA16_A_NUM_ROWS) | (0xb << TENSOR_FMA16_A_NUM_COLS) | (0xd << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0xdf << TENSOR_FMA16_SCP_LOC_B) | (0x9c << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ITER_3_HID_0:\n"
			"li x6, ((0x4 << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0xa << TENSOR_QUANT_QUANT_ROW) | (0x17 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x0 << TENSOR_QUANT_TRANSF1) | (0x8 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ITER_4_HID_0:\n"
			"li x5, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x1 << TENSOR_IMA_B_NUM_COLS) | (0x2 << TENSOR_IMA_A_NUM_ROWS) | (0x4 << TENSOR_IMA_A_NUM_COLS) | (0x6 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x8b << TENSOR_IMA_SCP_LOC_B) | (0x5a << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x0 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ITER_5_HID_0:\n"
			"li x5, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x3 << TENSOR_IMA_B_NUM_COLS) | (0x1 << TENSOR_IMA_A_NUM_ROWS) | (0x0 << TENSOR_IMA_A_NUM_COLS) | (0xc << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x2e << TENSOR_IMA_SCP_LOC_B) | (0x99 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x0 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x6, ((0xd << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0x1 << TENSOR_QUANT_QUANT_ROW) | (0x28 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x5 << TENSOR_QUANT_TRANSF2) | (0x8 << TENSOR_QUANT_TRANSF1) | (0xa << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ITER_6_HID_0:\n"
			"li x5, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x1 << TENSOR_FMA16_B_NUM_COLS) | (0x8 << TENSOR_FMA16_A_NUM_ROWS) | (0xe << TENSOR_FMA16_A_NUM_COLS) | (0x9 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0xcc << TENSOR_FMA16_SCP_LOC_B) | (0x2 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x6, ((0x1c << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0x8 << TENSOR_QUANT_QUANT_ROW) | (0x2c << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x7 << TENSOR_QUANT_TRANSF4) | (0x6 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ITER_7_HID_0:\n"
			"li x5, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x1 << TENSOR_FMA32_B_NUM_COLS) | (0x9 << TENSOR_FMA32_A_NUM_ROWS) | (0xd << TENSOR_FMA32_A_NUM_COLS) | (0x3 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x24 << TENSOR_FMA32_SCP_LOC_B) | (0x58 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x6, ((0x9 << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0x9 << TENSOR_QUANT_QUANT_ROW) | (0xa << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x7 << TENSOR_QUANT_TRANSF5) | (0x6 << TENSOR_QUANT_TRANSF4) | (0x6 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ITER_8_HID_0:\n"
			"li x5, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x3 << TENSOR_FMA32_B_NUM_COLS) | (0xf << TENSOR_FMA32_A_NUM_ROWS) | (0x1 << TENSOR_FMA32_A_NUM_COLS) | (0xd << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0xe6 << TENSOR_FMA32_SCP_LOC_B) | (0xed << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x0 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x6, ((0x17 << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0xf << TENSOR_QUANT_QUANT_ROW) | (0x6 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x0 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 7\n"
			"csrwi tensor_wait, 10\n"
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
			"LBL_HPM_3_CORE_DCACHE_MISSES0_FAIL_HID_0:\n"          
			"csrr x5, mhpmcounter3\n"
			"sd x5, 32 (x31)\n"
			"LBL_HPM_4_CORE_TQUANT_INST_FAIL_HID_0:\n"          
			"csrr x5, mhpmcounter4\n"
			"sd x5, 40 (x31)\n"
			"LBL_HPM_5_CORE_TFMA_INST_FAIL_HID_0:\n"          
			"csrr x5, mhpmcounter5\n"
			"sd x5, 48 (x31)\n"
			"LBL_HPM_6_CORE_TXFMA_OPS32_FAIL_HID_0:\n"          
			"csrr x5, mhpmcounter6\n"
			"sd x5, 56 (x31)\n"
			"LBL_HPM_7_NEIGH_TX_COOP_TLD_REQ_FAIL_HID_0:\n"          
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
			"LBL_HPM_3_CORE_DCACHE_MISSES0_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter3\n"
			"sd x5, 32 (x31)\n"
			"LBL_HPM_4_CORE_TQUANT_INST_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter4\n"
			"sd x5, 40 (x31)\n"
			"LBL_HPM_5_CORE_TFMA_INST_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter5\n"
			"sd x5, 48 (x31)\n"
			"LBL_HPM_6_CORE_TXFMA_OPS32_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter6\n"
			"sd x5, 56 (x31)\n"
			"LBL_HPM_7_NEIGH_TX_COOP_TLD_REQ_PASS_HID_0:\n"          
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
			"csrwi mhpmevent7, 1\n"   
			"csrwi mhpmevent8, 2\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f29,  960 (x5)\n"
			"flw.ps f31,  736 (x5)\n"
			"flw.ps f16,  544 (x5)\n"
			"flw.ps f18,  32 (x5)\n"
			"flw.ps f4,  672 (x5)\n"
			"flw.ps f3,  160 (x5)\n"
			"flw.ps f13,  288 (x5)\n"
			"flw.ps f27,  832 (x5)\n"
			"flw.ps f21,  384 (x5)\n"
			"flw.ps f7,  928 (x5)\n"
			"flw.ps f17,  768 (x5)\n"
			"flw.ps f19,  448 (x5)\n"
			"flw.ps f2,  480 (x5)\n"
			"flw.ps f5,  352 (x5)\n"
			"flw.ps f8,  416 (x5)\n"
			"flw.ps f6,  992 (x5)\n"
			"flw.ps f10,  704 (x5)\n"
			"flw.ps f15,  896 (x5)\n"
			"flw.ps f14,  576 (x5)\n"
			"flw.ps f11,  192 (x5)\n"
			"flw.ps f26,  64 (x5)\n"
			"flw.ps f24,  224 (x5)\n"
			"flw.ps f12,  608 (x5)\n"
			"flw.ps f28,  512 (x5)\n"
			"flw.ps f0,  96 (x5)\n"
			"flw.ps f20,  640 (x5)\n"
			"flw.ps f1,  864 (x5)\n"
			"flw.ps f30,  256 (x5)\n"
			"flw.ps f22,  128 (x5)\n"
			"flw.ps f23,  320 (x5)\n"
			"flw.ps f25,  800 (x5)\n"
			"flw.ps f9,  0 (x5)\n"
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
			"la x14, rand_int_32\n"
			"flw.ps f17, 768(x14)\n"
			"fpackrepb.pi f24, f17\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f30, 96(x18)\n"
			"flw.ps f14, 608(x18)\n"
			"feq.ps f30, f30, f14\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f14, 160(x12)\n"
			"flw.ps f19, 704(x12)\n"
			"flw.ps f17, 96(x12)\n"
			"fmadd.s f23, f14, f19, f17, rne\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f31, 512(x23)\n"
			"flw.ps f2, 480(x23)\n"
			"feq.ps f11, f31, f2\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f8, 96(x28)\n"
			"fclass.s x27, f8\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f17, 416(x27)\n"
			"flw.ps f8, 96(x27)\n"
			"fdiv.s f23, f17, f8, rmm\n" 
			"maskpopc x18, m2\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_1:\n"
			"mov.m.x m3, x27, 0x41\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x10, rand_int_32\n"
			"flw.ps f18, 768(x10)\n"
			"flw.ps f31, 832(x10)\n"
			"fmul.pi f11, f18, f31\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f15, 96(x6)\n"
			"flw.ps f22, 352(x6)\n"
			"flt.ps f23, f15, f22\n" 
			"LBL_SEQID_0_FP_TRANS_ID_1_HID_1:\n"
			"la x18, rand_ieee754_32\n"
			"flw.ps f24, 640(x18)\n"
			"fexp.ps f8, f24\n" 
			"LBL_SEQID_0_M0_WRITE_ID_2_HID_1:\n"
			"fsetm.pi m4, f18\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x23, rand_ieee754_32\n"
			"flw.ps f15, 832(x23)\n"
			"flw.ps f22, 160(x23)\n"
			"fadd.s f6, f15, f22, rtz\n" 
			"LBL_SEQID_0_M0_WRITE_ID_3_HID_1:\n"
			"maskand m4, m2, m2\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x28, rand_int_32\n"
			"flw.ps f27, 96(x28)\n"
			"fsrli.pi f9, f27, 0x6\n" 
			"LBL_SEQID_0_M0_WRITE_ID_4_HID_1:\n"
			"fltm.ps m6, f11, f23\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0x1690a9e879\n"
			"fsw.ps f0, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"csrw tensor_error, zero\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_1\n"
			"addi x30, x30, 1\n"
			"la x23, rand_int_32\n"
			"flw.ps f9, 160(x23)\n"
			"fcvt.ps.pwu f2, f9, rne\n" 
			"mova.x.m x28\n" 
			"la x6, rand_int_32\n"
			"flw.ps f18, 288(x6)\n"
			"flw.ps f9, 448(x6)\n"
			"flt.pi f18, f18, f9\n" 
			"la x14, rand_int_32\n"
			"flw.ps f27, 64(x14)\n"
			"fsrli.pi f23, f27, 0x7\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f27, 288(x14)\n"
			"flw.ps f18, 96(x14)\n"
			"fsgnj.s f31, f27, f18\n" 
			"la x23, rand_int_32\n"
			"flw.ps f15, 32(x23)\n"
			"flw.ps f8, 128(x23)\n"
			"fsra.pi f14, f15, f8\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f24, 0(x6)\n"
			"flw.ps f15, 416(x6)\n"
			"fadd.s f31, f24, f15, rtz\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f22, 480(x10)\n"
			"flw.ps f30, 960(x10)\n"
			"fmin.s f15, f22, f30\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f6, 128(x27)\n"
			"flw.ps f18, 384(x27)\n"
			"fle.ps f17, f6, f18\n" 
			"la x18, rand_int_32\n"
			"flw.ps f2, 608(x18)\n"
			"fslli.pi f31, f2, 0x7\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f19, 640(x15)\n"
			"fcvt.pw.ps f8, f19, rne\n" 
			"la x15, rand_int_32\n"
			"flw.ps f22, 960(x15)\n"
			"fsrai.pi f17, f22, 0x4\n" 
			"mova.x.m x6\n" 
			"fbci.pi f6, 0x4af26\n" 
			"la x18, rand_int_32\n"
			"flw.ps f18, 288(x18)\n"
			"flw.ps f19, 448(x18)\n"
			"fle.pi f11, f18, f19\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f2, 992(x14)\n"
			"fclass.s x15, f2\n" 
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
		0x8025f571, 
		0xf97e5d85, 
		0x4fe8062a, 
		0x5c12cd34, 
		0xe09476c5, 
		0x3528dd0f, 
		0x6cd1bb8c, 
		0x0b37b373, 
		0xa85b2e71, 
		0x3896ffc1, 
		0x597bfd82, 
		0x8a9bfc2f, 
		0x81fb7d77, 
		0xf4d065fa, 
		0xda4fca96, 
		0x1ded9672, 
		0x758d9600, 
		0xa84ae3de, 
		0xaec06a19, 
		0x174cd20b, 
		0x71f8f4cc, 
		0x07abd515, 
		0x7ce1f5ec, 
		0x621acf3c, 
		0x387b9354, 
		0x218cce29, 
		0xa4898803, 
		0xf1481eeb, 
		0xb029b543, 
		0x55e133b0, 
		0x74a045cb, 
		0xe7b3465a, 
		0x605ea17a, 
		0xe7f07673, 
		0x9fe04f15, 
		0x5aa767d3, 
		0x5bb5dbdd, 
		0x91b84287, 
		0xa09ea89f, 
		0xfcc106f3, 
		0x0e99d0d9, 
		0x2e9c5bb0, 
		0x38fac39d, 
		0xbb66093a, 
		0xd0228462, 
		0x6ec2c7a9, 
		0x8f43f4cf, 
		0x9be5ef3c, 
		0xdb5c0d97, 
		0x975e0325, 
		0x9e623c9f, 
		0xff0c7a41, 
		0xb57d80c4, 
		0x5ef537e7, 
		0x93d2a11c, 
		0xf69984fd, 
		0xda48bc31, 
		0x690902bc, 
		0x837b1c2e, 
		0x7ed9887e, 
		0x93b49f47, 
		0xfa17f8ee, 
		0xdb07e077, 
		0xd11d6700, 
		0x18760c61, 
		0x30af2902, 
		0x4ca9db0d, 
		0x09f7cfe2, 
		0xe59c6b0a, 
		0x16cb3e26, 
		0xfc26b6e0, 
		0x6cc323c0, 
		0x17d6d9ba, 
		0x1c15f473, 
		0x393ca8f2, 
		0x58e267b6, 
		0xe7f5a66b, 
		0xb6ac1b66, 
		0x0a773699, 
		0x012d628b, 
		0x5541931f, 
		0x4b78a443, 
		0xbb3c5717, 
		0x739ca925, 
		0xb0953a65, 
		0x0cf2a714, 
		0x2b122744, 
		0x6bc1c59f, 
		0x482c09b5, 
		0xbb803b38, 
		0xeeb9195d, 
		0x91db2edf, 
		0x12376e51, 
		0x21852349, 
		0x8161acfd, 
		0x4f689df6, 
		0x75ca53f7, 
		0xa7c8cf17, 
		0x4b35a951, 
		0x7661818b, 
		0xb7bfb403, 
		0x0b527e22, 
		0xfa1594ed, 
		0x189da8fe, 
		0x4f78afb8, 
		0x3496a053, 
		0x36f734fb, 
		0xdbed74a2, 
		0xc271d3f4, 
		0x8d6abc56, 
		0x14b6b621, 
		0x03af121f, 
		0x56d70ac8, 
		0x580558c2, 
		0x93761036, 
		0x82e7478d, 
		0x0c85d379, 
		0x256b421b, 
		0x646d49f4, 
		0xcce4d093, 
		0x3b4f7930, 
		0xf3ff820d, 
		0xe2d36b0a, 
		0x4baaa45e, 
		0x7542a5a4, 
		0xd79acdda, 
		0xff99931a, 
		0xe8d4ed96, 
		0xc8f9df60, 
		0x51367eef, 
		0xc636cb1f, 
		0x08351408, 
		0xe065818e, 
		0xc78d1688, 
		0x05afe56c, 
		0x55249abf, 
		0x3ab50d81, 
		0x82682cad, 
		0xe2b7d5c4, 
		0x09e6a8e3, 
		0xcad6c418, 
		0x41b89f8a, 
		0x08259bb3, 
		0x7f989b34, 
		0x78cdf916, 
		0xa25d4712, 
		0xe1f952b2, 
		0x2d4ee5a6, 
		0x13b6183e, 
		0x785258a3, 
		0xd52926b4, 
		0xc85d16a5, 
		0xc3941144, 
		0xa039ea7e, 
		0xba5d07df, 
		0x53e5647c, 
		0x7d56b962, 
		0xaa1bb50d, 
		0xad7de6b6, 
		0x78a66564, 
		0x6c6232a1, 
		0x901680f5, 
		0x2f20349c, 
		0x4df4d45a, 
		0xcad93d74, 
		0x47adfbba, 
		0x8748b380, 
		0x7ee43b1e, 
		0x1f5755f7, 
		0xf8a86092, 
		0x9296639f, 
		0xc1eeeb4e, 
		0xb9e1c50b, 
		0xd89c07a2, 
		0x6bcce9ca, 
		0x8f12395f, 
		0x044469d8, 
		0x47dbd7f0, 
		0x4a1aead8, 
		0x3bd7435d, 
		0x7ed70e8a, 
		0xbd63e39b, 
		0x6d26092f, 
		0xd482cee7, 
		0xf51a418f, 
		0x3775eb51, 
		0x40b2a647, 
		0x61b9027f, 
		0x8964f865, 
		0xb6bd36f5, 
		0x2e751607, 
		0xf0586109, 
		0x1ec33026, 
		0xfec09543, 
		0xcbd890ee, 
		0xd8637712, 
		0xfeb626ca, 
		0x4b6a9e21, 
		0xbed20f4f, 
		0x7c61aede, 
		0xf9a6b31b, 
		0xc867ab4e, 
		0xb9500b6f, 
		0xba8366b6, 
		0xe22d041a, 
		0x9865bf42, 
		0x98b73ed5, 
		0xea8298e7, 
		0xc455a0fa, 
		0x4bd0b657, 
		0x01ea58da, 
		0x96ecc067, 
		0xf1338573, 
		0xdb19128d, 
		0x3c55559f, 
		0x293e55c7, 
		0xe79a09cd, 
		0xe6313043, 
		0xe2d5b161, 
		0xdae88b86, 
		0x9b6528a5, 
		0x6c3f75a6, 
		0x43f08739, 
		0x45be9588, 
		0xf21956e1, 
		0x978bcc98, 
		0x13ae7f65, 
		0x91650862, 
		0x7f0c4dd7, 
		0x8a3b78d7, 
		0xdf08d95e, 
		0x04ba140f, 
		0x09704583, 
		0xbf997a46, 
		0x048b1c3b, 
		0x82f82d23, 
		0xdcfbd47c, 
		0x5f430873, 
		0x66ad472e, 
		0x01f40aa2, 
		0x2ddae449, 
		0x208e5abf, 
		0x6811537f, 
		0xec7466fd, 
		0xacabef88, 
		0xae3db3c4, 
		0x11b1735a, 
		0xb3e1096a, 
		0x995b5de1, 
		0x4f530122, 
		0x356c4fe1, 
		0x537d04c3, 
		0x53734019, 
		0xdeea0a13, 
		0x43939a84, 
		0xbe5fbe0a  
};
volatile uint32_t rand_ieee754_32[1536] __attribute__ ((aligned (2048))) = {
		0xfca40edb, 
		0x070647e1, 
		0xdc0b7e53, 
		0x80000020, 
		0xed053493, 
		0xc2780000, 
		0x0e00000f, 
		0x0e00ffff, 
		0x00000000, 
		0x80800001, 
		0x890eaa84, 
		0x11146e94, 
		0xc0c2105f, 
		0x655f92bc, 
		0x5bb52739, 
		0x42640000, 
		0x00000004, 
		0x0e003fff, 
		0x7fc00001, 
		0xc2680000, 
		0x41e00000, 
		0x0c7f8000, 
		0xc2580000, 
		0xa934574f, 
		0x00400000, 
		0x00000020, 
		0x464a6e09, 
		0x40e00000, 
		0x8125eddc, 
		0x0e00ffff, 
		0xff800000, 
		0xc20c0000, 
		0x0c7ffff0, 
		0x00040000, 
		0xa364f2ce, 
		0xf3b79d0f, 
		0x807652c2, 
		0xbf028f5c, 
		0x7f000000, 
		0x42140000, 
		0x00200000, 
		0xaa4d319c, 
		0x0c400000, 
		0xefdc593f, 
		0x7f800000, 
		0xc25c0000, 
		0x41f00000, 
		0xc1000000, 
		0xc20c0000, 
		0x0c7fffc0, 
		0x40400000, 
		0x0e07ffff, 
		0x80000001, 
		0x00800002, 
		0xc1400000, 
		0x00800001, 
		0xcb1060ae, 
		0x41c00000, 
		0x42440000, 
		0x80000040, 
		0x271811f9, 
		0xf06dc089, 
		0x80000000, 
		0x0c7fffc0, 
		0xd3431be7, 
		0x80000010, 
		0xc1e80000, 
		0x1ef38857, 
		0x80800001, 
		0x80000800, 
		0x0c600000, 
		0xe9876262, 
		0x3c46fd0a, 
		0xd6d5fedf, 
		0x4b8c4b40, 
		0x5061eed1, 
		0x5e098c94, 
		0x41b00000, 
		0x00800002, 
		0xc1000000, 
		0x00000001, 
		0x42040000, 
		0xc1f00000, 
		0x7f7ffffe, 
		0x7fc00000, 
		0xf033da11, 
		0x42600000, 
		0x00002000, 
		0x00400000, 
		0x4b8c4b40, 
		0x42480000, 
		0x7f7ffffe, 
		0x7bb996ed, 
		0x3f800000, 
		0xf471b1ce, 
		0x41a80000, 
		0xc26c0000, 
		0x80ffffff, 
		0x7f800000, 
		0xff000000, 
		0x42000000, 
		0xff800000, 
		0x969f9e97, 
		0xb12c110d, 
		0x539787f0, 
		0x00000020, 
		0xed380e50, 
		0x7f800000, 
		0x734c078f, 
		0x16858340, 
		0x7fffffff, 
		0x41100000, 
		0xc22c0000, 
		0x9854ed27, 
		0x42040000, 
		0x424c0000, 
		0xc24c0000, 
		0xc2000000, 
		0x80000000, 
		0x7fbf4fd5, 
		0x40400000, 
		0x00001000, 
		0x41880000, 
		0x2affafad, 
		0x0e001fff, 
		0x36de58e7, 
		0xf2936f0a, 
		0x00000020, 
		0x7f800000, 
		0xc1a80000, 
		0x0c7fffc0, 
		0x0c7ffffc, 
		0xc2540000, 
		0xffbfffff, 
		0x42100000, 
		0x34a34dca, 
		0xc2240000, 
		0x80000000, 
		0xc1200000, 
		0x00800000, 
		0x806b6288, 
		0xc1c00000, 
		0x0c7ffc00, 
		0xc0c00000, 
		0xc1b00000, 
		0x00800002, 
		0x7fa90cc2, 
		0xc2100000, 
		0xb8bc877c, 
		0x00800001, 
		0x536bd619, 
		0x425c0000, 
		0xf36590aa, 
		0x7f800000, 
		0x80800001, 
		0xcaa906e5, 
		0xc2280000, 
		0xff7fffff, 
		0xc2000000, 
		0x0c7c0000, 
		0xffe10e30, 
		0xc2040000, 
		0x423c0000, 
		0x42600000, 
		0x0e3fffff, 
		0x43838c97, 
		0xdaa076e3, 
		0xf62b6820, 
		0xff89d2c1, 
		0x0e00003f, 
		0xc1000000, 
		0x00800001, 
		0xc2480000, 
		0x4f250be9, 
		0xbd1a8a35, 
		0x41000000, 
		0x42780000, 
		0x00800001, 
		0x80000000, 
		0x17091e39, 
		0xcccccccc, 
		0x0c7fe000, 
		0x0d000ff0, 
		0x326c5368, 
		0x0c7fffff, 
		0xdd87de26, 
		0x26f295d7, 
		0x41980000, 
		0x7f000000, 
		0x28537f61, 
		0x7f800000, 
		0xc1980000, 
		0x41300000, 
		0xff800001, 
		0xc1e80000, 
		0x0c7fffc0, 
		0x00000000, 
		0x3457db32, 
		0x007e85a8, 
		0x00000001, 
		0x42440000, 
		0x00000000, 
		0x0f7fffff, 
		0x80010000, 
		0x00020000, 
		0x7f800000, 
		0x00800002, 
		0x0e1fffff, 
		0xf041b236, 
		0x80800002, 
		0xc0a00000, 
		0xb1cdcb83, 
		0x00000008, 
		0xc2280000, 
		0xf39f9b22, 
		0x4d6cf0af, 
		0xc15b5357, 
		0x00000004, 
		0xe47bd995, 
		0x00011111, 
		0x0c7e0000, 
		0xc0400000, 
		0x0e00007f, 
		0x00800001, 
		0xc2100000, 
		0xff7fffff, 
		0x0c780000, 
		0xde203d07, 
		0xffffffff, 
		0x2ecd22d6, 
		0x0e00003f, 
		0x00ffffff, 
		0x80800000, 
		0xffa6f3a5, 
		0xda4e075c, 
		0x0e0003ff, 
		0xc2040000, 
		0x00800003, 
		0x00800001, 
		0x41d00000, 
		0xc2280000, 
		0x00000001, 
		0x0c7fff80, 
		0x4b000000, 
		0x7f800000, 
		0xff7ffffe, 
		0x41f00000, 
		0x79af1274, 
		0x0e3fffff, 
		0xbf800001, 
		0xc2380000, 
		0x0c2dee55, 
		0x5bb733aa, 
		0x0c400000, 
		0x80000000, 
		0xc2600000, 
		0x42600000, 
		0x005418cc, 
		0x4b8f7239, 
		0xb0a00864, 
		0x3ebaf913, 
		0xc2000000, 
		0x0e001fff, 
		0x0c7fffff, 
		0xffac2a57, 
		0x7f800000, 
		0x1f01fe37, 
		0x42240000, 
		0x41980000, 
		0x89974934, 
		0x7f800001, 
		0xd50a056e, 
		0x00000000, 
		0xff7ffffe, 
		0xe5e5a119, 
		0x0c600000, 
		0x1b559445, 
		0xf45a6e3f, 
		0x806c6505, 
		0xc2580000, 
		0x55555555, 
		0xc1300000, 
		0x80002000, 
		0xc1800000, 
		0x36b2bed4, 
		0x41800000, 
		0x0e0007ff, 
		0x4bcee7c2, 
		0x007fffff, 
		0xc1c00000, 
		0x00000000, 
		0x007fffff, 
		0x382b8e1d, 
		0x0c400000, 
		0xc1200000, 
		0x52a68e9f, 
		0x0c600000, 
		0x6cbfac94, 
		0x0e0001ff, 
		0xff800000, 
		0xc2600000, 
		0x7f9ceb6c, 
		0x0c780000, 
		0x9d9bb22c, 
		0xc25c0000, 
		0x80b8ffa2, 
		0x41880000, 
		0x80000000, 
		0xc2740000, 
		0x00800003, 
		0x007ffffe, 
		0x41c00000, 
		0x7f8113fd, 
		0x00000800, 
		0x42280000, 
		0x80000000, 
		0x7e362b40, 
		0x00000080, 
		0x00238702, 
		0x41a80000, 
		0x95716093, 
		0x41c80000, 
		0x41600000, 
		0x42380000, 
		0x41200000, 
		0x80000000, 
		0x7fc00000, 
		0x0e1fffff, 
		0x9bff37bd, 
		0xc25c0000, 
		0xbf3b8544, 
		0x22e7d37c, 
		0x308b06ad, 
		0xe773bae3, 
		0xec5fe3dd, 
		0x00800003, 
		0x7f7fffff, 
		0x00000002, 
		0x721f7c00, 
		0x4159d9d3, 
		0x24f26c2d, 
		0x7f97f9b2, 
		0x0e00003f, 
		0xffb2d582, 
		0x007ffffe, 
		0x00800003, 
		0xc2040000, 
		0x42380000, 
		0x00000001, 
		0x80000000, 
		0x0d00fff0, 
		0x80000080, 
		0x0c700000, 
		0x7fc00001, 
		0x7f800001, 
		0xc1300000, 
		0x0e00003f, 
		0x0c7fff80, 
		0xa944a3d4, 
		0x420c0000, 
		0xbf028f5c, 
		0xa560caa3, 
		0x00800003, 
		0x3f800000, 
		0x40a3ad85, 
		0xffa001a4, 
		0x868ef0e5, 
		0xb9e77c69, 
		0x0e007fff, 
		0x41400000, 
		0xff8e7c70, 
		0xaaaaaaaa, 
		0x00800003, 
		0x0c7fffc0, 
		0x31b7666f, 
		0x9d2f8d5f, 
		0x00080000, 
		0x807ffffe, 
		0x00000004, 
		0x0f7ffffc, 
		0xc1c80000, 
		0x7f800000, 
		0x6f268a1f, 
		0x728e5b50, 
		0x42780000, 
		0x2d54d913, 
		0x584a5f23, 
		0x7f000000, 
		0x41b00000, 
		0x00000000, 
		0x334302d6, 
		0x1b8ded34, 
		0x80000000, 
		0x34a7b2e4, 
		0x80000000, 
		0x42740000, 
		0x0f7ffffc, 
		0x33333333, 
		0x00000008, 
		0x70414090, 
		0xfe01aaf4, 
		0x41e80000, 
		0xf41fd074, 
		0x80000000, 
		0x80000000, 
		0x42400000, 
		0xf19f1409, 
		0xc1b00000, 
		0x1ec28b78, 
		0x0e0000ff, 
		0xc0c00000, 
		0xc2480000, 
		0x80800000, 
		0x41f80000, 
		0x00000000, 
		0xc26c0000, 
		0xc1c80000, 
		0x4b8c4b40, 
		0xcaa6b391, 
		0x7fc00001, 
		0xc2200000, 
		0xc23c0000, 
		0x42000000, 
		0x41d00000, 
		0xff800000, 
		0x830f0a76, 
		0x80200000, 
		0x42040000, 
		0x41880000, 
		0xc1900000, 
		0x60d2ff6d, 
		0x42600000, 
		0x493a6074, 
		0x8026966d, 
		0x6b126481, 
		0x9a1e8706, 
		0x41200000, 
		0xc2780000, 
		0x0d000ff0, 
		0xa8788ad8, 
		0xc0e00000, 
		0x42200000, 
		0xea8dbe47, 
		0xf7bfd43b, 
		0xfec9eb72, 
		0x0e0007ff, 
		0x42200000, 
		0x80100000, 
		0x42780000, 
		0xbf800000, 
		0xacadd496, 
		0x3f800001, 
		0xc2640000, 
		0xc1800000, 
		0xc2540000, 
		0x526d41fd, 
		0x0e0007ff, 
		0xff9a644b, 
		0xd801d759, 
		0xffbb189a, 
		0x9baf88cd, 
		0x0c7fe000, 
		0xfedec8eb, 
		0x80800001, 
		0x8fa14e6d, 
		0x94f70637, 
		0x0c7e0000, 
		0x0e000001, 
		0x80000020, 
		0x7fc00001, 
		0x0c7fffc0, 
		0xb9d73101, 
		0x0c7fe000, 
		0xc2340000, 
		0xc2080000, 
		0xc2da29d0, 
		0xc2100000, 
		0xfc1575ba, 
		0x80000000, 
		0x0e00001f, 
		0xc1a80000, 
		0x00000001, 
		0x41200000, 
		0x7fffffff, 
		0x42440000, 
		0x00000000, 
		0x361717ec, 
		0xc95cbb76, 
		0x0e7fffff, 
		0x442f2e90, 
		0x80000800, 
		0x80000000, 
		0x80800002, 
		0x80000000, 
		0xc2540000, 
		0x7f800000, 
		0x80000000, 
		0x80000000, 
		0xf99acbf9, 
		0xf290c4cb, 
		0x80020000, 
		0x0c7ffc00, 
		0x8f4ef826, 
		0xcf16c823, 
		0x3c0b4e63, 
		0xff800000, 
		0x17fc4636, 
		0xfd2b28de, 
		0x00000400, 
		0x0c7c0000, 
		0x2e144130, 
		0x7681ed3a, 
		0x00800000, 
		0xe5664889, 
		0x0c7ffff0, 
		0x00800001, 
		0xcb8c4b40, 
		0x41100000, 
		0xc1b00000, 
		0x00080000, 
		0x7f800000, 
		0x00000000, 
		0x80200000, 
		0x6e466773, 
		0x0c7fff80, 
		0x80000000, 
		0x41900000, 
		0x00800001, 
		0xe345fc81, 
		0xc1c00000, 
		0xc2440000, 
		0x8950a37e, 
		0x0c780000, 
		0x6017df4e, 
		0x8f09792a, 
		0xc0a00000, 
		0x849b2d39, 
		0x0e0001ff, 
		0x00000000, 
		0x00000020, 
		0x80000080, 
		0x3d44c525, 
		0x41500000, 
		0x42340000, 
		0xffc00000, 
		0xc1f00000, 
		0x00008000, 
		0x80800001, 
		0xc2180000, 
		0xa678f235, 
		0x0c7e0000, 
		0x55910812, 
		0x0e03ffff, 
		0x80800001, 
		0x41b00000, 
		0x0e1fffff, 
		0x0e07ffff, 
		0x42340000, 
		0x0e07ffff, 
		0xc2780000, 
		0x0326c6c3, 
		0x0c7c0000, 
		0x00800003, 
		0xa8c4607b, 
		0xf37ca8e7, 
		0x80010000, 
		0x80000001, 
		0xffc00000, 
		0x803c3dd1, 
		0x111b7c52, 
		0xc22c0000, 
		0x4b8c4b40, 
		0xaf337e34, 
		0x80004000, 
		0xc2000000, 
		0xc1d80000, 
		0x41400000, 
		0xffc00001, 
		0x60ecc72e, 
		0xff800000, 
		0x8dfd564a, 
		0xf6975f5e, 
		0x41d80000, 
		0x41880000, 
		0x0e0007ff, 
		0x42000000, 
		0xc0c00000, 
		0x41300000, 
		0x3f800001, 
		0x80010000, 
		0x00000000, 
		0x80800003, 
		0x00002000, 
		0x7f000000, 
		0xc1980000, 
		0xc2200000, 
		0x00020000, 
		0x806797b9, 
		0xa80f0115, 
		0x7f800001, 
		0xff800000, 
		0xc24c0000, 
		0xc21c0000, 
		0x0f791616, 
		0x0e000003, 
		0xcd36cadd, 
		0x0d00fff0, 
		0x0e0001ff, 
		0x80800003, 
		0xff7ffffe, 
		0xc1e80000, 
		0xc0c00000, 
		0x2e7e45fe, 
		0x409b3696, 
		0x00800002, 
		0x80318428, 
		0x42280000, 
		0x80000080, 
		0x048e9f46, 
		0xfa47d930, 
		0xc0800000, 
		0x634441cf, 
		0x80000000, 
		0x00000000, 
		0x003ba491, 
		0x00000000, 
		0xab6c69d9, 
		0x00800002, 
		0x4ab1e268, 
		0xc1b80000, 
		0x42480000, 
		0xc2680000, 
		0x00000000, 
		0x00200000, 
		0x00100000, 
		0x615a078c, 
		0xc26c0000, 
		0xc26c0000, 
		0x37d124f0, 
		0xff800000, 
		0x42080000, 
		0x00000400, 
		0xffffffff, 
		0xb833dffd, 
		0x80000080, 
		0x0e0fffff, 
		0x00000040, 
		0xc1b00000, 
		0xaa8f1213, 
		0x80000000, 
		0x41000000, 
		0x423c0000, 
		0xfdf30beb, 
		0xc2200000, 
		0x381fa22b, 
		0x0e0007ff, 
		0x0c7fc000, 
		0x80000400, 
		0x80000000, 
		0x7f800000, 
		0xc2440000, 
		0xeea8f278, 
		0x80000000, 
		0xc1c80000, 
		0xf1fb0958, 
		0x697db4a6, 
		0x434d464c, 
		0xc1c00000, 
		0x00000100, 
		0x80000001, 
		0x42400000, 
		0x80000010, 
		0xd55e874a, 
		0x0e07ffff, 
		0xc2200000, 
		0xc1b80000, 
		0x00000000, 
		0x00800001, 
		0x80040000, 
		0x80000000, 
		0x2d8bd0ab, 
		0x7f975b46, 
		0x41d80000, 
		0x00000000, 
		0x0c7fffc0, 
		0xc1e00000, 
		0x0e00000f, 
		0x00200000, 
		0x8027ab1a, 
		0x00800000, 
		0x6125f803, 
		0x41a80000, 
		0x0e00003f, 
		0xc1f80000, 
		0xff8b58c6, 
		0xff800000, 
		0xc8ed115d, 
		0x268b5993, 
		0x0115474e, 
		0x42580000, 
		0x4b8c4b40, 
		0x80000400, 
		0x54daa448, 
		0xc2180000, 
		0x80000001, 
		0x40800000, 
		0x0c7fffff, 
		0x42080000, 
		0x0e01ffff, 
		0x0f7ffffc, 
		0xca6b544a, 
		0x7f9820fc, 
		0x005b7930, 
		0x7f7ffffe, 
		0x0c7f8000, 
		0x80800000, 
		0xffc00000, 
		0x0ee34b90, 
		0x0c7e0000, 
		0x80000000, 
		0xc1400000, 
		0x80000800, 
		0x67312f4e, 
		0x42240000, 
		0x76472434, 
		0x8555e371, 
		0xc1980000, 
		0x40c00000, 
		0xde954402, 
		0xc24c0000, 
		0xe2f81103, 
		0x41d00000, 
		0xc1600000, 
		0xaf5ffd1b, 
		0x0f7ffffe, 
		0x40800000, 
		0x0e01ffff, 
		0xc2640000, 
		0x42680000, 
		0x186e1fe5, 
		0x426c0000, 
		0xc2480000, 
		0xffbfffff, 
		0x937ab712, 
		0x80800000, 
		0x0f7ffffe, 
		0xa20a7032, 
		0xc1900000, 
		0x80000000, 
		0x6c8384f0, 
		0x0c7fffc0, 
		0x80000000, 
		0xb0d9464c, 
		0x00800003, 
		0xc2200000, 
		0x7fbfffff, 
		0x80000800, 
		0x3c35cdd2, 
		0xc2140000, 
		0x923669ce, 
		0x7f800000, 
		0xaefa0f87, 
		0x4b8c4b40, 
		0x41f80000, 
		0x0e001fff, 
		0x00000001, 
		0x41b00000, 
		0xffffffff, 
		0x00000000, 
		0x00000008, 
		0x0e0fffff, 
		0x80001000, 
		0x80800000, 
		0x00800001, 
		0x422c0000, 
		0xc0875d59, 
		0x41100000, 
		0xc1900000, 
		0xbf028f5c, 
		0x7f7ffffe, 
		0x6bc23101, 
		0xe74dc83d, 
		0xc97e7dfd, 
		0x41a00000, 
		0x7f7fffff, 
		0x60969c75, 
		0xbd23e39d, 
		0xc1e80000, 
		0xa8ac2962, 
		0xff9e0721, 
		0x40400000, 
		0x80040000, 
		0x55958fb3, 
		0x41a80000, 
		0xa8659055, 
		0xc0c00000, 
		0x007ffffe, 
		0xca89e022, 
		0x76428bab, 
		0x4754d81d, 
		0x55555555, 
		0xc2240000, 
		0x0e00000f, 
		0x07c0587f, 
		0x00000000, 
		0xffc00000, 
		0x41e80000, 
		0x00004000, 
		0xc2340000, 
		0x64881d83, 
		0x41a80000, 
		0xffffffff, 
		0x42040000, 
		0x33333333, 
		0x8f7fffff, 
		0xc2780000, 
		0xf04ec427, 
		0xc0000000, 
		0x00800001, 
		0xf4e6fd86, 
		0x3f800001, 
		0xe2df71e7, 
		0xc0e00000, 
		0x3c6f623b, 
		0x0c7c0000, 
		0x15bbc559, 
		0xc1d80000, 
		0xf60a6d42, 
		0x0e00001f, 
		0x80000002, 
		0xff7ffffe, 
		0xc0800000, 
		0x78353e07, 
		0x7f7fffff, 
		0xe5fd5c0f, 
		0x7fc00001, 
		0xc1900000, 
		0x00000100, 
		0x42280000, 
		0x00000200, 
		0xc0c00000, 
		0x00800002, 
		0x67548a12, 
		0x00000000, 
		0x7f000000, 
		0x98ed0da7, 
		0xff800000, 
		0x2e031f10, 
		0xbd589c1c, 
		0xd3cc322a, 
		0x80000000, 
		0x42240000, 
		0x00800001, 
		0x7f800001, 
		0x807fffff, 
		0xc2040000, 
		0x1acac5f4, 
		0x7f800000, 
		0x80000000, 
		0x7f7ffffe, 
		0x00000000, 
		0x00002000, 
		0x0e01ffff, 
		0x42140000, 
		0x0c7fffc0, 
		0xbf028f5c, 
		0xdf48ee2a, 
		0x0d00fff0, 
		0x80001000, 
		0xff7ffffe, 
		0xaf516c44, 
		0xbf9a5a18, 
		0x00000400, 
		0x00010000, 
		0x41600000, 
		0xc24c0000, 
		0x00000008, 
		0x7f800000, 
		0xff800000, 
		0x981a8734, 
		0x00400000, 
		0x80000000, 
		0xff800001, 
		0xff800000, 
		0xc2200000, 
		0x00000000, 
		0x426c0000, 
		0x0c7ffff0, 
		0x42600000, 
		0xb542a1e1, 
		0x80000040, 
		0x00800003, 
		0x80000008, 
		0x6dedeefa, 
		0x41000000, 
		0xcb8c4b40, 
		0x0af90d23, 
		0x00000000, 
		0xe2d5b729, 
		0xf43be75e, 
		0x0e00ffff, 
		0xe2472ac1, 
		0x80000001, 
		0x67784815, 
		0x7fc00001, 
		0x80000000, 
		0x2836038a, 
		0x80000200, 
		0x7f800000, 
		0xc1b00000, 
		0x9481581c, 
		0xc2280000, 
		0xc1900000, 
		0x3f238bb4, 
		0xc2080000, 
		0x4b000000, 
		0x423c0000, 
		0x41000000, 
		0xff7ffffe, 
		0xff800000, 
		0x9b6b51a2, 
		0x42680000, 
		0x80000000, 
		0x045a1a8f, 
		0x80000000, 
		0x42416016, 
		0x00000008, 
		0x4b000000, 
		0xa90e1414, 
		0x31976128, 
		0x0c780000, 
		0x00020000, 
		0x00400000, 
		0x03a5d8ea, 
		0xff864b15, 
		0x8f7fffff, 
		0x00000000, 
		0x80100000, 
		0x0f7ffffd, 
		0x7fd840df, 
		0xc1980000, 
		0x00004000, 
		0x80000000, 
		0x41200000, 
		0x4b000000, 
		0x42000000, 
		0x00400000, 
		0x41a80000, 
		0x425c0000, 
		0x0e01ffff, 
		0x7f90bd47, 
		0xba7fb94f, 
		0xccc93e01, 
		0x7c0974a9, 
		0x0e03ffff, 
		0xbde6f617, 
		0xc2780000, 
		0x80000800, 
		0x807ffffe, 
		0xc2280000, 
		0x8f7ffffc, 
		0x00800003, 
		0x93fd2e16, 
		0x00000000, 
		0x48407faa, 
		0x0c7f8000, 
		0xb8e954dd, 
		0x80000000, 
		0x41300000, 
		0x283952a6, 
		0x0c7fffc0, 
		0xb0d354ea, 
		0x9c855baa, 
		0x00000080, 
		0xa1b02b20, 
		0xc2440000, 
		0xeef900e9, 
		0x0e1fffff, 
		0x80000400, 
		0x6006ee87, 
		0x00000040, 
		0x0e00003f, 
		0xff7fffff, 
		0x42300000, 
		0xff7ffffe, 
		0xb573c41f, 
		0x00800000, 
		0x80004000, 
		0xc27c0000, 
		0x80800001, 
		0x8373e0ce, 
		0x0c780000, 
		0xc1b00000, 
		0xc24c0000, 
		0xe9d7d180, 
		0x00800000, 
		0xc2000000, 
		0xffb314da, 
		0xff800000, 
		0x42500000, 
		0x010a6da5, 
		0x80000000, 
		0xc2100000, 
		0xc2380000, 
		0x1e4219df, 
		0xff800001, 
		0x0c7ff800, 
		0x80000000, 
		0x081b57db, 
		0x0f7ffffd, 
		0x80603d65, 
		0x717291c8, 
		0x0e0001ff, 
		0x0e0001ff, 
		0x41200000, 
		0x80000000, 
		0xc1a00000, 
		0x435a5526, 
		0x0e01ffff, 
		0xc0a00000, 
		0x294123d5, 
		0x80800003, 
		0xd4086228, 
		0x4724051c, 
		0xcccccccc, 
		0x4b000000, 
		0x0e0000ff, 
		0x1ac95323, 
		0x41900000, 
		0xc1d00000, 
		0x7fa8b50b, 
		0xc1100000, 
		0xa6d56c69, 
		0x0c7fe000, 
		0xc1e80000, 
		0x2449a3fa, 
		0x0c7ffff8, 
		0x7fc00000, 
		0xc2080000, 
		0x9f2e0a1c, 
		0xff800000, 
		0x41300000, 
		0x0e000fff, 
		0xc0000000, 
		0x2ccd6631, 
		0x18eb6308, 
		0x0c7ffc00, 
		0x00000008, 
		0xff800000, 
		0x00000200, 
		0x00001000, 
		0x0c7ffff0, 
		0x207e6740, 
		0xc1b00000, 
		0x7f7fffff, 
		0xd467b4ba, 
		0x00800001, 
		0xc2680000, 
		0xc0c00000, 
		0x7f7fffff, 
		0x1a0b860f, 
		0x7f800000, 
		0x42780000, 
		0x80100000, 
		0x7f7ffffe, 
		0x1220e71b, 
		0x0e001fff, 
		0x80000000, 
		0xc1900000, 
		0x3dbdfce9, 
		0xb157283d, 
		0x41e00000, 
		0x80000000, 
		0x007fffff, 
		0xc1200000, 
		0x7fc00000, 
		0x00800000, 
		0x42600000, 
		0xd81f1d1e, 
		0x7f7ffffe, 
		0x615a9ed3, 
		0x6aaf2cc5, 
		0x8f7fffff, 
		0xc25c0000, 
		0x7f800000, 
		0xc2180000, 
		0xc1d80000, 
		0xc9f4153a, 
		0x7f800000, 
		0x7fc00001, 
		0x7f7ffffe, 
		0x0e00000f, 
		0xc2700000, 
		0x00000000, 
		0x94fc94b6, 
		0x0c7fffff, 
		0xc2480000, 
		0x80001000, 
		0xaff413aa, 
		0x803fadca, 
		0x0e000007, 
		0xff800000, 
		0xe7ca440b, 
		0x70efad2e, 
		0xc1900000, 
		0x1662bcb8, 
		0x40000000, 
		0x0d5efb45, 
		0x80002000, 
		0x00400000, 
		0x7373aea5, 
		0xc1f80000, 
		0x8f7fffff, 
		0x0e0003ff, 
		0x0e000007, 
		0x80000000, 
		0x00000000, 
		0x41f80000, 
		0x0000278e, 
		0x00800000, 
		0x001d72b8, 
		0x7fb5667b, 
		0x7f7ffffe, 
		0x0bbed915, 
		0x00000000, 
		0xc2700000, 
		0xf6d7a70e, 
		0x006681fc, 
		0x80000000, 
		0x0c780000, 
		0x0e00000f, 
		0x98bd1288, 
		0xbf5d5c77, 
		0x80800001, 
		0x11448234, 
		0xcdfeaae3, 
		0xc0000000, 
		0x7fffffff, 
		0x41700000, 
		0x00020000, 
		0xff800000, 
		0x42580000, 
		0xcdb0b4ce, 
		0xbf926076, 
		0x4685bc8e, 
		0x80000010, 
		0x0e03ffff, 
		0x5b8a836e, 
		0x41e00000, 
		0xff7ffffe, 
		0x80000002, 
		0x0022144a, 
		0x1f2f0440, 
		0x3153bb2b, 
		0x00000000, 
		0x41200000, 
		0x7f800000, 
		0x7f7ffffe, 
		0xc1100000, 
		0x00000000, 
		0x00000040, 
		0x427c0000, 
		0x8582c5cb, 
		0x00000000, 
		0x8095d572, 
		0x7fafd8ae, 
		0x0c7ffffc, 
		0x80800001, 
		0x7fbadf92, 
		0x87c761df, 
		0x2b7d0435, 
		0x00800001, 
		0x00800003, 
		0x00000000, 
		0x80800003, 
		0x42180000, 
		0x7f7ffffe, 
		0xfc0eaeb4, 
		0x8008db44, 
		0x7f7fffff, 
		0x0f7fffff, 
		0xcb000000, 
		0xc0c00000, 
		0x422c0000, 
		0xe505d7d4, 
		0xc1f80000, 
		0x00400000, 
		0x80800001, 
		0x30745cd9, 
		0x15d64ba8, 
		0x061806ad, 
		0x0e00001f, 
		0x40c00000, 
		0x80000000, 
		0x00400000, 
		0x00800002, 
		0x40800000, 
		0x7db6439e, 
		0x00000000, 
		0x00800001, 
		0x41200000, 
		0x00800003, 
		0x41600000, 
		0xc1300000, 
		0x00000000, 
		0x0e112b41, 
		0x00400000, 
		0x2d2c7d04, 
		0xc26c0000, 
		0x80000000, 
		0x00000200, 
		0x80800000, 
		0xc2280000, 
		0xf38561dd, 
		0xff800000, 
		0x80000020, 
		0x0e003fff, 
		0x5b065dda, 
		0x33333333, 
		0x00000000, 
		0xe9b1352b, 
		0x94e1bad9, 
		0x80000080, 
		0x423c0000, 
		0x56d6f238, 
		0x0e0fffff, 
		0x42040000, 
		0xff800000, 
		0x41700000, 
		0x00000004, 
		0xff800000, 
		0x80fbd691, 
		0x0c7ff800, 
		0x7f7fffff, 
		0xdf575b83, 
		0xd2012c29, 
		0xc2440000, 
		0x41f00000, 
		0x7fc00000, 
		0x42000000, 
		0x80800002, 
		0x7869777d, 
		0x7e404fee, 
		0x80000001, 
		0x2326bfbc, 
		0xff800000, 
		0x00000002, 
		0xc0a00000, 
		0x80000001, 
		0x7fbe4c24, 
		0xc1c80000, 
		0xc2280000, 
		0x00000000, 
		0x42300000, 
		0x41a00000, 
		0x40e24108, 
		0xe76dbccb, 
		0xffaf5c50, 
		0x2db6544a, 
		0x40e00000, 
		0x00000000, 
		0x0e00000f, 
		0x2ace51f1, 
		0x0c7ffff8, 
		0x80000001, 
		0xc2600000, 
		0x80000000, 
		0x80000000, 
		0x80000002, 
		0xc2400000, 
		0x0e000fff, 
		0xc1200000, 
		0x423c0000, 
		0x03dde9fe, 
		0x80002000, 
		0x41800000, 
		0x41b80000, 
		0x97658431, 
		0x40e00000, 
		0xffc00000, 
		0xbf800000, 
		0x42640000, 
		0x80000000, 
		0xeecb80e3, 
		0xff800000, 
		0x00000010, 
		0x0e00001f, 
		0xbdf5aa9a, 
		0xc9d53428, 
		0x7f800000, 
		0xc1d00000, 
		0x80000000, 
		0x41c80000, 
		0x41c80000, 
		0x7f9a9ed6, 
		0x80000000, 
		0xbd514d57, 
		0xc21c0000, 
		0xc2680000, 
		0x0f03c34a, 
		0x4b8c4b40, 
		0x80000000, 
		0x04fae7b8, 
		0x42000000, 
		0x961ec7a4, 
		0xc1880000, 
		0x00200000, 
		0xff7fffff, 
		0x80040000, 
		0xc2080000, 
		0x0e0fffff, 
		0xe4a9ae10, 
		0x04e702d1, 
		0xfe758793, 
		0x76315f87, 
		0x1e8145e1, 
		0xa8a06845, 
		0x00000002, 
		0xc1700000, 
		0x7f7fffff, 
		0xc1600000, 
		0x00000002, 
		0x0e3fffff, 
		0x41b00000, 
		0x0f7ffffd, 
		0x00000002, 
		0x41300000, 
		0x4875482b, 
		0x7fc00000, 
		0x7f800000, 
		0xc24c0000, 
		0xffc00000, 
		0xe57a52e4, 
		0x00000000, 
		0x3f028f5c, 
		0xcccccccc, 
		0x0c7ffc00, 
		0x26975ea4, 
		0x02b50275, 
		0xc1b80000, 
		0xb958dc0c, 
		0x42000000, 
		0x0e3fffff, 
		0x0e0007ff, 
		0xfd2d0844, 
		0xfef5ece8, 
		0x00011111, 
		0xff800000, 
		0x2d0d77d2, 
		0x0e0001ff, 
		0xc2280000, 
		0x42700000, 
		0x41600000, 
		0xc2180000, 
		0x0e000003, 
		0x42540000, 
		0xff800000, 
		0x7f800000, 
		0xc2380000, 
		0x0e00ffff, 
		0x00000080, 
		0xff7ffffe, 
		0xc2480000, 
		0xc64439ed, 
		0x42340000, 
		0x0e03ffff, 
		0x279b11a0, 
		0x774aba6e, 
		0xff7fffff, 
		0x80200000, 
		0xa28abb68, 
		0xc2200000, 
		0x41400000, 
		0x00040000, 
		0x41e80000, 
		0x41100000, 
		0xffc00001, 
		0x41d00000, 
		0xff800000, 
		0x80010000, 
		0xc1600000, 
		0x0c7f0000, 
		0xffbd2069, 
		0xff800000, 
		0x426c0000, 
		0xb6302000, 
		0xc26c0000, 
		0x42100000, 
		0x80000000, 
		0x1bae8c25, 
		0x0f7ffffc, 
		0xff7fffff, 
		0x7fbfffff, 
		0x0ba4a437, 
		0xba8d5cd2, 
		0xc0000000, 
		0x00000000, 
		0x0c7fffc0, 
		0x00000000, 
		0x006c2ea1, 
		0xc1400000, 
		0xc21c0000, 
		0x42280000, 
		0x41880000, 
		0x41400000, 
		0x0e00007f, 
		0x0c7fffff, 
		0x80040000, 
		0x87bd05bf, 
		0x40400000, 
		0x00000001, 
		0xc1200000, 
		0x00000000, 
		0xff851f5a, 
		0x427c0000, 
		0xa214918b, 
		0xc1a80000, 
		0x3a785fdf, 
		0x42600000, 
		0x8f7ffffe, 
		0x42400000, 
		0x41a80000, 
		0x4734bc03, 
		0x80000020, 
		0xff7ffffe, 
		0xc2000000, 
		0xc1800000, 
		0x202e13e1, 
		0xc2080000, 
		0x42380000, 
		0x00000000, 
		0x0c7f8000, 
		0xc0800000, 
		0x41559074, 
		0x42480000, 
		0xe7fdefa9, 
		0x42040000, 
		0x0e7fffff, 
		0x7f800000, 
		0x80000800, 
		0xc24c0000, 
		0x500db39c, 
		0x80000000, 
		0x42400000, 
		0x80000200, 
		0x80000000, 
		0x007fffff, 
		0x7fa21e5f, 
		0xc2300000, 
		0x00800002, 
		0x0c7fffff, 
		0x00000000, 
		0x00001000, 
		0x96e19159, 
		0x807bb27a, 
		0x80000100, 
		0x16d47d90, 
		0x00000100, 
		0x7fe9448b, 
		0xcccccccc, 
		0x00001000, 
		0x80000020, 
		0x00800001, 
		0x7f7ffffe, 
		0x41b00000, 
		0x487059c4, 
		0x80000000, 
		0x4345809f, 
		0x42480000, 
		0xbf800000, 
		0xc1200000, 
		0x3f800000, 
		0x0c7ff800, 
		0x81c49044, 
		0x3a32899e, 
		0x0e001fff, 
		0xc1a00000, 
		0x00000000, 
		0xc1f00000, 
		0xe0497fc6, 
		0xc20c0000, 
		0xbe4728c5, 
		0x00000000, 
		0x00000400, 
		0xafca11d1, 
		0x0e000fff, 
		0xe0fb740d, 
		0x80000200, 
		0x7f800000, 
		0x425c0000, 
		0xd65abc18, 
		0x41800000, 
		0xffaf0301, 
		0x7f800000, 
		0x3fd08005, 
		0x9f404076, 
		0xc1500000, 
		0x0d00fff0, 
		0x0e0fffff, 
		0x80800000, 
		0xc2680000, 
		0x80800001, 
		0xffbfffff, 
		0x0e1fffff, 
		0xff7ffffe, 
		0x7f7ffffe, 
		0x9953a7d4, 
		0x00000020, 
		0xf2ca4d08, 
		0xcd4ca2f4, 
		0x8db985ff, 
		0xc23c0000, 
		0x454c69e2, 
		0xc1300000, 
		0x2ee6c63f, 
		0x002a0405, 
		0xc83d78ae, 
		0x00000000, 
		0x6df75237, 
		0x420c0000, 
		0xbc518d16, 
		0x00000040, 
		0x80040000, 
		0x7fa3b2d6, 
		0xc2540000, 
		0xffc88c94, 
		0x00000001, 
		0x259ea977, 
		0x0d82256d, 
		0xfb7de988, 
		0xc1200000, 
		0x86f60d78, 
		0x0e0fffff, 
		0x421c0000, 
		0x7f800000, 
		0x0c7fc000  
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
		0x00000297, 
		0x000000ed, 
		0x00000035, 
		0x000003eb, 
		0x0000007b, 
		0x0000002b, 
		0x00000305, 
		0x00000147, 
		0x0000018d, 
		0x0000002d, 
		0x000003b9, 
		0x00000143, 
		0x0000010d, 
		0x00000307, 
		0x000002a9, 
		0x0000038b, 
		0x000000cb, 
		0x00000175, 
		0x00000081, 
		0x0000018b, 
		0x0000015b, 
		0x0000002f, 
		0x000000fd, 
		0x0000030f, 
		0x00000085, 
		0x0000034d, 
		0x00000389, 
		0x0000035b, 
		0x00000007, 
		0x00000305, 
		0x0000017d, 
		0x000000ed, 
		0x0000035d, 
		0x00000105, 
		0x000000dd, 
		0x000000ad, 
		0x000002a5, 
		0x000001dd, 
		0x000000d1, 
		0x00000343, 
		0x000001ff, 
		0x0000025b, 
		0x000003cd, 
		0x00000303, 
		0x0000032f, 
		0x000003f5, 
		0x000002e9, 
		0x000001e1, 
		0x00000107, 
		0x00000261, 
		0x00000305, 
		0x0000023f, 
		0x000000e9, 
		0x00000393, 
		0x000000a1, 
		0x00000289, 
		0x0000021f, 
		0x00000125, 
		0x00000301, 
		0x000003ff, 
		0x000000af, 
		0x000003d1, 
		0x000003d7, 
		0x0000036f, 
		0x000002c9, 
		0x000001a1, 
		0x0000031d, 
		0x000000f3, 
		0x00000279, 
		0x00000343, 
		0x000002db, 
		0x0000011d, 
		0x00000201, 
		0x000003c3, 
		0x000000a9, 
		0x000001fb, 
		0x000000b1, 
		0x000003c5, 
		0x0000000d, 
		0x00000235, 
		0x000001ff, 
		0x000003fd, 
		0x000003c1, 
		0x000003f5, 
		0x00000097, 
		0x0000038b, 
		0x000000fb, 
		0x00000239, 
		0x000003af, 
		0x00000129, 
		0x000002df, 
		0x000002b5, 
		0x00000333, 
		0x000002e3, 
		0x000002cf, 
		0x00000285, 
		0x00000039, 
		0x00000337, 
		0x00000123, 
		0x000000a3, 
		0x00000025, 
		0x00000109, 
		0x0000002b, 
		0x00000235, 
		0x000002d9, 
		0x000002c5, 
		0x000002b1, 
		0x00000195, 
		0x0000004f, 
		0x0000036f, 
		0x00000157, 
		0x0000039b, 
		0x0000037d, 
		0x00000157, 
		0x000001db, 
		0x000003cd, 
		0x00000095, 
		0x000001f3, 
		0x000001d3, 
		0x000001af, 
		0x000001e5, 
		0x000003a3, 
		0x00000329, 
		0x00000089, 
		0x000001e9, 
		0x00000053, 
		0x0000013b, 
		0x00000139, 
		0x00000387, 
		0x000001cb, 
		0x00000287, 
		0x000000f1, 
		0x000000cf, 
		0x0000036b, 
		0x00000069, 
		0x00000349, 
		0x000002c1, 
		0x0000020b, 
		0x0000034d, 
		0x000003e3, 
		0x00000333, 
		0x000000e5, 
		0x000000f7, 
		0x000003bf, 
		0x000000fd, 
		0x000001a5, 
		0x00000347, 
		0x00000033, 
		0x00000275, 
		0x00000211, 
		0x0000002f, 
		0x00000173, 
		0x0000019f, 
		0x000002af, 
		0x0000035b, 
		0x00000079, 
		0x00000297, 
		0x00000103, 
		0x0000005f, 
		0x00000267, 
		0x00000183, 
		0x000001c1, 
		0x00000389, 
		0x000003cd, 
		0x000003e1, 
		0x00000249, 
		0x000001cd, 
		0x000001d3, 
		0x00000231, 
		0x00000171, 
		0x0000008d, 
		0x000000c1, 
		0x00000097, 
		0x000003e1, 
		0x0000018f, 
		0x000003ab, 
		0x0000010d, 
		0x000002e9, 
		0x000001e5, 
		0x000002b5, 
		0x000000d7, 
		0x0000038b, 
		0x000002a1, 
		0x000000d3, 
		0x000003d3, 
		0x00000043, 
		0x0000030b, 
		0x00000343, 
		0x0000005b, 
		0x000001b9, 
		0x00000315, 
		0x000002b3, 
		0x0000012d, 
		0x000001ed, 
		0x000002d9, 
		0x0000037d, 
		0x0000030f, 
		0x0000001f, 
		0x000003df, 
		0x0000001f, 
		0x0000012f, 
		0x00000009, 
		0x0000001f, 
		0x00000075, 
		0x000002a7, 
		0x0000030d, 
		0x0000007b, 
		0x00000295, 
		0x00000315, 
		0x000000d3, 
		0x000000e7, 
		0x000003c9, 
		0x00000279, 
		0x00000105, 
		0x0000002f, 
		0x00000313, 
		0x00000039, 
		0x00000289, 
		0x000001b7, 
		0x00000201, 
		0x00000001, 
		0x00000319, 
		0x00000121, 
		0x000000a5, 
		0x00000299, 
		0x00000143, 
		0x000003af, 
		0x00000361, 
		0x00000371, 
		0x0000012b, 
		0x00000029, 
		0x000001ab, 
		0x000001a3, 
		0x00000131, 
		0x00000075, 
		0x00000043, 
		0x00000147, 
		0x0000032b, 
		0x00000217, 
		0x000003a3, 
		0x0000035f, 
		0x00000295, 
		0x00000283, 
		0x000003e3, 
		0x000003e5, 
		0x00000235, 
		0x0000019b, 
		0x00000293, 
		0x00000311, 
		0x00000041, 
		0x00000287, 
		0x00000393, 
		0x000001f1, 
		0x000002b5, 
		0x00000351, 
		0x0000030f  
};

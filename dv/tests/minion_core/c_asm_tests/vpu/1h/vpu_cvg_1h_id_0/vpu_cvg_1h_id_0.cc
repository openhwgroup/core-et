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
	if ( hid == 0 ) { 
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
			"csrwi mhpmevent4, 17\n"   
			"csrwi mhpmevent5, 26\n"   
			"csrwi mhpmevent6, 19\n"   
			"csrwi mhpmevent7, 6\n"   
			"csrwi mhpmevent8, 4\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f9,  352 (x5)\n"
			"flw.ps f8,  864 (x5)\n"
			"flw.ps f16,  0 (x5)\n"
			"flw.ps f19,  736 (x5)\n"
			"flw.ps f18,  896 (x5)\n"
			"flw.ps f23,  960 (x5)\n"
			"flw.ps f15,  608 (x5)\n"
			"flw.ps f12,  384 (x5)\n"
			"flw.ps f5,  768 (x5)\n"
			"flw.ps f0,  832 (x5)\n"
			"flw.ps f3,  192 (x5)\n"
			"flw.ps f2,  320 (x5)\n"
			"flw.ps f28,  928 (x5)\n"
			"flw.ps f22,  480 (x5)\n"
			"flw.ps f7,  128 (x5)\n"
			"flw.ps f13,  64 (x5)\n"
			"flw.ps f26,  224 (x5)\n"
			"flw.ps f31,  704 (x5)\n"
			"flw.ps f1,  992 (x5)\n"
			"flw.ps f14,  416 (x5)\n"
			"flw.ps f20,  576 (x5)\n"
			"flw.ps f24,  800 (x5)\n"
			"flw.ps f17,  448 (x5)\n"
			"flw.ps f4,  640 (x5)\n"
			"flw.ps f11,  544 (x5)\n"
			"flw.ps f10,  512 (x5)\n"
			"flw.ps f25,  256 (x5)\n"
			"flw.ps f21,  96 (x5)\n"
			"flw.ps f29,  32 (x5)\n"
			"flw.ps f27,  672 (x5)\n"
			"flw.ps f30,  160 (x5)\n"
			"flw.ps f6,  288 (x5)\n"
			"LBL_VADDR_INIT_HID_0:\n"          
			"li x13, 0x81004807e0 \n"                 
			"fsw.ps f15, (x13) \n"                 
			"li x21, 0x8100aa01c0 \n"                 
			"fsw.ps f6, (x21) \n"                 
			"li x12, 0x810108cfe0 \n"                 
			"fsw.ps f27, (x12) \n"                 
			"li x25, 0x8101980000 \n"                 
			"fsw.ps f20, (x25) \n"                 
			"li x19, 0x8102dd6000 \n"                 
			"fsw.ps f3, (x19) \n"                 
			"li x11, 0x8104122240 \n"                 
			"fsw.ps f27, (x11) \n"                 
			"li x20, 0x81053ae800 \n"                 
			"fsw.ps f19, (x20) \n"                 
			"li x14, 0x8105440020 \n"                 
			"fsw.ps f28, (x14) \n"                 
			"li x11, 0x81056ef000 \n"                 
			"fsw.ps f10, (x11) \n"                 
			"li x16, 0x8106a86820 \n"                 
			"fsw.ps f1, (x16) \n"                 
			"li x16, 0x810a7a6020 \n"                 
			"fsw.ps f5, (x16) \n"                 
			"li x12, 0x810e392060 \n"                 
			"fsw.ps f10, (x12) \n"                 
			"li x15, 0x810faa07e0 \n"                 
			"fsw.ps f12, (x15) \n"                 
			"li x21, 0x8200a1a080 \n"                 
			"fsw.ps f19, (x21) \n"                 
			"li x12, 0x8201060800 \n"                 
			"fsw.ps f24, (x12) \n"                 
			"li x21, 0x82022e0020 \n"                 
			"fsw.ps f15, (x21) \n"                 
			"li x14, 0x82057a0000 \n"                 
			"fsw.ps f12, (x14) \n"                 
			"li x16, 0x82067a0400 \n"                 
			"fsw.ps f1, (x16) \n"                 
			"li x13, 0x8208a3f000 \n"                 
			"fsw.ps f29, (x13) \n"                 
			"li x11, 0x8208a45aa0 \n"                 
			"fsw.ps f15, (x11) \n"                 
			"li x13, 0x8208e60000 \n"                 
			"fsw.ps f6, (x13) \n"                 
			"li x19, 0x8209b80020 \n"                 
			"fsw.ps f26, (x19) \n"                 
			"li x19, 0x8209ec77c0 \n"                 
			"fsw.ps f11, (x19) \n"                 
			"li x10, 0x820a6e0260 \n"                 
			"fsw.ps f3, (x10) \n"                 
			"li x16, 0x820daa0020 \n"                 
			"fsw.ps f26, (x16) \n"                 
			"li x16, 0x820ed69820 \n"                 
			"fsw.ps f29, (x16) \n"                 
			"li x24, 0x820f588820 \n"                 
			"fsw.ps f30, (x24) \n"                 
			"fence\n"
			"LBL_GLOBAL_VADDR_INIT_HID_0:\n"          
			"li x12, 0x81204807e0 \n"                 
			"fswg.ps f1, (x12) \n"                 
			"li x14, 0x81219607a0 \n"                 
			"fswg.ps f0, (x14) \n"                 
			"li x24, 0x8126a86820 \n"                 
			"fswg.ps f14, (x24) \n"                 
			"li x23, 0x812faa07e0 \n"                 
			"fswg.ps f5, (x23) \n"                 
			"li x15, 0x813108cfe0 \n"                 
			"fswg.ps f3, (x15) \n"                 
			"li x25, 0x8131980000 \n"                 
			"fswg.ps f30, (x25) \n"                 
			"li x23, 0x8132dd6000 \n"                 
			"fswg.ps f28, (x23) \n"                 
			"li x12, 0x8134122240 \n"                 
			"fswg.ps f31, (x12) \n"                 
			"li x23, 0x8135440020 \n"                 
			"fswg.ps f31, (x23) \n"                 
			"li x21, 0x8137260060 \n"                 
			"fswg.ps f29, (x21) \n"                 
			"li x20, 0x813c040340 \n"                 
			"fswg.ps f1, (x20) \n"                 
			"li x20, 0x813e2002a0 \n"                 
			"fswg.ps f22, (x20) \n"                 
			"li x13, 0x813e392060 \n"                 
			"fswg.ps f25, (x13) \n"                 
			"li x24, 0x82222e0020 \n"                 
			"fswg.ps f13, (x24) \n"                 
			"li x21, 0x8223d60000 \n"                 
			"fswg.ps f9, (x21) \n"                 
			"li x20, 0x822533bb40 \n"                 
			"fswg.ps f22, (x20) \n"                 
			"li x21, 0x82267a0400 \n"                 
			"fswg.ps f24, (x21) \n"                 
			"li x24, 0x8229ec77c0 \n"                 
			"fswg.ps f9, (x24) \n"                 
			"li x22, 0x822d1a0020 \n"                 
			"fswg.ps f19, (x22) \n"                 
			"li x20, 0x822daa0020 \n"                 
			"fswg.ps f27, (x20) \n"                 
			"li x18, 0x8230329780 \n"                 
			"fswg.ps f9, (x18) \n"                 
			"li x12, 0x8230a1a080 \n"                 
			"fswg.ps f9, (x12) \n"                 
			"li x21, 0x8231060800 \n"                 
			"fswg.ps f28, (x21) \n"                 
			"li x15, 0x82357a0000 \n"                 
			"fswg.ps f12, (x15) \n"                 
			"li x25, 0x8238a3f000 \n"                 
			"fswg.ps f2, (x25) \n"                 
			"li x10, 0x8238a45aa0 \n"                 
			"fswg.ps f13, (x10) \n"                 
			"li x15, 0x823f588820 \n"                 
			"fswg.ps f26, (x15) \n"                 
			"fence\n"
			"LBL_LOCAL_VADDR_INIT_HID_0:\n"          
			"li x18, 0x8142dd6000 \n"                 
			"fswl.ps f10, (x18) \n"                 
			"li x14, 0x8144122240 \n"                 
			"fswl.ps f22, (x14) \n"                 
			"li x14, 0x81453ae800 \n"                 
			"fswl.ps f6, (x14) \n"                 
			"li x18, 0x8145440020 \n"                 
			"fswl.ps f20, (x18) \n"                 
			"li x25, 0x8146a86820 \n"                 
			"fswl.ps f24, (x25) \n"                 
			"li x15, 0x8147260060 \n"                 
			"fswl.ps f23, (x15) \n"                 
			"li x23, 0x814faa07e0 \n"                 
			"fswl.ps f12, (x23) \n"                 
			"li x23, 0x8150aa01c0 \n"                 
			"fswl.ps f29, (x23) \n"                 
			"li x13, 0x81519607a0 \n"                 
			"fswl.ps f21, (x13) \n"                 
			"li x18, 0x8151980000 \n"                 
			"fswl.ps f14, (x18) \n"                 
			"li x13, 0x815774c9c0 \n"                 
			"fswl.ps f14, (x13) \n"                 
			"li x19, 0x815e2002a0 \n"                 
			"fswl.ps f3, (x19) \n"                 
			"li x11, 0x8241060800 \n"                 
			"fswl.ps f19, (x11) \n"                 
			"li x24, 0x824533bb40 \n"                 
			"fswl.ps f17, (x24) \n"                 
			"li x15, 0x82457a0000 \n"                 
			"fswl.ps f11, (x15) \n"                 
			"li x14, 0x8248a3f000 \n"                 
			"fswl.ps f12, (x14) \n"                 
			"li x23, 0x8248e60000 \n"                 
			"fswl.ps f9, (x23) \n"                 
			"li x15, 0x8249ec77c0 \n"                 
			"fswl.ps f20, (x15) \n"                 
			"li x13, 0x824a6e0260 \n"                 
			"fswl.ps f30, (x13) \n"                 
			"li x22, 0x824d1a0020 \n"                 
			"fswl.ps f27, (x22) \n"                 
			"li x13, 0x824daa0020 \n"                 
			"fswl.ps f4, (x13) \n"                 
			"li x16, 0x824e020020 \n"                 
			"fswl.ps f21, (x16) \n"                 
			"li x18, 0x824f588820 \n"                 
			"fswl.ps f18, (x18) \n"                 
			"li x19, 0x8250a1a080 \n"                 
			"fswl.ps f15, (x19) \n"                 
			"li x17, 0x8253d60000 \n"                 
			"fswl.ps f29, (x17) \n"                 
			"li x22, 0x82567a0400 \n"                 
			"fswl.ps f13, (x22) \n"                 
			"li x17, 0x8259b80020 \n"                 
			"fswl.ps f28, (x17) \n"                 
			"fence\n"
			"LBL_LOCAL_L2SCP_INIT_HID_0:\n"          
			"la x11, rand_ieee754_32\n"    
			"li x31, 0x40\n"                 
			"li x5, 0x1a00000000002f \n"                 
			"add x5, x11, x5\n"               
			"csrw tensor_load_l2, x5\n"         
			"addi x31, x31, 1\n"                 
			"li x5, 0x1e00000000002f \n"                 
			"add x5, x11, x5\n"               
			"csrw tensor_load_l2, x5\n"         
			"addi x31, x31, -1\n"                 
			"li x5, 0x2200000000002f \n"                 
			"add x5, x11, x5\n"               
			"csrw tensor_load_l2, x5\n"         
			"addi x31, x31, 1\n"                 
			"li x5, 0x2600000000002f \n"                 
			"add x5, x11, x5\n"               
			"csrw tensor_load_l2, x5\n"         
			"addi x31, x31, -1\n"                 
			"li x5, 0x3b00000000001f \n"                 
			"add x5, x11, x5\n"               
			"csrw tensor_load_l2, x5\n"         
			"addi x31, x31, 1\n"                 
			"li x5, 0x3f00000000001f \n"                 
			"add x5, x11, x5\n"               
			"csrw tensor_load_l2, x5\n"         
			"addi x31, x31, -1\n"                 
			"li x5, 0x4300000000001f \n"                 
			"add x5, x11, x5\n"               
			"csrw tensor_load_l2, x5\n"         
			"addi x31, x31, 1\n"                 
			"li x5, 0x4700000000001f \n"                 
			"add x5, x11, x5\n"               
			"csrw tensor_load_l2, x5\n"         
			"addi x31, x31, -1\n"                 
			"li x5, 0x4e00000000000f \n"                 
			"add x5, x11, x5\n"               
			"csrw tensor_load_l2, x5\n"         
			"addi x31, x31, 1\n"                 
			"li x5, 0x5200000000000f \n"                 
			"add x5, x11, x5\n"               
			"csrw tensor_load_l2, x5\n"         
			"addi x31, x31, -1\n"                 
			"li x5, 0x5600000000000f \n"                 
			"add x5, x11, x5\n"               
			"csrw tensor_load_l2, x5\n"         
			"addi x31, x31, 1\n"                 
			"li x5, 0x5a00000000000f \n"                 
			"add x5, x11, x5\n"               
			"csrw tensor_load_l2, x5\n"         
			"addi x31, x31, -1\n"                 
			"li x5, 0x6100000000002f \n"                 
			"add x5, x11, x5\n"               
			"csrw tensor_load_l2, x5\n"         
			"addi x31, x31, 1\n"                 
			"li x5, 0x6400000000000f \n"                 
			"add x5, x11, x5\n"               
			"csrw tensor_load_l2, x5\n"         
			"addi x31, x31, -1\n"                 
			"li x5, 0x6500000000002f \n"                 
			"add x5, x11, x5\n"               
			"csrw tensor_load_l2, x5\n"         
			"addi x31, x31, 1\n"                 
			"li x5, 0x6800000000000f \n"                 
			"add x5, x11, x5\n"               
			"csrw tensor_load_l2, x5\n"         
			"addi x31, x31, -1\n"                 
			"li x5, 0x6900000000002f \n"                 
			"add x5, x11, x5\n"               
			"csrw tensor_load_l2, x5\n"         
			"addi x31, x31, 1\n"                 
			"li x5, 0x6c00000000000f \n"                 
			"add x5, x11, x5\n"               
			"csrw tensor_load_l2, x5\n"         
			"addi x31, x31, -1\n"                 
			"li x5, 0x6d00000000002f \n"                 
			"add x5, x11, x5\n"               
			"csrw tensor_load_l2, x5\n"         
			"addi x31, x31, 1\n"                 
			"li x5, 0x7000000000000f \n"                 
			"add x5, x11, x5\n"               
			"csrw tensor_load_l2, x5\n"         
			"addi x31, x31, -1\n"                 
			"li x5, 0x7000000000000f \n"                 
			"add x5, x11, x5\n"               
			"csrw tensor_load_l2, x5\n"         
			"addi x31, x31, 1\n"                 
			"li x5, 0x7400000000000f \n"                 
			"add x5, x11, x5\n"               
			"csrw tensor_load_l2, x5\n"         
			"addi x31, x31, -1\n"                 
			"li x5, 0x7800000000000f \n"                 
			"add x5, x11, x5\n"               
			"csrw tensor_load_l2, x5\n"         
			"addi x31, x31, 1\n"                 
			"li x5, 0x7c00000000000f \n"                 
			"add x5, x11, x5\n"               
			"csrw tensor_load_l2, x5\n"         
			"addi x31, x31, -1\n"                 
			"li x5, 0x900000000002f \n"                 
			"add x5, x11, x5\n"               
			"csrw tensor_load_l2, x5\n"         
			"addi x31, x31, 1\n"                 
			"li x5, 0xd00000000002f \n"                 
			"add x5, x11, x5\n"               
			"csrw tensor_load_l2, x5\n"         
			"addi x31, x31, -1\n"                 
			"li x5, 0x1100000000002f \n"                 
			"add x5, x11, x5\n"               
			"csrw tensor_load_l2, x5\n"         
			"addi x31, x31, 1\n"                 
			"li x5, 0x1500000000002f \n"                 
			"add x5, x11, x5\n"               
			"csrw tensor_load_l2, x5\n"         
			"addi x31, x31, -1\n"                 
			"li x5, 0x5f00000000001f \n"                 
			"add x5, x11, x5\n"               
			"csrw tensor_load_l2, x5\n"         
			"addi x31, x31, 1\n"                 
			"li x5, 0x6300000000001f \n"                 
			"add x5, x11, x5\n"               
			"csrw tensor_load_l2, x5\n"         
			"addi x31, x31, -1\n"                 
			"li x5, 0x6700000000001f \n"                 
			"add x5, x11, x5\n"               
			"csrw tensor_load_l2, x5\n"         
			"addi x31, x31, 1\n"                 
			"li x5, 0x6b00000000001f \n"                 
			"add x5, x11, x5\n"               
			"csrw tensor_load_l2, x5\n"         
			"addi x31, x31, -1\n"                 
			"csrwi tensor_wait, 0x2\n"      
			"csrwi tensor_wait, 0x3\n"      
			"fence\n"
			"LBL_TXR_INIT_HID_0:\n"          
			"li x14, 0x816e392040 \n"                 
			"fsw.ps f21, 0(x14) \n"                 
			"fsw.ps f23, 64(x14) \n"                 
			"fsw.ps f10, 128(x14) \n"                 
			"fsw.ps f27, 192(x14) \n"                 
			"fsw.ps f21, 256(x14) \n"                 
			"fsw.ps f16, 320(x14) \n"                 
			"fsw.ps f20, 384(x14) \n"                 
			"fsw.ps f20, 448(x14) \n"                 
			"fsw.ps f26, 512(x14) \n"                 
			"fsw.ps f18, 576(x14) \n"                 
			"fsw.ps f14, 640(x14) \n"                 
			"fsw.ps f18, 704(x14) \n"                 
			"fsw.ps f22, 768(x14) \n"                 
			"fsw.ps f28, 832(x14) \n"                 
			"fsw.ps f24, 896(x14) \n"                 
			"li x15, 0x8174122240 \n"                 
			"fsw.ps f20, 0(x15) \n"                 
			"fsw.ps f8, 64(x15) \n"                 
			"fsw.ps f16, 128(x15) \n"                 
			"fsw.ps f27, 192(x15) \n"                 
			"fsw.ps f0, 256(x15) \n"                 
			"fsw.ps f3, 320(x15) \n"                 
			"fsw.ps f7, 384(x15) \n"                 
			"fsw.ps f7, 448(x15) \n"                 
			"fsw.ps f1, 512(x15) \n"                 
			"fsw.ps f22, 576(x15) \n"                 
			"fsw.ps f23, 640(x15) \n"                 
			"fsw.ps f9, 704(x15) \n"                 
			"li x23, 0x81753ae800 \n"                 
			"fsw.ps f27, 0(x23) \n"                 
			"fsw.ps f20, 64(x23) \n"                 
			"fsw.ps f31, 128(x23) \n"                 
			"fsw.ps f22, 192(x23) \n"                 
			"fsw.ps f13, 256(x23) \n"                 
			"fsw.ps f22, 320(x23) \n"                 
			"li x20, 0x81756ef000 \n"                 
			"fsw.ps f31, 0(x20) \n"                 
			"fsw.ps f3, 64(x20) \n"                 
			"fsw.ps f23, 128(x20) \n"                 
			"fsw.ps f17, 192(x20) \n"                 
			"fsw.ps f30, 256(x20) \n"                 
			"fsw.ps f10, 320(x20) \n"                 
			"fsw.ps f25, 384(x20) \n"                 
			"fsw.ps f14, 448(x20) \n"                 
			"fsw.ps f3, 512(x20) \n"                 
			"fsw.ps f4, 576(x20) \n"                 
			"fsw.ps f20, 640(x20) \n"                 
			"fsw.ps f17, 704(x20) \n"                 
			"fsw.ps f31, 768(x20) \n"                 
			"fsw.ps f14, 832(x20) \n"                 
			"fsw.ps f23, 896(x20) \n"                 
			"fsw.ps f6, 960(x20) \n"                 
			"li x20, 0x8176a86800 \n"                 
			"fsw.ps f21, 0(x20) \n"                 
			"fsw.ps f16, 64(x20) \n"                 
			"fsw.ps f2, 128(x20) \n"                 
			"fsw.ps f25, 192(x20) \n"                 
			"fsw.ps f6, 256(x20) \n"                 
			"fsw.ps f29, 320(x20) \n"                 
			"fsw.ps f21, 384(x20) \n"                 
			"fsw.ps f26, 448(x20) \n"                 
			"fsw.ps f29, 512(x20) \n"                 
			"fsw.ps f13, 576(x20) \n"                 
			"fsw.ps f28, 640(x20) \n"                 
			"fsw.ps f14, 704(x20) \n"                 
			"fsw.ps f25, 768(x20) \n"                 
			"li x11, 0x8176d8f000 \n"                 
			"fsw.ps f4, 0(x11) \n"                 
			"fsw.ps f23, 64(x11) \n"                 
			"fsw.ps f25, 128(x11) \n"                 
			"fsw.ps f8, 192(x11) \n"                 
			"fsw.ps f8, 256(x11) \n"                 
			"li x13, 0x817774c9c0 \n"                 
			"fsw.ps f20, 0(x13) \n"                 
			"fsw.ps f20, 64(x13) \n"                 
			"fsw.ps f20, 128(x13) \n"                 
			"fsw.ps f16, 192(x13) \n"                 
			"fsw.ps f25, 256(x13) \n"                 
			"fsw.ps f11, 320(x13) \n"                 
			"fsw.ps f24, 384(x13) \n"                 
			"fsw.ps f17, 448(x13) \n"                 
			"fsw.ps f19, 512(x13) \n"                 
			"fsw.ps f9, 576(x13) \n"                 
			"fsw.ps f30, 640(x13) \n"                 
			"fsw.ps f7, 704(x13) \n"                 
			"fsw.ps f21, 768(x13) \n"                 
			"fsw.ps f5, 832(x13) \n"                 
			"fsw.ps f7, 896(x13) \n"                 
			"fsw.ps f26, 960(x13) \n"                 
			"li x10, 0x8185522400 \n"                 
			"fsw.ps f11, 0(x10) \n"                 
			"fsw.ps f24, 64(x10) \n"                 
			"fsw.ps f11, 128(x10) \n"                 
			"fsw.ps f17, 192(x10) \n"                 
			"fsw.ps f8, 256(x10) \n"                 
			"fsw.ps f3, 320(x10) \n"                 
			"fsw.ps f8, 384(x10) \n"                 
			"fsw.ps f21, 448(x10) \n"                 
			"fsw.ps f1, 512(x10) \n"                 
			"fsw.ps f22, 576(x10) \n"                 
			"fsw.ps f29, 640(x10) \n"                 
			"fsw.ps f17, 704(x10) \n"                 
			"fsw.ps f3, 768(x10) \n"                 
			"fsw.ps f1, 832(x10) \n"                 
			"fsw.ps f11, 896(x10) \n"                 
			"li x18, 0x81856ef000 \n"                 
			"fsw.ps f23, 0(x18) \n"                 
			"fsw.ps f17, 64(x18) \n"                 
			"fsw.ps f20, 128(x18) \n"                 
			"fsw.ps f19, 192(x18) \n"                 
			"fsw.ps f27, 256(x18) \n"                 
			"fsw.ps f14, 320(x18) \n"                 
			"fsw.ps f19, 384(x18) \n"                 
			"fsw.ps f25, 448(x18) \n"                 
			"fsw.ps f3, 512(x18) \n"                 
			"fsw.ps f24, 576(x18) \n"                 
			"fsw.ps f27, 640(x18) \n"                 
			"fsw.ps f12, 704(x18) \n"                 
			"fsw.ps f23, 768(x18) \n"                 
			"li x12, 0x8192dd6000 \n"                 
			"fsw.ps f10, 0(x12) \n"                 
			"fsw.ps f18, 64(x12) \n"                 
			"fsw.ps f14, 128(x12) \n"                 
			"fsw.ps f31, 192(x12) \n"                 
			"fsw.ps f24, 256(x12) \n"                 
			"fsw.ps f26, 320(x12) \n"                 
			"li x19, 0x8194122240 \n"                 
			"fsw.ps f28, 0(x19) \n"                 
			"fsw.ps f27, 64(x19) \n"                 
			"fsw.ps f15, 128(x19) \n"                 
			"fsw.ps f9, 192(x19) \n"                 
			"fsw.ps f14, 256(x19) \n"                 
			"li x13, 0x819a7a6000 \n"                 
			"fsw.ps f6, 0(x13) \n"                 
			"fsw.ps f19, 64(x13) \n"                 
			"fsw.ps f5, 128(x13) \n"                 
			"fsw.ps f11, 192(x13) \n"                 
			"fsw.ps f17, 256(x13) \n"                 
			"fsw.ps f3, 320(x13) \n"                 
			"fsw.ps f21, 384(x13) \n"                 
			"fsw.ps f17, 448(x13) \n"                 
			"fsw.ps f16, 512(x13) \n"                 
			"fsw.ps f4, 576(x13) \n"                 
			"fsw.ps f5, 640(x13) \n"                 
			"fsw.ps f28, 704(x13) \n"                 
			"fsw.ps f31, 768(x13) \n"                 
			"fsw.ps f20, 832(x13) \n"                 
			"li x22, 0x819e392040 \n"                 
			"fsw.ps f27, 0(x22) \n"                 
			"fsw.ps f15, 64(x22) \n"                 
			"fsw.ps f3, 128(x22) \n"                 
			"fsw.ps f31, 192(x22) \n"                 
			"fsw.ps f15, 256(x22) \n"                 
			"fsw.ps f8, 320(x22) \n"                 
			"fsw.ps f25, 384(x22) \n"                 
			"fsw.ps f0, 448(x22) \n"                 
			"fsw.ps f22, 512(x22) \n"                 
			"fsw.ps f10, 576(x22) \n"                 
			"fsw.ps f11, 640(x22) \n"                 
			"fsw.ps f18, 704(x22) \n"                 
			"fsw.ps f21, 768(x22) \n"                 
			"fsw.ps f30, 832(x22) \n"                 
			"li x19, 0x8268a3f000 \n"                 
			"fsw.ps f19, 0(x19) \n"                 
			"fsw.ps f15, 64(x19) \n"                 
			"fsw.ps f22, 128(x19) \n"                 
			"fsw.ps f17, 192(x19) \n"                 
			"fsw.ps f21, 256(x19) \n"                 
			"fsw.ps f14, 320(x19) \n"                 
			"fsw.ps f24, 384(x19) \n"                 
			"fsw.ps f14, 448(x19) \n"                 
			"li x22, 0x8270329780 \n"                 
			"fsw.ps f27, 0(x22) \n"                 
			"fsw.ps f10, 64(x22) \n"                 
			"fsw.ps f9, 128(x22) \n"                 
			"fsw.ps f1, 192(x22) \n"                 
			"fsw.ps f0, 256(x22) \n"                 
			"fsw.ps f13, 320(x22) \n"                 
			"fsw.ps f13, 384(x22) \n"                 
			"fsw.ps f12, 448(x22) \n"                 
			"fsw.ps f16, 512(x22) \n"                 
			"fsw.ps f14, 576(x22) \n"                 
			"fsw.ps f27, 640(x22) \n"                 
			"li x16, 0x8270a1a080 \n"                 
			"fsw.ps f8, 0(x16) \n"                 
			"fsw.ps f15, 64(x16) \n"                 
			"fsw.ps f14, 128(x16) \n"                 
			"fsw.ps f13, 192(x16) \n"                 
			"fsw.ps f6, 256(x16) \n"                 
			"fsw.ps f22, 320(x16) \n"                 
			"fsw.ps f18, 384(x16) \n"                 
			"fsw.ps f25, 448(x16) \n"                 
			"fsw.ps f2, 512(x16) \n"                 
			"fsw.ps f19, 576(x16) \n"                 
			"fsw.ps f17, 640(x16) \n"                 
			"li x22, 0x8278a45a80 \n"                 
			"fsw.ps f14, 0(x22) \n"                 
			"fsw.ps f19, 64(x22) \n"                 
			"fsw.ps f6, 128(x22) \n"                 
			"fsw.ps f3, 192(x22) \n"                 
			"fsw.ps f2, 256(x22) \n"                 
			"fsw.ps f20, 320(x22) \n"                 
			"fsw.ps f23, 384(x22) \n"                 
			"fsw.ps f17, 448(x22) \n"                 
			"fsw.ps f24, 512(x22) \n"                 
			"fsw.ps f22, 576(x22) \n"                 
			"fsw.ps f1, 640(x22) \n"                 
			"fsw.ps f13, 704(x22) \n"                 
			"fsw.ps f3, 768(x22) \n"                 
			"li x15, 0x827f588800 \n"                 
			"fsw.ps f18, 0(x15) \n"                 
			"fsw.ps f25, 64(x15) \n"                 
			"fsw.ps f6, 128(x15) \n"                 
			"fsw.ps f27, 192(x15) \n"                 
			"fsw.ps f9, 256(x15) \n"                 
			"fsw.ps f17, 320(x15) \n"                 
			"fsw.ps f22, 384(x15) \n"                 
			"fsw.ps f6, 448(x15) \n"                 
			"fsw.ps f22, 512(x15) \n"                 
			"fsw.ps f19, 576(x15) \n"                 
			"li x23, 0x8280329780 \n"                 
			"fsw.ps f3, 0(x23) \n"                 
			"fsw.ps f20, 64(x23) \n"                 
			"fsw.ps f16, 128(x23) \n"                 
			"fsw.ps f17, 192(x23) \n"                 
			"fsw.ps f15, 256(x23) \n"                 
			"fsw.ps f27, 320(x23) \n"                 
			"fsw.ps f9, 384(x23) \n"                 
			"fsw.ps f16, 448(x23) \n"                 
			"fsw.ps f22, 512(x23) \n"                 
			"fsw.ps f16, 576(x23) \n"                 
			"fsw.ps f15, 640(x23) \n"                 
			"fsw.ps f22, 704(x23) \n"                 
			"fsw.ps f7, 768(x23) \n"                 
			"fsw.ps f9, 832(x23) \n"                 
			"fsw.ps f8, 896(x23) \n"                 
			"li x22, 0x8280a1a080 \n"                 
			"fsw.ps f8, 0(x22) \n"                 
			"fsw.ps f19, 64(x22) \n"                 
			"fsw.ps f2, 128(x22) \n"                 
			"fsw.ps f30, 192(x22) \n"                 
			"fsw.ps f23, 256(x22) \n"                 
			"li x18, 0x8281060800 \n"                 
			"fsw.ps f23, 0(x18) \n"                 
			"fsw.ps f0, 64(x18) \n"                 
			"fsw.ps f4, 128(x18) \n"                 
			"fsw.ps f12, 192(x18) \n"                 
			"fsw.ps f8, 256(x18) \n"                 
			"fsw.ps f3, 320(x18) \n"                 
			"fsw.ps f28, 384(x18) \n"                 
			"fsw.ps f27, 448(x18) \n"                 
			"li x13, 0x828533bb40 \n"                 
			"fsw.ps f23, 0(x13) \n"                 
			"fsw.ps f2, 64(x13) \n"                 
			"fsw.ps f7, 128(x13) \n"                 
			"fsw.ps f10, 192(x13) \n"                 
			"fsw.ps f0, 256(x13) \n"                 
			"fsw.ps f9, 320(x13) \n"                 
			"fsw.ps f15, 384(x13) \n"                 
			"fsw.ps f29, 448(x13) \n"                 
			"fsw.ps f22, 512(x13) \n"                 
			"fsw.ps f8, 576(x13) \n"                 
			"fsw.ps f7, 640(x13) \n"                 
			"fsw.ps f5, 704(x13) \n"                 
			"fsw.ps f26, 768(x13) \n"                 
			"li x21, 0x8288a3f000 \n"                 
			"fsw.ps f14, 0(x21) \n"                 
			"fsw.ps f16, 64(x21) \n"                 
			"fsw.ps f28, 128(x21) \n"                 
			"fsw.ps f20, 192(x21) \n"                 
			"fsw.ps f11, 256(x21) \n"                 
			"fsw.ps f29, 320(x21) \n"                 
			"fsw.ps f15, 384(x21) \n"                 
			"fsw.ps f10, 448(x21) \n"                 
			"fsw.ps f20, 512(x21) \n"                 
			"fsw.ps f1, 576(x21) \n"                 
			"li x18, 0x828f588800 \n"                 
			"fsw.ps f29, 0(x18) \n"                 
			"fsw.ps f17, 64(x18) \n"                 
			"fsw.ps f29, 128(x18) \n"                 
			"fsw.ps f25, 192(x18) \n"                 
			"fsw.ps f1, 256(x18) \n"                 
			"fsw.ps f22, 320(x18) \n"                 
			"fsw.ps f20, 384(x18) \n"                 
			"fsw.ps f26, 448(x18) \n"                 
			"fsw.ps f1, 512(x18) \n"                 
			"li x20, 0x8299ec77c0 \n"                 
			"fsw.ps f11, 0(x20) \n"                 
			"fsw.ps f13, 64(x20) \n"                 
			"fsw.ps f11, 128(x20) \n"                 
			"fsw.ps f15, 192(x20) \n"                 
			"fsw.ps f16, 256(x20) \n"                 
			"fsw.ps f4, 320(x20) \n"                 
			"fsw.ps f20, 384(x20) \n"                 
			"fsw.ps f31, 448(x20) \n"                 
			"fsw.ps f10, 512(x20) \n"                 
			"fsw.ps f17, 576(x20) \n"                 
			"fsw.ps f25, 640(x20) \n"                 
			"fsw.ps f0, 704(x20) \n"                 
			"li x19, 0x829ed69800 \n"                 
			"fsw.ps f1, 0(x19) \n"                 
			"fsw.ps f25, 64(x19) \n"                 
			"fsw.ps f20, 128(x19) \n"                 
			"fsw.ps f11, 192(x19) \n"                 
			"fsw.ps f12, 256(x19) \n"                 
			"fsw.ps f9, 320(x19) \n"                 
			"fsw.ps f14, 384(x19) \n"                 
			"fence\n"
			"fence\n"
			"csrw fflags, zero\n"
			"csrwi frm, 0x0\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_mask, zero\n"
			"LBL_KERNEL_HID_0:\n"          
			"li x5, 0xFEEDFEED\n"
			"li x0, 0x5633d0986e\n" 
			"slti x0, x0, 0x7f2\n"
			"la x31, check_sum_hid_0\n"
			"li x5, 0x4\n" 
			"sw x5, (x31)\n"
			"li x29, 0x4\n"
			"LBL_OUTER_LOOP_SEQ_HID_0:\n"
			"nop \n"
			"nop \n"
			"li x30, 0\n"
			"addi x30, x30, 1\n"
			"la x16, rand_ieee754_32\n"
			"flw.ps f5, 608(x16)\n"
			"fclass.ps f2, f5\n" 
			"fsub.pi f28, f5, f28\n"
			"fpackreph.pi f5, f14\n"
			"li x12, 0x54515fa3a7e5ca96\n"
			"ori x7, x12, 0xb\n" 
			"feq.pi f2, f16, f14\n"
			"bne x4, x5, 1f\n"
			"1:\n"
			"addiw x28, x15, 0x1f\n"
			"bgeu x23, x26, 1f\n"
			"1:\n"
			"la x22, rand_ieee754_32\n"
			"flw.ps f7, 64(x22)\n"
			"flw.ps f15, 256(x22)\n"
			"flw.ps f29, 64(x22)\n"
			"fmadd.s f29, f7, f15, f29, rup\n" 
			"fadd.ps f29, f25, f8, rtz\n"
			"fmadd.s f9, f26, f12, f16, rne\n"
			"li x17, 0xea347275b526dd1c\n"
			"li x7, 0xd35b66685e378a27\n"
			"slt x28, x17, x7\n" 
			"fmul.s f5, f14, f5, rne\n"
			"fmv.s.x f14, x11\n"
			"la x20, rand_ieee754_32\n"
			"flw.ps f14, 960(x20)\n"
			"flw.ps f2, 320(x20)\n"
			"flw.ps f10, 224(x20)\n"
			"fcmov.ps f16, f14, f2, f10\n" 
			"fnmsub.ps f2, f15, f26, f21, rne\n"
			"bgeu x2, x13, 1f\n"
			"1:\n"
			"fexp.ps f9, f8\n"
			"blt x0, x8, 1f\n"
			"1:\n"
			"la x22, rand_int_32\n"
			"flw.ps f9, 704(x22)\n"
			"fpackreph.pi f31, f9\n" 
			"li x5, 0xf85268c0a71c41a7\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0xa3369b45bdf541ea\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"fsub.pi f21, f7, f21\n"
			"fmv.s.x f16, x11\n"
			"la x12, rand_ieee754_32\n"
			"flw.ps f10, 736(x12)\n"
			"flw.ps f15, 352(x12)\n"
			"fadd.s f2, f10, f15, rup\n" 
			"fle.ps f14, f14, f14\n"
			"fsatu8.pi f21, f12\n"
			"la x16, rand_ieee754_32\n"
			"flw.ps f29, 800(x16)\n"
			"flw.ps f15, 192(x16)\n"
			"fadd.s f16, f29, f15, rtz\n" 
			"fsrli.pi f31, f15, 0xd\n"
			"fadd.ps f14, f26, f2, rup\n"
			"la x11, rand_ieee754_32\n"
			"flw.ps f10, 0(x11)\n"
			"flw.ps f15, 64(x11)\n"
			"fmadd.ps f7, f10, f15, f15, rtz\n" 
			"fmadd.ps f25, f21, f15, f25, rdn\n"
			"fmul.s f21, f12, f21, rne\n"
			"la x10, rand_int_32\n"
			"flw.ps f7, 832(x10)\n"
			"faddi.pi f31, f7, 0x1ea\n" 
			"faddi.pi f9, f21, 0x1d5\n"
			"fmul.s f21, f2, f2, rdn\n"
			"fbci.pi f29, 0x67248\n" 
			"fmadd.ps f31, f28, f28, f28, rdn\n"
			"fexp.ps f29, f29\n"
			"la x22, rand_ieee754_32\n"
			"flw.ps f14, 192(x22)\n"
			"flw.ps f29, 288(x22)\n"
			"fmax.s f5, f14, f29\n" 
			"slt x7, x7, x28\n"
			"fmadd.s f25, f26, f10, f14, rdn\n"
			"bge x11, x22, 1f\n"
			"1:\n"
			"fbci.pi f21, 0x365d3\n" 
			"ori x11, x15, 0xb\n"
			"fltm.ps m1, f9, f28\n"
			"bge x2, x26, 1f\n"
			"1:\n"
			"la x20, rand_ieee754_32\n"
			"flw.ps f8, 864(x20)\n"
			"fcvt.w.s x15, f8, rup\n" 
			"fadd.s f31, f10, f15, rne\n"
			"bne x14, x5, 1f\n"
			"1:\n"
			"faddi.pi f8, f8, 0x193\n"
			"la x11, rand_int_32\n"
			"flw.ps f12, 224(x11)\n"
			"flw.ps f31, 128(x11)\n"
			"fsub.pi f8, f12, f31\n" 
			"fsra.pi f10, f21, f10\n"
			"blt x20, x21, 1f\n"
			"1:\n"
			"fsra.pi f10, f26, f15\n"
			"bgeu x25, x20, 1f\n"
			"1:\n"
			"la x16, rand_ieee754_32\n"
			"flw.ps f14, 416(x16)\n"
			"flw.ps f25, 480(x16)\n"
			"fnmsub.ps f16, f14, f25, f14, rtz\n" 
			"fsub.pi f16, f2, f16\n"
			"fsub.pi f14, f2, f14\n"
			"li x7, 0x796e735cbfeb8fbb\n"
			"addiw x28, x7, 0x17\n" 
			"fsgnjn.s f29, f25, f7\n"
			"beq x17, x2, 1f\n"
			"1:\n"
			"slt x15, x27, x27\n"
			"la x7, rand_int_32\n"
			"flw.ps f16, 448(x7)\n"
			"fpackreph.pi f2, f16\n" 
			"slli x28, x12, 0\n"
			"fsrli.pi f21, f8, 0xf\n"
			"la x27, rand_ieee754_32\n"
			"flw.ps f31, 512(x27)\n"
			"flw.ps f10, 800(x27)\n"
			"fadd.s f10, f31, f10, rmm\n" 
			"fadd.ps f31, f7, f14, rtz\n"
			"sll x12, x28, x12\n"
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_0:\n"
			"maskand m2, m2, m1\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"fsra.pi f2, f25, f2\n"
			"bne x4, x24, 1f\n"
			"1:\n"
			"fcmov.ps f28, f31, f14, f9\n"
			"la x28, rand_int_32\n"
			"flw.ps f14, 928(x28)\n"
			"flw.ps f31, 256(x28)\n"
			"fsrl.pi f16, f14, f31\n" 
			"sll x22, x22, x27\n"
			"fle.ps f15, f12, f5\n"
			"blt x28, x14, 1f\n"
			"1:\n"
			"la x12, rand_ieee754_32\n"
			"flw.ps f12, 704(x12)\n"
			"fclass.ps f9, f12\n" 
			"slt x28, x16, x28\n"
			"fcmov.ps f5, f21, f26, f9\n"
			"li x17, 0x992973fd4ac8929d\n"
			"ori x15, x17, 0x12\n" 
			"fsgnjn.s f2, f10, f10\n"
			"fexp.ps f2, f16\n"
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_0:\n"
			"fltm.ps m5, f8, f29\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"slli x7, x7, 1\n"
			"maskand m1, m1, m6\n"
			"la x17, rand_int_32\n"
			"flw.ps f2, 864(x17)\n"
			"fpackreph.pi f7, f2\n" 
			"fclass.ps f26, f31\n"
			"fsub.pi f14, f16, f5\n"
			"la x12, rand_int_32\n"
			"flw.ps f9, 832(x12)\n"
			"fsrli.pi f2, f9, 0x0\n" 
			"fltm.ps m5, f2, f2\n"
			"sraw x16, x10, x7\n"
			"la x15, rand_ieee754_32\n"
			"flw.ps f7, 832(x15)\n"
			"fcvt.w.s x28, f7, rup\n" 
			"fxor.pi f2, f21, f2\n"
			"fadd.ps f9, f21, f29, rmm\n"
			"bge x7, x2, 1f\n"
			"1:\n"
			"la x10, rand_ieee754_32\n"
			"flw.ps f29, 736(x10)\n"
			"flw.ps f16, 192(x10)\n"
			"flw.ps f25, 992(x10)\n"
			"fmadd.ps f26, f29, f16, f25, rmm\n" 
			"fcmov.ps f10, f5, f8, f14\n"
			"faddi.pi f25, f28, 0x1e0\n"
			"la x17, rand_ieee754_32\n"
			"flw.ps f5, 800(x17)\n"
			"flw.ps f2, 864(x17)\n"
			"fle.ps f16, f5, f2\n" 
			"fsgnjn.s f16, f25, f26\n"
			"bge x22, x13, 1f\n"
			"1:\n"
			"faddi.pi f25, f2, 0x110\n"
			"li x11, 0xf21eab9815d872ab\n"
			"fmv.s.x f31, x11\n" 
			"fxor.pi f2, f10, f2\n"
			"sraw x10, x22, x7\n"
			"la x7, rand_ieee754_32\n"
			"flw.ps f26, 480(x7)\n"
			"flw.ps f5, 512(x7)\n"
			"fsgnjn.s f21, f26, f5\n" 
			"fmv.s.x f28, x20\n"
			"fclass.ps f14, f7\n"
			"la x27, rand_int_32\n"
			"flw.ps f10, 704(x27)\n"
			"flw.ps f15, 704(x27)\n"
			"feq.pi f14, f10, f15\n" 
			"fcvt.s.w f28, x28, rne\n"
			"fsgnjn.s f31, f12, f31\n"
			"csrw tensor_error, zero\n" 
			"csrwi tensor_wait, 0x6\n" 
			"fence\n"
			"li x5,  0xc0000000000000f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc0000000000004f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc0000000000008f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc000000000000cf\n" 
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
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ID_280972_HID_0:\n"
			"li x6, ((0x8 << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0xb << TENSOR_QUANT_QUANT_ROW) | (0x24 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x9 << TENSOR_QUANT_TRANSF6) | (0x4 << TENSOR_QUANT_TRANSF5) | (0x9 << TENSOR_QUANT_TRANSF4) | (0x4 << TENSOR_QUANT_TRANSF3) | (0x3 << TENSOR_QUANT_TRANSF2) | (0x3 << TENSOR_QUANT_TRANSF1) | (0x5 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x7, ((0x2 << TENSOR_STORE_REG_STRIDE) | (0x6 << TENSOR_STORE_START_REG) | (0x1 << TENSOR_STORE_STORE_COLS) | (0x3 << TENSOR_STORE_AROWS) | (0x0 << TENSOR_STORE_COOP_STORE) | (0x0 << TENSOR_STORE_OPC_0) | (0x8270a1a08 << TENSOR_STORE_VADDR) | 0 )\n" 
			"li x31, (0x20 << 4)\n"
			"csrw tensor_quant, x6\n"	
			"csrw tensor_store, x7\n"	
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ID_235069_HID_0:\n"
			"li x5, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x1 << TENSOR_IMA_B_NUM_COLS) | (0xe << TENSOR_IMA_A_NUM_ROWS) | (0x0 << TENSOR_IMA_A_NUM_COLS) | (0x7 << TENSOR_IMA_OFFSET) | (0x0 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x27 << TENSOR_IMA_SCP_LOC_B) | (0x6c << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x0 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x6, ((0x12 << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0xe << TENSOR_QUANT_QUANT_ROW) | (0x2b << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x0 << TENSOR_QUANT_TRANSF1) | (0x0 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x7, ((0x2 << TENSOR_STORE_REG_STRIDE) | (0xb << TENSOR_STORE_START_REG) | (0x0 << TENSOR_STORE_STORE_COLS) | (0x5 << TENSOR_STORE_AROWS) | (0x0 << TENSOR_STORE_COOP_STORE) | (0x0 << TENSOR_STORE_OPC_0) | (0x816e39206 << TENSOR_STORE_VADDR) | 0 )\n" 
			"li x31, (0x20 << 4)\n"
			"csrw tensor_fma, x5\n"	
			"csrw tensor_quant, x6\n"	
			"csrw tensor_store, x7\n"	
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ID_559279_HID_0:\n"
			"li x5, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x0 << TENSOR_IMA_B_NUM_COLS) | (0x1 << TENSOR_IMA_A_NUM_ROWS) | (0x6 << TENSOR_IMA_A_NUM_COLS) | (0x4 << TENSOR_IMA_OFFSET) | (0x0 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x40 << TENSOR_IMA_SCP_LOC_B) | (0x4b << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x6, ((0x1f << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0x1 << TENSOR_QUANT_QUANT_ROW) | (0x3e << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x1 << TENSOR_QUANT_TRANSF2) | (0x8 << TENSOR_QUANT_TRANSF1) | (0x5 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x7, ((0x1 << TENSOR_STORE_SCP_ENTRY_STRIDE) | (0x4 << TENSOR_STORE_SCP_START_SCP_ENTRY) | (0x9 << TENSOR_STORE_SCP_AROWS) | (0x1 << TENSOR_STORE_SCP_OPC_1) | (0x209e2916a << TENSOR_STORE_SCP_VADDR) | 0 )\n" 
			"li x31, (0x3 << 6)\n"
			"csrw tensor_fma, x5\n"	
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 7\n"
			"csrwi tensor_wait, 10\n"
			"csrw tensor_store, x7\n"	
			"csrwi tensor_wait, 8\n"
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ID_515267_HID_0:\n"
			"li x5, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x1 << TENSOR_FMA16_B_NUM_COLS) | (0x5 << TENSOR_FMA16_A_NUM_ROWS) | (0xc << TENSOR_FMA16_A_NUM_COLS) | (0xf << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0xd8 << TENSOR_FMA16_SCP_LOC_B) | (0xcf << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x0 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x6, ((0x10 << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0x5 << TENSOR_QUANT_QUANT_ROW) | (0x37 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ID_444174_HID_0:\n"
			"li x5, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x1 << TENSOR_FMA32_B_NUM_COLS) | (0x3 << TENSOR_FMA32_A_NUM_ROWS) | (0x0 << TENSOR_FMA32_A_NUM_COLS) | (0xa << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0xec << TENSOR_FMA32_SCP_LOC_B) | (0xa5 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ID_363718_HID_0:\n"
			"li x5, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x0 << TENSOR_FMA32_B_NUM_COLS) | (0xd << TENSOR_FMA32_A_NUM_ROWS) | (0xf << TENSOR_FMA32_A_NUM_COLS) | (0xb << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x5 << TENSOR_FMA32_SCP_LOC_B) | (0x34 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0xd << TENSOR_QUANT_QUANT_ROW) | (0x32 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x0 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x7, ((0x2 << TENSOR_STORE_REG_STRIDE) | (0x1e << TENSOR_STORE_START_REG) | (0x1 << TENSOR_STORE_STORE_COLS) | (0xb << TENSOR_STORE_AROWS) | (0x0 << TENSOR_STORE_COOP_STORE) | (0x0 << TENSOR_STORE_OPC_0) | (0x817412225 << TENSOR_STORE_VADDR) | 0 )\n" 
			"li x31, (0x40 << 4)\n"
			"csrw tensor_fma, x5\n"	
			"csrw tensor_quant, x6\n"	
			"csrw tensor_store, x7\n"	
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ID_62970_HID_0:\n"
			"li x5, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x3 << TENSOR_IMA_B_NUM_COLS) | (0x9 << TENSOR_IMA_A_NUM_ROWS) | (0xe << TENSOR_IMA_A_NUM_COLS) | (0xd << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x95 << TENSOR_IMA_SCP_LOC_B) | (0x6f << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x0 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x7, ((0x1 << TENSOR_STORE_SCP_ENTRY_STRIDE) | (0x3d << TENSOR_STORE_SCP_START_SCP_ENTRY) | (0x1 << TENSOR_STORE_SCP_AROWS) | (0x1 << TENSOR_STORE_SCP_OPC_1) | (0x2fe017d << TENSOR_STORE_SCP_VADDR) | 0 )\n" 
			"li x31, (0x2 << 6)\n"
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
			"csrwi tensor_wait, 10\n"
			"csrw tensor_store, x7\n"	
			"csrwi tensor_wait, 8\n"
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ID_963362_HID_0:\n"
			"li x6, ((0x7 << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0xb << TENSOR_QUANT_QUANT_ROW) | (0x29 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x2 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ID_85591_HID_0:\n"
			"li x7, ((0x3 << TENSOR_STORE_SCP_ENTRY_STRIDE) | (0x2e << TENSOR_STORE_SCP_START_SCP_ENTRY) | (0xc << TENSOR_STORE_SCP_AROWS) | (0x1 << TENSOR_STORE_SCP_OPC_1) | (0x209e2916a << TENSOR_STORE_SCP_VADDR) | 0 )\n" 
			"li x31, (0x3 << 6)\n"
			"csrwi tensor_wait, 10\n"
			"csrw tensor_store, x7\n"	
			"csrwi tensor_wait, 8\n"
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ID_542207_HID_0:\n"
			"li x5, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x1 << TENSOR_FMA32_B_NUM_COLS) | (0xf << TENSOR_FMA32_A_NUM_ROWS) | (0xb << TENSOR_FMA32_A_NUM_COLS) | (0xa << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0xff << TENSOR_FMA32_SCP_LOC_B) | (0xb5 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x0 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x6, ((0x18 << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0xf << TENSOR_QUANT_QUANT_ROW) | (0x3b << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x2 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x7, ((0x1 << TENSOR_STORE_SCP_ENTRY_STRIDE) | (0x13 << TENSOR_STORE_SCP_START_SCP_ENTRY) | (0x7 << TENSOR_STORE_SCP_AROWS) | (0x1 << TENSOR_STORE_SCP_OPC_1) | (0x205d5bbc0 << TENSOR_STORE_SCP_VADDR) | 0 )\n" 
			"li x31, (0x2 << 6)\n"
			"csrw tensor_fma, x5\n"	
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 7\n"
			"csrwi tensor_wait, 10\n"
			"csrw tensor_store, x7\n"	
			"csrwi tensor_wait, 8\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_0\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ITER_0_HID_0:\n"
			"li x6, ((0xc << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0xf << TENSOR_QUANT_QUANT_ROW) | (0x1d << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x1 << TENSOR_QUANT_TRANSF1) | (0x3 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x7, ((0x3 << TENSOR_STORE_REG_STRIDE) | (0x14 << TENSOR_STORE_START_REG) | (0x3 << TENSOR_STORE_STORE_COLS) | (0x4 << TENSOR_STORE_AROWS) | (0x0 << TENSOR_STORE_COOP_STORE) | (0x0 << TENSOR_STORE_OPC_0) | (0x80004e2 << TENSOR_STORE_VADDR) | 0 )\n" 
			"li x31, (0x80 << 4)\n"
			"csrw tensor_quant, x6\n"	
			"csrw tensor_store, x7\n"	
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ITER_1_HID_0:\n"
			"li x5, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x0 << TENSOR_FMA16_B_NUM_COLS) | (0xa << TENSOR_FMA16_A_NUM_ROWS) | (0x2 << TENSOR_FMA16_A_NUM_COLS) | (0x7 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0xf8 << TENSOR_FMA16_SCP_LOC_B) | (0x6e << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x0 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ITER_2_HID_0:\n"
			"li x5, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x3 << TENSOR_FMA32_B_NUM_COLS) | (0xf << TENSOR_FMA32_A_NUM_ROWS) | (0x9 << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0xc1 << TENSOR_FMA32_SCP_LOC_B) | (0x1f << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x0 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x6, ((0x10 << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0xf << TENSOR_QUANT_QUANT_ROW) | (0x11 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x7, ((0x1 << TENSOR_STORE_REG_STRIDE) | (0x15 << TENSOR_STORE_START_REG) | (0x3 << TENSOR_STORE_STORE_COLS) | (0xb << TENSOR_STORE_AROWS) | (0x0 << TENSOR_STORE_COOP_STORE) | (0x0 << TENSOR_STORE_OPC_0) | (0xbf8009b << TENSOR_STORE_VADDR) | 0 )\n" 
			"li x31, (0x40 << 4)\n"
			"csrw tensor_fma, x5\n"	
			"csrw tensor_quant, x6\n"	
			"csrw tensor_store, x7\n"	
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ITER_3_HID_0:\n"
			"li x6, ((0x16 << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0x8 << TENSOR_QUANT_QUANT_ROW) | (0x24 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x2 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x7, ((0x0 << TENSOR_STORE_REG_STRIDE) | (0x16 << TENSOR_STORE_START_REG) | (0x3 << TENSOR_STORE_STORE_COLS) | (0x1 << TENSOR_STORE_AROWS) | (0x0 << TENSOR_STORE_COOP_STORE) | (0x0 << TENSOR_STORE_OPC_0) | (0x827032979 << TENSOR_STORE_VADDR) | 0 )\n" 
			"li x31, (0x80 << 4)\n"
			"csrw tensor_quant, x6\n"	
			"csrw tensor_store, x7\n"	
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ITER_4_HID_0:\n"
			"li x5, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x2 << TENSOR_FMA32_B_NUM_COLS) | (0x1 << TENSOR_FMA32_A_NUM_ROWS) | (0xf << TENSOR_FMA32_A_NUM_COLS) | (0xe << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x2f << TENSOR_FMA32_SCP_LOC_B) | (0x1b << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x6, ((0x16 << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0x1 << TENSOR_QUANT_QUANT_ROW) | (0x25 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x0 << TENSOR_QUANT_TRANSF1) | (0x2 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x7, ((0x0 << TENSOR_STORE_REG_STRIDE) | (0x1b << TENSOR_STORE_START_REG) | (0x3 << TENSOR_STORE_STORE_COLS) | (0xf << TENSOR_STORE_AROWS) | (0x0 << TENSOR_STORE_COOP_STORE) | (0x0 << TENSOR_STORE_OPC_0) | (0x828106080 << TENSOR_STORE_VADDR) | 0 )\n" 
			"li x31, (0x80 << 4)\n"
			"csrw tensor_fma, x5\n"	
			"csrw tensor_quant, x6\n"	
			"csrw tensor_store, x7\n"	
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ITER_5_HID_0:\n"
			"li x6, ((0x1e << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0x0 << TENSOR_QUANT_QUANT_ROW) | (0x27 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x2 << TENSOR_QUANT_TRANSF4) | (0x6 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x7, ((0x0 << TENSOR_STORE_REG_STRIDE) | (0x5 << TENSOR_STORE_START_REG) | (0x0 << TENSOR_STORE_STORE_COLS) | (0xd << TENSOR_STORE_AROWS) | (0x0 << TENSOR_STORE_COOP_STORE) | (0x0 << TENSOR_STORE_OPC_0) | (0x828533bb5 << TENSOR_STORE_VADDR) | 0 )\n" 
			"li x31, (0x20 << 4)\n"
			"csrw tensor_quant, x6\n"	
			"csrw tensor_store, x7\n"	
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ITER_6_HID_0:\n"
			"li x5, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x1 << TENSOR_FMA32_B_NUM_COLS) | (0x7 << TENSOR_FMA32_A_NUM_ROWS) | (0x4 << TENSOR_FMA32_A_NUM_COLS) | (0x8 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0xe0 << TENSOR_FMA32_SCP_LOC_B) | (0x57 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x0 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x6, ((0x19 << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0x7 << TENSOR_QUANT_QUANT_ROW) | (0x17 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x7, ((0x0 << TENSOR_STORE_REG_STRIDE) | (0x10 << TENSOR_STORE_START_REG) | (0x3 << TENSOR_STORE_STORE_COLS) | (0x1 << TENSOR_STORE_AROWS) | (0x0 << TENSOR_STORE_COOP_STORE) | (0x0 << TENSOR_STORE_OPC_0) | (0x8268a3f01 << TENSOR_STORE_VADDR) | 0 )\n" 
			"li x31, (0xc0 << 4)\n"
			"csrw tensor_fma, x5\n"	
			"csrw tensor_quant, x6\n"	
			"csrw tensor_store, x7\n"	
			"csrwi tensor_wait, 7\n"
			"csrwi tensor_wait, 10\n"
			"csrwi tensor_wait, 8\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_0\n"
			"csrw tensor_error, x0\n"
			"li x5,  0x1ff800000610007\n" 
			"csrw tensor_fma, x5\n" 
			"csrwi tensor_wait, 7\n" 
			"csrwi tensor_wait, 0x6\n" 
			"fence\n"
			"li x5,  0xc0000000000000f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc0000000000004f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc0000000000008f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc000000000000cf\n" 
			"csrw   evict_sw, x5\n" 
			"csrwi tensor_wait, 0x6\n" 
			"csrrci x0, mcache_control, 3\n"
			"csrwi tensor_wait, 0x6\n" 
			"fence\n"
			"mov.m.x m0, x0, 0xFF\n"
			"li x5, 0x20\n"
			"la x31, rand_ieee754_32\n"
			"add x31, x31, x5\n"
			"flw.ps f10, 0(x31)\n"
			"flw.ps f12, 0(x31)\n"
			"flw.ps f14, 3(x31)\n"
			"flw.ps f15, 8(x31)\n"
			"flw.ps f16, 6(x31)\n"
			"flw.ps f2, 0(x31)\n"
			"flw.ps f21, 0(x31)\n"
			"flw.ps f25, 21(x31)\n"
			"flw.ps f26, 20(x31)\n"
			"flw.ps f28, 2(x31)\n"
			"flw.ps f29, 29(x31)\n"
			"flw.ps f31, 42(x31)\n"
			"flw.ps f5, 28(x31)\n"
			"flw.ps f7, 25(x31)\n"
			"flw.ps f8, 2(x31)\n"
			"flw.ps f9, 22(x31)\n"
			"li x21, 0x3\n"
			"LBL_SEQID_0_LOOP_SEQ_HID_0:\n"
			"faddi.pi f2, f16, 0x83\n"
			"fsgnjn.s f26, f12, f25\n"
			"fmul.s f21, f8, f5, rup\n"
			"fmul.s f12, f31, f25, rne\n"
			"feq.pi f29, f9, f25\n"
			"feq.pi f25, f25, f2\n"
			"fmax.s f7, f2, f16\n"
			"bgeu x23, x29, 1f\n"
			"1:\n"
			"fmax.s f5, f16, f25\n"
			"fsrl.pi f7, f29, f7\n"
			"faddi.pi f9, f2, 0xc3\n"
			"fadd.s f25, f25, f21, rmm\n"
			"fsgnjn.s f26, f10, f26\n"
			"faddi.pi f15, f21, 0x1e3\n"
			"bne x30, x19, 1f\n"
			"1:\n"
			"fsrl.pi f7, f21, f2\n"
			"faddi.pi f12, f29, 0x56\n"
			"fsrl.pi f26, f2, f26\n"
			"fmul.s f26, f15, f25, rne\n"
			"fexp.ps f12, f21\n"
			"fmul.s f15, f9, f15, rup\n"
			"fexp.ps f2, f7\n"
			"fexp.ps f12, f12\n"
			"fsgnjn.s f28, f14, f28\n"
			"fsrl.pi f8, f9, f7\n"
			"fsgnjn.s f26, f21, f26\n"
			"feq.pi f31, f28, f12\n"
			"faddi.pi f14, f15, 0xa1\n"
			"fmax.s f14, f29, f25\n"
			"faddi.pi f26, f2, 0x185\n"
			"fmax.s f8, f8, f9\n"
			"fmax.s f15, f12, f5\n"
			"fsrli.pi f29, f25, 0x8\n"
			"fadd.s f16, f10, f14, rne\n"
			"fadd.s f16, f16, f15, rup\n"
			"fsrl.pi f7, f5, f7\n"
			"faddi.pi f10, f16, 0x19d\n"
			"fexp.ps f25, f14\n"
			"faddi.pi f28, f8, 0x39\n"
			"fsrli.pi f31, f12, 0xa\n"
			"fexp.ps f31, f21\n"
			"bge x17, x5, 1f\n"
			"1:\n"
			"feq.pi f21, f8, f8\n"
			"fexp.ps f7, f7\n"
			"bne x9, x27, 1f\n"
			"1:\n"
			"fsrli.pi f9, f5, 0x2\n"
			"fexp.ps f31, f15\n"
			"fmax.s f2, f26, f16\n"
			"fadd.s f28, f5, f2, rmm\n"
			"fsgnjn.s f21, f14, f15\n"
			"faddi.pi f26, f8, 0x193\n"
			"fsgnjn.s f31, f9, f31\n"
			"fsgnjn.s f29, f15, f16\n"
			"fmul.s f8, f12, f8, rdn\n"
			"fsgnjn.s f2, f26, f2\n"
			"fadd.s f26, f2, f15, rmm\n"
			"feq.pi f15, f10, f15\n"
			"fmul.s f7, f7, f26, rne\n"
			"feq.pi f15, f9, f15\n"
			"fadd.s f26, f29, f15, rdn\n"
			"fadd.s f14, f16, f14, rmm\n"
			"bne x31, x17, 1f\n"
			"1:\n"
			"fadd.s f15, f25, f15, rdn\n"
			"fadd.s f21, f2, f2, rup\n"
			"fadd.s f25, f21, f28, rdn\n"
			"fmul.s f26, f21, f15, rmm\n"
			"fsrli.pi f10, f31, 0x5\n"
			"fmax.s f9, f28, f31\n"
			"bne x13, x29, 1f\n"
			"1:\n"
			"fsgnjn.s f5, f16, f5\n"
			"fmul.s f10, f10, f12, rup\n"
			"blt x9, x25, 1f\n"
			"1:\n"
			"fsrl.pi f21, f26, f31\n"
			"fsgnjn.s f2, f31, f21\n"
			"fsgnjn.s f2, f29, f31\n"
			"bgeu x31, x29, 1f\n"
			"1:\n"
			"fmax.s f16, f5, f2\n"
			"fmul.s f29, f16, f26, rtz\n"
			"feq.pi f29, f25, f21\n"
			"fmul.s f28, f7, f25, rdn\n"
			"fsrli.pi f31, f9, 0x0\n"
			"fsrl.pi f21, f21, f21\n"
			"fmul.s f7, f12, f21, rtz\n"
			"feq.pi f29, f7, f29\n"
			"fadd.s f21, f14, f21, rne\n"
			"bltu x10, x5, 1f\n"
			"1:\n"
			"faddi.pi f5, f15, 0x14d\n"
			"fsrl.pi f15, f29, f28\n"
			"fsrli.pi f12, f31, 0xb\n"
			"fmul.s f26, f29, f16, rtz\n"
			"feq.pi f16, f25, f29\n"
			"bne x30, x22, 1f\n"
			"1:\n"
			"fadd.s f28, f7, f15, rmm\n"
			"fsgnjn.s f10, f12, f8\n"
			"feq.pi f10, f10, f7\n"
			"fsrl.pi f25, f14, f31\n"
			"fsgnjn.s f31, f31, f12\n"
			"fsgnjn.s f2, f12, f9\n"
			"fadd.s f5, f31, f26, rmm\n"
			"bltu x5, x19, 1f\n"
			"1:\n"
			"fmax.s f12, f10, f25\n"
			"fsrl.pi f10, f14, f28\n"
			"fsrl.pi f21, f21, f16\n"
			"blt x24, x11, 1f\n"
			"1:\n"
			"fsrl.pi f10, f10, f25\n"
			"fadd.s f21, f8, f10, rup\n"
			"faddi.pi f25, f2, 0x175\n"
			"fsgnjn.s f26, f10, f26\n"
			"fsrli.pi f2, f9, 0x7\n"
			"bne x8, x23, 1f\n"
			"1:\n"
			"fexp.ps f21, f29\n"
			"blt x4, x13, 1f\n"
			"1:\n"
			"faddi.pi f8, f21, 0x1c5\n"
			"faddi.pi f25, f14, 0x30\n"
			"fadd.s f7, f8, f7, rtz\n"
			"blt x20, x9, 1f\n"
			"1:\n"
			"fsrl.pi f5, f25, f7\n"
			"fsrli.pi f9, f26, 0x3\n"
			"fsgnjn.s f26, f31, f12\n"
			"feq.pi f29, f12, f8\n"
			"fmul.s f8, f28, f25, rdn\n"
			"bne x19, x12, 1f\n"
			"1:\n"
			"fsrl.pi f31, f14, f21\n"
			"bgeu x3, x3, 1f\n"
			"1:\n"
			"fmul.s f31, f31, f21, rdn\n"
			"fsrli.pi f5, f12, 0xa\n"
			"fsrli.pi f2, f9, 0xe\n"
			"fexp.ps f31, f8\n"
			"faddi.pi f2, f7, 0xc2\n"
			"bltu x18, x12, 1f\n"
			"1:\n"
			"fexp.ps f16, f29\n"
			"faddi.pi f12, f16, 0x166\n"
			"fexp.ps f29, f29\n"
			"blt x8, x24, 1f\n"
			"1:\n"
			"faddi.pi f10, f10, 0xb5\n"
			"fsgnjn.s f21, f8, f10\n"
			"fmul.s f2, f7, f8, rmm\n"
			"feq.pi f9, f14, f21\n"
			"fsgnjn.s f14, f28, f9\n"
			"bne x17, x6, 1f\n"
			"1:\n"
			"fmax.s f8, f8, f21\n"
			"faddi.pi f26, f8, 0x1ba\n"
			"fmul.s f10, f15, f14, rmm\n"
			"fmax.s f7, f12, f12\n"
			"fexp.ps f5, f9\n"
			"fsrli.pi f9, f25, 0x0\n"
			"fexp.ps f2, f5\n"
			"fsrl.pi f14, f10, f28\n"
			"fexp.ps f15, f21\n"
			"fmul.s f12, f14, f12, rtz\n"
			"faddi.pi f12, f21, 0x1d7\n"
			"bne x11, x20, 1f\n"
			"1:\n"
			"fsrl.pi f16, f26, f16\n"
			"fsrli.pi f5, f31, 0xe\n"
			"beq x8, x25, 1f\n"
			"1:\n"
			"fexp.ps f8, f8\n"
			"fmax.s f16, f7, f8\n"
			"fadd.s f12, f12, f25, rne\n"
			"fsgnjn.s f26, f12, f28\n"
			"fmax.s f26, f26, f5\n"
			"beq x20, x26, 1f\n"
			"1:\n"
			"feq.pi f29, f15, f29\n"
			"faddi.pi f2, f16, 0x78\n"
			"faddi.pi f9, f10, 0x26\n"
			"fmul.s f8, f12, f15, rdn\n"
			"fadd.s f25, f25, f12, rne\n"
			"fexp.ps f26, f7\n"
			"blt x7, x26, 1f\n"
			"1:\n"
			"fsrl.pi f7, f14, f12\n"
			"bltu x28, x16, 1f\n"
			"1:\n"
			"faddi.pi f5, f5, 0x93\n"
			"fsrli.pi f10, f14, 0x1\n"
			"addi x21, x21, -1\n"
			"bne x21, x0, LBL_SEQID_0_LOOP_SEQ_HID_0\n"
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_0\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_1_FP_L_SCATTER_ID_2_HID_0:\n"
			"li x15, 0x8144122250\n" 
			"la x11, mem_ptr_offset\n"
			"flw.ps f10, 28(x11)\n"
			"fscbl.ps f25, f10 (x15)\n" 
			"fsgnjx.s f29, f26, f26\n"
			"fsll.pi f28, f14, f25\n"
			"fcvt.ps.pwu f15, f31, rne\n"
			"fadd.s f2, f2, f15, rne\n"
			"fsgnjn.s f25, f26, f2\n"
			"subw x28, x15, x16\n"
			"mul x20, x28, x12\n"
			"bge x30, x13, 1f\n"
			"1:\n"
			"remw x16, x16, x11\n"
			"fmulhu.pi f10, f10, f10\n"
			"fmvs.x.ps x17, f5, 4\n"
			"LBL_SEQID_1_INT_AMO_ID_3_HID_0:\n"
			"li x22, 0x8231060808\n" 
			"amoandg.w x12, x27, (x22)\n" 
			"fsetm.pi m6, f2\n"
			"fcvt.s.w f29, x17, rmm\n"
			"beq x24, x25, 1f\n"
			"1:\n"
			"fmv.s.x f16, x16\n"
			"divw x16, x20, x22\n"
			"addw x12, x12, x7\n"
			"bge x23, x16, 1f\n"
			"1:\n"
			"fmulh.pi f10, f15, f16\n"
			"fcvt.ps.f16 f2, f29\n"
			"sraw x10, x15, x10\n"
			"fandi.pi f12, f28, 0xb\n"
			"feq.ps f25, f28, f25\n"
			"LBL_SEQID_1_INT_AMO_ID_4_HID_0:\n"
			"li x27, 0x8259b80030\n" 
			"amoaddl.d x12, x22, (x27)\n" 
			"fle.pi f12, f14, f21\n"
			"fandi.pi f29, f5, 0xc5\n"
			"fand.pi f14, f9, f16\n"
			"fmin.pi f9, f9, f15\n"
			"fmin.s f12, f12, f10\n"
			"for.pi f9, f12, f10\n"
			"fcvt.ps.pw f21, f21, rmm\n"
			"fsub.s f7, f7, f21, rne\n"
			"fsub.ps f21, f31, f21, rne\n"
			"fsrl.pi f29, f14, f14\n"
			"beq x23, x12, 1f\n"
			"1:\n"
			"LBL_SEQID_1_INT_AMO_ID_5_HID_0:\n"
			"li x12, 0x8253d60010\n" 
			"amomaxul.w x10, x27, (x12)\n" 
			"fcmovm.ps f28, f25, f5\n"
			"fmadd.ps f16, f2, f28, f29, rtz\n"
			"fexp.ps f14, f14\n"
			"fcvt.ps.pw f28, f12, rmm\n"
			"fnot.pi f9, f9\n"
			"fle.pi f5, f2, f5\n"
			"blt x7, x13, 1f\n"
			"1:\n"
			"fmin.pi f8, f8, f12\n"
			"ffrc.ps f10, f21\n"
			"fcmovm.ps f26, f5, f26\n"
			"bgeu x3, x2, 1f\n"
			"1:\n"
			"fltu.pi f25, f5, f2\n"
			"LBL_SEQID_1_FP_AMO_ID_6_HID_0:\n"
			"li x10, 0x824d1a0024\n" 
			"la x15, mem_ptr_offset\n"
			"flw.ps f28, 288(x15)\n"
			"famomaxl.ps f15, f28 (x10)\n" 
			"feq.pi f12, f16, f12\n"
			"fcvt.pwu.ps f16, f16, rne\n"
			"remu x20, x22, x28\n"
			"slli x28, x12, 2\n"
			"fsub.ps f25, f25, f9, rne\n"
			"fsrl.pi f2, f16, f10\n"
			"fmulhu.pi f10, f25, f10\n"
			"sraw x15, x10, x16\n"
			"faddi.pi f16, f7, 0x156\n"
			"feq.pi f12, f16, f16\n"
			"LBL_SEQID_1_FP_AMO_ID_7_HID_0:\n"
			"li x28, 0x8145522410\n" 
			"la x7, mem_ptr_offset\n"
			"flw.ps f5, 224(x7)\n"
			"famominl.pi f28, f5 (x28)\n" 
			"li x5, 0xf002\n"
			"csrw tensor_mask, x5\n"	
			"and x15, x12, x27\n"
			"fsgnj.s f21, f10, f28\n"
			"fmulh.pi f2, f2, f25\n"
			"andi x22, x20, 0x1a\n"
			"fnmsub.s f16, f26, f16, f12, rmm\n"
			"bge x4, x22, 1f\n"
			"1:\n"
			"fmulhu.pi f8, f9, f2\n"
			"fcvt.w.s x28, f9, rup\n"
			"bge x20, x7, 1f\n"
			"1:\n"
			"or x20, x16, x20\n"
			"slliw x28, x10, 5\n"
			"fsatu8.pi f10, f10\n"
			"bge x4, x12, 1f\n"
			"1:\n"
			"LBL_SEQID_1_INT_STORE_ID_8_HID_0:\n"
			"li x27, 0x82067a041f\n" 
			"sd x17, -1675(x27)\n" 
			"fsra.pi f2, f7, f7\n"
			"bgeu x14, x17, 1f\n"
			"1:\n"
			"fadd.s f9, f8, f2, rdn\n"
			"fcvt.pwu.ps f31, f7, rmm\n"
			"ffrc.ps f15, f15\n"
			"fcvt.pw.ps f14, f28, rup\n"
			"fsgnjn.s f16, f8, f28\n"
			"fsgnjn.ps f14, f31, f31\n"
			"bltu x0, x7, 1f\n"
			"1:\n"
			"fmaxu.pi f7, f8, f26\n"
			"bgeu x30, x17, 1f\n"
			"1:\n"
			"fmax.pi f12, f2, f12\n"
			"fsgnjx.s f31, f26, f8\n"
			"beq x16, x18, 1f\n"
			"1:\n"
			"LBL_SEQID_1_FP_L_SCATTER_ID_9_HID_0:\n"
			"li x20, 0x82567a0418\n" 
			"la x12, mem_ptr_offset\n"
			"flq2 f9, 76(x12)\n"
			"fscwl.ps f10, f9 (x20)\n" 
			"fminu.pi f12, f12, f28\n"
			"fmax.ps f7, f8, f16\n"
			"fand.pi f31, f7, f31\n"
			"bne x8, x21, 1f\n"
			"1:\n"
			"fsgnjn.ps f28, f25, f28\n"
			"fmulh.pi f15, f29, f15\n"
			"frcp.ps f29, f8\n"
			"fminu.pi f8, f12, f7\n"
			"ffrc.ps f31, f2\n"
			"fmul.pi f12, f28, f8\n"
			"fexp.ps f25, f31\n"
			"LBL_SEQID_1_INT_AMO_ID_10_HID_0:\n"
			"li x16, 0x8248a45ab4\n" 
			"amomaxul.w x17, x28, (x16)\n" 
			"fsrl.pi f15, f29, f31\n"
			"fsra.pi f21, f29, f28\n"
			"blt x3, x7, 1f\n"
			"1:\n"
			"fpackrepb.pi f12, f31\n"
			"fsgnj.ps f12, f9, f29\n"
			"bne x8, x12, 1f\n"
			"1:\n"
			"fslli.pi f21, f9, 0xd\n"
			"fle.ps f29, f9, f9\n"
			"fsll.pi f5, f9, f28\n"
			"bge x25, x20, 1f\n"
			"1:\n"
			"fcvt.ps.pw f10, f9, rne\n"
			"fsra.pi f12, f9, f29\n"
			"bltu x23, x28, 1f\n"
			"1:\n"
			"fsub.pi f10, f31, f7\n"
			"LBL_SEQID_1_INT_STORE_ID_11_HID_0:\n"
			"li x17, 0x810e392064\n" 
			"sh x11, -1234(x17)\n" 
			"fmin.pi f26, f14, f26\n"
			"fsub.s f21, f2, f28, rup\n"
			"bgeu x29, x12, 1f\n"
			"1:\n"
			"flt.ps f8, f12, f8\n"
			"mulw x12, x15, x12\n"
			"ebreak\n"
			"maskor m6, m1, m2\n"
			"masknot m2, m6\n"
			"srl x22, x20, x11\n"
			"blt x4, x2, 1f\n"
			"1:\n"
			"subw x15, x16, x10\n"
			"fmsub.s f15, f21, f10, f2, rne\n"
			"LBL_SEQID_1_FP_G_SCATTER_ID_12_HID_0:\n"
			"li x20, 0x813e2002a8\n" 
			"la x17, mem_ptr_offset\n"
			"flq2 f15, 200(x17)\n"
			"fscwg.ps f21, f15 (x20)\n" 
			"srai x16, x7, 5\n"
			"fbci.pi f21, 0x71204\n"
			"fmaxu.pi f14, f14, f15\n"
			"remw x11, x11, x17\n"
			"sraw x28, x20, x28\n"
			"fadd.pi f25, f16, f31\n"
			"bne x10, x8, 1f\n"
			"1:\n"
			"ecall\n"
			"fle.pi f8, f9, f28\n"
			"fcvt.pwu.ps f31, f2, rtz\n"
			"fsat8.pi f26, f15\n"
			"LBL_SEQID_1_INT_AMO_ID_13_HID_0:\n"
			"li x7, 0x8248a3f010\n" 
			"amoswapl.d x27, x17, (x7)\n" 
			"sra x11, x11, x7\n"
			"beq x16, x3, 1f\n"
			"1:\n"
			"remu x22, x20, x17\n"
			"blt x22, x30, 1f\n"
			"1:\n"
			"packb x28, x11, x16\n"
			"sra x17, x28, x11\n"
			"fcvt.w.s x17, f26, rne\n"
			"blt x26, x6, 1f\n"
			"1:\n"
			"divw x12, x7, x15\n"
			"fpackrepb.pi f7, f14\n"
			"srlw x22, x22, x12\n"
			"fsrai.pi f25, f5, 0x6\n"
			"feqm.ps m5, f5, f5\n"
			"LBL_SEQID_1_INT_G_STORE_ID_14_HID_0:\n"
			"li x16, 0x812774c9c0\n" 
			"shg x27, (x16)\n" 
			"fsgnjn.s f21, f21, f7\n"
			"fsrl.pi f8, f14, f31\n"
			"fxor.pi f14, f7, f14\n"
			"fsra.pi f7, f7, f7\n"
			"fsat8.pi f21, f31\n"
			"fmadd.ps f16, f29, f29, f7, rmm\n"
			"fmul.ps f14, f14, f31, rmm\n"
			"fcmovm.ps f28, f31, f14\n"
			"fswizz.ps f16, f31, 0x17\n"
			"fxor.pi f7, f7, f29\n"
			"LBL_SEQID_1_FP_AMO_ID_15_HID_0:\n"
			"li x12, 0x8249ec77d4\n" 
			"la x10, mem_ptr_offset\n"
			"flw.ps f5, 416(x10)\n"
			"famomaxl.ps f16, f5 (x12)\n" 
			"mova.x.m x10\n"
			"bltu x16, x8, 1f\n"
			"1:\n"
			"frcp.ps f10, f10\n"
			"bltu x12, x17, 1f\n"
			"1:\n"
			"fcvt.s.wu f25, x20, rdn\n"
			"bgeu x13, x12, 1f\n"
			"1:\n"
			"fmvs.x.ps x7, f8, 3\n"
			"andi x11, x17, 0x1a\n"
			"bge x13, x16, 1f\n"
			"1:\n"
			"faddi.pi f21, f26, 0x41\n"
			"bne x23, x4, 1f\n"
			"1:\n"
			"feq.pi f26, f9, f29\n"
			"bge x30, x0, 1f\n"
			"1:\n"
			"feq.ps f8, f15, f10\n"
			"fadd.ps f2, f25, f2, rtz\n"
			"bge x10, x6, 1f\n"
			"1:\n"
			"andi x16, x22, 0x9\n"
			"bgeu x1, x18, 1f\n"
			"1:\n"
			"LBL_SEQID_1_INT_AMO_ID_16_HID_0:\n"
			"li x10, 0x8147260060\n" 
			"amoswapl.d x16, x28, (x10)\n" 
			"fcvt.ps.pwu f25, f10, rmm\n"
			"fmsub.s f16, f25, f2, f25, rmm\n"
			"fsub.ps f8, f8, f31, rdn\n"
			"fmulh.pi f8, f12, f8\n"
			"bgeu x15, x22, 1f\n"
			"1:\n"
			"flt.pi f5, f8, f21\n"
			"frsq.ps f15, f2\n"
			"bge x9, x4, 1f\n"
			"1:\n"
			"fnot.pi f7, f25\n"
			"fmul.ps f29, f10, f29, rne\n"
			"fsat8.pi f14, f8\n"
			"fclass.ps f28, f25\n"
			"LBL_SEQID_1_FP_L_SCATTER_ID_17_HID_0:\n"
			"li x17, 0x81404807e0\n" 
			"la x20, mem_ptr_offset\n"
			"flq2 f26, 96(x20)\n"
			"fscwl.ps f26, f26 (x17)\n" 
			"fsub.s f10, f7, f31, rup\n"
			"fmul.pi f7, f10, f7\n"
			"fmul.s f25, f7, f31, rdn\n"
			"fadd.s f12, f31, f7, rmm\n"
			"bne x31, x10, 1f\n"
			"1:\n"
			"frsq.ps f31, f31\n"
			"fmaxu.pi f5, f7, f31\n"
			"fnmadd.s f31, f31, f10, f10, rtz\n"
			"fsub.s f29, f10, f10, rdn\n"
			"fnmadd.ps f10, f10, f31, f7, rne\n"
			"fadd.s f29, f31, f7, rdn\n"
			"LBL_SEQID_1_INT_AMO_ID_18_HID_0:\n"
			"li x28, 0x82567a041c\n" 
			"amoorl.w x27, x16, (x28)\n" 
			"fmul.pi f29, f9, f8\n"
			"fnmadd.s f7, f26, f5, f28, rup\n"
			"bge x3, x5, 1f\n"
			"1:\n"
			"fmin.pi f12, f2, f28\n"
			"fmin.ps f9, f26, f2\n"
			"fnmsub.s f26, f26, f2, f10, rne\n"
			"fsub.pi f29, f5, f29\n"
			"bltu x21, x13, 1f\n"
			"1:\n"
			"fmax.s f10, f5, f29\n"
			"fmadd.s f5, f10, f8, f9, rtz\n"
			"fsgnjx.ps f29, f28, f31\n"
			"fsrli.pi f16, f28, 0x0\n"
			"LBL_SEQID_1_INT_G_STORE_ID_19_HID_0:\n"
			"li x10, 0x8130aa01c8\n" 
			"shg x17, (x10)\n" 
			"fsrl.pi f9, f29, f26\n"
			"beq x14, x17, 1f\n"
			"1:\n"
			"fmin.ps f2, f26, f16\n"
			"fmax.pi f12, f26, f14\n"
			"blt x23, x2, 1f\n"
			"1:\n"
			"fsgnjx.s f10, f8, f29\n"
			"fle.pi f2, f14, f16\n"
			"fsub.s f25, f26, f14, rdn\n"
			"fmul.ps f2, f29, f14, rne\n"
			"fsgnj.s f25, f26, f16\n"
			"for.pi f2, f25, f26\n"
			"fcvt.f16.ps f10, f9\n"
			"LBL_SEQID_1_INT_L_STORE_ID_20_HID_0:\n"
			"li x15, 0x824533bb50\n" 
			"shl x11, (x15)\n" 
			"fslli.pi f2, f14, 0x1\n"
			"fmax.pi f21, f16, f25\n"
			"feq.pi f2, f2, f28\n"
			"fslli.pi f14, f26, 0xf\n"
			"bltu x4, x0, 1f\n"
			"1:\n"
			"fcvt.ps.pw f16, f28, rtz\n"
			"fltu.pi f8, f8, f2\n"
			"fmul.ps f9, f25, f14, rne\n"
			"fmul.s f25, f16, f29, rup\n"
			"beq x12, x28, 1f\n"
			"1:\n"
			"fsgnjx.ps f15, f5, f7\n"
			"ffrc.ps f21, f28\n"
			"LBL_SEQID_1_FP_L_SCATTER_ID_21_HID_0:\n"
			"li x7, 0x81404807e0\n" 
			"la x20, mem_ptr_offset\n"
			"flw.ps f5, 76(x20)\n"
			"fscwl.ps f28, f5 (x7)\n" 
			"fpackrepb.pi f26, f25\n"
			"fcvt.pwu.ps f25, f15, rne\n"
			"ffrc.ps f31, f25\n"
			"fltu.pi f5, f5, f26\n"
			"fadd.ps f5, f8, f10, rtz\n"
			"fle.pi f15, f15, f15\n"
			"fmax.pi f15, f8, f5\n"
			"fmul.pi f26, f10, f8\n"
			"fcvt.ps.pw f26, f9, rne\n"
			"bgeu x0, x12, 1f\n"
			"1:\n"
			"ffrc.ps f10, f9\n"
			"LBL_SEQID_1_CSR_ID_22_HID_0:\n"
			"csrr x0, gsc_progress\n"
			"fsub.ps f9, f21, f9, rup\n"
			"bltu x24, x28, 1f\n"
			"1:\n"
			"fcvt.pwu.ps f21, f12, rup\n"
			"fsgnj.ps f9, f9, f9\n"
			"fslli.pi f9, f12, 0x2\n"
			"fsgnjn.ps f21, f9, f21\n"
			"fmadd.s f29, f12, f21, f21, rne\n"
			"fmulhu.pi f10, f9, f9\n"
			"fmul.pi f12, f21, f21\n"
			"bgeu x21, x0, 1f\n"
			"1:\n"
			"fminu.pi f10, f12, f9\n"
			"faddi.pi f9, f12, 0xa4\n"
			"LBL_SEQID_1_INT_AMO_ID_23_HID_0:\n"
			"li x15, 0x82422e0030\n" 
			"amomaxl.d x27, x7, (x15)\n" 
			"fmul.pi f5, f28, f12\n"
			"fsgnj.s f29, f28, f15\n"
			"fsrl.pi f14, f28, f12\n"
			"blt x14, x17, 1f\n"
			"1:\n"
			"fcvt.ps.f16 f14, f12\n"
			"flt.pi f7, f2, f15\n"
			"fmaxu.pi f29, f28, f2\n"
			"fsgnj.ps f14, f2, f2\n"
			"fadd.s f29, f28, f12, rtz\n"
			"fnmadd.s f7, f28, f2, f12, rmm\n"
			"fsgnjn.ps f15, f29, f15\n"
			"LBL_SEQID_1_INT_AMO_ID_24_HID_0:\n"
			"li x22, 0x8144122254\n" 
			"amomaxul.w x27, x12, (x22)\n" 
			"maskpopcz x15, m6\n"
			"maskxor m5, m5, m6\n"
			"auipc x17, 0xc\n"
			"flem.ps m1, f2, f16\n"
			"fswizz.ps f5, f16, 0x19\n"
			"fcmovm.ps f25, f26, f16\n"
			"ffrc.ps f5, f16\n"
			"bne x20, x29, 1f\n"
			"1:\n"
			"fmul.pi f29, f2, f16\n"
			"fsgnjx.s f25, f16, f16\n"
			"bne x31, x1, 1f\n"
			"1:\n"
			"fmin.ps f25, f26, f16\n"
			"LBL_SEQID_1_CSR_ID_25_HID_0:\n"
			"csrr x0, mscratch\n"
			"fmsub.ps f10, f8, f14, f2, rmm\n"
			"srl x7, x15, x28\n"
			"fnmsub.ps f26, f2, f2, f14, rmm\n"
			"fltm.pi m1, f14, f2\n"
			"fcvt.pwu.ps f10, f14, rne\n"
			"bne x0, x27, 1f\n"
			"1:\n"
			"fnmadd.s f10, f2, f14, f14, rdn\n"
			"beq x10, x24, 1f\n"
			"1:\n"
			"masknot m1, m5\n"
			"sltiu x11, x28, 0xc\n"
			"bge x19, x30, 1f\n"
			"1:\n"
			"fcvt.ps.f16 f8, f8\n"
			"ffrc.ps f10, f14\n"
			"LBL_SEQID_1_INT_AMO_ID_26_HID_0:\n"
			"li x28, 0x8132dd6008\n" 
			"amoandg.d x12, x17, (x28)\n" 
			"srli x28, x11, 1\n"
			"beq x22, x10, 1f\n"
			"1:\n"
			"fmaxu.pi f14, f28, f28\n"
			"flt.s x16, f2, f29\n"
			"div x27, x27, x16\n"
			"fsgnjx.s f31, f15, f14\n"
			"fnmsub.s f14, f15, f29, f21, rmm\n"
			"fsgnj.ps f9, f9, f9\n"
			"fence\n"
			"fmul.s f15, f29, f2, rtz\n"
			"fround.ps f31, f28, rtz\n"
			"LBL_SEQID_1_FP_L_SCATTER_ID_27_HID_0:\n"
			"li x20, 0x8253d60010\n" 
			"la x22, mem_ptr_offset\n"
			"flw.ps f5, 112(x22)\n"
			"fscbl.ps f26, f5 (x20)\n" 
			"fsrl.pi f25, f25, f21\n"
			"fmulh.pi f26, f9, f9\n"
			"fmulhu.pi f26, f25, f2\n"
			"bltu x9, x8, 1f\n"
			"1:\n"
			"fmsub.ps f8, f26, f5, f26, rne\n"
			"fadd.pi f12, f2, f15\n"
			"beq x20, x13, 1f\n"
			"1:\n"
			"fcvt.ps.pwu f15, f8, rne\n"
			"frsq.ps f7, f5\n"
			"bgeu x10, x31, 1f\n"
			"1:\n"
			"fsat8.pi f12, f21\n"
			"fsgnj.ps f9, f25, f9\n"
			"feq.ps f21, f9, f2\n"
			"LBL_SEQID_1_INT_AMO_ID_28_HID_0:\n"
			"li x20, 0x8248a3f010\n" 
			"amomaxl.d x16, x22, (x20)\n" 
			"frsq.ps f28, f25\n"
			"fround.ps f31, f31, rup\n"
			"fcmovm.ps f28, f21, f2\n"
			"fmadd.s f28, f7, f28, f16, rup\n"
			"bltu x19, x19, 1f\n"
			"1:\n"
			"fmax.pi f2, f15, f9\n"
			"bge x12, x20, 1f\n"
			"1:\n"
			"fmin.pi f5, f26, f2\n"
			"bltu x10, x1, 1f\n"
			"1:\n"
			"fmul.ps f16, f8, f26, rmm\n"
			"ffrc.ps f9, f26\n"
			"fsin.ps f5, f5\n"
			"fmul.ps f9, f9, f25, rtz\n"
			"LBL_SEQID_1_FP_GATHER_ID_29_HID_0:\n"
			"li x12, 0x8201060808\n" 
			"li x27, 0xedb7219b5000ecfa\n"
			"fsc32w.ps f25, x27 (x12)\n"
			"fg32b.ps f2, x27 (x12)\n" 
			"fsll.pi f5, f5, f16\n"
			"fxor.pi f31, f5, f31\n"
			"bge x7, x10, 1f\n"
			"1:\n"
			"fmul.ps f10, f5, f16, rtz\n"
			"flog.ps f7, f16\n"
			"fmin.ps f10, f5, f5\n"
			"fand.pi f31, f31, f5\n"
			"flt.pi f5, f16, f5\n"
			"fminu.pi f16, f5, f5\n"
			"fsub.pi f5, f31, f31\n"
			"fmul.ps f25, f5, f31, rup\n"
			"LBL_SEQID_1_INT_STORE_ID_30_HID_0:\n"
			"li x12, 0x810a7a602d\n" 
			"sh x27, -172(x12)\n" 
			"fcvt.f16.ps f9, f9\n"
			"blt x27, x18, 1f\n"
			"1:\n"
			"fcmov.ps f10, f9, f8, f14\n"
			"fsub.ps f31, f28, f21, rmm\n"
			"fltu.pi f31, f16, f31\n"
			"frsq.ps f9, f31\n"
			"bgeu x14, x9, 1f\n"
			"1:\n"
			"fpackreph.pi f8, f21\n"
			"fmsub.ps f25, f31, f31, f31, rtz\n"
			"fnmsub.s f28, f5, f28, f28, rup\n"
			"blt x31, x25, 1f\n"
			"1:\n"
			"fmin.s f9, f5, f16\n"
			"fsgnj.s f10, f16, f5\n"
			"LBL_SEQID_1_FP_GATHER_ID_31_HID_0:\n"
			"li x15, 0x8208a3f016\n" 
			"li x20, 0xa25804e68742c134\n"
			"fsc32w.ps f25, x20 (x15)\n"
			"fg32b.ps f15, x20 (x15)\n" 
			"ffrc.ps f31, f2\n"
			"fnmsub.ps f5, f5, f12, f2, rdn\n"
			"fsgnjn.s f2, f21, f5\n"
			"fcmov.ps f26, f28, f26, f26\n"
			"ffrc.ps f16, f7\n"
			"fnmadd.ps f31, f21, f7, f28, rmm\n"
			"fsgnjx.s f16, f25, f25\n"
			"fsgnjx.s f5, f28, f5\n"
			"fmin.pi f9, f25, f28\n"
			"fcvt.pw.ps f28, f7, rtz\n"
			"LBL_SEQID_1_FP_L_SCATTER_ID_32_HID_0:\n"
			"li x17, 0x82567a0418\n" 
			"la x12, mem_ptr_offset\n"
			"flw.ps f14, 16(x12)\n"
			"fscbl.ps f15, f14 (x17)\n" 
			"fsra.pi f7, f29, f14\n"
			"fswizz.ps f28, f7, 0x18\n"
			"ffrc.ps f16, f7\n"
			"divuw x22, x12, x22\n"
			"blt x7, x25, 1f\n"
			"1:\n"
			"fnmsub.ps f31, f2, f2, f15, rup\n"
			"maskxor m1, m5, m6\n"
			"bge x14, x3, 1f\n"
			"1:\n"
			"sub x22, x22, x28\n"
			"fsgnj.ps f9, f31, f21\n"
			"fmvs.x.ps x16, f16, 7\n"
			"bne x11, x25, 1f\n"
			"1:\n"
			"fcvt.wu.s x27, f15, rmm\n"
			"bge x27, x19, 1f\n"
			"1:\n"
			"LBL_SEQID_1_FP_AMO_ID_33_HID_0:\n"
			"li x20, 0x8150aa01c8\n" 
			"la x7, mem_ptr_offset\n"
			"flw.ps f25, 416(x7)\n"
			"famoswapl.pi f26, f25 (x20)\n" 
			"fmulhu.pi f8, f26, f7\n"
			"fle.pi f28, f2, f29\n"
			"blt x5, x7, 1f\n"
			"1:\n"
			"fsrl.pi f2, f7, f29\n"
			"fcmov.ps f12, f15, f5, f14\n"
			"bge x17, x0, 1f\n"
			"1:\n"
			"fcmovm.ps f10, f8, f10\n"
			"fsgnjn.s f10, f26, f25\n"
			"flt.pi f7, f31, f9\n"
			"fmadd.s f25, f2, f8, f26, rmm\n"
			"blt x1, x14, 1f\n"
			"1:\n"
			"fle.pi f7, f15, f9\n"
			"bne x4, x21, 1f\n"
			"1:\n"
			"fsrai.pi f8, f8, 0x3\n"
			"csrw tensor_error, zero\n" 
			"csrwi tensor_wait, 0x6\n" 
			"fence\n"
			"li x5,  0xc0000000000000f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc0000000000004f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc0000000000008f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc000000000000cf\n" 
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
			"fsatu8.pi f28, f29\n"
			"feq.ps f29, f31, f26\n"
			"fmul.pi f28, f12, f7\n"
			"fmaxu.pi f9, f7, f16\n"
			"bne x29, x19, 1f\n"
			"1:\n"
			"fsgnjx.s f8, f25, f9\n"
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ID_843294_HID_0:\n"
			"li x7, ((0x1 << TENSOR_STORE_REG_STRIDE) | (0xa << TENSOR_STORE_START_REG) | (0x0 << TENSOR_STORE_STORE_COLS) | (0xd << TENSOR_STORE_AROWS) | (0x0 << TENSOR_STORE_COOP_STORE) | (0x0 << TENSOR_STORE_OPC_0) | (0x819412225 << TENSOR_STORE_VADDR) | 0 )\n" 
			"li x31, (0x80 << 4)\n"
			"csrw tensor_store, x7\n"	
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ID_673405_HID_0:\n"
			"li x7, ((0x0 << TENSOR_STORE_REG_STRIDE) | (0x19 << TENSOR_STORE_START_REG) | (0x3 << TENSOR_STORE_STORE_COLS) | (0x4 << TENSOR_STORE_AROWS) | (0x0 << TENSOR_STORE_COOP_STORE) | (0x0 << TENSOR_STORE_OPC_0) | (0x8192dd600 << TENSOR_STORE_VADDR) | 0 )\n" 
			"li x31, (0x80 << 4)\n"
			"csrw tensor_store, x7\n"	
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ID_92127_HID_0:\n"
			"li x7, ((0x0 << TENSOR_STORE_REG_STRIDE) | (0x12 << TENSOR_STORE_START_REG) | (0x3 << TENSOR_STORE_STORE_COLS) | (0x7 << TENSOR_STORE_AROWS) | (0x0 << TENSOR_STORE_COOP_STORE) | (0x0 << TENSOR_STORE_OPC_0) | (0x818552241 << TENSOR_STORE_VADDR) | 0 )\n" 
			"li x31, (0xc0 << 4)\n"
			"csrw tensor_store, x7\n"	
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ID_518238_HID_0:\n"
			"li x7, ((0x0 << TENSOR_STORE_REG_STRIDE) | (0x18 << TENSOR_STORE_START_REG) | (0x0 << TENSOR_STORE_STORE_COLS) | (0x3 << TENSOR_STORE_AROWS) | (0x0 << TENSOR_STORE_COOP_STORE) | (0x0 << TENSOR_STORE_OPC_0) | (0x8176d8f00 << TENSOR_STORE_VADDR) | 0 )\n" 
			"li x31, (0x40 << 4)\n"
			"csrw tensor_store, x7\n"	
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ID_868715_HID_0:\n"
			"li x7, ((0x2 << TENSOR_STORE_REG_STRIDE) | (0x1d << TENSOR_STORE_START_REG) | (0x0 << TENSOR_STORE_STORE_COLS) | (0xf << TENSOR_STORE_AROWS) | (0x0 << TENSOR_STORE_COOP_STORE) | (0x0 << TENSOR_STORE_OPC_0) | (0x80001a9 << TENSOR_STORE_VADDR) | 0 )\n" 
			"li x31, (0x80 << 4)\n"
			"csrw tensor_store, x7\n"	
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ID_188306_HID_0:\n"
			"li x7, ((0x3 << TENSOR_STORE_REG_STRIDE) | (0x4 << TENSOR_STORE_START_REG) | (0x1 << TENSOR_STORE_STORE_COLS) | (0xa << TENSOR_STORE_AROWS) | (0x0 << TENSOR_STORE_COOP_STORE) | (0x0 << TENSOR_STORE_OPC_0) | (0x8299ec77d << TENSOR_STORE_VADDR) | 0 )\n" 
			"li x31, (0x20 << 4)\n"
			"csrw tensor_store, x7\n"	
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ID_951712_HID_0:\n"
			"li x7, ((0x0 << TENSOR_STORE_REG_STRIDE) | (0x6 << TENSOR_STORE_START_REG) | (0x3 << TENSOR_STORE_STORE_COLS) | (0xc << TENSOR_STORE_AROWS) | (0x0 << TENSOR_STORE_COOP_STORE) | (0x0 << TENSOR_STORE_OPC_0) | (0x827032979 << TENSOR_STORE_VADDR) | 0 )\n" 
			"li x31, (0xc0 << 4)\n"
			"csrw tensor_store, x7\n"	
			"csrwi tensor_wait, 8\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_0\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_1_TLD_XMA_STR_SEQ_ITER_0_HID_0:\n"
			"la x31, self_check_32\n"
			"li x5, 0x80000000000000\n"
			"add x5, x5, x31\n"
			"li x6, 0x10000000000005\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x2 << TENSOR_FMA32_B_NUM_COLS) | (0x0 << TENSOR_FMA32_A_NUM_ROWS) | (0x5 << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x1 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1b << TENSOR_FMA32_SCP_LOC_B) | (0x4 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x9, ((0xf << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0x0 << TENSOR_QUANT_QUANT_ROW) | (0x1d << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x6 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x10, ((0x1 << TENSOR_STORE_REG_STRIDE) | (0x1a << TENSOR_STORE_START_REG) | (0x3 << TENSOR_STORE_STORE_COLS) | (0x1 << TENSOR_STORE_AROWS) | (0x0 << TENSOR_STORE_COOP_STORE) | (0x0 << TENSOR_STORE_OPC_0) | (0x80003b4 << TENSOR_STORE_VADDR) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0x6\n"
			"bne x5, x31, LBL_FAIL_HID_0\n"
			"csrw tensor_quant, x9\n"	
			"li x31, (0xc0 << 4)\n"
			"csrw tensor_store, x10\n"	
			"csrwi tensor_wait, 10\n"
			"csrwi tensor_wait, 8\n"
			"LBL_SEQID_1_TLD_XMA_STR_SEQ_ITER_1_HID_0:\n"
			"li x5, ((0x0 << TENSOR_LOAD_T32_USE_TMASK) | (0x0 << TENSOR_LOAD_T32_USE_COOP) | (0x7 << TENSOR_LOAD_T32_OPC_111) | (0x3 << TENSOR_LOAD_T32_DST_START) | (0x205d4eba0 << TENSOR_LOAD_T32_VADDR) | (0x5 << TENSOR_LOAD_T32_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_T32_USE_TMASK) | (0x0 << TENSOR_LOAD_T32_USE_COOP) | (0x7 << TENSOR_LOAD_T32_OPC_111) | (0x1c << TENSOR_LOAD_T32_DST_START) | (0x209fd6220 << TENSOR_LOAD_T32_VADDR) | (0xc << TENSOR_LOAD_T32_NUM_LINES) | 0 )\n" 
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x0 << TENSOR_IMA_B_NUM_COLS) | (0x5 << TENSOR_IMA_A_NUM_ROWS) | (0xc << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x0 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1c << TENSOR_IMA_SCP_LOC_B) | (0x3 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x9, ((0x12 << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0x5 << TENSOR_QUANT_QUANT_ROW) | (0x26 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x8 << TENSOR_QUANT_TRANSF7) | (0x8 << TENSOR_QUANT_TRANSF6) | (0xa << TENSOR_QUANT_TRANSF5) | (0x3 << TENSOR_QUANT_TRANSF4) | (0x8 << TENSOR_QUANT_TRANSF3) | (0x4 << TENSOR_QUANT_TRANSF2) | (0x3 << TENSOR_QUANT_TRANSF1) | (0xa << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 7\n"
			"csrwi tensor_wait, 10\n"
			"LBL_SEQID_1_TLD_XMA_STR_SEQ_ITER_2_HID_0:\n"
			"li x5, ((0x0 << TENSOR_LOAD_T16_USE_TMASK) | (0x0 << TENSOR_LOAD_T16_USE_COOP) | (0x6 << TENSOR_LOAD_T16_OPC_110) | (0x1 << TENSOR_LOAD_T16_DST_START) | (0x2000186 << TENSOR_LOAD_T16_VADDR) | (0x0 << TENSOR_LOAD_T16_OFFSET) | (0x3 << TENSOR_LOAD_T16_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_SETUP_B_USE_COOP) | (0x0 << TENSOR_LOAD_SETUP_B_OPC_000) | (0x1 << TENSOR_LOAD_SETUP_B_OPC_1) | (0x209e2916a << TENSOR_LOAD_SETUP_B_VADDR) | (0x5 << TENSOR_LOAD_SETUP_B_NUM_LINES) | 0 )\n" 
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x0 << TENSOR_FMA16_B_NUM_COLS) | (0x3 << TENSOR_FMA16_A_NUM_ROWS) | (0x5 << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x1 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1e << TENSOR_FMA16_SCP_LOC_B) | (0x1 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x0 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_0\n"
			"csrw tensor_error, x0\n"
			"li x5,  0x1ff800000610007\n" 
			"csrw tensor_fma, x5\n" 
			"csrwi tensor_wait, 7\n" 
			"csrwi tensor_wait, 0x6\n" 
			"fence\n"
			"li x5,  0xc0000000000000f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc0000000000004f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc0000000000008f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc000000000000cf\n" 
			"csrw   evict_sw, x5\n" 
			"csrwi tensor_wait, 0x6\n" 
			"csrrci x0, mcache_control, 2\n"
			"csrwi tensor_wait, 0x6\n" 
			"fence\n"
			"mov.m.x m0, x0, 0xFF\n"
			"li x5, 0x40\n"
			"la x31, rand_ieee754_32\n"
			"add x31, x31, x5\n"
			"flw.ps f10, 0(x31)\n"
			"flw.ps f12, 2(x31)\n"
			"flw.ps f15, 7(x31)\n"
			"flw.ps f16, 11(x31)\n"
			"flw.ps f25, 3(x31)\n"
			"flw.ps f28, 18(x31)\n"
			"flw.ps f29, 16(x31)\n"
			"flw.ps f7, 21(x31)\n"
			"flw.ps f8, 31(x31)\n"
			"flw.ps f9, 9(x31)\n"
			"li x21, 0x3\n"
			"LBL_SEQID_1_LOOP_SEQ_HID_0:\n"
			"fcmov.ps f12, f7, f28, f8\n"
			"fmin.pi f16, f9, f7\n"
			"fmax.s f16, f16, f16\n"
			"for.pi f26, f10, f12\n"
			"fsrli.pi f5, f28, 0xc\n"
			"fcmovm.ps f16, f15, f7\n"
			"bgeu x16, x30, 1f\n"
			"1:\n"
			"fcvt.pw.ps f16, f12, rdn\n"
			"fmul.s f29, f15, f16, rdn\n"
			"fsgnjx.s f31, f15, f12\n"
			"fmax.s f9, f9, f12\n"
			"fsat8.pi f21, f29\n"
			"blt x8, x26, 1f\n"
			"1:\n"
			"fmin.ps f14, f25, f29\n"
			"fsgnjx.s f25, f8, f29\n"
			"fsra.pi f29, f15, f29\n"
			"fle.ps f31, f29, f12\n"
			"fsin.ps f16, f16\n"
			"fround.ps f12, f12, rmm\n"
			"bge x4, x20, 1f\n"
			"1:\n"
			"fsat8.pi f10, f8\n"
			"frsq.ps f28, f28\n"
			"fclass.ps f28, f25\n"
			"bne x30, x2, 1f\n"
			"1:\n"
			"fsat8.pi f9, f10\n"
			"faddi.pi f14, f29, 0x136\n"
			"fmul.ps f28, f10, f15, rne\n"
			"fadd.s f21, f16, f10, rne\n"
			"fcvt.ps.f16 f16, f16\n"
			"frcp.ps f31, f15\n"
			"fmsub.ps f25, f15, f25, f16, rne\n"
			"fmin.s f29, f15, f25\n"
			"fsub.ps f29, f29, f10, rtz\n"
			"fmadd.ps f12, f15, f7, f10, rup\n"
			"fmulh.pi f14, f7, f9\n"
			"beq x28, x25, 1f\n"
			"1:\n"
			"frsq.ps f25, f15\n"
			"fmsub.s f16, f10, f9, f16, rdn\n"
			"fsrli.pi f28, f12, 0xc\n"
			"fle.ps f12, f25, f29\n"
			"fround.ps f26, f9, rne\n"
			"beq x20, x26, 1f\n"
			"1:\n"
			"fcmovm.ps f14, f29, f8\n"
			"fsgnjn.ps f25, f10, f25\n"
			"fslli.pi f25, f29, 0xa\n"
			"fnmadd.ps f21, f15, f10, f8, rdn\n"
			"beq x22, x21, 1f\n"
			"1:\n"
			"fpackrepb.pi f14, f7\n"
			"fmul.s f25, f7, f28, rdn\n"
			"blt x26, x21, 1f\n"
			"1:\n"
			"fsgnjx.s f25, f29, f25\n"
			"fnot.pi f9, f9\n"
			"fsub.pi f16, f12, f8\n"
			"fsrl.pi f9, f9, f12\n"
			"blt x20, x8, 1f\n"
			"1:\n"
			"fsrai.pi f28, f8, 0x8\n"
			"fsgnjn.s f29, f9, f29\n"
			"fandi.pi f8, f9, 0x91\n"
			"fsgnjn.ps f29, f16, f16\n"
			"fsgnjx.ps f14, f10, f12\n"
			"fnmadd.s f29, f25, f10, f29, rmm\n"
			"fcmovm.ps f25, f16, f16\n"
			"fmax.pi f16, f12, f16\n"
			"bge x7, x13, 1f\n"
			"1:\n"
			"fmsub.ps f25, f25, f12, f12, rtz\n"
			"fadd.pi f8, f8, f15\n"
			"fslli.pi f12, f7, 0x3\n"
			"fsgnjx.s f12, f12, f10\n"
			"fpackreph.pi f5, f29\n"
			"fnot.pi f14, f16\n"
			"fsatu8.pi f26, f7\n"
			"fsra.pi f21, f10, f16\n"
			"fpackrepb.pi f12, f16\n"
			"fround.ps f21, f10, rmm\n"
			"fmax.pi f14, f8, f9\n"
			"faddi.pi f15, f12, 0x14a\n"
			"bne x2, x19, 1f\n"
			"1:\n"
			"fclass.ps f21, f8\n"
			"fnmadd.ps f8, f7, f29, f8, rmm\n"
			"fsatu8.pi f8, f8\n"
			"beq x21, x12, 1f\n"
			"1:\n"
			"fsin.ps f9, f9\n"
			"faddi.pi f10, f28, 0x1b3\n"
			"fsub.pi f26, f25, f25\n"
			"fcmov.ps f16, f15, f8, f16\n"
			"fmsub.s f29, f16, f28, f25, rup\n"
			"fnot.pi f16, f7\n"
			"fand.pi f25, f15, f25\n"
			"fsub.s f16, f7, f25, rmm\n"
			"fmsub.ps f29, f28, f9, f29, rdn\n"
			"fpackreph.pi f29, f8\n"
			"fsub.ps f28, f7, f12, rmm\n"
			"fsub.pi f16, f16, f15\n"
			"fadd.pi f28, f16, f12\n"
			"ffrc.ps f29, f29\n"
			"fxor.pi f16, f15, f16\n"
			"fsgnjn.ps f15, f10, f15\n"
			"fexp.ps f5, f28\n"
			"fmaxu.pi f5, f16, f29\n"
			"feq.ps f31, f7, f8\n"
			"fmadd.s f10, f9, f10, f25, rmm\n"
			"fadd.s f14, f25, f25, rup\n"
			"faddi.pi f21, f25, 0xb9\n"
			"fsgnjn.ps f10, f9, f16\n"
			"fnmadd.s f8, f8, f16, f15, rup\n"
			"fsrli.pi f9, f29, 0x3\n"
			"fmadd.s f26, f28, f10, f28, rdn\n"
			"fmadd.ps f14, f10, f12, f12, rdn\n"
			"fmulh.pi f25, f7, f9\n"
			"flog.ps f31, f25\n"
			"feq.pi f25, f29, f9\n"
			"fmul.pi f26, f10, f16\n"
			"fle.ps f5, f9, f7\n"
			"bne x30, x2, 1f\n"
			"1:\n"
			"flog.ps f9, f16\n"
			"flt.pi f16, f9, f7\n"
			"flog.ps f28, f10\n"
			"fltu.pi f25, f25, f10\n"
			"bge x7, x0, 1f\n"
			"1:\n"
			"fexp.ps f14, f9\n"
			"fsat8.pi f10, f10\n"
			"fsub.s f10, f10, f16, rup\n"
			"bge x2, x9, 1f\n"
			"1:\n"
			"flt.ps f29, f8, f29\n"
			"fsll.pi f9, f10, f7\n"
			"fcvt.pwu.ps f29, f29, rdn\n"
			"fcvt.ps.f16 f8, f12\n"
			"bgeu x18, x3, 1f\n"
			"1:\n"
			"fpackrepb.pi f31, f9\n"
			"fsgnj.s f8, f16, f8\n"
			"fmin.s f28, f16, f10\n"
			"flog.ps f9, f25\n"
			"fmax.pi f9, f9, f9\n"
			"frsq.ps f10, f15\n"
			"fnmsub.s f25, f8, f15, f15, rtz\n"
			"feq.pi f29, f8, f29\n"
			"fcmovm.ps f12, f15, f12\n"
			"fsin.ps f10, f12\n"
			"frcp.ps f29, f8\n"
			"fmul.s f26, f7, f7, rtz\n"
			"fsin.ps f25, f25\n"
			"fmul.s f25, f28, f7, rdn\n"
			"fpackrepb.pi f25, f29\n"
			"fcvt.ps.pw f8, f12, rtz\n"
			"fadd.ps f12, f7, f16, rne\n"
			"fpackrepb.pi f10, f29\n"
			"fsrai.pi f31, f12, 0xa\n"
			"fsgnjn.s f21, f25, f29\n"
			"fadd.ps f29, f29, f8, rne\n"
			"beq x8, x15, 1f\n"
			"1:\n"
			"fadd.pi f12, f29, f12\n"
			"fsra.pi f14, f16, f25\n"
			"fsll.pi f25, f25, f16\n"
			"fmul.s f25, f25, f28, rne\n"
			"fsrai.pi f26, f15, 0x5\n"
			"fmax.pi f16, f12, f15\n"
			"fcvt.ps.pw f9, f15, rtz\n"
			"fnmadd.s f14, f10, f25, f16, rmm\n"
			"fsat8.pi f5, f7\n"
			"fmsub.ps f5, f7, f25, f12, rmm\n"
			"fswizz.ps f25, f29, 0x1b\n"
			"fsrli.pi f10, f7, 0x4\n"
			"fsrl.pi f10, f12, f8\n"
			"fmax.pi f12, f12, f29\n"
			"fslli.pi f29, f10, 0x8\n"
			"fcvt.pw.ps f14, f16, rmm\n"
			"fsub.s f16, f10, f16, rne\n"
			"fnot.pi f12, f7\n"
			"bltu x29, x26, 1f\n"
			"1:\n"
			"fle.pi f7, f7, f8\n"
			"fmadd.ps f29, f29, f28, f29, rdn\n"
			"fnmadd.ps f29, f12, f29, f25, rdn\n"
			"fle.ps f9, f12, f9\n"
			"fadd.s f28, f16, f16, rtz\n"
			"blt x3, x13, 1f\n"
			"1:\n"
			"fcvt.pw.ps f7, f12, rtz\n"
			"fsll.pi f29, f16, f10\n"
			"bge x16, x26, 1f\n"
			"1:\n"
			"fmsub.s f14, f25, f25, f28, rdn\n"
			"fsgnj.ps f28, f25, f10\n"
			"bgeu x15, x2, 1f\n"
			"1:\n"
			"fsub.pi f31, f8, f29\n"
			"fsrl.pi f12, f28, f29\n"
			"fcvt.ps.pw f28, f7, rmm\n"
			"fmul.pi f9, f12, f12\n"
			"bltu x22, x2, 1f\n"
			"1:\n"
			"fmax.pi f29, f12, f29\n"
			"fmin.ps f16, f7, f12\n"
			"fsra.pi f9, f29, f7\n"
			"flog.ps f31, f7\n"
			"bge x8, x15, 1f\n"
			"1:\n"
			"fsgnj.ps f10, f12, f29\n"
			"fmul.ps f9, f29, f16, rdn\n"
			"fcmov.ps f10, f28, f15, f8\n"
			"bgeu x28, x18, 1f\n"
			"1:\n"
			"fmadd.s f29, f29, f10, f25, rdn\n"
			"fle.ps f16, f16, f25\n"
			"fsatu8.pi f14, f28\n"
			"fsin.ps f28, f29\n"
			"frsq.ps f25, f10\n"
			"bge x22, x17, 1f\n"
			"1:\n"
			"fle.ps f25, f15, f29\n"
			"frsq.ps f7, f7\n"
			"fsrai.pi f14, f28, 0xb\n"
			"faddi.pi f12, f28, 0x1e1\n"
			"fmin.s f9, f8, f9\n"
			"fsra.pi f12, f12, f8\n"
			"fcvt.ps.pw f10, f10, rmm\n"
			"bge x19, x14, 1f\n"
			"1:\n"
			"fadd.pi f26, f9, f15\n"
			"fle.pi f5, f12, f25\n"
			"faddi.pi f15, f9, 0x1e9\n"
			"fmin.pi f31, f8, f25\n"
			"bgeu x15, x23, 1f\n"
			"1:\n"
			"fslli.pi f29, f9, 0xe\n"
			"fltu.pi f9, f28, f9\n"
			"fmin.s f12, f12, f28\n"
			"fxor.pi f25, f9, f9\n"
			"fclass.ps f29, f9\n"
			"flt.ps f25, f7, f15\n"
			"fcvt.ps.pwu f25, f9, rup\n"
			"fmsub.ps f14, f7, f9, f8, rne\n"
			"feq.ps f26, f16, f25\n"
			"fsgnjn.s f25, f7, f9\n"
			"fmul.pi f29, f29, f16\n"
			"fsgnjn.ps f16, f12, f29\n"
			"fsub.s f16, f16, f7, rtz\n"
			"addi x21, x21, -1\n"
			"bne x21, x0, LBL_SEQID_1_LOOP_SEQ_HID_0\n"
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_0\n"
			"addi x30, x30, 1\n"
			"la x10, rand_int_32\n"
			"flw.ps f21, 576(x10)\n"
			"fpackreph.pi f28, f21\n" 
			"li x27, 0x9e0da569b135b05c\n"
			"li x11, 0xe6bd9c97a3ac31ef\n"
			"slt x7, x27, x11\n" 
			"li x5, 0x63d3\n"
			"csrw tensor_mask, x5\n"	
			"LBL_SEQID_2_M0_WRITE_ID_34_HID_0:\n"
			"fltm.ps m6, f31, f28\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x12, rand_ieee754_32\n"
			"flw.ps f16, 192(x12)\n"
			"fmax.s f15, f16, f16\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f26, 256(x7)\n"
			"flw.ps f7, 704(x7)\n"
			"fmax.s f5, f26, f7\n" 
			"la x11, rand_int_32\n"
			"flw.ps f15, 608(x11)\n"
			"fpackreph.pi f10, f15\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f9, 224(x16)\n"
			"flw.ps f16, 896(x16)\n"
			"fcmov.ps f26, f9, f16, f16\n" 
			"li x17, 0x4a34024390a43e7e\n"
			"fmv.s.x f28, x17\n" 
			"LBL_SEQID_2_M0_WRITE_ID_35_HID_0:\n"
			"feqm.ps m2, f15, f21\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x15, rand_ieee754_32\n"
			"flw.ps f14, 928(x15)\n"
			"flw.ps f21, 768(x15)\n"
			"fadd.s f31, f14, f21, rup\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f5, 960(x11)\n"
			"fmadd.s f29, f5, f5, f5, rne\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f29, 160(x10)\n"
			"fclass.ps f28, f29\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f10, 832(x7)\n"
			"flw.ps f5, 384(x7)\n"
			"fmul.s f25, f10, f5, rdn\n" 
			"la x12, rand_int_32\n"
			"flw.ps f10, 224(x12)\n"
			"flw.ps f14, 384(x12)\n"
			"feq.pi f28, f10, f14\n" 
			"li x5, 0xdb40\n"
			"csrw tensor_mask, x5\n"	
			"la x17, rand_int_32\n"
			"flw.ps f29, 320(x17)\n"
			"flw.ps f31, 608(x17)\n"
			"feq.pi f12, f29, f31\n" 
			"la x11, rand_int_32\n"
			"flw.ps f7, 992(x11)\n"
			"faddi.pi f10, f7, 0x66\n" 
			"la x16, rand_int_32\n"
			"flw.ps f9, 832(x16)\n"
			"faddi.pi f14, f9, 0x91\n" 
			"LBL_SEQID_2_M0_WRITE_ID_36_HID_0:\n"
			"maskand m6, m6, m2\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"li x10, 0xd8d774dfe4180e23\n"
			"ori x7, x10, 0x8\n" 
			"la x15, rand_int_32\n"
			"flw.ps f7, 384(x15)\n"
			"flw.ps f10, 608(x15)\n"
			"fsub.pi f5, f7, f10\n" 
			"LBL_SEQID_2_M0_WRITE_ID_37_HID_0:\n"
			"maskand m5, m2, m2\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x17, rand_ieee754_32\n"
			"flw.ps f2, 832(x17)\n"
			"flw.ps f10, 416(x17)\n"
			"fadd.s f2, f2, f10, rdn\n" 
			"la x27, rand_int_32\n"
			"flw.ps f2, 800(x27)\n"
			"flw.ps f16, 320(x27)\n"
			"fsub.pi f14, f2, f16\n" 
			"li x17, 0xc0e677541c31f73a\n"
			"li x16, 0x5dc421767f4274d5\n"
			"sraw x20, x17, x16\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f15, 896(x7)\n"
			"flw.ps f8, 768(x7)\n"
			"fadd.ps f10, f15, f8, rne\n" 
			"li x7, 0xed55af79998bbf55\n"
			"slli x17, x7, 5\n" 
			"li x11, 0x77c747f2313ce49f\n"
			"li x15, 0x487cb1c6ace9a221\n"
			"slt x27, x11, x15\n" 
			"li x5, 0x409f0f38def6fcc8\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0x7d61dc8320291c19\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"li x10, 0xc5aad6e63feb7a25\n"
			"slli x10, x10, 0\n" 
			"li x17, 0xd06c4aab1a39e913\n"
			"li x15, 0x7fc0bfc6ae8029be\n"
			"slt x20, x17, x15\n" 
			"li x12, 0x61411f2dd8789e9\n"
			"fcvt.s.w f29, x12, rup\n" 
			"la x16, rand_int_32\n"
			"flw.ps f31, 576(x16)\n"
			"fsrai.pi f31, f31, 0x4\n" 
			"li x5, 0xd42f\n"
			"csrw tensor_mask, x5\n"	
			"la x28, rand_int_32\n"
			"flw.ps f29, 64(x28)\n"
			"fsatu8.pi f12, f29\n" 
			"csrw tensor_error, zero\n" 
			"csrwi tensor_wait, 0x6\n" 
			"fence\n"
			"li x5,  0xc0000000000000f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc0000000000004f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc0000000000008f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc000000000000cf\n" 
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
			"LBL_SEQID_2_XMA_QNT_STR_SEQ_ID_963820_HID_0:\n"
			"li x7, ((0x3 << TENSOR_STORE_REG_STRIDE) | (0x7 << TENSOR_STORE_START_REG) | (0x1 << TENSOR_STORE_STORE_COLS) | (0x7 << TENSOR_STORE_AROWS) | (0x0 << TENSOR_STORE_COOP_STORE) | (0x0 << TENSOR_STORE_OPC_0) | (0x828533bb5 << TENSOR_STORE_VADDR) | 0 )\n" 
			"li x31, (0x20 << 4)\n"
			"csrw tensor_store, x7\n"	
			"LBL_SEQID_2_XMA_QNT_STR_SEQ_ID_171690_HID_0:\n"
			"li x7, ((0x2 << TENSOR_STORE_REG_STRIDE) | (0xa << TENSOR_STORE_START_REG) | (0x3 << TENSOR_STORE_STORE_COLS) | (0x4 << TENSOR_STORE_AROWS) | (0x0 << TENSOR_STORE_COOP_STORE) | (0x0 << TENSOR_STORE_OPC_0) | (0x8192dd600 << TENSOR_STORE_VADDR) | 0 )\n" 
			"li x31, (0x40 << 4)\n"
			"csrw tensor_store, x7\n"	
			"LBL_SEQID_2_XMA_QNT_STR_SEQ_ID_518371_HID_0:\n"
			"li x7, ((0x1 << TENSOR_STORE_REG_STRIDE) | (0x1b << TENSOR_STORE_START_REG) | (0x0 << TENSOR_STORE_STORE_COLS) | (0xc << TENSOR_STORE_AROWS) | (0x0 << TENSOR_STORE_COOP_STORE) | (0x0 << TENSOR_STORE_OPC_0) | (0x819a7a602 << TENSOR_STORE_VADDR) | 0 )\n" 
			"li x31, (0x40 << 4)\n"
			"csrw tensor_store, x7\n"	
			"LBL_SEQID_2_XMA_QNT_STR_SEQ_ID_220597_HID_0:\n"
			"li x7, ((0x3 << TENSOR_STORE_REG_STRIDE) | (0x2 << TENSOR_STORE_START_REG) | (0x1 << TENSOR_STORE_STORE_COLS) | (0x8 << TENSOR_STORE_AROWS) | (0x0 << TENSOR_STORE_COOP_STORE) | (0x0 << TENSOR_STORE_OPC_0) | (0x817412225 << TENSOR_STORE_VADDR) | 0 )\n" 
			"li x31, (0x80 << 4)\n"
			"csrw tensor_store, x7\n"	
			"LBL_SEQID_2_XMA_QNT_STR_SEQ_ID_576829_HID_0:\n"
			"li x7, ((0x2 << TENSOR_STORE_REG_STRIDE) | (0x5 << TENSOR_STORE_START_REG) | (0x1 << TENSOR_STORE_STORE_COLS) | (0xe << TENSOR_STORE_AROWS) | (0x0 << TENSOR_STORE_COOP_STORE) | (0x0 << TENSOR_STORE_OPC_0) | (0x819a7a602 << TENSOR_STORE_VADDR) | 0 )\n" 
			"li x31, (0x80 << 4)\n"
			"csrw tensor_store, x7\n"	
			"LBL_SEQID_2_XMA_QNT_STR_SEQ_ID_236542_HID_0:\n"
			"li x7, ((0x3 << TENSOR_STORE_REG_STRIDE) | (0x7 << TENSOR_STORE_START_REG) | (0x3 << TENSOR_STORE_STORE_COLS) | (0x2 << TENSOR_STORE_AROWS) | (0x0 << TENSOR_STORE_COOP_STORE) | (0x0 << TENSOR_STORE_OPC_0) | (0x827f58882 << TENSOR_STORE_VADDR) | 0 )\n" 
			"li x31, (0xc0 << 4)\n"
			"csrw tensor_store, x7\n"	
			"LBL_SEQID_2_XMA_QNT_STR_SEQ_ID_549832_HID_0:\n"
			"li x7, ((0x1 << TENSOR_STORE_REG_STRIDE) | (0x1c << TENSOR_STORE_START_REG) | (0x3 << TENSOR_STORE_STORE_COLS) | (0xc << TENSOR_STORE_AROWS) | (0x0 << TENSOR_STORE_COOP_STORE) | (0x0 << TENSOR_STORE_OPC_0) | (0x8268a3f01 << TENSOR_STORE_VADDR) | 0 )\n" 
			"li x31, (0xc0 << 4)\n"
			"csrw tensor_store, x7\n"	
			"LBL_SEQID_2_XMA_QNT_STR_SEQ_ID_590933_HID_0:\n"
			"li x7, ((0x1 << TENSOR_STORE_REG_STRIDE) | (0xc << TENSOR_STORE_START_REG) | (0x1 << TENSOR_STORE_STORE_COLS) | (0x5 << TENSOR_STORE_AROWS) | (0x0 << TENSOR_STORE_COOP_STORE) | (0x0 << TENSOR_STORE_OPC_0) | (0x81756ef00 << TENSOR_STORE_VADDR) | 0 )\n" 
			"li x31, (0x20 << 4)\n"
			"csrw tensor_store, x7\n"	
			"LBL_SEQID_2_XMA_QNT_STR_SEQ_ID_533453_HID_0:\n"
			"li x7, ((0x3 << TENSOR_STORE_REG_STRIDE) | (0xe << TENSOR_STORE_START_REG) | (0x3 << TENSOR_STORE_STORE_COLS) | (0xb << TENSOR_STORE_AROWS) | (0x0 << TENSOR_STORE_COOP_STORE) | (0x0 << TENSOR_STORE_OPC_0) | (0x800061b << TENSOR_STORE_VADDR) | 0 )\n" 
			"li x31, (0x80 << 4)\n"
			"csrw tensor_store, x7\n"	
			"LBL_SEQID_2_XMA_QNT_STR_SEQ_ID_218414_HID_0:\n"
			"li x7, ((0x3 << TENSOR_STORE_REG_STRIDE) | (0x6 << TENSOR_STORE_START_REG) | (0x1 << TENSOR_STORE_STORE_COLS) | (0x6 << TENSOR_STORE_AROWS) | (0x0 << TENSOR_STORE_COOP_STORE) | (0x0 << TENSOR_STORE_OPC_0) | (0x827f58882 << TENSOR_STORE_VADDR) | 0 )\n" 
			"li x31, (0x80 << 4)\n"
			"csrw tensor_store, x7\n"	
			"csrwi tensor_wait, 8\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_0\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_2_XMA_QNT_STR_SEQ_ITER_0_HID_0:\n"
			"li x6, ((0xc << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0x3 << TENSOR_QUANT_QUANT_ROW) | (0x28 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x2 << TENSOR_QUANT_TRANSF9) | (0x7 << TENSOR_QUANT_TRANSF8) | (0x6 << TENSOR_QUANT_TRANSF7) | (0x6 << TENSOR_QUANT_TRANSF6) | (0x6 << TENSOR_QUANT_TRANSF5) | (0x7 << TENSOR_QUANT_TRANSF4) | (0x6 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x7, ((0x2 << TENSOR_STORE_SCP_ENTRY_STRIDE) | (0x24 << TENSOR_STORE_SCP_START_SCP_ENTRY) | (0xa << TENSOR_STORE_SCP_AROWS) | (0x1 << TENSOR_STORE_SCP_OPC_1) | (0x205d4eba0 << TENSOR_STORE_SCP_VADDR) | 0 )\n" 
			"li x31, (0x1 << 6)\n"
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 10\n"
			"csrw tensor_store, x7\n"	
			"csrwi tensor_wait, 8\n"
			"LBL_SEQID_2_XMA_QNT_STR_SEQ_ITER_1_HID_0:\n"
			"li x5, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x1 << TENSOR_FMA32_B_NUM_COLS) | (0x1 << TENSOR_FMA32_A_NUM_ROWS) | (0x8 << TENSOR_FMA32_A_NUM_COLS) | (0xc << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0xa3 << TENSOR_FMA32_SCP_LOC_B) | (0xf << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x6, ((0x11 << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0x1 << TENSOR_QUANT_QUANT_ROW) | (0x1 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x7 << TENSOR_QUANT_TRANSF5) | (0x7 << TENSOR_QUANT_TRANSF4) | (0x6 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x7, ((0x2 << TENSOR_STORE_SCP_ENTRY_STRIDE) | (0x28 << TENSOR_STORE_SCP_START_SCP_ENTRY) | (0x4 << TENSOR_STORE_SCP_AROWS) | (0x1 << TENSOR_STORE_SCP_OPC_1) | (0x20a228fc0 << TENSOR_STORE_SCP_VADDR) | 0 )\n" 
			"li x31, (0x2 << 6)\n"
			"csrw tensor_fma, x5\n"	
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 7\n"
			"csrwi tensor_wait, 10\n"
			"csrw tensor_store, x7\n"	
			"csrwi tensor_wait, 8\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_0\n"
			"csrw tensor_error, x0\n"
			"li x5,  0x1ff800000610007\n" 
			"csrw tensor_fma, x5\n" 
			"csrwi tensor_wait, 7\n" 
			"csrwi tensor_wait, 0x6\n" 
			"fence\n"
			"li x5,  0xc0000000000000f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc0000000000004f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc0000000000008f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc000000000000cf\n" 
			"csrw   evict_sw, x5\n" 
			"csrwi tensor_wait, 0x6\n" 
			"csrrci x0, mcache_control, 2\n"
			"csrwi tensor_wait, 0x6\n" 
			"fence\n"
			"mov.m.x m0, x0, 0xFF\n"
			"li x5, 0x30\n"
			"la x31, rand_ieee754_32\n"
			"add x31, x31, x5\n"
			"flw.ps f10, 0(x31)\n"
			"flw.ps f12, 0(x31)\n"
			"flw.ps f14, 4(x31)\n"
			"flw.ps f15, 0(x31)\n"
			"flw.ps f16, 13(x31)\n"
			"flw.ps f2, 14(x31)\n"
			"flw.ps f21, 1(x31)\n"
			"flw.ps f25, 13(x31)\n"
			"flw.ps f26, 24(x31)\n"
			"flw.ps f28, 14(x31)\n"
			"flw.ps f29, 14(x31)\n"
			"flw.ps f31, 39(x31)\n"
			"flw.ps f5, 22(x31)\n"
			"flw.ps f7, 32(x31)\n"
			"flw.ps f8, 42(x31)\n"
			"flw.ps f9, 1(x31)\n"
			"la x31, rand_int_32\n"
			"add x31, x31, x5\n"
			"lw x10, 0(x31)\n"
			"lw x11, 0(x31)\n"
			"lw x12, 4(x31)\n"
			"lw x16, 9(x31)\n"
			"lw x17, 15(x31)\n"
			"lw x20, 17(x31)\n"
			"lw x22, 24(x31)\n"
			"lw x27, 23(x31)\n"
			"lw x7, 31(x31)\n"
			"li x21, 0x2\n"
			"LBL_SEQID_2_LOOP_SEQ_HID_0:\n"
			"fxor.pi f29, f25, f2\n"
			"fmadd.s f8, f14, f8, f7, rtz\n"
			"fmadd.s f29, f16, f26, f12, rup\n"
			"fadd.ps f16, f25, f2, rne\n"
			"fnmsub.ps f12, f12, f10, f25, rtz\n"
			"beq x20, x12, 1f\n"
			"1:\n"
			"fsatu8.pi f15, f8\n"
			"fmin.s f29, f9, f25\n"
			"fmadd.ps f12, f12, f28, f15, rne\n"
			"bltu x1, x31, 1f\n"
			"1:\n"
			"fmul.s f9, f7, f25, rne\n"
			"bge x4, x31, 1f\n"
			"1:\n"
			"fsra.pi f21, f21, f10\n"
			"fsra.pi f10, f10, f8\n"
			"fmul.s f16, f16, f26, rdn\n"
			"bge x3, x17, 1f\n"
			"1:\n"
			"fxor.pi f8, f28, f12\n"
			"fexp.ps f12, f26\n"
			"fsgnjn.s f31, f2, f16\n"
			"fsrli.pi f15, f29, 0xe\n"
			"slli x10, x10, 2\n"
			"slli x20, x12, 4\n"
			"fadd.ps f21, f8, f21, rup\n"
			"fsatu8.pi f28, f16\n"
			"blt x11, x17, 1f\n"
			"1:\n"
			"fmax.s f25, f9, f2\n"
			"fmul.s f7, f12, f7, rup\n"
			"fexp.ps f8, f31\n"
			"fsatu8.pi f26, f25\n"
			"fxor.pi f25, f12, f29\n"
			"blt x15, x30, 1f\n"
			"1:\n"
			"fmadd.ps f5, f31, f2, f12, rne\n"
			"bge x12, x21, 1f\n"
			"1:\n"
			"fadd.ps f16, f10, f26, rne\n"
			"bge x26, x6, 1f\n"
			"1:\n"
			"fle.ps f28, f28, f2\n"
			"bltu x13, x31, 1f\n"
			"1:\n"
			"fmax.s f10, f5, f10\n"
			"fmin.s f14, f25, f14\n"
			"fsra.pi f16, f25, f16\n"
			"fmax.s f14, f14, f2\n"
			"slli x10, x10, 0\n"
			"fsrli.pi f28, f10, 0xb\n"
			"bltu x0, x14, 1f\n"
			"1:\n"
			"fexp.ps f10, f10\n"
			"fadd.ps f25, f29, f31, rdn\n"
			"fsrli.pi f2, f10, 0x0\n"
			"fnmsub.ps f29, f10, f29, f8, rup\n"
			"fsra.pi f12, f12, f5\n"
			"fmul.s f7, f12, f21, rmm\n"
			"fsatu8.pi f21, f12\n"
			"fmul.s f16, f16, f10, rtz\n"
			"fmin.s f25, f14, f8\n"
			"fsrli.pi f28, f12, 0x0\n"
			"fadd.s f29, f15, f31, rmm\n"
			"fmadd.s f28, f25, f31, f28, rup\n"
			"beq x27, x23, 1f\n"
			"1:\n"
			"fsatu8.pi f10, f26\n"
			"blt x2, x23, 1f\n"
			"1:\n"
			"fexp.ps f15, f8\n"
			"fsrli.pi f10, f10, 0xe\n"
			"fsatu8.pi f12, f31\n"
			"fsrli.pi f5, f12, 0x7\n"
			"fmadd.s f9, f8, f10, f12, rdn\n"
			"bne x14, x8, 1f\n"
			"1:\n"
			"fmax.s f12, f31, f8\n"
			"fxor.pi f21, f29, f16\n"
			"beq x7, x15, 1f\n"
			"1:\n"
			"fadd.ps f9, f15, f26, rup\n"
			"fsatu8.pi f15, f21\n"
			"bne x13, x27, 1f\n"
			"1:\n"
			"fle.ps f21, f21, f26\n"
			"fmadd.s f28, f5, f21, f16, rup\n"
			"fadd.s f16, f31, f12, rdn\n"
			"fmadd.s f14, f14, f21, f21, rup\n"
			"fmin.s f10, f28, f2\n"
			"bge x4, x18, 1f\n"
			"1:\n"
			"fsra.pi f10, f31, f16\n"
			"ori x22, x12, 0x1c\n"
			"fmadd.s f28, f29, f15, f28, rne\n"
			"fmadd.s f7, f21, f31, f9, rdn\n"
			"fadd.ps f21, f15, f2, rdn\n"
			"beq x22, x25, 1f\n"
			"1:\n"
			"fsra.pi f8, f25, f10\n"
			"fxor.pi f16, f2, f5\n"
			"fsrli.pi f16, f9, 0xc\n"
			"fadd.ps f25, f29, f7, rne\n"
			"fexp.ps f25, f25\n"
			"fadd.ps f5, f5, f2, rdn\n"
			"fadd.s f2, f7, f8, rup\n"
			"slli x7, x12, 2\n"
			"fnmsub.ps f14, f14, f7, f10, rdn\n"
			"ori x28, x16, 0xd\n"
			"fadd.ps f29, f21, f25, rmm\n"
			"fxor.pi f16, f16, f10\n"
			"bne x8, x29, 1f\n"
			"1:\n"
			"fsatu8.pi f9, f5\n"
			"fxor.pi f5, f15, f15\n"
			"fmax.s f21, f31, f16\n"
			"fmadd.s f16, f2, f16, f12, rup\n"
			"fnmsub.ps f15, f29, f2, f15, rtz\n"
			"ori x11, x11, 0x17\n"
			"fsgnjn.s f12, f2, f12\n"
			"beq x23, x9, 1f\n"
			"1:\n"
			"slli x12, x11, 1\n"
			"fexp.ps f5, f26\n"
			"fmax.s f16, f16, f28\n"
			"fmax.s f26, f31, f10\n"
			"fmadd.s f28, f2, f29, f29, rup\n"
			"fmin.s f28, f21, f8\n"
			"slli x28, x11, 2\n"
			"fsrli.pi f28, f28, 0x5\n"
			"bltu x25, x26, 1f\n"
			"1:\n"
			"fmul.s f7, f14, f31, rmm\n"
			"blt x26, x14, 1f\n"
			"1:\n"
			"slli x22, x22, 4\n"
			"fnmsub.ps f25, f2, f15, f29, rup\n"
			"ori x20, x17, 0xc\n"
			"fsatu8.pi f9, f28\n"
			"fsatu8.pi f29, f25\n"
			"fsrli.pi f10, f29, 0xc\n"
			"fxor.pi f31, f12, f14\n"
			"fsra.pi f31, f31, f25\n"
			"fnmsub.ps f7, f31, f21, f2, rne\n"
			"fnmsub.ps f14, f14, f5, f31, rmm\n"
			"fmadd.s f8, f29, f5, f7, rmm\n"
			"fsatu8.pi f10, f15\n"
			"bge x23, x25, 1f\n"
			"1:\n"
			"ori x15, x11, 0xf\n"
			"bge x8, x24, 1f\n"
			"1:\n"
			"fmadd.s f21, f7, f12, f8, rup\n"
			"bge x11, x2, 1f\n"
			"1:\n"
			"fsgnjn.s f26, f7, f8\n"
			"bge x4, x13, 1f\n"
			"1:\n"
			"fmin.s f15, f10, f25\n"
			"fmadd.s f31, f14, f14, f31, rup\n"
			"fexp.ps f2, f12\n"
			"fsgnjn.s f12, f25, f29\n"
			"fmadd.ps f12, f8, f12, f5, rtz\n"
			"fxor.pi f10, f8, f31\n"
			"fnmsub.ps f9, f9, f7, f15, rup\n"
			"fsra.pi f14, f29, f8\n"
			"fsra.pi f21, f21, f12\n"
			"fxor.pi f16, f16, f5\n"
			"fnmsub.ps f7, f12, f9, f25, rtz\n"
			"slli x11, x16, 3\n"
			"beq x17, x30, 1f\n"
			"1:\n"
			"ori x12, x27, 0xf\n"
			"bge x25, x18, 1f\n"
			"1:\n"
			"fmul.s f8, f8, f8, rup\n"
			"fmadd.s f10, f16, f28, f2, rdn\n"
			"ori x17, x10, 0x9\n"
			"fadd.s f2, f2, f28, rne\n"
			"ori x28, x17, 0x4\n"
			"fadd.s f25, f16, f31, rdn\n"
			"fsrli.pi f14, f7, 0xe\n"
			"bgeu x29, x30, 1f\n"
			"1:\n"
			"fadd.s f31, f29, f31, rne\n"
			"fexp.ps f26, f7\n"
			"fmax.s f21, f16, f12\n"
			"ori x22, x20, 0x3\n"
			"fexp.ps f26, f26\n"
			"fle.ps f9, f31, f25\n"
			"fadd.s f7, f9, f10, rmm\n"
			"fmul.s f26, f26, f2, rmm\n"
			"bge x12, x21, 1f\n"
			"1:\n"
			"fmax.s f5, f28, f28\n"
			"ori x17, x22, 0x3\n"
			"fmadd.s f5, f8, f5, f2, rup\n"
			"fadd.s f26, f10, f21, rdn\n"
			"bge x23, x17, 1f\n"
			"1:\n"
			"fsatu8.pi f12, f12\n"
			"fsrli.pi f14, f9, 0xc\n"
			"fle.ps f29, f15, f14\n"
			"fle.ps f28, f31, f26\n"
			"fadd.s f12, f5, f14, rdn\n"
			"fsgnjn.s f15, f31, f25\n"
			"fexp.ps f29, f15\n"
			"ori x11, x17, 0x16\n"
			"fadd.s f2, f12, f5, rmm\n"
			"fsra.pi f26, f10, f16\n"
			"fsrli.pi f14, f16, 0x5\n"
			"fxor.pi f8, f8, f5\n"
			"fsrli.pi f7, f16, 0x1\n"
			"fsrli.pi f28, f26, 0x6\n"
			"fnmsub.ps f21, f25, f9, f31, rup\n"
			"ori x10, x12, 0xd\n"
			"fmadd.ps f10, f14, f7, f8, rmm\n"
			"fexp.ps f31, f29\n"
			"fsra.pi f28, f15, f9\n"
			"fsatu8.pi f7, f16\n"
			"fsgnjn.s f7, f7, f9\n"
			"bltu x16, x0, 1f\n"
			"1:\n"
			"fexp.ps f28, f26\n"
			"fadd.s f25, f8, f7, rne\n"
			"fle.ps f15, f25, f15\n"
			"fsgnjn.s f15, f15, f9\n"
			"fsgnjn.s f16, f2, f12\n"
			"fsrli.pi f26, f21, 0x1\n"
			"blt x17, x6, 1f\n"
			"1:\n"
			"slli x16, x7, 4\n"
			"fsatu8.pi f10, f9\n"
			"fsgnjn.s f14, f29, f14\n"
			"fadd.s f26, f26, f8, rne\n"
			"addi x21, x21, -1\n"
			"bne x21, x0, LBL_SEQID_2_LOOP_SEQ_HID_0\n"
			"li x5, 3\n" 
			"bne x30, x5, LBL_FAIL_HID_0\n"
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f8,  672 (x5)\n"
			"flw.ps f4,  384 (x5)\n"
			"flw.ps f5,  736 (x5)\n"
			"flw.ps f2,  320 (x5)\n"
			"flw.ps f9,  352 (x5)\n"
			"flw.ps f20,  640 (x5)\n"
			"flw.ps f26,  928 (x5)\n"
			"flw.ps f0,  96 (x5)\n"
			"flw.ps f12,  608 (x5)\n"
			"flw.ps f28,  224 (x5)\n"
			"flw.ps f31,  544 (x5)\n"
			"flw.ps f30,  448 (x5)\n"
			"flw.ps f24,  256 (x5)\n"
			"flw.ps f3,  64 (x5)\n"
			"flw.ps f16,  160 (x5)\n"
			"flw.ps f7,  800 (x5)\n"
			"flw.ps f21,  192 (x5)\n"
			"flw.ps f1,  864 (x5)\n"
			"flw.ps f15,  832 (x5)\n"
			"flw.ps f11,  512 (x5)\n"
			"flw.ps f27,  576 (x5)\n"
			"flw.ps f14,  992 (x5)\n"
			"flw.ps f10,  128 (x5)\n"
			"flw.ps f25,  288 (x5)\n"
			"flw.ps f29,  0 (x5)\n"
			"flw.ps f18,  416 (x5)\n"
			"flw.ps f6,  704 (x5)\n"
			"flw.ps f23,  960 (x5)\n"
			"flw.ps f13,  32 (x5)\n"
			"flw.ps f19,  896 (x5)\n"
			"flw.ps f17,  480 (x5)\n"
			"flw.ps f22,  768 (x5)\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_3_INT_AMO_ID_38_HID_0:\n"
			"li x28, 0x8146d8f00c\n" 
			"amominl.w x12, x7, (x28)\n" 
			"li x5, 0x3073\n"
			"csrw tensor_mask, x5\n"	
			"fsgnjx.s f10, f15, f7\n"
			"LBL_SEQID_3_INT_L_STORE_ID_39_HID_0:\n"
			"li x7, 0x824f588824\n" 
			"shl x27, (x7)\n" 
			"fmax.pi f21, f21, f14\n"
			"LBL_SEQID_3_FP_L_GATHER_ID_40_HID_0:\n"
			"li x7, 0x824f588820\n" 
			"la x15, mem_ptr_offset\n"
			"flq2 f21, 16(x15)\n"
			"fscwl.ps f5, f21 (x7)\n"
			"fgbl.ps f12, f21 (x7)\n" 
			"maskand m6, m1, m6\n"
			"LBL_SEQID_3_INT_AMO_ID_41_HID_0:\n"
			"li x27, 0x824d1a0020\n" 
			"amoorl.d x15, x22, (x27)\n" 
			"fandi.pi f26, f14, 0x3c\n"
			"LBL_SEQID_3_FP_AMO_ID_42_HID_0:\n"
			"li x12, 0x814e392064\n" 
			"la x17, mem_ptr_offset\n"
			"flw.ps f25, 832(x17)\n"
			"famomaxl.ps f10, f25 (x12)\n" 
			"fpackreph.pi f28, f29\n"
			"LBL_SEQID_3_INT_AMO_ID_43_HID_0:\n"
			"li x11, 0x82457a0018\n" 
			"amoorl.w x7, x22, (x11)\n" 
			"auipc x15, 0x15\n"
			"LBL_SEQID_3_FP_STORE_ID_44_HID_0:\n"
			"li x28, 0x8104122256\n" 
			"fsq2 f25, 851(x28)\n" 
			"addw x11, x10, x11\n"
			"LBL_SEQID_3_FP_GATHER_ID_45_HID_0:\n"
			"li x12, 0x8203d60010\n" 
			"li x7, 0xb9ecc30c80d025d1\n"
			"fsc32w.ps f28, x7 (x12)\n"
			"fg32b.ps f25, x7 (x12)\n" 
			"fmulh.pi f9, f14, f16\n"
			"LBL_SEQID_3_FP_L_GATHER_ID_46_HID_0:\n"
			"li x17, 0x8146d8f008\n" 
			"la x10, mem_ptr_offset\n"
			"flq2 f31, 148(x10)\n"
			"fscwl.ps f7, f31 (x17)\n"
			"fgbl.ps f21, f31 (x17)\n" 
			"fxor.pi f25, f16, f25\n"
			"LBL_SEQID_3_INT_AMO_ID_47_HID_0:\n"
			"li x16, 0x824f588824\n" 
			"amoorl.w x12, x15, (x16)\n" 
			"fsgnj.ps f25, f29, f12\n"
			"LBL_SEQID_3_FP_L_GATHER_ID_48_HID_0:\n"
			"li x17, 0x8142dd6008\n" 
			"la x22, mem_ptr_offset\n"
			"flq2 f29, 108(x22)\n"
			"fscwl.ps f23, f29 (x17)\n"
			"fghl.ps f16, f29 (x17)\n" 
			"fmin.s f5, f5, f9\n"
			"LBL_SEQID_3_INT_AMO_ID_49_HID_0:\n"
			"li x16, 0x81556ef008\n" 
			"amoswapl.d x15, x11, (x16)\n" 
			"fsgnj.ps f29, f29, f21\n"
			"LBL_SEQID_3_FP_L_SCATTER_ID_50_HID_0:\n"
			"li x22, 0x8145522410\n" 
			"la x10, mem_ptr_offset\n"
			"flw.ps f28, 156(x10)\n"
			"fscwl.ps f21, f28 (x22)\n" 
			"fcvt.ps.f16 f15, f15\n"
			"LBL_SEQID_3_FP_GATHER_ID_51_HID_0:\n"
			"li x15, 0x810108cfea\n" 
			"li x10, 0x6f52ab739e1920a0\n"
			"fsc32w.ps f17, x10 (x15)\n"
			"fg32b.ps f26, x10 (x15)\n" 
			"fsub.pi f25, f29, f5\n"
			"LBL_SEQID_3_INT_AMO_ID_52_HID_0:\n"
			"li x22, 0x82422e0030\n" 
			"amoorl.w x28, x11, (x22)\n" 
			"fsat8.pi f15, f5\n"
			"bne x21, x16, 1f\n"
			"1:\n"
			"LBL_SEQID_3_INT_L_STORE_ID_53_HID_0:\n"
			"li x12, 0x8150aa01c8\n" 
			"shl x20, (x12)\n" 
			"li x5, 0x8ce8\n"
			"csrw tensor_mask, x5\n"	
			"fpackrepb.pi f5, f12\n"
			"LBL_SEQID_3_CSR_ID_54_HID_0:\n"
			"csrr x0, fflags\n"
			"remuw x22, x22, x10\n"
			"LBL_SEQID_3_INT_STORE_ID_55_HID_0:\n"
			"li x17, 0x8107260063\n" 
			"sd x16, -541(x17)\n" 
			"fmadd.s f15, f28, f21, f14, rdn\n"
			"LBL_SEQID_3_INT_AMO_ID_56_HID_0:\n"
			"li x17, 0x824533bb50\n" 
			"amoorl.w x28, x20, (x17)\n" 
			"fsrli.pi f25, f16, 0x5\n"
			"LBL_SEQID_3_CSR_ID_57_HID_0:\n"
			"csrr x0, frm\n"
			"fmax.pi f7, f12, f8\n"
			"LBL_SEQID_3_FP_GATHER_ID_58_HID_0:\n"
			"li x16, 0x8106a8682c\n" 
			"li x7, 0x3b8fc26d9aeb6756\n"
			"fsc32w.ps f27, x7 (x16)\n"
			"fg32b.ps f16, x7 (x16)\n" 
			"srliw x15, x16, 4\n"
			"LBL_SEQID_3_CSR_ID_59_HID_0:\n"
			"li x12, 0x800000810e39204c\n"
			"li x31, (0x2 << 6) | 0x1\n"
			"csrw prefetch_va, x12\n" 
			"csrwi tensor_wait, 6\n" 
			"csrwi tensor_wait, 5\n" 
			"fadd.ps f2, f14, f2, rtz\n"
			"LBL_SEQID_3_FP_G_SCATTER_ID_60_HID_0:\n"
			"li x17, 0x8130aa01c8\n" 
			"la x28, mem_ptr_offset\n"
			"flw.ps f5, 16(x28)\n"
			"fschg.ps f15, f5 (x17)\n" 
			"fcmov.ps f21, f16, f14, f12\n"
			"LBL_SEQID_3_FP_L_GATHER_ID_61_HID_0:\n"
			"li x27, 0x81453ae810\n" 
			"la x15, mem_ptr_offset\n"
			"flq2 f28, 144(x15)\n"
			"fscwl.ps f19, f28 (x27)\n"
			"fgbl.ps f28, f28 (x27)\n" 
			"faddi.pi f31, f21, 0x1cf\n"
			"LBL_SEQID_3_FP_G_SCATTER_ID_62_HID_0:\n"
			"li x20, 0x823f588820\n" 
			"la x10, mem_ptr_offset\n"
			"flw.ps f21, 108(x10)\n"
			"fscwg.ps f15, f21 (x20)\n" 
			"fxor.pi f26, f31, f26\n"
			"LBL_SEQID_3_FP_L_SCATTER_ID_63_HID_0:\n"
			"li x20, 0x8147260060\n" 
			"la x10, mem_ptr_offset\n"
			"flq2 f25, 108(x10)\n"
			"fscwl.ps f21, f25 (x20)\n" 
			"fand.pi f31, f16, f15\n"
			"LBL_SEQID_3_INT_STORE_ID_64_HID_0:\n"
			"li x17, 0x810a7a602d\n" 
			"sd x27, -1562(x17)\n" 
			"fsll.pi f10, f10, f7\n"
			"LBL_SEQID_3_FP_STORE_ID_65_HID_0:\n"
			"li x22, 0x8104122256\n" 
			"fsq2 f15, 1086(x22)\n" 
			"fcvt.f16.ps f2, f5\n"
			"bne x14, x15, 1f\n"
			"1:\n"
			"LBL_SEQID_3_FP_G_SCATTER_ID_66_HID_0:\n"
			"li x16, 0x82222e0030\n" 
			"la x20, mem_ptr_offset\n"
			"flw.ps f15, 8(x20)\n"
			"fschg.ps f16, f15 (x16)\n" 
			"fence\n"
			"LBL_SEQID_3_FP_L_GATHER_ID_67_HID_0:\n"
			"li x28, 0x81519607b8\n" 
			"la x22, mem_ptr_offset\n"
			"flw.ps f2, 12(x22)\n"
			"fscwl.ps f9, f2 (x28)\n"
			"fghl.ps f28, f2 (x28)\n" 
			"fsgnjx.s f14, f26, f12\n"
			"beq x12, x1, 1f\n"
			"1:\n"
			"LBL_SEQID_3_INT_AMO_ID_68_HID_0:\n"
			"li x20, 0x8145440038\n" 
			"amomaxl.d x11, x22, (x20)\n" 
			"fmsub.ps f29, f9, f14, f9, rmm\n"
			"LBL_SEQID_3_INT_AMO_ID_69_HID_0:\n"
			"li x17, 0x82567a0418\n" 
			"amoaddl.d x28, x28, (x17)\n" 
			"mova.m.x x27\n"
			"csrw tensor_error, zero\n" 
			"csrwi tensor_wait, 0x6\n" 
			"fence\n"
			"li x5,  0xc0000000000000f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc0000000000004f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc0000000000008f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc000000000000cf\n" 
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
			"fand.pi f16, f15, f15\n"
			"fadd.pi f15, f25, f16\n"
			"fsgnjx.s f5, f26, f8\n"
			"fslli.pi f29, f14, 0x0\n"
			"fand.pi f7, f31, f7\n"
			"fsat8.pi f16, f12\n"
			"fle.ps f21, f5, f8\n"
			"bne x25, x27, 1f\n"
			"1:\n"
			"LBL_SEQID_3_XMA_QNT_STR_SEQ_ITER_0_HID_0:\n"
			"li x7, ((0x1 << TENSOR_STORE_REG_STRIDE) | (0x16 << TENSOR_STORE_START_REG) | (0x0 << TENSOR_STORE_STORE_COLS) | (0x6 << TENSOR_STORE_AROWS) | (0x0 << TENSOR_STORE_COOP_STORE) | (0x0 << TENSOR_STORE_OPC_0) | (0x817412225 << TENSOR_STORE_VADDR) | 0 )\n" 
			"li x31, (0x10 << 4)\n"
			"csrw tensor_store, x7\n"	
			"LBL_SEQID_3_XMA_QNT_STR_SEQ_ITER_1_HID_0:\n"
			"li x7, ((0x0 << TENSOR_STORE_REG_STRIDE) | (0xb << TENSOR_STORE_START_REG) | (0x1 << TENSOR_STORE_STORE_COLS) | (0x5 << TENSOR_STORE_AROWS) | (0x0 << TENSOR_STORE_COOP_STORE) | (0x0 << TENSOR_STORE_OPC_0) | (0x819e39206 << TENSOR_STORE_VADDR) | 0 )\n" 
			"li x31, (0x20 << 4)\n"
			"csrw tensor_store, x7\n"	
			"LBL_SEQID_3_XMA_QNT_STR_SEQ_ITER_2_HID_0:\n"
			"li x7, ((0x0 << TENSOR_STORE_REG_STRIDE) | (0x1d << TENSOR_STORE_START_REG) | (0x0 << TENSOR_STORE_STORE_COLS) | (0xc << TENSOR_STORE_AROWS) | (0x0 << TENSOR_STORE_COOP_STORE) | (0x0 << TENSOR_STORE_OPC_0) | (0x80003b4 << TENSOR_STORE_VADDR) | 0 )\n" 
			"li x31, (0x80 << 4)\n"
			"csrw tensor_store, x7\n"	
			"LBL_SEQID_3_XMA_QNT_STR_SEQ_ITER_3_HID_0:\n"
			"li x7, ((0x2 << TENSOR_STORE_REG_STRIDE) | (0x5 << TENSOR_STORE_START_REG) | (0x3 << TENSOR_STORE_STORE_COLS) | (0xd << TENSOR_STORE_AROWS) | (0x0 << TENSOR_STORE_COOP_STORE) | (0x0 << TENSOR_STORE_OPC_0) | (0x817774c9c << TENSOR_STORE_VADDR) | 0 )\n" 
			"li x31, (0x80 << 4)\n"
			"csrw tensor_store, x7\n"	
			"csrwi tensor_wait, 8\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_0\n"
			"csrw tensor_error, x0\n"
			"li x5,  0x1ff800000610007\n" 
			"csrw tensor_fma, x5\n" 
			"csrwi tensor_wait, 7\n" 
			"csrwi tensor_wait, 0x6\n" 
			"fence\n"
			"li x5,  0xc0000000000000f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc0000000000004f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc0000000000008f\n" 
			"csrw   evict_sw, x5\n" 
			"li x5,  0xc000000000000cf\n" 
			"csrw   evict_sw, x5\n" 
			"csrwi tensor_wait, 0x6\n" 
			"csrrci x0, mcache_control, 3\n"
			"csrwi tensor_wait, 0x6\n" 
			"fence\n"
			"mov.m.x m0, x0, 0xFF\n"
			"li x5, 0x20\n"
			"la x31, rand_ieee754_32\n"
			"add x31, x31, x5\n"
			"flw.ps f10, 0(x31)\n"
			"flw.ps f12, 0(x31)\n"
			"flw.ps f14, 6(x31)\n"
			"flw.ps f15, 12(x31)\n"
			"flw.ps f16, 8(x31)\n"
			"flw.ps f2, 0(x31)\n"
			"flw.ps f21, 15(x31)\n"
			"flw.ps f25, 8(x31)\n"
			"flw.ps f26, 27(x31)\n"
			"flw.ps f28, 23(x31)\n"
			"flw.ps f29, 9(x31)\n"
			"flw.ps f31, 38(x31)\n"
			"flw.ps f5, 3(x31)\n"
			"flw.ps f7, 26(x31)\n"
			"flw.ps f8, 6(x31)\n"
			"flw.ps f9, 10(x31)\n"
			"la x31, rand_int_32\n"
			"add x31, x31, x5\n"
			"lw x10, 0(x31)\n"
			"lw x11, 3(x31)\n"
			"lw x12, 5(x31)\n"
			"lw x15, 2(x31)\n"
			"lw x16, 13(x31)\n"
			"lw x17, 19(x31)\n"
			"lw x20, 16(x31)\n"
			"lw x22, 24(x31)\n"
			"lw x27, 11(x31)\n"
			"lw x28, 22(x31)\n"
			"lw x7, 34(x31)\n"
			"li x21, 0x1\n"
			"LBL_SEQID_3_LOOP_SEQ_HID_0:\n"
			"sraw x7, x7, x10\n"
			"fle.pi f15, f9, f10\n"
			"bgeu x31, x25, 1f\n"
			"1:\n"
			"ffrc.ps f8, f31\n"
			"feqm.ps m5, f21, f5\n"
			"maskor m1, m1, m1\n"
			"slti x22, x15, 0x3\n"
			"maskand m1, m1, m6\n"
			"fsin.ps f15, f2\n"
			"fslli.pi f10, f16, 0x7\n"
			"masknot m6, m2\n"
			"fle.pi f28, f25, f21\n"
			"fence\n"
			"fle.pi f29, f21, f26\n"
			"xor x20, x7, x28\n"
			"fnot.pi f7, f14\n"
			"sltu x27, x27, x20\n"
			"mova.x.m x17\n"
			"fswizz.ps f5, f10, 0x8\n"
			"fsgnjn.s f10, f29, f29\n"
			"subw x20, x22, x12\n"
			"rem x22, x17, x22\n"
			"sub x11, x28, x22\n"
			"beq x22, x18, 1f\n"
			"1:\n"
			"addiw x15, x17, 0xb\n"
			"flog.ps f25, f16\n"
			"masknot m5, m1\n"
			"add x15, x15, x15\n"
			"ori x11, x17, 0x10\n"
			"feqm.ps m2, f26, f21\n"
			"bne x21, x6, 1f\n"
			"1:\n"
			"fsub.s f26, f29, f5, rup\n"
			"blt x6, x1, 1f\n"
			"1:\n"
			"fmul.s f21, f25, f15, rne\n"
			"fcvt.s.w f16, x17, rup\n"
			"fmul.ps f8, f10, f28, rtz\n"
			"fmsub.ps f28, f28, f16, f2, rup\n"
			"beq x19, x5, 1f\n"
			"1:\n"
			"fsra.pi f12, f10, f12\n"
			"flog.ps f16, f15\n"
			"ecall\n"
			"fsub.pi f16, f21, f12\n"
			"mova.m.x x12\n"
			"fence\n"
			"blt x7, x6, 1f\n"
			"1:\n"
			"mulh x16, x17, x17\n"
			"mov.m.x m2, x7, 0x52\n"
			"bge x26, x14, 1f\n"
			"1:\n"
			"frcp.ps f29, f9\n"
			"fcvt.s.w f16, x12, rdn\n"
			"mulhsu x22, x10, x27\n"
			"fsra.pi f31, f29, f10\n"
			"fsatu8.pi f9, f14\n"
			"flog.ps f29, f29\n"
			"srliw x7, x15, 2\n"
			"fswizz.ps f26, f16, 0x12\n"
			"blt x5, x27, 1f\n"
			"1:\n"
			"fround.ps f7, f2, rtz\n"
			"mul x10, x22, x22\n"
			"flt.pi f8, f10, f16\n"
			"srlw x10, x7, x27\n"
			"frsq.ps f28, f9\n"
			"remuw x27, x10, x27\n"
			"fmv.x.s x12, f26\n"
			"fclass.s x10, f12\n"
			"divu x7, x27, x15\n"
			"fmadd.ps f2, f28, f15, f10, rne\n"
			"fle.s x28, f21, f25\n"
			"fsrli.pi f8, f26, 0xa\n"
			"fnmadd.ps f21, f2, f5, f7, rdn\n"
			"feq.pi f7, f7, f2\n"
			"beq x22, x15, 1f\n"
			"1:\n"
			"mova.x.m x15\n"
			"fcvt.wu.s x28, f29, rtz\n"
			"fsgnj.ps f12, f26, f12\n"
			"fandi.pi f14, f14, 0x1fe\n"
			"blt x23, x10, 1f\n"
			"1:\n"
			"fsrli.pi f5, f2, 0x8\n"
			"addiw x22, x15, 0x9\n"
			"sltiu x15, x27, 0xb\n"
			"bne x2, x3, 1f\n"
			"1:\n"
			"ecall\n"
			"mulhu x28, x12, x28\n"
			"remw x16, x28, x15\n"
			"fmv.x.s x15, f12\n"
			"feq.s x20, f8, f9\n"
			"bge x14, x13, 1f\n"
			"1:\n"
			"fence\n"
			"addiw x12, x20, 0x5\n"
			"fcvt.s.wu f15, x16, rdn\n"
			"fcvt.s.wu f29, x12, rdn\n"
			"mul x10, x15, x22\n"
			"auipc x16, 0xc\n"
			"fsrai.pi f15, f12, 0x2\n"
			"ebreak\n"
			"fmsub.ps f26, f5, f12, f14, rmm\n"
			"blt x29, x6, 1f\n"
			"1:\n"
			"masknot m1, m5\n"
			"fmin.pi f5, f25, f14\n"
			"fsetm.pi m5, f2\n"
			"lui x15, 0x0\n"
			"slti x15, x17, 0x12\n"
			"flt.ps f7, f10, f5\n"
			"flt.pi f15, f10, f21\n"
			"sub x27, x27, x12\n"
			"fnmadd.s f10, f29, f5, f31, rmm\n"
			"fclass.s x7, f5\n"
			"packb x22, x28, x27\n"
			"ecall\n"
			"bge x26, x31, 1f\n"
			"1:\n"
			"fsgnj.s f8, f8, f26\n"
			"masknot m2, m2\n"
			"fadd.pi f2, f2, f2\n"
			"fsgnjn.s f14, f14, f2\n"
			"fmsub.ps f10, f31, f2, f8, rdn\n"
			"fcvt.wu.s x17, f15, rup\n"
			"fandi.pi f21, f21, 0x167\n"
			"bgeu x16, x12, 1f\n"
			"1:\n"
			"fsin.ps f21, f29\n"
			"frsq.ps f15, f29\n"
			"bltu x19, x19, 1f\n"
			"1:\n"
			"maskxor m6, m6, m5\n"
			"subw x7, x16, x22\n"
			"fsll.pi f5, f5, f25\n"
			"bltu x29, x28, 1f\n"
			"1:\n"
			"fltm.ps m1, f9, f29\n"
			"fsin.ps f8, f25\n"
			"fbci.pi f31, 0x267d\n"
			"bne x12, x15, 1f\n"
			"1:\n"
			"or x27, x27, x20\n"
			"sra x20, x20, x16\n"
			"fmin.ps f12, f9, f21\n"
			"fbci.ps f8, 0x6420f\n"
			"divw x10, x10, x20\n"
			"fbcx.ps f14, x11\n"
			"addw x20, x20, x10\n"
			"fltu.pi f8, f8, f26\n"
			"maskor m5, m1, m2\n"
			"ebreak\n"
			"mov.m.x m6, x11, 0x58\n"
			"fsrl.pi f8, f8, f21\n"
			"divuw x10, x20, x16\n"
			"srliw x12, x12, 3\n"
			"fltm.pi m6, f15, f14\n"
			"fcvt.pw.ps f21, f21, rup\n"
			"sraiw x28, x28, 4\n"
			"fadd.s f15, f28, f16, rtz\n"
			"fcvt.pwu.ps f29, f29, rup\n"
			"subw x10, x10, x10\n"
			"remw x15, x7, x10\n"
			"sllw x7, x7, x27\n"
			"beq x22, x29, 1f\n"
			"1:\n"
			"ecall\n"
			"fsrl.pi f21, f21, f7\n"
			"fpackrepb.pi f26, f15\n"
			"fexp.ps f15, f12\n"
			"fexp.ps f10, f10\n"
			"srliw x10, x12, 0\n"
			"beq x31, x28, 1f\n"
			"1:\n"
			"fsatu8.pi f10, f31\n"
			"fsat8.pi f2, f26\n"
			"fnmadd.ps f5, f7, f7, f5, rtz\n"
			"bgeu x29, x0, 1f\n"
			"1:\n"
			"remuw x12, x11, x12\n"
			"fand.pi f14, f9, f15\n"
			"bne x20, x29, 1f\n"
			"1:\n"
			"fandi.pi f21, f29, 0x59\n"
			"fround.ps f12, f15, rup\n"
			"fcvt.s.w f15, x15, rmm\n"
			"fcvt.wu.s x10, f15, rdn\n"
			"fsgnj.ps f15, f21, f29\n"
			"divuw x27, x12, x17\n"
			"fmul.ps f15, f28, f12, rne\n"
			"fsrl.pi f2, f16, f16\n"
			"blt x4, x14, 1f\n"
			"1:\n"
			"fxor.pi f21, f5, f16\n"
			"fclass.s x20, f2\n"
			"fmul.s f8, f9, f14, rne\n"
			"fmax.ps f14, f14, f29\n"
			"fpackreph.pi f26, f29\n"
			"fround.ps f5, f5, rmm\n"
			"bgeu x0, x21, 1f\n"
			"1:\n"
			"fclass.s x10, f9\n"
			"mulhsu x11, x15, x7\n"
			"slt x10, x10, x10\n"
			"subw x20, x20, x22\n"
			"bne x8, x11, 1f\n"
			"1:\n"
			"fmaxu.pi f29, f10, f21\n"
			"fsgnjx.s f16, f25, f15\n"
			"fsub.pi f14, f16, f10\n"
			"mul x12, x12, x15\n"
			"bgeu x25, x30, 1f\n"
			"1:\n"
			"ffrc.ps f31, f29\n"
			"fadd.ps f8, f8, f9, rtz\n"
			"fexp.ps f7, f8\n"
			"srli x7, x22, 4\n"
			"fmulhu.pi f14, f7, f2\n"
			"fcvt.w.s x10, f28, rmm\n"
			"fmax.s f12, f25, f31\n"
			"fmin.s f21, f25, f7\n"
			"maskpopc x15, m5\n"
			"fcvt.s.w f5, x15, rdn\n"
			"fmsub.ps f10, f5, f15, f10, rup\n"
			"blt x13, x28, 1f\n"
			"1:\n"
			"flem.ps m5, f25, f10\n"
			"fcvt.f16.ps f10, f21\n"
			"feq.pi f7, f26, f29\n"
			"packb x27, x27, x10\n"
			"addi x10, x20, 0x0\n"
			"mova.x.m x16\n"
			"fsgnj.s f5, f9, f16\n"
			"flem.ps m1, f29, f28\n"
			"fmsub.s f31, f29, f16, f5, rtz\n"
			"fcvt.s.w f21, x7, rdn\n"
			"mova.m.x x10\n"
			"fsatu8.pi f28, f25\n"
			"addi x21, x21, -1\n"
			"bne x21, x0, LBL_SEQID_3_LOOP_SEQ_HID_0\n"
			"li x5, 4\n" 
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
			"LBL_HPM_4_CORE_TS_OPS_FAIL_HID_0:\n"          
			"csrr x5, mhpmcounter4\n"
			"sd x5, 40 (x31)\n"
			"LBL_HPM_5_CORE_TFMA_INST_FAIL_HID_0:\n"          
			"csrr x5, mhpmcounter5\n"
			"sd x5, 48 (x31)\n"
			"LBL_HPM_6_CORE_TIMA_OPS_FAIL_HID_0:\n"          
			"csrr x5, mhpmcounter6\n"
			"sd x5, 56 (x31)\n"
			"LBL_HPM_7_NEIGH_TX_COOP_TST_REQ_FAIL_HID_0:\n"          
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
			"LBL_HPM_3_CORE_TL_OPS_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter3\n"
			"sd x5, 32 (x31)\n"
			"LBL_HPM_4_CORE_TS_OPS_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter4\n"
			"sd x5, 40 (x31)\n"
			"LBL_HPM_5_CORE_TFMA_INST_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter5\n"
			"sd x5, 48 (x31)\n"
			"LBL_HPM_6_CORE_TIMA_OPS_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter6\n"
			"sd x5, 56 (x31)\n"
			"LBL_HPM_7_NEIGH_TX_COOP_TST_REQ_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter7\n"
			"sd x5, 64 (x31)\n"
			"LBL_HPM_8_NEIGH_TX_NEIGH_COOP_TLD_REQ_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter8\n"
			"sd x5, 72 (x31)\n"
			"slti x0, x0, 0x7fe\n"
			VSNIP_RSV
		);
		C_TEST_PASS;
		__asm__ __volatile__ ( 
			"LBL_HALT_HID_0:\n"          
			".global LBL_HALT_HID_0\n"          
			"wfi\n"
			"j LBL_HALT_HID_0\n"          
			VSNIP_RSV
		);
		return 0;       
	};
}
volatile uint32_t rand_int_32[256] __attribute__ ((aligned (2048))) = {
		0xb73fb9ec, 
		0x92a46ed1, 
		0x32661c2f, 
		0x7cbf9cef, 
		0x477d6df5, 
		0xaef5c033, 
		0xc6095126, 
		0x7a91879c, 
		0xc65df9a9, 
		0x839feb11, 
		0x0e31bec4, 
		0x5bec15a1, 
		0x79a11df7, 
		0xc101b7e3, 
		0x954ad626, 
		0x748d10ef, 
		0x8b284a00, 
		0x8157588a, 
		0x677830ef, 
		0xaa7e8ba5, 
		0xa7edf88d, 
		0x240854c6, 
		0x781bd020, 
		0xf360fac6, 
		0x6897c592, 
		0xbe75c35a, 
		0x56a49ee7, 
		0x91bd2ed2, 
		0xd48211dd, 
		0xf3141785, 
		0xf085fcb6, 
		0x29e2314f, 
		0x919a2c2c, 
		0xa49d4318, 
		0xf227ee06, 
		0xcb5ee871, 
		0x22faf39b, 
		0xb8a6051a, 
		0x7777ee13, 
		0xa27f84b8, 
		0x4ccc00e1, 
		0x80eb1714, 
		0xcc1a4c67, 
		0xaef988d4, 
		0xd55cb89d, 
		0xfc19acf7, 
		0x6f1e0e1e, 
		0xcc02a1ea, 
		0x689c83ff, 
		0xa619129b, 
		0x3d9e6893, 
		0x136ff6a7, 
		0x6846f4a4, 
		0x38110993, 
		0x6085af06, 
		0x2c79edbe, 
		0x47729b30, 
		0xc16acb59, 
		0xb1c6a680, 
		0x69647c46, 
		0x522d5ab3, 
		0x43d1b311, 
		0x86ac6a47, 
		0x77732306, 
		0x4c7a00fd, 
		0x3eedf6c3, 
		0x2b502a89, 
		0xb5dc87ac, 
		0xd9b641b7, 
		0xf4871a29, 
		0x2be65ae8, 
		0x71d3516f, 
		0x6481e382, 
		0xc2e7a6e0, 
		0x2cea6e02, 
		0x372845f9, 
		0xa223dd95, 
		0xd79c9aba, 
		0x6de2ca36, 
		0xca789741, 
		0x9723260f, 
		0x520fd47b, 
		0xd4e9bbc0, 
		0xcd1f49a8, 
		0x62714ade, 
		0x6f9d5a01, 
		0x0ecb7c92, 
		0xd78e7da6, 
		0xd22a7576, 
		0x01d6edee, 
		0x314c9e70, 
		0x54ea8a54, 
		0x35576566, 
		0xe69d0f23, 
		0x6b00fc97, 
		0xb4720503, 
		0x91c5586e, 
		0x70dc8179, 
		0x7711e06b, 
		0xb72ae624, 
		0x47a9ade2, 
		0xa5e74615, 
		0x9d906def, 
		0x396798d5, 
		0x824de666, 
		0xccf3d9c9, 
		0x078fe132, 
		0x93d9c83c, 
		0x0d03a659, 
		0x6e040732, 
		0xec8d8223, 
		0xdc0deb11, 
		0x92c451b0, 
		0x6f2610d8, 
		0x7bcb655f, 
		0xabde70e6, 
		0xb58768a2, 
		0x4eb0a03a, 
		0x328c9e58, 
		0x8709817d, 
		0xfc72b586, 
		0x21e36d00, 
		0x4662e172, 
		0x7c445d10, 
		0x410e95ba, 
		0x5f58c56d, 
		0xeb63dbd9, 
		0x1001d059, 
		0xf17a80e1, 
		0x2a70273c, 
		0xbddfae5d, 
		0x9610cc91, 
		0xe5beaa0d, 
		0x9101fc38, 
		0x9c46e921, 
		0xa24606b1, 
		0xdae95138, 
		0xe6f55fb4, 
		0xeb0f1ce6, 
		0x08aef031, 
		0x5999c317, 
		0x3ef44a70, 
		0xc5f3258f, 
		0xa48aa248, 
		0x059e9b74, 
		0x884e13ee, 
		0xd8e6b714, 
		0x9bda354a, 
		0x80186441, 
		0xc565e489, 
		0x8fc5db76, 
		0x8edcd6f2, 
		0x0c234952, 
		0x869c9d5d, 
		0xbb9b2c62, 
		0xb15d2c17, 
		0xbd7ceba5, 
		0xfbd7b422, 
		0xf8997f92, 
		0xf894c79d, 
		0x598bdc62, 
		0x86ff615e, 
		0x40c8cc37, 
		0x453f5c17, 
		0x50d6e3c2, 
		0xc669cfa7, 
		0xae680fb8, 
		0x2e45ff13, 
		0x8be202b5, 
		0x1f5ffca9, 
		0x7fd04c18, 
		0x3c605076, 
		0xd28ce2a7, 
		0x89b232e1, 
		0xc2e03322, 
		0xbe845556, 
		0x467b2246, 
		0x1e1d7cde, 
		0x2c2dd9b6, 
		0xb9d4bed9, 
		0xf88ff8db, 
		0xe2795ca3, 
		0x36665419, 
		0xc70aeef6, 
		0x85aae646, 
		0x35b255af, 
		0xe838b625, 
		0xa1431e83, 
		0x38453895, 
		0x712f0b92, 
		0xbc051393, 
		0x04e99bfa, 
		0x5acd88fb, 
		0x420872e5, 
		0x0f0891e3, 
		0xe23f692d, 
		0x140ceefe, 
		0xe5dd591a, 
		0x2fef53b8, 
		0x27d72f94, 
		0xd903940c, 
		0x1e9f92f0, 
		0x27bd6996, 
		0x42ef56fe, 
		0xbd88968a, 
		0x9e077d8e, 
		0x862d6666, 
		0xfb4de12e, 
		0x88c5d279, 
		0x307cfa35, 
		0x3bdac056, 
		0x346d23c5, 
		0x5fe5662c, 
		0x83defb0a, 
		0xf5b8e05c, 
		0xb249ae0a, 
		0x891b8a21, 
		0x3adb32ab, 
		0x43c0e1e9, 
		0x9f822ac2, 
		0x496139b9, 
		0xc8f1371a, 
		0x9f40a0e3, 
		0x060ece1d, 
		0xa5a30eef, 
		0x06d91423, 
		0x3e81e73f, 
		0x81d4bf13, 
		0xec66afe8, 
		0x03a2787f, 
		0xbfe96fd5, 
		0xd25e7db7, 
		0x5c736825, 
		0xb75a383a, 
		0x97f182e9, 
		0xccbd31bb, 
		0x24abd1ff, 
		0x4c0a9a6a, 
		0xee889bfe, 
		0xd2ac857c, 
		0xd1e4836b, 
		0x7707ece6, 
		0xbe599aa3, 
		0x0c363772, 
		0x3ba7a78d, 
		0x48667a88, 
		0xa8446b1c, 
		0x770c4fa7, 
		0xcc98f653, 
		0xb60c8287, 
		0x396cbdc5, 
		0xb361b6cf, 
		0x46b5d25f, 
		0x27c311de, 
		0x98c654df, 
		0xdaab9a51  
};
volatile uint32_t rand_ieee754_32[1536] __attribute__ ((aligned (2048))) = {
		0x66dde7e0, 
		0x41300000, 
		0xbfeaa6a1, 
		0x0e0001ff, 
		0x970a479d, 
		0x00000002, 
		0x3f800001, 
		0x0f7ffffc, 
		0x33333333, 
		0x33d597cf, 
		0x41500000, 
		0x80000040, 
		0x1b7bc717, 
		0x0f7ffffe, 
		0xc0800000, 
		0x0c7fe000, 
		0x42280000, 
		0x8f7ffffe, 
		0xaaaaaaaa, 
		0x96f46d7a, 
		0x0d000ff0, 
		0x4f1bd8a2, 
		0xa7c7b605, 
		0xc1000000, 
		0x292ca853, 
		0x84c2ef4d, 
		0xa8811978, 
		0x80000002, 
		0x00800002, 
		0xc1a00000, 
		0x00000000, 
		0xca51f516, 
		0x879cc17e, 
		0x3f800000, 
		0x80000000, 
		0x00040000, 
		0x42440000, 
		0x42040000, 
		0x04a498d3, 
		0xaa450f23, 
		0xc1c00000, 
		0xff800000, 
		0x7f800000, 
		0x0e0000ff, 
		0x00000100, 
		0xbf800001, 
		0x00000200, 
		0x315355a5, 
		0x7fc00000, 
		0x00080000, 
		0x00000002, 
		0x00000000, 
		0x00800001, 
		0x7f8fb5ca, 
		0x00000000, 
		0x057eecc7, 
		0x342463e6, 
		0x35ccb7d9, 
		0x41300000, 
		0x00040000, 
		0x8f7ffffd, 
		0xc07e6128, 
		0x4de0a623, 
		0x00000000, 
		0x0c7c0000, 
		0x097edff7, 
		0x80000000, 
		0x2fbca955, 
		0x80800003, 
		0x00000000, 
		0x7fffffff, 
		0x00002000, 
		0xc273b4cb, 
		0x67855d1c, 
		0x41f00000, 
		0x80000000, 
		0x0e00001f, 
		0x80000020, 
		0x8f7ffffd, 
		0x0c780000, 
		0x237fcfb3, 
		0x00004000, 
		0x80010000, 
		0x2ceabd64, 
		0x00000008, 
		0xc1b00000, 
		0x42380000, 
		0x80000100, 
		0x0c7fffe0, 
		0xf65ebee5, 
		0x80000008, 
		0x0e7fffff, 
		0x00000000, 
		0x00000000, 
		0x79972ef0, 
		0x42000000, 
		0xc1f00000, 
		0x3b635b33, 
		0x0c7ff800, 
		0x00800000, 
		0x8cab5f7d, 
		0x7e86ca30, 
		0x42180000, 
		0x42340000, 
		0x00011111, 
		0xa2407419, 
		0xc1f80000, 
		0x00595970, 
		0x7f800000, 
		0x421c0000, 
		0x0c7fffc0, 
		0xcb000000, 
		0x00008000, 
		0x41a80000, 
		0x42680000, 
		0x0e0fffff, 
		0x41600000, 
		0x80040000, 
		0x865b9e6f, 
		0x0e001fff, 
		0xc2600000, 
		0x40800000, 
		0x00000000, 
		0x00000200, 
		0x37d91975, 
		0x426c0000, 
		0x80800001, 
		0xc1300000, 
		0x0e0fffff, 
		0x007fffff, 
		0x41300000, 
		0x80ffffff, 
		0xc2380000, 
		0x7daee546, 
		0xcb000000, 
		0x80000000, 
		0x9604e427, 
		0x0c7fffe0, 
		0x41500000, 
		0xb4ece24c, 
		0x42280000, 
		0x80000010, 
		0x00800003, 
		0x0c7ffff8, 
		0x41c00000, 
		0x0e00000f, 
		0x41300000, 
		0x42740000, 
		0x7faeeb01, 
		0x41000000, 
		0xc2140000, 
		0x0f7ffffe, 
		0x0c7f0000, 
		0x0b8a590a, 
		0x0c7fff80, 
		0x80000800, 
		0x8f7ffffe, 
		0xff800000, 
		0x7fc00001, 
		0x80000000, 
		0x991fdef1, 
		0x41c80000, 
		0x80000100, 
		0xc1a80000, 
		0x80800003, 
		0x00000002, 
		0x00462008, 
		0x00000000, 
		0x80800003, 
		0x422c0000, 
		0x465e4547, 
		0x7fffffff, 
		0x660afa0c, 
		0x602a0432, 
		0x0e0000ff, 
		0x00000000, 
		0x0ff30312, 
		0x00000000, 
		0xffaa71b0, 
		0x807ffffe, 
		0x0e001fff, 
		0x5cdb3724, 
		0x80800002, 
		0x80800000, 
		0x00000000, 
		0x00000010, 
		0xc6401d57, 
		0x80001000, 
		0x42500000, 
		0xaf6f4432, 
		0x350b48a4, 
		0x88c13e91, 
		0x80008000, 
		0x460e8c5b, 
		0xffa101b8, 
		0x0e007fff, 
		0x80002000, 
		0x7f849a91, 
		0x17ecd10e, 
		0xc1600000, 
		0x2365fee8, 
		0x1d4add5c, 
		0x41980000, 
		0x0c7f0000, 
		0xed4ae1f6, 
		0x53645219, 
		0x93fe78e7, 
		0x00000000, 
		0xb2034a31, 
		0x80000000, 
		0xd84dcd80, 
		0x8f7fffff, 
		0x2f54488c, 
		0x947c871d, 
		0x7f000000, 
		0xc2040000, 
		0xff7ffffe, 
		0x42100000, 
		0xc0000000, 
		0x7f7ffffe, 
		0x40a00000, 
		0x0c7fe000, 
		0x80000000, 
		0x00000001, 
		0x0c7f0000, 
		0xcccccccc, 
		0x80000000, 
		0x42640000, 
		0x6fe6a2b9, 
		0x9298b303, 
		0xc3b9d3c7, 
		0x7d5202d3, 
		0xc1100000, 
		0x87d9806a, 
		0x00000000, 
		0x00000040, 
		0x0e3fffff, 
		0x42500000, 
		0xb15ff019, 
		0x42040000, 
		0x56b81ebc, 
		0xdde085de, 
		0xa8551f84, 
		0x0c1dbb38, 
		0xc2600000, 
		0x00ffffff, 
		0x80000000, 
		0x80004000, 
		0xea286fb3, 
		0x80800000, 
		0x00000000, 
		0x42480000, 
		0x00000000, 
		0x00000000, 
		0x118b65c4, 
		0xc757665c, 
		0xc1600000, 
		0x0c7ffc00, 
		0xc2000000, 
		0x7fa4f1f9, 
		0xef0513f1, 
		0x40c00000, 
		0x500ae83f, 
		0x7f000000, 
		0x80800002, 
		0xbc4d9c5a, 
		0xc5ffab82, 
		0xc23c0000, 
		0xadb92b92, 
		0x80000000, 
		0x68ed5ac8, 
		0x9d4866c2, 
		0x422c3364, 
		0x0e07ffff, 
		0xc0400000, 
		0x80000000, 
		0x0e000003, 
		0x420c0000, 
		0x40400000, 
		0x8649740f, 
		0xff800001, 
		0x80040000, 
		0x77c26bfd, 
		0xc1a00000, 
		0x007fffff, 
		0x0e00000f, 
		0x0c7ffff0, 
		0x00000001, 
		0xc1900000, 
		0x7fc00000, 
		0x80000000, 
		0xc0e70734, 
		0x0e0003ff, 
		0x3c36d0ae, 
		0xbc08a922, 
		0x00000080, 
		0x0c7ffc00, 
		0x007ffffe, 
		0x41b00000, 
		0x80000010, 
		0xc2140000, 
		0x42280000, 
		0x39235ae3, 
		0x7fffffff, 
		0x00000001, 
		0x80000100, 
		0x00000000, 
		0xc1800000, 
		0x4b000000, 
		0x00100000, 
		0xc2500000, 
		0xffffffff, 
		0xc20c0000, 
		0x80000000, 
		0xe04dfcd6, 
		0xc21c0000, 
		0xff875cb9, 
		0x7f800000, 
		0x00000000, 
		0x803b8715, 
		0x0f7ffffe, 
		0xc1d80000, 
		0x41b80000, 
		0xc2140000, 
		0x2d22242e, 
		0x8f7ffffe, 
		0x80000000, 
		0xff800000, 
		0xff800001, 
		0x0c7e0000, 
		0x7f88e8c0, 
		0x40000000, 
		0x97e4cb67, 
		0xb8724fde, 
		0xc2680000, 
		0x1a67032c, 
		0xc478d456, 
		0x80000200, 
		0x1a05bdb1, 
		0x219d80e9, 
		0xc1200000, 
		0x80000000, 
		0x42140000, 
		0x856a8972, 
		0x42000000, 
		0x41600000, 
		0x0e00001f, 
		0xc1880000, 
		0x420c0000, 
		0x80000000, 
		0x4956f1cd, 
		0x9bf5f421, 
		0x33333333, 
		0x0c7f8000, 
		0x0c7ffffc, 
		0x00800002, 
		0x42500000, 
		0x0d00fff0, 
		0x426c0000, 
		0x4f66d7ec, 
		0x0e1fffff, 
		0xc23c0000, 
		0x00040000, 
		0x9f167018, 
		0x0c7ffffc, 
		0x068a5de1, 
		0x926bae56, 
		0xbf800000, 
		0xc2100000, 
		0xc1100000, 
		0x80000800, 
		0x42080000, 
		0x00000000, 
		0x7f800000, 
		0x80002000, 
		0x00000000, 
		0x00000800, 
		0xc2440000, 
		0x7f800000, 
		0xaaaaaaaa, 
		0x0e03ffff, 
		0x2b39a533, 
		0xb82da827, 
		0x0e0000ff, 
		0x80000000, 
		0x80000000, 
		0xc1100000, 
		0xc1a00000, 
		0x62b87ce1, 
		0x42180000, 
		0x616f9bb5, 
		0x0c7ffc00, 
		0x80511f27, 
		0xa92dea18, 
		0x42440000, 
		0x00000000, 
		0x0c7e0000, 
		0x0c7ffffc, 
		0x00011111, 
		0x0d81cc95, 
		0xc2180000, 
		0x42440000, 
		0x4fc3e6d6, 
		0xc2400000, 
		0x68084dea, 
		0x98bbc612, 
		0x0c7ffffe, 
		0x0d00fff0, 
		0x0c600000, 
		0x41200000, 
		0xb1b9e01d, 
		0x41200000, 
		0x00000040, 
		0x0e03ffff, 
		0xc1e80000, 
		0x0c7fff00, 
		0x00011111, 
		0x803e9044, 
		0x41000000, 
		0x41500000, 
		0x7fbfffff, 
		0x422c0000, 
		0x0c7f0000, 
		0xff800000, 
		0x41500000, 
		0x2980dd41, 
		0xc1300000, 
		0x221a20a0, 
		0x0e7fffff, 
		0x41a80000, 
		0x00000000, 
		0x00800003, 
		0xc43d8e99, 
		0x80000020, 
		0xcd03565b, 
		0x33333333, 
		0x80800000, 
		0x00800002, 
		0xff7fffff, 
		0x42580000, 
		0x7fc00001, 
		0xc1f00000, 
		0x40400000, 
		0x7f96161d, 
		0x80800003, 
		0x315aace3, 
		0x0e00001f, 
		0x41880000, 
		0x80800001, 
		0x41300000, 
		0xff7ffffe, 
		0x2943175e, 
		0x42300000, 
		0x80100000, 
		0x7f000000, 
		0xc1700000, 
		0x42340000, 
		0x40400000, 
		0x8071a4ed, 
		0x3207f63a, 
		0x5d3f5063, 
		0xe0a61763, 
		0xcec225c5, 
		0xc1d00000, 
		0x42140000, 
		0xc2400000, 
		0x42480000, 
		0xb4d31d1f, 
		0x80000040, 
		0xc7b853de, 
		0x7eff1faa, 
		0xc1f80000, 
		0xf6c00e07, 
		0x0e0fffff, 
		0x55555555, 
		0x00008000, 
		0x0c7ffffc, 
		0x7f853114, 
		0xff800000, 
		0x80000000, 
		0x0f7ffffc, 
		0x0e000001, 
		0x80001000, 
		0xff800000, 
		0x61e1feb8, 
		0x42140000, 
		0x41000000, 
		0x41300000, 
		0x00400000, 
		0x3f028f5c, 
		0x41b80000, 
		0x00000000, 
		0x00000200, 
		0x00000000, 
		0x3599bae8, 
		0x27cd2971, 
		0xffc00000, 
		0x80000000, 
		0xc0400000, 
		0x41c80000, 
		0x42540000, 
		0x42500000, 
		0xc1800000, 
		0x00100000, 
		0x00000020, 
		0x43f6eb3d, 
		0x00008000, 
		0x00800000, 
		0x42300000, 
		0x4756a730, 
		0x956069a1, 
		0xc4645025, 
		0xffa0fdd8, 
		0xff9af2a5, 
		0x1bd8ab67, 
		0xc2540000, 
		0x7f7ffffe, 
		0x00000001, 
		0x4b8c4b40, 
		0xedaf37ff, 
		0x0e0000ff, 
		0x7f800000, 
		0x00000000, 
		0xce92c81e, 
		0x32ec6f01, 
		0xffe42e9d, 
		0x0c7ffe00, 
		0x42300000, 
		0x41500000, 
		0x41800000, 
		0x6d3a93ab, 
		0xc0a00000, 
		0xaaaaaaaa, 
		0x00800002, 
		0x0c7ffff8, 
		0x80000000, 
		0x0e00ffff, 
		0xffc00000, 
		0x3b49a6bc, 
		0xc1300000, 
		0x00000100, 
		0x0e001fff, 
		0x1ac19924, 
		0xff800000, 
		0x00004000, 
		0x3bed76ce, 
		0x0e1fffff, 
		0x40000000, 
		0xc2780000, 
		0x00080000, 
		0x41300000, 
		0x40c00000, 
		0x80000000, 
		0x41300000, 
		0x1f3f48ba, 
		0x3f800001, 
		0x51bb4ee3, 
		0xc1500000, 
		0xdbb73342, 
		0x7f800000, 
		0x41900000, 
		0xff800000, 
		0x426c0000, 
		0xc2000000, 
		0x41880000, 
		0x00000001, 
		0x00000000, 
		0xc1600000, 
		0xff800000, 
		0xf8dbdf0a, 
		0x41300000, 
		0x24beb306, 
		0x820b79df, 
		0xc1500000, 
		0xf84172b2, 
		0xba277603, 
		0x00000000, 
		0x7fffffff, 
		0x3f800000, 
		0x7fd47fde, 
		0xff800001, 
		0x7f800000, 
		0xb72fd280, 
		0x41100000, 
		0x8fb58184, 
		0x3dedf370, 
		0x01c3c426, 
		0xff800000, 
		0x89b892ec, 
		0x80100000, 
		0xff7ffffe, 
		0xc2540000, 
		0x00004000, 
		0x0c7fff00, 
		0x1610b2a0, 
		0xe52780e9, 
		0x13bb567b, 
		0xc1d00000, 
		0x18a6712d, 
		0xe9e9b50f, 
		0x5bf99329, 
		0x00000000, 
		0x00002000, 
		0x00400000, 
		0x15690839, 
		0x80000000, 
		0x80000000, 
		0x62c61c4e, 
		0xc1b00000, 
		0xc6991032, 
		0xc1d80000, 
		0xff7ffffe, 
		0x80004000, 
		0xc2180000, 
		0xe376ca86, 
		0x0e007fff, 
		0x80080000, 
		0x7f90e57a, 
		0x7f800000, 
		0x0e3fffff, 
		0x8431a42f, 
		0x8292aecd, 
		0x00000000, 
		0xae8f5710, 
		0x0d000ff0, 
		0x89c955b2, 
		0x1cd06d11, 
		0x33333333, 
		0xc2340000, 
		0x3125b769, 
		0xc2640000, 
		0x8e570d7f, 
		0x65d8fd2c, 
		0x3d49c3c7, 
		0x6e0bdf60, 
		0x3ff1d07e, 
		0x00800003, 
		0x085ab5c5, 
		0xff7ffffe, 
		0xff7ffffe, 
		0x80000000, 
		0x0e00000f, 
		0xbd6721ba, 
		0x42780000, 
		0xc2580000, 
		0x42300000, 
		0xff800000, 
		0xc20c0000, 
		0xc2680000, 
		0x41880000, 
		0xc967e0db, 
		0x0c600000, 
		0x80000000, 
		0xff97f5bc, 
		0x80000000, 
		0x00800001, 
		0x8f7f24af, 
		0x80000000, 
		0x00020000, 
		0x00000001, 
		0x80020000, 
		0xc1f00000, 
		0xc23c0000, 
		0xc0e00000, 
		0x060714f4, 
		0xa9a7e4c8, 
		0x40e00000, 
		0x425c0000, 
		0x11fddfd6, 
		0x76c87d5b, 
		0x41f80000, 
		0x0e000fff, 
		0x80800003, 
		0xb8415d27, 
		0x0c7f0000, 
		0xc1b80000, 
		0x133e45cb, 
		0xaaaaaaaa, 
		0x7f800000, 
		0x80800002, 
		0xa971c98c, 
		0x0c7ffff0, 
		0x80000000, 
		0x00000000, 
		0x44f9d0a7, 
		0xff7fffff, 
		0x7dc9bb6e, 
		0xff82df23, 
		0xbf800000, 
		0xff800000, 
		0x80800002, 
		0x0c7ffc00, 
		0x5620aab5, 
		0x00800000, 
		0xc2180000, 
		0xa6f71bf6, 
		0xc2380000, 
		0xff800000, 
		0x7f800000, 
		0x7fc00000, 
		0x3f800000, 
		0x80000100, 
		0xf04eb1b9, 
		0xc2780000, 
		0x80000002, 
		0xd9244890, 
		0x00001000, 
		0xc2080000, 
		0x00000000, 
		0x00010000, 
		0xbacebd36, 
		0x41b00000, 
		0x98eeed1d, 
		0xef6ab293, 
		0x80000000, 
		0xc1980000, 
		0xc1900000, 
		0xc1f80000, 
		0x7fb4b449, 
		0x00000000, 
		0x41000000, 
		0x80000000, 
		0x0c7ffe00, 
		0x00000000, 
		0x7fc00001, 
		0xfaa1c82a, 
		0xcfeaa060, 
		0x42640000, 
		0x41880000, 
		0x7eeb4752, 
		0xc1d80000, 
		0x80000010, 
		0x7f800000, 
		0x7f000000, 
		0xc7ba8eaa, 
		0x41b00000, 
		0x2ce18cde, 
		0xc0c00000, 
		0x89d1e801, 
		0x427c0000, 
		0x0f7fffff, 
		0x41700000, 
		0x33333333, 
		0x3a6e3d8f, 
		0x42780000, 
		0x0c7fff00, 
		0x42380000, 
		0x00001000, 
		0xff800000, 
		0x0c7ffc00, 
		0x0e1fffff, 
		0x982c4f9e, 
		0xfb406a4c, 
		0xc20c0000, 
		0x45ff2b9d, 
		0x4b5f08ed, 
		0xffc00001, 
		0x41880000, 
		0x41e80000, 
		0x0d00fff0, 
		0xc2340000, 
		0xa39fc3c4, 
		0x0e0f9591, 
		0x7fbfffff, 
		0xc2580000, 
		0x80000000, 
		0xff7ffffe, 
		0x41f00000, 
		0xff7ffffe, 
		0x7f800000, 
		0x80010000, 
		0x8f7ffffc, 
		0xc1980000, 
		0x2e6111e7, 
		0x00200000, 
		0x8f7ffffc, 
		0x80000000, 
		0x0c7fc000, 
		0xc1c80000, 
		0x80080000, 
		0x42000000, 
		0xc2040000, 
		0x42480000, 
		0xc1d80000, 
		0x00100000, 
		0xc2100000, 
		0x3f028f5c, 
		0x42200000, 
		0xc2700000, 
		0x80000000, 
		0x41b80000, 
		0x00800003, 
		0x1e57c982, 
		0x0c7fffff, 
		0x4b8c4b40, 
		0xc2180000, 
		0x7f800000, 
		0x3f800000, 
		0xcb000000, 
		0x0e001fff, 
		0xffffffff, 
		0xc2500000, 
		0xff800001, 
		0x7fc00000, 
		0x489f448f, 
		0x91a308ae, 
		0x80800003, 
		0x40e00000, 
		0x00000020, 
		0x7f800000, 
		0x41800000, 
		0xc0e00000, 
		0x0e0fffff, 
		0x806b0682, 
		0x41800000, 
		0x80000001, 
		0x426c0000, 
		0x0c7ffff0, 
		0x7f7ffffe, 
		0x41e80000, 
		0xc1000000, 
		0xc2580000, 
		0x80800000, 
		0x40000000, 
		0x42680000, 
		0xc27c0000, 
		0xf7b36ff3, 
		0x80011111, 
		0x00ffffff, 
		0xc2200000, 
		0xac831b24, 
		0x0f7ffffe, 
		0x00000010, 
		0xc0000000, 
		0x00000000, 
		0x7f7ffffe, 
		0x42240000, 
		0x0e03ffff, 
		0x35f3695d, 
		0x00800003, 
		0x41f80000, 
		0xc26c0000, 
		0x42540000, 
		0x7f800000, 
		0x7f7fffff, 
		0x10a7abba, 
		0x80000020, 
		0x80000002, 
		0xff7fffff, 
		0x422c0000, 
		0x01ddd5a1, 
		0x635fc385, 
		0x0fbf366c, 
		0x00000001, 
		0xc273b126, 
		0x00000008, 
		0xdf44a5d6, 
		0xc1a00000, 
		0x80800002, 
		0x80ca8bab, 
		0x41880000, 
		0x33124f0a, 
		0x80000080, 
		0x41884220, 
		0x007fffff, 
		0x43d492a3, 
		0x0c7fe000, 
		0x80000000, 
		0x41600000, 
		0xb757fde4, 
		0x2e14ac0d, 
		0xc25c0000, 
		0x24576b48, 
		0xa79dab32, 
		0x41900000, 
		0xc1400000, 
		0x42480000, 
		0x0bf1a22f, 
		0x7fb5d94d, 
		0x8455f84a, 
		0xc26c0000, 
		0x80000000, 
		0x0e0007ff, 
		0x42480000, 
		0x00002000, 
		0x422c0000, 
		0xc2500000, 
		0x80000004, 
		0xc2140000, 
		0x132242b0, 
		0x272542fe, 
		0x58a875ab, 
		0x80000000, 
		0xc2380000, 
		0x936de149, 
		0x00000000, 
		0x41600000, 
		0x00400000, 
		0xff800000, 
		0xffffffff, 
		0xff800000, 
		0x446ec1f8, 
		0x42f9b612, 
		0x80000000, 
		0x0e00007f, 
		0x00400000, 
		0x0d000ff0, 
		0x00000020, 
		0xb1c785c4, 
		0x57ca036e, 
		0x0c7fffe0, 
		0x42700000, 
		0x4288cb70, 
		0x3a37980b, 
		0xf92585f0, 
		0x0c7e0000, 
		0xc0a00000, 
		0x7f800000, 
		0x7f800000, 
		0x80000040, 
		0xcb000000, 
		0x86dd5b67, 
		0x7fffffff, 
		0x00000000, 
		0xc0a00000, 
		0x80000400, 
		0x40800000, 
		0x427c0000, 
		0xb161c2a2, 
		0xff7ffffe, 
		0x8c216a18, 
		0x80800001, 
		0x41700000, 
		0x80800000, 
		0xc1000000, 
		0xedce09ff, 
		0x932eea3f, 
		0xc2480000, 
		0xc1900000, 
		0x42200000, 
		0x00000040, 
		0x00000000, 
		0x6d9513a8, 
		0x42740000, 
		0xfe60da4f, 
		0x42740000, 
		0xff800000, 
		0x0f7ffffc, 
		0x421c0000, 
		0xc1700000, 
		0x00011111, 
		0xc2580000, 
		0x7f7fffff, 
		0xc1400000, 
		0xc2480000, 
		0xc2380000, 
		0x42300000, 
		0x0e000001, 
		0x27dea548, 
		0xff8cf923, 
		0x7f7ffffe, 
		0x421c0000, 
		0x2681f922, 
		0x41d00000, 
		0x00011111, 
		0xcc06a077, 
		0xc2600000, 
		0x0e007fff, 
		0x8b5788b0, 
		0x40000000, 
		0x350b21fc, 
		0xc1900000, 
		0x0e003fff, 
		0xc1100000, 
		0x7f7ffffe, 
		0xc1500000, 
		0x55af6a9d, 
		0x80020000, 
		0x41c80000, 
		0x42700000, 
		0x0e00003f, 
		0x00000100, 
		0x41700000, 
		0xbf800000, 
		0xc65ce9c2, 
		0xff800000, 
		0x00000000, 
		0x0e00001f, 
		0x80000002, 
		0xc27c0000, 
		0xff800000, 
		0x321bee69, 
		0x80000200, 
		0x00000000, 
		0x8f7ffffd, 
		0x587080aa, 
		0x72226f4d, 
		0x41200000, 
		0xad120e13, 
		0x42240000, 
		0x42400000, 
		0xd27dd590, 
		0x80011111, 
		0x0e001fff, 
		0x8a410484, 
		0xc23c0000, 
		0x0e00001f, 
		0x80000000, 
		0xc1200000, 
		0xc2100000, 
		0x80000000, 
		0xb05e9b70, 
		0xffffffff, 
		0x0e00007f, 
		0x80000000, 
		0xb65fa1fb, 
		0xb3f401f3, 
		0xf5151c5f, 
		0xc2140000, 
		0x0e001fff, 
		0xbcea5ddf, 
		0xc24c0000, 
		0xc1b00000, 
		0x422c0000, 
		0x3f800000, 
		0x00000020, 
		0x42600000, 
		0x774cf82c, 
		0xa48f6b1e, 
		0x00000400, 
		0xd5df6637, 
		0x80000000, 
		0xb1038b45, 
		0x0c7ffe00, 
		0x9863eb4b, 
		0x0c7fffe0, 
		0x80000000, 
		0x0507daac, 
		0x42640000, 
		0x00000200, 
		0x0e0001ff, 
		0xffb94fc3, 
		0xc20c0000, 
		0x807ffffe, 
		0x00000000, 
		0x80800000, 
		0x0c600000, 
		0xd556a6d7, 
		0x41100000, 
		0xc1e80000, 
		0xc8fafdde, 
		0x0c7fffff, 
		0x0c7e0000, 
		0x41880000, 
		0x00080000, 
		0xf96fd4a2, 
		0x844dc1ec, 
		0xf414f783, 
		0x1da76cb0, 
		0xc20c0000, 
		0x55ea0c0d, 
		0xc0400000, 
		0x00010000, 
		0x7fbfffff, 
		0x42000000, 
		0x007ffffe, 
		0xc2280000, 
		0x31b5979d, 
		0x80800001, 
		0x0c7f8000, 
		0x80800002, 
		0x7f800000, 
		0x0c7ffffc, 
		0x00001000, 
		0xb055c918, 
		0x3698b257, 
		0x41300000, 
		0x3ef2a5ea, 
		0x00011111, 
		0x007fffff, 
		0x41b80000, 
		0xc1a80000, 
		0x42040000, 
		0x80000001, 
		0x80000001, 
		0xcd631c02, 
		0xff800000, 
		0x00011111, 
		0x80000000, 
		0x80800001, 
		0x0e00000f, 
		0x41200000, 
		0x0e007fff, 
		0x7f7ffffe, 
		0x40400000, 
		0x80000000, 
		0x00800003, 
		0x7f800000, 
		0x3f800001, 
		0xbf800000, 
		0x00020000, 
		0x0b5708a6, 
		0x80000004, 
		0x41980000, 
		0xd6af87bb, 
		0x4b8c4b40, 
		0xc2540000, 
		0x0e1fffff, 
		0xffc00001, 
		0x0e003fff, 
		0x80800002, 
		0x00800001, 
		0x80000000, 
		0xc0a00000, 
		0xdb1e8fc5, 
		0x80000800, 
		0x00000010, 
		0x00040000, 
		0x40c00000, 
		0x41000000, 
		0x40400000, 
		0x41980000, 
		0x007fffff, 
		0x7eef8b1d, 
		0x74a3955b, 
		0xe17156dc, 
		0x00000000, 
		0xc24c0000, 
		0x436962f4, 
		0x12b80556, 
		0x40400000, 
		0x98d16d89, 
		0xdaf4cdd5, 
		0xb73f5464, 
		0x80000000, 
		0x0c7ffe00, 
		0x80000000, 
		0x80000040, 
		0x55f0fe7e, 
		0x765b3c08, 
		0xc746cb42, 
		0xc22c0000, 
		0x80236318, 
		0x00000000, 
		0xffb2ff54, 
		0x80000000, 
		0x0e0003ff, 
		0x425c0000, 
		0x80080000, 
		0xc23c0000, 
		0x42380000, 
		0x7fb53bca, 
		0x44706d24, 
		0x00000002, 
		0x8ce6d342, 
		0xff800000, 
		0x80000001, 
		0x00000001, 
		0x946e1b7a, 
		0x41a00000, 
		0x7f800000, 
		0xc1b80000, 
		0xb909ec65, 
		0x095e62da, 
		0xca8567a9, 
		0xf1d9ee24, 
		0xc2740000, 
		0x42040000, 
		0xe2689861, 
		0x0c7ffff8, 
		0x00800001, 
		0x41500000, 
		0x7f7ffffe, 
		0x00400000, 
		0x807fffff, 
		0x7fbfffff, 
		0xff800000, 
		0x00000000, 
		0xd603da79, 
		0xc1f80000, 
		0xff9f752d, 
		0x41500000, 
		0x0e000003, 
		0x41300000, 
		0xff85e494, 
		0x8f7ffffe, 
		0xff7ffffe, 
		0x41100000, 
		0xcc6edd5a, 
		0xa6202746, 
		0x8d08058b, 
		0x00000400, 
		0x00080000, 
		0xa7029ce7, 
		0x0e00001f, 
		0xb3066d5a, 
		0x80000002, 
		0xff800000, 
		0x80000000, 
		0xfb68f6f9, 
		0x00000001, 
		0x7f800000, 
		0x00000000, 
		0xb91d4fa4, 
		0x80000004, 
		0x7f8d44d8, 
		0x80010000, 
		0x0c400000, 
		0xc4f6a0d7, 
		0xff800000, 
		0xcb8c4b40, 
		0x41500000, 
		0xaaaaaaaa, 
		0xc0000000, 
		0x0e0001ff, 
		0x0e000003, 
		0x10275576, 
		0x42480000, 
		0x42100000, 
		0xffaf5e0c, 
		0x80800001, 
		0x0e00001f, 
		0x0c7fff80, 
		0x7fc00000, 
		0x0c7ffe00, 
		0xc1c80000, 
		0xc1b00000, 
		0x00400000, 
		0x00011111, 
		0xf342f2be, 
		0xffaf407f, 
		0x80000001, 
		0x80002000, 
		0x80000000, 
		0x975b69e9, 
		0x80000200, 
		0x41400000, 
		0x952b1058, 
		0x95d62bac, 
		0x6efc54fc, 
		0x0e007fff, 
		0x0c7ffc00, 
		0x7f800000, 
		0xd8c79d2f, 
		0xc25c0000, 
		0x0c7fe000, 
		0x724048a7, 
		0x00000001, 
		0xffc93bdc, 
		0x80800003, 
		0x3e8dd8a7, 
		0xc24c0000, 
		0xe8c839b8, 
		0x619f8a3a, 
		0x41c00000, 
		0x0d000ff0, 
		0x3f800000, 
		0x7f800000, 
		0x00800002, 
		0xc1e00000, 
		0xb685d915, 
		0xcb8c4b40, 
		0x80000400, 
		0x0e00003f, 
		0x0e000fff, 
		0xf33928e1, 
		0xa79d0cd2, 
		0x80000000, 
		0x80000000, 
		0x5f748e89, 
		0x80002000, 
		0xc0c00000, 
		0x0e001fff, 
		0xc5d8e646, 
		0xc23c0000, 
		0xff7ffffe, 
		0x80000000, 
		0xc2700000, 
		0x7fffffff, 
		0xd28e76f6, 
		0xa855fb88, 
		0x81e05a93, 
		0x7fc00001, 
		0xff800000, 
		0x00000000, 
		0x5e144c2f, 
		0xc7db024a, 
		0x86d0b78e, 
		0xc1a80000, 
		0xc2680000, 
		0xff800000, 
		0x0f7ffffc, 
		0x80800003, 
		0xc2340000, 
		0x437ec810, 
		0x41e80000, 
		0x80800003, 
		0xd668572e, 
		0x00000000, 
		0x0e000fff, 
		0x0f7ffffd, 
		0x00000010, 
		0x80800001, 
		0x0f7ffffe, 
		0x0c7ff800, 
		0x7f2d8ce6, 
		0xff802b45, 
		0xc1900000, 
		0xc1e80000, 
		0x5ea3ad4b, 
		0xbf800000, 
		0x0c7ffe00, 
		0xa237f37a, 
		0x0e001fff, 
		0x4b24a333, 
		0xb67c157a, 
		0x4249705a, 
		0x3b71dafd, 
		0x00000000, 
		0x0c7ffffe, 
		0x905b5aff, 
		0x057496a6, 
		0x80002000, 
		0x41880000, 
		0xe1a7ad04, 
		0x9244fc26, 
		0x55a846cb, 
		0x0ca3667f, 
		0x42480000, 
		0x00011111, 
		0x007ffffe, 
		0x80001000, 
		0x00000000, 
		0x3f570452, 
		0x7f800000, 
		0x6e24aeb6, 
		0x00200000, 
		0x0e00ffff, 
		0xb1e91a26, 
		0xc1400000, 
		0x7f000000, 
		0x42600000, 
		0x41c80000, 
		0x00000000, 
		0x4ce0eca1, 
		0xcb8c4b40, 
		0x0c780000, 
		0x421c0000, 
		0x40000000, 
		0xff800000, 
		0x6f38935c, 
		0x00011111, 
		0x54773bcb, 
		0x41400000, 
		0x0c7fff00, 
		0xff800000, 
		0x80000008, 
		0xc1c00000, 
		0x7321f895, 
		0xc23c0000, 
		0x80545383, 
		0x77127468, 
		0xff8f791a, 
		0xc2540000, 
		0x0012f138, 
		0xbf800001, 
		0x0e00001f, 
		0x0e0007ff, 
		0xc1200000, 
		0x41f00000, 
		0x80800002, 
		0xeebb87c7, 
		0x3f800000, 
		0x426c0000, 
		0xc1880000, 
		0xa2ac77d9, 
		0x7f800000, 
		0x42700000, 
		0x7f800000, 
		0x0c7ffffe, 
		0x42400000, 
		0x80ffffff, 
		0x0e003fff, 
		0x4bce17ba, 
		0xbf028f5c, 
		0x80001000, 
		0x41f00000, 
		0x0c7fe000, 
		0x41b80000, 
		0x00000002, 
		0xc2200000, 
		0x3f800000, 
		0x7ff96a20, 
		0x00000002, 
		0x66b4dbeb, 
		0x0c7fffe0, 
		0xc0c00000, 
		0xbf800000, 
		0xc1b00000, 
		0x306238d7, 
		0x80000000, 
		0x42240000, 
		0x41000000, 
		0x0c7ffe00, 
		0xff800000, 
		0x00000020, 
		0x80040000, 
		0x54e261dc, 
		0x41880000, 
		0x14c5ee41, 
		0x80000000, 
		0x9aeed54a, 
		0x80000000, 
		0xc27c0000, 
		0x00000080, 
		0x423c0000, 
		0x41a00000, 
		0x47f63b88, 
		0x8f7ffffe, 
		0x80000100, 
		0x41600000, 
		0x0e000001, 
		0xff7ffffe, 
		0x80000000, 
		0x23f38a0b, 
		0x0c7f8000, 
		0x0e00ffff, 
		0x7f9dd277, 
		0xc0a00000, 
		0x00000000, 
		0x80000000, 
		0x80000001, 
		0x80200000, 
		0xffc00001, 
		0xc3b447fa, 
		0x41d00000, 
		0x55555555, 
		0x0e00001f, 
		0x41800000, 
		0x40800000, 
		0x41880000, 
		0x0c600000, 
		0x0e0007ff, 
		0x42480000, 
		0x42600000, 
		0x00100000, 
		0x00000000, 
		0xde632876, 
		0xc1900000, 
		0xc2ec6073, 
		0x0ce432d6, 
		0x0c7e0000, 
		0x82b1ef98, 
		0x0c7ffff0, 
		0x0502ed06, 
		0x0e000fff, 
		0x80001000, 
		0xffc441a5, 
		0x0e01ffff, 
		0x00000040, 
		0x1711001a, 
		0xff84904e, 
		0x0e000fff, 
		0xc1a80000, 
		0x7f800000, 
		0x004635a0, 
		0xe52180d2, 
		0x0c600000, 
		0x80000004, 
		0x80000000, 
		0x00000002, 
		0x41900000, 
		0x80000000, 
		0x0c400000, 
		0xc2740000, 
		0x80000400, 
		0x0fc3f37a, 
		0xbe24cc7c, 
		0xc1e80000, 
		0x00000000, 
		0x42640000, 
		0x0c700000, 
		0x424c0000, 
		0x41500000, 
		0xc1700000, 
		0xc25c0000, 
		0x80000000, 
		0xc0c00000, 
		0x00800000, 
		0xc2580000, 
		0xe50b21f2, 
		0xf6989861, 
		0x80000001, 
		0x42540000, 
		0x7f800000, 
		0xf750f032, 
		0x42700000, 
		0x8f7ffffd, 
		0x80008000, 
		0x00080000, 
		0x00000010, 
		0xbf800000, 
		0xcb8c4b40, 
		0xc1a00000, 
		0x5fdd3470, 
		0xa984a111, 
		0x40800000, 
		0x81a47fa2, 
		0xc1b80000, 
		0xc25c0000, 
		0x7f800000, 
		0x0c600000, 
		0x0d00fff0, 
		0xc1500000, 
		0x0e6b259a, 
		0xadd4e8e9, 
		0x00800000, 
		0x7fc00000, 
		0x0c7e0000, 
		0x0c7ff000, 
		0x7fc00001, 
		0xcb8c4b40, 
		0xff9e61b7, 
		0x00000000, 
		0xff800001, 
		0xc2500000, 
		0xffe6e2e2, 
		0x80800003, 
		0x41500000, 
		0xc25c0000, 
		0x41c00000, 
		0xbb5b3081, 
		0x40e0810b, 
		0x0d00fff0, 
		0xc1700000, 
		0x0e003fff  
};
volatile uint32_t self_check_32[1024] __attribute__ ((aligned (2048))) = {
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000, 
		0x3f800000  
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

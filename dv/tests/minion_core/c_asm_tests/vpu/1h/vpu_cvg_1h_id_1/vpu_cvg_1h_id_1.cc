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
			"csrwi mhpmevent3, 18\n"   
			"csrwi mhpmevent4, 6\n"   
			"csrwi mhpmevent5, 20\n"   
			"csrwi mhpmevent6, 16\n"   
			"csrwi mhpmevent7, 3\n"   
			"csrwi mhpmevent8, 6\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f17,  544 (x5)\n"
			"flw.ps f18,  192 (x5)\n"
			"flw.ps f12,  800 (x5)\n"
			"flw.ps f21,  768 (x5)\n"
			"flw.ps f9,  896 (x5)\n"
			"flw.ps f3,  832 (x5)\n"
			"flw.ps f16,  576 (x5)\n"
			"flw.ps f5,  960 (x5)\n"
			"flw.ps f29,  32 (x5)\n"
			"flw.ps f19,  672 (x5)\n"
			"flw.ps f10,  64 (x5)\n"
			"flw.ps f2,  256 (x5)\n"
			"flw.ps f8,  416 (x5)\n"
			"flw.ps f0,  96 (x5)\n"
			"flw.ps f27,  640 (x5)\n"
			"flw.ps f11,  864 (x5)\n"
			"flw.ps f14,  224 (x5)\n"
			"flw.ps f30,  480 (x5)\n"
			"flw.ps f7,  608 (x5)\n"
			"flw.ps f25,  320 (x5)\n"
			"flw.ps f13,  448 (x5)\n"
			"flw.ps f1,  160 (x5)\n"
			"flw.ps f6,  0 (x5)\n"
			"flw.ps f31,  928 (x5)\n"
			"flw.ps f22,  992 (x5)\n"
			"flw.ps f26,  512 (x5)\n"
			"flw.ps f15,  384 (x5)\n"
			"flw.ps f20,  128 (x5)\n"
			"flw.ps f23,  736 (x5)\n"
			"flw.ps f24,  704 (x5)\n"
			"flw.ps f28,  352 (x5)\n"
			"flw.ps f4,  288 (x5)\n"
			"LBL_VADDR_INIT_HID_0:\n"          
			"li x19, 0x8100b602e0 \n"                 
			"fsw.ps f9, (x19) \n"                 
			"li x21, 0x8100b66000 \n"                 
			"fsw.ps f22, (x21) \n"                 
			"li x25, 0x8100bb7000 \n"                 
			"fsw.ps f28, (x25) \n"                 
			"li x20, 0x8100e88800 \n"                 
			"fsw.ps f1, (x20) \n"                 
			"li x14, 0x810146d540 \n"                 
			"fsw.ps f7, (x14) \n"                 
			"li x11, 0x81014c0020 \n"                 
			"fsw.ps f17, (x11) \n"                 
			"li x25, 0x8101ae0160 \n"                 
			"fsw.ps f6, (x25) \n"                 
			"li x12, 0x8101d60320 \n"                 
			"fsw.ps f15, (x12) \n"                 
			"li x11, 0x8101d6a800 \n"                 
			"fsw.ps f28, (x11) \n"                 
			"li x25, 0x81020a0760 \n"                 
			"fsw.ps f23, (x25) \n"                 
			"li x16, 0x810284ae40 \n"                 
			"fsw.ps f26, (x16) \n"                 
			"li x25, 0x8102b207e0 \n"                 
			"fsw.ps f12, (x25) \n"                 
			"li x18, 0x8103140020 \n"                 
			"fsw.ps f9, (x18) \n"                 
			"li x17, 0x810376e3e0 \n"                 
			"fsw.ps f19, (x17) \n"                 
			"li x16, 0x81038a0000 \n"                 
			"fsw.ps f22, (x16) \n"                 
			"li x24, 0x8103f98840 \n"                 
			"fsw.ps f26, (x24) \n"                 
			"li x14, 0x8103fe17e0 \n"                 
			"fsw.ps f30, (x14) \n"                 
			"li x15, 0x8104820740 \n"                 
			"fsw.ps f13, (x15) \n"                 
			"li x10, 0x81048c3820 \n"                 
			"fsw.ps f27, (x10) \n"                 
			"li x23, 0x81048ca000 \n"                 
			"fsw.ps f22, (x23) \n"                 
			"li x21, 0x81049a0300 \n"                 
			"fsw.ps f6, (x21) \n"                 
			"li x13, 0x8104db4000 \n"                 
			"fsw.ps f8, (x13) \n"                 
			"li x19, 0x8104f3f820 \n"                 
			"fsw.ps f29, (x19) \n"                 
			"li x24, 0x81050a05a0 \n"                 
			"fsw.ps f3, (x24) \n"                 
			"li x16, 0x81050f6a00 \n"                 
			"fsw.ps f24, (x16) \n"                 
			"li x17, 0x81054e0040 \n"                 
			"fsw.ps f20, (x17) \n"                 
			"li x24, 0x8106160b60 \n"                 
			"fsw.ps f24, (x24) \n"                 
			"li x24, 0x8106360000 \n"                 
			"fsw.ps f5, (x24) \n"                 
			"li x16, 0x81066c0000 \n"                 
			"fsw.ps f10, (x16) \n"                 
			"li x14, 0x81068602c0 \n"                 
			"fsw.ps f16, (x14) \n"                 
			"li x24, 0x81069306e0 \n"                 
			"fsw.ps f29, (x24) \n"                 
			"li x20, 0x8107093020 \n"                 
			"fsw.ps f8, (x20) \n"                 
			"li x17, 0x8107392080 \n"                 
			"fsw.ps f18, (x17) \n"                 
			"li x16, 0x810768c820 \n"                 
			"fsw.ps f1, (x16) \n"                 
			"li x14, 0x8107c4ea20 \n"                 
			"fsw.ps f20, (x14) \n"                 
			"li x25, 0x8107fb0020 \n"                 
			"fsw.ps f9, (x25) \n"                 
			"li x13, 0x81082400a0 \n"                 
			"fsw.ps f5, (x13) \n"                 
			"li x15, 0x8108ca07c0 \n"                 
			"fsw.ps f8, (x15) \n"                 
			"li x10, 0x81097ac4e0 \n"                 
			"fsw.ps f11, (x10) \n"                 
			"li x19, 0x8109860520 \n"                 
			"fsw.ps f9, (x19) \n"                 
			"li x15, 0x8109895bc0 \n"                 
			"fsw.ps f0, (x15) \n"                 
			"li x17, 0x8109a00000 \n"                 
			"fsw.ps f16, (x17) \n"                 
			"li x13, 0x8109b20000 \n"                 
			"fsw.ps f23, (x13) \n"                 
			"li x14, 0x8109ba0060 \n"                 
			"fsw.ps f1, (x14) \n"                 
			"li x21, 0x810a660020 \n"                 
			"fsw.ps f31, (x21) \n"                 
			"li x21, 0x810ac68f20 \n"                 
			"fsw.ps f3, (x21) \n"                 
			"li x24, 0x810b0600c0 \n"                 
			"fsw.ps f5, (x24) \n"                 
			"li x18, 0x810b200100 \n"                 
			"fsw.ps f21, (x18) \n"                 
			"li x23, 0x810b45c820 \n"                 
			"fsw.ps f21, (x23) \n"                 
			"li x21, 0x810b53d9e0 \n"                 
			"fsw.ps f1, (x21) \n"                 
			"li x17, 0x810b5a0160 \n"                 
			"fsw.ps f4, (x17) \n"                 
			"li x25, 0x810bbc0000 \n"                 
			"fsw.ps f0, (x25) \n"                 
			"li x16, 0x810bfb6e80 \n"                 
			"fsw.ps f18, (x16) \n"                 
			"li x20, 0x810c4c0820 \n"                 
			"fsw.ps f0, (x20) \n"                 
			"li x23, 0x810cb82820 \n"                 
			"fsw.ps f5, (x23) \n"                 
			"li x23, 0x810cbe0680 \n"                 
			"fsw.ps f12, (x23) \n"                 
			"li x19, 0x810cf80020 \n"                 
			"fsw.ps f12, (x19) \n"                 
			"li x14, 0x810d878f60 \n"                 
			"fsw.ps f22, (x14) \n"                 
			"li x12, 0x810e3a0000 \n"                 
			"fsw.ps f11, (x12) \n"                 
			"li x16, 0x810e3c0020 \n"                 
			"fsw.ps f25, (x16) \n"                 
			"li x23, 0x810e580640 \n"                 
			"fsw.ps f12, (x23) \n"                 
			"li x15, 0x810ef27800 \n"                 
			"fsw.ps f18, (x15) \n"                 
			"li x19, 0x810fd00000 \n"                 
			"fsw.ps f3, (x19) \n"                 
			"li x21, 0x8200040020 \n"                 
			"fsw.ps f27, (x21) \n"                 
			"li x23, 0x8201fb8800 \n"                 
			"fsw.ps f0, (x23) \n"                 
			"li x12, 0x8202680020 \n"                 
			"fsw.ps f19, (x12) \n"                 
			"li x22, 0x8202ac0020 \n"                 
			"fsw.ps f4, (x22) \n"                 
			"li x21, 0x820322e800 \n"                 
			"fsw.ps f4, (x21) \n"                 
			"li x18, 0x82034601e0 \n"                 
			"fsw.ps f7, (x18) \n"                 
			"li x12, 0x82034e0000 \n"                 
			"fsw.ps f22, (x12) \n"                 
			"li x10, 0x82038a0000 \n"                 
			"fsw.ps f24, (x10) \n"                 
			"li x17, 0x82038d2760 \n"                 
			"fsw.ps f21, (x17) \n"                 
			"li x19, 0x820395c000 \n"                 
			"fsw.ps f11, (x19) \n"                 
			"li x16, 0x8203fe07c0 \n"                 
			"fsw.ps f21, (x16) \n"                 
			"li x20, 0x8204540240 \n"                 
			"fsw.ps f18, (x20) \n"                 
			"li x17, 0x8204a002a0 \n"                 
			"fsw.ps f22, (x17) \n"                 
			"li x12, 0x8204a21a40 \n"                 
			"fsw.ps f26, (x12) \n"                 
			"li x18, 0x8204a80000 \n"                 
			"fsw.ps f30, (x18) \n"                 
			"li x14, 0x8204b60660 \n"                 
			"fsw.ps f28, (x14) \n"                 
			"li x13, 0x8204ee0020 \n"                 
			"fsw.ps f29, (x13) \n"                 
			"li x20, 0x8204f4b000 \n"                 
			"fsw.ps f12, (x20) \n"                 
			"li x25, 0x82050a0000 \n"                 
			"fsw.ps f10, (x25) \n"                 
			"li x23, 0x8205280000 \n"                 
			"fsw.ps f17, (x23) \n"                 
			"li x14, 0x8205744700 \n"                 
			"fsw.ps f29, (x14) \n"                 
			"li x22, 0x8205928800 \n"                 
			"fsw.ps f12, (x22) \n"                 
			"li x21, 0x8205e1a7c0 \n"                 
			"fsw.ps f31, (x21) \n"                 
			"li x17, 0x8206ec0720 \n"                 
			"fsw.ps f23, (x17) \n"                 
			"li x10, 0x8206f00020 \n"                 
			"fsw.ps f16, (x10) \n"                 
			"li x16, 0x8206f32820 \n"                 
			"fsw.ps f29, (x16) \n"                 
			"li x16, 0x8207060000 \n"                 
			"fsw.ps f15, (x16) \n"                 
			"li x14, 0x82074f8800 \n"                 
			"fsw.ps f6, (x14) \n"                 
			"li x21, 0x8207c20000 \n"                 
			"fsw.ps f8, (x21) \n"                 
			"li x12, 0x8207f37800 \n"                 
			"fsw.ps f26, (x12) \n"                 
			"li x19, 0x8208780000 \n"                 
			"fsw.ps f23, (x19) \n"                 
			"li x17, 0x8209000020 \n"                 
			"fsw.ps f0, (x17) \n"                 
			"li x10, 0x82090005e0 \n"                 
			"fsw.ps f29, (x10) \n"                 
			"li x25, 0x8209260720 \n"                 
			"fsw.ps f28, (x25) \n"                 
			"li x12, 0x820928d820 \n"                 
			"fsw.ps f1, (x12) \n"                 
			"li x16, 0x820934c800 \n"                 
			"fsw.ps f24, (x16) \n"                 
			"li x12, 0x82093ae160 \n"                 
			"fsw.ps f14, (x12) \n"                 
			"li x11, 0x820976f820 \n"                 
			"fsw.ps f9, (x11) \n"                 
			"li x10, 0x8209820020 \n"                 
			"fsw.ps f29, (x10) \n"                 
			"li x15, 0x8209977580 \n"                 
			"fsw.ps f5, (x15) \n"                 
			"li x10, 0x820a4c0020 \n"                 
			"fsw.ps f21, (x10) \n"                 
			"li x17, 0x820a565020 \n"                 
			"fsw.ps f29, (x17) \n"                 
			"li x19, 0x820a6adc20 \n"                 
			"fsw.ps f3, (x19) \n"                 
			"li x18, 0x820a8000a0 \n"                 
			"fsw.ps f16, (x18) \n"                 
			"li x15, 0x820ac267c0 \n"                 
			"fsw.ps f29, (x15) \n"                 
			"li x20, 0x820acc0420 \n"                 
			"fsw.ps f14, (x20) \n"                 
			"li x22, 0x820ad003c0 \n"                 
			"fsw.ps f1, (x22) \n"                 
			"li x13, 0x820b43d020 \n"                 
			"fsw.ps f26, (x13) \n"                 
			"li x18, 0x820b5b0040 \n"                 
			"fsw.ps f24, (x18) \n"                 
			"li x24, 0x820b6007e0 \n"                 
			"fsw.ps f16, (x24) \n"                 
			"li x19, 0x820b960660 \n"                 
			"fsw.ps f30, (x19) \n"                 
			"li x15, 0x820bb20260 \n"                 
			"fsw.ps f18, (x15) \n"                 
			"li x19, 0x820bfc0020 \n"                 
			"fsw.ps f15, (x19) \n"                 
			"li x17, 0x820cccc060 \n"                 
			"fsw.ps f29, (x17) \n"                 
			"li x17, 0x820cea0020 \n"                 
			"fsw.ps f18, (x17) \n"                 
			"li x25, 0x820cf298e0 \n"                 
			"fsw.ps f0, (x25) \n"                 
			"li x17, 0x820d361dc0 \n"                 
			"fsw.ps f18, (x17) \n"                 
			"li x22, 0x820d3f9800 \n"                 
			"fsw.ps f17, (x22) \n"                 
			"li x11, 0x820d8c0360 \n"                 
			"fsw.ps f18, (x11) \n"                 
			"li x23, 0x820dbed820 \n"                 
			"fsw.ps f29, (x23) \n"                 
			"li x19, 0x820dc00020 \n"                 
			"fsw.ps f17, (x19) \n"                 
			"li x14, 0x820dda0020 \n"                 
			"fsw.ps f29, (x14) \n"                 
			"li x12, 0x820de00020 \n"                 
			"fsw.ps f24, (x12) \n"                 
			"li x13, 0x820e680000 \n"                 
			"fsw.ps f22, (x13) \n"                 
			"li x13, 0x820e7c51c0 \n"                 
			"fsw.ps f29, (x13) \n"                 
			"li x11, 0x820e978800 \n"                 
			"fsw.ps f4, (x11) \n"                 
			"li x15, 0x820f281800 \n"                 
			"fsw.ps f6, (x15) \n"                 
			"li x18, 0x820f36b800 \n"                 
			"fsw.ps f3, (x18) \n"                 
			"li x19, 0x820f3d2800 \n"                 
			"fsw.ps f21, (x19) \n"                 
			"li x15, 0x820fc20220 \n"                 
			"fsw.ps f8, (x15) \n"                 
			"li x15, 0x820fca1fc0 \n"                 
			"fsw.ps f2, (x15) \n"                 
			"fence\n"
			"LBL_GLOBAL_VADDR_INIT_HID_0:\n"          
			"li x11, 0x81211fb660 \n"                 
			"fswg.ps f21, (x11) \n"                 
			"li x25, 0x812146d540 \n"                 
			"fswg.ps f25, (x25) \n"                 
			"li x14, 0x81214c0020 \n"                 
			"fswg.ps f23, (x14) \n"                 
			"li x22, 0x8121d60320 \n"                 
			"fswg.ps f7, (x22) \n"                 
			"li x12, 0x812221c820 \n"                 
			"fswg.ps f29, (x12) \n"                 
			"li x13, 0x8122dc0000 \n"                 
			"fswg.ps f24, (x13) \n"                 
			"li x22, 0x8123140020 \n"                 
			"fswg.ps f5, (x22) \n"                 
			"li x19, 0x812376e3e0 \n"                 
			"fswg.ps f20, (x19) \n"                 
			"li x24, 0x81238a0000 \n"                 
			"fswg.ps f29, (x24) \n"                 
			"li x19, 0x8123fe17e0 \n"                 
			"fswg.ps f1, (x19) \n"                 
			"li x11, 0x81244a0020 \n"                 
			"fswg.ps f24, (x11) \n"                 
			"li x14, 0x81250a05a0 \n"                 
			"fswg.ps f28, (x14) \n"                 
			"li x20, 0x81254e0040 \n"                 
			"fswg.ps f23, (x20) \n"                 
			"li x16, 0x8126320020 \n"                 
			"fswg.ps f2, (x16) \n"                 
			"li x12, 0x8126360000 \n"                 
			"fswg.ps f8, (x12) \n"                 
			"li x12, 0x8127093020 \n"                 
			"fswg.ps f27, (x12) \n"                 
			"li x13, 0x812724c820 \n"                 
			"fswg.ps f4, (x13) \n"                 
			"li x23, 0x8127c4ea20 \n"                 
			"fswg.ps f10, (x23) \n"                 
			"li x10, 0x8127fb0020 \n"                 
			"fswg.ps f19, (x10) \n"                 
			"li x12, 0x8128ca07c0 \n"                 
			"fswg.ps f22, (x12) \n"                 
			"li x22, 0x8129b20000 \n"                 
			"fswg.ps f20, (x22) \n"                 
			"li x10, 0x812a660020 \n"                 
			"fswg.ps f8, (x10) \n"                 
			"li x12, 0x812ae65800 \n"                 
			"fswg.ps f6, (x12) \n"                 
			"li x12, 0x812b200100 \n"                 
			"fswg.ps f31, (x12) \n"                 
			"li x19, 0x812b4dbae0 \n"                 
			"fswg.ps f17, (x19) \n"                 
			"li x14, 0x812c583660 \n"                 
			"fswg.ps f10, (x14) \n"                 
			"li x16, 0x812d8e03e0 \n"                 
			"fswg.ps f29, (x16) \n"                 
			"li x17, 0x812e3a0000 \n"                 
			"fswg.ps f4, (x17) \n"                 
			"li x22, 0x812e580640 \n"                 
			"fswg.ps f6, (x22) \n"                 
			"li x14, 0x812ef27800 \n"                 
			"fswg.ps f18, (x14) \n"                 
			"li x23, 0x812fd00000 \n"                 
			"fswg.ps f5, (x23) \n"                 
			"li x20, 0x81309607e0 \n"                 
			"fswg.ps f27, (x20) \n"                 
			"li x15, 0x8130b602e0 \n"                 
			"fswg.ps f31, (x15) \n"                 
			"li x24, 0x8130bb7000 \n"                 
			"fswg.ps f17, (x24) \n"                 
			"li x17, 0x813101c080 \n"                 
			"fswg.ps f20, (x17) \n"                 
			"li x24, 0x8131756f20 \n"                 
			"fswg.ps f29, (x24) \n"                 
			"li x11, 0x8131d6a800 \n"                 
			"fswg.ps f31, (x11) \n"                 
			"li x20, 0x8131e99820 \n"                 
			"fswg.ps f31, (x20) \n"                 
			"li x24, 0x8132040000 \n"                 
			"fswg.ps f26, (x24) \n"                 
			"li x14, 0x8132b207e0 \n"                 
			"fswg.ps f10, (x14) \n"                 
			"li x11, 0x8133fc0020 \n"                 
			"fswg.ps f10, (x11) \n"                 
			"li x13, 0x8134772ca0 \n"                 
			"fswg.ps f21, (x13) \n"                 
			"li x25, 0x81348ca000 \n"                 
			"fswg.ps f17, (x25) \n"                 
			"li x13, 0x81349a0300 \n"                 
			"fswg.ps f9, (x13) \n"                 
			"li x16, 0x8134db4000 \n"                 
			"fswg.ps f22, (x16) \n"                 
			"li x19, 0x81350f6a00 \n"                 
			"fswg.ps f18, (x19) \n"                 
			"li x22, 0x8135b63260 \n"                 
			"fswg.ps f5, (x22) \n"                 
			"li x19, 0x8135f90020 \n"                 
			"fswg.ps f21, (x19) \n"                 
			"li x22, 0x81369306e0 \n"                 
			"fswg.ps f11, (x22) \n"                 
			"li x19, 0x813768c820 \n"                 
			"fswg.ps f7, (x19) \n"                 
			"li x17, 0x8137e3f000 \n"                 
			"fswg.ps f27, (x17) \n"                 
			"li x12, 0x81382400a0 \n"                 
			"fswg.ps f14, (x12) \n"                 
			"li x12, 0x81397ac4e0 \n"                 
			"fswg.ps f11, (x12) \n"                 
			"li x11, 0x8139860520 \n"                 
			"fswg.ps f9, (x11) \n"                 
			"li x12, 0x8139895bc0 \n"                 
			"fswg.ps f6, (x12) \n"                 
			"li x13, 0x8139af9140 \n"                 
			"fswg.ps f2, (x13) \n"                 
			"li x14, 0x813acf4820 \n"                 
			"fswg.ps f6, (x14) \n"                 
			"li x23, 0x813b45c820 \n"                 
			"fswg.ps f14, (x23) \n"                 
			"li x19, 0x813bfb6e80 \n"                 
			"fswg.ps f14, (x19) \n"                 
			"li x13, 0x813c4c0820 \n"                 
			"fswg.ps f22, (x13) \n"                 
			"li x19, 0x813cb82820 \n"                 
			"fswg.ps f11, (x19) \n"                 
			"li x23, 0x813cbe0680 \n"                 
			"fswg.ps f23, (x23) \n"                 
			"li x10, 0x8222680020 \n"                 
			"fswg.ps f26, (x10) \n"                 
			"li x25, 0x82231cdfc0 \n"                 
			"fswg.ps f10, (x25) \n"                 
			"li x18, 0x82237404e0 \n"                 
			"fswg.ps f21, (x18) \n"                 
			"li x11, 0x82238a0000 \n"                 
			"fswg.ps f5, (x11) \n"                 
			"li x25, 0x822395c000 \n"                 
			"fswg.ps f23, (x25) \n"                 
			"li x18, 0x8223fe07c0 \n"                 
			"fswg.ps f0, (x18) \n"                 
			"li x24, 0x8224a21a40 \n"                 
			"fswg.ps f30, (x24) \n"                 
			"li x13, 0x82256e7000 \n"                 
			"fswg.ps f22, (x13) \n"                 
			"li x20, 0x8225e1a7c0 \n"                 
			"fswg.ps f0, (x20) \n"                 
			"li x23, 0x8226ec0720 \n"                 
			"fswg.ps f6, (x23) \n"                 
			"li x11, 0x8227060000 \n"                 
			"fswg.ps f17, (x11) \n"                 
			"li x17, 0x8228780000 \n"                 
			"fswg.ps f13, (x17) \n"                 
			"li x16, 0x82290005e0 \n"                 
			"fswg.ps f11, (x16) \n"                 
			"li x10, 0x822928d820 \n"                 
			"fswg.ps f11, (x10) \n"                 
			"li x16, 0x822976f820 \n"                 
			"fswg.ps f14, (x16) \n"                 
			"li x10, 0x8229820020 \n"                 
			"fswg.ps f7, (x10) \n"                 
			"li x10, 0x8229a601c0 \n"                 
			"fswg.ps f29, (x10) \n"                 
			"li x19, 0x822a565020 \n"                 
			"fswg.ps f13, (x19) \n"                 
			"li x21, 0x822a8000a0 \n"                 
			"fswg.ps f13, (x21) \n"                 
			"li x13, 0x822b5b0040 \n"                 
			"fswg.ps f3, (x13) \n"                 
			"li x17, 0x822b960660 \n"                 
			"fswg.ps f6, (x17) \n"                 
			"li x13, 0x822bb20260 \n"                 
			"fswg.ps f23, (x13) \n"                 
			"li x18, 0x822bfc0020 \n"                 
			"fswg.ps f22, (x18) \n"                 
			"li x15, 0x822c1742e0 \n"                 
			"fswg.ps f31, (x15) \n"                 
			"li x17, 0x822c480000 \n"                 
			"fswg.ps f14, (x17) \n"                 
			"li x25, 0x822cefa2a0 \n"                 
			"fswg.ps f11, (x25) \n"                 
			"li x19, 0x822cf298e0 \n"                 
			"fswg.ps f12, (x19) \n"                 
			"li x19, 0x822d361dc0 \n"                 
			"fswg.ps f2, (x19) \n"                 
			"li x15, 0x822d863820 \n"                 
			"fswg.ps f28, (x15) \n"                 
			"li x18, 0x822dbed820 \n"                 
			"fswg.ps f13, (x18) \n"                 
			"li x24, 0x822dda0020 \n"                 
			"fswg.ps f7, (x24) \n"                 
			"li x25, 0x822e63c820 \n"                 
			"fswg.ps f31, (x25) \n"                 
			"li x21, 0x822e978800 \n"                 
			"fswg.ps f19, (x21) \n"                 
			"li x17, 0x822ed20140 \n"                 
			"fswg.ps f26, (x17) \n"                 
			"li x19, 0x822f36b800 \n"                 
			"fswg.ps f13, (x19) \n"                 
			"li x13, 0x822f3d2800 \n"                 
			"fswg.ps f8, (x13) \n"                 
			"li x12, 0x822fc20220 \n"                 
			"fswg.ps f1, (x12) \n"                 
			"li x25, 0x822fca1fc0 \n"                 
			"fswg.ps f1, (x25) \n"                 
			"li x20, 0x8230040020 \n"                 
			"fswg.ps f0, (x20) \n"                 
			"li x15, 0x8230c40020 \n"                 
			"fswg.ps f31, (x15) \n"                 
			"li x25, 0x8231d20020 \n"                 
			"fswg.ps f4, (x25) \n"                 
			"li x24, 0x82320b7820 \n"                 
			"fswg.ps f19, (x24) \n"                 
			"li x23, 0x82335c02a0 \n"                 
			"fswg.ps f27, (x23) \n"                 
			"li x17, 0x82337bd800 \n"                 
			"fswg.ps f29, (x17) \n"                 
			"li x15, 0x82338c0000 \n"                 
			"fswg.ps f17, (x15) \n"                 
			"li x21, 0x8234540240 \n"                 
			"fswg.ps f18, (x21) \n"                 
			"li x24, 0x8234a002a0 \n"                 
			"fswg.ps f4, (x24) \n"                 
			"li x23, 0x8234a80000 \n"                 
			"fswg.ps f18, (x23) \n"                 
			"li x13, 0x8235280000 \n"                 
			"fswg.ps f1, (x13) \n"                 
			"li x12, 0x8235744700 \n"                 
			"fswg.ps f23, (x12) \n"                 
			"li x23, 0x8235928800 \n"                 
			"fswg.ps f18, (x23) \n"                 
			"li x10, 0x82368e0480 \n"                 
			"fswg.ps f15, (x10) \n"                 
			"li x15, 0x8236f00020 \n"                 
			"fswg.ps f22, (x15) \n"                 
			"li x20, 0x82379e0540 \n"                 
			"fswg.ps f11, (x20) \n"                 
			"li x16, 0x82380131c0 \n"                 
			"fswg.ps f24, (x16) \n"                 
			"li x13, 0x8239000020 \n"                 
			"fswg.ps f30, (x13) \n"                 
			"li x10, 0x823934c800 \n"                 
			"fswg.ps f2, (x10) \n"                 
			"li x25, 0x823a4c0020 \n"                 
			"fswg.ps f1, (x25) \n"                 
			"li x22, 0x823a6adc20 \n"                 
			"fswg.ps f21, (x22) \n"                 
			"li x17, 0x823acc0420 \n"                 
			"fswg.ps f22, (x17) \n"                 
			"li x17, 0x823ad003c0 \n"                 
			"fswg.ps f25, (x17) \n"                 
			"li x18, 0x823b6007e0 \n"                 
			"fswg.ps f17, (x18) \n"                 
			"li x24, 0x823b650820 \n"                 
			"fswg.ps f6, (x24) \n"                 
			"li x11, 0x823bf60000 \n"                 
			"fswg.ps f20, (x11) \n"                 
			"li x19, 0x823c0c0700 \n"                 
			"fswg.ps f28, (x19) \n"                 
			"li x19, 0x823c200040 \n"                 
			"fswg.ps f14, (x19) \n"                 
			"li x16, 0x823cccc060 \n"                 
			"fswg.ps f3, (x16) \n"                 
			"li x19, 0x823cea0020 \n"                 
			"fswg.ps f2, (x19) \n"                 
			"li x22, 0x823d3e0000 \n"                 
			"fswg.ps f19, (x22) \n"                 
			"li x17, 0x823d7e0000 \n"                 
			"fswg.ps f16, (x17) \n"                 
			"li x16, 0x823d8be320 \n"                 
			"fswg.ps f9, (x16) \n"                 
			"li x14, 0x823e680000 \n"                 
			"fswg.ps f26, (x14) \n"                 
			"fence\n"
			"LBL_LOCAL_VADDR_INIT_HID_0:\n"          
			"li x15, 0x81406fd300 \n"                 
			"fswl.ps f18, (x15) \n"                 
			"li x15, 0x81409607e0 \n"                 
			"fswl.ps f14, (x15) \n"                 
			"li x10, 0x8140bb7000 \n"                 
			"fswl.ps f0, (x10) \n"                 
			"li x21, 0x8140e88800 \n"                 
			"fswl.ps f23, (x21) \n"                 
			"li x17, 0x814101c080 \n"                 
			"fswl.ps f18, (x17) \n"                 
			"li x12, 0x81411fb660 \n"                 
			"fswl.ps f26, (x12) \n"                 
			"li x16, 0x8142b70dc0 \n"                 
			"fswl.ps f23, (x16) \n"                 
			"li x24, 0x8143140020 \n"                 
			"fswl.ps f28, (x24) \n"                 
			"li x25, 0x81438a0000 \n"                 
			"fswl.ps f19, (x25) \n"                 
			"li x10, 0x8143fe17e0 \n"                 
			"fswl.ps f8, (x10) \n"                 
			"li x22, 0x81444a0020 \n"                 
			"fswl.ps f13, (x22) \n"                 
			"li x10, 0x81448ca000 \n"                 
			"fswl.ps f15, (x10) \n"                 
			"li x14, 0x81449a0300 \n"                 
			"fswl.ps f31, (x14) \n"                 
			"li x20, 0x81450a05a0 \n"                 
			"fswl.ps f21, (x20) \n"                 
			"li x16, 0x81454e0040 \n"                 
			"fswl.ps f22, (x16) \n"                 
			"li x11, 0x8145f90020 \n"                 
			"fswl.ps f26, (x11) \n"                 
			"li x22, 0x814724c820 \n"                 
			"fswl.ps f14, (x22) \n"                 
			"li x11, 0x8147e3f000 \n"                 
			"fswl.ps f31, (x11) \n"                 
			"li x10, 0x81485c05c0 \n"                 
			"fswl.ps f7, (x10) \n"                 
			"li x23, 0x8149860520 \n"                 
			"fswl.ps f17, (x23) \n"                 
			"li x20, 0x814a660020 \n"                 
			"fswl.ps f26, (x20) \n"                 
			"li x12, 0x814ae65800 \n"                 
			"fswl.ps f31, (x12) \n"                 
			"li x14, 0x814b200100 \n"                 
			"fswl.ps f25, (x14) \n"                 
			"li x19, 0x814b45c820 \n"                 
			"fswl.ps f26, (x19) \n"                 
			"li x14, 0x814b4dbae0 \n"                 
			"fswl.ps f2, (x14) \n"                 
			"li x22, 0x814b53d9e0 \n"                 
			"fswl.ps f10, (x22) \n"                 
			"li x23, 0x814b5a0160 \n"                 
			"fswl.ps f30, (x23) \n"                 
			"li x22, 0x814d8e03e0 \n"                 
			"fswl.ps f12, (x22) \n"                 
			"li x25, 0x814e3c0020 \n"                 
			"fswl.ps f25, (x25) \n"                 
			"li x11, 0x814ead0520 \n"                 
			"fswl.ps f23, (x11) \n"                 
			"li x20, 0x814fd00000 \n"                 
			"fswl.ps f7, (x20) \n"                 
			"li x23, 0x8150b602e0 \n"                 
			"fswl.ps f8, (x23) \n"                 
			"li x23, 0x81514c0020 \n"                 
			"fswl.ps f18, (x23) \n"                 
			"li x24, 0x8151756f20 \n"                 
			"fswl.ps f15, (x24) \n"                 
			"li x24, 0x8151ae0160 \n"                 
			"fswl.ps f2, (x24) \n"                 
			"li x11, 0x8151d60320 \n"                 
			"fswl.ps f16, (x11) \n"                 
			"li x12, 0x8151d6a800 \n"                 
			"fswl.ps f19, (x12) \n"                 
			"li x21, 0x8151e99820 \n"                 
			"fswl.ps f15, (x21) \n"                 
			"li x14, 0x8152040000 \n"                 
			"fswl.ps f0, (x14) \n"                 
			"li x20, 0x815221c820 \n"                 
			"fswl.ps f3, (x20) \n"                 
			"li x20, 0x8152b207e0 \n"                 
			"fswl.ps f29, (x20) \n"                 
			"li x17, 0x8152dc0000 \n"                 
			"fswl.ps f11, (x17) \n"                 
			"li x12, 0x815376e3e0 \n"                 
			"fswl.ps f3, (x12) \n"                 
			"li x17, 0x8153f98840 \n"                 
			"fswl.ps f11, (x17) \n"                 
			"li x10, 0x8153fc0020 \n"                 
			"fswl.ps f0, (x10) \n"                 
			"li x24, 0x8154820740 \n"                 
			"fswl.ps f24, (x24) \n"                 
			"li x23, 0x8154f3f820 \n"                 
			"fswl.ps f18, (x23) \n"                 
			"li x25, 0x8155b63260 \n"                 
			"fswl.ps f4, (x25) \n"                 
			"li x17, 0x8156320020 \n"                 
			"fswl.ps f21, (x17) \n"                 
			"li x20, 0x8156360000 \n"                 
			"fswl.ps f1, (x20) \n"                 
			"li x22, 0x81568602c0 \n"                 
			"fswl.ps f29, (x22) \n"                 
			"li x14, 0x8156887920 \n"                 
			"fswl.ps f5, (x14) \n"                 
			"li x21, 0x8157093020 \n"                 
			"fswl.ps f23, (x21) \n"                 
			"li x18, 0x8157392080 \n"                 
			"fswl.ps f13, (x18) \n"                 
			"li x20, 0x815768c820 \n"                 
			"fswl.ps f7, (x20) \n"                 
			"li x25, 0x8158ca07c0 \n"                 
			"fswl.ps f0, (x25) \n"                 
			"li x24, 0x8159a00000 \n"                 
			"fswl.ps f19, (x24) \n"                 
			"li x24, 0x8159af9140 \n"                 
			"fswl.ps f26, (x24) \n"                 
			"li x15, 0x8159ba0060 \n"                 
			"fswl.ps f21, (x15) \n"                 
			"li x23, 0x8159e9f160 \n"                 
			"fswl.ps f16, (x23) \n"                 
			"li x19, 0x815b0600c0 \n"                 
			"fswl.ps f19, (x19) \n"                 
			"li x10, 0x815bbc0000 \n"                 
			"fswl.ps f1, (x10) \n"                 
			"li x15, 0x815c583660 \n"                 
			"fswl.ps f9, (x15) \n"                 
			"li x10, 0x815cbe0680 \n"                 
			"fswl.ps f1, (x10) \n"                 
			"li x18, 0x815cf80020 \n"                 
			"fswl.ps f8, (x18) \n"                 
			"li x15, 0x815d878f60 \n"                 
			"fswl.ps f16, (x15) \n"                 
			"li x12, 0x815e580640 \n"                 
			"fswl.ps f5, (x12) \n"                 
			"li x24, 0x8240040020 \n"                 
			"fswl.ps f25, (x24) \n"                 
			"li x21, 0x82420b7820 \n"                 
			"fswl.ps f19, (x21) \n"                 
			"li x19, 0x82434e0000 \n"                 
			"fswl.ps f19, (x19) \n"                 
			"li x21, 0x82438c0000 \n"                 
			"fswl.ps f25, (x21) \n"                 
			"li x17, 0x824395c000 \n"                 
			"fswl.ps f30, (x17) \n"                 
			"li x16, 0x8244a002a0 \n"                 
			"fswl.ps f2, (x16) \n"                 
			"li x20, 0x8244f4b000 \n"                 
			"fswl.ps f27, (x20) \n"                 
			"li x14, 0x82450a0000 \n"                 
			"fswl.ps f22, (x14) \n"                 
			"li x12, 0x82456e7000 \n"                 
			"fswl.ps f15, (x12) \n"                 
			"li x25, 0x8245744700 \n"                 
			"fswl.ps f27, (x25) \n"                 
			"li x13, 0x8246ec0720 \n"                 
			"fswl.ps f4, (x13) \n"                 
			"li x14, 0x8247060000 \n"                 
			"fswl.ps f28, (x14) \n"                 
			"li x12, 0x8247f37800 \n"                 
			"fswl.ps f1, (x12) \n"                 
			"li x14, 0x82480131c0 \n"                 
			"fswl.ps f22, (x14) \n"                 
			"li x11, 0x824934c800 \n"                 
			"fswl.ps f8, (x11) \n"                 
			"li x20, 0x824976f820 \n"                 
			"fswl.ps f20, (x20) \n"                 
			"li x20, 0x8249977580 \n"                 
			"fswl.ps f13, (x20) \n"                 
			"li x12, 0x8249a601c0 \n"                 
			"fswl.ps f30, (x12) \n"                 
			"li x23, 0x824a4c0020 \n"                 
			"fswl.ps f16, (x23) \n"                 
			"li x10, 0x824a565020 \n"                 
			"fswl.ps f5, (x10) \n"                 
			"li x23, 0x824a6adc20 \n"                 
			"fswl.ps f19, (x23) \n"                 
			"li x11, 0x824a8000a0 \n"                 
			"fswl.ps f25, (x11) \n"                 
			"li x18, 0x824ac267c0 \n"                 
			"fswl.ps f28, (x18) \n"                 
			"li x24, 0x824c0c0700 \n"                 
			"fswl.ps f10, (x24) \n"                 
			"li x18, 0x824c200040 \n"                 
			"fswl.ps f29, (x18) \n"                 
			"li x11, 0x824cea0020 \n"                 
			"fswl.ps f21, (x11) \n"                 
			"li x10, 0x824cefa2a0 \n"                 
			"fswl.ps f10, (x10) \n"                 
			"li x11, 0x824d3f9800 \n"                 
			"fswl.ps f16, (x11) \n"                 
			"li x25, 0x824d863820 \n"                 
			"fswl.ps f14, (x25) \n"                 
			"li x20, 0x824de00020 \n"                 
			"fswl.ps f11, (x20) \n"                 
			"li x24, 0x824e7c51c0 \n"                 
			"fswl.ps f12, (x24) \n"                 
			"li x21, 0x824e978800 \n"                 
			"fswl.ps f10, (x21) \n"                 
			"li x15, 0x824f3d2800 \n"                 
			"fswl.ps f5, (x15) \n"                 
			"li x17, 0x824f9e0220 \n"                 
			"fswl.ps f9, (x17) \n"                 
			"li x14, 0x82506601c0 \n"                 
			"fswl.ps f5, (x14) \n"                 
			"li x15, 0x8251d20020 \n"                 
			"fswl.ps f16, (x15) \n"                 
			"li x13, 0x8252680020 \n"                 
			"fswl.ps f30, (x13) \n"                 
			"li x16, 0x82530406e0 \n"                 
			"fswl.ps f0, (x16) \n"                 
			"li x11, 0x82531cdfc0 \n"                 
			"fswl.ps f25, (x11) \n"                 
			"li x19, 0x825322e800 \n"                 
			"fswl.ps f12, (x19) \n"                 
			"li x22, 0x82537404e0 \n"                 
			"fswl.ps f7, (x22) \n"                 
			"li x15, 0x82538d2760 \n"                 
			"fswl.ps f5, (x15) \n"                 
			"li x25, 0x8253fe07c0 \n"                 
			"fswl.ps f28, (x25) \n"                 
			"li x20, 0x825430aca0 \n"                 
			"fswl.ps f11, (x20) \n"                 
			"li x16, 0x8254a21a40 \n"                 
			"fswl.ps f15, (x16) \n"                 
			"li x13, 0x8255280000 \n"                 
			"fswl.ps f31, (x13) \n"                 
			"li x22, 0x82568e0480 \n"                 
			"fswl.ps f3, (x22) \n"                 
			"li x23, 0x8256f00020 \n"                 
			"fswl.ps f18, (x23) \n"                 
			"li x23, 0x8256f32820 \n"                 
			"fswl.ps f13, (x23) \n"                 
			"li x10, 0x82574f8800 \n"                 
			"fswl.ps f4, (x10) \n"                 
			"li x15, 0x8257f20020 \n"                 
			"fswl.ps f28, (x15) \n"                 
			"li x19, 0x8258780000 \n"                 
			"fswl.ps f22, (x19) \n"                 
			"li x13, 0x82590005e0 \n"                 
			"fswl.ps f14, (x13) \n"                 
			"li x20, 0x82593ae160 \n"                 
			"fswl.ps f4, (x20) \n"                 
			"li x21, 0x825ad003c0 \n"                 
			"fswl.ps f27, (x21) \n"                 
			"li x13, 0x825b6007e0 \n"                 
			"fswl.ps f11, (x13) \n"                 
			"li x25, 0x825b650820 \n"                 
			"fswl.ps f5, (x25) \n"                 
			"li x17, 0x825ba60000 \n"                 
			"fswl.ps f30, (x17) \n"                 
			"li x11, 0x825bf60000 \n"                 
			"fswl.ps f10, (x11) \n"                 
			"li x13, 0x825bfc0020 \n"                 
			"fswl.ps f0, (x13) \n"                 
			"li x14, 0x825c480000 \n"                 
			"fswl.ps f20, (x14) \n"                 
			"li x22, 0x825c50f280 \n"                 
			"fswl.ps f7, (x22) \n"                 
			"li x23, 0x825cccc060 \n"                 
			"fswl.ps f19, (x23) \n"                 
			"li x25, 0x825d361dc0 \n"                 
			"fswl.ps f13, (x25) \n"                 
			"li x11, 0x825d3e0000 \n"                 
			"fswl.ps f19, (x11) \n"                 
			"li x22, 0x825d8c0360 \n"                 
			"fswl.ps f11, (x22) \n"                 
			"li x18, 0x825fca1fc0 \n"                 
			"fswl.ps f14, (x18) \n"                 
			"fence\n"
			"LBL_LOCAL_L2SCP_INIT_HID_0:\n"          
			"la x11, rand_ieee754_32\n"    
			"li x31, 0x40\n"                 
			"li x5, 0x200000000000f \n"                 
			"add x5, x11, x5\n"               
			"csrw tensor_load_l2, x5\n"         
			"addi x31, x31, 1\n"                 
			"li x5, 0x600000000000f \n"                 
			"add x5, x11, x5\n"               
			"csrw tensor_load_l2, x5\n"         
			"addi x31, x31, -1\n"                 
			"li x5, 0xa00000000000f \n"                 
			"add x5, x11, x5\n"               
			"csrw tensor_load_l2, x5\n"         
			"addi x31, x31, 1\n"                 
			"li x5, 0xe00000000000f \n"                 
			"add x5, x11, x5\n"               
			"csrw tensor_load_l2, x5\n"         
			"addi x31, x31, -1\n"                 
			"li x5, 0x1200000000002f \n"                 
			"add x5, x11, x5\n"               
			"csrw tensor_load_l2, x5\n"         
			"addi x31, x31, 1\n"                 
			"li x5, 0x1400000000001f \n"                 
			"add x5, x11, x5\n"               
			"csrw tensor_load_l2, x5\n"         
			"addi x31, x31, -1\n"                 
			"li x5, 0x1600000000002f \n"                 
			"add x5, x11, x5\n"               
			"csrw tensor_load_l2, x5\n"         
			"addi x31, x31, 1\n"                 
			"li x5, 0x1800000000001f \n"                 
			"add x5, x11, x5\n"               
			"csrw tensor_load_l2, x5\n"         
			"addi x31, x31, -1\n"                 
			"li x5, 0x1a00000000002f \n"                 
			"add x5, x11, x5\n"               
			"csrw tensor_load_l2, x5\n"         
			"addi x31, x31, 1\n"                 
			"li x5, 0x1c00000000001f \n"                 
			"add x5, x11, x5\n"               
			"csrw tensor_load_l2, x5\n"         
			"addi x31, x31, -1\n"                 
			"li x5, 0x1e00000000002f \n"                 
			"add x5, x11, x5\n"               
			"csrw tensor_load_l2, x5\n"         
			"addi x31, x31, 1\n"                 
			"li x5, 0x2000000000001f \n"                 
			"add x5, x11, x5\n"               
			"csrw tensor_load_l2, x5\n"         
			"addi x31, x31, -1\n"                 
			"li x5, 0x2100000000000f \n"                 
			"add x5, x11, x5\n"               
			"csrw tensor_load_l2, x5\n"         
			"addi x31, x31, 1\n"                 
			"li x5, 0x2400000000003f \n"                 
			"add x5, x11, x5\n"               
			"csrw tensor_load_l2, x5\n"         
			"addi x31, x31, -1\n"                 
			"li x5, 0x2500000000000f \n"                 
			"add x5, x11, x5\n"               
			"csrw tensor_load_l2, x5\n"         
			"addi x31, x31, 1\n"                 
			"li x5, 0x2800000000003f \n"                 
			"add x5, x11, x5\n"               
			"csrw tensor_load_l2, x5\n"         
			"addi x31, x31, -1\n"                 
			"li x5, 0x2900000000000f \n"                 
			"add x5, x11, x5\n"               
			"csrw tensor_load_l2, x5\n"         
			"addi x31, x31, 1\n"                 
			"li x5, 0x2c00000000003f \n"                 
			"add x5, x11, x5\n"               
			"csrw tensor_load_l2, x5\n"         
			"addi x31, x31, -1\n"                 
			"li x5, 0x2d00000000000f \n"                 
			"add x5, x11, x5\n"               
			"csrw tensor_load_l2, x5\n"         
			"addi x31, x31, 1\n"                 
			"li x5, 0x2f00000000003f \n"                 
			"add x5, x11, x5\n"               
			"csrw tensor_load_l2, x5\n"         
			"addi x31, x31, -1\n"                 
			"li x5, 0x3000000000003f \n"                 
			"add x5, x11, x5\n"               
			"csrw tensor_load_l2, x5\n"         
			"addi x31, x31, 1\n"                 
			"li x5, 0x3300000000003f \n"                 
			"add x5, x11, x5\n"               
			"csrw tensor_load_l2, x5\n"         
			"addi x31, x31, -1\n"                 
			"li x5, 0x3700000000003f \n"                 
			"add x5, x11, x5\n"               
			"csrw tensor_load_l2, x5\n"         
			"addi x31, x31, 1\n"                 
			"li x5, 0x3a00000000002f \n"                 
			"add x5, x11, x5\n"               
			"csrw tensor_load_l2, x5\n"         
			"addi x31, x31, -1\n"                 
			"li x5, 0x3a00000000003f \n"                 
			"add x5, x11, x5\n"               
			"csrw tensor_load_l2, x5\n"         
			"addi x31, x31, 1\n"                 
			"li x5, 0x3b00000000001f \n"                 
			"add x5, x11, x5\n"               
			"csrw tensor_load_l2, x5\n"         
			"addi x31, x31, -1\n"                 
			"li x5, 0x3b00000000003f \n"                 
			"add x5, x11, x5\n"               
			"csrw tensor_load_l2, x5\n"         
			"addi x31, x31, 1\n"                 
			"li x5, 0x3e00000000002f \n"                 
			"add x5, x11, x5\n"               
			"csrw tensor_load_l2, x5\n"         
			"addi x31, x31, -1\n"                 
			"li x5, 0x3e00000000003f \n"                 
			"add x5, x11, x5\n"               
			"csrw tensor_load_l2, x5\n"         
			"addi x31, x31, 1\n"                 
			"li x5, 0x3f00000000001f \n"                 
			"add x5, x11, x5\n"               
			"csrw tensor_load_l2, x5\n"         
			"addi x31, x31, -1\n"                 
			"li x5, 0x4200000000002f \n"                 
			"add x5, x11, x5\n"               
			"csrw tensor_load_l2, x5\n"         
			"addi x31, x31, 1\n"                 
			"li x5, 0x4200000000003f \n"                 
			"add x5, x11, x5\n"               
			"csrw tensor_load_l2, x5\n"         
			"addi x31, x31, -1\n"                 
			"li x5, 0x4300000000001f \n"                 
			"add x5, x11, x5\n"               
			"csrw tensor_load_l2, x5\n"         
			"addi x31, x31, 1\n"                 
			"li x5, 0x4600000000002f \n"                 
			"add x5, x11, x5\n"               
			"csrw tensor_load_l2, x5\n"         
			"addi x31, x31, -1\n"                 
			"li x5, 0x4600000000003f \n"                 
			"add x5, x11, x5\n"               
			"csrw tensor_load_l2, x5\n"         
			"addi x31, x31, 1\n"                 
			"li x5, 0x4700000000001f \n"                 
			"add x5, x11, x5\n"               
			"csrw tensor_load_l2, x5\n"         
			"addi x31, x31, -1\n"                 
			"li x5, 0xa00000000003f \n"                 
			"add x5, x11, x5\n"               
			"csrw tensor_load_l2, x5\n"         
			"addi x31, x31, 1\n"                 
			"li x5, 0xe00000000003f \n"                 
			"add x5, x11, x5\n"               
			"csrw tensor_load_l2, x5\n"         
			"addi x31, x31, -1\n"                 
			"li x5, 0x1200000000003f \n"                 
			"add x5, x11, x5\n"               
			"csrw tensor_load_l2, x5\n"         
			"addi x31, x31, 1\n"                 
			"li x5, 0x1600000000003f \n"                 
			"add x5, x11, x5\n"               
			"csrw tensor_load_l2, x5\n"         
			"addi x31, x31, -1\n"                 
			"li x5, 0x2500000000002f \n"                 
			"add x5, x11, x5\n"               
			"csrw tensor_load_l2, x5\n"         
			"addi x31, x31, 1\n"                 
			"li x5, 0x2900000000002f \n"                 
			"add x5, x11, x5\n"               
			"csrw tensor_load_l2, x5\n"         
			"addi x31, x31, -1\n"                 
			"li x5, 0x2a00000000002f \n"                 
			"add x5, x11, x5\n"               
			"csrw tensor_load_l2, x5\n"         
			"addi x31, x31, 1\n"                 
			"li x5, 0x2d00000000002f \n"                 
			"add x5, x11, x5\n"               
			"csrw tensor_load_l2, x5\n"         
			"addi x31, x31, -1\n"                 
			"li x5, 0x2e00000000002f \n"                 
			"add x5, x11, x5\n"               
			"csrw tensor_load_l2, x5\n"         
			"addi x31, x31, 1\n"                 
			"li x5, 0x3100000000000f \n"                 
			"add x5, x11, x5\n"               
			"csrw tensor_load_l2, x5\n"         
			"addi x31, x31, -1\n"                 
			"li x5, 0x3100000000002f \n"                 
			"add x5, x11, x5\n"               
			"csrw tensor_load_l2, x5\n"         
			"addi x31, x31, 1\n"                 
			"li x5, 0x3200000000002f \n"                 
			"add x5, x11, x5\n"               
			"csrw tensor_load_l2, x5\n"         
			"addi x31, x31, -1\n"                 
			"li x5, 0x3500000000000f \n"                 
			"add x5, x11, x5\n"               
			"csrw tensor_load_l2, x5\n"         
			"addi x31, x31, 1\n"                 
			"li x5, 0x3600000000000f \n"                 
			"add x5, x11, x5\n"               
			"csrw tensor_load_l2, x5\n"         
			"addi x31, x31, -1\n"                 
			"li x5, 0x3600000000002f \n"                 
			"add x5, x11, x5\n"               
			"csrw tensor_load_l2, x5\n"         
			"addi x31, x31, 1\n"                 
			"li x5, 0x3900000000000f \n"                 
			"add x5, x11, x5\n"               
			"csrw tensor_load_l2, x5\n"         
			"addi x31, x31, -1\n"                 
			"li x5, 0x3a00000000000f \n"                 
			"add x5, x11, x5\n"               
			"csrw tensor_load_l2, x5\n"         
			"addi x31, x31, 1\n"                 
			"li x5, 0x3d00000000000f \n"                 
			"add x5, x11, x5\n"               
			"csrw tensor_load_l2, x5\n"         
			"addi x31, x31, -1\n"                 
			"li x5, 0x3e00000000000f \n"                 
			"add x5, x11, x5\n"               
			"csrw tensor_load_l2, x5\n"         
			"addi x31, x31, 1\n"                 
			"li x5, 0x4200000000000f \n"                 
			"add x5, x11, x5\n"               
			"csrw tensor_load_l2, x5\n"         
			"addi x31, x31, -1\n"                 
			"li x5, 0x5100000000003f \n"                 
			"add x5, x11, x5\n"               
			"csrw tensor_load_l2, x5\n"         
			"addi x31, x31, 1\n"                 
			"li x5, 0x5500000000003f \n"                 
			"add x5, x11, x5\n"               
			"csrw tensor_load_l2, x5\n"         
			"addi x31, x31, -1\n"                 
			"li x5, 0x5900000000003f \n"                 
			"add x5, x11, x5\n"               
			"csrw tensor_load_l2, x5\n"         
			"addi x31, x31, 1\n"                 
			"li x5, 0x5d00000000003f \n"                 
			"add x5, x11, x5\n"               
			"csrw tensor_load_l2, x5\n"         
			"addi x31, x31, -1\n"                 
			"li x5, 0x6500000000001f \n"                 
			"add x5, x11, x5\n"               
			"csrw tensor_load_l2, x5\n"         
			"addi x31, x31, 1\n"                 
			"li x5, 0x6700000000001f \n"                 
			"add x5, x11, x5\n"               
			"csrw tensor_load_l2, x5\n"         
			"addi x31, x31, -1\n"                 
			"li x5, 0x6900000000001f \n"                 
			"add x5, x11, x5\n"               
			"csrw tensor_load_l2, x5\n"         
			"addi x31, x31, 1\n"                 
			"li x5, 0x6b00000000001f \n"                 
			"add x5, x11, x5\n"               
			"csrw tensor_load_l2, x5\n"         
			"addi x31, x31, -1\n"                 
			"li x5, 0x6d00000000001f \n"                 
			"add x5, x11, x5\n"               
			"csrw tensor_load_l2, x5\n"         
			"addi x31, x31, 1\n"                 
			"li x5, 0x6f00000000001f \n"                 
			"add x5, x11, x5\n"               
			"csrw tensor_load_l2, x5\n"         
			"addi x31, x31, -1\n"                 
			"li x5, 0x7100000000001f \n"                 
			"add x5, x11, x5\n"               
			"csrw tensor_load_l2, x5\n"         
			"addi x31, x31, 1\n"                 
			"li x5, 0x7300000000001f \n"                 
			"add x5, x11, x5\n"               
			"csrw tensor_load_l2, x5\n"         
			"addi x31, x31, -1\n"                 
			"li x5, 0x7b00000000000f \n"                 
			"add x5, x11, x5\n"               
			"csrw tensor_load_l2, x5\n"         
			"addi x31, x31, 1\n"                 
			"li x5, 0x7f00000000000f \n"                 
			"add x5, x11, x5\n"               
			"csrw tensor_load_l2, x5\n"         
			"addi x31, x31, -1\n"                 
			"li x5, 0x8300000000000f \n"                 
			"add x5, x11, x5\n"               
			"csrw tensor_load_l2, x5\n"         
			"addi x31, x31, 1\n"                 
			"li x5, 0x8700000000000f \n"                 
			"add x5, x11, x5\n"               
			"csrw tensor_load_l2, x5\n"         
			"addi x31, x31, -1\n"                 
			"csrwi tensor_wait, 0x2\n"      
			"csrwi tensor_wait, 0x3\n"      
			"fence\n"
			"LBL_TXR_INIT_HID_0:\n"          
			"li x16, 0x816b45c800 \n"                 
			"fsw.ps f8, 0(x16) \n"                 
			"fsw.ps f22, 64(x16) \n"                 
			"fsw.ps f28, 128(x16) \n"                 
			"fsw.ps f21, 192(x16) \n"                 
			"fsw.ps f23, 256(x16) \n"                 
			"li x12, 0x816d878f40 \n"                 
			"fsw.ps f28, 0(x12) \n"                 
			"fsw.ps f27, 64(x12) \n"                 
			"fsw.ps f5, 128(x12) \n"                 
			"fsw.ps f11, 192(x12) \n"                 
			"fsw.ps f27, 256(x12) \n"                 
			"li x12, 0x816ef27800 \n"                 
			"fsw.ps f3, 0(x12) \n"                 
			"fsw.ps f30, 64(x12) \n"                 
			"fsw.ps f13, 128(x12) \n"                 
			"fsw.ps f0, 192(x12) \n"                 
			"fsw.ps f14, 256(x12) \n"                 
			"li x10, 0x8170e88800 \n"                 
			"fsw.ps f24, 0(x10) \n"                 
			"fsw.ps f22, 64(x10) \n"                 
			"fsw.ps f9, 128(x10) \n"                 
			"fsw.ps f1, 192(x10) \n"                 
			"fsw.ps f7, 256(x10) \n"                 
			"fsw.ps f7, 320(x10) \n"                 
			"fsw.ps f15, 384(x10) \n"                 
			"fsw.ps f30, 448(x10) \n"                 
			"fsw.ps f26, 512(x10) \n"                 
			"fsw.ps f27, 576(x10) \n"                 
			"fsw.ps f22, 640(x10) \n"                 
			"fsw.ps f0, 704(x10) \n"                 
			"li x23, 0x81711fb640 \n"                 
			"fsw.ps f9, 0(x23) \n"                 
			"fsw.ps f27, 64(x23) \n"                 
			"fsw.ps f5, 128(x23) \n"                 
			"fsw.ps f13, 192(x23) \n"                 
			"fsw.ps f11, 256(x23) \n"                 
			"fsw.ps f19, 320(x23) \n"                 
			"fsw.ps f18, 384(x23) \n"                 
			"li x17, 0x8171e99800 \n"                 
			"fsw.ps f2, 0(x17) \n"                 
			"fsw.ps f4, 64(x17) \n"                 
			"fsw.ps f6, 128(x17) \n"                 
			"fsw.ps f21, 192(x17) \n"                 
			"fsw.ps f29, 256(x17) \n"                 
			"fsw.ps f22, 320(x17) \n"                 
			"li x22, 0x8174772c80 \n"                 
			"fsw.ps f4, 0(x22) \n"                 
			"fsw.ps f15, 64(x22) \n"                 
			"fsw.ps f15, 128(x22) \n"                 
			"fsw.ps f24, 192(x22) \n"                 
			"fsw.ps f14, 256(x22) \n"                 
			"fsw.ps f3, 320(x22) \n"                 
			"fsw.ps f16, 384(x22) \n"                 
			"fsw.ps f15, 448(x22) \n"                 
			"fsw.ps f27, 512(x22) \n"                 
			"fsw.ps f22, 576(x22) \n"                 
			"fsw.ps f29, 640(x22) \n"                 
			"fsw.ps f27, 704(x22) \n"                 
			"fsw.ps f0, 768(x22) \n"                 
			"li x15, 0x82637bd800 \n"                 
			"fsw.ps f29, 0(x15) \n"                 
			"fsw.ps f28, 64(x15) \n"                 
			"fsw.ps f0, 128(x15) \n"                 
			"fsw.ps f11, 192(x15) \n"                 
			"fsw.ps f23, 256(x15) \n"                 
			"fsw.ps f27, 320(x15) \n"                 
			"li x17, 0x8267f37800 \n"                 
			"fsw.ps f11, 0(x17) \n"                 
			"fsw.ps f4, 64(x17) \n"                 
			"fsw.ps f24, 128(x17) \n"                 
			"fsw.ps f18, 192(x17) \n"                 
			"fsw.ps f5, 256(x17) \n"                 
			"fsw.ps f10, 320(x17) \n"                 
			"fsw.ps f30, 384(x17) \n"                 
			"fsw.ps f14, 448(x17) \n"                 
			"fsw.ps f24, 512(x17) \n"                 
			"fsw.ps f21, 576(x17) \n"                 
			"fsw.ps f10, 640(x17) \n"                 
			"fsw.ps f17, 704(x17) \n"                 
			"fsw.ps f8, 768(x17) \n"                 
			"li x18, 0x82680131c0 \n"                 
			"fsw.ps f30, 0(x18) \n"                 
			"fsw.ps f16, 64(x18) \n"                 
			"fsw.ps f16, 128(x18) \n"                 
			"fsw.ps f9, 192(x18) \n"                 
			"fsw.ps f28, 256(x18) \n"                 
			"fsw.ps f12, 320(x18) \n"                 
			"fsw.ps f28, 384(x18) \n"                 
			"fsw.ps f18, 448(x18) \n"                 
			"fsw.ps f28, 512(x18) \n"                 
			"fsw.ps f30, 576(x18) \n"                 
			"li x25, 0x826f36b800 \n"                 
			"fsw.ps f19, 0(x25) \n"                 
			"fsw.ps f29, 64(x25) \n"                 
			"fsw.ps f12, 128(x25) \n"                 
			"fsw.ps f12, 192(x25) \n"                 
			"fsw.ps f14, 256(x25) \n"                 
			"fsw.ps f1, 320(x25) \n"                 
			"fsw.ps f7, 384(x25) \n"                 
			"fsw.ps f21, 448(x25) \n"                 
			"li x25, 0x827de00000 \n"                 
			"fsw.ps f3, 0(x25) \n"                 
			"fsw.ps f30, 64(x25) \n"                 
			"fsw.ps f18, 128(x25) \n"                 
			"fsw.ps f13, 192(x25) \n"                 
			"fsw.ps f19, 256(x25) \n"                 
			"fsw.ps f25, 320(x25) \n"                 
			"fsw.ps f31, 384(x25) \n"                 
			"fsw.ps f16, 448(x25) \n"                 
			"fsw.ps f14, 512(x25) \n"                 
			"fsw.ps f9, 576(x25) \n"                 
			"fsw.ps f9, 640(x25) \n"                 
			"fsw.ps f20, 704(x25) \n"                 
			"fsw.ps f27, 768(x25) \n"                 
			"fence\n"
			"fence\n"
			"csrw fflags, zero\n"
			"csrwi frm, 0x0\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_mask, zero\n"
			"LBL_KERNEL_HID_0:\n"          
			"li x5, 0xFEEDFEED\n"
			"li x0, 0x56339bd7e6\n" 
			"slti x0, x0, 0x7f2\n"
			"la x31, check_sum_hid_0\n"
			"li x5, 0x6\n" 
			"sw x5, (x31)\n"
			"li x29, 0x5\n"
			"LBL_OUTER_LOOP_SEQ_HID_0:\n"
			"nop \n"
			"nop \n"
			"li x30, 0\n"
			"addi x30, x30, 1\n"
			"la x10, rand_int_32\n"
			"flw.ps f27, 448(x10)\n"
			"fsrai.pi f26, f27, 0xd\n" 
			"fcvt.ps.pw f12, f12, rmm\n"
			"fcmovm.ps f8, f9, f9\n"
			"slt x0, x15, x26\n"
			"sraiw x7, x6, 0\n"
			"bgeu x6, x10, 1f\n"
			"1:\n"
			"subw x9, x7, x15\n"
			"fltm.pi m6, f29, f9\n"
			"sraiw x5, x26, 5\n"
			"bltu x3, x18, 1f\n"
			"1:\n"
			"auipc x0, 0x14\n"
			"bltu x6, x28, 1f\n"
			"1:\n"
			"la x19, rand_ieee754_32\n"
			"flw.ps f4, 256(x19)\n"
			"fround.ps f11, f4, rmm\n" 
			"fmv.x.s x7, f8\n"
			"andi x11, x9, 0xe\n"
			"mova.x.m x28\n"
			"bltu x9, x18, 1f\n"
			"1:\n"
			"fcmovm.ps f4, f21, f2\n"
			"fmvs.x.ps x27, f29, 3\n"
			"fcvt.s.w f17, x13, rne\n"
			"masknot m4, m5\n"
			"fbci.pi f5, 0x3fa60\n"
			"la x6, rand_int_32\n"
			"flw.ps f9, 992(x6)\n"
			"fslli.pi f3, f9, 0x4\n" 
			"fsgnjx.ps f18, f4, f16\n"
			"addw x27, x20, x19\n"
			"fsra.pi f26, f15, f0\n"
			"beq x20, x30, 1f\n"
			"1:\n"
			"fslli.pi f28, f10, 0x3\n"
			"bge x22, x3, 1f\n"
			"1:\n"
			"fnmadd.s f9, f0, f0, f28, rne\n"
			"fsub.ps f21, f4, f15, rne\n"
			"fsrai.pi f16, f6, 0x7\n"
			"sraiw x15, x15, 2\n"
			"bltu x9, x28, 1f\n"
			"1:\n"
			"mova.x.m x15\n" 
			"fsra.pi f22, f13, f11\n"
			"bge x29, x4, 1f\n"
			"1:\n"
			"fslli.pi f21, f27, 0xd\n"
			"fslli.pi f16, f23, 0xe\n"
			"fsrai.pi f1, f19, 0xc\n"
			"fsrai.pi f31, f9, 0x1\n"
			"bge x15, x29, 1f\n"
			"1:\n"
			"fsra.pi f22, f26, f30\n"
			"fsub.ps f15, f3, f26, rne\n"
			"bne x21, x11, 1f\n"
			"1:\n"
			"fsrai.pi f22, f15, 0x2\n"
			"la x14, rand_ieee754_32\n"
			"flw.ps f28, 224(x14)\n"
			"flw.ps f19, 896(x14)\n"
			"flw.ps f12, 448(x14)\n"
			"fnmadd.s f20, f28, f19, f12, rmm\n" 
			"fcvt.ps.f16 f18, f10\n"
			"beq x11, x20, 1f\n"
			"1:\n"
			"fround.ps f26, f1, rtz\n"
			"ori x12, x19, 0x18\n"
			"fsrai.pi f11, f16, 0x5\n"
			"fltm.pi m2, f16, f3\n"
			"slt x14, x14, x0\n"
			"srlw x15, x15, x12\n"
			"masknot m0, m7\n"
			"mov.m.x m0, x0, 0xff\n"
			"la x10, rand_ieee754_32\n"
			"flw.ps f15, 352(x10)\n"
			"flw.ps f30, 928(x10)\n"
			"fsub.ps f25, f15, f30, rup\n" 
			"fnmadd.s f30, f0, f20, f23, rup\n"
			"bne x11, x15, 1f\n"
			"1:\n"
			"fnmadd.s f9, f23, f9, f9, rmm\n"
			"fcmovm.ps f9, f9, f23\n"
			"sraiw x10, x7, 4\n"
			"fmin.pi f18, f28, f13\n"
			"srliw x13, x13, 5\n"
			"fsub.ps f30, f9, f28, rmm\n"
			"fnmadd.s f28, f28, f28, f28, rtz\n"
			"li x5, 0xae8503885e22999\n"
			"li x19, 0x8defd554b6c67b86\n"
			"srlw x14, x5, x19\n" 
			"fmax.s f31, f1, f27\n"
			"bge x21, x2, 1f\n"
			"1:\n"
			"fround.ps f15, f20, rdn\n"
			"fcvt.ps.f16 f1, f20\n"
			"fcvt.ps.f16 f22, f22\n"
			"for.pi f31, f27, f10\n"
			"bgeu x27, x11, 1f\n"
			"1:\n"
			"fmadd.ps f31, f20, f1, f27, rmm\n"
			"bltu x20, x10, 1f\n"
			"1:\n"
			"fmin.pi f0, f22, f27\n"
			"fmadd.ps f15, f22, f1, f1, rmm\n"
			"li x27, 0xe3db4a01ffe7989d\n"
			"li x10, 0x4b68b1744cbe421a\n"
			"subw x10, x27, x10\n" 
			"subw x20, x27, x9\n"
			"fcvt.f16.ps f30, f30\n"
			"fcvt.ps.pw f30, f30, rdn\n"
			"fcvt.f16.ps f4, f30\n"
			"bgeu x22, x26, 1f\n"
			"1:\n"
			"srli x9, x7, 3\n"
			"mulh x20, x7, x9\n"
			"fsrai.pi f21, f2, 0xd\n"
			"fsra.pi f8, f2, f30\n"
			"bge x19, x8, 1f\n"
			"1:\n"
			"li x15, 0x686ccfa70a36b680\n"
			"srliw x13, x15, 5\n" 
			"fbci.pi f13, 0x7107e\n"
			"fmvs.x.ps x20, f12, 6\n"
			"subw x6, x5, x19\n"
			"blt x8, x12, 1f\n"
			"1:\n"
			"fsin.ps f7, f17\n"
			"blt x19, x14, 1f\n"
			"1:\n"
			"xori x15, x22, 0x1e\n"
			"fsra.pi f22, f5, f5\n"
			"srliw x11, x14, 2\n"
			"fxor.pi f12, f12, f5\n"
			"la x28, rand_ieee754_16\n"
			"flw.ps f2, 352(x28)\n"
			"fcvt.ps.f16 f31, f2\n" 
			"fmin.pi f28, f23, f0\n"
			"fmvs.x.ps x17, f26, 7\n"
			"fltm.pi m6, f7, f30\n"
			"addw x20, x20, x13\n"
			"bltu x8, x17, 1f\n"
			"1:\n"
			"fsgnjx.ps f29, f31, f29\n"
			"maskxor m0, m3, m7\n"
			"mov.m.x m0, x0, 0xff\n"
			"fmadd.ps f2, f24, f12, f20, rdn\n"
			"fsin.ps f26, f27\n"
			"li x12, 0xa6a62a3489194a60\n"
			"li x9, 0xde8d47822da2ac99\n"
			"slt x16, x12, x9\n" 
			"fmin.pi f7, f1, f28\n"
			"fsgnjx.ps f11, f26, f29\n"
			"fround.ps f2, f2, rdn\n"
			"mulw x28, x27, x28\n"
			"fxor.pi f3, f26, f29\n"
			"fmax.s f27, f26, f28\n"
			"fsgnjx.ps f8, f26, f26\n"
			"srliw x0, x15, 5\n"
			"la x13, rand_int_32\n"
			"flw.ps f8, 160(x13)\n"
			"flw.ps f1, 992(x13)\n"
			"for.pi f19, f8, f1\n" 
			"fsin.ps f28, f21\n"
			"fsra.pi f22, f22, f29\n"
			"fsub.ps f28, f0, f16, rup\n"
			"fxor.pi f30, f18, f16\n"
			"bgeu x9, x23, 1f\n"
			"1:\n"
			"for.pi f23, f15, f22\n"
			"bgeu x17, x31, 1f\n"
			"1:\n"
			"fsin.ps f30, f2\n"
			"bgeu x26, x19, 1f\n"
			"1:\n"
			"fsin.ps f28, f21\n"
			"fround.ps f28, f2, rne\n"
			"bltu x23, x9, 1f\n"
			"1:\n"
			"li x20, 0x375c33519e86cd6e\n"
			"li x25, 0xa392c3399ce53b0c\n"
			"srlw x10, x20, x25\n" 
			"fsatu8.pi f14, f29\n"
			"ori x19, x19, 0x13\n"
			"fcvt.ps.pw f6, f18, rtz\n"
			"fsatu8.pi f17, f3\n"
			"fnmadd.s f18, f18, f14, f12, rdn\n"
			"fnmadd.s f13, f2, f16, f3, rup\n"
			"sraiw x25, x14, 2\n"
			"fmin.pi f6, f1, f26\n"
			"bgeu x10, x15, 1f\n"
			"1:\n"
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_0:\n"
			"maskxor m2, m0, m6\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"srlw x10, x27, x16\n"
			"bltu x4, x18, 1f\n"
			"1:\n"
			"auipc x10, 0x1b\n"
			"fadd.s f2, f17, f1, rne\n"
			"fmvs.x.ps x14, f19, 6\n"
			"fsatu8.pi f19, f9\n"
			"auipc x9, 0x19\n"
			"fmadd.ps f22, f16, f8, f17, rmm\n"
			"fcmovm.ps f17, f16, f19\n"
			"li x22, 0xd320ece143191110\n"
			"andi x6, x22, 0xc\n" 
			"fcvt.f16.ps f25, f0\n"
			"fsrai.pi f0, f25, 0x1\n"
			"fxor.pi f20, f0, f6\n"
			"fround.ps f0, f29, rup\n"
			"blt x31, x7, 1f\n"
			"1:\n"
			"fcvt.ps.pw f8, f25, rdn\n"
			"bge x22, x10, 1f\n"
			"1:\n"
			"sraiw x11, x14, 3\n"
			"beq x3, x18, 1f\n"
			"1:\n"
			"fbci.pi f15, 0x5f04c\n"
			"fmadd.ps f0, f25, f25, f0, rup\n"
			"beq x18, x19, 1f\n"
			"1:\n"
			"li x9, 0x424ff63b7e2c7a49\n"
			"andi x26, x9, 0xf\n" 
			"for.pi f20, f20, f13\n"
			"beq x7, x16, 1f\n"
			"1:\n"
			"fsrai.pi f1, f16, 0x5\n"
			"fcmovm.ps f19, f14, f12\n"
			"sraiw x16, x16, 3\n"
			"fsgnjx.ps f7, f0, f1\n"
			"fsub.ps f31, f6, f16, rne\n"
			"fslli.pi f21, f15, 0xd\n"
			"fnmadd.s f9, f29, f26, f9, rdn\n"
			"li x6, 0x551ce9f07d87db0a\n"
			"li x14, 0xc9bf32fe71bc6b9c\n"
			"addw x20, x6, x14\n" 
			"fcvt.ps.f16 f16, f20\n"
			"fcvt.f16.ps f24, f24\n"
			"sraiw x19, x19, 3\n"
			"srli x17, x16, 3\n"
			"fmv.x.s x9, f0\n"
			"ebreak\n"
			"srliw x12, x12, 1\n"
			"fmvs.x.ps x12, f30, 1\n"
			"la x27, rand_ieee754_32\n"
			"flw.ps f24, 992(x27)\n"
			"flw.ps f18, 864(x27)\n"
			"fmax.s f21, f24, f18\n" 
			"fcvt.ps.pw f24, f24, rdn\n"
			"fsra.pi f1, f24, f5\n"
			"fmvs.x.ps x9, f0, 3\n"
			"masknot m3, m3\n"
			"fsatu8.pi f24, f15\n"
			"fsra.pi f19, f21, f9\n"
			"fmax.s f9, f9, f24\n"
			"blt x22, x9, 1f\n"
			"1:\n"
			"fcvt.ps.pw f21, f21, rmm\n"
			"bgeu x13, x8, 1f\n"
			"1:\n"
			"la x27, rand_ieee754_32\n"
			"flw.ps f21, 352(x27)\n"
			"flw.ps f29, 928(x27)\n"
			"flw.ps f1, 320(x27)\n"
			"fnmadd.s f14, f21, f29, f1, rne\n" 
			"fbci.pi f5, 0x5c4f4\n"
			"bge x10, x16, 1f\n"
			"1:\n"
			"auipc x28, 0x1d\n"
			"fcmovm.ps f4, f10, f5\n"
			"bne x6, x27, 1f\n"
			"1:\n"
			"fnmadd.s f20, f5, f5, f5, rdn\n"
			"auipc x16, 0x12\n"
			"bgeu x25, x4, 1f\n"
			"1:\n"
			"for.pi f5, f3, f5\n"
			"fxor.pi f4, f3, f5\n"
			"andi x13, x27, 0x1b\n"
			"la x25, rand_ieee754_32\n"
			"flw.ps f19, 448(x25)\n"
			"flw.ps f18, 384(x25)\n"
			"flw.ps f27, 288(x25)\n"
			"fnmadd.s f8, f19, f18, f27, rup\n" 
			"fmin.pi f30, f6, f22\n"
			"fltm.pi m1, f19, f6\n"
			"fround.ps f3, f3, rne\n"
			"fadd.s f28, f19, f3, rtz\n"
			"fadd.s f4, f22, f5, rmm\n"
			"fslli.pi f22, f22, 0x5\n"
			"fslli.pi f6, f6, 0x1\n"
			"fcmovm.ps f28, f5, f6\n"
			"beq x31, x14, 1f\n"
			"1:\n"
			"ebreak\n" 
			"fround.ps f31, f31, rmm\n"
			"blt x29, x12, 1f\n"
			"1:\n"
			"fsrai.pi f14, f31, 0x9\n"
			"fsra.pi f6, f15, f29\n"
			"fmvs.x.ps x7, f4, 4\n"
			"fcvt.f16.ps f12, f4\n"
			"xori x15, x9, 0x17\n"
			"fxor.pi f11, f29, f4\n"
			"fmax.s f31, f26, f31\n"
			"li x25, 0x9a32a9e0ed1ab761\n"
			"xori x27, x25, 0x1e\n" 
			"frsq.ps f4, f4\n"
			"bge x8, x25, 1f\n"
			"1:\n"
			"mulw x15, x12, x23\n"
			"fxor.pi f19, f22, f1\n"
			"fcvt.ps.pw f20, f18, rdn\n"
			"ori x12, x12, 0x14\n"
			"fcmovm.ps f12, f22, f4\n"
			"auipc x27, 0x16\n"
			"fmin.pi f25, f22, f9\n"
			"la x5, rand_ieee754_32\n"
			"flw.ps f9, 192(x5)\n"
			"flw.ps f1, 352(x5)\n"
			"flw.ps f28, 640(x5)\n"
			"fnmadd.s f29, f9, f1, f28, rne\n" 
			"for.pi f13, f3, f2\n"
			"fcvt.f16.ps f3, f22\n"
			"fcvt.ps.pw f25, f10, rup\n"
			"fmadd.ps f7, f7, f31, f1, rdn\n"
			"fadd.s f12, f18, f6, rne\n"
			"beq x24, x28, 1f\n"
			"1:\n"
			"fadd.s f20, f29, f20, rdn\n"
			"beq x6, x18, 1f\n"
			"1:\n"
			"fsub.ps f4, f4, f29, rdn\n"
			"bgeu x23, x17, 1f\n"
			"1:\n"
			"fmadd.ps f2, f5, f5, f26, rne\n"
			"bge x20, x21, 1f\n"
			"1:\n"
			"la x9, rand_ieee754_32\n"
			"flw.ps f3, 864(x9)\n"
			"fmv.x.s x6, f3\n" 
			"fcmovm.ps f30, f21, f30\n"
			"fround.ps f6, f0, rtz\n"
			"fsatu8.pi f30, f30\n"
			"fsrai.pi f21, f17, 0xc\n"
			"fsatu8.pi f20, f0\n"
			"fmin.pi f6, f21, f21\n"
			"fcvt.ps.f16 f21, f17\n"
			"fsatu8.pi f6, f17\n"
			"bgeu x26, x10, 1f\n"
			"1:\n"
			"la x28, rand_ieee754_32\n"
			"flw.ps f24, 832(x28)\n"
			"flw.ps f7, 768(x28)\n"
			"fsgnjx.ps f15, f24, f7\n" 
			"fcvt.f16.ps f15, f5\n"
			"fcvt.ps.f16 f26, f9\n"
			"fxor.pi f26, f23, f26\n"
			"xori x9, x15, 0x15\n"
			"sraiw x19, x19, 5\n"
			"fcvt.s.w f8, x22, rup\n"
			"fcvt.ps.pw f13, f23, rne\n"
			"blt x0, x5, 1f\n"
			"1:\n"
			"andi x15, x15, 0x1a\n"
			"la x18, rand_int_32\n"
			"flw.ps f1, 736(x18)\n"
			"fsatu8.pi f29, f1\n" 
			"fmadd.ps f18, f24, f28, f24, rdn\n"
			"sraiw x25, x9, 2\n"
			"fmax.s f2, f2, f2\n"
			"fsatu8.pi f2, f28\n"
			"fmadd.ps f31, f2, f24, f24, rtz\n"
			"for.pi f2, f2, f2\n"
			"addw x17, x6, x9\n"
			"fsra.pi f31, f24, f28\n"
			"li x11, 0xf08126a65278813b\n"
			"srli x5, x11, 4\n" 
			"subw x13, x16, x18\n"
			"bltu x31, x4, 1f\n"
			"1:\n"
			"fsatu8.pi f31, f19\n"
			"fadd.s f20, f0, f0, rmm\n"
			"fsub.ps f19, f2, f2, rtz\n"
			"mulw x13, x16, x16\n"
			"mulh x5, x26, x18\n"
			"auipc x5, 0x3\n"
			"bge x15, x27, 1f\n"
			"1:\n"
			"auipc x18, 0x12\n"
			"la x14, rand_ieee754_16\n"
			"flw.ps f3, 32(x14)\n"
			"fcvt.ps.f16 f20, f3\n" 
			"li x5, 0xc9\n"
			"csrw tensor_mask, x5\n"	
			"fsin.ps f7, f7\n"
			"fxor.pi f14, f25, f15\n"
			"fcvt.f16.ps f24, f28\n"
			"fadd.s f10, f16, f16, rtz\n"
			"fcvt.f16.ps f0, f19\n"
			"fsub.ps f28, f26, f6, rup\n"
			"beq x17, x13, 1f\n"
			"1:\n"
			"fcvt.ps.pw f30, f8, rdn\n"
			"fcvt.ps.pw f21, f7, rdn\n"
			"la x10, rand_ieee754_32\n"
			"flw.ps f6, 576(x10)\n"
			"flw.ps f7, 384(x10)\n"
			"fsub.ps f29, f6, f7, rmm\n" 
			"fmvs.x.ps x25, f22, 6\n"
			"fround.ps f18, f6, rne\n"
			"fsin.ps f20, f20\n"
			"andi x20, x20, 0x3\n"
			"fsatu8.pi f23, f25\n"
			"mova.x.m x7\n"
			"fsrai.pi f29, f29, 0x8\n"
			"slt x23, x9, x22\n"
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_0:\n"
			"maskand m2, m0, m7\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"slt x12, x28, x19\n"
			"bne x18, x15, 1f\n"
			"1:\n"
			"fround.ps f29, f26, rmm\n"
			"for.pi f3, f30, f31\n"
			"maskxor m4, m4, m3\n"
			"fsin.ps f25, f25\n"
			"beq x28, x23, 1f\n"
			"1:\n"
			"fsatu8.pi f18, f0\n"
			"fnmadd.s f8, f0, f29, f19, rtz\n"
			"fcvt.f16.ps f0, f15\n"
			"LBL_SEQID_0_M0_WRITE_ID_2_HID_0:\n"
			"maskxor m4, m6, m6\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"fmv.x.s x5, f17\n"
			"bge x7, x22, 1f\n"
			"1:\n"
			"addw x5, x6, x5\n"
			"ebreak\n"
			"ori x16, x16, 0xe\n"
			"srli x22, x6, 1\n"
			"bgeu x0, x27, 1f\n"
			"1:\n"
			"mulh x14, x6, x6\n"
			"fnmadd.s f19, f28, f21, f21, rtz\n"
			"fcvt.f16.ps f13, f17\n"
			"la x23, rand_int_32\n"
			"flw.ps f26, 864(x23)\n"
			"flw.ps f5, 192(x23)\n"
			"for.pi f16, f26, f5\n" 
			"fxor.pi f22, f15, f27\n"
			"fadd.s f30, f9, f15, rup\n"
			"frsq.ps f10, f22\n"
			"srliw x10, x10, 1\n"
			"for.pi f11, f28, f13\n"
			"bge x13, x31, 1f\n"
			"1:\n"
			"fcvt.s.w f13, x27, rtz\n"
			"fcvt.f16.ps f27, f11\n"
			"bgeu x28, x17, 1f\n"
			"1:\n"
			"srliw x23, x5, 5\n"
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
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ITER_0_HID_0:\n"
			"li x6, ((0x8 << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0x2 << TENSOR_QUANT_QUANT_ROW) | (0x1f << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 10\n"
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
			"li x5, 0x8\n"
			"la x31, rand_ieee754_32\n"
			"add x31, x31, x5\n"
			"flw.ps f0, 0(x31)\n"
			"flw.ps f12, 1(x31)\n"
			"flw.ps f18, 5(x31)\n"
			"flw.ps f6, 1(x31)\n"
			"la x31, rand_int_32\n"
			"add x31, x31, x5\n"
			"lw x0, 0(x31)\n"
			"lw x12, 0(x31)\n"
			"lw x20, 4(x31)\n"
			"li x21, 0x1\n"
			"LBL_SEQID_0_LOOP_SEQ_HID_0:\n"
			"fcvt.ps.f16 f0, f18\n"
			"fmax.s f21, f6, f0\n"
			"fltm.pi m1, f18, f18\n"
			"andi x19, x0, 0x3\n"
			"fcvt.ps.pw f0, f0, rne\n"
			"fnmadd.s f0, f12, f0, f6, rdn\n"
			"ori x20, x12, 0x3\n"
			"fnmadd.s f22, f6, f6, f12, rne\n"
			"fround.ps f0, f18, rmm\n"
			"mova.x.m x7\n"
			"for.pi f12, f12, f12\n"
			"subw x20, x20, x20\n"
			"xori x20, x12, 0xa\n"
			"maskxor m6, m2, m2\n"
			"bgeu x24, x2, 1f\n"
			"1:\n"
			"fsrai.pi f25, f0, 0xb\n"
			"andi x7, x0, 0x8\n"
			"fsin.ps f6, f0\n"
			"frsq.ps f0, f0\n"
			"fround.ps f22, f6, rdn\n"
			"andi x12, x12, 0x1b\n"
			"srli x19, x12, 3\n"
			"fbci.pi f0, 0x706b\n"
			"fround.ps f13, f0, rne\n"
			"srlw x12, x20, x12\n"
			"beq x23, x25, 1f\n"
			"1:\n"
			"xori x10, x0, 0x8\n"
			"fsub.ps f21, f0, f12, rmm\n"
			"fmv.x.s x20, f12\n"
			"blt x27, x19, 1f\n"
			"1:\n"
			"maskxor m2, m1, m2\n"
			"bge x14, x18, 1f\n"
			"1:\n"
			"subw x19, x12, x0\n"
			"fmvs.x.ps x14, f18, 7\n"
			"fcvt.s.w f22, x20, rne\n"
			"bge x2, x15, 1f\n"
			"1:\n"
			"subw x0, x0, x0\n"
			"maskxor m2, m1, m2\n"
			"bgeu x1, x25, 1f\n"
			"1:\n"
			"fmadd.ps f16, f12, f6, f0, rne\n"
			"subw x7, x12, x0\n"
			"fcvt.s.w f0, x0, rmm\n"
			"fsrai.pi f12, f12, 0x6\n"
			"fsra.pi f21, f18, f0\n"
			"bgeu x8, x13, 1f\n"
			"1:\n"
			"andi x12, x12, 0x5\n"
			"fsin.ps f21, f12\n"
			"fxor.pi f0, f18, f12\n"
			"maskxor m2, m2, m1\n"
			"srliw x20, x0, 1\n"
			"auipc x12, 0xa\n"
			"fnmadd.s f0, f0, f12, f0, rdn\n"
			"fmax.s f0, f12, f12\n"
			"bge x17, x7, 1f\n"
			"1:\n"
			"maskand m1, m2, m2\n"
			"fcvt.ps.f16 f22, f6\n"
			"addw x20, x20, x12\n"
			"bge x0, x3, 1f\n"
			"1:\n"
			"srliw x13, x0, 3\n"
			"fmv.x.s x0, f6\n"
			"fsin.ps f13, f18\n"
			"fcvt.ps.f16 f16, f0\n"
			"bgeu x24, x23, 1f\n"
			"1:\n"
			"mova.x.m x13\n"
			"mulw x0, x0, x0\n"
			"maskand m6, m1, m1\n"
			"fmv.x.s x12, f18\n"
			"fadd.s f25, f12, f6, rmm\n"
			"fslli.pi f16, f18, 0x4\n"
			"fbci.pi f6, 0x7a0e9\n"
			"fbci.pi f13, 0x56214\n"
			"xori x14, x12, 0x8\n"
			"srlw x20, x0, x20\n"
			"fslli.pi f13, f18, 0x9\n"
			"bgeu x2, x7, 1f\n"
			"1:\n"
			"mova.x.m x7\n"
			"fslli.pi f12, f6, 0x8\n"
			"fcvt.ps.pw f21, f18, rtz\n"
			"fcmovm.ps f0, f0, f6\n"
			"blt x21, x20, 1f\n"
			"1:\n"
			"xori x0, x0, 0x15\n"
			"bltu x12, x15, 1f\n"
			"1:\n"
			"ori x10, x20, 0x1f\n"
			"srli x13, x12, 0\n"
			"subw x10, x0, x20\n"
			"fcmovm.ps f6, f6, f6\n"
			"maskand m6, m1, m1\n"
			"blt x16, x18, 1f\n"
			"1:\n"
			"for.pi f18, f12, f18\n"
			"addw x14, x0, x12\n"
			"fmv.x.s x0, f0\n"
			"bgeu x8, x23, 1f\n"
			"1:\n"
			"fcmovm.ps f21, f0, f6\n"
			"fadd.s f16, f18, f0, rmm\n"
			"fltm.pi m2, f12, f0\n"
			"mova.x.m x0\n"
			"fsrai.pi f25, f0, 0x7\n"
			"mova.x.m x19\n"
			"srliw x14, x20, 0\n"
			"addw x14, x0, x0\n"
			"mova.x.m x12\n"
			"auipc x7, 0xf\n"
			"fsin.ps f21, f18\n"
			"fxor.pi f0, f0, f12\n"
			"mulw x10, x20, x0\n"
			"masknot m1, m2\n"
			"auipc x19, 0x19\n"
			"fsrai.pi f0, f12, 0xf\n"
			"fslli.pi f16, f6, 0xe\n"
			"fsra.pi f16, f6, f12\n"
			"xori x0, x0, 0x1e\n"
			"bgeu x16, x11, 1f\n"
			"1:\n"
			"fsgnjx.ps f13, f6, f6\n"
			"beq x18, x24, 1f\n"
			"1:\n"
			"auipc x20, 0x9\n"
			"fcvt.s.w f16, x12, rne\n"
			"bgeu x29, x29, 1f\n"
			"1:\n"
			"ebreak\n"
			"fadd.s f0, f18, f12, rup\n"
			"auipc x19, 0x4\n"
			"fsin.ps f21, f12\n"
			"fcvt.f16.ps f12, f0\n"
			"srliw x10, x0, 2\n"
			"mova.x.m x0\n"
			"mulh x20, x12, x20\n"
			"blt x10, x22, 1f\n"
			"1:\n"
			"fmax.s f13, f18, f18\n"
			"beq x5, x11, 1f\n"
			"1:\n"
			"srli x10, x12, 5\n"
			"fsin.ps f13, f6\n"
			"andi x13, x20, 0x3\n"
			"fcvt.ps.f16 f25, f12\n"
			"fltm.pi m2, f18, f0\n"
			"fbci.pi f18, 0x7dca9\n"
			"fsra.pi f16, f18, f6\n"
			"fmin.pi f13, f0, f0\n"
			"bne x16, x17, 1f\n"
			"1:\n"
			"fmvs.x.ps x19, f18, 5\n"
			"blt x16, x9, 1f\n"
			"1:\n"
			"fxor.pi f0, f0, f6\n"
			"ebreak\n"
			"fmv.x.s x10, f12\n"
			"fmax.s f12, f18, f6\n"
			"bltu x0, x27, 1f\n"
			"1:\n"
			"fcvt.f16.ps f18, f6\n"
			"srliw x13, x12, 0\n"
			"fsrai.pi f12, f12, 0xe\n"
			"fsgnjx.ps f22, f18, f18\n"
			"fsin.ps f21, f12\n"
			"fsin.ps f16, f0\n"
			"fcmovm.ps f12, f0, f18\n"
			"fcvt.ps.pw f18, f18, rne\n"
			"ebreak\n"
			"ori x20, x0, 0x1c\n"
			"fsra.pi f0, f12, f0\n"
			"fmulhu.pi f25, f18, f12\n"
			"fxor.pi f0, f0, f6\n"
			"addw x19, x20, x12\n"
			"fnmadd.s f25, f0, f18, f0, rdn\n"
			"blt x16, x0, 1f\n"
			"1:\n"
			"maskxor m1, m1, m1\n"
			"fxor.pi f0, f18, f12\n"
			"fxor.pi f0, f0, f0\n"
			"beq x24, x19, 1f\n"
			"1:\n"
			"ebreak\n"
			"srli x10, x20, 1\n"
			"fcmovm.ps f13, f12, f0\n"
			"fcvt.s.w f0, x20, rtz\n"
			"fmv.x.s x7, f18\n"
			"fsrai.pi f12, f12, 0xd\n"
			"maskxor m2, m1, m2\n"
			"slt x12, x0, x12\n"
			"fnmadd.s f6, f6, f12, f0, rup\n"
			"fcmovm.ps f6, f6, f6\n"
			"blt x14, x9, 1f\n"
			"1:\n"
			"fxor.pi f16, f0, f12\n"
			"fltm.pi m2, f6, f12\n"
			"addw x14, x20, x0\n"
			"maskxor m1, m1, m2\n"
			"fcmovm.ps f18, f18, f12\n"
			"bge x22, x30, 1f\n"
			"1:\n"
			"mulh x14, x20, x0\n"
			"fsrai.pi f0, f0, 0x4\n"
			"ori x10, x0, 0x19\n"
			"sraiw x0, x20, 3\n"
			"srliw x19, x12, 1\n"
			"fsin.ps f6, f12\n"
			"fsra.pi f0, f0, f18\n"
			"bgeu x6, x2, 1f\n"
			"1:\n"
			"maskand m1, m1, m1\n"
			"ebreak\n"
			"fcmovm.ps f0, f6, f18\n"
			"fsra.pi f13, f18, f12\n"
			"blt x5, x30, 1f\n"
			"1:\n"
			"mulh x12, x12, x12\n"
			"masknot m1, m1\n"
			"fsgnjx.ps f0, f18, f0\n"
			"blt x11, x26, 1f\n"
			"1:\n"
			"fmadd.ps f13, f12, f6, f12, rdn\n"
			"fcmovm.ps f25, f6, f6\n"
			"bltu x28, x6, 1f\n"
			"1:\n"
			"addw x7, x12, x12\n"
			"bge x20, x19, 1f\n"
			"1:\n"
			"ori x13, x0, 0x1\n"
			"fcvt.ps.f16 f12, f6\n"
			"srli x19, x0, 2\n"
			"xori x10, x12, 0x10\n"
			"ori x0, x0, 0x5\n"
			"addi x21, x21, -1\n"
			"bne x21, x0, LBL_SEQID_0_LOOP_SEQ_HID_0\n"
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_0\n"
			"addi x30, x30, 1\n"
			"la x11, rand_ieee754_32\n"
			"flw.ps f2, 800(x11)\n"
			"fmv.x.s x22, f2\n" 
			"fround.ps f21, f7, rdn\n"
			"auipc x28, 0x9\n"
			"bltu x12, x18, 1f\n"
			"1:\n"
			"fmax.s f20, f7, f17\n"
			"fbci.pi f31, 0x6c006\n"
			"fnmadd.s f6, f6, f21, f17, rdn\n"
			"ebreak\n"
			"LBL_SEQID_1_M0_WRITE_ID_3_HID_0:\n"
			"maskxor m2, m6, m4\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"ebreak\n"
			"auipc x9, 0x1c\n"
			"fmin.pi f1, f28, f18\n"
			"masknot m6, m6\n"
			"fcvt.s.w f9, x27, rup\n"
			"fmv.x.s x7, f30\n"
			"la x23, rand_ieee754_32\n"
			"flw.ps f4, 800(x23)\n"
			"fcvt.f16.ps f2, f4\n" 
			"slt x25, x27, x18\n"
			"mulw x27, x7, x13\n"
			"fsub.ps f29, f16, f7, rdn\n"
			"fnmadd.s f29, f7, f7, f29, rup\n"
			"slt x14, x19, x5\n"
			"blt x16, x24, 1f\n"
			"1:\n"
			"fxor.pi f30, f13, f29\n"
			"fbci.pi f23, 0x7660a\n" 
			"fcvt.ps.f16 f13, f14\n"
			"andi x13, x27, 0x11\n"
			"blt x7, x24, 1f\n"
			"1:\n"
			"sraiw x6, x20, 4\n"
			"fbci.pi f26, 0x28c3c\n"
			"fnmadd.s f7, f26, f0, f23, rup\n"
			"fslli.pi f22, f1, 0x4\n"
			"la x15, rand_ieee754_32\n"
			"flw.ps f26, 192(x15)\n"
			"flw.ps f24, 960(x15)\n"
			"fsgnjx.ps f3, f26, f24\n" 
			"for.pi f10, f2, f10\n"
			"srliw x15, x13, 1\n"
			"bltu x7, x20, 1f\n"
			"1:\n"
			"xori x10, x13, 0x19\n"
			"srliw x13, x17, 2\n"
			"fmax.s f28, f22, f1\n"
			"fslli.pi f10, f4, 0xd\n"
			"blt x11, x6, 1f\n"
			"1:\n"
			"li x15, 0x8c8df6c5f18ab344\n"
			"srli x6, x15, 1\n" 
			"li x5, 0x2681\n"
			"csrw tensor_mask, x5\n"	
			"mulh x17, x12, x5\n"
			"fsin.ps f29, f19\n"
			"fsub.ps f18, f29, f19, rmm\n"
			"fadd.s f5, f29, f19, rne\n"
			"fsub.ps f6, f30, f6, rtz\n"
			"fmulhu.pi f3, f16, f30\n"
			"mova.x.m x12\n" 
			"andi x18, x6, 0x15\n"
			"fxor.pi f26, f29, f0\n"
			"fcmovm.ps f20, f4, f20\n"
			"bge x13, x3, 1f\n"
			"1:\n"
			"fcmovm.ps f7, f12, f26\n"
			"fsin.ps f17, f31\n"
			"fcmovm.ps f5, f21, f30\n"
			"bge x20, x13, 1f\n"
			"1:\n"
			"li x5, 0xc9c40c0eb3d0fec5\n"
			"srliw x17, x5, 2\n" 
			"fmulhu.pi f7, f1, f6\n"
			"maskxor m5, m5, m5\n"
			"fmv.x.s x26, f1\n"
			"bge x22, x28, 1f\n"
			"1:\n"
			"fsgnjx.ps f7, f6, f1\n"
			"srliw x7, x18, 5\n"
			"bltu x2, x7, 1f\n"
			"1:\n"
			"fcvt.ps.f16 f7, f7\n"
			"blt x13, x18, 1f\n"
			"1:\n"
			"la x27, rand_int_32\n"
			"flw.ps f13, 128(x27)\n"
			"flw.ps f3, 992(x27)\n"
			"fsra.pi f3, f13, f3\n" 
			"fsgnjx.ps f24, f27, f6\n"
			"bge x27, x6, 1f\n"
			"1:\n"
			"fround.ps f26, f21, rmm\n"
			"bgeu x10, x10, 1f\n"
			"1:\n"
			"fcvt.ps.f16 f27, f21\n"
			"fsub.ps f10, f4, f4, rne\n"
			"fcvt.f16.ps f23, f14\n"
			"fcvt.ps.pw f6, f24, rdn\n"
			"LBL_SEQID_1_M0_WRITE_ID_4_HID_0:\n"
			"maskxor m4, m6, m1\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0x86119c9822\n"
			"fsw.ps f0, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"fnmadd.s f17, f14, f27, f4, rne\n"
			"fcvt.f16.ps f29, f9\n"
			"fcvt.s.w f22, x17, rne\n"
			"bge x31, x27, 1f\n"
			"1:\n"
			"mulh x28, x19, x23\n"
			"fmvs.x.ps x26, f3, 1\n"
			"masknot m4, m3\n"
			"li x11, 0x19a4e7c3cd79ba28\n"
			"li x9, 0x36f788880b7873a\n"
			"slt x23, x11, x9\n" 
			"fadd.s f23, f13, f13, rne\n"
			"beq x0, x30, 1f\n"
			"1:\n"
			"fadd.s f15, f7, f20, rmm\n"
			"slt x13, x18, x16\n"
			"fmulhu.pi f8, f8, f8\n"
			"fround.ps f25, f25, rtz\n"
			"addw x10, x28, x18\n"
			"la x7, rand_ieee754_32\n"
			"flw.ps f4, 576(x7)\n"
			"flw.ps f31, 0(x7)\n"
			"fsub.ps f8, f4, f31, rup\n" 
			"fsra.pi f23, f17, f14\n"
			"fnmadd.s f29, f29, f11, f29, rtz\n"
			"ori x23, x7, 0xf\n"
			"fadd.s f12, f12, f10, rtz\n"
			"blt x10, x5, 1f\n"
			"1:\n"
			"andi x19, x5, 0x1\n"
			"subw x26, x20, x0\n"
			"li x20, 0x639236c9b450d07e\n"
			"li x6, 0xb4f40a4aae5f8efa\n"
			"srlw x22, x20, x6\n" 
			"masknot m2, m4\n"
			"xori x18, x18, 0x2\n"
			"addw x14, x28, x20\n"
			"mulw x13, x26, x17\n"
			"fround.ps f3, f7, rtz\n"
			"mulh x13, x17, x17\n"
			"li x10, 0x2327361a242b2159\n"
			"li x9, 0xe9a4dac7ecd8b5ed\n"
			"addw x10, x10, x9\n" 
			"srliw x22, x22, 4\n"
			"fadd.s f16, f16, f17, rmm\n"
			"fsatu8.pi f10, f10\n"
			"fsub.ps f7, f31, f2, rne\n"
			"subw x5, x5, x6\n"
			"bne x29, x14, 1f\n"
			"1:\n"
			"fmin.pi f18, f18, f6\n"
			"li x19, 0xfac440a5f80822d5\n"
			"li x12, 0xefe79eb118c7e0a3\n"
			"mulh x9, x19, x12\n" 
			"masknot m6, m5\n"
			"ebreak\n"
			"fxor.pi f3, f10, f2\n"
			"fmax.s f28, f19, f2\n"
			"fmv.x.s x15, f10\n"
			"masknot m7, m2\n"
			"la x19, rand_ieee754_32\n"
			"flw.ps f26, 0(x19)\n"
			"flw.ps f16, 96(x19)\n"
			"flw.ps f20, 960(x19)\n"
			"fmadd.ps f5, f26, f16, f20, rup\n" 
			"sraiw x27, x10, 2\n"
			"sraiw x5, x7, 2\n"
			"fadd.s f26, f5, f25, rtz\n"
			"fmulhu.pi f30, f30, f8\n"
			"fslli.pi f20, f25, 0xc\n"
			"fmvs.x.ps x27, f19, 5\n"
			"li x6, 0x32077c5227b2653b\n"
			"andi x18, x6, 0x3\n" 
			"fmadd.ps f30, f24, f30, f1, rtz\n"
			"bge x24, x21, 1f\n"
			"1:\n"
			"andi x19, x22, 0xc\n"
			"fbci.pi f27, 0x3b7f1\n"
			"bgeu x2, x6, 1f\n"
			"1:\n"
			"mova.x.m x19\n"
			"fsin.ps f25, f17\n"
			"fsatu8.pi f22, f18\n"
			"bne x3, x20, 1f\n"
			"1:\n"
			"li x5, 0x15a9b2980bc017d\n"
			"xori x17, x5, 0x9\n" 
			"fcvt.ps.pw f0, f27, rne\n"
			"fsub.ps f8, f12, f4, rup\n"
			"subw x26, x17, x14\n"
			"for.pi f27, f14, f17\n"
			"blt x27, x7, 1f\n"
			"1:\n"
			"andi x19, x10, 0xe\n"
			"fsra.pi f17, f4, f16\n"
			"la x22, rand_int_32\n"
			"flw.ps f2, 288(x22)\n"
			"fsrai.pi f26, f2, 0xb\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"fround.ps f0, f4, rtz\n"
			"fcmovm.ps f22, f13, f7\n"
			"fxor.pi f12, f3, f14\n"
			"fsub.ps f21, f20, f7, rup\n"
			"fadd.s f26, f26, f31, rtz\n"
			"fsin.ps f8, f22\n"
			"la x23, rand_int_32\n"
			"flw.ps f12, 128(x23)\n"
			"fsrai.pi f18, f12, 0x0\n" 
			"maskand m3, m7, m3\n"
			"bne x14, x16, 1f\n"
			"1:\n"
			"fcvt.ps.pw f7, f23, rtz\n"
			"fsgnjx.ps f21, f12, f16\n"
			"fsra.pi f2, f23, f17\n"
			"bge x12, x17, 1f\n"
			"1:\n"
			"fmv.x.s x27, f7\n"
			"blt x0, x9, 1f\n"
			"1:\n"
			"for.pi f0, f7, f16\n"
			"li x18, 0x5e915018227c8f9c\n"
			"li x28, 0x9fae20ca91b6679b\n"
			"slt x12, x18, x28\n" 
			"li x5, 0x1472\n"
			"csrw tensor_mask, x5\n"	
			"maskand m0, m0, m0\n"
			"fmv.x.s x13, f18\n"
			"fcvt.f16.ps f25, f14\n"
			"fmvs.x.ps x17, f2, 2\n"
			"auipc x28, 0x9\n"
			"for.pi f10, f11, f22\n"
			"la x13, rand_ieee754_32\n"
			"flw.ps f0, 672(x13)\n"
			"flw.ps f3, 416(x13)\n"
			"flw.ps f27, 576(x13)\n"
			"fmadd.ps f13, f0, f3, f27, rup\n" 
			"subw x5, x5, x11\n"
			"fcvt.ps.pw f4, f20, rmm\n"
			"subw x5, x7, x5\n"
			"andi x27, x23, 0xc\n"
			"slt x6, x5, x16\n"
			"auipc x5, 0x15\n"
			"la x17, rand_int_32\n"
			"flw.ps f16, 832(x17)\n"
			"fcvt.ps.pw f15, f16, rne\n" 
			"fmax.s f8, f10, f16\n"
			"fnmadd.s f14, f26, f13, f25, rmm\n"
			"beq x1, x30, 1f\n"
			"1:\n"
			"masknot m5, m5\n"
			"fltm.pi m6, f15, f2\n"
			"fcvt.s.w f0, x17, rmm\n"
			"fnmadd.s f1, f1, f28, f25, rne\n"
			"auipc x11, 0x18\n" 
			"srlw x0, x0, x14\n"
			"bgeu x3, x27, 1f\n"
			"1:\n"
			"fadd.s f19, f27, f19, rup\n"
			"for.pi f3, f27, f13\n"
			"blt x9, x24, 1f\n"
			"1:\n"
			"fcmovm.ps f5, f27, f30\n"
			"frsq.ps f3, f11\n"
			"srlw x20, x0, x15\n"
			"la x28, rand_ieee754_32\n"
			"flw.ps f14, 160(x28)\n"
			"fmv.x.s x7, f14\n" 
			"fmulhu.pi f13, f6, f21\n"
			"fmulhu.pi f22, f22, f29\n"
			"fcvt.ps.pw f20, f20, rup\n"
			"fmulhu.pi f12, f11, f16\n"
			"fmax.s f26, f20, f1\n"
			"fcvt.ps.f16 f12, f28\n"
			"fbci.pi f19, 0x7becf\n" 
			"fmulhu.pi f17, f11, f10\n"
			"for.pi f12, f12, f5\n"
			"bltu x30, x6, 1f\n"
			"1:\n"
			"fsatu8.pi f2, f16\n"
			"blt x10, x0, 1f\n"
			"1:\n"
			"slt x19, x28, x7\n"
			"fsin.ps f4, f13\n"
			"fxor.pi f30, f20, f11\n"
			"li x22, 0x39260e79f9c7691f\n"
			"sraiw x7, x22, 2\n" 
			"fcvt.ps.pw f20, f18, rup\n"
			"bltu x6, x19, 1f\n"
			"1:\n"
			"fmadd.ps f30, f14, f26, f18, rtz\n"
			"fmulhu.pi f30, f18, f14\n"
			"fsgnjx.ps f0, f26, f0\n"
			"fxor.pi f27, f14, f27\n"
			"fsin.ps f30, f22\n"
			"la x16, rand_int_32\n"
			"flw.ps f14, 640(x16)\n"
			"flw.ps f22, 736(x16)\n"
			"for.pi f0, f14, f22\n" 
			"fmadd.ps f23, f23, f16, f17, rtz\n"
			"srlw x17, x27, x10\n"
			"fsrai.pi f15, f14, 0xb\n"
			"for.pi f22, f17, f25\n"
			"fsatu8.pi f16, f16\n"
			"blt x31, x18, 1f\n"
			"1:\n"
			"frsq.ps f29, f13\n"
			"bgeu x10, x28, 1f\n"
			"1:\n"
			"li x9, 0x1079e4744e0c200d\n"
			"ori x19, x9, 0x2\n" 
			"xori x14, x23, 0x1b\n"
			"fsrai.pi f12, f25, 0x9\n"
			"ori x26, x16, 0x1c\n"
			"srli x7, x11, 4\n"
			"ori x14, x5, 0xa\n"
			"sraiw x26, x22, 1\n"
			"la x19, rand_ieee754_32\n"
			"flw.ps f6, 384(x19)\n"
			"flw.ps f30, 416(x19)\n"
			"fadd.s f13, f6, f30, rdn\n" 
			"fsrai.pi f9, f4, 0xf\n"
			"fadd.s f28, f22, f29, rup\n"
			"bne x26, x0, 1f\n"
			"1:\n"
			"fadd.s f27, f6, f20, rmm\n"
			"fadd.s f8, f28, f29, rne\n"
			"mulh x25, x19, x27\n"
			"fmulhu.pi f31, f31, f21\n"
			"la x25, rand_ieee754_32\n"
			"flw.ps f22, 352(x25)\n"
			"flw.ps f19, 672(x25)\n"
			"fadd.s f31, f22, f19, rtz\n" 
			"li x5, 0xd14c\n"
			"csrw tensor_mask, x5\n"	
			"fmax.s f6, f23, f30\n"
			"fcmovm.ps f27, f20, f3\n"
			"fcvt.f16.ps f22, f27\n"
			"fmax.s f0, f27, f23\n"
			"fadd.s f21, f3, f6, rdn\n"
			"fcmovm.ps f31, f23, f6\n"
			"bge x0, x2, 1f\n"
			"1:\n"
			"auipc x5, 0x0\n" 
			"for.pi f23, f14, f21\n"
			"fadd.s f4, f3, f3, rmm\n"
			"fadd.s f10, f3, f3, rup\n"
			"fround.ps f18, f21, rup\n"
			"fadd.s f23, f21, f3, rtz\n"
			"fmin.pi f14, f21, f14\n"
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
			"frsq.ps f1, f30\n"
			"bgeu x15, x9, 1f\n"
			"1:\n"
			"auipc x15, 0x1e\n"
			"andi x25, x15, 0x9\n"
			"slt x20, x20, x17\n"
			"subw x6, x17, x22\n"
			"addw x12, x6, x12\n"
			"fcvt.f16.ps f30, f6\n"
			"beq x14, x27, 1f\n"
			"1:\n"
			"auipc x0, 0x10\n"
			"fadd.s f7, f2, f7, rtz\n"
			"sraiw x9, x17, 0\n"
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ITER_0_HID_0:\n"
			"li x7, ((0x1 << TENSOR_STORE_REG_STRIDE) | (0x4 << TENSOR_STORE_START_REG) | (0x1 << TENSOR_STORE_STORE_COLS) | (0x6 << TENSOR_STORE_AROWS) | (0x0 << TENSOR_STORE_COOP_STORE) | (0x0 << TENSOR_STORE_OPC_0) | (0x8284f4b00 << TENSOR_STORE_VADDR) | 0 )\n" 
			"li x31, (0x80 << 4)\n"
			"csrw tensor_store, x7\n"	
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ITER_1_HID_0:\n"
			"li x7, ((0x0 << TENSOR_STORE_REG_STRIDE) | (0x14 << TENSOR_STORE_START_REG) | (0x0 << TENSOR_STORE_STORE_COLS) | (0x0 << TENSOR_STORE_AROWS) | (0x0 << TENSOR_STORE_COOP_STORE) | (0x0 << TENSOR_STORE_OPC_0) | (0x8184f3f82 << TENSOR_STORE_VADDR) | 0 )\n" 
			"li x31, (0x80 << 4)\n"
			"csrw tensor_store, x7\n"	
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ITER_2_HID_0:\n"
			"li x7, ((0x3 << TENSOR_STORE_REG_STRIDE) | (0xa << TENSOR_STORE_START_REG) | (0x3 << TENSOR_STORE_STORE_COLS) | (0x2 << TENSOR_STORE_AROWS) | (0x0 << TENSOR_STORE_COOP_STORE) | (0x0 << TENSOR_STORE_OPC_0) | (0x82838d276 << TENSOR_STORE_VADDR) | 0 )\n" 
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
			"csrrci x0, mcache_control, 2\n"
			"csrwi tensor_wait, 0x6\n" 
			"fence\n"
			"mov.m.x m0, x0, 0xFF\n"
			"li x5, 0x40\n"
			"la x31, rand_ieee754_32\n"
			"add x31, x31, x5\n"
			"flw.ps f0, 0(x31)\n"
			"flw.ps f1, 0(x31)\n"
			"flw.ps f10, 7(x31)\n"
			"flw.ps f11, 9(x31)\n"
			"flw.ps f12, 12(x31)\n"
			"flw.ps f13, 16(x31)\n"
			"flw.ps f14, 20(x31)\n"
			"flw.ps f15, 21(x31)\n"
			"flw.ps f16, 18(x31)\n"
			"flw.ps f17, 19(x31)\n"
			"flw.ps f18, 26(x31)\n"
			"flw.ps f19, 2(x31)\n"
			"flw.ps f2, 14(x31)\n"
			"flw.ps f20, 49(x31)\n"
			"flw.ps f21, 44(x31)\n"
			"flw.ps f22, 32(x31)\n"
			"flw.ps f23, 50(x31)\n"
			"flw.ps f24, 58(x31)\n"
			"flw.ps f25, 35(x31)\n"
			"flw.ps f26, 50(x31)\n"
			"flw.ps f27, 43(x31)\n"
			"flw.ps f28, 22(x31)\n"
			"flw.ps f29, 86(x31)\n"
			"flw.ps f3, 45(x31)\n"
			"flw.ps f30, 26(x31)\n"
			"flw.ps f31, 54(x31)\n"
			"flw.ps f4, 97(x31)\n"
			"flw.ps f5, 48(x31)\n"
			"flw.ps f6, 32(x31)\n"
			"flw.ps f7, 40(x31)\n"
			"flw.ps f8, 111(x31)\n"
			"flw.ps f9, 38(x31)\n"
			"la x31, rand_int_32\n"
			"add x31, x31, x5\n"
			"lw x0, 0(x31)\n"
			"lw x10, 0(x31)\n"
			"lw x11, 0(x31)\n"
			"lw x12, 3(x31)\n"
			"lw x13, 2(x31)\n"
			"lw x15, 1(x31)\n"
			"lw x16, 16(x31)\n"
			"lw x17, 6(x31)\n"
			"lw x18, 23(x31)\n"
			"lw x19, 9(x31)\n"
			"lw x20, 13(x31)\n"
			"lw x22, 8(x31)\n"
			"lw x23, 39(x31)\n"
			"lw x25, 3(x31)\n"
			"lw x26, 41(x31)\n"
			"lw x27, 22(x31)\n"
			"lw x28, 56(x31)\n"
			"lw x5, 54(x31)\n"
			"lw x6, 4(x31)\n"
			"lw x7, 36(x31)\n"
			"lw x9, 15(x31)\n"
			"li x21, 0x3\n"
			"LBL_SEQID_1_LOOP_SEQ_HID_0:\n"
			"fmax.s f16, f15, f11\n"
			"fsra.pi f25, f9, f17\n"
			"fmax.s f6, f26, f6\n"
			"blt x2, x9, 1f\n"
			"1:\n"
			"srli x12, x12, 2\n"
			"blt x8, x11, 1f\n"
			"1:\n"
			"fmin.pi f31, f28, f6\n"
			"srli x7, x17, 4\n"
			"fmin.pi f3, f25, f24\n"
			"sraiw x5, x16, 0\n"
			"frsq.ps f19, f31\n"
			"fcvt.f16.ps f2, f2\n"
			"fmulhu.pi f22, f9, f2\n"
			"sraiw x23, x23, 5\n"
			"fsrai.pi f12, f1, 0x9\n"
			"fxor.pi f0, f0, f0\n"
			"srli x14, x0, 3\n"
			"srliw x11, x7, 3\n"
			"xori x0, x9, 0xb\n"
			"subw x5, x5, x7\n"
			"fslli.pi f2, f2, 0x1\n"
			"frsq.ps f10, f6\n"
			"bltu x24, x15, 1f\n"
			"1:\n"
			"subw x9, x6, x20\n"
			"fsatu8.pi f1, f22\n"
			"xori x16, x18, 0x2\n"
			"srli x7, x7, 0\n"
			"blt x6, x27, 1f\n"
			"1:\n"
			"fxor.pi f8, f28, f27\n"
			"fxor.pi f6, f21, f6\n"
			"srliw x12, x26, 3\n"
			"fmin.pi f8, f18, f22\n"
			"fslli.pi f19, f30, 0xe\n"
			"bne x11, x9, 1f\n"
			"1:\n"
			"fsrai.pi f29, f25, 0x4\n"
			"fcvt.f16.ps f8, f30\n"
			"bltu x22, x4, 1f\n"
			"1:\n"
			"fround.ps f15, f20, rtz\n"
			"subw x27, x15, x27\n"
			"fround.ps f29, f21, rup\n"
			"subw x27, x27, x16\n"
			"fsatu8.pi f29, f31\n"
			"fmulhu.pi f28, f24, f21\n"
			"addw x7, x11, x28\n"
			"fmadd.ps f30, f23, f24, f30, rup\n"
			"fsrai.pi f22, f22, 0x6\n"
			"fcvt.f16.ps f31, f31\n"
			"addw x28, x18, x17\n"
			"fslli.pi f19, f23, 0x4\n"
			"beq x7, x22, 1f\n"
			"1:\n"
			"andi x27, x27, 0x6\n"
			"fsatu8.pi f3, f9\n"
			"bne x14, x7, 1f\n"
			"1:\n"
			"sraiw x15, x7, 2\n"
			"beq x18, x27, 1f\n"
			"1:\n"
			"fadd.s f31, f12, f5, rmm\n"
			"addw x6, x17, x15\n"
			"srli x11, x11, 0\n"
			"fsatu8.pi f24, f24\n"
			"subw x17, x10, x22\n"
			"blt x15, x21, 1f\n"
			"1:\n"
			"addw x9, x23, x5\n"
			"fmax.s f30, f2, f19\n"
			"bltu x11, x4, 1f\n"
			"1:\n"
			"fadd.s f9, f1, f6, rtz\n"
			"slt x20, x12, x20\n"
			"fround.ps f4, f19, rne\n"
			"fround.ps f17, f13, rup\n"
			"bltu x9, x19, 1f\n"
			"1:\n"
			"fsrai.pi f20, f29, 0x7\n"
			"frsq.ps f30, f20\n"
			"for.pi f24, f0, f14\n"
			"bne x7, x19, 1f\n"
			"1:\n"
			"fround.ps f10, f3, rup\n"
			"fmulhu.pi f19, f28, f27\n"
			"slt x20, x26, x20\n"
			"bge x8, x10, 1f\n"
			"1:\n"
			"fmulhu.pi f10, f6, f23\n"
			"fmin.pi f14, f16, f4\n"
			"srliw x9, x9, 0\n"
			"srli x25, x15, 2\n"
			"fadd.s f10, f10, f8, rdn\n"
			"andi x22, x17, 0x16\n"
			"beq x0, x10, 1f\n"
			"1:\n"
			"for.pi f18, f26, f24\n"
			"srli x10, x26, 1\n"
			"sraiw x19, x19, 5\n"
			"fslli.pi f9, f31, 0xf\n"
			"fsra.pi f8, f1, f1\n"
			"bne x18, x13, 1f\n"
			"1:\n"
			"fmulhu.pi f19, f21, f25\n"
			"subw x25, x5, x13\n"
			"sraiw x22, x12, 3\n"
			"sraiw x15, x17, 5\n"
			"fsrai.pi f2, f23, 0xe\n"
			"sraiw x5, x0, 5\n"
			"fmadd.ps f17, f17, f16, f18, rtz\n"
			"fround.ps f15, f15, rdn\n"
			"fcvt.f16.ps f16, f16\n"
			"sraiw x26, x17, 5\n"
			"for.pi f26, f20, f28\n"
			"fsatu8.pi f11, f9\n"
			"fsrai.pi f3, f15, 0x5\n"
			"srli x19, x19, 1\n"
			"fmin.pi f8, f8, f2\n"
			"xori x28, x28, 0x12\n"
			"fround.ps f18, f19, rmm\n"
			"fsra.pi f29, f18, f26\n"
			"andi x0, x15, 0x7\n"
			"fslli.pi f3, f23, 0xc\n"
			"fmulhu.pi f25, f10, f25\n"
			"fslli.pi f22, f22, 0xf\n"
			"fadd.s f30, f30, f5, rne\n"
			"fmadd.ps f10, f11, f10, f15, rne\n"
			"srliw x0, x26, 0\n"
			"bgeu x17, x17, 1f\n"
			"1:\n"
			"addw x9, x9, x6\n"
			"fadd.s f29, f29, f14, rne\n"
			"fround.ps f15, f7, rtz\n"
			"mulh x9, x9, x26\n"
			"fcmovm.ps f19, f30, f7\n"
			"fslli.pi f13, f22, 0x7\n"
			"fxor.pi f21, f3, f23\n"
			"fadd.s f7, f13, f26, rtz\n"
			"srli x14, x10, 3\n"
			"fmin.pi f6, f23, f26\n"
			"fsrai.pi f19, f1, 0xf\n"
			"bne x6, x31, 1f\n"
			"1:\n"
			"mulh x10, x28, x10\n"
			"fmin.pi f19, f7, f18\n"
			"xori x20, x11, 0x7\n"
			"sraiw x18, x17, 0\n"
			"slt x26, x11, x23\n"
			"addw x11, x11, x12\n"
			"srli x14, x25, 0\n"
			"frsq.ps f11, f17\n"
			"xori x28, x5, 0x16\n"
			"fslli.pi f22, f22, 0x8\n"
			"bge x15, x26, 1f\n"
			"1:\n"
			"andi x15, x25, 0x12\n"
			"beq x5, x11, 1f\n"
			"1:\n"
			"fsatu8.pi f10, f21\n"
			"fsrai.pi f8, f9, 0x2\n"
			"frsq.ps f10, f21\n"
			"bne x1, x26, 1f\n"
			"1:\n"
			"addw x11, x27, x23\n"
			"fmax.s f12, f12, f27\n"
			"fslli.pi f16, f0, 0x6\n"
			"srliw x28, x5, 2\n"
			"xori x26, x13, 0x13\n"
			"fmulhu.pi f25, f25, f20\n"
			"addw x27, x17, x27\n"
			"srliw x20, x25, 5\n"
			"andi x12, x17, 0x18\n"
			"srli x26, x18, 4\n"
			"fmax.s f4, f8, f8\n"
			"fmadd.ps f16, f16, f9, f20, rne\n"
			"fmax.s f20, f7, f20\n"
			"for.pi f3, f5, f13\n"
			"frsq.ps f23, f27\n"
			"fadd.s f31, f31, f19, rne\n"
			"fround.ps f6, f23, rdn\n"
			"bne x0, x5, 1f\n"
			"1:\n"
			"addi x21, x21, -1\n"
			"bne x21, x0, LBL_SEQID_1_LOOP_SEQ_HID_0\n"
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_0\n"
			"addi x30, x30, 1\n"
			"la x26, rand_ieee754_32\n"
			"flw.ps f25, 800(x26)\n"
			"flw.ps f21, 704(x26)\n"
			"flw.ps f12, 352(x26)\n"
			"fmadd.ps f12, f25, f21, f12, rdn\n" 
			"fcvt.f16.ps f22, f10\n"
			"ebreak\n"
			"mova.x.m x16\n"
			"mova.x.m x12\n"
			"subw x23, x13, x12\n"
			"fsgnjx.ps f22, f10, f29\n"
			"slt x0, x22, x13\n"
			"frsq.ps f19, f11\n"
			"beq x24, x25, 1f\n"
			"1:\n"
			"fmax.s f18, f29, f29\n"
			"beq x8, x10, 1f\n"
			"1:\n"
			"fadd.s f19, f21, f12, rdn\n"
			"bgeu x7, x24, 1f\n"
			"1:\n"
			"la x14, rand_int_32\n"
			"flw.ps f24, 512(x14)\n"
			"flw.ps f13, 192(x14)\n"
			"fmin.pi f7, f24, f13\n" 
			"andi x15, x15, 0x4\n"
			"fcvt.f16.ps f1, f18\n"
			"fsgnjx.ps f19, f13, f19\n"
			"fmin.pi f6, f25, f6\n"
			"fcvt.f16.ps f18, f1\n"
			"fcvt.ps.pw f5, f5, rne\n"
			"andi x14, x20, 0x5\n"
			"bne x22, x17, 1f\n"
			"1:\n"
			"andi x18, x13, 0x4\n"
			"fslli.pi f8, f22, 0x8\n"
			"fsgnjx.ps f8, f7, f15\n"
			"la x19, rand_int_32\n"
			"flw.ps f21, 608(x19)\n"
			"flw.ps f18, 288(x19)\n"
			"fsra.pi f1, f21, f18\n" 
			"fadd.s f7, f7, f13, rmm\n"
			"fsrai.pi f24, f4, 0xf\n"
			"bge x29, x25, 1f\n"
			"1:\n"
			"fnmadd.s f27, f4, f7, f26, rmm\n"
			"fmax.s f26, f6, f19\n"
			"fmin.pi f29, f6, f6\n"
			"fsgnjx.ps f26, f19, f26\n"
			"fmadd.ps f24, f7, f4, f4, rtz\n"
			"fmin.pi f6, f18, f18\n"
			"bge x0, x31, 1f\n"
			"1:\n"
			"fsatu8.pi f24, f18\n"
			"fmadd.ps f18, f4, f26, f18, rtz\n"
			"fbci.pi f19, 0x65169\n" 
			"frsq.ps f18, f22\n"
			"frsq.ps f8, f8\n"
			"frsq.ps f16, f7\n"
			"fslli.pi f31, f9, 0x5\n"
			"fcmovm.ps f31, f7, f18\n"
			"for.pi f4, f9, f9\n"
			"fcmovm.ps f9, f8, f8\n"
			"fsra.pi f24, f23, f24\n"
			"blt x18, x10, 1f\n"
			"1:\n"
			"fcmovm.ps f24, f22, f7\n"
			"fcvt.ps.f16 f6, f18\n"
			"li x14, 0x92d548041450bfa6\n"
			"andi x27, x14, 0xd\n" 
			"fxor.pi f23, f1, f19\n"
			"fmin.pi f2, f2, f2\n"
			"fsra.pi f11, f19, f11\n"
			"maskxor m1, m4, m0\n"
			"srlw x12, x6, x23\n"
			"bltu x18, x20, 1f\n"
			"1:\n"
			"fmadd.ps f12, f27, f8, f23, rup\n"
			"beq x17, x29, 1f\n"
			"1:\n"
			"mova.x.m x16\n"
			"maskxor m4, m7, m5\n"
			"maskxor m6, m2, m2\n"
			"fbci.pi f7, 0x71128\n"
			"li x27, 0xaffdfa1471beb83f\n"
			"fcvt.s.w f18, x27, rne\n" 
			"fmulhu.pi f10, f13, f8\n"
			"fsgnjx.ps f6, f2, f13\n"
			"fcvt.ps.pw f10, f2, rdn\n"
			"fltm.pi m6, f8, f20\n"
			"auipc x6, 0x6\n"
			"mova.x.m x6\n"
			"fmin.pi f31, f0, f2\n"
			"masknot m5, m5\n"
			"andi x13, x6, 0x18\n"
			"fltm.pi m0, f20, f13\n"
			"la x22, rand_int_32\n"
			"flw.ps f30, 288(x22)\n"
			"fcvt.ps.pw f14, f30, rne\n" 
			"fsub.ps f6, f6, f9, rdn\n"
			"fsatu8.pi f6, f15\n"
			"mulh x12, x17, x17\n"
			"beq x26, x17, 1f\n"
			"1:\n"
			"ori x27, x17, 0x11\n"
			"srliw x17, x17, 5\n"
			"fxor.pi f18, f6, f6\n"
			"bge x19, x0, 1f\n"
			"1:\n"
			"sraiw x0, x16, 4\n"
			"srliw x9, x15, 1\n"
			"fcmovm.ps f30, f25, f25\n"
			"srliw x12, x12, 2\n"
			"bltu x1, x30, 1f\n"
			"1:\n"
			"la x19, rand_int_32\n"
			"flw.ps f18, 32(x19)\n"
			"flw.ps f29, 704(x19)\n"
			"for.pi f10, f18, f29\n" 
			"fsrai.pi f31, f18, 0x5\n"
			"masknot m0, m1\n"
			"mov.m.x m0, x0, 0xff\n"
			"fsrai.pi f9, f16, 0x4\n"
			"fsatu8.pi f7, f15\n"
			"fcmovm.ps f28, f21, f27\n"
			"fsin.ps f7, f30\n"
			"mova.x.m x0\n"
			"fbci.pi f6, 0xdc9b\n"
			"ori x7, x23, 0x12\n"
			"xori x7, x14, 0xc\n"
			"la x26, rand_int_32\n"
			"flw.ps f28, 256(x26)\n"
			"fsrai.pi f30, f28, 0x3\n" 
			"fslli.pi f3, f3, 0xf\n"
			"fsra.pi f26, f13, f25\n"
			"fsgnjx.ps f28, f25, f2\n"
			"fcvt.ps.f16 f10, f23\n"
			"bltu x20, x3, 1f\n"
			"1:\n"
			"masknot m0, m2\n"
			"mov.m.x m0, x0, 0xff\n"
			"sraiw x26, x7, 2\n"
			"ori x26, x6, 0x7\n"
			"fcvt.f16.ps f2, f25\n"
			"beq x4, x18, 1f\n"
			"1:\n"
			"fnmadd.s f13, f25, f24, f13, rup\n"
			"beq x24, x7, 1f\n"
			"1:\n"
			"fcmovm.ps f26, f3, f25\n"
			"LBL_SEQID_2_M0_WRITE_ID_5_HID_0:\n"
			"maskand m6, m3, m5\n" 
			"li x5, 0x803400D8\n"
			"ld x5, 0(x5)\n"
			"fslli.pi f0, f0, 0x5\n"
			"slt x22, x9, x22\n"
			"bge x25, x12, 1f\n"
			"1:\n"
			"fround.ps f21, f21, rtz\n"
			"frsq.ps f12, f4\n"
			"sraiw x9, x9, 1\n"
			"fcvt.ps.f16 f31, f9\n"
			"slt x0, x0, x0\n"
			"fxor.pi f24, f0, f4\n"
			"fmadd.ps f9, f23, f28, f21, rne\n"
			"frsq.ps f9, f0\n"
			"la x16, rand_ieee754_32\n"
			"flw.ps f30, 704(x16)\n"
			"flw.ps f9, 544(x16)\n"
			"flw.ps f20, 288(x16)\n"
			"fmadd.ps f17, f30, f9, f20, rup\n" 
			"li x5, 0x873d\n"
			"csrw tensor_mask, x5\n"	
			"masknot m0, m1\n"
			"bltu x20, x28, 1f\n"
			"1:\n"
			"sraiw x9, x15, 0\n"
			"bgeu x30, x28, 1f\n"
			"1:\n"
			"fxor.pi f9, f1, f28\n"
			"subw x5, x7, x10\n"
			"xori x6, x5, 0x1c\n"
			"fsgnjx.ps f25, f11, f9\n"
			"frsq.ps f1, f1\n"
			"blt x2, x12, 1f\n"
			"1:\n"
			"ebreak\n"
			"bgeu x25, x19, 1f\n"
			"1:\n"
			"fsrai.pi f25, f11, 0xd\n"
			"blt x30, x20, 1f\n"
			"1:\n"
			"fltm.pi m7, f1, f24\n"
			"beq x29, x29, 1f\n"
			"1:\n"
			"la x13, rand_ieee754_32\n"
			"flw.ps f31, 128(x13)\n"
			"fround.ps f15, f31, rmm\n" 
			"fcvt.f16.ps f20, f12\n"
			"slt x10, x27, x10\n"
			"blt x17, x3, 1f\n"
			"1:\n"
			"sraiw x14, x10, 2\n"
			"for.pi f27, f17, f12\n"
			"blt x13, x19, 1f\n"
			"1:\n"
			"ori x16, x10, 0x1e\n"
			"fsgnjx.ps f6, f6, f18\n"
			"subw x28, x27, x22\n"
			"for.pi f20, f6, f8\n"
			"fcmovm.ps f10, f12, f17\n"
			"blt x23, x12, 1f\n"
			"1:\n"
			"sraiw x27, x27, 2\n"
			"la x12, rand_int_32\n"
			"flw.ps f18, 480(x12)\n"
			"fslli.pi f12, f18, 0x7\n" 
			"auipc x17, 0x19\n"
			"fcvt.ps.pw f15, f6, rne\n"
			"fsrai.pi f18, f11, 0x4\n"
			"fmax.s f15, f10, f15\n"
			"fsatu8.pi f2, f9\n"
			"bgeu x1, x20, 1f\n"
			"1:\n"
			"fcvt.f16.ps f23, f17\n"
			"frsq.ps f22, f7\n"
			"fsub.ps f6, f24, f17, rdn\n"
			"maskand m3, m7, m0\n"
			"srliw x15, x13, 2\n"
			"li x26, 0xed5e9a3a539fc153\n"
			"li x15, 0x659b1388ddf1ddb7\n"
			"srlw x19, x26, x15\n" 
			"fcvt.f16.ps f31, f31\n"
			"fcvt.ps.pw f13, f22, rmm\n"
			"fnmadd.s f15, f25, f29, f5, rmm\n"
			"fmadd.ps f16, f27, f10, f4, rdn\n"
			"fsub.ps f24, f28, f24, rup\n"
			"fcvt.ps.pw f17, f6, rup\n"
			"fmadd.ps f21, f13, f30, f1, rtz\n"
			"fslli.pi f22, f21, 0xd\n"
			"fmadd.ps f14, f21, f15, f14, rne\n"
			"fcvt.ps.pw f2, f10, rne\n"
			"beq x3, x24, 1f\n"
			"1:\n"
			"li x22, 0x3bcedf1b313b2e1b\n"
			"srli x14, x22, 2\n" 
			"fsrai.pi f26, f7, 0xa\n"
			"srliw x19, x20, 1\n"
			"blt x22, x1, 1f\n"
			"1:\n"
			"fsin.ps f29, f15\n"
			"fnmadd.s f31, f22, f25, f25, rtz\n"
			"fmulhu.pi f13, f21, f25\n"
			"fnmadd.s f4, f0, f4, f20, rne\n"
			"fsra.pi f3, f3, f0\n"
			"xori x10, x10, 0x11\n"
			"fnmadd.s f14, f29, f18, f20, rtz\n"
			"srliw x14, x9, 0\n"
			"la x26, rand_ieee754_32\n"
			"flw.ps f19, 992(x26)\n"
			"frsq.ps f31, f19\n" 
			"frsq.ps f24, f14\n"
			"beq x12, x21, 1f\n"
			"1:\n"
			"fsgnjx.ps f16, f16, f18\n"
			"fsgnjx.ps f0, f16, f0\n"
			"fcvt.ps.f16 f27, f24\n"
			"fround.ps f24, f20, rmm\n"
			"fcvt.ps.f16 f14, f26\n"
			"blt x24, x30, 1f\n"
			"1:\n"
			"fsrai.pi f5, f24, 0xe\n"
			"fsgnjx.ps f16, f18, f16\n"
			"bne x28, x26, 1f\n"
			"1:\n"
			"fsgnjx.ps f7, f29, f24\n"
			"frsq.ps f16, f24\n"
			"la x10, rand_ieee754_32\n"
			"flw.ps f17, 832(x10)\n"
			"flw.ps f15, 160(x10)\n"
			"fsub.ps f16, f17, f15, rup\n" 
			"andi x25, x9, 0x2\n"
			"fcvt.s.w f21, x11, rup\n"
			"subw x14, x26, x5\n"
			"bge x10, x20, 1f\n"
			"1:\n"
			"mulw x7, x26, x17\n"
			"fsatu8.pi f14, f2\n"
			"maskand m3, m3, m3\n"
			"andi x26, x11, 0x13\n"
			"fsub.ps f16, f27, f3, rup\n"
			"auipc x10, 0x5\n"
			"srliw x15, x9, 0\n"
			"bgeu x11, x9, 1f\n"
			"1:\n"
			"li x15, 0x621c94fca6db06f9\n"
			"li x10, 0x286d2f7fd0b433b0\n"
			"slt x28, x15, x10\n" 
			"subw x20, x20, x7\n"
			"srli x6, x27, 0\n"
			"fsrai.pi f12, f31, 0xe\n"
			"maskand m1, m1, m6\n"
			"auipc x14, 0x18\n"
			"fcvt.ps.f16 f5, f16\n"
			"fsrai.pi f29, f20, 0x7\n"
			"mulh x26, x18, x26\n"
			"xori x25, x15, 0x1b\n"
			"andi x9, x14, 0x5\n"
			"mova.x.m x26\n" 
			"fsub.ps f24, f26, f1, rmm\n"
			"fcvt.ps.f16 f23, f24\n"
			"fcvt.f16.ps f27, f24\n"
			"bltu x13, x27, 1f\n"
			"1:\n"
			"fsub.ps f4, f24, f24, rdn\n"
			"fslli.pi f18, f24, 0xc\n"
			"fsub.ps f18, f24, f26, rmm\n"
			"fround.ps f1, f1, rmm\n"
			"blt x8, x5, 1f\n"
			"1:\n"
			"fcvt.ps.f16 f24, f26\n"
			"fcvt.ps.f16 f4, f26\n"
			"blt x31, x30, 1f\n"
			"1:\n"
			"fcvt.f16.ps f27, f1\n"
			"la x25, rand_int_32\n"
			"flw.ps f30, 640(x25)\n"
			"flw.ps f9, 512(x25)\n"
			"fmin.pi f24, f30, f9\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"fmadd.ps f29, f24, f29, f18, rtz\n"
			"fsin.ps f16, f13\n"
			"fcvt.f16.ps f8, f1\n"
			"fadd.s f0, f24, f7, rne\n"
			"fmulhu.pi f5, f30, f22\n"
			"fsra.pi f16, f8, f10\n"
			"fsrai.pi f12, f8, 0xc\n"
			"fcvt.ps.f16 f28, f4\n"
			"bltu x11, x9, 1f\n"
			"1:\n"
			"fsra.pi f30, f9, f23\n"
			"beq x6, x15, 1f\n"
			"1:\n"
			"fcmovm.ps f16, f2, f24\n"
			"la x9, rand_ieee754_32\n"
			"flw.ps f26, 128(x9)\n"
			"fcvt.f16.ps f13, f26\n" 
			"li x5, 0x33b5\n"
			"csrw tensor_mask, x5\n"	
			"fadd.s f13, f13, f17, rmm\n"
			"blt x22, x8, 1f\n"
			"1:\n"
			"srli x22, x27, 3\n"
			"fsub.ps f23, f26, f15, rdn\n"
			"fround.ps f27, f3, rdn\n"
			"bgeu x2, x27, 1f\n"
			"1:\n"
			"fsub.ps f14, f21, f1, rup\n"
			"srli x19, x20, 2\n"
			"mulw x15, x20, x15\n"
			"fnmadd.s f21, f3, f17, f21, rmm\n"
			"fsub.ps f26, f2, f23, rne\n"
			"bne x29, x22, 1f\n"
			"1:\n"
			"mulw x17, x0, x13\n"
			"beq x1, x22, 1f\n"
			"1:\n"
			"auipc x22, 0x3\n" 
			"fnmadd.s f21, f4, f4, f12, rdn\n"
			"fnmadd.s f22, f4, f22, f6, rdn\n"
			"fsra.pi f4, f4, f12\n"
			"fsrai.pi f4, f4, 0x7\n"
			"bltu x17, x6, 1f\n"
			"1:\n"
			"fcvt.f16.ps f22, f12\n"
			"fsrai.pi f16, f22, 0x3\n"
			"blt x19, x2, 1f\n"
			"1:\n"
			"fcvt.f16.ps f3, f4\n"
			"fcvt.ps.f16 f12, f12\n"
			"fmulhu.pi f12, f14, f12\n"
			"fsin.ps f16, f12\n"
			"beq x23, x5, 1f\n"
			"1:\n"
			"LBL_SEQID_2_M0_WRITE_ID_6_HID_0:\n"
			"fltm.pi m2, f6, f3\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"masknot m1, m1\n"
			"bge x6, x8, 1f\n"
			"1:\n"
			"slt x23, x23, x16\n"
			"frsq.ps f21, f16\n"
			"andi x20, x18, 0x19\n"
			"fround.ps f26, f15, rdn\n"
			"fsrai.pi f26, f26, 0x4\n"
			"fround.ps f10, f30, rne\n"
			"fsin.ps f14, f17\n"
			"frsq.ps f17, f18\n"
			"maskand m1, m3, m7\n"
			"bne x31, x15, 1f\n"
			"1:\n"
			"li x16, 0xc160e9c8e0df6fbf\n"
			"xori x25, x16, 0x1f\n" 
			"fnmadd.s f24, f31, f24, f1, rdn\n"
			"bgeu x23, x0, 1f\n"
			"1:\n"
			"fsrai.pi f1, f1, 0x4\n"
			"fmulhu.pi f24, f24, f24\n"
			"ori x27, x14, 0x1c\n"
			"bne x19, x15, 1f\n"
			"1:\n"
			"andi x19, x19, 0x17\n"
			"fsgnjx.ps f17, f24, f31\n"
			"xori x27, x23, 0x16\n"
			"bge x11, x3, 1f\n"
			"1:\n"
			"fxor.pi f24, f1, f24\n"
			"fnmadd.s f24, f1, f1, f31, rne\n"
			"fcvt.ps.f16 f1, f1\n"
			"bne x23, x31, 1f\n"
			"1:\n"
			"la x25, rand_int_32\n"
			"flw.ps f16, 64(x25)\n"
			"fslli.pi f30, f16, 0x5\n" 
			"fsra.pi f13, f5, f27\n"
			"fxor.pi f13, f10, f5\n"
			"fsatu8.pi f18, f27\n"
			"for.pi f2, f27, f5\n"
			"fmadd.ps f30, f30, f5, f30, rtz\n"
			"fsatu8.pi f27, f27\n"
			"bltu x24, x22, 1f\n"
			"1:\n"
			"for.pi f13, f27, f5\n"
			"fmax.s f1, f27, f27\n"
			"fround.ps f30, f30, rne\n"
			"blt x16, x22, 1f\n"
			"1:\n"
			"fnmadd.s f27, f27, f27, f30, rtz\n"
			"li x18, 0x47dcd2f1226c0402\n"
			"li x19, 0xada06ac12985dd09\n"
			"subw x15, x18, x19\n" 
			"mulw x25, x27, x20\n"
			"srli x27, x16, 1\n"
			"fmax.s f19, f31, f4\n"
			"slt x22, x22, x11\n"
			"srliw x10, x27, 2\n"
			"fsatu8.pi f14, f5\n"
			"srliw x28, x27, 1\n"
			"fmulhu.pi f31, f5, f31\n"
			"fnmadd.s f28, f17, f7, f28, rne\n"
			"bge x26, x0, 1f\n"
			"1:\n"
			"addw x9, x28, x20\n"
			"la x25, rand_ieee754_32\n"
			"flw.ps f21, 256(x25)\n"
			"fmvs.x.ps x25, f21, 2\n" 
			"fsin.ps f25, f25\n"
			"fsub.ps f23, f25, f23, rne\n"
			"for.pi f17, f17, f12\n"
			"fnmadd.s f25, f28, f23, f25, rmm\n"
			"bgeu x1, x15, 1f\n"
			"1:\n"
			"fsrai.pi f22, f28, 0x6\n"
			"frsq.ps f22, f12\n"
			"fround.ps f14, f23, rtz\n"
			"fadd.s f12, f12, f25, rmm\n"
			"bge x12, x16, 1f\n"
			"1:\n"
			"fsgnjx.ps f14, f23, f28\n"
			"fsub.ps f25, f29, f25, rmm\n"
			"bne x6, x28, 1f\n"
			"1:\n"
			"la x18, rand_ieee754_32\n"
			"flw.ps f0, 864(x18)\n"
			"flw.ps f3, 928(x18)\n"
			"fmax.s f19, f0, f3\n" 
			"fmin.pi f20, f18, f3\n"
			"blt x1, x31, 1f\n"
			"1:\n"
			"fmin.pi f10, f28, f17\n"
			"fsra.pi f0, f18, f12\n"
			"fsrai.pi f5, f5, 0xa\n"
			"fmadd.ps f12, f8, f28, f21, rne\n"
			"fmin.pi f28, f28, f12\n"
			"fmadd.ps f8, f3, f5, f13, rup\n"
			"fsra.pi f6, f24, f5\n"
			"fsrai.pi f29, f29, 0xe\n"
			"fmin.pi f9, f20, f10\n"
			"beq x18, x9, 1f\n"
			"1:\n"
			"la x26, rand_ieee754_32\n"
			"flw.ps f16, 672(x26)\n"
			"fcvt.f16.ps f30, f16\n" 
			"subw x26, x13, x10\n"
			"mulh x6, x0, x16\n"
			"fsin.ps f23, f15\n"
			"slt x6, x27, x13\n"
			"bgeu x29, x13, 1f\n"
			"1:\n"
			"sraiw x7, x13, 1\n"
			"fsatu8.pi f15, f11\n"
			"frsq.ps f11, f8\n"
			"fcvt.ps.f16 f26, f5\n"
			"fadd.s f17, f17, f11, rmm\n"
			"subw x23, x22, x27\n"
			"li x5, 0x6df0f8f8378d357\n"
			"srliw x23, x5, 3\n" 
			"fmin.pi f19, f21, f9\n"
			"fsgnjx.ps f28, f0, f10\n"
			"for.pi f22, f12, f22\n"
			"fround.ps f25, f14, rdn\n"
			"fsgnjx.ps f24, f11, f3\n"
			"blt x2, x5, 1f\n"
			"1:\n"
			"fmadd.ps f29, f22, f22, f1, rdn\n"
			"fsgnjx.ps f22, f28, f4\n"
			"fsgnjx.ps f27, f9, f27\n"
			"fsgnjx.ps f5, f20, f5\n"
			"fsgnjx.ps f16, f16, f31\n"
			"bgeu x14, x21, 1f\n"
			"1:\n"
			"la x13, rand_ieee754_32\n"
			"flw.ps f8, 896(x13)\n"
			"fsin.ps f11, f8\n" 
			"mulh x26, x19, x26\n"
			"fcvt.ps.f16 f2, f15\n"
			"bge x21, x27, 1f\n"
			"1:\n"
			"subw x7, x9, x6\n"
			"fsin.ps f24, f24\n"
			"fsra.pi f5, f30, f5\n"
			"fsin.ps f9, f15\n"
			"beq x21, x17, 1f\n"
			"1:\n"
			"mulh x26, x11, x11\n"
			"fsin.ps f5, f15\n"
			"fround.ps f5, f5, rne\n"
			"fsin.ps f15, f24\n"
			"li x19, 0x3643a0f420700c05\n"
			"srli x18, x19, 0\n" 
			"fslli.pi f21, f29, 0x2\n"
			"xori x0, x5, 0x17\n"
			"addw x19, x17, x6\n"
			"bne x31, x13, 1f\n"
			"1:\n"
			"fcvt.f16.ps f29, f29\n"
			"beq x20, x31, 1f\n"
			"1:\n"
			"fsatu8.pi f8, f0\n"
			"beq x28, x12, 1f\n"
			"1:\n"
			"srli x14, x17, 3\n"
			"fsin.ps f14, f0\n"
			"beq x13, x11, 1f\n"
			"1:\n"
			"fmax.s f0, f0, f15\n"
			"srliw x0, x11, 0\n"
			"fslli.pi f24, f0, 0x7\n"
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
			"LBL_SEQID_2_XMA_QNT_STR_SEQ_ID_571013_HID_0:\n"
			"li x5, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x2 << TENSOR_IMA_B_NUM_COLS) | (0x1 << TENSOR_IMA_A_NUM_ROWS) | (0xe << TENSOR_IMA_A_NUM_COLS) | (0x4 << TENSOR_IMA_OFFSET) | (0x0 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x75 << TENSOR_IMA_SCP_LOC_B) | (0xbd << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x0 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"LBL_SEQID_2_XMA_QNT_STR_SEQ_ID_644983_HID_0:\n"
			"li x5, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x0 << TENSOR_FMA16_B_NUM_COLS) | (0x0 << TENSOR_FMA16_A_NUM_ROWS) | (0x0 << TENSOR_FMA16_A_NUM_COLS) | (0xd << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0xc2 << TENSOR_FMA16_SCP_LOC_B) | (0xe << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x0 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"LBL_SEQID_2_XMA_QNT_STR_SEQ_ID_27186_HID_0:\n"
			"li x5, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x3 << TENSOR_FMA16_B_NUM_COLS) | (0x8 << TENSOR_FMA16_A_NUM_ROWS) | (0x3 << TENSOR_FMA16_A_NUM_COLS) | (0x2 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0xfc << TENSOR_FMA16_SCP_LOC_B) | (0x5b << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x0 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_0\n"
			"csrw tensor_error, x0\n"
			"frsq.ps f1, f25\n"
			"fslli.pi f1, f7, 0x1\n"
			"fcmovm.ps f17, f17, f7\n"
			"fnmadd.s f5, f5, f20, f6, rdn\n"
			"fmin.pi f5, f5, f21\n"
			"li x5, 0xcaff96c5d091bf60\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0xc2ac055415576ee1\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"LBL_SEQID_2_XMA_QNT_STR_SEQ_ITER_0_HID_0:\n"
			"li x7, ((0x1 << TENSOR_STORE_REG_STRIDE) | (0xb << TENSOR_STORE_START_REG) | (0x0 << TENSOR_STORE_STORE_COLS) | (0x4 << TENSOR_STORE_AROWS) | (0x0 << TENSOR_STORE_COOP_STORE) | (0x0 << TENSOR_STORE_OPC_0) | (0x82893ae16 << TENSOR_STORE_VADDR) | 0 )\n" 
			"li x31, (0x20 << 4)\n"
			"csrw tensor_store, x7\n"	
			"LBL_SEQID_2_XMA_QNT_STR_SEQ_ITER_1_HID_0:\n"
			"li x7, ((0x2 << TENSOR_STORE_REG_STRIDE) | (0xe << TENSOR_STORE_START_REG) | (0x1 << TENSOR_STORE_STORE_COLS) | (0x6 << TENSOR_STORE_AROWS) | (0x0 << TENSOR_STORE_COOP_STORE) | (0x0 << TENSOR_STORE_OPC_0) | (0x8000023 << TENSOR_STORE_VADDR) | 0 )\n" 
			"li x31, (0x80 << 4)\n"
			"csrw tensor_store, x7\n"	
			"LBL_SEQID_2_XMA_QNT_STR_SEQ_ITER_2_HID_0:\n"
			"li x7, ((0x2 << TENSOR_STORE_REG_STRIDE) | (0xf << TENSOR_STORE_START_REG) | (0x1 << TENSOR_STORE_STORE_COLS) | (0x5 << TENSOR_STORE_AROWS) | (0x0 << TENSOR_STORE_COOP_STORE) | (0x0 << TENSOR_STORE_OPC_0) | (0xbf800af << TENSOR_STORE_VADDR) | 0 )\n" 
			"li x31, (0x80 << 4)\n"
			"csrw tensor_store, x7\n"	
			"LBL_SEQID_2_XMA_QNT_STR_SEQ_ITER_3_HID_0:\n"
			"li x7, ((0x1 << TENSOR_STORE_REG_STRIDE) | (0x1e << TENSOR_STORE_START_REG) | (0x3 << TENSOR_STORE_STORE_COLS) | (0x6 << TENSOR_STORE_AROWS) | (0x0 << TENSOR_STORE_COOP_STORE) | (0x0 << TENSOR_STORE_OPC_0) | (0x80002fc << TENSOR_STORE_VADDR) | 0 )\n" 
			"li x31, (0x40 << 4)\n"
			"csrw tensor_store, x7\n"	
			"LBL_SEQID_2_XMA_QNT_STR_SEQ_ITER_4_HID_0:\n"
			"li x7, ((0x1 << TENSOR_STORE_REG_STRIDE) | (0x4 << TENSOR_STORE_START_REG) | (0x3 << TENSOR_STORE_STORE_COLS) | (0xe << TENSOR_STORE_AROWS) | (0x0 << TENSOR_STORE_COOP_STORE) | (0x0 << TENSOR_STORE_OPC_0) | (0x828d86382 << TENSOR_STORE_VADDR) | 0 )\n" 
			"li x31, (0xc0 << 4)\n"
			"csrw tensor_store, x7\n"	
			"LBL_SEQID_2_XMA_QNT_STR_SEQ_ITER_5_HID_0:\n"
			"li x7, ((0x3 << TENSOR_STORE_REG_STRIDE) | (0x13 << TENSOR_STORE_START_REG) | (0x0 << TENSOR_STORE_STORE_COLS) | (0xd << TENSOR_STORE_AROWS) | (0x0 << TENSOR_STORE_COOP_STORE) | (0x0 << TENSOR_STORE_OPC_0) | (0x8189af914 << TENSOR_STORE_VADDR) | 0 )\n" 
			"li x31, (0x10 << 4)\n"
			"csrw tensor_store, x7\n"	
			"LBL_SEQID_2_XMA_QNT_STR_SEQ_ITER_6_HID_0:\n"
			"li x7, ((0x1 << TENSOR_STORE_REG_STRIDE) | (0x9 << TENSOR_STORE_START_REG) | (0x1 << TENSOR_STORE_STORE_COLS) | (0xf << TENSOR_STORE_AROWS) | (0x0 << TENSOR_STORE_COOP_STORE) | (0x0 << TENSOR_STORE_OPC_0) | (0xbf802aa << TENSOR_STORE_VADDR) | 0 )\n" 
			"li x31, (0x80 << 4)\n"
			"csrw tensor_store, x7\n"	
			"LBL_SEQID_2_XMA_QNT_STR_SEQ_ITER_7_HID_0:\n"
			"li x7, ((0x3 << TENSOR_STORE_REG_STRIDE) | (0x9 << TENSOR_STORE_START_REG) | (0x0 << TENSOR_STORE_STORE_COLS) | (0xa << TENSOR_STORE_AROWS) | (0x0 << TENSOR_STORE_COOP_STORE) | (0x0 << TENSOR_STORE_OPC_0) | (0x829c1742e << TENSOR_STORE_VADDR) | 0 )\n" 
			"li x31, (0x20 << 4)\n"
			"csrw tensor_store, x7\n"	
			"LBL_SEQID_2_XMA_QNT_STR_SEQ_ITER_8_HID_0:\n"
			"li x7, ((0x0 << TENSOR_STORE_REG_STRIDE) | (0x1e << TENSOR_STORE_START_REG) | (0x1 << TENSOR_STORE_STORE_COLS) | (0xb << TENSOR_STORE_AROWS) | (0x0 << TENSOR_STORE_COOP_STORE) | (0x0 << TENSOR_STORE_OPC_0) | (0xbf8051c << TENSOR_STORE_VADDR) | 0 )\n" 
			"li x31, (0x40 << 4)\n"
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
			"li x5, 0x30\n"
			"la x31, rand_ieee754_32\n"
			"add x31, x31, x5\n"
			"flw.ps f11, 0(x31)\n"
			"flw.ps f2, 3(x31)\n"
			"flw.ps f20, 7(x31)\n"
			"flw.ps f22, 8(x31)\n"
			"flw.ps f24, 1(x31)\n"
			"flw.ps f27, 13(x31)\n"
			"flw.ps f4, 3(x31)\n"
			"flw.ps f5, 3(x31)\n"
			"flw.ps f6, 18(x31)\n"
			"la x31, rand_int_32\n"
			"add x31, x31, x5\n"
			"lw x14, 0(x31)\n"
			"lw x15, 4(x31)\n"
			"lw x20, 5(x31)\n"
			"lw x22, 7(x31)\n"
			"lw x5, 0(x31)\n"
			"lw x6, 7(x31)\n"
			"lw x7, 21(x31)\n"
			"lw x9, 21(x31)\n"
			"li x21, 0x2\n"
			"LBL_SEQID_2_LOOP_SEQ_HID_0:\n"
			"fmax.s f2, f2, f5\n"
			"fmin.pi f2, f2, f27\n"
			"fsatu8.pi f27, f6\n"
			"bltu x26, x14, 1f\n"
			"1:\n"
			"fxor.pi f15, f20, f20\n"
			"mulh x5, x9, x5\n"
			"fcvt.ps.pw f2, f2, rne\n"
			"fadd.s f3, f2, f24, rup\n"
			"fnmadd.s f31, f22, f20, f27, rup\n"
			"fnmadd.s f31, f20, f22, f11, rup\n"
			"maskxor m6, m7, m6\n"
			"bge x22, x14, 1f\n"
			"1:\n"
			"fltm.pi m0, f4, f20\n"
			"mov.m.x m0, x0, 0xff\n"
			"fslli.pi f16, f5, 0x8\n"
			"fadd.s f13, f20, f2, rne\n"
			"for.pi f31, f22, f5\n"
			"bge x8, x23, 1f\n"
			"1:\n"
			"srliw x0, x9, 4\n"
			"fmax.s f11, f11, f20\n"
			"fsin.ps f3, f20\n"
			"bne x4, x10, 1f\n"
			"1:\n"
			"fmadd.ps f16, f5, f6, f11, rtz\n"
			"fround.ps f13, f2, rup\n"
			"srliw x26, x5, 0\n"
			"fsub.ps f20, f20, f20, rdn\n"
			"xori x14, x14, 0x1a\n"
			"slt x14, x20, x14\n"
			"addw x27, x6, x9\n"
			"bltu x10, x12, 1f\n"
			"1:\n"
			"srliw x11, x5, 1\n"
			"srliw x12, x20, 2\n"
			"masknot m5, m3\n"
			"fsgnjx.ps f16, f6, f4\n"
			"masknot m3, m0\n"
			"mov.m.x m0, x0, 0xff\n"
			"andi x7, x9, 0x1e\n"
			"fcvt.f16.ps f16, f22\n"
			"fsrai.pi f3, f22, 0x0\n"
			"bge x24, x18, 1f\n"
			"1:\n"
			"fmvs.x.ps x10, f5, 1\n"
			"fnmadd.s f24, f6, f24, f27, rdn\n"
			"fcmovm.ps f31, f4, f24\n"
			"fmvs.x.ps x14, f24, 3\n"
			"fmadd.ps f15, f27, f20, f2, rup\n"
			"for.pi f24, f2, f24\n"
			"bgeu x2, x22, 1f\n"
			"1:\n"
			"sraiw x20, x20, 5\n"
			"fcvt.f16.ps f22, f22\n"
			"srli x15, x5, 1\n"
			"fround.ps f5, f11, rdn\n"
			"beq x6, x0, 1f\n"
			"1:\n"
			"auipc x7, 0x19\n"
			"blt x30, x19, 1f\n"
			"1:\n"
			"fmvs.x.ps x7, f27, 2\n"
			"bne x10, x15, 1f\n"
			"1:\n"
			"addw x27, x22, x20\n"
			"maskand m6, m3, m6\n"
			"fltm.pi m1, f6, f2\n"
			"addw x0, x15, x5\n"
			"fmvs.x.ps x10, f27, 6\n"
			"fmadd.ps f22, f6, f20, f22, rup\n"
			"fsub.ps f22, f22, f6, rmm\n"
			"fmax.s f24, f20, f24\n"
			"bgeu x2, x26, 1f\n"
			"1:\n"
			"fadd.s f5, f5, f6, rdn\n"
			"srli x14, x14, 3\n"
			"slt x18, x5, x7\n"
			"fnmadd.s f28, f24, f5, f6, rup\n"
			"for.pi f3, f6, f27\n"
			"fxor.pi f5, f11, f6\n"
			"fcvt.ps.pw f6, f6, rdn\n"
			"fbci.pi f4, 0x6a248\n"
			"maskand m2, m6, m2\n"
			"fmulhu.pi f2, f20, f2\n"
			"fcvt.s.w f22, x5, rne\n"
			"fcvt.ps.pw f3, f4, rdn\n"
			"subw x15, x15, x22\n"
			"fmin.pi f2, f4, f24\n"
			"fslli.pi f2, f2, 0xd\n"
			"mulw x10, x22, x6\n"
			"fmulhu.pi f4, f4, f20\n"
			"fltm.pi m3, f20, f5\n"
			"fsgnjx.ps f13, f20, f20\n"
			"ori x6, x6, 0x1b\n"
			"fsatu8.pi f22, f24\n"
			"fmadd.ps f28, f27, f6, f24, rne\n"
			"fxor.pi f5, f11, f5\n"
			"blt x27, x22, 1f\n"
			"1:\n"
			"srliw x0, x15, 3\n"
			"bltu x21, x1, 1f\n"
			"1:\n"
			"fround.ps f7, f5, rup\n"
			"fcvt.f16.ps f5, f11\n"
			"fsgnjx.ps f27, f24, f4\n"
			"fsrai.pi f24, f6, 0x5\n"
			"blt x4, x4, 1f\n"
			"1:\n"
			"andi x15, x5, 0x6\n"
			"xori x14, x14, 0x17\n"
			"ebreak\n"
			"bge x22, x0, 1f\n"
			"1:\n"
			"ori x7, x5, 0x7\n"
			"bgeu x1, x0, 1f\n"
			"1:\n"
			"fadd.s f15, f22, f20, rtz\n"
			"fsin.ps f20, f11\n"
			"fsatu8.pi f24, f2\n"
			"andi x14, x9, 0xd\n"
			"fsgnjx.ps f31, f2, f2\n"
			"auipc x14, 0x1a\n"
			"for.pi f31, f20, f20\n"
			"fnmadd.s f13, f20, f4, f5, rne\n"
			"fsra.pi f27, f24, f27\n"
			"andi x20, x9, 0x10\n"
			"blt x23, x11, 1f\n"
			"1:\n"
			"fslli.pi f28, f24, 0xc\n"
			"maskxor m2, m7, m2\n"
			"mova.x.m x6\n"
			"fcvt.ps.pw f20, f11, rne\n"
			"fsrai.pi f22, f5, 0x5\n"
			"fsin.ps f5, f22\n"
			"fxor.pi f6, f6, f20\n"
			"mulh x22, x22, x20\n"
			"maskand m5, m6, m3\n"
			"auipc x5, 0xa\n"
			"fcmovm.ps f2, f2, f24\n"
			"fmax.s f27, f27, f22\n"
			"slt x22, x22, x6\n"
			"for.pi f13, f6, f27\n"
			"beq x22, x16, 1f\n"
			"1:\n"
			"mulh x11, x14, x7\n"
			"fsub.ps f13, f20, f27, rdn\n"
			"fsgnjx.ps f5, f5, f11\n"
			"for.pi f31, f11, f20\n"
			"fsatu8.pi f16, f24\n"
			"addi x21, x21, -1\n"
			"bne x21, x0, LBL_SEQID_2_LOOP_SEQ_HID_0\n"
			"li x5, 3\n" 
			"bne x30, x5, LBL_FAIL_HID_0\n"
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f12,  512 (x5)\n"
			"flw.ps f28,  192 (x5)\n"
			"flw.ps f6,  288 (x5)\n"
			"flw.ps f31,  352 (x5)\n"
			"flw.ps f13,  640 (x5)\n"
			"flw.ps f20,  704 (x5)\n"
			"flw.ps f8,  448 (x5)\n"
			"flw.ps f11,  320 (x5)\n"
			"flw.ps f26,  416 (x5)\n"
			"flw.ps f18,  0 (x5)\n"
			"flw.ps f0,  64 (x5)\n"
			"flw.ps f9,  128 (x5)\n"
			"flw.ps f22,  576 (x5)\n"
			"flw.ps f23,  608 (x5)\n"
			"flw.ps f19,  480 (x5)\n"
			"flw.ps f10,  32 (x5)\n"
			"flw.ps f3,  960 (x5)\n"
			"flw.ps f4,  256 (x5)\n"
			"flw.ps f30,  672 (x5)\n"
			"flw.ps f1,  224 (x5)\n"
			"flw.ps f2,  992 (x5)\n"
			"flw.ps f24,  96 (x5)\n"
			"flw.ps f14,  384 (x5)\n"
			"flw.ps f16,  832 (x5)\n"
			"flw.ps f15,  544 (x5)\n"
			"flw.ps f29,  928 (x5)\n"
			"flw.ps f25,  768 (x5)\n"
			"flw.ps f5,  800 (x5)\n"
			"flw.ps f17,  160 (x5)\n"
			"flw.ps f27,  864 (x5)\n"
			"flw.ps f7,  736 (x5)\n"
			"flw.ps f21,  896 (x5)\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_3_M0_WRITE_ID_7_HID_0:\n"
			"maskand m2, m5, m7\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"fxor.pi f4, f25, f31\n"
			"blt x27, x29, 1f\n"
			"1:\n"
			"frsq.ps f2, f2\n"
			"li x10, 0xd8d812319a11407e\n"
			"srliw x6, x10, 3\n" 
			"fsra.pi f12, f7, f20\n"
			"bne x29, x7, 1f\n"
			"1:\n"
			"mulw x11, x14, x11\n"
			"bltu x5, x9, 1f\n"
			"1:\n"
			"mova.x.m x12\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"fsub.ps f26, f18, f26, rtz\n"
			"maskand m4, m2, m5\n"
			"bne x20, x12, 1f\n"
			"1:\n"
			"li x16, 0xd0290c676b0ec3d2\n"
			"sraiw x5, x16, 1\n" 
			"fsrai.pi f3, f21, 0xf\n"
			"for.pi f1, f21, f3\n"
			"la x7, rand_ieee754_32\n"
			"flw.ps f3, 544(x7)\n"
			"fsin.ps f7, f3\n" 
			"maskand m6, m5, m5\n"
			"mova.x.m x22\n"
			"la x25, rand_ieee754_32\n"
			"flw.ps f16, 640(x25)\n"
			"flw.ps f31, 96(x25)\n"
			"flw.ps f11, 544(x25)\n"
			"fnmadd.s f27, f16, f31, f11, rmm\n" 
			"fsra.pi f13, f18, f27\n"
			"srlw x20, x25, x12\n"
			"la x23, rand_int_32\n"
			"flw.ps f23, 608(x23)\n"
			"fsatu8.pi f29, f23\n" 
			"fmax.s f30, f18, f3\n"
			"fadd.s f6, f28, f7, rtz\n"
			"la x5, rand_ieee754_32\n"
			"flw.ps f14, 576(x5)\n"
			"flw.ps f6, 864(x5)\n"
			"flw.ps f21, 32(x5)\n"
			"fmadd.ps f10, f14, f6, f21, rdn\n" 
			"for.pi f22, f13, f31\n"
			"fround.ps f2, f2, rdn\n"
			"la x10, rand_ieee754_16\n"
			"flw.ps f9, 960(x10)\n"
			"fcvt.ps.f16 f11, f9\n" 
			"andi x10, x0, 0x2\n"
			"fcvt.ps.pw f2, f20, rne\n"
			"li x12, 0x70d2c4c320c87ac6\n"
			"srliw x11, x12, 1\n" 
			"ori x19, x20, 0x1b\n"
			"for.pi f30, f26, f25\n"
			"auipc x20, 0x1d\n" 
			"ebreak\n"
			"andi x19, x19, 0x15\n"
			"bltu x31, x26, 1f\n"
			"1:\n"
			"li x25, 0xf39b6eecc66d2470\n"
			"li x9, 0x4dc971babdaa8cbd\n"
			"addw x28, x25, x9\n" 
			"fsin.ps f28, f6\n"
			"fmadd.ps f14, f6, f22, f22, rne\n"
			"LBL_SEQID_3_M0_WRITE_ID_8_HID_0:\n"
			"maskxor m2, m5, m3\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"ebreak\n"
			"addw x13, x19, x12\n"
			"li x27, 0x40439a3e2f4f288b\n"
			"srliw x20, x27, 2\n" 
			"fsrai.pi f15, f15, 0x4\n"
			"bgeu x20, x14, 1f\n"
			"1:\n"
			"for.pi f15, f23, f14\n"
			"la x12, rand_ieee754_32\n"
			"flw.ps f0, 352(x12)\n"
			"fmv.x.s x20, f0\n" 
			"fsub.ps f4, f14, f13, rne\n"
			"fcvt.ps.pw f18, f2, rne\n"
			"beq x30, x19, 1f\n"
			"1:\n"
			"li x26, 0x2f577bde00b2bb47\n"
			"srli x11, x26, 3\n" 
			"fsrai.pi f19, f5, 0x0\n"
			"fmin.pi f14, f30, f30\n"
			"bne x13, x7, 1f\n"
			"1:\n"
			"auipc x11, 0x1f\n" 
			"fslli.pi f2, f2, 0xf\n"
			"fcvt.f16.ps f15, f17\n"
			"la x5, rand_int_32\n"
			"flw.ps f19, 928(x5)\n"
			"flw.ps f9, 320(x5)\n"
			"fsra.pi f3, f19, f9\n" 
			"fsra.pi f28, f17, f28\n"
			"slt x14, x11, x14\n"
			"li x5, 0xd67eb047f11db265\n"
			"xori x10, x5, 0x11\n" 
			"fcvt.s.w f23, x13, rup\n"
			"fsin.ps f21, f18\n"
			"blt x2, x7, 1f\n"
			"1:\n"
			"fbci.pi f11, 0x56854\n" 
			"srlw x18, x22, x27\n"
			"fxor.pi f19, f5, f2\n"
			"la x26, rand_ieee754_32\n"
			"flw.ps f26, 128(x26)\n"
			"flw.ps f13, 512(x26)\n"
			"fcmovm.ps f21, f26, f13\n" 
			"ori x18, x23, 0x1d\n"
			"fmvs.x.ps x11, f29, 0\n"
			"la x16, rand_int_32\n"
			"flw.ps f26, 160(x16)\n"
			"fcvt.ps.pw f16, f26, rne\n" 
			"fsub.ps f29, f19, f15, rdn\n"
			"fxor.pi f29, f31, f3\n"
			"li x5, 0xddcd57a19046a847\n"
			"li x10, 0xe900b41bf5487833\n"
			"srlw x25, x5, x10\n" 
			"fmv.x.s x11, f21\n"
			"mova.x.m x23\n"
			"la x5, rand_int_32\n"
			"flw.ps f27, 128(x5)\n"
			"fslli.pi f2, f27, 0xe\n" 
			"srli x10, x14, 4\n"
			"fsrai.pi f25, f7, 0xa\n"
			"la x15, rand_int_32\n"
			"flw.ps f11, 352(x15)\n"
			"flw.ps f20, 544(x15)\n"
			"fmulhu.pi f30, f11, f20\n" 
			"fslli.pi f26, f6, 0x5\n"
			"bne x19, x5, 1f\n"
			"1:\n"
			"frsq.ps f10, f25\n"
			"bge x12, x29, 1f\n"
			"1:\n"
			"li x22, 0x29a9de69540add9c\n"
			"xori x6, x22, 0x13\n" 
			"fcmovm.ps f6, f22, f25\n"
			"fcmovm.ps f18, f25, f0\n"
			"mova.x.m x25\n" 
			"fsub.ps f14, f14, f14, rup\n"
			"addw x17, x14, x14\n"
			"li x27, 0x315c138bf6fdcc9e\n"
			"srliw x6, x27, 4\n" 
			"mulw x20, x19, x20\n"
			"fmin.pi f17, f11, f11\n"
			"la x26, rand_int_32\n"
			"flw.ps f16, 896(x26)\n"
			"flw.ps f22, 864(x26)\n"
			"fsra.pi f31, f16, f22\n" 
			"addw x14, x16, x15\n"
			"fslli.pi f28, f20, 0x8\n"
			"LBL_SEQID_3_M0_WRITE_ID_9_HID_0:\n"
			"fltm.pi m4, f20, f13\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"fmin.pi f0, f27, f28\n"
			"frsq.ps f22, f22\n"
			"fbci.pi f28, 0x2ddb5\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"srliw x11, x26, 2\n"
			"bgeu x17, x1, 1f\n"
			"1:\n"
			"fmadd.ps f19, f17, f19, f17, rdn\n"
			"la x15, rand_int_32\n"
			"flw.ps f13, 864(x15)\n"
			"flw.ps f0, 672(x15)\n"
			"fmin.pi f24, f13, f0\n" 
			"fslli.pi f9, f21, 0x7\n"
			"bne x13, x16, 1f\n"
			"1:\n"
			"fcmovm.ps f13, f13, f21\n"
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
			"fcmovm.ps f7, f5, f3\n"
			"fround.ps f5, f24, rne\n"
			"beq x25, x27, 1f\n"
			"1:\n"
			"fadd.s f5, f24, f3, rdn\n"
			"for.pi f24, f24, f3\n"
			"fslli.pi f5, f5, 0xf\n"
			"fmin.pi f3, f3, f5\n"
			"fxor.pi f3, f5, f3\n"
			"fmin.pi f5, f3, f24\n"
			"bgeu x31, x16, 1f\n"
			"1:\n"
			"fnmadd.s f3, f3, f3, f3, rdn\n"
			"fmulhu.pi f9, f5, f24\n"
			"LBL_SEQID_3_TLD_XMA_STR_SEQ_ID_641263_HID_0:\n"
			"li x5, ((0x0 << TENSOR_LOAD_T16_USE_TMASK) | (0x0 << TENSOR_LOAD_T16_USE_COOP) | (0x6 << TENSOR_LOAD_T16_OPC_110) | (0x3 << TENSOR_LOAD_T16_DST_START) | (0x209f78000 << TENSOR_LOAD_T16_VADDR) | (0x0 << TENSOR_LOAD_T16_OFFSET) | (0x8 << TENSOR_LOAD_T16_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_I16_USE_TMASK) | (0x0 << TENSOR_LOAD_I16_USE_COOP) | (0x2 << TENSOR_LOAD_I16_OPC_010) | (0x19 << TENSOR_LOAD_I16_DST_START) | (0x2099fcde0 << TENSOR_LOAD_I16_VADDR) | (0x0 << TENSOR_LOAD_I16_OFFSET) | (0x1 << TENSOR_LOAD_I16_NUM_LINES) | 0 )\n" 
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x0 << TENSOR_FMA16_B_NUM_COLS) | (0x8 << TENSOR_FMA16_A_NUM_ROWS) | (0x1 << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x19 << TENSOR_FMA16_SCP_LOC_B) | (0x3 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x17 << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0x8 << TENSOR_QUANT_QUANT_ROW) | (0x2d << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x0 << TENSOR_QUANT_TRANSF1) | (0x2 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x10, ((0x0 << TENSOR_STORE_REG_STRIDE) | (0x1c << TENSOR_STORE_START_REG) | (0x0 << TENSOR_STORE_STORE_COLS) | (0x9 << TENSOR_STORE_AROWS) | (0x0 << TENSOR_STORE_COOP_STORE) | (0x0 << TENSOR_STORE_OPC_0) | (0xbf807b2 << TENSOR_STORE_VADDR) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrw tensor_quant, x9\n"	
			"li x31, (0x80 << 4)\n"
			"csrw tensor_store, x10\n"	
			"csrwi tensor_wait, 7\n"
			"csrwi tensor_wait, 10\n"
			"csrwi tensor_wait, 8\n"
			"LBL_SEQID_3_TLD_XMA_STR_SEQ_ID_922757_HID_0:\n"
			"li x5, ((0x0 << TENSOR_LOAD_T8_USE_TMASK) | (0x0 << TENSOR_LOAD_T8_USE_COOP) | (0x5 << TENSOR_LOAD_T8_OPC_101) | (0x7 << TENSOR_LOAD_T8_DST_START) | (0x205c7a660 << TENSOR_LOAD_T8_VADDR) | (0x0 << TENSOR_LOAD_T8_OFFSET) | (0x1 << TENSOR_LOAD_T8_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_T32_USE_TMASK) | (0x0 << TENSOR_LOAD_T32_USE_COOP) | (0x7 << TENSOR_LOAD_T32_OPC_111) | (0x19 << TENSOR_LOAD_T32_DST_START) | (0x205ad1720 << TENSOR_LOAD_T32_VADDR) | (0x3 << TENSOR_LOAD_T32_NUM_LINES) | 0 )\n" 
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x2 << TENSOR_IMA_B_NUM_COLS) | (0x1 << TENSOR_IMA_A_NUM_ROWS) | (0x3 << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x0 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x19 << TENSOR_IMA_SCP_LOC_B) | (0x7 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x0 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x9, ((0x9 << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0x1 << TENSOR_QUANT_QUANT_ROW) | (0x2c << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x3 << TENSOR_QUANT_TRANSF7) | (0x5 << TENSOR_QUANT_TRANSF6) | (0x5 << TENSOR_QUANT_TRANSF5) | (0x4 << TENSOR_QUANT_TRANSF4) | (0x5 << TENSOR_QUANT_TRANSF3) | (0x3 << TENSOR_QUANT_TRANSF2) | (0x5 << TENSOR_QUANT_TRANSF1) | (0x4 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 7\n"
			"csrwi tensor_wait, 10\n"
			"LBL_SEQID_3_TLD_XMA_STR_SEQ_ID_176455_HID_0:\n"
			"li x5, ((0x0 << TENSOR_LOAD_I8_USE_TMASK) | (0x0 << TENSOR_LOAD_I8_USE_COOP) | (0x1 << TENSOR_LOAD_I8_OPC_001) | (0x2 << TENSOR_LOAD_I8_DST_START) | (0x205b61e3d << TENSOR_LOAD_I8_VADDR) | (0x0 << TENSOR_LOAD_I8_OFFSET) | (0x6 << TENSOR_LOAD_I8_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_T8_USE_TMASK) | (0x0 << TENSOR_LOAD_T8_USE_COOP) | (0x5 << TENSOR_LOAD_T8_OPC_101) | (0x18 << TENSOR_LOAD_T8_DST_START) | (0x205d1dcb2 << TENSOR_LOAD_T8_VADDR) | (0x0 << TENSOR_LOAD_T8_OFFSET) | (0xa << TENSOR_LOAD_T8_NUM_LINES) | 0 )\n" 
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x2 << TENSOR_IMA_B_NUM_COLS) | (0x6 << TENSOR_IMA_A_NUM_ROWS) | (0xa << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x18 << TENSOR_IMA_SCP_LOC_B) | (0x2 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x0 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x9, ((0xa << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0x6 << TENSOR_QUANT_QUANT_ROW) | (0x23 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x1 << TENSOR_QUANT_TRANSF1) | (0x5 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x10, ((0x0 << TENSOR_STORE_REG_STRIDE) | (0x11 << TENSOR_STORE_START_REG) | (0x1 << TENSOR_STORE_STORE_COLS) | (0x9 << TENSOR_STORE_AROWS) | (0x0 << TENSOR_STORE_COOP_STORE) | (0x0 << TENSOR_STORE_OPC_0) | (0xbf80361 << TENSOR_STORE_VADDR) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrw tensor_quant, x9\n"	
			"li x31, (0x40 << 4)\n"
			"csrw tensor_store, x10\n"	
			"csrwi tensor_wait, 7\n"
			"csrwi tensor_wait, 10\n"
			"csrwi tensor_wait, 8\n"
			"LBL_SEQID_3_TLD_XMA_STR_SEQ_ID_703619_HID_0:\n"
			"li x5, ((0x0 << TENSOR_LOAD_I8_USE_TMASK) | (0x0 << TENSOR_LOAD_I8_USE_COOP) | (0x1 << TENSOR_LOAD_I8_OPC_001) | (0x4 << TENSOR_LOAD_I8_DST_START) | (0x205bbc9e0 << TENSOR_LOAD_I8_VADDR) | (0x0 << TENSOR_LOAD_I8_OFFSET) | (0xc << TENSOR_LOAD_I8_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_SETUP_B_USE_COOP) | (0x0 << TENSOR_LOAD_SETUP_B_OPC_000) | (0x1 << TENSOR_LOAD_SETUP_B_OPC_1) | (0x20000ed << TENSOR_LOAD_SETUP_B_VADDR) | (0xa << TENSOR_LOAD_SETUP_B_NUM_LINES) | 0 )\n" 
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x1 << TENSOR_FMA32_B_NUM_COLS) | (0xc << TENSOR_FMA32_A_NUM_ROWS) | (0xa << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x1 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1d << TENSOR_FMA32_SCP_LOC_B) | (0x4 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x0 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x15 << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0xc << TENSOR_QUANT_QUANT_ROW) | (0xd << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x2 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 7\n"
			"csrwi tensor_wait, 10\n"
			"LBL_SEQID_3_TLD_XMA_STR_SEQ_ID_300784_HID_0:\n"
			"la x31, self_check_8\n"
			"li x5, 0xe000000000000b\n"
			"add x5, x5, x31\n"
			"li x6, 0x1000000000000e\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x0 << TENSOR_IMA_B_NUM_COLS) | (0xb << TENSOR_IMA_A_NUM_ROWS) | (0xe << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x1 << TENSOR_IMA_TEN_B_IN_MEM) | (0x19 << TENSOR_IMA_SCP_LOC_B) | (0x7 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x9, ((0x16 << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0xb << TENSOR_QUANT_QUANT_ROW) | (0x2b << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x1 << TENSOR_QUANT_TRANSF4) | (0x5 << TENSOR_QUANT_TRANSF3) | (0x8 << TENSOR_QUANT_TRANSF2) | (0x4 << TENSOR_QUANT_TRANSF1) | (0x9 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x10, ((0x1 << TENSOR_STORE_SCP_ENTRY_STRIDE) | (0x2d << TENSOR_STORE_SCP_START_SCP_ENTRY) | (0x5 << TENSOR_STORE_SCP_AROWS) | (0x1 << TENSOR_STORE_SCP_OPC_1) | (0x200004a << TENSOR_STORE_SCP_VADDR) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fmvz.x.ps x31, f0, 0\n"
			"li x5, 0x3c\n"
			"bne x5, x31, LBL_FAIL_HID_0\n"
			"csrw tensor_quant, x9\n"	
			"li x31, (0x1 << 6)\n"
			"csrwi tensor_wait, 10\n"
			"csrw tensor_store, x10\n"	
			"csrwi tensor_wait, 8\n"
			"LBL_SEQID_3_TLD_XMA_STR_SEQ_ID_551937_HID_0:\n"
			"la x31, self_check_16\n"
			"li x5, 0xe0000000000009\n"
			"add x5, x5, x31\n"
			"li x6, 0x10000000000004\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x2 << TENSOR_FMA16_B_NUM_COLS) | (0x9 << TENSOR_FMA16_A_NUM_ROWS) | (0x4 << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x1 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1d << TENSOR_FMA16_SCP_LOC_B) | (0x7 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x10, ((0x2 << TENSOR_STORE_SCP_ENTRY_STRIDE) | (0x28 << TENSOR_STORE_SCP_START_SCP_ENTRY) | (0x9 << TENSOR_STORE_SCP_AROWS) | (0x1 << TENSOR_STORE_SCP_OPC_1) | (0x20a164a20 << TENSOR_STORE_SCP_VADDR) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0xa\n"
			"bne x5, x31, LBL_FAIL_HID_0\n"
			"li x31, (0x3 << 6)\n"
			"csrw tensor_store, x10\n"	
			"csrwi tensor_wait, 8\n"
			"LBL_SEQID_3_TLD_XMA_STR_SEQ_ID_393304_HID_0:\n"
			"li x5, ((0x0 << TENSOR_LOAD_USE_TMASK) | (0x0 << TENSOR_LOAD_USE_COOP) | (0x0 << TENSOR_LOAD_OPC_000) | (0x5 << TENSOR_LOAD_DST_START) | (0x0 << TENSOR_LOAD_OPC_0) | (0x2098def60 << TENSOR_LOAD_VADDR) | (0x8 << TENSOR_LOAD_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_SETUP_B_USE_COOP) | (0x0 << TENSOR_LOAD_SETUP_B_OPC_000) | (0x1 << TENSOR_LOAD_SETUP_B_OPC_1) | (0x205c3a220 << TENSOR_LOAD_SETUP_B_VADDR) | (0x6 << TENSOR_LOAD_SETUP_B_NUM_LINES) | 0 )\n" 
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x3 << TENSOR_FMA32_B_NUM_COLS) | (0x8 << TENSOR_FMA32_A_NUM_ROWS) | (0x6 << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x1 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1f << TENSOR_FMA32_SCP_LOC_B) | (0x5 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x0 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x9, ((0xf << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0x8 << TENSOR_QUANT_QUANT_ROW) | (0x39 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x6 << TENSOR_QUANT_TRANSF7) | (0x6 << TENSOR_QUANT_TRANSF6) | (0x7 << TENSOR_QUANT_TRANSF5) | (0x7 << TENSOR_QUANT_TRANSF4) | (0x6 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 7\n"
			"csrwi tensor_wait, 10\n"
			"LBL_SEQID_3_TLD_XMA_STR_SEQ_ID_623554_HID_0:\n"
			"li x5, ((0x0 << TENSOR_LOAD_I16_USE_TMASK) | (0x0 << TENSOR_LOAD_I16_USE_COOP) | (0x2 << TENSOR_LOAD_I16_OPC_010) | (0x2 << TENSOR_LOAD_I16_DST_START) | (0x2fe0096 << TENSOR_LOAD_I16_VADDR) | (0x0 << TENSOR_LOAD_I16_OFFSET) | (0x4 << TENSOR_LOAD_I16_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_USE_TMASK) | (0x0 << TENSOR_LOAD_USE_COOP) | (0x0 << TENSOR_LOAD_OPC_000) | (0x1b << TENSOR_LOAD_DST_START) | (0x0 << TENSOR_LOAD_OPC_0) | (0x2fe00c4 << TENSOR_LOAD_VADDR) | (0x3 << TENSOR_LOAD_NUM_LINES) | 0 )\n" 
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x2 << TENSOR_FMA32_B_NUM_COLS) | (0x4 << TENSOR_FMA32_A_NUM_ROWS) | (0x3 << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1b << TENSOR_FMA32_SCP_LOC_B) | (0x2 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x0 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x10, ((0x1 << TENSOR_STORE_REG_STRIDE) | (0x4 << TENSOR_STORE_START_REG) | (0x0 << TENSOR_STORE_STORE_COLS) | (0x1 << TENSOR_STORE_AROWS) | (0x0 << TENSOR_STORE_COOP_STORE) | (0x0 << TENSOR_STORE_OPC_0) | (0x80003ab << TENSOR_STORE_VADDR) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"li x31, (0x80 << 4)\n"
			"csrw tensor_store, x10\n"	
			"csrwi tensor_wait, 7\n"
			"csrwi tensor_wait, 8\n"
			"LBL_SEQID_3_TLD_XMA_STR_SEQ_ID_280359_HID_0:\n"
			"li x5, ((0x0 << TENSOR_LOAD_T32_USE_TMASK) | (0x0 << TENSOR_LOAD_T32_USE_COOP) | (0x7 << TENSOR_LOAD_T32_OPC_111) | (0x6 << TENSOR_LOAD_T32_DST_START) | (0x209bcdae0 << TENSOR_LOAD_T32_VADDR) | (0x9 << TENSOR_LOAD_T32_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_I16_USE_TMASK) | (0x0 << TENSOR_LOAD_I16_USE_COOP) | (0x2 << TENSOR_LOAD_I16_OPC_010) | (0x1f << TENSOR_LOAD_I16_DST_START) | (0x20000eb << TENSOR_LOAD_I16_VADDR) | (0x0 << TENSOR_LOAD_I16_OFFSET) | (0x3 << TENSOR_LOAD_I16_NUM_LINES) | 0 )\n" 
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x0 << TENSOR_FMA32_B_NUM_COLS) | (0x9 << TENSOR_FMA32_A_NUM_ROWS) | (0x3 << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1f << TENSOR_FMA32_SCP_LOC_B) | (0x6 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x0 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x10, ((0x0 << TENSOR_STORE_REG_STRIDE) | (0xf << TENSOR_STORE_START_REG) | (0x3 << TENSOR_STORE_STORE_COLS) | (0x5 << TENSOR_STORE_AROWS) | (0x0 << TENSOR_STORE_COOP_STORE) | (0x0 << TENSOR_STORE_OPC_0) | (0xbf80676 << TENSOR_STORE_VADDR) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"li x31, (0x40 << 4)\n"
			"csrw tensor_store, x10\n"	
			"csrwi tensor_wait, 7\n"
			"csrwi tensor_wait, 8\n"
			"LBL_SEQID_3_TLD_XMA_STR_SEQ_ID_557584_HID_0:\n"
			"li x5, ((0x0 << TENSOR_LOAD_T16_USE_TMASK) | (0x0 << TENSOR_LOAD_T16_USE_COOP) | (0x6 << TENSOR_LOAD_T16_OPC_110) | (0x4 << TENSOR_LOAD_T16_DST_START) | (0x209a004c7 << TENSOR_LOAD_T16_VADDR) | (0x0 << TENSOR_LOAD_T16_OFFSET) | (0xb << TENSOR_LOAD_T16_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_USE_TMASK) | (0x0 << TENSOR_LOAD_USE_COOP) | (0x0 << TENSOR_LOAD_OPC_000) | (0x18 << TENSOR_LOAD_DST_START) | (0x0 << TENSOR_LOAD_OPC_0) | (0x205c47ed9 << TENSOR_LOAD_VADDR) | (0xf << TENSOR_LOAD_NUM_LINES) | 0 )\n" 
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x0 << TENSOR_IMA_B_NUM_COLS) | (0xb << TENSOR_IMA_A_NUM_ROWS) | (0xf << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x18 << TENSOR_IMA_SCP_LOC_B) | (0x4 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x0 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x9, ((0x1c << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0xb << TENSOR_QUANT_QUANT_ROW) | (0x17 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0xa << TENSOR_QUANT_TRANSF3) | (0x9 << TENSOR_QUANT_TRANSF2) | (0x5 << TENSOR_QUANT_TRANSF1) | (0x5 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x10, ((0x2 << TENSOR_STORE_REG_STRIDE) | (0x1f << TENSOR_STORE_START_REG) | (0x3 << TENSOR_STORE_STORE_COLS) | (0xc << TENSOR_STORE_AROWS) | (0x0 << TENSOR_STORE_COOP_STORE) | (0x0 << TENSOR_STORE_OPC_0) | (0x8296f3283 << TENSOR_STORE_VADDR) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrw tensor_quant, x9\n"	
			"li x31, (0x80 << 4)\n"
			"csrw tensor_store, x10\n"	
			"csrwi tensor_wait, 7\n"
			"csrwi tensor_wait, 10\n"
			"csrwi tensor_wait, 8\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_0\n"
			"csrw tensor_error, x0\n"
			"fmvs.x.ps x9, f8, 6\n"
			"auipc x13, 0x15\n"
			"fcvt.f16.ps f14, f19\n"
			"fxor.pi f14, f12, f12\n"
			"mova.x.m x23\n"
			"masknot m2, m4\n"
			"mova.x.m x22\n"
			"auipc x13, 0xf\n"
			"fsgnjx.ps f13, f8, f20\n"
			"bgeu x13, x22, 1f\n"
			"1:\n"
			"fslli.pi f31, f20, 0x0\n"
			"LBL_SEQID_3_XMA_QNT_STR_SEQ_ITER_0_HID_0:\n"
			"li x7, ((0x2 << TENSOR_STORE_REG_STRIDE) | (0x1f << TENSOR_STORE_START_REG) | (0x1 << TENSOR_STORE_STORE_COLS) | (0x1 << TENSOR_STORE_AROWS) | (0x0 << TENSOR_STORE_COOP_STORE) | (0x0 << TENSOR_STORE_OPC_0) | (0x818ef2781 << TENSOR_STORE_VADDR) | 0 )\n" 
			"li x31, (0x40 << 4)\n"
			"csrw tensor_store, x7\n"	
			"LBL_SEQID_3_XMA_QNT_STR_SEQ_ITER_1_HID_0:\n"
			"li x7, ((0x3 << TENSOR_STORE_REG_STRIDE) | (0x1e << TENSOR_STORE_START_REG) | (0x3 << TENSOR_STORE_STORE_COLS) | (0xe << TENSOR_STORE_AROWS) | (0x0 << TENSOR_STORE_COOP_STORE) | (0x0 << TENSOR_STORE_OPC_0) | (0x8187fb002 << TENSOR_STORE_VADDR) | 0 )\n" 
			"li x31, (0x80 << 4)\n"
			"csrw tensor_store, x7\n"	
			"LBL_SEQID_3_XMA_QNT_STR_SEQ_ITER_2_HID_0:\n"
			"li x7, ((0x2 << TENSOR_STORE_REG_STRIDE) | (0x1f << TENSOR_STORE_START_REG) | (0x0 << TENSOR_STORE_STORE_COLS) | (0xd << TENSOR_STORE_AROWS) | (0x0 << TENSOR_STORE_COOP_STORE) | (0x0 << TENSOR_STORE_OPC_0) | (0xbf80657 << TENSOR_STORE_VADDR) | 0 )\n" 
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
			"csrrci x0, mcache_control, 2\n"
			"csrwi tensor_wait, 0x6\n" 
			"fence\n"
			"mov.m.x m0, x0, 0xFF\n"
			"li x5, 0x20\n"
			"la x31, rand_ieee754_32\n"
			"add x31, x31, x5\n"
			"flw.ps f0, 0(x31)\n"
			"flw.ps f11, 2(x31)\n"
			"flw.ps f12, 8(x31)\n"
			"flw.ps f20, 4(x31)\n"
			"flw.ps f22, 7(x31)\n"
			"flw.ps f29, 9(x31)\n"
			"flw.ps f30, 8(x31)\n"
			"la x31, rand_int_32\n"
			"add x31, x31, x5\n"
			"lw x13, 0(x31)\n"
			"lw x27, 3(x31)\n"
			"lw x6, 6(x31)\n"
			"li x21, 0x1\n"
			"LBL_SEQID_3_LOOP_SEQ_HID_0:\n"
			"sraiw x27, x27, 3\n"
			"fsatu8.pi f0, f29\n"
			"fmadd.ps f29, f20, f22, f22, rup\n"
			"fsra.pi f29, f20, f29\n"
			"blt x8, x30, 1f\n"
			"1:\n"
			"auipc x13, 0xe\n"
			"frsq.ps f0, f30\n"
			"auipc x27, 0x12\n"
			"bge x6, x9, 1f\n"
			"1:\n"
			"ebreak\n"
			"fsgnjx.ps f0, f12, f20\n"
			"subw x6, x13, x6\n"
			"fmadd.ps f29, f29, f11, f29, rup\n"
			"beq x24, x5, 1f\n"
			"1:\n"
			"fsrai.pi f8, f0, 0x0\n"
			"ebreak\n"
			"fcvt.ps.pw f0, f11, rdn\n"
			"mulh x6, x6, x13\n"
			"ebreak\n"
			"maskxor m6, m2, m6\n"
			"auipc x22, 0x1c\n"
			"subw x22, x6, x27\n"
			"srli x27, x27, 0\n"
			"fsatu8.pi f0, f30\n"
			"auipc x6, 0x0\n"
			"mulh x16, x6, x27\n"
			"bgeu x2, x22, 1f\n"
			"1:\n"
			"srlw x6, x13, x6\n"
			"auipc x22, 0x15\n"
			"fadd.s f29, f30, f0, rtz\n"
			"ebreak\n"
			"fltm.pi m6, f20, f29\n"
			"fnmadd.s f8, f22, f20, f20, rtz\n"
			"fround.ps f0, f20, rmm\n"
			"fsra.pi f0, f11, f29\n"
			"sraiw x16, x13, 4\n"
			"masknot m2, m6\n"
			"fadd.s f29, f29, f29, rdn\n"
			"fsra.pi f0, f30, f22\n"
			"addw x22, x27, x6\n"
			"xori x6, x6, 0x2\n"
			"fltm.pi m2, f29, f30\n"
			"blt x15, x23, 1f\n"
			"1:\n"
			"ori x16, x27, 0xd\n"
			"fmadd.ps f29, f30, f29, f11, rup\n"
			"maskand m6, m2, m6\n"
			"fmulhu.pi f20, f0, f20\n"
			"fmv.x.s x10, f11\n"
			"ebreak\n"
			"fadd.s f0, f12, f20, rmm\n"
			"beq x6, x29, 1f\n"
			"1:\n"
			"fcvt.s.w f29, x6, rdn\n"
			"blt x9, x8, 1f\n"
			"1:\n"
			"frsq.ps f0, f30\n"
			"srlw x22, x6, x13\n"
			"maskand m2, m2, m2\n"
			"fnmadd.s f22, f30, f22, f0, rtz\n"
			"beq x30, x24, 1f\n"
			"1:\n"
			"fbci.pi f29, 0xc9f2\n"
			"subw x13, x13, x13\n"
			"fltm.pi m6, f22, f11\n"
			"mulh x16, x6, x13\n"
			"fsrai.pi f8, f22, 0xb\n"
			"fcvt.ps.pw f11, f0, rmm\n"
			"sraiw x22, x27, 1\n"
			"fadd.s f29, f20, f12, rup\n"
			"frsq.ps f11, f20\n"
			"subw x27, x13, x27\n"
			"addw x27, x27, x6\n"
			"fcvt.f16.ps f12, f29\n"
			"bgeu x8, x19, 1f\n"
			"1:\n"
			"fxor.pi f12, f12, f20\n"
			"beq x22, x3, 1f\n"
			"1:\n"
			"sraiw x16, x13, 3\n"
			"srliw x22, x27, 3\n"
			"addw x10, x6, x27\n"
			"subw x22, x13, x13\n"
			"mova.x.m x22\n"
			"fsin.ps f29, f12\n"
			"bge x12, x11, 1f\n"
			"1:\n"
			"fcvt.ps.pw f12, f30, rup\n"
			"srlw x16, x27, x27\n"
			"andi x10, x6, 0xd\n"
			"slt x6, x13, x6\n"
			"beq x31, x9, 1f\n"
			"1:\n"
			"fsrai.pi f8, f20, 0xd\n"
			"frsq.ps f0, f12\n"
			"xori x27, x27, 0x6\n"
			"blt x29, x10, 1f\n"
			"1:\n"
			"frsq.ps f8, f12\n"
			"blt x2, x1, 1f\n"
			"1:\n"
			"fcvt.s.w f11, x27, rup\n"
			"srlw x16, x27, x13\n"
			"fcvt.f16.ps f29, f29\n"
			"fmv.x.s x10, f11\n"
			"fltm.pi m2, f11, f11\n"
			"fsub.ps f29, f29, f0, rne\n"
			"blt x17, x30, 1f\n"
			"1:\n"
			"fcvt.s.w f29, x6, rmm\n"
			"fsgnjx.ps f30, f30, f30\n"
			"fnmadd.s f29, f29, f22, f0, rtz\n"
			"mulw x10, x13, x13\n"
			"fround.ps f8, f11, rmm\n"
			"addw x16, x27, x13\n"
			"mulh x6, x6, x27\n"
			"fcmovm.ps f0, f12, f11\n"
			"mova.x.m x13\n"
			"frsq.ps f30, f0\n"
			"for.pi f8, f20, f0\n"
			"fmvs.x.ps x6, f0, 0\n"
			"fsrai.pi f8, f30, 0x8\n"
			"fltm.pi m6, f20, f11\n"
			"beq x26, x2, 1f\n"
			"1:\n"
			"fadd.s f0, f29, f30, rup\n"
			"fbci.pi f29, 0x57713\n"
			"bltu x29, x28, 1f\n"
			"1:\n"
			"ebreak\n"
			"mulw x6, x6, x13\n"
			"auipc x13, 0xa\n"
			"bge x20, x23, 1f\n"
			"1:\n"
			"masknot m2, m6\n"
			"blt x10, x0, 1f\n"
			"1:\n"
			"fbci.pi f29, 0x4d990\n"
			"fmadd.ps f0, f12, f20, f20, rdn\n"
			"fsub.ps f29, f12, f29, rmm\n"
			"ebreak\n"
			"fsatu8.pi f8, f20\n"
			"fadd.s f0, f0, f29, rdn\n"
			"bne x4, x9, 1f\n"
			"1:\n"
			"fmax.s f20, f0, f20\n"
			"fcvt.s.w f0, x27, rne\n"
			"mulh x13, x13, x13\n"
			"fslli.pi f29, f12, 0x2\n"
			"mova.x.m x10\n"
			"fltm.pi m2, f30, f0\n"
			"auipc x6, 0x5\n"
			"fcmovm.ps f29, f29, f22\n"
			"sraiw x6, x27, 0\n"
			"srlw x6, x13, x6\n"
			"fbci.pi f0, 0x45e6f\n"
			"fxor.pi f12, f20, f12\n"
			"fsin.ps f29, f0\n"
			"masknot m2, m6\n"
			"fsgnjx.ps f29, f20, f29\n"
			"bne x16, x21, 1f\n"
			"1:\n"
			"sraiw x13, x13, 1\n"
			"bltu x0, x25, 1f\n"
			"1:\n"
			"fmadd.ps f11, f22, f0, f11, rne\n"
			"ori x6, x13, 0xf\n"
			"fmin.pi f0, f30, f12\n"
			"fmvs.x.ps x27, f30, 6\n"
			"blt x26, x4, 1f\n"
			"1:\n"
			"fadd.s f11, f11, f22, rne\n"
			"srliw x6, x27, 3\n"
			"bne x27, x2, 1f\n"
			"1:\n"
			"fxor.pi f11, f22, f29\n"
			"frsq.ps f29, f11\n"
			"fslli.pi f20, f20, 0xf\n"
			"fxor.pi f12, f20, f22\n"
			"mulh x6, x6, x13\n"
			"fxor.pi f29, f0, f30\n"
			"fadd.s f22, f20, f22, rne\n"
			"masknot m2, m2\n"
			"frsq.ps f12, f12\n"
			"fmadd.ps f8, f22, f11, f20, rup\n"
			"fmadd.ps f29, f11, f11, f20, rmm\n"
			"bltu x6, x29, 1f\n"
			"1:\n"
			"fsgnjx.ps f0, f20, f29\n"
			"ori x6, x27, 0x1e\n"
			"fsrai.pi f8, f0, 0x4\n"
			"fslli.pi f11, f29, 0x7\n"
			"auipc x27, 0x1d\n"
			"bge x7, x16, 1f\n"
			"1:\n"
			"maskand m6, m2, m2\n"
			"bltu x28, x6, 1f\n"
			"1:\n"
			"fsin.ps f8, f30\n"
			"mulh x22, x13, x6\n"
			"maskand m2, m2, m2\n"
			"addw x6, x6, x6\n"
			"fsgnjx.ps f0, f22, f22\n"
			"subw x13, x13, x27\n"
			"frsq.ps f0, f30\n"
			"andi x16, x27, 0x1e\n"
			"fsin.ps f11, f12\n"
			"masknot m6, m6\n"
			"andi x10, x6, 0x2\n"
			"subw x27, x13, x27\n"
			"srli x6, x13, 5\n"
			"auipc x27, 0xe\n"
			"fsub.ps f29, f30, f30, rup\n"
			"bltu x27, x13, 1f\n"
			"1:\n"
			"addi x21, x21, -1\n"
			"bne x21, x0, LBL_SEQID_3_LOOP_SEQ_HID_0\n"
			"li x5, 4\n" 
			"bne x30, x5, LBL_FAIL_HID_0\n"
			"addi x30, x30, 1\n"
			"auipc x22, 0x16\n" 
			"for.pi f29, f16, f17\n"
			"fsub.ps f0, f2, f6, rne\n"
			"bgeu x30, x22, 1f\n"
			"1:\n"
			"fcmovm.ps f31, f8, f3\n"
			"fmulhu.pi f24, f29, f28\n"
			"fmadd.ps f25, f22, f29, f5, rtz\n"
			"beq x27, x10, 1f\n"
			"1:\n"
			"fnmadd.s f23, f9, f25, f22, rtz\n"
			"fslli.pi f26, f8, 0xa\n"
			"fmin.pi f7, f0, f29\n"
			"li x17, 0xac2debcdaff22e52\n"
			"sraiw x9, x17, 2\n" 
			"ori x16, x0, 0xd\n"
			"fmax.s f27, f6, f8\n"
			"maskand m7, m6, m2\n"
			"fcvt.f16.ps f1, f23\n"
			"fsra.pi f30, f24, f2\n"
			"mova.x.m x17\n"
			"ori x16, x7, 0xc\n"
			"beq x31, x6, 1f\n"
			"1:\n"
			"fround.ps f2, f2, rtz\n"
			"bne x2, x0, 1f\n"
			"1:\n"
			"la x13, rand_ieee754_32\n"
			"flw.ps f17, 992(x13)\n"
			"flw.ps f11, 160(x13)\n"
			"fcmovm.ps f9, f17, f11\n" 
			"fcvt.f16.ps f21, f28\n"
			"fcvt.f16.ps f12, f11\n"
			"fround.ps f3, f9, rne\n"
			"fround.ps f18, f28, rdn\n"
			"fround.ps f19, f9, rne\n"
			"addw x12, x25, x20\n"
			"andi x9, x11, 0x4\n"
			"fcvt.f16.ps f26, f10\n"
			"li x13, 0x957549ca196dfaad\n"
			"li x16, 0x5864618486a8a485\n"
			"srlw x27, x13, x16\n" 
			"mulh x17, x17, x11\n"
			"beq x18, x28, 1f\n"
			"1:\n"
			"ori x10, x10, 0x3\n"
			"fsub.ps f24, f13, f30, rup\n"
			"srlw x12, x20, x27\n"
			"fcvt.ps.pw f26, f1, rdn\n"
			"fcmovm.ps f31, f31, f1\n"
			"xori x11, x11, 0x17\n"
			"mulh x26, x7, x7\n"
			"beq x10, x2, 1f\n"
			"1:\n"
			"la x10, rand_ieee754_32\n"
			"flw.ps f16, 800(x10)\n"
			"flw.ps f8, 480(x10)\n"
			"fsgnjx.ps f2, f16, f8\n" 
			"fcvt.s.w f21, x25, rup\n"
			"addw x18, x25, x12\n"
			"fltm.pi m7, f3, f3\n"
			"fmv.x.s x12, f20\n"
			"masknot m6, m7\n"
			"bgeu x27, x17, 1f\n"
			"1:\n"
			"auipc x25, 0x15\n"
			"sraiw x28, x9, 5\n"
			"bltu x18, x5, 1f\n"
			"1:\n"
			"fround.ps f19, f20, rdn\n"
			"la x11, rand_int_32\n"
			"flw.ps f9, 704(x11)\n"
			"fslli.pi f17, f9, 0x6\n" 
			"srliw x20, x19, 5\n"
			"mulh x7, x6, x11\n"
			"fadd.s f18, f27, f21, rtz\n"
			"fsrai.pi f4, f10, 0xb\n"
			"fround.ps f13, f21, rne\n"
			"fsub.ps f0, f21, f0, rup\n"
			"fcvt.ps.pw f30, f13, rne\n"
			"subw x6, x7, x14\n"
			"la x26, rand_int_32\n"
			"flw.ps f3, 96(x26)\n"
			"fslli.pi f14, f3, 0x6\n" 
			"srliw x27, x20, 2\n"
			"fcmovm.ps f2, f2, f30\n"
			"fsrai.pi f10, f3, 0x8\n"
			"fbci.pi f30, 0x76a44\n"
			"fsub.ps f18, f6, f10, rne\n"
			"fslli.pi f21, f21, 0x3\n"
			"fmin.pi f24, f3, f28\n"
			"fltm.pi m2, f4, f5\n"
			"la x7, rand_int_32\n"
			"flw.ps f22, 192(x7)\n"
			"flw.ps f20, 864(x7)\n"
			"fmulhu.pi f18, f22, f20\n" 
			"frsq.ps f9, f21\n"
			"fround.ps f22, f22, rtz\n"
			"fsra.pi f22, f22, f16\n"
			"fround.ps f29, f3, rdn\n"
			"fxor.pi f15, f22, f16\n"
			"fmin.pi f10, f3, f16\n"
			"mulh x20, x20, x7\n"
			"fcmovm.ps f28, f20, f6\n"
			"la x17, rand_int_32\n"
			"flw.ps f29, 576(x17)\n"
			"flw.ps f25, 32(x17)\n"
			"fmin.pi f28, f29, f25\n" 
			"xori x23, x23, 0x1d\n"
			"fmvs.x.ps x12, f19, 0\n"
			"fcvt.f16.ps f6, f6\n"
			"fcmovm.ps f12, f3, f3\n"
			"fcmovm.ps f15, f4, f21\n"
			"fcvt.f16.ps f10, f15\n"
			"masknot m6, m6\n"
			"bne x30, x6, 1f\n"
			"1:\n"
			"fround.ps f24, f16, rdn\n"
			"li x17, 0xb0bf34d74d4d3194\n"
			"srli x15, x17, 2\n" 
			"fmadd.ps f0, f5, f10, f11, rmm\n"
			"fcmovm.ps f6, f6, f11\n"
			"bltu x25, x16, 1f\n"
			"1:\n"
			"fmadd.ps f30, f10, f6, f5, rmm\n"
			"fcmovm.ps f10, f25, f7\n"
			"bgeu x12, x8, 1f\n"
			"1:\n"
			"fadd.s f7, f25, f12, rmm\n"
			"fmadd.ps f30, f10, f6, f11, rup\n"
			"fxor.pi f12, f7, f12\n"
			"fsatu8.pi f6, f19\n"
			"la x25, rand_ieee754_32\n"
			"flw.ps f27, 64(x25)\n"
			"flw.ps f8, 0(x25)\n"
			"fmax.s f5, f27, f8\n" 
			"fslli.pi f0, f15, 0x8\n"
			"fsra.pi f30, f25, f29\n"
			"fslli.pi f5, f25, 0x4\n"
			"frsq.ps f28, f28\n"
			"fxor.pi f28, f25, f28\n"
			"fslli.pi f25, f28, 0x0\n"
			"fmax.s f5, f29, f25\n"
			"fsgnjx.ps f22, f15, f28\n"
			"li x5, 0x6a8908cb1c7d5d56\n"
			"li x14, 0x501b7251290dbbf8\n"
			"addw x5, x5, x14\n" 
			"fsin.ps f20, f21\n"
			"ori x11, x27, 0x1b\n"
			"fsra.pi f13, f7, f13\n"
			"fsin.ps f28, f28\n"
			"bltu x16, x31, 1f\n"
			"1:\n"
			"ebreak\n"
			"fround.ps f23, f23, rdn\n"
			"sraiw x28, x28, 1\n"
			"sraiw x26, x17, 5\n"
			"la x6, rand_ieee754_32\n"
			"flw.ps f18, 352(x6)\n"
			"flw.ps f7, 352(x6)\n"
			"fsgnjx.ps f4, f18, f7\n" 
			"fadd.s f28, f8, f11, rtz\n"
			"fsgnjx.ps f31, f8, f13\n"
			"srli x13, x5, 2\n"
			"fadd.s f13, f2, f2, rmm\n"
			"bge x27, x17, 1f\n"
			"1:\n"
			"sraiw x23, x10, 5\n"
			"sraiw x5, x7, 5\n"
			"fsub.ps f3, f13, f8, rmm\n"
			"fsin.ps f31, f2\n"
			"la x9, rand_ieee754_32\n"
			"flw.ps f29, 800(x9)\n"
			"fmvs.x.ps x7, f29, 6\n" 
			"fmax.s f21, f31, f23\n"
			"fcvt.f16.ps f21, f23\n"
			"beq x27, x8, 1f\n"
			"1:\n"
			"subw x10, x0, x25\n"
			"auipc x20, 0x1f\n"
			"fmvs.x.ps x10, f27, 7\n"
			"fslli.pi f31, f27, 0xa\n"
			"fmvs.x.ps x0, f31, 5\n"
			"fcvt.ps.f16 f21, f23\n"
			"la x25, rand_int_32\n"
			"flw.ps f18, 896(x25)\n"
			"fcvt.ps.pw f17, f18, rne\n" 
			"fcvt.ps.pw f17, f27, rup\n"
			"fxor.pi f11, f17, f11\n"
			"fcvt.ps.pw f2, f8, rdn\n"
			"for.pi f8, f0, f27\n"
			"fslli.pi f27, f5, 0xd\n"
			"beq x2, x15, 1f\n"
			"1:\n"
			"fxor.pi f0, f17, f11\n"
			"fmadd.ps f17, f0, f5, f17, rdn\n"
			"for.pi f0, f15, f11\n"
			"beq x23, x14, 1f\n"
			"1:\n"
			"li x20, 0x6988507678ebf8e4\n"
			"xori x14, x20, 0x9\n" 
			"sraiw x11, x6, 3\n"
			"for.pi f6, f21, f24\n"
			"masknot m2, m1\n"
			"maskxor m2, m0, m1\n"
			"mov.m.x m0, x0, 0xff\n"
			"fcvt.ps.pw f9, f0, rmm\n"
			"bltu x1, x7, 1f\n"
			"1:\n"
			"sraiw x6, x19, 4\n"
			"fmin.pi f0, f0, f21\n"
			"fmv.x.s x15, f0\n"
			"li x18, 0xb22f39ac2d0cb33d\n"
			"li x15, 0xb038ee45db356006\n"
			"slt x13, x18, x15\n" 
			"fcvt.ps.f16 f28, f8\n"
			"for.pi f16, f14, f8\n"
			"bgeu x17, x6, 1f\n"
			"1:\n"
			"fxor.pi f9, f28, f28\n"
			"slt x26, x26, x19\n"
			"bgeu x19, x2, 1f\n"
			"1:\n"
			"ebreak\n"
			"subw x9, x9, x26\n"
			"mulw x19, x19, x9\n"
			"slt x10, x17, x17\n"
			"bltu x3, x7, 1f\n"
			"1:\n"
			"la x12, rand_int_32\n"
			"flw.ps f23, 384(x12)\n"
			"flw.ps f6, 384(x12)\n"
			"fxor.pi f7, f23, f6\n" 
			"fmax.s f12, f19, f19\n"
			"fcmovm.ps f15, f12, f15\n"
			"fmax.s f14, f1, f15\n"
			"fsin.ps f30, f16\n"
			"fmin.pi f17, f1, f12\n"
			"fsrai.pi f14, f1, 0xf\n"
			"bne x9, x30, 1f\n"
			"1:\n"
			"fsin.ps f0, f15\n"
			"fcvt.ps.f16 f1, f19\n"
			"la x12, rand_ieee754_32\n"
			"flw.ps f22, 352(x12)\n"
			"flw.ps f23, 416(x12)\n"
			"fadd.s f3, f22, f23, rdn\n" 
			"fcmovm.ps f25, f16, f14\n"
			"fmvs.x.ps x25, f9, 6\n"
			"fmadd.ps f27, f9, f16, f14, rne\n"
			"bne x17, x9, 1f\n"
			"1:\n"
			"fcvt.ps.f16 f30, f14\n"
			"fround.ps f9, f14, rtz\n"
			"maskxor m4, m7, m1\n"
			"fround.ps f25, f16, rdn\n"
			"fmv.x.s x17, f16\n"
			"LBL_SEQID_4_M0_WRITE_ID_10_HID_0:\n"
			"maskand m4, m6, m0\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0xFF\n"
			"fmvs.x.ps x15, f1, 6\n"
			"for.pi f6, f6, f1\n"
			"maskand m2, m6, m2\n"
			"maskand m4, m4, m2\n"
			"bne x10, x15, 1f\n"
			"1:\n"
			"mulh x12, x6, x18\n"
			"mulw x17, x26, x18\n"
			"blt x12, x13, 1f\n"
			"1:\n"
			"frsq.ps f19, f16\n"
			"frsq.ps f15, f9\n"
			"auipc x12, 0x1d\n" 
			"frsq.ps f31, f6\n"
			"fround.ps f24, f14, rup\n"
			"fcvt.ps.pw f29, f4, rtz\n"
			"fcvt.ps.f16 f1, f6\n"
			"bge x11, x2, 1f\n"
			"1:\n"
			"fnmadd.s f2, f4, f4, f6, rdn\n"
			"bge x31, x14, 1f\n"
			"1:\n"
			"for.pi f29, f25, f25\n"
			"fsub.ps f14, f14, f25, rdn\n"
			"fcvt.ps.pw f31, f25, rdn\n"
			"li x11, 0x2edbed25f83b6bd8\n"
			"ori x13, x11, 0xa\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"fsra.pi f2, f10, f2\n"
			"fcvt.f16.ps f12, f24\n"
			"bne x31, x7, 1f\n"
			"1:\n"
			"fmulhu.pi f31, f1, f6\n"
			"for.pi f14, f26, f3\n"
			"srli x13, x13, 5\n"
			"for.pi f29, f6, f22\n"
			"fsgnjx.ps f15, f30, f9\n"
			"fxor.pi f6, f16, f16\n"
			"li x13, 0xbfe4946db0850eab\n"
			"sraiw x10, x13, 5\n" 
			"mulw x16, x16, x28\n"
			"sraiw x14, x25, 5\n"
			"srlw x15, x6, x0\n"
			"slt x7, x7, x16\n"
			"bltu x13, x27, 1f\n"
			"1:\n"
			"xori x7, x0, 0x6\n"
			"fxor.pi f3, f15, f1\n"
			"blt x12, x18, 1f\n"
			"1:\n"
			"fadd.s f20, f25, f25, rdn\n"
			"mulw x26, x5, x25\n"
			"LBL_SEQID_4_M0_WRITE_ID_11_HID_0:\n"
			"maskxor m2, m1, m1\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"srli x15, x6, 0\n"
			"fmadd.ps f12, f20, f28, f12, rmm\n"
			"srliw x25, x16, 4\n"
			"bltu x20, x0, 1f\n"
			"1:\n"
			"masknot m2, m2\n"
			"fbci.pi f30, 0x1810b\n"
			"bne x17, x11, 1f\n"
			"1:\n"
			"fsub.ps f18, f12, f26, rtz\n"
			"bltu x13, x4, 1f\n"
			"1:\n"
			"srliw x27, x15, 0\n"
			"fmvs.x.ps x22, f4, 7\n"
			"mova.x.m x15\n" 
			"fcvt.ps.pw f11, f27, rtz\n"
			"fsatu8.pi f27, f27\n"
			"andi x16, x18, 0x17\n"
			"bgeu x23, x18, 1f\n"
			"1:\n"
			"fsub.ps f11, f11, f23, rtz\n"
			"fsatu8.pi f23, f23\n"
			"bgeu x31, x29, 1f\n"
			"1:\n"
			"srlw x16, x28, x11\n"
			"blt x15, x9, 1f\n"
			"1:\n"
			"fcvt.ps.f16 f7, f6\n"
			"ori x15, x17, 0x17\n"
			"li x12, 0xe4231f1511e33f6e\n"
			"fcvt.s.w f28, x12, rne\n" 
			"fsub.ps f24, f10, f22, rup\n"
			"beq x15, x16, 1f\n"
			"1:\n"
			"fsatu8.pi f31, f13\n"
			"mulh x17, x25, x17\n"
			"bge x0, x31, 1f\n"
			"1:\n"
			"fmadd.ps f25, f5, f20, f5, rne\n"
			"bne x22, x16, 1f\n"
			"1:\n"
			"sraiw x7, x19, 0\n"
			"fsrai.pi f14, f24, 0x7\n"
			"fmadd.ps f6, f13, f21, f17, rne\n"
			"mulh x26, x0, x9\n"
			"bne x4, x31, 1f\n"
			"1:\n"
			"la x7, rand_ieee754_32\n"
			"flw.ps f0, 544(x7)\n"
			"flw.ps f29, 992(x7)\n"
			"fadd.s f9, f0, f29, rtz\n" 
			"fxor.pi f21, f14, f21\n"
			"ori x10, x23, 0x2\n"
			"mulw x9, x14, x14\n"
			"bgeu x15, x14, 1f\n"
			"1:\n"
			"slt x17, x0, x0\n"
			"fsgnjx.ps f14, f22, f14\n"
			"bge x5, x30, 1f\n"
			"1:\n"
			"fmin.pi f15, f28, f15\n"
			"fxor.pi f14, f28, f21\n"
			"subw x14, x14, x27\n"
			"la x12, rand_int_32\n"
			"flw.ps f20, 576(x12)\n"
			"flw.ps f4, 192(x12)\n"
			"fmin.pi f12, f20, f4\n" 
			"fsin.ps f27, f6\n"
			"frsq.ps f0, f21\n"
			"fnmadd.s f1, f26, f21, f25, rup\n"
			"fslli.pi f21, f26, 0x8\n"
			"fadd.s f25, f21, f11, rne\n"
			"fmadd.ps f25, f25, f25, f31, rup\n"
			"bne x31, x5, 1f\n"
			"1:\n"
			"for.pi f25, f23, f17\n"
			"bge x15, x28, 1f\n"
			"1:\n"
			"fslli.pi f7, f25, 0x3\n"
			"la x23, rand_ieee754_32\n"
			"flw.ps f15, 416(x23)\n"
			"fmv.x.s x14, f15\n" 
			"li x5, 0x2acfc1885bc533c1\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0xf388e27e40b1549e\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"fsin.ps f25, f7\n"
			"fadd.s f26, f26, f25, rne\n"
			"bne x31, x26, 1f\n"
			"1:\n"
			"fxor.pi f29, f4, f25\n"
			"fround.ps f14, f10, rne\n"
			"fround.ps f27, f27, rtz\n"
			"fround.ps f17, f7, rne\n"
			"fnmadd.s f14, f14, f29, f25, rtz\n"
			"fsub.ps f6, f14, f6, rdn\n"
			"la x20, rand_ieee754_32\n"
			"flw.ps f10, 576(x20)\n"
			"frsq.ps f29, f10\n" 
			"fsatu8.pi f7, f7\n"
			"mova.x.m x20\n"
			"fmv.x.s x20, f5\n"
			"fxor.pi f17, f0, f14\n"
			"fsrai.pi f28, f30, 0x9\n"
			"fxor.pi f6, f30, f7\n"
			"fsatu8.pi f14, f30\n"
			"fcvt.f16.ps f16, f30\n"
			"bltu x23, x3, 1f\n"
			"1:\n"
			"li x9, 0x5a29a21b4123274d\n"
			"ori x5, x9, 0x4\n" 
			"fround.ps f21, f14, rup\n"
			"ori x26, x6, 0x1a\n"
			"xori x14, x11, 0x1a\n"
			"ori x25, x13, 0x5\n"
			"xori x14, x6, 0x1\n"
			"andi x10, x14, 0xa\n"
			"bne x23, x15, 1f\n"
			"1:\n"
			"xori x11, x11, 0x14\n"
			"fround.ps f15, f3, rne\n"
			"la x7, rand_ieee754_16\n"
			"flw.ps f29, 928(x7)\n"
			"fcvt.ps.f16 f18, f29\n" 
			"fsub.ps f9, f3, f9, rtz\n"
			"mulh x22, x9, x17\n"
			"fsatu8.pi f7, f5\n"
			"fadd.s f19, f29, f29, rne\n"
			"fadd.s f28, f3, f29, rtz\n"
			"beq x0, x8, 1f\n"
			"1:\n"
			"fmadd.ps f2, f15, f18, f15, rtz\n"
			"xori x7, x20, 0x1a\n"
			"fcmovm.ps f19, f18, f7\n"
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
			"fsub.ps f0, f31, f17, rup\n"
			"bne x13, x11, 1f\n"
			"1:\n"
			"fmadd.ps f6, f18, f6, f6, rup\n"
			"beq x16, x5, 1f\n"
			"1:\n"
			"fsatu8.pi f28, f17\n"
			"fadd.s f30, f20, f4, rne\n"
			"fcmovm.ps f10, f14, f4\n"
			"bgeu x0, x9, 1f\n"
			"1:\n"
			"LBL_SEQID_4_XMA_QNT_STR_SEQ_ITER_0_HID_0:\n"
			"li x7, ((0x3 << TENSOR_STORE_REG_STRIDE) | (0x13 << TENSOR_STORE_START_REG) | (0x0 << TENSOR_STORE_STORE_COLS) | (0x2 << TENSOR_STORE_AROWS) | (0x0 << TENSOR_STORE_COOP_STORE) | (0x0 << TENSOR_STORE_OPC_0) | (0x818d878f6 << TENSOR_STORE_VADDR) | 0 )\n" 
			"li x31, (0x40 << 4)\n"
			"csrw tensor_store, x7\n"	
			"LBL_SEQID_4_XMA_QNT_STR_SEQ_ITER_1_HID_0:\n"
			"li x7, ((0x0 << TENSOR_STORE_REG_STRIDE) | (0x9 << TENSOR_STORE_START_REG) | (0x3 << TENSOR_STORE_STORE_COLS) | (0xd << TENSOR_STORE_AROWS) | (0x0 << TENSOR_STORE_COOP_STORE) | (0x0 << TENSOR_STORE_OPC_0) | (0x828322e80 << TENSOR_STORE_VADDR) | 0 )\n" 
			"li x31, (0x40 << 4)\n"
			"csrw tensor_store, x7\n"	
			"LBL_SEQID_4_XMA_QNT_STR_SEQ_ITER_2_HID_0:\n"
			"li x7, ((0x2 << TENSOR_STORE_REG_STRIDE) | (0x1 << TENSOR_STORE_START_REG) | (0x3 << TENSOR_STORE_STORE_COLS) | (0x0 << TENSOR_STORE_AROWS) | (0x0 << TENSOR_STORE_COOP_STORE) | (0x0 << TENSOR_STORE_OPC_0) | (0x8296f3283 << TENSOR_STORE_VADDR) | 0 )\n" 
			"li x31, (0xc0 << 4)\n"
			"csrw tensor_store, x7\n"	
			"LBL_SEQID_4_XMA_QNT_STR_SEQ_ITER_3_HID_0:\n"
			"li x7, ((0x3 << TENSOR_STORE_REG_STRIDE) | (0x1e << TENSOR_STORE_START_REG) | (0x0 << TENSOR_STORE_STORE_COLS) | (0xd << TENSOR_STORE_AROWS) | (0x0 << TENSOR_STORE_COOP_STORE) | (0x0 << TENSOR_STORE_OPC_0) | (0x80003ab << TENSOR_STORE_VADDR) | 0 )\n" 
			"li x31, (0x80 << 4)\n"
			"csrw tensor_store, x7\n"	
			"LBL_SEQID_4_XMA_QNT_STR_SEQ_ITER_4_HID_0:\n"
			"li x7, ((0x0 << TENSOR_STORE_REG_STRIDE) | (0x15 << TENSOR_STORE_START_REG) | (0x3 << TENSOR_STORE_STORE_COLS) | (0xd << TENSOR_STORE_AROWS) | (0x0 << TENSOR_STORE_COOP_STORE) | (0x0 << TENSOR_STORE_OPC_0) | (0x829976f83 << TENSOR_STORE_VADDR) | 0 )\n" 
			"li x31, (0xc0 << 4)\n"
			"csrw tensor_store, x7\n"	
			"LBL_SEQID_4_XMA_QNT_STR_SEQ_ITER_5_HID_0:\n"
			"li x7, ((0x0 << TENSOR_STORE_REG_STRIDE) | (0x3 << TENSOR_STORE_START_REG) | (0x1 << TENSOR_STORE_STORE_COLS) | (0x0 << TENSOR_STORE_AROWS) | (0x0 << TENSOR_STORE_COOP_STORE) | (0x0 << TENSOR_STORE_OPC_0) | (0x8281fb880 << TENSOR_STORE_VADDR) | 0 )\n" 
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
			"li x5, 0xa0\n"
			"la x31, rand_ieee754_32\n"
			"add x31, x31, x5\n"
			"flw.ps f0, 0(x31)\n"
			"flw.ps f1, 0(x31)\n"
			"flw.ps f13, 0(x31)\n"
			"flw.ps f14, 7(x31)\n"
			"flw.ps f28, 3(x31)\n"
			"flw.ps f29, 20(x31)\n"
			"flw.ps f3, 3(x31)\n"
			"flw.ps f31, 4(x31)\n"
			"flw.ps f4, 11(x31)\n"
			"flw.ps f5, 28(x31)\n"
			"flw.ps f6, 20(x31)\n"
			"la x31, rand_int_32\n"
			"add x31, x31, x5\n"
			"lw x0, 0(x31)\n"
			"lw x11, 3(x31)\n"
			"lw x12, 5(x31)\n"
			"lw x13, 2(x31)\n"
			"lw x19, 13(x31)\n"
			"lw x20, 17(x31)\n"
			"lw x22, 17(x31)\n"
			"lw x26, 1(x31)\n"
			"lw x27, 19(x31)\n"
			"lw x5, 20(x31)\n"
			"lw x9, 5(x31)\n"
			"li x21, 0x3\n"
			"LBL_SEQID_4_LOOP_SEQ_HID_0:\n"
			"fadd.s f31, f13, f31, rup\n"
			"srliw x0, x9, 1\n"
			"andi x25, x20, 0x14\n"
			"bne x24, x21, 1f\n"
			"1:\n"
			"fslli.pi f14, f1, 0xc\n"
			"fround.ps f5, f5, rtz\n"
			"fnmadd.s f13, f1, f28, f3, rne\n"
			"fmax.s f0, f3, f3\n"
			"blt x24, x25, 1f\n"
			"1:\n"
			"fmadd.ps f0, f29, f4, f0, rtz\n"
			"srli x5, x5, 4\n"
			"fadd.s f11, f13, f0, rup\n"
			"bge x0, x8, 1f\n"
			"1:\n"
			"fsatu8.pi f12, f1\n"
			"fmax.s f9, f4, f3\n"
			"srliw x25, x27, 5\n"
			"fround.ps f28, f14, rtz\n"
			"srli x26, x26, 1\n"
			"fround.ps f9, f14, rtz\n"
			"mulh x0, x12, x13\n"
			"fadd.s f4, f4, f0, rup\n"
			"fmadd.ps f15, f6, f0, f4, rdn\n"
			"srliw x19, x19, 2\n"
			"fmadd.ps f9, f29, f31, f14, rup\n"
			"bne x8, x12, 1f\n"
			"1:\n"
			"fround.ps f15, f3, rtz\n"
			"fround.ps f3, f3, rne\n"
			"andi x23, x9, 0x17\n"
			"fround.ps f9, f13, rne\n"
			"srli x19, x13, 0\n"
			"andi x0, x22, 0x8\n"
			"fadd.s f13, f14, f4, rmm\n"
			"fcvt.ps.f16 f5, f29\n"
			"srliw x23, x27, 1\n"
			"srliw x0, x9, 1\n"
			"bltu x29, x7, 1f\n"
			"1:\n"
			"fmax.s f28, f28, f6\n"
			"fslli.pi f27, f14, 0x8\n"
			"fadd.s f0, f5, f0, rdn\n"
			"fslli.pi f29, f29, 0xf\n"
			"beq x11, x3, 1f\n"
			"1:\n"
			"fsatu8.pi f18, f31\n"
			"fnmadd.s f6, f0, f13, f6, rup\n"
			"mulh x28, x22, x19\n"
			"mulh x9, x26, x9\n"
			"fround.ps f18, f31, rmm\n"
			"fsatu8.pi f5, f31\n"
			"fcmovm.ps f13, f13, f14\n"
			"fmadd.ps f31, f29, f1, f28, rup\n"
			"fsatu8.pi f28, f28\n"
			"fcmovm.ps f15, f14, f6\n"
			"fmin.pi f12, f14, f1\n"
			"xori x0, x12, 0x0\n"
			"andi x23, x20, 0xa\n"
			"xori x7, x12, 0x11\n"
			"bge x17, x6, 1f\n"
			"1:\n"
			"fnmadd.s f13, f6, f0, f4, rmm\n"
			"fcvt.ps.f16 f13, f13\n"
			"mulh x25, x20, x26\n"
			"fmadd.ps f18, f29, f3, f5, rup\n"
			"bltu x7, x4, 1f\n"
			"1:\n"
			"fround.ps f27, f4, rne\n"
			"fadd.s f9, f31, f3, rup\n"
			"fcvt.ps.f16 f5, f6\n"
			"fmin.pi f31, f28, f4\n"
			"xori x7, x5, 0xd\n"
			"fmin.pi f31, f31, f29\n"
			"xori x28, x12, 0x12\n"
			"fnmadd.s f29, f4, f29, f31, rup\n"
			"bgeu x5, x12, 1f\n"
			"1:\n"
			"xori x22, x12, 0x8\n"
			"bltu x16, x29, 1f\n"
			"1:\n"
			"fmax.s f11, f0, f0\n"
			"addw x25, x19, x13\n"
			"bltu x25, x27, 1f\n"
			"1:\n"
			"srli x12, x12, 3\n"
			"fmin.pi f31, f6, f29\n"
			"addw x9, x9, x9\n"
			"bge x1, x26, 1f\n"
			"1:\n"
			"fnmadd.s f28, f29, f13, f28, rdn\n"
			"fmadd.ps f15, f0, f3, f14, rtz\n"
			"addw x19, x19, x11\n"
			"blt x2, x5, 1f\n"
			"1:\n"
			"mulh x23, x26, x27\n"
			"fmin.pi f11, f14, f31\n"
			"bge x2, x5, 1f\n"
			"1:\n"
			"fsub.ps f18, f3, f4, rmm\n"
			"blt x28, x20, 1f\n"
			"1:\n"
			"srliw x0, x19, 5\n"
			"bltu x2, x4, 1f\n"
			"1:\n"
			"srli x28, x9, 3\n"
			"fsatu8.pi f11, f31\n"
			"fmin.pi f11, f3, f5\n"
			"fmin.pi f13, f31, f13\n"
			"bgeu x26, x14, 1f\n"
			"1:\n"
			"fnmadd.s f18, f14, f28, f29, rup\n"
			"srli x20, x20, 0\n"
			"bltu x16, x27, 1f\n"
			"1:\n"
			"fmin.pi f4, f4, f28\n"
			"andi x23, x26, 0x8\n"
			"fround.ps f27, f29, rne\n"
			"fmin.pi f12, f31, f31\n"
			"fslli.pi f13, f3, 0x5\n"
			"bne x27, x23, 1f\n"
			"1:\n"
			"fadd.s f12, f31, f31, rup\n"
			"andi x0, x0, 0x12\n"
			"fnmadd.s f15, f1, f0, f4, rmm\n"
			"mulh x13, x13, x9\n"
			"fsatu8.pi f27, f3\n"
			"fsatu8.pi f5, f13\n"
			"srliw x26, x26, 5\n"
			"fsatu8.pi f4, f3\n"
			"bne x5, x15, 1f\n"
			"1:\n"
			"addw x28, x5, x11\n"
			"fsub.ps f13, f28, f3, rne\n"
			"fmin.pi f5, f4, f4\n"
			"fmadd.ps f12, f13, f5, f31, rtz\n"
			"addw x0, x13, x0\n"
			"fnmadd.s f13, f31, f29, f14, rup\n"
			"srliw x28, x20, 4\n"
			"fnmadd.s f6, f6, f6, f6, rmm\n"
			"fmax.s f15, f13, f29\n"
			"blt x26, x6, 1f\n"
			"1:\n"
			"fsatu8.pi f27, f13\n"
			"mulh x28, x9, x27\n"
			"fmin.pi f3, f3, f5\n"
			"fmax.s f12, f28, f1\n"
			"bltu x26, x20, 1f\n"
			"1:\n"
			"fslli.pi f0, f29, 0x6\n"
			"blt x2, x12, 1f\n"
			"1:\n"
			"xori x12, x19, 0x10\n"
			"fadd.s f5, f14, f3, rup\n"
			"fsatu8.pi f29, f13\n"
			"bge x31, x16, 1f\n"
			"1:\n"
			"mulh x22, x0, x13\n"
			"fnmadd.s f3, f3, f3, f4, rmm\n"
			"fnmadd.s f0, f6, f0, f31, rmm\n"
			"beq x22, x26, 1f\n"
			"1:\n"
			"fcmovm.ps f3, f4, f3\n"
			"fsub.ps f11, f3, f29, rdn\n"
			"addw x22, x5, x20\n"
			"bne x11, x14, 1f\n"
			"1:\n"
			"addw x11, x9, x11\n"
			"fsatu8.pi f9, f14\n"
			"bne x28, x2, 1f\n"
			"1:\n"
			"fsub.ps f14, f4, f14, rdn\n"
			"fround.ps f4, f4, rtz\n"
			"fmadd.ps f18, f3, f14, f13, rdn\n"
			"fcvt.ps.f16 f5, f3\n"
			"bne x24, x28, 1f\n"
			"1:\n"
			"fslli.pi f4, f3, 0xc\n"
			"addw x19, x0, x19\n"
			"fnmadd.s f31, f5, f29, f28, rmm\n"
			"fmax.s f15, f13, f14\n"
			"fnmadd.s f5, f28, f6, f5, rtz\n"
			"fcmovm.ps f31, f28, f28\n"
			"blt x3, x16, 1f\n"
			"1:\n"
			"srliw x27, x27, 4\n"
			"bltu x26, x3, 1f\n"
			"1:\n"
			"fcvt.ps.f16 f13, f6\n"
			"bgeu x20, x0, 1f\n"
			"1:\n"
			"andi x28, x22, 0x10\n"
			"fmax.s f31, f5, f6\n"
			"addi x21, x21, -1\n"
			"bne x21, x0, LBL_SEQID_4_LOOP_SEQ_HID_0\n"
			"li x5, 5\n" 
			"bne x30, x5, LBL_FAIL_HID_0\n"
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f24,  704 (x5)\n"
			"flw.ps f23,  0 (x5)\n"
			"flw.ps f17,  128 (x5)\n"
			"flw.ps f1,  800 (x5)\n"
			"flw.ps f14,  288 (x5)\n"
			"flw.ps f22,  864 (x5)\n"
			"flw.ps f2,  928 (x5)\n"
			"flw.ps f19,  512 (x5)\n"
			"flw.ps f21,  64 (x5)\n"
			"flw.ps f8,  544 (x5)\n"
			"flw.ps f12,  192 (x5)\n"
			"flw.ps f9,  416 (x5)\n"
			"flw.ps f5,  576 (x5)\n"
			"flw.ps f26,  480 (x5)\n"
			"flw.ps f11,  320 (x5)\n"
			"flw.ps f29,  608 (x5)\n"
			"flw.ps f7,  32 (x5)\n"
			"flw.ps f4,  448 (x5)\n"
			"flw.ps f20,  832 (x5)\n"
			"flw.ps f15,  992 (x5)\n"
			"flw.ps f25,  960 (x5)\n"
			"flw.ps f28,  672 (x5)\n"
			"flw.ps f27,  224 (x5)\n"
			"flw.ps f0,  736 (x5)\n"
			"flw.ps f31,  640 (x5)\n"
			"flw.ps f10,  256 (x5)\n"
			"flw.ps f18,  896 (x5)\n"
			"flw.ps f16,  352 (x5)\n"
			"flw.ps f3,  384 (x5)\n"
			"flw.ps f6,  96 (x5)\n"
			"flw.ps f30,  768 (x5)\n"
			"flw.ps f13,  160 (x5)\n"
			"addi x30, x30, 1\n"
			"la x23, rand_int_32\n"
			"flw.ps f10, 128(x23)\n"
			"fremu.pi f6, f10, f10\n" 
			"fmin.pi f23, f28, f0\n"
			"or x9, x15, x22\n"
			"bge x24, x9, 1f\n"
			"1:\n"
			"fcmovm.ps f24, f8, f28\n"
			"fclass.ps f0, f8\n"
			"bltu x5, x2, 1f\n"
			"1:\n"
			"LBL_SEQID_5_INT_G_STORE_ID_12_HID_0:\n"
			"li x28, 0x822d86382c\n" 
			"shg x25, (x28)\n" 
			"fcvt.pwu.ps f17, f17, rmm\n"
			"fcvt.s.wu f29, x12, rtz\n"
			"fsub.ps f30, f18, f21, rmm\n"
			"subw x17, x5, x0\n"
			"bge x20, x22, 1f\n"
			"1:\n"
			"LBL_SEQID_5_FP_L_GATHER_ID_13_HID_0:\n"
			"li x27, 0x8241fb8800\n" 
			"la x16, mem_ptr_offset\n"
			"flw.ps f9, 0(x16)\n"
			"fscwl.ps f0, f9 (x27)\n"
			"fgbl.ps f24, f9 (x27)\n" 
			"fmax.ps f17, f18, f17\n"
			"fnot.pi f29, f6\n"
			"fmsub.ps f19, f18, f10, f23, rmm\n"
			"bltu x5, x19, 1f\n"
			"1:\n"
			"fmin.pi f22, f7, f18\n"
			"LBL_SEQID_5_INT_L_STORE_ID_14_HID_0:\n"
			"li x19, 0x814b200108\n" 
			"sbl x7, (x19)\n" 
			"fmin.s f20, f20, f20\n"
			"packb x5, x5, x5\n"
			"fand.pi f5, f3, f23\n"
			"divuw x13, x27, x0\n"
			"LBL_SEQID_5_FP_SCATTER_ID_15_HID_0:\n"
			"li x28, 0x8109e9f174\n" 
			"la x26, mem_ptr_offset\n"
			"flw.ps f3, 212(x26)\n"
			"fscb.ps f29, f3 (x28)\n" 
			"fsgnjn.ps f31, f10, f8\n"
			"fltu.pi f2, f2, f10\n"
			"blt x18, x17, 1f\n"
			"1:\n"
			"fle.pi f27, f27, f0\n"
			"fnmadd.ps f12, f0, f19, f13, rdn\n"
			"LBL_SEQID_5_FP_L_SCATTER_ID_16_HID_0:\n"
			"li x15, 0x82574f8810\n" 
			"la x27, mem_ptr_offset\n"
			"flw.ps f28, 148(x27)\n"
			"fscbl.ps f6, f28 (x15)\n" 
			"fsub.s f8, f27, f7, rtz\n"
			"bne x17, x20, 1f\n"
			"1:\n"
			"fmulhu.pi f13, f26, f4\n"
			"bge x19, x27, 1f\n"
			"1:\n"
			"fround.ps f14, f22, rmm\n"
			"fmadd.ps f13, f22, f2, f21, rmm\n"
			"LBL_SEQID_5_INT_AMO_ID_17_HID_0:\n"
			"li x10, 0x8251d2003c\n" 
			"amoswapl.w x11, x5, (x10)\n" 
			"add x12, x12, x23\n"
			"bne x1, x15, 1f\n"
			"1:\n"
			"div x14, x10, x13\n"
			"sub x15, x13, x11\n"
			"fmin.s f27, f21, f8\n"
			"LBL_SEQID_5_INT_AMO_ID_18_HID_0:\n"
			"li x13, 0x8140e88804\n" 
			"amoxorl.w x11, x11, (x13)\n" 
			"frcp.ps f18, f18\n"
			"feqm.ps m6, f27, f24\n"
			"fclass.ps f29, f24\n"
			"fmax.s f24, f18, f18\n"
			"LBL_SEQID_5_FP_AMO_ID_19_HID_0:\n"
			"li x20, 0x82338c0018\n" 
			"la x18, mem_ptr_offset\n"
			"flw.ps f27, 320(x18)\n"
			"famomaxug.pi f15, f27 (x20)\n" 
			"fexp.ps f1, f28\n"
			"fnmsub.ps f1, f14, f24, f24, rtz\n"
			"fminu.pi f1, f24, f28\n"
			"fsgnjn.ps f24, f14, f24\n"
			"LBL_SEQID_5_FP_G_SCATTER_ID_20_HID_0:\n"
			"li x14, 0x8127c4ea20\n" 
			"la x22, mem_ptr_offset\n"
			"flq2 f10, 32(x22)\n"
			"fscwg.ps f29, f10 (x14)\n" 
			"fcvt.ps.f16 f2, f11\n"
			"frsq.ps f18, f7\n"
			"fltu.pi f11, f11, f25\n"
			"fmv.s.x f11, x23\n"
			"LBL_SEQID_5_FP_LOAD_ID_21_HID_0:\n"
			"li x7, 0x82035c02a6\n" 
			"fsw.ps f0, -524(x7)\n"
			"flq2 f6, -524(x7)\n" 
			"faddi.pi f20, f15, 0x171\n"
			"fsgnjn.s f30, f23, f10\n"
			"bge x15, x27, 1f\n"
			"1:\n"
			"fslli.pi f30, f10, 0x9\n"
			"for.pi f15, f12, f12\n"
			"LBL_SEQID_5_FP_LOAD_ID_22_HID_0:\n"
			"li x18, 0x8109af914c\n" 
			"fsq2 f4, -203(x18)\n"
			"flw.ps f1, -203(x18)\n" 
			"fnmsub.s f9, f21, f31, f25, rtz\n"
			"fsgnjn.s f18, f18, f10\n"
			"feq.ps f31, f2, f20\n"
			"flog.ps f19, f11\n"
			"LBL_SEQID_5_INT_AMO_ID_23_HID_0:\n"
			"li x20, 0x812ef27818\n" 
			"amomaxg.w x11, x5, (x20)\n" 
			"fsin.ps f31, f1\n"
			"fcvt.s.w f8, x5, rtz\n"
			"ffrc.ps f19, f26\n"
			"bgeu x9, x5, 1f\n"
			"1:\n"
			"packb x13, x7, x13\n"
			"LBL_SEQID_5_INT_STORE_ID_24_HID_0:\n"
			"li x26, 0x810cb82828\n" 
			"sh x28, -1762(x26)\n" 
			"fmax.ps f24, f29, f24\n"
			"bne x16, x12, 1f\n"
			"1:\n"
			"fand.pi f17, f28, f17\n"
			"fltu.pi f1, f19, f5\n"
			"fsub.ps f12, f29, f17, rup\n"
			"LBL_SEQID_5_FP_AMO_ID_25_HID_0:\n"
			"li x20, 0x823004003c\n" 
			"la x23, mem_ptr_offset\n"
			"flw.ps f25, 928(x23)\n"
			"famoswapg.pi f24, f25 (x20)\n" 
			"for.pi f11, f17, f28\n"
			"fcvt.pw.ps f31, f31, rne\n"
			"fsrli.pi f3, f2, 0x8\n"
			"fmulhu.pi f5, f5, f16\n"
			"LBL_SEQID_5_CSR_ID_26_HID_0:\n"
			"li x14, 0xc00008209977585\n"
			"li x31, (0x3 << 6) | 0x1\n"
			"csrw evict_va, x14\n" 
			"csrwi tensor_wait, 6\n" 
			"fmulh.pi f14, f7, f2\n"
			"bge x14, x26, 1f\n"
			"1:\n"
			"remuw x18, x10, x18\n"
			"fclass.s x26, f7\n"
			"fmul.s f27, f2, f31, rup\n"
			"bne x1, x11, 1f\n"
			"1:\n"
			"LBL_SEQID_5_FP_L_SCATTER_ID_27_HID_0:\n"
			"li x13, 0x815bbc0008\n" 
			"la x26, mem_ptr_offset\n"
			"flw.ps f13, 180(x26)\n"
			"fscwl.ps f1, f13 (x13)\n" 
			"flt.pi f3, f10, f10\n"
			"frsq.ps f25, f22\n"
			"fexp.ps f8, f26\n"
			"bge x30, x10, 1f\n"
			"1:\n"
			"fnmsub.s f10, f19, f14, f14, rtz\n"
			"LBL_SEQID_5_FP_AMO_ID_28_HID_0:\n"
			"li x12, 0x81566c0004\n" 
			"la x5, mem_ptr_offset\n"
			"flw.ps f0, 640(x5)\n"
			"famoorl.pi f30, f0 (x12)\n" 
			"fmax.pi f2, f24, f31\n"
			"bgeu x8, x31, 1f\n"
			"1:\n"
			"fcvt.ps.pw f15, f1, rmm\n"
			"flog.ps f1, f23\n"
			"fcvt.pw.ps f15, f1, rup\n"
			"blt x10, x22, 1f\n"
			"1:\n"
			"LBL_SEQID_5_INT_L_STORE_ID_29_HID_0:\n"
			"li x12, 0x825ad003c4\n" 
			"sbl x14, (x12)\n" 
			"fsetm.pi m6, f21\n"
			"sllw x12, x12, x5\n"
			"fand.pi f30, f8, f8\n"
			"mulw x27, x16, x11\n"
			"LBL_SEQID_5_FP_L_SCATTER_ID_30_HID_0:\n"
			"li x6, 0x8249000030\n" 
			"la x13, mem_ptr_offset\n"
			"flq2 f6, 208(x13)\n"
			"fscbl.ps f1, f6 (x6)\n" 
			"fnmsub.s f7, f20, f11, f16, rdn\n"
			"fmul.ps f30, f30, f0, rtz\n"
			"sra x14, x7, x26\n"
			"slliw x28, x28, 5\n"
			"LBL_SEQID_5_FP_AMO_ID_31_HID_0:\n"
			"li x18, 0x824934c810\n" 
			"la x20, mem_ptr_offset\n"
			"flw.ps f5, 480(x20)\n"
			"famoorl.pi f8, f5 (x18)\n" 
			"ffrc.ps f8, f31\n"
			"fsgnjx.ps f14, f0, f5\n"
			"fmulhu.pi f14, f5, f0\n"
			"fcvt.ps.pwu f4, f7, rup\n"
			"bge x9, x31, 1f\n"
			"1:\n"
			"LBL_SEQID_5_INT_LOAD_ID_32_HID_0:\n"
			"li x18, 0x8204a002a9\n" 
			"sd x16, -1862(x18)\n"
			"lh x6, -1862(x18)\n" 
			"fsub.s f12, f12, f3, rne\n"
			"frsq.ps f30, f20\n"
			"beq x5, x4, 1f\n"
			"1:\n"
			"fsra.pi f5, f3, f5\n"
			"beq x3, x28, 1f\n"
			"1:\n"
			"fmax.s f24, f5, f5\n"
			"LBL_SEQID_5_FP_AMO_ID_33_HID_0:\n"
			"li x22, 0x8137e3f004\n" 
			"la x6, mem_ptr_offset\n"
			"flw.ps f14, 224(x6)\n"
			"famoxorg.pi f30, f14 (x22)\n" 
			"fnmadd.ps f14, f14, f14, f8, rup\n"
			"addw x16, x19, x16\n"
			"bne x12, x5, 1f\n"
			"1:\n"
			"fbcx.ps f14, x28\n"
			"masknot m4, m1\n"
			"LBL_SEQID_5_FP_LOAD_ID_34_HID_0:\n"
			"li x14, 0x8207f37813\n" 
			"fsw.ps f8, 1680(x14)\n"
			"flw f10, 1680(x14)\n" 
			"fandi.pi f19, f11, 0x1cf\n"
			"frcp.ps f6, f11\n"
			"fsgnjx.s f29, f5, f6\n"
			"fmin.ps f29, f7, f11\n"
			"LBL_SEQID_5_INT_STORE_ID_35_HID_0:\n"
			"li x6, 0x820997759d\n" 
			"sd x17, 1707(x6)\n" 
			"slti x13, x12, 0xa\n"
			"sltiu x14, x14, 0x1a\n"
			"fadd.ps f6, f6, f10, rup\n"
			"fle.ps f30, f30, f17\n"
			"LBL_SEQID_5_INT_STORE_ID_36_HID_0:\n"
			"li x14, 0x82090005e1\n" 
			"sh x19, 64(x14)\n" 
			"fmulh.pi f20, f21, f24\n"
			"fsin.ps f19, f21\n"
			"fcvt.pw.ps f21, f28, rdn\n"
			"bge x17, x19, 1f\n"
			"1:\n"
			"feq.ps f22, f24, f28\n"
			"LBL_SEQID_5_CSR_ID_37_HID_0:\n"
			"li x28, 0x4d005dd85900b3c0\n"
			"csrw tensor_conv_size, x28\n" 
			"fcmov.ps f6, f28, f26, f22\n"
			"fcvt.f16.ps f9, f29\n"
			"fcmovm.ps f26, f27, f29\n"
			"fle.pi f16, f16, f0\n"
			"bgeu x7, x30, 1f\n"
			"1:\n"
			"LBL_SEQID_5_FP_LOAD_ID_38_HID_0:\n"
			"li x12, 0x820d361dc6\n" 
			"fsw.ps f15, -1988(x12)\n"
			"flw f9, -1988(x12)\n" 
			"fmin.pi f12, f12, f12\n"
			"bne x20, x11, 1f\n"
			"1:\n"
			"fsgnjx.s f3, f22, f20\n"
			"flt.pi f22, f20, f22\n"
			"fltu.pi f29, f12, f8\n"
			"LBL_SEQID_5_INT_AMO_ID_39_HID_0:\n"
			"li x6, 0x82480131dc\n" 
			"amoswapl.w x10, x14, (x6)\n" 
			"fminu.pi f19, f20, f1\n"
			"fle.ps f9, f28, f16\n"
			"fcvt.s.w f2, x5, rdn\n"
			"ebreak\n"
			"bne x1, x26, 1f\n"
			"1:\n"
			"LBL_SEQID_5_CSR_ID_40_HID_0:\n"
			"csrr x0, hartid\n"
			"fsgnjn.s f6, f9, f19\n"
			"fround.ps f26, f10, rne\n"
			"fsin.ps f16, f13\n"
			"fexp.ps f31, f30\n"
			"la x18, rand_ieee754_32\n"
			"flw.ps f20, 352(x18)\n"
			"flw.ps f14, 384(x18)\n"
			"fdiv.ps f6, f20, f14, rdn\n" 
			"mul x15, x15, x28\n"
			"fpackrepb.pi f20, f26\n"
			"fcvt.ps.pw f20, f20, rdn\n"
			"lui x11, 0x18\n"
			"LBL_SEQID_5_INT_STORE_ID_41_HID_0:\n"
			"li x9, 0x820322e801\n" 
			"sw x11, -760(x9)\n" 
			"fsin.ps f18, f12\n"
			"blt x4, x18, 1f\n"
			"1:\n"
			"fround.ps f20, f2, rmm\n"
			"fmin.s f26, f17, f26\n"
			"fcvt.pw.ps f8, f7, rdn\n"
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
			"fsgnjx.ps f8, f0, f0\n"
			"bge x7, x28, 1f\n"
			"1:\n"
			"fmin.pi f27, f21, f2\n"
			"fround.ps f27, f0, rdn\n"
			"fmax.pi f27, f0, f0\n"
			"fmsub.s f0, f2, f25, f1, rup\n"
			"fadd.ps f31, f25, f0, rne\n"
			"LBL_SEQID_5_XMA_QNT_STR_SEQ_ITER_0_HID_0:\n"
			"li x6, ((0x10 << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0x6 << TENSOR_QUANT_QUANT_ROW) | (0x11 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x2 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_5_XMA_QNT_STR_SEQ_ITER_1_HID_0:\n"
			"li x6, ((0x14 << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0x3 << TENSOR_QUANT_QUANT_ROW) | (0xa << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x2 << TENSOR_QUANT_TRANSF4) | (0x6 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_5_XMA_QNT_STR_SEQ_ITER_2_HID_0:\n"
			"li x6, ((0x1d << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0xe << TENSOR_QUANT_QUANT_ROW) | (0x2 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x2 << TENSOR_QUANT_TRANSF4) | (0x6 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_5_XMA_QNT_STR_SEQ_ITER_3_HID_0:\n"
			"li x6, ((0x16 << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0x5 << TENSOR_QUANT_QUANT_ROW) | (0x3a << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x8 << TENSOR_QUANT_TRANSF2) | (0x8 << TENSOR_QUANT_TRANSF1) | (0xa << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_5_XMA_QNT_STR_SEQ_ITER_4_HID_0:\n"
			"li x6, ((0x7 << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0xf << TENSOR_QUANT_QUANT_ROW) | (0x34 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x6 << TENSOR_QUANT_TRANSF5) | (0x7 << TENSOR_QUANT_TRANSF4) | (0x7 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_5_XMA_QNT_STR_SEQ_ITER_5_HID_0:\n"
			"li x6, ((0x6 << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0x5 << TENSOR_QUANT_QUANT_ROW) | (0x3e << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x2 << TENSOR_QUANT_TRANSF6) | (0x6 << TENSOR_QUANT_TRANSF5) | (0x6 << TENSOR_QUANT_TRANSF4) | (0x6 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 10\n"
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
			"li x5, 0xc0\n"
			"la x31, rand_ieee754_32\n"
			"add x31, x31, x5\n"
			"flw.ps f15, 0(x31)\n"
			"flw.ps f2, 1(x31)\n"
			"flw.ps f26, 7(x31)\n"
			"flw.ps f8, 3(x31)\n"
			"li x21, 0x3\n"
			"LBL_SEQID_5_LOOP_SEQ_HID_0:\n"
			"fsra.pi f25, f8, f26\n"
			"bge x6, x16, 1f\n"
			"1:\n"
			"fle.pi f2, f2, f26\n"
			"fadd.ps f2, f26, f15, rdn\n"
			"for.pi f29, f26, f15\n"
			"fmin.pi f26, f15, f26\n"
			"fmin.pi f8, f26, f8\n"
			"fsgnjn.s f26, f26, f2\n"
			"fsra.pi f26, f8, f8\n"
			"fsrl.pi f29, f2, f2\n"
			"fltu.pi f29, f8, f15\n"
			"fle.ps f2, f2, f2\n"
			"flog.ps f2, f15\n"
			"fcvt.ps.pw f7, f26, rmm\n"
			"bge x27, x22, 1f\n"
			"1:\n"
			"fround.ps f8, f8, rne\n"
			"fand.pi f26, f26, f26\n"
			"frcp.ps f26, f8\n"
			"fnmsub.s f2, f8, f26, f2, rdn\n"
			"fadd.ps f31, f2, f2, rmm\n"
			"fsub.pi f26, f26, f26\n"
			"bgeu x6, x30, 1f\n"
			"1:\n"
			"fmax.pi f8, f8, f8\n"
			"faddi.pi f2, f26, 0xa3\n"
			"fmax.s f2, f15, f8\n"
			"bltu x31, x5, 1f\n"
			"1:\n"
			"fnmsub.ps f2, f2, f26, f15, rne\n"
			"bltu x14, x31, 1f\n"
			"1:\n"
			"fsrli.pi f15, f15, 0x2\n"
			"fsgnjx.ps f2, f2, f15\n"
			"fmul.ps f8, f15, f8, rtz\n"
			"fsra.pi f26, f26, f15\n"
			"bltu x16, x8, 1f\n"
			"1:\n"
			"fcmovm.ps f31, f2, f26\n"
			"bne x3, x7, 1f\n"
			"1:\n"
			"fmin.pi f15, f26, f8\n"
			"beq x3, x0, 1f\n"
			"1:\n"
			"flt.pi f7, f8, f15\n"
			"fnmadd.ps f8, f8, f15, f2, rdn\n"
			"fmadd.s f26, f2, f26, f8, rne\n"
			"for.pi f8, f8, f2\n"
			"fsub.ps f26, f15, f8, rup\n"
			"fmax.pi f26, f15, f2\n"
			"bltu x13, x15, 1f\n"
			"1:\n"
			"fsgnj.s f25, f2, f26\n"
			"fcvt.ps.pw f26, f26, rne\n"
			"blt x5, x8, 1f\n"
			"1:\n"
			"fcvt.f16.ps f15, f15\n"
			"feq.pi f25, f26, f26\n"
			"fmin.ps f26, f15, f8\n"
			"fsll.pi f25, f2, f8\n"
			"fmadd.ps f15, f2, f15, f15, rup\n"
			"fsgnjx.s f31, f8, f2\n"
			"bgeu x1, x7, 1f\n"
			"1:\n"
			"flt.pi f25, f26, f2\n"
			"fmax.ps f31, f8, f26\n"
			"bne x13, x0, 1f\n"
			"1:\n"
			"flt.pi f2, f26, f8\n"
			"frcp.ps f25, f26\n"
			"fsub.pi f2, f8, f2\n"
			"feq.pi f31, f2, f26\n"
			"fadd.s f31, f8, f2, rne\n"
			"fcvt.ps.pw f29, f26, rne\n"
			"fpackrepb.pi f8, f26\n"
			"fadd.ps f15, f15, f8, rtz\n"
			"fsgnj.s f29, f2, f2\n"
			"ffrc.ps f31, f2\n"
			"fsgnjn.ps f2, f26, f26\n"
			"beq x13, x11, 1f\n"
			"1:\n"
			"fle.pi f8, f15, f15\n"
			"fltu.pi f7, f2, f2\n"
			"fswizz.ps f7, f2, 0x18\n"
			"fadd.ps f2, f2, f2, rdn\n"
			"fcvt.pwu.ps f8, f8, rtz\n"
			"feq.pi f15, f15, f15\n"
			"fnot.pi f31, f8\n"
			"fadd.s f15, f26, f2, rdn\n"
			"fsub.pi f7, f8, f15\n"
			"fle.pi f2, f26, f8\n"
			"fcmovm.ps f25, f8, f8\n"
			"fnmadd.ps f15, f15, f15, f8, rtz\n"
			"fandi.pi f25, f2, 0x38\n"
			"blt x7, x9, 1f\n"
			"1:\n"
			"fmin.pi f26, f15, f26\n"
			"fround.ps f26, f2, rne\n"
			"fadd.s f25, f2, f15, rup\n"
			"fsub.ps f31, f2, f8, rdn\n"
			"bgeu x17, x5, 1f\n"
			"1:\n"
			"fmaxu.pi f31, f8, f8\n"
			"fsgnjn.s f8, f8, f26\n"
			"bge x19, x14, 1f\n"
			"1:\n"
			"fnot.pi f8, f8\n"
			"beq x7, x4, 1f\n"
			"1:\n"
			"flog.ps f2, f2\n"
			"fltu.pi f2, f8, f15\n"
			"fclass.ps f31, f15\n"
			"bltu x7, x27, 1f\n"
			"1:\n"
			"flt.ps f8, f8, f26\n"
			"fmin.s f25, f26, f8\n"
			"bge x9, x9, 1f\n"
			"1:\n"
			"fle.ps f2, f15, f2\n"
			"fmax.pi f7, f26, f2\n"
			"fnmadd.ps f7, f8, f2, f2, rup\n"
			"flt.ps f15, f8, f26\n"
			"fsgnjx.ps f7, f8, f8\n"
			"fmsub.s f2, f2, f15, f26, rtz\n"
			"bne x10, x31, 1f\n"
			"1:\n"
			"fminu.pi f7, f15, f15\n"
			"fmin.ps f8, f26, f15\n"
			"flt.pi f26, f26, f8\n"
			"bge x27, x2, 1f\n"
			"1:\n"
			"fsgnjn.ps f2, f26, f2\n"
			"fmulh.pi f26, f8, f26\n"
			"fadd.ps f7, f2, f8, rmm\n"
			"bge x28, x29, 1f\n"
			"1:\n"
			"fnmadd.s f26, f8, f26, f8, rup\n"
			"fnot.pi f26, f15\n"
			"bltu x15, x14, 1f\n"
			"1:\n"
			"feq.pi f29, f15, f8\n"
			"fmul.s f31, f15, f26, rmm\n"
			"fcvt.pw.ps f8, f2, rup\n"
			"faddi.pi f26, f26, 0x15e\n"
			"faddi.pi f7, f2, 0x1f2\n"
			"fpackreph.pi f25, f15\n"
			"fnmadd.ps f26, f2, f26, f2, rmm\n"
			"fmulh.pi f2, f8, f26\n"
			"fround.ps f2, f8, rmm\n"
			"fnmadd.ps f26, f26, f26, f15, rmm\n"
			"fcvt.ps.f16 f26, f26\n"
			"fsgnjx.s f7, f15, f15\n"
			"frcp.ps f25, f2\n"
			"bltu x2, x22, 1f\n"
			"1:\n"
			"fandi.pi f29, f26, 0x1d9\n"
			"fsgnjn.ps f7, f26, f8\n"
			"frsq.ps f26, f8\n"
			"fnmadd.ps f7, f15, f2, f2, rmm\n"
			"feq.ps f25, f26, f2\n"
			"fmsub.ps f8, f8, f8, f26, rup\n"
			"fadd.s f29, f2, f26, rne\n"
			"fmadd.s f26, f26, f15, f15, rdn\n"
			"fltu.pi f31, f2, f2\n"
			"fle.ps f26, f15, f26\n"
			"fsgnj.ps f2, f15, f15\n"
			"fmul.pi f2, f2, f26\n"
			"fexp.ps f2, f2\n"
			"fmin.pi f26, f26, f26\n"
			"fsub.pi f26, f15, f8\n"
			"fand.pi f2, f2, f26\n"
			"fclass.ps f7, f8\n"
			"feq.pi f7, f26, f26\n"
			"fsrl.pi f26, f2, f26\n"
			"beq x22, x4, 1f\n"
			"1:\n"
			"flt.ps f26, f2, f26\n"
			"fadd.pi f26, f8, f26\n"
			"fsatu8.pi f29, f8\n"
			"fsat8.pi f7, f26\n"
			"fmsub.ps f29, f15, f8, f2, rup\n"
			"for.pi f15, f15, f15\n"
			"ffrc.ps f31, f15\n"
			"fsgnj.s f29, f2, f2\n"
			"faddi.pi f25, f15, 0x19c\n"
			"faddi.pi f8, f8, 0x1b8\n"
			"fmsub.ps f26, f8, f15, f2, rdn\n"
			"fadd.pi f8, f8, f15\n"
			"feq.pi f25, f15, f15\n"
			"fsgnj.ps f15, f26, f15\n"
			"fround.ps f31, f2, rne\n"
			"fsgnj.s f25, f15, f8\n"
			"ffrc.ps f31, f15\n"
			"fmax.s f7, f26, f8\n"
			"bgeu x5, x14, 1f\n"
			"1:\n"
			"fsub.s f29, f8, f2, rtz\n"
			"bge x14, x0, 1f\n"
			"1:\n"
			"fcvt.ps.pw f7, f26, rup\n"
			"fmsub.ps f7, f15, f8, f15, rdn\n"
			"fsll.pi f2, f26, f26\n"
			"beq x7, x22, 1f\n"
			"1:\n"
			"feq.ps f26, f26, f15\n"
			"bge x8, x29, 1f\n"
			"1:\n"
			"addi x21, x21, -1\n"
			"bne x21, x0, LBL_SEQID_5_LOOP_SEQ_HID_0\n"
			"li x5, 6\n" 
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
			"LBL_HPM_3_CORE_TFMA_WAIT_TENB_FAIL_HID_0:\n"          
			"csrr x5, mhpmcounter3\n"
			"sd x5, 32 (x31)\n"
			"LBL_HPM_4_CORE_DCACHE_ACCESS0_FAIL_HID_0:\n"          
			"csrr x5, mhpmcounter4\n"
			"sd x5, 40 (x31)\n"
			"LBL_HPM_5_CORE_TXFMA_OPS16_FAIL_HID_0:\n"          
			"csrr x5, mhpmcounter5\n"
			"sd x5, 48 (x31)\n"
			"LBL_HPM_6_CORE_TS_INST_FAIL_HID_0:\n"          
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
			"LBL_HPM_3_CORE_TFMA_WAIT_TENB_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter3\n"
			"sd x5, 32 (x31)\n"
			"LBL_HPM_4_CORE_DCACHE_ACCESS0_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter4\n"
			"sd x5, 40 (x31)\n"
			"LBL_HPM_5_CORE_TXFMA_OPS16_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter5\n"
			"sd x5, 48 (x31)\n"
			"LBL_HPM_6_CORE_TS_INST_PASS_HID_0:\n"          
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
		0xf6301f88, 
		0x3a02527f, 
		0xf07061fd, 
		0xd698cadd, 
		0xc2ed168f, 
		0x7274cc81, 
		0x78e9666f, 
		0x4c0e08fe, 
		0x1517c3ea, 
		0xb3ebc95a, 
		0xfb590ecf, 
		0xc58ebf13, 
		0xfcfb5610, 
		0x23889d6b, 
		0x6982328b, 
		0xd9807f77, 
		0xa144d831, 
		0xd600b8e8, 
		0x0ead7cf1, 
		0x8e97a392, 
		0xfdaf7e4f, 
		0x3a73a906, 
		0x047f6124, 
		0x27c501ae, 
		0x3290b729, 
		0x988f1884, 
		0x29ba1cfb, 
		0x02f7426e, 
		0xa7b8856b, 
		0x58b377df, 
		0xd99f295e, 
		0x36df3f35, 
		0x01641ff5, 
		0xcb734913, 
		0xd372c109, 
		0x1e6ee52e, 
		0xbb4bd4f1, 
		0x35156832, 
		0xd58db60a, 
		0xfa7ebb2a, 
		0x39eab9da, 
		0xce8116b0, 
		0x7869498d, 
		0x20b558ea, 
		0x091d5a59, 
		0x3fba37e7, 
		0x4268b289, 
		0x80112a62, 
		0x75af2605, 
		0xf6d68760, 
		0xe1f90f99, 
		0xe4811bf5, 
		0x0272a329, 
		0xc5c9013e, 
		0x74725fd7, 
		0xdd95ce4a, 
		0x77e7eb05, 
		0x45ce1f0e, 
		0x006d1fcf, 
		0x39cb96e4, 
		0x6e1764ec, 
		0xbefcf759, 
		0x70c9433e, 
		0x96b7fe4c, 
		0xf9889941, 
		0x128c9583, 
		0x609c7826, 
		0x301f8d68, 
		0xeb5b6310, 
		0x2b13684b, 
		0x8854a46f, 
		0x7e136fad, 
		0x4ed60cb8, 
		0xbfbbbd7e, 
		0x8134db10, 
		0x06c9fe41, 
		0x818b778f, 
		0x3be56819, 
		0xafbe811b, 
		0x5acdaa31, 
		0x540947a9, 
		0x5b1d4217, 
		0x18252b5a, 
		0x158b77af, 
		0xe59097b4, 
		0x84c3ade0, 
		0xc42a1609, 
		0xd31422f1, 
		0x4431f54f, 
		0x72835eb4, 
		0x6c759444, 
		0x744bcf9e, 
		0xcfacddf0, 
		0xad62e352, 
		0x2aabfb6a, 
		0xd924efb7, 
		0x682a62b6, 
		0x55408322, 
		0x56906408, 
		0xc0ce2180, 
		0xafb005b4, 
		0xd29d598b, 
		0xaecfe087, 
		0xea2f4a06, 
		0x2b60da77, 
		0xb95022a9, 
		0xc111a4c5, 
		0x646dc5cb, 
		0x0b74888b, 
		0xb9604faa, 
		0xa16987a6, 
		0x24e35fa6, 
		0xc95a8e9d, 
		0xe66fd056, 
		0xb9632cdb, 
		0xe67b2aeb, 
		0x75a5715f, 
		0xf1dea995, 
		0x1ec1c671, 
		0xbbd16b26, 
		0x9bf15ad0, 
		0x184d9893, 
		0x7d6ce0c0, 
		0x9f658447, 
		0x11e910ca, 
		0x36d8f20d, 
		0x63bf8c98, 
		0x29e044ca, 
		0x42430e7e, 
		0x8f102e5c, 
		0xd4061e15, 
		0x491b26e4, 
		0xfa5a40f0, 
		0x8d3197bc, 
		0xc37824ff, 
		0xb60385a7, 
		0x64d27e22, 
		0xf592a221, 
		0x582d2feb, 
		0xf14ecd6b, 
		0xabdfff6c, 
		0xda6d6c03, 
		0xc51abec6, 
		0xd1d9b9d0, 
		0x3ad5f558, 
		0xfb220a0e, 
		0xc9ab3232, 
		0x03e21eec, 
		0x51b81c10, 
		0x9e3a1ccd, 
		0x8252e55b, 
		0xcf6944be, 
		0x33a85cb8, 
		0xbd275f9f, 
		0xb1afc3ae, 
		0x1178bff0, 
		0x2a5efa5c, 
		0x69a76ed7, 
		0x463b47d5, 
		0x85be9477, 
		0xd660ee34, 
		0x71ba94a3, 
		0x0a6399e6, 
		0xb79c2fd7, 
		0x95e1601b, 
		0x3f270b19, 
		0xfdbdba5e, 
		0x1637c760, 
		0x67493d6c, 
		0x302dada5, 
		0xf20faece, 
		0x82f67de1, 
		0x85e1eb1f, 
		0xbb905b76, 
		0xa2ef7790, 
		0x55e485e0, 
		0x338c7c8c, 
		0x0144bccb, 
		0xcbdc15cc, 
		0x9b50c207, 
		0x60331e99, 
		0x9dbf20b0, 
		0x2fd9f374, 
		0x3a4b0a4e, 
		0x7ed40e5f, 
		0x6290c8f8, 
		0x09bbe646, 
		0x4224540e, 
		0x8fa5e9a5, 
		0x87b301bc, 
		0x069311f4, 
		0xbb2c9033, 
		0x7d87c51c, 
		0xfb060546, 
		0x368c9c6b, 
		0x7bb6dce1, 
		0x469ad161, 
		0x510db34a, 
		0x9bd2a414, 
		0x7ea64cb8, 
		0x7f8f8b39, 
		0x9ba146fc, 
		0x69358fee, 
		0xb645033c, 
		0xd4fe6b25, 
		0x853bb361, 
		0x54c66fdc, 
		0x3ccdb33b, 
		0xa8d1caa0, 
		0x08528eca, 
		0x33848e76, 
		0xfbb58ca6, 
		0xc133bc1d, 
		0x2505641b, 
		0x146ce18d, 
		0xae37e00f, 
		0x0fc46f8f, 
		0xe5682732, 
		0xee93a168, 
		0xc353e7fa, 
		0xe7400dde, 
		0xc84e5dda, 
		0xf3bcfcb3, 
		0x051d12db, 
		0xb93224e4, 
		0xad90e56c, 
		0xeef879ca, 
		0x9ee4bf22, 
		0x267a1db1, 
		0x8505f758, 
		0xe9072647, 
		0xc1f6b609, 
		0x0fb56698, 
		0x0ad95947, 
		0xc7df15f4, 
		0x31966833, 
		0x3b2dd00c, 
		0x56abf1a5, 
		0xfde18b50, 
		0x0d51f195, 
		0xe62d4393, 
		0xd7aae256, 
		0x9ede1015, 
		0x8d99a756, 
		0xf7d6a965, 
		0x2201561f, 
		0x54ab3bea, 
		0x0363d50f, 
		0xc5e1b8b4, 
		0x5394ea93, 
		0x0f6f9732, 
		0x1c50fce4, 
		0x58da689c, 
		0x9e240e92, 
		0x458df7c5, 
		0xb32209ec  
};
volatile uint16_t rand_ieee754_16[1536] __attribute__ ((aligned (2048))) = {
		0xce80, 
		0x51c0, 
		0x4700, 
		0xc200, 
		0xacd3, 
		0x6a97, 
		0x5220, 
		0x9b44, 
		0x5240, 
		0xcd00, 
		0xf157, 
		0xc500, 
		0x52c0, 
		0xd0e0, 
		0xcfc0, 
		0x0000, 
		0x5320, 
		0xd180, 
		0xe9e7, 
		0x1158, 
		0x5020, 
		0x4d80, 
		0xd300, 
		0x52a0, 
		0xd360, 
		0x50c0, 
		0x4f80, 
		0xd260, 
		0x51c0, 
		0xd100, 
		0xd1e0, 
		0xd040, 
		0x4400, 
		0xd000, 
		0xfa48, 
		0xc980, 
		0x4f40, 
		0xd2a0, 
		0xcf00, 
		0xcb80, 
		0xcb80, 
		0x0770, 
		0x1623, 
		0x4f40, 
		0xf6fc, 
		0xcc80, 
		0xce80, 
		0x8000, 
		0x4c80, 
		0x84b1, 
		0xd140, 
		0x5100, 
		0x5220, 
		0xd320, 
		0x8000, 
		0xc980, 
		0x8000, 
		0xcf40, 
		0x753c, 
		0xca80, 
		0x53a0, 
		0xb5af, 
		0x5020, 
		0x53c0, 
		0x7c00, 
		0x53a0, 
		0x5360, 
		0x8000, 
		0xfc00, 
		0xd320, 
		0xc980, 
		0xbc00, 
		0x7944, 
		0x717c, 
		0xa2e8, 
		0xc000, 
		0xa7d4, 
		0x1aa4, 
		0x15ca, 
		0xce80, 
		0xb8fe, 
		0x4e00, 
		0xca00, 
		0x4bcd, 
		0x51e0, 
		0xd3a0, 
		0xcf40, 
		0x7ba0, 
		0x53c0, 
		0xc700, 
		0x4700, 
		0x5120, 
		0x5a38, 
		0x2ae0, 
		0x4400, 
		0x0000, 
		0x5120, 
		0xd020, 
		0xd320, 
		0x51c0, 
		0x53a0, 
		0xbb2e, 
		0x03c2, 
		0x5360, 
		0xe08d, 
		0x52c0, 
		0x4a80, 
		0x5300, 
		0x50c0, 
		0xd0e0, 
		0x5040, 
		0xfd8e, 
		0xcd40, 
		0x2ddd, 
		0x4d80, 
		0x52e0, 
		0x8000, 
		0x8000, 
		0x5360, 
		0xfcf0, 
		0xd120, 
		0xeec0, 
		0xcb00, 
		0x5380, 
		0x7d59, 
		0x591b, 
		0xc3cc, 
		0xce40, 
		0xc3fd, 
		0xd180, 
		0x5240, 
		0x5000, 
		0xd1c0, 
		0x4c80, 
		0x50e0, 
		0x5380, 
		0x4ec0, 
		0xd68e, 
		0xce80, 
		0x0b39, 
		0xd0e0, 
		0x4e80, 
		0x4b80, 
		0xc700, 
		0x9253, 
		0x0000, 
		0xcc40, 
		0xd240, 
		0x53a0, 
		0x4c00, 
		0x5040, 
		0x50a0, 
		0xd1c0, 
		0x89f4, 
		0x5260, 
		0xc700, 
		0x4a00, 
		0x5100, 
		0xcd80, 
		0xcc00, 
		0x8000, 
		0x8f28, 
		0x4a80, 
		0x8000, 
		0xcd40, 
		0xed7e, 
		0x4ec0, 
		0x4e80, 
		0x5320, 
		0x50c0, 
		0xf991, 
		0x9b18, 
		0xca80, 
		0xd0a0, 
		0x4500, 
		0x7c00, 
		0xd380, 
		0x4dc0, 
		0x1f72, 
		0xc800, 
		0x5020, 
		0xd0e0, 
		0x5240, 
		0x4555, 
		0xb90c, 
		0x4ec0, 
		0x8000, 
		0xcf80, 
		0xd120, 
		0xd040, 
		0xd280, 
		0x4200, 
		0x026b, 
		0x4b00, 
		0x5080, 
		0xd320, 
		0xd3c0, 
		0x0000, 
		0xfc00, 
		0xc880, 
		0xc500, 
		0x6ad7, 
		0x40d8, 
		0x51c0, 
		0x4ec0, 
		0x51fd, 
		0x0908, 
		0x4800, 
		0x5300, 
		0xe749, 
		0xfc00, 
		0xdb32, 
		0x7cc4, 
		0xd040, 
		0x4b80, 
		0x4bc6, 
		0xff55, 
		0x4f80, 
		0x4d40, 
		0xce40, 
		0x5100, 
		0x41a8, 
		0xc880, 
		0x80cb, 
		0xd240, 
		0x6893, 
		0xc880, 
		0xcaf2, 
		0xc500, 
		0x5380, 
		0xd340, 
		0xbdf8, 
		0xd280, 
		0x4b00, 
		0x8d27, 
		0xfc00, 
		0x9c17, 
		0xce00, 
		0x747b, 
		0xd1a0, 
		0x5360, 
		0xd380, 
		0xdb20, 
		0x53e0, 
		0x4c40, 
		0x8000, 
		0x94da, 
		0xd1a0, 
		0xd080, 
		0x50a0, 
		0x4cc0, 
		0x0055, 
		0x4800, 
		0x98ef, 
		0xa718, 
		0x024a, 
		0x4d80, 
		0xd040, 
		0x4200, 
		0xbd4b, 
		0xf58f, 
		0x4200, 
		0x4c7a, 
		0xd000, 
		0xbf7d, 
		0xd1c0, 
		0xa7fc, 
		0x5260, 
		0x6fee, 
		0xfcd0, 
		0xac1d, 
		0xd240, 
		0x5120, 
		0x1488, 
		0x4500, 
		0xd1a0, 
		0xc980, 
		0x8000, 
		0xcd40, 
		0x3fa7, 
		0x7c56, 
		0xd1c0, 
		0x5320, 
		0x7c47, 
		0xcd80, 
		0x3296, 
		0xcf40, 
		0xcec0, 
		0x8000, 
		0xd360, 
		0x4400, 
		0xd040, 
		0xd100, 
		0x5100, 
		0x4fc0, 
		0xd360, 
		0x6a6f, 
		0x1d32, 
		0x23a6, 
		0x4c00, 
		0x5240, 
		0x7c00, 
		0xf505, 
		0x8719, 
		0xd200, 
		0xca0e, 
		0xd260, 
		0xd000, 
		0x4980, 
		0x9278, 
		0xc200, 
		0xcc80, 
		0xd100, 
		0xcf00, 
		0xca80, 
		0x0000, 
		0x5ccc, 
		0x5380, 
		0xa111, 
		0xe469, 
		0xcd40, 
		0x7753, 
		0xca00, 
		0xfc00, 
		0x8000, 
		0x0730, 
		0xcd80, 
		0x51e0, 
		0x5320, 
		0xd3e0, 
		0xd100, 
		0xc000, 
		0x0000, 
		0xcd00, 
		0x7146, 
		0xcfc0, 
		0x6a78, 
		0x4cc0, 
		0x5100, 
		0xfc00, 
		0x3c00, 
		0x5360, 
		0xd2e0, 
		0x50a0, 
		0xfc00, 
		0xc400, 
		0x6571, 
		0x0000, 
		0xd3e0, 
		0xc900, 
		0xc342, 
		0xd8dd, 
		0xd240, 
		0xcd80, 
		0x52e0, 
		0xd300, 
		0x4880, 
		0x696a, 
		0x4800, 
		0xccc0, 
		0x7863, 
		0x50a0, 
		0xd180, 
		0xca80, 
		0x7c00, 
		0x5080, 
		0xc800, 
		0x4b00, 
		0x64f6, 
		0xd0c0, 
		0x5320, 
		0x28f3, 
		0xdf59, 
		0xc200, 
		0xfc68, 
		0xd2c0, 
		0xc61a, 
		0xe96f, 
		0x159f, 
		0x2b9b, 
		0xd220, 
		0x9730, 
		0xcd80, 
		0x5200, 
		0x7c00, 
		0xf119, 
		0x36d0, 
		0xd060, 
		0xec13, 
		0x3289, 
		0xd220, 
		0x3007, 
		0x29a8, 
		0xd0e0, 
		0xccc0, 
		0x4f80, 
		0x4dc0, 
		0xfc00, 
		0xd66d, 
		0xfc00, 
		0x4de8, 
		0xd320, 
		0xc800, 
		0x5040, 
		0xcac5, 
		0xca80, 
		0x4b80, 
		0x4400, 
		0xf06a, 
		0x51c0, 
		0x7cd6, 
		0x5280, 
		0x8000, 
		0x6d55, 
		0x4880, 
		0x5280, 
		0xc96e, 
		0x0000, 
		0x4200, 
		0x8000, 
		0xd2c0, 
		0x5040, 
		0xd320, 
		0xd040, 
		0xd2c0, 
		0xb21a, 
		0x4e40, 
		0xc800, 
		0xcc80, 
		0xfc00, 
		0xccc0, 
		0xd2c0, 
		0x4885, 
		0x8000, 
		0x7b2a, 
		0x6512, 
		0xfc00, 
		0x2225, 
		0xfd1c, 
		0xd060, 
		0xfc00, 
		0xd300, 
		0xd68c, 
		0x26f3, 
		0xbb01, 
		0x4000, 
		0x5060, 
		0xd140, 
		0xb2ea, 
		0x8000, 
		0x50c0, 
		0xca80, 
		0x4b00, 
		0x4c40, 
		0x4880, 
		0xd120, 
		0xa202, 
		0xd380, 
		0x5120, 
		0x4b00, 
		0x52a0, 
		0x4900, 
		0x50c0, 
		0x98fd, 
		0x8000, 
		0xe96b, 
		0x0000, 
		0xd1c0, 
		0x8000, 
		0xcd00, 
		0x5120, 
		0x8b9e, 
		0x4c00, 
		0xc200, 
		0x8000, 
		0xc724, 
		0xd3cc, 
		0xd200, 
		0x3f9f, 
		0xcc40, 
		0x4200, 
		0xc000, 
		0x0000, 
		0x53a0, 
		0x8c2f, 
		0xd879, 
		0x314c, 
		0x5080, 
		0x5100, 
		0x5000, 
		0xfc00, 
		0xd380, 
		0xc500, 
		0xb80d, 
		0xf0e2, 
		0x6a44, 
		0xd2e0, 
		0xc400, 
		0xd200, 
		0xce00, 
		0x4d83, 
		0xaad3, 
		0x27fd, 
		0xce40, 
		0x5080, 
		0x4600, 
		0xce80, 
		0xcb00, 
		0x4a80, 
		0xfc00, 
		0x50a0, 
		0x2d08, 
		0x4800, 
		0x7c19, 
		0xfb37, 
		0x5020, 
		0x8000, 
		0x6bf1, 
		0xd200, 
		0x5000, 
		0xd360, 
		0xf96a, 
		0xb5de, 
		0xdb4a, 
		0x4c40, 
		0x50c0, 
		0xbc00, 
		0xc700, 
		0x3a0c, 
		0xcb00, 
		0xfc00, 
		0x5220, 
		0xcfc0, 
		0xc32d, 
		0x4e40, 
		0x83b8, 
		0x4b80, 
		0x5380, 
		0xfc00, 
		0x29ec, 
		0xcdb4, 
		0x33f8, 
		0x5080, 
		0xfc00, 
		0x50e0, 
		0x5360, 
		0xd2e0, 
		0xc900, 
		0xd380, 
		0x4a00, 
		0x8619, 
		0x4b00, 
		0xc800, 
		0x4200, 
		0xfdfb, 
		0x5240, 
		0xd040, 
		0xc9b1, 
		0xe2ff, 
		0x4000, 
		0xfaf7, 
		0xce00, 
		0x6aa6, 
		0xd180, 
		0x4d00, 
		0x4c00, 
		0x20d7, 
		0x6495, 
		0x51a0, 
		0xcd40, 
		0xd1c0, 
		0x53c0, 
		0xd180, 
		0xd100, 
		0x5020, 
		0x2f22, 
		0x65ec, 
		0xd0e0, 
		0x5340, 
		0xd260, 
		0x53c0, 
		0xa49d, 
		0xcd00, 
		0x51e0, 
		0xc265, 
		0x4b80, 
		0x3c00, 
		0xbaf6, 
		0x0000, 
		0xc980, 
		0x5000, 
		0x4500, 
		0x4d40, 
		0xc500, 
		0x0caa, 
		0x4900, 
		0x4400, 
		0xbb21, 
		0xd080, 
		0x4900, 
		0x5120, 
		0xd0c0, 
		0xc600, 
		0x51a0, 
		0xcf40, 
		0x5320, 
		0x52a0, 
		0x1464, 
		0x51e0, 
		0x8000, 
		0x8000, 
		0x8000, 
		0x5280, 
		0xd360, 
		0x69fa, 
		0x2bdd, 
		0x0756, 
		0x50a0, 
		0x56f9, 
		0x53a0, 
		0xd1a0, 
		0x5340, 
		0x5220, 
		0x3c82, 
		0x4000, 
		0x3b5a, 
		0x5120, 
		0x50c0, 
		0xc800, 
		0x4600, 
		0xd1a0, 
		0x53a0, 
		0x03a0, 
		0xc980, 
		0x4500, 
		0x4400, 
		0x52a0, 
		0xd3a0, 
		0x65bc, 
		0x8dd9, 
		0xe977, 
		0x9ffc, 
		0x5080, 
		0xcfa9, 
		0xd1c0, 
		0x8000, 
		0x4200, 
		0xcf00, 
		0x51c0, 
		0x0000, 
		0xc700, 
		0x8160, 
		0x4dc0, 
		0xce80, 
		0xd240, 
		0x9f1d, 
		0x07d0, 
		0xc2c3, 
		0xda9e, 
		0xcfc0, 
		0x8000, 
		0x53c0, 
		0xd3a0, 
		0xcec0, 
		0x3595, 
		0x4800, 
		0xcd40, 
		0xd080, 
		0x0000, 
		0x4cc0, 
		0x0fe2, 
		0x5200, 
		0x4400, 
		0xc700, 
		0xce80, 
		0x0000, 
		0xbc00, 
		0x2daf, 
		0xcec0, 
		0x4700, 
		0xc000, 
		0x791e, 
		0xcc40, 
		0xc500, 
		0xa190, 
		0x53e0, 
		0xd8b2, 
		0xfd7a, 
		0x8000, 
		0x8000, 
		0x5140, 
		0xd280, 
		0x8f9c, 
		0xd1c0, 
		0xd000, 
		0xc600, 
		0x0000, 
		0xfc00, 
		0xfc49, 
		0xd300, 
		0x52c0, 
		0x44b0, 
		0x5120, 
		0xcf00, 
		0x51e0, 
		0x8000, 
		0x855e, 
		0x52e0, 
		0x51c0, 
		0x8000, 
		0xd280, 
		0xaeab, 
		0xccc0, 
		0xf608, 
		0x4d00, 
		0x4e40, 
		0xbc00, 
		0xa880, 
		0x5340, 
		0x4e80, 
		0x8000, 
		0x5380, 
		0xb623, 
		0x5260, 
		0x4800, 
		0x8000, 
		0x4c80, 
		0xc600, 
		0x5180, 
		0x53c0, 
		0xca50, 
		0x4fd2, 
		0x4600, 
		0xdd90, 
		0xcd80, 
		0x7c00, 
		0x4101, 
		0xdff0, 
		0x5060, 
		0xd320, 
		0xcb00, 
		0xf4fc, 
		0x03fe, 
		0xd100, 
		0x4a80, 
		0xd0c0, 
		0x526e, 
		0x55fc, 
		0x5240, 
		0xd1a0, 
		0xce40, 
		0x396f, 
		0xcb00, 
		0xfc00, 
		0x5000, 
		0xd3ee, 
		0x31c3, 
		0x7d06, 
		0xc200, 
		0x22e8, 
		0x50c0, 
		0xc880, 
		0x50e0, 
		0x50a0, 
		0xa0a8, 
		0xd020, 
		0x7c00, 
		0x5260, 
		0x5000, 
		0x4ec0, 
		0xc73a, 
		0xd380, 
		0xd3a0, 
		0xcdc0, 
		0xcd80, 
		0x5080, 
		0x523c, 
		0x4fc0, 
		0x37d1, 
		0xf991, 
		0x7c00, 
		0x4700, 
		0x53e0, 
		0x23c8, 
		0x5080, 
		0x5320, 
		0x0000, 
		0xc880, 
		0x5020, 
		0xbc00, 
		0xbf29, 
		0xd2e0, 
		0x4fc0, 
		0xcc40, 
		0x53a0, 
		0x0000, 
		0xcb00, 
		0x7c00, 
		0x9923, 
		0xcf80, 
		0xd173, 
		0xf962, 
		0xbc00, 
		0xfc00, 
		0xd020, 
		0xd0e0, 
		0xc900, 
		0xd000, 
		0xd060, 
		0x2843, 
		0x4400, 
		0xd040, 
		0xd240, 
		0xd3a0, 
		0xd1e0, 
		0x65dd, 
		0x4700, 
		0x5360, 
		0x52e0, 
		0xcc80, 
		0x4062, 
		0xfcf9, 
		0xf52a, 
		0xd120, 
		0xc17f, 
		0x6f5c, 
		0x7c00, 
		0x5040, 
		0xcf80, 
		0x5180, 
		0xd1a0, 
		0xd240, 
		0x5120, 
		0x4400, 
		0xcd40, 
		0xcd00, 
		0xcc80, 
		0x5e2b, 
		0xe149, 
		0xd260, 
		0x4cc0, 
		0x5380, 
		0x4900, 
		0xfc00, 
		0x5060, 
		0x0e76, 
		0xfc00, 
		0xd120, 
		0xcdc0, 
		0xd040, 
		0xbba4, 
		0x4f80, 
		0xcb80, 
		0x8000, 
		0xe04c, 
		0xd220, 
		0x50c0, 
		0x83ed, 
		0x9d6b, 
		0xcebf, 
		0xd140, 
		0x4c80, 
		0xcb00, 
		0x07e2, 
		0xc700, 
		0xcec0, 
		0xce80, 
		0x52c0, 
		0xa30c, 
		0xd140, 
		0xfce2, 
		0x5280, 
		0x7c39, 
		0xd0e0, 
		0x9d20, 
		0xfe17, 
		0x4700, 
		0xd040, 
		0x1cdf, 
		0xc397, 
		0x0000, 
		0x7c00, 
		0xd67e, 
		0x0000, 
		0x93e1, 
		0x4000, 
		0xd380, 
		0x29cc, 
		0x4fc0, 
		0x4f80, 
		0x5380, 
		0x0424, 
		0x5020, 
		0xfc00, 
		0x5020, 
		0x4dc0, 
		0xc700, 
		0xd3c0, 
		0x7cf1, 
		0x50c0, 
		0x4c40, 
		0xce40, 
		0x50c0, 
		0x5320, 
		0x7c00, 
		0x8000, 
		0x4900, 
		0x5020, 
		0xd100, 
		0x5260, 
		0x5360, 
		0x4c00, 
		0x53e0, 
		0x53a0, 
		0x5340, 
		0x0000, 
		0x4f40, 
		0x5020, 
		0xa9af, 
		0x5140, 
		0xf9cc, 
		0xd3a0, 
		0x135a, 
		0xc600, 
		0x8000, 
		0x52c0, 
		0xfdac, 
		0xad28, 
		0xbf91, 
		0x727d, 
		0xcd00, 
		0xd7ca, 
		0xd180, 
		0xc880, 
		0xada2, 
		0xd3a0, 
		0xd220, 
		0x50e0, 
		0xed1b, 
		0x5080, 
		0xd300, 
		0xd0c0, 
		0x5280, 
		0x4600, 
		0x7c00, 
		0xc700, 
		0xd605, 
		0x0a3e, 
		0xcc00, 
		0x50a0, 
		0xce40, 
		0xd140, 
		0x1df1, 
		0xd140, 
		0xbec4, 
		0x51e0, 
		0x5000, 
		0xa7bb, 
		0x50c7, 
		0x8000, 
		0x53e0, 
		0xce40, 
		0x7c90, 
		0xcd40, 
		0xb4b8, 
		0x4ec0, 
		0x81a0, 
		0x0000, 
		0x4e00, 
		0x8000, 
		0x7c00, 
		0x5180, 
		0xca9e, 
		0x2519, 
		0x4c00, 
		0x5120, 
		0x4fc0, 
		0xccc0, 
		0xfda2, 
		0x5040, 
		0xcfc0, 
		0xd374, 
		0xc880, 
		0xb263, 
		0xfc00, 
		0x50c0, 
		0x2f9c, 
		0x1210, 
		0xe510, 
		0x4d80, 
		0x0603, 
		0xce40, 
		0x0000, 
		0x6776, 
		0x441c, 
		0xccb9, 
		0xacc1, 
		0xcf80, 
		0x8000, 
		0x2a7e, 
		0x4d80, 
		0x6425, 
		0x4066, 
		0xcc00, 
		0xae76, 
		0x8458, 
		0x9121, 
		0x9b97, 
		0xd040, 
		0xc019, 
		0xb907, 
		0x5260, 
		0xd0a0, 
		0x5360, 
		0x5160, 
		0x5240, 
		0x4ec0, 
		0xd1c0, 
		0xd120, 
		0x4c00, 
		0xd1a0, 
		0xd200, 
		0x52e0, 
		0x53a0, 
		0x5360, 
		0x0565, 
		0xd280, 
		0x4ec0, 
		0x4600, 
		0xcfdd, 
		0x1347, 
		0x2475, 
		0xcdc0, 
		0xca80, 
		0xce40, 
		0xcc80, 
		0x202a, 
		0x50c0, 
		0xd3e0, 
		0x4880, 
		0xaf86, 
		0xcc80, 
		0xcd40, 
		0xcdc0, 
		0x4400, 
		0x762d, 
		0x4a00, 
		0xcc00, 
		0xd3e0, 
		0xcf40, 
		0x51e0, 
		0xfc00, 
		0xcc40, 
		0x5140, 
		0xc700, 
		0x5340, 
		0x8000, 
		0xcc80, 
		0xcc80, 
		0xbac8, 
		0xcf00, 
		0x2af9, 
		0xd719, 
		0x5120, 
		0xc800, 
		0x8000, 
		0x38c0, 
		0x1be0, 
		0x5220, 
		0xd020, 
		0x4900, 
		0xf05e, 
		0xd300, 
		0xd100, 
		0xd000, 
		0x5260, 
		0x51a0, 
		0xd040, 
		0xc700, 
		0xd330, 
		0xcc40, 
		0x0069, 
		0x410e, 
		0xd180, 
		0x65f2, 
		0x7c00, 
		0xd100, 
		0xcc40, 
		0x5240, 
		0x3fb9, 
		0xa8c1, 
		0xd3c0, 
		0x1c8b, 
		0xfc00, 
		0xce00, 
		0xcf40, 
		0x0000, 
		0xd220, 
		0x4a80, 
		0xca39, 
		0xb918, 
		0xd2e0, 
		0x53c0, 
		0xd260, 
		0x5120, 
		0xc000, 
		0x4cc0, 
		0x2181, 
		0xfc00, 
		0x46d7, 
		0x7c00, 
		0xd060, 
		0x91fe, 
		0x7777, 
		0xfc00, 
		0x5340, 
		0xcf00, 
		0x144d, 
		0xca00, 
		0xb6f6, 
		0x7c00, 
		0xc000, 
		0xf630, 
		0x4ec0, 
		0x5120, 
		0x5100, 
		0x5320, 
		0xd260, 
		0x4b00, 
		0xd1a0, 
		0x5360, 
		0x4980, 
		0x12ff, 
		0xe647, 
		0x4e00, 
		0x0000, 
		0x0000, 
		0x646a, 
		0xd300, 
		0x4d80, 
		0x4d40, 
		0x4f00, 
		0x53a0, 
		0xb4e2, 
		0x52a0, 
		0xca6d, 
		0xd3c0, 
		0x50a0, 
		0x6016, 
		0x4d80, 
		0xbd39, 
		0x082c, 
		0xcc00, 
		0x4c80, 
		0x1cef, 
		0xd300, 
		0xc3a2, 
		0x50e0, 
		0x4b37, 
		0x5180, 
		0xe654, 
		0xcd93, 
		0xe83a, 
		0xd280, 
		0x4d80, 
		0xc000, 
		0x5260, 
		0xd2e0, 
		0xb005, 
		0xce40, 
		0x4e00, 
		0x5711, 
		0x5020, 
		0x5340, 
		0xcf40, 
		0x4dc0, 
		0xd220, 
		0x0447, 
		0x9f7d, 
		0x3a70, 
		0xbb70, 
		0x5060, 
		0xcfc0, 
		0x5080, 
		0x5180, 
		0x5240, 
		0xd0c0, 
		0xcb80, 
		0x4c00, 
		0xd3e0, 
		0x4600, 
		0x8000, 
		0x9e0b, 
		0x4d00, 
		0x8000, 
		0xab70, 
		0xc019, 
		0xcc80, 
		0xce80, 
		0xd1e0, 
		0x52c0, 
		0xc700, 
		0x8000, 
		0xcb80, 
		0xd200, 
		0x4e00, 
		0xbb2c, 
		0xd020, 
		0xd187, 
		0x4cc0, 
		0x8860, 
		0x50a0, 
		0xcc40, 
		0xce00, 
		0xd135, 
		0x4e00, 
		0xcf2d, 
		0x0000, 
		0x53c0, 
		0xf25e, 
		0x4600, 
		0xd100, 
		0x5220, 
		0x7c00, 
		0x8000, 
		0xcb00, 
		0x52a0, 
		0xd100, 
		0x0000, 
		0xd160, 
		0x4400, 
		0xcb00, 
		0xd380, 
		0x3c00, 
		0x4e00, 
		0x4dc0, 
		0xd300, 
		0x4dc0, 
		0x8de6, 
		0x5080, 
		0xb62a, 
		0xd0a0, 
		0x4b80, 
		0xc400, 
		0xfc00, 
		0x5060, 
		0xce40, 
		0x4c80, 
		0xd020, 
		0x53c0, 
		0x4980, 
		0x668a, 
		0xce40, 
		0xdf41, 
		0xd000, 
		0x93e9, 
		0x4880, 
		0x4000, 
		0xcfc0, 
		0x0000, 
		0x0000, 
		0xf4c5, 
		0xf996, 
		0x50e0, 
		0x5667, 
		0xc600, 
		0x4b80, 
		0xcd40, 
		0x5380, 
		0x4e80, 
		0xaba1, 
		0x4cc0, 
		0xcd40, 
		0x1408, 
		0xcec0, 
		0xce80, 
		0xd000, 
		0xb32f, 
		0xd280, 
		0x4ec0, 
		0x4e80, 
		0x378d, 
		0x8333, 
		0x4e63, 
		0x4c80, 
		0xc700, 
		0x8188, 
		0xcc00, 
		0xbf83, 
		0xa70f, 
		0xda8c, 
		0xd160, 
		0x1253, 
		0xd140, 
		0x52a0, 
		0x4800, 
		0x52c0, 
		0xcfc0, 
		0xe0af, 
		0xb1cd, 
		0xcb0b, 
		0xce80, 
		0x0000, 
		0xe9be, 
		0xd5f8, 
		0x00a6, 
		0xd000, 
		0x693e, 
		0xd120, 
		0xca00, 
		0x50a0, 
		0x0000, 
		0x4f40, 
		0x4e40, 
		0x4200, 
		0x04e4, 
		0x41fb, 
		0xc700, 
		0x8000, 
		0x5255, 
		0xd180, 
		0x50c0, 
		0xd2a0, 
		0x5951, 
		0xc600, 
		0x27ed, 
		0x53a0, 
		0xd140, 
		0xc880, 
		0xfd80, 
		0x4f40, 
		0xdba7, 
		0x5140, 
		0xd280, 
		0xfce1, 
		0x4d80, 
		0x2c74, 
		0xf9ef, 
		0xcb00, 
		0xa460, 
		0xfc00, 
		0xdb56, 
		0x5300, 
		0xf48a, 
		0x8397, 
		0x8000, 
		0x8000, 
		0xd000, 
		0xd2e0, 
		0x4fc0, 
		0xcf40, 
		0x07b2, 
		0x7c00, 
		0x63f1, 
		0xe04f, 
		0x9a87, 
		0x4500, 
		0xd2e0, 
		0x7f4e, 
		0xd180, 
		0xd658, 
		0xfc00, 
		0xc980, 
		0xfc00, 
		0x50c0, 
		0x1f76, 
		0x6a99, 
		0xcbdc, 
		0xcc80, 
		0x29ba, 
		0xcf00, 
		0x95b8, 
		0xd300, 
		0xc900, 
		0xd060, 
		0x5320, 
		0x5260, 
		0xcf80, 
		0x0000, 
		0xd1c0, 
		0x338a, 
		0xd1c0, 
		0xe1a4, 
		0x6291, 
		0x4d00, 
		0x9d29, 
		0xe013, 
		0x53a0, 
		0xd280, 
		0x4c40, 
		0x4d00, 
		0xd0c0, 
		0x5360, 
		0x5380, 
		0xd060, 
		0x08fc, 
		0x5040, 
		0x4a00, 
		0xccc0, 
		0x4000, 
		0x7c00, 
		0x4c00, 
		0x7c00, 
		0x8000, 
		0xed5e, 
		0x0000, 
		0xfd44, 
		0x5100, 
		0xd3a0, 
		0xeba0, 
		0x2d1a, 
		0x7c32, 
		0x3c00, 
		0x4e40, 
		0x64be, 
		0xd1a0, 
		0xd0e0, 
		0x5040, 
		0x51c2, 
		0xa744, 
		0xe895, 
		0xacbc, 
		0x5080, 
		0xd120, 
		0x5020, 
		0xcfc0, 
		0x5120, 
		0x53c0, 
		0x8000, 
		0xcf40, 
		0xd2a0, 
		0xfc00, 
		0x6ae7, 
		0x450f, 
		0x4200, 
		0x4f40, 
		0x4e00, 
		0xc000, 
		0xd0a0, 
		0xd3a0, 
		0x00df, 
		0xfc00, 
		0xc200, 
		0x4f00, 
		0xca00, 
		0xb4d7, 
		0x5316, 
		0x4880, 
		0x4b80, 
		0x10f1, 
		0x8000, 
		0xd300, 
		0xc800, 
		0xce17, 
		0x4b00, 
		0x8000, 
		0x7c00, 
		0x4cc0, 
		0x5100, 
		0x4600, 
		0xd3e0, 
		0x5100, 
		0x5340, 
		0x5360, 
		0x52c0, 
		0x5000, 
		0x51a0, 
		0x7845, 
		0x4d80, 
		0x7c48, 
		0x4980, 
		0x4f80, 
		0xd020, 
		0x4000, 
		0x5220, 
		0xcd40, 
		0x8a50, 
		0x52c0, 
		0xcc40, 
		0x51c0, 
		0xfd39, 
		0xc953, 
		0xd2a0, 
		0x5020, 
		0x5200, 
		0x121c, 
		0xd280, 
		0xcd40, 
		0x4f00, 
		0xd3a0, 
		0xd000, 
		0xd0ec, 
		0xdff8, 
		0x2a83, 
		0x58ef, 
		0x1425, 
		0xccc0, 
		0x5380, 
		0x4800, 
		0xcdc0, 
		0xd000, 
		0x53e0, 
		0x7c00, 
		0x5360, 
		0x6884, 
		0x4ec0, 
		0x6e3c, 
		0xcc00, 
		0x2b5c, 
		0x436b, 
		0x52a0, 
		0xfc00, 
		0xd080, 
		0x8d49, 
		0x8000, 
		0x4900, 
		0xd080, 
		0x6760, 
		0x5080, 
		0xd380, 
		0x5200, 
		0x4f40, 
		0xc600, 
		0xcc80, 
		0x4e80, 
		0x4500, 
		0x47bc, 
		0xc700, 
		0x4dc0, 
		0xc800, 
		0x8e9a, 
		0xd25f, 
		0xd060, 
		0x6014, 
		0xca80, 
		0x4d00  
};
volatile uint32_t rand_ieee754_32[1536] __attribute__ ((aligned (2048))) = {
		0x00080000, 
		0xc24c0000, 
		0x00000800, 
		0xc124197c, 
		0x7fc00000, 
		0x0c7c0000, 
		0xf3ec67f9, 
		0x00000020, 
		0x7f000000, 
		0x2f898495, 
		0xc23c0000, 
		0x7fc00001, 
		0xff800000, 
		0x8480b1d5, 
		0x0e1fffff, 
		0x41c80000, 
		0x0f7ffffd, 
		0x79bbcb20, 
		0x80100000, 
		0xc20c0000, 
		0x42440000, 
		0x0f7ffffd, 
		0x0c7ffe00, 
		0x00040000, 
		0xc1700000, 
		0x7fc00000, 
		0x00000040, 
		0xffad5230, 
		0x80000000, 
		0x20c3ffa3, 
		0xc1800000, 
		0x80040000, 
		0xc2740000, 
		0x8f7ffffe, 
		0x426c0000, 
		0x42100000, 
		0x7fbfffff, 
		0x41300000, 
		0xd5fa7daa, 
		0x00100000, 
		0xdd00516f, 
		0x0e0fffff, 
		0xc591feba, 
		0x0e003fff, 
		0xc13f212e, 
		0x003f7961, 
		0x32a08161, 
		0xc2340000, 
		0xc1000000, 
		0x1641c6c2, 
		0x42140000, 
		0xc27c0000, 
		0x00000001, 
		0x0e0003ff, 
		0xbf028f5c, 
		0x17c8540d, 
		0x80000000, 
		0x1f245d63, 
		0x80000000, 
		0x5b3c0aed, 
		0x00000100, 
		0xc2340000, 
		0xffbe688a, 
		0x0e7fffff, 
		0x80000001, 
		0x00000000, 
		0x80800003, 
		0x00000002, 
		0x531cd10e, 
		0xe1e6177c, 
		0x3e6e9430, 
		0xf4bfeca8, 
		0x80080000, 
		0x7f800000, 
		0xc1900000, 
		0x80000000, 
		0x42780000, 
		0x7f800000, 
		0x41880000, 
		0xb15bfa39, 
		0x55555555, 
		0xff800000, 
		0x02143e89, 
		0x80800002, 
		0x41100000, 
		0xff800000, 
		0xc2000000, 
		0x00400000, 
		0xff800000, 
		0x3ecee3f5, 
		0x5e028522, 
		0xecf1f0b2, 
		0xcccccccc, 
		0x4e246baf, 
		0x0c700000, 
		0x00000000, 
		0xffffffff, 
		0x80080000, 
		0x5f350b25, 
		0x0e3fffff, 
		0xff800000, 
		0x592e8fb4, 
		0xc2080000, 
		0xff800000, 
		0x00000000, 
		0xc2480000, 
		0x0c7ffe00, 
		0xff7ffffe, 
		0x00000001, 
		0xc0400000, 
		0x0f7ffffe, 
		0x41f80000, 
		0xe34df779, 
		0x800020bd, 
		0x7f7fffff, 
		0x00000001, 
		0x0c7ffc00, 
		0x6915b8f1, 
		0xc1980000, 
		0xff800000, 
		0x33333333, 
		0x693057e3, 
		0x80000020, 
		0x80040000, 
		0x8b908ebe, 
		0xffbfffff, 
		0x80000000, 
		0x0c7ff000, 
		0x7fb6637c, 
		0xc1900000, 
		0x80800001, 
		0xff014c8e, 
		0x00000004, 
		0x42400000, 
		0xc23c0000, 
		0x0c7ffff0, 
		0x00000001, 
		0xff800000, 
		0xc6073190, 
		0xc26c0000, 
		0xcb8c4b40, 
		0x0c7ffff8, 
		0x00000080, 
		0x6d23f241, 
		0x00800002, 
		0xc2780000, 
		0x09977367, 
		0x882e01dc, 
		0x00000000, 
		0x6c30aa8e, 
		0xd8fe2a44, 
		0x4cb95e29, 
		0xe3e87b67, 
		0x00000000, 
		0xff800000, 
		0x7f841ceb, 
		0xb6209e62, 
		0x0e000007, 
		0x7fc00000, 
		0x0c7ffffe, 
		0x41f80000, 
		0x42180000, 
		0x03455e16, 
		0x8b81da07, 
		0x0d00fff0, 
		0x00000080, 
		0x7fffffff, 
		0x0c7ffffc, 
		0x8f7ffffd, 
		0x805f957f, 
		0x2a923326, 
		0xc1e80000, 
		0x0ed145ba, 
		0xc1a00000, 
		0x00000000, 
		0x80000080, 
		0x0c7c0000, 
		0x42500000, 
		0x00000000, 
		0x3ac41ca5, 
		0xff800000, 
		0x41d80000, 
		0x8f7ffffc, 
		0x00080000, 
		0xcccccccc, 
		0x0e03ffff, 
		0x41a00000, 
		0x42140000, 
		0xe8cacfd5, 
		0x80200000, 
		0xad7f8f66, 
		0x37f160d1, 
		0x00100000, 
		0xc27c0000, 
		0xc2280000, 
		0x70a1a9ce, 
		0xb3347677, 
		0xc928ff19, 
		0x421c0000, 
		0x00800003, 
		0x898005d4, 
		0x80000000, 
		0xc2200000, 
		0x647e9db6, 
		0x80000004, 
		0x41880000, 
		0x80000002, 
		0xf019300b, 
		0x42400000, 
		0x42000000, 
		0x7f800000, 
		0x55a54ab5, 
		0x8caa77ce, 
		0x1009f2e3, 
		0xffaf5493, 
		0x00800002, 
		0x00000004, 
		0xc8fa6ab8, 
		0xf9fbfacc, 
		0x00000080, 
		0x42600000, 
		0x00800001, 
		0x5529cc74, 
		0xbd5f89f3, 
		0xf98ada44, 
		0x3fb70ca7, 
		0x32935ceb, 
		0x42380000, 
		0x00000000, 
		0xffc00000, 
		0xc2500000, 
		0xc25c0000, 
		0xbbb12298, 
		0x42080000, 
		0xff800000, 
		0xc7a4dbdd, 
		0x7f7ffffe, 
		0x6d166c8c, 
		0x0e000001, 
		0x4b000000, 
		0xad3e0cf0, 
		0x6489682a, 
		0x80800000, 
		0x80000010, 
		0x80000000, 
		0x7fa8f7fb, 
		0xcb000000, 
		0x00800003, 
		0x7f7fffff, 
		0x80000001, 
		0xec7d4749, 
		0xaeeee972, 
		0xffa612e8, 
		0x0c7fe000, 
		0x80000000, 
		0xc0e00000, 
		0xbf028f5c, 
		0xc2780000, 
		0x0e00ffff, 
		0x6e7f632f, 
		0x0c7fffc0, 
		0x3ab9ad8d, 
		0x80800002, 
		0x7f800000, 
		0x452b7ff8, 
		0x80100000, 
		0x4f841f10, 
		0x80000000, 
		0x0cf5365f, 
		0xe55c1e0c, 
		0xc1c00000, 
		0x20e1193a, 
		0x80800001, 
		0x7f000000, 
		0x42240000, 
		0x42780000, 
		0xffc00001, 
		0x40c00000, 
		0x80000800, 
		0x4753a223, 
		0xff7ffffe, 
		0xc2740000, 
		0x891d3ae1, 
		0xe19f3bd6, 
		0x41900000, 
		0x00800000, 
		0x7f800001, 
		0xaaaaaaaa, 
		0xb5a74f39, 
		0x916f5601, 
		0x00020000, 
		0xf02f14e6, 
		0xc1880000, 
		0xc2240000, 
		0xc1900000, 
		0x0e003fff, 
		0x424c0000, 
		0x8f7fffff, 
		0xc1e80000, 
		0x00800000, 
		0x80000001, 
		0x41d00000, 
		0x00000400, 
		0x622e40eb, 
		0xdf7895b2, 
		0xfc6170ba, 
		0x42040000, 
		0x00800003, 
		0x3d114621, 
		0x221a8969, 
		0x00400000, 
		0xf0fa71b7, 
		0xff7fffff, 
		0x0c7fe000, 
		0xc7467680, 
		0xb73b0f6e, 
		0x426f84c5, 
		0xe3bd87fd, 
		0x7f800000, 
		0x7fb84a86, 
		0x80008000, 
		0xc0c00000, 
		0xddff7c53, 
		0x5dd6232a, 
		0x0c7c0000, 
		0x0e7fffff, 
		0x80000040, 
		0xff7ffffe, 
		0xc1b00000, 
		0x41000000, 
		0x43297ea7, 
		0x0e0001ff, 
		0x7fbfffff, 
		0x425c0000, 
		0x7f7ffffe, 
		0xff855178, 
		0xc0a00000, 
		0x0e003fff, 
		0x0c7fffff, 
		0x00800003, 
		0x7f7ffffe, 
		0x80000010, 
		0x42380000, 
		0x00800002, 
		0x80040000, 
		0x42600000, 
		0xc2280000, 
		0x7ff70a44, 
		0x40800000, 
		0xe55f80ab, 
		0x80000000, 
		0x422c0000, 
		0x80645ce0, 
		0x5600e339, 
		0xc2700000, 
		0x7fc00001, 
		0x4c74ffc1, 
		0xd5b8d074, 
		0x41800000, 
		0xa149b2fc, 
		0x0c7ffff0, 
		0x7ed48990, 
		0xff7ffffe, 
		0x42480000, 
		0x0c700000, 
		0x80011111, 
		0xc2240000, 
		0xef23581b, 
		0xc2480000, 
		0xc2100000, 
		0xb7f39a17, 
		0x0e03ffff, 
		0x0e00000f, 
		0xc0c00000, 
		0x80002000, 
		0x0c7ffff0, 
		0xff7ffffe, 
		0x00000000, 
		0xc24c0000, 
		0x20b74b66, 
		0x15b1dc72, 
		0x41a80000, 
		0xc2140000, 
		0x7ff06f37, 
		0x91bf0f0f, 
		0x80000001, 
		0x80011111, 
		0x3e715247, 
		0x00000100, 
		0x00000000, 
		0x80000002, 
		0xff800000, 
		0x4a57b549, 
		0xd8559c5e, 
		0x0e00ffff, 
		0x41c80000, 
		0x9707c918, 
		0x47eef8d1, 
		0x0f7ffffd, 
		0x94f21a97, 
		0x42000000, 
		0x31b107d4, 
		0xdfb2eaf2, 
		0x84523800, 
		0x0e1fffff, 
		0xcccccccc, 
		0x80080000, 
		0x803a877e, 
		0xa9d4dff2, 
		0xdf83eff8, 
		0x00800000, 
		0x00011111, 
		0x50ef5a64, 
		0x00004000, 
		0x41980000, 
		0x42240000, 
		0x8f7ffffc, 
		0x00000000, 
		0x00000000, 
		0x007ffffe, 
		0xcb000000, 
		0x5d406adf, 
		0xff800000, 
		0x80000000, 
		0xc1200000, 
		0x7f81866c, 
		0x3f98f8fb, 
		0xfb9c1543, 
		0x42100000, 
		0x80000000, 
		0x00000040, 
		0xbf800001, 
		0x468de10a, 
		0x42480000, 
		0xc1b80000, 
		0x80100000, 
		0xf593097a, 
		0x80800001, 
		0xd0de30c7, 
		0x268d5906, 
		0x00000000, 
		0x80800003, 
		0xc1800000, 
		0x42340000, 
		0x8f7ffffe, 
		0x42780000, 
		0x41a00000, 
		0xd110086d, 
		0x00000100, 
		0x40000000, 
		0x700981fd, 
		0x80000000, 
		0xff7ffffe, 
		0x0148b323, 
		0xd50a2a63, 
		0xeef50452, 
		0x00008000, 
		0x60f28ea5, 
		0xd7e3ed52, 
		0x22bd2e14, 
		0x39cca9f0, 
		0x0e7fffff, 
		0x7fc00000, 
		0x00000000, 
		0x80000020, 
		0x53c51d21, 
		0x0c400000, 
		0xc25c0000, 
		0x007fffff, 
		0x0e000007, 
		0xff800000, 
		0x8034ac07, 
		0x802b3dfb, 
		0x41900000, 
		0x42340000, 
		0x42440000, 
		0x7f904c89, 
		0x4b000000, 
		0x42140000, 
		0x0d00fff0, 
		0x0d00fff0, 
		0x41600000, 
		0xff800000, 
		0x80000002, 
		0x0c7fffe0, 
		0xc1800000, 
		0xae6417c5, 
		0x9eecbdc1, 
		0xc2180000, 
		0x41d00000, 
		0xff86c89b, 
		0x5ebf5feb, 
		0xc2480000, 
		0x903e34d3, 
		0xff7fffff, 
		0x80020000, 
		0x08817013, 
		0x0c7ffff8, 
		0x41500000, 
		0x7fc00000, 
		0x0c7fffc0, 
		0x7fbf4141, 
		0xbf028f5c, 
		0x00040000, 
		0x7104aa88, 
		0x00000000, 
		0x8058b44b, 
		0x425c0000, 
		0xee46754e, 
		0xcd0a62fb, 
		0x422c0000, 
		0xc1000000, 
		0x42240000, 
		0x42300000, 
		0x80080000, 
		0x00020000, 
		0xaaaaaaaa, 
		0x15953096, 
		0xc1100000, 
		0x0c7ffe00, 
		0xc7e3f77d, 
		0x41880000, 
		0xc2180000, 
		0x80020000, 
		0xc1980000, 
		0x41880000, 
		0x8f7ffffc, 
		0xffffffff, 
		0x41700000, 
		0xbb7b52bb, 
		0x0e0007ff, 
		0x425c0000, 
		0xc1400000, 
		0xc2740000, 
		0x00080000, 
		0x41d00000, 
		0xffbfffff, 
		0x42080000, 
		0x00000000, 
		0x0e000001, 
		0x29f383a6, 
		0x00800002, 
		0x7f800001, 
		0x00000000, 
		0x00100000, 
		0x44686f1b, 
		0x80000000, 
		0x80000000, 
		0xff800000, 
		0x0e0003ff, 
		0x41c80000, 
		0xbf028f5c, 
		0x00000000, 
		0xfef057e6, 
		0xc1b00000, 
		0x89880db5, 
		0x80000200, 
		0x80000000, 
		0x80000001, 
		0x42640000, 
		0x7f7fffff, 
		0xff000000, 
		0x40400000, 
		0x4cc2db00, 
		0x7f800000, 
		0xff7fffff, 
		0x80000001, 
		0x2d3cdc27, 
		0x00000000, 
		0x00000002, 
		0xc2540000, 
		0x807fffff, 
		0x00000002, 
		0x422c0000, 
		0xff800000, 
		0x7f800000, 
		0x007fffff, 
		0x41a80000, 
		0x3f800000, 
		0x007fffff, 
		0x8803ae93, 
		0x0e7fffff, 
		0x42780000, 
		0x7e19922f, 
		0x80200000, 
		0xff9d2347, 
		0x00000010, 
		0x4e3eec16, 
		0xc1c80000, 
		0x0e001fff, 
		0xc1300000, 
		0x42640000, 
		0x7f800000, 
		0x80000000, 
		0xc2640000, 
		0xc1a00000, 
		0x80000000, 
		0x33333333, 
		0xc2580000, 
		0xff000000, 
		0x0e03ffff, 
		0x00000000, 
		0xb4f3a7fd, 
		0x8f7ffffd, 
		0x65ee0f27, 
		0x94bd0e92, 
		0xc2400000, 
		0x80000000, 
		0x80800001, 
		0x42200000, 
		0xf1e49f2d, 
		0x8a474951, 
		0x00800000, 
		0x41d80000, 
		0xe1bbca4f, 
		0x33333333, 
		0xc1500000, 
		0x7f800000, 
		0x80008000, 
		0x2004101e, 
		0xc2280000, 
		0x42480000, 
		0xffc00000, 
		0xff800000, 
		0x427c0000, 
		0xc1880000, 
		0xbf800001, 
		0x00800001, 
		0x496e7027, 
		0x7fbd8331, 
		0x5e380420, 
		0x00000000, 
		0x00200000, 
		0x80080000, 
		0x46a42db6, 
		0x00800000, 
		0x4e0d2592, 
		0x0c7f8000, 
		0x7efa5939, 
		0x80000002, 
		0xc24c0000, 
		0x41200000, 
		0xc2540000, 
		0x04bafe1e, 
		0xc1700000, 
		0x592a6b14, 
		0x3f800000, 
		0x0e000003, 
		0x426c0000, 
		0x7f9a77df, 
		0xff7fffff, 
		0x80000008, 
		0x7fc00001, 
		0x0c400000, 
		0x3f800001, 
		0xa9c23700, 
		0x0f7fffff, 
		0xff800000, 
		0x5625fb55, 
		0xbf028f5c, 
		0x0e0fffff, 
		0x6fb13016, 
		0xf89fa64c, 
		0xc1880000, 
		0xff800000, 
		0x80000008, 
		0x51dd1cf2, 
		0x42300000, 
		0x0c700000, 
		0x40800000, 
		0x4b3e8fd6, 
		0xf5688e50, 
		0x80000000, 
		0x45bdb7cb, 
		0xc1d80000, 
		0x7646e26e, 
		0xc1a00000, 
		0x5415a70e, 
		0x0e001fff, 
		0xc23c0000, 
		0x33333333, 
		0x42500000, 
		0xcf1176c1, 
		0xc2180000, 
		0x33333333, 
		0xbbf8ecef, 
		0x00000000, 
		0x80000000, 
		0x0c400000, 
		0x41880000, 
		0x437496fd, 
		0x7f800000, 
		0x4f8a216a, 
		0x7f800000, 
		0xffc00001, 
		0xaaaaaaaa, 
		0x42340000, 
		0x40000000, 
		0x9545b0b7, 
		0x00040000, 
		0xc0000000, 
		0x884aa285, 
		0x00000000, 
		0x0e000007, 
		0x40800000, 
		0xff800000, 
		0x41300000, 
		0x42500000, 
		0xbf800000, 
		0x58c8088f, 
		0xc1d00000, 
		0x7f800001, 
		0xc2240000, 
		0x42040000, 
		0x41300000, 
		0xb499e6cb, 
		0x12e544f3, 
		0x0e000001, 
		0x00004000, 
		0x00800003, 
		0x41200000, 
		0x426c0000, 
		0x42300000, 
		0xffa80aee, 
		0x3d42ae76, 
		0x00800001, 
		0x27feeaf9, 
		0x80800001, 
		0x7f7ffffe, 
		0x80000080, 
		0xc1c80000, 
		0x80000200, 
		0x0e000fff, 
		0x93f1a614, 
		0x6c00d8c1, 
		0xedfefd6e, 
		0xbb66c817, 
		0xcccccccc, 
		0x00000010, 
		0xc1e00000, 
		0x00000000, 
		0xb5ab9236, 
		0x4522c4f0, 
		0xff800000, 
		0x0aef36c6, 
		0x0c7fffe0, 
		0x0c7fc000, 
		0x5654704a, 
		0xa3015858, 
		0x42240000, 
		0x00001000, 
		0x38c04bd1, 
		0x80000000, 
		0x7f7ffffe, 
		0xc0e00000, 
		0xff000000, 
		0x41a00000, 
		0x4b8c4b40, 
		0x0c7ffc00, 
		0x72218351, 
		0x7fbfffff, 
		0x00000000, 
		0xc1000000, 
		0xc25c0000, 
		0x00010000, 
		0x00000010, 
		0xcb8c4b40, 
		0x90c33140, 
		0x0f7fffff, 
		0x42340000, 
		0xc2180000, 
		0xf87a4259, 
		0x2e486fcd, 
		0x453fe5fc, 
		0x55555555, 
		0xc1f80000, 
		0x0f7ffffd, 
		0x7dbdb5c0, 
		0xc23c0000, 
		0x00000000, 
		0x90ba98f2, 
		0x42400000, 
		0x0e0000ff, 
		0x96997187, 
		0x6dc8e77d, 
		0xe009345c, 
		0x8047775d, 
		0x7fa08918, 
		0xc24c0000, 
		0x046c7a48, 
		0x80000000, 
		0x7fa076ef, 
		0x42700000, 
		0x6113e413, 
		0x80000080, 
		0xc6b8924a, 
		0x7b922763, 
		0x41600000, 
		0x8617ab9c, 
		0x80000200, 
		0xc1300000, 
		0x0f7ffffc, 
		0xc1700000, 
		0xff800000, 
		0x80000000, 
		0xc1a80000, 
		0x00000100, 
		0x7c31821c, 
		0x54e18b72, 
		0x0e0000ff, 
		0xbf800001, 
		0x00800003, 
		0x00100000, 
		0xc1200000, 
		0xc24c0000, 
		0x42780000, 
		0x42080000, 
		0xff800000, 
		0x80412fd6, 
		0x1ffaf330, 
		0xaf16c8e9, 
		0x41400000, 
		0xc26c0000, 
		0x1f18eb84, 
		0xff7ffffe, 
		0x572dbf94, 
		0x0c7ff000, 
		0xc2580000, 
		0x80800002, 
		0xd9850d80, 
		0x7fc00001, 
		0x42680000, 
		0xb8aa1a54, 
		0xaf580487, 
		0x0c7f0000, 
		0xff81a322, 
		0x00000001, 
		0xc2780000, 
		0x9cd17afb, 
		0x7f800000, 
		0x80000000, 
		0x00000001, 
		0x00000020, 
		0xa21040e9, 
		0x7f7fffff, 
		0x00800000, 
		0x0c7fc000, 
		0x8320a6e5, 
		0xc2780000, 
		0x42000000, 
		0x740d593b, 
		0x0f7fffff, 
		0x9f04b758, 
		0xc2640000, 
		0xe9527eed, 
		0x89215d13, 
		0x11de850c, 
		0x0e000001, 
		0xbfe0a848, 
		0x337a0ede, 
		0x42740000, 
		0xc2180000, 
		0x005d98c5, 
		0xffc00001, 
		0x00000002, 
		0x00000000, 
		0xc0c00000, 
		0x80000004, 
		0x40a00000, 
		0xff800000, 
		0xff7ffffe, 
		0x7fc00001, 
		0x80000000, 
		0xdcec505b, 
		0x42600000, 
		0x0c7f8000, 
		0x80800000, 
		0xcf5a1172, 
		0x31722888, 
		0x41f00000, 
		0x41e00000, 
		0xc2680000, 
		0x410e86ba, 
		0xc1a00000, 
		0x531a80f6, 
		0x0e00007f, 
		0x00000020, 
		0x805f959a, 
		0x80800000, 
		0x00000000, 
		0x61c5b5c6, 
		0x42600000, 
		0x42040000, 
		0x9fef3cfb, 
		0x80011111, 
		0x0c600000, 
		0x0e0001ff, 
		0x7fc00001, 
		0x7fdff4c1, 
		0xd7deadf2, 
		0x4bea729b, 
		0x41600000, 
		0x063aa49f, 
		0x41300000, 
		0x2ee4647a, 
		0x0e00000f, 
		0x563aaaac, 
		0x47f9701a, 
		0x414843ee, 
		0x00000008, 
		0x41000000, 
		0xbe008a57, 
		0x7fffffff, 
		0x96b01f68, 
		0x87bc7b87, 
		0x5cdc027e, 
		0x80000040, 
		0xb9fce1b4, 
		0x84b0f85d, 
		0xc1600000, 
		0x80800001, 
		0x12857ead, 
		0xc0c00000, 
		0x028b4a96, 
		0xc2780000, 
		0x7e10bb10, 
		0x646835ce, 
		0x00800003, 
		0xa5a11dfe, 
		0xc2280000, 
		0x197f59fe, 
		0x0e3fffff, 
		0x8005dd9f, 
		0xd142ed40, 
		0x9556758d, 
		0xc1b80000, 
		0xd9fa2dab, 
		0xc1900000, 
		0x55278967, 
		0xd5652407, 
		0xc1500000, 
		0x0c7f0000, 
		0x00000000, 
		0x17a964a7, 
		0x80001000, 
		0xf8fe43d2, 
		0xf9343684, 
		0x42240000, 
		0x42140000, 
		0x0074add6, 
		0x80800000, 
		0xffc00001, 
		0x00000001, 
		0x5cfe3a86, 
		0xc1980000, 
		0xbf800000, 
		0x00800002, 
		0x80000001, 
		0x41a80000, 
		0x7f800000, 
		0x0d000ff0, 
		0x0c7ff000, 
		0x00800003, 
		0xc1600000, 
		0x00000000, 
		0x80010000, 
		0x7223ccc9, 
		0x80000000, 
		0x427c0000, 
		0x8dd58490, 
		0xc2600000, 
		0x41100000, 
		0x00800003, 
		0x3fda0eec, 
		0xbab893d8, 
		0x80000080, 
		0x41100000, 
		0x80010000, 
		0x871070a5, 
		0x00000000, 
		0xcb9fc150, 
		0x80000000, 
		0x00400000, 
		0x7f89c6c1, 
		0x5b51a103, 
		0xad30454c, 
		0x0c7e0000, 
		0x0c780000, 
		0xff800000, 
		0x8e0566b7, 
		0x80000040, 
		0xb91d250d, 
		0xff800000, 
		0xc1700000, 
		0x2cfbc448, 
		0x00000001, 
		0x501253e8, 
		0x00800000, 
		0xc65ebeb4, 
		0x80000040, 
		0x424c0000, 
		0x41200000, 
		0xff7ffffe, 
		0xd29d9af9, 
		0x75f8e2f9, 
		0x7f800000, 
		0x7f800000, 
		0xbe86bef7, 
		0x00000200, 
		0x8ff6323e, 
		0x00000000, 
		0x42140000, 
		0xc1c80000, 
		0x41800000, 
		0x41800000, 
		0x6ac7db56, 
		0x80000400, 
		0x00000000, 
		0x119090cd, 
		0x42780000, 
		0x80000000, 
		0x0c7ffe00, 
		0x00000008, 
		0xbb9c1a8c, 
		0x41f80000, 
		0x0cb03b65, 
		0x4fe534bf, 
		0xff9629fe, 
		0x8f7ffffd, 
		0x80000000, 
		0x80040000, 
		0x7f800000, 
		0x807ffffe, 
		0x0c7fc000, 
		0x9b218ca9, 
		0x7ae351f3, 
		0x538ec604, 
		0x40800000, 
		0xc2540000, 
		0x0c7fe000, 
		0x0e95b7c6, 
		0xc2000000, 
		0xc1d80000, 
		0xdadc521b, 
		0x40c00000, 
		0x425c0000, 
		0xc45a9cd7, 
		0x49e43ad6, 
		0x25ab778a, 
		0xc0e00000, 
		0x80008000, 
		0x00000000, 
		0xc2100000, 
		0x0e1fffff, 
		0x213206f7, 
		0xd544a722, 
		0x8003dcd4, 
		0x5d7a1257, 
		0x0023e16a, 
		0xc1b80000, 
		0xff7ffffe, 
		0x0e00003f, 
		0x97531439, 
		0xc1800000, 
		0x00000000, 
		0x80000002, 
		0x9e30b9a6, 
		0x00004000, 
		0xccb59b1c, 
		0x1cb3d001, 
		0x318448f4, 
		0x5ced6458, 
		0xff7fffff, 
		0x41c80000, 
		0x42480000, 
		0x41800000, 
		0x00004000, 
		0x41d80000, 
		0x80ffffff, 
		0x0c7ff800, 
		0x3f7dfb33, 
		0x41f80000, 
		0xc1f00000, 
		0x00100000, 
		0xc7deb857, 
		0x0e0007ff, 
		0x40e00000, 
		0x0e1fffff, 
		0x6c69e623, 
		0xd07b18cb, 
		0x537d4dcd, 
		0xbcd1e589, 
		0xcbc6f269, 
		0x826027a6, 
		0x00000000, 
		0xa5671a2b, 
		0xc2500000, 
		0x00000000, 
		0x00000100, 
		0xd4c84195, 
		0x6b8c789a, 
		0x0e3fffff, 
		0x427c0000, 
		0xffffffff, 
		0xc2140000, 
		0x0f7ffffc, 
		0x00000000, 
		0x0e07ffff, 
		0x00000000, 
		0x0e0001ff, 
		0x8e568d1c, 
		0x00800003, 
		0x422c0000, 
		0x41a00000, 
		0x80800003, 
		0xff800000, 
		0x7f800000, 
		0xdb4de8c4, 
		0xc0f9a195, 
		0x4de04acb, 
		0x00000000, 
		0x007ffffe, 
		0x00800000, 
		0xff800000, 
		0x8cf8585e, 
		0x80800003, 
		0x80011111, 
		0x0e007fff, 
		0x2de7e691, 
		0x00080000, 
		0xc1700000, 
		0x19e0a91b, 
		0x3f028f5c, 
		0x0e00001f, 
		0x52d6e303, 
		0xc2080000, 
		0x3392964a, 
		0x89a69b55, 
		0xcb000000, 
		0x2c8c6a41, 
		0xc1300000, 
		0x0f7ffffd, 
		0x5109d522, 
		0x7fbab34b, 
		0x80000000, 
		0x0014201d, 
		0xa330bade, 
		0xcb000000, 
		0xbf3b8ee9, 
		0xc1a00000, 
		0x0c600000, 
		0xff800000, 
		0x80000000, 
		0x41300000, 
		0x00800002, 
		0x00800000, 
		0xfe384610, 
		0x0e0001ff, 
		0x41d00000, 
		0x80000000, 
		0x007ffffe, 
		0x06ac056b, 
		0x00800000, 
		0x40c00000, 
		0x40c00000, 
		0x4b000000, 
		0x41c00000, 
		0xc2640000, 
		0xc1400000, 
		0x00040000, 
		0x0e0000ff, 
		0x41a00000, 
		0x3acd9ed2, 
		0xff96554c, 
		0xc2200000, 
		0xf20d3698, 
		0x00000004, 
		0x0388f526, 
		0x80000000, 
		0x80040000, 
		0x1321c6a3, 
		0x00000000, 
		0x4b8c4b40, 
		0x41c80000, 
		0xffbfffff, 
		0x42780000, 
		0x00100000, 
		0x427c0000, 
		0x0c7c0000, 
		0x0c600000, 
		0x80000000, 
		0x0d00fff0, 
		0x7a1c31ce, 
		0x41f00000, 
		0xc2280000, 
		0x00000000, 
		0x08d5db30, 
		0x00800001, 
		0xc1200000, 
		0xa3fac50b, 
		0x00000000, 
		0xf3f2d408, 
		0x00000000, 
		0xf720c172, 
		0x80800002, 
		0x42340000, 
		0x80000000, 
		0x80800003, 
		0x0c7ffe00, 
		0x00000020, 
		0xd6834d44, 
		0x2b9e306d, 
		0xc22c0000, 
		0x00800000, 
		0x80000000, 
		0x80000000, 
		0x41000000, 
		0x41700000, 
		0x33573c62, 
		0x41980000, 
		0x7bc8da00, 
		0x80000000, 
		0x41b00000, 
		0x350442e6, 
		0xcb8c4b40, 
		0xffde08e2, 
		0x807fffff, 
		0xbabd1144, 
		0x00000100, 
		0x7fc00000, 
		0x7f800000, 
		0xff800000, 
		0x80000002, 
		0x8f7ffffe, 
		0x0e1fffff, 
		0xffae5eed, 
		0xff800000, 
		0x0c7fff00, 
		0x80800003, 
		0x0c600000, 
		0xff820447, 
		0x50dbcba7, 
		0x0e0001ff, 
		0xc26c0000, 
		0xc2000000, 
		0xc1000000, 
		0x00000000, 
		0x80800002, 
		0x0e00007f, 
		0x35bba54b, 
		0x19c972c0, 
		0xc1a00000, 
		0x00000000, 
		0x80800001, 
		0x6a63f090, 
		0xb245df4f, 
		0x7f000000, 
		0x42340000, 
		0x412e1d6a, 
		0x2620069d, 
		0x3abcea2e, 
		0x8b9956b6, 
		0xff7ffffe, 
		0x41900000, 
		0xc2080000, 
		0x0d45a695, 
		0xdb2dc3d6, 
		0x0b846973, 
		0x0f7ffffc, 
		0xff800000, 
		0xffb07bd4, 
		0xcb000000, 
		0x3f800000, 
		0x00400000, 
		0xb3b383a9, 
		0xff9dbd5a, 
		0xff7fffff, 
		0x42780000, 
		0x42280000, 
		0x80000000, 
		0xff800000, 
		0x00002000, 
		0x80800000, 
		0xc1d00000, 
		0xc1500000, 
		0x03d833ca, 
		0xffa0c41f, 
		0x7f800000, 
		0xbdf9fb18, 
		0x42580000, 
		0x0c7fff80, 
		0xed3b27b9, 
		0xc2340000, 
		0xc0e00000, 
		0x754bf452, 
		0x0f7ffffe, 
		0x263f9e5f, 
		0xfecf37d7, 
		0x4a7e5869, 
		0x421c0000, 
		0x34e61545, 
		0x7f7ffffe, 
		0xc0400000, 
		0x20d9561a, 
		0xc2680000, 
		0x80000002, 
		0xc2580000, 
		0x00000000, 
		0x41600000, 
		0xc1500000, 
		0x42680000, 
		0x7f800000, 
		0xc1300000, 
		0x0f7ffffd, 
		0xc1a80000, 
		0x7757b10f, 
		0x007fffff, 
		0x7f800000, 
		0xc2000000, 
		0x80000008, 
		0xffa75ab3, 
		0x621e828d, 
		0x7f91f0f6, 
		0xc2400000, 
		0xffac8ac6, 
		0x42580000, 
		0x28fcbb43, 
		0x0c7ff000, 
		0x80000001, 
		0x7cd754c0, 
		0xbf028f5c, 
		0x5b6ae5a9, 
		0x807ffffe, 
		0x7f7ffffe, 
		0x0e00000f, 
		0x0c7fc000, 
		0x42100000, 
		0x41800000, 
		0x80000000, 
		0xc2400000, 
		0x0daaa2f3, 
		0x423c0000, 
		0x80000000, 
		0x352e2b91, 
		0x41500000, 
		0xd66f9065, 
		0x41000000, 
		0x1698ef08, 
		0xff800000, 
		0x42280000, 
		0xfbac3a9c, 
		0x3d91f72c, 
		0xc1f80000, 
		0xc20c0000, 
		0xc1c80000, 
		0x0c600000, 
		0x0f7ffffe, 
		0x8bf511f8, 
		0x80ffffff, 
		0x80000000, 
		0x0e007fff, 
		0x80800000, 
		0x8bb72e4e, 
		0x80000040, 
		0x41e80000, 
		0x8f7ffffd, 
		0xcaae25f6, 
		0x42c39a5d, 
		0x0c780000, 
		0xc1b00000, 
		0x0e003fff, 
		0xff800000, 
		0xff7fffff, 
		0xc56dd1de, 
		0x80000000, 
		0x00800001, 
		0x33333333, 
		0x80800000, 
		0x42300000, 
		0x80800000, 
		0x1580a26d, 
		0x0c7ffff0, 
		0x166469a0, 
		0x009ad877, 
		0x17c51097, 
		0x42080000, 
		0xcc364d04, 
		0x21c3f1d6, 
		0xc1000000, 
		0x00000000, 
		0xa3beecef, 
		0x80000000, 
		0x42140000, 
		0x80000000, 
		0xff800000, 
		0x8f7fffff, 
		0x00800002, 
		0x8113a3e9, 
		0x00800003, 
		0x41d80000, 
		0xe088e29a, 
		0x8a48ab05, 
		0xff7ffffe, 
		0x41f00000, 
		0x00000000, 
		0x7f7fffff, 
		0x7f7fffff, 
		0xc25c0000, 
		0x626bf7eb, 
		0x7f800000, 
		0x7fffffff, 
		0x42340000, 
		0xff9a452e, 
		0xc2480000, 
		0xc2700000, 
		0xff800000, 
		0xff7ffffe, 
		0x84017344, 
		0x00000020, 
		0x69dd85fc, 
		0x80040000, 
		0x00000002, 
		0x3868dff9, 
		0xd559d796, 
		0xc1400000, 
		0x501b4dfa, 
		0x80020000, 
		0x00000400, 
		0x00000000, 
		0xa7827917, 
		0xe6f19528, 
		0x00800002, 
		0x00000000, 
		0xc2700000, 
		0x0e001fff, 
		0x00800000, 
		0x42500000, 
		0x00400000, 
		0xc1880000, 
		0x8227e95c, 
		0xff800000, 
		0x226fd02b, 
		0xed397246, 
		0xc2040000, 
		0x0d000ff0, 
		0x0c7fffc0, 
		0x41d00000, 
		0x80000020, 
		0x13705131, 
		0x00000001, 
		0xcb000000, 
		0x00800002, 
		0xffc00000, 
		0x00800000, 
		0xb72f2130, 
		0x48244511, 
		0xf6c6409e, 
		0x0e0000ff, 
		0xc1e80000, 
		0xcb000000, 
		0x00000800, 
		0x50665d4e, 
		0x7c214897, 
		0xc1b80000, 
		0x6452cccb, 
		0x55555555, 
		0x80000000, 
		0xc2500000, 
		0xbf028f5c, 
		0x43ad13d9, 
		0xc2100000, 
		0x427c0000, 
		0x0c700000, 
		0x80000008, 
		0x80200000, 
		0x28552b1e, 
		0x7be906fa, 
		0x42700000, 
		0xd526af20, 
		0x7f800000, 
		0x80800002, 
		0x0f7ffffc, 
		0xff7fffff, 
		0x80000008, 
		0x41980000, 
		0x690b69cf, 
		0x1baf3f5c, 
		0x00011111, 
		0x42340000, 
		0xddf7edce, 
		0xc1900000, 
		0x5c2489f3, 
		0x41880000, 
		0x42740000, 
		0x80800000, 
		0xffd5d4cd, 
		0x0c7ffff0, 
		0xff800000, 
		0x41e00000, 
		0xff800000, 
		0xc1600000, 
		0x80000001, 
		0xff800000, 
		0x0c7c0000, 
		0x80000000, 
		0xc2180000, 
		0x00800001, 
		0x2fc955ab, 
		0x4a92bbc2, 
		0x00800002, 
		0xc22c0000, 
		0xe0deef13, 
		0x0027ea1d, 
		0xc1980000, 
		0x41801164, 
		0x7fb61394, 
		0xc2740000, 
		0x3ad25e6f, 
		0x7de95960, 
		0x007ffffe, 
		0x3f800000, 
		0x00000000, 
		0x7f7fffff, 
		0xff800000, 
		0xc2380000, 
		0x425c0000, 
		0x4485314c, 
		0xbe47a396, 
		0xc27c0000, 
		0xcccccccc, 
		0xd5177302, 
		0x0e003fff, 
		0xc9c0ceb5, 
		0xa0307765, 
		0x08d68548, 
		0x3f800001  
};
volatile uint8_t self_check_8[1024] __attribute__ ((aligned (2048))) = {
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01, 
		0x01  
};
volatile uint16_t self_check_16[1024] __attribute__ ((aligned (2048))) = {
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00, 
		0x3c00  
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

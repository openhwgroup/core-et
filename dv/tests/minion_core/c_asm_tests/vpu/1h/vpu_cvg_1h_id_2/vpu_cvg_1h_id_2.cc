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
			"csrwi mhpmevent3, 6\n"   
			"csrwi mhpmevent4, 21\n"   
			"csrwi mhpmevent5, 8\n"   
			"csrwi mhpmevent6, 4\n"   
			"csrwi mhpmevent7, 6\n"   
			"csrwi mhpmevent8, 3\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f10,  128 (x5)\n"
			"flw.ps f6,  544 (x5)\n"
			"flw.ps f22,  928 (x5)\n"
			"flw.ps f26,  960 (x5)\n"
			"flw.ps f1,  288 (x5)\n"
			"flw.ps f8,  864 (x5)\n"
			"flw.ps f16,  160 (x5)\n"
			"flw.ps f5,  672 (x5)\n"
			"flw.ps f4,  224 (x5)\n"
			"flw.ps f3,  192 (x5)\n"
			"flw.ps f23,  0 (x5)\n"
			"flw.ps f17,  768 (x5)\n"
			"flw.ps f14,  416 (x5)\n"
			"flw.ps f19,  32 (x5)\n"
			"flw.ps f28,  512 (x5)\n"
			"flw.ps f11,  96 (x5)\n"
			"flw.ps f29,  64 (x5)\n"
			"flw.ps f15,  992 (x5)\n"
			"flw.ps f31,  480 (x5)\n"
			"flw.ps f30,  800 (x5)\n"
			"flw.ps f27,  384 (x5)\n"
			"flw.ps f0,  608 (x5)\n"
			"flw.ps f24,  832 (x5)\n"
			"flw.ps f2,  896 (x5)\n"
			"flw.ps f21,  640 (x5)\n"
			"flw.ps f13,  352 (x5)\n"
			"flw.ps f25,  704 (x5)\n"
			"flw.ps f7,  256 (x5)\n"
			"flw.ps f12,  736 (x5)\n"
			"flw.ps f18,  320 (x5)\n"
			"flw.ps f9,  576 (x5)\n"
			"flw.ps f20,  448 (x5)\n"
			"LBL_VADDR_INIT_HID_0:\n"          
			"li x22, 0x8100686a40 \n"                 
			"fsw.ps f19, (x22) \n"                 
			"li x22, 0x81016c0000 \n"                 
			"fsw.ps f1, (x22) \n"                 
			"li x14, 0x8103da4d60 \n"                 
			"fsw.ps f7, (x14) \n"                 
			"li x18, 0x8106280000 \n"                 
			"fsw.ps f14, (x18) \n"                 
			"li x20, 0x8107bbf020 \n"                 
			"fsw.ps f27, (x20) \n"                 
			"li x17, 0x8109776000 \n"                 
			"fsw.ps f11, (x17) \n"                 
			"li x18, 0x810cc18400 \n"                 
			"fsw.ps f26, (x18) \n"                 
			"li x25, 0x810d1e0560 \n"                 
			"fsw.ps f24, (x25) \n"                 
			"li x20, 0x810d4af820 \n"                 
			"fsw.ps f22, (x20) \n"                 
			"li x11, 0x810dd20540 \n"                 
			"fsw.ps f28, (x11) \n"                 
			"li x14, 0x8200357c00 \n"                 
			"fsw.ps f29, (x14) \n"                 
			"li x25, 0x8200e83000 \n"                 
			"fsw.ps f18, (x25) \n"                 
			"li x20, 0x8201dc05c0 \n"                 
			"fsw.ps f20, (x20) \n"                 
			"li x20, 0x8204e5edc0 \n"                 
			"fsw.ps f31, (x20) \n"                 
			"li x18, 0x8205760100 \n"                 
			"fsw.ps f4, (x18) \n"                 
			"li x19, 0x8206546000 \n"                 
			"fsw.ps f7, (x19) \n"                 
			"li x12, 0x8208da0400 \n"                 
			"fsw.ps f28, (x12) \n"                 
			"li x10, 0x82095a0020 \n"                 
			"fsw.ps f20, (x10) \n"                 
			"li x16, 0x8209680240 \n"                 
			"fsw.ps f14, (x16) \n"                 
			"li x24, 0x820a360000 \n"                 
			"fsw.ps f24, (x24) \n"                 
			"li x20, 0x820ace0020 \n"                 
			"fsw.ps f21, (x20) \n"                 
			"li x16, 0x820b060740 \n"                 
			"fsw.ps f16, (x16) \n"                 
			"li x25, 0x820c83e820 \n"                 
			"fsw.ps f11, (x25) \n"                 
			"li x12, 0x820e260420 \n"                 
			"fsw.ps f9, (x12) \n"                 
			"li x11, 0x820e745820 \n"                 
			"fsw.ps f29, (x11) \n"                 
			"li x12, 0x820ee6a920 \n"                 
			"fsw.ps f10, (x12) \n"                 
			"li x19, 0x820fe78ae0 \n"                 
			"fsw.ps f21, (x19) \n"                 
			"fence\n"
			"LBL_GLOBAL_VADDR_INIT_HID_0:\n"          
			"li x10, 0x8129776000 \n"                 
			"fswg.ps f10, (x10) \n"                 
			"li x11, 0x812d1e0560 \n"                 
			"fswg.ps f3, (x11) \n"                 
			"li x15, 0x8130686a40 \n"                 
			"fswg.ps f8, (x15) \n"                 
			"li x24, 0x81316c0000 \n"                 
			"fswg.ps f16, (x24) \n"                 
			"li x20, 0x8133da4d60 \n"                 
			"fswg.ps f26, (x20) \n"                 
			"li x15, 0x8135f60000 \n"                 
			"fswg.ps f30, (x15) \n"                 
			"li x15, 0x8136280000 \n"                 
			"fswg.ps f18, (x15) \n"                 
			"li x20, 0x8137bbf020 \n"                 
			"fswg.ps f31, (x20) \n"                 
			"li x22, 0x813ada8020 \n"                 
			"fswg.ps f0, (x22) \n"                 
			"li x17, 0x8220e83000 \n"                 
			"fswg.ps f30, (x17) \n"                 
			"li x12, 0x8221dc05c0 \n"                 
			"fswg.ps f30, (x12) \n"                 
			"li x10, 0x8224c72000 \n"                 
			"fswg.ps f24, (x10) \n"                 
			"li x13, 0x8226546000 \n"                 
			"fswg.ps f26, (x13) \n"                 
			"li x21, 0x822a0a03a0 \n"                 
			"fswg.ps f13, (x21) \n"                 
			"li x21, 0x822a360000 \n"                 
			"fswg.ps f31, (x21) \n"                 
			"li x16, 0x822ace0020 \n"                 
			"fswg.ps f13, (x16) \n"                 
			"li x16, 0x822c83e820 \n"                 
			"fswg.ps f29, (x16) \n"                 
			"li x23, 0x822edf8760 \n"                 
			"fswg.ps f19, (x23) \n"                 
			"li x17, 0x822f3002e0 \n"                 
			"fswg.ps f7, (x17) \n"                 
			"li x11, 0x8230357c00 \n"                 
			"fswg.ps f3, (x11) \n"                 
			"li x14, 0x8235760100 \n"                 
			"fswg.ps f30, (x14) \n"                 
			"li x16, 0x82395a0020 \n"                 
			"fswg.ps f24, (x16) \n"                 
			"li x25, 0x823d700020 \n"                 
			"fswg.ps f30, (x25) \n"                 
			"li x12, 0x823e260420 \n"                 
			"fswg.ps f2, (x12) \n"                 
			"li x11, 0x823ee6a920 \n"                 
			"fswg.ps f0, (x11) \n"                 
			"li x25, 0x823fe78ae0 \n"                 
			"fswg.ps f22, (x25) \n"                 
			"li x21, 0x823ff87000 \n"                 
			"fswg.ps f9, (x21) \n"                 
			"fence\n"
			"LBL_LOCAL_VADDR_INIT_HID_0:\n"          
			"li x14, 0x8143020020 \n"                 
			"fswl.ps f20, (x14) \n"                 
			"li x23, 0x8143da4d60 \n"                 
			"fswl.ps f18, (x23) \n"                 
			"li x24, 0x8149776000 \n"                 
			"fswl.ps f26, (x24) \n"                 
			"li x13, 0x814d1e0560 \n"                 
			"fswl.ps f22, (x13) \n"                 
			"li x20, 0x81516c0000 \n"                 
			"fswl.ps f25, (x20) \n"                 
			"li x17, 0x8155f60000 \n"                 
			"fswl.ps f26, (x17) \n"                 
			"li x13, 0x8157bbf020 \n"                 
			"fswl.ps f15, (x13) \n"                 
			"li x14, 0x815cc18400 \n"                 
			"fswl.ps f30, (x14) \n"                 
			"li x11, 0x815dd20540 \n"                 
			"fswl.ps f4, (x11) \n"                 
			"li x16, 0x8240e83000 \n"                 
			"fswl.ps f21, (x16) \n"                 
			"li x11, 0x8245760100 \n"                 
			"fswl.ps f2, (x11) \n"                 
			"li x12, 0x8248da0400 \n"                 
			"fswl.ps f24, (x12) \n"                 
			"li x10, 0x8249680240 \n"                 
			"fswl.ps f15, (x10) \n"                 
			"li x13, 0x824a360000 \n"                 
			"fswl.ps f25, (x13) \n"                 
			"li x21, 0x8250357c00 \n"                 
			"fswl.ps f31, (x21) \n"                 
			"li x14, 0x8251dc05c0 \n"                 
			"fswl.ps f29, (x14) \n"                 
			"li x12, 0x8253120020 \n"                 
			"fswl.ps f18, (x12) \n"                 
			"li x12, 0x82538c0000 \n"                 
			"fswl.ps f17, (x12) \n"                 
			"li x25, 0x8254e5edc0 \n"                 
			"fswl.ps f19, (x25) \n"                 
			"li x10, 0x8256546000 \n"                 
			"fswl.ps f7, (x10) \n"                 
			"li x22, 0x82595a0020 \n"                 
			"fswl.ps f26, (x22) \n"                 
			"li x13, 0x825a0a03a0 \n"                 
			"fswl.ps f3, (x13) \n"                 
			"li x10, 0x825ace0020 \n"                 
			"fswl.ps f22, (x10) \n"                 
			"li x25, 0x825b060740 \n"                 
			"fswl.ps f25, (x25) \n"                 
			"li x18, 0x825b83d560 \n"                 
			"fswl.ps f8, (x18) \n"                 
			"li x12, 0x825f3002e0 \n"                 
			"fswl.ps f4, (x12) \n"                 
			"li x25, 0x825ff87000 \n"                 
			"fswl.ps f0, (x25) \n"                 
			"fence\n"
			"fence\n"
			"csrw fflags, zero\n"
			"csrwi frm, 0x0\n"
			"LBL_KERNEL_HID_0:\n"          
			"li x5, 0xFEEDFEED\n"
			"li x0, 0x56341e8940\n" 
			"slti x0, x0, 0x7f2\n"
			"la x31, check_sum_hid_0\n"
			"li x5, 0x5\n" 
			"sw x5, (x31)\n"
			"li x29, 0x7\n"
			"LBL_OUTER_LOOP_SEQ_HID_0:\n"
			"nop \n"
			"nop \n"
			"li x30, 0\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_0_INT_LOAD_ID_0_HID_0:\n"
			"li x23, 0x8204e5edcb\n" 
			"sd x5, -899(x23)\n"
			"lbu x25, -899(x23)\n" 
			"fsll.pi f26, f26, f18\n"
			"fsub.ps f22, f2, f2, rdn\n"
			"bge x23, x24, 1f\n"
			"1:\n"
			"LBL_SEQID_0_FP_SCATTER_ID_1_HID_0:\n"
			"li x5, 0x820b060754\n" 
			"li x15, 0x23775b73a8160d38\n"
			"fsc32w.ps f22, x15 (x5)\n" 
			"fmul.ps f22, f10, f18, rtz\n"
			"fsgnjx.ps f22, f9, f10\n"
			"LBL_SEQID_0_FP_L_GATHER_ID_2_HID_0:\n"
			"li x10, 0x8149776018\n" 
			"la x28, mem_ptr_offset\n"
			"flq2 f17, 180(x28)\n"
			"fscwl.ps f15, f17 (x10)\n"
			"fgbl.ps f9, f17 (x10)\n" 
			"fmulh.pi f15, f17, f0\n"
			"beq x15, x21, 1f\n"
			"1:\n"
			"fnmadd.ps f0, f2, f26, f10, rtz\n"
			"LBL_SEQID_0_INT_LOAD_ID_3_HID_0:\n"
			"li x15, 0x8200e8301b\n" 
			"sd x5, -377(x15)\n"
			"lb x6, -377(x15)\n" 
			"feq.ps f20, f15, f0\n"
			"fswizz.ps f26, f26, 0x2\n"
			"LBL_SEQID_0_INT_G_STORE_ID_4_HID_0:\n"
			"li x10, 0x823d70002c\n" 
			"shg x25, (x10)\n" 
			"fmax.pi f11, f12, f22\n"
			"bne x29, x16, 1f\n"
			"1:\n"
			"fsub.ps f17, f22, f17, rne\n"
			"LBL_SEQID_0_INT_AMO_ID_5_HID_0:\n"
			"li x23, 0x8121f2f380\n" 
			"amoswapg.d x13, x13, (x23)\n" 
			"maskpopc x15, m2\n"
			"fmvs.x.ps x23, f2, 0\n"
			"LBL_SEQID_0_INT_AMO_ID_6_HID_0:\n"
			"li x25, 0x82238c0000\n" 
			"amoandg.d x7, x17, (x25)\n" 
			"fle.ps f3, f25, f10\n"
			"fsrl.pi f18, f15, f18\n"
			"LBL_SEQID_0_FP_SCATTER_ID_7_HID_0:\n"
			"li x6, 0x820c83e823\n" 
			"la x5, mem_ptr_offset\n"
			"flw.ps f9, 188(x5)\n"
			"fscb.ps f8, f9 (x6)\n" 
			"sllw x17, x5, x7\n"
			"fnot.pi f18, f0\n"
			"LBL_SEQID_0_INT_STORE_ID_8_HID_0:\n"
			"li x5, 0x8208da040b\n" 
			"sh x7, -1432(x5)\n" 
			"fadd.ps f11, f15, f18, rup\n"
			"fmax.s f9, f9, f26\n"
			"LBL_SEQID_0_INT_AMO_ID_9_HID_0:\n"
			"li x6, 0x812d4af828\n" 
			"amomaxg.w x25, x14, (x6)\n" 
			"flt.ps f20, f11, f20\n"
			"fmax.ps f18, f20, f20\n"
			"LBL_SEQID_0_FP_G_GATHER_ID_10_HID_0:\n"
			"li x17, 0x8235760100\n" 
			"la x25, mem_ptr_offset\n"
			"flw.ps f17, 164(x25)\n"
			"fscwg.ps f31, f17 (x17)\n"
			"fghg.ps f4, f17 (x17)\n" 
			"fmin.ps f9, f2, f15\n"
			"fmadd.s f3, f3, f17, f2, rmm\n"
			"LBL_SEQID_0_FP_AMO_ID_11_HID_0:\n"
			"li x15, 0x823d70002c\n" 
			"la x14, mem_ptr_offset\n"
			"flw.ps f9, 192(x14)\n"
			"famoming.ps f12, f9 (x15)\n" 
			"for.pi f17, f17, f22\n"
			"fsrli.pi f15, f15, 0x4\n"
			"LBL_SEQID_0_FP_LOAD_ID_12_HID_0:\n"
			"li x10, 0x820312003c\n" 
			"fsw.ps f12, -423(x10)\n"
			"flw f3, -423(x10)\n" 
			"faddi.pi f10, f8, 0x167\n"
			"xor x15, x6, x14\n"
			"bge x28, x0, 1f\n"
			"1:\n"
			"LBL_SEQID_0_INT_AMO_ID_13_HID_0:\n"
			"li x17, 0x825ace0020\n" 
			"amominl.d x7, x6, (x17)\n" 
			"fexp.ps f25, f12\n"
			"sll x17, x15, x25\n"
			"LBL_SEQID_0_FP_L_SCATTER_ID_14_HID_0:\n"
			"li x6, 0x825e260420\n" 
			"la x13, mem_ptr_offset\n"
			"flw.ps f9, 132(x13)\n"
			"fscwl.ps f4, f9 (x6)\n" 
			"fsetm.pi m4, f17\n"
			"for.pi f12, f0, f0\n"
			"LBL_SEQID_0_INT_AMO_ID_15_HID_0:\n"
			"li x13, 0x812d4af828\n" 
			"amoming.d x7, x15, (x13)\n" 
			"mulw x17, x15, x23\n"
			"bgeu x31, x31, 1f\n"
			"1:\n"
			"sraiw x5, x7, 3\n"
			"LBL_SEQID_0_FP_AMO_ID_16_HID_0:\n"
			"li x13, 0x825c83e820\n" 
			"la x28, mem_ptr_offset\n"
			"flw.ps f12, 64(x28)\n"
			"famoaddl.pi f9, f12 (x13)\n" 
			"fpackrepb.pi f10, f25\n"
			"srliw x5, x7, 5\n"
			"LBL_SEQID_0_INT_STORE_ID_17_HID_0:\n"
			"li x7, 0x810cc1841b\n" 
			"sw x5, -314(x7)\n" 
			"fsrl.pi f9, f20, f15\n"
			"blt x31, x23, 1f\n"
			"1:\n"
			"fsatu8.pi f2, f3\n"
			"bgeu x21, x17, 1f\n"
			"1:\n"
			"LBL_SEQID_0_FP_SCATTER_ID_18_HID_0:\n"
			"li x23, 0x8201dc05dc\n" 
			"li x28, 0x72030f100ed83fda\n"
			"fsc32h.ps f4, x28 (x23)\n" 
			"fnmadd.ps f22, f8, f22, f3, rup\n"
			"fandi.pi f17, f17, 0x149\n"
			"LBL_SEQID_0_INT_AMO_ID_19_HID_0:\n"
			"li x15, 0x8123020030\n" 
			"amominug.d x7, x6, (x15)\n" 
			"fsra.pi f2, f20, f2\n"
			"fnmsub.ps f22, f22, f20, f12, rup\n"
			"LBL_SEQID_0_INT_STORE_ID_20_HID_0:\n"
			"li x14, 0x820b060754\n" 
			"sd x10, 1147(x14)\n" 
			"fmadd.s f18, f22, f18, f18, rdn\n"
			"fle.pi f8, f25, f0\n"
			"LBL_SEQID_0_CSR_ID_21_HID_0:\n"
			"li x10, 0x800000820edf874a\n"
			"li x31, (0x3 << 6) | 0x1\n"
			"csrw flush_va, x10\n" 
			"csrwi tensor_wait, 6\n" 
			"fsgnjn.ps f12, f12, f25\n"
			"bne x14, x1, 1f\n"
			"1:\n"
			"divu x6, x15, x6\n"
			"blt x11, x20, 1f\n"
			"1:\n"
			"LBL_SEQID_0_FP_AMO_ID_22_HID_0:\n"
			"li x25, 0x8149776018\n" 
			"la x28, mem_ptr_offset\n"
			"flw.ps f10, 384(x28)\n"
			"famoandl.pi f2, f10 (x25)\n" 
			"fandi.pi f0, f10, 0x86\n"
			"fsgnjn.s f4, f4, f11\n"
			"LBL_SEQID_0_INT_LOAD_ID_23_HID_0:\n"
			"li x23, 0x810cc1841b\n" 
			"sd x6, -1032(x23)\n"
			"lw x5, -1032(x23)\n" 
			"divuw x14, x14, x14\n"
			"flt.ps f9, f9, f9\n"
			"LBL_SEQID_0_INT_AMO_ID_24_HID_0:\n"
			"li x14, 0x82238c0004\n" 
			"amoswapg.w x5, x28, (x14)\n" 
			"sltiu x23, x23, 0x0\n"
			"rem x15, x15, x17\n"
			"LBL_SEQID_0_INT_AMO_ID_25_HID_0:\n"
			"li x10, 0x824fe78af8\n" 
			"amoorl.w x28, x23, (x10)\n" 
			"fsat8.pi f12, f3\n"
			"bgeu x18, x0, 1f\n"
			"1:\n"
			"fle.pi f20, f20, f11\n"
			"LBL_SEQID_0_INT_AMO_ID_26_HID_0:\n"
			"li x7, 0x822b83d56c\n" 
			"amoandg.w x15, x23, (x7)\n" 
			"maskor m2, m7, m4\n"
			"fsrl.pi f15, f9, f17\n"
			"LBL_SEQID_0_INT_G_STORE_ID_27_HID_0:\n"
			"li x15, 0x8229680250\n" 
			"sbg x7, (x15)\n" 
			"fclass.ps f25, f22\n"
			"fmaxu.pi f22, f26, f15\n"
			"LBL_SEQID_0_INT_AMO_ID_28_HID_0:\n"
			"li x14, 0x823e260420\n" 
			"amoaddg.d x13, x13, (x14)\n" 
			"srli x17, x17, 4\n"
			"remuw x15, x7, x15\n"
			"bgeu x13, x31, 1f\n"
			"1:\n"
			"LBL_SEQID_0_FP_G_GATHER_ID_29_HID_0:\n"
			"li x28, 0x823e745820\n" 
			"la x25, mem_ptr_offset\n"
			"flw.ps f9, 108(x25)\n"
			"fscwg.ps f0, f9 (x28)\n"
			"fgbg.ps f18, f9 (x28)\n" 
			"fmadd.ps f0, f18, f17, f3, rup\n"
			"fsatu8.pi f9, f0\n"
			"la x13, rand_ieee754_32\n"
			"flw.ps f0, 320(x13)\n"
			"fcvt.lu.s x23, f0, rne\n" 
			"fle.pi f0, f3, f25\n"
			"blt x29, x30, 1f\n"
			"1:\n"
			"fmadd.s f10, f12, f2, f10, rmm\n"
			"LBL_SEQID_0_INT_AMO_ID_30_HID_0:\n"
			"li x14, 0x8256546008\n" 
			"amoswapl.w x13, x10, (x14)\n" 
			"xori x13, x15, 0x1\n"
			"frcp.ps f12, f3\n"
			"mov.m.x m0, x0, 0xFF\n"
			"li x5, 0x8\n"
			"la x31, rand_ieee754_32\n"
			"add x31, x31, x5\n"
			"flw.ps f0, 0(x31)\n"
			"flw.ps f10, 1(x31)\n"
			"flw.ps f11, 7(x31)\n"
			"flw.ps f12, 0(x31)\n"
			"flw.ps f15, 15(x31)\n"
			"flw.ps f17, 0(x31)\n"
			"flw.ps f18, 0(x31)\n"
			"flw.ps f2, 21(x31)\n"
			"flw.ps f20, 32(x31)\n"
			"flw.ps f22, 18(x31)\n"
			"flw.ps f25, 2(x31)\n"
			"flw.ps f26, 19(x31)\n"
			"flw.ps f3, 47(x31)\n"
			"flw.ps f4, 46(x31)\n"
			"flw.ps f8, 22(x31)\n"
			"flw.ps f9, 22(x31)\n"
			"li x21, 0x1\n"
			"LBL_SEQID_0_LOOP_SEQ_HID_0:\n"
			"fmaxu.pi f18, f11, f0\n"
			"bltu x29, x28, 1f\n"
			"1:\n"
			"fsgnj.s f8, f11, f20\n"
			"fnot.pi f22, f22\n"
			"bltu x15, x30, 1f\n"
			"1:\n"
			"fcmovm.ps f4, f9, f20\n"
			"fsat8.pi f22, f9\n"
			"fadd.ps f25, f25, f15, rup\n"
			"fround.ps f15, f20, rdn\n"
			"fle.ps f4, f22, f11\n"
			"beq x27, x24, 1f\n"
			"1:\n"
			"fnot.pi f4, f25\n"
			"flt.pi f22, f8, f9\n"
			"bne x2, x31, 1f\n"
			"1:\n"
			"fsat8.pi f26, f20\n"
			"fmin.s f26, f18, f18\n"
			"flt.pi f0, f22, f0\n"
			"fnot.pi f4, f26\n"
			"fadd.pi f20, f20, f18\n"
			"fmul.s f26, f9, f3, rdn\n"
			"blt x12, x13, 1f\n"
			"1:\n"
			"fxor.pi f0, f0, f10\n"
			"bne x0, x30, 1f\n"
			"1:\n"
			"fand.pi f17, f17, f20\n"
			"fclass.ps f18, f26\n"
			"blt x21, x17, 1f\n"
			"1:\n"
			"feq.pi f12, f20, f12\n"
			"feq.ps f15, f3, f15\n"
			"fmax.s f4, f4, f11\n"
			"fpackreph.pi f25, f9\n"
			"bne x20, x23, 1f\n"
			"1:\n"
			"fmul.pi f12, f11, f10\n"
			"flt.ps f9, f17, f9\n"
			"fadd.s f11, f2, f9, rup\n"
			"fsrl.pi f26, f12, f26\n"
			"fsgnj.ps f3, f26, f17\n"
			"fsub.pi f20, f15, f15\n"
			"bltu x6, x13, 1f\n"
			"1:\n"
			"fsub.pi f25, f8, f15\n"
			"fsub.pi f3, f10, f17\n"
			"fandi.pi f4, f10, 0x5b\n"
			"fmadd.s f22, f4, f18, f20, rmm\n"
			"fxor.pi f9, f10, f2\n"
			"fcmovm.ps f0, f0, f22\n"
			"fand.pi f20, f17, f2\n"
			"fpackreph.pi f11, f11\n"
			"fmin.pi f10, f25, f4\n"
			"fminu.pi f4, f3, f26\n"
			"fcvt.pw.ps f10, f18, rtz\n"
			"fcvt.pw.ps f8, f12, rtz\n"
			"fsgnjn.ps f18, f20, f18\n"
			"fnmadd.ps f26, f26, f11, f4, rdn\n"
			"fcvt.ps.pw f22, f22, rup\n"
			"fcvt.ps.pwu f22, f20, rdn\n"
			"fsll.pi f25, f8, f20\n"
			"fsatu8.pi f3, f18\n"
			"fcvt.pwu.ps f17, f20, rne\n"
			"fsgnj.ps f17, f4, f18\n"
			"fsll.pi f3, f11, f10\n"
			"fmadd.s f4, f9, f3, f18, rtz\n"
			"bge x11, x3, 1f\n"
			"1:\n"
			"fcmovm.ps f3, f2, f25\n"
			"fandi.pi f25, f2, 0x25\n"
			"fpackrepb.pi f20, f20\n"
			"feq.pi f12, f25, f15\n"
			"fmul.s f22, f22, f20, rup\n"
			"bge x26, x28, 1f\n"
			"1:\n"
			"flog.ps f8, f15\n"
			"feq.ps f2, f11, f8\n"
			"beq x9, x4, 1f\n"
			"1:\n"
			"fcvt.ps.pwu f18, f3, rne\n"
			"bgeu x2, x23, 1f\n"
			"1:\n"
			"fmadd.ps f4, f2, f0, f12, rdn\n"
			"fmax.ps f2, f9, f0\n"
			"fmulh.pi f2, f0, f15\n"
			"fpackrepb.pi f10, f12\n"
			"fsat8.pi f12, f12\n"
			"fmin.pi f11, f4, f17\n"
			"fsgnjx.ps f20, f22, f9\n"
			"fpackreph.pi f18, f10\n"
			"fcvt.ps.pw f8, f18, rup\n"
			"fmadd.s f20, f20, f0, f2, rne\n"
			"fmin.ps f20, f20, f11\n"
			"fsgnjn.ps f2, f3, f17\n"
			"beq x26, x0, 1f\n"
			"1:\n"
			"fsub.ps f12, f8, f20, rdn\n"
			"fsrl.pi f25, f25, f17\n"
			"bltu x15, x13, 1f\n"
			"1:\n"
			"fsll.pi f22, f22, f25\n"
			"fmin.s f3, f22, f3\n"
			"fsgnjx.s f4, f4, f9\n"
			"bltu x8, x30, 1f\n"
			"1:\n"
			"fsgnjx.ps f0, f12, f0\n"
			"fsin.ps f11, f4\n"
			"fsrli.pi f0, f12, 0x8\n"
			"flt.ps f15, f22, f15\n"
			"fsgnj.s f2, f26, f4\n"
			"fle.pi f4, f2, f4\n"
			"flog.ps f8, f2\n"
			"fclass.ps f11, f20\n"
			"fnmsub.s f2, f2, f26, f20, rup\n"
			"fsat8.pi f8, f17\n"
			"fsra.pi f3, f22, f4\n"
			"fsatu8.pi f3, f18\n"
			"fltu.pi f26, f18, f20\n"
			"fsrl.pi f26, f12, f3\n"
			"fandi.pi f17, f17, 0x137\n"
			"bge x27, x22, 1f\n"
			"1:\n"
			"fmul.pi f25, f4, f25\n"
			"fsrai.pi f15, f15, 0x3\n"
			"bne x15, x12, 1f\n"
			"1:\n"
			"fsrl.pi f9, f0, f9\n"
			"fsgnj.ps f18, f25, f3\n"
			"fsatu8.pi f25, f9\n"
			"bgeu x0, x13, 1f\n"
			"1:\n"
			"fsin.ps f3, f22\n"
			"fadd.pi f0, f22, f0\n"
			"fmul.ps f2, f18, f0, rtz\n"
			"bgeu x16, x3, 1f\n"
			"1:\n"
			"fcmov.ps f9, f9, f18, f17\n"
			"fmul.ps f25, f15, f0, rne\n"
			"fsin.ps f0, f12\n"
			"fminu.pi f15, f0, f20\n"
			"frcp.ps f26, f2\n"
			"fsgnjn.ps f11, f3, f10\n"
			"fsrai.pi f0, f11, 0x8\n"
			"blt x26, x22, 1f\n"
			"1:\n"
			"fmadd.ps f2, f25, f2, f2, rmm\n"
			"fswizz.ps f3, f3, 0x12\n"
			"fpackrepb.pi f26, f3\n"
			"bltu x20, x4, 1f\n"
			"1:\n"
			"fand.pi f2, f2, f12\n"
			"flt.ps f25, f11, f12\n"
			"fsra.pi f10, f18, f4\n"
			"fsgnj.ps f2, f26, f20\n"
			"fnmadd.ps f18, f18, f11, f10, rdn\n"
			"fcmov.ps f4, f17, f11, f18\n"
			"fsub.s f8, f26, f11, rne\n"
			"fclass.ps f25, f10\n"
			"fmadd.ps f0, f0, f25, f4, rne\n"
			"fcvt.ps.f16 f26, f26\n"
			"fmul.s f0, f4, f9, rne\n"
			"fsub.ps f20, f4, f11, rmm\n"
			"blt x31, x12, 1f\n"
			"1:\n"
			"fsll.pi f0, f0, f3\n"
			"fand.pi f10, f8, f18\n"
			"fmax.ps f20, f12, f26\n"
			"beq x13, x14, 1f\n"
			"1:\n"
			"fcvt.ps.f16 f20, f20\n"
			"fsrl.pi f17, f22, f18\n"
			"fcmovm.ps f18, f3, f0\n"
			"flog.ps f3, f17\n"
			"feq.pi f18, f22, f15\n"
			"fmin.pi f26, f25, f4\n"
			"fmulhu.pi f2, f8, f10\n"
			"beq x22, x0, 1f\n"
			"1:\n"
			"fslli.pi f0, f8, 0xc\n"
			"fnmsub.ps f25, f17, f15, f8, rmm\n"
			"fsra.pi f9, f15, f8\n"
			"flt.ps f8, f3, f8\n"
			"beq x20, x4, 1f\n"
			"1:\n"
			"fswizz.ps f9, f20, 0x1e\n"
			"fsgnjx.ps f18, f15, f22\n"
			"fmsub.s f0, f15, f18, f0, rdn\n"
			"bge x24, x29, 1f\n"
			"1:\n"
			"fsrli.pi f8, f17, 0x6\n"
			"fsub.s f11, f3, f15, rmm\n"
			"fclass.ps f9, f2\n"
			"flt.pi f15, f10, f0\n"
			"fmadd.s f0, f0, f11, f20, rtz\n"
			"fandi.pi f8, f17, 0x14f\n"
			"fnmadd.ps f10, f18, f22, f12, rmm\n"
			"bge x31, x11, 1f\n"
			"1:\n"
			"fmin.ps f20, f26, f20\n"
			"feq.ps f0, f15, f9\n"
			"fcvt.pw.ps f11, f12, rmm\n"
			"fsub.pi f3, f26, f12\n"
			"faddi.pi f17, f17, 0x10a\n"
			"fsatu8.pi f26, f8\n"
			"fsgnj.s f11, f3, f12\n"
			"fsgnj.s f11, f26, f26\n"
			"fcvt.f16.ps f8, f0\n"
			"beq x24, x13, 1f\n"
			"1:\n"
			"flt.pi f15, f9, f15\n"
			"fltu.pi f2, f8, f2\n"
			"beq x21, x12, 1f\n"
			"1:\n"
			"fsgnjx.s f10, f4, f3\n"
			"fnmadd.s f11, f12, f4, f11, rup\n"
			"bgeu x31, x30, 1f\n"
			"1:\n"
			"fcvt.pw.ps f18, f11, rne\n"
			"bne x31, x4, 1f\n"
			"1:\n"
			"fadd.ps f8, f12, f2, rne\n"
			"addi x21, x21, -1\n"
			"bne x21, x0, LBL_SEQID_0_LOOP_SEQ_HID_0\n"
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_0\n"
			"addi x30, x30, 1\n"
			"la x5, rand_ieee754_16\n"
			"flw.ps f18, 256(x5)\n"
			"fcvt.ps.f16 f9, f18\n" 
			"xor x25, x25, x5\n"
			"fswizz.ps f11, f10, 0x16\n"
			"fsub.pi f10, f3, f17\n"
			"bge x11, x10, 1f\n"
			"1:\n"
			"ori x23, x25, 0xc\n"
			"fnot.pi f18, f3\n"
			"fsgnjn.ps f8, f8, f15\n"
			"blt x28, x17, 1f\n"
			"1:\n"
			"fsrli.pi f22, f9, 0xe\n"
			"fmul.pi f17, f20, f17\n"
			"li x15, 0xb1fa170154de14ea\n"
			"ori x23, x15, 0x0\n" 
			"mulh x6, x14, x14\n"
			"blt x21, x12, 1f\n"
			"1:\n"
			"srliw x13, x7, 1\n"
			"fmadd.ps f2, f26, f2, f25, rdn\n"
			"fmul.pi f4, f15, f4\n"
			"sub x23, x7, x25\n"
			"srli x10, x10, 1\n"
			"remw x14, x14, x14\n"
			"fsgnjx.ps f11, f15, f4\n"
			"la x25, rand_ieee754_32\n"
			"flw.ps f18, 480(x25)\n"
			"flw.ps f25, 608(x25)\n"
			"fle.s x6, f18, f25\n" 
			"sltu x13, x13, x14\n"
			"bge x27, x24, 1f\n"
			"1:\n"
			"subw x13, x28, x13\n"
			"fsub.s f17, f17, f4, rtz\n"
			"fcvt.ps.pw f3, f3, rtz\n"
			"or x14, x14, x14\n"
			"fmax.s f3, f15, f17\n"
			"fsin.ps f11, f4\n"
			"fle.pi f25, f15, f4\n"
			"li x13, 0x555198d59bd8c521\n"
			"srli x10, x13, 3\n" 
			"fmul.ps f22, f3, f0, rmm\n"
			"fmin.ps f20, f12, f17\n"
			"fsgnjx.ps f22, f8, f0\n"
			"divu x25, x7, x25\n"
			"fmul.s f11, f11, f25, rmm\n"
			"packb x10, x15, x6\n"
			"frsq.ps f22, f10\n"
			"bge x0, x4, 1f\n"
			"1:\n"
			"div x28, x23, x28\n"
			"li x14, 0x58b606cf7b8815d1\n"
			"li x5, 0x9dd681041c1fb182\n"
			"sltu x17, x14, x5\n" 
			"fclass.ps f22, f0\n"
			"fminu.pi f17, f2, f0\n"
			"fminu.pi f25, f3, f2\n"
			"flt.ps f0, f8, f9\n"
			"fnmadd.s f11, f0, f11, f9, rup\n"
			"fmulhu.pi f15, f8, f0\n"
			"beq x18, x21, 1f\n"
			"1:\n"
			"fsrai.pi f17, f12, 0xd\n"
			"fmax.s f9, f10, f22\n"
			"la x28, rand_int_32\n"
			"flw.ps f12, 448(x28)\n"
			"flw.ps f2, 768(x28)\n"
			"fsrl.pi f17, f12, f2\n" 
			"fcvt.f16.ps f9, f3\n"
			"bge x30, x10, 1f\n"
			"1:\n"
			"mulw x7, x5, x7\n"
			"maskor m5, m5, m5\n"
			"mulhu x7, x13, x14\n"
			"fmul.s f12, f2, f15, rtz\n"
			"bne x5, x13, 1f\n"
			"1:\n"
			"fsat8.pi f26, f18\n"
			"fmax.ps f18, f3, f15\n"
			"divuw x13, x5, x17\n"
			"la x25, rand_ieee754_32\n"
			"flw.ps f12, 64(x25)\n"
			"flw.ps f25, 800(x25)\n"
			"fmadd.s f17, f12, f25, f12, rmm\n" 
			"fpackrepb.pi f11, f17\n"
			"fmax.s f8, f26, f9\n"
			"fcmov.ps f15, f25, f10, f4\n"
			"fmadd.s f12, f17, f12, f8, rne\n"
			"fsra.pi f20, f3, f3\n"
			"feq.pi f12, f9, f12\n"
			"fle.pi f10, f10, f17\n"
			"bge x10, x21, 1f\n"
			"1:\n"
			"fswizz.ps f3, f11, 0x15\n"
			"la x25, rand_int_32\n"
			"flw.ps f12, 480(x25)\n"
			"flw.ps f15, 640(x25)\n"
			"fltu.pi f25, f12, f15\n" 
			"fminu.pi f26, f26, f11\n"
			"bgeu x9, x29, 1f\n"
			"1:\n"
			"fmul.ps f9, f4, f12, rne\n"
			"fpackreph.pi f2, f4\n"
			"fsub.ps f8, f15, f4, rmm\n"
			"fsub.pi f18, f9, f3\n"
			"fminu.pi f10, f2, f2\n"
			"bge x22, x26, 1f\n"
			"1:\n"
			"faddi.pi f8, f3, 0xf7\n"
			"fsgnjn.ps f10, f9, f11\n"
			"bge x16, x13, 1f\n"
			"1:\n"
			"la x23, rand_int_32\n"
			"flw.ps f12, 0(x23)\n"
			"flw.ps f4, 0(x23)\n"
			"fmax.pi f12, f12, f4\n" 
			"fnmadd.ps f25, f26, f18, f15, rne\n"
			"fnmadd.s f20, f8, f18, f10, rdn\n"
			"fminu.pi f25, f25, f18\n"
			"fnmsub.s f2, f3, f18, f22, rtz\n"
			"fsgnj.s f18, f20, f18\n"
			"blt x19, x14, 1f\n"
			"1:\n"
			"fcvt.f16.ps f20, f20\n"
			"fcmov.ps f8, f18, f15, f8\n"
			"fcvt.f16.ps f12, f20\n"
			"LBL_SEQID_1_M0_WRITE_ID_31_HID_0:\n"
			"maskxor m2, m7, m7\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"fmin.pi f15, f25, f25\n"
			"fminu.pi f15, f10, f4\n"
			"feq.ps f18, f11, f8\n"
			"srlw x13, x17, x25\n"
			"blt x15, x28, 1f\n"
			"1:\n"
			"fsgnj.s f20, f26, f18\n"
			"feq.ps f20, f26, f18\n"
			"bltu x13, x11, 1f\n"
			"1:\n"
			"feq.pi f20, f25, f20\n"
			"fcmovm.ps f20, f18, f11\n"
			"LBL_SEQID_1_INT_DIV_ID_32_HID_0:\n"
			"div x6, x17, x15\n" 
			"sll x15, x23, x10\n"
			"slliw x5, x14, 2\n"
			"srliw x6, x5, 0\n"
			"blt x3, x18, 1f\n"
			"1:\n"
			"srl x14, x15, x13\n"
			"fsll.pi f25, f15, f25\n"
			"fsub.pi f0, f26, f10\n"
			"fandi.pi f18, f25, 0x8e\n"
			"fclass.ps f11, f4\n"
			"beq x20, x4, 1f\n"
			"1:\n"
			"la x5, rand_int_32\n"
			"flw.ps f25, 608(x5)\n"
			"flw.ps f4, 448(x5)\n"
			"fsll.pi f11, f25, f4\n" 
			"fclass.ps f26, f15\n"
			"bge x19, x17, 1f\n"
			"1:\n"
			"maskor m5, m4, m2\n"
			"fsra.pi f0, f4, f4\n"
			"sll x13, x17, x17\n"
			"frsq.ps f3, f18\n"
			"fle.ps f20, f15, f20\n"
			"divw x14, x14, x10\n"
			"maskor m4, m4, m7\n"
			"beq x22, x16, 1f\n"
			"1:\n"
			"la x17, rand_ieee754_32\n"
			"flw.ps f0, 480(x17)\n"
			"flw.ps f12, 864(x17)\n"
			"flw.ps f8, 960(x17)\n"
			"fmadd.s f4, f0, f12, f8, rmm\n" 
			"fltm.pi m7, f18, f2\n"
			"auipc x23, 0x2\n"
			"fbci.ps f26, 0x125be\n"
			"flem.ps m4, f20, f8\n"
			"bgeu x30, x4, 1f\n"
			"1:\n"
			"fandi.pi f8, f8, 0x10\n"
			"fsgnjx.ps f0, f0, f8\n"
			"fmin.ps f15, f15, f2\n"
			"fsgnj.s f2, f2, f8\n"
			"li x28, 0x264a013bc0366a08\n"
			"srai x10, x28, 0\n" 
			"fmax.pi f12, f18, f10\n"
			"sraw x10, x10, x10\n"
			"srai x5, x13, 5\n"
			"fsgnjn.ps f22, f15, f18\n"
			"fcvt.ps.pw f26, f26, rmm\n"
			"bge x9, x2, 1f\n"
			"1:\n"
			"slti x14, x10, 0x6\n"
			"fadd.ps f15, f12, f15, rmm\n"
			"flt.ps f25, f9, f8\n"
			"la x6, rand_ieee754_32\n"
			"flw.ps f15, 448(x6)\n"
			"fcvt.pwu.ps f18, f15, rup\n" 
			"fsrli.pi f12, f4, 0xf\n"
			"fpackreph.pi f17, f22\n"
			"addiw x14, x14, 0x10\n"
			"for.pi f25, f11, f25\n"
			"fsrl.pi f15, f2, f15\n"
			"fmax.s f15, f11, f15\n"
			"fmvs.x.ps x5, f12, 7\n"
			"feq.pi f20, f20, f11\n"
			"la x10, rand_int_32\n"
			"flw.ps f10, 672(x10)\n"
			"flw.ps f8, 192(x10)\n"
			"fsra.pi f15, f10, f8\n" 
			"or x7, x17, x25\n"
			"feqm.ps m4, f18, f3\n"
			"mov.m.x m5, x7, 0x53\n"
			"fsgnjn.s f18, f3, f20\n"
			"or x25, x25, x14\n"
			"fsrl.pi f18, f9, f20\n"
			"fsgnjx.ps f3, f3, f20\n"
			"flog.ps f17, f9\n"
			"la x5, rand_ieee754_32\n"
			"flw.ps f9, 288(x5)\n"
			"fcvt.pwu.ps f18, f9, rup\n" 
			"fcvt.ps.f16 f25, f22\n"
			"bltu x31, x17, 1f\n"
			"1:\n"
			"fmax.s f11, f11, f20\n"
			"fmin.ps f25, f20, f20\n"
			"fand.pi f3, f11, f11\n"
			"fmin.s f22, f22, f20\n"
			"fpackreph.pi f20, f11\n"
			"fle.ps f0, f22, f17\n"
			"fandi.pi f25, f11, 0x1dd\n"
			"la x7, rand_int_32\n"
			"flw.ps f17, 448(x7)\n"
			"flw.ps f25, 256(x7)\n"
			"fle.pi f2, f17, f25\n" 
			"fmul.s f18, f18, f20, rmm\n"
			"fmvs.x.ps x7, f20, 6\n"
			"fcvt.ps.pw f9, f20, rtz\n"
			"fmulh.pi f18, f15, f11\n"
			"maskpopc x23, m2\n"
			"addiw x28, x10, 0x17\n"
			"slt x23, x17, x15\n"
			"fmax.s f17, f20, f15\n"
			"bne x4, x27, 1f\n"
			"1:\n"
			"la x14, rand_ieee754_32\n"
			"flw.ps f17, 544(x14)\n"
			"flw.ps f9, 320(x14)\n"
			"fsub.ps f10, f17, f9, rdn\n" 
			"fcvt.ps.f16 f0, f25\n"
			"beq x29, x0, 1f\n"
			"1:\n"
			"fsll.pi f15, f8, f9\n"
			"fadd.pi f4, f4, f0\n"
			"bne x24, x6, 1f\n"
			"1:\n"
			"fsgnjx.s f4, f10, f10\n"
			"fsgnjn.ps f12, f9, f10\n"
			"bne x30, x20, 1f\n"
			"1:\n"
			"fnmsub.ps f15, f4, f18, f15, rne\n"
			"bltu x12, x12, 1f\n"
			"1:\n"
			"fnmadd.s f8, f4, f8, f8, rup\n"
			"fnmsub.s f11, f22, f22, f9, rne\n"
			"la x15, rand_ieee754_32\n"
			"flw.ps f11, 256(x15)\n"
			"flw.ps f26, 160(x15)\n"
			"fadd.ps f2, f11, f26, rdn\n" 
			"fmul.s f8, f8, f0, rmm\n"
			"fnmadd.ps f11, f3, f11, f8, rne\n"
			"fnmadd.s f22, f17, f17, f26, rdn\n"
			"feq.ps f8, f0, f22\n"
			"frcp.ps f25, f25\n"
			"bne x2, x17, 1f\n"
			"1:\n"
			"fmadd.s f25, f25, f26, f3, rmm\n"
			"bne x12, x19, 1f\n"
			"1:\n"
			"fcvt.ps.pw f25, f4, rtz\n"
			"fsat8.pi f18, f17\n"
			"la x15, rand_ieee754_16\n"
			"flw.ps f25, 64(x15)\n"
			"fcvt.ps.f16 f9, f25\n" 
			"fpackrepb.pi f2, f2\n"
			"fsin.ps f26, f22\n"
			"fmax.ps f25, f8, f2\n"
			"fcvt.ps.pwu f26, f0, rtz\n"
			"bne x24, x3, 1f\n"
			"1:\n"
			"fxor.pi f26, f22, f0\n"
			"fcmov.ps f15, f10, f8, f8\n"
			"fsrli.pi f4, f4, 0xb\n"
			"bgeu x25, x24, 1f\n"
			"1:\n"
			"faddi.pi f3, f26, 0x113\n"
			"bgeu x21, x11, 1f\n"
			"1:\n"
			"LBL_SEQID_1_INT_DIV_ID_33_HID_0:\n"
			"div x10, x10, x14\n" 
			"srl x23, x14, x17\n"
			"feq.ps f10, f25, f10\n"
			"fpackreph.pi f26, f18\n"
			"ecall\n"
			"bgeu x15, x14, 1f\n"
			"1:\n"
			"div x5, x10, x23\n"
			"bge x10, x8, 1f\n"
			"1:\n"
			"fnmadd.s f26, f10, f26, f12, rne\n"
			"and x13, x13, x23\n"
			"masknot m2, m2\n"
			"bne x24, x0, 1f\n"
			"1:\n"
			"LBL_SEQID_1_M0_WRITE_ID_34_HID_0:\n"
			"maskor m4, m7, m2\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"fxor.pi f9, f9, f8\n"
			"fmaxu.pi f20, f12, f20\n"
			"fmul.ps f2, f15, f8, rdn\n"
			"fpackreph.pi f15, f17\n"
			"beq x22, x30, 1f\n"
			"1:\n"
			"fcvt.ps.f16 f3, f4\n"
			"fsin.ps f9, f25\n"
			"fcvt.ps.f16 f18, f11\n"
			"fcvt.pwu.ps f17, f8, rtz\n"
			"LBL_SEQID_1_M0_WRITE_ID_35_HID_0:\n"
			"fltm.ps m2, f10, f2\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"fandi.pi f18, f3, 0x1d8\n"
			"fsgnj.s f4, f12, f0\n"
			"bne x5, x23, 1f\n"
			"1:\n"
			"fminu.pi f17, f3, f17\n"
			"fcvt.s.wu f17, x5, rdn\n"
			"fmax.s f17, f17, f12\n"
			"addi x6, x25, 0x1\n"
			"fltu.pi f18, f2, f2\n"
			"fmin.pi f18, f12, f3\n"
			"la x23, rand_int_32\n"
			"flw.ps f4, 800(x23)\n"
			"flw.ps f10, 64(x23)\n"
			"flt.pi f0, f4, f10\n" 
			"fand.pi f2, f9, f2\n"
			"andi x25, x25, 0x14\n"
			"fmin.ps f2, f22, f20\n"
			"fand.pi f9, f2, f2\n"
			"fnmadd.ps f9, f22, f22, f9, rtz\n"
			"bltu x15, x11, 1f\n"
			"1:\n"
			"fnmsub.ps f3, f20, f3, f3, rup\n"
			"fand.pi f26, f22, f20\n"
			"addi x14, x13, 0x7\n"
			"la x28, rand_int_32\n"
			"flw.ps f25, 96(x28)\n"
			"flw.ps f2, 576(x28)\n"
			"fminu.pi f20, f25, f2\n" 
			"fltu.pi f8, f25, f22\n"
			"fnmadd.ps f12, f12, f25, f17, rup\n"
			"bltu x12, x16, 1f\n"
			"1:\n"
			"fpackrepb.pi f0, f10\n"
			"fpackrepb.pi f2, f25\n"
			"flt.pi f22, f2, f2\n"
			"bgeu x25, x27, 1f\n"
			"1:\n"
			"fxor.pi f15, f12, f2\n"
			"fmulhu.pi f12, f12, f20\n"
			"fadd.s f25, f12, f10, rup\n"
			"la x28, rand_int_32\n"
			"flw.ps f9, 96(x28)\n"
			"flw.ps f11, 992(x28)\n"
			"fmaxu.pi f12, f9, f11\n" 
			"fpackreph.pi f26, f25\n"
			"fsra.pi f11, f22, f11\n"
			"fmaxu.pi f11, f20, f22\n"
			"fnmadd.ps f0, f20, f0, f12, rmm\n"
			"fmsub.ps f9, f25, f11, f9, rdn\n"
			"fand.pi f15, f3, f22\n"
			"fsub.ps f22, f17, f20, rup\n"
			"fnmsub.ps f22, f0, f22, f17, rdn\n"
			"la x7, rand_ieee754_32\n"
			"flw.ps f0, 736(x7)\n"
			"fmvz.x.ps x7, f0, 7\n" 
			"fand.pi f3, f8, f3\n"
			"fmax.s f17, f17, f18\n"
			"bgeu x9, x31, 1f\n"
			"1:\n"
			"feq.pi f18, f25, f11\n"
			"addiw x28, x14, 0x6\n"
			"fsat8.pi f10, f18\n"
			"frcp.ps f3, f11\n"
			"addiw x13, x17, 0x1b\n"
			"fclass.ps f12, f9\n"
			"bne x20, x0, 1f\n"
			"1:\n"
			"LBL_SEQID_1_INT_DIV_ID_36_HID_0:\n"
			"div x13, x13, x13\n" 
			"fmin.pi f2, f20, f4\n"
			"fmul.pi f8, f15, f26\n"
			"fexp.ps f2, f2\n"
			"fle.ps f11, f20, f9\n"
			"fmin.pi f0, f18, f25\n"
			"fsrai.pi f3, f22, 0xf\n"
			"fsrli.pi f9, f25, 0xc\n"
			"beq x6, x15, 1f\n"
			"1:\n"
			"fsin.ps f11, f4\n"
			"bge x15, x10, 1f\n"
			"1:\n"
			"la x23, rand_int_32\n"
			"flw.ps f2, 192(x23)\n"
			"flw.ps f10, 256(x23)\n"
			"fmin.pi f20, f2, f10\n" 
			"fnmsub.s f8, f11, f2, f10, rtz\n"
			"fnmsub.ps f2, f8, f2, f10, rne\n"
			"fpackreph.pi f12, f3\n"
			"beq x10, x17, 1f\n"
			"1:\n"
			"fmin.ps f25, f25, f15\n"
			"addi x28, x28, 0x17\n"
			"fand.pi f2, f2, f3\n"
			"addi x28, x23, 0x12\n"
			"addi x17, x17, 0x7\n"
			"la x7, rand_int_32\n"
			"flw.ps f0, 32(x7)\n"
			"flw.ps f26, 64(x7)\n"
			"fmulh.pi f11, f0, f26\n" 
			"fle.s x10, f9, f2\n"
			"blt x15, x24, 1f\n"
			"1:\n"
			"fmv.s.x f22, x28\n"
			"ffrc.ps f22, f11\n"
			"or x13, x25, x13\n"
			"bne x20, x6, 1f\n"
			"1:\n"
			"fsub.ps f9, f22, f10, rne\n"
			"remw x23, x5, x25\n"
			"sub x15, x14, x28\n"
			"fadd.ps f26, f22, f25, rmm\n"
			"la x6, rand_ieee754_32\n"
			"flw.ps f26, 768(x6)\n"
			"flw.ps f17, 512(x6)\n"
			"flw.ps f11, 544(x6)\n"
			"fmadd.s f18, f26, f17, f11, rmm\n" 
			"add x10, x14, x15\n"
			"fmax.ps f17, f10, f12\n"
			"fsin.ps f26, f25\n"
			"fsrai.pi f25, f0, 0x1\n"
			"fnmadd.ps f26, f17, f22, f10, rdn\n"
			"fmax.pi f17, f17, f12\n"
			"fle.s x13, f25, f12\n"
			"fsll.pi f11, f20, f0\n"
			"mov.m.x m0, x0, 0xFF\n"
			"li x5, 0x40\n"
			"la x31, rand_ieee754_32\n"
			"add x31, x31, x5\n"
			"flw.ps f0, 0(x31)\n"
			"flw.ps f20, 4(x31)\n"
			"flw.ps f25, 5(x31)\n"
			"flw.ps f3, 7(x31)\n"
			"flw.ps f4, 1(x31)\n"
			"flw.ps f9, 12(x31)\n"
			"la x31, rand_int_32\n"
			"add x31, x31, x5\n"
			"lw x10, 0(x31)\n"
			"lw x13, 0(x31)\n"
			"lw x15, 3(x31)\n"
			"lw x25, 10(x31)\n"
			"lw x7, 10(x31)\n"
			"li x21, 0x3\n"
			"LBL_SEQID_1_LOOP_SEQ_HID_0:\n"
			"sub x28, x25, x25\n"
			"srl x13, x25, x13\n"
			"flt.s x15, f9, f4\n"
			"fsatu8.pi f25, f25\n"
			"sraw x15, x25, x25\n"
			"subw x10, x25, x10\n"
			"bne x9, x18, 1f\n"
			"1:\n"
			"fsll.pi f4, f3, f4\n"
			"fminu.pi f12, f25, f9\n"
			"fandi.pi f4, f9, 0x10c\n"
			"fsrli.pi f0, f9, 0xf\n"
			"and x17, x10, x7\n"
			"fltm.ps m5, f4, f20\n"
			"bne x18, x20, 1f\n"
			"1:\n"
			"fltm.ps m2, f25, f4\n"
			"mov.m.x m7, x13, 0x36\n"
			"blt x29, x10, 1f\n"
			"1:\n"
			"feq.ps f10, f3, f0\n"
			"sub x13, x25, x15\n"
			"srl x23, x13, x10\n"
			"fmvs.x.ps x23, f0, 3\n"
			"feq.pi f18, f3, f0\n"
			"fmin.ps f3, f4, f3\n"
			"fsgnj.ps f4, f25, f4\n"
			"maskxor m5, m5, m4\n"
			"feq.ps f9, f0, f4\n"
			"fclass.ps f20, f20\n"
			"fbcx.ps f4, x7\n"
			"fsgnjn.s f9, f25, f20\n"
			"fsra.pi f3, f3, f4\n"
			"fsgnjn.s f25, f9, f4\n"
			"bgeu x26, x16, 1f\n"
			"1:\n"
			"fand.pi f4, f20, f4\n"
			"fence\n"
			"fsrli.pi f0, f9, 0x4\n"
			"mulhsu x28, x7, x10\n"
			"fmv.x.s x7, f20\n"
			"bne x4, x10, 1f\n"
			"1:\n"
			"packb x13, x13, x10\n"
			"fmax.s f9, f3, f9\n"
			"ori x28, x15, 0x1a\n"
			"bgeu x5, x1, 1f\n"
			"1:\n"
			"fsat8.pi f4, f9\n"
			"fbci.ps f25, 0x15bda\n"
			"fmul.pi f10, f9, f9\n"
			"fltm.ps m4, f9, f25\n"
			"srai x17, x25, 3\n"
			"fnmsub.ps f4, f0, f3, f0, rtz\n"
			"fmin.s f20, f20, f20\n"
			"bge x17, x22, 1f\n"
			"1:\n"
			"sra x13, x25, x13\n"
			"fsgnjx.ps f11, f20, f20\n"
			"slliw x25, x7, 3\n"
			"fle.ps f25, f20, f0\n"
			"sltu x7, x13, x7\n"
			"bge x28, x11, 1f\n"
			"1:\n"
			"fsat8.pi f4, f25\n"
			"blt x31, x28, 1f\n"
			"1:\n"
			"fbci.ps f9, 0x440e9\n"
			"fsrli.pi f12, f4, 0xb\n"
			"blt x4, x5, 1f\n"
			"1:\n"
			"fmulh.pi f10, f3, f20\n"
			"maskpopc x23, m4\n"
			"fmax.ps f9, f3, f9\n"
			"fcvt.s.w f3, x10, rtz\n"
			"fbcx.ps f12, x25\n"
			"bne x6, x12, 1f\n"
			"1:\n"
			"mulw x23, x10, x15\n"
			"fltm.pi m2, f20, f0\n"
			"bltu x3, x21, 1f\n"
			"1:\n"
			"fsgnj.s f0, f25, f0\n"
			"maskand m7, m5, m7\n"
			"fsetm.pi m2, f0\n"
			"sra x10, x13, x10\n"
			"fsat8.pi f10, f25\n"
			"or x17, x15, x15\n"
			"fltu.pi f10, f0, f9\n"
			"mulw x10, x10, x15\n"
			"maskor m7, m4, m7\n"
			"fnmadd.ps f9, f9, f3, f25, rdn\n"
			"bgeu x1, x24, 1f\n"
			"1:\n"
			"fcvt.wu.s x7, f25, rne\n"
			"mov.m.x m5, x13, 0x6a\n"
			"fltm.pi m4, f0, f25\n"
			"flog.ps f10, f20\n"
			"packb x7, x7, x25\n"
			"maskor m7, m7, m7\n"
			"sra x10, x15, x10\n"
			"bgeu x7, x17, 1f\n"
			"1:\n"
			"fcvt.pw.ps f9, f4, rdn\n"
			"fsra.pi f12, f4, f20\n"
			"fswizz.ps f20, f4, 0xc\n"
			"remu x6, x13, x15\n"
			"bgeu x19, x0, 1f\n"
			"1:\n"
			"fnmsub.ps f10, f9, f9, f0, rmm\n"
			"fcvt.ps.pwu f10, f4, rdn\n"
			"remw x25, x25, x13\n"
			"bgeu x6, x22, 1f\n"
			"1:\n"
			"fsub.ps f3, f4, f4, rne\n"
			"bltu x19, x2, 1f\n"
			"1:\n"
			"fmulhu.pi f3, f0, f3\n"
			"fbci.ps f25, 0x7226a\n"
			"flt.s x28, f20, f20\n"
			"fsra.pi f3, f9, f3\n"
			"flt.ps f9, f9, f4\n"
			"bge x7, x0, 1f\n"
			"1:\n"
			"maskand m2, m7, m4\n"
			"mulh x15, x15, x7\n"
			"fmvs.x.ps x15, f9, 2\n"
			"mulh x10, x15, x7\n"
			"packb x15, x13, x13\n"
			"fsetm.pi m4, f3\n"
			"fcvt.ps.f16 f12, f4\n"
			"fmadd.s f9, f4, f25, f3, rtz\n"
			"maskpopcz x6, m5\n"
			"bne x26, x4, 1f\n"
			"1:\n"
			"fsin.ps f11, f25\n"
			"fsat8.pi f9, f9\n"
			"beq x7, x7, 1f\n"
			"1:\n"
			"fle.ps f11, f20, f0\n"
			"fmsub.ps f10, f25, f3, f9, rup\n"
			"fbci.ps f3, 0x3d3d5\n"
			"fmsub.ps f25, f20, f25, f0, rne\n"
			"add x13, x15, x13\n"
			"fmax.pi f11, f9, f0\n"
			"fmvz.x.ps x23, f0, 7\n"
			"fsetm.pi m5, f4\n"
			"sraw x10, x25, x10\n"
			"fmin.ps f12, f20, f20\n"
			"auipc x13, 0x3\n"
			"slliw x10, x25, 4\n"
			"fadd.s f25, f20, f25, rtz\n"
			"fcmovm.ps f4, f0, f25\n"
			"fnmsub.ps f4, f4, f3, f4, rmm\n"
			"rem x13, x13, x13\n"
			"fmsub.ps f3, f20, f25, f20, rdn\n"
			"fsgnjx.s f3, f9, f3\n"
			"frsq.ps f4, f4\n"
			"fmv.s.x f10, x10\n"
			"fmaxu.pi f4, f25, f4\n"
			"fnmadd.s f12, f3, f3, f0, rdn\n"
			"mulhu x15, x15, x13\n"
			"fmax.pi f3, f25, f3\n"
			"frcp.ps f3, f3\n"
			"mov.m.x m7, x25, 0x6a\n"
			"divuw x15, x13, x7\n"
			"feqm.ps m2, f25, f0\n"
			"fmv.x.s x13, f0\n"
			"bgeu x3, x14, 1f\n"
			"1:\n"
			"fnmadd.ps f9, f9, f9, f25, rdn\n"
			"masknot m2, m4\n"
			"blt x15, x14, 1f\n"
			"1:\n"
			"sra x28, x10, x13\n"
			"bge x29, x14, 1f\n"
			"1:\n"
			"fcmov.ps f25, f20, f20, f20\n"
			"fcvt.pw.ps f25, f4, rmm\n"
			"masknot m5, m4\n"
			"sra x10, x15, x7\n"
			"fsgnjx.s f18, f4, f3\n"
			"fsin.ps f20, f20\n"
			"sub x15, x10, x15\n"
			"packb x15, x15, x7\n"
			"bltu x2, x12, 1f\n"
			"1:\n"
			"masknot m4, m5\n"
			"and x6, x15, x25\n"
			"fnmadd.s f25, f0, f9, f9, rdn\n"
			"fswizz.ps f9, f9, 0x1d\n"
			"srai x15, x15, 2\n"
			"fmsub.s f9, f20, f9, f3, rdn\n"
			"fswizz.ps f3, f9, 0x1\n"
			"faddi.pi f12, f3, 0xdf\n"
			"remuw x13, x10, x10\n"
			"fmax.s f10, f3, f0\n"
			"bge x11, x25, 1f\n"
			"1:\n"
			"slti x28, x10, 0x3\n"
			"bne x11, x5, 1f\n"
			"1:\n"
			"fmin.ps f18, f0, f20\n"
			"fsgnj.ps f4, f4, f3\n"
			"slli x28, x15, 1\n"
			"maskpopcz x15, m7\n"
			"maskpopc x6, m2\n"
			"fltu.pi f25, f0, f25\n"
			"divu x15, x13, x15\n"
			"blt x0, x12, 1f\n"
			"1:\n"
			"fand.pi f12, f25, f4\n"
			"fmin.s f3, f0, f25\n"
			"mulw x17, x25, x25\n"
			"fcvt.ps.f16 f3, f3\n"
			"fcvt.wu.s x17, f25, rup\n"
			"fmax.s f4, f3, f4\n"
			"feq.pi f25, f20, f4\n"
			"bne x10, x7, 1f\n"
			"1:\n"
			"mulh x15, x15, x15\n"
			"ori x10, x10, 0x10\n"
			"auipc x28, 0x8\n"
			"ori x10, x7, 0x3\n"
			"fsin.ps f9, f9\n"
			"fbcx.ps f4, x25\n"
			"blt x29, x30, 1f\n"
			"1:\n"
			"slli x10, x10, 0\n"
			"fcvt.ps.pw f4, f4, rtz\n"
			"or x17, x25, x7\n"
			"fcvt.w.s x15, f0, rtz\n"
			"fmax.s f25, f25, f0\n"
			"flem.ps m2, f9, f4\n"
			"fadd.ps f20, f9, f20, rmm\n"
			"bge x9, x13, 1f\n"
			"1:\n"
			"feqm.ps m7, f9, f3\n"
			"fence\n"
			"remw x15, x15, x7\n"
			"beq x1, x11, 1f\n"
			"1:\n"
			"fcvt.ps.f16 f20, f20\n"
			"fltu.pi f25, f25, f4\n"
			"blt x7, x8, 1f\n"
			"1:\n"
			"fpackreph.pi f25, f3\n"
			"fmax.pi f18, f0, f25\n"
			"fmin.pi f20, f20, f9\n"
			"fcmovm.ps f12, f4, f20\n"
			"fpackreph.pi f3, f0\n"
			"bge x2, x21, 1f\n"
			"1:\n"
			"addi x21, x21, -1\n"
			"bne x21, x0, LBL_SEQID_1_LOOP_SEQ_HID_0\n"
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_0\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_2_FP_GATHER_ID_37_HID_0:\n"
			"li x5, 0x82095a002a\n" 
			"li x6, 0x3450e45a52288fb0\n"
			"fsc32w.ps f7, x6 (x5)\n"
			"fg32h.ps f0, x6 (x5)\n" 
			"flt.ps f22, f26, f10\n"
			"feq.pi f22, f25, f18\n"
			"LBL_SEQID_2_CSR_ID_38_HID_0:\n"
			"csrr x0, scounteren\n"
			"fmax.pi f22, f18, f10\n"
			"bltu x14, x4, 1f\n"
			"1:\n"
			"fnot.pi f10, f15\n"
			"beq x20, x30, 1f\n"
			"1:\n"
			"LBL_SEQID_2_CSR_ID_39_HID_0:\n"
			"li x13, 0x80000820d700001\n"
			"li x31, (0x3 << 6) | 0x1\n"
			"csrw prefetch_va, x13\n" 
			"csrwi tensor_wait, 6\n" 
			"csrwi tensor_wait, 5\n" 
			"sraw x10, x5, x25\n"
			"fsin.ps f25, f26\n"
			"LBL_SEQID_2_INT_AMO_ID_40_HID_0:\n"
			"li x10, 0x82595a0028\n" 
			"amoxorl.d x6, x23, (x10)\n" 
			"fsgnjn.s f17, f25, f26\n"
			"andi x13, x13, 0xa\n"
			"LBL_SEQID_2_FP_G_GATHER_ID_41_HID_0:\n"
			"li x6, 0x823ee6a938\n" 
			"la x5, mem_ptr_offset\n"
			"flq2 f9, 24(x5)\n"
			"fscwg.ps f26, f9 (x6)\n"
			"fgbg.ps f2, f9 (x6)\n" 
			"ffrc.ps f22, f22\n"
			"addw x7, x10, x10\n"
			"LBL_SEQID_2_INT_AMO_ID_42_HID_0:\n"
			"li x25, 0x8149776018\n" 
			"amominul.d x17, x15, (x25)\n" 
			"fsgnjn.ps f2, f8, f17\n"
			"fminu.pi f11, f25, f8\n"
			"LBL_SEQID_2_FP_STORE_ID_43_HID_0:\n"
			"li x17, 0x8103020033\n" 
			"fsw f2, 1940(x17)\n" 
			"divu x6, x7, x7\n"
			"fsub.ps f26, f25, f0, rtz\n"
			"LBL_SEQID_2_INT_AMO_ID_44_HID_0:\n"
			"li x5, 0x822edf8778\n" 
			"amomaxg.d x6, x7, (x5)\n" 
			"slt x5, x5, x14\n"
			"fsrl.pi f10, f11, f4\n"
			"LBL_SEQID_2_FP_L_SCATTER_ID_45_HID_0:\n"
			"li x5, 0x825b060754\n" 
			"la x10, mem_ptr_offset\n"
			"flw.ps f8, 144(x10)\n"
			"fschl.ps f18, f8 (x5)\n" 
			"fltm.ps m2, f4, f18\n"
			"faddi.pi f0, f20, 0x35\n"
			"LBL_SEQID_2_FP_LOAD_ID_46_HID_0:\n"
			"li x13, 0x820c83e823\n" 
			"fsw.ps f26, -1841(x13)\n"
			"fbc.ps f11, -1841(x13)\n" 
			"fcvt.pwu.ps f22, f22, rup\n"
			"fnot.pi f2, f9\n"
			"LBL_SEQID_2_FP_L_GATHER_ID_47_HID_0:\n"
			"li x6, 0x815d4af828\n" 
			"la x15, mem_ptr_offset\n"
			"flw.ps f12, 60(x15)\n"
			"fscwl.ps f18, f12 (x6)\n"
			"fgwl.ps f9, f12 (x6)\n" 
			"slliw x25, x17, 3\n"
			"divu x17, x10, x25\n"
			"bne x28, x4, 1f\n"
			"1:\n"
			"LBL_SEQID_2_INT_AMO_ID_48_HID_0:\n"
			"li x25, 0x814d1e0568\n" 
			"amoandl.w x13, x7, (x25)\n" 
			"fmul.s f26, f4, f8, rmm\n"
			"fround.ps f0, f18, rdn\n"
			"la x15, rand_ieee754_32\n"
			"flw.ps f4, 128(x15)\n"
			"flw.ps f9, 512(x15)\n"
			"fdiv.s f18, f4, f9, rne\n" 
			"fclass.ps f15, f11\n"
			"fmsub.s f11, f11, f15, f11, rtz\n"
			"la x28, rand_int_32\n"
			"flw.ps f25, 832(x28)\n"
			"flw.ps f4, 704(x28)\n"
			"fdiv.pi f8, f25, f4\n" 
			"fsat8.pi f10, f0\n"
			"fmin.ps f0, f2, f2\n"
			"LBL_SEQID_2_INT_AMO_ID_49_HID_0:\n"
			"li x6, 0x813cc18418\n" 
			"amoandg.d x13, x14, (x6)\n" 
			"fsgnjx.ps f22, f20, f2\n"
			"bltu x3, x24, 1f\n"
			"1:\n"
			"ffrc.ps f22, f15\n"
			"LBL_SEQID_2_FP_SCATTER_ID_50_HID_0:\n"
			"li x23, 0x810dd20558\n" 
			"la x5, mem_ptr_offset\n"
			"flq2 f17, 192(x5)\n"
			"fsch.ps f17, f17 (x23)\n" 
			"remu x17, x28, x14\n"
			"flem.ps m2, f18, f26\n"
			"LBL_SEQID_2_INT_LOAD_ID_51_HID_0:\n"
			"li x17, 0x8209680251\n" 
			"sd x6, 661(x17)\n"
			"lb x5, 661(x17)\n" 
			"sll x23, x23, x15\n"
			"frsq.ps f22, f22\n"
			"bltu x5, x16, 1f\n"
			"1:\n"
			"LBL_SEQID_2_FP_L_LOAD_ID_52_HID_0:\n"
			"li x28, 0x81516c0000\n" 
			"mov.m.x m0, zero, 0xff\n"
			"fswl.ps f20, (x28)\n"
			"flwl.ps f4, (x28)\n" 
			"fcvt.f16.ps f2, f0\n"
			"sub x5, x13, x13\n"
			"LBL_SEQID_2_FP_G_SCATTER_ID_53_HID_0:\n"
			"li x14, 0x822a0a03b8\n" 
			"la x13, mem_ptr_offset\n"
			"flq2 f12, 120(x13)\n"
			"fscwg.ps f0, f12 (x14)\n" 
			"fslli.pi f8, f15, 0xd\n"
			"bgeu x3, x10, 1f\n"
			"1:\n"
			"fpackrepb.pi f4, f22\n"
			"LBL_SEQID_2_INT_L_STORE_ID_54_HID_0:\n"
			"li x13, 0x815d4af828\n" 
			"sbl x23, (x13)\n" 
			"fcvt.pwu.ps f9, f11, rmm\n"
			"fmv.x.s x23, f2\n"
			"LBL_SEQID_2_INT_LOAD_ID_55_HID_0:\n"
			"li x10, 0x810977601b\n" 
			"sd x13, -213(x10)\n"
			"lb x17, -213(x10)\n" 
			"feq.s x23, f25, f0\n"
			"fnot.pi f12, f26\n"
			"LBL_SEQID_2_FP_G_SCATTER_ID_56_HID_0:\n"
			"li x13, 0x82238c0000\n" 
			"la x14, mem_ptr_offset\n"
			"flq2 f3, 220(x14)\n"
			"fscbg.ps f15, f3 (x13)\n" 
			"fnmsub.ps f15, f15, f25, f17, rdn\n"
			"fcvt.pwu.ps f2, f3, rne\n"
			"LBL_SEQID_2_INT_AMO_ID_57_HID_0:\n"
			"li x17, 0x8248da0408\n" 
			"amomaxl.d x23, x10, (x17)\n" 
			"fmul.pi f2, f18, f12\n"
			"mulw x23, x23, x10\n"
			"LBL_SEQID_2_FP_AMO_ID_58_HID_0:\n"
			"li x6, 0x823ff87004\n" 
			"la x14, mem_ptr_offset\n"
			"flw.ps f8, 224(x14)\n"
			"famomaxg.pi f0, f8 (x6)\n" 
			"fpackreph.pi f18, f18\n"
			"bgeu x26, x8, 1f\n"
			"1:\n"
			"fnmsub.s f12, f10, f10, f10, rmm\n"
			"la x17, rand_int_32\n"
			"flw.ps f10, 64(x17)\n"
			"flw.ps f8, 800(x17)\n"
			"fremu.pi f0, f10, f8\n" 
			"addi x10, x23, 0xf\n"
			"fmin.s f12, f10, f26\n"
			"LBL_SEQID_2_INT_AMO_ID_59_HID_0:\n"
			"li x15, 0x8220e83018\n" 
			"amoaddg.d x5, x28, (x15)\n" 
			"fmax.pi f20, f20, f20\n"
			"fmsub.s f4, f20, f20, f3, rne\n"
			"bne x29, x3, 1f\n"
			"1:\n"
			"LBL_SEQID_2_INT_LOAD_ID_60_HID_0:\n"
			"li x23, 0x8201dc05dc\n" 
			"sd x6, 1186(x23)\n"
			"lwu x13, 1186(x23)\n" 
			"feq.s x25, f25, f25\n"
			"srlw x10, x5, x15\n"
			"bge x31, x10, 1f\n"
			"1:\n"
			"LBL_SEQID_2_FP_G_SCATTER_ID_61_HID_0:\n"
			"li x17, 0x8221dc05d8\n" 
			"la x10, mem_ptr_offset\n"
			"flq2 f8, 128(x10)\n"
			"fscwg.ps f3, f8 (x17)\n" 
			"fbcx.ps f15, x5\n"
			"bltu x4, x25, 1f\n"
			"1:\n"
			"fnmadd.ps f17, f26, f25, f18, rdn\n"
			"beq x8, x23, 1f\n"
			"1:\n"
			"LBL_SEQID_2_INT_G_STORE_ID_62_HID_0:\n"
			"li x25, 0x8224c7201c\n" 
			"shg x6, (x25)\n" 
			"xori x28, x13, 0x1c\n"
			"bgeu x6, x13, 1f\n"
			"1:\n"
			"fsub.ps f26, f8, f11, rne\n"
			"LBL_SEQID_2_INT_STORE_ID_63_HID_0:\n"
			"li x7, 0x820f3002eb\n" 
			"sw x6, -793(x7)\n" 
			"fmadd.s f9, f25, f11, f8, rup\n"
			"fsrai.pi f4, f25, 0x1\n"
			"bgeu x4, x18, 1f\n"
			"1:\n"
			"LBL_SEQID_2_FP_AMO_ID_64_HID_0:\n"
			"li x25, 0x825e260420\n" 
			"la x13, mem_ptr_offset\n"
			"flw.ps f22, 960(x13)\n"
			"famomaxl.pi f0, f22 (x25)\n" 
			"fsrli.pi f9, f17, 0xb\n"
			"fmul.s f0, f4, f0, rdn\n"
			"LBL_SEQID_2_FP_L_SCATTER_ID_65_HID_0:\n"
			"li x25, 0x825d700028\n" 
			"la x23, mem_ptr_offset\n"
			"flw.ps f25, 48(x23)\n"
			"fscwl.ps f0, f25 (x25)\n" 
			"subw x28, x10, x28\n"
			"fcvt.wu.s x23, f0, rup\n"
			"bne x25, x0, 1f\n"
			"1:\n"
			"mov.m.x m0, x0, 0xFF\n"
			"li x5, 0x60\n"
			"la x31, rand_ieee754_32\n"
			"add x31, x31, x5\n"
			"flw.ps f0, 0(x31)\n"
			"flw.ps f10, 3(x31)\n"
			"flw.ps f11, 4(x31)\n"
			"flw.ps f12, 6(x31)\n"
			"flw.ps f15, 14(x31)\n"
			"flw.ps f17, 19(x31)\n"
			"flw.ps f18, 10(x31)\n"
			"flw.ps f2, 1(x31)\n"
			"flw.ps f20, 5(x31)\n"
			"flw.ps f22, 19(x31)\n"
			"flw.ps f25, 18(x31)\n"
			"flw.ps f26, 8(x31)\n"
			"flw.ps f3, 4(x31)\n"
			"flw.ps f4, 46(x31)\n"
			"flw.ps f8, 4(x31)\n"
			"flw.ps f9, 48(x31)\n"
			"la x31, rand_int_32\n"
			"add x31, x31, x5\n"
			"lw x10, 0(x31)\n"
			"lw x13, 3(x31)\n"
			"lw x14, 5(x31)\n"
			"lw x15, 11(x31)\n"
			"lw x17, 4(x31)\n"
			"lw x23, 1(x31)\n"
			"lw x25, 23(x31)\n"
			"lw x28, 9(x31)\n"
			"lw x5, 13(x31)\n"
			"lw x6, 22(x31)\n"
			"lw x7, 23(x31)\n"
			"li x21, 0x3\n"
			"LBL_SEQID_2_LOOP_SEQ_HID_0:\n"
			"and x28, x7, x13\n"
			"beq x14, x29, 1f\n"
			"1:\n"
			"fsra.pi f8, f2, f8\n"
			"ecall\n"
			"fclass.s x23, f3\n"
			"divw x10, x10, x7\n"
			"ecall\n"
			"fcvt.wu.s x7, f22, rne\n"
			"fsra.pi f15, f18, f20\n"
			"addi x5, x17, 0x15\n"
			"blt x12, x27, 1f\n"
			"1:\n"
			"srl x28, x10, x28\n"
			"fmin.pi f17, f12, f18\n"
			"fsrl.pi f11, f2, f15\n"
			"beq x20, x16, 1f\n"
			"1:\n"
			"fmin.pi f25, f4, f18\n"
			"fcvt.ps.pwu f25, f4, rup\n"
			"andi x10, x23, 0x1b\n"
			"srlw x7, x5, x7\n"
			"fmadd.s f10, f0, f15, f10, rtz\n"
			"bge x5, x9, 1f\n"
			"1:\n"
			"or x6, x13, x23\n"
			"fsll.pi f12, f4, f4\n"
			"for.pi f4, f25, f4\n"
			"fnmsub.ps f22, f18, f25, f15, rup\n"
			"bgeu x1, x8, 1f\n"
			"1:\n"
			"ebreak\n"
			"fcvt.ps.pwu f15, f10, rup\n"
			"bgeu x14, x20, 1f\n"
			"1:\n"
			"or x23, x25, x23\n"
			"bgeu x3, x20, 1f\n"
			"1:\n"
			"fnmsub.s f22, f22, f11, f17, rdn\n"
			"packb x6, x10, x6\n"
			"fmulhu.pi f22, f22, f26\n"
			"subw x6, x25, x6\n"
			"fsgnjx.s f11, f9, f26\n"
			"remw x28, x28, x23\n"
			"fltu.pi f26, f10, f4\n"
			"andi x17, x5, 0x17\n"
			"fbci.pi f3, 0x35023\n"
			"fbci.ps f4, 0x77838\n"
			"subw x17, x6, x13\n"
			"flt.s x10, f25, f12\n"
			"fbci.ps f2, 0x59426\n"
			"flt.pi f17, f17, f3\n"
			"divuw x15, x7, x28\n"
			"bgeu x1, x26, 1f\n"
			"1:\n"
			"fltu.pi f22, f2, f9\n"
			"fslli.pi f4, f11, 0xe\n"
			"fxor.pi f17, f15, f17\n"
			"fsatu8.pi f10, f11\n"
			"fmin.pi f0, f10, f3\n"
			"lui x10, 0x0\n"
			"mul x14, x23, x14\n"
			"srli x14, x28, 0\n"
			"fsatu8.pi f9, f3\n"
			"flog.ps f9, f10\n"
			"feq.pi f12, f10, f26\n"
			"fand.pi f2, f22, f22\n"
			"fmvz.x.ps x13, f9, 6\n"
			"fcvt.w.s x13, f17, rdn\n"
			"fmadd.ps f15, f12, f20, f0, rne\n"
			"fsgnjx.s f25, f11, f22\n"
			"blt x28, x26, 1f\n"
			"1:\n"
			"addiw x15, x10, 0x10\n"
			"add x23, x28, x15\n"
			"fmul.s f8, f8, f26, rtz\n"
			"maskand m4, m4, m4\n"
			"fmsub.ps f9, f22, f10, f0, rne\n"
			"maskxor m7, m4, m5\n"
			"fxor.pi f22, f2, f3\n"
			"fbci.pi f12, 0x7e85\n"
			"sra x17, x25, x17\n"
			"fmv.x.s x28, f10\n"
			"fmulh.pi f15, f9, f15\n"
			"beq x21, x17, 1f\n"
			"1:\n"
			"fsetm.pi m4, f18\n"
			"beq x2, x0, 1f\n"
			"1:\n"
			"flt.s x5, f0, f20\n"
			"sll x28, x13, x17\n"
			"masknot m2, m2\n"
			"fsrai.pi f20, f8, 0x2\n"
			"packb x15, x17, x7\n"
			"fmadd.ps f11, f11, f17, f11, rtz\n"
			"fcvt.pw.ps f0, f25, rup\n"
			"bgeu x26, x1, 1f\n"
			"1:\n"
			"fmax.ps f9, f25, f8\n"
			"fltm.pi m5, f4, f26\n"
			"bltu x22, x19, 1f\n"
			"1:\n"
			"faddi.pi f8, f0, 0x71\n"
			"for.pi f4, f9, f4\n"
			"ffrc.ps f18, f25\n"
			"fclass.s x15, f0\n"
			"fsub.s f17, f17, f26, rdn\n"
			"fsrai.pi f20, f4, 0xf\n"
			"fsll.pi f2, f8, f26\n"
			"bge x20, x12, 1f\n"
			"1:\n"
			"fsrli.pi f8, f8, 0xd\n"
			"rem x15, x15, x25\n"
			"maskxor m5, m7, m4\n"
			"fbci.ps f18, 0x3fb02\n"
			"mulhsu x7, x15, x5\n"
			"mov.m.x m7, x15, 0x8\n"
			"fmvz.x.ps x10, f18, 0\n"
			"mova.m.x x7\n"
			"fsgnj.ps f11, f11, f20\n"
			"fmaxu.pi f25, f26, f25\n"
			"beq x0, x21, 1f\n"
			"1:\n"
			"fcvt.ps.f16 f22, f18\n"
			"bltu x24, x24, 1f\n"
			"1:\n"
			"fnmsub.ps f20, f20, f17, f25, rtz\n"
			"fsgnjn.s f22, f20, f0\n"
			"beq x0, x26, 1f\n"
			"1:\n"
			"fcvt.ps.pw f18, f18, rmm\n"
			"xori x10, x10, 0xb\n"
			"mulhu x7, x23, x28\n"
			"srliw x10, x6, 2\n"
			"ori x7, x7, 0xd\n"
			"addi x7, x15, 0x17\n"
			"feq.ps f4, f20, f4\n"
			"frcp.ps f26, f11\n"
			"sra x23, x25, x25\n"
			"fsub.ps f17, f15, f17, rdn\n"
			"blt x20, x27, 1f\n"
			"1:\n"
			"srai x23, x13, 1\n"
			"bne x28, x8, 1f\n"
			"1:\n"
			"addiw x15, x15, 0x1e\n"
			"mul x6, x6, x25\n"
			"fexp.ps f8, f10\n"
			"srl x13, x10, x5\n"
			"fcvt.pw.ps f2, f8, rup\n"
			"fmax.ps f3, f3, f18\n"
			"bge x31, x25, 1f\n"
			"1:\n"
			"fmaxu.pi f2, f22, f3\n"
			"fmin.s f22, f22, f20\n"
			"slliw x7, x7, 1\n"
			"fmadd.ps f12, f12, f8, f11, rtz\n"
			"beq x4, x18, 1f\n"
			"1:\n"
			"faddi.pi f17, f3, 0x1b4\n"
			"fclass.s x23, f0\n"
			"beq x26, x16, 1f\n"
			"1:\n"
			"fmvs.x.ps x14, f8, 1\n"
			"ori x15, x15, 0x17\n"
			"fsin.ps f25, f18\n"
			"bne x9, x26, 1f\n"
			"1:\n"
			"fcvt.ps.pw f26, f26, rmm\n"
			"fltu.pi f25, f11, f12\n"
			"fle.s x25, f12, f8\n"
			"bltu x13, x11, 1f\n"
			"1:\n"
			"ebreak\n"
			"fpackreph.pi f11, f11\n"
			"bne x28, x24, 1f\n"
			"1:\n"
			"fpackreph.pi f15, f17\n"
			"mulhu x6, x14, x14\n"
			"srlw x10, x23, x13\n"
			"fmin.ps f11, f17, f11\n"
			"fxor.pi f26, f17, f25\n"
			"srl x14, x15, x14\n"
			"bge x29, x12, 1f\n"
			"1:\n"
			"fpackreph.pi f17, f10\n"
			"ffrc.ps f8, f0\n"
			"remu x25, x13, x25\n"
			"fsgnjx.ps f3, f8, f18\n"
			"fcvt.s.wu f25, x10, rdn\n"
			"fmadd.ps f2, f4, f0, f0, rdn\n"
			"lui x10, 0xb\n"
			"fcvt.ps.f16 f18, f9\n"
			"maskpopcz x15, m5\n"
			"fbci.pi f8, 0x13476\n"
			"fsgnjn.ps f10, f2, f9\n"
			"fmax.s f25, f15, f22\n"
			"fclass.s x25, f0\n"
			"fmin.pi f9, f15, f12\n"
			"fcvt.ps.f16 f3, f15\n"
			"fcvt.w.s x13, f25, rmm\n"
			"frcp.ps f12, f8\n"
			"fence\n"
			"fltm.pi m4, f26, f20\n"
			"fminu.pi f2, f3, f20\n"
			"maskxor m4, m7, m7\n"
			"fsub.s f10, f20, f22, rtz\n"
			"addi x21, x21, -1\n"
			"bne x21, x0, LBL_SEQID_2_LOOP_SEQ_HID_0\n"
			"li x5, 3\n" 
			"bne x30, x5, LBL_FAIL_HID_0\n"
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f10,  992 (x5)\n"
			"flw.ps f9,  64 (x5)\n"
			"flw.ps f16,  384 (x5)\n"
			"flw.ps f3,  960 (x5)\n"
			"flw.ps f7,  192 (x5)\n"
			"flw.ps f26,  448 (x5)\n"
			"flw.ps f29,  640 (x5)\n"
			"flw.ps f11,  96 (x5)\n"
			"flw.ps f21,  896 (x5)\n"
			"flw.ps f2,  832 (x5)\n"
			"flw.ps f28,  256 (x5)\n"
			"flw.ps f23,  160 (x5)\n"
			"flw.ps f25,  768 (x5)\n"
			"flw.ps f17,  224 (x5)\n"
			"flw.ps f20,  0 (x5)\n"
			"flw.ps f5,  32 (x5)\n"
			"flw.ps f24,  416 (x5)\n"
			"flw.ps f8,  128 (x5)\n"
			"flw.ps f1,  512 (x5)\n"
			"flw.ps f13,  576 (x5)\n"
			"flw.ps f14,  928 (x5)\n"
			"flw.ps f4,  608 (x5)\n"
			"flw.ps f15,  704 (x5)\n"
			"flw.ps f6,  544 (x5)\n"
			"flw.ps f12,  352 (x5)\n"
			"flw.ps f0,  736 (x5)\n"
			"flw.ps f19,  480 (x5)\n"
			"flw.ps f27,  288 (x5)\n"
			"flw.ps f31,  800 (x5)\n"
			"flw.ps f30,  864 (x5)\n"
			"flw.ps f22,  672 (x5)\n"
			"flw.ps f18,  320 (x5)\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_3_FP_AMO_ID_66_HID_0:\n"
			"li x13, 0x825312003c\n" 
			"la x23, mem_ptr_offset\n"
			"flw.ps f2, 160(x23)\n"
			"famoaddl.pi f26, f2 (x13)\n" 
			"fsgnjx.ps f0, f26, f26\n"
			"fsrli.pi f26, f2, 0xb\n"
			"fle.ps f26, f4, f4\n"
			"blt x28, x2, 1f\n"
			"1:\n"
			"LBL_SEQID_3_FP_GATHER_ID_67_HID_0:\n"
			"li x6, 0x820f3002eb\n" 
			"li x10, 0xe63606a20e63dc00\n"
			"fsc32w.ps f8, x10 (x6)\n"
			"fg32h.ps f17, x10 (x6)\n" 
			"fsub.ps f20, f25, f20, rup\n"
			"bge x23, x22, 1f\n"
			"1:\n"
			"fmvs.x.ps x28, f20, 2\n"
			"fpackreph.pi f12, f25\n"
			"LBL_SEQID_3_INT_AMO_ID_68_HID_0:\n"
			"li x23, 0x825e260420\n" 
			"amomaxl.d x14, x5, (x23)\n" 
			"fsgnj.ps f8, f18, f26\n"
			"fsra.pi f17, f2, f25\n"
			"maskxor m5, m7, m4\n"
			"LBL_SEQID_3_FP_G_STORE_ID_69_HID_0:\n"
			"li x10, 0x82238c0000\n" 
			"mov.m.x m0, zero, 0xff\n"
			"fswg.ps f17, (x10)\n" 
			"for.pi f26, f17, f10\n"
			"beq x2, x21, 1f\n"
			"1:\n"
			"fle.pi f8, f2, f17\n"
			"fcvt.pwu.ps f0, f10, rup\n"
			"LBL_SEQID_3_FP_G_SCATTER_ID_70_HID_0:\n"
			"li x7, 0x81316c0010\n" 
			"la x13, mem_ptr_offset\n"
			"flw.ps f2, 136(x13)\n"
			"fscwg.ps f2, f2 (x7)\n" 
			"fcmov.ps f25, f10, f2, f8\n"
			"fsub.s f10, f17, f0, rdn\n"
			"fsgnjn.s f12, f0, f12\n"
			"LBL_SEQID_3_FP_AMO_ID_71_HID_0:\n"
			"li x6, 0x813ada8024\n" 
			"la x28, mem_ptr_offset\n"
			"flw.ps f25, 160(x28)\n"
			"famoswapg.pi f4, f25 (x6)\n" 
			"fsub.s f0, f9, f0, rdn\n"
			"bltu x26, x30, 1f\n"
			"1:\n"
			"fsra.pi f0, f9, f0\n"
			"bgeu x21, x6, 1f\n"
			"1:\n"
			"fnmadd.s f8, f9, f11, f25, rne\n"
			"la x13, rand_int_32\n"
			"flw.ps f8, 800(x13)\n"
			"flw.ps f11, 416(x13)\n"
			"fremu.pi f11, f8, f11\n" 
			"fexp.ps f15, f0\n"
			"blt x9, x27, 1f\n"
			"1:\n"
			"fsatu8.pi f22, f20\n"
			"fcvt.pwu.ps f22, f26, rmm\n"
			"blt x25, x25, 1f\n"
			"1:\n"
			"LBL_SEQID_3_FP_AMO_ID_72_HID_0:\n"
			"li x17, 0x823e260420\n" 
			"la x7, mem_ptr_offset\n"
			"flw.ps f3, 480(x7)\n"
			"famoandg.pi f8, f3 (x17)\n" 
			"fsat8.pi f20, f0\n"
			"bne x15, x30, 1f\n"
			"1:\n"
			"andi x25, x28, 0x13\n"
			"fadd.s f11, f11, f0, rdn\n"
			"LBL_SEQID_3_FP_AMO_ID_73_HID_0:\n"
			"li x17, 0x824e745820\n" 
			"la x28, mem_ptr_offset\n"
			"flw.ps f12, 928(x28)\n"
			"famomaxl.pi f25, f12 (x17)\n" 
			"fsgnj.s f15, f8, f26\n"
			"fmin.s f26, f10, f8\n"
			"blt x8, x2, 1f\n"
			"1:\n"
			"frsq.ps f10, f26\n"
			"beq x5, x12, 1f\n"
			"1:\n"
			"LBL_SEQID_3_INT_AMO_ID_74_HID_0:\n"
			"li x25, 0x824edf8778\n" 
			"amoorl.d x15, x10, (x25)\n" 
			"fnot.pi f2, f15\n"
			"bge x25, x9, 1f\n"
			"1:\n"
			"fmin.s f0, f22, f11\n"
			"fsat8.pi f10, f15\n"
			"bltu x29, x7, 1f\n"
			"1:\n"
			"LBL_SEQID_3_INT_STORE_ID_75_HID_0:\n"
			"li x15, 0x820fe78af8\n" 
			"sw x10, 615(x15)\n" 
			"fsgnjx.ps f0, f2, f15\n"
			"fsub.pi f22, f22, f10\n"
			"beq x12, x25, 1f\n"
			"1:\n"
			"fle.s x13, f10, f2\n"
			"LBL_SEQID_3_FP_AMO_ID_76_HID_0:\n"
			"li x17, 0x8137bbf024\n" 
			"la x23, mem_ptr_offset\n"
			"flw.ps f26, 992(x23)\n"
			"famoandg.pi f2, f26 (x17)\n" 
			"fmaxu.pi f20, f20, f8\n"
			"fcvt.pw.ps f4, f12, rne\n"
			"fminu.pi f8, f11, f8\n"
			"LBL_SEQID_3_FP_AMO_ID_77_HID_0:\n"
			"li x10, 0x8244c7201c\n" 
			"la x25, mem_ptr_offset\n"
			"flw.ps f25, 704(x25)\n"
			"famomaxl.ps f4, f25 (x10)\n" 
			"fsat8.pi f22, f22\n"
			"bne x30, x22, 1f\n"
			"1:\n"
			"rem x25, x13, x10\n"
			"sra x10, x17, x13\n"
			"LBL_SEQID_3_CSR_ID_78_HID_0:\n"
			"csrr x0, fcsr\n"
			"fslli.pi f18, f18, 0xc\n"
			"fadd.pi f20, f12, f2\n"
			"fsgnjx.ps f22, f0, f4\n"
			"LBL_SEQID_3_FP_SCATTER_ID_79_HID_0:\n"
			"li x28, 0x8200357c03\n" 
			"li x23, 0xb7ce607615e50ca2\n"
			"fsc32h.ps f2, x23 (x28)\n" 
			"fadd.s f2, f2, f18, rtz\n"
			"fsub.ps f4, f4, f0, rmm\n"
			"bgeu x30, x5, 1f\n"
			"1:\n"
			"fand.pi f12, f4, f2\n"
			"blt x15, x5, 1f\n"
			"1:\n"
			"LBL_SEQID_3_FP_LOAD_ID_80_HID_0:\n"
			"li x25, 0x820ee6a93b\n" 
			"fsq2 f11, 1085(x25)\n"
			"fbc.ps f8, 1085(x25)\n" 
			"fcvt.f16.ps f8, f26\n"
			"fcvt.ps.f16 f11, f4\n"
			"bgeu x11, x0, 1f\n"
			"1:\n"
			"fsgnjx.ps f11, f11, f0\n"
			"blt x27, x30, 1f\n"
			"1:\n"
			"LBL_SEQID_3_FP_AMO_ID_81_HID_0:\n"
			"li x15, 0x812d1e0568\n" 
			"la x28, mem_ptr_offset\n"
			"flw.ps f3, 960(x28)\n"
			"famomaxg.pi f4, f3 (x15)\n" 
			"fandi.pi f8, f17, 0x15d\n"
			"fsrli.pi f17, f26, 0xe\n"
			"fnmadd.s f12, f11, f2, f0, rne\n"
			"LBL_SEQID_3_INT_AMO_ID_82_HID_0:\n"
			"li x13, 0x82238c0004\n" 
			"amoswapg.w x23, x17, (x13)\n" 
			"fnmadd.s f10, f10, f10, f3, rne\n"
			"fsrl.pi f18, f11, f25\n"
			"fsin.ps f11, f0\n"
			"beq x28, x28, 1f\n"
			"1:\n"
			"LBL_SEQID_3_INT_AMO_ID_83_HID_0:\n"
			"li x28, 0x8245760100\n" 
			"amoorl.w x25, x6, (x28)\n" 
			"addw x5, x15, x23\n"
			"fltm.pi m7, f22, f8\n"
			"bge x31, x10, 1f\n"
			"1:\n"
			"fmadd.ps f9, f18, f9, f9, rne\n"
			"LBL_SEQID_3_FP_G_SCATTER_ID_84_HID_0:\n"
			"li x23, 0x822f3002e8\n" 
			"la x25, mem_ptr_offset\n"
			"flw.ps f11, 172(x25)\n"
			"fscwg.ps f20, f11 (x23)\n" 
			"fle.ps f10, f0, f26\n"
			"fmsub.s f8, f26, f8, f25, rne\n"
			"fsin.ps f15, f9\n"
			"bge x18, x19, 1f\n"
			"1:\n"
			"LBL_SEQID_3_INT_AMO_ID_85_HID_0:\n"
			"li x17, 0x8133da4d64\n" 
			"amoaddg.w x25, x10, (x17)\n" 
			"fmax.pi f26, f26, f26\n"
			"flog.ps f15, f15\n"
			"fslli.pi f20, f8, 0x8\n"
			"LBL_SEQID_3_INT_AMO_ID_86_HID_0:\n"
			"li x7, 0x814d1e0568\n" 
			"amoxorl.w x25, x15, (x7)\n" 
			"fnmadd.s f2, f11, f11, f2, rtz\n"
			"fsgnjx.ps f26, f9, f9\n"
			"fmadd.ps f22, f10, f9, f18, rtz\n"
			"LBL_SEQID_3_FP_GATHER_ID_87_HID_0:\n"
			"li x25, 0x8205760101\n" 
			"li x15, 0xacf7a6ed7c293dff\n"
			"fsc32w.ps f9, x15 (x25)\n"
			"fg32b.ps f4, x15 (x25)\n" 
			"fcmovm.ps f20, f9, f9\n"
			"fpackrepb.pi f26, f26\n"
			"faddi.pi f15, f15, 0x11f\n"
			"LBL_SEQID_3_INT_G_STORE_ID_88_HID_0:\n"
			"li x28, 0x823b060754\n" 
			"shg x5, (x28)\n" 
			"fcvt.w.s x5, f26, rne\n"
			"fmax.pi f25, f10, f22\n"
			"blt x24, x13, 1f\n"
			"1:\n"
			"srl x15, x23, x10\n"
			"bgeu x15, x15, 1f\n"
			"1:\n"
			"LBL_SEQID_3_FP_G_STORE_ID_89_HID_0:\n"
			"li x17, 0x812d1e0560\n" 
			"mov.m.x m0, zero, 0xff\n"
			"fswg.ps f15, (x17)\n" 
			"fmv.s.x f9, x5\n"
			"fsgnjn.ps f10, f18, f12\n"
			"fslli.pi f12, f9, 0x6\n"
			"LBL_SEQID_3_FP_AMO_ID_90_HID_0:\n"
			"li x15, 0x8135f60004\n" 
			"la x13, mem_ptr_offset\n"
			"flw.ps f15, 608(x13)\n"
			"famoxorg.pi f8, f15 (x15)\n" 
			"fsetm.pi m7, f12\n"
			"slliw x6, x6, 1\n"
			"flt.s x10, f12, f12\n"
			"LBL_SEQID_3_FP_AMO_ID_91_HID_0:\n"
			"li x15, 0x82238c0004\n" 
			"la x10, mem_ptr_offset\n"
			"flw.ps f17, 608(x10)\n"
			"famoorg.pi f10, f17 (x15)\n" 
			"fltm.pi m4, f20, f0\n"
			"fsgnjx.ps f20, f15, f18\n"
			"slti x25, x10, 0x1e\n"
			"LBL_SEQID_3_INT_AMO_ID_92_HID_0:\n"
			"li x7, 0x815cc18418\n" 
			"amomaxul.d x13, x17, (x7)\n" 
			"fle.pi f22, f17, f3\n"
			"fminu.pi f25, f25, f17\n"
			"beq x2, x24, 1f\n"
			"1:\n"
			"feq.s x7, f10, f11\n"
			"fence.i\n" 
			"fcvt.s.wu f25, x17, rmm\n"
			"fmin.s f8, f3, f10\n"
			"bltu x9, x18, 1f\n"
			"1:\n"
			"masknot m2, m2\n"
			"LBL_SEQID_3_CSR_ID_93_HID_0:\n"
			"csrr x0, tdata2\n"
			"fmulhu.pi f25, f3, f26\n"
			"fmax.s f26, f3, f3\n"
			"fsrli.pi f26, f26, 0xc\n"
			"LBL_SEQID_3_INT_AMO_ID_94_HID_0:\n"
			"li x25, 0x8220e83018\n" 
			"amoandg.w x28, x5, (x25)\n" 
			"sltu x10, x10, x5\n"
			"div x5, x7, x6\n"
			"frsq.ps f3, f2\n"
			"LBL_SEQID_3_FP_L_LOAD_ID_95_HID_0:\n"
			"li x23, 0x8248da0400\n" 
			"mov.m.x m0, zero, 0xff\n"
			"fswl.ps f8, (x23)\n"
			"flwl.ps f20, (x23)\n" 
			"xor x14, x15, x14\n"
			"bgeu x25, x0, 1f\n"
			"1:\n"
			"feqm.ps m2, f26, f10\n"
			"fcvt.s.wu f26, x14, rtz\n"
			"mov.m.x m0, x0, 0xFF\n"
			"li x5, 0x80\n"
			"la x31, rand_ieee754_32\n"
			"add x31, x31, x5\n"
			"flw.ps f0, 0(x31)\n"
			"flw.ps f10, 2(x31)\n"
			"flw.ps f11, 4(x31)\n"
			"flw.ps f12, 3(x31)\n"
			"flw.ps f15, 5(x31)\n"
			"flw.ps f17, 8(x31)\n"
			"flw.ps f18, 3(x31)\n"
			"flw.ps f2, 19(x31)\n"
			"flw.ps f20, 17(x31)\n"
			"flw.ps f22, 33(x31)\n"
			"flw.ps f25, 7(x31)\n"
			"flw.ps f26, 21(x31)\n"
			"flw.ps f3, 5(x31)\n"
			"flw.ps f4, 3(x31)\n"
			"flw.ps f8, 23(x31)\n"
			"flw.ps f9, 33(x31)\n"
			"la x31, rand_int_32\n"
			"add x31, x31, x5\n"
			"lw x10, 0(x31)\n"
			"lw x13, 3(x31)\n"
			"lw x14, 1(x31)\n"
			"lw x15, 3(x31)\n"
			"lw x17, 6(x31)\n"
			"lw x23, 7(x31)\n"
			"lw x25, 8(x31)\n"
			"lw x28, 23(x31)\n"
			"lw x5, 31(x31)\n"
			"lw x6, 29(x31)\n"
			"lw x7, 12(x31)\n"
			"li x21, 0x3\n"
			"LBL_SEQID_3_LOOP_SEQ_HID_0:\n"
			"fsgnj.ps f2, f15, f25\n"
			"fadd.ps f26, f9, f18, rtz\n"
			"fmin.s f4, f0, f3\n"
			"ffrc.ps f26, f22\n"
			"mulhsu x6, x6, x7\n"
			"andi x28, x25, 0x15\n"
			"ori x5, x6, 0x16\n"
			"fsgnj.ps f11, f22, f12\n"
			"mul x7, x23, x7\n"
			"bne x9, x4, 1f\n"
			"1:\n"
			"div x25, x6, x10\n"
			"xor x15, x17, x28\n"
			"fbci.ps f11, 0x24c00\n"
			"fcvt.f16.ps f9, f10\n"
			"fmaxu.pi f15, f20, f9\n"
			"maskxor m5, m7, m2\n"
			"bgeu x8, x31, 1f\n"
			"1:\n"
			"fmulh.pi f3, f25, f18\n"
			"fcmovm.ps f4, f25, f22\n"
			"mul x13, x6, x5\n"
			"flog.ps f15, f15\n"
			"fsub.ps f2, f10, f2, rmm\n"
			"mova.x.m x28\n"
			"bne x27, x20, 1f\n"
			"1:\n"
			"masknot m7, m7\n"
			"fcvt.ps.pwu f8, f8, rtz\n"
			"bltu x13, x3, 1f\n"
			"1:\n"
			"fmvs.x.ps x23, f26, 2\n"
			"fsub.ps f10, f10, f22, rup\n"
			"fle.pi f4, f20, f18\n"
			"bge x30, x7, 1f\n"
			"1:\n"
			"mulhu x17, x13, x13\n"
			"fmsub.ps f15, f0, f10, f25, rmm\n"
			"beq x11, x6, 1f\n"
			"1:\n"
			"fmax.pi f17, f18, f11\n"
			"for.pi f12, f9, f12\n"
			"slt x6, x6, x17\n"
			"fmul.ps f26, f17, f18, rtz\n"
			"srliw x5, x15, 1\n"
			"fandi.pi f4, f3, 0xcd\n"
			"fclass.s x23, f4\n"
			"fcvt.s.wu f10, x14, rdn\n"
			"fcvt.pwu.ps f25, f20, rne\n"
			"divw x23, x5, x25\n"
			"bge x29, x0, 1f\n"
			"1:\n"
			"fsub.s f4, f11, f4, rtz\n"
			"fsrai.pi f3, f9, 0xe\n"
			"fle.ps f8, f18, f18\n"
			"bne x22, x22, 1f\n"
			"1:\n"
			"xori x7, x5, 0xb\n"
			"mulhu x10, x10, x5\n"
			"divw x6, x17, x13\n"
			"fsgnj.s f8, f18, f15\n"
			"fsll.pi f12, f12, f8\n"
			"bltu x20, x19, 1f\n"
			"1:\n"
			"fsgnjn.s f22, f22, f2\n"
			"fbci.ps f17, 0x50d8c\n"
			"maskor m5, m4, m5\n"
			"fnmsub.ps f3, f2, f11, f12, rtz\n"
			"fsgnjn.ps f18, f15, f10\n"
			"fltm.ps m7, f20, f0\n"
			"fnmsub.s f8, f18, f4, f26, rmm\n"
			"fsub.pi f18, f18, f10\n"
			"div x13, x28, x23\n"
			"maskpopc x25, m2\n"
			"fsll.pi f18, f8, f8\n"
			"lui x10, 0x2\n"
			"mova.m.x x23\n"
			"fcvt.ps.pwu f18, f17, rup\n"
			"fmadd.s f2, f22, f2, f18, rup\n"
			"fclass.ps f25, f22\n"
			"fnot.pi f3, f9\n"
			"fmulh.pi f11, f26, f8\n"
			"fcvt.s.wu f25, x14, rup\n"
			"fnmsub.ps f26, f22, f8, f0, rtz\n"
			"fbci.pi f12, 0x38181\n"
			"fxor.pi f8, f0, f10\n"
			"sltiu x28, x28, 0x16\n"
			"bgeu x7, x28, 1f\n"
			"1:\n"
			"fadd.pi f25, f25, f22\n"
			"flog.ps f3, f2\n"
			"fsra.pi f4, f4, f8\n"
			"xor x15, x25, x6\n"
			"sll x28, x28, x14\n"
			"fsgnjx.ps f26, f9, f25\n"
			"fsin.ps f22, f9\n"
			"slliw x14, x13, 0\n"
			"fadd.s f22, f11, f25, rne\n"
			"sltiu x28, x28, 0x10\n"
			"fsatu8.pi f15, f18\n"
			"fsgnj.s f2, f10, f2\n"
			"faddi.pi f2, f2, 0x1ec\n"
			"fclass.ps f9, f9\n"
			"fcvt.ps.f16 f2, f9\n"
			"srl x14, x17, x25\n"
			"fmax.s f17, f22, f11\n"
			"fnmsub.ps f18, f0, f10, f12, rne\n"
			"addiw x28, x10, 0x1f\n"
			"and x10, x17, x6\n"
			"maskxor m4, m4, m5\n"
			"feq.ps f12, f3, f12\n"
			"feq.pi f0, f9, f0\n"
			"addi x14, x23, 0x8\n"
			"lui x6, 0x6\n"
			"fslli.pi f26, f26, 0x0\n"
			"fclass.ps f22, f12\n"
			"fnmsub.ps f17, f10, f10, f17, rtz\n"
			"fle.pi f11, f26, f11\n"
			"bltu x23, x1, 1f\n"
			"1:\n"
			"fcmov.ps f25, f3, f26, f26\n"
			"fltm.pi m7, f3, f0\n"
			"ebreak\n"
			"bge x22, x20, 1f\n"
			"1:\n"
			"fsin.ps f0, f11\n"
			"fminu.pi f17, f17, f9\n"
			"bge x19, x28, 1f\n"
			"1:\n"
			"fnmsub.ps f18, f11, f11, f25, rdn\n"
			"fcvt.w.s x17, f26, rmm\n"
			"fcvt.pw.ps f17, f18, rmm\n"
			"bge x29, x20, 1f\n"
			"1:\n"
			"fand.pi f18, f0, f9\n"
			"fcvt.ps.pwu f26, f8, rdn\n"
			"mulh x15, x28, x15\n"
			"fsetm.pi m7, f12\n"
			"lui x15, 0x12\n"
			"srlw x17, x25, x15\n"
			"divw x28, x10, x28\n"
			"fence\n"
			"fmvs.x.ps x17, f25, 6\n"
			"fltm.ps m2, f26, f25\n"
			"srlw x7, x14, x6\n"
			"bgeu x1, x16, 1f\n"
			"1:\n"
			"fmulhu.pi f0, f18, f4\n"
			"bne x17, x28, 1f\n"
			"1:\n"
			"srliw x6, x7, 4\n"
			"subw x23, x13, x28\n"
			"srlw x6, x10, x17\n"
			"fmul.ps f10, f17, f17, rdn\n"
			"addi x17, x17, 0xa\n"
			"bgeu x13, x17, 1f\n"
			"1:\n"
			"fclass.s x13, f2\n"
			"srl x28, x23, x7\n"
			"fmadd.s f15, f18, f3, f15, rdn\n"
			"bgeu x2, x25, 1f\n"
			"1:\n"
			"sraw x5, x10, x5\n"
			"fpackrepb.pi f25, f3\n"
			"bgeu x18, x10, 1f\n"
			"1:\n"
			"fsra.pi f22, f3, f3\n"
			"fmax.pi f8, f0, f4\n"
			"fmadd.s f25, f9, f9, f11, rtz\n"
			"bne x25, x14, 1f\n"
			"1:\n"
			"fsub.s f8, f11, f2, rne\n"
			"sltiu x28, x14, 0x1\n"
			"fmsub.s f9, f12, f10, f9, rne\n"
			"flem.ps m5, f15, f0\n"
			"blt x28, x10, 1f\n"
			"1:\n"
			"mulhu x6, x23, x14\n"
			"fmaxu.pi f9, f22, f25\n"
			"for.pi f10, f22, f0\n"
			"blt x11, x27, 1f\n"
			"1:\n"
			"fcvt.wu.s x10, f25, rtz\n"
			"fsgnjx.ps f18, f0, f18\n"
			"fnmadd.ps f10, f2, f17, f8, rdn\n"
			"fandi.pi f25, f15, 0xdc\n"
			"fmadd.ps f11, f15, f26, f17, rup\n"
			"fandi.pi f25, f15, 0x154\n"
			"fsgnjx.ps f20, f15, f9\n"
			"remu x25, x6, x14\n"
			"fpackrepb.pi f9, f9\n"
			"mova.x.m x6\n"
			"blt x27, x11, 1f\n"
			"1:\n"
			"maskxor m2, m2, m7\n"
			"fswizz.ps f22, f11, 0x1e\n"
			"fsrl.pi f2, f22, f20\n"
			"beq x6, x30, 1f\n"
			"1:\n"
			"sraw x10, x13, x15\n"
			"fsatu8.pi f25, f17\n"
			"maskxor m7, m4, m7\n"
			"srliw x28, x28, 0\n"
			"bgeu x9, x29, 1f\n"
			"1:\n"
			"divuw x25, x15, x23\n"
			"fadd.s f22, f22, f4, rmm\n"
			"fsat8.pi f26, f3\n"
			"addi x21, x21, -1\n"
			"bne x21, x0, LBL_SEQID_3_LOOP_SEQ_HID_0\n"
			"li x5, 4\n" 
			"bne x30, x5, LBL_FAIL_HID_0\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_4_FP_G_SCATTER_ID_96_HID_0:\n"
			"li x10, 0x8123020030\n" 
			"la x5, mem_ptr_offset\n"
			"flw.ps f20, 128(x5)\n"
			"fschg.ps f9, f20 (x10)\n" 
			"LBL_SEQID_4_FP_G_LOAD_ID_97_HID_0:\n"
			"li x5, 0x8230357c00\n" 
			"mov.m.x m0, zero, 0xff\n"
			"fswg.ps f8, (x5)\n"
			"flwg.ps f0, (x5)\n" 
			"LBL_SEQID_4_INT_STORE_ID_98_HID_0:\n"
			"li x5, 0x8101f2f386\n" 
			"sh x28, -1885(x5)\n" 
			"LBL_SEQID_4_FP_AMO_ID_99_HID_0:\n"
			"li x10, 0x8248da0408\n" 
			"la x13, mem_ptr_offset\n"
			"flw.ps f3, 480(x13)\n"
			"famoxorl.pi f11, f3 (x10)\n" 
			"LBL_SEQID_4_INT_AMO_ID_100_HID_0:\n"
			"li x5, 0x825e260420\n" 
			"amomaxul.w x17, x10, (x5)\n" 
			"LBL_SEQID_4_INT_AMO_ID_101_HID_0:\n"
			"li x5, 0x8250357c00\n" 
			"amoandl.d x17, x6, (x5)\n" 
			"LBL_SEQID_4_FP_AMO_ID_102_HID_0:\n"
			"li x14, 0x815d4af828\n" 
			"la x6, mem_ptr_offset\n"
			"flw.ps f2, 96(x6)\n"
			"famoxorl.pi f11, f2 (x14)\n" 
			"LBL_SEQID_4_INT_G_STORE_ID_103_HID_0:\n"
			"li x17, 0x8224c7201c\n" 
			"sbg x25, (x17)\n" 
			"LBL_SEQID_4_INT_AMO_ID_104_HID_0:\n"
			"li x23, 0x81316c0014\n" 
			"amoaddg.w x15, x10, (x23)\n" 
			"LBL_SEQID_4_FP_AMO_ID_105_HID_0:\n"
			"li x23, 0x8130686a5c\n" 
			"la x10, mem_ptr_offset\n"
			"flw.ps f22, 224(x10)\n"
			"famoaddg.pi f2, f22 (x23)\n" 
			"LBL_SEQID_4_FP_L_GATHER_ID_106_HID_0:\n"
			"li x10, 0x825312003c\n" 
			"la x25, mem_ptr_offset\n"
			"flw.ps f11, 56(x25)\n"
			"fscwl.ps f23, f11 (x10)\n"
			"fghl.ps f0, f11 (x10)\n" 
			"LBL_SEQID_4_FP_L_SCATTER_ID_107_HID_0:\n"
			"li x10, 0x8253120038\n" 
			"la x7, mem_ptr_offset\n"
			"flq2 f10, 24(x7)\n"
			"fscbl.ps f12, f10 (x10)\n" 
			"LBL_SEQID_4_INT_AMO_ID_108_HID_0:\n"
			"li x14, 0x824ee6a938\n" 
			"amoswapl.w x10, x5, (x14)\n" 
			"LBL_SEQID_4_CSR_ID_109_HID_0:\n"
			"li x14, 0x80000081016c000f\n"
			"li x31, (0x2 << 6) | 0x1\n"
			"csrw prefetch_va, x14\n" 
			"csrwi tensor_wait, 6\n" 
			"csrwi tensor_wait, 5\n" 
			"LBL_SEQID_4_INT_LOAD_ID_110_HID_0:\n"
			"li x10, 0x820e745823\n" 
			"sd x28, 600(x10)\n"
			"lbu x7, 600(x10)\n" 
			"LBL_SEQID_4_FP_AMO_ID_111_HID_0:\n"
			"li x7, 0x824ee6a938\n" 
			"la x25, mem_ptr_offset\n"
			"flw.ps f20, 544(x25)\n"
			"famoorl.pi f10, f20 (x7)\n" 
			"LBL_SEQID_4_INT_AMO_ID_112_HID_0:\n"
			"li x10, 0x8123020030\n" 
			"amominug.d x17, x6, (x10)\n" 
			"LBL_SEQID_4_FP_AMO_ID_113_HID_0:\n"
			"li x14, 0x8256546008\n" 
			"la x28, mem_ptr_offset\n"
			"flw.ps f18, 832(x28)\n"
			"famominul.pi f15, f18 (x14)\n" 
			"LBL_SEQID_4_FP_L_SCATTER_ID_114_HID_0:\n"
			"li x28, 0x815cc18418\n" 
			"la x6, mem_ptr_offset\n"
			"flq2 f20, 112(x6)\n"
			"fscbl.ps f15, f20 (x28)\n" 
			"LBL_SEQID_4_FP_AMO_ID_115_HID_0:\n"
			"li x6, 0x8130686a5c\n" 
			"la x28, mem_ptr_offset\n"
			"flw.ps f22, 96(x28)\n"
			"famoandg.pi f18, f22 (x6)\n" 
			"LBL_SEQID_4_FP_L_SCATTER_ID_116_HID_0:\n"
			"li x17, 0x8141f2f384\n" 
			"la x25, mem_ptr_offset\n"
			"flw.ps f20, 52(x25)\n"
			"fschl.ps f8, f20 (x17)\n" 
			"LBL_SEQID_4_INT_AMO_ID_117_HID_0:\n"
			"li x14, 0x8224c7201c\n" 
			"amomaxug.w x10, x7, (x14)\n" 
			"LBL_SEQID_4_FP_L_SCATTER_ID_118_HID_0:\n"
			"li x6, 0x8240e83018\n" 
			"la x15, mem_ptr_offset\n"
			"flw.ps f10, 12(x15)\n"
			"fscwl.ps f22, f10 (x6)\n" 
			"LBL_SEQID_4_INT_AMO_ID_119_HID_0:\n"
			"li x23, 0x824fe78af8\n" 
			"amomaxul.w x25, x28, (x23)\n" 
			"LBL_SEQID_4_FP_SCATTER_ID_120_HID_0:\n"
			"li x14, 0x8103da4d66\n" 
			"la x28, mem_ptr_offset\n"
			"flq2 f12, 172(x28)\n"
			"fscb.ps f20, f12 (x14)\n" 
			"LBL_SEQID_4_INT_AMO_ID_121_HID_0:\n"
			"li x13, 0x825c83e820\n" 
			"amoxorl.w x23, x7, (x13)\n" 
			"LBL_SEQID_4_FP_AMO_ID_122_HID_0:\n"
			"li x15, 0x8137bbf024\n" 
			"la x10, mem_ptr_offset\n"
			"flw.ps f0, 672(x10)\n"
			"famoming.pi f0, f0 (x15)\n" 
			"LBL_SEQID_4_FP_SCATTER_ID_123_HID_0:\n"
			"li x23, 0x820e745823\n" 
			"la x15, mem_ptr_offset\n"
			"flq2 f3, 12(x15)\n"
			"fscb.ps f10, f3 (x23)\n" 
			"LBL_SEQID_4_INT_LOAD_ID_124_HID_0:\n"
			"li x10, 0x820e745823\n" 
			"sd x25, 1225(x10)\n"
			"lb x17, 1225(x10)\n" 
			"LBL_SEQID_4_CSR_ID_125_HID_0:\n"
			"csrr x0, sstatus\n"
			"LBL_SEQID_4_FP_G_GATHER_ID_126_HID_0:\n"
			"li x7, 0x8238da0408\n" 
			"la x15, mem_ptr_offset\n"
			"flq2 f25, 176(x15)\n"
			"fscwg.ps f29, f25 (x7)\n"
			"fghg.ps f4, f25 (x7)\n" 
			"LBL_SEQID_4_INT_AMO_ID_127_HID_0:\n"
			"li x23, 0x8253120038\n" 
			"amoaddl.d x17, x13, (x23)\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"li x5, 0x28\n"
			"la x31, rand_ieee754_32\n"
			"add x31, x31, x5\n"
			"flw.ps f10, 0(x31)\n"
			"flw.ps f12, 4(x31)\n"
			"flw.ps f15, 2(x31)\n"
			"flw.ps f18, 10(x31)\n"
			"flw.ps f2, 6(x31)\n"
			"flw.ps f20, 12(x31)\n"
			"flw.ps f22, 0(x31)\n"
			"flw.ps f25, 7(x31)\n"
			"flw.ps f3, 10(x31)\n"
			"flw.ps f8, 12(x31)\n"
			"flw.ps f9, 10(x31)\n"
			"la x31, rand_int_32\n"
			"add x31, x31, x5\n"
			"lw x10, 0(x31)\n"
			"lw x13, 2(x31)\n"
			"lw x14, 1(x31)\n"
			"lw x15, 11(x31)\n"
			"lw x17, 0(x31)\n"
			"lw x23, 14(x31)\n"
			"lw x25, 15(x31)\n"
			"lw x28, 21(x31)\n"
			"lw x5, 2(x31)\n"
			"lw x6, 24(x31)\n"
			"lw x7, 15(x31)\n"
			"li x21, 0x1\n"
			"LBL_SEQID_4_LOOP_SEQ_HID_0:\n"
			"slt x6, x17, x10\n"
			"fslli.pi f12, f18, 0x6\n"
			"fmulhu.pi f20, f22, f25\n"
			"blt x25, x23, 1f\n"
			"1:\n"
			"fcmov.ps f15, f25, f15, f20\n"
			"fcvt.ps.pwu f4, f10, rup\n"
			"fsetm.pi m2, f9\n"
			"fmvz.x.ps x25, f20, 6\n"
			"slti x6, x6, 0x1f\n"
			"fsgnjn.ps f20, f12, f3\n"
			"xori x17, x28, 0x8\n"
			"addw x25, x28, x5\n"
			"fcvt.wu.s x10, f2, rmm\n"
			"fcvt.ps.pwu f8, f25, rdn\n"
			"fadd.pi f22, f18, f3\n"
			"bgeu x25, x8, 1f\n"
			"1:\n"
			"mova.m.x x28\n"
			"beq x7, x9, 1f\n"
			"1:\n"
			"fadd.pi f15, f2, f8\n"
			"fmsub.s f15, f8, f2, f22, rup\n"
			"beq x15, x29, 1f\n"
			"1:\n"
			"packb x28, x15, x14\n"
			"fmul.s f4, f8, f15, rtz\n"
			"fsrai.pi f10, f10, 0xc\n"
			"feqm.ps m4, f20, f9\n"
			"rem x15, x7, x15\n"
			"rem x23, x23, x28\n"
			"remw x17, x10, x17\n"
			"bge x1, x7, 1f\n"
			"1:\n"
			"fadd.ps f11, f22, f10, rtz\n"
			"blt x26, x9, 1f\n"
			"1:\n"
			"divw x6, x23, x28\n"
			"bltu x17, x13, 1f\n"
			"1:\n"
			"fle.s x6, f9, f2\n"
			"ebreak\n"
			"bge x7, x22, 1f\n"
			"1:\n"
			"fround.ps f9, f9, rup\n"
			"srlw x10, x10, x14\n"
			"ebreak\n"
			"fsgnjx.s f18, f15, f18\n"
			"fslli.pi f0, f20, 0xd\n"
			"fslli.pi f3, f18, 0x2\n"
			"fmvz.x.ps x25, f10, 3\n"
			"fcvt.w.s x7, f12, rdn\n"
			"fmin.pi f3, f3, f25\n"
			"fsub.s f3, f25, f3, rmm\n"
			"fmul.pi f15, f2, f20\n"
			"fcvt.s.wu f10, x28, rtz\n"
			"fnmsub.s f9, f8, f9, f9, rne\n"
			"fmin.s f3, f8, f10\n"
			"blt x11, x15, 1f\n"
			"1:\n"
			"divuw x13, x6, x28\n"
			"fpackreph.pi f4, f10\n"
			"remuw x15, x28, x5\n"
			"fround.ps f10, f20, rdn\n"
			"fmadd.ps f3, f18, f3, f3, rup\n"
			"fbci.ps f25, 0x3f583\n"
			"blt x20, x15, 1f\n"
			"1:\n"
			"divu x13, x25, x23\n"
			"fsub.ps f20, f2, f10, rdn\n"
			"div x15, x15, x6\n"
			"fsub.s f18, f18, f25, rtz\n"
			"fmin.s f0, f2, f2\n"
			"andi x6, x25, 0xf\n"
			"fadd.ps f26, f25, f25, rdn\n"
			"rem x25, x6, x13\n"
			"blt x19, x21, 1f\n"
			"1:\n"
			"fsgnj.s f3, f22, f3\n"
			"fmv.s.x f22, x15\n"
			"flog.ps f15, f9\n"
			"subw x13, x13, x14\n"
			"fmvz.x.ps x15, f10, 0\n"
			"maskpopcz x23, m4\n"
			"fmsub.ps f8, f8, f3, f2, rmm\n"
			"fcvt.ps.pw f10, f15, rdn\n"
			"maskand m5, m5, m5\n"
			"slliw x23, x14, 5\n"
			"fsrl.pi f25, f25, f2\n"
			"bne x29, x21, 1f\n"
			"1:\n"
			"fclass.s x25, f8\n"
			"blt x3, x28, 1f\n"
			"1:\n"
			"mulw x13, x14, x25\n"
			"fmax.ps f2, f2, f10\n"
			"mov.m.x m4, x28, 0x71\n"
			"fand.pi f26, f15, f8\n"
			"sraiw x5, x17, 2\n"
			"ebreak\n"
			"fcvt.ps.f16 f8, f8\n"
			"fbci.pi f18, 0x4241f\n"
			"fsin.ps f22, f25\n"
			"blt x1, x1, 1f\n"
			"1:\n"
			"fslli.pi f26, f15, 0x1\n"
			"feqm.ps m4, f10, f25\n"
			"fmvs.x.ps x15, f9, 6\n"
			"fnot.pi f10, f9\n"
			"feq.pi f15, f8, f9\n"
			"frcp.ps f0, f25\n"
			"bne x27, x17, 1f\n"
			"1:\n"
			"slliw x5, x6, 5\n"
			"fle.s x6, f10, f3\n"
			"fle.pi f20, f20, f2\n"
			"fmul.ps f9, f9, f3, rdn\n"
			"ebreak\n"
			"fcvt.ps.pw f26, f25, rtz\n"
			"flt.ps f26, f10, f18\n"
			"fcmov.ps f3, f18, f15, f8\n"
			"bgeu x26, x29, 1f\n"
			"1:\n"
			"fbcx.ps f18, x28\n"
			"bne x12, x4, 1f\n"
			"1:\n"
			"fsetm.pi m2, f25\n"
			"fmvz.x.ps x7, f22, 3\n"
			"lui x23, 0x14\n"
			"mova.m.x x17\n"
			"subw x13, x10, x15\n"
			"fsub.pi f11, f22, f15\n"
			"fnmadd.ps f9, f2, f9, f15, rmm\n"
			"fltm.pi m7, f12, f9\n"
			"fsgnj.ps f3, f25, f3\n"
			"fsrai.pi f11, f10, 0x1\n"
			"blt x16, x24, 1f\n"
			"1:\n"
			"flem.ps m4, f10, f25\n"
			"sraiw x17, x17, 0\n"
			"fbci.pi f15, 0x44efc\n"
			"divw x6, x17, x15\n"
			"bne x20, x8, 1f\n"
			"1:\n"
			"fslli.pi f3, f3, 0x1\n"
			"fcvt.ps.pw f10, f20, rdn\n"
			"fsgnjx.ps f15, f8, f15\n"
			"feq.ps f18, f12, f20\n"
			"divw x23, x5, x23\n"
			"andi x14, x10, 0x12\n"
			"fpackrepb.pi f9, f9\n"
			"fsatu8.pi f15, f15\n"
			"addiw x6, x6, 0x10\n"
			"feq.pi f20, f22, f3\n"
			"add x13, x7, x28\n"
			"fsub.s f12, f18, f22, rmm\n"
			"slliw x15, x7, 2\n"
			"bne x13, x25, 1f\n"
			"1:\n"
			"sltu x14, x15, x5\n"
			"fcmovm.ps f15, f8, f20\n"
			"fcvt.wu.s x10, f12, rup\n"
			"fmulhu.pi f12, f25, f12\n"
			"fcvt.pwu.ps f10, f9, rmm\n"
			"fadd.ps f12, f15, f9, rdn\n"
			"addiw x13, x13, 0x1b\n"
			"sraw x15, x7, x28\n"
			"fmaxu.pi f8, f9, f8\n"
			"fxor.pi f0, f20, f10\n"
			"maskpopcz x25, m5\n"
			"bltu x8, x6, 1f\n"
			"1:\n"
			"feq.ps f0, f12, f15\n"
			"fsra.pi f15, f15, f9\n"
			"sllw x13, x7, x14\n"
			"bge x22, x31, 1f\n"
			"1:\n"
			"fmv.s.x f12, x13\n"
			"bltu x17, x29, 1f\n"
			"1:\n"
			"and x13, x13, x5\n"
			"fswizz.ps f20, f25, 0x19\n"
			"fnmsub.ps f12, f12, f18, f2, rmm\n"
			"fnmadd.ps f8, f8, f10, f9, rmm\n"
			"fandi.pi f26, f2, 0x108\n"
			"fmin.pi f10, f18, f3\n"
			"fcvt.wu.s x15, f3, rne\n"
			"divu x7, x28, x14\n"
			"fcvt.pwu.ps f15, f20, rmm\n"
			"bne x12, x31, 1f\n"
			"1:\n"
			"fbcx.ps f12, x13\n"
			"fcvt.s.wu f10, x14, rtz\n"
			"flt.pi f3, f3, f25\n"
			"fmax.ps f15, f20, f3\n"
			"flem.ps m4, f8, f22\n"
			"fnmsub.s f12, f12, f10, f10, rtz\n"
			"fcvt.pw.ps f2, f2, rtz\n"
			"add x10, x10, x13\n"
			"bne x20, x29, 1f\n"
			"1:\n"
			"divuw x14, x10, x15\n"
			"frsq.ps f3, f3\n"
			"fmul.s f15, f9, f18, rmm\n"
			"fandi.pi f9, f18, 0x117\n"
			"fnmadd.ps f4, f8, f25, f15, rne\n"
			"flt.s x6, f22, f2\n"
			"srliw x6, x10, 4\n"
			"remw x5, x23, x15\n"
			"bge x14, x3, 1f\n"
			"1:\n"
			"fsgnj.s f18, f8, f22\n"
			"mova.m.x x10\n"
			"srai x14, x17, 5\n"
			"fltm.pi m4, f22, f3\n"
			"slti x13, x7, 0x3\n"
			"fmadd.s f10, f3, f20, f12, rtz\n"
			"ori x6, x6, 0xc\n"
			"bgeu x2, x14, 1f\n"
			"1:\n"
			"fle.ps f3, f8, f9\n"
			"addi x21, x21, -1\n"
			"bne x21, x0, LBL_SEQID_4_LOOP_SEQ_HID_0\n"
			"li x5, 5\n" 
			"bne x30, x5, LBL_FAIL_HID_0\n"
			"nop \n"
			"nop \n"
			"addi x29, x29, -1\n"
			"bne x29, x0, LBL_OUTER_LOOP_SEQ_HID_0\n"
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f21,  768 (x5)\n"
			"flw.ps f26,  224 (x5)\n"
			"flw.ps f10,  832 (x5)\n"
			"flw.ps f8,  320 (x5)\n"
			"flw.ps f16,  448 (x5)\n"
			"flw.ps f4,  576 (x5)\n"
			"flw.ps f15,  160 (x5)\n"
			"flw.ps f27,  256 (x5)\n"
			"flw.ps f0,  672 (x5)\n"
			"flw.ps f6,  192 (x5)\n"
			"flw.ps f13,  480 (x5)\n"
			"flw.ps f1,  704 (x5)\n"
			"flw.ps f17,  64 (x5)\n"
			"flw.ps f31,  896 (x5)\n"
			"flw.ps f12,  864 (x5)\n"
			"flw.ps f24,  640 (x5)\n"
			"flw.ps f5,  736 (x5)\n"
			"flw.ps f30,  288 (x5)\n"
			"flw.ps f7,  32 (x5)\n"
			"flw.ps f29,  608 (x5)\n"
			"flw.ps f19,  800 (x5)\n"
			"flw.ps f11,  544 (x5)\n"
			"flw.ps f3,  352 (x5)\n"
			"flw.ps f28,  416 (x5)\n"
			"flw.ps f25,  384 (x5)\n"
			"flw.ps f20,  96 (x5)\n"
			"flw.ps f9,  128 (x5)\n"
			"flw.ps f18,  512 (x5)\n"
			"flw.ps f22,  928 (x5)\n"
			"flw.ps f14,  960 (x5)\n"
			"flw.ps f2,  992 (x5)\n"
			"flw.ps f23,  0 (x5)\n"
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
			"LBL_HPM_3_CORE_DCACHE_ACCESS0_FAIL_HID_0:\n"          
			"csrr x5, mhpmcounter3\n"
			"sd x5, 32 (x31)\n"
			"LBL_HPM_4_CORE_TXFMA_OPS32_FAIL_HID_0:\n"          
			"csrr x5, mhpmcounter4\n"
			"sd x5, 40 (x31)\n"
			"LBL_HPM_5_CORE_DCACHE_MISSES0_FAIL_HID_0:\n"          
			"csrr x5, mhpmcounter5\n"
			"sd x5, 48 (x31)\n"
			"LBL_HPM_6_CORE_BRANCHES0_FAIL_HID_0:\n"          
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
			"LBL_HPM_3_CORE_DCACHE_ACCESS0_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter3\n"
			"sd x5, 32 (x31)\n"
			"LBL_HPM_4_CORE_TXFMA_OPS32_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter4\n"
			"sd x5, 40 (x31)\n"
			"LBL_HPM_5_CORE_DCACHE_MISSES0_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter5\n"
			"sd x5, 48 (x31)\n"
			"LBL_HPM_6_CORE_BRANCHES0_PASS_HID_0:\n"          
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
		0x70e7c22c, 
		0xb2f6e446, 
		0x5030a95f, 
		0xac1de4e2, 
		0xbcdd0851, 
		0x818607ae, 
		0xb892258e, 
		0xb8803570, 
		0x53bf1cae, 
		0x23fb7e1c, 
		0xc8e1e2cb, 
		0xb648b165, 
		0x336c64bd, 
		0xba0d5eb8, 
		0x0913b979, 
		0x959bee1b, 
		0xd2b93e7c, 
		0xd1c60d9b, 
		0xf979bb77, 
		0x0c85c009, 
		0x99215555, 
		0x5ce3cb81, 
		0x2ef4d82c, 
		0xd8e2d4d6, 
		0x93c50ccc, 
		0xf3967599, 
		0x68cdae46, 
		0x0ad9ba9c, 
		0xea063077, 
		0x55328bb5, 
		0x18232340, 
		0x63a5ecbb, 
		0xbacc5219, 
		0x178d15e2, 
		0xf5b321b2, 
		0x194a11c3, 
		0xce67d12f, 
		0xaf09aca9, 
		0x8410a782, 
		0x64488243, 
		0x10b2a28f, 
		0xf9915868, 
		0xedbdede1, 
		0x464e3dbc, 
		0x60f77075, 
		0xbe09935a, 
		0xf89d70a5, 
		0x486f58ad, 
		0xa13a0fc9, 
		0x589323a9, 
		0xadd4ca61, 
		0xce38bd57, 
		0x867e2d67, 
		0x362f9864, 
		0x3db9758d, 
		0x8b43f779, 
		0xfcb335ed, 
		0xa85e1499, 
		0x6bd3c596, 
		0xf84a699f, 
		0x6e7bb663, 
		0xd07fe355, 
		0x3d5b8069, 
		0xa90ee22d, 
		0x583588be, 
		0x3df8600d, 
		0x8a2c4d51, 
		0x366346a5, 
		0x660a11db, 
		0x2e3a75e0, 
		0x1eec1de2, 
		0xe2e69b0e, 
		0x2a640f06, 
		0x040c275a, 
		0x049fbbf6, 
		0xa31d2d1e, 
		0xd75dcc1b, 
		0xd067466b, 
		0x6d45d7bf, 
		0x4480b28c, 
		0x7b3ae614, 
		0xcf8b6468, 
		0x22475aaf, 
		0x73a2a7b0, 
		0x700ec4ee, 
		0x0d8bdfc2, 
		0xe0971901, 
		0x3c2a7809, 
		0x095769ac, 
		0xaeb7d893, 
		0x8d2fed42, 
		0x659f1ad6, 
		0x9eebd3bb, 
		0x28c1297b, 
		0x0c0298f0, 
		0x3012989e, 
		0x7e99c34b, 
		0x08306d92, 
		0x4d89e528, 
		0x54c3b169, 
		0xc8603c6b, 
		0xb4132bad, 
		0xff41c2d0, 
		0x862ed0ef, 
		0x027342dc, 
		0x8c73283a, 
		0x2ba4d381, 
		0x4efb437f, 
		0x68a429da, 
		0x1485af63, 
		0xdc6c0171, 
		0x2b88fdb0, 
		0xdd85e7b9, 
		0xd4a81871, 
		0x1a4439fc, 
		0x2ff90936, 
		0x9b0aa351, 
		0x67227237, 
		0xa70c4207, 
		0xcdb9f692, 
		0xb7cb78c1, 
		0x470155a0, 
		0x10c86892, 
		0xa1d944d4, 
		0xa2bcc884, 
		0x0c0d7a4a, 
		0x7c27c83e, 
		0xc2b237e3, 
		0xf65c02fb, 
		0x0efc2623, 
		0xa5d583dc, 
		0xa202b7ab, 
		0x746ed102, 
		0xddeb6c5f, 
		0x20b39cd2, 
		0xd9fbb700, 
		0xdbc3666c, 
		0xc80d768f, 
		0xcc7853a6, 
		0xc767cc1a, 
		0x9816634b, 
		0xbf854feb, 
		0x4f6fd307, 
		0x656ce406, 
		0x7ce8c5ae, 
		0x6baebe5b, 
		0xce8271d1, 
		0x7237a2a2, 
		0x787e50c5, 
		0x42fc9990, 
		0x80be916f, 
		0xb8155bf0, 
		0x072b3b71, 
		0x72f15bd8, 
		0x05fc687a, 
		0xd9e0915b, 
		0x78875dd1, 
		0x048ed661, 
		0x6e36599d, 
		0xc57402ea, 
		0x988f4916, 
		0x3bde6663, 
		0x2f08de5d, 
		0x95d5b55d, 
		0x0d4fb950, 
		0xbaae6d25, 
		0xd00e5e33, 
		0xe6c58ec7, 
		0x0e3d09c6, 
		0xd3f9b923, 
		0xc2f464ca, 
		0x25ed8227, 
		0x33c8e885, 
		0x5ba4d6cf, 
		0x88aa8c4d, 
		0xb399bc62, 
		0x10be7879, 
		0xbb42f383, 
		0xa54589f7, 
		0x4a72ada7, 
		0x9cd1da9b, 
		0x9df04c2e, 
		0xfca94158, 
		0x6aa0b94d, 
		0x21202810, 
		0xfc50bff1, 
		0x23b1e5e7, 
		0xc5f63ce3, 
		0xa6ae69e6, 
		0x3095fcb7, 
		0xf0f246d5, 
		0x47f65647, 
		0xeb32ef6a, 
		0x7613651e, 
		0x2fe7a517, 
		0x69203274, 
		0x0cc8f9ad, 
		0x10548030, 
		0xe20019f0, 
		0xf91e3888, 
		0x9529673e, 
		0x83cd820d, 
		0x5a2a87cc, 
		0xcdcd97f6, 
		0x6b38ca51, 
		0x0a40f92e, 
		0x05b24a09, 
		0x0d8c2ef7, 
		0xac0d4167, 
		0xe87908e6, 
		0x561a274f, 
		0xad63c36e, 
		0xc834ca11, 
		0x2c0d8a90, 
		0xd1dcffd3, 
		0x489bbbb5, 
		0x3b8739e2, 
		0x1496c8d0, 
		0x04184665, 
		0x0e5cada9, 
		0xd7bec335, 
		0x3a7b72b8, 
		0xf7937579, 
		0x8ac1480b, 
		0xd4c8a3bc, 
		0x0cbbeb5a, 
		0x365727ed, 
		0x31063b22, 
		0x8a9c2f34, 
		0x3dd90887, 
		0xf61fe183, 
		0x5175ddf4, 
		0xa8121100, 
		0x2af2c8c6, 
		0x255caa9d, 
		0xdee61131, 
		0x9e954180, 
		0x9b962296, 
		0xbf4d26be, 
		0xd052202f, 
		0xd0287336, 
		0xf598e15a, 
		0xd0b85cc6, 
		0xa0b11dc9, 
		0x6e369e5f, 
		0x61c02319, 
		0xfb1ae213, 
		0xdff4c885, 
		0xd437c2bb, 
		0x7af0ccc7, 
		0x5a479e29, 
		0x8c2d1ea5, 
		0xc475256c, 
		0x815bd213, 
		0x7a00cfbf, 
		0xa3d9679f  
};
volatile uint16_t rand_ieee754_16[1536] __attribute__ ((aligned (2048))) = {
		0xd200, 
		0xcf00, 
		0xd3f2, 
		0xae69, 
		0x0e5c, 
		0x7d70, 
		0xd140, 
		0xd0a0, 
		0x92ec, 
		0x4b80, 
		0x0000, 
		0x6ec2, 
		0x4b00, 
		0x4b80, 
		0xcf40, 
		0x8000, 
		0x532d, 
		0x4e40, 
		0xcd40, 
		0x5320, 
		0xd100, 
		0x8000, 
		0xca80, 
		0xc900, 
		0x936e, 
		0xfd8c, 
		0x4f40, 
		0x5320, 
		0xd040, 
		0x918e, 
		0x1015, 
		0x44a3, 
		0x4d40, 
		0x5300, 
		0x4700, 
		0x0ec9, 
		0xd100, 
		0x6a6a, 
		0x27c1, 
		0x53e0, 
		0x14ca, 
		0x5340, 
		0x1fa9, 
		0x7c00, 
		0x5060, 
		0xba4c, 
		0xe602, 
		0xd340, 
		0x4f80, 
		0x0000, 
		0xb637, 
		0xce40, 
		0x0000, 
		0x2dc5, 
		0x7aea, 
		0x42ed, 
		0x5100, 
		0x18c3, 
		0x8759, 
		0x5120, 
		0xcb80, 
		0x8949, 
		0x53a0, 
		0xc200, 
		0xa661, 
		0x393f, 
		0x6ec7, 
		0xd120, 
		0x8000, 
		0x5240, 
		0x3121, 
		0x5000, 
		0xcb00, 
		0x5260, 
		0xccc0, 
		0x06de, 
		0x4a00, 
		0x4ec0, 
		0x4a00, 
		0x4600, 
		0xa258, 
		0x2437, 
		0xcc80, 
		0xd100, 
		0x5320, 
		0xd3e0, 
		0x6fd3, 
		0x51e0, 
		0xd3e0, 
		0x8000, 
		0xfc00, 
		0xc43a, 
		0x6b40, 
		0xd2a0, 
		0x66cd, 
		0xd300, 
		0xd320, 
		0xccc0, 
		0x0fa1, 
		0xcb00, 
		0x4dc0, 
		0x9f3d, 
		0xccc0, 
		0xd200, 
		0x5340, 
		0x4b80, 
		0x4a00, 
		0x4d00, 
		0x5180, 
		0x52e0, 
		0xe623, 
		0x53e0, 
		0xae34, 
		0x8433, 
		0x5200, 
		0xd0a0, 
		0xce80, 
		0x5320, 
		0x4700, 
		0x5928, 
		0x089c, 
		0x8000, 
		0x3c00, 
		0x51e0, 
		0x4000, 
		0xcdc0, 
		0x0015, 
		0x34b5, 
		0xd2e0, 
		0x5040, 
		0x5160, 
		0xd0c0, 
		0xd2a0, 
		0x0315, 
		0xd200, 
		0x4880, 
		0xd2a0, 
		0xd360, 
		0x51e0, 
		0xd060, 
		0xd1a0, 
		0x8552, 
		0x5380, 
		0x52a0, 
		0x2ea1, 
		0xcdc0, 
		0x7c00, 
		0x0000, 
		0xd1a0, 
		0xc700, 
		0x4000, 
		0xc200, 
		0x887d, 
		0xcd40, 
		0x03d6, 
		0xa20c, 
		0xbc00, 
		0x7985, 
		0xd100, 
		0x8000, 
		0x0000, 
		0xa472, 
		0xcc80, 
		0xd2e0, 
		0xc000, 
		0xc400, 
		0x07f1, 
		0x4500, 
		0xd180, 
		0x5180, 
		0x50e0, 
		0xd020, 
		0x9ba2, 
		0x6fa7, 
		0xd080, 
		0x5260, 
		0x4e00, 
		0x4200, 
		0x691c, 
		0xcc80, 
		0xce40, 
		0xb6ff, 
		0xca80, 
		0xd2c0, 
		0x5360, 
		0xd300, 
		0x9979, 
		0xb829, 
		0x4880, 
		0x2685, 
		0x5240, 
		0xd040, 
		0xcf80, 
		0x53a0, 
		0x0000, 
		0xd100, 
		0x48f9, 
		0xd120, 
		0x5260, 
		0xc700, 
		0x4d80, 
		0x5040, 
		0x8000, 
		0xc600, 
		0xae63, 
		0xd0e0, 
		0x7c00, 
		0xd220, 
		0x7cbe, 
		0x4600, 
		0xc0d5, 
		0xcf40, 
		0x09e0, 
		0x52e0, 
		0x5260, 
		0x8000, 
		0xca80, 
		0xbc5f, 
		0xd2a0, 
		0x8000, 
		0x8b2f, 
		0x7650, 
		0xd0a0, 
		0xccc0, 
		0xcf40, 
		0xa6dc, 
		0x47c8, 
		0x0000, 
		0xd2a0, 
		0xd120, 
		0x0000, 
		0x0000, 
		0xd040, 
		0x53e0, 
		0xfc60, 
		0xc200, 
		0x4d80, 
		0xb4de, 
		0xd240, 
		0x70b7, 
		0x0000, 
		0x4e00, 
		0x39ca, 
		0x4400, 
		0xca00, 
		0x0813, 
		0xd260, 
		0x0e51, 
		0xbd6c, 
		0x5000, 
		0x19a9, 
		0x52a0, 
		0x7a3e, 
		0x7a23, 
		0xcd80, 
		0xcc00, 
		0x4b80, 
		0x7c00, 
		0xcc40, 
		0x5080, 
		0xd120, 
		0x52a0, 
		0xfb02, 
		0xd380, 
		0x4a80, 
		0xad41, 
		0xd240, 
		0xd220, 
		0xe332, 
		0xc700, 
		0x5020, 
		0xc800, 
		0x50e0, 
		0x4f40, 
		0xc4dd, 
		0x4cc0, 
		0x7c00, 
		0xc880, 
		0xd2e0, 
		0x495d, 
		0x83aa, 
		0x4600, 
		0xc200, 
		0xdfa2, 
		0xc400, 
		0x5380, 
		0x4880, 
		0x0000, 
		0x4f00, 
		0x7ab6, 
		0xfc00, 
		0xcc40, 
		0xc980, 
		0xfc00, 
		0x4f80, 
		0x5340, 
		0x52e0, 
		0xc500, 
		0xcd40, 
		0xbc5f, 
		0xfecc, 
		0x5320, 
		0xd220, 
		0xd240, 
		0xd120, 
		0x5040, 
		0x53e0, 
		0xd080, 
		0xd0db, 
		0xd2e0, 
		0xcec0, 
		0xcd40, 
		0xd53a, 
		0xd0c0, 
		0xc700, 
		0xc500, 
		0xc400, 
		0x52c0, 
		0xd140, 
		0xd020, 
		0x15fa, 
		0xcd00, 
		0xd300, 
		0x454f, 
		0x4600, 
		0x5260, 
		0x0bb5, 
		0x5060, 
		0x5060, 
		0x5380, 
		0xc400, 
		0xd280, 
		0x5240, 
		0xcfc0, 
		0x4500, 
		0x5140, 
		0xada5, 
		0xa673, 
		0xf248, 
		0x43ef, 
		0xfd0e, 
		0x5180, 
		0xbac8, 
		0xd305, 
		0x51e0, 
		0x4500, 
		0x51a0, 
		0xfc00, 
		0xd180, 
		0xd2c0, 
		0xe912, 
		0xc900, 
		0xf2dc, 
		0x52e0, 
		0xd3e0, 
		0x81d2, 
		0xd220, 
		0x073b, 
		0x4f00, 
		0x4e80, 
		0xd0a0, 
		0xd0a0, 
		0xab83, 
		0x2807, 
		0x65d9, 
		0x0000, 
		0xcf80, 
		0x53a0, 
		0x0ca3, 
		0x5360, 
		0x01f6, 
		0x7c55, 
		0x4c40, 
		0x5080, 
		0xc880, 
		0x5020, 
		0x9403, 
		0x046b, 
		0x993b, 
		0xd380, 
		0x8000, 
		0x4a80, 
		0x915f, 
		0xc400, 
		0x2382, 
		0x7c00, 
		0x1f8a, 
		0xc700, 
		0xf2eb, 
		0xc600, 
		0xd240, 
		0xcc40, 
		0xc900, 
		0xd0e0, 
		0x8000, 
		0x05d0, 
		0x5300, 
		0x69c2, 
		0xd140, 
		0xca00, 
		0xce00, 
		0x52a0, 
		0xca00, 
		0xc700, 
		0x5240, 
		0x87ad, 
		0x9c35, 
		0x35f8, 
		0xd0a0, 
		0xd280, 
		0xbed0, 
		0xd060, 
		0x4e40, 
		0x0dec, 
		0xd1a0, 
		0xd060, 
		0x5a13, 
		0xf911, 
		0xcec0, 
		0xc900, 
		0xcec0, 
		0xd140, 
		0x5349, 
		0xd220, 
		0x6c6e, 
		0xcdc0, 
		0x0a51, 
		0xf60f, 
		0x4400, 
		0x8000, 
		0x4f00, 
		0xc980, 
		0x5020, 
		0xd060, 
		0x0ef3, 
		0xc000, 
		0x4c80, 
		0xd060, 
		0x5380, 
		0x6070, 
		0x5180, 
		0x4e00, 
		0x5040, 
		0x5320, 
		0x4600, 
		0xeba5, 
		0x1e53, 
		0xd200, 
		0x2490, 
		0x4c40, 
		0xcf40, 
		0x7cfb, 
		0x0000, 
		0xd160, 
		0xfc00, 
		0xca80, 
		0x3638, 
		0xc600, 
		0x834e, 
		0x5040, 
		0xfc00, 
		0xc500, 
		0x7c00, 
		0xd180, 
		0x7c00, 
		0x4c02, 
		0x5100, 
		0xc58d, 
		0xd280, 
		0x4500, 
		0xd160, 
		0xc400, 
		0x5000, 
		0x53c0, 
		0x24b2, 
		0xc96e, 
		0x0000, 
		0xe0a9, 
		0x7c00, 
		0xd340, 
		0xd060, 
		0x4700, 
		0x5020, 
		0x8000, 
		0xd1c0, 
		0x51e0, 
		0xd1e0, 
		0xa516, 
		0x3c06, 
		0x4949, 
		0xce75, 
		0x50a0, 
		0xd080, 
		0x4600, 
		0x0000, 
		0x0000, 
		0xd240, 
		0x5120, 
		0x5220, 
		0x7dcb, 
		0xd0e0, 
		0x9217, 
		0x2bba, 
		0xfc69, 
		0xc662, 
		0xd240, 
		0x7cfd, 
		0x638b, 
		0x4ec0, 
		0x5000, 
		0xa225, 
		0x4600, 
		0xcd80, 
		0xd0c0, 
		0x4e40, 
		0x5320, 
		0xfc76, 
		0x4e2c, 
		0xb760, 
		0x07e9, 
		0x0280, 
		0x0000, 
		0xc400, 
		0xd2a0, 
		0x0000, 
		0xce40, 
		0xd529, 
		0x4d80, 
		0x0000, 
		0x4400, 
		0xbc00, 
		0x4b00, 
		0x6353, 
		0x3369, 
		0xd320, 
		0x6ff3, 
		0xd1c0, 
		0x8000, 
		0xb33b, 
		0x7f48, 
		0xefe4, 
		0xd23a, 
		0x546d, 
		0xd200, 
		0xd3a0, 
		0xd200, 
		0x51a0, 
		0x4600, 
		0x5180, 
		0x0bc9, 
		0x5060, 
		0xd1a0, 
		0xd3a0, 
		0xccc0, 
		0x8000, 
		0x4564, 
		0xd040, 
		0xe4c7, 
		0xcd80, 
		0xd2e0, 
		0xca80, 
		0x6c94, 
		0x52a0, 
		0xcfc0, 
		0xbe27, 
		0xd140, 
		0xbbfd, 
		0x0125, 
		0xbf6b, 
		0x52e0, 
		0xd080, 
		0xce00, 
		0x0000, 
		0xd040, 
		0xcbec, 
		0xce00, 
		0xce00, 
		0x8388, 
		0x8000, 
		0x5360, 
		0x797a, 
		0xd260, 
		0x5260, 
		0x0b9f, 
		0x5588, 
		0x4d40, 
		0x4c00, 
		0x0000, 
		0x4b9d, 
		0xcdc0, 
		0x4600, 
		0xa053, 
		0xc400, 
		0x52a0, 
		0x5240, 
		0x38be, 
		0x5300, 
		0x0215, 
		0xec9d, 
		0xdac3, 
		0xcec0, 
		0xb15c, 
		0xd3c0, 
		0xfc00, 
		0x9eb6, 
		0x4b80, 
		0xfda5, 
		0xae9e, 
		0x5160, 
		0xce00, 
		0x4e80, 
		0x505c, 
		0xc600, 
		0x8000, 
		0xfc00, 
		0x5340, 
		0xda0a, 
		0xcc3b, 
		0xfc00, 
		0x5280, 
		0x5060, 
		0x5240, 
		0x4fc0, 
		0xc200, 
		0xcf80, 
		0xcd40, 
		0xce80, 
		0xccc0, 
		0xd3c0, 
		0x4d80, 
		0x4900, 
		0x50a0, 
		0x7c00, 
		0xce00, 
		0x3c65, 
		0x4500, 
		0xfc00, 
		0x4c80, 
		0x5120, 
		0xfc00, 
		0x7d8b, 
		0x5280, 
		0xbc00, 
		0x1291, 
		0x0000, 
		0xebaa, 
		0x4dbe, 
		0xd0c8, 
		0x5260, 
		0x55ea, 
		0xd180, 
		0xd0c0, 
		0x5907, 
		0xd3a0, 
		0x4b80, 
		0xd280, 
		0x50e0, 
		0x53e0, 
		0x5f7b, 
		0x53a0, 
		0xe866, 
		0xcc40, 
		0xfc00, 
		0xd300, 
		0x5140, 
		0xd340, 
		0x4000, 
		0x5160, 
		0x4fc0, 
		0x244f, 
		0x4c80, 
		0xd140, 
		0xd140, 
		0x52c0, 
		0x0000, 
		0x0000, 
		0x9d1a, 
		0xcf00, 
		0x4f40, 
		0xfc00, 
		0xcd80, 
		0x8643, 
		0xd040, 
		0xd858, 
		0x4a43, 
		0xd120, 
		0xcc80, 
		0x5080, 
		0xc400, 
		0x4a80, 
		0xc800, 
		0xd2e0, 
		0x27d1, 
		0x5260, 
		0x8000, 
		0x50a0, 
		0x5140, 
		0x4e40, 
		0x1f94, 
		0x4800, 
		0xcb80, 
		0xd320, 
		0x1090, 
		0x8000, 
		0x53e0, 
		0x5040, 
		0xb551, 
		0xc980, 
		0x4c40, 
		0xc700, 
		0xcd3b, 
		0x4d82, 
		0xc500, 
		0xcfc0, 
		0xbbe5, 
		0xfc74, 
		0x94f2, 
		0x0000, 
		0xec34, 
		0xc980, 
		0xd6b2, 
		0xfc26, 
		0x021c, 
		0x4b00, 
		0x8f32, 
		0x0000, 
		0xd1e0, 
		0xba82, 
		0xd020, 
		0xfc00, 
		0xa246, 
		0x4f80, 
		0xd380, 
		0x4dc0, 
		0xcc80, 
		0x670d, 
		0x74f7, 
		0xcc40, 
		0x50e0, 
		0xcfc0, 
		0x2f73, 
		0x68c7, 
		0xfd76, 
		0xc000, 
		0xcb80, 
		0x9bde, 
		0xbcad, 
		0xfc00, 
		0x6ccb, 
		0xc700, 
		0xd280, 
		0xd040, 
		0x8000, 
		0xd3a0, 
		0x5180, 
		0x5140, 
		0x4ac8, 
		0x2e69, 
		0x7c00, 
		0x1392, 
		0x7016, 
		0xc500, 
		0x50c0, 
		0x51a0, 
		0x4b00, 
		0x53c0, 
		0xd380, 
		0x5040, 
		0x50e0, 
		0xa51d, 
		0x776c, 
		0x51c0, 
		0x4800, 
		0x5180, 
		0xd0a0, 
		0x5140, 
		0xcdc0, 
		0xd120, 
		0x35aa, 
		0x51c0, 
		0x8bee, 
		0x4ec0, 
		0x51a0, 
		0x4500, 
		0x480c, 
		0x22aa, 
		0x8000, 
		0x4e40, 
		0x14cf, 
		0x4400, 
		0x60fb, 
		0xd2c0, 
		0x52db, 
		0x8000, 
		0x7922, 
		0xd040, 
		0xc91e, 
		0x74f3, 
		0x5220, 
		0x53c0, 
		0x7398, 
		0xd220, 
		0x3bc2, 
		0x5060, 
		0xd160, 
		0x7dc1, 
		0x7877, 
		0x7c81, 
		0xcd40, 
		0x4c40, 
		0x4600, 
		0x7451, 
		0xd000, 
		0x65a6, 
		0xdea2, 
		0x8000, 
		0x4c00, 
		0xd000, 
		0xca00, 
		0x0000, 
		0x0000, 
		0xc200, 
		0xd3a0, 
		0x5060, 
		0xd2e0, 
		0x4880, 
		0xd0e0, 
		0xce00, 
		0xd040, 
		0xd380, 
		0xce40, 
		0x4f00, 
		0xa2f0, 
		0xd1a0, 
		0x83cd, 
		0x8559, 
		0x8d30, 
		0x5060, 
		0xc400, 
		0xfc00, 
		0xca3f, 
		0x2cb2, 
		0xd9c8, 
		0xd180, 
		0x5cd8, 
		0xca80, 
		0xc700, 
		0xce56, 
		0x0000, 
		0x0000, 
		0xc200, 
		0x9529, 
		0x0000, 
		0x7c00, 
		0x7c00, 
		0x202e, 
		0xd1e0, 
		0xfc00, 
		0xf7da, 
		0xcf80, 
		0xf477, 
		0xd1a0, 
		0x5380, 
		0xd040, 
		0xd2a0, 
		0xfc00, 
		0xca00, 
		0x3317, 
		0x50a0, 
		0x5100, 
		0x02c9, 
		0x51a0, 
		0xd1a5, 
		0xd0c0, 
		0x6b65, 
		0xacf9, 
		0x6bc7, 
		0x52a0, 
		0xd000, 
		0xc900, 
		0x51a0, 
		0xc800, 
		0x0a57, 
		0xd160, 
		0xdf20, 
		0x008c, 
		0xef40, 
		0x0000, 
		0xd260, 
		0xcf40, 
		0x6849, 
		0xc700, 
		0xd0a0, 
		0xd340, 
		0x4400, 
		0xc600, 
		0x4ec0, 
		0xd300, 
		0xce80, 
		0xcd40, 
		0xfd01, 
		0x3408, 
		0x5280, 
		0x3c00, 
		0x32b6, 
		0xd100, 
		0x8000, 
		0xd020, 
		0x5040, 
		0xfd45, 
		0xbfa7, 
		0x04e5, 
		0xdbf3, 
		0xca80, 
		0xfc00, 
		0xd300, 
		0x8000, 
		0xd9bd, 
		0x6737, 
		0xc685, 
		0x042e, 
		0x4ec0, 
		0xcfc0, 
		0x6303, 
		0x4a80, 
		0x3c00, 
		0x0000, 
		0xd3a0, 
		0xccc0, 
		0xf4d8, 
		0x1c01, 
		0xc700, 
		0x0000, 
		0x5100, 
		0x0000, 
		0x5eb9, 
		0x0629, 
		0x745a, 
		0x7661, 
		0x4f80, 
		0xb92c, 
		0xd3e0, 
		0xb522, 
		0xce80, 
		0xfcd7, 
		0x2fa0, 
		0xcb80, 
		0xda6c, 
		0x4ec0, 
		0x5300, 
		0xcb00, 
		0xe0f0, 
		0xab60, 
		0x4000, 
		0x5c30, 
		0xcd40, 
		0xd100, 
		0xd0e0, 
		0x0000, 
		0xdffc, 
		0x5040, 
		0xd1c0, 
		0x3e88, 
		0xc800, 
		0xd160, 
		0x8b3c, 
		0xf11d, 
		0xd1e0, 
		0x51c0, 
		0x4900, 
		0x4c12, 
		0x4b80, 
		0x5e4a, 
		0x292c, 
		0xb4a9, 
		0xc600, 
		0x8000, 
		0xc800, 
		0x5498, 
		0xfa9c, 
		0x22bd, 
		0x8000, 
		0xa41e, 
		0x2b91, 
		0x51c0, 
		0x9e59, 
		0xc980, 
		0xc880, 
		0xa03c, 
		0xcf00, 
		0xd180, 
		0x5020, 
		0x5240, 
		0xcd80, 
		0x85c1, 
		0xbc00, 
		0xd1e0, 
		0xe08c, 
		0xd3a0, 
		0xc79d, 
		0xe21f, 
		0x4b00, 
		0x62a7, 
		0xc980, 
		0xc900, 
		0x5260, 
		0x5340, 
		0x5360, 
		0x05c3, 
		0x4400, 
		0x50c0, 
		0x5080, 
		0x4c40, 
		0xcc40, 
		0x184b, 
		0xd160, 
		0x50c0, 
		0x5160, 
		0x9f4f, 
		0xcc00, 
		0xcaea, 
		0xd3c0, 
		0xcb00, 
		0x5040, 
		0x0000, 
		0x4d40, 
		0xd080, 
		0x4b80, 
		0x8069, 
		0x8abc, 
		0xcd00, 
		0xc980, 
		0xaa49, 
		0xd1a0, 
		0x4800, 
		0x0cc7, 
		0x4d45, 
		0x248d, 
		0x4400, 
		0x51c0, 
		0x5080, 
		0xd3e0, 
		0x244a, 
		0xc700, 
		0xd000, 
		0x4cf9, 
		0x53e0, 
		0xd1c0, 
		0x0445, 
		0xcb00, 
		0x2720, 
		0x0000, 
		0x8000, 
		0xd0a0, 
		0xfc00, 
		0x7c00, 
		0x4b00, 
		0x6e18, 
		0x3c00, 
		0xd100, 
		0xc000, 
		0x5080, 
		0xde2f, 
		0xcf80, 
		0x4cc0, 
		0x5100, 
		0x8000, 
		0x8000, 
		0xc03d, 
		0x5040, 
		0x0006, 
		0x8000, 
		0xac4f, 
		0xc912, 
		0xf5c6, 
		0xa116, 
		0xa1dc, 
		0x0000, 
		0xcb80, 
		0x4fc4, 
		0xd0c0, 
		0xdf45, 
		0xfc00, 
		0x2175, 
		0xd040, 
		0x7c33, 
		0xca9e, 
		0xd380, 
		0xcc28, 
		0x8000, 
		0x5000, 
		0xd3e0, 
		0x5020, 
		0x9179, 
		0x3338, 
		0x5280, 
		0xa6b5, 
		0xcd80, 
		0xd1a0, 
		0x4c00, 
		0xcfc0, 
		0x2517, 
		0x460f, 
		0x5220, 
		0x4c00, 
		0xcc80, 
		0x3998, 
		0x5340, 
		0x4fa6, 
		0xce80, 
		0x51c0, 
		0xcf80, 
		0x3932, 
		0xc847, 
		0xfc00, 
		0x0000, 
		0xc200, 
		0xc900, 
		0xd320, 
		0xd3c0, 
		0x7c00, 
		0x0000, 
		0xf0e3, 
		0x8157, 
		0xe0c8, 
		0x0000, 
		0xd160, 
		0xfd72, 
		0x0f7d, 
		0xcb00, 
		0x4f40, 
		0x4b80, 
		0xd3a0, 
		0xc26b, 
		0xbd17, 
		0xd0c0, 
		0xcc40, 
		0xcdc0, 
		0xd751, 
		0xd3a0, 
		0x4a88, 
		0xd2c0, 
		0x020d, 
		0xce00, 
		0x0623, 
		0xd180, 
		0x8b8b, 
		0x5140, 
		0x4b80, 
		0xd0e0, 
		0x5000, 
		0x0630, 
		0xd200, 
		0x4500, 
		0xd320, 
		0x5320, 
		0xce80, 
		0x7c00, 
		0xd100, 
		0xc700, 
		0x4880, 
		0xf0f6, 
		0x77c2, 
		0x9d9f, 
		0xca00, 
		0xce2b, 
		0xc000, 
		0x6ce0, 
		0xfc00, 
		0x0000, 
		0x90ae, 
		0x7c00, 
		0xd260, 
		0x4880, 
		0x724f, 
		0x65ea, 
		0x50a0, 
		0x8000, 
		0x5220, 
		0xfa5b, 
		0x8000, 
		0x4400, 
		0xd220, 
		0xcf80, 
		0xd525, 
		0xd120, 
		0x4b80, 
		0xd320, 
		0x7d50, 
		0x5240, 
		0x4900, 
		0xa84c, 
		0xf1ea, 
		0x8000, 
		0xc980, 
		0x8000, 
		0xc700, 
		0x0000, 
		0x3c7b, 
		0x53e0, 
		0x292d, 
		0xfd8a, 
		0xd3a0, 
		0xd360, 
		0xfcc7, 
		0xd120, 
		0x7c00, 
		0x0000, 
		0xdff7, 
		0xd140, 
		0xe766, 
		0xcb80, 
		0x3c00, 
		0xbc00, 
		0x298a, 
		0x4d80, 
		0x1da1, 
		0xc700, 
		0x52a0, 
		0xd100, 
		0x5060, 
		0x7573, 
		0xbe23, 
		0x4900, 
		0x1920, 
		0xd340, 
		0x9c57, 
		0x00fb, 
		0xde63, 
		0xc200, 
		0x5060, 
		0xde36, 
		0xd320, 
		0x3973, 
		0xcb00, 
		0xcd40, 
		0xd493, 
		0x52e0, 
		0x4400, 
		0x7430, 
		0xcc40, 
		0xd240, 
		0xd160, 
		0x52a0, 
		0x52c0, 
		0xcb00, 
		0x0000, 
		0xd240, 
		0xcb80, 
		0x8551, 
		0x5cf0, 
		0x4880, 
		0xcc80, 
		0x5280, 
		0x50a0, 
		0x5080, 
		0x50e0, 
		0x4b29, 
		0x4cc0, 
		0x5080, 
		0x5040, 
		0x5080, 
		0xca80, 
		0x5100, 
		0x5140, 
		0xd120, 
		0xada8, 
		0x6357, 
		0xc200, 
		0x7431, 
		0xce00, 
		0x5300, 
		0xd100, 
		0x4ec0, 
		0x52e0, 
		0x51e0, 
		0x57fd, 
		0xeb0c, 
		0x1c31, 
		0xc000, 
		0xcb80, 
		0x16ec, 
		0x5120, 
		0x4500, 
		0xd120, 
		0x50a0, 
		0xbe9a, 
		0xce40, 
		0x5340, 
		0x9c91, 
		0x50c0, 
		0xd100, 
		0x5200, 
		0xa3c1, 
		0xd2c0, 
		0xfdb5, 
		0xe516, 
		0x5240, 
		0x5200, 
		0x4880, 
		0x5020, 
		0x40e5, 
		0xd0a0, 
		0x5280, 
		0xcc86, 
		0x51e0, 
		0x896c, 
		0xd100, 
		0x2935, 
		0xffc3, 
		0xd240, 
		0x4e80, 
		0x4e80, 
		0x3c00, 
		0xd2a0, 
		0xd300, 
		0x4f80, 
		0x5320, 
		0x5910, 
		0x4b80, 
		0xd240, 
		0x5140, 
		0x7e56, 
		0xc980, 
		0xfdd7, 
		0x515a, 
		0xd6bc, 
		0xcc41, 
		0x50c0, 
		0x4ec0, 
		0xd0a0, 
		0x276b, 
		0x5240, 
		0x4900, 
		0xce80, 
		0x58ea, 
		0xd6a0, 
		0x0156, 
		0x4cc0, 
		0x4800, 
		0x0000, 
		0xc3a6, 
		0x5320, 
		0xd0e0, 
		0xfc00, 
		0xb1ca, 
		0x4880, 
		0xd36e, 
		0x4700, 
		0x7277, 
		0xd1e0, 
		0x5280, 
		0x0000, 
		0xd577, 
		0x51c0, 
		0xd1e0, 
		0x5100, 
		0x653b, 
		0x0000, 
		0xf9cd, 
		0x4737, 
		0x7a2a, 
		0xcdc0, 
		0x42df, 
		0xd0ce, 
		0xcfc0, 
		0x5380, 
		0xb15e, 
		0xd320, 
		0xd140, 
		0xce80, 
		0x15b6, 
		0xc800, 
		0x4d00, 
		0x4e00, 
		0x73c6, 
		0x4000, 
		0x0000, 
		0xd240, 
		0xd2a0, 
		0x7c00, 
		0x0000, 
		0xfc00, 
		0x5280, 
		0x52e0, 
		0xcca8, 
		0x4a00, 
		0xf614, 
		0xcc00, 
		0x4f40, 
		0x8000, 
		0x5080, 
		0x5080, 
		0x8328, 
		0x5380, 
		0x4400, 
		0xce00, 
		0x5320, 
		0x4f80, 
		0x7c59, 
		0xd592, 
		0x8000, 
		0x5200, 
		0x816f, 
		0x9bfd, 
		0x7c78, 
		0x0000, 
		0x01e0, 
		0xc980, 
		0xca80, 
		0xfbb8, 
		0xce00, 
		0x01a2, 
		0x57a5, 
		0xb038, 
		0x1308, 
		0xc175, 
		0xccc0, 
		0x7c00, 
		0x5300, 
		0x4d40, 
		0xd340, 
		0xfc00, 
		0xe81a, 
		0x4880, 
		0x5100, 
		0xcdc0, 
		0x5080, 
		0x5060, 
		0x5340, 
		0x4c80, 
		0x4e40, 
		0x4f00, 
		0x5220, 
		0x5240, 
		0xc600, 
		0x0000, 
		0xd240, 
		0x4f80, 
		0x5040, 
		0xabf2, 
		0xc880, 
		0x4200, 
		0xdaea, 
		0xce40, 
		0xd080, 
		0x5360, 
		0xce40, 
		0xa51a, 
		0xbc8a, 
		0xcc40, 
		0xc880, 
		0x50a0, 
		0x0275, 
		0x4b00, 
		0xfc00, 
		0x527a, 
		0xd040, 
		0x4880, 
		0xd080, 
		0x4b00, 
		0xcd40, 
		0xca80, 
		0x8012, 
		0xbdc1, 
		0xcb80, 
		0x8000, 
		0xd120, 
		0xbc00, 
		0xd3e0, 
		0xd120, 
		0x5060, 
		0xcc80, 
		0xcd80, 
		0x2636, 
		0x213a, 
		0x50e0, 
		0x2ecd, 
		0x3dbf, 
		0x4900, 
		0x4b80, 
		0xa676, 
		0xcc00, 
		0x7d43, 
		0x1435, 
		0x2dc3, 
		0x4b00, 
		0x53c0, 
		0xd360, 
		0xd1e0, 
		0xd180, 
		0xca00, 
		0xcc00, 
		0x7c00, 
		0x2b94, 
		0x31cd, 
		0xd120, 
		0xca00, 
		0x0e24, 
		0x8c4b, 
		0xf70e, 
		0xd060, 
		0x53e0, 
		0x4c00, 
		0x4700, 
		0x4c00, 
		0xcdc0, 
		0x0619, 
		0xcfc0, 
		0x8352, 
		0x4e00, 
		0x97e4, 
		0x5b21, 
		0x8c5c, 
		0xd200, 
		0x4ec0, 
		0xd2e0, 
		0xd340, 
		0x4600, 
		0x5100, 
		0xc309, 
		0xc000, 
		0x4d00, 
		0x4ccf, 
		0x4fc0, 
		0x464c, 
		0xd060, 
		0xcf40, 
		0x3ed5, 
		0x8000, 
		0xd240, 
		0x0000, 
		0x4ec0, 
		0xc000, 
		0x4ec0, 
		0x4c00  
};
volatile uint32_t rand_ieee754_32[1536] __attribute__ ((aligned (2048))) = {
		0xc0e00000, 
		0x80800002, 
		0x00000200, 
		0x0c400000, 
		0xc0000000, 
		0xfb2f7185, 
		0x4b8c4b40, 
		0xafa8f064, 
		0xe8f7a19f, 
		0x7f9d6668, 
		0x859f0442, 
		0xd4eff765, 
		0xfb3a8d92, 
		0x0e000003, 
		0x32470de4, 
		0xf9e018fd, 
		0x80000010, 
		0x42240000, 
		0x00800003, 
		0x42240000, 
		0x80000001, 
		0xecacef4f, 
		0x80011111, 
		0xc2740000, 
		0xc27c0000, 
		0xc2600000, 
		0xc1300000, 
		0x80008000, 
		0xc1a80000, 
		0x80000000, 
		0x7f800000, 
		0x40e00000, 
		0xd0c23912, 
		0x4e148656, 
		0xa6cf3260, 
		0xde7b1c1a, 
		0x80000000, 
		0x80000000, 
		0x80100000, 
		0x007fffff, 
		0x80800003, 
		0x0d00fff0, 
		0xc26c0000, 
		0xc1e80000, 
		0x00000400, 
		0xf7d3e984, 
		0xc1980000, 
		0xc2200000, 
		0x41f00000, 
		0x41a00000, 
		0x42080000, 
		0x804d26d1, 
		0xde441059, 
		0x00000000, 
		0x0c7ffe00, 
		0xc1200000, 
		0x00080000, 
		0x00000010, 
		0x2e18e059, 
		0xff7ffffe, 
		0x00000000, 
		0x3881ce2a, 
		0xc1980000, 
		0x511015e0, 
		0xffbfffff, 
		0x00800003, 
		0xc1900000, 
		0x41b00000, 
		0x00000000, 
		0xc1a00000, 
		0x9db6f396, 
		0x7fe1efe1, 
		0xff800000, 
		0x714b099f, 
		0x0e7fffff, 
		0x0c7fffff, 
		0xfc9044cd, 
		0x80011111, 
		0x80000000, 
		0xa4707577, 
		0x7faf27e5, 
		0x00020000, 
		0xb3012176, 
		0x00080000, 
		0x0c7fe000, 
		0xc2440000, 
		0x80000200, 
		0xc1700000, 
		0x00800001, 
		0x0e00ffff, 
		0x0e0fffff, 
		0xadd57b89, 
		0x80000010, 
		0x40a00000, 
		0x8f7ffffe, 
		0x65ddec2a, 
		0xcccccccc, 
		0x42040000, 
		0xb1216d8b, 
		0x2f45862d, 
		0x00537822, 
		0x80800001, 
		0x0c700000, 
		0x002c4938, 
		0x0e00001f, 
		0x00800001, 
		0x7f7ffffe, 
		0xd5cd7656, 
		0x7fc00000, 
		0x00800000, 
		0x42280000, 
		0x00000000, 
		0x0e3fffff, 
		0x7ffba791, 
		0x42780000, 
		0x00000100, 
		0xd8a48936, 
		0x00800001, 
		0x00000000, 
		0x802a7b71, 
		0x00000100, 
		0x80800000, 
		0x80000004, 
		0xbf028f5c, 
		0x210bbbb9, 
		0xb878d202, 
		0x80000000, 
		0x3606b5e3, 
		0x7fc00001, 
		0x0c7ff000, 
		0xe4131a27, 
		0x80002000, 
		0x00011111, 
		0x40800000, 
		0x80000000, 
		0xc2180000, 
		0x0c7ffffe, 
		0xc2500000, 
		0x80000000, 
		0x80000002, 
		0x0e0fffff, 
		0x00000000, 
		0x80800001, 
		0xbf800000, 
		0x42640000, 
		0x41880000, 
		0x80080000, 
		0x425c0000, 
		0xff800000, 
		0x0c7fffc0, 
		0x649210f6, 
		0xff7ffffe, 
		0x0c7fff00, 
		0x00800003, 
		0xc2140000, 
		0x80000000, 
		0xc1a00000, 
		0x0d000ff0, 
		0x80000800, 
		0xff800000, 
		0x9389a6f9, 
		0x00000000, 
		0x6d06cc8a, 
		0x80000000, 
		0x756e8669, 
		0x8806d5c9, 
		0xc2440000, 
		0x9719b7e5, 
		0xc2500000, 
		0xff800000, 
		0x64fa6fd3, 
		0x00000000, 
		0x0e00ffff, 
		0x41b80000, 
		0x7f800000, 
		0x8b5480c1, 
		0x00811d9b, 
		0x8f7ffffd, 
		0x55a8f86a, 
		0x420c0000, 
		0x0f7ffffe, 
		0x00000800, 
		0x80000000, 
		0x3765e5f0, 
		0x0c7ffc00, 
		0xf467437d, 
		0xc1c80000, 
		0x7f800000, 
		0x07f4c591, 
		0xc1500000, 
		0x80000001, 
		0xff800000, 
		0xd3de24d8, 
		0x40a00000, 
		0x7fbfffff, 
		0x0e0000ff, 
		0x00800003, 
		0x80008000, 
		0x95ddb554, 
		0x40400000, 
		0x33333333, 
		0xc2240000, 
		0xc2080000, 
		0xf2d62eb2, 
		0x7f7ffffe, 
		0x0e0fffff, 
		0x7f7ffffe, 
		0xc2740000, 
		0x00000010, 
		0x01efc715, 
		0x5e095b39, 
		0x0e3fffff, 
		0xc1e80000, 
		0xf688e5bc, 
		0x42500000, 
		0x41800000, 
		0x7fb00982, 
		0x3ddb70ca, 
		0xc1600000, 
		0x00800000, 
		0x55068b0c, 
		0xffbfffff, 
		0x0c7f0000, 
		0x0c7fff80, 
		0x80000001, 
		0x8cdb63a0, 
		0xffc00000, 
		0x0c600000, 
		0x00000001, 
		0x7f800000, 
		0x0e0fffff, 
		0xffab71a9, 
		0xffbc1308, 
		0x80000000, 
		0x00000000, 
		0x007fffff, 
		0x42080000, 
		0x0f7ffffe, 
		0xff800000, 
		0xcc24b6e3, 
		0x00000000, 
		0xc0f19093, 
		0xc1700000, 
		0x80000000, 
		0x424c0000, 
		0x8147c4d9, 
		0x7f800000, 
		0x80684708, 
		0x955e0f41, 
		0x80000001, 
		0xc2380000, 
		0x42680000, 
		0x80800002, 
		0xc54d0207, 
		0xd24bcf57, 
		0x0e000001, 
		0xff7ffffe, 
		0x013a52c1, 
		0xeb9b512c, 
		0x53cac758, 
		0xb2e0f8f6, 
		0xcb000000, 
		0x3b8db1de, 
		0xc2100000, 
		0x422c0000, 
		0x42440000, 
		0x6c9decde, 
		0xc2680000, 
		0x4b000000, 
		0x80800001, 
		0xe408b69e, 
		0x40c00000, 
		0x80800001, 
		0xff000000, 
		0xc20c0000, 
		0x4e217cc3, 
		0xc91cf715, 
		0xeca25816, 
		0x63368df8, 
		0x7fb5163d, 
		0x80000000, 
		0xdb466fc8, 
		0x0e3fffff, 
		0xff876035, 
		0x42040000, 
		0x8f7fffff, 
		0x00040000, 
		0xff7ffffe, 
		0x41d80000, 
		0xdb46bf85, 
		0x00000000, 
		0x80800001, 
		0x48885a94, 
		0x0c7ffc00, 
		0x53110b1d, 
		0x424c0000, 
		0x41500000, 
		0x80800003, 
		0x0cbc2ecd, 
		0x199f143b, 
		0x00000020, 
		0x421c0000, 
		0x41b00000, 
		0x0e1fffff, 
		0x0c7f0000, 
		0x04b28d80, 
		0xb3fdb9e5, 
		0x638b09c5, 
		0x53c8bfa5, 
		0x7fbfffff, 
		0x23dd6f2f, 
		0x087642ac, 
		0x00800001, 
		0x0e00ffff, 
		0xe8e76e75, 
		0x42700000, 
		0x0c7fffe0, 
		0x0c7f8000, 
		0x00000001, 
		0x80000000, 
		0x2a1a65aa, 
		0x7fb45bfa, 
		0x41800000, 
		0x80800001, 
		0x26f7050b, 
		0x00000001, 
		0x420c0000, 
		0x0c7ffffc, 
		0x41f00000, 
		0x41b00000, 
		0x98e9276c, 
		0xc0e00000, 
		0x00020000, 
		0x00800001, 
		0x422c0000, 
		0x00000100, 
		0xff800000, 
		0x427c0000, 
		0x00800001, 
		0xff88acef, 
		0xed097cad, 
		0x7f800000, 
		0x80004000, 
		0xd83cd4d8, 
		0xc26c0000, 
		0x42140000, 
		0xc1000000, 
		0x421c0000, 
		0x80000000, 
		0x0e0007ff, 
		0x00000400, 
		0x00000000, 
		0xb7375f35, 
		0x7f7ffffe, 
		0x00000800, 
		0xbf800001, 
		0x41c00000, 
		0x0f7ffffd, 
		0x5539c7bb, 
		0xaaaaaaaa, 
		0xbf028f5c, 
		0x42280000, 
		0x00000000, 
		0xc2140000, 
		0x41980000, 
		0x41c80000, 
		0xc8d6d0c1, 
		0x0f7ffffd, 
		0x00200000, 
		0x762a2fa0, 
		0x7f7ffffe, 
		0x0dd61bad, 
		0x41400000, 
		0xcb8c4b40, 
		0x00000002, 
		0x510e3bcd, 
		0x00000000, 
		0x425c0000, 
		0xc0e00000, 
		0x3f800001, 
		0x007fffff, 
		0x00000008, 
		0x41a00000, 
		0x42300000, 
		0x00000008, 
		0xc1300000, 
		0xc2480000, 
		0x00000002, 
		0x00000000, 
		0x4d53f78f, 
		0x80000000, 
		0x00000000, 
		0x00000000, 
		0xc1c80000, 
		0x0e0003ff, 
		0x42700000, 
		0x4a388651, 
		0x2ca1235c, 
		0x80000400, 
		0xf0782591, 
		0x0e7fffff, 
		0x0c7f8000, 
		0xc1100000, 
		0xc2700000, 
		0x71b71423, 
		0x42700000, 
		0x06e98ab1, 
		0xf1bc4e56, 
		0x55555555, 
		0x80ffffff, 
		0xc24c0000, 
		0x00000000, 
		0x80000000, 
		0xfd879e61, 
		0x0e003fff, 
		0x41400000, 
		0x0e00007f, 
		0x41e00000, 
		0x51ca2b9b, 
		0x69931cb3, 
		0x8f7ffffc, 
		0x80000000, 
		0x80000000, 
		0x80000080, 
		0x0c400000, 
		0xff800000, 
		0x72bd7d53, 
		0x80800001, 
		0x6f047be6, 
		0x0c7ffc00, 
		0x7fbfffff, 
		0x3da5ea9f, 
		0x2a2fc8cc, 
		0x42380000, 
		0xe422fe8d, 
		0x057d457d, 
		0xc6a8cf72, 
		0x80000001, 
		0x7f800000, 
		0x00000000, 
		0x096f6ca0, 
		0x00000000, 
		0x0e00000f, 
		0x6e2e3234, 
		0x41000000, 
		0x8f7ffffc, 
		0x41880000, 
		0x2e4404c1, 
		0x300c493e, 
		0x80011111, 
		0x80080000, 
		0x7f000000, 
		0x80000400, 
		0x00000002, 
		0x00800000, 
		0xc25c0000, 
		0x42200000, 
		0x41100000, 
		0x3e609346, 
		0x5c0d7a07, 
		0xcccccccc, 
		0x1730c1e0, 
		0x41700000, 
		0xc7277616, 
		0x40000000, 
		0xc47469b6, 
		0x80000001, 
		0xd43650f0, 
		0x80000001, 
		0x00011111, 
		0x96f023da, 
		0x80002000, 
		0x41000000, 
		0x00000001, 
		0x425c0000, 
		0xc0c7ec6b, 
		0x0d00fff0, 
		0x33333333, 
		0xc2580000, 
		0x41000000, 
		0x42340000, 
		0x9842983c, 
		0x8dc8b762, 
		0x0c700000, 
		0xc1300000, 
		0x80000000, 
		0x7fc00001, 
		0x41f00000, 
		0x7fc00000, 
		0x488caab3, 
		0x80020000, 
		0xff800001, 
		0x42680000, 
		0x0e0fffff, 
		0x00000040, 
		0xc25c0000, 
		0xcecd8dbd, 
		0x0e7fffff, 
		0x80800001, 
		0x7f800000, 
		0x40c00000, 
		0x33333333, 
		0xa7437074, 
		0xb6338848, 
		0x11d79208, 
		0xb012d7d8, 
		0xc0800000, 
		0x0e003fff, 
		0xff800001, 
		0x0e000007, 
		0x80ffffff, 
		0x00004000, 
		0x41500000, 
		0x0e000007, 
		0x0c7ffffe, 
		0xc2600000, 
		0x7f800000, 
		0x3f800000, 
		0x00000000, 
		0x00400000, 
		0xff800000, 
		0x0c7f0000, 
		0x80800002, 
		0x00000000, 
		0x0d000ff0, 
		0x80000001, 
		0xc0c00000, 
		0x80800001, 
		0x0c7fffff, 
		0x80000001, 
		0x1f8154d7, 
		0x41500000, 
		0x41700000, 
		0x9ea614ee, 
		0xff7ffffe, 
		0x00000000, 
		0x0c7fff00, 
		0xbe050aa7, 
		0xc2400000, 
		0xc2540000, 
		0x00000400, 
		0x16e64323, 
		0x42000000, 
		0x80000020, 
		0xc2680000, 
		0x55555555, 
		0x80001000, 
		0x0e000fff, 
		0xc22c0000, 
		0xc2500000, 
		0xc1000000, 
		0xbff4d899, 
		0xaaaaaaaa, 
		0x8f7ffffc, 
		0x42780000, 
		0x80000002, 
		0xc1600000, 
		0x7d17bdce, 
		0xff800000, 
		0xc2400000, 
		0x80800000, 
		0x4ac8404b, 
		0xfffa7cc3, 
		0xc2640000, 
		0xb33a9eef, 
		0x41700000, 
		0xa4406265, 
		0xb2a7a3fd, 
		0x42200000, 
		0x42680000, 
		0xff800000, 
		0x40400000, 
		0x7fffffff, 
		0x7084d238, 
		0x80001000, 
		0x42300000, 
		0x42600000, 
		0x00400000, 
		0xc20c0000, 
		0x3f800001, 
		0x8f7ffffd, 
		0x80000800, 
		0x0c700000, 
		0x00000000, 
		0x631a5580, 
		0x00000000, 
		0x80000000, 
		0xc1d80000, 
		0xcccccccc, 
		0x14fa8fce, 
		0xade68501, 
		0x47f61c8c, 
		0xff800000, 
		0xb9160aff, 
		0xffac4b8a, 
		0xff7ffffe, 
		0x41c00000, 
		0x41e80000, 
		0x41400000, 
		0x00000002, 
		0x645e74ba, 
		0xc2680000, 
		0xf548d869, 
		0x41e80000, 
		0x0c7fffc0, 
		0x805aee48, 
		0xff800000, 
		0x41800000, 
		0x04666a5f, 
		0x80020000, 
		0x80000000, 
		0x0c7fff00, 
		0xc2540000, 
		0xc1e80000, 
		0x3f800000, 
		0x42780000, 
		0x03b3336e, 
		0x0c3dfca4, 
		0x2513e6a9, 
		0x41d80000, 
		0x6b59a967, 
		0xd4faf802, 
		0xc2100000, 
		0x80800002, 
		0x80800001, 
		0x7fb58e36, 
		0x0c7c0000, 
		0x0c7ff000, 
		0x04ae592d, 
		0x0c7ffc00, 
		0x84abb6c8, 
		0xb6fd9894, 
		0xa2fdb163, 
		0x0c7fff00, 
		0xc1200000, 
		0x7f7ffffe, 
		0x0f7ffffc, 
		0x10442ff0, 
		0x00000000, 
		0x0c7ffffc, 
		0x1b8fa9a7, 
		0x00200000, 
		0x0c7ffffc, 
		0xcd6e340c, 
		0x7fc00001, 
		0x80000001, 
		0x007fffff, 
		0x8793160d, 
		0x6655dc26, 
		0x42680000, 
		0xc1c00000, 
		0x0e003fff, 
		0x80000100, 
		0xff7ffffe, 
		0x367f81bd, 
		0xcb000000, 
		0xf1042c0c, 
		0x42640000, 
		0x64531a07, 
		0xc1900000, 
		0x42180000, 
		0x80000040, 
		0x80000000, 
		0x50a0e5c9, 
		0x7f7ffffe, 
		0x0d000ff0, 
		0x00000010, 
		0x80800002, 
		0x41b00000, 
		0xa90feaf4, 
		0x0f7ffffe, 
		0xbf028f5c, 
		0x80000002, 
		0xc1b80000, 
		0xeabb3671, 
		0x80800003, 
		0xe48760b3, 
		0xc2780000, 
		0x7f000000, 
		0x42240000, 
		0x0c7ff800, 
		0x00800000, 
		0x00400000, 
		0x80000001, 
		0x0c7ffe00, 
		0x41b80000, 
		0x80004000, 
		0x0e1fffff, 
		0x1bccce33, 
		0xcf397db6, 
		0xae4e99e8, 
		0x7f800000, 
		0x41c00000, 
		0x00400000, 
		0x00002000, 
		0x00040000, 
		0x272e8c67, 
		0x00200000, 
		0x80000000, 
		0x9c1c7927, 
		0xc2400000, 
		0xa26ebf11, 
		0x4985d4a3, 
		0xaee2d894, 
		0xe48a35f5, 
		0x7f000000, 
		0x3796815d, 
		0x855b8d05, 
		0x41980000, 
		0xabebe2f4, 
		0x01cdc5e0, 
		0x42180000, 
		0x87b12abb, 
		0x00000001, 
		0x0e07ffff, 
		0x41c00000, 
		0x80001000, 
		0x3ec50b4e, 
		0xc1e00000, 
		0x80000000, 
		0x007ffffe, 
		0x0045b3bc, 
		0x31a69702, 
		0x00000040, 
		0x00000000, 
		0x41a80000, 
		0x80000000, 
		0x00ffffff, 
		0x00000000, 
		0x42700000, 
		0x80040000, 
		0x80800001, 
		0x74053d2b, 
		0x941847b4, 
		0xc1f00000, 
		0xc8cf4ab6, 
		0x0c7ff800, 
		0xc20c0000, 
		0x80008000, 
		0x783deab2, 
		0x41a80000, 
		0x8c771525, 
		0xe4bf051f, 
		0x7f800001, 
		0x80010000, 
		0x7f800000, 
		0x5df41385, 
		0x25f2ada9, 
		0x0e00003f, 
		0x0c7ffff0, 
		0x0c7ffffc, 
		0x0e000001, 
		0x0c7ffffe, 
		0x80008000, 
		0xcd597ecb, 
		0xc0800000, 
		0x80100000, 
		0x42240000, 
		0x0c7fff80, 
		0x807ffffe, 
		0x00000001, 
		0xd0772d45, 
		0x7f93f218, 
		0x0c7ffc00, 
		0x55555555, 
		0x6613e930, 
		0xd34121df, 
		0x0e00ffff, 
		0xffffffff, 
		0xfb60b122, 
		0x80800001, 
		0x42780000, 
		0x7f800000, 
		0xc2680000, 
		0xc2340000, 
		0x00000040, 
		0x80000400, 
		0xc1700000, 
		0xc0e00000, 
		0xe78fabd4, 
		0x3f800001, 
		0x00000000, 
		0x0c780000, 
		0xc1d80000, 
		0xeb974637, 
		0x80008000, 
		0x80020000, 
		0xdcf37b54, 
		0x41a80000, 
		0x80000008, 
		0x8f7fffff, 
		0x42740000, 
		0x00000400, 
		0xc2440000, 
		0x80000100, 
		0x0d76b83f, 
		0x7f800000, 
		0x80000000, 
		0x0e0003ff, 
		0xbf800001, 
		0x4b000000, 
		0xb360afb0, 
		0xff800000, 
		0x00000200, 
		0x007fffff, 
		0x80000000, 
		0x0e000001, 
		0x427c0000, 
		0x41f80000, 
		0x75fefff4, 
		0x0c7fffe0, 
		0xffb606a5, 
		0x5f786fd1, 
		0x80000100, 
		0x4b8c4b40, 
		0x80000002, 
		0x80008000, 
		0x0e0007ff, 
		0x80000200, 
		0x76151487, 
		0x42000000, 
		0x00000004, 
		0x426c0000, 
		0xbf800001, 
		0xc1a00000, 
		0x00000000, 
		0xbf800000, 
		0x42580000, 
		0xad06fd8c, 
		0xc0c00000, 
		0x0c7ffff0, 
		0xcb8c4b40, 
		0x80001000, 
		0x57e8da12, 
		0x0e0fffff, 
		0x53afec7a, 
		0xc1d00000, 
		0xb8b05b42, 
		0x00000000, 
		0xff7ffffe, 
		0xd79db0fa, 
		0x7f7ffffe, 
		0x42340000, 
		0x00200000, 
		0x00800000, 
		0x3ef12df2, 
		0xc22c0000, 
		0x7f800000, 
		0x07afd62f, 
		0x42780000, 
		0xc2280000, 
		0x80800000, 
		0x7f800000, 
		0x80080000, 
		0x3f800000, 
		0x8f7ffffd, 
		0x7f800000, 
		0xa57d9647, 
		0x7f7ffffe, 
		0xc1f80000, 
		0x0c7fffe0, 
		0x75fed8bf, 
		0x40000000, 
		0xc1c00000, 
		0x41f80000, 
		0x80000000, 
		0x9cc1fc40, 
		0xc1d80000, 
		0x0e01ffff, 
		0xc2740000, 
		0x65328710, 
		0xa44c75f2, 
		0x80000000, 
		0x0e000001, 
		0x0c7fe000, 
		0x00000000, 
		0xc26c0000, 
		0x42000000, 
		0x41b80000, 
		0x0db92289, 
		0xaf267571, 
		0x42200000, 
		0x7f800000, 
		0x61b0bf59, 
		0x00800002, 
		0xc1880000, 
		0x0e003fff, 
		0xee0e539e, 
		0x0c7ffffe, 
		0x41500000, 
		0xc2480000, 
		0x498b1f1b, 
		0x80000000, 
		0x80800002, 
		0x007fffff, 
		0x0c7ffffc, 
		0x80002000, 
		0x41800000, 
		0x0e00ffff, 
		0x80080000, 
		0x00000001, 
		0xc2280000, 
		0x71b369ca, 
		0x00000001, 
		0x969232e8, 
		0xff800000, 
		0x9ff9a4a6, 
		0x42700000, 
		0x0e00003f, 
		0xc21c0000, 
		0x7f800001, 
		0x0c7c0000, 
		0xfaebb524, 
		0x990adea0, 
		0x79951b46, 
		0x5170ed20, 
		0xc27c0000, 
		0x845ff0e1, 
		0x80000020, 
		0x42340000, 
		0xff7ffffe, 
		0xc24c0000, 
		0x00800001, 
		0x0003e7a6, 
		0x99444470, 
		0x88fe6300, 
		0xc2580000, 
		0x7f7ffffe, 
		0xc2380000, 
		0x971c58a1, 
		0x00000020, 
		0xdddbf9a0, 
		0x41f00000, 
		0x0c7ffe00, 
		0xe8112bfc, 
		0x7f7ffffe, 
		0x80008000, 
		0xc1f00000, 
		0x00000000, 
		0xc2600000, 
		0x00000000, 
		0x80800000, 
		0xc2640000, 
		0xc1c00000, 
		0x13671620, 
		0x80000000, 
		0x0e00001f, 
		0x01e72915, 
		0x06759c0f, 
		0xff7ffffe, 
		0xc2340000, 
		0x00000010, 
		0x7f7fffff, 
		0x80000000, 
		0xff800000, 
		0x925326a1, 
		0x420c0000, 
		0x80800000, 
		0x80000000, 
		0x5fa109f9, 
		0x80000002, 
		0x0c7fff80, 
		0xc2340000, 
		0x00800001, 
		0xc1980000, 
		0xc0400000, 
		0x0e7fffff, 
		0xff7ffffe, 
		0x80002000, 
		0x3f028f5c, 
		0xc1800000, 
		0x80000000, 
		0xc1c80000, 
		0x41300000, 
		0xbf800001, 
		0x80641c43, 
		0x42680000, 
		0x378590a4, 
		0x42400000, 
		0x42400000, 
		0x80000040, 
		0x59399773, 
		0x69277d09, 
		0x0c7f0000, 
		0x7f7ffffe, 
		0x0e00001f, 
		0x00000001, 
		0x00800001, 
		0x80000002, 
		0xc0400000, 
		0xd6101414, 
		0xed84171b, 
		0x0e0000ff, 
		0x0e000007, 
		0xd90ef4a1, 
		0x42140000, 
		0x0e00007f, 
		0x427c0000, 
		0x80000000, 
		0xc2540000, 
		0x0e003fff, 
		0x442cd72c, 
		0x426c0000, 
		0xf8cb6788, 
		0x3f800000, 
		0x0e03ffff, 
		0x80000020, 
		0x42040000, 
		0xffdfaf89, 
		0x66c2c89a, 
		0xfafeeb80, 
		0x80002000, 
		0x00000000, 
		0x0e01ffff, 
		0x0c780000, 
		0x7f800000, 
		0xffb2b8c3, 
		0x801ca583, 
		0xff800000, 
		0xc21c0000, 
		0x5ec2d023, 
		0x7fbdbfed, 
		0x00800001, 
		0x0e1fffff, 
		0xf4b57134, 
		0x422c0000, 
		0x2fd91080, 
		0x4262dc80, 
		0x80000000, 
		0x7f99b1d2, 
		0x48c908b1, 
		0x3afc0adb, 
		0xc24c0000, 
		0x85f8b4a2, 
		0x0e0fffff, 
		0x4e969422, 
		0x00010000, 
		0x0e0000ff, 
		0xc2100000, 
		0x0c7ff800, 
		0x6cb1a956, 
		0x00040000, 
		0x9ef0a1a0, 
		0x00800002, 
		0x80040000, 
		0x2aca94ec, 
		0x7fbc97b7, 
		0x00011111, 
		0xc2680000, 
		0x92de9c2f, 
		0x0e00ffff, 
		0xc1c80000, 
		0x0c7fc000, 
		0x00000000, 
		0xff800000, 
		0x0e0001ff, 
		0x7f800001, 
		0x0c7ff000, 
		0x80000800, 
		0x80000010, 
		0x80000000, 
		0xc1e80000, 
		0x80000100, 
		0xf1b7ed5e, 
		0x7f95eeb7, 
		0x0f7ffffd, 
		0xc2240000, 
		0xc1300000, 
		0xaaaaaaaa, 
		0x0e0fffff, 
		0x424c0000, 
		0x6dacb52a, 
		0x42240000, 
		0x0c7ffffe, 
		0x41d00000, 
		0xc1a80000, 
		0x41500000, 
		0x7f800000, 
		0x587a9388, 
		0x7f800000, 
		0x3925ccc5, 
		0xc2740000, 
		0x7f7ffffe, 
		0x5650611c, 
		0x42100000, 
		0xbc58741b, 
		0x80008000, 
		0xd9492805, 
		0x00400000, 
		0xc1d00000, 
		0x80000000, 
		0xc1b80000, 
		0xffbfffff, 
		0x00011111, 
		0x40e00000, 
		0x0e0000ff, 
		0x0e007fff, 
		0xc2180000, 
		0x42040000, 
		0x00400000, 
		0x7f800001, 
		0x00008000, 
		0x7f800000, 
		0x807d10e9, 
		0xc2200000, 
		0x0e00007f, 
		0xc1c80000, 
		0x007fffff, 
		0xff800000, 
		0x41e80000, 
		0x44c626e3, 
		0x42380000, 
		0xc432a3f5, 
		0xc2780000, 
		0x0f7ffffe, 
		0x4ad07108, 
		0x00000200, 
		0xfec58b87, 
		0x00800002, 
		0x6c755f2c, 
		0x00040000, 
		0x52c8136c, 
		0xc2500000, 
		0xff800000, 
		0x7f800001, 
		0x80ffffff, 
		0xff800000, 
		0xc23c0000, 
		0x8f7fffff, 
		0xc27c0000, 
		0xa4654a32, 
		0x80800003, 
		0xd3769491, 
		0x00008000, 
		0x0e7fffff, 
		0x0c7ffff0, 
		0x00001000, 
		0x0c7ffffc, 
		0x425c0000, 
		0xff800000, 
		0xbf800001, 
		0x083a2584, 
		0x7f800000, 
		0x00200000, 
		0x03002aa8, 
		0x7f800000, 
		0xb60c650c, 
		0xffc00000, 
		0x3cf85ea0, 
		0x004b1b3e, 
		0x2f022ca0, 
		0x5d8f696a, 
		0xc648c604, 
		0x7fc00000, 
		0x80000002, 
		0x41c80000, 
		0xaf4cf3c3, 
		0xf310e35c, 
		0xc1c00000, 
		0x0d00fff0, 
		0x0e1fffff, 
		0xc2300000, 
		0x80000000, 
		0x0e1fffff, 
		0xc2240000, 
		0x41000000, 
		0xc1a00000, 
		0x00800002, 
		0x48c74789, 
		0x00000400, 
		0xff800000, 
		0xb82aee6d, 
		0x42740000, 
		0x5eff7890, 
		0x80004000, 
		0x2cf1540a, 
		0x00f9de01, 
		0x7fffffff, 
		0x0e03ffff, 
		0xc2080000, 
		0x80800002, 
		0x8319b50a, 
		0x00000000, 
		0xe49bfc01, 
		0x00400000, 
		0xcedf051c, 
		0xa55179b8, 
		0x7f800000, 
		0x00000000, 
		0xc2040000, 
		0x0e00ffff, 
		0xcc1fd6d1, 
		0x7f800000, 
		0x0f7fffff, 
		0x00000000, 
		0x00000100, 
		0x41d00000, 
		0x80000001, 
		0x24442422, 
		0xc2240000, 
		0x3f028f5c, 
		0x0d000ff0, 
		0x80008000, 
		0x0c7f0000, 
		0x0c7ffe00, 
		0x0c7ffffe, 
		0x0c7ffe00, 
		0x80000004, 
		0x0c7ff800, 
		0x0fe95bd6, 
		0x422c0000, 
		0x40400000, 
		0x80000400, 
		0x0d000ff0, 
		0x00800003, 
		0x00040000, 
		0x41880000, 
		0xc22c0000, 
		0x0f7ffffc, 
		0x0d000ff0, 
		0x0c7ffff0, 
		0x2466fe8e, 
		0x36f61c4b, 
		0xff800000, 
		0x80407b75, 
		0x80000000, 
		0xc27c0000, 
		0x42500000, 
		0x41300000, 
		0xc2380000, 
		0xb39e1299, 
		0x36edeeef, 
		0x0e1fffff, 
		0x7f800000, 
		0x0e00001f, 
		0x7fb32204, 
		0xec1c4078, 
		0xc1b80000, 
		0xc1900000, 
		0x80000002, 
		0x41c80000, 
		0x80200000, 
		0x41300000, 
		0xff7ffffe, 
		0x40c00000, 
		0x7fc00001, 
		0x0c400000, 
		0xc1600000, 
		0x87ca5309, 
		0x42680000, 
		0x3bb91d60, 
		0x80020000, 
		0x00000000, 
		0x57cbe2ba, 
		0x7f7ffffe, 
		0x41c80000, 
		0xff800000, 
		0x0e000001, 
		0xc1100000, 
		0x80200000, 
		0xc1100000, 
		0xe123cfa0, 
		0x0c7fff80, 
		0x41b80000, 
		0x0c7fff00, 
		0xa7569686, 
		0xff7fffff, 
		0x0e000001, 
		0x40400000, 
		0x7025841b, 
		0xffc00000, 
		0x00011111, 
		0x06ed90dc, 
		0x8060a619, 
		0x3a4c8beb, 
		0x00000010, 
		0x00400000, 
		0x80000008, 
		0x00000001, 
		0xc24c0000, 
		0x0c7ffffe, 
		0x00000020, 
		0x41700000, 
		0x00800001, 
		0x80000000, 
		0x80000000, 
		0xc2100000, 
		0x5146fa12, 
		0xc923056b, 
		0xc2400000, 
		0x2e3dc7f9, 
		0x1e56aa80, 
		0x41500000, 
		0x41300000, 
		0x31fe8555, 
		0x4a788b0d, 
		0x4cd3d7ed, 
		0xb3c523b5, 
		0xc2280000, 
		0x973239cc, 
		0xc1200000, 
		0x41b80000, 
		0xa92ad08a, 
		0x0e0000ff, 
		0xfc68b412, 
		0xb81fc52e, 
		0x44ecc536, 
		0xe71b6539, 
		0x0c7e0000, 
		0xb4fef1e9, 
		0xade67e63, 
		0xffabf359, 
		0xdaca79e5, 
		0x7f800000, 
		0x99afd3c6, 
		0x0e00ffff, 
		0x7f7fffff, 
		0xaa79cbdc, 
		0x41f00000, 
		0x3223c50b, 
		0xff800000, 
		0x40800000, 
		0xf69c4c4a, 
		0x0a5a0865, 
		0xff800000, 
		0x41c80000, 
		0x7fffffff, 
		0x00000100, 
		0x00000002, 
		0x80011111, 
		0xffa8b45d, 
		0x11d977e4, 
		0x41880000, 
		0x80000000, 
		0xd3215e1c, 
		0x95672740, 
		0x26c9fb34, 
		0xc2680000, 
		0x00000008, 
		0x41600000, 
		0xb5d13669, 
		0x9978d751, 
		0x9ab80bc7, 
		0xff800000, 
		0x6e11e9fb, 
		0xc2500000, 
		0x80000002, 
		0x32b24971, 
		0xc1500000, 
		0x007ffffe, 
		0xc45fb832, 
		0x42780000, 
		0x80ffffff, 
		0x0e01ffff, 
		0xc27c0000, 
		0x8f7ffffd, 
		0x41f00000, 
		0x42340000, 
		0xf7465672, 
		0x41400000, 
		0x1620ec8e, 
		0x55f75964, 
		0xb25ed33f, 
		0x2a70440e, 
		0xc2700000, 
		0xef2654ab, 
		0x00000001, 
		0x00011111, 
		0x0e3fffff, 
		0xbf800001, 
		0x41700000, 
		0x8b5f1af1, 
		0x88b8e30c, 
		0xbf159ced, 
		0x804d46d5, 
		0x4c6fa635, 
		0xa2f2b3ea, 
		0x40c00000, 
		0x828b2ca0, 
		0x0c7fffff, 
		0xffd23269, 
		0x132abffd, 
		0x80800000, 
		0x42300000, 
		0x7f800001, 
		0x88267f40, 
		0x00001000, 
		0x0c7fe000, 
		0x0c7fffe0, 
		0x7fc00001, 
		0xffe08beb, 
		0x41b80000, 
		0xc1600000, 
		0x40000000, 
		0x764357ec, 
		0xc2380000, 
		0x00002000, 
		0xff800000, 
		0x9542116e, 
		0x6e82f6c3, 
		0x7f800001, 
		0x42080000, 
		0xba334d51, 
		0x80800000, 
		0xff81e2c1, 
		0x800241e3, 
		0xc1000000, 
		0x0e03ffff, 
		0x41900000, 
		0x42200000, 
		0xff7ffffe, 
		0xc1400000, 
		0x9dc52a1e, 
		0x42340000, 
		0xc21c0000, 
		0xc2400000, 
		0x7fc00000, 
		0xeb92f7bd, 
		0x60f15a9c, 
		0x00000000, 
		0x80372b96, 
		0x0e07ffff, 
		0xc1600000, 
		0x00000000, 
		0x0e3fffff, 
		0x42180000, 
		0x99052a96, 
		0xc2080000, 
		0xde14aaec, 
		0x72439f59, 
		0x802e08ba, 
		0x7fc00001, 
		0x00000002, 
		0xdb544269, 
		0x41880000, 
		0x80000020, 
		0x9e76e0e6, 
		0x80000000, 
		0x0e000fff, 
		0x82485e10, 
		0x40000000, 
		0xc2080000, 
		0xc1500000, 
		0x80000010, 
		0x3b22ad65, 
		0xc2700000, 
		0x80000040, 
		0x0e0000ff, 
		0xc2200000, 
		0xff800000, 
		0x42300000, 
		0x7f7ffffe, 
		0xb88b670b, 
		0xcccccccc, 
		0xc6de6c8b, 
		0x7fffffff, 
		0x00000000, 
		0x41300000, 
		0x76bf1647, 
		0xff800000, 
		0x0e1fffff, 
		0xff1800c9, 
		0xc375907b, 
		0xc21c0000, 
		0x0c7ff800, 
		0x425c0000, 
		0x944d84d9, 
		0x41d80000, 
		0xbbf1f2ac, 
		0x8f7fffff, 
		0xc2080000, 
		0x007ffffe, 
		0x4d1cfb2a, 
		0x41500000, 
		0x7f7ffffe, 
		0x0ce599c0, 
		0xe93cbbdc, 
		0xc2540000, 
		0x6021dee0, 
		0x48124d96, 
		0x41d00000, 
		0x00000100, 
		0x3f800000, 
		0x7fc00000, 
		0x48252292, 
		0xb5a63777, 
		0x42700000, 
		0x3f028f5c, 
		0x94050744, 
		0xc5fa7f8a, 
		0xc1e00000, 
		0xbf800000, 
		0x41980000, 
		0x00000001, 
		0x41300000, 
		0x80000400, 
		0x40800000, 
		0x4b8c4b40, 
		0x6fa0024d, 
		0x7fc00001, 
		0x00000000, 
		0x00000100, 
		0x0c7ffffe, 
		0x07d6cf64, 
		0x80000000, 
		0xff800000, 
		0x0c7fffc0, 
		0x25903152, 
		0x50b342f6, 
		0x0c7f8000, 
		0x80000001, 
		0xc0000000, 
		0x006a17cc, 
		0x445efb2f, 
		0x0c400000, 
		0x7f7fffff, 
		0xc1400000, 
		0x7fb28eaf, 
		0x41500000, 
		0x38d2c784, 
		0xc2700000, 
		0xe7426c1a, 
		0xbf38cab2, 
		0x00000004, 
		0xc24c0000, 
		0xe11119b8, 
		0xff800000, 
		0xf061cd6e, 
		0x41f00000, 
		0x9d6005de, 
		0x6d3fca99, 
		0xaaaaaaaa, 
		0x0e00000f, 
		0x5c6d4eea, 
		0x42440000, 
		0x00080000  
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

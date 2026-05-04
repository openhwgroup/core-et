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
			"csrwi mhpmevent4, 4\n"   
			"csrwi mhpmevent5, 28\n"   
			"csrwi mhpmevent6, 21\n"   
			"csrwi mhpmevent7, 7\n"   
			"csrwi mhpmevent8, 5\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f10,  32 (x5)\n"
			"flw.ps f28,  128 (x5)\n"
			"flw.ps f27,  416 (x5)\n"
			"flw.ps f18,  224 (x5)\n"
			"flw.ps f7,  768 (x5)\n"
			"flw.ps f15,  512 (x5)\n"
			"flw.ps f26,  928 (x5)\n"
			"flw.ps f17,  320 (x5)\n"
			"flw.ps f4,  288 (x5)\n"
			"flw.ps f31,  800 (x5)\n"
			"flw.ps f14,  256 (x5)\n"
			"flw.ps f0,  352 (x5)\n"
			"flw.ps f22,  160 (x5)\n"
			"flw.ps f21,  192 (x5)\n"
			"flw.ps f12,  64 (x5)\n"
			"flw.ps f1,  960 (x5)\n"
			"flw.ps f30,  864 (x5)\n"
			"flw.ps f16,  832 (x5)\n"
			"flw.ps f11,  544 (x5)\n"
			"flw.ps f23,  704 (x5)\n"
			"flw.ps f24,  736 (x5)\n"
			"flw.ps f5,  992 (x5)\n"
			"flw.ps f9,  896 (x5)\n"
			"flw.ps f19,  576 (x5)\n"
			"flw.ps f3,  96 (x5)\n"
			"flw.ps f2,  448 (x5)\n"
			"flw.ps f29,  480 (x5)\n"
			"flw.ps f8,  0 (x5)\n"
			"flw.ps f25,  640 (x5)\n"
			"flw.ps f6,  384 (x5)\n"
			"flw.ps f13,  672 (x5)\n"
			"flw.ps f20,  608 (x5)\n"
			"LBL_VADDR_INIT_HID_0:\n"          
			"li x25, 0x81001c0000 \n"                 
			"fsw.ps f9, (x25) \n"                 
			"li x15, 0x8101540020 \n"                 
			"fsw.ps f10, (x15) \n"                 
			"li x22, 0x810337aec0 \n"                 
			"fsw.ps f24, (x22) \n"                 
			"li x24, 0x8103760100 \n"                 
			"fsw.ps f16, (x24) \n"                 
			"li x17, 0x8104e400a0 \n"                 
			"fsw.ps f21, (x17) \n"                 
			"li x22, 0x81058a0020 \n"                 
			"fsw.ps f4, (x22) \n"                 
			"li x19, 0x8106170080 \n"                 
			"fsw.ps f18, (x19) \n"                 
			"li x23, 0x8108d1b020 \n"                 
			"fsw.ps f1, (x23) \n"                 
			"li x15, 0x810af52000 \n"                 
			"fsw.ps f26, (x15) \n"                 
			"li x24, 0x810b078020 \n"                 
			"fsw.ps f26, (x24) \n"                 
			"li x14, 0x810cd7e000 \n"                 
			"fsw.ps f24, (x14) \n"                 
			"li x11, 0x810f068000 \n"                 
			"fsw.ps f7, (x11) \n"                 
			"li x21, 0x810fd40000 \n"                 
			"fsw.ps f20, (x21) \n"                 
			"li x19, 0x82005205e0 \n"                 
			"fsw.ps f16, (x19) \n"                 
			"li x20, 0x8200e80540 \n"                 
			"fsw.ps f22, (x20) \n"                 
			"li x23, 0x82013e0020 \n"                 
			"fsw.ps f7, (x23) \n"                 
			"li x11, 0x8201da06c0 \n"                 
			"fsw.ps f21, (x11) \n"                 
			"li x17, 0x82025ac0a0 \n"                 
			"fsw.ps f29, (x17) \n"                 
			"li x11, 0x82028a0720 \n"                 
			"fsw.ps f27, (x11) \n"                 
			"li x21, 0x820292e020 \n"                 
			"fsw.ps f25, (x21) \n"                 
			"li x23, 0x82030ccf00 \n"                 
			"fsw.ps f4, (x23) \n"                 
			"li x10, 0x8207407000 \n"                 
			"fsw.ps f27, (x10) \n"                 
			"li x12, 0x8208acb5e0 \n"                 
			"fsw.ps f7, (x12) \n"                 
			"li x10, 0x820a920000 \n"                 
			"fsw.ps f29, (x10) \n"                 
			"li x12, 0x820ca20000 \n"                 
			"fsw.ps f24, (x12) \n"                 
			"li x17, 0x820db69000 \n"                 
			"fsw.ps f16, (x17) \n"                 
			"li x22, 0x820e240160 \n"                 
			"fsw.ps f10, (x22) \n"                 
			"fence\n"
			"LBL_GLOBAL_VADDR_INIT_HID_0:\n"          
			"li x25, 0x81258a0020 \n"                 
			"fswg.ps f7, (x25) \n"                 
			"li x19, 0x8126199160 \n"                 
			"fswg.ps f16, (x19) \n"                 
			"li x17, 0x8126fa0000 \n"                 
			"fswg.ps f3, (x17) \n"                 
			"li x18, 0x812b078020 \n"                 
			"fswg.ps f15, (x18) \n"                 
			"li x19, 0x812bd80080 \n"                 
			"fswg.ps f3, (x19) \n"                 
			"li x13, 0x812cd7e000 \n"                 
			"fswg.ps f25, (x13) \n"                 
			"li x14, 0x812f068000 \n"                 
			"fswg.ps f20, (x14) \n"                 
			"li x24, 0x812f2ab660 \n"                 
			"fswg.ps f18, (x24) \n"                 
			"li x22, 0x812f799a40 \n"                 
			"fswg.ps f29, (x22) \n"                 
			"li x14, 0x813337aec0 \n"                 
			"fswg.ps f25, (x14) \n"                 
			"li x23, 0x8134e400a0 \n"                 
			"fswg.ps f8, (x23) \n"                 
			"li x23, 0x8136170080 \n"                 
			"fswg.ps f16, (x23) \n"                 
			"li x16, 0x8138040260 \n"                 
			"fswg.ps f16, (x16) \n"                 
			"li x12, 0x8138c80660 \n"                 
			"fswg.ps f5, (x12) \n"                 
			"li x17, 0x813af52000 \n"                 
			"fswg.ps f1, (x17) \n"                 
			"li x13, 0x8221da06c0 \n"                 
			"fswg.ps f27, (x13) \n"                 
			"li x19, 0x82223b2820 \n"                 
			"fswg.ps f24, (x19) \n"                 
			"li x10, 0x82228a0720 \n"                 
			"fswg.ps f9, (x10) \n"                 
			"li x24, 0x8227407000 \n"                 
			"fswg.ps f6, (x24) \n"                 
			"li x17, 0x822ca20000 \n"                 
			"fswg.ps f26, (x17) \n"                 
			"li x14, 0x822db69000 \n"                 
			"fswg.ps f9, (x14) \n"                 
			"li x13, 0x82301a0000 \n"                 
			"fswg.ps f28, (x13) \n"                 
			"li x12, 0x82305205e0 \n"                 
			"fswg.ps f25, (x12) \n"                 
			"li x16, 0x8230e80540 \n"                 
			"fswg.ps f13, (x16) \n"                 
			"li x23, 0x82325ac0a0 \n"                 
			"fswg.ps f23, (x23) \n"                 
			"li x19, 0x823292e020 \n"                 
			"fswg.ps f11, (x19) \n"                 
			"li x10, 0x8233115820 \n"                 
			"fswg.ps f8, (x10) \n"                 
			"fence\n"
			"LBL_LOCAL_VADDR_INIT_HID_0:\n"          
			"li x16, 0x81401c0000 \n"                 
			"fswl.ps f14, (x16) \n"                 
			"li x18, 0x8146199160 \n"                 
			"fswl.ps f18, (x18) \n"                 
			"li x22, 0x8146fa0000 \n"                 
			"fswl.ps f24, (x22) \n"                 
			"li x13, 0x8148c80660 \n"                 
			"fswl.ps f17, (x13) \n"                 
			"li x11, 0x8148d1b020 \n"                 
			"fswl.ps f29, (x11) \n"                 
			"li x23, 0x814b078020 \n"                 
			"fswl.ps f15, (x23) \n"                 
			"li x14, 0x814c660000 \n"                 
			"fswl.ps f1, (x14) \n"                 
			"li x18, 0x814cd7e000 \n"                 
			"fswl.ps f16, (x18) \n"                 
			"li x25, 0x814f068000 \n"                 
			"fswl.ps f8, (x25) \n"                 
			"li x24, 0x8153760100 \n"                 
			"fswl.ps f9, (x24) \n"                 
			"li x19, 0x81558a0020 \n"                 
			"fswl.ps f13, (x19) \n"                 
			"li x24, 0x8156170080 \n"                 
			"fswl.ps f28, (x24) \n"                 
			"li x20, 0x815af52000 \n"                 
			"fswl.ps f12, (x20) \n"                 
			"li x20, 0x815bd80080 \n"                 
			"fswl.ps f24, (x20) \n"                 
			"li x19, 0x815f2ab660 \n"                 
			"fswl.ps f1, (x19) \n"                 
			"li x13, 0x815f799a40 \n"                 
			"fswl.ps f5, (x13) \n"                 
			"li x21, 0x8241da06c0 \n"                 
			"fswl.ps f25, (x21) \n"                 
			"li x22, 0x82423b2820 \n"                 
			"fswl.ps f20, (x22) \n"                 
			"li x19, 0x82428a0720 \n"                 
			"fswl.ps f23, (x19) \n"                 
			"li x24, 0x824292e020 \n"                 
			"fswl.ps f14, (x24) \n"                 
			"li x22, 0x8247407000 \n"                 
			"fswl.ps f24, (x22) \n"                 
			"li x13, 0x824af2c120 \n"                 
			"fswl.ps f23, (x13) \n"                 
			"li x14, 0x82525ac0a0 \n"                 
			"fswl.ps f6, (x14) \n"                 
			"li x22, 0x8258acb5e0 \n"                 
			"fswl.ps f15, (x22) \n"                 
			"li x11, 0x825a920000 \n"                 
			"fswl.ps f2, (x11) \n"                 
			"li x25, 0x825ca20000 \n"                 
			"fswl.ps f12, (x25) \n"                 
			"li x25, 0x825e240160 \n"                 
			"fswl.ps f3, (x25) \n"                 
			"fence\n"
			"LBL_LOCAL_L2SCP_INIT_HID_0:\n"          
			"la x11, rand_ieee754_32\n"    
			"li x31, 0x40\n"                 
			"li x5, 0x2f \n"                 
			"add x5, x11, x5\n"               
			"csrw tensor_load_l2, x5\n"         
			"addi x31, x31, 1\n"                 
			"li x5, 0x200000000003f \n"                 
			"add x5, x11, x5\n"               
			"csrw tensor_load_l2, x5\n"         
			"addi x31, x31, -1\n"                 
			"li x5, 0x400000000002f \n"                 
			"add x5, x11, x5\n"               
			"csrw tensor_load_l2, x5\n"         
			"addi x31, x31, 1\n"                 
			"li x5, 0x600000000003f \n"                 
			"add x5, x11, x5\n"               
			"csrw tensor_load_l2, x5\n"         
			"addi x31, x31, -1\n"                 
			"li x5, 0x800000000002f \n"                 
			"add x5, x11, x5\n"               
			"csrw tensor_load_l2, x5\n"         
			"addi x31, x31, 1\n"                 
			"li x5, 0xa00000000003f \n"                 
			"add x5, x11, x5\n"               
			"csrw tensor_load_l2, x5\n"         
			"addi x31, x31, -1\n"                 
			"li x5, 0xc00000000002f \n"                 
			"add x5, x11, x5\n"               
			"csrw tensor_load_l2, x5\n"         
			"addi x31, x31, 1\n"                 
			"li x5, 0xe00000000003f \n"                 
			"add x5, x11, x5\n"               
			"csrw tensor_load_l2, x5\n"         
			"addi x31, x31, -1\n"                 
			"li x5, 0x2300000000002f \n"                 
			"add x5, x11, x5\n"               
			"csrw tensor_load_l2, x5\n"         
			"addi x31, x31, 1\n"                 
			"li x5, 0x2700000000002f \n"                 
			"add x5, x11, x5\n"               
			"csrw tensor_load_l2, x5\n"         
			"addi x31, x31, -1\n"                 
			"li x5, 0x2b00000000002f \n"                 
			"add x5, x11, x5\n"               
			"csrw tensor_load_l2, x5\n"         
			"addi x31, x31, 1\n"                 
			"li x5, 0x2f00000000002f \n"                 
			"add x5, x11, x5\n"               
			"csrw tensor_load_l2, x5\n"         
			"addi x31, x31, -1\n"                 
			"li x5, 0x4500000000000f \n"                 
			"add x5, x11, x5\n"               
			"csrw tensor_load_l2, x5\n"         
			"addi x31, x31, 1\n"                 
			"li x5, 0x4900000000000f \n"                 
			"add x5, x11, x5\n"               
			"csrw tensor_load_l2, x5\n"         
			"addi x31, x31, -1\n"                 
			"li x5, 0x4d00000000000f \n"                 
			"add x5, x11, x5\n"               
			"csrw tensor_load_l2, x5\n"         
			"addi x31, x31, 1\n"                 
			"li x5, 0x5100000000000f \n"                 
			"add x5, x11, x5\n"               
			"csrw tensor_load_l2, x5\n"         
			"addi x31, x31, -1\n"                 
			"csrwi tensor_wait, 0x2\n"      
			"csrwi tensor_wait, 0x3\n"      
			"fence\n"
			"LBL_TXR_INIT_HID_0:\n"          
			"li x17, 0x816af52000 \n"                 
			"fsw.ps f2, 0(x17) \n"                 
			"fsw.ps f15, 64(x17) \n"                 
			"fsw.ps f31, 128(x17) \n"                 
			"fsw.ps f24, 192(x17) \n"                 
			"fsw.ps f18, 256(x17) \n"                 
			"fsw.ps f9, 320(x17) \n"                 
			"fsw.ps f24, 384(x17) \n"                 
			"li x16, 0x8186170080 \n"                 
			"fsw.ps f23, 0(x16) \n"                 
			"fsw.ps f3, 64(x16) \n"                 
			"fsw.ps f28, 128(x16) \n"                 
			"fsw.ps f20, 192(x16) \n"                 
			"fsw.ps f8, 256(x16) \n"                 
			"fsw.ps f26, 320(x16) \n"                 
			"fsw.ps f22, 384(x16) \n"                 
			"fsw.ps f16, 448(x16) \n"                 
			"fsw.ps f1, 512(x16) \n"                 
			"fsw.ps f8, 576(x16) \n"                 
			"li x22, 0x8186199140 \n"                 
			"fsw.ps f29, 0(x22) \n"                 
			"fsw.ps f12, 64(x22) \n"                 
			"fsw.ps f2, 128(x22) \n"                 
			"fsw.ps f23, 192(x22) \n"                 
			"fsw.ps f1, 256(x22) \n"                 
			"fsw.ps f2, 320(x22) \n"                 
			"fsw.ps f29, 384(x22) \n"                 
			"fsw.ps f30, 448(x22) \n"                 
			"fsw.ps f23, 512(x22) \n"                 
			"fsw.ps f8, 576(x22) \n"                 
			"fsw.ps f16, 640(x22) \n"                 
			"li x25, 0x818af52000 \n"                 
			"fsw.ps f12, 0(x25) \n"                 
			"fsw.ps f6, 64(x25) \n"                 
			"fsw.ps f6, 128(x25) \n"                 
			"fsw.ps f6, 192(x25) \n"                 
			"fsw.ps f3, 256(x25) \n"                 
			"fsw.ps f17, 320(x25) \n"                 
			"fsw.ps f28, 384(x25) \n"                 
			"fsw.ps f23, 448(x25) \n"                 
			"fsw.ps f27, 512(x25) \n"                 
			"fsw.ps f8, 576(x25) \n"                 
			"fsw.ps f25, 640(x25) \n"                 
			"fsw.ps f21, 704(x25) \n"                 
			"li x21, 0x819337aec0 \n"                 
			"fsw.ps f14, 0(x21) \n"                 
			"fsw.ps f11, 64(x21) \n"                 
			"fsw.ps f15, 128(x21) \n"                 
			"fsw.ps f16, 192(x21) \n"                 
			"fsw.ps f20, 256(x21) \n"                 
			"fsw.ps f30, 320(x21) \n"                 
			"fsw.ps f26, 384(x21) \n"                 
			"fsw.ps f14, 448(x21) \n"                 
			"fsw.ps f11, 512(x21) \n"                 
			"fsw.ps f23, 576(x21) \n"                 
			"fsw.ps f13, 640(x21) \n"                 
			"fsw.ps f30, 704(x21) \n"                 
			"fsw.ps f27, 768(x21) \n"                 
			"fsw.ps f7, 832(x21) \n"                 
			"fsw.ps f29, 896(x21) \n"                 
			"fsw.ps f18, 960(x21) \n"                 
			"li x12, 0x819f799a40 \n"                 
			"fsw.ps f3, 0(x12) \n"                 
			"fsw.ps f12, 64(x12) \n"                 
			"fsw.ps f21, 128(x12) \n"                 
			"fsw.ps f1, 192(x12) \n"                 
			"fsw.ps f2, 256(x12) \n"                 
			"fsw.ps f29, 320(x12) \n"                 
			"fsw.ps f31, 384(x12) \n"                 
			"fsw.ps f24, 448(x12) \n"                 
			"fsw.ps f6, 512(x12) \n"                 
			"fsw.ps f5, 576(x12) \n"                 
			"fsw.ps f17, 640(x12) \n"                 
			"fsw.ps f29, 704(x12) \n"                 
			"li x15, 0x826af2c100 \n"                 
			"fsw.ps f21, 0(x15) \n"                 
			"fsw.ps f29, 64(x15) \n"                 
			"fsw.ps f13, 128(x15) \n"                 
			"fsw.ps f29, 192(x15) \n"                 
			"fsw.ps f28, 256(x15) \n"                 
			"fsw.ps f2, 320(x15) \n"                 
			"fsw.ps f25, 384(x15) \n"                 
			"fsw.ps f31, 448(x15) \n"                 
			"fsw.ps f0, 512(x15) \n"                 
			"fsw.ps f25, 576(x15) \n"                 
			"fsw.ps f25, 640(x15) \n"                 
			"fsw.ps f5, 704(x15) \n"                 
			"fsw.ps f13, 768(x15) \n"                 
			"fsw.ps f19, 832(x15) \n"                 
			"li x10, 0x827292e000 \n"                 
			"fsw.ps f26, 0(x10) \n"                 
			"fsw.ps f21, 64(x10) \n"                 
			"fsw.ps f10, 128(x10) \n"                 
			"fsw.ps f22, 192(x10) \n"                 
			"fsw.ps f9, 256(x10) \n"                 
			"fsw.ps f8, 320(x10) \n"                 
			"li x10, 0x8278acb5c0 \n"                 
			"fsw.ps f25, 0(x10) \n"                 
			"fsw.ps f12, 64(x10) \n"                 
			"fsw.ps f17, 128(x10) \n"                 
			"fsw.ps f16, 192(x10) \n"                 
			"fsw.ps f17, 256(x10) \n"                 
			"fsw.ps f6, 320(x10) \n"                 
			"fsw.ps f16, 384(x10) \n"                 
			"fsw.ps f28, 448(x10) \n"                 
			"fsw.ps f27, 512(x10) \n"                 
			"fsw.ps f29, 576(x10) \n"                 
			"fsw.ps f0, 640(x10) \n"                 
			"fsw.ps f26, 704(x10) \n"                 
			"fsw.ps f15, 768(x10) \n"                 
			"fsw.ps f22, 832(x10) \n"                 
			"fsw.ps f19, 896(x10) \n"                 
			"li x12, 0x827db69000 \n"                 
			"fsw.ps f29, 0(x12) \n"                 
			"fsw.ps f16, 64(x12) \n"                 
			"fsw.ps f20, 128(x12) \n"                 
			"fsw.ps f26, 192(x12) \n"                 
			"fsw.ps f16, 256(x12) \n"                 
			"li x17, 0x8288acb5c0 \n"                 
			"fsw.ps f19, 0(x17) \n"                 
			"fsw.ps f8, 64(x17) \n"                 
			"fsw.ps f1, 128(x17) \n"                 
			"fsw.ps f1, 192(x17) \n"                 
			"fsw.ps f5, 256(x17) \n"                 
			"li x12, 0x829db69000 \n"                 
			"fsw.ps f22, 0(x12) \n"                 
			"fsw.ps f6, 64(x12) \n"                 
			"fsw.ps f29, 128(x12) \n"                 
			"fsw.ps f23, 192(x12) \n"                 
			"fsw.ps f4, 256(x12) \n"                 
			"fsw.ps f17, 320(x12) \n"                 
			"fsw.ps f25, 384(x12) \n"                 
			"fsw.ps f6, 448(x12) \n"                 
			"fsw.ps f24, 512(x12) \n"                 
			"fsw.ps f18, 576(x12) \n"                 
			"fsw.ps f27, 640(x12) \n"                 
			"fsw.ps f23, 704(x12) \n"                 
			"fsw.ps f18, 768(x12) \n"                 
			"fsw.ps f27, 832(x12) \n"                 
			"fsw.ps f9, 896(x12) \n"                 
			"fence\n"
			"fence\n"
			"csrw fflags, zero\n"
			"csrwi frm, 0x0\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_mask, zero\n"
			"LBL_KERNEL_HID_0:\n"          
			"li x5, 0xFEEDFEED\n"
			"li x0, 0x5633de2be7\n" 
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
			"la x12, rand_int_32\n"
			"flw.ps f12, 288(x12)\n"
			"flw.ps f21, 416(x12)\n"
			"fand.pi f20, f12, f21\n" 
			"flem.ps m6, f6, f12\n"
			"fle.pi f7, f21, f3\n"
			"sll x0, x6, x26\n"
			"srli x6, x12, 5\n"
			"xor x12, x26, x9\n"
			"or x12, x12, x26\n"
			"bltu x28, x7, 1f\n"
			"1:\n"
			"fadd.pi f6, f21, f14\n"
			"la x12, rand_int_32\n"
			"flw.ps f3, 640(x12)\n"
			"fnot.pi f31, f3\n" 
			"frcp.ps f20, f6\n"
			"sraw x0, x23, x26\n"
			"blt x8, x19, 1f\n"
			"1:\n"
			"sllw x17, x11, x23\n"
			"fle.pi f20, f3, f6\n"
			"add x0, x20, x26\n"
			"beq x22, x16, 1f\n"
			"1:\n"
			"packb x12, x12, x11\n"
			"bltu x31, x24, 1f\n"
			"1:\n"
			"fmul.pi f20, f1, f26\n"
			"la x26, rand_int_32\n"
			"flw.ps f1, 928(x26)\n"
			"flw.ps f31, 224(x26)\n"
			"fsrl.pi f26, f1, f31\n" 
			"fmax.pi f26, f23, f27\n"
			"srli x11, x26, 0\n"
			"blt x3, x25, 1f\n"
			"1:\n"
			"divuw x9, x28, x7\n"
			"sltiu x11, x17, 0x18\n"
			"fmulhu.pi f20, f7, f23\n"
			"blt x7, x8, 1f\n"
			"1:\n"
			"mulhsu x26, x17, x17\n"
			"fmvz.x.ps x7, f27, 0\n"
			"li x12, 0x516cfda690159cd1\n"
			"li x26, 0x58c031690910638c\n"
			"srlw x20, x12, x26\n" 
			"fmax.pi f1, f1, f1\n"
			"fsin.ps f27, f12\n"
			"fmadd.s f30, f31, f31, f1, rdn\n"
			"fmadd.ps f27, f1, f27, f31, rdn\n"
			"fpackreph.pi f30, f24\n"
			"fcvt.pwu.ps f1, f24, rmm\n"
			"fnot.pi f30, f5\n"
			"auipc x20, 0x10\n" 
			"srlw x11, x7, x26\n"
			"fnmadd.ps f12, f5, f12, f20, rmm\n"
			"ecall\n"
			"fltu.pi f20, f7, f3\n"
			"fmvz.x.ps x11, f5, 1\n"
			"fsub.s f26, f26, f20, rtz\n"
			"bltu x23, x24, 1f\n"
			"1:\n"
			"add x6, x26, x17\n"
			"la x6, rand_int_32\n"
			"flw.ps f6, 640(x6)\n"
			"flw.ps f30, 544(x6)\n"
			"fltu.pi f26, f6, f30\n" 
			"xor x11, x9, x9\n"
			"bltu x17, x1, 1f\n"
			"1:\n"
			"add x6, x6, x7\n"
			"divuw x7, x0, x7\n"
			"add x20, x12, x6\n"
			"bgeu x21, x25, 1f\n"
			"1:\n"
			"slliw x20, x7, 1\n"
			"blt x16, x6, 1f\n"
			"1:\n"
			"fbci.ps f26, 0x64882\n"
			"fsll.pi f5, f30, f23\n"
			"bltu x14, x5, 1f\n"
			"1:\n"
			"li x20, 0x5e776cdc997df074\n"
			"slliw x28, x20, 4\n" 
			"mulh x28, x6, x17\n"
			"mulhsu x17, x23, x26\n"
			"fadd.pi f3, f27, f6\n"
			"bge x11, x18, 1f\n"
			"1:\n"
			"fsub.pi f1, f31, f3\n"
			"frcp.ps f6, f30\n"
			"fnmsub.ps f17, f23, f31, f23, rmm\n"
			"slt x26, x26, x11\n"
			"la x11, rand_ieee754_32\n"
			"flw.ps f31, 480(x11)\n"
			"flw.ps f26, 128(x11)\n"
			"flw.ps f5, 480(x11)\n"
			"fmadd.s f26, f31, f26, f5, rne\n" 
			"fmul.s f6, f6, f1, rne\n"
			"fmin.ps f1, f24, f1\n"
			"fsll.pi f21, f24, f21\n"
			"fmul.s f31, f21, f21, rtz\n"
			"fmaxu.pi f31, f6, f24\n"
			"fslli.pi f1, f24, 0x5\n"
			"fnot.pi f30, f6\n"
			"li x7, 0x68187e5db822b8d8\n"
			"sllw x12, x7, x7\n" 
			"fsub.s f31, f17, f31, rup\n"
			"xor x9, x17, x7\n"
			"fmin.ps f14, f14, f31\n"
			"fsgnjn.ps f31, f27, f31\n"
			"fsat8.pi f20, f3\n"
			"bge x29, x30, 1f\n"
			"1:\n"
			"auipc x7, 0x1c\n"
			"blt x3, x14, 1f\n"
			"1:\n"
			"auipc x26, 0x1c\n"
			"lui x17, 0x16\n" 
			"srl x20, x0, x20\n"
			"lui x12, 0x5\n"
			"fmadd.ps f26, f26, f20, f14, rtz\n"
			"fltu.pi f30, f26, f31\n"
			"maskpopcz x0, m4\n"
			"sllw x17, x28, x17\n"
			"xor x9, x28, x26\n"
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_0:\n"
			"fltm.pi m4, f12, f30\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"fnmsub.ps f17, f23, f20, f14, rne\n"
			"fmulhu.pi f30, f20, f23\n"
			"bge x14, x29, 1f\n"
			"1:\n"
			"fadd.pi f21, f21, f20\n"
			"srlw x23, x20, x7\n"
			"blt x3, x30, 1f\n"
			"1:\n"
			"fmvz.x.ps x7, f17, 1\n"
			"fmvz.x.ps x7, f27, 0\n"
			"maskpopcz x0, m2\n"
			"la x11, rand_ieee754_32\n"
			"flw.ps f21, 480(x11)\n"
			"flw.ps f24, 928(x11)\n"
			"flw.ps f30, 416(x11)\n"
			"fmadd.ps f24, f21, f24, f30, rne\n" 
			"fslli.pi f7, f7, 0x2\n"
			"sra x0, x6, x0\n"
			"fsrl.pi f20, f30, f20\n"
			"remu x17, x11, x11\n"
			"div x9, x9, x20\n"
			"fmul.ps f26, f3, f23, rne\n"
			"mulhsu x11, x28, x26\n"
			"la x12, rand_ieee754_32\n"
			"flw.ps f7, 448(x12)\n"
			"flw.ps f6, 192(x12)\n"
			"fmax.ps f21, f7, f6\n" 
			"divuw x28, x28, x11\n"
			"fnmsub.ps f21, f21, f21, f21, rup\n"
			"flt.ps f17, f23, f23\n"
			"addw x17, x17, x11\n"
			"fmulhu.pi f24, f21, f12\n"
			"xor x17, x17, x12\n"
			"bne x21, x17, 1f\n"
			"1:\n"
			"fsub.pi f21, f23, f12\n"
			"blt x17, x12, 1f\n"
			"1:\n"
			"li x20, 0x972b3fc8f9ba0742\n"
			"li x28, 0x1f928f2822ac55e4\n"
			"sra x28, x20, x28\n" 
			"fcvt.pwu.ps f5, f17, rdn\n"
			"feq.pi f5, f1, f21\n"
			"fnmadd.ps f31, f20, f12, f17, rmm\n"
			"fmin.pi f31, f30, f1\n"
			"fand.pi f30, f30, f17\n"
			"fsll.pi f5, f12, f17\n"
			"fmax.pi f21, f30, f20\n"
			"la x28, rand_ieee754_32\n"
			"flw.ps f17, 320(x28)\n"
			"fclass.s x20, f17\n" 
			"ecall\n"
			"bgeu x19, x26, 1f\n"
			"1:\n"
			"fcvt.pwu.ps f20, f31, rdn\n"
			"fmadd.s f20, f5, f20, f23, rdn\n"
			"bltu x20, x17, 1f\n"
			"1:\n"
			"sub x26, x26, x6\n"
			"remuw x26, x26, x26\n"
			"srl x26, x12, x6\n"
			"srlw x6, x6, x17\n"
			"la x28, rand_int_32\n"
			"flw.ps f1, 352(x28)\n"
			"fsub.pi f12, f1, f1\n" 
			"sraw x6, x23, x0\n"
			"bgeu x24, x3, 1f\n"
			"1:\n"
			"auipc x9, 0x17\n"
			"auipc x6, 0x5\n"
			"beq x14, x26, 1f\n"
			"1:\n"
			"fclass.s x0, f12\n"
			"feq.pi f30, f6, f24\n"
			"mova.m.x x20\n"
			"fmul.s f27, f6, f14, rtz\n"
			"li x17, 0xdfde73ee406be244\n"
			"li x6, 0x7cc20d4e7b72e7d4\n"
			"or x12, x17, x6\n" 
			"remuw x6, x9, x11\n"
			"fmax.pi f3, f20, f17\n"
			"fmin.ps f20, f27, f27\n"
			"fmaxu.pi f31, f12, f30\n"
			"fsll.pi f14, f26, f20\n"
			"fmadd.ps f17, f17, f27, f12, rtz\n"
			"blt x4, x3, 1f\n"
			"1:\n"
			"fmin.pi f17, f20, f27\n"
			"la x26, rand_int_32\n"
			"flw.ps f14, 160(x26)\n"
			"flw.ps f31, 32(x26)\n"
			"fltu.pi f5, f14, f31\n" 
			"sub x23, x20, x23\n"
			"mulh x17, x11, x7\n"
			"fnmadd.ps f14, f14, f12, f6, rne\n"
			"blt x31, x13, 1f\n"
			"1:\n"
			"divuw x11, x6, x12\n"
			"fmadd.ps f5, f12, f17, f5, rne\n"
			"bge x15, x30, 1f\n"
			"1:\n"
			"fnmsub.ps f30, f14, f6, f12, rdn\n"
			"bgeu x10, x15, 1f\n"
			"1:\n"
			"fmax.pi f30, f5, f17\n"
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_0:\n"
			"fltm.pi m7, f3, f20\n" 
			"li x5, 0x803400C8\n"
			"ld x5, 0(x5)\n"
			"flt.ps f21, f27, f23\n"
			"fmadd.ps f26, f6, f3, f14, rmm\n"
			"divuw x20, x7, x28\n"
			"bltu x21, x2, 1f\n"
			"1:\n"
			"fcvt.f16.ps f26, f5\n"
			"slli x26, x26, 0\n"
			"fnmadd.ps f5, f6, f7, f30, rne\n"
			"sltiu x9, x0, 0x10\n"
			"li x28, 0xf882d4d03ce409a3\n"
			"li x11, 0x5d7da3e2dee25039\n"
			"addw x12, x28, x11\n" 
			"packb x9, x0, x11\n"
			"sllw x12, x7, x20\n"
			"fcvt.pwu.ps f27, f23, rmm\n"
			"fmax.ps f17, f7, f3\n"
			"bge x0, x5, 1f\n"
			"1:\n"
			"divuw x20, x20, x7\n"
			"sra x23, x6, x12\n"
			"bgeu x16, x11, 1f\n"
			"1:\n"
			"divuw x7, x26, x11\n"
			"la x26, rand_ieee754_32\n"
			"flw.ps f7, 96(x26)\n"
			"flw.ps f3, 0(x26)\n"
			"fmin.ps f27, f7, f3\n" 
			"fnmsub.ps f7, f26, f26, f21, rtz\n"
			"bgeu x4, x24, 1f\n"
			"1:\n"
			"fmin.pi f21, f21, f24\n"
			"fpackreph.pi f1, f14\n"
			"fle.ps f5, f6, f21\n"
			"fslli.pi f3, f21, 0x7\n"
			"fmul.s f24, f21, f7, rmm\n"
			"fmadd.s f24, f3, f24, f7, rtz\n"
			"la x28, rand_int_32\n"
			"flw.ps f27, 768(x28)\n"
			"fandi.pi f6, f27, 0x1b7\n" 
			"fltu.pi f24, f26, f3\n"
			"fle.pi f17, f31, f5\n"
			"fmax.ps f27, f30, f5\n"
			"mulw x11, x11, x12\n"
			"fmax.ps f21, f23, f6\n"
			"addw x17, x17, x26\n"
			"mulhsu x17, x17, x12\n"
			"la x28, rand_int_32\n"
			"flw.ps f24, 320(x28)\n"
			"flw.ps f6, 608(x28)\n"
			"fsrl.pi f1, f24, f6\n" 
			"fmin.ps f21, f31, f21\n"
			"fround.ps f31, f31, rdn\n"
			"fmvz.x.ps x6, f21, 1\n"
			"fsin.ps f17, f17\n"
			"fandi.pi f31, f12, 0x95\n"
			"frcp.ps f5, f30\n"
			"sub x6, x20, x7\n"
			"la x23, rand_int_32\n"
			"flw.ps f31, 576(x23)\n"
			"flw.ps f1, 256(x23)\n"
			"fsub.pi f3, f31, f1\n" 
			"sra x12, x23, x26\n"
			"slt x26, x26, x17\n"
			"fmadd.ps f21, f5, f21, f21, rne\n"
			"add x23, x26, x26\n"
			"bltu x22, x10, 1f\n"
			"1:\n"
			"fadd.pi f14, f5, f27\n"
			"maskpopcz x20, m6\n"
			"sll x20, x17, x17\n"
			"la x23, rand_ieee754_32\n"
			"flw.ps f1, 768(x23)\n"
			"flw.ps f31, 160(x23)\n"
			"fmadd.s f24, f1, f31, f31, rdn\n" 
			"fltm.pi m6, f20, f23\n"
			"or x28, x17, x12\n"
			"fle.pi f21, f23, f1\n"
			"fsat8.pi f21, f20\n"
			"bltu x0, x25, 1f\n"
			"1:\n"
			"fmin.ps f21, f30, f17\n"
			"fcvt.wu.s x6, f1, rup\n"
			"bne x15, x29, 1f\n"
			"1:\n"
			"fmax.pi f30, f23, f20\n"
			"bgeu x5, x1, 1f\n"
			"1:\n"
			"la x9, rand_int_32\n"
			"flw.ps f3, 192(x9)\n"
			"flw.ps f21, 704(x9)\n"
			"fsrl.pi f12, f3, f21\n" 
			"fmin.pi f17, f6, f23\n"
			"fmax.pi f23, f17, f27\n"
			"frcp.ps f21, f5\n"
			"frcp.ps f20, f12\n"
			"fsub.s f12, f23, f3, rmm\n"
			"fadd.pi f30, f6, f7\n"
			"fsin.ps f1, f31\n"
			"LBL_SEQID_0_FP_TRANS_ID_2_HID_0:\n"
			"la x12, rand_ieee754_32\n"
			"flw.ps f23, 0(x12)\n"
			"frcp.ps f23, f23\n" 
			"sll x9, x9, x23\n"
			"bltu x4, x0, 1f\n"
			"1:\n"
			"fmul.s f7, f26, f20, rtz\n"
			"srlw x12, x23, x9\n"
			"fmaxu.pi f5, f30, f21\n"
			"bltu x31, x29, 1f\n"
			"1:\n"
			"fpackreph.pi f7, f23\n"
			"fsat8.pi f20, f23\n"
			"sll x12, x12, x28\n"
			"li x23, 0x560231496d5b318b\n"
			"slli x20, x23, 2\n" 
			"fltu.pi f31, f27, f21\n"
			"fsin.ps f26, f21\n"
			"beq x17, x9, 1f\n"
			"1:\n"
			"maskpopcz x12, m7\n"
			"fnmadd.ps f21, f21, f26, f30, rtz\n"
			"fandi.pi f20, f31, 0x30\n"
			"srl x9, x9, x20\n"
			"fsat8.pi f20, f14\n"
			"li x20, 0xba7f90425c28846f\n"
			"li x17, 0xab56ee5951b83dbc\n"
			"sra x6, x20, x17\n" 
			"fnmsub.ps f30, f26, f30, f21, rmm\n"
			"packb x20, x6, x20\n"
			"fsat8.pi f23, f24\n"
			"or x26, x7, x26\n"
			"fcvt.pw.ps f1, f14, rtz\n"
			"frcp.ps f17, f17\n"
			"remu x7, x28, x7\n"
			"la x26, rand_ieee754_32\n"
			"flw.ps f7, 640(x26)\n"
			"flw.ps f21, 736(x26)\n"
			"flw.ps f30, 64(x26)\n"
			"fnmsub.ps f31, f7, f21, f30, rne\n" 
			"divuw x6, x12, x23\n"
			"bltu x16, x20, 1f\n"
			"1:\n"
			"auipc x7, 0x1a\n"
			"fmadd.ps f30, f14, f27, f24, rtz\n"
			"fmaxu.pi f6, f6, f30\n"
			"bge x10, x23, 1f\n"
			"1:\n"
			"mulw x20, x20, x20\n"
			"fsgnjn.ps f21, f24, f21\n"
			"fmadd.ps f23, f21, f27, f3, rtz\n"
			"la x28, rand_ieee754_32\n"
			"flw.ps f17, 832(x28)\n"
			"flw.ps f14, 320(x28)\n"
			"fmax.ps f26, f17, f14\n" 
			"add x23, x6, x9\n"
			"mova.m.x x11\n"
			"fmadd.ps f31, f12, f14, f31, rmm\n"
			"auipc x26, 0xf\n"
			"feq.pi f3, f12, f31\n"
			"blt x22, x28, 1f\n"
			"1:\n"
			"sll x26, x6, x26\n"
			"bgeu x17, x3, 1f\n"
			"1:\n"
			"mova.m.x x20\n"
			"la x9, rand_int_32\n"
			"flw.ps f21, 928(x9)\n"
			"flw.ps f5, 800(x9)\n"
			"fle.pi f5, f21, f5\n" 
			"fmul.s f1, f5, f1, rup\n"
			"fsll.pi f23, f23, f5\n"
			"beq x18, x17, 1f\n"
			"1:\n"
			"fmul.s f6, f6, f5, rdn\n"
			"fmax.ps f5, f5, f6\n"
			"fmin.pi f1, f6, f1\n"
			"fmulhu.pi f3, f6, f1\n"
			"fmin.pi f1, f5, f23\n"
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
			"fmax.ps f27, f27, f6\n"
			"fmulhu.pi f21, f21, f7\n"
			"fmax.ps f20, f24, f20\n"
			"fmadd.ps f5, f6, f24, f3, rdn\n"
			"sra x7, x23, x23\n"
			"remu x6, x17, x23\n"
			"fnot.pi f5, f30\n"
			"mulw x23, x7, x9\n"
			"blt x26, x20, 1f\n"
			"1:\n"
			"fpackreph.pi f3, f1\n"
			"fmax.pi f1, f20, f7\n"
			"bgeu x3, x3, 1f\n"
			"1:\n"
			"fpackreph.pi f27, f12\n"
			"fcvt.pw.ps f7, f24, rdn\n"
			"fround.ps f31, f20, rup\n"
			"fcvt.pw.ps f17, f20, rmm\n"
			"fcvt.pw.ps f7, f7, rtz\n"
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ITER_0_HID_0:\n"
			"li x7, ((0x1 << TENSOR_STORE_REG_STRIDE) | (0x17 << TENSOR_STORE_START_REG) | (0x3 << TENSOR_STORE_STORE_COLS) | (0x3 << TENSOR_STORE_AROWS) | (0x0 << TENSOR_STORE_COOP_STORE) | (0x0 << TENSOR_STORE_OPC_0) | (0x816af5201 << TENSOR_STORE_VADDR) | 0 )\n" 
			"li x31, (0xc0 << 4)\n"
			"csrw tensor_store, x7\n"	
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ITER_1_HID_0:\n"
			"li x7, ((0x3 << TENSOR_STORE_REG_STRIDE) | (0xf << TENSOR_STORE_START_REG) | (0x0 << TENSOR_STORE_STORE_COLS) | (0x3 << TENSOR_STORE_AROWS) | (0x0 << TENSOR_STORE_COOP_STORE) | (0x0 << TENSOR_STORE_OPC_0) | (0x8000009 << TENSOR_STORE_VADDR) | 0 )\n" 
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
			"csrrci x0, mcache_control, 2\n"
			"csrwi tensor_wait, 0x6\n" 
			"fence\n"
			"mov.m.x m0, x0, 0xFF\n"
			"li x5, 0x8\n"
			"la x31, rand_ieee754_32\n"
			"add x31, x31, x5\n"
			"flw.ps f1, 0(x31)\n"
			"flw.ps f12, 0(x31)\n"
			"flw.ps f14, 3(x31)\n"
			"flw.ps f17, 1(x31)\n"
			"flw.ps f20, 10(x31)\n"
			"flw.ps f21, 13(x31)\n"
			"flw.ps f23, 23(x31)\n"
			"flw.ps f24, 22(x31)\n"
			"flw.ps f26, 25(x31)\n"
			"flw.ps f27, 31(x31)\n"
			"flw.ps f3, 21(x31)\n"
			"flw.ps f30, 40(x31)\n"
			"flw.ps f31, 2(x31)\n"
			"flw.ps f5, 17(x31)\n"
			"flw.ps f6, 42(x31)\n"
			"flw.ps f7, 14(x31)\n"
			"li x21, 0x1\n"
			"LBL_SEQID_0_LOOP_SEQ_HID_0:\n"
			"fmul.pi f7, f12, f20\n"
			"fmax.ps f12, f27, f1\n"
			"fmax.ps f17, f20, f5\n"
			"fslli.pi f5, f26, 0x1\n"
			"fcvt.f16.ps f1, f5\n"
			"bltu x3, x23, 1f\n"
			"1:\n"
			"fle.ps f1, f31, f17\n"
			"fcvt.pw.ps f1, f31, rdn\n"
			"bltu x26, x10, 1f\n"
			"1:\n"
			"fle.pi f6, f14, f30\n"
			"fcvt.f16.ps f24, f30\n"
			"fle.pi f24, f6, f7\n"
			"fpackreph.pi f5, f6\n"
			"fmax.ps f27, f7, f31\n"
			"fmulhu.pi f12, f17, f3\n"
			"blt x7, x13, 1f\n"
			"1:\n"
			"fcvt.pw.ps f27, f6, rtz\n"
			"bltu x23, x27, 1f\n"
			"1:\n"
			"fround.ps f21, f5, rmm\n"
			"fltu.pi f30, f5, f24\n"
			"fsll.pi f17, f17, f30\n"
			"fslli.pi f12, f3, 0x8\n"
			"bne x15, x4, 1f\n"
			"1:\n"
			"fcvt.pwu.ps f21, f17, rmm\n"
			"blt x0, x9, 1f\n"
			"1:\n"
			"fpackreph.pi f5, f12\n"
			"flt.ps f26, f30, f14\n"
			"flt.ps f20, f30, f6\n"
			"bge x27, x2, 1f\n"
			"1:\n"
			"fsgnjn.ps f3, f1, f30\n"
			"flt.ps f27, f5, f14\n"
			"fltu.pi f17, f17, f20\n"
			"fcvt.pw.ps f12, f12, rtz\n"
			"fmax.pi f3, f17, f3\n"
			"fcvt.f16.ps f21, f26\n"
			"fand.pi f5, f3, f1\n"
			"fnot.pi f6, f20\n"
			"bne x2, x1, 1f\n"
			"1:\n"
			"fle.pi f23, f31, f6\n"
			"flt.ps f7, f3, f20\n"
			"fmin.pi f1, f12, f7\n"
			"fpackreph.pi f6, f21\n"
			"fsgnjn.ps f14, f6, f12\n"
			"fmulhu.pi f1, f26, f21\n"
			"fmulhu.pi f12, f23, f3\n"
			"blt x20, x4, 1f\n"
			"1:\n"
			"fmadd.ps f21, f21, f31, f27, rmm\n"
			"fround.ps f24, f23, rtz\n"
			"fslli.pi f20, f23, 0x5\n"
			"fpackreph.pi f6, f24\n"
			"bgeu x13, x9, 1f\n"
			"1:\n"
			"fsat8.pi f23, f27\n"
			"fle.pi f24, f12, f20\n"
			"fsgnjn.ps f27, f7, f24\n"
			"fle.ps f20, f17, f5\n"
			"fmin.ps f5, f14, f5\n"
			"flt.ps f12, f17, f14\n"
			"bgeu x5, x9, 1f\n"
			"1:\n"
			"fnot.pi f30, f1\n"
			"fsgnjn.ps f14, f6, f17\n"
			"fmin.ps f17, f26, f14\n"
			"fcvt.pwu.ps f30, f6, rne\n"
			"fle.ps f3, f12, f3\n"
			"fmin.ps f6, f3, f17\n"
			"bgeu x12, x9, 1f\n"
			"1:\n"
			"fsll.pi f21, f7, f1\n"
			"fmadd.ps f20, f20, f20, f7, rup\n"
			"fsll.pi f7, f7, f6\n"
			"fmul.pi f21, f3, f21\n"
			"fmin.pi f26, f31, f26\n"
			"bne x1, x6, 1f\n"
			"1:\n"
			"fle.ps f23, f23, f12\n"
			"fcvt.f16.ps f17, f5\n"
			"fmul.pi f21, f7, f12\n"
			"fpackreph.pi f21, f5\n"
			"fsgnjn.ps f12, f24, f12\n"
			"fnot.pi f12, f27\n"
			"fpackreph.pi f31, f12\n"
			"fle.ps f14, f12, f21\n"
			"fltu.pi f27, f27, f12\n"
			"bge x16, x13, 1f\n"
			"1:\n"
			"fpackreph.pi f21, f21\n"
			"fmin.ps f30, f5, f5\n"
			"fsat8.pi f20, f17\n"
			"flt.ps f14, f14, f31\n"
			"fmax.pi f24, f14, f12\n"
			"bltu x18, x27, 1f\n"
			"1:\n"
			"fcvt.pwu.ps f14, f14, rmm\n"
			"fsat8.pi f3, f1\n"
			"fsgnjn.ps f6, f6, f14\n"
			"bge x2, x3, 1f\n"
			"1:\n"
			"fsgnjn.ps f26, f5, f1\n"
			"fnot.pi f20, f7\n"
			"fmul.s f31, f20, f3, rne\n"
			"beq x0, x5, 1f\n"
			"1:\n"
			"fand.pi f14, f6, f20\n"
			"fand.pi f24, f21, f23\n"
			"fcvt.pw.ps f1, f5, rne\n"
			"fmadd.ps f24, f21, f30, f31, rne\n"
			"fpackreph.pi f31, f6\n"
			"fround.ps f17, f17, rup\n"
			"fsgnjn.ps f14, f24, f3\n"
			"fmulhu.pi f30, f17, f26\n"
			"fle.pi f21, f20, f23\n"
			"fmadd.ps f24, f6, f26, f7, rup\n"
			"blt x1, x16, 1f\n"
			"1:\n"
			"fmin.ps f7, f7, f3\n"
			"bne x14, x27, 1f\n"
			"1:\n"
			"fmin.pi f20, f3, f26\n"
			"fcvt.f16.ps f24, f21\n"
			"fmul.s f5, f5, f31, rne\n"
			"fmul.s f31, f5, f31, rmm\n"
			"fslli.pi f12, f6, 0xc\n"
			"fmax.pi f6, f21, f7\n"
			"fmax.ps f23, f5, f6\n"
			"fcvt.pw.ps f27, f27, rup\n"
			"fsll.pi f26, f1, f12\n"
			"fmin.pi f27, f17, f1\n"
			"fsgnjn.ps f30, f14, f3\n"
			"fmul.s f6, f26, f20, rdn\n"
			"fround.ps f23, f14, rup\n"
			"fand.pi f1, f24, f12\n"
			"fltu.pi f21, f30, f5\n"
			"fmin.pi f3, f27, f1\n"
			"fpackreph.pi f5, f3\n"
			"fmax.ps f21, f17, f14\n"
			"bgeu x2, x21, 1f\n"
			"1:\n"
			"fmul.pi f17, f23, f26\n"
			"bgeu x24, x16, 1f\n"
			"1:\n"
			"fnot.pi f26, f26\n"
			"fnmadd.ps f17, f6, f12, f14, rtz\n"
			"bge x15, x3, 1f\n"
			"1:\n"
			"fmax.pi f31, f1, f1\n"
			"beq x17, x22, 1f\n"
			"1:\n"
			"fnmadd.ps f24, f5, f5, f27, rne\n"
			"fmin.pi f21, f21, f7\n"
			"fcvt.f16.ps f12, f12\n"
			"bgeu x27, x14, 1f\n"
			"1:\n"
			"fsll.pi f5, f14, f21\n"
			"beq x19, x21, 1f\n"
			"1:\n"
			"fand.pi f6, f17, f20\n"
			"fle.ps f3, f27, f3\n"
			"fslli.pi f30, f26, 0x0\n"
			"beq x15, x15, 1f\n"
			"1:\n"
			"fslli.pi f1, f1, 0x9\n"
			"fcvt.f16.ps f31, f30\n"
			"fmin.ps f30, f7, f20\n"
			"fcvt.pwu.ps f30, f21, rtz\n"
			"fsat8.pi f5, f21\n"
			"fpackreph.pi f17, f17\n"
			"fmadd.ps f26, f30, f7, f14, rmm\n"
			"fmul.pi f26, f26, f26\n"
			"fmin.ps f23, f3, f21\n"
			"fle.ps f1, f27, f27\n"
			"fmul.s f1, f30, f12, rup\n"
			"fcvt.pwu.ps f27, f7, rtz\n"
			"blt x12, x21, 1f\n"
			"1:\n"
			"fmin.pi f30, f5, f3\n"
			"fmul.pi f12, f14, f12\n"
			"fnot.pi f23, f6\n"
			"fsat8.pi f17, f5\n"
			"bltu x26, x16, 1f\n"
			"1:\n"
			"fsll.pi f27, f31, f20\n"
			"fround.ps f24, f1, rne\n"
			"fmin.ps f27, f27, f1\n"
			"fmul.s f26, f7, f26, rdn\n"
			"fle.ps f27, f7, f5\n"
			"fmadd.ps f24, f24, f3, f14, rdn\n"
			"fnot.pi f3, f24\n"
			"fcvt.pwu.ps f14, f27, rdn\n"
			"blt x8, x10, 1f\n"
			"1:\n"
			"fltu.pi f3, f30, f3\n"
			"fmax.ps f14, f24, f5\n"
			"fcvt.pw.ps f26, f7, rtz\n"
			"fle.ps f7, f23, f1\n"
			"fmulhu.pi f21, f6, f21\n"
			"fslli.pi f3, f31, 0x6\n"
			"fnmadd.ps f12, f31, f3, f31, rtz\n"
			"fmax.pi f3, f21, f20\n"
			"fle.pi f30, f30, f30\n"
			"bne x18, x8, 1f\n"
			"1:\n"
			"fnot.pi f27, f17\n"
			"fle.ps f20, f12, f20\n"
			"fand.pi f31, f1, f1\n"
			"fsgnjn.ps f7, f5, f7\n"
			"fle.ps f7, f21, f7\n"
			"bge x23, x18, 1f\n"
			"1:\n"
			"fmadd.ps f14, f1, f17, f7, rtz\n"
			"bgeu x5, x1, 1f\n"
			"1:\n"
			"fltu.pi f7, f7, f26\n"
			"fcvt.pwu.ps f27, f3, rmm\n"
			"beq x19, x25, 1f\n"
			"1:\n"
			"fltu.pi f23, f3, f20\n"
			"fltu.pi f17, f5, f3\n"
			"fmulhu.pi f14, f3, f6\n"
			"fcvt.pwu.ps f24, f26, rtz\n"
			"fmulhu.pi f12, f12, f20\n"
			"fmul.s f26, f31, f23, rdn\n"
			"bge x11, x2, 1f\n"
			"1:\n"
			"fmax.pi f14, f27, f27\n"
			"fmul.s f5, f5, f30, rdn\n"
			"fmax.pi f17, f7, f17\n"
			"bge x7, x18, 1f\n"
			"1:\n"
			"fsat8.pi f24, f30\n"
			"fmax.ps f6, f17, f6\n"
			"fmin.ps f27, f26, f23\n"
			"fpackreph.pi f12, f20\n"
			"bgeu x29, x15, 1f\n"
			"1:\n"
			"fcvt.f16.ps f30, f26\n"
			"fle.pi f14, f30, f6\n"
			"fsat8.pi f17, f17\n"
			"fsgnjn.ps f7, f1, f31\n"
			"fmul.s f14, f7, f14, rne\n"
			"fcvt.pwu.ps f7, f20, rtz\n"
			"fltu.pi f1, f6, f24\n"
			"fmul.s f7, f24, f6, rtz\n"
			"fsat8.pi f5, f5\n"
			"bge x18, x19, 1f\n"
			"1:\n"
			"fand.pi f20, f30, f12\n"
			"addi x21, x21, -1\n"
			"bne x21, x0, LBL_SEQID_0_LOOP_SEQ_HID_0\n"
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_0\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_1_INT_AMO_ID_3_HID_0:\n"
			"li x20, 0x823af2c120\n" 
			"amominug.w x11, x26, (x20)\n" 
			"fclass.ps f12, f12\n"
			"fxor.pi f27, f14, f17\n"
			"LBL_SEQID_1_FP_AMO_ID_4_HID_0:\n"
			"li x28, 0x8243115838\n" 
			"la x20, mem_ptr_offset\n"
			"flw.ps f24, 960(x20)\n"
			"famoandl.pi f14, f24 (x28)\n" 
			"fltu.pi f27, f24, f3\n"
			"blt x28, x24, 1f\n"
			"1:\n"
			"fsgnjx.s f5, f27, f5\n"
			"LBL_SEQID_1_INT_AMO_ID_5_HID_0:\n"
			"li x12, 0x824292e020\n" 
			"amominul.d x17, x23, (x12)\n" 
			"frsq.ps f3, f3\n"
			"bge x6, x14, 1f\n"
			"1:\n"
			"feq.ps f20, f6, f20\n"
			"LBL_SEQID_1_FP_AMO_ID_6_HID_0:\n"
			"li x9, 0x814cd7e010\n" 
			"la x28, mem_ptr_offset\n"
			"flw.ps f5, 736(x28)\n"
			"famomaxl.pi f1, f5 (x9)\n" 
			"fsub.pi f5, f21, f5\n"
			"fsatu8.pi f20, f12\n"
			"LBL_SEQID_1_FP_SCATTER_ID_7_HID_0:\n"
			"li x6, 0x820292e022\n" 
			"la x7, mem_ptr_offset\n"
			"flq2 f27, 160(x7)\n"
			"fsch.ps f23, f27 (x6)\n" 
			"rem x26, x23, x26\n"
			"fmax.s f20, f20, f27\n"
			"LBL_SEQID_1_FP_GATHER_ID_8_HID_0:\n"
			"li x7, 0x810617009e\n" 
			"la x6, mem_ptr_offset\n"
			"flw.ps f27, 8(x6)\n"
			"fscw.ps f12, f27 (x7)\n"
			"fgb.ps f5, f27 (x7)\n" 
			"fsra.pi f30, f24, f27\n"
			"fnmadd.ps f24, f24, f12, f12, rmm\n"
			"LBL_SEQID_1_CSR_ID_9_HID_0:\n"
			"li x12, 0x840000810c66000c\n"
			"li x31, (0x2 << 6) | 0x0\n"
			"csrw evict_va, x12\n" 
			"csrwi tensor_wait, 6\n" 
			"fmvs.x.ps x9, f3, 4\n"
			"beq x27, x18, 1f\n"
			"1:\n"
			"fence\n"
			"blt x25, x3, 1f\n"
			"1:\n"
			"LBL_SEQID_1_FP_SCATTER_ID_10_HID_0:\n"
			"li x11, 0x82001a0002\n" 
			"la x20, mem_ptr_offset\n"
			"flw.ps f24, 112(x20)\n"
			"fscw.ps f21, f24 (x11)\n" 
			"feq.s x26, f12, f21\n"
			"for.pi f26, f14, f27\n"
			"LBL_SEQID_1_CSR_ID_11_HID_0:\n"
			"li x7, 0x815f\n"
			"csrw tensor_mask, x7\n" 
			"fmulhu.pi f14, f14, f12\n"
			"fmin.ps f21, f12, f12\n"
			"LBL_SEQID_1_FP_AMO_ID_12_HID_0:\n"
			"li x20, 0x8247407010\n" 
			"la x28, mem_ptr_offset\n"
			"flw.ps f17, 352(x28)\n"
			"famominl.ps f20, f17 (x20)\n" 
			"feq.pi f3, f31, f6\n"
			"fswizz.ps f14, f3, 0x11\n"
			"LBL_SEQID_1_FP_L_SCATTER_ID_13_HID_0:\n"
			"li x7, 0x82505205e8\n" 
			"la x28, mem_ptr_offset\n"
			"flq2 f21, 108(x28)\n"
			"fscbl.ps f17, f21 (x7)\n" 
			"feq.ps f24, f27, f3\n"
			"addiw x17, x11, 0x8\n"
			"LBL_SEQID_1_FP_L_GATHER_ID_14_HID_0:\n"
			"li x28, 0x815f2ab668\n" 
			"la x11, mem_ptr_offset\n"
			"flq2 f23, 76(x11)\n"
			"fscwl.ps f14, f23 (x28)\n"
			"fgbl.ps f23, f23 (x28)\n" 
			"auipc x11, 0xd\n"
			"slt x6, x28, x11\n"
			"LBL_SEQID_1_INT_AMO_ID_15_HID_0:\n"
			"li x12, 0x813fd40018\n" 
			"amominug.w x11, x26, (x12)\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"fsgnjx.s f27, f23, f17\n"
			"fmadd.ps f17, f17, f12, f1, rup\n"
			"LBL_SEQID_1_FP_AMO_ID_16_HID_0:\n"
			"li x7, 0x82428a072c\n" 
			"la x6, mem_ptr_offset\n"
			"flw.ps f3, 640(x6)\n"
			"famomaxl.ps f7, f3 (x7)\n" 
			"fmulh.pi f31, f3, f31\n"
			"fmul.ps f6, f23, f6, rne\n"
			"LBL_SEQID_1_INT_AMO_ID_17_HID_0:\n"
			"li x20, 0x82223b2838\n" 
			"amomaxg.d x6, x17, (x20)\n" 
			"fcvt.pw.ps f23, f1, rup\n"
			"fmin.pi f24, f5, f12\n"
			"LBL_SEQID_1_INT_AMO_ID_18_HID_0:\n"
			"li x20, 0x8141c6a550\n" 
			"amoswapl.d x28, x28, (x20)\n" 
			"fnmadd.ps f27, f26, f20, f7, rdn\n"
			"feq.pi f12, f6, f17\n"
			"LBL_SEQID_1_INT_LOAD_ID_19_HID_0:\n"
			"li x26, 0x8108d1b038\n" 
			"sd x28, 1657(x26)\n"
			"ld x28, 1657(x26)\n" 
			"fcmov.ps f24, f1, f5, f1\n"
			"bgeu x3, x28, 1f\n"
			"1:\n"
			"fsatu8.pi f30, f7\n"
			"LBL_SEQID_1_FP_G_SCATTER_ID_20_HID_0:\n"
			"li x12, 0x812b078028\n" 
			"la x11, mem_ptr_offset\n"
			"flq2 f24, 124(x11)\n"
			"fscwg.ps f14, f24 (x12)\n" 
			"fsra.pi f1, f17, f30\n"
			"bne x4, x4, 1f\n"
			"1:\n"
			"fsub.s f12, f3, f12, rup\n"
			"LBL_SEQID_1_INT_AMO_ID_21_HID_0:\n"
			"li x11, 0x814cd7e010\n" 
			"amoaddl.w x12, x12, (x11)\n" 
			"flt.ps f23, f1, f23\n"
			"ffrc.ps f14, f21\n"
			"LBL_SEQID_1_INT_LOAD_ID_22_HID_0:\n"
			"li x28, 0x81001c0013\n" 
			"sd x11, 1469(x28)\n"
			"lb x26, 1469(x28)\n" 
			"fmv.x.s x26, f31\n"
			"flog.ps f3, f3\n"
			"LBL_SEQID_1_FP_L_GATHER_ID_23_HID_0:\n"
			"li x17, 0x8141c6a550\n" 
			"la x28, mem_ptr_offset\n"
			"flq2 f14, 216(x28)\n"
			"fscwl.ps f21, f14 (x17)\n"
			"fgbl.ps f26, f14 (x17)\n" 
			"fmadd.s f5, f14, f3, f5, rdn\n"
			"fsub.ps f7, f7, f1, rne\n"
			"LBL_SEQID_1_FP_AMO_ID_24_HID_0:\n"
			"li x11, 0x814b07802c\n" 
			"la x17, mem_ptr_offset\n"
			"flw.ps f30, 480(x17)\n"
			"famominl.ps f5, f30 (x11)\n" 
			"fround.ps f5, f24, rup\n"
			"maskxor m7, m7, m6\n"
			"LBL_SEQID_1_CSR_ID_25_HID_0:\n"
			"csrr x0, fcsr\n"
			"fcvt.pw.ps f24, f24, rmm\n"
			"fadd.s f1, f21, f24, rmm\n"
			"LBL_SEQID_1_INT_AMO_ID_26_HID_0:\n"
			"li x23, 0x8153760118\n" 
			"amomaxl.d x9, x7, (x23)\n" 
			"fcvt.ps.f16 f3, f26\n"
			"frsq.ps f12, f30\n"
			"LBL_SEQID_1_FP_AMO_ID_27_HID_0:\n"
			"li x28, 0x815f2ab668\n" 
			"la x7, mem_ptr_offset\n"
			"flw.ps f23, 672(x7)\n"
			"famomaxul.pi f23, f23 (x28)\n" 
			"li x5, 0x73cce81954577fb5\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0x58a7bf3c601f682c\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"fsub.s f20, f3, f23, rdn\n"
			"beq x6, x22, 1f\n"
			"1:\n"
			"fcvt.ps.pw f5, f1, rdn\n"
			"bgeu x13, x30, 1f\n"
			"1:\n"
			"LBL_SEQID_1_FP_G_GATHER_ID_28_HID_0:\n"
			"li x28, 0x82228a072c\n" 
			"la x12, mem_ptr_offset\n"
			"flq2 f21, 36(x12)\n"
			"fscwg.ps f0, f21 (x28)\n"
			"fghg.ps f31, f21 (x28)\n" 
			"srli x6, x6, 5\n"
			"fsub.pi f5, f20, f26\n"
			"LBL_SEQID_1_FP_SCATTER_ID_29_HID_0:\n"
			"li x6, 0x820ca20018\n" 
			"la x7, mem_ptr_offset\n"
			"flw.ps f31, 68(x7)\n"
			"fsch.ps f5, f31 (x6)\n" 
			"fcvt.s.w f5, x28, rdn\n"
			"xor x28, x28, x17\n"
			"LBL_SEQID_1_INT_AMO_ID_30_HID_0:\n"
			"li x23, 0x814c66000c\n" 
			"amoaddl.w x28, x6, (x23)\n" 
			"fnmadd.s f27, f23, f24, f24, rmm\n"
			"fle.pi f5, f24, f23\n"
			"LBL_SEQID_1_FP_AMO_ID_31_HID_0:\n"
			"li x23, 0x8227407010\n" 
			"la x9, mem_ptr_offset\n"
			"flw.ps f12, 832(x9)\n"
			"famoming.pi f30, f12 (x23)\n" 
			"fand.pi f30, f26, f14\n"
			"fcmov.ps f23, f21, f23, f23\n"
			"LBL_SEQID_1_INT_AMO_ID_32_HID_0:\n"
			"li x26, 0x82413e0034\n" 
			"amomaxul.w x7, x9, (x26)\n" 
			"fmsub.s f27, f30, f23, f27, rmm\n"
			"fsgnjx.s f24, f17, f17\n"
			"LBL_SEQID_1_FP_G_SCATTER_ID_33_HID_0:\n"
			"li x11, 0x823292e020\n" 
			"la x9, mem_ptr_offset\n"
			"flq2 f12, 176(x9)\n"
			"fscwg.ps f26, f12 (x11)\n" 
			"ffrc.ps f20, f12\n"
			"bne x1, x16, 1f\n"
			"1:\n"
			"fsrli.pi f1, f31, 0xe\n"
			"bge x13, x17, 1f\n"
			"1:\n"
			"LBL_SEQID_1_INT_LOAD_ID_34_HID_0:\n"
			"li x28, 0x8106199160\n" 
			"sd x6, 460(x28)\n"
			"lbu x11, 460(x28)\n" 
			"fcvt.pw.ps f31, f1, rdn\n"
			"fslli.pi f20, f1, 0x3\n"
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
			"fpackrepb.pi f3, f30\n"
			"fmvz.x.ps x23, f5, 7\n"
			"fclass.ps f12, f21\n"
			"slti x9, x28, 0x1d\n"
			"fcmovm.ps f21, f23, f20\n"
			"lui x7, 0xf\n"
			"fnmsub.ps f24, f30, f30, f24, rne\n"
			"li x5, 0xaccc\n"
			"csrw tensor_mask, x5\n"	
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ID_742971_HID_0:\n"
			"li x7, ((0x1 << TENSOR_STORE_REG_STRIDE) | (0x10 << TENSOR_STORE_START_REG) | (0x0 << TENSOR_STORE_STORE_COLS) | (0x4 << TENSOR_STORE_AROWS) | (0x0 << TENSOR_STORE_COOP_STORE) | (0x0 << TENSOR_STORE_OPC_0) | (0x827db6901 << TENSOR_STORE_VADDR) | 0 )\n" 
			"li x31, (0x20 << 4)\n"
			"csrw tensor_store, x7\n"	
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ID_77963_HID_0:\n"
			"li x7, ((0x1 << TENSOR_STORE_REG_STRIDE) | (0x1f << TENSOR_STORE_START_REG) | (0x0 << TENSOR_STORE_STORE_COLS) | (0x7 << TENSOR_STORE_AROWS) | (0x0 << TENSOR_STORE_COOP_STORE) | (0x0 << TENSOR_STORE_OPC_0) | (0x826af2c12 << TENSOR_STORE_VADDR) | 0 )\n" 
			"li x31, (0x40 << 4)\n"
			"csrw tensor_store, x7\n"	
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ID_696752_HID_0:\n"
			"li x7, ((0x0 << TENSOR_STORE_REG_STRIDE) | (0x17 << TENSOR_STORE_START_REG) | (0x3 << TENSOR_STORE_STORE_COLS) | (0xa << TENSOR_STORE_AROWS) | (0x0 << TENSOR_STORE_COOP_STORE) | (0x0 << TENSOR_STORE_OPC_0) | (0x819337aec << TENSOR_STORE_VADDR) | 0 )\n" 
			"li x31, (0x80 << 4)\n"
			"csrw tensor_store, x7\n"	
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ID_301880_HID_0:\n"
			"li x7, ((0x1 << TENSOR_STORE_REG_STRIDE) | (0x11 << TENSOR_STORE_START_REG) | (0x1 << TENSOR_STORE_STORE_COLS) | (0xe << TENSOR_STORE_AROWS) | (0x0 << TENSOR_STORE_COOP_STORE) | (0x0 << TENSOR_STORE_OPC_0) | (0xbf80239 << TENSOR_STORE_VADDR) | 0 )\n" 
			"li x31, (0x80 << 4)\n"
			"csrw tensor_store, x7\n"	
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ID_836280_HID_0:\n"
			"li x7, ((0x1 << TENSOR_STORE_REG_STRIDE) | (0x17 << TENSOR_STORE_START_REG) | (0x1 << TENSOR_STORE_STORE_COLS) | (0x3 << TENSOR_STORE_AROWS) | (0x0 << TENSOR_STORE_COOP_STORE) | (0x0 << TENSOR_STORE_OPC_0) | (0x818619916 << TENSOR_STORE_VADDR) | 0 )\n" 
			"li x31, (0x40 << 4)\n"
			"csrw tensor_store, x7\n"	
			"csrwi tensor_wait, 8\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_0\n"
			"csrw tensor_error, x0\n"
			"fmul.s f5, f1, f23, rmm\n"
			"fle.ps f30, f21, f3\n"
			"fcvt.pwu.ps f1, f24, rne\n"
			"fmax.s f12, f3, f26\n"
			"fsin.ps f20, f26\n"
			"flog.ps f1, f30\n"
			"fand.pi f12, f20, f14\n"
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ITER_0_HID_0:\n"
			"li x6, ((0x10 << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0xa << TENSOR_QUANT_QUANT_ROW) | (0x39 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x1 << TENSOR_QUANT_TRANSF8) | (0xa << TENSOR_QUANT_TRANSF7) | (0x3 << TENSOR_QUANT_TRANSF6) | (0x9 << TENSOR_QUANT_TRANSF5) | (0x4 << TENSOR_QUANT_TRANSF4) | (0x4 << TENSOR_QUANT_TRANSF3) | (0xa << TENSOR_QUANT_TRANSF2) | (0x5 << TENSOR_QUANT_TRANSF1) | (0x5 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"li x5, 0x10\n"
			"la x31, rand_ieee754_32\n"
			"add x31, x31, x5\n"
			"flw.ps f1, 0(x31)\n"
			"flw.ps f12, 3(x31)\n"
			"flw.ps f14, 3(x31)\n"
			"flw.ps f17, 9(x31)\n"
			"flw.ps f20, 9(x31)\n"
			"flw.ps f21, 6(x31)\n"
			"flw.ps f23, 23(x31)\n"
			"flw.ps f24, 27(x31)\n"
			"flw.ps f26, 10(x31)\n"
			"flw.ps f27, 17(x31)\n"
			"flw.ps f3, 10(x31)\n"
			"flw.ps f30, 14(x31)\n"
			"flw.ps f31, 45(x31)\n"
			"flw.ps f5, 16(x31)\n"
			"flw.ps f6, 43(x31)\n"
			"flw.ps f7, 35(x31)\n"
			"la x31, rand_int_32\n"
			"add x31, x31, x5\n"
			"lw x0, 0(x31)\n"
			"lw x11, 4(x31)\n"
			"lw x12, 6(x31)\n"
			"lw x17, 12(x31)\n"
			"lw x20, 10(x31)\n"
			"lw x23, 9(x31)\n"
			"lw x26, 4(x31)\n"
			"lw x28, 21(x31)\n"
			"lw x6, 24(x31)\n"
			"lw x7, 23(x31)\n"
			"lw x9, 2(x31)\n"
			"li x21, 0x1\n"
			"LBL_SEQID_1_LOOP_SEQ_HID_0:\n"
			"ecall\n"
			"flog.ps f5, f14\n"
			"fcmovm.ps f6, f24, f30\n"
			"fsgnjn.s f23, f21, f5\n"
			"fence\n"
			"fsub.s f6, f27, f5, rdn\n"
			"fadd.pi f24, f24, f12\n"
			"frsq.ps f7, f21\n"
			"fle.s x28, f5, f17\n"
			"fsgnjx.ps f6, f23, f6\n"
			"remu x26, x11, x20\n"
			"remu x6, x11, x6\n"
			"fclass.ps f30, f6\n"
			"sltiu x6, x23, 0xe\n"
			"ori x0, x11, 0x4\n"
			"divw x26, x7, x26\n"
			"fmv.s.x f21, x26\n"
			"fnmadd.ps f23, f24, f24, f23, rdn\n"
			"maskpopc x23, m7\n"
			"or x26, x26, x0\n"
			"fmulh.pi f23, f20, f6\n"
			"beq x16, x26, 1f\n"
			"1:\n"
			"xori x17, x20, 0x16\n"
			"addw x20, x0, x20\n"
			"fsub.ps f27, f3, f26, rmm\n"
			"fmsub.s f17, f31, f26, f24, rne\n"
			"fmv.x.s x0, f17\n"
			"sra x11, x12, x6\n"
			"fsgnjn.ps f6, f31, f26\n"
			"fswizz.ps f5, f14, 0xe\n"
			"fle.pi f12, f1, f12\n"
			"slliw x23, x28, 5\n"
			"fmulhu.pi f27, f21, f12\n"
			"feq.ps f31, f24, f3\n"
			"flog.ps f6, f30\n"
			"blt x18, x25, 1f\n"
			"1:\n"
			"remuw x9, x28, x11\n"
			"fsatu8.pi f12, f14\n"
			"slli x26, x23, 3\n"
			"sub x23, x17, x6\n"
			"xor x28, x23, x26\n"
			"bgeu x7, x31, 1f\n"
			"1:\n"
			"addw x11, x7, x11\n"
			"mulw x7, x7, x12\n"
			"sllw x6, x17, x12\n"
			"faddi.pi f23, f14, 0xbf\n"
			"fsub.pi f30, f3, f24\n"
			"bge x11, x10, 1f\n"
			"1:\n"
			"sll x0, x12, x0\n"
			"fmv.x.s x28, f21\n"
			"srlw x20, x28, x6\n"
			"remw x11, x11, x9\n"
			"ori x6, x11, 0x1\n"
			"bge x29, x3, 1f\n"
			"1:\n"
			"xor x12, x28, x12\n"
			"auipc x12, 0xd\n"
			"fmv.s.x f27, x28\n"
			"bltu x15, x0, 1f\n"
			"1:\n"
			"fmulhu.pi f17, f6, f7\n"
			"beq x14, x24, 1f\n"
			"1:\n"
			"ffrc.ps f27, f23\n"
			"or x26, x0, x20\n"
			"masknot m4, m6\n"
			"beq x6, x29, 1f\n"
			"1:\n"
			"fle.s x9, f7, f17\n"
			"bltu x0, x12, 1f\n"
			"1:\n"
			"flog.ps f5, f27\n"
			"frcp.ps f27, f6\n"
			"addiw x11, x26, 0x1c\n"
			"ffrc.ps f20, f27\n"
			"beq x18, x22, 1f\n"
			"1:\n"
			"and x11, x23, x12\n"
			"fnmadd.s f14, f3, f14, f23, rmm\n"
			"fsgnjx.ps f31, f3, f31\n"
			"bne x12, x11, 1f\n"
			"1:\n"
			"fclass.s x12, f20\n"
			"bltu x9, x30, 1f\n"
			"1:\n"
			"fsgnjx.ps f3, f12, f14\n"
			"fnot.pi f14, f21\n"
			"remw x0, x26, x28\n"
			"fnmsub.ps f31, f20, f26, f7, rdn\n"
			"flog.ps f5, f27\n"
			"blt x2, x9, 1f\n"
			"1:\n"
			"fclass.s x26, f21\n"
			"fcvt.s.w f21, x9, rmm\n"
			"bge x1, x1, 1f\n"
			"1:\n"
			"maskpopcz x17, m6\n"
			"srlw x9, x7, x23\n"
			"bgeu x1, x8, 1f\n"
			"1:\n"
			"frsq.ps f7, f7\n"
			"fle.ps f3, f20, f21\n"
			"fmin.s f5, f26, f30\n"
			"sub x12, x12, x6\n"
			"bge x14, x18, 1f\n"
			"1:\n"
			"fnmadd.ps f6, f14, f6, f6, rmm\n"
			"fmadd.ps f17, f6, f6, f24, rup\n"
			"bltu x11, x10, 1f\n"
			"1:\n"
			"fcvt.ps.pwu f20, f24, rup\n"
			"bltu x9, x20, 1f\n"
			"1:\n"
			"fsrli.pi f6, f31, 0x1\n"
			"mulhsu x9, x9, x7\n"
			"bne x3, x21, 1f\n"
			"1:\n"
			"fsrai.pi f30, f30, 0x6\n"
			"maskpopc x11, m6\n"
			"bltu x10, x5, 1f\n"
			"1:\n"
			"remw x20, x9, x11\n"
			"div x12, x20, x12\n"
			"beq x21, x11, 1f\n"
			"1:\n"
			"remu x17, x28, x9\n"
			"fsub.ps f14, f23, f12, rtz\n"
			"fmax.ps f1, f6, f24\n"
			"beq x21, x31, 1f\n"
			"1:\n"
			"and x6, x12, x6\n"
			"addi x12, x12, 0x16\n"
			"srlw x0, x23, x12\n"
			"mulh x20, x0, x28\n"
			"fmvs.x.ps x0, f24, 1\n"
			"fmaxu.pi f23, f3, f30\n"
			"bgeu x28, x21, 1f\n"
			"1:\n"
			"fltm.pi m4, f23, f23\n"
			"fmv.x.s x6, f20\n"
			"fmul.s f31, f17, f31, rup\n"
			"fcvt.ps.pw f31, f6, rup\n"
			"addi x21, x21, -1\n"
			"bne x21, x0, LBL_SEQID_1_LOOP_SEQ_HID_0\n"
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_0\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_2_INT_AMO_ID_35_HID_0:\n"
			"li x23, 0x823e240178\n" 
			"amoxorg.d x28, x6, (x23)\n" 
			"fmvs.x.ps x28, f24, 7\n"
			"fcvt.ps.pwu f20, f31, rmm\n"
			"addw x9, x11, x11\n"
			"fsra.pi f1, f27, f5\n"
			"fmaxu.pi f27, f12, f27\n"
			"frsq.ps f27, f24\n"
			"fsgnjx.ps f27, f27, f24\n"
			"feq.ps f12, f12, f27\n"
			"LBL_SEQID_2_INT_AMO_ID_36_HID_0:\n"
			"li x23, 0x813af5201c\n" 
			"amoxorg.w x17, x7, (x23)\n" 
			"fsat8.pi f30, f30\n"
			"fmul.ps f12, f12, f14, rne\n"
			"fmul.pi f23, f14, f6\n"
			"flog.ps f24, f14\n"
			"fmin.ps f27, f6, f27\n"
			"fadd.s f12, f14, f12, rdn\n"
			"fminu.pi f24, f14, f27\n"
			"fandi.pi f24, f6, 0x57\n"
			"LBL_SEQID_2_FP_LOAD_ID_37_HID_0:\n"
			"li x9, 0x8106fa000b\n" 
			"fsw.ps f26, -704(x9)\n"
			"flw f31, -704(x9)\n" 
			"divw x9, x28, x9\n"
			"bge x15, x9, 1f\n"
			"1:\n"
			"frcp.ps f21, f26\n"
			"mov.m.x m7, x20, 0x32\n"
			"beq x8, x3, 1f\n"
			"1:\n"
			"sub x11, x11, x23\n"
			"fmul.pi f23, f23, f14\n"
			"bge x4, x7, 1f\n"
			"1:\n"
			"fadd.pi f14, f17, f31\n"
			"fslli.pi f5, f26, 0x5\n"
			"bne x29, x8, 1f\n"
			"1:\n"
			"fcvt.ps.pw f31, f27, rmm\n"
			"LBL_SEQID_2_INT_AMO_ID_38_HID_0:\n"
			"li x20, 0x812f068008\n" 
			"amoandg.d x6, x12, (x20)\n" 
			"faddi.pi f14, f5, 0x1b0\n"
			"flem.ps m6, f23, f21\n"
			"fltm.pi m4, f26, f27\n"
			"mova.m.x x23\n"
			"fnmsub.ps f20, f24, f23, f14, rup\n"
			"fswizz.ps f21, f21, 0x11\n"
			"fmul.pi f24, f3, f23\n"
			"mulhu x23, x28, x23\n"
			"LBL_SEQID_2_FP_STORE_ID_39_HID_0:\n"
			"li x20, 0x81058a002c\n" 
			"fsq2 f26, 1870(x20)\n" 
			"faddi.pi f20, f26, 0x4e\n"
			"maskpopc x28, m6\n"
			"slti x7, x7, 0xa\n"
			"beq x22, x27, 1f\n"
			"1:\n"
			"remu x9, x7, x0\n"
			"faddi.pi f23, f1, 0x155\n"
			"srlw x9, x26, x9\n"
			"rem x17, x20, x11\n"
			"subw x23, x23, x6\n"
			"LBL_SEQID_2_FP_AMO_ID_40_HID_0:\n"
			"li x11, 0x8228acb5ec\n" 
			"la x7, mem_ptr_offset\n"
			"flw.ps f17, 192(x7)\n"
			"famoming.pi f30, f17 (x11)\n" 
			"fnmsub.ps f30, f20, f30, f14, rne\n"
			"fltu.pi f26, f24, f20\n"
			"fmsub.ps f23, f23, f26, f23, rdn\n"
			"fnmsub.ps f27, f3, f27, f20, rne\n"
			"bgeu x25, x12, 1f\n"
			"1:\n"
			"fsgnj.ps f27, f27, f6\n"
			"frsq.ps f27, f6\n"
			"fmin.s f27, f3, f17\n"
			"bge x2, x24, 1f\n"
			"1:\n"
			"fnmsub.s f1, f23, f23, f6, rdn\n"
			"LBL_SEQID_2_FP_SCATTER_ID_41_HID_0:\n"
			"li x28, 0x810376011f\n" 
			"la x17, mem_ptr_offset\n"
			"flw.ps f21, 116(x17)\n"
			"fscw.ps f17, f21 (x28)\n" 
			"fxor.pi f17, f3, f17\n"
			"fmulhu.pi f26, f3, f31\n"
			"and x11, x11, x11\n"
			"fmsub.s f17, f24, f1, f17, rmm\n"
			"fmvz.x.ps x23, f1, 4\n"
			"bltu x8, x15, 1f\n"
			"1:\n"
			"masknot m6, m6\n"
			"bne x5, x14, 1f\n"
			"1:\n"
			"fmin.s f17, f3, f17\n"
			"bge x29, x27, 1f\n"
			"1:\n"
			"fsetm.pi m4, f21\n"
			"LBL_SEQID_2_INT_LOAD_ID_42_HID_0:\n"
			"li x23, 0x8108d1b038\n" 
			"sd x9, 1411(x23)\n"
			"lwu x6, 1411(x23)\n" 
			"for.pi f21, f21, f21\n"
			"fsub.s f24, f1, f7, rne\n"
			"fbcx.ps f20, x7\n"
			"mulhu x9, x9, x26\n"
			"fsub.ps f26, f3, f3, rtz\n"
			"fle.pi f20, f12, f1\n"
			"blt x2, x7, 1f\n"
			"1:\n"
			"fmax.ps f1, f1, f23\n"
			"frsq.ps f24, f3\n"
			"LBL_SEQID_2_INT_AMO_ID_43_HID_0:\n"
			"li x6, 0x814337aecc\n" 
			"amominl.w x11, x7, (x6)\n" 
			"flt.pi f30, f24, f21\n"
			"addw x0, x26, x0\n"
			"fltm.pi m6, f30, f24\n"
			"bge x30, x19, 1f\n"
			"1:\n"
			"fcvt.ps.pwu f30, f30, rtz\n"
			"fsub.pi f30, f21, f7\n"
			"fpackreph.pi f1, f17\n"
			"fsgnjx.ps f7, f17, f21\n"
			"sra x6, x9, x0\n"
			"LBL_SEQID_2_FP_STORE_ID_44_HID_0:\n"
			"li x17, 0x82025ac0a4\n" 
			"fsq2 f27, 1115(x17)\n" 
			"fexp.ps f31, f20\n"
			"fmul.pi f31, f20, f27\n"
			"bge x20, x8, 1f\n"
			"1:\n"
			"fcvt.ps.f16 f31, f20\n"
			"faddi.pi f23, f27, 0x37\n"
			"fsub.ps f6, f26, f24, rdn\n"
			"beq x29, x19, 1f\n"
			"1:\n"
			"fmadd.s f5, f5, f20, f30, rmm\n"
			"fmax.pi f6, f6, f24\n"
			"fcvt.pwu.ps f6, f6, rdn\n"
			"LBL_SEQID_2_INT_L_STORE_ID_45_HID_0:\n"
			"li x7, 0x8158040268\n" 
			"shl x26, (x7)\n" 
			"fmax.s f31, f21, f31\n"
			"bne x23, x21, 1f\n"
			"1:\n"
			"fmvs.x.ps x26, f1, 4\n"
			"fmvs.x.ps x11, f12, 5\n"
			"feqm.ps m6, f23, f12\n"
			"fsetm.pi m6, f21\n"
			"feqm.ps m2, f21, f20\n"
			"and x7, x26, x9\n"
			"bgeu x14, x15, 1f\n"
			"1:\n"
			"maskor m6, m4, m7\n"
			"LBL_SEQID_2_INT_AMO_ID_46_HID_0:\n"
			"li x6, 0x814b07802c\n" 
			"amomaxul.w x11, x12, (x6)\n" 
			"fsin.ps f24, f27\n"
			"remu x23, x9, x12\n"
			"bne x28, x11, 1f\n"
			"1:\n"
			"fcvt.ps.f16 f1, f17\n"
			"fsin.ps f3, f3\n"
			"mulhu x6, x26, x6\n"
			"fand.pi f6, f3, f20\n"
			"bgeu x14, x17, 1f\n"
			"1:\n"
			"fslli.pi f24, f24, 0x8\n"
			"fmvs.x.ps x6, f3, 1\n"
			"la x12, rand_ieee754_32\n"
			"flw.ps f21, 384(x12)\n"
			"fsqrt.s f27, f21, rup\n" 
			"srliw x6, x6, 5\n"
			"andi x6, x28, 0x16\n"
			"ffrc.ps f7, f7\n"
			"sltu x6, x6, x0\n"
			"addi x9, x26, 0x1\n"
			"maskor m4, m4, m2\n"
			"flem.ps m7, f23, f20\n"
			"flem.ps m4, f5, f7\n"
			"LBL_SEQID_2_INT_L_STORE_ID_47_HID_0:\n"
			"li x7, 0x815617009c\n" 
			"shl x20, (x7)\n" 
			"fadd.ps f31, f17, f31, rup\n"
			"fmax.ps f17, f31, f17\n"
			"fle.ps f17, f17, f30\n"
			"fmax.pi f6, f6, f6\n"
			"bgeu x19, x11, 1f\n"
			"1:\n"
			"fmul.ps f3, f17, f6, rup\n"
			"fmin.ps f20, f31, f20\n"
			"fround.ps f6, f6, rmm\n"
			"fle.pi f14, f31, f17\n"
			"LBL_SEQID_2_CSR_ID_48_HID_0:\n"
			"csrr x0, sstatus\n"
			"fadd.pi f12, f23, f12\n"
			"fswizz.ps f20, f20, 0x15\n"
			"fmulhu.pi f3, f20, f3\n"
			"fcvt.pw.ps f17, f12, rne\n"
			"fmul.ps f14, f30, f27, rup\n"
			"fmulh.pi f20, f7, f12\n"
			"fmsub.s f20, f26, f12, f20, rtz\n"
			"fsin.ps f17, f30\n"
			"LBL_SEQID_2_CSR_ID_49_HID_0:\n"
			"li x28, 0xc00008201da06c2\n"
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw prefetch_va, x28\n" 
			"csrwi tensor_wait, 6\n" 
			"csrwi tensor_wait, 4\n" 
			"flt.ps f7, f14, f7\n"
			"fsub.ps f24, f6, f24, rdn\n"
			"fsgnj.ps f6, f24, f26\n"
			"beq x16, x6, 1f\n"
			"1:\n"
			"feq.pi f3, f31, f24\n"
			"fsub.s f14, f14, f27, rtz\n"
			"fmsub.s f20, f20, f12, f3, rne\n"
			"fcmovm.ps f27, f27, f27\n"
			"bltu x8, x23, 1f\n"
			"1:\n"
			"fcvt.ps.pw f6, f23, rdn\n"
			"LBL_SEQID_2_FP_L_GATHER_ID_50_HID_0:\n"
			"li x11, 0x82505205e8\n" 
			"la x12, mem_ptr_offset\n"
			"flw.ps f5, 148(x12)\n"
			"fscwl.ps f27, f5 (x11)\n"
			"fghl.ps f12, f5 (x11)\n" 
			"fbci.ps f24, 0x75762\n"
			"fmax.s f24, f5, f24\n"
			"slt x17, x26, x7\n"
			"maskpopc x7, m7\n"
			"fcvt.pwu.ps f1, f21, rne\n"
			"fslli.pi f21, f24, 0xa\n"
			"auipc x0, 0x14\n"
			"fsub.s f21, f5, f21, rdn\n"
			"LBL_SEQID_2_INT_AMO_ID_51_HID_0:\n"
			"li x6, 0x814b07802c\n" 
			"amoaddl.w x7, x17, (x6)\n" 
			"fsgnjn.s f6, f21, f6\n"
			"fle.pi f12, f3, f24\n"
			"flog.ps f6, f6\n"
			"fmadd.ps f30, f30, f5, f7, rup\n"
			"feq.ps f24, f14, f20\n"
			"fpackrepb.pi f20, f27\n"
			"fadd.pi f7, f5, f24\n"
			"fcvt.f16.ps f21, f24\n"
			"LBL_SEQID_2_INT_AMO_ID_52_HID_0:\n"
			"li x12, 0x822ca20018\n" 
			"amoorg.d x9, x17, (x12)\n" 
			"slliw x20, x12, 2\n"
			"fslli.pi f26, f26, 0x9\n"
			"bltu x5, x27, 1f\n"
			"1:\n"
			"fmadd.ps f3, f3, f12, f12, rmm\n"
			"beq x20, x27, 1f\n"
			"1:\n"
			"sltu x9, x7, x0\n"
			"beq x11, x12, 1f\n"
			"1:\n"
			"fsin.ps f3, f20\n"
			"sltiu x7, x7, 0x1c\n"
			"fmsub.s f26, f30, f26, f12, rdn\n"
			"fcvt.w.s x11, f7, rdn\n"
			"LBL_SEQID_2_INT_AMO_ID_53_HID_0:\n"
			"li x28, 0x813c66000c\n" 
			"amomaxug.w x11, x26, (x28)\n" 
			"slt x28, x7, x12\n"
			"fmaxu.pi f12, f12, f14\n"
			"fmv.s.x f20, x12\n"
			"fmadd.ps f17, f14, f20, f5, rmm\n"
			"fminu.pi f23, f26, f7\n"
			"bltu x19, x8, 1f\n"
			"1:\n"
			"slli x6, x6, 5\n"
			"beq x9, x31, 1f\n"
			"1:\n"
			"addi x0, x17, 0xa\n"
			"bltu x10, x14, 1f\n"
			"1:\n"
			"lui x23, 0x1a\n"
			"LBL_SEQID_2_FP_LOAD_ID_54_HID_0:\n"
			"li x23, 0x82030ccf07\n" 
			"fsw.ps f17, 1818(x23)\n"
			"flw f31, 1818(x23)\n" 
			"fcmovm.ps f20, f17, f1\n"
			"fcvt.pwu.ps f31, f1, rmm\n"
			"fmadd.ps f30, f23, f5, f30, rne\n"
			"fcvt.f16.ps f26, f3\n"
			"fpackreph.pi f30, f24\n"
			"fnmsub.ps f14, f1, f5, f23, rtz\n"
			"fsgnj.ps f30, f6, f5\n"
			"ffrc.ps f3, f27\n"
			"beq x3, x23, 1f\n"
			"1:\n"
			"LBL_SEQID_2_INT_AMO_ID_55_HID_0:\n"
			"li x6, 0x82413e0034\n" 
			"amoaddl.w x11, x26, (x6)\n" 
			"fmin.ps f21, f6, f14\n"
			"fslli.pi f5, f27, 0xf\n"
			"bgeu x11, x17, 1f\n"
			"1:\n"
			"fsub.pi f1, f23, f12\n"
			"fmadd.ps f26, f21, f6, f23, rdn\n"
			"fsll.pi f12, f12, f14\n"
			"bne x24, x16, 1f\n"
			"1:\n"
			"fexp.ps f3, f6\n"
			"fcvt.ps.pwu f31, f31, rdn\n"
			"fnot.pi f14, f6\n"
			"LBL_SEQID_2_FP_L_SCATTER_ID_56_HID_0:\n"
			"li x9, 0x814cd7e010\n" 
			"la x12, mem_ptr_offset\n"
			"flw.ps f20, 84(x12)\n"
			"fscbl.ps f12, f20 (x9)\n" 
			"slliw x7, x11, 4\n"
			"mova.x.m x17\n"
			"fswizz.ps f12, f31, 0x0\n"
			"fle.ps f1, f6, f12\n"
			"fcvt.s.w f26, x0, rmm\n"
			"fcvt.ps.pwu f23, f12, rtz\n"
			"bltu x30, x0, 1f\n"
			"1:\n"
			"slliw x0, x11, 3\n"
			"fandi.pi f23, f26, 0x1d8\n"
			"LBL_SEQID_2_FP_AMO_ID_57_HID_0:\n"
			"li x23, 0x823292e020\n" 
			"la x9, mem_ptr_offset\n"
			"flw.ps f27, 32(x9)\n"
			"famomaxg.pi f7, f27 (x23)\n" 
			"li x5, 0x3fae\n"
			"csrw tensor_mask, x5\n"	
			"fmadd.ps f27, f1, f27, f23, rtz\n"
			"flt.pi f30, f1, f1\n"
			"fsll.pi f5, f26, f3\n"
			"fcvt.ps.pwu f1, f31, rne\n"
			"bge x27, x13, 1f\n"
			"1:\n"
			"fsub.s f5, f27, f3, rtz\n"
			"bne x21, x11, 1f\n"
			"1:\n"
			"fsgnj.ps f5, f30, f3\n"
			"fslli.pi f3, f1, 0xd\n"
			"fltu.pi f1, f27, f1\n"
			"LBL_SEQID_2_INT_AMO_ID_58_HID_0:\n"
			"li x6, 0x8121c6a550\n" 
			"amomaxg.w x26, x26, (x6)\n" 
			"fmulhu.pi f7, f20, f24\n"
			"fmadd.ps f24, f17, f30, f14, rmm\n"
			"frcp.ps f20, f30\n"
			"fmulhu.pi f12, f30, f12\n"
			"sra x12, x20, x11\n"
			"blt x27, x25, 1f\n"
			"1:\n"
			"fmv.x.s x26, f1\n"
			"sltu x26, x26, x28\n"
			"bne x14, x3, 1f\n"
			"1:\n"
			"fand.pi f17, f24, f14\n"
			"LBL_SEQID_2_CSR_ID_59_HID_0:\n"
			"li x20, 0x9c9900006975\n"
			"csrw tensor_conv_ctrl, x20\n" 
			"li x5, 0xe90b\n"
			"csrw tensor_mask, x5\n"	
			"fmulh.pi f14, f14, f14\n"
			"bge x3, x14, 1f\n"
			"1:\n"
			"ori x12, x9, 0xa\n"
			"fcvt.pwu.ps f14, f14, rdn\n"
			"slt x12, x12, x9\n"
			"fcvt.f16.ps f1, f21\n"
			"ebreak\n"
			"beq x3, x7, 1f\n"
			"1:\n"
			"fcvt.ps.f16 f31, f21\n"
			"fmaxu.pi f1, f3, f7\n"
			"LBL_SEQID_2_FP_SCATTER_ID_60_HID_0:\n"
			"li x7, 0x810f06800a\n" 
			"la x23, mem_ptr_offset\n"
			"flw.ps f3, 108(x23)\n"
			"fscw.ps f26, f3 (x7)\n" 
			"fcvt.pw.ps f23, f3, rdn\n"
			"fmulhu.pi f6, f3, f14\n"
			"flog.ps f17, f17\n"
			"fnmadd.ps f14, f14, f17, f17, rmm\n"
			"fsrli.pi f23, f3, 0x7\n"
			"fminu.pi f6, f3, f17\n"
			"fmul.pi f14, f17, f14\n"
			"fmin.ps f14, f17, f17\n"
			"LBL_SEQID_2_CSR_ID_61_HID_0:\n"
			"csrr x0, mscratch\n"
			"fnot.pi f3, f3\n"
			"bge x23, x24, 1f\n"
			"1:\n"
			"sltu x12, x12, x26\n"
			"fcvt.wu.s x6, f31, rne\n"
			"remuw x17, x17, x12\n"
			"bge x1, x12, 1f\n"
			"1:\n"
			"fmul.pi f23, f14, f23\n"
			"maskor m4, m4, m4\n"
			"fltu.pi f20, f20, f3\n"
			"fcvt.w.s x17, f20, rmm\n"
			"LBL_SEQID_2_INT_LOAD_ID_62_HID_0:\n"
			"li x12, 0x810f06800a\n" 
			"sd x26, -1822(x12)\n"
			"lwu x11, -1822(x12)\n" 
			"add x23, x23, x7\n"
			"fminu.pi f31, f31, f31\n"
			"addi x20, x9, 0x1b\n"
			"slli x6, x28, 3\n"
			"slliw x17, x9, 3\n"
			"blt x8, x18, 1f\n"
			"1:\n"
			"fence\n"
			"blt x26, x5, 1f\n"
			"1:\n"
			"fmin.ps f31, f31, f24\n"
			"bge x12, x26, 1f\n"
			"1:\n"
			"fsub.s f24, f24, f31, rup\n"
			"bge x22, x26, 1f\n"
			"1:\n"
			"LBL_SEQID_2_INT_AMO_ID_63_HID_0:\n"
			"li x26, 0x8141c6a550\n" 
			"amoandl.d x7, x17, (x26)\n" 
			"fclass.s x23, f12\n"
			"mul x0, x26, x0\n"
			"bltu x23, x28, 1f\n"
			"1:\n"
			"sltiu x17, x17, 0x7\n"
			"fmin.pi f5, f24, f1\n"
			"beq x25, x4, 1f\n"
			"1:\n"
			"fminu.pi f3, f31, f31\n"
			"maskpopcz x23, m4\n"
			"fmvz.x.ps x11, f26, 6\n"
			"slli x9, x28, 2\n"
			"LBL_SEQID_2_INT_AMO_ID_64_HID_0:\n"
			"li x28, 0x8146fa0008\n" 
			"amoaddl.w x7, x26, (x28)\n" 
			"feq.ps f24, f1, f1\n"
			"fbci.ps f1, 0x223ab\n"
			"fsgnjx.s f23, f23, f21\n"
			"divu x23, x11, x23\n"
			"fbcx.ps f7, x23\n"
			"blt x25, x30, 1f\n"
			"1:\n"
			"xori x12, x20, 0x15\n"
			"fsra.pi f3, f3, f21\n"
			"packb x28, x20, x11\n"
			"LBL_SEQID_2_FP_SCATTER_ID_65_HID_0:\n"
			"li x11, 0x820e24017f\n" 
			"la x12, mem_ptr_offset\n"
			"flq2 f26, 48(x12)\n"
			"fsch.ps f12, f26 (x11)\n" 
			"fsrai.pi f12, f12, 0x8\n"
			"fcvt.s.wu f14, x12, rdn\n"
			"flog.ps f31, f3\n"
			"addi x6, x11, 0xe\n"
			"fsatu8.pi f21, f12\n"
			"bne x26, x18, 1f\n"
			"1:\n"
			"fcvt.ps.f16 f31, f12\n"
			"bgeu x23, x0, 1f\n"
			"1:\n"
			"fsgnj.s f7, f12, f30\n"
			"xor x26, x20, x12\n"
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
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"LBL_SEQID_2_XMA_QNT_STR_SEQ_ID_591526_HID_0:\n"
			"li x5, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x1 << TENSOR_FMA16_B_NUM_COLS) | (0x5 << TENSOR_FMA16_A_NUM_ROWS) | (0xc << TENSOR_FMA16_A_NUM_COLS) | (0xf << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x3c << TENSOR_FMA16_SCP_LOC_B) | (0xcf << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x0 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"LBL_SEQID_2_XMA_QNT_STR_SEQ_ID_368076_HID_0:\n"
			"li x5, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x1 << TENSOR_FMA32_B_NUM_COLS) | (0x1 << TENSOR_FMA32_A_NUM_ROWS) | (0xd << TENSOR_FMA32_A_NUM_COLS) | (0xb << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x74 << TENSOR_FMA32_SCP_LOC_B) | (0x49 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x0 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"LBL_SEQID_2_XMA_QNT_STR_SEQ_ID_357616_HID_0:\n"
			"li x5, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x3 << TENSOR_FMA32_B_NUM_COLS) | (0xa << TENSOR_FMA32_A_NUM_ROWS) | (0x6 << TENSOR_FMA32_A_NUM_COLS) | (0xb << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0xd << TENSOR_FMA32_SCP_LOC_B) | (0xf9 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x0 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"LBL_SEQID_2_XMA_QNT_STR_SEQ_ID_554750_HID_0:\n"
			"li x5, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x3 << TENSOR_IMA_B_NUM_COLS) | (0xa << TENSOR_IMA_A_NUM_ROWS) | (0x8 << TENSOR_IMA_A_NUM_COLS) | (0xe << TENSOR_IMA_OFFSET) | (0x0 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0xc6 << TENSOR_IMA_SCP_LOC_B) | (0x8f << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_0\n"
			"csrw tensor_error, x0\n"
			"faddi.pi f30, f20, 0x4d\n"
			"fadd.ps f30, f27, f7, rdn\n"
			"fslli.pi f24, f31, 0xb\n"
			"fnmsub.ps f17, f14, f23, f23, rup\n"
			"fadd.ps f7, f12, f20, rtz\n"
			"LBL_SEQID_2_TLD_XMA_STR_SEQ_ITER_0_HID_0:\n"
			"la x31, self_check_8\n"
			"li x5, 0x80000000000002\n"
			"add x5, x5, x31\n"
			"li x6, 0x3a000000000000e\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x0 << TENSOR_IMA_B_NUM_COLS) | (0x2 << TENSOR_IMA_A_NUM_ROWS) | (0xe << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1d << TENSOR_IMA_SCP_LOC_B) | (0x4 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x9, ((0x1e << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0x2 << TENSOR_QUANT_QUANT_ROW) | (0x1b << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x1 << TENSOR_QUANT_TRANSF6) | (0xa << TENSOR_QUANT_TRANSF5) | (0x4 << TENSOR_QUANT_TRANSF4) | (0x8 << TENSOR_QUANT_TRANSF3) | (0x9 << TENSOR_QUANT_TRANSF2) | (0x8 << TENSOR_QUANT_TRANSF1) | (0xa << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fmvz.x.ps x31, f0, 0\n"
			"li x5, 0x3c\n"
			"bne x5, x31, LBL_FAIL_HID_0\n"
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 10\n"
			"LBL_SEQID_2_TLD_XMA_STR_SEQ_ITER_1_HID_0:\n"
			"li x5, ((0x0 << TENSOR_LOAD_T32_USE_TMASK) | (0x0 << TENSOR_LOAD_T32_USE_COOP) | (0x7 << TENSOR_LOAD_T32_OPC_111) | (0x5 << TENSOR_LOAD_T32_DST_START) | (0x20a76da40 << TENSOR_LOAD_T32_VADDR) | (0xa << TENSOR_LOAD_T32_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_SETUP_B_USE_COOP) | (0x0 << TENSOR_LOAD_SETUP_B_OPC_000) | (0x1 << TENSOR_LOAD_SETUP_B_OPC_1) | (0x20a22b2d7 << TENSOR_LOAD_SETUP_B_VADDR) | (0xc << TENSOR_LOAD_SETUP_B_NUM_LINES) | 0 )\n" 
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x1 << TENSOR_FMA16_B_NUM_COLS) | (0xa << TENSOR_FMA16_A_NUM_ROWS) | (0xc << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x1 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1b << TENSOR_FMA16_SCP_LOC_B) | (0x5 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x0 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x9, ((0xe << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0xa << TENSOR_QUANT_QUANT_ROW) | (0x2c << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x2 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 7\n"
			"csrwi tensor_wait, 10\n"
			"LBL_SEQID_2_TLD_XMA_STR_SEQ_ITER_2_HID_0:\n"
			"li x5, ((0x0 << TENSOR_LOAD_T16_USE_TMASK) | (0x0 << TENSOR_LOAD_T16_USE_COOP) | (0x6 << TENSOR_LOAD_T16_OPC_110) | (0x3 << TENSOR_LOAD_T16_DST_START) | (0x2062bd480 << TENSOR_LOAD_T16_VADDR) | (0x0 << TENSOR_LOAD_T16_OFFSET) | (0x3 << TENSOR_LOAD_T16_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_SETUP_B_USE_COOP) | (0x0 << TENSOR_LOAD_SETUP_B_OPC_000) | (0x1 << TENSOR_LOAD_SETUP_B_OPC_1) | (0x200000b << TENSOR_LOAD_SETUP_B_VADDR) | (0x2 << TENSOR_LOAD_SETUP_B_NUM_LINES) | 0 )\n" 
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x0 << TENSOR_FMA16_B_NUM_COLS) | (0x3 << TENSOR_FMA16_A_NUM_ROWS) | (0x2 << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x1 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1a << TENSOR_FMA16_SCP_LOC_B) | (0x3 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x0 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x17 << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0x3 << TENSOR_QUANT_QUANT_ROW) | (0x8 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x2 << TENSOR_QUANT_TRANSF9) | (0x6 << TENSOR_QUANT_TRANSF8) | (0x7 << TENSOR_QUANT_TRANSF7) | (0x7 << TENSOR_QUANT_TRANSF6) | (0x6 << TENSOR_QUANT_TRANSF5) | (0x6 << TENSOR_QUANT_TRANSF4) | (0x7 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x10, ((0x1 << TENSOR_STORE_SCP_ENTRY_STRIDE) | (0x34 << TENSOR_STORE_SCP_START_SCP_ENTRY) | (0xd << TENSOR_STORE_SCP_AROWS) | (0x1 << TENSOR_STORE_SCP_OPC_1) | (0x2067de669 << TENSOR_STORE_SCP_VADDR) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrw tensor_quant, x9\n"	
			"li x31, (0x3 << 6)\n"
			"csrwi tensor_wait, 7\n"
			"csrwi tensor_wait, 10\n"
			"csrw tensor_store, x10\n"	
			"csrwi tensor_wait, 8\n"
			"LBL_SEQID_2_TLD_XMA_STR_SEQ_ITER_3_HID_0:\n"
			"la x31, self_check_16\n"
			"li x5, 0x40000000000004\n"
			"add x5, x5, x31\n"
			"li x6, 0x1000000000000a\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x2 << TENSOR_FMA16_B_NUM_COLS) | (0x4 << TENSOR_FMA16_A_NUM_ROWS) | (0xa << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x1 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1b << TENSOR_FMA16_SCP_LOC_B) | (0x2 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0x16\n"
			"bne x5, x31, LBL_FAIL_HID_0\n"
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
			"li x5, 0x60\n"
			"la x31, rand_ieee754_32\n"
			"add x31, x31, x5\n"
			"flw.ps f1, 0(x31)\n"
			"flw.ps f17, 4(x31)\n"
			"flw.ps f21, 6(x31)\n"
			"flw.ps f23, 11(x31)\n"
			"flw.ps f24, 10(x31)\n"
			"flw.ps f3, 4(x31)\n"
			"flw.ps f30, 6(x31)\n"
			"flw.ps f6, 4(x31)\n"
			"li x21, 0x3\n"
			"LBL_SEQID_2_LOOP_SEQ_HID_0:\n"
			"fround.ps f6, f1, rne\n"
			"flt.pi f17, f6, f24\n"
			"bgeu x21, x2, 1f\n"
			"1:\n"
			"fmax.pi f21, f17, f21\n"
			"flt.ps f21, f23, f17\n"
			"fmulh.pi f5, f3, f17\n"
			"fnot.pi f1, f30\n"
			"fxor.pi f21, f3, f24\n"
			"fcmov.ps f1, f23, f23, f3\n"
			"fnmadd.ps f27, f6, f17, f3, rmm\n"
			"fmin.pi f6, f6, f6\n"
			"fsrai.pi f27, f3, 0xb\n"
			"fcmovm.ps f17, f21, f23\n"
			"bltu x31, x17, 1f\n"
			"1:\n"
			"fsrl.pi f31, f23, f1\n"
			"fandi.pi f23, f23, 0x1c7\n"
			"fmulhu.pi f24, f24, f30\n"
			"fmin.pi f17, f6, f17\n"
			"fadd.pi f5, f6, f1\n"
			"fadd.ps f21, f24, f21, rtz\n"
			"fsgnjx.ps f31, f23, f6\n"
			"fmadd.s f31, f30, f23, f30, rne\n"
			"flt.ps f6, f30, f3\n"
			"fmul.s f21, f23, f23, rne\n"
			"fcmov.ps f5, f17, f23, f30\n"
			"fmin.s f17, f1, f24\n"
			"bge x0, x25, 1f\n"
			"1:\n"
			"fxor.pi f17, f21, f17\n"
			"fclass.ps f17, f24\n"
			"fcvt.f16.ps f27, f17\n"
			"fmulhu.pi f3, f3, f24\n"
			"fmsub.ps f5, f6, f24, f30, rtz\n"
			"fcmov.ps f27, f17, f24, f21\n"
			"fmulh.pi f27, f6, f23\n"
			"bge x12, x7, 1f\n"
			"1:\n"
			"fsgnj.ps f30, f6, f30\n"
			"fmin.pi f24, f23, f24\n"
			"fslli.pi f5, f24, 0xd\n"
			"fsgnjx.s f1, f1, f6\n"
			"fnmadd.ps f30, f30, f21, f6, rtz\n"
			"fsra.pi f3, f24, f21\n"
			"bltu x16, x27, 1f\n"
			"1:\n"
			"fminu.pi f1, f1, f21\n"
			"bltu x29, x30, 1f\n"
			"1:\n"
			"fcvt.ps.pw f31, f17, rdn\n"
			"fexp.ps f17, f1\n"
			"fcvt.ps.pw f5, f17, rne\n"
			"fswizz.ps f7, f1, 0x2\n"
			"fandi.pi f3, f3, 0xaa\n"
			"fmul.pi f24, f3, f24\n"
			"fsgnjx.ps f1, f1, f3\n"
			"fsgnj.ps f1, f6, f1\n"
			"beq x8, x8, 1f\n"
			"1:\n"
			"frcp.ps f1, f1\n"
			"fmax.ps f27, f6, f23\n"
			"fmadd.ps f3, f21, f24, f3, rup\n"
			"beq x15, x7, 1f\n"
			"1:\n"
			"flt.ps f17, f1, f21\n"
			"ffrc.ps f3, f24\n"
			"fmadd.ps f24, f23, f24, f17, rup\n"
			"fle.ps f5, f3, f6\n"
			"flt.ps f7, f3, f1\n"
			"fsrl.pi f5, f6, f3\n"
			"flt.pi f21, f30, f6\n"
			"fnmsub.ps f31, f1, f30, f1, rdn\n"
			"bgeu x6, x16, 1f\n"
			"1:\n"
			"fmulh.pi f27, f21, f6\n"
			"fmadd.ps f1, f23, f17, f6, rmm\n"
			"fcvt.pwu.ps f23, f21, rmm\n"
			"bne x0, x20, 1f\n"
			"1:\n"
			"fmax.pi f3, f3, f3\n"
			"blt x0, x15, 1f\n"
			"1:\n"
			"fslli.pi f24, f17, 0x7\n"
			"fslli.pi f3, f24, 0x7\n"
			"fmul.pi f27, f30, f30\n"
			"bne x25, x29, 1f\n"
			"1:\n"
			"fcvt.f16.ps f21, f3\n"
			"fsrai.pi f23, f23, 0xb\n"
			"fmin.ps f1, f17, f1\n"
			"fsin.ps f21, f30\n"
			"fcvt.ps.pw f21, f23, rtz\n"
			"fminu.pi f3, f21, f17\n"
			"fswizz.ps f3, f17, 0x1d\n"
			"fsra.pi f21, f21, f6\n"
			"beq x7, x21, 1f\n"
			"1:\n"
			"faddi.pi f5, f6, 0x1c3\n"
			"fmin.s f21, f6, f23\n"
			"beq x6, x22, 1f\n"
			"1:\n"
			"fadd.pi f6, f23, f30\n"
			"fnmadd.s f3, f3, f24, f23, rdn\n"
			"fle.pi f23, f23, f17\n"
			"fclass.ps f7, f6\n"
			"fexp.ps f27, f24\n"
			"fcvt.ps.f16 f3, f21\n"
			"fxor.pi f24, f24, f23\n"
			"fpackrepb.pi f27, f1\n"
			"fcvt.f16.ps f3, f3\n"
			"bne x26, x21, 1f\n"
			"1:\n"
			"fnmadd.s f21, f21, f3, f23, rdn\n"
			"fpackrepb.pi f21, f30\n"
			"fcvt.ps.f16 f1, f23\n"
			"fmadd.s f3, f3, f24, f24, rup\n"
			"fsrai.pi f1, f17, 0xd\n"
			"fsgnjn.ps f6, f30, f6\n"
			"fand.pi f24, f24, f23\n"
			"fmulh.pi f1, f1, f6\n"
			"bgeu x22, x21, 1f\n"
			"1:\n"
			"fcmov.ps f23, f23, f21, f6\n"
			"fcvt.ps.pw f27, f30, rmm\n"
			"blt x13, x9, 1f\n"
			"1:\n"
			"faddi.pi f7, f23, 0x1d2\n"
			"fsub.pi f27, f24, f17\n"
			"fmadd.s f5, f24, f24, f3, rdn\n"
			"bge x9, x26, 1f\n"
			"1:\n"
			"fcmov.ps f17, f17, f17, f21\n"
			"feq.ps f17, f30, f30\n"
			"faddi.pi f3, f6, 0x138\n"
			"flt.pi f24, f23, f24\n"
			"faddi.pi f27, f3, 0x1e8\n"
			"beq x18, x25, 1f\n"
			"1:\n"
			"fmadd.s f21, f1, f1, f23, rmm\n"
			"fand.pi f21, f1, f17\n"
			"frcp.ps f17, f30\n"
			"bge x25, x4, 1f\n"
			"1:\n"
			"fmax.s f7, f3, f1\n"
			"fsrai.pi f23, f24, 0xe\n"
			"fmax.pi f7, f21, f6\n"
			"fsgnjx.s f17, f23, f17\n"
			"blt x21, x9, 1f\n"
			"1:\n"
			"fadd.pi f5, f6, f3\n"
			"fmadd.s f27, f21, f23, f1, rup\n"
			"fmax.pi f17, f30, f1\n"
			"fmin.pi f17, f3, f21\n"
			"fpackreph.pi f31, f23\n"
			"ffrc.ps f31, f1\n"
			"fsrli.pi f21, f21, 0xd\n"
			"fsgnj.ps f30, f1, f3\n"
			"fadd.pi f1, f21, f6\n"
			"fminu.pi f5, f23, f17\n"
			"fmul.s f21, f17, f24, rdn\n"
			"fxor.pi f3, f23, f23\n"
			"fadd.s f3, f3, f21, rtz\n"
			"flt.ps f17, f1, f3\n"
			"bne x28, x24, 1f\n"
			"1:\n"
			"fsgnj.ps f7, f30, f17\n"
			"bne x26, x12, 1f\n"
			"1:\n"
			"fsatu8.pi f24, f24\n"
			"feq.ps f7, f24, f23\n"
			"fcmov.ps f24, f6, f24, f6\n"
			"fsgnjx.ps f17, f30, f1\n"
			"for.pi f5, f21, f21\n"
			"fsgnjn.s f30, f30, f23\n"
			"beq x20, x26, 1f\n"
			"1:\n"
			"fmadd.ps f5, f1, f6, f17, rdn\n"
			"fmadd.s f5, f3, f6, f3, rmm\n"
			"bge x22, x19, 1f\n"
			"1:\n"
			"fmin.s f5, f23, f17\n"
			"blt x1, x25, 1f\n"
			"1:\n"
			"fexp.ps f31, f6\n"
			"beq x27, x30, 1f\n"
			"1:\n"
			"fmax.pi f17, f6, f6\n"
			"fminu.pi f1, f1, f23\n"
			"fsgnjx.s f7, f23, f3\n"
			"feq.ps f21, f21, f17\n"
			"fadd.ps f30, f17, f30, rne\n"
			"fmadd.s f5, f1, f1, f17, rmm\n"
			"fle.pi f3, f3, f21\n"
			"fmul.pi f7, f24, f3\n"
			"fsgnj.s f17, f1, f17\n"
			"fsgnjx.ps f6, f6, f6\n"
			"feq.pi f31, f3, f23\n"
			"for.pi f5, f1, f3\n"
			"fnmsub.ps f17, f17, f17, f1, rtz\n"
			"fcvt.ps.pwu f17, f30, rmm\n"
			"fltu.pi f24, f30, f24\n"
			"fadd.s f23, f3, f23, rtz\n"
			"fmul.ps f5, f3, f21, rup\n"
			"fsub.ps f31, f24, f1, rup\n"
			"fsrai.pi f3, f21, 0x3\n"
			"fmulhu.pi f27, f21, f1\n"
			"fmul.pi f21, f6, f24\n"
			"fand.pi f21, f6, f3\n"
			"fand.pi f17, f1, f17\n"
			"fmax.pi f21, f24, f3\n"
			"bge x24, x10, 1f\n"
			"1:\n"
			"fand.pi f27, f24, f6\n"
			"fltu.pi f27, f21, f1\n"
			"fsgnj.s f1, f3, f3\n"
			"fmadd.ps f30, f6, f30, f21, rdn\n"
			"fand.pi f30, f30, f24\n"
			"fadd.s f24, f24, f6, rdn\n"
			"fmax.ps f27, f6, f23\n"
			"frsq.ps f24, f6\n"
			"bne x8, x10, 1f\n"
			"1:\n"
			"fmadd.ps f23, f24, f23, f17, rmm\n"
			"beq x0, x31, 1f\n"
			"1:\n"
			"fpackreph.pi f7, f1\n"
			"fsub.ps f31, f3, f6, rup\n"
			"addi x21, x21, -1\n"
			"bne x21, x0, LBL_SEQID_2_LOOP_SEQ_HID_0\n"
			"li x5, 3\n" 
			"bne x30, x5, LBL_FAIL_HID_0\n"
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f24,  224 (x5)\n"
			"flw.ps f10,  480 (x5)\n"
			"flw.ps f30,  448 (x5)\n"
			"flw.ps f13,  192 (x5)\n"
			"flw.ps f17,  64 (x5)\n"
			"flw.ps f25,  0 (x5)\n"
			"flw.ps f8,  128 (x5)\n"
			"flw.ps f20,  416 (x5)\n"
			"flw.ps f23,  672 (x5)\n"
			"flw.ps f16,  96 (x5)\n"
			"flw.ps f15,  736 (x5)\n"
			"flw.ps f22,  544 (x5)\n"
			"flw.ps f5,  704 (x5)\n"
			"flw.ps f6,  832 (x5)\n"
			"flw.ps f31,  384 (x5)\n"
			"flw.ps f18,  256 (x5)\n"
			"flw.ps f9,  608 (x5)\n"
			"flw.ps f0,  576 (x5)\n"
			"flw.ps f12,  512 (x5)\n"
			"flw.ps f2,  992 (x5)\n"
			"flw.ps f27,  288 (x5)\n"
			"flw.ps f29,  160 (x5)\n"
			"flw.ps f26,  352 (x5)\n"
			"flw.ps f19,  864 (x5)\n"
			"flw.ps f4,  768 (x5)\n"
			"flw.ps f1,  800 (x5)\n"
			"flw.ps f11,  896 (x5)\n"
			"flw.ps f7,  320 (x5)\n"
			"flw.ps f14,  32 (x5)\n"
			"flw.ps f21,  640 (x5)\n"
			"flw.ps f28,  928 (x5)\n"
			"flw.ps f3,  960 (x5)\n"
			"addi x30, x30, 1\n"
			"la x6, rand_int_32\n"
			"flw.ps f26, 96(x6)\n"
			"flw.ps f1, 320(x6)\n"
			"fmaxu.pi f5, f26, f1\n" 
			"fadd.pi f31, f30, f31\n"
			"fsrl.pi f14, f21, f20\n"
			"fpackreph.pi f7, f7\n"
			"frcp.ps f21, f23\n"
			"div x20, x28, x20\n"
			"fnmsub.ps f7, f1, f14, f21, rmm\n"
			"div x28, x23, x26\n"
			"la x23, rand_int_32\n"
			"flw.ps f31, 0(x23)\n"
			"fsat8.pi f7, f31\n" 
			"sraw x26, x0, x20\n"
			"fcvt.f16.ps f12, f17\n"
			"fmvz.x.ps x7, f6, 5\n"
			"fslli.pi f23, f7, 0x3\n"
			"fclass.s x11, f20\n"
			"bne x9, x0, 1f\n"
			"1:\n"
			"slt x23, x23, x0\n"
			"fmaxu.pi f31, f5, f5\n"
			"la x23, rand_int_32\n"
			"flw.ps f30, 32(x23)\n"
			"fle.pi f27, f30, f30\n" 
			"fsat8.pi f20, f20\n"
			"fsub.pi f31, f31, f31\n"
			"fsgnjn.ps f6, f7, f3\n"
			"fandi.pi f31, f31, 0x9\n"
			"bltu x1, x7, 1f\n"
			"1:\n"
			"fandi.pi f5, f3, 0x1c1\n"
			"fand.pi f20, f20, f20\n"
			"fsin.ps f31, f12\n"
			"la x6, rand_ieee754_32\n"
			"flw.ps f24, 512(x6)\n"
			"flw.ps f30, 896(x6)\n"
			"fnmsub.ps f20, f24, f30, f24, rup\n" 
			"fmax.ps f30, f31, f31\n"
			"xor x0, x7, x26\n"
			"divuw x26, x26, x23\n"
			"bltu x18, x18, 1f\n"
			"1:\n"
			"fmaxu.pi f26, f20, f31\n"
			"fsin.ps f21, f20\n"
			"or x23, x23, x26\n"
			"fnmadd.ps f21, f1, f24, f31, rmm\n"
			"la x7, rand_ieee754_32\n"
			"flw.ps f30, 448(x7)\n"
			"fcvt.f16.ps f14, f30\n" 
			"fsgnjn.ps f24, f6, f26\n"
			"fsub.pi f27, f24, f26\n"
			"or x20, x26, x26\n"
			"div x7, x9, x6\n"
			"beq x14, x10, 1f\n"
			"1:\n"
			"fsub.pi f23, f1, f1\n"
			"div x23, x7, x6\n"
			"bgeu x20, x27, 1f\n"
			"1:\n"
			"remu x12, x6, x9\n"
			"la x28, rand_ieee754_32\n"
			"flw.ps f20, 640(x28)\n"
			"fcvt.wu.s x28, f20, rmm\n" 
			"fandi.pi f26, f3, 0x2f\n"
			"bne x15, x10, 1f\n"
			"1:\n"
			"mova.m.x x28\n"
			"fsat8.pi f23, f6\n"
			"sll x12, x11, x12\n"
			"fmul.pi f7, f14, f30\n"
			"fround.ps f3, f26, rne\n"
			"bne x29, x30, 1f\n"
			"1:\n"
			"slt x23, x11, x7\n"
			"la x9, rand_ieee754_32\n"
			"flw.ps f30, 704(x9)\n"
			"fclass.s x28, f30\n" 
			"slt x20, x0, x20\n"
			"mov.m.x m4, x11, 0x7d\n"
			"slli x23, x7, 2\n"
			"fmin.pi f26, f30, f27\n"
			"divuw x6, x28, x26\n"
			"frcp.ps f30, f30\n"
			"fnmsub.ps f27, f3, f5, f5, rdn\n"
			"blt x30, x15, 1f\n"
			"1:\n"
			"li x7, 0xf4562235390d2ebe\n"
			"li x11, 0x2b55c36f2a6bec9a\n"
			"addw x17, x7, x11\n" 
			"slt x23, x12, x23\n"
			"srlw x12, x11, x9\n"
			"mulw x20, x17, x23\n"
			"flt.s x9, f27, f20\n"
			"blt x13, x11, 1f\n"
			"1:\n"
			"feq.pi f21, f21, f14\n"
			"sllw x6, x23, x6\n"
			"fcvt.pwu.ps f14, f14, rne\n"
			"la x6, rand_ieee754_32\n"
			"flw.ps f30, 320(x6)\n"
			"flw.ps f6, 832(x6)\n"
			"fmadd.ps f30, f30, f6, f30, rmm\n" 
			"srli x7, x7, 2\n"
			"addw x23, x23, x23\n"
			"fnmadd.ps f1, f3, f1, f24, rne\n"
			"fmul.ps f27, f17, f1, rne\n"
			"slt x28, x7, x26\n"
			"fcvt.pwu.ps f7, f31, rne\n"
			"fnmadd.ps f17, f17, f5, f27, rne\n"
			"bne x16, x10, 1f\n"
			"1:\n"
			"li x28, 0x6fcff4dcb340716f\n"
			"li x23, 0x45b00698b02aa2a\n"
			"slt x28, x28, x23\n" 
			"fsin.ps f26, f27\n"
			"fround.ps f23, f12, rtz\n"
			"fadd.pi f17, f1, f17\n"
			"fround.ps f3, f6, rdn\n"
			"fsin.ps f24, f1\n"
			"bgeu x5, x2, 1f\n"
			"1:\n"
			"fsin.ps f26, f1\n"
			"fcvt.pwu.ps f30, f24, rne\n"
			"la x17, rand_ieee754_32\n"
			"flw.ps f12, 576(x17)\n"
			"flw.ps f5, 480(x17)\n"
			"flw.ps f7, 352(x17)\n"
			"fnmsub.ps f31, f12, f5, f7, rne\n" 
			"mulw x11, x7, x26\n"
			"fmax.ps f21, f6, f6\n"
			"fslli.pi f3, f30, 0x9\n"
			"mulhsu x0, x23, x0\n"
			"bne x29, x8, 1f\n"
			"1:\n"
			"fmax.ps f24, f26, f6\n"
			"fslli.pi f31, f31, 0x4\n"
			"fle.ps f27, f5, f26\n"
			"bge x4, x23, 1f\n"
			"1:\n"
			"la x26, rand_int_32\n"
			"flw.ps f17, 96(x26)\n"
			"flw.ps f12, 0(x26)\n"
			"fand.pi f1, f17, f12\n" 
			"fmul.s f30, f14, f20, rmm\n"
			"blt x16, x8, 1f\n"
			"1:\n"
			"remuw x9, x9, x12\n"
			"fnot.pi f27, f14\n"
			"frcp.ps f7, f12\n"
			"slt x17, x11, x17\n"
			"bne x7, x18, 1f\n"
			"1:\n"
			"sub x17, x17, x12\n"
			"fand.pi f27, f31, f14\n"
			"la x11, rand_int_32\n"
			"flw.ps f21, 896(x11)\n"
			"flw.ps f23, 576(x11)\n"
			"fmaxu.pi f6, f21, f23\n" 
			"fmul.ps f23, f21, f24, rne\n"
			"fle.pi f23, f31, f23\n"
			"fsub.s f21, f23, f20, rdn\n"
			"fmin.ps f24, f24, f20\n"
			"xor x28, x6, x28\n"
			"sll x23, x9, x11\n"
			"fle.pi f21, f31, f21\n"
			"la x7, rand_ieee754_32\n"
			"flw.ps f12, 928(x7)\n"
			"fclass.s x7, f12\n" 
			"sub x7, x20, x9\n"
			"bge x28, x31, 1f\n"
			"1:\n"
			"flt.ps f30, f1, f1\n"
			"slt x26, x0, x28\n"
			"srlw x7, x20, x0\n"
			"bge x20, x4, 1f\n"
			"1:\n"
			"divuw x9, x9, x9\n"
			"fsrl.pi f1, f30, f7\n"
			"fmadd.s f1, f1, f1, f7, rmm\n"
			"beq x15, x8, 1f\n"
			"1:\n"
			"la x28, rand_ieee754_32\n"
			"flw.ps f27, 544(x28)\n"
			"flw.ps f17, 800(x28)\n"
			"flt.ps f30, f27, f17\n" 
			"fcvt.pw.ps f30, f1, rtz\n"
			"fmadd.s f1, f3, f26, f1, rdn\n"
			"fslli.pi f1, f27, 0x7\n"
			"div x7, x26, x26\n"
			"beq x6, x27, 1f\n"
			"1:\n"
			"addw x0, x0, x23\n"
			"bge x23, x9, 1f\n"
			"1:\n"
			"fmaxu.pi f27, f23, f3\n"
			"bltu x1, x25, 1f\n"
			"1:\n"
			"maskpopcz x23, m4\n"
			"la x28, rand_int_32\n"
			"flw.ps f3, 864(x28)\n"
			"fslli.pi f30, f3, 0x1\n" 
			"fsll.pi f31, f26, f21\n"
			"fsub.pi f6, f7, f3\n"
			"bgeu x14, x26, 1f\n"
			"1:\n"
			"feq.pi f5, f17, f6\n"
			"fsll.pi f21, f21, f21\n"
			"fmadd.s f5, f26, f27, f5, rmm\n"
			"fmulhu.pi f21, f3, f26\n"
			"fpackreph.pi f21, f27\n"
			"LBL_SEQID_3_M0_WRITE_ID_66_HID_0:\n"
			"feqm.ps m4, f12, f6\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"fround.ps f23, f3, rup\n"
			"slliw x28, x28, 4\n"
			"sub x23, x9, x28\n"
			"frcp.ps f12, f7\n"
			"slliw x6, x6, 5\n"
			"frcp.ps f21, f27\n"
			"fand.pi f5, f5, f7\n"
			"la x7, rand_ieee754_32\n"
			"flw.ps f21, 224(x7)\n"
			"flw.ps f1, 96(x7)\n"
			"fsub.s f23, f21, f1, rtz\n" 
			"fsll.pi f24, f12, f26\n"
			"fmaxu.pi f1, f12, f17\n"
			"fmulhu.pi f26, f26, f21\n"
			"fsub.s f31, f5, f24, rne\n"
			"fsll.pi f21, f21, f21\n"
			"blt x8, x18, 1f\n"
			"1:\n"
			"fsll.pi f6, f17, f31\n"
			"fmax.ps f12, f26, f12\n"
			"la x17, rand_int_32\n"
			"flw.ps f30, 672(x17)\n"
			"flw.ps f6, 576(x17)\n"
			"fsub.pi f1, f30, f6\n" 
			"fltu.pi f21, f23, f31\n"
			"remu x17, x12, x28\n"
			"sll x6, x0, x23\n"
			"add x17, x7, x7\n"
			"bgeu x27, x11, 1f\n"
			"1:\n"
			"fle.ps f26, f26, f20\n"
			"mulw x7, x20, x26\n"
			"remu x7, x7, x20\n"
			"li x9, 0xe2d75e2ee92bf101\n"
			"li x17, 0xa07d441cb63326da\n"
			"mulh x28, x9, x17\n" 
			"fmaxu.pi f21, f14, f26\n"
			"fmaxu.pi f26, f26, f20\n"
			"srl x0, x7, x17\n"
			"slt x7, x7, x20\n"
			"fle.pi f14, f3, f20\n"
			"fsin.ps f30, f21\n"
			"fmul.s f21, f21, f24, rtz\n"
			"li x9, 0x3f03c23fcddb856b\n"
			"sltiu x26, x9, 0x18\n" 
			"frcp.ps f20, f20\n"
			"flt.ps f17, f12, f31\n"
			"fmul.pi f30, f20, f30\n"
			"fsll.pi f31, f12, f31\n"
			"bltu x18, x23, 1f\n"
			"1:\n"
			"fsll.pi f6, f31, f30\n"
			"feq.pi f3, f31, f3\n"
			"fcvt.pw.ps f3, f23, rne\n"
			"bltu x15, x17, 1f\n"
			"1:\n"
			"li x6, 0x2c508a7400f165c3\n"
			"li x26, 0xae8403c3ebdf895b\n"
			"sraw x11, x6, x26\n" 
			"frcp.ps f6, f17\n"
			"fmul.pi f6, f26, f23\n"
			"bne x23, x14, 1f\n"
			"1:\n"
			"fmul.pi f27, f27, f27\n"
			"fadd.pi f17, f12, f27\n"
			"fltu.pi f24, f27, f14\n"
			"fmul.ps f27, f24, f7, rup\n"
			"fmin.ps f6, f27, f27\n"
			"bge x0, x10, 1f\n"
			"1:\n"
			"li x17, 0x5161d5d5e5362235\n"
			"li x23, 0xe0438f033fc49e96\n"
			"sraw x26, x17, x23\n" 
			"fand.pi f27, f30, f20\n"
			"fmin.pi f7, f26, f27\n"
			"fcvt.f16.ps f1, f21\n"
			"feqm.ps m7, f26, f12\n"
			"bgeu x11, x1, 1f\n"
			"1:\n"
			"slli x20, x9, 2\n"
			"packb x11, x12, x6\n"
			"bge x5, x29, 1f\n"
			"1:\n"
			"fsll.pi f7, f5, f24\n"
			"li x20, 0x532933cfb853599e\n"
			"li x11, 0xd9d91c909bd62444\n"
			"mulw x9, x20, x11\n" 
			"fsgnjn.ps f26, f3, f3\n"
			"fsub.pi f17, f17, f7\n"
			"fpackreph.pi f20, f24\n"
			"fnmadd.ps f24, f26, f3, f20, rtz\n"
			"fround.ps f1, f7, rtz\n"
			"fmin.ps f3, f12, f26\n"
			"fand.pi f20, f24, f31\n"
			"bltu x29, x7, 1f\n"
			"1:\n"
			"li x17, 0xaecad71bdaa66273\n"
			"li x7, 0xdf778b9859551cd5\n"
			"sll x23, x17, x7\n" 
			"flt.ps f3, f30, f3\n"
			"fmax.pi f12, f6, f12\n"
			"fmaxu.pi f12, f5, f1\n"
			"fsgnjn.ps f20, f21, f7\n"
			"bltu x5, x7, 1f\n"
			"1:\n"
			"add x17, x20, x26\n"
			"bgeu x3, x29, 1f\n"
			"1:\n"
			"fsrl.pi f3, f21, f27\n"
			"flem.ps m2, f21, f20\n"
			"la x6, rand_int_32\n"
			"flw.ps f30, 576(x6)\n"
			"flw.ps f21, 128(x6)\n"
			"fmul.pi f6, f30, f21\n" 
			"fmaxu.pi f14, f5, f6\n"
			"fcvt.f16.ps f5, f27\n"
			"fsll.pi f12, f21, f27\n"
			"fmin.ps f17, f20, f1\n"
			"bltu x25, x2, 1f\n"
			"1:\n"
			"fmadd.s f30, f21, f17, f5, rup\n"
			"bgeu x31, x3, 1f\n"
			"1:\n"
			"fmin.pi f30, f6, f20\n"
			"fcvt.pw.ps f24, f24, rdn\n"
			"bgeu x24, x4, 1f\n"
			"1:\n"
			"li x23, 0xb4908bee1e434736\n"
			"packb x7, x23, x23\n" 
			"fslli.pi f24, f24, 0x3\n"
			"fslli.pi f14, f12, 0xb\n"
			"fmin.ps f24, f26, f27\n"
			"fround.ps f26, f23, rmm\n"
			"fle.pi f14, f12, f20\n"
			"fle.pi f17, f1, f31\n"
			"fle.pi f26, f30, f26\n"
			"la x6, rand_int_32\n"
			"flw.ps f6, 384(x6)\n"
			"fandi.pi f14, f6, 0x11a\n" 
			"fsat8.pi f21, f21\n"
			"fltu.pi f31, f31, f17\n"
			"fsub.s f23, f20, f12, rne\n"
			"fmadd.ps f20, f23, f23, f20, rtz\n"
			"fsat8.pi f26, f23\n"
			"fround.ps f23, f14, rmm\n"
			"fmax.ps f26, f12, f30\n"
			"la x26, rand_ieee754_32\n"
			"flw.ps f30, 672(x26)\n"
			"flw.ps f1, 512(x26)\n"
			"flw.ps f26, 960(x26)\n"
			"fnmadd.ps f12, f30, f1, f26, rmm\n" 
			"fmadd.s f3, f30, f21, f12, rmm\n"
			"bltu x0, x24, 1f\n"
			"1:\n"
			"fmul.ps f3, f17, f12, rmm\n"
			"beq x12, x31, 1f\n"
			"1:\n"
			"fmulhu.pi f12, f27, f31\n"
			"fand.pi f3, f30, f24\n"
			"fsub.pi f27, f17, f31\n"
			"fcvt.pwu.ps f27, f21, rtz\n"
			"fandi.pi f12, f14, 0x16d\n"
			"li x20, 0x96eb5c2aca693951\n"
			"li x17, 0xd67a6e07693c53b3\n"
			"remuw x28, x20, x17\n" 
			"fbci.ps f1, 0x79027\n"
			"beq x22, x24, 1f\n"
			"1:\n"
			"fmul.s f27, f5, f27, rtz\n"
			"fmv.s.x f7, x17\n"
			"fcvt.f16.ps f12, f20\n"
			"fand.pi f7, f7, f7\n"
			"fandi.pi f23, f27, 0x18e\n"
			"fnmsub.ps f12, f12, f14, f1, rne\n"
			"LBL_SEQID_3_M0_WRITE_ID_67_HID_0:\n"
			"fltm.pi m2, f17, f5\n" 
			"li x5, 0x803400C8\n"
			"ld x5, 0(x5)\n"
			"fmin.ps f26, f23, f6\n"
			"bne x14, x13, 1f\n"
			"1:\n"
			"mov.m.x m7, x26, 0x4\n"
			"bgeu x10, x16, 1f\n"
			"1:\n"
			"fmvz.x.ps x0, f30, 4\n"
			"fmv.s.x f1, x28\n"
			"fcvt.pw.ps f24, f30, rtz\n"
			"fandi.pi f1, f7, 0x61\n"
			"auipc x12, 0x17\n"
			"la x6, rand_int_32\n"
			"flw.ps f27, 864(x6)\n"
			"fslli.pi f30, f27, 0xe\n" 
			"fslli.pi f14, f1, 0x9\n"
			"slli x17, x28, 0\n"
			"add x6, x17, x6\n"
			"fmul.s f6, f30, f12, rmm\n"
			"sll x17, x7, x6\n"
			"bltu x3, x25, 1f\n"
			"1:\n"
			"mulh x17, x0, x28\n"
			"beq x29, x7, 1f\n"
			"1:\n"
			"addw x26, x17, x11\n"
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
			"flt.ps f7, f7, f1\n"
			"srlw x23, x28, x11\n"
			"sub x6, x12, x28\n"
			"bge x12, x7, 1f\n"
			"1:\n"
			"flem.ps m2, f14, f23\n"
			"fsat8.pi f7, f23\n"
			"auipc x17, 0x13\n"
			"fclass.s x12, f23\n"
			"bltu x28, x20, 1f\n"
			"1:\n"
			"add x0, x11, x12\n"
			"fmadd.ps f26, f26, f23, f23, rmm\n"
			"bge x17, x6, 1f\n"
			"1:\n"
			"fmin.pi f6, f3, f14\n"
			"bltu x23, x16, 1f\n"
			"1:\n"
			"fsub.s f26, f26, f26, rup\n"
			"fsub.pi f24, f27, f27\n"
			"bltu x0, x18, 1f\n"
			"1:\n"
			"fnmsub.ps f3, f14, f26, f27, rmm\n"
			"fmax.pi f6, f5, f27\n"
			"fle.pi f27, f26, f7\n"
			"fmax.pi f26, f26, f31\n"
			"frcp.ps f24, f14\n"
			"LBL_SEQID_3_XMA_QNT_STR_SEQ_ITER_0_HID_0:\n"
			"li x6, ((0x15 << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0x4 << TENSOR_QUANT_QUANT_ROW) | (0x28 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x0 << TENSOR_QUANT_TRANSF1) | (0x8 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_3_XMA_QNT_STR_SEQ_ITER_1_HID_0:\n"
			"li x6, ((0x1c << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0xb << TENSOR_QUANT_QUANT_ROW) | (0x1d << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x4 << TENSOR_QUANT_TRANSF4) | (0x4 << TENSOR_QUANT_TRANSF3) | (0x4 << TENSOR_QUANT_TRANSF2) | (0x9 << TENSOR_QUANT_TRANSF1) | (0x4 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_3_XMA_QNT_STR_SEQ_ITER_2_HID_0:\n"
			"li x6, ((0x2 << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0x8 << TENSOR_QUANT_QUANT_ROW) | (0x16 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x1 << TENSOR_QUANT_TRANSF3) | (0x9 << TENSOR_QUANT_TRANSF2) | (0xa << TENSOR_QUANT_TRANSF1) | (0x4 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"li x5, 0x20\n"
			"la x31, rand_ieee754_32\n"
			"add x31, x31, x5\n"
			"flw.ps f1, 0(x31)\n"
			"flw.ps f12, 0(x31)\n"
			"flw.ps f14, 0(x31)\n"
			"flw.ps f17, 8(x31)\n"
			"flw.ps f20, 7(x31)\n"
			"flw.ps f21, 10(x31)\n"
			"flw.ps f23, 10(x31)\n"
			"flw.ps f24, 2(x31)\n"
			"flw.ps f26, 11(x31)\n"
			"flw.ps f27, 23(x31)\n"
			"flw.ps f3, 23(x31)\n"
			"flw.ps f30, 30(x31)\n"
			"flw.ps f31, 46(x31)\n"
			"flw.ps f5, 40(x31)\n"
			"flw.ps f6, 42(x31)\n"
			"flw.ps f7, 14(x31)\n"
			"la x31, rand_int_32\n"
			"add x31, x31, x5\n"
			"lw x0, 0(x31)\n"
			"lw x11, 1(x31)\n"
			"lw x12, 4(x31)\n"
			"lw x17, 12(x31)\n"
			"lw x20, 6(x31)\n"
			"lw x23, 16(x31)\n"
			"lw x26, 2(x31)\n"
			"lw x28, 17(x31)\n"
			"lw x6, 21(x31)\n"
			"lw x7, 26(x31)\n"
			"lw x9, 18(x31)\n"
			"li x21, 0x1\n"
			"LBL_SEQID_3_LOOP_SEQ_HID_0:\n"
			"fmadd.s f14, f21, f21, f27, rup\n"
			"bgeu x31, x28, 1f\n"
			"1:\n"
			"addw x0, x11, x17\n"
			"fmax.pi f30, f30, f21\n"
			"ecall\n"
			"bne x14, x2, 1f\n"
			"1:\n"
			"fbci.ps f20, 0x506c\n"
			"fround.ps f7, f7, rdn\n"
			"fmax.pi f1, f1, f24\n"
			"flem.ps m4, f27, f7\n"
			"flt.s x23, f17, f3\n"
			"srlw x11, x17, x17\n"
			"fcvt.f16.ps f6, f24\n"
			"fmadd.ps f6, f21, f24, f7, rne\n"
			"fltu.pi f26, f26, f6\n"
			"fsll.pi f21, f14, f14\n"
			"fnmsub.ps f1, f30, f17, f24, rne\n"
			"bge x20, x0, 1f\n"
			"1:\n"
			"maskpopcz x28, m6\n"
			"ecall\n"
			"fpackreph.pi f14, f6\n"
			"beq x27, x2, 1f\n"
			"1:\n"
			"flt.s x23, f23, f3\n"
			"mulw x11, x17, x11\n"
			"lui x12, 0x7\n"
			"blt x2, x24, 1f\n"
			"1:\n"
			"divuw x17, x11, x26\n"
			"fsgnjn.ps f6, f1, f1\n"
			"div x17, x23, x17\n"
			"mulhsu x17, x11, x6\n"
			"fcvt.f16.ps f31, f12\n"
			"blt x22, x21, 1f\n"
			"1:\n"
			"sll x26, x0, x26\n"
			"mulh x17, x20, x9\n"
			"fnot.pi f6, f30\n"
			"fcvt.pwu.ps f14, f12, rmm\n"
			"fsat8.pi f7, f3\n"
			"blt x12, x10, 1f\n"
			"1:\n"
			"sll x9, x23, x28\n"
			"flt.ps f20, f23, f20\n"
			"fmulhu.pi f1, f5, f3\n"
			"bge x11, x13, 1f\n"
			"1:\n"
			"fnmadd.ps f30, f26, f12, f14, rmm\n"
			"fcvt.pwu.ps f20, f21, rmm\n"
			"blt x28, x17, 1f\n"
			"1:\n"
			"fsat8.pi f17, f27\n"
			"fnmadd.ps f23, f3, f23, f7, rmm\n"
			"flt.ps f27, f1, f12\n"
			"fmin.pi f26, f14, f14\n"
			"beq x19, x17, 1f\n"
			"1:\n"
			"auipc x28, 0x11\n"
			"fsub.s f24, f31, f31, rmm\n"
			"fle.pi f7, f7, f17\n"
			"sll x11, x20, x11\n"
			"srli x17, x23, 2\n"
			"add x11, x17, x6\n"
			"flt.s x28, f20, f31\n"
			"mov.m.x m2, x7, 0xe\n"
			"bne x19, x26, 1f\n"
			"1:\n"
			"fmul.ps f23, f17, f20, rtz\n"
			"fpackreph.pi f3, f3\n"
			"fsub.pi f26, f26, f7\n"
			"fmul.s f6, f17, f24, rdn\n"
			"fmul.ps f12, f20, f12, rdn\n"
			"fltm.pi m6, f1, f26\n"
			"fsub.s f26, f27, f17, rne\n"
			"fsgnjn.ps f24, f23, f24\n"
			"fle.pi f30, f26, f12\n"
			"srl x28, x28, x20\n"
			"feq.pi f26, f20, f26\n"
			"fnmadd.ps f23, f31, f23, f7, rtz\n"
			"bge x31, x31, 1f\n"
			"1:\n"
			"srl x7, x9, x0\n"
			"mulh x17, x7, x6\n"
			"srli x17, x20, 5\n"
			"srlw x0, x23, x0\n"
			"sraw x28, x0, x6\n"
			"fadd.pi f27, f5, f20\n"
			"maskpopcz x0, m2\n"
			"flt.s x11, f3, f27\n"
			"fsgnjn.ps f14, f14, f12\n"
			"fsgnjn.ps f14, f20, f5\n"
			"feqm.ps m2, f1, f21\n"
			"fandi.pi f21, f27, 0x1cb\n"
			"flt.s x26, f31, f27\n"
			"slli x0, x11, 1\n"
			"fmin.pi f26, f14, f26\n"
			"fslli.pi f20, f3, 0xe\n"
			"fround.ps f6, f6, rne\n"
			"flem.ps m6, f12, f31\n"
			"bne x4, x16, 1f\n"
			"1:\n"
			"fsll.pi f7, f7, f23\n"
			"mov.m.x m2, x23, 0x29\n"
			"fcvt.wu.s x6, f12, rmm\n"
			"fsat8.pi f23, f23\n"
			"bne x3, x15, 1f\n"
			"1:\n"
			"fcvt.f16.ps f3, f21\n"
			"flem.ps m2, f23, f6\n"
			"fmadd.s f21, f7, f27, f5, rtz\n"
			"fmul.pi f7, f12, f20\n"
			"fltm.pi m2, f5, f17\n"
			"fcvt.pw.ps f30, f30, rup\n"
			"bne x15, x1, 1f\n"
			"1:\n"
			"addw x28, x28, x0\n"
			"fslli.pi f3, f3, 0x3\n"
			"bgeu x20, x9, 1f\n"
			"1:\n"
			"auipc x0, 0xf\n"
			"beq x23, x22, 1f\n"
			"1:\n"
			"ecall\n"
			"fsub.pi f26, f24, f20\n"
			"sll x12, x9, x12\n"
			"fmadd.ps f17, f7, f1, f3, rmm\n"
			"fcvt.f16.ps f31, f31\n"
			"blt x7, x14, 1f\n"
			"1:\n"
			"fnmsub.ps f5, f31, f5, f26, rmm\n"
			"fcvt.f16.ps f17, f31\n"
			"bltu x17, x6, 1f\n"
			"1:\n"
			"fsat8.pi f20, f21\n"
			"srlw x26, x9, x9\n"
			"auipc x26, 0x10\n"
			"fmul.pi f21, f6, f17\n"
			"bne x24, x29, 1f\n"
			"1:\n"
			"fmadd.ps f1, f3, f27, f1, rup\n"
			"fmax.ps f3, f3, f3\n"
			"fsat8.pi f21, f1\n"
			"fmadd.ps f20, f17, f23, f23, rmm\n"
			"mulw x20, x7, x20\n"
			"mov.m.x m6, x12, 0x1a\n"
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
			"LBL_HPM_4_CORE_BRANCHES0_FAIL_HID_0:\n"          
			"csrr x5, mhpmcounter4\n"
			"sd x5, 40 (x31)\n"
			"LBL_HPM_5_CORE_TQUANT_INST_FAIL_HID_0:\n"          
			"csrr x5, mhpmcounter5\n"
			"sd x5, 48 (x31)\n"
			"LBL_HPM_6_CORE_TXFMA_OPS32_FAIL_HID_0:\n"          
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
			"LBL_HPM_3_CORE_TL_OPS_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter3\n"
			"sd x5, 32 (x31)\n"
			"LBL_HPM_4_CORE_BRANCHES0_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter4\n"
			"sd x5, 40 (x31)\n"
			"LBL_HPM_5_CORE_TQUANT_INST_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter5\n"
			"sd x5, 48 (x31)\n"
			"LBL_HPM_6_CORE_TXFMA_OPS32_PASS_HID_0:\n"          
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
		0x0c3745bd, 
		0xc63e359b, 
		0xae96f1a0, 
		0x98828a3b, 
		0x0679b809, 
		0x85f873df, 
		0x1e339698, 
		0x19c08330, 
		0x22ef1e29, 
		0x227aedf1, 
		0x3dcdf7be, 
		0x5d0bdfd6, 
		0xde6b4c4b, 
		0xf24e8711, 
		0xcf462765, 
		0x904e7a9d, 
		0x62899680, 
		0xd888fe09, 
		0x4e59caf1, 
		0x59341f72, 
		0x3d98f1c5, 
		0xdc7084d7, 
		0xdf98537b, 
		0x60c96f98, 
		0xe8664b8c, 
		0xde78203b, 
		0x95912bce, 
		0x71d648fa, 
		0xbf5a040a, 
		0x0a65754a, 
		0x0326e877, 
		0xc2cfdd88, 
		0x5db57bb4, 
		0x63939efd, 
		0xcd01693c, 
		0x7cb6b693, 
		0x77538abd, 
		0x191d7823, 
		0xa03d12f2, 
		0xd78ba743, 
		0x167a4e55, 
		0x4f0bf918, 
		0x5ebf0b15, 
		0xc33639e4, 
		0x83e7b4ce, 
		0x5dd0b1ae, 
		0xeec532d9, 
		0x6480dfc3, 
		0x93f39cad, 
		0x9f2bce5e, 
		0xbfe20254, 
		0x756e067a, 
		0x0815f58f, 
		0xfe74c6f9, 
		0xbfac91bf, 
		0xfc2f87f9, 
		0x58f263b9, 
		0xd5d781ba, 
		0xbb29afac, 
		0xe7baff17, 
		0xd6ece308, 
		0x18d68dcc, 
		0xff765ff8, 
		0x705a8436, 
		0x9e37bd77, 
		0x29a414ca, 
		0x8a3799bc, 
		0x7c7257ad, 
		0xf686c903, 
		0x97c50ec4, 
		0x5d0f5a94, 
		0xb4e3a734, 
		0xb2941b36, 
		0x5304a829, 
		0x7b251e7b, 
		0xe8f8f389, 
		0xf59f2370, 
		0xccef32d4, 
		0x5d9da1a1, 
		0xb7ac16b4, 
		0x55a64f9d, 
		0x98ac8f1e, 
		0xcea15d1b, 
		0x855b6276, 
		0xec22d882, 
		0x1ab9ba38, 
		0x96d12ca2, 
		0xd80539ab, 
		0x8bb688db, 
		0xcca6f045, 
		0xb5e8b6e6, 
		0x4407089d, 
		0x7f77913d, 
		0x83e616b9, 
		0xe8499a87, 
		0x890d648b, 
		0xbdc65ab9, 
		0x8a7ba04a, 
		0x04323517, 
		0x72d9d906, 
		0x748a73cc, 
		0x35f6b538, 
		0x9f92828f, 
		0xed518ec3, 
		0x750fb060, 
		0x8045b5f3, 
		0xba955904, 
		0x5aa52bdd, 
		0x04d908bc, 
		0x5d57e225, 
		0x5603f8c9, 
		0x8bafff21, 
		0xcbd58a0d, 
		0xfc6f0b45, 
		0xc31f252d, 
		0x4f967e9f, 
		0x73a3c3a2, 
		0xa2c6fbf3, 
		0x429cc328, 
		0x242818f7, 
		0x10ef7601, 
		0x5b02414c, 
		0xac924a07, 
		0x9d23af03, 
		0x1daca3e4, 
		0x50c173b5, 
		0x6dbff492, 
		0x90430f4a, 
		0x8a4ac820, 
		0xe60b92df, 
		0xabdf1b40, 
		0x28c3ce38, 
		0xd9e39b28, 
		0x520f1028, 
		0x29390187, 
		0x37f99256, 
		0x0fed7b35, 
		0x0be60ab0, 
		0x8bcf46ee, 
		0x5c631cf8, 
		0x80c18527, 
		0x29e17f8d, 
		0x49eb6506, 
		0x02193378, 
		0x7e604a1e, 
		0x5962c205, 
		0x4bc26250, 
		0x0dcc568c, 
		0xf981ae6b, 
		0xad63cff5, 
		0x7c68e7c6, 
		0x25ff65e8, 
		0x911cb81c, 
		0xd7a739c4, 
		0x4c3ffee4, 
		0xecfd3c38, 
		0x35489358, 
		0x65639cab, 
		0x63f7c8af, 
		0xa00debaf, 
		0x2430b771, 
		0x6ea2e7bf, 
		0xafd01b31, 
		0x6f52302f, 
		0x8e249e23, 
		0x4629f2e5, 
		0xedbd5bbd, 
		0xe5255c46, 
		0x11d1d004, 
		0xc62c018e, 
		0x507eef37, 
		0x04a4ebe2, 
		0x46938016, 
		0x144f8b8d, 
		0x7fb71265, 
		0x05b0a0c4, 
		0x8a7f6879, 
		0x32abbe8b, 
		0x8228528c, 
		0xa4335833, 
		0x9d825437, 
		0x2092af99, 
		0xa943509b, 
		0x93936887, 
		0x7b442629, 
		0x7813a0e4, 
		0x853d3960, 
		0x374e93a1, 
		0x20e470b7, 
		0x49a1edb7, 
		0x6a18a219, 
		0x5159d1e3, 
		0xaa1dab95, 
		0x231d1322, 
		0x3d162ddb, 
		0x52c7cd88, 
		0x8a4e1961, 
		0xa83b7100, 
		0xbbcaef56, 
		0x656caf8d, 
		0x4959ca5a, 
		0xf521b1c4, 
		0x3b63f248, 
		0xe029a4fa, 
		0x82d1025d, 
		0x12b00bc1, 
		0xc36d195e, 
		0x104bfd54, 
		0x651afd48, 
		0xf2dfa24d, 
		0x73b9cfdf, 
		0xdb6051ca, 
		0x7370d9e9, 
		0x1c6d00d1, 
		0xbbe3a47b, 
		0xfafe98f0, 
		0x077d8545, 
		0x2b09419e, 
		0x379fdb24, 
		0x13da6907, 
		0x21c26a68, 
		0x4c4e6918, 
		0x1739093a, 
		0x3edef140, 
		0x381f519c, 
		0x1d64804c, 
		0x24a4a277, 
		0xc98077da, 
		0x3a509c7b, 
		0xfabd720f, 
		0x1c50d955, 
		0x673cc836, 
		0xbe8bbd47, 
		0xe5a7bf50, 
		0x7a63a058, 
		0xcc8aa22a, 
		0xa030b457, 
		0xeb04495f, 
		0x9f2526c7, 
		0xa90a9614, 
		0x4c4ec2d8, 
		0x45920044, 
		0xa8ad504f, 
		0xee6ae684, 
		0xb9881f06, 
		0xbdf3274c, 
		0x243f51a0, 
		0x2e237585, 
		0x8dd3fd4c, 
		0x4d342f3d, 
		0x45ad0c56, 
		0x85bb4828, 
		0x9ec3f491, 
		0x05951cfa, 
		0x59487b39, 
		0x37d10fce  
};
volatile uint32_t rand_ieee754_32[1536] __attribute__ ((aligned (2048))) = {
		0x02506289, 
		0x007ffffe, 
		0x7f800000, 
		0x7fc00001, 
		0xffac4b1d, 
		0x00800000, 
		0x42480000, 
		0x80000008, 
		0x0e00000f, 
		0x80000000, 
		0x007fffff, 
		0x80001000, 
		0x00008000, 
		0x7314c939, 
		0xc2340000, 
		0xb4595def, 
		0xef4ccdb8, 
		0x425c0000, 
		0x3f028f5c, 
		0x0e0007ff, 
		0xc2240000, 
		0xd3aaef87, 
		0xff7fffff, 
		0x0e007fff, 
		0x484988a5, 
		0x7f000000, 
		0x00800000, 
		0xc0000000, 
		0x80080000, 
		0x00000040, 
		0xc24c0000, 
		0x68fcdee9, 
		0x561a2446, 
		0x80001000, 
		0xdf70c44f, 
		0xc1880000, 
		0x0e03ffff, 
		0x00000080, 
		0xc1800000, 
		0x0e000007, 
		0xe3c239fe, 
		0x41500000, 
		0x0414f247, 
		0x510fac2a, 
		0x42480000, 
		0x80000002, 
		0xeb952acf, 
		0x7f7ffffe, 
		0xc2240000, 
		0x00000000, 
		0xcb8c4b40, 
		0x80000000, 
		0x00200000, 
		0x0c7ffff8, 
		0xffbfffff, 
		0x0f7ffffd, 
		0x84e1eca6, 
		0xc2380000, 
		0xcb8c4b40, 
		0x42600000, 
		0x42780000, 
		0xffc00001, 
		0xa200f4d5, 
		0x00000400, 
		0x41c80000, 
		0x7895d90c, 
		0x1d05bcea, 
		0x7f7fffff, 
		0x80800002, 
		0x49d12737, 
		0x80000000, 
		0xc2140000, 
		0x00000002, 
		0x41600000, 
		0x80000000, 
		0xc2240000, 
		0xfffad55e, 
		0x7fc00000, 
		0x00000000, 
		0x80000000, 
		0x552ef3e2, 
		0x00800000, 
		0x0e3fffff, 
		0x41880000, 
		0x0c7f8000, 
		0x40800000, 
		0xc1400000, 
		0x00000000, 
		0x41a80000, 
		0x00000002, 
		0x007fffff, 
		0xc2100000, 
		0x41c80000, 
		0x0c7c0000, 
		0xc1a80000, 
		0xc2040000, 
		0x00000000, 
		0x50bd7768, 
		0x00000010, 
		0x00000000, 
		0xfd13df86, 
		0xc1300000, 
		0xffbfffff, 
		0x5d2abf3b, 
		0x42340000, 
		0x7fa4dc1c, 
		0xd1e08bc8, 
		0xa59949ff, 
		0x00000200, 
		0x00100000, 
		0x809243b7, 
		0x00001000, 
		0x41f00000, 
		0x901a2b4e, 
		0xbf028f5c, 
		0xcb8c4b40, 
		0x00800001, 
		0xc2780000, 
		0xe7ed9c3e, 
		0x7fc00000, 
		0x80002000, 
		0x0e000003, 
		0x0c7ffc00, 
		0xc1300000, 
		0x426c0000, 
		0x11bdfe38, 
		0x7f000000, 
		0xc29cfa83, 
		0x80800001, 
		0x8ac4580d, 
		0x7f800000, 
		0x00800002, 
		0x41b80000, 
		0x80002000, 
		0xda6e1089, 
		0x0c7fffc0, 
		0xfbf5396b, 
		0x06eb6363, 
		0x3ae15aa4, 
		0x00800001, 
		0x80000000, 
		0x6d19f1d8, 
		0x425c0000, 
		0xff7ffffe, 
		0x0c7020a4, 
		0x2fcedae8, 
		0x41f80000, 
		0xfe9ca54e, 
		0x007fffff, 
		0x919539e2, 
		0xc0800000, 
		0x7fc00000, 
		0xc338996a, 
		0x80000000, 
		0x41b00000, 
		0xfe122d93, 
		0x2338f95a, 
		0x80000000, 
		0x159584e5, 
		0x1e0cbbc4, 
		0xc2280000, 
		0x41700000, 
		0x80800001, 
		0xc2500000, 
		0x007fffff, 
		0x8f7ffffe, 
		0x40a00000, 
		0x0f7ffffe, 
		0xc2240000, 
		0x0e0000ff, 
		0x0f7ffffe, 
		0x0c7f8000, 
		0x00ffffff, 
		0x9d648e3d, 
		0xc0800000, 
		0x42380000, 
		0x00800002, 
		0x1f6a0d0d, 
		0xbf800001, 
		0x41a80000, 
		0x0e000001, 
		0x0c7fffc0, 
		0x00000000, 
		0xe663e14a, 
		0x3f800000, 
		0x0000e733, 
		0x80000001, 
		0x007ffffe, 
		0x40400000, 
		0x8f7ffffd, 
		0x4795da48, 
		0x7f800000, 
		0x41e80000, 
		0x8f7fffff, 
		0xc2180000, 
		0x0e003fff, 
		0x02657931, 
		0x7f7ffffe, 
		0x00000000, 
		0xc2500000, 
		0xc2000000, 
		0x69d7dc82, 
		0xc2580000, 
		0x7f800000, 
		0x41a80000, 
		0x422c0000, 
		0x0e0007ff, 
		0x00000000, 
		0xff800000, 
		0x80080000, 
		0x42780000, 
		0x3d94eecb, 
		0xc0a00000, 
		0x41000000, 
		0x003e3f5d, 
		0xe538b427, 
		0x1cb1f4b2, 
		0x00800000, 
		0x801e6f3c, 
		0xff9345e6, 
		0xc1800000, 
		0x42200000, 
		0x00040000, 
		0x0c7ffffe, 
		0xe8f0f2b4, 
		0x00800000, 
		0x80008000, 
		0x80080000, 
		0x5e580533, 
		0x00800001, 
		0x7f7fffff, 
		0x41e00000, 
		0x89bf564b, 
		0xff4e57b0, 
		0x001efcc3, 
		0x7f7ffffe, 
		0xc22c0000, 
		0x81683205, 
		0x5ba23fce, 
		0x41800000, 
		0xc1300000, 
		0xff7ffffe, 
		0x424be5af, 
		0x7fc00001, 
		0x0e0003ff, 
		0x0f7ffffe, 
		0x8311d6d5, 
		0x41700000, 
		0x80000008, 
		0x41100000, 
		0x0c600000, 
		0xff800000, 
		0xc21c0000, 
		0x8f7ffffe, 
		0xcbe7192a, 
		0x0e07ffff, 
		0x2c530245, 
		0x7f800000, 
		0x41a00000, 
		0x80000000, 
		0x00000200, 
		0xfd85ffe4, 
		0x6ee9ab5a, 
		0xc1f80000, 
		0xc0e00000, 
		0xae1bbd54, 
		0x00800001, 
		0xc25c0000, 
		0x4b8c4b40, 
		0x00000000, 
		0x7fffffff, 
		0x80000001, 
		0xb7963422, 
		0x00000000, 
		0x41d80000, 
		0xc13ec007, 
		0x00000000, 
		0xffbcd530, 
		0x41900000, 
		0x42180000, 
		0x0c7fffe0, 
		0x80800000, 
		0x804c5d0d, 
		0x69b73952, 
		0x1b7d778c, 
		0x41f80000, 
		0x846a0b47, 
		0xc0000000, 
		0x00000001, 
		0x95708db6, 
		0x3e23aa0b, 
		0x7f800000, 
		0x97ca6142, 
		0xc1e00000, 
		0x805133e1, 
		0x0e07ffff, 
		0x591b110a, 
		0xa6fe8c37, 
		0xbf800000, 
		0x422c0000, 
		0x0de81bd5, 
		0x0e000003, 
		0xaf88bf54, 
		0x7f800000, 
		0xff8b301a, 
		0x00000000, 
		0x80000000, 
		0xc1980000, 
		0x834a33c7, 
		0xc2200000, 
		0x8f7ffffe, 
		0xffb5c506, 
		0x41b80000, 
		0x00000800, 
		0x42600000, 
		0x80000000, 
		0xc0c00000, 
		0x24dd9524, 
		0x7f000000, 
		0xcccccccc, 
		0x80000000, 
		0xff7ffffe, 
		0xfeba353c, 
		0xe59aea14, 
		0x41a80000, 
		0xff7ffffe, 
		0x421c0000, 
		0xfff666d9, 
		0x0e007fff, 
		0xdbfabcf8, 
		0xc1100000, 
		0xc2240000, 
		0x8f7ffffe, 
		0xc888d956, 
		0x4c0d433f, 
		0xa040b52e, 
		0x7f7ffffe, 
		0xbf800001, 
		0xbf800001, 
		0x00000000, 
		0xd51de3e9, 
		0x19af7333, 
		0x80000000, 
		0x00000001, 
		0x00800000, 
		0xc1f00000, 
		0x41a00000, 
		0x00000002, 
		0xc1200000, 
		0xada620d5, 
		0x7f800001, 
		0xbc50e5ab, 
		0xc1f00000, 
		0x0e000007, 
		0x00000000, 
		0x32f43a76, 
		0x00000000, 
		0x80000010, 
		0x7f9c2038, 
		0x41f00000, 
		0x00000040, 
		0x41980000, 
		0xc2440000, 
		0xff7fffff, 
		0xc2680000, 
		0x41300000, 
		0xff7ffffe, 
		0x290610f9, 
		0x7046fcaa, 
		0xf3e800cf, 
		0xa862a35e, 
		0x03a4c950, 
		0xc2100000, 
		0x40e00000, 
		0xc1600000, 
		0x4922856f, 
		0xff800000, 
		0xc23c0000, 
		0x7f800000, 
		0x26fd7923, 
		0x00800001, 
		0x80000000, 
		0x41e80000, 
		0x00800001, 
		0xc2280000, 
		0xfa638d93, 
		0x00000000, 
		0xff7fffff, 
		0xc21c0000, 
		0x80000080, 
		0x80000000, 
		0x5fee0ed6, 
		0xc1200000, 
		0x551472b7, 
		0x23300f5c, 
		0x0e000001, 
		0x7f000000, 
		0x97e00734, 
		0x60654c79, 
		0x007fffff, 
		0x7f7ffffe, 
		0x00800002, 
		0x80000001, 
		0x0c7ffff8, 
		0xffc00001, 
		0x8b3ed70d, 
		0x7a7afc1f, 
		0xb655040f, 
		0x00000800, 
		0xf12ddda8, 
		0x0e000007, 
		0x7f000000, 
		0x42140000, 
		0x0e000003, 
		0x00000000, 
		0xc2600000, 
		0x7fbfffff, 
		0x29e0f0e6, 
		0x0e000007, 
		0x00008000, 
		0xc2680000, 
		0xc2180000, 
		0xc25c0000, 
		0x0e3fffff, 
		0x80000010, 
		0xdfb90c40, 
		0x42540000, 
		0xcf00ab24, 
		0xc2680000, 
		0x80000000, 
		0x424c0000, 
		0x42180000, 
		0xc2040000, 
		0x80010000, 
		0x41a80000, 
		0x08d10f61, 
		0x80002000, 
		0x01fd76f2, 
		0x42280000, 
		0x0c7f0000, 
		0x80000004, 
		0xd0d841f9, 
		0x41d00000, 
		0xea8e261f, 
		0xc20c0000, 
		0x80200000, 
		0xac871517, 
		0x80001000, 
		0x0e0003ff, 
		0x00000000, 
		0x0c7f0000, 
		0xffaa82a2, 
		0x7fc5872a, 
		0xb5386c47, 
		0xc5b8c8f5, 
		0x0e03ffff, 
		0xc2200000, 
		0xc2380000, 
		0x807ffffe, 
		0x80000010, 
		0x4b000000, 
		0x4e1304d8, 
		0x42300000, 
		0x0c7ffff0, 
		0x0c7e0000, 
		0x00000000, 
		0x80000000, 
		0x41400000, 
		0x09fe156f, 
		0x00000000, 
		0x00000002, 
		0x0e000fff, 
		0xffbfffff, 
		0x0c7f0000, 
		0x42180000, 
		0xde4c997c, 
		0xd84d5c77, 
		0x0f7ffffd, 
		0x42380000, 
		0x41700000, 
		0x0c7ffe00, 
		0x72c9b0be, 
		0x0e000003, 
		0xc2180000, 
		0x80800003, 
		0x093bb23a, 
		0x41200000, 
		0x00800000, 
		0xff800000, 
		0x41200000, 
		0x42280000, 
		0x80000000, 
		0x41980000, 
		0xd8f4724b, 
		0xffbbc834, 
		0x257b84d0, 
		0x42700000, 
		0x00800001, 
		0x00000002, 
		0x0e3fffff, 
		0xff000000, 
		0x0c7e0000, 
		0x7f800001, 
		0xf5ef1d8e, 
		0x80000040, 
		0x7f800000, 
		0xc0400000, 
		0xbefc5952, 
		0xbbebc62e, 
		0x365603bb, 
		0x00000008, 
		0x80000000, 
		0x80000001, 
		0x80800003, 
		0x80000000, 
		0xc2300000, 
		0x00000001, 
		0x00200000, 
		0x00800001, 
		0x00000008, 
		0xf4b185ea, 
		0x41f00000, 
		0x7f800000, 
		0xcb8c4b40, 
		0xff800000, 
		0xf6bdb36c, 
		0x0c7ffff0, 
		0x8f7ffffc, 
		0x50ec1d80, 
		0xcb8c4b40, 
		0x9ce0a134, 
		0x00000000, 
		0xe2bf612f, 
		0x7f7ffffe, 
		0x42180000, 
		0x7f7fffff, 
		0x80800000, 
		0x0e03ffff, 
		0x00000000, 
		0xec543d5f, 
		0x7f800000, 
		0x29b9fa6c, 
		0x80800000, 
		0x1207dd30, 
		0x3bb38b97, 
		0x41400000, 
		0x00800002, 
		0x42740000, 
		0x00000000, 
		0xc2080000, 
		0xc1980000, 
		0x80000020, 
		0x7f800000, 
		0xd0e72996, 
		0x57a56839, 
		0x0e001fff, 
		0x00000000, 
		0x00ffffff, 
		0x7f800001, 
		0x0c7fff00, 
		0x80800000, 
		0x30da875f, 
		0x94987371, 
		0x80020000, 
		0x0e0fffff, 
		0x0e0001ff, 
		0x42580000, 
		0xc2200000, 
		0x0be93bec, 
		0x41400000, 
		0x4684ca1a, 
		0x80800001, 
		0x0c7fe000, 
		0x00200000, 
		0xe7b06901, 
		0x00000000, 
		0x00000000, 
		0x00800000, 
		0xffc00000, 
		0x42140000, 
		0xff7fffff, 
		0x4c7051ee, 
		0xff7ffffe, 
		0x49f13473, 
		0xc2280000, 
		0x0e00001f, 
		0xb2d73e51, 
		0x6e7e4224, 
		0x00000004, 
		0x0e07ffff, 
		0xbf800000, 
		0x3fdf20f1, 
		0x80080000, 
		0x00000000, 
		0xc2340000, 
		0x6f8ea0af, 
		0xbe82cf18, 
		0x28a8af59, 
		0x40400000, 
		0x88877f8c, 
		0xc1300000, 
		0x80017324, 
		0xffc00000, 
		0x41400000, 
		0xc1300000, 
		0xf1878473, 
		0xffffffff, 
		0x4b000000, 
		0xc0800000, 
		0xc5322d93, 
		0xff800000, 
		0x80004000, 
		0xffa53221, 
		0x0f250cf0, 
		0x00000000, 
		0x49e1ef29, 
		0x0f7ffffe, 
		0xc0800000, 
		0xc23c0000, 
		0xc0626fe8, 
		0x42100000, 
		0xad390d05, 
		0xff7ffffe, 
		0x80020000, 
		0x00000001, 
		0x41c80000, 
		0x73dacc5b, 
		0x00800002, 
		0xd7e2192b, 
		0x0c600000, 
		0x80800000, 
		0xc0000000, 
		0xc1100000, 
		0xb47fb7a2, 
		0x00000002, 
		0x7fa210cc, 
		0x40e00000, 
		0x7fc00001, 
		0xe231017e, 
		0x80800001, 
		0x0c400000, 
		0x80000080, 
		0x41d80000, 
		0xbed49b99, 
		0x0e0000ff, 
		0x801ad869, 
		0xc1500000, 
		0xc27c0000, 
		0x002bed82, 
		0x80011111, 
		0xc2740000, 
		0x7f7fffff, 
		0xcf1178f0, 
		0x580285f0, 
		0x00000000, 
		0x80800003, 
		0x7f800000, 
		0x41d00000, 
		0x7f800000, 
		0x53d28b20, 
		0x97fe484f, 
		0x8b840710, 
		0x0a1322dc, 
		0x00200000, 
		0x2f11eca2, 
		0x7deba268, 
		0x00800000, 
		0xcb000000, 
		0x80800000, 
		0x1888c125, 
		0xff800000, 
		0x7fc00001, 
		0x80000000, 
		0x0e0007ff, 
		0x00000000, 
		0x00400000, 
		0x2bb47f37, 
		0xff7ffffe, 
		0xc1200000, 
		0xe764a29c, 
		0x00000000, 
		0x425c0000, 
		0xc1800000, 
		0xc22c0000, 
		0x0e007fff, 
		0xff800000, 
		0x9eddc802, 
		0x0e00ffff, 
		0x67e080ca, 
		0x4c63d6cc, 
		0x0c7fff80, 
		0x14ed3033, 
		0x0c7fffc0, 
		0x84ff15c1, 
		0xc2580000, 
		0x0d00fff0, 
		0xc17a5ee1, 
		0x91dc0a45, 
		0x00000000, 
		0x7f800000, 
		0x41b00000, 
		0xff800000, 
		0x0e00001f, 
		0x0c7fffff, 
		0x1d4687fe, 
		0x7f7fffff, 
		0x2d78bcb6, 
		0x35e7b4a9, 
		0xc1e80000, 
		0x3f800001, 
		0x00040000, 
		0xc1300000, 
		0x00ffffff, 
		0xc0000000, 
		0x43f52d83, 
		0x80800000, 
		0xc25c0000, 
		0xc1f00000, 
		0x80000000, 
		0x66dc4244, 
		0xc2200000, 
		0x74e4f326, 
		0x0c7ffff8, 
		0x80000001, 
		0xc23c0000, 
		0x1971d354, 
		0xc1900000, 
		0xa763d1bf, 
		0x80000000, 
		0x0c600000, 
		0x80000002, 
		0xffbfffff, 
		0xffffffff, 
		0x3f800001, 
		0x8f7ffffe, 
		0xc2180000, 
		0x4d340c44, 
		0xff92026b, 
		0x0e3fffff, 
		0x00800000, 
		0xeb4e2497, 
		0x59e7689c, 
		0x0e03ffff, 
		0x80000000, 
		0x003cb54b, 
		0xd2b889d4, 
		0xc2100000, 
		0xc2200000, 
		0x0c7fffff, 
		0xc1600000, 
		0x007ffffe, 
		0x62f43b5f, 
		0x00000002, 
		0x41100000, 
		0xb9d52785, 
		0x805a279b, 
		0x899f576f, 
		0x70c54ab7, 
		0x7f800000, 
		0x0e00007f, 
		0x0e0000ff, 
		0x42240000, 
		0xc1100000, 
		0x4779e8e8, 
		0x9506eacb, 
		0x42780000, 
		0x41e80000, 
		0x00000000, 
		0xc23c0000, 
		0x42780000, 
		0x0e00ffff, 
		0xc2440000, 
		0xcb000000, 
		0x80000100, 
		0xb42f733c, 
		0xbb3621e1, 
		0xe0e98fdb, 
		0xc1880000, 
		0xf27a942a, 
		0x52eea60f, 
		0x42480000, 
		0x00800001, 
		0x3f028f5c, 
		0x061974b3, 
		0xcd7b4b46, 
		0x00040000, 
		0xd54a291f, 
		0x0e000007, 
		0x0c7fffff, 
		0xd1424825, 
		0xc2200000, 
		0x83fd968a, 
		0x00000000, 
		0x7f7fffff, 
		0x41f80000, 
		0x2f411d7e, 
		0xcf5ecab8, 
		0x5126ffc3, 
		0x00000002, 
		0xc5424141, 
		0x5a7886f5, 
		0x3f800000, 
		0x8f7ffffd, 
		0xbf800001, 
		0xec8881b4, 
		0x9378dac0, 
		0x5a9595db, 
		0xc22c0000, 
		0x00000000, 
		0x7484e101, 
		0x80000001, 
		0x7fc00000, 
		0x80000000, 
		0x00000000, 
		0x8871804a, 
		0x79d9b800, 
		0x0c7fff80, 
		0x02cda173, 
		0x5e418f6c, 
		0x7f800001, 
		0xb5efd82a, 
		0xc1400000, 
		0xc1900000, 
		0x8057177a, 
		0x41b00000, 
		0xffaba271, 
		0xf20e14bf, 
		0xa5b919c0, 
		0x0c400000, 
		0xe0c3b3ce, 
		0x00000100, 
		0x42580000, 
		0xc21c0000, 
		0x00556945, 
		0x9e39725f, 
		0xc1e80000, 
		0x7f7fffff, 
		0x42640000, 
		0x18019c63, 
		0x0c7ffc00, 
		0x7419ba9f, 
		0x24a7b0e6, 
		0xc1300000, 
		0xff826212, 
		0xffa79560, 
		0x00000000, 
		0xc1aedc11, 
		0x80000000, 
		0x42794b3d, 
		0x80800000, 
		0x7f9af1b3, 
		0x4c2e9e11, 
		0x3cc98b4c, 
		0xc22c0000, 
		0x0e00ffff, 
		0xf0c383d1, 
		0x2edd091c, 
		0x41900000, 
		0x0c7fff80, 
		0x59e0d81a, 
		0xf12fa5d8, 
		0x42540000, 
		0x7f800000, 
		0x36621454, 
		0x0e000001, 
		0xc2380000, 
		0xc1a00000, 
		0xc01408ec, 
		0x00001000, 
		0x7f7fffff, 
		0x17e54f9a, 
		0x80000000, 
		0xc2400000, 
		0x7f7ffffe, 
		0xc1980000, 
		0x7fba899b, 
		0x8003a375, 
		0xff800000, 
		0xc2040000, 
		0x0e001fff, 
		0x00004000, 
		0x80800000, 
		0xc23c0000, 
		0x00477b6d, 
		0x14013e45, 
		0x5cab612b, 
		0x41d00000, 
		0xad4a61bf, 
		0x80800001, 
		0x7f000000, 
		0x00000000, 
		0x7f7ffffe, 
		0x00000008, 
		0x922c677a, 
		0x4b000000, 
		0x0f7ffffc, 
		0x80800003, 
		0x0f7fffff, 
		0x80008000, 
		0x40400000, 
		0xc8082978, 
		0xc0800000, 
		0x5db63ff0, 
		0x93acd4b3, 
		0x41600000, 
		0x80000000, 
		0xc2200000, 
		0x00001000, 
		0xc2500000, 
		0x80800003, 
		0x42640000, 
		0x425c9fee, 
		0x381e5dc7, 
		0xaaaaaaaa, 
		0xbf700a30, 
		0x7fffffff, 
		0x9a661eb3, 
		0xc1900000, 
		0x80800001, 
		0x0c600000, 
		0xff800000, 
		0x80100000, 
		0x80008000, 
		0xc2500000, 
		0xff7ffffe, 
		0x80800002, 
		0x00080000, 
		0xc2040000, 
		0x423c0000, 
		0x41980000, 
		0xae3bee2e, 
		0xeefc1add, 
		0x80200000, 
		0xc2aef91d, 
		0x42180000, 
		0x42480000, 
		0x42780000, 
		0x82fc38d7, 
		0xb9eb1b7a, 
		0xc1b80000, 
		0x0e03ffff, 
		0x42440000, 
		0x10ca6d44, 
		0x00000010, 
		0x00800001, 
		0xc2300000, 
		0xda140c20, 
		0x80000000, 
		0x00800001, 
		0xc24c0000, 
		0x5f877558, 
		0x00004000, 
		0x42000000, 
		0xe6ddc5b0, 
		0xd81dac70, 
		0x00040000, 
		0x41600000, 
		0xc2140000, 
		0x00000000, 
		0x1c718699, 
		0x5265a1fc, 
		0xc2340000, 
		0xc2340000, 
		0x7fbfffff, 
		0x41000000, 
		0x80020000, 
		0x80000001, 
		0x80000010, 
		0x8f7ffffe, 
		0x41000000, 
		0xe9f3d330, 
		0xc1f80000, 
		0x41600000, 
		0x00000002, 
		0x7f800001, 
		0x1a998cce, 
		0x0c7e0000, 
		0x80000000, 
		0x5cd37c5e, 
		0x83215b29, 
		0x179f5f67, 
		0x51ea0401, 
		0x00000200, 
		0x9f077dd6, 
		0x42740000, 
		0x729859ed, 
		0xffbfffff, 
		0x00000000, 
		0x41500000, 
		0x80800001, 
		0x00000001, 
		0x0c700000, 
		0x0e003fff, 
		0x80800003, 
		0x0e00001f, 
		0x00000800, 
		0xd32e591a, 
		0x0c7ffe00, 
		0x0e0000ff, 
		0xc2000000, 
		0xc2200000, 
		0x0f08a954, 
		0xeb8a0fd4, 
		0xc1b00000, 
		0x0e0fffff, 
		0x0c7b19fa, 
		0x8f7ffffd, 
		0xbb4d7aa2, 
		0x80000080, 
		0x11c04667, 
		0xc2340000, 
		0x73b2a13e, 
		0x0e00001f, 
		0xd9ce1297, 
		0x40000000, 
		0x42180000, 
		0x00100000, 
		0xe5f6dd0a, 
		0x80800001, 
		0xe19e0946, 
		0x0c7f8000, 
		0x46677a76, 
		0xc1c00000, 
		0x0d00fff0, 
		0x55555555, 
		0x00000000, 
		0x89de4d20, 
		0x7f800001, 
		0x42100000, 
		0xc2540000, 
		0x41500000, 
		0xac672675, 
		0x40000000, 
		0x42680000, 
		0x5f374c18, 
		0x42440000, 
		0x5aac353b, 
		0x00800002, 
		0x5b45c17d, 
		0x0e3fffff, 
		0x80800002, 
		0x41a80000, 
		0x80000400, 
		0x8f7ffffe, 
		0x00000008, 
		0xc2140000, 
		0xc21c0000, 
		0x0c600000, 
		0xc24c0000, 
		0x00000010, 
		0x42400000, 
		0xc1c00000, 
		0x41880000, 
		0x80000000, 
		0x423c0000, 
		0x42680000, 
		0x7a28605a, 
		0x40800000, 
		0x19e27994, 
		0x7f800000, 
		0xeca1a16b, 
		0x42e5adfe, 
		0xeb202f17, 
		0x41a00000, 
		0xbf800001, 
		0x41b00000, 
		0x12226260, 
		0x42740000, 
		0x55555555, 
		0x280d6b0f, 
		0xc1500000, 
		0xc1400000, 
		0xaab7e9c9, 
		0x41880000, 
		0xc2740000, 
		0x41a80000, 
		0x0035502b, 
		0x8f7ffffd, 
		0x0e000007, 
		0x0f14edf2, 
		0x40a00000, 
		0x42000000, 
		0x80000000, 
		0x172dc9ff, 
		0x5022eb05, 
		0x0c7fff00, 
		0x1a76beee, 
		0xe7272660, 
		0x5ded59cd, 
		0x424c0000, 
		0x80800000, 
		0x0f7ffffe, 
		0x00800002, 
		0x41800000, 
		0x80000000, 
		0x0c7ffc00, 
		0x7f800000, 
		0x3642507e, 
		0x40800000, 
		0x425c0000, 
		0x4972887b, 
		0x42780000, 
		0x00000000, 
		0x41800000, 
		0x41880000, 
		0x42740000, 
		0x80000000, 
		0x0c7ffc00, 
		0x00000100, 
		0xffb5921d, 
		0x80020000, 
		0xe574590e, 
		0x9c128f32, 
		0x7fc00001, 
		0x42040000, 
		0xc2280000, 
		0x0c7f8000, 
		0x87ca61c1, 
		0x42580000, 
		0x42440000, 
		0xff7ffffe, 
		0x7f800000, 
		0x42600000, 
		0x80000100, 
		0xcab3c3b6, 
		0x80800003, 
		0x0e001fff, 
		0x39386230, 
		0x00200000, 
		0x00800002, 
		0x0e007fff, 
		0xff800000, 
		0x00000000, 
		0x422c0000, 
		0x0c7ff000, 
		0xff800001, 
		0x0f7ffffe, 
		0x0e7fffff, 
		0x00002000, 
		0x42280000, 
		0x80002000, 
		0x42740000, 
		0xc1600000, 
		0x803545e5, 
		0xb36d0b6b, 
		0x4b8c4b40, 
		0x0c7ff000, 
		0x0e0001ff, 
		0xc2340000, 
		0xc2780000, 
		0x00000100, 
		0x00800003, 
		0xc2740000, 
		0xff7ffffe, 
		0xffb3a3ff, 
		0x637717e4, 
		0x0c7fffff, 
		0x40b00d05, 
		0x0c7c0000, 
		0xc0800000, 
		0x80040000, 
		0x0c963a98, 
		0x554af40b, 
		0x71745af6, 
		0x80002000, 
		0x007fffff, 
		0x00000020, 
		0x7f800001, 
		0x41a80000, 
		0x00000010, 
		0x00800000, 
		0xf86a28b1, 
		0x7d93ba23, 
		0x9a9eda88, 
		0x15bdbb2f, 
		0x80200000, 
		0x8071552e, 
		0x492174e1, 
		0x5d78e653, 
		0xaaaaaaaa, 
		0x5bdeb752, 
		0xc0c00000, 
		0x2ca8771a, 
		0x4f211be6, 
		0x0e000007, 
		0xc2200000, 
		0x00010000, 
		0x41f80000, 
		0x63733f8b, 
		0x60a9d0c6, 
		0x80000000, 
		0x41b00000, 
		0x42740000, 
		0x00000000, 
		0x0e7fffff, 
		0xdce54d17, 
		0x8f7fffff, 
		0xc1700000, 
		0x31d8767f, 
		0x00800000, 
		0x4a2914bc, 
		0x42340000, 
		0xb1ddd14f, 
		0x7f800000, 
		0xac263ce6, 
		0x40e00000, 
		0xc2300000, 
		0x41800000, 
		0x0e000007, 
		0x00000001, 
		0x80800000, 
		0x80398f4c, 
		0xbc5d2a55, 
		0x76290344, 
		0x00800000, 
		0x7f800000, 
		0x80000000, 
		0x42040000, 
		0xc2300000, 
		0x0c7fffe0, 
		0x608b5004, 
		0x55555555, 
		0xc1a00000, 
		0x57a099cb, 
		0x00001000, 
		0xe18341fb, 
		0x01860fa5, 
		0x7f800000, 
		0x0097e643, 
		0x0c7c0000, 
		0xc2180000, 
		0x7fc00001, 
		0x767c776d, 
		0x0c7ffff0, 
		0x0c600000, 
		0xc24c0000, 
		0x00000008, 
		0x80000000, 
		0x41400000, 
		0xff8e3e75, 
		0xff7fffff, 
		0x0e3fffff, 
		0x41300000, 
		0xc2280000, 
		0x00010000, 
		0x33333333, 
		0x80000001, 
		0x00040000, 
		0x7f000000, 
		0x00000000, 
		0xf2e35b08, 
		0x42280000, 
		0x7f7ffffe, 
		0xfeba5c03, 
		0x9ca8486c, 
		0xff800000, 
		0x16a9618f, 
		0xa9b7020e, 
		0x00000002, 
		0x00000002, 
		0xc2340000, 
		0x0e000001, 
		0x00000020, 
		0x40e00000, 
		0x3505ad55, 
		0x80000010, 
		0xd0184f61, 
		0x8f7ffffc, 
		0xffffffff, 
		0x00000000, 
		0xc1880000, 
		0xff7ffffe, 
		0x2fdcb39a, 
		0x00000000, 
		0x427c0000, 
		0xc20c0000, 
		0x658a660f, 
		0x8f7ffffc, 
		0x0415ce83, 
		0xc1f00000, 
		0x41880000, 
		0x0f7ffffe, 
		0xc2740000, 
		0x40eae50f, 
		0x80000080, 
		0x74d237c5, 
		0x0c7f0000, 
		0x42500000, 
		0xc0a00000, 
		0x17336149, 
		0xa594ca24, 
		0xc6afea63, 
		0x40a00000, 
		0x00100000, 
		0x80000020, 
		0x329a30ae, 
		0xc2500000, 
		0xff800000, 
		0x007ffffe, 
		0xc1500000, 
		0x7f800000, 
		0xff800000, 
		0x28ad4110, 
		0xa20a014f, 
		0x007fffff, 
		0x7f800000, 
		0x80000000, 
		0xfbf6973d, 
		0x80000000, 
		0x7f800000, 
		0x00200000, 
		0x00000080, 
		0x7f943032, 
		0xff7ffffe, 
		0x0f7ffffc, 
		0x7f800000, 
		0x24d32b5a, 
		0xc27c0000, 
		0xa3c98f41, 
		0x80000000, 
		0x80000010, 
		0x7f8b82e2, 
		0x335d9252, 
		0xff800000, 
		0x3f028f5c, 
		0xdc3bf83a, 
		0x42340000, 
		0x42200000, 
		0x10e7ff80, 
		0x22753280, 
		0x0e00000f, 
		0x00004000, 
		0x40400000, 
		0x00800001, 
		0x8402cca4, 
		0xb03ed451, 
		0x00000000, 
		0x5e12453c, 
		0x00000008, 
		0x00040000, 
		0xb43df999, 
		0xc0c00000, 
		0x0c600000, 
		0x3e5e9170, 
		0x00000000, 
		0x0e00ffff, 
		0x42200000, 
		0x00ffffff, 
		0xc1400000, 
		0x42640000, 
		0x20e20d10, 
		0xffa197e6, 
		0x00000010, 
		0x00800000, 
		0xc0a00000, 
		0x69b14c95, 
		0x42240000, 
		0x206862a1, 
		0x70aee92e, 
		0x425c0000, 
		0x0d000ff0, 
		0x8f7ffffe, 
		0xc1500000, 
		0x0c7ffffe, 
		0x6d211b4e, 
		0x5e255785, 
		0x42400000, 
		0x42340000, 
		0xffc00000, 
		0x7f7ffffe, 
		0x80000001, 
		0x80000000, 
		0xaaaaaaaa, 
		0x0c7f0000, 
		0xd0709d2a, 
		0x807fffff, 
		0x0e3fffff, 
		0x0c7ffff8, 
		0xc2740000, 
		0x80800000, 
		0x007fffff, 
		0x41d00000, 
		0xc1980000, 
		0xc2600000, 
		0x00ffffff, 
		0x9fa3424c, 
		0x71e14614, 
		0x00000001, 
		0x80001000, 
		0x421c0000, 
		0x1510001a, 
		0x0e01ffff, 
		0xff7fffff, 
		0x0c7c0000, 
		0x32a52811, 
		0x80000002, 
		0x41c80000, 
		0x80000002, 
		0x80001000, 
		0x306675a3, 
		0xff800000, 
		0xd804c027, 
		0x00800001, 
		0xa55a9108, 
		0xf3b3c5e9, 
		0x41f00000, 
		0x41e80000, 
		0x8967c1d4, 
		0xffffffff, 
		0xc0e00000, 
		0xc1e80000, 
		0x80011111, 
		0x7f7ffffe, 
		0x7f7ffffe, 
		0x80000000, 
		0xffffffff, 
		0x0f7ffffc, 
		0xc1100000, 
		0x62ac74be, 
		0x4722e3e6, 
		0x00080000, 
		0x00000000, 
		0x0d08f1c0, 
		0x7f800000, 
		0xc1500000, 
		0xc2480000, 
		0x0c7ffffc, 
		0x0c7ffffe, 
		0x00002000, 
		0x0e3fffff, 
		0xc1500000, 
		0xc2000000, 
		0x42280000, 
		0x0e0000ff, 
		0x00800001, 
		0xc1b00000, 
		0x20c8b5d0, 
		0xc2340000, 
		0x80000008, 
		0x00000040, 
		0x0c7fff80, 
		0x80800001, 
		0x80000800, 
		0x3678e891, 
		0x680a321d, 
		0x40c00000, 
		0x80020000, 
		0x80635bdf, 
		0xff97808d, 
		0x0715584f, 
		0xff7ffffe, 
		0xe7679b07, 
		0xae9768a8, 
		0xc2640000, 
		0x80000000, 
		0x7fc0c9dc, 
		0x7f800001, 
		0xf96dee63, 
		0x7f7ffffe, 
		0x32ab0abf, 
		0x544734e9, 
		0x80800002, 
		0x6f6b5822, 
		0xb4f474cd, 
		0xd22aa8e6, 
		0xc1f00000, 
		0xc2780000, 
		0xc25c0000, 
		0xffc00000, 
		0x654ca382, 
		0x8c0d779a, 
		0x1a51a1be, 
		0x836ba9bb, 
		0xc2740000, 
		0x80000000, 
		0x42400000, 
		0x427c0000, 
		0x15996bd7, 
		0x80000040, 
		0x42080000, 
		0x0e001fff, 
		0x42340000, 
		0x0e03ffff, 
		0x42200000, 
		0x80000200, 
		0x007fffff, 
		0x80010000, 
		0xc1600000, 
		0xc1e00000, 
		0x8f7ffffc, 
		0x655da93d, 
		0x80000000, 
		0x982979d3, 
		0x30580820, 
		0x0c7fc000, 
		0x0c700000, 
		0xd8a4c3ef, 
		0x3ffe0211, 
		0x0e000003, 
		0xf9d7ff44, 
		0x361ca876, 
		0xa2499997, 
		0x0e007fff, 
		0x8f7ffffe, 
		0xdac9dea7, 
		0xc1e00000, 
		0x0c7ffff0, 
		0xe107594e, 
		0x10b8b628, 
		0x0c7c0000, 
		0x80020000, 
		0x80004000, 
		0x0f7ffffc, 
		0x00000002, 
		0x0b2855a9, 
		0xc7951043, 
		0x00000000, 
		0x002430f3, 
		0x8f7fffff, 
		0x7f7ffffe, 
		0x80800001, 
		0x00000002, 
		0x807e6052, 
		0x076690ba, 
		0x41d80000, 
		0x8f7fffff, 
		0x80000001, 
		0xb953ebe6, 
		0x41d80000, 
		0xff800000, 
		0x0411b48a, 
		0xe129b101, 
		0x0c7f8000, 
		0xb2e927c8, 
		0x7f000000, 
		0xc22c0000  
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

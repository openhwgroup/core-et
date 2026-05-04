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
			"csrwi mhpmevent3, 2\n"   
			"csrwi mhpmevent4, 28\n"   
			"csrwi mhpmevent5, 14\n"   
			"csrwi mhpmevent6, 27\n"   
			"csrwi mhpmevent7, 5\n"   
			"csrwi mhpmevent8, 3\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f19,  704 (x5)\n"
			"flw.ps f18,  928 (x5)\n"
			"flw.ps f2,  672 (x5)\n"
			"flw.ps f24,  480 (x5)\n"
			"flw.ps f25,  352 (x5)\n"
			"flw.ps f26,  0 (x5)\n"
			"flw.ps f29,  256 (x5)\n"
			"flw.ps f7,  608 (x5)\n"
			"flw.ps f5,  288 (x5)\n"
			"flw.ps f13,  896 (x5)\n"
			"flw.ps f27,  448 (x5)\n"
			"flw.ps f16,  640 (x5)\n"
			"flw.ps f17,  800 (x5)\n"
			"flw.ps f10,  992 (x5)\n"
			"flw.ps f20,  64 (x5)\n"
			"flw.ps f15,  768 (x5)\n"
			"flw.ps f4,  512 (x5)\n"
			"flw.ps f9,  864 (x5)\n"
			"flw.ps f1,  160 (x5)\n"
			"flw.ps f0,  416 (x5)\n"
			"flw.ps f28,  32 (x5)\n"
			"flw.ps f12,  128 (x5)\n"
			"flw.ps f30,  832 (x5)\n"
			"flw.ps f22,  192 (x5)\n"
			"flw.ps f14,  544 (x5)\n"
			"flw.ps f3,  576 (x5)\n"
			"flw.ps f21,  384 (x5)\n"
			"flw.ps f23,  224 (x5)\n"
			"flw.ps f8,  960 (x5)\n"
			"flw.ps f31,  320 (x5)\n"
			"flw.ps f6,  736 (x5)\n"
			"flw.ps f11,  96 (x5)\n"
			"LBL_VADDR_INIT_HID_0:\n"          
			"li x21, 0x810035a020 \n"                 
			"fsw.ps f27, (x21) \n"                 
			"li x23, 0x81005ed800 \n"                 
			"fsw.ps f13, (x23) \n"                 
			"li x17, 0x8100d006c0 \n"                 
			"fsw.ps f2, (x17) \n"                 
			"li x12, 0x81016a0000 \n"                 
			"fsw.ps f14, (x12) \n"                 
			"li x22, 0x81017a1020 \n"                 
			"fsw.ps f0, (x22) \n"                 
			"li x25, 0x8102be00a0 \n"                 
			"fsw.ps f25, (x25) \n"                 
			"li x16, 0x8103ad2f40 \n"                 
			"fsw.ps f15, (x16) \n"                 
			"li x18, 0x8103c49000 \n"                 
			"fsw.ps f4, (x18) \n"                 
			"li x23, 0x8105d4a080 \n"                 
			"fsw.ps f0, (x23) \n"                 
			"li x21, 0x810896e000 \n"                 
			"fsw.ps f13, (x21) \n"                 
			"li x21, 0x810bfa0400 \n"                 
			"fsw.ps f20, (x21) \n"                 
			"li x14, 0x810c940120 \n"                 
			"fsw.ps f13, (x14) \n"                 
			"li x22, 0x810cea5020 \n"                 
			"fsw.ps f28, (x22) \n"                 
			"li x10, 0x810e380000 \n"                 
			"fsw.ps f13, (x10) \n"                 
			"li x15, 0x810fea0760 \n"                 
			"fsw.ps f7, (x15) \n"                 
			"li x11, 0x82012a0020 \n"                 
			"fsw.ps f10, (x11) \n"                 
			"li x16, 0x8201a0ffa0 \n"                 
			"fsw.ps f11, (x16) \n"                 
			"li x16, 0x82029f7020 \n"                 
			"fsw.ps f5, (x16) \n"                 
			"li x17, 0x82032c0020 \n"                 
			"fsw.ps f12, (x17) \n"                 
			"li x15, 0x8203860a20 \n"                 
			"fsw.ps f24, (x15) \n"                 
			"li x21, 0x8203861f60 \n"                 
			"fsw.ps f8, (x21) \n"                 
			"li x20, 0x8207600740 \n"                 
			"fsw.ps f13, (x20) \n"                 
			"li x12, 0x8209bee820 \n"                 
			"fsw.ps f22, (x12) \n"                 
			"li x20, 0x820a78e020 \n"                 
			"fsw.ps f0, (x20) \n"                 
			"li x13, 0x820a9a0000 \n"                 
			"fsw.ps f8, (x13) \n"                 
			"li x25, 0x820d47ef60 \n"                 
			"fsw.ps f15, (x25) \n"                 
			"li x24, 0x820f1c0020 \n"                 
			"fsw.ps f11, (x24) \n"                 
			"fence\n"
			"LBL_GLOBAL_VADDR_INIT_HID_0:\n"          
			"li x13, 0x812035a020 \n"                 
			"fswg.ps f21, (x13) \n"                 
			"li x11, 0x81216a0000 \n"                 
			"fswg.ps f29, (x11) \n"                 
			"li x25, 0x8121deb800 \n"                 
			"fswg.ps f23, (x25) \n"                 
			"li x19, 0x8123c49000 \n"                 
			"fswg.ps f2, (x19) \n"                 
			"li x21, 0x81256400a0 \n"                 
			"fswg.ps f20, (x21) \n"                 
			"li x18, 0x812c940120 \n"                 
			"fswg.ps f1, (x18) \n"                 
			"li x12, 0x812e380000 \n"                 
			"fswg.ps f26, (x12) \n"                 
			"li x14, 0x81317a1020 \n"                 
			"fswg.ps f4, (x14) \n"                 
			"li x17, 0x81338e0020 \n"                 
			"fswg.ps f4, (x17) \n"                 
			"li x10, 0x813896e000 \n"                 
			"fswg.ps f6, (x10) \n"                 
			"li x22, 0x813bfa0400 \n"                 
			"fswg.ps f31, (x22) \n"                 
			"li x17, 0x813cea5020 \n"                 
			"fswg.ps f0, (x17) \n"                 
			"li x11, 0x813fea0760 \n"                 
			"fswg.ps f19, (x11) \n"                 
			"li x22, 0x813ff6c420 \n"                 
			"fswg.ps f14, (x22) \n"                 
			"li x23, 0x8221a0ffa0 \n"                 
			"fswg.ps f9, (x23) \n"                 
			"li x14, 0x8227600740 \n"                 
			"fswg.ps f3, (x14) \n"                 
			"li x10, 0x822c89aec0 \n"                 
			"fswg.ps f14, (x10) \n"                 
			"li x18, 0x822d47ef60 \n"                 
			"fswg.ps f13, (x18) \n"                 
			"li x16, 0x822f1c0020 \n"                 
			"fswg.ps f6, (x16) \n"                 
			"li x17, 0x82312a0020 \n"                 
			"fswg.ps f11, (x17) \n"                 
			"li x21, 0x82329f7020 \n"                 
			"fswg.ps f13, (x21) \n"                 
			"li x17, 0x82332c0020 \n"                 
			"fswg.ps f28, (x17) \n"                 
			"li x23, 0x8233860a20 \n"                 
			"fswg.ps f31, (x23) \n"                 
			"li x10, 0x8233de0000 \n"                 
			"fswg.ps f27, (x10) \n"                 
			"li x22, 0x8234f600a0 \n"                 
			"fswg.ps f26, (x22) \n"                 
			"li x25, 0x82359407a0 \n"                 
			"fswg.ps f22, (x25) \n"                 
			"li x23, 0x8239640020 \n"                 
			"fswg.ps f30, (x23) \n"                 
			"fence\n"
			"LBL_LOCAL_VADDR_INIT_HID_0:\n"          
			"li x13, 0x814035a020 \n"                 
			"fswl.ps f31, (x13) \n"                 
			"li x17, 0x8140d006c0 \n"                 
			"fswl.ps f31, (x17) \n"                 
			"li x18, 0x8141deb800 \n"                 
			"fswl.ps f8, (x18) \n"                 
			"li x13, 0x8143c49000 \n"                 
			"fswl.ps f24, (x13) \n"                 
			"li x17, 0x81440a0200 \n"                 
			"fswl.ps f24, (x17) \n"                 
			"li x11, 0x81456400a0 \n"                 
			"fswl.ps f29, (x11) \n"                 
			"li x12, 0x814b6ff100 \n"                 
			"fswl.ps f26, (x12) \n"                 
			"li x19, 0x814ff6c420 \n"                 
			"fswl.ps f19, (x19) \n"                 
			"li x13, 0x81505ed800 \n"                 
			"fswl.ps f11, (x13) \n"                 
			"li x13, 0x81517a1020 \n"                 
			"fswl.ps f7, (x13) \n"                 
			"li x15, 0x8152be00a0 \n"                 
			"fswl.ps f3, (x15) \n"                 
			"li x14, 0x8153ad2f40 \n"                 
			"fswl.ps f2, (x14) \n"                 
			"li x16, 0x815896e000 \n"                 
			"fswl.ps f29, (x16) \n"                 
			"li x10, 0x815bfa0400 \n"                 
			"fswl.ps f31, (x10) \n"                 
			"li x16, 0x8241a0ffa0 \n"                 
			"fswl.ps f18, (x16) \n"                 
			"li x21, 0x8243de0000 \n"                 
			"fswl.ps f6, (x21) \n"                 
			"li x24, 0x8244f600a0 \n"                 
			"fswl.ps f5, (x24) \n"                 
			"li x12, 0x82459407a0 \n"                 
			"fswl.ps f15, (x12) \n"                 
			"li x16, 0x8249bee820 \n"                 
			"fswl.ps f1, (x16) \n"                 
			"li x12, 0x824a9a0000 \n"                 
			"fswl.ps f5, (x12) \n"                 
			"li x25, 0x82512a0020 \n"                 
			"fswl.ps f4, (x25) \n"                 
			"li x12, 0x8253861f60 \n"                 
			"fswl.ps f2, (x12) \n"                 
			"li x16, 0x8257600740 \n"                 
			"fswl.ps f0, (x16) \n"                 
			"li x16, 0x825a78e020 \n"                 
			"fswl.ps f1, (x16) \n"                 
			"li x13, 0x825c89aec0 \n"                 
			"fswl.ps f12, (x13) \n"                 
			"li x24, 0x825d47ef60 \n"                 
			"fswl.ps f17, (x24) \n"                 
			"li x20, 0x825f1c0020 \n"                 
			"fswl.ps f17, (x20) \n"                 
			"fence\n"
			"LBL_LOCAL_L2SCP_INIT_HID_0:\n"          
			"la x11, rand_ieee754_32\n"    
			"li x31, 0x40\n"                 
			"li x5, 0x1f \n"                 
			"add x5, x11, x5\n"               
			"csrw tensor_load_l2, x5\n"         
			"addi x31, x31, 1\n"                 
			"li x5, 0x400000000001f \n"                 
			"add x5, x11, x5\n"               
			"csrw tensor_load_l2, x5\n"         
			"addi x31, x31, -1\n"                 
			"li x5, 0x800000000001f \n"                 
			"add x5, x11, x5\n"               
			"csrw tensor_load_l2, x5\n"         
			"addi x31, x31, 1\n"                 
			"li x5, 0xc00000000001f \n"                 
			"add x5, x11, x5\n"               
			"csrw tensor_load_l2, x5\n"         
			"addi x31, x31, -1\n"                 
			"li x5, 0x4100000000003f \n"                 
			"add x5, x11, x5\n"               
			"csrw tensor_load_l2, x5\n"         
			"addi x31, x31, 1\n"                 
			"li x5, 0x4500000000003f \n"                 
			"add x5, x11, x5\n"               
			"csrw tensor_load_l2, x5\n"         
			"addi x31, x31, -1\n"                 
			"li x5, 0x4900000000003f \n"                 
			"add x5, x11, x5\n"               
			"csrw tensor_load_l2, x5\n"         
			"addi x31, x31, 1\n"                 
			"li x5, 0x4d00000000003f \n"                 
			"add x5, x11, x5\n"               
			"csrw tensor_load_l2, x5\n"         
			"addi x31, x31, -1\n"                 
			"li x5, 0x6b00000000001f \n"                 
			"add x5, x11, x5\n"               
			"csrw tensor_load_l2, x5\n"         
			"addi x31, x31, 1\n"                 
			"li x5, 0x6c00000000001f \n"                 
			"add x5, x11, x5\n"               
			"csrw tensor_load_l2, x5\n"         
			"addi x31, x31, -1\n"                 
			"li x5, 0x6f00000000001f \n"                 
			"add x5, x11, x5\n"               
			"csrw tensor_load_l2, x5\n"         
			"addi x31, x31, 1\n"                 
			"li x5, 0x7000000000001f \n"                 
			"add x5, x11, x5\n"               
			"csrw tensor_load_l2, x5\n"         
			"addi x31, x31, -1\n"                 
			"li x5, 0x7300000000001f \n"                 
			"add x5, x11, x5\n"               
			"csrw tensor_load_l2, x5\n"         
			"addi x31, x31, 1\n"                 
			"li x5, 0x7400000000001f \n"                 
			"add x5, x11, x5\n"               
			"csrw tensor_load_l2, x5\n"         
			"addi x31, x31, -1\n"                 
			"li x5, 0x7700000000001f \n"                 
			"add x5, x11, x5\n"               
			"csrw tensor_load_l2, x5\n"         
			"addi x31, x31, 1\n"                 
			"li x5, 0x7800000000001f \n"                 
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
			"li x5, 0x7f00000000003f \n"                 
			"add x5, x11, x5\n"               
			"csrw tensor_load_l2, x5\n"         
			"addi x31, x31, 1\n"                 
			"li x5, 0x8300000000000f \n"                 
			"add x5, x11, x5\n"               
			"csrw tensor_load_l2, x5\n"         
			"addi x31, x31, -1\n"                 
			"li x5, 0x8300000000003f \n"                 
			"add x5, x11, x5\n"               
			"csrw tensor_load_l2, x5\n"         
			"addi x31, x31, 1\n"                 
			"li x5, 0x8700000000000f \n"                 
			"add x5, x11, x5\n"               
			"csrw tensor_load_l2, x5\n"         
			"addi x31, x31, -1\n"                 
			"li x5, 0x8700000000003f \n"                 
			"add x5, x11, x5\n"               
			"csrw tensor_load_l2, x5\n"         
			"addi x31, x31, 1\n"                 
			"li x5, 0x8b00000000003f \n"                 
			"add x5, x11, x5\n"               
			"csrw tensor_load_l2, x5\n"         
			"addi x31, x31, -1\n"                 
			"li x5, 0x3f00000000001f \n"                 
			"add x5, x11, x5\n"               
			"csrw tensor_load_l2, x5\n"         
			"addi x31, x31, 1\n"                 
			"li x5, 0x4300000000001f \n"                 
			"add x5, x11, x5\n"               
			"csrw tensor_load_l2, x5\n"         
			"addi x31, x31, -1\n"                 
			"li x5, 0x4700000000001f \n"                 
			"add x5, x11, x5\n"               
			"csrw tensor_load_l2, x5\n"         
			"addi x31, x31, 1\n"                 
			"li x5, 0x4700000000002f \n"                 
			"add x5, x11, x5\n"               
			"csrw tensor_load_l2, x5\n"         
			"addi x31, x31, -1\n"                 
			"li x5, 0x4b00000000001f \n"                 
			"add x5, x11, x5\n"               
			"csrw tensor_load_l2, x5\n"         
			"addi x31, x31, 1\n"                 
			"li x5, 0x4b00000000002f \n"                 
			"add x5, x11, x5\n"               
			"csrw tensor_load_l2, x5\n"         
			"addi x31, x31, -1\n"                 
			"li x5, 0x4f00000000002f \n"                 
			"add x5, x11, x5\n"               
			"csrw tensor_load_l2, x5\n"         
			"addi x31, x31, 1\n"                 
			"li x5, 0x5300000000002f \n"                 
			"add x5, x11, x5\n"               
			"csrw tensor_load_l2, x5\n"         
			"addi x31, x31, -1\n"                 
			"li x5, 0x7500000000000f \n"                 
			"add x5, x11, x5\n"               
			"csrw tensor_load_l2, x5\n"         
			"addi x31, x31, 1\n"                 
			"li x5, 0x7900000000000f \n"                 
			"add x5, x11, x5\n"               
			"csrw tensor_load_l2, x5\n"         
			"addi x31, x31, -1\n"                 
			"li x5, 0x7d00000000000f \n"                 
			"add x5, x11, x5\n"               
			"csrw tensor_load_l2, x5\n"         
			"addi x31, x31, 1\n"                 
			"li x5, 0x8100000000000f \n"                 
			"add x5, x11, x5\n"               
			"csrw tensor_load_l2, x5\n"         
			"addi x31, x31, -1\n"                 
			"csrwi tensor_wait, 0x2\n"      
			"csrwi tensor_wait, 0x3\n"      
			"fence\n"
			"LBL_TXR_INIT_HID_0:\n"          
			"li x25, 0x81617a1000 \n"                 
			"fsw.ps f21, 0(x25) \n"                 
			"fsw.ps f15, 64(x25) \n"                 
			"fsw.ps f4, 128(x25) \n"                 
			"fsw.ps f28, 192(x25) \n"                 
			"fsw.ps f29, 256(x25) \n"                 
			"fsw.ps f11, 320(x25) \n"                 
			"fsw.ps f17, 384(x25) \n"                 
			"fsw.ps f2, 448(x25) \n"                 
			"fsw.ps f3, 512(x25) \n"                 
			"fsw.ps f19, 576(x25) \n"                 
			"fsw.ps f17, 640(x25) \n"                 
			"fsw.ps f28, 704(x25) \n"                 
			"fsw.ps f24, 768(x25) \n"                 
			"li x23, 0x8161deb800 \n"                 
			"fsw.ps f10, 0(x23) \n"                 
			"fsw.ps f18, 64(x23) \n"                 
			"fsw.ps f11, 128(x23) \n"                 
			"fsw.ps f4, 192(x23) \n"                 
			"fsw.ps f26, 256(x23) \n"                 
			"li x12, 0x8163c49000 \n"                 
			"fsw.ps f0, 0(x12) \n"                 
			"fsw.ps f19, 64(x12) \n"                 
			"fsw.ps f19, 128(x12) \n"                 
			"fsw.ps f9, 192(x12) \n"                 
			"fsw.ps f11, 256(x12) \n"                 
			"fsw.ps f17, 320(x12) \n"                 
			"fsw.ps f24, 384(x12) \n"                 
			"fsw.ps f18, 448(x12) \n"                 
			"fsw.ps f15, 512(x12) \n"                 
			"fsw.ps f15, 576(x12) \n"                 
			"fsw.ps f2, 640(x12) \n"                 
			"li x11, 0x816ff6c400 \n"                 
			"fsw.ps f21, 0(x11) \n"                 
			"fsw.ps f27, 64(x11) \n"                 
			"fsw.ps f20, 128(x11) \n"                 
			"fsw.ps f2, 192(x11) \n"                 
			"fsw.ps f29, 256(x11) \n"                 
			"fsw.ps f6, 320(x11) \n"                 
			"fsw.ps f3, 384(x11) \n"                 
			"fsw.ps f8, 448(x11) \n"                 
			"fsw.ps f7, 512(x11) \n"                 
			"fsw.ps f9, 576(x11) \n"                 
			"fsw.ps f28, 640(x11) \n"                 
			"fsw.ps f26, 704(x11) \n"                 
			"fsw.ps f17, 768(x11) \n"                 
			"fsw.ps f29, 832(x11) \n"                 
			"li x22, 0x81705ed800 \n"                 
			"fsw.ps f3, 0(x22) \n"                 
			"fsw.ps f4, 64(x22) \n"                 
			"fsw.ps f15, 128(x22) \n"                 
			"fsw.ps f10, 192(x22) \n"                 
			"fsw.ps f11, 256(x22) \n"                 
			"fsw.ps f14, 320(x22) \n"                 
			"fsw.ps f18, 384(x22) \n"                 
			"fsw.ps f7, 448(x22) \n"                 
			"fsw.ps f26, 512(x22) \n"                 
			"fsw.ps f11, 576(x22) \n"                 
			"fsw.ps f28, 640(x22) \n"                 
			"fsw.ps f20, 704(x22) \n"                 
			"fsw.ps f5, 768(x22) \n"                 
			"fsw.ps f25, 832(x22) \n"                 
			"fsw.ps f17, 896(x22) \n"                 
			"fsw.ps f24, 960(x22) \n"                 
			"li x20, 0x8172be0080 \n"                 
			"fsw.ps f17, 0(x20) \n"                 
			"fsw.ps f15, 64(x20) \n"                 
			"fsw.ps f4, 128(x20) \n"                 
			"fsw.ps f29, 192(x20) \n"                 
			"fsw.ps f11, 256(x20) \n"                 
			"li x11, 0x817896e000 \n"                 
			"fsw.ps f11, 0(x11) \n"                 
			"fsw.ps f2, 64(x11) \n"                 
			"fsw.ps f0, 128(x11) \n"                 
			"fsw.ps f0, 192(x11) \n"                 
			"fsw.ps f20, 256(x11) \n"                 
			"fsw.ps f6, 320(x11) \n"                 
			"fsw.ps f22, 384(x11) \n"                 
			"fsw.ps f8, 448(x11) \n"                 
			"fsw.ps f11, 512(x11) \n"                 
			"fsw.ps f3, 576(x11) \n"                 
			"li x25, 0x817b6ff100 \n"                 
			"fsw.ps f20, 0(x25) \n"                 
			"fsw.ps f29, 64(x25) \n"                 
			"fsw.ps f16, 128(x25) \n"                 
			"fsw.ps f30, 192(x25) \n"                 
			"fsw.ps f9, 256(x25) \n"                 
			"fsw.ps f2, 320(x25) \n"                 
			"fsw.ps f16, 384(x25) \n"                 
			"fsw.ps f7, 448(x25) \n"                 
			"fsw.ps f29, 512(x25) \n"                 
			"fsw.ps f24, 576(x25) \n"                 
			"li x25, 0x817cea5000 \n"                 
			"fsw.ps f27, 0(x25) \n"                 
			"fsw.ps f28, 64(x25) \n"                 
			"fsw.ps f6, 128(x25) \n"                 
			"fsw.ps f0, 192(x25) \n"                 
			"fsw.ps f11, 256(x25) \n"                 
			"fsw.ps f30, 320(x25) \n"                 
			"fsw.ps f12, 384(x25) \n"                 
			"fsw.ps f13, 448(x25) \n"                 
			"fsw.ps f29, 512(x25) \n"                 
			"fsw.ps f26, 576(x25) \n"                 
			"fsw.ps f6, 640(x25) \n"                 
			"fsw.ps f13, 704(x25) \n"                 
			"fsw.ps f13, 768(x25) \n"                 
			"fsw.ps f12, 832(x25) \n"                 
			"fsw.ps f5, 896(x25) \n"                 
			"li x24, 0x81805ed800 \n"                 
			"fsw.ps f23, 0(x24) \n"                 
			"fsw.ps f29, 64(x24) \n"                 
			"fsw.ps f30, 128(x24) \n"                 
			"fsw.ps f2, 192(x24) \n"                 
			"fsw.ps f4, 256(x24) \n"                 
			"li x21, 0x818b6ff100 \n"                 
			"fsw.ps f30, 0(x21) \n"                 
			"fsw.ps f2, 64(x21) \n"                 
			"fsw.ps f22, 128(x21) \n"                 
			"fsw.ps f13, 192(x21) \n"                 
			"fsw.ps f1, 256(x21) \n"                 
			"fsw.ps f23, 320(x21) \n"                 
			"fsw.ps f0, 384(x21) \n"                 
			"fsw.ps f21, 448(x21) \n"                 
			"fsw.ps f6, 512(x21) \n"                 
			"fsw.ps f30, 576(x21) \n"                 
			"fsw.ps f21, 640(x21) \n"                 
			"fsw.ps f16, 704(x21) \n"                 
			"li x13, 0x81917a1000 \n"                 
			"fsw.ps f19, 0(x13) \n"                 
			"fsw.ps f19, 64(x13) \n"                 
			"fsw.ps f12, 128(x13) \n"                 
			"fsw.ps f19, 192(x13) \n"                 
			"fsw.ps f24, 256(x13) \n"                 
			"fsw.ps f18, 320(x13) \n"                 
			"fsw.ps f12, 384(x13) \n"                 
			"fsw.ps f26, 448(x13) \n"                 
			"li x15, 0x8192be0080 \n"                 
			"fsw.ps f21, 0(x15) \n"                 
			"fsw.ps f6, 64(x15) \n"                 
			"fsw.ps f25, 128(x15) \n"                 
			"fsw.ps f19, 192(x15) \n"                 
			"fsw.ps f22, 256(x15) \n"                 
			"fsw.ps f7, 320(x15) \n"                 
			"fsw.ps f3, 384(x15) \n"                 
			"fsw.ps f14, 448(x15) \n"                 
			"fsw.ps f11, 512(x15) \n"                 
			"fsw.ps f22, 576(x15) \n"                 
			"fsw.ps f28, 640(x15) \n"                 
			"fsw.ps f15, 704(x15) \n"                 
			"fsw.ps f11, 768(x15) \n"                 
			"fsw.ps f9, 832(x15) \n"                 
			"fsw.ps f0, 896(x15) \n"                 
			"li x17, 0x8193ad2f40 \n"                 
			"fsw.ps f18, 0(x17) \n"                 
			"fsw.ps f8, 64(x17) \n"                 
			"fsw.ps f12, 128(x17) \n"                 
			"fsw.ps f3, 192(x17) \n"                 
			"fsw.ps f12, 256(x17) \n"                 
			"fsw.ps f16, 320(x17) \n"                 
			"fsw.ps f12, 384(x17) \n"                 
			"fsw.ps f11, 448(x17) \n"                 
			"li x14, 0x819ff6c400 \n"                 
			"fsw.ps f9, 0(x14) \n"                 
			"fsw.ps f25, 64(x14) \n"                 
			"fsw.ps f20, 128(x14) \n"                 
			"fsw.ps f31, 192(x14) \n"                 
			"fsw.ps f1, 256(x14) \n"                 
			"li x13, 0x8263861f40 \n"                 
			"fsw.ps f3, 0(x13) \n"                 
			"fsw.ps f31, 64(x13) \n"                 
			"fsw.ps f28, 128(x13) \n"                 
			"fsw.ps f30, 192(x13) \n"                 
			"fsw.ps f4, 256(x13) \n"                 
			"fsw.ps f15, 320(x13) \n"                 
			"fsw.ps f4, 384(x13) \n"                 
			"fsw.ps f19, 448(x13) \n"                 
			"fsw.ps f23, 512(x13) \n"                 
			"li x12, 0x82729f7000 \n"                 
			"fsw.ps f7, 0(x12) \n"                 
			"fsw.ps f16, 64(x12) \n"                 
			"fsw.ps f27, 128(x12) \n"                 
			"fsw.ps f8, 192(x12) \n"                 
			"fsw.ps f16, 256(x12) \n"                 
			"fsw.ps f29, 320(x12) \n"                 
			"fsw.ps f15, 384(x12) \n"                 
			"fsw.ps f13, 448(x12) \n"                 
			"fsw.ps f28, 512(x12) \n"                 
			"li x10, 0x8273860a00 \n"                 
			"fsw.ps f27, 0(x10) \n"                 
			"fsw.ps f30, 64(x10) \n"                 
			"fsw.ps f22, 128(x10) \n"                 
			"fsw.ps f11, 192(x10) \n"                 
			"fsw.ps f6, 256(x10) \n"                 
			"fsw.ps f24, 320(x10) \n"                 
			"fsw.ps f24, 384(x10) \n"                 
			"fsw.ps f7, 448(x10) \n"                 
			"fsw.ps f20, 512(x10) \n"                 
			"fsw.ps f30, 576(x10) \n"                 
			"li x14, 0x8279bee800 \n"                 
			"fsw.ps f4, 0(x14) \n"                 
			"fsw.ps f3, 64(x14) \n"                 
			"fsw.ps f5, 128(x14) \n"                 
			"fsw.ps f26, 192(x14) \n"                 
			"fsw.ps f29, 256(x14) \n"                 
			"fsw.ps f7, 320(x14) \n"                 
			"fsw.ps f29, 384(x14) \n"                 
			"li x21, 0x8283860a00 \n"                 
			"fsw.ps f31, 0(x21) \n"                 
			"fsw.ps f15, 64(x21) \n"                 
			"fsw.ps f18, 128(x21) \n"                 
			"fsw.ps f1, 192(x21) \n"                 
			"fsw.ps f31, 256(x21) \n"                 
			"fsw.ps f5, 320(x21) \n"                 
			"fsw.ps f21, 384(x21) \n"                 
			"fsw.ps f30, 448(x21) \n"                 
			"fsw.ps f9, 512(x21) \n"                 
			"fsw.ps f16, 576(x21) \n"                 
			"li x16, 0x8289bee800 \n"                 
			"fsw.ps f1, 0(x16) \n"                 
			"fsw.ps f10, 64(x16) \n"                 
			"fsw.ps f10, 128(x16) \n"                 
			"fsw.ps f11, 192(x16) \n"                 
			"fsw.ps f18, 256(x16) \n"                 
			"fsw.ps f27, 320(x16) \n"                 
			"fsw.ps f16, 384(x16) \n"                 
			"fsw.ps f16, 448(x16) \n"                 
			"li x25, 0x82929f7000 \n"                 
			"fsw.ps f28, 0(x25) \n"                 
			"fsw.ps f23, 64(x25) \n"                 
			"fsw.ps f24, 128(x25) \n"                 
			"fsw.ps f0, 192(x25) \n"                 
			"fsw.ps f27, 256(x25) \n"                 
			"fsw.ps f6, 320(x25) \n"                 
			"fsw.ps f4, 384(x25) \n"                 
			"fsw.ps f15, 448(x25) \n"                 
			"li x23, 0x829d47ef40 \n"                 
			"fsw.ps f30, 0(x23) \n"                 
			"fsw.ps f17, 64(x23) \n"                 
			"fsw.ps f20, 128(x23) \n"                 
			"fsw.ps f5, 192(x23) \n"                 
			"fsw.ps f21, 256(x23) \n"                 
			"fsw.ps f14, 320(x23) \n"                 
			"fsw.ps f27, 384(x23) \n"                 
			"fence\n"
			"fence\n"
			"csrw fflags, zero\n"
			"csrwi frm, 0x0\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_mask, zero\n"
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
			"LBL_KERNEL_HID_0:\n"          
			"li x5, 0xFEEDFEED\n"
			"li x0, 0x5633a408ad\n" 
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
			"LBL_SEQID_0_FP_AMO_ID_0_HID_0:\n"
			"li x9, 0x814035a03c\n" 
			"la x12, mem_ptr_offset\n"
			"flw.ps f14, 832(x12)\n"
			"famomaxl.pi f23, f14 (x9)\n" 
			"fnmsub.ps f9, f16, f0, f9, rdn\n"
			"fsub.pi f9, f31, f9\n"
			"fmax.ps f16, f7, f16\n"
			"bgeu x16, x18, 1f\n"
			"1:\n"
			"fmadd.ps f31, f31, f3, f7, rmm\n"
			"bne x3, x12, 1f\n"
			"1:\n"
			"fand.pi f18, f20, f11\n"
			"fsub.ps f16, f0, f19, rne\n"
			"fnmadd.s f3, f14, f31, f14, rdn\n"
			"fsat8.pi f23, f7\n"
			"fsatu8.pi f23, f31\n"
			"LBL_SEQID_0_FP_L_STORE_ID_1_HID_0:\n"
			"li x13, 0x824a9a0000\n" 
			"mov.m.x m0, zero, 0xff\n"
			"fswl.ps f14, (x13)\n" 
			"fsll.pi f31, f16, f14\n"
			"fsat8.pi f19, f9\n"
			"fnmadd.s f13, f19, f19, f13, rtz\n"
			"fmax.pi f19, f19, f28\n"
			"frcp.ps f5, f9\n"
			"fsrl.pi f3, f16, f11\n"
			"fsin.ps f14, f3\n"
			"fmax.ps f11, f11, f28\n"
			"fsgnjn.ps f20, f3, f19\n"
			"bne x3, x15, 1f\n"
			"1:\n"
			"LBL_SEQID_0_FP_AMO_ID_2_HID_0:\n"
			"li x18, 0x8152be00ac\n" 
			"la x28, mem_ptr_offset\n"
			"flw.ps f16, 256(x28)\n"
			"famomaxul.pi f16, f16 (x18)\n" 
			"fpackrepb.pi f3, f23\n"
			"fminu.pi f16, f23, f23\n"
			"fcvt.pwu.ps f3, f20, rdn\n"
			"fnmadd.s f26, f13, f20, f9, rtz\n"
			"bltu x5, x14, 1f\n"
			"1:\n"
			"fsgnjn.ps f9, f9, f9\n"
			"fmin.s f26, f26, f26\n"
			"fslli.pi f20, f23, 0xa\n"
			"bltu x11, x21, 1f\n"
			"1:\n"
			"fltu.pi f11, f11, f23\n"
			"blt x5, x21, 1f\n"
			"1:\n"
			"fmax.pi f13, f13, f13\n"
			"bge x11, x30, 1f\n"
			"1:\n"
			"LBL_SEQID_0_CSR_ID_3_HID_0:\n"
			"csrr x0, tdata1\n"
			"fexp.ps f26, f23\n"
			"flt.ps f5, f23, f18\n"
			"fadd.ps f18, f18, f31, rmm\n"
			"maskxor m0, m2, m6\n"
			"bne x1, x28, 1f\n"
			"1:\n"
			"fsatu8.pi f26, f26\n"
			"fnmadd.s f18, f18, f18, f31, rmm\n"
			"fsrai.pi f23, f23, 0xb\n"
			"fcvt.f16.ps f26, f7\n"
			"bge x7, x4, 1f\n"
			"1:\n"
			"fcvt.ps.pw f23, f23, rne\n"
			"bne x25, x31, 1f\n"
			"1:\n"
			"LBL_SEQID_0_INT_LOAD_ID_4_HID_0:\n"
			"li x11, 0x82029f703c\n" 
			"sd x9, 1805(x11)\n"
			"lwu x17, 1805(x11)\n" 
			"fcmovm.ps f9, f9, f31\n"
			"bne x7, x24, 1f\n"
			"1:\n"
			"fclass.ps f28, f31\n"
			"fpackreph.pi f16, f28\n"
			"fcvt.ps.pwu f19, f20, rne\n"
			"fsub.ps f14, f16, f16, rup\n"
			"fexp.ps f13, f16\n"
			"fsin.ps f0, f16\n"
			"fnmadd.ps f20, f28, f20, f9, rne\n"
			"fnmadd.ps f5, f31, f19, f20, rdn\n"
			"LBL_SEQID_0_INT_AMO_ID_5_HID_0:\n"
			"li x11, 0x8244f600a8\n" 
			"amomaxul.d x17, x12, (x11)\n" 
			"fadd.pi f28, f28, f28\n"
			"fsgnjn.s f0, f18, f16\n"
			"bltu x30, x8, 1f\n"
			"1:\n"
			"fsra.pi f28, f14, f18\n"
			"fmadd.s f14, f16, f14, f14, rmm\n"
			"fcvt.ps.pwu f28, f28, rne\n"
			"fsrli.pi f19, f20, 0x6\n"
			"fmul.pi f18, f18, f18\n"
			"fpackreph.pi f9, f9\n"
			"fsgnj.ps f19, f14, f18\n"
			"LBL_SEQID_0_INT_AMO_ID_6_HID_0:\n"
			"li x13, 0x814035a038\n" 
			"amoorl.d x28, x28, (x13)\n" 
			"fclass.ps f31, f16\n"
			"fsin.ps f0, f5\n"
			"blt x21, x16, 1f\n"
			"1:\n"
			"fsrli.pi f3, f3, 0x5\n"
			"flt.pi f11, f18, f11\n"
			"fround.ps f3, f20, rmm\n"
			"frcp.ps f31, f14\n"
			"fmadd.ps f16, f9, f19, f16, rtz\n"
			"fswizz.ps f13, f20, 0x19\n"
			"fsll.pi f26, f9, f26\n"
			"LBL_SEQID_0_INT_AMO_ID_7_HID_0:\n"
			"li x18, 0x82359407a4\n" 
			"amomaxug.w x12, x13, (x18)\n" 
			"fsub.ps f23, f11, f14, rne\n"
			"fltm.pi m0, f11, f3\n"
			"fslli.pi f19, f19, 0xc\n"
			"feqm.ps m3, f0, f3\n"
			"for.pi f19, f19, f3\n"
			"or x7, x7, x7\n"
			"beq x13, x23, 1f\n"
			"1:\n"
			"fmadd.ps f13, f20, f20, f20, rdn\n"
			"fadd.s f0, f3, f31, rmm\n"
			"fcmov.ps f19, f11, f7, f11\n"
			"LBL_SEQID_0_INT_LOAD_ID_8_HID_0:\n"
			"li x11, 0x82032c003e\n" 
			"sd x7, 597(x11)\n"
			"lh x11, 597(x11)\n" 
			"li x5, 0x7fb1\n"
			"csrw tensor_mask, x5\n"	
			"faddi.pi f5, f28, 0xab\n"
			"fswizz.ps f11, f7, 0x16\n"
			"bgeu x7, x19, 1f\n"
			"1:\n"
			"fadd.pi f16, f14, f20\n"
			"fslli.pi f19, f19, 0x1\n"
			"fmaxu.pi f5, f5, f11\n"
			"feq.ps f23, f7, f11\n"
			"fle.ps f28, f28, f13\n"
			"fcvt.pwu.ps f13, f13, rmm\n"
			"fmsub.s f11, f20, f14, f9, rtz\n"
			"LBL_SEQID_0_INT_AMO_ID_9_HID_0:\n"
			"li x10, 0x815cea5030\n" 
			"amoxorl.d x7, x7, (x10)\n" 
			"fle.pi f23, f23, f0\n"
			"fcvt.ps.f16 f26, f19\n"
			"fsgnj.s f23, f23, f20\n"
			"fround.ps f9, f16, rtz\n"
			"fmadd.s f19, f19, f20, f23, rtz\n"
			"fand.pi f9, f26, f26\n"
			"fsrai.pi f20, f26, 0x1\n"
			"frsq.ps f9, f0\n"
			"fsgnjx.s f7, f13, f19\n"
			"LBL_SEQID_0_FP_AMO_ID_10_HID_0:\n"
			"li x19, 0x8243de0000\n" 
			"la x7, mem_ptr_offset\n"
			"flw.ps f31, 640(x7)\n"
			"famoorl.pi f3, f31 (x19)\n" 
			"fmsub.ps f23, f19, f5, f5, rne\n"
			"fcmovm.ps f26, f9, f5\n"
			"bne x17, x7, 1f\n"
			"1:\n"
			"fnmsub.ps f13, f7, f13, f14, rmm\n"
			"fnot.pi f7, f31\n"
			"fmsub.s f23, f16, f23, f9, rtz\n"
			"fcmovm.ps f3, f16, f5\n"
			"fsub.pi f20, f14, f5\n"
			"flog.ps f20, f0\n"
			"fadd.s f14, f28, f26, rmm\n"
			"LBL_SEQID_0_INT_AMO_ID_11_HID_0:\n"
			"li x9, 0x81516a0004\n" 
			"amomaxul.w x19, x13, (x9)\n" 
			"lui x12, 0x16\n"
			"fbci.ps f28, 0x41c4c\n"
			"sub x7, x7, x19\n"
			"fle.s x19, f14, f5\n"
			"fclass.ps f26, f5\n"
			"fpackrepb.pi f31, f14\n"
			"lui x12, 0x9\n"
			"mov.m.x m6, x7, 0x5b\n"
			"fence\n"
			"LBL_SEQID_0_FP_AMO_ID_12_HID_0:\n"
			"li x9, 0x8132be00ac\n" 
			"la x7, mem_ptr_offset\n"
			"flw.ps f0, 224(x7)\n"
			"famominug.pi f5, f0 (x9)\n" 
			"fround.ps f0, f20, rmm\n"
			"or x12, x18, x11\n"
			"slli x28, x19, 2\n"
			"ecall\n"
			"addw x7, x18, x11\n"
			"remu x7, x17, x12\n"
			"maskpopc x7, m6\n"
			"fmv.s.x f28, x0\n"
			"fmax.ps f20, f13, f20\n"
			"LBL_SEQID_0_FP_G_GATHER_ID_13_HID_0:\n"
			"li x19, 0x8239640028\n" 
			"la x10, mem_ptr_offset\n"
			"flq2 f16, 220(x10)\n"
			"fscwg.ps f23, f16 (x19)\n"
			"fgwg.ps f14, f16 (x19)\n" 
			"fcvt.ps.pwu f28, f3, rup\n"
			"fsub.pi f3, f3, f9\n"
			"fcvt.ps.pwu f5, f5, rtz\n"
			"fle.ps f5, f5, f18\n"
			"bge x19, x28, 1f\n"
			"1:\n"
			"fminu.pi f0, f13, f20\n"
			"fpackreph.pi f3, f7\n"
			"fmsub.s f7, f26, f7, f16, rtz\n"
			"fmul.ps f7, f28, f3, rne\n"
			"fmsub.s f5, f9, f9, f7, rmm\n"
			"LBL_SEQID_0_INT_AMO_ID_14_HID_0:\n"
			"li x13, 0x81456400b4\n" 
			"amomaxul.w x7, x17, (x13)\n" 
			"fcvt.ps.pw f31, f14, rmm\n"
			"sraiw x0, x7, 5\n"
			"fmsub.s f9, f0, f5, f7, rmm\n"
			"fsra.pi f19, f11, f19\n"
			"fmadd.s f16, f16, f16, f0, rne\n"
			"mov.m.x m0, x12, 0x12\n"
			"mov.m.x m0, x0, 0xff\n"
			"fsgnj.s f0, f5, f16\n"
			"fmadd.ps f31, f31, f31, f7, rtz\n"
			"slti x17, x18, 0x0\n"
			"bge x22, x2, 1f\n"
			"1:\n"
			"LBL_SEQID_0_INT_AMO_ID_15_HID_0:\n"
			"li x12, 0x825d47ef70\n" 
			"amominul.d x19, x10, (x12)\n" 
			"li x5, 0x41f2ab59eb912c6e\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0x3279574c9234d728\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"fle.ps f26, f16, f20\n"
			"blt x21, x28, 1f\n"
			"1:\n"
			"fmul.ps f0, f0, f26, rtz\n"
			"beq x5, x3, 1f\n"
			"1:\n"
			"fmadd.s f20, f23, f13, f19, rne\n"
			"fpackrepb.pi f7, f13\n"
			"fmax.ps f5, f5, f19\n"
			"ffrc.ps f5, f13\n"
			"fnmsub.s f3, f23, f26, f26, rmm\n"
			"fnot.pi f11, f20\n"
			"bge x22, x7, 1f\n"
			"1:\n"
			"fcvt.ps.pw f13, f20, rtz\n"
			"LBL_SEQID_0_FP_AMO_ID_16_HID_0:\n"
			"li x18, 0x8244a20010\n" 
			"la x17, mem_ptr_offset\n"
			"flw.ps f18, 160(x17)\n"
			"famominul.pi f3, f18 (x18)\n" 
			"fmax.pi f13, f11, f20\n"
			"fand.pi f13, f13, f20\n"
			"fmul.ps f11, f11, f19, rne\n"
			"fslli.pi f19, f13, 0x2\n"
			"fsrai.pi f20, f7, 0xa\n"
			"frsq.ps f13, f11\n"
			"beq x18, x29, 1f\n"
			"1:\n"
			"fmadd.ps f5, f20, f13, f26, rtz\n"
			"fsrai.pi f26, f26, 0x3\n"
			"fmulh.pi f20, f20, f20\n"
			"LBL_SEQID_0_FP_SCATTER_ID_17_HID_0:\n"
			"li x18, 0x810fea0776\n" 
			"la x19, mem_ptr_offset\n"
			"flq2 f0, 140(x19)\n"
			"fscw.ps f19, f0 (x18)\n" 
			"li x5, 0x6c7f\n"
			"csrw tensor_mask, x5\n"	
			"fnmsub.s f5, f19, f0, f13, rdn\n"
			"fsra.pi f19, f11, f5\n"
			"fmul.ps f16, f28, f11, rdn\n"
			"bne x1, x27, 1f\n"
			"1:\n"
			"fsrli.pi f16, f3, 0x2\n"
			"fcmovm.ps f23, f5, f28\n"
			"fsgnjx.ps f18, f16, f3\n"
			"fnmsub.s f13, f13, f31, f23, rne\n"
			"fmadd.s f31, f31, f0, f19, rmm\n"
			"fand.pi f13, f28, f13\n"
			"bltu x23, x5, 1f\n"
			"1:\n"
			"LBL_SEQID_0_CSR_ID_18_HID_0:\n"
			"li x9, 0x252900817b6ff100\n"
			"li x31, (0x1 << 6)\n"
			"csrw tensor_store, x9\n" 
			"csrwi tensor_wait, 8\n" 
			"flt.pi f16, f11, f0\n"
			"fsin.ps f11, f3\n"
			"bgeu x3, x17, 1f\n"
			"1:\n"
			"sub x11, x11, x13\n"
			"slli x28, x7, 5\n"
			"mov.m.x m3, x28, 0x8\n"
			"fadd.s f14, f20, f11, rtz\n"
			"fmul.s f19, f19, f20, rup\n"
			"srl x19, x19, x17\n"
			"fadd.ps f28, f5, f5, rdn\n"
			"LBL_SEQID_0_FP_LOAD_ID_19_HID_0:\n"
			"li x18, 0x8204a20013\n" 
			"fsq2 f20, -1875(x18)\n"
			"fbc.ps f20, -1875(x18)\n" 
			"fsub.s f20, f20, f0, rdn\n"
			"bgeu x30, x19, 1f\n"
			"1:\n"
			"fnmsub.ps f5, f31, f19, f26, rne\n"
			"andi x28, x18, 0x4\n"
			"fence\n"
			"slt x9, x19, x9\n"
			"bgeu x22, x20, 1f\n"
			"1:\n"
			"fcvt.f16.ps f7, f26\n"
			"bgeu x3, x26, 1f\n"
			"1:\n"
			"fmul.ps f14, f14, f19, rmm\n"
			"beq x29, x17, 1f\n"
			"1:\n"
			"fclass.s x12, f13\n"
			"fnmsub.ps f14, f9, f31, f18, rtz\n"
			"beq x3, x10, 1f\n"
			"1:\n"
			"LBL_SEQID_0_CSR_ID_20_HID_0:\n"
			"csrr x0, marchid\n"
			"ffrc.ps f14, f20\n"
			"fle.ps f7, f28, f28\n"
			"blt x16, x13, 1f\n"
			"1:\n"
			"fnmsub.s f31, f14, f28, f16, rmm\n"
			"fcvt.pwu.ps f23, f18, rup\n"
			"blt x27, x7, 1f\n"
			"1:\n"
			"fle.ps f9, f11, f20\n"
			"fsra.pi f16, f16, f19\n"
			"flog.ps f14, f14\n"
			"blt x8, x29, 1f\n"
			"1:\n"
			"fpackreph.pi f18, f14\n"
			"fmax.ps f14, f14, f9\n"
			"LBL_SEQID_0_FP_AMO_ID_21_HID_0:\n"
			"li x7, 0x8153ad2f4c\n" 
			"la x9, mem_ptr_offset\n"
			"flw.ps f26, 288(x9)\n"
			"famoaddl.pi f14, f26 (x7)\n" 
			"fandi.pi f5, f19, 0x86\n"
			"fnot.pi f0, f20\n"
			"fmadd.ps f20, f7, f18, f23, rtz\n"
			"fmin.ps f23, f16, f13\n"
			"fcvt.f16.ps f14, f16\n"
			"bge x24, x17, 1f\n"
			"1:\n"
			"fsatu8.pi f5, f3\n"
			"fsgnjx.s f23, f31, f19\n"
			"fsub.pi f9, f20, f9\n"
			"fcvt.ps.pw f28, f16, rtz\n"
			"LBL_SEQID_0_INT_AMO_ID_22_HID_0:\n"
			"li x9, 0x8257600750\n" 
			"amoxorl.d x19, x12, (x9)\n" 
			"fround.ps f7, f14, rdn\n"
			"fclass.s x17, f11\n"
			"fmv.x.s x7, f28\n"
			"fnot.pi f28, f28\n"
			"ffrc.ps f31, f7\n"
			"slli x18, x18, 0\n"
			"maskor m6, m0, m2\n"
			"beq x6, x0, 1f\n"
			"1:\n"
			"fcvt.f16.ps f18, f7\n"
			"fcvt.wu.s x18, f19, rup\n"
			"LBL_SEQID_0_FP_AMO_ID_23_HID_0:\n"
			"li x11, 0x813fea0774\n" 
			"la x7, mem_ptr_offset\n"
			"flw.ps f7, 864(x7)\n"
			"famominug.pi f11, f7 (x11)\n" 
			"fxor.pi f13, f31, f31\n"
			"for.pi f20, f26, f20\n"
			"fadd.s f31, f31, f7, rne\n"
			"bge x4, x7, 1f\n"
			"1:\n"
			"fand.pi f9, f9, f3\n"
			"fcvt.f16.ps f7, f7\n"
			"fsgnjx.s f11, f7, f3\n"
			"fmax.pi f16, f26, f7\n"
			"blt x3, x17, 1f\n"
			"1:\n"
			"faddi.pi f0, f31, 0x1d9\n"
			"bge x18, x4, 1f\n"
			"1:\n"
			"fmax.ps f13, f20, f14\n"
			"LBL_SEQID_0_FP_L_GATHER_ID_24_HID_0:\n"
			"li x18, 0x8241a0ffb8\n" 
			"la x7, mem_ptr_offset\n"
			"flq2 f7, 32(x7)\n"
			"fscwl.ps f7, f7 (x18)\n"
			"fgbl.ps f11, f7 (x18)\n" 
			"auipc x11, 0x1a\n"
			"sllw x18, x28, x18\n"
			"fpackreph.pi f19, f28\n"
			"bltu x3, x23, 1f\n"
			"1:\n"
			"fswizz.ps f20, f16, 0x6\n"
			"flem.ps m0, f14, f16\n"
			"sraiw x17, x12, 0\n"
			"mova.m.x x10\n"
			"fand.pi f23, f18, f16\n"
			"fround.ps f20, f3, rtz\n"
			"bgeu x25, x19, 1f\n"
			"1:\n"
			"LBL_SEQID_0_FP_L_LOAD_ID_25_HID_0:\n"
			"li x17, 0x8247d00740\n" 
			"mov.m.x m0, zero, 0xff\n"
			"fswl.ps f31, (x17)\n"
			"flwl.ps f23, (x17)\n" 
			"sll x17, x0, x0\n"
			"ebreak\n"
			"for.pi f28, f20, f9\n"
			"addw x18, x12, x28\n"
			"bltu x5, x30, 1f\n"
			"1:\n"
			"divu x17, x10, x18\n"
			"subw x7, x10, x17\n"
			"sraw x18, x0, x12\n"
			"bge x25, x3, 1f\n"
			"1:\n"
			"fsat8.pi f11, f13\n"
			"maskpopc x13, m2\n"
			"LBL_SEQID_0_FP_GATHER_ID_26_HID_0:\n"
			"li x9, 0x810cea5034\n" 
			"la x12, mem_ptr_offset\n"
			"flq2 f0, 44(x12)\n"
			"fscw.ps f27, f0 (x9)\n"
			"fgb.ps f3, f0 (x9)\n" 
			"fpackrepb.pi f14, f11\n"
			"beq x11, x29, 1f\n"
			"1:\n"
			"fsub.s f7, f7, f11, rmm\n"
			"fmaxu.pi f26, f0, f18\n"
			"bgeu x11, x10, 1f\n"
			"1:\n"
			"fsgnjn.s f16, f7, f18\n"
			"fmul.pi f18, f18, f0\n"
			"fswizz.ps f14, f14, 0x5\n"
			"bge x1, x9, 1f\n"
			"1:\n"
			"fcvt.ps.pwu f14, f0, rdn\n"
			"fslli.pi f5, f14, 0x5\n"
			"bltu x4, x25, 1f\n"
			"1:\n"
			"fround.ps f26, f14, rup\n"
			"LBL_SEQID_0_INT_AMO_ID_27_HID_0:\n"
			"li x18, 0x81517a1020\n" 
			"amoxorl.d x19, x28, (x18)\n" 
			"fsub.pi f20, f31, f14\n"
			"fsgnjx.ps f13, f16, f31\n"
			"fcvt.ps.f16 f18, f0\n"
			"bge x26, x3, 1f\n"
			"1:\n"
			"fclass.ps f31, f31\n"
			"frcp.ps f18, f18\n"
			"flog.ps f9, f9\n"
			"fmin.pi f7, f23, f19\n"
			"fxor.pi f3, f20, f9\n"
			"fsgnj.s f5, f31, f7\n"
			"LBL_SEQID_0_INT_AMO_ID_28_HID_0:\n"
			"li x12, 0x8140d006c0\n" 
			"amoaddl.d x9, x10, (x12)\n" 
			"fcvt.wu.s x9, f23, rne\n"
			"fcvt.ps.f16 f18, f23\n"
			"fand.pi f7, f7, f5\n"
			"fpackreph.pi f16, f16\n"
			"divw x11, x7, x11\n"
			"beq x26, x30, 1f\n"
			"1:\n"
			"fclass.s x19, f31\n"
			"bne x4, x11, 1f\n"
			"1:\n"
			"xori x7, x28, 0xb\n"
			"fadd.s f7, f19, f19, rup\n"
			"bltu x27, x15, 1f\n"
			"1:\n"
			"faddi.pi f14, f23, 0x118\n"
			"LBL_SEQID_0_FP_G_STORE_ID_29_HID_0:\n"
			"li x10, 0x823a78e020\n" 
			"mov.m.x m0, zero, 0xff\n"
			"fswg.ps f31, (x10)\n" 
			"fround.ps f20, f14, rdn\n"
			"fswizz.ps f23, f23, 0x19\n"
			"bge x20, x15, 1f\n"
			"1:\n"
			"fmulh.pi f13, f20, f26\n"
			"fcmov.ps f19, f23, f18, f31\n"
			"bge x3, x16, 1f\n"
			"1:\n"
			"fadd.ps f31, f23, f14, rtz\n"
			"fxor.pi f31, f7, f11\n"
			"faddi.pi f26, f16, 0x2b\n"
			"fmulhu.pi f7, f16, f7\n"
			"fminu.pi f11, f20, f20\n"
			"LBL_SEQID_0_FP_STORE_ID_30_HID_0:\n"
			"li x12, 0x8103c49008\n" 
			"fsq2 f18, -593(x12)\n" 
			"fcvt.ps.pwu f3, f3, rne\n"
			"fcvt.f16.ps f20, f3\n"
			"fmul.pi f18, f9, f7\n"
			"frsq.ps f7, f19\n"
			"flt.ps f3, f9, f3\n"
			"frcp.ps f28, f9\n"
			"bge x9, x30, 1f\n"
			"1:\n"
			"fadd.s f3, f11, f19, rup\n"
			"blt x18, x23, 1f\n"
			"1:\n"
			"fmulh.pi f23, f11, f23\n"
			"fnmadd.ps f0, f23, f7, f19, rdn\n"
			"blt x23, x7, 1f\n"
			"1:\n"
			"LBL_SEQID_0_INT_AMO_ID_31_HID_0:\n"
			"li x11, 0x813fea0774\n" 
			"amomaxug.w x18, x10, (x11)\n" 
			"fmin.ps f23, f9, f28\n"
			"fexp.ps f9, f16\n"
			"fcvt.ps.pw f9, f19, rtz\n"
			"fsrl.pi f13, f20, f28\n"
			"fmulhu.pi f5, f13, f31\n"
			"fpackreph.pi f11, f3\n"
			"fsub.ps f3, f11, f19, rmm\n"
			"fnmadd.ps f28, f9, f31, f31, rtz\n"
			"fmul.ps f16, f16, f16, rne\n"
			"csrw tensor_error, zero\n" 
			"fand.pi f16, f16, f3\n"
			"bge x1, x24, 1f\n"
			"1:\n"
			"fsgnjx.s f7, f0, f7\n"
			"srliw x19, x11, 3\n"
			"fltm.ps m3, f11, f11\n"
			"fmaxu.pi f7, f23, f7\n"
			"fsatu8.pi f19, f0\n"
			"fcvt.wu.s x9, f26, rmm\n"
			"srl x0, x10, x18\n"
			"beq x21, x31, 1f\n"
			"1:\n"
			"fmin.ps f9, f16, f19\n"
			"fand.pi f3, f14, f14\n"
			"bne x22, x29, 1f\n"
			"1:\n"
			"fsatu8.pi f0, f0\n"
			"fsrai.pi f0, f20, 0xd\n"
			"fcvt.ps.f16 f20, f20\n"
			"bge x2, x18, 1f\n"
			"1:\n"
			"fcvt.f16.ps f3, f20\n"
			"frcp.ps f0, f14\n"
			"fadd.s f16, f14, f0, rup\n"
			"fand.pi f9, f13, f9\n"
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ITER_0_HID_0:\n"
			"li x5, ((0x0 << TENSOR_LOAD_I16_USE_TMASK) | (0x0 << TENSOR_LOAD_I16_USE_COOP) | (0x2 << TENSOR_LOAD_I16_OPC_010) | (0x7 << TENSOR_LOAD_I16_DST_START) | (0x205f3a940 << TENSOR_LOAD_I16_VADDR) | (0x0 << TENSOR_LOAD_I16_OFFSET) | (0xb << TENSOR_LOAD_I16_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_SETUP_B_USE_COOP) | (0x0 << TENSOR_LOAD_SETUP_B_OPC_000) | (0x1 << TENSOR_LOAD_SETUP_B_OPC_1) | (0x20001ff << TENSOR_LOAD_SETUP_B_VADDR) | (0xb << TENSOR_LOAD_SETUP_B_NUM_LINES) | 0 )\n" 
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x3 << TENSOR_FMA16_B_NUM_COLS) | (0xb << TENSOR_FMA16_A_NUM_ROWS) | (0xb << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x1 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1f << TENSOR_FMA16_SCP_LOC_B) | (0x7 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_0\n"
			"csrw tensor_error, x0\n"
			"mov.m.x m0, x0, 0xFF\n"
			"li x5, 0x8\n"
			"la x31, rand_ieee754_32\n"
			"add x31, x31, x5\n"
			"flw.ps f0, 0(x31)\n"
			"flw.ps f11, 3(x31)\n"
			"flw.ps f19, 5(x31)\n"
			"flw.ps f26, 8(x31)\n"
			"la x31, rand_int_32\n"
			"add x31, x31, x5\n"
			"lw x0, 0(x31)\n"
			"lw x10, 1(x31)\n"
			"lw x13, 6(x31)\n"
			"lw x18, 3(x31)\n"
			"lw x9, 0(x31)\n"
			"li x21, 0x1\n"
			"LBL_SEQID_0_LOOP_SEQ_HID_0:\n"
			"srlw x28, x18, x13\n"
			"mova.m.x x10\n"
			"ori x12, x0, 0x10\n"
			"packb x18, x18, x0\n"
			"fmul.pi f26, f26, f11\n"
			"fmin.pi f26, f19, f26\n"
			"fmul.ps f31, f26, f0, rne\n"
			"fpackrepb.pi f31, f19\n"
			"fmsub.ps f31, f0, f0, f26, rne\n"
			"feq.pi f5, f26, f26\n"
			"fmv.s.x f5, x13\n"
			"fsgnjx.s f5, f26, f26\n"
			"fsub.ps f5, f19, f11, rup\n"
			"bltu x4, x19, 1f\n"
			"1:\n"
			"mulhsu x12, x10, x0\n"
			"beq x7, x27, 1f\n"
			"1:\n"
			"sra x13, x10, x13\n"
			"sllw x0, x18, x13\n"
			"fence\n"
			"mulhu x0, x9, x0\n"
			"bge x15, x13, 1f\n"
			"1:\n"
			"fclass.ps f9, f0\n"
			"ebreak\n"
			"fltm.pi m3, f0, f19\n"
			"packb x28, x13, x18\n"
			"fcvt.s.w f9, x0, rtz\n"
			"bgeu x13, x23, 1f\n"
			"1:\n"
			"ffrc.ps f0, f0\n"
			"fmv.s.x f0, x10\n"
			"flem.ps m2, f26, f26\n"
			"div x18, x10, x18\n"
			"fcvt.pw.ps f31, f11, rup\n"
			"bge x0, x19, 1f\n"
			"1:\n"
			"fsgnj.s f9, f26, f11\n"
			"fsrai.pi f5, f11, 0x6\n"
			"divu x12, x0, x10\n"
			"divu x9, x9, x9\n"
			"addiw x28, x10, 0x1\n"
			"fnmsub.s f5, f11, f11, f0, rdn\n"
			"fsrai.pi f9, f26, 0x5\n"
			"fmul.pi f0, f19, f11\n"
			"fcvt.s.wu f9, x0, rdn\n"
			"maskpopcz x13, m3\n"
			"fmul.pi f0, f26, f0\n"
			"fnmadd.s f9, f11, f26, f0, rmm\n"
			"beq x26, x24, 1f\n"
			"1:\n"
			"add x18, x18, x9\n"
			"fpackreph.pi f19, f11\n"
			"bgeu x6, x27, 1f\n"
			"1:\n"
			"fminu.pi f9, f0, f19\n"
			"blt x23, x19, 1f\n"
			"1:\n"
			"maskpopcz x12, m0\n"
			"fmsub.ps f9, f0, f26, f26, rtz\n"
			"fsgnj.s f9, f11, f26\n"
			"beq x3, x0, 1f\n"
			"1:\n"
			"add x0, x13, x10\n"
			"fmin.ps f5, f19, f11\n"
			"fsrai.pi f9, f26, 0x7\n"
			"beq x28, x31, 1f\n"
			"1:\n"
			"fcmovm.ps f11, f11, f11\n"
			"maskor m2, m2, m2\n"
			"fpackreph.pi f31, f19\n"
			"fmax.s f9, f19, f11\n"
			"maskpopcz x9, m0\n"
			"mov.m.x m0, x0, 0xff\n"
			"bge x19, x29, 1f\n"
			"1:\n"
			"ori x18, x9, 0x1c\n"
			"fclass.s x9, f26\n"
			"bgeu x25, x11, 1f\n"
			"1:\n"
			"sltu x0, x0, x9\n"
			"bltu x14, x19, 1f\n"
			"1:\n"
			"flt.ps f11, f11, f11\n"
			"fnmsub.s f26, f26, f11, f19, rmm\n"
			"fcvt.f16.ps f5, f11\n"
			"fsgnjx.s f31, f11, f0\n"
			"xori x0, x13, 0x0\n"
			"fswizz.ps f5, f0, 0xa\n"
			"blt x16, x3, 1f\n"
			"1:\n"
			"fcvt.ps.pwu f31, f0, rmm\n"
			"fmul.pi f31, f0, f11\n"
			"fmin.s f5, f26, f19\n"
			"fsatu8.pi f19, f11\n"
			"fmv.x.s x28, f26\n"
			"mova.x.m x12\n"
			"srai x28, x13, 2\n"
			"slt x28, x18, x0\n"
			"mulh x28, x18, x13\n"
			"srai x10, x0, 4\n"
			"blt x25, x15, 1f\n"
			"1:\n"
			"fmulh.pi f19, f26, f19\n"
			"fmvz.x.ps x13, f11, 7\n"
			"fmul.pi f5, f26, f26\n"
			"bne x1, x19, 1f\n"
			"1:\n"
			"fnmsub.ps f26, f26, f19, f26, rne\n"
			"sra x28, x13, x13\n"
			"fsrli.pi f9, f19, 0x5\n"
			"fltu.pi f31, f19, f11\n"
			"xor x9, x9, x0\n"
			"srl x18, x18, x13\n"
			"mul x12, x9, x18\n"
			"fround.ps f31, f0, rdn\n"
			"ebreak\n"
			"remw x0, x0, x18\n"
			"fcvt.ps.f16 f5, f0\n"
			"bgeu x21, x9, 1f\n"
			"1:\n"
			"fsgnjx.s f9, f11, f19\n"
			"beq x3, x4, 1f\n"
			"1:\n"
			"flog.ps f19, f26\n"
			"fmul.ps f9, f11, f19, rup\n"
			"bgeu x0, x28, 1f\n"
			"1:\n"
			"flem.ps m6, f11, f0\n"
			"fsll.pi f9, f0, f19\n"
			"slliw x9, x0, 2\n"
			"maskxor m2, m3, m3\n"
			"addw x28, x9, x9\n"
			"fcvt.s.wu f31, x9, rtz\n"
			"fand.pi f0, f26, f0\n"
			"fsub.ps f31, f0, f11, rup\n"
			"blt x27, x19, 1f\n"
			"1:\n"
			"fsrli.pi f31, f0, 0xa\n"
			"andi x0, x9, 0xe\n"
			"fsrli.pi f0, f0, 0x9\n"
			"fsra.pi f5, f19, f26\n"
			"srlw x12, x13, x10\n"
			"sllw x0, x18, x0\n"
			"fcmov.ps f31, f26, f26, f19\n"
			"fnot.pi f0, f0\n"
			"fmax.pi f5, f0, f26\n"
			"flem.ps m3, f19, f19\n"
			"bne x9, x18, 1f\n"
			"1:\n"
			"fltm.pi m0, f0, f26\n"
			"mov.m.x m0, x0, 0xff\n"
			"fsub.s f9, f26, f26, rmm\n"
			"fmul.s f5, f26, f19, rdn\n"
			"ebreak\n"
			"mulh x12, x18, x10\n"
			"bge x10, x5, 1f\n"
			"1:\n"
			"faddi.pi f31, f26, 0xf2\n"
			"addi x0, x0, 0x1c\n"
			"fpackreph.pi f11, f19\n"
			"bltu x18, x20, 1f\n"
			"1:\n"
			"fsat8.pi f0, f0\n"
			"bltu x3, x28, 1f\n"
			"1:\n"
			"slliw x0, x18, 3\n"
			"fltm.ps m6, f26, f0\n"
			"fcvt.s.wu f26, x0, rne\n"
			"fcvt.s.wu f0, x18, rmm\n"
			"mulhu x12, x18, x10\n"
			"fbci.ps f11, 0x6849e\n"
			"fexp.ps f19, f19\n"
			"bltu x17, x26, 1f\n"
			"1:\n"
			"fexp.ps f5, f26\n"
			"blt x11, x8, 1f\n"
			"1:\n"
			"fnmsub.s f5, f19, f11, f19, rmm\n"
			"divw x0, x13, x13\n"
			"maskand m2, m3, m0\n"
			"mova.x.m x0\n"
			"fnmadd.s f9, f11, f26, f11, rdn\n"
			"fcvt.ps.pw f9, f11, rup\n"
			"sltu x18, x18, x10\n"
			"fadd.s f31, f19, f26, rne\n"
			"bge x31, x18, 1f\n"
			"1:\n"
			"feqm.ps m0, f26, f26\n"
			"sll x10, x10, x9\n"
			"fadd.pi f5, f19, f19\n"
			"bltu x5, x26, 1f\n"
			"1:\n"
			"mulhu x0, x9, x9\n"
			"fsgnjx.s f5, f19, f11\n"
			"bne x13, x6, 1f\n"
			"1:\n"
			"fsgnjn.ps f0, f0, f11\n"
			"div x12, x9, x18\n"
			"bltu x29, x3, 1f\n"
			"1:\n"
			"fclass.s x12, f19\n"
			"beq x31, x29, 1f\n"
			"1:\n"
			"fmul.pi f19, f26, f19\n"
			"addw x12, x0, x9\n"
			"maskand m6, m2, m6\n"
			"fsgnj.s f9, f0, f19\n"
			"fpackrepb.pi f9, f19\n"
			"fadd.s f5, f19, f0, rup\n"
			"sllw x12, x0, x18\n"
			"fsetm.pi m2, f26\n"
			"bltu x3, x11, 1f\n"
			"1:\n"
			"fsub.ps f31, f0, f19, rtz\n"
			"mulh x18, x9, x18\n"
			"divu x0, x18, x9\n"
			"fexp.ps f31, f26\n"
			"fltu.pi f9, f0, f26\n"
			"fminu.pi f31, f26, f26\n"
			"bne x4, x31, 1f\n"
			"1:\n"
			"srai x28, x0, 5\n"
			"fswizz.ps f0, f0, 0x19\n"
			"fmadd.s f26, f26, f0, f0, rdn\n"
			"fclass.s x12, f11\n"
			"mova.x.m x0\n"
			"flt.ps f19, f11, f26\n"
			"fmulhu.pi f26, f0, f11\n"
			"fcvt.s.wu f0, x9, rdn\n"
			"addi x21, x21, -1\n"
			"bne x21, x0, LBL_SEQID_0_LOOP_SEQ_HID_0\n"
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_0\n"
			"addi x30, x30, 1\n"
			"li x13, 0xf22fd03ad2c858e4\n"
			"li x9, 0x97e2da16e77337f2\n"
			"remw x10, x13, x9\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"divw x13, x28, x7\n"
			"fsgnjn.s f23, f23, f13\n"
			"flt.pi f26, f26, f3\n"
			"li x11, 0xfc78eaf9396b97f3\n"
			"addi x13, x11, 0x1e\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"slt x28, x7, x28\n"
			"sltiu x19, x7, 0x2\n"
			"fle.pi f28, f11, f23\n"
			"bgeu x22, x2, 1f\n"
			"1:\n"
			"la x10, rand_int_32\n"
			"flw.ps f14, 704(x10)\n"
			"fcvt.ps.pw f19, f14, rdn\n" 
			"fadd.s f3, f18, f11, rdn\n"
			"fsgnj.s f26, f13, f3\n"
			"ffrc.ps f23, f0\n"
			"la x28, rand_int_32\n"
			"flw.ps f9, 64(x28)\n"
			"flw.ps f14, 160(x28)\n"
			"fmul.pi f13, f9, f14\n" 
			"li x5, 0x54ce9a738e3662f0\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0x1a6e08c14804f18e\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"fsgnjn.s f23, f31, f7\n"
			"fltm.ps m6, f13, f3\n"
			"addiw x0, x28, 0x2\n"
			"la x10, rand_ieee754_32\n"
			"flw.ps f3, 480(x10)\n"
			"flw.ps f28, 608(x10)\n"
			"fmax.s f14, f3, f28\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"ecall\n"
			"fadd.ps f14, f23, f20, rmm\n"
			"fpackrepb.pi f18, f20\n"
			"blt x30, x10, 1f\n"
			"1:\n"
			"la x18, rand_int_32\n"
			"flw.ps f5, 896(x18)\n"
			"fsrli.pi f18, f5, 0x5\n" 
			"fnmadd.ps f9, f26, f9, f9, rup\n"
			"fpackrepb.pi f0, f7\n"
			"bgeu x12, x13, 1f\n"
			"1:\n"
			"slliw x10, x13, 4\n"
			"li x13, 0xc10749e76847a3e0\n"
			"li x10, 0x4144a59b53ff2e7d\n"
			"sll x9, x13, x10\n" 
			"fnot.pi f13, f5\n"
			"fsin.ps f13, f5\n"
			"fsub.ps f7, f13, f14, rdn\n"
			"beq x31, x18, 1f\n"
			"1:\n"
			"la x17, rand_ieee754_32\n"
			"flw.ps f23, 544(x17)\n"
			"flw.ps f13, 736(x17)\n"
			"fadd.ps f16, f23, f13, rmm\n" 
			"fmsub.s f23, f14, f0, f13, rup\n"
			"remuw x9, x7, x19\n"
			"feqm.ps m6, f13, f14\n"
			"LBL_SEQID_1_M0_WRITE_ID_32_HID_0:\n"
			"flem.ps m3, f23, f23\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"fsetm.pi m6, f14\n"
			"fmin.s f0, f13, f26\n"
			"fandi.pi f3, f0, 0x1ea\n"
			"bge x22, x26, 1f\n"
			"1:\n"
			"li x28, 0x9580203640cbe44\n"
			"li x18, 0xe1555357d26e1ba1\n"
			"sub x17, x28, x18\n" 
			"fltu.pi f5, f7, f5\n"
			"fmax.pi f18, f3, f18\n"
			"srai x17, x17, 5\n"
			"li x17, 0x3485f8dcc002f3f2\n"
			"addi x17, x17, 0xa\n" 
			"fsll.pi f31, f5, f28\n"
			"fsub.pi f19, f9, f19\n"
			"fsll.pi f9, f9, f16\n"
			"la x28, rand_ieee754_32\n"
			"flw.ps f23, 416(x28)\n"
			"fswizz.ps f3, f23, 0x1d\n" 
			"fand.pi f9, f19, f23\n"
			"fsgnj.ps f5, f16, f9\n"
			"fadd.ps f28, f13, f31, rne\n"
			"li x10, 0xf46c858af464737f\n"
			"li x18, 0xc831e203262718d7\n"
			"remw x12, x10, x18\n" 
			"fmsub.ps f31, f31, f31, f23, rup\n"
			"fsat8.pi f28, f3\n"
			"divw x11, x28, x0\n"
			"li x18, 0xe8043109c0175c9\n"
			"srli x19, x18, 0\n" 
			"fmsub.s f19, f20, f11, f0, rup\n"
			"fsll.pi f19, f7, f3\n"
			"fle.s x11, f14, f3\n"
			"la x12, rand_int_32\n"
			"flw.ps f26, 224(x12)\n"
			"faddi.pi f18, f26, 0xef\n" 
			"fslli.pi f13, f9, 0x0\n"
			"slliw x17, x0, 5\n"
			"slt x0, x19, x7\n"
			"LBL_SEQID_1_FP_TRANS_ID_33_HID_0:\n"
			"la x7, rand_ieee754_32\n"
			"flw.ps f28, 672(x7)\n"
			"flog.ps f19, f28\n" 
			"sra x19, x12, x10\n"
			"fsgnjx.s f28, f16, f23\n"
			"remw x17, x12, x10\n"
			"la x12, rand_ieee754_32\n"
			"flw.ps f5, 640(x12)\n"
			"flw.ps f23, 640(x12)\n"
			"fmin.s f23, f5, f23\n" 
			"fsgnjn.ps f14, f7, f9\n"
			"fround.ps f7, f26, rmm\n"
			"fexp.ps f5, f26\n"
			"LBL_SEQID_1_INT_DIV_ID_34_HID_0:\n"
			"divuw x13, x17, x7\n" 
			"fcmovm.ps f9, f3, f13\n"
			"fxor.pi f20, f11, f16\n"
			"bgeu x29, x24, 1f\n"
			"1:\n"
			"fsgnj.ps f3, f3, f3\n"
			"bgeu x26, x20, 1f\n"
			"1:\n"
			"li x10, 0x96602a0e78431b2c\n"
			"slli x13, x10, 0\n" 
			"fcvt.s.w f18, x11, rtz\n"
			"fcvt.wu.s x0, f28, rdn\n"
			"fbcx.ps f31, x12\n"
			"la x19, rand_ieee754_32\n"
			"flw.ps f14, 416(x19)\n"
			"flw.ps f19, 416(x19)\n"
			"fle.s x7, f14, f19\n" 
			"remu x19, x17, x19\n"
			"beq x20, x27, 1f\n"
			"1:\n"
			"fadd.ps f9, f9, f5, rmm\n"
			"srlw x18, x12, x19\n"
			"beq x3, x11, 1f\n"
			"1:\n"
			"la x28, rand_int_32\n"
			"flw.ps f3, 416(x28)\n"
			"fsrli.pi f7, f3, 0xb\n" 
			"frcp.ps f9, f9\n"
			"bltu x18, x25, 1f\n"
			"1:\n"
			"fsub.pi f11, f11, f9\n"
			"fmulh.pi f28, f9, f31\n"
			"la x19, rand_int_32\n"
			"flw.ps f9, 576(x19)\n"
			"flw.ps f19, 640(x19)\n"
			"fmulhu.pi f28, f9, f19\n" 
			"subw x18, x9, x28\n"
			"fcmovm.ps f13, f13, f13\n"
			"frsq.ps f16, f14\n"
			"la x11, rand_int_32\n"
			"flw.ps f13, 800(x11)\n"
			"flw.ps f28, 480(x11)\n"
			"fle.pi f26, f13, f28\n" 
			"fmul.ps f28, f0, f18, rdn\n"
			"faddi.pi f28, f23, 0x1c6\n"
			"fnmsub.s f0, f23, f0, f23, rmm\n"
			"li x7, 0xfee9623471a928e\n"
			"li x12, 0x37c777d208370456\n"
			"slt x28, x7, x12\n" 
			"li x5, 0xaa51\n"
			"csrw tensor_mask, x5\n"	
			"fswizz.ps f28, f19, 0x16\n"
			"fnot.pi f20, f11\n"
			"fpackreph.pi f19, f7\n"
			"LBL_SEQID_1_M0_WRITE_ID_35_HID_0:\n"
			"mov.m.x m6, x13, 0x1d\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"fmsub.s f3, f5, f26, f11, rmm\n"
			"flt.pi f5, f26, f5\n"
			"fmaxu.pi f18, f13, f26\n"
			"bge x28, x15, 1f\n"
			"1:\n"
			"la x19, rand_ieee754_32\n"
			"flw.ps f7, 416(x19)\n"
			"flw.ps f20, 192(x19)\n"
			"fcmovm.ps f18, f7, f20\n" 
			"fmul.s f31, f3, f0, rdn\n"
			"fmv.x.s x0, f3\n"
			"and x7, x10, x12\n"
			"li x9, 0x7a95a9fa06114e3d\n"
			"srli x10, x9, 5\n" 
			"fsub.ps f13, f14, f3, rtz\n"
			"fmax.s f13, f13, f13\n"
			"fcvt.ps.f16 f23, f3\n"
			"li x18, 0xfd2f14e5de094ba2\n"
			"li x19, 0x21806124512dd7d8\n"
			"mulh x11, x18, x19\n" 
			"fsgnjx.ps f18, f9, f13\n"
			"flog.ps f0, f3\n"
			"faddi.pi f7, f9, 0x130\n"
			"la x11, rand_ieee754_32\n"
			"flw.ps f11, 64(x11)\n"
			"flw.ps f5, 448(x11)\n"
			"fmax.ps f7, f11, f5\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"fcvt.f16.ps f23, f23\n"
			"bltu x1, x28, 1f\n"
			"1:\n"
			"fadd.s f0, f7, f26, rmm\n"
			"fcmovm.ps f5, f23, f26\n"
			"li x19, 0x6d481459cd3389b7\n"
			"sltiu x17, x19, 0x8\n" 
			"frsq.ps f26, f3\n"
			"fence\n"
			"bltu x8, x21, 1f\n"
			"1:\n"
			"flt.s x13, f31, f19\n"
			"LBL_SEQID_1_M0_WRITE_ID_36_HID_0:\n"
			"maskxor m2, m6, m2\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"fmin.ps f0, f18, f23\n"
			"fadd.pi f20, f18, f20\n"
			"packb x7, x17, x7\n"
			"bge x4, x17, 1f\n"
			"1:\n"
			"mova.x.m x7\n" 
			"fmadd.s f9, f13, f16, f7, rne\n"
			"fexp.ps f26, f26\n"
			"fsrai.pi f9, f9, 0xb\n"
			"bltu x12, x2, 1f\n"
			"1:\n"
			"csrw tensor_error, zero\n" 
			"or x10, x7, x18\n"
			"fnmsub.s f23, f16, f23, f13, rmm\n"
			"sllw x11, x13, x11\n"
			"fmulhu.pi f9, f20, f0\n"
			"sltiu x28, x18, 0x17\n"
			"blt x29, x10, 1f\n"
			"1:\n"
			"slt x10, x28, x13\n"
			"divu x10, x11, x9\n"
			"blt x25, x9, 1f\n"
			"1:\n"
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ITER_0_HID_0:\n"
			"li x6, ((0x1c << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0xb << TENSOR_QUANT_QUANT_ROW) | (0xe << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x0 << TENSOR_QUANT_TRANSF1) | (0x2 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ITER_1_HID_0:\n"
			"li x6, ((0xc << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0xd << TENSOR_QUANT_QUANT_ROW) | (0x24 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x6 << TENSOR_QUANT_TRANSF4) | (0x6 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ITER_2_HID_0:\n"
			"li x6, ((0x1b << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0x4 << TENSOR_QUANT_QUANT_ROW) | (0x20 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x1 << TENSOR_QUANT_TRANSF9) | (0x4 << TENSOR_QUANT_TRANSF8) | (0x9 << TENSOR_QUANT_TRANSF7) | (0x5 << TENSOR_QUANT_TRANSF6) | (0x8 << TENSOR_QUANT_TRANSF5) | (0x4 << TENSOR_QUANT_TRANSF4) | (0x4 << TENSOR_QUANT_TRANSF3) | (0x3 << TENSOR_QUANT_TRANSF2) | (0xa << TENSOR_QUANT_TRANSF1) | (0x4 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ITER_3_HID_0:\n"
			"li x6, ((0xe << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0xa << TENSOR_QUANT_QUANT_ROW) | (0xb << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x2 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ITER_4_HID_0:\n"
			"li x6, ((0x7 << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0xc << TENSOR_QUANT_QUANT_ROW) | (0x38 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x0 << TENSOR_QUANT_TRANSF1) | (0x0 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ITER_5_HID_0:\n"
			"li x6, ((0xb << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0xe << TENSOR_QUANT_QUANT_ROW) | (0x3d << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x6 << TENSOR_QUANT_TRANSF4) | (0x6 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ITER_6_HID_0:\n"
			"li x6, ((0x18 << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0x8 << TENSOR_QUANT_QUANT_ROW) | (0x13 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x6 << TENSOR_QUANT_TRANSF8) | (0x7 << TENSOR_QUANT_TRANSF7) | (0x7 << TENSOR_QUANT_TRANSF6) | (0x6 << TENSOR_QUANT_TRANSF5) | (0x7 << TENSOR_QUANT_TRANSF4) | (0x7 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ITER_7_HID_0:\n"
			"li x6, ((0xc << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0x6 << TENSOR_QUANT_QUANT_ROW) | (0xe << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x1 << TENSOR_QUANT_TRANSF3) | (0x4 << TENSOR_QUANT_TRANSF2) | (0x5 << TENSOR_QUANT_TRANSF1) | (0x5 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ITER_8_HID_0:\n"
			"li x6, ((0xb << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0x2 << TENSOR_QUANT_QUANT_ROW) | (0x10 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x7 << TENSOR_QUANT_TRANSF7) | (0x6 << TENSOR_QUANT_TRANSF6) | (0x7 << TENSOR_QUANT_TRANSF5) | (0x6 << TENSOR_QUANT_TRANSF4) | (0x7 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_0\n"
			"csrw tensor_error, x0\n"
			"mov.m.x m0, x0, 0xFF\n"
			"li x5, 0x40\n"
			"la x31, rand_ieee754_32\n"
			"add x31, x31, x5\n"
			"flw.ps f11, 0(x31)\n"
			"flw.ps f13, 0(x31)\n"
			"flw.ps f14, 4(x31)\n"
			"flw.ps f19, 10(x31)\n"
			"flw.ps f23, 4(x31)\n"
			"flw.ps f28, 12(x31)\n"
			"flw.ps f3, 21(x31)\n"
			"flw.ps f31, 18(x31)\n"
			"flw.ps f5, 21(x31)\n"
			"flw.ps f7, 8(x31)\n"
			"la x31, rand_int_32\n"
			"add x31, x31, x5\n"
			"lw x0, 0(x31)\n"
			"lw x10, 3(x31)\n"
			"lw x11, 6(x31)\n"
			"lw x12, 0(x31)\n"
			"lw x13, 16(x31)\n"
			"lw x17, 1(x31)\n"
			"lw x18, 1(x31)\n"
			"lw x19, 3(x31)\n"
			"lw x28, 23(x31)\n"
			"lw x7, 10(x31)\n"
			"lw x9, 25(x31)\n"
			"li x21, 0x3\n"
			"LBL_SEQID_1_LOOP_SEQ_HID_0:\n"
			"flt.pi f16, f7, f19\n"
			"xor x9, x17, x13\n"
			"fsgnj.ps f7, f31, f14\n"
			"bne x26, x3, 1f\n"
			"1:\n"
			"remw x0, x9, x11\n"
			"fmin.s f19, f3, f11\n"
			"fmul.s f31, f7, f19, rup\n"
			"fsra.pi f19, f7, f13\n"
			"xor x28, x12, x28\n"
			"fsrai.pi f23, f28, 0x0\n"
			"fmulhu.pi f13, f14, f3\n"
			"fmin.pi f26, f13, f5\n"
			"bge x28, x27, 1f\n"
			"1:\n"
			"fmaxu.pi f9, f3, f7\n"
			"for.pi f3, f5, f7\n"
			"fsll.pi f5, f23, f5\n"
			"srlw x12, x13, x12\n"
			"fmax.pi f23, f11, f23\n"
			"fcvt.ps.f16 f5, f7\n"
			"fmax.pi f13, f5, f31\n"
			"subw x10, x10, x0\n"
			"srai x0, x17, 4\n"
			"fsatu8.pi f23, f13\n"
			"bne x6, x30, 1f\n"
			"1:\n"
			"fcvt.ps.pwu f16, f23, rdn\n"
			"faddi.pi f7, f7, 0x141\n"
			"fcvt.ps.f16 f9, f11\n"
			"for.pi f7, f3, f23\n"
			"fsgnj.ps f14, f23, f28\n"
			"bne x13, x17, 1f\n"
			"1:\n"
			"fsgnjx.s f14, f11, f7\n"
			"fsrl.pi f23, f23, f23\n"
			"bltu x25, x17, 1f\n"
			"1:\n"
			"fsll.pi f7, f7, f19\n"
			"fsin.ps f19, f7\n"
			"bgeu x28, x25, 1f\n"
			"1:\n"
			"for.pi f14, f23, f28\n"
			"xori x17, x17, 0x0\n"
			"fcvt.f16.ps f31, f23\n"
			"feq.ps f11, f5, f31\n"
			"fxor.pi f19, f13, f11\n"
			"sllw x0, x0, x9\n"
			"sub x28, x28, x18\n"
			"mulhu x19, x18, x19\n"
			"remw x7, x7, x9\n"
			"fmulhu.pi f13, f13, f23\n"
			"sraw x12, x9, x7\n"
			"fmax.ps f9, f3, f13\n"
			"blt x12, x6, 1f\n"
			"1:\n"
			"fround.ps f16, f23, rtz\n"
			"fcmovm.ps f23, f28, f3\n"
			"andi x7, x9, 0x12\n"
			"fmulh.pi f9, f11, f28\n"
			"fadd.pi f9, f19, f5\n"
			"bne x7, x19, 1f\n"
			"1:\n"
			"fcvt.pw.ps f28, f28, rdn\n"
			"blt x27, x22, 1f\n"
			"1:\n"
			"fexp.ps f3, f23\n"
			"add x0, x12, x28\n"
			"fmin.s f16, f5, f3\n"
			"fle.ps f9, f3, f7\n"
			"fmax.s f13, f19, f13\n"
			"bltu x19, x2, 1f\n"
			"1:\n"
			"fcvt.ps.f16 f14, f7\n"
			"fmul.ps f13, f5, f11, rdn\n"
			"fmul.s f23, f14, f31, rup\n"
			"fcvt.pw.ps f31, f11, rtz\n"
			"fandi.pi f14, f31, 0x1c9\n"
			"mulh x11, x28, x17\n"
			"beq x12, x10, 1f\n"
			"1:\n"
			"sra x28, x10, x0\n"
			"fsgnjn.s f28, f14, f31\n"
			"fcvt.ps.pwu f28, f11, rdn\n"
			"fsub.pi f9, f7, f13\n"
			"fmax.pi f7, f7, f7\n"
			"beq x28, x23, 1f\n"
			"1:\n"
			"fmul.pi f23, f23, f23\n"
			"bge x25, x27, 1f\n"
			"1:\n"
			"xor x12, x11, x12\n"
			"bgeu x9, x3, 1f\n"
			"1:\n"
			"fsub.s f19, f3, f19, rtz\n"
			"fsrai.pi f26, f14, 0x9\n"
			"fsin.ps f11, f14\n"
			"sub x0, x0, x9\n"
			"mulw x11, x13, x9\n"
			"flog.ps f7, f28\n"
			"sll x7, x12, x7\n"
			"blt x5, x28, 1f\n"
			"1:\n"
			"fsgnj.s f28, f28, f13\n"
			"fsat8.pi f23, f28\n"
			"fsub.s f9, f28, f28, rtz\n"
			"fmsub.ps f19, f19, f23, f14, rup\n"
			"flt.ps f3, f19, f3\n"
			"flt.ps f3, f5, f11\n"
			"slli x10, x17, 1\n"
			"fmul.s f14, f5, f19, rmm\n"
			"fadd.ps f28, f5, f28, rne\n"
			"fsub.s f31, f14, f14, rtz\n"
			"fandi.pi f9, f13, 0x5a\n"
			"fmaxu.pi f31, f31, f19\n"
			"xor x17, x0, x17\n"
			"blt x25, x18, 1f\n"
			"1:\n"
			"subw x12, x12, x28\n"
			"fclass.ps f5, f5\n"
			"bne x17, x11, 1f\n"
			"1:\n"
			"sltiu x18, x28, 0x9\n"
			"bne x8, x27, 1f\n"
			"1:\n"
			"fswizz.ps f23, f19, 0x13\n"
			"mulhu x11, x11, x12\n"
			"fminu.pi f19, f11, f28\n"
			"flt.ps f23, f13, f14\n"
			"fmaxu.pi f13, f3, f19\n"
			"slli x19, x17, 4\n"
			"fclass.ps f13, f28\n"
			"blt x26, x16, 1f\n"
			"1:\n"
			"fmsub.ps f23, f19, f23, f13, rtz\n"
			"bgeu x20, x21, 1f\n"
			"1:\n"
			"remu x17, x0, x7\n"
			"flt.pi f26, f11, f19\n"
			"bgeu x0, x27, 1f\n"
			"1:\n"
			"sraw x0, x10, x17\n"
			"fsatu8.pi f23, f5\n"
			"feq.pi f11, f14, f11\n"
			"bge x24, x20, 1f\n"
			"1:\n"
			"frcp.ps f23, f5\n"
			"fandi.pi f11, f11, 0x1cb\n"
			"flt.ps f19, f5, f13\n"
			"slt x0, x19, x9\n"
			"fclass.ps f31, f31\n"
			"beq x3, x1, 1f\n"
			"1:\n"
			"addi x28, x28, 0xa\n"
			"sub x9, x17, x10\n"
			"fcvt.pwu.ps f26, f23, rne\n"
			"beq x15, x10, 1f\n"
			"1:\n"
			"sltiu x19, x13, 0xe\n"
			"fsgnjx.ps f5, f5, f23\n"
			"beq x2, x9, 1f\n"
			"1:\n"
			"remuw x10, x0, x10\n"
			"xori x9, x13, 0xd\n"
			"fmsub.s f13, f13, f31, f13, rup\n"
			"bge x8, x25, 1f\n"
			"1:\n"
			"fmax.pi f26, f23, f31\n"
			"fcvt.ps.f16 f14, f31\n"
			"fcvt.ps.pwu f13, f19, rup\n"
			"fsrl.pi f11, f14, f13\n"
			"fsgnjn.ps f14, f14, f13\n"
			"bne x12, x11, 1f\n"
			"1:\n"
			"fswizz.ps f26, f23, 0x14\n"
			"fcvt.pw.ps f13, f13, rmm\n"
			"slt x17, x17, x13\n"
			"fpackrepb.pi f14, f13\n"
			"sllw x18, x18, x17\n"
			"srliw x0, x18, 3\n"
			"fadd.ps f31, f5, f23, rup\n"
			"slli x7, x19, 0\n"
			"fcvt.pwu.ps f19, f3, rmm\n"
			"fcvt.f16.ps f31, f28\n"
			"bne x29, x7, 1f\n"
			"1:\n"
			"addi x19, x19, 0x7\n"
			"mul x28, x7, x18\n"
			"ori x11, x11, 0xa\n"
			"fcvt.pwu.ps f28, f19, rtz\n"
			"for.pi f28, f19, f3\n"
			"fslli.pi f14, f7, 0x3\n"
			"feq.pi f23, f13, f31\n"
			"xori x19, x12, 0xc\n"
			"fmax.pi f31, f7, f3\n"
			"blt x27, x23, 1f\n"
			"1:\n"
			"fmax.ps f28, f7, f28\n"
			"bne x0, x21, 1f\n"
			"1:\n"
			"fmadd.ps f28, f7, f14, f14, rup\n"
			"xori x0, x9, 0xa\n"
			"slli x18, x19, 1\n"
			"ffrc.ps f9, f5\n"
			"sra x13, x0, x0\n"
			"addi x21, x21, -1\n"
			"bne x21, x0, LBL_SEQID_1_LOOP_SEQ_HID_0\n"
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_0\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_2_INT_AMO_ID_37_HID_0:\n"
			"li x7, 0x822f1c0030\n" 
			"amoandg.w x19, x11, (x7)\n" 
			"LBL_SEQID_2_INT_AMO_ID_38_HID_0:\n"
			"li x7, 0x81317a1020\n" 
			"amoandg.w x11, x12, (x7)\n" 
			"LBL_SEQID_2_INT_AMO_ID_39_HID_0:\n"
			"li x12, 0x8141deb810\n" 
			"amoswapl.d x17, x11, (x12)\n" 
			"LBL_SEQID_2_FP_GATHER_ID_40_HID_0:\n"
			"li x17, 0x8201a0ffbd\n" 
			"la x11, mem_ptr_offset\n"
			"flw.ps f11, 168(x11)\n"
			"fscw.ps f16, f11 (x17)\n"
			"fgh.ps f7, f11 (x17)\n" 
			"LBL_SEQID_2_FP_LOAD_ID_41_HID_0:\n"
			"li x19, 0x810bfa040a\n" 
			"fsw.ps f3, -904(x19)\n"
			"flw f16, -904(x19)\n" 
			"LBL_SEQID_2_INT_AMO_ID_42_HID_0:\n"
			"li x18, 0x823964002c\n" 
			"amomaxug.w x28, x28, (x18)\n" 
			"LBL_SEQID_2_FP_LOAD_ID_43_HID_0:\n"
			"li x11, 0x8100d006c0\n" 
			"fsw.ps f18, -1163(x11)\n"
			"flw.ps f28, -1163(x11)\n" 
			"LBL_SEQID_2_INT_LOAD_ID_44_HID_0:\n"
			"li x13, 0x8203861f60\n" 
			"sd x19, 1189(x13)\n"
			"lhu x9, 1189(x13)\n" 
			"LBL_SEQID_2_CSR_ID_45_HID_0:\n"
			"csrr x0, stvec\n"
			"LBL_SEQID_2_FP_L_SCATTER_ID_46_HID_0:\n"
			"li x11, 0x82512a0028\n" 
			"la x13, mem_ptr_offset\n"
			"flw.ps f20, 52(x13)\n"
			"fscwl.ps f11, f20 (x11)\n" 
			"LBL_SEQID_2_FP_AMO_ID_47_HID_0:\n"
			"li x18, 0x813bfa0408\n" 
			"la x7, mem_ptr_offset\n"
			"flw.ps f28, 864(x7)\n"
			"famoming.pi f26, f28 (x18)\n" 
			"LBL_SEQID_2_FP_GATHER_ID_48_HID_0:\n"
			"li x17, 0x820a78e020\n" 
			"la x7, mem_ptr_offset\n"
			"flq2 f28, 172(x7)\n"
			"fscw.ps f28, f28 (x17)\n"
			"fgh.ps f19, f28 (x17)\n" 
			"LBL_SEQID_2_INT_AMO_ID_49_HID_0:\n"
			"li x18, 0x825a78e020\n" 
			"amominl.w x10, x13, (x18)\n" 
			"LBL_SEQID_2_INT_STORE_ID_50_HID_0:\n"
			"li x13, 0x81005ed81a\n" 
			"sw x11, -978(x13)\n" 
			"LBL_SEQID_2_INT_AMO_ID_51_HID_0:\n"
			"li x17, 0x8140d006c0\n" 
			"amoaddl.d x9, x28, (x17)\n" 
			"LBL_SEQID_2_CSR_ID_52_HID_0:\n"
			"csrr x0, misa\n"
			"LBL_SEQID_2_FP_STORE_ID_53_HID_0:\n"
			"li x13, 0x8209bee833\n" 
			"fsq2 f13, -336(x13)\n" 
			"LBL_SEQID_2_INT_LOAD_ID_54_HID_0:\n"
			"li x7, 0x8105d4a08a\n" 
			"sd x18, 1813(x7)\n"
			"lh x9, 1813(x7)\n" 
			"LBL_SEQID_2_FP_AMO_ID_55_HID_0:\n"
			"li x12, 0x81505ed818\n" 
			"la x17, mem_ptr_offset\n"
			"flw.ps f18, 256(x17)\n"
			"famomaxl.ps f7, f18 (x12)\n" 
			"LBL_SEQID_2_FP_GATHER_ID_56_HID_0:\n"
			"li x9, 0x81016a0004\n" 
			"li x10, 0x7db67e3bd62700c3\n"
			"fsc32w.ps f12, x10 (x9)\n"
			"fg32h.ps f18, x10 (x9)\n" 
			"LBL_SEQID_2_INT_AMO_ID_57_HID_0:\n"
			"li x12, 0x813bfa0408\n" 
			"amoming.d x11, x13, (x12)\n" 
			"LBL_SEQID_2_FP_GATHER_ID_58_HID_0:\n"
			"li x12, 0x820f1c0030\n" 
			"li x10, 0xef4b798f2f9fa324\n"
			"fsc32w.ps f23, x10 (x12)\n"
			"fg32h.ps f13, x10 (x12)\n" 
			"LBL_SEQID_2_INT_LOAD_ID_59_HID_0:\n"
			"li x28, 0x81056400b4\n" 
			"sd x10, 384(x28)\n"
			"lhu x28, 384(x28)\n" 
			"fence.i\n" 
			"LBL_SEQID_2_INT_AMO_ID_60_HID_0:\n"
			"li x28, 0x81440a0208\n" 
			"amominul.d x11, x19, (x28)\n" 
			"LBL_SEQID_2_FP_SCATTER_ID_61_HID_0:\n"
			"li x9, 0x820d47ef76\n" 
			"la x17, mem_ptr_offset\n"
			"flq2 f23, 144(x17)\n"
			"fsch.ps f13, f23 (x9)\n" 
			"LBL_SEQID_2_INT_AMO_ID_62_HID_0:\n"
			"li x17, 0x8253860a28\n" 
			"amominl.d x13, x9, (x17)\n" 
			"LBL_SEQID_2_INT_LOAD_ID_63_HID_0:\n"
			"li x18, 0x8209bee833\n" 
			"sd x13, 367(x18)\n"
			"lbu x12, 367(x18)\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"LBL_SEQID_2_FP_AMO_ID_64_HID_0:\n"
			"li x18, 0x813896e014\n" 
			"la x19, mem_ptr_offset\n"
			"flw.ps f20, 32(x19)\n"
			"famomaxg.pi f26, f20 (x18)\n" 
			"LBL_SEQID_2_FP_G_SCATTER_ID_65_HID_0:\n"
			"li x13, 0x82312a0028\n" 
			"la x7, mem_ptr_offset\n"
			"flw.ps f26, 216(x7)\n"
			"fscwg.ps f11, f26 (x13)\n" 
			"LBL_SEQID_2_CSR_ID_66_HID_0:\n"
			"li x17, 0x3c20000080007fcc\n"
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x17\n" 
			"csrwi tensor_wait, 1\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"LBL_SEQID_2_FP_LOAD_ID_67_HID_0:\n"
			"li x13, 0x81056400b4\n" 
			"fsw.ps f7, -53(x13)\n"
			"flw.ps f0, -53(x13)\n" 
			"csrw tensor_error, zero\n" 
			"fmin.s f19, f5, f5\n"
			"fmulhu.pi f26, f18, f11\n"
			"fsgnjn.s f0, f5, f11\n"
			"beq x12, x9, 1f\n"
			"1:\n"
			"fsgnjx.s f5, f5, f5\n"
			"feq.ps f11, f5, f11\n"
			"bne x11, x4, 1f\n"
			"1:\n"
			"fsub.s f20, f20, f14, rdn\n"
			"fmul.s f26, f14, f16, rne\n"
			"fsgnj.ps f9, f11, f16\n"
			"fmadd.s f23, f26, f16, f28, rdn\n"
			"fsub.s f9, f11, f20, rtz\n"
			"fsrl.pi f23, f14, f9\n"
			"faddi.pi f23, f11, 0x113\n"
			"fcvt.f16.ps f9, f26\n"
			"feq.ps f20, f26, f20\n"
			"fpackreph.pi f26, f26\n"
			"LBL_SEQID_2_TLD_XMA_STR_SEQ_ITER_0_HID_0:\n"
			"li x5, ((0x0 << TENSOR_LOAD_T32_USE_TMASK) | (0x0 << TENSOR_LOAD_T32_USE_COOP) | (0x7 << TENSOR_LOAD_T32_OPC_111) | (0x1 << TENSOR_LOAD_T32_DST_START) | (0x2058f1240 << TENSOR_LOAD_T32_VADDR) | (0x3 << TENSOR_LOAD_T32_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_I8_USE_TMASK) | (0x0 << TENSOR_LOAD_I8_USE_COOP) | (0x1 << TENSOR_LOAD_I8_OPC_001) | (0x18 << TENSOR_LOAD_I8_DST_START) | (0x205f3a940 << TENSOR_LOAD_I8_VADDR) | (0x0 << TENSOR_LOAD_I8_OFFSET) | (0x9 << TENSOR_LOAD_I8_NUM_LINES) | 0 )\n" 
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x2 << TENSOR_FMA16_B_NUM_COLS) | (0x3 << TENSOR_FMA16_A_NUM_ROWS) | (0x9 << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x18 << TENSOR_FMA16_SCP_LOC_B) | (0x1 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x0 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x12 << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0x3 << TENSOR_QUANT_QUANT_ROW) | (0x11 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x2 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x10, ((0x2 << TENSOR_STORE_SCP_ENTRY_STRIDE) | (0x1a << TENSOR_STORE_SCP_START_SCP_ENTRY) | (0xf << TENSOR_STORE_SCP_AROWS) | (0x1 << TENSOR_STORE_SCP_OPC_1) | (0x2058f1240 << TENSOR_STORE_SCP_VADDR) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrw tensor_quant, x9\n"	
			"li x31, (0x3 << 6)\n"
			"csrwi tensor_wait, 7\n"
			"csrwi tensor_wait, 10\n"
			"csrw tensor_store, x10\n"	
			"csrwi tensor_wait, 8\n"
			"LBL_SEQID_2_TLD_XMA_STR_SEQ_ITER_1_HID_0:\n"
			"la x31, self_check_8\n"
			"li x5, 0xd\n"
			"add x5, x5, x31\n"
			"li x6, 0x3e0000000000009\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x1 << TENSOR_IMA_B_NUM_COLS) | (0xd << TENSOR_IMA_A_NUM_ROWS) | (0x9 << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1f << TENSOR_IMA_SCP_LOC_B) | (0x0 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fmvz.x.ps x31, f0, 0\n"
			"li x5, 0x28\n"
			"bne x5, x31, LBL_FAIL_HID_0\n"
			"LBL_SEQID_2_TLD_XMA_STR_SEQ_ITER_2_HID_0:\n"
			"li x5, ((0x0 << TENSOR_LOAD_USE_TMASK) | (0x0 << TENSOR_LOAD_USE_COOP) | (0x0 << TENSOR_LOAD_OPC_000) | (0x0 << TENSOR_LOAD_DST_START) | (0x0 << TENSOR_LOAD_OPC_0) | (0x209e6fba0 << TENSOR_LOAD_VADDR) | (0xc << TENSOR_LOAD_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_SETUP_B_USE_COOP) | (0x0 << TENSOR_LOAD_SETUP_B_OPC_000) | (0x1 << TENSOR_LOAD_SETUP_B_OPC_1) | (0x20001ad << TENSOR_LOAD_SETUP_B_VADDR) | (0x7 << TENSOR_LOAD_SETUP_B_NUM_LINES) | 0 )\n" 
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x1 << TENSOR_IMA_B_NUM_COLS) | (0xc << TENSOR_IMA_A_NUM_ROWS) | (0x7 << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x1 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1d << TENSOR_IMA_SCP_LOC_B) | (0x0 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x0 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x10, ((0x2 << TENSOR_STORE_REG_STRIDE) | (0x1a << TENSOR_STORE_START_REG) | (0x3 << TENSOR_STORE_STORE_COLS) | (0xa << TENSOR_STORE_AROWS) | (0x0 << TENSOR_STORE_COOP_STORE) | (0x0 << TENSOR_STORE_OPC_0) | (0x800041c << TENSOR_STORE_VADDR) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"li x31, (0x40 << 4)\n"
			"csrw tensor_store, x10\n"	
			"csrwi tensor_wait, 7\n"
			"csrwi tensor_wait, 8\n"
			"LBL_SEQID_2_TLD_XMA_STR_SEQ_ITER_3_HID_0:\n"
			"li x5, ((0x0 << TENSOR_LOAD_I16_USE_TMASK) | (0x0 << TENSOR_LOAD_I16_USE_COOP) | (0x2 << TENSOR_LOAD_I16_OPC_010) | (0x0 << TENSOR_LOAD_I16_DST_START) | (0x2fe011e << TENSOR_LOAD_I16_VADDR) | (0x0 << TENSOR_LOAD_I16_OFFSET) | (0x9 << TENSOR_LOAD_I16_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_T32_USE_TMASK) | (0x0 << TENSOR_LOAD_T32_USE_COOP) | (0x7 << TENSOR_LOAD_T32_OPC_111) | (0x1e << TENSOR_LOAD_T32_DST_START) | (0x206017b60 << TENSOR_LOAD_T32_VADDR) | (0x8 << TENSOR_LOAD_T32_NUM_LINES) | 0 )\n" 
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x0 << TENSOR_FMA16_B_NUM_COLS) | (0x9 << TENSOR_FMA16_A_NUM_ROWS) | (0x8 << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1e << TENSOR_FMA16_SCP_LOC_B) | (0x0 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x0 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x10, ((0x2 << TENSOR_STORE_REG_STRIDE) | (0x1e << TENSOR_STORE_START_REG) | (0x3 << TENSOR_STORE_STORE_COLS) | (0x4 << TENSOR_STORE_AROWS) | (0x0 << TENSOR_STORE_COOP_STORE) | (0x0 << TENSOR_STORE_OPC_0) | (0x8000007 << TENSOR_STORE_VADDR) | 0 )\n" 
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
			"LBL_SEQID_2_TLD_XMA_STR_SEQ_ITER_4_HID_0:\n"
			"li x5, ((0x0 << TENSOR_LOAD_T8_USE_TMASK) | (0x0 << TENSOR_LOAD_T8_USE_COOP) | (0x5 << TENSOR_LOAD_T8_OPC_101) | (0x7 << TENSOR_LOAD_T8_DST_START) | (0x209ca7dc0 << TENSOR_LOAD_T8_VADDR) | (0x0 << TENSOR_LOAD_T8_OFFSET) | (0x0 << TENSOR_LOAD_T8_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_T16_USE_TMASK) | (0x0 << TENSOR_LOAD_T16_USE_COOP) | (0x6 << TENSOR_LOAD_T16_OPC_110) | (0x1f << TENSOR_LOAD_T16_DST_START) | (0x209ce1828 << TENSOR_LOAD_T16_VADDR) | (0x0 << TENSOR_LOAD_T16_OFFSET) | (0xb << TENSOR_LOAD_T16_NUM_LINES) | 0 )\n" 
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x2 << TENSOR_IMA_B_NUM_COLS) | (0x0 << TENSOR_IMA_A_NUM_ROWS) | (0xb << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x0 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1f << TENSOR_IMA_SCP_LOC_B) | (0x7 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"LBL_SEQID_2_TLD_XMA_STR_SEQ_ITER_5_HID_0:\n"
			"li x5, ((0x0 << TENSOR_LOAD_T8_USE_TMASK) | (0x0 << TENSOR_LOAD_T8_USE_COOP) | (0x5 << TENSOR_LOAD_T8_OPC_101) | (0x6 << TENSOR_LOAD_T8_DST_START) | (0x2058f1240 << TENSOR_LOAD_T8_VADDR) | (0x0 << TENSOR_LOAD_T8_OFFSET) | (0x4 << TENSOR_LOAD_T8_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_SETUP_B_USE_COOP) | (0x0 << TENSOR_LOAD_SETUP_B_OPC_000) | (0x1 << TENSOR_LOAD_SETUP_B_OPC_1) | (0x205edbfc4 << TENSOR_LOAD_SETUP_B_VADDR) | (0xc << TENSOR_LOAD_SETUP_B_NUM_LINES) | 0 )\n" 
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x3 << TENSOR_IMA_B_NUM_COLS) | (0x4 << TENSOR_IMA_A_NUM_ROWS) | (0xc << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x0 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x1 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1a << TENSOR_IMA_SCP_LOC_B) | (0x6 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x9, ((0xa << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0x4 << TENSOR_QUANT_QUANT_ROW) | (0x24 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x4 << TENSOR_QUANT_TRANSF5) | (0x4 << TENSOR_QUANT_TRANSF4) | (0x8 << TENSOR_QUANT_TRANSF3) | (0x5 << TENSOR_QUANT_TRANSF2) | (0x9 << TENSOR_QUANT_TRANSF1) | (0x5 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x10, ((0x0 << TENSOR_STORE_SCP_ENTRY_STRIDE) | (0x11 << TENSOR_STORE_SCP_START_SCP_ENTRY) | (0x0 << TENSOR_STORE_SCP_AROWS) | (0x1 << TENSOR_STORE_SCP_OPC_1) | (0x2067fdb10 << TENSOR_STORE_SCP_VADDR) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrw tensor_quant, x9\n"	
			"li x31, (0x3 << 6)\n"
			"csrwi tensor_wait, 7\n"
			"csrwi tensor_wait, 10\n"
			"csrw tensor_store, x10\n"	
			"csrwi tensor_wait, 8\n"
			"LBL_SEQID_2_TLD_XMA_STR_SEQ_ITER_6_HID_0:\n"
			"li x5, ((0x0 << TENSOR_LOAD_I8_USE_TMASK) | (0x0 << TENSOR_LOAD_I8_USE_COOP) | (0x1 << TENSOR_LOAD_I8_OPC_001) | (0x6 << TENSOR_LOAD_I8_DST_START) | (0x20001ec << TENSOR_LOAD_I8_VADDR) | (0x0 << TENSOR_LOAD_I8_OFFSET) | (0x7 << TENSOR_LOAD_I8_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_SETUP_B_USE_COOP) | (0x0 << TENSOR_LOAD_SETUP_B_OPC_000) | (0x1 << TENSOR_LOAD_SETUP_B_OPC_1) | (0x20585e840 << TENSOR_LOAD_SETUP_B_VADDR) | (0x6 << TENSOR_LOAD_SETUP_B_NUM_LINES) | 0 )\n" 
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x1 << TENSOR_IMA_B_NUM_COLS) | (0x7 << TENSOR_IMA_A_NUM_ROWS) | (0x6 << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x1 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1b << TENSOR_IMA_SCP_LOC_B) | (0x6 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x0 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x9, ((0x10 << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0x7 << TENSOR_QUANT_QUANT_ROW) | (0x1d << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x9 << TENSOR_QUANT_TRANSF5) | (0x8 << TENSOR_QUANT_TRANSF4) | (0x5 << TENSOR_QUANT_TRANSF3) | (0x4 << TENSOR_QUANT_TRANSF2) | (0x5 << TENSOR_QUANT_TRANSF1) | (0x4 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 7\n"
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_0\n"
			"csrw tensor_error, x0\n"
			"mov.m.x m0, x0, 0xFF\n"
			"li x5, 0x30\n"
			"la x31, rand_ieee754_32\n"
			"add x31, x31, x5\n"
			"flw.ps f0, 0(x31)\n"
			"flw.ps f11, 3(x31)\n"
			"flw.ps f13, 3(x31)\n"
			"flw.ps f14, 4(x31)\n"
			"flw.ps f16, 8(x31)\n"
			"flw.ps f18, 0(x31)\n"
			"flw.ps f19, 3(x31)\n"
			"flw.ps f20, 6(x31)\n"
			"flw.ps f23, 2(x31)\n"
			"flw.ps f26, 21(x31)\n"
			"flw.ps f28, 17(x31)\n"
			"flw.ps f3, 21(x31)\n"
			"flw.ps f31, 32(x31)\n"
			"flw.ps f5, 38(x31)\n"
			"flw.ps f7, 5(x31)\n"
			"flw.ps f9, 53(x31)\n"
			"la x31, rand_int_32\n"
			"add x31, x31, x5\n"
			"lw x0, 0(x31)\n"
			"lw x10, 3(x31)\n"
			"lw x11, 7(x31)\n"
			"lw x12, 7(x31)\n"
			"lw x13, 9(x31)\n"
			"lw x17, 6(x31)\n"
			"lw x18, 18(x31)\n"
			"lw x19, 17(x31)\n"
			"lw x28, 3(x31)\n"
			"lw x7, 9(x31)\n"
			"lw x9, 23(x31)\n"
			"li x21, 0x2\n"
			"LBL_SEQID_2_LOOP_SEQ_HID_0:\n"
			"srli x17, x13, 5\n"
			"fence\n"
			"fsra.pi f20, f28, f13\n"
			"ecall\n"
			"mulhu x17, x18, x28\n"
			"fmax.s f7, f18, f9\n"
			"frsq.ps f20, f5\n"
			"remuw x10, x18, x10\n"
			"blt x27, x6, 1f\n"
			"1:\n"
			"slliw x18, x9, 0\n"
			"fmin.pi f16, f13, f16\n"
			"bne x16, x9, 1f\n"
			"1:\n"
			"mulh x19, x19, x19\n"
			"bne x10, x13, 1f\n"
			"1:\n"
			"fsin.ps f31, f31\n"
			"fxor.pi f0, f14, f9\n"
			"fmin.ps f13, f23, f13\n"
			"divuw x28, x0, x11\n"
			"fmax.s f16, f0, f5\n"
			"bne x21, x12, 1f\n"
			"1:\n"
			"sraw x7, x13, x13\n"
			"mul x17, x10, x17\n"
			"fnmsub.s f0, f0, f23, f3, rmm\n"
			"divu x19, x9, x19\n"
			"divuw x13, x7, x11\n"
			"fswizz.ps f11, f26, 0xa\n"
			"fnmadd.s f20, f20, f0, f19, rtz\n"
			"flt.s x19, f5, f18\n"
			"mov.m.x m2, x28, 0x73\n"
			"slti x19, x7, 0x1d\n"
			"subw x19, x13, x10\n"
			"packb x13, x19, x19\n"
			"fsub.s f14, f7, f13, rup\n"
			"srai x10, x12, 1\n"
			"frsq.ps f3, f3\n"
			"frsq.ps f16, f16\n"
			"fle.ps f11, f16, f5\n"
			"sraw x17, x17, x11\n"
			"maskxor m2, m0, m3\n"
			"mov.m.x m0, x0, 0xff\n"
			"fsrai.pi f31, f7, 0x0\n"
			"fbcx.ps f7, x13\n"
			"fmsub.ps f26, f11, f14, f28, rdn\n"
			"fcmov.ps f16, f16, f14, f18\n"
			"blt x8, x15, 1f\n"
			"1:\n"
			"fcmov.ps f11, f28, f28, f11\n"
			"bge x1, x23, 1f\n"
			"1:\n"
			"remuw x17, x17, x11\n"
			"fnot.pi f26, f16\n"
			"fadd.pi f19, f13, f20\n"
			"addw x9, x13, x10\n"
			"feq.s x7, f18, f28\n"
			"slti x28, x12, 0x14\n"
			"fsin.ps f23, f13\n"
			"fcvt.pw.ps f18, f14, rne\n"
			"addiw x12, x12, 0x1a\n"
			"fmin.pi f28, f31, f0\n"
			"bltu x0, x26, 1f\n"
			"1:\n"
			"remu x13, x7, x0\n"
			"fbcx.ps f31, x0\n"
			"fmax.s f14, f5, f9\n"
			"mova.x.m x28\n"
			"ori x11, x17, 0x1f\n"
			"fround.ps f18, f19, rne\n"
			"flem.ps m3, f23, f18\n"
			"flt.s x9, f0, f28\n"
			"mov.m.x m2, x0, 0x7d\n"
			"fmv.s.x f16, x19\n"
			"blt x6, x15, 1f\n"
			"1:\n"
			"fpackreph.pi f26, f5\n"
			"maskxor m2, m0, m3\n"
			"bltu x6, x29, 1f\n"
			"1:\n"
			"fmul.pi f31, f23, f3\n"
			"bgeu x16, x12, 1f\n"
			"1:\n"
			"addi x17, x28, 0xf\n"
			"remw x13, x28, x13\n"
			"fmin.ps f13, f7, f16\n"
			"fcvt.f16.ps f13, f26\n"
			"fcvt.ps.pwu f28, f26, rdn\n"
			"add x13, x18, x11\n"
			"feq.s x10, f7, f20\n"
			"beq x27, x27, 1f\n"
			"1:\n"
			"fmin.pi f16, f14, f16\n"
			"bltu x12, x10, 1f\n"
			"1:\n"
			"auipc x18, 0x1c\n"
			"maskand m2, m2, m2\n"
			"fminu.pi f11, f31, f11\n"
			"bne x5, x11, 1f\n"
			"1:\n"
			"fcvt.w.s x13, f7, rup\n"
			"fround.ps f16, f7, rne\n"
			"fand.pi f0, f3, f23\n"
			"bge x29, x31, 1f\n"
			"1:\n"
			"fandi.pi f23, f20, 0xec\n"
			"maskor m0, m0, m0\n"
			"mov.m.x m0, x0, 0xff\n"
			"fle.ps f9, f19, f9\n"
			"mov.m.x m3, x9, 0x71\n"
			"fslli.pi f31, f19, 0x6\n"
			"fltm.ps m6, f11, f18\n"
			"fmin.pi f11, f0, f11\n"
			"remu x12, x12, x11\n"
			"fle.s x28, f14, f11\n"
			"beq x17, x8, 1f\n"
			"1:\n"
			"fmv.x.s x12, f19\n"
			"fbcx.ps f5, x18\n"
			"bne x26, x28, 1f\n"
			"1:\n"
			"fcvt.f16.ps f11, f7\n"
			"fle.pi f0, f5, f19\n"
			"bltu x20, x7, 1f\n"
			"1:\n"
			"mulw x0, x10, x0\n"
			"fmsub.ps f20, f3, f3, f20, rdn\n"
			"fbci.ps f3, 0x378ba\n"
			"divuw x19, x0, x13\n"
			"mov.m.x m0, x13, 0x62\n"
			"fbci.ps f31, 0x43ed3\n"
			"fmv.x.s x17, f9\n"
			"fslli.pi f13, f26, 0xf\n"
			"slliw x19, x11, 2\n"
			"fround.ps f13, f7, rmm\n"
			"sltiu x11, x18, 0x12\n"
			"bge x4, x3, 1f\n"
			"1:\n"
			"feq.pi f9, f19, f18\n"
			"maskand m6, m0, m0\n"
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m3, x17, 0x35\n"
			"sllw x28, x0, x28\n"
			"fcvt.ps.pwu f16, f16, rup\n"
			"fclass.s x10, f18\n"
			"fcvt.ps.pw f14, f14, rup\n"
			"fmin.s f13, f18, f9\n"
			"fcvt.ps.pw f14, f18, rtz\n"
			"fcmov.ps f9, f31, f0, f9\n"
			"fsgnjn.s f16, f18, f3\n"
			"addi x21, x21, -1\n"
			"bne x21, x0, LBL_SEQID_2_LOOP_SEQ_HID_0\n"
			"li x5, 3\n" 
			"bne x30, x5, LBL_FAIL_HID_0\n"
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f12,  352 (x5)\n"
			"flw.ps f25,  192 (x5)\n"
			"flw.ps f16,  832 (x5)\n"
			"flw.ps f21,  288 (x5)\n"
			"flw.ps f17,  384 (x5)\n"
			"flw.ps f19,  480 (x5)\n"
			"flw.ps f22,  256 (x5)\n"
			"flw.ps f5,  864 (x5)\n"
			"flw.ps f30,  960 (x5)\n"
			"flw.ps f4,  544 (x5)\n"
			"flw.ps f8,  128 (x5)\n"
			"flw.ps f29,  96 (x5)\n"
			"flw.ps f9,  992 (x5)\n"
			"flw.ps f3,  672 (x5)\n"
			"flw.ps f7,  736 (x5)\n"
			"flw.ps f20,  576 (x5)\n"
			"flw.ps f1,  608 (x5)\n"
			"flw.ps f18,  224 (x5)\n"
			"flw.ps f24,  0 (x5)\n"
			"flw.ps f10,  896 (x5)\n"
			"flw.ps f13,  800 (x5)\n"
			"flw.ps f2,  320 (x5)\n"
			"flw.ps f28,  416 (x5)\n"
			"flw.ps f31,  64 (x5)\n"
			"flw.ps f23,  928 (x5)\n"
			"flw.ps f14,  32 (x5)\n"
			"flw.ps f15,  768 (x5)\n"
			"flw.ps f26,  448 (x5)\n"
			"flw.ps f6,  704 (x5)\n"
			"flw.ps f0,  512 (x5)\n"
			"flw.ps f11,  640 (x5)\n"
			"flw.ps f27,  160 (x5)\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_3_INT_AMO_ID_68_HID_0:\n"
			"li x10, 0x81256400b4\n" 
			"amoswapg.w x18, x19, (x10)\n" 
			"fcvt.pwu.ps f14, f13, rtz\n"
			"frcp.ps f26, f26\n"
			"maskpopcz x13, m6\n"
			"fsra.pi f0, f0, f20\n"
			"fcvt.pwu.ps f19, f23, rup\n"
			"fpackreph.pi f9, f23\n"
			"bgeu x3, x18, 1f\n"
			"1:\n"
			"fmin.ps f18, f26, f13\n"
			"LBL_SEQID_3_INT_STORE_ID_69_HID_0:\n"
			"li x28, 0x82032c003e\n" 
			"sd x19, -1324(x28)\n" 
			"fmax.s f9, f9, f5\n"
			"fclass.ps f13, f19\n"
			"fnot.pi f26, f18\n"
			"bltu x13, x13, 1f\n"
			"1:\n"
			"fsgnj.s f26, f19, f19\n"
			"fxor.pi f9, f13, f5\n"
			"blt x12, x6, 1f\n"
			"1:\n"
			"flt.pi f19, f18, f19\n"
			"bge x12, x15, 1f\n"
			"1:\n"
			"faddi.pi f9, f9, 0xf7\n"
			"LBL_SEQID_3_FP_G_GATHER_ID_70_HID_0:\n"
			"li x11, 0x8123c49008\n" 
			"la x12, mem_ptr_offset\n"
			"flw.ps f28, 0(x12)\n"
			"fscwg.ps f27, f28 (x11)\n"
			"fghg.ps f7, f28 (x11)\n" 
			"feqm.ps m6, f0, f14\n"
			"slt x17, x9, x11\n"
			"fsatu8.pi f19, f5\n"
			"frcp.ps f31, f5\n"
			"bltu x11, x23, 1f\n"
			"1:\n"
			"divw x7, x13, x7\n"
			"fsra.pi f9, f31, f3\n"
			"bge x0, x18, 1f\n"
			"1:\n"
			"fswizz.ps f14, f11, 0x1\n"
			"LBL_SEQID_3_INT_STORE_ID_71_HID_0:\n"
			"li x10, 0x81017a1023\n" 
			"sw x28, -790(x10)\n" 
			"divu x13, x17, x17\n"
			"feq.pi f3, f9, f9\n"
			"masknot m2, m0\n"
			"mov.m.x m0, x0, 0xff\n"
			"bne x26, x30, 1f\n"
			"1:\n"
			"fmin.ps f19, f18, f18\n"
			"bge x26, x18, 1f\n"
			"1:\n"
			"fmvz.x.ps x18, f11, 7\n"
			"maskand m3, m0, m6\n"
			"fsgnjn.ps f19, f18, f11\n"
			"LBL_SEQID_3_FP_AMO_ID_72_HID_0:\n"
			"li x28, 0x81517a1020\n" 
			"la x10, mem_ptr_offset\n"
			"flw.ps f31, 640(x10)\n"
			"famomaxl.pi f31, f31 (x28)\n" 
			"li x5, 0xdf56\n"
			"csrw tensor_mask, x5\n"	
			"fle.s x0, f11, f19\n"
			"remw x9, x10, x17\n"
			"fbci.ps f19, 0x2c1d2\n"
			"fbci.ps f11, 0x70412\n"
			"bge x30, x30, 1f\n"
			"1:\n"
			"fadd.s f23, f7, f0, rmm\n"
			"fltu.pi f19, f19, f5\n"
			"addi x28, x9, 0xf\n"
			"LBL_SEQID_3_INT_AMO_ID_73_HID_0:\n"
			"li x7, 0x814e380000\n" 
			"amoorl.d x11, x12, (x7)\n" 
			"li x5, 0x6cad\n"
			"csrw tensor_mask, x5\n"	
			"fmadd.s f13, f14, f28, f5, rmm\n"
			"bltu x20, x27, 1f\n"
			"1:\n"
			"fclass.ps f20, f3\n"
			"beq x29, x26, 1f\n"
			"1:\n"
			"fmadd.s f14, f14, f31, f23, rmm\n"
			"beq x13, x9, 1f\n"
			"1:\n"
			"fadd.ps f7, f31, f23, rdn\n"
			"fadd.ps f13, f13, f16, rtz\n"
			"fsrl.pi f9, f9, f14\n"
			"fpackreph.pi f18, f9\n"
			"LBL_SEQID_3_CSR_ID_74_HID_0:\n"
			"li x12, 0x80aa380000093aa1\n"
			"csrw tensor_fma, x12\n" 
			"csrwi tensor_wait, 7\n" 
			"fxor.pi f0, f13, f14\n"
			"fmulhu.pi f18, f20, f31\n"
			"fcvt.pwu.ps f19, f16, rtz\n"
			"fle.ps f5, f5, f9\n"
			"fsatu8.pi f9, f20\n"
			"fandi.pi f5, f11, 0x7e\n"
			"fmax.pi f9, f9, f9\n"
			"LBL_SEQID_3_INT_AMO_ID_75_HID_0:\n"
			"li x7, 0x812e380000\n" 
			"amomaxug.d x13, x19, (x7)\n" 
			"li x5, 0x2845f0d228a91c2d\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0x7292f5b9e1c5c2f3\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"fmax.pi f3, f3, f5\n"
			"feq.ps f11, f0, f5\n"
			"bne x13, x11, 1f\n"
			"1:\n"
			"fle.ps f0, f3, f16\n"
			"fcvt.ps.pwu f19, f5, rne\n"
			"fmaxu.pi f18, f13, f23\n"
			"fle.pi f13, f31, f11\n"
			"fsrli.pi f18, f20, 0x5\n"
			"LBL_SEQID_3_INT_STORE_ID_76_HID_0:\n"
			"li x11, 0x810c940125\n" 
			"sw x7, -645(x11)\n" 
			"fclass.s x11, f0\n"
			"fsrl.pi f26, f5, f28\n"
			"fsgnj.ps f0, f5, f11\n"
			"bge x12, x15, 1f\n"
			"1:\n"
			"slliw x18, x18, 4\n"
			"fsrli.pi f26, f26, 0x1\n"
			"bltu x9, x22, 1f\n"
			"1:\n"
			"fsra.pi f5, f5, f16\n"
			"mova.x.m x19\n"
			"LBL_SEQID_3_FP_SCATTER_ID_77_HID_0:\n"
			"li x11, 0x810035a03d\n" 
			"la x12, mem_ptr_offset\n"
			"flq2 f28, 4(x12)\n"
			"fscb.ps f5, f28 (x11)\n" 
			"srliw x19, x19, 3\n"
			"fsgnjn.ps f5, f16, f9\n"
			"sltu x19, x19, x19\n"
			"fmul.pi f0, f0, f18\n"
			"fsub.ps f0, f28, f5, rne\n"
			"fsra.pi f0, f31, f13\n"
			"fmsub.ps f28, f0, f28, f19, rne\n"
			"LBL_SEQID_3_INT_AMO_ID_78_HID_0:\n"
			"li x13, 0x8221a0ffb8\n" 
			"amominug.d x11, x17, (x13)\n" 
			"fcvt.ps.pwu f9, f9, rtz\n"
			"fexp.ps f13, f28\n"
			"bltu x19, x10, 1f\n"
			"1:\n"
			"fmin.ps f5, f20, f19\n"
			"for.pi f5, f11, f23\n"
			"fsgnjx.ps f9, f9, f7\n"
			"bge x30, x25, 1f\n"
			"1:\n"
			"fmin.ps f9, f31, f9\n"
			"fsgnj.ps f13, f11, f11\n"
			"LBL_SEQID_3_INT_STORE_ID_79_HID_0:\n"
			"li x7, 0x81038e003d\n" 
			"sw x18, -416(x7)\n" 
			"fmaxu.pi f7, f23, f7\n"
			"fmsub.ps f5, f11, f16, f20, rmm\n"
			"ffrc.ps f3, f16\n"
			"fnot.pi f7, f14\n"
			"fsgnjx.s f3, f0, f31\n"
			"fslli.pi f26, f3, 0x5\n"
			"fand.pi f28, f18, f13\n"
			"beq x28, x25, 1f\n"
			"1:\n"
			"LBL_SEQID_3_INT_AMO_ID_80_HID_0:\n"
			"li x11, 0x8237d00748\n" 
			"amominug.d x7, x10, (x11)\n" 
			"fsub.pi f0, f16, f11\n"
			"fmadd.s f13, f18, f14, f26, rup\n"
			"blt x5, x29, 1f\n"
			"1:\n"
			"fsgnj.ps f11, f3, f11\n"
			"fandi.pi f3, f3, 0x169\n"
			"srliw x0, x12, 0\n"
			"bgeu x31, x3, 1f\n"
			"1:\n"
			"fswizz.ps f3, f3, 0xe\n"
			"feqm.ps m0, f3, f26\n"
			"blt x12, x3, 1f\n"
			"1:\n"
			"LBL_SEQID_3_INT_AMO_ID_81_HID_0:\n"
			"li x12, 0x812b6ff11c\n" 
			"amoorg.w x7, x17, (x12)\n" 
			"fmulh.pi f26, f3, f9\n"
			"fcvt.ps.f16 f3, f3\n"
			"fmul.s f7, f7, f3, rmm\n"
			"fsub.s f26, f31, f26, rne\n"
			"bne x22, x11, 1f\n"
			"1:\n"
			"fnmadd.ps f18, f9, f31, f31, rdn\n"
			"fpackreph.pi f7, f7\n"
			"bne x11, x21, 1f\n"
			"1:\n"
			"fsin.ps f5, f7\n"
			"LBL_SEQID_3_FP_SCATTER_ID_82_HID_0:\n"
			"li x7, 0x810035a03d\n" 
			"li x28, 0x1324c68c32cb417a\n"
			"fsc32w.ps f0, x28 (x7)\n" 
			"fsgnj.s f3, f14, f7\n"
			"frcp.ps f31, f0\n"
			"fnmsub.ps f14, f18, f5, f5, rne\n"
			"fsin.ps f9, f18\n"
			"fcvt.pwu.ps f18, f5, rne\n"
			"fcvt.ps.pw f7, f18, rup\n"
			"beq x15, x13, 1f\n"
			"1:\n"
			"fcmov.ps f11, f18, f0, f9\n"
			"LBL_SEQID_3_FP_LOAD_ID_83_HID_0:\n"
			"li x10, 0x81016a0004\n" 
			"fsw.ps f5, 1203(x10)\n"
			"flw.ps f19, 1203(x10)\n" 
			"li x5, 0xe9779aa7be16da47\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0x92c97e7430277ba7\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"fsrai.pi f13, f14, 0x9\n"
			"fslli.pi f23, f23, 0x2\n"
			"fcvt.ps.pw f31, f20, rmm\n"
			"fmax.ps f31, f23, f20\n"
			"fmul.pi f13, f14, f14\n"
			"bgeu x28, x1, 1f\n"
			"1:\n"
			"fnmsub.ps f20, f0, f20, f28, rdn\n"
			"fmsub.s f26, f0, f14, f26, rdn\n"
			"bltu x6, x27, 1f\n"
			"1:\n"
			"LBL_SEQID_3_CSR_ID_84_HID_0:\n"
			"csrr x0, scounteren\n"
			"sraiw x17, x13, 5\n"
			"fcmovm.ps f19, f23, f19\n"
			"packb x10, x18, x18\n"
			"fcmovm.ps f16, f26, f26\n"
			"feq.ps f19, f26, f18\n"
			"lui x18, 0xf\n"
			"fsgnj.s f20, f20, f19\n"
			"LBL_SEQID_3_FP_L_GATHER_ID_85_HID_0:\n"
			"li x10, 0x8244a20010\n" 
			"la x11, mem_ptr_offset\n"
			"flq2 f13, 216(x11)\n"
			"fscwl.ps f0, f13 (x10)\n"
			"fghl.ps f18, f13 (x10)\n" 
			"fcvt.s.w f26, x13, rmm\n"
			"divw x0, x12, x17\n"
			"fbcx.ps f0, x11\n"
			"fltm.ps m2, f20, f9\n"
			"bgeu x6, x31, 1f\n"
			"1:\n"
			"fpackreph.pi f28, f19\n"
			"fround.ps f18, f7, rdn\n"
			"bne x29, x4, 1f\n"
			"1:\n"
			"fmvz.x.ps x0, f26, 2\n"
			"beq x10, x30, 1f\n"
			"1:\n"
			"LBL_SEQID_3_INT_AMO_ID_86_HID_0:\n"
			"li x12, 0x8233860a28\n" 
			"amoorg.d x7, x10, (x12)\n" 
			"fcvt.ps.pw f18, f5, rne\n"
			"fsgnj.ps f26, f16, f16\n"
			"fsub.pi f14, f14, f28\n"
			"fmax.s f14, f19, f14\n"
			"faddi.pi f18, f9, 0x1db\n"
			"fandi.pi f9, f20, 0x19e\n"
			"fmax.s f20, f9, f20\n"
			"LBL_SEQID_3_FP_AMO_ID_87_HID_0:\n"
			"li x28, 0x8224a20010\n" 
			"la x11, mem_ptr_offset\n"
			"flw.ps f19, 576(x11)\n"
			"famoxorg.pi f18, f19 (x28)\n" 
			"fclass.s x28, f23\n"
			"fcvt.wu.s x18, f16, rne\n"
			"fbcx.ps f14, x11\n"
			"bne x30, x13, 1f\n"
			"1:\n"
			"fpackrepb.pi f26, f13\n"
			"fsin.ps f11, f31\n"
			"fadd.pi f19, f28, f28\n"
			"bge x4, x19, 1f\n"
			"1:\n"
			"srl x10, x12, x10\n"
			"LBL_SEQID_3_INT_LOAD_ID_88_HID_0:\n"
			"li x13, 0x81038e003d\n" 
			"sd x28, 908(x13)\n"
			"ld x9, 908(x13)\n" 
			"fmulh.pi f19, f5, f20\n"
			"addiw x11, x7, 0xe\n"
			"fadd.pi f19, f3, f19\n"
			"srliw x7, x28, 4\n"
			"fmadd.ps f19, f19, f19, f23, rdn\n"
			"fpackreph.pi f13, f5\n"
			"fadd.ps f31, f3, f31, rne\n"
			"LBL_SEQID_3_FP_STORE_ID_89_HID_0:\n"
			"li x13, 0x820c89aec9\n" 
			"fsw.ps f7, -350(x13)\n" 
			"fsgnjx.s f3, f23, f19\n"
			"frcp.ps f23, f23\n"
			"fcmovm.ps f13, f13, f19\n"
			"fmadd.s f0, f28, f20, f9, rmm\n"
			"fsrli.pi f9, f16, 0x8\n"
			"fcvt.pw.ps f14, f5, rdn\n"
			"bne x2, x2, 1f\n"
			"1:\n"
			"fmul.ps f19, f23, f31, rtz\n"
			"LBL_SEQID_3_INT_AMO_ID_90_HID_0:\n"
			"li x17, 0x8121deb814\n" 
			"amoxorg.w x7, x9, (x17)\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"xor x13, x28, x9\n"
			"flt.ps f18, f18, f31\n"
			"frsq.ps f16, f23\n"
			"xori x11, x11, 0xf\n"
			"srli x13, x12, 0\n"
			"fadd.s f13, f9, f5, rmm\n"
			"mulh x17, x0, x17\n"
			"bltu x5, x17, 1f\n"
			"1:\n"
			"LBL_SEQID_3_INT_AMO_ID_91_HID_0:\n"
			"li x9, 0x812035a03c\n" 
			"amoxorg.w x12, x7, (x9)\n" 
			"fcvt.ps.f16 f23, f23\n"
			"fnmsub.s f20, f14, f20, f11, rne\n"
			"fsatu8.pi f18, f23\n"
			"blt x8, x10, 1f\n"
			"1:\n"
			"fsin.ps f13, f23\n"
			"beq x17, x22, 1f\n"
			"1:\n"
			"fltu.pi f20, f23, f20\n"
			"fsrl.pi f0, f11, f11\n"
			"fcvt.f16.ps f20, f14\n"
			"LBL_SEQID_3_INT_AMO_ID_92_HID_0:\n"
			"li x9, 0x8132be00a8\n" 
			"amoaddg.d x10, x19, (x9)\n" 
			"fsgnjx.s f7, f7, f9\n"
			"blt x24, x7, 1f\n"
			"1:\n"
			"fmax.pi f16, f9, f3\n"
			"fadd.ps f5, f16, f5, rne\n"
			"fnmsub.ps f19, f7, f9, f19, rup\n"
			"fmin.s f16, f5, f11\n"
			"beq x17, x17, 1f\n"
			"1:\n"
			"flt.pi f18, f5, f5\n"
			"bltu x27, x7, 1f\n"
			"1:\n"
			"fadd.s f11, f11, f3, rne\n"
			"LBL_SEQID_3_CSR_ID_93_HID_0:\n"
			"li x11, 0x10205edbfc4703\n"
			"li x31, (0x3 << 6) | 0x1\n"
			"csrw tensor_load, x11\n" 
			"fnmsub.s f19, f28, f16, f23, rdn\n"
			"feq.pi f14, f19, f19\n"
			"fmsub.ps f26, f26, f19, f14, rne\n"
			"fcvt.ps.pw f13, f16, rtz\n"
			"fmulhu.pi f11, f23, f19\n"
			"feq.pi f14, f14, f28\n"
			"fmadd.ps f31, f28, f14, f9, rtz\n"
			"LBL_SEQID_3_FP_L_STORE_ID_94_HID_0:\n"
			"li x12, 0x8247d00740\n" 
			"mov.m.x m0, zero, 0xff\n"
			"fswl.ps f20, (x12)\n" 
			"fsgnjn.ps f28, f28, f13\n"
			"feqm.ps m3, f18, f18\n"
			"fmin.ps f19, f18, f11\n"
			"fadd.ps f14, f14, f20, rmm\n"
			"sra x18, x0, x28\n"
			"fcvt.f16.ps f13, f28\n"
			"masknot m2, m3\n"
			"LBL_SEQID_3_INT_AMO_ID_95_HID_0:\n"
			"li x13, 0x82512a0028\n" 
			"amomaxul.d x17, x28, (x13)\n" 
			"fltm.ps m2, f5, f19\n"
			"fsatu8.pi f14, f28\n"
			"mulw x7, x7, x13\n"
			"divw x12, x28, x13\n"
			"fsub.ps f31, f3, f28, rup\n"
			"or x0, x0, x0\n"
			"fbci.ps f9, 0x27fc1\n"
			"LBL_SEQID_3_INT_LOAD_ID_96_HID_0:\n"
			"li x12, 0x8101deb817\n" 
			"sd x7, -857(x12)\n"
			"ld x11, -857(x12)\n" 
			"li x5, 0xe302c9e55934835\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0xc60d5d38e4ce9d7c\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"fsub.pi f18, f5, f9\n"
			"fsin.ps f11, f9\n"
			"fsgnjx.ps f13, f13, f5\n"
			"fmul.ps f16, f13, f5, rne\n"
			"fsgnjx.ps f18, f5, f9\n"
			"fsat8.pi f11, f9\n"
			"bgeu x6, x9, 1f\n"
			"1:\n"
			"fnmadd.s f18, f9, f9, f9, rne\n"
			"beq x2, x19, 1f\n"
			"1:\n"
			"LBL_SEQID_3_FP_STORE_ID_97_HID_0:\n"
			"li x18, 0x81056400b4\n" 
			"fsw.ps f31, 569(x18)\n" 
			"fcmovm.ps f14, f18, f11\n"
			"fltm.ps m6, f9, f18\n"
			"bltu x20, x6, 1f\n"
			"1:\n"
			"fmax.pi f3, f3, f14\n"
			"fmv.x.s x12, f18\n"
			"maskxor m0, m6, m0\n"
			"fmul.ps f3, f11, f9, rup\n"
			"fswizz.ps f9, f14, 0x1c\n"
			"LBL_SEQID_3_INT_STORE_ID_98_HID_0:\n"
			"li x28, 0x810c940125\n" 
			"sd x11, 1553(x28)\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"feq.ps f14, f31, f7\n"
			"fle.ps f28, f20, f31\n"
			"fle.ps f3, f13, f13\n"
			"bne x17, x13, 1f\n"
			"1:\n"
			"fmsub.ps f31, f31, f31, f31, rtz\n"
			"blt x8, x3, 1f\n"
			"1:\n"
			"fsrai.pi f14, f16, 0x9\n"
			"fmadd.s f28, f31, f31, f31, rtz\n"
			"fadd.ps f14, f20, f3, rup\n"
			"LBL_SEQID_3_FP_AMO_ID_99_HID_0:\n"
			"li x12, 0x8249bee830\n" 
			"la x13, mem_ptr_offset\n"
			"flw.ps f7, 704(x13)\n"
			"famomaxl.pi f31, f7 (x12)\n" 
			"addw x10, x7, x19\n"
			"maskpopcz x19, m2\n"
			"fsgnjn.s f18, f11, f20\n"
			"bltu x4, x18, 1f\n"
			"1:\n"
			"fcvt.ps.pwu f9, f16, rdn\n"
			"addiw x28, x13, 0x1e\n"
			"fcvt.ps.f16 f19, f20\n"
			"sltiu x0, x0, 0x1c\n"
			"li x5,  0x1ff800000610007\n" 
			"csrw tensor_fma, x5\n"
			"csrwi tensor_wait, 7\n"
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_3_TLD_XMA_STR_SEQ_ID_552344_HID_0:\n"
			"la x31, self_check_16\n"
			"li x5, 0xa0000000000003\n"
			"add x5, x5, x31\n"
			"li x6, 0x10000000000008\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x0 << TENSOR_FMA16_B_NUM_COLS) | (0x3 << TENSOR_FMA16_A_NUM_ROWS) | (0x8 << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x1 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1b << TENSOR_FMA16_SCP_LOC_B) | (0x5 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x5 << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0x3 << TENSOR_QUANT_QUANT_ROW) | (0x1a << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x0 << TENSOR_QUANT_TRANSF1) | (0x0 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0x12\n"
			"bne x5, x31, LBL_FAIL_HID_0\n"
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 10\n"
			"LBL_SEQID_3_TLD_XMA_STR_SEQ_ID_705283_HID_0:\n"
			"li x5, ((0x0 << TENSOR_LOAD_I16_USE_TMASK) | (0x0 << TENSOR_LOAD_I16_USE_COOP) | (0x2 << TENSOR_LOAD_I16_OPC_010) | (0x0 << TENSOR_LOAD_I16_DST_START) | (0x2fe00fd << TENSOR_LOAD_I16_VADDR) | (0x0 << TENSOR_LOAD_I16_OFFSET) | (0xf << TENSOR_LOAD_I16_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_I8_USE_TMASK) | (0x0 << TENSOR_LOAD_I8_USE_COOP) | (0x1 << TENSOR_LOAD_I8_OPC_001) | (0x19 << TENSOR_LOAD_I8_DST_START) | (0x205877ae0 << TENSOR_LOAD_I8_VADDR) | (0x0 << TENSOR_LOAD_I8_OFFSET) | (0xf << TENSOR_LOAD_I8_NUM_LINES) | 0 )\n" 
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x3 << TENSOR_FMA32_B_NUM_COLS) | (0xf << TENSOR_FMA32_A_NUM_ROWS) | (0xf << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x19 << TENSOR_FMA32_SCP_LOC_B) | (0x0 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x0 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x10, ((0x2 << TENSOR_STORE_REG_STRIDE) | (0x14 << TENSOR_STORE_START_REG) | (0x3 << TENSOR_STORE_STORE_COLS) | (0x1 << TENSOR_STORE_AROWS) | (0x0 << TENSOR_STORE_COOP_STORE) | (0x0 << TENSOR_STORE_OPC_0) | (0x817896e01 << TENSOR_STORE_VADDR) | 0 )\n" 
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
			"LBL_SEQID_3_TLD_XMA_STR_SEQ_ID_437601_HID_0:\n"
			"la x31, self_check_16\n"
			"li x5, 0xc000000000000a\n"
			"add x5, x5, x31\n"
			"li x6, 0x380000000000002\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x3 << TENSOR_FMA16_B_NUM_COLS) | (0xa << TENSOR_FMA16_A_NUM_ROWS) | (0x2 << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1c << TENSOR_FMA16_SCP_LOC_B) | (0x6 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x1b << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0xa << TENSOR_QUANT_QUANT_ROW) | (0x6 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x2 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0x6\n"
			"bne x5, x31, LBL_FAIL_HID_0\n"
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 10\n"
			"LBL_SEQID_3_TLD_XMA_STR_SEQ_ID_166139_HID_0:\n"
			"la x31, self_check_32\n"
			"li x5, 0xe000000000000d\n"
			"add x5, x5, x31\n"
			"li x6, 0x1000000000000e\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x0 << TENSOR_FMA32_B_NUM_COLS) | (0xd << TENSOR_FMA32_A_NUM_ROWS) | (0xe << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x1 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1e << TENSOR_FMA32_SCP_LOC_B) | (0x7 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0xf\n"
			"bne x5, x31, LBL_FAIL_HID_0\n"
			"LBL_SEQID_3_TLD_XMA_STR_SEQ_ID_660252_HID_0:\n"
			"la x31, self_check_16\n"
			"li x5, 0xc0000000000009\n"
			"add x5, x5, x31\n"
			"li x6, 0x1000000000000b\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x2 << TENSOR_FMA16_B_NUM_COLS) | (0x9 << TENSOR_FMA16_A_NUM_ROWS) | (0xb << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x1 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1a << TENSOR_FMA16_SCP_LOC_B) | (0x6 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x11 << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0x9 << TENSOR_QUANT_QUANT_ROW) | (0x19 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x2 << TENSOR_QUANT_TRANSF4) | (0x7 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0x18\n"
			"bne x5, x31, LBL_FAIL_HID_0\n"
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 10\n"
			"LBL_SEQID_3_TLD_XMA_STR_SEQ_ID_38494_HID_0:\n"
			"li x5, ((0x0 << TENSOR_LOAD_T8_USE_TMASK) | (0x0 << TENSOR_LOAD_T8_USE_COOP) | (0x5 << TENSOR_LOAD_T8_OPC_101) | (0x4 << TENSOR_LOAD_T8_DST_START) | (0x205bfdb10 << TENSOR_LOAD_T8_VADDR) | (0x0 << TENSOR_LOAD_T8_OFFSET) | (0xb << TENSOR_LOAD_T8_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_T32_USE_TMASK) | (0x0 << TENSOR_LOAD_T32_USE_COOP) | (0x7 << TENSOR_LOAD_T32_OPC_111) | (0x1c << TENSOR_LOAD_T32_DST_START) | (0x2064eb4bd << TENSOR_LOAD_T32_VADDR) | (0x8 << TENSOR_LOAD_T32_NUM_LINES) | 0 )\n" 
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x0 << TENSOR_IMA_B_NUM_COLS) | (0xb << TENSOR_IMA_A_NUM_ROWS) | (0x8 << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x0 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1c << TENSOR_IMA_SCP_LOC_B) | (0x4 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x0 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x9, ((0x2 << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0xb << TENSOR_QUANT_QUANT_ROW) | (0x14 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x0 << TENSOR_QUANT_TRANSF1) | (0x0 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x10, ((0x1 << TENSOR_STORE_REG_STRIDE) | (0x3 << TENSOR_STORE_START_REG) | (0x1 << TENSOR_STORE_STORE_COLS) | (0x5 << TENSOR_STORE_AROWS) | (0x0 << TENSOR_STORE_COOP_STORE) | (0x0 << TENSOR_STORE_OPC_0) | (0x80007fc << TENSOR_STORE_VADDR) | 0 )\n" 
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
			"LBL_SEQID_3_TLD_XMA_STR_SEQ_ID_445481_HID_0:\n"
			"li x5, ((0x0 << TENSOR_LOAD_T8_USE_TMASK) | (0x0 << TENSOR_LOAD_T8_USE_COOP) | (0x5 << TENSOR_LOAD_T8_OPC_101) | (0x3 << TENSOR_LOAD_T8_DST_START) | (0x2062dbfc4 << TENSOR_LOAD_T8_VADDR) | (0x0 << TENSOR_LOAD_T8_OFFSET) | (0xb << TENSOR_LOAD_T8_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_T16_USE_TMASK) | (0x0 << TENSOR_LOAD_T16_USE_COOP) | (0x6 << TENSOR_LOAD_T16_OPC_110) | (0x1a << TENSOR_LOAD_T16_DST_START) | (0x20a26fba0 << TENSOR_LOAD_T16_VADDR) | (0x0 << TENSOR_LOAD_T16_OFFSET) | (0xe << TENSOR_LOAD_T16_NUM_LINES) | 0 )\n" 
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x2 << TENSOR_FMA16_B_NUM_COLS) | (0xb << TENSOR_FMA16_A_NUM_ROWS) | (0xe << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1a << TENSOR_FMA16_SCP_LOC_B) | (0x3 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x0 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x14 << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0xb << TENSOR_QUANT_QUANT_ROW) | (0x35 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x7 << TENSOR_QUANT_TRANSF4) | (0x6 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"LBL_SEQID_3_TLD_XMA_STR_SEQ_ID_532203_HID_0:\n"
			"li x5, ((0x0 << TENSOR_LOAD_USE_TMASK) | (0x0 << TENSOR_LOAD_USE_COOP) | (0x0 << TENSOR_LOAD_OPC_000) | (0x1 << TENSOR_LOAD_DST_START) | (0x0 << TENSOR_LOAD_OPC_0) | (0x205f3a940 << TENSOR_LOAD_VADDR) | (0x0 << TENSOR_LOAD_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_T8_USE_TMASK) | (0x0 << TENSOR_LOAD_T8_USE_COOP) | (0x5 << TENSOR_LOAD_T8_OPC_101) | (0x18 << TENSOR_LOAD_T8_DST_START) | (0x2000107 << TENSOR_LOAD_T8_VADDR) | (0x0 << TENSOR_LOAD_T8_OFFSET) | (0x5 << TENSOR_LOAD_T8_NUM_LINES) | 0 )\n" 
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x0 << TENSOR_IMA_B_NUM_COLS) | (0x0 << TENSOR_IMA_A_NUM_ROWS) | (0x5 << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x18 << TENSOR_IMA_SCP_LOC_B) | (0x1 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x0 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x10, ((0x3 << TENSOR_STORE_REG_STRIDE) | (0xb << TENSOR_STORE_START_REG) | (0x3 << TENSOR_STORE_STORE_COLS) | (0x8 << TENSOR_STORE_AROWS) | (0x0 << TENSOR_STORE_COOP_STORE) | (0x0 << TENSOR_STORE_OPC_0) | (0x80007fc << TENSOR_STORE_VADDR) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"li x31, (0xc0 << 4)\n"
			"csrw tensor_store, x10\n"	
			"csrwi tensor_wait, 7\n"
			"csrwi tensor_wait, 8\n"
			"LBL_SEQID_3_TLD_XMA_STR_SEQ_ID_799790_HID_0:\n"
			"la x31, self_check_8\n"
			"li x5, 0x20000000000007\n"
			"add x5, x5, x31\n"
			"li x6, 0x10000000000003\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x3 << TENSOR_IMA_B_NUM_COLS) | (0x7 << TENSOR_IMA_A_NUM_ROWS) | (0x3 << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x1 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1a << TENSOR_IMA_SCP_LOC_B) | (0x1 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x9, ((0x19 << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0x7 << TENSOR_QUANT_QUANT_ROW) | (0x10 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x1 << TENSOR_QUANT_TRANSF3) | (0x3 << TENSOR_QUANT_TRANSF2) | (0x4 << TENSOR_QUANT_TRANSF1) | (0xa << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x10, ((0x1 << TENSOR_STORE_SCP_ENTRY_STRIDE) | (0x35 << TENSOR_STORE_SCP_START_SCP_ENTRY) | (0xb << TENSOR_STORE_SCP_AROWS) | (0x1 << TENSOR_STORE_SCP_OPC_1) | (0x2098e187d << TENSOR_STORE_SCP_VADDR) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fmvz.x.ps x31, f0, 0\n"
			"li x5, 0x10\n"
			"bne x5, x31, LBL_FAIL_HID_0\n"
			"csrw tensor_quant, x9\n"	
			"li x31, (0x2 << 6)\n"
			"csrwi tensor_wait, 10\n"
			"csrw tensor_store, x10\n"	
			"csrwi tensor_wait, 8\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_0\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_3_XMA_QNT_STR_SEQ_ITER_0_HID_0:\n"
			"li x5, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x0 << TENSOR_FMA16_B_NUM_COLS) | (0x2 << TENSOR_FMA16_A_NUM_ROWS) | (0xf << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x99 << TENSOR_FMA16_SCP_LOC_B) | (0xae << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x6, ((0x18 << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0x2 << TENSOR_QUANT_QUANT_ROW) | (0x27 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x2 << TENSOR_QUANT_TRANSF8) | (0x7 << TENSOR_QUANT_TRANSF7) | (0x6 << TENSOR_QUANT_TRANSF6) | (0x6 << TENSOR_QUANT_TRANSF5) | (0x7 << TENSOR_QUANT_TRANSF4) | (0x7 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x7, ((0x3 << TENSOR_STORE_SCP_ENTRY_STRIDE) | (0xd << TENSOR_STORE_SCP_START_SCP_ENTRY) | (0xf << TENSOR_STORE_SCP_AROWS) | (0x1 << TENSOR_STORE_SCP_OPC_1) | (0x2fe01d4 << TENSOR_STORE_SCP_VADDR) | 0 )\n" 
			"li x31, (0x1 << 6)\n"
			"csrw tensor_fma, x5\n"	
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 7\n"
			"csrwi tensor_wait, 10\n"
			"csrw tensor_store, x7\n"	
			"csrwi tensor_wait, 8\n"
			"LBL_SEQID_3_XMA_QNT_STR_SEQ_ITER_1_HID_0:\n"
			"li x5, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x0 << TENSOR_IMA_B_NUM_COLS) | (0xa << TENSOR_IMA_A_NUM_ROWS) | (0xf << TENSOR_IMA_A_NUM_COLS) | (0x5 << TENSOR_IMA_OFFSET) | (0x0 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1a << TENSOR_IMA_SCP_LOC_B) | (0x74 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x6, ((0x6 << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0xa << TENSOR_QUANT_QUANT_ROW) | (0x6 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0xa << TENSOR_QUANT_TRANSF2) | (0x9 << TENSOR_QUANT_TRANSF1) | (0x5 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x7, ((0x2 << TENSOR_STORE_SCP_ENTRY_STRIDE) | (0x17 << TENSOR_STORE_SCP_START_SCP_ENTRY) | (0x9 << TENSOR_STORE_SCP_AROWS) | (0x1 << TENSOR_STORE_SCP_OPC_1) | (0x20645e840 << TENSOR_STORE_SCP_VADDR) | 0 )\n" 
			"li x31, (0x2 << 6)\n"
			"csrw tensor_fma, x5\n"	
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 7\n"
			"csrwi tensor_wait, 10\n"
			"csrw tensor_store, x7\n"	
			"csrwi tensor_wait, 8\n"
			"LBL_SEQID_3_XMA_QNT_STR_SEQ_ITER_2_HID_0:\n"
			"li x5, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x0 << TENSOR_FMA32_B_NUM_COLS) | (0x0 << TENSOR_FMA32_A_NUM_ROWS) | (0x8 << TENSOR_FMA32_A_NUM_COLS) | (0xf << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x4d << TENSOR_FMA32_SCP_LOC_B) | (0xe3 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x0 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x6, ((0x1f << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0x0 << TENSOR_QUANT_QUANT_ROW) | (0x3d << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x7 << TENSOR_QUANT_TRANSF7) | (0x6 << TENSOR_QUANT_TRANSF6) | (0x7 << TENSOR_QUANT_TRANSF5) | (0x7 << TENSOR_QUANT_TRANSF4) | (0x6 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x7, ((0x0 << TENSOR_STORE_SCP_ENTRY_STRIDE) | (0xd << TENSOR_STORE_SCP_START_SCP_ENTRY) | (0x1 << TENSOR_STORE_SCP_AROWS) | (0x1 << TENSOR_STORE_SCP_OPC_1) | (0x2062dbfc4 << TENSOR_STORE_SCP_VADDR) | 0 )\n" 
			"li x31, (0x3 << 6)\n"
			"csrw tensor_fma, x5\n"	
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 7\n"
			"csrwi tensor_wait, 10\n"
			"csrw tensor_store, x7\n"	
			"csrwi tensor_wait, 8\n"
			"LBL_SEQID_3_XMA_QNT_STR_SEQ_ITER_3_HID_0:\n"
			"li x5, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x0 << TENSOR_FMA16_B_NUM_COLS) | (0x4 << TENSOR_FMA16_A_NUM_ROWS) | (0xc << TENSOR_FMA16_A_NUM_COLS) | (0x4 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0xed << TENSOR_FMA16_SCP_LOC_B) | (0xb5 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x0 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x7, ((0x0 << TENSOR_STORE_SCP_ENTRY_STRIDE) | (0x16 << TENSOR_STORE_SCP_START_SCP_ENTRY) | (0x4 << TENSOR_STORE_SCP_AROWS) | (0x1 << TENSOR_STORE_SCP_OPC_1) | (0x20a4a7dc0 << TENSOR_STORE_SCP_VADDR) | 0 )\n" 
			"li x31, (0x1 << 6)\n"
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
			"csrw tensor_store, x7\n"	
			"csrwi tensor_wait, 8\n"
			"LBL_SEQID_3_XMA_QNT_STR_SEQ_ITER_4_HID_0:\n"
			"li x6, ((0x12 << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0xb << TENSOR_QUANT_QUANT_ROW) | (0xb << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x2 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x7, ((0x0 << TENSOR_STORE_SCP_ENTRY_STRIDE) | (0x2e << TENSOR_STORE_SCP_START_SCP_ENTRY) | (0x7 << TENSOR_STORE_SCP_AROWS) | (0x1 << TENSOR_STORE_SCP_OPC_1) | (0x20001b1 << TENSOR_STORE_SCP_VADDR) | 0 )\n" 
			"li x31, (0x3 << 6)\n"
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 10\n"
			"csrw tensor_store, x7\n"	
			"csrwi tensor_wait, 8\n"
			"LBL_SEQID_3_XMA_QNT_STR_SEQ_ITER_5_HID_0:\n"
			"li x5, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x2 << TENSOR_FMA32_B_NUM_COLS) | (0x1 << TENSOR_FMA32_A_NUM_ROWS) | (0xd << TENSOR_FMA32_A_NUM_COLS) | (0xd << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x4 << TENSOR_FMA32_SCP_LOC_B) | (0x6e << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x0 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x6, ((0x1c << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0x1 << TENSOR_QUANT_QUANT_ROW) | (0x1b << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x2 << TENSOR_QUANT_TRANSF7) | (0x6 << TENSOR_QUANT_TRANSF6) | (0x6 << TENSOR_QUANT_TRANSF5) | (0x7 << TENSOR_QUANT_TRANSF4) | (0x6 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_3_XMA_QNT_STR_SEQ_ITER_6_HID_0:\n"
			"li x5, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x1 << TENSOR_IMA_B_NUM_COLS) | (0x9 << TENSOR_IMA_A_NUM_ROWS) | (0xd << TENSOR_IMA_A_NUM_COLS) | (0xa << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x7a << TENSOR_IMA_SCP_LOC_B) | (0x83 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x0 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x6, ((0x7 << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0x9 << TENSOR_QUANT_QUANT_ROW) | (0x0 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x1 << TENSOR_QUANT_TRANSF4) | (0x8 << TENSOR_QUANT_TRANSF3) | (0x3 << TENSOR_QUANT_TRANSF2) | (0x8 << TENSOR_QUANT_TRANSF1) | (0x3 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 7\n"
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_0\n"
			"csrw tensor_error, x0\n"
			"mov.m.x m0, x0, 0xFF\n"
			"li x5, 0x20\n"
			"la x31, rand_ieee754_32\n"
			"add x31, x31, x5\n"
			"flw.ps f11, 0(x31)\n"
			"flw.ps f14, 4(x31)\n"
			"flw.ps f18, 6(x31)\n"
			"flw.ps f19, 11(x31)\n"
			"flw.ps f28, 13(x31)\n"
			"flw.ps f3, 17(x31)\n"
			"flw.ps f5, 7(x31)\n"
			"flw.ps f9, 7(x31)\n"
			"la x31, rand_int_32\n"
			"add x31, x31, x5\n"
			"lw x0, 0(x31)\n"
			"lw x10, 2(x31)\n"
			"lw x11, 1(x31)\n"
			"lw x12, 12(x31)\n"
			"lw x13, 10(x31)\n"
			"lw x17, 14(x31)\n"
			"lw x18, 8(x31)\n"
			"lw x19, 18(x31)\n"
			"lw x28, 30(x31)\n"
			"lw x7, 12(x31)\n"
			"lw x9, 36(x31)\n"
			"li x21, 0x1\n"
			"LBL_SEQID_3_LOOP_SEQ_HID_0:\n"
			"slti x17, x12, 0x3\n"
			"fsgnjx.ps f3, f3, f14\n"
			"bne x3, x18, 1f\n"
			"1:\n"
			"andi x10, x10, 0x13\n"
			"sraw x11, x18, x9\n"
			"mulh x28, x10, x18\n"
			"blt x4, x10, 1f\n"
			"1:\n"
			"andi x12, x13, 0x3\n"
			"fmadd.s f3, f3, f5, f3, rdn\n"
			"fround.ps f14, f14, rup\n"
			"fmsub.s f14, f9, f14, f14, rne\n"
			"and x7, x11, x7\n"
			"fmin.ps f16, f14, f28\n"
			"fnmsub.ps f23, f14, f28, f9, rup\n"
			"fltm.ps m6, f28, f5\n"
			"fmsub.ps f14, f5, f28, f19, rne\n"
			"fmax.s f5, f28, f28\n"
			"fmul.s f11, f14, f28, rne\n"
			"fmv.s.x f5, x17\n"
			"fmin.s f11, f11, f11\n"
			"div x13, x12, x10\n"
			"fmax.pi f9, f14, f28\n"
			"fle.pi f7, f5, f9\n"
			"fcvt.pwu.ps f9, f9, rup\n"
			"srlw x9, x28, x13\n"
			"fnmadd.ps f26, f28, f28, f28, rdn\n"
			"packb x9, x13, x19\n"
			"maskxor m6, m6, m6\n"
			"flt.s x0, f28, f9\n"
			"mulhu x9, x13, x9\n"
			"maskor m2, m2, m0\n"
			"sraw x11, x10, x17\n"
			"fnot.pi f23, f3\n"
			"blt x10, x12, 1f\n"
			"1:\n"
			"fmax.pi f14, f28, f3\n"
			"fnmsub.ps f11, f3, f19, f14, rmm\n"
			"bne x9, x17, 1f\n"
			"1:\n"
			"fsgnj.ps f14, f14, f3\n"
			"srl x13, x17, x19\n"
			"mulw x28, x13, x28\n"
			"fsub.pi f19, f19, f14\n"
			"divu x28, x19, x0\n"
			"xor x10, x9, x19\n"
			"maskor m0, m0, m2\n"
			"mov.m.x m0, x0, 0xff\n"
			"divw x18, x7, x7\n"
			"and x7, x7, x12\n"
			"fmulh.pi f9, f9, f5\n"
			"fnot.pi f18, f18\n"
			"fadd.s f26, f28, f18, rdn\n"
			"fmul.s f19, f19, f14, rup\n"
			"bgeu x21, x9, 1f\n"
			"1:\n"
			"sraw x9, x9, x10\n"
			"fclass.ps f20, f11\n"
			"bltu x8, x28, 1f\n"
			"1:\n"
			"mulhsu x28, x12, x28\n"
			"mova.x.m x9\n"
			"sltiu x18, x11, 0x13\n"
			"fmsub.s f14, f19, f11, f5, rmm\n"
			"fmv.x.s x12, f18\n"
			"maskand m2, m2, m0\n"
			"mov.m.x m0, x0, 0xff\n"
			"fsrli.pi f28, f11, 0x9\n"
			"lui x17, 0x1f\n"
			"flt.pi f16, f14, f14\n"
			"remuw x7, x7, x9\n"
			"ffrc.ps f11, f19\n"
			"fle.s x19, f9, f11\n"
			"fadd.pi f18, f9, f28\n"
			"fmadd.s f16, f14, f11, f28, rup\n"
			"fsrli.pi f23, f11, 0xb\n"
			"bge x7, x10, 1f\n"
			"1:\n"
			"fsetm.pi m3, f19\n"
			"fmadd.s f28, f19, f18, f28, rdn\n"
			"fcvt.w.s x12, f19, rdn\n"
			"beq x2, x14, 1f\n"
			"1:\n"
			"fsgnj.ps f7, f19, f14\n"
			"sllw x0, x9, x10\n"
			"bge x7, x16, 1f\n"
			"1:\n"
			"fsrl.pi f11, f18, f3\n"
			"fmsub.s f20, f18, f14, f3, rne\n"
			"flem.ps m6, f19, f11\n"
			"mov.m.x m3, x11, 0xc\n"
			"feqm.ps m0, f14, f14\n"
			"addiw x13, x18, 0x13\n"
			"fmsub.ps f14, f18, f14, f9, rup\n"
			"blt x5, x31, 1f\n"
			"1:\n"
			"fsgnj.ps f19, f14, f3\n"
			"fsgnjx.ps f11, f28, f3\n"
			"rem x17, x17, x9\n"
			"xor x12, x12, x18\n"
			"fswizz.ps f9, f9, 0x12\n"
			"fsatu8.pi f18, f5\n"
			"flog.ps f16, f3\n"
			"beq x6, x16, 1f\n"
			"1:\n"
			"mul x18, x9, x19\n"
			"fltu.pi f19, f5, f9\n"
			"fclass.s x12, f28\n"
			"bltu x9, x12, 1f\n"
			"1:\n"
			"xor x11, x0, x11\n"
			"bne x18, x24, 1f\n"
			"1:\n"
			"div x7, x7, x9\n"
			"fence\n"
			"remuw x19, x9, x19\n"
			"addi x19, x11, 0x18\n"
			"bltu x3, x13, 1f\n"
			"1:\n"
			"fsub.pi f28, f3, f11\n"
			"flt.ps f11, f28, f18\n"
			"divw x13, x13, x12\n"
			"sraw x13, x9, x19\n"
			"divuw x9, x10, x17\n"
			"fsrai.pi f3, f28, 0x1\n"
			"fexp.ps f26, f3\n"
			"bge x0, x10, 1f\n"
			"1:\n"
			"fcvt.ps.pwu f3, f19, rtz\n"
			"fcvt.pwu.ps f26, f28, rtz\n"
			"flem.ps m6, f28, f9\n"
			"fnmsub.s f3, f14, f9, f18, rne\n"
			"slt x0, x11, x18\n"
			"fcvt.ps.pw f14, f11, rup\n"
			"bgeu x25, x5, 1f\n"
			"1:\n"
			"fsll.pi f3, f3, f3\n"
			"fpackrepb.pi f11, f9\n"
			"sra x11, x18, x7\n"
			"blt x30, x19, 1f\n"
			"1:\n"
			"fnmadd.s f20, f5, f18, f14, rup\n"
			"fadd.pi f23, f11, f19\n"
			"fcvt.pwu.ps f11, f3, rup\n"
			"fexp.ps f23, f19\n"
			"beq x24, x10, 1f\n"
			"1:\n"
			"maskpopcz x13, m6\n"
			"blt x13, x8, 1f\n"
			"1:\n"
			"maskpopc x11, m3\n"
			"maskor m6, m2, m6\n"
			"slti x0, x12, 0x15\n"
			"beq x5, x31, 1f\n"
			"1:\n"
			"maskxor m2, m2, m6\n"
			"fclass.s x11, f19\n"
			"divuw x0, x0, x0\n"
			"fmaxu.pi f7, f3, f18\n"
			"srlw x17, x17, x11\n"
			"sltiu x10, x9, 0xe\n"
			"bgeu x17, x7, 1f\n"
			"1:\n"
			"fnmadd.ps f11, f11, f5, f11, rmm\n"
			"frcp.ps f18, f19\n"
			"slt x13, x13, x10\n"
			"subw x7, x10, x9\n"
			"fmin.ps f11, f14, f11\n"
			"add x13, x9, x10\n"
			"fand.pi f19, f9, f28\n"
			"fmaxu.pi f28, f14, f3\n"
			"bge x30, x7, 1f\n"
			"1:\n"
			"for.pi f20, f5, f14\n"
			"fsrl.pi f16, f9, f5\n"
			"fnot.pi f9, f9\n"
			"div x12, x19, x17\n"
			"mulw x18, x10, x18\n"
			"fbci.pi f9, 0x10634\n"
			"flem.ps m6, f3, f18\n"
			"mulhu x11, x11, x11\n"
			"bltu x19, x5, 1f\n"
			"1:\n"
			"fadd.ps f18, f28, f3, rmm\n"
			"mova.m.x x28\n"
			"fmadd.s f11, f28, f18, f14, rne\n"
			"fxor.pi f16, f18, f18\n"
			"mul x0, x0, x11\n"
			"fsra.pi f14, f19, f14\n"
			"mulw x17, x0, x10\n"
			"auipc x0, 0x1d\n"
			"fmin.ps f14, f5, f3\n"
			"srli x19, x17, 0\n"
			"mulh x17, x17, x19\n"
			"div x17, x10, x19\n"
			"srai x28, x28, 4\n"
			"div x0, x9, x28\n"
			"bne x28, x0, 1f\n"
			"1:\n"
			"fmin.pi f14, f18, f14\n"
			"srlw x10, x0, x12\n"
			"bne x2, x31, 1f\n"
			"1:\n"
			"frcp.ps f18, f19\n"
			"for.pi f20, f28, f14\n"
			"fcvt.ps.f16 f3, f11\n"
			"maskxor m6, m0, m3\n"
			"fmulhu.pi f23, f28, f18\n"
			"fnmadd.ps f28, f11, f28, f11, rtz\n"
			"fsub.pi f28, f28, f19\n"
			"fclass.ps f7, f18\n"
			"remu x28, x19, x10\n"
			"fmadd.ps f18, f14, f18, f28, rtz\n"
			"fsub.ps f5, f11, f3, rmm\n"
			"bgeu x3, x9, 1f\n"
			"1:\n"
			"addiw x17, x17, 0x16\n"
			"maskpopcz x19, m6\n"
			"fclass.s x19, f19\n"
			"fpackrepb.pi f26, f18\n"
			"bne x28, x6, 1f\n"
			"1:\n"
			"fsat8.pi f3, f28\n"
			"bge x24, x2, 1f\n"
			"1:\n"
			"fmv.s.x f28, x28\n"
			"addi x21, x21, -1\n"
			"bne x21, x0, LBL_SEQID_3_LOOP_SEQ_HID_0\n"
			"li x5, 4\n" 
			"bne x30, x5, LBL_FAIL_HID_0\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_4_FP_LOAD_ID_100_HID_0:\n"
			"li x17, 0x8102be00ac\n" 
			"fsw.ps f11, -615(x17)\n"
			"flw f13, -615(x17)\n" 
			"addw x7, x7, x18\n"
			"blt x10, x8, 1f\n"
			"1:\n"
			"LBL_SEQID_4_FP_AMO_ID_101_HID_0:\n"
			"li x18, 0x814b6ff11c\n" 
			"la x9, mem_ptr_offset\n"
			"flw.ps f20, 704(x9)\n"
			"famomaxl.pi f31, f20 (x18)\n" 
			"srl x13, x13, x13\n"
			"beq x28, x5, 1f\n"
			"1:\n"
			"LBL_SEQID_4_FP_SCATTER_ID_102_HID_0:\n"
			"li x9, 0x8203de0003\n" 
			"li x19, 0xf58f1b8e9e13897c\n"
			"fsc32h.ps f31, x19 (x9)\n" 
			"feq.pi f20, f31, f5\n"
			"LBL_SEQID_4_INT_AMO_ID_103_HID_0:\n"
			"li x7, 0x82312a002c\n" 
			"amoaddg.w x10, x10, (x7)\n" 
			"fmax.s f26, f20, f26\n"
			"LBL_SEQID_4_INT_AMO_ID_104_HID_0:\n"
			"li x11, 0x81240a020c\n" 
			"amomaxug.w x18, x28, (x11)\n" 
			"fandi.pi f23, f23, 0xd6\n"
			"LBL_SEQID_4_CSR_ID_105_HID_0:\n"
			"li x12, 0x81038e0002\n"
			"li x31, (0x2 << 6) | 0x1\n"
			"csrw lock_va, x12\n" 
			"csrwi tensor_wait, 6\n" 
			"fmin.pi f7, f28, f23\n"
			"bgeu x21, x20, 1f\n"
			"1:\n"
			"LBL_SEQID_4_FP_GATHER_ID_106_HID_0:\n"
			"li x12, 0x8203861f60\n" 
			"la x17, mem_ptr_offset\n"
			"flw.ps f31, 0(x17)\n"
			"fscw.ps f8, f31 (x12)\n"
			"fgh.ps f23, f31 (x12)\n" 
			"fsll.pi f31, f7, f28\n"
			"LBL_SEQID_4_FP_G_GATHER_ID_107_HID_0:\n"
			"li x7, 0x813896e010\n" 
			"la x13, mem_ptr_offset\n"
			"flw.ps f20, 212(x13)\n"
			"fscwg.ps f28, f20 (x7)\n"
			"fgbg.ps f16, f20 (x7)\n" 
			"li x5, 0x6d99\n"
			"csrw tensor_mask, x5\n"	
			"fsgnjx.ps f11, f31, f0\n"
			"LBL_SEQID_4_FP_AMO_ID_108_HID_0:\n"
			"li x13, 0x814fea0774\n" 
			"la x10, mem_ptr_offset\n"
			"flw.ps f11, 544(x10)\n"
			"famomaxul.pi f26, f11 (x13)\n" 
			"fmax.pi f18, f16, f20\n"
			"LBL_SEQID_4_FP_AMO_ID_109_HID_0:\n"
			"li x9, 0x82512a002c\n" 
			"la x11, mem_ptr_offset\n"
			"flw.ps f28, 864(x11)\n"
			"famoxorl.pi f14, f28 (x9)\n" 
			"fcvt.f16.ps f31, f9\n"
			"LBL_SEQID_4_INT_AMO_ID_110_HID_0:\n"
			"li x19, 0x81317a1020\n" 
			"amoxorg.w x11, x9, (x19)\n" 
			"packb x11, x11, x11\n"
			"LBL_SEQID_4_INT_AMO_ID_111_HID_0:\n"
			"li x10, 0x8153ad2f48\n" 
			"amominul.d x28, x19, (x10)\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"fsgnjn.s f11, f23, f3\n"
			"LBL_SEQID_4_INT_AMO_ID_112_HID_0:\n"
			"li x19, 0x81440a0208\n" 
			"amominul.d x18, x28, (x19)\n" 
			"sltu x19, x28, x17\n"
			"bltu x31, x4, 1f\n"
			"1:\n"
			"LBL_SEQID_4_INT_G_STORE_ID_113_HID_0:\n"
			"li x13, 0x8125d4a088\n" 
			"shg x11, (x13)\n" 
			"fslli.pi f5, f20, 0x2\n"
			"LBL_SEQID_4_FP_G_GATHER_ID_114_HID_0:\n"
			"li x19, 0x822f1c0030\n" 
			"la x11, mem_ptr_offset\n"
			"flq2 f23, 104(x11)\n"
			"fscwg.ps f15, f23 (x19)\n"
			"fgbg.ps f19, f23 (x19)\n" 
			"maskor m2, m3, m2\n"
			"LBL_SEQID_4_FP_L_GATHER_ID_115_HID_0:\n"
			"li x7, 0x814b6ff118\n" 
			"la x10, mem_ptr_offset\n"
			"flq2 f9, 24(x10)\n"
			"fscwl.ps f17, f9 (x7)\n"
			"fgbl.ps f9, f9 (x7)\n" 
			"fxor.pi f31, f20, f20\n"
			"LBL_SEQID_4_INT_LOAD_ID_116_HID_0:\n"
			"li x7, 0x8207d0074d\n" 
			"sd x12, 1109(x7)\n"
			"lhu x18, 1109(x7)\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"fmulhu.pi f13, f14, f28\n"
			"LBL_SEQID_4_INT_AMO_ID_117_HID_0:\n"
			"li x13, 0x8233861f60\n" 
			"amoorg.d x18, x19, (x13)\n" 
			"fnmadd.s f5, f31, f5, f19, rtz\n"
			"LBL_SEQID_4_FP_AMO_ID_118_HID_0:\n"
			"li x7, 0x8125d4a088\n" 
			"la x11, mem_ptr_offset\n"
			"flw.ps f13, 704(x11)\n"
			"famomaxug.pi f9, f13 (x7)\n" 
			"fslli.pi f23, f11, 0xd\n"
			"LBL_SEQID_4_FP_AMO_ID_119_HID_0:\n"
			"li x12, 0x814035a03c\n" 
			"la x7, mem_ptr_offset\n"
			"flw.ps f11, 448(x7)\n"
			"famomaxul.pi f13, f11 (x12)\n" 
			"sra x7, x11, x7\n"
			"LBL_SEQID_4_FP_LOAD_ID_120_HID_0:\n"
			"li x10, 0x8103ad2f4d\n" 
			"fsw.ps f0, 1552(x10)\n"
			"fbc.ps f28, 1552(x10)\n" 
			"fcvt.ps.pw f20, f20, rmm\n"
			"LBL_SEQID_4_FP_AMO_ID_121_HID_0:\n"
			"li x19, 0x825f1c0030\n" 
			"la x10, mem_ptr_offset\n"
			"flw.ps f18, 960(x10)\n"
			"famomaxl.ps f19, f18 (x19)\n" 
			"fmaxu.pi f3, f26, f23\n"
			"LBL_SEQID_4_FP_AMO_ID_122_HID_0:\n"
			"li x28, 0x8247d0074c\n" 
			"la x19, mem_ptr_offset\n"
			"flw.ps f26, 288(x19)\n"
			"famoswapl.pi f13, f26 (x28)\n" 
			"fadd.ps f5, f0, f11, rmm\n"
			"LBL_SEQID_4_INT_AMO_ID_123_HID_0:\n"
			"li x10, 0x8249640028\n" 
			"amoandl.d x13, x28, (x10)\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"for.pi f3, f20, f3\n"
			"bgeu x31, x5, 1f\n"
			"1:\n"
			"LBL_SEQID_4_INT_AMO_ID_124_HID_0:\n"
			"li x17, 0x815bfa0408\n" 
			"amomaxul.d x9, x18, (x17)\n" 
			"fnmsub.ps f5, f11, f5, f28, rup\n"
			"LBL_SEQID_4_INT_AMO_ID_125_HID_0:\n"
			"li x10, 0x812c940124\n" 
			"amoorg.w x28, x11, (x10)\n" 
			"fmin.ps f28, f14, f14\n"
			"bne x25, x14, 1f\n"
			"1:\n"
			"LBL_SEQID_4_FP_AMO_ID_126_HID_0:\n"
			"li x28, 0x8233860a2c\n" 
			"la x19, mem_ptr_offset\n"
			"flw.ps f5, 832(x19)\n"
			"famoming.ps f0, f5 (x28)\n" 
			"fminu.pi f7, f7, f23\n"
			"blt x28, x3, 1f\n"
			"1:\n"
			"LBL_SEQID_4_INT_G_STORE_ID_127_HID_0:\n"
			"li x19, 0x81317a1020\n" 
			"sbg x12, (x19)\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"mul x7, x9, x7\n"
			"LBL_SEQID_4_FP_L_SCATTER_ID_128_HID_0:\n"
			"li x9, 0x825a78e020\n" 
			"la x11, mem_ptr_offset\n"
			"flq2 f14, 120(x11)\n"
			"fschl.ps f23, f14 (x9)\n" 
			"fsub.ps f0, f20, f20, rtz\n"
			"LBL_SEQID_4_INT_AMO_ID_129_HID_0:\n"
			"li x28, 0x81256400b4\n" 
			"amoaddg.w x18, x18, (x28)\n" 
			"auipc x12, 0x18\n"
			"LBL_SEQID_4_FP_GATHER_ID_130_HID_0:\n"
			"li x19, 0x8102be00ac\n" 
			"li x9, 0xbad909470ef25789\n"
			"fsc32w.ps f22, x9 (x19)\n"
			"fg32w.ps f3, x9 (x19)\n" 
			"fexp.ps f23, f23\n"
			"blt x19, x0, 1f\n"
			"1:\n"
			"LBL_SEQID_4_INT_AMO_ID_131_HID_0:\n"
			"li x18, 0x82332c003c\n" 
			"amomaxg.w x9, x9, (x18)\n" 
			"fnmadd.ps f5, f13, f5, f11, rne\n"
			"csrwi tensor_wait, 6\n"
			"fence\n"
			"li x5, 0x81038e0002\n" 
			"li x31, (0x2 << 6) | 0x1\n"
			"csrw unlock_va, x5\n"
			"csrwi tensor_wait, 6\n"
			"li x31, 0b11 << 58\n"
			"add x5, x31, x5\n"
			"li x31, (0x2 << 6) | 0x1\n"
			"csrw flush_va, x5\n"
			"csrwi tensor_wait, 6\n"
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_4_XMA_QNT_STR_SEQ_ITER_0_HID_0:\n"
			"li x5, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x3 << TENSOR_IMA_B_NUM_COLS) | (0xa << TENSOR_IMA_A_NUM_ROWS) | (0x3 << TENSOR_IMA_A_NUM_COLS) | (0xb << TENSOR_IMA_OFFSET) | (0x0 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x5a << TENSOR_IMA_SCP_LOC_B) | (0x69 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x0 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x6, ((0x1e << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0xa << TENSOR_QUANT_QUANT_ROW) | (0x1c << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0xa << TENSOR_QUANT_TRANSF8) | (0x3 << TENSOR_QUANT_TRANSF7) | (0xa << TENSOR_QUANT_TRANSF6) | (0x8 << TENSOR_QUANT_TRANSF5) | (0x4 << TENSOR_QUANT_TRANSF4) | (0x3 << TENSOR_QUANT_TRANSF3) | (0x4 << TENSOR_QUANT_TRANSF2) | (0xa << TENSOR_QUANT_TRANSF1) | (0x4 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_4_XMA_QNT_STR_SEQ_ITER_1_HID_0:\n"
			"li x5, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x0 << TENSOR_FMA32_B_NUM_COLS) | (0xd << TENSOR_FMA32_A_NUM_ROWS) | (0x9 << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x92 << TENSOR_FMA32_SCP_LOC_B) | (0x46 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x6, ((0xa << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0xd << TENSOR_QUANT_QUANT_ROW) | (0x9 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x0 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_4_XMA_QNT_STR_SEQ_ITER_2_HID_0:\n"
			"li x6, ((0x14 << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0xb << TENSOR_QUANT_QUANT_ROW) | (0x34 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x2 << TENSOR_QUANT_TRANSF5) | (0x6 << TENSOR_QUANT_TRANSF4) | (0x7 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_4_XMA_QNT_STR_SEQ_ITER_3_HID_0:\n"
			"li x5, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x1 << TENSOR_IMA_B_NUM_COLS) | (0xd << TENSOR_IMA_A_NUM_ROWS) | (0x5 << TENSOR_IMA_A_NUM_COLS) | (0x1 << TENSOR_IMA_OFFSET) | (0x0 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x7d << TENSOR_IMA_SCP_LOC_B) | (0xa8 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x7, ((0x3 << TENSOR_STORE_SCP_ENTRY_STRIDE) | (0x6 << TENSOR_STORE_SCP_START_SCP_ENTRY) | (0x6 << TENSOR_STORE_SCP_AROWS) | (0x1 << TENSOR_STORE_SCP_OPC_1) | (0x20001ad << TENSOR_STORE_SCP_VADDR) | 0 )\n" 
			"li x31, (0x1 << 6)\n"
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
			"csrwi tensor_wait, 10\n"
			"csrw tensor_store, x7\n"	
			"csrwi tensor_wait, 8\n"
			"LBL_SEQID_4_XMA_QNT_STR_SEQ_ITER_4_HID_0:\n"
			"li x6, ((0xb << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0xf << TENSOR_QUANT_QUANT_ROW) | (0x35 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x7 << TENSOR_QUANT_TRANSF4) | (0x6 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_0\n"
			"csrw tensor_error, x0\n"
			"mov.m.x m0, x0, 0xFF\n"
			"li x5, 0xa0\n"
			"la x31, rand_ieee754_32\n"
			"add x31, x31, x5\n"
			"flw.ps f11, 0(x31)\n"
			"flw.ps f13, 4(x31)\n"
			"flw.ps f16, 2(x31)\n"
			"flw.ps f18, 8(x31)\n"
			"flw.ps f19, 13(x31)\n"
			"flw.ps f3, 5(x31)\n"
			"flw.ps f31, 9(x31)\n"
			"flw.ps f5, 6(x31)\n"
			"flw.ps f9, 15(x31)\n"
			"la x31, rand_int_32\n"
			"add x31, x31, x5\n"
			"lw x0, 0(x31)\n"
			"lw x10, 0(x31)\n"
			"lw x11, 4(x31)\n"
			"lw x12, 7(x31)\n"
			"lw x13, 11(x31)\n"
			"lw x17, 18(x31)\n"
			"lw x18, 16(x31)\n"
			"lw x19, 27(x31)\n"
			"lw x28, 26(x31)\n"
			"lw x7, 29(x31)\n"
			"lw x9, 38(x31)\n"
			"li x21, 0x3\n"
			"LBL_SEQID_4_LOOP_SEQ_HID_0:\n"
			"fmin.pi f9, f9, f9\n"
			"fmadd.ps f3, f9, f11, f3, rmm\n"
			"fcvt.wu.s x0, f3, rtz\n"
			"srliw x28, x18, 0\n"
			"subw x7, x17, x19\n"
			"mova.x.m x10\n"
			"packb x18, x18, x17\n"
			"fmadd.ps f31, f31, f19, f9, rup\n"
			"fadd.s f16, f18, f9, rtz\n"
			"frsq.ps f19, f19\n"
			"fandi.pi f18, f18, 0x12e\n"
			"fmvs.x.ps x12, f19, 4\n"
			"fpackreph.pi f0, f11\n"
			"fcvt.wu.s x10, f19, rne\n"
			"fclass.ps f3, f11\n"
			"fpackrepb.pi f11, f16\n"
			"srai x11, x11, 2\n"
			"blt x7, x25, 1f\n"
			"1:\n"
			"addw x10, x17, x28\n"
			"blt x11, x7, 1f\n"
			"1:\n"
			"flem.ps m3, f13, f16\n"
			"fltm.ps m0, f18, f13\n"
			"mov.m.x m0, x0, 0xff\n"
			"fsatu8.pi f18, f3\n"
			"sraw x19, x18, x19\n"
			"bltu x26, x29, 1f\n"
			"1:\n"
			"fmvs.x.ps x10, f16, 5\n"
			"fnmadd.ps f13, f18, f16, f13, rtz\n"
			"addw x9, x12, x9\n"
			"bne x24, x28, 1f\n"
			"1:\n"
			"fcvt.ps.pwu f5, f5, rup\n"
			"bltu x13, x22, 1f\n"
			"1:\n"
			"fcmovm.ps f5, f11, f13\n"
			"fcvt.s.wu f11, x28, rmm\n"
			"fltm.ps m6, f19, f3\n"
			"fclass.ps f18, f18\n"
			"divuw x0, x11, x9\n"
			"fmv.x.s x12, f3\n"
			"fsatu8.pi f11, f13\n"
			"sraw x0, x11, x0\n"
			"frsq.ps f13, f13\n"
			"fsgnjx.s f5, f9, f3\n"
			"for.pi f13, f13, f3\n"
			"bge x4, x4, 1f\n"
			"1:\n"
			"mulhu x0, x18, x10\n"
			"fsrai.pi f18, f9, 0x6\n"
			"fpackrepb.pi f19, f19\n"
			"fmadd.ps f13, f5, f3, f18, rtz\n"
			"feq.pi f18, f13, f9\n"
			"fsatu8.pi f7, f5\n"
			"fmin.s f16, f18, f5\n"
			"fsgnjn.ps f31, f31, f9\n"
			"sra x19, x19, x11\n"
			"fbcx.ps f18, x11\n"
			"frcp.ps f3, f31\n"
			"fand.pi f19, f19, f5\n"
			"fcvt.ps.f16 f14, f13\n"
			"blt x30, x7, 1f\n"
			"1:\n"
			"fround.ps f5, f16, rne\n"
			"bltu x6, x7, 1f\n"
			"1:\n"
			"fadd.s f7, f9, f16, rup\n"
			"faddi.pi f9, f11, 0x82\n"
			"bgeu x8, x0, 1f\n"
			"1:\n"
			"frcp.ps f5, f11\n"
			"fsrli.pi f3, f3, 0x4\n"
			"fnmadd.ps f16, f16, f16, f3, rtz\n"
			"mul x13, x19, x7\n"
			"fmin.ps f16, f16, f9\n"
			"bne x19, x24, 1f\n"
			"1:\n"
			"mulh x28, x11, x28\n"
			"bge x13, x4, 1f\n"
			"1:\n"
			"fnmsub.s f5, f5, f16, f16, rtz\n"
			"bne x3, x8, 1f\n"
			"1:\n"
			"sub x12, x19, x12\n"
			"fcmovm.ps f18, f31, f13\n"
			"fcvt.s.wu f11, x10, rmm\n"
			"fmulhu.pi f16, f18, f3\n"
			"fence\n"
			"fmv.x.s x0, f19\n"
			"bltu x15, x0, 1f\n"
			"1:\n"
			"fsgnjn.ps f5, f5, f13\n"
			"beq x22, x15, 1f\n"
			"1:\n"
			"packb x7, x11, x28\n"
			"srai x28, x28, 5\n"
			"divu x10, x19, x9\n"
			"mov.m.x m6, x11, 0x54\n"
			"fle.ps f18, f9, f13\n"
			"fmin.pi f3, f5, f16\n"
			"fround.ps f5, f11, rmm\n"
			"fsatu8.pi f16, f16\n"
			"xor x17, x10, x0\n"
			"fsgnjx.ps f19, f16, f13\n"
			"remw x19, x19, x18\n"
			"xori x17, x12, 0x14\n"
			"fcvt.ps.f16 f9, f9\n"
			"fsat8.pi f7, f31\n"
			"beq x17, x4, 1f\n"
			"1:\n"
			"fmulhu.pi f0, f9, f31\n"
			"frsq.ps f16, f19\n"
			"fslli.pi f31, f31, 0x3\n"
			"fmvs.x.ps x28, f16, 7\n"
			"ffrc.ps f19, f16\n"
			"sll x19, x10, x9\n"
			"sraiw x7, x7, 5\n"
			"fcvt.ps.pw f9, f13, rdn\n"
			"bltu x6, x20, 1f\n"
			"1:\n"
			"fpackreph.pi f18, f3\n"
			"fence\n"
			"feqm.ps m6, f13, f5\n"
			"fsgnjx.ps f16, f16, f13\n"
			"bltu x10, x18, 1f\n"
			"1:\n"
			"mulw x18, x17, x11\n"
			"fadd.ps f13, f5, f13, rmm\n"
			"fadd.ps f9, f9, f9, rne\n"
			"fence\n"
			"bltu x15, x20, 1f\n"
			"1:\n"
			"sltu x17, x7, x17\n"
			"mov.m.x m0, x18, 0x68\n"
			"remuw x13, x13, x17\n"
			"fsub.s f3, f19, f5, rtz\n"
			"auipc x10, 0x12\n"
			"bgeu x17, x20, 1f\n"
			"1:\n"
			"fmulhu.pi f19, f19, f18\n"
			"fence\n"
			"sub x19, x19, x0\n"
			"blt x30, x22, 1f\n"
			"1:\n"
			"fsra.pi f13, f13, f11\n"
			"beq x30, x29, 1f\n"
			"1:\n"
			"fsgnjn.s f19, f3, f19\n"
			"ecall\n"
			"fbci.ps f7, 0x445b4\n"
			"fbci.pi f11, 0x2bf3a\n"
			"fsra.pi f3, f3, f11\n"
			"fsat8.pi f31, f3\n"
			"fcvt.ps.f16 f26, f11\n"
			"fpackreph.pi f26, f13\n"
			"fmul.pi f5, f5, f11\n"
			"fbcx.ps f11, x7\n"
			"fmvs.x.ps x13, f13, 6\n"
			"fmin.ps f3, f9, f31\n"
			"remuw x18, x7, x17\n"
			"fmadd.ps f14, f13, f16, f31, rdn\n"
			"fsll.pi f19, f18, f19\n"
			"fsub.pi f7, f16, f13\n"
			"fadd.ps f9, f3, f9, rmm\n"
			"remw x13, x28, x13\n"
			"fsub.pi f13, f18, f16\n"
			"fsin.ps f9, f3\n"
			"fsub.pi f11, f3, f5\n"
			"packb x19, x11, x17\n"
			"divuw x17, x12, x17\n"
			"fpackrepb.pi f18, f5\n"
			"remw x10, x19, x18\n"
			"and x10, x13, x11\n"
			"fnmadd.ps f16, f11, f16, f9, rmm\n"
			"fpackreph.pi f14, f9\n"
			"fclass.ps f26, f19\n"
			"fsub.pi f13, f31, f19\n"
			"fmax.s f5, f16, f5\n"
			"fmvs.x.ps x7, f11, 0\n"
			"bne x30, x14, 1f\n"
			"1:\n"
			"mova.m.x x11\n"
			"blt x15, x17, 1f\n"
			"1:\n"
			"addi x21, x21, -1\n"
			"bne x21, x0, LBL_SEQID_4_LOOP_SEQ_HID_0\n"
			"li x5, 5\n" 
			"bne x30, x5, LBL_FAIL_HID_0\n"
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f6,  448 (x5)\n"
			"flw.ps f4,  896 (x5)\n"
			"flw.ps f19,  192 (x5)\n"
			"flw.ps f9,  864 (x5)\n"
			"flw.ps f15,  96 (x5)\n"
			"flw.ps f1,  288 (x5)\n"
			"flw.ps f18,  576 (x5)\n"
			"flw.ps f22,  640 (x5)\n"
			"flw.ps f21,  672 (x5)\n"
			"flw.ps f7,  480 (x5)\n"
			"flw.ps f28,  256 (x5)\n"
			"flw.ps f0,  160 (x5)\n"
			"flw.ps f25,  800 (x5)\n"
			"flw.ps f8,  64 (x5)\n"
			"flw.ps f27,  352 (x5)\n"
			"flw.ps f20,  832 (x5)\n"
			"flw.ps f2,  512 (x5)\n"
			"flw.ps f5,  608 (x5)\n"
			"flw.ps f23,  0 (x5)\n"
			"flw.ps f24,  544 (x5)\n"
			"flw.ps f14,  384 (x5)\n"
			"flw.ps f10,  128 (x5)\n"
			"flw.ps f26,  992 (x5)\n"
			"flw.ps f30,  704 (x5)\n"
			"flw.ps f17,  928 (x5)\n"
			"flw.ps f29,  32 (x5)\n"
			"flw.ps f31,  736 (x5)\n"
			"flw.ps f12,  960 (x5)\n"
			"flw.ps f13,  768 (x5)\n"
			"flw.ps f3,  320 (x5)\n"
			"flw.ps f16,  224 (x5)\n"
			"flw.ps f11,  416 (x5)\n"
			"addi x30, x30, 1\n"
			"la x17, rand_int_32\n"
			"flw.ps f0, 288(x17)\n"
			"fnot.pi f0, f0\n" 
			"fmax.ps f5, f20, f0\n"
			"sllw x28, x0, x9\n"
			"fsra.pi f19, f11, f7\n"
			"beq x30, x23, 1f\n"
			"1:\n"
			"fmsub.ps f14, f16, f31, f5, rtz\n"
			"fnmadd.ps f9, f19, f5, f16, rtz\n"
			"fmax.pi f13, f13, f18\n"
			"feq.s x17, f3, f0\n"
			"maskxor m0, m0, m2\n"
			"mov.m.x m0, x0, 0xff\n"
			"fnmsub.ps f28, f18, f31, f5, rmm\n"
			"fmaxu.pi f19, f28, f3\n"
			"mova.x.m x18\n" 
			"fbci.ps f11, 0x30c7e\n"
			"ffrc.ps f20, f0\n"
			"fsub.pi f23, f14, f14\n"
			"divw x7, x7, x11\n"
			"fclass.s x17, f7\n"
			"fltm.pi m6, f20, f3\n"
			"fmulhu.pi f14, f14, f14\n"
			"slliw x19, x28, 2\n"
			"fbci.pi f3, 0x585bc\n"
			"maskand m2, m2, m2\n"
			"la x18, rand_ieee754_32\n"
			"flw.ps f23, 96(x18)\n"
			"flw.ps f31, 544(x18)\n"
			"feq.s x10, f23, f31\n" 
			"frcp.ps f28, f28\n"
			"fmul.s f16, f20, f20, rmm\n"
			"fmin.pi f26, f18, f26\n"
			"xor x9, x28, x10\n"
			"fcmovm.ps f26, f5, f0\n"
			"fle.ps f16, f31, f5\n"
			"sltu x12, x7, x7\n"
			"ffrc.ps f28, f26\n"
			"fmul.pi f14, f18, f16\n"
			"bne x4, x27, 1f\n"
			"1:\n"
			"fsub.s f0, f9, f0, rdn\n"
			"la x18, rand_ieee754_32\n"
			"flw.ps f19, 352(x18)\n"
			"flw.ps f5, 64(x18)\n"
			"fsgnj.ps f14, f19, f5\n" 
			"fmin.pi f18, f11, f28\n"
			"sll x12, x13, x18\n"
			"fsub.s f3, f5, f13, rne\n"
			"fsub.s f0, f7, f0, rne\n"
			"fcmovm.ps f19, f0, f18\n"
			"fcvt.ps.pwu f16, f3, rtz\n"
			"bge x28, x1, 1f\n"
			"1:\n"
			"fbci.pi f9, 0x41af3\n"
			"fclass.s x19, f14\n"
			"bne x9, x14, 1f\n"
			"1:\n"
			"fmul.ps f5, f5, f16, rtz\n"
			"bne x27, x0, 1f\n"
			"1:\n"
			"fmsub.ps f13, f28, f13, f18, rdn\n"
			"LBL_SEQID_5_M0_WRITE_ID_132_HID_0:\n"
			"fsetm.pi m3, f19\n" 
			"li x5, 0x803400D8\n"
			"ld x5, 0(x5)\n"
			"fsub.ps f19, f3, f23, rne\n"
			"remw x10, x10, x13\n"
			"divu x17, x17, x11\n"
			"bne x3, x19, 1f\n"
			"1:\n"
			"fadd.s f31, f31, f23, rdn\n"
			"add x18, x9, x18\n"
			"fmin.pi f9, f31, f11\n"
			"fmul.ps f0, f31, f11, rdn\n"
			"addi x13, x0, 0xc\n"
			"fround.ps f20, f3, rup\n"
			"fnot.pi f11, f28\n"
			"la x9, rand_int_32\n"
			"flw.ps f23, 224(x9)\n"
			"flw.ps f28, 320(x9)\n"
			"fmulhu.pi f20, f23, f28\n" 
			"fand.pi f7, f28, f3\n"
			"blt x23, x24, 1f\n"
			"1:\n"
			"fsgnjx.ps f7, f19, f14\n"
			"and x13, x12, x12\n"
			"fmul.pi f23, f0, f19\n"
			"fmin.ps f3, f14, f7\n"
			"mulw x10, x18, x17\n"
			"sraiw x0, x18, 0\n"
			"bltu x23, x19, 1f\n"
			"1:\n"
			"fle.ps f3, f7, f28\n"
			"fandi.pi f7, f7, 0x49\n"
			"fsub.pi f7, f20, f0\n"
			"la x10, rand_int_32\n"
			"flw.ps f5, 896(x10)\n"
			"flw.ps f16, 480(x10)\n"
			"fsub.pi f26, f5, f16\n" 
			"fcmovm.ps f23, f7, f28\n"
			"fmulh.pi f13, f3, f0\n"
			"fand.pi f28, f14, f5\n"
			"fxor.pi f23, f13, f13\n"
			"faddi.pi f26, f5, 0xed\n"
			"fsat8.pi f11, f3\n"
			"bgeu x8, x11, 1f\n"
			"1:\n"
			"fmin.ps f18, f0, f18\n"
			"frsq.ps f28, f28\n"
			"fsub.ps f20, f7, f13, rne\n"
			"fexp.ps f11, f18\n"
			"blt x28, x21, 1f\n"
			"1:\n"
			"la x19, rand_ieee754_32\n"
			"flw.ps f16, 576(x19)\n"
			"fclass.s x11, f16\n" 
			"fxor.pi f11, f31, f16\n"
			"fbci.ps f18, 0x43d4d\n"
			"flt.ps f19, f5, f19\n"
			"fsgnjn.ps f18, f18, f7\n"
			"blt x11, x29, 1f\n"
			"1:\n"
			"fsll.pi f23, f20, f5\n"
			"lui x12, 0x6\n"
			"fltm.pi m0, f20, f18\n"
			"fsgnjn.ps f19, f19, f11\n"
			"bge x22, x18, 1f\n"
			"1:\n"
			"remuw x7, x7, x10\n"
			"fadd.ps f19, f5, f20, rmm\n"
			"bne x15, x3, 1f\n"
			"1:\n"
			"li x11, 0x97a5b4ea9684b832\n"
			"sraiw x12, x11, 5\n" 
			"addiw x17, x28, 0x1f\n"
			"mov.m.x m2, x9, 0xc\n"
			"for.pi f13, f14, f14\n"
			"fmul.s f20, f16, f16, rup\n"
			"fltm.ps m2, f26, f16\n"
			"fmsub.ps f13, f3, f3, f3, rmm\n"
			"flog.ps f16, f26\n"
			"fxor.pi f14, f26, f14\n"
			"fcvt.f16.ps f7, f16\n"
			"fsgnjx.ps f19, f26, f3\n"
			"li x28, 0x5b20132dd08ceb80\n"
			"li x12, 0xe6b5d095438405f9\n"
			"mulw x17, x28, x12\n" 
			"addiw x12, x11, 0x1d\n"
			"srli x19, x0, 2\n"
			"fmaxu.pi f23, f14, f18\n"
			"bne x27, x26, 1f\n"
			"1:\n"
			"fence\n"
			"maskor m6, m6, m2\n"
			"fmadd.s f11, f19, f7, f14, rdn\n"
			"sraw x17, x7, x0\n"
			"fmulhu.pi f18, f16, f5\n"
			"rem x12, x12, x10\n"
			"fand.pi f19, f19, f31\n"
			"la x9, rand_int_32\n"
			"flw.ps f3, 960(x9)\n"
			"flw.ps f9, 448(x9)\n"
			"fxor.pi f18, f3, f9\n" 
			"feq.pi f11, f5, f20\n"
			"fnot.pi f11, f0\n"
			"frsq.ps f14, f14\n"
			"fbcx.ps f20, x10\n"
			"bgeu x13, x23, 1f\n"
			"1:\n"
			"fmax.ps f19, f20, f13\n"
			"fpackreph.pi f9, f9\n"
			"fmul.ps f9, f3, f5, rdn\n"
			"sltiu x17, x7, 0x10\n"
			"bne x24, x31, 1f\n"
			"1:\n"
			"fnmadd.ps f9, f7, f18, f14, rtz\n"
			"fsub.pi f11, f16, f9\n"
			"li x17, 0x777c6cb76fe1e2c7\n"
			"li x7, 0xf85c5868def0f57a\n"
			"add x7, x17, x7\n" 
			"sllw x7, x9, x7\n"
			"bne x20, x24, 1f\n"
			"1:\n"
			"fmul.pi f5, f16, f26\n"
			"bgeu x20, x5, 1f\n"
			"1:\n"
			"fsgnjx.ps f28, f26, f28\n"
			"fandi.pi f26, f16, 0x19b\n"
			"for.pi f23, f26, f28\n"
			"feq.pi f26, f26, f16\n"
			"for.pi f5, f26, f26\n"
			"bne x15, x20, 1f\n"
			"1:\n"
			"fandi.pi f26, f28, 0xe3\n"
			"slliw x18, x12, 5\n"
			"fmulhu.pi f5, f16, f16\n"
			"la x19, rand_ieee754_32\n"
			"flw.ps f16, 672(x19)\n"
			"frsq.ps f3, f16\n" 
			"ffrc.ps f3, f14\n"
			"fnmsub.ps f11, f11, f14, f14, rmm\n"
			"fcvt.pw.ps f20, f11, rtz\n"
			"beq x16, x2, 1f\n"
			"1:\n"
			"fsat8.pi f18, f11\n"
			"bne x31, x7, 1f\n"
			"1:\n"
			"fnmadd.ps f3, f7, f11, f7, rne\n"
			"fsll.pi f14, f14, f11\n"
			"fsub.ps f20, f11, f14, rtz\n"
			"mulh x17, x17, x17\n"
			"fle.ps f7, f7, f7\n"
			"fsll.pi f11, f11, f7\n"
			"bge x28, x13, 1f\n"
			"1:\n"
			"la x7, rand_int_32\n"
			"flw.ps f7, 96(x7)\n"
			"flw.ps f11, 256(x7)\n"
			"feq.pi f9, f7, f11\n" 
			"fand.pi f20, f16, f20\n"
			"fsrai.pi f0, f28, 0x0\n"
			"flt.ps f7, f5, f9\n"
			"fmul.ps f31, f16, f5, rdn\n"
			"fmax.s f7, f7, f0\n"
			"fle.ps f14, f9, f9\n"
			"fcmov.ps f9, f9, f9, f14\n"
			"fsub.s f28, f5, f26, rne\n"
			"fsgnjn.s f23, f7, f20\n"
			"fmin.ps f28, f14, f0\n"
			"la x19, rand_ieee754_32\n"
			"flw.ps f7, 0(x19)\n"
			"flw.ps f18, 192(x19)\n"
			"fsgnjn.s f14, f7, f18\n" 
			"divu x9, x0, x13\n"
			"fle.pi f31, f3, f13\n"
			"blt x26, x28, 1f\n"
			"1:\n"
			"sraw x13, x7, x17\n"
			"bgeu x20, x24, 1f\n"
			"1:\n"
			"fsub.pi f11, f18, f11\n"
			"sraw x9, x11, x13\n"
			"divw x28, x13, x28\n"
			"mulhu x11, x28, x10\n"
			"fcvt.pwu.ps f20, f20, rmm\n"
			"fsgnjx.s f7, f28, f19\n"
			"fsrai.pi f20, f3, 0x4\n"
			"bgeu x3, x4, 1f\n"
			"1:\n"
			"la x9, rand_ieee754_32\n"
			"flw.ps f28, 320(x9)\n"
			"flw.ps f3, 960(x9)\n"
			"feq.ps f11, f28, f3\n" 
			"fmulhu.pi f23, f18, f16\n"
			"fmadd.s f16, f26, f16, f18, rne\n"
			"fcmov.ps f19, f18, f16, f26\n"
			"fmax.s f23, f26, f18\n"
			"fmadd.ps f23, f16, f23, f18, rdn\n"
			"fmul.s f26, f23, f26, rup\n"
			"fsatu8.pi f19, f16\n"
			"fsgnjx.ps f7, f18, f26\n"
			"fcvt.ps.pwu f23, f18, rmm\n"
			"bgeu x4, x22, 1f\n"
			"1:\n"
			"fmin.pi f7, f18, f16\n"
			"LBL_SEQID_5_FP_TRANS_ID_133_HID_0:\n"
			"la x17, rand_ieee754_32\n"
			"flw.ps f13, 192(x17)\n"
			"flog.ps f9, f13\n" 
			"fpackreph.pi f0, f26\n"
			"fnmadd.ps f7, f7, f7, f7, rtz\n"
			"fcvt.f16.ps f19, f18\n"
			"fnmsub.ps f26, f26, f9, f7, rdn\n"
			"fnmsub.ps f18, f7, f9, f18, rtz\n"
			"fsub.ps f9, f18, f9, rmm\n"
			"fcvt.f16.ps f7, f9\n"
			"fmax.pi f26, f26, f26\n"
			"bltu x3, x20, 1f\n"
			"1:\n"
			"fsat8.pi f20, f26\n"
			"frcp.ps f23, f26\n"
			"la x13, rand_int_32\n"
			"flw.ps f11, 832(x13)\n"
			"fsrli.pi f19, f11, 0xe\n" 
			"mul x28, x0, x11\n"
			"fmul.pi f9, f19, f31\n"
			"packb x11, x11, x28\n"
			"flt.ps f9, f16, f16\n"
			"fcmov.ps f19, f16, f13, f5\n"
			"sltiu x10, x19, 0x1d\n"
			"bgeu x2, x27, 1f\n"
			"1:\n"
			"xori x0, x28, 0xb\n"
			"slti x12, x12, 0x1\n"
			"fclass.ps f19, f28\n"
			"fmin.s f20, f7, f0\n"
			"la x7, rand_ieee754_32\n"
			"flw.ps f19, 192(x7)\n"
			"flw.ps f18, 512(x7)\n"
			"flw.ps f31, 768(x7)\n"
			"fcmov.ps f13, f19, f18, f31\n" 
			"fbci.ps f0, 0x26d84\n"
			"bne x12, x26, 1f\n"
			"1:\n"
			"fle.s x0, f16, f26\n"
			"fsgnj.ps f7, f16, f16\n"
			"fnot.pi f13, f13\n"
			"slli x12, x18, 3\n"
			"fcmovm.ps f16, f26, f26\n"
			"fpackreph.pi f0, f26\n"
			"bgeu x7, x29, 1f\n"
			"1:\n"
			"fexp.ps f0, f13\n"
			"maskxor m3, m3, m6\n"
			"divu x28, x28, x9\n"
			"li x17, 0x245574a374e6f9f3\n"
			"li x7, 0xc58a08cbde320220\n"
			"addw x13, x17, x7\n" 
			"srliw x0, x13, 2\n"
			"frsq.ps f18, f18\n"
			"bge x4, x29, 1f\n"
			"1:\n"
			"sub x10, x0, x18\n"
			"sraiw x0, x11, 2\n"
			"bltu x5, x2, 1f\n"
			"1:\n"
			"fadd.s f11, f18, f3, rne\n"
			"bne x7, x2, 1f\n"
			"1:\n"
			"add x7, x28, x13\n"
			"fmaxu.pi f14, f3, f14\n"
			"fmvs.x.ps x28, f9, 6\n"
			"fcmov.ps f13, f13, f23, f31\n"
			"faddi.pi f16, f9, 0x1f7\n"
			"li x17, 0xdd7f7c0a15c058b6\n"
			"and x11, x17, x17\n" 
			"fmin.ps f9, f9, f9\n"
			"bgeu x3, x17, 1f\n"
			"1:\n"
			"fmadd.s f13, f13, f26, f9, rup\n"
			"beq x19, x3, 1f\n"
			"1:\n"
			"fcvt.pw.ps f0, f7, rup\n"
			"fswizz.ps f7, f28, 0x1f\n"
			"fsrl.pi f13, f13, f28\n"
			"fsra.pi f26, f20, f26\n"
			"fsgnjn.ps f5, f11, f5\n"
			"fcvt.ps.f16 f16, f19\n"
			"fsgnjn.ps f23, f18, f9\n"
			"fcvt.pw.ps f26, f14, rup\n"
			"la x19, rand_int_32\n"
			"flw.ps f23, 864(x19)\n"
			"flw.ps f0, 768(x19)\n"
			"fsub.pi f13, f23, f0\n" 
			"packb x17, x19, x17\n"
			"mova.m.x x10\n"
			"slli x17, x13, 1\n"
			"fnmsub.ps f14, f9, f14, f14, rtz\n"
			"fsrli.pi f16, f28, 0x2\n"
			"flem.ps m3, f11, f28\n"
			"bgeu x2, x27, 1f\n"
			"1:\n"
			"feqm.ps m3, f9, f28\n"
			"fcvt.ps.pwu f13, f11, rmm\n"
			"frcp.ps f3, f28\n"
			"fmaxu.pi f14, f14, f3\n"
			"LBL_SEQID_5_FP_TRANS_ID_134_HID_0:\n"
			"la x13, rand_ieee754_32\n"
			"flw.ps f5, 768(x13)\n"
			"fexp.ps f19, f5\n" 
			"srai x18, x7, 1\n"
			"sltu x17, x10, x0\n"
			"fsgnjx.ps f11, f20, f28\n"
			"fmsub.ps f19, f20, f28, f0, rtz\n"
			"bge x4, x3, 1f\n"
			"1:\n"
			"fcmov.ps f7, f7, f7, f28\n"
			"frcp.ps f28, f28\n"
			"fmadd.ps f19, f11, f20, f7, rtz\n"
			"blt x23, x4, 1f\n"
			"1:\n"
			"srlw x28, x28, x28\n"
			"bltu x3, x2, 1f\n"
			"1:\n"
			"and x28, x7, x28\n"
			"fcvt.ps.f16 f0, f7\n"
			"blt x20, x9, 1f\n"
			"1:\n"
			"la x13, rand_ieee754_32\n"
			"flw.ps f5, 896(x13)\n"
			"fsin.ps f16, f5\n" 
			"ffrc.ps f11, f23\n"
			"andi x12, x12, 0xe\n"
			"fxor.pi f18, f0, f18\n"
			"sub x18, x7, x7\n"
			"fnot.pi f26, f26\n"
			"fnot.pi f0, f0\n"
			"for.pi f3, f0, f7\n"
			"fsatu8.pi f31, f14\n"
			"bne x6, x27, 1f\n"
			"1:\n"
			"fpackreph.pi f7, f7\n"
			"fsgnjn.ps f18, f26, f19\n"
			"la x18, rand_ieee754_32\n"
			"flw.ps f18, 128(x18)\n"
			"flw.ps f13, 992(x18)\n"
			"fle.s x13, f18, f13\n" 
			"maskxor m0, m6, m0\n"
			"mov.m.x m0, x0, 0xff\n"
			"fcmov.ps f3, f19, f26, f9\n"
			"fcmov.ps f26, f13, f16, f28\n"
			"flt.ps f16, f16, f0\n"
			"mulhu x17, x11, x7\n"
			"divu x12, x11, x7\n"
			"slt x18, x18, x28\n"
			"fsra.pi f0, f13, f26\n"
			"sra x17, x0, x17\n"
			"addw x0, x13, x13\n"
			"la x10, rand_ieee754_32\n"
			"flw.ps f13, 512(x10)\n"
			"flw.ps f19, 832(x10)\n"
			"flw.ps f18, 512(x10)\n"
			"fmsub.s f3, f13, f19, f18, rdn\n" 
			"subw x13, x12, x10\n"
			"packb x0, x12, x19\n"
			"bgeu x8, x15, 1f\n"
			"1:\n"
			"flt.ps f26, f16, f9\n"
			"srliw x11, x11, 4\n"
			"fmax.ps f31, f16, f14\n"
			"blt x0, x5, 1f\n"
			"1:\n"
			"or x11, x0, x7\n"
			"ffrc.ps f13, f9\n"
			"fsat8.pi f28, f26\n"
			"fnmsub.ps f19, f19, f23, f23, rtz\n"
			"flt.pi f13, f16, f13\n"
			"LBL_SEQID_5_FP_TRANS_ID_135_HID_0:\n"
			"la x18, rand_ieee754_32\n"
			"flw.ps f7, 192(x18)\n"
			"frcp.ps f26, f7\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"fminu.pi f9, f5, f0\n"
			"fsra.pi f0, f7, f0\n"
			"fmax.pi f0, f5, f5\n"
			"fsrai.pi f16, f31, 0x9\n"
			"fcvt.ps.f16 f13, f14\n"
			"flt.ps f31, f31, f31\n"
			"fandi.pi f16, f0, 0x1c6\n"
			"fsrli.pi f9, f19, 0x0\n"
			"fsgnjn.ps f5, f19, f31\n"
			"blt x26, x4, 1f\n"
			"1:\n"
			"fsat8.pi f14, f5\n"
			"li x9, 0x14cae0a88e1f530b\n"
			"fbcx.ps f18, x9\n" 
			"fcvt.pw.ps f13, f26, rmm\n"
			"fround.ps f18, f9, rup\n"
			"fsll.pi f23, f23, f11\n"
			"feq.pi f3, f28, f11\n"
			"fxor.pi f13, f13, f26\n"
			"fmax.ps f31, f3, f11\n"
			"fsgnj.s f0, f14, f11\n"
			"beq x31, x10, 1f\n"
			"1:\n"
			"feq.pi f0, f5, f7\n"
			"fminu.pi f13, f9, f20\n"
			"fsrl.pi f13, f23, f0\n"
			"beq x18, x5, 1f\n"
			"1:\n"
			"li x17, 0xb44a90d82d892103\n"
			"srliw x11, x17, 1\n" 
			"remw x10, x10, x9\n"
			"div x0, x0, x18\n"
			"add x0, x28, x12\n"
			"andi x11, x18, 0x1d\n"
			"blt x31, x29, 1f\n"
			"1:\n"
			"srliw x9, x0, 5\n"
			"fcmov.ps f14, f7, f18, f3\n"
			"bgeu x24, x10, 1f\n"
			"1:\n"
			"srlw x10, x12, x28\n"
			"beq x12, x5, 1f\n"
			"1:\n"
			"fpackrepb.pi f28, f14\n"
			"rem x7, x28, x12\n"
			"fcvt.f16.ps f3, f28\n"
			"lui x12, 0x1b\n" 
			"fnmsub.ps f23, f0, f23, f19, rne\n"
			"bge x4, x12, 1f\n"
			"1:\n"
			"fmax.s f14, f0, f5\n"
			"fsgnj.s f16, f13, f7\n"
			"bge x24, x1, 1f\n"
			"1:\n"
			"fmin.ps f28, f14, f7\n"
			"fcvt.f16.ps f5, f5\n"
			"fmax.s f3, f16, f3\n"
			"blt x31, x18, 1f\n"
			"1:\n"
			"fcvt.ps.pw f7, f14, rtz\n"
			"fadd.ps f16, f23, f14, rtz\n"
			"fltu.pi f3, f13, f5\n"
			"fmadd.s f5, f13, f14, f3, rtz\n"
			"bgeu x15, x13, 1f\n"
			"1:\n"
			"la x13, rand_int_32\n"
			"flw.ps f9, 768(x13)\n"
			"flw.ps f26, 352(x13)\n"
			"fxor.pi f20, f9, f26\n" 
			"fandi.pi f31, f19, 0x50\n"
			"fsgnjx.s f31, f26, f9\n"
			"fsgnjn.ps f23, f23, f11\n"
			"bge x15, x24, 1f\n"
			"1:\n"
			"fxor.pi f7, f9, f9\n"
			"fswizz.ps f20, f20, 0x2\n"
			"fbci.ps f14, 0x23001\n"
			"bgeu x7, x11, 1f\n"
			"1:\n"
			"mova.m.x x12\n"
			"fle.pi f0, f0, f14\n"
			"fmin.ps f20, f19, f7\n"
			"fmadd.s f5, f5, f28, f5, rdn\n"
			"li x12, 0x60951990e7a0b206\n"
			"fbcx.ps f31, x12\n" 
			"fpackrepb.pi f23, f11\n"
			"fmul.s f28, f0, f0, rmm\n"
			"fslli.pi f0, f0, 0x7\n"
			"fcmov.ps f23, f3, f26, f3\n"
			"fsin.ps f31, f3\n"
			"fsub.s f0, f0, f0, rtz\n"
			"flog.ps f26, f13\n"
			"fnmsub.s f26, f19, f3, f3, rmm\n"
			"fcmov.ps f26, f26, f19, f11\n"
			"fandi.pi f19, f19, 0x1e1\n"
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_5_TLD_XMA_STR_SEQ_ID_147618_HID_0:\n"
			"li x5, ((0x0 << TENSOR_LOAD_T32_USE_TMASK) | (0x0 << TENSOR_LOAD_T32_USE_COOP) | (0x7 << TENSOR_LOAD_T32_OPC_111) | (0x1 << TENSOR_LOAD_T32_DST_START) | (0x205caf802 << TENSOR_LOAD_T32_VADDR) | (0xc << TENSOR_LOAD_T32_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_I8_USE_TMASK) | (0x0 << TENSOR_LOAD_I8_USE_COOP) | (0x1 << TENSOR_LOAD_I8_OPC_001) | (0x1d << TENSOR_LOAD_I8_DST_START) | (0x20a0e1828 << TENSOR_LOAD_I8_VADDR) | (0x0 << TENSOR_LOAD_I8_OFFSET) | (0xe << TENSOR_LOAD_I8_NUM_LINES) | 0 )\n" 
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x3 << TENSOR_IMA_B_NUM_COLS) | (0xc << TENSOR_IMA_A_NUM_ROWS) | (0xe << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x0 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1d << TENSOR_IMA_SCP_LOC_B) | (0x1 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x0 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x9, ((0xf << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0xc << TENSOR_QUANT_QUANT_ROW) | (0x1d << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x1 << TENSOR_QUANT_TRANSF7) | (0xa << TENSOR_QUANT_TRANSF6) | (0xa << TENSOR_QUANT_TRANSF5) | (0x8 << TENSOR_QUANT_TRANSF4) | (0x8 << TENSOR_QUANT_TRANSF3) | (0x5 << TENSOR_QUANT_TRANSF2) | (0x5 << TENSOR_QUANT_TRANSF1) | (0x9 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"LBL_SEQID_5_TLD_XMA_STR_SEQ_ID_419131_HID_0:\n"
			"li x5, ((0x0 << TENSOR_LOAD_I8_USE_TMASK) | (0x0 << TENSOR_LOAD_I8_USE_COOP) | (0x1 << TENSOR_LOAD_I8_OPC_001) | (0x5 << TENSOR_LOAD_I8_DST_START) | (0x20001b1 << TENSOR_LOAD_I8_VADDR) | (0x0 << TENSOR_LOAD_I8_OFFSET) | (0x8 << TENSOR_LOAD_I8_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_SETUP_B_USE_COOP) | (0x0 << TENSOR_LOAD_SETUP_B_OPC_000) | (0x1 << TENSOR_LOAD_SETUP_B_OPC_1) | (0x205c17b60 << TENSOR_LOAD_SETUP_B_VADDR) | (0xe << TENSOR_LOAD_SETUP_B_NUM_LINES) | 0 )\n" 
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x2 << TENSOR_IMA_B_NUM_COLS) | (0x8 << TENSOR_IMA_A_NUM_ROWS) | (0xe << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x0 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x1 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1a << TENSOR_IMA_SCP_LOC_B) | (0x5 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x9, ((0x8 << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0x8 << TENSOR_QUANT_QUANT_ROW) | (0xd << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x1 << TENSOR_QUANT_TRANSF7) | (0xa << TENSOR_QUANT_TRANSF6) | (0xa << TENSOR_QUANT_TRANSF5) | (0xa << TENSOR_QUANT_TRANSF4) | (0x5 << TENSOR_QUANT_TRANSF3) | (0x8 << TENSOR_QUANT_TRANSF2) | (0x4 << TENSOR_QUANT_TRANSF1) | (0x9 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 7\n"
			"csrwi tensor_wait, 10\n"
			"LBL_SEQID_5_TLD_XMA_STR_SEQ_ID_404197_HID_0:\n"
			"la x31, self_check_16\n"
			"li x5, 0xa0000000000009\n"
			"add x5, x5, x31\n"
			"li x6, 0x10000000000003\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x2 << TENSOR_FMA16_B_NUM_COLS) | (0x9 << TENSOR_FMA16_A_NUM_ROWS) | (0x3 << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x1 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1e << TENSOR_FMA16_SCP_LOC_B) | (0x5 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x9, ((0xc << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0x9 << TENSOR_QUANT_QUANT_ROW) | (0xc << TENSOR_QUANT_QUANT_SCP_LOC) | (0x2 << TENSOR_QUANT_TRANSF9) | (0x6 << TENSOR_QUANT_TRANSF8) | (0x6 << TENSOR_QUANT_TRANSF7) | (0x6 << TENSOR_QUANT_TRANSF6) | (0x7 << TENSOR_QUANT_TRANSF5) | (0x6 << TENSOR_QUANT_TRANSF4) | (0x6 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0x8\n"
			"bne x5, x31, LBL_FAIL_HID_0\n"
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 10\n"
			"LBL_SEQID_5_TLD_XMA_STR_SEQ_ID_762552_HID_0:\n"
			"la x31, self_check_8\n"
			"li x5, 0x60000000000004\n"
			"add x5, x5, x31\n"
			"li x6, 0x10000000000000\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x2 << TENSOR_IMA_B_NUM_COLS) | (0x4 << TENSOR_IMA_A_NUM_ROWS) | (0x0 << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x1 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1b << TENSOR_IMA_SCP_LOC_B) | (0x3 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x9, ((0x4 << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0x4 << TENSOR_QUANT_QUANT_ROW) | (0x3b << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x1 << TENSOR_QUANT_TRANSF5) | (0x8 << TENSOR_QUANT_TRANSF4) | (0xa << TENSOR_QUANT_TRANSF3) | (0xa << TENSOR_QUANT_TRANSF2) | (0x5 << TENSOR_QUANT_TRANSF1) | (0x4 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x10, ((0x2 << TENSOR_STORE_SCP_ENTRY_STRIDE) | (0x28 << TENSOR_STORE_SCP_START_SCP_ENTRY) | (0xa << TENSOR_STORE_SCP_AROWS) | (0x1 << TENSOR_STORE_SCP_OPC_1) | (0x2064af802 << TENSOR_STORE_SCP_VADDR) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fmvz.x.ps x31, f0, 0\n"
			"li x5, 0x4\n"
			"bne x5, x31, LBL_FAIL_HID_0\n"
			"csrw tensor_quant, x9\n"	
			"li x31, (0x3 << 6)\n"
			"csrwi tensor_wait, 10\n"
			"csrw tensor_store, x10\n"	
			"csrwi tensor_wait, 8\n"
			"LBL_SEQID_5_TLD_XMA_STR_SEQ_ID_803889_HID_0:\n"
			"li x5, ((0x0 << TENSOR_LOAD_T32_USE_TMASK) | (0x0 << TENSOR_LOAD_T32_USE_COOP) | (0x7 << TENSOR_LOAD_T32_OPC_111) | (0x1 << TENSOR_LOAD_T32_DST_START) | (0x2fe011e << TENSOR_LOAD_T32_VADDR) | (0x8 << TENSOR_LOAD_T32_NUM_LINES) | 0 )\n" 
			"li x6, ((0x0 << TENSOR_LOAD_T32_USE_TMASK) | (0x0 << TENSOR_LOAD_T32_USE_COOP) | (0x7 << TENSOR_LOAD_T32_OPC_111) | (0x1a << TENSOR_LOAD_T32_DST_START) | (0x205bfdb10 << TENSOR_LOAD_T32_VADDR) | (0xc << TENSOR_LOAD_T32_NUM_LINES) | 0 )\n" 
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x2 << TENSOR_FMA32_B_NUM_COLS) | (0x8 << TENSOR_FMA32_A_NUM_ROWS) | (0xc << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1a << TENSOR_FMA32_SCP_LOC_B) | (0x1 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x5 << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0x8 << TENSOR_QUANT_QUANT_ROW) | (0x12 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x0 << TENSOR_QUANT_TRANSF1) | (0x2 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x10, ((0x0 << TENSOR_STORE_REG_STRIDE) | (0x1c << TENSOR_STORE_START_REG) | (0x1 << TENSOR_STORE_STORE_COLS) | (0x7 << TENSOR_STORE_AROWS) | (0x0 << TENSOR_STORE_COOP_STORE) | (0x0 << TENSOR_STORE_OPC_0) | (0x829d47ef7 << TENSOR_STORE_VADDR) | 0 )\n" 
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
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_0\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_5_XMA_QNT_STR_SEQ_ITER_0_HID_0:\n"
			"li x7, ((0x1 << TENSOR_STORE_SCP_ENTRY_STRIDE) | (0x35 << TENSOR_STORE_SCP_START_SCP_ENTRY) | (0x5 << TENSOR_STORE_SCP_AROWS) | (0x1 << TENSOR_STORE_SCP_OPC_1) | (0x20a751fbd << TENSOR_STORE_SCP_VADDR) | 0 )\n" 
			"li x31, (0x3 << 6)\n"
			"csrw tensor_store, x7\n"	
			"csrwi tensor_wait, 8\n"
			"LBL_SEQID_5_XMA_QNT_STR_SEQ_ITER_1_HID_0:\n"
			"li x5, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x2 << TENSOR_FMA16_B_NUM_COLS) | (0xe << TENSOR_FMA16_A_NUM_ROWS) | (0x9 << TENSOR_FMA16_A_NUM_COLS) | (0xb << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x41 << TENSOR_FMA16_SCP_LOC_B) | (0x24 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x0 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x6, ((0xe << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0xe << TENSOR_QUANT_QUANT_ROW) | (0x2b << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x2 << TENSOR_QUANT_TRANSF5) | (0x7 << TENSOR_QUANT_TRANSF4) | (0x7 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_5_XMA_QNT_STR_SEQ_ITER_2_HID_0:\n"
			"li x5, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x1 << TENSOR_FMA32_B_NUM_COLS) | (0x8 << TENSOR_FMA32_A_NUM_ROWS) | (0xe << TENSOR_FMA32_A_NUM_COLS) | (0xd << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0xc6 << TENSOR_FMA32_SCP_LOC_B) | (0x2e << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x0 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x7, ((0x3 << TENSOR_STORE_SCP_ENTRY_STRIDE) | (0x12 << TENSOR_STORE_SCP_START_SCP_ENTRY) | (0x0 << TENSOR_STORE_SCP_AROWS) | (0x1 << TENSOR_STORE_SCP_OPC_1) | (0x2062dbfc4 << TENSOR_STORE_SCP_VADDR) | 0 )\n" 
			"li x31, (0x2 << 6)\n"
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
			"csrwi tensor_wait, 10\n"
			"csrw tensor_store, x7\n"	
			"csrwi tensor_wait, 8\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_0\n"
			"csrw tensor_error, x0\n"
			"mov.m.x m0, x0, 0xFF\n"
			"li x5, 0xc0\n"
			"la x31, rand_ieee754_32\n"
			"add x31, x31, x5\n"
			"flw.ps f0, 0(x31)\n"
			"flw.ps f11, 1(x31)\n"
			"flw.ps f13, 8(x31)\n"
			"flw.ps f14, 3(x31)\n"
			"flw.ps f16, 10(x31)\n"
			"flw.ps f18, 19(x31)\n"
			"flw.ps f19, 9(x31)\n"
			"flw.ps f20, 0(x31)\n"
			"flw.ps f23, 11(x31)\n"
			"flw.ps f26, 5(x31)\n"
			"flw.ps f28, 22(x31)\n"
			"flw.ps f3, 11(x31)\n"
			"flw.ps f31, 29(x31)\n"
			"flw.ps f5, 20(x31)\n"
			"flw.ps f7, 43(x31)\n"
			"flw.ps f9, 33(x31)\n"
			"la x31, rand_int_32\n"
			"add x31, x31, x5\n"
			"lw x0, 0(x31)\n"
			"lw x10, 3(x31)\n"
			"lw x11, 7(x31)\n"
			"lw x12, 10(x31)\n"
			"lw x13, 2(x31)\n"
			"lw x17, 0(x31)\n"
			"lw x18, 21(x31)\n"
			"lw x19, 11(x31)\n"
			"lw x28, 31(x31)\n"
			"lw x7, 4(x31)\n"
			"lw x9, 13(x31)\n"
			"li x21, 0x3\n"
			"LBL_SEQID_5_LOOP_SEQ_HID_0:\n"
			"mulh x12, x18, x7\n"
			"fcvt.wu.s x9, f16, rne\n"
			"fnmadd.ps f26, f19, f7, f7, rne\n"
			"fltu.pi f16, f16, f28\n"
			"div x10, x18, x19\n"
			"fxor.pi f31, f0, f28\n"
			"fnmsub.s f16, f0, f16, f16, rne\n"
			"bltu x24, x2, 1f\n"
			"1:\n"
			"fsgnjx.ps f0, f23, f13\n"
			"maskand m0, m0, m3\n"
			"mov.m.x m0, x0, 0xff\n"
			"fltm.ps m2, f23, f14\n"
			"blt x28, x30, 1f\n"
			"1:\n"
			"add x12, x7, x12\n"
			"fxor.pi f26, f7, f26\n"
			"fsgnjn.s f20, f13, f16\n"
			"auipc x10, 0x13\n"
			"sraiw x11, x13, 3\n"
			"fcvt.ps.pwu f5, f16, rne\n"
			"fsgnjx.s f19, f19, f20\n"
			"fle.s x28, f5, f18\n"
			"fswizz.ps f20, f18, 0x6\n"
			"fpackreph.pi f28, f20\n"
			"fsll.pi f28, f0, f0\n"
			"srl x0, x11, x11\n"
			"mulh x10, x12, x10\n"
			"div x28, x7, x0\n"
			"fsgnj.s f20, f9, f0\n"
			"masknot m6, m3\n"
			"fpackrepb.pi f9, f7\n"
			"bgeu x13, x20, 1f\n"
			"1:\n"
			"fsub.pi f20, f14, f26\n"
			"bltu x8, x18, 1f\n"
			"1:\n"
			"fmul.ps f9, f14, f9, rtz\n"
			"bgeu x29, x14, 1f\n"
			"1:\n"
			"mul x28, x13, x7\n"
			"bne x21, x16, 1f\n"
			"1:\n"
			"ecall\n"
			"beq x27, x7, 1f\n"
			"1:\n"
			"divw x17, x18, x7\n"
			"rem x12, x12, x18\n"
			"ffrc.ps f9, f9\n"
			"for.pi f18, f19, f3\n"
			"bltu x31, x16, 1f\n"
			"1:\n"
			"fmul.pi f23, f7, f14\n"
			"fltu.pi f19, f11, f20\n"
			"fcmovm.ps f9, f26, f3\n"
			"bltu x3, x10, 1f\n"
			"1:\n"
			"fsgnj.s f13, f31, f26\n"
			"blt x16, x29, 1f\n"
			"1:\n"
			"packb x0, x0, x18\n"
			"div x12, x13, x7\n"
			"fnmsub.ps f9, f5, f0, f16, rdn\n"
			"fsra.pi f20, f0, f3\n"
			"bne x9, x31, 1f\n"
			"1:\n"
			"fsub.s f20, f20, f28, rmm\n"
			"bltu x10, x16, 1f\n"
			"1:\n"
			"fmax.pi f28, f16, f18\n"
			"mulh x17, x28, x17\n"
			"blt x12, x14, 1f\n"
			"1:\n"
			"fmin.ps f26, f11, f31\n"
			"fcvt.f16.ps f20, f11\n"
			"blt x22, x10, 1f\n"
			"1:\n"
			"mova.x.m x17\n"
			"lui x17, 0x18\n"
			"fmulhu.pi f28, f28, f11\n"
			"blt x25, x12, 1f\n"
			"1:\n"
			"auipc x19, 0x1f\n"
			"fmin.pi f0, f13, f7\n"
			"bne x13, x24, 1f\n"
			"1:\n"
			"ecall\n"
			"flog.ps f3, f28\n"
			"fxor.pi f20, f16, f23\n"
			"bgeu x3, x27, 1f\n"
			"1:\n"
			"for.pi f31, f16, f26\n"
			"bgeu x30, x18, 1f\n"
			"1:\n"
			"fmadd.s f13, f5, f16, f20, rmm\n"
			"fltm.pi m3, f7, f9\n"
			"fcvt.s.wu f23, x0, rup\n"
			"ecall\n"
			"fltm.ps m3, f19, f14\n"
			"fmvs.x.ps x18, f14, 0\n"
			"fmin.ps f26, f13, f20\n"
			"bltu x16, x2, 1f\n"
			"1:\n"
			"fsll.pi f18, f14, f28\n"
			"fmadd.s f26, f31, f31, f14, rne\n"
			"fsra.pi f9, f20, f5\n"
			"fand.pi f31, f19, f7\n"
			"fmv.x.s x19, f19\n"
			"packb x13, x18, x11\n"
			"fnmadd.ps f0, f0, f23, f16, rup\n"
			"fmax.pi f11, f14, f16\n"
			"blt x31, x24, 1f\n"
			"1:\n"
			"fcvt.pwu.ps f23, f0, rup\n"
			"sra x11, x13, x11\n"
			"bgeu x29, x30, 1f\n"
			"1:\n"
			"fle.pi f7, f7, f11\n"
			"fbci.ps f19, 0x22dcb\n"
			"remuw x0, x28, x18\n"
			"bne x0, x3, 1f\n"
			"1:\n"
			"fmax.pi f13, f5, f28\n"
			"divu x18, x18, x7\n"
			"fmul.pi f13, f13, f28\n"
			"fmax.ps f13, f13, f5\n"
			"fcmovm.ps f18, f7, f31\n"
			"bge x16, x14, 1f\n"
			"1:\n"
			"fswizz.ps f13, f23, 0x7\n"
			"sll x28, x28, x9\n"
			"fcmovm.ps f9, f0, f14\n"
			"fmin.pi f7, f31, f13\n"
			"slli x10, x17, 5\n"
			"bltu x20, x16, 1f\n"
			"1:\n"
			"fnmsub.s f0, f13, f3, f20, rtz\n"
			"fltu.pi f26, f7, f26\n"
			"addi x9, x9, 0x1e\n"
			"bltu x3, x1, 1f\n"
			"1:\n"
			"fsatu8.pi f16, f16\n"
			"flt.ps f0, f16, f18\n"
			"beq x22, x24, 1f\n"
			"1:\n"
			"fsgnj.s f9, f16, f11\n"
			"bgeu x14, x20, 1f\n"
			"1:\n"
			"fnmadd.ps f16, f16, f20, f14, rup\n"
			"sltiu x9, x10, 0xd\n"
			"bge x16, x14, 1f\n"
			"1:\n"
			"addw x10, x13, x10\n"
			"fltm.pi m3, f20, f16\n"
			"frsq.ps f9, f14\n"
			"fsrl.pi f7, f18, f9\n"
			"bge x24, x12, 1f\n"
			"1:\n"
			"maskand m2, m6, m2\n"
			"masknot m0, m0\n"
			"mov.m.x m0, x0, 0xff\n"
			"maskpopcz x7, m0\n"
			"maskpopcz x0, m6\n"
			"fltm.pi m3, f20, f26\n"
			"fmin.s f16, f26, f28\n"
			"fmul.pi f18, f0, f0\n"
			"divu x11, x12, x9\n"
			"fnot.pi f11, f23\n"
			"flt.ps f31, f31, f31\n"
			"slti x0, x17, 0x12\n"
			"bgeu x1, x5, 1f\n"
			"1:\n"
			"fmin.pi f14, f18, f19\n"
			"addw x11, x7, x9\n"
			"addi x28, x11, 0x1e\n"
			"srli x0, x9, 4\n"
			"fle.pi f3, f23, f31\n"
			"flog.ps f9, f9\n"
			"fcvt.pwu.ps f23, f19, rup\n"
			"fsetm.pi m0, f31\n"
			"mov.m.x m0, x0, 0xff\n"
			"fmvs.x.ps x13, f31, 2\n"
			"sraiw x17, x18, 2\n"
			"fsrl.pi f31, f11, f3\n"
			"fle.ps f9, f31, f23\n"
			"fsub.s f26, f5, f28, rtz\n"
			"fcvt.ps.pwu f20, f5, rdn\n"
			"sub x17, x12, x17\n"
			"bne x2, x9, 1f\n"
			"1:\n"
			"divuw x17, x9, x10\n"
			"fsrai.pi f19, f16, 0xd\n"
			"maskor m6, m6, m6\n"
			"fnmadd.ps f16, f7, f23, f3, rmm\n"
			"maskpopc x19, m6\n"
			"fmvz.x.ps x13, f3, 1\n"
			"fmul.pi f13, f3, f13\n"
			"fcvt.ps.pw f18, f26, rup\n"
			"bltu x14, x13, 1f\n"
			"1:\n"
			"auipc x13, 0x8\n"
			"maskand m0, m6, m2\n"
			"mov.m.x m0, x0, 0xff\n"
			"srliw x9, x18, 1\n"
			"beq x28, x18, 1f\n"
			"1:\n"
			"maskpopc x0, m3\n"
			"mova.m.x x10\n"
			"fbci.pi f0, 0x1b233\n"
			"fnmadd.ps f23, f23, f18, f3, rdn\n"
			"addw x0, x13, x0\n"
			"beq x26, x0, 1f\n"
			"1:\n"
			"fmvz.x.ps x28, f0, 4\n"
			"bgeu x24, x29, 1f\n"
			"1:\n"
			"div x0, x28, x17\n"
			"mul x12, x10, x12\n"
			"fmulhu.pi f5, f0, f28\n"
			"divuw x7, x17, x28\n"
			"bltu x11, x7, 1f\n"
			"1:\n"
			"rem x18, x28, x11\n"
			"bne x9, x6, 1f\n"
			"1:\n"
			"xori x12, x7, 0x1c\n"
			"fsll.pi f14, f14, f14\n"
			"ffrc.ps f16, f0\n"
			"fsrai.pi f13, f31, 0xe\n"
			"fmadd.ps f20, f13, f28, f23, rne\n"
			"add x10, x12, x10\n"
			"fnot.pi f31, f7\n"
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
			"LBL_HPM_3_CORE_RETIRED_INST0_FAIL_HID_0:\n"          
			"csrr x5, mhpmcounter3\n"
			"sd x5, 32 (x31)\n"
			"LBL_HPM_4_CORE_TQUANT_INST_FAIL_HID_0:\n"          
			"csrr x5, mhpmcounter4\n"
			"sd x5, 40 (x31)\n"
			"LBL_HPM_5_CORE_TL_INST_FAIL_HID_0:\n"          
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
			"LBL_HPM_3_CORE_RETIRED_INST0_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter3\n"
			"sd x5, 32 (x31)\n"
			"LBL_HPM_4_CORE_TQUANT_INST_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter4\n"
			"sd x5, 40 (x31)\n"
			"LBL_HPM_5_CORE_TL_INST_PASS_HID_0:\n"          
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
		0x42aae856, 
		0xe3de453d, 
		0x93445943, 
		0x2dfe0fd0, 
		0x78351d8f, 
		0x3293b739, 
		0xe128e9ab, 
		0x2ab7b3fb, 
		0x77b4437a, 
		0xc4ee373d, 
		0x378ac694, 
		0x2cec986c, 
		0xff46db0c, 
		0xe1590a41, 
		0x840acb5c, 
		0xda3e472c, 
		0xa4f4fa92, 
		0x5522cdb7, 
		0x5f6457b1, 
		0xd4a451d4, 
		0x89832878, 
		0xce789b22, 
		0xc7467a94, 
		0x4a0302d2, 
		0x0e5e719e, 
		0x5e3fb8e9, 
		0x4a679350, 
		0x4ee035bc, 
		0x954e9de4, 
		0x48820428, 
		0x32350435, 
		0xb1b799f5, 
		0x238a4f16, 
		0x61d72e1e, 
		0xcbb4cb81, 
		0xc9e0860d, 
		0xb0e113d4, 
		0xc17a8bdf, 
		0xe23a4bf0, 
		0x8b4e928d, 
		0x6e67dc31, 
		0x43c2058b, 
		0xe7c654db, 
		0xb25a51ca, 
		0xdc736c92, 
		0xf498fe00, 
		0x376677fa, 
		0x1d28007f, 
		0xe6cbfebf, 
		0x574ba713, 
		0x0e9a8c80, 
		0xf109f362, 
		0xcbf07830, 
		0x2d45df28, 
		0x851fd523, 
		0x78e08c16, 
		0x3b2c3214, 
		0x70c13b43, 
		0x398bd24f, 
		0xcb91ec65, 
		0xb100bf1e, 
		0xa523aaee, 
		0x2144e9f9, 
		0xc306d616, 
		0x6e0da536, 
		0x3026d5b7, 
		0xb4725122, 
		0x3ad07825, 
		0x20a3ebed, 
		0x49f9e873, 
		0xab1c0e36, 
		0x81258b24, 
		0xb830a49e, 
		0xd125ef27, 
		0x7482161e, 
		0x7896255e, 
		0x2a0fbed7, 
		0x35f3051c, 
		0xeb966fde, 
		0x2416fdfc, 
		0xb3064152, 
		0x7df44403, 
		0x83cd8b42, 
		0x823d2142, 
		0xff9fc9eb, 
		0x54a42366, 
		0x836e4789, 
		0xc6373d67, 
		0x0ac78258, 
		0x2169b3cd, 
		0x1dbe8eee, 
		0xd88dd77f, 
		0x59713855, 
		0x1fa3bc3c, 
		0x0ee2d106, 
		0xc83922ba, 
		0x375cf6d3, 
		0xe4070e86, 
		0xc225c81b, 
		0x4a1b4e2a, 
		0x9f582778, 
		0xd0061e2c, 
		0x1773234e, 
		0x4c943fb0, 
		0xc7f9a587, 
		0xaf51bee5, 
		0xa854c9eb, 
		0x885dc01b, 
		0x16f21e74, 
		0x983e70ec, 
		0xf738b7ba, 
		0x252dc654, 
		0xe36888cf, 
		0x381fc5cf, 
		0x781a7b9e, 
		0x6c64b42a, 
		0x0379a05d, 
		0x415df2f0, 
		0x76ba7a47, 
		0x33e7ce51, 
		0x4c8859a5, 
		0x7009bc8e, 
		0x7c2ccbff, 
		0xc44525c5, 
		0x24297248, 
		0x91bfa1ec, 
		0x89387faf, 
		0x9670e180, 
		0xca877a5e, 
		0x2ed04386, 
		0x5a9760d7, 
		0x30778640, 
		0x117c78e6, 
		0xe4df05c3, 
		0xba27ff47, 
		0xb3360681, 
		0xb53ae62e, 
		0x56a99277, 
		0x850b713e, 
		0xc7cde1c9, 
		0x49b3b6b7, 
		0xc4bdf98e, 
		0x07374cf7, 
		0x1ae81d01, 
		0x4d53cd02, 
		0x53c667e6, 
		0x851c9c40, 
		0xb79e7e13, 
		0x9529b3aa, 
		0xf5cbba83, 
		0x37096fb4, 
		0x5fd22d84, 
		0x07bbac44, 
		0x1c78ecae, 
		0xa938487e, 
		0xff5c60ce, 
		0xd55a0834, 
		0x52724838, 
		0x5b051238, 
		0xcf1ce4f1, 
		0xaa90b713, 
		0xacb13121, 
		0x315f46a8, 
		0xfb0acf1f, 
		0xd81b12f0, 
		0xf8382387, 
		0x4f43e574, 
		0x0d2349c0, 
		0xc6c39b0f, 
		0xf5f5d6e0, 
		0xc69ed5b1, 
		0x575f5a8f, 
		0xac75589d, 
		0x71fa40c6, 
		0x23673873, 
		0x3c453a6c, 
		0x15e1f81e, 
		0xd50da953, 
		0x685292ef, 
		0x6eaa1a98, 
		0xcc9580db, 
		0x57f36b90, 
		0x06a5a5f6, 
		0x1b4babda, 
		0xec6a62a7, 
		0x32d58b1e, 
		0x686cb5cf, 
		0xcc59123f, 
		0x701f4e80, 
		0x1fafc4a8, 
		0x5107f012, 
		0x8807966a, 
		0xe402b6da, 
		0xee66ed43, 
		0x00f507e4, 
		0x8d64e002, 
		0xbdad203a, 
		0x82808aff, 
		0x99c9c796, 
		0xab7fbd90, 
		0xc01f13c8, 
		0x6f1ecf27, 
		0xae594563, 
		0xa6661d12, 
		0x5383a0c4, 
		0x8be9b17c, 
		0x050797fb, 
		0x800ccce2, 
		0x1434aa20, 
		0x3ad39241, 
		0x9e37db76, 
		0x4a035a16, 
		0x36739421, 
		0xee63a654, 
		0x6d3ac141, 
		0xb1fe3e73, 
		0xb437b6d1, 
		0x1ef9be17, 
		0x30b1f819, 
		0x0c728a38, 
		0xec0b408c, 
		0x4cbffc05, 
		0x5f4ee6dc, 
		0xeef18b8f, 
		0x813c77a2, 
		0x1b3670ef, 
		0x72d89b8d, 
		0xcff524aa, 
		0xe81dcd20, 
		0x955b1d93, 
		0x1a05f0af, 
		0x497f0d0a, 
		0x6b9ada33, 
		0x51991086, 
		0xcd934c9e, 
		0xdda004d5, 
		0x210820fa, 
		0x4190c328, 
		0x798db096, 
		0xdfb0c999, 
		0xbfdd71c5, 
		0x398f6a9b, 
		0xbeb01ac8, 
		0x76882bdd, 
		0x9463bea0, 
		0x5b155829, 
		0xa5b1ab04, 
		0x118e0c4c, 
		0xad88fe2f, 
		0xe9ddd04c, 
		0x25ad2e5a, 
		0x49b288d9, 
		0x683bc3df, 
		0x7bb06d7f, 
		0xa76b2bc3, 
		0x04458d1b  
};
volatile uint32_t rand_ieee754_32[1536] __attribute__ ((aligned (2048))) = {
		0x807fffff, 
		0x41820e2b, 
		0xc1400000, 
		0x7f7fffff, 
		0x005bbf2a, 
		0xa31bd7fb, 
		0xc1100000, 
		0x3a569a3f, 
		0xc23c0000, 
		0x3c721d47, 
		0x7f800000, 
		0x420c0000, 
		0x42440000, 
		0xa12d5b6b, 
		0x8f7ffffe, 
		0x9d13b64e, 
		0x80000001, 
		0x7f7fffff, 
		0xd2aaf32b, 
		0xc26c0000, 
		0x8022168f, 
		0xaaaaaaaa, 
		0x0009c5e0, 
		0xc0000000, 
		0x908ffed7, 
		0xc2480000, 
		0x7b6c9952, 
		0x2d51ffa1, 
		0x00800003, 
		0x4b8c4b40, 
		0x42200000, 
		0x0c7fff80, 
		0x941dff10, 
		0x4868267b, 
		0x0c7e0000, 
		0x80800001, 
		0x13ff8916, 
		0x0f7ffffc, 
		0x396229ff, 
		0x1a9f5aed, 
		0x42200000, 
		0x00000000, 
		0x80000000, 
		0x89e8f140, 
		0x4c8b70be, 
		0xff7fffff, 
		0x4ce4b7d0, 
		0x41f00000, 
		0x00683207, 
		0x0c7fe000, 
		0x0c7ffc00, 
		0x6d3685c2, 
		0xc1b80000, 
		0x0e007fff, 
		0x47894a71, 
		0x40400000, 
		0x80001000, 
		0x4b8c4b40, 
		0x80000000, 
		0x80000100, 
		0x42400000, 
		0x669c3481, 
		0xc23c0000, 
		0x00800002, 
		0x80010000, 
		0xb89700c7, 
		0xc1500000, 
		0x00000000, 
		0x00200000, 
		0x41880000, 
		0xe3b06f06, 
		0x80004000, 
		0xc2080000, 
		0x42000000, 
		0xc1c80000, 
		0xd61da2f3, 
		0x00000000, 
		0x0e3fffff, 
		0x80040000, 
		0x7f800000, 
		0xdf2067b1, 
		0x0e000fff, 
		0x8c950817, 
		0xc1e80000, 
		0x29b6dd0d, 
		0xd3d1a79d, 
		0x00000000, 
		0x807fffff, 
		0x40000000, 
		0x88516121, 
		0xff7fffff, 
		0xf5f474d2, 
		0x80800003, 
		0x42640000, 
		0x3f800000, 
		0x65d60583, 
		0x80ffffff, 
		0xff800000, 
		0xc1880000, 
		0x42580000, 
		0x5b00fe2a, 
		0x0c7ff800, 
		0x41d00000, 
		0x0c600000, 
		0xc0000000, 
		0xc2400000, 
		0x7f800000, 
		0x80000000, 
		0x06d812f2, 
		0xc16b7f00, 
		0x0e00000f, 
		0xa9c96f1a, 
		0xc1f80000, 
		0x0e00007f, 
		0x00000080, 
		0x7f800000, 
		0x40a00000, 
		0x80800003, 
		0x80000800, 
		0x41c00000, 
		0x41b00000, 
		0x425c0000, 
		0x00000000, 
		0x00000040, 
		0xa66e5da9, 
		0xcc31fe5d, 
		0x42700000, 
		0x406ef7b8, 
		0xc2400000, 
		0x499c9c2f, 
		0xc2640000, 
		0x55db43bb, 
		0xc2180000, 
		0x486a36cf, 
		0x3f028f5c, 
		0x00000000, 
		0x2fdad9cd, 
		0xc2100000, 
		0x42780000, 
		0x0e000003, 
		0x0e1fffff, 
		0x80020000, 
		0xc5ce0c7d, 
		0xbf977b2f, 
		0x00000000, 
		0x55555555, 
		0xa805e8fb, 
		0x80000000, 
		0xca11a81b, 
		0x0d000ff0, 
		0x00000004, 
		0xff800001, 
		0x2816b2c1, 
		0x42640000, 
		0x40000000, 
		0xc2580000, 
		0xff8af4e3, 
		0x80800000, 
		0x41800000, 
		0x80800001, 
		0x7f800000, 
		0x0e07ffff, 
		0x9a076fd1, 
		0x4ba14fcd, 
		0xff800000, 
		0x80419c9e, 
		0xc1c80000, 
		0x42000000, 
		0x7f800001, 
		0xd152a65b, 
		0x00000000, 
		0x00080000, 
		0x2ae5652c, 
		0xc2180000, 
		0x698eae8f, 
		0xbf8c81b5, 
		0x41980000, 
		0x0e07ffff, 
		0xff800000, 
		0x950a5ea7, 
		0x15ab3e6e, 
		0x7f800000, 
		0x00000800, 
		0x00000000, 
		0x42600000, 
		0xc1900000, 
		0x574f119c, 
		0x7fffffff, 
		0x0c700000, 
		0xc2100000, 
		0x3f028f5c, 
		0xd9028462, 
		0x41a80000, 
		0x00000002, 
		0x70652741, 
		0x00000800, 
		0x0c7c0000, 
		0x0c7ff000, 
		0x7f7fffff, 
		0x57a7fbd1, 
		0x0c7f8000, 
		0x00000000, 
		0x3d13b728, 
		0x904af400, 
		0x00800002, 
		0x80000000, 
		0x42140000, 
		0x85b31805, 
		0x0e001fff, 
		0x7e3a32b7, 
		0xc2440000, 
		0x7f800000, 
		0x7fc00001, 
		0x0c600000, 
		0x41900000, 
		0x49267654, 
		0x24292c6e, 
		0x7fffffff, 
		0xa50c211c, 
		0x8004cc6d, 
		0x00020000, 
		0x95149806, 
		0x00008000, 
		0x0e07ffff, 
		0x00010000, 
		0x566534ef, 
		0x0a9ef55e, 
		0x42080000, 
		0x00800001, 
		0x00000000, 
		0x42700000, 
		0x0c7ffff8, 
		0x52ddd31d, 
		0x0c7fffe0, 
		0x00000000, 
		0x00000080, 
		0xc2380000, 
		0x42640000, 
		0xc2600000, 
		0xc2740000, 
		0x00000100, 
		0xc1500000, 
		0xc2340000, 
		0x31d4cc15, 
		0x42040000, 
		0x8006579f, 
		0x7f800001, 
		0xff800001, 
		0xc2400000, 
		0xffbfffff, 
		0xc1c00000, 
		0xff800000, 
		0x4b74ce81, 
		0x0e3fffff, 
		0x41900000, 
		0xbcef366f, 
		0xc2780000, 
		0x7f8d0c7b, 
		0x40a00000, 
		0xc1ddf6f7, 
		0xc2680000, 
		0xc1a00000, 
		0x41c80000, 
		0x0c7fc000, 
		0xdc8395f8, 
		0x006ab4e7, 
		0x3f214dc0, 
		0x3f800000, 
		0x33f0ed6a, 
		0x4a667ec4, 
		0x7f7ffffe, 
		0xb05fa80b, 
		0xc20c0000, 
		0x80001000, 
		0x00000000, 
		0x0e0001ff, 
		0xff7fffff, 
		0x80ffffff, 
		0x80800001, 
		0xf4b33d2c, 
		0x80000000, 
		0x7a173ab9, 
		0x4cc5ab22, 
		0x42040000, 
		0xa32f0e21, 
		0x7f800000, 
		0x421c0000, 
		0x42640000, 
		0xc2280000, 
		0xff800000, 
		0xffb3c1d5, 
		0x80000000, 
		0x00000000, 
		0x16cf352f, 
		0x80000000, 
		0x0c7fffc0, 
		0xff7ffffe, 
		0x80800002, 
		0x44aa84b7, 
		0x41c00000, 
		0xb4aef5f7, 
		0xb2306f23, 
		0x0c7c0000, 
		0x007fffff, 
		0x00400000, 
		0x224e9267, 
		0xff7ffffe, 
		0xff800000, 
		0xff800000, 
		0x00000001, 
		0x0ab3008a, 
		0x35139ee0, 
		0x106059d6, 
		0xc1900000, 
		0xc1980000, 
		0x3f800000, 
		0x422c0000, 
		0x40f91ed3, 
		0x0c7ffff8, 
		0x7e755e1d, 
		0x00010000, 
		0x0f7ffffd, 
		0x804d5727, 
		0x41c00000, 
		0x80010000, 
		0x41600000, 
		0x426c0000, 
		0x7f7fffff, 
		0xb68108b0, 
		0x00200000, 
		0xd3acddac, 
		0x8b78c83d, 
		0x42200000, 
		0xff7fffff, 
		0xcccccccc, 
		0x0c7ffe00, 
		0xc20c0000, 
		0x80800003, 
		0x2555c555, 
		0x0c700000, 
		0x00800001, 
		0x0c780000, 
		0x41300000, 
		0x80800001, 
		0xff7ffffe, 
		0xc15c9376, 
		0xff94e701, 
		0x3e24aee8, 
		0xff7fffff, 
		0x3cdc43e6, 
		0x00080000, 
		0x0c7ffff0, 
		0x33db3254, 
		0x00800002, 
		0x0e00007f, 
		0x0e07ffff, 
		0x7f800000, 
		0xd6ff0914, 
		0x80800003, 
		0x7f7ffffe, 
		0x423c0000, 
		0x84eb949b, 
		0xfffd7353, 
		0x42780000, 
		0xa442c02b, 
		0xc2740000, 
		0x0e0003ff, 
		0x80000001, 
		0x0e00000f, 
		0x0e00ffff, 
		0x00000400, 
		0x00ffffff, 
		0xc2700000, 
		0x80800001, 
		0x007ffffe, 
		0x83df4673, 
		0xff800000, 
		0x7f95b014, 
		0xc2680000, 
		0x5b031129, 
		0x9a9dcbe5, 
		0x80011111, 
		0x00000001, 
		0x80080000, 
		0xbf800001, 
		0x80800001, 
		0x0c7fc000, 
		0xb804db73, 
		0x80000000, 
		0xc2680000, 
		0x007fffff, 
		0x7f7ffffe, 
		0x00000000, 
		0x005db7e6, 
		0x0e00ffff, 
		0xff7ffffe, 
		0xbdfdbabe, 
		0xed4d2bba, 
		0x0e0003ff, 
		0x80000040, 
		0xdf0ef9b7, 
		0x42400000, 
		0x00000000, 
		0x00800003, 
		0x00000000, 
		0x7fb2cc4b, 
		0x6b79d7f7, 
		0x8dbc298f, 
		0x423c0000, 
		0xc2380000, 
		0xc1b00000, 
		0xc23c0000, 
		0xfac8eae7, 
		0x0e07ffff, 
		0x00000000, 
		0x45eaaf37, 
		0x74fea297, 
		0x0c7fffff, 
		0x80000001, 
		0x0c7ffffc, 
		0xf56540c5, 
		0xc2240000, 
		0xff800000, 
		0x80800001, 
		0x00000000, 
		0xc2540000, 
		0xb328c9a3, 
		0x42180000, 
		0xc1200000, 
		0xc2780000, 
		0x6fb6ad22, 
		0xc1400000, 
		0x0c600000, 
		0x41400000, 
		0x55555555, 
		0xa6ad723c, 
		0x80800000, 
		0x42240000, 
		0xbf800001, 
		0x00000000, 
		0xc0e00000, 
		0x426c0000, 
		0x49e3d6be, 
		0xbf028f5c, 
		0x80800003, 
		0x41e80000, 
		0x7f800000, 
		0x55555555, 
		0x7fbfffff, 
		0xc2780000, 
		0x16db9ce3, 
		0x0f7ffffd, 
		0xbea84e37, 
		0xc8d48614, 
		0xc55b30f5, 
		0x95e213d9, 
		0x7f800000, 
		0x00800000, 
		0x80000200, 
		0x80008000, 
		0x00800001, 
		0x00ffffff, 
		0x0c7ff800, 
		0x42680000, 
		0xc1d80000, 
		0x0f7ffffc, 
		0x41880000, 
		0x7fc00000, 
		0x40a00000, 
		0xddfb2de9, 
		0x0c7fc000, 
		0xc2740000, 
		0x41980000, 
		0xc1c80000, 
		0x8a8d9a8a, 
		0xc2280000, 
		0x7f800000, 
		0x55014b8d, 
		0x00000000, 
		0x0e3fffff, 
		0x1e37f411, 
		0x41f80000, 
		0x80000000, 
		0x80000001, 
		0x0f7fffff, 
		0x3eb920b3, 
		0x00800001, 
		0x80000000, 
		0x00000000, 
		0xc752c02a, 
		0x3f800001, 
		0x0c7e0000, 
		0x0e07ffff, 
		0xc1d80000, 
		0xff7fffff, 
		0x807fffff, 
		0xf46d1c76, 
		0xc2180000, 
		0xff800000, 
		0x00000400, 
		0x42480000, 
		0x00800000, 
		0x40000000, 
		0x42bef3f4, 
		0x427c0000, 
		0xffba5356, 
		0xd87c6a56, 
		0x42640000, 
		0xc1700000, 
		0x7f800000, 
		0x63fa811c, 
		0x422c0000, 
		0x80000040, 
		0x64a5cafd, 
		0x5dfbe811, 
		0x80000010, 
		0x41f80000, 
		0xafde4312, 
		0xe0a38ac2, 
		0x00002000, 
		0x7fa6e17e, 
		0x00800002, 
		0x0c7ffff8, 
		0x00000100, 
		0x80040000, 
		0x93f54984, 
		0xc1b00000, 
		0xf2eaeea4, 
		0x0c7ffff0, 
		0xc1200000, 
		0x00800002, 
		0xd444cd5d, 
		0x7f000000, 
		0x80800002, 
		0xc1900000, 
		0x0e000003, 
		0xfb994692, 
		0xff800000, 
		0xb63ff07a, 
		0x80000004, 
		0x5cf55994, 
		0x80200000, 
		0x0c7fffff, 
		0x00000000, 
		0x98b2ec0a, 
		0x0fc91557, 
		0xc2180000, 
		0x42100000, 
		0xc29db506, 
		0x00002000, 
		0x0c7fffc0, 
		0x4c43fd32, 
		0xc0a00000, 
		0x6c46aad4, 
		0xffb446c1, 
		0x41d00000, 
		0x42400000, 
		0x0e001fff, 
		0x4b000000, 
		0x0c9acca6, 
		0xbf028f5c, 
		0x00800000, 
		0x80000000, 
		0x0c7fffc0, 
		0x42480000, 
		0x7fc00000, 
		0x00400000, 
		0x0c7fe000, 
		0xfdce0e3c, 
		0x4c54dcdd, 
		0xffb4c707, 
		0xbd691494, 
		0x00000000, 
		0x00000400, 
		0xc0c00000, 
		0x80000000, 
		0x7f000000, 
		0xe2a87a43, 
		0x40a00000, 
		0x1d720252, 
		0x909b8f43, 
		0x80000001, 
		0x0e001fff, 
		0x80000000, 
		0xff800000, 
		0x80800002, 
		0x8039b9fb, 
		0x0e00ffff, 
		0xe5f95a94, 
		0x42500000, 
		0x41a80000, 
		0x00000001, 
		0x3131ef55, 
		0x7e106a5f, 
		0x00100000, 
		0x42500000, 
		0xd6b303d9, 
		0xff800000, 
		0xe08e1e9b, 
		0x0c7c0000, 
		0x182112a0, 
		0x80000000, 
		0x41b00000, 
		0x41d00000, 
		0xfff3368e, 
		0x69969a2e, 
		0x00317413, 
		0x807fffff, 
		0xc1b80000, 
		0x0e0000ff, 
		0xc340c134, 
		0x00200000, 
		0x7f800000, 
		0x5d90b772, 
		0x80000000, 
		0x328b9b06, 
		0x0e3fffff, 
		0x0c7ff800, 
		0xc0c00000, 
		0x0e0001ff, 
		0x80000200, 
		0x42780000, 
		0x9fd0111e, 
		0x421c0000, 
		0x80040000, 
		0xc2500000, 
		0xff99f008, 
		0x0c7c0000, 
		0xc1d00000, 
		0x7fbfe931, 
		0x00000001, 
		0x0c7fe000, 
		0xff800000, 
		0x00400000, 
		0x8c436a73, 
		0x00800000, 
		0x1ef6960a, 
		0x425c0000, 
		0xc2740000, 
		0x4d032702, 
		0x00000000, 
		0x40e00000, 
		0x0e00001f, 
		0x00800000, 
		0x40000000, 
		0x41980000, 
		0x0e0000ff, 
		0xc1200000, 
		0x8f7ffffc, 
		0xc2140000, 
		0x42140000, 
		0xc2540000, 
		0x7f8350bf, 
		0x41800000, 
		0xe886c320, 
		0x00000040, 
		0x21e53958, 
		0x7fc00001, 
		0x699eaa1e, 
		0x00000400, 
		0x00000002, 
		0x2b0b1952, 
		0x7f7ffffe, 
		0x7fc00000, 
		0xc1700000, 
		0x42440000, 
		0x41900000, 
		0x42780000, 
		0x0fb9936c, 
		0x00400000, 
		0x42380000, 
		0xe56249c9, 
		0x9f787926, 
		0xc1500000, 
		0x3a6807a6, 
		0xc25c0000, 
		0x7f7ffffe, 
		0x80004000, 
		0x0e000003, 
		0x80000040, 
		0x0e1fffff, 
		0xc2000000, 
		0x6f06b41e, 
		0x80000008, 
		0x7f800000, 
		0xaaaaaaaa, 
		0x7fa7e6e4, 
		0x7f800000, 
		0x41e00000, 
		0x4ca5bfed, 
		0x8dbc906a, 
		0xbe49342b, 
		0x40000000, 
		0xd879f1b7, 
		0xa3802bfb, 
		0x80008000, 
		0x80080000, 
		0x1aee86ad, 
		0xd246c0f1, 
		0x41a80000, 
		0xc2480000, 
		0x42180000, 
		0x0d000ff0, 
		0xff7fffff, 
		0xff800000, 
		0x7c78d973, 
		0xc1a80000, 
		0x8305c8b1, 
		0x80002000, 
		0x17797b61, 
		0xc2680000, 
		0xc0c00000, 
		0x42140000, 
		0x0e003fff, 
		0x80000000, 
		0x00000001, 
		0x80000010, 
		0x0c7ffe00, 
		0xff800000, 
		0x421c0000, 
		0xc0800000, 
		0x80011111, 
		0x00000000, 
		0x7f8098e5, 
		0x59055480, 
		0xd669c38d, 
		0x80080000, 
		0xc2340000, 
		0xc1700000, 
		0x7f800000, 
		0x42600000, 
		0x7a46688e, 
		0x80000000, 
		0x80800002, 
		0xc20c0000, 
		0x0026578b, 
		0x00ffffff, 
		0x41400000, 
		0x80000000, 
		0xc2100000, 
		0x420c0000, 
		0x0074329b, 
		0x0c7ffe00, 
		0xc2540000, 
		0x00800000, 
		0x80542703, 
		0x42580000, 
		0xffba4538, 
		0x00000000, 
		0x0e3fffff, 
		0xc1700000, 
		0x80800000, 
		0x00000000, 
		0x00000004, 
		0xc2380000, 
		0x2510b7a7, 
		0x00000000, 
		0x1236db6e, 
		0x7f800000, 
		0x475436ce, 
		0x41d80000, 
		0x0fc27585, 
		0x5214e3e2, 
		0x80000001, 
		0x41b00000, 
		0x80000800, 
		0x0e001fff, 
		0x00722030, 
		0xff7fffff, 
		0x80000800, 
		0x5c1ac882, 
		0x5f79ab28, 
		0x3f028f5c, 
		0x80000000, 
		0xff800000, 
		0x80000800, 
		0x0e0003ff, 
		0x5087fcdc, 
		0xc1f00000, 
		0x60ee7bfe, 
		0xb1e3fdc9, 
		0x2380e267, 
		0x00000000, 
		0xbf9ce006, 
		0xc9e27a9f, 
		0x1d2405f8, 
		0x0f7ffffd, 
		0x00011111, 
		0x876b09a9, 
		0x00020000, 
		0x0c780000, 
		0x2d0148a6, 
		0xe0afd43a, 
		0x0e00007f, 
		0x41900000, 
		0x6426c049, 
		0xffae0684, 
		0x0e1fffff, 
		0xc1980000, 
		0x00000000, 
		0x41100000, 
		0x00800000, 
		0x1fce157e, 
		0xffd88066, 
		0x40000000, 
		0xff800000, 
		0x3f800000, 
		0x42000000, 
		0xc1b80000, 
		0xff000000, 
		0xff800000, 
		0xff800000, 
		0x151d567f, 
		0x00020000, 
		0x0e1fffff, 
		0x7f7ffffe, 
		0x00011111, 
		0x759b4079, 
		0xaaaaaaaa, 
		0x80000000, 
		0x00000000, 
		0xe5dd5983, 
		0x41600000, 
		0x0c7ffff8, 
		0x41b00000, 
		0x105041f4, 
		0x41d80000, 
		0x0c600000, 
		0x807fffff, 
		0x94ea3f6c, 
		0x42480000, 
		0x00004000, 
		0x950ef7fa, 
		0x80001000, 
		0x80200000, 
		0x6af1f2ae, 
		0xc1000000, 
		0x80800002, 
		0x41d80000, 
		0x00002000, 
		0x80800000, 
		0x80000000, 
		0x70af513b, 
		0xc2740000, 
		0x761884e1, 
		0x7f7fffff, 
		0x0c7fe000, 
		0x00800003, 
		0x42580000, 
		0x0e007fff, 
		0x00000002, 
		0x80000000, 
		0x20182fff, 
		0x82e081ff, 
		0x0e7fffff, 
		0xbfec5b79, 
		0xff800001, 
		0x7f800000, 
		0xc2080000, 
		0x42540000, 
		0xcc3fb67f, 
		0xb5709894, 
		0x4fd95031, 
		0xc1a00000, 
		0x41c00000, 
		0x0e0003ff, 
		0x00002000, 
		0xc2380000, 
		0xc1b00000, 
		0x00000008, 
		0xc0e00000, 
		0x00800002, 
		0x42280000, 
		0x41400000, 
		0x0e03ffff, 
		0x80800000, 
		0x6dc37a3f, 
		0x0e00ffff, 
		0x0e000fff, 
		0xaf03f333, 
		0x7fc00001, 
		0x422c0000, 
		0x0e0000ff, 
		0xc2380000, 
		0xde58239e, 
		0xe946cf35, 
		0xc1600000, 
		0x80800000, 
		0xff800000, 
		0x00800000, 
		0x0c7fc000, 
		0xba93d3a6, 
		0x80800002, 
		0xc2600000, 
		0x00000100, 
		0xc2140000, 
		0x0d000ff0, 
		0x7f83a4ab, 
		0xc21c0000, 
		0x18df234a, 
		0x11206085, 
		0x426c0000, 
		0x257da550, 
		0xd7f8cc66, 
		0xe5446f67, 
		0x00800002, 
		0xb0e77648, 
		0x80010000, 
		0x00000000, 
		0x42340000, 
		0x9ee512d3, 
		0x0c7ffc00, 
		0x41800000, 
		0xc2100000, 
		0x09bf80ab, 
		0xcb8c4b40, 
		0x4b000000, 
		0x00000000, 
		0x00000008, 
		0xf6c7bad4, 
		0xc2700000, 
		0x4a0f5f2a, 
		0x83ee10c8, 
		0x6806b6d0, 
		0x0c600000, 
		0x0c7ffc00, 
		0x80000000, 
		0xc1900000, 
		0x8d5a157d, 
		0x0f35f62a, 
		0x80000800, 
		0xc64f5f0d, 
		0x0268d147, 
		0x41e00000, 
		0xc1e00000, 
		0x00800003, 
		0x0f7ffffe, 
		0x3b9f318f, 
		0x80000000, 
		0x33698638, 
		0x80000000, 
		0x1f52866a, 
		0xe209e1fc, 
		0x80000008, 
		0x013c459d, 
		0xc2740000, 
		0x80000000, 
		0xc2480000, 
		0x164534f5, 
		0xff7ffffe, 
		0x0e0000ff, 
		0x80100000, 
		0x42480000, 
		0xa8869921, 
		0x54508338, 
		0xbf028f5c, 
		0xc2600000, 
		0xad04322a, 
		0x3d1c88b6, 
		0x7f800000, 
		0x80080000, 
		0x4b8c4b40, 
		0x7fc00000, 
		0xff7ffffe, 
		0x42700000, 
		0xe47ec2a9, 
		0x80000000, 
		0x1ee9f8f7, 
		0x00800000, 
		0x331e5ece, 
		0x41a80000, 
		0x00000000, 
		0x40400000, 
		0x007fffff, 
		0xffb6fcf1, 
		0x007ffffe, 
		0x422c0000, 
		0x80000001, 
		0x443d7b20, 
		0xc2700000, 
		0x80000001, 
		0x80000001, 
		0xb4dc4109, 
		0x00000100, 
		0x0c7ffc00, 
		0x0e007fff, 
		0x395a3a28, 
		0xdf0c122e, 
		0x41c00000, 
		0x42240000, 
		0x80000200, 
		0xc2540000, 
		0xa874bf3d, 
		0x7f800000, 
		0xff7ffffe, 
		0xc1f00000, 
		0x80000000, 
		0x7f000000, 
		0xfe57eb82, 
		0x00000040, 
		0xc2200000, 
		0x37357a96, 
		0x199f2868, 
		0x0e07ffff, 
		0x0c7fff00, 
		0x80800000, 
		0x80020000, 
		0xca10c21f, 
		0x4cc9ebaf, 
		0x530ff02b, 
		0x9fd65283, 
		0x3000dbb4, 
		0x865c696a, 
		0xfac29b6e, 
		0x7f7fffff, 
		0x7030ff2e, 
		0x0f7ffffe, 
		0x1fa1761d, 
		0xd63763ce, 
		0xcdb4495e, 
		0xb17f0c29, 
		0x80800001, 
		0x0e0000ff, 
		0x00000800, 
		0x6e549309, 
		0x7f800000, 
		0x0e00007f, 
		0x40800000, 
		0xd493088f, 
		0xff7ffffe, 
		0xc2280000, 
		0x019c0d7e, 
		0x5ec2500d, 
		0x817b86db, 
		0x46b44a9e, 
		0xc1600000, 
		0x0f7ffffd, 
		0x8d4f5257, 
		0x80000000, 
		0x0e0000ff, 
		0xff800000, 
		0x7f800000, 
		0xff2f981d, 
		0xbf800000, 
		0x0c7ffffe, 
		0xa0770ebb, 
		0x00011111, 
		0x00800000, 
		0xdae4b408, 
		0x0f7ffffd, 
		0x722ce735, 
		0xe3b83ab7, 
		0x807ffffe, 
		0x7fbfffff, 
		0xc24c0000, 
		0x7f800000, 
		0xe2243a9b, 
		0x0bc6985e, 
		0x00000000, 
		0x0115aa27, 
		0x42600000, 
		0x42080000, 
		0x00000001, 
		0xa7cf059f, 
		0x00800002, 
		0xc2740000, 
		0xc1c00000, 
		0x80002000, 
		0x00000000, 
		0x0e0001ff, 
		0xd40e48a2, 
		0x40400000, 
		0x00000200, 
		0x8061d064, 
		0xc2380000, 
		0xc2480000, 
		0x41000000, 
		0x8c1b9b5e, 
		0x41d00000, 
		0x0c7ffc00, 
		0x00001000, 
		0x80200000, 
		0xff800000, 
		0x422c0000, 
		0x807ffffe, 
		0x0c780000, 
		0x42700000, 
		0x80200000, 
		0x0e0001ff, 
		0x80000010, 
		0x40800000, 
		0xbbbc5afc, 
		0x80800001, 
		0x41200000, 
		0x80000800, 
		0x3f800001, 
		0xe18bcb6a, 
		0x00800000, 
		0x00200000, 
		0x427c0000, 
		0x80000000, 
		0x42580000, 
		0x00129122, 
		0x41e00000, 
		0xc1a80000, 
		0x42600000, 
		0x80000000, 
		0x05672b1c, 
		0x7f16f49b, 
		0xf0eb6b9c, 
		0xff956c5e, 
		0xc2500000, 
		0x7f800000, 
		0x86e28f4a, 
		0x80800003, 
		0x7fc00001, 
		0xa0e174b2, 
		0x55555555, 
		0x0d000ff0, 
		0x0e003fff, 
		0x0e0fffff, 
		0x41b00000, 
		0xd7437bda, 
		0x33333333, 
		0x0e000003, 
		0xaebf84f3, 
		0x55555555, 
		0xff800000, 
		0x40e00000, 
		0x7fc00000, 
		0x42380000, 
		0x80800000, 
		0x41a00000, 
		0x00800001, 
		0x00010000, 
		0x80000008, 
		0x00000000, 
		0x0f7fffff, 
		0x41300000, 
		0xc1c00000, 
		0x0e1fffff, 
		0x7fc00000, 
		0x40800000, 
		0x0d00fff0, 
		0x7f94b6eb, 
		0xffc00001, 
		0xcaedc669, 
		0x0e0fffff, 
		0x967460a6, 
		0x42680000, 
		0xc2080000, 
		0x41800000, 
		0xc1b00000, 
		0xffc00001, 
		0xc2240000, 
		0xc1d80000, 
		0xff7ffffe, 
		0xff800000, 
		0x1ea4ad5d, 
		0x80557feb, 
		0xc286d96b, 
		0x00000000, 
		0x539a244f, 
		0x80020000, 
		0xc7c3c75b, 
		0x0025e211, 
		0x0e0fffff, 
		0x00800000, 
		0x007ffffe, 
		0xc0ee5d66, 
		0x80000000, 
		0x00800001, 
		0xc2300000, 
		0x00000000, 
		0x00800001, 
		0x00000020, 
		0x0f7fffff, 
		0xc1f80000, 
		0x0979d882, 
		0x807fffff, 
		0x80ffffff, 
		0x3f800001, 
		0xc2240000, 
		0x80000001, 
		0x80010000, 
		0xc2740000, 
		0x0e03ffff, 
		0x8f7ffffc, 
		0x00000000, 
		0x0f7fffff, 
		0x80000000, 
		0xedd62ea1, 
		0x41f80000, 
		0x141597cb, 
		0x42540000, 
		0x00000000, 
		0xf5de3411, 
		0xc2680000, 
		0x00400000, 
		0x00000080, 
		0xc1800000, 
		0x42680000, 
		0x42680000, 
		0xab063200, 
		0x00000008, 
		0x006d1255, 
		0x00200000, 
		0x55555555, 
		0x0c7ffff0, 
		0x7fae231c, 
		0x01c92a21, 
		0x91e2d3a8, 
		0x2ecfe1af, 
		0x42000000, 
		0x7f7ffffe, 
		0x41f00000, 
		0x00ffffff, 
		0xe4b4b095, 
		0x8f7ffffc, 
		0xdc3c2828, 
		0xc23c0000, 
		0xe7566c7f, 
		0x7f800000, 
		0x0e001fff, 
		0xff800000, 
		0x93f39246, 
		0xffffffff, 
		0xfd38d5d1, 
		0x41100000, 
		0xc0e00000, 
		0xe609d275, 
		0x41100000, 
		0x6a985fcb, 
		0xff800000, 
		0x13d8b950, 
		0x00000000, 
		0x422c0000, 
		0x5247ca9d, 
		0x0e000003, 
		0xff800001, 
		0x0d000ff0, 
		0x00000000, 
		0x8304875c, 
		0xff800000, 
		0x424c0000, 
		0x42500000, 
		0xfdcf29da, 
		0x80000400, 
		0x80800000, 
		0x7f800000, 
		0x408a2229, 
		0xc23c0000, 
		0xc22c0000, 
		0x42540000, 
		0x3b21942a, 
		0x00000001, 
		0xa2fb9155, 
		0xff7fffff, 
		0x42640000, 
		0x80800001, 
		0x7fc00001, 
		0x00800003, 
		0xffbfffff, 
		0xc1000000, 
		0x00800001, 
		0x7f800000, 
		0x609b733d, 
		0x41d80000, 
		0xff800000, 
		0x0e01ffff, 
		0x80800001, 
		0x877260b5, 
		0x42500000, 
		0xff800000, 
		0xff9086c4, 
		0x00000080, 
		0xc22c0000, 
		0xab9cbbbf, 
		0x7fd11c21, 
		0xc2389566, 
		0x8f7fffff, 
		0x80020000, 
		0xc9a7b826, 
		0x0c7e0000, 
		0x6de311cf, 
		0x7dcccfd1, 
		0x425c0000, 
		0x423c0000, 
		0x2c551dea, 
		0xc2140000, 
		0x0e0fffff, 
		0xbee42a11, 
		0x80000000, 
		0x5306fa13, 
		0x1fe9464e, 
		0x941b4f33, 
		0x80800001, 
		0x80000000, 
		0x75f57121, 
		0x0e3fffff, 
		0xc21c0000, 
		0xc0e00000, 
		0x369f03e8, 
		0x887736aa, 
		0x0f7ffffe, 
		0x0e007fff, 
		0x0c600000, 
		0x80011111, 
		0x80000001, 
		0x0c7fffe0, 
		0x7f800000, 
		0xa5a9b3ec, 
		0x0c7fc000, 
		0x7272edef, 
		0xbc54288e, 
		0x41e80000, 
		0xffc00000, 
		0x00000002, 
		0xc2540000, 
		0x80800001, 
		0x00002000, 
		0x00000000, 
		0x00000004, 
		0x0e03ffff, 
		0x1a619f9f, 
		0x0c7fff80, 
		0xe4ff8337, 
		0x00000000, 
		0x0e0003ff, 
		0x80004000, 
		0x80000000, 
		0x40000000, 
		0x40e00000, 
		0xec4ac57e, 
		0xc1d00000, 
		0x802632d8, 
		0x00000000, 
		0x42700000, 
		0xc1400000, 
		0x7f800000, 
		0x0e000007, 
		0xffbfffff, 
		0x42680000, 
		0x00000000, 
		0x59aa79e9, 
		0x41a00000, 
		0xa0acd38e, 
		0x00000000, 
		0x41200000, 
		0x0e001fff, 
		0x0f7fffff, 
		0xc27c0000, 
		0x00ffffff, 
		0xff7ffffe, 
		0x41a80000, 
		0x80000000, 
		0x80000000, 
		0xcf14721f, 
		0x41c00000, 
		0x7f7ffffe, 
		0x1d882799, 
		0xb39fe1c2, 
		0x424c0000, 
		0x8f7ffffd, 
		0x007ffffe, 
		0xc2140000, 
		0x2752d015, 
		0x886c7608, 
		0x807fffff, 
		0xffbfffff, 
		0xc26c0000, 
		0xc2200000, 
		0x66966854, 
		0x41400000, 
		0xd49a563a, 
		0xe76c8582, 
		0x00400000, 
		0xc2640000, 
		0x00800003, 
		0xc7e92041, 
		0x80800001, 
		0x51e6d01b, 
		0x42300000, 
		0xff7ffffe, 
		0x0e0003ff, 
		0x80000002, 
		0x7f7ffffe, 
		0x80000002, 
		0x46c332ca, 
		0x698f3527, 
		0x0c7ffe00, 
		0x7fc00001, 
		0x0c7ffff8, 
		0x0c7ffc00, 
		0xff800001, 
		0x807ffffe, 
		0x66ad240d, 
		0x7fa58e38, 
		0xfe402e62, 
		0x346eb71b, 
		0x04984d7a, 
		0x80800001, 
		0x00080000, 
		0x49b8be36, 
		0xffa769f7, 
		0xe55b8cac, 
		0x7f7fffff, 
		0xc0c00000, 
		0x7f7ffffe, 
		0xc1200000, 
		0x60231084, 
		0xe778ddae, 
		0x04efc1b0, 
		0xffc00000, 
		0x96791b04, 
		0xc1d00000, 
		0x900232fc, 
		0xbff74db8, 
		0x427e0061, 
		0x42040000, 
		0x0c7fff00, 
		0x0c700000, 
		0x803a11f8, 
		0x41000000, 
		0x58c6d74e, 
		0x80800001, 
		0x00000002, 
		0x80000004, 
		0x00011111, 
		0x94e837fd, 
		0x80000000, 
		0x80800001, 
		0xffa2ee4e, 
		0xbe8e471d, 
		0xc1f00000, 
		0x420c0000, 
		0xea4fc27f, 
		0x80800003, 
		0x00000000, 
		0x00000001, 
		0x43f12f87, 
		0x00000001, 
		0xb4777d51, 
		0x00000004, 
		0x80000080, 
		0x403de30f, 
		0x42540000, 
		0x4b000000, 
		0x421c0000, 
		0x8958ca2c, 
		0x360592f7, 
		0x80000002, 
		0xc0400000, 
		0xc1a80000, 
		0x42340000, 
		0x006dc30d, 
		0x8e73611f, 
		0x00000000, 
		0xff83b075, 
		0x7f7ffffe, 
		0xc1c80000, 
		0xc1d00000, 
		0x55555555, 
		0x9d629d38, 
		0x0c7ffffc, 
		0x8f7fffff, 
		0x41900000, 
		0xc23c0000, 
		0xf7ef1d17, 
		0x80080000, 
		0xc2440000, 
		0x0e07ffff, 
		0x80800000, 
		0x00001000, 
		0xc2680000, 
		0x80800001, 
		0x80001000, 
		0x55555555, 
		0x3c55bcf7, 
		0xcf6cd3f3, 
		0x00000200, 
		0x7f800000, 
		0x98a44c81, 
		0xc1c00000, 
		0x007fffff, 
		0xc1000000, 
		0x7143547a, 
		0xc6bc2310, 
		0x67d710ca, 
		0x41e80000, 
		0x8cce61c4, 
		0x41600000, 
		0x00800002, 
		0x00800002, 
		0x41c80000, 
		0x41980000, 
		0xcccccccc, 
		0x5edff2e4, 
		0x0e001fff, 
		0x7f800000, 
		0xb7937e10, 
		0x00002000, 
		0xc1a00000, 
		0x5103dee4, 
		0x7f800000, 
		0xfad68590, 
		0x41700000, 
		0x41f80000, 
		0xfafbd110, 
		0xdb634cc8, 
		0xae95e394, 
		0x80200000, 
		0xffbfffff, 
		0x41f00000, 
		0xc1500000, 
		0x423c0000, 
		0xab55ccbd, 
		0xa445fe0f, 
		0x00000000, 
		0x82cb4c67, 
		0x0e000fff, 
		0x1ce8711c, 
		0xc1d00000, 
		0x80000000, 
		0x423c0000, 
		0xc0e00000, 
		0x7f7ffffe, 
		0x00000010, 
		0x6b3d06d3, 
		0x0e000007, 
		0xc1980000, 
		0x0c400000, 
		0x0c7f8000, 
		0xc20c0000, 
		0xff800000, 
		0xffbfffff  
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

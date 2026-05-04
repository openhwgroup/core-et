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
			"csrwi mhpmevent3, 22\n"   
			"csrwi mhpmevent4, 18\n"   
			"csrwi mhpmevent5, 15\n"   
			"csrwi mhpmevent6, 8\n"   
			"csrwi mhpmevent7, 7\n"   
			"csrwi mhpmevent8, 5\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f16,  32 (x5)\n"
			"flw.ps f3,  320 (x5)\n"
			"flw.ps f24,  960 (x5)\n"
			"flw.ps f25,  384 (x5)\n"
			"flw.ps f12,  832 (x5)\n"
			"flw.ps f29,  64 (x5)\n"
			"flw.ps f8,  512 (x5)\n"
			"flw.ps f9,  896 (x5)\n"
			"flw.ps f2,  640 (x5)\n"
			"flw.ps f14,  864 (x5)\n"
			"flw.ps f27,  736 (x5)\n"
			"flw.ps f1,  224 (x5)\n"
			"flw.ps f17,  96 (x5)\n"
			"flw.ps f10,  608 (x5)\n"
			"flw.ps f23,  672 (x5)\n"
			"flw.ps f13,  800 (x5)\n"
			"flw.ps f15,  768 (x5)\n"
			"flw.ps f20,  256 (x5)\n"
			"flw.ps f22,  928 (x5)\n"
			"flw.ps f21,  480 (x5)\n"
			"flw.ps f18,  416 (x5)\n"
			"flw.ps f0,  0 (x5)\n"
			"flw.ps f5,  544 (x5)\n"
			"flw.ps f31,  992 (x5)\n"
			"flw.ps f19,  448 (x5)\n"
			"flw.ps f26,  352 (x5)\n"
			"flw.ps f6,  288 (x5)\n"
			"flw.ps f11,  160 (x5)\n"
			"flw.ps f7,  192 (x5)\n"
			"flw.ps f30,  128 (x5)\n"
			"flw.ps f28,  704 (x5)\n"
			"flw.ps f4,  576 (x5)\n"
			"LBL_VADDR_INIT_HID_0:\n"          
			"li x16, 0x81001e2700 \n"                 
			"fsw.ps f19, (x16) \n"                 
			"li x12, 0x810041f020 \n"                 
			"fsw.ps f18, (x12) \n"                 
			"li x17, 0x81004e06c0 \n"                 
			"fsw.ps f12, (x17) \n"                 
			"li x10, 0x8101140760 \n"                 
			"fsw.ps f1, (x10) \n"                 
			"li x18, 0x81013fa600 \n"                 
			"fsw.ps f12, (x18) \n"                 
			"li x20, 0x8102040000 \n"                 
			"fsw.ps f12, (x20) \n"                 
			"li x13, 0x8102210020 \n"                 
			"fsw.ps f15, (x13) \n"                 
			"li x14, 0x81025d1020 \n"                 
			"fsw.ps f18, (x14) \n"                 
			"li x21, 0x8102ae01c0 \n"                 
			"fsw.ps f13, (x21) \n"                 
			"li x21, 0x8102d60000 \n"                 
			"fsw.ps f17, (x21) \n"                 
			"li x23, 0x8102fa0020 \n"                 
			"fsw.ps f27, (x23) \n"                 
			"li x10, 0x81030a0000 \n"                 
			"fsw.ps f23, (x10) \n"                 
			"li x13, 0x81031032a0 \n"                 
			"fsw.ps f24, (x13) \n"                 
			"li x17, 0x8103660020 \n"                 
			"fsw.ps f21, (x17) \n"                 
			"li x25, 0x8103b7a540 \n"                 
			"fsw.ps f14, (x25) \n"                 
			"li x20, 0x8103e0a820 \n"                 
			"fsw.ps f9, (x20) \n"                 
			"li x23, 0x8104d9f820 \n"                 
			"fsw.ps f20, (x23) \n"                 
			"li x11, 0x8104deb9e0 \n"                 
			"fsw.ps f21, (x11) \n"                 
			"li x18, 0x8104efba40 \n"                 
			"fsw.ps f10, (x18) \n"                 
			"li x22, 0x81050e0000 \n"                 
			"fsw.ps f28, (x22) \n"                 
			"li x16, 0x8106a006a0 \n"                 
			"fsw.ps f8, (x16) \n"                 
			"li x17, 0x8106a7a000 \n"                 
			"fsw.ps f22, (x17) \n"                 
			"li x24, 0x8106da0520 \n"                 
			"fsw.ps f1, (x24) \n"                 
			"li x17, 0x8107140720 \n"                 
			"fsw.ps f3, (x17) \n"                 
			"li x22, 0x810722c820 \n"                 
			"fsw.ps f29, (x22) \n"                 
			"li x22, 0x8107240620 \n"                 
			"fsw.ps f26, (x22) \n"                 
			"li x20, 0x8107321020 \n"                 
			"fsw.ps f16, (x20) \n"                 
			"li x15, 0x810779b820 \n"                 
			"fsw.ps f3, (x15) \n"                 
			"li x21, 0x8107a605a0 \n"                 
			"fsw.ps f21, (x21) \n"                 
			"li x19, 0x8107ee0020 \n"                 
			"fsw.ps f10, (x19) \n"                 
			"li x10, 0x81083db800 \n"                 
			"fsw.ps f4, (x10) \n"                 
			"li x20, 0x8108740020 \n"                 
			"fsw.ps f13, (x20) \n"                 
			"li x12, 0x81088c0420 \n"                 
			"fsw.ps f2, (x12) \n"                 
			"li x25, 0x8108d60000 \n"                 
			"fsw.ps f12, (x25) \n"                 
			"li x14, 0x8108e11ae0 \n"                 
			"fsw.ps f31, (x14) \n"                 
			"li x10, 0x8108ed2000 \n"                 
			"fsw.ps f16, (x10) \n"                 
			"li x24, 0x81095207c0 \n"                 
			"fsw.ps f30, (x24) \n"                 
			"li x24, 0x81096098e0 \n"                 
			"fsw.ps f31, (x24) \n"                 
			"li x20, 0x8109712920 \n"                 
			"fsw.ps f12, (x20) \n"                 
			"li x15, 0x810a3d6160 \n"                 
			"fsw.ps f25, (x15) \n"                 
			"li x10, 0x810b052800 \n"                 
			"fsw.ps f10, (x10) \n"                 
			"li x23, 0x810b240020 \n"                 
			"fsw.ps f4, (x23) \n"                 
			"li x12, 0x810b7c0020 \n"                 
			"fsw.ps f8, (x12) \n"                 
			"li x25, 0x810b860900 \n"                 
			"fsw.ps f9, (x25) \n"                 
			"li x14, 0x810b8a02e0 \n"                 
			"fsw.ps f28, (x14) \n"                 
			"li x12, 0x810b8c0000 \n"                 
			"fsw.ps f18, (x12) \n"                 
			"li x24, 0x810b9c6020 \n"                 
			"fsw.ps f10, (x24) \n"                 
			"li x16, 0x810bab1740 \n"                 
			"fsw.ps f23, (x16) \n"                 
			"li x24, 0x810c4b6000 \n"                 
			"fsw.ps f20, (x24) \n"                 
			"li x21, 0x810c7c8ce0 \n"                 
			"fsw.ps f25, (x21) \n"                 
			"li x19, 0x810c8e0000 \n"                 
			"fsw.ps f15, (x19) \n"                 
			"li x11, 0x810cd2fe20 \n"                 
			"fsw.ps f14, (x11) \n"                 
			"li x25, 0x810cfa0000 \n"                 
			"fsw.ps f11, (x25) \n"                 
			"li x11, 0x810cfd1020 \n"                 
			"fsw.ps f2, (x11) \n"                 
			"li x25, 0x810d34d020 \n"                 
			"fsw.ps f9, (x25) \n"                 
			"li x24, 0x810d48e000 \n"                 
			"fsw.ps f3, (x24) \n"                 
			"li x16, 0x810d7000e0 \n"                 
			"fsw.ps f27, (x16) \n"                 
			"li x23, 0x810dae06e0 \n"                 
			"fsw.ps f3, (x23) \n"                 
			"li x23, 0x810de20020 \n"                 
			"fsw.ps f9, (x23) \n"                 
			"li x18, 0x810e5dd820 \n"                 
			"fsw.ps f30, (x18) \n"                 
			"li x14, 0x810e97f320 \n"                 
			"fsw.ps f3, (x14) \n"                 
			"li x24, 0x810eb66580 \n"                 
			"fsw.ps f18, (x24) \n"                 
			"li x21, 0x810ebe0040 \n"                 
			"fsw.ps f24, (x21) \n"                 
			"li x21, 0x810ed50500 \n"                 
			"fsw.ps f22, (x21) \n"                 
			"li x25, 0x810efa0300 \n"                 
			"fsw.ps f16, (x25) \n"                 
			"li x25, 0x810f1a0000 \n"                 
			"fsw.ps f22, (x25) \n"                 
			"li x22, 0x810f40e020 \n"                 
			"fsw.ps f23, (x22) \n"                 
			"li x25, 0x810f840000 \n"                 
			"fsw.ps f13, (x25) \n"                 
			"li x24, 0x810fb36000 \n"                 
			"fsw.ps f22, (x24) \n"                 
			"li x25, 0x810fed3ae0 \n"                 
			"fsw.ps f16, (x25) \n"                 
			"li x15, 0x820007de60 \n"                 
			"fsw.ps f23, (x15) \n"                 
			"li x21, 0x8200300020 \n"                 
			"fsw.ps f4, (x21) \n"                 
			"li x14, 0x8200380500 \n"                 
			"fsw.ps f10, (x14) \n"                 
			"li x23, 0x8200460020 \n"                 
			"fsw.ps f19, (x23) \n"                 
			"li x15, 0x8200ddb460 \n"                 
			"fsw.ps f24, (x15) \n"                 
			"li x13, 0x820112f680 \n"                 
			"fsw.ps f31, (x13) \n"                 
			"li x15, 0x820145d020 \n"                 
			"fsw.ps f25, (x15) \n"                 
			"li x18, 0x82014b3020 \n"                 
			"fsw.ps f28, (x18) \n"                 
			"li x21, 0x820171efe0 \n"                 
			"fsw.ps f3, (x21) \n"                 
			"li x23, 0x8201a60020 \n"                 
			"fsw.ps f7, (x23) \n"                 
			"li x10, 0x8201c4bb60 \n"                 
			"fsw.ps f7, (x10) \n"                 
			"li x23, 0x8202a06100 \n"                 
			"fsw.ps f12, (x23) \n"                 
			"li x21, 0x8202a60000 \n"                 
			"fsw.ps f0, (x21) \n"                 
			"li x15, 0x8202da0560 \n"                 
			"fsw.ps f16, (x15) \n"                 
			"li x11, 0x8202f6a000 \n"                 
			"fsw.ps f22, (x11) \n"                 
			"li x24, 0x8203ce0020 \n"                 
			"fsw.ps f25, (x24) \n"                 
			"li x17, 0x8203ea0020 \n"                 
			"fsw.ps f9, (x17) \n"                 
			"li x22, 0x8203ef2fe0 \n"                 
			"fsw.ps f27, (x22) \n"                 
			"li x24, 0x820410a680 \n"                 
			"fsw.ps f13, (x24) \n"                 
			"li x17, 0x82042afb20 \n"                 
			"fsw.ps f12, (x17) \n"                 
			"li x10, 0x8204323140 \n"                 
			"fsw.ps f10, (x10) \n"                 
			"li x25, 0x8204580020 \n"                 
			"fsw.ps f13, (x25) \n"                 
			"li x17, 0x8204606060 \n"                 
			"fsw.ps f25, (x17) \n"                 
			"li x10, 0x8204720620 \n"                 
			"fsw.ps f10, (x10) \n"                 
			"li x16, 0x8204e7b000 \n"                 
			"fsw.ps f5, (x16) \n"                 
			"li x19, 0x8204f93800 \n"                 
			"fsw.ps f9, (x19) \n"                 
			"li x12, 0x8205e14d00 \n"                 
			"fsw.ps f31, (x12) \n"                 
			"li x23, 0x8206568820 \n"                 
			"fsw.ps f16, (x23) \n"                 
			"li x25, 0x82065a04e0 \n"                 
			"fsw.ps f9, (x25) \n"                 
			"li x13, 0x8206fe0280 \n"                 
			"fsw.ps f22, (x13) \n"                 
			"li x18, 0x8207b24000 \n"                 
			"fsw.ps f0, (x18) \n"                 
			"li x20, 0x8207b536e0 \n"                 
			"fsw.ps f2, (x20) \n"                 
			"li x23, 0x820816b020 \n"                 
			"fsw.ps f29, (x23) \n"                 
			"li x11, 0x82088e05c0 \n"                 
			"fsw.ps f22, (x11) \n"                 
			"li x16, 0x8208d1b820 \n"                 
			"fsw.ps f23, (x16) \n"                 
			"li x22, 0x820922dde0 \n"                 
			"fsw.ps f30, (x22) \n"                 
			"li x14, 0x8209698800 \n"                 
			"fsw.ps f21, (x14) \n"                 
			"li x17, 0x8209c024c0 \n"                 
			"fsw.ps f31, (x17) \n"                 
			"li x10, 0x8209d1e820 \n"                 
			"fsw.ps f8, (x10) \n"                 
			"li x10, 0x820a320320 \n"                 
			"fsw.ps f3, (x10) \n"                 
			"li x25, 0x820a960000 \n"                 
			"fsw.ps f2, (x25) \n"                 
			"li x24, 0x820aca0000 \n"                 
			"fsw.ps f15, (x24) \n"                 
			"li x17, 0x820acfb000 \n"                 
			"fsw.ps f19, (x17) \n"                 
			"li x21, 0x820ade0120 \n"                 
			"fsw.ps f17, (x21) \n"                 
			"li x18, 0x820b3a04a0 \n"                 
			"fsw.ps f20, (x18) \n"                 
			"li x21, 0x820b8c0140 \n"                 
			"fsw.ps f26, (x21) \n"                 
			"li x12, 0x820c08cb60 \n"                 
			"fsw.ps f13, (x12) \n"                 
			"li x13, 0x820c27e300 \n"                 
			"fsw.ps f2, (x13) \n"                 
			"li x13, 0x820c7e0020 \n"                 
			"fsw.ps f13, (x13) \n"                 
			"li x10, 0x820c9e0660 \n"                 
			"fsw.ps f29, (x10) \n"                 
			"li x24, 0x820d1602e0 \n"                 
			"fsw.ps f23, (x24) \n"                 
			"li x17, 0x820d740360 \n"                 
			"fsw.ps f4, (x17) \n"                 
			"li x10, 0x820d7e0000 \n"                 
			"fsw.ps f29, (x10) \n"                 
			"li x12, 0x820d8407c0 \n"                 
			"fsw.ps f31, (x12) \n"                 
			"li x25, 0x820dae0000 \n"                 
			"fsw.ps f13, (x25) \n"                 
			"li x22, 0x820e020340 \n"                 
			"fsw.ps f9, (x22) \n"                 
			"li x21, 0x820e290e20 \n"                 
			"fsw.ps f8, (x21) \n"                 
			"li x17, 0x820ec61020 \n"                 
			"fsw.ps f31, (x17) \n"                 
			"li x10, 0x820ee00760 \n"                 
			"fsw.ps f28, (x10) \n"                 
			"li x10, 0x820ee80280 \n"                 
			"fsw.ps f2, (x10) \n"                 
			"li x13, 0x820efc2020 \n"                 
			"fsw.ps f23, (x13) \n"                 
			"li x13, 0x820f21a640 \n"                 
			"fsw.ps f11, (x13) \n"                 
			"li x20, 0x820f3f8820 \n"                 
			"fsw.ps f0, (x20) \n"                 
			"li x22, 0x820fd20000 \n"                 
			"fsw.ps f7, (x22) \n"                 
			"fence\n"
			"LBL_GLOBAL_VADDR_INIT_HID_0:\n"          
			"li x25, 0x81201e2700 \n"                 
			"fswg.ps f14, (x25) \n"                 
			"li x25, 0x812041f020 \n"                 
			"fswg.ps f6, (x25) \n"                 
			"li x17, 0x8121140760 \n"                 
			"fswg.ps f31, (x17) \n"                 
			"li x21, 0x81213fa600 \n"                 
			"fswg.ps f26, (x21) \n"                 
			"li x17, 0x8121512420 \n"                 
			"fswg.ps f6, (x17) \n"                 
			"li x14, 0x8122ae01c0 \n"                 
			"fswg.ps f7, (x14) \n"                 
			"li x16, 0x81230a0000 \n"                 
			"fswg.ps f29, (x16) \n"                 
			"li x25, 0x8123660020 \n"                 
			"fswg.ps f10, (x25) \n"                 
			"li x17, 0x8124060260 \n"                 
			"fswg.ps f4, (x17) \n"                 
			"li x17, 0x8124deb9e0 \n"                 
			"fswg.ps f27, (x17) \n"                 
			"li x10, 0x8124efba40 \n"                 
			"fswg.ps f23, (x10) \n"                 
			"li x13, 0x81257c0020 \n"                 
			"fswg.ps f14, (x13) \n"                 
			"li x15, 0x8125ba0020 \n"                 
			"fswg.ps f14, (x15) \n"                 
			"li x10, 0x8126200020 \n"                 
			"fswg.ps f30, (x10) \n"                 
			"li x14, 0x8126a7a000 \n"                 
			"fswg.ps f9, (x14) \n"                 
			"li x21, 0x81278e04e0 \n"                 
			"fswg.ps f29, (x21) \n"                 
			"li x20, 0x8128580020 \n"                 
			"fswg.ps f26, (x20) \n"                 
			"li x17, 0x8128740020 \n"                 
			"fswg.ps f24, (x17) \n"                 
			"li x22, 0x8128d60000 \n"                 
			"fswg.ps f1, (x22) \n"                 
			"li x19, 0x8129709020 \n"                 
			"fswg.ps f3, (x19) \n"                 
			"li x22, 0x812a3d6160 \n"                 
			"fswg.ps f27, (x22) \n"                 
			"li x12, 0x812ae40020 \n"                 
			"fswg.ps f31, (x12) \n"                 
			"li x10, 0x812b34e300 \n"                 
			"fswg.ps f25, (x10) \n"                 
			"li x21, 0x812b8a02e0 \n"                 
			"fswg.ps f5, (x21) \n"                 
			"li x18, 0x812bc80100 \n"                 
			"fswg.ps f18, (x18) \n"                 
			"li x18, 0x812c6c0020 \n"                 
			"fswg.ps f25, (x18) \n"                 
			"li x12, 0x812c8e0000 \n"                 
			"fswg.ps f6, (x12) \n"                 
			"li x20, 0x812d34d020 \n"                 
			"fswg.ps f4, (x20) \n"                 
			"li x23, 0x812dae06e0 \n"                 
			"fswg.ps f10, (x23) \n"                 
			"li x12, 0x812e5dd820 \n"                 
			"fswg.ps f30, (x12) \n"                 
			"li x14, 0x812efa0300 \n"                 
			"fswg.ps f11, (x14) \n"                 
			"li x24, 0x812f1a0000 \n"                 
			"fswg.ps f9, (x24) \n"                 
			"li x21, 0x812fec06c0 \n"                 
			"fswg.ps f25, (x21) \n"                 
			"li x17, 0x8130a40100 \n"                 
			"fswg.ps f9, (x17) \n"                 
			"li x25, 0x8131120320 \n"                 
			"fswg.ps f3, (x25) \n"                 
			"li x23, 0x8132210020 \n"                 
			"fswg.ps f5, (x23) \n"                 
			"li x18, 0x81323e0000 \n"                 
			"fswg.ps f6, (x18) \n"                 
			"li x25, 0x81325d1020 \n"                 
			"fswg.ps f11, (x25) \n"                 
			"li x20, 0x8132e20440 \n"                 
			"fswg.ps f17, (x20) \n"                 
			"li x20, 0x8132fa0020 \n"                 
			"fswg.ps f30, (x20) \n"                 
			"li x21, 0x81338b0800 \n"                 
			"fswg.ps f29, (x21) \n"                 
			"li x11, 0x81341ec020 \n"                 
			"fswg.ps f25, (x11) \n"                 
			"li x25, 0x813546e800 \n"                 
			"fswg.ps f20, (x25) \n"                 
			"li x13, 0x8135680680 \n"                 
			"fswg.ps f25, (x13) \n"                 
			"li x11, 0x8135b60020 \n"                 
			"fswg.ps f10, (x11) \n"                 
			"li x19, 0x8135be76c0 \n"                 
			"fswg.ps f23, (x19) \n"                 
			"li x23, 0x8136a006a0 \n"                 
			"fswg.ps f14, (x23) \n"                 
			"li x14, 0x8136da0520 \n"                 
			"fswg.ps f21, (x14) \n"                 
			"li x17, 0x8136e201c0 \n"                 
			"fswg.ps f25, (x17) \n"                 
			"li x11, 0x8137140720 \n"                 
			"fswg.ps f15, (x11) \n"                 
			"li x17, 0x813722c820 \n"                 
			"fswg.ps f18, (x17) \n"                 
			"li x22, 0x813779b820 \n"                 
			"fswg.ps f12, (x22) \n"                 
			"li x11, 0x8137edc020 \n"                 
			"fswg.ps f29, (x11) \n"                 
			"li x21, 0x81388c0420 \n"                 
			"fswg.ps f26, (x21) \n"                 
			"li x15, 0x8138e11ae0 \n"                 
			"fswg.ps f2, (x15) \n"                 
			"li x21, 0x8138ed2000 \n"                 
			"fswg.ps f30, (x21) \n"                 
			"li x24, 0x81395207c0 \n"                 
			"fswg.ps f31, (x24) \n"                 
			"li x17, 0x81396098e0 \n"                 
			"fswg.ps f5, (x17) \n"                 
			"li x16, 0x8139712920 \n"                 
			"fswg.ps f9, (x16) \n"                 
			"li x18, 0x813b8c0000 \n"                 
			"fswg.ps f24, (x18) \n"                 
			"li x24, 0x813c7c8ce0 \n"                 
			"fswg.ps f4, (x24) \n"                 
			"li x22, 0x813cd2fe20 \n"                 
			"fswg.ps f13, (x22) \n"                 
			"li x14, 0x813cfd1020 \n"                 
			"fswg.ps f29, (x14) \n"                 
			"li x25, 0x813de20020 \n"                 
			"fswg.ps f16, (x25) \n"                 
			"li x13, 0x813eb66580 \n"                 
			"fswg.ps f1, (x13) \n"                 
			"li x15, 0x813fb36000 \n"                 
			"fswg.ps f11, (x15) \n"                 
			"li x21, 0x813fed3ae0 \n"                 
			"fswg.ps f0, (x21) \n"                 
			"li x10, 0x8220300020 \n"                 
			"fswg.ps f22, (x10) \n"                 
			"li x13, 0x822145d020 \n"                 
			"fswg.ps f1, (x13) \n"                 
			"li x11, 0x82228a0100 \n"                 
			"fswg.ps f21, (x11) \n"                 
			"li x18, 0x8222a60000 \n"                 
			"fswg.ps f20, (x18) \n"                 
			"li x24, 0x8223bc0320 \n"                 
			"fswg.ps f4, (x24) \n"                 
			"li x21, 0x82242afb20 \n"                 
			"fswg.ps f10, (x21) \n"                 
			"li x19, 0x822449e000 \n"                 
			"fswg.ps f18, (x19) \n"                 
			"li x17, 0x8224580020 \n"                 
			"fswg.ps f25, (x17) \n"                 
			"li x23, 0x8224e7b000 \n"                 
			"fswg.ps f12, (x23) \n"                 
			"li x17, 0x8224f93800 \n"                 
			"fswg.ps f9, (x17) \n"                 
			"li x21, 0x8226dc0660 \n"                 
			"fswg.ps f9, (x21) \n"                 
			"li x19, 0x8226fe0280 \n"                 
			"fswg.ps f19, (x19) \n"                 
			"li x10, 0x82281004c0 \n"                 
			"fswg.ps f22, (x10) \n"                 
			"li x10, 0x82288e05c0 \n"                 
			"fswg.ps f28, (x10) \n"                 
			"li x18, 0x8228d1b820 \n"                 
			"fswg.ps f5, (x18) \n"                 
			"li x15, 0x822922dde0 \n"                 
			"fswg.ps f13, (x15) \n"                 
			"li x23, 0x8229698800 \n"                 
			"fswg.ps f3, (x23) \n"                 
			"li x15, 0x8229c024c0 \n"                 
			"fswg.ps f15, (x15) \n"                 
			"li x12, 0x8229d1e820 \n"                 
			"fswg.ps f4, (x12) \n"                 
			"li x22, 0x822a320320 \n"                 
			"fswg.ps f26, (x22) \n"                 
			"li x25, 0x822abcd800 \n"                 
			"fswg.ps f27, (x25) \n"                 
			"li x22, 0x822aca0000 \n"                 
			"fswg.ps f9, (x22) \n"                 
			"li x17, 0x822c08cb60 \n"                 
			"fswg.ps f4, (x17) \n"                 
			"li x14, 0x822c9e0660 \n"                 
			"fswg.ps f25, (x14) \n"                 
			"li x23, 0x822d141820 \n"                 
			"fswg.ps f24, (x23) \n"                 
			"li x16, 0x822d7e0000 \n"                 
			"fswg.ps f17, (x16) \n"                 
			"li x14, 0x822d8407c0 \n"                 
			"fswg.ps f12, (x14) \n"                 
			"li x17, 0x822d9e0320 \n"                 
			"fswg.ps f17, (x17) \n"                 
			"li x24, 0x822e020340 \n"                 
			"fswg.ps f30, (x24) \n"                 
			"li x22, 0x822e220000 \n"                 
			"fswg.ps f8, (x22) \n"                 
			"li x17, 0x822ee80280 \n"                 
			"fswg.ps f6, (x17) \n"                 
			"li x18, 0x8230380500 \n"                 
			"fswg.ps f28, (x18) \n"                 
			"li x24, 0x8230820000 \n"                 
			"fswg.ps f20, (x24) \n"                 
			"li x20, 0x82314b3020 \n"                 
			"fswg.ps f21, (x20) \n"                 
			"li x24, 0x8231c4bb60 \n"                 
			"fswg.ps f6, (x24) \n"                 
			"li x20, 0x8232a06100 \n"                 
			"fswg.ps f7, (x20) \n"                 
			"li x16, 0x8232da0560 \n"                 
			"fswg.ps f7, (x16) \n"                 
			"li x18, 0x8232f6a000 \n"                 
			"fswg.ps f0, (x18) \n"                 
			"li x16, 0x8233600220 \n"                 
			"fswg.ps f12, (x16) \n"                 
			"li x23, 0x8233ce0020 \n"                 
			"fswg.ps f31, (x23) \n"                 
			"li x18, 0x8233ea0020 \n"                 
			"fswg.ps f0, (x18) \n"                 
			"li x15, 0x823401f2a0 \n"                 
			"fswg.ps f23, (x15) \n"                 
			"li x22, 0x8234323140 \n"                 
			"fswg.ps f17, (x22) \n"                 
			"li x25, 0x8234460000 \n"                 
			"fswg.ps f14, (x25) \n"                 
			"li x20, 0x8234606060 \n"                 
			"fswg.ps f1, (x20) \n"                 
			"li x17, 0x8234720620 \n"                 
			"fswg.ps f30, (x17) \n"                 
			"li x15, 0x8235020020 \n"                 
			"fswg.ps f18, (x15) \n"                 
			"li x11, 0x823566b000 \n"                 
			"fswg.ps f14, (x11) \n"                 
			"li x14, 0x82358bd740 \n"                 
			"fswg.ps f21, (x14) \n"                 
			"li x17, 0x8235e14d00 \n"                 
			"fswg.ps f12, (x17) \n"                 
			"li x16, 0x82365a04e0 \n"                 
			"fswg.ps f5, (x16) \n"                 
			"li x20, 0x8237b536e0 \n"                 
			"fswg.ps f27, (x20) \n"                 
			"li x12, 0x823816b020 \n"                 
			"fswg.ps f23, (x12) \n"                 
			"li x18, 0x8238ea0000 \n"                 
			"fswg.ps f1, (x18) \n"                 
			"li x20, 0x8239c20020 \n"                 
			"fswg.ps f27, (x20) \n"                 
			"li x16, 0x823a320020 \n"                 
			"fswg.ps f9, (x16) \n"                 
			"li x25, 0x823ade0120 \n"                 
			"fswg.ps f14, (x25) \n"                 
			"li x20, 0x823af00000 \n"                 
			"fswg.ps f27, (x20) \n"                 
			"li x21, 0x823b8c0140 \n"                 
			"fswg.ps f10, (x21) \n"                 
			"li x22, 0x823c7e0020 \n"                 
			"fswg.ps f29, (x22) \n"                 
			"li x16, 0x823d1602e0 \n"                 
			"fswg.ps f23, (x16) \n"                 
			"li x15, 0x823d740360 \n"                 
			"fswg.ps f0, (x15) \n"                 
			"li x16, 0x823e12f6c0 \n"                 
			"fswg.ps f19, (x16) \n"                 
			"li x16, 0x823ec61020 \n"                 
			"fswg.ps f25, (x16) \n"                 
			"li x15, 0x823ee00760 \n"                 
			"fswg.ps f0, (x15) \n"                 
			"li x10, 0x823f320000 \n"                 
			"fswg.ps f3, (x10) \n"                 
			"li x10, 0x823f3f8820 \n"                 
			"fswg.ps f3, (x10) \n"                 
			"fence\n"
			"LBL_LOCAL_VADDR_INIT_HID_0:\n"          
			"li x22, 0x81401e2700 \n"                 
			"fswl.ps f24, (x22) \n"                 
			"li x21, 0x8140a40100 \n"                 
			"fswl.ps f25, (x21) \n"                 
			"li x21, 0x8141140760 \n"                 
			"fswl.ps f25, (x21) \n"                 
			"li x22, 0x81423e0000 \n"                 
			"fswl.ps f13, (x22) \n"                 
			"li x23, 0x81431032a0 \n"                 
			"fswl.ps f0, (x23) \n"                 
			"li x10, 0x8143660020 \n"                 
			"fswl.ps f28, (x10) \n"                 
			"li x23, 0x8143b7a540 \n"                 
			"fswl.ps f16, (x23) \n"                 
			"li x11, 0x8144fd2820 \n"                 
			"fswl.ps f16, (x11) \n"                 
			"li x21, 0x81457c0020 \n"                 
			"fswl.ps f9, (x21) \n"                 
			"li x11, 0x81469ee000 \n"                 
			"fswl.ps f25, (x11) \n"                 
			"li x21, 0x8146a006a0 \n"                 
			"fswl.ps f15, (x21) \n"                 
			"li x24, 0x8146da0520 \n"                 
			"fswl.ps f13, (x24) \n"                 
			"li x20, 0x814722c820 \n"                 
			"fswl.ps f15, (x20) \n"                 
			"li x15, 0x8147321020 \n"                 
			"fswl.ps f16, (x15) \n"                 
			"li x23, 0x8147740020 \n"                 
			"fswl.ps f5, (x23) \n"                 
			"li x13, 0x81478e04e0 \n"                 
			"fswl.ps f12, (x13) \n"                 
			"li x19, 0x81483db800 \n"                 
			"fswl.ps f10, (x19) \n"                 
			"li x24, 0x81488c0420 \n"                 
			"fswl.ps f23, (x24) \n"                 
			"li x14, 0x8148e11ae0 \n"                 
			"fswl.ps f5, (x14) \n"                 
			"li x14, 0x81495207c0 \n"                 
			"fswl.ps f23, (x14) \n"                 
			"li x15, 0x81496098e0 \n"                 
			"fswl.ps f6, (x15) \n"                 
			"li x19, 0x814977a300 \n"                 
			"fswl.ps f10, (x19) \n"                 
			"li x11, 0x814b052800 \n"                 
			"fswl.ps f30, (x11) \n"                 
			"li x21, 0x814b240020 \n"                 
			"fswl.ps f10, (x21) \n"                 
			"li x12, 0x814b860900 \n"                 
			"fswl.ps f22, (x12) \n"                 
			"li x13, 0x814c7c8ce0 \n"                 
			"fswl.ps f26, (x13) \n"                 
			"li x11, 0x814d34d020 \n"                 
			"fswl.ps f4, (x11) \n"                 
			"li x18, 0x814de20020 \n"                 
			"fswl.ps f20, (x18) \n"                 
			"li x16, 0x814f1a0000 \n"                 
			"fswl.ps f17, (x16) \n"                 
			"li x23, 0x814fb36000 \n"                 
			"fswl.ps f12, (x23) \n"                 
			"li x21, 0x815041f020 \n"                 
			"fswl.ps f23, (x21) \n"                 
			"li x24, 0x81507e0160 \n"                 
			"fswl.ps f9, (x24) \n"                 
			"li x17, 0x81513fa600 \n"                 
			"fswl.ps f18, (x17) \n"                 
			"li x11, 0x8151512420 \n"                 
			"fswl.ps f31, (x11) \n"                 
			"li x10, 0x81519fd2c0 \n"                 
			"fswl.ps f24, (x10) \n"                 
			"li x20, 0x8152040000 \n"                 
			"fswl.ps f24, (x20) \n"                 
			"li x20, 0x81525d1020 \n"                 
			"fswl.ps f2, (x20) \n"                 
			"li x19, 0x8152ae01c0 \n"                 
			"fswl.ps f15, (x19) \n"                 
			"li x10, 0x8152d60000 \n"                 
			"fswl.ps f24, (x10) \n"                 
			"li x24, 0x81539c14a0 \n"                 
			"fswl.ps f16, (x24) \n"                 
			"li x10, 0x8153e0a820 \n"                 
			"fswl.ps f23, (x10) \n"                 
			"li x25, 0x8154060260 \n"                 
			"fswl.ps f31, (x25) \n"                 
			"li x17, 0x81541ec020 \n"                 
			"fswl.ps f11, (x17) \n"                 
			"li x19, 0x8154d9f820 \n"                 
			"fswl.ps f22, (x19) \n"                 
			"li x11, 0x8155ba0020 \n"                 
			"fswl.ps f26, (x11) \n"                 
			"li x22, 0x8155be76c0 \n"                 
			"fswl.ps f16, (x22) \n"                 
			"li x13, 0x8156200020 \n"                 
			"fswl.ps f12, (x13) \n"                 
			"li x18, 0x8156a7a000 \n"                 
			"fswl.ps f15, (x18) \n"                 
			"li x23, 0x815779b820 \n"                 
			"fswl.ps f0, (x23) \n"                 
			"li x11, 0x8157dd1ce0 \n"                 
			"fswl.ps f21, (x11) \n"                 
			"li x22, 0x8157edc020 \n"                 
			"fswl.ps f8, (x22) \n"                 
			"li x25, 0x8157ee0020 \n"                 
			"fswl.ps f25, (x25) \n"                 
			"li x11, 0x8158d60000 \n"                 
			"fswl.ps f8, (x11) \n"                 
			"li x13, 0x8158ed2000 \n"                 
			"fswl.ps f17, (x13) \n"                 
			"li x10, 0x8159709020 \n"                 
			"fswl.ps f6, (x10) \n"                 
			"li x25, 0x815a3d6160 \n"                 
			"fswl.ps f27, (x25) \n"                 
			"li x24, 0x815a832900 \n"                 
			"fswl.ps f31, (x24) \n"                 
			"li x15, 0x815ae40020 \n"                 
			"fswl.ps f16, (x15) \n"                 
			"li x21, 0x815b34e300 \n"                 
			"fswl.ps f0, (x21) \n"                 
			"li x14, 0x815b83d800 \n"                 
			"fswl.ps f27, (x14) \n"                 
			"li x20, 0x815b8a02e0 \n"                 
			"fswl.ps f7, (x20) \n"                 
			"li x18, 0x815b8c0000 \n"                 
			"fswl.ps f1, (x18) \n"                 
			"li x19, 0x815b9c6020 \n"                 
			"fswl.ps f14, (x19) \n"                 
			"li x23, 0x815bab1740 \n"                 
			"fswl.ps f17, (x23) \n"                 
			"li x10, 0x815bc80100 \n"                 
			"fswl.ps f2, (x10) \n"                 
			"li x18, 0x815c6c0020 \n"                 
			"fswl.ps f5, (x18) \n"                 
			"li x21, 0x815db1a800 \n"                 
			"fswl.ps f6, (x21) \n"                 
			"li x17, 0x815e97f320 \n"                 
			"fswl.ps f9, (x17) \n"                 
			"li x22, 0x815eb66580 \n"                 
			"fswl.ps f7, (x22) \n"                 
			"li x17, 0x815efa0300 \n"                 
			"fswl.ps f23, (x17) \n"                 
			"li x23, 0x815f840000 \n"                 
			"fswl.ps f0, (x23) \n"                 
			"li x11, 0x815fd48800 \n"                 
			"fswl.ps f16, (x11) \n"                 
			"li x15, 0x815fed3ae0 \n"                 
			"fswl.ps f31, (x15) \n"                 
			"li x14, 0x82414b3020 \n"                 
			"fswl.ps f8, (x14) \n"                 
			"li x14, 0x8241c4bb60 \n"                 
			"fswl.ps f19, (x14) \n"                 
			"li x14, 0x82428a0100 \n"                 
			"fswl.ps f26, (x14) \n"                 
			"li x15, 0x8242a06100 \n"                 
			"fswl.ps f27, (x15) \n"                 
			"li x22, 0x8243600220 \n"                 
			"fswl.ps f3, (x22) \n"                 
			"li x17, 0x8243ef2fe0 \n"                 
			"fswl.ps f28, (x17) \n"                 
			"li x20, 0x824401f2a0 \n"                 
			"fswl.ps f17, (x20) \n"                 
			"li x21, 0x824410a680 \n"                 
			"fswl.ps f24, (x21) \n"                 
			"li x16, 0x8244323140 \n"                 
			"fswl.ps f26, (x16) \n"                 
			"li x19, 0x8244580020 \n"                 
			"fswl.ps f18, (x19) \n"                 
			"li x10, 0x82460bf9e0 \n"                 
			"fswl.ps f5, (x10) \n"                 
			"li x11, 0x8246568820 \n"                 
			"fswl.ps f4, (x11) \n"                 
			"li x19, 0x8246fe0280 \n"                 
			"fswl.ps f22, (x19) \n"                 
			"li x22, 0x8247b24000 \n"                 
			"fswl.ps f5, (x22) \n"                 
			"li x17, 0x82488e05c0 \n"                 
			"fswl.ps f27, (x17) \n"                 
			"li x14, 0x8248ea0000 \n"                 
			"fswl.ps f21, (x14) \n"                 
			"li x24, 0x8249698800 \n"                 
			"fswl.ps f24, (x24) \n"                 
			"li x19, 0x8249c20020 \n"                 
			"fswl.ps f9, (x19) \n"                 
			"li x19, 0x824a320020 \n"                 
			"fswl.ps f26, (x19) \n"                 
			"li x22, 0x824a320320 \n"                 
			"fswl.ps f6, (x22) \n"                 
			"li x21, 0x824a960000 \n"                 
			"fswl.ps f12, (x21) \n"                 
			"li x13, 0x824aa002e0 \n"                 
			"fswl.ps f8, (x13) \n"                 
			"li x10, 0x824aca0000 \n"                 
			"fswl.ps f18, (x10) \n"                 
			"li x22, 0x824acfb000 \n"                 
			"fswl.ps f13, (x22) \n"                 
			"li x25, 0x824af00000 \n"                 
			"fswl.ps f3, (x25) \n"                 
			"li x18, 0x824c7e0020 \n"                 
			"fswl.ps f16, (x18) \n"                 
			"li x18, 0x824d7e0000 \n"                 
			"fswl.ps f7, (x18) \n"                 
			"li x21, 0x824dae0000 \n"                 
			"fswl.ps f13, (x21) \n"                 
			"li x24, 0x824e020340 \n"                 
			"fswl.ps f23, (x24) \n"                 
			"li x20, 0x824e220000 \n"                 
			"fswl.ps f26, (x20) \n"                 
			"li x21, 0x824e290e20 \n"                 
			"fswl.ps f27, (x21) \n"                 
			"li x14, 0x824ee80280 \n"                 
			"fswl.ps f6, (x14) \n"                 
			"li x21, 0x824f320000 \n"                 
			"fswl.ps f24, (x21) \n"                 
			"li x12, 0x824f3f8820 \n"                 
			"fswl.ps f1, (x12) \n"                 
			"li x24, 0x824fd20000 \n"                 
			"fswl.ps f2, (x24) \n"                 
			"li x20, 0x8250300020 \n"                 
			"fswl.ps f21, (x20) \n"                 
			"li x13, 0x8250380500 \n"                 
			"fswl.ps f17, (x13) \n"                 
			"li x19, 0x8250460020 \n"                 
			"fswl.ps f21, (x19) \n"                 
			"li x22, 0x8250820000 \n"                 
			"fswl.ps f0, (x22) \n"                 
			"li x16, 0x825171efe0 \n"                 
			"fswl.ps f4, (x16) \n"                 
			"li x12, 0x8252a60000 \n"                 
			"fswl.ps f1, (x12) \n"                 
			"li x18, 0x8252f6a000 \n"                 
			"fswl.ps f18, (x18) \n"                 
			"li x23, 0x8253bc0320 \n"                 
			"fswl.ps f8, (x23) \n"                 
			"li x20, 0x8253ea0020 \n"                 
			"fswl.ps f24, (x20) \n"                 
			"li x14, 0x8254460000 \n"                 
			"fswl.ps f22, (x14) \n"                 
			"li x13, 0x8254606060 \n"                 
			"fswl.ps f20, (x13) \n"                 
			"li x18, 0x8254720620 \n"                 
			"fswl.ps f29, (x18) \n"                 
			"li x23, 0x8254f93800 \n"                 
			"fswl.ps f0, (x23) \n"                 
			"li x21, 0x8255020020 \n"                 
			"fswl.ps f3, (x21) \n"                 
			"li x15, 0x82558bd740 \n"                 
			"fswl.ps f18, (x15) \n"                 
			"li x21, 0x825591aa20 \n"                 
			"fswl.ps f23, (x21) \n"                 
			"li x11, 0x825816b020 \n"                 
			"fswl.ps f22, (x11) \n"                 
			"li x25, 0x8258cb5820 \n"                 
			"fswl.ps f8, (x25) \n"                 
			"li x22, 0x825922dde0 \n"                 
			"fswl.ps f31, (x22) \n"                 
			"li x19, 0x825c27e300 \n"                 
			"fswl.ps f2, (x19) \n"                 
			"li x11, 0x825c9e0660 \n"                 
			"fswl.ps f2, (x11) \n"                 
			"li x13, 0x825d141820 \n"                 
			"fswl.ps f24, (x13) \n"                 
			"li x14, 0x825d8407c0 \n"                 
			"fswl.ps f23, (x14) \n"                 
			"li x11, 0x825ec61020 \n"                 
			"fswl.ps f12, (x11) \n"                 
			"li x14, 0x825efc2020 \n"                 
			"fswl.ps f30, (x14) \n"                 
			"li x10, 0x825f21a640 \n"                 
			"fswl.ps f0, (x10) \n"                 
			"fence\n"
			"fence\n"
			"csrw fflags, zero\n"
			"csrwi frm, 0x0\n"
			"LBL_KERNEL_HID_0:\n"          
			"li x5, 0xFEEDFEED\n"
			"li x0, 0x5633ba99ea\n" 
			"slti x0, x0, 0x7f2\n"
			"la x31, check_sum_hid_0\n"
			"li x5, 0x6\n" 
			"sw x5, (x31)\n"
			"li x29, 0x6\n"
			"LBL_OUTER_LOOP_SEQ_HID_0:\n"
			"nop \n"
			"nop \n"
			"li x30, 0\n"
			"addi x30, x30, 1\n"
			"li x5, 0xd5cf52017171f7fc\n"
			"mulhu x17, x5, x5\n" 
			"slli x18, x12, 1\n"
			"fminu.pi f25, f1, f21\n"
			"bge x31, x11, 1f\n"
			"1:\n"
			"slli x22, x14, 4\n"
			"faddi.pi f30, f21, 0x80\n"
			"fandi.pi f8, f20, 0xb2\n"
			"fpackrepb.pi f6, f21\n"
			"blt x11, x22, 1f\n"
			"1:\n"
			"fpackrepb.pi f10, f21\n"
			"and x0, x14, x12\n"
			"sraiw x6, x26, 3\n"
			"la x27, rand_ieee754_32\n"
			"flw.ps f19, 288(x27)\n"
			"flw.ps f13, 352(x27)\n"
			"fmul.ps f8, f19, f13, rdn\n" 
			"fltm.ps m0, f29, f15\n"
			"mov.m.x m0, x0, 0xff\n"
			"fltm.ps m4, f22, f7\n"
			"fadd.s f1, f11, f9, rmm\n"
			"fle.pi f2, f0, f3\n"
			"slli x7, x27, 0\n"
			"fle.pi f26, f6, f30\n"
			"addi x9, x28, 0x7\n"
			"masknot m4, m7\n"
			"fsll.pi f14, f13, f14\n"
			"blt x6, x1, 1f\n"
			"1:\n"
			"li x6, 0x27541261d4a1ee31\n"
			"addiw x27, x6, 0x10\n" 
			"feq.ps f9, f26, f8\n"
			"addiw x7, x9, 0x4\n"
			"beq x25, x15, 1f\n"
			"1:\n"
			"fclass.s x26, f6\n"
			"fclass.s x20, f21\n"
			"fmv.x.s x20, f0\n"
			"beq x16, x0, 1f\n"
			"1:\n"
			"sllw x6, x5, x26\n"
			"fsrl.pi f7, f24, f24\n"
			"or x18, x25, x5\n"
			"sllw x26, x26, x26\n"
			"la x19, rand_int_32\n"
			"flw.ps f0, 672(x19)\n"
			"fcvt.ps.pw f9, f0, rne\n" 
			"mulhu x16, x19, x16\n"
			"fpackrepb.pi f23, f7\n"
			"addw x26, x10, x5\n"
			"or x9, x10, x19\n"
			"fnot.pi f11, f12\n"
			"fadd.s f5, f12, f0, rmm\n"
			"packb x20, x26, x16\n"
			"fexp.ps f17, f0\n"
			"beq x27, x5, 1f\n"
			"1:\n"
			"fsrl.pi f13, f6, f25\n"
			"la x9, rand_int_32\n"
			"flw.ps f18, 256(x9)\n"
			"fandi.pi f25, f18, 0xd0\n" 
			"faddi.pi f23, f26, 0x39\n"
			"addw x11, x28, x9\n"
			"fle.ps f23, f7, f7\n"
			"blt x19, x27, 1f\n"
			"1:\n"
			"fmsub.s f19, f23, f26, f7, rdn\n"
			"fmulhu.pi f15, f30, f30\n"
			"blt x18, x4, 1f\n"
			"1:\n"
			"add x5, x0, x5\n"
			"bltu x26, x20, 1f\n"
			"1:\n"
			"flem.ps m2, f27, f23\n"
			"blt x19, x20, 1f\n"
			"1:\n"
			"fmsub.s f8, f9, f27, f7, rdn\n"
			"srlw x14, x5, x12\n"
			"la x16, rand_int_32\n"
			"flw.ps f7, 352(x16)\n"
			"fnot.pi f11, f7\n" 
			"fsgnjx.s f10, f8, f12\n"
			"fmv.x.s x12, f14\n"
			"bltu x19, x29, 1f\n"
			"1:\n"
			"maskpopcz x20, m5\n"
			"bge x2, x6, 1f\n"
			"1:\n"
			"subw x16, x23, x16\n"
			"fmsub.s f12, f12, f4, f1, rdn\n"
			"or x28, x11, x10\n"
			"bne x14, x31, 1f\n"
			"1:\n"
			"fadd.ps f21, f15, f22, rdn\n"
			"subw x20, x6, x15\n"
			"fle.pi f10, f22, f21\n"
			"bne x20, x11, 1f\n"
			"1:\n"
			"la x19, rand_ieee754_32\n"
			"flw.ps f27, 672(x19)\n"
			"flw.ps f20, 352(x19)\n"
			"fle.s x16, f27, f20\n" 
			"fsll.pi f11, f1, f13\n"
			"fsll.pi f1, f26, f31\n"
			"slliw x19, x17, 0\n"
			"slti x7, x11, 0x1c\n"
			"mulhu x5, x18, x17\n"
			"slti x28, x25, 0x10\n"
			"fcvt.ps.f16 f3, f28\n"
			"fmax.pi f5, f29, f12\n"
			"bge x4, x14, 1f\n"
			"1:\n"
			"sll x12, x17, x11\n"
			"li x23, 0xaab93317a552b1f4\n"
			"li x14, 0x8a31cd60b5e6f6eb\n"
			"subw x27, x23, x14\n" 
			"sraiw x17, x26, 5\n"
			"fexp.ps f6, f5\n"
			"fmulhu.pi f12, f5, f5\n"
			"fbci.pi f28, 0x15a79\n"
			"fmsub.s f5, f5, f5, f20, rmm\n"
			"fadd.ps f14, f6, f17, rup\n"
			"fsrai.pi f12, f17, 0x2\n"
			"xori x9, x10, 0x17\n"
			"fsgnjx.s f5, f5, f17\n"
			"li x6, 0x5bfd1a8828f4d59d\n"
			"sraiw x12, x6, 3\n" 
			"fmax.pi f6, f31, f24\n"
			"fle.ps f17, f4, f17\n"
			"fle.pi f23, f23, f18\n"
			"fmax.pi f31, f17, f1\n"
			"fminu.pi f30, f7, f1\n"
			"fle.ps f3, f18, f15\n"
			"fsll.pi f19, f22, f15\n"
			"fsgnjn.ps f21, f7, f26\n"
			"fcvt.ps.pw f31, f17, rne\n"
			"la x18, rand_ieee754_32\n"
			"flw.ps f16, 384(x18)\n"
			"fcvt.w.s x17, f16, rdn\n" 
			"fsrai.pi f9, f4, 0x2\n"
			"maskpopcz x11, m6\n"
			"beq x19, x21, 1f\n"
			"1:\n"
			"addiw x12, x17, 0x14\n"
			"flem.ps m7, f28, f4\n"
			"fltm.pi m0, f28, f30\n"
			"mov.m.x m0, x0, 0xff\n"
			"srl x25, x25, x23\n"
			"fmul.ps f14, f6, f14, rne\n"
			"fsrli.pi f10, f9, 0x3\n"
			"fsrli.pi f4, f6, 0xa\n"
			"la x11, rand_ieee754_32\n"
			"flw.ps f19, 704(x11)\n"
			"flw.ps f20, 608(x11)\n"
			"fsgnj.ps f4, f19, f20\n" 
			"fsrli.pi f27, f5, 0xf\n"
			"fcvt.f16.ps f26, f26\n"
			"fcvt.ps.pw f30, f0, rne\n"
			"fcvt.f16.ps f7, f7\n"
			"bge x29, x28, 1f\n"
			"1:\n"
			"fmax.s f15, f0, f0\n"
			"fsgnjn.ps f10, f27, f0\n"
			"fsin.ps f21, f22\n"
			"fsgnjn.ps f25, f25, f4\n"
			"fsra.pi f26, f11, f5\n"
			"la x9, rand_int_32\n"
			"flw.ps f15, 128(x9)\n"
			"flw.ps f4, 736(x9)\n"
			"fmulhu.pi f9, f15, f4\n" 
			"sraiw x10, x12, 3\n"
			"fle.ps f19, f17, f29\n"
			"fnot.pi f16, f16\n"
			"fpackrepb.pi f2, f16\n"
			"fsra.pi f15, f23, f15\n"
			"fpackrepb.pi f23, f23\n"
			"mulhu x12, x19, x12\n"
			"fsatu8.pi f4, f16\n"
			"bltu x27, x7, 1f\n"
			"1:\n"
			"addw x10, x12, x11\n"
			"li x28, 0x7c47a1ac6bba12fa\n"
			"li x13, 0xfdce43935c7609cd\n"
			"sraw x13, x28, x13\n" 
			"fsgnjn.ps f29, f19, f26\n"
			"fsgnj.ps f25, f8, f18\n"
			"fsgnj.ps f25, f18, f6\n"
			"fsgnjn.ps f15, f8, f28\n"
			"fmsub.s f30, f28, f27, f18, rup\n"
			"fsrai.pi f11, f28, 0x7\n"
			"fsrai.pi f28, f27, 0xd\n"
			"fmax.s f30, f28, f8\n"
			"fmax.s f8, f6, f27\n"
			"la x20, rand_int_32\n"
			"flw.ps f0, 512(x20)\n"
			"flw.ps f17, 480(x20)\n"
			"fsrl.pi f28, f0, f17\n" 
			"faddi.pi f19, f19, 0x164\n"
			"fsgnjx.s f7, f13, f19\n"
			"fsatu8.pi f7, f19\n"
			"bge x31, x29, 1f\n"
			"1:\n"
			"fcmovm.ps f8, f13, f13\n"
			"blt x11, x16, 1f\n"
			"1:\n"
			"fsll.pi f11, f8, f8\n"
			"blt x2, x4, 1f\n"
			"1:\n"
			"fmulhu.pi f8, f19, f28\n"
			"fsgnjn.ps f11, f13, f19\n"
			"fsatu8.pi f18, f18\n"
			"addw x5, x17, x5\n"
			"li x7, 0x8a455c8994085dbe\n"
			"li x19, 0xf3653d0dbd343c61\n"
			"srl x7, x7, x19\n" 
			"packb x12, x6, x6\n"
			"beq x25, x21, 1f\n"
			"1:\n"
			"slti x12, x6, 0x1a\n"
			"xori x23, x11, 0x1e\n"
			"fmsub.ps f22, f10, f22, f22, rup\n"
			"fmsub.ps f18, f22, f10, f30, rmm\n"
			"fnot.pi f20, f22\n"
			"blt x15, x16, 1f\n"
			"1:\n"
			"slti x11, x11, 0x1c\n"
			"fsrl.pi f20, f10, f10\n"
			"fsgnj.ps f25, f10, f10\n"
			"LBL_SEQID_0_FP_TRANS_ID_0_HID_0:\n"
			"la x27, rand_ieee754_32\n"
			"flw.ps f4, 192(x27)\n"
			"fexp.ps f31, f4\n" 
			"fsgnjn.s f28, f22, f18\n"
			"bltu x16, x14, 1f\n"
			"1:\n"
			"fsrai.pi f19, f13, 0xb\n"
			"fsra.pi f8, f6, f29\n"
			"fpackreph.pi f0, f29\n"
			"beq x27, x15, 1f\n"
			"1:\n"
			"fle.pi f12, f12, f1\n"
			"fcvt.ps.pw f19, f14, rdn\n"
			"fmax.s f28, f29, f11\n"
			"bgeu x11, x9, 1f\n"
			"1:\n"
			"fsrl.pi f8, f29, f11\n"
			"fle.pi f13, f13, f22\n"
			"la x26, rand_ieee754_32\n"
			"flw.ps f22, 256(x26)\n"
			"flw.ps f10, 960(x26)\n"
			"flw.ps f1, 512(x26)\n"
			"fmsub.ps f2, f22, f10, f1, rtz\n" 
			"mulhu x14, x25, x25\n"
			"or x6, x13, x18\n"
			"or x14, x16, x14\n"
			"sll x9, x15, x16\n"
			"addiw x5, x7, 0xb\n"
			"srl x10, x6, x10\n"
			"beq x17, x17, 1f\n"
			"1:\n"
			"fsgnjn.ps f23, f29, f25\n"
			"fadd.s f19, f25, f16, rmm\n"
			"srl x10, x9, x14\n"
			"li x17, 0x7102cc4faeee8cde\n"
			"slli x27, x17, 3\n" 
			"slt x12, x17, x15\n"
			"slliw x28, x20, 2\n"
			"beq x21, x10, 1f\n"
			"1:\n"
			"fsgnjn.ps f8, f6, f2\n"
			"fmin.pi f22, f0, f21\n"
			"fcvt.pw.ps f0, f26, rdn\n"
			"srliw x13, x10, 3\n"
			"fsin.ps f0, f14\n"
			"feq.ps f22, f2, f18\n"
			"or x20, x6, x17\n"
			"la x18, rand_int_32\n"
			"flw.ps f17, 576(x18)\n"
			"fsatu8.pi f29, f17\n" 
			"fcmovm.ps f19, f19, f25\n"
			"srliw x9, x17, 4\n"
			"fandi.pi f0, f0, 0xd1\n"
			"fsatu8.pi f4, f10\n"
			"fsll.pi f31, f6, f22\n"
			"add x22, x22, x5\n"
			"beq x1, x4, 1f\n"
			"1:\n"
			"srliw x26, x28, 4\n"
			"fandi.pi f23, f25, 0x49\n"
			"blt x18, x22, 1f\n"
			"1:\n"
			"srliw x7, x12, 5\n"
			"li x17, 0xe93d9466f74898f0\n"
			"li x9, 0x8500a26fc4c55626\n"
			"mulhu x17, x17, x9\n" 
			"fandi.pi f7, f10, 0xef\n"
			"fcvt.ps.f16 f21, f15\n"
			"faddi.pi f25, f10, 0x12d\n"
			"fcmovm.ps f10, f10, f6\n"
			"fmin.pi f29, f9, f17\n"
			"fsgnjn.s f9, f22, f15\n"
			"fsgnjx.s f9, f6, f14\n"
			"faddi.pi f10, f10, 0x1a4\n"
			"fcvt.f16.ps f7, f7\n"
			"li x26, 0x1f31bb1018c49368\n"
			"xori x27, x26, 0xd\n" 
			"slti x17, x17, 0xd\n"
			"fadd.ps f27, f22, f31, rtz\n"
			"bne x13, x5, 1f\n"
			"1:\n"
			"fmul.pi f2, f30, f2\n"
			"blt x1, x4, 1f\n"
			"1:\n"
			"divw x23, x17, x17\n"
			"srlw x15, x13, x13\n"
			"bgeu x13, x9, 1f\n"
			"1:\n"
			"and x20, x0, x19\n"
			"beq x17, x19, 1f\n"
			"1:\n"
			"slti x22, x13, 0xb\n"
			"fsin.ps f31, f31\n"
			"bltu x25, x30, 1f\n"
			"1:\n"
			"ebreak\n"
			"la x23, rand_int_32\n"
			"flw.ps f12, 864(x23)\n"
			"fandi.pi f12, f12, 0x2\n" 
			"fmin.pi f11, f11, f22\n"
			"bge x21, x0, 1f\n"
			"1:\n"
			"srl x28, x28, x9\n"
			"maskand m5, m7, m2\n"
			"bgeu x26, x20, 1f\n"
			"1:\n"
			"sllw x16, x17, x28\n"
			"sraw x20, x26, x17\n"
			"fcvt.w.s x15, f6, rup\n"
			"fltm.pi m1, f27, f31\n"
			"addi x28, x19, 0x1c\n"
			"fmax.pi f6, f7, f29\n"
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_0:\n"
			"fltm.pi m5, f19, f5\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0xad0d5870f\n"
			"fsw.ps f0, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"fminu.pi f10, f19, f15\n"
			"fnmsub.s f26, f26, f26, f3, rup\n"
			"faddi.pi f19, f22, 0x1a3\n"
			"fcvt.f16.ps f31, f22\n"
			"fsrai.pi f23, f25, 0x0\n"
			"fsgnjx.s f12, f12, f28\n"
			"fmul.pi f12, f12, f15\n"
			"fmul.ps f26, f3, f25, rne\n"
			"fle.pi f22, f4, f19\n"
			"la x25, rand_int_32\n"
			"flw.ps f26, 672(x25)\n"
			"fpackrepb.pi f3, f26\n" 
			"fnmsub.s f18, f29, f29, f29, rne\n"
			"fmax.pi f18, f30, f30\n"
			"ecall\n"
			"fpackreph.pi f8, f30\n"
			"fpackrepb.pi f18, f20\n"
			"fmulhu.pi f21, f24, f24\n"
			"fcvt.s.wu f30, x17, rmm\n"
			"fmsub.ps f30, f30, f29, f24, rmm\n"
			"ebreak\n"
			"la x5, rand_int_32\n"
			"flw.ps f18, 992(x5)\n"
			"fcvt.ps.pw f1, f18, rmm\n" 
			"fmax.s f6, f6, f13\n"
			"slti x26, x28, 0x10\n"
			"divw x5, x0, x5\n"
			"addw x25, x6, x27\n"
			"fmul.pi f3, f24, f12\n"
			"bge x30, x7, 1f\n"
			"1:\n"
			"fsgnjn.ps f17, f22, f6\n"
			"sraw x27, x17, x6\n"
			"fle.ps f14, f22, f4\n"
			"fsgnjn.s f21, f21, f24\n"
			"beq x11, x6, 1f\n"
			"1:\n"
			"li x6, 0x60c0af49a226ffe3\n"
			"li x7, 0xd9a450e05cab425f\n"
			"sllw x26, x6, x7\n" 
			"fsatu8.pi f1, f28\n"
			"fcvt.f16.ps f29, f27\n"
			"fsgnjn.s f1, f13, f27\n"
			"fexp.ps f21, f0\n"
			"fmul.pi f22, f20, f8\n"
			"fsrai.pi f0, f8, 0xe\n"
			"sll x26, x11, x13\n"
			"slt x0, x25, x15\n"
			"bne x28, x6, 1f\n"
			"1:\n"
			"slti x17, x11, 0x14\n"
			"la x25, rand_ieee754_32\n"
			"flw.ps f0, 32(x25)\n"
			"flw.ps f30, 576(x25)\n"
			"fsgnj.ps f20, f0, f30\n" 
			"srl x18, x10, x18\n"
			"fmax.pi f17, f9, f30\n"
			"bge x10, x31, 1f\n"
			"1:\n"
			"srlw x20, x25, x13\n"
			"ebreak\n"
			"fle.s x15, f22, f20\n"
			"bltu x23, x11, 1f\n"
			"1:\n"
			"faddi.pi f8, f4, 0x40\n"
			"add x9, x7, x28\n"
			"fpackreph.pi f20, f27\n"
			"fltm.ps m4, f31, f4\n"
			"la x20, rand_ieee754_32\n"
			"flw.ps f29, 896(x20)\n"
			"flw.ps f23, 224(x20)\n"
			"fmax.s f25, f29, f23\n" 
			"fmv.x.s x5, f26\n"
			"fmsub.s f28, f2, f28, f22, rtz\n"
			"fsrli.pi f10, f10, 0x7\n"
			"and x16, x15, x11\n"
			"fence\n"
			"feq.ps f28, f0, f22\n"
			"fcvt.f16.ps f26, f7\n"
			"flem.ps m7, f28, f26\n"
			"slt x28, x0, x18\n"
			"li x19, 0x74609b26248ce856\n"
			"slliw x6, x19, 2\n" 
			"fle.pi f23, f9, f31\n"
			"fmul.ps f9, f24, f11, rtz\n"
			"fexp.ps f15, f11\n"
			"fnmsub.s f3, f9, f24, f9, rmm\n"
			"fnot.pi f23, f31\n"
			"fsra.pi f19, f24, f11\n"
			"fmax.s f19, f24, f12\n"
			"fadd.s f12, f21, f12, rtz\n"
			"fadd.s f9, f11, f21, rup\n"
			"li x12, 0xd9416453645e0364\n"
			"li x18, 0x611f810b21457e91\n"
			"sraw x9, x12, x18\n" 
			"fsatu8.pi f11, f27\n"
			"bne x11, x6, 1f\n"
			"1:\n"
			"maskpopcz x5, m0\n"
			"mov.m.x m0, x0, 0xff\n"
			"fnot.pi f11, f0\n"
			"slli x0, x16, 0\n"
			"beq x26, x26, 1f\n"
			"1:\n"
			"fsgnj.ps f27, f0, f27\n"
			"sraw x0, x6, x5\n"
			"fle.pi f0, f0, f0\n"
			"srliw x10, x19, 0\n"
			"bge x19, x15, 1f\n"
			"1:\n"
			"or x28, x6, x17\n"
			"li x6, 0xb6b525c82a8d249\n"
			"li x5, 0x4054c39c2fdd764c\n"
			"srl x18, x6, x5\n" 
			"slli x11, x26, 2\n"
			"fmulhu.pi f31, f11, f15\n"
			"srliw x23, x27, 5\n"
			"slliw x6, x6, 3\n"
			"bge x17, x0, 1f\n"
			"1:\n"
			"mulhu x10, x16, x5\n"
			"fsll.pi f2, f27, f19\n"
			"fnmsub.s f13, f2, f12, f8, rup\n"
			"bgeu x11, x2, 1f\n"
			"1:\n"
			"fsrli.pi f16, f20, 0xb\n"
			"fsll.pi f20, f3, f20\n"
			"la x11, rand_int_32\n"
			"flw.ps f7, 416(x11)\n"
			"fpackreph.pi f27, f7\n" 
			"fsll.pi f13, f5, f14\n"
			"blt x10, x14, 1f\n"
			"1:\n"
			"fsrl.pi f17, f7, f22\n"
			"fminu.pi f6, f6, f30\n"
			"maskand m0, m1, m7\n"
			"mov.m.x m0, x0, 0xff\n"
			"fmul.pi f17, f17, f30\n"
			"beq x16, x20, 1f\n"
			"1:\n"
			"slti x5, x25, 0x3\n"
			"addi x22, x14, 0x8\n"
			"maskpopcz x23, m2\n"
			"fsgnjn.ps f18, f18, f11\n"
			"mov.m.x m0, x0, 0xFF\n"
			"li x5, 0x20\n"
			"la x31, rand_ieee754_32\n"
			"add x31, x31, x5\n"
			"flw.ps f10, 0(x31)\n"
			"flw.ps f11, 0(x31)\n"
			"flw.ps f12, 5(x31)\n"
			"flw.ps f20, 8(x31)\n"
			"flw.ps f21, 3(x31)\n"
			"flw.ps f23, 1(x31)\n"
			"flw.ps f29, 10(x31)\n"
			"flw.ps f6, 8(x31)\n"
			"li x21, 0x3\n"
			"LBL_SEQID_0_LOOP_SEQ_HID_0:\n"
			"fsatu8.pi f20, f10\n"
			"fnmsub.s f22, f20, f10, f29, rne\n"
			"beq x16, x30, 1f\n"
			"1:\n"
			"fandi.pi f29, f12, 0x185\n"
			"fsgnj.ps f10, f23, f21\n"
			"fmax.s f2, f6, f6\n"
			"fmax.s f22, f11, f20\n"
			"fmax.pi f21, f11, f21\n"
			"bne x6, x4, 1f\n"
			"1:\n"
			"fadd.ps f10, f29, f23, rmm\n"
			"fmulh.pi f29, f29, f23\n"
			"fmax.pi f22, f12, f10\n"
			"fcvt.pw.ps f8, f23, rdn\n"
			"fcvt.ps.pw f29, f29, rdn\n"
			"fsgnjx.s f29, f23, f11\n"
			"fsra.pi f10, f12, f20\n"
			"bltu x27, x25, 1f\n"
			"1:\n"
			"fsgnjn.ps f8, f20, f21\n"
			"fle.ps f22, f20, f12\n"
			"fsatu8.pi f11, f11\n"
			"fsrl.pi f10, f23, f29\n"
			"feq.ps f2, f20, f10\n"
			"fsra.pi f29, f29, f20\n"
			"bltu x25, x5, 1f\n"
			"1:\n"
			"fsrai.pi f22, f12, 0x2\n"
			"fmul.pi f29, f11, f29\n"
			"fsrl.pi f20, f6, f12\n"
			"fcvt.ps.f16 f10, f6\n"
			"fmulhu.pi f10, f10, f20\n"
			"fmsub.ps f3, f20, f23, f6, rdn\n"
			"fcmovm.ps f10, f10, f29\n"
			"fsrl.pi f30, f10, f29\n"
			"fpackrepb.pi f6, f21\n"
			"fmul.pi f24, f12, f20\n"
			"fle.pi f21, f29, f20\n"
			"fmsub.ps f20, f23, f20, f20, rmm\n"
			"fmulh.pi f10, f12, f10\n"
			"fsll.pi f24, f11, f21\n"
			"bgeu x10, x30, 1f\n"
			"1:\n"
			"fandi.pi f2, f23, 0xf5\n"
			"fmsub.ps f25, f21, f12, f10, rdn\n"
			"fsrl.pi f27, f6, f21\n"
			"fandi.pi f10, f29, 0xf6\n"
			"fmin.pi f3, f10, f11\n"
			"fmul.pi f23, f10, f23\n"
			"fpackreph.pi f2, f6\n"
			"fexp.ps f29, f23\n"
			"faddi.pi f21, f10, 0x1e\n"
			"fpackreph.pi f15, f6\n"
			"fsgnjn.s f20, f12, f20\n"
			"beq x24, x13, 1f\n"
			"1:\n"
			"fadd.ps f21, f21, f29, rtz\n"
			"bge x17, x27, 1f\n"
			"1:\n"
			"fcvt.f16.ps f10, f12\n"
			"fsatu8.pi f15, f23\n"
			"fmax.pi f10, f12, f10\n"
			"fmul.ps f25, f29, f11, rdn\n"
			"fexp.ps f6, f29\n"
			"fpackreph.pi f12, f10\n"
			"fmulh.pi f27, f20, f20\n"
			"fmax.pi f11, f29, f11\n"
			"fcvt.ps.f16 f10, f10\n"
			"beq x1, x19, 1f\n"
			"1:\n"
			"fmin.pi f29, f12, f29\n"
			"fsra.pi f8, f20, f29\n"
			"fsin.ps f12, f12\n"
			"fsgnjn.ps f10, f20, f6\n"
			"bgeu x11, x11, 1f\n"
			"1:\n"
			"fle.ps f6, f6, f6\n"
			"fsgnjn.s f2, f11, f23\n"
			"fsrl.pi f2, f6, f12\n"
			"fmsub.ps f10, f10, f10, f10, rup\n"
			"fmulh.pi f25, f10, f10\n"
			"fminu.pi f2, f11, f6\n"
			"beq x2, x28, 1f\n"
			"1:\n"
			"fle.pi f24, f21, f6\n"
			"fsrai.pi f24, f20, 0xc\n"
			"bne x24, x20, 1f\n"
			"1:\n"
			"fmulh.pi f29, f6, f21\n"
			"bltu x2, x22, 1f\n"
			"1:\n"
			"fmul.pi f25, f23, f12\n"
			"fandi.pi f22, f23, 0x1f9\n"
			"fmax.pi f29, f29, f23\n"
			"fmin.pi f25, f21, f29\n"
			"fadd.s f6, f12, f6, rmm\n"
			"bltu x9, x10, 1f\n"
			"1:\n"
			"faddi.pi f2, f10, 0xd4\n"
			"fsll.pi f24, f10, f10\n"
			"bge x11, x9, 1f\n"
			"1:\n"
			"fmax.s f6, f20, f20\n"
			"fminu.pi f11, f6, f10\n"
			"fcvt.ps.pw f10, f21, rtz\n"
			"fandi.pi f8, f10, 0x185\n"
			"fmax.s f12, f6, f12\n"
			"fnmsub.s f30, f20, f29, f20, rne\n"
			"fcvt.ps.pw f22, f10, rne\n"
			"fmul.pi f10, f21, f23\n"
			"bge x6, x1, 1f\n"
			"1:\n"
			"fmin.pi f11, f11, f29\n"
			"fcvt.ps.f16 f20, f20\n"
			"fcvt.pw.ps f2, f23, rdn\n"
			"beq x6, x19, 1f\n"
			"1:\n"
			"fsra.pi f15, f20, f12\n"
			"fcvt.ps.f16 f11, f21\n"
			"bgeu x27, x29, 1f\n"
			"1:\n"
			"fpackreph.pi f15, f6\n"
			"fle.ps f21, f29, f23\n"
			"fsrai.pi f27, f12, 0x3\n"
			"fsgnjx.s f10, f10, f10\n"
			"beq x10, x5, 1f\n"
			"1:\n"
			"fmulh.pi f22, f20, f23\n"
			"fpackreph.pi f23, f23\n"
			"fsin.ps f23, f12\n"
			"feq.ps f8, f10, f12\n"
			"beq x15, x16, 1f\n"
			"1:\n"
			"fsatu8.pi f24, f20\n"
			"fsgnjx.s f25, f10, f12\n"
			"fsrai.pi f25, f12, 0xd\n"
			"fcvt.ps.f16 f25, f20\n"
			"fsin.ps f24, f12\n"
			"fsrli.pi f2, f21, 0xa\n"
			"fsin.ps f12, f29\n"
			"fle.pi f30, f6, f23\n"
			"fsrl.pi f6, f12, f29\n"
			"fmulh.pi f10, f20, f29\n"
			"fmax.s f2, f20, f21\n"
			"fsgnjn.ps f10, f6, f29\n"
			"fcvt.pw.ps f2, f10, rdn\n"
			"fminu.pi f11, f20, f11\n"
			"fsin.ps f12, f12\n"
			"fsin.ps f24, f12\n"
			"fnmsub.s f2, f12, f12, f23, rup\n"
			"fcvt.ps.pw f30, f11, rmm\n"
			"fmulhu.pi f27, f21, f6\n"
			"fmulhu.pi f15, f10, f12\n"
			"blt x23, x19, 1f\n"
			"1:\n"
			"fmulh.pi f11, f10, f11\n"
			"faddi.pi f29, f21, 0x1fb\n"
			"fnmsub.s f11, f12, f11, f23, rne\n"
			"fpackreph.pi f2, f23\n"
			"fle.ps f23, f12, f10\n"
			"fcvt.ps.f16 f3, f12\n"
			"fadd.ps f25, f23, f12, rdn\n"
			"fsin.ps f27, f10\n"
			"fsll.pi f21, f11, f6\n"
			"fsrl.pi f20, f12, f20\n"
			"fle.pi f24, f11, f21\n"
			"fsgnj.ps f23, f23, f23\n"
			"bne x16, x27, 1f\n"
			"1:\n"
			"fsgnjn.ps f11, f11, f20\n"
			"fsgnjx.s f10, f6, f10\n"
			"faddi.pi f15, f23, 0x172\n"
			"beq x8, x22, 1f\n"
			"1:\n"
			"fcmovm.ps f29, f10, f11\n"
			"fexp.ps f10, f10\n"
			"fmin.pi f12, f20, f6\n"
			"fmsub.ps f25, f11, f21, f12, rdn\n"
			"fadd.s f3, f11, f12, rtz\n"
			"fmul.pi f10, f6, f20\n"
			"fmulh.pi f22, f10, f20\n"
			"bgeu x16, x9, 1f\n"
			"1:\n"
			"fpackrepb.pi f15, f23\n"
			"fsatu8.pi f29, f11\n"
			"beq x16, x19, 1f\n"
			"1:\n"
			"fle.pi f25, f10, f23\n"
			"fadd.ps f27, f29, f10, rne\n"
			"fle.pi f11, f11, f23\n"
			"fsgnjn.s f24, f6, f20\n"
			"fmax.s f22, f11, f6\n"
			"blt x26, x14, 1f\n"
			"1:\n"
			"fminu.pi f10, f20, f21\n"
			"fsatu8.pi f2, f10\n"
			"fsrai.pi f11, f12, 0x2\n"
			"addi x21, x21, -1\n"
			"bne x21, x0, LBL_SEQID_0_LOOP_SEQ_HID_0\n"
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_0\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_1_FP_AMO_ID_2_HID_0:\n"
			"li x12, 0x8230460030\n" 
			"la x17, mem_ptr_offset\n"
			"flw.ps f26, 288(x17)\n"
			"famomaxug.pi f18, f26 (x12)\n" 
			"fsgnjx.s f0, f29, f10\n"
			"frcp.ps f11, f7\n"
			"feq.ps f12, f10, f10\n"
			"LBL_SEQID_1_FP_G_GATHER_ID_3_HID_0:\n"
			"li x12, 0x822c27e308\n" 
			"la x23, mem_ptr_offset\n"
			"flq2 f27, 72(x23)\n"
			"fscwg.ps f19, f27 (x12)\n"
			"fgwg.ps f1, f27 (x12)\n" 
			"slliw x11, x11, 3\n"
			"flog.ps f19, f4\n"
			"flt.s x13, f17, f10\n"
			"bltu x23, x6, 1f\n"
			"1:\n"
			"LBL_SEQID_1_FP_GATHER_ID_4_HID_0:\n"
			"li x9, 0x810c8e0017\n" 
			"la x7, mem_ptr_offset\n"
			"flw.ps f31, 128(x7)\n"
			"fscw.ps f13, f31 (x9)\n"
			"fgb.ps f15, f31 (x9)\n" 
			"fsub.s f25, f8, f2, rtz\n"
			"ffrc.ps f9, f29\n"
			"bge x10, x27, 1f\n"
			"1:\n"
			"fandi.pi f19, f19, 0xa1\n"
			"LBL_SEQID_1_INT_STORE_ID_5_HID_0:\n"
			"li x17, 0x820e290e26\n" 
			"sd x5, 1476(x17)\n" 
			"fadd.pi f6, f2, f19\n"
			"xori x16, x5, 0xc\n"
			"fmin.pi f1, f20, f7\n"
			"LBL_SEQID_1_INT_AMO_ID_6_HID_0:\n"
			"li x26, 0x8229c024d0\n" 
			"amoswapg.d x15, x27, (x26)\n" 
			"flt.ps f27, f14, f11\n"
			"feq.ps f16, f5, f11\n"
			"fmsub.s f15, f4, f4, f27, rup\n"
			"LBL_SEQID_1_INT_LOAD_ID_7_HID_0:\n"
			"li x18, 0x8209698807\n" 
			"sd x22, 1710(x18)\n"
			"lh x9, 1710(x18)\n" 
			"ori x25, x25, 0x0\n"
			"fsgnjn.s f0, f0, f0\n"
			"frcp.ps f2, f0\n"
			"LBL_SEQID_1_INT_LOAD_ID_8_HID_0:\n"
			"li x26, 0x820dae0001\n" 
			"sd x5, -396(x26)\n"
			"lh x19, -396(x26)\n" 
			"fcvt.pw.ps f17, f17, rne\n"
			"feq.s x6, f25, f23\n"
			"bge x1, x0, 1f\n"
			"1:\n"
			"fsgnjn.ps f10, f30, f28\n"
			"LBL_SEQID_1_INT_AMO_ID_9_HID_0:\n"
			"li x5, 0x825145d038\n" 
			"amoxorl.d x22, x22, (x5)\n" 
			"flem.ps m1, f10, f9\n"
			"remu x14, x23, x14\n"
			"fmul.s f20, f20, f10, rdn\n"
			"blt x17, x18, 1f\n"
			"1:\n"
			"LBL_SEQID_1_FP_STORE_ID_10_HID_0:\n"
			"li x25, 0x820360023a\n" 
			"fsw f2, 1655(x25)\n" 
			"fsra.pi f22, f22, f13\n"
			"fxor.pi f12, f22, f25\n"
			"fcvt.ps.pw f9, f22, rup\n"
			"LBL_SEQID_1_INT_AMO_ID_11_HID_0:\n"
			"li x11, 0x8133b7a540\n" 
			"amoswapg.d x6, x10, (x11)\n" 
			"frsq.ps f18, f16\n"
			"fcvt.ps.pw f16, f16, rtz\n"
			"fle.pi f7, f17, f7\n"
			"LBL_SEQID_1_INT_AMO_ID_12_HID_0:\n"
			"li x27, 0x822f21a640\n" 
			"amoorg.d x12, x26, (x27)\n" 
			"fsll.pi f12, f19, f17\n"
			"fltu.pi f21, f14, f31\n"
			"bge x5, x17, 1f\n"
			"1:\n"
			"fmul.pi f27, f27, f25\n"
			"LBL_SEQID_1_INT_STORE_ID_13_HID_0:\n"
			"li x10, 0x81013fa617\n" 
			"sw x25, 1749(x10)\n" 
			"fcmov.ps f23, f10, f23, f17\n"
			"beq x8, x14, 1f\n"
			"1:\n"
			"fmaxu.pi f5, f24, f20\n"
			"bgeu x21, x27, 1f\n"
			"1:\n"
			"fmax.s f3, f6, f27\n"
			"la x14, rand_int_32\n"
			"flw.ps f11, 0(x14)\n"
			"flw.ps f7, 384(x14)\n"
			"fremu.pi f13, f11, f7\n" 
			"fmin.pi f5, f1, f7\n"
			"fmin.pi f11, f25, f18\n"
			"beq x10, x31, 1f\n"
			"1:\n"
			"flt.ps f20, f19, f5\n"
			"LBL_SEQID_1_INT_AMO_ID_14_HID_0:\n"
			"li x27, 0x824d7e0018\n" 
			"amoandl.w x19, x17, (x27)\n" 
			"fcvt.ps.f16 f11, f11\n"
			"faddi.pi f31, f20, 0x146\n"
			"fle.pi f25, f29, f25\n"
			"LBL_SEQID_1_FP_AMO_ID_15_HID_0:\n"
			"li x23, 0x812d34d02c\n" 
			"la x18, mem_ptr_offset\n"
			"flw.ps f11, 224(x18)\n"
			"famominug.pi f25, f11 (x23)\n" 
			"fcvt.ps.pw f2, f15, rdn\n"
			"fcvt.ps.pw f7, f21, rdn\n"
			"fmin.s f9, f21, f20\n"
			"LBL_SEQID_1_INT_AMO_ID_16_HID_0:\n"
			"li x22, 0x822449e000\n" 
			"amoswapg.d x17, x11, (x22)\n" 
			"fsub.pi f15, f30, f8\n"
			"faddi.pi f5, f28, 0x12d\n"
			"fltu.pi f22, f1, f30\n"
			"LBL_SEQID_1_INT_AMO_ID_17_HID_0:\n"
			"li x19, 0x822410a688\n" 
			"amoorg.d x26, x9, (x19)\n" 
			"fsll.pi f10, f0, f2\n"
			"fcvt.ps.pw f5, f25, rmm\n"
			"fclass.s x15, f0\n"
			"LBL_SEQID_1_CSR_ID_18_HID_0:\n"
			"csrr x0, sie\n"
			"fsgnjn.ps f15, f24, f5\n"
			"fle.ps f2, f5, f7\n"
			"bgeu x8, x23, 1f\n"
			"1:\n"
			"addw x6, x6, x6\n"
			"LBL_SEQID_1_INT_STORE_ID_19_HID_0:\n"
			"li x12, 0x820ade0120\n" 
			"sw x7, -564(x12)\n" 
			"fslli.pi f9, f11, 0x7\n"
			"fcmov.ps f21, f21, f28, f20\n"
			"faddi.pi f13, f28, 0x1b5\n"
			"LBL_SEQID_1_INT_AMO_ID_20_HID_0:\n"
			"li x12, 0x8136e201d0\n" 
			"amomaxg.d x13, x19, (x12)\n" 
			"fsub.pi f22, f4, f30\n"
			"bltu x28, x25, 1f\n"
			"1:\n"
			"frsq.ps f17, f22\n"
			"fcvt.pw.ps f25, f4, rdn\n"
			"bge x30, x7, 1f\n"
			"1:\n"
			"LBL_SEQID_1_INT_STORE_ID_21_HID_0:\n"
			"li x19, 0x810fec06d8\n" 
			"sw x12, 274(x19)\n" 
			"fmul.ps f28, f18, f4, rtz\n"
			"bgeu x2, x11, 1f\n"
			"1:\n"
			"fcvt.ps.pwu f28, f17, rdn\n"
			"fand.pi f23, f23, f29\n"
			"bgeu x14, x5, 1f\n"
			"1:\n"
			"LBL_SEQID_1_FP_G_GATHER_ID_22_HID_0:\n"
			"li x19, 0x822a320330\n" 
			"la x20, mem_ptr_offset\n"
			"flq2 f0, 52(x20)\n"
			"fscwg.ps f14, f0 (x19)\n"
			"fghg.ps f29, f0 (x19)\n" 
			"fcmovm.ps f0, f3, f0\n"
			"fbcx.ps f16, x12\n"
			"fswizz.ps f31, f1, 0x15\n"
			"LBL_SEQID_1_FP_L_LOAD_ID_23_HID_0:\n"
			"li x5, 0x8159709020\n" 
			"mov.m.x m0, zero, 0xff\n"
			"fswl.ps f30, (x5)\n"
			"flwl.ps f27, (x5)\n" 
			"fle.ps f2, f4, f4\n"
			"fmul.pi f21, f31, f31\n"
			"fnmadd.ps f31, f4, f31, f27, rdn\n"
			"bgeu x15, x18, 1f\n"
			"1:\n"
			"LBL_SEQID_1_INT_AMO_ID_24_HID_0:\n"
			"li x5, 0x812c8e0010\n" 
			"amoming.d x27, x23, (x5)\n" 
			"frsq.ps f6, f7\n"
			"fsub.s f23, f7, f6, rdn\n"
			"fnmsub.s f27, f15, f25, f7, rne\n"
			"LBL_SEQID_1_FP_L_SCATTER_ID_25_HID_0:\n"
			"li x11, 0x824ee80288\n" 
			"la x6, mem_ptr_offset\n"
			"flq2 f0, 48(x6)\n"
			"fscbl.ps f8, f0 (x11)\n" 
			"flt.pi f30, f9, f19\n"
			"fswizz.ps f6, f6, 0x1c\n"
			"bltu x5, x3, 1f\n"
			"1:\n"
			"fround.ps f30, f7, rup\n"
			"bne x21, x12, 1f\n"
			"1:\n"
			"LBL_SEQID_1_INT_AMO_ID_26_HID_0:\n"
			"li x6, 0x815db1a800\n" 
			"amominul.d x11, x16, (x6)\n" 
			"fsrai.pi f26, f26, 0xd\n"
			"fmax.ps f9, f9, f7\n"
			"fmul.pi f22, f11, f22\n"
			"LBL_SEQID_1_CSR_ID_27_HID_0:\n"
			"li x11, 0x8209c024c2\n"
			"li x31, (0x2 << 6) | 0x1\n"
			"csrw lock_va, x11\n" 
			"csrwi tensor_wait, 6\n" 
			"fsgnj.s f5, f30, f6\n"
			"fsgnj.s f28, f30, f30\n"
			"fmadd.s f20, f6, f20, f30, rne\n"
			"bne x1, x19, 1f\n"
			"1:\n"
			"LBL_SEQID_1_FP_AMO_ID_28_HID_0:\n"
			"li x17, 0x8224e7b008\n" 
			"la x6, mem_ptr_offset\n"
			"flw.ps f10, 480(x6)\n"
			"famomaxg.pi f16, f10 (x17)\n" 
			"slliw x25, x13, 3\n"
			"mulhsu x19, x19, x7\n"
			"fmaxu.pi f15, f4, f14\n"
			"bge x13, x26, 1f\n"
			"1:\n"
			"LBL_SEQID_1_FP_SCATTER_ID_29_HID_0:\n"
			"li x20, 0x8107ee0033\n" 
			"li x27, 0x2a64bfc599f2b0f\n"
			"fsc32h.ps f18, x27 (x20)\n" 
			"fsub.ps f16, f18, f16, rtz\n"
			"fround.ps f27, f18, rmm\n"
			"bne x15, x14, 1f\n"
			"1:\n"
			"fcvt.ps.pw f22, f16, rtz\n"
			"bne x23, x10, 1f\n"
			"1:\n"
			"LBL_SEQID_1_INT_LOAD_ID_30_HID_0:\n"
			"li x23, 0x810e97f331\n" 
			"sd x9, 429(x23)\n"
			"lb x12, 429(x23)\n" 
			"flt.pi f22, f26, f22\n"
			"fsgnjx.ps f10, f22, f26\n"
			"fmax.s f26, f26, f22\n"
			"LBL_SEQID_1_INT_STORE_ID_31_HID_0:\n"
			"li x17, 0x810a3d6177\n" 
			"sd x28, -1788(x17)\n" 
			"fxor.pi f7, f1, f7\n"
			"bge x20, x27, 1f\n"
			"1:\n"
			"slliw x15, x26, 2\n"
			"addiw x23, x20, 0x9\n"
			"bltu x8, x25, 1f\n"
			"1:\n"
			"LBL_SEQID_1_INT_AMO_ID_32_HID_0:\n"
			"li x27, 0x815fd48800\n" 
			"amomaxul.w x22, x6, (x27)\n" 
			"fsub.s f10, f13, f27, rdn\n"
			"fsin.ps f25, f2\n"
			"frsq.ps f7, f7\n"
			"csrwi tensor_wait, 6\n"
			"fence\n"
			"li x5, 0x8209c024c2\n" 
			"li x31, (0x2 << 6) | 0x1\n"
			"csrw unlock_va, x5\n"
			"csrwi tensor_wait, 6\n"
			"li x31, 0b11 << 58\n"
			"add x5, x31, x5\n"
			"li x31, (0x2 << 6) | 0x1\n"
			"csrw flush_va, x5\n"
			"csrwi tensor_wait, 6\n"
			"mov.m.x m0, x0, 0xFF\n"
			"li x5, 0x10\n"
			"la x31, rand_ieee754_32\n"
			"add x31, x31, x5\n"
			"flw.ps f0, 0(x31)\n"
			"flw.ps f1, 4(x31)\n"
			"flw.ps f10, 4(x31)\n"
			"flw.ps f11, 9(x31)\n"
			"flw.ps f12, 0(x31)\n"
			"flw.ps f13, 12(x31)\n"
			"flw.ps f14, 4(x31)\n"
			"flw.ps f15, 13(x31)\n"
			"flw.ps f16, 24(x31)\n"
			"flw.ps f17, 32(x31)\n"
			"flw.ps f18, 40(x31)\n"
			"flw.ps f19, 37(x31)\n"
			"flw.ps f2, 16(x31)\n"
			"flw.ps f20, 51(x31)\n"
			"flw.ps f21, 49(x31)\n"
			"flw.ps f22, 14(x31)\n"
			"flw.ps f23, 8(x31)\n"
			"flw.ps f24, 32(x31)\n"
			"flw.ps f25, 2(x31)\n"
			"flw.ps f26, 53(x31)\n"
			"flw.ps f27, 68(x31)\n"
			"flw.ps f28, 84(x31)\n"
			"flw.ps f29, 16(x31)\n"
			"flw.ps f3, 5(x31)\n"
			"flw.ps f30, 23(x31)\n"
			"flw.ps f31, 2(x31)\n"
			"flw.ps f4, 20(x31)\n"
			"flw.ps f5, 25(x31)\n"
			"flw.ps f6, 92(x31)\n"
			"flw.ps f7, 96(x31)\n"
			"flw.ps f8, 84(x31)\n"
			"flw.ps f9, 81(x31)\n"
			"li x21, 0x1\n"
			"LBL_SEQID_1_LOOP_SEQ_HID_0:\n"
			"fmul.s f21, f21, f4, rdn\n"
			"blt x20, x11, 1f\n"
			"1:\n"
			"fsrl.pi f17, f14, f20\n"
			"fand.pi f26, f12, f31\n"
			"fcvt.ps.pwu f8, f8, rtz\n"
			"bltu x1, x20, 1f\n"
			"1:\n"
			"fswizz.ps f16, f28, 0xf\n"
			"fadd.pi f31, f1, f17\n"
			"fltu.pi f8, f8, f6\n"
			"fmin.ps f26, f26, f18\n"
			"fmin.s f13, f9, f21\n"
			"fxor.pi f2, f2, f25\n"
			"fpackrepb.pi f9, f24\n"
			"blt x9, x30, 1f\n"
			"1:\n"
			"fle.pi f30, f27, f22\n"
			"fmin.ps f27, f24, f27\n"
			"fsgnjn.s f10, f3, f18\n"
			"fcvt.pwu.ps f10, f25, rmm\n"
			"fsgnjn.ps f14, f12, f14\n"
			"bltu x23, x16, 1f\n"
			"1:\n"
			"fsra.pi f27, f4, f11\n"
			"fcvt.ps.pwu f25, f18, rtz\n"
			"fmin.ps f1, f1, f2\n"
			"ffrc.ps f20, f21\n"
			"fmul.pi f13, f17, f3\n"
			"bne x3, x10, 1f\n"
			"1:\n"
			"fnmsub.ps f31, f14, f8, f28, rne\n"
			"faddi.pi f29, f30, 0x165\n"
			"fcvt.ps.pw f27, f25, rdn\n"
			"fltu.pi f24, f2, f24\n"
			"blt x11, x20, 1f\n"
			"1:\n"
			"for.pi f16, f19, f6\n"
			"fsgnj.ps f17, f21, f31\n"
			"fsgnjn.ps f8, f5, f28\n"
			"fmax.pi f10, f10, f13\n"
			"flt.pi f13, f1, f21\n"
			"fmax.s f11, f11, f24\n"
			"fmul.pi f14, f30, f18\n"
			"beq x26, x28, 1f\n"
			"1:\n"
			"fsatu8.pi f16, f25\n"
			"ffrc.ps f10, f2\n"
			"fsgnjn.s f26, f27, f26\n"
			"fmin.ps f27, f20, f29\n"
			"fcvt.ps.pwu f16, f6, rne\n"
			"flt.ps f4, f1, f31\n"
			"bne x22, x10, 1f\n"
			"1:\n"
			"fsrl.pi f20, f8, f25\n"
			"flog.ps f19, f13\n"
			"fmsub.s f24, f24, f5, f27, rne\n"
			"fmul.ps f15, f9, f17, rtz\n"
			"fadd.ps f8, f13, f8, rup\n"
			"beq x19, x27, 1f\n"
			"1:\n"
			"fle.ps f19, f25, f8\n"
			"fsrai.pi f27, f25, 0x7\n"
			"fltu.pi f28, f28, f5\n"
			"fcvt.ps.pwu f9, f28, rup\n"
			"bltu x6, x0, 1f\n"
			"1:\n"
			"fand.pi f20, f11, f31\n"
			"fsat8.pi f17, f29\n"
			"ffrc.ps f28, f12\n"
			"feq.pi f23, f18, f23\n"
			"bge x17, x26, 1f\n"
			"1:\n"
			"fmul.s f1, f1, f27, rne\n"
			"fmulhu.pi f14, f18, f30\n"
			"fmsub.ps f20, f22, f2, f20, rup\n"
			"fsrai.pi f2, f3, 0xd\n"
			"fsgnjn.s f31, f15, f29\n"
			"bgeu x27, x17, 1f\n"
			"1:\n"
			"fcvt.pw.ps f28, f2, rmm\n"
			"bge x16, x9, 1f\n"
			"1:\n"
			"fcvt.f16.ps f29, f6\n"
			"fmax.ps f10, f2, f24\n"
			"fmul.ps f30, f14, f30, rne\n"
			"feq.ps f17, f17, f17\n"
			"fnot.pi f4, f12\n"
			"fand.pi f4, f23, f25\n"
			"fcvt.ps.pw f28, f15, rne\n"
			"bgeu x9, x1, 1f\n"
			"1:\n"
			"fmulh.pi f5, f17, f6\n"
			"bge x31, x25, 1f\n"
			"1:\n"
			"fle.ps f15, f1, f9\n"
			"fnmadd.s f25, f22, f14, f23, rmm\n"
			"blt x12, x2, 1f\n"
			"1:\n"
			"fmsub.s f6, f17, f6, f10, rmm\n"
			"fsgnjx.ps f29, f29, f11\n"
			"fsub.s f7, f9, f7, rne\n"
			"fmin.ps f14, f14, f20\n"
			"faddi.pi f7, f29, 0x92\n"
			"fsgnjn.ps f21, f13, f20\n"
			"fsgnjx.s f2, f2, f17\n"
			"fcvt.ps.pw f3, f17, rup\n"
			"fnot.pi f7, f26\n"
			"fsatu8.pi f7, f17\n"
			"fround.ps f8, f8, rmm\n"
			"bgeu x19, x3, 1f\n"
			"1:\n"
			"flt.pi f0, f1, f11\n"
			"beq x4, x15, 1f\n"
			"1:\n"
			"fmadd.s f3, f3, f27, f18, rtz\n"
			"bge x22, x25, 1f\n"
			"1:\n"
			"fsub.ps f9, f4, f8, rdn\n"
			"fsgnjn.s f15, f10, f23\n"
			"fminu.pi f1, f16, f12\n"
			"fadd.ps f19, f3, f19, rup\n"
			"fmulhu.pi f30, f21, f1\n"
			"fadd.s f22, f29, f22, rtz\n"
			"bgeu x4, x10, 1f\n"
			"1:\n"
			"fmin.ps f10, f12, f23\n"
			"fadd.s f6, f26, f5, rmm\n"
			"fadd.s f23, f8, f7, rne\n"
			"fmin.ps f29, f8, f25\n"
			"feq.pi f13, f3, f6\n"
			"feq.pi f30, f20, f24\n"
			"fnot.pi f31, f24\n"
			"fcvt.pwu.ps f18, f0, rmm\n"
			"fsgnjx.ps f28, f13, f16\n"
			"fminu.pi f2, f1, f11\n"
			"fle.pi f15, f26, f18\n"
			"fadd.s f28, f6, f11, rdn\n"
			"fle.ps f18, f7, f18\n"
			"fsgnj.ps f31, f16, f23\n"
			"fle.ps f21, f25, f25\n"
			"fsin.ps f14, f3\n"
			"bge x10, x9, 1f\n"
			"1:\n"
			"fnmsub.s f8, f21, f11, f29, rne\n"
			"fmul.s f18, f17, f15, rup\n"
			"faddi.pi f6, f6, 0x15\n"
			"bge x27, x25, 1f\n"
			"1:\n"
			"fsgnjn.s f11, f2, f11\n"
			"fcmovm.ps f27, f7, f20\n"
			"bne x6, x4, 1f\n"
			"1:\n"
			"fsll.pi f23, f29, f13\n"
			"fsgnjx.s f14, f14, f19\n"
			"fmax.pi f8, f0, f8\n"
			"fnmadd.s f28, f17, f24, f27, rdn\n"
			"fsrl.pi f30, f16, f2\n"
			"fsub.pi f29, f17, f12\n"
			"fmin.ps f17, f15, f2\n"
			"fandi.pi f23, f24, 0xbc\n"
			"fsub.ps f16, f8, f3, rdn\n"
			"bne x15, x22, 1f\n"
			"1:\n"
			"fcvt.ps.f16 f10, f5\n"
			"fminu.pi f0, f11, f29\n"
			"fsub.ps f23, f13, f30, rup\n"
			"fandi.pi f24, f10, 0x4d\n"
			"fsrl.pi f25, f28, f16\n"
			"fcvt.ps.f16 f19, f2\n"
			"fsub.pi f18, f2, f18\n"
			"fpackreph.pi f9, f22\n"
			"fmsub.s f19, f0, f19, f25, rne\n"
			"fsin.ps f15, f5\n"
			"bne x2, x12, 1f\n"
			"1:\n"
			"fnmadd.s f15, f18, f23, f25, rtz\n"
			"feq.pi f7, f4, f9\n"
			"bltu x30, x4, 1f\n"
			"1:\n"
			"fsatu8.pi f26, f26\n"
			"fmsub.s f21, f21, f31, f5, rne\n"
			"bne x6, x12, 1f\n"
			"1:\n"
			"fminu.pi f21, f25, f21\n"
			"fsll.pi f0, f2, f0\n"
			"fmsub.s f29, f23, f29, f13, rdn\n"
			"bne x21, x9, 1f\n"
			"1:\n"
			"fmul.pi f10, f6, f16\n"
			"blt x5, x5, 1f\n"
			"1:\n"
			"fmin.pi f24, f6, f5\n"
			"bge x12, x11, 1f\n"
			"1:\n"
			"fslli.pi f25, f25, 0xb\n"
			"faddi.pi f18, f15, 0x45\n"
			"bne x7, x23, 1f\n"
			"1:\n"
			"fxor.pi f24, f2, f28\n"
			"fmul.s f9, f4, f4, rdn\n"
			"for.pi f28, f28, f24\n"
			"fle.ps f30, f30, f24\n"
			"fnmsub.s f27, f0, f18, f22, rne\n"
			"fsgnj.ps f0, f18, f7\n"
			"fexp.ps f16, f25\n"
			"fsub.pi f14, f15, f14\n"
			"fsgnjx.s f7, f29, f17\n"
			"fminu.pi f5, f21, f8\n"
			"for.pi f29, f20, f12\n"
			"bltu x18, x22, 1f\n"
			"1:\n"
			"fmin.pi f4, f20, f13\n"
			"fcmov.ps f13, f24, f0, f3\n"
			"beq x30, x3, 1f\n"
			"1:\n"
			"flt.pi f16, f12, f7\n"
			"bgeu x20, x11, 1f\n"
			"1:\n"
			"fexp.ps f25, f17\n"
			"fnmadd.ps f7, f25, f22, f27, rdn\n"
			"beq x17, x8, 1f\n"
			"1:\n"
			"fclass.ps f7, f7\n"
			"beq x22, x30, 1f\n"
			"1:\n"
			"frcp.ps f28, f9\n"
			"fadd.pi f27, f8, f21\n"
			"fmax.pi f18, f9, f18\n"
			"fmadd.ps f1, f27, f1, f2, rne\n"
			"fcvt.ps.pw f9, f18, rup\n"
			"fadd.s f22, f14, f0, rdn\n"
			"fmul.pi f31, f2, f2\n"
			"faddi.pi f14, f11, 0x17b\n"
			"fmax.pi f22, f17, f9\n"
			"fclass.ps f0, f20\n"
			"fmaxu.pi f2, f12, f27\n"
			"fle.pi f30, f1, f30\n"
			"bltu x1, x22, 1f\n"
			"1:\n"
			"fminu.pi f17, f6, f20\n"
			"fmulh.pi f8, f8, f21\n"
			"fpackreph.pi f18, f17\n"
			"bltu x12, x14, 1f\n"
			"1:\n"
			"fsra.pi f20, f1, f0\n"
			"frcp.ps f0, f17\n"
			"fswizz.ps f7, f19, 0x12\n"
			"blt x11, x25, 1f\n"
			"1:\n"
			"fadd.s f12, f30, f18, rdn\n"
			"fcvt.f16.ps f5, f7\n"
			"fcmovm.ps f15, f7, f15\n"
			"fexp.ps f2, f8\n"
			"flt.ps f9, f3, f3\n"
			"fcmov.ps f5, f17, f3, f7\n"
			"fandi.pi f28, f28, 0xd4\n"
			"feq.pi f2, f0, f27\n"
			"for.pi f25, f12, f25\n"
			"fsrai.pi f15, f19, 0x5\n"
			"beq x6, x11, 1f\n"
			"1:\n"
			"ffrc.ps f31, f31\n"
			"addi x21, x21, -1\n"
			"bne x21, x0, LBL_SEQID_1_LOOP_SEQ_HID_0\n"
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_0\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_2_FP_GATHER_ID_33_HID_0:\n"
			"li x16, 0x810b860903\n" 
			"la x9, mem_ptr_offset\n"
			"flw.ps f2, 112(x9)\n"
			"fscw.ps f3, f2 (x16)\n"
			"fgw.ps f0, f2 (x16)\n" 
			"fmsub.s f27, f19, f25, f5, rmm\n"
			"fadd.ps f19, f27, f23, rne\n"
			"fminu.pi f10, f3, f1\n"
			"fsub.pi f15, f18, f18\n"
			"fsub.pi f24, f0, f29\n"
			"LBL_SEQID_2_CSR_ID_34_HID_0:\n"
			"csrr x0, hartid\n"
			"remu x12, x6, x19\n"
			"fnmadd.s f26, f21, f26, f8, rup\n"
			"srli x9, x12, 5\n"
			"bgeu x5, x7, 1f\n"
			"1:\n"
			"fand.pi f18, f21, f26\n"
			"slt x25, x13, x6\n"
			"LBL_SEQID_2_FP_G_GATHER_ID_35_HID_0:\n"
			"li x6, 0x81250e0018\n" 
			"la x22, mem_ptr_offset\n"
			"flq2 f25, 120(x22)\n"
			"fscwg.ps f22, f25 (x6)\n"
			"fghg.ps f9, f25 (x6)\n" 
			"fsrl.pi f19, f27, f26\n"
			"bge x0, x12, 1f\n"
			"1:\n"
			"fsgnjx.ps f31, f7, f17\n"
			"beq x1, x5, 1f\n"
			"1:\n"
			"frcp.ps f31, f17\n"
			"flt.pi f20, f16, f26\n"
			"faddi.pi f5, f17, 0x1a9\n"
			"LBL_SEQID_2_FP_STORE_ID_36_HID_0:\n"
			"li x14, 0x8203bc033d\n" 
			"fsw f22, 26(x14)\n" 
			"srl x5, x13, x22\n"
			"blt x18, x7, 1f\n"
			"1:\n"
			"packb x18, x18, x7\n"
			"fmadd.ps f19, f9, f22, f13, rmm\n"
			"mov.m.x m0, x7, 0x23\n"
			"srai x19, x9, 4\n"
			"LBL_SEQID_2_FP_AMO_ID_37_HID_0:\n"
			"li x27, 0x825171eff0\n" 
			"la x11, mem_ptr_offset\n"
			"flw.ps f21, 512(x11)\n"
			"famoorl.pi f12, f21 (x27)\n" 
			"fmadd.s f8, f1, f20, f24, rdn\n"
			"fle.pi f28, f11, f6\n"
			"fsrai.pi f24, f24, 0xe\n"
			"fsll.pi f4, f2, f29\n"
			"feq.pi f26, f9, f18\n"
			"LBL_SEQID_2_INT_AMO_ID_38_HID_0:\n"
			"li x14, 0x8137740030\n" 
			"amoorg.d x20, x11, (x14)\n" 
			"fsatu8.pi f24, f2\n"
			"beq x20, x19, 1f\n"
			"1:\n"
			"ffrc.ps f30, f11\n"
			"subw x27, x6, x10\n"
			"fmvs.x.ps x22, f30, 6\n"
			"ffrc.ps f0, f13\n"
			"LBL_SEQID_2_INT_AMO_ID_39_HID_0:\n"
			"li x23, 0x814fb36018\n" 
			"amoxorl.d x9, x17, (x23)\n" 
			"fsatu8.pi f30, f4\n"
			"ebreak\n"
			"fmv.s.x f11, x25\n"
			"fnmsub.ps f4, f4, f4, f1, rup\n"
			"andi x15, x17, 0x19\n"
			"LBL_SEQID_2_CSR_ID_40_HID_0:\n"
			"csrr x0, menable_shadows\n"
			"fsub.pi f31, f5, f31\n"
			"fand.pi f1, f13, f26\n"
			"fcvt.f16.ps f13, f13\n"
			"slliw x28, x7, 3\n"
			"bltu x16, x8, 1f\n"
			"1:\n"
			"divw x7, x27, x7\n"
			"LBL_SEQID_2_INT_AMO_ID_41_HID_0:\n"
			"li x26, 0x81257c0038\n" 
			"amomaxug.d x22, x5, (x26)\n" 
			"maskxor m0, m1, m1\n"
			"fslli.pi f12, f12, 0xb\n"
			"maskpopc x10, m7\n"
			"fltm.pi m0, f12, f13\n"
			"mov.m.x m0, x0, 0xff\n"
			"bge x30, x21, 1f\n"
			"1:\n"
			"fcvt.ps.f16 f12, f13\n"
			"LBL_SEQID_2_CSR_ID_42_HID_0:\n"
			"csrr x0, tselect\n"
			"fsll.pi f24, f30, f0\n"
			"fnmadd.s f12, f1, f12, f30, rne\n"
			"fnmsub.s f26, f8, f0, f0, rdn\n"
			"fcvt.pwu.ps f12, f8, rdn\n"
			"bne x24, x24, 1f\n"
			"1:\n"
			"fsgnj.s f8, f2, f8\n"
			"fence.i\n" 
			"fmsub.ps f8, f12, f19, f8, rmm\n"
			"flt.s x5, f16, f28\n"
			"fence\n"
			"fminu.pi f27, f29, f11\n"
			"srai x15, x14, 1\n"
			"LBL_SEQID_2_FP_LOAD_ID_43_HID_0:\n"
			"li x26, 0x810e97f331\n" 
			"fsw.ps f21, -1067(x26)\n"
			"flw f1, -1067(x26)\n" 
			"fcvt.pwu.ps f30, f8, rne\n"
			"ffrc.ps f30, f27\n"
			"remu x22, x11, x9\n"
			"flt.pi f30, f27, f2\n"
			"beq x2, x4, 1f\n"
			"1:\n"
			"fmin.ps f25, f8, f13\n"
			"bgeu x11, x4, 1f\n"
			"1:\n"
			"LBL_SEQID_2_FP_SCATTER_ID_44_HID_0:\n"
			"li x14, 0x820a960014\n" 
			"la x22, mem_ptr_offset\n"
			"flw.ps f3, 216(x22)\n"
			"fsch.ps f5, f3 (x14)\n" 
			"fsgnj.s f3, f3, f22\n"
			"fclass.ps f8, f26\n"
			"fadd.pi f26, f1, f28\n"
			"bltu x8, x24, 1f\n"
			"1:\n"
			"fpackrepb.pi f17, f26\n"
			"fcvt.ps.f16 f26, f28\n"
			"LBL_SEQID_2_INT_L_STORE_ID_45_HID_0:\n"
			"li x25, 0x8242a06118\n" 
			"sbl x20, (x25)\n" 
			"mulh x25, x14, x11\n"
			"fnmsub.s f28, f5, f14, f26, rdn\n"
			"fmv.s.x f18, x13\n"
			"bltu x4, x10, 1f\n"
			"1:\n"
			"maskpopc x22, m2\n"
			"fadd.pi f18, f19, f14\n"
			"LBL_SEQID_2_INT_L_STORE_ID_46_HID_0:\n"
			"li x15, 0x824af00000\n" 
			"sbl x14, (x15)\n" 
			"fmulh.pi f15, f31, f4\n"
			"maskand m4, m7, m7\n"
			"fclass.s x18, f31\n"
			"srai x18, x7, 1\n"
			"beq x7, x19, 1f\n"
			"1:\n"
			"fslli.pi f14, f20, 0x7\n"
			"bge x6, x7, 1f\n"
			"1:\n"
			"LBL_SEQID_2_FP_SCATTER_ID_47_HID_0:\n"
			"li x19, 0x820c27e30a\n" 
			"li x12, 0xf59947f13a385323\n"
			"fsc32h.ps f23, x12 (x19)\n" 
			"fsrai.pi f1, f8, 0x5\n"
			"flt.s x20, f2, f20\n"
			"mulw x12, x12, x18\n"
			"bne x27, x6, 1f\n"
			"1:\n"
			"fle.pi f10, f22, f11\n"
			"flt.pi f5, f16, f15\n"
			"LBL_SEQID_2_FP_AMO_ID_48_HID_0:\n"
			"li x11, 0x825e12f6d4\n" 
			"la x26, mem_ptr_offset\n"
			"flw.ps f3, 864(x26)\n"
			"famominl.ps f3, f3 (x11)\n" 
			"fsat8.pi f12, f30\n"
			"fexp.ps f6, f0\n"
			"div x12, x13, x26\n"
			"bge x6, x29, 1f\n"
			"1:\n"
			"sraw x11, x11, x28\n"
			"frcp.ps f25, f14\n"
			"LBL_SEQID_2_FP_AMO_ID_49_HID_0:\n"
			"li x10, 0x822145d03c\n" 
			"la x13, mem_ptr_offset\n"
			"flw.ps f6, 960(x13)\n"
			"famomaxg.pi f2, f6 (x10)\n" 
			"fxor.pi f17, f4, f28\n"
			"flog.ps f24, f27\n"
			"fsin.ps f21, f21\n"
			"ffrc.ps f5, f27\n"
			"fmadd.ps f28, f26, f26, f28, rtz\n"
			"LBL_SEQID_2_FP_G_GATHER_ID_50_HID_0:\n"
			"li x22, 0x822d9e0320\n" 
			"la x17, mem_ptr_offset\n"
			"flw.ps f5, 52(x17)\n"
			"fscwg.ps f24, f5 (x22)\n"
			"fghg.ps f28, f5 (x22)\n" 
			"fsrli.pi f18, f23, 0x7\n"
			"bne x16, x31, 1f\n"
			"1:\n"
			"fle.pi f18, f9, f25\n"
			"feq.pi f0, f6, f25\n"
			"bge x29, x12, 1f\n"
			"1:\n"
			"fmax.ps f15, f23, f25\n"
			"fsin.ps f25, f23\n"
			"LBL_SEQID_2_INT_AMO_ID_51_HID_0:\n"
			"li x28, 0x8234606070\n" 
			"amomaxug.d x17, x10, (x28)\n" 
			"fnmsub.s f26, f26, f4, f26, rne\n"
			"divu x27, x27, x0\n"
			"fmin.ps f26, f31, f4\n"
			"mulh x27, x17, x27\n"
			"fsgnjn.s f25, f24, f24\n"
			"LBL_SEQID_2_FP_GATHER_ID_52_HID_0:\n"
			"li x25, 0x82014b303d\n" 
			"la x12, mem_ptr_offset\n"
			"flw.ps f30, 116(x12)\n"
			"fscw.ps f18, f30 (x25)\n"
			"fgb.ps f13, f30 (x25)\n" 
			"fslli.pi f12, f2, 0x4\n"
			"fsgnjx.s f16, f16, f16\n"
			"bltu x10, x2, 1f\n"
			"1:\n"
			"fsgnjx.s f2, f16, f2\n"
			"fsatu8.pi f17, f17\n"
			"bne x19, x0, 1f\n"
			"1:\n"
			"fmax.pi f16, f2, f16\n"
			"LBL_SEQID_2_FP_SCATTER_ID_53_HID_0:\n"
			"li x10, 0x820410a68d\n" 
			"la x19, mem_ptr_offset\n"
			"flq2 f4, 196(x19)\n"
			"fscw.ps f19, f4 (x10)\n" 
			"fsub.s f9, f19, f1, rtz\n"
			"bgeu x7, x17, 1f\n"
			"1:\n"
			"ecall\n"
			"fexp.ps f11, f19\n"
			"fcmovm.ps f29, f4, f19\n"
			"srlw x28, x6, x25\n"
			"bltu x11, x3, 1f\n"
			"1:\n"
			"LBL_SEQID_2_FP_SCATTER_ID_54_HID_0:\n"
			"li x25, 0x820171eff2\n" 
			"la x16, mem_ptr_offset\n"
			"flw.ps f5, 32(x16)\n"
			"fscb.ps f14, f5 (x25)\n" 
			"fsub.pi f21, f21, f30\n"
			"fswizz.ps f5, f19, 0x19\n"
			"bne x11, x23, 1f\n"
			"1:\n"
			"fcvt.pwu.ps f5, f27, rup\n"
			"fmin.ps f22, f21, f4\n"
			"fand.pi f15, f4, f30\n"
			"LBL_SEQID_2_INT_AMO_ID_55_HID_0:\n"
			"li x17, 0x812b34e31c\n" 
			"amoming.w x6, x9, (x17)\n" 
			"fnmsub.s f18, f14, f11, f4, rne\n"
			"blt x30, x19, 1f\n"
			"1:\n"
			"fmadd.ps f16, f22, f8, f21, rtz\n"
			"fcvt.f16.ps f4, f29\n"
			"fsgnjn.s f12, f6, f21\n"
			"fltu.pi f16, f21, f16\n"
			"bge x7, x28, 1f\n"
			"1:\n"
			"LBL_SEQID_2_INT_LOAD_ID_56_HID_0:\n"
			"li x25, 0x810fed3ae1\n" 
			"sd x16, 1852(x25)\n"
			"lh x17, 1852(x25)\n" 
			"frsq.ps f22, f30\n"
			"bltu x10, x21, 1f\n"
			"1:\n"
			"fpackreph.pi f22, f30\n"
			"fmin.s f5, f12, f31\n"
			"feq.ps f19, f3, f3\n"
			"fandi.pi f30, f20, 0x74\n"
			"LBL_SEQID_2_FP_AMO_ID_57_HID_0:\n"
			"li x10, 0x823d740370\n" 
			"la x5, mem_ptr_offset\n"
			"flw.ps f8, 704(x5)\n"
			"famomaxug.pi f26, f8 (x10)\n" 
			"divw x11, x27, x17\n"
			"fmul.ps f11, f4, f2, rne\n"
			"fmax.s f12, f31, f2\n"
			"sll x17, x17, x23\n"
			"xori x11, x27, 0x18\n"
			"LBL_SEQID_2_CSR_ID_58_HID_0:\n"
			"csrr x0, medeleg\n"
			"fminu.pi f30, f14, f16\n"
			"fsgnjx.s f6, f11, f7\n"
			"fmul.s f18, f1, f15, rne\n"
			"fsrai.pi f31, f19, 0x5\n"
			"fmul.s f29, f29, f23, rdn\n"
			"LBL_SEQID_2_INT_STORE_ID_59_HID_0:\n"
			"li x25, 0x8205e14d1c\n" 
			"sh x22, 1888(x25)\n" 
			"and x28, x10, x18\n"
			"bltu x27, x22, 1f\n"
			"1:\n"
			"divuw x28, x22, x6\n"
			"fclass.s x15, f24\n"
			"bltu x30, x18, 1f\n"
			"1:\n"
			"subw x23, x6, x23\n"
			"slli x22, x23, 3\n"
			"LBL_SEQID_2_FP_LOAD_ID_60_HID_0:\n"
			"li x12, 0x8102ae01d8\n" 
			"fsq2 f17, -866(x12)\n"
			"fbc.ps f4, -866(x12)\n" 
			"flt.ps f17, f25, f26\n"
			"maskxor m0, m1, m2\n"
			"mov.m.x m0, x0, 0xff\n"
			"bgeu x27, x18, 1f\n"
			"1:\n"
			"fsgnjx.ps f27, f13, f21\n"
			"slt x10, x10, x13\n"
			"fclass.s x5, f12\n"
			"LBL_SEQID_2_FP_AMO_ID_61_HID_0:\n"
			"li x16, 0x814977a308\n" 
			"la x5, mem_ptr_offset\n"
			"flw.ps f18, 64(x5)\n"
			"famomaxul.pi f23, f18 (x16)\n" 
			"fadd.s f3, f27, f3, rmm\n"
			"slli x17, x26, 0\n"
			"fle.ps f28, f27, f26\n"
			"bltu x16, x27, 1f\n"
			"1:\n"
			"fle.ps f3, f26, f9\n"
			"mova.x.m x17\n"
			"LBL_SEQID_2_INT_AMO_ID_62_HID_0:\n"
			"li x18, 0x824dae0000\n" 
			"amoaddl.d x25, x19, (x18)\n" 
			"fmv.s.x f1, x6\n"
			"fcmov.ps f23, f15, f23, f12\n"
			"addiw x15, x5, 0x0\n"
			"fltm.pi m4, f12, f15\n"
			"mulh x5, x15, x5\n"
			"LBL_SEQID_2_FP_L_SCATTER_ID_63_HID_0:\n"
			"li x7, 0x825d9e0320\n" 
			"la x6, mem_ptr_offset\n"
			"flq2 f25, 204(x6)\n"
			"fscbl.ps f7, f25 (x7)\n" 
			"fmsub.s f14, f14, f21, f27, rtz\n"
			"fmax.pi f17, f25, f30\n"
			"bltu x4, x9, 1f\n"
			"1:\n"
			"maskxor m4, m4, m4\n"
			"ori x13, x13, 0x14\n"
			"fence\n"
			"bltu x14, x2, 1f\n"
			"1:\n"
			"mov.m.x m0, x0, 0xFF\n"
			"li x5, 0x60\n"
			"la x31, rand_ieee754_32\n"
			"add x31, x31, x5\n"
			"flw.ps f11, 0(x31)\n"
			"flw.ps f12, 0(x31)\n"
			"flw.ps f18, 2(x31)\n"
			"flw.ps f2, 3(x31)\n"
			"flw.ps f5, 2(x31)\n"
			"flw.ps f6, 0(x31)\n"
			"flw.ps f8, 14(x31)\n"
			"li x21, 0x3\n"
			"LBL_SEQID_2_LOOP_SEQ_HID_0:\n"
			"ffrc.ps f8, f11\n"
			"bge x17, x10, 1f\n"
			"1:\n"
			"fmin.ps f8, f2, f8\n"
			"flog.ps f1, f5\n"
			"fmulh.pi f17, f8, f12\n"
			"fnot.pi f1, f8\n"
			"fsub.pi f11, f11, f11\n"
			"fnmsub.s f3, f2, f12, f11, rne\n"
			"fcmovm.ps f6, f6, f6\n"
			"bne x13, x6, 1f\n"
			"1:\n"
			"fsgnj.ps f1, f8, f12\n"
			"fcmov.ps f2, f2, f5, f11\n"
			"fcvt.ps.pwu f8, f18, rmm\n"
			"fsgnjn.s f1, f8, f11\n"
			"blt x11, x4, 1f\n"
			"1:\n"
			"fcvt.ps.pw f3, f8, rtz\n"
			"flog.ps f17, f2\n"
			"bne x6, x0, 1f\n"
			"1:\n"
			"fsra.pi f1, f18, f2\n"
			"bge x10, x31, 1f\n"
			"1:\n"
			"fmaxu.pi f17, f12, f2\n"
			"fnmadd.s f8, f8, f12, f11, rup\n"
			"fmax.ps f6, f18, f8\n"
			"fsub.pi f3, f6, f18\n"
			"blt x19, x11, 1f\n"
			"1:\n"
			"fsatu8.pi f18, f12\n"
			"flt.ps f11, f11, f2\n"
			"fsgnjn.s f11, f2, f2\n"
			"feq.ps f2, f2, f2\n"
			"ffrc.ps f11, f6\n"
			"bgeu x11, x7, 1f\n"
			"1:\n"
			"fmax.s f17, f12, f8\n"
			"fxor.pi f17, f18, f18\n"
			"fnmsub.ps f5, f5, f6, f5, rdn\n"
			"fcvt.f16.ps f18, f18\n"
			"fmul.pi f6, f6, f18\n"
			"fmulh.pi f3, f6, f12\n"
			"fmadd.s f11, f11, f12, f8, rmm\n"
			"fnmsub.s f18, f18, f18, f2, rdn\n"
			"bgeu x26, x24, 1f\n"
			"1:\n"
			"fsub.s f18, f12, f18, rtz\n"
			"feq.pi f17, f5, f11\n"
			"fmax.ps f8, f12, f2\n"
			"frsq.ps f11, f11\n"
			"bne x25, x0, 1f\n"
			"1:\n"
			"faddi.pi f5, f18, 0x74\n"
			"fsgnjx.s f17, f5, f5\n"
			"fand.pi f8, f11, f8\n"
			"fsub.s f11, f11, f6, rmm\n"
			"fmax.pi f1, f8, f5\n"
			"flt.pi f5, f18, f2\n"
			"fmin.s f11, f5, f8\n"
			"for.pi f11, f18, f11\n"
			"fmin.s f12, f12, f12\n"
			"bltu x1, x8, 1f\n"
			"1:\n"
			"fnmadd.ps f18, f11, f18, f18, rne\n"
			"beq x6, x25, 1f\n"
			"1:\n"
			"feq.pi f11, f2, f18\n"
			"bne x31, x7, 1f\n"
			"1:\n"
			"fltu.pi f3, f12, f12\n"
			"fcmovm.ps f1, f5, f6\n"
			"ffrc.ps f3, f6\n"
			"fsll.pi f5, f2, f8\n"
			"feq.ps f8, f2, f11\n"
			"fmulh.pi f1, f8, f11\n"
			"blt x10, x9, 1f\n"
			"1:\n"
			"feq.ps f2, f2, f18\n"
			"bne x15, x8, 1f\n"
			"1:\n"
			"fexp.ps f11, f11\n"
			"fmadd.ps f3, f5, f12, f12, rup\n"
			"fsrl.pi f17, f12, f18\n"
			"fnmadd.ps f11, f18, f18, f11, rmm\n"
			"fsgnj.ps f6, f6, f12\n"
			"fnmsub.s f8, f5, f18, f6, rtz\n"
			"fmadd.s f11, f11, f6, f18, rmm\n"
			"feq.ps f11, f18, f11\n"
			"fpackreph.pi f3, f5\n"
			"fmulh.pi f11, f6, f12\n"
			"fsub.ps f17, f18, f11, rdn\n"
			"bgeu x16, x16, 1f\n"
			"1:\n"
			"fsgnj.s f3, f18, f11\n"
			"blt x13, x8, 1f\n"
			"1:\n"
			"feq.ps f8, f18, f2\n"
			"fcvt.pw.ps f11, f6, rne\n"
			"fclass.ps f8, f5\n"
			"fsrli.pi f3, f5, 0x2\n"
			"beq x19, x5, 1f\n"
			"1:\n"
			"fsin.ps f2, f2\n"
			"fcvt.ps.f16 f8, f6\n"
			"beq x13, x4, 1f\n"
			"1:\n"
			"fpackrepb.pi f17, f6\n"
			"beq x25, x0, 1f\n"
			"1:\n"
			"faddi.pi f5, f5, 0xe\n"
			"fmax.pi f1, f18, f6\n"
			"fmadd.ps f12, f12, f5, f5, rmm\n"
			"fsub.s f18, f18, f11, rup\n"
			"fnmsub.ps f11, f12, f6, f11, rup\n"
			"fmulh.pi f8, f6, f5\n"
			"fmax.ps f1, f11, f6\n"
			"bne x19, x20, 1f\n"
			"1:\n"
			"flt.pi f11, f11, f2\n"
			"fmax.ps f3, f11, f12\n"
			"beq x5, x2, 1f\n"
			"1:\n"
			"fcmov.ps f12, f11, f12, f2\n"
			"fmadd.s f17, f18, f2, f8, rdn\n"
			"bne x8, x11, 1f\n"
			"1:\n"
			"frcp.ps f18, f18\n"
			"fsrl.pi f11, f6, f12\n"
			"fmul.pi f1, f8, f11\n"
			"fsgnjn.s f8, f6, f8\n"
			"flt.ps f3, f5, f5\n"
			"faddi.pi f3, f6, 0x7d\n"
			"fsgnjn.s f3, f12, f18\n"
			"fnmadd.ps f12, f12, f18, f12, rdn\n"
			"fpackrepb.pi f18, f18\n"
			"fcvt.pwu.ps f12, f2, rtz\n"
			"fadd.s f8, f12, f2, rdn\n"
			"beq x3, x27, 1f\n"
			"1:\n"
			"fmsub.s f1, f2, f5, f11, rmm\n"
			"blt x2, x11, 1f\n"
			"1:\n"
			"fandi.pi f6, f2, 0x5c\n"
			"fslli.pi f11, f18, 0x1\n"
			"frcp.ps f12, f8\n"
			"fnmadd.ps f11, f6, f5, f2, rmm\n"
			"fcvt.pwu.ps f6, f11, rmm\n"
			"fxor.pi f11, f5, f5\n"
			"fslli.pi f11, f2, 0x8\n"
			"fsll.pi f11, f8, f12\n"
			"fsgnjx.ps f6, f12, f6\n"
			"fsgnjn.s f8, f8, f11\n"
			"feq.pi f3, f6, f5\n"
			"fnmsub.ps f3, f18, f18, f5, rdn\n"
			"fmsub.ps f11, f2, f6, f11, rdn\n"
			"fsra.pi f11, f5, f11\n"
			"fand.pi f1, f5, f12\n"
			"faddi.pi f18, f18, 0x178\n"
			"blt x19, x5, 1f\n"
			"1:\n"
			"fcmov.ps f3, f12, f5, f11\n"
			"fnmsub.s f8, f5, f11, f2, rtz\n"
			"faddi.pi f2, f2, 0x8b\n"
			"fnmadd.s f5, f11, f5, f5, rtz\n"
			"fadd.pi f1, f2, f8\n"
			"frsq.ps f11, f11\n"
			"fslli.pi f3, f18, 0x7\n"
			"frcp.ps f8, f18\n"
			"fslli.pi f18, f12, 0x7\n"
			"flog.ps f1, f5\n"
			"fcvt.pwu.ps f11, f12, rup\n"
			"for.pi f3, f6, f5\n"
			"beq x13, x30, 1f\n"
			"1:\n"
			"fmax.s f17, f8, f18\n"
			"fmax.s f18, f18, f8\n"
			"fsra.pi f8, f12, f6\n"
			"fsub.ps f8, f8, f6, rmm\n"
			"fnmadd.s f17, f8, f18, f2, rup\n"
			"bge x23, x31, 1f\n"
			"1:\n"
			"fmax.ps f3, f5, f11\n"
			"bgeu x26, x11, 1f\n"
			"1:\n"
			"fmin.ps f3, f18, f8\n"
			"fsll.pi f8, f8, f11\n"
			"addi x21, x21, -1\n"
			"bne x21, x0, LBL_SEQID_2_LOOP_SEQ_HID_0\n"
			"li x5, 3\n" 
			"bne x30, x5, LBL_FAIL_HID_0\n"
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f21,  928 (x5)\n"
			"flw.ps f11,  704 (x5)\n"
			"flw.ps f19,  320 (x5)\n"
			"flw.ps f18,  992 (x5)\n"
			"flw.ps f7,  32 (x5)\n"
			"flw.ps f16,  800 (x5)\n"
			"flw.ps f20,  576 (x5)\n"
			"flw.ps f13,  384 (x5)\n"
			"flw.ps f31,  224 (x5)\n"
			"flw.ps f22,  960 (x5)\n"
			"flw.ps f3,  832 (x5)\n"
			"flw.ps f10,  448 (x5)\n"
			"flw.ps f27,  896 (x5)\n"
			"flw.ps f14,  544 (x5)\n"
			"flw.ps f0,  768 (x5)\n"
			"flw.ps f2,  416 (x5)\n"
			"flw.ps f23,  736 (x5)\n"
			"flw.ps f5,  96 (x5)\n"
			"flw.ps f29,  640 (x5)\n"
			"flw.ps f9,  192 (x5)\n"
			"flw.ps f6,  128 (x5)\n"
			"flw.ps f8,  672 (x5)\n"
			"flw.ps f25,  160 (x5)\n"
			"flw.ps f17,  288 (x5)\n"
			"flw.ps f15,  64 (x5)\n"
			"flw.ps f4,  864 (x5)\n"
			"flw.ps f1,  352 (x5)\n"
			"flw.ps f24,  480 (x5)\n"
			"flw.ps f26,  0 (x5)\n"
			"flw.ps f28,  512 (x5)\n"
			"flw.ps f30,  608 (x5)\n"
			"flw.ps f12,  256 (x5)\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_3_FP_AMO_ID_64_HID_0:\n"
			"li x27, 0x8256dc0660\n" 
			"la x22, mem_ptr_offset\n"
			"flw.ps f21, 96(x22)\n"
			"famominl.ps f12, f21 (x27)\n" 
			"feq.pi f6, f4, f11\n"
			"bne x24, x11, 1f\n"
			"1:\n"
			"fsgnjx.ps f6, f10, f31\n"
			"fnmsub.s f6, f4, f4, f14, rdn\n"
			"LBL_SEQID_3_FP_SCATTER_ID_65_HID_0:\n"
			"li x17, 0x8106da053d\n" 
			"la x23, mem_ptr_offset\n"
			"flq2 f0, 192(x23)\n"
			"fscb.ps f29, f0 (x17)\n" 
			"fmax.ps f28, f19, f19\n"
			"fmadd.s f10, f25, f4, f10, rmm\n"
			"fsgnjx.ps f30, f30, f31\n"
			"LBL_SEQID_3_FP_G_GATHER_ID_66_HID_0:\n"
			"li x13, 0x8226dc0660\n" 
			"la x12, mem_ptr_offset\n"
			"flq2 f2, 0(x12)\n"
			"fscwg.ps f14, f2 (x13)\n"
			"fghg.ps f27, f2 (x13)\n" 
			"fsgnjx.ps f23, f18, f18\n"
			"fxor.pi f18, f24, f18\n"
			"fand.pi f14, f24, f2\n"
			"LBL_SEQID_3_INT_AMO_ID_67_HID_0:\n"
			"li x17, 0x814c7c8ce8\n" 
			"amoandl.d x25, x7, (x17)\n" 
			"sltiu x6, x10, 0x2\n"
			"fadd.s f2, f5, f5, rne\n"
			"bgeu x3, x16, 1f\n"
			"1:\n"
			"fltm.ps m3, f2, f12\n"
			"LBL_SEQID_3_FP_LOAD_ID_68_HID_0:\n"
			"li x12, 0x8107740031\n" 
			"fsw.ps f17, 985(x12)\n"
			"flw f24, 985(x12)\n" 
			"fmulhu.pi f2, f28, f23\n"
			"blt x23, x9, 1f\n"
			"1:\n"
			"fsra.pi f31, f26, f31\n"
			"fsrl.pi f26, f15, f10\n"
			"bne x20, x11, 1f\n"
			"1:\n"
			"LBL_SEQID_3_INT_AMO_ID_69_HID_0:\n"
			"li x17, 0x823b8c0140\n" 
			"amomaxug.d x10, x9, (x17)\n" 
			"fmulh.pi f5, f29, f9\n"
			"fsgnjx.ps f13, f30, f13\n"
			"fmul.pi f13, f11, f28\n"
			"LBL_SEQID_3_INT_AMO_ID_70_HID_0:\n"
			"li x28, 0x813722c830\n" 
			"amomaxug.d x11, x23, (x28)\n" 
			"fsgnjx.ps f24, f15, f13\n"
			"fsgnj.ps f14, f15, f14\n"
			"beq x8, x24, 1f\n"
			"1:\n"
			"fle.pi f18, f15, f14\n"
			"LBL_SEQID_3_FP_G_SCATTER_ID_71_HID_0:\n"
			"li x11, 0x8230820018\n" 
			"la x7, mem_ptr_offset\n"
			"flq2 f3, 212(x7)\n"
			"fscbg.ps f8, f3 (x11)\n" 
			"feq.s x5, f22, f9\n"
			"fmvz.x.ps x16, f25, 2\n"
			"fsgnj.s f6, f17, f1\n"
			"LBL_SEQID_3_INT_AMO_ID_72_HID_0:\n"
			"li x7, 0x815a832918\n" 
			"amoandl.d x18, x5, (x7)\n" 
			"faddi.pi f30, f0, 0x22\n"
			"frsq.ps f20, f23\n"
			"for.pi f10, f19, f22\n"
			"bge x24, x26, 1f\n"
			"1:\n"
			"LBL_SEQID_3_INT_AMO_ID_73_HID_0:\n"
			"li x15, 0x824abcd818\n" 
			"amoaddl.d x22, x14, (x15)\n" 
			"fle.s x10, f21, f21\n"
			"fpackrepb.pi f27, f15\n"
			"flog.ps f1, f26\n"
			"LBL_SEQID_3_INT_AMO_ID_74_HID_0:\n"
			"li x28, 0x825d740370\n" 
			"amoxorl.w x5, x16, (x28)\n" 
			"fmaxu.pi f6, f10, f18\n"
			"fsgnjx.s f15, f19, f19\n"
			"beq x0, x21, 1f\n"
			"1:\n"
			"fmax.ps f27, f10, f27\n"
			"LBL_SEQID_3_FP_L_STORE_ID_75_HID_0:\n"
			"li x28, 0x815b34e300\n" 
			"mov.m.x m0, zero, 0xff\n"
			"fswl.ps f1, (x28)\n" 
			"fsgnj.ps f12, f28, f8\n"
			"flt.pi f7, f16, f8\n"
			"flt.pi f8, f16, f3\n"
			"LBL_SEQID_3_INT_AMO_ID_76_HID_0:\n"
			"li x17, 0x8132210020\n" 
			"amomaxg.d x18, x16, (x17)\n" 
			"fsgnjn.s f4, f5, f4\n"
			"bge x29, x20, 1f\n"
			"1:\n"
			"fslli.pi f26, f14, 0x4\n"
			"fnmsub.ps f21, f6, f29, f20, rne\n"
			"LBL_SEQID_3_INT_AMO_ID_77_HID_0:\n"
			"li x5, 0x815fed3ae0\n" 
			"amoandl.w x14, x19, (x5)\n" 
			"frsq.ps f6, f29\n"
			"fsll.pi f10, f10, f29\n"
			"mulh x7, x9, x12\n"
			"blt x28, x6, 1f\n"
			"1:\n"
			"LBL_SEQID_3_CSR_ID_78_HID_0:\n"
			"csrr x0, mhpmevent8\n"
			"or x11, x13, x25\n"
			"or x14, x19, x25\n"
			"flem.ps m7, f17, f12\n"
			"LBL_SEQID_3_FP_AMO_ID_79_HID_0:\n"
			"li x12, 0x825bfc0030\n" 
			"la x23, mem_ptr_offset\n"
			"flw.ps f29, 0(x23)\n"
			"famoorl.pi f22, f29 (x12)\n" 
			"fadd.ps f12, f12, f0, rne\n"
			"fmsub.ps f14, f30, f30, f2, rdn\n"
			"maskxor m0, m0, m0\n"
			"bgeu x2, x29, 1f\n"
			"1:\n"
			"LBL_SEQID_3_INT_G_STORE_ID_80_HID_0:\n"
			"li x28, 0x81257c003c\n" 
			"shg x10, (x28)\n" 
			"fmin.ps f19, f10, f23\n"
			"blt x22, x17, 1f\n"
			"1:\n"
			"fmsub.ps f19, f23, f6, f6, rmm\n"
			"fmax.ps f23, f23, f20\n"
			"blt x30, x4, 1f\n"
			"1:\n"
			"LBL_SEQID_3_FP_GATHER_ID_81_HID_0:\n"
			"li x20, 0x820922ddee\n" 
			"la x13, mem_ptr_offset\n"
			"flw.ps f1, 212(x13)\n"
			"fscw.ps f6, f1 (x20)\n"
			"fgb.ps f24, f1 (x20)\n" 
			"fxor.pi f7, f21, f6\n"
			"fcmov.ps f20, f13, f13, f20\n"
			"bgeu x16, x17, 1f\n"
			"1:\n"
			"fnmadd.ps f2, f7, f24, f19, rne\n"
			"LBL_SEQID_3_FP_SCATTER_ID_82_HID_0:\n"
			"li x15, 0x820c08cb7e\n" 
			"la x19, mem_ptr_offset\n"
			"flq2 f1, 24(x19)\n"
			"fscw.ps f8, f1 (x15)\n" 
			"fsgnj.s f25, f25, f3\n"
			"fand.pi f19, f2, f19\n"
			"flt.ps f23, f1, f24\n"
			"LBL_SEQID_3_FP_LOAD_ID_83_HID_0:\n"
			"li x25, 0x820401f2a7\n" 
			"fsw.ps f1, 963(x25)\n"
			"flw f7, 963(x25)\n" 
			"fpackreph.pi f10, f30\n"
			"fsatu8.pi f24, f4\n"
			"fsgnjn.s f0, f22, f25\n"
			"bge x9, x11, 1f\n"
			"1:\n"
			"LBL_SEQID_3_FP_LOAD_ID_84_HID_0:\n"
			"li x5, 0x820c7e003d\n" 
			"fsw.ps f20, 1944(x5)\n"
			"fbc.ps f0, 1944(x5)\n" 
			"ebreak\n"
			"fmulhu.pi f3, f3, f31\n"
			"remw x27, x13, x10\n"
			"bltu x2, x0, 1f\n"
			"1:\n"
			"LBL_SEQID_3_FP_STORE_ID_85_HID_0:\n"
			"li x7, 0x810b7c0020\n" 
			"fsw f5, -706(x7)\n" 
			"fmvz.x.ps x23, f24, 5\n"
			"feq.pi f20, f14, f11\n"
			"rem x27, x18, x6\n"
			"la x11, rand_int_32\n"
			"flw.ps f4, 416(x11)\n"
			"flw.ps f30, 896(x11)\n"
			"fdiv.pi f18, f4, f30\n" 
			"fxor.pi f26, f1, f22\n"
			"fsra.pi f18, f18, f28\n"
			"fsgnjx.s f16, f23, f21\n"
			"LBL_SEQID_3_INT_AMO_ID_86_HID_0:\n"
			"li x14, 0x814fec06d8\n" 
			"amoxorl.d x15, x19, (x14)\n" 
			"fsgnj.ps f0, f1, f12\n"
			"fsgnj.ps f29, f29, f0\n"
			"fmax.s f11, f11, f3\n"
			"LBL_SEQID_3_CSR_ID_87_HID_0:\n"
			"li x17, 0x8000008208d1b802\n"
			"li x31, (0x2 << 6) | 0x0\n"
			"csrw unlock_va, x17\n" 
			"csrwi tensor_wait, 6\n" 
			"fandi.pi f16, f3, 0xe0\n"
			"bge x11, x2, 1f\n"
			"1:\n"
			"fmul.pi f6, f6, f29\n"
			"frcp.ps f1, f7\n"
			"LBL_SEQID_3_FP_SCATTER_ID_88_HID_0:\n"
			"li x26, 0x820e12f6d7\n" 
			"li x18, 0x5b5c80ddf7e0b8b7\n"
			"fsc32h.ps f28, x18 (x26)\n" 
			"sllw x11, x0, x18\n"
			"srli x6, x28, 5\n"
			"bltu x16, x14, 1f\n"
			"1:\n"
			"fmax.pi f9, f0, f28\n"
			"beq x13, x15, 1f\n"
			"1:\n"
			"LBL_SEQID_3_FP_AMO_ID_89_HID_0:\n"
			"li x26, 0x812f1a000c\n" 
			"la x12, mem_ptr_offset\n"
			"flw.ps f22, 32(x12)\n"
			"famominug.pi f28, f22 (x26)\n" 
			"fmul.s f21, f7, f21, rdn\n"
			"fcvt.pwu.ps f13, f13, rdn\n"
			"bltu x22, x1, 1f\n"
			"1:\n"
			"fsrai.pi f0, f28, 0x5\n"
			"LBL_SEQID_3_INT_AMO_ID_90_HID_0:\n"
			"li x16, 0x824abcd81c\n" 
			"amomaxul.w x10, x10, (x16)\n" 
			"fcmovm.ps f16, f22, f22\n"
			"fmulhu.pi f3, f23, f3\n"
			"fnmsub.ps f24, f5, f28, f28, rmm\n"
			"LBL_SEQID_3_CSR_ID_91_HID_0:\n"
			"li x11, 0x8000008104d9f804\n"
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw unlock_va, x11\n" 
			"csrwi tensor_wait, 6\n" 
			"divuw x5, x12, x5\n"
			"fcmovm.ps f13, f19, f14\n"
			"beq x2, x1, 1f\n"
			"1:\n"
			"divuw x26, x15, x20\n"
			"LBL_SEQID_3_INT_AMO_ID_92_HID_0:\n"
			"li x28, 0x8241c4bb64\n" 
			"amominul.w x6, x12, (x28)\n" 
			"fmvs.x.ps x26, f19, 2\n"
			"fmv.x.s x19, f21\n"
			"fbci.ps f6, 0x795a6\n"
			"LBL_SEQID_3_INT_AMO_ID_93_HID_0:\n"
			"li x20, 0x822e020350\n" 
			"amomaxg.d x14, x23, (x20)\n" 
			"fadd.ps f31, f7, f0, rtz\n"
			"fsgnj.s f6, f6, f7\n"
			"fle.ps f6, f13, f0\n"
			"LBL_SEQID_3_FP_G_LOAD_ID_94_HID_0:\n"
			"li x17, 0x8125ba0020\n" 
			"mov.m.x m0, zero, 0xff\n"
			"fswg.ps f31, (x17)\n"
			"flwg.ps f14, (x17)\n" 
			"slt x17, x18, x11\n"
			"fxor.pi f22, f17, f22\n"
			"subw x0, x0, x20\n"
			"bgeu x15, x12, 1f\n"
			"1:\n"
			"mov.m.x m0, x0, 0xFF\n"
			"li x5, 0x40\n"
			"la x31, rand_ieee754_32\n"
			"add x31, x31, x5\n"
			"flw.ps f16, 0(x31)\n"
			"flw.ps f17, 2(x31)\n"
			"flw.ps f19, 1(x31)\n"
			"flw.ps f20, 6(x31)\n"
			"flw.ps f26, 16(x31)\n"
			"flw.ps f30, 2(x31)\n"
			"li x21, 0x2\n"
			"LBL_SEQID_3_LOOP_SEQ_HID_0:\n"
			"fsatu8.pi f18, f16\n"
			"fadd.pi f18, f26, f20\n"
			"fmin.pi f30, f17, f30\n"
			"fsub.s f29, f26, f30, rdn\n"
			"fsub.pi f30, f17, f19\n"
			"bltu x2, x9, 1f\n"
			"1:\n"
			"fnmsub.ps f30, f20, f26, f17, rne\n"
			"fmax.ps f10, f17, f30\n"
			"fsub.pi f30, f17, f17\n"
			"fsin.ps f12, f17\n"
			"fsrai.pi f12, f16, 0xd\n"
			"fmadd.s f16, f26, f26, f16, rne\n"
			"fnmadd.ps f12, f17, f20, f30, rtz\n"
			"fcvt.f16.ps f12, f17\n"
			"fsub.pi f18, f20, f19\n"
			"fcvt.pwu.ps f31, f19, rne\n"
			"bgeu x23, x28, 1f\n"
			"1:\n"
			"fexp.ps f30, f19\n"
			"ffrc.ps f10, f30\n"
			"beq x12, x14, 1f\n"
			"1:\n"
			"fnmadd.s f18, f26, f30, f26, rmm\n"
			"fmin.s f20, f20, f26\n"
			"bgeu x3, x12, 1f\n"
			"1:\n"
			"fsgnjn.s f12, f30, f19\n"
			"fsgnj.ps f30, f30, f30\n"
			"fsgnjn.ps f30, f20, f17\n"
			"fswizz.ps f18, f19, 0x16\n"
			"fcvt.ps.f16 f31, f20\n"
			"beq x9, x1, 1f\n"
			"1:\n"
			"fmin.ps f12, f30, f19\n"
			"fsgnjx.ps f29, f30, f26\n"
			"bltu x13, x28, 1f\n"
			"1:\n"
			"fsgnjx.ps f10, f17, f26\n"
			"frcp.ps f12, f30\n"
			"fsgnjn.ps f31, f16, f30\n"
			"blt x5, x22, 1f\n"
			"1:\n"
			"fnot.pi f10, f30\n"
			"fpackreph.pi f20, f30\n"
			"frsq.ps f12, f30\n"
			"fnmadd.s f20, f17, f20, f20, rne\n"
			"fmaxu.pi f19, f17, f19\n"
			"fsin.ps f18, f17\n"
			"fsgnjn.ps f19, f16, f19\n"
			"fmadd.ps f30, f16, f17, f20, rtz\n"
			"fclass.ps f10, f30\n"
			"fsrli.pi f30, f30, 0x5\n"
			"frcp.ps f30, f30\n"
			"beq x0, x17, 1f\n"
			"1:\n"
			"fpackrepb.pi f30, f30\n"
			"fltu.pi f12, f16, f17\n"
			"bltu x30, x27, 1f\n"
			"1:\n"
			"ffrc.ps f17, f16\n"
			"fsin.ps f12, f17\n"
			"blt x17, x15, 1f\n"
			"1:\n"
			"frsq.ps f29, f26\n"
			"blt x25, x18, 1f\n"
			"1:\n"
			"fcvt.ps.pw f29, f30, rne\n"
			"fxor.pi f30, f20, f30\n"
			"bgeu x24, x29, 1f\n"
			"1:\n"
			"fmax.ps f19, f16, f19\n"
			"fsat8.pi f10, f17\n"
			"fnot.pi f29, f26\n"
			"fsub.s f29, f17, f26, rdn\n"
			"for.pi f10, f26, f17\n"
			"fnmadd.s f26, f26, f20, f26, rmm\n"
			"fsrai.pi f10, f19, 0x1\n"
			"fmin.ps f30, f19, f30\n"
			"flt.ps f30, f30, f17\n"
			"beq x23, x29, 1f\n"
			"1:\n"
			"flog.ps f30, f17\n"
			"for.pi f10, f20, f30\n"
			"feq.pi f31, f30, f30\n"
			"fsub.ps f30, f20, f26, rne\n"
			"fsgnjn.ps f17, f20, f16\n"
			"fexp.ps f30, f20\n"
			"fmul.s f12, f26, f20, rmm\n"
			"fsub.pi f12, f17, f26\n"
			"fsub.ps f31, f26, f26, rtz\n"
			"feq.pi f17, f17, f17\n"
			"fadd.pi f12, f26, f19\n"
			"bge x16, x5, 1f\n"
			"1:\n"
			"fcmov.ps f29, f20, f30, f19\n"
			"fminu.pi f18, f17, f30\n"
			"bltu x17, x3, 1f\n"
			"1:\n"
			"fmax.ps f31, f26, f17\n"
			"fmin.pi f16, f19, f16\n"
			"fmadd.ps f31, f26, f26, f30, rup\n"
			"for.pi f19, f16, f19\n"
			"bge x31, x7, 1f\n"
			"1:\n"
			"fsgnjx.s f17, f17, f26\n"
			"fsrli.pi f29, f20, 0x2\n"
			"faddi.pi f31, f30, 0x1bf\n"
			"beq x11, x9, 1f\n"
			"1:\n"
			"fadd.s f29, f16, f20, rtz\n"
			"beq x11, x7, 1f\n"
			"1:\n"
			"fmulhu.pi f31, f19, f26\n"
			"blt x8, x25, 1f\n"
			"1:\n"
			"fpackrepb.pi f31, f26\n"
			"flt.pi f18, f20, f16\n"
			"bgeu x27, x10, 1f\n"
			"1:\n"
			"feq.pi f16, f30, f19\n"
			"fcvt.ps.pw f30, f16, rup\n"
			"feq.ps f30, f26, f20\n"
			"fmax.pi f17, f17, f16\n"
			"blt x25, x2, 1f\n"
			"1:\n"
			"fadd.pi f26, f26, f30\n"
			"fmax.ps f18, f20, f19\n"
			"fle.ps f20, f20, f30\n"
			"fsin.ps f18, f19\n"
			"bgeu x12, x26, 1f\n"
			"1:\n"
			"fle.ps f30, f20, f17\n"
			"fclass.ps f12, f19\n"
			"ffrc.ps f17, f20\n"
			"bne x25, x0, 1f\n"
			"1:\n"
			"fmsub.ps f16, f19, f16, f30, rdn\n"
			"fsra.pi f16, f19, f16\n"
			"fsatu8.pi f29, f17\n"
			"fmadd.s f29, f20, f19, f17, rmm\n"
			"fnmadd.ps f12, f26, f17, f16, rup\n"
			"beq x7, x8, 1f\n"
			"1:\n"
			"ffrc.ps f16, f17\n"
			"flog.ps f31, f16\n"
			"fmsub.s f18, f30, f20, f19, rtz\n"
			"blt x27, x30, 1f\n"
			"1:\n"
			"for.pi f19, f17, f19\n"
			"fmulhu.pi f30, f20, f19\n"
			"bltu x29, x3, 1f\n"
			"1:\n"
			"fsgnjx.ps f30, f16, f30\n"
			"fpackrepb.pi f30, f16\n"
			"fmadd.ps f29, f16, f19, f26, rne\n"
			"fmsub.ps f30, f19, f26, f19, rtz\n"
			"fsub.pi f31, f30, f16\n"
			"beq x20, x24, 1f\n"
			"1:\n"
			"fmadd.ps f30, f19, f17, f20, rne\n"
			"fsin.ps f19, f19\n"
			"beq x17, x28, 1f\n"
			"1:\n"
			"fcvt.ps.f16 f18, f17\n"
			"bgeu x13, x6, 1f\n"
			"1:\n"
			"fnmsub.ps f10, f16, f20, f20, rne\n"
			"fround.ps f29, f20, rup\n"
			"fand.pi f20, f26, f20\n"
			"fle.ps f12, f26, f20\n"
			"fmulhu.pi f30, f30, f20\n"
			"fmax.pi f19, f20, f26\n"
			"fround.ps f30, f16, rmm\n"
			"beq x23, x7, 1f\n"
			"1:\n"
			"fclass.ps f19, f19\n"
			"bltu x3, x0, 1f\n"
			"1:\n"
			"fmulh.pi f10, f20, f19\n"
			"fsgnjx.s f12, f16, f16\n"
			"bgeu x3, x11, 1f\n"
			"1:\n"
			"fmax.s f12, f26, f17\n"
			"fsrai.pi f17, f30, 0x1\n"
			"fsgnjn.s f29, f26, f20\n"
			"feq.ps f20, f20, f17\n"
			"flog.ps f12, f26\n"
			"bge x1, x4, 1f\n"
			"1:\n"
			"fnmsub.ps f30, f17, f19, f17, rup\n"
			"fmulh.pi f18, f16, f16\n"
			"fcmov.ps f10, f20, f26, f30\n"
			"fsrli.pi f20, f20, 0x7\n"
			"bltu x30, x10, 1f\n"
			"1:\n"
			"fmulh.pi f31, f26, f17\n"
			"fcvt.pwu.ps f29, f26, rtz\n"
			"beq x5, x26, 1f\n"
			"1:\n"
			"fnot.pi f29, f16\n"
			"ffrc.ps f18, f30\n"
			"fcvt.pwu.ps f30, f17, rtz\n"
			"for.pi f30, f30, f20\n"
			"fcvt.f16.ps f17, f16\n"
			"bne x12, x21, 1f\n"
			"1:\n"
			"fnot.pi f31, f17\n"
			"fsub.s f26, f19, f26, rdn\n"
			"bgeu x21, x2, 1f\n"
			"1:\n"
			"fmax.pi f10, f19, f19\n"
			"fnmsub.ps f30, f26, f16, f26, rtz\n"
			"bgeu x26, x26, 1f\n"
			"1:\n"
			"fsgnjx.ps f31, f16, f20\n"
			"faddi.pi f30, f16, 0x17b\n"
			"fnmadd.ps f30, f30, f19, f30, rup\n"
			"bltu x29, x15, 1f\n"
			"1:\n"
			"feq.pi f26, f20, f26\n"
			"bne x7, x21, 1f\n"
			"1:\n"
			"fle.pi f26, f17, f26\n"
			"bltu x10, x25, 1f\n"
			"1:\n"
			"fmax.s f16, f30, f16\n"
			"bgeu x3, x2, 1f\n"
			"1:\n"
			"fnmsub.ps f20, f20, f16, f20, rne\n"
			"blt x12, x19, 1f\n"
			"1:\n"
			"fcvt.ps.pw f18, f20, rdn\n"
			"fsrli.pi f30, f16, 0x4\n"
			"fcvt.ps.pwu f30, f26, rup\n"
			"fsgnj.s f16, f30, f16\n"
			"fnmsub.ps f18, f30, f17, f20, rmm\n"
			"beq x3, x13, 1f\n"
			"1:\n"
			"fexp.ps f12, f19\n"
			"bltu x19, x18, 1f\n"
			"1:\n"
			"fcvt.ps.f16 f12, f30\n"
			"fmul.ps f29, f30, f30, rne\n"
			"fand.pi f10, f20, f26\n"
			"bne x29, x21, 1f\n"
			"1:\n"
			"fexp.ps f31, f19\n"
			"feq.pi f20, f20, f26\n"
			"fmul.ps f18, f20, f20, rmm\n"
			"ffrc.ps f31, f20\n"
			"beq x2, x0, 1f\n"
			"1:\n"
			"fsgnjx.s f30, f26, f17\n"
			"fslli.pi f30, f26, 0xd\n"
			"fsgnjn.s f26, f16, f20\n"
			"fsgnjx.s f31, f30, f26\n"
			"blt x19, x2, 1f\n"
			"1:\n"
			"fpackrepb.pi f20, f16\n"
			"bne x24, x9, 1f\n"
			"1:\n"
			"fsrli.pi f31, f17, 0x3\n"
			"fsrai.pi f30, f19, 0x4\n"
			"fcmov.ps f18, f20, f16, f20\n"
			"bltu x27, x12, 1f\n"
			"1:\n"
			"fsll.pi f18, f16, f26\n"
			"addi x21, x21, -1\n"
			"bne x21, x0, LBL_SEQID_3_LOOP_SEQ_HID_0\n"
			"li x5, 4\n" 
			"bne x30, x5, LBL_FAIL_HID_0\n"
			"addi x30, x30, 1\n"
			"la x10, rand_ieee754_32\n"
			"flw.ps f16, 128(x10)\n"
			"flw.ps f1, 928(x10)\n"
			"fle.s x15, f16, f1\n" 
			"fcvt.s.wu f0, x19, rne\n"
			"fence\n" 
			"fmin.pi f16, f2, f16\n"
			"la x22, rand_ieee754_32\n"
			"flw.ps f1, 672(x22)\n"
			"flw.ps f16, 864(x22)\n"
			"fadd.s f17, f1, f16, rne\n" 
			"and x13, x7, x6\n"
			"la x22, rand_int_32\n"
			"flw.ps f8, 96(x22)\n"
			"fsrli.pi f23, f8, 0x2\n" 
			"ecall\n"
			"la x10, rand_ieee754_32\n"
			"flw.ps f26, 640(x10)\n"
			"flw.ps f9, 512(x10)\n"
			"fmax.s f28, f26, f9\n" 
			"ebreak\n"
			"la x27, rand_ieee754_32\n"
			"flw.ps f6, 960(x27)\n"
			"fmv.x.s x13, f6\n" 
			"fcvt.f16.ps f19, f19\n"
			"la x10, rand_ieee754_32\n"
			"flw.ps f1, 192(x10)\n"
			"fsin.ps f15, f1\n" 
			"or x16, x23, x0\n"
			"li x18, 0x34392a6269fbdf3c\n"
			"li x25, 0x21220a9579eb3f96\n"
			"or x12, x18, x25\n" 
			"add x19, x18, x19\n"
			"blt x4, x11, 1f\n"
			"1:\n"
			"la x12, rand_int_32\n"
			"flw.ps f9, 992(x12)\n"
			"flw.ps f29, 864(x12)\n"
			"fle.pi f29, f9, f29\n" 
			"fadd.ps f5, f4, f18, rdn\n"
			"LBL_SEQID_4_M0_WRITE_ID_95_HID_0:\n"
			"flem.ps m5, f25, f22\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"fmsub.s f5, f14, f6, f19, rup\n"
			"bne x10, x9, 1f\n"
			"1:\n"
			"li x28, 0xb56790657c7079aa\n"
			"li x27, 0xdafa3d8c1b7c5f65\n"
			"mulhu x26, x28, x27\n" 
			"sraw x14, x17, x16\n"
			"li x26, 0x51941618b72c391\n"
			"slliw x15, x26, 2\n" 
			"fmulh.pi f25, f4, f21\n"
			"li x18, 0xbaeb2266a0e6a020\n"
			"li x19, 0xffa0492b960af30b\n"
			"mulhu x12, x18, x19\n" 
			"fminu.pi f2, f9, f7\n"
			"li x19, 0x354ea6c19d7bb55f\n"
			"slli x26, x19, 4\n" 
			"packb x13, x9, x22\n"
			"la x12, rand_ieee754_32\n"
			"flw.ps f2, 480(x12)\n"
			"flw.ps f5, 0(x12)\n"
			"fsgnjx.s f22, f2, f5\n" 
			"divw x19, x19, x5\n"
			"la x13, rand_int_32\n"
			"flw.ps f21, 736(x13)\n"
			"flw.ps f11, 896(x13)\n"
			"fmulh.pi f30, f21, f11\n" 
			"fclass.s x16, f12\n"
			"la x5, rand_ieee754_32\n"
			"flw.ps f23, 480(x5)\n"
			"flw.ps f25, 544(x5)\n"
			"fadd.ps f20, f23, f25, rmm\n" 
			"add x12, x20, x12\n"
			"la x6, rand_ieee754_32\n"
			"flw.ps f19, 416(x6)\n"
			"flw.ps f29, 544(x6)\n"
			"flw.ps f3, 896(x6)\n"
			"fmsub.s f17, f19, f29, f3, rdn\n" 
			"feq.ps f3, f3, f24\n"
			"la x14, rand_int_32\n"
			"flw.ps f8, 544(x14)\n"
			"fpackreph.pi f9, f8\n" 
			"or x12, x12, x15\n"
			"beq x18, x27, 1f\n"
			"1:\n"
			"la x15, rand_int_32\n"
			"flw.ps f7, 928(x15)\n"
			"fsrai.pi f30, f7, 0x8\n" 
			"fminu.pi f9, f13, f2\n"
			"la x14, rand_int_32\n"
			"flw.ps f4, 736(x14)\n"
			"flw.ps f19, 896(x14)\n"
			"fle.pi f23, f4, f19\n" 
			"maskpopcz x9, m4\n"
			"la x19, rand_int_32\n"
			"flw.ps f26, 224(x19)\n"
			"flw.ps f18, 192(x19)\n"
			"fmulh.pi f29, f26, f18\n" 
			"subw x18, x26, x28\n"
			"LBL_SEQID_4_M0_WRITE_ID_96_HID_0:\n"
			"mov.m.x m2, x23, 0x1a\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"fsrai.pi f0, f24, 0x3\n"
			"la x10, rand_ieee754_32\n"
			"flw.ps f7, 384(x10)\n"
			"flw.ps f10, 736(x10)\n"
			"fsgnjx.s f17, f7, f10\n" 
			"fcmovm.ps f15, f3, f31\n"
			"bgeu x6, x20, 1f\n"
			"1:\n"
			"li x10, 0x92d9ea318baad485\n"
			"li x23, 0xec040273ff787906\n"
			"slt x20, x10, x23\n" 
			"srliw x15, x6, 5\n"
			"la x26, rand_ieee754_32\n"
			"flw.ps f5, 672(x26)\n"
			"flw.ps f8, 544(x26)\n"
			"fsgnj.ps f25, f5, f8\n" 
			"fmul.pi f14, f23, f27\n"
			"la x10, rand_ieee754_32\n"
			"flw.ps f0, 928(x10)\n"
			"flw.ps f31, 160(x10)\n"
			"flw.ps f3, 544(x10)\n"
			"fmsub.s f24, f0, f31, f3, rne\n" 
			"slliw x12, x12, 3\n"
			"li x18, 0x7dda883ab72ac970\n"
			"addiw x9, x18, 0x1f\n" 
			"fle.pi f12, f15, f15\n"
			"la x27, rand_ieee754_32\n"
			"flw.ps f31, 992(x27)\n"
			"flw.ps f18, 960(x27)\n"
			"fadd.ps f2, f31, f18, rup\n" 
			"feq.ps f2, f25, f13\n"
			"li x6, 0xab366205a19fcea\n"
			"li x27, 0x8100fe5fae6c83e7\n"
			"slt x26, x6, x27\n" 
			"fsgnjn.ps f19, f4, f17\n"
			"li x28, 0xa519d60de2b39f78\n"
			"li x15, 0x9e38023dc8093c2a\n"
			"srlw x15, x28, x15\n" 
			"fcvt.ps.pw f15, f24, rne\n"
			"li x12, 0x39effb38640c75e3\n"
			"li x22, 0xb5562aeabb9e3107\n"
			"sraw x14, x12, x22\n" 
			"or x0, x12, x0\n"
			"mov.m.x m0, x0, 0xFF\n"
			"li x5, 0x28\n"
			"la x31, rand_ieee754_32\n"
			"add x31, x31, x5\n"
			"flw.ps f0, 0(x31)\n"
			"flw.ps f12, 3(x31)\n"
			"flw.ps f15, 2(x31)\n"
			"flw.ps f2, 0(x31)\n"
			"flw.ps f20, 8(x31)\n"
			"flw.ps f22, 11(x31)\n"
			"flw.ps f28, 20(x31)\n"
			"flw.ps f30, 13(x31)\n"
			"flw.ps f31, 21(x31)\n"
			"flw.ps f4, 28(x31)\n"
			"flw.ps f6, 26(x31)\n"
			"flw.ps f9, 21(x31)\n"
			"la x31, rand_int_32\n"
			"add x31, x31, x5\n"
			"lw x10, 0(x31)\n"
			"lw x12, 0(x31)\n"
			"lw x15, 5(x31)\n"
			"lw x17, 4(x31)\n"
			"lw x19, 11(x31)\n"
			"lw x23, 19(x31)\n"
			"lw x25, 15(x31)\n"
			"lw x26, 27(x31)\n"
			"lw x9, 7(x31)\n"
			"li x21, 0x1\n"
			"LBL_SEQID_4_LOOP_SEQ_HID_0:\n"
			"faddi.pi f31, f2, 0x192\n"
			"fsrai.pi f27, f20, 0xd\n"
			"subw x12, x17, x26\n"
			"addi x9, x26, 0x16\n"
			"fcvt.s.wu f4, x10, rup\n"
			"fadd.s f22, f2, f22, rup\n"
			"divw x19, x12, x25\n"
			"fcmovm.ps f15, f0, f12\n"
			"fminu.pi f6, f0, f6\n"
			"fcvt.ps.f16 f7, f31\n"
			"fnmsub.s f22, f30, f2, f22, rne\n"
			"subw x14, x17, x19\n"
			"bge x2, x11, 1f\n"
			"1:\n"
			"fsgnjn.s f30, f2, f15\n"
			"bne x7, x23, 1f\n"
			"1:\n"
			"fadd.s f4, f31, f2, rup\n"
			"bgeu x6, x8, 1f\n"
			"1:\n"
			"sllw x19, x15, x23\n"
			"bne x8, x22, 1f\n"
			"1:\n"
			"or x23, x23, x9\n"
			"fcvt.f16.ps f27, f31\n"
			"fsgnjn.ps f0, f28, f28\n"
			"slt x9, x10, x9\n"
			"blt x16, x23, 1f\n"
			"1:\n"
			"fnmsub.s f27, f31, f4, f15, rne\n"
			"fcvt.s.wu f7, x15, rne\n"
			"masknot m0, m0\n"
			"blt x26, x29, 1f\n"
			"1:\n"
			"feq.ps f7, f15, f31\n"
			"addi x26, x26, 0xe\n"
			"add x26, x15, x26\n"
			"add x23, x19, x23\n"
			"masknot m0, m5\n"
			"fmsub.ps f22, f22, f0, f22, rne\n"
			"bgeu x5, x5, 1f\n"
			"1:\n"
			"fmul.pi f17, f22, f9\n"
			"bge x18, x28, 1f\n"
			"1:\n"
			"fsgnjn.s f15, f20, f31\n"
			"packb x19, x10, x23\n"
			"fcvt.s.wu f22, x26, rtz\n"
			"fmsub.s f11, f30, f9, f4, rup\n"
			"packb x9, x19, x17\n"
			"fcvt.ps.f16 f4, f9\n"
			"addw x7, x23, x17\n"
			"srliw x23, x9, 4\n"
			"fsra.pi f7, f28, f31\n"
			"slt x12, x19, x12\n"
			"blt x12, x7, 1f\n"
			"1:\n"
			"subw x6, x19, x15\n"
			"subw x9, x9, x19\n"
			"xori x7, x9, 0xa\n"
			"fmax.s f3, f30, f2\n"
			"fmax.pi f17, f12, f4\n"
			"bgeu x15, x20, 1f\n"
			"1:\n"
			"divw x15, x15, x10\n"
			"fmax.pi f12, f22, f12\n"
			"srliw x12, x10, 2\n"
			"masknot m6, m4\n"
			"fcvt.ps.f16 f3, f28\n"
			"blt x19, x20, 1f\n"
			"1:\n"
			"fmsub.ps f15, f30, f0, f9, rdn\n"
			"bne x14, x17, 1f\n"
			"1:\n"
			"slt x7, x26, x12\n"
			"fnot.pi f12, f6\n"
			"srliw x12, x9, 2\n"
			"add x7, x10, x23\n"
			"fcvt.ps.f16 f7, f0\n"
			"bge x13, x31, 1f\n"
			"1:\n"
			"feq.ps f2, f0, f6\n"
			"blt x23, x25, 1f\n"
			"1:\n"
			"fmsub.s f3, f4, f2, f9, rup\n"
			"maskand m6, m0, m2\n"
			"fmax.s f7, f12, f6\n"
			"addi x25, x26, 0x1d\n"
			"fnmsub.s f9, f9, f9, f20, rne\n"
			"bne x31, x27, 1f\n"
			"1:\n"
			"fcvt.pw.ps f0, f28, rup\n"
			"fle.s x15, f28, f22\n"
			"fsgnjn.s f20, f28, f20\n"
			"maskand m1, m2, m1\n"
			"masknot m5, m1\n"
			"fmsub.s f2, f15, f0, f2, rmm\n"
			"fcvt.pw.ps f15, f9, rup\n"
			"fmsub.ps f27, f12, f22, f31, rtz\n"
			"bne x27, x28, 1f\n"
			"1:\n"
			"fmulhu.pi f30, f30, f30\n"
			"bge x1, x12, 1f\n"
			"1:\n"
			"fnmsub.s f11, f6, f15, f2, rup\n"
			"bge x8, x2, 1f\n"
			"1:\n"
			"sllw x23, x17, x25\n"
			"xori x15, x15, 0x2\n"
			"fadd.s f22, f6, f22, rdn\n"
			"feq.ps f30, f9, f20\n"
			"fsgnjx.s f22, f9, f22\n"
			"fsra.pi f30, f30, f31\n"
			"xori x9, x25, 0x1d\n"
			"fmsub.ps f27, f28, f9, f6, rdn\n"
			"fmul.ps f3, f28, f15, rdn\n"
			"fmul.ps f7, f6, f15, rdn\n"
			"fpackreph.pi f15, f31\n"
			"beq x17, x2, 1f\n"
			"1:\n"
			"fcvt.s.wu f30, x12, rmm\n"
			"sraw x0, x17, x23\n"
			"fcvt.ps.f16 f20, f0\n"
			"fmsub.ps f7, f20, f9, f20, rup\n"
			"blt x29, x9, 1f\n"
			"1:\n"
			"lui x7, 0x6\n"
			"fclass.s x6, f31\n"
			"fmul.ps f22, f6, f4, rdn\n"
			"fle.s x23, f15, f30\n"
			"fadd.s f30, f2, f31, rne\n"
			"subw x7, x17, x15\n"
			"xori x9, x15, 0x1f\n"
			"fcvt.f16.ps f0, f28\n"
			"fmsub.s f4, f31, f28, f4, rmm\n"
			"masknot m2, m1\n"
			"masknot m0, m1\n"
			"fmul.pi f27, f2, f0\n"
			"sraw x0, x23, x17\n"
			"addw x14, x19, x9\n"
			"blt x29, x24, 1f\n"
			"1:\n"
			"fsgnjx.s f20, f4, f31\n"
			"fminu.pi f20, f20, f15\n"
			"fcmovm.ps f6, f31, f6\n"
			"blt x27, x28, 1f\n"
			"1:\n"
			"fmsub.s f30, f30, f30, f0, rtz\n"
			"fcvt.s.wu f6, x17, rne\n"
			"lui x25, 0xe\n"
			"beq x13, x10, 1f\n"
			"1:\n"
			"fpackreph.pi f0, f0\n"
			"sraw x19, x9, x17\n"
			"masknot m3, m5\n"
			"fexp.ps f4, f4\n"
			"fsin.ps f30, f2\n"
			"addw x0, x17, x19\n"
			"fcvt.ps.f16 f30, f22\n"
			"srliw x19, x25, 1\n"
			"fsrai.pi f4, f30, 0xe\n"
			"bgeu x18, x30, 1f\n"
			"1:\n"
			"or x6, x10, x19\n"
			"bgeu x26, x27, 1f\n"
			"1:\n"
			"fpackreph.pi f3, f22\n"
			"fsgnjx.s f6, f6, f20\n"
			"divw x19, x23, x23\n"
			"fmul.ps f20, f20, f22, rne\n"
			"fmax.s f7, f22, f15\n"
			"fclass.s x9, f31\n"
			"fsatu8.pi f7, f22\n"
			"bne x2, x17, 1f\n"
			"1:\n"
			"masknot m5, m5\n"
			"subw x6, x15, x15\n"
			"subw x9, x26, x15\n"
			"divw x10, x10, x12\n"
			"fnot.pi f22, f2\n"
			"addw x23, x19, x25\n"
			"fsgnjn.ps f3, f6, f15\n"
			"fcvt.pw.ps f30, f9, rtz\n"
			"fmsub.ps f7, f2, f30, f2, rdn\n"
			"fsgnjn.ps f20, f22, f15\n"
			"masknot m3, m5\n"
			"bne x24, x14, 1f\n"
			"1:\n"
			"faddi.pi f4, f22, 0xb4\n"
			"srliw x6, x23, 0\n"
			"fcvt.f16.ps f27, f15\n"
			"fmax.pi f11, f15, f2\n"
			"sllw x23, x10, x12\n"
			"fexp.ps f28, f28\n"
			"blt x11, x17, 1f\n"
			"1:\n"
			"fmsub.s f30, f30, f30, f9, rdn\n"
			"fmulhu.pi f0, f2, f0\n"
			"fsgnjn.ps f12, f12, f31\n"
			"fle.ps f17, f28, f2\n"
			"fpackreph.pi f22, f4\n"
			"addw x25, x19, x25\n"
			"subw x9, x10, x9\n"
			"addw x26, x26, x19\n"
			"fpackreph.pi f11, f20\n"
			"fsrai.pi f12, f9, 0x3\n"
			"blt x18, x21, 1f\n"
			"1:\n"
			"maskand m4, m1, m4\n"
			"subw x9, x12, x12\n"
			"divw x0, x23, x15\n"
			"lui x9, 0x8\n"
			"bgeu x14, x11, 1f\n"
			"1:\n"
			"srliw x17, x15, 0\n"
			"fmsub.s f11, f4, f22, f0, rne\n"
			"fmsub.s f20, f0, f15, f0, rne\n"
			"feq.ps f11, f12, f0\n"
			"mulhu x12, x12, x9\n"
			"fsin.ps f12, f4\n"
			"fsgnjn.ps f27, f30, f6\n"
			"beq x16, x21, 1f\n"
			"1:\n"
			"add x9, x17, x10\n"
			"lui x19, 0x17\n"
			"fpackreph.pi f15, f15\n"
			"beq x23, x23, 1f\n"
			"1:\n"
			"fadd.s f27, f12, f0, rup\n"
			"fnot.pi f2, f2\n"
			"fcvt.pw.ps f11, f15, rne\n"
			"fcvt.s.wu f7, x25, rne\n"
			"fadd.s f6, f6, f4, rdn\n"
			"xori x6, x25, 0xa\n"
			"fmsub.s f4, f4, f9, f12, rne\n"
			"packb x0, x15, x15\n"
			"xori x6, x17, 0x12\n"
			"beq x29, x28, 1f\n"
			"1:\n"
			"fsin.ps f4, f6\n"
			"bge x29, x28, 1f\n"
			"1:\n"
			"fmul.ps f27, f0, f0, rup\n"
			"fcvt.ps.f16 f7, f20\n"
			"blt x29, x21, 1f\n"
			"1:\n"
			"fexp.ps f12, f22\n"
			"fadd.s f9, f20, f9, rmm\n"
			"fmul.ps f22, f28, f4, rne\n"
			"fsin.ps f6, f6\n"
			"fsatu8.pi f12, f0\n"
			"bge x26, x1, 1f\n"
			"1:\n"
			"fsgnjx.s f12, f20, f12\n"
			"fnot.pi f30, f30\n"
			"feq.ps f20, f20, f30\n"
			"beq x11, x11, 1f\n"
			"1:\n"
			"or x9, x23, x9\n"
			"addi x23, x17, 0x18\n"
			"fle.s x14, f15, f6\n"
			"fsgnjx.s f17, f20, f2\n"
			"faddi.pi f28, f30, 0x1f0\n"
			"subw x19, x9, x10\n"
			"fsra.pi f17, f28, f15\n"
			"divw x9, x12, x23\n"
			"fcvt.f16.ps f22, f28\n"
			"sllw x7, x17, x12\n"
			"bltu x4, x1, 1f\n"
			"1:\n"
			"fsgnjn.ps f15, f31, f15\n"
			"fpackreph.pi f0, f0\n"
			"feq.ps f20, f20, f20\n"
			"addi x6, x12, 0xb\n"
			"blt x20, x27, 1f\n"
			"1:\n"
			"fcvt.ps.f16 f22, f20\n"
			"bltu x6, x29, 1f\n"
			"1:\n"
			"fsin.ps f6, f6\n"
			"bgeu x13, x14, 1f\n"
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
			"flw.ps f20,  704 (x5)\n"
			"flw.ps f13,  224 (x5)\n"
			"flw.ps f16,  160 (x5)\n"
			"flw.ps f14,  672 (x5)\n"
			"flw.ps f24,  736 (x5)\n"
			"flw.ps f4,  608 (x5)\n"
			"flw.ps f1,  992 (x5)\n"
			"flw.ps f2,  768 (x5)\n"
			"flw.ps f18,  352 (x5)\n"
			"flw.ps f5,  832 (x5)\n"
			"flw.ps f23,  896 (x5)\n"
			"flw.ps f6,  416 (x5)\n"
			"flw.ps f15,  576 (x5)\n"
			"flw.ps f21,  128 (x5)\n"
			"flw.ps f3,  960 (x5)\n"
			"flw.ps f0,  96 (x5)\n"
			"flw.ps f12,  32 (x5)\n"
			"flw.ps f27,  800 (x5)\n"
			"flw.ps f25,  192 (x5)\n"
			"flw.ps f22,  640 (x5)\n"
			"flw.ps f8,  384 (x5)\n"
			"flw.ps f19,  288 (x5)\n"
			"flw.ps f11,  320 (x5)\n"
			"flw.ps f26,  544 (x5)\n"
			"flw.ps f7,  928 (x5)\n"
			"flw.ps f28,  64 (x5)\n"
			"flw.ps f17,  480 (x5)\n"
			"flw.ps f30,  512 (x5)\n"
			"flw.ps f10,  864 (x5)\n"
			"flw.ps f31,  256 (x5)\n"
			"flw.ps f9,  0 (x5)\n"
			"flw.ps f29,  448 (x5)\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_5_FP_SCATTER_ID_97_HID_0:\n"
			"li x18, 0x8103b7a546\n" 
			"la x26, mem_ptr_offset\n"
			"flq2 f10, 168(x26)\n"
			"fscb.ps f17, f10 (x18)\n" 
			"LBL_SEQID_5_FP_SCATTER_ID_98_HID_0:\n"
			"li x27, 0x820591aa27\n" 
			"la x23, mem_ptr_offset\n"
			"flq2 f27, 8(x23)\n"
			"fscb.ps f4, f27 (x27)\n" 
			"LBL_SEQID_5_FP_AMO_ID_99_HID_0:\n"
			"li x26, 0x814b240030\n" 
			"la x11, mem_ptr_offset\n"
			"flw.ps f22, 960(x11)\n"
			"famomaxul.pi f3, f22 (x26)\n" 
			"LBL_SEQID_5_FP_AMO_ID_100_HID_0:\n"
			"li x28, 0x8138ed2004\n" 
			"la x13, mem_ptr_offset\n"
			"flw.ps f4, 96(x13)\n"
			"famominug.pi f2, f4 (x28)\n" 
			"LBL_SEQID_5_CSR_ID_101_HID_0:\n"
			"csrr x0, scause\n"
			"LBL_SEQID_5_INT_LOAD_ID_102_HID_0:\n"
			"li x26, 0x820bfc0033\n" 
			"sd x14, -475(x26)\n"
			"lh x25, -475(x26)\n" 
			"LBL_SEQID_5_INT_AMO_ID_103_HID_0:\n"
			"li x27, 0x81525d1038\n" 
			"amoandl.d x26, x20, (x27)\n" 
			"LBL_SEQID_5_INT_AMO_ID_104_HID_0:\n"
			"li x5, 0x8232da0560\n" 
			"amomaxg.d x26, x23, (x5)\n" 
			"LBL_SEQID_5_FP_AMO_ID_105_HID_0:\n"
			"li x19, 0x825082001c\n" 
			"la x18, mem_ptr_offset\n"
			"flw.ps f27, 224(x18)\n"
			"famoorl.pi f8, f27 (x19)\n" 
			"LBL_SEQID_5_INT_AMO_ID_106_HID_0:\n"
			"li x6, 0x81278e04f8\n" 
			"amoxorg.d x27, x5, (x6)\n" 
			"LBL_SEQID_5_FP_STORE_ID_107_HID_0:\n"
			"li x27, 0x8203ce0028\n" 
			"fsw f18, -1759(x27)\n" 
			"LBL_SEQID_5_INT_AMO_ID_108_HID_0:\n"
			"li x14, 0x8226dc0660\n" 
			"amoming.w x22, x5, (x14)\n" 
			"LBL_SEQID_5_INT_AMO_ID_109_HID_0:\n"
			"li x14, 0x822d9e0320\n" 
			"amoorg.w x23, x25, (x14)\n" 
			"LBL_SEQID_5_INT_AMO_ID_110_HID_0:\n"
			"li x6, 0x824ee0077c\n" 
			"amomaxl.w x26, x5, (x6)\n" 
			"LBL_SEQID_5_FP_LOAD_ID_111_HID_0:\n"
			"li x26, 0x810971293d\n" 
			"fsw.ps f6, 241(x26)\n"
			"flw f18, 241(x26)\n" 
			"LBL_SEQID_5_INT_G_STORE_ID_112_HID_0:\n"
			"li x6, 0x813b83d814\n" 
			"shg x23, (x6)\n" 
			"LBL_SEQID_5_FP_G_GATHER_ID_113_HID_0:\n"
			"li x18, 0x813b83d814\n" 
			"la x14, mem_ptr_offset\n"
			"flq2 f28, 88(x14)\n"
			"fscwg.ps f8, f28 (x18)\n"
			"fghg.ps f31, f28 (x18)\n" 
			"LBL_SEQID_5_INT_AMO_ID_114_HID_0:\n"
			"li x25, 0x823e290e20\n" 
			"amomaxg.d x14, x28, (x25)\n" 
			"LBL_SEQID_5_CSR_ID_115_HID_0:\n"
			"csrr x0, tselect\n"
			"LBL_SEQID_5_FP_AMO_ID_116_HID_0:\n"
			"li x19, 0x8137240630\n" 
			"la x10, mem_ptr_offset\n"
			"flw.ps f23, 672(x10)\n"
			"famoswapg.pi f20, f23 (x19)\n" 
			"LBL_SEQID_5_INT_AMO_ID_117_HID_0:\n"
			"li x17, 0x8241a60028\n" 
			"amoxorl.w x23, x26, (x17)\n" 
			"LBL_SEQID_5_FP_SCATTER_ID_118_HID_0:\n"
			"li x14, 0x8208ea0009\n" 
			"la x16, mem_ptr_offset\n"
			"flw.ps f23, 204(x16)\n"
			"fscw.ps f24, f23 (x14)\n" 
			"LBL_SEQID_5_FP_SCATTER_ID_119_HID_0:\n"
			"li x28, 0x81007e0172\n" 
			"la x14, mem_ptr_offset\n"
			"flq2 f12, 108(x14)\n"
			"fscb.ps f4, f12 (x28)\n" 
			"LBL_SEQID_5_INT_AMO_ID_120_HID_0:\n"
			"li x18, 0x825112f698\n" 
			"amoandl.d x22, x5, (x18)\n" 
			"LBL_SEQID_5_FP_L_STORE_ID_121_HID_0:\n"
			"li x5, 0x814f1a0000\n" 
			"mov.m.x m0, zero, 0xff\n"
			"fswl.ps f21, (x5)\n" 
			"LBL_SEQID_5_INT_AMO_ID_122_HID_0:\n"
			"li x15, 0x814b052800\n" 
			"amomaxul.w x14, x11, (x15)\n" 
			"LBL_SEQID_5_INT_STORE_ID_123_HID_0:\n"
			"li x25, 0x820a320330\n" 
			"sw x10, 1997(x25)\n" 
			"la x27, rand_int_32\n"
			"flw.ps f8, 672(x27)\n"
			"flw.ps f21, 32(x27)\n"
			"fremu.pi f21, f8, f21\n" 
			"LBL_SEQID_5_FP_G_GATHER_ID_124_HID_0:\n"
			"li x9, 0x8126a7a000\n" 
			"la x14, mem_ptr_offset\n"
			"flw.ps f18, 176(x14)\n"
			"fscwg.ps f5, f18 (x9)\n"
			"fgbg.ps f3, f18 (x9)\n" 
			"LBL_SEQID_5_FP_SCATTER_ID_125_HID_0:\n"
			"li x13, 0x810c4b6017\n" 
			"li x18, 0xddec1c8f662f2c59\n"
			"fsc32h.ps f0, x18 (x13)\n" 
			"LBL_SEQID_5_FP_AMO_ID_126_HID_0:\n"
			"li x11, 0x82581004d4\n" 
			"la x27, mem_ptr_offset\n"
			"flw.ps f5, 864(x27)\n"
			"famomaxul.pi f25, f5 (x11)\n" 
			"LBL_SEQID_5_INT_AMO_ID_127_HID_0:\n"
			"li x17, 0x8254f9381c\n" 
			"amoandl.w x13, x6, (x17)\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"li x5, 0xc0\n"
			"la x31, rand_ieee754_32\n"
			"add x31, x31, x5\n"
			"flw.ps f0, 0(x31)\n"
			"flw.ps f1, 2(x31)\n"
			"flw.ps f10, 4(x31)\n"
			"flw.ps f11, 1(x31)\n"
			"flw.ps f12, 3(x31)\n"
			"flw.ps f13, 9(x31)\n"
			"flw.ps f14, 21(x31)\n"
			"flw.ps f15, 17(x31)\n"
			"flw.ps f16, 7(x31)\n"
			"flw.ps f17, 28(x31)\n"
			"flw.ps f18, 19(x31)\n"
			"flw.ps f19, 39(x31)\n"
			"flw.ps f2, 42(x31)\n"
			"flw.ps f20, 30(x31)\n"
			"flw.ps f21, 19(x31)\n"
			"flw.ps f22, 20(x31)\n"
			"flw.ps f23, 0(x31)\n"
			"flw.ps f24, 30(x31)\n"
			"flw.ps f25, 67(x31)\n"
			"flw.ps f26, 35(x31)\n"
			"flw.ps f27, 66(x31)\n"
			"flw.ps f28, 55(x31)\n"
			"flw.ps f29, 8(x31)\n"
			"flw.ps f3, 80(x31)\n"
			"flw.ps f30, 81(x31)\n"
			"flw.ps f31, 60(x31)\n"
			"flw.ps f4, 58(x31)\n"
			"flw.ps f5, 75(x31)\n"
			"flw.ps f6, 5(x31)\n"
			"flw.ps f7, 58(x31)\n"
			"flw.ps f8, 99(x31)\n"
			"flw.ps f9, 1(x31)\n"
			"la x31, rand_int_32\n"
			"add x31, x31, x5\n"
			"lw x0, 0(x31)\n"
			"lw x10, 4(x31)\n"
			"lw x11, 6(x31)\n"
			"lw x12, 5(x31)\n"
			"lw x13, 10(x31)\n"
			"lw x14, 1(x31)\n"
			"lw x15, 23(x31)\n"
			"lw x16, 21(x31)\n"
			"lw x17, 4(x31)\n"
			"lw x18, 11(x31)\n"
			"lw x19, 8(x31)\n"
			"lw x20, 36(x31)\n"
			"lw x22, 8(x31)\n"
			"lw x23, 48(x31)\n"
			"lw x25, 40(x31)\n"
			"lw x26, 51(x31)\n"
			"lw x27, 62(x31)\n"
			"lw x28, 21(x31)\n"
			"lw x5, 27(x31)\n"
			"lw x6, 40(x31)\n"
			"lw x7, 8(x31)\n"
			"lw x9, 8(x31)\n"
			"li x21, 0x3\n"
			"LBL_SEQID_5_LOOP_SEQ_HID_0:\n"
			"flt.ps f24, f21, f14\n"
			"maskor m3, m3, m1\n"
			"fmsub.ps f18, f29, f8, f12, rdn\n"
			"srl x17, x0, x11\n"
			"fmsub.s f8, f15, f27, f21, rup\n"
			"bgeu x7, x24, 1f\n"
			"1:\n"
			"flog.ps f14, f14\n"
			"fnmsub.ps f11, f31, f11, f17, rup\n"
			"bgeu x9, x17, 1f\n"
			"1:\n"
			"fmsub.ps f18, f26, f25, f3, rne\n"
			"rem x19, x17, x16\n"
			"ebreak\n"
			"fsll.pi f4, f16, f4\n"
			"fnmsub.ps f1, f1, f18, f10, rne\n"
			"fadd.pi f8, f25, f6\n"
			"bne x28, x20, 1f\n"
			"1:\n"
			"fnmsub.ps f14, f28, f9, f6, rne\n"
			"addi x9, x12, 0x9\n"
			"mulhsu x15, x26, x13\n"
			"bne x11, x17, 1f\n"
			"1:\n"
			"fbcx.ps f28, x26\n"
			"fence\n"
			"bltu x8, x5, 1f\n"
			"1:\n"
			"divuw x11, x6, x28\n"
			"addi x20, x9, 0x1f\n"
			"ecall\n"
			"fsrli.pi f4, f8, 0x8\n"
			"blt x30, x0, 1f\n"
			"1:\n"
			"frsq.ps f12, f3\n"
			"or x27, x20, x10\n"
			"rem x13, x13, x25\n"
			"maskand m4, m5, m3\n"
			"sltu x23, x23, x26\n"
			"beq x16, x9, 1f\n"
			"1:\n"
			"fadd.ps f5, f15, f21, rtz\n"
			"fmulh.pi f25, f24, f11\n"
			"fmaxu.pi f30, f17, f30\n"
			"bgeu x30, x5, 1f\n"
			"1:\n"
			"frsq.ps f0, f0\n"
			"fadd.ps f13, f13, f7, rup\n"
			"mulw x6, x11, x13\n"
			"mova.m.x x14\n"
			"mova.x.m x17\n"
			"rem x23, x18, x20\n"
			"fswizz.ps f22, f5, 0x1a\n"
			"fmadd.s f13, f19, f13, f4, rdn\n"
			"maskand m5, m2, m0\n"
			"bge x9, x0, 1f\n"
			"1:\n"
			"sll x20, x25, x0\n"
			"fcvt.pw.ps f27, f1, rtz\n"
			"andi x13, x5, 0x12\n"
			"fbci.pi f8, 0x35667\n"
			"remw x5, x5, x6\n"
			"subw x6, x6, x19\n"
			"addiw x26, x5, 0x1a\n"
			"beq x22, x16, 1f\n"
			"1:\n"
			"fmvs.x.ps x19, f26, 5\n"
			"mulh x7, x14, x7\n"
			"maskxor m5, m3, m4\n"
			"fcvt.ps.pwu f14, f14, rne\n"
			"sltu x25, x11, x20\n"
			"fmax.ps f10, f13, f5\n"
			"bgeu x22, x18, 1f\n"
			"1:\n"
			"fadd.ps f16, f10, f2, rup\n"
			"fmax.s f25, f16, f20\n"
			"fcvt.pwu.ps f21, f12, rmm\n"
			"fmin.ps f17, f6, f17\n"
			"fround.ps f15, f6, rup\n"
			"fmul.ps f2, f26, f0, rup\n"
			"fround.ps f6, f8, rdn\n"
			"fmsub.ps f5, f7, f6, f5, rne\n"
			"fnot.pi f28, f30\n"
			"frsq.ps f21, f17\n"
			"remuw x20, x9, x28\n"
			"feq.s x22, f7, f16\n"
			"fcvt.ps.pwu f20, f20, rup\n"
			"fand.pi f12, f14, f7\n"
			"fmulh.pi f30, f8, f7\n"
			"fcvt.wu.s x23, f27, rdn\n"
			"fcvt.w.s x10, f12, rtz\n"
			"feq.ps f7, f7, f2\n"
			"addw x15, x15, x11\n"
			"feqm.ps m2, f22, f15\n"
			"beq x30, x23, 1f\n"
			"1:\n"
			"fbci.ps f28, 0x92e4\n"
			"fand.pi f25, f8, f25\n"
			"bge x26, x22, 1f\n"
			"1:\n"
			"addiw x20, x20, 0x1a\n"
			"fswizz.ps f16, f21, 0xe\n"
			"fsat8.pi f0, f9\n"
			"divu x14, x14, x7\n"
			"bgeu x25, x20, 1f\n"
			"1:\n"
			"maskor m2, m2, m4\n"
			"feqm.ps m1, f6, f14\n"
			"sll x17, x13, x9\n"
			"addw x19, x6, x16\n"
			"sllw x7, x25, x7\n"
			"bltu x0, x10, 1f\n"
			"1:\n"
			"fadd.s f12, f20, f30, rup\n"
			"fmvs.x.ps x0, f11, 7\n"
			"fsgnjn.ps f26, f0, f26\n"
			"packb x19, x15, x0\n"
			"fsrli.pi f0, f21, 0x4\n"
			"fltm.pi m3, f4, f7\n"
			"fcvt.ps.pwu f16, f30, rne\n"
			"fsat8.pi f17, f5\n"
			"slti x23, x23, 0x1c\n"
			"fmv.x.s x19, f29\n"
			"fcvt.pw.ps f26, f18, rne\n"
			"bgeu x19, x9, 1f\n"
			"1:\n"
			"fmul.pi f15, f3, f29\n"
			"fnot.pi f28, f10\n"
			"mulw x15, x0, x11\n"
			"fadd.ps f0, f26, f18, rtz\n"
			"fsgnjx.ps f27, f14, f5\n"
			"add x16, x16, x7\n"
			"fclass.s x18, f0\n"
			"fmin.s f22, f12, f22\n"
			"fround.ps f2, f29, rmm\n"
			"remw x28, x26, x18\n"
			"lui x0, 0x7\n"
			"bgeu x8, x3, 1f\n"
			"1:\n"
			"fclass.ps f31, f23\n"
			"mulh x13, x10, x7\n"
			"fcmovm.ps f31, f19, f22\n"
			"fandi.pi f24, f7, 0x1de\n"
			"beq x7, x23, 1f\n"
			"1:\n"
			"sraiw x27, x19, 5\n"
			"fmaxu.pi f20, f4, f22\n"
			"auipc x12, 0x12\n"
			"or x6, x20, x13\n"
			"slti x22, x26, 0x1\n"
			"fswizz.ps f8, f6, 0x1b\n"
			"fmax.s f21, f10, f9\n"
			"mul x17, x28, x22\n"
			"bne x22, x11, 1f\n"
			"1:\n"
			"fcvt.ps.f16 f22, f27\n"
			"or x17, x28, x15\n"
			"fsgnjn.ps f6, f3, f22\n"
			"fsub.pi f27, f3, f31\n"
			"bgeu x8, x13, 1f\n"
			"1:\n"
			"fadd.pi f12, f23, f6\n"
			"fxor.pi f18, f25, f18\n"
			"fnmsub.s f11, f14, f11, f20, rtz\n"
			"bgeu x3, x26, 1f\n"
			"1:\n"
			"fmsub.ps f25, f5, f1, f17, rtz\n"
			"bgeu x14, x1, 1f\n"
			"1:\n"
			"fcvt.ps.f16 f27, f9\n"
			"divw x18, x5, x18\n"
			"fminu.pi f19, f3, f8\n"
			"fmax.s f8, f25, f31\n"
			"xori x13, x6, 0x8\n"
			"fcvt.wu.s x11, f10, rtz\n"
			"srlw x14, x20, x0\n"
			"bne x11, x1, 1f\n"
			"1:\n"
			"fnmadd.s f27, f23, f27, f19, rdn\n"
			"fmvz.x.ps x28, f9, 4\n"
			"fcvt.s.wu f14, x22, rtz\n"
			"fcvt.pw.ps f5, f15, rdn\n"
			"fltm.ps m6, f25, f17\n"
			"fsatu8.pi f27, f27\n"
			"divu x18, x12, x27\n"
			"bge x13, x1, 1f\n"
			"1:\n"
			"for.pi f14, f19, f15\n"
			"fnmadd.ps f16, f16, f5, f19, rtz\n"
			"frcp.ps f24, f1\n"
			"blt x21, x13, 1f\n"
			"1:\n"
			"divu x11, x0, x15\n"
			"packb x18, x28, x7\n"
			"maskand m3, m3, m1\n"
			"fsrl.pi f16, f11, f22\n"
			"bgeu x17, x25, 1f\n"
			"1:\n"
			"srlw x7, x12, x6\n"
			"fslli.pi f16, f20, 0xa\n"
			"divuw x20, x28, x11\n"
			"bgeu x21, x4, 1f\n"
			"1:\n"
			"for.pi f12, f12, f4\n"
			"fmin.s f15, f16, f2\n"
			"maskpopcz x7, m4\n"
			"bltu x9, x7, 1f\n"
			"1:\n"
			"fpackreph.pi f23, f29\n"
			"fsgnjn.s f1, f10, f0\n"
			"faddi.pi f9, f22, 0x5b\n"
			"blt x0, x27, 1f\n"
			"1:\n"
			"flt.s x26, f29, f29\n"
			"sraiw x25, x11, 5\n"
			"bge x17, x3, 1f\n"
			"1:\n"
			"sraiw x16, x5, 3\n"
			"bgeu x17, x25, 1f\n"
			"1:\n"
			"fltm.ps m7, f6, f31\n"
			"flt.s x15, f24, f6\n"
			"bltu x8, x20, 1f\n"
			"1:\n"
			"fsgnjx.ps f14, f16, f2\n"
			"ori x19, x11, 0x13\n"
			"addiw x23, x25, 0x6\n"
			"feqm.ps m4, f7, f0\n"
			"fle.s x11, f31, f22\n"
			"fbci.pi f5, 0x44729\n"
			"fclass.ps f2, f24\n"
			"fmvz.x.ps x12, f24, 5\n"
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
			"LBL_HPM_3_CORE_TXFMA_INT_OPS_FAIL_HID_0:\n"          
			"csrr x5, mhpmcounter3\n"
			"sd x5, 32 (x31)\n"
			"LBL_HPM_4_CORE_TFMA_WAIT_TENB_FAIL_HID_0:\n"          
			"csrr x5, mhpmcounter4\n"
			"sd x5, 40 (x31)\n"
			"LBL_HPM_5_CORE_TL_OPS_FAIL_HID_0:\n"          
			"csrr x5, mhpmcounter5\n"
			"sd x5, 48 (x31)\n"
			"LBL_HPM_6_CORE_DCACHE_MISSES0_FAIL_HID_0:\n"          
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
			"LBL_HPM_3_CORE_TXFMA_INT_OPS_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter3\n"
			"sd x5, 32 (x31)\n"
			"LBL_HPM_4_CORE_TFMA_WAIT_TENB_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter4\n"
			"sd x5, 40 (x31)\n"
			"LBL_HPM_5_CORE_TL_OPS_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter5\n"
			"sd x5, 48 (x31)\n"
			"LBL_HPM_6_CORE_DCACHE_MISSES0_PASS_HID_0:\n"          
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
		0x72809676, 
		0x48b065f8, 
		0xdc9c319c, 
		0x56c850df, 
		0xc6baabe7, 
		0x13dfba36, 
		0xf9886f37, 
		0x36d5759d, 
		0x36301fa8, 
		0x1ba90abf, 
		0x79ee0585, 
		0x2eb09ba6, 
		0x57d78f35, 
		0xb1dc8d8a, 
		0xcfa57534, 
		0x09263887, 
		0xbfc74a63, 
		0x105c2476, 
		0x527ce744, 
		0x63dc0129, 
		0x44f55fd5, 
		0xd0c3731a, 
		0x456f46e6, 
		0x5019c60b, 
		0xb04709af, 
		0x2278da0e, 
		0x135d6b10, 
		0xbeb2a7ac, 
		0x70b2febe, 
		0x25d6efda, 
		0x8e8740eb, 
		0xc7581be6, 
		0x08d54e0c, 
		0xd47f1677, 
		0x23ad3035, 
		0xb500ae11, 
		0x0d1646ba, 
		0xe7727393, 
		0x8bfab40e, 
		0x82957a16, 
		0x4b62f82d, 
		0x35653c9c, 
		0xaa73293f, 
		0x7e8ee578, 
		0x19349655, 
		0xea40badf, 
		0x6da597c5, 
		0xe045ddc2, 
		0x67e933b3, 
		0xa339bf39, 
		0x3e8a2bcd, 
		0x46c82b71, 
		0xa3d56c4a, 
		0xfd5eb85f, 
		0xee3eb0e1, 
		0x2c7c95b4, 
		0x3947ea34, 
		0x824f6f02, 
		0x26820183, 
		0xc8136f37, 
		0xd5f9cf4b, 
		0xebbd3c73, 
		0x28632f5f, 
		0x35320369, 
		0x28eae076, 
		0xc202c52d, 
		0x875a11a1, 
		0x85baf4ee, 
		0x7d73a02c, 
		0xfca5d709, 
		0xc9c98286, 
		0xe8405e44, 
		0x6b6099b9, 
		0x73d1956c, 
		0xf397741d, 
		0xd8a8b66f, 
		0x83a6fce4, 
		0x77d6bf7b, 
		0xf233b644, 
		0x2d78c6d0, 
		0x0f357589, 
		0xab441f46, 
		0x90ba0346, 
		0x787b15c7, 
		0x2d3c69aa, 
		0x44107bb7, 
		0x2a514ad3, 
		0xfb82bb7d, 
		0xd58dd318, 
		0xd29c13b1, 
		0x3616e747, 
		0xb54faadc, 
		0x4d57c3ef, 
		0x0cd891a2, 
		0x305486c3, 
		0x238fb033, 
		0xa63cef92, 
		0x5eebc2a3, 
		0xe7514212, 
		0xd34f042c, 
		0x2c17525e, 
		0x0e912522, 
		0x99a43202, 
		0xc6c8cdec, 
		0x0acb78d6, 
		0x25f4ce28, 
		0xc39c87fb, 
		0x0a530abf, 
		0x0b0c61df, 
		0xc8a07c45, 
		0xfe755c69, 
		0x931b9da3, 
		0x7016d7b8, 
		0x952a3340, 
		0xb8fd349b, 
		0x9c7b5e6c, 
		0x85e8c534, 
		0x790f6bab, 
		0x7ce44697, 
		0xb7916dbd, 
		0x7e7d32e6, 
		0xd108a3a8, 
		0x1f0826d1, 
		0x1b7e7ba3, 
		0x23ed6674, 
		0x3a99eacd, 
		0xbb54f743, 
		0xe5a3ec1e, 
		0x602d4d35, 
		0xcfce60d4, 
		0x6cbad881, 
		0x52c94699, 
		0x25e16cf8, 
		0x2b303eeb, 
		0x57cbf30c, 
		0xff073d82, 
		0x8158cdce, 
		0x7d25541b, 
		0xce93c0bf, 
		0x0d49b82a, 
		0x79e9c60b, 
		0x91cb1400, 
		0xffadfa95, 
		0x7a8ecc4e, 
		0x1ee8678a, 
		0x8247dac2, 
		0x1e14efe6, 
		0x86b9033d, 
		0x55831ea8, 
		0x509531af, 
		0xa18f2a2d, 
		0x0fefc5c9, 
		0x9c0167b9, 
		0x848fa0ae, 
		0xcdf06f5c, 
		0x6bfad401, 
		0xb174ecb5, 
		0x17d3a0d7, 
		0x1025c231, 
		0x5e328d29, 
		0xcd93144d, 
		0x5dcf545c, 
		0xef853543, 
		0x3a39e4be, 
		0x3f5f6f96, 
		0x7984b572, 
		0xde72fe1e, 
		0x02876f02, 
		0x765897bd, 
		0x355c7e6a, 
		0x9f5fa3e9, 
		0xe265ec10, 
		0x297a2332, 
		0x70e12917, 
		0x41657ba8, 
		0x4e1452e0, 
		0x93634ea0, 
		0x04505bb2, 
		0x697160c1, 
		0xb9ff08ed, 
		0x26c46ae0, 
		0x269b6cb0, 
		0x9abcdbc9, 
		0xe7e62e37, 
		0xae6c8bc2, 
		0x44f6df92, 
		0xeebd5e57, 
		0xa34f6195, 
		0x7877a1a8, 
		0x8d62a56a, 
		0x7bbc189b, 
		0x48a794a7, 
		0xd0d7f74f, 
		0xa5d745d0, 
		0xdabde75d, 
		0x2ad47e6e, 
		0x8c5c3b6c, 
		0xc8724dd5, 
		0xad9a7ab3, 
		0x7c8e72bf, 
		0xcd8c535a, 
		0xf570b1f6, 
		0x5973a02a, 
		0xc102ebf7, 
		0x4d4322b9, 
		0x2bae0891, 
		0xdd23084b, 
		0x6d3c760c, 
		0x44caef8b, 
		0x2c137f97, 
		0x22b71668, 
		0xd007a83e, 
		0xf79280ea, 
		0x7ae453fc, 
		0x1c572012, 
		0xca68666b, 
		0x195ba7ef, 
		0x916b9b17, 
		0x3be8a805, 
		0x1b8d73ca, 
		0xe83960ea, 
		0xef6bb21f, 
		0x056f3986, 
		0x8fcbf549, 
		0x1ac66749, 
		0xf3e77ed5, 
		0x3f0169b0, 
		0x13ad5e46, 
		0x1760ebad, 
		0x9dc69ff2, 
		0x5514e2c8, 
		0xfee61578, 
		0xccbd97f9, 
		0x22e3a940, 
		0x79545a94, 
		0x3a8c3b5b, 
		0xc137f7b5, 
		0x158e5871, 
		0x3f95068c, 
		0x3dd35de9, 
		0x21926ba0, 
		0xe9777996, 
		0x1e487ec6, 
		0x911a97c1, 
		0x0315152a, 
		0x658376dc, 
		0x282b517f, 
		0x91dc9c78, 
		0x598c4e82, 
		0xfb14e8a4, 
		0x77db5c0c, 
		0x18fef42d, 
		0x3aa0150b, 
		0x5c43d797, 
		0x4dcb57c5, 
		0x504807c8  
};
volatile uint32_t rand_ieee754_32[1536] __attribute__ ((aligned (2048))) = {
		0xc0e00000, 
		0xc1900000, 
		0x00000000, 
		0xc1b80000, 
		0xc2700000, 
		0xc1300000, 
		0xc20d4ff2, 
		0xc2180000, 
		0x7fffffff, 
		0x0c7ffc00, 
		0x7f7ffffe, 
		0xc2440000, 
		0x1e4946eb, 
		0x42500000, 
		0x9b5768ad, 
		0x6f3d64eb, 
		0x80080000, 
		0xc2500000, 
		0xa5346db9, 
		0x00000200, 
		0x41e00000, 
		0x00011111, 
		0xcef902ce, 
		0x5e857375, 
		0xc22c0000, 
		0x3f028f5c, 
		0x80800002, 
		0x11dbcf77, 
		0x00800003, 
		0x00020000, 
		0x4a879f07, 
		0xa4f1f3e6, 
		0x4fa94b31, 
		0x41980000, 
		0x421c0000, 
		0x3f800001, 
		0xc2080000, 
		0xff000000, 
		0xc0800000, 
		0x0e0000ff, 
		0xff800000, 
		0x41b80000, 
		0xbf144955, 
		0xae1e6f97, 
		0x3f800001, 
		0x0e00000f, 
		0xb3112a0b, 
		0x95f288f3, 
		0x92cc76b6, 
		0x80000001, 
		0x41300000, 
		0xbe24dd6b, 
		0x8c4dc867, 
		0x5e5f5510, 
		0xc1e00000, 
		0x00000008, 
		0x0c7c0000, 
		0x00000000, 
		0x72b1ff3a, 
		0xe517e338, 
		0x9ccd6186, 
		0x7fffffff, 
		0xc1980000, 
		0x00400000, 
		0x4f56e28a, 
		0x487cccc1, 
		0x0c7fc000, 
		0x0c7fe000, 
		0x80800002, 
		0x7f800000, 
		0x0c7fc000, 
		0xffe90380, 
		0xc1f80000, 
		0x19243abf, 
		0xd1eac0fb, 
		0xa70a5829, 
		0x0d000ff0, 
		0xff7ffffe, 
		0x41d80000, 
		0xff800000, 
		0x42500000, 
		0xb14ac211, 
		0x599e7c53, 
		0x00000000, 
		0x8035c32d, 
		0x00800000, 
		0xc84fa145, 
		0x00080000, 
		0xc1c80000, 
		0xc5a800e1, 
		0x0f7ffffe, 
		0x7f86f221, 
		0x80000000, 
		0x6679c987, 
		0x42280000, 
		0x41600000, 
		0x0e00007f, 
		0xff800001, 
		0x2d9db5d4, 
		0x807fffff, 
		0xcedddec6, 
		0x5f316c54, 
		0x41e00000, 
		0x807ffffe, 
		0x80000000, 
		0xf79a5869, 
		0x92ba9ead, 
		0x97de8be5, 
		0xad367a23, 
		0x00800001, 
		0x8b666420, 
		0x3d3b6d7d, 
		0x80000002, 
		0x0e0fffff, 
		0xc1500000, 
		0xc0000000, 
		0x78bc250d, 
		0x77689255, 
		0x3f800001, 
		0x3c7f243c, 
		0xc2700000, 
		0x0e00001f, 
		0x0c7e0000, 
		0x80000000, 
		0x00000000, 
		0xf98ef612, 
		0x0c7f8000, 
		0x80000020, 
		0x0e18d571, 
		0xeb548cba, 
		0xd9b8bb4a, 
		0x40000000, 
		0x4ae26dc7, 
		0x00000000, 
		0xbc71f57d, 
		0x8f7fffff, 
		0x0e00003f, 
		0x71ad19dd, 
		0x1a4de3fb, 
		0xa241d17c, 
		0x7f800000, 
		0xc1a80000, 
		0x00000000, 
		0x8ddba9d0, 
		0x39d7adae, 
		0x00800001, 
		0xa4f0bb7e, 
		0x41e00000, 
		0x379684d1, 
		0xff7fffff, 
		0x00000020, 
		0x00000020, 
		0x48db3c43, 
		0x80000000, 
		0x7f7ffffe, 
		0x40c00000, 
		0x00000000, 
		0x26aff0ce, 
		0x7f7ffffe, 
		0xc1c80000, 
		0xffb06b2c, 
		0x06d6384e, 
		0x00597e5f, 
		0x8374f943, 
		0x4bbbfc7e, 
		0x00000000, 
		0x7f7ffffe, 
		0x00001000, 
		0x6d823f77, 
		0xbf028f5c, 
		0x424c0000, 
		0x0f7ffffd, 
		0x0c7f8000, 
		0x0031e575, 
		0xff7fffff, 
		0xbf406d17, 
		0x80800003, 
		0xc1200000, 
		0x180f2a7c, 
		0x26eed94b, 
		0xc2540000, 
		0xff7ffffe, 
		0x0e03ffff, 
		0x80000000, 
		0xff800000, 
		0x80000000, 
		0x235ff11a, 
		0x185c96bb, 
		0x41f00000, 
		0xc2440000, 
		0xcf44aa5f, 
		0x42a5e93d, 
		0xc1b00000, 
		0x1ab0b590, 
		0x00000000, 
		0x41b80000, 
		0x00004000, 
		0x7fb25f8f, 
		0x80000400, 
		0x80800000, 
		0x00000400, 
		0x3ff62aca, 
		0x455d11b5, 
		0x42240000, 
		0x80800000, 
		0xa5693f18, 
		0x42100000, 
		0x80200000, 
		0xb29dfb7a, 
		0x40e00000, 
		0x3a76e9b9, 
		0x80000000, 
		0x7f9ec152, 
		0x41980000, 
		0x41f00000, 
		0x10ba87e4, 
		0x80000008, 
		0xff800000, 
		0x42640000, 
		0x0c600000, 
		0x41880000, 
		0x0e07ffff, 
		0x11e86dcb, 
		0x421c0000, 
		0x00400000, 
		0x44acb23c, 
		0x80000080, 
		0x0e000003, 
		0x80800002, 
		0xc2740000, 
		0xc1300000, 
		0x41880000, 
		0x0e0001ff, 
		0xc1980000, 
		0x7fffffff, 
		0x8daaa4f6, 
		0xff800000, 
		0x8d56cbf0, 
		0xffddcbe0, 
		0xf89cae02, 
		0x80002000, 
		0x7f800000, 
		0x0f7ffffe, 
		0xc2180000, 
		0x0e0001ff, 
		0xe4b6cc3a, 
		0xafec3f6b, 
		0x80080000, 
		0x004353d8, 
		0xc1b80000, 
		0xb5b8d889, 
		0x41a80000, 
		0x4010f8d0, 
		0xc1200000, 
		0xffffffff, 
		0xc1980000, 
		0x41e00000, 
		0xc1a80000, 
		0x7f800000, 
		0x007ffffe, 
		0x57244e34, 
		0xc1500000, 
		0x40e00000, 
		0x00011111, 
		0x42580000, 
		0xc2580000, 
		0xa5d62dfb, 
		0x86dff4a4, 
		0x426c0000, 
		0x41700000, 
		0x80ffffff, 
		0x7f800000, 
		0x00800000, 
		0x80000000, 
		0x0e00000f, 
		0x41980000, 
		0xbf028f5c, 
		0x80010000, 
		0x7fc00001, 
		0xc25c0000, 
		0x8a01688f, 
		0x425c0000, 
		0xab6b0b37, 
		0xc1b00000, 
		0x80800000, 
		0xc1a00000, 
		0xffbe51ff, 
		0x80800000, 
		0xcefe3053, 
		0x00000800, 
		0x0c7fffc0, 
		0x4b000000, 
		0xc2140000, 
		0x00000002, 
		0x0c7ff800, 
		0x97dc69de, 
		0x385d5f7c, 
		0x51ed70c6, 
		0x266b67f7, 
		0x9bb6f97d, 
		0x0ae3128a, 
		0xc2300000, 
		0x86e30783, 
		0xa1435cc9, 
		0xc1f00000, 
		0x80000000, 
		0x41244728, 
		0x80010000, 
		0x00800002, 
		0x00000800, 
		0x42740000, 
		0x7f800000, 
		0x0e00000f, 
		0xc2580000, 
		0x426c0000, 
		0x0e0007ff, 
		0xbf800000, 
		0x7f800000, 
		0x7e5430f0, 
		0xc2100000, 
		0x41500000, 
		0xff85cc9f, 
		0xc2140000, 
		0x42440000, 
		0x80800000, 
		0x80080000, 
		0xffb6c3aa, 
		0x42480000, 
		0x8f7ffffe, 
		0x41581b3e, 
		0xc1700000, 
		0x00000000, 
		0x40c00000, 
		0x80000400, 
		0x33333333, 
		0x0f7ffffd, 
		0xe5c34c9a, 
		0x0e000003, 
		0x6316edc9, 
		0x0e0000ff, 
		0x40a00000, 
		0x80000000, 
		0x80000000, 
		0xe77b272b, 
		0x007ffffe, 
		0x7fffffff, 
		0x0f7fffff, 
		0xcb8c4b40, 
		0x00507d08, 
		0x00080000, 
		0x807ffffe, 
		0xf52e4d48, 
		0x80004000, 
		0x91659564, 
		0x015d837d, 
		0x41b00000, 
		0x42700000, 
		0x0c780000, 
		0x80ffffff, 
		0x8bbcb95f, 
		0x007fffff, 
		0x37ba0883, 
		0x80000800, 
		0x41e00000, 
		0x3a039807, 
		0x7fc00001, 
		0xffaea868, 
		0xcb000000, 
		0xd9e2fd79, 
		0x00800003, 
		0x873260e8, 
		0xfda15361, 
		0x0c7fffe0, 
		0x119c91eb, 
		0x00004000, 
		0x80000000, 
		0x0e000fff, 
		0xc1e80000, 
		0xfb656620, 
		0x42040000, 
		0xf415dc0d, 
		0xdded27e0, 
		0x41200000, 
		0xc22c0000, 
		0x7f7ffffe, 
		0xc25c0000, 
		0xae9f0656, 
		0xb7e2253a, 
		0x00000000, 
		0x00800001, 
		0x80000000, 
		0x00020000, 
		0x4b000000, 
		0x80000000, 
		0xff800000, 
		0x00004000, 
		0x42340000, 
		0xc1200000, 
		0x7f000000, 
		0x0e7fffff, 
		0xc0c00000, 
		0x0fe704a3, 
		0x7f8d1bfe, 
		0xc2580000, 
		0xae4b39a9, 
		0x60b80c9f, 
		0x0c7ff000, 
		0xc2180000, 
		0xc2640000, 
		0x42740000, 
		0x38a943e4, 
		0x4b8c4b40, 
		0x80001000, 
		0x01b26ece, 
		0x80000200, 
		0x80011111, 
		0x0e0007ff, 
		0xc2340000, 
		0x8120b5d1, 
		0x00800003, 
		0xc1a00000, 
		0xef606c15, 
		0xff800000, 
		0x00800002, 
		0x00008000, 
		0x7073c805, 
		0xba4804a3, 
		0x804f952c, 
		0xb06373cc, 
		0x900ce2cc, 
		0xc1880000, 
		0x0e00ffff, 
		0x6c0fb651, 
		0x80800000, 
		0x80800000, 
		0xea168dcf, 
		0x0d00fff0, 
		0x29478d0e, 
		0x7f7ffffe, 
		0x4b000000, 
		0x0c780000, 
		0x58276f2c, 
		0x9bc29480, 
		0x00000001, 
		0xc2280000, 
		0xc5cd384c, 
		0xcb000000, 
		0x0e000007, 
		0xbffb1ff3, 
		0xc1b80000, 
		0x007fffff, 
		0xbf800000, 
		0x00000000, 
		0x7fc00000, 
		0x80800001, 
		0x4c1d6976, 
		0xa5885a87, 
		0x003460cf, 
		0x00000001, 
		0x7f800000, 
		0x5b707991, 
		0x40c00000, 
		0x00000040, 
		0x00800001, 
		0x00000000, 
		0x780c96fa, 
		0x4b000000, 
		0x07197739, 
		0x00080000, 
		0x4a51769e, 
		0x9d9762f2, 
		0xeebefe21, 
		0x7f7ffffe, 
		0x80000000, 
		0xcb8c4b40, 
		0x0f7ffffd, 
		0x42200000, 
		0x0e65e287, 
		0x00004000, 
		0xc2140000, 
		0x40abba2c, 
		0xda010ceb, 
		0x426c0000, 
		0xcccccccc, 
		0x0c7fff80, 
		0x24dee7d6, 
		0x7f86faba, 
		0x7fc00000, 
		0xc2700000, 
		0xc1800000, 
		0x484efaff, 
		0x0d000ff0, 
		0xce4bccfb, 
		0x80000000, 
		0x00010000, 
		0x4815ed27, 
		0x807fffff, 
		0x0c7c0000, 
		0x00400000, 
		0x00011111, 
		0x7f7fffff, 
		0xbf800001, 
		0xa2f7d157, 
		0xdf324e30, 
		0x91e8cf12, 
		0xbf800001, 
		0xbf800001, 
		0xfd0e0876, 
		0xe3f54f88, 
		0xc2140000, 
		0xd565ac51, 
		0x7f7ffffe, 
		0x00000000, 
		0x0e001fff, 
		0x42580000, 
		0x41500000, 
		0x42580000, 
		0x41c00000, 
		0x80800000, 
		0x80000008, 
		0xe728ac36, 
		0x007fffff, 
		0x5a70e3ab, 
		0x4206f035, 
		0xc2480000, 
		0x5fdbb6f8, 
		0x0c7c0000, 
		0x00800002, 
		0x424c0000, 
		0x807ffffe, 
		0x41e00000, 
		0x2a87eb01, 
		0xff800000, 
		0xb61bb233, 
		0x0e3fffff, 
		0xb0e44fbd, 
		0x42000000, 
		0xc0a00000, 
		0x7f800000, 
		0x80000001, 
		0x00000800, 
		0x3328ea43, 
		0xc1300000, 
		0x272a5eb0, 
		0xc1880000, 
		0x00000000, 
		0xc1f80000, 
		0xe03a717b, 
		0x0c7fffe0, 
		0x007fffff, 
		0x4b49f749, 
		0x0e0003ff, 
		0x0e00ffff, 
		0x00800003, 
		0x80000000, 
		0xc2540000, 
		0x41100000, 
		0x41d80000, 
		0x42580000, 
		0x7fc00001, 
		0xff800000, 
		0xff800000, 
		0x005d503f, 
		0xe41275f8, 
		0xff800000, 
		0x3ed28056, 
		0x12f149e0, 
		0xa90c65bc, 
		0x0d00fff0, 
		0x42780000, 
		0x0e000001, 
		0x709ddb20, 
		0xe536c313, 
		0xc2200000, 
		0xff800000, 
		0x7fbfffff, 
		0x7f800001, 
		0xc1800000, 
		0x3926916e, 
		0x80000080, 
		0x40e00000, 
		0x64a02d26, 
		0x00800001, 
		0x16fa8c67, 
		0xc1f00000, 
		0xc24c0000, 
		0x7f800000, 
		0x41d80000, 
		0x41500000, 
		0xb4de1c33, 
		0xe2791552, 
		0x00000000, 
		0x80000000, 
		0xc0e00000, 
		0x00000001, 
		0x1236ecd0, 
		0x7f8f12e3, 
		0x80000800, 
		0x49f71da5, 
		0x00000000, 
		0xcb000000, 
		0x284b588f, 
		0xffffffff, 
		0x0f7ffffd, 
		0x00000002, 
		0x00000800, 
		0xff7ffffe, 
		0x266dee41, 
		0x00800000, 
		0x80000002, 
		0x7f800000, 
		0x80800001, 
		0x41d00000, 
		0x26409703, 
		0xe626ab02, 
		0x0c7ff800, 
		0x426c0000, 
		0x80000000, 
		0x80800001, 
		0x7f800000, 
		0xdde1e17c, 
		0xc1900000, 
		0x80000000, 
		0x14b01b02, 
		0x7f800000, 
		0x7fafd978, 
		0x023747fa, 
		0x2dc94841, 
		0x8b6564a5, 
		0xc2340000, 
		0x0c7c0000, 
		0x00100000, 
		0x0c7fc000, 
		0xc24c0000, 
		0x94fa1865, 
		0x0f7ffffd, 
		0x42640000, 
		0x7fbfffff, 
		0xe4f0322b, 
		0x00000800, 
		0x41600000, 
		0x0d322c5f, 
		0x80011111, 
		0xcdc0a0ab, 
		0xc1200000, 
		0x9bd3605e, 
		0xc1200000, 
		0x7f000000, 
		0x5565fe73, 
		0x3f800001, 
		0x8812540e, 
		0xb540853e, 
		0x7f800000, 
		0xb348676a, 
		0x00000100, 
		0x41100000, 
		0xbcb131b1, 
		0x0e00ffff, 
		0x41b00000, 
		0x852a60f8, 
		0x80800003, 
		0x0f7ffffc, 
		0x04a8c3b6, 
		0xc2180000, 
		0xc1400000, 
		0x55555555, 
		0x0e000001, 
		0x3f800000, 
		0xff7ffffe, 
		0x80000000, 
		0x80000000, 
		0x42600000, 
		0x770b893d, 
		0x49892e68, 
		0xc2380000, 
		0x36da94a5, 
		0x00000000, 
		0x0f7fffff, 
		0x00008000, 
		0x40400000, 
		0x00020000, 
		0x80800001, 
		0x7fffffff, 
		0x7f800000, 
		0x0c700000, 
		0x80000001, 
		0x78c8c79f, 
		0xa7bc2778, 
		0x80000000, 
		0xffffffff, 
		0xcccccccc, 
		0x00000000, 
		0xf7e22d44, 
		0x0c7c0000, 
		0xc1800000, 
		0x6ce6ea6a, 
		0x420c0000, 
		0x423c0000, 
		0x27f84dc3, 
		0x42300000, 
		0x7fa85b48, 
		0xc2300000, 
		0xff7ffffe, 
		0x80000002, 
		0x00000200, 
		0x00000000, 
		0xcccccccc, 
		0x42340000, 
		0xbfc1fca3, 
		0x00000004, 
		0x7fc00000, 
		0x41e00000, 
		0x0c7fffff, 
		0x80800000, 
		0xc1600000, 
		0x80000008, 
		0xff800000, 
		0xca6855a4, 
		0xff7ffffe, 
		0x41400000, 
		0x80000000, 
		0x42280000, 
		0xbaa5d703, 
		0x3f800000, 
		0xff7ffffe, 
		0xff800000, 
		0x41a00000, 
		0x42100000, 
		0x00800001, 
		0xc1e00000, 
		0x426c0000, 
		0xffc00001, 
		0x80000010, 
		0x00004000, 
		0x00000001, 
		0xc1600000, 
		0x00000040, 
		0x7f7ffffe, 
		0xc2340000, 
		0x786c006c, 
		0xd175800f, 
		0x3f800001, 
		0x67f2d481, 
		0xf6f60af9, 
		0xc1980000, 
		0xc1b80000, 
		0x7fd0ea56, 
		0x5b41e640, 
		0x42100000, 
		0xc1800000, 
		0xc2240000, 
		0x87955d18, 
		0x00000000, 
		0x00000000, 
		0x00000000, 
		0x7f800000, 
		0x3f800001, 
		0xc1b80000, 
		0x0c7fffe0, 
		0x7f800000, 
		0x80000004, 
		0x80002000, 
		0x70720e22, 
		0x50406724, 
		0xc27c0000, 
		0x7f8437a1, 
		0x7f800000, 
		0x80000000, 
		0xaaaaaaaa, 
		0x0d000ff0, 
		0xff7ffffe, 
		0x00000020, 
		0x952f6332, 
		0x00be386d, 
		0x5a5a72dc, 
		0x41700000, 
		0x0e000007, 
		0x80004000, 
		0x00000000, 
		0x807fffff, 
		0x80000000, 
		0xa923a2bf, 
		0x0c7ffff8, 
		0xcb000000, 
		0x42240000, 
		0x0f7ffffe, 
		0xffa57fc4, 
		0x18618141, 
		0x00010000, 
		0xc0a00000, 
		0x42040000, 
		0x7f800000, 
		0x7f7fffff, 
		0x42200000, 
		0x0e007fff, 
		0xc2680000, 
		0xc1d80000, 
		0x8e6496ee, 
		0xc2140000, 
		0x2250a605, 
		0x42140000, 
		0x7bea5680, 
		0x0e3fffff, 
		0x0e00001f, 
		0x7b54106d, 
		0x426b7b83, 
		0x0c7ff000, 
		0x00800002, 
		0x22fe271c, 
		0x0f7ffffe, 
		0xc32caf1c, 
		0x42193585, 
		0x80080000, 
		0xc1300000, 
		0xd32f2835, 
		0x0e0fffff, 
		0x00011111, 
		0x00008000, 
		0x3ffc3828, 
		0x951f769c, 
		0xc2740000, 
		0x00000000, 
		0xaaaaaaaa, 
		0x7f7ffffe, 
		0x40000000, 
		0x426c0000, 
		0x00800001, 
		0xff800000, 
		0x80000000, 
		0x0c780000, 
		0x98c95b69, 
		0x3f800000, 
		0x4741012b, 
		0x0c7f0000, 
		0x3e31f168, 
		0x0c1acf50, 
		0x8007a992, 
		0xff800000, 
		0x0c7f0000, 
		0xc1000000, 
		0x00800002, 
		0xc20c0000, 
		0xafe8a2b6, 
		0x7f7ffffe, 
		0x42540000, 
		0x00415aa5, 
		0x40800000, 
		0x39d07a23, 
		0x42780000, 
		0x00000000, 
		0xc1e80000, 
		0x00000001, 
		0x42380000, 
		0xc1400000, 
		0x7f7ffffe, 
		0x422c0000, 
		0x455aa63e, 
		0xf9b70770, 
		0xc1d00000, 
		0x80ffffff, 
		0x80000000, 
		0x80000010, 
		0x6d222d7a, 
		0x2c9f5717, 
		0x41700000, 
		0x41a80000, 
		0xf30ce3d6, 
		0x00200000, 
		0xc1880000, 
		0x0c7f8000, 
		0x2ac7d4a5, 
		0x80281b84, 
		0x0c7fff80, 
		0x0c7ffffc, 
		0x00020000, 
		0xff8c22ed, 
		0x55ed81e3, 
		0x877838d6, 
		0xc26c0000, 
		0x945a3bd1, 
		0x0e00000f, 
		0xd5b18361, 
		0x80100000, 
		0x68647494, 
		0x0c7fff80, 
		0x7f000000, 
		0x33333333, 
		0xc0800000, 
		0x80758de4, 
		0xc24c0000, 
		0x00080000, 
		0x7f000000, 
		0xb1d701c7, 
		0x0c7fe000, 
		0xc27c0000, 
		0x424c0000, 
		0x00000000, 
		0x007fffff, 
		0x0e0003ff, 
		0x0e7fffff, 
		0x3f800000, 
		0x67301f4f, 
		0x41f80000, 
		0x00000001, 
		0x00ffffff, 
		0xff800000, 
		0x80040000, 
		0x80800000, 
		0x0e7fffff, 
		0x41900000, 
		0x41c00000, 
		0x00800000, 
		0x00400000, 
		0xebbfd9de, 
		0x55555555, 
		0x0c7ffff0, 
		0xc1300000, 
		0x80000010, 
		0x268946a0, 
		0xe86ff4e1, 
		0x80004000, 
		0x9f0e812c, 
		0x42580000, 
		0xffc00000, 
		0x4dfc262a, 
		0xc1f80000, 
		0x80000000, 
		0xc2000000, 
		0xbac83527, 
		0x80800001, 
		0x1a389c96, 
		0x07ad94cf, 
		0x00000000, 
		0x42200000, 
		0xabb4d776, 
		0xc1800000, 
		0x8930a825, 
		0x7f800000, 
		0xc1300000, 
		0xc2200000, 
		0x61c9753d, 
		0xc2740000, 
		0x42140000, 
		0x80800000, 
		0x13324724, 
		0x00000000, 
		0x00800002, 
		0xcc646595, 
		0x0c600000, 
		0xff885b5b, 
		0x00001000, 
		0x00002000, 
		0xff7fffff, 
		0x89c35e40, 
		0x41f80000, 
		0x41100000, 
		0xc1a00000, 
		0x0c7fff00, 
		0x9ed4e67c, 
		0x42740000, 
		0x32fb3da6, 
		0x0c90c7a5, 
		0x322e89fc, 
		0x00000000, 
		0x40400000, 
		0x80800002, 
		0x00000000, 
		0x7f800000, 
		0x7f000000, 
		0xc2480000, 
		0x80800000, 
		0x220488bc, 
		0xc22c0000, 
		0x0c7f8000, 
		0x80002000, 
		0x3f028f5c, 
		0xe8164a30, 
		0x00020000, 
		0x1e04dcd7, 
		0x80000001, 
		0xff800000, 
		0x8efc4325, 
		0x4eb54f6a, 
		0xff800000, 
		0x42740000, 
		0xc1d80000, 
		0xe9a104c5, 
		0x80000000, 
		0x26d4c45b, 
		0x0e1fffff, 
		0x7f800000, 
		0x0e03ffff, 
		0x42100000, 
		0xedc2a7a0, 
		0x0e000fff, 
		0x00400000, 
		0xa765f7da, 
		0x80000080, 
		0xff7ffffe, 
		0xc1300000, 
		0x41d00000, 
		0x00000000, 
		0x7f9e5c1c, 
		0x427c0000, 
		0x89bb8f8b, 
		0xc20c0000, 
		0x00000001, 
		0x80000000, 
		0x80000800, 
		0x41100000, 
		0x42240000, 
		0xf9f19df7, 
		0x76c98223, 
		0x0e000007, 
		0x41980000, 
		0x0f7ffffd, 
		0xff800000, 
		0x7e9b34f4, 
		0xffbd2505, 
		0x424c0000, 
		0x80000800, 
		0x3f109031, 
		0xcc66c252, 
		0x0e0003ff, 
		0x007ffffe, 
		0xc2680000, 
		0xe1eea11b, 
		0xc20c0000, 
		0x80800000, 
		0x80000008, 
		0x80000000, 
		0x80000000, 
		0x41d00000, 
		0x427c0000, 
		0xc2500000, 
		0x00000008, 
		0x09538305, 
		0x68a7c659, 
		0x6b3aeb6f, 
		0x42180000, 
		0xc2100000, 
		0xc2100000, 
		0x00000001, 
		0x0f8bea04, 
		0x0efe3d32, 
		0x80020000, 
		0x00000002, 
		0xaf77f99f, 
		0x5c50bb72, 
		0xb7b9b0af, 
		0x80000000, 
		0xc2000000, 
		0xa367c026, 
		0xffc00000, 
		0x00000000, 
		0x41d80000, 
		0x00000000, 
		0xbbab41f9, 
		0x00800000, 
		0x67b242b4, 
		0xbdffb85d, 
		0xe49a0a7a, 
		0x65196fc2, 
		0xc2580000, 
		0x0e3fffff, 
		0x7fc00001, 
		0x80000000, 
		0xeaa087f8, 
		0xc2380000, 
		0x0d00fff0, 
		0x00008000, 
		0xc1980000, 
		0xff800000, 
		0x00800001, 
		0x42200000, 
		0xc1200000, 
		0x7fbe3d2a, 
		0x7fd8af04, 
		0xc2340000, 
		0x61251c71, 
		0x7f800000, 
		0x7f800000, 
		0xd457873f, 
		0xd3d06050, 
		0x807ffffe, 
		0x4d22dc26, 
		0xff7ffffe, 
		0x0e0000ff, 
		0xb8b18c85, 
		0xc2500000, 
		0xff800000, 
		0x41e00000, 
		0xc2680000, 
		0xc81f24ba, 
		0x80000000, 
		0xa87692f9, 
		0x41b80000, 
		0x80000000, 
		0x0e8da967, 
		0x0d7a0ef1, 
		0x00000000, 
		0x0d000ff0, 
		0xc23c0000, 
		0xc2400000, 
		0x00000001, 
		0xd7b20928, 
		0x00800003, 
		0x42240000, 
		0x22b64346, 
		0x80004000, 
		0x0c7ffffe, 
		0x0c7fc000, 
		0xf229fdc2, 
		0xf05924eb, 
		0xf2803ff4, 
		0xfff5c8f8, 
		0xc2400000, 
		0x424c0000, 
		0x2c303cbd, 
		0x00000800, 
		0xc1f80000, 
		0xc2700000, 
		0xc1900000, 
		0x6b2093f6, 
		0x3a4133d0, 
		0xef24011a, 
		0xc005554d, 
		0xc35a8d2b, 
		0x071bd273, 
		0x14f72dce, 
		0x80000000, 
		0x0c7fff00, 
		0xc2600000, 
		0x00cc8a15, 
		0xc2340000, 
		0x41d80000, 
		0x42040000, 
		0x0e03ffff, 
		0xda6bd827, 
		0x7fc00001, 
		0x76bbe2be, 
		0xc190da2c, 
		0xc2780000, 
		0x00000000, 
		0x42540000, 
		0x80200000, 
		0x00000000, 
		0x80000000, 
		0xff000000, 
		0x7f800000, 
		0xc2040000, 
		0xc22c0000, 
		0xf4eec8e1, 
		0xa2994fcc, 
		0x7fc00000, 
		0x3de6359f, 
		0x41300000, 
		0x00000020, 
		0xc2780000, 
		0x958077d8, 
		0xff800000, 
		0x42080000, 
		0x42140000, 
		0x41880000, 
		0x80800000, 
		0x6636a477, 
		0x41400000, 
		0x80000100, 
		0x8f7ffffd, 
		0x5acdbf57, 
		0x6638e79d, 
		0x00000000, 
		0x5d1d1e65, 
		0x00000100, 
		0x00000080, 
		0xff800000, 
		0x80000000, 
		0x7fc00001, 
		0x9a78f6ea, 
		0xcdcbd1ed, 
		0xc2680000, 
		0x00040000, 
		0x00011111, 
		0x41e00000, 
		0xc3f154c8, 
		0x0e0003ff, 
		0x0f7ffffe, 
		0x6dc8efee, 
		0xc2700000, 
		0x40220cec, 
		0xbf800001, 
		0x0c600000, 
		0xff8beab0, 
		0x40beb102, 
		0x7f800000, 
		0x41900000, 
		0x41b00000, 
		0x30f408f0, 
		0xc2480000, 
		0x807fffff, 
		0x80000400, 
		0xc1d00000, 
		0xc1000000, 
		0x0e0003ff, 
		0xa3cac6db, 
		0x80000001, 
		0xcccccccc, 
		0xc2740000, 
		0xbf028f5c, 
		0x00100000, 
		0x42200000, 
		0x0f7fffff, 
		0xc2340000, 
		0x00100000, 
		0x0e00ffff, 
		0xc2600000, 
		0x00800000, 
		0x0e03ffff, 
		0x55555555, 
		0x41500000, 
		0x427c0000, 
		0xced8af4f, 
		0x00000000, 
		0x00100000, 
		0x802593f8, 
		0xbf800001, 
		0xc2080000, 
		0xa1a8fd68, 
		0x0c7fffc0, 
		0x0c7ffc00, 
		0xc2280000, 
		0x00080000, 
		0x3b56d484, 
		0x0ed1e043, 
		0x0c7fe000, 
		0x00000002, 
		0x7f800000, 
		0xc21c0000, 
		0x40e00000, 
		0xc2400000, 
		0x41f80000, 
		0x9ea04a2e, 
		0x00800001, 
		0x80004000, 
		0xe4fd1e44, 
		0x8f7fffff, 
		0xc2600000, 
		0xff9753ba, 
		0x5a7dd597, 
		0xf0253c02, 
		0xf2b66191, 
		0xbf800000, 
		0x4c4508f8, 
		0x40a00000, 
		0x42540000, 
		0x0e0007ff, 
		0xc2280000, 
		0x6259d664, 
		0xb09a5387, 
		0x007fffff, 
		0x80800001, 
		0xc1a00000, 
		0xc1e00000, 
		0x0c7ffff0, 
		0x7f800000, 
		0xc6e9c84c, 
		0xc4611f72, 
		0xa968938e, 
		0x00000001, 
		0xf8663770, 
		0xff800000, 
		0x55555555, 
		0x80000000, 
		0x5db1b632, 
		0x00001000, 
		0x0e00007f, 
		0x42480000, 
		0xa42dd6bb, 
		0x4b4bbb72, 
		0x41c80000, 
		0xc804406c, 
		0xc2180000, 
		0x80000000, 
		0xff800000, 
		0x7f7ffffe, 
		0xc24c0000, 
		0x80000000, 
		0x41200000, 
		0x3f800000, 
		0x0e0001ff, 
		0x7fffffff, 
		0x5c6b55a0, 
		0x3c8e4b71, 
		0x1fd2ba53, 
		0x80000080, 
		0x41200000, 
		0x35e28144, 
		0xb6dac38d, 
		0xc0e00000, 
		0xc27c0000, 
		0x71dd6451, 
		0x0e3fffff, 
		0xf1a0424f, 
		0x57657ced, 
		0x80000004, 
		0xff800000, 
		0xc25c0000, 
		0xc2380000, 
		0x00000000, 
		0x80800002, 
		0xf80afd31, 
		0x0c7fffff, 
		0x41700000, 
		0x80011111, 
		0x8eb79822, 
		0x04e4eb4d, 
		0x00800000, 
		0x4be25e11, 
		0x42780000, 
		0x492153b3, 
		0x4058d81b, 
		0x7fffffff, 
		0x0e3fffff, 
		0xbf22c117, 
		0x80000000, 
		0x00800001, 
		0xc0800000, 
		0x80020000, 
		0x42140000, 
		0x0c7ffffc, 
		0x0e00001f, 
		0x00000000, 
		0x7f7ffffe, 
		0xc23b9ba9, 
		0xc1300000, 
		0x00000020, 
		0x0e001fff, 
		0xed758ad5, 
		0x80000000, 
		0xc2500000, 
		0x0e00000f, 
		0x80100000, 
		0x00ffffff, 
		0xff7ffffe, 
		0x00000000, 
		0x00011111, 
		0xc1e00000, 
		0x402fb790, 
		0x4a2c6d67, 
		0x54c41437, 
		0x0c7f0000, 
		0xb9bfa33f, 
		0xc1980000, 
		0xe42f6446, 
		0x00000002, 
		0x7f800000, 
		0xc1980000, 
		0xff910a26, 
		0xc1b80000, 
		0x00800000, 
		0x00000010, 
		0xbf028f5c, 
		0x8f7ffffd, 
		0x0d000ff0, 
		0xd4632aac, 
		0xc0e00000, 
		0x7b2228d9, 
		0xa4b39b8f, 
		0xc1d00000, 
		0xc1b80000, 
		0x7f7ffffe, 
		0xbc441ca3, 
		0x80100000, 
		0xdddd0ed9, 
		0xc1500000, 
		0x0c7ff800, 
		0x00000000, 
		0x80020000, 
		0x71c872aa, 
		0x04d33130, 
		0xbf800001, 
		0xb0cdfff0, 
		0x80800002, 
		0x0e00007f, 
		0x00000002, 
		0xc1800000, 
		0xc2780000, 
		0x70753b4b, 
		0x7859f13f, 
		0x0c7fffc0, 
		0x07397300, 
		0xd2188d61, 
		0x00000000, 
		0x43ffa91b, 
		0xec1d9e63, 
		0x7fdfc002, 
		0x1a276a1b, 
		0xc2500000, 
		0x8f7ffffc, 
		0xff9391e3, 
		0x9e6b6db4, 
		0x00000200, 
		0x9e108513, 
		0x00011111, 
		0x041f4190, 
		0x41000000, 
		0x80000000, 
		0x00011111, 
		0x80011111, 
		0x0c780000, 
		0x7f7fffff, 
		0x4b8c4b40, 
		0x42540000, 
		0x80000000, 
		0xc1000000, 
		0x80100000, 
		0x41900000, 
		0xc1000000, 
		0xde6b301c, 
		0x41b80000, 
		0xeaeeb034, 
		0x5262ace5, 
		0xc1a00000, 
		0x42640000, 
		0x0f7fffff, 
		0xc2400000, 
		0x8075e01c, 
		0x5c233dee, 
		0x00000010, 
		0xb6ba3421, 
		0x23821e59, 
		0x01480006, 
		0xc2300000, 
		0x42740000, 
		0x40e00000, 
		0x00000000, 
		0xef5cc13d, 
		0xb313bc9d, 
		0x41e80000, 
		0x4f67f60f, 
		0xc2640000, 
		0x80080000, 
		0x80000000, 
		0x00000020, 
		0x42500000, 
		0x0e00ffff, 
		0xbf028f5c, 
		0x00000000, 
		0x272f8459, 
		0x00800001, 
		0x0e0fffff, 
		0x80000000, 
		0x0c7fe000, 
		0x8ab04aa1, 
		0x2f8ecf89, 
		0x41880000, 
		0x80800001, 
		0x41a80000, 
		0x80000008, 
		0xffc00001, 
		0x80ffffff, 
		0x7f800000, 
		0xcb8c4b40, 
		0x80000000, 
		0xedbf99e9, 
		0xd6ce3116, 
		0xec81d2cc, 
		0xc13a9c8c, 
		0x8010f131, 
		0xff800000, 
		0x3872ce11, 
		0x0e0001ff, 
		0x427c0000, 
		0xcb000000, 
		0x0e00000f, 
		0x42280000, 
		0x007fffff, 
		0x7f86e4b4, 
		0x00011111, 
		0xff7ffffe, 
		0xc1100000, 
		0x80000000, 
		0xd33a34d4, 
		0x42580000, 
		0xc1a00000, 
		0xd24a34f0, 
		0x03416594, 
		0x22d1961c, 
		0x80800003, 
		0x80ffffff, 
		0xc1880000, 
		0xff7fffff, 
		0xaaaaaaaa, 
		0xc2180000, 
		0x00000002, 
		0xcb000000, 
		0x0c7f8000, 
		0x0e0fffff, 
		0x4627a8c0, 
		0x49e2bc59, 
		0x7fbfffff, 
		0x80800001, 
		0x92f63676, 
		0x7fc00001, 
		0x0c7ffffc, 
		0xc23c0000, 
		0x40000000, 
		0xc2780000, 
		0xc2680000, 
		0x80020000, 
		0x41a80000, 
		0xc1400000, 
		0x41900000, 
		0x50aeea56, 
		0xa15a2ab3, 
		0x900c3503, 
		0x7f7fffff, 
		0xc2600000, 
		0x80040000, 
		0x00000000, 
		0xcbbe5022, 
		0x2c986646, 
		0x80800001, 
		0x00010000, 
		0xb31419b2, 
		0x0c700000, 
		0xfd566408, 
		0xff800000, 
		0x7ae3be1a, 
		0x427c0000, 
		0x80800001, 
		0x80800001, 
		0x40a00000, 
		0x41000000, 
		0x00002000, 
		0x7f96c8f8  
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

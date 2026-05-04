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
			"csrwi mhpmevent3, 20\n"   
			"csrwi mhpmevent4, 8\n"   
			"csrwi mhpmevent5, 14\n"   
			"csrwi mhpmevent6, 17\n"   
			"csrwi mhpmevent7, 3\n"   
			"csrwi mhpmevent8, 6\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f14,  640 (x5)\n"
			"flw.ps f31,  224 (x5)\n"
			"flw.ps f3,  288 (x5)\n"
			"flw.ps f10,  736 (x5)\n"
			"flw.ps f15,  192 (x5)\n"
			"flw.ps f11,  992 (x5)\n"
			"flw.ps f28,  832 (x5)\n"
			"flw.ps f24,  512 (x5)\n"
			"flw.ps f26,  928 (x5)\n"
			"flw.ps f9,  384 (x5)\n"
			"flw.ps f21,  160 (x5)\n"
			"flw.ps f17,  448 (x5)\n"
			"flw.ps f5,  576 (x5)\n"
			"flw.ps f16,  768 (x5)\n"
			"flw.ps f1,  608 (x5)\n"
			"flw.ps f22,  864 (x5)\n"
			"flw.ps f23,  0 (x5)\n"
			"flw.ps f8,  64 (x5)\n"
			"flw.ps f19,  480 (x5)\n"
			"flw.ps f25,  800 (x5)\n"
			"flw.ps f2,  416 (x5)\n"
			"flw.ps f18,  672 (x5)\n"
			"flw.ps f0,  96 (x5)\n"
			"flw.ps f4,  352 (x5)\n"
			"flw.ps f6,  704 (x5)\n"
			"flw.ps f20,  128 (x5)\n"
			"flw.ps f27,  544 (x5)\n"
			"flw.ps f12,  960 (x5)\n"
			"flw.ps f29,  256 (x5)\n"
			"flw.ps f30,  320 (x5)\n"
			"flw.ps f7,  32 (x5)\n"
			"flw.ps f13,  896 (x5)\n"
			"LBL_VADDR_INIT_HID_0:\n"          
			"li x24, 0x8101ee0000 \n"                 
			"fsw.ps f21, (x24) \n"                 
			"li x13, 0x8102339820 \n"                 
			"fsw.ps f3, (x13) \n"                 
			"li x14, 0x81027db340 \n"                 
			"fsw.ps f15, (x14) \n"                 
			"li x21, 0x8102d32160 \n"                 
			"fsw.ps f29, (x21) \n"                 
			"li x15, 0x81035c0020 \n"                 
			"fsw.ps f15, (x15) \n"                 
			"li x14, 0x8105e40680 \n"                 
			"fsw.ps f2, (x14) \n"                 
			"li x25, 0x810824ee80 \n"                 
			"fsw.ps f18, (x25) \n"                 
			"li x12, 0x81096c5800 \n"                 
			"fsw.ps f0, (x12) \n"                 
			"li x19, 0x810b43b920 \n"                 
			"fsw.ps f17, (x19) \n"                 
			"li x17, 0x810c0824c0 \n"                 
			"fsw.ps f29, (x17) \n"                 
			"li x21, 0x810c7e0020 \n"                 
			"fsw.ps f5, (x21) \n"                 
			"li x25, 0x810d7e0000 \n"                 
			"fsw.ps f7, (x25) \n"                 
			"li x24, 0x810da13800 \n"                 
			"fsw.ps f16, (x24) \n"                 
			"li x22, 0x8200120020 \n"                 
			"fsw.ps f21, (x22) \n"                 
			"li x18, 0x820171d020 \n"                 
			"fsw.ps f16, (x18) \n"                 
			"li x24, 0x8202a42000 \n"                 
			"fsw.ps f15, (x24) \n"                 
			"li x12, 0x8205060000 \n"                 
			"fsw.ps f27, (x12) \n"                 
			"li x19, 0x8205de5800 \n"                 
			"fsw.ps f23, (x19) \n"                 
			"li x13, 0x8206555000 \n"                 
			"fsw.ps f17, (x13) \n"                 
			"li x17, 0x8206920000 \n"                 
			"fsw.ps f0, (x17) \n"                 
			"li x13, 0x82074e0760 \n"                 
			"fsw.ps f10, (x13) \n"                 
			"li x20, 0x82095406c0 \n"                 
			"fsw.ps f10, (x20) \n"                 
			"li x24, 0x820a3c0160 \n"                 
			"fsw.ps f9, (x24) \n"                 
			"li x18, 0x820a620300 \n"                 
			"fsw.ps f10, (x18) \n"                 
			"li x12, 0x820afc3020 \n"                 
			"fsw.ps f9, (x12) \n"                 
			"li x10, 0x820d5c04e0 \n"                 
			"fsw.ps f20, (x10) \n"                 
			"li x17, 0x820dcf2ea0 \n"                 
			"fsw.ps f3, (x17) \n"                 
			"fence\n"
			"LBL_GLOBAL_VADDR_INIT_HID_0:\n"          
			"li x13, 0x8121ee0000 \n"                 
			"fswg.ps f28, (x13) \n"                 
			"li x20, 0x81250e06a0 \n"                 
			"fswg.ps f24, (x20) \n"                 
			"li x24, 0x812824ee80 \n"                 
			"fswg.ps f24, (x24) \n"                 
			"li x10, 0x8129460340 \n"                 
			"fswg.ps f25, (x10) \n"                 
			"li x25, 0x81296c5800 \n"                 
			"fswg.ps f12, (x25) \n"                 
			"li x12, 0x812ae6f700 \n"                 
			"fswg.ps f0, (x12) \n"                 
			"li x11, 0x812b43b920 \n"                 
			"fswg.ps f23, (x11) \n"                 
			"li x22, 0x812c7e0020 \n"                 
			"fswg.ps f20, (x22) \n"                 
			"li x19, 0x8132339820 \n"                 
			"fswg.ps f26, (x19) \n"                 
			"li x13, 0x813895f820 \n"                 
			"fswg.ps f26, (x13) \n"                 
			"li x18, 0x8139e20020 \n"                 
			"fswg.ps f15, (x18) \n"                 
			"li x23, 0x813aa40120 \n"                 
			"fswg.ps f11, (x23) \n"                 
			"li x24, 0x813c0824c0 \n"                 
			"fswg.ps f22, (x24) \n"                 
			"li x24, 0x813d7e0000 \n"                 
			"fswg.ps f7, (x24) \n"                 
			"li x24, 0x813da13800 \n"                 
			"fswg.ps f27, (x24) \n"                 
			"li x17, 0x822171d020 \n"                 
			"fswg.ps f19, (x17) \n"                 
			"li x10, 0x8222a42000 \n"                 
			"fswg.ps f21, (x10) \n"                 
			"li x24, 0x82295406c0 \n"                 
			"fswg.ps f11, (x24) \n"                 
			"li x19, 0x8229b00720 \n"                 
			"fswg.ps f28, (x19) \n"                 
			"li x25, 0x822a3c0160 \n"                 
			"fswg.ps f17, (x25) \n"                 
			"li x19, 0x822b7a46c0 \n"                 
			"fswg.ps f27, (x19) \n"                 
			"li x24, 0x822d5c04e0 \n"                 
			"fswg.ps f28, (x24) \n"                 
			"li x18, 0x822dcf2ea0 \n"                 
			"fswg.ps f4, (x18) \n"                 
			"li x14, 0x8235de5800 \n"                 
			"fswg.ps f16, (x14) \n"                 
			"li x25, 0x8236555000 \n"                 
			"fswg.ps f13, (x25) \n"                 
			"li x14, 0x823a620300 \n"                 
			"fswg.ps f12, (x14) \n"                 
			"li x12, 0x823acc0020 \n"                 
			"fswg.ps f24, (x12) \n"                 
			"fence\n"
			"LBL_LOCAL_VADDR_INIT_HID_0:\n"          
			"li x20, 0x8142339820 \n"                 
			"fswl.ps f20, (x20) \n"                 
			"li x16, 0x81427db340 \n"                 
			"fswl.ps f1, (x16) \n"                 
			"li x24, 0x81435c0020 \n"                 
			"fswl.ps f2, (x24) \n"                 
			"li x13, 0x8145e40680 \n"                 
			"fswl.ps f14, (x13) \n"                 
			"li x16, 0x8148160000 \n"                 
			"fswl.ps f10, (x16) \n"                 
			"li x17, 0x814895f820 \n"                 
			"fswl.ps f28, (x17) \n"                 
			"li x16, 0x814aa40120 \n"                 
			"fswl.ps f8, (x16) \n"                 
			"li x18, 0x814b43b920 \n"                 
			"fswl.ps f13, (x18) \n"                 
			"li x14, 0x814d7e0000 \n"                 
			"fswl.ps f28, (x14) \n"                 
			"li x25, 0x8151ee0000 \n"                 
			"fswl.ps f29, (x25) \n"                 
			"li x19, 0x81550e06a0 \n"                 
			"fswl.ps f2, (x19) \n"                 
			"li x16, 0x815824ee80 \n"                 
			"fswl.ps f16, (x16) \n"                 
			"li x14, 0x8159460340 \n"                 
			"fswl.ps f15, (x14) \n"                 
			"li x12, 0x81596c5800 \n"                 
			"fswl.ps f31, (x12) \n"                 
			"li x15, 0x815c7e0020 \n"                 
			"fswl.ps f27, (x15) \n"                 
			"li x17, 0x815ee69300 \n"                 
			"fswl.ps f22, (x17) \n"                 
			"li x18, 0x824171d020 \n"                 
			"fswl.ps f0, (x18) \n"                 
			"li x10, 0x8246920000 \n"                 
			"fswl.ps f21, (x10) \n"                 
			"li x18, 0x82495406c0 \n"                 
			"fswl.ps f30, (x18) \n"                 
			"li x10, 0x824b7a46c0 \n"                 
			"fswl.ps f5, (x10) \n"                 
			"li x18, 0x824e8b4760 \n"                 
			"fswl.ps f20, (x18) \n"                 
			"li x23, 0x8250120020 \n"                 
			"fswl.ps f8, (x23) \n"                 
			"li x21, 0x8252a42000 \n"                 
			"fswl.ps f8, (x21) \n"                 
			"li x14, 0x8255060000 \n"                 
			"fswl.ps f11, (x14) \n"                 
			"li x14, 0x8255de5800 \n"                 
			"fswl.ps f12, (x14) \n"                 
			"li x16, 0x8256555000 \n"                 
			"fswl.ps f19, (x16) \n"                 
			"li x17, 0x825a620300 \n"                 
			"fswl.ps f10, (x17) \n"                 
			"fence\n"
			"fence\n"
			"csrw fflags, zero\n"
			"csrwi frm, 0x0\n"
			"LBL_KERNEL_HID_0:\n"          
			"li x5, 0xFEEDFEED\n"
			"li x0, 0x5633d1ee03\n" 
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
			"LBL_SEQID_0_FP_SCATTER_ID_0_HID_0:\n"
			"li x28, 0x8205de580e\n" 
			"li x10, 0x27bc888360822e5a\n"
			"fsc32b.ps f27, x10 (x28)\n" 
			"mova.m.x x5\n"
			"fltm.pi m1, f27, f22\n"
			"blt x29, x0, 1f\n"
			"1:\n"
			"LBL_SEQID_0_FP_GATHER_ID_1_HID_0:\n"
			"li x22, 0x820d5c04ed\n" 
			"la x16, mem_ptr_offset\n"
			"flq2 f11, 120(x16)\n"
			"fscw.ps f19, f11 (x22)\n"
			"fgw.ps f15, f11 (x22)\n" 
			"fmax.ps f30, f4, f11\n"
			"srliw x14, x14, 0\n"
			"LBL_SEQID_0_INT_AMO_ID_2_HID_0:\n"
			"li x18, 0x822dcf2eac\n" 
			"amoxorg.w x25, x16, (x18)\n" 
			"slt x22, x7, x23\n"
			"sllw x10, x19, x23\n"
			"LBL_SEQID_0_FP_GATHER_ID_3_HID_0:\n"
			"li x28, 0x8102339832\n" 
			"li x25, 0xa0774eac9919fca4\n"
			"fsc32w.ps f13, x25 (x28)\n"
			"fg32b.ps f4, x25 (x28)\n" 
			"fmsub.s f1, f1, f30, f30, rne\n"
			"fsat8.pi f31, f9\n"
			"LBL_SEQID_0_INT_AMO_ID_4_HID_0:\n"
			"li x14, 0x814895f828\n" 
			"amoandl.d x23, x19, (x14)\n" 
			"fsrl.pi f4, f22, f4\n"
			"fnmsub.ps f9, f27, f24, f28, rne\n"
			"LBL_SEQID_0_INT_STORE_ID_5_HID_0:\n"
			"li x14, 0x810aa40134\n" 
			"sb x5, 1679(x14)\n" 
			"fslli.pi f14, f14, 0x8\n"
			"bne x2, x27, 1f\n"
			"1:\n"
			"xor x5, x5, x25\n"
			"LBL_SEQID_0_INT_AMO_ID_6_HID_0:\n"
			"li x14, 0x81235c0038\n" 
			"amomaxg.w x19, x22, (x14)\n" 
			"fmin.pi f22, f11, f4\n"
			"faddi.pi f14, f4, 0x128\n"
			"LBL_SEQID_0_FP_LOAD_ID_7_HID_0:\n"
			"li x23, 0x8205de580e\n" 
			"fsq2 f22, 1319(x23)\n"
			"flw.ps f27, 1319(x23)\n" 
			"fsgnjn.s f14, f24, f14\n"
			"bgeu x6, x4, 1f\n"
			"1:\n"
			"fmaxu.pi f30, f24, f30\n"
			"LBL_SEQID_0_INT_AMO_ID_8_HID_0:\n"
			"li x28, 0x81250e06b8\n" 
			"amoxorg.w x22, x10, (x28)\n" 
			"fsra.pi f14, f14, f27\n"
			"fcvt.ps.pwu f4, f16, rne\n"
			"LBL_SEQID_0_INT_AMO_ID_9_HID_0:\n"
			"li x23, 0x8129460340\n" 
			"amoandg.d x5, x14, (x23)\n" 
			"fmax.ps f30, f22, f12\n"
			"fmax.s f24, f24, f15\n"
			"LBL_SEQID_0_INT_AMO_ID_10_HID_0:\n"
			"li x25, 0x8246920008\n" 
			"amoswapl.d x14, x16, (x25)\n" 
			"fnmadd.s f15, f6, f6, f31, rdn\n"
			"feq.pi f15, f22, f31\n"
			"LBL_SEQID_0_INT_AMO_ID_11_HID_0:\n"
			"li x28, 0x82274e0768\n" 
			"amomaxug.d x5, x16, (x28)\n" 
			"fslli.pi f1, f1, 0x2\n"
			"fnmadd.ps f12, f30, f14, f11, rne\n"
			"LBL_SEQID_0_FP_AMO_ID_12_HID_0:\n"
			"li x25, 0x8246920008\n" 
			"la x28, mem_ptr_offset\n"
			"flw.ps f28, 992(x28)\n"
			"famoorl.pi f14, f28 (x25)\n" 
			"sltu x18, x16, x18\n"
			"blt x6, x11, 1f\n"
			"1:\n"
			"srlw x28, x14, x18\n"
			"LBL_SEQID_0_FP_G_GATHER_ID_13_HID_0:\n"
			"li x19, 0x813ee69310\n" 
			"la x14, mem_ptr_offset\n"
			"flw.ps f15, 16(x14)\n"
			"fscwg.ps f29, f15 (x19)\n"
			"fghg.ps f6, f15 (x19)\n" 
			"fsll.pi f4, f4, f4\n"
			"fmul.pi f15, f24, f16\n"
			"LBL_SEQID_0_FP_L_SCATTER_ID_14_HID_0:\n"
			"li x22, 0x814d7e0008\n" 
			"la x25, mem_ptr_offset\n"
			"flw.ps f1, 0(x25)\n"
			"fscwl.ps f12, f1 (x22)\n" 
			"fsatu8.pi f12, f22\n"
			"mul x10, x16, x10\n"
			"LBL_SEQID_0_FP_G_GATHER_ID_15_HID_0:\n"
			"li x16, 0x8235de580c\n" 
			"la x7, mem_ptr_offset\n"
			"flw.ps f15, 96(x7)\n"
			"fscwg.ps f20, f15 (x16)\n"
			"fghg.ps f4, f15 (x16)\n" 
			"flt.pi f11, f4, f9\n"
			"fcvt.ps.f16 f12, f1\n"
			"LBL_SEQID_0_INT_AMO_ID_16_HID_0:\n"
			"li x7, 0x8250120024\n" 
			"amominl.w x18, x16, (x7)\n" 
			"fnmadd.s f11, f27, f15, f15, rne\n"
			"fslli.pi f30, f22, 0x8\n"
			"LBL_SEQID_0_FP_AMO_ID_17_HID_0:\n"
			"li x5, 0x822d5c04ec\n" 
			"la x14, mem_ptr_offset\n"
			"flw.ps f31, 608(x14)\n"
			"famominug.pi f22, f31 (x5)\n" 
			"fmax.s f16, f28, f31\n"
			"fadd.ps f1, f12, f28, rne\n"
			"LBL_SEQID_0_INT_L_STORE_ID_18_HID_0:\n"
			"li x16, 0x81596c5808\n" 
			"sbl x5, (x16)\n" 
			"fsin.ps f15, f28\n"
			"fltm.ps m1, f28, f28\n"
			"LBL_SEQID_0_FP_GATHER_ID_19_HID_0:\n"
			"li x22, 0x81027db34d\n" 
			"li x28, 0xb93c06f46c448725\n"
			"fsc32w.ps f31, x28 (x22)\n"
			"fg32b.ps f15, x28 (x22)\n" 
			"fexp.ps f16, f16\n"
			"fcvt.pw.ps f6, f1, rtz\n"
			"LBL_SEQID_0_CSR_ID_20_HID_0:\n"
			"li x7, 0x80000081050e0689\n"
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw unlock_va, x7\n" 
			"csrwi tensor_wait, 6\n" 
			"fmul.pi f4, f4, f4\n"
			"fsgnjn.ps f4, f12, f30\n"
			"LBL_SEQID_0_FP_GATHER_ID_21_HID_0:\n"
			"li x22, 0x820d5c04ed\n" 
			"la x19, mem_ptr_offset\n"
			"flw.ps f29, 112(x19)\n"
			"fscw.ps f31, f29 (x22)\n"
			"fgw.ps f6, f29 (x22)\n" 
			"srlw x28, x10, x28\n"
			"fsetm.pi m0, f24\n"
			"LBL_SEQID_0_FP_LOAD_ID_22_HID_0:\n"
			"li x19, 0x810b43b933\n" 
			"fsw.ps f14, 744(x19)\n"
			"flw.ps f11, 744(x19)\n" 
			"fmin.ps f11, f31, f27\n"
			"slliw x16, x23, 3\n"
			"LBL_SEQID_0_INT_STORE_ID_23_HID_0:\n"
			"li x14, 0x820d5c04ed\n" 
			"sb x7, -1797(x14)\n" 
			"feq.ps f4, f4, f24\n"
			"fsrai.pi f16, f4, 0xa\n"
			"bge x26, x15, 1f\n"
			"1:\n"
			"LBL_SEQID_0_INT_AMO_ID_24_HID_0:\n"
			"li x25, 0x814d7e0008\n" 
			"amoswapl.d x22, x22, (x25)\n" 
			"divu x22, x5, x18\n"
			"bne x25, x15, 1f\n"
			"1:\n"
			"srliw x22, x25, 3\n"
			"LBL_SEQID_0_INT_L_STORE_ID_25_HID_0:\n"
			"li x18, 0x814895f82c\n" 
			"sbl x16, (x18)\n" 
			"sra x25, x25, x25\n"
			"fsatu8.pi f4, f31\n"
			"LBL_SEQID_0_FP_AMO_ID_26_HID_0:\n"
			"li x16, 0x82495406d8\n" 
			"la x18, mem_ptr_offset\n"
			"flw.ps f11, 992(x18)\n"
			"famomaxl.ps f22, f11 (x16)\n" 
			"xor x10, x10, x28\n"
			"fsin.ps f14, f31\n"
			"LBL_SEQID_0_INT_L_STORE_ID_27_HID_0:\n"
			"li x14, 0x824afc3024\n" 
			"shl x28, (x14)\n" 
			"flog.ps f27, f15\n"
			"bge x29, x30, 1f\n"
			"1:\n"
			"addiw x18, x5, 0x3\n"
			"LBL_SEQID_0_FP_AMO_ID_28_HID_0:\n"
			"li x22, 0x812824ee8c\n" 
			"la x10, mem_ptr_offset\n"
			"flw.ps f11, 928(x10)\n"
			"famominug.pi f27, f11 (x22)\n" 
			"fmax.pi f9, f4, f1\n"
			"mulhu x23, x25, x23\n"
			"LBL_SEQID_0_FP_G_GATHER_ID_29_HID_0:\n"
			"li x7, 0x813ee69310\n" 
			"la x10, mem_ptr_offset\n"
			"flw.ps f27, 168(x10)\n"
			"fscwg.ps f8, f27 (x7)\n"
			"fghg.ps f29, f27 (x7)\n" 
			"fsgnjx.s f31, f30, f31\n"
			"beq x21, x10, 1f\n"
			"1:\n"
			"fpackreph.pi f27, f31\n"
			"LBL_SEQID_0_FP_AMO_ID_30_HID_0:\n"
			"li x16, 0x814aa40134\n" 
			"la x5, mem_ptr_offset\n"
			"flw.ps f15, 608(x5)\n"
			"famoxorl.pi f31, f15 (x16)\n" 
			"fnmsub.s f31, f30, f4, f6, rtz\n"
			"fmax.s f27, f4, f30\n"
			"LBL_SEQID_0_INT_AMO_ID_31_HID_0:\n"
			"li x23, 0x814aa40134\n" 
			"amoaddl.w x10, x7, (x23)\n" 
			"fcvt.pw.ps f11, f31, rmm\n"
			"bne x11, x13, 1f\n"
			"1:\n"
			"fcvt.pwu.ps f9, f27, rdn\n"
			"mov.m.x m0, x0, 0xFF\n"
			"li x5, 0x8\n"
			"la x31, rand_ieee754_32\n"
			"add x31, x31, x5\n"
			"flw.ps f1, 0(x31)\n"
			"flw.ps f12, 2(x31)\n"
			"flw.ps f14, 3(x31)\n"
			"flw.ps f16, 6(x31)\n"
			"flw.ps f22, 2(x31)\n"
			"flw.ps f24, 14(x31)\n"
			"flw.ps f27, 22(x31)\n"
			"flw.ps f28, 26(x31)\n"
			"flw.ps f30, 11(x31)\n"
			"flw.ps f31, 36(x31)\n"
			"flw.ps f4, 17(x31)\n"
			"flw.ps f6, 33(x31)\n"
			"li x21, 0x1\n"
			"LBL_SEQID_0_LOOP_SEQ_HID_0:\n"
			"fle.ps f6, f30, f16\n"
			"fsll.pi f6, f12, f27\n"
			"fmin.ps f22, f27, f12\n"
			"fsgnjx.ps f1, f1, f14\n"
			"fcmovm.ps f1, f28, f1\n"
			"fcmovm.ps f1, f22, f12\n"
			"fadd.ps f1, f22, f12, rtz\n"
			"fcvt.ps.pw f16, f1, rtz\n"
			"fsub.pi f22, f24, f4\n"
			"fcvt.ps.f16 f6, f24\n"
			"fsub.ps f1, f1, f27, rmm\n"
			"fsgnjx.ps f16, f4, f22\n"
			"feq.ps f4, f31, f22\n"
			"fsin.ps f27, f1\n"
			"beq x10, x20, 1f\n"
			"1:\n"
			"fcmovm.ps f31, f6, f31\n"
			"for.pi f22, f31, f22\n"
			"fmadd.s f31, f28, f22, f31, rtz\n"
			"fpackrepb.pi f22, f4\n"
			"fmul.s f1, f14, f1, rdn\n"
			"fle.ps f11, f22, f30\n"
			"fmax.pi f22, f22, f12\n"
			"fmulh.pi f27, f30, f24\n"
			"fcvt.f16.ps f31, f31\n"
			"bgeu x17, x20, 1f\n"
			"1:\n"
			"feq.pi f16, f28, f12\n"
			"fmul.ps f11, f28, f4, rup\n"
			"fsrai.pi f6, f6, 0xc\n"
			"fsgnj.ps f28, f1, f31\n"
			"fmax.ps f28, f1, f27\n"
			"fadd.s f27, f27, f30, rmm\n"
			"fminu.pi f31, f6, f31\n"
			"fcmov.ps f30, f30, f30, f12\n"
			"fcvt.f16.ps f6, f4\n"
			"fsgnjx.s f24, f24, f16\n"
			"fsrl.pi f22, f6, f27\n"
			"fmin.ps f16, f27, f16\n"
			"flt.pi f4, f27, f31\n"
			"fsgnj.s f14, f27, f24\n"
			"fsub.s f30, f14, f30, rdn\n"
			"frsq.ps f27, f16\n"
			"fsgnjx.ps f22, f24, f27\n"
			"fmul.ps f24, f24, f24, rdn\n"
			"fmadd.s f30, f4, f30, f6, rmm\n"
			"fcmov.ps f1, f1, f30, f30\n"
			"feq.pi f28, f28, f31\n"
			"beq x7, x23, 1f\n"
			"1:\n"
			"fcmovm.ps f30, f24, f30\n"
			"bgeu x19, x11, 1f\n"
			"1:\n"
			"flog.ps f22, f14\n"
			"flt.pi f31, f12, f30\n"
			"faddi.pi f27, f24, 0x134\n"
			"bge x0, x26, 1f\n"
			"1:\n"
			"flt.ps f28, f22, f16\n"
			"fmulhu.pi f16, f1, f16\n"
			"fmin.ps f6, f16, f6\n"
			"fsgnjn.ps f4, f4, f14\n"
			"fclass.ps f6, f6\n"
			"fadd.s f28, f4, f28, rtz\n"
			"fadd.s f16, f24, f1, rup\n"
			"fswizz.ps f27, f1, 0x5\n"
			"fnmadd.s f27, f27, f12, f30, rne\n"
			"fadd.pi f28, f28, f16\n"
			"fpackrepb.pi f1, f30\n"
			"blt x10, x0, 1f\n"
			"1:\n"
			"fsll.pi f28, f12, f28\n"
			"fsin.ps f6, f27\n"
			"fsrli.pi f12, f27, 0x2\n"
			"fsrai.pi f4, f28, 0xf\n"
			"fadd.s f22, f12, f22, rne\n"
			"fmulh.pi f1, f1, f27\n"
			"fadd.s f1, f12, f30, rdn\n"
			"fle.ps f28, f30, f4\n"
			"bltu x11, x15, 1f\n"
			"1:\n"
			"fmadd.s f6, f28, f1, f28, rmm\n"
			"flt.ps f28, f6, f14\n"
			"blt x20, x25, 1f\n"
			"1:\n"
			"fsrai.pi f28, f30, 0xc\n"
			"fminu.pi f14, f22, f16\n"
			"fmaxu.pi f28, f30, f28\n"
			"fand.pi f11, f1, f4\n"
			"fnmsub.s f6, f24, f24, f14, rne\n"
			"fadd.ps f22, f22, f31, rmm\n"
			"fswizz.ps f4, f24, 0x4\n"
			"fmul.s f22, f16, f22, rmm\n"
			"fand.pi f1, f24, f28\n"
			"fmulhu.pi f4, f4, f12\n"
			"fsub.s f27, f27, f4, rdn\n"
			"fsll.pi f4, f16, f4\n"
			"fsrai.pi f6, f4, 0x0\n"
			"flt.pi f6, f27, f16\n"
			"bgeu x3, x17, 1f\n"
			"1:\n"
			"fsatu8.pi f31, f4\n"
			"fclass.ps f4, f24\n"
			"fmadd.s f31, f31, f24, f24, rtz\n"
			"faddi.pi f4, f28, 0x78\n"
			"fsub.pi f1, f16, f22\n"
			"fmul.s f14, f4, f14, rtz\n"
			"fsub.pi f4, f31, f30\n"
			"fsub.s f6, f31, f22, rtz\n"
			"fnmadd.s f27, f4, f14, f12, rtz\n"
			"fpackrepb.pi f30, f14\n"
			"fnmsub.s f11, f28, f14, f4, rdn\n"
			"fcmovm.ps f27, f6, f24\n"
			"fsgnjn.s f1, f14, f24\n"
			"fnmadd.s f4, f1, f22, f27, rmm\n"
			"fmulhu.pi f16, f12, f4\n"
			"fmax.ps f24, f16, f24\n"
			"fsgnjx.ps f1, f22, f12\n"
			"fmax.pi f24, f31, f24\n"
			"fxor.pi f11, f31, f4\n"
			"frcp.ps f31, f24\n"
			"fmin.ps f11, f22, f12\n"
			"blt x6, x16, 1f\n"
			"1:\n"
			"flt.pi f28, f22, f6\n"
			"fsgnjx.ps f31, f14, f31\n"
			"blt x7, x12, 1f\n"
			"1:\n"
			"fsrai.pi f11, f14, 0x4\n"
			"fpackreph.pi f6, f30\n"
			"fswizz.ps f1, f30, 0x1\n"
			"bge x22, x30, 1f\n"
			"1:\n"
			"fmadd.ps f6, f27, f14, f24, rmm\n"
			"blt x26, x28, 1f\n"
			"1:\n"
			"fmsub.s f27, f6, f28, f12, rup\n"
			"fle.pi f6, f1, f31\n"
			"fsatu8.pi f11, f31\n"
			"blt x9, x16, 1f\n"
			"1:\n"
			"fcvt.ps.f16 f16, f16\n"
			"fcvt.ps.f16 f30, f14\n"
			"fexp.ps f22, f1\n"
			"fcvt.ps.pwu f31, f27, rtz\n"
			"feq.pi f27, f30, f22\n"
			"fmsub.ps f27, f30, f6, f30, rtz\n"
			"bltu x9, x29, 1f\n"
			"1:\n"
			"fcvt.pw.ps f12, f30, rdn\n"
			"flt.pi f16, f16, f22\n"
			"fnmsub.ps f28, f24, f30, f30, rup\n"
			"fmul.ps f22, f6, f28, rne\n"
			"fsatu8.pi f6, f16\n"
			"fadd.pi f24, f16, f27\n"
			"fsgnjn.s f16, f14, f12\n"
			"fadd.pi f6, f12, f1\n"
			"fcvt.pw.ps f28, f16, rmm\n"
			"bgeu x1, x21, 1f\n"
			"1:\n"
			"fmul.pi f27, f4, f12\n"
			"fltu.pi f31, f14, f1\n"
			"bne x2, x5, 1f\n"
			"1:\n"
			"fpackreph.pi f27, f14\n"
			"fslli.pi f28, f22, 0x0\n"
			"fmaxu.pi f11, f27, f6\n"
			"fsub.pi f16, f30, f28\n"
			"beq x0, x2, 1f\n"
			"1:\n"
			"fsrli.pi f24, f30, 0x4\n"
			"fmulh.pi f27, f31, f4\n"
			"fnmsub.s f27, f1, f30, f27, rtz\n"
			"fmul.ps f11, f16, f24, rmm\n"
			"flt.ps f1, f6, f6\n"
			"fsub.pi f4, f14, f28\n"
			"feq.ps f27, f4, f24\n"
			"ffrc.ps f4, f12\n"
			"fxor.pi f24, f24, f12\n"
			"fcvt.pw.ps f30, f27, rdn\n"
			"feq.ps f22, f16, f28\n"
			"frsq.ps f6, f12\n"
			"fcmovm.ps f6, f30, f6\n"
			"bltu x2, x16, 1f\n"
			"1:\n"
			"fsgnjn.ps f14, f27, f14\n"
			"fpackrepb.pi f27, f27\n"
			"fsgnj.s f16, f24, f22\n"
			"fsll.pi f30, f12, f1\n"
			"fmulh.pi f4, f16, f6\n"
			"bltu x25, x1, 1f\n"
			"1:\n"
			"fsgnjn.s f27, f22, f6\n"
			"for.pi f31, f6, f1\n"
			"fnmadd.ps f1, f27, f14, f1, rdn\n"
			"fandi.pi f11, f12, 0xee\n"
			"feq.pi f16, f27, f24\n"
			"fle.ps f22, f14, f22\n"
			"fswizz.ps f27, f27, 0x16\n"
			"blt x11, x29, 1f\n"
			"1:\n"
			"flog.ps f12, f6\n"
			"flog.ps f6, f4\n"
			"ffrc.ps f12, f12\n"
			"fsat8.pi f1, f6\n"
			"fsub.s f31, f12, f31, rdn\n"
			"bgeu x17, x7, 1f\n"
			"1:\n"
			"flog.ps f16, f22\n"
			"fmadd.s f4, f27, f14, f30, rdn\n"
			"fnmsub.ps f16, f1, f16, f16, rtz\n"
			"fmadd.s f31, f6, f31, f14, rtz\n"
			"fsrl.pi f14, f14, f14\n"
			"fcmovm.ps f11, f30, f30\n"
			"bne x29, x4, 1f\n"
			"1:\n"
			"fnmadd.s f27, f31, f27, f31, rne\n"
			"fsra.pi f22, f6, f27\n"
			"fcvt.f16.ps f11, f28\n"
			"fcvt.pw.ps f6, f28, rup\n"
			"blt x31, x8, 1f\n"
			"1:\n"
			"fsub.s f22, f6, f22, rtz\n"
			"fmul.s f27, f16, f22, rdn\n"
			"bge x4, x0, 1f\n"
			"1:\n"
			"fmsub.s f6, f12, f22, f24, rtz\n"
			"fminu.pi f1, f22, f1\n"
			"fsin.ps f28, f28\n"
			"fle.pi f28, f24, f1\n"
			"fmin.ps f31, f31, f14\n"
			"flt.ps f11, f6, f24\n"
			"fpackrepb.pi f11, f16\n"
			"addi x21, x21, -1\n"
			"bne x21, x0, LBL_SEQID_0_LOOP_SEQ_HID_0\n"
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_0\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_1_FP_SCATTER_ID_32_HID_0:\n"
			"li x10, 0x810da13814\n" 
			"li x28, 0x6c3ba677fa0a2c68\n"
			"fsc32b.ps f11, x28 (x10)\n" 
			"fpackrepb.pi f12, f31\n"
			"fmul.ps f9, f9, f6, rmm\n"
			"LBL_SEQID_1_INT_STORE_ID_33_HID_0:\n"
			"li x14, 0x81027db34d\n" 
			"sd x18, 616(x14)\n" 
			"fnot.pi f27, f27\n"
			"bltu x8, x27, 1f\n"
			"1:\n"
			"fsatu8.pi f24, f24\n"
			"la x5, rand_int_32\n"
			"flw.ps f16, 256(x5)\n"
			"flw.ps f6, 96(x5)\n"
			"fdivu.pi f16, f16, f6\n" 
			"mulh x19, x23, x25\n"
			"for.pi f6, f1, f14\n"
			"bgeu x10, x14, 1f\n"
			"1:\n"
			"LBL_SEQID_1_INT_AMO_ID_34_HID_0:\n"
			"li x10, 0x815c7e0030\n" 
			"amoandl.w x16, x23, (x10)\n" 
			"fle.pi f30, f30, f15\n"
			"fsgnjx.ps f24, f6, f15\n"
			"bltu x4, x24, 1f\n"
			"1:\n"
			"LBL_SEQID_1_INT_AMO_ID_35_HID_0:\n"
			"li x28, 0x822d5c04ec\n" 
			"amoxorg.w x14, x16, (x28)\n" 
			"sra x23, x7, x22\n"
			"fswizz.ps f27, f30, 0x0\n"
			"LBL_SEQID_1_FP_G_LOAD_ID_36_HID_0:\n"
			"li x19, 0x813c0824c0\n" 
			"mov.m.x m0, zero, 0xff\n"
			"fswg.ps f11, (x19)\n"
			"flwg.ps f14, (x19)\n" 
			"fnot.pi f11, f1\n"
			"fadd.pi f12, f12, f12\n"
			"LBL_SEQID_1_FP_LOAD_ID_37_HID_0:\n"
			"li x19, 0x8200120025\n" 
			"fsq2 f16, 1459(x19)\n"
			"flw.ps f6, 1459(x19)\n" 
			"frcp.ps f11, f12\n"
			"fmul.s f16, f30, f6, rtz\n"
			"la x14, rand_int_32\n"
			"flw.ps f15, 896(x14)\n"
			"flw.ps f16, 0(x14)\n"
			"fdivu.pi f9, f15, f16\n" 
			"mova.m.x x16\n"
			"slti x16, x19, 0x1d\n"
			"LBL_SEQID_1_INT_AMO_ID_38_HID_0:\n"
			"li x25, 0x8250120024\n" 
			"amoorl.w x5, x16, (x25)\n" 
			"andi x5, x18, 0x1f\n"
			"bge x1, x14, 1f\n"
			"1:\n"
			"fadd.s f9, f27, f24, rup\n"
			"LBL_SEQID_1_INT_STORE_ID_39_HID_0:\n"
			"li x14, 0x810824ee8e\n" 
			"sw x23, -520(x14)\n" 
			"mulh x18, x25, x18\n"
			"fswizz.ps f30, f6, 0x5\n"
			"fence.i\n" 
			"feqm.ps m1, f11, f4\n"
			"fmin.ps f16, f4, f16\n"
			"LBL_SEQID_1_INT_LOAD_ID_40_HID_0:\n"
			"li x7, 0x820d5c04ed\n" 
			"sd x18, 104(x7)\n"
			"ld x18, 104(x7)\n" 
			"fltm.pi m0, f22, f30\n"
			"mov.m.x m0, x0, 0xff\n"
			"xori x10, x7, 0x2\n"
			"LBL_SEQID_1_INT_AMO_ID_41_HID_0:\n"
			"li x14, 0x824171d034\n" 
			"amoorl.w x23, x19, (x14)\n" 
			"fandi.pi f14, f6, 0x16e\n"
			"fsub.s f29, f29, f1, rdn\n"
			"bgeu x21, x25, 1f\n"
			"1:\n"
			"LBL_SEQID_1_FP_STORE_ID_42_HID_0:\n"
			"li x22, 0x8200120025\n" 
			"fsw f4, 1104(x22)\n" 
			"fsrli.pi f12, f12, 0x9\n"
			"fsgnjx.ps f24, f31, f24\n"
			"bltu x5, x17, 1f\n"
			"1:\n"
			"LBL_SEQID_1_CSR_ID_43_HID_0:\n"
			"li x19, 0x800000810ae6f70d\n"
			"li x31, (0x3 << 6) | 0x0\n"
			"csrw unlock_va, x19\n" 
			"csrwi tensor_wait, 6\n" 
			"fle.pi f27, f27, f11\n"
			"fmax.pi f27, f6, f6\n"
			"LBL_SEQID_1_FP_AMO_ID_44_HID_0:\n"
			"li x14, 0x824b7a46cc\n" 
			"la x7, mem_ptr_offset\n"
			"flw.ps f1, 352(x7)\n"
			"famominul.pi f16, f1 (x14)\n" 
			"fnmsub.ps f12, f27, f14, f12, rne\n"
			"fltu.pi f14, f24, f16\n"
			"LBL_SEQID_1_INT_AMO_ID_45_HID_0:\n"
			"li x28, 0x814895f82c\n" 
			"amoandl.w x18, x7, (x28)\n" 
			"fsll.pi f11, f12, f16\n"
			"bge x29, x11, 1f\n"
			"1:\n"
			"fandi.pi f9, f16, 0x1de\n"
			"LBL_SEQID_1_FP_AMO_ID_46_HID_0:\n"
			"li x7, 0x81596c5808\n" 
			"la x28, mem_ptr_offset\n"
			"flw.ps f16, 928(x28)\n"
			"famoxorl.pi f1, f16 (x7)\n" 
			"fsgnj.s f22, f1, f6\n"
			"fsrai.pi f1, f1, 0xf\n"
			"LBL_SEQID_1_FP_GATHER_ID_47_HID_0:\n"
			"li x7, 0x82074e076a\n" 
			"la x14, mem_ptr_offset\n"
			"flw.ps f28, 164(x14)\n"
			"fscw.ps f27, f28 (x7)\n"
			"fgw.ps f6, f28 (x7)\n" 
			"fsra.pi f24, f31, f4\n"
			"fsrli.pi f28, f11, 0xf\n"
			"LBL_SEQID_1_INT_STORE_ID_48_HID_0:\n"
			"li x10, 0x810da13814\n" 
			"sd x22, 387(x10)\n" 
			"sraiw x19, x25, 1\n"
			"fsrl.pi f30, f4, f15\n"
			"LBL_SEQID_1_INT_AMO_ID_49_HID_0:\n"
			"li x23, 0x812824ee88\n" 
			"amoorg.d x25, x18, (x23)\n" 
			"fclass.ps f28, f11\n"
			"for.pi f28, f16, f28\n"
			"LBL_SEQID_1_INT_AMO_ID_50_HID_0:\n"
			"li x16, 0x812b43b930\n" 
			"amomaxg.w x5, x19, (x16)\n" 
			"fswizz.ps f14, f12, 0x5\n"
			"fexp.ps f9, f31\n"
			"LBL_SEQID_1_INT_AMO_ID_51_HID_0:\n"
			"li x10, 0x8255060004\n" 
			"amoaddl.w x16, x25, (x10)\n" 
			"fexp.ps f30, f16\n"
			"fsgnj.s f27, f6, f31\n"
			"LBL_SEQID_1_FP_AMO_ID_52_HID_0:\n"
			"li x10, 0x8152d32168\n" 
			"la x14, mem_ptr_offset\n"
			"flw.ps f15, 32(x14)\n"
			"famoxorl.pi f1, f15 (x10)\n" 
			"mova.x.m x14\n"
			"fbci.pi f31, 0x688ea\n"
			"LBL_SEQID_1_INT_AMO_ID_53_HID_0:\n"
			"li x10, 0x814895f82c\n" 
			"amoorl.w x7, x22, (x10)\n" 
			"fxor.pi f27, f12, f24\n"
			"fadd.pi f24, f12, f16\n"
			"LBL_SEQID_1_INT_AMO_ID_54_HID_0:\n"
			"li x5, 0x824e8b4760\n" 
			"amoswapl.d x23, x7, (x5)\n" 
			"fsub.s f14, f28, f4, rne\n"
			"fnot.pi f29, f14\n"
			"bgeu x1, x7, 1f\n"
			"1:\n"
			"LBL_SEQID_1_INT_AMO_ID_55_HID_0:\n"
			"li x19, 0x8128160010\n" 
			"amoxorg.w x25, x25, (x19)\n" 
			"addi x28, x28, 0x1a\n"
			"blt x9, x11, 1f\n"
			"1:\n"
			"fsrai.pi f6, f29, 0xd\n"
			"LBL_SEQID_1_FP_LOAD_ID_56_HID_0:\n"
			"li x22, 0x810aa40134\n" 
			"fsw.ps f28, -1(x22)\n"
			"flq2 f12, -1(x22)\n" 
			"mulhu x22, x7, x28\n"
			"fsrai.pi f14, f14, 0xf\n"
			"LBL_SEQID_1_FP_STORE_ID_57_HID_0:\n"
			"li x25, 0x82074e076a\n" 
			"fsw f4, 768(x25)\n" 
			"masknot m1, m0\n"
			"fsll.pi f12, f30, f30\n"
			"LBL_SEQID_1_FP_SCATTER_ID_58_HID_0:\n"
			"li x10, 0x810895f82f\n" 
			"la x7, mem_ptr_offset\n"
			"flw.ps f6, 0(x7)\n"
			"fsch.ps f1, f6 (x10)\n" 
			"fnmsub.ps f24, f24, f4, f31, rtz\n"
			"fsgnjx.ps f22, f11, f22\n"
			"LBL_SEQID_1_FP_G_SCATTER_ID_59_HID_0:\n"
			"li x28, 0x8139e20024\n" 
			"la x10, mem_ptr_offset\n"
			"flq2 f9, 28(x10)\n"
			"fschg.ps f16, f9 (x28)\n" 
			"fmul.s f9, f28, f12, rne\n"
			"fnmsub.s f22, f27, f24, f28, rmm\n"
			"LBL_SEQID_1_INT_AMO_ID_60_HID_0:\n"
			"li x5, 0x81227db348\n" 
			"amomaxug.d x23, x28, (x5)\n" 
			"fxor.pi f31, f16, f28\n"
			"fminu.pi f24, f29, f15\n"
			"mov.m.x m0, x0, 0xFF\n"
			"li x5, 0x10\n"
			"la x31, rand_ieee754_32\n"
			"add x31, x31, x5\n"
			"flw.ps f1, 0(x31)\n"
			"flw.ps f11, 3(x31)\n"
			"flw.ps f12, 8(x31)\n"
			"flw.ps f14, 8(x31)\n"
			"flw.ps f15, 12(x31)\n"
			"flw.ps f16, 5(x31)\n"
			"flw.ps f27, 4(x31)\n"
			"flw.ps f28, 21(x31)\n"
			"flw.ps f31, 20(x31)\n"
			"flw.ps f6, 33(x31)\n"
			"flw.ps f9, 29(x31)\n"
			"la x31, rand_int_32\n"
			"add x31, x31, x5\n"
			"lw x10, 0(x31)\n"
			"lw x14, 4(x31)\n"
			"lw x16, 4(x31)\n"
			"lw x18, 0(x31)\n"
			"lw x19, 10(x31)\n"
			"lw x22, 11(x31)\n"
			"lw x23, 12(x31)\n"
			"lw x25, 3(x31)\n"
			"lw x28, 25(x31)\n"
			"lw x5, 35(x31)\n"
			"lw x7, 1(x31)\n"
			"li x21, 0x1\n"
			"LBL_SEQID_1_LOOP_SEQ_HID_0:\n"
			"maskxor m2, m1, m6\n"
			"maskpopc x16, m0\n"
			"sll x22, x23, x22\n"
			"maskpopcz x22, m0\n"
			"fadd.s f11, f16, f11, rdn\n"
			"fmul.pi f29, f14, f28\n"
			"fminu.pi f9, f15, f14\n"
			"flt.s x28, f27, f15\n"
			"and x18, x19, x22\n"
			"fcvt.wu.s x23, f31, rdn\n"
			"mova.x.m x25\n"
			"ori x14, x22, 0xe\n"
			"bgeu x0, x26, 1f\n"
			"1:\n"
			"fexp.ps f27, f11\n"
			"fmadd.ps f15, f16, f28, f16, rne\n"
			"packb x18, x7, x14\n"
			"fround.ps f31, f16, rup\n"
			"blt x17, x28, 1f\n"
			"1:\n"
			"fcvt.ps.f16 f29, f27\n"
			"fmin.ps f4, f1, f16\n"
			"bltu x26, x13, 1f\n"
			"1:\n"
			"mova.x.m x14\n"
			"frcp.ps f9, f28\n"
			"bge x25, x16, 1f\n"
			"1:\n"
			"mova.m.x x7\n"
			"auipc x10, 0x1a\n"
			"fmvs.x.ps x5, f12, 4\n"
			"maskxor m6, m0, m0\n"
			"faddi.pi f9, f27, 0x1f6\n"
			"fsll.pi f31, f1, f31\n"
			"sraw x10, x23, x19\n"
			"maskor m0, m0, m6\n"
			"mov.m.x m0, x0, 0xff\n"
			"feq.s x10, f27, f31\n"
			"bltu x21, x8, 1f\n"
			"1:\n"
			"maskpopcz x10, m1\n"
			"fcvt.ps.pwu f15, f12, rdn\n"
			"fmaxu.pi f9, f12, f28\n"
			"masknot m0, m2\n"
			"fcvt.ps.f16 f9, f1\n"
			"fcvt.s.w f11, x23, rdn\n"
			"maskxor m0, m2, m0\n"
			"mov.m.x m0, x0, 0xff\n"
			"beq x20, x29, 1f\n"
			"1:\n"
			"feq.s x25, f12, f14\n"
			"bgeu x19, x20, 1f\n"
			"1:\n"
			"sltu x5, x23, x10\n"
			"fmv.s.x f28, x7\n"
			"maskpopcz x10, m2\n"
			"fsgnjx.s f30, f31, f31\n"
			"bne x13, x10, 1f\n"
			"1:\n"
			"fexp.ps f28, f28\n"
			"fmin.ps f12, f6, f27\n"
			"fsgnj.ps f9, f16, f11\n"
			"srli x10, x19, 1\n"
			"blt x7, x21, 1f\n"
			"1:\n"
			"fmin.pi f27, f27, f15\n"
			"maskor m0, m2, m1\n"
			"mov.m.x m0, x0, 0xff\n"
			"fclass.ps f14, f14\n"
			"fmv.s.x f30, x28\n"
			"addi x28, x28, 0x1b\n"
			"fand.pi f16, f15, f12\n"
			"bltu x29, x20, 1f\n"
			"1:\n"
			"fsgnjn.ps f1, f1, f31\n"
			"masknot m6, m1\n"
			"div x28, x16, x19\n"
			"fsrli.pi f16, f15, 0x0\n"
			"bltu x24, x15, 1f\n"
			"1:\n"
			"fcvt.w.s x10, f14, rmm\n"
			"bne x24, x14, 1f\n"
			"1:\n"
			"sll x18, x28, x19\n"
			"fmin.ps f9, f6, f27\n"
			"fmvz.x.ps x14, f15, 4\n"
			"fsetm.pi m1, f28\n"
			"fmul.ps f14, f14, f15, rdn\n"
			"mova.m.x x28\n"
			"bge x20, x10, 1f\n"
			"1:\n"
			"fcvt.ps.pwu f27, f9, rtz\n"
			"for.pi f28, f31, f11\n"
			"mulhu x23, x23, x22\n"
			"sltu x23, x23, x7\n"
			"mulw x28, x28, x10\n"
			"andi x18, x10, 0x3\n"
			"ebreak\n"
			"andi x14, x10, 0x1d\n"
			"or x25, x22, x5\n"
			"fsub.ps f31, f16, f6, rmm\n"
			"fltu.pi f12, f28, f12\n"
			"fsgnjx.ps f29, f15, f14\n"
			"flog.ps f16, f16\n"
			"fsrai.pi f28, f6, 0x7\n"
			"fbci.ps f22, 0x53986\n"
			"mulh x14, x14, x5\n"
			"fcvt.ps.pwu f14, f14, rtz\n"
			"fmul.pi f31, f28, f27\n"
			"fcvt.pw.ps f22, f31, rmm\n"
			"fcvt.f16.ps f27, f6\n"
			"fcvt.ps.f16 f11, f31\n"
			"bltu x16, x31, 1f\n"
			"1:\n"
			"or x18, x18, x19\n"
			"remw x18, x7, x7\n"
			"mov.m.x m6, x25, 0xc\n"
			"fle.ps f30, f9, f14\n"
			"fminu.pi f16, f15, f16\n"
			"fsatu8.pi f16, f14\n"
			"fsgnjx.ps f16, f16, f6\n"
			"fsgnjx.s f22, f31, f16\n"
			"bge x2, x28, 1f\n"
			"1:\n"
			"ebreak\n"
			"masknot m1, m1\n"
			"xor x25, x25, x18\n"
			"fmvz.x.ps x25, f31, 7\n"
			"fnmadd.s f27, f27, f27, f6, rmm\n"
			"div x5, x14, x16\n"
			"srlw x28, x28, x23\n"
			"fsub.s f28, f28, f12, rup\n"
			"fsetm.pi m1, f9\n"
			"fle.s x5, f31, f31\n"
			"bgeu x30, x7, 1f\n"
			"1:\n"
			"feq.ps f16, f9, f16\n"
			"frsq.ps f27, f12\n"
			"fnmadd.s f1, f1, f1, f14, rup\n"
			"mulw x5, x22, x7\n"
			"sra x14, x14, x16\n"
			"fcmovm.ps f22, f1, f27\n"
			"mova.x.m x5\n"
			"fltm.pi m2, f1, f14\n"
			"flt.pi f16, f16, f9\n"
			"fmin.pi f16, f12, f1\n"
			"mulw x19, x19, x5\n"
			"fsub.ps f12, f1, f12, rne\n"
			"sub x14, x19, x14\n"
			"masknot m1, m1\n"
			"fcvt.pwu.ps f30, f11, rdn\n"
			"srai x10, x23, 4\n"
			"auipc x23, 0x16\n"
			"fmvz.x.ps x19, f28, 5\n"
			"masknot m1, m0\n"
			"mov.m.x m0, x0, 0xff\n"
			"xori x22, x22, 0x1c\n"
			"fnmsub.ps f28, f9, f28, f31, rmm\n"
			"fcvt.s.wu f15, x23, rdn\n"
			"fminu.pi f27, f11, f1\n"
			"divuw x7, x16, x23\n"
			"fcvt.w.s x14, f9, rdn\n"
			"addi x21, x21, -1\n"
			"bne x21, x0, LBL_SEQID_1_LOOP_SEQ_HID_0\n"
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_0\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_2_INT_DIV_ID_61_HID_0:\n"
			"div x19, x25, x19\n" 
			"fmulh.pi f24, f27, f31\n"
			"fsgnjn.s f4, f15, f22\n"
			"fmadd.s f29, f28, f31, f29, rdn\n"
			"fle.ps f28, f28, f9\n"
			"bne x21, x30, 1f\n"
			"1:\n"
			"fmulh.pi f30, f16, f9\n"
			"beq x26, x23, 1f\n"
			"1:\n"
			"la x18, rand_ieee754_32\n"
			"flw.ps f1, 992(x18)\n"
			"flw.ps f31, 960(x18)\n"
			"fmul.s f29, f1, f31, rup\n" 
			"fmulh.pi f12, f14, f14\n"
			"fexp.ps f27, f6\n"
			"fmulh.pi f14, f9, f30\n"
			"blt x18, x21, 1f\n"
			"1:\n"
			"fsin.ps f9, f9\n"
			"fmadd.s f6, f6, f6, f6, rmm\n"
			"bne x16, x9, 1f\n"
			"1:\n"
			"mova.x.m x14\n" 
			"fmsub.s f14, f12, f27, f14, rmm\n"
			"fsgnj.ps f22, f24, f12\n"
			"fnot.pi f15, f22\n"
			"fmul.ps f24, f16, f24, rne\n"
			"fle.pi f1, f15, f16\n"
			"la x10, rand_ieee754_32\n"
			"flw.ps f28, 736(x10)\n"
			"flw.ps f14, 800(x10)\n"
			"fmax.ps f16, f28, f14\n" 
			"fltu.pi f29, f9, f9\n"
			"or x5, x18, x14\n"
			"fsgnjn.ps f9, f22, f27\n"
			"fcvt.s.w f9, x16, rne\n"
			"bgeu x10, x17, 1f\n"
			"1:\n"
			"fsll.pi f12, f9, f22\n"
			"la x28, rand_ieee754_32\n"
			"flw.ps f27, 864(x28)\n"
			"fsub.ps f4, f27, f27, rne\n" 
			"fsin.ps f29, f16\n"
			"fnmsub.s f27, f12, f6, f11, rdn\n"
			"fmul.ps f22, f27, f27, rmm\n"
			"bge x30, x17, 1f\n"
			"1:\n"
			"masknot m1, m0\n"
			"mov.m.x m0, x0, 0xff\n"
			"div x25, x28, x28\n"
			"la x23, rand_ieee754_32\n"
			"flw.ps f24, 288(x23)\n"
			"flw.ps f1, 192(x23)\n"
			"fsgnj.ps f9, f24, f1\n" 
			"div x7, x16, x16\n"
			"bgeu x30, x13, 1f\n"
			"1:\n"
			"feq.s x25, f11, f15\n"
			"mulhu x23, x23, x16\n"
			"frcp.ps f30, f9\n"
			"fmul.ps f6, f6, f14, rne\n"
			"la x7, rand_ieee754_32\n"
			"flw.ps f30, 448(x7)\n"
			"flw.ps f22, 160(x7)\n"
			"fmul.ps f28, f30, f22, rup\n" 
			"fsll.pi f14, f27, f22\n"
			"fsub.s f24, f6, f1, rne\n"
			"fandi.pi f4, f24, 0x1e\n"
			"fmul.ps f22, f12, f6, rne\n"
			"fsgnjn.s f22, f16, f28\n"
			"la x28, rand_int_32\n"
			"flw.ps f12, 832(x28)\n"
			"flw.ps f24, 384(x28)\n"
			"fmaxu.pi f14, f12, f24\n" 
			"fcmovm.ps f14, f14, f11\n"
			"srai x14, x14, 3\n"
			"fmulh.pi f16, f11, f14\n"
			"bgeu x29, x0, 1f\n"
			"1:\n"
			"fnmsub.s f1, f1, f11, f1, rdn\n"
			"fmax.pi f14, f1, f29\n"
			"la x14, rand_int_32\n"
			"flw.ps f6, 928(x14)\n"
			"fsat8.pi f16, f6\n" 
			"fltm.pi m1, f30, f30\n"
			"divu x10, x28, x19\n"
			"fmadd.s f11, f16, f30, f30, rup\n"
			"bgeu x12, x22, 1f\n"
			"1:\n"
			"fcvt.pwu.ps f27, f9, rmm\n"
			"fsat8.pi f27, f4\n"
			"la x5, rand_ieee754_32\n"
			"flw.ps f6, 928(x5)\n"
			"fcvt.pw.ps f11, f6, rtz\n" 
			"srli x5, x18, 4\n"
			"fsrl.pi f11, f1, f27\n"
			"blt x24, x29, 1f\n"
			"1:\n"
			"fmv.s.x f29, x18\n"
			"mulw x18, x19, x18\n"
			"fexp.ps f11, f9\n"
			"LBL_SEQID_2_M0_WRITE_ID_62_HID_0:\n"
			"masknot m1, m0\n" 
			"li x5, 0x803401F0\n"
			"ld x5, 0(x5)\n"
			"fltm.pi m1, f11, f30\n"
			"fadd.pi f6, f1, f16\n"
			"fsin.ps f29, f4\n"
			"fmul.ps f9, f31, f14, rne\n"
			"packb x5, x23, x23\n"
			"la x16, rand_ieee754_32\n"
			"flw.ps f4, 32(x16)\n"
			"fcvt.w.s x16, f4, rdn\n" 
			"fle.pi f16, f1, f30\n"
			"fsgnj.s f27, f12, f27\n"
			"fsub.ps f22, f29, f11, rmm\n"
			"fsgnjn.s f12, f28, f12\n"
			"fsub.ps f9, f22, f28, rtz\n"
			"mova.x.m x10\n" 
			"ori x5, x18, 0x7\n"
			"fcvt.ps.pwu f30, f22, rtz\n"
			"fminu.pi f30, f16, f6\n"
			"feq.s x23, f30, f1\n"
			"flog.ps f27, f30\n"
			"bgeu x21, x28, 1f\n"
			"1:\n"
			"la x28, rand_ieee754_32\n"
			"flw.ps f30, 928(x28)\n"
			"fcvt.wu.s x7, f30, rne\n" 
			"fminu.pi f1, f15, f16\n"
			"slliw x10, x16, 5\n"
			"fmulh.pi f12, f22, f16\n"
			"fminu.pi f31, f11, f27\n"
			"fexp.ps f28, f24\n"
			"la x18, rand_ieee754_32\n"
			"flw.ps f22, 32(x18)\n"
			"flw.ps f16, 352(x18)\n"
			"fnmadd.s f15, f22, f16, f16, rdn\n" 
			"slliw x14, x14, 5\n"
			"fsgnjn.ps f4, f15, f31\n"
			"maskpopc x19, m1\n"
			"xori x23, x14, 0x14\n"
			"beq x25, x25, 1f\n"
			"1:\n"
			"fmin.pi f15, f27, f22\n"
			"bne x21, x5, 1f\n"
			"1:\n"
			"LBL_SEQID_2_INT_DIV_ID_63_HID_0:\n"
			"divuw x28, x23, x18\n" 
			"fsgnjn.s f30, f30, f16\n"
			"fnmadd.s f31, f29, f14, f30, rne\n"
			"fcvt.ps.pwu f1, f12, rdn\n"
			"fsin.ps f4, f24\n"
			"frcp.ps f30, f28\n"
			"LBL_SEQID_2_M0_WRITE_ID_64_HID_0:\n"
			"fsetm.pi m2, f30\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"fsat8.pi f24, f9\n"
			"fsll.pi f6, f24, f6\n"
			"fmul.s f22, f12, f24, rtz\n"
			"fmulh.pi f29, f11, f4\n"
			"bgeu x5, x23, 1f\n"
			"1:\n"
			"fmadd.s f1, f9, f9, f28, rup\n"
			"la x25, rand_ieee754_32\n"
			"flw.ps f31, 384(x25)\n"
			"flw.ps f6, 224(x25)\n"
			"fsgnjn.s f22, f31, f6\n" 
			"fmul.ps f11, f11, f31, rup\n"
			"fmul.s f6, f27, f28, rmm\n"
			"fmax.ps f4, f12, f4\n"
			"blt x24, x1, 1f\n"
			"1:\n"
			"frcp.ps f28, f29\n"
			"fmaxu.pi f6, f15, f27\n"
			"li x28, 0x9b5d95cc7de6d960\n"
			"li x16, 0x1e590639346f2ab2\n"
			"or x5, x28, x16\n" 
			"packb x14, x14, x14\n"
			"packb x18, x5, x14\n"
			"fle.ps f12, f16, f14\n"
			"fsgnjn.s f4, f4, f4\n"
			"fsgnjn.s f4, f4, f4\n"
			"la x25, rand_ieee754_32\n"
			"flw.ps f15, 736(x25)\n"
			"flw.ps f30, 320(x25)\n"
			"flw.ps f9, 64(x25)\n"
			"fmsub.s f22, f15, f30, f9, rmm\n" 
			"packb x5, x25, x18\n"
			"xori x10, x18, 0xf\n"
			"bltu x0, x21, 1f\n"
			"1:\n"
			"fmul.ps f29, f4, f14, rtz\n"
			"bne x23, x2, 1f\n"
			"1:\n"
			"fsat8.pi f4, f16\n"
			"div x18, x16, x18\n"
			"la x25, rand_ieee754_32\n"
			"flw.ps f1, 544(x25)\n"
			"fsin.ps f14, f1\n" 
			"fsin.ps f14, f14\n"
			"bltu x3, x22, 1f\n"
			"1:\n"
			"fle.ps f31, f16, f14\n"
			"fcvt.ps.pwu f14, f4, rmm\n"
			"bge x31, x21, 1f\n"
			"1:\n"
			"fnmadd.ps f4, f24, f24, f9, rtz\n"
			"fmul.s f4, f31, f16, rtz\n"
			"blt x27, x21, 1f\n"
			"1:\n"
			"li x10, 0xcfca7526b43a0230\n"
			"sub x25, x10, x10\n" 
			"maskpopcz x25, m1\n"
			"fminu.pi f30, f24, f24\n"
			"or x7, x25, x23\n"
			"bgeu x28, x12, 1f\n"
			"1:\n"
			"fmadd.s f27, f6, f31, f27, rdn\n"
			"fltm.ps m1, f30, f14\n"
			"la x7, rand_ieee754_32\n"
			"flw.ps f12, 128(x7)\n"
			"flw.ps f11, 288(x7)\n"
			"flw.ps f14, 992(x7)\n"
			"fnmadd.ps f4, f12, f11, f14, rdn\n" 
			"divu x28, x14, x23\n"
			"fcmovm.ps f14, f24, f29\n"
			"fsub.ps f29, f6, f24, rmm\n"
			"frsq.ps f1, f1\n"
			"bge x8, x7, 1f\n"
			"1:\n"
			"fsub.ps f28, f28, f6, rtz\n"
			"LBL_SEQID_2_INT_DIV_ID_65_HID_0:\n"
			"div x14, x7, x28\n" 
			"fsub.ps f22, f30, f1, rmm\n"
			"fadd.pi f29, f4, f9\n"
			"fcmovm.ps f31, f4, f4\n"
			"fsub.s f24, f24, f24, rdn\n"
			"fle.pi f15, f4, f24\n"
			"li x14, 0x6465c1dba00bba1e\n"
			"fcvt.s.w f12, x14, rup\n" 
			"srai x14, x5, 4\n"
			"fsgnjn.ps f28, f28, f12\n"
			"mulhu x22, x7, x5\n"
			"fmax.pi f6, f15, f4\n"
			"divuw x16, x7, x22\n"
			"blt x14, x15, 1f\n"
			"1:\n"
			"la x28, rand_int_32\n"
			"flw.ps f22, 704(x28)\n"
			"fmaxu.pi f24, f22, f22\n" 
			"fmsub.s f12, f12, f11, f12, rne\n"
			"fsatu8.pi f30, f11\n"
			"flog.ps f14, f16\n"
			"fsatu8.pi f14, f12\n"
			"flog.ps f14, f12\n"
			"la x18, rand_ieee754_32\n"
			"flw.ps f1, 960(x18)\n"
			"fmvs.x.ps x19, f1, 2\n" 
			"frcp.ps f24, f15\n"
			"frsq.ps f4, f4\n"
			"ori x5, x5, 0xc\n"
			"fsgnj.s f14, f11, f11\n"
			"sub x7, x7, x28\n"
			"la x18, rand_ieee754_32\n"
			"flw.ps f27, 448(x18)\n"
			"flw.ps f28, 928(x18)\n"
			"flw.ps f12, 960(x18)\n"
			"fnmadd.ps f6, f27, f28, f12, rmm\n" 
			"fmaxu.pi f4, f16, f30\n"
			"sraw x7, x7, x22\n"
			"fltm.ps m0, f12, f6\n"
			"srli x18, x25, 0\n"
			"fcmovm.ps f11, f6, f12\n"
			"la x28, rand_ieee754_32\n"
			"flw.ps f14, 480(x28)\n"
			"flw.ps f6, 288(x28)\n"
			"fsgnjn.s f1, f14, f6\n" 
			"fsgnjn.s f15, f11, f15\n"
			"fcvt.wu.s x7, f4, rne\n"
			"fnmadd.ps f1, f15, f22, f12, rmm\n"
			"fcvt.wu.s x28, f24, rne\n"
			"mova.x.m x10\n"
			"la x5, rand_ieee754_32\n"
			"flw.ps f4, 896(x5)\n"
			"flw.ps f24, 832(x5)\n"
			"fsgnj.ps f4, f4, f24\n" 
			"fle.pi f22, f12, f1\n"
			"fnmadd.ps f28, f28, f24, f28, rne\n"
			"fsub.s f12, f6, f30, rdn\n"
			"fswizz.ps f16, f29, 0x13\n"
			"fmulh.pi f16, f12, f31\n"
			"mova.x.m x25\n" 
			"fsatu8.pi f27, f6\n"
			"mova.x.m x14\n"
			"bne x31, x3, 1f\n"
			"1:\n"
			"fsub.ps f27, f31, f30, rup\n"
			"fmv.s.x f14, x18\n"
			"fcvt.pwu.ps f4, f27, rdn\n"
			"li x19, 0xe7bbffb8fe2161fd\n"
			"li x7, 0x3b9d613f6567bf01\n"
			"sraw x14, x19, x7\n" 
			"fcvt.ps.pwu f4, f6, rup\n"
			"fmsub.s f27, f11, f11, f6, rup\n"
			"fsrl.pi f22, f22, f24\n"
			"fnot.pi f4, f30\n"
			"fmul.ps f12, f12, f1, rne\n"
			"mov.m.x m0, x0, 0xFF\n"
			"li x5, 0x30\n"
			"la x31, rand_ieee754_32\n"
			"add x31, x31, x5\n"
			"flw.ps f16, 0(x31)\n"
			"flw.ps f27, 1(x31)\n"
			"flw.ps f28, 6(x31)\n"
			"flw.ps f4, 10(x31)\n"
			"flw.ps f9, 9(x31)\n"
			"la x31, rand_int_32\n"
			"add x31, x31, x5\n"
			"lw x10, 0(x31)\n"
			"lw x14, 1(x31)\n"
			"lw x22, 4(x31)\n"
			"lw x23, 3(x31)\n"
			"li x21, 0x2\n"
			"LBL_SEQID_2_LOOP_SEQ_HID_0:\n"
			"fcmovm.ps f24, f16, f16\n"
			"sub x14, x23, x22\n"
			"bgeu x3, x10, 1f\n"
			"1:\n"
			"fsin.ps f9, f27\n"
			"fsat8.pi f27, f16\n"
			"fmax.ps f31, f16, f27\n"
			"divuw x10, x22, x14\n"
			"fnmadd.ps f31, f4, f28, f27, rup\n"
			"fsgnj.ps f9, f27, f9\n"
			"bne x17, x27, 1f\n"
			"1:\n"
			"sraw x23, x23, x23\n"
			"bltu x20, x12, 1f\n"
			"1:\n"
			"fsin.ps f4, f16\n"
			"fslli.pi f4, f27, 0xc\n"
			"sra x19, x10, x22\n"
			"beq x11, x2, 1f\n"
			"1:\n"
			"frsq.ps f9, f4\n"
			"frsq.ps f9, f9\n"
			"mulhu x22, x22, x22\n"
			"fminu.pi f1, f9, f27\n"
			"fmax.s f9, f27, f16\n"
			"divu x25, x14, x10\n"
			"fsub.ps f31, f27, f27, rmm\n"
			"blt x26, x16, 1f\n"
			"1:\n"
			"fmsub.s f27, f27, f27, f28, rup\n"
			"divu x22, x22, x10\n"
			"flog.ps f9, f16\n"
			"fsll.pi f1, f16, f4\n"
			"faddi.pi f9, f9, 0x197\n"
			"fmin.pi f4, f4, f4\n"
			"slli x14, x23, 0\n"
			"fsat8.pi f4, f27\n"
			"xori x14, x14, 0x16\n"
			"fmax.pi f28, f27, f16\n"
			"fle.pi f6, f28, f27\n"
			"xori x14, x22, 0x16\n"
			"srai x22, x22, 0\n"
			"bltu x4, x26, 1f\n"
			"1:\n"
			"subw x10, x23, x10\n"
			"faddi.pi f28, f9, 0x192\n"
			"fslli.pi f6, f9, 0xf\n"
			"fswizz.ps f28, f27, 0x3\n"
			"beq x15, x10, 1f\n"
			"1:\n"
			"fcvt.ps.pwu f4, f4, rup\n"
			"bgeu x17, x22, 1f\n"
			"1:\n"
			"fsatu8.pi f6, f28\n"
			"beq x13, x14, 1f\n"
			"1:\n"
			"fmulh.pi f27, f9, f27\n"
			"beq x3, x28, 1f\n"
			"1:\n"
			"ori x25, x10, 0x1e\n"
			"mulhu x10, x14, x14\n"
			"divuw x14, x14, x14\n"
			"fcvt.ps.pwu f28, f27, rdn\n"
			"fnmadd.s f1, f4, f27, f9, rup\n"
			"fmax.pi f6, f27, f4\n"
			"fsgnj.s f28, f16, f27\n"
			"fltu.pi f4, f16, f28\n"
			"slliw x10, x23, 4\n"
			"fslli.pi f9, f9, 0x7\n"
			"fmulh.pi f27, f27, f28\n"
			"ori x22, x22, 0x1d\n"
			"beq x29, x1, 1f\n"
			"1:\n"
			"fnot.pi f1, f16\n"
			"fsgnjn.s f16, f16, f16\n"
			"remw x25, x14, x14\n"
			"fnmsub.s f6, f16, f16, f16, rtz\n"
			"fnmadd.s f6, f4, f27, f27, rtz\n"
			"fmax.pi f9, f16, f28\n"
			"fsll.pi f27, f27, f16\n"
			"mulhu x10, x10, x10\n"
			"fsgnjn.s f1, f27, f28\n"
			"fsatu8.pi f24, f9\n"
			"fminu.pi f9, f27, f16\n"
			"slliw x19, x23, 1\n"
			"fnot.pi f28, f4\n"
			"fcvt.pwu.ps f28, f16, rmm\n"
			"bltu x9, x25, 1f\n"
			"1:\n"
			"divu x25, x10, x14\n"
			"packb x14, x22, x14\n"
			"fsin.ps f16, f4\n"
			"fcvt.ps.pwu f28, f4, rdn\n"
			"fsub.s f4, f28, f28, rmm\n"
			"remw x10, x22, x14\n"
			"fmsub.s f27, f27, f4, f27, rne\n"
			"fmul.s f27, f16, f4, rmm\n"
			"or x25, x22, x14\n"
			"srai x25, x10, 1\n"
			"fswizz.ps f27, f27, 0x11\n"
			"fle.pi f28, f28, f27\n"
			"fltu.pi f1, f9, f16\n"
			"divuw x23, x23, x23\n"
			"ori x14, x23, 0xc\n"
			"fmax.ps f27, f27, f16\n"
			"fnmsub.s f16, f16, f16, f4, rup\n"
			"fsat8.pi f28, f27\n"
			"fswizz.ps f4, f9, 0x11\n"
			"fcvt.ps.pwu f31, f9, rdn\n"
			"subw x10, x10, x23\n"
			"bltu x18, x20, 1f\n"
			"1:\n"
			"or x19, x22, x10\n"
			"fsin.ps f9, f28\n"
			"fltu.pi f6, f16, f27\n"
			"fcvt.ps.pwu f28, f16, rup\n"
			"fsatu8.pi f31, f28\n"
			"fsrl.pi f6, f16, f4\n"
			"bgeu x0, x10, 1f\n"
			"1:\n"
			"fnot.pi f4, f9\n"
			"fcvt.ps.pwu f1, f16, rne\n"
			"xori x10, x23, 0xa\n"
			"fsgnjn.ps f9, f4, f9\n"
			"mulhu x25, x22, x23\n"
			"frsq.ps f28, f28\n"
			"packb x10, x23, x23\n"
			"fsatu8.pi f1, f27\n"
			"fsub.s f28, f16, f28, rup\n"
			"sraw x22, x22, x14\n"
			"fsat8.pi f24, f9\n"
			"flog.ps f9, f27\n"
			"fnmadd.ps f6, f4, f4, f16, rmm\n"
			"fltu.pi f24, f27, f27\n"
			"divuw x25, x23, x23\n"
			"fswizz.ps f16, f16, 0xd\n"
			"fandi.pi f24, f16, 0x136\n"
			"fsrl.pi f27, f4, f16\n"
			"fsin.ps f27, f27\n"
			"fsgnjn.s f6, f28, f28\n"
			"bne x15, x6, 1f\n"
			"1:\n"
			"fandi.pi f28, f27, 0x109\n"
			"sra x22, x22, x23\n"
			"ori x10, x23, 0xa\n"
			"fmul.s f4, f27, f28, rdn\n"
			"frsq.ps f28, f27\n"
			"fnot.pi f4, f27\n"
			"bne x19, x3, 1f\n"
			"1:\n"
			"fandi.pi f27, f27, 0xb9\n"
			"bgeu x4, x19, 1f\n"
			"1:\n"
			"frsq.ps f9, f9\n"
			"fle.pi f24, f28, f27\n"
			"fsgnj.s f1, f9, f28\n"
			"faddi.pi f9, f16, 0x3f\n"
			"frsq.ps f27, f16\n"
			"xori x14, x14, 0xf\n"
			"bgeu x12, x4, 1f\n"
			"1:\n"
			"ori x23, x23, 0x13\n"
			"fandi.pi f27, f28, 0x19d\n"
			"sra x10, x10, x14\n"
			"fmul.s f9, f9, f28, rup\n"
			"fsub.s f27, f28, f9, rtz\n"
			"fadd.pi f1, f4, f9\n"
			"fmin.pi f9, f9, f27\n"
			"sra x22, x14, x22\n"
			"blt x14, x23, 1f\n"
			"1:\n"
			"fsgnjn.ps f16, f9, f4\n"
			"beq x17, x6, 1f\n"
			"1:\n"
			"slli x14, x23, 3\n"
			"fnmadd.ps f4, f28, f4, f27, rmm\n"
			"fnot.pi f9, f4\n"
			"bgeu x10, x28, 1f\n"
			"1:\n"
			"fsub.s f28, f28, f28, rdn\n"
			"flog.ps f27, f16\n"
			"mulhu x10, x10, x10\n"
			"fmul.ps f27, f4, f27, rup\n"
			"fminu.pi f27, f16, f28\n"
			"sub x14, x10, x10\n"
			"packb x10, x10, x14\n"
			"divuw x25, x14, x10\n"
			"bltu x9, x6, 1f\n"
			"1:\n"
			"fsgnjn.ps f27, f28, f27\n"
			"divuw x14, x14, x10\n"
			"sraw x25, x14, x10\n"
			"beq x21, x8, 1f\n"
			"1:\n"
			"fcvt.ps.pwu f31, f4, rtz\n"
			"bge x28, x4, 1f\n"
			"1:\n"
			"mulhu x14, x22, x22\n"
			"fmin.pi f4, f27, f28\n"
			"fsgnj.s f28, f28, f28\n"
			"fnmadd.s f16, f27, f16, f28, rmm\n"
			"fslli.pi f6, f28, 0x9\n"
			"fmax.ps f28, f16, f27\n"
			"fmsub.s f4, f9, f4, f16, rdn\n"
			"fmsub.s f16, f9, f16, f27, rup\n"
			"divu x25, x23, x14\n"
			"fsub.s f1, f16, f27, rtz\n"
			"fmax.s f9, f16, f9\n"
			"fnmadd.ps f27, f28, f4, f16, rdn\n"
			"addi x21, x21, -1\n"
			"bne x21, x0, LBL_SEQID_2_LOOP_SEQ_HID_0\n"
			"li x5, 3\n" 
			"bne x30, x5, LBL_FAIL_HID_0\n"
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f22,  704 (x5)\n"
			"flw.ps f0,  736 (x5)\n"
			"flw.ps f24,  128 (x5)\n"
			"flw.ps f15,  192 (x5)\n"
			"flw.ps f5,  640 (x5)\n"
			"flw.ps f21,  928 (x5)\n"
			"flw.ps f13,  32 (x5)\n"
			"flw.ps f30,  768 (x5)\n"
			"flw.ps f20,  448 (x5)\n"
			"flw.ps f25,  960 (x5)\n"
			"flw.ps f7,  896 (x5)\n"
			"flw.ps f27,  288 (x5)\n"
			"flw.ps f26,  992 (x5)\n"
			"flw.ps f16,  832 (x5)\n"
			"flw.ps f1,  672 (x5)\n"
			"flw.ps f29,  224 (x5)\n"
			"flw.ps f4,  800 (x5)\n"
			"flw.ps f17,  864 (x5)\n"
			"flw.ps f19,  544 (x5)\n"
			"flw.ps f8,  512 (x5)\n"
			"flw.ps f6,  384 (x5)\n"
			"flw.ps f28,  576 (x5)\n"
			"flw.ps f12,  480 (x5)\n"
			"flw.ps f11,  352 (x5)\n"
			"flw.ps f2,  160 (x5)\n"
			"flw.ps f10,  320 (x5)\n"
			"flw.ps f23,  64 (x5)\n"
			"flw.ps f31,  608 (x5)\n"
			"flw.ps f9,  416 (x5)\n"
			"flw.ps f3,  96 (x5)\n"
			"flw.ps f14,  256 (x5)\n"
			"flw.ps f18,  0 (x5)\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_3_FP_G_SCATTER_ID_66_HID_0:\n"
			"li x7, 0x8236555000\n" 
			"la x22, mem_ptr_offset\n"
			"flw.ps f16, 148(x22)\n"
			"fschg.ps f1, f16 (x7)\n" 
			"fmin.ps f27, f14, f6\n"
			"feq.pi f28, f9, f29\n"
			"fsll.pi f9, f15, f30\n"
			"fround.ps f30, f11, rne\n"
			"fcvt.ps.pwu f28, f16, rne\n"
			"fmaxu.pi f6, f27, f30\n"
			"frcp.ps f16, f30\n"
			"LBL_SEQID_3_INT_AMO_ID_67_HID_0:\n"
			"li x25, 0x814b43b930\n" 
			"amominl.w x7, x16, (x25)\n" 
			"srli x28, x14, 5\n"
			"fmulh.pi f16, f14, f28\n"
			"beq x10, x2, 1f\n"
			"1:\n"
			"frsq.ps f15, f6\n"
			"fle.ps f6, f4, f30\n"
			"bltu x31, x11, 1f\n"
			"1:\n"
			"fmin.s f11, f11, f9\n"
			"ffrc.ps f29, f1\n"
			"beq x31, x2, 1f\n"
			"1:\n"
			"fcvt.ps.pwu f24, f24, rup\n"
			"LBL_SEQID_3_FP_L_GATHER_ID_68_HID_0:\n"
			"li x19, 0x824afc3020\n" 
			"la x22, mem_ptr_offset\n"
			"flw.ps f9, 60(x22)\n"
			"fscwl.ps f28, f9 (x19)\n"
			"fgbl.ps f11, f9 (x19)\n" 
			"fle.pi f15, f6, f30\n"
			"bne x14, x24, 1f\n"
			"1:\n"
			"add x25, x10, x10\n"
			"ecall\n"
			"bgeu x23, x22, 1f\n"
			"1:\n"
			"fmin.s f15, f28, f27\n"
			"fcvt.wu.s x7, f28, rmm\n"
			"ecall\n"
			"fcvt.ps.pwu f12, f28, rne\n"
			"bge x12, x24, 1f\n"
			"1:\n"
			"LBL_SEQID_3_FP_G_SCATTER_ID_69_HID_0:\n"
			"li x5, 0x822d5c04ec\n" 
			"la x28, mem_ptr_offset\n"
			"flw.ps f16, 192(x28)\n"
			"fschg.ps f11, f16 (x5)\n" 
			"fcmov.ps f22, f9, f22, f1\n"
			"fcvt.f16.ps f16, f12\n"
			"bltu x3, x9, 1f\n"
			"1:\n"
			"flt.ps f29, f9, f29\n"
			"fmax.ps f31, f28, f4\n"
			"fswizz.ps f27, f9, 0xa\n"
			"frsq.ps f14, f16\n"
			"fsgnj.s f24, f28, f15\n"
			"LBL_SEQID_3_FP_G_GATHER_ID_70_HID_0:\n"
			"li x28, 0x8139e20024\n" 
			"la x5, mem_ptr_offset\n"
			"flw.ps f12, 28(x5)\n"
			"fscwg.ps f16, f12 (x28)\n"
			"fghg.ps f28, f12 (x28)\n" 
			"fsrl.pi f15, f15, f1\n"
			"fand.pi f24, f28, f12\n"
			"fsat8.pi f1, f31\n"
			"fminu.pi f30, f14, f14\n"
			"fnmsub.ps f29, f22, f12, f29, rup\n"
			"fcmovm.ps f9, f27, f14\n"
			"fpackreph.pi f24, f28\n"
			"LBL_SEQID_3_FP_AMO_ID_71_HID_0:\n"
			"li x19, 0x824e8b4760\n" 
			"la x25, mem_ptr_offset\n"
			"flw.ps f24, 480(x25)\n"
			"famominul.pi f14, f24 (x19)\n" 
			"fmadd.ps f29, f29, f29, f30, rdn\n"
			"fsrai.pi f1, f1, 0xc\n"
			"fmulh.pi f1, f29, f29\n"
			"beq x4, x20, 1f\n"
			"1:\n"
			"fle.pi f15, f1, f1\n"
			"fmsub.ps f4, f4, f22, f29, rup\n"
			"fnot.pi f15, f31\n"
			"fsin.ps f15, f15\n"
			"bltu x3, x6, 1f\n"
			"1:\n"
			"LBL_SEQID_3_INT_AMO_ID_72_HID_0:\n"
			"li x10, 0x813aa40134\n" 
			"amoorg.w x28, x25, (x10)\n" 
			"fsrai.pi f22, f22, 0xd\n"
			"fcvt.ps.f16 f6, f22\n"
			"fmul.s f22, f22, f22, rup\n"
			"bge x15, x19, 1f\n"
			"1:\n"
			"fsub.pi f6, f15, f15\n"
			"fsin.ps f6, f15\n"
			"fmin.s f22, f22, f22\n"
			"bltu x4, x22, 1f\n"
			"1:\n"
			"fxor.pi f6, f22, f24\n"
			"LBL_SEQID_3_INT_AMO_ID_73_HID_0:\n"
			"li x23, 0x8128160010\n" 
			"amoorg.w x18, x14, (x23)\n" 
			"fmulh.pi f22, f22, f28\n"
			"flt.ps f9, f27, f1\n"
			"fmadd.s f4, f29, f27, f29, rne\n"
			"blt x22, x0, 1f\n"
			"1:\n"
			"fle.ps f4, f11, f4\n"
			"beq x9, x12, 1f\n"
			"1:\n"
			"fminu.pi f31, f30, f31\n"
			"fmadd.s f27, f15, f22, f4, rtz\n"
			"flt.ps f14, f27, f14\n"
			"LBL_SEQID_3_FP_G_SCATTER_ID_74_HID_0:\n"
			"li x10, 0x8139e20024\n" 
			"la x19, mem_ptr_offset\n"
			"flq2 f4, 76(x19)\n"
			"fschg.ps f31, f4 (x10)\n" 
			"fsgnjx.ps f22, f1, f28\n"
			"slt x16, x5, x25\n"
			"fsrli.pi f24, f27, 0x9\n"
			"slli x28, x22, 2\n"
			"fclass.s x19, f22\n"
			"addi x25, x25, 0x1d\n"
			"fsub.s f24, f11, f11, rup\n"
			"LBL_SEQID_3_INT_L_STORE_ID_75_HID_0:\n"
			"li x5, 0x825d5c04ec\n" 
			"shl x28, (x5)\n" 
			"fmax.s f31, f31, f1\n"
			"fnmsub.ps f15, f31, f12, f6, rtz\n"
			"fcvt.pwu.ps f22, f28, rtz\n"
			"faddi.pi f6, f27, 0x30\n"
			"bgeu x21, x6, 1f\n"
			"1:\n"
			"fmul.s f1, f31, f31, rdn\n"
			"fmulh.pi f1, f31, f1\n"
			"bge x14, x29, 1f\n"
			"1:\n"
			"for.pi f27, f1, f12\n"
			"LBL_SEQID_3_INT_AMO_ID_76_HID_0:\n"
			"li x22, 0x8142339830\n" 
			"amominl.w x23, x10, (x22)\n" 
			"fsgnjn.s f16, f1, f16\n"
			"fcvt.f16.ps f29, f14\n"
			"flt.pi f24, f1, f4\n"
			"fpackrepb.pi f28, f22\n"
			"fcvt.ps.f16 f9, f9\n"
			"bgeu x5, x24, 1f\n"
			"1:\n"
			"fnot.pi f14, f9\n"
			"fcvt.ps.pw f11, f11, rdn\n"
			"LBL_SEQID_3_INT_AMO_ID_77_HID_0:\n"
			"li x7, 0x81296c5808\n" 
			"amoming.d x25, x22, (x7)\n" 
			"fsgnj.ps f29, f15, f29\n"
			"flt.s x18, f11, f1\n"
			"fsub.s f12, f30, f12, rne\n"
			"fsrl.pi f30, f9, f9\n"
			"fexp.ps f6, f28\n"
			"fpackrepb.pi f27, f31\n"
			"fsatu8.pi f1, f6\n"
			"LBL_SEQID_3_INT_AMO_ID_78_HID_0:\n"
			"li x23, 0x822a3c0160\n" 
			"amominug.w x19, x10, (x23)\n" 
			"fsgnj.ps f27, f22, f30\n"
			"fcvt.ps.pwu f29, f11, rup\n"
			"frsq.ps f9, f16\n"
			"fmul.s f9, f4, f9, rup\n"
			"feq.ps f11, f27, f9\n"
			"fsgnj.ps f29, f6, f14\n"
			"bgeu x18, x0, 1f\n"
			"1:\n"
			"fswizz.ps f27, f14, 0xa\n"
			"bge x31, x1, 1f\n"
			"1:\n"
			"LBL_SEQID_3_FP_L_SCATTER_ID_79_HID_0:\n"
			"li x5, 0x8148160010\n" 
			"la x18, mem_ptr_offset\n"
			"flq2 f9, 40(x18)\n"
			"fscwl.ps f1, f9 (x5)\n" 
			"fsgnj.ps f6, f31, f9\n"
			"bge x9, x0, 1f\n"
			"1:\n"
			"fsgnjn.s f28, f24, f24\n"
			"ebreak\n"
			"fmadd.s f11, f24, f24, f11, rup\n"
			"fpackreph.pi f31, f24\n"
			"blt x23, x2, 1f\n"
			"1:\n"
			"fbci.ps f14, 0x725d8\n"
			"mulw x5, x23, x19\n"
			"bltu x9, x11, 1f\n"
			"1:\n"
			"LBL_SEQID_3_FP_AMO_ID_80_HID_0:\n"
			"li x10, 0x8250120024\n" 
			"la x7, mem_ptr_offset\n"
			"flw.ps f30, 640(x7)\n"
			"famomaxl.ps f28, f30 (x10)\n" 
			"feq.ps f15, f31, f15\n"
			"lui x10, 0x19\n"
			"fle.s x14, f15, f28\n"
			"fcvt.ps.pw f6, f31, rdn\n"
			"fminu.pi f6, f24, f31\n"
			"mova.m.x x25\n"
			"remw x25, x18, x25\n"
			"LBL_SEQID_3_FP_AMO_ID_81_HID_0:\n"
			"li x16, 0x814d7e0008\n" 
			"la x18, mem_ptr_offset\n"
			"flw.ps f29, 608(x18)\n"
			"famoswapl.pi f30, f29 (x16)\n" 
			"divuw x23, x14, x28\n"
			"fsub.s f22, f9, f29, rup\n"
			"fmax.ps f12, f4, f22\n"
			"bgeu x6, x12, 1f\n"
			"1:\n"
			"sraw x5, x5, x14\n"
			"sraiw x16, x22, 2\n"
			"fmax.pi f1, f12, f6\n"
			"fnmsub.ps f29, f27, f6, f30, rne\n"
			"LBL_SEQID_3_INT_AMO_ID_82_HID_0:\n"
			"li x14, 0x81227db348\n" 
			"amoming.d x23, x28, (x14)\n" 
			"fcvt.ps.f16 f16, f1\n"
			"bne x24, x28, 1f\n"
			"1:\n"
			"fsgnjx.s f30, f22, f28\n"
			"ffrc.ps f27, f15\n"
			"frcp.ps f1, f27\n"
			"fmadd.ps f24, f29, f24, f9, rtz\n"
			"fmul.pi f22, f22, f30\n"
			"fmax.pi f22, f6, f29\n"
			"LBL_SEQID_3_FP_G_LOAD_ID_83_HID_0:\n"
			"li x7, 0x8139e20020\n" 
			"mov.m.x m0, zero, 0xff\n"
			"fswg.ps f15, (x7)\n"
			"flwg.ps f4, (x7)\n" 
			"fclass.ps f4, f22\n"
			"maskor m1, m1, m0\n"
			"rem x22, x14, x5\n"
			"fmax.pi f1, f22, f1\n"
			"ecall\n"
			"bgeu x21, x23, 1f\n"
			"1:\n"
			"frsq.ps f11, f1\n"
			"fandi.pi f22, f22, 0x150\n"
			"LBL_SEQID_3_INT_AMO_ID_84_HID_0:\n"
			"li x14, 0x814895f828\n" 
			"amoaddl.d x10, x28, (x14)\n" 
			"fmvs.x.ps x14, f16, 3\n"
			"ori x23, x23, 0xa\n"
			"and x19, x10, x10\n"
			"fsub.pi f9, f6, f11\n"
			"fmadd.ps f29, f29, f16, f16, rtz\n"
			"fmul.pi f9, f9, f11\n"
			"fsub.pi f9, f16, f9\n"
			"LBL_SEQID_3_INT_L_STORE_ID_85_HID_0:\n"
			"li x28, 0x82495406d8\n" 
			"sbl x10, (x28)\n" 
			"fmsub.s f15, f30, f27, f9, rdn\n"
			"fltu.pi f27, f30, f9\n"
			"xor x7, x14, x28\n"
			"feq.pi f9, f27, f9\n"
			"bltu x12, x5, 1f\n"
			"1:\n"
			"fxor.pi f6, f27, f27\n"
			"andi x28, x16, 0x7\n"
			"blt x8, x4, 1f\n"
			"1:\n"
			"fround.ps f30, f9, rdn\n"
			"LBL_SEQID_3_INT_AMO_ID_86_HID_0:\n"
			"li x25, 0x8225060000\n" 
			"amoandg.d x28, x22, (x25)\n" 
			"flt.pi f6, f27, f9\n"
			"fmulh.pi f4, f22, f9\n"
			"fmax.s f4, f12, f22\n"
			"beq x9, x29, 1f\n"
			"1:\n"
			"fslli.pi f31, f31, 0x0\n"
			"fle.pi f27, f9, f31\n"
			"fsub.ps f31, f15, f24, rdn\n"
			"fsub.ps f9, f29, f6, rdn\n"
			"LBL_SEQID_3_FP_AMO_ID_87_HID_0:\n"
			"li x28, 0x815ae6f708\n" 
			"la x10, mem_ptr_offset\n"
			"flw.ps f22, 896(x10)\n"
			"famoswapl.pi f12, f22 (x28)\n" 
			"fcvt.ps.f16 f11, f11\n"
			"bge x15, x25, 1f\n"
			"1:\n"
			"fmaxu.pi f29, f15, f1\n"
			"fminu.pi f22, f29, f22\n"
			"fsub.pi f29, f29, f1\n"
			"fcvt.pw.ps f6, f1, rtz\n"
			"fcvt.pw.ps f22, f22, rmm\n"
			"beq x6, x1, 1f\n"
			"1:\n"
			"fround.ps f15, f22, rne\n"
			"LBL_SEQID_3_FP_SCATTER_ID_88_HID_0:\n"
			"li x10, 0x820d5c04ed\n" 
			"li x7, 0x1dde9a2b57ecedb6\n"
			"fsc32b.ps f12, x7 (x10)\n" 
			"fmin.s f9, f28, f28\n"
			"bge x27, x18, 1f\n"
			"1:\n"
			"fle.s x7, f28, f24\n"
			"fbci.pi f29, 0x53ebd\n"
			"sra x25, x7, x25\n"
			"bge x3, x10, 1f\n"
			"1:\n"
			"fsgnjx.s f12, f24, f28\n"
			"fsgnj.s f11, f4, f28\n"
			"mulhsu x16, x23, x25\n"
			"LBL_SEQID_3_FP_STORE_ID_89_HID_0:\n"
			"li x23, 0x810da13814\n" 
			"fsw.ps f22, -1226(x23)\n" 
			"remu x14, x18, x18\n"
			"fsub.s f1, f1, f27, rup\n"
			"fmin.ps f30, f1, f1\n"
			"addiw x5, x28, 0x2\n"
			"rem x7, x23, x23\n"
			"beq x11, x24, 1f\n"
			"1:\n"
			"mulh x14, x5, x18\n"
			"fmax.ps f12, f27, f1\n"
			"bge x31, x4, 1f\n"
			"1:\n"
			"LBL_SEQID_3_FP_AMO_ID_90_HID_0:\n"
			"li x28, 0x8246920008\n" 
			"la x14, mem_ptr_offset\n"
			"flw.ps f12, 768(x14)\n"
			"famominul.pi f1, f12 (x28)\n" 
			"fmsub.s f22, f22, f22, f27, rne\n"
			"fmin.ps f9, f29, f16\n"
			"auipc x5, 0x19\n"
			"fcvt.ps.f16 f22, f14\n"
			"fsra.pi f31, f27, f27\n"
			"fltu.pi f4, f6, f14\n"
			"fmul.ps f27, f28, f11, rup\n"
			"LBL_SEQID_3_INT_L_STORE_ID_91_HID_0:\n"
			"li x18, 0x8252a42018\n" 
			"shl x19, (x18)\n" 
			"fcvt.ps.pwu f1, f24, rup\n"
			"blt x29, x23, 1f\n"
			"1:\n"
			"fnot.pi f24, f4\n"
			"fmaxu.pi f24, f1, f4\n"
			"blt x13, x24, 1f\n"
			"1:\n"
			"fsat8.pi f24, f1\n"
			"bge x11, x5, 1f\n"
			"1:\n"
			"fsgnjx.ps f14, f4, f4\n"
			"fsub.ps f24, f24, f24, rup\n"
			"fswizz.ps f14, f24, 0x1b\n"
			"LBL_SEQID_3_INT_AMO_ID_92_HID_0:\n"
			"li x25, 0x815ee69310\n" 
			"amoaddl.w x28, x22, (x25)\n" 
			"fmin.s f30, f30, f30\n"
			"fbcx.ps f4, x10\n"
			"fence\n"
			"mulh x28, x5, x23\n"
			"fcvt.w.s x23, f14, rne\n"
			"fcvt.ps.pw f30, f14, rmm\n"
			"fadd.s f6, f27, f15, rne\n"
			"LBL_SEQID_3_FP_G_SCATTER_ID_93_HID_0:\n"
			"li x23, 0x812ae6f708\n" 
			"la x28, mem_ptr_offset\n"
			"flw.ps f24, 216(x28)\n"
			"fschg.ps f12, f24 (x23)\n" 
			"fmadd.s f22, f22, f9, f24, rmm\n"
			"fmax.ps f24, f24, f22\n"
			"fclass.ps f1, f22\n"
			"fsub.pi f1, f22, f29\n"
			"blt x25, x1, 1f\n"
			"1:\n"
			"fsrai.pi f16, f29, 0x7\n"
			"fmul.s f15, f15, f29, rtz\n"
			"fltu.pi f9, f9, f22\n"
			"LBL_SEQID_3_FP_STORE_ID_94_HID_0:\n"
			"li x10, 0x8202a42019\n" 
			"fsw f27, 1441(x10)\n" 
			"fpackrepb.pi f1, f14\n"
			"blt x2, x10, 1f\n"
			"1:\n"
			"feq.ps f24, f1, f30\n"
			"bgeu x10, x28, 1f\n"
			"1:\n"
			"fadd.pi f28, f29, f28\n"
			"beq x9, x23, 1f\n"
			"1:\n"
			"fsat8.pi f30, f14\n"
			"fand.pi f31, f29, f28\n"
			"remw x5, x14, x5\n"
			"srliw x5, x16, 4\n"
			"LBL_SEQID_3_FP_STORE_ID_95_HID_0:\n"
			"li x5, 0x81050e06b8\n" 
			"fsw.ps f29, -974(x5)\n" 
			"fclass.s x7, f11\n"
			"sub x5, x18, x23\n"
			"beq x20, x0, 1f\n"
			"1:\n"
			"divuw x10, x23, x5\n"
			"fsgnjx.s f24, f22, f14\n"
			"fpackreph.pi f31, f22\n"
			"bgeu x24, x11, 1f\n"
			"1:\n"
			"divw x18, x5, x14\n"
			"fmvs.x.ps x28, f22, 1\n"
			"LBL_SEQID_3_FP_LOAD_ID_96_HID_0:\n"
			"li x5, 0x810895f82f\n" 
			"fsq2 f24, -1528(x5)\n"
			"flw.ps f4, -1528(x5)\n" 
			"fcvt.ps.f16 f4, f4\n"
			"flt.pi f12, f1, f11\n"
			"fslli.pi f24, f30, 0x3\n"
			"fadd.ps f12, f11, f15, rtz\n"
			"beq x9, x22, 1f\n"
			"1:\n"
			"fsrai.pi f22, f1, 0x8\n"
			"bgeu x19, x22, 1f\n"
			"1:\n"
			"fcvt.ps.pwu f9, f9, rne\n"
			"fsub.ps f1, f12, f9, rne\n"
			"LBL_SEQID_3_FP_SCATTER_ID_97_HID_0:\n"
			"li x23, 0x820b7a46cf\n" 
			"la x16, mem_ptr_offset\n"
			"flq2 f4, 168(x16)\n"
			"fsch.ps f6, f4 (x23)\n" 
			"frcp.ps f29, f14\n"
			"fsgnjx.s f6, f4, f31\n"
			"fsll.pi f29, f14, f4\n"
			"fclass.ps f29, f31\n"
			"bge x12, x27, 1f\n"
			"1:\n"
			"fle.pi f12, f1, f30\n"
			"fmin.s f31, f4, f14\n"
			"fsgnj.s f31, f4, f14\n"
			"mov.m.x m0, x0, 0xFF\n"
			"li x5, 0x80\n"
			"la x31, rand_ieee754_32\n"
			"add x31, x31, x5\n"
			"flw.ps f11, 0(x31)\n"
			"flw.ps f12, 0(x31)\n"
			"flw.ps f14, 8(x31)\n"
			"flw.ps f15, 6(x31)\n"
			"flw.ps f16, 10(x31)\n"
			"flw.ps f22, 3(x31)\n"
			"flw.ps f24, 9(x31)\n"
			"flw.ps f27, 26(x31)\n"
			"flw.ps f30, 9(x31)\n"
			"flw.ps f31, 20(x31)\n"
			"flw.ps f4, 21(x31)\n"
			"la x31, rand_int_32\n"
			"add x31, x31, x5\n"
			"lw x10, 0(x31)\n"
			"lw x14, 0(x31)\n"
			"lw x16, 5(x31)\n"
			"lw x18, 10(x31)\n"
			"lw x19, 15(x31)\n"
			"lw x22, 13(x31)\n"
			"lw x23, 23(x31)\n"
			"lw x25, 8(x31)\n"
			"lw x28, 23(x31)\n"
			"lw x5, 23(x31)\n"
			"lw x7, 36(x31)\n"
			"li x21, 0x3\n"
			"LBL_SEQID_3_LOOP_SEQ_HID_0:\n"
			"sraiw x28, x7, 3\n"
			"fcvt.ps.f16 f11, f15\n"
			"sub x7, x28, x7\n"
			"fcvt.ps.pwu f24, f31, rdn\n"
			"flog.ps f4, f31\n"
			"sub x25, x23, x16\n"
			"fsrli.pi f12, f27, 0x4\n"
			"fnmadd.ps f6, f22, f22, f22, rmm\n"
			"fmvs.x.ps x7, f14, 4\n"
			"slliw x19, x10, 0\n"
			"sltu x19, x18, x25\n"
			"xori x10, x19, 0x1b\n"
			"fminu.pi f22, f22, f14\n"
			"bltu x0, x21, 1f\n"
			"1:\n"
			"ffrc.ps f11, f22\n"
			"fnmsub.s f4, f31, f31, f30, rne\n"
			"fence\n"
			"fsll.pi f30, f27, f27\n"
			"fround.ps f31, f11, rmm\n"
			"fmax.s f9, f22, f14\n"
			"fmv.s.x f31, x5\n"
			"fmsub.s f6, f14, f4, f31, rup\n"
			"fsgnjx.s f12, f22, f14\n"
			"ffrc.ps f4, f22\n"
			"fpackreph.pi f4, f11\n"
			"fandi.pi f1, f4, 0xce\n"
			"fmin.s f30, f30, f24\n"
			"mulhu x10, x7, x22\n"
			"sraw x5, x19, x18\n"
			"fsub.ps f9, f27, f14, rmm\n"
			"maskpopcz x19, m1\n"
			"flt.pi f30, f31, f12\n"
			"srai x10, x16, 4\n"
			"fadd.pi f24, f31, f24\n"
			"sltu x14, x25, x22\n"
			"flt.pi f4, f27, f24\n"
			"srl x28, x16, x16\n"
			"fcvt.wu.s x10, f30, rtz\n"
			"feq.s x23, f11, f15\n"
			"fmadd.ps f31, f4, f4, f31, rdn\n"
			"sltiu x5, x25, 0x1c\n"
			"fltu.pi f24, f12, f12\n"
			"bltu x19, x18, 1f\n"
			"1:\n"
			"ori x10, x25, 0xf\n"
			"fpackrepb.pi f9, f14\n"
			"fmin.pi f9, f24, f14\n"
			"fmin.ps f11, f11, f30\n"
			"fsrai.pi f22, f22, 0x6\n"
			"bgeu x25, x3, 1f\n"
			"1:\n"
			"fsll.pi f12, f14, f22\n"
			"sraiw x23, x14, 2\n"
			"fmvs.x.ps x18, f15, 6\n"
			"fcvt.pw.ps f24, f22, rtz\n"
			"auipc x7, 0x1f\n"
			"bne x30, x6, 1f\n"
			"1:\n"
			"sltiu x10, x22, 0xa\n"
			"sra x28, x7, x10\n"
			"fmv.s.x f27, x16\n"
			"fslli.pi f22, f11, 0x4\n"
			"mulh x18, x10, x14\n"
			"maskor m0, m0, m0\n"
			"mov.m.x m0, x0, 0xff\n"
			"fle.s x10, f24, f24\n"
			"and x7, x23, x23\n"
			"sraw x5, x25, x5\n"
			"bltu x20, x29, 1f\n"
			"1:\n"
			"xori x23, x23, 0x14\n"
			"bgeu x14, x29, 1f\n"
			"1:\n"
			"mov.m.x m6, x10, 0x27\n"
			"xor x28, x22, x14\n"
			"fmin.pi f31, f24, f16\n"
			"andi x16, x25, 0x10\n"
			"bgeu x16, x23, 1f\n"
			"1:\n"
			"fcvt.ps.pw f12, f24, rtz\n"
			"slliw x10, x10, 1\n"
			"fmsub.s f22, f31, f30, f22, rup\n"
			"fltm.pi m1, f14, f30\n"
			"divuw x7, x28, x5\n"
			"fslli.pi f24, f30, 0xa\n"
			"fpackrepb.pi f15, f15\n"
			"fle.ps f15, f16, f15\n"
			"fmv.s.x f30, x18\n"
			"fmsub.s f24, f15, f14, f30, rdn\n"
			"fsrl.pi f30, f27, f31\n"
			"mulw x23, x7, x18\n"
			"sraiw x16, x16, 5\n"
			"fmin.s f15, f14, f4\n"
			"maskpopc x25, m6\n"
			"fminu.pi f16, f16, f30\n"
			"fsgnj.ps f31, f15, f11\n"
			"ffrc.ps f11, f12\n"
			"slti x19, x7, 0x0\n"
			"divuw x22, x19, x18\n"
			"fclass.ps f9, f15\n"
			"srliw x28, x28, 0\n"
			"slli x18, x25, 4\n"
			"srl x25, x14, x22\n"
			"fmvs.x.ps x10, f31, 5\n"
			"beq x31, x31, 1f\n"
			"1:\n"
			"fmvz.x.ps x23, f12, 4\n"
			"sub x23, x23, x5\n"
			"divw x25, x25, x10\n"
			"div x22, x19, x10\n"
			"fmul.pi f12, f24, f12\n"
			"fmul.s f30, f27, f24, rup\n"
			"flt.s x16, f24, f12\n"
			"fmul.s f15, f15, f30, rne\n"
			"fsat8.pi f12, f4\n"
			"beq x11, x7, 1f\n"
			"1:\n"
			"fsub.pi f15, f16, f22\n"
			"slliw x5, x5, 0\n"
			"bge x12, x3, 1f\n"
			"1:\n"
			"srai x28, x18, 1\n"
			"fsgnj.ps f22, f4, f30\n"
			"xori x5, x7, 0x14\n"
			"fnmsub.ps f6, f15, f22, f30, rne\n"
			"srlw x14, x10, x14\n"
			"fmadd.ps f12, f11, f16, f4, rtz\n"
			"frcp.ps f22, f4\n"
			"frsq.ps f12, f27\n"
			"fcvt.pw.ps f11, f4, rup\n"
			"fmul.pi f11, f11, f31\n"
			"fnmadd.ps f12, f22, f14, f12, rtz\n"
			"srl x22, x22, x10\n"
			"fle.pi f12, f16, f31\n"
			"auipc x19, 0xd\n"
			"bgeu x1, x2, 1f\n"
			"1:\n"
			"fmv.x.s x7, f24\n"
			"ori x22, x7, 0x17\n"
			"feq.pi f30, f4, f4\n"
			"ecall\n"
			"fsrai.pi f31, f14, 0x0\n"
			"fsub.pi f27, f24, f24\n"
			"bne x13, x0, 1f\n"
			"1:\n"
			"sra x25, x10, x5\n"
			"fsgnj.ps f6, f30, f30\n"
			"beq x29, x2, 1f\n"
			"1:\n"
			"fcvt.wu.s x7, f16, rne\n"
			"fsub.ps f22, f22, f4, rne\n"
			"feq.ps f14, f12, f14\n"
			"sraw x28, x25, x28\n"
			"fsatu8.pi f24, f24\n"
			"beq x1, x4, 1f\n"
			"1:\n"
			"slti x22, x19, 0x1f\n"
			"fsgnjx.s f15, f15, f30\n"
			"fnot.pi f15, f15\n"
			"for.pi f14, f22, f14\n"
			"fcvt.pwu.ps f28, f4, rdn\n"
			"maskpopcz x28, m2\n"
			"fle.s x25, f27, f22\n"
			"bge x20, x9, 1f\n"
			"1:\n"
			"slt x19, x23, x28\n"
			"beq x2, x14, 1f\n"
			"1:\n"
			"flem.ps m0, f24, f27\n"
			"slli x28, x28, 5\n"
			"fadd.ps f12, f16, f14, rtz\n"
			"addi x21, x21, -1\n"
			"bne x21, x0, LBL_SEQID_3_LOOP_SEQ_HID_0\n"
			"li x5, 4\n" 
			"bne x30, x5, LBL_FAIL_HID_0\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_4_FP_AMO_ID_98_HID_0:\n"
			"li x25, 0x8255de580c\n" 
			"la x23, mem_ptr_offset\n"
			"flw.ps f4, 512(x23)\n"
			"famoswapl.pi f11, f4 (x25)\n" 
			"fmulh.pi f14, f31, f11\n"
			"fexp.ps f31, f1\n"
			"fsat8.pi f15, f9\n"
			"bge x25, x13, 1f\n"
			"1:\n"
			"fmax.ps f16, f30, f30\n"
			"bne x5, x7, 1f\n"
			"1:\n"
			"fsat8.pi f22, f31\n"
			"fnmsub.s f27, f9, f22, f11, rne\n"
			"LBL_SEQID_4_INT_AMO_ID_99_HID_0:\n"
			"li x28, 0x8246920008\n" 
			"amoandl.d x16, x14, (x28)\n" 
			"fltu.pi f16, f16, f24\n"
			"fmax.ps f14, f28, f4\n"
			"fcvt.f16.ps f24, f24\n"
			"bne x27, x14, 1f\n"
			"1:\n"
			"srl x19, x25, x14\n"
			"fmulh.pi f16, f16, f28\n"
			"fnmsub.s f14, f14, f30, f4, rne\n"
			"LBL_SEQID_4_INT_STORE_ID_100_HID_0:\n"
			"li x16, 0x820dcf2eaf\n" 
			"sb x10, -1749(x16)\n" 
			"fswizz.ps f11, f28, 0x1a\n"
			"fmadd.ps f14, f15, f28, f14, rtz\n"
			"fsatu8.pi f22, f29\n"
			"blt x16, x31, 1f\n"
			"1:\n"
			"fmul.ps f27, f31, f15, rmm\n"
			"fexp.ps f14, f28\n"
			"fsin.ps f27, f22\n"
			"LBL_SEQID_4_FP_LOAD_ID_101_HID_0:\n"
			"li x25, 0x8102339832\n" 
			"fsw.ps f11, 20(x25)\n"
			"flq2 f12, 20(x25)\n" 
			"flog.ps f30, f12\n"
			"blt x14, x25, 1f\n"
			"1:\n"
			"fexp.ps f16, f28\n"
			"frsq.ps f6, f4\n"
			"for.pi f12, f12, f1\n"
			"flt.ps f6, f28, f6\n"
			"fmulhu.pi f1, f1, f1\n"
			"LBL_SEQID_4_INT_AMO_ID_102_HID_0:\n"
			"li x14, 0x8220bc5020\n" 
			"amomaxg.d x5, x22, (x14)\n" 
			"packb x19, x7, x19\n"
			"fcvt.pw.ps f15, f6, rup\n"
			"fmax.ps f15, f6, f14\n"
			"faddi.pi f12, f14, 0x49\n"
			"bge x5, x13, 1f\n"
			"1:\n"
			"fswizz.ps f6, f15, 0x17\n"
			"bne x19, x29, 1f\n"
			"1:\n"
			"remuw x23, x25, x23\n"
			"LBL_SEQID_4_CSR_ID_103_HID_0:\n"
			"li x5, 0x81096c580a\n"
			"li x31, (0x3 << 6) | 0x1\n"
			"csrw prefetch_va, x5\n" 
			"csrwi tensor_wait, 6\n" 
			"csrwi tensor_wait, 5\n" 
			"feq.s x18, f29, f29\n"
			"fslli.pi f29, f29, 0x1\n"
			"maskor m0, m1, m0\n"
			"fcmovm.ps f12, f24, f29\n"
			"slti x22, x23, 0x12\n"
			"fadd.ps f24, f29, f24, rup\n"
			"LBL_SEQID_4_FP_LOAD_ID_104_HID_0:\n"
			"li x28, 0x82095406d9\n" 
			"fsw.ps f30, 763(x28)\n"
			"flq2 f22, 763(x28)\n" 
			"fence\n"
			"fxor.pi f28, f24, f31\n"
			"faddi.pi f11, f15, 0x1fd\n"
			"fmvs.x.ps x16, f15, 2\n"
			"beq x4, x20, 1f\n"
			"1:\n"
			"fsgnjn.ps f29, f15, f15\n"
			"fle.s x19, f22, f22\n"
			"LBL_SEQID_4_FP_AMO_ID_105_HID_0:\n"
			"li x22, 0x813da13814\n" 
			"la x18, mem_ptr_offset\n"
			"flw.ps f16, 544(x18)\n"
			"famomaxg.pi f15, f16 (x22)\n" 
			"flog.ps f6, f9\n"
			"fmin.s f9, f31, f9\n"
			"fnmadd.s f30, f30, f15, f16, rup\n"
			"fsub.s f15, f24, f15, rmm\n"
			"fadd.pi f6, f24, f11\n"
			"fsra.pi f15, f16, f4\n"
			"LBL_SEQID_4_FP_LOAD_ID_106_HID_0:\n"
			"li x7, 0x81096c580b\n" 
			"fsq2 f4, -1099(x7)\n"
			"flw.ps f30, -1099(x7)\n" 
			"faddi.pi f11, f9, 0x17d\n"
			"bltu x27, x25, 1f\n"
			"1:\n"
			"fcvt.ps.f16 f29, f29\n"
			"fsra.pi f11, f11, f4\n"
			"fmul.pi f31, f28, f22\n"
			"bge x13, x8, 1f\n"
			"1:\n"
			"fnmsub.ps f31, f31, f30, f28, rmm\n"
			"fslli.pi f9, f22, 0x6\n"
			"LBL_SEQID_4_FP_G_SCATTER_ID_107_HID_0:\n"
			"li x25, 0x81250e06b8\n" 
			"la x7, mem_ptr_offset\n"
			"flw.ps f15, 92(x7)\n"
			"fschg.ps f16, f15 (x25)\n" 
			"fmaxu.pi f15, f22, f15\n"
			"fmin.ps f24, f15, f1\n"
			"fsgnj.s f11, f27, f9\n"
			"fxor.pi f12, f9, f28\n"
			"fsub.s f24, f1, f1, rup\n"
			"fcmovm.ps f29, f14, f29\n"
			"LBL_SEQID_4_INT_L_STORE_ID_108_HID_0:\n"
			"li x19, 0x815824ee8c\n" 
			"shl x10, (x19)\n" 
			"fcmov.ps f31, f27, f12, f27\n"
			"frcp.ps f24, f27\n"
			"fandi.pi f29, f22, 0x1eb\n"
			"fmsub.s f12, f29, f22, f12, rne\n"
			"fcvt.pwu.ps f29, f15, rmm\n"
			"fswizz.ps f4, f15, 0x4\n"
			"LBL_SEQID_4_CSR_ID_109_HID_0:\n"
			"csrr x0, mtval\n"
			"div x23, x28, x23\n"
			"subw x5, x16, x19\n"
			"addi x28, x28, 0xe\n"
			"bge x24, x18, 1f\n"
			"1:\n"
			"fmin.ps f24, f11, f9\n"
			"blt x15, x18, 1f\n"
			"1:\n"
			"ori x10, x10, 0xf\n"
			"fle.pi f12, f12, f29\n"
			"LBL_SEQID_4_FP_GATHER_ID_110_HID_0:\n"
			"li x16, 0x81096c580b\n" 
			"la x14, mem_ptr_offset\n"
			"flq2 f15, 20(x14)\n"
			"fscw.ps f25, f15 (x16)\n"
			"fgw.ps f9, f15 (x16)\n" 
			"mul x18, x19, x16\n"
			"fswizz.ps f9, f30, 0x1a\n"
			"fmax.ps f27, f27, f22\n"
			"bge x9, x11, 1f\n"
			"1:\n"
			"divw x25, x28, x10\n"
			"mova.m.x x5\n"
			"mulw x22, x18, x10\n"
			"bltu x0, x25, 1f\n"
			"1:\n"
			"LBL_SEQID_4_FP_STORE_ID_111_HID_0:\n"
			"li x18, 0x81050e06b8\n" 
			"fsw f4, -964(x18)\n" 
			"fswizz.ps f24, f4, 0xe\n"
			"feq.ps f16, f16, f30\n"
			"blt x8, x15, 1f\n"
			"1:\n"
			"fnmadd.ps f28, f22, f1, f1, rup\n"
			"fcmovm.ps f29, f11, f1\n"
			"fmsub.ps f14, f12, f12, f11, rne\n"
			"fcmov.ps f30, f12, f30, f31\n"
			"LBL_SEQID_4_INT_AMO_ID_112_HID_0:\n"
			"li x23, 0x8222a42018\n" 
			"amomaxg.w x5, x10, (x23)\n" 
			"fnmsub.ps f16, f29, f15, f1, rmm\n"
			"bltu x26, x25, 1f\n"
			"1:\n"
			"fsrli.pi f27, f30, 0xe\n"
			"fnot.pi f4, f9\n"
			"fcmovm.ps f30, f30, f28\n"
			"fmin.ps f29, f6, f27\n"
			"fsll.pi f12, f6, f11\n"
			"LBL_SEQID_4_FP_SCATTER_ID_113_HID_0:\n"
			"li x18, 0x8200120025\n" 
			"li x25, 0xfaa3eedf3aa4b03c\n"
			"fsc32b.ps f6, x25 (x18)\n" 
			"slt x10, x23, x23\n"
			"lui x14, 0x17\n"
			"blt x10, x29, 1f\n"
			"1:\n"
			"maskor m2, m0, m6\n"
			"mov.m.x m0, x0, 0xff\n"
			"srai x10, x16, 1\n"
			"fle.pi f29, f6, f29\n"
			"fltm.pi m1, f6, f9\n"
			"LBL_SEQID_4_FP_GATHER_ID_114_HID_0:\n"
			"li x28, 0x820dcf2eaf\n" 
			"la x23, mem_ptr_offset\n"
			"flq2 f14, 24(x23)\n"
			"fscw.ps f21, f14 (x28)\n"
			"fgw.ps f22, f14 (x28)\n" 
			"fadd.s f11, f22, f27, rne\n"
			"frcp.ps f28, f6\n"
			"fsub.s f1, f14, f4, rtz\n"
			"fsgnj.s f12, f12, f30\n"
			"fmul.s f16, f24, f22, rup\n"
			"feq.ps f15, f28, f14\n"
			"bne x27, x30, 1f\n"
			"1:\n"
			"LBL_SEQID_4_FP_LOAD_ID_115_HID_0:\n"
			"li x18, 0x810c7e0030\n" 
			"fsq2 f1, -2040(x18)\n"
			"flw.ps f15, -2040(x18)\n" 
			"frcp.ps f1, f1\n"
			"beq x9, x18, 1f\n"
			"1:\n"
			"ffrc.ps f27, f22\n"
			"fsgnjx.s f27, f31, f31\n"
			"flt.pi f4, f24, f4\n"
			"feq.ps f1, f1, f24\n"
			"fswizz.ps f22, f22, 0x18\n"
			"LBL_SEQID_4_FP_L_SCATTER_ID_116_HID_0:\n"
			"li x25, 0x815ae6f708\n" 
			"la x22, mem_ptr_offset\n"
			"flw.ps f30, 140(x22)\n"
			"fscwl.ps f4, f30 (x25)\n" 
			"remw x14, x19, x28\n"
			"fcvt.s.w f30, x18, rtz\n"
			"fmul.s f9, f29, f14, rdn\n"
			"fmadd.s f16, f15, f12, f9, rne\n"
			"fmv.x.s x14, f15\n"
			"frsq.ps f16, f9\n"
			"bge x2, x29, 1f\n"
			"1:\n"
			"LBL_SEQID_4_FP_G_SCATTER_ID_117_HID_0:\n"
			"li x16, 0x8121ee0014\n" 
			"la x10, mem_ptr_offset\n"
			"flw.ps f9, 156(x10)\n"
			"fschg.ps f11, f9 (x16)\n" 
			"flt.ps f27, f6, f28\n"
			"fsrl.pi f4, f16, f15\n"
			"fsub.ps f22, f22, f12, rmm\n"
			"faddi.pi f4, f28, 0xfc\n"
			"fmin.s f9, f12, f4\n"
			"fltu.pi f1, f14, f27\n"
			"LBL_SEQID_4_FP_AMO_ID_118_HID_0:\n"
			"li x14, 0x814da13814\n" 
			"la x5, mem_ptr_offset\n"
			"flw.ps f24, 800(x5)\n"
			"famoswapl.pi f1, f24 (x14)\n" 
			"fbci.pi f28, 0x39e9\n"
			"fmul.s f16, f24, f24, rne\n"
			"fmv.s.x f9, x22\n"
			"xori x10, x14, 0x2\n"
			"fcvt.ps.pw f15, f24, rtz\n"
			"bge x14, x0, 1f\n"
			"1:\n"
			"fcvt.s.w f30, x22, rtz\n"
			"LBL_SEQID_4_INT_AMO_ID_119_HID_0:\n"
			"li x18, 0x8220120024\n" 
			"amominug.w x5, x25, (x18)\n" 
			"fminu.pi f31, f31, f30\n"
			"fmin.ps f12, f16, f31\n"
			"fsrl.pi f28, f30, f4\n"
			"fltu.pi f12, f24, f4\n"
			"fslli.pi f16, f16, 0xf\n"
			"fclass.ps f11, f29\n"
			"LBL_SEQID_4_FP_L_SCATTER_ID_120_HID_0:\n"
			"li x19, 0x8240bc5020\n" 
			"la x28, mem_ptr_offset\n"
			"flw.ps f16, 0(x28)\n"
			"fscwl.ps f14, f16 (x19)\n" 
			"flog.ps f31, f28\n"
			"for.pi f28, f28, f22\n"
			"feq.ps f28, f14, f15\n"
			"fpackrepb.pi f1, f4\n"
			"fsatu8.pi f29, f29\n"
			"fnmadd.s f12, f6, f12, f29, rne\n"
			"LBL_SEQID_4_CSR_ID_121_HID_0:\n"
			"csrr x0, hpmcounter8\n"
			"fpackreph.pi f29, f14\n"
			"mul x25, x18, x25\n"
			"flt.s x5, f31, f24\n"
			"fmaxu.pi f6, f30, f28\n"
			"beq x19, x27, 1f\n"
			"1:\n"
			"maskor m1, m1, m0\n"
			"subw x25, x18, x19\n"
			"beq x21, x19, 1f\n"
			"1:\n"
			"LBL_SEQID_4_INT_AMO_ID_122_HID_0:\n"
			"li x23, 0x8225060004\n" 
			"amominug.w x7, x16, (x23)\n" 
			"for.pi f28, f31, f14\n"
			"fcmov.ps f9, f15, f9, f31\n"
			"fpackrepb.pi f31, f9\n"
			"fltu.pi f12, f28, f1\n"
			"fsrli.pi f12, f28, 0xd\n"
			"bge x16, x11, 1f\n"
			"1:\n"
			"fmaxu.pi f16, f4, f29\n"
			"LBL_SEQID_4_INT_STORE_ID_123_HID_0:\n"
			"li x23, 0x810c0824dc\n" 
			"sw x14, 1212(x23)\n" 
			"flt.s x10, f27, f6\n"
			"fcmovm.ps f1, f14, f9\n"
			"fcvt.s.wu f9, x22, rtz\n"
			"srlw x23, x23, x19\n"
			"srliw x23, x23, 4\n"
			"ffrc.ps f29, f28\n"
			"LBL_SEQID_4_INT_STORE_ID_124_HID_0:\n"
			"li x23, 0x8200120025\n" 
			"sd x28, -631(x23)\n" 
			"fmulh.pi f31, f30, f9\n"
			"fsub.ps f15, f11, f16, rmm\n"
			"for.pi f22, f6, f15\n"
			"fsrai.pi f27, f30, 0x1\n"
			"bge x6, x9, 1f\n"
			"1:\n"
			"fsgnjn.s f31, f11, f9\n"
			"fnmsub.s f22, f12, f24, f4, rmm\n"
			"LBL_SEQID_4_FP_AMO_ID_125_HID_0:\n"
			"li x14, 0x8252a42018\n" 
			"la x23, mem_ptr_offset\n"
			"flw.ps f24, 512(x23)\n"
			"famoswapl.pi f6, f24 (x14)\n" 
			"fbci.pi f12, 0x6f5d1\n"
			"bltu x10, x14, 1f\n"
			"1:\n"
			"fsub.s f12, f12, f29, rne\n"
			"fltm.pi m0, f31, f16\n"
			"bgeu x16, x0, 1f\n"
			"1:\n"
			"fmaxu.pi f4, f4, f9\n"
			"mova.x.m x19\n"
			"frsq.ps f31, f11\n"
			"LBL_SEQID_4_FP_AMO_ID_126_HID_0:\n"
			"li x23, 0x813d7e0008\n" 
			"la x10, mem_ptr_offset\n"
			"flw.ps f29, 800(x10)\n"
			"famomaxg.ps f12, f29 (x23)\n" 
			"divw x16, x23, x16\n"
			"fmin.pi f22, f11, f12\n"
			"fmsub.s f12, f12, f1, f11, rdn\n"
			"bne x15, x10, 1f\n"
			"1:\n"
			"sltu x19, x16, x25\n"
			"bgeu x2, x10, 1f\n"
			"1:\n"
			"fmadd.s f27, f22, f11, f11, rne\n"
			"fpackreph.pi f1, f11\n"
			"LBL_SEQID_4_FP_SCATTER_ID_127_HID_0:\n"
			"li x28, 0x820171d037\n" 
			"li x10, 0x6e10ce504680f847\n"
			"fsc32b.ps f24, x10 (x28)\n" 
			"fsgnjn.ps f4, f16, f15\n"
			"fnmsub.s f15, f15, f16, f11, rdn\n"
			"fnmadd.ps f30, f4, f24, f4, rmm\n"
			"fmulh.pi f12, f12, f12\n"
			"fmadd.s f6, f16, f29, f15, rup\n"
			"blt x19, x30, 1f\n"
			"1:\n"
			"fminu.pi f4, f4, f15\n"
			"blt x28, x20, 1f\n"
			"1:\n"
			"LBL_SEQID_4_FP_L_GATHER_ID_128_HID_0:\n"
			"li x22, 0x8145e40688\n" 
			"la x18, mem_ptr_offset\n"
			"flw.ps f11, 68(x18)\n"
			"fscwl.ps f19, f11 (x22)\n"
			"fgbl.ps f6, f11 (x22)\n" 
			"mova.x.m x23\n"
			"fexp.ps f29, f12\n"
			"fltm.pi m2, f9, f11\n"
			"mulh x22, x23, x19\n"
			"slli x18, x18, 1\n"
			"maskand m1, m1, m2\n"
			"LBL_SEQID_4_FP_L_SCATTER_ID_129_HID_0:\n"
			"li x23, 0x81427db34c\n" 
			"la x25, mem_ptr_offset\n"
			"flq2 f29, 44(x25)\n"
			"fschl.ps f16, f29 (x23)\n" 
			"srai x23, x22, 2\n"
			"remw x23, x23, x22\n"
			"fcvt.pw.ps f16, f28, rmm\n"
			"fsat8.pi f24, f28\n"
			"fbci.pi f24, 0x7d130\n"
			"divw x28, x22, x19\n"
			"mov.m.x m0, x0, 0xFF\n"
			"li x5, 0xa0\n"
			"la x31, rand_ieee754_32\n"
			"add x31, x31, x5\n"
			"flw.ps f1, 0(x31)\n"
			"flw.ps f11, 3(x31)\n"
			"flw.ps f14, 8(x31)\n"
			"flw.ps f6, 10(x31)\n"
			"la x31, rand_int_32\n"
			"add x31, x31, x5\n"
			"lw x14, 0(x31)\n"
			"lw x16, 3(x31)\n"
			"lw x23, 5(x31)\n"
			"lw x25, 2(x31)\n"
			"lw x5, 1(x31)\n"
			"li x21, 0x3\n"
			"LBL_SEQID_4_LOOP_SEQ_HID_0:\n"
			"fltm.ps m6, f1, f11\n"
			"fle.s x10, f14, f6\n"
			"rem x14, x5, x25\n"
			"fand.pi f31, f11, f6\n"
			"bne x22, x2, 1f\n"
			"1:\n"
			"auipc x7, 0x12\n"
			"fandi.pi f14, f11, 0x199\n"
			"fcmov.ps f6, f6, f6, f6\n"
			"beq x15, x23, 1f\n"
			"1:\n"
			"fnmsub.ps f14, f14, f14, f11, rmm\n"
			"ffrc.ps f12, f11\n"
			"bge x7, x7, 1f\n"
			"1:\n"
			"frsq.ps f6, f6\n"
			"feqm.ps m6, f11, f1\n"
			"fsgnjn.s f6, f6, f11\n"
			"remu x16, x23, x23\n"
			"fnmsub.ps f28, f14, f11, f14, rtz\n"
			"fsgnjn.ps f27, f11, f14\n"
			"fnmsub.ps f28, f14, f14, f1, rdn\n"
			"bltu x28, x26, 1f\n"
			"1:\n"
			"fltu.pi f27, f6, f11\n"
			"fsrai.pi f24, f14, 0x8\n"
			"add x7, x14, x25\n"
			"fsra.pi f24, f6, f11\n"
			"fltm.ps m6, f6, f11\n"
			"sltu x10, x16, x25\n"
			"slti x14, x14, 0x9\n"
			"fnmsub.ps f28, f11, f11, f14, rne\n"
			"fandi.pi f9, f6, 0xbb\n"
			"remu x14, x25, x5\n"
			"ffrc.ps f31, f1\n"
			"fswizz.ps f11, f11, 0x6\n"
			"bltu x7, x28, 1f\n"
			"1:\n"
			"fsll.pi f9, f6, f1\n"
			"sra x7, x25, x5\n"
			"fmaxu.pi f9, f1, f6\n"
			"fbci.pi f6, 0x1fe9\n"
			"packb x25, x16, x25\n"
			"fsub.ps f15, f14, f11, rmm\n"
			"fclass.ps f1, f1\n"
			"slli x22, x25, 2\n"
			"fnmadd.s f14, f14, f11, f14, rmm\n"
			"fmsub.ps f1, f14, f11, f11, rne\n"
			"fbci.ps f28, 0x50a49\n"
			"fsub.s f31, f11, f6, rmm\n"
			"fcvt.s.wu f31, x25, rne\n"
			"mova.m.x x14\n"
			"fmadd.s f24, f11, f11, f6, rdn\n"
			"bge x8, x12, 1f\n"
			"1:\n"
			"fsll.pi f27, f1, f14\n"
			"fadd.s f15, f6, f6, rup\n"
			"bltu x12, x12, 1f\n"
			"1:\n"
			"fbcx.ps f24, x25\n"
			"fclass.ps f31, f11\n"
			"ebreak\n"
			"fcmov.ps f27, f11, f11, f11\n"
			"feq.s x22, f6, f6\n"
			"fbci.ps f6, 0x31264\n"
			"srai x22, x25, 2\n"
			"maskxor m6, m2, m6\n"
			"fle.ps f6, f6, f11\n"
			"remuw x22, x23, x23\n"
			"ffrc.ps f11, f1\n"
			"sub x5, x16, x5\n"
			"fmul.ps f14, f1, f6, rdn\n"
			"beq x27, x17, 1f\n"
			"1:\n"
			"frsq.ps f31, f11\n"
			"sll x16, x25, x23\n"
			"mova.x.m x7\n"
			"slt x14, x5, x5\n"
			"remu x16, x23, x16\n"
			"bgeu x21, x11, 1f\n"
			"1:\n"
			"addw x5, x16, x5\n"
			"fmulh.pi f1, f1, f11\n"
			"fnmadd.ps f11, f1, f11, f6, rne\n"
			"bge x6, x18, 1f\n"
			"1:\n"
			"fcvt.ps.pw f11, f6, rne\n"
			"maskpopc x23, m0\n"
			"mov.m.x m0, x0, 0xff\n"
			"bge x0, x20, 1f\n"
			"1:\n"
			"fsatu8.pi f12, f11\n"
			"fsin.ps f15, f14\n"
			"fsgnjx.ps f28, f14, f11\n"
			"sub x16, x5, x5\n"
			"xor x7, x25, x14\n"
			"bgeu x5, x29, 1f\n"
			"1:\n"
			"slt x16, x23, x25\n"
			"fsgnjn.s f27, f11, f1\n"
			"fround.ps f1, f6, rdn\n"
			"mov.m.x m1, x25, 0x31\n"
			"slliw x22, x16, 4\n"
			"fmul.s f1, f11, f11, rmm\n"
			"fsgnj.ps f6, f11, f1\n"
			"masknot m1, m1\n"
			"fmsub.s f24, f11, f14, f1, rtz\n"
			"fcvt.f16.ps f1, f6\n"
			"fltm.ps m2, f6, f14\n"
			"andi x10, x25, 0x1b\n"
			"andi x22, x25, 0x1b\n"
			"fle.pi f9, f6, f1\n"
			"beq x22, x28, 1f\n"
			"1:\n"
			"rem x10, x5, x5\n"
			"fsub.ps f24, f11, f1, rtz\n"
			"rem x7, x16, x5\n"
			"mulhu x25, x25, x25\n"
			"blt x9, x5, 1f\n"
			"1:\n"
			"fsgnjn.ps f1, f1, f1\n"
			"ecall\n"
			"fround.ps f11, f1, rdn\n"
			"fmsub.ps f11, f11, f1, f11, rdn\n"
			"fxor.pi f6, f1, f14\n"
			"ori x16, x25, 0x3\n"
			"bne x10, x23, 1f\n"
			"1:\n"
			"remuw x25, x25, x25\n"
			"faddi.pi f14, f1, 0x193\n"
			"bge x30, x26, 1f\n"
			"1:\n"
			"addw x7, x25, x16\n"
			"fmulhu.pi f14, f1, f14\n"
			"or x7, x14, x25\n"
			"fslli.pi f15, f1, 0x6\n"
			"fnmadd.s f24, f6, f6, f11, rne\n"
			"flog.ps f15, f6\n"
			"fand.pi f14, f14, f14\n"
			"fadd.ps f14, f1, f14, rmm\n"
			"and x7, x14, x14\n"
			"fcvt.ps.pw f24, f6, rup\n"
			"sltu x5, x5, x23\n"
			"fence\n"
			"maskxor m1, m0, m6\n"
			"mov.m.x m0, x0, 0xff\n"
			"fsub.s f6, f1, f6, rmm\n"
			"feq.s x7, f11, f6\n"
			"fle.s x16, f1, f6\n"
			"fpackrepb.pi f9, f6\n"
			"slliw x16, x16, 5\n"
			"sllw x7, x5, x16\n"
			"feq.ps f1, f1, f6\n"
			"flem.ps m0, f6, f14\n"
			"fmin.s f1, f14, f14\n"
			"fltu.pi f1, f14, f1\n"
			"fsrli.pi f14, f14, 0x4\n"
			"bne x30, x8, 1f\n"
			"1:\n"
			"flt.s x10, f1, f14\n"
			"divuw x7, x16, x16\n"
			"fltm.pi m6, f14, f1\n"
			"fcmovm.ps f9, f6, f1\n"
			"fmsub.s f28, f14, f11, f1, rne\n"
			"fltm.pi m2, f6, f6\n"
			"fltu.pi f14, f14, f14\n"
			"fmsub.ps f1, f1, f11, f14, rne\n"
			"fmulhu.pi f12, f1, f1\n"
			"bgeu x6, x7, 1f\n"
			"1:\n"
			"packb x16, x25, x23\n"
			"fsrli.pi f9, f6, 0x7\n"
			"for.pi f24, f14, f11\n"
			"mulhu x22, x5, x14\n"
			"ori x5, x5, 0x1b\n"
			"addiw x10, x16, 0x7\n"
			"fsrai.pi f27, f11, 0x4\n"
			"flog.ps f15, f14\n"
			"fsub.pi f1, f6, f1\n"
			"faddi.pi f1, f6, 0x100\n"
			"frsq.ps f11, f6\n"
			"ecall\n"
			"fmulh.pi f11, f14, f1\n"
			"fclass.s x16, f11\n"
			"bne x4, x25, 1f\n"
			"1:\n"
			"fmsub.ps f27, f6, f14, f11, rdn\n"
			"fclass.s x16, f6\n"
			"remuw x16, x25, x25\n"
			"subw x14, x5, x16\n"
			"fclass.ps f15, f11\n"
			"fswizz.ps f15, f11, 0x12\n"
			"fmul.pi f27, f6, f14\n"
			"fround.ps f6, f1, rup\n"
			"maskpopcz x7, m1\n"
			"mov.m.x m6, x16, 0x23\n"
			"bge x25, x1, 1f\n"
			"1:\n"
			"fbcx.ps f27, x25\n"
			"maskxor m2, m2, m2\n"
			"srli x16, x14, 3\n"
			"auipc x7, 0x15\n"
			"fcmovm.ps f11, f11, f1\n"
			"bge x19, x20, 1f\n"
			"1:\n"
			"masknot m6, m1\n"
			"rem x16, x5, x14\n"
			"fmsub.s f1, f1, f11, f14, rtz\n"
			"bltu x22, x29, 1f\n"
			"1:\n"
			"fle.s x14, f6, f14\n"
			"ebreak\n"
			"fcvt.f16.ps f9, f11\n"
			"ebreak\n"
			"srli x23, x5, 4\n"
			"divuw x14, x14, x25\n"
			"mulhu x14, x25, x14\n"
			"bltu x30, x9, 1f\n"
			"1:\n"
			"fltm.pi m2, f11, f11\n"
			"fsgnjn.ps f14, f11, f14\n"
			"sllw x7, x23, x23\n"
			"fsub.s f28, f6, f14, rtz\n"
			"fsub.pi f9, f6, f1\n"
			"bgeu x16, x13, 1f\n"
			"1:\n"
			"faddi.pi f12, f11, 0xb2\n"
			"fltm.pi m6, f14, f6\n"
			"fslli.pi f11, f11, 0xa\n"
			"divuw x22, x23, x25\n"
			"srliw x14, x14, 2\n"
			"sra x16, x16, x14\n"
			"beq x26, x9, 1f\n"
			"1:\n"
			"feq.pi f11, f11, f14\n"
			"beq x26, x6, 1f\n"
			"1:\n"
			"fnot.pi f24, f11\n"
			"fsgnjx.s f27, f6, f11\n"
			"fsat8.pi f1, f6\n"
			"fmin.ps f11, f14, f11\n"
			"fmulhu.pi f6, f11, f1\n"
			"addi x21, x21, -1\n"
			"bne x21, x0, LBL_SEQID_4_LOOP_SEQ_HID_0\n"
			"li x5, 5\n" 
			"bne x30, x5, LBL_FAIL_HID_0\n"
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f23,  928 (x5)\n"
			"flw.ps f4,  480 (x5)\n"
			"flw.ps f17,  352 (x5)\n"
			"flw.ps f5,  32 (x5)\n"
			"flw.ps f27,  768 (x5)\n"
			"flw.ps f21,  512 (x5)\n"
			"flw.ps f6,  0 (x5)\n"
			"flw.ps f29,  256 (x5)\n"
			"flw.ps f28,  864 (x5)\n"
			"flw.ps f22,  640 (x5)\n"
			"flw.ps f7,  192 (x5)\n"
			"flw.ps f3,  416 (x5)\n"
			"flw.ps f9,  128 (x5)\n"
			"flw.ps f15,  672 (x5)\n"
			"flw.ps f10,  608 (x5)\n"
			"flw.ps f24,  448 (x5)\n"
			"flw.ps f19,  992 (x5)\n"
			"flw.ps f25,  800 (x5)\n"
			"flw.ps f30,  736 (x5)\n"
			"flw.ps f26,  576 (x5)\n"
			"flw.ps f12,  896 (x5)\n"
			"flw.ps f1,  288 (x5)\n"
			"flw.ps f0,  320 (x5)\n"
			"flw.ps f18,  224 (x5)\n"
			"flw.ps f14,  96 (x5)\n"
			"flw.ps f2,  704 (x5)\n"
			"flw.ps f13,  384 (x5)\n"
			"flw.ps f20,  544 (x5)\n"
			"flw.ps f16,  160 (x5)\n"
			"flw.ps f31,  64 (x5)\n"
			"flw.ps f8,  832 (x5)\n"
			"flw.ps f11,  960 (x5)\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_5_FP_GATHER_ID_130_HID_0:\n"
			"li x23, 0x820e8b4761\n" 
			"la x16, mem_ptr_offset\n"
			"flq2 f11, 104(x16)\n"
			"fscw.ps f5, f11 (x23)\n"
			"fgh.ps f29, f11 (x23)\n" 
			"srlw x28, x25, x28\n"
			"fcvt.wu.s x16, f6, rne\n"
			"fcvt.pwu.ps f4, f16, rdn\n"
			"fsll.pi f31, f24, f11\n"
			"li x7, 0x85438ff4286bb454\n"
			"fcvt.s.lu f12, x7, rup\n" 
			"fmin.s f28, f30, f15\n"
			"fcvt.ps.f16 f15, f24\n"
			"fsub.ps f27, f30, f9, rtz\n"
			"fmulh.pi f29, f29, f15\n"
			"LBL_SEQID_5_INT_AMO_ID_131_HID_0:\n"
			"li x10, 0x82495406d8\n" 
			"amoswapl.d x22, x25, (x10)\n" 
			"fminu.pi f31, f1, f27\n"
			"blt x8, x17, 1f\n"
			"1:\n"
			"fcvt.s.wu f15, x16, rmm\n"
			"fxor.pi f11, f16, f14\n"
			"bne x22, x2, 1f\n"
			"1:\n"
			"flt.ps f4, f15, f27\n"
			"LBL_SEQID_5_FP_L_SCATTER_ID_132_HID_0:\n"
			"li x7, 0x82495406d8\n" 
			"la x16, mem_ptr_offset\n"
			"flw.ps f31, 56(x16)\n"
			"fscwl.ps f27, f31 (x7)\n" 
			"frsq.ps f12, f15\n"
			"blt x4, x3, 1f\n"
			"1:\n"
			"flog.ps f4, f29\n"
			"fcvt.f16.ps f14, f14\n"
			"blt x19, x15, 1f\n"
			"1:\n"
			"fsetm.pi m2, f29\n"
			"LBL_SEQID_5_FP_STORE_ID_133_HID_0:\n"
			"li x14, 0x810824ee8e\n" 
			"fsw f14, -640(x14)\n" 
			"fsub.ps f15, f15, f24, rdn\n"
			"fmax.pi f29, f30, f9\n"
			"xori x19, x22, 0xf\n"
			"fnmsub.s f1, f24, f28, f16, rdn\n"
			"beq x30, x7, 1f\n"
			"1:\n"
			"LBL_SEQID_5_INT_AMO_ID_134_HID_0:\n"
			"li x16, 0x8135e4068c\n" 
			"amoorg.w x22, x25, (x16)\n" 
			"fmax.pi f30, f15, f27\n"
			"fmadd.s f4, f28, f15, f9, rmm\n"
			"bgeu x23, x17, 1f\n"
			"1:\n"
			"fcvt.ps.pw f24, f11, rdn\n"
			"fmaxu.pi f9, f6, f27\n"
			"bne x22, x2, 1f\n"
			"1:\n"
			"LBL_SEQID_5_INT_AMO_ID_135_HID_0:\n"
			"li x28, 0x825a620310\n" 
			"amoswapl.d x16, x16, (x28)\n" 
			"fnmadd.ps f30, f6, f12, f11, rdn\n"
			"blt x3, x1, 1f\n"
			"1:\n"
			"fle.pi f29, f14, f11\n"
			"fcvt.ps.f16 f27, f6\n"
			"fand.pi f11, f11, f12\n"
			"LBL_SEQID_5_FP_GATHER_ID_136_HID_0:\n"
			"li x18, 0x82074e076a\n" 
			"li x28, 0xaca0982bb173f0e2\n"
			"fsc32w.ps f0, x28 (x18)\n"
			"fg32b.ps f28, x28 (x18)\n" 
			"feq.ps f22, f1, f14\n"
			"fsin.ps f29, f27\n"
			"fmin.s f24, f9, f30\n"
			"fadd.ps f24, f27, f30, rdn\n"
			"LBL_SEQID_5_INT_AMO_ID_137_HID_0:\n"
			"li x19, 0x8225060000\n" 
			"amoorg.d x22, x5, (x19)\n" 
			"or x22, x10, x14\n"
			"fsub.ps f27, f27, f14, rmm\n"
			"fmv.x.s x7, f11\n"
			"fsgnjn.ps f6, f6, f6\n"
			"bge x26, x28, 1f\n"
			"1:\n"
			"LBL_SEQID_5_FP_LOAD_ID_138_HID_0:\n"
			"li x16, 0x8202a42019\n" 
			"fsq2 f22, -630(x16)\n"
			"flq2 f28, -630(x16)\n" 
			"frsq.ps f27, f27\n"
			"fadd.s f11, f15, f27, rne\n"
			"fsat8.pi f30, f30\n"
			"fmin.ps f6, f12, f22\n"
			"LBL_SEQID_5_INT_L_STORE_ID_139_HID_0:\n"
			"li x19, 0x824e8b4760\n" 
			"shl x25, (x19)\n" 
			"fclass.ps f1, f29\n"
			"fcvt.w.s x10, f28, rmm\n"
			"fpackrepb.pi f16, f24\n"
			"blt x30, x31, 1f\n"
			"1:\n"
			"sra x18, x18, x7\n"
			"LBL_SEQID_5_FP_STORE_ID_140_HID_0:\n"
			"li x14, 0x820a620314\n" 
			"fsw.ps f28, -101(x14)\n" 
			"fsrli.pi f24, f9, 0x4\n"
			"fmul.s f27, f16, f12, rtz\n"
			"fcvt.pw.ps f6, f14, rtz\n"
			"fsat8.pi f12, f27\n"
			"LBL_SEQID_5_FP_STORE_ID_141_HID_0:\n"
			"li x5, 0x820e8b4761\n" 
			"fsw f11, -1501(x5)\n" 
			"div x7, x14, x14\n"
			"slliw x14, x18, 1\n"
			"slli x7, x5, 5\n"
			"fnot.pi f24, f28\n"
			"LBL_SEQID_5_FP_AMO_ID_142_HID_0:\n"
			"li x28, 0x8256555000\n" 
			"la x22, mem_ptr_offset\n"
			"flw.ps f12, 320(x22)\n"
			"famoswapl.pi f1, f12 (x28)\n" 
			"fmulhu.pi f4, f24, f15\n"
			"bgeu x13, x26, 1f\n"
			"1:\n"
			"fsgnjn.ps f24, f15, f30\n"
			"bltu x21, x24, 1f\n"
			"1:\n"
			"fmulhu.pi f15, f24, f15\n"
			"fsatu8.pi f24, f24\n"
			"LBL_SEQID_5_INT_AMO_ID_143_HID_0:\n"
			"li x25, 0x813ee69310\n" 
			"amoxorg.w x28, x19, (x25)\n" 
			"sllw x23, x19, x23\n"
			"feq.pi f31, f6, f1\n"
			"sraw x5, x16, x5\n"
			"fmin.s f4, f1, f30\n"
			"LBL_SEQID_5_INT_AMO_ID_144_HID_0:\n"
			"li x14, 0x8132339830\n" 
			"amomaxug.d x25, x18, (x14)\n" 
			"faddi.pi f28, f31, 0xf8\n"
			"ffrc.ps f28, f11\n"
			"fandi.pi f6, f1, 0x82\n"
			"frcp.ps f11, f9\n"
			"la x25, rand_int_32\n"
			"flw.ps f29, 512(x25)\n"
			"flw.ps f31, 896(x25)\n"
			"fdivu.pi f12, f29, f31\n" 
			"fcvt.wu.s x10, f6, rmm\n"
			"bgeu x16, x31, 1f\n"
			"1:\n"
			"fltu.pi f9, f9, f27\n"
			"sllw x19, x7, x14\n"
			"bgeu x15, x18, 1f\n"
			"1:\n"
			"add x18, x5, x28\n"
			"LBL_SEQID_5_INT_AMO_ID_145_HID_0:\n"
			"li x19, 0x81235c0038\n" 
			"amominug.w x25, x22, (x19)\n" 
			"fle.pi f12, f12, f27\n"
			"blt x27, x8, 1f\n"
			"1:\n"
			"fsgnj.s f12, f27, f12\n"
			"feqm.ps m0, f22, f16\n"
			"mov.m.x m0, x0, 0xff\n"
			"bltu x30, x29, 1f\n"
			"1:\n"
			"fnot.pi f12, f11\n"
			"LBL_SEQID_5_INT_AMO_ID_146_HID_0:\n"
			"li x16, 0x82295406d8\n" 
			"amoandg.w x25, x5, (x16)\n" 
			"srliw x22, x19, 3\n"
			"bge x8, x27, 1f\n"
			"1:\n"
			"fsll.pi f22, f16, f28\n"
			"ffrc.ps f16, f16\n"
			"bltu x24, x6, 1f\n"
			"1:\n"
			"fsub.ps f28, f28, f28, rdn\n"
			"LBL_SEQID_5_FP_AMO_ID_147_HID_0:\n"
			"li x5, 0x82574e0768\n" 
			"la x22, mem_ptr_offset\n"
			"flw.ps f24, 512(x22)\n"
			"famomaxl.pi f30, f24 (x5)\n" 
			"fsetm.pi m0, f4\n"
			"mov.m.x m0, x0, 0xff\n"
			"feq.pi f4, f6, f16\n"
			"xor x23, x23, x23\n"
			"fsgnjx.ps f12, f14, f16\n"
			"LBL_SEQID_5_FP_AMO_ID_148_HID_0:\n"
			"li x14, 0x814895f82c\n" 
			"la x23, mem_ptr_offset\n"
			"flw.ps f6, 544(x23)\n"
			"famoxorl.pi f1, f6 (x14)\n" 
			"slt x16, x25, x23\n"
			"beq x25, x11, 1f\n"
			"1:\n"
			"maskpopc x28, m0\n"
			"mov.m.x m0, x0, 0xff\n"
			"beq x29, x14, 1f\n"
			"1:\n"
			"divw x19, x23, x10\n"
			"fmv.x.s x5, f14\n"
			"LBL_SEQID_5_FP_AMO_ID_149_HID_0:\n"
			"li x10, 0x825d5c04ec\n" 
			"la x5, mem_ptr_offset\n"
			"flw.ps f9, 480(x5)\n"
			"famomaxl.ps f29, f9 (x10)\n" 
			"fand.pi f28, f28, f24\n"
			"fmulhu.pi f11, f27, f11\n"
			"fpackreph.pi f28, f24\n"
			"bge x6, x18, 1f\n"
			"1:\n"
			"flog.ps f28, f11\n"
			"LBL_SEQID_5_INT_AMO_ID_150_HID_0:\n"
			"li x18, 0x81250e06b8\n" 
			"amomaxug.w x19, x16, (x18)\n" 
			"fsgnjx.s f9, f9, f29\n"
			"fsgnj.ps f12, f9, f16\n"
			"fnmadd.s f24, f24, f29, f31, rup\n"
			"fexp.ps f16, f4\n"
			"LBL_SEQID_5_INT_LOAD_ID_151_HID_0:\n"
			"li x25, 0x8108160012\n" 
			"sd x16, -1846(x25)\n"
			"ld x22, -1846(x25)\n" 
			"fsgnj.ps f22, f28, f12\n"
			"faddi.pi f11, f30, 0x5d\n"
			"fxor.pi f28, f12, f31\n"
			"fmin.ps f28, f28, f12\n"
			"LBL_SEQID_5_INT_AMO_ID_152_HID_0:\n"
			"li x14, 0x815824ee88\n" 
			"amoaddl.d x19, x23, (x14)\n" 
			"fpackreph.pi f9, f4\n"
			"fmul.pi f24, f24, f29\n"
			"bltu x26, x10, 1f\n"
			"1:\n"
			"fsat8.pi f11, f27\n"
			"fsra.pi f16, f12, f16\n"
			"bltu x30, x12, 1f\n"
			"1:\n"
			"LBL_SEQID_5_FP_G_LOAD_ID_153_HID_0:\n"
			"li x25, 0x822dcf2ea0\n" 
			"mov.m.x m0, zero, 0xff\n"
			"fswg.ps f28, (x25)\n"
			"flwg.ps f12, (x25)\n" 
			"fsgnj.s f1, f31, f24\n"
			"fmaxu.pi f4, f15, f6\n"
			"bgeu x13, x13, 1f\n"
			"1:\n"
			"fmax.ps f24, f6, f22\n"
			"fnmsub.ps f27, f6, f31, f6, rdn\n"
			"beq x5, x30, 1f\n"
			"1:\n"
			"LBL_SEQID_5_INT_AMO_ID_154_HID_0:\n"
			"li x16, 0x822d5c04e8\n" 
			"amoandg.d x18, x23, (x16)\n" 
			"faddi.pi f4, f28, 0x115\n"
			"fadd.pi f15, f24, f16\n"
			"fsgnj.ps f4, f4, f22\n"
			"fmulh.pi f16, f30, f31\n"
			"LBL_SEQID_5_CSR_ID_155_HID_0:\n"
			"li x10, 0x8000008205de5802\n"
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw unlock_va, x10\n" 
			"csrwi tensor_wait, 6\n" 
			"fmulh.pi f11, f9, f16\n"
			"feq.ps f9, f30, f30\n"
			"fsgnj.ps f14, f24, f14\n"
			"fminu.pi f1, f22, f12\n"
			"LBL_SEQID_5_FP_AMO_ID_156_HID_0:\n"
			"li x16, 0x82495406d8\n" 
			"la x23, mem_ptr_offset\n"
			"flw.ps f14, 576(x23)\n"
			"famomaxl.ps f31, f14 (x16)\n" 
			"fadd.s f16, f16, f11, rtz\n"
			"fsrai.pi f28, f4, 0xb\n"
			"fcmovm.ps f9, f4, f14\n"
			"fmaxu.pi f16, f4, f16\n"
			"bne x10, x12, 1f\n"
			"1:\n"
			"LBL_SEQID_5_FP_G_LOAD_ID_157_HID_0:\n"
			"li x16, 0x8236920000\n" 
			"mov.m.x m0, zero, 0xff\n"
			"fswg.ps f31, (x16)\n"
			"flwg.ps f28, (x16)\n" 
			"fnot.pi f6, f14\n"
			"fmax.ps f30, f9, f31\n"
			"fexp.ps f6, f28\n"
			"fsatu8.pi f14, f12\n"
			"LBL_SEQID_5_FP_L_GATHER_ID_158_HID_0:\n"
			"li x18, 0x8149e20020\n" 
			"la x16, mem_ptr_offset\n"
			"flq2 f27, 220(x16)\n"
			"fscwl.ps f28, f27 (x18)\n"
			"fgbl.ps f1, f27 (x18)\n" 
			"fsrl.pi f30, f15, f27\n"
			"ffrc.ps f12, f16\n"
			"fsrl.pi f22, f24, f27\n"
			"fpackrepb.pi f9, f27\n"
			"LBL_SEQID_5_INT_AMO_ID_159_HID_0:\n"
			"li x14, 0x81250e06b8\n" 
			"amoorg.d x5, x28, (x14)\n" 
			"fslli.pi f31, f29, 0xd\n"
			"bltu x1, x11, 1f\n"
			"1:\n"
			"fmax.pi f14, f12, f22\n"
			"fmin.ps f15, f12, f1\n"
			"fcvt.pwu.ps f27, f27, rdn\n"
			"mov.m.x m0, x0, 0xFF\n"
			"li x5, 0x60\n"
			"la x31, rand_ieee754_32\n"
			"add x31, x31, x5\n"
			"flw.ps f12, 0(x31)\n"
			"flw.ps f16, 3(x31)\n"
			"flw.ps f24, 0(x31)\n"
			"flw.ps f28, 10(x31)\n"
			"flw.ps f31, 14(x31)\n"
			"la x31, rand_int_32\n"
			"add x31, x31, x5\n"
			"lw x10, 0(x31)\n"
			"lw x16, 4(x31)\n"
			"lw x18, 5(x31)\n"
			"lw x22, 0(x31)\n"
			"lw x23, 15(x31)\n"
			"lw x25, 9(x31)\n"
			"lw x5, 0(x31)\n"
			"li x21, 0x2\n"
			"LBL_SEQID_5_LOOP_SEQ_HID_0:\n"
			"fround.ps f4, f24, rtz\n"
			"fsgnj.ps f4, f31, f16\n"
			"bltu x3, x13, 1f\n"
			"1:\n"
			"ffrc.ps f16, f31\n"
			"mova.m.x x16\n"
			"srl x23, x23, x18\n"
			"srai x16, x18, 3\n"
			"fexp.ps f4, f12\n"
			"fround.ps f28, f28, rdn\n"
			"fnot.pi f4, f31\n"
			"add x16, x16, x5\n"
			"fslli.pi f12, f12, 0x6\n"
			"fmulhu.pi f16, f12, f31\n"
			"addw x10, x5, x18\n"
			"fsra.pi f16, f16, f16\n"
			"flem.ps m0, f31, f31\n"
			"mov.m.x m0, x0, 0xff\n"
			"bge x21, x18, 1f\n"
			"1:\n"
			"fmin.pi f16, f24, f16\n"
			"add x19, x25, x10\n"
			"bne x27, x24, 1f\n"
			"1:\n"
			"fmulh.pi f16, f16, f16\n"
			"bne x27, x25, 1f\n"
			"1:\n"
			"fpackreph.pi f4, f28\n"
			"divu x10, x10, x23\n"
			"fnmsub.s f6, f31, f12, f31, rup\n"
			"fmul.s f16, f24, f31, rmm\n"
			"fmul.pi f16, f12, f31\n"
			"fmulhu.pi f4, f16, f16\n"
			"fminu.pi f6, f28, f31\n"
			"remw x10, x23, x25\n"
			"mulhu x16, x25, x22\n"
			"fmv.s.x f28, x5\n"
			"blt x17, x5, 1f\n"
			"1:\n"
			"fmin.pi f28, f12, f28\n"
			"fmul.ps f4, f24, f24, rne\n"
			"fclass.s x18, f31\n"
			"fbci.pi f4, 0x273af\n"
			"mova.m.x x22\n"
			"fltm.ps m0, f12, f24\n"
			"fcvt.f16.ps f4, f16\n"
			"masknot m2, m0\n"
			"beq x18, x22, 1f\n"
			"1:\n"
			"fclass.s x22, f16\n"
			"fcvt.s.w f24, x25, rmm\n"
			"fsetm.pi m0, f31\n"
			"fandi.pi f6, f28, 0xa1\n"
			"fsgnjx.ps f12, f12, f12\n"
			"fclass.s x10, f31\n"
			"sra x10, x10, x22\n"
			"bge x29, x12, 1f\n"
			"1:\n"
			"fsub.ps f4, f16, f16, rup\n"
			"fmax.s f31, f31, f31\n"
			"feq.pi f12, f28, f12\n"
			"sraiw x10, x5, 1\n"
			"fclass.ps f4, f28\n"
			"remu x19, x10, x5\n"
			"sub x16, x10, x18\n"
			"bge x17, x3, 1f\n"
			"1:\n"
			"for.pi f12, f12, f12\n"
			"bge x4, x8, 1f\n"
			"1:\n"
			"ebreak\n"
			"fmadd.ps f16, f16, f16, f16, rmm\n"
			"fslli.pi f6, f24, 0x1\n"
			"remuw x16, x25, x23\n"
			"masknot m1, m0\n"
			"beq x30, x17, 1f\n"
			"1:\n"
			"mulhu x16, x16, x18\n"
			"fnmadd.ps f12, f28, f12, f24, rmm\n"
			"sltiu x14, x16, 0x10\n"
			"bne x31, x2, 1f\n"
			"1:\n"
			"fcvt.w.s x18, f16, rne\n"
			"masknot m0, m1\n"
			"mov.m.x m0, x0, 0xff\n"
			"bltu x10, x18, 1f\n"
			"1:\n"
			"fclass.s x23, f12\n"
			"sraw x10, x10, x10\n"
			"flt.pi f28, f12, f28\n"
			"fsgnjx.ps f28, f24, f28\n"
			"fpackrepb.pi f4, f16\n"
			"fltu.pi f16, f24, f16\n"
			"fsub.ps f4, f24, f12, rtz\n"
			"ffrc.ps f16, f12\n"
			"maskor m2, m1, m0\n"
			"fmvz.x.ps x10, f31, 4\n"
			"beq x31, x5, 1f\n"
			"1:\n"
			"masknot m1, m1\n"
			"sra x22, x22, x16\n"
			"bne x8, x7, 1f\n"
			"1:\n"
			"fle.s x10, f16, f31\n"
			"bne x13, x21, 1f\n"
			"1:\n"
			"fcvt.pw.ps f12, f31, rdn\n"
			"fltm.ps m1, f24, f28\n"
			"fsub.ps f12, f12, f16, rup\n"
			"frsq.ps f4, f24\n"
			"fbcx.ps f16, x16\n"
			"fence\n"
			"fxor.pi f16, f12, f24\n"
			"fsin.ps f6, f31\n"
			"fsub.pi f24, f31, f28\n"
			"fcvt.wu.s x16, f12, rne\n"
			"xor x10, x16, x25\n"
			"bge x7, x20, 1f\n"
			"1:\n"
			"fmaxu.pi f6, f12, f16\n"
			"fmul.s f24, f12, f24, rup\n"
			"bltu x14, x3, 1f\n"
			"1:\n"
			"maskand m0, m0, m2\n"
			"mov.m.x m0, x0, 0xff\n"
			"beq x4, x26, 1f\n"
			"1:\n"
			"addw x22, x18, x22\n"
			"maskpopc x19, m0\n"
			"mov.m.x m0, x0, 0xff\n"
			"fcmovm.ps f6, f12, f24\n"
			"fand.pi f16, f16, f28\n"
			"fbci.ps f4, 0x5fb90\n"
			"sra x22, x10, x22\n"
			"fadd.s f6, f16, f28, rtz\n"
			"feqm.ps m0, f31, f28\n"
			"mov.m.x m0, x0, 0xff\n"
			"fsgnj.s f6, f28, f16\n"
			"for.pi f16, f12, f12\n"
			"sllw x14, x5, x16\n"
			"fnot.pi f24, f16\n"
			"fslli.pi f6, f12, 0xd\n"
			"fminu.pi f31, f31, f28\n"
			"auipc x14, 0x6\n"
			"flt.s x25, f31, f16\n"
			"fsatu8.pi f16, f31\n"
			"fnmadd.ps f6, f28, f24, f28, rup\n"
			"fcvt.f16.ps f4, f16\n"
			"fsub.s f6, f16, f24, rne\n"
			"fcvt.wu.s x25, f24, rup\n"
			"lui x19, 0x1\n"
			"flt.ps f6, f24, f24\n"
			"bne x23, x1, 1f\n"
			"1:\n"
			"div x5, x5, x18\n"
			"blt x22, x24, 1f\n"
			"1:\n"
			"sub x23, x23, x5\n"
			"fmulh.pi f6, f31, f16\n"
			"fswizz.ps f28, f12, 0x10\n"
			"maskpopcz x19, m2\n"
			"fcvt.pwu.ps f16, f16, rmm\n"
			"fcvt.wu.s x16, f28, rdn\n"
			"slti x14, x25, 0xf\n"
			"fcmov.ps f6, f28, f12, f12\n"
			"xor x14, x10, x23\n"
			"bge x22, x4, 1f\n"
			"1:\n"
			"fcvt.ps.f16 f12, f12\n"
			"addiw x10, x10, 0x17\n"
			"fmadd.ps f16, f16, f31, f24, rmm\n"
			"srlw x16, x23, x16\n"
			"fmul.pi f4, f16, f31\n"
			"fclass.s x10, f16\n"
			"fsin.ps f16, f28\n"
			"mul x25, x23, x25\n"
			"sllw x16, x22, x10\n"
			"slli x18, x18, 3\n"
			"fmadd.s f24, f16, f24, f24, rdn\n"
			"fmsub.s f24, f31, f24, f31, rne\n"
			"fsgnj.ps f6, f28, f31\n"
			"fadd.pi f28, f28, f31\n"
			"bne x30, x16, 1f\n"
			"1:\n"
			"fnmsub.s f12, f12, f24, f28, rdn\n"
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
			"LBL_HPM_3_CORE_TXFMA_OPS16_FAIL_HID_0:\n"          
			"csrr x5, mhpmcounter3\n"
			"sd x5, 32 (x31)\n"
			"LBL_HPM_4_CORE_DCACHE_MISSES0_FAIL_HID_0:\n"          
			"csrr x5, mhpmcounter4\n"
			"sd x5, 40 (x31)\n"
			"LBL_HPM_5_CORE_TL_INST_FAIL_HID_0:\n"          
			"csrr x5, mhpmcounter5\n"
			"sd x5, 48 (x31)\n"
			"LBL_HPM_6_CORE_TS_OPS_FAIL_HID_0:\n"          
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
			"LBL_HPM_3_CORE_TXFMA_OPS16_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter3\n"
			"sd x5, 32 (x31)\n"
			"LBL_HPM_4_CORE_DCACHE_MISSES0_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter4\n"
			"sd x5, 40 (x31)\n"
			"LBL_HPM_5_CORE_TL_INST_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter5\n"
			"sd x5, 48 (x31)\n"
			"LBL_HPM_6_CORE_TS_OPS_PASS_HID_0:\n"          
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
		0xae832651, 
		0xe165f375, 
		0x737eda56, 
		0x99cec94b, 
		0x05b80b3a, 
		0x490fd235, 
		0x8adfc9ea, 
		0x4315d044, 
		0x04f61b0c, 
		0xf7ab250f, 
		0x8d970f47, 
		0xc2b0dae1, 
		0xc8a2457d, 
		0xd7976393, 
		0x1b081513, 
		0x0f5e8297, 
		0x38165506, 
		0x103df8e4, 
		0x0b074d67, 
		0x6145bb03, 
		0xa2dedbf8, 
		0xe926619a, 
		0x17202807, 
		0xdf92aff8, 
		0xa1954168, 
		0x89392979, 
		0xf94481bd, 
		0x97bd4779, 
		0xce5cbdc5, 
		0xb32e251a, 
		0x424dd9ec, 
		0x4e2e70f6, 
		0x57ff557b, 
		0x49d9fc72, 
		0xe51e5f53, 
		0x26d45e16, 
		0x4de084c4, 
		0xd83486fb, 
		0x559bc60a, 
		0x8a08df56, 
		0xab0cdffe, 
		0x60ae3095, 
		0x04b7fd0b, 
		0x9e98bdf4, 
		0xcc831a1d, 
		0xc1d21dcd, 
		0x64be89ce, 
		0x4bd8883c, 
		0xc22c6328, 
		0xb9c6e239, 
		0x22cfba61, 
		0xb64b53fa, 
		0x06e9d9e7, 
		0x499d6e79, 
		0xc45f668f, 
		0x97cc61c6, 
		0xbfff1bab, 
		0x3abd1239, 
		0xb97f288b, 
		0x1b0d4bbe, 
		0xfa422533, 
		0xe1a0ccb9, 
		0x89d91e2d, 
		0xb0f206e7, 
		0x82386838, 
		0x60773e45, 
		0x1c6a2855, 
		0xfb190e2e, 
		0x761afcd7, 
		0x2a60fe2a, 
		0xf525b972, 
		0xcb5676d3, 
		0xa35d0b3c, 
		0x932f54a1, 
		0x3a188360, 
		0xf23f3fc7, 
		0x49ff025f, 
		0xc0d1fb6b, 
		0xe46eb3bb, 
		0x17ecb841, 
		0xa76b45de, 
		0xf2f33263, 
		0x02212b23, 
		0x3742dfac, 
		0xe38056f1, 
		0x3d4f50fa, 
		0xf9986b90, 
		0x44276701, 
		0xb36d08c2, 
		0x9e1b3b87, 
		0x82997d9b, 
		0x4298c0d8, 
		0x4f32dd74, 
		0xd8551312, 
		0x1256848b, 
		0x892e0eb9, 
		0xe9b9f3f7, 
		0x3afef82b, 
		0x0440ebe9, 
		0xa2185f7a, 
		0xa16ca302, 
		0xcc3d1745, 
		0x3b85f0c1, 
		0x7abadc36, 
		0xc966192a, 
		0xc4351dc1, 
		0xf68bc358, 
		0x645222a8, 
		0x23b20f08, 
		0x730292f4, 
		0x02a5b726, 
		0xee9ef20a, 
		0x03d1da07, 
		0x527fd092, 
		0xb60f133c, 
		0x5fbfed89, 
		0xf33cbf57, 
		0x334fd03a, 
		0xad3e8d7e, 
		0x120d4704, 
		0x10d43477, 
		0x9eb6d03c, 
		0xab5f7d5c, 
		0xa2aa0e99, 
		0x42b977f5, 
		0x896d9e4e, 
		0x91161c30, 
		0x875ece5d, 
		0x4a949c3a, 
		0xaeda2d26, 
		0x1448708e, 
		0x79f2048d, 
		0x20b2a0b8, 
		0x60b12dd9, 
		0xf22a3295, 
		0xc5126694, 
		0xdb4c0c59, 
		0x0f8e5482, 
		0xf07c8c92, 
		0x17162904, 
		0x7aa84cd3, 
		0x5686f713, 
		0x231aaa29, 
		0x05ee8d32, 
		0xda8591f2, 
		0x9e578124, 
		0x42a7f70f, 
		0x99b55f3f, 
		0x4732ce22, 
		0x98bc3111, 
		0x522d3e7d, 
		0xf484c2f4, 
		0xf8008185, 
		0xff761f96, 
		0xa6604759, 
		0x94c9ccc4, 
		0x152c0283, 
		0xcd3c0980, 
		0x1d3eb718, 
		0x0c28543c, 
		0x8743450b, 
		0xd987e1e4, 
		0x0aba26f7, 
		0x07c95550, 
		0x84a8e6e5, 
		0x463258a3, 
		0xf27a7289, 
		0xff803188, 
		0x88c44302, 
		0x49a99c57, 
		0x06cd7e46, 
		0xb53a07bb, 
		0x0cb523af, 
		0x6dc4c31e, 
		0x157852fb, 
		0x74d79f8d, 
		0x631500f1, 
		0x8728ca33, 
		0x95e24e3f, 
		0x6e4345a8, 
		0x9fca5880, 
		0x041dcbbe, 
		0x4cf8b997, 
		0x1a80a8e6, 
		0x009101dc, 
		0x4006c8fb, 
		0x788d325a, 
		0xf02222fc, 
		0x0b07e5c8, 
		0x4317356d, 
		0x58929656, 
		0x9c8d0837, 
		0x41288634, 
		0xc05324ff, 
		0xa702dd0d, 
		0xc8d21113, 
		0x811b60a8, 
		0xd8a1ac13, 
		0xcbe479b5, 
		0xfc6c61ac, 
		0x050b9c7f, 
		0x925faaaf, 
		0xea16c4a7, 
		0xe32f8b87, 
		0x227a5de7, 
		0x7a486907, 
		0x357f11c8, 
		0x4d4e879e, 
		0xbe2bd821, 
		0x50d8e511, 
		0xe5b70846, 
		0xf31b3a6b, 
		0x14cf346c, 
		0x9587eb42, 
		0xf0bb305d, 
		0xd77cadfc, 
		0x8e77f388, 
		0x95d9d497, 
		0xbde8d6b8, 
		0x28f0aea5, 
		0xd1ed35c3, 
		0x5380ccc8, 
		0x6147cf61, 
		0x2307cdf4, 
		0x2a161c91, 
		0x88ae936d, 
		0x1b33c67a, 
		0x8683d5f4, 
		0xcdda4739, 
		0x70601f7c, 
		0xcce60d87, 
		0x76f70c9b, 
		0xccd3d453, 
		0x68fe64cd, 
		0x14d0a3b1, 
		0x4e1c84a3, 
		0x6996d253, 
		0x178e0ef9, 
		0x8567e8d9, 
		0x3f4e3aca, 
		0x18570363, 
		0xa952a50a, 
		0x6414a5f9, 
		0xdc24c135, 
		0xc82fedda, 
		0xe2ab49e3, 
		0x46cb067f, 
		0xaa73ce16, 
		0x763885c0, 
		0x6356b21b, 
		0x96c45eb4, 
		0xf25af70f, 
		0x03256854, 
		0xe53182ee, 
		0xe1a423d3, 
		0x2907613b  
};
volatile uint32_t rand_ieee754_32[1536] __attribute__ ((aligned (2048))) = {
		0xc1700000, 
		0x7f7ffffe, 
		0xc1b00000, 
		0x80000020, 
		0x749f6e29, 
		0x003388e8, 
		0x42780000, 
		0x7fffffff, 
		0x6dd2737c, 
		0x7fccaa79, 
		0x7ffa634b, 
		0xaed8524e, 
		0xf68e951e, 
		0x00400000, 
		0x0e00007f, 
		0xab265248, 
		0xc1e00000, 
		0xb3d98eaf, 
		0xc25c0000, 
		0x00800003, 
		0x2745b7fc, 
		0xb6c9868f, 
		0xff800000, 
		0xc1100000, 
		0x80000200, 
		0x0c7f8000, 
		0x0e00000f, 
		0xff800000, 
		0x80000001, 
		0x00000000, 
		0xc1400000, 
		0xff800000, 
		0x7f800000, 
		0x0c600000, 
		0x219a71e0, 
		0x7f800000, 
		0x8ff42c27, 
		0x7fc00000, 
		0x80004000, 
		0x0f7fffff, 
		0x42680000, 
		0x80000000, 
		0x0c780000, 
		0x65b78059, 
		0x0f7ffffc, 
		0x7f800000, 
		0xb5189158, 
		0x42280000, 
		0x272976d1, 
		0x8f7fffff, 
		0xff7ffffe, 
		0x0e0003ff, 
		0xa415b000, 
		0x807db1a3, 
		0x7f7ffffe, 
		0x41300000, 
		0x6196c454, 
		0xb76c1c62, 
		0x42400000, 
		0xc0e00000, 
		0xc1300000, 
		0xc1300000, 
		0x00000040, 
		0x80800003, 
		0x0f7ffffc, 
		0xf88fbaa2, 
		0xc2100000, 
		0x80800003, 
		0x007fffff, 
		0x00008000, 
		0xd9e200d0, 
		0xcda2a9aa, 
		0x00000020, 
		0xead4e820, 
		0x314c23e4, 
		0x200bffe2, 
		0xc2700000, 
		0xffa08e9e, 
		0xcb000000, 
		0xa861af49, 
		0x0e001fff, 
		0x41880000, 
		0x33333333, 
		0x41d80000, 
		0x80000000, 
		0xe97cb782, 
		0x7f800000, 
		0x366faf8d, 
		0x80000040, 
		0x7fccc25d, 
		0x00400000, 
		0x7f800000, 
		0xd0473a6d, 
		0x3f800001, 
		0x0c7ff000, 
		0xc2640000, 
		0x0e007fff, 
		0x80040000, 
		0x87d5daab, 
		0x42400000, 
		0x06e120f2, 
		0x80000800, 
		0xc1200000, 
		0x7f7ffffe, 
		0xf7820622, 
		0xeb097cdb, 
		0x00000001, 
		0x80000000, 
		0x41000000, 
		0x4feffc82, 
		0x41200000, 
		0x4de47b27, 
		0x80000000, 
		0x7f800000, 
		0xebb1738f, 
		0xf5dc6c45, 
		0x00ffffff, 
		0x42640000, 
		0x112eccbd, 
		0x41d00000, 
		0xc2480000, 
		0x7fbfffff, 
		0x40a00000, 
		0x41ea12a6, 
		0xfea0e3b4, 
		0x00800003, 
		0x89da156d, 
		0xe35eae51, 
		0x0e0fffff, 
		0x0e000001, 
		0x47adb973, 
		0x7f7ffffe, 
		0x7fc00001, 
		0xc1700000, 
		0x1dcfadac, 
		0x0c7fe000, 
		0x80000000, 
		0x3f028f5c, 
		0x00000000, 
		0xa2b822a1, 
		0x7fbe5350, 
		0x700dd87a, 
		0xde7eda69, 
		0x2a84397e, 
		0x41b00000, 
		0x80000100, 
		0x42780000, 
		0x19e3ddfd, 
		0xff800001, 
		0xc23c0000, 
		0x648ee70b, 
		0x0e0000ff, 
		0x3f800000, 
		0x4f70af2c, 
		0x5a546b73, 
		0x41e00000, 
		0x8c381031, 
		0x88e9958a, 
		0x42680000, 
		0xd5612ae7, 
		0x0e003fff, 
		0x42700000, 
		0x0e01ffff, 
		0x1bfb5dd9, 
		0x7f9971c4, 
		0x6eb44c91, 
		0xff7fffff, 
		0x00000000, 
		0x870801fe, 
		0x41c00000, 
		0xbf800000, 
		0x0ab8cf02, 
		0x7f800000, 
		0x6201b825, 
		0x57363a76, 
		0x42180000, 
		0x80000000, 
		0x20ac91a4, 
		0x40000000, 
		0x80000000, 
		0x41880000, 
		0x42340000, 
		0x0e000003, 
		0x94d3da72, 
		0x00002000, 
		0x0c700000, 
		0x80000000, 
		0xbf8521a8, 
		0x80001000, 
		0x63e4648a, 
		0xc0e00000, 
		0xe779a782, 
		0x80004000, 
		0x8f7ffffd, 
		0x00000200, 
		0xd1c40394, 
		0xff7ffffe, 
		0x42580000, 
		0x986746f8, 
		0xb09a3fc0, 
		0xea0efa96, 
		0x071b0774, 
		0x7f7ffffe, 
		0x34e94ba5, 
		0x00000001, 
		0x80000000, 
		0x21809c08, 
		0x80000080, 
		0x7fc00000, 
		0xffc00000, 
		0xe41c7873, 
		0x3f7f5e6a, 
		0x41f00000, 
		0x80800003, 
		0x7fc00000, 
		0x42000000, 
		0x42080000, 
		0x80000000, 
		0xc1e7d59c, 
		0x9f150b48, 
		0x41200000, 
		0x7c284d77, 
		0x00000000, 
		0x41c00000, 
		0x75cc84ab, 
		0xc1d00000, 
		0xc1000000, 
		0x63ffb807, 
		0x4063f9c3, 
		0x00000020, 
		0xc23c0000, 
		0xff7ffffe, 
		0x426c0000, 
		0x41c80000, 
		0x41500000, 
		0x3ca74507, 
		0x80ffffff, 
		0xb1546368, 
		0x0c7f0000, 
		0x0c7fff00, 
		0xc22c0000, 
		0x2b31bfa7, 
		0xc0c00000, 
		0xff800000, 
		0xcf2543db, 
		0x70bf8940, 
		0xdca3c717, 
		0x62dbaa8e, 
		0xb096a232, 
		0x59728081, 
		0x843eebf3, 
		0xc21c0000, 
		0x41100000, 
		0x00000000, 
		0x80000000, 
		0x424c0000, 
		0x42600000, 
		0xf029cc2e, 
		0x9e9601d5, 
		0xc1000000, 
		0xe15eb795, 
		0x6c89b82a, 
		0xb7b40e07, 
		0x72470c12, 
		0x0e00ffff, 
		0xff800001, 
		0x80000020, 
		0x421c0000, 
		0x416811e5, 
		0x8818ba40, 
		0x7f800000, 
		0x1ab7d160, 
		0xa31faabe, 
		0x80800003, 
		0x16c0d9c5, 
		0x8d553ba4, 
		0x3646e471, 
		0x41300000, 
		0x0c7f8000, 
		0x41c00000, 
		0xc2480000, 
		0xc2500000, 
		0xa41783f9, 
		0x62140603, 
		0xc1980000, 
		0x41d80000, 
		0x0caf26ac, 
		0x00800002, 
		0xc2500000, 
		0x80800002, 
		0xc0c00000, 
		0x807fffff, 
		0x82c1130d, 
		0x7f800000, 
		0x7f800000, 
		0x7f97963f, 
		0x0e00001f, 
		0xc22c0000, 
		0x0e00ffff, 
		0x40a00000, 
		0xaaaaaaaa, 
		0xc1000000, 
		0xa33b54c9, 
		0xc1900000, 
		0xf108ffb9, 
		0xc1400000, 
		0xc1d80000, 
		0x422c0000, 
		0xc23c0000, 
		0x41f80000, 
		0xc0a00000, 
		0x80000000, 
		0x41900000, 
		0x0f7ffffe, 
		0x7fbb1601, 
		0x807fffff, 
		0x80000100, 
		0x00008000, 
		0x1fd8f4c0, 
		0x00400000, 
		0xcccccccc, 
		0xc1a00000, 
		0x41980000, 
		0x41600000, 
		0x31e80385, 
		0x80001000, 
		0x0c7ffffc, 
		0xc25c0000, 
		0x4e4ee7e1, 
		0x42240000, 
		0x80000800, 
		0xc15a5a3b, 
		0x80000002, 
		0xff800001, 
		0x6a9cdf39, 
		0xc2700000, 
		0xff800000, 
		0xc83b7c5b, 
		0xf31fa7f5, 
		0xca40086a, 
		0x00100000, 
		0x80000400, 
		0xb5deb0de, 
		0xc2640000, 
		0x61caeb1b, 
		0x00000800, 
		0xe8d8099a, 
		0x80080000, 
		0x2e686f96, 
		0x00400000, 
		0x41e80000, 
		0x5ce22598, 
		0x3812d2b9, 
		0x00800002, 
		0xc2300000, 
		0x0c7ffff8, 
		0x80000000, 
		0xc2640000, 
		0x42780000, 
		0xc1c80000, 
		0x807ffffe, 
		0x4398f115, 
		0x00ffffff, 
		0xc2300000, 
		0xc88ac071, 
		0x7fbfffff, 
		0xc1200000, 
		0x41980000, 
		0x0c7ff800, 
		0xefff95b4, 
		0xc1c00000, 
		0x41a80000, 
		0x7fc00000, 
		0xc2600000, 
		0x427c0000, 
		0x00800001, 
		0x7fadbc5d, 
		0xc0a00000, 
		0x7fd3bc32, 
		0x038e8ce7, 
		0xc2140000, 
		0x1f39b93d, 
		0x7f800000, 
		0xc2500000, 
		0x841bd854, 
		0xc1d00000, 
		0x00000000, 
		0x40800000, 
		0xd906b293, 
		0x0c7ffc00, 
		0x26acf7e3, 
		0x42280000, 
		0x41c00000, 
		0xc1800000, 
		0x0c7fff00, 
		0x80000200, 
		0x0c7fffff, 
		0xc974cbde, 
		0x42240000, 
		0x41500000, 
		0x80040000, 
		0xc1e00000, 
		0x7fffffff, 
		0x42680000, 
		0x7b2a3118, 
		0x0e007fff, 
		0x0c7fffc0, 
		0x89830cf5, 
		0x0c7f0000, 
		0x7f800000, 
		0x41d80000, 
		0xc1a80000, 
		0x7f7fffff, 
		0xffd71a2f, 
		0x28747a2a, 
		0x41a80000, 
		0x6d5279a8, 
		0x80000002, 
		0x80000002, 
		0x42540000, 
		0x80000000, 
		0x42080000, 
		0x62957dfc, 
		0x7fc00001, 
		0x00000000, 
		0x80800000, 
		0xdcfdc2ac, 
		0xf397beec, 
		0x7fc00001, 
		0xe61fbd40, 
		0xd8c457d1, 
		0x40400000, 
		0x4c276936, 
		0x0d000ff0, 
		0xc0c00000, 
		0x00800001, 
		0x983c2d5b, 
		0x80800000, 
		0xffc00001, 
		0x00800000, 
		0x41c80000, 
		0xc0800000, 
		0xc1880000, 
		0x0079bc35, 
		0xa9f2d934, 
		0x03a26db8, 
		0xff800000, 
		0x41980000, 
		0x80800001, 
		0xff000000, 
		0x029744a7, 
		0x80000000, 
		0x00100000, 
		0x34dc2e10, 
		0xc2080000, 
		0x0c7ffc00, 
		0x41f00000, 
		0x80000000, 
		0x0d00fff0, 
		0x00002000, 
		0x00000010, 
		0x00000080, 
		0xc2180000, 
		0x0c7ff800, 
		0xff7ffffe, 
		0x80000000, 
		0x00080000, 
		0x0c7ffe00, 
		0xb21d0a59, 
		0x00800002, 
		0x057a57ec, 
		0xa0250b55, 
		0xc2700000, 
		0x42000000, 
		0xc26c0000, 
		0x7f800000, 
		0x80000100, 
		0x41100000, 
		0xec4383ea, 
		0x00800002, 
		0x14f675c9, 
		0x04defdbe, 
		0x00800001, 
		0x80000001, 
		0xafc1dd89, 
		0x00011111, 
		0x0e0000ff, 
		0x80004000, 
		0x41100000, 
		0x727d5fde, 
		0x3f028f5c, 
		0x41980000, 
		0x00800001, 
		0xc0a00000, 
		0x6f7c7b31, 
		0x80800003, 
		0x4db61bfe, 
		0x4cad4834, 
		0x4d59d6bd, 
		0xc2740000, 
		0x62a87fb4, 
		0x38348f96, 
		0x00000020, 
		0xc1a80000, 
		0x40c00000, 
		0x40400000, 
		0x42740000, 
		0x00800003, 
		0x0c7ffffc, 
		0x7f800000, 
		0x71bbe6f6, 
		0xff000000, 
		0x78f64d87, 
		0xff800000, 
		0xb5ee78a1, 
		0xc24c0000, 
		0x80010000, 
		0x48093201, 
		0x7d63f8f3, 
		0xc1800000, 
		0x0c7ff800, 
		0x6200cb24, 
		0x42300000, 
		0x0c7f0000, 
		0xc0000000, 
		0x526f79d2, 
		0x8217121e, 
		0xc1900000, 
		0x0c7f0000, 
		0x00327b8a, 
		0xc1980000, 
		0xd932476a, 
		0x007cb7e1, 
		0x80010000, 
		0x7f800000, 
		0xcb000000, 
		0x7f000000, 
		0x020eaad1, 
		0xc2000000, 
		0x80000004, 
		0x0c7fc000, 
		0x80367c8b, 
		0x425c0000, 
		0x42280000, 
		0x00800003, 
		0x80000000, 
		0x80000010, 
		0x0c7fff00, 
		0x3aade10d, 
		0x7f7ffffe, 
		0x00000000, 
		0x00080000, 
		0x80000000, 
		0xbf59ede7, 
		0x2b91e750, 
		0xd570d08e, 
		0x42080000, 
		0x40e00000, 
		0xc0000000, 
		0x0d2219f1, 
		0xcb1baead, 
		0x00800002, 
		0x5588bc04, 
		0x0470157e, 
		0x426c0000, 
		0xe71e9793, 
		0xc0400000, 
		0x0f7ffffe, 
		0x7f52784e, 
		0x00800002, 
		0x0e00003f, 
		0x0c7ffc00, 
		0xc2500000, 
		0x40c00000, 
		0x41b80000, 
		0xffc00000, 
		0x7fc00001, 
		0x42340000, 
		0x0e001fff, 
		0x0c7fffe0, 
		0x0e0000ff, 
		0x423c0000, 
		0xe4280de2, 
		0x00000010, 
		0x2bf9b46a, 
		0x0f7ffffc, 
		0xc2580000, 
		0xc1e80000, 
		0x9311fe0f, 
		0x42000000, 
		0x8f7fffff, 
		0x0e000fff, 
		0xac95873a, 
		0xc2240000, 
		0x80000001, 
		0x80000200, 
		0x00000000, 
		0x42600000, 
		0xeeb9007c, 
		0x051c517f, 
		0x40a00000, 
		0xc9734ded, 
		0xc6233763, 
		0xeaf52b31, 
		0x80080000, 
		0x52fde1f9, 
		0xbf028f5c, 
		0xc63e4e20, 
		0x41d00000, 
		0xf5ecb0cf, 
		0x420c0000, 
		0x00800000, 
		0x961486d3, 
		0x42780000, 
		0xc1400000, 
		0x42380000, 
		0x0c7fff80, 
		0x41980000, 
		0x74535fd9, 
		0x9d7763f1, 
		0x80800002, 
		0x29ad26b5, 
		0x07280bd0, 
		0x80000080, 
		0x00800001, 
		0xc1d00000, 
		0x7f8dbfff, 
		0xf69207a1, 
		0x80040000, 
		0xc26c0000, 
		0xffffffff, 
		0x426c0000, 
		0x00000000, 
		0x0c7ffe00, 
		0x0e03ffff, 
		0x42300000, 
		0xc1880000, 
		0xffc00001, 
		0x7f800000, 
		0x0e01ffff, 
		0x40400000, 
		0x0c7fffff, 
		0x42280000, 
		0x80000000, 
		0xc2500000, 
		0x42280000, 
		0xc27c0000, 
		0x0e000007, 
		0xff9d71b6, 
		0x210e5935, 
		0x7d8b811c, 
		0x42440000, 
		0x42180000, 
		0x3874812d, 
		0xc1f00000, 
		0x00800001, 
		0x0e000fff, 
		0x00000001, 
		0xbf951ffd, 
		0x89549107, 
		0x0e000007, 
		0x8f7ffffc, 
		0xec3c2d56, 
		0x426b906b, 
		0x7fc00001, 
		0x00000001, 
		0xb9d93453, 
		0x04d304da, 
		0x251ea3be, 
		0xff800000, 
		0xc1880000, 
		0x7fc00001, 
		0xb5bd62a1, 
		0xaf77c2a7, 
		0x38839da3, 
		0xbdb75fcf, 
		0xffa66829, 
		0x00000100, 
		0x0e000fff, 
		0x00800001, 
		0x7f800000, 
		0x80000000, 
		0x0c7fff00, 
		0x00010000, 
		0x00000000, 
		0x00000800, 
		0x41500000, 
		0x86ada1ad, 
		0x42040000, 
		0x80000010, 
		0x80000002, 
		0x80008000, 
		0xff800000, 
		0x00008000, 
		0xc1400000, 
		0x80000800, 
		0x422c0000, 
		0xc1f80000, 
		0x0c7ffff8, 
		0xc1200000, 
		0x80800001, 
		0xf7205bac, 
		0x7f800000, 
		0xb7cd6ac1, 
		0x5c846c4d, 
		0x1a98fd8d, 
		0x00011111, 
		0x774d77bd, 
		0xd410cc2c, 
		0x19cbc96d, 
		0x0e07ffff, 
		0x01e4b18a, 
		0x00347922, 
		0xc1c80000, 
		0xbc8d1adb, 
		0x7f878d32, 
		0xc0c00000, 
		0x0c7fff00, 
		0x835dd8f0, 
		0x80000000, 
		0x0374d84f, 
		0x41a00000, 
		0x00800001, 
		0x0f7ffffc, 
		0xffc00000, 
		0x1e2f50fc, 
		0xc24c0000, 
		0x80000000, 
		0x2e0d75c9, 
		0x7ff6a4ef, 
		0x788c4505, 
		0x7f800000, 
		0x42640000, 
		0x007ffffe, 
		0x42680000, 
		0x00000000, 
		0xff800000, 
		0x80800001, 
		0x41e00000, 
		0x0e0a1916, 
		0x7fc00001, 
		0xc1a00000, 
		0x42480000, 
		0x999cc20e, 
		0xfb8a898d, 
		0x0c700000, 
		0x0c7ffc00, 
		0xcccccccc, 
		0xff7ffffe, 
		0x805f25dd, 
		0xcc7a24b7, 
		0x7f7fffff, 
		0x00000000, 
		0x13f1f1e9, 
		0xba392b31, 
		0xc26c0000, 
		0x9c936565, 
		0x42080000, 
		0x40c00000, 
		0xd52825d5, 
		0xd17a75d4, 
		0x80000000, 
		0x7f7fffff, 
		0xc1e00000, 
		0x7f800000, 
		0xffbc1ce5, 
		0x1f604e7f, 
		0x42740000, 
		0x80000000, 
		0x0c7ff000, 
		0x061f2a28, 
		0x0e0fffff, 
		0x41300000, 
		0xdc342405, 
		0xffefa1da, 
		0x134c9192, 
		0x00000000, 
		0x5e7b42a1, 
		0x00008000, 
		0xc1600000, 
		0x00100000, 
		0xc1700000, 
		0xfc25582f, 
		0x7220e65c, 
		0xc0c00000, 
		0xc2640000, 
		0x00ffffff, 
		0x0f7fffff, 
		0x00010000, 
		0x80800000, 
		0x9b78fbdf, 
		0x823e7bd4, 
		0x80000000, 
		0xc2000000, 
		0x00010000, 
		0x5482d0ec, 
		0x00200000, 
		0x00000000, 
		0xc1a00000, 
		0xc2065e63, 
		0x7ff1e6dc, 
		0x697e29ed, 
		0x41300000, 
		0x75e44dfe, 
		0xffaeab73, 
		0x7fb4eff4, 
		0x41fbb5f7, 
		0xc1a00000, 
		0x80800003, 
		0x4d5e9da3, 
		0x00000000, 
		0x7fc00000, 
		0x80800001, 
		0x80000000, 
		0x19623df4, 
		0x8f7ffffe, 
		0x571cf25d, 
		0x00080000, 
		0xc26c0000, 
		0x80000100, 
		0x00000000, 
		0xc2740000, 
		0x0c7ffc00, 
		0x41b00000, 
		0x425c0000, 
		0x88f4cd5b, 
		0x41200000, 
		0x00000100, 
		0x41300000, 
		0xc26c0000, 
		0x7f7fffff, 
		0xe865e2c6, 
		0x7fffffff, 
		0x0e000007, 
		0xff800000, 
		0x466d83af, 
		0xe4394da9, 
		0x80000000, 
		0x00000000, 
		0x61146a9e, 
		0xff800001, 
		0x30d453ae, 
		0x0c7fffff, 
		0x80000020, 
		0xff000000, 
		0x00002000, 
		0x41980000, 
		0x41500000, 
		0x0e3fffff, 
		0x61facf55, 
		0x226b5643, 
		0x3d963830, 
		0xc24c0000, 
		0xc0c00000, 
		0xb3699eae, 
		0x40800000, 
		0x00000000, 
		0x82956d32, 
		0x00000010, 
		0xd93138df, 
		0x0e0003ff, 
		0x66196f66, 
		0x41d80000, 
		0x5e12ea9f, 
		0x7f7ffffe, 
		0x689f6119, 
		0xffc00000, 
		0x00002000, 
		0x80000020, 
		0xb2db7f42, 
		0xc2080000, 
		0xc0e00000, 
		0xbf800001, 
		0x80080000, 
		0x7f7ffffe, 
		0x0b7e3439, 
		0x607f07fe, 
		0xc2740000, 
		0x7f7ffffe, 
		0x00800000, 
		0xff7ffffe, 
		0x80000100, 
		0xc20c0000, 
		0xc0000000, 
		0xfca8cda7, 
		0x41a00000, 
		0x0e000007, 
		0x36210333, 
		0x00000000, 
		0x7f7ffffe, 
		0x1344af6f, 
		0x41000000, 
		0x00000020, 
		0x1bf710a7, 
		0x00004000, 
		0xc0a00000, 
		0xffbfffff, 
		0x50b581c2, 
		0xe0742525, 
		0x7f83f224, 
		0x7fc00001, 
		0x2a334332, 
		0xc0800000, 
		0x00000000, 
		0x0c7ffe00, 
		0x0c7ffffc, 
		0x8f7ffffc, 
		0x00ffffff, 
		0x41e80000, 
		0x00000100, 
		0x8f7ffffc, 
		0xfb1a6a0b, 
		0x0c7ffc00, 
		0x42340000, 
		0x0e00000f, 
		0x431fa0ab, 
		0x80000020, 
		0x80000200, 
		0x7f7ffffe, 
		0x3250f605, 
		0x17c2278b, 
		0x40400000, 
		0x40a00000, 
		0x42340000, 
		0x80000000, 
		0x5f5614ef, 
		0x80020000, 
		0x00800000, 
		0x80000000, 
		0x02f04fa0, 
		0x80800001, 
		0x480a370e, 
		0xaf76b2be, 
		0x0e00ffff, 
		0x34be8753, 
		0xc1980000, 
		0x3057aecc, 
		0x42300000, 
		0x7fa48c3b, 
		0xff903cd3, 
		0x00800003, 
		0x8f7ffffd, 
		0xa2060def, 
		0x42100000, 
		0x426c0000, 
		0xef112260, 
		0x6c156ec2, 
		0x675368bf, 
		0x97de901e, 
		0xafd91b3b, 
		0x00000020, 
		0xc2480000, 
		0x00800001, 
		0x0e00001f, 
		0xc1200000, 
		0x00000000, 
		0x41600000, 
		0xc25c0000, 
		0x80000000, 
		0xc26c0000, 
		0x00000002, 
		0x00800001, 
		0x817fe102, 
		0x7f800001, 
		0x41500000, 
		0x0c7fff80, 
		0xff800001, 
		0x0c7fffff, 
		0x4be8ab9e, 
		0x9405935f, 
		0x7f800000, 
		0x42500000, 
		0xaaaaaaaa, 
		0xf778fd17, 
		0x8f3b9097, 
		0x79487292, 
		0x42100000, 
		0x00800002, 
		0x41700000, 
		0x25884fd0, 
		0x7fbfffff, 
		0x41980000, 
		0xa3e309b4, 
		0x7f800000, 
		0x00002000, 
		0x2ee12864, 
		0x32d7e19d, 
		0xc1d80000, 
		0x00000000, 
		0xff800000, 
		0x145ed00d, 
		0xc38191a3, 
		0x00008000, 
		0x42140000, 
		0x80ffffff, 
		0x00000002, 
		0x00000100, 
		0x00000020, 
		0x804234f7, 
		0x80000020, 
		0x00000800, 
		0xff800000, 
		0x00000080, 
		0x65f2d5ef, 
		0x098b7cbf, 
		0xc2580000, 
		0x228a3261, 
		0x42640000, 
		0x80000001, 
		0x277aa05c, 
		0x80000800, 
		0xecb9bd80, 
		0x846a8dc3, 
		0x7f800000, 
		0x80000001, 
		0x72f78344, 
		0x844b64c3, 
		0x80008000, 
		0x41600000, 
		0xedd08510, 
		0x854c1d3d, 
		0xfe18ee47, 
		0xff7ffffe, 
		0x59a82cd6, 
		0x0e000fff, 
		0xc2100000, 
		0xc1d00000, 
		0x41a80000, 
		0x0c600000, 
		0x42780000, 
		0x00800001, 
		0x806a1eea, 
		0x0d00fff0, 
		0x6528a9a5, 
		0x7f800000, 
		0x80800000, 
		0x0e03ffff, 
		0x80000000, 
		0xc2200000, 
		0x41a80000, 
		0x00400000, 
		0x80000020, 
		0x00000000, 
		0x0c780000, 
		0x00800002, 
		0xc1400000, 
		0x49c7cf62, 
		0x0c7fff80, 
		0x80020000, 
		0xc26c0000, 
		0x00000001, 
		0x420c0000, 
		0x41c00000, 
		0x42140000, 
		0x5640f08a, 
		0x0c7fffff, 
		0x80000080, 
		0x0c7ffffc, 
		0xffc00000, 
		0x0c7c0000, 
		0x00000000, 
		0xc1b00000, 
		0xff000000, 
		0x4b000000, 
		0x80000000, 
		0x7fd8f8e7, 
		0x42780000, 
		0x11b4955b, 
		0x0d00fff0, 
		0xa8c5846e, 
		0xff800000, 
		0x80000000, 
		0x40000000, 
		0x0c7f8000, 
		0x00000000, 
		0x42040000, 
		0x41e00000, 
		0x80800002, 
		0x00800002, 
		0x807fffff, 
		0x00000008, 
		0x80000000, 
		0x00000000, 
		0x00000004, 
		0x80001000, 
		0xff7ffffe, 
		0xff800000, 
		0x11d4a1ac, 
		0x2658610d, 
		0x80100000, 
		0x0e000001, 
		0xc1980000, 
		0xc2280000, 
		0x41900000, 
		0xdda50990, 
		0x32370eca, 
		0x7fc00000, 
		0x00020000, 
		0x00000200, 
		0x0e1fffff, 
		0x0c7ff800, 
		0x00000000, 
		0x42340000, 
		0x007fffff, 
		0xb60cce04, 
		0xbc7bba8f, 
		0x2a6b5c14, 
		0xcb000000, 
		0x42640000, 
		0xafbe1aed, 
		0x42080000, 
		0x00000000, 
		0x80000004, 
		0x00000000, 
		0xff8d2b5e, 
		0x41c00000, 
		0x08c5aeea, 
		0x8f7ffffd, 
		0xc0800000, 
		0x00000400, 
		0x01161eff, 
		0x40400000, 
		0x00008000, 
		0x80008000, 
		0x4229d197, 
		0x42040000, 
		0x80000000, 
		0x42440000, 
		0x80000000, 
		0xc1880000, 
		0x7b0c90e8, 
		0x66f6ec41, 
		0x42500000, 
		0x80000000, 
		0xf5785fe6, 
		0x0448a447, 
		0xaaaaaaaa, 
		0x41f00000, 
		0x00000000, 
		0x7f800001, 
		0x27671642, 
		0xa8d173a5, 
		0x00000001, 
		0x00000000, 
		0x00000000, 
		0xe520547c, 
		0x4a66992d, 
		0x00000000, 
		0x0c7ff000, 
		0x80040000, 
		0x7f800001, 
		0x42500000, 
		0x80200000, 
		0x2d30bfdf, 
		0xf516e47a, 
		0x0c7e0000, 
		0x0d00fff0, 
		0x0e0003ff, 
		0x41a80000, 
		0x0e0001ff, 
		0x80800001, 
		0x22d0babc, 
		0x80000000, 
		0x00040000, 
		0x42180000, 
		0xff800000, 
		0x803a4f97, 
		0x00000000, 
		0x40a00000, 
		0xc2700000, 
		0x533eb866, 
		0x8116efe2, 
		0x8077a6b8, 
		0xc0e00000, 
		0x4502fbcc, 
		0x80000000, 
		0x0e01ffff, 
		0x0c7e0000, 
		0xc26c0000, 
		0xc2280000, 
		0x41e00000, 
		0x80122c82, 
		0x2d99a2ee, 
		0x42380000, 
		0xfc7ba848, 
		0x41000000, 
		0x00000004, 
		0x80000001, 
		0x0e00000f, 
		0x0c7ffffc, 
		0x80000002, 
		0x80008000, 
		0xcccccccc, 
		0x007ffffe, 
		0x6e3729a6, 
		0x41f5f732, 
		0x0e07ffff, 
		0xc9965b4d, 
		0x00800000, 
		0x420c0000, 
		0xc1b80000, 
		0x42500000, 
		0xc2b27d8b, 
		0xe0d3ab28, 
		0x86a25b90, 
		0xc1600000, 
		0xc2040000, 
		0xe26a2b8c, 
		0x42440000, 
		0xffc00000, 
		0xbf028f5c, 
		0x6e9c20c9, 
		0xc1f80000, 
		0xc950cf8c, 
		0xc1a00000, 
		0xc0000000, 
		0x5f1d8d12, 
		0x0e0001ff, 
		0x00000080, 
		0x00000008, 
		0xc2440000, 
		0x9b393c9a, 
		0xbee1ac11, 
		0xffbfffff, 
		0xcb000000, 
		0xd392f10c, 
		0x80000000, 
		0xc131251b, 
		0xc1700000, 
		0x1cbc66ce, 
		0xff7ffffe, 
		0x2dfe4463, 
		0x0c7fff00, 
		0x0e0fffff, 
		0x0e003fff, 
		0x98fb7efd, 
		0x0f7ffffc, 
		0x00800000, 
		0x7fc00000, 
		0x4af2ba55, 
		0x40400000, 
		0x80000020, 
		0x97146b2f, 
		0x40000000, 
		0x0c7e0000, 
		0xc1c00000, 
		0x42400000, 
		0xc2100000, 
		0x41100000, 
		0x41c80000, 
		0x35f71545, 
		0x00000020, 
		0xc2500000, 
		0x80000000, 
		0xaed334c4, 
		0x80800001, 
		0x00000001, 
		0x423c0000, 
		0x00000040, 
		0x0c7f8000, 
		0x0c400000, 
		0xb065ad2a, 
		0x1814a648, 
		0xfc11398e, 
		0x41c00000, 
		0x807aa6b3, 
		0xff7ffffe, 
		0xff7fffff, 
		0xd7a7bf81, 
		0x80040000, 
		0x0c7fffe0, 
		0x7f800000, 
		0x41200000, 
		0x41d00000, 
		0x2d22bb9f, 
		0xe6faf7aa, 
		0x8052c363, 
		0x00000040, 
		0x00000000, 
		0x80000000, 
		0xc25c0000, 
		0x24a29fda, 
		0x00000080, 
		0xb9fd6b45, 
		0x6061af0c, 
		0x7f800000, 
		0x1bcf61e3, 
		0x80000001, 
		0x88d2dd5c, 
		0xe2f283b8, 
		0x0e00000f, 
		0x80000000, 
		0x02d587c3, 
		0x3f7b6864, 
		0x0c7ff800, 
		0x40c00000, 
		0xab72bc8d, 
		0x0e000003, 
		0xf0b4e19c, 
		0x0e000fff, 
		0x00800000, 
		0x311c00b3, 
		0x80000000, 
		0x7fc00000, 
		0x00000100, 
		0x7f7ffffe, 
		0x421c0000, 
		0x42640000, 
		0xc2600000, 
		0x42440000, 
		0x42680000, 
		0xff800000, 
		0x7f800000, 
		0x7f800001, 
		0x803028fe, 
		0xc1800000, 
		0x120cb69c, 
		0x40883034, 
		0xde96f877, 
		0x40e00000, 
		0x80000001, 
		0x0c7fffc0, 
		0x6ce3b621, 
		0x41300000, 
		0x35a97319, 
		0x565c8801, 
		0xa5977e59, 
		0x41500000, 
		0x7f800000, 
		0xff800000, 
		0x7d50ee5f, 
		0xb5a7cecb, 
		0x1955fefa, 
		0xc2440000, 
		0x00002000, 
		0x80000000, 
		0x00000000, 
		0xbf800001, 
		0x41a80000, 
		0x0af332e8, 
		0x00000000, 
		0x00000000, 
		0xf89aeef0, 
		0xbbf24e73, 
		0x00800001, 
		0x13212c3e, 
		0x80000000, 
		0x80000000, 
		0x80000002, 
		0x42640000, 
		0x7f7fffff, 
		0xc0400000, 
		0xc2680000, 
		0x80000004, 
		0x00000400, 
		0xff800000, 
		0x0e000003, 
		0x000d0154, 
		0xc26c0000, 
		0x8cc38153, 
		0x00800002, 
		0x427c0000, 
		0x9b082b20, 
		0x80011111, 
		0x80000000, 
		0xff800000, 
		0x40800000, 
		0x42340000, 
		0x55555555, 
		0x339f7782, 
		0x80000000, 
		0x80000000, 
		0xc0c00000, 
		0x80800003, 
		0x31230476, 
		0xff800000, 
		0x282aaf08, 
		0x11a80aa8, 
		0x55555555, 
		0x0e001fff, 
		0x0c7f0000, 
		0x40c00000, 
		0xff800000, 
		0x79f818e3, 
		0xc20c0000, 
		0xc2000000, 
		0xe8a7976a, 
		0x41d80000, 
		0x7f85948f, 
		0x002976c9, 
		0x00800000, 
		0x902737e1, 
		0x7f800000, 
		0x40a00000, 
		0x99be5000, 
		0x0b76207f, 
		0x0c7fff00, 
		0x00000000, 
		0xf29629df, 
		0x80000000, 
		0x41a00000, 
		0xaf5c1786, 
		0x80000040, 
		0x040a35f3, 
		0x48905bde, 
		0x2c840a7f, 
		0x7f800000, 
		0x12ed0f6c, 
		0xff7fffff, 
		0x0e00003f, 
		0x7f800000, 
		0x42640000, 
		0xa39b16f3, 
		0x80000000, 
		0x03c95a6c, 
		0x80001000, 
		0xc21c0000, 
		0x7f7fffff, 
		0x00000000, 
		0x00000004, 
		0xf30914a5, 
		0x0c7f8000, 
		0x2189381e, 
		0x80000000, 
		0x0c400000, 
		0x807ffffe, 
		0x6d4c11f6, 
		0xff7ffffe, 
		0x0e00ffff, 
		0x0055a36f, 
		0x80800002, 
		0x42180000, 
		0x00010000, 
		0x80004000, 
		0x911c36e6, 
		0x423c0000, 
		0x7f7fffff, 
		0x3c3ca9fd, 
		0x80000000, 
		0x42280000, 
		0xc2280000, 
		0x426c0000, 
		0xffbfffff, 
		0x7f800000, 
		0xa9ec6b14, 
		0x40c00000, 
		0x00011111, 
		0x7fc00001, 
		0xffb869e5, 
		0x7572bc1d, 
		0x0d00fff0, 
		0xc1700000, 
		0x510e7585, 
		0xc1100000, 
		0x0e00007f, 
		0xc2600000, 
		0x8eb545a9, 
		0x0e07ffff, 
		0x509e1933, 
		0x463305c2, 
		0x80040000, 
		0x00800000, 
		0x80100000, 
		0xc2100000, 
		0x6521a46a, 
		0x00002000, 
		0xcd0c9f12, 
		0x0e003fff, 
		0x00800003, 
		0xc1c80000, 
		0x7f800000, 
		0x0c7ff000, 
		0x427c0000, 
		0x80800001, 
		0xc1b80000, 
		0x007fffff, 
		0x5e3dbd35, 
		0x42780000, 
		0xcc67f847, 
		0xcb8c4b40, 
		0x4b000000, 
		0xddac49dd, 
		0xc2240000, 
		0xc0800000, 
		0x05743496, 
		0x0c600000, 
		0x42140000, 
		0x00400000, 
		0x80800001, 
		0x530caed0, 
		0x0c7ffffe, 
		0x42140000, 
		0x426c0000, 
		0xa85bfc41, 
		0x20b731bb, 
		0x7f800000, 
		0x80000000, 
		0x423c0000, 
		0x00800000, 
		0xffffffff, 
		0xefdd89c7, 
		0xc2100000, 
		0xc0d7c0c4, 
		0x3b3daf2f, 
		0xc2740000, 
		0x3f028f5c, 
		0xc2400000, 
		0xf89fe81f, 
		0xff7ffffe, 
		0x0e001fff, 
		0xc2600000, 
		0x0e000003, 
		0x7fb64f85, 
		0x2a506241, 
		0xb791d69a, 
		0x00040000, 
		0x42740000, 
		0xaa7f2df4, 
		0xb57052ea, 
		0xda4ea38a, 
		0x42240000, 
		0x0c7c0000, 
		0x41b00000, 
		0x00040000, 
		0x7f800000, 
		0x42200000, 
		0x75057a0e, 
		0x00000800, 
		0x425c0000, 
		0xeb2f14ca, 
		0x7f7ffffe, 
		0x8f7ffffd, 
		0x00000040, 
		0x41980000, 
		0x735dec50, 
		0xc1980000  
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

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
			"csrwi mhpmevent4, 22\n"   
			"csrwi mhpmevent5, 14\n"   
			"csrwi mhpmevent6, 27\n"   
			"csrwi mhpmevent7, 6\n"   
			"csrwi mhpmevent8, 3\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f2,  192 (x5)\n"
			"flw.ps f9,  0 (x5)\n"
			"flw.ps f10,  480 (x5)\n"
			"flw.ps f24,  416 (x5)\n"
			"flw.ps f13,  448 (x5)\n"
			"flw.ps f21,  928 (x5)\n"
			"flw.ps f20,  64 (x5)\n"
			"flw.ps f1,  320 (x5)\n"
			"flw.ps f28,  832 (x5)\n"
			"flw.ps f27,  544 (x5)\n"
			"flw.ps f31,  960 (x5)\n"
			"flw.ps f14,  160 (x5)\n"
			"flw.ps f7,  736 (x5)\n"
			"flw.ps f5,  32 (x5)\n"
			"flw.ps f6,  576 (x5)\n"
			"flw.ps f29,  256 (x5)\n"
			"flw.ps f4,  864 (x5)\n"
			"flw.ps f30,  512 (x5)\n"
			"flw.ps f12,  96 (x5)\n"
			"flw.ps f11,  128 (x5)\n"
			"flw.ps f8,  384 (x5)\n"
			"flw.ps f17,  800 (x5)\n"
			"flw.ps f19,  224 (x5)\n"
			"flw.ps f18,  992 (x5)\n"
			"flw.ps f15,  704 (x5)\n"
			"flw.ps f23,  672 (x5)\n"
			"flw.ps f22,  640 (x5)\n"
			"flw.ps f26,  288 (x5)\n"
			"flw.ps f0,  608 (x5)\n"
			"flw.ps f16,  768 (x5)\n"
			"flw.ps f25,  352 (x5)\n"
			"flw.ps f3,  896 (x5)\n"
			"LBL_VADDR_INIT_HID_0:\n"          
			"li x22, 0x81020ec820 \n"                 
			"fsw.ps f5, (x22) \n"                 
			"li x25, 0x8102707700 \n"                 
			"fsw.ps f30, (x25) \n"                 
			"li x23, 0x8104860020 \n"                 
			"fsw.ps f27, (x23) \n"                 
			"li x14, 0x8104ac0020 \n"                 
			"fsw.ps f18, (x14) \n"                 
			"li x13, 0x8104c80ea0 \n"                 
			"fsw.ps f0, (x13) \n"                 
			"li x21, 0x81072204c0 \n"                 
			"fsw.ps f0, (x21) \n"                 
			"li x12, 0x8107ce0240 \n"                 
			"fsw.ps f11, (x12) \n"                 
			"li x12, 0x810a208000 \n"                 
			"fsw.ps f19, (x12) \n"                 
			"li x10, 0x810a4c0020 \n"                 
			"fsw.ps f4, (x10) \n"                 
			"li x10, 0x810a640020 \n"                 
			"fsw.ps f18, (x10) \n"                 
			"li x17, 0x810ab00500 \n"                 
			"fsw.ps f16, (x17) \n"                 
			"li x15, 0x810b0f4800 \n"                 
			"fsw.ps f14, (x15) \n"                 
			"li x11, 0x810c76d800 \n"                 
			"fsw.ps f6, (x11) \n"                 
			"li x14, 0x810da60680 \n"                 
			"fsw.ps f0, (x14) \n"                 
			"li x19, 0x810e0c0240 \n"                 
			"fsw.ps f24, (x19) \n"                 
			"li x24, 0x810ff5b020 \n"                 
			"fsw.ps f4, (x24) \n"                 
			"li x22, 0x82009c0040 \n"                 
			"fsw.ps f29, (x22) \n"                 
			"li x22, 0x82031dda60 \n"                 
			"fsw.ps f10, (x22) \n"                 
			"li x23, 0x82031f0020 \n"                 
			"fsw.ps f22, (x23) \n"                 
			"li x22, 0x8205c56800 \n"                 
			"fsw.ps f4, (x22) \n"                 
			"li x23, 0x82070e64e0 \n"                 
			"fsw.ps f12, (x23) \n"                 
			"li x21, 0x8207300020 \n"                 
			"fsw.ps f24, (x21) \n"                 
			"li x12, 0x820ab48820 \n"                 
			"fsw.ps f5, (x12) \n"                 
			"li x21, 0x820b080000 \n"                 
			"fsw.ps f28, (x21) \n"                 
			"li x15, 0x820e50e620 \n"                 
			"fsw.ps f11, (x15) \n"                 
			"li x24, 0x820e94a800 \n"                 
			"fsw.ps f23, (x24) \n"                 
			"li x25, 0x820f900000 \n"                 
			"fsw.ps f22, (x25) \n"                 
			"fence\n"
			"LBL_GLOBAL_VADDR_INIT_HID_0:\n"          
			"li x12, 0x8122707700 \n"                 
			"fswg.ps f1, (x12) \n"                 
			"li x11, 0x8123800620 \n"                 
			"fswg.ps f29, (x11) \n"                 
			"li x13, 0x8124ac0020 \n"                 
			"fswg.ps f27, (x13) \n"                 
			"li x19, 0x812a208000 \n"                 
			"fswg.ps f13, (x19) \n"                 
			"li x15, 0x812a4c0020 \n"                 
			"fswg.ps f15, (x15) \n"                 
			"li x10, 0x812ab00500 \n"                 
			"fswg.ps f11, (x10) \n"                 
			"li x12, 0x812b920020 \n"                 
			"fswg.ps f7, (x12) \n"                 
			"li x12, 0x812da60680 \n"                 
			"fswg.ps f31, (x12) \n"                 
			"li x10, 0x8134860020 \n"                 
			"fswg.ps f13, (x10) \n"                 
			"li x22, 0x81390aa020 \n"                 
			"fswg.ps f12, (x22) \n"                 
			"li x15, 0x813a640020 \n"                 
			"fswg.ps f4, (x15) \n"                 
			"li x19, 0x813c76d800 \n"                 
			"fswg.ps f30, (x19) \n"                 
			"li x10, 0x82226c0700 \n"                 
			"fswg.ps f20, (x10) \n"                 
			"li x17, 0x82231dda60 \n"                 
			"fswg.ps f14, (x17) \n"                 
			"li x15, 0x82262003e0 \n"                 
			"fswg.ps f28, (x15) \n"                 
			"li x16, 0x8227300020 \n"                 
			"fswg.ps f10, (x16) \n"                 
			"li x12, 0x822b080000 \n"                 
			"fswg.ps f18, (x12) \n"                 
			"li x21, 0x822f900000 \n"                 
			"fswg.ps f26, (x21) \n"                 
			"li x25, 0x82309c0040 \n"                 
			"fswg.ps f3, (x25) \n"                 
			"li x19, 0x82331f0020 \n"                 
			"fswg.ps f11, (x19) \n"                 
			"li x11, 0x8235c56800 \n"                 
			"fswg.ps f29, (x11) \n"                 
			"li x22, 0x82370e64e0 \n"                 
			"fswg.ps f26, (x22) \n"                 
			"li x12, 0x82379a02c0 \n"                 
			"fswg.ps f15, (x12) \n"                 
			"li x23, 0x82388ba2c0 \n"                 
			"fswg.ps f10, (x23) \n"                 
			"li x16, 0x823de80a40 \n"                 
			"fswg.ps f1, (x16) \n"                 
			"li x13, 0x823e50e620 \n"                 
			"fswg.ps f0, (x13) \n"                 
			"li x13, 0x823e94a800 \n"                 
			"fswg.ps f26, (x13) \n"                 
			"fence\n"
			"LBL_LOCAL_VADDR_INIT_HID_0:\n"          
			"li x12, 0x81403e0020 \n"                 
			"fswl.ps f30, (x12) \n"                 
			"li x20, 0x81420ec820 \n"                 
			"fswl.ps f8, (x20) \n"                 
			"li x21, 0x8144c80ea0 \n"                 
			"fswl.ps f10, (x21) \n"                 
			"li x12, 0x81472204c0 \n"                 
			"fswl.ps f25, (x12) \n"                 
			"li x16, 0x814a4c0020 \n"                 
			"fswl.ps f10, (x16) \n"                 
			"li x10, 0x814a640020 \n"                 
			"fswl.ps f21, (x10) \n"                 
			"li x10, 0x814ab00500 \n"                 
			"fswl.ps f7, (x10) \n"                 
			"li x25, 0x814b0f4800 \n"                 
			"fswl.ps f4, (x25) \n"                 
			"li x16, 0x814c76d800 \n"                 
			"fswl.ps f12, (x16) \n"                 
			"li x12, 0x814e0c0240 \n"                 
			"fswl.ps f23, (x12) \n"                 
			"li x14, 0x8154860020 \n"                 
			"fswl.ps f6, (x14) \n"                 
			"li x14, 0x8154ac0020 \n"                 
			"fswl.ps f6, (x14) \n"                 
			"li x16, 0x815a208000 \n"                 
			"fswl.ps f17, (x16) \n"                 
			"li x11, 0x815e5cd3e0 \n"                 
			"fswl.ps f4, (x11) \n"                 
			"li x22, 0x815ff5b020 \n"                 
			"fswl.ps f31, (x22) \n"                 
			"li x20, 0x82479a02c0 \n"                 
			"fswl.ps f26, (x20) \n"                 
			"li x14, 0x8248bb9520 \n"                 
			"fswl.ps f26, (x14) \n"                 
			"li x23, 0x824b080000 \n"                 
			"fswl.ps f17, (x23) \n"                 
			"li x24, 0x824e94a800 \n"                 
			"fswl.ps f2, (x24) \n"                 
			"li x21, 0x8252f40020 \n"                 
			"fswl.ps f1, (x21) \n"                 
			"li x23, 0x82531f0020 \n"                 
			"fswl.ps f1, (x23) \n"                 
			"li x15, 0x8255c56800 \n"                 
			"fswl.ps f23, (x15) \n"                 
			"li x11, 0x82570e64e0 \n"                 
			"fswl.ps f29, (x11) \n"                 
			"li x12, 0x825ab48820 \n"                 
			"fswl.ps f5, (x12) \n"                 
			"li x17, 0x825de80a40 \n"                 
			"fswl.ps f18, (x17) \n"                 
			"li x15, 0x825e50e620 \n"                 
			"fswl.ps f14, (x15) \n"                 
			"li x21, 0x825f900000 \n"                 
			"fswl.ps f8, (x21) \n"                 
			"fence\n"
			"fence\n"
			"csrw fflags, zero\n"
			"csrwi frm, 0x0\n"
			"LBL_KERNEL_HID_0:\n"          
			"li x5, 0xFEEDFEED\n"
			"li x0, 0x5633e0e002\n" 
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
			"LBL_SEQID_0_INT_AMO_ID_0_HID_0:\n"
			"li x16, 0x8157ce0258\n" 
			"amominul.d x20, x20, (x16)\n" 
			"fcvt.pw.ps f16, f5, rne\n"
			"fsrli.pi f27, f27, 0xd\n"
			"fmadd.s f6, f1, f2, f30, rup\n"
			"bgeu x4, x6, 1f\n"
			"1:\n"
			"fpackreph.pi f5, f28\n"
			"bgeu x7, x23, 1f\n"
			"1:\n"
			"feq.ps f8, f2, f27\n"
			"bltu x8, x29, 1f\n"
			"1:\n"
			"LBL_SEQID_0_INT_LOAD_ID_1_HID_0:\n"
			"li x27, 0x8205c5681d\n" 
			"sd x28, 510(x27)\n"
			"lhu x10, 510(x27)\n" 
			"fmin.s f2, f1, f28\n"
			"fcmov.ps f13, f16, f28, f16\n"
			"bge x21, x23, 1f\n"
			"1:\n"
			"sllw x0, x11, x15\n"
			"fmaxu.pi f13, f6, f1\n"
			"fcvt.ps.pwu f9, f4, rdn\n"
			"LBL_SEQID_0_FP_L_GATHER_ID_2_HID_0:\n"
			"li x14, 0x825f90000c\n" 
			"la x20, mem_ptr_offset\n"
			"flw.ps f7, 212(x20)\n"
			"fscwl.ps f17, f7 (x14)\n"
			"fghl.ps f30, f7 (x14)\n" 
			"fand.pi f27, f27, f28\n"
			"fnmadd.ps f4, f6, f28, f4, rtz\n"
			"bgeu x17, x31, 1f\n"
			"1:\n"
			"fmin.pi f8, f5, f6\n"
			"fsat8.pi f28, f4\n"
			"fsgnjx.ps f4, f7, f4\n"
			"LBL_SEQID_0_INT_LOAD_ID_3_HID_0:\n"
			"li x15, 0x82009c0049\n" 
			"sd x26, 1629(x15)\n"
			"lh x14, 1629(x15)\n" 
			"fsgnjx.ps f7, f7, f1\n"
			"fadd.s f13, f30, f8, rdn\n"
			"fround.ps f27, f27, rup\n"
			"fmulh.pi f4, f4, f13\n"
			"fcmov.ps f16, f16, f1, f9\n"
			"LBL_SEQID_0_INT_AMO_ID_4_HID_0:\n"
			"li x16, 0x81472204dc\n" 
			"amomaxl.w x26, x20, (x16)\n" 
			"fmax.ps f6, f7, f10\n"
			"fmax.pi f8, f13, f20\n"
			"fnmadd.s f1, f30, f1, f8, rtz\n"
			"fsub.ps f1, f6, f8, rne\n"
			"fmin.pi f16, f8, f1\n"
			"bne x0, x2, 1f\n"
			"1:\n"
			"LBL_SEQID_0_FP_AMO_ID_5_HID_0:\n"
			"li x28, 0x82379a02d8\n" 
			"la x11, mem_ptr_offset\n"
			"flw.ps f5, 736(x11)\n"
			"famomaxg.pi f13, f5 (x28)\n" 
			"fslli.pi f5, f1, 0x6\n"
			"flt.pi f27, f7, f7\n"
			"fsgnjx.ps f5, f16, f8\n"
			"maskxor m2, m2, m5\n"
			"faddi.pi f5, f3, 0x1e5\n"
			"beq x31, x16, 1f\n"
			"1:\n"
			"LBL_SEQID_0_FP_SCATTER_ID_6_HID_0:\n"
			"li x20, 0x81090aa021\n" 
			"la x18, mem_ptr_offset\n"
			"flw.ps f8, 96(x18)\n"
			"fscb.ps f10, f8 (x20)\n" 
			"flt.ps f13, f30, f10\n"
			"fcvt.pw.ps f10, f10, rup\n"
			"fsgnjx.s f10, f10, f16\n"
			"fminu.pi f13, f16, f10\n"
			"bgeu x3, x17, 1f\n"
			"1:\n"
			"fmulh.pi f16, f16, f20\n"
			"bltu x16, x26, 1f\n"
			"1:\n"
			"LBL_SEQID_0_INT_AMO_ID_7_HID_0:\n"
			"li x10, 0x8154ac0034\n" 
			"amomaxul.w x27, x20, (x10)\n" 
			"fcvt.pw.ps f30, f30, rtz\n"
			"fmadd.ps f3, f8, f20, f8, rmm\n"
			"flem.ps m1, f13, f6\n"
			"fswizz.ps f20, f30, 0x10\n"
			"fle.s x26, f13, f30\n"
			"LBL_SEQID_0_INT_AMO_ID_8_HID_0:\n"
			"li x10, 0x8154ac0030\n" 
			"amominul.d x14, x15, (x10)\n" 
			"fswizz.ps f2, f2, 0xf\n"
			"faddi.pi f2, f2, 0x131\n"
			"fmadd.s f1, f16, f20, f2, rup\n"
			"fsatu8.pi f2, f2\n"
			"fmv.x.s x11, f2\n"
			"bgeu x13, x5, 1f\n"
			"1:\n"
			"LBL_SEQID_0_INT_AMO_ID_9_HID_0:\n"
			"li x18, 0x825de80a5c\n" 
			"amoaddl.w x27, x26, (x18)\n" 
			"fpackreph.pi f1, f9\n"
			"feq.ps f4, f6, f4\n"
			"fsub.pi f27, f8, f28\n"
			"bge x4, x2, 1f\n"
			"1:\n"
			"masknot m5, m1\n"
			"fence\n"
			"LBL_SEQID_0_FP_AMO_ID_10_HID_0:\n"
			"li x15, 0x8247300030\n" 
			"la x28, mem_ptr_offset\n"
			"flw.ps f1, 864(x28)\n"
			"famominl.pi f10, f1 (x15)\n" 
			"srl x10, x10, x28\n"
			"sll x10, x26, x14\n"
			"beq x3, x29, 1f\n"
			"1:\n"
			"srl x15, x11, x18\n"
			"fsra.pi f4, f16, f20\n"
			"remuw x10, x10, x14\n"
			"LBL_SEQID_0_FP_AMO_ID_11_HID_0:\n"
			"li x28, 0x822f90000c\n" 
			"la x15, mem_ptr_offset\n"
			"flw.ps f28, 512(x15)\n"
			"famoxorg.pi f2, f28 (x28)\n" 
			"fsub.pi f5, f5, f5\n"
			"fsll.pi f27, f2, f5\n"
			"fslli.pi f27, f2, 0x0\n"
			"fcvt.pwu.ps f9, f5, rmm\n"
			"flog.ps f16, f27\n"
			"blt x6, x14, 1f\n"
			"1:\n"
			"LBL_SEQID_0_FP_L_GATHER_ID_12_HID_0:\n"
			"li x28, 0x8248bb9538\n" 
			"la x20, mem_ptr_offset\n"
			"flw.ps f8, 104(x20)\n"
			"fscwl.ps f7, f8 (x28)\n"
			"fgwl.ps f7, f8 (x28)\n" 
			"fcvt.ps.pwu f6, f1, rmm\n"
			"bltu x19, x27, 1f\n"
			"1:\n"
			"fsatu8.pi f27, f2\n"
			"ori x26, x26, 0x1c\n"
			"ebreak\n"
			"fcvt.wu.s x14, f2, rne\n"
			"LBL_SEQID_0_INT_AMO_ID_13_HID_0:\n"
			"li x28, 0x81472204d8\n" 
			"amoswapl.d x27, x14, (x28)\n" 
			"add x20, x20, x28\n"
			"remu x26, x20, x0\n"
			"fmvs.x.ps x27, f2, 5\n"
			"slt x27, x14, x0\n"
			"divuw x0, x11, x18\n"
			"LBL_SEQID_0_INT_LOAD_ID_14_HID_0:\n"
			"li x14, 0x82079a02da\n" 
			"sd x10, 240(x14)\n"
			"lbu x26, 240(x14)\n" 
			"fle.pi f4, f10, f5\n"
			"flt.pi f3, f5, f10\n"
			"fexp.ps f10, f27\n"
			"fround.ps f30, f27, rtz\n"
			"for.pi f10, f10, f2\n"
			"LBL_SEQID_0_INT_AMO_ID_15_HID_0:\n"
			"li x28, 0x812a4c0020\n" 
			"amoming.d x11, x16, (x28)\n" 
			"flog.ps f28, f1\n"
			"fcvt.pwu.ps f27, f7, rmm\n"
			"fle.ps f3, f1, f9\n"
			"fswizz.ps f7, f3, 0x2\n"
			"frsq.ps f3, f5\n"
			"bne x10, x23, 1f\n"
			"1:\n"
			"LBL_SEQID_0_INT_AMO_ID_16_HID_0:\n"
			"li x11, 0x824b080018\n" 
			"amoswapl.d x14, x20, (x11)\n" 
			"fsrai.pi f8, f13, 0x0\n"
			"frcp.ps f9, f28\n"
			"fsatu8.pi f4, f30\n"
			"fmax.ps f9, f8, f3\n"
			"fmaxu.pi f27, f13, f20\n"
			"LBL_SEQID_0_FP_AMO_ID_17_HID_0:\n"
			"li x15, 0x825e50e62c\n" 
			"la x20, mem_ptr_offset\n"
			"flw.ps f5, 128(x20)\n"
			"famomaxl.pi f6, f5 (x15)\n" 
			"fpackreph.pi f8, f1\n"
			"fnmadd.s f1, f7, f5, f1, rmm\n"
			"fmin.s f2, f6, f8\n"
			"bgeu x13, x16, 1f\n"
			"1:\n"
			"fswizz.ps f2, f30, 0x1a\n"
			"fnmsub.ps f20, f28, f5, f20, rtz\n"
			"LBL_SEQID_0_INT_AMO_ID_18_HID_0:\n"
			"li x16, 0x82309c0048\n" 
			"amominug.w x10, x11, (x16)\n" 
			"fnmadd.s f13, f30, f9, f2, rdn\n"
			"bltu x22, x29, 1f\n"
			"1:\n"
			"fcvt.wu.s x20, f9, rtz\n"
			"bgeu x2, x8, 1f\n"
			"1:\n"
			"subw x10, x16, x15\n"
			"for.pi f28, f2, f9\n"
			"fxor.pi f2, f2, f5\n"
			"LBL_SEQID_0_FP_AMO_ID_19_HID_0:\n"
			"li x16, 0x8144c80eb4\n" 
			"la x11, mem_ptr_offset\n"
			"flw.ps f1, 576(x11)\n"
			"famoswapl.pi f27, f1 (x16)\n" 
			"flog.ps f28, f5\n"
			"fnmsub.ps f27, f4, f9, f27, rtz\n"
			"fnmadd.ps f13, f13, f3, f5, rne\n"
			"fmaxu.pi f4, f4, f4\n"
			"fsrl.pi f5, f8, f16\n"
			"LBL_SEQID_0_FP_G_GATHER_ID_20_HID_0:\n"
			"li x11, 0x812380063c\n" 
			"la x26, mem_ptr_offset\n"
			"flq2 f3, 52(x26)\n"
			"fscwg.ps f25, f3 (x11)\n"
			"fghg.ps f28, f3 (x11)\n" 
			"fcmovm.ps f13, f10, f8\n"
			"fsub.s f16, f28, f2, rmm\n"
			"bne x6, x21, 1f\n"
			"1:\n"
			"fsgnj.s f13, f2, f13\n"
			"fmaxu.pi f5, f5, f20\n"
			"frsq.ps f9, f9\n"
			"blt x25, x6, 1f\n"
			"1:\n"
			"LBL_SEQID_0_FP_AMO_ID_21_HID_0:\n"
			"li x11, 0x815e5cd3f8\n" 
			"la x28, mem_ptr_offset\n"
			"flw.ps f27, 928(x28)\n"
			"famoswapl.pi f13, f27 (x11)\n" 
			"frsq.ps f3, f7\n"
			"bltu x6, x20, 1f\n"
			"1:\n"
			"fxor.pi f27, f9, f4\n"
			"fmax.pi f9, f6, f1\n"
			"fnmadd.ps f8, f27, f9, f4, rne\n"
			"frsq.ps f7, f4\n"
			"LBL_SEQID_0_CSR_ID_22_HID_0:\n"
			"li x11, 0x8000008208bb9502\n"
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw unlock_va, x11\n" 
			"csrwi tensor_wait, 6\n" 
			"fmul.ps f6, f10, f6, rdn\n"
			"fmax.s f16, f20, f13\n"
			"bgeu x31, x8, 1f\n"
			"1:\n"
			"fsra.pi f2, f1, f8\n"
			"fcvt.pwu.ps f2, f5, rdn\n"
			"for.pi f6, f6, f16\n"
			"bge x27, x16, 1f\n"
			"1:\n"
			"LBL_SEQID_0_FP_AMO_ID_23_HID_0:\n"
			"li x10, 0x813486003c\n" 
			"la x11, mem_ptr_offset\n"
			"flw.ps f6, 800(x11)\n"
			"famoandg.pi f13, f6 (x10)\n" 
			"fcvt.pw.ps f9, f30, rup\n"
			"fmul.s f2, f30, f7, rmm\n"
			"frsq.ps f30, f7\n"
			"flt.pi f6, f7, f20\n"
			"fsub.pi f9, f8, f30\n"
			"beq x2, x0, 1f\n"
			"1:\n"
			"LBL_SEQID_0_INT_AMO_ID_24_HID_0:\n"
			"li x10, 0x8248bb9538\n" 
			"amomaxul.d x26, x28, (x10)\n" 
			"fmin.ps f27, f27, f8\n"
			"fsin.ps f6, f8\n"
			"bgeu x4, x30, 1f\n"
			"1:\n"
			"fsgnjx.s f27, f8, f8\n"
			"bltu x0, x18, 1f\n"
			"1:\n"
			"flt.ps f8, f2, f3\n"
			"bltu x12, x6, 1f\n"
			"1:\n"
			"fclass.ps f13, f2\n"
			"LBL_SEQID_0_INT_LOAD_ID_25_HID_0:\n"
			"li x20, 0x81020ec82c\n" 
			"sd x27, 460(x20)\n"
			"lwu x28, 460(x20)\n" 
			"fsetm.pi m2, f3\n"
			"packb x16, x16, x10\n"
			"slliw x16, x26, 1\n"
			"maskpopc x16, m5\n"
			"bge x17, x16, 1f\n"
			"1:\n"
			"fsgnj.ps f4, f4, f4\n"
			"bge x6, x29, 1f\n"
			"1:\n"
			"LBL_SEQID_0_INT_AMO_ID_26_HID_0:\n"
			"li x10, 0x8238bb953c\n" 
			"amomaxg.w x16, x26, (x10)\n" 
			"fle.pi f1, f9, f5\n"
			"fsub.pi f16, f20, f16\n"
			"remu x16, x26, x10\n"
			"fnmsub.s f4, f13, f1, f3, rtz\n"
			"fnmsub.s f20, f1, f13, f20, rtz\n"
			"LBL_SEQID_0_FP_GATHER_ID_27_HID_0:\n"
			"li x10, 0x810c76d803\n" 
			"la x11, mem_ptr_offset\n"
			"flq2 f5, 160(x11)\n"
			"fscw.ps f8, f5 (x10)\n"
			"fgh.ps f16, f5 (x10)\n" 
			"fcmov.ps f3, f4, f20, f3\n"
			"bge x24, x12, 1f\n"
			"1:\n"
			"mova.m.x x26\n"
			"ecall\n"
			"fcvt.s.w f6, x14, rup\n"
			"fmul.pi f27, f5, f6\n"
			"la x26, rand_ieee754_32\n"
			"flw.ps f3, 512(x26)\n"
			"fcvt.lu.s x26, f3, rdn\n" 
			"sllw x18, x18, x26\n"
			"feq.s x27, f3, f13\n"
			"frsq.ps f2, f13\n"
			"fmulhu.pi f16, f20, f28\n"
			"remw x28, x16, x20\n"
			"LBL_SEQID_0_INT_LOAD_ID_28_HID_0:\n"
			"li x28, 0x81003e0025\n" 
			"sd x10, 1254(x28)\n"
			"lw x26, 1254(x28)\n" 
			"masknot m2, m5\n"
			"rem x14, x18, x18\n"
			"maskand m5, m5, m6\n"
			"beq x24, x14, 1f\n"
			"1:\n"
			"srai x18, x0, 3\n"
			"bge x13, x11, 1f\n"
			"1:\n"
			"fltm.pi m2, f7, f5\n"
			"LBL_SEQID_0_INT_AMO_ID_29_HID_0:\n"
			"li x14, 0x8153800638\n" 
			"amoorl.d x27, x20, (x14)\n" 
			"fmax.s f10, f10, f10\n"
			"fsll.pi f4, f2, f10\n"
			"beq x28, x22, 1f\n"
			"1:\n"
			"fmaxu.pi f13, f2, f10\n"
			"beq x0, x19, 1f\n"
			"1:\n"
			"fmaxu.pi f13, f2, f10\n"
			"fmsub.ps f4, f10, f10, f1, rup\n"
			"beq x16, x19, 1f\n"
			"1:\n"
			"LBL_SEQID_0_FP_AMO_ID_30_HID_0:\n"
			"li x26, 0x822f90000c\n" 
			"la x20, mem_ptr_offset\n"
			"flw.ps f7, 832(x20)\n"
			"famomaxg.ps f7, f7 (x26)\n" 
			"fmul.ps f27, f20, f9, rne\n"
			"addw x18, x16, x18\n"
			"fandi.pi f30, f20, 0x1ad\n"
			"bltu x29, x10, 1f\n"
			"1:\n"
			"fcvt.ps.pw f1, f5, rmm\n"
			"bne x13, x1, 1f\n"
			"1:\n"
			"fmul.pi f3, f20, f20\n"
			"blt x11, x7, 1f\n"
			"1:\n"
			"mov.m.x m0, x0, 0xFF\n"
			"li x5, 0x20\n"
			"la x31, rand_ieee754_32\n"
			"add x31, x31, x5\n"
			"flw.ps f1, 0(x31)\n"
			"flw.ps f16, 2(x31)\n"
			"flw.ps f2, 5(x31)\n"
			"flw.ps f27, 10(x31)\n"
			"flw.ps f28, 2(x31)\n"
			"flw.ps f3, 0(x31)\n"
			"flw.ps f30, 13(x31)\n"
			"flw.ps f5, 15(x31)\n"
			"flw.ps f6, 27(x31)\n"
			"flw.ps f8, 20(x31)\n"
			"flw.ps f9, 24(x31)\n"
			"la x31, rand_int_32\n"
			"add x31, x31, x5\n"
			"lw x0, 0(x31)\n"
			"lw x10, 2(x31)\n"
			"lw x11, 2(x31)\n"
			"lw x14, 1(x31)\n"
			"lw x15, 1(x31)\n"
			"lw x16, 10(x31)\n"
			"lw x18, 21(x31)\n"
			"lw x20, 9(x31)\n"
			"lw x26, 15(x31)\n"
			"lw x27, 9(x31)\n"
			"lw x28, 13(x31)\n"
			"li x21, 0x3\n"
			"LBL_SEQID_0_LOOP_SEQ_HID_0:\n"
			"fsll.pi f28, f3, f6\n"
			"flt.ps f30, f28, f27\n"
			"fence\n"
			"bltu x27, x8, 1f\n"
			"1:\n"
			"divw x27, x10, x27\n"
			"mova.x.m x15\n"
			"mulhu x14, x10, x26\n"
			"fle.ps f13, f8, f1\n"
			"fnmadd.ps f6, f6, f9, f2, rup\n"
			"bge x26, x11, 1f\n"
			"1:\n"
			"andi x11, x11, 0x1e\n"
			"fsatu8.pi f16, f28\n"
			"fmaxu.pi f13, f30, f16\n"
			"ebreak\n"
			"sra x15, x27, x28\n"
			"bgeu x15, x17, 1f\n"
			"1:\n"
			"mova.m.x x28\n"
			"fmulh.pi f28, f28, f5\n"
			"fslli.pi f6, f30, 0xb\n"
			"maskpopcz x11, m5\n"
			"remu x16, x20, x16\n"
			"bne x30, x17, 1f\n"
			"1:\n"
			"fsetm.pi m5, f1\n"
			"flt.pi f27, f27, f3\n"
			"slti x27, x18, 0xb\n"
			"fsgnj.ps f28, f30, f27\n"
			"bne x3, x5, 1f\n"
			"1:\n"
			"fsgnjn.s f3, f6, f2\n"
			"bne x9, x29, 1f\n"
			"1:\n"
			"srliw x11, x14, 0\n"
			"fmadd.ps f28, f1, f28, f27, rup\n"
			"fmul.ps f28, f16, f5, rup\n"
			"fmax.s f2, f30, f6\n"
			"bne x25, x0, 1f\n"
			"1:\n"
			"fcvt.ps.f16 f30, f30\n"
			"srl x15, x14, x14\n"
			"bne x19, x8, 1f\n"
			"1:\n"
			"fmin.s f30, f30, f16\n"
			"add x14, x14, x18\n"
			"fsra.pi f9, f9, f6\n"
			"beq x2, x7, 1f\n"
			"1:\n"
			"mova.m.x x15\n"
			"fsgnjn.s f30, f30, f27\n"
			"fpackreph.pi f30, f2\n"
			"bgeu x4, x23, 1f\n"
			"1:\n"
			"slti x20, x28, 0x19\n"
			"masknot m6, m6\n"
			"fcvt.ps.pw f1, f8, rdn\n"
			"fsgnjn.ps f3, f30, f28\n"
			"masknot m1, m2\n"
			"bgeu x0, x5, 1f\n"
			"1:\n"
			"maskand m2, m5, m2\n"
			"fsgnj.s f2, f6, f27\n"
			"fmax.pi f5, f2, f1\n"
			"slti x28, x16, 0x3\n"
			"div x18, x18, x14\n"
			"mulw x10, x26, x14\n"
			"ffrc.ps f3, f9\n"
			"sltu x15, x18, x15\n"
			"fsetm.pi m5, f8\n"
			"fsub.ps f2, f2, f8, rdn\n"
			"fltm.pi m6, f6, f30\n"
			"bne x9, x26, 1f\n"
			"1:\n"
			"flt.pi f27, f8, f6\n"
			"fmax.ps f27, f28, f30\n"
			"divuw x28, x0, x28\n"
			"beq x23, x23, 1f\n"
			"1:\n"
			"fslli.pi f27, f28, 0x8\n"
			"fmul.pi f2, f3, f8\n"
			"sraw x10, x11, x0\n"
			"fmadd.s f1, f1, f16, f1, rmm\n"
			"fmax.s f8, f16, f8\n"
			"fltm.pi m1, f1, f6\n"
			"bne x28, x28, 1f\n"
			"1:\n"
			"feq.s x15, f28, f5\n"
			"srlw x27, x14, x16\n"
			"frcp.ps f2, f2\n"
			"fpackrepb.pi f16, f2\n"
			"bgeu x20, x12, 1f\n"
			"1:\n"
			"fmadd.s f7, f3, f6, f6, rtz\n"
			"divu x18, x10, x16\n"
			"fcvt.ps.pwu f28, f30, rne\n"
			"bgeu x27, x31, 1f\n"
			"1:\n"
			"slli x15, x15, 3\n"
			"subw x26, x14, x18\n"
			"fmsub.s f27, f16, f30, f16, rne\n"
			"fcmov.ps f7, f28, f30, f1\n"
			"bge x30, x2, 1f\n"
			"1:\n"
			"fsrai.pi f28, f28, 0x7\n"
			"bge x13, x11, 1f\n"
			"1:\n"
			"sltiu x18, x18, 0x17\n"
			"flt.s x27, f27, f27\n"
			"fbci.ps f7, 0x7910f\n"
			"fbci.ps f13, 0x70ddc\n"
			"bge x7, x31, 1f\n"
			"1:\n"
			"fcvt.ps.pwu f27, f3, rdn\n"
			"frcp.ps f30, f8\n"
			"feq.s x28, f5, f16\n"
			"fbcx.ps f8, x0\n"
			"beq x7, x19, 1f\n"
			"1:\n"
			"slliw x10, x15, 4\n"
			"bgeu x14, x22, 1f\n"
			"1:\n"
			"maskxor m5, m6, m5\n"
			"maskand m2, m2, m2\n"
			"fandi.pi f13, f3, 0x16f\n"
			"fle.ps f28, f9, f28\n"
			"bltu x17, x27, 1f\n"
			"1:\n"
			"faddi.pi f3, f16, 0xe4\n"
			"fsgnjx.ps f3, f28, f27\n"
			"addw x11, x11, x27\n"
			"fsub.s f3, f28, f5, rtz\n"
			"maskand m5, m5, m1\n"
			"sltu x0, x15, x14\n"
			"fsgnj.ps f13, f16, f28\n"
			"slli x28, x18, 4\n"
			"maskor m1, m2, m5\n"
			"sltu x28, x20, x15\n"
			"fcmovm.ps f5, f5, f8\n"
			"fmulhu.pi f16, f1, f16\n"
			"fsatu8.pi f13, f9\n"
			"fsub.ps f30, f5, f27, rup\n"
			"bltu x21, x3, 1f\n"
			"1:\n"
			"fsra.pi f28, f9, f16\n"
			"xor x0, x14, x16\n"
			"fclass.ps f10, f27\n"
			"xor x0, x14, x0\n"
			"fsgnjx.ps f7, f1, f3\n"
			"fmv.x.s x10, f2\n"
			"rem x10, x10, x20\n"
			"fcvt.wu.s x10, f6, rdn\n"
			"fminu.pi f16, f9, f16\n"
			"fsgnjn.ps f8, f9, f8\n"
			"fmvz.x.ps x28, f6, 7\n"
			"xori x14, x0, 0x11\n"
			"bltu x23, x29, 1f\n"
			"1:\n"
			"srlw x18, x10, x20\n"
			"xor x16, x16, x15\n"
			"fsat8.pi f10, f16\n"
			"blt x21, x23, 1f\n"
			"1:\n"
			"maskor m2, m2, m5\n"
			"sll x27, x27, x14\n"
			"frcp.ps f6, f27\n"
			"slti x14, x18, 0x4\n"
			"fcvt.pwu.ps f13, f1, rmm\n"
			"fswizz.ps f9, f3, 0xa\n"
			"sll x0, x10, x0\n"
			"or x10, x10, x14\n"
			"bne x12, x11, 1f\n"
			"1:\n"
			"fmax.s f13, f8, f27\n"
			"srl x10, x0, x11\n"
			"xor x27, x26, x0\n"
			"fswizz.ps f27, f28, 0x17\n"
			"sra x16, x10, x26\n"
			"fcvt.w.s x16, f27, rup\n"
			"fbci.ps f28, 0x502b5\n"
			"srl x18, x11, x0\n"
			"sll x0, x0, x11\n"
			"fmul.s f28, f9, f28, rmm\n"
			"fadd.pi f30, f30, f8\n"
			"beq x9, x4, 1f\n"
			"1:\n"
			"fsll.pi f3, f8, f6\n"
			"auipc x11, 0x14\n"
			"fswizz.ps f28, f6, 0xe\n"
			"fsrl.pi f27, f6, f2\n"
			"fsgnj.ps f2, f2, f2\n"
			"fsgnj.s f27, f1, f30\n"
			"bge x23, x11, 1f\n"
			"1:\n"
			"srai x20, x26, 5\n"
			"fcvt.w.s x28, f6, rtz\n"
			"fmsub.s f7, f6, f2, f28, rmm\n"
			"fsgnjn.ps f10, f30, f9\n"
			"lui x27, 0x17\n"
			"fcvt.f16.ps f10, f5\n"
			"fadd.s f13, f1, f9, rtz\n"
			"or x27, x20, x28\n"
			"blt x21, x28, 1f\n"
			"1:\n"
			"mov.m.x m1, x27, 0x6c\n"
			"blt x25, x12, 1f\n"
			"1:\n"
			"andi x10, x10, 0x1c\n"
			"fsin.ps f10, f27\n"
			"fpackreph.pi f13, f6\n"
			"remuw x14, x0, x16\n"
			"sraiw x14, x14, 5\n"
			"remw x10, x10, x15\n"
			"fswizz.ps f5, f5, 0x1\n"
			"bne x22, x3, 1f\n"
			"1:\n"
			"ori x11, x16, 0x18\n"
			"flt.ps f27, f6, f2\n"
			"bne x27, x26, 1f\n"
			"1:\n"
			"sltu x18, x18, x10\n"
			"fmin.pi f13, f6, f8\n"
			"fpackreph.pi f1, f28\n"
			"beq x19, x15, 1f\n"
			"1:\n"
			"feq.pi f10, f2, f30\n"
			"fsatu8.pi f5, f8\n"
			"flt.ps f5, f28, f5\n"
			"fsgnjx.ps f10, f30, f8\n"
			"fmadd.s f10, f6, f27, f30, rtz\n"
			"blt x20, x4, 1f\n"
			"1:\n"
			"fle.pi f7, f27, f3\n"
			"mulh x14, x28, x10\n"
			"fle.pi f6, f9, f6\n"
			"srli x20, x0, 0\n"
			"flem.ps m6, f5, f2\n"
			"srli x0, x18, 0\n"
			"auipc x27, 0x1c\n"
			"addi x21, x21, -1\n"
			"bne x21, x0, LBL_SEQID_0_LOOP_SEQ_HID_0\n"
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_0\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_1_FP_SCATTER_ID_31_HID_0:\n"
			"li x11, 0x8202f4003b\n" 
			"la x14, mem_ptr_offset\n"
			"flq2 f20, 152(x14)\n"
			"fscw.ps f6, f20 (x11)\n" 
			"fmadd.ps f2, f7, f13, f2, rtz\n"
			"fnmadd.s f4, f4, f4, f2, rtz\n"
			"blt x16, x19, 1f\n"
			"1:\n"
			"fle.pi f8, f2, f13\n"
			"flog.ps f1, f8\n"
			"blt x4, x3, 1f\n"
			"1:\n"
			"fcmov.ps f5, f20, f2, f20\n"
			"fmax.ps f4, f4, f2\n"
			"fsgnjn.s f8, f13, f7\n"
			"fmin.ps f20, f2, f4\n"
			"fmsub.ps f20, f20, f2, f13, rne\n"
			"LBL_SEQID_1_INT_AMO_ID_32_HID_0:\n"
			"li x10, 0x81203e0020\n" 
			"amomaxg.d x11, x27, (x10)\n" 
			"fnmsub.ps f5, f27, f5, f10, rne\n"
			"fsat8.pi f30, f7\n"
			"fmax.ps f27, f10, f13\n"
			"blt x2, x11, 1f\n"
			"1:\n"
			"fmulh.pi f5, f20, f7\n"
			"fmul.ps f27, f27, f7, rmm\n"
			"bgeu x7, x11, 1f\n"
			"1:\n"
			"fslli.pi f2, f2, 0xe\n"
			"bne x30, x3, 1f\n"
			"1:\n"
			"fcvt.ps.pw f10, f16, rne\n"
			"fmax.s f10, f13, f5\n"
			"fmin.ps f10, f9, f5\n"
			"bltu x26, x0, 1f\n"
			"1:\n"
			"LBL_SEQID_1_FP_AMO_ID_33_HID_0:\n"
			"li x14, 0x815ff5b02c\n" 
			"la x26, mem_ptr_offset\n"
			"flw.ps f2, 832(x26)\n"
			"famoorl.pi f27, f2 (x14)\n" 
			"fmin.pi f16, f8, f27\n"
			"fmul.s f20, f10, f28, rup\n"
			"mova.x.m x20\n"
			"fbci.ps f5, 0x49974\n"
			"mulhu x15, x18, x10\n"
			"beq x21, x19, 1f\n"
			"1:\n"
			"and x0, x28, x0\n"
			"sraw x18, x18, x14\n"
			"mulh x15, x27, x14\n"
			"srlw x20, x28, x16\n"
			"LBL_SEQID_1_INT_STORE_ID_34_HID_0:\n"
			"li x20, 0x810380063f\n" 
			"sd x10, -1155(x20)\n" 
			"fandi.pi f4, f4, 0x1e5\n"
			"fcvt.pwu.ps f3, f13, rdn\n"
			"fnmadd.ps f4, f6, f5, f7, rdn\n"
			"fmin.ps f13, f13, f16\n"
			"fsin.ps f5, f1\n"
			"fsub.pi f7, f28, f8\n"
			"fsrl.pi f30, f10, f30\n"
			"flog.ps f6, f16\n"
			"fnmadd.ps f30, f30, f2, f13, rup\n"
			"LBL_SEQID_1_FP_G_STORE_ID_35_HID_0:\n"
			"li x28, 0x812da60680\n" 
			"mov.m.x m0, zero, 0xff\n"
			"fswg.ps f28, (x28)\n" 
			"fnot.pi f8, f30\n"
			"fmax.ps f2, f16, f20\n"
			"fsin.ps f28, f10\n"
			"flt.pi f13, f13, f16\n"
			"fcvt.ps.f16 f3, f13\n"
			"fmax.pi f10, f28, f20\n"
			"fcvt.pw.ps f7, f8, rup\n"
			"slli x18, x18, 5\n"
			"srliw x11, x0, 3\n"
			"la x20, rand_int_32\n"
			"flw.ps f13, 256(x20)\n"
			"flw.ps f5, 160(x20)\n"
			"fremu.pi f2, f13, f5\n" 
			"lui x18, 0x1e\n"
			"fmulhu.pi f7, f4, f6\n"
			"fcvt.ps.pw f10, f3, rne\n"
			"blt x5, x17, 1f\n"
			"1:\n"
			"fltu.pi f1, f1, f4\n"
			"fnmadd.ps f1, f30, f4, f30, rdn\n"
			"fle.s x14, f6, f1\n"
			"maskand m1, m2, m2\n"
			"fadd.ps f2, f7, f3, rup\n"
			"fsub.ps f16, f1, f30, rup\n"
			"LBL_SEQID_1_FP_AMO_ID_36_HID_0:\n"
			"li x10, 0x82409c0048\n" 
			"la x14, mem_ptr_offset\n"
			"flw.ps f6, 64(x14)\n"
			"famomaxl.ps f6, f6 (x10)\n" 
			"fsatu8.pi f8, f7\n"
			"fmul.ps f7, f8, f7, rtz\n"
			"fsub.pi f20, f8, f8\n"
			"fsra.pi f8, f8, f8\n"
			"fsub.pi f30, f8, f30\n"
			"fminu.pi f8, f30, f30\n"
			"fadd.s f30, f8, f7, rne\n"
			"fmaxu.pi f30, f30, f7\n"
			"fxor.pi f30, f30, f30\n"
			"LBL_SEQID_1_INT_STORE_ID_37_HID_0:\n"
			"li x10, 0x8107ce0258\n" 
			"sd x26, -1770(x10)\n" 
			"fcvt.s.w f3, x0, rup\n"
			"bge x24, x30, 1f\n"
			"1:\n"
			"fsat8.pi f3, f7\n"
			"beq x11, x21, 1f\n"
			"1:\n"
			"fmulhu.pi f30, f8, f8\n"
			"fsatu8.pi f13, f1\n"
			"mulhu x27, x27, x0\n"
			"fsatu8.pi f9, f8\n"
			"flt.s x16, f3, f20\n"
			"fle.ps f7, f8, f3\n"
			"fsetm.pi m1, f7\n"
			"LBL_SEQID_1_INT_AMO_ID_38_HID_0:\n"
			"li x18, 0x82370e64f8\n" 
			"amominug.d x26, x27, (x18)\n" 
			"fadd.pi f2, f10, f13\n"
			"fsgnjx.ps f5, f9, f2\n"
			"blt x12, x13, 1f\n"
			"1:\n"
			"fsll.pi f6, f8, f6\n"
			"fltu.pi f2, f6, f20\n"
			"fltm.ps m1, f5, f8\n"
			"fmsub.s f6, f30, f6, f20, rup\n"
			"bgeu x14, x21, 1f\n"
			"1:\n"
			"fltm.pi m5, f27, f8\n"
			"srlw x16, x20, x28\n"
			"slti x14, x20, 0xc\n"
			"bne x17, x17, 1f\n"
			"1:\n"
			"LBL_SEQID_1_FP_SCATTER_ID_39_HID_0:\n"
			"li x10, 0x82070e64f8\n" 
			"la x14, mem_ptr_offset\n"
			"flq2 f8, 12(x14)\n"
			"fscw.ps f7, f8 (x10)\n" 
			"fslli.pi f30, f5, 0x5\n"
			"blt x1, x12, 1f\n"
			"1:\n"
			"fadd.s f16, f3, f27, rne\n"
			"fcvt.ps.pwu f5, f4, rtz\n"
			"fexp.ps f5, f20\n"
			"fle.ps f8, f8, f6\n"
			"bge x22, x30, 1f\n"
			"1:\n"
			"fsrai.pi f9, f2, 0xb\n"
			"fmax.s f13, f27, f5\n"
			"fmul.s f5, f8, f8, rne\n"
			"fpackrepb.pi f8, f5\n"
			"LBL_SEQID_1_FP_L_SCATTER_ID_40_HID_0:\n"
			"li x26, 0x825ab4882c\n" 
			"la x20, mem_ptr_offset\n"
			"flw.ps f4, 204(x20)\n"
			"fschl.ps f30, f4 (x26)\n" 
			"fcvt.pw.ps f5, f10, rmm\n"
			"fmin.s f10, f10, f3\n"
			"bltu x22, x30, 1f\n"
			"1:\n"
			"divuw x26, x0, x0\n"
			"fmvs.x.ps x10, f5, 6\n"
			"srliw x0, x10, 3\n"
			"maskor m5, m5, m5\n"
			"mulhu x11, x10, x0\n"
			"fmsub.s f5, f5, f16, f5, rdn\n"
			"div x11, x26, x0\n"
			"LBL_SEQID_1_FP_G_GATHER_ID_41_HID_0:\n"
			"li x14, 0x81203e0024\n" 
			"la x27, mem_ptr_offset\n"
			"flq2 f13, 184(x27)\n"
			"fscwg.ps f9, f13 (x14)\n"
			"fghg.ps f16, f13 (x14)\n" 
			"sltu x16, x16, x14\n"
			"xor x11, x18, x11\n"
			"maskxor m6, m1, m5\n"
			"bltu x12, x7, 1f\n"
			"1:\n"
			"srliw x18, x18, 0\n"
			"beq x0, x2, 1f\n"
			"1:\n"
			"fcvt.ps.pwu f7, f3, rup\n"
			"maskxor m5, m2, m6\n"
			"fadd.ps f7, f7, f16, rne\n"
			"bge x17, x9, 1f\n"
			"1:\n"
			"mulh x11, x18, x16\n"
			"maskor m5, m6, m6\n"
			"LBL_SEQID_1_INT_AMO_ID_42_HID_0:\n"
			"li x14, 0x813e5cd3f8\n" 
			"amoorg.d x28, x28, (x14)\n" 
			"fclass.ps f20, f6\n"
			"fmul.pi f5, f16, f27\n"
			"fandi.pi f6, f6, 0x20\n"
			"bgeu x13, x31, 1f\n"
			"1:\n"
			"fsin.ps f20, f27\n"
			"bltu x8, x20, 1f\n"
			"1:\n"
			"for.pi f5, f16, f6\n"
			"bne x23, x15, 1f\n"
			"1:\n"
			"fsgnjn.ps f10, f5, f16\n"
			"fcvt.f16.ps f16, f4\n"
			"fcvt.pwu.ps f1, f5, rmm\n"
			"fnmadd.ps f28, f6, f7, f30, rdn\n"
			"LBL_SEQID_1_FP_AMO_ID_43_HID_0:\n"
			"li x15, 0x8157ce0258\n" 
			"la x18, mem_ptr_offset\n"
			"flw.ps f6, 512(x18)\n"
			"famomaxul.pi f30, f6 (x15)\n" 
			"fminu.pi f27, f30, f7\n"
			"fmsub.s f2, f8, f7, f8, rne\n"
			"fcvt.ps.pwu f1, f7, rmm\n"
			"faddi.pi f28, f8, 0x79\n"
			"blt x9, x8, 1f\n"
			"1:\n"
			"fmulhu.pi f2, f8, f30\n"
			"fnmsub.ps f30, f8, f30, f8, rmm\n"
			"bgeu x5, x2, 1f\n"
			"1:\n"
			"fnmadd.ps f1, f30, f30, f7, rup\n"
			"faddi.pi f3, f7, 0x156\n"
			"feq.pi f8, f8, f8\n"
			"LBL_SEQID_1_INT_STORE_ID_44_HID_0:\n"
			"li x27, 0x810a4c0027\n" 
			"sw x18, 125(x27)\n" 
			"faddi.pi f9, f9, 0x1b2\n"
			"fmin.s f28, f10, f9\n"
			"fcmov.ps f16, f5, f1, f10\n"
			"fxor.pi f3, f3, f10\n"
			"fsgnjn.s f13, f4, f13\n"
			"fclass.ps f20, f20\n"
			"flog.ps f20, f1\n"
			"fcvt.pwu.ps f3, f2, rmm\n"
			"fmax.pi f8, f8, f30\n"
			"LBL_SEQID_1_INT_AMO_ID_45_HID_0:\n"
			"li x27, 0x8123800638\n" 
			"amoorg.d x14, x15, (x27)\n" 
			"fcvt.ps.pw f5, f13, rmm\n"
			"bgeu x28, x25, 1f\n"
			"1:\n"
			"feq.pi f2, f28, f13\n"
			"bge x6, x13, 1f\n"
			"1:\n"
			"fadd.ps f16, f13, f16, rdn\n"
			"fnmadd.ps f13, f28, f13, f8, rtz\n"
			"fmsub.ps f10, f16, f8, f28, rdn\n"
			"fnot.pi f20, f27\n"
			"feq.pi f10, f27, f13\n"
			"fltu.pi f2, f13, f20\n"
			"fand.pi f5, f16, f20\n"
			"LBL_SEQID_1_INT_AMO_ID_46_HID_0:\n"
			"li x16, 0x815b920038\n" 
			"amoxorl.d x18, x26, (x16)\n" 
			"fadd.s f16, f28, f1, rup\n"
			"bltu x7, x19, 1f\n"
			"1:\n"
			"fmul.s f16, f3, f5, rtz\n"
			"fcvt.ps.pw f2, f2, rdn\n"
			"fmax.s f16, f3, f16\n"
			"fmul.ps f4, f6, f16, rtz\n"
			"bgeu x30, x14, 1f\n"
			"1:\n"
			"fandi.pi f20, f10, 0x3c\n"
			"fsat8.pi f6, f30\n"
			"fandi.pi f30, f8, 0x7\n"
			"frcp.ps f28, f2\n"
			"LBL_SEQID_1_INT_AMO_ID_47_HID_0:\n"
			"li x16, 0x82409c0048\n" 
			"amoorl.w x27, x15, (x16)\n" 
			"fpackreph.pi f6, f30\n"
			"fpackrepb.pi f2, f28\n"
			"feq.pi f2, f4, f3\n"
			"fsll.pi f5, f2, f7\n"
			"fsat8.pi f10, f5\n"
			"fround.ps f9, f27, rdn\n"
			"fcvt.f16.ps f2, f9\n"
			"fsub.ps f16, f10, f9, rdn\n"
			"bge x0, x12, 1f\n"
			"1:\n"
			"fle.ps f20, f7, f13\n"
			"LBL_SEQID_1_INT_AMO_ID_48_HID_0:\n"
			"li x16, 0x81403e0024\n" 
			"amomaxl.w x20, x11, (x16)\n" 
			"for.pi f16, f9, f10\n"
			"ffrc.ps f5, f16\n"
			"fadd.pi f4, f13, f30\n"
			"fsgnj.s f9, f8, f9\n"
			"fnmadd.s f9, f5, f30, f2, rup\n"
			"beq x7, x14, 1f\n"
			"1:\n"
			"fsub.ps f30, f30, f10, rtz\n"
			"frsq.ps f27, f6\n"
			"fcvt.f16.ps f10, f3\n"
			"fnmsub.ps f3, f13, f2, f6, rtz\n"
			"LBL_SEQID_1_FP_AMO_ID_49_HID_0:\n"
			"li x28, 0x8124c80eb4\n" 
			"la x16, mem_ptr_offset\n"
			"flw.ps f4, 704(x16)\n"
			"famominug.pi f7, f4 (x28)\n" 
			"fsub.ps f10, f5, f6, rne\n"
			"beq x16, x8, 1f\n"
			"1:\n"
			"fsra.pi f4, f3, f3\n"
			"flem.ps m5, f20, f8\n"
			"fsub.s f5, f5, f5, rtz\n"
			"remu x14, x27, x0\n"
			"bge x7, x7, 1f\n"
			"1:\n"
			"fsgnj.s f5, f13, f2\n"
			"mov.m.x m1, x20, 0x32\n"
			"fsrl.pi f16, f1, f9\n"
			"flog.ps f4, f8\n"
			"bltu x26, x9, 1f\n"
			"1:\n"
			"LBL_SEQID_1_INT_AMO_ID_50_HID_0:\n"
			"li x10, 0x8222f40038\n" 
			"amomaxug.w x27, x28, (x10)\n" 
			"fnmadd.ps f4, f20, f30, f4, rne\n"
			"fmin.s f30, f4, f30\n"
			"fclass.ps f1, f30\n"
			"blt x3, x9, 1f\n"
			"1:\n"
			"fmin.ps f20, f4, f20\n"
			"fsgnjx.s f4, f7, f10\n"
			"fsub.pi f1, f1, f1\n"
			"fmin.pi f10, f1, f10\n"
			"fcvt.ps.f16 f7, f8\n"
			"blt x14, x27, 1f\n"
			"1:\n"
			"fle.ps f9, f10, f7\n"
			"LBL_SEQID_1_FP_L_SCATTER_ID_51_HID_0:\n"
			"li x14, 0x8144c80eb4\n" 
			"la x28, mem_ptr_offset\n"
			"flq2 f2, 48(x28)\n"
			"fschl.ps f3, f2 (x14)\n" 
			"mova.m.x x16\n"
			"frsq.ps f30, f4\n"
			"rem x26, x26, x0\n"
			"sra x18, x16, x0\n"
			"srlw x16, x16, x16\n"
			"frsq.ps f1, f10\n"
			"bgeu x4, x25, 1f\n"
			"1:\n"
			"fadd.s f4, f30, f4, rup\n"
			"rem x11, x26, x16\n"
			"ebreak\n"
			"beq x23, x26, 1f\n"
			"1:\n"
			"LBL_SEQID_1_FP_GATHER_ID_52_HID_0:\n"
			"li x28, 0x81072204df\n" 
			"la x16, mem_ptr_offset\n"
			"flw.ps f16, 144(x16)\n"
			"fscw.ps f16, f16 (x28)\n"
			"fgb.ps f20, f16 (x28)\n" 
			"fsin.ps f28, f1\n"
			"fbci.ps f30, 0xf548\n"
			"fslli.pi f9, f9, 0x4\n"
			"addiw x18, x28, 0x17\n"
			"or x10, x10, x0\n"
			"fclass.s x11, f30\n"
			"slli x15, x20, 2\n"
			"bltu x3, x21, 1f\n"
			"1:\n"
			"fnmadd.ps f7, f2, f30, f30, rtz\n"
			"fsub.pi f16, f1, f13\n"
			"LBL_SEQID_1_INT_G_STORE_ID_53_HID_0:\n"
			"li x28, 0x82379a02d8\n" 
			"shg x26, (x28)\n" 
			"sll x16, x0, x16\n"
			"ebreak\n"
			"fmvs.x.ps x15, f20, 6\n"
			"fltm.pi m2, f4, f10\n"
			"feq.pi f20, f16, f20\n"
			"fmul.ps f16, f16, f6, rdn\n"
			"for.pi f9, f4, f4\n"
			"fmul.s f28, f20, f10, rne\n"
			"bgeu x15, x1, 1f\n"
			"1:\n"
			"fbci.pi f20, 0x12e70\n"
			"LBL_SEQID_1_FP_L_GATHER_ID_54_HID_0:\n"
			"li x11, 0x8252f40038\n" 
			"la x20, mem_ptr_offset\n"
			"flw.ps f9, 88(x20)\n"
			"fscwl.ps f5, f9 (x11)\n"
			"fghl.ps f16, f9 (x11)\n" 
			"frsq.ps f4, f8\n"
			"fnmadd.s f13, f5, f28, f4, rup\n"
			"fcvt.pwu.ps f28, f28, rdn\n"
			"fsat8.pi f7, f16\n"
			"faddi.pi f16, f3, 0x154\n"
			"bgeu x18, x5, 1f\n"
			"1:\n"
			"fpackreph.pi f7, f28\n"
			"fmadd.ps f13, f16, f7, f7, rtz\n"
			"fadd.ps f5, f5, f16, rtz\n"
			"fltu.pi f16, f2, f9\n"
			"LBL_SEQID_1_FP_AMO_ID_55_HID_0:\n"
			"li x20, 0x824e94a81c\n" 
			"la x11, mem_ptr_offset\n"
			"flw.ps f16, 896(x11)\n"
			"famominul.pi f1, f16 (x20)\n" 
			"fsrai.pi f1, f30, 0x9\n"
			"bge x10, x30, 1f\n"
			"1:\n"
			"slt x15, x27, x0\n"
			"fnmsub.ps f5, f16, f6, f6, rmm\n"
			"fbci.pi f6, 0x4ff54\n"
			"fsub.ps f1, f8, f20, rtz\n"
			"bgeu x21, x28, 1f\n"
			"1:\n"
			"feq.ps f27, f8, f20\n"
			"sltu x20, x0, x27\n"
			"remuw x20, x27, x0\n"
			"fsgnjn.ps f20, f6, f1\n"
			"LBL_SEQID_1_FP_STORE_ID_56_HID_0:\n"
			"li x26, 0x820de80a5f\n" 
			"fsw.ps f28, -1423(x26)\n" 
			"fnmsub.s f4, f6, f28, f13, rtz\n"
			"feq.s x11, f28, f27\n"
			"srliw x20, x15, 1\n"
			"fsll.pi f27, f10, f6\n"
			"sllw x10, x16, x10\n"
			"fle.s x28, f2, f2\n"
			"andi x14, x27, 0x5\n"
			"bltu x1, x12, 1f\n"
			"1:\n"
			"fltu.pi f13, f28, f13\n"
			"mulh x18, x18, x26\n"
			"bgeu x11, x29, 1f\n"
			"1:\n"
			"LBL_SEQID_1_CSR_ID_57_HID_0:\n"
			"li x14, 0xc000081090aa00a\n"
			"li x31, (0x2 << 6) | 0x1\n"
			"csrw evict_va, x14\n" 
			"csrwi tensor_wait, 6\n" 
			"divw x11, x0, x16\n"
			"fcmov.ps f10, f3, f20, f1\n"
			"fnmadd.ps f5, f6, f5, f6, rtz\n"
			"bgeu x14, x6, 1f\n"
			"1:\n"
			"fmsub.ps f5, f5, f27, f27, rne\n"
			"packb x15, x27, x0\n"
			"mulhsu x16, x18, x16\n"
			"slli x20, x10, 2\n"
			"slliw x27, x26, 5\n"
			"slli x20, x10, 5\n"
			"LBL_SEQID_1_FP_GATHER_ID_58_HID_0:\n"
			"li x18, 0x8207300033\n" 
			"la x16, mem_ptr_offset\n"
			"flw.ps f1, 132(x16)\n"
			"fscw.ps f18, f1 (x18)\n"
			"fgw.ps f1, f1 (x18)\n" 
			"fmadd.ps f27, f30, f16, f27, rup\n"
			"divu x27, x18, x28\n"
			"mulw x16, x16, x15\n"
			"fsll.pi f6, f8, f8\n"
			"fmaxu.pi f30, f30, f30\n"
			"ecall\n"
			"fnmsub.ps f5, f30, f20, f10, rdn\n"
			"fsgnjn.s f3, f8, f7\n"
			"fmaxu.pi f30, f20, f30\n"
			"bne x31, x31, 1f\n"
			"1:\n"
			"LBL_SEQID_1_INT_G_STORE_ID_59_HID_0:\n"
			"li x14, 0x812a20800c\n" 
			"shg x16, (x14)\n" 
			"divu x10, x26, x27\n"
			"fsetm.pi m1, f20\n"
			"bne x14, x24, 1f\n"
			"1:\n"
			"slti x14, x14, 0x5\n"
			"fclass.ps f6, f10\n"
			"srl x15, x20, x18\n"
			"fmin.pi f4, f10, f9\n"
			"fsra.pi f6, f6, f20\n"
			"mulhu x10, x27, x18\n"
			"maskor m1, m5, m1\n"
			"LBL_SEQID_1_INT_AMO_ID_60_HID_0:\n"
			"li x11, 0x813a640038\n" 
			"amomaxg.w x16, x20, (x11)\n" 
			"fadd.pi f4, f6, f5\n"
			"fsra.pi f1, f3, f1\n"
			"fsgnj.ps f3, f16, f3\n"
			"fcvt.pwu.ps f16, f5, rup\n"
			"fpackrepb.pi f30, f5\n"
			"bgeu x11, x26, 1f\n"
			"1:\n"
			"fsrli.pi f27, f1, 0x0\n"
			"fandi.pi f4, f1, 0x157\n"
			"fmax.s f3, f3, f20\n"
			"beq x30, x1, 1f\n"
			"1:\n"
			"flt.pi f4, f16, f1\n"
			"LBL_SEQID_1_INT_AMO_ID_61_HID_0:\n"
			"li x18, 0x8137ce0258\n" 
			"amoorg.d x10, x20, (x18)\n" 
			"fcvt.ps.f16 f27, f28\n"
			"fpackrepb.pi f9, f4\n"
			"sll x28, x28, x10\n"
			"fmadd.ps f6, f13, f6, f7, rup\n"
			"for.pi f13, f3, f2\n"
			"fclass.s x20, f8\n"
			"fpackreph.pi f8, f27\n"
			"fmadd.s f9, f28, f4, f10, rup\n"
			"flt.s x15, f7, f4\n"
			"mov.m.x m0, x0, 0xFF\n"
			"li x5, 0x40\n"
			"la x31, rand_ieee754_32\n"
			"add x31, x31, x5\n"
			"flw.ps f1, 0(x31)\n"
			"flw.ps f10, 3(x31)\n"
			"flw.ps f13, 3(x31)\n"
			"flw.ps f16, 3(x31)\n"
			"flw.ps f20, 1(x31)\n"
			"flw.ps f27, 5(x31)\n"
			"flw.ps f5, 14(x31)\n"
			"flw.ps f7, 28(x31)\n"
			"li x21, 0x3\n"
			"LBL_SEQID_1_LOOP_SEQ_HID_0:\n"
			"fmin.pi f28, f16, f5\n"
			"fle.ps f6, f20, f7\n"
			"fround.ps f5, f27, rtz\n"
			"fcmovm.ps f13, f1, f20\n"
			"fmax.s f27, f5, f5\n"
			"fnmsub.s f13, f5, f10, f10, rtz\n"
			"fpackrepb.pi f10, f27\n"
			"fmul.s f27, f27, f1, rdn\n"
			"fmulh.pi f13, f27, f13\n"
			"fmax.s f6, f7, f5\n"
			"fround.ps f27, f7, rne\n"
			"bltu x4, x5, 1f\n"
			"1:\n"
			"fsgnj.ps f28, f10, f10\n"
			"fcmov.ps f27, f16, f5, f1\n"
			"bge x23, x10, 1f\n"
			"1:\n"
			"fexp.ps f1, f1\n"
			"fle.pi f27, f27, f16\n"
			"fmax.s f27, f1, f20\n"
			"bgeu x5, x26, 1f\n"
			"1:\n"
			"for.pi f10, f10, f27\n"
			"bgeu x30, x29, 1f\n"
			"1:\n"
			"fmulhu.pi f3, f20, f27\n"
			"fmul.s f10, f20, f7, rup\n"
			"fcvt.ps.pwu f16, f16, rmm\n"
			"fmin.pi f10, f27, f20\n"
			"beq x16, x25, 1f\n"
			"1:\n"
			"for.pi f6, f20, f16\n"
			"fminu.pi f6, f16, f1\n"
			"bne x22, x29, 1f\n"
			"1:\n"
			"fmadd.ps f28, f5, f1, f16, rup\n"
			"blt x2, x22, 1f\n"
			"1:\n"
			"flog.ps f27, f13\n"
			"fcvt.ps.f16 f16, f16\n"
			"fmadd.s f28, f27, f27, f1, rne\n"
			"fsra.pi f28, f16, f13\n"
			"flt.pi f6, f1, f1\n"
			"fnmadd.s f13, f13, f5, f20, rtz\n"
			"fnmadd.s f13, f13, f7, f10, rdn\n"
			"fmax.ps f28, f1, f7\n"
			"fadd.s f20, f13, f5, rmm\n"
			"faddi.pi f13, f1, 0x17d\n"
			"fxor.pi f1, f10, f10\n"
			"fsgnj.s f3, f13, f5\n"
			"fadd.pi f13, f13, f13\n"
			"fandi.pi f28, f7, 0xd5\n"
			"fadd.pi f28, f27, f20\n"
			"bgeu x28, x24, 1f\n"
			"1:\n"
			"fcvt.pwu.ps f10, f16, rmm\n"
			"bgeu x30, x21, 1f\n"
			"1:\n"
			"fround.ps f20, f16, rne\n"
			"fmaxu.pi f13, f13, f1\n"
			"fmin.pi f10, f1, f27\n"
			"fxor.pi f16, f16, f16\n"
			"fsra.pi f13, f27, f27\n"
			"bge x4, x25, 1f\n"
			"1:\n"
			"fmul.pi f10, f10, f10\n"
			"fsrai.pi f3, f5, 0x2\n"
			"fnmadd.ps f10, f20, f20, f10, rdn\n"
			"fsat8.pi f20, f5\n"
			"bne x27, x7, 1f\n"
			"1:\n"
			"fcvt.f16.ps f20, f20\n"
			"fnot.pi f28, f1\n"
			"fsat8.pi f13, f1\n"
			"fsatu8.pi f7, f7\n"
			"fnmsub.ps f7, f5, f7, f20, rne\n"
			"frcp.ps f13, f16\n"
			"fmin.s f6, f7, f5\n"
			"fmin.s f20, f5, f16\n"
			"fsrl.pi f7, f27, f7\n"
			"beq x30, x14, 1f\n"
			"1:\n"
			"fsll.pi f27, f13, f7\n"
			"fexp.ps f7, f7\n"
			"fcvt.ps.pw f1, f13, rne\n"
			"fmax.ps f3, f20, f10\n"
			"fnot.pi f27, f20\n"
			"fnmadd.ps f6, f10, f20, f13, rdn\n"
			"fandi.pi f6, f1, 0x84\n"
			"fsrl.pi f20, f27, f27\n"
			"fmul.s f10, f1, f5, rmm\n"
			"fmax.ps f13, f27, f27\n"
			"bltu x1, x5, 1f\n"
			"1:\n"
			"fmax.ps f3, f7, f16\n"
			"fcvt.f16.ps f20, f27\n"
			"fminu.pi f10, f16, f10\n"
			"fsrl.pi f27, f16, f1\n"
			"fround.ps f10, f16, rup\n"
			"fcvt.ps.pw f10, f16, rtz\n"
			"bne x6, x4, 1f\n"
			"1:\n"
			"fsub.pi f27, f7, f20\n"
			"fsat8.pi f7, f13\n"
			"fsgnj.s f13, f1, f7\n"
			"beq x7, x19, 1f\n"
			"1:\n"
			"fadd.pi f10, f1, f10\n"
			"fcvt.ps.pw f10, f10, rup\n"
			"fmin.pi f7, f5, f5\n"
			"bltu x17, x18, 1f\n"
			"1:\n"
			"fle.ps f5, f10, f13\n"
			"for.pi f28, f10, f16\n"
			"fmax.s f3, f16, f13\n"
			"fmul.s f27, f13, f27, rup\n"
			"fmaxu.pi f16, f16, f13\n"
			"bne x3, x22, 1f\n"
			"1:\n"
			"fcvt.f16.ps f16, f16\n"
			"beq x12, x12, 1f\n"
			"1:\n"
			"fpackrepb.pi f20, f7\n"
			"fnmsub.ps f3, f16, f5, f1, rmm\n"
			"fmin.ps f6, f13, f7\n"
			"fsrai.pi f28, f27, 0x1\n"
			"fcvt.ps.pwu f5, f10, rtz\n"
			"ffrc.ps f5, f7\n"
			"bge x7, x15, 1f\n"
			"1:\n"
			"fmul.s f3, f10, f27, rup\n"
			"fcvt.ps.pw f10, f13, rdn\n"
			"fminu.pi f6, f13, f7\n"
			"fle.ps f10, f10, f10\n"
			"frsq.ps f27, f1\n"
			"fminu.pi f20, f5, f20\n"
			"frsq.ps f3, f27\n"
			"beq x27, x22, 1f\n"
			"1:\n"
			"fsin.ps f20, f1\n"
			"fmax.pi f20, f10, f10\n"
			"fmsub.ps f13, f13, f20, f20, rmm\n"
			"fand.pi f28, f20, f13\n"
			"fminu.pi f6, f20, f20\n"
			"fsra.pi f20, f5, f16\n"
			"fmin.ps f27, f16, f1\n"
			"fcmov.ps f5, f1, f5, f10\n"
			"beq x5, x11, 1f\n"
			"1:\n"
			"fsrl.pi f13, f13, f13\n"
			"fmulh.pi f20, f13, f13\n"
			"fnmadd.ps f27, f13, f16, f16, rdn\n"
			"fsgnjx.ps f20, f10, f16\n"
			"bltu x28, x20, 1f\n"
			"1:\n"
			"fsat8.pi f13, f7\n"
			"fsgnjx.ps f3, f5, f5\n"
			"frcp.ps f3, f20\n"
			"fcmovm.ps f13, f5, f5\n"
			"fsin.ps f5, f10\n"
			"fle.ps f7, f16, f27\n"
			"fcmovm.ps f3, f5, f16\n"
			"frsq.ps f27, f10\n"
			"fpackreph.pi f27, f27\n"
			"flt.ps f27, f16, f1\n"
			"flt.pi f16, f16, f13\n"
			"bne x9, x8, 1f\n"
			"1:\n"
			"fadd.pi f3, f1, f27\n"
			"fmsub.s f3, f13, f7, f5, rmm\n"
			"fmin.ps f10, f20, f13\n"
			"fmsub.s f28, f7, f5, f1, rup\n"
			"fsgnjn.s f1, f1, f27\n"
			"fnot.pi f13, f27\n"
			"fle.ps f20, f10, f5\n"
			"bgeu x15, x15, 1f\n"
			"1:\n"
			"fsrl.pi f3, f7, f27\n"
			"bltu x8, x31, 1f\n"
			"1:\n"
			"fsgnj.s f6, f27, f20\n"
			"frcp.ps f10, f10\n"
			"fnmadd.s f28, f10, f27, f13, rmm\n"
			"fmax.s f13, f27, f13\n"
			"bne x30, x12, 1f\n"
			"1:\n"
			"fcmovm.ps f7, f5, f16\n"
			"fadd.s f3, f20, f10, rtz\n"
			"bltu x26, x17, 1f\n"
			"1:\n"
			"fmadd.s f13, f5, f27, f27, rne\n"
			"fsgnjx.ps f13, f13, f13\n"
			"fsgnjn.ps f5, f27, f5\n"
			"fmulh.pi f28, f5, f16\n"
			"frcp.ps f13, f27\n"
			"fltu.pi f3, f1, f10\n"
			"fexp.ps f27, f7\n"
			"fmaxu.pi f1, f1, f10\n"
			"fmsub.s f1, f1, f5, f27, rmm\n"
			"fslli.pi f20, f20, 0x5\n"
			"fmul.ps f28, f7, f27, rtz\n"
			"fsgnjx.ps f3, f1, f16\n"
			"fmax.ps f20, f1, f20\n"
			"flog.ps f27, f27\n"
			"fswizz.ps f13, f5, 0xb\n"
			"fpackrepb.pi f7, f13\n"
			"fsgnjx.ps f3, f1, f7\n"
			"fsub.s f16, f16, f1, rne\n"
			"blt x18, x18, 1f\n"
			"1:\n"
			"fsatu8.pi f3, f5\n"
			"beq x16, x27, 1f\n"
			"1:\n"
			"fnmadd.s f6, f5, f13, f7, rdn\n"
			"fadd.ps f13, f16, f10, rne\n"
			"fmax.ps f7, f13, f5\n"
			"fsub.s f10, f5, f5, rdn\n"
			"blt x15, x22, 1f\n"
			"1:\n"
			"flt.ps f20, f20, f5\n"
			"fmulhu.pi f28, f10, f10\n"
			"fadd.pi f3, f20, f1\n"
			"frsq.ps f10, f5\n"
			"fandi.pi f28, f13, 0x6b\n"
			"bgeu x30, x2, 1f\n"
			"1:\n"
			"fpackrepb.pi f7, f27\n"
			"fcvt.f16.ps f20, f10\n"
			"for.pi f6, f27, f7\n"
			"bne x6, x3, 1f\n"
			"1:\n"
			"fmsub.ps f27, f1, f13, f1, rne\n"
			"bltu x31, x0, 1f\n"
			"1:\n"
			"fsrli.pi f27, f10, 0x8\n"
			"fsra.pi f5, f5, f10\n"
			"blt x14, x20, 1f\n"
			"1:\n"
			"fmul.ps f7, f16, f20, rdn\n"
			"fround.ps f28, f7, rdn\n"
			"fcvt.f16.ps f20, f7\n"
			"fnot.pi f5, f13\n"
			"addi x21, x21, -1\n"
			"bne x21, x0, LBL_SEQID_1_LOOP_SEQ_HID_0\n"
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_0\n"
			"addi x30, x30, 1\n"
			"la x16, rand_ieee754_32\n"
			"flw.ps f2, 288(x16)\n"
			"flw.ps f1, 32(x16)\n"
			"fle.s x27, f2, f1\n" 
			"fcvt.pwu.ps f9, f4, rmm\n"
			"sltu x11, x15, x11\n"
			"srai x10, x11, 2\n"
			"ori x28, x28, 0xe\n"
			"mulhu x26, x26, x14\n"
			"fnot.pi f10, f20\n"
			"add x28, x27, x28\n"
			"LBL_SEQID_2_FP_TRANS_ID_62_HID_0:\n"
			"la x10, rand_ieee754_32\n"
			"flw.ps f10, 608(x10)\n"
			"fexp.ps f7, f10\n" 
			"fsgnj.s f5, f8, f6\n"
			"fsll.pi f8, f30, f20\n"
			"fsgnj.s f7, f5, f9\n"
			"bne x23, x14, 1f\n"
			"1:\n"
			"fsgnj.ps f27, f4, f30\n"
			"fle.pi f8, f8, f6\n"
			"fand.pi f30, f1, f10\n"
			"fsra.pi f9, f27, f1\n"
			"la x11, rand_ieee754_32\n"
			"flw.ps f6, 992(x11)\n"
			"flw.ps f7, 608(x11)\n"
			"fsgnjx.ps f30, f6, f7\n" 
			"fsub.pi f28, f2, f28\n"
			"fmaxu.pi f4, f16, f13\n"
			"fltu.pi f2, f27, f4\n"
			"flt.pi f4, f5, f27\n"
			"fmulh.pi f27, f2, f27\n"
			"fsrl.pi f27, f5, f27\n"
			"fmadd.ps f27, f27, f13, f13, rmm\n"
			"la x16, rand_ieee754_32\n"
			"flw.ps f30, 608(x16)\n"
			"fmv.x.s x20, f30\n" 
			"fpackrepb.pi f13, f5\n"
			"fsgnjx.ps f27, f8, f7\n"
			"fnmsub.ps f13, f1, f5, f30, rdn\n"
			"fadd.s f7, f8, f7, rmm\n"
			"fmaxu.pi f7, f30, f7\n"
			"bgeu x23, x9, 1f\n"
			"1:\n"
			"fcvt.f16.ps f7, f7\n"
			"fsra.pi f1, f1, f7\n"
			"bge x17, x26, 1f\n"
			"1:\n"
			"la x20, rand_int_32\n"
			"flw.ps f27, 96(x20)\n"
			"flw.ps f30, 64(x20)\n"
			"fsub.pi f27, f27, f30\n" 
			"fmaxu.pi f5, f30, f20\n"
			"fsgnjx.ps f30, f30, f6\n"
			"fswizz.ps f6, f20, 0x4\n"
			"fadd.pi f6, f6, f6\n"
			"fcmov.ps f30, f30, f6, f30\n"
			"fcmov.ps f30, f20, f30, f6\n"
			"fmulh.pi f10, f20, f20\n"
			"bltu x27, x22, 1f\n"
			"1:\n"
			"li x28, 0xab74142f47c7479a\n"
			"sltiu x16, x28, 0x12\n" 
			"subw x10, x11, x28\n"
			"frcp.ps f3, f13\n"
			"fsub.pi f7, f9, f27\n"
			"add x15, x14, x27\n"
			"fltu.pi f9, f3, f28\n"
			"fle.ps f7, f7, f30\n"
			"fmsub.ps f9, f28, f9, f5, rup\n"
			"la x18, rand_ieee754_32\n"
			"flw.ps f4, 896(x18)\n"
			"fclass.ps f7, f4\n" 
			"fbci.pi f27, 0x672c3\n"
			"andi x10, x10, 0xe\n"
			"fadd.pi f20, f7, f5\n"
			"bltu x6, x30, 1f\n"
			"1:\n"
			"fcvt.pw.ps f5, f5, rne\n"
			"fadd.ps f16, f13, f5, rne\n"
			"maskand m1, m1, m2\n"
			"beq x28, x29, 1f\n"
			"1:\n"
			"srai x26, x26, 5\n"
			"li x16, 0xba3bafde05c731e1\n"
			"sltiu x14, x16, 0x1a\n" 
			"auipc x16, 0x12\n"
			"frcp.ps f10, f10\n"
			"sltiu x26, x10, 0x16\n"
			"add x27, x10, x10\n"
			"divu x27, x10, x16\n"
			"blt x6, x7, 1f\n"
			"1:\n"
			"sltu x10, x10, x26\n"
			"ebreak\n"
			"li x14, 0x453fa71835c9ccd8\n"
			"li x10, 0xee4c45441896e9e5\n"
			"add x15, x14, x10\n" 
			"fmul.ps f6, f6, f16, rtz\n"
			"for.pi f16, f6, f5\n"
			"fmin.pi f28, f16, f27\n"
			"blt x23, x24, 1f\n"
			"1:\n"
			"fpackreph.pi f27, f27\n"
			"fsgnjn.ps f16, f27, f30\n"
			"frsq.ps f27, f27\n"
			"beq x0, x15, 1f\n"
			"1:\n"
			"fslli.pi f3, f5, 0x4\n"
			"la x16, rand_ieee754_32\n"
			"flw.ps f5, 896(x16)\n"
			"flw.ps f20, 576(x16)\n"
			"flw.ps f8, 320(x16)\n"
			"fmsub.s f13, f5, f20, f8, rdn\n" 
			"fcvt.ps.pwu f10, f9, rup\n"
			"fandi.pi f20, f1, 0x4a\n"
			"faddi.pi f7, f2, 0x160\n"
			"flt.s x18, f16, f16\n"
			"ebreak\n"
			"fmvz.x.ps x18, f6, 7\n"
			"fmin.pi f3, f4, f16\n"
			"li x15, 0x98a26881cae6aa0a\n"
			"mulhu x10, x15, x15\n" 
			"subw x20, x16, x18\n"
			"bge x21, x26, 1f\n"
			"1:\n"
			"fxor.pi f3, f10, f3\n"
			"or x11, x15, x18\n"
			"addi x20, x0, 0x19\n"
			"remuw x26, x18, x26\n"
			"fcvt.ps.pw f5, f5, rup\n"
			"fadd.ps f16, f2, f7, rtz\n"
			"la x27, rand_ieee754_32\n"
			"flw.ps f9, 384(x27)\n"
			"flw.ps f4, 416(x27)\n"
			"fsgnjx.ps f27, f9, f4\n" 
			"divuw x14, x26, x16\n"
			"beq x6, x11, 1f\n"
			"1:\n"
			"slliw x28, x26, 4\n"
			"fsgnj.ps f13, f30, f10\n"
			"bne x3, x16, 1f\n"
			"1:\n"
			"fnmadd.ps f20, f5, f2, f20, rne\n"
			"bltu x2, x13, 1f\n"
			"1:\n"
			"sraw x18, x26, x10\n"
			"fmax.s f3, f4, f9\n"
			"sll x26, x26, x26\n"
			"la x26, rand_ieee754_32\n"
			"flw.ps f28, 320(x26)\n"
			"fswizz.ps f16, f28, 0x7\n" 
			"fcvt.ps.pwu f5, f4, rdn\n"
			"fnmsub.ps f13, f13, f10, f9, rtz\n"
			"fsll.pi f10, f10, f10\n"
			"fsub.pi f27, f28, f28\n"
			"bge x21, x27, 1f\n"
			"1:\n"
			"ori x20, x14, 0xd\n"
			"addi x26, x15, 0x17\n"
			"fmadd.s f4, f4, f27, f4, rup\n"
			"li x15, 0x740f5bbc9d016e1f\n"
			"slti x27, x15, 0x11\n" 
			"fsat8.pi f13, f16\n"
			"fcvt.pw.ps f20, f4, rdn\n"
			"fmul.ps f28, f6, f27, rtz\n"
			"fandi.pi f10, f5, 0x113\n"
			"fmax.pi f8, f8, f8\n"
			"bne x30, x20, 1f\n"
			"1:\n"
			"fxor.pi f1, f27, f4\n"
			"fnmadd.s f6, f6, f7, f27, rdn\n"
			"li x10, 0xd24e7105c04f30fd\n"
			"li x14, 0xb9659004177810a6\n"
			"sub x10, x10, x14\n" 
			"fmin.pi f5, f5, f27\n"
			"bgeu x28, x14, 1f\n"
			"1:\n"
			"fadd.pi f1, f1, f16\n"
			"fsgnj.ps f16, f20, f10\n"
			"fcvt.pw.ps f7, f7, rup\n"
			"fminu.pi f7, f9, f7\n"
			"fmadd.ps f20, f7, f5, f30, rdn\n"
			"bltu x28, x17, 1f\n"
			"1:\n"
			"slli x27, x27, 2\n"
			"la x14, rand_int_32\n"
			"flw.ps f1, 352(x14)\n"
			"fsatu8.pi f28, f1\n" 
			"andi x26, x11, 0x1f\n"
			"fsrl.pi f27, f20, f2\n"
			"fpackreph.pi f10, f13\n"
			"fmsub.ps f4, f10, f30, f2, rup\n"
			"fmin.s f13, f13, f20\n"
			"blt x31, x22, 1f\n"
			"1:\n"
			"andi x10, x14, 0x18\n"
			"blt x4, x16, 1f\n"
			"1:\n"
			"fmulh.pi f2, f1, f13\n"
			"bltu x5, x0, 1f\n"
			"1:\n"
			"la x10, rand_ieee754_32\n"
			"flw.ps f2, 32(x10)\n"
			"flw.ps f9, 320(x10)\n"
			"fmsub.ps f20, f2, f9, f2, rdn\n" 
			"remu x28, x0, x28\n"
			"fcmov.ps f2, f2, f13, f4\n"
			"fcvt.ps.pwu f4, f27, rup\n"
			"bge x28, x23, 1f\n"
			"1:\n"
			"fltm.pi m2, f20, f27\n"
			"fmax.pi f30, f1, f30\n"
			"mulh x18, x20, x18\n"
			"maskor m5, m5, m2\n"
			"la x10, rand_ieee754_32\n"
			"flw.ps f20, 160(x10)\n"
			"fclass.ps f4, f20\n" 
			"ebreak\n"
			"fsetm.pi m6, f16\n"
			"beq x27, x31, 1f\n"
			"1:\n"
			"flt.s x11, f1, f8\n"
			"fsgnjn.s f9, f7, f8\n"
			"frcp.ps f9, f7\n"
			"fsin.ps f9, f8\n"
			"fmax.ps f10, f30, f10\n"
			"li x26, 0x6619acc6d5aa8eed\n"
			"slti x11, x26, 0x18\n" 
			"packb x27, x0, x14\n"
			"bge x12, x22, 1f\n"
			"1:\n"
			"mul x0, x10, x15\n"
			"fmadd.ps f7, f9, f6, f7, rtz\n"
			"bne x29, x13, 1f\n"
			"1:\n"
			"fcvt.ps.f16 f30, f9\n"
			"bge x12, x19, 1f\n"
			"1:\n"
			"fsgnjn.s f13, f13, f30\n"
			"flt.pi f30, f2, f30\n"
			"flem.ps m1, f30, f8\n"
			"LBL_SEQID_2_M0_WRITE_ID_63_HID_0:\n"
			"flem.ps m5, f30, f8\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0x54ef000f99\n"
			"fsw.ps f0, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"mulhu x18, x16, x18\n"
			"fcmov.ps f4, f6, f10, f9\n"
			"fmsub.s f20, f9, f8, f9, rup\n"
			"mul x28, x28, x28\n"
			"divu x27, x16, x14\n"
			"fadd.s f9, f4, f8, rmm\n"
			"blt x12, x7, 1f\n"
			"1:\n"
			"fminu.pi f9, f9, f2\n"
			"li x18, 0x249cd936d11dce3d\n"
			"srliw x26, x18, 2\n" 
			"fmulh.pi f1, f16, f3\n"
			"flog.ps f4, f20\n"
			"fsrli.pi f13, f9, 0xb\n"
			"fsgnjx.ps f1, f2, f4\n"
			"fmv.x.s x15, f5\n"
			"bge x8, x0, 1f\n"
			"1:\n"
			"fcvt.ps.pw f7, f5, rmm\n"
			"fround.ps f4, f5, rmm\n"
			"la x14, rand_int_32\n"
			"flw.ps f30, 288(x14)\n"
			"flw.ps f7, 64(x14)\n"
			"fsrl.pi f2, f30, f7\n" 
			"mulw x16, x27, x18\n"
			"blt x28, x15, 1f\n"
			"1:\n"
			"fcvt.s.wu f6, x27, rtz\n"
			"sltu x14, x0, x14\n"
			"xori x14, x28, 0x1c\n"
			"addw x0, x10, x0\n"
			"feqm.ps m5, f3, f9\n"
			"bltu x27, x30, 1f\n"
			"1:\n"
			"ori x26, x15, 0x1f\n"
			"bge x30, x11, 1f\n"
			"1:\n"
			"li x20, 0x16613f22e858d8c9\n"
			"li x11, 0x4c8a8abb158188be\n"
			"sltu x20, x20, x11\n" 
			"fcmov.ps f8, f9, f8, f8\n"
			"slli x15, x27, 5\n"
			"fexp.ps f9, f27\n"
			"div x15, x27, x11\n"
			"fmulhu.pi f16, f27, f20\n"
			"fmaxu.pi f6, f9, f8\n"
			"or x0, x0, x0\n"
			"li x10, 0x81d4f724e1a18d90\n"
			"fcvt.s.w f8, x10, rup\n" 
			"fltu.pi f8, f9, f9\n"
			"fcvt.ps.f16 f8, f28\n"
			"sra x14, x14, x14\n"
			"bltu x15, x14, 1f\n"
			"1:\n"
			"srli x18, x14, 4\n"
			"fsetm.pi m2, f9\n"
			"flt.ps f3, f8, f9\n"
			"divu x28, x14, x20\n"
			"LBL_SEQID_2_M0_WRITE_ID_64_HID_0:\n"
			"fltm.ps m2, f27, f6\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"fnmsub.s f2, f30, f13, f2, rdn\n"
			"fsgnj.ps f5, f20, f3\n"
			"fxor.pi f8, f9, f8\n"
			"fcvt.f16.ps f2, f27\n"
			"bltu x27, x30, 1f\n"
			"1:\n"
			"feq.ps f27, f27, f27\n"
			"fcmovm.ps f10, f13, f10\n"
			"fmax.ps f2, f10, f10\n"
			"la x16, rand_ieee754_32\n"
			"flw.ps f27, 512(x16)\n"
			"flw.ps f8, 736(x16)\n"
			"fmin.s f7, f27, f8\n" 
			"xori x16, x16, 0x12\n"
			"fadd.ps f2, f20, f2, rmm\n"
			"bgeu x2, x0, 1f\n"
			"1:\n"
			"fmadd.ps f8, f8, f2, f2, rne\n"
			"divw x16, x16, x20\n"
			"fmadd.s f8, f16, f2, f8, rne\n"
			"bltu x4, x12, 1f\n"
			"1:\n"
			"fmvs.x.ps x16, f20, 3\n"
			"fpackreph.pi f10, f2\n"
			"la x18, rand_ieee754_32\n"
			"flw.ps f4, 192(x18)\n"
			"flw.ps f10, 928(x18)\n"
			"fcmovm.ps f20, f4, f10\n" 
			"fcvt.ps.pwu f7, f8, rne\n"
			"bne x11, x13, 1f\n"
			"1:\n"
			"fle.s x18, f28, f30\n"
			"fcvt.pw.ps f7, f8, rne\n"
			"fle.s x27, f28, f10\n"
			"flt.ps f28, f8, f16\n"
			"fmax.ps f8, f8, f1\n"
			"mulh x11, x20, x28\n"
			"la x10, rand_int_32\n"
			"flw.ps f2, 96(x10)\n"
			"flw.ps f20, 576(x10)\n"
			"fadd.pi f5, f2, f20\n" 
			"fsub.ps f1, f13, f6, rtz\n"
			"fcvt.f16.ps f6, f28\n"
			"remw x18, x28, x28\n"
			"fsgnjn.s f5, f7, f3\n"
			"slli x20, x20, 2\n"
			"frcp.ps f4, f8\n"
			"beq x12, x31, 1f\n"
			"1:\n"
			"fle.ps f2, f7, f2\n"
			"li x18, 0x12c9206f21289d58\n"
			"li x27, 0x13fa4a5c031f37fb\n"
			"mulhsu x11, x18, x27\n" 
			"mova.m.x x26\n"
			"srai x16, x11, 5\n"
			"bge x17, x14, 1f\n"
			"1:\n"
			"srliw x0, x0, 3\n"
			"bltu x18, x15, 1f\n"
			"1:\n"
			"mova.x.m x0\n"
			"fcvt.ps.pw f7, f7, rne\n"
			"fcvt.ps.f16 f7, f10\n"
			"divw x10, x15, x28\n"
			"LBL_SEQID_2_M0_WRITE_ID_65_HID_0:\n"
			"maskand m5, m1, m5\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"faddi.pi f9, f5, 0x1d3\n"
			"ffrc.ps f27, f3\n"
			"fandi.pi f27, f4, 0x10d\n"
			"fexp.ps f8, f27\n"
			"fcmov.ps f8, f6, f8, f13\n"
			"fnmadd.ps f5, f20, f5, f4, rtz\n"
			"bltu x12, x26, 1f\n"
			"1:\n"
			"fcvt.ps.f16 f8, f8\n"
			"li x28, 0x4a971276ff2eb83\n"
			"li x15, 0x873cdd8f1a4d559a\n"
			"packb x27, x28, x15\n" 
			"slli x15, x15, 1\n"
			"fpackreph.pi f6, f3\n"
			"fmul.ps f3, f6, f30, rmm\n"
			"andi x20, x0, 0x1\n"
			"mova.x.m x16\n"
			"fpackreph.pi f5, f3\n"
			"fcvt.ps.pw f30, f30, rup\n"
			"li x18, 0xc9d4c1cb845e1cb0\n"
			"addi x11, x18, 0xd\n" 
			"fnmadd.s f5, f8, f5, f9, rtz\n"
			"srlw x16, x28, x20\n"
			"sll x14, x18, x16\n"
			"fadd.pi f6, f5, f28\n"
			"fnmsub.s f1, f4, f7, f7, rup\n"
			"flt.s x15, f4, f1\n"
			"packb x26, x14, x20\n"
			"mov.m.x m0, x0, 0xFF\n"
			"li x5, 0x30\n"
			"la x31, rand_ieee754_32\n"
			"add x31, x31, x5\n"
			"flw.ps f1, 0(x31)\n"
			"flw.ps f10, 0(x31)\n"
			"flw.ps f13, 6(x31)\n"
			"flw.ps f16, 1(x31)\n"
			"flw.ps f2, 11(x31)\n"
			"flw.ps f20, 9(x31)\n"
			"flw.ps f27, 1(x31)\n"
			"flw.ps f28, 1(x31)\n"
			"flw.ps f3, 10(x31)\n"
			"flw.ps f30, 8(x31)\n"
			"flw.ps f4, 1(x31)\n"
			"flw.ps f5, 13(x31)\n"
			"flw.ps f6, 48(x31)\n"
			"flw.ps f7, 10(x31)\n"
			"flw.ps f8, 8(x31)\n"
			"flw.ps f9, 50(x31)\n"
			"la x31, rand_int_32\n"
			"add x31, x31, x5\n"
			"lw x0, 0(x31)\n"
			"lw x10, 3(x31)\n"
			"lw x11, 2(x31)\n"
			"lw x14, 9(x31)\n"
			"lw x15, 5(x31)\n"
			"lw x16, 12(x31)\n"
			"lw x18, 4(x31)\n"
			"lw x20, 21(x31)\n"
			"lw x26, 25(x31)\n"
			"lw x27, 3(x31)\n"
			"lw x28, 31(x31)\n"
			"li x21, 0x2\n"
			"LBL_SEQID_2_LOOP_SEQ_HID_0:\n"
			"xor x0, x16, x11\n"
			"slliw x16, x16, 2\n"
			"fsgnjx.s f10, f20, f4\n"
			"bltu x14, x0, 1f\n"
			"1:\n"
			"fle.ps f6, f27, f9\n"
			"faddi.pi f9, f9, 0x93\n"
			"fcvt.ps.pwu f1, f20, rdn\n"
			"packb x27, x27, x27\n"
			"fcmovm.ps f6, f28, f20\n"
			"fadd.s f27, f9, f9, rtz\n"
			"srli x10, x16, 5\n"
			"fadd.s f8, f10, f30, rmm\n"
			"sll x11, x27, x10\n"
			"blt x24, x0, 1f\n"
			"1:\n"
			"fpackreph.pi f28, f3\n"
			"fmin.pi f3, f2, f3\n"
			"srli x15, x16, 4\n"
			"fmin.ps f20, f8, f20\n"
			"srli x28, x26, 3\n"
			"fadd.s f5, f5, f2, rne\n"
			"beq x2, x13, 1f\n"
			"1:\n"
			"xor x20, x14, x16\n"
			"flt.ps f1, f9, f10\n"
			"mul x28, x15, x18\n"
			"and x10, x26, x27\n"
			"addw x11, x14, x26\n"
			"fnot.pi f6, f7\n"
			"fsgnjx.ps f4, f28, f27\n"
			"srlw x0, x16, x14\n"
			"fsub.s f3, f3, f13, rmm\n"
			"fsub.pi f13, f10, f13\n"
			"sra x20, x15, x28\n"
			"fmaxu.pi f27, f4, f30\n"
			"fmsub.ps f28, f3, f30, f5, rtz\n"
			"fmsub.s f9, f4, f4, f28, rmm\n"
			"fsgnjn.s f28, f4, f1\n"
			"addw x18, x26, x18\n"
			"beq x3, x30, 1f\n"
			"1:\n"
			"fclass.ps f8, f27\n"
			"and x11, x0, x15\n"
			"sltiu x11, x10, 0x18\n"
			"fsgnjx.s f5, f27, f30\n"
			"fmax.pi f4, f5, f8\n"
			"fmin.ps f5, f8, f20\n"
			"fsgnjn.s f7, f13, f6\n"
			"bge x27, x31, 1f\n"
			"1:\n"
			"fmin.s f3, f3, f9\n"
			"fsrl.pi f2, f3, f16\n"
			"sltiu x18, x18, 0x19\n"
			"fround.ps f10, f10, rmm\n"
			"fle.pi f27, f3, f30\n"
			"sll x28, x28, x18\n"
			"fsub.pi f3, f30, f3\n"
			"remuw x10, x11, x10\n"
			"fmul.pi f4, f2, f30\n"
			"fmax.ps f9, f9, f6\n"
			"fmadd.s f7, f7, f2, f28, rdn\n"
			"fcvt.pw.ps f5, f2, rne\n"
			"sraw x14, x26, x18\n"
			"fsll.pi f7, f2, f16\n"
			"fmadd.ps f5, f28, f16, f5, rmm\n"
			"and x27, x28, x10\n"
			"fnmsub.ps f10, f9, f20, f13, rne\n"
			"bne x12, x25, 1f\n"
			"1:\n"
			"fmin.ps f16, f3, f13\n"
			"fpackrepb.pi f8, f3\n"
			"fmax.pi f13, f3, f2\n"
			"beq x21, x20, 1f\n"
			"1:\n"
			"fcvt.ps.pwu f5, f4, rtz\n"
			"fmadd.ps f5, f1, f5, f8, rne\n"
			"sltiu x10, x15, 0x0\n"
			"fround.ps f9, f30, rtz\n"
			"fnmsub.ps f1, f2, f4, f7, rne\n"
			"fmax.pi f5, f5, f1\n"
			"bgeu x25, x13, 1f\n"
			"1:\n"
			"fnmsub.s f6, f6, f20, f8, rne\n"
			"fmax.pi f7, f8, f6\n"
			"fmax.ps f2, f20, f2\n"
			"fmul.s f3, f6, f2, rdn\n"
			"fmax.pi f20, f6, f20\n"
			"blt x25, x26, 1f\n"
			"1:\n"
			"srliw x16, x27, 4\n"
			"slliw x27, x18, 2\n"
			"fcmov.ps f16, f16, f6, f28\n"
			"fnmadd.s f7, f10, f13, f28, rtz\n"
			"addiw x11, x10, 0x15\n"
			"bne x30, x19, 1f\n"
			"1:\n"
			"fle.ps f13, f2, f1\n"
			"fcvt.ps.pwu f13, f6, rmm\n"
			"sltiu x10, x11, 0x0\n"
			"fle.pi f27, f27, f28\n"
			"addw x15, x10, x0\n"
			"fminu.pi f13, f9, f30\n"
			"for.pi f28, f5, f10\n"
			"sltiu x27, x27, 0xc\n"
			"fsll.pi f5, f5, f5\n"
			"bge x12, x13, 1f\n"
			"1:\n"
			"remu x11, x16, x27\n"
			"srlw x0, x27, x28\n"
			"blt x27, x11, 1f\n"
			"1:\n"
			"fsgnjn.ps f6, f28, f6\n"
			"sraiw x0, x11, 1\n"
			"addi x14, x0, 0x3\n"
			"blt x23, x3, 1f\n"
			"1:\n"
			"fadd.ps f20, f16, f1, rmm\n"
			"fcvt.f16.ps f6, f6\n"
			"fsgnjx.s f3, f16, f3\n"
			"or x18, x26, x18\n"
			"remu x0, x11, x20\n"
			"or x16, x11, x26\n"
			"slli x20, x20, 3\n"
			"fminu.pi f9, f9, f30\n"
			"for.pi f13, f13, f30\n"
			"div x20, x28, x14\n"
			"bltu x20, x8, 1f\n"
			"1:\n"
			"fclass.ps f20, f8\n"
			"fcvt.f16.ps f13, f2\n"
			"fle.pi f30, f10, f4\n"
			"fnmsub.ps f10, f10, f30, f2, rdn\n"
			"fmax.pi f30, f30, f5\n"
			"bgeu x5, x30, 1f\n"
			"1:\n"
			"fnmadd.s f10, f27, f10, f3, rdn\n"
			"sltiu x27, x0, 0x1b\n"
			"fnot.pi f6, f8\n"
			"beq x19, x1, 1f\n"
			"1:\n"
			"fand.pi f2, f27, f7\n"
			"flog.ps f13, f28\n"
			"div x16, x16, x10\n"
			"xori x15, x20, 0x1b\n"
			"feq.ps f13, f10, f1\n"
			"bltu x22, x2, 1f\n"
			"1:\n"
			"andi x26, x16, 0x18\n"
			"or x15, x27, x18\n"
			"fadd.pi f6, f16, f16\n"
			"fcmov.ps f16, f7, f20, f2\n"
			"bne x21, x13, 1f\n"
			"1:\n"
			"frcp.ps f8, f13\n"
			"bge x3, x16, 1f\n"
			"1:\n"
			"mul x20, x27, x18\n"
			"fsatu8.pi f4, f4\n"
			"fcvt.ps.pw f9, f27, rup\n"
			"fmsub.s f27, f10, f9, f28, rtz\n"
			"addi x21, x21, -1\n"
			"bne x21, x0, LBL_SEQID_2_LOOP_SEQ_HID_0\n"
			"li x5, 3\n" 
			"bne x30, x5, LBL_FAIL_HID_0\n"
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f2,  192 (x5)\n"
			"flw.ps f15,  384 (x5)\n"
			"flw.ps f19,  640 (x5)\n"
			"flw.ps f24,  320 (x5)\n"
			"flw.ps f4,  128 (x5)\n"
			"flw.ps f8,  896 (x5)\n"
			"flw.ps f14,  288 (x5)\n"
			"flw.ps f23,  736 (x5)\n"
			"flw.ps f1,  352 (x5)\n"
			"flw.ps f16,  256 (x5)\n"
			"flw.ps f5,  832 (x5)\n"
			"flw.ps f25,  544 (x5)\n"
			"flw.ps f20,  576 (x5)\n"
			"flw.ps f30,  512 (x5)\n"
			"flw.ps f13,  768 (x5)\n"
			"flw.ps f12,  224 (x5)\n"
			"flw.ps f7,  992 (x5)\n"
			"flw.ps f26,  64 (x5)\n"
			"flw.ps f10,  864 (x5)\n"
			"flw.ps f21,  800 (x5)\n"
			"flw.ps f11,  928 (x5)\n"
			"flw.ps f6,  480 (x5)\n"
			"flw.ps f29,  32 (x5)\n"
			"flw.ps f18,  96 (x5)\n"
			"flw.ps f17,  672 (x5)\n"
			"flw.ps f9,  960 (x5)\n"
			"flw.ps f3,  704 (x5)\n"
			"flw.ps f27,  608 (x5)\n"
			"flw.ps f31,  416 (x5)\n"
			"flw.ps f28,  448 (x5)\n"
			"flw.ps f22,  160 (x5)\n"
			"flw.ps f0,  0 (x5)\n"
			"addi x30, x30, 1\n"
			"la x15, rand_int_32\n"
			"flw.ps f16, 512(x15)\n"
			"flw.ps f30, 512(x15)\n"
			"fremu.pi f2, f16, f30\n" 
			"fmax.s f4, f28, f28\n"
			"fnmsub.s f28, f28, f3, f13, rup\n"
			"divw x0, x10, x10\n"
			"bne x0, x31, 1f\n"
			"1:\n"
			"ffrc.ps f9, f28\n"
			"blt x21, x1, 1f\n"
			"1:\n"
			"maskxor m6, m5, m6\n"
			"ecall\n"
			"andi x20, x16, 0x4\n"
			"beq x31, x30, 1f\n"
			"1:\n"
			"xor x0, x20, x10\n"
			"div x10, x16, x10\n"
			"fcvt.ps.f16 f27, f28\n"
			"LBL_SEQID_3_CSR_ID_66_HID_0:\n"
			"li x27, 0x8800008207300007\n"
			"li x31, (0x2 << 6) | 0x0\n"
			"csrw evict_va, x27\n" 
			"csrwi tensor_wait, 6\n" 
			"subw x10, x10, x16\n"
			"or x28, x26, x15\n"
			"blt x28, x20, 1f\n"
			"1:\n"
			"srl x20, x15, x20\n"
			"rem x14, x20, x15\n"
			"srliw x20, x27, 3\n"
			"fsra.pi f4, f6, f3\n"
			"fsgnjn.ps f7, f8, f28\n"
			"fbcx.ps f6, x0\n"
			"addi x16, x16, 0x10\n"
			"fsgnj.ps f9, f8, f9\n"
			"LBL_SEQID_3_INT_AMO_ID_67_HID_0:\n"
			"li x14, 0x812a20800c\n" 
			"amominug.w x18, x26, (x14)\n" 
			"fround.ps f2, f8, rup\n"
			"fsin.ps f2, f2\n"
			"fle.ps f1, f1, f16\n"
			"fmulh.pi f2, f30, f7\n"
			"fandi.pi f5, f16, 0x6d\n"
			"bltu x3, x27, 1f\n"
			"1:\n"
			"fcmov.ps f20, f9, f20, f7\n"
			"fsgnjn.ps f30, f9, f2\n"
			"fmsub.ps f4, f4, f2, f5, rmm\n"
			"fsub.ps f9, f6, f6, rtz\n"
			"fpackrepb.pi f9, f2\n"
			"LBL_SEQID_3_FP_STORE_ID_68_HID_0:\n"
			"li x28, 0x820b08001c\n" 
			"fsw f7, -1690(x28)\n" 
			"fxor.pi f5, f10, f16\n"
			"fsub.s f28, f9, f28, rdn\n"
			"fsrai.pi f2, f7, 0xb\n"
			"fmax.ps f20, f1, f20\n"
			"for.pi f1, f2, f8\n"
			"fmul.ps f28, f28, f9, rup\n"
			"fand.pi f3, f3, f20\n"
			"fclass.ps f20, f9\n"
			"ffrc.ps f10, f9\n"
			"fsrl.pi f30, f30, f1\n"
			"LBL_SEQID_3_FP_AMO_ID_69_HID_0:\n"
			"li x15, 0x81203e0024\n" 
			"la x26, mem_ptr_offset\n"
			"flw.ps f9, 96(x26)\n"
			"famoming.pi f6, f9 (x15)\n" 
			"fsgnj.ps f20, f5, f8\n"
			"fcvt.ps.f16 f8, f8\n"
			"blt x22, x26, 1f\n"
			"1:\n"
			"fsgnj.ps f8, f16, f20\n"
			"fand.pi f20, f5, f20\n"
			"fmadd.ps f4, f20, f5, f5, rne\n"
			"fsat8.pi f8, f28\n"
			"faddi.pi f28, f28, 0x9d\n"
			"fswizz.ps f9, f5, 0x11\n"
			"fsrl.pi f9, f16, f27\n"
			"flog.ps f4, f20\n"
			"LBL_SEQID_3_INT_AMO_ID_70_HID_0:\n"
			"li x28, 0x823e50e62c\n" 
			"amoming.w x27, x11, (x28)\n" 
			"fsin.ps f7, f20\n"
			"blt x18, x27, 1f\n"
			"1:\n"
			"mulhsu x26, x14, x11\n"
			"fbcx.ps f9, x14\n"
			"ffrc.ps f8, f8\n"
			"fsll.pi f6, f4, f27\n"
			"fcvt.f16.ps f8, f9\n"
			"bgeu x16, x20, 1f\n"
			"1:\n"
			"divu x26, x27, x0\n"
			"fence\n"
			"fle.ps f13, f10, f7\n"
			"fcvt.s.w f5, x18, rtz\n"
			"LBL_SEQID_3_FP_LOAD_ID_71_HID_0:\n"
			"li x16, 0x810380063f\n" 
			"fsq2 f30, 1851(x16)\n"
			"fbc.ps f10, 1851(x16)\n" 
			"fsrai.pi f28, f28, 0xd\n"
			"flog.ps f28, f28\n"
			"fnmadd.s f27, f28, f30, f20, rtz\n"
			"fmsub.s f5, f1, f5, f9, rdn\n"
			"fmulh.pi f1, f9, f5\n"
			"fltu.pi f16, f10, f20\n"
			"fpackrepb.pi f7, f10\n"
			"flt.ps f8, f16, f28\n"
			"fcmovm.ps f28, f7, f13\n"
			"fround.ps f4, f8, rmm\n"
			"LBL_SEQID_3_FP_STORE_ID_72_HID_0:\n"
			"li x20, 0x82079a02da\n" 
			"fsw f3, -341(x20)\n" 
			"fcvt.ps.pw f4, f9, rtz\n"
			"fsgnj.ps f1, f28, f7\n"
			"fmsub.s f30, f16, f28, f30, rmm\n"
			"fmulh.pi f3, f3, f7\n"
			"fsrl.pi f1, f30, f1\n"
			"fadd.ps f30, f5, f7, rdn\n"
			"fcmov.ps f2, f2, f16, f8\n"
			"fsgnjx.ps f13, f2, f7\n"
			"fsrli.pi f7, f3, 0x8\n"
			"fmin.s f9, f5, f13\n"
			"LBL_SEQID_3_FP_STORE_ID_73_HID_0:\n"
			"li x26, 0x810c76d803\n" 
			"fsw f6, 1231(x26)\n" 
			"fadd.s f10, f16, f2, rne\n"
			"fsgnj.ps f30, f27, f13\n"
			"fcvt.ps.f16 f27, f6\n"
			"maskor m6, m6, m2\n"
			"fsrai.pi f6, f8, 0xe\n"
			"blt x19, x15, 1f\n"
			"1:\n"
			"fmax.s f16, f2, f20\n"
			"feqm.ps m5, f30, f10\n"
			"ecall\n"
			"frcp.ps f28, f2\n"
			"fcmov.ps f3, f5, f10, f27\n"
			"LBL_SEQID_3_INT_AMO_ID_74_HID_0:\n"
			"li x15, 0x82570e64f8\n" 
			"amoxorl.d x11, x14, (x15)\n" 
			"fadd.pi f3, f16, f27\n"
			"beq x5, x11, 1f\n"
			"1:\n"
			"fmax.pi f28, f27, f13\n"
			"slli x18, x16, 4\n"
			"sraiw x11, x10, 1\n"
			"fbci.pi f13, 0x2bafb\n"
			"fcvt.s.w f1, x27, rup\n"
			"maskpopcz x11, m2\n"
			"mov.m.x m6, x10, 0x7f\n"
			"fnmsub.ps f20, f16, f3, f7, rmm\n"
			"fsatu8.pi f7, f30\n"
			"blt x29, x29, 1f\n"
			"1:\n"
			"LBL_SEQID_3_FP_AMO_ID_75_HID_0:\n"
			"li x14, 0x823e50e62c\n" 
			"la x27, mem_ptr_offset\n"
			"flw.ps f1, 160(x27)\n"
			"famoorg.pi f16, f1 (x14)\n" 
			"fsrli.pi f2, f28, 0x2\n"
			"flt.pi f20, f3, f2\n"
			"fand.pi f10, f13, f16\n"
			"fadd.pi f30, f7, f27\n"
			"fnot.pi f9, f7\n"
			"fsub.pi f9, f20, f3\n"
			"fmax.s f2, f2, f30\n"
			"fminu.pi f10, f8, f2\n"
			"fsub.s f7, f10, f4, rne\n"
			"fsub.s f16, f9, f27, rdn\n"
			"LBL_SEQID_3_FP_GATHER_ID_76_HID_0:\n"
			"li x14, 0x810a64003a\n" 
			"li x10, 0x1792f15a25463eb9\n"
			"fsc32w.ps f15, x10 (x14)\n"
			"fg32h.ps f4, x10 (x14)\n" 
			"fmadd.s f10, f10, f13, f13, rne\n"
			"fpackrepb.pi f3, f7\n"
			"fsin.ps f4, f3\n"
			"fcvt.f16.ps f8, f8\n"
			"fmsub.ps f20, f30, f4, f20, rup\n"
			"fmaxu.pi f20, f8, f6\n"
			"fadd.ps f7, f1, f5, rup\n"
			"fxor.pi f8, f20, f9\n"
			"bge x10, x26, 1f\n"
			"1:\n"
			"fswizz.ps f27, f4, 0x11\n"
			"bgeu x16, x9, 1f\n"
			"1:\n"
			"fcvt.pwu.ps f13, f30, rup\n"
			"bltu x3, x24, 1f\n"
			"1:\n"
			"LBL_SEQID_3_INT_LOAD_ID_77_HID_0:\n"
			"li x11, 0x810a19b856\n" 
			"sd x26, -1858(x11)\n"
			"lw x10, -1858(x11)\n" 
			"fslli.pi f13, f13, 0x8\n"
			"flog.ps f7, f8\n"
			"fnot.pi f1, f6\n"
			"flt.ps f8, f8, f3\n"
			"fadd.ps f6, f13, f28, rne\n"
			"blt x24, x24, 1f\n"
			"1:\n"
			"ffrc.ps f3, f3\n"
			"fswizz.ps f2, f9, 0xe\n"
			"fsgnjx.s f10, f6, f3\n"
			"fsgnjx.s f13, f10, f13\n"
			"flt.ps f2, f16, f9\n"
			"LBL_SEQID_3_FP_AMO_ID_78_HID_0:\n"
			"li x14, 0x82331f0024\n" 
			"la x20, mem_ptr_offset\n"
			"flw.ps f5, 960(x20)\n"
			"famomaxg.pi f1, f5 (x14)\n" 
			"fcmov.ps f27, f6, f3, f28\n"
			"fsgnjn.ps f9, f5, f10\n"
			"bne x8, x3, 1f\n"
			"1:\n"
			"fnot.pi f28, f30\n"
			"fminu.pi f13, f28, f9\n"
			"fcvt.ps.f16 f16, f13\n"
			"fminu.pi f10, f13, f9\n"
			"fltu.pi f8, f13, f6\n"
			"fmul.pi f9, f9, f20\n"
			"fround.ps f9, f9, rmm\n"
			"frcp.ps f3, f5\n"
			"LBL_SEQID_3_FP_G_STORE_ID_79_HID_0:\n"
			"li x16, 0x812a19b840\n" 
			"mov.m.x m0, zero, 0xff\n"
			"fswg.ps f4, (x16)\n" 
			"flt.ps f6, f2, f13\n"
			"bltu x9, x15, 1f\n"
			"1:\n"
			"fcvt.pwu.ps f9, f10, rne\n"
			"fnmsub.s f2, f13, f2, f28, rtz\n"
			"bne x31, x6, 1f\n"
			"1:\n"
			"fnot.pi f2, f9\n"
			"fsgnjx.s f16, f27, f2\n"
			"fadd.pi f8, f5, f6\n"
			"fnmadd.s f6, f27, f6, f8, rdn\n"
			"fsra.pi f2, f7, f2\n"
			"fcmov.ps f20, f28, f6, f28\n"
			"fmulh.pi f4, f3, f28\n"
			"blt x14, x19, 1f\n"
			"1:\n"
			"LBL_SEQID_3_FP_STORE_ID_80_HID_0:\n"
			"li x18, 0x81072204df\n" 
			"fsw.ps f9, -257(x18)\n" 
			"srli x27, x28, 5\n"
			"slt x28, x0, x11\n"
			"fmin.ps f4, f16, f1\n"
			"fexp.ps f3, f20\n"
			"slliw x26, x27, 1\n"
			"fmulh.pi f1, f4, f10\n"
			"fmsub.ps f27, f8, f27, f3, rne\n"
			"fsub.pi f27, f27, f3\n"
			"fcvt.s.w f4, x10, rup\n"
			"addw x14, x16, x14\n"
			"LBL_SEQID_3_INT_L_STORE_ID_81_HID_0:\n"
			"li x10, 0x815a19b854\n" 
			"sbl x20, (x10)\n" 
			"fslli.pi f2, f1, 0xd\n"
			"divuw x26, x16, x20\n"
			"fmulh.pi f20, f20, f8\n"
			"blt x10, x4, 1f\n"
			"1:\n"
			"fnot.pi f28, f10\n"
			"fsrl.pi f2, f20, f8\n"
			"fcvt.ps.pwu f28, f13, rne\n"
			"sltu x0, x14, x26\n"
			"fmadd.s f20, f13, f8, f9, rmm\n"
			"flt.s x28, f1, f16\n"
			"auipc x18, 0x1a\n"
			"LBL_SEQID_3_INT_G_STORE_ID_82_HID_0:\n"
			"li x14, 0x8124c80eb4\n" 
			"sbg x16, (x14)\n" 
			"fsgnjn.ps f27, f27, f4\n"
			"fround.ps f20, f9, rtz\n"
			"fmulhu.pi f9, f3, f20\n"
			"fround.ps f9, f27, rtz\n"
			"fmin.pi f1, f7, f9\n"
			"fcmov.ps f13, f10, f1, f28\n"
			"fmulhu.pi f16, f28, f10\n"
			"fswizz.ps f5, f16, 0x13\n"
			"feq.pi f2, f8, f8\n"
			"feq.pi f7, f20, f7\n"
			"LBL_SEQID_3_FP_L_STORE_ID_83_HID_0:\n"
			"li x28, 0x8248bb9520\n" 
			"mov.m.x m0, zero, 0xff\n"
			"fswl.ps f13, (x28)\n" 
			"slti x0, x27, 0x1d\n"
			"fcvt.ps.f16 f13, f13\n"
			"blt x2, x3, 1f\n"
			"1:\n"
			"fmax.ps f2, f10, f1\n"
			"bgeu x23, x4, 1f\n"
			"1:\n"
			"fsgnj.s f3, f27, f2\n"
			"sltiu x10, x20, 0xe\n"
			"fmin.s f27, f9, f20\n"
			"mulw x28, x18, x28\n"
			"remw x10, x0, x14\n"
			"fexp.ps f13, f7\n"
			"blt x16, x13, 1f\n"
			"1:\n"
			"mova.m.x x18\n"
			"LBL_SEQID_3_FP_STORE_ID_84_HID_0:\n"
			"li x26, 0x82009c0049\n" 
			"fsw f7, -864(x26)\n" 
			"fpackreph.pi f2, f2\n"
			"blt x26, x18, 1f\n"
			"1:\n"
			"frcp.ps f8, f2\n"
			"bltu x25, x0, 1f\n"
			"1:\n"
			"fcmovm.ps f8, f8, f3\n"
			"for.pi f7, f2, f3\n"
			"fsgnjx.s f2, f2, f28\n"
			"fsub.s f20, f2, f28, rtz\n"
			"fmul.pi f7, f3, f28\n"
			"for.pi f3, f3, f3\n"
			"fpackreph.pi f28, f2\n"
			"fsgnj.ps f30, f2, f30\n"
			"LBL_SEQID_3_INT_G_STORE_ID_85_HID_0:\n"
			"li x26, 0x8227300030\n" 
			"sbg x20, (x26)\n" 
			"mulw x16, x15, x26\n"
			"bne x22, x19, 1f\n"
			"1:\n"
			"fsrl.pi f9, f9, f5\n"
			"blt x6, x30, 1f\n"
			"1:\n"
			"slli x10, x28, 0\n"
			"fsrai.pi f9, f5, 0xd\n"
			"fcvt.pw.ps f10, f3, rtz\n"
			"bne x11, x1, 1f\n"
			"1:\n"
			"masknot m2, m2\n"
			"bltu x29, x18, 1f\n"
			"1:\n"
			"fle.ps f27, f3, f30\n"
			"srlw x20, x26, x27\n"
			"srl x10, x14, x0\n"
			"bltu x3, x2, 1f\n"
			"1:\n"
			"fslli.pi f30, f30, 0xa\n"
			"bge x28, x10, 1f\n"
			"1:\n"
			"LBL_SEQID_3_INT_LOAD_ID_86_HID_0:\n"
			"li x18, 0x82031dda6a\n" 
			"sd x20, 125(x18)\n"
			"lb x26, 125(x18)\n" 
			"fxor.pi f3, f13, f30\n"
			"fnmsub.ps f7, f3, f1, f9, rdn\n"
			"bgeu x0, x0, 1f\n"
			"1:\n"
			"fcmovm.ps f5, f13, f5\n"
			"fsat8.pi f7, f8\n"
			"fmax.pi f1, f16, f1\n"
			"bge x31, x15, 1f\n"
			"1:\n"
			"fcvt.pw.ps f9, f13, rne\n"
			"fsat8.pi f2, f30\n"
			"fxor.pi f16, f7, f16\n"
			"fsub.ps f4, f28, f6, rtz\n"
			"fmax.pi f4, f30, f6\n"
			"LBL_SEQID_3_FP_LOAD_ID_87_HID_0:\n"
			"li x10, 0x810a20800d\n" 
			"fsq2 f27, -627(x10)\n"
			"fbc.ps f13, -627(x10)\n" 
			"fsgnjx.ps f2, f28, f6\n"
			"frsq.ps f9, f9\n"
			"blt x18, x16, 1f\n"
			"1:\n"
			"fmul.s f30, f6, f1, rne\n"
			"fcmov.ps f3, f6, f3, f30\n"
			"fmul.pi f20, f1, f6\n"
			"fandi.pi f16, f1, 0x84\n"
			"fsgnjx.ps f30, f7, f7\n"
			"fsrl.pi f6, f9, f6\n"
			"fnot.pi f4, f4\n"
			"bge x28, x2, 1f\n"
			"1:\n"
			"fpackrepb.pi f4, f2\n"
			"LBL_SEQID_3_INT_AMO_ID_88_HID_0:\n"
			"li x16, 0x813e5cd3f8\n" 
			"amomaxug.w x10, x10, (x16)\n" 
			"fmaxu.pi f8, f4, f2\n"
			"fmax.s f28, f4, f2\n"
			"frcp.ps f27, f20\n"
			"fclass.ps f30, f8\n"
			"ffrc.ps f28, f3\n"
			"fsrai.pi f8, f4, 0x0\n"
			"srlw x27, x15, x27\n"
			"and x0, x0, x10\n"
			"blt x26, x1, 1f\n"
			"1:\n"
			"fsgnjn.ps f20, f6, f20\n"
			"blt x0, x29, 1f\n"
			"1:\n"
			"divuw x16, x0, x26\n"
			"bltu x10, x30, 1f\n"
			"1:\n"
			"LBL_SEQID_3_INT_AMO_ID_89_HID_0:\n"
			"li x18, 0x823ab48828\n" 
			"amominug.d x11, x10, (x18)\n" 
			"fltm.ps m6, f8, f5\n"
			"fmaxu.pi f10, f5, f5\n"
			"addiw x10, x16, 0x0\n"
			"mulhsu x0, x16, x16\n"
			"fsub.pi f30, f8, f30\n"
			"fcvt.s.wu f13, x26, rdn\n"
			"flt.pi f5, f30, f8\n"
			"fmsub.s f10, f8, f13, f13, rne\n"
			"fsatu8.pi f8, f8\n"
			"feqm.ps m1, f30, f13\n"
			"LBL_SEQID_3_INT_AMO_ID_90_HID_0:\n"
			"li x28, 0x82426c0708\n" 
			"amoandl.d x11, x18, (x28)\n" 
			"fmvs.x.ps x28, f3, 5\n"
			"fmvz.x.ps x16, f4, 1\n"
			"feq.s x0, f20, f10\n"
			"fsetm.pi m2, f1\n"
			"for.pi f5, f4, f10\n"
			"feq.s x14, f2, f1\n"
			"bge x6, x24, 1f\n"
			"1:\n"
			"fmin.ps f8, f3, f2\n"
			"fsra.pi f1, f13, f30\n"
			"fmin.s f30, f30, f3\n"
			"blt x0, x7, 1f\n"
			"1:\n"
			"maskpopcz x20, m6\n"
			"LBL_SEQID_3_FP_SCATTER_ID_91_HID_0:\n"
			"li x27, 0x8207300033\n" 
			"la x11, mem_ptr_offset\n"
			"flw.ps f9, 108(x11)\n"
			"fscb.ps f7, f9 (x27)\n" 
			"fltm.pi m6, f28, f13\n"
			"sllw x15, x11, x15\n"
			"fmax.ps f1, f4, f1\n"
			"fltm.pi m1, f9, f27\n"
			"fnmsub.ps f1, f13, f4, f1, rtz\n"
			"fmin.s f16, f9, f9\n"
			"srli x10, x26, 4\n"
			"fsat8.pi f3, f20\n"
			"auipc x20, 0x10\n"
			"bne x22, x27, 1f\n"
			"1:\n"
			"fmsub.ps f27, f13, f27, f27, rdn\n"
			"LBL_SEQID_3_INT_LOAD_ID_92_HID_0:\n"
			"li x20, 0x8104c80eb7\n" 
			"sd x16, 1271(x20)\n"
			"lw x14, 1271(x20)\n" 
			"subw x10, x10, x16\n"
			"sra x28, x28, x28\n"
			"mova.x.m x10\n"
			"sra x28, x11, x26\n"
			"blt x12, x0, 1f\n"
			"1:\n"
			"srliw x26, x26, 1\n"
			"fmsub.ps f6, f8, f6, f5, rmm\n"
			"frcp.ps f8, f1\n"
			"fsgnjn.ps f10, f13, f27\n"
			"blt x12, x17, 1f\n"
			"1:\n"
			"slliw x16, x14, 4\n"
			"beq x6, x10, 1f\n"
			"1:\n"
			"sraiw x11, x0, 4\n"
			"LBL_SEQID_3_FP_SCATTER_ID_93_HID_0:\n"
			"li x15, 0x82062003f6\n" 
			"li x18, 0xe09dbabf3d73f01a\n"
			"fsc32h.ps f10, x18 (x15)\n" 
			"fcvt.ps.pwu f28, f30, rdn\n"
			"fmin.s f13, f9, f7\n"
			"fcmov.ps f1, f2, f4, f13\n"
			"fpackrepb.pi f10, f6\n"
			"fnot.pi f1, f6\n"
			"fmul.s f9, f8, f10, rtz\n"
			"fmin.s f27, f20, f27\n"
			"fmul.ps f2, f2, f20, rne\n"
			"flt.ps f28, f8, f7\n"
			"blt x0, x8, 1f\n"
			"1:\n"
			"flt.ps f7, f6, f8\n"
			"LBL_SEQID_3_FP_L_GATHER_ID_94_HID_0:\n"
			"li x18, 0x815a19b854\n" 
			"la x11, mem_ptr_offset\n"
			"flw.ps f30, 84(x11)\n"
			"fscwl.ps f30, f30 (x18)\n"
			"fghl.ps f20, f30 (x18)\n" 
			"fcvt.pw.ps f10, f6, rtz\n"
			"fcmov.ps f10, f2, f30, f10\n"
			"fmaxu.pi f1, f6, f6\n"
			"fsgnj.ps f6, f6, f3\n"
			"fmul.s f9, f10, f7, rdn\n"
			"fsub.s f9, f7, f30, rup\n"
			"fsgnjn.ps f5, f10, f10\n"
			"frcp.ps f10, f7\n"
			"bltu x21, x29, 1f\n"
			"1:\n"
			"fadd.s f30, f30, f16, rtz\n"
			"fadd.pi f1, f6, f16\n"
			"LBL_SEQID_3_INT_AMO_ID_95_HID_0:\n"
			"li x16, 0x812a20800c\n" 
			"amoaddg.w x14, x28, (x16)\n" 
			"remw x10, x16, x10\n"
			"subw x27, x27, x14\n"
			"and x28, x26, x26\n"
			"bne x9, x2, 1f\n"
			"1:\n"
			"fnmadd.ps f30, f28, f13, f30, rdn\n"
			"sllw x28, x10, x18\n"
			"flog.ps f13, f13\n"
			"and x26, x18, x26\n"
			"remuw x16, x15, x15\n"
			"flt.s x10, f9, f7\n"
			"fmv.x.s x0, f20\n"
			"LBL_SEQID_3_INT_STORE_ID_96_HID_0:\n"
			"li x11, 0x810ab0050a\n" 
			"sb x28, -97(x11)\n" 
			"fminu.pi f30, f30, f9\n"
			"blt x11, x22, 1f\n"
			"1:\n"
			"srliw x11, x28, 2\n"
			"fsrl.pi f16, f16, f7\n"
			"srliw x20, x11, 0\n"
			"fcvt.ps.pw f30, f28, rne\n"
			"bltu x19, x20, 1f\n"
			"1:\n"
			"mova.x.m x28\n"
			"remuw x16, x20, x15\n"
			"fnmsub.ps f30, f2, f13, f4, rup\n"
			"fcmovm.ps f16, f5, f1\n"
			"mul x26, x28, x0\n"
			"mov.m.x m0, x0, 0xFF\n"
			"li x5, 0x80\n"
			"la x31, rand_ieee754_32\n"
			"add x31, x31, x5\n"
			"flw.ps f10, 0(x31)\n"
			"flw.ps f2, 1(x31)\n"
			"flw.ps f28, 8(x31)\n"
			"flw.ps f5, 6(x31)\n"
			"flw.ps f7, 6(x31)\n"
			"la x31, rand_int_32\n"
			"add x31, x31, x5\n"
			"lw x0, 0(x31)\n"
			"lw x10, 2(x31)\n"
			"lw x14, 1(x31)\n"
			"lw x15, 4(x31)\n"
			"lw x16, 11(x31)\n"
			"lw x26, 15(x31)\n"
			"lw x27, 24(x31)\n"
			"li x21, 0x3\n"
			"LBL_SEQID_3_LOOP_SEQ_HID_0:\n"
			"fsgnj.s f16, f7, f28\n"
			"bge x7, x20, 1f\n"
			"1:\n"
			"srl x20, x26, x27\n"
			"bgeu x16, x14, 1f\n"
			"1:\n"
			"sltiu x26, x10, 0x7\n"
			"sllw x10, x10, x16\n"
			"bne x20, x11, 1f\n"
			"1:\n"
			"fcvt.f16.ps f30, f5\n"
			"frsq.ps f4, f5\n"
			"fnmadd.ps f10, f5, f5, f2, rup\n"
			"feq.ps f8, f2, f2\n"
			"addi x11, x15, 0x17\n"
			"fmax.pi f2, f5, f10\n"
			"fsgnj.s f7, f5, f28\n"
			"mulhsu x14, x26, x14\n"
			"fcmov.ps f2, f2, f28, f28\n"
			"div x18, x0, x10\n"
			"fnmsub.ps f6, f5, f10, f5, rmm\n"
			"fswizz.ps f30, f2, 0xe\n"
			"slt x26, x15, x26\n"
			"slliw x26, x15, 3\n"
			"fmsub.ps f28, f7, f5, f28, rne\n"
			"bltu x16, x5, 1f\n"
			"1:\n"
			"flem.ps m1, f2, f28\n"
			"bltu x22, x31, 1f\n"
			"1:\n"
			"fbcx.ps f4, x16\n"
			"fslli.pi f6, f5, 0xe\n"
			"bne x7, x23, 1f\n"
			"1:\n"
			"ebreak\n"
			"fmadd.ps f10, f10, f28, f10, rdn\n"
			"blt x19, x12, 1f\n"
			"1:\n"
			"frsq.ps f2, f7\n"
			"add x26, x26, x26\n"
			"slli x26, x26, 5\n"
			"fmadd.ps f30, f2, f5, f7, rup\n"
			"ffrc.ps f2, f28\n"
			"ecall\n"
			"flt.s x0, f10, f28\n"
			"bne x27, x11, 1f\n"
			"1:\n"
			"fmadd.s f6, f28, f5, f2, rmm\n"
			"fmulhu.pi f28, f2, f5\n"
			"fltm.ps m2, f7, f2\n"
			"flt.pi f4, f10, f2\n"
			"fsrai.pi f6, f28, 0xe\n"
			"fsub.s f7, f10, f7, rtz\n"
			"add x20, x15, x14\n"
			"fcvt.pwu.ps f8, f5, rdn\n"
			"blt x2, x4, 1f\n"
			"1:\n"
			"or x20, x16, x15\n"
			"fmin.ps f28, f7, f5\n"
			"masknot m6, m6\n"
			"fmul.ps f16, f7, f10, rmm\n"
			"beq x22, x4, 1f\n"
			"1:\n"
			"maskpopcz x11, m2\n"
			"mova.x.m x14\n"
			"mulhsu x11, x27, x16\n"
			"fmaxu.pi f10, f7, f7\n"
			"fmin.ps f28, f28, f2\n"
			"fnmadd.ps f28, f7, f28, f7, rtz\n"
			"fmax.ps f10, f10, f2\n"
			"fandi.pi f5, f10, 0x77\n"
			"fslli.pi f7, f7, 0xd\n"
			"fcvt.wu.s x14, f28, rmm\n"
			"fsgnjn.ps f7, f7, f5\n"
			"fcvt.s.w f10, x14, rtz\n"
			"fnot.pi f2, f2\n"
			"fsgnjn.s f28, f10, f28\n"
			"fmulhu.pi f30, f7, f5\n"
			"slli x26, x15, 0\n"
			"add x11, x27, x16\n"
			"fsat8.pi f6, f2\n"
			"sllw x10, x14, x26\n"
			"addw x26, x26, x27\n"
			"bgeu x5, x5, 1f\n"
			"1:\n"
			"fmul.pi f2, f7, f5\n"
			"fmulh.pi f7, f2, f7\n"
			"ebreak\n"
			"fmul.s f16, f2, f5, rdn\n"
			"sra x26, x15, x27\n"
			"ebreak\n"
			"sltu x11, x10, x16\n"
			"and x11, x14, x27\n"
			"divu x26, x26, x27\n"
			"fmadd.ps f8, f5, f28, f10, rdn\n"
			"fle.pi f2, f10, f2\n"
			"maskand m1, m1, m6\n"
			"fsin.ps f2, f2\n"
			"feq.s x26, f10, f10\n"
			"fadd.pi f2, f2, f2\n"
			"bltu x16, x12, 1f\n"
			"1:\n"
			"fnot.pi f10, f2\n"
			"bne x31, x17, 1f\n"
			"1:\n"
			"fsra.pi f4, f5, f10\n"
			"fcmov.ps f5, f2, f5, f2\n"
			"fcvt.f16.ps f6, f5\n"
			"sraw x0, x0, x26\n"
			"fmvz.x.ps x0, f5, 7\n"
			"fmul.ps f28, f10, f10, rup\n"
			"sllw x18, x16, x10\n"
			"addw x26, x27, x27\n"
			"fnot.pi f2, f28\n"
			"fsgnjn.s f28, f7, f28\n"
			"feqm.ps m5, f2, f10\n"
			"blt x18, x10, 1f\n"
			"1:\n"
			"fpackrepb.pi f30, f2\n"
			"xori x16, x16, 0xf\n"
			"fmax.pi f4, f5, f5\n"
			"bne x4, x9, 1f\n"
			"1:\n"
			"flt.pi f28, f5, f10\n"
			"fsrl.pi f4, f10, f10\n"
			"fclass.ps f6, f2\n"
			"bltu x1, x9, 1f\n"
			"1:\n"
			"fle.pi f8, f10, f28\n"
			"divuw x16, x16, x27\n"
			"addi x15, x15, 0x8\n"
			"fsat8.pi f4, f10\n"
			"slli x26, x26, 0\n"
			"bltu x25, x2, 1f\n"
			"1:\n"
			"fbcx.ps f28, x15\n"
			"bgeu x25, x4, 1f\n"
			"1:\n"
			"sraw x10, x10, x14\n"
			"sltu x10, x16, x14\n"
			"feq.pi f2, f2, f5\n"
			"feq.s x26, f7, f2\n"
			"srai x18, x26, 4\n"
			"subw x0, x15, x0\n"
			"for.pi f28, f10, f28\n"
			"fmv.x.s x11, f28\n"
			"fle.pi f28, f5, f7\n"
			"fminu.pi f2, f7, f2\n"
			"blt x1, x6, 1f\n"
			"1:\n"
			"fpackrepb.pi f10, f10\n"
			"packb x10, x15, x10\n"
			"feq.pi f10, f5, f5\n"
			"beq x16, x14, 1f\n"
			"1:\n"
			"ecall\n"
			"fle.s x0, f10, f2\n"
			"fmax.s f30, f2, f7\n"
			"fsll.pi f30, f10, f7\n"
			"mulhsu x11, x16, x26\n"
			"bltu x10, x22, 1f\n"
			"1:\n"
			"fnmadd.ps f28, f10, f28, f10, rup\n"
			"fslli.pi f16, f7, 0x8\n"
			"srliw x16, x16, 3\n"
			"fcvt.pwu.ps f30, f2, rtz\n"
			"bltu x28, x17, 1f\n"
			"1:\n"
			"fbci.ps f30, 0x47a92\n"
			"mulhsu x27, x15, x27\n"
			"andi x0, x0, 0x4\n"
			"fltu.pi f28, f28, f28\n"
			"masknot m1, m6\n"
			"fcvt.ps.pwu f10, f7, rup\n"
			"maskand m1, m5, m1\n"
			"blt x2, x6, 1f\n"
			"1:\n"
			"fsub.s f6, f5, f5, rne\n"
			"sraw x20, x26, x27\n"
			"fnmsub.s f2, f2, f2, f5, rne\n"
			"frcp.ps f28, f5\n"
			"fmin.pi f5, f5, f10\n"
			"srli x26, x27, 0\n"
			"or x10, x10, x16\n"
			"sraiw x18, x27, 2\n"
			"fsin.ps f4, f7\n"
			"fbci.ps f2, 0x1dbf1\n"
			"xori x27, x27, 0x4\n"
			"fsat8.pi f10, f5\n"
			"blt x15, x16, 1f\n"
			"1:\n"
			"maskand m2, m2, m1\n"
			"flt.s x0, f5, f10\n"
			"fle.s x11, f7, f7\n"
			"srai x11, x10, 5\n"
			"fsgnjn.s f2, f7, f10\n"
			"sub x0, x15, x26\n"
			"fcvt.wu.s x0, f28, rtz\n"
			"addi x21, x21, -1\n"
			"bne x21, x0, LBL_SEQID_3_LOOP_SEQ_HID_0\n"
			"li x5, 4\n" 
			"bne x30, x5, LBL_FAIL_HID_0\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_4_INT_AMO_ID_97_HID_0:\n"
			"li x11, 0x825e50e628\n" 
			"amominul.d x16, x18, (x11)\n" 
			"fnmadd.ps f2, f20, f20, f27, rmm\n"
			"fmadd.ps f28, f20, f27, f27, rmm\n"
			"fnmsub.s f27, f27, f4, f28, rne\n"
			"feq.ps f28, f4, f4\n"
			"flt.ps f2, f4, f27\n"
			"LBL_SEQID_4_FP_AMO_ID_98_HID_0:\n"
			"li x14, 0x815486003c\n" 
			"la x27, mem_ptr_offset\n"
			"flw.ps f3, 768(x27)\n"
			"famominl.ps f1, f3 (x14)\n" 
			"fle.pi f7, f8, f7\n"
			"bgeu x11, x23, 1f\n"
			"1:\n"
			"fand.pi f5, f6, f7\n"
			"fclass.ps f5, f20\n"
			"fsll.pi f3, f7, f3\n"
			"fltu.pi f20, f6, f20\n"
			"LBL_SEQID_4_FP_AMO_ID_99_HID_0:\n"
			"li x15, 0x812b920038\n" 
			"la x11, mem_ptr_offset\n"
			"flw.ps f10, 224(x11)\n"
			"famoxorg.pi f10, f10 (x15)\n" 
			"fmin.s f4, f9, f13\n"
			"beq x16, x14, 1f\n"
			"1:\n"
			"feq.ps f8, f3, f2\n"
			"fmadd.s f4, f10, f20, f10, rtz\n"
			"fsrai.pi f28, f28, 0x0\n"
			"fnmsub.s f1, f5, f3, f20, rdn\n"
			"LBL_SEQID_4_INT_AMO_ID_100_HID_0:\n"
			"li x16, 0x82262003f4\n" 
			"amomaxug.w x10, x28, (x16)\n" 
			"fsatu8.pi f27, f8\n"
			"fsin.ps f3, f8\n"
			"fminu.pi f28, f2, f8\n"
			"fcvt.pw.ps f3, f8, rup\n"
			"fcvt.ps.pwu f10, f2, rne\n"
			"LBL_SEQID_4_FP_L_GATHER_ID_101_HID_0:\n"
			"li x15, 0x814ab00508\n" 
			"la x18, mem_ptr_offset\n"
			"flq2 f27, 20(x18)\n"
			"fscwl.ps f31, f27 (x15)\n"
			"fghl.ps f3, f27 (x15)\n" 
			"fmul.s f7, f7, f28, rdn\n"
			"bltu x13, x17, 1f\n"
			"1:\n"
			"fcmovm.ps f9, f28, f27\n"
			"flt.ps f28, f28, f10\n"
			"fbcx.ps f16, x10\n"
			"maskand m6, m5, m6\n"
			"LBL_SEQID_4_INT_STORE_ID_102_HID_0:\n"
			"li x28, 0x82079a02da\n" 
			"sd x26, -1006(x28)\n" 
			"fsra.pi f4, f27, f8\n"
			"auipc x20, 0x19\n"
			"fclass.s x28, f30\n"
			"mulhsu x0, x28, x16\n"
			"fsgnjn.ps f8, f7, f2\n"
			"LBL_SEQID_4_INT_AMO_ID_103_HID_0:\n"
			"li x14, 0x81203e0020\n" 
			"amomaxg.d x26, x16, (x14)\n" 
			"fmax.pi f6, f20, f10\n"
			"bgeu x6, x30, 1f\n"
			"1:\n"
			"fsgnjx.ps f27, f20, f10\n"
			"fround.ps f10, f10, rup\n"
			"bne x13, x28, 1f\n"
			"1:\n"
			"flt.ps f3, f10, f3\n"
			"fsgnjx.s f16, f9, f28\n"
			"LBL_SEQID_4_FP_STORE_ID_104_HID_0:\n"
			"li x27, 0x8104c80eb7\n" 
			"fsw.ps f28, 193(x27)\n" 
			"fmul.ps f20, f20, f30, rdn\n"
			"fcmov.ps f2, f10, f13, f30\n"
			"fnmsub.ps f27, f16, f20, f30, rup\n"
			"fcmovm.ps f20, f1, f30\n"
			"fminu.pi f13, f10, f20\n"
			"bgeu x3, x29, 1f\n"
			"1:\n"
			"LBL_SEQID_4_FP_AMO_ID_105_HID_0:\n"
			"li x18, 0x825e50e62c\n" 
			"la x28, mem_ptr_offset\n"
			"flw.ps f3, 800(x28)\n"
			"famominul.pi f16, f3 (x18)\n" 
			"feq.ps f13, f10, f13\n"
			"fswizz.ps f1, f7, 0x16\n"
			"fsub.s f9, f9, f8, rtz\n"
			"fsrai.pi f1, f1, 0xf\n"
			"beq x5, x2, 1f\n"
			"1:\n"
			"fclass.ps f20, f9\n"
			"LBL_SEQID_4_FP_STORE_ID_106_HID_0:\n"
			"li x18, 0x810da6068f\n" 
			"fsq2 f20, -480(x18)\n" 
			"mova.m.x x14\n"
			"ecall\n"
			"slli x20, x15, 4\n"
			"subw x10, x10, x20\n"
			"fswizz.ps f9, f30, 0x1d\n"
			"LBL_SEQID_4_INT_AMO_ID_107_HID_0:\n"
			"li x28, 0x82409c0048\n" 
			"amominul.d x16, x26, (x28)\n" 
			"fsrai.pi f9, f16, 0x5\n"
			"fle.pi f16, f30, f16\n"
			"fle.ps f30, f3, f30\n"
			"fmsub.s f20, f30, f20, f16, rtz\n"
			"fmulhu.pi f30, f10, f30\n"
			"LBL_SEQID_4_INT_AMO_ID_108_HID_0:\n"
			"li x26, 0x815b920038\n" 
			"amoorl.w x16, x14, (x26)\n" 
			"fmax.ps f5, f6, f16\n"
			"fminu.pi f27, f8, f6\n"
			"bge x21, x3, 1f\n"
			"1:\n"
			"fcvt.ps.pw f16, f9, rtz\n"
			"fcvt.f16.ps f5, f9\n"
			"fmin.s f7, f9, f16\n"
			"bgeu x14, x0, 1f\n"
			"1:\n"
			"LBL_SEQID_4_INT_LOAD_ID_109_HID_0:\n"
			"li x26, 0x810b0f4812\n" 
			"sd x18, 357(x26)\n"
			"lwu x16, 357(x26)\n" 
			"fsrl.pi f8, f28, f3\n"
			"fand.pi f7, f30, f20\n"
			"fexp.ps f4, f4\n"
			"fpackrepb.pi f5, f5\n"
			"blt x24, x19, 1f\n"
			"1:\n"
			"fmaxu.pi f28, f28, f5\n"
			"LBL_SEQID_4_INT_LOAD_ID_110_HID_0:\n"
			"li x20, 0x820de80a5f\n" 
			"sd x16, 1011(x20)\n"
			"lbu x26, 1011(x20)\n" 
			"fmul.pi f5, f20, f2\n"
			"fxor.pi f10, f9, f9\n"
			"fpackreph.pi f10, f9\n"
			"bne x8, x24, 1f\n"
			"1:\n"
			"fadd.ps f10, f20, f27, rtz\n"
			"bne x0, x8, 1f\n"
			"1:\n"
			"fle.pi f5, f13, f6\n"
			"LBL_SEQID_4_FP_AMO_ID_111_HID_0:\n"
			"li x16, 0x813486003c\n" 
			"la x15, mem_ptr_offset\n"
			"flw.ps f10, 704(x15)\n"
			"famoswapg.pi f5, f10 (x16)\n" 
			"fandi.pi f16, f5, 0x188\n"
			"fsrli.pi f9, f30, 0x2\n"
			"fsrai.pi f3, f3, 0x3\n"
			"fnmadd.s f30, f28, f3, f13, rne\n"
			"beq x30, x10, 1f\n"
			"1:\n"
			"fsrai.pi f4, f6, 0x6\n"
			"LBL_SEQID_4_INT_LOAD_ID_112_HID_0:\n"
			"li x14, 0x820e50e62c\n" 
			"sd x27, -1941(x14)\n"
			"lw x14, -1941(x14)\n" 
			"fand.pi f20, f20, f3\n"
			"frcp.ps f1, f2\n"
			"fpackrepb.pi f28, f20\n"
			"fmadd.ps f9, f9, f28, f9, rmm\n"
			"fmax.s f4, f1, f13\n"
			"LBL_SEQID_4_INT_AMO_ID_113_HID_0:\n"
			"li x11, 0x812ab00508\n" 
			"amoandg.d x14, x27, (x11)\n" 
			"fmv.s.x f1, x14\n"
			"fmax.s f20, f10, f1\n"
			"mul x15, x28, x20\n"
			"fmin.ps f30, f7, f1\n"
			"bgeu x0, x26, 1f\n"
			"1:\n"
			"feqm.ps m2, f30, f9\n"
			"LBL_SEQID_4_FP_LOAD_ID_114_HID_0:\n"
			"li x28, 0x820e94a81e\n" 
			"fsw.ps f1, 392(x28)\n"
			"flw f2, 392(x28)\n" 
			"fexp.ps f13, f4\n"
			"bne x12, x23, 1f\n"
			"1:\n"
			"fand.pi f28, f3, f30\n"
			"fmsub.ps f28, f30, f9, f3, rne\n"
			"fsgnjn.s f13, f2, f13\n"
			"mulh x10, x27, x11\n"
			"beq x18, x3, 1f\n"
			"1:\n"
			"LBL_SEQID_4_INT_AMO_ID_115_HID_0:\n"
			"li x15, 0x8153800638\n" 
			"amoswapl.d x20, x20, (x15)\n" 
			"subw x28, x15, x15\n"
			"remuw x27, x0, x27\n"
			"srliw x16, x15, 3\n"
			"bge x28, x26, 1f\n"
			"1:\n"
			"lui x18, 0x1\n"
			"fsra.pi f27, f27, f27\n"
			"LBL_SEQID_4_FP_GATHER_ID_116_HID_0:\n"
			"li x20, 0x810a64003a\n" 
			"la x28, mem_ptr_offset\n"
			"flq2 f3, 188(x28)\n"
			"fscw.ps f0, f3 (x20)\n"
			"fgb.ps f27, f3 (x20)\n" 
			"fsra.pi f30, f20, f3\n"
			"fsra.pi f2, f4, f8\n"
			"fmul.s f9, f9, f3, rup\n"
			"fsub.pi f8, f8, f20\n"
			"fsub.s f28, f4, f2, rmm\n"
			"beq x19, x27, 1f\n"
			"1:\n"
			"LBL_SEQID_4_INT_LOAD_ID_117_HID_0:\n"
			"li x28, 0x820f90000f\n" 
			"sd x16, 1670(x28)\n"
			"lb x14, 1670(x28)\n" 
			"fexp.ps f4, f20\n"
			"fsrli.pi f6, f2, 0x6\n"
			"bge x22, x24, 1f\n"
			"1:\n"
			"feq.pi f8, f20, f8\n"
			"bltu x11, x6, 1f\n"
			"1:\n"
			"fsin.ps f8, f7\n"
			"ffrc.ps f8, f9\n"
			"LBL_SEQID_4_FP_AMO_ID_118_HID_0:\n"
			"li x16, 0x82226c0708\n" 
			"la x28, mem_ptr_offset\n"
			"flw.ps f3, 864(x28)\n"
			"famoxorg.pi f2, f3 (x16)\n" 
			"fsin.ps f20, f1\n"
			"fsgnjx.ps f30, f6, f3\n"
			"fnmsub.ps f27, f8, f10, f13, rtz\n"
			"fmadd.s f30, f28, f27, f10, rup\n"
			"fexp.ps f10, f30\n"
			"LBL_SEQID_4_FP_AMO_ID_119_HID_0:\n"
			"li x10, 0x8154ac0034\n" 
			"la x18, mem_ptr_offset\n"
			"flw.ps f9, 160(x18)\n"
			"famoandl.pi f27, f9 (x10)\n" 
			"maskor m5, m6, m5\n"
			"fsat8.pi f10, f16\n"
			"fcmovm.ps f3, f2, f3\n"
			"ffrc.ps f20, f30\n"
			"beq x15, x22, 1f\n"
			"1:\n"
			"fmul.s f10, f2, f2, rtz\n"
			"LBL_SEQID_4_FP_L_GATHER_ID_120_HID_0:\n"
			"li x27, 0x814a640038\n" 
			"la x10, mem_ptr_offset\n"
			"flq2 f3, 212(x10)\n"
			"fscwl.ps f12, f3 (x27)\n"
			"fgwl.ps f10, f3 (x27)\n" 
			"fence\n"
			"sra x11, x28, x14\n"
			"bgeu x30, x26, 1f\n"
			"1:\n"
			"fence\n"
			"fround.ps f9, f10, rdn\n"
			"beq x28, x29, 1f\n"
			"1:\n"
			"fsll.pi f9, f13, f1\n"
			"blt x27, x30, 1f\n"
			"1:\n"
			"LBL_SEQID_4_INT_AMO_ID_121_HID_0:\n"
			"li x26, 0x815b920038\n" 
			"amomaxul.d x27, x18, (x26)\n" 
			"fand.pi f8, f9, f9\n"
			"fsgnjn.s f8, f8, f30\n"
			"fcvt.ps.pw f13, f30, rtz\n"
			"fmax.s f7, f30, f9\n"
			"feq.pi f6, f9, f8\n"
			"LBL_SEQID_4_FP_AMO_ID_122_HID_0:\n"
			"li x27, 0x82431dda68\n" 
			"la x20, mem_ptr_offset\n"
			"flw.ps f1, 288(x20)\n"
			"famoswapl.pi f5, f1 (x27)\n" 
			"fmax.s f13, f9, f6\n"
			"ecall\n"
			"fmaxu.pi f30, f9, f1\n"
			"packb x10, x18, x15\n"
			"fltm.ps m6, f20, f30\n"
			"LBL_SEQID_4_INT_AMO_ID_123_HID_0:\n"
			"li x15, 0x8248bb9538\n" 
			"amoswapl.d x11, x27, (x15)\n" 
			"fcvt.ps.pw f30, f16, rup\n"
			"fcvt.ps.pwu f10, f30, rup\n"
			"fmin.ps f1, f30, f5\n"
			"fmul.s f3, f7, f4, rne\n"
			"fle.ps f30, f4, f30\n"
			"bltu x16, x13, 1f\n"
			"1:\n"
			"LBL_SEQID_4_INT_LOAD_ID_124_HID_0:\n"
			"li x20, 0x810c76d803\n" 
			"sd x15, 856(x20)\n"
			"lwu x10, 856(x20)\n" 
			"fsgnjx.ps f9, f9, f30\n"
			"feq.pi f8, f30, f5\n"
			"fnmsub.ps f1, f30, f30, f9, rne\n"
			"fmul.s f5, f30, f5, rdn\n"
			"fslli.pi f2, f30, 0xf\n"
			"LBL_SEQID_4_FP_AMO_ID_125_HID_0:\n"
			"li x15, 0x822f90000c\n" 
			"la x16, mem_ptr_offset\n"
			"flw.ps f4, 992(x16)\n"
			"famoandg.pi f3, f4 (x15)\n" 
			"fcvt.f16.ps f7, f10\n"
			"fsatu8.pi f1, f10\n"
			"fcmovm.ps f6, f8, f6\n"
			"fmv.s.x f8, x0\n"
			"fmv.x.s x0, f2\n"
			"LBL_SEQID_4_INT_AMO_ID_126_HID_0:\n"
			"li x28, 0x82562003f4\n" 
			"amominl.w x16, x10, (x28)\n" 
			"fadd.s f9, f9, f9, rdn\n"
			"feq.ps f9, f9, f10\n"
			"fsra.pi f27, f27, f10\n"
			"feq.pi f6, f9, f13\n"
			"fsin.ps f16, f9\n"
			"LBL_SEQID_4_INT_AMO_ID_127_HID_0:\n"
			"li x15, 0x81490aa020\n" 
			"amomaxul.w x14, x10, (x15)\n" 
			"fsub.pi f3, f10, f3\n"
			"sltu x20, x16, x10\n"
			"bltu x15, x0, 1f\n"
			"1:\n"
			"fcvt.f16.ps f4, f6\n"
			"sllw x20, x14, x11\n"
			"fsub.s f4, f10, f6, rtz\n"
			"la x18, rand_int_32\n"
			"flw.ps f4, 224(x18)\n"
			"flw.ps f5, 96(x18)\n"
			"frem.pi f7, f4, f5\n" 
			"fxor.pi f5, f28, f28\n"
			"bge x20, x6, 1f\n"
			"1:\n"
			"fcvt.ps.pw f9, f16, rup\n"
			"fnmsub.s f20, f7, f6, f20, rup\n"
			"frcp.ps f16, f7\n"
			"ffrc.ps f6, f6\n"
			"mov.m.x m0, x0, 0xFF\n"
			"li x5, 0x28\n"
			"la x31, rand_ieee754_32\n"
			"add x31, x31, x5\n"
			"flw.ps f13, 0(x31)\n"
			"flw.ps f16, 2(x31)\n"
			"flw.ps f2, 6(x31)\n"
			"flw.ps f20, 3(x31)\n"
			"flw.ps f27, 15(x31)\n"
			"flw.ps f4, 1(x31)\n"
			"flw.ps f5, 23(x31)\n"
			"flw.ps f6, 7(x31)\n"
			"la x31, rand_int_32\n"
			"add x31, x31, x5\n"
			"lw x0, 0(x31)\n"
			"lw x10, 1(x31)\n"
			"lw x27, 0(x31)\n"
			"li x21, 0x1\n"
			"LBL_SEQID_4_LOOP_SEQ_HID_0:\n"
			"feqm.ps m5, f5, f2\n"
			"fsin.ps f28, f13\n"
			"fcvt.wu.s x15, f20, rdn\n"
			"beq x30, x14, 1f\n"
			"1:\n"
			"fsgnjn.ps f2, f2, f16\n"
			"feq.pi f16, f13, f16\n"
			"bgeu x1, x8, 1f\n"
			"1:\n"
			"divw x15, x27, x10\n"
			"flt.pi f6, f6, f4\n"
			"feq.pi f28, f27, f20\n"
			"maskor m1, m6, m6\n"
			"flt.s x10, f6, f4\n"
			"beq x6, x28, 1f\n"
			"1:\n"
			"remw x10, x27, x10\n"
			"masknot m5, m1\n"
			"fsra.pi f5, f4, f2\n"
			"packb x28, x10, x10\n"
			"fsin.ps f20, f6\n"
			"fmv.s.x f5, x27\n"
			"fcvt.ps.pwu f16, f13, rmm\n"
			"fxor.pi f5, f2, f27\n"
			"srai x28, x27, 4\n"
			"fadd.ps f5, f13, f27, rup\n"
			"fxor.pi f13, f2, f13\n"
			"fadd.pi f2, f4, f2\n"
			"fbci.ps f2, 0xeddf\n"
			"lui x0, 0x12\n"
			"for.pi f20, f13, f20\n"
			"beq x18, x2, 1f\n"
			"1:\n"
			"mul x0, x10, x0\n"
			"sraw x18, x27, x27\n"
			"fslli.pi f28, f27, 0x0\n"
			"fle.pi f20, f5, f20\n"
			"slti x18, x10, 0xb\n"
			"fswizz.ps f1, f5, 0x17\n"
			"faddi.pi f1, f2, 0x1fb\n"
			"addiw x28, x27, 0x18\n"
			"mul x10, x0, x10\n"
			"sraiw x0, x0, 1\n"
			"bgeu x26, x14, 1f\n"
			"1:\n"
			"fsub.ps f28, f20, f27, rdn\n"
			"fxor.pi f13, f13, f4\n"
			"fcvt.ps.f16 f20, f6\n"
			"fslli.pi f5, f5, 0xb\n"
			"fsra.pi f16, f6, f4\n"
			"xori x10, x0, 0x5\n"
			"feq.ps f5, f4, f4\n"
			"mov.m.x m2, x0, 0x5c\n"
			"blt x21, x16, 1f\n"
			"1:\n"
			"div x15, x10, x10\n"
			"flt.pi f1, f5, f27\n"
			"feq.s x0, f20, f6\n"
			"fmul.pi f1, f6, f20\n"
			"fsrl.pi f28, f27, f4\n"
			"and x28, x10, x0\n"
			"srai x18, x27, 1\n"
			"frsq.ps f28, f4\n"
			"maskpopcz x15, m1\n"
			"beq x18, x28, 1f\n"
			"1:\n"
			"sraiw x28, x10, 3\n"
			"bltu x0, x24, 1f\n"
			"1:\n"
			"div x27, x0, x27\n"
			"fsrai.pi f6, f6, 0xf\n"
			"fswizz.ps f16, f16, 0x0\n"
			"slt x27, x27, x27\n"
			"sll x15, x10, x10\n"
			"maskpopcz x28, m2\n"
			"fcmov.ps f1, f13, f20, f4\n"
			"blt x7, x30, 1f\n"
			"1:\n"
			"feqm.ps m2, f4, f6\n"
			"frcp.ps f16, f20\n"
			"bltu x30, x28, 1f\n"
			"1:\n"
			"srli x18, x0, 4\n"
			"bne x27, x27, 1f\n"
			"1:\n"
			"sra x15, x27, x10\n"
			"mulh x15, x10, x0\n"
			"remu x27, x27, x27\n"
			"fcvt.s.w f2, x0, rmm\n"
			"sra x10, x10, x27\n"
			"blt x23, x22, 1f\n"
			"1:\n"
			"srlw x18, x27, x0\n"
			"maskor m5, m6, m1\n"
			"bgeu x25, x18, 1f\n"
			"1:\n"
			"mulw x15, x0, x27\n"
			"faddi.pi f28, f2, 0x49\n"
			"bltu x3, x17, 1f\n"
			"1:\n"
			"sub x15, x10, x10\n"
			"fsub.ps f1, f5, f4, rmm\n"
			"flt.s x15, f6, f20\n"
			"sltu x27, x27, x0\n"
			"fmv.x.s x0, f5\n"
			"sll x18, x10, x10\n"
			"beq x11, x20, 1f\n"
			"1:\n"
			"srai x10, x0, 2\n"
			"blt x9, x13, 1f\n"
			"1:\n"
			"fltm.pi m5, f16, f4\n"
			"fmul.ps f7, f16, f4, rmm\n"
			"fclass.ps f16, f13\n"
			"frcp.ps f5, f4\n"
			"divuw x28, x0, x10\n"
			"srliw x18, x27, 0\n"
			"fsub.s f13, f13, f6, rup\n"
			"fcmovm.ps f16, f4, f2\n"
			"bgeu x30, x4, 1f\n"
			"1:\n"
			"ori x15, x10, 0xe\n"
			"bgeu x17, x22, 1f\n"
			"1:\n"
			"fsll.pi f16, f4, f4\n"
			"fmin.ps f5, f27, f5\n"
			"slli x28, x10, 2\n"
			"flt.ps f7, f2, f20\n"
			"fmsub.ps f7, f6, f4, f6, rmm\n"
			"mov.m.x m5, x10, 0x72\n"
			"fltm.pi m5, f27, f13\n"
			"srai x28, x0, 1\n"
			"sll x0, x10, x0\n"
			"fsetm.pi m5, f16\n"
			"srai x0, x0, 3\n"
			"xori x10, x0, 0x6\n"
			"or x0, x10, x0\n"
			"fcvt.w.s x0, f16, rup\n"
			"fsin.ps f7, f2\n"
			"fsub.ps f16, f2, f6, rup\n"
			"beq x27, x25, 1f\n"
			"1:\n"
			"fadd.ps f7, f27, f4, rne\n"
			"frsq.ps f16, f16\n"
			"fmax.s f20, f20, f16\n"
			"masknot m2, m6\n"
			"fcvt.s.w f16, x27, rne\n"
			"fsub.ps f1, f6, f2, rdn\n"
			"remuw x27, x27, x27\n"
			"fmin.pi f28, f4, f6\n"
			"addiw x0, x10, 0x17\n"
			"bge x10, x3, 1f\n"
			"1:\n"
			"fmax.s f13, f16, f13\n"
			"fsgnjn.ps f5, f16, f20\n"
			"fsetm.pi m2, f6\n"
			"bltu x31, x2, 1f\n"
			"1:\n"
			"div x15, x0, x27\n"
			"fadd.ps f28, f16, f4, rup\n"
			"fsub.ps f1, f13, f2, rne\n"
			"fpackrepb.pi f2, f16\n"
			"xor x10, x10, x10\n"
			"blt x24, x15, 1f\n"
			"1:\n"
			"fle.pi f2, f2, f6\n"
			"slt x28, x10, x10\n"
			"bgeu x8, x20, 1f\n"
			"1:\n"
			"fsrl.pi f16, f4, f16\n"
			"fmin.pi f2, f20, f2\n"
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
			"flw.ps f14,  832 (x5)\n"
			"flw.ps f13,  928 (x5)\n"
			"flw.ps f29,  320 (x5)\n"
			"flw.ps f21,  160 (x5)\n"
			"flw.ps f11,  960 (x5)\n"
			"flw.ps f25,  512 (x5)\n"
			"flw.ps f5,  640 (x5)\n"
			"flw.ps f20,  416 (x5)\n"
			"flw.ps f1,  800 (x5)\n"
			"flw.ps f17,  352 (x5)\n"
			"flw.ps f0,  608 (x5)\n"
			"flw.ps f2,  32 (x5)\n"
			"flw.ps f3,  288 (x5)\n"
			"flw.ps f19,  544 (x5)\n"
			"flw.ps f10,  480 (x5)\n"
			"flw.ps f22,  128 (x5)\n"
			"flw.ps f15,  736 (x5)\n"
			"flw.ps f26,  896 (x5)\n"
			"flw.ps f27,  704 (x5)\n"
			"flw.ps f12,  448 (x5)\n"
			"flw.ps f30,  192 (x5)\n"
			"flw.ps f7,  224 (x5)\n"
			"flw.ps f18,  672 (x5)\n"
			"flw.ps f8,  864 (x5)\n"
			"flw.ps f23,  256 (x5)\n"
			"flw.ps f16,  96 (x5)\n"
			"flw.ps f24,  0 (x5)\n"
			"flw.ps f31,  992 (x5)\n"
			"flw.ps f6,  64 (x5)\n"
			"flw.ps f28,  384 (x5)\n"
			"flw.ps f4,  768 (x5)\n"
			"flw.ps f9,  576 (x5)\n"
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
			"LBL_HPM_3_CORE_RETIRED_INST0_FAIL_HID_0:\n"          
			"csrr x5, mhpmcounter3\n"
			"sd x5, 32 (x31)\n"
			"LBL_HPM_4_CORE_TXFMA_INT_OPS_FAIL_HID_0:\n"          
			"csrr x5, mhpmcounter4\n"
			"sd x5, 40 (x31)\n"
			"LBL_HPM_5_CORE_TL_INST_FAIL_HID_0:\n"          
			"csrr x5, mhpmcounter5\n"
			"sd x5, 48 (x31)\n"
			"LBL_HPM_6_CORE_TREDUCE_INST_FAIL_HID_0:\n"          
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
			"LBL_HPM_3_CORE_RETIRED_INST0_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter3\n"
			"sd x5, 32 (x31)\n"
			"LBL_HPM_4_CORE_TXFMA_INT_OPS_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter4\n"
			"sd x5, 40 (x31)\n"
			"LBL_HPM_5_CORE_TL_INST_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter5\n"
			"sd x5, 48 (x31)\n"
			"LBL_HPM_6_CORE_TREDUCE_INST_PASS_HID_0:\n"          
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
		0x93db93f0, 
		0x1229251a, 
		0xdc1ea289, 
		0xda39d6eb, 
		0xdde8bbce, 
		0xcb0b8791, 
		0x45883bac, 
		0x51cb94a0, 
		0xd0c6e50e, 
		0x1e7d408b, 
		0xce02cdd4, 
		0x5446e81f, 
		0x6a525bc9, 
		0xef728c4e, 
		0xf35cb7f5, 
		0x2f59f81a, 
		0xa6bd3d31, 
		0x372569b8, 
		0x6ca341a8, 
		0xc5867d98, 
		0x7662bf59, 
		0x5feac544, 
		0x24799215, 
		0xe329cf6e, 
		0x50d7760a, 
		0x1c2fa035, 
		0xdc963b27, 
		0xefafb6be, 
		0x2e15db3c, 
		0x21633fa5, 
		0x57f0f4d2, 
		0xb5ffc4d3, 
		0x3c22eccf, 
		0xb8d0b6b1, 
		0xa9829134, 
		0x12d1ebc3, 
		0xc4bf7605, 
		0xa210d191, 
		0xf2baa63d, 
		0x1412bf6b, 
		0xa7f45e06, 
		0x38c79cfc, 
		0x3a315c87, 
		0xc5fefcf2, 
		0xfdddbee9, 
		0xde2bdfeb, 
		0xec55495e, 
		0x97374fb3, 
		0xe4df06e9, 
		0x8edec375, 
		0x760fa398, 
		0xd5a4e9ba, 
		0x9a81f9de, 
		0xf36e05ee, 
		0x98f031db, 
		0x89922f04, 
		0xaddd9fd5, 
		0x5994b840, 
		0x6af9b7bc, 
		0x70eec03b, 
		0x59e7cede, 
		0x59767df1, 
		0xf40079f9, 
		0x2df6a994, 
		0xe258074e, 
		0x7abdda73, 
		0xd71f566a, 
		0x83415058, 
		0xf5180a23, 
		0xf099f717, 
		0x2454afaf, 
		0x8add80ed, 
		0xce5a54bc, 
		0x092bad99, 
		0x79552210, 
		0x159c8d8f, 
		0xccfee714, 
		0x915e4620, 
		0x705c98b3, 
		0x8f761e0d, 
		0xa0d2f737, 
		0xdfba1172, 
		0x525e5027, 
		0x430beb3d, 
		0xd9e385f3, 
		0x66f2946d, 
		0x3a377f69, 
		0x46921134, 
		0x69d3cc8a, 
		0x864e2fb1, 
		0xf7be1b39, 
		0x51140c9a, 
		0x997ee66d, 
		0xb5610788, 
		0x39be5eaa, 
		0xebd4dc33, 
		0x41c8ea34, 
		0xd2fbf676, 
		0x40d4f64f, 
		0x61910a3b, 
		0x4c92763d, 
		0x9d080f06, 
		0x4e8a036b, 
		0xc3e6969f, 
		0xa5fe8257, 
		0xa1ae22c2, 
		0x7580bd40, 
		0x74afc50c, 
		0xcd151d11, 
		0xec3a02a3, 
		0x05a8e843, 
		0x4e01c8b6, 
		0x98b30b75, 
		0x25a5a552, 
		0x57faee0e, 
		0x959c77e8, 
		0xfe488737, 
		0x6bc7ee9b, 
		0x29857806, 
		0x72d33900, 
		0x6e466fdc, 
		0xeb39139a, 
		0xe34df59a, 
		0x062a8c61, 
		0x41011f44, 
		0xb593d432, 
		0x347de233, 
		0xba7fce47, 
		0x3e9eed6d, 
		0x06ddf212, 
		0xdf34b628, 
		0x6dd84c77, 
		0x5201d061, 
		0xbc4a3139, 
		0xfaa42e91, 
		0x9b084cd7, 
		0xfc361d7c, 
		0x7f541150, 
		0x254d170d, 
		0x8f69955d, 
		0x4976b1d9, 
		0x1c4f7079, 
		0x09dc3c13, 
		0x7810a9c9, 
		0x2732b2db, 
		0xdfdadf41, 
		0xa2a471f9, 
		0x71a0bc6e, 
		0xca62eeac, 
		0xf2bf0bd6, 
		0x00092243, 
		0xc425d318, 
		0xd39fa155, 
		0x659346f5, 
		0xd6126414, 
		0x231e3a19, 
		0xae82076b, 
		0x1bedc3b9, 
		0x61e62229, 
		0x770b7c69, 
		0x2fbcc431, 
		0xa1f564c9, 
		0x80ba67e5, 
		0x5787e705, 
		0x3a49e8d5, 
		0x0f4a5061, 
		0x56abed96, 
		0x092ff0de, 
		0xc2fe6acd, 
		0xac4fc707, 
		0xfffa5e4d, 
		0x76f5b819, 
		0x0dd215ce, 
		0x51fedc84, 
		0x933057a8, 
		0x30d07f8b, 
		0x06d2beca, 
		0x0239f31b, 
		0x788ee369, 
		0x514e4485, 
		0x3df13f0f, 
		0x13ceaf9f, 
		0xc67847ab, 
		0xadc90580, 
		0xa74da99d, 
		0x14465734, 
		0xd830461e, 
		0x71f01516, 
		0x872d5419, 
		0x8a82f420, 
		0x3c98f6a2, 
		0x3dd26a46, 
		0xd652b32d, 
		0xb3ae9452, 
		0x8638e8ee, 
		0xbbf90ff7, 
		0x48d4227d, 
		0xbc634df5, 
		0x899b0e50, 
		0xc009b14a, 
		0x93cba524, 
		0xf6935f11, 
		0x309b0eb3, 
		0x950d3a16, 
		0xe6cff26c, 
		0xedda61d8, 
		0xcc7a8c5f, 
		0x4e0e6696, 
		0x6e502fe0, 
		0xddffecff, 
		0x8cb37d38, 
		0x556913ad, 
		0x02da608a, 
		0x7b770a1e, 
		0x810b3b8c, 
		0xfa36af7d, 
		0x6ad1a727, 
		0x4f955064, 
		0x192c7385, 
		0x749d9bb9, 
		0x899872dc, 
		0x7d45c17b, 
		0x5632a214, 
		0x6d755f2b, 
		0xf06fa418, 
		0x73981dd9, 
		0x1478eea2, 
		0xe6926893, 
		0xdcb4550d, 
		0xef542355, 
		0xcc4c5af4, 
		0x9752e055, 
		0x4984756d, 
		0x6fe494f4, 
		0x46997cc3, 
		0xd4442043, 
		0x22eb608a, 
		0xe43f05ea, 
		0x65343d07, 
		0x668ffb3f, 
		0x4b5ea8b0, 
		0xf5141dc6, 
		0xf5aa9e44, 
		0x8c48f2c5, 
		0x196134a0, 
		0x78d8a981, 
		0x0b85c8da, 
		0x2ca91306, 
		0xf88100b2, 
		0xdfd64261, 
		0x4b133d56, 
		0x6c013cef, 
		0xe7769366, 
		0x44544abe, 
		0x42017489, 
		0x8e8e3cb7  
};
volatile uint32_t rand_ieee754_32[1536] __attribute__ ((aligned (2048))) = {
		0x42740000, 
		0x880ef1b5, 
		0x7fffffff, 
		0x007ffffe, 
		0xc2700000, 
		0x8f644eaa, 
		0x00800000, 
		0x80020000, 
		0xc2340000, 
		0x41400000, 
		0x76048295, 
		0xe60e4470, 
		0x41400000, 
		0xd5ea427a, 
		0x80094d34, 
		0xac60e430, 
		0x00000100, 
		0xc2540000, 
		0x74ec55f4, 
		0xf9967a29, 
		0x4194aad4, 
		0x80000008, 
		0x913b9d1b, 
		0x80000040, 
		0xff800000, 
		0x01a27637, 
		0x00400000, 
		0x00000000, 
		0x55555555, 
		0x40a00000, 
		0x42680000, 
		0x74e36e50, 
		0x0e000001, 
		0x80000000, 
		0xff800000, 
		0x0c7ffe00, 
		0x00001000, 
		0x751c0caf, 
		0x00200000, 
		0x80002000, 
		0x00010000, 
		0x7fbfffff, 
		0x00011111, 
		0xf3888d3b, 
		0x00400000, 
		0x623166bc, 
		0x57755a3b, 
		0x7fafb40e, 
		0xc1200000, 
		0xff000000, 
		0x2ad4b8a6, 
		0x00000020, 
		0x93ff2cfc, 
		0x00000000, 
		0x80800001, 
		0x42340000, 
		0x94aef6ba, 
		0x7fc00000, 
		0xa20aed95, 
		0xd0e25ee8, 
		0x0c7f8000, 
		0x42000000, 
		0x41300000, 
		0xbf028f5c, 
		0x0e0000ff, 
		0x80000020, 
		0x80000000, 
		0x80000000, 
		0x0c7fffff, 
		0x00000008, 
		0x80000200, 
		0xb5f0a24a, 
		0x0c7ffc00, 
		0x42480000, 
		0x42340000, 
		0x41d80000, 
		0x80800001, 
		0xc2100000, 
		0x00800002, 
		0x0e000001, 
		0x14ce4154, 
		0x0e0000ff, 
		0x8056a95e, 
		0x0e3fffff, 
		0xff000000, 
		0xc1e00000, 
		0x80040000, 
		0xd81f7ad6, 
		0x807bcddd, 
		0x80800001, 
		0x247f9fab, 
		0x419b2bc2, 
		0x00002000, 
		0x42400000, 
		0x4add6171, 
		0x0148d37e, 
		0x0c700000, 
		0xc2580000, 
		0x0c7fff80, 
		0x0f7ffffd, 
		0x7f7fffff, 
		0x73400eb2, 
		0x0c7ffff0, 
		0x10eaf6fb, 
		0x41d80000, 
		0xd49c3746, 
		0x00000000, 
		0x3f800001, 
		0x0327c5b7, 
		0x001565b0, 
		0x7fc00000, 
		0x007ffffe, 
		0xff800000, 
		0x61991754, 
		0x7f800000, 
		0xbdc42fbf, 
		0x7fbfffff, 
		0x0c7fc000, 
		0x41c00000, 
		0x0a282931, 
		0xc1a00000, 
		0x3f028f5c, 
		0x00000020, 
		0xc1d00000, 
		0xc1500000, 
		0x500aeaec, 
		0x3be0a5d9, 
		0x00000020, 
		0x80000000, 
		0x80000004, 
		0x79b9b00f, 
		0x80000000, 
		0x800dfa2f, 
		0xfed4c21c, 
		0xa9973b89, 
		0x42380000, 
		0x3f4f9a4d, 
		0x80000000, 
		0x4ab77f12, 
		0x80800002, 
		0xc2400000, 
		0x4b000000, 
		0x80000000, 
		0x42140000, 
		0xb79f5b7d, 
		0x005ce04a, 
		0x30f69932, 
		0xeb31c176, 
		0x7bdb1162, 
		0x7fffffff, 
		0x00800003, 
		0x97768864, 
		0x7f7ffffe, 
		0xff800000, 
		0x80800002, 
		0x7f7ffffe, 
		0xc1c00000, 
		0x9144b6a3, 
		0x41d00000, 
		0x80ffffff, 
		0xf668d1f4, 
		0x7d0cc349, 
		0xc886ef79, 
		0x42380000, 
		0x80000001, 
		0x00002000, 
		0x00000001, 
		0xbb6d2557, 
		0xc1900000, 
		0x38433ad5, 
		0x80000000, 
		0x0c7ffffc, 
		0x42480000, 
		0xb55c73ad, 
		0x80000000, 
		0x42500000, 
		0x0be758e0, 
		0xc2380000, 
		0x7f800000, 
		0x7f7ffffe, 
		0x80080000, 
		0x62ee7f19, 
		0x41f00000, 
		0x0c780000, 
		0x80000001, 
		0xff7fffff, 
		0x80800002, 
		0x2358074f, 
		0xc0800000, 
		0x7fc00000, 
		0x80000002, 
		0x0e0003ff, 
		0x7f800000, 
		0x7f800000, 
		0xe6ae54ff, 
		0x50b00355, 
		0x80000004, 
		0x9c062d28, 
		0xffffffff, 
		0xff800000, 
		0x00100000, 
		0x80000001, 
		0x0f7fffff, 
		0x00000000, 
		0xc1c00000, 
		0x00010000, 
		0x0c7f8000, 
		0x428b4d56, 
		0x00040000, 
		0x42480000, 
		0xc1000000, 
		0x80800000, 
		0xde2f63cc, 
		0xdf6e82f3, 
		0x0c400000, 
		0x80800000, 
		0xc2380000, 
		0x7f800000, 
		0x1aca2ae2, 
		0x0140ed7e, 
		0x0c7ff800, 
		0xff800000, 
		0x0c7fff80, 
		0x41700000, 
		0x0c7fffc0, 
		0x27e5a7fe, 
		0xdec04468, 
		0xc2540000, 
		0x41b00000, 
		0xd8297340, 
		0x40000000, 
		0xff000000, 
		0x80800003, 
		0x00000000, 
		0x3f028f5c, 
		0x00000000, 
		0x00100000, 
		0x00000000, 
		0x41a00000, 
		0x80000000, 
		0x07006b65, 
		0xc20a2a06, 
		0x00000020, 
		0xc1980000, 
		0x7fa9ba83, 
		0x7f800000, 
		0xcf78768b, 
		0x0e3fffff, 
		0x80800001, 
		0x00206c13, 
		0x436cd3a1, 
		0x0c7ffff8, 
		0xea545bb7, 
		0x7f800000, 
		0x80100000, 
		0xc0400000, 
		0xc18e7dac, 
		0x00020000, 
		0x7fbb447d, 
		0x00040000, 
		0xbf800000, 
		0xff7ffffe, 
		0xccc7851d, 
		0x41b80000, 
		0x63270fcd, 
		0xbf800001, 
		0x7feed2f0, 
		0x6290eb4c, 
		0xe512012f, 
		0x00400000, 
		0x3d81cac6, 
		0xc2700000, 
		0xff9c7c0a, 
		0xf1353808, 
		0x9ec5d9a2, 
		0xffa96705, 
		0x7f800000, 
		0xc1c00000, 
		0x7f800000, 
		0x80800001, 
		0x7f7ffffe, 
		0x41300000, 
		0x42540000, 
		0x0c600000, 
		0x42280000, 
		0xeab7c4fa, 
		0xc0000000, 
		0xbe18ae8a, 
		0x0e0003ff, 
		0x80000004, 
		0x00080000, 
		0x0fe76a55, 
		0xffe491f6, 
		0xff800000, 
		0xd3c0b2e6, 
		0x0c7f8000, 
		0x0e003fff, 
		0x423c0000, 
		0x41a00000, 
		0x5a1b2856, 
		0x80000000, 
		0x41c80000, 
		0xa1461474, 
		0x00100000, 
		0x0c7fc000, 
		0x30c0f5d7, 
		0xc0400000, 
		0x80000000, 
		0x5e54aefc, 
		0xc1900000, 
		0xc0a00000, 
		0x41880000, 
		0x870e153e, 
		0xc0400000, 
		0xff7fffff, 
		0xc2400000, 
		0x3dbf2a09, 
		0x41300000, 
		0xff7ffffe, 
		0x8bee7759, 
		0x1298a451, 
		0x7f7fffff, 
		0xff7ffffe, 
		0xfb944fd0, 
		0x0e001fff, 
		0xea94f475, 
		0x0c7ffe00, 
		0x0c7fffe0, 
		0x00040000, 
		0x80000000, 
		0x3036dff9, 
		0xbfb45f72, 
		0xc0af98bc, 
		0xc33af7cf, 
		0x29b6af36, 
		0x00800001, 
		0x42700000, 
		0x005502c8, 
		0xc1c80000, 
		0xc2640000, 
		0x8ae8cee6, 
		0xe9b771ac, 
		0x00800003, 
		0x00008000, 
		0x7f7fffff, 
		0x1562433d, 
		0x00000000, 
		0x00011111, 
		0x45a80166, 
		0x6219775a, 
		0x007ffffe, 
		0xff8d527c, 
		0x42580000, 
		0x00000008, 
		0x80000200, 
		0x887e6b5b, 
		0x2b010268, 
		0x3e48cd2a, 
		0x7fd10042, 
		0x80000010, 
		0xc1100000, 
		0xc1500000, 
		0x3d09fd27, 
		0x80001000, 
		0x0f7fffff, 
		0x0c7fffe0, 
		0x7f800000, 
		0x00000000, 
		0x7f800000, 
		0x00001000, 
		0x42680000, 
		0x0e001fff, 
		0x80000004, 
		0x837873b0, 
		0x96add83f, 
		0x80ffffff, 
		0x00000800, 
		0xff859e31, 
		0x80000000, 
		0x80000000, 
		0xc2400000, 
		0x007ffffe, 
		0x0c700000, 
		0xfc3ae725, 
		0x80000400, 
		0x80200000, 
		0x807ffffe, 
		0x0e03ffff, 
		0x0f7ffffc, 
		0x007fffff, 
		0x00004000, 
		0x7f93d99e, 
		0x33333333, 
		0x40800000, 
		0x80011111, 
		0x00010000, 
		0xc2080000, 
		0x0c7ffffc, 
		0xff800000, 
		0x7fc00000, 
		0x3f028f5c, 
		0x42640000, 
		0xff800000, 
		0x7f800000, 
		0xf37710a8, 
		0xc1000000, 
		0x80000001, 
		0x0c7f8000, 
		0xc1500000, 
		0x0c7f8000, 
		0x42640000, 
		0x3b7e668a, 
		0x00800001, 
		0xc98816dd, 
		0x0e01ffff, 
		0x80000001, 
		0x4e4db08c, 
		0xd6f54d35, 
		0xa1ce04a5, 
		0x00000000, 
		0x00000000, 
		0x3a03c96d, 
		0x42300000, 
		0x41800000, 
		0x00000000, 
		0xc2040000, 
		0x426c0000, 
		0x40e00000, 
		0x42740000, 
		0xc2640000, 
		0x4fade1ea, 
		0x00000200, 
		0x41f80000, 
		0xbb99d536, 
		0x0c7ffc00, 
		0x00000000, 
		0x00400000, 
		0xf2a83454, 
		0xc2200000, 
		0x0066bd5c, 
		0x00000000, 
		0xc1300000, 
		0x839abdf2, 
		0x7fc00000, 
		0x80800003, 
		0x00000000, 
		0x80000000, 
		0x54a467bf, 
		0x613dca41, 
		0x00000400, 
		0x80000001, 
		0x8290a9f7, 
		0xffbe82f5, 
		0xc1980000, 
		0x80420547, 
		0xc2740000, 
		0x00001000, 
		0x00010000, 
		0x80000000, 
		0x00000000, 
		0x41f0f67d, 
		0xe4a2ef66, 
		0xc1e00000, 
		0xc2240000, 
		0x00000000, 
		0x00000000, 
		0x0c7e0000, 
		0x80800001, 
		0x30374b8d, 
		0xc2480000, 
		0x036f281b, 
		0x5683dc21, 
		0x7fbfffff, 
		0xffed6226, 
		0x0e00000f, 
		0x967eef3f, 
		0x55a1b2ee, 
		0x3e07abe9, 
		0x0d000ff0, 
		0x8036dd77, 
		0xc8eba9d4, 
		0xc1f80000, 
		0x32f8045c, 
		0x42280000, 
		0x00080000, 
		0x41a00000, 
		0x00000000, 
		0xc2500000, 
		0xc0800000, 
		0x0d000ff0, 
		0x00661132, 
		0x80008000, 
		0x00800002, 
		0x243164c5, 
		0x42340000, 
		0x30474f72, 
		0x80000080, 
		0xc1400000, 
		0x00400000, 
		0x44d0ebfb, 
		0x00800000, 
		0x89700eca, 
		0x425c0000, 
		0x80800001, 
		0x00400000, 
		0xc21c0000, 
		0xf4956c38, 
		0x4b8c4b40, 
		0x00000800, 
		0x80000000, 
		0x0e00003f, 
		0xc1d00000, 
		0x00000000, 
		0xc1f00000, 
		0x42280000, 
		0x425c0000, 
		0x825da542, 
		0x6fa26dd9, 
		0x42140000, 
		0x00800000, 
		0xfc45d821, 
		0xf1ae668b, 
		0x94db513c, 
		0x00000000, 
		0xffffffff, 
		0x00000000, 
		0x41e80000, 
		0xc22c0000, 
		0x40400000, 
		0x7f7fffff, 
		0xc3ca94e7, 
		0x80800000, 
		0xff7fffff, 
		0x68139493, 
		0x0c7ffff8, 
		0xc1400000, 
		0x0e07ffff, 
		0x423c0000, 
		0xff92aafb, 
		0x116d79fb, 
		0x80200000, 
		0xc224ab78, 
		0xc0800000, 
		0xc941d89c, 
		0x5e1389a8, 
		0x41e00000, 
		0x80000080, 
		0x6891db9d, 
		0x00000000, 
		0x80000010, 
		0xc21c0000, 
		0xc1980000, 
		0xc9e13b70, 
		0x0c7c0000, 
		0x2ab8765e, 
		0x0f7ffffc, 
		0xc2640000, 
		0x25b2737d, 
		0x42700000, 
		0xc26c0000, 
		0x41400000, 
		0xc1200000, 
		0x00800003, 
		0x8f7ffffe, 
		0x40c00000, 
		0x01ab9faa, 
		0xc1b80000, 
		0x00000000, 
		0x0c7e0000, 
		0x80200000, 
		0x00ffffff, 
		0x131ac6f3, 
		0xc1600000, 
		0xc1300000, 
		0x7ef1a148, 
		0xc1d80000, 
		0x80011111, 
		0xc2580000, 
		0x753ff60c, 
		0x80800002, 
		0x00ffffff, 
		0x00000000, 
		0xff800000, 
		0x807fffff, 
		0x9ae83266, 
		0x0f7ffffe, 
		0x0c400000, 
		0xff800000, 
		0x8f7ffffd, 
		0xc21c0000, 
		0x09cd9285, 
		0x200a28d7, 
		0xc1a00000, 
		0x41d00000, 
		0x1dae40fc, 
		0x66962836, 
		0x0d6833f6, 
		0x42340000, 
		0x82dc7fa0, 
		0x807fffff, 
		0x1fb5a28a, 
		0xc2400000, 
		0x0e7fffff, 
		0x00000002, 
		0xbf028f5c, 
		0x00000002, 
		0x40ef9eff, 
		0x42300000, 
		0xc2080000, 
		0x40a00000, 
		0xa4ade113, 
		0x3bf15e1b, 
		0xc2680000, 
		0xff800000, 
		0x7f000000, 
		0x7f7fffff, 
		0xff000000, 
		0xff7ffffe, 
		0x7f800000, 
		0x00010000, 
		0xede7c294, 
		0x5ec940a7, 
		0xc2780000, 
		0x7c13c78d, 
		0x58d8fc33, 
		0x0d00fff0, 
		0xc2380000, 
		0xe6543078, 
		0x80000000, 
		0x0e000001, 
		0x00000001, 
		0x3f800001, 
		0xff8c7291, 
		0x32edfff7, 
		0x0028cdb6, 
		0xc2440000, 
		0x91ff2a28, 
		0xc0800000, 
		0x34934557, 
		0xe2ef6e98, 
		0x80800003, 
		0xffbfa8c0, 
		0x00800002, 
		0xc1a00000, 
		0x422c0000, 
		0x0e01ffff, 
		0x80020000, 
		0x0e000007, 
		0xc20c0000, 
		0x4da88395, 
		0x94796faf, 
		0x41b80000, 
		0x80000000, 
		0xa6469093, 
		0x7fc00000, 
		0x7ee2ef3d, 
		0x719520da, 
		0x00001000, 
		0x80000000, 
		0x80000008, 
		0x0e7fffff, 
		0xff7ffffe, 
		0xa9a2f632, 
		0xc2480000, 
		0x3631d6b6, 
		0x00000000, 
		0xc1000000, 
		0xc2600000, 
		0x7f800001, 
		0x7f7ffffe, 
		0x42540000, 
		0x7f800000, 
		0x42140000, 
		0xff7fffff, 
		0x17e64c88, 
		0xd6fbb1c8, 
		0xff800000, 
		0x00400000, 
		0x80000100, 
		0x605e8bca, 
		0x00000800, 
		0x42000000, 
		0x22d6e60e, 
		0x00004000, 
		0xe952cf96, 
		0x675e94d2, 
		0x00000004, 
		0x00000000, 
		0x643c3b79, 
		0x24c15283, 
		0x0e000007, 
		0x00000000, 
		0xff800000, 
		0x0e0000ff, 
		0xffe94a90, 
		0xff888019, 
		0x4e8871b6, 
		0x0e00003f, 
		0xa16039d2, 
		0xc2400000, 
		0x812135f4, 
		0x41700000, 
		0x1c5c46fa, 
		0xc1900000, 
		0x7f800000, 
		0xc2440000, 
		0x0e7fffff, 
		0x00000000, 
		0xb3ded8f3, 
		0x84a189b3, 
		0x80000200, 
		0x954304bf, 
		0x00000002, 
		0x8f7ffffd, 
		0xce9c2dec, 
		0x0c780000, 
		0xd0a28360, 
		0xc0400000, 
		0x80000000, 
		0x0c7fff80, 
		0x41000000, 
		0x0c7fffe0, 
		0xaaaaaaaa, 
		0x004f32a3, 
		0x75987bd0, 
		0x7f800000, 
		0x63bed91d, 
		0xf091a81e, 
		0x1ebe6d92, 
		0x80800001, 
		0xe200eee5, 
		0xc2080000, 
		0x960887e7, 
		0x4b000000, 
		0xc2780000, 
		0xbb5fda32, 
		0xc1100000, 
		0x42180000, 
		0xf81a8297, 
		0x4bc23c02, 
		0x9f33d0dc, 
		0x0c700000, 
		0xe79830b4, 
		0xc24c0000, 
		0x00800001, 
		0xbef53ba0, 
		0x3f4e6a87, 
		0x00004000, 
		0x4d624135, 
		0x41000000, 
		0x80000000, 
		0x00800001, 
		0x00000000, 
		0x72d3476c, 
		0x41f00000, 
		0x42500000, 
		0x00000000, 
		0x7fbfffff, 
		0x00000000, 
		0x80080000, 
		0x5aab61f0, 
		0x80000000, 
		0x0e03ffff, 
		0x42240000, 
		0x8cba0d70, 
		0xff800000, 
		0x3f028f5c, 
		0xc1f00000, 
		0xc2a69afc, 
		0xc1d80000, 
		0x42040000, 
		0x42740000, 
		0x26484cc0, 
		0x80000080, 
		0x00000000, 
		0xff800000, 
		0x5f53ec33, 
		0x424c0000, 
		0x41400000, 
		0x80000002, 
		0xff7ffffe, 
		0x00004000, 
		0xc1f00000, 
		0x80040000, 
		0x0c7ffc00, 
		0x85fd207b, 
		0x425c0000, 
		0x7faa8002, 
		0x6dcbe73d, 
		0x0dbe56e7, 
		0x80000004, 
		0x8f7ffffe, 
		0x0e1fffff, 
		0xd1373f1a, 
		0x4fa6eefa, 
		0xff7ffffe, 
		0x00000000, 
		0x7fad4f1f, 
		0x80000000, 
		0x3ecdfbe6, 
		0x41d00000, 
		0x0e07ffff, 
		0x4578f8da, 
		0xff800001, 
		0xa0dee182, 
		0xd1451a5a, 
		0x7fffffff, 
		0x2cc080de, 
		0x7f800000, 
		0xd06d467c, 
		0x00400000, 
		0xc1900000, 
		0x41b80000, 
		0xdc3579fd, 
		0xff800000, 
		0x40800000, 
		0xff7fffff, 
		0x7f7fffff, 
		0x7e7c1282, 
		0x00000000, 
		0x0e0001ff, 
		0xc25c0000, 
		0x0f7ffffc, 
		0xf414a006, 
		0xc2240000, 
		0x0e001fff, 
		0x59bed456, 
		0x2fd032fb, 
		0x00800003, 
		0x4cb0744d, 
		0xaaaaaaaa, 
		0xaaaaaaaa, 
		0xff800000, 
		0x80000002, 
		0x00040000, 
		0x41e00000, 
		0x7f7ffffe, 
		0x8b50e18a, 
		0x098b84dd, 
		0xc2700000, 
		0xc27c0000, 
		0x7f000000, 
		0x0e0007ff, 
		0xc98fdd54, 
		0x80f0aeff, 
		0x4b8a01e4, 
		0x41e80000, 
		0x7fa21239, 
		0x80000000, 
		0x80000000, 
		0x50d0a792, 
		0x00000002, 
		0x80800002, 
		0xbef10437, 
		0x7fadc341, 
		0x8ea1b46c, 
		0x80000000, 
		0x00000000, 
		0x3f800001, 
		0x21ef00b0, 
		0x00000020, 
		0x00800000, 
		0x0c780000, 
		0xb4814f6e, 
		0x0f7ffffd, 
		0xc2680000, 
		0xc0a00000, 
		0xff7ffffe, 
		0x68fc5861, 
		0x81ced507, 
		0x56ba2980, 
		0x00000080, 
		0x3e1e1ab3, 
		0xff7ffffe, 
		0x41d00000, 
		0x00000080, 
		0xc1e00000, 
		0x9095e105, 
		0xc2700000, 
		0x95512e65, 
		0xff800000, 
		0x00000002, 
		0xffaa19b3, 
		0xc0400000, 
		0x0e0007ff, 
		0xff800000, 
		0x00000008, 
		0x0e1fffff, 
		0x41e80000, 
		0x3f028f5c, 
		0x00000000, 
		0x0e03ffff, 
		0x41200000, 
		0x00000400, 
		0x0e01ffff, 
		0x0c7fffc0, 
		0xc1e00000, 
		0xc1a80000, 
		0xc5ac4020, 
		0xfe7802de, 
		0x0e01ffff, 
		0x9d8bc99c, 
		0x0c7ffff8, 
		0xffbfffff, 
		0x80800003, 
		0x0c7c0000, 
		0xa9dfda71, 
		0x00002000, 
		0x42240000, 
		0x927e7263, 
		0x42700000, 
		0x80000080, 
		0xc1c00000, 
		0x2657bb6f, 
		0x42740000, 
		0x91b30412, 
		0xac75bbe0, 
		0xabbcafd2, 
		0xc2000000, 
		0x41100000, 
		0x40000000, 
		0xff7ffffe, 
		0xb625c107, 
		0x00400000, 
		0x3f800000, 
		0x6a22e8b4, 
		0xcbd3cc26, 
		0xc2080000, 
		0x42600000, 
		0x42180000, 
		0x25e4ca26, 
		0x41200000, 
		0x1226eeee, 
		0x00800003, 
		0x706e5935, 
		0xff800000, 
		0x80000000, 
		0xc1800000, 
		0x00001000, 
		0x80002000, 
		0x00000001, 
		0xc1400000, 
		0xc2740000, 
		0x4c908f12, 
		0x7f800000, 
		0x7fc00000, 
		0x00800001, 
		0x42740000, 
		0x00002000, 
		0x00000000, 
		0x80040000, 
		0xc2040000, 
		0x8018974a, 
		0x0e007fff, 
		0x0c7fff00, 
		0x00000000, 
		0x0e001fff, 
		0xd504ba5a, 
		0xfc5e52dd, 
		0x41d00000, 
		0x00000000, 
		0x80010000, 
		0xee55d8b1, 
		0xa56332f7, 
		0x41000000, 
		0x42600000, 
		0x7fc00000, 
		0x217c151f, 
		0x7f7fffff, 
		0x42280000, 
		0x41880000, 
		0x00000200, 
		0xff800000, 
		0x0f7ffffd, 
		0xa90e1246, 
		0x41600000, 
		0x00000000, 
		0x622f181b, 
		0x35c51cd4, 
		0xc0bf1e70, 
		0xff800000, 
		0x00000000, 
		0xcda49eba, 
		0x00000800, 
		0xd4ec1dc6, 
		0x0c7ffffc, 
		0x80000000, 
		0x42440000, 
		0xc2700000, 
		0x00000000, 
		0xc25c0000, 
		0x91846303, 
		0x00000000, 
		0x00000000, 
		0x80000040, 
		0x552d1f0c, 
		0x0c400000, 
		0xc2380000, 
		0x00000000, 
		0x20359980, 
		0x42500000, 
		0xc1980000, 
		0x7fc00001, 
		0x00800000, 
		0x0e7fffff, 
		0x0201bdcd, 
		0xff800001, 
		0x13170486, 
		0x80000000, 
		0xc1d00000, 
		0x7f7fffff, 
		0x0e1fffff, 
		0xffbfffff, 
		0x80000100, 
		0x8f7ffffc, 
		0x0b0e0602, 
		0x0c700000, 
		0x7f7ffffe, 
		0x0e000fff, 
		0x00800002, 
		0x0c600000, 
		0xff800000, 
		0x80000004, 
		0x40a71923, 
		0x00008000, 
		0xcccedafa, 
		0x7f7fffff, 
		0xff800000, 
		0x805bf4a1, 
		0xb3ecb435, 
		0x7fc00000, 
		0x00010000, 
		0x80000000, 
		0xc25c0000, 
		0x00000000, 
		0x00000002, 
		0x00000008, 
		0xc2580000, 
		0xebcecb9f, 
		0x0c7ffc00, 
		0x7f800000, 
		0x00002000, 
		0x00800002, 
		0x00011111, 
		0x7fb4289a, 
		0xc2180000, 
		0x80000800, 
		0x7fc00001, 
		0x00800001, 
		0x0e01ffff, 
		0x42200000, 
		0x00000000, 
		0x0e0003ff, 
		0x420c0000, 
		0x638b2e67, 
		0x00400000, 
		0x00000000, 
		0xc1d80000, 
		0x00000000, 
		0x42600000, 
		0x426c0000, 
		0x2f0bfb8c, 
		0x42740000, 
		0xc2780000, 
		0xc1a80000, 
		0xff000000, 
		0x80000000, 
		0xc0400000, 
		0xe991556c, 
		0x70afb55b, 
		0xff7ffffe, 
		0x41700000, 
		0xff8d3037, 
		0x00000000, 
		0x74ecb8de, 
		0x9214e858, 
		0x20e76786, 
		0x00200000, 
		0x00000004, 
		0x80000040, 
		0xffffffff, 
		0x80800000, 
		0x3f800001, 
		0x00200000, 
		0x0e007fff, 
		0xff7ffffe, 
		0x49136032, 
		0x80800001, 
		0x41600000, 
		0x00100000, 
		0x00000000, 
		0x9b3150a4, 
		0xc0400000, 
		0x93d22cf2, 
		0x40e00000, 
		0x7fbfffff, 
		0xdf904c31, 
		0x80000000, 
		0x0d000ff0, 
		0x42540000, 
		0x0c7fff80, 
		0xc22c0000, 
		0x7fbe7aec, 
		0xc1880000, 
		0xc22c0000, 
		0x7f7fffff, 
		0x05af57ba, 
		0x4c50d9f1, 
		0x4639d25d, 
		0x00000010, 
		0x80000002, 
		0xc22c0000, 
		0x0f7fffff, 
		0xff800001, 
		0x96d87b61, 
		0x00004000, 
		0x42300000, 
		0x80800000, 
		0x80000080, 
		0x80008000, 
		0x00800000, 
		0x7f7ffffe, 
		0x41200000, 
		0x0e007fff, 
		0x00000000, 
		0x00800001, 
		0x7f800000, 
		0x5c3c068b, 
		0x41400000, 
		0xc2280000, 
		0xffffffff, 
		0xc2640000, 
		0xc2300000, 
		0x8f9a344b, 
		0xc2680000, 
		0xd4167291, 
		0xba6a55d2, 
		0x80002000, 
		0xc1700000, 
		0x41200000, 
		0x0c600000, 
		0x00000001, 
		0x80000000, 
		0x80000200, 
		0x427c0000, 
		0x41500000, 
		0x41500000, 
		0xaa0045b0, 
		0xb7f9a45a, 
		0x0c7f0000, 
		0x7fa5e48e, 
		0x0c7fffc0, 
		0xc1b80000, 
		0x00000010, 
		0x00020000, 
		0x924c4b6e, 
		0xff922ad8, 
		0xa6b73766, 
		0x9df51800, 
		0x41700000, 
		0x80000800, 
		0xc1c00000, 
		0x00800000, 
		0xff800000, 
		0x3758f748, 
		0x80000020, 
		0x106e6ad9, 
		0x80000040, 
		0xc0e00000, 
		0x00800003, 
		0xc1a00000, 
		0x8052580b, 
		0xc1c80000, 
		0x00011111, 
		0xc2540000, 
		0x41200000, 
		0xa9485c97, 
		0x6beda652, 
		0x80000004, 
		0xff800000, 
		0x0e00003f, 
		0xc1f80000, 
		0x0c7fc000, 
		0x0c4d8245, 
		0xc1800000, 
		0x4b8c4b40, 
		0x7f800001, 
		0x0e000fff, 
		0xc1a00000, 
		0xff7ffffe, 
		0x0c7ff000, 
		0xdb4f152e, 
		0xff8d0d35, 
		0xff800000, 
		0xc2440000, 
		0x0c700000, 
		0x42400000, 
		0x74517464, 
		0xff800000, 
		0x0d000ff0, 
		0xc1600000, 
		0x00020000, 
		0x0c7fc000, 
		0x437ad60f, 
		0xc2280000, 
		0x41f00000, 
		0x7f7ffffe, 
		0x80000100, 
		0x54b2e2bd, 
		0xc0000000, 
		0x33333333, 
		0x00800003, 
		0xff800000, 
		0x00000200, 
		0x41100000, 
		0xd73b1521, 
		0xc2400000, 
		0x42780000, 
		0xd9993733, 
		0x7f800000, 
		0x7f8d8144, 
		0x8ddaf422, 
		0xcb8c4b40, 
		0x40800000, 
		0x0eb1a3f8, 
		0x007fffff, 
		0x00100000, 
		0xc2540000, 
		0x42740000, 
		0xff800000, 
		0x4095e07a, 
		0x41300000, 
		0x00000100, 
		0x8f7fffff, 
		0xb92f57ac, 
		0x0e000001, 
		0x464588b0, 
		0x423c0000, 
		0x80000200, 
		0x41300000, 
		0x42780000, 
		0xf3c1150a, 
		0x0c7fe000, 
		0x00000000, 
		0x44514e10, 
		0x6d70251b, 
		0x41a00000, 
		0x42240000, 
		0xbf800000, 
		0xefdd12c8, 
		0x00000100, 
		0xc0a00000, 
		0x00000000, 
		0x0bf0093b, 
		0xff98d0a4, 
		0x3ec30202, 
		0x12ee1009, 
		0xf890781a, 
		0x0e0003ff, 
		0xd69315bf, 
		0x8c1e3742, 
		0xc1f00000, 
		0x00000010, 
		0x0f7ffffc, 
		0x5d061633, 
		0xf610d4a4, 
		0x00000001, 
		0x0c7fff00, 
		0x80000800, 
		0x0e00007f, 
		0xffc00001, 
		0xde4c1e89, 
		0x0c7fffff, 
		0x42f9b4e2, 
		0x2e117005, 
		0x80000000, 
		0xf9c08934, 
		0xff800001, 
		0x80800003, 
		0x40a00000, 
		0xc27c0000, 
		0xdd4a6a4d, 
		0xcb000000, 
		0x42040000, 
		0xd33ba391, 
		0x00000002, 
		0xfa7ecf62, 
		0xbf800001, 
		0x7f800000, 
		0xc1700000, 
		0x0e000003, 
		0x7f9493cb, 
		0x88a470f0, 
		0x00000001, 
		0x0c7fff80, 
		0x42700000, 
		0x00000400, 
		0x80800003, 
		0xbf028f5c, 
		0x7f800000, 
		0xb9540f19, 
		0x42540000, 
		0x41d00000, 
		0x00000000, 
		0xc2640000, 
		0x80800001, 
		0xc2780000, 
		0x33333333, 
		0x0e00001f, 
		0xb7c0ede9, 
		0x82326584, 
		0x00000010, 
		0xff7ffffe, 
		0x00800001, 
		0x0c7fff80, 
		0x0e01ffff, 
		0xc1100000, 
		0x41500000, 
		0x22a4ca75, 
		0x00000000, 
		0x80002000, 
		0x3fbb4df1, 
		0x80787363, 
		0x0e03ffff, 
		0x0e00ffff, 
		0x7f800000, 
		0x42600000, 
		0x80000200, 
		0xc2480000, 
		0x426c0000, 
		0x80000001, 
		0x0c7fff80, 
		0x00000200, 
		0x0e00001f, 
		0x0c7fffc0, 
		0x7b604884, 
		0x00000000, 
		0x0c7ffff0, 
		0x00800000, 
		0x7f835ea8, 
		0x7fc00000, 
		0xc1900000, 
		0x5adf6240, 
		0xff8f6100, 
		0x00000000, 
		0xab800137, 
		0xb416878a, 
		0x00000002, 
		0x40c00000, 
		0xc26c0000, 
		0xc2080000, 
		0x422c0000, 
		0x41900000, 
		0x00000040, 
		0x0c7ffff8, 
		0xffa2f40b, 
		0x00040000, 
		0xfc26ebd9, 
		0x80000004, 
		0x41700000, 
		0x00040000, 
		0x89a7e97a, 
		0x6b899862, 
		0xb77402a9, 
		0x2be0d6db, 
		0xbf028f5c, 
		0x0c7ffff8, 
		0x00000200, 
		0xc0e00000, 
		0x0e00003f, 
		0x7f9d1956, 
		0x7f800000, 
		0x00000000, 
		0x40c00000, 
		0xffc00001, 
		0x40400000, 
		0xd34f04b2, 
		0xc2200000, 
		0xc2180000, 
		0x80000004, 
		0xfc19a2ad, 
		0x0c7c0000, 
		0x3ff03e25, 
		0x856972ba, 
		0x8f7ffffc, 
		0x8f7fffff, 
		0xbf800001, 
		0x00000000, 
		0x6b43002f, 
		0x80004000, 
		0xff7ffffe, 
		0x00000001, 
		0x0f7ffffe, 
		0x80ffffff, 
		0xc2140000, 
		0x41b80000, 
		0xcf4d00d7, 
		0x0e1fffff, 
		0xff800000, 
		0x80100000, 
		0x423c0000, 
		0x41b80000, 
		0x7ac8e545, 
		0xc1d80000, 
		0xff800000, 
		0xe453e9c8, 
		0xc2340000, 
		0x4cbd0ad1, 
		0x00000000, 
		0x66894cd6, 
		0xc23c0000, 
		0x41b80000, 
		0x3e142816, 
		0xff7ffffe, 
		0x41000000, 
		0x7f800000, 
		0x00000080, 
		0x007fffff, 
		0x7f800000, 
		0xc22c0000, 
		0x42740000, 
		0x00800003, 
		0xc1500000, 
		0xeed7ebf5, 
		0x0c7fe000, 
		0x0e7fffff, 
		0xc1400000, 
		0x80800001, 
		0x040889f8, 
		0x013afdc4, 
		0xa4d1a6de, 
		0xc2000000, 
		0xd0af0f11, 
		0x00400000, 
		0xa2d65528, 
		0x43631554, 
		0x42580000, 
		0xc2740000, 
		0xc1b80000, 
		0x7f90ab42, 
		0xc2600000, 
		0x0e1fffff, 
		0xd5bcfe8a, 
		0x0663ab8d, 
		0x3abb5474, 
		0xff7ffffe, 
		0x80800002, 
		0x00800000, 
		0x00200000, 
		0x007ffffe, 
		0xc1a80000, 
		0x81bdf213, 
		0x41200000, 
		0x39ba2961, 
		0x80800001, 
		0xc0800000, 
		0x3b5f2283, 
		0x3f800001, 
		0xbd370452, 
		0x42600000, 
		0x00800003, 
		0xcb8c4b40, 
		0xff7ffffe, 
		0xc26c0000, 
		0x3f028f5c, 
		0x80000100, 
		0x8f7ffffe, 
		0xc2680000, 
		0xc2180000, 
		0x80800002, 
		0x0f7ffffe, 
		0x80011111, 
		0x0f7fffff, 
		0xd7ddafc0, 
		0xc24c0000, 
		0x00000001, 
		0x80040000, 
		0xc2480000, 
		0x0c7ff000, 
		0x0e3fffff, 
		0x55555555, 
		0x40e00000, 
		0x422c0000, 
		0xc1d80000, 
		0x80800003, 
		0x41a80000, 
		0xc0c00000, 
		0xd023c644, 
		0xc0a00000, 
		0xc1e00000, 
		0x94129b15, 
		0x41400000, 
		0x41d00000, 
		0x42140000, 
		0x80011111, 
		0xb210ae93, 
		0x41a00000, 
		0xc1200000, 
		0xbd745c98, 
		0x80800000, 
		0x4757ada1, 
		0x40c00000, 
		0xc33bf5ae, 
		0x427c0000, 
		0x7f800000, 
		0xbaf6ed83, 
		0x8c31a317, 
		0xff9b6bdd, 
		0x1991b138, 
		0xc2380000, 
		0xc2100000, 
		0x0c7ff800, 
		0x00800001, 
		0x00000800, 
		0x794de89d, 
		0xc2480000, 
		0x0c7fff80, 
		0xcccccccc, 
		0xf0186e76, 
		0x7f800000, 
		0xff800000, 
		0x2e2c445f, 
		0xcccccccc, 
		0x40000000, 
		0x8b8c3ce5, 
		0x80000002, 
		0x4b8c4b40, 
		0xbb81e0c9, 
		0xff97dd41, 
		0x7b6baeb9, 
		0x80000040, 
		0x2e09f272, 
		0x00000001, 
		0x42100000, 
		0x0e0000ff, 
		0xc2480000  
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

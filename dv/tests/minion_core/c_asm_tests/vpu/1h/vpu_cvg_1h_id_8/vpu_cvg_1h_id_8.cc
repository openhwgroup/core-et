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
			"csrwi mhpmevent3, 14\n"   
			"csrwi mhpmevent4, 27\n"   
			"csrwi mhpmevent5, 22\n"   
			"csrwi mhpmevent6, 21\n"   
			"csrwi mhpmevent7, 5\n"   
			"csrwi mhpmevent8, 6\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f0,  672 (x5)\n"
			"flw.ps f2,  416 (x5)\n"
			"flw.ps f10,  800 (x5)\n"
			"flw.ps f15,  768 (x5)\n"
			"flw.ps f22,  640 (x5)\n"
			"flw.ps f14,  0 (x5)\n"
			"flw.ps f17,  224 (x5)\n"
			"flw.ps f29,  192 (x5)\n"
			"flw.ps f20,  576 (x5)\n"
			"flw.ps f11,  480 (x5)\n"
			"flw.ps f3,  320 (x5)\n"
			"flw.ps f5,  256 (x5)\n"
			"flw.ps f4,  32 (x5)\n"
			"flw.ps f26,  928 (x5)\n"
			"flw.ps f1,  704 (x5)\n"
			"flw.ps f21,  960 (x5)\n"
			"flw.ps f31,  384 (x5)\n"
			"flw.ps f7,  448 (x5)\n"
			"flw.ps f6,  288 (x5)\n"
			"flw.ps f30,  832 (x5)\n"
			"flw.ps f18,  992 (x5)\n"
			"flw.ps f28,  544 (x5)\n"
			"flw.ps f8,  160 (x5)\n"
			"flw.ps f12,  864 (x5)\n"
			"flw.ps f27,  736 (x5)\n"
			"flw.ps f24,  96 (x5)\n"
			"flw.ps f16,  608 (x5)\n"
			"flw.ps f25,  512 (x5)\n"
			"flw.ps f19,  64 (x5)\n"
			"flw.ps f23,  128 (x5)\n"
			"flw.ps f9,  896 (x5)\n"
			"flw.ps f13,  352 (x5)\n"
			"LBL_VADDR_INIT_HID_0:\n"          
			"li x22, 0x8105237000 \n"                 
			"fsw.ps f29, (x22) \n"                 
			"li x11, 0x8105a00020 \n"                 
			"fsw.ps f13, (x11) \n"                 
			"li x18, 0x810b7dd820 \n"                 
			"fsw.ps f27, (x18) \n"                 
			"li x21, 0x810c9e0700 \n"                 
			"fsw.ps f16, (x21) \n"                 
			"li x16, 0x810cce0000 \n"                 
			"fsw.ps f9, (x16) \n"                 
			"li x10, 0x810f400760 \n"                 
			"fsw.ps f5, (x10) \n"                 
			"li x20, 0x8200315d60 \n"                 
			"fsw.ps f15, (x20) \n"                 
			"li x25, 0x8200ae0040 \n"                 
			"fsw.ps f10, (x25) \n"                 
			"li x13, 0x820275f820 \n"                 
			"fsw.ps f18, (x13) \n"                 
			"li x24, 0x8202aa0000 \n"                 
			"fsw.ps f20, (x24) \n"                 
			"li x13, 0x820333a580 \n"                 
			"fsw.ps f14, (x13) \n"                 
			"li x14, 0x8203420000 \n"                 
			"fsw.ps f16, (x14) \n"                 
			"li x21, 0x82037772e0 \n"                 
			"fsw.ps f11, (x21) \n"                 
			"li x13, 0x8204107020 \n"                 
			"fsw.ps f7, (x13) \n"                 
			"li x25, 0x8204257f20 \n"                 
			"fsw.ps f28, (x25) \n"                 
			"li x16, 0x8204b04800 \n"                 
			"fsw.ps f24, (x16) \n"                 
			"li x18, 0x8205bd2800 \n"                 
			"fsw.ps f12, (x18) \n"                 
			"li x12, 0x8206f00140 \n"                 
			"fsw.ps f29, (x12) \n"                 
			"li x16, 0x8207ae0560 \n"                 
			"fsw.ps f4, (x16) \n"                 
			"li x11, 0x8207bc0020 \n"                 
			"fsw.ps f17, (x11) \n"                 
			"li x11, 0x820a5f3060 \n"                 
			"fsw.ps f16, (x11) \n"                 
			"li x20, 0x820b05f800 \n"                 
			"fsw.ps f8, (x20) \n"                 
			"li x25, 0x820b300020 \n"                 
			"fsw.ps f20, (x25) \n"                 
			"li x20, 0x820d0f4f60 \n"                 
			"fsw.ps f29, (x20) \n"                 
			"li x18, 0x820d1006a0 \n"                 
			"fsw.ps f12, (x18) \n"                 
			"li x10, 0x820d151820 \n"                 
			"fsw.ps f10, (x10) \n"                 
			"li x15, 0x820dde0580 \n"                 
			"fsw.ps f14, (x15) \n"                 
			"fence\n"
			"LBL_GLOBAL_VADDR_INIT_HID_0:\n"          
			"li x20, 0x81225015c0 \n"                 
			"fswg.ps f22, (x20) \n"                 
			"li x10, 0x8125237000 \n"                 
			"fswg.ps f19, (x10) \n"                 
			"li x24, 0x8125a00020 \n"                 
			"fswg.ps f12, (x24) \n"                 
			"li x21, 0x8129792b80 \n"                 
			"fswg.ps f5, (x21) \n"                 
			"li x25, 0x812b7dd820 \n"                 
			"fswg.ps f31, (x25) \n"                 
			"li x25, 0x812cce0000 \n"                 
			"fswg.ps f5, (x25) \n"                 
			"li x17, 0x8130418960 \n"                 
			"fswg.ps f3, (x17) \n"                 
			"li x23, 0x813634d020 \n"                 
			"fswg.ps f7, (x23) \n"                 
			"li x13, 0x8136560380 \n"                 
			"fswg.ps f14, (x13) \n"                 
			"li x19, 0x8139420020 \n"                 
			"fswg.ps f23, (x19) \n"                 
			"li x10, 0x813d5605c0 \n"                 
			"fswg.ps f30, (x10) \n"                 
			"li x21, 0x8222aa0000 \n"                 
			"fswg.ps f17, (x21) \n"                 
			"li x10, 0x822333a580 \n"                 
			"fswg.ps f20, (x10) \n"                 
			"li x12, 0x8224107020 \n"                 
			"fswg.ps f24, (x12) \n"                 
			"li x13, 0x8224b04800 \n"                 
			"fswg.ps f20, (x13) \n"                 
			"li x19, 0x822b300020 \n"                 
			"fswg.ps f21, (x19) \n"                 
			"li x19, 0x822d151820 \n"                 
			"fswg.ps f14, (x19) \n"                 
			"li x15, 0x8230315d60 \n"                 
			"fswg.ps f2, (x15) \n"                 
			"li x19, 0x8232040020 \n"                 
			"fswg.ps f11, (x19) \n"                 
			"li x10, 0x82337772e0 \n"                 
			"fswg.ps f4, (x10) \n"                 
			"li x21, 0x8234257f20 \n"                 
			"fswg.ps f28, (x21) \n"                 
			"li x24, 0x8235bd2800 \n"                 
			"fswg.ps f4, (x24) \n"                 
			"li x19, 0x8236f00140 \n"                 
			"fswg.ps f5, (x19) \n"                 
			"li x20, 0x8237ae0560 \n"                 
			"fswg.ps f20, (x20) \n"                 
			"li x22, 0x8237bc0020 \n"                 
			"fswg.ps f20, (x22) \n"                 
			"li x14, 0x823a5f3060 \n"                 
			"fswg.ps f13, (x14) \n"                 
			"li x21, 0x823d1006a0 \n"                 
			"fswg.ps f8, (x21) \n"                 
			"fence\n"
			"LBL_LOCAL_VADDR_INIT_HID_0:\n"          
			"li x24, 0x8145a00020 \n"                 
			"fswl.ps f26, (x24) \n"                 
			"li x11, 0x8146560380 \n"                 
			"fswl.ps f0, (x11) \n"                 
			"li x14, 0x8147880000 \n"                 
			"fswl.ps f16, (x14) \n"                 
			"li x12, 0x814cce0000 \n"                 
			"fswl.ps f26, (x12) \n"                 
			"li x19, 0x8150418960 \n"                 
			"fswl.ps f19, (x19) \n"                 
			"li x16, 0x81525015c0 \n"                 
			"fswl.ps f13, (x16) \n"                 
			"li x20, 0x8155237000 \n"                 
			"fswl.ps f10, (x20) \n"                 
			"li x10, 0x8155de0000 \n"                 
			"fswl.ps f5, (x10) \n"                 
			"li x16, 0x81565de000 \n"                 
			"fswl.ps f21, (x16) \n"                 
			"li x19, 0x8159420020 \n"                 
			"fswl.ps f31, (x19) \n"                 
			"li x11, 0x815b7dd820 \n"                 
			"fswl.ps f27, (x11) \n"                 
			"li x12, 0x815c9e0700 \n"                 
			"fswl.ps f14, (x12) \n"                 
			"li x14, 0x815d4db680 \n"                 
			"fswl.ps f14, (x14) \n"                 
			"li x24, 0x8240315d60 \n"                 
			"fswl.ps f10, (x24) \n"                 
			"li x17, 0x8240ae0040 \n"                 
			"fswl.ps f28, (x17) \n"                 
			"li x19, 0x824275f820 \n"                 
			"fswl.ps f4, (x19) \n"                 
			"li x12, 0x824333a580 \n"                 
			"fswl.ps f18, (x12) \n"                 
			"li x10, 0x8246f00140 \n"                 
			"fswl.ps f1, (x10) \n"                 
			"li x18, 0x824d0f4f60 \n"                 
			"fswl.ps f2, (x18) \n"                 
			"li x25, 0x8252040020 \n"                 
			"fswl.ps f15, (x25) \n"                 
			"li x24, 0x8254107020 \n"                 
			"fswl.ps f26, (x24) \n"                 
			"li x18, 0x8254257f20 \n"                 
			"fswl.ps f20, (x18) \n"                 
			"li x21, 0x825a5f3060 \n"                 
			"fswl.ps f8, (x21) \n"                 
			"li x10, 0x825b05f800 \n"                 
			"fswl.ps f2, (x10) \n"                 
			"li x18, 0x825b0602e0 \n"                 
			"fswl.ps f24, (x18) \n"                 
			"li x16, 0x825d1006a0 \n"                 
			"fswl.ps f18, (x16) \n"                 
			"li x17, 0x825dde0580 \n"                 
			"fswl.ps f19, (x17) \n"                 
			"fence\n"
			"fence\n"
			"csrw fflags, zero\n"
			"csrwi frm, 0x0\n"
			"LBL_KERNEL_HID_0:\n"          
			"li x5, 0xFEEDFEED\n"
			"li x0, 0x5633ecef3b\n" 
			"slti x0, x0, 0x7f2\n"
			"la x31, check_sum_hid_0\n"
			"li x5, 0x5\n" 
			"sw x5, (x31)\n"
			"li x29, 0x6\n"
			"LBL_OUTER_LOOP_SEQ_HID_0:\n"
			"nop \n"
			"nop \n"
			"li x30, 0\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_0_FP_SCATTER_ID_0_HID_0:\n"
			"li x25, 0x820b30002b\n" 
			"li x5, 0x1cd54e1d76f4ef0\n"
			"fsc32b.ps f19, x5 (x25)\n" 
			"fclass.ps f19, f26\n"
			"bltu x30, x15, 1f\n"
			"1:\n"
			"fsrl.pi f9, f6, f27\n"
			"fnot.pi f10, f16\n"
			"fcmov.ps f27, f18, f4, f27\n"
			"fexp.ps f6, f3\n"
			"frcp.ps f10, f15\n"
			"fmin.s f6, f19, f6\n"
			"LBL_SEQID_0_FP_AMO_ID_1_HID_0:\n"
			"li x22, 0x8224107024\n" 
			"la x5, mem_ptr_offset\n"
			"flw.ps f13, 512(x5)\n"
			"famoxorg.pi f31, f13 (x22)\n" 
			"fnmsub.s f16, f31, f18, f16, rtz\n"
			"fminu.pi f16, f0, f27\n"
			"fpackrepb.pi f3, f3\n"
			"fcvt.ps.pwu f11, f6, rtz\n"
			"fmadd.s f16, f4, f10, f13, rmm\n"
			"fcmovm.ps f0, f0, f18\n"
			"flt.pi f9, f16, f26\n"
			"LBL_SEQID_0_FP_GATHER_ID_2_HID_0:\n"
			"li x17, 0x810c9e071c\n" 
			"la x12, mem_ptr_offset\n"
			"flw.ps f11, 116(x12)\n"
			"fscw.ps f5, f11 (x17)\n"
			"fgh.ps f0, f11 (x17)\n" 
			"flt.ps f10, f0, f16\n"
			"fsgnj.ps f9, f9, f4\n"
			"fadd.ps f10, f26, f16, rmm\n"
			"faddi.pi f15, f10, 0x1b3\n"
			"frcp.ps f15, f6\n"
			"bge x26, x24, 1f\n"
			"1:\n"
			"div x6, x16, x6\n"
			"fcvt.w.s x12, f15, rne\n"
			"LBL_SEQID_0_FP_GATHER_ID_3_HID_0:\n"
			"li x22, 0x8202040031\n" 
			"li x18, 0x134dc91dbb1e115b\n"
			"fsc32w.ps f22, x18 (x22)\n"
			"fg32w.ps f19, x18 (x22)\n" 
			"flt.ps f15, f16, f15\n"
			"fnmsub.ps f10, f4, f10, f4, rup\n"
			"fcvt.ps.f16 f13, f16\n"
			"fmin.s f6, f15, f15\n"
			"fsub.ps f18, f15, f26, rdn\n"
			"fswizz.ps f21, f15, 0x14\n"
			"fsgnjx.s f27, f15, f16\n"
			"LBL_SEQID_0_INT_LOAD_ID_4_HID_0:\n"
			"li x12, 0x8106560387\n" 
			"sd x6, -335(x12)\n"
			"ld x5, -335(x12)\n" 
			"fltu.pi f15, f13, f26\n"
			"srli x5, x18, 3\n"
			"fcvt.f16.ps f19, f16\n"
			"beq x22, x21, 1f\n"
			"1:\n"
			"remw x0, x0, x22\n"
			"fcvt.w.s x0, f27, rmm\n"
			"divuw x18, x25, x18\n"
			"srli x18, x18, 0\n"
			"LBL_SEQID_0_FP_SCATTER_ID_5_HID_0:\n"
			"li x5, 0x8202aa0010\n" 
			"li x18, 0x78098d767a0de573\n"
			"fsc32b.ps f15, x18 (x5)\n" 
			"fnmsub.ps f31, f26, f10, f10, rtz\n"
			"fltu.pi f11, f0, f15\n"
			"bne x2, x20, 1f\n"
			"1:\n"
			"fle.ps f4, f4, f0\n"
			"for.pi f19, f10, f15\n"
			"fcvt.f16.ps f26, f3\n"
			"fnot.pi f3, f21\n"
			"beq x24, x12, 1f\n"
			"1:\n"
			"fmul.ps f10, f11, f26, rtz\n"
			"blt x12, x12, 1f\n"
			"1:\n"
			"LBL_SEQID_0_INT_AMO_ID_6_HID_0:\n"
			"li x15, 0x822333a588\n" 
			"amoming.d x17, x12, (x15)\n" 
			"remu x5, x9, x9\n"
			"fle.s x0, f3, f18\n"
			"fmv.s.x f27, x5\n"
			"beq x19, x5, 1f\n"
			"1:\n"
			"fle.pi f4, f3, f4\n"
			"sltiu x15, x15, 0x16\n"
			"flem.ps m2, f4, f0\n"
			"fcmovm.ps f27, f19, f0\n"
			"LBL_SEQID_0_FP_AMO_ID_7_HID_0:\n"
			"li x5, 0x824333a588\n" 
			"la x15, mem_ptr_offset\n"
			"flw.ps f19, 256(x15)\n"
			"famominl.ps f10, f19 (x5)\n" 
			"flog.ps f19, f13\n"
			"fsatu8.pi f10, f16\n"
			"fsrl.pi f31, f31, f31\n"
			"fmax.pi f16, f16, f19\n"
			"fmin.s f18, f10, f16\n"
			"frcp.ps f31, f31\n"
			"fmin.ps f27, f4, f13\n"
			"LBL_SEQID_0_FP_AMO_ID_8_HID_0:\n"
			"li x5, 0x822d151828\n" 
			"la x6, mem_ptr_offset\n"
			"flw.ps f4, 928(x6)\n"
			"famoming.ps f4, f4 (x5)\n" 
			"fle.ps f31, f31, f31\n"
			"faddi.pi f27, f11, 0x81\n"
			"beq x21, x27, 1f\n"
			"1:\n"
			"fsgnjn.s f26, f11, f16\n"
			"fnmadd.ps f16, f6, f11, f15, rtz\n"
			"fcvt.pwu.ps f3, f16, rtz\n"
			"beq x22, x7, 1f\n"
			"1:\n"
			"fnmadd.s f26, f15, f3, f11, rmm\n"
			"bgeu x7, x30, 1f\n"
			"1:\n"
			"fsrli.pi f26, f27, 0xf\n"
			"LBL_SEQID_0_FP_GATHER_ID_9_HID_0:\n"
			"li x16, 0x8203420013\n" 
			"li x17, 0x4b692227a3d5c979\n"
			"fsc32w.ps f13, x17 (x16)\n"
			"fg32w.ps f9, x17 (x16)\n" 
			"ffrc.ps f27, f13\n"
			"fsgnjn.s f18, f15, f16\n"
			"packb x5, x18, x22\n"
			"divuw x0, x25, x25\n"
			"divu x5, x15, x18\n"
			"feq.ps f19, f3, f3\n"
			"remw x0, x15, x25\n"
			"LBL_SEQID_0_INT_AMO_ID_10_HID_0:\n"
			"li x22, 0x812d4db698\n" 
			"amoming.d x15, x25, (x22)\n" 
			"or x5, x25, x5\n"
			"bge x4, x4, 1f\n"
			"1:\n"
			"fcvt.ps.pw f11, f18, rne\n"
			"remu x6, x16, x6\n"
			"feq.ps f18, f15, f3\n"
			"fsgnjx.ps f11, f15, f15\n"
			"ffrc.ps f21, f6\n"
			"fltm.pi m3, f21, f3\n"
			"LBL_SEQID_0_FP_SCATTER_ID_11_HID_0:\n"
			"li x15, 0x8207bc0036\n" 
			"li x6, 0x2b26974a2d1070ff\n"
			"fsc32h.ps f9, x6 (x15)\n" 
			"sltu x12, x25, x16\n"
			"fmadd.ps f16, f21, f0, f3, rtz\n"
			"fmv.s.x f21, x16\n"
			"mov.m.x m3, x18, 0x72\n"
			"feq.ps f11, f4, f19\n"
			"blt x28, x7, 1f\n"
			"1:\n"
			"srlw x9, x22, x22\n"
			"add x12, x12, x12\n"
			"LBL_SEQID_0_INT_AMO_ID_12_HID_0:\n"
			"li x16, 0x8145a00038\n" 
			"amominl.d x17, x5, (x16)\n" 
			"fmul.pi f9, f16, f6\n"
			"fmax.s f9, f16, f9\n"
			"bne x14, x5, 1f\n"
			"1:\n"
			"flog.ps f9, f19\n"
			"feq.pi f21, f6, f19\n"
			"fmax.ps f16, f16, f11\n"
			"bltu x11, x2, 1f\n"
			"1:\n"
			"fmul.s f21, f18, f19, rtz\n"
			"fmulh.pi f15, f6, f3\n"
			"LBL_SEQID_0_INT_AMO_ID_13_HID_0:\n"
			"li x25, 0x8125237004\n" 
			"amoming.w x9, x5, (x25)\n" 
			"divuw x5, x9, x9\n"
			"fltu.pi f9, f10, f27\n"
			"flog.ps f9, f26\n"
			"fslli.pi f27, f27, 0x2\n"
			"fpackreph.pi f4, f4\n"
			"fxor.pi f16, f4, f10\n"
			"fmulhu.pi f9, f11, f4\n"
			"LBL_SEQID_0_FP_AMO_ID_14_HID_0:\n"
			"li x15, 0x824275f82c\n" 
			"la x12, mem_ptr_offset\n"
			"flw.ps f19, 608(x12)\n"
			"famominl.ps f9, f19 (x15)\n" 
			"fpackreph.pi f26, f6\n"
			"fsub.s f31, f19, f16, rup\n"
			"fmul.pi f6, f19, f6\n"
			"fmulh.pi f9, f16, f6\n"
			"ffrc.ps f19, f6\n"
			"fmsub.ps f9, f19, f6, f6, rtz\n"
			"bne x14, x10, 1f\n"
			"1:\n"
			"fminu.pi f6, f6, f6\n"
			"LBL_SEQID_0_INT_LOAD_ID_15_HID_0:\n"
			"li x16, 0x8105237005\n" 
			"sd x22, -1525(x16)\n"
			"lw x15, -1525(x16)\n" 
			"fsub.pi f31, f6, f6\n"
			"flt.ps f19, f26, f9\n"
			"fcvt.pw.ps f15, f11, rup\n"
			"fmin.s f19, f21, f13\n"
			"fcvt.ps.pw f11, f9, rne\n"
			"bgeu x22, x19, 1f\n"
			"1:\n"
			"fadd.pi f15, f15, f31\n"
			"frcp.ps f16, f11\n"
			"LBL_SEQID_0_INT_STORE_ID_16_HID_0:\n"
			"li x25, 0x8202040031\n" 
			"sw x17, -63(x25)\n" 
			"fsrli.pi f21, f31, 0xe\n"
			"fle.pi f21, f4, f21\n"
			"rem x17, x15, x18\n"
			"fmvz.x.ps x18, f27, 5\n"
			"fmsub.ps f26, f21, f3, f18, rne\n"
			"fsgnjx.s f21, f26, f27\n"
			"andi x15, x12, 0xd\n"
			"LBL_SEQID_0_FP_L_LOAD_ID_17_HID_0:\n"
			"li x6, 0x8155237000\n" 
			"mov.m.x m0, zero, 0xff\n"
			"fswl.ps f16, (x6)\n"
			"flwl.ps f15, (x6)\n" 
			"fpackrepb.pi f13, f3\n"
			"fcvt.ps.f16 f0, f13\n"
			"fle.ps f15, f3, f15\n"
			"fle.pi f31, f3, f26\n"
			"blt x21, x21, 1f\n"
			"1:\n"
			"fsgnjx.ps f4, f0, f11\n"
			"fadd.ps f3, f13, f3, rdn\n"
			"fsrl.pi f21, f3, f4\n"
			"LBL_SEQID_0_INT_AMO_ID_18_HID_0:\n"
			"li x6, 0x823d1006a0\n" 
			"amoming.d x12, x16, (x6)\n" 
			"fnmsub.s f10, f13, f18, f10, rne\n"
			"faddi.pi f19, f13, 0x102\n"
			"bne x12, x6, 1f\n"
			"1:\n"
			"fnot.pi f19, f26\n"
			"fsgnjx.s f26, f16, f19\n"
			"fsat8.pi f10, f18\n"
			"fxor.pi f6, f18, f18\n"
			"fexp.ps f31, f31\n"
			"LBL_SEQID_0_FP_GATHER_ID_19_HID_0:\n"
			"li x12, 0x8109792b95\n" 
			"li x18, 0xbc80d95bdcda1edc\n"
			"fsc32w.ps f24, x18 (x12)\n"
			"fg32b.ps f9, x18 (x12)\n" 
			"fcvt.f16.ps f13, f4\n"
			"fcvt.pwu.ps f31, f4, rdn\n"
			"bge x25, x20, 1f\n"
			"1:\n"
			"fmulhu.pi f3, f10, f27\n"
			"fmsub.s f10, f19, f19, f16, rmm\n"
			"for.pi f6, f11, f3\n"
			"fcvt.f16.ps f19, f3\n"
			"fnmadd.ps f9, f18, f11, f16, rup\n"
			"bltu x7, x4, 1f\n"
			"1:\n"
			"LBL_SEQID_0_INT_STORE_ID_20_HID_0:\n"
			"li x9, 0x8109420031\n" 
			"sd x12, 253(x9)\n" 
			"divu x22, x22, x5\n"
			"fle.s x17, f6, f31\n"
			"maskxor m2, m2, m2\n"
			"fsetm.pi m2, f0\n"
			"divu x6, x5, x6\n"
			"bltu x18, x30, 1f\n"
			"1:\n"
			"fadd.pi f10, f15, f19\n"
			"bltu x9, x4, 1f\n"
			"1:\n"
			"fsgnj.ps f15, f15, f11\n"
			"LBL_SEQID_0_INT_AMO_ID_21_HID_0:\n"
			"li x16, 0x813c9e0718\n" 
			"amoming.d x15, x17, (x16)\n" 
			"fsub.ps f16, f9, f3, rtz\n"
			"fmin.ps f31, f3, f11\n"
			"fnmadd.ps f10, f3, f11, f3, rmm\n"
			"fnmadd.ps f9, f9, f9, f3, rup\n"
			"fand.pi f3, f9, f3\n"
			"fmulh.pi f10, f11, f11\n"
			"fsra.pi f18, f9, f3\n"
			"LBL_SEQID_0_FP_G_GATHER_ID_22_HID_0:\n"
			"li x17, 0x8235bd2810\n" 
			"la x25, mem_ptr_offset\n"
			"flq2 f0, 124(x25)\n"
			"fscwg.ps f28, f0 (x17)\n"
			"fgwg.ps f0, f0 (x17)\n" 
			"maskor m7, m3, m7\n"
			"addiw x0, x18, 0x1b\n"
			"fcvt.w.s x17, f18, rup\n"
			"bgeu x12, x2, 1f\n"
			"1:\n"
			"and x16, x15, x0\n"
			"bge x10, x20, 1f\n"
			"1:\n"
			"packb x9, x18, x25\n"
			"bltu x3, x4, 1f\n"
			"1:\n"
			"flem.ps m3, f26, f13\n"
			"divuw x0, x6, x0\n"
			"LBL_SEQID_0_FP_G_GATHER_ID_23_HID_0:\n"
			"li x25, 0x823a5f3068\n" 
			"la x6, mem_ptr_offset\n"
			"flw.ps f11, 16(x6)\n"
			"fscwg.ps f17, f11 (x25)\n"
			"fgbg.ps f9, f11 (x25)\n" 
			"fnmsub.s f3, f21, f18, f9, rtz\n"
			"ffrc.ps f9, f9\n"
			"srliw x16, x17, 5\n"
			"auipc x5, 0x1f\n"
			"fcvt.ps.f16 f10, f21\n"
			"srliw x12, x18, 1\n"
			"sltiu x9, x6, 0x5\n"
			"LBL_SEQID_0_FP_GATHER_ID_24_HID_0:\n"
			"li x18, 0x8204107025\n" 
			"li x9, 0x7ddc72653b87d6da\n"
			"fsc32w.ps f31, x9 (x18)\n"
			"fg32h.ps f10, x9 (x18)\n" 
			"fmv.x.s x15, f19\n"
			"bge x2, x11, 1f\n"
			"1:\n"
			"fsra.pi f19, f19, f31\n"
			"packb x25, x16, x25\n"
			"bne x19, x28, 1f\n"
			"1:\n"
			"mulh x25, x12, x25\n"
			"bltu x17, x26, 1f\n"
			"1:\n"
			"fltu.pi f4, f31, f10\n"
			"fadd.ps f10, f10, f21, rup\n"
			"flog.ps f4, f21\n"
			"LBL_SEQID_0_FP_L_STORE_ID_25_HID_0:\n"
			"li x25, 0x8240ae0040\n" 
			"mov.m.x m0, zero, 0xff\n"
			"fswl.ps f11, (x25)\n" 
			"fsra.pi f19, f0, f21\n"
			"fcmovm.ps f0, f0, f18\n"
			"fmulhu.pi f13, f31, f3\n"
			"flog.ps f11, f11\n"
			"fsatu8.pi f15, f18\n"
			"bge x1, x12, 1f\n"
			"1:\n"
			"fmulh.pi f0, f15, f13\n"
			"bge x12, x0, 1f\n"
			"1:\n"
			"fadd.s f18, f3, f4, rup\n"
			"LBL_SEQID_0_FP_L_SCATTER_ID_26_HID_0:\n"
			"li x18, 0x814634d038\n" 
			"la x5, mem_ptr_offset\n"
			"flw.ps f3, 28(x5)\n"
			"fscbl.ps f21, f3 (x18)\n" 
			"sub x17, x22, x16\n"
			"srai x25, x6, 2\n"
			"bgeu x9, x21, 1f\n"
			"1:\n"
			"feqm.ps m2, f18, f27\n"
			"fmvz.x.ps x0, f27, 5\n"
			"fnmadd.s f31, f15, f11, f31, rdn\n"
			"bge x28, x9, 1f\n"
			"1:\n"
			"fnmsub.s f9, f31, f16, f27, rdn\n"
			"sllw x16, x17, x15\n"
			"blt x26, x31, 1f\n"
			"1:\n"
			"LBL_SEQID_0_FP_G_STORE_ID_27_HID_0:\n"
			"li x15, 0x8127880000\n" 
			"mov.m.x m0, zero, 0xff\n"
			"fswg.ps f11, (x15)\n" 
			"fcvt.pwu.ps f16, f11, rdn\n"
			"flog.ps f13, f4\n"
			"fxor.pi f10, f27, f3\n"
			"bge x25, x8, 1f\n"
			"1:\n"
			"fslli.pi f27, f27, 0xe\n"
			"feq.ps f9, f9, f0\n"
			"blt x30, x16, 1f\n"
			"1:\n"
			"feq.pi f19, f27, f4\n"
			"fmax.ps f11, f21, f13\n"
			"beq x26, x14, 1f\n"
			"1:\n"
			"LBL_SEQID_0_INT_AMO_ID_28_HID_0:\n"
			"li x25, 0x8155237004\n" 
			"amomaxl.w x18, x22, (x25)\n" 
			"fnmadd.ps f19, f13, f11, f16, rup\n"
			"for.pi f9, f9, f9\n"
			"fcvt.s.w f11, x5, rup\n"
			"fandi.pi f6, f0, 0x99\n"
			"fsgnj.s f11, f26, f13\n"
			"fmax.s f11, f3, f9\n"
			"maskor m5, m7, m7\n"
			"bgeu x29, x7, 1f\n"
			"1:\n"
			"LBL_SEQID_0_FP_AMO_ID_29_HID_0:\n"
			"li x22, 0x825b300028\n" 
			"la x25, mem_ptr_offset\n"
			"flw.ps f3, 0(x25)\n"
			"famomaxl.pi f16, f3 (x22)\n" 
			"fsub.pi f4, f3, f3\n"
			"frsq.ps f0, f26\n"
			"fmul.s f9, f27, f18, rne\n"
			"fsll.pi f27, f26, f9\n"
			"fsgnjn.ps f31, f26, f26\n"
			"fle.ps f21, f21, f11\n"
			"fsgnjx.s f9, f9, f27\n"
			"LBL_SEQID_0_INT_LOAD_ID_30_HID_0:\n"
			"li x25, 0x8200ae005e\n" 
			"sd x5, 705(x25)\n"
			"lb x25, 705(x25)\n" 
			"andi x25, x17, 0x15\n"
			"srai x18, x0, 0\n"
			"bgeu x3, x20, 1f\n"
			"1:\n"
			"masknot m3, m3\n"
			"bltu x6, x30, 1f\n"
			"1:\n"
			"srlw x9, x15, x0\n"
			"fcvt.wu.s x6, f6, rup\n"
			"bne x29, x17, 1f\n"
			"1:\n"
			"fsra.pi f11, f31, f10\n"
			"fsrai.pi f15, f9, 0xc\n"
			"LBL_SEQID_0_FP_AMO_ID_31_HID_0:\n"
			"li x9, 0x8252040030\n" 
			"la x16, mem_ptr_offset\n"
			"flw.ps f0, 480(x16)\n"
			"famoaddl.pi f0, f0 (x9)\n" 
			"ecall\n"
			"fcvt.ps.pw f27, f4, rdn\n"
			"xor x5, x18, x17\n"
			"feq.s x22, f9, f27\n"
			"flog.ps f4, f4\n"
			"fsgnj.ps f18, f16, f6\n"
			"mova.m.x x9\n"
			"mov.m.x m0, x0, 0xFF\n"
			"li x5, 0x20\n"
			"la x31, rand_ieee754_32\n"
			"add x31, x31, x5\n"
			"flw.ps f0, 0(x31)\n"
			"flw.ps f10, 1(x31)\n"
			"flw.ps f15, 4(x31)\n"
			"flw.ps f16, 11(x31)\n"
			"flw.ps f19, 9(x31)\n"
			"flw.ps f3, 16(x31)\n"
			"flw.ps f4, 11(x31)\n"
			"la x31, rand_int_32\n"
			"add x31, x31, x5\n"
			"lw x12, 0(x31)\n"
			"lw x15, 2(x31)\n"
			"lw x16, 7(x31)\n"
			"lw x17, 8(x31)\n"
			"lw x22, 13(x31)\n"
			"lw x5, 8(x31)\n"
			"li x21, 0x3\n"
			"LBL_SEQID_0_LOOP_SEQ_HID_0:\n"
			"fle.ps f27, f19, f16\n"
			"remuw x22, x22, x16\n"
			"mulhu x15, x15, x12\n"
			"bltu x21, x27, 1f\n"
			"1:\n"
			"srl x5, x17, x17\n"
			"fmaxu.pi f0, f4, f4\n"
			"mulhu x22, x5, x22\n"
			"bltu x5, x18, 1f\n"
			"1:\n"
			"fmv.x.s x0, f0\n"
			"xori x5, x5, 0x1c\n"
			"blt x0, x23, 1f\n"
			"1:\n"
			"fexp.ps f26, f19\n"
			"fmsub.s f0, f0, f4, f16, rmm\n"
			"fsin.ps f18, f10\n"
			"sra x22, x16, x17\n"
			"and x25, x15, x12\n"
			"addw x25, x17, x12\n"
			"divu x22, x12, x17\n"
			"packb x16, x12, x16\n"
			"fsetm.pi m3, f15\n"
			"fmvz.x.ps x17, f4, 3\n"
			"fcmovm.ps f18, f19, f0\n"
			"fcvt.s.w f26, x15, rup\n"
			"fpackrepb.pi f4, f4\n"
			"bltu x21, x0, 1f\n"
			"1:\n"
			"fswizz.ps f15, f4, 0x1b\n"
			"frsq.ps f6, f10\n"
			"fsub.pi f27, f4, f3\n"
			"feq.ps f3, f3, f3\n"
			"div x17, x16, x17\n"
			"fmadd.s f27, f0, f10, f15, rup\n"
			"beq x21, x23, 1f\n"
			"1:\n"
			"ebreak\n"
			"fcvt.s.wu f15, x12, rdn\n"
			"bltu x21, x7, 1f\n"
			"1:\n"
			"fmvz.x.ps x25, f0, 6\n"
			"slliw x25, x17, 0\n"
			"bgeu x20, x2, 1f\n"
			"1:\n"
			"fsat8.pi f19, f4\n"
			"srlw x22, x15, x22\n"
			"divuw x16, x16, x17\n"
			"fcvt.pwu.ps f6, f15, rdn\n"
			"beq x17, x24, 1f\n"
			"1:\n"
			"fbcx.ps f15, x12\n"
			"fmax.pi f0, f16, f15\n"
			"fsrli.pi f19, f3, 0x8\n"
			"blt x31, x17, 1f\n"
			"1:\n"
			"fsgnj.s f3, f3, f16\n"
			"fcvt.s.wu f3, x12, rmm\n"
			"fcvt.s.w f3, x5, rtz\n"
			"xor x12, x17, x12\n"
			"mova.x.m x5\n"
			"fnmsub.s f0, f10, f10, f0, rtz\n"
			"blt x6, x18, 1f\n"
			"1:\n"
			"and x17, x15, x17\n"
			"flt.pi f10, f10, f0\n"
			"ebreak\n"
			"fsrli.pi f0, f10, 0xf\n"
			"or x15, x12, x15\n"
			"fmax.ps f26, f10, f0\n"
			"feqm.ps m5, f10, f10\n"
			"mulhu x0, x5, x12\n"
			"remuw x5, x17, x5\n"
			"bltu x20, x20, 1f\n"
			"1:\n"
			"add x5, x17, x5\n"
			"fsetm.pi m5, f16\n"
			"bltu x8, x0, 1f\n"
			"1:\n"
			"fnmadd.ps f26, f4, f19, f4, rdn\n"
			"fmax.s f16, f16, f0\n"
			"fadd.pi f4, f4, f16\n"
			"maskpopcz x25, m5\n"
			"fle.pi f6, f19, f3\n"
			"fle.ps f4, f10, f4\n"
			"fmvs.x.ps x0, f4, 2\n"
			"blt x6, x12, 1f\n"
			"1:\n"
			"packb x25, x22, x15\n"
			"blt x17, x1, 1f\n"
			"1:\n"
			"fsrai.pi f19, f19, 0xa\n"
			"and x0, x5, x12\n"
			"fcvt.w.s x17, f19, rdn\n"
			"fswizz.ps f15, f3, 0x1d\n"
			"fcmovm.ps f3, f3, f16\n"
			"sra x15, x17, x15\n"
			"feqm.ps m3, f10, f15\n"
			"sll x17, x22, x17\n"
			"beq x15, x25, 1f\n"
			"1:\n"
			"fmul.s f6, f16, f3, rup\n"
			"bge x27, x30, 1f\n"
			"1:\n"
			"faddi.pi f27, f19, 0xfb\n"
			"sra x0, x5, x12\n"
			"bgeu x15, x23, 1f\n"
			"1:\n"
			"fmul.s f27, f15, f16, rtz\n"
			"lui x17, 0x6\n"
			"divw x5, x15, x22\n"
			"or x0, x22, x16\n"
			"fmin.s f26, f15, f15\n"
			"mov.m.x m5, x22, 0x3\n"
			"bltu x8, x29, 1f\n"
			"1:\n"
			"fmaxu.pi f10, f10, f19\n"
			"blt x18, x5, 1f\n"
			"1:\n"
			"fle.pi f6, f15, f10\n"
			"sub x17, x16, x12\n"
			"fle.ps f3, f3, f15\n"
			"srl x5, x12, x16\n"
			"bgeu x31, x10, 1f\n"
			"1:\n"
			"fcvt.w.s x22, f15, rdn\n"
			"slti x0, x16, 0xe\n"
			"bge x26, x28, 1f\n"
			"1:\n"
			"fle.ps f16, f4, f10\n"
			"mul x17, x15, x15\n"
			"bge x12, x16, 1f\n"
			"1:\n"
			"fltu.pi f6, f3, f15\n"
			"remuw x25, x22, x16\n"
			"fround.ps f10, f3, rdn\n"
			"mul x0, x16, x22\n"
			"fmvs.x.ps x17, f4, 0\n"
			"beq x11, x31, 1f\n"
			"1:\n"
			"fround.ps f4, f15, rmm\n"
			"blt x23, x4, 1f\n"
			"1:\n"
			"ffrc.ps f19, f3\n"
			"fadd.s f19, f3, f0, rmm\n"
			"fmsub.s f18, f4, f19, f15, rdn\n"
			"fandi.pi f6, f0, 0x1ec\n"
			"ffrc.ps f6, f3\n"
			"div x5, x17, x22\n"
			"bltu x31, x13, 1f\n"
			"1:\n"
			"fsatu8.pi f10, f4\n"
			"fcmovm.ps f18, f3, f16\n"
			"fence\n"
			"fclass.s x5, f4\n"
			"fmax.ps f3, f10, f3\n"
			"fswizz.ps f10, f15, 0x1e\n"
			"blt x26, x9, 1f\n"
			"1:\n"
			"fclass.ps f19, f15\n"
			"fsgnjn.s f26, f15, f4\n"
			"bltu x31, x23, 1f\n"
			"1:\n"
			"sraw x17, x12, x5\n"
			"bge x11, x16, 1f\n"
			"1:\n"
			"fmul.pi f26, f3, f16\n"
			"bgeu x29, x15, 1f\n"
			"1:\n"
			"srl x15, x15, x12\n"
			"fsgnj.s f26, f3, f15\n"
			"maskpopc x5, m5\n"
			"divw x17, x16, x5\n"
			"bge x31, x12, 1f\n"
			"1:\n"
			"sltiu x15, x15, 0x1b\n"
			"andi x5, x17, 0x1c\n"
			"fcvt.f16.ps f3, f3\n"
			"beq x27, x14, 1f\n"
			"1:\n"
			"srlw x0, x17, x12\n"
			"mulhu x5, x16, x22\n"
			"bge x0, x16, 1f\n"
			"1:\n"
			"fmv.s.x f19, x17\n"
			"flt.s x22, f4, f0\n"
			"lui x5, 0xe\n"
			"fcvt.w.s x17, f3, rmm\n"
			"masknot m2, m5\n"
			"subw x25, x22, x22\n"
			"sra x5, x5, x16\n"
			"addi x21, x21, -1\n"
			"bne x21, x0, LBL_SEQID_0_LOOP_SEQ_HID_0\n"
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_0\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_1_FP_G_STORE_ID_32_HID_0:\n"
			"li x22, 0x8236f00140\n" 
			"mov.m.x m0, zero, 0xff\n"
			"fswg.ps f0, (x22)\n" 
			"fmadd.ps f18, f6, f6, f18, rdn\n"
			"beq x26, x9, 1f\n"
			"1:\n"
			"fsrl.pi f3, f3, f15\n"
			"fnmsub.s f19, f15, f13, f10, rdn\n"
			"fsub.pi f19, f3, f18\n"
			"LBL_SEQID_1_FP_STORE_ID_33_HID_0:\n"
			"li x25, 0x8105de0019\n" 
			"fsw.ps f31, 1150(x25)\n" 
			"fsgnj.ps f6, f26, f13\n"
			"fand.pi f0, f26, f26\n"
			"beq x12, x4, 1f\n"
			"1:\n"
			"fpackreph.pi f4, f13\n"
			"fadd.s f4, f13, f26, rdn\n"
			"LBL_SEQID_1_INT_G_STORE_ID_34_HID_0:\n"
			"li x16, 0x823d0f4f78\n" 
			"shg x5, (x16)\n" 
			"flt.pi f10, f0, f16\n"
			"fnmsub.s f27, f13, f21, f16, rdn\n"
			"fmul.pi f10, f21, f10\n"
			"fcmov.ps f27, f13, f21, f18\n"
			"LBL_SEQID_1_INT_AMO_ID_35_HID_0:\n"
			"li x25, 0x825b05f81c\n" 
			"amominl.w x6, x16, (x25)\n" 
			"fnot.pi f13, f13\n"
			"bgeu x23, x3, 1f\n"
			"1:\n"
			"mulhsu x18, x18, x0\n"
			"fltm.pi m3, f13, f4\n"
			"divuw x22, x22, x22\n"
			"LBL_SEQID_1_FP_STORE_ID_36_HID_0:\n"
			"li x5, 0x820b0602fe\n" 
			"fsw f11, -1068(x5)\n" 
			"fslli.pi f19, f3, 0x3\n"
			"fsub.ps f21, f21, f31, rne\n"
			"fsin.ps f4, f4\n"
			"fnmsub.s f16, f6, f31, f19, rtz\n"
			"bgeu x1, x0, 1f\n"
			"1:\n"
			"LBL_SEQID_1_FP_STORE_ID_37_HID_0:\n"
			"li x15, 0x820d1006a7\n" 
			"fsq2 f3, -1346(x15)\n" 
			"fmul.s f15, f13, f27, rdn\n"
			"beq x24, x4, 1f\n"
			"1:\n"
			"fadd.pi f10, f4, f10\n"
			"fnmsub.ps f18, f10, f19, f27, rtz\n"
			"fsgnjx.s f4, f18, f27\n"
			"LBL_SEQID_1_INT_AMO_ID_38_HID_0:\n"
			"li x5, 0x8233420010\n" 
			"amoming.d x9, x16, (x5)\n" 
			"fmsub.s f10, f13, f10, f10, rup\n"
			"fclass.ps f31, f15\n"
			"fminu.pi f9, f31, f9\n"
			"fadd.ps f31, f31, f13, rmm\n"
			"LBL_SEQID_1_FP_SCATTER_ID_39_HID_0:\n"
			"li x5, 0x820a5f306a\n" 
			"la x12, mem_ptr_offset\n"
			"flq2 f3, 108(x12)\n"
			"fsch.ps f9, f3 (x5)\n" 
			"fandi.pi f9, f16, 0x60\n"
			"fsra.pi f11, f16, f6\n"
			"blt x25, x18, 1f\n"
			"1:\n"
			"fnmsub.ps f13, f6, f0, f6, rne\n"
			"bge x23, x12, 1f\n"
			"1:\n"
			"fcvt.f16.ps f6, f9\n"
			"LBL_SEQID_1_CSR_ID_40_HID_0:\n"
			"csrr x0, mtval\n"
			"fcvt.pwu.ps f9, f18, rne\n"
			"bge x6, x11, 1f\n"
			"1:\n"
			"fsrli.pi f11, f15, 0x4\n"
			"fmax.pi f26, f3, f3\n"
			"sraw x15, x22, x25\n"
			"LBL_SEQID_1_FP_G_LOAD_ID_41_HID_0:\n"
			"li x5, 0x813634d020\n" 
			"mov.m.x m0, zero, 0xff\n"
			"fswg.ps f11, (x5)\n"
			"flwg.ps f21, (x5)\n" 
			"fnmsub.ps f11, f4, f11, f15, rtz\n"
			"bgeu x28, x19, 1f\n"
			"1:\n"
			"fsgnjn.ps f0, f15, f9\n"
			"fmax.s f16, f9, f15\n"
			"bltu x6, x4, 1f\n"
			"1:\n"
			"fmadd.ps f9, f9, f26, f9, rdn\n"
			"bltu x10, x0, 1f\n"
			"1:\n"
			"LBL_SEQID_1_INT_AMO_ID_42_HID_0:\n"
			"li x15, 0x812b7dd828\n" 
			"amoaddg.d x12, x6, (x15)\n" 
			"fsgnjn.s f31, f18, f18\n"
			"bge x3, x14, 1f\n"
			"1:\n"
			"fclass.ps f6, f26\n"
			"fle.pi f13, f15, f3\n"
			"fmin.s f6, f26, f4\n"
			"LBL_SEQID_1_INT_LOAD_ID_43_HID_0:\n"
			"li x5, 0x820b30002b\n" 
			"sd x12, -901(x5)\n"
			"lh x18, -901(x5)\n" 
			"frcp.ps f18, f21\n"
			"srai x16, x15, 5\n"
			"bltu x11, x28, 1f\n"
			"1:\n"
			"sllw x6, x6, x18\n"
			"fcvt.f16.ps f18, f18\n"
			"LBL_SEQID_1_INT_L_STORE_ID_44_HID_0:\n"
			"li x6, 0x814634d038\n" 
			"shl x9, (x6)\n" 
			"fxor.pi f13, f13, f26\n"
			"fbci.ps f6, 0x5fb86\n"
			"maskpopcz x5, m3\n"
			"sllw x0, x25, x18\n"
			"LBL_SEQID_1_FP_AMO_ID_45_HID_0:\n"
			"li x17, 0x814f400770\n" 
			"la x6, mem_ptr_offset\n"
			"flw.ps f9, 768(x6)\n"
			"famomaxl.ps f10, f9 (x17)\n" 
			"fsatu8.pi f15, f26\n"
			"fmulhu.pi f21, f19, f16\n"
			"fsrai.pi f21, f26, 0xd\n"
			"fcvt.pwu.ps f0, f3, rtz\n"
			"LBL_SEQID_1_FP_AMO_ID_46_HID_0:\n"
			"li x15, 0x8252040030\n" 
			"la x25, mem_ptr_offset\n"
			"flw.ps f27, 416(x25)\n"
			"famomaxl.ps f11, f27 (x15)\n" 
			"fmulhu.pi f31, f19, f21\n"
			"fmin.pi f18, f4, f21\n"
			"slli x5, x17, 1\n"
			"divw x18, x18, x0\n"
			"fence.i\n" 
			"fnot.pi f26, f11\n"
			"for.pi f13, f9, f6\n"
			"fsll.pi f21, f9, f11\n"
			"fcvt.ps.pw f4, f6, rdn\n"
			"bge x20, x1, 1f\n"
			"1:\n"
			"LBL_SEQID_1_INT_G_STORE_ID_47_HID_0:\n"
			"li x12, 0x822d151828\n" 
			"sbg x22, (x12)\n" 
			"fnot.pi f4, f21\n"
			"sll x25, x25, x18\n"
			"fmin.pi f6, f6, f13\n"
			"fcvt.s.w f27, x9, rup\n"
			"LBL_SEQID_1_INT_STORE_ID_48_HID_0:\n"
			"li x18, 0x8202aa0010\n" 
			"sw x22, -258(x18)\n" 
			"fnmsub.s f3, f26, f21, f26, rtz\n"
			"feq.pi f3, f26, f21\n"
			"bltu x27, x23, 1f\n"
			"1:\n"
			"fsub.pi f4, f21, f21\n"
			"fcvt.ps.pwu f21, f21, rtz\n"
			"LBL_SEQID_1_INT_LOAD_ID_49_HID_0:\n"
			"li x25, 0x8105de0019\n" 
			"sd x18, 104(x25)\n"
			"lhu x18, 104(x25)\n" 
			"sub x16, x12, x22\n"
			"fslli.pi f18, f18, 0xd\n"
			"mulhsu x17, x15, x16\n"
			"fsat8.pi f10, f3\n"
			"blt x8, x5, 1f\n"
			"1:\n"
			"la x15, rand_ieee754_32\n"
			"flw.ps f10, 0(x15)\n"
			"fsqrt.s f13, f10, rup\n" 
			"fltm.pi m7, f15, f18\n"
			"blt x20, x1, 1f\n"
			"1:\n"
			"fclass.s x0, f15\n"
			"fmin.pi f16, f16, f16\n"
			"fsrl.pi f16, f18, f18\n"
			"LBL_SEQID_1_FP_L_GATHER_ID_50_HID_0:\n"
			"li x12, 0x8242aa0010\n" 
			"la x18, mem_ptr_offset\n"
			"flq2 f0, 220(x18)\n"
			"fscwl.ps f15, f0 (x12)\n"
			"fgbl.ps f11, f0 (x12)\n" 
			"fmadd.s f6, f31, f21, f21, rmm\n"
			"fmadd.ps f16, f0, f11, f4, rtz\n"
			"fnmadd.s f26, f15, f27, f19, rne\n"
			"blt x25, x14, 1f\n"
			"1:\n"
			"fsub.pi f18, f21, f3\n"
			"LBL_SEQID_1_INT_AMO_ID_51_HID_0:\n"
			"li x22, 0x825b300028\n" 
			"amominul.w x17, x17, (x22)\n" 
			"faddi.pi f11, f15, 0x14c\n"
			"fsra.pi f21, f18, f21\n"
			"bne x31, x18, 1f\n"
			"1:\n"
			"fround.ps f19, f15, rdn\n"
			"fnmadd.s f19, f15, f19, f21, rdn\n"
			"bge x20, x24, 1f\n"
			"1:\n"
			"LBL_SEQID_1_INT_AMO_ID_52_HID_0:\n"
			"li x18, 0x822333a588\n" 
			"amoswapg.w x9, x17, (x18)\n" 
			"fsgnj.ps f31, f11, f31\n"
			"fsub.pi f0, f27, f26\n"
			"fnot.pi f18, f21\n"
			"fsgnjn.ps f21, f6, f19\n"
			"LBL_SEQID_1_INT_AMO_ID_53_HID_0:\n"
			"li x18, 0x822b300028\n" 
			"amoming.d x16, x25, (x18)\n" 
			"fand.pi f15, f16, f9\n"
			"fmaxu.pi f0, f16, f26\n"
			"fle.ps f10, f3, f15\n"
			"fnmadd.ps f6, f19, f31, f0, rmm\n"
			"LBL_SEQID_1_FP_LOAD_ID_54_HID_0:\n"
			"li x12, 0x8105a0003e\n" 
			"fsw.ps f18, 1812(x12)\n"
			"fbc.ps f18, 1812(x12)\n" 
			"fsub.s f9, f3, f18, rtz\n"
			"fmulhu.pi f9, f18, f0\n"
			"fsrl.pi f9, f3, f16\n"
			"fsgnj.s f16, f3, f0\n"
			"LBL_SEQID_1_FP_AMO_ID_55_HID_0:\n"
			"li x22, 0x8254107024\n" 
			"la x6, mem_ptr_offset\n"
			"flw.ps f16, 64(x6)\n"
			"famoaddl.pi f4, f16 (x22)\n" 
			"fsetm.pi m7, f3\n"
			"sll x22, x17, x22\n"
			"divuw x25, x12, x25\n"
			"fcvt.s.wu f4, x15, rdn\n"
			"LBL_SEQID_1_INT_AMO_ID_56_HID_0:\n"
			"li x9, 0x8125de0018\n" 
			"amomaxug.d x22, x18, (x9)\n" 
			"mul x25, x9, x16\n"
			"bltu x21, x16, 1f\n"
			"1:\n"
			"fle.ps f15, f9, f4\n"
			"fsgnjn.s f9, f10, f9\n"
			"mova.x.m x0\n"
			"LBL_SEQID_1_INT_AMO_ID_57_HID_0:\n"
			"li x12, 0x8245bd2810\n" 
			"amomaxul.w x16, x18, (x12)\n" 
			"fmsub.s f27, f6, f9, f4, rne\n"
			"sraiw x16, x22, 3\n"
			"sll x15, x0, x15\n"
			"fcvt.f16.ps f16, f6\n"
			"LBL_SEQID_1_INT_AMO_ID_58_HID_0:\n"
			"li x12, 0x8237ae0578\n" 
			"amominug.d x15, x22, (x12)\n" 
			"fmadd.ps f21, f21, f27, f16, rdn\n"
			"fnmadd.s f6, f18, f18, f4, rmm\n"
			"fsrli.pi f0, f11, 0xd\n"
			"fadd.pi f9, f15, f27\n"
			"LBL_SEQID_1_FP_GATHER_ID_59_HID_0:\n"
			"li x6, 0x8205bd2812\n" 
			"li x15, 0x70971dd320f3c647\n"
			"fsc32w.ps f2, x15 (x6)\n"
			"fg32w.ps f19, x15 (x6)\n" 
			"feq.pi f9, f18, f26\n"
			"mulhsu x12, x12, x16\n"
			"bge x23, x28, 1f\n"
			"1:\n"
			"fle.s x0, f0, f21\n"
			"fsrai.pi f15, f21, 0x5\n"
			"LBL_SEQID_1_FP_SCATTER_ID_60_HID_0:\n"
			"li x9, 0x810d4db698\n" 
			"la x17, mem_ptr_offset\n"
			"flq2 f27, 128(x17)\n"
			"fsch.ps f6, f27 (x9)\n" 
			"fminu.pi f9, f10, f9\n"
			"frsq.ps f3, f9\n"
			"fsgnjx.ps f19, f19, f15\n"
			"fnmsub.s f26, f26, f21, f10, rup\n"
			"bne x26, x14, 1f\n"
			"1:\n"
			"la x22, rand_ieee754_32\n"
			"flw.ps f21, 736(x22)\n"
			"fsqrt.s f11, f21, rmm\n" 
			"fsll.pi f6, f31, f4\n"
			"fnmadd.ps f4, f18, f3, f15, rtz\n"
			"bge x7, x21, 1f\n"
			"1:\n"
			"faddi.pi f10, f10, 0x16d\n"
			"fnot.pi f21, f19\n"
			"bltu x5, x1, 1f\n"
			"1:\n"
			"mov.m.x m0, x0, 0xFF\n"
			"li x5, 0x40\n"
			"la x31, rand_ieee754_32\n"
			"add x31, x31, x5\n"
			"flw.ps f10, 0(x31)\n"
			"flw.ps f11, 2(x31)\n"
			"flw.ps f15, 3(x31)\n"
			"flw.ps f16, 9(x31)\n"
			"flw.ps f18, 14(x31)\n"
			"flw.ps f19, 15(x31)\n"
			"flw.ps f21, 17(x31)\n"
			"flw.ps f26, 24(x31)\n"
			"flw.ps f27, 26(x31)\n"
			"flw.ps f4, 15(x31)\n"
			"flw.ps f9, 19(x31)\n"
			"la x31, rand_int_32\n"
			"add x31, x31, x5\n"
			"lw x0, 0(x31)\n"
			"lw x12, 4(x31)\n"
			"lw x15, 1(x31)\n"
			"lw x16, 12(x31)\n"
			"lw x17, 13(x31)\n"
			"lw x18, 1(x31)\n"
			"lw x22, 5(x31)\n"
			"lw x25, 7(x31)\n"
			"lw x5, 28(x31)\n"
			"lw x6, 29(x31)\n"
			"lw x9, 18(x31)\n"
			"li x21, 0x3\n"
			"LBL_SEQID_1_LOOP_SEQ_HID_0:\n"
			"maskor m2, m2, m3\n"
			"fle.s x9, f19, f4\n"
			"fmax.pi f27, f11, f11\n"
			"flog.ps f27, f16\n"
			"fbcx.ps f31, x16\n"
			"fmvs.x.ps x18, f10, 5\n"
			"fcvt.f16.ps f18, f27\n"
			"bne x17, x7, 1f\n"
			"1:\n"
			"fmulh.pi f18, f18, f18\n"
			"divw x16, x15, x17\n"
			"fcmov.ps f3, f21, f21, f19\n"
			"fltm.ps m2, f4, f18\n"
			"feq.s x12, f16, f11\n"
			"fltu.pi f26, f10, f21\n"
			"flt.pi f19, f27, f19\n"
			"fnot.pi f10, f9\n"
			"fadd.ps f4, f26, f4, rtz\n"
			"fcmovm.ps f31, f10, f27\n"
			"rem x0, x12, x9\n"
			"ffrc.ps f3, f26\n"
			"fsgnj.ps f21, f21, f19\n"
			"add x22, x22, x16\n"
			"fmul.ps f19, f21, f21, rup\n"
			"bgeu x25, x16, 1f\n"
			"1:\n"
			"fcvt.wu.s x17, f15, rtz\n"
			"srl x6, x6, x25\n"
			"and x0, x9, x9\n"
			"fsrl.pi f10, f19, f18\n"
			"blt x12, x27, 1f\n"
			"1:\n"
			"fsgnj.s f4, f26, f4\n"
			"feq.ps f21, f19, f4\n"
			"fmsub.s f11, f11, f11, f21, rmm\n"
			"fence\n"
			"frsq.ps f19, f10\n"
			"faddi.pi f18, f11, 0x1ce\n"
			"fltm.ps m2, f4, f19\n"
			"sraw x0, x5, x15\n"
			"remuw x5, x18, x5\n"
			"fle.ps f11, f26, f10\n"
			"bge x23, x22, 1f\n"
			"1:\n"
			"packb x16, x16, x5\n"
			"mulw x6, x25, x25\n"
			"fcvt.pw.ps f27, f27, rdn\n"
			"bltu x20, x21, 1f\n"
			"1:\n"
			"faddi.pi f15, f9, 0x99\n"
			"flem.ps m3, f11, f18\n"
			"slli x16, x18, 4\n"
			"fnmadd.ps f11, f9, f15, f4, rne\n"
			"srlw x25, x15, x25\n"
			"fcvt.ps.pw f31, f4, rup\n"
			"frsq.ps f10, f27\n"
			"fsub.pi f26, f11, f16\n"
			"div x18, x12, x25\n"
			"beq x5, x26, 1f\n"
			"1:\n"
			"maskor m5, m5, m2\n"
			"fmin.s f21, f21, f10\n"
			"fcvt.ps.pw f6, f19, rmm\n"
			"for.pi f19, f9, f21\n"
			"fmax.s f31, f21, f21\n"
			"fnmsub.ps f15, f15, f9, f10, rne\n"
			"fcvt.s.w f10, x5, rtz\n"
			"maskxor m3, m7, m5\n"
			"fsat8.pi f4, f4\n"
			"fsetm.pi m5, f9\n"
			"fence\n"
			"xor x0, x22, x12\n"
			"fandi.pi f4, f4, 0x9\n"
			"fsrai.pi f10, f9, 0x7\n"
			"divw x5, x15, x16\n"
			"bge x15, x13, 1f\n"
			"1:\n"
			"fcvt.w.s x6, f18, rdn\n"
			"fcvt.s.wu f16, x18, rup\n"
			"rem x16, x0, x25\n"
			"fadd.ps f19, f18, f10, rmm\n"
			"ffrc.ps f26, f16\n"
			"feq.pi f3, f9, f26\n"
			"lui x5, 0x14\n"
			"fsetm.pi m5, f21\n"
			"fsgnjx.s f9, f9, f16\n"
			"fmaxu.pi f27, f16, f15\n"
			"remw x15, x12, x9\n"
			"fsll.pi f15, f10, f11\n"
			"sub x22, x15, x22\n"
			"fmul.pi f6, f4, f15\n"
			"fsgnjn.ps f21, f27, f10\n"
			"fandi.pi f16, f26, 0x159\n"
			"fandi.pi f11, f19, 0x1ed\n"
			"fsrai.pi f4, f10, 0xa\n"
			"fnot.pi f10, f18\n"
			"rem x15, x17, x5\n"
			"fadd.pi f15, f15, f4\n"
			"beq x21, x13, 1f\n"
			"1:\n"
			"fsgnjn.ps f4, f15, f27\n"
			"sltiu x9, x16, 0x4\n"
			"fmax.s f26, f15, f9\n"
			"packb x0, x0, x6\n"
			"bgeu x1, x17, 1f\n"
			"1:\n"
			"flog.ps f18, f18\n"
			"fadd.s f19, f9, f21, rne\n"
			"fnmadd.ps f11, f18, f21, f4, rne\n"
			"fadd.pi f3, f16, f16\n"
			"mulhsu x17, x0, x5\n"
			"fle.pi f10, f11, f27\n"
			"fmax.ps f19, f9, f19\n"
			"sll x5, x5, x18\n"
			"fle.s x0, f26, f16\n"
			"mulhsu x17, x25, x6\n"
			"bltu x19, x18, 1f\n"
			"1:\n"
			"mov.m.x m5, x22, 0x43\n"
			"fmulhu.pi f26, f26, f4\n"
			"remw x16, x18, x25\n"
			"ori x9, x9, 0x1b\n"
			"bne x14, x19, 1f\n"
			"1:\n"
			"sraw x6, x5, x6\n"
			"maskand m3, m2, m3\n"
			"xori x6, x17, 0xb\n"
			"sltu x6, x18, x16\n"
			"subw x5, x25, x5\n"
			"fcvt.ps.pwu f16, f21, rmm\n"
			"flt.s x17, f19, f11\n"
			"fmax.pi f21, f16, f27\n"
			"fltu.pi f31, f26, f26\n"
			"fbci.pi f10, 0x59f1a\n"
			"auipc x17, 0x18\n"
			"bne x13, x7, 1f\n"
			"1:\n"
			"fsat8.pi f21, f27\n"
			"fmsub.s f26, f26, f16, f11, rtz\n"
			"fltm.ps m2, f26, f26\n"
			"bge x1, x27, 1f\n"
			"1:\n"
			"sub x18, x17, x22\n"
			"bgeu x13, x3, 1f\n"
			"1:\n"
			"slliw x15, x15, 5\n"
			"fence\n"
			"bgeu x13, x17, 1f\n"
			"1:\n"
			"fxor.pi f19, f19, f18\n"
			"fle.s x17, f4, f11\n"
			"beq x9, x6, 1f\n"
			"1:\n"
			"fsra.pi f9, f9, f11\n"
			"xori x9, x9, 0x13\n"
			"lui x6, 0x0\n"
			"blt x26, x29, 1f\n"
			"1:\n"
			"fmaxu.pi f18, f21, f15\n"
			"fsatu8.pi f6, f27\n"
			"bltu x0, x27, 1f\n"
			"1:\n"
			"slti x12, x0, 0x12\n"
			"bltu x8, x29, 1f\n"
			"1:\n"
			"sub x0, x18, x12\n"
			"fsgnjx.ps f10, f27, f4\n"
			"feqm.ps m7, f16, f27\n"
			"fmsub.s f15, f26, f27, f9, rup\n"
			"fmin.s f15, f4, f26\n"
			"maskxor m3, m2, m7\n"
			"flog.ps f21, f19\n"
			"beq x28, x24, 1f\n"
			"1:\n"
			"fcmovm.ps f31, f11, f21\n"
			"slli x22, x12, 0\n"
			"mulhsu x18, x17, x17\n"
			"maskpopc x12, m7\n"
			"beq x28, x23, 1f\n"
			"1:\n"
			"mulh x15, x25, x0\n"
			"fcvt.pwu.ps f15, f18, rne\n"
			"fcvt.pw.ps f18, f10, rne\n"
			"beq x31, x25, 1f\n"
			"1:\n"
			"mova.m.x x12\n"
			"remw x5, x5, x5\n"
			"fnmadd.ps f19, f11, f21, f4, rne\n"
			"fmsub.s f21, f27, f19, f18, rdn\n"
			"mova.x.m x16\n"
			"ecall\n"
			"blt x8, x5, 1f\n"
			"1:\n"
			"divu x6, x22, x16\n"
			"bltu x16, x27, 1f\n"
			"1:\n"
			"flog.ps f26, f19\n"
			"fsll.pi f16, f26, f16\n"
			"remu x25, x18, x25\n"
			"sltiu x15, x18, 0x3\n"
			"fmin.pi f11, f16, f4\n"
			"rem x16, x16, x6\n"
			"mulh x5, x6, x5\n"
			"fcvt.w.s x22, f21, rdn\n"
			"xor x25, x0, x15\n"
			"beq x27, x25, 1f\n"
			"1:\n"
			"mul x0, x22, x5\n"
			"fmaxu.pi f11, f11, f19\n"
			"beq x10, x30, 1f\n"
			"1:\n"
			"fand.pi f10, f19, f21\n"
			"fsgnj.s f18, f18, f19\n"
			"mulhsu x16, x15, x16\n"
			"feq.s x17, f26, f16\n"
			"fmul.ps f3, f26, f4, rup\n"
			"srl x0, x18, x0\n"
			"sra x9, x9, x22\n"
			"fltu.pi f18, f27, f11\n"
			"mulh x6, x15, x16\n"
			"fle.pi f10, f27, f26\n"
			"fcvt.pw.ps f26, f27, rmm\n"
			"addw x12, x6, x16\n"
			"fltm.pi m2, f21, f4\n"
			"fcvt.pw.ps f26, f26, rtz\n"
			"fmin.pi f27, f27, f26\n"
			"fsgnjn.s f27, f26, f16\n"
			"bne x11, x2, 1f\n"
			"1:\n"
			"srai x17, x15, 4\n"
			"bne x16, x23, 1f\n"
			"1:\n"
			"slli x12, x0, 2\n"
			"maskand m2, m5, m2\n"
			"sltu x17, x17, x22\n"
			"slli x9, x9, 5\n"
			"feq.pi f15, f9, f9\n"
			"slt x9, x12, x18\n"
			"addiw x5, x15, 0x10\n"
			"divuw x22, x0, x22\n"
			"bne x8, x30, 1f\n"
			"1:\n"
			"auipc x15, 0xe\n"
			"fsrl.pi f27, f27, f11\n"
			"blt x18, x12, 1f\n"
			"1:\n"
			"fsub.s f16, f9, f10, rdn\n"
			"beq x0, x0, 1f\n"
			"1:\n"
			"fmulhu.pi f18, f18, f26\n"
			"fbcx.ps f16, x12\n"
			"blt x12, x24, 1f\n"
			"1:\n"
			"fxor.pi f31, f19, f4\n"
			"addi x21, x21, -1\n"
			"bne x21, x0, LBL_SEQID_1_LOOP_SEQ_HID_0\n"
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_0\n"
			"addi x30, x30, 1\n"
			"li x16, 0xcf907224873b7bad\n"
			"fcvt.s.w f27, x16, rne\n" 
			"srlw x15, x15, x18\n"
			"frsq.ps f9, f27\n"
			"fclass.s x25, f26\n"
			"fcmov.ps f21, f31, f27, f15\n"
			"slli x5, x5, 4\n"
			"bltu x30, x9, 1f\n"
			"1:\n"
			"fswizz.ps f4, f10, 0x1b\n"
			"beq x7, x0, 1f\n"
			"1:\n"
			"fnmadd.s f6, f26, f18, f6, rup\n"
			"fmulhu.pi f18, f21, f27\n"
			"fmin.pi f31, f4, f16\n"
			"la x25, rand_int_32\n"
			"flw.ps f4, 992(x25)\n"
			"flw.ps f16, 736(x25)\n"
			"fmulhu.pi f18, f4, f16\n" 
			"fxor.pi f27, f27, f10\n"
			"fmulhu.pi f19, f19, f27\n"
			"fsrai.pi f18, f13, 0x2\n"
			"fcvt.ps.pw f11, f11, rtz\n"
			"blt x11, x25, 1f\n"
			"1:\n"
			"fle.ps f19, f9, f4\n"
			"flog.ps f18, f11\n"
			"frcp.ps f27, f4\n"
			"fsgnj.ps f26, f4, f26\n"
			"bge x14, x9, 1f\n"
			"1:\n"
			"fcvt.pwu.ps f4, f9, rne\n"
			"la x15, rand_ieee754_32\n"
			"flw.ps f21, 832(x15)\n"
			"fmvz.x.ps x6, f21, 1\n" 
			"fltu.pi f21, f9, f21\n"
			"fslli.pi f18, f11, 0xb\n"
			"for.pi f0, f13, f31\n"
			"fmax.s f11, f4, f26\n"
			"fsub.ps f21, f13, f0, rup\n"
			"fsat8.pi f4, f11\n"
			"fsrl.pi f18, f0, f0\n"
			"blt x30, x13, 1f\n"
			"1:\n"
			"fnmadd.ps f4, f4, f9, f19, rdn\n"
			"for.pi f3, f19, f21\n"
			"bne x4, x20, 1f\n"
			"1:\n"
			"li x16, 0xf1ec50d985ada425\n"
			"li x5, 0x40c7c49385bd0052\n"
			"remuw x16, x16, x5\n" 
			"divu x12, x16, x17\n"
			"fmaxu.pi f4, f0, f31\n"
			"add x17, x6, x17\n"
			"fsgnj.s f4, f27, f26\n"
			"fsgnjn.ps f11, f11, f9\n"
			"divw x22, x12, x12\n"
			"slti x18, x6, 0x14\n"
			"beq x12, x23, 1f\n"
			"1:\n"
			"fadd.ps f13, f31, f3, rup\n"
			"sltiu x5, x18, 0x1a\n"
			"li x5, 0x7dd6ece98ec42492\n"
			"fbcx.ps f0, x5\n" 
			"fsgnjn.ps f6, f3, f6\n"
			"fsrl.pi f21, f13, f13\n"
			"fcmov.ps f9, f3, f13, f15\n"
			"fmax.s f13, f15, f6\n"
			"fnmsub.s f4, f0, f0, f0, rne\n"
			"fsgnjx.s f21, f0, f0\n"
			"fsll.pi f19, f6, f3\n"
			"fsgnjx.s f0, f10, f0\n"
			"fcmovm.ps f21, f19, f13\n"
			"li x12, 0xc1cc2b8be36cb498\n"
			"srai x25, x12, 1\n" 
			"fcvt.pw.ps f11, f18, rdn\n"
			"fbcx.ps f31, x22\n"
			"remuw x9, x6, x6\n"
			"fcvt.s.w f11, x22, rup\n"
			"frcp.ps f21, f4\n"
			"bne x26, x17, 1f\n"
			"1:\n"
			"srli x6, x5, 5\n"
			"fmul.s f26, f21, f21, rup\n"
			"addiw x9, x15, 0xa\n"
			"beq x16, x23, 1f\n"
			"1:\n"
			"fcmovm.ps f4, f16, f18\n"
			"la x5, rand_ieee754_32\n"
			"flw.ps f0, 384(x5)\n"
			"fcvt.f16.ps f6, f0\n" 
			"fltm.pi m5, f6, f31\n"
			"fsgnjn.ps f27, f16, f9\n"
			"add x25, x25, x6\n"
			"srai x17, x12, 5\n"
			"bne x10, x15, 1f\n"
			"1:\n"
			"fle.pi f21, f26, f10\n"
			"bge x17, x12, 1f\n"
			"1:\n"
			"slliw x5, x15, 1\n"
			"fcvt.ps.pw f18, f15, rtz\n"
			"fcvt.pw.ps f13, f10, rtz\n"
			"bne x17, x1, 1f\n"
			"1:\n"
			"remuw x18, x12, x6\n"
			"bltu x2, x11, 1f\n"
			"1:\n"
			"la x17, rand_ieee754_32\n"
			"flw.ps f4, 512(x17)\n"
			"flw.ps f9, 480(x17)\n"
			"fadd.s f3, f4, f9, rne\n" 
			"fnmsub.s f3, f3, f15, f3, rup\n"
			"fcmovm.ps f9, f15, f3\n"
			"fsra.pi f3, f31, f26\n"
			"fexp.ps f9, f15\n"
			"fcvt.ps.f16 f15, f3\n"
			"fminu.pi f15, f31, f26\n"
			"fle.pi f11, f31, f31\n"
			"bge x14, x17, 1f\n"
			"1:\n"
			"fadd.ps f16, f15, f26, rdn\n"
			"fmul.ps f9, f3, f15, rdn\n"
			"la x9, rand_ieee754_32\n"
			"flw.ps f18, 544(x9)\n"
			"flw.ps f10, 288(x9)\n"
			"fmin.ps f4, f18, f10\n" 
			"or x6, x22, x6\n"
			"slliw x6, x25, 3\n"
			"div x6, x5, x16\n"
			"fnmadd.ps f15, f15, f10, f27, rdn\n"
			"for.pi f16, f13, f9\n"
			"fadd.s f13, f13, f3, rmm\n"
			"sra x16, x16, x25\n"
			"and x16, x17, x5\n"
			"fcvt.pw.ps f21, f0, rdn\n"
			"la x16, rand_ieee754_32\n"
			"flw.ps f3, 224(x16)\n"
			"flw.ps f18, 288(x16)\n"
			"flt.s x12, f3, f18\n" 
			"fround.ps f11, f11, rup\n"
			"bge x31, x26, 1f\n"
			"1:\n"
			"fmsub.ps f3, f3, f31, f3, rdn\n"
			"fmaxu.pi f18, f15, f11\n"
			"fnmadd.ps f19, f6, f0, f19, rdn\n"
			"bgeu x24, x14, 1f\n"
			"1:\n"
			"fcvt.ps.pwu f3, f3, rup\n"
			"sllw x0, x0, x16\n"
			"fsgnjx.s f21, f10, f0\n"
			"fsatu8.pi f31, f27\n"
			"fsin.ps f26, f18\n"
			"la x5, rand_ieee754_32\n"
			"flw.ps f21, 256(x5)\n"
			"fclass.ps f6, f21\n" 
			"fcvt.pwu.ps f9, f27, rne\n"
			"fexp.ps f10, f26\n"
			"sllw x17, x22, x15\n"
			"divu x5, x18, x25\n"
			"fcvt.pw.ps f13, f15, rdn\n"
			"addi x15, x25, 0x6\n"
			"fandi.pi f9, f6, 0x1e2\n"
			"fmulh.pi f31, f31, f21\n"
			"bltu x28, x13, 1f\n"
			"1:\n"
			"sraw x25, x6, x15\n"
			"la x6, rand_ieee754_32\n"
			"flw.ps f27, 896(x6)\n"
			"flw.ps f4, 96(x6)\n"
			"fmax.ps f26, f27, f4\n" 
			"divu x22, x22, x6\n"
			"blt x8, x21, 1f\n"
			"1:\n"
			"xor x18, x22, x6\n"
			"mulh x22, x22, x22\n"
			"fcvt.wu.s x25, f13, rup\n"
			"fcvt.s.w f13, x12, rne\n"
			"blt x7, x25, 1f\n"
			"1:\n"
			"fmax.s f11, f4, f21\n"
			"fsetm.pi m5, f11\n"
			"fsgnj.ps f0, f11, f13\n"
			"fsgnjn.ps f6, f4, f21\n"
			"la x18, rand_ieee754_32\n"
			"flw.ps f6, 992(x18)\n"
			"flw.ps f16, 736(x18)\n"
			"fsub.ps f11, f6, f16, rdn\n" 
			"fclass.s x17, f10\n"
			"bgeu x2, x22, 1f\n"
			"1:\n"
			"ecall\n"
			"bgeu x11, x10, 1f\n"
			"1:\n"
			"fle.ps f6, f11, f4\n"
			"fxor.pi f6, f11, f11\n"
			"fpackrepb.pi f16, f16\n"
			"fsra.pi f13, f10, f18\n"
			"fsll.pi f27, f27, f26\n"
			"slti x6, x6, 0x5\n"
			"slli x16, x18, 5\n"
			"la x9, rand_ieee754_32\n"
			"flw.ps f10, 832(x9)\n"
			"flw.ps f3, 672(x9)\n"
			"fcmovm.ps f16, f10, f3\n" 
			"fcvt.ps.f16 f26, f6\n"
			"sub x15, x22, x15\n"
			"fcvt.s.wu f6, x12, rup\n"
			"blt x0, x8, 1f\n"
			"1:\n"
			"fnmadd.ps f18, f26, f10, f6, rdn\n"
			"mov.m.x m3, x22, 0x37\n"
			"lui x5, 0xc\n"
			"divw x17, x12, x17\n"
			"packb x6, x12, x6\n"
			"divu x5, x12, x16\n"
			"li x25, 0x13801e722b1b2a4c\n"
			"mulhu x15, x25, x25\n" 
			"fcvt.ps.pw f4, f31, rup\n"
			"fsin.ps f21, f10\n"
			"fcvt.ps.pwu f15, f27, rtz\n"
			"feq.ps f21, f21, f18\n"
			"fexp.ps f11, f9\n"
			"fmvs.x.ps x12, f6, 5\n"
			"slti x18, x6, 0x10\n"
			"subw x15, x9, x16\n"
			"fsetm.pi m3, f31\n"
			"la x25, rand_int_32\n"
			"flw.ps f4, 928(x25)\n"
			"fcvt.ps.pw f10, f4, rtz\n" 
			"fle.pi f6, f11, f6\n"
			"flog.ps f26, f15\n"
			"fadd.ps f0, f0, f0, rup\n"
			"feqm.ps m7, f15, f0\n"
			"ffrc.ps f19, f26\n"
			"sllw x25, x25, x6\n"
			"ecall\n"
			"slliw x25, x5, 1\n"
			"mova.m.x x17\n"
			"la x5, rand_int_32\n"
			"flw.ps f10, 512(x5)\n"
			"flw.ps f27, 448(x5)\n"
			"fmulh.pi f18, f10, f27\n" 
			"fpackreph.pi f15, f15\n"
			"sltiu x0, x5, 0x1\n"
			"fmin.ps f16, f16, f15\n"
			"bgeu x3, x22, 1f\n"
			"1:\n"
			"mulw x12, x12, x0\n"
			"fmin.s f13, f16, f13\n"
			"sra x15, x16, x15\n"
			"fcvt.pw.ps f3, f4, rmm\n"
			"fcmov.ps f9, f4, f10, f4\n"
			"slti x6, x6, 0x1b\n"
			"la x6, rand_ieee754_32\n"
			"flw.ps f16, 224(x6)\n"
			"flw.ps f13, 768(x6)\n"
			"fmul.s f13, f16, f13, rtz\n" 
			"fsub.ps f13, f13, f13, rne\n"
			"fmadd.s f26, f16, f0, f0, rdn\n"
			"subw x22, x9, x22\n"
			"fslli.pi f11, f16, 0xe\n"
			"srli x22, x22, 2\n"
			"bltu x13, x19, 1f\n"
			"1:\n"
			"fbcx.ps f11, x5\n"
			"fadd.s f6, f27, f13, rne\n"
			"addw x15, x5, x15\n"
			"fmulhu.pi f13, f4, f15\n"
			"ecall\n" 
			"fle.ps f0, f0, f26\n"
			"srlw x17, x9, x15\n"
			"remw x9, x0, x9\n"
			"flt.ps f15, f26, f15\n"
			"srai x17, x15, 4\n"
			"fsub.pi f18, f18, f13\n"
			"bltu x16, x21, 1f\n"
			"1:\n"
			"fcvt.f16.ps f15, f15\n"
			"srai x12, x22, 3\n"
			"bltu x16, x30, 1f\n"
			"1:\n"
			"maskxor m7, m7, m7\n"
			"la x16, rand_ieee754_32\n"
			"flw.ps f15, 960(x16)\n"
			"flw.ps f19, 64(x16)\n"
			"fsgnjx.ps f19, f15, f19\n" 
			"mulhsu x9, x22, x15\n"
			"fmvz.x.ps x6, f27, 4\n"
			"ebreak\n"
			"beq x18, x23, 1f\n"
			"1:\n"
			"fxor.pi f6, f6, f15\n"
			"fmin.s f27, f11, f27\n"
			"andi x6, x5, 0x15\n"
			"beq x12, x3, 1f\n"
			"1:\n"
			"feq.ps f10, f18, f27\n"
			"fexp.ps f27, f27\n"
			"fcvt.ps.pwu f16, f16, rne\n"
			"la x22, rand_ieee754_32\n"
			"flw.ps f4, 864(x22)\n"
			"flw.ps f16, 992(x22)\n"
			"flw.ps f0, 96(x22)\n"
			"fmsub.s f31, f4, f16, f0, rdn\n" 
			"srai x5, x5, 2\n"
			"fsin.ps f3, f0\n"
			"srlw x18, x17, x15\n"
			"beq x2, x3, 1f\n"
			"1:\n"
			"slt x12, x6, x25\n"
			"fmsub.s f4, f10, f4, f13, rne\n"
			"fsgnjn.s f26, f26, f0\n"
			"sraiw x6, x16, 4\n"
			"srl x5, x17, x22\n"
			"bge x16, x25, 1f\n"
			"1:\n"
			"remu x6, x16, x25\n"
			"la x12, rand_ieee754_32\n"
			"flw.ps f27, 160(x12)\n"
			"fcvt.pw.ps f6, f27, rdn\n" 
			"addw x22, x6, x9\n"
			"fcvt.s.w f16, x22, rdn\n"
			"divuw x18, x18, x12\n"
			"addiw x15, x9, 0x6\n"
			"fsgnjn.ps f6, f16, f6\n"
			"feqm.ps m5, f19, f16\n"
			"bne x12, x24, 1f\n"
			"1:\n"
			"fminu.pi f27, f15, f19\n"
			"feqm.ps m7, f31, f13\n"
			"mulhsu x25, x16, x16\n"
			"bge x14, x28, 1f\n"
			"1:\n"
			"la x6, rand_ieee754_32\n"
			"flw.ps f13, 704(x6)\n"
			"flw.ps f21, 704(x6)\n"
			"fsgnjn.ps f31, f13, f21\n" 
			"fsatu8.pi f31, f13\n"
			"bltu x11, x21, 1f\n"
			"1:\n"
			"fround.ps f11, f15, rne\n"
			"fcvt.ps.pw f16, f27, rdn\n"
			"fexp.ps f27, f27\n"
			"bgeu x19, x18, 1f\n"
			"1:\n"
			"flog.ps f10, f10\n"
			"divu x22, x22, x17\n"
			"fminu.pi f26, f0, f6\n"
			"fmadd.s f3, f19, f9, f21, rne\n"
			"rem x5, x15, x25\n"
			"li x16, 0x86f01c04dc3d4e30\n"
			"li x15, 0x441acd37e3b8277a\n"
			"srlw x12, x16, x15\n" 
			"addi x12, x22, 0x1f\n"
			"beq x14, x8, 1f\n"
			"1:\n"
			"divu x18, x6, x18\n"
			"beq x21, x26, 1f\n"
			"1:\n"
			"fnmsub.s f31, f4, f13, f6, rup\n"
			"auipc x6, 0x1d\n"
			"remu x15, x15, x22\n"
			"fence\n"
			"fslli.pi f31, f4, 0x0\n"
			"fexp.ps f19, f6\n"
			"bne x12, x19, 1f\n"
			"1:\n"
			"slli x0, x6, 2\n"
			"la x25, rand_ieee754_32\n"
			"flw.ps f10, 896(x25)\n"
			"flw.ps f3, 544(x25)\n"
			"fmin.s f9, f10, f3\n" 
			"divu x6, x25, x12\n"
			"fmv.s.x f9, x17\n"
			"fsra.pi f10, f0, f15\n"
			"feq.ps f9, f16, f9\n"
			"feq.pi f27, f21, f27\n"
			"slt x12, x18, x12\n"
			"addw x22, x22, x22\n"
			"bge x17, x10, 1f\n"
			"1:\n"
			"srl x5, x9, x5\n"
			"auipc x9, 0x1a\n"
			"li x22, 0x3d49337497150a09\n"
			"li x9, 0x319c32f06a65217c\n"
			"rem x16, x22, x9\n" 
			"fbci.pi f13, 0x6f7a9\n"
			"fsgnj.ps f4, f31, f16\n"
			"srai x18, x6, 0\n"
			"fsin.ps f4, f21\n"
			"flt.ps f27, f27, f31\n"
			"maskpopc x18, m3\n"
			"remw x16, x22, x0\n"
			"srl x9, x17, x17\n"
			"bne x30, x10, 1f\n"
			"1:\n"
			"fcvt.wu.s x9, f31, rtz\n"
			"la x9, rand_ieee754_32\n"
			"flw.ps f26, 832(x9)\n"
			"fmvz.x.ps x25, f26, 4\n" 
			"fsgnj.s f31, f21, f4\n"
			"bltu x12, x17, 1f\n"
			"1:\n"
			"fsgnjn.s f0, f18, f0\n"
			"flog.ps f27, f3\n"
			"fmax.ps f19, f13, f15\n"
			"fmin.ps f4, f26, f10\n"
			"bgeu x16, x31, 1f\n"
			"1:\n"
			"fle.pi f13, f9, f13\n"
			"fslli.pi f26, f9, 0xe\n"
			"beq x13, x21, 1f\n"
			"1:\n"
			"fcmov.ps f10, f31, f10, f6\n"
			"beq x3, x23, 1f\n"
			"1:\n"
			"fsll.pi f21, f0, f13\n"
			"la x15, rand_ieee754_32\n"
			"flw.ps f4, 768(x15)\n"
			"flw.ps f11, 288(x15)\n"
			"fsgnjn.s f19, f4, f11\n" 
			"ffrc.ps f11, f31\n"
			"fmadd.s f16, f3, f4, f4, rmm\n"
			"fcvt.pw.ps f6, f0, rup\n"
			"subw x17, x22, x17\n"
			"fcvt.ps.pw f9, f18, rmm\n"
			"subw x6, x6, x22\n"
			"fminu.pi f10, f4, f16\n"
			"fsgnjx.s f18, f3, f18\n"
			"fxor.pi f0, f0, f26\n"
			"la x18, rand_ieee754_32\n"
			"flw.ps f9, 864(x18)\n"
			"flw.ps f10, 416(x18)\n"
			"flw.ps f11, 960(x18)\n"
			"fnmsub.ps f10, f9, f10, f11, rdn\n" 
			"xor x18, x25, x0\n"
			"fcvt.w.s x0, f16, rne\n"
			"fsrl.pi f3, f3, f21\n"
			"srli x16, x16, 2\n"
			"fltu.pi f27, f18, f10\n"
			"frcp.ps f27, f27\n"
			"fsrai.pi f26, f27, 0xa\n"
			"sllw x22, x6, x9\n"
			"fcvt.ps.f16 f6, f6\n"
			"bne x1, x8, 1f\n"
			"1:\n"
			"la x5, rand_int_32\n"
			"flw.ps f4, 928(x5)\n"
			"fpackrepb.pi f13, f4\n" 
			"fnot.pi f31, f26\n"
			"fnmsub.s f11, f0, f26, f15, rup\n"
			"fcmov.ps f0, f9, f26, f0\n"
			"remuw x0, x9, x0\n"
			"andi x9, x12, 0x5\n"
			"subw x0, x0, x12\n"
			"bge x31, x7, 1f\n"
			"1:\n"
			"ebreak\n"
			"fswizz.ps f18, f26, 0xa\n"
			"fclass.ps f11, f10\n"
			"la x16, rand_ieee754_32\n"
			"flw.ps f16, 32(x16)\n"
			"flw.ps f4, 128(x16)\n"
			"fcmovm.ps f11, f16, f4\n" 
			"mul x17, x0, x17\n"
			"blt x24, x31, 1f\n"
			"1:\n"
			"feq.s x0, f13, f31\n"
			"beq x8, x5, 1f\n"
			"1:\n"
			"ffrc.ps f10, f19\n"
			"ebreak\n"
			"sra x22, x18, x0\n"
			"sltiu x22, x15, 0x1d\n"
			"mul x18, x18, x18\n"
			"bgeu x0, x21, 1f\n"
			"1:\n"
			"fmin.pi f18, f13, f19\n"
			"rem x9, x15, x15\n"
			"li x6, 0x605606dad0ae373\n"
			"fcvt.s.wu f26, x6, rtz\n" 
			"fcvt.pw.ps f0, f3, rup\n"
			"remu x16, x6, x16\n"
			"masknot m2, m3\n"
			"lui x9, 0xd\n"
			"flt.pi f15, f16, f16\n"
			"mova.x.m x25\n"
			"bltu x27, x27, 1f\n"
			"1:\n"
			"fmadd.s f9, f9, f21, f9, rmm\n"
			"fswizz.ps f0, f9, 0x12\n"
			"div x6, x12, x6\n"
			"bltu x3, x3, 1f\n"
			"1:\n"
			"mov.m.x m0, x0, 0xFF\n"
			"li x5, 0x60\n"
			"la x31, rand_ieee754_32\n"
			"add x31, x31, x5\n"
			"flw.ps f0, 0(x31)\n"
			"flw.ps f10, 4(x31)\n"
			"flw.ps f11, 4(x31)\n"
			"flw.ps f13, 10(x31)\n"
			"flw.ps f15, 3(x31)\n"
			"flw.ps f16, 1(x31)\n"
			"flw.ps f18, 18(x31)\n"
			"flw.ps f19, 18(x31)\n"
			"flw.ps f21, 6(x31)\n"
			"flw.ps f26, 36(x31)\n"
			"flw.ps f27, 33(x31)\n"
			"flw.ps f3, 6(x31)\n"
			"flw.ps f31, 26(x31)\n"
			"flw.ps f4, 14(x31)\n"
			"flw.ps f6, 20(x31)\n"
			"flw.ps f9, 54(x31)\n"
			"la x31, rand_int_32\n"
			"add x31, x31, x5\n"
			"lw x0, 0(x31)\n"
			"lw x12, 3(x31)\n"
			"lw x15, 6(x31)\n"
			"lw x16, 2(x31)\n"
			"lw x17, 14(x31)\n"
			"lw x18, 6(x31)\n"
			"lw x22, 21(x31)\n"
			"lw x25, 5(x31)\n"
			"lw x5, 10(x31)\n"
			"lw x6, 17(x31)\n"
			"lw x9, 5(x31)\n"
			"li x21, 0x3\n"
			"LBL_SEQID_2_LOOP_SEQ_HID_0:\n"
			"fmsub.ps f0, f9, f15, f26, rne\n"
			"flt.ps f3, f4, f18\n"
			"blt x5, x30, 1f\n"
			"1:\n"
			"fsub.s f3, f11, f15, rdn\n"
			"fnmadd.s f18, f21, f15, f26, rtz\n"
			"sllw x17, x17, x5\n"
			"andi x9, x22, 0x5\n"
			"beq x20, x10, 1f\n"
			"1:\n"
			"auipc x12, 0x1d\n"
			"bge x7, x27, 1f\n"
			"1:\n"
			"auipc x22, 0xf\n"
			"fmsub.ps f11, f0, f19, f13, rtz\n"
			"bne x13, x22, 1f\n"
			"1:\n"
			"mulhu x9, x16, x12\n"
			"addi x6, x0, 0x0\n"
			"mulh x5, x25, x5\n"
			"flem.ps m2, f21, f21\n"
			"fmin.pi f26, f16, f9\n"
			"fsub.pi f19, f13, f15\n"
			"bne x12, x11, 1f\n"
			"1:\n"
			"flt.pi f15, f18, f11\n"
			"fbci.pi f19, 0x5ed11\n"
			"fsat8.pi f21, f27\n"
			"fmadd.s f0, f19, f21, f27, rmm\n"
			"srl x0, x17, x16\n"
			"maskor m2, m2, m7\n"
			"fsgnjn.s f0, f4, f6\n"
			"sllw x16, x25, x5\n"
			"bltu x6, x2, 1f\n"
			"1:\n"
			"or x16, x15, x22\n"
			"fcvt.wu.s x5, f6, rdn\n"
			"subw x9, x12, x25\n"
			"bne x15, x9, 1f\n"
			"1:\n"
			"fmsub.ps f10, f10, f19, f6, rup\n"
			"fcvt.pw.ps f3, f11, rdn\n"
			"srai x0, x18, 5\n"
			"fcvt.s.w f6, x18, rmm\n"
			"bgeu x25, x21, 1f\n"
			"1:\n"
			"fcmov.ps f18, f11, f21, f9\n"
			"fclass.s x22, f4\n"
			"fexp.ps f31, f10\n"
			"mulhsu x18, x18, x5\n"
			"fmsub.ps f3, f0, f3, f18, rmm\n"
			"bgeu x10, x16, 1f\n"
			"1:\n"
			"div x9, x0, x22\n"
			"fsrai.pi f13, f9, 0x7\n"
			"divw x17, x12, x18\n"
			"mul x12, x9, x18\n"
			"fadd.pi f15, f11, f19\n"
			"fclass.s x6, f4\n"
			"srl x22, x18, x22\n"
			"fsatu8.pi f26, f9\n"
			"fsub.ps f13, f27, f18, rdn\n"
			"slli x18, x15, 2\n"
			"slt x5, x22, x18\n"
			"beq x19, x29, 1f\n"
			"1:\n"
			"fslli.pi f0, f11, 0xc\n"
			"subw x12, x17, x6\n"
			"fsin.ps f16, f16\n"
			"remuw x15, x9, x15\n"
			"bne x20, x25, 1f\n"
			"1:\n"
			"fsub.s f15, f4, f21, rdn\n"
			"fbci.ps f27, 0x7bbd6\n"
			"beq x13, x17, 1f\n"
			"1:\n"
			"fclass.s x22, f15\n"
			"fcvt.wu.s x17, f15, rdn\n"
			"fsgnjx.ps f31, f3, f10\n"
			"fsgnj.ps f13, f31, f6\n"
			"fsgnj.s f27, f21, f11\n"
			"and x25, x9, x15\n"
			"beq x26, x29, 1f\n"
			"1:\n"
			"flog.ps f13, f19\n"
			"fcvt.s.wu f16, x9, rtz\n"
			"blt x16, x17, 1f\n"
			"1:\n"
			"fltu.pi f11, f11, f18\n"
			"flt.pi f3, f3, f3\n"
			"remw x25, x25, x25\n"
			"fmadd.s f18, f18, f26, f18, rne\n"
			"sub x12, x9, x18\n"
			"fmsub.s f0, f3, f16, f0, rup\n"
			"fcvt.w.s x18, f16, rdn\n"
			"fnmadd.s f15, f19, f4, f10, rdn\n"
			"bne x23, x0, 1f\n"
			"1:\n"
			"fmax.pi f26, f26, f11\n"
			"sltu x16, x22, x12\n"
			"bge x3, x0, 1f\n"
			"1:\n"
			"fsub.ps f21, f15, f3, rup\n"
			"srlw x0, x12, x16\n"
			"bgeu x17, x11, 1f\n"
			"1:\n"
			"xori x12, x9, 0x1c\n"
			"fclass.ps f15, f26\n"
			"srliw x9, x16, 2\n"
			"fpackreph.pi f10, f13\n"
			"sllw x12, x22, x12\n"
			"sraiw x15, x6, 0\n"
			"srli x18, x17, 2\n"
			"maskor m7, m7, m2\n"
			"mov.m.x m3, x6, 0x51\n"
			"fmin.ps f16, f19, f16\n"
			"ffrc.ps f31, f9\n"
			"mulh x12, x22, x18\n"
			"beq x7, x6, 1f\n"
			"1:\n"
			"fminu.pi f4, f4, f9\n"
			"andi x5, x9, 0x16\n"
			"and x12, x6, x25\n"
			"bne x23, x22, 1f\n"
			"1:\n"
			"sltu x16, x25, x9\n"
			"fsgnjn.s f31, f15, f31\n"
			"mul x5, x5, x6\n"
			"bgeu x15, x19, 1f\n"
			"1:\n"
			"fsatu8.pi f11, f15\n"
			"subw x0, x12, x0\n"
			"frsq.ps f9, f9\n"
			"bge x28, x28, 1f\n"
			"1:\n"
			"rem x25, x22, x25\n"
			"srai x17, x17, 5\n"
			"fandi.pi f19, f18, 0x1fe\n"
			"mulhu x15, x15, x5\n"
			"mul x0, x12, x22\n"
			"slli x25, x5, 5\n"
			"bltu x17, x4, 1f\n"
			"1:\n"
			"fltm.ps m3, f27, f15\n"
			"fsetm.pi m7, f19\n"
			"and x18, x0, x15\n"
			"fcvt.ps.f16 f16, f13\n"
			"fsrl.pi f0, f6, f31\n"
			"fsub.s f4, f9, f31, rdn\n"
			"fle.s x17, f4, f11\n"
			"fclass.ps f15, f15\n"
			"fmv.s.x f13, x0\n"
			"fnmsub.s f0, f13, f16, f10, rne\n"
			"divu x22, x12, x17\n"
			"fsub.pi f31, f9, f16\n"
			"fle.pi f27, f19, f16\n"
			"sll x18, x15, x0\n"
			"fandi.pi f10, f9, 0xfd\n"
			"for.pi f3, f27, f15\n"
			"fmax.pi f11, f31, f11\n"
			"fmaxu.pi f3, f9, f6\n"
			"mulhu x0, x18, x6\n"
			"ffrc.ps f9, f26\n"
			"fsgnjx.ps f3, f4, f16\n"
			"fmadd.s f4, f10, f10, f4, rtz\n"
			"rem x12, x12, x18\n"
			"bne x9, x16, 1f\n"
			"1:\n"
			"sra x5, x18, x5\n"
			"sllw x6, x5, x17\n"
			"divu x15, x16, x15\n"
			"ecall\n"
			"bltu x22, x23, 1f\n"
			"1:\n"
			"remu x25, x25, x6\n"
			"add x0, x0, x25\n"
			"fsrli.pi f0, f11, 0x2\n"
			"fmax.ps f13, f11, f26\n"
			"flt.pi f4, f4, f21\n"
			"fclass.s x18, f19\n"
			"fadd.pi f21, f11, f21\n"
			"bne x0, x1, 1f\n"
			"1:\n"
			"mulh x16, x25, x16\n"
			"ffrc.ps f4, f4\n"
			"fsrai.pi f16, f13, 0x7\n"
			"flt.s x16, f13, f19\n"
			"fmul.ps f4, f11, f4, rdn\n"
			"mulhsu x16, x16, x6\n"
			"fltm.pi m7, f0, f15\n"
			"srli x16, x17, 2\n"
			"fswizz.ps f11, f16, 0x7\n"
			"mov.m.x m5, x0, 0x42\n"
			"srai x22, x22, 4\n"
			"slli x15, x0, 1\n"
			"flt.s x15, f26, f11\n"
			"feq.ps f15, f11, f26\n"
			"and x16, x16, x18\n"
			"bltu x12, x24, 1f\n"
			"1:\n"
			"fsrl.pi f18, f26, f4\n"
			"and x17, x6, x9\n"
			"fsub.pi f18, f11, f18\n"
			"fmvz.x.ps x9, f9, 6\n"
			"srli x17, x17, 3\n"
			"bgeu x13, x2, 1f\n"
			"1:\n"
			"fsetm.pi m5, f18\n"
			"mul x22, x12, x18\n"
			"xor x0, x0, x17\n"
			"fadd.pi f19, f19, f18\n"
			"frcp.ps f6, f31\n"
			"feq.ps f11, f15, f13\n"
			"fmsub.s f31, f15, f16, f18, rne\n"
			"addw x18, x17, x16\n"
			"blt x29, x0, 1f\n"
			"1:\n"
			"sraiw x17, x25, 3\n"
			"remuw x0, x12, x0\n"
			"fslli.pi f18, f6, 0x6\n"
			"frcp.ps f16, f15\n"
			"fsgnjx.s f21, f4, f21\n"
			"fmax.pi f9, f26, f10\n"
			"srlw x18, x16, x12\n"
			"bltu x12, x3, 1f\n"
			"1:\n"
			"fmul.pi f13, f26, f21\n"
			"ffrc.ps f21, f0\n"
			"bltu x6, x29, 1f\n"
			"1:\n"
			"slliw x0, x17, 4\n"
			"fnot.pi f4, f4\n"
			"slli x0, x16, 3\n"
			"mulw x12, x12, x15\n"
			"addi x6, x6, 0x0\n"
			"fsub.s f19, f13, f16, rtz\n"
			"fsgnjx.s f26, f18, f4\n"
			"feq.ps f31, f19, f11\n"
			"bgeu x0, x6, 1f\n"
			"1:\n"
			"faddi.pi f4, f16, 0xd0\n"
			"addiw x17, x17, 0xc\n"
			"fmv.s.x f31, x9\n"
			"flem.ps m7, f3, f3\n"
			"ffrc.ps f11, f11\n"
			"fsin.ps f9, f9\n"
			"feq.ps f6, f6, f27\n"
			"sraw x12, x12, x16\n"
			"fsgnjx.s f18, f26, f18\n"
			"bgeu x29, x4, 1f\n"
			"1:\n"
			"addi x21, x21, -1\n"
			"bne x21, x0, LBL_SEQID_2_LOOP_SEQ_HID_0\n"
			"li x5, 3\n" 
			"bne x30, x5, LBL_FAIL_HID_0\n"
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f11,  768 (x5)\n"
			"flw.ps f18,  480 (x5)\n"
			"flw.ps f24,  512 (x5)\n"
			"flw.ps f19,  0 (x5)\n"
			"flw.ps f28,  64 (x5)\n"
			"flw.ps f23,  704 (x5)\n"
			"flw.ps f17,  96 (x5)\n"
			"flw.ps f5,  832 (x5)\n"
			"flw.ps f3,  896 (x5)\n"
			"flw.ps f7,  576 (x5)\n"
			"flw.ps f29,  128 (x5)\n"
			"flw.ps f30,  160 (x5)\n"
			"flw.ps f26,  320 (x5)\n"
			"flw.ps f8,  608 (x5)\n"
			"flw.ps f20,  928 (x5)\n"
			"flw.ps f9,  192 (x5)\n"
			"flw.ps f25,  352 (x5)\n"
			"flw.ps f10,  992 (x5)\n"
			"flw.ps f21,  32 (x5)\n"
			"flw.ps f15,  448 (x5)\n"
			"flw.ps f2,  288 (x5)\n"
			"flw.ps f16,  672 (x5)\n"
			"flw.ps f1,  416 (x5)\n"
			"flw.ps f0,  864 (x5)\n"
			"flw.ps f6,  384 (x5)\n"
			"flw.ps f22,  736 (x5)\n"
			"flw.ps f4,  256 (x5)\n"
			"flw.ps f31,  544 (x5)\n"
			"flw.ps f27,  224 (x5)\n"
			"flw.ps f14,  640 (x5)\n"
			"flw.ps f12,  800 (x5)\n"
			"flw.ps f13,  960 (x5)\n"
			"addi x30, x30, 1\n"
			"li x12, 0xdce13ed51969430b\n"
			"li x9, 0x8b4304d1fc703766\n"
			"mulh x17, x12, x9\n" 
			"fsub.s f6, f6, f15, rtz\n"
			"feq.ps f15, f9, f19\n"
			"feq.ps f16, f0, f11\n"
			"beq x8, x15, 1f\n"
			"1:\n"
			"fcvt.ps.f16 f13, f3\n"
			"for.pi f4, f6, f10\n"
			"fcmov.ps f4, f18, f26, f11\n"
			"feq.pi f16, f18, f6\n"
			"LBL_SEQID_3_INT_DIV_ID_61_HID_0:\n"
			"divw x25, x17, x22\n" 
			"mova.m.x x15\n"
			"remuw x17, x16, x25\n"
			"fsgnj.s f15, f11, f15\n"
			"bltu x22, x24, 1f\n"
			"1:\n"
			"fadd.s f10, f27, f10, rne\n"
			"masknot m3, m5\n"
			"fsub.ps f13, f21, f3, rup\n"
			"fmvz.x.ps x25, f26, 6\n"
			"li x15, 0x6ecc2b88b3fb9d4d\n"
			"srli x5, x15, 0\n" 
			"divuw x5, x6, x22\n"
			"bne x23, x9, 1f\n"
			"1:\n"
			"fsgnjx.ps f31, f19, f3\n"
			"fadd.s f21, f13, f4, rup\n"
			"fsetm.pi m5, f27\n"
			"fsetm.pi m5, f18\n"
			"add x16, x18, x18\n"
			"fsetm.pi m5, f6\n"
			"la x5, rand_ieee754_32\n"
			"flw.ps f21, 608(x5)\n"
			"flw.ps f16, 640(x5)\n"
			"fmin.ps f10, f21, f16\n" 
			"fsgnjn.s f4, f26, f3\n"
			"fcvt.pw.ps f26, f13, rne\n"
			"faddi.pi f11, f11, 0x92\n"
			"fmul.pi f11, f26, f18\n"
			"fsrli.pi f3, f13, 0xb\n"
			"fsub.s f3, f11, f13, rne\n"
			"fsrai.pi f18, f13, 0xf\n"
			"bne x2, x25, 1f\n"
			"1:\n"
			"li x17, 0xdddf29e510bf152b\n"
			"packb x6, x17, x17\n" 
			"fmin.pi f27, f10, f26\n"
			"frcp.ps f4, f16\n"
			"fle.ps f27, f11, f27\n"
			"bltu x3, x0, 1f\n"
			"1:\n"
			"fsub.s f21, f6, f11, rne\n"
			"fclass.ps f4, f31\n"
			"fcvt.ps.f16 f26, f11\n"
			"slti x17, x17, 0x1c\n"
			"LBL_SEQID_3_M0_WRITE_ID_62_HID_0:\n"
			"mova.m.x x6\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0x66a366a5b\n"
			"flw.ps f5, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"fand.pi f9, f0, f21\n"
			"packb x15, x15, x22\n"
			"fence\n"
			"fsatu8.pi f4, f10\n"
			"fcvt.pwu.ps f15, f15, rdn\n"
			"srl x17, x18, x17\n"
			"fle.s x0, f4, f9\n"
			"bge x7, x30, 1f\n"
			"1:\n"
			"la x12, rand_int_32\n"
			"flw.ps f9, 512(x12)\n"
			"flw.ps f4, 192(x12)\n"
			"fmaxu.pi f18, f9, f4\n" 
			"fcvt.ps.f16 f3, f13\n"
			"fsgnjn.ps f16, f3, f13\n"
			"fsub.ps f16, f13, f18, rmm\n"
			"fltu.pi f16, f18, f13\n"
			"fand.pi f16, f18, f3\n"
			"fcmov.ps f3, f3, f13, f3\n"
			"beq x13, x19, 1f\n"
			"1:\n"
			"fmax.ps f19, f18, f3\n"
			"li x6, 0xeb8e2f13be66b2a7\n"
			"fbcx.ps f0, x6\n" 
			"fmin.s f9, f11, f15\n"
			"fand.pi f31, f15, f19\n"
			"fcvt.ps.pw f9, f9, rup\n"
			"xori x6, x16, 0x1d\n"
			"sltu x9, x25, x5\n"
			"fadd.pi f16, f13, f16\n"
			"fadd.ps f15, f3, f16, rmm\n"
			"la x18, rand_int_32\n"
			"flw.ps f4, 352(x18)\n"
			"flw.ps f16, 96(x18)\n"
			"fmaxu.pi f10, f4, f16\n" 
			"ffrc.ps f10, f31\n"
			"fmadd.s f15, f27, f21, f13, rdn\n"
			"fsgnjx.ps f21, f15, f27\n"
			"flt.pi f3, f26, f31\n"
			"mova.m.x x16\n"
			"fsatu8.pi f16, f26\n"
			"bltu x24, x2, 1f\n"
			"1:\n"
			"fltu.pi f4, f31, f31\n"
			"la x9, rand_int_32\n"
			"flw.ps f16, 64(x9)\n"
			"fsat8.pi f21, f16\n" 
			"fadd.pi f27, f27, f27\n"
			"fcvt.f16.ps f13, f13\n"
			"fsrl.pi f16, f27, f9\n"
			"for.pi f21, f13, f9\n"
			"fsgnjn.ps f11, f13, f31\n"
			"fmadd.ps f13, f31, f31, f13, rne\n"
			"frcp.ps f27, f27\n"
			"li x12, 0xad07b66ae0780481\n"
			"li x15, 0xc8579c8c7d4045e5\n"
			"mul x6, x12, x15\n" 
			"frsq.ps f18, f0\n"
			"fsin.ps f19, f19\n"
			"beq x15, x21, 1f\n"
			"1:\n"
			"fxor.pi f19, f18, f19\n"
			"fmin.pi f18, f19, f18\n"
			"fxor.pi f16, f18, f0\n"
			"fminu.pi f4, f19, f19\n"
			"fsrli.pi f18, f19, 0x4\n"
			"bge x21, x9, 1f\n"
			"1:\n"
			"la x9, rand_ieee754_32\n"
			"flw.ps f16, 0(x9)\n"
			"flw.ps f0, 32(x9)\n"
			"fsgnjn.ps f31, f16, f0\n" 
			"fmin.pi f18, f31, f18\n"
			"fsgnjn.ps f27, f16, f26\n"
			"fmulhu.pi f13, f0, f31\n"
			"beq x9, x30, 1f\n"
			"1:\n"
			"fround.ps f31, f4, rne\n"
			"fclass.ps f0, f9\n"
			"fcvt.f16.ps f3, f9\n"
			"fsub.ps f0, f19, f15, rne\n"
			"la x15, rand_ieee754_32\n"
			"flw.ps f21, 576(x15)\n"
			"flw.ps f3, 352(x15)\n"
			"fsub.s f21, f21, f3, rne\n" 
			"srai x16, x17, 2\n"
			"auipc x0, 0x1\n"
			"mov.m.x m2, x5, 0x37\n"
			"rem x5, x6, x5\n"
			"frcp.ps f18, f16\n"
			"fcvt.wu.s x6, f9, rdn\n"
			"fle.ps f16, f16, f31\n"
			"la x22, rand_int_32\n"
			"flw.ps f13, 352(x22)\n"
			"fcvt.ps.pwu f3, f13, rdn\n" 
			"fmax.pi f13, f11, f4\n"
			"beq x25, x21, 1f\n"
			"1:\n"
			"masknot m7, m2\n"
			"packb x6, x22, x22\n"
			"or x12, x12, x18\n"
			"flt.s x6, f13, f15\n"
			"mulhu x25, x12, x25\n"
			"fmaxu.pi f4, f26, f3\n"
			"beq x18, x11, 1f\n"
			"1:\n"
			"la x6, rand_ieee754_32\n"
			"flw.ps f13, 480(x6)\n"
			"flw.ps f19, 704(x6)\n"
			"fmin.s f9, f13, f19\n" 
			"fmin.pi f27, f16, f3\n"
			"beq x31, x26, 1f\n"
			"1:\n"
			"mova.m.x x17\n"
			"beq x20, x17, 1f\n"
			"1:\n"
			"for.pi f0, f16, f0\n"
			"mova.m.x x15\n"
			"fmulhu.pi f4, f10, f18\n"
			"fclass.s x5, f11\n"
			"ecall\n"
			"li x18, 0x4dec0710100b162a\n"
			"slti x22, x18, 0xe\n" 
			"srli x0, x0, 0\n"
			"fcvt.wu.s x9, f11, rdn\n"
			"bltu x23, x22, 1f\n"
			"1:\n"
			"fpackreph.pi f27, f18\n"
			"beq x15, x14, 1f\n"
			"1:\n"
			"fcvt.ps.pwu f11, f11, rne\n"
			"flt.pi f31, f11, f27\n"
			"mulw x12, x17, x22\n"
			"mulw x5, x5, x6\n"
			"li x12, 0xae329e848e2adf3d\n"
			"li x25, 0xba2922ee97368f38\n"
			"slt x5, x12, x25\n" 
			"flt.pi f16, f3, f9\n"
			"fcvt.ps.pwu f11, f19, rtz\n"
			"fmulh.pi f0, f3, f19\n"
			"sltu x0, x18, x0\n"
			"bge x6, x20, 1f\n"
			"1:\n"
			"andi x12, x25, 0x7\n"
			"maskpopcz x25, m2\n"
			"bne x20, x2, 1f\n"
			"1:\n"
			"fle.pi f11, f11, f4\n"
			"LBL_SEQID_3_M0_WRITE_ID_63_HID_0:\n"
			"feqm.ps m5, f11, f9\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"fsrai.pi f31, f31, 0x7\n"
			"maskand m3, m3, m3\n"
			"flt.pi f18, f19, f10\n"
			"blt x0, x18, 1f\n"
			"1:\n"
			"fmul.pi f13, f9, f18\n"
			"fsgnjn.ps f6, f9, f15\n"
			"maskpopcz x12, m5\n"
			"feq.ps f6, f31, f10\n"
			"la x5, rand_int_32\n"
			"flw.ps f27, 672(x5)\n"
			"flw.ps f9, 704(x5)\n"
			"fmin.pi f27, f27, f9\n" 
			"fsgnj.ps f10, f15, f13\n"
			"fnmsub.s f31, f15, f18, f0, rdn\n"
			"xor x12, x6, x9\n"
			"divu x17, x16, x15\n"
			"xor x17, x17, x12\n"
			"slti x6, x9, 0x1a\n"
			"bltu x29, x11, 1f\n"
			"1:\n"
			"ori x16, x6, 0x15\n"
			"li x17, 0x1f4b338711d6ede\n"
			"li x9, 0x534e97454f0c97a0\n"
			"mul x25, x17, x9\n" 
			"sub x16, x9, x15\n"
			"addiw x16, x12, 0xa\n"
			"frsq.ps f13, f13\n"
			"and x25, x25, x6\n"
			"sub x16, x16, x5\n"
			"fltm.pi m5, f0, f31\n"
			"fmax.ps f9, f11, f9\n"
			"la x9, rand_int_32\n"
			"flw.ps f31, 192(x9)\n"
			"flw.ps f21, 896(x9)\n"
			"fadd.pi f10, f31, f21\n" 
			"flt.pi f19, f3, f4\n"
			"slli x15, x15, 0\n"
			"fsgnjn.s f4, f13, f9\n"
			"fclass.ps f3, f3\n"
			"fltu.pi f4, f27, f10\n"
			"srai x18, x16, 3\n"
			"fmul.pi f10, f10, f4\n"
			"li x17, 0xe5b29c18274ae65d\n"
			"addi x18, x17, 0xd\n" 
			"fsgnjn.s f26, f16, f0\n"
			"bge x26, x31, 1f\n"
			"1:\n"
			"fsub.s f13, f13, f11, rmm\n"
			"blt x9, x9, 1f\n"
			"1:\n"
			"fsat8.pi f11, f11\n"
			"bge x4, x22, 1f\n"
			"1:\n"
			"fcvt.pw.ps f6, f16, rup\n"
			"bgeu x22, x31, 1f\n"
			"1:\n"
			"fpackreph.pi f16, f31\n"
			"fsatu8.pi f11, f3\n"
			"bge x30, x5, 1f\n"
			"1:\n"
			"fminu.pi f16, f16, f31\n"
			"li x5, 0x414e4fb4674b881a\n"
			"mulhu x6, x5, x5\n" 
			"mulhsu x15, x5, x17\n"
			"fsrli.pi f19, f19, 0x6\n"
			"fmul.ps f10, f13, f26, rtz\n"
			"fsgnjn.s f0, f0, f3\n"
			"fcvt.wu.s x22, f11, rup\n"
			"auipc x15, 0x1d\n"
			"bgeu x11, x21, 1f\n"
			"1:\n"
			"flt.pi f15, f27, f9\n"
			"la x9, rand_ieee754_32\n"
			"flw.ps f13, 960(x9)\n"
			"fcvt.pwu.ps f4, f13, rmm\n" 
			"maskor m7, m5, m2\n"
			"fsgnj.s f6, f3, f10\n"
			"sraiw x6, x6, 4\n"
			"fnmadd.s f6, f6, f16, f3, rmm\n"
			"fmaxu.pi f6, f4, f3\n"
			"fmvz.x.ps x17, f27, 3\n"
			"bltu x9, x0, 1f\n"
			"1:\n"
			"fsgnj.s f6, f6, f6\n"
			"la x6, rand_ieee754_32\n"
			"flw.ps f6, 160(x6)\n"
			"fcvt.w.s x25, f6, rne\n" 
			"fsrai.pi f27, f10, 0xb\n"
			"fle.ps f0, f10, f10\n"
			"fsat8.pi f0, f31\n"
			"fand.pi f11, f6, f11\n"
			"bne x9, x28, 1f\n"
			"1:\n"
			"fadd.pi f11, f10, f10\n"
			"fsrli.pi f11, f10, 0x3\n"
			"bltu x2, x30, 1f\n"
			"1:\n"
			"fsra.pi f27, f10, f11\n"
			"LBL_SEQID_3_M0_WRITE_ID_64_HID_0:\n"
			"feqm.ps m3, f0, f16\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"fsll.pi f16, f16, f0\n"
			"fsra.pi f27, f19, f0\n"
			"fsub.pi f26, f18, f11\n"
			"fnmsub.s f31, f9, f15, f21, rtz\n"
			"xori x6, x22, 0x19\n"
			"fandi.pi f27, f26, 0x25\n"
			"remw x22, x5, x22\n"
			"bne x18, x10, 1f\n"
			"1:\n"
			"LBL_SEQID_3_M0_WRITE_ID_65_HID_0:\n"
			"feqm.ps m5, f10, f0\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"flog.ps f16, f16\n"
			"fcvt.pw.ps f3, f3, rdn\n"
			"fmul.s f21, f27, f3, rup\n"
			"fle.ps f26, f26, f26\n"
			"fmulhu.pi f16, f16, f6\n"
			"fsub.s f15, f16, f19, rup\n"
			"fsat8.pi f9, f4\n"
			"li x5, 0xa40a3a100900f2a6\n"
			"li x25, 0x1e3ef8bc464027bb\n"
			"sraw x5, x5, x25\n" 
			"feq.pi f11, f3, f11\n"
			"sll x17, x16, x17\n"
			"ebreak\n"
			"fadd.ps f16, f13, f3, rup\n"
			"sltu x0, x22, x16\n"
			"beq x31, x29, 1f\n"
			"1:\n"
			"fmax.s f3, f3, f27\n"
			"subw x9, x17, x16\n"
			"li x9, 0x5460f3557429d3ae\n"
			"addiw x22, x9, 0x1e\n" 
			"fmax.s f9, f0, f13\n"
			"bne x0, x5, 1f\n"
			"1:\n"
			"fcmovm.ps f10, f19, f0\n"
			"mulw x18, x18, x25\n"
			"fcvt.ps.pwu f18, f26, rtz\n"
			"div x22, x17, x22\n"
			"fadd.ps f6, f19, f9, rup\n"
			"fcvt.ps.pw f31, f11, rne\n"
			"la x5, rand_ieee754_32\n"
			"flw.ps f19, 320(x5)\n"
			"flw.ps f15, 224(x5)\n"
			"flw.ps f3, 512(x5)\n"
			"fnmadd.ps f6, f19, f15, f3, rmm\n" 
			"fswizz.ps f15, f13, 0x12\n"
			"add x12, x9, x0\n"
			"maskand m2, m3, m3\n"
			"ecall\n"
			"remu x25, x12, x0\n"
			"fadd.s f10, f10, f11, rtz\n"
			"fcvt.wu.s x5, f13, rtz\n"
			"LBL_SEQID_3_M0_WRITE_ID_66_HID_0:\n"
			"maskor m3, m7, m2\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0x6622b59d01\n"
			"flw.ps f5, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"masknot m2, m5\n"
			"fsgnjx.ps f21, f11, f21\n"
			"fandi.pi f27, f21, 0x16f\n"
			"fsgnjn.ps f3, f21, f13\n"
			"fmax.pi f26, f0, f16\n"
			"xor x16, x6, x0\n"
			"fle.s x18, f0, f27\n"
			"LBL_SEQID_3_M0_WRITE_ID_67_HID_0:\n"
			"mova.m.x x5\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"fmv.x.s x17, f3\n"
			"fandi.pi f9, f0, 0xde\n"
			"fmvs.x.ps x12, f0, 5\n"
			"mov.m.x m3, x17, 0x53\n"
			"fle.pi f9, f9, f9\n"
			"mov.m.x m5, x16, 0x15\n"
			"maskpopc x5, m7\n"
			"mov.m.x m0, x0, 0xFF\n"
			"li x5, 0x20\n"
			"la x31, rand_ieee754_32\n"
			"add x31, x31, x5\n"
			"flw.ps f0, 0(x31)\n"
			"flw.ps f10, 3(x31)\n"
			"flw.ps f11, 7(x31)\n"
			"flw.ps f13, 12(x31)\n"
			"flw.ps f15, 13(x31)\n"
			"flw.ps f16, 4(x31)\n"
			"flw.ps f18, 14(x31)\n"
			"flw.ps f19, 5(x31)\n"
			"flw.ps f21, 6(x31)\n"
			"flw.ps f26, 18(x31)\n"
			"flw.ps f27, 11(x31)\n"
			"flw.ps f3, 38(x31)\n"
			"flw.ps f31, 26(x31)\n"
			"flw.ps f4, 20(x31)\n"
			"flw.ps f6, 39(x31)\n"
			"flw.ps f9, 52(x31)\n"
			"la x31, rand_int_32\n"
			"add x31, x31, x5\n"
			"lw x0, 0(x31)\n"
			"lw x12, 2(x31)\n"
			"lw x15, 5(x31)\n"
			"lw x16, 2(x31)\n"
			"lw x17, 5(x31)\n"
			"lw x18, 18(x31)\n"
			"lw x22, 24(x31)\n"
			"lw x25, 15(x31)\n"
			"lw x5, 28(x31)\n"
			"lw x6, 17(x31)\n"
			"lw x9, 29(x31)\n"
			"li x21, 0x1\n"
			"LBL_SEQID_3_LOOP_SEQ_HID_0:\n"
			"fsrl.pi f21, f11, f10\n"
			"fcmov.ps f6, f18, f10, f3\n"
			"fcvt.ps.f16 f4, f4\n"
			"maskand m5, m7, m3\n"
			"fle.s x12, f18, f21\n"
			"fmax.pi f10, f26, f16\n"
			"srliw x16, x16, 2\n"
			"feq.ps f19, f6, f26\n"
			"fcvt.pwu.ps f19, f19, rdn\n"
			"beq x13, x23, 1f\n"
			"1:\n"
			"fcvt.pw.ps f16, f31, rmm\n"
			"fcvt.pwu.ps f27, f21, rtz\n"
			"fmax.pi f3, f10, f3\n"
			"for.pi f10, f9, f16\n"
			"fnmsub.ps f27, f0, f21, f31, rne\n"
			"flt.ps f10, f6, f10\n"
			"div x0, x9, x6\n"
			"fcvt.s.w f19, x5, rtz\n"
			"fsub.ps f27, f4, f19, rup\n"
			"fclass.ps f3, f11\n"
			"fnmadd.ps f18, f18, f19, f13, rtz\n"
			"for.pi f26, f26, f19\n"
			"blt x14, x26, 1f\n"
			"1:\n"
			"fnot.pi f15, f15\n"
			"divw x18, x18, x25\n"
			"bge x28, x30, 1f\n"
			"1:\n"
			"maskpopc x18, m5\n"
			"feq.s x25, f10, f18\n"
			"fmin.pi f19, f9, f4\n"
			"add x22, x18, x15\n"
			"fsub.pi f26, f21, f26\n"
			"beq x22, x31, 1f\n"
			"1:\n"
			"fmulhu.pi f15, f0, f0\n"
			"fsgnjn.s f16, f11, f16\n"
			"fmin.s f18, f18, f16\n"
			"bne x14, x19, 1f\n"
			"1:\n"
			"fmadd.s f9, f18, f11, f9, rdn\n"
			"fmv.s.x f31, x5\n"
			"remw x16, x16, x5\n"
			"fxor.pi f15, f18, f18\n"
			"srai x5, x17, 1\n"
			"fsetm.pi m2, f21\n"
			"bgeu x2, x22, 1f\n"
			"1:\n"
			"or x18, x5, x18\n"
			"beq x22, x5, 1f\n"
			"1:\n"
			"flt.pi f31, f13, f26\n"
			"bltu x22, x22, 1f\n"
			"1:\n"
			"sub x0, x6, x6\n"
			"div x17, x5, x17\n"
			"rem x0, x0, x17\n"
			"srlw x17, x15, x18\n"
			"slliw x12, x5, 0\n"
			"srli x16, x0, 5\n"
			"fsub.ps f11, f13, f11, rtz\n"
			"mov.m.x m7, x6, 0x6a\n"
			"fcmovm.ps f26, f21, f21\n"
			"sub x9, x12, x9\n"
			"fcmovm.ps f15, f15, f19\n"
			"blt x4, x25, 1f\n"
			"1:\n"
			"fmv.s.x f3, x17\n"
			"sub x12, x18, x12\n"
			"fmin.ps f16, f21, f16\n"
			"srl x15, x15, x25\n"
			"fsin.ps f18, f13\n"
			"fcvt.wu.s x17, f19, rup\n"
			"fsgnjx.s f3, f16, f27\n"
			"auipc x22, 0x16\n"
			"fsin.ps f15, f15\n"
			"fpackrepb.pi f21, f15\n"
			"fsub.pi f9, f15, f27\n"
			"fltu.pi f31, f31, f19\n"
			"fcvt.f16.ps f11, f27\n"
			"srlw x5, x16, x12\n"
			"mov.m.x m7, x12, 0xe\n"
			"blt x11, x8, 1f\n"
			"1:\n"
			"sraiw x16, x25, 0\n"
			"fmsub.s f13, f27, f15, f19, rdn\n"
			"bltu x24, x24, 1f\n"
			"1:\n"
			"fsgnjn.ps f13, f9, f26\n"
			"fcvt.s.w f16, x17, rtz\n"
			"xor x18, x18, x25\n"
			"fmin.s f31, f31, f11\n"
			"fsgnjx.ps f16, f16, f0\n"
			"flt.s x6, f0, f0\n"
			"fbci.pi f15, 0xef52\n"
			"ebreak\n"
			"bge x30, x7, 1f\n"
			"1:\n"
			"sll x0, x12, x5\n"
			"bltu x15, x17, 1f\n"
			"1:\n"
			"mova.m.x x5\n"
			"fence\n"
			"fcmov.ps f16, f13, f31, f27\n"
			"fmaxu.pi f0, f10, f19\n"
			"sltiu x12, x12, 0xd\n"
			"sraiw x9, x9, 0\n"
			"fcmovm.ps f26, f3, f26\n"
			"fmv.x.s x22, f10\n"
			"bltu x13, x18, 1f\n"
			"1:\n"
			"fpackreph.pi f18, f13\n"
			"fsgnjx.s f10, f16, f11\n"
			"fclass.s x0, f19\n"
			"bltu x3, x12, 1f\n"
			"1:\n"
			"ecall\n"
			"maskand m2, m3, m7\n"
			"bltu x30, x7, 1f\n"
			"1:\n"
			"fslli.pi f11, f18, 0x6\n"
			"fnmsub.ps f11, f26, f31, f11, rne\n"
			"srlw x9, x12, x0\n"
			"faddi.pi f6, f31, 0x1ca\n"
			"fsgnjx.ps f11, f13, f13\n"
			"ffrc.ps f6, f18\n"
			"fmax.s f10, f26, f3\n"
			"fsgnjx.ps f11, f27, f4\n"
			"fclass.ps f18, f31\n"
			"andi x15, x9, 0xc\n"
			"blt x4, x11, 1f\n"
			"1:\n"
			"fcvt.wu.s x17, f9, rtz\n"
			"fmadd.ps f31, f0, f27, f13, rtz\n"
			"beq x7, x8, 1f\n"
			"1:\n"
			"fcvt.pw.ps f0, f0, rtz\n"
			"fle.ps f31, f10, f26\n"
			"srli x6, x16, 3\n"
			"srli x6, x15, 2\n"
			"srai x22, x16, 1\n"
			"bltu x8, x19, 1f\n"
			"1:\n"
			"frcp.ps f0, f21\n"
			"fmax.s f21, f10, f18\n"
			"xor x22, x22, x22\n"
			"divu x18, x15, x16\n"
			"mulhsu x18, x5, x22\n"
			"bne x20, x0, 1f\n"
			"1:\n"
			"fmin.pi f10, f11, f10\n"
			"fexp.ps f26, f9\n"
			"fcvt.pw.ps f21, f21, rtz\n"
			"ecall\n"
			"ffrc.ps f26, f21\n"
			"fltm.pi m7, f9, f21\n"
			"fmsub.s f21, f21, f6, f13, rdn\n"
			"for.pi f0, f21, f26\n"
			"fround.ps f18, f21, rup\n"
			"feq.pi f18, f18, f26\n"
			"bltu x28, x5, 1f\n"
			"1:\n"
			"flog.ps f26, f10\n"
			"fmin.pi f4, f26, f4\n"
			"mulw x22, x22, x6\n"
			"divw x18, x15, x18\n"
			"fclass.s x5, f19\n"
			"lui x18, 0x9\n"
			"fmul.pi f0, f27, f21\n"
			"flt.ps f26, f11, f3\n"
			"fnot.pi f26, f27\n"
			"bne x17, x23, 1f\n"
			"1:\n"
			"fadd.ps f15, f0, f15, rup\n"
			"sltiu x18, x22, 0x7\n"
			"fcvt.s.w f19, x17, rup\n"
			"srliw x17, x18, 1\n"
			"fcvt.ps.pw f11, f27, rdn\n"
			"fmvz.x.ps x22, f10, 0\n"
			"fcvt.pwu.ps f13, f19, rne\n"
			"fmin.ps f26, f13, f0\n"
			"fsll.pi f0, f9, f15\n"
			"fcvt.w.s x25, f31, rne\n"
			"bge x26, x23, 1f\n"
			"1:\n"
			"fround.ps f18, f18, rtz\n"
			"div x6, x0, x6\n"
			"fmaxu.pi f6, f15, f16\n"
			"fxor.pi f31, f21, f13\n"
			"ffrc.ps f6, f9\n"
			"lui x15, 0x8\n"
			"bltu x4, x0, 1f\n"
			"1:\n"
			"fexp.ps f0, f16\n"
			"maskxor m2, m2, m5\n"
			"flog.ps f16, f31\n"
			"fslli.pi f11, f11, 0xa\n"
			"fsgnjn.ps f3, f21, f26\n"
			"fsgnjn.ps f21, f21, f9\n"
			"srli x16, x22, 3\n"
			"fsrli.pi f31, f13, 0xe\n"
			"bge x30, x0, 1f\n"
			"1:\n"
			"fcvt.w.s x5, f10, rtz\n"
			"feqm.ps m2, f6, f10\n"
			"fsrli.pi f26, f21, 0xf\n"
			"rem x18, x18, x15\n"
			"fsrli.pi f18, f27, 0x6\n"
			"bne x2, x12, 1f\n"
			"1:\n"
			"mul x25, x12, x25\n"
			"blt x7, x19, 1f\n"
			"1:\n"
			"fmul.ps f4, f9, f15, rdn\n"
			"fslli.pi f6, f15, 0xe\n"
			"bge x5, x26, 1f\n"
			"1:\n"
			"ffrc.ps f16, f9\n"
			"addi x21, x21, -1\n"
			"bne x21, x0, LBL_SEQID_3_LOOP_SEQ_HID_0\n"
			"li x5, 4\n" 
			"bne x30, x5, LBL_FAIL_HID_0\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_4_INT_AMO_ID_68_HID_0:\n"
			"li x6, 0x8159792b90\n" 
			"amoaddl.d x17, x22, (x6)\n" 
			"feq.ps f6, f16, f19\n"
			"fmaxu.pi f26, f10, f3\n"
			"bge x21, x31, 1f\n"
			"1:\n"
			"fmsub.ps f19, f19, f3, f15, rtz\n"
			"for.pi f6, f15, f10\n"
			"fmax.s f13, f10, f21\n"
			"fexp.ps f26, f3\n"
			"fnmsub.s f19, f10, f10, f19, rdn\n"
			"fsgnjn.ps f11, f21, f16\n"
			"fsatu8.pi f19, f21\n"
			"LBL_SEQID_4_INT_AMO_ID_69_HID_0:\n"
			"li x17, 0x82337772ec\n" 
			"amominug.w x15, x16, (x17)\n" 
			"fsub.ps f10, f10, f16, rtz\n"
			"fadd.s f6, f31, f31, rmm\n"
			"fnot.pi f6, f21\n"
			"fmin.pi f27, f27, f27\n"
			"fxor.pi f26, f26, f27\n"
			"beq x30, x28, 1f\n"
			"1:\n"
			"feq.pi f18, f10, f26\n"
			"fcmovm.ps f19, f26, f26\n"
			"bge x16, x11, 1f\n"
			"1:\n"
			"fsgnj.s f31, f31, f31\n"
			"fadd.ps f26, f13, f10, rdn\n"
			"LBL_SEQID_4_INT_STORE_ID_70_HID_0:\n"
			"li x9, 0x8106560387\n" 
			"sb x17, -52(x9)\n" 
			"maskxor m3, m3, m7\n"
			"addiw x5, x5, 0x1b\n"
			"maskand m2, m2, m2\n"
			"beq x8, x20, 1f\n"
			"1:\n"
			"mul x17, x6, x25\n"
			"fpackreph.pi f13, f31\n"
			"fbci.pi f4, 0x1eb96\n"
			"bne x23, x24, 1f\n"
			"1:\n"
			"mulh x12, x9, x0\n"
			"fsetm.pi m5, f6\n"
			"bge x8, x2, 1f\n"
			"1:\n"
			"mova.x.m x15\n"
			"LBL_SEQID_4_INT_AMO_ID_71_HID_0:\n"
			"li x6, 0x815b7dd82c\n" 
			"amominul.w x9, x18, (x6)\n" 
			"fsgnjx.s f13, f13, f19\n"
			"blt x16, x18, 1f\n"
			"1:\n"
			"fcvt.pwu.ps f11, f10, rdn\n"
			"fmin.ps f0, f9, f13\n"
			"fsub.pi f26, f27, f9\n"
			"beq x19, x6, 1f\n"
			"1:\n"
			"fsub.pi f4, f4, f9\n"
			"fclass.ps f27, f10\n"
			"fmulh.pi f0, f16, f16\n"
			"fmulh.pi f0, f3, f4\n"
			"fmsub.ps f10, f9, f11, f10, rup\n"
			"LBL_SEQID_4_INT_AMO_ID_72_HID_0:\n"
			"li x25, 0x8235bd2810\n" 
			"amoxorg.w x6, x6, (x25)\n" 
			"fsatu8.pi f21, f21\n"
			"feqm.ps m5, f13, f18\n"
			"fbcx.ps f19, x0\n"
			"flog.ps f18, f18\n"
			"bne x17, x11, 1f\n"
			"1:\n"
			"sllw x17, x16, x25\n"
			"maskpopcz x22, m5\n"
			"auipc x9, 0x2\n"
			"fmsub.s f16, f0, f13, f9, rup\n"
			"frcp.ps f16, f0\n"
			"LBL_SEQID_4_FP_AMO_ID_73_HID_0:\n"
			"li x16, 0x8252040030\n" 
			"la x17, mem_ptr_offset\n"
			"flw.ps f0, 896(x17)\n"
			"famoaddl.pi f3, f0 (x16)\n" 
			"fswizz.ps f6, f31, 0x1d\n"
			"mul x5, x16, x0\n"
			"packb x16, x0, x16\n"
			"sllw x16, x5, x0\n"
			"fsin.ps f10, f10\n"
			"srli x0, x0, 4\n"
			"fcvt.ps.f16 f10, f31\n"
			"fpackrepb.pi f0, f10\n"
			"fnmadd.s f10, f10, f19, f10, rtz\n"
			"LBL_SEQID_4_FP_L_GATHER_ID_74_HID_0:\n"
			"li x15, 0x8254b04800\n" 
			"la x22, mem_ptr_offset\n"
			"flq2 f16, 192(x22)\n"
			"fscwl.ps f24, f16 (x15)\n"
			"fgbl.ps f19, f16 (x15)\n" 
			"fsub.pi f3, f4, f10\n"
			"for.pi f4, f19, f15\n"
			"fcmov.ps f16, f18, f10, f16\n"
			"bltu x9, x25, 1f\n"
			"1:\n"
			"fsrl.pi f21, f19, f19\n"
			"fsll.pi f9, f6, f13\n"
			"ffrc.ps f27, f6\n"
			"for.pi f4, f21, f15\n"
			"fmul.pi f21, f21, f19\n"
			"blt x10, x11, 1f\n"
			"1:\n"
			"fsgnjx.ps f9, f3, f3\n"
			"LBL_SEQID_4_FP_G_STORE_ID_75_HID_0:\n"
			"li x5, 0x82337772e0\n" 
			"mov.m.x m0, zero, 0xff\n"
			"fswg.ps f15, (x5)\n" 
			"fsrli.pi f9, f0, 0x8\n"
			"fnmadd.ps f9, f9, f10, f11, rup\n"
			"blt x1, x19, 1f\n"
			"1:\n"
			"fadd.s f19, f18, f31, rmm\n"
			"subw x17, x15, x17\n"
			"fmin.pi f6, f21, f11\n"
			"bge x6, x16, 1f\n"
			"1:\n"
			"fnmsub.ps f4, f27, f13, f0, rup\n"
			"maskpopcz x5, m7\n"
			"bne x23, x3, 1f\n"
			"1:\n"
			"fsgnjx.ps f0, f11, f10\n"
			"divw x6, x16, x25\n"
			"blt x1, x4, 1f\n"
			"1:\n"
			"LBL_SEQID_4_INT_AMO_ID_76_HID_0:\n"
			"li x16, 0x825dde0590\n" 
			"amoaddl.d x15, x6, (x16)\n" 
			"fmul.pi f6, f15, f26\n"
			"sraiw x22, x22, 0\n"
			"bltu x6, x8, 1f\n"
			"1:\n"
			"feq.s x6, f13, f9\n"
			"fle.pi f9, f9, f26\n"
			"bge x27, x21, 1f\n"
			"1:\n"
			"fcvt.w.s x15, f9, rup\n"
			"fslli.pi f6, f0, 0x4\n"
			"fbcx.ps f18, x22\n"
			"fslli.pi f26, f9, 0xf\n"
			"mova.m.x x25\n"
			"LBL_SEQID_4_INT_AMO_ID_77_HID_0:\n"
			"li x15, 0x814cce0018\n" 
			"amominl.d x9, x18, (x15)\n" 
			"faddi.pi f0, f4, 0x82\n"
			"divuw x22, x18, x22\n"
			"fsatu8.pi f31, f4\n"
			"fsgnjn.s f6, f6, f6\n"
			"fcmov.ps f31, f31, f31, f4\n"
			"fmv.s.x f18, x18\n"
			"remu x25, x16, x18\n"
			"slli x16, x12, 2\n"
			"bne x18, x21, 1f\n"
			"1:\n"
			"sra x12, x9, x25\n"
			"LBL_SEQID_4_FP_STORE_ID_78_HID_0:\n"
			"li x15, 0x81025015d9\n" 
			"fsw.ps f0, -1909(x15)\n" 
			"fcvt.s.w f16, x0, rmm\n"
			"fround.ps f15, f10, rtz\n"
			"ori x15, x9, 0x2\n"
			"fsrl.pi f6, f16, f27\n"
			"bne x30, x20, 1f\n"
			"1:\n"
			"fsrl.pi f27, f16, f31\n"
			"add x15, x5, x25\n"
			"fmax.pi f18, f27, f19\n"
			"fcmov.ps f27, f0, f15, f15\n"
			"fnmadd.s f6, f27, f31, f31, rup\n"
			"LBL_SEQID_4_FP_L_SCATTER_ID_79_HID_0:\n"
			"li x18, 0x8159792b90\n" 
			"la x12, mem_ptr_offset\n"
			"flw.ps f10, 20(x12)\n"
			"fscbl.ps f0, f10 (x18)\n" 
			"fmin.s f10, f27, f3\n"
			"fmax.pi f9, f9, f10\n"
			"fmulhu.pi f10, f13, f19\n"
			"bgeu x8, x19, 1f\n"
			"1:\n"
			"fcvt.f16.ps f11, f0\n"
			"fmulhu.pi f21, f10, f9\n"
			"beq x27, x21, 1f\n"
			"1:\n"
			"fnmadd.ps f21, f10, f18, f19, rne\n"
			"fmul.s f18, f0, f6, rtz\n"
			"fcvt.ps.pw f3, f3, rup\n"
			"bge x17, x8, 1f\n"
			"1:\n"
			"flt.pi f18, f27, f0\n"
			"LBL_SEQID_4_FP_AMO_ID_80_HID_0:\n"
			"li x5, 0x8257bc0034\n" 
			"la x22, mem_ptr_offset\n"
			"flw.ps f4, 544(x22)\n"
			"famoorl.pi f0, f4 (x5)\n" 
			"auipc x6, 0x0\n"
			"bgeu x12, x27, 1f\n"
			"1:\n"
			"fcvt.pw.ps f11, f9, rmm\n"
			"bgeu x27, x8, 1f\n"
			"1:\n"
			"sllw x18, x18, x18\n"
			"bne x21, x22, 1f\n"
			"1:\n"
			"fltm.ps m3, f0, f0\n"
			"bgeu x15, x13, 1f\n"
			"1:\n"
			"flem.ps m2, f3, f11\n"
			"fltm.pi m5, f31, f16\n"
			"fmin.ps f11, f11, f6\n"
			"fcvt.s.w f16, x5, rup\n"
			"for.pi f3, f3, f16\n"
			"LBL_SEQID_4_FP_L_GATHER_ID_81_HID_0:\n"
			"li x9, 0x815d4db698\n" 
			"la x5, mem_ptr_offset\n"
			"flw.ps f31, 132(x5)\n"
			"fscwl.ps f5, f31 (x9)\n"
			"fghl.ps f10, f31 (x9)\n" 
			"fcvt.pwu.ps f26, f9, rne\n"
			"bgeu x20, x28, 1f\n"
			"1:\n"
			"fsgnj.s f11, f31, f11\n"
			"fswizz.ps f9, f18, 0x2\n"
			"fcvt.ps.pwu f16, f9, rdn\n"
			"fsrl.pi f19, f3, f19\n"
			"fmadd.s f21, f10, f21, f16, rne\n"
			"fcvt.f16.ps f6, f15\n"
			"bge x17, x5, 1f\n"
			"1:\n"
			"for.pi f15, f11, f4\n"
			"fsrl.pi f6, f6, f13\n"
			"LBL_SEQID_4_FP_AMO_ID_82_HID_0:\n"
			"li x5, 0x823dde0590\n" 
			"la x18, mem_ptr_offset\n"
			"flw.ps f21, 64(x18)\n"
			"famoming.pi f16, f21 (x5)\n" 
			"fsgnjn.s f27, f11, f27\n"
			"fsrai.pi f9, f10, 0xc\n"
			"flt.pi f9, f18, f4\n"
			"fmax.pi f10, f11, f19\n"
			"fcvt.ps.pw f10, f10, rdn\n"
			"fmax.pi f9, f10, f10\n"
			"fadd.ps f26, f27, f19, rne\n"
			"feq.pi f10, f15, f10\n"
			"fmulh.pi f4, f10, f15\n"
			"LBL_SEQID_4_CSR_ID_83_HID_0:\n"
			"csrr x0, hpmcounter4\n"
			"flog.ps f21, f13\n"
			"fsat8.pi f13, f11\n"
			"fmvz.x.ps x9, f13, 3\n"
			"mova.m.x x12\n"
			"maskpopcz x16, m5\n"
			"fsrai.pi f13, f31, 0xd\n"
			"flog.ps f0, f11\n"
			"bge x7, x29, 1f\n"
			"1:\n"
			"fnmsub.s f6, f6, f19, f13, rdn\n"
			"srli x15, x12, 0\n"
			"LBL_SEQID_4_FP_L_LOAD_ID_84_HID_0:\n"
			"li x22, 0x824275f820\n" 
			"mov.m.x m0, zero, 0xff\n"
			"fswl.ps f19, (x22)\n"
			"flwl.ps f3, (x22)\n" 
			"frsq.ps f18, f4\n"
			"fmax.pi f6, f6, f10\n"
			"fcvt.pwu.ps f13, f13, rne\n"
			"fsub.s f4, f6, f15, rtz\n"
			"fsgnj.s f26, f31, f10\n"
			"bgeu x2, x25, 1f\n"
			"1:\n"
			"fmax.s f27, f27, f15\n"
			"faddi.pi f27, f4, 0x1b6\n"
			"fnmsub.ps f19, f18, f27, f0, rdn\n"
			"beq x22, x10, 1f\n"
			"1:\n"
			"fle.pi f27, f15, f27\n"
			"LBL_SEQID_4_FP_L_LOAD_ID_85_HID_0:\n"
			"li x5, 0x8150418960\n" 
			"mov.m.x m0, zero, 0xff\n"
			"fswl.ps f16, (x5)\n"
			"flwl.ps f9, (x5)\n" 
			"fsub.ps f21, f21, f27, rdn\n"
			"fle.pi f16, f15, f16\n"
			"blt x2, x16, 1f\n"
			"1:\n"
			"ffrc.ps f13, f15\n"
			"bne x18, x25, 1f\n"
			"1:\n"
			"fminu.pi f21, f27, f6\n"
			"fmin.pi f18, f18, f18\n"
			"fle.pi f15, f10, f15\n"
			"fnmsub.ps f0, f26, f27, f0, rmm\n"
			"fcmov.ps f27, f27, f11, f26\n"
			"bltu x8, x12, 1f\n"
			"1:\n"
			"fnmsub.ps f13, f19, f0, f18, rmm\n"
			"LBL_SEQID_4_INT_AMO_ID_86_HID_0:\n"
			"li x9, 0x824333a588\n" 
			"amoaddl.d x12, x6, (x9)\n" 
			"fsgnj.ps f18, f18, f21\n"
			"beq x10, x19, 1f\n"
			"1:\n"
			"fadd.pi f3, f3, f16\n"
			"fclass.ps f9, f6\n"
			"fexp.ps f11, f15\n"
			"fcvt.ps.pwu f3, f0, rmm\n"
			"fle.pi f15, f19, f3\n"
			"fcvt.ps.f16 f31, f3\n"
			"fcmovm.ps f16, f16, f21\n"
			"bge x14, x26, 1f\n"
			"1:\n"
			"ffrc.ps f16, f4\n"
			"LBL_SEQID_4_FP_AMO_ID_87_HID_0:\n"
			"li x18, 0x814d5605d8\n" 
			"la x22, mem_ptr_offset\n"
			"flw.ps f13, 928(x22)\n"
			"famominl.pi f26, f13 (x18)\n" 
			"fcvt.s.wu f3, x17, rup\n"
			"fandi.pi f26, f26, 0x5c\n"
			"addw x12, x0, x17\n"
			"fcvt.s.wu f31, x6, rne\n"
			"fbci.pi f13, 0x22c7a\n"
			"fmul.s f26, f21, f21, rne\n"
			"fsrai.pi f21, f21, 0xd\n"
			"fadd.s f26, f26, f15, rdn\n"
			"remw x18, x18, x25\n"
			"LBL_SEQID_4_INT_AMO_ID_88_HID_0:\n"
			"li x18, 0x8224107020\n" 
			"amominug.d x22, x6, (x18)\n" 
			"mova.m.x x25\n"
			"fmadd.s f16, f3, f27, f9, rne\n"
			"remu x17, x6, x17\n"
			"beq x2, x12, 1f\n"
			"1:\n"
			"fsrl.pi f18, f27, f21\n"
			"mulhsu x16, x6, x18\n"
			"maskxor m3, m5, m2\n"
			"srai x12, x12, 3\n"
			"fclass.s x9, f9\n"
			"mul x22, x0, x9\n"
			"LBL_SEQID_4_INT_STORE_ID_89_HID_0:\n"
			"li x12, 0x8207ae057a\n" 
			"sd x6, 1133(x12)\n" 
			"fpackrepb.pi f3, f3\n"
			"feq.ps f18, f15, f15\n"
			"bltu x29, x30, 1f\n"
			"1:\n"
			"fnot.pi f11, f4\n"
			"bltu x22, x27, 1f\n"
			"1:\n"
			"fcvt.ps.pw f15, f9, rne\n"
			"flt.pi f9, f9, f15\n"
			"fandi.pi f27, f4, 0x6\n"
			"fsat8.pi f10, f26\n"
			"fmin.ps f3, f16, f9\n"
			"fmin.s f27, f27, f10\n"
			"LBL_SEQID_4_INT_AMO_ID_90_HID_0:\n"
			"li x6, 0x8125237004\n" 
			"amoorg.w x25, x12, (x6)\n" 
			"fcvt.w.s x25, f11, rtz\n"
			"fsat8.pi f11, f10\n"
			"fnmadd.ps f15, f31, f6, f10, rtz\n"
			"fle.ps f26, f0, f27\n"
			"srai x18, x9, 0\n"
			"srlw x6, x6, x17\n"
			"bltu x17, x28, 1f\n"
			"1:\n"
			"masknot m5, m7\n"
			"packb x25, x25, x0\n"
			"fbci.pi f6, 0x648f0\n"
			"LBL_SEQID_4_INT_AMO_ID_91_HID_0:\n"
			"li x12, 0x8220ae005c\n" 
			"amominug.w x9, x17, (x12)\n" 
			"add x5, x5, x6\n"
			"maskand m3, m3, m2\n"
			"fnmadd.s f26, f26, f26, f19, rup\n"
			"slt x12, x17, x16\n"
			"flt.pi f0, f19, f26\n"
			"fsra.pi f16, f16, f26\n"
			"bge x31, x22, 1f\n"
			"1:\n"
			"sltiu x25, x16, 0x13\n"
			"sraiw x6, x17, 0\n"
			"blt x27, x1, 1f\n"
			"1:\n"
			"fbcx.ps f26, x0\n"
			"LBL_SEQID_4_INT_AMO_ID_92_HID_0:\n"
			"li x6, 0x8237ae0578\n" 
			"amoandg.d x5, x5, (x6)\n" 
			"fsgnjx.s f4, f18, f15\n"
			"bgeu x22, x23, 1f\n"
			"1:\n"
			"ffrc.ps f19, f21\n"
			"fexp.ps f11, f19\n"
			"bgeu x5, x23, 1f\n"
			"1:\n"
			"fle.pi f19, f19, f10\n"
			"faddi.pi f21, f10, 0x1ca\n"
			"fmul.ps f26, f26, f31, rdn\n"
			"bltu x1, x13, 1f\n"
			"1:\n"
			"fnmadd.ps f15, f6, f9, f15, rup\n"
			"bne x26, x20, 1f\n"
			"1:\n"
			"fnot.pi f10, f31\n"
			"fsub.s f10, f21, f13, rup\n"
			"LBL_SEQID_4_INT_AMO_ID_93_HID_0:\n"
			"li x16, 0x8234257f20\n" 
			"amoming.w x12, x5, (x16)\n" 
			"div x12, x12, x6\n"
			"bge x8, x11, 1f\n"
			"1:\n"
			"fltu.pi f3, f3, f15\n"
			"mulw x0, x6, x6\n"
			"bgeu x25, x15, 1f\n"
			"1:\n"
			"feq.ps f3, f3, f26\n"
			"auipc x12, 0xb\n"
			"divu x15, x12, x12\n"
			"fmvz.x.ps x6, f26, 3\n"
			"fsgnjn.ps f0, f27, f27\n"
			"bne x31, x28, 1f\n"
			"1:\n"
			"fminu.pi f13, f31, f26\n"
			"LBL_SEQID_4_INT_LOAD_ID_94_HID_0:\n"
			"li x16, 0x820333a588\n" 
			"sd x18, -490(x16)\n"
			"lhu x17, -490(x16)\n" 
			"ori x15, x0, 0x3\n"
			"fminu.pi f26, f31, f15\n"
			"feq.s x25, f26, f19\n"
			"subw x17, x17, x17\n"
			"fcvt.ps.pw f21, f15, rmm\n"
			"fsll.pi f21, f26, f15\n"
			"sra x25, x17, x17\n"
			"slt x18, x0, x5\n"
			"bge x23, x26, 1f\n"
			"1:\n"
			"sllw x17, x17, x5\n"
			"bge x18, x21, 1f\n"
			"1:\n"
			"LBL_SEQID_4_FP_G_LOAD_ID_95_HID_0:\n"
			"li x25, 0x822b0602e0\n" 
			"mov.m.x m0, zero, 0xff\n"
			"fswg.ps f9, (x25)\n"
			"flwg.ps f31, (x25)\n" 
			"fmulh.pi f0, f0, f0\n"
			"bge x15, x2, 1f\n"
			"1:\n"
			"addw x0, x22, x0\n"
			"fround.ps f4, f4, rmm\n"
			"packb x25, x0, x12\n"
			"fsrli.pi f11, f11, 0x8\n"
			"blt x10, x24, 1f\n"
			"1:\n"
			"remu x5, x6, x6\n"
			"fsub.ps f18, f4, f11, rne\n"
			"fsrai.pi f18, f6, 0xe\n"
			"mova.x.m x6\n"
			"LBL_SEQID_4_FP_AMO_ID_96_HID_0:\n"
			"li x15, 0x81225015d8\n" 
			"la x12, mem_ptr_offset\n"
			"flw.ps f27, 864(x12)\n"
			"famoandg.pi f11, f27 (x15)\n" 
			"fnot.pi f16, f6\n"
			"bgeu x7, x14, 1f\n"
			"1:\n"
			"fltm.pi m5, f16, f16\n"
			"xor x12, x12, x9\n"
			"fbci.pi f9, 0x7f66c\n"
			"fle.s x6, f27, f10\n"
			"bge x10, x4, 1f\n"
			"1:\n"
			"xor x17, x22, x17\n"
			"bne x4, x23, 1f\n"
			"1:\n"
			"auipc x0, 0xa\n"
			"fand.pi f0, f21, f0\n"
			"fsub.s f16, f26, f10, rmm\n"
			"bne x27, x31, 1f\n"
			"1:\n"
			"LBL_SEQID_4_FP_AMO_ID_97_HID_0:\n"
			"li x17, 0x825b300028\n" 
			"la x9, mem_ptr_offset\n"
			"flw.ps f21, 928(x9)\n"
			"famoorl.pi f31, f21 (x17)\n" 
			"fcmovm.ps f6, f4, f4\n"
			"for.pi f0, f0, f9\n"
			"flog.ps f6, f11\n"
			"fswizz.ps f16, f4, 0xb\n"
			"fpackrepb.pi f6, f18\n"
			"fpackreph.pi f10, f15\n"
			"fsgnjn.s f16, f15, f0\n"
			"fcmovm.ps f18, f18, f18\n"
			"fcvt.pw.ps f16, f18, rtz\n"
			"LBL_SEQID_4_FP_L_SCATTER_ID_98_HID_0:\n"
			"li x5, 0x814f400770\n" 
			"la x25, mem_ptr_offset\n"
			"flw.ps f9, 152(x25)\n"
			"fschl.ps f13, f9 (x5)\n" 
			"fsub.pi f16, f26, f16\n"
			"bge x16, x1, 1f\n"
			"1:\n"
			"fslli.pi f13, f4, 0x0\n"
			"fcvt.f16.ps f10, f26\n"
			"fnmadd.ps f6, f10, f10, f13, rup\n"
			"fmaxu.pi f27, f18, f16\n"
			"bne x2, x10, 1f\n"
			"1:\n"
			"flt.pi f6, f13, f10\n"
			"fcmovm.ps f18, f26, f21\n"
			"blt x22, x30, 1f\n"
			"1:\n"
			"fcvt.ps.pwu f6, f4, rdn\n"
			"fround.ps f18, f21, rdn\n"
			"LBL_SEQID_4_INT_AMO_ID_99_HID_0:\n"
			"li x22, 0x8155de0018\n" 
			"amoandl.d x5, x16, (x22)\n" 
			"fmax.pi f26, f19, f26\n"
			"fle.pi f15, f16, f16\n"
			"fnmsub.s f16, f21, f31, f16, rmm\n"
			"fcvt.ps.pwu f16, f19, rdn\n"
			"fclass.ps f16, f16\n"
			"fsrai.pi f10, f18, 0x4\n"
			"flt.ps f16, f16, f16\n"
			"fsin.ps f26, f31\n"
			"fsgnj.s f10, f31, f26\n"
			"bne x30, x1, 1f\n"
			"1:\n"
			"mov.m.x m0, x0, 0xFF\n"
			"li x5, 0x50\n"
			"la x31, rand_ieee754_32\n"
			"add x31, x31, x5\n"
			"flw.ps f0, 0(x31)\n"
			"flw.ps f10, 2(x31)\n"
			"flw.ps f11, 4(x31)\n"
			"flw.ps f13, 11(x31)\n"
			"flw.ps f16, 2(x31)\n"
			"flw.ps f19, 18(x31)\n"
			"flw.ps f21, 24(x31)\n"
			"flw.ps f26, 18(x31)\n"
			"flw.ps f27, 31(x31)\n"
			"flw.ps f4, 10(x31)\n"
			"flw.ps f6, 38(x31)\n"
			"flw.ps f9, 16(x31)\n"
			"li x21, 0x2\n"
			"LBL_SEQID_4_LOOP_SEQ_HID_0:\n"
			"fle.pi f27, f13, f4\n"
			"bgeu x12, x18, 1f\n"
			"1:\n"
			"fsat8.pi f21, f4\n"
			"beq x17, x2, 1f\n"
			"1:\n"
			"fnmsub.s f21, f19, f26, f10, rdn\n"
			"fmin.s f21, f13, f21\n"
			"fmaxu.pi f26, f21, f4\n"
			"fexp.ps f27, f21\n"
			"ffrc.ps f19, f11\n"
			"fcvt.ps.f16 f18, f0\n"
			"fcvt.ps.f16 f15, f0\n"
			"fslli.pi f4, f21, 0x8\n"
			"blt x22, x6, 1f\n"
			"1:\n"
			"fmin.s f9, f4, f19\n"
			"bgeu x9, x21, 1f\n"
			"1:\n"
			"fadd.pi f6, f21, f6\n"
			"fpackreph.pi f26, f11\n"
			"blt x10, x1, 1f\n"
			"1:\n"
			"flt.pi f18, f26, f0\n"
			"fand.pi f6, f6, f9\n"
			"bgeu x19, x19, 1f\n"
			"1:\n"
			"fsub.pi f21, f10, f21\n"
			"blt x28, x27, 1f\n"
			"1:\n"
			"flt.pi f4, f4, f10\n"
			"fmin.s f15, f21, f21\n"
			"fsrl.pi f6, f4, f4\n"
			"fmsub.ps f9, f26, f21, f26, rne\n"
			"fmin.pi f6, f13, f9\n"
			"fsgnjx.s f13, f26, f10\n"
			"bltu x28, x31, 1f\n"
			"1:\n"
			"fmaxu.pi f9, f16, f9\n"
			"fnmadd.ps f16, f16, f16, f9, rne\n"
			"beq x18, x16, 1f\n"
			"1:\n"
			"fminu.pi f26, f4, f10\n"
			"flog.ps f21, f0\n"
			"fmul.s f19, f9, f19, rdn\n"
			"beq x26, x4, 1f\n"
			"1:\n"
			"fsrli.pi f15, f19, 0xa\n"
			"fexp.ps f15, f26\n"
			"fsra.pi f18, f4, f26\n"
			"frsq.ps f9, f10\n"
			"flog.ps f4, f4\n"
			"fmin.pi f0, f6, f6\n"
			"fmaxu.pi f4, f21, f4\n"
			"fround.ps f0, f11, rmm\n"
			"fcvt.f16.ps f26, f4\n"
			"fmulhu.pi f15, f26, f6\n"
			"blt x5, x1, 1f\n"
			"1:\n"
			"fclass.ps f9, f6\n"
			"fmin.s f11, f0, f10\n"
			"fslli.pi f26, f9, 0x9\n"
			"fand.pi f21, f0, f27\n"
			"fadd.pi f18, f0, f9\n"
			"fadd.s f4, f11, f26, rmm\n"
			"fle.ps f6, f0, f11\n"
			"fmulhu.pi f18, f16, f13\n"
			"fltu.pi f26, f27, f26\n"
			"beq x22, x7, 1f\n"
			"1:\n"
			"fand.pi f26, f16, f27\n"
			"fmax.pi f11, f9, f13\n"
			"fandi.pi f27, f19, 0xe6\n"
			"fmul.ps f19, f10, f6, rmm\n"
			"feq.ps f6, f26, f6\n"
			"fcmovm.ps f18, f9, f9\n"
			"feq.pi f9, f11, f13\n"
			"fsin.ps f6, f19\n"
			"fsll.pi f26, f19, f21\n"
			"frsq.ps f10, f10\n"
			"fsub.pi f19, f6, f9\n"
			"fpackreph.pi f21, f13\n"
			"flt.ps f16, f19, f16\n"
			"fltu.pi f9, f16, f16\n"
			"feq.ps f16, f27, f11\n"
			"ffrc.ps f0, f19\n"
			"fsgnj.ps f27, f21, f27\n"
			"fmul.s f4, f13, f6, rne\n"
			"fcvt.pwu.ps f0, f6, rmm\n"
			"fclass.ps f6, f21\n"
			"fsatu8.pi f18, f16\n"
			"fle.ps f19, f6, f11\n"
			"fltu.pi f18, f27, f9\n"
			"blt x22, x7, 1f\n"
			"1:\n"
			"fpackreph.pi f10, f10\n"
			"fsub.ps f26, f27, f4, rup\n"
			"bgeu x10, x16, 1f\n"
			"1:\n"
			"fmin.s f27, f9, f0\n"
			"frcp.ps f21, f6\n"
			"frsq.ps f6, f4\n"
			"bge x5, x30, 1f\n"
			"1:\n"
			"fadd.pi f4, f0, f13\n"
			"beq x6, x28, 1f\n"
			"1:\n"
			"frsq.ps f0, f21\n"
			"fle.ps f26, f11, f11\n"
			"fltu.pi f21, f10, f9\n"
			"fmulhu.pi f19, f10, f19\n"
			"fcvt.pwu.ps f19, f27, rmm\n"
			"fcvt.f16.ps f19, f4\n"
			"fmulh.pi f16, f19, f16\n"
			"fmax.ps f10, f19, f10\n"
			"fmsub.s f27, f27, f27, f21, rne\n"
			"fpackrepb.pi f4, f21\n"
			"fsgnj.ps f0, f26, f11\n"
			"fnmsub.ps f26, f21, f4, f21, rmm\n"
			"fsrl.pi f19, f19, f16\n"
			"fmadd.s f0, f0, f27, f11, rmm\n"
			"bltu x2, x24, 1f\n"
			"1:\n"
			"fround.ps f0, f6, rtz\n"
			"frcp.ps f19, f6\n"
			"fnmsub.s f4, f21, f0, f21, rmm\n"
			"flt.pi f10, f13, f10\n"
			"feq.ps f9, f16, f4\n"
			"fmax.s f10, f16, f10\n"
			"fnmadd.s f16, f16, f16, f26, rne\n"
			"fle.ps f13, f13, f26\n"
			"fmax.ps f13, f19, f27\n"
			"fsub.pi f6, f10, f19\n"
			"flog.ps f27, f10\n"
			"fsgnjx.ps f4, f13, f21\n"
			"fclass.ps f10, f10\n"
			"fcvt.f16.ps f4, f0\n"
			"ffrc.ps f15, f13\n"
			"bltu x4, x20, 1f\n"
			"1:\n"
			"fmax.ps f27, f27, f27\n"
			"fsub.s f15, f11, f11, rup\n"
			"bne x23, x14, 1f\n"
			"1:\n"
			"fcvt.f16.ps f27, f6\n"
			"fmul.ps f21, f19, f27, rdn\n"
			"fsgnjn.ps f18, f10, f19\n"
			"fswizz.ps f9, f10, 0x17\n"
			"bltu x21, x12, 1f\n"
			"1:\n"
			"fadd.s f16, f10, f19, rup\n"
			"beq x26, x4, 1f\n"
			"1:\n"
			"feq.pi f19, f13, f13\n"
			"fsub.ps f18, f16, f27, rdn\n"
			"fcvt.ps.pw f21, f6, rup\n"
			"fsub.pi f21, f11, f10\n"
			"fxor.pi f4, f13, f19\n"
			"bgeu x7, x23, 1f\n"
			"1:\n"
			"fmin.pi f13, f21, f10\n"
			"fcvt.ps.f16 f26, f10\n"
			"fsrli.pi f21, f11, 0x7\n"
			"feq.ps f19, f27, f13\n"
			"feq.ps f15, f27, f16\n"
			"fmul.s f13, f13, f21, rne\n"
			"fsgnjx.ps f4, f9, f4\n"
			"fmax.pi f11, f26, f11\n"
			"bge x4, x22, 1f\n"
			"1:\n"
			"fadd.ps f27, f13, f9, rne\n"
			"bne x27, x4, 1f\n"
			"1:\n"
			"fsgnjn.ps f27, f10, f27\n"
			"fmin.s f16, f16, f21\n"
			"fsgnjx.s f4, f11, f0\n"
			"blt x27, x6, 1f\n"
			"1:\n"
			"feq.pi f13, f13, f13\n"
			"feq.pi f26, f26, f27\n"
			"faddi.pi f19, f4, 0x71\n"
			"bge x2, x7, 1f\n"
			"1:\n"
			"feq.ps f11, f4, f11\n"
			"flt.ps f27, f13, f10\n"
			"bgeu x17, x5, 1f\n"
			"1:\n"
			"flog.ps f4, f16\n"
			"fnmsub.s f10, f10, f6, f9, rdn\n"
			"ffrc.ps f9, f11\n"
			"bge x24, x23, 1f\n"
			"1:\n"
			"ffrc.ps f19, f19\n"
			"feq.pi f9, f26, f16\n"
			"fxor.pi f18, f27, f27\n"
			"fcvt.f16.ps f15, f0\n"
			"fsat8.pi f19, f27\n"
			"fcvt.ps.f16 f6, f26\n"
			"fsgnj.ps f9, f9, f21\n"
			"fmsub.s f21, f27, f16, f4, rup\n"
			"fround.ps f15, f6, rmm\n"
			"blt x25, x19, 1f\n"
			"1:\n"
			"fsatu8.pi f15, f16\n"
			"fltu.pi f9, f9, f13\n"
			"fsgnj.ps f21, f19, f19\n"
			"fmin.s f6, f13, f16\n"
			"fmin.ps f18, f9, f10\n"
			"fmsub.s f27, f19, f0, f26, rmm\n"
			"bne x2, x27, 1f\n"
			"1:\n"
			"faddi.pi f0, f4, 0x77\n"
			"fsub.s f15, f0, f10, rtz\n"
			"fnmsub.s f19, f19, f26, f16, rne\n"
			"fadd.s f6, f6, f9, rmm\n"
			"bne x16, x23, 1f\n"
			"1:\n"
			"fmaxu.pi f4, f13, f4\n"
			"fmaxu.pi f27, f26, f0\n"
			"fclass.ps f4, f19\n"
			"fround.ps f6, f11, rtz\n"
			"fexp.ps f4, f11\n"
			"fadd.pi f19, f26, f0\n"
			"fsgnjn.ps f11, f11, f11\n"
			"fsrai.pi f0, f9, 0x7\n"
			"fcmovm.ps f21, f6, f26\n"
			"fmin.s f0, f4, f0\n"
			"fcvt.ps.f16 f10, f9\n"
			"flt.ps f4, f13, f21\n"
			"bltu x15, x21, 1f\n"
			"1:\n"
			"fsll.pi f9, f0, f13\n"
			"flt.pi f26, f9, f26\n"
			"fmax.s f0, f21, f0\n"
			"fnot.pi f19, f13\n"
			"fcvt.ps.pw f21, f26, rdn\n"
			"fcvt.ps.pw f4, f26, rne\n"
			"fcvt.ps.pw f13, f6, rtz\n"
			"fround.ps f18, f11, rne\n"
			"fsatu8.pi f9, f11\n"
			"fnmsub.ps f16, f16, f21, f26, rup\n"
			"fnmadd.s f13, f13, f16, f11, rup\n"
			"blt x11, x31, 1f\n"
			"1:\n"
			"fle.ps f26, f27, f9\n"
			"fsub.s f6, f9, f13, rup\n"
			"bge x8, x14, 1f\n"
			"1:\n"
			"fcmovm.ps f19, f4, f4\n"
			"faddi.pi f0, f16, 0x1e8\n"
			"fmaxu.pi f10, f27, f27\n"
			"blt x10, x22, 1f\n"
			"1:\n"
			"fcmovm.ps f4, f13, f9\n"
			"fltu.pi f19, f21, f6\n"
			"bltu x7, x27, 1f\n"
			"1:\n"
			"fminu.pi f27, f4, f11\n"
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
			"flw.ps f12,  800 (x5)\n"
			"flw.ps f20,  352 (x5)\n"
			"flw.ps f14,  192 (x5)\n"
			"flw.ps f10,  256 (x5)\n"
			"flw.ps f21,  128 (x5)\n"
			"flw.ps f17,  448 (x5)\n"
			"flw.ps f26,  928 (x5)\n"
			"flw.ps f19,  160 (x5)\n"
			"flw.ps f0,  288 (x5)\n"
			"flw.ps f23,  64 (x5)\n"
			"flw.ps f28,  864 (x5)\n"
			"flw.ps f13,  320 (x5)\n"
			"flw.ps f16,  960 (x5)\n"
			"flw.ps f6,  992 (x5)\n"
			"flw.ps f18,  544 (x5)\n"
			"flw.ps f29,  32 (x5)\n"
			"flw.ps f31,  704 (x5)\n"
			"flw.ps f15,  224 (x5)\n"
			"flw.ps f27,  608 (x5)\n"
			"flw.ps f3,  0 (x5)\n"
			"flw.ps f22,  768 (x5)\n"
			"flw.ps f2,  736 (x5)\n"
			"flw.ps f24,  384 (x5)\n"
			"flw.ps f8,  640 (x5)\n"
			"flw.ps f4,  416 (x5)\n"
			"flw.ps f9,  480 (x5)\n"
			"flw.ps f7,  832 (x5)\n"
			"flw.ps f25,  512 (x5)\n"
			"flw.ps f5,  96 (x5)\n"
			"flw.ps f1,  896 (x5)\n"
			"flw.ps f11,  672 (x5)\n"
			"flw.ps f30,  576 (x5)\n"
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
			"LBL_HPM_3_CORE_TL_INST_FAIL_HID_0:\n"          
			"csrr x5, mhpmcounter3\n"
			"sd x5, 32 (x31)\n"
			"LBL_HPM_4_CORE_TREDUCE_INST_FAIL_HID_0:\n"          
			"csrr x5, mhpmcounter4\n"
			"sd x5, 40 (x31)\n"
			"LBL_HPM_5_CORE_TXFMA_INT_OPS_FAIL_HID_0:\n"          
			"csrr x5, mhpmcounter5\n"
			"sd x5, 48 (x31)\n"
			"LBL_HPM_6_CORE_TXFMA_OPS32_FAIL_HID_0:\n"          
			"csrr x5, mhpmcounter6\n"
			"sd x5, 56 (x31)\n"
			"LBL_HPM_7_NEIGH_RX_COOP_TLD_RSP_FAIL_HID_0:\n"          
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
			"LBL_HPM_3_CORE_TL_INST_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter3\n"
			"sd x5, 32 (x31)\n"
			"LBL_HPM_4_CORE_TREDUCE_INST_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter4\n"
			"sd x5, 40 (x31)\n"
			"LBL_HPM_5_CORE_TXFMA_INT_OPS_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter5\n"
			"sd x5, 48 (x31)\n"
			"LBL_HPM_6_CORE_TXFMA_OPS32_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter6\n"
			"sd x5, 56 (x31)\n"
			"LBL_HPM_7_NEIGH_RX_COOP_TLD_RSP_PASS_HID_0:\n"          
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
		0x8367992c, 
		0xfceb1955, 
		0x5f91e9d8, 
		0x1474f29e, 
		0xdba0bd23, 
		0xcf559fc3, 
		0x42bd8acd, 
		0xce2a2d89, 
		0x3ffddfd5, 
		0x92e915a3, 
		0xab1461fc, 
		0xf292e549, 
		0x43cf0d66, 
		0x59dd1b5c, 
		0x3508cb5c, 
		0x9c6d71dc, 
		0x50ec9d4e, 
		0x6fe63a2c, 
		0x1c0a6b04, 
		0x707c1af1, 
		0xc767cd1e, 
		0x9f77c058, 
		0xca66e65e, 
		0xca1b566a, 
		0x24ccc368, 
		0xb036f827, 
		0x0552c057, 
		0xd1724c7a, 
		0x0f6a7538, 
		0xf3949dda, 
		0xcf8d5e23, 
		0x63e4ad13, 
		0xa1974824, 
		0x6c3413c0, 
		0x86c25c6e, 
		0x4518357f, 
		0xd05ab9ad, 
		0xc1f6e92a, 
		0x0084434e, 
		0x9ad3e0a6, 
		0x29bbf296, 
		0x848e803d, 
		0xd4d9dd72, 
		0x1824e0b7, 
		0x9155c910, 
		0x08009d27, 
		0x85c3b00a, 
		0x0bc7ced7, 
		0x21d310fd, 
		0xb9ba13ca, 
		0xa99a059d, 
		0x3afc5fe1, 
		0x1481b39b, 
		0xd543023f, 
		0xbf8b9bb7, 
		0x51ee971f, 
		0xc803ef89, 
		0x8c12e758, 
		0x00caa834, 
		0x3005d858, 
		0x829b9cc7, 
		0xbd66ee5d, 
		0x904c8ddb, 
		0x1f9a3b4d, 
		0x164b3df0, 
		0x2549db9b, 
		0xe5de935b, 
		0x0c4c9f5e, 
		0x784d2f7f, 
		0x1d4c8729, 
		0x3add656a, 
		0x3eff596a, 
		0xae550f32, 
		0x139fae55, 
		0x3ad1785c, 
		0x39175626, 
		0x9ac8e175, 
		0x6aba812d, 
		0x4985ed40, 
		0xbdd98a87, 
		0xa3050231, 
		0x8fe768c1, 
		0x1d45b331, 
		0x19885556, 
		0x8d538ce3, 
		0x0f80dc65, 
		0x5af32176, 
		0xd5df92ab, 
		0x27554e10, 
		0x69fdade5, 
		0x690b1b5d, 
		0x7398a5a2, 
		0x5149256a, 
		0x8640ae93, 
		0xff3fc057, 
		0x8057922a, 
		0xb7de1757, 
		0x965726a0, 
		0x53da6200, 
		0x5cf19af6, 
		0x99151cea, 
		0x75944571, 
		0xe989b509, 
		0xe80b1dc8, 
		0x31c09def, 
		0xd10b112a, 
		0x71c8361f, 
		0xdd8af23a, 
		0xa5f15afc, 
		0xcd83be8c, 
		0x146afd7b, 
		0x5a378035, 
		0x4e9b841a, 
		0x66f2b0c4, 
		0x65e60786, 
		0xd25eb786, 
		0x1bf7277a, 
		0xdf74f38a, 
		0xdb51e55e, 
		0x758da1fa, 
		0xeca7e95f, 
		0xf154fce8, 
		0x97f2405d, 
		0xa25719ca, 
		0x35866990, 
		0x5a331022, 
		0x4fc3ec23, 
		0x9db2ebc6, 
		0x98b12633, 
		0xf268e2b4, 
		0x6a6e0eb9, 
		0xe1facdcc, 
		0x195eed15, 
		0xf75b543a, 
		0x05af5f58, 
		0xb4d557e6, 
		0xaf94f82a, 
		0x543d3416, 
		0x9ad2a458, 
		0xf830ea73, 
		0xa00e7eeb, 
		0x3eb1cdff, 
		0xcd06848f, 
		0x8096bb13, 
		0xaf57d83b, 
		0xb9a69a16, 
		0x80d34f4f, 
		0x36e6f32e, 
		0x67965190, 
		0x44d17e8b, 
		0xff4fb0fd, 
		0x31fcfde3, 
		0x887cc8f7, 
		0x714de09c, 
		0xdc88d7de, 
		0x741ca833, 
		0x889923e6, 
		0xc2852e99, 
		0x3c003c88, 
		0xc7d8e0b3, 
		0x807521e7, 
		0x3e59ddaa, 
		0x85abfb82, 
		0xe1e31eaa, 
		0x0951454f, 
		0xfef35e8a, 
		0x47d3f880, 
		0xd9f7f1ed, 
		0xd466bca8, 
		0xbbd6583a, 
		0xf32b5245, 
		0xcfc35fd7, 
		0x194b046b, 
		0xac9f2e7d, 
		0xfc7507d5, 
		0xa330f3b4, 
		0xd6e93b47, 
		0xc66df070, 
		0x5b14d332, 
		0xda209e27, 
		0x5052954a, 
		0x04146552, 
		0x147104ee, 
		0xf83b06b3, 
		0x9e48270c, 
		0x7469f1aa, 
		0x050bd132, 
		0x6efb283a, 
		0xce1a2f54, 
		0x8e2ffacb, 
		0xb018b3b1, 
		0xe622d19e, 
		0x72f7afb1, 
		0xfc74bdf3, 
		0x81a12540, 
		0x7f6afeb9, 
		0x9c7b9796, 
		0x2347df3d, 
		0xc75922c4, 
		0xcbd271d1, 
		0x3e867867, 
		0x27ee8bbc, 
		0xcf87b367, 
		0x50e8b023, 
		0x634dbb60, 
		0x51fa83fd, 
		0x710f64f3, 
		0xb662e850, 
		0x77c99f96, 
		0x89587227, 
		0xb2ab4ad2, 
		0x949369a4, 
		0xf14999a6, 
		0xa5de810d, 
		0xc81bf224, 
		0x1bb59243, 
		0xdfb5ad5b, 
		0x293322f1, 
		0x33214cda, 
		0xbb18d150, 
		0xb591aff1, 
		0x8fb70aab, 
		0x2729830c, 
		0x2e6b64af, 
		0xa96b7817, 
		0x8ee53b01, 
		0x9d5bb6fb, 
		0x385c9900, 
		0xc7bc6bbe, 
		0xe6df2067, 
		0x2547872c, 
		0x29feb9dc, 
		0xc4e531b5, 
		0xeab9a237, 
		0xe7866ffe, 
		0x530be2c6, 
		0x69ba1cd3, 
		0xdaf6b326, 
		0x2da9548a, 
		0x8e628fa7, 
		0x62d84f13, 
		0xc5481786, 
		0x541fca7b, 
		0xdcb75cf5, 
		0x5d7803af, 
		0x76cbc6f0, 
		0x2455072c, 
		0xc3ad0dee, 
		0xe7f342ac, 
		0x4e104cce, 
		0xf472e47d, 
		0x9aca138a, 
		0x881cf9a1, 
		0x7c39504d, 
		0xdd218560, 
		0x976f17d9  
};
volatile uint32_t rand_ieee754_32[1536] __attribute__ ((aligned (2048))) = {
		0x00000000, 
		0xce5b35fd, 
		0x80800001, 
		0x41a00000, 
		0x41000000, 
		0xf77f72a4, 
		0x7f7fffff, 
		0x41f80000, 
		0xff94124b, 
		0xfc172411, 
		0xdfd53b2a, 
		0x41880000, 
		0x597f3a9b, 
		0xc1980000, 
		0x0c7ffff0, 
		0x7f92fc28, 
		0x0e0003ff, 
		0x7fae39a2, 
		0x80011111, 
		0xc2ed891e, 
		0x00800002, 
		0x8062c40f, 
		0x80000002, 
		0x0e000003, 
		0xc2740000, 
		0xc6e51565, 
		0x7fc00000, 
		0xfc712c8e, 
		0x00000000, 
		0xff800000, 
		0xff800000, 
		0xbe3f6169, 
		0x621b6e08, 
		0x00040000, 
		0x1e8e9476, 
		0x0c7ffff8, 
		0x80ffffff, 
		0x00000008, 
		0x7933ee8d, 
		0x0e00000f, 
		0x00000000, 
		0x7f800000, 
		0xff800000, 
		0xe03e085c, 
		0xdbebc825, 
		0x42300000, 
		0xc2680000, 
		0x0e000001, 
		0x41900000, 
		0x3f3ad680, 
		0x0c7ffff8, 
		0xd6e6c80d, 
		0xd29a6ddc, 
		0x0e1fffff, 
		0x41d80000, 
		0xd6cd9937, 
		0x80040000, 
		0x41d80000, 
		0x33333333, 
		0xc2540000, 
		0xc1e80000, 
		0x00000000, 
		0x0d00fff0, 
		0x29ecf8d0, 
		0x0e00007f, 
		0xc2340000, 
		0x40c00000, 
		0x00000000, 
		0x41300000, 
		0x41000000, 
		0xc1c80000, 
		0xcddfeb09, 
		0x00800001, 
		0xff7ffffe, 
		0x41e80000, 
		0x00800003, 
		0xff800000, 
		0xff7ffffe, 
		0xc1100000, 
		0x7f7ffffe, 
		0xc2000000, 
		0x42080000, 
		0x00000000, 
		0xa9fe623d, 
		0x421c0000, 
		0x42200000, 
		0xc2180000, 
		0x00800002, 
		0x3f028f5c, 
		0xa173391c, 
		0x00000008, 
		0x80000100, 
		0x7f800000, 
		0xc1b00000, 
		0xc1a80000, 
		0x00020000, 
		0x7f7fffff, 
		0x9019b333, 
		0x00800003, 
		0x80800002, 
		0x41600000, 
		0x0c7ff000, 
		0xc2780000, 
		0x80000000, 
		0x424c0000, 
		0x80000008, 
		0x954e7a5e, 
		0xd65e057c, 
		0x0f7fffff, 
		0x0c7ffffe, 
		0x2bc3e1f6, 
		0x80000020, 
		0x42180000, 
		0x00000000, 
		0x7fc3ab63, 
		0x00000001, 
		0x00000000, 
		0x80000000, 
		0x00000000, 
		0x7f7ffffe, 
		0xc1e00000, 
		0x00000001, 
		0xff800000, 
		0xc25c0000, 
		0x7bfb60f4, 
		0x80000000, 
		0x80000000, 
		0x6d0d930d, 
		0x459457c1, 
		0xc1d00000, 
		0x80000000, 
		0x354488ca, 
		0xe44f27e4, 
		0x39a079ef, 
		0x41300000, 
		0x80000000, 
		0x00000200, 
		0x77337b4d, 
		0xc1880000, 
		0xc24c0000, 
		0x0c7c0000, 
		0x80000000, 
		0x44c6862b, 
		0x0e7fffff, 
		0x0062b0b9, 
		0xc2300000, 
		0x0e003fff, 
		0x41d00000, 
		0xff7ffffe, 
		0x421c0000, 
		0x903b3f46, 
		0x80010000, 
		0x00000000, 
		0xff8be2d4, 
		0x80000080, 
		0x00000040, 
		0x3000cd91, 
		0x00000000, 
		0x710b9452, 
		0x0c7fff00, 
		0xf38b7db3, 
		0xc26f603f, 
		0x427c0000, 
		0x6c77ff43, 
		0x80800001, 
		0x7e82ce61, 
		0x76446c05, 
		0x80008000, 
		0x8f7ffffd, 
		0x0e0001ff, 
		0x7f7fffff, 
		0x80000000, 
		0xc1c80000, 
		0xc1100000, 
		0x7c16e3c5, 
		0x81155982, 
		0x80004000, 
		0x7fc00001, 
		0xff800000, 
		0xff800000, 
		0x7fa0e9ed, 
		0x00000004, 
		0x00000200, 
		0x7fc00001, 
		0x41000000, 
		0x2e178c70, 
		0x80000000, 
		0x7f000000, 
		0x9880283c, 
		0x3f800000, 
		0x41880000, 
		0x565b5145, 
		0x42300000, 
		0x80010000, 
		0x42700000, 
		0x01cdb906, 
		0x0d000ff0, 
		0xc1800000, 
		0x424c0000, 
		0x80000000, 
		0x00040000, 
		0x7f9106a4, 
		0xa9190537, 
		0x7f000000, 
		0xc1980000, 
		0x42680000, 
		0xebb196ae, 
		0x14113d23, 
		0xc0a00000, 
		0xc1c00000, 
		0x00020000, 
		0x0c7ffffe, 
		0x59609426, 
		0x8f7ffffe, 
		0x0e3fffff, 
		0x00000000, 
		0x66379ad9, 
		0x40e94224, 
		0xc26c0000, 
		0x42140000, 
		0x41c80000, 
		0x42000000, 
		0x41200000, 
		0x4e5d485c, 
		0x00000000, 
		0x3f800000, 
		0x98044ad6, 
		0x27581390, 
		0x00000020, 
		0x80100000, 
		0x00000000, 
		0x0c7ffff8, 
		0x42380000, 
		0xd03812e8, 
		0xc1b80000, 
		0x20abe43c, 
		0xc2600000, 
		0x00800000, 
		0x42100000, 
		0xc1000000, 
		0xc1a80000, 
		0x0c7fc000, 
		0x7f7fffff, 
		0x5e839330, 
		0x7fd74957, 
		0xcb8c4b40, 
		0x0c7ffffc, 
		0x20f727ca, 
		0x80000100, 
		0xdd4bc01f, 
		0x42400000, 
		0xc2200000, 
		0xcb8c4b40, 
		0x80800000, 
		0x71a304b3, 
		0x42040000, 
		0x7f7ffffe, 
		0x42380000, 
		0x80000800, 
		0x93c96e12, 
		0x334534b8, 
		0xea1a19e0, 
		0x0e007fff, 
		0xa937382c, 
		0x73cd0415, 
		0x807fffff, 
		0x00800002, 
		0xc0c00000, 
		0x00000000, 
		0x0c400000, 
		0xd09b9566, 
		0x50ba904e, 
		0xc2080000, 
		0xeef0357a, 
		0x0c7fffc0, 
		0x41f80000, 
		0x281e4d88, 
		0x421c0000, 
		0x007ffffe, 
		0x33333333, 
		0xc0c00000, 
		0x8f7ffffe, 
		0x7f7fffff, 
		0x42600000, 
		0x4332988a, 
		0xff800000, 
		0x0d000ff0, 
		0xc2580000, 
		0x58a66322, 
		0x42580000, 
		0x3d76b30b, 
		0x41d00000, 
		0xdde0fb35, 
		0x00000040, 
		0x0e0001ff, 
		0xbf028f5c, 
		0xff800000, 
		0x1feaa9d0, 
		0x80000000, 
		0x00000000, 
		0x42580000, 
		0xc1f00000, 
		0x41e00000, 
		0x3f028f5c, 
		0xc0400000, 
		0x00008000, 
		0xc2780000, 
		0x0e000003, 
		0x42700000, 
		0xc2240000, 
		0x00000008, 
		0x0e000fff, 
		0x86cb5e6a, 
		0x51bdc160, 
		0x7f800000, 
		0x80800002, 
		0x80000100, 
		0x41500000, 
		0x42300000, 
		0xee0df93c, 
		0x0c7ffffc, 
		0x0e000fff, 
		0x00000100, 
		0x0f7ffffc, 
		0x00000100, 
		0x7f8cad78, 
		0x007ffffe, 
		0x00f6854f, 
		0x32360dcd, 
		0x0e0003ff, 
		0xc1e00000, 
		0xc2700000, 
		0x00800002, 
		0x427c0000, 
		0xc1700000, 
		0x0c7ffffe, 
		0x694748a3, 
		0x424c0000, 
		0xc2700000, 
		0x0e3fffff, 
		0x80000000, 
		0x0c7f8000, 
		0x00800002, 
		0x0e00ffff, 
		0x7573f779, 
		0xff800000, 
		0x00000000, 
		0x72341d53, 
		0x8fe263ee, 
		0x0013f40f, 
		0xc0400000, 
		0x0fa2a699, 
		0x00000400, 
		0xc1a00000, 
		0x47fa05e2, 
		0x00800001, 
		0x80001000, 
		0xed3109df, 
		0x82b6a4d7, 
		0x80000000, 
		0x24137780, 
		0x7022a110, 
		0x2d7280c4, 
		0xff7ffffe, 
		0x0c7fffff, 
		0xc2240000, 
		0x80000002, 
		0x7914b5a4, 
		0x8e1fe202, 
		0x007fffff, 
		0x80000200, 
		0x00000800, 
		0x00008000, 
		0x0e0003ff, 
		0x1b070190, 
		0x6b4e1db8, 
		0x961f128d, 
		0x42380000, 
		0xc1c80000, 
		0xff800000, 
		0x0e00001f, 
		0x7f800000, 
		0x0c7ff800, 
		0x8f7ffffe, 
		0xc1f00000, 
		0x9186d5e4, 
		0x42100000, 
		0xc26c0000, 
		0x00000000, 
		0x0e07ffff, 
		0x42200000, 
		0x3f3689e6, 
		0x8af77f7f, 
		0xbf028f5c, 
		0x2a6f40e3, 
		0x00000400, 
		0xff800001, 
		0x2845754f, 
		0xc2700000, 
		0x7f800000, 
		0x0e01ffff, 
		0x41000000, 
		0x42280000, 
		0x234dd880, 
		0xc1f00000, 
		0x00000000, 
		0x80000000, 
		0x4b8c4b40, 
		0x5b9dc33f, 
		0x80ffffff, 
		0x909255e5, 
		0xc2580000, 
		0x00080000, 
		0x80000800, 
		0x42740000, 
		0x00000040, 
		0x40e00000, 
		0x0e0fffff, 
		0x41500000, 
		0x8f7ffffc, 
		0x42280000, 
		0xcb000000, 
		0x41500000, 
		0x559b59e0, 
		0x0c7ff000, 
		0x41e00000, 
		0x40c00000, 
		0x7592245d, 
		0x269f1925, 
		0x75c07974, 
		0xf7a92ea3, 
		0x3ab61964, 
		0xc1d00000, 
		0xc1300000, 
		0x0f7fffff, 
		0xb5f3ea9b, 
		0xff000000, 
		0xc2680000, 
		0x00000000, 
		0xffade684, 
		0x80000000, 
		0x80080000, 
		0x00000000, 
		0x0493805e, 
		0x80000000, 
		0x80000000, 
		0x0e01ffff, 
		0x7f7ffffe, 
		0x38a5c582, 
		0x41200000, 
		0x00000000, 
		0x00000002, 
		0xff800000, 
		0xf52a5925, 
		0x422c0000, 
		0x0c7ffffc, 
		0x0c7fff80, 
		0x80000000, 
		0x007fffff, 
		0x00000000, 
		0x41100000, 
		0x80000001, 
		0x5ef826f7, 
		0xc21c0000, 
		0xc1329c1f, 
		0xf96fd525, 
		0xaaaaaaaa, 
		0x00004000, 
		0x0f7ffffd, 
		0x37b5fb63, 
		0xc2340000, 
		0xc2300000, 
		0x0e000001, 
		0xd7606661, 
		0xc1f80000, 
		0xc2540000, 
		0x42140000, 
		0xbcee6341, 
		0xc1980000, 
		0x0e0fffff, 
		0xffa2871e, 
		0x3cbcd230, 
		0x80000040, 
		0xff7ffffe, 
		0x00000004, 
		0x6d6812e7, 
		0xc26c0000, 
		0x0fd53b32, 
		0xc1000000, 
		0x00040000, 
		0x0fcef23e, 
		0x0e001fff, 
		0xc6be0945, 
		0x80000001, 
		0xc20c0000, 
		0x80011111, 
		0xb5544dfe, 
		0x0d00fff0, 
		0x0c700000, 
		0xdeab4e61, 
		0x80800000, 
		0x00000000, 
		0xc2219d65, 
		0x41a80000, 
		0x00000000, 
		0xcb8c4b40, 
		0x41700000, 
		0x41d80000, 
		0xc1300000, 
		0x3f800001, 
		0xc8b7a7b9, 
		0x00010000, 
		0x3eb584d2, 
		0x7f7ffffe, 
		0xc2180000, 
		0x41f80000, 
		0x1a59d692, 
		0x80000000, 
		0x6dc9f462, 
		0x00000000, 
		0x0e0fffff, 
		0x00000000, 
		0x405019ec, 
		0x80000040, 
		0xb849adca, 
		0x14491bb0, 
		0x0e0003ff, 
		0xeb7beb73, 
		0x40400000, 
		0x80000800, 
		0x3f028f5c, 
		0x7f7ffffe, 
		0x80000000, 
		0xb5b0d4db, 
		0x4b60bfd3, 
		0x95419df6, 
		0x80800003, 
		0xc2100000, 
		0x24268673, 
		0xc0e00000, 
		0x7f800000, 
		0x42640000, 
		0x80000001, 
		0xc6a3bfbe, 
		0xc2040000, 
		0x4a7fd63b, 
		0x0c700000, 
		0x80010000, 
		0xe292ca81, 
		0xffc00000, 
		0x41880000, 
		0x00800002, 
		0x80800001, 
		0xc1c00000, 
		0x80000002, 
		0xffc00000, 
		0xc2580000, 
		0x6fd77ce7, 
		0x80000400, 
		0x4950605b, 
		0x42200000, 
		0xea2c1e82, 
		0x5ad5b136, 
		0x80800001, 
		0xc512e9d6, 
		0xdac90c45, 
		0xc1300000, 
		0x0e07ffff, 
		0xbf028f5c, 
		0x80000200, 
		0x80000800, 
		0x80200000, 
		0x7f9d3425, 
		0x426c0000, 
		0xc0800000, 
		0x80000040, 
		0xc1c80000, 
		0x3f028f5c, 
		0xc2540000, 
		0x7fcaf67e, 
		0xff609a76, 
		0x76797c0c, 
		0xffbfffff, 
		0x00010000, 
		0x40800000, 
		0x7f000000, 
		0xe9317f9f, 
		0x00000100, 
		0x0e00003f, 
		0x80ffffff, 
		0x00000000, 
		0xff800000, 
		0x0e1fffff, 
		0x7f990f2a, 
		0x83caeab1, 
		0xc2140000, 
		0xae945e48, 
		0x1b4244bb, 
		0x00000040, 
		0x80000000, 
		0x00008000, 
		0xff800000, 
		0xaaaaaaaa, 
		0x0e00000f, 
		0x00008000, 
		0x37d6ee34, 
		0xc2740000, 
		0xc1f00000, 
		0x41f00000, 
		0x7f983c80, 
		0x00800000, 
		0x42780000, 
		0xf6f34994, 
		0xc21c0000, 
		0x00000000, 
		0xcabe0587, 
		0x46e1089c, 
		0xc2000000, 
		0x42100000, 
		0x007fffff, 
		0x4e07148a, 
		0xb5a69d08, 
		0x0c7f8000, 
		0x807a48bb, 
		0x14d70da0, 
		0x007fffff, 
		0x3f68f5a4, 
		0x90cb6f7c, 
		0x9ec628bf, 
		0x00800003, 
		0x80000000, 
		0x00800000, 
		0xe47ff9ce, 
		0x80000000, 
		0x00000000, 
		0x00040000, 
		0x80000001, 
		0x49ce843e, 
		0x0c7fff00, 
		0x00000000, 
		0x7fc1833b, 
		0x40400000, 
		0x80010000, 
		0xff85fae1, 
		0x0e00ffff, 
		0xc2340000, 
		0x18ab50d3, 
		0xffc00000, 
		0x7f800000, 
		0x0c600000, 
		0x0c7ffc00, 
		0x80000800, 
		0x00010000, 
		0x3f028f5c, 
		0x0e0003ff, 
		0x80000002, 
		0xc1e00000, 
		0x00000002, 
		0x0c7ff000, 
		0x42140000, 
		0x42700000, 
		0x42100000, 
		0xc1f80000, 
		0x0c600000, 
		0x41700000, 
		0x0c7ffc00, 
		0x807effe8, 
		0x41d00000, 
		0x51c03e71, 
		0x4908761f, 
		0x423c0000, 
		0xd1a36439, 
		0x0e1fffff, 
		0x3f028f5c, 
		0x7fc00000, 
		0xc2440000, 
		0x80000000, 
		0xb58d6a75, 
		0x00100000, 
		0x425c0000, 
		0x00800001, 
		0x6227c3e3, 
		0x80000000, 
		0x80800003, 
		0xc1600000, 
		0x80800003, 
		0x423c0000, 
		0xc23c0000, 
		0x40400000, 
		0x59732833, 
		0x00000010, 
		0x053ad705, 
		0xd0dd3428, 
		0x7bca3c31, 
		0xa8ca2c00, 
		0x0c780000, 
		0x40400000, 
		0x80800001, 
		0xffaecaeb, 
		0xc5a8a266, 
		0xd357c15b, 
		0xc2400000, 
		0xff7fffff, 
		0x40000000, 
		0x96f5bbd9, 
		0x41600000, 
		0x00011111, 
		0xff800000, 
		0xf1d9e982, 
		0x00800003, 
		0xc20c0000, 
		0x80000000, 
		0xc2140000, 
		0x72aa6aa3, 
		0x0c7e0000, 
		0xff970e4c, 
		0x0e00000f, 
		0x3845916a, 
		0x00c22aa2, 
		0x0e0001ff, 
		0xc2440000, 
		0x05bfd7a4, 
		0x0e03ffff, 
		0x00000001, 
		0xfc16253e, 
		0xc1900000, 
		0xff9700aa, 
		0x6208d63f, 
		0xd9c82363, 
		0x65ffae7b, 
		0xc1d80000, 
		0xc1d00000, 
		0x2ac730a8, 
		0x00000000, 
		0xa52c4a52, 
		0x80800002, 
		0xff7fffff, 
		0x0f7ffffe, 
		0x3a2cde9a, 
		0x00000001, 
		0xf94108a9, 
		0x00000000, 
		0x80010000, 
		0x0e1fffff, 
		0x80000001, 
		0x42580000, 
		0xd3a8e367, 
		0x41e00000, 
		0x80800000, 
		0x00000001, 
		0x0c700000, 
		0x0c7fc000, 
		0x0e000001, 
		0xf08fc560, 
		0x7fffffff, 
		0x42400000, 
		0xff800000, 
		0x82d280ba, 
		0x100bee71, 
		0x7ea6b43c, 
		0x0c780000, 
		0xff800000, 
		0x0c7ffc00, 
		0xc4a064ba, 
		0xff800000, 
		0xc2200000, 
		0x0e0001ff, 
		0x421c0000, 
		0x4ebb5f54, 
		0x41d80000, 
		0x0c7fffc0, 
		0x0e003fff, 
		0x00800003, 
		0xc27c0000, 
		0x7f7ffffe, 
		0x8ab6e288, 
		0x80004000, 
		0x0d00fff0, 
		0xff800000, 
		0x2d801d53, 
		0xc21c0000, 
		0x00000002, 
		0xcb000000, 
		0x1010df88, 
		0x41a00000, 
		0xff7ffffe, 
		0x7f7ffffe, 
		0x51922caf, 
		0xe43248b6, 
		0x71375784, 
		0x74f8cd1c, 
		0xc2000000, 
		0x921923e3, 
		0xcbdc1d20, 
		0x7f000000, 
		0xc0c00000, 
		0x00800001, 
		0xffbfffff, 
		0x80000080, 
		0x5d55eca5, 
		0x0c7fc000, 
		0xc2080000, 
		0x80000002, 
		0xc1700000, 
		0x80200000, 
		0xcb000000, 
		0xc2780000, 
		0x41600000, 
		0xfffa562d, 
		0x00800000, 
		0x00100000, 
		0x00800000, 
		0x00000000, 
		0x80000000, 
		0x0e03ffff, 
		0x0c7e0000, 
		0x426c0000, 
		0x00000000, 
		0x42140000, 
		0x00001000, 
		0x5611168b, 
		0x8f7fffff, 
		0x00800003, 
		0x5edc539d, 
		0x0c600000, 
		0xc2140000, 
		0x8f7ffffd, 
		0x42040000, 
		0x0e000001, 
		0x3f8e85a9, 
		0xc2400000, 
		0x3c99616d, 
		0x42680000, 
		0xad680aef, 
		0xaaaaaaaa, 
		0x038d93af, 
		0xc1980000, 
		0x80000001, 
		0xc1f80000, 
		0x41d80000, 
		0x9a0ef666, 
		0x80000000, 
		0xc1980000, 
		0x0c7fffc0, 
		0x42640000, 
		0xffffffff, 
		0x00453594, 
		0xbf028f5c, 
		0x0e07ffff, 
		0xc0000000, 
		0xa9ef90c6, 
		0x80011111, 
		0xff800000, 
		0xff800001, 
		0x33c55ea5, 
		0x7f7ffffe, 
		0xe9ff4ff2, 
		0x80001000, 
		0x80000004, 
		0xff800001, 
		0x00691ae3, 
		0x41140992, 
		0x7ff7c1e8, 
		0xff800000, 
		0x007fffff, 
		0x42680000, 
		0x00011111, 
		0x0f1d6000, 
		0xc34a56a1, 
		0x41900000, 
		0x482836ba, 
		0x66a559cf, 
		0x42540000, 
		0x41980000, 
		0x7fc00001, 
		0x01d3cef9, 
		0x42080000, 
		0x7fc00000, 
		0x53a0c950, 
		0xa3d5fb27, 
		0x7f9e7e8c, 
		0x8079fb6a, 
		0x6b378c54, 
		0x2e52647c, 
		0x7f800000, 
		0x4c07c5dd, 
		0x0c7fffc0, 
		0xb8effb3a, 
		0x86aa3db9, 
		0xc2180000, 
		0x0d000ff0, 
		0x80000000, 
		0x917ba9a7, 
		0xc23c0000, 
		0xc1200000, 
		0x41300000, 
		0x0e03ffff, 
		0xfe4b0dac, 
		0x00800002, 
		0xc2100000, 
		0x0c7fffc0, 
		0x80800001, 
		0x00008000, 
		0xc2400000, 
		0x15bd7b07, 
		0x0c600000, 
		0xace99999, 
		0xe778aac5, 
		0xff7fffff, 
		0x69e3722c, 
		0x42440000, 
		0x80000100, 
		0x4e977dfa, 
		0xbf800000, 
		0x00000800, 
		0x00000008, 
		0x10a308cd, 
		0xe7b9bdb3, 
		0x80800001, 
		0xff7fffff, 
		0x420c0000, 
		0x0c7ffffc, 
		0x00400000, 
		0x0d000ff0, 
		0x007ffffe, 
		0x18582eef, 
		0x2eff180e, 
		0x00000000, 
		0xc1400000, 
		0x42640000, 
		0xd0927660, 
		0x7fc00000, 
		0xbf800000, 
		0x54fcc95a, 
		0x0c7fff80, 
		0xff800000, 
		0x197adf9f, 
		0x00800000, 
		0x6f00afcb, 
		0xc1a00000, 
		0xda3eea7f, 
		0xc0000000, 
		0xc0400000, 
		0x80572666, 
		0x42640000, 
		0x299626e0, 
		0x80000004, 
		0x0fac72f9, 
		0x15bbcdb3, 
		0xc1a80000, 
		0x9d726d16, 
		0xffbfffff, 
		0x00002000, 
		0xff7ffffe, 
		0xc1a00000, 
		0x98c91d00, 
		0x42600000, 
		0x56266fb9, 
		0xf5243274, 
		0x00fd92cc, 
		0xc2380000, 
		0x80800000, 
		0xff800000, 
		0xc26c0000, 
		0xc1f80000, 
		0x00000200, 
		0x7f800000, 
		0x3ff3f0cf, 
		0x0c7ffffe, 
		0x64db47c0, 
		0x00010000, 
		0x80000004, 
		0x41b00000, 
		0x00000000, 
		0x055e4047, 
		0x42100000, 
		0x41000000, 
		0x0c7fe000, 
		0xc2140000, 
		0xc22c0000, 
		0x0c7ff800, 
		0x1095ad44, 
		0x6b7bafca, 
		0x3017cf8a, 
		0x7f9e5870, 
		0xaaaaaaaa, 
		0x80800001, 
		0xcf5777dd, 
		0xc0e00000, 
		0xba96d041, 
		0xc2000000, 
		0x8f7ffffd, 
		0x8fb21b03, 
		0xfac0776b, 
		0x2e445b4e, 
		0x7f7ffffe, 
		0xc23c0000, 
		0x7f7ffffe, 
		0xc25c0000, 
		0x0e7fffff, 
		0x41400000, 
		0x0f7fffff, 
		0x93f55885, 
		0x41800000, 
		0xffffffff, 
		0xcae5ed8d, 
		0x0e0007ff, 
		0xcb8c4b40, 
		0x06b35f90, 
		0xc21c0000, 
		0x00000000, 
		0x9779b13f, 
		0xd7f57cec, 
		0x860521d5, 
		0x00000000, 
		0x42200000, 
		0x70a1edbe, 
		0x7fda174f, 
		0x0e00001f, 
		0x4b371151, 
		0xff7fffff, 
		0x00000004, 
		0x76ffa6ac, 
		0xff9ffa97, 
		0x00000000, 
		0x80000000, 
		0x0043fee9, 
		0xcf180ea2, 
		0x9e494086, 
		0xc7f90d84, 
		0x21d9a71c, 
		0x424c0000, 
		0x8f19c99f, 
		0xff800000, 
		0x80800000, 
		0xc1100000, 
		0x3e9cc8ea, 
		0x0e0001ff, 
		0x42580000, 
		0xc1980000, 
		0x42500000, 
		0xc0a00000, 
		0x00010000, 
		0x7288e987, 
		0x0e0003ff, 
		0xc20c0000, 
		0xa8f5d951, 
		0x42000000, 
		0x00000200, 
		0x0c7fc000, 
		0xff7ffffe, 
		0x27c3fe59, 
		0x9e4b2289, 
		0x00800002, 
		0x13c4761a, 
		0x0e0000ff, 
		0xc2140000, 
		0x80000000, 
		0x807ffffe, 
		0x7e253a17, 
		0x566e4660, 
		0xf82a69c1, 
		0xc2000000, 
		0x00040000, 
		0x4940c60b, 
		0xb481f9bd, 
		0x1e67608b, 
		0x5949edd6, 
		0x000bbbcf, 
		0xc1500000, 
		0x80011111, 
		0xc1d80000, 
		0x42600000, 
		0x0c7fff00, 
		0xc1f80000, 
		0x00008000, 
		0x7f7ffffe, 
		0xffb5a53d, 
		0x80100000, 
		0xc2500000, 
		0x42140000, 
		0xa82d408e, 
		0x2a0294a0, 
		0x42380000, 
		0xc2100000, 
		0x9d6a2e77, 
		0x3d168447, 
		0x9bf4e9a0, 
		0x5a1f8ec2, 
		0x7f800000, 
		0x00080000, 
		0x00800000, 
		0x0e00000f, 
		0x80252c58, 
		0x3f800001, 
		0x24de47f1, 
		0x00002000, 
		0x41200000, 
		0x80200000, 
		0x0c7fffc0, 
		0xc2140000, 
		0xc1d80000, 
		0x7fc00000, 
		0x7fa5f5e8, 
		0x80800000, 
		0xd836039a, 
		0xf0beb0b7, 
		0x3b780d27, 
		0x0e0001ff, 
		0x80000000, 
		0xbf800001, 
		0x74c12986, 
		0xaeade996, 
		0x422c0000, 
		0x425c0000, 
		0x4ee59d23, 
		0x41400000, 
		0x42100000, 
		0x80080000, 
		0x7fffffff, 
		0x80800003, 
		0x0c7fe000, 
		0x8687bd76, 
		0x42780000, 
		0x80000100, 
		0x05f5c567, 
		0x0c7ffff0, 
		0xff97d956, 
		0x423c0000, 
		0x4b8c4b40, 
		0x035f4abe, 
		0x80010000, 
		0xaf33d0f9, 
		0x42200000, 
		0x41400000, 
		0x42280000, 
		0x3f98fba0, 
		0x00000200, 
		0xff000000, 
		0xb25be511, 
		0xf813e3e0, 
		0xb7554e87, 
		0xce5fbc42, 
		0xa6aa75a6, 
		0x42340000, 
		0xc2500000, 
		0xe4aa5208, 
		0x586f924f, 
		0x80000008, 
		0x80000001, 
		0xc0400000, 
		0x41800000, 
		0x6c824267, 
		0x00000000, 
		0xdfdc5211, 
		0xc378cfc0, 
		0x421c0000, 
		0x41880000, 
		0x41200000, 
		0x80800000, 
		0x6f2527e2, 
		0x42240000, 
		0x00010000, 
		0xc2340000, 
		0x00000000, 
		0xd8fd6039, 
		0x80000000, 
		0x9579943f, 
		0xc1900000, 
		0x00800002, 
		0xc2700000, 
		0x40000000, 
		0x41f00000, 
		0x7ea09290, 
		0xf1c02c8e, 
		0x80800003, 
		0x80800000, 
		0x303f7d44, 
		0x59db518c, 
		0xc27c0000, 
		0xa8d076d3, 
		0x00000000, 
		0xc0800000, 
		0x00200000, 
		0x7fbc8d23, 
		0xa22de1a4, 
		0x80000000, 
		0x80004000, 
		0xcb000000, 
		0xff7fffff, 
		0x80ffffff, 
		0x1f572bbf, 
		0x0374a39a, 
		0xc0000000, 
		0x00004000, 
		0x93006a09, 
		0x0c700000, 
		0x0c7ffe00, 
		0x0e7fffff, 
		0x0e0007ff, 
		0xc1de37bf, 
		0x0e01ffff, 
		0x7f800000, 
		0x42580000, 
		0x80800003, 
		0x7f800000, 
		0xd1516304, 
		0x5cb2d07a, 
		0x8837b822, 
		0x00010000, 
		0x80000000, 
		0xf56fa4f5, 
		0x80000000, 
		0x7f800000, 
		0x7f800000, 
		0x0c7fff80, 
		0x00000000, 
		0x42200000, 
		0x00000100, 
		0xff800000, 
		0x00000800, 
		0xe7bd6620, 
		0x7f7ffffe, 
		0xeccdb503, 
		0x80010000, 
		0x00000000, 
		0x0e000fff, 
		0xc1200000, 
		0x0c7fe000, 
		0x80008000, 
		0xffa816ab, 
		0x42480000, 
		0x7f800000, 
		0x2e07f149, 
		0x80ffffff, 
		0xb8a4a3f2, 
		0x7f800000, 
		0xff800000, 
		0x42500000, 
		0x66774ef1, 
		0x80000000, 
		0xa05d9dc5, 
		0x00800002, 
		0x0e0007ff, 
		0x80000020, 
		0xc2280000, 
		0x80010000, 
		0x1f237a2d, 
		0x0f60ed76, 
		0x80000000, 
		0x7f7fffff, 
		0x3ebdb242, 
		0x57c7b0ee, 
		0xc1900000, 
		0xffbfffff, 
		0x0c7fff00, 
		0x05208b2d, 
		0x224f5ccb, 
		0xff800000, 
		0x00000000, 
		0x7f882236, 
		0xc1f80000, 
		0x7f800000, 
		0xffffffff, 
		0xb4e83da3, 
		0x00000001, 
		0x8e59d107, 
		0x392734b5, 
		0x80000400, 
		0x40a00000, 
		0xdb729ff5, 
		0x7f800000, 
		0x06a3b15e, 
		0x0c7ffff0, 
		0x0e07ffff, 
		0x7f800000, 
		0x14143995, 
		0x10755d63, 
		0x8e2ab2e0, 
		0xc1500000, 
		0x82279209, 
		0x689f5b50, 
		0x0c7c0000, 
		0x7f800000, 
		0x953aaa93, 
		0x8b979425, 
		0xff800000, 
		0x0e176981, 
		0xcccccccc, 
		0x40000000, 
		0x00000010, 
		0x314a2ad7, 
		0x8f7fffff, 
		0xc1880000, 
		0x7d800009, 
		0x45496bb0, 
		0x0736a70f, 
		0x00000800, 
		0x353c6eee, 
		0xc1c00000, 
		0x80800000, 
		0x80004000, 
		0xddcdf094, 
		0x6eb02848, 
		0xffa0a6ce, 
		0x095b6b1d, 
		0x41c00000, 
		0x54d2aedc, 
		0x71679d79, 
		0x4a4e95e5, 
		0x0e007fff, 
		0x80000800, 
		0x7fc00001, 
		0x3d2a4524, 
		0x42580000, 
		0xff800000, 
		0x1d491157, 
		0x00011111, 
		0x41caebfd, 
		0xab1944ce, 
		0xd549f755, 
		0x42440000, 
		0xc0000000, 
		0x80002000, 
		0x53c711aa, 
		0x696585cb, 
		0xea201c59, 
		0xc2300000, 
		0xc86876ad, 
		0x0c7ffc00, 
		0x9b421cfa, 
		0x0e000007, 
		0x0c7ffffc, 
		0xc2080000, 
		0xc0800000, 
		0x00000000, 
		0xff7ffffe, 
		0x6803b469, 
		0x8f1cbf3d, 
		0x7f000000, 
		0xff90a432, 
		0xacd9e6f6, 
		0x00000000, 
		0xc20c0000, 
		0xf30c6c59, 
		0xc2100000, 
		0x0c600000, 
		0xcb000000, 
		0x00000100, 
		0x42440000, 
		0x7763c6c2, 
		0x41f80000, 
		0x41800000, 
		0x6a5f28ba, 
		0xc27c0000, 
		0x46f4e90a, 
		0x42400000, 
		0x00000004, 
		0xaa22643a, 
		0x00000000, 
		0x0a8bbe82, 
		0x80000000, 
		0xbf800000, 
		0x4b8c4b40, 
		0xc3beea64, 
		0xc1c00000, 
		0x40c00000, 
		0x41000000, 
		0xc1600000, 
		0xc1f80000, 
		0x0e07ffff, 
		0x80200000, 
		0x00000001, 
		0xddbc9b11, 
		0xc2440000, 
		0x0f7ffffe, 
		0x2075b2c9, 
		0x80011111, 
		0xff2ac740, 
		0x41700000, 
		0x00002000, 
		0x3f800001, 
		0xff8d01c6, 
		0xa42a160e, 
		0x9931989a, 
		0xff800000, 
		0x0c7fe000, 
		0xb79fda5e, 
		0x6752a317, 
		0xff998f2b, 
		0x00000001, 
		0x323ac400, 
		0x29842755, 
		0x80800000, 
		0xec08cc26, 
		0x41600000, 
		0x41900000, 
		0xc2280000, 
		0xff7ffffe, 
		0x0b9f742f, 
		0x98dd087d, 
		0x7fbda093, 
		0xecd6257e, 
		0xc24c0000, 
		0xc0400000, 
		0xcccccccc, 
		0xc25c0000, 
		0x800a54c6, 
		0x0e00ffff, 
		0xadde8178, 
		0x00251e5a, 
		0x41f80000, 
		0x80080000, 
		0x42080000, 
		0x4734e9c3, 
		0x9efb0bcb, 
		0x8f7ffffc, 
		0x00800003, 
		0x4b000000, 
		0xa03dc481, 
		0x7f7fffff, 
		0x0c400000, 
		0x113475c5, 
		0x41500000, 
		0x7f800000, 
		0x00000000, 
		0xa1539ffd, 
		0x00800003, 
		0x77ddb1ff, 
		0xea58c06d, 
		0xc2680000, 
		0x80000000, 
		0x0e001fff, 
		0x6799599a, 
		0x0e000fff, 
		0xff7ffffe, 
		0x0c7fffc0, 
		0x303f0430, 
		0x3ddb153b, 
		0x0c7fff00, 
		0xc2300000, 
		0x423c0000, 
		0x0e001fff, 
		0x7f800000, 
		0x7f800000, 
		0xd7aa657d, 
		0x0e0003ff, 
		0xa2df4e9f, 
		0x41f80000, 
		0xff7ffffe, 
		0x607c1504, 
		0x144454ff, 
		0x5f57f3a6, 
		0x42480000, 
		0xc2400000, 
		0xc2380000, 
		0x6d162dfa, 
		0x41e00000, 
		0x7f7ffffe, 
		0x34362d29, 
		0x4b000000, 
		0x42680000, 
		0xc2080000, 
		0x00001000, 
		0xc0e00000, 
		0xe670fb4a, 
		0xc0000000, 
		0x40400000, 
		0x00004000, 
		0x00800001, 
		0x7fc00000, 
		0x8990e65b, 
		0xd3ee97b6, 
		0x00000000, 
		0x7f800000, 
		0x0c7c0000, 
		0xc2280000, 
		0x0ee2ba8d, 
		0x7f800000, 
		0x618d8cff, 
		0xc0000000, 
		0x80200000, 
		0x807fffff, 
		0x4b8c4b40, 
		0x807fffff, 
		0x00000080, 
		0xf8fd6d7a, 
		0x00100000, 
		0x80100000, 
		0x41400000, 
		0x41700000, 
		0x30d0d66f, 
		0xc1d00000, 
		0x4eb548e3, 
		0x0e0003ff, 
		0xf500f53a, 
		0xc2180000, 
		0x8f7ffffe, 
		0x00000008, 
		0x0c7fe000, 
		0xd4b2af14, 
		0x0c7fff00, 
		0x6ac11091, 
		0x432bd5e1, 
		0xcb8c4b40, 
		0x42040000, 
		0x0025385d, 
		0xc2480000, 
		0xc2300000, 
		0xb1b866f0, 
		0x4e733ffd, 
		0x0c7ffff0, 
		0xc0c00000, 
		0xc21c0000, 
		0x36787102, 
		0xf314e66d, 
		0xffbfffff, 
		0xb73bfcc3, 
		0xbd1881ce, 
		0x807dddcb, 
		0xc1500000, 
		0x7a7fb009, 
		0x41454ef7, 
		0x0e01ffff, 
		0x3df1ca8d, 
		0x80000008, 
		0x42700000, 
		0xeb6fbff2, 
		0xc1700000, 
		0x3f028f5c, 
		0x7f000000, 
		0x80000002, 
		0x41c00000, 
		0x004aeb5b, 
		0x490fca4d, 
		0x80000020  
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

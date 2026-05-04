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
			"csrwi mhpmevent4, 17\n"   
			"csrwi mhpmevent5, 27\n"   
			"csrwi mhpmevent6, 26\n"   
			"csrwi mhpmevent7, 6\n"   
			"csrwi mhpmevent8, 4\n"   
			"li x5, 0x00000901        \n"   
			"li x6, 0x80F1E030      \n"   
			"sd x5, 0(x6)             \n"   
			"fence iorw, iorw         \n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f19,  32 (x5)\n"
			"flw.ps f21,  128 (x5)\n"
			"flw.ps f28,  384 (x5)\n"
			"flw.ps f3,  256 (x5)\n"
			"flw.ps f5,  288 (x5)\n"
			"flw.ps f2,  448 (x5)\n"
			"flw.ps f26,  896 (x5)\n"
			"flw.ps f22,  192 (x5)\n"
			"flw.ps f15,  672 (x5)\n"
			"flw.ps f23,  608 (x5)\n"
			"flw.ps f1,  160 (x5)\n"
			"flw.ps f25,  832 (x5)\n"
			"flw.ps f17,  544 (x5)\n"
			"flw.ps f24,  768 (x5)\n"
			"flw.ps f12,  480 (x5)\n"
			"flw.ps f13,  512 (x5)\n"
			"flw.ps f27,  320 (x5)\n"
			"flw.ps f31,  352 (x5)\n"
			"flw.ps f7,  704 (x5)\n"
			"flw.ps f20,  96 (x5)\n"
			"flw.ps f0,  736 (x5)\n"
			"flw.ps f6,  224 (x5)\n"
			"flw.ps f18,  960 (x5)\n"
			"flw.ps f9,  0 (x5)\n"
			"flw.ps f11,  64 (x5)\n"
			"flw.ps f14,  416 (x5)\n"
			"flw.ps f10,  576 (x5)\n"
			"flw.ps f4,  992 (x5)\n"
			"flw.ps f30,  640 (x5)\n"
			"flw.ps f16,  864 (x5)\n"
			"flw.ps f8,  800 (x5)\n"
			"flw.ps f29,  928 (x5)\n"
			"LBL_EN_COOP_ESR_HID_0:\n"          
			"li x5, 0x80740290\n"  
			"li x31, 1\n"
			"sd x31, 0(x5)\n"
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
			"LBL_INIT_FCC_CTRS_HID_0:\n"          
			"li x31, 0x803400C0\n" 
			"sd x0, 0(x31)\n"
			"li x31, 0x803400C8\n" 
			"sd x0, 0(x31)\n"
			"li x31, 0x803400D0\n" 
			"sd x0, 0(x31)\n"
			"li x31, 0x803400D8\n" 
			"sd x0, 0(x31)\n"
			"fence\n"
			"LBL_INIT_FLB_CTRS_HID_0:\n"          
			"li x31, 0x80340100\n"
			"sd x0, 0(x31)\n"
			"li x31, 0x80340108\n"
			"sd x0, 0(x31)\n"
			"li x31, 0x80340110\n"
			"sd x0, 0(x31)\n"
			"li x31, 0x80340118\n"
			"sd x0, 0(x31)\n"
			"li x31, 0x80340120\n"
			"sd x0, 0(x31)\n"
			"li x31, 0x80340128\n"
			"sd x0, 0(x31)\n"
			"li x31, 0x80340130\n"
			"sd x0, 0(x31)\n"
			"li x31, 0x80340138\n"
			"sd x0, 0(x31)\n"
			"li x31, 0x80340140\n"
			"sd x0, 0(x31)\n"
			"li x31, 0x80340148\n"
			"sd x0, 0(x31)\n"
			"li x31, 0x80340150\n"
			"sd x0, 0(x31)\n"
			"li x31, 0x80340158\n"
			"sd x0, 0(x31)\n"
			"li x31, 0x80340160\n"
			"sd x0, 0(x31)\n"
			"li x31, 0x80340168\n"
			"sd x0, 0(x31)\n"
			"li x31, 0x80340170\n"
			"sd x0, 0(x31)\n"
			"li x31, 0x80340178\n"
			"sd x0, 0(x31)\n"
			"li x31, 0x80340180\n"
			"sd x0, 0(x31)\n"
			"li x31, 0x80340188\n"
			"sd x0, 0(x31)\n"
			"li x31, 0x80340190\n"
			"sd x0, 0(x31)\n"
			"li x31, 0x80340198\n"
			"sd x0, 0(x31)\n"
			"li x31, 0x803401A0\n"
			"sd x0, 0(x31)\n"
			"li x31, 0x803401A8\n"
			"sd x0, 0(x31)\n"
			"li x31, 0x803401B0\n"
			"sd x0, 0(x31)\n"
			"li x31, 0x803401B8\n"
			"sd x0, 0(x31)\n"
			"li x31, 0x803401C0\n"
			"sd x0, 0(x31)\n"
			"li x31, 0x803401C8\n"
			"sd x0, 0(x31)\n"
			"li x31, 0x803401D0\n"
			"sd x0, 0(x31)\n"
			"li x31, 0x803401D8\n"
			"sd x0, 0(x31)\n"
			"li x31, 0x803401E0\n"
			"sd x0, 0(x31)\n"
			"li x31, 0x803401E8\n"
			"sd x0, 0(x31)\n"
			"li x31, 0x803401F0\n"
			"sd x0, 0(x31)\n"
			"li x31, 0x803401F8\n"
			"sd x0, 0(x31)\n"
			"fence\n"
			"li x5, 0x803400C0\n"
			"li x31, 0xff\n"
			"sd x31, 0(x5)\n"
			"li x5, 0x803400D0\n"
			"li x31, 0xff\n"
			"sd x31, 0(x5)\n"
			"fence\n"
			"nop\n"          
		       VSNIP_RSV     
		);
		__asm__ __volatile__ ( 
			"nop\n"          
			"LBL_KERNEL_HID_0:\n"          
			"li x5, 0xFEEDFEED\n"
			"li x0, 0x4\n" 
			"slti x0, x0, 0x7f2\n"
			"li x29, 0x6\n"
			"LBL_OUTER_LOOP_SEQ_HID_0:\n"
			"nop \n"
			"nop\n"          
		       VSNIP_RSV     
		);
		__asm__ __volatile__ ( 
			"nop\n"          
			"nop \n"
			"addi x29, x29, -1\n"
			"bne x29, x0, LBL_OUTER_LOOP_SEQ_HID_0\n"
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
			"nop\n"          
		       VSNIP_RSV     
		);
		__asm__ __volatile__ ( 
			"nop\n"          
			"LBL_DIS_COOP_ESR_HID_0:\n"          
			"li x5, 0x80740290\n"  
			"li x31, 0\n"
			"sd x31, 0(x5)\n"
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
			"LBL_HPM_4_CORE_TS_OPS_FAIL_HID_0:\n"          
			"csrr x5, mhpmcounter4\n"
			"sd x5, 40 (x31)\n"
			"LBL_HPM_5_CORE_TREDUCE_INST_FAIL_HID_0:\n"          
			"csrr x5, mhpmcounter5\n"
			"sd x5, 48 (x31)\n"
			"LBL_HPM_6_CORE_TFMA_INST_FAIL_HID_0:\n"          
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
			"LBL_HPM_3_CORE_DCACHE_ACCESS0_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter3\n"
			"sd x5, 32 (x31)\n"
			"LBL_HPM_4_CORE_TS_OPS_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter4\n"
			"sd x5, 40 (x31)\n"
			"LBL_HPM_5_CORE_TREDUCE_INST_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter5\n"
			"sd x5, 48 (x31)\n"
			"LBL_HPM_6_CORE_TFMA_INST_PASS_HID_0:\n"          
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
		__asm__ __volatile__ ( 
			"LBL_HALT_HID_0:\n"          
			".global LBL_HALT_HID_0\n"          
			VSNIP_RSV
		);
	};
	if ( hid == 1 ) { 
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
			"csrwi mhpmevent3, 1\n"   
			"csrwi mhpmevent4, 6\n"   
			"csrwi mhpmevent5, 3\n"   
			"csrwi mhpmevent6, 2\n"   
			"csrwi mhpmevent7, 1\n"   
			"csrwi mhpmevent8, 22\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f29,  864 (x5)\n"
			"flw.ps f18,  576 (x5)\n"
			"flw.ps f19,  800 (x5)\n"
			"flw.ps f9,  512 (x5)\n"
			"flw.ps f25,  480 (x5)\n"
			"flw.ps f8,  928 (x5)\n"
			"flw.ps f4,  224 (x5)\n"
			"flw.ps f28,  160 (x5)\n"
			"flw.ps f16,  544 (x5)\n"
			"flw.ps f14,  896 (x5)\n"
			"flw.ps f10,  32 (x5)\n"
			"flw.ps f0,  960 (x5)\n"
			"flw.ps f27,  672 (x5)\n"
			"flw.ps f11,  320 (x5)\n"
			"flw.ps f26,  832 (x5)\n"
			"flw.ps f13,  768 (x5)\n"
			"flw.ps f22,  128 (x5)\n"
			"flw.ps f12,  64 (x5)\n"
			"flw.ps f20,  96 (x5)\n"
			"flw.ps f6,  256 (x5)\n"
			"flw.ps f5,  992 (x5)\n"
			"flw.ps f30,  736 (x5)\n"
			"flw.ps f31,  352 (x5)\n"
			"flw.ps f7,  384 (x5)\n"
			"flw.ps f15,  416 (x5)\n"
			"flw.ps f23,  640 (x5)\n"
			"flw.ps f24,  608 (x5)\n"
			"flw.ps f1,  448 (x5)\n"
			"flw.ps f3,  704 (x5)\n"
			"flw.ps f17,  288 (x5)\n"
			"flw.ps f2,  192 (x5)\n"
			"flw.ps f21,  0 (x5)\n"
			"csrw fflags, zero\n"
			"csrwi frm, 0x0\n"
			"li x31, 100\n"
			"1:\n" 
			"addi x31, x31, -1\n"
			"bne x31, x0, 1b\n" 
			"csrw 0x821, x0\n" 
			"nop\n"          
		       VSNIP_RSV     
		);
		__asm__ __volatile__ ( 
			"nop\n"          
			"LBL_KERNEL_HID_1:\n"          
			"li x5, 0xFEEDFEED\n"
			"slti x0, x0, 0x7f2\n"
			"li x29, 0x6\n"
			"LBL_OUTER_LOOP_SEQ_HID_1:\n"
			"nop \n"
			"nop\n"          
		       VSNIP_RSV     
		);
		__asm__ __volatile__ ( 
			"nop\n"          
			"nop \n"
			"addi x29, x29, -1\n"
			"bne x29, x0, LBL_OUTER_LOOP_SEQ_HID_1\n"
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
			"nop\n"          
		       VSNIP_RSV     
		);
		__asm__ __volatile__ ( 
			"nop\n"          
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
			"LBL_HPM_3_CORE_MCYCLES_FAIL_HID_1:\n"          
			"csrr x5, mhpmcounter3\n"
			"sd x5, 32 (x31)\n"
			"LBL_HPM_4_CORE_DCACHE_ACCESS0_FAIL_HID_1:\n"          
			"csrr x5, mhpmcounter4\n"
			"sd x5, 40 (x31)\n"
			"LBL_HPM_5_CORE_RETIRED_INST1_FAIL_HID_1:\n"          
			"csrr x5, mhpmcounter5\n"
			"sd x5, 48 (x31)\n"
			"LBL_HPM_6_CORE_RETIRED_INST0_FAIL_HID_1:\n"          
			"csrr x5, mhpmcounter6\n"
			"sd x5, 56 (x31)\n"
			"LBL_HPM_7_NEIGH_ANY_CORE_TX_ETL_REQ_FAIL_HID_1:\n"          
			"csrr x5, mhpmcounter7\n"
			"sd x5, 64 (x31)\n"
			"LBL_HPM_8_NEIGH_ETL_REQ_TO_SC_UC_FIFO_FAIL_HID_1:\n"          
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
			"LBL_HPM_3_CORE_MCYCLES_PASS_HID_1:\n"          
			"csrr x5, mhpmcounter3\n"
			"sd x5, 32 (x31)\n"
			"LBL_HPM_4_CORE_DCACHE_ACCESS0_PASS_HID_1:\n"          
			"csrr x5, mhpmcounter4\n"
			"sd x5, 40 (x31)\n"
			"LBL_HPM_5_CORE_RETIRED_INST1_PASS_HID_1:\n"          
			"csrr x5, mhpmcounter5\n"
			"sd x5, 48 (x31)\n"
			"LBL_HPM_6_CORE_RETIRED_INST0_PASS_HID_1:\n"          
			"csrr x5, mhpmcounter6\n"
			"sd x5, 56 (x31)\n"
			"LBL_HPM_7_NEIGH_ANY_CORE_TX_ETL_REQ_PASS_HID_1:\n"          
			"csrr x5, mhpmcounter7\n"
			"sd x5, 64 (x31)\n"
			"LBL_HPM_8_NEIGH_ETL_REQ_TO_SC_UC_FIFO_PASS_HID_1:\n"          
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
	if ( hid == 2 ) { 
		C_TEST_START;
		__asm__ __volatile__ (
			"LBL_PREAMBLE_HID_2:\n"          
			"la x5, LBL_M_MODE_E_H\n"
			"csrw mtvec, x5\n"
			"la x5, LBL_FAIL_HID_2\n"
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
			"csrwi mhpmevent4, 17\n"   
			"csrwi mhpmevent5, 27\n"   
			"csrwi mhpmevent6, 26\n"   
			"csrwi mhpmevent7, 6\n"   
			"csrwi mhpmevent8, 4\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f27,  352 (x5)\n"
			"flw.ps f31,  576 (x5)\n"
			"flw.ps f22,  672 (x5)\n"
			"flw.ps f20,  320 (x5)\n"
			"flw.ps f23,  416 (x5)\n"
			"flw.ps f17,  832 (x5)\n"
			"flw.ps f18,  608 (x5)\n"
			"flw.ps f25,  448 (x5)\n"
			"flw.ps f10,  928 (x5)\n"
			"flw.ps f29,  192 (x5)\n"
			"flw.ps f6,  256 (x5)\n"
			"flw.ps f13,  640 (x5)\n"
			"flw.ps f7,  0 (x5)\n"
			"flw.ps f14,  64 (x5)\n"
			"flw.ps f2,  704 (x5)\n"
			"flw.ps f24,  32 (x5)\n"
			"flw.ps f19,  544 (x5)\n"
			"flw.ps f16,  128 (x5)\n"
			"flw.ps f28,  288 (x5)\n"
			"flw.ps f9,  992 (x5)\n"
			"flw.ps f4,  960 (x5)\n"
			"flw.ps f11,  480 (x5)\n"
			"flw.ps f15,  160 (x5)\n"
			"flw.ps f12,  224 (x5)\n"
			"flw.ps f1,  800 (x5)\n"
			"flw.ps f3,  512 (x5)\n"
			"flw.ps f30,  96 (x5)\n"
			"flw.ps f5,  736 (x5)\n"
			"flw.ps f8,  896 (x5)\n"
			"flw.ps f21,  384 (x5)\n"
			"flw.ps f26,  768 (x5)\n"
			"flw.ps f0,  864 (x5)\n"
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
			"li x31, 100\n"
			"1:\n" 
			"addi x31, x31, -1\n"
			"bne x31, x0, 1b\n" 
			"csrw 0x821, x0\n" 
			"nop\n"          
		       VSNIP_RSV     
		);
		__asm__ __volatile__ ( 
			"nop\n"          
			"LBL_KERNEL_HID_2:\n"          
			"li x5, 0xFEEDFEED\n"
			"slti x0, x0, 0x7f2\n"
			"li x29, 0x6\n"
			"LBL_OUTER_LOOP_SEQ_HID_2:\n"
			"nop \n"
			"nop\n"          
		       VSNIP_RSV     
		);
		__asm__ __volatile__ ( 
			"nop\n"          
			"nop \n"
			"addi x29, x29, -1\n"
			"bne x29, x0, LBL_OUTER_LOOP_SEQ_HID_2\n"
			"LBL_POSTAMBLE_HID_2:\n"          
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
			"csrrci x0, mcache_control, 2\n"
			"csrwi tensor_wait, 0x6\n" 
			"fence\n"
			"csrwi tensor_wait, 0x6\n" 
			"fence\n"
			"csrwi excl_mode, 0\n"
			"nop\n"          
		       VSNIP_RSV     
		);
		__asm__ __volatile__ ( 
			"nop\n"          
			"csrwi excl_mode, 0\n"
			"j LBL_PASS_HID_2\n"          
			VSNIP_RSV
		);
		__asm__ __volatile__ ( 
			"LBL_FAIL_HID_2:\n"          
			".global LBL_FAIL_HID_2\n"          
			VSNIP_RSV
		);
		C_TEST_FAIL;
		__asm__ __volatile__ ( 
			"j LBL_HALT_HID_2\n"          
			VSNIP_RSV
		);
		__asm__ __volatile__ ( 
			"LBL_PASS_HID_2:\n"          
			".global LBL_PASS_HID_2\n"          
			VSNIP_RSV
		);
		__asm__ __volatile__ ( 
			"LBL_HALT_HID_2:\n"          
			".global LBL_HALT_HID_2\n"          
			VSNIP_RSV
		);
	};
	if ( hid == 3 ) { 
		C_TEST_START;
		__asm__ __volatile__ (
			"LBL_PREAMBLE_HID_3:\n"          
			"la x5, LBL_M_MODE_E_H\n"
			"csrw mtvec, x5\n"
			"la x5, LBL_FAIL_HID_3\n"
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
			"csrwi mhpmevent3, 1\n"   
			"csrwi mhpmevent4, 6\n"   
			"csrwi mhpmevent5, 3\n"   
			"csrwi mhpmevent6, 2\n"   
			"csrwi mhpmevent7, 1\n"   
			"csrwi mhpmevent8, 22\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f18,  608 (x5)\n"
			"flw.ps f13,  800 (x5)\n"
			"flw.ps f6,  0 (x5)\n"
			"flw.ps f11,  576 (x5)\n"
			"flw.ps f22,  288 (x5)\n"
			"flw.ps f20,  704 (x5)\n"
			"flw.ps f16,  896 (x5)\n"
			"flw.ps f26,  512 (x5)\n"
			"flw.ps f27,  96 (x5)\n"
			"flw.ps f8,  640 (x5)\n"
			"flw.ps f2,  992 (x5)\n"
			"flw.ps f10,  544 (x5)\n"
			"flw.ps f1,  128 (x5)\n"
			"flw.ps f31,  256 (x5)\n"
			"flw.ps f21,  768 (x5)\n"
			"flw.ps f17,  384 (x5)\n"
			"flw.ps f4,  928 (x5)\n"
			"flw.ps f12,  320 (x5)\n"
			"flw.ps f23,  960 (x5)\n"
			"flw.ps f24,  736 (x5)\n"
			"flw.ps f28,  224 (x5)\n"
			"flw.ps f25,  672 (x5)\n"
			"flw.ps f29,  64 (x5)\n"
			"flw.ps f9,  832 (x5)\n"
			"flw.ps f0,  480 (x5)\n"
			"flw.ps f14,  352 (x5)\n"
			"flw.ps f7,  192 (x5)\n"
			"flw.ps f30,  416 (x5)\n"
			"flw.ps f5,  32 (x5)\n"
			"flw.ps f15,  864 (x5)\n"
			"flw.ps f19,  160 (x5)\n"
			"flw.ps f3,  448 (x5)\n"
			"csrw fflags, zero\n"
			"csrwi frm, 0x0\n"
			"li x31, 100\n"
			"1:\n" 
			"addi x31, x31, -1\n"
			"bne x31, x0, 1b\n" 
			"csrw 0x821, x0\n" 
			"nop\n"          
		       VSNIP_RSV     
		);
		__asm__ __volatile__ ( 
			"nop\n"          
			"LBL_KERNEL_HID_3:\n"          
			"li x5, 0xFEEDFEED\n"
			"slti x0, x0, 0x7f2\n"
			"li x29, 0x6\n"
			"LBL_OUTER_LOOP_SEQ_HID_3:\n"
			"nop \n"
			"nop\n"          
		       VSNIP_RSV     
		);
		__asm__ __volatile__ ( 
			"nop\n"          
			"nop \n"
			"addi x29, x29, -1\n"
			"bne x29, x0, LBL_OUTER_LOOP_SEQ_HID_3\n"
			"LBL_POSTAMBLE_HID_3:\n"          
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
			"nop\n"          
		       VSNIP_RSV     
		);
		__asm__ __volatile__ ( 
			"nop\n"          
			"csrwi excl_mode, 0\n"
			"j LBL_PASS_HID_3\n"          
			VSNIP_RSV
		);
		__asm__ __volatile__ ( 
			"LBL_FAIL_HID_3:\n"          
			".global LBL_FAIL_HID_3\n"          
			VSNIP_RSV
		);
		C_TEST_FAIL;
		__asm__ __volatile__ ( 
			"j LBL_HALT_HID_3\n"          
			VSNIP_RSV
		);
		__asm__ __volatile__ ( 
			"LBL_PASS_HID_3:\n"          
			".global LBL_PASS_HID_3\n"          
			VSNIP_RSV
		);
		__asm__ __volatile__ ( 
			"LBL_HALT_HID_3:\n"          
			".global LBL_HALT_HID_3\n"          
			VSNIP_RSV
		);
	};
	if ( hid == 4 ) { 
		C_TEST_START;
		__asm__ __volatile__ (
			"LBL_PREAMBLE_HID_4:\n"          
			"la x5, LBL_M_MODE_E_H\n"
			"csrw mtvec, x5\n"
			"la x5, LBL_FAIL_HID_4\n"
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
			"csrwi mhpmevent4, 17\n"   
			"csrwi mhpmevent5, 27\n"   
			"csrwi mhpmevent6, 26\n"   
			"csrwi mhpmevent7, 6\n"   
			"csrwi mhpmevent8, 4\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f16,  224 (x5)\n"
			"flw.ps f30,  704 (x5)\n"
			"flw.ps f9,  672 (x5)\n"
			"flw.ps f3,  608 (x5)\n"
			"flw.ps f5,  736 (x5)\n"
			"flw.ps f7,  192 (x5)\n"
			"flw.ps f11,  640 (x5)\n"
			"flw.ps f4,  960 (x5)\n"
			"flw.ps f18,  896 (x5)\n"
			"flw.ps f22,  544 (x5)\n"
			"flw.ps f26,  576 (x5)\n"
			"flw.ps f17,  128 (x5)\n"
			"flw.ps f25,  256 (x5)\n"
			"flw.ps f29,  768 (x5)\n"
			"flw.ps f6,  384 (x5)\n"
			"flw.ps f21,  832 (x5)\n"
			"flw.ps f19,  0 (x5)\n"
			"flw.ps f1,  160 (x5)\n"
			"flw.ps f12,  992 (x5)\n"
			"flw.ps f24,  480 (x5)\n"
			"flw.ps f28,  64 (x5)\n"
			"flw.ps f23,  352 (x5)\n"
			"flw.ps f13,  32 (x5)\n"
			"flw.ps f27,  416 (x5)\n"
			"flw.ps f20,  512 (x5)\n"
			"flw.ps f31,  800 (x5)\n"
			"flw.ps f8,  448 (x5)\n"
			"flw.ps f10,  864 (x5)\n"
			"flw.ps f0,  320 (x5)\n"
			"flw.ps f14,  928 (x5)\n"
			"flw.ps f15,  288 (x5)\n"
			"flw.ps f2,  96 (x5)\n"
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
			"li x31, 100\n"
			"1:\n" 
			"addi x31, x31, -1\n"
			"bne x31, x0, 1b\n" 
			"csrw 0x821, x0\n" 
			"nop\n"          
		       VSNIP_RSV     
		);
		__asm__ __volatile__ ( 
			"nop\n"          
			"LBL_KERNEL_HID_4:\n"          
			"li x5, 0xFEEDFEED\n"
			"slti x0, x0, 0x7f2\n"
			"li x29, 0x6\n"
			"LBL_OUTER_LOOP_SEQ_HID_4:\n"
			"nop \n"
			"nop\n"          
		       VSNIP_RSV     
		);
		__asm__ __volatile__ ( 
			"nop\n"          
			"nop \n"
			"addi x29, x29, -1\n"
			"bne x29, x0, LBL_OUTER_LOOP_SEQ_HID_4\n"
			"LBL_POSTAMBLE_HID_4:\n"          
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
			"nop\n"          
		       VSNIP_RSV     
		);
		__asm__ __volatile__ ( 
			"nop\n"          
			"csrwi excl_mode, 0\n"
			"j LBL_PASS_HID_4\n"          
			VSNIP_RSV
		);
		__asm__ __volatile__ ( 
			"LBL_FAIL_HID_4:\n"          
			".global LBL_FAIL_HID_4\n"          
			VSNIP_RSV
		);
		C_TEST_FAIL;
		__asm__ __volatile__ ( 
			"j LBL_HALT_HID_4\n"          
			VSNIP_RSV
		);
		__asm__ __volatile__ ( 
			"LBL_PASS_HID_4:\n"          
			".global LBL_PASS_HID_4\n"          
			VSNIP_RSV
		);
		__asm__ __volatile__ ( 
			"LBL_HALT_HID_4:\n"          
			".global LBL_HALT_HID_4\n"          
			VSNIP_RSV
		);
	};
	if ( hid == 5 ) { 
		C_TEST_START;
		__asm__ __volatile__ (
			"LBL_PREAMBLE_HID_5:\n"          
			"la x5, LBL_M_MODE_E_H\n"
			"csrw mtvec, x5\n"
			"la x5, LBL_FAIL_HID_5\n"
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
			"csrwi mhpmevent3, 1\n"   
			"csrwi mhpmevent4, 6\n"   
			"csrwi mhpmevent5, 3\n"   
			"csrwi mhpmevent6, 2\n"   
			"csrwi mhpmevent7, 1\n"   
			"csrwi mhpmevent8, 22\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f1,  928 (x5)\n"
			"flw.ps f14,  288 (x5)\n"
			"flw.ps f17,  736 (x5)\n"
			"flw.ps f9,  320 (x5)\n"
			"flw.ps f3,  576 (x5)\n"
			"flw.ps f16,  864 (x5)\n"
			"flw.ps f21,  0 (x5)\n"
			"flw.ps f2,  704 (x5)\n"
			"flw.ps f15,  544 (x5)\n"
			"flw.ps f22,  64 (x5)\n"
			"flw.ps f24,  896 (x5)\n"
			"flw.ps f27,  96 (x5)\n"
			"flw.ps f23,  608 (x5)\n"
			"flw.ps f6,  480 (x5)\n"
			"flw.ps f25,  224 (x5)\n"
			"flw.ps f28,  672 (x5)\n"
			"flw.ps f26,  352 (x5)\n"
			"flw.ps f12,  128 (x5)\n"
			"flw.ps f18,  160 (x5)\n"
			"flw.ps f0,  192 (x5)\n"
			"flw.ps f20,  32 (x5)\n"
			"flw.ps f30,  800 (x5)\n"
			"flw.ps f19,  384 (x5)\n"
			"flw.ps f4,  832 (x5)\n"
			"flw.ps f11,  992 (x5)\n"
			"flw.ps f29,  960 (x5)\n"
			"flw.ps f8,  512 (x5)\n"
			"flw.ps f7,  416 (x5)\n"
			"flw.ps f13,  448 (x5)\n"
			"flw.ps f5,  768 (x5)\n"
			"flw.ps f10,  256 (x5)\n"
			"flw.ps f31,  640 (x5)\n"
			"csrw fflags, zero\n"
			"csrwi frm, 0x0\n"
			"li x31, 100\n"
			"1:\n" 
			"addi x31, x31, -1\n"
			"bne x31, x0, 1b\n" 
			"csrw 0x821, x0\n" 
			"nop\n"          
		       VSNIP_RSV     
		);
		__asm__ __volatile__ ( 
			"nop\n"          
			"LBL_KERNEL_HID_5:\n"          
			"li x5, 0xFEEDFEED\n"
			"slti x0, x0, 0x7f2\n"
			"li x29, 0x6\n"
			"LBL_OUTER_LOOP_SEQ_HID_5:\n"
			"nop \n"
			"nop\n"          
		       VSNIP_RSV     
		);
		__asm__ __volatile__ ( 
			"nop\n"          
			"nop \n"
			"addi x29, x29, -1\n"
			"bne x29, x0, LBL_OUTER_LOOP_SEQ_HID_5\n"
			"LBL_POSTAMBLE_HID_5:\n"          
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
			"nop\n"          
		       VSNIP_RSV     
		);
		__asm__ __volatile__ ( 
			"nop\n"          
			"csrwi excl_mode, 0\n"
			"j LBL_PASS_HID_5\n"          
			VSNIP_RSV
		);
		__asm__ __volatile__ ( 
			"LBL_FAIL_HID_5:\n"          
			".global LBL_FAIL_HID_5\n"          
			VSNIP_RSV
		);
		C_TEST_FAIL;
		__asm__ __volatile__ ( 
			"j LBL_HALT_HID_5\n"          
			VSNIP_RSV
		);
		__asm__ __volatile__ ( 
			"LBL_PASS_HID_5:\n"          
			".global LBL_PASS_HID_5\n"          
			VSNIP_RSV
		);
		__asm__ __volatile__ ( 
			"LBL_HALT_HID_5:\n"          
			".global LBL_HALT_HID_5\n"          
			VSNIP_RSV
		);
	};
	if ( hid == 6 ) { 
		C_TEST_START;
		__asm__ __volatile__ (
			"LBL_PREAMBLE_HID_6:\n"          
			"la x5, LBL_M_MODE_E_H\n"
			"csrw mtvec, x5\n"
			"la x5, LBL_FAIL_HID_6\n"
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
			"csrwi mhpmevent4, 17\n"   
			"csrwi mhpmevent5, 27\n"   
			"csrwi mhpmevent6, 26\n"   
			"csrwi mhpmevent7, 6\n"   
			"csrwi mhpmevent8, 4\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f8,  448 (x5)\n"
			"flw.ps f20,  704 (x5)\n"
			"flw.ps f31,  480 (x5)\n"
			"flw.ps f1,  32 (x5)\n"
			"flw.ps f5,  576 (x5)\n"
			"flw.ps f11,  416 (x5)\n"
			"flw.ps f10,  0 (x5)\n"
			"flw.ps f21,  800 (x5)\n"
			"flw.ps f0,  832 (x5)\n"
			"flw.ps f19,  640 (x5)\n"
			"flw.ps f24,  672 (x5)\n"
			"flw.ps f27,  128 (x5)\n"
			"flw.ps f22,  960 (x5)\n"
			"flw.ps f30,  352 (x5)\n"
			"flw.ps f16,  896 (x5)\n"
			"flw.ps f15,  64 (x5)\n"
			"flw.ps f18,  224 (x5)\n"
			"flw.ps f17,  192 (x5)\n"
			"flw.ps f12,  160 (x5)\n"
			"flw.ps f28,  768 (x5)\n"
			"flw.ps f26,  928 (x5)\n"
			"flw.ps f23,  512 (x5)\n"
			"flw.ps f25,  544 (x5)\n"
			"flw.ps f9,  864 (x5)\n"
			"flw.ps f7,  320 (x5)\n"
			"flw.ps f6,  992 (x5)\n"
			"flw.ps f14,  96 (x5)\n"
			"flw.ps f3,  608 (x5)\n"
			"flw.ps f2,  384 (x5)\n"
			"flw.ps f29,  256 (x5)\n"
			"flw.ps f4,  736 (x5)\n"
			"flw.ps f13,  288 (x5)\n"
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
			"li x31, 100\n"
			"1:\n" 
			"addi x31, x31, -1\n"
			"bne x31, x0, 1b\n" 
			"csrw 0x821, x0\n" 
			"nop\n"          
		       VSNIP_RSV     
		);
		__asm__ __volatile__ ( 
			"nop\n"          
			"LBL_KERNEL_HID_6:\n"          
			"li x5, 0xFEEDFEED\n"
			"slti x0, x0, 0x7f2\n"
			"li x29, 0x6\n"
			"LBL_OUTER_LOOP_SEQ_HID_6:\n"
			"nop \n"
			"nop\n"          
		       VSNIP_RSV     
		);
		__asm__ __volatile__ ( 
			"nop\n"          
			"nop \n"
			"addi x29, x29, -1\n"
			"bne x29, x0, LBL_OUTER_LOOP_SEQ_HID_6\n"
			"LBL_POSTAMBLE_HID_6:\n"          
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
			"csrrci x0, mcache_control, 2\n"
			"csrwi tensor_wait, 0x6\n" 
			"fence\n"
			"csrwi tensor_wait, 0x6\n" 
			"fence\n"
			"csrwi excl_mode, 0\n"
			"nop\n"          
		       VSNIP_RSV     
		);
		__asm__ __volatile__ ( 
			"nop\n"          
			"csrwi excl_mode, 0\n"
			"j LBL_PASS_HID_6\n"          
			VSNIP_RSV
		);
		__asm__ __volatile__ ( 
			"LBL_FAIL_HID_6:\n"          
			".global LBL_FAIL_HID_6\n"          
			VSNIP_RSV
		);
		C_TEST_FAIL;
		__asm__ __volatile__ ( 
			"j LBL_HALT_HID_6\n"          
			VSNIP_RSV
		);
		__asm__ __volatile__ ( 
			"LBL_PASS_HID_6:\n"          
			".global LBL_PASS_HID_6\n"          
			VSNIP_RSV
		);
		__asm__ __volatile__ ( 
			"LBL_HALT_HID_6:\n"          
			".global LBL_HALT_HID_6\n"          
			VSNIP_RSV
		);
	};
	if ( hid == 7 ) { 
		C_TEST_START;
		__asm__ __volatile__ (
			"LBL_PREAMBLE_HID_7:\n"          
			"la x5, LBL_M_MODE_E_H\n"
			"csrw mtvec, x5\n"
			"la x5, LBL_FAIL_HID_7\n"
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
			"csrwi mhpmevent3, 1\n"   
			"csrwi mhpmevent4, 6\n"   
			"csrwi mhpmevent5, 3\n"   
			"csrwi mhpmevent6, 2\n"   
			"csrwi mhpmevent7, 1\n"   
			"csrwi mhpmevent8, 22\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f4,  768 (x5)\n"
			"flw.ps f17,  480 (x5)\n"
			"flw.ps f7,  160 (x5)\n"
			"flw.ps f12,  320 (x5)\n"
			"flw.ps f8,  32 (x5)\n"
			"flw.ps f29,  416 (x5)\n"
			"flw.ps f11,  288 (x5)\n"
			"flw.ps f14,  864 (x5)\n"
			"flw.ps f1,  96 (x5)\n"
			"flw.ps f15,  448 (x5)\n"
			"flw.ps f0,  928 (x5)\n"
			"flw.ps f6,  544 (x5)\n"
			"flw.ps f23,  640 (x5)\n"
			"flw.ps f16,  704 (x5)\n"
			"flw.ps f2,  736 (x5)\n"
			"flw.ps f9,  384 (x5)\n"
			"flw.ps f21,  800 (x5)\n"
			"flw.ps f10,  352 (x5)\n"
			"flw.ps f26,  992 (x5)\n"
			"flw.ps f22,  608 (x5)\n"
			"flw.ps f28,  0 (x5)\n"
			"flw.ps f30,  256 (x5)\n"
			"flw.ps f25,  224 (x5)\n"
			"flw.ps f24,  64 (x5)\n"
			"flw.ps f5,  896 (x5)\n"
			"flw.ps f18,  192 (x5)\n"
			"flw.ps f19,  832 (x5)\n"
			"flw.ps f3,  512 (x5)\n"
			"flw.ps f27,  672 (x5)\n"
			"flw.ps f20,  128 (x5)\n"
			"flw.ps f31,  576 (x5)\n"
			"flw.ps f13,  960 (x5)\n"
			"csrw fflags, zero\n"
			"csrwi frm, 0x0\n"
			"li x31, 100\n"
			"1:\n" 
			"addi x31, x31, -1\n"
			"bne x31, x0, 1b\n" 
			"csrw 0x821, x0\n" 
			"nop\n"          
		       VSNIP_RSV     
		);
		__asm__ __volatile__ ( 
			"nop\n"          
			"LBL_KERNEL_HID_7:\n"          
			"li x5, 0xFEEDFEED\n"
			"slti x0, x0, 0x7f2\n"
			"li x29, 0x6\n"
			"LBL_OUTER_LOOP_SEQ_HID_7:\n"
			"nop \n"
			"nop\n"          
		       VSNIP_RSV     
		);
		__asm__ __volatile__ ( 
			"nop\n"          
			"nop \n"
			"addi x29, x29, -1\n"
			"bne x29, x0, LBL_OUTER_LOOP_SEQ_HID_7\n"
			"LBL_POSTAMBLE_HID_7:\n"          
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
			"nop\n"          
		       VSNIP_RSV     
		);
		__asm__ __volatile__ ( 
			"nop\n"          
			"csrwi excl_mode, 0\n"
			"j LBL_PASS_HID_7\n"          
			VSNIP_RSV
		);
		__asm__ __volatile__ ( 
			"LBL_FAIL_HID_7:\n"          
			".global LBL_FAIL_HID_7\n"          
			VSNIP_RSV
		);
		C_TEST_FAIL;
		__asm__ __volatile__ ( 
			"j LBL_HALT_HID_7\n"          
			VSNIP_RSV
		);
		__asm__ __volatile__ ( 
			"LBL_PASS_HID_7:\n"          
			".global LBL_PASS_HID_7\n"          
			VSNIP_RSV
		);
		__asm__ __volatile__ ( 
			"LBL_HALT_HID_7:\n"          
			".global LBL_HALT_HID_7\n"          
			VSNIP_RSV
		);
	};
	if ( hid == 8 ) { 
		C_TEST_START;
		__asm__ __volatile__ (
			"LBL_PREAMBLE_HID_8:\n"          
			"la x5, LBL_M_MODE_E_H\n"
			"csrw mtvec, x5\n"
			"la x5, LBL_FAIL_HID_8\n"
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
			"csrwi mhpmevent4, 17\n"   
			"csrwi mhpmevent5, 27\n"   
			"csrwi mhpmevent6, 26\n"   
			"csrwi mhpmevent7, 6\n"   
			"csrwi mhpmevent8, 4\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f10,  416 (x5)\n"
			"flw.ps f23,  608 (x5)\n"
			"flw.ps f6,  320 (x5)\n"
			"flw.ps f7,  384 (x5)\n"
			"flw.ps f21,  256 (x5)\n"
			"flw.ps f11,  928 (x5)\n"
			"flw.ps f8,  480 (x5)\n"
			"flw.ps f14,  992 (x5)\n"
			"flw.ps f25,  64 (x5)\n"
			"flw.ps f27,  96 (x5)\n"
			"flw.ps f31,  160 (x5)\n"
			"flw.ps f5,  672 (x5)\n"
			"flw.ps f0,  640 (x5)\n"
			"flw.ps f12,  800 (x5)\n"
			"flw.ps f18,  576 (x5)\n"
			"flw.ps f17,  736 (x5)\n"
			"flw.ps f9,  224 (x5)\n"
			"flw.ps f1,  32 (x5)\n"
			"flw.ps f19,  512 (x5)\n"
			"flw.ps f30,  352 (x5)\n"
			"flw.ps f22,  864 (x5)\n"
			"flw.ps f20,  448 (x5)\n"
			"flw.ps f2,  896 (x5)\n"
			"flw.ps f15,  288 (x5)\n"
			"flw.ps f4,  0 (x5)\n"
			"flw.ps f29,  704 (x5)\n"
			"flw.ps f13,  128 (x5)\n"
			"flw.ps f3,  768 (x5)\n"
			"flw.ps f26,  192 (x5)\n"
			"flw.ps f24,  832 (x5)\n"
			"flw.ps f28,  544 (x5)\n"
			"flw.ps f16,  960 (x5)\n"
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
			"li x31, 100\n"
			"1:\n" 
			"addi x31, x31, -1\n"
			"bne x31, x0, 1b\n" 
			"csrw 0x821, x0\n" 
			"nop\n"          
		       VSNIP_RSV     
		);
		__asm__ __volatile__ ( 
			"nop\n"          
			"LBL_KERNEL_HID_8:\n"          
			"li x5, 0xFEEDFEED\n"
			"slti x0, x0, 0x7f2\n"
			"li x29, 0x6\n"
			"LBL_OUTER_LOOP_SEQ_HID_8:\n"
			"nop \n"
			"nop\n"          
		       VSNIP_RSV     
		);
		__asm__ __volatile__ ( 
			"nop\n"          
			"nop \n"
			"addi x29, x29, -1\n"
			"bne x29, x0, LBL_OUTER_LOOP_SEQ_HID_8\n"
			"LBL_POSTAMBLE_HID_8:\n"          
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
			"csrrci x0, mcache_control, 2\n"
			"csrwi tensor_wait, 0x6\n" 
			"fence\n"
			"csrrci x0, mcache_control, 1\n"
			"csrwi tensor_wait, 0x6\n" 
			"fence\n"
			"csrwi excl_mode, 0\n"
			"nop\n"          
		       VSNIP_RSV     
		);
		__asm__ __volatile__ ( 
			"nop\n"          
			"csrwi excl_mode, 0\n"
			"j LBL_PASS_HID_8\n"          
			VSNIP_RSV
		);
		__asm__ __volatile__ ( 
			"LBL_FAIL_HID_8:\n"          
			".global LBL_FAIL_HID_8\n"          
			VSNIP_RSV
		);
		C_TEST_FAIL;
		__asm__ __volatile__ ( 
			"j LBL_HALT_HID_8\n"          
			VSNIP_RSV
		);
		__asm__ __volatile__ ( 
			"LBL_PASS_HID_8:\n"          
			".global LBL_PASS_HID_8\n"          
			VSNIP_RSV
		);
		__asm__ __volatile__ ( 
			"LBL_HALT_HID_8:\n"          
			".global LBL_HALT_HID_8\n"          
			VSNIP_RSV
		);
	};
	if ( hid == 9 ) { 
		C_TEST_START;
		__asm__ __volatile__ (
			"LBL_PREAMBLE_HID_9:\n"          
			"la x5, LBL_M_MODE_E_H\n"
			"csrw mtvec, x5\n"
			"la x5, LBL_FAIL_HID_9\n"
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
			"csrwi mhpmevent3, 1\n"   
			"csrwi mhpmevent4, 6\n"   
			"csrwi mhpmevent5, 3\n"   
			"csrwi mhpmevent6, 2\n"   
			"csrwi mhpmevent7, 1\n"   
			"csrwi mhpmevent8, 22\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f10,  544 (x5)\n"
			"flw.ps f31,  64 (x5)\n"
			"flw.ps f4,  288 (x5)\n"
			"flw.ps f8,  480 (x5)\n"
			"flw.ps f7,  704 (x5)\n"
			"flw.ps f27,  128 (x5)\n"
			"flw.ps f21,  256 (x5)\n"
			"flw.ps f19,  800 (x5)\n"
			"flw.ps f3,  768 (x5)\n"
			"flw.ps f9,  608 (x5)\n"
			"flw.ps f18,  640 (x5)\n"
			"flw.ps f16,  416 (x5)\n"
			"flw.ps f15,  928 (x5)\n"
			"flw.ps f26,  96 (x5)\n"
			"flw.ps f12,  864 (x5)\n"
			"flw.ps f30,  672 (x5)\n"
			"flw.ps f1,  448 (x5)\n"
			"flw.ps f29,  32 (x5)\n"
			"flw.ps f28,  320 (x5)\n"
			"flw.ps f20,  832 (x5)\n"
			"flw.ps f14,  992 (x5)\n"
			"flw.ps f11,  576 (x5)\n"
			"flw.ps f22,  896 (x5)\n"
			"flw.ps f17,  224 (x5)\n"
			"flw.ps f25,  0 (x5)\n"
			"flw.ps f5,  352 (x5)\n"
			"flw.ps f13,  736 (x5)\n"
			"flw.ps f0,  512 (x5)\n"
			"flw.ps f24,  192 (x5)\n"
			"flw.ps f2,  160 (x5)\n"
			"flw.ps f6,  960 (x5)\n"
			"flw.ps f23,  384 (x5)\n"
			"csrw fflags, zero\n"
			"csrwi frm, 0x0\n"
			"li x31, 100\n"
			"1:\n" 
			"addi x31, x31, -1\n"
			"bne x31, x0, 1b\n" 
			"csrw 0x821, x0\n" 
			"nop\n"          
		       VSNIP_RSV     
		);
		__asm__ __volatile__ ( 
			"nop\n"          
			"LBL_KERNEL_HID_9:\n"          
			"li x5, 0xFEEDFEED\n"
			"slti x0, x0, 0x7f2\n"
			"li x29, 0x6\n"
			"LBL_OUTER_LOOP_SEQ_HID_9:\n"
			"nop \n"
			"nop\n"          
		       VSNIP_RSV     
		);
		__asm__ __volatile__ ( 
			"nop\n"          
			"nop \n"
			"addi x29, x29, -1\n"
			"bne x29, x0, LBL_OUTER_LOOP_SEQ_HID_9\n"
			"LBL_POSTAMBLE_HID_9:\n"          
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
			"nop\n"          
		       VSNIP_RSV     
		);
		__asm__ __volatile__ ( 
			"nop\n"          
			"csrwi excl_mode, 0\n"
			"j LBL_PASS_HID_9\n"          
			VSNIP_RSV
		);
		__asm__ __volatile__ ( 
			"LBL_FAIL_HID_9:\n"          
			".global LBL_FAIL_HID_9\n"          
			VSNIP_RSV
		);
		C_TEST_FAIL;
		__asm__ __volatile__ ( 
			"j LBL_HALT_HID_9\n"          
			VSNIP_RSV
		);
		__asm__ __volatile__ ( 
			"LBL_PASS_HID_9:\n"          
			".global LBL_PASS_HID_9\n"          
			VSNIP_RSV
		);
		__asm__ __volatile__ ( 
			"LBL_HALT_HID_9:\n"          
			".global LBL_HALT_HID_9\n"          
			VSNIP_RSV
		);
	};
	if ( hid == 10 ) { 
		C_TEST_START;
		__asm__ __volatile__ (
			"LBL_PREAMBLE_HID_10:\n"          
			"la x5, LBL_M_MODE_E_H\n"
			"csrw mtvec, x5\n"
			"la x5, LBL_FAIL_HID_10\n"
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
			"csrwi mhpmevent4, 17\n"   
			"csrwi mhpmevent5, 27\n"   
			"csrwi mhpmevent6, 26\n"   
			"csrwi mhpmevent7, 6\n"   
			"csrwi mhpmevent8, 4\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f11,  512 (x5)\n"
			"flw.ps f31,  864 (x5)\n"
			"flw.ps f6,  800 (x5)\n"
			"flw.ps f30,  640 (x5)\n"
			"flw.ps f19,  896 (x5)\n"
			"flw.ps f24,  928 (x5)\n"
			"flw.ps f21,  352 (x5)\n"
			"flw.ps f26,  256 (x5)\n"
			"flw.ps f3,  320 (x5)\n"
			"flw.ps f18,  608 (x5)\n"
			"flw.ps f7,  224 (x5)\n"
			"flw.ps f15,  384 (x5)\n"
			"flw.ps f25,  128 (x5)\n"
			"flw.ps f28,  704 (x5)\n"
			"flw.ps f8,  576 (x5)\n"
			"flw.ps f29,  672 (x5)\n"
			"flw.ps f20,  992 (x5)\n"
			"flw.ps f23,  448 (x5)\n"
			"flw.ps f17,  96 (x5)\n"
			"flw.ps f9,  544 (x5)\n"
			"flw.ps f16,  832 (x5)\n"
			"flw.ps f22,  960 (x5)\n"
			"flw.ps f13,  0 (x5)\n"
			"flw.ps f0,  32 (x5)\n"
			"flw.ps f12,  480 (x5)\n"
			"flw.ps f5,  192 (x5)\n"
			"flw.ps f1,  64 (x5)\n"
			"flw.ps f4,  160 (x5)\n"
			"flw.ps f2,  288 (x5)\n"
			"flw.ps f14,  768 (x5)\n"
			"flw.ps f27,  736 (x5)\n"
			"flw.ps f10,  416 (x5)\n"
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
			"li x31, 100\n"
			"1:\n" 
			"addi x31, x31, -1\n"
			"bne x31, x0, 1b\n" 
			"csrw 0x821, x0\n" 
			"nop\n"          
		       VSNIP_RSV     
		);
		__asm__ __volatile__ ( 
			"nop\n"          
			"LBL_KERNEL_HID_10:\n"          
			"li x5, 0xFEEDFEED\n"
			"slti x0, x0, 0x7f2\n"
			"li x29, 0x6\n"
			"LBL_OUTER_LOOP_SEQ_HID_10:\n"
			"nop \n"
			"nop\n"          
		       VSNIP_RSV     
		);
		__asm__ __volatile__ ( 
			"nop\n"          
			"nop \n"
			"addi x29, x29, -1\n"
			"bne x29, x0, LBL_OUTER_LOOP_SEQ_HID_10\n"
			"LBL_POSTAMBLE_HID_10:\n"          
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
			"csrrci x0, mcache_control, 2\n"
			"csrwi tensor_wait, 0x6\n" 
			"fence\n"
			"csrrci x0, mcache_control, 1\n"
			"csrwi tensor_wait, 0x6\n" 
			"fence\n"
			"csrwi excl_mode, 0\n"
			"nop\n"          
		       VSNIP_RSV     
		);
		__asm__ __volatile__ ( 
			"nop\n"          
			"csrwi excl_mode, 0\n"
			"j LBL_PASS_HID_10\n"          
			VSNIP_RSV
		);
		__asm__ __volatile__ ( 
			"LBL_FAIL_HID_10:\n"          
			".global LBL_FAIL_HID_10\n"          
			VSNIP_RSV
		);
		C_TEST_FAIL;
		__asm__ __volatile__ ( 
			"j LBL_HALT_HID_10\n"          
			VSNIP_RSV
		);
		__asm__ __volatile__ ( 
			"LBL_PASS_HID_10:\n"          
			".global LBL_PASS_HID_10\n"          
			VSNIP_RSV
		);
		__asm__ __volatile__ ( 
			"LBL_HALT_HID_10:\n"          
			".global LBL_HALT_HID_10\n"          
			VSNIP_RSV
		);
	};
	if ( hid == 11 ) { 
		C_TEST_START;
		__asm__ __volatile__ (
			"LBL_PREAMBLE_HID_11:\n"          
			"la x5, LBL_M_MODE_E_H\n"
			"csrw mtvec, x5\n"
			"la x5, LBL_FAIL_HID_11\n"
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
			"csrwi mhpmevent3, 1\n"   
			"csrwi mhpmevent4, 6\n"   
			"csrwi mhpmevent5, 3\n"   
			"csrwi mhpmevent6, 2\n"   
			"csrwi mhpmevent7, 1\n"   
			"csrwi mhpmevent8, 22\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f20,  544 (x5)\n"
			"flw.ps f13,  352 (x5)\n"
			"flw.ps f22,  800 (x5)\n"
			"flw.ps f16,  672 (x5)\n"
			"flw.ps f21,  608 (x5)\n"
			"flw.ps f15,  0 (x5)\n"
			"flw.ps f28,  992 (x5)\n"
			"flw.ps f5,  864 (x5)\n"
			"flw.ps f1,  928 (x5)\n"
			"flw.ps f7,  96 (x5)\n"
			"flw.ps f19,  512 (x5)\n"
			"flw.ps f6,  832 (x5)\n"
			"flw.ps f2,  64 (x5)\n"
			"flw.ps f3,  576 (x5)\n"
			"flw.ps f8,  768 (x5)\n"
			"flw.ps f11,  384 (x5)\n"
			"flw.ps f29,  704 (x5)\n"
			"flw.ps f27,  480 (x5)\n"
			"flw.ps f18,  192 (x5)\n"
			"flw.ps f26,  256 (x5)\n"
			"flw.ps f12,  288 (x5)\n"
			"flw.ps f0,  736 (x5)\n"
			"flw.ps f17,  128 (x5)\n"
			"flw.ps f25,  896 (x5)\n"
			"flw.ps f30,  160 (x5)\n"
			"flw.ps f31,  640 (x5)\n"
			"flw.ps f23,  224 (x5)\n"
			"flw.ps f10,  448 (x5)\n"
			"flw.ps f14,  960 (x5)\n"
			"flw.ps f24,  320 (x5)\n"
			"flw.ps f9,  416 (x5)\n"
			"flw.ps f4,  32 (x5)\n"
			"csrw fflags, zero\n"
			"csrwi frm, 0x0\n"
			"li x31, 100\n"
			"1:\n" 
			"addi x31, x31, -1\n"
			"bne x31, x0, 1b\n" 
			"csrw 0x821, x0\n" 
			"nop\n"          
		       VSNIP_RSV     
		);
		__asm__ __volatile__ ( 
			"nop\n"          
			"LBL_KERNEL_HID_11:\n"          
			"li x5, 0xFEEDFEED\n"
			"slti x0, x0, 0x7f2\n"
			"li x29, 0x6\n"
			"LBL_OUTER_LOOP_SEQ_HID_11:\n"
			"nop \n"
			"nop\n"          
		       VSNIP_RSV     
		);
		__asm__ __volatile__ ( 
			"nop\n"          
			"nop \n"
			"addi x29, x29, -1\n"
			"bne x29, x0, LBL_OUTER_LOOP_SEQ_HID_11\n"
			"LBL_POSTAMBLE_HID_11:\n"          
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
			"nop\n"          
		       VSNIP_RSV     
		);
		__asm__ __volatile__ ( 
			"nop\n"          
			"csrwi excl_mode, 0\n"
			"j LBL_PASS_HID_11\n"          
			VSNIP_RSV
		);
		__asm__ __volatile__ ( 
			"LBL_FAIL_HID_11:\n"          
			".global LBL_FAIL_HID_11\n"          
			VSNIP_RSV
		);
		C_TEST_FAIL;
		__asm__ __volatile__ ( 
			"j LBL_HALT_HID_11\n"          
			VSNIP_RSV
		);
		__asm__ __volatile__ ( 
			"LBL_PASS_HID_11:\n"          
			".global LBL_PASS_HID_11\n"          
			VSNIP_RSV
		);
		__asm__ __volatile__ ( 
			"LBL_HALT_HID_11:\n"          
			".global LBL_HALT_HID_11\n"          
			VSNIP_RSV
		);
	};
	if ( hid == 12 ) { 
		C_TEST_START;
		__asm__ __volatile__ (
			"LBL_PREAMBLE_HID_12:\n"          
			"la x5, LBL_M_MODE_E_H\n"
			"csrw mtvec, x5\n"
			"la x5, LBL_FAIL_HID_12\n"
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
			"csrwi mhpmevent4, 17\n"   
			"csrwi mhpmevent5, 27\n"   
			"csrwi mhpmevent6, 26\n"   
			"csrwi mhpmevent7, 6\n"   
			"csrwi mhpmevent8, 4\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f16,  96 (x5)\n"
			"flw.ps f14,  960 (x5)\n"
			"flw.ps f4,  384 (x5)\n"
			"flw.ps f6,  928 (x5)\n"
			"flw.ps f20,  800 (x5)\n"
			"flw.ps f8,  128 (x5)\n"
			"flw.ps f24,  288 (x5)\n"
			"flw.ps f0,  544 (x5)\n"
			"flw.ps f25,  896 (x5)\n"
			"flw.ps f28,  64 (x5)\n"
			"flw.ps f5,  256 (x5)\n"
			"flw.ps f26,  160 (x5)\n"
			"flw.ps f31,  512 (x5)\n"
			"flw.ps f1,  352 (x5)\n"
			"flw.ps f19,  0 (x5)\n"
			"flw.ps f18,  416 (x5)\n"
			"flw.ps f30,  640 (x5)\n"
			"flw.ps f22,  32 (x5)\n"
			"flw.ps f23,  448 (x5)\n"
			"flw.ps f10,  480 (x5)\n"
			"flw.ps f17,  192 (x5)\n"
			"flw.ps f3,  864 (x5)\n"
			"flw.ps f11,  320 (x5)\n"
			"flw.ps f27,  576 (x5)\n"
			"flw.ps f2,  768 (x5)\n"
			"flw.ps f9,  832 (x5)\n"
			"flw.ps f12,  992 (x5)\n"
			"flw.ps f29,  704 (x5)\n"
			"flw.ps f13,  608 (x5)\n"
			"flw.ps f15,  672 (x5)\n"
			"flw.ps f7,  736 (x5)\n"
			"flw.ps f21,  224 (x5)\n"
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
			"li x31, 100\n"
			"1:\n" 
			"addi x31, x31, -1\n"
			"bne x31, x0, 1b\n" 
			"csrw 0x821, x0\n" 
			"nop\n"          
		       VSNIP_RSV     
		);
		__asm__ __volatile__ ( 
			"nop\n"          
			"LBL_KERNEL_HID_12:\n"          
			"li x5, 0xFEEDFEED\n"
			"slti x0, x0, 0x7f2\n"
			"li x29, 0x6\n"
			"LBL_OUTER_LOOP_SEQ_HID_12:\n"
			"nop \n"
			"nop\n"          
		       VSNIP_RSV     
		);
		__asm__ __volatile__ ( 
			"nop\n"          
			"nop \n"
			"addi x29, x29, -1\n"
			"bne x29, x0, LBL_OUTER_LOOP_SEQ_HID_12\n"
			"LBL_POSTAMBLE_HID_12:\n"          
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
			"nop\n"          
		       VSNIP_RSV     
		);
		__asm__ __volatile__ ( 
			"nop\n"          
			"csrwi excl_mode, 0\n"
			"j LBL_PASS_HID_12\n"          
			VSNIP_RSV
		);
		__asm__ __volatile__ ( 
			"LBL_FAIL_HID_12:\n"          
			".global LBL_FAIL_HID_12\n"          
			VSNIP_RSV
		);
		C_TEST_FAIL;
		__asm__ __volatile__ ( 
			"j LBL_HALT_HID_12\n"          
			VSNIP_RSV
		);
		__asm__ __volatile__ ( 
			"LBL_PASS_HID_12:\n"          
			".global LBL_PASS_HID_12\n"          
			VSNIP_RSV
		);
		__asm__ __volatile__ ( 
			"LBL_HALT_HID_12:\n"          
			".global LBL_HALT_HID_12\n"          
			VSNIP_RSV
		);
	};
	if ( hid == 13 ) { 
		C_TEST_START;
		__asm__ __volatile__ (
			"LBL_PREAMBLE_HID_13:\n"          
			"la x5, LBL_M_MODE_E_H\n"
			"csrw mtvec, x5\n"
			"la x5, LBL_FAIL_HID_13\n"
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
			"csrwi mhpmevent3, 1\n"   
			"csrwi mhpmevent4, 6\n"   
			"csrwi mhpmevent5, 3\n"   
			"csrwi mhpmevent6, 2\n"   
			"csrwi mhpmevent7, 1\n"   
			"csrwi mhpmevent8, 22\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f27,  32 (x5)\n"
			"flw.ps f24,  0 (x5)\n"
			"flw.ps f1,  576 (x5)\n"
			"flw.ps f10,  512 (x5)\n"
			"flw.ps f17,  896 (x5)\n"
			"flw.ps f30,  320 (x5)\n"
			"flw.ps f7,  352 (x5)\n"
			"flw.ps f8,  992 (x5)\n"
			"flw.ps f25,  288 (x5)\n"
			"flw.ps f21,  96 (x5)\n"
			"flw.ps f5,  160 (x5)\n"
			"flw.ps f0,  928 (x5)\n"
			"flw.ps f4,  192 (x5)\n"
			"flw.ps f31,  64 (x5)\n"
			"flw.ps f6,  480 (x5)\n"
			"flw.ps f29,  448 (x5)\n"
			"flw.ps f2,  832 (x5)\n"
			"flw.ps f20,  864 (x5)\n"
			"flw.ps f22,  768 (x5)\n"
			"flw.ps f26,  640 (x5)\n"
			"flw.ps f28,  416 (x5)\n"
			"flw.ps f18,  960 (x5)\n"
			"flw.ps f23,  384 (x5)\n"
			"flw.ps f16,  608 (x5)\n"
			"flw.ps f14,  672 (x5)\n"
			"flw.ps f12,  736 (x5)\n"
			"flw.ps f15,  256 (x5)\n"
			"flw.ps f19,  544 (x5)\n"
			"flw.ps f9,  224 (x5)\n"
			"flw.ps f11,  128 (x5)\n"
			"flw.ps f3,  704 (x5)\n"
			"flw.ps f13,  800 (x5)\n"
			"csrw fflags, zero\n"
			"csrwi frm, 0x0\n"
			"li x31, 100\n"
			"1:\n" 
			"addi x31, x31, -1\n"
			"bne x31, x0, 1b\n" 
			"csrw 0x821, x0\n" 
			"nop\n"          
		       VSNIP_RSV     
		);
		__asm__ __volatile__ ( 
			"nop\n"          
			"LBL_KERNEL_HID_13:\n"          
			"li x5, 0xFEEDFEED\n"
			"slti x0, x0, 0x7f2\n"
			"li x29, 0x6\n"
			"LBL_OUTER_LOOP_SEQ_HID_13:\n"
			"nop \n"
			"nop\n"          
		       VSNIP_RSV     
		);
		__asm__ __volatile__ ( 
			"nop\n"          
			"nop \n"
			"addi x29, x29, -1\n"
			"bne x29, x0, LBL_OUTER_LOOP_SEQ_HID_13\n"
			"LBL_POSTAMBLE_HID_13:\n"          
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
			"nop\n"          
		       VSNIP_RSV     
		);
		__asm__ __volatile__ ( 
			"nop\n"          
			"csrwi excl_mode, 0\n"
			"j LBL_PASS_HID_13\n"          
			VSNIP_RSV
		);
		__asm__ __volatile__ ( 
			"LBL_FAIL_HID_13:\n"          
			".global LBL_FAIL_HID_13\n"          
			VSNIP_RSV
		);
		C_TEST_FAIL;
		__asm__ __volatile__ ( 
			"j LBL_HALT_HID_13\n"          
			VSNIP_RSV
		);
		__asm__ __volatile__ ( 
			"LBL_PASS_HID_13:\n"          
			".global LBL_PASS_HID_13\n"          
			VSNIP_RSV
		);
		__asm__ __volatile__ ( 
			"LBL_HALT_HID_13:\n"          
			".global LBL_HALT_HID_13\n"          
			VSNIP_RSV
		);
	};
	if ( hid == 14 ) { 
		C_TEST_START;
		__asm__ __volatile__ (
			"LBL_PREAMBLE_HID_14:\n"          
			"la x5, LBL_M_MODE_E_H\n"
			"csrw mtvec, x5\n"
			"la x5, LBL_FAIL_HID_14\n"
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
			"csrwi mhpmevent4, 17\n"   
			"csrwi mhpmevent5, 27\n"   
			"csrwi mhpmevent6, 26\n"   
			"csrwi mhpmevent7, 6\n"   
			"csrwi mhpmevent8, 4\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f14,  352 (x5)\n"
			"flw.ps f29,  480 (x5)\n"
			"flw.ps f9,  960 (x5)\n"
			"flw.ps f5,  192 (x5)\n"
			"flw.ps f18,  992 (x5)\n"
			"flw.ps f8,  160 (x5)\n"
			"flw.ps f30,  608 (x5)\n"
			"flw.ps f27,  288 (x5)\n"
			"flw.ps f21,  768 (x5)\n"
			"flw.ps f12,  928 (x5)\n"
			"flw.ps f10,  640 (x5)\n"
			"flw.ps f28,  416 (x5)\n"
			"flw.ps f20,  896 (x5)\n"
			"flw.ps f13,  448 (x5)\n"
			"flw.ps f6,  736 (x5)\n"
			"flw.ps f23,  672 (x5)\n"
			"flw.ps f25,  256 (x5)\n"
			"flw.ps f7,  512 (x5)\n"
			"flw.ps f24,  224 (x5)\n"
			"flw.ps f26,  544 (x5)\n"
			"flw.ps f4,  32 (x5)\n"
			"flw.ps f2,  64 (x5)\n"
			"flw.ps f22,  96 (x5)\n"
			"flw.ps f17,  864 (x5)\n"
			"flw.ps f31,  800 (x5)\n"
			"flw.ps f19,  128 (x5)\n"
			"flw.ps f15,  384 (x5)\n"
			"flw.ps f0,  320 (x5)\n"
			"flw.ps f3,  0 (x5)\n"
			"flw.ps f1,  704 (x5)\n"
			"flw.ps f11,  576 (x5)\n"
			"flw.ps f16,  832 (x5)\n"
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
			"li x31, 100\n"
			"1:\n" 
			"addi x31, x31, -1\n"
			"bne x31, x0, 1b\n" 
			"csrw 0x821, x0\n" 
			"nop\n"          
		       VSNIP_RSV     
		);
		__asm__ __volatile__ ( 
			"nop\n"          
			"LBL_KERNEL_HID_14:\n"          
			"li x5, 0xFEEDFEED\n"
			"slti x0, x0, 0x7f2\n"
			"li x29, 0x6\n"
			"LBL_OUTER_LOOP_SEQ_HID_14:\n"
			"nop \n"
			"nop\n"          
		       VSNIP_RSV     
		);
		__asm__ __volatile__ ( 
			"nop\n"          
			"nop \n"
			"addi x29, x29, -1\n"
			"bne x29, x0, LBL_OUTER_LOOP_SEQ_HID_14\n"
			"LBL_POSTAMBLE_HID_14:\n"          
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
			"csrrci x0, mcache_control, 2\n"
			"csrwi tensor_wait, 0x6\n" 
			"fence\n"
			"csrrci x0, mcache_control, 1\n"
			"csrwi tensor_wait, 0x6\n" 
			"fence\n"
			"csrwi excl_mode, 0\n"
			"nop\n"          
		       VSNIP_RSV     
		);
		__asm__ __volatile__ ( 
			"nop\n"          
			"csrwi excl_mode, 0\n"
			"j LBL_PASS_HID_14\n"          
			VSNIP_RSV
		);
		__asm__ __volatile__ ( 
			"LBL_FAIL_HID_14:\n"          
			".global LBL_FAIL_HID_14\n"          
			VSNIP_RSV
		);
		C_TEST_FAIL;
		__asm__ __volatile__ ( 
			"j LBL_HALT_HID_14\n"          
			VSNIP_RSV
		);
		__asm__ __volatile__ ( 
			"LBL_PASS_HID_14:\n"          
			".global LBL_PASS_HID_14\n"          
			VSNIP_RSV
		);
		__asm__ __volatile__ ( 
			"LBL_HALT_HID_14:\n"          
			".global LBL_HALT_HID_14\n"          
			VSNIP_RSV
		);
	};
	if ( hid == 15 ) { 
		C_TEST_START;
		__asm__ __volatile__ (
			"LBL_PREAMBLE_HID_15:\n"          
			"la x5, LBL_M_MODE_E_H\n"
			"csrw mtvec, x5\n"
			"la x5, LBL_FAIL_HID_15\n"
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
			"csrwi mhpmevent3, 1\n"   
			"csrwi mhpmevent4, 6\n"   
			"csrwi mhpmevent5, 3\n"   
			"csrwi mhpmevent6, 2\n"   
			"csrwi mhpmevent7, 1\n"   
			"csrwi mhpmevent8, 22\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f9,  672 (x5)\n"
			"flw.ps f10,  576 (x5)\n"
			"flw.ps f3,  416 (x5)\n"
			"flw.ps f5,  480 (x5)\n"
			"flw.ps f14,  128 (x5)\n"
			"flw.ps f1,  896 (x5)\n"
			"flw.ps f20,  224 (x5)\n"
			"flw.ps f27,  800 (x5)\n"
			"flw.ps f6,  512 (x5)\n"
			"flw.ps f12,  544 (x5)\n"
			"flw.ps f30,  992 (x5)\n"
			"flw.ps f7,  704 (x5)\n"
			"flw.ps f23,  864 (x5)\n"
			"flw.ps f26,  288 (x5)\n"
			"flw.ps f13,  640 (x5)\n"
			"flw.ps f19,  384 (x5)\n"
			"flw.ps f21,  96 (x5)\n"
			"flw.ps f24,  736 (x5)\n"
			"flw.ps f28,  0 (x5)\n"
			"flw.ps f2,  320 (x5)\n"
			"flw.ps f29,  928 (x5)\n"
			"flw.ps f16,  960 (x5)\n"
			"flw.ps f25,  256 (x5)\n"
			"flw.ps f22,  32 (x5)\n"
			"flw.ps f4,  608 (x5)\n"
			"flw.ps f8,  64 (x5)\n"
			"flw.ps f31,  768 (x5)\n"
			"flw.ps f11,  832 (x5)\n"
			"flw.ps f15,  352 (x5)\n"
			"flw.ps f0,  448 (x5)\n"
			"flw.ps f17,  192 (x5)\n"
			"flw.ps f18,  160 (x5)\n"
			"csrw fflags, zero\n"
			"csrwi frm, 0x0\n"
			"li x31, 100\n"
			"1:\n" 
			"addi x31, x31, -1\n"
			"bne x31, x0, 1b\n" 
			"csrw 0x821, x0\n" 
			"nop\n"          
		       VSNIP_RSV     
		);
		__asm__ __volatile__ ( 
			"nop\n"          
			"LBL_KERNEL_HID_15:\n"          
			"li x5, 0xFEEDFEED\n"
			"slti x0, x0, 0x7f2\n"
			"li x29, 0x6\n"
			"LBL_OUTER_LOOP_SEQ_HID_15:\n"
			"nop \n"
			"nop\n"          
		       VSNIP_RSV     
		);
		__asm__ __volatile__ ( 
			"nop\n"          
			"nop \n"
			"addi x29, x29, -1\n"
			"bne x29, x0, LBL_OUTER_LOOP_SEQ_HID_15\n"
			"LBL_POSTAMBLE_HID_15:\n"          
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
			"nop\n"          
		       VSNIP_RSV     
		);
		__asm__ __volatile__ ( 
			"nop\n"          
			"csrwi excl_mode, 0\n"
			"j LBL_PASS_HID_15\n"          
			VSNIP_RSV
		);
		__asm__ __volatile__ ( 
			"LBL_FAIL_HID_15:\n"          
			".global LBL_FAIL_HID_15\n"          
			VSNIP_RSV
		);
		C_TEST_FAIL;
		__asm__ __volatile__ ( 
			"j LBL_HALT_HID_15\n"          
			VSNIP_RSV
		);
		__asm__ __volatile__ ( 
			"LBL_PASS_HID_15:\n"          
			".global LBL_PASS_HID_15\n"          
			VSNIP_RSV
		);
		__asm__ __volatile__ ( 
			"LBL_HALT_HID_15:\n"          
			".global LBL_HALT_HID_15\n"          
			VSNIP_RSV
		);
	};
		C_TEST_PASS;
		return 0;       
}
volatile uint32_t rand_int_32[256] __attribute__ ((aligned (2048))) = {
		0x4f4099ac, 
		0xd32b273c, 
		0x1002336e, 
		0x6d1ae1ec, 
		0x2b04f39e, 
		0xc727318d, 
		0x2015937d, 
		0xd8089138, 
		0x35a3fb53, 
		0x92fd7cd2, 
		0xca8f1cd3, 
		0x54e2049d, 
		0xd7163580, 
		0x65945cc4, 
		0x22941b8e, 
		0x283aeb1e, 
		0xe06ac292, 
		0x3ca94ebf, 
		0x42b71e3d, 
		0xb9165217, 
		0x31de6259, 
		0x480655ba, 
		0xa286ac2f, 
		0xf5b58d05, 
		0xad4c3cb3, 
		0x6f055f5b, 
		0x564fddf9, 
		0x6fb063ad, 
		0xaa6f8c95, 
		0x20d053f2, 
		0xd4798417, 
		0x8893d0a7, 
		0x9b3febc5, 
		0x384019fd, 
		0x5c916911, 
		0x3b60bcb5, 
		0x138131f5, 
		0x59419798, 
		0x445aa364, 
		0x2280c5fa, 
		0x9b526523, 
		0xbdb31821, 
		0x2ff70782, 
		0x02a69db5, 
		0xc968a574, 
		0xbb14d323, 
		0x4c61575e, 
		0xcfa2ed89, 
		0xee11c099, 
		0x75fb0eeb, 
		0x90080611, 
		0x947b75f1, 
		0xf82e4ffc, 
		0x3855c073, 
		0xb01fb502, 
		0xcd5e4d5b, 
		0x2c26b780, 
		0xb682083a, 
		0xaa2d9498, 
		0x7988e782, 
		0xc96b6ce2, 
		0x87a27d2b, 
		0x19e1c6a7, 
		0x21310897, 
		0x71ffd862, 
		0xb2713969, 
		0x37b514c7, 
		0x64d992dd, 
		0xbfd7d3e7, 
		0x1978ca3d, 
		0x5e8b2594, 
		0xfeabea78, 
		0x79e9827e, 
		0x6d921c1f, 
		0xbbd2e6fc, 
		0xcf597727, 
		0xfb9d09e1, 
		0xaf02c57e, 
		0xadad7652, 
		0xf00ac8dd, 
		0xaa8fc641, 
		0xcc7bc37c, 
		0xaaadc8b7, 
		0x19c7d5cf, 
		0x72b2330e, 
		0x5db068c3, 
		0x832f2fbe, 
		0xe4871a02, 
		0xfec4757a, 
		0x875acfe0, 
		0x9d2a2315, 
		0xcb1fd0d5, 
		0xde80019a, 
		0x8695e79d, 
		0xb678e557, 
		0x4aa51583, 
		0xcee4d9dc, 
		0xa4334061, 
		0x82ca7984, 
		0x4c762fe3, 
		0x8e6b5085, 
		0xf5a8ff2c, 
		0x2cda0e99, 
		0xfc4701d4, 
		0x20cf5f79, 
		0xdb31b696, 
		0x4dec4e43, 
		0x8fa5c0a3, 
		0x8078c3fc, 
		0xb1dbbd08, 
		0x3e6beb8b, 
		0xe05649c8, 
		0xafd988e9, 
		0x57f140ad, 
		0x95439034, 
		0x611cd8f7, 
		0xae214295, 
		0xf4c08523, 
		0x74ab0e17, 
		0xb37e9f02, 
		0x7efe7937, 
		0x05b49020, 
		0xd1a41cd5, 
		0x82710fa8, 
		0x57fe832e, 
		0x3bc10006, 
		0xc748d98c, 
		0xc7b53163, 
		0xb8a9f384, 
		0xc818d50b, 
		0x7d781d34, 
		0xf12e5fa9, 
		0x52b4c18c, 
		0xc98cec1b, 
		0xacb3a362, 
		0xdec24a2b, 
		0x88009f78, 
		0xd884bfd2, 
		0x30502ae2, 
		0x93b8f0e4, 
		0x03db1506, 
		0xee97f970, 
		0x47d25ae2, 
		0x06c32e88, 
		0x819a38d2, 
		0xf237b193, 
		0x20bd81be, 
		0xebd5f759, 
		0x7f52d25d, 
		0x842fbfa6, 
		0x73538ebf, 
		0x3e1daad3, 
		0x464e8eff, 
		0x769c5269, 
		0x203e0511, 
		0xdeb5a555, 
		0xa83c17a0, 
		0xecd155c0, 
		0x4b9e8a3f, 
		0x8196b064, 
		0x938254f0, 
		0x6e96f23b, 
		0xaaaba6ce, 
		0x94b0c25f, 
		0x249b678a, 
		0x2da9036f, 
		0x489634fa, 
		0xe5c7d3a8, 
		0xd9c154fd, 
		0x642d5919, 
		0xef31ed9c, 
		0xd858847f, 
		0xaf72fa01, 
		0x032ad9bf, 
		0x53f81e12, 
		0x87a4f148, 
		0x901a6d8b, 
		0xf9fbbb80, 
		0x50ed4e0a, 
		0x9e08e9b9, 
		0x9ad747af, 
		0x42985a0f, 
		0x2d356132, 
		0x924c43c8, 
		0x6229a2ef, 
		0x382282be, 
		0x9d219757, 
		0x666d1039, 
		0x2ed72480, 
		0xd14b55fb, 
		0x8bfdb59f, 
		0xf8b5034b, 
		0x270b3489, 
		0x7a9b3c80, 
		0xd874efcc, 
		0xac5fe8c6, 
		0x7cf701a3, 
		0x77fe05b6, 
		0x913a3504, 
		0xef476959, 
		0x09d2bc83, 
		0x8130f54a, 
		0x9007926f, 
		0xaa0f9292, 
		0x503261e4, 
		0x87d0b65f, 
		0x9aa2ed52, 
		0xf230961c, 
		0x985a50ea, 
		0x89fed741, 
		0xfc6a3458, 
		0x2e741328, 
		0x98946eac, 
		0x688ce97b, 
		0x2493ffd1, 
		0x68331ba0, 
		0xc63e6551, 
		0x1366a777, 
		0x0035504c, 
		0x1ae78d1b, 
		0x3bb8cad3, 
		0x0b564cfd, 
		0x133ddb4b, 
		0x0d7d5edc, 
		0xd207984d, 
		0x522051e3, 
		0xbdc0be6c, 
		0x4b76b411, 
		0x569fb055, 
		0xf1a1326d, 
		0x76329e50, 
		0x9441901c, 
		0x71784b49, 
		0x379cacaf, 
		0x5c740e4a, 
		0x6fa21286, 
		0x56ce14d2, 
		0x2b26da88, 
		0x87d743b3, 
		0xa1110e7f, 
		0x1986fa09, 
		0x5b33a6ba, 
		0x51bf572a, 
		0xadd0b51c, 
		0x04af646a, 
		0x12e83acd, 
		0x0ceb02e0, 
		0x64720003, 
		0xbc5514cd, 
		0x7376b501, 
		0x30636b00, 
		0x37abba13, 
		0xe2566060, 
		0x0fa94bf5, 
		0xda857671, 
		0x1d3f4c37  
};
volatile uint32_t id_int_32[256] __attribute__ ((aligned (2048))) = {
		0x00000000, 
		0x00000001, 
		0x00000002, 
		0x00000003, 
		0x00000004, 
		0x00000005, 
		0x00000006, 
		0x00000007, 
		0x00000008, 
		0x00000009, 
		0x0000000a, 
		0x0000000b, 
		0x0000000c, 
		0x0000000d, 
		0x0000000e, 
		0x0000000f, 
		0x00000010, 
		0x00000011, 
		0x00000012, 
		0x00000013, 
		0x00000014, 
		0x00000015, 
		0x00000016, 
		0x00000017, 
		0x00000018, 
		0x00000019, 
		0x0000001a, 
		0x0000001b, 
		0x0000001c, 
		0x0000001d, 
		0x0000001e, 
		0x0000001f, 
		0x00000020, 
		0x00000021, 
		0x00000022, 
		0x00000023, 
		0x00000024, 
		0x00000025, 
		0x00000026, 
		0x00000027, 
		0x00000028, 
		0x00000029, 
		0x0000002a, 
		0x0000002b, 
		0x0000002c, 
		0x0000002d, 
		0x0000002e, 
		0x0000002f, 
		0x00000030, 
		0x00000031, 
		0x00000032, 
		0x00000033, 
		0x00000034, 
		0x00000035, 
		0x00000036, 
		0x00000037, 
		0x00000038, 
		0x00000039, 
		0x0000003a, 
		0x0000003b, 
		0x0000003c, 
		0x0000003d, 
		0x0000003e, 
		0x0000003f, 
		0x00000040, 
		0x00000041, 
		0x00000042, 
		0x00000043, 
		0x00000044, 
		0x00000045, 
		0x00000046, 
		0x00000047, 
		0x00000048, 
		0x00000049, 
		0x0000004a, 
		0x0000004b, 
		0x0000004c, 
		0x0000004d, 
		0x0000004e, 
		0x0000004f, 
		0x00000050, 
		0x00000051, 
		0x00000052, 
		0x00000053, 
		0x00000054, 
		0x00000055, 
		0x00000056, 
		0x00000057, 
		0x00000058, 
		0x00000059, 
		0x0000005a, 
		0x0000005b, 
		0x0000005c, 
		0x0000005d, 
		0x0000005e, 
		0x0000005f, 
		0x00000060, 
		0x00000061, 
		0x00000062, 
		0x00000063, 
		0x00000064, 
		0x00000065, 
		0x00000066, 
		0x00000067, 
		0x00000068, 
		0x00000069, 
		0x0000006a, 
		0x0000006b, 
		0x0000006c, 
		0x0000006d, 
		0x0000006e, 
		0x0000006f, 
		0x00000070, 
		0x00000071, 
		0x00000072, 
		0x00000073, 
		0x00000074, 
		0x00000075, 
		0x00000076, 
		0x00000077, 
		0x00000078, 
		0x00000079, 
		0x0000007a, 
		0x0000007b, 
		0x0000007c, 
		0x0000007d, 
		0x0000007e, 
		0x0000007f, 
		0x00000080, 
		0x00000081, 
		0x00000082, 
		0x00000083, 
		0x00000084, 
		0x00000085, 
		0x00000086, 
		0x00000087, 
		0x00000088, 
		0x00000089, 
		0x0000008a, 
		0x0000008b, 
		0x0000008c, 
		0x0000008d, 
		0x0000008e, 
		0x0000008f, 
		0x00000090, 
		0x00000091, 
		0x00000092, 
		0x00000093, 
		0x00000094, 
		0x00000095, 
		0x00000096, 
		0x00000097, 
		0x00000098, 
		0x00000099, 
		0x0000009a, 
		0x0000009b, 
		0x0000009c, 
		0x0000009d, 
		0x0000009e, 
		0x0000009f, 
		0x000000a0, 
		0x000000a1, 
		0x000000a2, 
		0x000000a3, 
		0x000000a4, 
		0x000000a5, 
		0x000000a6, 
		0x000000a7, 
		0x000000a8, 
		0x000000a9, 
		0x000000aa, 
		0x000000ab, 
		0x000000ac, 
		0x000000ad, 
		0x000000ae, 
		0x000000af, 
		0x000000b0, 
		0x000000b1, 
		0x000000b2, 
		0x000000b3, 
		0x000000b4, 
		0x000000b5, 
		0x000000b6, 
		0x000000b7, 
		0x000000b8, 
		0x000000b9, 
		0x000000ba, 
		0x000000bb, 
		0x000000bc, 
		0x000000bd, 
		0x000000be, 
		0x000000bf, 
		0x000000c0, 
		0x000000c1, 
		0x000000c2, 
		0x000000c3, 
		0x000000c4, 
		0x000000c5, 
		0x000000c6, 
		0x000000c7, 
		0x000000c8, 
		0x000000c9, 
		0x000000ca, 
		0x000000cb, 
		0x000000cc, 
		0x000000cd, 
		0x000000ce, 
		0x000000cf, 
		0x000000d0, 
		0x000000d1, 
		0x000000d2, 
		0x000000d3, 
		0x000000d4, 
		0x000000d5, 
		0x000000d6, 
		0x000000d7, 
		0x000000d8, 
		0x000000d9, 
		0x000000da, 
		0x000000db, 
		0x000000dc, 
		0x000000dd, 
		0x000000de, 
		0x000000df, 
		0x000000e0, 
		0x000000e1, 
		0x000000e2, 
		0x000000e3, 
		0x000000e4, 
		0x000000e5, 
		0x000000e6, 
		0x000000e7, 
		0x000000e8, 
		0x000000e9, 
		0x000000ea, 
		0x000000eb, 
		0x000000ec, 
		0x000000ed, 
		0x000000ee, 
		0x000000ef, 
		0x000000f0, 
		0x000000f1, 
		0x000000f2, 
		0x000000f3, 
		0x000000f4, 
		0x000000f5, 
		0x000000f6, 
		0x000000f7, 
		0x000000f8, 
		0x000000f9, 
		0x000000fa, 
		0x000000fb, 
		0x000000fc, 
		0x000000fd, 
		0x000000fe, 
		0x000000ff  
};
volatile uint32_t constants_int_32[256] __attribute__ ((aligned (2048))) = {
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008, 
		0x00000008  
};
volatile uint16_t rand_ieee754_16[1536] __attribute__ ((aligned (2048))) = {
		0xc500, 
		0x375d, 
		0x2b3d, 
		0x4098, 
		0xcfdd, 
		0xcc40, 
		0xd280, 
		0x4f40, 
		0xe58a, 
		0x1714, 
		0x2fa5, 
		0x0000, 
		0xc182, 
		0x4400, 
		0x52e3, 
		0xcf00, 
		0xc800, 
		0x8000, 
		0xd300, 
		0xc980, 
		0x3c00, 
		0xcf40, 
		0x8000, 
		0xc880, 
		0x1ba5, 
		0x0000, 
		0xcec0, 
		0x35bf, 
		0x0000, 
		0x028f, 
		0xc980, 
		0xbab0, 
		0x4d00, 
		0xd79f, 
		0xcec0, 
		0xd3c0, 
		0x0000, 
		0xcc00, 
		0x88a1, 
		0x5380, 
		0x5380, 
		0xc500, 
		0xccc0, 
		0x5200, 
		0xd040, 
		0xcf40, 
		0x4000, 
		0xcc80, 
		0x0000, 
		0xd160, 
		0x50c0, 
		0xd300, 
		0x4d80, 
		0x8029, 
		0x9a0a, 
		0xd1c0, 
		0xca80, 
		0x9731, 
		0xcec0, 
		0x4b00, 
		0x7d60, 
		0xccc0, 
		0xc74f, 
		0x6516, 
		0xd080, 
		0x35cc, 
		0x4b80, 
		0xa2c5, 
		0x52c0, 
		0x18da, 
		0x0000, 
		0xcf40, 
		0x4700, 
		0xd060, 
		0xf4fc, 
		0xca00, 
		0xd0a0, 
		0x51e0, 
		0xb2d1, 
		0x26ec, 
		0x3e79, 
		0xd3a1, 
		0xd1c0, 
		0xd340, 
		0x271e, 
		0x92e5, 
		0x0000, 
		0xcf00, 
		0xca00, 
		0xfc00, 
		0x2c79, 
		0x4797, 
		0x3004, 
		0xce80, 
		0x4880, 
		0xce40, 
		0x3c00, 
		0x5160, 
		0xd300, 
		0x4c00, 
		0x0c97, 
		0xfc66, 
		0x5240, 
		0x4c80, 
		0x7aed, 
		0x8000, 
		0x005c, 
		0xd320, 
		0x8000, 
		0xbca8, 
		0x30a1, 
		0x8000, 
		0xcc80, 
		0x4c40, 
		0x8000, 
		0xb3b4, 
		0xcf80, 
		0xcc80, 
		0xd060, 
		0xd240, 
		0xce40, 
		0x5360, 
		0x51c0, 
		0xcc00, 
		0xd3f7, 
		0x2c34, 
		0x8834, 
		0x4c80, 
		0x23f9, 
		0xd2e0, 
		0x8222, 
		0xce40, 
		0xa158, 
		0x76ed, 
		0x70e3, 
		0x36b8, 
		0xdec0, 
		0x5320, 
		0xc200, 
		0xd1c0, 
		0x8000, 
		0x0000, 
		0x52e0, 
		0x4c40, 
		0xfc00, 
		0x8000, 
		0x4c00, 
		0xd0c0, 
		0xd3e0, 
		0x4500, 
		0x50c0, 
		0x4200, 
		0x5200, 
		0x51a0, 
		0xc600, 
		0xd3e0, 
		0x4c80, 
		0xd280, 
		0xc400, 
		0xc900, 
		0xc471, 
		0x7c00, 
		0x0000, 
		0x4e00, 
		0x8000, 
		0xd080, 
		0xa55d, 
		0x4980, 
		0x8edf, 
		0xc980, 
		0xc2fc, 
		0xd941, 
		0x17bf, 
		0xd080, 
		0x53a0, 
		0x2c06, 
		0x5280, 
		0xd140, 
		0x8000, 
		0x94af, 
		0xd220, 
		0x51a0, 
		0xca80, 
		0x6794, 
		0x5320, 
		0x84f4, 
		0xd1c0, 
		0xd020, 
		0xef97, 
		0x8000, 
		0xcfc0, 
		0xca00, 
		0x5000, 
		0xceeb, 
		0x8000, 
		0x632b, 
		0x5020, 
		0x5100, 
		0xd220, 
		0xa1a4, 
		0x3ea9, 
		0xccc0, 
		0x6154, 
		0xc900, 
		0x063f, 
		0xd140, 
		0x77a7, 
		0x7c6c, 
		0x5260, 
		0x2ae4, 
		0xcf00, 
		0x50c0, 
		0xc880, 
		0x4d80, 
		0x25f9, 
		0xcd80, 
		0xc4cd, 
		0xb8f7, 
		0x1cbb, 
		0x5320, 
		0x8000, 
		0x4d00, 
		0xa60d, 
		0x15b8, 
		0x4dc0, 
		0xd000, 
		0x53a0, 
		0x4926, 
		0x4400, 
		0x50c0, 
		0x8000, 
		0x51e0, 
		0xfc00, 
		0xc980, 
		0xcb80, 
		0x684c, 
		0xd000, 
		0xc2b4, 
		0xd1e0, 
		0xd260, 
		0x4f40, 
		0xfc00, 
		0xd3e0, 
		0x4880, 
		0x53a0, 
		0xd9de, 
		0xd3c0, 
		0x4cc0, 
		0x5240, 
		0xcf00, 
		0x7c00, 
		0xd140, 
		0xfd4b, 
		0x4c00, 
		0x7c03, 
		0xc2c3, 
		0xc900, 
		0xcfc0, 
		0xccc0, 
		0x0000, 
		0x4a00, 
		0x53e0, 
		0xd0c0, 
		0xaabd, 
		0x4c40, 
		0xcc00, 
		0x69fa, 
		0xca48, 
		0x5300, 
		0xa6a9, 
		0x7c00, 
		0x6b0d, 
		0x5000, 
		0xc700, 
		0x4400, 
		0x5060, 
		0xcd80, 
		0x4ec0, 
		0xe660, 
		0x035b, 
		0xd280, 
		0xcd00, 
		0x47f5, 
		0x0000, 
		0xc900, 
		0xcd00, 
		0x7e2d, 
		0x52a0, 
		0xf54d, 
		0x0358, 
		0xd280, 
		0x4800, 
		0xc900, 
		0xc700, 
		0xf6e5, 
		0x4900, 
		0x5360, 
		0x8000, 
		0xd3e0, 
		0x0d50, 
		0x5120, 
		0x4000, 
		0x50c0, 
		0xc9e3, 
		0x0000, 
		0x51e0, 
		0x9a22, 
		0x53e0, 
		0xc5f5, 
		0x8e16, 
		0x7c00, 
		0x5000, 
		0xce80, 
		0xc900, 
		0x4800, 
		0xd220, 
		0x8e36, 
		0xcf00, 
		0x0000, 
		0x0a14, 
		0xa727, 
		0x7f7f, 
		0xec83, 
		0xcf40, 
		0xca80, 
		0x5c13, 
		0x4dc0, 
		0xabe4, 
		0x53c0, 
		0xe8a8, 
		0xc880, 
		0x0000, 
		0x4fc0, 
		0xd160, 
		0xd320, 
		0x4e00, 
		0xac6e, 
		0xfaf8, 
		0x5020, 
		0x62aa, 
		0xee4c, 
		0x7da9, 
		0x0000, 
		0x4f80, 
		0xed95, 
		0x0000, 
		0x5160, 
		0x0000, 
		0x5380, 
		0x50c0, 
		0xd1a0, 
		0xd360, 
		0xfc00, 
		0x4200, 
		0x50a0, 
		0x4dc0, 
		0x4d80, 
		0x0000, 
		0xd2e0, 
		0x7c00, 
		0x4f40, 
		0x4f00, 
		0x8000, 
		0x8000, 
		0xd0e0, 
		0x6682, 
		0x8000, 
		0x7a53, 
		0x53c0, 
		0x4f00, 
		0x981b, 
		0x4900, 
		0xfc00, 
		0x4fc0, 
		0x4ec0, 
		0x4cc0, 
		0x4f40, 
		0x4c00, 
		0x005e, 
		0xd3a0, 
		0x5320, 
		0x8000, 
		0x0000, 
		0x4b00, 
		0x7d4e, 
		0xd100, 
		0xe6be, 
		0xc880, 
		0x8e1e, 
		0x6c73, 
		0x4c80, 
		0x2f28, 
		0x0000, 
		0xc500, 
		0x4d00, 
		0xe4a5, 
		0x0855, 
		0x9e99, 
		0x5220, 
		0x51a0, 
		0xcd40, 
		0x89d5, 
		0xfc00, 
		0xb807, 
		0xd2c0, 
		0x50c0, 
		0xbc00, 
		0x8da5, 
		0xcdc0, 
		0xd280, 
		0xa6e0, 
		0xa22b, 
		0x3677, 
		0x4c00, 
		0x5020, 
		0x4a80, 
		0x53a0, 
		0xd180, 
		0xd380, 
		0xd1a0, 
		0xcf40, 
		0x0000, 
		0x59fc, 
		0x7c00, 
		0x772f, 
		0x6f30, 
		0x37fe, 
		0xcb80, 
		0xc600, 
		0x4b80, 
		0x6474, 
		0xbdb0, 
		0x4f80, 
		0x7fb6, 
		0x4f40, 
		0x5260, 
		0xd200, 
		0x66c6, 
		0xcf80, 
		0xca00, 
		0x5340, 
		0xd1e0, 
		0x7883, 
		0xd000, 
		0xf638, 
		0xce00, 
		0x8000, 
		0x4cc0, 
		0x7a3e, 
		0x5080, 
		0x5260, 
		0x9f87, 
		0xcfc0, 
		0xcc80, 
		0x5380, 
		0xd1a0, 
		0xd1e0, 
		0x7d2a, 
		0xf75e, 
		0xc4fd, 
		0xd000, 
		0x4400, 
		0xd3c0, 
		0x281e, 
		0x4dc0, 
		0x4880, 
		0x5f11, 
		0x0e6a, 
		0xce80, 
		0xcfc0, 
		0x0000, 
		0x81f5, 
		0x6f12, 
		0xbc00, 
		0x4600, 
		0xefe8, 
		0xd7e9, 
		0xf65f, 
		0x4b00, 
		0x0000, 
		0xfbc4, 
		0x4f80, 
		0x50e0, 
		0x5180, 
		0x4200, 
		0x8000, 
		0x5240, 
		0x4f00, 
		0xc600, 
		0x5120, 
		0x4200, 
		0x4f80, 
		0x4800, 
		0x8000, 
		0xd220, 
		0x51a0, 
		0xc500, 
		0xe895, 
		0x4880, 
		0xbae1, 
		0x5080, 
		0x8429, 
		0x178c, 
		0xfc00, 
		0x4b80, 
		0xce00, 
		0xaaf8, 
		0x4200, 
		0x15ae, 
		0xd160, 
		0xc32c, 
		0x8321, 
		0xd35e, 
		0xcd40, 
		0xd10c, 
		0x8000, 
		0x4400, 
		0x0000, 
		0x4c00, 
		0xd280, 
		0x5240, 
		0x5360, 
		0x4d00, 
		0x4700, 
		0xd080, 
		0xcfc0, 
		0xd3a0, 
		0x4c80, 
		0x5320, 
		0x4bc7, 
		0xf4e7, 
		0x5360, 
		0xce3b, 
		0x261b, 
		0xd3a0, 
		0xcc80, 
		0x1508, 
		0x79e4, 
		0x53a0, 
		0xd060, 
		0x0389, 
		0x4b4a, 
		0x8d74, 
		0xcec0, 
		0xd1a0, 
		0xc500, 
		0x8000, 
		0x1fac, 
		0xd1e0, 
		0x4f7d, 
		0x4900, 
		0x52e0, 
		0xd0a0, 
		0x5300, 
		0xd140, 
		0x4a80, 
		0x4cc0, 
		0xcd00, 
		0xdd8a, 
		0x0000, 
		0xc500, 
		0x5060, 
		0x4b00, 
		0x7c00, 
		0xc0b4, 
		0x5080, 
		0x4dc0, 
		0x8ec3, 
		0xce40, 
		0x6b69, 
		0x4e40, 
		0xd280, 
		0x8000, 
		0x2345, 
		0xcc40, 
		0x4b00, 
		0x4a80, 
		0xd3c0, 
		0xa224, 
		0x268d, 
		0x50c0, 
		0xca80, 
		0x51e0, 
		0xcf00, 
		0x5040, 
		0xca00, 
		0x0000, 
		0x2c71, 
		0x58bd, 
		0xce00, 
		0xf388, 
		0x2444, 
		0xcd00, 
		0xcf64, 
		0x4400, 
		0xa0b0, 
		0xcf00, 
		0x4600, 
		0x6128, 
		0xb7fd, 
		0xf989, 
		0x5000, 
		0xd000, 
		0xcd40, 
		0x5080, 
		0xd360, 
		0xa55b, 
		0x4a00, 
		0xd0a0, 
		0x3719, 
		0x6d51, 
		0xcc00, 
		0x30c5, 
		0x5060, 
		0x84d1, 
		0x3ad7, 
		0xdeb6, 
		0x838b, 
		0xc500, 
		0x5320, 
		0xe814, 
		0xf18a, 
		0x8000, 
		0x7aae, 
		0xb2ff, 
		0xcc00, 
		0x4000, 
		0xd260, 
		0x4c00, 
		0xc200, 
		0x11fe, 
		0xc200, 
		0xa951, 
		0x7c00, 
		0xc200, 
		0xe517, 
		0x4400, 
		0x4e40, 
		0x4dc0, 
		0x4f40, 
		0x0c52, 
		0xd180, 
		0xdcb8, 
		0xd180, 
		0x7a53, 
		0x53ee, 
		0x53e7, 
		0x4cc0, 
		0x4d40, 
		0xfc85, 
		0x4f00, 
		0x929f, 
		0xd200, 
		0x53c0, 
		0xaa41, 
		0x5180, 
		0x4ec0, 
		0x52e0, 
		0xfc00, 
		0x4c40, 
		0x4fc0, 
		0xc700, 
		0xca80, 
		0xcd40, 
		0xcd40, 
		0x5320, 
		0x4b80, 
		0xc400, 
		0x9f13, 
		0xcc40, 
		0x5180, 
		0xc700, 
		0xcfc0, 
		0x8000, 
		0x5020, 
		0x5300, 
		0x526a, 
		0x00b3, 
		0x0621, 
		0xefad, 
		0xc855, 
		0x5140, 
		0xb72d, 
		0xd080, 
		0xd100, 
		0x2f49, 
		0xcc40, 
		0x77df, 
		0xd2e0, 
		0xb2ff, 
		0x28fa, 
		0x5a31, 
		0x51c0, 
		0xd220, 
		0x5060, 
		0x11c5, 
		0x8126, 
		0x7c00, 
		0x5340, 
		0xb002, 
		0x82f4, 
		0xc700, 
		0xf468, 
		0x8000, 
		0x50c0, 
		0x0000, 
		0x0000, 
		0x8000, 
		0x7c00, 
		0xd2a0, 
		0x7c00, 
		0xb06c, 
		0xd300, 
		0xb242, 
		0x0000, 
		0xd340, 
		0xe549, 
		0xd9af, 
		0x6bb2, 
		0x3b99, 
		0x52c0, 
		0x5040, 
		0x5220, 
		0xd000, 
		0xd1e0, 
		0xd300, 
		0xca00, 
		0x7ae9, 
		0xd260, 
		0x45d2, 
		0x0000, 
		0x4d80, 
		0x5180, 
		0x5640, 
		0x97a9, 
		0x9211, 
		0x1919, 
		0x7d41, 
		0x5240, 
		0xd320, 
		0xd040, 
		0x2e70, 
		0x4cc0, 
		0xd060, 
		0x4d40, 
		0xc8d6, 
		0x5499, 
		0x53e0, 
		0x5340, 
		0x4c80, 
		0xcbfb, 
		0x51c0, 
		0xd2c0, 
		0x5360, 
		0xe65b, 
		0x8159, 
		0xd1a0, 
		0xfc77, 
		0xcdc0, 
		0xd1e0, 
		0xd2c0, 
		0x4d00, 
		0x84de, 
		0xd3c0, 
		0x2598, 
		0xcd40, 
		0xb42c, 
		0xe8a2, 
		0x1c1f, 
		0x817a, 
		0xa454, 
		0x0079, 
		0xfbe3, 
		0xfc00, 
		0x6600, 
		0xfc00, 
		0xf87b, 
		0x0ccd, 
		0x4700, 
		0xd180, 
		0xc700, 
		0x5020, 
		0x4e00, 
		0xcf80, 
		0xf7a2, 
		0xd1a0, 
		0x4f80, 
		0xd100, 
		0xd020, 
		0x4fc0, 
		0xa15a, 
		0x5620, 
		0xd220, 
		0xdfc5, 
		0xfcc1, 
		0x8000, 
		0xca80, 
		0xc135, 
		0x51a0, 
		0x9972, 
		0xcfd6, 
		0x51a0, 
		0x5080, 
		0x0349, 
		0xcdc0, 
		0xfc00, 
		0x5160, 
		0x5220, 
		0xd3c0, 
		0x9f56, 
		0xcd40, 
		0x4ec0, 
		0xcf80, 
		0xcec0, 
		0xcd40, 
		0x13f3, 
		0x5240, 
		0x6bdc, 
		0xd1e0, 
		0x52c0, 
		0xd080, 
		0x4a80, 
		0xca00, 
		0x940c, 
		0xc980, 
		0xfccd, 
		0xd3ee, 
		0xd360, 
		0x298d, 
		0xfc00, 
		0x50e1, 
		0xcc40, 
		0x7995, 
		0x32d1, 
		0xce80, 
		0xc980, 
		0x5260, 
		0x5020, 
		0x8000, 
		0x52a0, 
		0x4400, 
		0x4ec0, 
		0xc980, 
		0xfc00, 
		0xf762, 
		0x7d40, 
		0x3d59, 
		0x7c1c, 
		0x678d, 
		0xc700, 
		0xd280, 
		0x3a73, 
		0x4000, 
		0xc980, 
		0x0000, 
		0xd200, 
		0x53c0, 
		0x5100, 
		0xce40, 
		0x4cc0, 
		0xd1a0, 
		0x8000, 
		0x4b80, 
		0xd0e0, 
		0x4ec0, 
		0xfc00, 
		0x4400, 
		0xd300, 
		0x8000, 
		0xbb9e, 
		0xd360, 
		0xf469, 
		0xd120, 
		0xd280, 
		0x4c80, 
		0x5260, 
		0x50e0, 
		0xd3e0, 
		0xd100, 
		0x4d40, 
		0xf8a2, 
		0x8000, 
		0x81eb, 
		0xbb8e, 
		0x5140, 
		0x4e40, 
		0xc800, 
		0x8cc3, 
		0xd2c0, 
		0xeb9a, 
		0x5120, 
		0xfc00, 
		0xa829, 
		0x23b0, 
		0x7573, 
		0xcb00, 
		0x4400, 
		0xd2c0, 
		0x9006, 
		0xd71f, 
		0xfa2c, 
		0x4900, 
		0x53a0, 
		0x535f, 
		0x8acd, 
		0x05e2, 
		0x0000, 
		0xd2c0, 
		0xb8ca, 
		0x17fc, 
		0xd380, 
		0xcb00, 
		0xcb80, 
		0xcfc0, 
		0xd260, 
		0x5240, 
		0xc980, 
		0x4980, 
		0x51a0, 
		0x7b7e, 
		0xc971, 
		0xd300, 
		0x4d00, 
		0xd1a0, 
		0xcfc0, 
		0x5340, 
		0x1508, 
		0x52c0, 
		0x4cc0, 
		0x8f3a, 
		0xd020, 
		0x48a1, 
		0xd020, 
		0xd380, 
		0xd0c0, 
		0xb9d5, 
		0x4700, 
		0x4d00, 
		0xd4a9, 
		0x8000, 
		0xc900, 
		0xcec0, 
		0x4f40, 
		0xd260, 
		0x9924, 
		0xd180, 
		0x937e, 
		0x0000, 
		0xc800, 
		0xc800, 
		0xfad3, 
		0x4c40, 
		0x047d, 
		0xcc40, 
		0xb16d, 
		0x5300, 
		0x5280, 
		0x8000, 
		0xd3c0, 
		0x735b, 
		0xfc00, 
		0x5240, 
		0x3465, 
		0x4600, 
		0xd280, 
		0x6631, 
		0xd2c0, 
		0xca00, 
		0x50c0, 
		0x4c40, 
		0xd511, 
		0xc600, 
		0xa4bb, 
		0x53a0, 
		0x6f7d, 
		0xd320, 
		0xcd00, 
		0xd020, 
		0xd240, 
		0x5080, 
		0x4200, 
		0xd080, 
		0x4500, 
		0xd060, 
		0xcf80, 
		0xd180, 
		0x4880, 
		0x4900, 
		0x2c9d, 
		0x4e80, 
		0x1ffe, 
		0x9886, 
		0xf666, 
		0xea98, 
		0x0000, 
		0xd060, 
		0xce00, 
		0x3348, 
		0x5c11, 
		0xcb48, 
		0x4c40, 
		0xfc00, 
		0x5280, 
		0x4cc0, 
		0x13dd, 
		0xd040, 
		0x5160, 
		0xc8fc, 
		0x83aa, 
		0x5040, 
		0x5000, 
		0x5340, 
		0x8000, 
		0x8000, 
		0x6c45, 
		0xd180, 
		0xa825, 
		0x3ce2, 
		0xc35d, 
		0xc700, 
		0x51c0, 
		0xcfce, 
		0xd0c0, 
		0x5240, 
		0xd1c0, 
		0xd200, 
		0x5040, 
		0xcc00, 
		0x7c00, 
		0xd1e0, 
		0xcec0, 
		0x51c0, 
		0xd1a0, 
		0xa182, 
		0x7890, 
		0xcb00, 
		0x4d40, 
		0xd280, 
		0xfc00, 
		0x3d16, 
		0x7608, 
		0xcf40, 
		0xbe7b, 
		0xd320, 
		0xc400, 
		0x5220, 
		0x2533, 
		0xcb80, 
		0x50c0, 
		0x018a, 
		0x4210, 
		0x5060, 
		0x8000, 
		0x43e1, 
		0x8000, 
		0x4a00, 
		0x3731, 
		0x63f0, 
		0xd2c0, 
		0x51e0, 
		0xb283, 
		0x3bbc, 
		0xf468, 
		0x53a0, 
		0xd380, 
		0xc880, 
		0x50c0, 
		0x47d2, 
		0x4c00, 
		0x0a8d, 
		0x0537, 
		0xd240, 
		0xfd1e, 
		0x5140, 
		0x4e80, 
		0x4b00, 
		0x53a0, 
		0xcf40, 
		0xebda, 
		0x52a0, 
		0xcd40, 
		0x4800, 
		0xcec0, 
		0x2b8a, 
		0x8000, 
		0xd2c0, 
		0x0842, 
		0x572e, 
		0x1dca, 
		0x5180, 
		0x4a00, 
		0xf936, 
		0x0000, 
		0x5160, 
		0xd380, 
		0x5380, 
		0xc500, 
		0x8000, 
		0x7b33, 
		0xcf00, 
		0x5060, 
		0xe944, 
		0xcfc0, 
		0xbc00, 
		0x5360, 
		0xd0e0, 
		0xd220, 
		0xc900, 
		0x4200, 
		0xd1a0, 
		0x3e23, 
		0xcfc0, 
		0xd300, 
		0x5e0a, 
		0xd0c0, 
		0x51a0, 
		0xa349, 
		0xd280, 
		0x5bc4, 
		0x4900, 
		0xb8a5, 
		0xd060, 
		0x5000, 
		0x4990, 
		0x8000, 
		0x7c00, 
		0xe5cb, 
		0x4500, 
		0x84e9, 
		0x50e0, 
		0xc500, 
		0x6597, 
		0xd360, 
		0x4b80, 
		0x5ee4, 
		0xccc0, 
		0xa2ad, 
		0x6174, 
		0xc200, 
		0xd3a0, 
		0xcb80, 
		0x4400, 
		0xc880, 
		0x52c0, 
		0x4800, 
		0xd160, 
		0xd1c0, 
		0xcf40, 
		0x9202, 
		0x8c16, 
		0x5340, 
		0xe271, 
		0x5340, 
		0xc600, 
		0xc880, 
		0xd200, 
		0x0f33, 
		0x0e13, 
		0xcec0, 
		0xcc40, 
		0xfbf2, 
		0x5300, 
		0x3206, 
		0xf5e1, 
		0x0000, 
		0x8f66, 
		0x5220, 
		0x8b3a, 
		0x7d93, 
		0x59fc, 
		0x5000, 
		0xcdc0, 
		0xa678, 
		0x4600, 
		0x5160, 
		0x9dbb, 
		0x0000, 
		0xf347, 
		0x9d13, 
		0x5180, 
		0x7ca2, 
		0xc000, 
		0x5080, 
		0x1753, 
		0x7c00, 
		0x5100, 
		0x172f, 
		0xfc3c, 
		0xd120, 
		0xfc00, 
		0x0f1f, 
		0xcc00, 
		0x4c80, 
		0x8000, 
		0x4a80, 
		0x52a0, 
		0xa206, 
		0xd0e0, 
		0x4500, 
		0xc14f, 
		0xc700, 
		0xd240, 
		0x16f4, 
		0x4c00, 
		0xc600, 
		0xd240, 
		0x0000, 
		0xd180, 
		0xd5a4, 
		0x4e00, 
		0xd140, 
		0xc600, 
		0x52a0, 
		0x5320, 
		0x2751, 
		0x4c80, 
		0xd0c0, 
		0x5380, 
		0xd380, 
		0xd000, 
		0x244a, 
		0xd3c0, 
		0xd1e0, 
		0xc00b, 
		0x51a0, 
		0x1601, 
		0x50a0, 
		0xc25b, 
		0x1a9d, 
		0xc278, 
		0xcf40, 
		0xd260, 
		0xd320, 
		0x418f, 
		0x6f66, 
		0x53c0, 
		0x3c00, 
		0x4cc0, 
		0xfc00, 
		0x55ea, 
		0xc500, 
		0xc400, 
		0x4dc0, 
		0xd3c0, 
		0x4e40, 
		0xcd80, 
		0xd240, 
		0xb7da, 
		0x4000, 
		0xd55e, 
		0xcd40, 
		0x149e, 
		0xd2c0, 
		0x5320, 
		0x5200, 
		0x483e, 
		0x44a7, 
		0x52a0, 
		0xcec0, 
		0x950a, 
		0xe084, 
		0x06fd, 
		0x50e0, 
		0xf079, 
		0x4cce, 
		0x4fc0, 
		0xf41a, 
		0x028f, 
		0x8000, 
		0x8eb1, 
		0x8569, 
		0xf20a, 
		0x17df, 
		0xf320, 
		0x52c0, 
		0xc1b6, 
		0xcdc0, 
		0x47df, 
		0x4cc0, 
		0x53e0, 
		0xd1e0, 
		0x5360, 
		0x4e00, 
		0x4500, 
		0xce00, 
		0x950f, 
		0x5300, 
		0x0000, 
		0xd080, 
		0xd380, 
		0xd42c, 
		0x4ec0, 
		0xd200, 
		0xfc57, 
		0xcec0, 
		0x4cc0, 
		0x0000, 
		0x02db, 
		0x3c00, 
		0xbc00, 
		0xd140, 
		0x2c1a, 
		0xd0c0, 
		0xd320, 
		0x5180, 
		0xf7d8, 
		0xcc40, 
		0xa839, 
		0x8f7d, 
		0x4b03, 
		0xd3e0, 
		0x5200, 
		0x6f2b, 
		0x5080, 
		0xd080, 
		0xd100, 
		0xfd2e, 
		0xc478, 
		0x1f5d, 
		0xe5ce, 
		0xc3c4, 
		0x5200, 
		0xdd79, 
		0x6166, 
		0xd060, 
		0xeb0d, 
		0xd200, 
		0xd4e5, 
		0x5320, 
		0x4980, 
		0x8000, 
		0x5360, 
		0x46fb, 
		0xd140, 
		0xd2c0, 
		0xd180, 
		0x5020, 
		0x4e40, 
		0xc900, 
		0x5280, 
		0x72ad, 
		0x3576, 
		0x7c00, 
		0x712f, 
		0xe0a2, 
		0xd305, 
		0xd000, 
		0xe708, 
		0xd020, 
		0xd1c0, 
		0xd080, 
		0xd060, 
		0xcd40, 
		0xd2c0, 
		0xcb00, 
		0xd020, 
		0x0000, 
		0x4e00, 
		0xdc65, 
		0xd1c0, 
		0xc57b, 
		0xd260, 
		0x8000, 
		0x57f4, 
		0x5080, 
		0x4d00, 
		0xc980, 
		0xd000, 
		0xd080, 
		0xcf40, 
		0xd2a0, 
		0x0000, 
		0x6c7c, 
		0x8000, 
		0xca80, 
		0x4a00, 
		0xcf40, 
		0x7bdb, 
		0xcc68, 
		0x4f80, 
		0x4980, 
		0xce00, 
		0xd3a0, 
		0x4ec0, 
		0x9fa2, 
		0x4c40, 
		0x3168, 
		0xd220, 
		0xaf10, 
		0x7cfa, 
		0xbd93, 
		0x4200, 
		0xcb80, 
		0x52c0, 
		0x5380, 
		0x4c40, 
		0x5280, 
		0x4f80, 
		0xd340, 
		0xd100, 
		0x7b81, 
		0x4d00, 
		0x4f00, 
		0xad5e, 
		0x4e80, 
		0x5340, 
		0xcb00, 
		0x9994, 
		0xcc40, 
		0xc980, 
		0x8000, 
		0x7db4, 
		0x0592, 
		0x4e00, 
		0xc880, 
		0x100e, 
		0x53a0, 
		0x4d40, 
		0x4cc0, 
		0x52e0, 
		0xc500, 
		0x4f37, 
		0x4e80, 
		0x4a00, 
		0xd040, 
		0x4f00, 
		0x9905, 
		0xdb92, 
		0x4a80, 
		0xd300, 
		0x928d, 
		0xd1a0, 
		0x5260, 
		0xa735, 
		0x4200, 
		0xd120, 
		0x5280, 
		0x53c0, 
		0x0f1b, 
		0x5060, 
		0x310c, 
		0xf880, 
		0xdebd, 
		0x5280, 
		0xd220, 
		0xc600, 
		0xce00, 
		0x2b09, 
		0x9342, 
		0x4fc0, 
		0x8000, 
		0xd0c0, 
		0xd080, 
		0x50c0, 
		0x4400, 
		0x4cc0, 
		0xc600, 
		0x53c0, 
		0x5240, 
		0xcc40, 
		0x2a96, 
		0x5aec, 
		0x51e0, 
		0x4e00, 
		0x8000, 
		0xd020, 
		0x27f5, 
		0x5120, 
		0xcd00, 
		0xd624, 
		0x6976, 
		0x4a00, 
		0x68b7, 
		0xd140, 
		0x4658, 
		0xce00, 
		0xcc40, 
		0xcd00, 
		0xd320, 
		0x5080, 
		0x4d40, 
		0xd220, 
		0xc700, 
		0x167a, 
		0x4f42, 
		0x8000, 
		0x0000, 
		0xa684, 
		0x4c40, 
		0xd1a0, 
		0x8000, 
		0x36c5, 
		0xcec0, 
		0x0000, 
		0x64a1, 
		0x5120, 
		0xd000, 
		0x4700, 
		0x2738, 
		0x4980, 
		0xf91d, 
		0xcfc0, 
		0x53a0, 
		0xd340, 
		0x37a0, 
		0xca00, 
		0xc8bf, 
		0x5220, 
		0xcf80, 
		0xd340, 
		0x5240, 
		0x4200, 
		0x53a0, 
		0xcec0, 
		0x0000, 
		0x0000, 
		0x7c00, 
		0xcb80, 
		0xcd00, 
		0xcf00, 
		0x2b78, 
		0xcb80, 
		0xfcb4, 
		0x0000, 
		0xd260, 
		0x4e00, 
		0x8000, 
		0x5060, 
		0x3f43, 
		0xd220, 
		0x53c0, 
		0xd3c0, 
		0xf755, 
		0x3c00, 
		0xfc5f, 
		0x4c80  
};
volatile uint32_t rand_ieee754_32[1536] __attribute__ ((aligned (2048))) = {
		0xc2000000, 
		0x807fffff, 
		0x41700000, 
		0x7f800000, 
		0x070a5e83, 
		0xace838ba, 
		0x360ddb5f, 
		0x7f800000, 
		0x80000000, 
		0xbf028f5c, 
		0x3ff476c3, 
		0xc1e80000, 
		0x41c80000, 
		0x7f7ffffe, 
		0x0f7ffffe, 
		0xff800001, 
		0x00004000, 
		0x002b554e, 
		0xc27c0000, 
		0xcbd2aef3, 
		0x76748f6b, 
		0xffbfffff, 
		0x0e001fff, 
		0x2e65d4dc, 
		0x40c00000, 
		0xcb000000, 
		0xa4d5fb12, 
		0x0e7fffff, 
		0xe36219e4, 
		0xc2640000, 
		0x84b747a0, 
		0x00002000, 
		0xc1b80000, 
		0xc1f80000, 
		0x4b8c4b40, 
		0xffda20d0, 
		0x00000800, 
		0xff800000, 
		0xc0000000, 
		0xc2200000, 
		0xf7ae0234, 
		0xf353776c, 
		0x53e17f97, 
		0x7f7ffffe, 
		0x0e007fff, 
		0x80800000, 
		0x4b000000, 
		0xd079c334, 
		0x00000000, 
		0x41a80000, 
		0x00472ec5, 
		0x33333333, 
		0xefc992a5, 
		0xc2040000, 
		0x80000000, 
		0xc2340000, 
		0x3efbba72, 
		0xc2600000, 
		0x80000002, 
		0x27cc6cb9, 
		0x00010000, 
		0x20884a48, 
		0x56b40401, 
		0x440f6769, 
		0xc1e80000, 
		0x00400000, 
		0x33333333, 
		0x662259eb, 
		0xff800000, 
		0xff000000, 
		0x3b2c14bd, 
		0xc2400000, 
		0x7fb8b3ab, 
		0xb7bb67e9, 
		0x42780000, 
		0x00800003, 
		0x41c00000, 
		0x00000000, 
		0x0f538813, 
		0x3111943c, 
		0xc1800000, 
		0xba8da096, 
		0x80062ddb, 
		0x7fae4a17, 
		0x0c7fffe0, 
		0x0f7fffff, 
		0x0e000003, 
		0x7f800000, 
		0x80000008, 
		0xb28bb649, 
		0x00000000, 
		0x00000200, 
		0x5b74ee76, 
		0x7f7ffffe, 
		0x8287d694, 
		0x80800003, 
		0x00000001, 
		0x42400000, 
		0x67ccf2d2, 
		0xe5e5c516, 
		0x426c0000, 
		0x00000080, 
		0x41c80000, 
		0x80000000, 
		0x42740000, 
		0xc1200000, 
		0x00000000, 
		0xbcffd6ba, 
		0x0e0007ff, 
		0xc0c00000, 
		0x7f800000, 
		0x0c7fff80, 
		0xc0a00000, 
		0xb8ee1b02, 
		0xc1600000, 
		0x80011111, 
		0x42280000, 
		0xc2740000, 
		0xffc00000, 
		0xffb5c508, 
		0x80000000, 
		0x80000800, 
		0x38b7c2ac, 
		0xc2700000, 
		0x0d000ff0, 
		0x420c0000, 
		0x00392165, 
		0x00000002, 
		0x80000000, 
		0xb7a643fc, 
		0x34ecc8c3, 
		0x41000000, 
		0x8826976a, 
		0x00000000, 
		0x52255820, 
		0x3d573721, 
		0x80000000, 
		0x8f7ffffe, 
		0x80ffffff, 
		0x41c00000, 
		0xc1a00000, 
		0xc2540000, 
		0x0e00000f, 
		0x427c0000, 
		0x7f800000, 
		0x00000000, 
		0x7f7ffffe, 
		0xff7ffffe, 
		0x42100000, 
		0xae833422, 
		0xff000000, 
		0x74a421e8, 
		0xcf06ff71, 
		0x00000000, 
		0xc26c0000, 
		0xc1d00000, 
		0xff800000, 
		0x42740000, 
		0xc0800000, 
		0xc2480000, 
		0xc1c00000, 
		0xd5134bac, 
		0x80000000, 
		0x80000000, 
		0x0c7fffc0, 
		0xc1f00000, 
		0x7f800000, 
		0x807fffff, 
		0xc2040000, 
		0x00800000, 
		0xff800000, 
		0xc0e00000, 
		0x80800001, 
		0x7b1914db, 
		0xff7ffffe, 
		0x0f7fffff, 
		0xe63a3dd3, 
		0x00000200, 
		0x42600000, 
		0x7f800000, 
		0x007ffffe, 
		0x007ffffe, 
		0x80ffffff, 
		0xff800000, 
		0xc1600000, 
		0x2349b67b, 
		0x00800000, 
		0x00800003, 
		0xe73b444f, 
		0xc0000000, 
		0x463daeb5, 
		0xcb000000, 
		0x4e5e52df, 
		0x0f7ffffe, 
		0x7f7ffffe, 
		0x3f800000, 
		0x80000001, 
		0xc1300000, 
		0x9575c55f, 
		0x4ade42ed, 
		0x86203c8f, 
		0x3c1954a8, 
		0x028976e3, 
		0x0e00ffff, 
		0xc1880000, 
		0x937404c5, 
		0xd9e6e8e7, 
		0x0e00000f, 
		0x42780000, 
		0x42040000, 
		0x41f80000, 
		0x42540000, 
		0xff800000, 
		0x41200000, 
		0x41300000, 
		0x7f7fffff, 
		0x42240000, 
		0xc1000000, 
		0x0c7fffff, 
		0x80800002, 
		0x00200000, 
		0x44edbfff, 
		0x0f7fffff, 
		0x00000000, 
		0x0265b354, 
		0xc2600000, 
		0x3810721f, 
		0x034d1c18, 
		0xff7fffff, 
		0x80000040, 
		0xea5f2fa3, 
		0x57ade459, 
		0x42000000, 
		0x00000000, 
		0xa5043533, 
		0x00000000, 
		0xb64fda83, 
		0x80200000, 
		0x6aaf70ed, 
		0xff800000, 
		0x0e000007, 
		0x0e0fffff, 
		0x65fb8943, 
		0x0d8ae852, 
		0xee2b9478, 
		0x80199f56, 
		0x211c41d2, 
		0xf03e9c4e, 
		0x0c7fff80, 
		0x00000000, 
		0x0c400000, 
		0x41d80000, 
		0x0c7f8000, 
		0x00000000, 
		0x0e00001f, 
		0x00800001, 
		0x84bf80b7, 
		0x0c780000, 
		0x7f800000, 
		0xd3e2518b, 
		0x0cacbd46, 
		0xff800000, 
		0x42200000, 
		0xa9fd1e25, 
		0x42440000, 
		0xaaaaaaaa, 
		0x0c600000, 
		0x41532e14, 
		0x80000001, 
		0xc2400000, 
		0x14395259, 
		0xbe699afb, 
		0xc25c0000, 
		0xd9d89ff3, 
		0xc2740000, 
		0x00000000, 
		0x80000100, 
		0x40e00000, 
		0x96d513ea, 
		0x41900000, 
		0x0c7ffffe, 
		0xff7ffffe, 
		0x2624bf9b, 
		0x80000020, 
		0x80000080, 
		0x41500000, 
		0x80000000, 
		0x42580000, 
		0xc2380000, 
		0x3ef24d5a, 
		0xb5ad66d3, 
		0xff800000, 
		0x0c7ffe00, 
		0xc1f80000, 
		0xff7ffffe, 
		0x80000000, 
		0x7f800000, 
		0x41c80000, 
		0x00000800, 
		0x41700000, 
		0x3652506b, 
		0xc2300000, 
		0x3d711989, 
		0x86daf88d, 
		0xc2140000, 
		0xc1900000, 
		0xc27c0000, 
		0xc2680000, 
		0x55555555, 
		0x420c0000, 
		0x3a89bed7, 
		0x7f7fffff, 
		0xc1c80000, 
		0xffbcfb71, 
		0x7e554e0b, 
		0x00000000, 
		0x7f800000, 
		0xc26c0000, 
		0xc22c0000, 
		0x1b94f660, 
		0xc2200000, 
		0x7f800000, 
		0xffbc6cb3, 
		0xc21c0000, 
		0xc1f00000, 
		0xc0400000, 
		0x8ec4b1c1, 
		0x42200000, 
		0x00000800, 
		0x3923e83c, 
		0x80000000, 
		0xcb8c4b40, 
		0x8266d0e7, 
		0x80000020, 
		0x00000002, 
		0x807fffff, 
		0xff8e9ebb, 
		0x00000000, 
		0x7f7fffff, 
		0x80800001, 
		0x42480000, 
		0x80000000, 
		0x46251139, 
		0x5516c6a4, 
		0x0e7fffff, 
		0x8137e222, 
		0x0e007fff, 
		0x00000000, 
		0x80000001, 
		0xc2440000, 
		0x7fc00001, 
		0x00000000, 
		0xc27c0000, 
		0x80000002, 
		0x80004725, 
		0x80000010, 
		0xcccccccc, 
		0x98bb5d5d, 
		0x65840622, 
		0xc1f00000, 
		0x15964deb, 
		0x83b8e065, 
		0xaaaaaaaa, 
		0x58ee51a1, 
		0x8020103b, 
		0xff000000, 
		0x00000001, 
		0x7fc00000, 
		0xc1300000, 
		0x41700000, 
		0x2483c27b, 
		0x17ed5439, 
		0xaaaaaaaa, 
		0x77699ae1, 
		0x80000000, 
		0x173eaffd, 
		0x0e03ffff, 
		0x42500000, 
		0x2a85d3a9, 
		0x00000000, 
		0x80800002, 
		0x00800003, 
		0xcccccccc, 
		0x7f800000, 
		0x80010000, 
		0x00000000, 
		0x42600000, 
		0x7f800001, 
		0x0c7fffe0, 
		0xc2300000, 
		0x7fffffff, 
		0xe14922c4, 
		0xbd5619e9, 
		0x0d000ff0, 
		0xf265862a, 
		0xc2200000, 
		0xa1af47df, 
		0xb24a7068, 
		0x41200000, 
		0xc2200000, 
		0xff7fffff, 
		0xad6ec6d0, 
		0xdf43a8a5, 
		0x80008000, 
		0xc20c0000, 
		0x80000000, 
		0xffe0bc94, 
		0xff800000, 
		0x0c7ff000, 
		0x33333333, 
		0x41d00000, 
		0x2966f677, 
		0x150e3388, 
		0x41c00000, 
		0x3f800000, 
		0x00000001, 
		0x00800000, 
		0xff7ffffe, 
		0x2c7dddf1, 
		0xff800000, 
		0x80000000, 
		0x0e00ffff, 
		0x00000002, 
		0x41700000, 
		0x1e779ed7, 
		0xc20c0000, 
		0x7fcc01cc, 
		0x40800000, 
		0xc24c0000, 
		0xff7fffff, 
		0xff7ffffe, 
		0x00800001, 
		0xc2440000, 
		0xc2540000, 
		0xc2780000, 
		0x40800000, 
		0x00000000, 
		0x3f800000, 
		0x41d80000, 
		0x2b1c2d21, 
		0x80000000, 
		0xc2540000, 
		0x00000000, 
		0x00000000, 
		0x80000002, 
		0x3f800001, 
		0xc20c0000, 
		0x41300000, 
		0x00002000, 
		0x41b80000, 
		0x0e00007f, 
		0x7f800000, 
		0x00000000, 
		0x42580000, 
		0x7f7ffffe, 
		0x93aa0b7a, 
		0x99f9f16a, 
		0x2c51b7fd, 
		0x7fc00001, 
		0x7fb28938, 
		0xc23c0000, 
		0xc0000000, 
		0xf57f5423, 
		0x26107b89, 
		0x410680c9, 
		0x80000002, 
		0x41a80000, 
		0x274f4609, 
		0xffc00001, 
		0x00000002, 
		0xc0a00000, 
		0x423c0000, 
		0x80000004, 
		0x0e01ffff, 
		0x260d6e68, 
		0x00000000, 
		0x09575f54, 
		0x41000000, 
		0xc2280000, 
		0x80000040, 
		0x7f800000, 
		0x9eb64c55, 
		0x0c400000, 
		0xbf028f5c, 
		0xeb7ad849, 
		0xc0000000, 
		0x58e04b72, 
		0x4753de44, 
		0x38f1b45a, 
		0x80000000, 
		0x00004000, 
		0x80ffffff, 
		0x0e0001ff, 
		0x5d5b12dc, 
		0x80000002, 
		0x80000000, 
		0xffbd5256, 
		0xdfef1226, 
		0xbfb9c011, 
		0x40000000, 
		0x007fffff, 
		0x4d942b3e, 
		0x7f800000, 
		0x4e9691c4, 
		0x0c7fffe0, 
		0x66c84ed7, 
		0x87c59fe1, 
		0x00020000, 
		0xd02563e8, 
		0xc1000000, 
		0xc0e00000, 
		0x0c7fff00, 
		0xae86a575, 
		0xc5956d2f, 
		0x0c7ff000, 
		0x80000000, 
		0xc631b6f9, 
		0x00000001, 
		0x8043ea46, 
		0x41b80000, 
		0x80000004, 
		0xc22c0000, 
		0xffffffff, 
		0x69ef1769, 
		0x80002000, 
		0xd71c457d, 
		0xc1600000, 
		0x0c7ffffe, 
		0x2ae68138, 
		0x0f7ffffe, 
		0x0e00000f, 
		0x0c7fff00, 
		0x9f5ef853, 
		0x7fc00001, 
		0x00000000, 
		0x80100000, 
		0x7f97ae43, 
		0xcccccccc, 
		0xc2400000, 
		0x00020000, 
		0xc0000000, 
		0xcb000000, 
		0x00ffffff, 
		0x88c94ea1, 
		0x80000000, 
		0x41f80000, 
		0x00800001, 
		0x80004000, 
		0x41e80000, 
		0x420c0000, 
		0x17227ef5, 
		0x80080000, 
		0x7d22e3a7, 
		0x7fabe9e9, 
		0xc27c0000, 
		0xffc00000, 
		0x34ad8891, 
		0xffffffff, 
		0x42100000, 
		0x9d884a9d, 
		0x80000000, 
		0x1a90bf80, 
		0x4259e367, 
		0x80000000, 
		0xc2500000, 
		0x00000000, 
		0x32c535d0, 
		0xcb8c4b40, 
		0xd2899de6, 
		0x80000000, 
		0xc2300000, 
		0x7f800000, 
		0xdd868b71, 
		0x46a3b6d5, 
		0x0d000ff0, 
		0x42580000, 
		0xb2696b89, 
		0x0c600000, 
		0xe1e9a153, 
		0x80001000, 
		0xc1500000, 
		0xabd6185c, 
		0xff800000, 
		0x00040000, 
		0x80000080, 
		0xc0a00000, 
		0xc2040000, 
		0x00000080, 
		0x80000008, 
		0x00000000, 
		0x956866f9, 
		0x004c1133, 
		0x7f800000, 
		0x80001000, 
		0xbd29fce7, 
		0xc1500000, 
		0xf684eb18, 
		0x80000004, 
		0x2763163f, 
		0xff800000, 
		0x9b44d8b1, 
		0x7f800000, 
		0x41200000, 
		0xffabb94c, 
		0x8053bcce, 
		0xa6c88d11, 
		0xc7350468, 
		0xc2500000, 
		0x3f028f5c, 
		0x40400000, 
		0xde8ac58c, 
		0x40400000, 
		0xffde2b90, 
		0x7f800000, 
		0x42240000, 
		0x42080000, 
		0x80000040, 
		0x80000001, 
		0xf0a55a70, 
		0x00000004, 
		0x80040000, 
		0x41000000, 
		0x2ab1f149, 
		0xa32dc1dc, 
		0xe61b2df6, 
		0x602edfd6, 
		0xffe748df, 
		0x9968436e, 
		0x7f800000, 
		0x5c7e4bdf, 
		0x80000000, 
		0xbc944e95, 
		0x00800000, 
		0x423c0000, 
		0x80000000, 
		0x0c7ff800, 
		0x10d7701f, 
		0x00000000, 
		0x40c00000, 
		0x427c0000, 
		0x83848e11, 
		0x0c7ffff8, 
		0x00800003, 
		0x740e3cf3, 
		0x6f055065, 
		0x42380000, 
		0x00000000, 
		0x57591422, 
		0x00800003, 
		0x00000200, 
		0x80000004, 
		0xf708d71f, 
		0x0c600000, 
		0x41980000, 
		0x5a8867b7, 
		0x7f7ffffe, 
		0x80000000, 
		0x80000000, 
		0x80000001, 
		0x00800001, 
		0xc2580000, 
		0x80011111, 
		0x8f7ffffd, 
		0x80800000, 
		0x00000000, 
		0xf03c1fe0, 
		0x7f7ffffe, 
		0xff97e6f2, 
		0xff929c03, 
		0x7f800000, 
		0xc0e00000, 
		0xbf800000, 
		0xc2740000, 
		0xc0400000, 
		0xe82ff5a4, 
		0x00000000, 
		0x00000040, 
		0x00000001, 
		0x0c7ff800, 
		0x00200000, 
		0xa2938710, 
		0x1a1a6057, 
		0xc1700000, 
		0x7fc00001, 
		0x4943b942, 
		0x1757da09, 
		0xa5f9ca70, 
		0x4b000000, 
		0x80000020, 
		0x0e0003ff, 
		0xc2780000, 
		0x00000200, 
		0x41c80000, 
		0x5eb6de35, 
		0x80000001, 
		0x80800001, 
		0x0e00003f, 
		0x0e000fff, 
		0xc1980000, 
		0x3f028f5c, 
		0x0e0007ff, 
		0x25c09e4e, 
		0x80000000, 
		0x7f800001, 
		0x0e0001ff, 
		0x00020000, 
		0x42640000, 
		0x00ffffff, 
		0x7f7ffffe, 
		0x0e000003, 
		0x00000000, 
		0x111ff18b, 
		0x0e03ffff, 
		0x8f7fffff, 
		0xcccccccc, 
		0x41000000, 
		0x00800002, 
		0x00080000, 
		0x135bf019, 
		0x00800003, 
		0xec395a8d, 
		0x427c0000, 
		0xcb8c4b40, 
		0x0e0fffff, 
		0x96124855, 
		0x89848f3e, 
		0xff800000, 
		0x66d96512, 
		0x00200000, 
		0x420c0000, 
		0x80000000, 
		0x00000000, 
		0x420c0000, 
		0xcccccccc, 
		0x00400000, 
		0x42700000, 
		0x63083830, 
		0xffbfffff, 
		0x0e0fffff, 
		0x6d5ae95f, 
		0x3f028f5c, 
		0x80000000, 
		0xdbdaaf08, 
		0xff000000, 
		0x40800000, 
		0x7f7fffff, 
		0x80200000, 
		0x8c8ce074, 
		0x7f800000, 
		0x80000000, 
		0x00800001, 
		0xc1d00000, 
		0xbf800000, 
		0xcccccccc, 
		0xfacebc9d, 
		0x42540000, 
		0xbf028f5c, 
		0x16de7460, 
		0x0e000001, 
		0x70a95869, 
		0xda5fb5ed, 
		0x54c1a3e5, 
		0xc2200000, 
		0x69eb4e8e, 
		0x0e01ffff, 
		0x41b00000, 
		0x5a8e6dc3, 
		0x9617d7b5, 
		0x11726051, 
		0xcaef088a, 
		0x40321cb3, 
		0x7fc00001, 
		0x0e0fffff, 
		0x0f9a5ce3, 
		0xca0e7dc8, 
		0xd15e6a04, 
		0x60104a4c, 
		0x42380000, 
		0x3174cbd4, 
		0xb3af86c4, 
		0xc1c80000, 
		0x80000000, 
		0x05ff3ae1, 
		0x00000000, 
		0xc373e756, 
		0x3d8a9377, 
		0x0c7fff80, 
		0x4b000000, 
		0x8bbac501, 
		0x0e000007, 
		0x98d8bea0, 
		0x41500000, 
		0xff7fffff, 
		0x42540000, 
		0x7f800000, 
		0x7f7ffffe, 
		0xc1a80000, 
		0xa9087c06, 
		0x42480000, 
		0xc1600000, 
		0x4b000000, 
		0xea5550b8, 
		0x3f028f5c, 
		0xa106c2db, 
		0x0e00003f, 
		0xdceda16b, 
		0xc1e00000, 
		0x00000000, 
		0xc1900000, 
		0xff7ffffe, 
		0xc1175c73, 
		0x41e00000, 
		0x00800002, 
		0x80000001, 
		0x80000000, 
		0xff800000, 
		0x00000400, 
		0x57dc6a03, 
		0x1939114a, 
		0x9b2dd023, 
		0x41c80000, 
		0x80080000, 
		0xc2600000, 
		0xdde68a87, 
		0x00800001, 
		0x00040000, 
		0x0c7fe000, 
		0xc27c0000, 
		0x42300000, 
		0x0d00fff0, 
		0xe143299c, 
		0x40000000, 
		0x66d11c5b, 
		0x7ff68c88, 
		0x42740000, 
		0x8a6c264c, 
		0x40038470, 
		0xc1900000, 
		0x00002000, 
		0xc21c0000, 
		0xfe0fa69a, 
		0xff800001, 
		0x007ffffe, 
		0x0e07ffff, 
		0xff800000, 
		0x41f00000, 
		0x80800001, 
		0x0c7ff000, 
		0xc0a00000, 
		0x8f7fffff, 
		0x41d80000, 
		0x80800000, 
		0x1c36fab6, 
		0x4f227dc1, 
		0x0c7fe000, 
		0x0e07ffff, 
		0x00200000, 
		0x00000000, 
		0x80800001, 
		0x00800003, 
		0x0c400000, 
		0x007fffff, 
		0x0e01ffff, 
		0x33333333, 
		0xc2440000, 
		0xef63127d, 
		0x7fc00000, 
		0x00800002, 
		0xff7fffff, 
		0x80004000, 
		0x0c7fffe0, 
		0x80000000, 
		0x0e000003, 
		0x067b3711, 
		0x41700000, 
		0x80000000, 
		0x7fa787af, 
		0xff637939, 
		0xd8a8344c, 
		0x41500000, 
		0xff7ffffe, 
		0x7f800000, 
		0x80800001, 
		0xc1300000, 
		0xffbfa8bb, 
		0x80000020, 
		0xcb8c4b40, 
		0x00000000, 
		0x80010000, 
		0x80000000, 
		0x0e00000f, 
		0x0f7ffffd, 
		0x41c00000, 
		0x7f7ffffe, 
		0x3b34ea29, 
		0x7fba5c5c, 
		0xc6be506a, 
		0x80000000, 
		0x00000800, 
		0xc1c00000, 
		0xc2140000, 
		0x007fffff, 
		0x42500000, 
		0x0f7fffff, 
		0x4684b722, 
		0x3f800001, 
		0xc21c0000, 
		0x7f7ffffe, 
		0xf68bccc3, 
		0x80000000, 
		0xed6da16f, 
		0x41d80000, 
		0x00011111, 
		0xc0e00000, 
		0x00800002, 
		0x42440000, 
		0x8aef6850, 
		0x42000000, 
		0xffb7e4b3, 
		0xffc00000, 
		0x422c0000, 
		0x0c780000, 
		0x80000400, 
		0x80000000, 
		0x80000000, 
		0x3a24add4, 
		0x80101981, 
		0x37c97c29, 
		0xf390cc12, 
		0x7f7ffffe, 
		0xffffffff, 
		0x00000001, 
		0x508f57fe, 
		0xfbe3edf7, 
		0xff7fffff, 
		0x80000002, 
		0x00000000, 
		0x41f00000, 
		0x40800000, 
		0x00000000, 
		0x15b2ad3f, 
		0xc1b00000, 
		0x9134fcc3, 
		0xff800000, 
		0x41600000, 
		0x80000000, 
		0x7f000000, 
		0xac6b90a0, 
		0x0f7ffffc, 
		0x75a318e4, 
		0x0c400000, 
		0xc1700000, 
		0x80000400, 
		0xff800000, 
		0x5bb7b2b7, 
		0x0e0000ff, 
		0x00000002, 
		0x00000080, 
		0x80000001, 
		0x0e000007, 
		0x7f800000, 
		0x7f800000, 
		0xfd887328, 
		0x42100000, 
		0x41a80000, 
		0x00800000, 
		0x55555555, 
		0xaa00eb25, 
		0x40c00000, 
		0x924d442a, 
		0x41d80000, 
		0x41f00000, 
		0x8c88a1f3, 
		0x80000010, 
		0x33e50bf7, 
		0x80639e62, 
		0x7fb4868a, 
		0x19352d42, 
		0xc1300000, 
		0x0e0001ff, 
		0x42540000, 
		0x2eb96f06, 
		0x1467de35, 
		0xdb4d976b, 
		0x80ffffff, 
		0x8170560f, 
		0xc1200000, 
		0x0f7fffff, 
		0x80800001, 
		0x42540000, 
		0xdb6b6569, 
		0x8112a01a, 
		0x41900000, 
		0x00020000, 
		0xbf028f5c, 
		0x0c7ffffe, 
		0x426c0000, 
		0x80000000, 
		0xc0000000, 
		0xc24c0000, 
		0x6147dafd, 
		0x0c7ffffc, 
		0xc0800000, 
		0xf9bd744e, 
		0x41b00000, 
		0x7f800000, 
		0x6a8523ad, 
		0x2b3064c5, 
		0xe6545ff7, 
		0x967e6b50, 
		0xc0000000, 
		0x7ffcc700, 
		0xc2280000, 
		0x9fda018c, 
		0x41c00000, 
		0xc2040000, 
		0x0c7f0000, 
		0x55555555, 
		0x38eedb02, 
		0x74079a3c, 
		0x80485831, 
		0x0c7f8000, 
		0x80001000, 
		0x00040000, 
		0x80800001, 
		0x41c80000, 
		0x6bbd5a31, 
		0x00000000, 
		0x9b9fe544, 
		0xb829e98e, 
		0x7f800001, 
		0xd648981a, 
		0x0e01ffff, 
		0xa4b60f53, 
		0x562f8f20, 
		0xef818d47, 
		0x8dcb2adb, 
		0x0e0003ff, 
		0x00800003, 
		0xffbee3f0, 
		0x154ec599, 
		0x5b269c91, 
		0x00000020, 
		0x42600000, 
		0x00100000, 
		0x13463941, 
		0x00800000, 
		0x807fffff, 
		0xea995b53, 
		0x554a09d3, 
		0x3c1e56ea, 
		0xc1d80000, 
		0x6c36ee02, 
		0x00001000, 
		0x41800000, 
		0x00000000, 
		0x0c780000, 
		0x6a729a89, 
		0xffffffff, 
		0x80000001, 
		0x8c33bd28, 
		0x00000008, 
		0xc1900000, 
		0x0c7f0000, 
		0x3f800000, 
		0xb769b063, 
		0x00800003, 
		0x41c00000, 
		0x80000002, 
		0x41400000, 
		0x3cb6a160, 
		0xc2640000, 
		0xc1900000, 
		0x10837730, 
		0x0e007fff, 
		0x817f7d40, 
		0x80ffffff, 
		0xb3e380a1, 
		0x0e0fffff, 
		0xcc29aec2, 
		0x00000200, 
		0xc2540000, 
		0xc1600000, 
		0xdb039284, 
		0x00004000, 
		0x0e00003f, 
		0xb23e2851, 
		0x80f3c6da, 
		0x80000004, 
		0xc0800000, 
		0x00000001, 
		0x0c7f8000, 
		0x80000040, 
		0x0e000001, 
		0xc24c0000, 
		0x8f7ffffd, 
		0x41000000, 
		0x0f9e6765, 
		0x00800001, 
		0xc390a6e6, 
		0xc2140000, 
		0x007fffff, 
		0x41b80000, 
		0x00000000, 
		0x00000000, 
		0x80000000, 
		0xaf216b7f, 
		0x41200000, 
		0xff7ffffe, 
		0x6229a2e3, 
		0xdc59e482, 
		0x422c0000, 
		0xff800000, 
		0x2d6fff51, 
		0x7f7fffff, 
		0x98e18889, 
		0x057b1060, 
		0x0e007fff, 
		0x00400000, 
		0x41e00000, 
		0xd145fc7b, 
		0xff800000, 
		0x00000008, 
		0x7fc00001, 
		0xff7ffffe, 
		0x00000000, 
		0x42680000, 
		0x00800003, 
		0xff800000, 
		0x80800000, 
		0x3f800001, 
		0x7f7ffffe, 
		0x0b9edaa8, 
		0x0c7ff800, 
		0x41e80000, 
		0xc1600000, 
		0x00000002, 
		0x007ffffe, 
		0xc1900000, 
		0x41500000, 
		0x3536390b, 
		0x80ffffff, 
		0x80000800, 
		0x8022ab09, 
		0xc0400000, 
		0x0e6df742, 
		0x42000000, 
		0x00800003, 
		0xda25b30d, 
		0x0c7ffe00, 
		0x80000000, 
		0x79f092f2, 
		0x42140000, 
		0xff7fffff, 
		0x00000000, 
		0x002467a6, 
		0x80000040, 
		0xffbf942c, 
		0x0039d850, 
		0xd8958968, 
		0xc1a80000, 
		0x00000000, 
		0xc2740000, 
		0x426c0000, 
		0x80800000, 
		0x0068d2e6, 
		0x42000000, 
		0x4126503e, 
		0xff800000, 
		0xcaa98b93, 
		0x7f800000, 
		0x0d000ff0, 
		0x0e07ffff, 
		0x0e1fffff, 
		0x0d00fff0, 
		0x7f800000, 
		0x00001000, 
		0xc0400000, 
		0xff800001, 
		0x8f7ffffc, 
		0x00000010, 
		0xc23c0000, 
		0x80004000, 
		0x80000010, 
		0x09fc3760, 
		0x00400000, 
		0xdab74ff5, 
		0x8b83be88, 
		0x80000000, 
		0xc1d00000, 
		0x7f800000, 
		0x00000100, 
		0x623b832e, 
		0x42400000, 
		0x6d492309, 
		0xc2440000, 
		0x7fa2f943, 
		0xf21c7543, 
		0x42440000, 
		0xc0e00000, 
		0x00000000, 
		0x42300000, 
		0x9c19bb7c, 
		0x14316af2, 
		0x52c350c3, 
		0x807ad601, 
		0xbf58ef2c, 
		0x00000008, 
		0x7f9f939f, 
		0x42000000, 
		0x6de592d0, 
		0x00008000, 
		0x9fc3e054, 
		0x55489340, 
		0xc1880000, 
		0xc1d80000, 
		0x4b7adbd1, 
		0x7fbfffff, 
		0x41e80000, 
		0x0c600000, 
		0x80800001, 
		0x0e000003, 
		0xc86b9e7c, 
		0x80000004, 
		0x4b3d4cb9, 
		0x601bbd8c, 
		0x40000000, 
		0xc1880000, 
		0x0e0007ff, 
		0x9a3b1edc, 
		0xf66932da, 
		0xe2bd9757, 
		0x00000000, 
		0x84c9f401, 
		0x54496e1f, 
		0x80000000, 
		0x7fbfffff, 
		0x80000000, 
		0x8025ed77, 
		0x782539eb, 
		0xe9184262, 
		0xaaaaaaaa, 
		0x7fbfffff, 
		0x5a468616, 
		0x00800001, 
		0xff800000, 
		0xc0400000, 
		0x0c7e0000, 
		0xbf028f5c, 
		0x7f800000, 
		0x9356c5d5, 
		0x00080000, 
		0x42580000, 
		0x0e0003ff, 
		0x41d00000, 
		0x80000080, 
		0x000b69e5, 
		0x2febe6ed, 
		0x00080000, 
		0x23ea5c52, 
		0x0e0007ff, 
		0xc2080000, 
		0xc27c0000, 
		0x0c7ff800, 
		0x0e1996ed, 
		0xc21c0000, 
		0x7ff5fa14, 
		0x0c7ffc00, 
		0x42c2a03d, 
		0xc2100000, 
		0xc2140000, 
		0x40000000, 
		0xa11ac22a, 
		0x80000002, 
		0x80800003, 
		0xfd2d92e4, 
		0x0e001fff, 
		0x0c7ffffe, 
		0xffeb4fd1, 
		0x002942ae, 
		0x00010000, 
		0xc1200000, 
		0x41800000, 
		0xffc4207e, 
		0x41f80000, 
		0x80010000, 
		0x9014ec80, 
		0x00000004, 
		0xfc3848df, 
		0x9a2dc7f5, 
		0x41e00000, 
		0xed265503, 
		0xc0000000, 
		0x92b21c8f, 
		0x80010000, 
		0x9f24930a, 
		0x0016f232, 
		0x8f7ffffd, 
		0xff800000, 
		0x80000000, 
		0x0c7fffc0, 
		0x00008000, 
		0x0c7fffff, 
		0x00000400, 
		0xc2380000, 
		0xc1200000, 
		0x0e00000f, 
		0x7f800000, 
		0x00000000, 
		0x7fc00001, 
		0x80000000, 
		0x80800000, 
		0xc0400000, 
		0xb1239d64, 
		0xe429e8d4, 
		0x0f7fffff, 
		0xc2540000, 
		0x3f800001, 
		0x9e4b4b26, 
		0x21956e08, 
		0xd11ffb43, 
		0x80001000, 
		0x80000800, 
		0x8f7ffffd, 
		0x42640000, 
		0xf0ec42b3, 
		0x0c7c0000, 
		0x7fc00000, 
		0x42140000, 
		0xff7fffff, 
		0x5e9061b0, 
		0xaaaaaaaa, 
		0xdfab95b4, 
		0xc2680000, 
		0x2a8bf284, 
		0xc23c0000, 
		0xff800001, 
		0x0e00ffff, 
		0xa8a52330, 
		0x842f1d5f, 
		0x027ee351, 
		0x425c0000, 
		0x80800002, 
		0x0e000003, 
		0xc1400000, 
		0x421c0000, 
		0x42240000, 
		0x41900000, 
		0x8ab9ea9a, 
		0xc20c0000, 
		0x80000000, 
		0x0c7ffff8, 
		0x345efa33, 
		0x2de0ed4a, 
		0x7fbfffff, 
		0x00400000, 
		0x5d82d839, 
		0x40667b8b, 
		0x80000200, 
		0xc1900000, 
		0x7fc00000, 
		0x80010000, 
		0x0c600000, 
		0xc1c80000, 
		0x0e00000f, 
		0x80000000, 
		0x0e07ffff, 
		0x557a7ccc, 
		0xc2240000, 
		0xaf5541e1, 
		0xa5c823db, 
		0xc1a80000, 
		0x0c400000, 
		0x41200000, 
		0x7a80f603, 
		0x80000004, 
		0x2d261340, 
		0x00000000, 
		0x7fbfffff, 
		0x2335cab2, 
		0x42380000, 
		0xc0c00000, 
		0xa19656ac, 
		0xc21c0000, 
		0x00001000, 
		0x0c7ffff8, 
		0xff7fffff, 
		0x46d4d953, 
		0x0e1fffff, 
		0x0e00001f, 
		0x0c7fffe0, 
		0x0c400000, 
		0x7f000000, 
		0x42340000, 
		0xa69b5243, 
		0x72b6d880, 
		0x0c7ffc00, 
		0x00020000, 
		0x80000000, 
		0x0e0003ff, 
		0x00000020, 
		0x42480000, 
		0x96af6ca5, 
		0x2fa56024, 
		0x80000010, 
		0xcfa70309, 
		0x80100000, 
		0x40400000, 
		0x80000004, 
		0xff800000, 
		0xc1e80000, 
		0x00000000, 
		0x007ffffe, 
		0x7f800000, 
		0xbc2f9dda, 
		0xc1a80000, 
		0x0e007fff, 
		0x00000080, 
		0x41400000, 
		0x0c7fffc0, 
		0xc1e00000, 
		0x389470ee, 
		0x80040000, 
		0x00000000, 
		0x0c7f0000, 
		0x803683ae, 
		0xc1a00000, 
		0x80000000, 
		0x00000000, 
		0x535f1655, 
		0x80000000, 
		0x41500000, 
		0x51622358, 
		0x0f7ffffc, 
		0xc26c0000, 
		0x18ce06ba, 
		0xe2790c92, 
		0x7fbfffff, 
		0xfe504538, 
		0x00000000, 
		0x41980000, 
		0x424c0000, 
		0x92af0a45, 
		0x0c7fff80, 
		0x41d80000, 
		0x40400000, 
		0x80000008, 
		0x00008000, 
		0xc2180000, 
		0xc1400000, 
		0xff800000, 
		0x05053434, 
		0x96d881bb, 
		0x0aaf5452, 
		0x0e00001f, 
		0x41f80000, 
		0x00002000, 
		0xb53147ea, 
		0x55555555, 
		0x80008000, 
		0x41e80000, 
		0x80ffffff, 
		0x0ff86c71, 
		0xda06fd1e, 
		0xffffffff, 
		0xb8141307, 
		0xaa992a94, 
		0x00000000, 
		0xc2740000, 
		0xc2300000, 
		0x41400000, 
		0xc2740000, 
		0xa6a52bb3, 
		0x42300000, 
		0xc2440000, 
		0x80800001, 
		0x8e715c63, 
		0xc1a00000, 
		0x80040000, 
		0xc2700000, 
		0x7f800000, 
		0x00800001, 
		0x2fe0dbf5, 
		0x0c7ffffc, 
		0x80080000, 
		0xffd100e6, 
		0x42600000, 
		0x00000000, 
		0xc1400000, 
		0x00002000, 
		0xff800000, 
		0x00100000, 
		0x427c0000, 
		0x0e000003, 
		0xff7ffffe, 
		0x00000100, 
		0x80000100, 
		0x7fffffff, 
		0x4b8c4b40, 
		0xc1b80000, 
		0xffc00000, 
		0x426c0000, 
		0x7fb1d13a, 
		0x0c700000, 
		0x7f800000, 
		0x80080000, 
		0x42540000, 
		0xc0c00000, 
		0x0c7c0000, 
		0x0e001fff, 
		0x108c063b, 
		0xc1100000, 
		0x80000000, 
		0x0c7fff80, 
		0xc5e83286, 
		0x7f9485a6, 
		0xc2300000, 
		0x00800002, 
		0x3f800001, 
		0x0117de53, 
		0x0f7fffff, 
		0x1e6ab338, 
		0x0c780000, 
		0x80000000, 
		0x00000000, 
		0xc20c0000, 
		0x426c0000, 
		0x0c7ffffe, 
		0x00000001, 
		0x3c71a07a, 
		0x0f7ffffe, 
		0x7841e2e6, 
		0x80000002, 
		0x7445e5a6, 
		0x41300000, 
		0x0c7f0000  
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
volatile uint64_t check_sum_hid_2[16] __attribute__ ((aligned (1024))) = {
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
volatile uint64_t check_sum_hid_3[16] __attribute__ ((aligned (1024))) = {
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
volatile uint64_t check_sum_hid_4[16] __attribute__ ((aligned (1024))) = {
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
volatile uint64_t check_sum_hid_5[16] __attribute__ ((aligned (1024))) = {
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
volatile uint64_t check_sum_hid_6[16] __attribute__ ((aligned (1024))) = {
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
volatile uint64_t check_sum_hid_7[16] __attribute__ ((aligned (1024))) = {
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
volatile uint64_t check_sum_hid_8[16] __attribute__ ((aligned (1024))) = {
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
volatile uint64_t check_sum_hid_9[16] __attribute__ ((aligned (1024))) = {
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
volatile uint64_t check_sum_hid_10[16] __attribute__ ((aligned (1024))) = {
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
volatile uint64_t check_sum_hid_11[16] __attribute__ ((aligned (1024))) = {
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
volatile uint64_t check_sum_hid_12[16] __attribute__ ((aligned (1024))) = {
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
volatile uint64_t check_sum_hid_13[16] __attribute__ ((aligned (1024))) = {
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
volatile uint64_t check_sum_hid_14[16] __attribute__ ((aligned (1024))) = {
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
volatile uint64_t check_sum_hid_15[16] __attribute__ ((aligned (1024))) = {
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
		0x00000389, 
		0x0000029f, 
		0x00000285, 
		0x00000193, 
		0x00000363, 
		0x000001bf, 
		0x000001b3, 
		0x00000391, 
		0x00000195, 
		0x000000f3, 
		0x000000b1, 
		0x000001ad, 
		0x00000083, 
		0x0000001f, 
		0x0000020d, 
		0x000000d9, 
		0x00000105, 
		0x0000035d, 
		0x00000133, 
		0x0000017d, 
		0x000003b1, 
		0x000000a1, 
		0x00000183, 
		0x00000057, 
		0x00000323, 
		0x000001ef, 
		0x00000041, 
		0x000002b3, 
		0x0000037f, 
		0x000003ef, 
		0x0000038d, 
		0x000002a3, 
		0x000000d5, 
		0x00000283, 
		0x000000af, 
		0x000001d9, 
		0x0000001b, 
		0x000003a3, 
		0x000001df, 
		0x0000012f, 
		0x00000197, 
		0x000003d1, 
		0x0000021f, 
		0x000000f5, 
		0x00000317, 
		0x00000339, 
		0x000002cd, 
		0x000002b1, 
		0x000002e3, 
		0x00000387, 
		0x00000155, 
		0x000002cd, 
		0x0000030d, 
		0x000003c3, 
		0x0000032f, 
		0x000001b7, 
		0x00000371, 
		0x0000032b, 
		0x000003cb, 
		0x000002e9, 
		0x00000379, 
		0x00000045, 
		0x000000c9, 
		0x000003c5, 
		0x000002ab, 
		0x0000000f, 
		0x000002c5, 
		0x000003d7, 
		0x00000293, 
		0x0000022d, 
		0x00000001, 
		0x0000019d, 
		0x000002db, 
		0x00000095, 
		0x0000000d, 
		0x00000369, 
		0x000002cd, 
		0x00000081, 
		0x000000e7, 
		0x00000335, 
		0x00000305, 
		0x00000331, 
		0x000001a9, 
		0x00000243, 
		0x00000099, 
		0x00000361, 
		0x000000fb, 
		0x000002c7, 
		0x000001a1, 
		0x000001bf, 
		0x0000023f, 
		0x00000187, 
		0x00000013, 
		0x0000037b, 
		0x000003d1, 
		0x00000377, 
		0x00000011, 
		0x00000399, 
		0x00000249, 
		0x0000036b, 
		0x000001db, 
		0x00000203, 
		0x0000037d, 
		0x00000273, 
		0x0000030f, 
		0x000000db, 
		0x000001ed, 
		0x000001cd, 
		0x000001af, 
		0x0000007b, 
		0x000000f3, 
		0x000003f1, 
		0x00000043, 
		0x0000002d, 
		0x000001ed, 
		0x0000039d, 
		0x000001c7, 
		0x0000030f, 
		0x000003bf, 
		0x000000c7, 
		0x000003e3, 
		0x00000223, 
		0x0000002b, 
		0x00000209, 
		0x00000327, 
		0x000000a5, 
		0x00000147, 
		0x000003f9, 
		0x00000335, 
		0x00000171, 
		0x00000087, 
		0x00000073, 
		0x0000014f, 
		0x0000037f, 
		0x00000245, 
		0x00000243, 
		0x00000019, 
		0x0000005b, 
		0x000002c1, 
		0x0000035f, 
		0x00000279, 
		0x0000035f, 
		0x00000091, 
		0x00000147, 
		0x0000006f, 
		0x00000207, 
		0x00000381, 
		0x00000143, 
		0x00000359, 
		0x00000035, 
		0x00000329, 
		0x000003c7, 
		0x0000022b, 
		0x000001cf, 
		0x0000031b, 
		0x00000267, 
		0x0000008d, 
		0x000001f1, 
		0x0000002b, 
		0x00000375, 
		0x0000025b, 
		0x00000301, 
		0x00000047, 
		0x0000005d, 
		0x000003df, 
		0x000002cd, 
		0x000002ed, 
		0x0000030f, 
		0x00000131, 
		0x0000026f, 
		0x000003af, 
		0x00000121, 
		0x00000285, 
		0x000000cd, 
		0x0000018f, 
		0x0000000f, 
		0x00000237, 
		0x0000010f, 
		0x00000283, 
		0x0000010f, 
		0x00000133, 
		0x000002e3, 
		0x00000073, 
		0x00000115, 
		0x000002c9, 
		0x000000d9, 
		0x00000211, 
		0x00000091, 
		0x00000327, 
		0x0000027b, 
		0x00000021, 
		0x0000031d, 
		0x00000241, 
		0x000002d1, 
		0x000000a9, 
		0x0000015b, 
		0x00000137, 
		0x000003f1, 
		0x000000db, 
		0x00000351, 
		0x0000007b, 
		0x000002bf, 
		0x00000205, 
		0x00000059, 
		0x000003b1, 
		0x000000c9, 
		0x00000103, 
		0x00000127, 
		0x000003e9, 
		0x00000043, 
		0x00000387, 
		0x00000253, 
		0x0000028d, 
		0x000002d5, 
		0x0000020f, 
		0x000002b3, 
		0x00000113, 
		0x00000319, 
		0x000003bf, 
		0x0000014f, 
		0x000001d7, 
		0x0000019b, 
		0x000003c1, 
		0x000002df, 
		0x000002b9, 
		0x00000037, 
		0x000000a9, 
		0x00000033, 
		0x000000c7, 
		0x00000201, 
		0x000000a3, 
		0x00000329, 
		0x00000127, 
		0x0000023d, 
		0x00000199, 
		0x000002cb, 
		0x00000203, 
		0x0000027f, 
		0x00000241, 
		0x00000239, 
		0x000000bf, 
		0x0000031b, 
		0x000003f1, 
		0x00000393, 
		0x000000b5, 
		0x000003d9, 
		0x0000023d, 
		0x000001e3, 
		0x0000016d, 
		0x000002fb, 
		0x00000119, 
		0x00000049, 
		0x000000a1, 
		0x0000013b, 
		0x000002eb, 
		0x000001c7  
};

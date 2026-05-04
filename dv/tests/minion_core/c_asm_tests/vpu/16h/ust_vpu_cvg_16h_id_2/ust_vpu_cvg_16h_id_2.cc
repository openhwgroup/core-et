/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>
#include "minion.h"
#include "macros.h"
#include "../ust_vpu_utils.h"

#define VSNIP_RSV           : :  : "f31","x21","x24","x29","x30","x31"

void load_vpu_regs(void){
		__asm__ __volatile__ ( 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f0,  0 (x5)\n"
			"flw.ps f1,  32 (x5)\n"
			"flw.ps f2,  64 (x5)\n"
			"flw.ps f3,  96 (x5)\n"
			"flw.ps f4,  128 (x5)\n"
			"flw.ps f5,  160 (x5)\n"
			"flw.ps f6,  192 (x5)\n"
			"flw.ps f7,  224 (x5)\n"
			"flw.ps f8,  256 (x5)\n"
			"flw.ps f9,  288 (x5)\n"
			"flw.ps f10,  320 (x5)\n"
			"flw.ps f11,  352 (x5)\n"
			"flw.ps f12,  384 (x5)\n"
			"flw.ps f13,  416 (x5)\n"
			"flw.ps f14,  448 (x5)\n"
			"flw.ps f15,  480 (x5)\n"
			"flw.ps f16,  512 (x5)\n"
			"flw.ps f17,  544 (x5)\n"
			"flw.ps f18,  576 (x5)\n"
			"flw.ps f19,  608 (x5)\n"
			"flw.ps f20,  640 (x5)\n"
			"flw.ps f21,  672 (x5)\n"
			"flw.ps f22,  704 (x5)\n"
			"flw.ps f23,  736 (x5)\n"
			"flw.ps f24,  768 (x5)\n"
			"flw.ps f25,  800 (x5)\n"
			"flw.ps f26,  832 (x5)\n"
			"flw.ps f27,  864 (x5)\n"
			"flw.ps f28,  896 (x5)\n"
			"flw.ps f29,  928 (x5)\n"
			"flw.ps f30,  960 (x5)\n"
			"flw.ps f31,  992 (x5)\n"
		       VSNIP_RSV     
		);
}
void trans(void){
		__asm__ __volatile__ ( 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_int_32\n"
			"flw.ps f0, 0 (x5)\n"
			"la x5, rand_ieee754_32\n"
			"flw.ps f1, 0 (x5)\n"
			"li x5, 142\n"
			"faddi.pi f2, f0, 424\n"
			"fadd.pi f3, f2, f1\n"
			"fadd.pi f4, f3, f1\n"
			"fadd.pi f5, f4, f1\n"
			"fadd.pi f6, f5, f1\n"
			"fadd.pi f7, f6, f1\n"
			"fadd.pi f8, f7, f1\n"
			"fadd.pi f9, f8, f1\n"
			"fadd.pi f10, f9, f1\n"
			"fadd.pi f11, f10, f1\n"
			"fadd.pi f12, f11, f1\n"
			"fadd.pi f13, f12, f1\n"
			"fadd.pi f14, f13, f1\n"
			"fadd.pi f15, f14, f1\n"
			"fadd.pi f16, f15, f1\n"
			"fadd.pi f17, f16, f1\n"
			"fadd.pi f18, f17, f1\n"
		"1:\n"
			"flog.ps f18, f2\n"
			"flog.ps f19, f3\n"
			"flog.ps f20, f4\n"
			"flog.ps f21, f5\n"
			"flog.ps f22, f6\n"
			"flog.ps f23, f7\n"
			"flog.ps f24, f8\n"
			"flog.ps f25, f9\n"
			"flog.ps f26, f10\n"
			"flog.ps f27, f11\n"
			"flog.ps f28, f12\n"
			"flog.ps f29, f13\n"
			"flog.ps f30, f14\n"
			"flog.ps f31, f15\n"
			"flog.ps f0, f16\n"
			"flog.ps f1, f17\n"
			"faddi.pi f2, f2, 128\n"
			"faddi.pi f3, f3, 128\n"
			"faddi.pi f4, f4, 128\n"
			"faddi.pi f5, f5, 128\n"
			"faddi.pi f6, f6, 128\n"
			"faddi.pi f7, f7, 128\n"
			"faddi.pi f8, f8, 128\n"
			"faddi.pi f9, f9, 128\n"
			"faddi.pi f10, f10, 128\n"
			"faddi.pi f11, f11, 128\n"
			"faddi.pi f12, f12, 128\n"
			"faddi.pi f13, f13, 128\n"
			"faddi.pi f14, f14, 128\n"
			"faddi.pi f15, f15, 128\n"
			"faddi.pi f16, f16, 128\n"
			"faddi.pi f17, f17, 128\n"
			"addi x5, x5, -1\n"
			"bne x5, x0, 1b\n"
		       VSNIP_RSV     
		);
}
void trans_exhaustive(void){
		__asm__ __volatile__ ( 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, id_int_32\n"
			"flw.ps f0, 0 (x5)\n"
			"la x5, constants_int_32\n"
			"flw.ps f1, 0 (x5)\n"
			"li x5, 33554432\n"
			"faddi.pi f2, f0, 0\n"
			"fadd.pi f3, f2, f1\n"
			"fadd.pi f4, f3, f1\n"
			"fadd.pi f5, f4, f1\n"
			"fadd.pi f6, f5, f1\n"
			"fadd.pi f7, f6, f1\n"
			"fadd.pi f8, f7, f1\n"
			"fadd.pi f9, f8, f1\n"
			"fadd.pi f10, f9, f1\n"
			"fadd.pi f11, f10, f1\n"
			"fadd.pi f12, f11, f1\n"
			"fadd.pi f13, f12, f1\n"
			"fadd.pi f14, f13, f1\n"
			"fadd.pi f15, f14, f1\n"
			"fadd.pi f16, f15, f1\n"
			"fadd.pi f17, f16, f1\n"
			"fadd.pi f18, f17, f1\n"
		"1:\n"
			"flog.ps f18, f2\n"
			"flog.ps f19, f3\n"
			"flog.ps f20, f4\n"
			"flog.ps f21, f5\n"
			"flog.ps f22, f6\n"
			"flog.ps f23, f7\n"
			"flog.ps f24, f8\n"
			"flog.ps f25, f9\n"
			"flog.ps f26, f10\n"
			"flog.ps f27, f11\n"
			"flog.ps f28, f12\n"
			"flog.ps f29, f13\n"
			"flog.ps f30, f14\n"
			"flog.ps f31, f15\n"
			"flog.ps f0, f16\n"
			"flog.ps f1, f17\n"
			"faddi.pi f2, f2, 128\n"
			"faddi.pi f3, f3, 128\n"
			"faddi.pi f4, f4, 128\n"
			"faddi.pi f5, f5, 128\n"
			"faddi.pi f6, f6, 128\n"
			"faddi.pi f7, f7, 128\n"
			"faddi.pi f8, f8, 128\n"
			"faddi.pi f9, f9, 128\n"
			"faddi.pi f10, f10, 128\n"
			"faddi.pi f11, f11, 128\n"
			"faddi.pi f12, f12, 128\n"
			"faddi.pi f13, f13, 128\n"
			"faddi.pi f14, f14, 128\n"
			"faddi.pi f15, f15, 128\n"
			"faddi.pi f16, f16, 128\n"
			"faddi.pi f17, f17, 128\n"
			"addi x5, x5, -1\n"
			"bne x5, x0, 1b\n"
		       VSNIP_RSV     
		);
}
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
	uint64_t sync_odd_harts;
	hid = get_hart_id();
	if ( hid%16 == 0 ) { 
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
			"csrwi mhpmevent3, 16\n"   
			"csrwi mhpmevent4, 6\n"   
			"csrwi mhpmevent5, 4\n"   
			"csrwi mhpmevent6, 27\n"   
			"csrwi mhpmevent7, 6\n"   
			"csrwi mhpmevent8, 3\n"   
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
			"flw.ps f25,  416 (x5)\n"
			"flw.ps f17,  288 (x5)\n"
			"flw.ps f16,  448 (x5)\n"
			"flw.ps f28,  0 (x5)\n"
			"flw.ps f13,  960 (x5)\n"
			"flw.ps f22,  256 (x5)\n"
			"flw.ps f4,  160 (x5)\n"
			"flw.ps f9,  544 (x5)\n"
			"flw.ps f29,  32 (x5)\n"
			"flw.ps f6,  736 (x5)\n"
			"flw.ps f14,  672 (x5)\n"
			"flw.ps f24,  320 (x5)\n"
			"flw.ps f10,  608 (x5)\n"
			"flw.ps f31,  384 (x5)\n"
			"flw.ps f15,  192 (x5)\n"
			"flw.ps f23,  576 (x5)\n"
			"flw.ps f30,  992 (x5)\n"
			"flw.ps f19,  864 (x5)\n"
			"flw.ps f8,  512 (x5)\n"
			"flw.ps f20,  768 (x5)\n"
			"flw.ps f21,  704 (x5)\n"
			"flw.ps f0,  832 (x5)\n"
			"flw.ps f1,  640 (x5)\n"
			"flw.ps f11,  224 (x5)\n"
			"flw.ps f2,  128 (x5)\n"
			"flw.ps f3,  352 (x5)\n"
			"flw.ps f7,  96 (x5)\n"
			"flw.ps f27,  64 (x5)\n"
			"flw.ps f18,  480 (x5)\n"
			"flw.ps f26,  800 (x5)\n"
			"flw.ps f5,  896 (x5)\n"
			"flw.ps f12,  928 (x5)\n"
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
			"li x31, CREDINC0\n" 
			"sd x0, 0(x31)\n"
			"li x31, CREDINC1\n" 
			"sd x0, 0(x31)\n"
			"li x31, CREDINC2\n" 
			"sd x0, 0(x31)\n"
			"li x31, CREDINC3\n" 
			"sd x0, 0(x31)\n"
			"fence\n"
			"LBL_INIT_FLB_CTRS_HID_0:\n"          
			"li x31, BARRIER0\n"
			"sd x0, 0(x31)\n"
			"li x31, BARRIER1\n"
			"sd x0, 0(x31)\n"
			"li x31, BARRIER2\n"
			"sd x0, 0(x31)\n"
			"li x31, BARRIER3\n"
			"sd x0, 0(x31)\n"
			"li x31, BARRIER4\n"
			"sd x0, 0(x31)\n"
			"li x31, BARRIER5\n"
			"sd x0, 0(x31)\n"
			"li x31, BARRIER6\n"
			"sd x0, 0(x31)\n"
			"li x31, BARRIER7\n"
			"sd x0, 0(x31)\n"
			"li x31, BARRIER8\n"
			"sd x0, 0(x31)\n"
			"li x31, BARRIER9\n"
			"sd x0, 0(x31)\n"
			"li x31, BARRIER10\n"
			"sd x0, 0(x31)\n"
			"li x31, BARRIER11\n"
			"sd x0, 0(x31)\n"
			"li x31, BARRIER12\n"
			"sd x0, 0(x31)\n"
			"li x31, BARRIER13\n"
			"sd x0, 0(x31)\n"
			"li x31, BARRIER14\n"
			"sd x0, 0(x31)\n"
			"li x31, BARRIER15\n"
			"sd x0, 0(x31)\n"
			"li x31, BARRIER16\n"
			"sd x0, 0(x31)\n"
			"li x31, BARRIER17\n"
			"sd x0, 0(x31)\n"
			"li x31, BARRIER18\n"
			"sd x0, 0(x31)\n"
			"li x31, BARRIER19\n"
			"sd x0, 0(x31)\n"
			"li x31, BARRIER20\n"
			"sd x0, 0(x31)\n"
			"li x31, BARRIER21\n"
			"sd x0, 0(x31)\n"
			"li x31, BARRIER22\n"
			"sd x0, 0(x31)\n"
			"li x31, BARRIER23\n"
			"sd x0, 0(x31)\n"
			"li x31, BARRIER24\n"
			"sd x0, 0(x31)\n"
			"li x31, BARRIER25\n"
			"sd x0, 0(x31)\n"
			"li x31, BARRIER26\n"
			"sd x0, 0(x31)\n"
			"li x31, BARRIER27\n"
			"sd x0, 0(x31)\n"
			"li x31, BARRIER28\n"
			"sd x0, 0(x31)\n"
			"li x31, BARRIER29\n"
			"sd x0, 0(x31)\n"
			"li x31, BARRIER30\n"
			"sd x0, 0(x31)\n"
			"li x31, BARRIER31\n"
			"sd x0, 0(x31)\n"
			"fence\n"

			"csrr x5, mhartid\n"
			"srli x5, x5, 4\n"
			"andi x5, x5, 3\n"
			"slli x5, x5, 3\n"
			"li x31, 0xff\n"
			"sll x31, x31, x5\n"

			"li x5, CREDINC0\n"
			"sd x31, 0(x5)\n"
			"li x5, CREDINC2\n"
			"sd x31, 0(x5)\n"
			"fence\n"
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=1); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"LBL_KERNEL_HID_0:\n"          
			"li x5, 0xFEEDFEED\n"
			"li x0, 0x12c\n" 
			"slti x0, x0, 0x7f2\n"
			"la x31, check_sum_hid_0\n"
			"li x5, 0x7\n" 
			"sw x5, (x31)\n"
			"li x29, 0x7\n"
			"LBL_OUTER_LOOP_SEQ_HID_0:\n"
			"nop \n"
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=1); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"nop \n"
			"li x30, 0\n"
			"addi x30, x30, 1\n"
			"la x23, rand_ieee754_32\n"
			"flw.ps f10, 64(x23)\n"
			"flw.ps f11, 608(x23)\n"
			"fsub.ps f3, f10, f11, rup\n" 
			"la x10, rand_int_32\n"
			"flw.ps f8, 608(x10)\n"
			"fsatu8.pi f5, f8\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f24, 128(x23)\n"
			"fswizz.ps f3, f24, 0x1c\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f1, 640(x15)\n"
			"flw.ps f24, 864(x15)\n"
			"flw.ps f3, 128(x15)\n"
			"fmsub.s f1, f1, f24, f3, rtz\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f28, 800(x7)\n"
			"fcmovm.ps f28, f28, f28\n" 
			"li x5, 0xbe2a272978b902fd\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0xa23a1bdf0cc8cdeb\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"la x14, rand_int_32\n"
			"flw.ps f5, 0(x14)\n"
			"flw.ps f11, 448(x14)\n"
			"fmulhu.pi f28, f5, f11\n" 
			"la x20, rand_int_32\n"
			"flw.ps f16, 448(x20)\n"
			"flw.ps f8, 928(x20)\n"
			"fand.pi f5, f16, f8\n" 
			"fbci.pi f16, 0x716b7\n" 
			"li x5, 0xd76561daca73cb1d\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0xea775cb8261f028d\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"la x26, rand_int_32\n"
			"flw.ps f5, 160(x26)\n"
			"flw.ps f31, 928(x26)\n"
			"fltu.pi f17, f5, f31\n" 
			"la x26, rand_int_32\n"
			"flw.ps f17, 0(x26)\n"
			"faddi.pi f15, f17, 0xd7\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_0:\n"
			"masknot m7, m4\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x10, rand_int_32\n"
			"flw.ps f27, 960(x10)\n"
			"fsat8.pi f5, f27\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_0:\n"
			"fltm.ps m4, f11, f16\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0x8568bc2bd7\n"
			"flw.ps f5, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x10, rand_int_32\n"
			"flw.ps f3, 704(x10)\n"
			"fsatu8.pi f15, f3\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f30, 704(x10)\n"
			"flw.ps f15, 64(x10)\n"
			"fsub.ps f23, f30, f15, rmm\n" 
			"la x7, rand_int_32\n"
			"flw.ps f23, 32(x7)\n"
			"fsrli.pi f24, f23, 0xa\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_0_RDX_ID_0_SHIRE_0_HID_0:\n"
			"li x27, 0x2c00000000110030\n"
			"csrr x5, mhartid\n"
			"srli x5, x5, 4\n"
			"slli x5, x5, 6\n"
			"add x27, x27, x5\n"
			"csrw tensor_reduce, x27\n" 
			"csrwi tensor_wait, 9\n" 
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=0); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ITER_0_HID_0:\n"
			"li x5, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x3 << TENSOR_IMA_B_NUM_COLS) | (0xf << TENSOR_IMA_A_NUM_ROWS) | (0x4 << TENSOR_IMA_A_NUM_COLS) | (0x8 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0xd0 << TENSOR_IMA_SCP_LOC_B) | (0x1b << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ITER_1_HID_0:\n"
			"li x5, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x3 << TENSOR_FMA32_B_NUM_COLS) | (0x5 << TENSOR_FMA32_A_NUM_ROWS) | (0x6 << TENSOR_FMA32_A_NUM_COLS) | (0xa << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x3d << TENSOR_FMA32_SCP_LOC_B) | (0x30 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x0 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_0\n"
			"csrw tensor_error, x0\n"
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_0\n"
			"addi x30, x30, 1\n"
			"la x10, rand_int_32\n"
			"flw.ps f26, 608(x10)\n"
			"flw.ps f28, 192(x10)\n"
			"fmulhu.pi f23, f26, f28\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f10, 64(x22)\n"
			"ffrc.ps f3, f10\n" 
			"la x26, rand_int_32\n"
			"flw.ps f5, 992(x26)\n"
			"flw.ps f11, 480(x26)\n"
			"fadd.pi f15, f5, f11\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f11, 224(x27)\n"
			"fcvt.wu.s x10, f11, rtz\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f10, 800(x26)\n"
			"flw.ps f11, 320(x26)\n"
			"flw.ps f3, 608(x26)\n"
			"fmadd.ps f31, f10, f11, f3, rtz\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f31, 768(x10)\n"
			"flw.ps f17, 800(x10)\n"
			"flw.ps f23, 192(x10)\n"
			"fmadd.ps f30, f31, f17, f23, rdn\n" 
			"la x20, rand_int_32\n"
			"flw.ps f24, 608(x20)\n"
			"flw.ps f10, 704(x20)\n"
			"fmaxu.pi f5, f24, f10\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f16, 928(x15)\n"
			"flw.ps f5, 192(x15)\n"
			"flw.ps f30, 384(x15)\n"
			"fmsub.s f10, f16, f5, f30, rdn\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f30, 288(x15)\n"
			"frsq.ps f11, f30\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f3, 320(x27)\n"
			"frsq.ps f10, f3\n" 
			"la x5, rand_int_32\n"
			"flw.ps f17, 480(x5)\n"
			"fcvt.ps.pwu f31, f17, rne\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f10, 672(x20)\n"
			"flw.ps f5, 384(x20)\n"
			"fsgnjn.ps f31, f10, f5\n" 
			"la x10, rand_int_32\n"
			"flw.ps f31, 544(x10)\n"
			"flw.ps f26, 224(x10)\n"
			"fsll.pi f26, f31, f26\n" 
			"la x23, rand_int_32\n"
			"flw.ps f11, 480(x23)\n"
			"flw.ps f28, 288(x23)\n"
			"fmulh.pi f26, f11, f28\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f27, 32(x20)\n"
			"flw.ps f23, 160(x20)\n"
			"fmax.s f3, f27, f23\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f8, 896(x14)\n"
			"flw.ps f10, 352(x14)\n"
			"fmax.ps f3, f8, f10\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_1_RDX_ID_1_SHIRE_0_HID_0:\n"
			"li x10, 0xa0000000010000a\n"
			"csrw tensor_reduce, x10\n" 
			"csrwi tensor_wait, 9\n" 
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ID_294475_HID_0:\n"
			"li x5, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x3 << TENSOR_FMA16_B_NUM_COLS) | (0xc << TENSOR_FMA16_A_NUM_ROWS) | (0xa << TENSOR_FMA16_A_NUM_COLS) | (0xe << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0xb << TENSOR_FMA16_SCP_LOC_B) | (0xdd << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x0 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x6, ((0x1e << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0xc << TENSOR_QUANT_QUANT_ROW) | (0x25 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x6 << TENSOR_QUANT_TRANSF4) | (0x7 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ID_482865_HID_0:\n"
			"li x5, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x2 << TENSOR_IMA_B_NUM_COLS) | (0x2 << TENSOR_IMA_A_NUM_ROWS) | (0x1 << TENSOR_IMA_A_NUM_COLS) | (0x5 << TENSOR_IMA_OFFSET) | (0x0 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0xfe << TENSOR_IMA_SCP_LOC_B) | (0x3b << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x0 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_0\n"
			"csrw tensor_error, x0\n"
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=0); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"LBL_SEQID_1_TLD_XMA_STR_SEQ_ITER_0_HID_0:\n"
			"la x31, self_check_8\n"
			"li x5, 0xe0000000000008\n"
			"add x5, x5, x31\n"
			"li x6, 0x1000000000000d\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x3 << TENSOR_IMA_B_NUM_COLS) | (0x8 << TENSOR_IMA_A_NUM_ROWS) | (0xd << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x1 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1d << TENSOR_IMA_SCP_LOC_B) | (0x7 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x9, ((0x1a << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0x8 << TENSOR_QUANT_QUANT_ROW) | (0x15 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x1 << TENSOR_QUANT_TRANSF9) | (0x3 << TENSOR_QUANT_TRANSF8) | (0x5 << TENSOR_QUANT_TRANSF7) | (0x3 << TENSOR_QUANT_TRANSF6) | (0x4 << TENSOR_QUANT_TRANSF5) | (0x8 << TENSOR_QUANT_TRANSF4) | (0xa << TENSOR_QUANT_TRANSF3) | (0x3 << TENSOR_QUANT_TRANSF2) | (0x8 << TENSOR_QUANT_TRANSF1) | (0x8 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fmvz.x.ps x31, f0, 0\n"
			"li x5, 0x38\n"
			"bne x5, x31, LBL_FAIL_HID_0\n"
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 10\n"
			"LBL_SEQID_1_TLD_XMA_STR_SEQ_ITER_1_HID_0:\n"
			"la x31, self_check_8\n"
			"li x5, 0x40000000000005\n"
			"add x5, x5, x31\n"
			"li x6, 0x1000000000000c\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x3 << TENSOR_IMA_B_NUM_COLS) | (0x5 << TENSOR_IMA_A_NUM_ROWS) | (0xc << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x1 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1b << TENSOR_IMA_SCP_LOC_B) | (0x2 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x9, ((0x16 << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0x5 << TENSOR_QUANT_QUANT_ROW) | (0x16 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x1 << TENSOR_QUANT_TRANSF7) | (0x3 << TENSOR_QUANT_TRANSF6) | (0xa << TENSOR_QUANT_TRANSF5) | (0x5 << TENSOR_QUANT_TRANSF4) | (0x9 << TENSOR_QUANT_TRANSF3) | (0x9 << TENSOR_QUANT_TRANSF2) | (0x8 << TENSOR_QUANT_TRANSF1) | (0xa << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fmvz.x.ps x31, f0, 0\n"
			"li x5, 0x34\n"
			"bne x5, x31, LBL_FAIL_HID_0\n"
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_0\n"
			"csrw tensor_error, x0\n"
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_0\n"
			"addi x30, x30, 1\n"
			"la x22, rand_ieee754_32\n"
			"flw.ps f31, 0(x22)\n"
			"fcvt.w.s x14, f31, rdn\n" 
			"mova.x.m x15\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f30, 992(x23)\n"
			"flw.ps f27, 864(x23)\n"
			"fcmovm.ps f24, f30, f27\n" 
			"LBL_SEQID_2_M0_WRITE_ID_2_HID_0:\n"
			"mov.m.x m7, x5, 0x2c\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x20, rand_int_32\n"
			"flw.ps f8, 64(x20)\n"
			"flw.ps f24, 96(x20)\n"
			"fmax.pi f15, f8, f24\n" 
			"li x5, 0x8881\n"
			"csrw tensor_mask, x5\n"	
			"la x7, rand_ieee754_32\n"
			"flw.ps f11, 736(x7)\n"
			"flw.ps f23, 224(x7)\n"
			"flw.ps f31, 576(x7)\n"
			"fnmadd.s f26, f11, f23, f31, rup\n" 
			"la x26, rand_int_32\n"
			"flw.ps f28, 672(x26)\n"
			"flw.ps f11, 384(x26)\n"
			"fminu.pi f5, f28, f11\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x27, rand_ieee754_32\n"
			"flw.ps f30, 160(x27)\n"
			"flw.ps f16, 480(x27)\n"
			"fle.s x14, f30, f16\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f27, 0(x14)\n"
			"flw.ps f5, 608(x14)\n"
			"fsgnj.ps f31, f27, f5\n" 
			"LBL_SEQID_2_M0_WRITE_ID_3_HID_0:\n"
			"maskor m2, m4, m0\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x22, rand_int_32\n"
			"flw.ps f15, 32(x22)\n"
			"flw.ps f10, 960(x22)\n"
			"fmul.pi f11, f15, f10\n" 
			"LBL_SEQID_2_M0_WRITE_ID_4_HID_0:\n"
			"flem.ps m2, f23, f15\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x23, rand_int_32\n"
			"flw.ps f24, 192(x23)\n"
			"flw.ps f28, 96(x23)\n"
			"fltu.pi f26, f24, f28\n" 
			"la x14, rand_int_32\n"
			"flw.ps f16, 96(x14)\n"
			"flw.ps f23, 480(x14)\n"
			"fmul.pi f27, f16, f23\n" 
			"la x14, rand_ieee754_16\n"
			"flw.ps f31, 416(x14)\n"
			"fcvt.ps.f16 f31, f31\n" 
			"LBL_SEQID_2_M0_WRITE_ID_5_HID_0:\n"
			"masknot m7, m0\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0xFF\n"
			"csrw tensor_error, zero\n" 
			"li x5, 0x20a2\n"
			"csrw tensor_mask, x5\n"	
			"LBL_SEQID_2_TLD_XMA_STR_SEQ_ID_891492_HID_0:\n"
			"la x31, self_check_8\n"
			"li x5, 0xc0000000000001\n"
			"add x5, x5, x31\n"
			"li x6, 0x34000000000000e\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x1 << TENSOR_IMA_B_NUM_COLS) | (0x1 << TENSOR_IMA_A_NUM_ROWS) | (0xe << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1a << TENSOR_IMA_SCP_LOC_B) | (0x6 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fmvz.x.ps x31, f0, 0\n"
			"li x5, 0x3c\n"
			"bne x5, x31, LBL_FAIL_HID_0\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_0\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_2_XMA_QNT_STR_SEQ_ITER_0_HID_0:\n"
			"li x5, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x0 << TENSOR_FMA32_B_NUM_COLS) | (0x9 << TENSOR_FMA32_A_NUM_ROWS) | (0x5 << TENSOR_FMA32_A_NUM_COLS) | (0xa << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x81 << TENSOR_FMA32_SCP_LOC_B) | (0x61 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x0 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"LBL_SEQID_2_XMA_QNT_STR_SEQ_ITER_1_HID_0:\n"
			"li x5, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x3 << TENSOR_IMA_B_NUM_COLS) | (0xe << TENSOR_IMA_A_NUM_ROWS) | (0x7 << TENSOR_IMA_A_NUM_COLS) | (0xb << TENSOR_IMA_OFFSET) | (0x0 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x3b << TENSOR_IMA_SCP_LOC_B) | (0xe7 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_0\n"
			"csrw tensor_error, x0\n"
			"li x5, 3\n" 
			"bne x30, x5, LBL_FAIL_HID_0\n"
			"nop\n"          
		       VSNIP_RSV     
		);
		load_vpu_regs();
		__asm__ __volatile__ ( 
			"nop\n"          
			"addi x30, x30, 1\n"
			"LBL_SEQID_3_FP_TRANS_ID_6_HID_0:\n"
			"la x10, rand_ieee754_32\n"
			"flw.ps f8, 544(x10)\n"
			"flog.ps f28, f8\n" 
			"la x5, rand_int_32\n"
			"flw.ps f15, 448(x5)\n"
			"flw.ps f30, 288(x5)\n"
			"fmulhu.pi f27, f15, f30\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f27, 224(x20)\n"
			"flw.ps f26, 704(x20)\n"
			"fadd.s f17, f27, f26, rup\n" 
			"la x26, rand_int_32\n"
			"flw.ps f26, 864(x26)\n"
			"fpackrepb.pi f31, f26\n" 
			"la x20, rand_int_32\n"
			"flw.ps f3, 448(x20)\n"
			"flw.ps f30, 224(x20)\n"
			"feq.pi f10, f3, f30\n" 
			"li x26, 0xeff48595fa4146c2\n"
			"fcvt.s.lu f31, x26, rdn\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f24, 32(x22)\n"
			"flw.ps f30, 288(x22)\n"
			"fmul.s f24, f24, f30, rup\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f31, 128(x20)\n"
			"flw.ps f24, 416(x20)\n"
			"feq.ps f15, f31, f24\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x23, rand_ieee754_32\n"
			"flw.ps f26, 608(x23)\n"
			"fswizz.ps f27, f26, 0x1b\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f27, 672(x5)\n"
			"flw.ps f16, 832(x5)\n"
			"fmul.ps f8, f27, f16, rtz\n" 
			"la x23, rand_int_32\n"
			"flw.ps f28, 32(x23)\n"
			"flw.ps f8, 832(x23)\n"
			"fremu.pi f28, f28, f8\n" 
			"la x15, rand_int_32\n"
			"flw.ps f10, 288(x15)\n"
			"fslli.pi f10, f10, 0x2\n" 
			"LBL_SEQID_3_M0_WRITE_ID_7_HID_0:\n"
			"feqm.ps m0, f30, f3\n" 
			"mov.m.x m0, x0, 0xff\n"
			"li x5, CREDINC1\n"
			"ld x5, 0(x5)\n"
			"la x27, rand_ieee754_32\n"
			"flw.ps f1, 672(x27)\n"
			"frsq.ps f3, f1\n" 
			"la x10, rand_int_32\n"
			"flw.ps f23, 480(x10)\n"
			"flw.ps f3, 512(x10)\n"
			"fminu.pi f5, f23, f3\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f31, 928(x10)\n"
			"flw.ps f11, 512(x10)\n"
			"flw.ps f10, 896(x10)\n"
			"fmadd.s f8, f31, f11, f10, rtz\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_3_XMA_QNT_STR_SEQ_ID_849017_HID_0:\n"
			"li x5, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x2 << TENSOR_IMA_B_NUM_COLS) | (0xf << TENSOR_IMA_A_NUM_ROWS) | (0x4 << TENSOR_IMA_A_NUM_COLS) | (0xd << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x9c << TENSOR_IMA_SCP_LOC_B) | (0xb6 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x0 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x6, ((0xd << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0xf << TENSOR_QUANT_QUANT_ROW) | (0xd << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x1 << TENSOR_QUANT_TRANSF6) | (0x5 << TENSOR_QUANT_TRANSF5) | (0x3 << TENSOR_QUANT_TRANSF4) | (0x5 << TENSOR_QUANT_TRANSF3) | (0x5 << TENSOR_QUANT_TRANSF2) | (0x8 << TENSOR_QUANT_TRANSF1) | (0x8 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 7\n"
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_0\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_3_TLD_XMA_STR_SEQ_ITER_0_HID_0:\n"
			"la x31, self_check_32\n"
			"li x5, 0x20000000000005\n"
			"add x5, x5, x31\n"
			"li x6, 0x300000000000007\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x3 << TENSOR_FMA32_B_NUM_COLS) | (0x5 << TENSOR_FMA32_A_NUM_ROWS) | (0x7 << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x18 << TENSOR_FMA32_SCP_LOC_B) | (0x1 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0x8\n"
			"bne x5, x31, LBL_FAIL_HID_0\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_0\n"
			"csrw tensor_error, x0\n"
			"li x5, 4\n" 
			"bne x30, x5, LBL_FAIL_HID_0\n"
			"nop\n"          
		       VSNIP_RSV     
		);
		trans();
		__asm__ __volatile__ ( 
			"nop\n"          
			"addi x30, x30, 1\n"
			"la x5, rand_ieee754_32\n"
			"flw.ps f23, 576(x5)\n"
			"ffrc.ps f27, f23\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f28, 480(x7)\n"
			"flw.ps f1, 672(x7)\n"
			"fsgnjx.ps f17, f28, f1\n" 
			"la x10, rand_int_32\n"
			"flw.ps f5, 736(x10)\n"
			"faddi.pi f5, f5, 0xb7\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f16, 896(x5)\n"
			"fcvt.f16.ps f31, f16\n" 
			"mova.x.m x26\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f1, 448(x26)\n"
			"fcvt.w.s x22, f1, rup\n" 
			"li x5, 0x845c5f6b7652c9e1\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0xb2149fac984190f\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"la x5, rand_ieee754_32\n"
			"flw.ps f26, 352(x5)\n"
			"fswizz.ps f15, f26, 0x12\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x7, rand_ieee754_32\n"
			"flw.ps f28, 448(x7)\n"
			"flw.ps f17, 64(x7)\n"
			"fmax.s f11, f28, f17\n" 
			"la x7, rand_int_32\n"
			"flw.ps f28, 96(x7)\n"
			"flw.ps f3, 672(x7)\n"
			"fand.pi f5, f28, f3\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f23, 832(x15)\n"
			"fclass.ps f17, f23\n" 
			"la x14, rand_int_32\n"
			"flw.ps f30, 512(x14)\n"
			"fcvt.ps.pw f11, f30, rne\n" 
			"fbci.pi f16, 0x65a9b\n" 
			"la x23, rand_int_32\n"
			"flw.ps f27, 448(x23)\n"
			"fpackreph.pi f8, f27\n" 
			"la x22, rand_int_32\n"
			"flw.ps f8, 160(x22)\n"
			"fsatu8.pi f31, f8\n" 
			"LBL_SEQID_4_M0_WRITE_ID_8_HID_0:\n"
			"maskxor m7, m7, m2\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"li x5, 0x346d\n"
			"csrw tensor_mask, x5\n"	
			"la x22, rand_ieee754_32\n"
			"flw.ps f31, 480(x22)\n"
			"flw.ps f8, 896(x22)\n"
			"flw.ps f3, 288(x22)\n"
			"fcmov.ps f16, f31, f8, f3\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_4_XMA_QNT_STR_SEQ_ID_545527_HID_0:\n"
			"li x5, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x3 << TENSOR_FMA16_B_NUM_COLS) | (0xb << TENSOR_FMA16_A_NUM_ROWS) | (0xc << TENSOR_FMA16_A_NUM_COLS) | (0x7 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x31 << TENSOR_FMA16_SCP_LOC_B) | (0x2f << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x0 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x6, ((0x6 << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0xb << TENSOR_QUANT_QUANT_ROW) | (0x2b << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x2 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 7\n"
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_0\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_4_XMA_QNT_STR_SEQ_ITER_0_HID_0:\n"
			"li x6, ((0x14 << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0x4 << TENSOR_QUANT_QUANT_ROW) | (0x7 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x1 << TENSOR_QUANT_TRANSF9) | (0xa << TENSOR_QUANT_TRANSF8) | (0x4 << TENSOR_QUANT_TRANSF7) | (0x4 << TENSOR_QUANT_TRANSF6) | (0x5 << TENSOR_QUANT_TRANSF5) | (0x5 << TENSOR_QUANT_TRANSF4) | (0x8 << TENSOR_QUANT_TRANSF3) | (0xa << TENSOR_QUANT_TRANSF2) | (0x9 << TENSOR_QUANT_TRANSF1) | (0xa << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_4_XMA_QNT_STR_SEQ_ITER_1_HID_0:\n"
			"li x5, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x3 << TENSOR_FMA16_B_NUM_COLS) | (0x6 << TENSOR_FMA16_A_NUM_ROWS) | (0xd << TENSOR_FMA16_A_NUM_COLS) | (0x9 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x4a << TENSOR_FMA16_SCP_LOC_B) | (0x17 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"LBL_SEQID_4_XMA_QNT_STR_SEQ_ITER_2_HID_0:\n"
			"li x5, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x2 << TENSOR_FMA32_B_NUM_COLS) | (0x5 << TENSOR_FMA32_A_NUM_ROWS) | (0x2 << TENSOR_FMA32_A_NUM_COLS) | (0x4 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0xda << TENSOR_FMA32_SCP_LOC_B) | (0xf8 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x6, ((0x3 << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0x5 << TENSOR_QUANT_QUANT_ROW) | (0x28 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x2 << TENSOR_QUANT_TRANSF9) | (0x6 << TENSOR_QUANT_TRANSF8) | (0x7 << TENSOR_QUANT_TRANSF7) | (0x6 << TENSOR_QUANT_TRANSF6) | (0x7 << TENSOR_QUANT_TRANSF5) | (0x6 << TENSOR_QUANT_TRANSF4) | (0x7 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 7\n"
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_0\n"
			"csrw tensor_error, x0\n"
			"li x5, 5\n" 
			"bne x30, x5, LBL_FAIL_HID_0\n"
			"nop\n"          
		       VSNIP_RSV     
		);
		load_vpu_regs();
		__asm__ __volatile__ ( 
			"nop\n"          
			"addi x30, x30, 1\n"
			"la x7, rand_ieee754_32\n"
			"flw.ps f8, 32(x7)\n"
			"flw.ps f30, 736(x7)\n"
			"fdiv.ps f15, f8, f30, rup\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f5, 32(x14)\n"
			"fcvt.w.s x7, f5, rdn\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f24, 128(x5)\n"
			"fdiv.ps f8, f24, f24, rdn\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f5, 896(x10)\n"
			"flw.ps f8, 768(x10)\n"
			"fsgnjx.ps f28, f5, f8\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f1, 608(x5)\n"
			"flw.ps f11, 192(x5)\n"
			"flw.ps f31, 384(x5)\n"
			"fnmsub.ps f17, f1, f11, f31, rtz\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f16, 960(x27)\n"
			"flw.ps f28, 320(x27)\n"
			"flt.ps f31, f16, f28\n" 
			"LBL_SEQID_5_M0_WRITE_ID_9_HID_0:\n"
			"maskor m7, m4, m2\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x27, rand_ieee754_32\n"
			"flw.ps f11, 320(x27)\n"
			"flw.ps f24, 352(x27)\n"
			"fmax.ps f17, f11, f24\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f27, 160(x27)\n"
			"flw.ps f3, 864(x27)\n"
			"fmul.ps f16, f27, f3, rup\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f16, 640(x23)\n"
			"fcvt.f16.ps f1, f16\n" 
			"la x10, rand_ieee754_16\n"
			"flw.ps f1, 800(x10)\n"
			"fcvt.ps.f16 f31, f1\n" 
			"la x15, rand_int_32\n"
			"flw.ps f27, 576(x15)\n"
			"flw.ps f16, 832(x15)\n"
			"fltu.pi f5, f27, f16\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f16, 704(x22)\n"
			"fcvt.lu.s x26, f16, rtz\n" 
			"fbci.ps f17, 0x4a17a\n" 
			"li x5, 0x5609\n"
			"csrw tensor_mask, x5\n"	
			"la x7, rand_ieee754_32\n"
			"flw.ps f31, 928(x7)\n"
			"flt.ps f30, f31, f31\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f16, 576(x10)\n"
			"flw.ps f5, 256(x10)\n"
			"fsgnj.s f23, f16, f5\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_5_XMA_QNT_STR_SEQ_ITER_0_HID_0:\n"
			"li x5, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x1 << TENSOR_FMA32_B_NUM_COLS) | (0x0 << TENSOR_FMA32_A_NUM_ROWS) | (0xb << TENSOR_FMA32_A_NUM_COLS) | (0x6 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1c << TENSOR_FMA32_SCP_LOC_B) | (0x1a << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"LBL_SEQID_5_XMA_QNT_STR_SEQ_ITER_1_HID_0:\n"
			"li x5, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x3 << TENSOR_FMA16_B_NUM_COLS) | (0xd << TENSOR_FMA16_A_NUM_ROWS) | (0x3 << TENSOR_FMA16_A_NUM_COLS) | (0x6 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x60 << TENSOR_FMA16_SCP_LOC_B) | (0xef << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x0 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_0\n"
			"csrw tensor_error, x0\n"
			"li x5, 6\n" 
			"bne x30, x5, LBL_FAIL_HID_0\n"
			"addi x30, x30, 1\n"
			"la x20, rand_ieee754_32\n"
			"flw.ps f1, 928(x20)\n"
			"flw.ps f26, 864(x20)\n"
			"flw.ps f3, 928(x20)\n"
			"fmadd.s f27, f1, f26, f3, rne\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f30, 480(x7)\n"
			"flw.ps f8, 928(x7)\n"
			"flw.ps f5, 224(x7)\n"
			"fnmsub.ps f8, f30, f8, f5, rtz\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f1, 960(x7)\n"
			"flw.ps f10, 864(x7)\n"
			"fsub.ps f15, f1, f10, rmm\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f23, 352(x10)\n"
			"fswizz.ps f30, f23, 0x7\n" 
			"LBL_SEQID_6_FP_TRANS_ID_10_HID_0:\n"
			"la x26, rand_ieee754_32\n"
			"flw.ps f15, 448(x26)\n"
			"flog.ps f10, f15\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f23, 128(x26)\n"
			"flw.ps f3, 896(x26)\n"
			"feq.ps f10, f23, f3\n" 
			"LBL_SEQID_6_M0_WRITE_ID_11_HID_0:\n"
			"mov.m.x m2, x27, 0x57\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0xd68f369254\n"
			"fsw.ps f0, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x15, rand_ieee754_32\n"
			"flw.ps f3, 64(x15)\n"
			"flw.ps f8, 384(x15)\n"
			"fmax.ps f30, f3, f8\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x22, rand_int_32\n"
			"flw.ps f3, 928(x22)\n"
			"flw.ps f26, 384(x22)\n"
			"for.pi f27, f3, f26\n" 
			"LBL_SEQID_6_FP_TRANS_ID_12_HID_0:\n"
			"la x10, rand_ieee754_32\n"
			"flw.ps f31, 0(x10)\n"
			"fexp.ps f1, f31\n" 
			"la x7, rand_int_32\n"
			"flw.ps f23, 768(x7)\n"
			"fsrli.pi f10, f23, 0x2\n" 
			"la x10, rand_int_32\n"
			"flw.ps f3, 736(x10)\n"
			"flw.ps f16, 800(x10)\n"
			"fsra.pi f26, f3, f16\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f27, 448(x10)\n"
			"fclass.ps f23, f27\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f5, 768(x7)\n"
			"flw.ps f10, 768(x7)\n"
			"flw.ps f1, 480(x7)\n"
			"fnmsub.ps f5, f5, f10, f1, rtz\n" 
			"fbci.ps f8, 0x3a274\n" 
			"la x5, rand_int_32\n"
			"flw.ps f17, 576(x5)\n"
			"fsatu8.pi f15, f17\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_6_TLD_XMA_STR_SEQ_ITER_0_HID_0:\n"
			"la x31, self_check_16\n"
			"li x5, 0x40000000000008\n"
			"add x5, x5, x31\n"
			"li x6, 0x3e000000000000e\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x0 << TENSOR_FMA16_B_NUM_COLS) | (0x8 << TENSOR_FMA16_A_NUM_ROWS) | (0xe << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1f << TENSOR_FMA16_SCP_LOC_B) | (0x2 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x15 << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0x8 << TENSOR_QUANT_QUANT_ROW) | (0x22 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x2 << TENSOR_QUANT_TRANSF7) | (0x7 << TENSOR_QUANT_TRANSF6) | (0x6 << TENSOR_QUANT_TRANSF5) | (0x6 << TENSOR_QUANT_TRANSF4) | (0x6 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0x1e\n"
			"bne x5, x31, LBL_FAIL_HID_0\n"
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_0\n"
			"csrw tensor_error, x0\n"
			"li x5, 7\n" 
			"bne x30, x5, LBL_FAIL_HID_0\n"
			"nop \n"
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=1); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"nop \n"
			"addi x29, x29, -1\n"
			"bne x29, x0, LBL_OUTER_LOOP_SEQ_HID_0\n"
			"nop\n"          
		       VSNIP_RSV     
		);
		load_vpu_regs();
		__asm__ __volatile__ ( 
			"nop\n"          
			"nop\n"          
		       VSNIP_RSV     
		);
		trans();
		__asm__ __volatile__ ( 
			"nop\n"          
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
		c_sync_point(sync_odd_harts=1); 
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
			"LBL_HPM_3_CORE_TS_INST_FAIL_HID_0:\n"          
			"csrr x5, mhpmcounter3\n"
			"sd x5, 32 (x31)\n"
			"LBL_HPM_4_CORE_DCACHE_ACCESS0_FAIL_HID_0:\n"          
			"csrr x5, mhpmcounter4\n"
			"sd x5, 40 (x31)\n"
			"LBL_HPM_5_CORE_BRANCHES0_FAIL_HID_0:\n"          
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
			"LBL_HPM_3_CORE_TS_INST_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter3\n"
			"sd x5, 32 (x31)\n"
			"LBL_HPM_4_CORE_DCACHE_ACCESS0_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter4\n"
			"sd x5, 40 (x31)\n"
			"LBL_HPM_5_CORE_BRANCHES0_PASS_HID_0:\n"          
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
		__asm__ __volatile__ ( 
			"LBL_HALT_HID_0:\n"          
			".global LBL_HALT_HID_0\n"          
			VSNIP_RSV
		);
	};
	if ( hid%16 == 1 ) { 
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
			"csrwi mhpmevent3, 11\n"   
			"csrwi mhpmevent4, 1\n"   
			"csrwi mhpmevent5, 6\n"   
			"csrwi mhpmevent6, 12\n"   
			"csrwi mhpmevent7, 23\n"   
			"csrwi mhpmevent8, 21\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f4,  288 (x5)\n"
			"flw.ps f7,  896 (x5)\n"
			"flw.ps f11,  480 (x5)\n"
			"flw.ps f3,  64 (x5)\n"
			"flw.ps f29,  800 (x5)\n"
			"flw.ps f1,  768 (x5)\n"
			"flw.ps f21,  960 (x5)\n"
			"flw.ps f12,  32 (x5)\n"
			"flw.ps f5,  736 (x5)\n"
			"flw.ps f10,  576 (x5)\n"
			"flw.ps f17,  704 (x5)\n"
			"flw.ps f8,  384 (x5)\n"
			"flw.ps f14,  992 (x5)\n"
			"flw.ps f26,  544 (x5)\n"
			"flw.ps f15,  96 (x5)\n"
			"flw.ps f24,  224 (x5)\n"
			"flw.ps f28,  512 (x5)\n"
			"flw.ps f0,  128 (x5)\n"
			"flw.ps f19,  192 (x5)\n"
			"flw.ps f2,  672 (x5)\n"
			"flw.ps f9,  352 (x5)\n"
			"flw.ps f27,  640 (x5)\n"
			"flw.ps f13,  832 (x5)\n"
			"flw.ps f20,  448 (x5)\n"
			"flw.ps f22,  160 (x5)\n"
			"flw.ps f23,  928 (x5)\n"
			"flw.ps f18,  608 (x5)\n"
			"flw.ps f31,  320 (x5)\n"
			"flw.ps f6,  864 (x5)\n"
			"flw.ps f30,  256 (x5)\n"
			"flw.ps f25,  0 (x5)\n"
			"flw.ps f16,  416 (x5)\n"
			"csrw fflags, zero\n"
			"csrwi frm, 0x0\n"
			"li x31, 100\n"
			"1:\n" 
			"addi x31, x31, -1\n"
			"bne x31, x0, 1b\n" 
			"csrw fcc, x0\n" 
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=1); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"LBL_KERNEL_HID_1:\n"          
			"li x5, 0xFEEDFEED\n"
			"slti x0, x0, 0x7f2\n"
			"la x31, check_sum_hid_1\n"
			"li x5, 0x7\n" 
			"sw x5, (x31)\n"
			"li x29, 0x7\n"
			"LBL_OUTER_LOOP_SEQ_HID_1:\n"
			"nop \n"
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=1); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"nop \n"
			"li x30, 0\n"
			"addi x30, x30, 1\n"
			"la x23, rand_int_32\n"
			"flw.ps f2, 640(x23)\n"
			"flt.pi f14, f2, f2\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f14, 160(x22)\n"
			"fclass.s x10, f14\n" 
			"mova.x.m x6\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f21, 256(x15)\n"
			"fcvt.f16.ps f31, f21\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f20, 864(x15)\n"
			"flw.ps f27, 960(x15)\n"
			"fmul.ps f14, f20, f27, rdn\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_1:\n"
			"mov.m.x m5, x15, 0x6c\n" 
			"li x5, CREDINC0\n"
			"ld x5, 0(x5)\n"
			"la x22, rand_ieee754_32\n"
			"flw.ps f28, 160(x22)\n"
			"flw.ps f8, 0(x22)\n"
			"flt.ps f26, f28, f8\n" 
			"la x27, rand_int_32\n"
			"flw.ps f31, 640(x27)\n"
			"fpackreph.pi f21, f31\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f15, 256(x22)\n"
			"fcvt.pw.ps f14, f15, rup\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f5, 512(x9)\n"
			"flw.ps f4, 992(x9)\n"
			"flt.s x6, f5, f4\n" 
			"la x22, rand_int_32\n"
			"flw.ps f31, 544(x22)\n"
			"flw.ps f14, 672(x22)\n"
			"fsra.pi f31, f31, f14\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f31, 800(x23)\n"
			"flw.ps f28, 320(x23)\n"
			"flw.ps f15, 384(x23)\n"
			"fmsub.ps f1, f31, f28, f15, rdn\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f20, 608(x9)\n"
			"flw.ps f28, 672(x9)\n"
			"flw.ps f15, 544(x9)\n"
			"fmadd.ps f26, f20, f28, f15, rtz\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f14, 32(x9)\n"
			"fcvt.pw.ps f27, f14, rdn\n" 
			"la x7, rand_int_32\n"
			"flw.ps f15, 512(x7)\n"
			"fpackreph.pi f31, f15\n" 
			"la x22, rand_int_32\n"
			"flw.ps f15, 544(x22)\n"
			"flw.ps f20, 960(x22)\n"
			"fand.pi f30, f15, f20\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_1\n"
			"addi x30, x30, 1\n"
			"maskpopcz x18, m6\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f14, 928(x10)\n"
			"fround.ps f14, f14, rdn\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f2, 320(x23)\n"
			"flw.ps f20, 384(x23)\n"
			"fmin.ps f21, f2, f20\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f31, 64(x22)\n"
			"flw.ps f20, 960(x22)\n"
			"fmin.s f4, f31, f20\n" 
			"fbci.ps f31, 0x195f7\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f27, 256(x11)\n"
			"fsub.ps f30, f27, f27, rmm\n" 
			"la x22, rand_int_32\n"
			"flw.ps f15, 896(x22)\n"
			"fpackrepb.pi f4, f15\n" 
			"la x10, rand_int_32\n"
			"flw.ps f16, 768(x10)\n"
			"flw.ps f21, 768(x10)\n"
			"fadd.pi f31, f16, f21\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f27, 576(x6)\n"
			"fclass.ps f8, f27\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f31, 544(x7)\n"
			"fcvt.w.s x18, f31, rtz\n" 
			"la x22, rand_int_32\n"
			"flw.ps f20, 704(x22)\n"
			"fnot.pi f5, f20\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f5, 672(x27)\n"
			"flw.ps f27, 224(x27)\n"
			"fmin.ps f16, f5, f27\n" 
			"la x23, rand_int_32\n"
			"flw.ps f16, 768(x23)\n"
			"flw.ps f31, 928(x23)\n"
			"fsll.pi f2, f16, f31\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f5, 0(x15)\n"
			"flw.ps f14, 992(x15)\n"
			"fsgnjn.s f8, f5, f14\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f16, 224(x15)\n"
			"flw.ps f28, 576(x15)\n"
			"feq.s x7, f16, f28\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f30, 192(x11)\n"
			"flw.ps f2, 352(x11)\n"
			"fmin.s f31, f30, f2\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_1\n"
			"addi x30, x30, 1\n"
			"la x11, rand_int_32\n"
			"flw.ps f31, 512(x11)\n"
			"flw.ps f20, 192(x11)\n"
			"fminu.pi f20, f31, f20\n" 
			"la x10, rand_int_32\n"
			"flw.ps f16, 960(x10)\n"
			"flw.ps f8, 992(x10)\n"
			"fsll.pi f4, f16, f8\n" 
			"la x18, rand_int_32\n"
			"flw.ps f21, 960(x18)\n"
			"fpackreph.pi f28, f21\n" 
			"la x15, rand_int_32\n"
			"flw.ps f5, 128(x15)\n"
			"fsat8.pi f21, f5\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f20, 256(x23)\n"
			"flw.ps f27, 128(x23)\n"
			"flt.ps f26, f20, f27\n" 
			"la x27, rand_int_32\n"
			"flw.ps f5, 128(x27)\n"
			"flw.ps f30, 192(x27)\n"
			"fltu.pi f27, f5, f30\n" 
			"la x27, rand_int_32\n"
			"flw.ps f21, 960(x27)\n"
			"flw.ps f26, 576(x27)\n"
			"fltu.pi f12, f21, f26\n" 
			"LBL_SEQID_2_M0_WRITE_ID_1_HID_1:\n"
			"mov.m.x m3, x7, 0x7f\n" 
			"li x5, 0x803401D0\n"
			"ld x5, 0(x5)\n"
			"la x11, rand_ieee754_32\n"
			"flw.ps f8, 608(x11)\n"
			"flw.ps f4, 448(x11)\n"
			"fadd.ps f4, f8, f4, rne\n" 
			"la x22, rand_int_32\n"
			"flw.ps f30, 960(x22)\n"
			"flw.ps f20, 448(x22)\n"
			"fmulh.pi f16, f30, f20\n" 
			"la x15, rand_int_32\n"
			"flw.ps f14, 352(x15)\n"
			"fpackrepb.pi f8, f14\n" 
			"la x11, rand_int_32\n"
			"flw.ps f27, 448(x11)\n"
			"flw.ps f20, 160(x11)\n"
			"fsub.pi f21, f27, f20\n" 
			"la x18, rand_int_32\n"
			"flw.ps f27, 608(x18)\n"
			"flw.ps f21, 736(x18)\n"
			"fmaxu.pi f28, f27, f21\n" 
			"la x7, rand_int_32\n"
			"flw.ps f30, 896(x7)\n"
			"fslli.pi f5, f30, 0x7\n" 
			"la x22, rand_int_32\n"
			"flw.ps f28, 288(x22)\n"
			"fcvt.ps.pw f12, f28, rup\n" 
			"la x23, rand_int_32\n"
			"flw.ps f2, 992(x23)\n"
			"fcvt.ps.pwu f31, f2, rne\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 3\n" 
			"bne x30, x5, LBL_FAIL_HID_1\n"
			"nop\n"          
		       VSNIP_RSV     
		);
		load_vpu_regs();
		__asm__ __volatile__ ( 
			"nop\n"          
			"addi x30, x30, 1\n"
			"la x6, rand_int_32\n"
			"flw.ps f21, 160(x6)\n"
			"fcvt.ps.pwu f12, f21, rne\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f20, 992(x9)\n"
			"flw.ps f28, 416(x9)\n"
			"flw.ps f15, 512(x9)\n"
			"fmadd.ps f2, f20, f28, f15, rdn\n" 
			"LBL_SEQID_3_M0_WRITE_ID_2_HID_1:\n"
			"fltm.ps m1, f27, f4\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x9, rand_int_32\n"
			"flw.ps f26, 64(x9)\n"
			"fltu.pi f16, f26, f26\n" 
			"la x22, rand_int_32\n"
			"flw.ps f15, 160(x22)\n"
			"fsatu8.pi f2, f15\n" 
			"la x22, rand_int_32\n"
			"flw.ps f26, 96(x22)\n"
			"flw.ps f14, 896(x22)\n"
			"feq.pi f5, f26, f14\n" 
			"la x15, rand_int_32\n"
			"flw.ps f27, 448(x15)\n"
			"flw.ps f16, 192(x15)\n"
			"for.pi f21, f27, f16\n" 
			"maskpopc x6, m1\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f5, 544(x10)\n"
			"flw.ps f28, 320(x10)\n"
			"fle.s x7, f5, f28\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f20, 32(x7)\n"
			"ffrc.ps f15, f20\n" 
			"la x11, rand_int_32\n"
			"flw.ps f21, 512(x11)\n"
			"fandi.pi f15, f21, 0x1bb\n" 
			"la x9, rand_int_32\n"
			"flw.ps f15, 576(x9)\n"
			"fpackrepb.pi f20, f15\n" 
			"LBL_SEQID_3_M0_WRITE_ID_3_HID_1:\n"
			"maskxor m6, m3, m1\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x6, rand_ieee754_32\n"
			"flw.ps f30, 32(x6)\n"
			"flw.ps f12, 576(x6)\n"
			"fle.s x22, f30, f12\n" 
			"la x9, rand_int_32\n"
			"flw.ps f16, 96(x9)\n"
			"flw.ps f2, 160(x9)\n"
			"fdiv.pi f5, f16, f2\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f12, 576(x10)\n"
			"flw.ps f2, 832(x10)\n"
			"fsgnjx.ps f8, f12, f2\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 4\n" 
			"bne x30, x5, LBL_FAIL_HID_1\n"
			"nop\n"          
		       VSNIP_RSV     
		);
		trans();
		__asm__ __volatile__ ( 
			"nop\n"          
			"addi x30, x30, 1\n"
			"LBL_SEQID_4_M0_WRITE_ID_4_HID_1:\n"
			"mov.m.x m1, x27, 0x65\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x7, rand_ieee754_32\n"
			"flw.ps f26, 704(x7)\n"
			"flw.ps f4, 480(x7)\n"
			"fsgnjx.ps f30, f26, f4\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f14, 64(x15)\n"
			"flw.ps f31, 768(x15)\n"
			"flw.ps f2, 608(x15)\n"
			"fnmadd.ps f8, f14, f31, f2, rne\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f2, 896(x6)\n"
			"fcvt.wu.s x18, f2, rup\n" 
			"maskpopc x27, m5\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f8, 352(x18)\n"
			"flw.ps f21, 448(x18)\n"
			"flw.ps f16, 0(x18)\n"
			"fnmsub.ps f2, f8, f21, f16, rmm\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f21, 832(x9)\n"
			"fround.ps f2, f21, rup\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f12, 384(x22)\n"
			"flw.ps f8, 928(x22)\n"
			"fsgnjx.s f20, f12, f8\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f15, 576(x9)\n"
			"ffrc.ps f21, f15\n" 
			"la x9, rand_int_32\n"
			"flw.ps f28, 416(x9)\n"
			"flw.ps f1, 288(x9)\n"
			"fle.pi f27, f28, f1\n" 
			"LBL_SEQID_4_M0_WRITE_ID_5_HID_1:\n"
			"flem.ps m3, f21, f20\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0x38273f4d9c\n"
			"flw.ps f5, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x15, rand_ieee754_32\n"
			"flw.ps f30, 672(x15)\n"
			"flw.ps f14, 608(x15)\n"
			"feq.s x9, f30, f14\n" 
			"la x9, rand_int_32\n"
			"flw.ps f20, 736(x9)\n"
			"flw.ps f30, 32(x9)\n"
			"fmax.pi f30, f20, f30\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f12, 192(x27)\n"
			"flw.ps f20, 256(x27)\n"
			"fadd.s f26, f12, f20, rtz\n" 
			"la x23, rand_int_32\n"
			"flw.ps f30, 64(x23)\n"
			"flw.ps f14, 800(x23)\n"
			"fltu.pi f30, f30, f14\n" 
			"la x7, rand_int_32\n"
			"flw.ps f30, 32(x7)\n"
			"flw.ps f15, 768(x7)\n"
			"fmin.pi f16, f30, f15\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 5\n" 
			"bne x30, x5, LBL_FAIL_HID_1\n"
			"nop\n"          
		       VSNIP_RSV     
		);
		load_vpu_regs();
		__asm__ __volatile__ ( 
			"nop\n"          
			"addi x30, x30, 1\n"
			"fbci.ps f15, 0x3dd6a\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f26, 992(x10)\n"
			"flw.ps f28, 288(x10)\n"
			"flw.ps f12, 768(x10)\n"
			"fmsub.s f27, f26, f28, f12, rne\n" 
			"la x27, rand_int_32\n"
			"flw.ps f12, 512(x27)\n"
			"flw.ps f20, 864(x27)\n"
			"fxor.pi f27, f12, f20\n" 
			"LBL_SEQID_5_FP_TRANS_ID_6_HID_1:\n"
			"la x11, rand_ieee754_32\n"
			"flw.ps f26, 768(x11)\n"
			"flog.ps f1, f26\n" 
			"fbci.ps f2, 0x45f65\n" 
			"la x11, rand_int_32\n"
			"flw.ps f21, 608(x11)\n"
			"flw.ps f26, 0(x11)\n"
			"fmulh.pi f26, f21, f26\n" 
			"la x6, rand_int_32\n"
			"flw.ps f30, 672(x6)\n"
			"fpackreph.pi f14, f30\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f27, 128(x27)\n"
			"flw.ps f20, 32(x27)\n"
			"flt.ps f28, f27, f20\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f8, 704(x10)\n"
			"fround.ps f1, f8, rtz\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f5, 608(x7)\n"
			"flw.ps f14, 224(x7)\n"
			"feq.ps f16, f5, f14\n" 
			"la x18, rand_int_32\n"
			"flw.ps f20, 768(x18)\n"
			"flw.ps f16, 928(x18)\n"
			"fremu.pi f1, f20, f16\n" 
			"LBL_SEQID_5_M0_WRITE_ID_7_HID_1:\n"
			"maskand m5, m3, m6\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x27, rand_int_32\n"
			"flw.ps f2, 256(x27)\n"
			"flw.ps f26, 0(x27)\n"
			"fmax.pi f8, f2, f26\n" 
			"la x10, rand_int_32\n"
			"flw.ps f15, 0(x10)\n"
			"fcvt.ps.pwu f12, f15, rne\n" 
			"LBL_SEQID_5_M0_WRITE_ID_8_HID_1:\n"
			"fsetm.pi m3, f26\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x15, rand_ieee754_32\n"
			"flw.ps f5, 800(x15)\n"
			"flw.ps f26, 832(x15)\n"
			"flt.ps f26, f5, f26\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 6\n" 
			"bne x30, x5, LBL_FAIL_HID_1\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_6_M0_WRITE_ID_9_HID_1:\n"
			"flem.ps m1, f5, f21\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x15, rand_ieee754_32\n"
			"flw.ps f1, 736(x15)\n"
			"flw.ps f27, 128(x15)\n"
			"fsgnjn.ps f28, f1, f27\n" 
			"li x18, 0x80d57d26eef40b68\n"
			"fcvt.s.lu f2, x18, rmm\n" 
			"la x11, rand_int_32\n"
			"flw.ps f28, 96(x11)\n"
			"flw.ps f31, 512(x11)\n"
			"fmul.pi f26, f28, f31\n" 
			"LBL_SEQID_6_M0_WRITE_ID_10_HID_1:\n"
			"fltm.pi m6, f5, f30\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0x56729baad4\n"
			"fsw.ps f0, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x27, rand_ieee754_32\n"
			"flw.ps f27, 928(x27)\n"
			"fcvt.f16.ps f15, f27\n" 
			"la x6, rand_int_32\n"
			"flw.ps f4, 896(x6)\n"
			"fnot.pi f26, f4\n" 
			"LBL_SEQID_6_M0_WRITE_ID_11_HID_1:\n"
			"fltm.ps m1, f16, f15\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0x52de80cf33\n"
			"flw.ps f5, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"maskpopcz x7, m6\n" 
			"la x22, rand_int_32\n"
			"flw.ps f27, 512(x22)\n"
			"fsatu8.pi f12, f27\n" 
			"la x27, rand_int_32\n"
			"flw.ps f26, 96(x27)\n"
			"fmulh.pi f30, f26, f26\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f1, 480(x22)\n"
			"fsgnjx.s f8, f1, f1\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f27, 608(x6)\n"
			"flw.ps f5, 704(x6)\n"
			"flw.ps f8, 320(x6)\n"
			"fnmsub.ps f16, f27, f5, f8, rne\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f28, 480(x7)\n"
			"flw.ps f20, 768(x7)\n"
			"fmin.ps f4, f28, f20\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f31, 256(x10)\n"
			"fround.ps f5, f31, rtz\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f15, 512(x6)\n"
			"flw.ps f27, 800(x6)\n"
			"flw.ps f31, 0(x6)\n"
			"fnmadd.ps f28, f15, f27, f31, rne\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 7\n" 
			"bne x30, x5, LBL_FAIL_HID_1\n"
			"nop \n"
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=1); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"nop \n"
			"addi x29, x29, -1\n"
			"bne x29, x0, LBL_OUTER_LOOP_SEQ_HID_1\n"
			"nop\n"          
		       VSNIP_RSV     
		);
		load_vpu_regs();
		__asm__ __volatile__ ( 
			"nop\n"          
			"nop\n"          
		       VSNIP_RSV     
		);
		trans();
		__asm__ __volatile__ ( 
			"nop\n"          
			"la x31, check_sum_hid_1\n"
			"lw x5, (x31)\n"
			"bne x30, x5, LBL_FAIL_HID_1\n"
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
		c_sync_point(sync_odd_harts=1); 
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
			"LBL_HPM_3_CORE_L2_MISS_REQ_REJ_FAIL_HID_1:\n"          
			"csrr x5, mhpmcounter3\n"
			"sd x5, 32 (x31)\n"
			"LBL_HPM_4_CORE_MCYCLES_FAIL_HID_1:\n"          
			"csrr x5, mhpmcounter4\n"
			"sd x5, 40 (x31)\n"
			"LBL_HPM_5_CORE_DCACHE_ACCESS0_FAIL_HID_1:\n"          
			"csrr x5, mhpmcounter5\n"
			"sd x5, 48 (x31)\n"
			"LBL_HPM_6_CORE_L2_EVICT_REQ_FAIL_HID_1:\n"          
			"csrr x5, mhpmcounter6\n"
			"sd x5, 56 (x31)\n"
			"LBL_HPM_7_NEIGH_ETL_RSP_FROM_SC_UC_FAIL_HID_1:\n"          
			"csrr x5, mhpmcounter7\n"
			"sd x5, 64 (x31)\n"
			"LBL_HPM_8_NEIGH_ETL_REQ_TO_INTERMEDIATE_FIFO_FAIL_HID_1:\n"          
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
			"LBL_HPM_3_CORE_L2_MISS_REQ_REJ_PASS_HID_1:\n"          
			"csrr x5, mhpmcounter3\n"
			"sd x5, 32 (x31)\n"
			"LBL_HPM_4_CORE_MCYCLES_PASS_HID_1:\n"          
			"csrr x5, mhpmcounter4\n"
			"sd x5, 40 (x31)\n"
			"LBL_HPM_5_CORE_DCACHE_ACCESS0_PASS_HID_1:\n"          
			"csrr x5, mhpmcounter5\n"
			"sd x5, 48 (x31)\n"
			"LBL_HPM_6_CORE_L2_EVICT_REQ_PASS_HID_1:\n"          
			"csrr x5, mhpmcounter6\n"
			"sd x5, 56 (x31)\n"
			"LBL_HPM_7_NEIGH_ETL_RSP_FROM_SC_UC_PASS_HID_1:\n"          
			"csrr x5, mhpmcounter7\n"
			"sd x5, 64 (x31)\n"
			"LBL_HPM_8_NEIGH_ETL_REQ_TO_INTERMEDIATE_FIFO_PASS_HID_1:\n"          
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
	if ( hid%16 == 2 ) { 
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
			"csrwi mhpmevent3, 16\n"   
			"csrwi mhpmevent4, 6\n"   
			"csrwi mhpmevent5, 4\n"   
			"csrwi mhpmevent6, 27\n"   
			"csrwi mhpmevent7, 6\n"   
			"csrwi mhpmevent8, 3\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f1,  736 (x5)\n"
			"flw.ps f21,  480 (x5)\n"
			"flw.ps f31,  256 (x5)\n"
			"flw.ps f28,  544 (x5)\n"
			"flw.ps f3,  832 (x5)\n"
			"flw.ps f5,  32 (x5)\n"
			"flw.ps f22,  224 (x5)\n"
			"flw.ps f15,  928 (x5)\n"
			"flw.ps f16,  352 (x5)\n"
			"flw.ps f20,  960 (x5)\n"
			"flw.ps f29,  704 (x5)\n"
			"flw.ps f6,  608 (x5)\n"
			"flw.ps f19,  288 (x5)\n"
			"flw.ps f14,  160 (x5)\n"
			"flw.ps f7,  640 (x5)\n"
			"flw.ps f8,  896 (x5)\n"
			"flw.ps f9,  576 (x5)\n"
			"flw.ps f23,  864 (x5)\n"
			"flw.ps f18,  320 (x5)\n"
			"flw.ps f4,  768 (x5)\n"
			"flw.ps f11,  192 (x5)\n"
			"flw.ps f13,  64 (x5)\n"
			"flw.ps f30,  0 (x5)\n"
			"flw.ps f17,  512 (x5)\n"
			"flw.ps f10,  96 (x5)\n"
			"flw.ps f0,  384 (x5)\n"
			"flw.ps f12,  128 (x5)\n"
			"flw.ps f2,  448 (x5)\n"
			"flw.ps f25,  800 (x5)\n"
			"flw.ps f27,  416 (x5)\n"
			"flw.ps f26,  672 (x5)\n"
			"flw.ps f24,  992 (x5)\n"
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
			"csrw fcc, x0\n" 
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=1); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"LBL_KERNEL_HID_2:\n"          
			"li x5, 0xFEEDFEED\n"
			"slti x0, x0, 0x7f2\n"
			"la x31, check_sum_hid_2\n"
			"li x5, 0x7\n" 
			"sw x5, (x31)\n"
			"li x29, 0x7\n"
			"LBL_OUTER_LOOP_SEQ_HID_2:\n"
			"nop \n"
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=1); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"nop \n"
			"li x30, 0\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_2:\n"
			"fltm.ps m5, f6, f13\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x5, rand_ieee754_32\n"
			"flw.ps f6, 288(x5)\n"
			"flw.ps f11, 736(x5)\n"
			"fsgnjn.s f3, f6, f11\n" 
			"la x13, rand_int_32\n"
			"flw.ps f14, 288(x13)\n"
			"flw.ps f23, 480(x13)\n"
			"fmulhu.pi f14, f14, f23\n" 
			"maskpopc x12, m0\n" 
			"mov.m.x m0, x0, 0xff\n"
			"la x17, rand_int_32\n"
			"flw.ps f23, 608(x17)\n"
			"fsat8.pi f19, f23\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f19, 544(x17)\n"
			"fclass.ps f13, f19\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f26, 448(x20)\n"
			"flw.ps f9, 992(x20)\n"
			"fsgnjn.ps f14, f26, f9\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f4, 992(x19)\n"
			"flw.ps f16, 224(x19)\n"
			"fmul.ps f28, f4, f16, rtz\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f3, 512(x19)\n"
			"flw.ps f13, 96(x19)\n"
			"fsub.ps f9, f3, f13, rup\n" 
			"la x11, rand_int_32\n"
			"flw.ps f9, 704(x11)\n"
			"flw.ps f16, 800(x11)\n"
			"fmulhu.pi f13, f9, f16\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_2:\n"
			"fltm.pi m6, f26, f25\n" 
			"li x5, 0x80340100\n"
			"ld x5, 0(x5)\n"
			"la x22, rand_ieee754_32\n"
			"flw.ps f31, 608(x22)\n"
			"flw.ps f23, 320(x22)\n"
			"flw.ps f9, 256(x22)\n"
			"fcmov.ps f26, f31, f23, f9\n" 
			"LBL_SEQID_0_M0_WRITE_ID_2_HID_2:\n"
			"fltm.pi m5, f14, f3\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0xd29c281d08\n"
			"fsw.ps f0, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x22, rand_int_32\n"
			"flw.ps f9, 832(x22)\n"
			"flw.ps f13, 896(x22)\n"
			"for.pi f28, f9, f13\n" 
			"li x5, 0x4e8e\n"
			"csrw tensor_mask, x5\n"	
			"LBL_SEQID_0_FP_TRANS_ID_3_HID_2:\n"
			"la x22, rand_ieee754_32\n"
			"flw.ps f14, 928(x22)\n"
			"frcp.ps f3, f14\n" 
			"li x22, 0x1b52b59e23088dd4\n"
			"fcvt.s.w f23, x22, rdn\n" 
			"li x5, 0xc3f7\n"
			"csrw tensor_mask, x5\n"	
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ID_378718_HID_2:\n"
			"li x5, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x0 << TENSOR_FMA16_B_NUM_COLS) | (0x6 << TENSOR_FMA16_A_NUM_ROWS) | (0xe << TENSOR_FMA16_A_NUM_COLS) | (0x1 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x3a << TENSOR_FMA16_SCP_LOC_B) | (0x7e << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x0 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ID_723164_HID_2:\n"
			"li x5, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x1 << TENSOR_FMA16_B_NUM_COLS) | (0xa << TENSOR_FMA16_A_NUM_ROWS) | (0xf << TENSOR_FMA16_A_NUM_COLS) | (0xf << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0xcf << TENSOR_FMA16_SCP_LOC_B) | (0x57 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_2\n"
			"csrw tensor_error, x0\n"
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=0); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ITER_0_HID_2:\n"
			"li x6, ((0x10 << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0xf << TENSOR_QUANT_QUANT_ROW) | (0x3e << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x6 << TENSOR_QUANT_TRANSF6) | (0x6 << TENSOR_QUANT_TRANSF5) | (0x7 << TENSOR_QUANT_TRANSF4) | (0x7 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_2\n"
			"csrw tensor_error, x0\n"
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_2\n"
			"addi x30, x30, 1\n"
			"li x20, 0x99fea5caf91a125a\n"
			"fcvt.s.w f3, x20, rne\n" 
			"la x20, rand_int_32\n"
			"flw.ps f28, 672(x20)\n"
			"flw.ps f4, 288(x20)\n"
			"fminu.pi f13, f28, f4\n" 
			"LBL_SEQID_1_FP_TRANS_ID_4_HID_2:\n"
			"la x11, rand_ieee754_32\n"
			"flw.ps f28, 96(x11)\n"
			"frcp.ps f9, f28\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f6, 320(x12)\n"
			"flw.ps f23, 0(x12)\n"
			"flw.ps f10, 64(x12)\n"
			"fmsub.s f0, f6, f23, f10, rtz\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x22, rand_ieee754_32\n"
			"flw.ps f31, 32(x22)\n"
			"flw.ps f6, 352(x22)\n"
			"fle.s x25, f31, f6\n" 
			"li x5, 0x1d2778f6d09eac24\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0x86071bd97acac8f0\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"la x12, rand_int_32\n"
			"flw.ps f4, 864(x12)\n"
			"fcvt.ps.pwu f14, f4, rmm\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f10, 512(x5)\n"
			"flw.ps f16, 608(x5)\n"
			"fle.s x12, f10, f16\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f3, 288(x20)\n"
			"flw.ps f9, 928(x20)\n"
			"fmin.ps f14, f3, f9\n" 
			"li x5, 0xadb1\n"
			"csrw tensor_mask, x5\n"	
			"la x22, rand_int_32\n"
			"flw.ps f14, 192(x22)\n"
			"fpackreph.pi f23, f14\n" 
			"LBL_SEQID_1_M0_WRITE_ID_5_HID_2:\n"
			"mova.m.x x12\n" 
			"li x5, CREDINC2\n"
			"ld x5, 0(x5)\n"
			"la x5, rand_ieee754_32\n"
			"flw.ps f19, 448(x5)\n"
			"ffrc.ps f23, f19\n" 
			"la x19, rand_int_32\n"
			"flw.ps f19, 544(x19)\n"
			"fslli.pi f4, f19, 0x9\n" 
			"la x12, rand_int_32\n"
			"flw.ps f19, 192(x12)\n"
			"fcvt.ps.pw f0, f19, rtz\n" 
			"maskpopcz x13, m3\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f25, 480(x5)\n"
			"flw.ps f11, 320(x5)\n"
			"fadd.ps f0, f25, f11, rdn\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f26, 416(x22)\n"
			"flw.ps f16, 768(x22)\n"
			"fcmovm.ps f25, f26, f16\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 0x1810\n"
			"csrw tensor_mask, x5\n"	
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ID_703456_HID_2:\n"
			"li x6, ((0x19 << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0x0 << TENSOR_QUANT_QUANT_ROW) | (0x7 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x2 << TENSOR_QUANT_TRANSF8) | (0x6 << TENSOR_QUANT_TRANSF7) | (0x7 << TENSOR_QUANT_TRANSF6) | (0x7 << TENSOR_QUANT_TRANSF5) | (0x7 << TENSOR_QUANT_TRANSF4) | (0x7 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_2\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_1_RDX_ID_1_SHIRE_0_HID_2:\n"
			"li x25, 0xa0000000010000a\n"
			"csrw tensor_reduce, x25\n" 
			"csrwi tensor_wait, 9\n" 
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=0); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"LBL_SEQID_1_TLD_XMA_STR_SEQ_ITER_0_HID_2:\n"
			"la x31, self_check_8\n"
			"li x5, 0xa0000000000001\n"
			"add x5, x5, x31\n"
			"li x6, 0x320000000000009\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x1 << TENSOR_IMA_B_NUM_COLS) | (0x1 << TENSOR_IMA_A_NUM_ROWS) | (0x9 << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x19 << TENSOR_IMA_SCP_LOC_B) | (0x5 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
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
			"bne x5, x31, LBL_FAIL_HID_2\n"
			"LBL_SEQID_1_TLD_XMA_STR_SEQ_ITER_1_HID_2:\n"
			"la x31, self_check_16\n"
			"li x5, 0xc0000000000007\n"
			"add x5, x5, x31\n"
			"li x6, 0x10000000000003\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x0 << TENSOR_FMA16_B_NUM_COLS) | (0x7 << TENSOR_FMA16_A_NUM_ROWS) | (0x3 << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x1 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x19 << TENSOR_FMA16_SCP_LOC_B) | (0x6 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0x8\n"
			"bne x5, x31, LBL_FAIL_HID_2\n"
			"LBL_SEQID_1_TLD_XMA_STR_SEQ_ITER_2_HID_2:\n"
			"la x31, self_check_16\n"
			"li x5, 0x4000000000000c\n"
			"add x5, x5, x31\n"
			"li x6, 0x3e0000000000007\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x3 << TENSOR_FMA16_B_NUM_COLS) | (0xc << TENSOR_FMA16_A_NUM_ROWS) | (0x7 << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1f << TENSOR_FMA16_SCP_LOC_B) | (0x2 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0x10\n"
			"bne x5, x31, LBL_FAIL_HID_2\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_2\n"
			"csrw tensor_error, x0\n"
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_2\n"
			"addi x30, x30, 1\n"
			"la x22, rand_ieee754_32\n"
			"flw.ps f4, 352(x22)\n"
			"flw.ps f0, 224(x22)\n"
			"fmin.s f0, f4, f0\n" 
			"maskpopc x25, m6\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f11, 704(x17)\n"
			"flw.ps f4, 928(x17)\n"
			"fsgnj.ps f25, f11, f4\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f13, 864(x15)\n"
			"flw.ps f3, 672(x15)\n"
			"flw.ps f26, 960(x15)\n"
			"fnmsub.ps f31, f13, f3, f26, rmm\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f3, 352(x15)\n"
			"flw.ps f19, 384(x15)\n"
			"flw.ps f9, 992(x15)\n"
			"fmsub.ps f0, f3, f19, f9, rmm\n" 
			"la x12, rand_int_32\n"
			"flw.ps f28, 704(x12)\n"
			"flw.ps f25, 992(x12)\n"
			"fmin.pi f19, f28, f25\n" 
			"la x25, rand_int_32\n"
			"flw.ps f28, 576(x25)\n"
			"flw.ps f19, 160(x25)\n"
			"fsra.pi f11, f28, f19\n" 
			"LBL_SEQID_2_FP_TRANS_ID_6_HID_2:\n"
			"la x11, rand_ieee754_32\n"
			"flw.ps f6, 704(x11)\n"
			"flog.ps f19, f6\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f9, 864(x12)\n"
			"flw.ps f19, 128(x12)\n"
			"feq.s x15, f9, f19\n" 
			"LBL_SEQID_2_M0_WRITE_ID_7_HID_2:\n"
			"maskxor m6, m0, m5\n" 
			"li x5, CREDINC1\n"
			"ld x5, 0(x5)\n"
			"la x22, rand_int_32\n"
			"flw.ps f16, 544(x22)\n"
			"fcvt.ps.pw f3, f16, rmm\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f23, 64(x11)\n"
			"flw.ps f28, 928(x11)\n"
			"flt.ps f6, f23, f28\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f23, 960(x19)\n"
			"flw.ps f0, 864(x19)\n"
			"fmin.ps f3, f23, f0\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f4, 704(x22)\n"
			"fsin.ps f13, f4\n" 
			"LBL_SEQID_2_M0_WRITE_ID_8_HID_2:\n"
			"masknot m5, m6\n" 
			"li x5, 0x803401F0\n"
			"ld x5, 0(x5)\n"
			"li x5, 0x9b42\n"
			"csrw tensor_mask, x5\n"	
			"la x22, rand_int_32\n"
			"flw.ps f31, 352(x22)\n"
			"flw.ps f14, 384(x22)\n"
			"fmulh.pi f14, f31, f14\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_2_XMA_QNT_STR_SEQ_ITER_0_HID_2:\n"
			"li x6, ((0xd << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0xf << TENSOR_QUANT_QUANT_ROW) | (0x3e << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x1 << TENSOR_QUANT_TRANSF6) | (0xa << TENSOR_QUANT_TRANSF5) | (0xa << TENSOR_QUANT_TRANSF4) | (0xa << TENSOR_QUANT_TRANSF3) | (0x9 << TENSOR_QUANT_TRANSF2) | (0x3 << TENSOR_QUANT_TRANSF1) | (0x4 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_2\n"
			"csrw tensor_error, x0\n"
			"li x5, 3\n" 
			"bne x30, x5, LBL_FAIL_HID_2\n"
			"nop\n"          
		       VSNIP_RSV     
		);
		load_vpu_regs();
		__asm__ __volatile__ ( 
			"nop\n"          
			"addi x30, x30, 1\n"
			"la x25, rand_int_32\n"
			"flw.ps f10, 352(x25)\n"
			"fsrli.pi f0, f10, 0x1\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f19, 448(x17)\n"
			"flw.ps f25, 64(x17)\n"
			"fsgnjx.s f9, f19, f25\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f28, 256(x25)\n"
			"flw.ps f3, 800(x25)\n"
			"fsgnjx.ps f4, f28, f3\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f4, 512(x12)\n"
			"flw.ps f3, 736(x12)\n"
			"fsgnjx.ps f13, f4, f3\n" 
			"LBL_SEQID_3_M0_WRITE_ID_9_HID_2:\n"
			"masknot m5, m6\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x5, rand_ieee754_32\n"
			"flw.ps f25, 768(x5)\n"
			"flw.ps f0, 320(x5)\n"
			"flw.ps f23, 960(x5)\n"
			"fcmov.ps f14, f25, f0, f23\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f11, 704(x17)\n"
			"flw.ps f9, 800(x17)\n"
			"fmin.ps f16, f11, f9\n" 
			"la x19, rand_int_32\n"
			"flw.ps f31, 64(x19)\n"
			"flw.ps f6, 672(x19)\n"
			"fmul.pi f16, f31, f6\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f19, 416(x25)\n"
			"fsin.ps f14, f19\n" 
			"la x5, rand_int_32\n"
			"flw.ps f3, 160(x5)\n"
			"flw.ps f28, 608(x5)\n"
			"frem.pi f3, f3, f28\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f13, 672(x17)\n"
			"fclass.ps f11, f13\n" 
			"LBL_SEQID_3_M0_WRITE_ID_10_HID_2:\n"
			"maskand m3, m3, m5\n" 
			"li x5, CREDINC1\n"
			"ld x5, 0(x5)\n"
			"la x11, rand_ieee754_32\n"
			"flw.ps f28, 128(x11)\n"
			"fcvt.pw.ps f13, f28, rmm\n" 
			"maskpopcz x15, m0\n" 
			"mov.m.x m0, x0, 0xff\n"
			"la x17, rand_ieee754_32\n"
			"flw.ps f11, 352(x17)\n"
			"flw.ps f31, 608(x17)\n"
			"fmax.s f31, f11, f31\n" 
			"la x19, rand_int_32\n"
			"flw.ps f4, 448(x19)\n"
			"fcvt.ps.pw f28, f4, rne\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_3_XMA_QNT_STR_SEQ_ITER_0_HID_2:\n"
			"li x5, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x3 << TENSOR_FMA32_B_NUM_COLS) | (0x2 << TENSOR_FMA32_A_NUM_ROWS) | (0x0 << TENSOR_FMA32_A_NUM_COLS) | (0x8 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x18 << TENSOR_FMA32_SCP_LOC_B) | (0xc3 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_2\n"
			"csrw tensor_error, x0\n"
			"li x5, 4\n" 
			"bne x30, x5, LBL_FAIL_HID_2\n"
			"nop\n"          
		       VSNIP_RSV     
		);
		trans();
		__asm__ __volatile__ ( 
			"nop\n"          
			"addi x30, x30, 1\n"
			"LBL_SEQID_4_M0_WRITE_ID_11_HID_2:\n"
			"maskand m0, m5, m6\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"li x5, 0xe19a\n"
			"csrw tensor_mask, x5\n"	
			"la x17, rand_ieee754_32\n"
			"flw.ps f9, 544(x17)\n"
			"flw.ps f13, 544(x17)\n"
			"fsub.s f26, f9, f13, rne\n" 
			"LBL_SEQID_4_M0_WRITE_ID_12_HID_2:\n"
			"fltm.ps m3, f26, f16\n" 
			"li x5, 0x80340108\n"
			"ld x5, 0(x5)\n"
			"la x20, rand_int_32\n"
			"flw.ps f4, 736(x20)\n"
			"flw.ps f19, 704(x20)\n"
			"fmax.pi f3, f4, f19\n" 
			"li x5, 0xec45\n"
			"csrw tensor_mask, x5\n"	
			"LBL_SEQID_4_M0_WRITE_ID_13_HID_2:\n"
			"fltm.pi m5, f6, f4\n" 
			"li x5, 0x80340128\n"
			"ld x5, 0(x5)\n"
			"li x5, 0x6a9e\n"
			"csrw tensor_mask, x5\n"	
			"la x19, rand_int_32\n"
			"flw.ps f9, 448(x19)\n"
			"flw.ps f11, 576(x19)\n"
			"fmul.pi f31, f9, f11\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f0, 480(x12)\n"
			"flw.ps f10, 0(x12)\n"
			"fmsub.ps f19, f0, f10, f10, rmm\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f28, 384(x11)\n"
			"flw.ps f14, 128(x11)\n"
			"flw.ps f9, 704(x11)\n"
			"fmsub.ps f14, f28, f14, f9, rup\n" 
			"LBL_SEQID_4_M0_WRITE_ID_14_HID_2:\n"
			"mov.m.x m5, x11, 0x25\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"fbci.pi f9, 0x494c6\n" 
			"la x12, rand_int_32\n"
			"flw.ps f26, 288(x12)\n"
			"fpackrepb.pi f3, f26\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f16, 32(x19)\n"
			"flw.ps f19, 288(x19)\n"
			"flw.ps f3, 672(x19)\n"
			"fmadd.ps f25, f16, f19, f3, rtz\n" 
			"la x19, rand_int_32\n"
			"flw.ps f11, 864(x19)\n"
			"fslli.pi f13, f11, 0xe\n" 
			"la x12, rand_int_32\n"
			"flw.ps f13, 640(x12)\n"
			"flw.ps f23, 256(x12)\n"
			"fmulh.pi f13, f13, f23\n" 
			"la x5, rand_int_32\n"
			"flw.ps f23, 608(x5)\n"
			"fsrli.pi f10, f23, 0x3\n" 
			"LBL_SEQID_4_FP_TRANS_ID_15_HID_2:\n"
			"la x15, rand_ieee754_32\n"
			"flw.ps f23, 832(x15)\n"
			"fexp.ps f6, f23\n" 
			"li x5, 0x6120\n"
			"csrw tensor_mask, x5\n"	
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_4_TLD_XMA_STR_SEQ_ID_368613_HID_2:\n"
			"la x31, self_check_32\n"
			"li x5, 0xc0000000000002\n"
			"add x5, x5, x31\n"
			"li x6, 0x10000000000001\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x2 << TENSOR_FMA32_B_NUM_COLS) | (0x2 << TENSOR_FMA32_A_NUM_ROWS) | (0x1 << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x1 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1f << TENSOR_FMA32_SCP_LOC_B) | (0x6 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x2 << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0x2 << TENSOR_QUANT_QUANT_ROW) | (0x14 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x6 << TENSOR_QUANT_TRANSF7) | (0x6 << TENSOR_QUANT_TRANSF6) | (0x7 << TENSOR_QUANT_TRANSF5) | (0x6 << TENSOR_QUANT_TRANSF4) | (0x6 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0x2\n"
			"bne x5, x31, LBL_FAIL_HID_2\n"
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 10\n"
			"LBL_SEQID_4_TLD_XMA_STR_SEQ_ID_125791_HID_2:\n"
			"la x31, self_check_16\n"
			"li x5, 0x9\n"
			"add x5, x5, x31\n"
			"li x6, 0x360000000000003\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x0 << TENSOR_FMA16_B_NUM_COLS) | (0x9 << TENSOR_FMA16_A_NUM_ROWS) | (0x3 << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1b << TENSOR_FMA16_SCP_LOC_B) | (0x0 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0x8\n"
			"bne x5, x31, LBL_FAIL_HID_2\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_2\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_4_XMA_QNT_STR_SEQ_ITER_0_HID_2:\n"
			"li x5, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x1 << TENSOR_FMA32_B_NUM_COLS) | (0x3 << TENSOR_FMA32_A_NUM_ROWS) | (0xc << TENSOR_FMA32_A_NUM_COLS) | (0x9 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x6d << TENSOR_FMA32_SCP_LOC_B) | (0xa3 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x0 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_2\n"
			"csrw tensor_error, x0\n"
			"li x5, 5\n" 
			"bne x30, x5, LBL_FAIL_HID_2\n"
			"nop\n"          
		       VSNIP_RSV     
		);
		load_vpu_regs();
		__asm__ __volatile__ ( 
			"nop\n"          
			"addi x30, x30, 1\n"
			"la x5, rand_ieee754_32\n"
			"flw.ps f13, 864(x5)\n"
			"fcvt.f16.ps f0, f13\n" 
			"maskpopc x5, m5\n" 
			"la x11, rand_int_32\n"
			"flw.ps f16, 352(x11)\n"
			"faddi.pi f28, f16, 0xb6\n" 
			"la x12, rand_int_32\n"
			"flw.ps f13, 256(x12)\n"
			"flw.ps f28, 704(x12)\n"
			"flt.pi f16, f13, f28\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f11, 672(x25)\n"
			"flw.ps f31, 416(x25)\n"
			"fsgnjx.ps f14, f11, f31\n" 
			"la x5, rand_int_32\n"
			"flw.ps f6, 608(x5)\n"
			"fslli.pi f6, f6, 0x5\n" 
			"LBL_SEQID_5_M0_WRITE_ID_16_HID_2:\n"
			"feqm.ps m0, f0, f23\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x5, rand_int_32\n"
			"flw.ps f13, 736(x5)\n"
			"flw.ps f28, 576(x5)\n"
			"fle.pi f6, f13, f28\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f25, 544(x12)\n"
			"frsq.ps f6, f25\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f4, 544(x19)\n"
			"flw.ps f23, 576(x19)\n"
			"fsgnjn.s f25, f4, f23\n" 
			"la x11, rand_int_32\n"
			"flw.ps f0, 672(x11)\n"
			"fmul.pi f25, f0, f0\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f28, 864(x17)\n"
			"flw.ps f26, 320(x17)\n"
			"feq.s x20, f28, f26\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f3, 512(x17)\n"
			"flw.ps f14, 128(x17)\n"
			"fmul.s f6, f3, f14, rup\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f28, 480(x13)\n"
			"flw.ps f25, 992(x13)\n"
			"fmadd.s f13, f28, f28, f25, rtz\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f28, 640(x25)\n"
			"flw.ps f13, 704(x25)\n"
			"fsub.ps f28, f28, f13, rne\n" 
			"la x17, rand_int_32\n"
			"flw.ps f19, 704(x17)\n"
			"fcvt.ps.pwu f28, f19, rtz\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_5_XMA_QNT_STR_SEQ_ITER_0_HID_2:\n"
			"li x5, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x2 << TENSOR_FMA32_B_NUM_COLS) | (0x0 << TENSOR_FMA32_A_NUM_ROWS) | (0x7 << TENSOR_FMA32_A_NUM_COLS) | (0xa << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x5a << TENSOR_FMA32_SCP_LOC_B) | (0x75 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x0 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"LBL_SEQID_5_XMA_QNT_STR_SEQ_ITER_1_HID_2:\n"
			"li x5, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x2 << TENSOR_IMA_B_NUM_COLS) | (0xd << TENSOR_IMA_A_NUM_ROWS) | (0xd << TENSOR_IMA_A_NUM_COLS) | (0x4 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0xee << TENSOR_IMA_SCP_LOC_B) | (0x11 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_2\n"
			"csrw tensor_error, x0\n"
			"li x5, 6\n" 
			"bne x30, x5, LBL_FAIL_HID_2\n"
			"addi x30, x30, 1\n"
			"la x25, rand_ieee754_32\n"
			"flw.ps f10, 256(x25)\n"
			"flw.ps f23, 896(x25)\n"
			"fsgnj.ps f13, f10, f23\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f3, 864(x11)\n"
			"flw.ps f4, 384(x11)\n"
			"flw.ps f11, 480(x11)\n"
			"fmadd.s f3, f3, f4, f11, rne\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f10, 192(x22)\n"
			"flw.ps f14, 768(x22)\n"
			"fsub.ps f9, f10, f14, rmm\n" 
			"la x5, rand_int_32\n"
			"flw.ps f0, 0(x5)\n"
			"fandi.pi f31, f0, 0x71\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f28, 96(x17)\n"
			"flw.ps f6, 992(x17)\n"
			"flt.ps f13, f28, f6\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f31, 544(x17)\n"
			"flw.ps f25, 352(x17)\n"
			"fmul.ps f10, f31, f25, rne\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f4, 352(x13)\n"
			"flw.ps f31, 512(x13)\n"
			"fsub.ps f4, f4, f31, rdn\n" 
			"la x22, rand_int_32\n"
			"flw.ps f25, 288(x22)\n"
			"fpackreph.pi f25, f25\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x11, rand_int_32\n"
			"flw.ps f14, 352(x11)\n"
			"flw.ps f16, 128(x11)\n"
			"fsra.pi f26, f14, f16\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f6, 384(x5)\n"
			"flw.ps f31, 64(x5)\n"
			"flw.ps f3, 992(x5)\n"
			"fmadd.s f3, f6, f31, f3, rdn\n" 
			"la x11, rand_int_32\n"
			"flw.ps f28, 704(x11)\n"
			"flw.ps f3, 320(x11)\n"
			"fmul.pi f23, f28, f3\n" 
			"LBL_SEQID_6_M0_WRITE_ID_17_HID_2:\n"
			"mov.m.x m3, x22, 0x47\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x13, rand_int_32\n"
			"flw.ps f31, 864(x13)\n"
			"flw.ps f13, 800(x13)\n"
			"fadd.pi f19, f31, f13\n" 
			"la x20, rand_int_32\n"
			"flw.ps f6, 352(x20)\n"
			"flw.ps f14, 800(x20)\n"
			"fmaxu.pi f11, f6, f14\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f4, 480(x25)\n"
			"flw.ps f6, 864(x25)\n"
			"flw.ps f11, 896(x25)\n"
			"fnmadd.s f3, f4, f6, f11, rne\n" 
			"LBL_SEQID_6_FP_TRANS_ID_18_HID_2:\n"
			"la x25, rand_ieee754_32\n"
			"flw.ps f14, 448(x25)\n"
			"flog.ps f28, f14\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_6_XMA_QNT_STR_SEQ_ITER_0_HID_2:\n"
			"li x5, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x1 << TENSOR_IMA_B_NUM_COLS) | (0x2 << TENSOR_IMA_A_NUM_ROWS) | (0x6 << TENSOR_IMA_A_NUM_COLS) | (0xf << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x9 << TENSOR_IMA_SCP_LOC_B) | (0x48 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x0 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_2\n"
			"csrw tensor_error, x0\n"
			"li x5, 7\n" 
			"bne x30, x5, LBL_FAIL_HID_2\n"
			"nop \n"
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=1); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"nop \n"
			"addi x29, x29, -1\n"
			"bne x29, x0, LBL_OUTER_LOOP_SEQ_HID_2\n"
			"nop\n"          
		       VSNIP_RSV     
		);
		load_vpu_regs();
		__asm__ __volatile__ ( 
			"nop\n"          
			"nop\n"          
		       VSNIP_RSV     
		);
		trans();
		__asm__ __volatile__ ( 
			"nop\n"          
			"la x31, check_sum_hid_2\n"
			"lw x5, (x31)\n"
			"bne x30, x5, LBL_FAIL_HID_2\n"
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
		c_sync_point(sync_odd_harts=1); 
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
	if ( hid%16 == 3 ) { 
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
			"csrwi mhpmevent3, 11\n"   
			"csrwi mhpmevent4, 1\n"   
			"csrwi mhpmevent5, 6\n"   
			"csrwi mhpmevent6, 12\n"   
			"csrwi mhpmevent7, 23\n"   
			"csrwi mhpmevent8, 21\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f2,  864 (x5)\n"
			"flw.ps f31,  640 (x5)\n"
			"flw.ps f8,  320 (x5)\n"
			"flw.ps f4,  832 (x5)\n"
			"flw.ps f28,  416 (x5)\n"
			"flw.ps f16,  64 (x5)\n"
			"flw.ps f13,  672 (x5)\n"
			"flw.ps f29,  96 (x5)\n"
			"flw.ps f6,  32 (x5)\n"
			"flw.ps f26,  800 (x5)\n"
			"flw.ps f12,  960 (x5)\n"
			"flw.ps f7,  256 (x5)\n"
			"flw.ps f3,  448 (x5)\n"
			"flw.ps f5,  384 (x5)\n"
			"flw.ps f1,  928 (x5)\n"
			"flw.ps f0,  352 (x5)\n"
			"flw.ps f19,  128 (x5)\n"
			"flw.ps f18,  192 (x5)\n"
			"flw.ps f22,  0 (x5)\n"
			"flw.ps f10,  992 (x5)\n"
			"flw.ps f15,  896 (x5)\n"
			"flw.ps f17,  608 (x5)\n"
			"flw.ps f23,  768 (x5)\n"
			"flw.ps f14,  512 (x5)\n"
			"flw.ps f30,  224 (x5)\n"
			"flw.ps f9,  480 (x5)\n"
			"flw.ps f27,  288 (x5)\n"
			"flw.ps f21,  160 (x5)\n"
			"flw.ps f11,  736 (x5)\n"
			"flw.ps f24,  544 (x5)\n"
			"flw.ps f25,  576 (x5)\n"
			"flw.ps f20,  704 (x5)\n"
			"csrw fflags, zero\n"
			"csrwi frm, 0x0\n"
			"li x31, 100\n"
			"1:\n" 
			"addi x31, x31, -1\n"
			"bne x31, x0, 1b\n" 
			"csrw fcc, x0\n" 
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=1); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"LBL_KERNEL_HID_3:\n"          
			"li x5, 0xFEEDFEED\n"
			"slti x0, x0, 0x7f2\n"
			"la x31, check_sum_hid_3\n"
			"li x5, 0x7\n" 
			"sw x5, (x31)\n"
			"li x29, 0x7\n"
			"LBL_OUTER_LOOP_SEQ_HID_3:\n"
			"nop \n"
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=1); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"nop \n"
			"li x30, 0\n"
			"addi x30, x30, 1\n"
			"la x17, rand_int_32\n"
			"flw.ps f19, 672(x17)\n"
			"fpackreph.pi f24, f19\n" 
			"la x22, rand_int_32\n"
			"flw.ps f19, 160(x22)\n"
			"flw.ps f17, 832(x22)\n"
			"feq.pi f24, f19, f17\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f30, 448(x22)\n"
			"fclass.ps f1, f30\n" 
			"maskpopc x22, m5\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_3:\n"
			"fsetm.pi m6, f24\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x15, rand_int_32\n"
			"flw.ps f11, 96(x15)\n"
			"fslli.pi f18, f11, 0x6\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f24, 384(x5)\n"
			"flw.ps f22, 800(x5)\n"
			"flw.ps f31, 992(x5)\n"
			"fnmsub.ps f31, f24, f22, f31, rmm\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f27, 704(x5)\n"
			"flw.ps f9, 288(x5)\n"
			"fle.s x5, f27, f9\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f7, 384(x10)\n"
			"fcvt.wu.s x14, f7, rup\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f22, 704(x16)\n"
			"fcvt.lu.s x18, f22, rmm\n" 
			"la x16, rand_int_32\n"
			"flw.ps f21, 736(x16)\n"
			"flw.ps f22, 832(x16)\n"
			"fmaxu.pi f31, f21, f22\n" 
			"la x27, rand_int_32\n"
			"flw.ps f21, 416(x27)\n"
			"fltu.pi f11, f21, f21\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f24, 608(x17)\n"
			"flw.ps f2, 576(x17)\n"
			"fsub.ps f27, f24, f2, rdn\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_3:\n"
			"maskand m6, m6, m5\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x5, rand_ieee754_32\n"
			"flw.ps f1, 992(x5)\n"
			"flw.ps f18, 992(x5)\n"
			"fsgnjx.ps f11, f1, f18\n" 
			"la x16, rand_int_32\n"
			"flw.ps f1, 32(x16)\n"
			"fandi.pi f23, f1, 0x4b\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_3\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_1_M0_WRITE_ID_2_HID_3:\n"
			"feqm.ps m6, f12, f12\n" 
			"li x5, 0x803401A0\n"
			"ld x5, 0(x5)\n"
			"la x27, rand_ieee754_16\n"
			"flw.ps f9, 448(x27)\n"
			"fcvt.ps.f16 f17, f9\n" 
			"LBL_SEQID_1_M0_WRITE_ID_3_HID_3:\n"
			"feqm.ps m4, f27, f21\n" 
			"li x5, 0x803401F0\n"
			"ld x5, 0(x5)\n"
			"la x27, rand_int_32\n"
			"flw.ps f7, 800(x27)\n"
			"fadd.pi f22, f7, f7\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f2, 800(x17)\n"
			"flw.ps f30, 192(x17)\n"
			"fmin.s f11, f2, f30\n" 
			"la x14, rand_ieee754_16\n"
			"flw.ps f30, 0(x14)\n"
			"fcvt.ps.f16 f17, f30\n" 
			"la x27, rand_int_32\n"
			"flw.ps f18, 896(x27)\n"
			"flw.ps f23, 672(x27)\n"
			"fsub.pi f22, f18, f23\n" 
			"la x17, rand_int_32\n"
			"flw.ps f19, 416(x17)\n"
			"flw.ps f2, 64(x17)\n"
			"fmulhu.pi f1, f19, f2\n" 
			"la x27, rand_int_32\n"
			"flw.ps f17, 160(x27)\n"
			"flw.ps f18, 704(x27)\n"
			"fmul.pi f18, f17, f18\n" 
			"li x10, 0xb4d82fa8c550a1f1\n"
			"fcvt.s.w f31, x10, rtz\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f9, 640(x18)\n"
			"flw.ps f18, 672(x18)\n"
			"fmul.s f17, f9, f18, rdn\n" 
			"maskpopcz x14, m2\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f12, 640(x16)\n"
			"ffrc.ps f11, f12\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f2, 864(x26)\n"
			"fcvt.lu.s x22, f2, rtz\n" 
			"la x18, rand_int_32\n"
			"flw.ps f22, 608(x18)\n"
			"fsrai.pi f31, f22, 0x0\n" 
			"LBL_SEQID_1_FP_TRANS_ID_4_HID_3:\n"
			"la x18, rand_ieee754_32\n"
			"flw.ps f22, 896(x18)\n"
			"fexp.ps f1, f22\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_3\n"
			"addi x30, x30, 1\n"
			"la x14, rand_ieee754_32\n"
			"flw.ps f22, 128(x14)\n"
			"ffrc.ps f23, f22\n" 
			"la x15, rand_int_32\n"
			"flw.ps f23, 704(x15)\n"
			"flw.ps f7, 544(x15)\n"
			"fmax.pi f7, f23, f7\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f31, 704(x15)\n"
			"flw.ps f12, 64(x15)\n"
			"flw.ps f23, 896(x15)\n"
			"fmadd.s f12, f31, f12, f23, rtz\n" 
			"maskpopcz x27, m2\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f24, 416(x18)\n"
			"flw.ps f2, 256(x18)\n"
			"fsgnjn.s f7, f24, f2\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f22, 320(x16)\n"
			"fround.ps f30, f22, rmm\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f27, 512(x18)\n"
			"flw.ps f18, 288(x18)\n"
			"flt.s x10, f27, f18\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f12, 736(x15)\n"
			"fcvt.pwu.ps f7, f12, rdn\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f12, 768(x5)\n"
			"flw.ps f27, 0(x5)\n"
			"flw.ps f1, 384(x5)\n"
			"fmadd.s f9, f12, f27, f1, rne\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f12, 96(x22)\n"
			"fcvt.pwu.ps f11, f12, rmm\n" 
			"la x10, rand_int_32\n"
			"flw.ps f22, 992(x10)\n"
			"flw.ps f21, 288(x10)\n"
			"frem.pi f12, f22, f21\n" 
			"la x5, rand_int_32\n"
			"flw.ps f21, 64(x5)\n"
			"fpackrepb.pi f17, f21\n" 
			"fbci.pi f23, 0x12b96\n" 
			"la x15, rand_int_32\n"
			"flw.ps f17, 960(x15)\n"
			"flw.ps f21, 736(x15)\n"
			"flt.pi f18, f17, f21\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f24, 800(x14)\n"
			"flw.ps f12, 992(x14)\n"
			"fsgnjx.ps f22, f24, f12\n" 
			"la x18, rand_int_32\n"
			"flw.ps f23, 768(x18)\n"
			"flw.ps f18, 736(x18)\n"
			"fmax.pi f7, f23, f18\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 3\n" 
			"bne x30, x5, LBL_FAIL_HID_3\n"
			"nop\n"          
		       VSNIP_RSV     
		);
		load_vpu_regs();
		__asm__ __volatile__ ( 
			"nop\n"          
			"addi x30, x30, 1\n"
			"la x16, rand_int_32\n"
			"flw.ps f27, 480(x16)\n"
			"flw.ps f24, 32(x16)\n"
			"fxor.pi f12, f27, f24\n" 
			"la x10, rand_int_32\n"
			"flw.ps f9, 384(x10)\n"
			"flw.ps f1, 64(x10)\n"
			"fmulhu.pi f27, f9, f1\n" 
			"la x18, rand_int_32\n"
			"flw.ps f17, 416(x18)\n"
			"flw.ps f31, 832(x18)\n"
			"fsub.pi f19, f17, f31\n" 
			"LBL_SEQID_3_M0_WRITE_ID_5_HID_3:\n"
			"fsetm.pi m6, f31\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x15, rand_int_32\n"
			"flw.ps f12, 512(x15)\n"
			"flw.ps f30, 768(x15)\n"
			"fsrl.pi f21, f12, f30\n" 
			"la x22, rand_int_32\n"
			"flw.ps f31, 704(x22)\n"
			"fsrli.pi f19, f31, 0x9\n" 
			"la x16, rand_int_32\n"
			"flw.ps f31, 544(x16)\n"
			"flw.ps f27, 160(x16)\n"
			"flt.pi f18, f31, f27\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f2, 800(x10)\n"
			"flw.ps f24, 832(x10)\n"
			"fmul.ps f2, f2, f24, rtz\n" 
			"la x16, rand_int_32\n"
			"flw.ps f31, 608(x16)\n"
			"fcvt.ps.pwu f23, f31, rmm\n" 
			"la x18, rand_int_32\n"
			"flw.ps f12, 800(x18)\n"
			"flw.ps f31, 992(x18)\n"
			"feq.pi f9, f12, f31\n" 
			"la x22, rand_int_32\n"
			"flw.ps f24, 768(x22)\n"
			"flw.ps f1, 128(x22)\n"
			"fadd.pi f18, f24, f1\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f7, 576(x16)\n"
			"flw.ps f1, 320(x16)\n"
			"fsub.ps f17, f7, f1, rdn\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f23, 96(x10)\n"
			"flw.ps f27, 128(x10)\n"
			"fcmovm.ps f7, f23, f27\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f17, 768(x18)\n"
			"flw.ps f19, 160(x18)\n"
			"flt.ps f9, f17, f19\n" 
			"la x18, rand_int_32\n"
			"flw.ps f7, 928(x18)\n"
			"flw.ps f31, 768(x18)\n"
			"fsrl.pi f21, f7, f31\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f1, 288(x18)\n"
			"flw.ps f22, 320(x18)\n"
			"fmul.s f19, f1, f22, rup\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 4\n" 
			"bne x30, x5, LBL_FAIL_HID_3\n"
			"nop\n"          
		       VSNIP_RSV     
		);
		trans();
		__asm__ __volatile__ ( 
			"nop\n"          
			"addi x30, x30, 1\n"
			"LBL_SEQID_4_FP_TRANS_ID_6_HID_3:\n"
			"la x16, rand_ieee754_32\n"
			"flw.ps f31, 480(x16)\n"
			"flog.ps f24, f31\n" 
			"la x5, rand_int_32\n"
			"flw.ps f24, 608(x5)\n"
			"flw.ps f19, 704(x5)\n"
			"fsrl.pi f27, f24, f19\n" 
			"la x10, rand_int_32\n"
			"flw.ps f21, 416(x10)\n"
			"flw.ps f1, 128(x10)\n"
			"fmax.pi f24, f21, f1\n" 
			"LBL_SEQID_4_FP_TRANS_ID_7_HID_3:\n"
			"la x22, rand_ieee754_32\n"
			"flw.ps f27, 288(x22)\n"
			"fexp.ps f30, f27\n" 
			"la x26, rand_int_32\n"
			"flw.ps f21, 192(x26)\n"
			"fcvt.ps.pw f23, f21, rne\n" 
			"la x18, rand_int_32\n"
			"flw.ps f19, 768(x18)\n"
			"flw.ps f23, 64(x18)\n"
			"fmul.pi f7, f19, f23\n" 
			"la x15, rand_int_32\n"
			"flw.ps f19, 608(x15)\n"
			"flw.ps f11, 224(x15)\n"
			"fle.pi f2, f19, f11\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f22, 256(x22)\n"
			"flw.ps f11, 960(x22)\n"
			"fsgnjn.s f17, f22, f11\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f1, 32(x5)\n"
			"flw.ps f17, 512(x5)\n"
			"fmadd.s f9, f1, f17, f17, rup\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f1, 608(x22)\n"
			"flw.ps f30, 576(x22)\n"
			"flt.s x5, f1, f30\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f11, 320(x17)\n"
			"flw.ps f23, 352(x17)\n"
			"flw.ps f17, 640(x17)\n"
			"fmsub.ps f9, f11, f23, f17, rup\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f2, 160(x16)\n"
			"flw.ps f18, 416(x16)\n"
			"flw.ps f19, 704(x16)\n"
			"fmsub.s f30, f2, f18, f19, rup\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f30, 768(x26)\n"
			"fsub.s f30, f30, f30, rne\n" 
			"LBL_SEQID_4_M0_WRITE_ID_8_HID_3:\n"
			"masknot m2, m2\n" 
			"li x5, 0x80340128\n"
			"ld x5, 0(x5)\n"
			"la x26, rand_ieee754_32\n"
			"flw.ps f22, 512(x26)\n"
			"feq.s x27, f22, f22\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f7, 480(x22)\n"
			"flw.ps f21, 704(x22)\n"
			"feq.ps f27, f7, f21\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 5\n" 
			"bne x30, x5, LBL_FAIL_HID_3\n"
			"nop\n"          
		       VSNIP_RSV     
		);
		load_vpu_regs();
		__asm__ __volatile__ ( 
			"nop\n"          
			"addi x30, x30, 1\n"
			"la x17, rand_ieee754_32\n"
			"flw.ps f7, 512(x17)\n"
			"flw.ps f31, 352(x17)\n"
			"fsub.ps f9, f7, f31, rtz\n" 
			"LBL_SEQID_5_M0_WRITE_ID_9_HID_3:\n"
			"fltm.ps m4, f31, f11\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x14, rand_ieee754_32\n"
			"flw.ps f12, 640(x14)\n"
			"flw.ps f19, 416(x14)\n"
			"fnmsub.ps f30, f12, f19, f12, rmm\n" 
			"la x18, rand_int_32\n"
			"flw.ps f2, 544(x18)\n"
			"flw.ps f21, 992(x18)\n"
			"feq.pi f30, f2, f21\n" 
			"la x18, rand_int_32\n"
			"flw.ps f11, 64(x18)\n"
			"flw.ps f1, 672(x18)\n"
			"fltu.pi f7, f11, f1\n" 
			"LBL_SEQID_5_M0_WRITE_ID_10_HID_3:\n"
			"fsetm.pi m6, f1\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x22, rand_ieee754_32\n"
			"flw.ps f7, 256(x22)\n"
			"fsqrt.ps f23, f7\n" 
			"LBL_SEQID_5_FP_TRANS_ID_11_HID_3:\n"
			"la x5, rand_ieee754_32\n"
			"flw.ps f30, 480(x5)\n"
			"frcp.ps f31, f30\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f27, 640(x15)\n"
			"fsqrt.s f18, f27, rdn\n" 
			"mova.x.m x14\n" 
			"LBL_SEQID_5_M0_WRITE_ID_12_HID_3:\n"
			"masknot m5, m6\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"fbci.ps f2, 0x71a67\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f19, 96(x15)\n"
			"flw.ps f22, 256(x15)\n"
			"fsub.s f2, f19, f22, rdn\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f31, 384(x22)\n"
			"fcvt.pwu.ps f18, f31, rmm\n" 
			"la x26, rand_int_32\n"
			"flw.ps f24, 544(x26)\n"
			"faddi.pi f11, f24, 0x1e8\n" 
			"LBL_SEQID_5_M0_WRITE_ID_13_HID_3:\n"
			"maskxor m5, m5, m5\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"csrw tensor_error, zero\n" 
			"li x5, 6\n" 
			"bne x30, x5, LBL_FAIL_HID_3\n"
			"addi x30, x30, 1\n"
			"la x17, rand_int_32\n"
			"flw.ps f7, 32(x17)\n"
			"flw.ps f12, 928(x17)\n"
			"fltu.pi f12, f7, f12\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f9, 128(x5)\n"
			"flw.ps f1, 128(x5)\n"
			"feq.s x18, f9, f1\n" 
			"LBL_SEQID_6_FP_TRANS_ID_14_HID_3:\n"
			"la x18, rand_ieee754_32\n"
			"flw.ps f23, 608(x18)\n"
			"frcp.ps f18, f23\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f31, 480(x5)\n"
			"fround.ps f9, f31, rdn\n" 
			"la x15, rand_int_32\n"
			"flw.ps f31, 800(x15)\n"
			"fnot.pi f17, f31\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f17, 992(x16)\n"
			"flw.ps f12, 256(x16)\n"
			"flw.ps f30, 640(x16)\n"
			"fnmsub.s f19, f17, f12, f30, rmm\n" 
			"la x16, rand_int_32\n"
			"flw.ps f7, 576(x16)\n"
			"fcvt.ps.pw f7, f7, rtz\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f23, 832(x10)\n"
			"flw.ps f31, 0(x10)\n"
			"fadd.s f1, f23, f31, rtz\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f27, 736(x5)\n"
			"flw.ps f9, 32(x5)\n"
			"feq.s x16, f27, f9\n" 
			"la x26, rand_int_32\n"
			"flw.ps f22, 384(x26)\n"
			"flw.ps f18, 640(x26)\n"
			"feq.pi f12, f22, f18\n" 
			"la x16, rand_int_32\n"
			"flw.ps f17, 416(x16)\n"
			"fsat8.pi f21, f17\n" 
			"la x26, rand_int_32\n"
			"flw.ps f27, 768(x26)\n"
			"flw.ps f19, 288(x26)\n"
			"fminu.pi f7, f27, f19\n" 
			"LBL_SEQID_6_FP_TRANS_ID_15_HID_3:\n"
			"la x17, rand_ieee754_32\n"
			"flw.ps f17, 256(x17)\n"
			"flog.ps f22, f17\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f24, 512(x10)\n"
			"flw.ps f7, 896(x10)\n"
			"feq.s x22, f24, f7\n" 
			"la x10, rand_int_32\n"
			"flw.ps f11, 288(x10)\n"
			"flw.ps f22, 288(x10)\n"
			"fsll.pi f1, f11, f22\n" 
			"fbci.ps f18, 0x3dcce\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 7\n" 
			"bne x30, x5, LBL_FAIL_HID_3\n"
			"nop \n"
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=1); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"nop \n"
			"addi x29, x29, -1\n"
			"bne x29, x0, LBL_OUTER_LOOP_SEQ_HID_3\n"
			"nop\n"          
		       VSNIP_RSV     
		);
		load_vpu_regs();
		__asm__ __volatile__ ( 
			"nop\n"          
			"nop\n"          
		       VSNIP_RSV     
		);
		trans();
		__asm__ __volatile__ ( 
			"nop\n"          
			"la x31, check_sum_hid_3\n"
			"lw x5, (x31)\n"
			"bne x30, x5, LBL_FAIL_HID_3\n"
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
		c_sync_point(sync_odd_harts=1); 
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
	if ( hid%16 == 4 ) { 
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
			"csrwi mhpmevent3, 16\n"   
			"csrwi mhpmevent4, 6\n"   
			"csrwi mhpmevent5, 4\n"   
			"csrwi mhpmevent6, 27\n"   
			"csrwi mhpmevent7, 6\n"   
			"csrwi mhpmevent8, 3\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f20,  896 (x5)\n"
			"flw.ps f9,  672 (x5)\n"
			"flw.ps f12,  736 (x5)\n"
			"flw.ps f15,  352 (x5)\n"
			"flw.ps f10,  256 (x5)\n"
			"flw.ps f8,  96 (x5)\n"
			"flw.ps f5,  288 (x5)\n"
			"flw.ps f13,  320 (x5)\n"
			"flw.ps f14,  224 (x5)\n"
			"flw.ps f22,  704 (x5)\n"
			"flw.ps f31,  416 (x5)\n"
			"flw.ps f18,  128 (x5)\n"
			"flw.ps f3,  992 (x5)\n"
			"flw.ps f23,  0 (x5)\n"
			"flw.ps f0,  864 (x5)\n"
			"flw.ps f28,  800 (x5)\n"
			"flw.ps f4,  576 (x5)\n"
			"flw.ps f24,  384 (x5)\n"
			"flw.ps f30,  832 (x5)\n"
			"flw.ps f25,  192 (x5)\n"
			"flw.ps f2,  32 (x5)\n"
			"flw.ps f7,  448 (x5)\n"
			"flw.ps f16,  768 (x5)\n"
			"flw.ps f21,  480 (x5)\n"
			"flw.ps f1,  928 (x5)\n"
			"flw.ps f26,  640 (x5)\n"
			"flw.ps f19,  608 (x5)\n"
			"flw.ps f17,  960 (x5)\n"
			"flw.ps f27,  512 (x5)\n"
			"flw.ps f11,  64 (x5)\n"
			"flw.ps f29,  544 (x5)\n"
			"flw.ps f6,  160 (x5)\n"
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
			"csrw fcc, x0\n" 
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=1); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"LBL_KERNEL_HID_4:\n"          
			"li x5, 0xFEEDFEED\n"
			"slti x0, x0, 0x7f2\n"
			"la x31, check_sum_hid_4\n"
			"li x5, 0x7\n" 
			"sw x5, (x31)\n"
			"li x29, 0x7\n"
			"LBL_OUTER_LOOP_SEQ_HID_4:\n"
			"nop \n"
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=1); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"nop \n"
			"li x30, 0\n"
			"addi x30, x30, 1\n"
			"maskpopc x7, m3\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f18, 544(x25)\n"
			"ffrc.ps f9, f18\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f19, 256(x18)\n"
			"flw.ps f28, 704(x18)\n"
			"flt.ps f13, f19, f28\n" 
			"la x17, rand_int_32\n"
			"flw.ps f19, 320(x17)\n"
			"fsatu8.pi f27, f19\n" 
			"la x11, rand_int_32\n"
			"flw.ps f31, 544(x11)\n"
			"flw.ps f13, 800(x11)\n"
			"fdiv.pi f13, f31, f13\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f18, 352(x25)\n"
			"flw.ps f31, 256(x25)\n"
			"fadd.ps f9, f18, f31, rtz\n" 
			"la x12, rand_int_32\n"
			"flw.ps f19, 832(x12)\n"
			"fpackreph.pi f13, f19\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f3, 576(x19)\n"
			"flw.ps f16, 160(x19)\n"
			"fsgnjx.s f19, f3, f16\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_4:\n"
			"masknot m5, m5\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x17, rand_ieee754_32\n"
			"flw.ps f11, 64(x17)\n"
			"fcvt.f16.ps f26, f11\n" 
			"maskpopcz x20, m3\n" 
			"la x15, rand_int_32\n"
			"flw.ps f8, 96(x15)\n"
			"flw.ps f31, 320(x15)\n"
			"fadd.pi f18, f8, f31\n" 
			"la x13, rand_int_32\n"
			"flw.ps f16, 736(x13)\n"
			"flw.ps f25, 512(x13)\n"
			"fmax.pi f5, f16, f25\n" 
			"la x18, rand_int_32\n"
			"flw.ps f8, 640(x18)\n"
			"flw.ps f26, 608(x18)\n"
			"flt.pi f13, f8, f26\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f19, 672(x12)\n"
			"flw.ps f9, 992(x12)\n"
			"fsub.s f0, f19, f9, rtz\n" 
			"la x28, rand_int_32\n"
			"flw.ps f3, 32(x28)\n"
			"flw.ps f8, 768(x28)\n"
			"fmul.pi f26, f3, f8\n" 
			"csrw tensor_error, zero\n" 
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=0); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ITER_0_HID_4:\n"
			"li x6, ((0x4 << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0xa << TENSOR_QUANT_QUANT_ROW) | (0x29 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_4\n"
			"csrw tensor_error, x0\n"
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_4\n"
			"addi x30, x30, 1\n"
			"la x15, rand_ieee754_32\n"
			"flw.ps f3, 352(x15)\n"
			"flw.ps f5, 352(x15)\n"
			"flw.ps f14, 768(x15)\n"
			"fnmadd.ps f0, f3, f5, f14, rmm\n" 
			"fbci.ps f31, 0x40ae0\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f16, 704(x20)\n"
			"frsq.ps f19, f16\n" 
			"la x18, rand_int_32\n"
			"flw.ps f19, 32(x18)\n"
			"fsatu8.pi f8, f19\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f28, 640(x18)\n"
			"flw.ps f5, 608(x18)\n"
			"fmax.s f26, f28, f5\n" 
			"li x5, 0x70f86ecafabfeb\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0x8ee650b2b2649a0e\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"la x17, rand_int_32\n"
			"flw.ps f8, 160(x17)\n"
			"flw.ps f16, 640(x17)\n"
			"feq.pi f19, f8, f16\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f13, 480(x25)\n"
			"flw.ps f16, 480(x25)\n"
			"feq.s x20, f13, f16\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f16, 608(x7)\n"
			"frsq.ps f8, f16\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f25, 192(x28)\n"
			"flw.ps f13, 768(x28)\n"
			"flt.s x13, f25, f13\n" 
			"la x28, rand_int_32\n"
			"flw.ps f16, 928(x28)\n"
			"fpackrepb.pi f13, f16\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f11, 736(x15)\n"
			"feq.ps f9, f11, f11\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f18, 672(x11)\n"
			"flw.ps f27, 608(x11)\n"
			"fsgnj.s f8, f18, f27\n" 
			"li x5, 0x485d\n"
			"csrw tensor_mask, x5\n"	
			"la x15, rand_ieee754_32\n"
			"flw.ps f27, 608(x15)\n"
			"fsqrt.s f31, f27, rtz\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f18, 0(x13)\n"
			"flw.ps f31, 768(x13)\n"
			"flw.ps f14, 768(x13)\n"
			"fmadd.s f28, f18, f31, f14, rdn\n" 
			"la x28, rand_int_32\n"
			"flw.ps f27, 704(x28)\n"
			"flw.ps f31, 96(x28)\n"
			"fsrl.pi f14, f27, f31\n" 
			"LBL_SEQID_1_FP_TRANS_ID_1_HID_4:\n"
			"la x25, rand_ieee754_32\n"
			"flw.ps f16, 736(x25)\n"
			"fexp.ps f16, f16\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ID_824316_HID_4:\n"
			"li x5, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x0 << TENSOR_FMA32_B_NUM_COLS) | (0x9 << TENSOR_FMA32_A_NUM_ROWS) | (0x2 << TENSOR_FMA32_A_NUM_COLS) | (0x1 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0xd7 << TENSOR_FMA32_SCP_LOC_B) | (0x1b << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x6, ((0x19 << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0x9 << TENSOR_QUANT_QUANT_ROW) | (0x3 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x2 << TENSOR_QUANT_TRANSF4) | (0x7 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 7\n"
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_4\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_1_RDX_ID_1_SHIRE_0_HID_4:\n"
			"li x28, 0xa0000000010000a\n"
			"csrw tensor_reduce, x28\n" 
			"csrwi tensor_wait, 9\n" 
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=0); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ITER_0_HID_4:\n"
			"li x6, ((0x16 << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0x0 << TENSOR_QUANT_QUANT_ROW) | (0x35 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x0 << TENSOR_QUANT_TRANSF1) | (0x0 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_4\n"
			"csrw tensor_error, x0\n"
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_4\n"
			"addi x30, x30, 1\n"
			"la x20, rand_int_32\n"
			"flw.ps f13, 96(x20)\n"
			"flw.ps f8, 896(x20)\n"
			"fand.pi f18, f13, f8\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f9, 480(x7)\n"
			"fcvt.pwu.ps f16, f9, rtz\n" 
			"LBL_SEQID_2_M0_WRITE_ID_2_HID_4:\n"
			"mova.m.x x12\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x11, rand_int_32\n"
			"flw.ps f25, 0(x11)\n"
			"fcvt.ps.pw f13, f25, rtz\n" 
			"la x17, rand_int_32\n"
			"flw.ps f19, 512(x17)\n"
			"flw.ps f18, 384(x17)\n"
			"flt.pi f3, f19, f18\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f11, 672(x17)\n"
			"fcvt.wu.s x15, f11, rmm\n" 
			"la x17, rand_int_32\n"
			"flw.ps f27, 0(x17)\n"
			"fcvt.ps.pwu f13, f27, rtz\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f26, 352(x25)\n"
			"flw.ps f18, 96(x25)\n"
			"fnmadd.ps f25, f26, f26, f18, rup\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f5, 992(x13)\n"
			"flw.ps f25, 416(x13)\n"
			"fsub.s f31, f5, f25, rtz\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f28, 192(x20)\n"
			"fround.ps f18, f28, rdn\n" 
			"LBL_SEQID_2_M0_WRITE_ID_3_HID_4:\n"
			"fltm.ps m2, f25, f16\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x12, rand_int_32\n"
			"flw.ps f28, 416(x12)\n"
			"faddi.pi f25, f28, 0x18c\n" 
			"la x12, rand_int_32\n"
			"flw.ps f13, 544(x12)\n"
			"fsrai.pi f9, f13, 0x2\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f16, 480(x15)\n"
			"fclass.s x20, f16\n" 
			"la x28, rand_int_32\n"
			"flw.ps f11, 320(x28)\n"
			"fmax.pi f18, f11, f11\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f16, 896(x11)\n"
			"flw.ps f25, 704(x11)\n"
			"flw.ps f26, 320(x11)\n"
			"fmsub.s f9, f16, f25, f26, rdn\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_2_TLD_XMA_STR_SEQ_ITER_0_HID_4:\n"
			"la x31, self_check_16\n"
			"li x5, 0x4000000000000e\n"
			"add x5, x5, x31\n"
			"li x6, 0x360000000000003\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x3 << TENSOR_FMA16_B_NUM_COLS) | (0xe << TENSOR_FMA16_A_NUM_ROWS) | (0x3 << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1b << TENSOR_FMA16_SCP_LOC_B) | (0x2 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0x8\n"
			"bne x5, x31, LBL_FAIL_HID_4\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_4\n"
			"csrw tensor_error, x0\n"
			"li x5, 3\n" 
			"bne x30, x5, LBL_FAIL_HID_4\n"
			"nop\n"          
		       VSNIP_RSV     
		);
		load_vpu_regs();
		__asm__ __volatile__ ( 
			"nop\n"          
			"addi x30, x30, 1\n"
			"la x13, rand_ieee754_32\n"
			"flw.ps f26, 896(x13)\n"
			"fcvt.w.s x12, f26, rup\n" 
			"LBL_SEQID_3_M0_WRITE_ID_4_HID_4:\n"
			"maskand m4, m4, m2\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0xee23ff8c33\n"
			"fsw.ps f0, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"li x5, 0x629a\n"
			"csrw tensor_mask, x5\n"	
			"la x28, rand_int_32\n"
			"flw.ps f9, 768(x28)\n"
			"flw.ps f31, 544(x28)\n"
			"fadd.pi f31, f9, f31\n" 
			"la x15, rand_int_32\n"
			"flw.ps f8, 960(x15)\n"
			"fcvt.ps.pwu f18, f8, rne\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f14, 832(x28)\n"
			"flw.ps f28, 992(x28)\n"
			"fcmovm.ps f8, f14, f28\n" 
			"la x17, rand_int_32\n"
			"flw.ps f26, 576(x17)\n"
			"flw.ps f14, 384(x17)\n"
			"fadd.pi f3, f26, f14\n" 
			"LBL_SEQID_3_M0_WRITE_ID_5_HID_4:\n"
			"mov.m.x m3, x17, 0x6a\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x11, rand_ieee754_32\n"
			"flw.ps f25, 64(x11)\n"
			"fcvt.pw.ps f14, f25, rtz\n" 
			"LBL_SEQID_3_M0_WRITE_ID_6_HID_4:\n"
			"mov.m.x m5, x7, 0x7e\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x13, rand_ieee754_32\n"
			"flw.ps f11, 416(x13)\n"
			"flw.ps f25, 736(x13)\n"
			"flw.ps f3, 128(x13)\n"
			"fcmov.ps f31, f11, f25, f3\n" 
			"LBL_SEQID_3_M0_WRITE_ID_7_HID_4:\n"
			"masknot m5, m2\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0x5bf52b449a\n"
			"flw.ps f5, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x15, rand_ieee754_32\n"
			"flw.ps f11, 480(x15)\n"
			"flt.ps f13, f11, f11\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f31, 320(x25)\n"
			"fsgnjn.s f11, f31, f31\n" 
			"li x5, 0x764b355eb0f7befe\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0xe38dcaf43d916d1\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"la x17, rand_int_32\n"
			"flw.ps f9, 0(x17)\n"
			"flw.ps f31, 736(x17)\n"
			"fmaxu.pi f16, f9, f31\n" 
			"la x15, rand_int_32\n"
			"flw.ps f28, 64(x15)\n"
			"fcvt.ps.pwu f5, f28, rdn\n" 
			"li x5, 0x36a4\n"
			"csrw tensor_mask, x5\n"	
			"la x11, rand_ieee754_32\n"
			"flw.ps f0, 288(x11)\n"
			"flw.ps f13, 416(x11)\n"
			"flt.ps f28, f0, f13\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_3_TLD_XMA_STR_SEQ_ID_340923_HID_4:\n"
			"la x31, self_check_8\n"
			"li x5, 0x4000000000000e\n"
			"add x5, x5, x31\n"
			"li x6, 0x3e0000000000002\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x0 << TENSOR_IMA_B_NUM_COLS) | (0xe << TENSOR_IMA_A_NUM_ROWS) | (0x2 << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1f << TENSOR_IMA_SCP_LOC_B) | (0x2 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fmvz.x.ps x31, f0, 0\n"
			"li x5, 0xc\n"
			"bne x5, x31, LBL_FAIL_HID_4\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_4\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_3_XMA_QNT_STR_SEQ_ITER_0_HID_4:\n"
			"li x5, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x0 << TENSOR_FMA32_B_NUM_COLS) | (0x5 << TENSOR_FMA32_A_NUM_ROWS) | (0x0 << TENSOR_FMA32_A_NUM_COLS) | (0x7 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0xb4 << TENSOR_FMA32_SCP_LOC_B) | (0xc9 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x0 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"LBL_SEQID_3_XMA_QNT_STR_SEQ_ITER_1_HID_4:\n"
			"li x5, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x2 << TENSOR_FMA16_B_NUM_COLS) | (0x2 << TENSOR_FMA16_A_NUM_ROWS) | (0xc << TENSOR_FMA16_A_NUM_COLS) | (0xc << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1d << TENSOR_FMA16_SCP_LOC_B) | (0x6f << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x0 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_4\n"
			"csrw tensor_error, x0\n"
			"li x5, 4\n" 
			"bne x30, x5, LBL_FAIL_HID_4\n"
			"nop\n"          
		       VSNIP_RSV     
		);
		trans();
		__asm__ __volatile__ ( 
			"nop\n"          
			"addi x30, x30, 1\n"
			"la x7, rand_int_32\n"
			"flw.ps f13, 512(x7)\n"
			"flw.ps f14, 896(x7)\n"
			"fdiv.pi f13, f13, f14\n" 
			"li x5, 0x3db2\n"
			"csrw tensor_mask, x5\n"	
			"la x20, rand_int_32\n"
			"flw.ps f25, 832(x20)\n"
			"fsatu8.pi f8, f25\n" 
			"LBL_SEQID_4_M0_WRITE_ID_8_HID_4:\n"
			"maskand m5, m3, m3\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x11, rand_ieee754_32\n"
			"flw.ps f28, 64(x11)\n"
			"fcvt.wu.s x25, f28, rdn\n" 
			"la x20, rand_int_32\n"
			"flw.ps f11, 512(x20)\n"
			"flw.ps f13, 576(x20)\n"
			"fmaxu.pi f28, f11, f13\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f26, 128(x11)\n"
			"fle.ps f3, f26, f26\n" 
			"la x12, rand_int_32\n"
			"flw.ps f26, 960(x12)\n"
			"flw.ps f25, 736(x12)\n"
			"fminu.pi f19, f26, f25\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f19, 896(x13)\n"
			"fclass.s x28, f19\n" 
			"la x7, rand_int_32\n"
			"flw.ps f16, 160(x7)\n"
			"flw.ps f5, 480(x7)\n"
			"fxor.pi f3, f16, f5\n" 
			"la x19, rand_int_32\n"
			"flw.ps f28, 0(x19)\n"
			"flw.ps f8, 896(x19)\n"
			"fminu.pi f26, f28, f8\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f0, 928(x19)\n"
			"flw.ps f11, 960(x19)\n"
			"flt.s x20, f0, f11\n" 
			"la x13, rand_int_32\n"
			"flw.ps f18, 736(x13)\n"
			"fle.pi f28, f18, f18\n" 
			"la x20, rand_ieee754_16\n"
			"flw.ps f27, 32(x20)\n"
			"fcvt.ps.f16 f14, f27\n" 
			"LBL_SEQID_4_M0_WRITE_ID_9_HID_4:\n"
			"flem.ps m5, f5, f0\n" 
			"li x5, CREDINC2\n"
			"ld x5, 0(x5)\n"
			"la x20, rand_ieee754_32\n"
			"flw.ps f13, 96(x20)\n"
			"flw.ps f28, 736(x20)\n"
			"fle.s x25, f13, f28\n" 
			"LBL_SEQID_4_FP_TRANS_ID_10_HID_4:\n"
			"la x19, rand_ieee754_32\n"
			"flw.ps f8, 768(x19)\n"
			"fexp.ps f19, f8\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_4_XMA_QNT_STR_SEQ_ITER_0_HID_4:\n"
			"li x6, ((0x4 << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0x4 << TENSOR_QUANT_QUANT_ROW) | (0x24 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x2 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_4\n"
			"csrw tensor_error, x0\n"
			"li x5, 5\n" 
			"bne x30, x5, LBL_FAIL_HID_4\n"
			"nop\n"          
		       VSNIP_RSV     
		);
		load_vpu_regs();
		__asm__ __volatile__ ( 
			"nop\n"          
			"addi x30, x30, 1\n"
			"LBL_SEQID_5_M0_WRITE_ID_11_HID_4:\n"
			"mov.m.x m4, x18, 0x2\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x15, rand_ieee754_32\n"
			"flw.ps f8, 448(x15)\n"
			"flw.ps f31, 672(x15)\n"
			"feq.ps f25, f8, f31\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f8, 352(x7)\n"
			"flw.ps f9, 832(x7)\n"
			"flw.ps f25, 832(x7)\n"
			"fnmsub.ps f27, f8, f9, f25, rdn\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f16, 640(x18)\n"
			"fsqrt.s f5, f16, rdn\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f13, 96(x17)\n"
			"flw.ps f9, 768(x17)\n"
			"fmul.ps f8, f13, f9, rdn\n" 
			"li x5, 0x5f5b\n"
			"csrw tensor_mask, x5\n"	
			"LBL_SEQID_5_M0_WRITE_ID_12_HID_4:\n"
			"fltm.ps m4, f16, f31\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_5_M0_WRITE_ID_13_HID_4:\n"
			"flem.ps m2, f26, f27\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x7, rand_int_32\n"
			"flw.ps f19, 288(x7)\n"
			"flw.ps f9, 128(x7)\n"
			"fminu.pi f11, f19, f9\n" 
			"la x13, rand_int_32\n"
			"flw.ps f31, 480(x13)\n"
			"fpackrepb.pi f19, f31\n" 
			"maskpopc x18, m5\n" 
			"LBL_SEQID_5_M0_WRITE_ID_14_HID_4:\n"
			"fsetm.pi m4, f27\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x19, rand_int_32\n"
			"flw.ps f9, 512(x19)\n"
			"flw.ps f18, 704(x19)\n"
			"for.pi f26, f9, f18\n" 
			"li x5, 0x655\n"
			"csrw tensor_mask, x5\n"	
			"LBL_SEQID_5_M0_WRITE_ID_15_HID_4:\n"
			"maskxor m5, m5, m3\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x18, rand_ieee754_32\n"
			"flw.ps f9, 480(x18)\n"
			"flw.ps f3, 192(x18)\n"
			"flw.ps f14, 352(x18)\n"
			"fmsub.s f28, f9, f3, f14, rup\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f31, 96(x25)\n"
			"flw.ps f13, 928(x25)\n"
			"flt.s x12, f31, f13\n" 
			"LBL_SEQID_5_M0_WRITE_ID_16_HID_4:\n"
			"fltm.ps m4, f28, f26\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_5_XMA_QNT_STR_SEQ_ITER_0_HID_4:\n"
			"li x5, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x2 << TENSOR_IMA_B_NUM_COLS) | (0x6 << TENSOR_IMA_A_NUM_ROWS) | (0xd << TENSOR_IMA_A_NUM_COLS) | (0x5 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x6 << TENSOR_IMA_SCP_LOC_B) | (0x74 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x6, ((0x17 << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0x6 << TENSOR_QUANT_QUANT_ROW) | (0x3b << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x1 << TENSOR_QUANT_TRANSF2) | (0x5 << TENSOR_QUANT_TRANSF1) | (0x3 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 7\n"
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_4\n"
			"csrw tensor_error, x0\n"
			"li x5, 6\n" 
			"bne x30, x5, LBL_FAIL_HID_4\n"
			"addi x30, x30, 1\n"
			"li x7, 0x25213d7f07602e15\n"
			"fcvt.s.wu f3, x7, rdn\n" 
			"la x12, rand_int_32\n"
			"flw.ps f31, 192(x12)\n"
			"flw.ps f16, 160(x12)\n"
			"fmin.pi f25, f31, f16\n" 
			"LBL_SEQID_6_M0_WRITE_ID_17_HID_4:\n"
			"maskxor m4, m4, m4\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"li x5, 0x47b7\n"
			"csrw tensor_mask, x5\n"	
			"maskpopcz x12, m2\n" 
			"la x7, rand_int_32\n"
			"flw.ps f0, 64(x7)\n"
			"flw.ps f3, 768(x7)\n"
			"for.pi f18, f0, f3\n" 
			"LBL_SEQID_6_M0_WRITE_ID_18_HID_4:\n"
			"feqm.ps m5, f25, f3\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x25, rand_int_32\n"
			"flw.ps f28, 32(x25)\n"
			"flw.ps f13, 576(x25)\n"
			"fmax.pi f19, f28, f13\n" 
			"la x11, rand_int_32\n"
			"flw.ps f18, 512(x11)\n"
			"flw.ps f9, 576(x11)\n"
			"fsll.pi f26, f18, f9\n" 
			"la x12, rand_int_32\n"
			"flw.ps f8, 128(x12)\n"
			"flw.ps f13, 416(x12)\n"
			"fmulhu.pi f28, f8, f13\n" 
			"fbci.pi f19, 0x6427\n" 
			"LBL_SEQID_6_M0_WRITE_ID_19_HID_4:\n"
			"maskor m3, m4, m3\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x17, rand_ieee754_32\n"
			"flw.ps f28, 160(x17)\n"
			"frsq.ps f19, f28\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x18, rand_ieee754_32\n"
			"flw.ps f5, 960(x18)\n"
			"fadd.ps f25, f5, f5, rmm\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f5, 416(x11)\n"
			"fswizz.ps f5, f5, 0x12\n" 
			"la x18, rand_int_32\n"
			"flw.ps f25, 832(x18)\n"
			"flw.ps f26, 480(x18)\n"
			"feq.pi f28, f25, f26\n" 
			"LBL_SEQID_6_FP_TRANS_ID_20_HID_4:\n"
			"la x11, rand_ieee754_32\n"
			"flw.ps f18, 512(x11)\n"
			"frcp.ps f9, f18\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_6_XMA_QNT_STR_SEQ_ITER_0_HID_4:\n"
			"li x5, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x2 << TENSOR_FMA16_B_NUM_COLS) | (0xf << TENSOR_FMA16_A_NUM_ROWS) | (0xa << TENSOR_FMA16_A_NUM_COLS) | (0x8 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x57 << TENSOR_FMA16_SCP_LOC_B) | (0xd3 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_4\n"
			"csrw tensor_error, x0\n"
			"li x5, 7\n" 
			"bne x30, x5, LBL_FAIL_HID_4\n"
			"nop \n"
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=1); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"nop \n"
			"addi x29, x29, -1\n"
			"bne x29, x0, LBL_OUTER_LOOP_SEQ_HID_4\n"
			"nop\n"          
		       VSNIP_RSV     
		);
		load_vpu_regs();
		__asm__ __volatile__ ( 
			"nop\n"          
			"nop\n"          
		       VSNIP_RSV     
		);
		trans();
		__asm__ __volatile__ ( 
			"nop\n"          
			"la x31, check_sum_hid_4\n"
			"lw x5, (x31)\n"
			"bne x30, x5, LBL_FAIL_HID_4\n"
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
			"csrrci x0, mcache_control, 2\n"
			"csrwi tensor_wait, 0x6\n" 
			"fence\n"
			"csrwi tensor_wait, 0x6\n" 
			"fence\n"
			"csrwi excl_mode, 0\n"
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=1); 
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
	if ( hid%16 == 5 ) { 
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
			"csrwi mhpmevent3, 11\n"   
			"csrwi mhpmevent4, 1\n"   
			"csrwi mhpmevent5, 6\n"   
			"csrwi mhpmevent6, 12\n"   
			"csrwi mhpmevent7, 23\n"   
			"csrwi mhpmevent8, 21\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f5,  736 (x5)\n"
			"flw.ps f21,  256 (x5)\n"
			"flw.ps f13,  448 (x5)\n"
			"flw.ps f18,  288 (x5)\n"
			"flw.ps f2,  800 (x5)\n"
			"flw.ps f20,  0 (x5)\n"
			"flw.ps f17,  224 (x5)\n"
			"flw.ps f26,  832 (x5)\n"
			"flw.ps f4,  640 (x5)\n"
			"flw.ps f27,  992 (x5)\n"
			"flw.ps f25,  864 (x5)\n"
			"flw.ps f3,  928 (x5)\n"
			"flw.ps f31,  896 (x5)\n"
			"flw.ps f7,  960 (x5)\n"
			"flw.ps f0,  384 (x5)\n"
			"flw.ps f15,  416 (x5)\n"
			"flw.ps f28,  704 (x5)\n"
			"flw.ps f16,  320 (x5)\n"
			"flw.ps f30,  672 (x5)\n"
			"flw.ps f10,  480 (x5)\n"
			"flw.ps f29,  64 (x5)\n"
			"flw.ps f1,  512 (x5)\n"
			"flw.ps f9,  608 (x5)\n"
			"flw.ps f8,  96 (x5)\n"
			"flw.ps f6,  192 (x5)\n"
			"flw.ps f12,  544 (x5)\n"
			"flw.ps f22,  128 (x5)\n"
			"flw.ps f14,  768 (x5)\n"
			"flw.ps f24,  576 (x5)\n"
			"flw.ps f11,  32 (x5)\n"
			"flw.ps f19,  352 (x5)\n"
			"flw.ps f23,  160 (x5)\n"
			"csrw fflags, zero\n"
			"csrwi frm, 0x0\n"
			"li x31, 100\n"
			"1:\n" 
			"addi x31, x31, -1\n"
			"bne x31, x0, 1b\n" 
			"csrw fcc, x0\n" 
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=1); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"LBL_KERNEL_HID_5:\n"          
			"li x5, 0xFEEDFEED\n"
			"slti x0, x0, 0x7f2\n"
			"la x31, check_sum_hid_5\n"
			"li x5, 0x7\n" 
			"sw x5, (x31)\n"
			"li x29, 0x7\n"
			"LBL_OUTER_LOOP_SEQ_HID_5:\n"
			"nop \n"
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=1); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"nop \n"
			"li x30, 0\n"
			"addi x30, x30, 1\n"
			"maskpopcz x7, m6\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f29, 928(x15)\n"
			"fcvt.w.s x13, f29, rtz\n" 
			"la x19, rand_int_32\n"
			"flw.ps f10, 768(x19)\n"
			"flw.ps f29, 64(x19)\n"
			"fmulhu.pi f14, f10, f29\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_5:\n"
			"fsetm.pi m6, f12\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x14, rand_ieee754_32\n"
			"flw.ps f2, 640(x14)\n"
			"flw.ps f26, 192(x14)\n"
			"fsgnjx.s f9, f2, f26\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_5:\n"
			"masknot m4, m1\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x13, rand_int_32\n"
			"flw.ps f0, 512(x13)\n"
			"flw.ps f14, 992(x13)\n"
			"fand.pi f14, f0, f14\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f6, 800(x6)\n"
			"flw.ps f26, 352(x6)\n"
			"fmul.s f28, f6, f26, rtz\n" 
			"la x6, rand_int_32\n"
			"flw.ps f6, 480(x6)\n"
			"fpackrepb.pi f12, f6\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f29, 352(x6)\n"
			"flw.ps f4, 896(x6)\n"
			"fsgnj.ps f6, f29, f4\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f12, 352(x19)\n"
			"flw.ps f11, 96(x19)\n"
			"fle.s x26, f12, f11\n" 
			"fbci.ps f9, 0x1520d\n" 
			"li x12, 0x36706e28b2ea212\n"
			"fcvt.s.l f30, x12, rne\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f6, 288(x19)\n"
			"fcvt.wu.s x15, f6, rtz\n" 
			"la x27, rand_int_32\n"
			"flw.ps f10, 480(x27)\n"
			"flw.ps f7, 960(x27)\n"
			"fsll.pi f26, f10, f7\n" 
			"li x14, 0x69c14ea8702223a2\n"
			"fcvt.s.w f27, x14, rmm\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_5\n"
			"addi x30, x30, 1\n"
			"la x15, rand_int_32\n"
			"flw.ps f2, 128(x15)\n"
			"flw.ps f11, 576(x15)\n"
			"fmul.pi f7, f2, f11\n" 
			"li x13, 0x8867f2f3340ade11\n"
			"fcvt.s.lu f7, x13, rdn\n" 
			"la x15, rand_int_32\n"
			"flw.ps f29, 0(x15)\n"
			"fcvt.ps.pwu f12, f29, rmm\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f11, 896(x27)\n"
			"flw.ps f27, 352(x27)\n"
			"fnmadd.s f0, f11, f11, f27, rne\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f30, 928(x15)\n"
			"flw.ps f11, 960(x15)\n"
			"flt.s x16, f30, f11\n" 
			"LBL_SEQID_1_M0_WRITE_ID_2_HID_5:\n"
			"mov.m.x m0, x6, 0x42\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x16, rand_ieee754_32\n"
			"flw.ps f28, 416(x16)\n"
			"flw.ps f4, 672(x16)\n"
			"flw.ps f9, 384(x16)\n"
			"fcmov.ps f30, f28, f4, f9\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f7, 640(x7)\n"
			"flw.ps f10, 0(x7)\n"
			"feq.ps f12, f7, f10\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f26, 608(x15)\n"
			"flw.ps f11, 0(x15)\n"
			"fadd.ps f29, f26, f11, rup\n" 
			"mova.x.m x6\n" 
			"la x16, rand_int_32\n"
			"flw.ps f0, 352(x16)\n"
			"flw.ps f9, 896(x16)\n"
			"fminu.pi f9, f0, f9\n" 
			"la x27, rand_int_32\n"
			"flw.ps f11, 928(x27)\n"
			"faddi.pi f10, f11, 0x18\n" 
			"mova.x.m x15\n" 
			"la x17, rand_int_32\n"
			"flw.ps f28, 320(x17)\n"
			"flw.ps f6, 576(x17)\n"
			"fmulh.pi f27, f28, f6\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f11, 480(x26)\n"
			"fcvt.pw.ps f0, f11, rup\n" 
			"li x16, 0x92934150867b9843\n"
			"fcvt.s.w f29, x16, rdn\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_5\n"
			"addi x30, x30, 1\n"
			"la x27, rand_int_32\n"
			"flw.ps f27, 64(x27)\n"
			"flw.ps f29, 832(x27)\n"
			"fadd.pi f2, f27, f29\n" 
			"la x6, rand_int_32\n"
			"flw.ps f1, 864(x6)\n"
			"flw.ps f6, 672(x6)\n"
			"fmaxu.pi f1, f1, f6\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f1, 960(x7)\n"
			"flw.ps f26, 512(x7)\n"
			"fnmsub.s f2, f1, f1, f26, rup\n" 
			"LBL_SEQID_2_M0_WRITE_ID_3_HID_5:\n"
			"mov.m.x m4, x14, 0x60\n" 
			"li x5, 0x80340190\n"
			"ld x5, 0(x5)\n"
			"la x16, rand_ieee754_32\n"
			"flw.ps f14, 416(x16)\n"
			"flw.ps f4, 960(x16)\n"
			"fle.ps f26, f14, f4\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f11, 832(x6)\n"
			"flw.ps f2, 192(x6)\n"
			"flw.ps f29, 480(x6)\n"
			"fnmadd.ps f28, f11, f2, f29, rdn\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f2, 640(x7)\n"
			"flw.ps f4, 800(x7)\n"
			"fadd.ps f1, f2, f4, rne\n" 
			"la x6, rand_int_32\n"
			"flw.ps f26, 992(x6)\n"
			"fpackreph.pi f10, f26\n" 
			"LBL_SEQID_2_FP_TRANS_ID_4_HID_5:\n"
			"la x6, rand_ieee754_32\n"
			"flw.ps f26, 512(x6)\n"
			"flog.ps f6, f26\n" 
			"la x26, rand_int_32\n"
			"flw.ps f0, 320(x26)\n"
			"flw.ps f11, 224(x26)\n"
			"fmul.pi f4, f0, f11\n" 
			"la x27, rand_int_32\n"
			"flw.ps f30, 0(x27)\n"
			"flw.ps f4, 64(x27)\n"
			"fle.pi f1, f30, f4\n" 
			"LBL_SEQID_2_M0_WRITE_ID_5_HID_5:\n"
			"maskand m6, m1, m4\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x12, rand_ieee754_32\n"
			"flw.ps f26, 0(x12)\n"
			"flw.ps f9, 256(x12)\n"
			"fle.s x17, f26, f9\n" 
			"li x26, 0xd94860a22b2cab10\n"
			"fcvt.s.wu f6, x26, rtz\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f10, 384(x16)\n"
			"flw.ps f26, 416(x16)\n"
			"fmul.ps f12, f10, f26, rdn\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f30, 864(x13)\n"
			"flw.ps f2, 736(x13)\n"
			"fadd.s f4, f30, f2, rup\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 3\n" 
			"bne x30, x5, LBL_FAIL_HID_5\n"
			"nop\n"          
		       VSNIP_RSV     
		);
		load_vpu_regs();
		__asm__ __volatile__ ( 
			"nop\n"          
			"addi x30, x30, 1\n"
			"la x15, rand_ieee754_32\n"
			"flw.ps f1, 448(x15)\n"
			"flw.ps f28, 288(x15)\n"
			"fsgnjx.ps f9, f1, f28\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f10, 480(x13)\n"
			"flw.ps f14, 64(x13)\n"
			"flw.ps f30, 192(x13)\n"
			"fmsub.s f2, f10, f14, f30, rdn\n" 
			"la x14, rand_int_32\n"
			"flw.ps f0, 736(x14)\n"
			"flw.ps f7, 992(x14)\n"
			"fxor.pi f1, f0, f7\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f14, 608(x26)\n"
			"flw.ps f4, 320(x26)\n"
			"feq.s x6, f14, f4\n" 
			"la x13, rand_int_32\n"
			"flw.ps f6, 576(x13)\n"
			"fslli.pi f11, f6, 0x2\n" 
			"la x17, rand_int_32\n"
			"flw.ps f4, 768(x17)\n"
			"fnot.pi f30, f4\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f6, 928(x12)\n"
			"ffrc.ps f1, f6\n" 
			"LBL_SEQID_3_M0_WRITE_ID_6_HID_5:\n"
			"maskxor m4, m4, m6\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x27, rand_int_32\n"
			"flw.ps f6, 832(x27)\n"
			"flw.ps f14, 448(x27)\n"
			"fsra.pi f29, f6, f14\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f7, 160(x17)\n"
			"flw.ps f12, 704(x17)\n"
			"fadd.ps f28, f7, f12, rmm\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f11, 128(x16)\n"
			"flw.ps f4, 576(x16)\n"
			"fmin.s f26, f11, f4\n" 
			"la x15, rand_int_32\n"
			"flw.ps f4, 768(x15)\n"
			"flw.ps f11, 576(x15)\n"
			"fminu.pi f2, f4, f11\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f10, 608(x6)\n"
			"fsqrt.ps f26, f10\n" 
			"la x15, rand_int_32\n"
			"flw.ps f28, 256(x15)\n"
			"flw.ps f1, 96(x15)\n"
			"for.pi f29, f28, f1\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f4, 256(x17)\n"
			"fcvt.f16.ps f27, f4\n" 
			"la x27, rand_int_32\n"
			"flw.ps f12, 384(x27)\n"
			"flw.ps f26, 672(x27)\n"
			"feq.pi f0, f12, f26\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 4\n" 
			"bne x30, x5, LBL_FAIL_HID_5\n"
			"nop\n"          
		       VSNIP_RSV     
		);
		trans();
		__asm__ __volatile__ ( 
			"nop\n"          
			"addi x30, x30, 1\n"
			"maskpopc x27, m6\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f9, 704(x13)\n"
			"fcvt.pw.ps f7, f9, rne\n" 
			"maskpopc x19, m0\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f10, 384(x12)\n"
			"flw.ps f9, 640(x12)\n"
			"flw.ps f29, 352(x12)\n"
			"fcmov.ps f0, f10, f9, f29\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f10, 800(x26)\n"
			"fclass.s x27, f10\n" 
			"fbci.pi f1, 0x8862\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f11, 96(x19)\n"
			"flw.ps f10, 384(x19)\n"
			"flw.ps f27, 192(x19)\n"
			"fcmov.ps f14, f11, f10, f27\n" 
			"fbci.pi f12, 0x3257c\n" 
			"la x14, rand_int_32\n"
			"flw.ps f6, 256(x14)\n"
			"fcvt.ps.pw f1, f6, rdn\n" 
			"LBL_SEQID_4_FP_TRANS_ID_7_HID_5:\n"
			"la x14, rand_ieee754_32\n"
			"flw.ps f14, 576(x14)\n"
			"fexp.ps f29, f14\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f4, 320(x27)\n"
			"flw.ps f26, 768(x27)\n"
			"fle.s x7, f4, f26\n" 
			"la x26, rand_int_32\n"
			"flw.ps f10, 160(x26)\n"
			"flw.ps f27, 480(x26)\n"
			"fsra.pi f29, f10, f27\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f12, 896(x16)\n"
			"fmin.ps f4, f12, f12\n" 
			"la x12, rand_int_32\n"
			"flw.ps f6, 832(x12)\n"
			"fpackreph.pi f6, f6\n" 
			"la x7, rand_int_32\n"
			"flw.ps f28, 864(x7)\n"
			"flw.ps f2, 640(x7)\n"
			"fmaxu.pi f1, f28, f2\n" 
			"LBL_SEQID_4_M0_WRITE_ID_8_HID_5:\n"
			"flem.ps m1, f29, f7\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"csrw tensor_error, zero\n" 
			"li x5, 5\n" 
			"bne x30, x5, LBL_FAIL_HID_5\n"
			"nop\n"          
		       VSNIP_RSV     
		);
		load_vpu_regs();
		__asm__ __volatile__ ( 
			"nop\n"          
			"addi x30, x30, 1\n"
			"la x16, rand_ieee754_32\n"
			"flw.ps f26, 608(x16)\n"
			"fclass.ps f6, f26\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f1, 96(x26)\n"
			"flw.ps f6, 928(x26)\n"
			"fsgnjx.s f6, f1, f6\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f9, 416(x26)\n"
			"flw.ps f29, 320(x26)\n"
			"fadd.s f14, f9, f29, rmm\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f28, 0(x17)\n"
			"fcvt.f16.ps f12, f28\n" 
			"la x7, rand_int_32\n"
			"flw.ps f4, 832(x7)\n"
			"flw.ps f7, 896(x7)\n"
			"for.pi f11, f4, f7\n" 
			"la x26, rand_int_32\n"
			"flw.ps f6, 0(x26)\n"
			"fpackreph.pi f10, f6\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f29, 192(x13)\n"
			"flw.ps f10, 224(x13)\n"
			"flw.ps f26, 896(x13)\n"
			"fcmov.ps f4, f29, f10, f26\n" 
			"la x7, rand_int_32\n"
			"flw.ps f14, 192(x7)\n"
			"faddi.pi f0, f14, 0x30\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f7, 448(x12)\n"
			"fclass.ps f14, f7\n" 
			"la x12, rand_int_32\n"
			"flw.ps f0, 0(x12)\n"
			"fpackrepb.pi f10, f0\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f7, 928(x13)\n"
			"fround.ps f2, f7, rmm\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f26, 192(x17)\n"
			"flw.ps f14, 736(x17)\n"
			"flw.ps f4, 928(x17)\n"
			"fmsub.ps f2, f26, f14, f4, rmm\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f4, 320(x17)\n"
			"flw.ps f9, 832(x17)\n"
			"fsgnj.s f6, f4, f9\n" 
			"la x7, rand_int_32\n"
			"flw.ps f9, 960(x7)\n"
			"fsatu8.pi f1, f9\n" 
			"LBL_SEQID_5_M0_WRITE_ID_9_HID_5:\n"
			"feqm.ps m4, f30, f1\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x6, rand_ieee754_32\n"
			"flw.ps f29, 448(x6)\n"
			"fclass.s x12, f29\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 6\n" 
			"bne x30, x5, LBL_FAIL_HID_5\n"
			"addi x30, x30, 1\n"
			"la x17, rand_int_32\n"
			"flw.ps f11, 384(x17)\n"
			"fcvt.ps.pwu f27, f11, rmm\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f2, 960(x6)\n"
			"flw.ps f4, 384(x6)\n"
			"feq.s x26, f2, f4\n" 
			"LBL_SEQID_6_M0_WRITE_ID_10_HID_5:\n"
			"fsetm.pi m1, f10\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x19, rand_int_32\n"
			"flw.ps f30, 288(x19)\n"
			"fsatu8.pi f4, f30\n" 
			"la x12, rand_int_32\n"
			"flw.ps f9, 416(x12)\n"
			"flw.ps f30, 352(x12)\n"
			"fmin.pi f11, f9, f30\n" 
			"la x17, rand_int_32\n"
			"flw.ps f2, 416(x17)\n"
			"fcvt.ps.pwu f30, f2, rtz\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f7, 640(x12)\n"
			"flw.ps f9, 128(x12)\n"
			"fmin.ps f30, f7, f9\n" 
			"la x7, rand_int_32\n"
			"flw.ps f4, 864(x7)\n"
			"flw.ps f26, 576(x7)\n"
			"fxor.pi f1, f4, f26\n" 
			"la x13, rand_int_32\n"
			"flw.ps f9, 224(x13)\n"
			"flw.ps f14, 608(x13)\n"
			"fmul.pi f1, f9, f14\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f11, 864(x26)\n"
			"fcvt.pw.ps f10, f11, rdn\n" 
			"la x16, rand_int_32\n"
			"flw.ps f0, 704(x16)\n"
			"flw.ps f30, 96(x16)\n"
			"fminu.pi f4, f0, f30\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f9, 608(x27)\n"
			"flw.ps f29, 256(x27)\n"
			"fadd.ps f0, f9, f29, rdn\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f12, 128(x13)\n"
			"flw.ps f30, 544(x13)\n"
			"fmadd.s f14, f12, f30, f12, rtz\n" 
			"fbci.ps f1, 0x6aae1\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f9, 928(x13)\n"
			"flw.ps f27, 288(x13)\n"
			"flw.ps f2, 704(x13)\n"
			"fnmsub.ps f4, f9, f27, f2, rne\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f12, 544(x13)\n"
			"fcvt.w.s x12, f12, rtz\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 7\n" 
			"bne x30, x5, LBL_FAIL_HID_5\n"
			"nop \n"
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=1); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"nop \n"
			"addi x29, x29, -1\n"
			"bne x29, x0, LBL_OUTER_LOOP_SEQ_HID_5\n"
			"nop\n"          
		       VSNIP_RSV     
		);
		load_vpu_regs();
		__asm__ __volatile__ ( 
			"nop\n"          
			"nop\n"          
		       VSNIP_RSV     
		);
		trans();
		__asm__ __volatile__ ( 
			"nop\n"          
			"la x31, check_sum_hid_5\n"
			"lw x5, (x31)\n"
			"bne x30, x5, LBL_FAIL_HID_5\n"
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
		c_sync_point(sync_odd_harts=1); 
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
	if ( hid%16 == 6 ) { 
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
			"csrwi mhpmevent3, 16\n"   
			"csrwi mhpmevent4, 6\n"   
			"csrwi mhpmevent5, 4\n"   
			"csrwi mhpmevent6, 27\n"   
			"csrwi mhpmevent7, 6\n"   
			"csrwi mhpmevent8, 3\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f28,  672 (x5)\n"
			"flw.ps f19,  768 (x5)\n"
			"flw.ps f30,  32 (x5)\n"
			"flw.ps f22,  64 (x5)\n"
			"flw.ps f29,  352 (x5)\n"
			"flw.ps f17,  928 (x5)\n"
			"flw.ps f5,  224 (x5)\n"
			"flw.ps f1,  640 (x5)\n"
			"flw.ps f13,  960 (x5)\n"
			"flw.ps f15,  864 (x5)\n"
			"flw.ps f4,  832 (x5)\n"
			"flw.ps f7,  256 (x5)\n"
			"flw.ps f27,  448 (x5)\n"
			"flw.ps f0,  896 (x5)\n"
			"flw.ps f6,  608 (x5)\n"
			"flw.ps f23,  0 (x5)\n"
			"flw.ps f11,  544 (x5)\n"
			"flw.ps f26,  128 (x5)\n"
			"flw.ps f9,  192 (x5)\n"
			"flw.ps f2,  288 (x5)\n"
			"flw.ps f18,  96 (x5)\n"
			"flw.ps f24,  992 (x5)\n"
			"flw.ps f8,  320 (x5)\n"
			"flw.ps f14,  704 (x5)\n"
			"flw.ps f21,  512 (x5)\n"
			"flw.ps f12,  800 (x5)\n"
			"flw.ps f16,  160 (x5)\n"
			"flw.ps f10,  416 (x5)\n"
			"flw.ps f31,  576 (x5)\n"
			"flw.ps f3,  384 (x5)\n"
			"flw.ps f25,  480 (x5)\n"
			"flw.ps f20,  736 (x5)\n"
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
			"csrw fcc, x0\n" 
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=1); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"LBL_KERNEL_HID_6:\n"          
			"li x5, 0xFEEDFEED\n"
			"slti x0, x0, 0x7f2\n"
			"la x31, check_sum_hid_6\n"
			"li x5, 0x7\n" 
			"sw x5, (x31)\n"
			"li x29, 0x7\n"
			"LBL_OUTER_LOOP_SEQ_HID_6:\n"
			"nop \n"
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=1); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"nop \n"
			"li x30, 0\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_6:\n"
			"masknot m4, m4\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x15, rand_ieee754_32\n"
			"flw.ps f8, 320(x15)\n"
			"fcvt.f16.ps f21, f8\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f29, 192(x19)\n"
			"flw.ps f18, 832(x19)\n"
			"fle.ps f9, f29, f18\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f5, 544(x10)\n"
			"flw.ps f21, 736(x10)\n"
			"fnmadd.ps f27, f5, f5, f21, rtz\n" 
			"li x17, 0xaabe0eebfc99557f\n"
			"fcvt.s.w f3, x17, rtz\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f20, 864(x22)\n"
			"flw.ps f5, 64(x22)\n"
			"flw.ps f9, 768(x22)\n"
			"fnmadd.s f5, f20, f5, f9, rdn\n" 
			"la x11, rand_int_32\n"
			"flw.ps f3, 800(x11)\n"
			"flw.ps f5, 320(x11)\n"
			"fxor.pi f26, f3, f5\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f26, 416(x15)\n"
			"fle.ps f8, f26, f26\n" 
			"la x10, rand_int_32\n"
			"flw.ps f12, 352(x10)\n"
			"fpackrepb.pi f20, f12\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f3, 576(x19)\n"
			"flw.ps f21, 928(x19)\n"
			"fmin.ps f27, f3, f21\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x17, rand_ieee754_32\n"
			"flw.ps f20, 192(x17)\n"
			"flw.ps f3, 928(x17)\n"
			"flw.ps f10, 800(x17)\n"
			"fmadd.ps f21, f20, f3, f10, rdn\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_6:\n"
			"maskxor m5, m1, m5\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x19, rand_int_32\n"
			"flw.ps f21, 352(x19)\n"
			"flw.ps f19, 640(x19)\n"
			"flt.pi f10, f21, f19\n" 
			"la x10, rand_int_32\n"
			"flw.ps f21, 768(x10)\n"
			"flw.ps f3, 384(x10)\n"
			"fmin.pi f26, f21, f3\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f14, 512(x10)\n"
			"fround.ps f21, f14, rdn\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f3, 160(x9)\n"
			"fcmovm.ps f11, f3, f3\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ID_156000_HID_6:\n"
			"li x5, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x1 << TENSOR_FMA16_B_NUM_COLS) | (0xb << TENSOR_FMA16_A_NUM_ROWS) | (0x9 << TENSOR_FMA16_A_NUM_COLS) | (0xb << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x5f << TENSOR_FMA16_SCP_LOC_B) | (0x24 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x0 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ID_116928_HID_6:\n"
			"li x5, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x0 << TENSOR_FMA32_B_NUM_COLS) | (0x8 << TENSOR_FMA32_A_NUM_ROWS) | (0x7 << TENSOR_FMA32_A_NUM_COLS) | (0x3 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0xb3 << TENSOR_FMA32_SCP_LOC_B) | (0xf << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_6\n"
			"csrw tensor_error, x0\n"
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=0); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ITER_0_HID_6:\n"
			"li x6, ((0xa << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0xc << TENSOR_QUANT_QUANT_ROW) | (0x1e << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x7 << TENSOR_QUANT_TRANSF6) | (0x6 << TENSOR_QUANT_TRANSF5) | (0x7 << TENSOR_QUANT_TRANSF4) | (0x7 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ITER_1_HID_6:\n"
			"li x6, ((0x1 << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0x2 << TENSOR_QUANT_QUANT_ROW) | (0x20 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_6\n"
			"csrw tensor_error, x0\n"
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_6\n"
			"addi x30, x30, 1\n"
			"la x15, rand_int_32\n"
			"flw.ps f9, 224(x15)\n"
			"fcvt.ps.pwu f19, f9, rup\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f18, 224(x15)\n"
			"flw.ps f9, 736(x15)\n"
			"fsgnjn.ps f18, f18, f9\n" 
			"LBL_SEQID_1_M0_WRITE_ID_2_HID_6:\n"
			"feqm.ps m1, f20, f30\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x15, rand_ieee754_32\n"
			"flw.ps f12, 192(x15)\n"
			"flw.ps f9, 256(x15)\n"
			"flw.ps f26, 448(x15)\n"
			"fnmadd.s f11, f12, f9, f26, rdn\n" 
			"la x19, rand_int_32\n"
			"flw.ps f5, 512(x19)\n"
			"flw.ps f20, 768(x19)\n"
			"fminu.pi f20, f5, f20\n" 
			"la x19, rand_int_32\n"
			"flw.ps f26, 608(x19)\n"
			"flw.ps f27, 736(x19)\n"
			"feq.pi f26, f26, f27\n" 
			"li x23, 0xb0c989d0ff400d5a\n"
			"fcvt.s.lu f3, x23, rtz\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f12, 928(x11)\n"
			"flw.ps f26, 896(x11)\n"
			"fmin.ps f19, f12, f26\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f29, 896(x20)\n"
			"flw.ps f14, 960(x20)\n"
			"fsgnj.ps f19, f29, f14\n" 
			"la x22, rand_int_32\n"
			"flw.ps f8, 384(x22)\n"
			"flw.ps f3, 736(x22)\n"
			"fdivu.pi f12, f8, f3\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f18, 928(x11)\n"
			"fadd.ps f8, f18, f18, rtz\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x23, rand_ieee754_32\n"
			"flw.ps f8, 896(x23)\n"
			"flw.ps f21, 160(x23)\n"
			"fsgnjn.ps f3, f8, f21\n" 
			"la x17, rand_int_32\n"
			"flw.ps f27, 544(x17)\n"
			"flw.ps f14, 928(x17)\n"
			"feq.pi f21, f27, f14\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f27, 576(x19)\n"
			"fcvt.f16.ps f18, f27\n" 
			"LBL_SEQID_1_FP_TRANS_ID_3_HID_6:\n"
			"la x11, rand_ieee754_32\n"
			"flw.ps f26, 768(x11)\n"
			"frcp.ps f19, f26\n" 
			"la x20, rand_int_32\n"
			"flw.ps f19, 192(x20)\n"
			"flw.ps f12, 608(x20)\n"
			"fdivu.pi f29, f19, f12\n" 
			"csrw tensor_error, zero\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"LBL_SEQID_1_TLD_XMA_STR_SEQ_ID_972246_HID_6:\n"
			"la x31, self_check_16\n"
			"li x5, 0xc0000000000005\n"
			"add x5, x5, x31\n"
			"li x6, 0x3c000000000000b\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x2 << TENSOR_FMA16_B_NUM_COLS) | (0x5 << TENSOR_FMA16_A_NUM_ROWS) | (0xb << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1e << TENSOR_FMA16_SCP_LOC_B) | (0x6 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0x18\n"
			"bne x5, x31, LBL_FAIL_HID_6\n"
			"LBL_SEQID_1_TLD_XMA_STR_SEQ_ID_600526_HID_6:\n"
			"la x31, self_check_8\n"
			"li x5, 0x40000000000007\n"
			"add x5, x5, x31\n"
			"li x6, 0x320000000000008\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x3 << TENSOR_IMA_B_NUM_COLS) | (0x7 << TENSOR_IMA_A_NUM_ROWS) | (0x8 << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x19 << TENSOR_IMA_SCP_LOC_B) | (0x2 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x9, ((0x4 << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0x7 << TENSOR_QUANT_QUANT_ROW) | (0x1c << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x4 << TENSOR_QUANT_TRANSF3) | (0x5 << TENSOR_QUANT_TRANSF2) | (0x3 << TENSOR_QUANT_TRANSF1) | (0x9 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fmvz.x.ps x31, f0, 0\n"
			"li x5, 0x24\n"
			"bne x5, x31, LBL_FAIL_HID_6\n"
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 10\n"
			"LBL_SEQID_1_TLD_XMA_STR_SEQ_ID_361446_HID_6:\n"
			"la x31, self_check_16\n"
			"li x5, 0x8000000000000e\n"
			"add x5, x5, x31\n"
			"li x6, 0x1000000000000c\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x2 << TENSOR_FMA16_B_NUM_COLS) | (0xe << TENSOR_FMA16_A_NUM_ROWS) | (0xc << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x1 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1b << TENSOR_FMA16_SCP_LOC_B) | (0x4 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x5 << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0xe << TENSOR_QUANT_QUANT_ROW) | (0x3f << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0x1a\n"
			"bne x5, x31, LBL_FAIL_HID_6\n"
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_6\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_1_RDX_ID_1_SHIRE_0_HID_6:\n"
			"li x22, 0xa0000000010000a\n"
			"csrw tensor_reduce, x22\n" 
			"csrwi tensor_wait, 9\n" 
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=0); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"LBL_SEQID_1_TLD_XMA_STR_SEQ_ITER_0_HID_6:\n"
			"la x31, self_check_16\n"
			"li x5, 0x4000000000000f\n"
			"add x5, x5, x31\n"
			"li x6, 0x3e000000000000d\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x2 << TENSOR_FMA16_B_NUM_COLS) | (0xf << TENSOR_FMA16_A_NUM_ROWS) | (0xd << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1f << TENSOR_FMA16_SCP_LOC_B) | (0x2 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0x1c\n"
			"bne x5, x31, LBL_FAIL_HID_6\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_6\n"
			"csrw tensor_error, x0\n"
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_6\n"
			"addi x30, x30, 1\n"
			"la x15, rand_int_32\n"
			"flw.ps f26, 576(x15)\n"
			"flw.ps f8, 864(x15)\n"
			"fsra.pi f11, f26, f8\n" 
			"la x19, rand_int_32\n"
			"flw.ps f29, 608(x19)\n"
			"flw.ps f9, 480(x19)\n"
			"flt.pi f21, f29, f9\n" 
			"la x23, rand_int_32\n"
			"flw.ps f10, 160(x23)\n"
			"flw.ps f11, 928(x23)\n"
			"fremu.pi f14, f10, f11\n" 
			"la x17, rand_int_32\n"
			"flw.ps f11, 672(x17)\n"
			"flw.ps f9, 160(x17)\n"
			"fadd.pi f5, f11, f9\n" 
			"LBL_SEQID_2_M0_WRITE_ID_4_HID_6:\n"
			"maskor m1, m5, m6\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x18, rand_ieee754_32\n"
			"flw.ps f26, 448(x18)\n"
			"flw.ps f21, 896(x18)\n"
			"fmax.s f27, f26, f21\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"LBL_SEQID_2_FP_TRANS_ID_5_HID_6:\n"
			"la x19, rand_ieee754_32\n"
			"flw.ps f27, 896(x19)\n"
			"fexp.ps f29, f27\n" 
			"la x22, rand_int_32\n"
			"flw.ps f29, 928(x22)\n"
			"flw.ps f14, 832(x22)\n"
			"fmulh.pi f18, f29, f14\n" 
			"la x19, rand_int_32\n"
			"flw.ps f12, 576(x19)\n"
			"flw.ps f10, 608(x19)\n"
			"fminu.pi f21, f12, f10\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f18, 800(x11)\n"
			"fsin.ps f14, f18\n" 
			"li x11, 0x6d9da2212ad17e91\n"
			"fcvt.s.w f14, x11, rup\n" 
			"mova.x.m x11\n" 
			"la x18, rand_int_32\n"
			"flw.ps f9, 640(x18)\n"
			"flw.ps f18, 192(x18)\n"
			"fand.pi f10, f9, f18\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f9, 640(x19)\n"
			"frsq.ps f21, f9\n" 
			"LBL_SEQID_2_M0_WRITE_ID_6_HID_6:\n"
			"maskxor m6, m5, m1\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x10, rand_int_32\n"
			"flw.ps f9, 576(x10)\n"
			"flw.ps f21, 416(x10)\n"
			"fmul.pi f12, f9, f21\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_2_XMA_QNT_STR_SEQ_ITER_0_HID_6:\n"
			"li x5, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x3 << TENSOR_IMA_B_NUM_COLS) | (0xc << TENSOR_IMA_A_NUM_ROWS) | (0x5 << TENSOR_IMA_A_NUM_COLS) | (0x8 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x2e << TENSOR_IMA_SCP_LOC_B) | (0x43 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x0 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_6\n"
			"csrw tensor_error, x0\n"
			"li x5, 3\n" 
			"bne x30, x5, LBL_FAIL_HID_6\n"
			"nop\n"          
		       VSNIP_RSV     
		);
		load_vpu_regs();
		__asm__ __volatile__ ( 
			"nop\n"          
			"addi x30, x30, 1\n"
			"la x18, rand_ieee754_32\n"
			"flw.ps f11, 352(x18)\n"
			"flw.ps f18, 224(x18)\n"
			"feq.ps f12, f11, f18\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f14, 64(x11)\n"
			"flw.ps f9, 512(x11)\n"
			"fsgnj.s f21, f14, f9\n" 
			"la x20, rand_int_32\n"
			"flw.ps f21, 768(x20)\n"
			"fxor.pi f11, f21, f21\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f26, 224(x11)\n"
			"flw.ps f3, 768(x11)\n"
			"fadd.ps f26, f26, f3, rne\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f11, 32(x10)\n"
			"flw.ps f12, 288(x10)\n"
			"fmul.s f30, f11, f12, rne\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f12, 64(x23)\n"
			"flw.ps f5, 480(x23)\n"
			"fmin.ps f8, f12, f5\n" 
			"la x11, rand_int_32\n"
			"flw.ps f12, 928(x11)\n"
			"fnot.pi f18, f12\n" 
			"LBL_SEQID_3_M0_WRITE_ID_7_HID_6:\n"
			"masknot m6, m5\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_3_FP_TRANS_ID_8_HID_6:\n"
			"la x23, rand_ieee754_32\n"
			"flw.ps f19, 480(x23)\n"
			"frcp.ps f19, f19\n" 
			"la x23, rand_int_32\n"
			"flw.ps f10, 672(x23)\n"
			"flw.ps f30, 160(x23)\n"
			"fsll.pi f19, f10, f30\n" 
			"la x17, rand_int_32\n"
			"flw.ps f30, 928(x17)\n"
			"flw.ps f3, 64(x17)\n"
			"fxor.pi f21, f30, f3\n" 
			"la x9, rand_int_32\n"
			"flw.ps f11, 480(x9)\n"
			"flw.ps f27, 448(x9)\n"
			"fle.pi f3, f11, f27\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f26, 64(x23)\n"
			"fcvt.w.s x20, f26, rmm\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f26, 128(x23)\n"
			"fcvt.f16.ps f12, f26\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f21, 800(x18)\n"
			"flw.ps f11, 576(x18)\n"
			"fmax.ps f14, f21, f11\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x17, rand_ieee754_32\n"
			"flw.ps f20, 768(x17)\n"
			"flw.ps f27, 192(x17)\n"
			"feq.ps f10, f20, f27\n" 
			"csrw tensor_error, zero\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"LBL_SEQID_3_XMA_QNT_STR_SEQ_ITER_0_HID_6:\n"
			"li x5, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x3 << TENSOR_IMA_B_NUM_COLS) | (0x7 << TENSOR_IMA_A_NUM_ROWS) | (0xd << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0xef << TENSOR_IMA_SCP_LOC_B) | (0xd4 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x0 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_6\n"
			"csrw tensor_error, x0\n"
			"li x5, 4\n" 
			"bne x30, x5, LBL_FAIL_HID_6\n"
			"nop\n"          
		       VSNIP_RSV     
		);
		trans();
		__asm__ __volatile__ ( 
			"nop\n"          
			"addi x30, x30, 1\n"
			"la x11, rand_ieee754_32\n"
			"flw.ps f19, 64(x11)\n"
			"flw.ps f14, 672(x11)\n"
			"fsgnjx.s f9, f19, f14\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f8, 992(x23)\n"
			"flw.ps f19, 544(x23)\n"
			"fsgnj.s f8, f8, f19\n" 
			"la x22, rand_int_32\n"
			"flw.ps f8, 256(x22)\n"
			"flw.ps f10, 512(x22)\n"
			"fsra.pi f10, f8, f10\n" 
			"maskpopcz x15, m5\n" 
			"la x11, rand_int_32\n"
			"flw.ps f20, 480(x11)\n"
			"fcvt.ps.pw f8, f20, rdn\n" 
			"la x9, rand_int_32\n"
			"flw.ps f18, 960(x9)\n"
			"flw.ps f14, 128(x9)\n"
			"fmaxu.pi f19, f18, f14\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f30, 736(x11)\n"
			"fcvt.wu.s x15, f30, rtz\n" 
			"li x5, 0xe587e3ccc82ff368\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0x200c275c0b1ba314\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"la x10, rand_int_32\n"
			"flw.ps f27, 160(x10)\n"
			"fsat8.pi f5, f27\n" 
			"la x17, rand_int_32\n"
			"flw.ps f21, 992(x17)\n"
			"fcvt.ps.pw f20, f21, rup\n" 
			"mova.x.m x18\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f10, 576(x19)\n"
			"flw.ps f27, 672(x19)\n"
			"flw.ps f19, 256(x19)\n"
			"fmsub.ps f8, f10, f27, f19, rmm\n" 
			"la x15, rand_int_32\n"
			"flw.ps f27, 416(x15)\n"
			"flw.ps f21, 320(x15)\n"
			"fand.pi f10, f27, f21\n" 
			"li x5, 0x1b00e99f26b898ef\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0xeb4ccbc516a49ab0\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"la x23, rand_ieee754_32\n"
			"flw.ps f30, 640(x23)\n"
			"flw.ps f12, 768(x23)\n"
			"feq.s x20, f30, f12\n" 
			"la x19, rand_int_32\n"
			"flw.ps f5, 768(x19)\n"
			"fnot.pi f10, f5\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f20, 544(x17)\n"
			"flw.ps f18, 96(x17)\n"
			"fmax.s f11, f20, f18\n" 
			"la x23, rand_int_32\n"
			"flw.ps f26, 32(x23)\n"
			"flw.ps f20, 640(x23)\n"
			"fxor.pi f27, f26, f20\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_4_TLD_XMA_STR_SEQ_ITER_0_HID_6:\n"
			"la x31, self_check_8\n"
			"li x5, 0xa0000000000000\n"
			"add x5, x5, x31\n"
			"li x6, 0x10000000000001\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x3 << TENSOR_IMA_B_NUM_COLS) | (0x0 << TENSOR_IMA_A_NUM_ROWS) | (0x1 << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x1 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1f << TENSOR_IMA_SCP_LOC_B) | (0x5 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x9, ((0x7 << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0x0 << TENSOR_QUANT_QUANT_ROW) | (0x30 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x1 << TENSOR_QUANT_TRANSF6) | (0x8 << TENSOR_QUANT_TRANSF5) | (0x4 << TENSOR_QUANT_TRANSF4) | (0x8 << TENSOR_QUANT_TRANSF3) | (0xa << TENSOR_QUANT_TRANSF2) | (0xa << TENSOR_QUANT_TRANSF1) | (0x9 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fmvz.x.ps x31, f0, 0\n"
			"li x5, 0x8\n"
			"bne x5, x31, LBL_FAIL_HID_6\n"
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_6\n"
			"csrw tensor_error, x0\n"
			"li x5, 5\n" 
			"bne x30, x5, LBL_FAIL_HID_6\n"
			"nop\n"          
		       VSNIP_RSV     
		);
		load_vpu_regs();
		__asm__ __volatile__ ( 
			"nop\n"          
			"addi x30, x30, 1\n"
			"la x11, rand_int_32\n"
			"flw.ps f20, 224(x11)\n"
			"fpackreph.pi f9, f20\n" 
			"LBL_SEQID_5_M0_WRITE_ID_9_HID_6:\n"
			"maskxor m6, m5, m4\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x9, rand_int_32\n"
			"flw.ps f10, 480(x9)\n"
			"fsatu8.pi f19, f10\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f11, 544(x20)\n"
			"fcvt.pwu.ps f10, f11, rne\n" 
			"LBL_SEQID_5_M0_WRITE_ID_10_HID_6:\n"
			"fsetm.pi m5, f9\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x11, rand_ieee754_32\n"
			"flw.ps f19, 256(x11)\n"
			"flw.ps f3, 672(x11)\n"
			"feq.ps f26, f19, f3\n" 
			"LBL_SEQID_5_M0_WRITE_ID_11_HID_6:\n"
			"mov.m.x m4, x18, 0x31\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x10, rand_ieee754_32\n"
			"flw.ps f20, 672(x10)\n"
			"flw.ps f19, 704(x10)\n"
			"fcmovm.ps f14, f20, f19\n" 
			"la x9, rand_int_32\n"
			"flw.ps f12, 192(x9)\n"
			"faddi.pi f19, f12, 0x1ae\n" 
			"LBL_SEQID_5_M0_WRITE_ID_12_HID_6:\n"
			"fltm.pi m5, f9, f26\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x18, rand_ieee754_32\n"
			"flw.ps f8, 96(x18)\n"
			"flw.ps f14, 480(x18)\n"
			"fsgnj.ps f18, f8, f14\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f8, 928(x11)\n"
			"fclass.ps f8, f8\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f29, 320(x15)\n"
			"flw.ps f20, 352(x15)\n"
			"fmul.s f30, f29, f20, rne\n" 
			"li x5, 0x1324d6b02f94c59b\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0xc8fa9d78c6df7a83\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"la x22, rand_ieee754_32\n"
			"flw.ps f3, 480(x22)\n"
			"flw.ps f18, 64(x22)\n"
			"flw.ps f12, 768(x22)\n"
			"fmadd.ps f30, f3, f18, f12, rtz\n" 
			"la x9, rand_int_32\n"
			"flw.ps f9, 448(x9)\n"
			"flw.ps f3, 864(x9)\n"
			"fmulhu.pi f21, f9, f3\n" 
			"la x18, rand_int_32\n"
			"flw.ps f9, 992(x18)\n"
			"flw.ps f3, 864(x18)\n"
			"fsrl.pi f14, f9, f3\n" 
			"csrw tensor_error, zero\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"LBL_SEQID_5_XMA_QNT_STR_SEQ_ID_81836_HID_6:\n"
			"li x6, ((0x5 << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0xc << TENSOR_QUANT_QUANT_ROW) | (0x36 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x2 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_6\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_5_XMA_QNT_STR_SEQ_ITER_0_HID_6:\n"
			"li x5, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x0 << TENSOR_IMA_B_NUM_COLS) | (0x2 << TENSOR_IMA_A_NUM_ROWS) | (0x4 << TENSOR_IMA_A_NUM_COLS) | (0x7 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0xa0 << TENSOR_IMA_SCP_LOC_B) | (0xd9 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_6\n"
			"csrw tensor_error, x0\n"
			"li x5, 6\n" 
			"bne x30, x5, LBL_FAIL_HID_6\n"
			"addi x30, x30, 1\n"
			"la x10, rand_ieee754_32\n"
			"flw.ps f12, 896(x10)\n"
			"fcvt.f16.ps f3, f12\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f19, 384(x23)\n"
			"flw.ps f9, 672(x23)\n"
			"fmul.ps f12, f19, f9, rdn\n" 
			"LBL_SEQID_6_M0_WRITE_ID_13_HID_6:\n"
			"masknot m5, m4\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x15, rand_ieee754_32\n"
			"flw.ps f21, 608(x15)\n"
			"flw.ps f27, 896(x15)\n"
			"fmax.s f9, f21, f27\n" 
			"maskpopc x17, m5\n" 
			"li x5, 0xe8f\n"
			"csrw tensor_mask, x5\n"	
			"la x22, rand_int_32\n"
			"flw.ps f14, 800(x22)\n"
			"fsat8.pi f11, f14\n" 
			"la x10, rand_int_32\n"
			"flw.ps f5, 320(x10)\n"
			"flw.ps f27, 192(x10)\n"
			"fsub.pi f9, f5, f27\n" 
			"la x18, rand_int_32\n"
			"flw.ps f9, 384(x18)\n"
			"fsrli.pi f30, f9, 0x7\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f12, 352(x20)\n"
			"flw.ps f27, 128(x20)\n"
			"flw.ps f29, 96(x20)\n"
			"fmadd.ps f29, f12, f27, f29, rup\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f3, 128(x20)\n"
			"flw.ps f14, 288(x20)\n"
			"fsub.s f5, f3, f14, rup\n" 
			"LBL_SEQID_6_M0_WRITE_ID_14_HID_6:\n"
			"maskxor m6, m6, m6\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"LBL_SEQID_6_M0_WRITE_ID_15_HID_6:\n"
			"fltm.ps m6, f9, f10\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x20, rand_int_32\n"
			"flw.ps f30, 32(x20)\n"
			"fandi.pi f26, f30, 0x1da\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f18, 512(x19)\n"
			"fswizz.ps f30, f18, 0x11\n" 
			"li x15, 0x79caa2913f72fd49\n"
			"fcvt.s.lu f29, x15, rtz\n" 
			"maskpopc x9, m5\n" 
			"li x5, 0x9321\n"
			"csrw tensor_mask, x5\n"	
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_6_XMA_QNT_STR_SEQ_ITER_0_HID_6:\n"
			"li x5, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x1 << TENSOR_FMA16_B_NUM_COLS) | (0xa << TENSOR_FMA16_A_NUM_ROWS) | (0x3 << TENSOR_FMA16_A_NUM_COLS) | (0x5 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x9c << TENSOR_FMA16_SCP_LOC_B) | (0x1c << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x0 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_6\n"
			"csrw tensor_error, x0\n"
			"li x5, 7\n" 
			"bne x30, x5, LBL_FAIL_HID_6\n"
			"nop \n"
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=1); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"nop \n"
			"addi x29, x29, -1\n"
			"bne x29, x0, LBL_OUTER_LOOP_SEQ_HID_6\n"
			"nop\n"          
		       VSNIP_RSV     
		);
		load_vpu_regs();
		__asm__ __volatile__ ( 
			"nop\n"          
			"nop\n"          
		       VSNIP_RSV     
		);
		trans();
		__asm__ __volatile__ ( 
			"nop\n"          
			"la x31, check_sum_hid_6\n"
			"lw x5, (x31)\n"
			"bne x30, x5, LBL_FAIL_HID_6\n"
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
			"csrrci x0, mcache_control, 3\n"
			"csrwi tensor_wait, 0x6\n" 
			"fence\n"
			"csrwi tensor_wait, 0x6\n" 
			"fence\n"
			"csrwi excl_mode, 0\n"
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=1); 
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
	if ( hid%16 == 7 ) { 
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
			"csrwi mhpmevent3, 11\n"   
			"csrwi mhpmevent4, 1\n"   
			"csrwi mhpmevent5, 6\n"   
			"csrwi mhpmevent6, 12\n"   
			"csrwi mhpmevent7, 23\n"   
			"csrwi mhpmevent8, 21\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f13,  64 (x5)\n"
			"flw.ps f1,  352 (x5)\n"
			"flw.ps f14,  480 (x5)\n"
			"flw.ps f25,  576 (x5)\n"
			"flw.ps f31,  192 (x5)\n"
			"flw.ps f5,  128 (x5)\n"
			"flw.ps f26,  448 (x5)\n"
			"flw.ps f27,  928 (x5)\n"
			"flw.ps f23,  960 (x5)\n"
			"flw.ps f4,  640 (x5)\n"
			"flw.ps f21,  544 (x5)\n"
			"flw.ps f0,  608 (x5)\n"
			"flw.ps f17,  512 (x5)\n"
			"flw.ps f6,  0 (x5)\n"
			"flw.ps f9,  896 (x5)\n"
			"flw.ps f8,  288 (x5)\n"
			"flw.ps f16,  672 (x5)\n"
			"flw.ps f7,  832 (x5)\n"
			"flw.ps f30,  384 (x5)\n"
			"flw.ps f10,  256 (x5)\n"
			"flw.ps f19,  992 (x5)\n"
			"flw.ps f11,  768 (x5)\n"
			"flw.ps f15,  704 (x5)\n"
			"flw.ps f28,  32 (x5)\n"
			"flw.ps f12,  416 (x5)\n"
			"flw.ps f22,  736 (x5)\n"
			"flw.ps f29,  160 (x5)\n"
			"flw.ps f3,  800 (x5)\n"
			"flw.ps f24,  320 (x5)\n"
			"flw.ps f2,  96 (x5)\n"
			"flw.ps f20,  224 (x5)\n"
			"flw.ps f18,  864 (x5)\n"
			"csrw fflags, zero\n"
			"csrwi frm, 0x0\n"
			"li x31, 100\n"
			"1:\n" 
			"addi x31, x31, -1\n"
			"bne x31, x0, 1b\n" 
			"csrw fcc, x0\n" 
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=1); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"LBL_KERNEL_HID_7:\n"          
			"li x5, 0xFEEDFEED\n"
			"slti x0, x0, 0x7f2\n"
			"la x31, check_sum_hid_7\n"
			"li x5, 0x7\n" 
			"sw x5, (x31)\n"
			"li x29, 0x7\n"
			"LBL_OUTER_LOOP_SEQ_HID_7:\n"
			"nop \n"
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=1); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"nop \n"
			"li x30, 0\n"
			"addi x30, x30, 1\n"
			"la x25, rand_ieee754_32\n"
			"flw.ps f26, 160(x25)\n"
			"flw.ps f21, 32(x25)\n"
			"fcmovm.ps f26, f26, f21\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f30, 384(x14)\n"
			"flw.ps f22, 640(x14)\n"
			"feq.ps f16, f30, f22\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f28, 672(x25)\n"
			"flw.ps f10, 864(x25)\n"
			"fle.s x14, f28, f10\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f22, 608(x26)\n"
			"flw.ps f7, 352(x26)\n"
			"flw.ps f10, 448(x26)\n"
			"fmsub.s f2, f22, f7, f10, rtz\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f21, 416(x7)\n"
			"flw.ps f0, 576(x7)\n"
			"fmax.ps f11, f21, f0\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f12, 224(x7)\n"
			"flw.ps f7, 864(x7)\n"
			"fmul.s f7, f12, f7, rmm\n" 
			"la x25, rand_int_32\n"
			"flw.ps f15, 320(x25)\n"
			"fpackrepb.pi f28, f15\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f15, 896(x25)\n"
			"fcvt.wu.s x25, f15, rne\n" 
			"la x25, rand_int_32\n"
			"flw.ps f10, 288(x25)\n"
			"flw.ps f28, 992(x25)\n"
			"fmaxu.pi f26, f10, f28\n" 
			"LBL_SEQID_0_FP_TRANS_ID_0_HID_7:\n"
			"la x9, rand_ieee754_32\n"
			"flw.ps f16, 192(x9)\n"
			"flog.ps f16, f16\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f7, 384(x14)\n"
			"flw.ps f28, 864(x14)\n"
			"flw.ps f0, 192(x14)\n"
			"fmsub.ps f20, f7, f28, f0, rne\n" 
			"la x17, rand_int_32\n"
			"flw.ps f28, 32(x17)\n"
			"fmulhu.pi f0, f28, f28\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f22, 0(x9)\n"
			"flw.ps f26, 768(x9)\n"
			"fsgnjn.ps f0, f22, f26\n" 
			"maskpopcz x19, m3\n" 
			"la x11, rand_int_32\n"
			"flw.ps f29, 960(x11)\n"
			"flw.ps f23, 96(x11)\n"
			"fle.pi f22, f29, f23\n" 
			"la x14, rand_int_32\n"
			"flw.ps f30, 992(x14)\n"
			"flw.ps f22, 192(x14)\n"
			"fle.pi f7, f30, f22\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_7\n"
			"addi x30, x30, 1\n"
			"la x26, rand_int_32\n"
			"flw.ps f20, 800(x26)\n"
			"flw.ps f0, 672(x26)\n"
			"fsub.pi f11, f20, f0\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f15, 352(x19)\n"
			"flw.ps f12, 96(x19)\n"
			"feq.s x19, f15, f12\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f22, 736(x14)\n"
			"flw.ps f15, 992(x14)\n"
			"flw.ps f23, 928(x14)\n"
			"fmsub.ps f2, f22, f15, f23, rmm\n" 
			"la x14, rand_int_32\n"
			"flw.ps f10, 768(x14)\n"
			"fsrli.pi f20, f10, 0x9\n" 
			"li x26, 0x9f3654fd8c40660b\n"
			"fcvt.s.wu f10, x26, rdn\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f21, 256(x23)\n"
			"flw.ps f20, 704(x23)\n"
			"feq.ps f29, f21, f20\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f16, 704(x9)\n"
			"ffrc.ps f20, f16\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f30, 448(x7)\n"
			"flw.ps f7, 576(x7)\n"
			"fcmovm.ps f26, f30, f7\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f7, 64(x14)\n"
			"flt.ps f30, f7, f7\n" 
			"LBL_SEQID_1_M0_WRITE_ID_1_HID_7:\n"
			"feqm.ps m3, f15, f23\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x11, rand_int_32\n"
			"flw.ps f0, 96(x11)\n"
			"flw.ps f11, 416(x11)\n"
			"fadd.pi f30, f0, f11\n" 
			"la x11, rand_int_32\n"
			"flw.ps f11, 768(x11)\n"
			"flw.ps f20, 384(x11)\n"
			"fminu.pi f2, f11, f20\n" 
			"LBL_SEQID_1_M0_WRITE_ID_2_HID_7:\n"
			"masknot m3, m3\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x14, rand_int_32\n"
			"flw.ps f12, 224(x14)\n"
			"flw.ps f16, 672(x14)\n"
			"fadd.pi f15, f12, f16\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f28, 160(x14)\n"
			"flw.ps f22, 0(x14)\n"
			"fmax.s f29, f28, f22\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f15, 960(x12)\n"
			"fcvt.wu.s x25, f15, rdn\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_7\n"
			"addi x30, x30, 1\n"
			"la x23, rand_int_32\n"
			"flw.ps f15, 64(x23)\n"
			"fpackrepb.pi f29, f15\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f10, 352(x19)\n"
			"flw.ps f0, 704(x19)\n"
			"fadd.s f21, f10, f0, rtz\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f2, 608(x14)\n"
			"flw.ps f20, 32(x14)\n"
			"fsgnjn.s f20, f2, f20\n" 
			"la x26, rand_int_32\n"
			"flw.ps f0, 928(x26)\n"
			"fandi.pi f26, f0, 0x1cb\n" 
			"la x25, rand_int_32\n"
			"flw.ps f22, 640(x25)\n"
			"flw.ps f26, 544(x25)\n"
			"fadd.pi f0, f22, f26\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f2, 256(x11)\n"
			"flw.ps f10, 672(x11)\n"
			"fsgnjx.ps f20, f2, f10\n" 
			"la x25, rand_int_32\n"
			"flw.ps f12, 960(x25)\n"
			"flw.ps f7, 928(x25)\n"
			"for.pi f7, f12, f7\n" 
			"la x7, rand_int_32\n"
			"flw.ps f11, 704(x7)\n"
			"fand.pi f28, f11, f11\n" 
			"la x25, rand_int_32\n"
			"flw.ps f0, 256(x25)\n"
			"flw.ps f7, 224(x25)\n"
			"fmax.pi f28, f0, f7\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f12, 96(x17)\n"
			"fcvt.pw.ps f10, f12, rup\n" 
			"la x25, rand_int_32\n"
			"flw.ps f29, 0(x25)\n"
			"fcvt.ps.pwu f30, f29, rmm\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f26, 256(x26)\n"
			"flw.ps f10, 832(x26)\n"
			"fsub.ps f28, f26, f10, rne\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f20, 768(x19)\n"
			"flw.ps f23, 32(x19)\n"
			"flw.ps f7, 352(x19)\n"
			"fmsub.s f7, f20, f23, f7, rtz\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f23, 576(x14)\n"
			"flw.ps f29, 640(x14)\n"
			"flt.ps f23, f23, f29\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f12, 512(x25)\n"
			"fsin.ps f22, f12\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f16, 288(x19)\n"
			"flw.ps f23, 928(x19)\n"
			"fsgnjx.ps f20, f16, f23\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 3\n" 
			"bne x30, x5, LBL_FAIL_HID_7\n"
			"nop\n"          
		       VSNIP_RSV     
		);
		load_vpu_regs();
		__asm__ __volatile__ ( 
			"nop\n"          
			"addi x30, x30, 1\n"
			"la x19, rand_int_32\n"
			"flw.ps f0, 960(x19)\n"
			"flw.ps f28, 32(x19)\n"
			"fmulhu.pi f28, f0, f28\n" 
			"la x25, rand_int_32\n"
			"flw.ps f20, 512(x25)\n"
			"fnot.pi f29, f20\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f30, 192(x9)\n"
			"flw.ps f29, 384(x9)\n"
			"fadd.s f30, f30, f29, rdn\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f12, 32(x25)\n"
			"flw.ps f26, 704(x25)\n"
			"fadd.ps f0, f12, f26, rmm\n" 
			"la x26, rand_int_32\n"
			"flw.ps f10, 320(x26)\n"
			"flw.ps f29, 736(x26)\n"
			"feq.pi f10, f10, f29\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f21, 448(x26)\n"
			"fcvt.w.s x12, f21, rtz\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f30, 160(x25)\n"
			"flw.ps f2, 512(x25)\n"
			"flw.ps f29, 832(x25)\n"
			"fnmadd.ps f29, f30, f2, f29, rup\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f16, 480(x7)\n"
			"fcvt.pwu.ps f28, f16, rne\n" 
			"la x19, rand_int_32\n"
			"flw.ps f12, 32(x19)\n"
			"flw.ps f30, 0(x19)\n"
			"fmax.pi f30, f12, f30\n" 
			"la x7, rand_int_32\n"
			"flw.ps f16, 928(x7)\n"
			"fsrli.pi f23, f16, 0xb\n" 
			"LBL_SEQID_3_M0_WRITE_ID_3_HID_7:\n"
			"masknot m2, m5\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_3_M0_WRITE_ID_4_HID_7:\n"
			"fsetm.pi m2, f23\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x7, rand_ieee754_32\n"
			"flw.ps f29, 576(x7)\n"
			"flw.ps f26, 544(x7)\n"
			"fsub.ps f29, f29, f26, rdn\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f15, 928(x12)\n"
			"flw.ps f28, 448(x12)\n"
			"fsgnjn.s f15, f15, f28\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f16, 192(x23)\n"
			"fcvt.wu.s x23, f16, rdn\n" 
			"mova.x.m x14\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 4\n" 
			"bne x30, x5, LBL_FAIL_HID_7\n"
			"nop\n"          
		       VSNIP_RSV     
		);
		trans();
		__asm__ __volatile__ ( 
			"nop\n"          
			"addi x30, x30, 1\n"
			"la x12, rand_ieee754_32\n"
			"flw.ps f11, 288(x12)\n"
			"flw.ps f22, 352(x12)\n"
			"fmin.ps f29, f11, f22\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f10, 416(x17)\n"
			"flw.ps f29, 992(x17)\n"
			"fsgnjn.s f11, f10, f29\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f21, 640(x23)\n"
			"flw.ps f23, 704(x23)\n"
			"fsgnj.s f0, f21, f23\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f10, 800(x19)\n"
			"fcvt.wu.s x23, f10, rtz\n" 
			"la x12, rand_int_32\n"
			"flw.ps f26, 928(x12)\n"
			"fcvt.ps.pw f22, f26, rne\n" 
			"la x11, rand_int_32\n"
			"flw.ps f20, 864(x11)\n"
			"fnot.pi f16, f20\n" 
			"la x11, rand_ieee754_16\n"
			"flw.ps f29, 800(x11)\n"
			"fcvt.ps.f16 f10, f29\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f28, 512(x26)\n"
			"flw.ps f22, 192(x26)\n"
			"fdiv.s f20, f28, f22, rup\n" 
			"mova.x.m x12\n" 
			"maskpopcz x14, m2\n" 
			"la x12, rand_int_32\n"
			"flw.ps f10, 352(x12)\n"
			"flw.ps f15, 0(x12)\n"
			"fsll.pi f20, f10, f15\n" 
			"la x7, rand_int_32\n"
			"flw.ps f22, 320(x7)\n"
			"flw.ps f7, 224(x7)\n"
			"fsub.pi f10, f22, f7\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f15, 608(x14)\n"
			"fmul.s f29, f15, f15, rne\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f15, 384(x12)\n"
			"flw.ps f22, 864(x12)\n"
			"flw.ps f26, 736(x12)\n"
			"fnmsub.s f0, f15, f22, f26, rdn\n" 
			"la x7, rand_int_32\n"
			"flw.ps f10, 800(x7)\n"
			"fcvt.ps.pw f15, f10, rne\n" 
			"LBL_SEQID_4_M0_WRITE_ID_5_HID_7:\n"
			"fsetm.pi m1, f15\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"csrw tensor_error, zero\n" 
			"li x5, 5\n" 
			"bne x30, x5, LBL_FAIL_HID_7\n"
			"nop\n"          
		       VSNIP_RSV     
		);
		load_vpu_regs();
		__asm__ __volatile__ ( 
			"nop\n"          
			"addi x30, x30, 1\n"
			"LBL_SEQID_5_M0_WRITE_ID_6_HID_7:\n"
			"fltm.pi m2, f29, f7\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0x5a50c498fc\n"
			"fsw.ps f0, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"li x7, 0x92b03d99c55020c9\n"
			"fcvt.s.w f30, x7, rup\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f29, 448(x7)\n"
			"flw.ps f26, 800(x7)\n"
			"fcmovm.ps f22, f29, f26\n" 
			"la x9, rand_int_32\n"
			"flw.ps f30, 480(x9)\n"
			"flw.ps f23, 480(x9)\n"
			"fmin.pi f15, f30, f23\n" 
			"la x14, rand_int_32\n"
			"flw.ps f20, 576(x14)\n"
			"fmaxu.pi f10, f20, f20\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f21, 768(x14)\n"
			"flw.ps f7, 480(x14)\n"
			"feq.s x17, f21, f7\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f30, 128(x11)\n"
			"flw.ps f28, 608(x11)\n"
			"flw.ps f21, 640(x11)\n"
			"fnmsub.ps f12, f30, f28, f21, rtz\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f11, 768(x17)\n"
			"flw.ps f2, 320(x17)\n"
			"fadd.s f11, f11, f2, rtz\n" 
			"maskpopc x11, m1\n" 
			"la x11, rand_int_32\n"
			"flw.ps f15, 640(x11)\n"
			"flw.ps f28, 704(x11)\n"
			"fmin.pi f7, f15, f28\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f16, 160(x9)\n"
			"flw.ps f29, 928(x9)\n"
			"fmsub.ps f20, f16, f29, f16, rmm\n" 
			"LBL_SEQID_5_M0_WRITE_ID_7_HID_7:\n"
			"maskxor m1, m2, m2\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x23, rand_int_32\n"
			"flw.ps f12, 448(x23)\n"
			"fcvt.ps.pw f2, f12, rtz\n" 
			"LBL_SEQID_5_FP_TRANS_ID_8_HID_7:\n"
			"la x25, rand_ieee754_32\n"
			"flw.ps f2, 672(x25)\n"
			"flog.ps f16, f2\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f7, 384(x9)\n"
			"flw.ps f30, 672(x9)\n"
			"fnmsub.ps f12, f7, f30, f7, rup\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f12, 256(x23)\n"
			"flw.ps f10, 224(x23)\n"
			"fcmovm.ps f28, f12, f10\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 6\n" 
			"bne x30, x5, LBL_FAIL_HID_7\n"
			"addi x30, x30, 1\n"
			"la x19, rand_int_32\n"
			"flw.ps f12, 960(x19)\n"
			"flw.ps f15, 928(x19)\n"
			"fmin.pi f0, f12, f15\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f30, 896(x9)\n"
			"flw.ps f0, 704(x9)\n"
			"fnmadd.s f23, f30, f0, f30, rmm\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f29, 864(x23)\n"
			"flw.ps f10, 576(x23)\n"
			"flw.ps f15, 864(x23)\n"
			"fnmadd.s f29, f29, f10, f15, rdn\n" 
			"la x17, rand_int_32\n"
			"flw.ps f26, 384(x17)\n"
			"fsrai.pi f28, f26, 0xf\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f12, 576(x17)\n"
			"flw.ps f29, 128(x17)\n"
			"flw.ps f10, 128(x17)\n"
			"fnmsub.s f23, f12, f29, f10, rmm\n" 
			"la x11, rand_int_32\n"
			"flw.ps f10, 96(x11)\n"
			"flw.ps f28, 128(x11)\n"
			"frem.pi f23, f10, f28\n" 
			"la x11, rand_int_32\n"
			"flw.ps f21, 832(x11)\n"
			"flw.ps f7, 896(x11)\n"
			"fle.pi f29, f21, f7\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f0, 544(x7)\n"
			"fclass.s x12, f0\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f22, 896(x9)\n"
			"flw.ps f15, 416(x9)\n"
			"fmin.s f22, f22, f15\n" 
			"la x9, rand_int_32\n"
			"flw.ps f0, 96(x9)\n"
			"flw.ps f11, 800(x9)\n"
			"fadd.pi f20, f0, f11\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f2, 416(x26)\n"
			"flw.ps f21, 32(x26)\n"
			"flw.ps f22, 160(x26)\n"
			"fnmsub.ps f23, f2, f21, f22, rne\n" 
			"la x14, rand_int_32\n"
			"flw.ps f28, 224(x14)\n"
			"fsrl.pi f2, f28, f28\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f16, 992(x9)\n"
			"fsgnj.ps f10, f16, f16\n" 
			"LBL_SEQID_6_FP_TRANS_ID_9_HID_7:\n"
			"la x26, rand_ieee754_32\n"
			"flw.ps f12, 960(x26)\n"
			"fexp.ps f16, f12\n" 
			"la x17, rand_int_32\n"
			"flw.ps f20, 256(x17)\n"
			"fmulhu.pi f26, f20, f20\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f16, 192(x14)\n"
			"flw.ps f23, 288(x14)\n"
			"flt.ps f23, f16, f23\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 7\n" 
			"bne x30, x5, LBL_FAIL_HID_7\n"
			"nop \n"
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=1); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"nop \n"
			"addi x29, x29, -1\n"
			"bne x29, x0, LBL_OUTER_LOOP_SEQ_HID_7\n"
			"nop\n"          
		       VSNIP_RSV     
		);
		load_vpu_regs();
		__asm__ __volatile__ ( 
			"nop\n"          
			"nop\n"          
		       VSNIP_RSV     
		);
		trans();
		__asm__ __volatile__ ( 
			"nop\n"          
			"la x31, check_sum_hid_7\n"
			"lw x5, (x31)\n"
			"bne x30, x5, LBL_FAIL_HID_7\n"
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
		c_sync_point(sync_odd_harts=1); 
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
	if ( hid%16 == 8 ) { 
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
			"csrwi mhpmevent3, 16\n"   
			"csrwi mhpmevent4, 6\n"   
			"csrwi mhpmevent5, 4\n"   
			"csrwi mhpmevent6, 27\n"   
			"csrwi mhpmevent7, 6\n"   
			"csrwi mhpmevent8, 3\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f14,  96 (x5)\n"
			"flw.ps f18,  672 (x5)\n"
			"flw.ps f6,  288 (x5)\n"
			"flw.ps f31,  960 (x5)\n"
			"flw.ps f26,  544 (x5)\n"
			"flw.ps f7,  352 (x5)\n"
			"flw.ps f0,  0 (x5)\n"
			"flw.ps f11,  512 (x5)\n"
			"flw.ps f2,  480 (x5)\n"
			"flw.ps f21,  448 (x5)\n"
			"flw.ps f15,  320 (x5)\n"
			"flw.ps f25,  704 (x5)\n"
			"flw.ps f17,  192 (x5)\n"
			"flw.ps f4,  64 (x5)\n"
			"flw.ps f8,  384 (x5)\n"
			"flw.ps f28,  640 (x5)\n"
			"flw.ps f1,  864 (x5)\n"
			"flw.ps f27,  800 (x5)\n"
			"flw.ps f23,  992 (x5)\n"
			"flw.ps f22,  576 (x5)\n"
			"flw.ps f5,  608 (x5)\n"
			"flw.ps f16,  832 (x5)\n"
			"flw.ps f12,  768 (x5)\n"
			"flw.ps f19,  928 (x5)\n"
			"flw.ps f13,  160 (x5)\n"
			"flw.ps f20,  896 (x5)\n"
			"flw.ps f30,  736 (x5)\n"
			"flw.ps f10,  224 (x5)\n"
			"flw.ps f3,  416 (x5)\n"
			"flw.ps f9,  32 (x5)\n"
			"flw.ps f24,  128 (x5)\n"
			"flw.ps f29,  256 (x5)\n"
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
			"csrw fcc, x0\n" 
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=1); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"LBL_KERNEL_HID_8:\n"          
			"li x5, 0xFEEDFEED\n"
			"slti x0, x0, 0x7f2\n"
			"la x31, check_sum_hid_8\n"
			"li x5, 0x7\n" 
			"sw x5, (x31)\n"
			"li x29, 0x7\n"
			"LBL_OUTER_LOOP_SEQ_HID_8:\n"
			"nop \n"
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=1); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"nop \n"
			"li x30, 0\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_8:\n"
			"mova.m.x x6\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x28, rand_ieee754_32\n"
			"flw.ps f14, 576(x28)\n"
			"flw.ps f11, 160(x28)\n"
			"flw.ps f13, 416(x28)\n"
			"fmadd.ps f31, f14, f11, f13, rne\n" 
			"maskpopc x18, m1\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f16, 864(x10)\n"
			"flw.ps f31, 288(x10)\n"
			"feq.ps f2, f16, f31\n" 
			"LBL_SEQID_0_FP_TRANS_ID_1_HID_8:\n"
			"la x5, rand_ieee754_32\n"
			"flw.ps f31, 288(x5)\n"
			"fexp.ps f9, f31\n" 
			"la x18, rand_int_32\n"
			"flw.ps f31, 384(x18)\n"
			"flw.ps f22, 288(x18)\n"
			"fsrl.pi f14, f31, f22\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f10, 128(x18)\n"
			"ffrc.ps f20, f10\n" 
			"la x5, rand_int_32\n"
			"flw.ps f26, 736(x5)\n"
			"flw.ps f10, 960(x5)\n"
			"fadd.pi f14, f26, f10\n" 
			"la x18, rand_int_32\n"
			"flw.ps f16, 544(x18)\n"
			"fpackreph.pi f2, f16\n" 
			"LBL_SEQID_0_M0_WRITE_ID_2_HID_8:\n"
			"flem.ps m6, f10, f1\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x18, rand_ieee754_32\n"
			"flw.ps f3, 224(x18)\n"
			"fcvt.f16.ps f1, f3\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f15, 736(x26)\n"
			"flw.ps f1, 672(x26)\n"
			"fmadd.s f22, f15, f1, f15, rdn\n" 
			"la x7, rand_int_32\n"
			"flw.ps f9, 288(x7)\n"
			"fsat8.pi f20, f9\n" 
			"li x5, 0x16bc\n"
			"csrw tensor_mask, x5\n"	
			"la x7, rand_ieee754_32\n"
			"flw.ps f13, 832(x7)\n"
			"flw.ps f29, 256(x7)\n"
			"fsgnjn.s f10, f13, f29\n" 
			"LBL_SEQID_0_M0_WRITE_ID_3_HID_8:\n"
			"fltm.pi m5, f11, f11\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x28, rand_ieee754_32\n"
			"flw.ps f2, 928(x28)\n"
			"fround.ps f14, f2, rup\n" 
			"csrw tensor_error, zero\n" 
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=0); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ITER_0_HID_8:\n"
			"li x5, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x2 << TENSOR_FMA16_B_NUM_COLS) | (0x4 << TENSOR_FMA16_A_NUM_ROWS) | (0xe << TENSOR_FMA16_A_NUM_COLS) | (0x7 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x44 << TENSOR_FMA16_SCP_LOC_B) | (0x3e << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x0 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_8\n"
			"csrw tensor_error, x0\n"
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_8\n"
			"addi x30, x30, 1\n"
			"la x27, rand_ieee754_32\n"
			"flw.ps f26, 96(x27)\n"
			"flw.ps f3, 640(x27)\n"
			"fadd.s f9, f26, f3, rtz\n" 
			"la x7, rand_int_32\n"
			"flw.ps f9, 352(x7)\n"
			"flw.ps f13, 448(x7)\n"
			"fmul.pi f18, f9, f13\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f10, 800(x6)\n"
			"flw.ps f15, 416(x6)\n"
			"flw.ps f20, 832(x6)\n"
			"fnmadd.s f29, f10, f15, f20, rtz\n" 
			"la x18, rand_int_32\n"
			"flw.ps f26, 0(x18)\n"
			"flw.ps f22, 160(x18)\n"
			"fltu.pi f20, f26, f22\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"LBL_SEQID_1_M0_WRITE_ID_4_HID_8:\n"
			"maskxor m6, m5, m1\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"li x19, 0xdaa349cde3fe2745\n"
			"fcvt.s.w f14, x19, rup\n" 
			"maskpopc x15, m6\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f20, 96(x26)\n"
			"flw.ps f16, 224(x26)\n"
			"fadd.ps f15, f20, f16, rdn\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f14, 736(x15)\n"
			"flw.ps f22, 544(x15)\n"
			"fsgnjx.s f11, f14, f22\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f3, 960(x6)\n"
			"fsub.ps f29, f3, f3, rne\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f10, 64(x5)\n"
			"flw.ps f1, 608(x5)\n"
			"fle.s x27, f10, f1\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f20, 32(x19)\n"
			"flw.ps f15, 96(x19)\n"
			"fle.ps f26, f20, f15\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f2, 864(x15)\n"
			"frsq.ps f26, f2\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f10, 480(x5)\n"
			"flw.ps f22, 192(x5)\n"
			"flw.ps f3, 160(x5)\n"
			"fnmadd.ps f20, f10, f22, f3, rmm\n" 
			"la x27, rand_int_32\n"
			"flw.ps f1, 32(x27)\n"
			"flw.ps f20, 544(x27)\n"
			"fmul.pi f10, f1, f20\n" 
			"la x27, rand_int_32\n"
			"flw.ps f20, 32(x27)\n"
			"flw.ps f31, 512(x27)\n"
			"fltu.pi f13, f20, f31\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_1_RDX_ID_1_SHIRE_0_HID_8:\n"
			"li x18, 0xa0000000010000a\n"
			"csrw tensor_reduce, x18\n" 
			"csrwi tensor_wait, 9\n" 
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ID_118313_HID_8:\n"
			"li x5, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x2 << TENSOR_IMA_B_NUM_COLS) | (0xa << TENSOR_IMA_A_NUM_ROWS) | (0x0 << TENSOR_IMA_A_NUM_COLS) | (0x3 << TENSOR_IMA_OFFSET) | (0x0 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x31 << TENSOR_IMA_SCP_LOC_B) | (0x30 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x0 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_8\n"
			"csrw tensor_error, x0\n"
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=0); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ITER_0_HID_8:\n"
			"li x6, ((0x1e << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0xe << TENSOR_QUANT_QUANT_ROW) | (0x3b << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x6 << TENSOR_QUANT_TRANSF5) | (0x7 << TENSOR_QUANT_TRANSF4) | (0x6 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_8\n"
			"csrw tensor_error, x0\n"
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_8\n"
			"addi x30, x30, 1\n"
			"la x7, rand_ieee754_16\n"
			"flw.ps f11, 480(x7)\n"
			"fcvt.ps.f16 f14, f11\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f22, 416(x19)\n"
			"fcvt.wu.s x10, f22, rne\n" 
			"LBL_SEQID_2_FP_TRANS_ID_5_HID_8:\n"
			"la x6, rand_ieee754_32\n"
			"flw.ps f18, 832(x6)\n"
			"flog.ps f3, f18\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f1, 864(x19)\n"
			"fsin.ps f14, f1\n" 
			"la x5, rand_int_32\n"
			"flw.ps f22, 512(x5)\n"
			"fcvt.ps.pwu f3, f22, rmm\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x28, rand_ieee754_32\n"
			"flw.ps f13, 64(x28)\n"
			"flw.ps f16, 448(x28)\n"
			"fmul.ps f1, f13, f16, rne\n" 
			"la x18, rand_int_32\n"
			"flw.ps f18, 768(x18)\n"
			"flw.ps f3, 704(x18)\n"
			"fxor.pi f9, f18, f3\n" 
			"li x5, 0x38f9\n"
			"csrw tensor_mask, x5\n"	
			"la x6, rand_ieee754_32\n"
			"flw.ps f22, 384(x6)\n"
			"fcvt.lu.s x27, f22, rdn\n" 
			"LBL_SEQID_2_M0_WRITE_ID_6_HID_8:\n"
			"maskor m6, m6, m1\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_2_FP_TRANS_ID_7_HID_8:\n"
			"la x18, rand_ieee754_32\n"
			"flw.ps f14, 928(x18)\n"
			"fexp.ps f10, f14\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f10, 384(x19)\n"
			"flw.ps f2, 640(x19)\n"
			"fle.ps f2, f10, f2\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f2, 320(x7)\n"
			"frsq.ps f15, f2\n" 
			"la x10, rand_int_32\n"
			"flw.ps f22, 576(x10)\n"
			"flw.ps f20, 608(x10)\n"
			"feq.pi f2, f22, f20\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f2, 800(x28)\n"
			"fmax.ps f11, f2, f2\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f15, 256(x19)\n"
			"fcvt.w.s x19, f15, rne\n" 
			"fbci.ps f2, 0x33cc0\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 0x1f4c0f9fc3431675\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0x2aa960f5f61f3567\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"LBL_SEQID_2_XMA_QNT_STR_SEQ_ITER_0_HID_8:\n"
			"li x5, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x1 << TENSOR_IMA_B_NUM_COLS) | (0xd << TENSOR_IMA_A_NUM_ROWS) | (0xd << TENSOR_IMA_A_NUM_COLS) | (0x2 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x32 << TENSOR_IMA_SCP_LOC_B) | (0xba << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x0 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_8\n"
			"csrw tensor_error, x0\n"
			"li x5, 3\n" 
			"bne x30, x5, LBL_FAIL_HID_8\n"
			"nop\n"          
		       VSNIP_RSV     
		);
		load_vpu_regs();
		__asm__ __volatile__ ( 
			"nop\n"          
			"addi x30, x30, 1\n"
			"la x5, rand_int_32\n"
			"flw.ps f15, 352(x5)\n"
			"fnot.pi f26, f15\n" 
			"la x26, rand_int_32\n"
			"flw.ps f2, 736(x26)\n"
			"flw.ps f20, 288(x26)\n"
			"fmul.pi f9, f2, f20\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f14, 832(x18)\n"
			"ffrc.ps f26, f14\n" 
			"maskpopcz x26, m5\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f9, 352(x27)\n"
			"ffrc.ps f29, f9\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f31, 672(x5)\n"
			"fsqrt.s f9, f31, rne\n" 
			"la x7, rand_int_32\n"
			"flw.ps f11, 64(x7)\n"
			"fpackreph.pi f18, f11\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f9, 288(x15)\n"
			"flw.ps f13, 384(x15)\n"
			"fsgnj.ps f18, f9, f13\n" 
			"la x19, rand_int_32\n"
			"flw.ps f1, 320(x19)\n"
			"flw.ps f13, 608(x19)\n"
			"fmax.pi f16, f1, f13\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f15, 256(x19)\n"
			"flw.ps f16, 832(x19)\n"
			"flt.s x28, f15, f16\n" 
			"la x6, rand_int_32\n"
			"flw.ps f9, 160(x6)\n"
			"flw.ps f15, 0(x6)\n"
			"fmaxu.pi f1, f9, f15\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f10, 256(x15)\n"
			"flw.ps f9, 512(x15)\n"
			"feq.ps f15, f10, f9\n" 
			"LBL_SEQID_3_M0_WRITE_ID_8_HID_8:\n"
			"mov.m.x m5, x28, 0x1d\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x10, rand_int_32\n"
			"flw.ps f15, 608(x10)\n"
			"flw.ps f9, 672(x10)\n"
			"feq.pi f15, f15, f9\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f3, 32(x6)\n"
			"flw.ps f14, 320(x6)\n"
			"flw.ps f20, 960(x6)\n"
			"fnmadd.ps f13, f3, f14, f20, rup\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f10, 544(x7)\n"
			"flw.ps f11, 192(x7)\n"
			"feq.ps f16, f10, f11\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_3_XMA_QNT_STR_SEQ_ID_963333_HID_8:\n"
			"li x6, ((0x10 << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0x9 << TENSOR_QUANT_QUANT_ROW) | (0x35 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x1 << TENSOR_QUANT_TRANSF2) | (0x5 << TENSOR_QUANT_TRANSF1) | (0x9 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_3_XMA_QNT_STR_SEQ_ID_929285_HID_8:\n"
			"li x6, ((0xe << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0x7 << TENSOR_QUANT_QUANT_ROW) | (0x39 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x7 << TENSOR_QUANT_TRANSF7) | (0x6 << TENSOR_QUANT_TRANSF6) | (0x6 << TENSOR_QUANT_TRANSF5) | (0x6 << TENSOR_QUANT_TRANSF4) | (0x7 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_8\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_3_XMA_QNT_STR_SEQ_ITER_0_HID_8:\n"
			"li x6, ((0x14 << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0x3 << TENSOR_QUANT_QUANT_ROW) | (0x11 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x6 << TENSOR_QUANT_TRANSF4) | (0x7 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_8\n"
			"csrw tensor_error, x0\n"
			"li x5, 4\n" 
			"bne x30, x5, LBL_FAIL_HID_8\n"
			"nop\n"          
		       VSNIP_RSV     
		);
		trans();
		__asm__ __volatile__ ( 
			"nop\n"          
			"addi x30, x30, 1\n"
			"la x7, rand_int_32\n"
			"flw.ps f31, 512(x7)\n"
			"flw.ps f22, 256(x7)\n"
			"fsub.pi f20, f31, f22\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f16, 640(x5)\n"
			"flw.ps f31, 544(x5)\n"
			"flw.ps f1, 768(x5)\n"
			"fnmadd.ps f10, f16, f31, f1, rup\n" 
			"mova.x.m x27\n" 
			"la x26, rand_int_32\n"
			"flw.ps f15, 704(x26)\n"
			"fcvt.ps.pwu f31, f15, rtz\n" 
			"la x18, rand_int_32\n"
			"flw.ps f18, 896(x18)\n"
			"flw.ps f26, 224(x18)\n"
			"fmaxu.pi f9, f18, f26\n" 
			"LBL_SEQID_4_FP_TRANS_ID_9_HID_8:\n"
			"la x26, rand_ieee754_32\n"
			"flw.ps f31, 672(x26)\n"
			"flog.ps f29, f31\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f10, 576(x5)\n"
			"flw.ps f2, 608(x5)\n"
			"fcmov.ps f15, f10, f2, f10\n" 
			"la x28, rand_int_32\n"
			"flw.ps f26, 992(x28)\n"
			"fmulh.pi f2, f26, f26\n" 
			"fbci.ps f1, 0x7d409\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f13, 832(x18)\n"
			"flw.ps f20, 704(x18)\n"
			"fmin.s f14, f13, f20\n" 
			"la x18, rand_int_32\n"
			"flw.ps f16, 704(x18)\n"
			"flw.ps f18, 416(x18)\n"
			"fsll.pi f9, f16, f18\n" 
			"la x6, rand_int_32\n"
			"flw.ps f20, 672(x6)\n"
			"flw.ps f13, 960(x6)\n"
			"fsra.pi f13, f20, f13\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f16, 256(x26)\n"
			"flw.ps f15, 480(x26)\n"
			"fsgnj.s f2, f16, f15\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f18, 768(x18)\n"
			"flw.ps f26, 160(x18)\n"
			"flw.ps f3, 992(x18)\n"
			"fmsub.s f14, f18, f26, f3, rup\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f14, 800(x15)\n"
			"fcvt.pwu.ps f15, f14, rmm\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f9, 704(x15)\n"
			"frsq.ps f31, f9\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_4_TLD_XMA_STR_SEQ_ITER_0_HID_8:\n"
			"la x31, self_check_8\n"
			"li x5, 0x2000000000000c\n"
			"add x5, x5, x31\n"
			"li x6, 0x3e000000000000e\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x1 << TENSOR_IMA_B_NUM_COLS) | (0xc << TENSOR_IMA_A_NUM_ROWS) | (0xe << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1f << TENSOR_IMA_SCP_LOC_B) | (0x1 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x9, ((0xd << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0xc << TENSOR_QUANT_QUANT_ROW) | (0x2d << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0xa << TENSOR_QUANT_TRANSF7) | (0x5 << TENSOR_QUANT_TRANSF6) | (0x9 << TENSOR_QUANT_TRANSF5) | (0x8 << TENSOR_QUANT_TRANSF4) | (0xa << TENSOR_QUANT_TRANSF3) | (0x8 << TENSOR_QUANT_TRANSF2) | (0x8 << TENSOR_QUANT_TRANSF1) | (0x3 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fmvz.x.ps x31, f0, 0\n"
			"li x5, 0x3c\n"
			"bne x5, x31, LBL_FAIL_HID_8\n"
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_8\n"
			"csrw tensor_error, x0\n"
			"li x5, 5\n" 
			"bne x30, x5, LBL_FAIL_HID_8\n"
			"nop\n"          
		       VSNIP_RSV     
		);
		load_vpu_regs();
		__asm__ __volatile__ ( 
			"nop\n"          
			"addi x30, x30, 1\n"
			"fbci.pi f26, 0x3adae\n" 
			"la x10, rand_int_32\n"
			"flw.ps f16, 416(x10)\n"
			"fsrai.pi f20, f16, 0xa\n" 
			"la x26, rand_int_32\n"
			"flw.ps f20, 960(x26)\n"
			"flw.ps f9, 448(x26)\n"
			"flt.pi f16, f20, f9\n" 
			"LBL_SEQID_5_FP_TRANS_ID_10_HID_8:\n"
			"la x5, rand_ieee754_32\n"
			"flw.ps f31, 864(x5)\n"
			"fexp.ps f13, f31\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f18, 960(x18)\n"
			"frsq.ps f10, f18\n" 
			"fbci.ps f10, 0x27221\n" 
			"la x28, rand_int_32\n"
			"flw.ps f26, 576(x28)\n"
			"flw.ps f14, 896(x28)\n"
			"fmin.pi f3, f26, f14\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f9, 224(x15)\n"
			"flw.ps f11, 832(x15)\n"
			"flw.ps f3, 0(x15)\n"
			"fcmov.ps f3, f9, f11, f3\n" 
			"la x27, rand_int_32\n"
			"flw.ps f31, 448(x27)\n"
			"faddi.pi f11, f31, 0xf6\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f9, 800(x15)\n"
			"fcvt.pw.ps f16, f9, rdn\n" 
			"li x5, 0x9c04\n"
			"csrw tensor_mask, x5\n"	
			"la x27, rand_int_32\n"
			"flw.ps f26, 736(x27)\n"
			"flw.ps f10, 896(x27)\n"
			"fsra.pi f22, f26, f10\n" 
			"la x28, rand_int_32\n"
			"flw.ps f22, 128(x28)\n"
			"flw.ps f14, 992(x28)\n"
			"fmin.pi f26, f22, f14\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f16, 736(x27)\n"
			"flw.ps f29, 864(x27)\n"
			"fsub.ps f9, f16, f29, rdn\n" 
			"la x26, rand_int_32\n"
			"flw.ps f22, 896(x26)\n"
			"fandi.pi f9, f22, 0x45\n" 
			"LBL_SEQID_5_FP_TRANS_ID_11_HID_8:\n"
			"la x28, rand_ieee754_32\n"
			"flw.ps f13, 864(x28)\n"
			"fexp.ps f26, f13\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f29, 832(x6)\n"
			"flw.ps f3, 192(x6)\n"
			"flw.ps f31, 416(x6)\n"
			"fnmadd.s f13, f29, f3, f31, rtz\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_5_TLD_XMA_STR_SEQ_ID_798191_HID_8:\n"
			"la x31, self_check_16\n"
			"li x5, 0x20000000000003\n"
			"add x5, x5, x31\n"
			"li x6, 0x10000000000009\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x2 << TENSOR_FMA16_B_NUM_COLS) | (0x3 << TENSOR_FMA16_A_NUM_ROWS) | (0x9 << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x1 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1d << TENSOR_FMA16_SCP_LOC_B) | (0x1 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0x14\n"
			"bne x5, x31, LBL_FAIL_HID_8\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_8\n"
			"csrw tensor_error, x0\n"
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"LBL_SEQID_5_TLD_XMA_STR_SEQ_ITER_0_HID_8:\n"
			"la x31, self_check_16\n"
			"li x5, 0x2000000000000c\n"
			"add x5, x5, x31\n"
			"li x6, 0x10000000000008\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x3 << TENSOR_FMA16_B_NUM_COLS) | (0xc << TENSOR_FMA16_A_NUM_ROWS) | (0x8 << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x1 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1c << TENSOR_FMA16_SCP_LOC_B) | (0x1 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x9, ((0xa << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0xc << TENSOR_QUANT_QUANT_ROW) | (0x1e << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x2 << TENSOR_QUANT_TRANSF4) | (0x7 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0x12\n"
			"bne x5, x31, LBL_FAIL_HID_8\n"
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_8\n"
			"csrw tensor_error, x0\n"
			"li x5, 6\n" 
			"bne x30, x5, LBL_FAIL_HID_8\n"
			"addi x30, x30, 1\n"
			"la x18, rand_ieee754_32\n"
			"flw.ps f1, 0(x18)\n"
			"flw.ps f2, 992(x18)\n"
			"flw.ps f10, 256(x18)\n"
			"fmsub.s f26, f1, f2, f10, rne\n" 
			"la x5, rand_int_32\n"
			"flw.ps f14, 928(x5)\n"
			"faddi.pi f26, f14, 0x16b\n" 
			"la x6, rand_int_32\n"
			"flw.ps f22, 800(x6)\n"
			"fsatu8.pi f11, f22\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f3, 992(x10)\n"
			"flw.ps f13, 96(x10)\n"
			"feq.s x15, f3, f13\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x15, rand_int_32\n"
			"flw.ps f3, 896(x15)\n"
			"flw.ps f1, 608(x15)\n"
			"fsll.pi f13, f3, f1\n" 
			"LBL_SEQID_6_M0_WRITE_ID_12_HID_8:\n"
			"mov.m.x m6, x10, 0x24\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x7, rand_int_32\n"
			"flw.ps f18, 672(x7)\n"
			"flw.ps f29, 480(x7)\n"
			"for.pi f20, f18, f29\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f11, 992(x6)\n"
			"fmin.ps f11, f11, f11\n" 
			"li x5, 0xc4b0\n"
			"csrw tensor_mask, x5\n"	
			"la x26, rand_int_32\n"
			"flw.ps f3, 96(x26)\n"
			"fsat8.pi f29, f3\n" 
			"la x28, rand_int_32\n"
			"flw.ps f1, 896(x28)\n"
			"fcvt.ps.pwu f2, f1, rup\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f11, 544(x6)\n"
			"fsin.ps f13, f11\n" 
			"la x7, rand_int_32\n"
			"flw.ps f16, 384(x7)\n"
			"flw.ps f2, 448(x7)\n"
			"fmul.pi f1, f16, f2\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f9, 160(x15)\n"
			"flw.ps f13, 768(x15)\n"
			"feq.ps f13, f9, f13\n" 
			"la x18, rand_int_32\n"
			"flw.ps f29, 96(x18)\n"
			"flw.ps f16, 800(x18)\n"
			"fsub.pi f14, f29, f16\n" 
			"la x7, rand_int_32\n"
			"flw.ps f14, 800(x7)\n"
			"fslli.pi f11, f14, 0x7\n" 
			"la x15, rand_int_32\n"
			"flw.ps f16, 928(x15)\n"
			"flw.ps f26, 384(x15)\n"
			"fsub.pi f18, f16, f26\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 0x942d145f9e711d89\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0x56d67e18e87a7fbc\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"LBL_SEQID_6_XMA_QNT_STR_SEQ_ITER_0_HID_8:\n"
			"li x5, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x1 << TENSOR_FMA16_B_NUM_COLS) | (0x0 << TENSOR_FMA16_A_NUM_ROWS) | (0x2 << TENSOR_FMA16_A_NUM_COLS) | (0x9 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x47 << TENSOR_FMA16_SCP_LOC_B) | (0xae << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x0 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_8\n"
			"csrw tensor_error, x0\n"
			"li x5, 7\n" 
			"bne x30, x5, LBL_FAIL_HID_8\n"
			"nop \n"
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=1); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"nop \n"
			"addi x29, x29, -1\n"
			"bne x29, x0, LBL_OUTER_LOOP_SEQ_HID_8\n"
			"nop\n"          
		       VSNIP_RSV     
		);
		load_vpu_regs();
		__asm__ __volatile__ ( 
			"nop\n"          
			"nop\n"          
		       VSNIP_RSV     
		);
		trans();
		__asm__ __volatile__ ( 
			"nop\n"          
			"la x31, check_sum_hid_8\n"
			"lw x5, (x31)\n"
			"bne x30, x5, LBL_FAIL_HID_8\n"
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
			"csrrci x0, mcache_control, 3\n"
			"csrwi tensor_wait, 0x6\n" 
			"fence\n"
			"csrwi tensor_wait, 0x6\n" 
			"fence\n"
			"csrwi excl_mode, 0\n"
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=1); 
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
	if ( hid%16 == 9 ) { 
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
			"csrwi mhpmevent3, 11\n"   
			"csrwi mhpmevent4, 1\n"   
			"csrwi mhpmevent5, 6\n"   
			"csrwi mhpmevent6, 12\n"   
			"csrwi mhpmevent7, 23\n"   
			"csrwi mhpmevent8, 21\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f24,  544 (x5)\n"
			"flw.ps f27,  384 (x5)\n"
			"flw.ps f2,  416 (x5)\n"
			"flw.ps f3,  192 (x5)\n"
			"flw.ps f29,  608 (x5)\n"
			"flw.ps f6,  928 (x5)\n"
			"flw.ps f16,  768 (x5)\n"
			"flw.ps f30,  864 (x5)\n"
			"flw.ps f31,  256 (x5)\n"
			"flw.ps f22,  128 (x5)\n"
			"flw.ps f0,  288 (x5)\n"
			"flw.ps f10,  832 (x5)\n"
			"flw.ps f13,  896 (x5)\n"
			"flw.ps f26,  960 (x5)\n"
			"flw.ps f11,  224 (x5)\n"
			"flw.ps f8,  0 (x5)\n"
			"flw.ps f7,  512 (x5)\n"
			"flw.ps f9,  352 (x5)\n"
			"flw.ps f15,  320 (x5)\n"
			"flw.ps f17,  64 (x5)\n"
			"flw.ps f19,  704 (x5)\n"
			"flw.ps f5,  640 (x5)\n"
			"flw.ps f18,  800 (x5)\n"
			"flw.ps f28,  96 (x5)\n"
			"flw.ps f23,  480 (x5)\n"
			"flw.ps f12,  32 (x5)\n"
			"flw.ps f1,  992 (x5)\n"
			"flw.ps f25,  576 (x5)\n"
			"flw.ps f20,  160 (x5)\n"
			"flw.ps f14,  672 (x5)\n"
			"flw.ps f21,  448 (x5)\n"
			"flw.ps f4,  736 (x5)\n"
			"csrw fflags, zero\n"
			"csrwi frm, 0x0\n"
			"li x31, 100\n"
			"1:\n" 
			"addi x31, x31, -1\n"
			"bne x31, x0, 1b\n" 
			"csrw fcc, x0\n" 
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=1); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"LBL_KERNEL_HID_9:\n"          
			"li x5, 0xFEEDFEED\n"
			"slti x0, x0, 0x7f2\n"
			"la x31, check_sum_hid_9\n"
			"li x5, 0x7\n" 
			"sw x5, (x31)\n"
			"li x29, 0x7\n"
			"LBL_OUTER_LOOP_SEQ_HID_9:\n"
			"nop \n"
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=1); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"nop \n"
			"li x30, 0\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_0_FP_TRANS_ID_0_HID_9:\n"
			"la x26, rand_ieee754_32\n"
			"flw.ps f7, 480(x26)\n"
			"frcp.ps f1, f7\n" 
			"maskpopcz x28, m6\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_9:\n"
			"mova.m.x x12\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x6, rand_ieee754_32\n"
			"flw.ps f16, 32(x6)\n"
			"flw.ps f6, 640(x6)\n"
			"fsgnjx.ps f6, f16, f6\n" 
			"la x12, rand_int_32\n"
			"flw.ps f25, 64(x12)\n"
			"fsrli.pi f16, f25, 0xc\n" 
			"la x16, rand_int_32\n"
			"flw.ps f1, 928(x16)\n"
			"flw.ps f25, 768(x16)\n"
			"fmin.pi f4, f1, f25\n" 
			"mova.x.m x22\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f0, 192(x5)\n"
			"flw.ps f30, 384(x5)\n"
			"flw.ps f28, 288(x5)\n"
			"fmadd.s f21, f0, f30, f28, rne\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f26, 896(x13)\n"
			"flw.ps f30, 128(x13)\n"
			"fadd.s f4, f26, f30, rtz\n" 
			"la x16, rand_int_32\n"
			"flw.ps f13, 864(x16)\n"
			"flw.ps f25, 0(x16)\n"
			"fmul.pi f21, f13, f25\n" 
			"fbci.ps f26, 0x222da\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f26, 320(x16)\n"
			"flw.ps f1, 864(x16)\n"
			"fcmovm.ps f4, f26, f1\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f21, 480(x5)\n"
			"flw.ps f25, 64(x5)\n"
			"fsgnj.ps f4, f21, f25\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f13, 800(x9)\n"
			"flw.ps f8, 544(x9)\n"
			"flt.s x9, f13, f8\n" 
			"LBL_SEQID_0_M0_WRITE_ID_2_HID_9:\n"
			"maskor m0, m6, m4\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x13, rand_ieee754_32\n"
			"flw.ps f28, 256(x13)\n"
			"flw.ps f30, 896(x13)\n"
			"flw.ps f4, 928(x13)\n"
			"fnmadd.ps f26, f28, f30, f4, rmm\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_9\n"
			"addi x30, x30, 1\n"
			"la x5, rand_ieee754_32\n"
			"flw.ps f25, 192(x5)\n"
			"flw.ps f11, 768(x5)\n"
			"flw.ps f27, 384(x5)\n"
			"fnmadd.ps f1, f25, f11, f27, rdn\n" 
			"li x12, 0x821a0f374930eaa8\n"
			"fcvt.s.w f4, x12, rne\n" 
			"la x16, rand_int_32\n"
			"flw.ps f25, 992(x16)\n"
			"flw.ps f21, 128(x16)\n"
			"fsub.pi f30, f25, f21\n" 
			"LBL_SEQID_1_M0_WRITE_ID_3_HID_9:\n"
			"feqm.ps m3, f7, f22\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0x8e7d24be08\n"
			"flw.ps f5, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"mova.x.m x5\n" 
			"LBL_SEQID_1_M0_WRITE_ID_4_HID_9:\n"
			"maskxor m6, m4, m3\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x13, rand_int_32\n"
			"flw.ps f28, 384(x13)\n"
			"fandi.pi f25, f28, 0x168\n" 
			"la x19, rand_int_32\n"
			"flw.ps f27, 832(x19)\n"
			"flw.ps f25, 384(x19)\n"
			"feq.pi f28, f27, f25\n" 
			"mova.x.m x6\n" 
			"la x22, rand_int_32\n"
			"flw.ps f25, 608(x22)\n"
			"flw.ps f4, 544(x22)\n"
			"fsll.pi f13, f25, f4\n" 
			"la x6, rand_int_32\n"
			"flw.ps f21, 512(x6)\n"
			"fxor.pi f26, f21, f21\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f6, 928(x26)\n"
			"fcvt.f16.ps f25, f6\n" 
			"la x5, rand_int_32\n"
			"flw.ps f0, 704(x5)\n"
			"flw.ps f27, 32(x5)\n"
			"fdiv.pi f30, f0, f27\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f16, 64(x13)\n"
			"frsq.ps f27, f16\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f22, 480(x19)\n"
			"fswizz.ps f1, f22, 0x1b\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f27, 576(x28)\n"
			"flw.ps f8, 96(x28)\n"
			"fsgnj.ps f11, f27, f8\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_9\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_2_M0_WRITE_ID_5_HID_9:\n"
			"fltm.pi m0, f22, f8\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x26, rand_ieee754_32\n"
			"flw.ps f28, 288(x26)\n"
			"fswizz.ps f22, f28, 0x4\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f11, 736(x19)\n"
			"flw.ps f7, 608(x19)\n"
			"fsgnjn.s f25, f11, f7\n" 
			"fbci.ps f22, 0x273a\n" 
			"la x26, rand_int_32\n"
			"flw.ps f22, 96(x26)\n"
			"faddi.pi f11, f22, 0xb5\n" 
			"LBL_SEQID_2_M0_WRITE_ID_6_HID_9:\n"
			"mova.m.x x6\n" 
			"li x5, 0x80340130\n"
			"ld x5, 0(x5)\n"
			"la x5, rand_ieee754_32\n"
			"flw.ps f4, 576(x5)\n"
			"flw.ps f28, 64(x5)\n"
			"fsgnjx.s f27, f4, f28\n" 
			"LBL_SEQID_2_M0_WRITE_ID_7_HID_9:\n"
			"masknot m4, m4\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x5, rand_ieee754_32\n"
			"flw.ps f6, 576(x5)\n"
			"flw.ps f0, 160(x5)\n"
			"fmsub.s f22, f6, f0, f0, rmm\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f27, 896(x12)\n"
			"fcvt.w.s x5, f27, rmm\n" 
			"la x19, rand_int_32\n"
			"flw.ps f30, 832(x19)\n"
			"fpackreph.pi f28, f30\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f0, 672(x22)\n"
			"flw.ps f22, 96(x22)\n"
			"flw.ps f25, 928(x22)\n"
			"fnmadd.s f0, f0, f22, f25, rdn\n" 
			"fbci.pi f4, 0x1dbef\n" 
			"la x5, rand_int_32\n"
			"flw.ps f13, 608(x5)\n"
			"flw.ps f4, 704(x5)\n"
			"fsrl.pi f30, f13, f4\n" 
			"la x22, rand_int_32\n"
			"flw.ps f30, 832(x22)\n"
			"flw.ps f28, 544(x22)\n"
			"fsra.pi f30, f30, f28\n" 
			"la x26, rand_int_32\n"
			"flw.ps f7, 864(x26)\n"
			"flw.ps f27, 832(x26)\n"
			"fmaxu.pi f25, f7, f27\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 3\n" 
			"bne x30, x5, LBL_FAIL_HID_9\n"
			"nop\n"          
		       VSNIP_RSV     
		);
		load_vpu_regs();
		__asm__ __volatile__ ( 
			"nop\n"          
			"addi x30, x30, 1\n"
			"la x26, rand_ieee754_32\n"
			"flw.ps f28, 416(x26)\n"
			"flw.ps f16, 128(x26)\n"
			"fcmovm.ps f21, f28, f16\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f26, 256(x26)\n"
			"flw.ps f21, 512(x26)\n"
			"fmin.ps f22, f26, f21\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f22, 160(x13)\n"
			"flw.ps f28, 544(x13)\n"
			"feq.s x9, f22, f28\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f27, 512(x25)\n"
			"fsin.ps f27, f27\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f13, 416(x6)\n"
			"flw.ps f0, 672(x6)\n"
			"fsgnjx.ps f21, f13, f0\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f4, 736(x28)\n"
			"flw.ps f28, 736(x28)\n"
			"flt.s x16, f4, f28\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f21, 288(x6)\n"
			"fcvt.wu.s x22, f21, rup\n" 
			"la x28, rand_ieee754_16\n"
			"flw.ps f21, 896(x28)\n"
			"fcvt.ps.f16 f30, f21\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f4, 352(x6)\n"
			"flw.ps f21, 352(x6)\n"
			"fsub.ps f30, f4, f21, rtz\n" 
			"maskpopcz x13, m4\n" 
			"LBL_SEQID_3_M0_WRITE_ID_8_HID_9:\n"
			"maskand m0, m0, m0\n" 
			"li x5, 0x80340138\n"
			"ld x5, 0(x5)\n"
			"LBL_SEQID_3_FP_TRANS_ID_9_HID_9:\n"
			"la x25, rand_ieee754_32\n"
			"flw.ps f16, 576(x25)\n"
			"flog.ps f8, f16\n" 
			"la x12, rand_int_32\n"
			"flw.ps f7, 192(x12)\n"
			"flw.ps f1, 0(x12)\n"
			"fmulh.pi f25, f7, f1\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f21, 736(x6)\n"
			"fclass.ps f4, f21\n" 
			"la x9, rand_int_32\n"
			"flw.ps f28, 128(x9)\n"
			"flw.ps f11, 64(x9)\n"
			"fsrl.pi f28, f28, f11\n" 
			"fbci.ps f25, 0x7a09a\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 4\n" 
			"bne x30, x5, LBL_FAIL_HID_9\n"
			"nop\n"          
		       VSNIP_RSV     
		);
		trans();
		__asm__ __volatile__ ( 
			"nop\n"          
			"addi x30, x30, 1\n"
			"la x19, rand_int_32\n"
			"flw.ps f4, 992(x19)\n"
			"fsatu8.pi f22, f4\n" 
			"la x22, rand_int_32\n"
			"flw.ps f13, 704(x22)\n"
			"fcvt.ps.pwu f28, f13, rmm\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f1, 800(x6)\n"
			"flw.ps f21, 992(x6)\n"
			"fsgnjn.ps f0, f1, f21\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f8, 352(x28)\n"
			"flw.ps f4, 64(x28)\n"
			"fnmadd.ps f11, f8, f4, f4, rne\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f16, 160(x6)\n"
			"flw.ps f22, 224(x6)\n"
			"fle.s x9, f16, f22\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f1, 448(x5)\n"
			"fcvt.pw.ps f21, f1, rne\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f0, 288(x25)\n"
			"flw.ps f13, 800(x25)\n"
			"fmax.ps f7, f0, f13\n" 
			"la x16, rand_int_32\n"
			"flw.ps f0, 736(x16)\n"
			"flw.ps f4, 128(x16)\n"
			"fmulhu.pi f13, f0, f4\n" 
			"mova.x.m x9\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f28, 544(x6)\n"
			"flw.ps f7, 512(x6)\n"
			"fmax.s f25, f28, f7\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f8, 800(x16)\n"
			"flw.ps f7, 736(x16)\n"
			"fsub.s f1, f8, f7, rmm\n" 
			"la x16, rand_int_32\n"
			"flw.ps f4, 800(x16)\n"
			"flw.ps f30, 896(x16)\n"
			"fminu.pi f11, f4, f30\n" 
			"la x28, rand_int_32\n"
			"flw.ps f7, 224(x28)\n"
			"fnot.pi f16, f7\n" 
			"LBL_SEQID_4_M0_WRITE_ID_10_HID_9:\n"
			"feqm.ps m3, f16, f16\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"li x28, 0x433ce39b9d91b882\n"
			"fcvt.s.lu f0, x28, rne\n" 
			"LBL_SEQID_4_FP_TRANS_ID_11_HID_9:\n"
			"la x13, rand_ieee754_32\n"
			"flw.ps f16, 320(x13)\n"
			"fexp.ps f6, f16\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 5\n" 
			"bne x30, x5, LBL_FAIL_HID_9\n"
			"nop\n"          
		       VSNIP_RSV     
		);
		load_vpu_regs();
		__asm__ __volatile__ ( 
			"nop\n"          
			"addi x30, x30, 1\n"
			"la x6, rand_int_32\n"
			"flw.ps f25, 192(x6)\n"
			"flw.ps f7, 960(x6)\n"
			"fmaxu.pi f1, f25, f7\n" 
			"la x6, rand_int_32\n"
			"flw.ps f27, 384(x6)\n"
			"fsrli.pi f4, f27, 0x8\n" 
			"la x6, rand_int_32\n"
			"flw.ps f6, 864(x6)\n"
			"flw.ps f21, 0(x6)\n"
			"fminu.pi f13, f6, f21\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f28, 672(x26)\n"
			"flw.ps f1, 0(x26)\n"
			"fsgnjn.s f21, f28, f1\n" 
			"la x25, rand_int_32\n"
			"flw.ps f26, 992(x25)\n"
			"flw.ps f30, 256(x25)\n"
			"fsra.pi f11, f26, f30\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f8, 128(x26)\n"
			"flw.ps f1, 0(x26)\n"
			"feq.s x5, f8, f1\n" 
			"la x26, rand_int_32\n"
			"flw.ps f26, 544(x26)\n"
			"flw.ps f21, 608(x26)\n"
			"fsll.pi f21, f26, f21\n" 
			"la x19, rand_int_32\n"
			"flw.ps f0, 192(x19)\n"
			"flw.ps f28, 928(x19)\n"
			"fmaxu.pi f25, f0, f28\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f13, 608(x12)\n"
			"flw.ps f4, 224(x12)\n"
			"flw.ps f21, 480(x12)\n"
			"fnmadd.ps f13, f13, f4, f21, rne\n" 
			"LBL_SEQID_5_M0_WRITE_ID_12_HID_9:\n"
			"mov.m.x m4, x16, 0x45\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x16, rand_ieee754_32\n"
			"flw.ps f25, 480(x16)\n"
			"flw.ps f22, 768(x16)\n"
			"fsgnjn.ps f8, f25, f22\n" 
			"li x9, 0x2a5a19a7ba3c8fd8\n"
			"fcvt.s.lu f27, x9, rtz\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f25, 512(x6)\n"
			"fcvt.wu.s x19, f25, rtz\n" 
			"LBL_SEQID_5_FP_TRANS_ID_13_HID_9:\n"
			"la x22, rand_ieee754_32\n"
			"flw.ps f8, 704(x22)\n"
			"flog.ps f1, f8\n" 
			"la x6, rand_int_32\n"
			"flw.ps f8, 736(x6)\n"
			"flw.ps f21, 320(x6)\n"
			"flt.pi f26, f8, f21\n" 
			"la x19, rand_int_32\n"
			"flw.ps f30, 960(x19)\n"
			"flw.ps f28, 896(x19)\n"
			"fminu.pi f4, f30, f28\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 6\n" 
			"bne x30, x5, LBL_FAIL_HID_9\n"
			"addi x30, x30, 1\n"
			"la x9, rand_int_32\n"
			"flw.ps f26, 800(x9)\n"
			"fsat8.pi f30, f26\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f28, 512(x5)\n"
			"flw.ps f16, 992(x5)\n"
			"fmul.s f21, f28, f16, rmm\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f1, 960(x6)\n"
			"fclass.s x5, f1\n" 
			"la x12, rand_int_32\n"
			"flw.ps f30, 992(x12)\n"
			"faddi.pi f11, f30, 0x139\n" 
			"la x26, rand_int_32\n"
			"flw.ps f6, 224(x26)\n"
			"flw.ps f8, 512(x26)\n"
			"fle.pi f0, f6, f8\n" 
			"LBL_SEQID_6_M0_WRITE_ID_14_HID_9:\n"
			"maskor m6, m6, m6\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x6, rand_ieee754_32\n"
			"flw.ps f28, 32(x6)\n"
			"fclass.ps f4, f28\n" 
			"la x5, rand_int_32\n"
			"flw.ps f22, 96(x5)\n"
			"flw.ps f1, 928(x5)\n"
			"feq.pi f0, f22, f1\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f7, 672(x5)\n"
			"flw.ps f4, 768(x5)\n"
			"fsgnj.s f26, f7, f4\n" 
			"LBL_SEQID_6_M0_WRITE_ID_15_HID_9:\n"
			"maskand m3, m0, m6\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x13, rand_ieee754_32\n"
			"flw.ps f30, 800(x13)\n"
			"flw.ps f27, 448(x13)\n"
			"flt.ps f0, f30, f27\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f11, 480(x16)\n"
			"flw.ps f27, 128(x16)\n"
			"fsub.s f8, f11, f27, rup\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f22, 0(x6)\n"
			"flw.ps f26, 672(x6)\n"
			"flw.ps f16, 288(x6)\n"
			"fnmsub.s f28, f22, f26, f16, rdn\n" 
			"la x16, rand_int_32\n"
			"flw.ps f0, 608(x16)\n"
			"flw.ps f21, 928(x16)\n"
			"fmulhu.pi f7, f0, f21\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f25, 96(x6)\n"
			"fsin.ps f0, f25\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f6, 544(x9)\n"
			"fcvt.pwu.ps f1, f6, rup\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 7\n" 
			"bne x30, x5, LBL_FAIL_HID_9\n"
			"nop \n"
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=1); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"nop \n"
			"addi x29, x29, -1\n"
			"bne x29, x0, LBL_OUTER_LOOP_SEQ_HID_9\n"
			"nop\n"          
		       VSNIP_RSV     
		);
		load_vpu_regs();
		__asm__ __volatile__ ( 
			"nop\n"          
			"nop\n"          
		       VSNIP_RSV     
		);
		trans();
		__asm__ __volatile__ ( 
			"nop\n"          
			"la x31, check_sum_hid_9\n"
			"lw x5, (x31)\n"
			"bne x30, x5, LBL_FAIL_HID_9\n"
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
		c_sync_point(sync_odd_harts=1); 
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
	if ( hid%16 == 10 ) { 
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
			"csrwi mhpmevent3, 16\n"   
			"csrwi mhpmevent4, 6\n"   
			"csrwi mhpmevent5, 4\n"   
			"csrwi mhpmevent6, 27\n"   
			"csrwi mhpmevent7, 6\n"   
			"csrwi mhpmevent8, 3\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f14,  96 (x5)\n"
			"flw.ps f31,  192 (x5)\n"
			"flw.ps f29,  832 (x5)\n"
			"flw.ps f30,  864 (x5)\n"
			"flw.ps f26,  960 (x5)\n"
			"flw.ps f16,  160 (x5)\n"
			"flw.ps f17,  576 (x5)\n"
			"flw.ps f8,  992 (x5)\n"
			"flw.ps f11,  800 (x5)\n"
			"flw.ps f28,  32 (x5)\n"
			"flw.ps f12,  256 (x5)\n"
			"flw.ps f15,  288 (x5)\n"
			"flw.ps f20,  768 (x5)\n"
			"flw.ps f25,  448 (x5)\n"
			"flw.ps f5,  928 (x5)\n"
			"flw.ps f22,  64 (x5)\n"
			"flw.ps f9,  128 (x5)\n"
			"flw.ps f23,  224 (x5)\n"
			"flw.ps f3,  320 (x5)\n"
			"flw.ps f27,  512 (x5)\n"
			"flw.ps f13,  416 (x5)\n"
			"flw.ps f2,  0 (x5)\n"
			"flw.ps f7,  480 (x5)\n"
			"flw.ps f24,  672 (x5)\n"
			"flw.ps f4,  352 (x5)\n"
			"flw.ps f21,  544 (x5)\n"
			"flw.ps f1,  704 (x5)\n"
			"flw.ps f0,  736 (x5)\n"
			"flw.ps f6,  608 (x5)\n"
			"flw.ps f10,  896 (x5)\n"
			"flw.ps f19,  640 (x5)\n"
			"flw.ps f18,  384 (x5)\n"
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
			"csrw fcc, x0\n" 
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=1); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"LBL_KERNEL_HID_10:\n"          
			"li x5, 0xFEEDFEED\n"
			"slti x0, x0, 0x7f2\n"
			"la x31, check_sum_hid_10\n"
			"li x5, 0x7\n" 
			"sw x5, (x31)\n"
			"li x29, 0x7\n"
			"LBL_OUTER_LOOP_SEQ_HID_10:\n"
			"nop \n"
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=1); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"nop \n"
			"li x30, 0\n"
			"addi x30, x30, 1\n"
			"la x27, rand_int_32\n"
			"flw.ps f21, 416(x27)\n"
			"flw.ps f4, 96(x27)\n"
			"fmax.pi f11, f21, f4\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f6, 960(x9)\n"
			"flw.ps f4, 96(x9)\n"
			"flt.ps f15, f6, f4\n" 
			"li x5, 0xe457\n"
			"csrw tensor_mask, x5\n"	
			"la x9, rand_ieee754_32\n"
			"flw.ps f24, 288(x9)\n"
			"ffrc.ps f24, f24\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f11, 224(x27)\n"
			"flw.ps f4, 320(x27)\n"
			"flw.ps f27, 96(x27)\n"
			"fnmadd.ps f17, f11, f4, f27, rtz\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f29, 512(x13)\n"
			"flw.ps f30, 992(x13)\n"
			"feq.s x9, f29, f30\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f6, 832(x19)\n"
			"fsqrt.s f28, f6, rmm\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f28, 608(x19)\n"
			"flw.ps f13, 160(x19)\n"
			"fmul.s f17, f28, f13, rtz\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f30, 928(x26)\n"
			"flw.ps f17, 128(x26)\n"
			"fdiv.s f3, f30, f17, rup\n" 
			"la x6, rand_int_32\n"
			"flw.ps f15, 992(x6)\n"
			"flw.ps f30, 768(x6)\n"
			"feq.pi f7, f15, f30\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f11, 960(x26)\n"
			"flw.ps f27, 960(x26)\n"
			"fsgnj.ps f15, f11, f27\n" 
			"la x13, rand_int_32\n"
			"flw.ps f30, 960(x13)\n"
			"fpackreph.pi f4, f30\n" 
			"li x5, 0x4243\n"
			"csrw tensor_mask, x5\n"	
			"la x27, rand_int_32\n"
			"flw.ps f3, 416(x27)\n"
			"fcvt.ps.pw f7, f3, rtz\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f15, 160(x13)\n"
			"flw.ps f7, 64(x13)\n"
			"flw.ps f3, 672(x13)\n"
			"fmsub.s f17, f15, f7, f3, rmm\n" 
			"li x5, 0x4cf5e18d427d1838\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0x56e610d4cee72d25\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"fbci.pi f7, 0x46ebd\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_10:\n"
			"masknot m0, m1\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"li x5, 0x8489\n"
			"csrw tensor_mask, x5\n"	
			"la x5, rand_ieee754_32\n"
			"flw.ps f17, 320(x5)\n"
			"flw.ps f3, 480(x5)\n"
			"flw.ps f2, 608(x5)\n"
			"fcmov.ps f29, f17, f3, f2\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ID_992278_HID_10:\n"
			"la x31, self_check_32\n"
			"li x5, 0x6000000000000a\n"
			"add x5, x5, x31\n"
			"li x6, 0x10000000000001\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x1 << TENSOR_FMA32_B_NUM_COLS) | (0xa << TENSOR_FMA32_A_NUM_ROWS) | (0x1 << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x1 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1c << TENSOR_FMA32_SCP_LOC_B) | (0x3 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x9, ((0xa << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0xa << TENSOR_QUANT_QUANT_ROW) | (0x9 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x2 << TENSOR_QUANT_TRANSF5) | (0x7 << TENSOR_QUANT_TRANSF4) | (0x7 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0x2\n"
			"bne x5, x31, LBL_FAIL_HID_10\n"
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 10\n"
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ID_646931_HID_10:\n"
			"la x31, self_check_32\n"
			"li x5, 0x2\n"
			"add x5, x5, x31\n"
			"li x6, 0x3a0000000000009\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x3 << TENSOR_FMA32_B_NUM_COLS) | (0x2 << TENSOR_FMA32_A_NUM_ROWS) | (0x9 << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1d << TENSOR_FMA32_SCP_LOC_B) | (0x0 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0xa\n"
			"bne x5, x31, LBL_FAIL_HID_10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_10\n"
			"csrw tensor_error, x0\n"
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=0); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ITER_0_HID_10:\n"
			"la x31, self_check_8\n"
			"li x5, 0x20000000000009\n"
			"add x5, x5, x31\n"
			"li x6, 0x10000000000002\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x2 << TENSOR_IMA_B_NUM_COLS) | (0x9 << TENSOR_IMA_A_NUM_ROWS) | (0x2 << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x1 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1b << TENSOR_IMA_SCP_LOC_B) | (0x1 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x9, ((0xf << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0x9 << TENSOR_QUANT_QUANT_ROW) | (0x4 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x1 << TENSOR_QUANT_TRANSF2) | (0x3 << TENSOR_QUANT_TRANSF1) | (0x8 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fmvz.x.ps x31, f0, 0\n"
			"li x5, 0xc\n"
			"bne x5, x31, LBL_FAIL_HID_10\n"
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_10\n"
			"csrw tensor_error, x0\n"
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_10\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_1_M0_WRITE_ID_1_HID_10:\n"
			"feqm.ps m5, f30, f17\n" 
			"li x5, CREDINC2\n"
			"ld x5, 0(x5)\n"
			"li x26, 0x694e160217d653bf\n"
			"fcvt.s.w f6, x26, rdn\n" 
			"LBL_SEQID_1_M0_WRITE_ID_2_HID_10:\n"
			"mov.m.x m0, x27, 0x6e\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x18, rand_ieee754_32\n"
			"flw.ps f17, 896(x18)\n"
			"flw.ps f29, 608(x18)\n"
			"flw.ps f6, 800(x18)\n"
			"fmsub.s f17, f17, f29, f6, rmm\n" 
			"la x20, rand_int_32\n"
			"flw.ps f17, 416(x20)\n"
			"fsrli.pi f6, f17, 0x4\n" 
			"la x5, rand_int_32\n"
			"flw.ps f30, 800(x5)\n"
			"flw.ps f6, 832(x5)\n"
			"flt.pi f13, f30, f6\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f2, 768(x27)\n"
			"flw.ps f13, 544(x27)\n"
			"fsgnjn.s f28, f2, f13\n" 
			"li x5, 0x70f751f4642dc0df\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0xd4b3ef3a9665a081\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"la x26, rand_ieee754_32\n"
			"flw.ps f3, 320(x26)\n"
			"fcvt.w.s x20, f3, rup\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f6, 256(x20)\n"
			"flw.ps f3, 544(x20)\n"
			"feq.ps f22, f6, f3\n" 
			"LBL_SEQID_1_M0_WRITE_ID_3_HID_10:\n"
			"flem.ps m5, f4, f21\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"li x26, 0x3711a32b462cdb8b\n"
			"fcvt.s.wu f21, x26, rup\n" 
			"mova.x.m x9\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f21, 704(x19)\n"
			"flw.ps f24, 32(x19)\n"
			"fmin.ps f3, f21, f24\n" 
			"la x20, rand_int_32\n"
			"flw.ps f22, 0(x20)\n"
			"fpackreph.pi f2, f22\n" 
			"la x23, rand_int_32\n"
			"flw.ps f21, 928(x23)\n"
			"fsatu8.pi f24, f21\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f27, 800(x26)\n"
			"fround.ps f21, f27, rmm\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_1_RDX_ID_1_SHIRE_0_HID_10:\n"
			"li x23, 0xa0000000010000a\n"
			"csrw tensor_reduce, x23\n" 
			"csrwi tensor_wait, 9\n" 
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ID_485902_HID_10:\n"
			"li x6, ((0x12 << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0x7 << TENSOR_QUANT_QUANT_ROW) | (0xa << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x2 << TENSOR_QUANT_TRANSF8) | (0x6 << TENSOR_QUANT_TRANSF7) | (0x7 << TENSOR_QUANT_TRANSF6) | (0x7 << TENSOR_QUANT_TRANSF5) | (0x6 << TENSOR_QUANT_TRANSF4) | (0x7 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_10\n"
			"csrw tensor_error, x0\n"
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=0); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"LBL_SEQID_1_TLD_XMA_STR_SEQ_ITER_0_HID_10:\n"
			"la x31, self_check_32\n"
			"li x5, 0xa0000000000004\n"
			"add x5, x5, x31\n"
			"li x6, 0x1000000000000d\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x2 << TENSOR_FMA32_B_NUM_COLS) | (0x4 << TENSOR_FMA32_A_NUM_ROWS) | (0xd << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x1 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1b << TENSOR_FMA32_SCP_LOC_B) | (0x5 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x9, ((0xf << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0x4 << TENSOR_QUANT_QUANT_ROW) | (0x20 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x2 << TENSOR_QUANT_TRANSF4) | (0x6 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0xe\n"
			"bne x5, x31, LBL_FAIL_HID_10\n"
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 10\n"
			"LBL_SEQID_1_TLD_XMA_STR_SEQ_ITER_1_HID_10:\n"
			"la x31, self_check_16\n"
			"li x5, 0xa000000000000a\n"
			"add x5, x5, x31\n"
			"li x6, 0x10000000000002\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x2 << TENSOR_FMA16_B_NUM_COLS) | (0xa << TENSOR_FMA16_A_NUM_ROWS) | (0x2 << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x1 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1e << TENSOR_FMA16_SCP_LOC_B) | (0x5 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0x6\n"
			"bne x5, x31, LBL_FAIL_HID_10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_10\n"
			"csrw tensor_error, x0\n"
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_10\n"
			"addi x30, x30, 1\n"
			"fbci.pi f30, 0xaa93\n" 
			"la x18, rand_int_32\n"
			"flw.ps f13, 448(x18)\n"
			"fsrli.pi f28, f13, 0xd\n" 
			"LBL_SEQID_2_M0_WRITE_ID_4_HID_10:\n"
			"mova.m.x x23\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x6, rand_int_32\n"
			"flw.ps f2, 480(x6)\n"
			"fdivu.pi f29, f2, f2\n" 
			"la x6, rand_int_32\n"
			"flw.ps f4, 384(x6)\n"
			"fpackrepb.pi f2, f4\n" 
			"la x19, rand_int_32\n"
			"flw.ps f21, 288(x19)\n"
			"flw.ps f27, 192(x19)\n"
			"fmaxu.pi f28, f21, f27\n" 
			"la x23, rand_ieee754_16\n"
			"flw.ps f17, 288(x23)\n"
			"fcvt.ps.f16 f13, f17\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f6, 832(x27)\n"
			"flw.ps f11, 96(x27)\n"
			"fsub.s f7, f6, f11, rne\n" 
			"li x23, 0xc6b8b09ce0f2f078\n"
			"fcvt.s.w f7, x23, rup\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f17, 64(x18)\n"
			"flw.ps f24, 160(x18)\n"
			"fnmsub.ps f30, f17, f17, f24, rne\n" 
			"la x23, rand_int_32\n"
			"flw.ps f2, 832(x23)\n"
			"fsrai.pi f17, f2, 0xe\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f21, 352(x27)\n"
			"fsqrt.s f24, f21, rup\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f13, 480(x5)\n"
			"flw.ps f22, 864(x5)\n"
			"fle.ps f11, f13, f22\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f17, 992(x6)\n"
			"fcvt.w.s x27, f17, rtz\n" 
			"li x5, 0x6373\n"
			"csrw tensor_mask, x5\n"	
			"LBL_SEQID_2_M0_WRITE_ID_5_HID_10:\n"
			"masknot m0, m0\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0xFF\n"
			"li x5, 0xda0689287ec5e688\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0xccb171576df945d\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"li x5, 0x8a60b25856f6ad98\n"
			"fcvt.s.l f4, x5, rtz\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_2_TLD_XMA_STR_SEQ_ID_617163_HID_10:\n"
			"la x31, self_check_8\n"
			"li x5, 0x2000000000000c\n"
			"add x5, x5, x31\n"
			"li x6, 0x10000000000001\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x1 << TENSOR_IMA_B_NUM_COLS) | (0xc << TENSOR_IMA_A_NUM_ROWS) | (0x1 << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x1 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1d << TENSOR_IMA_SCP_LOC_B) | (0x1 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fmvz.x.ps x31, f0, 0\n"
			"li x5, 0x8\n"
			"bne x5, x31, LBL_FAIL_HID_10\n"
			"LBL_SEQID_2_TLD_XMA_STR_SEQ_ID_287760_HID_10:\n"
			"la x31, self_check_32\n"
			"li x5, 0xe0000000000005\n"
			"add x5, x5, x31\n"
			"li x6, 0x340000000000006\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x1 << TENSOR_FMA32_B_NUM_COLS) | (0x5 << TENSOR_FMA32_A_NUM_ROWS) | (0x6 << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1a << TENSOR_FMA32_SCP_LOC_B) | (0x7 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0x7\n"
			"bne x5, x31, LBL_FAIL_HID_10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_10\n"
			"csrw tensor_error, x0\n"
			"li x5, 0x30c52a0720d90084\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0x21d96d71556ec718\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"LBL_SEQID_2_XMA_QNT_STR_SEQ_ITER_0_HID_10:\n"
			"li x5, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x2 << TENSOR_FMA32_B_NUM_COLS) | (0x0 << TENSOR_FMA32_A_NUM_ROWS) | (0xd << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0xed << TENSOR_FMA32_SCP_LOC_B) | (0x7a << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x0 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_10\n"
			"csrw tensor_error, x0\n"
			"li x5, 3\n" 
			"bne x30, x5, LBL_FAIL_HID_10\n"
			"nop\n"          
		       VSNIP_RSV     
		);
		load_vpu_regs();
		__asm__ __volatile__ ( 
			"nop\n"          
			"addi x30, x30, 1\n"
			"la x27, rand_int_32\n"
			"flw.ps f28, 608(x27)\n"
			"flw.ps f2, 640(x27)\n"
			"flt.pi f3, f28, f2\n" 
			"LBL_SEQID_3_M0_WRITE_ID_6_HID_10:\n"
			"maskand m7, m5, m7\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x6, rand_int_32\n"
			"flw.ps f6, 800(x6)\n"
			"fsrai.pi f4, f6, 0x5\n" 
			"la x5, rand_int_32\n"
			"flw.ps f17, 928(x5)\n"
			"flw.ps f29, 128(x5)\n"
			"fsub.pi f7, f17, f29\n" 
			"la x13, rand_int_32\n"
			"flw.ps f27, 64(x13)\n"
			"flw.ps f29, 768(x13)\n"
			"fmaxu.pi f29, f27, f29\n" 
			"la x20, rand_int_32\n"
			"flw.ps f30, 32(x20)\n"
			"fslli.pi f30, f30, 0x3\n" 
			"la x18, rand_int_32\n"
			"flw.ps f6, 640(x18)\n"
			"flw.ps f15, 768(x18)\n"
			"fsll.pi f6, f6, f15\n" 
			"la x27, rand_int_32\n"
			"flw.ps f3, 640(x27)\n"
			"faddi.pi f6, f3, 0x63\n" 
			"li x5, 0xa30f\n"
			"csrw tensor_mask, x5\n"	
			"la x26, rand_ieee754_32\n"
			"flw.ps f7, 640(x26)\n"
			"fclass.ps f17, f7\n" 
			"LBL_SEQID_3_M0_WRITE_ID_7_HID_10:\n"
			"fltm.ps m7, f3, f17\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"fbci.ps f21, 0x40a73\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f7, 672(x18)\n"
			"fcvt.w.s x23, f7, rne\n" 
			"la x9, rand_int_32\n"
			"flw.ps f30, 800(x9)\n"
			"flw.ps f6, 672(x9)\n"
			"fsra.pi f27, f30, f6\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f17, 288(x5)\n"
			"fcvt.wu.s x18, f17, rne\n" 
			"la x5, rand_int_32\n"
			"flw.ps f13, 480(x5)\n"
			"flw.ps f15, 192(x5)\n"
			"fmin.pi f22, f13, f15\n" 
			"LBL_SEQID_3_M0_WRITE_ID_8_HID_10:\n"
			"mova.m.x x27\n" 
			"li x5, CREDINC0\n"
			"ld x5, 0(x5)\n"
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_3_TLD_XMA_STR_SEQ_ITER_0_HID_10:\n"
			"la x31, self_check_16\n"
			"li x5, 0x4\n"
			"add x5, x5, x31\n"
			"li x6, 0x3e0000000000005\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x2 << TENSOR_FMA16_B_NUM_COLS) | (0x4 << TENSOR_FMA16_A_NUM_ROWS) | (0x5 << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1f << TENSOR_FMA16_SCP_LOC_B) | (0x0 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0xc\n"
			"bne x5, x31, LBL_FAIL_HID_10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_10\n"
			"csrw tensor_error, x0\n"
			"li x5, 4\n" 
			"bne x30, x5, LBL_FAIL_HID_10\n"
			"nop\n"          
		       VSNIP_RSV     
		);
		trans();
		__asm__ __volatile__ ( 
			"nop\n"          
			"addi x30, x30, 1\n"
			"maskpopcz x6, m1\n" 
			"la x20, rand_int_32\n"
			"flw.ps f17, 800(x20)\n"
			"fpackreph.pi f6, f17\n" 
			"la x6, rand_int_32\n"
			"flw.ps f15, 832(x6)\n"
			"flw.ps f7, 512(x6)\n"
			"fadd.pi f2, f15, f7\n" 
			"la x9, rand_int_32\n"
			"flw.ps f30, 480(x9)\n"
			"fsrli.pi f2, f30, 0x0\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f6, 352(x27)\n"
			"flw.ps f4, 544(x27)\n"
			"fmin.ps f30, f6, f4\n" 
			"la x18, rand_int_32\n"
			"flw.ps f4, 672(x18)\n"
			"faddi.pi f28, f4, 0x82\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f17, 352(x18)\n"
			"flw.ps f29, 192(x18)\n"
			"fcmovm.ps f30, f17, f29\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f13, 928(x9)\n"
			"flw.ps f21, 352(x9)\n"
			"flt.ps f6, f13, f21\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f28, 224(x18)\n"
			"flw.ps f13, 800(x18)\n"
			"fsgnjn.s f6, f28, f13\n" 
			"LBL_SEQID_4_FP_TRANS_ID_9_HID_10:\n"
			"la x26, rand_ieee754_32\n"
			"flw.ps f22, 928(x26)\n"
			"flog.ps f4, f22\n" 
			"LBL_SEQID_4_FP_TRANS_ID_10_HID_10:\n"
			"la x19, rand_ieee754_32\n"
			"flw.ps f27, 800(x19)\n"
			"flog.ps f2, f27\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x6, rand_ieee754_32\n"
			"flw.ps f27, 0(x6)\n"
			"flw.ps f13, 640(x6)\n"
			"fle.ps f21, f27, f13\n" 
			"LBL_SEQID_4_M0_WRITE_ID_11_HID_10:\n"
			"fsetm.pi m5, f13\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x27, rand_ieee754_16\n"
			"flw.ps f13, 992(x27)\n"
			"fcvt.ps.f16 f21, f13\n" 
			"la x18, rand_int_32\n"
			"flw.ps f17, 736(x18)\n"
			"flw.ps f15, 192(x18)\n"
			"fminu.pi f27, f17, f15\n" 
			"la x5, rand_int_32\n"
			"flw.ps f28, 544(x5)\n"
			"flw.ps f22, 160(x5)\n"
			"fsub.pi f30, f28, f22\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_4_XMA_QNT_STR_SEQ_ITER_0_HID_10:\n"
			"li x5, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x2 << TENSOR_FMA32_B_NUM_COLS) | (0xc << TENSOR_FMA32_A_NUM_ROWS) | (0xd << TENSOR_FMA32_A_NUM_COLS) | (0xf << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0xf << TENSOR_FMA32_SCP_LOC_B) | (0xa5 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x6, ((0xe << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0xc << TENSOR_QUANT_QUANT_ROW) | (0x35 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x2 << TENSOR_QUANT_TRANSF7) | (0x7 << TENSOR_QUANT_TRANSF6) | (0x6 << TENSOR_QUANT_TRANSF5) | (0x7 << TENSOR_QUANT_TRANSF4) | (0x6 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 7\n"
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_10\n"
			"csrw tensor_error, x0\n"
			"li x5, 5\n" 
			"bne x30, x5, LBL_FAIL_HID_10\n"
			"nop\n"          
		       VSNIP_RSV     
		);
		load_vpu_regs();
		__asm__ __volatile__ ( 
			"nop\n"          
			"addi x30, x30, 1\n"
			"la x26, rand_int_32\n"
			"flw.ps f30, 0(x26)\n"
			"fpackreph.pi f2, f30\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f28, 288(x6)\n"
			"fsgnjn.s f15, f28, f28\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f21, 384(x5)\n"
			"flw.ps f6, 32(x5)\n"
			"flw.ps f27, 448(x5)\n"
			"fnmadd.s f29, f21, f6, f27, rtz\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f11, 704(x20)\n"
			"fcvt.w.s x27, f11, rdn\n" 
			"LBL_SEQID_5_M0_WRITE_ID_12_HID_10:\n"
			"maskand m5, m0, m0\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_5_M0_WRITE_ID_13_HID_10:\n"
			"fltm.pi m1, f24, f27\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x19, rand_int_32\n"
			"flw.ps f21, 256(x19)\n"
			"flw.ps f2, 352(x19)\n"
			"feq.pi f21, f21, f2\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x18, rand_ieee754_32\n"
			"flw.ps f24, 704(x18)\n"
			"flw.ps f29, 992(x18)\n"
			"fcmovm.ps f3, f24, f29\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f24, 32(x9)\n"
			"flw.ps f28, 480(x9)\n"
			"fadd.ps f2, f24, f28, rdn\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f4, 288(x19)\n"
			"flw.ps f22, 896(x19)\n"
			"flw.ps f24, 128(x19)\n"
			"fnmadd.s f13, f4, f22, f24, rmm\n" 
			"LBL_SEQID_5_M0_WRITE_ID_14_HID_10:\n"
			"maskand m0, m5, m5\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x27, rand_ieee754_32\n"
			"flw.ps f3, 384(x27)\n"
			"fmul.ps f24, f3, f3, rdn\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f2, 32(x5)\n"
			"flw.ps f6, 640(x5)\n"
			"fcmov.ps f15, f2, f6, f6\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f24, 960(x13)\n"
			"feq.ps f29, f24, f24\n" 
			"la x5, rand_int_32\n"
			"flw.ps f7, 320(x5)\n"
			"fsrl.pi f30, f7, f7\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x20, rand_ieee754_32\n"
			"flw.ps f6, 64(x20)\n"
			"fclass.ps f24, f6\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_5_XMA_QNT_STR_SEQ_ITER_0_HID_10:\n"
			"li x6, ((0x1 << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0x6 << TENSOR_QUANT_QUANT_ROW) | (0x1f << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_10\n"
			"csrw tensor_error, x0\n"
			"li x5, 6\n" 
			"bne x30, x5, LBL_FAIL_HID_10\n"
			"addi x30, x30, 1\n"
			"la x20, rand_int_32\n"
			"flw.ps f3, 0(x20)\n"
			"flw.ps f11, 320(x20)\n"
			"fmul.pi f17, f3, f11\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f21, 576(x27)\n"
			"flw.ps f6, 448(x27)\n"
			"fmul.s f7, f21, f6, rtz\n" 
			"LBL_SEQID_6_M0_WRITE_ID_15_HID_10:\n"
			"maskand m7, m1, m1\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x19, rand_ieee754_32\n"
			"flw.ps f15, 160(x19)\n"
			"flw.ps f28, 704(x19)\n"
			"fmin.s f11, f15, f28\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f24, 768(x19)\n"
			"flw.ps f7, 128(x19)\n"
			"flt.s x19, f24, f7\n" 
			"la x23, rand_int_32\n"
			"flw.ps f11, 928(x23)\n"
			"flw.ps f21, 416(x23)\n"
			"fsub.pi f24, f11, f21\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f24, 416(x9)\n"
			"flw.ps f21, 128(x9)\n"
			"flt.ps f30, f24, f21\n" 
			"LBL_SEQID_6_M0_WRITE_ID_16_HID_10:\n"
			"mova.m.x x27\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x6, rand_ieee754_32\n"
			"flw.ps f28, 512(x6)\n"
			"flw.ps f4, 896(x6)\n"
			"fadd.s f21, f28, f4, rtz\n" 
			"la x23, rand_int_32\n"
			"flw.ps f6, 928(x23)\n"
			"flw.ps f30, 288(x23)\n"
			"feq.pi f27, f6, f30\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f24, 448(x27)\n"
			"flw.ps f4, 384(x27)\n"
			"fmax.s f30, f24, f4\n" 
			"la x6, rand_int_32\n"
			"flw.ps f28, 448(x6)\n"
			"flw.ps f15, 768(x6)\n"
			"fle.pi f30, f28, f15\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f27, 288(x18)\n"
			"flw.ps f28, 928(x18)\n"
			"fsgnjn.s f28, f27, f28\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x27, rand_ieee754_32\n"
			"flw.ps f6, 608(x27)\n"
			"flw.ps f21, 64(x27)\n"
			"fmul.ps f17, f6, f21, rne\n" 
			"la x26, rand_int_32\n"
			"flw.ps f3, 480(x26)\n"
			"fcvt.ps.pw f7, f3, rdn\n" 
			"la x26, rand_int_32\n"
			"flw.ps f22, 96(x26)\n"
			"flw.ps f30, 0(x26)\n"
			"fminu.pi f30, f22, f30\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 0x4bf\n"
			"csrw tensor_mask, x5\n"	
			"LBL_SEQID_6_XMA_QNT_STR_SEQ_ITER_0_HID_10:\n"
			"li x5, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x2 << TENSOR_FMA32_B_NUM_COLS) | (0x8 << TENSOR_FMA32_A_NUM_ROWS) | (0x9 << TENSOR_FMA32_A_NUM_COLS) | (0x9 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0xda << TENSOR_FMA32_SCP_LOC_B) | (0xff << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_10\n"
			"csrw tensor_error, x0\n"
			"li x5, 7\n" 
			"bne x30, x5, LBL_FAIL_HID_10\n"
			"nop \n"
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=1); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"nop \n"
			"addi x29, x29, -1\n"
			"bne x29, x0, LBL_OUTER_LOOP_SEQ_HID_10\n"
			"nop\n"          
		       VSNIP_RSV     
		);
		load_vpu_regs();
		__asm__ __volatile__ ( 
			"nop\n"          
			"nop\n"          
		       VSNIP_RSV     
		);
		trans();
		__asm__ __volatile__ ( 
			"nop\n"          
			"la x31, check_sum_hid_10\n"
			"lw x5, (x31)\n"
			"bne x30, x5, LBL_FAIL_HID_10\n"
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
			"csrwi tensor_wait, 0x6\n" 
			"fence\n"
			"csrwi excl_mode, 0\n"
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=1); 
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
	if ( hid%16 == 11 ) { 
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
			"csrwi mhpmevent3, 11\n"   
			"csrwi mhpmevent4, 1\n"   
			"csrwi mhpmevent5, 6\n"   
			"csrwi mhpmevent6, 12\n"   
			"csrwi mhpmevent7, 23\n"   
			"csrwi mhpmevent8, 21\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f15,  928 (x5)\n"
			"flw.ps f25,  832 (x5)\n"
			"flw.ps f4,  448 (x5)\n"
			"flw.ps f5,  960 (x5)\n"
			"flw.ps f7,  992 (x5)\n"
			"flw.ps f28,  160 (x5)\n"
			"flw.ps f16,  288 (x5)\n"
			"flw.ps f24,  736 (x5)\n"
			"flw.ps f30,  512 (x5)\n"
			"flw.ps f1,  32 (x5)\n"
			"flw.ps f19,  800 (x5)\n"
			"flw.ps f3,  320 (x5)\n"
			"flw.ps f17,  640 (x5)\n"
			"flw.ps f13,  352 (x5)\n"
			"flw.ps f10,  256 (x5)\n"
			"flw.ps f6,  224 (x5)\n"
			"flw.ps f0,  608 (x5)\n"
			"flw.ps f31,  128 (x5)\n"
			"flw.ps f21,  896 (x5)\n"
			"flw.ps f23,  672 (x5)\n"
			"flw.ps f20,  0 (x5)\n"
			"flw.ps f27,  576 (x5)\n"
			"flw.ps f11,  384 (x5)\n"
			"flw.ps f14,  864 (x5)\n"
			"flw.ps f29,  64 (x5)\n"
			"flw.ps f8,  416 (x5)\n"
			"flw.ps f9,  192 (x5)\n"
			"flw.ps f26,  544 (x5)\n"
			"flw.ps f12,  480 (x5)\n"
			"flw.ps f2,  768 (x5)\n"
			"flw.ps f18,  704 (x5)\n"
			"flw.ps f22,  96 (x5)\n"
			"csrw fflags, zero\n"
			"csrwi frm, 0x0\n"
			"li x31, 100\n"
			"1:\n" 
			"addi x31, x31, -1\n"
			"bne x31, x0, 1b\n" 
			"csrw fcc, x0\n" 
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=1); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"LBL_KERNEL_HID_11:\n"          
			"li x5, 0xFEEDFEED\n"
			"slti x0, x0, 0x7f2\n"
			"la x31, check_sum_hid_11\n"
			"li x5, 0x7\n" 
			"sw x5, (x31)\n"
			"li x29, 0x7\n"
			"LBL_OUTER_LOOP_SEQ_HID_11:\n"
			"nop \n"
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=1); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"nop \n"
			"li x30, 0\n"
			"addi x30, x30, 1\n"
			"la x11, rand_int_32\n"
			"flw.ps f22, 896(x11)\n"
			"fslli.pi f13, f22, 0x2\n" 
			"la x18, rand_int_32\n"
			"flw.ps f4, 544(x18)\n"
			"fcvt.ps.pwu f22, f4, rdn\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_11:\n"
			"feqm.ps m7, f17, f16\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x13, rand_int_32\n"
			"flw.ps f0, 512(x13)\n"
			"flw.ps f2, 992(x13)\n"
			"fmul.pi f20, f0, f2\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_11:\n"
			"fltm.pi m2, f27, f16\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0x9a0376bd28\n"
			"fsw.ps f0, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x23, rand_ieee754_32\n"
			"flw.ps f6, 608(x23)\n"
			"flw.ps f0, 896(x23)\n"
			"fmul.ps f4, f6, f0, rtz\n" 
			"la x23, rand_int_32\n"
			"flw.ps f4, 416(x23)\n"
			"flw.ps f16, 960(x23)\n"
			"flt.pi f19, f4, f16\n" 
			"la x23, rand_int_32\n"
			"flw.ps f7, 928(x23)\n"
			"fcvt.ps.pw f20, f7, rdn\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f0, 864(x27)\n"
			"fclass.ps f24, f0\n" 
			"LBL_SEQID_0_FP_TRANS_ID_2_HID_11:\n"
			"la x13, rand_ieee754_32\n"
			"flw.ps f27, 928(x13)\n"
			"flog.ps f7, f27\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f9, 928(x11)\n"
			"fsgnjn.s f9, f9, f9\n" 
			"la x23, rand_int_32\n"
			"flw.ps f4, 864(x23)\n"
			"flw.ps f9, 448(x23)\n"
			"fmaxu.pi f2, f4, f9\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f20, 256(x7)\n"
			"fcvt.f16.ps f9, f20\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f4, 224(x7)\n"
			"flw.ps f24, 640(x7)\n"
			"flt.s x23, f4, f24\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f19, 640(x23)\n"
			"fcvt.pw.ps f20, f19, rdn\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f19, 64(x27)\n"
			"flw.ps f22, 896(x27)\n"
			"fsgnjx.s f24, f19, f22\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_11\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_1_FP_TRANS_ID_3_HID_11:\n"
			"la x16, rand_ieee754_32\n"
			"flw.ps f20, 96(x16)\n"
			"flog.ps f7, f20\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f6, 608(x7)\n"
			"flw.ps f9, 864(x7)\n"
			"feq.ps f30, f6, f9\n" 
			"la x16, rand_int_32\n"
			"flw.ps f9, 0(x16)\n"
			"flw.ps f30, 64(x16)\n"
			"fsra.pi f7, f9, f30\n" 
			"LBL_SEQID_1_M0_WRITE_ID_4_HID_11:\n"
			"maskxor m2, m1, m7\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x18, rand_ieee754_32\n"
			"flw.ps f20, 608(x18)\n"
			"fcvt.pw.ps f24, f20, rne\n" 
			"la x7, rand_int_32\n"
			"flw.ps f19, 320(x7)\n"
			"flw.ps f30, 544(x7)\n"
			"fsra.pi f9, f19, f30\n" 
			"la x11, rand_int_32\n"
			"flw.ps f7, 960(x11)\n"
			"flw.ps f16, 992(x11)\n"
			"fadd.pi f24, f7, f16\n" 
			"la x7, rand_int_32\n"
			"flw.ps f9, 800(x7)\n"
			"flw.ps f17, 960(x7)\n"
			"fmulh.pi f19, f9, f17\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f9, 992(x6)\n"
			"flw.ps f0, 576(x6)\n"
			"fmax.s f6, f9, f0\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f9, 992(x13)\n"
			"fcvt.lu.s x26, f9, rtz\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f20, 736(x11)\n"
			"flw.ps f2, 256(x11)\n"
			"fsgnjx.s f9, f20, f2\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f19, 352(x13)\n"
			"flw.ps f30, 128(x13)\n"
			"fmul.ps f19, f19, f30, rtz\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f27, 992(x27)\n"
			"fswizz.ps f20, f27, 0xd\n" 
			"la x7, rand_int_32\n"
			"flw.ps f13, 512(x7)\n"
			"fsatu8.pi f7, f13\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f6, 96(x10)\n"
			"fclass.ps f2, f6\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f0, 416(x27)\n"
			"flw.ps f9, 832(x27)\n"
			"fle.ps f6, f0, f9\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_11\n"
			"addi x30, x30, 1\n"
			"la x10, rand_int_32\n"
			"flw.ps f4, 896(x10)\n"
			"flw.ps f30, 416(x10)\n"
			"flt.pi f13, f4, f30\n" 
			"li x23, 0xa10abeea1be77fe9\n"
			"fcvt.s.lu f16, x23, rne\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f19, 0(x23)\n"
			"fround.ps f0, f19, rtz\n" 
			"la x23, rand_int_32\n"
			"flw.ps f20, 192(x23)\n"
			"flw.ps f16, 256(x23)\n"
			"fsrl.pi f27, f20, f16\n" 
			"fbci.ps f5, 0xca62\n" 
			"la x6, rand_int_32\n"
			"flw.ps f7, 608(x6)\n"
			"flw.ps f27, 768(x6)\n"
			"fmax.pi f27, f7, f27\n" 
			"la x11, rand_int_32\n"
			"flw.ps f5, 0(x11)\n"
			"fsrli.pi f5, f5, 0x1\n" 
			"LBL_SEQID_2_FP_TRANS_ID_5_HID_11:\n"
			"la x10, rand_ieee754_32\n"
			"flw.ps f0, 352(x10)\n"
			"flog.ps f5, f0\n" 
			"LBL_SEQID_2_M0_WRITE_ID_6_HID_11:\n"
			"maskor m1, m5, m1\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_2_M0_WRITE_ID_7_HID_11:\n"
			"fsetm.pi m5, f9\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x10, rand_ieee754_32\n"
			"flw.ps f19, 992(x10)\n"
			"frsq.ps f0, f19\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f22, 0(x27)\n"
			"fcvt.pw.ps f13, f22, rmm\n" 
			"la x16, rand_int_32\n"
			"flw.ps f13, 960(x16)\n"
			"fxor.pi f6, f13, f13\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f6, 800(x13)\n"
			"flw.ps f5, 224(x13)\n"
			"flw.ps f22, 992(x13)\n"
			"fmsub.ps f2, f6, f5, f22, rmm\n" 
			"LBL_SEQID_2_FP_TRANS_ID_8_HID_11:\n"
			"la x10, rand_ieee754_32\n"
			"flw.ps f4, 544(x10)\n"
			"flog.ps f24, f4\n" 
			"li x10, 0x9bdbd7f92b291261\n"
			"fcvt.s.w f16, x10, rne\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 3\n" 
			"bne x30, x5, LBL_FAIL_HID_11\n"
			"nop\n"          
		       VSNIP_RSV     
		);
		load_vpu_regs();
		__asm__ __volatile__ ( 
			"nop\n"          
			"addi x30, x30, 1\n"
			"LBL_SEQID_3_M0_WRITE_ID_9_HID_11:\n"
			"maskand m5, m7, m5\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"mova.x.m x16\n" 
			"la x27, rand_int_32\n"
			"flw.ps f22, 192(x27)\n"
			"flw.ps f0, 576(x27)\n"
			"fadd.pi f5, f22, f0\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f9, 512(x23)\n"
			"flw.ps f16, 256(x23)\n"
			"flw.ps f22, 672(x23)\n"
			"fcmov.ps f24, f9, f16, f22\n" 
			"la x16, rand_int_32\n"
			"flw.ps f2, 64(x16)\n"
			"fandi.pi f19, f2, 0x139\n" 
			"la x6, rand_int_32\n"
			"flw.ps f19, 928(x6)\n"
			"fsat8.pi f19, f19\n" 
			"LBL_SEQID_3_M0_WRITE_ID_10_HID_11:\n"
			"maskand m2, m2, m2\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_3_M0_WRITE_ID_11_HID_11:\n"
			"maskand m2, m2, m2\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x7, rand_ieee754_32\n"
			"flw.ps f6, 608(x7)\n"
			"flw.ps f27, 928(x7)\n"
			"fmul.ps f13, f6, f27, rup\n" 
			"fbci.ps f4, 0x10ad0\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f22, 992(x7)\n"
			"flw.ps f0, 352(x7)\n"
			"flw.ps f7, 0(x7)\n"
			"fnmsub.s f13, f22, f0, f7, rdn\n" 
			"la x16, rand_int_32\n"
			"flw.ps f4, 640(x16)\n"
			"faddi.pi f17, f4, 0x1ec\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f27, 768(x6)\n"
			"flw.ps f0, 64(x6)\n"
			"fmin.ps f7, f27, f0\n" 
			"la x13, rand_int_32\n"
			"flw.ps f2, 352(x13)\n"
			"fpackreph.pi f13, f2\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f13, 864(x13)\n"
			"flw.ps f16, 192(x13)\n"
			"feq.ps f22, f13, f16\n" 
			"la x18, rand_int_32\n"
			"flw.ps f20, 480(x18)\n"
			"fandi.pi f9, f20, 0x170\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 4\n" 
			"bne x30, x5, LBL_FAIL_HID_11\n"
			"nop\n"          
		       VSNIP_RSV     
		);
		trans();
		__asm__ __volatile__ ( 
			"nop\n"          
			"addi x30, x30, 1\n"
			"la x11, rand_int_32\n"
			"flw.ps f5, 0(x11)\n"
			"flw.ps f30, 480(x11)\n"
			"fsub.pi f0, f5, f30\n" 
			"li x16, 0xaa9d108097938cd3\n"
			"fcvt.s.w f2, x16, rne\n" 
			"la x23, rand_int_32\n"
			"flw.ps f16, 544(x23)\n"
			"flw.ps f30, 992(x23)\n"
			"fxor.pi f0, f16, f30\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f24, 640(x7)\n"
			"ffrc.ps f4, f24\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f9, 512(x11)\n"
			"fcvt.w.s x16, f9, rne\n" 
			"LBL_SEQID_4_FP_TRANS_ID_12_HID_11:\n"
			"la x11, rand_ieee754_32\n"
			"flw.ps f13, 672(x11)\n"
			"frcp.ps f27, f13\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f9, 448(x26)\n"
			"fswizz.ps f19, f9, 0x18\n" 
			"la x13, rand_int_32\n"
			"flw.ps f0, 928(x13)\n"
			"flw.ps f9, 672(x13)\n"
			"for.pi f6, f0, f9\n" 
			"la x26, rand_int_32\n"
			"flw.ps f0, 736(x26)\n"
			"faddi.pi f13, f0, 0x83\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f6, 704(x13)\n"
			"fcvt.pwu.ps f20, f6, rmm\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f19, 448(x13)\n"
			"flw.ps f6, 768(x13)\n"
			"flw.ps f5, 288(x13)\n"
			"fmadd.ps f6, f19, f6, f5, rne\n" 
			"LBL_SEQID_4_M0_WRITE_ID_13_HID_11:\n"
			"masknot m5, m5\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x16, rand_int_32\n"
			"flw.ps f24, 96(x16)\n"
			"fltu.pi f0, f24, f24\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f24, 128(x16)\n"
			"frsq.ps f24, f24\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f22, 736(x6)\n"
			"flw.ps f5, 640(x6)\n"
			"flw.ps f7, 224(x6)\n"
			"fmsub.ps f7, f22, f5, f7, rmm\n" 
			"mova.x.m x23\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 5\n" 
			"bne x30, x5, LBL_FAIL_HID_11\n"
			"nop\n"          
		       VSNIP_RSV     
		);
		load_vpu_regs();
		__asm__ __volatile__ ( 
			"nop\n"          
			"addi x30, x30, 1\n"
			"la x11, rand_ieee754_32\n"
			"flw.ps f22, 896(x11)\n"
			"flw.ps f19, 0(x11)\n"
			"flw.ps f9, 704(x11)\n"
			"fnmadd.s f16, f22, f19, f9, rmm\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f6, 928(x6)\n"
			"fsin.ps f24, f6\n" 
			"LBL_SEQID_5_M0_WRITE_ID_14_HID_11:\n"
			"fltm.pi m7, f17, f19\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x11, rand_ieee754_32\n"
			"flw.ps f7, 928(x11)\n"
			"fcvt.w.s x26, f7, rmm\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f2, 928(x6)\n"
			"flw.ps f19, 896(x6)\n"
			"fadd.s f20, f2, f19, rmm\n" 
			"LBL_SEQID_5_FP_TRANS_ID_15_HID_11:\n"
			"la x18, rand_ieee754_32\n"
			"flw.ps f6, 992(x18)\n"
			"flog.ps f2, f6\n" 
			"la x7, rand_int_32\n"
			"flw.ps f19, 928(x7)\n"
			"flw.ps f0, 0(x7)\n"
			"fminu.pi f0, f19, f0\n" 
			"LBL_SEQID_5_M0_WRITE_ID_16_HID_11:\n"
			"masknot m2, m5\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x23, rand_ieee754_32\n"
			"flw.ps f22, 576(x23)\n"
			"fclass.s x16, f22\n" 
			"la x18, rand_int_32\n"
			"flw.ps f27, 128(x18)\n"
			"flw.ps f20, 736(x18)\n"
			"fmulh.pi f4, f27, f20\n" 
			"la x26, rand_int_32\n"
			"flw.ps f16, 608(x26)\n"
			"fsrai.pi f6, f16, 0xd\n" 
			"la x27, rand_int_32\n"
			"flw.ps f5, 384(x27)\n"
			"flw.ps f20, 320(x27)\n"
			"fminu.pi f19, f5, f20\n" 
			"la x16, rand_int_32\n"
			"flw.ps f17, 768(x16)\n"
			"fxor.pi f17, f17, f17\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f7, 672(x6)\n"
			"flw.ps f19, 416(x6)\n"
			"feq.ps f2, f7, f19\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f22, 96(x16)\n"
			"fcvt.pw.ps f30, f22, rtz\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f2, 448(x26)\n"
			"flw.ps f17, 544(x26)\n"
			"flw.ps f13, 96(x26)\n"
			"fnmsub.ps f27, f2, f17, f13, rtz\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 6\n" 
			"bne x30, x5, LBL_FAIL_HID_11\n"
			"addi x30, x30, 1\n"
			"la x26, rand_int_32\n"
			"flw.ps f17, 928(x26)\n"
			"fnot.pi f4, f17\n" 
			"la x11, rand_int_32\n"
			"flw.ps f13, 128(x11)\n"
			"fpackrepb.pi f30, f13\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f0, 992(x23)\n"
			"flw.ps f13, 864(x23)\n"
			"flw.ps f24, 512(x23)\n"
			"fmsub.s f13, f0, f13, f24, rtz\n" 
			"la x13, rand_int_32\n"
			"flw.ps f7, 128(x13)\n"
			"flw.ps f30, 960(x13)\n"
			"feq.pi f13, f7, f30\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f20, 64(x11)\n"
			"flw.ps f2, 352(x11)\n"
			"flw.ps f9, 288(x11)\n"
			"fnmsub.ps f24, f20, f2, f9, rne\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f5, 832(x6)\n"
			"fclass.s x26, f5\n" 
			"la x26, rand_int_32\n"
			"flw.ps f13, 608(x26)\n"
			"flw.ps f27, 736(x26)\n"
			"fsrl.pi f27, f13, f27\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f6, 96(x27)\n"
			"flw.ps f13, 64(x27)\n"
			"flw.ps f24, 352(x27)\n"
			"fmsub.ps f17, f6, f13, f24, rdn\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f16, 128(x27)\n"
			"fcvt.pw.ps f17, f16, rup\n" 
			"la x7, rand_int_32\n"
			"flw.ps f30, 896(x7)\n"
			"flw.ps f16, 544(x7)\n"
			"fsrl.pi f17, f30, f16\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f13, 832(x23)\n"
			"flw.ps f0, 384(x23)\n"
			"fmax.s f24, f13, f0\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f9, 160(x11)\n"
			"flw.ps f2, 0(x11)\n"
			"fmax.ps f22, f9, f2\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f2, 736(x11)\n"
			"fcvt.f16.ps f4, f2\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f22, 544(x13)\n"
			"flw.ps f4, 32(x13)\n"
			"flw.ps f17, 704(x13)\n"
			"fnmadd.ps f7, f22, f4, f17, rne\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f22, 224(x10)\n"
			"flw.ps f7, 736(x10)\n"
			"fcmovm.ps f16, f22, f7\n" 
			"la x11, rand_int_32\n"
			"flw.ps f5, 64(x11)\n"
			"flw.ps f2, 928(x11)\n"
			"for.pi f5, f5, f2\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 7\n" 
			"bne x30, x5, LBL_FAIL_HID_11\n"
			"nop \n"
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=1); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"nop \n"
			"addi x29, x29, -1\n"
			"bne x29, x0, LBL_OUTER_LOOP_SEQ_HID_11\n"
			"nop\n"          
		       VSNIP_RSV     
		);
		load_vpu_regs();
		__asm__ __volatile__ ( 
			"nop\n"          
			"nop\n"          
		       VSNIP_RSV     
		);
		trans();
		__asm__ __volatile__ ( 
			"nop\n"          
			"la x31, check_sum_hid_11\n"
			"lw x5, (x31)\n"
			"bne x30, x5, LBL_FAIL_HID_11\n"
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
		c_sync_point(sync_odd_harts=1); 
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
	if ( hid%16 == 12 ) { 
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
			"csrwi mhpmevent3, 16\n"   
			"csrwi mhpmevent4, 6\n"   
			"csrwi mhpmevent5, 4\n"   
			"csrwi mhpmevent6, 27\n"   
			"csrwi mhpmevent7, 6\n"   
			"csrwi mhpmevent8, 3\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f0,  192 (x5)\n"
			"flw.ps f11,  960 (x5)\n"
			"flw.ps f7,  384 (x5)\n"
			"flw.ps f26,  320 (x5)\n"
			"flw.ps f23,  288 (x5)\n"
			"flw.ps f16,  544 (x5)\n"
			"flw.ps f3,  640 (x5)\n"
			"flw.ps f2,  896 (x5)\n"
			"flw.ps f14,  608 (x5)\n"
			"flw.ps f18,  480 (x5)\n"
			"flw.ps f29,  448 (x5)\n"
			"flw.ps f4,  992 (x5)\n"
			"flw.ps f13,  672 (x5)\n"
			"flw.ps f12,  256 (x5)\n"
			"flw.ps f8,  64 (x5)\n"
			"flw.ps f15,  832 (x5)\n"
			"flw.ps f24,  416 (x5)\n"
			"flw.ps f6,  928 (x5)\n"
			"flw.ps f1,  0 (x5)\n"
			"flw.ps f30,  160 (x5)\n"
			"flw.ps f19,  96 (x5)\n"
			"flw.ps f5,  704 (x5)\n"
			"flw.ps f31,  224 (x5)\n"
			"flw.ps f27,  512 (x5)\n"
			"flw.ps f22,  864 (x5)\n"
			"flw.ps f17,  128 (x5)\n"
			"flw.ps f28,  32 (x5)\n"
			"flw.ps f10,  736 (x5)\n"
			"flw.ps f21,  576 (x5)\n"
			"flw.ps f9,  768 (x5)\n"
			"flw.ps f25,  800 (x5)\n"
			"flw.ps f20,  352 (x5)\n"
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
			"csrw fcc, x0\n" 
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=1); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"LBL_KERNEL_HID_12:\n"          
			"li x5, 0xFEEDFEED\n"
			"slti x0, x0, 0x7f2\n"
			"la x31, check_sum_hid_12\n"
			"li x5, 0x7\n" 
			"sw x5, (x31)\n"
			"li x29, 0x7\n"
			"LBL_OUTER_LOOP_SEQ_HID_12:\n"
			"nop \n"
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=1); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"nop \n"
			"li x30, 0\n"
			"addi x30, x30, 1\n"
			"la x12, rand_ieee754_32\n"
			"flw.ps f10, 352(x12)\n"
			"flw.ps f20, 128(x12)\n"
			"fsgnj.ps f3, f10, f20\n" 
			"la x22, rand_int_32\n"
			"flw.ps f18, 96(x22)\n"
			"flw.ps f21, 672(x22)\n"
			"fltu.pi f3, f18, f21\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_12:\n"
			"maskor m0, m0, m2\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x20, rand_ieee754_32\n"
			"flw.ps f22, 64(x20)\n"
			"flw.ps f11, 992(x20)\n"
			"fmin.s f11, f22, f11\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f10, 32(x22)\n"
			"flw.ps f1, 352(x22)\n"
			"flw.ps f21, 512(x22)\n"
			"fmadd.ps f1, f10, f1, f21, rdn\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_12:\n"
			"masknot m0, m0\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_0_M0_WRITE_ID_2_HID_12:\n"
			"flem.ps m7, f18, f8\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"maskpopcz x26, m2\n" 
			"la x28, rand_int_32\n"
			"flw.ps f14, 672(x28)\n"
			"flw.ps f7, 0(x28)\n"
			"fmaxu.pi f0, f14, f7\n" 
			"la x20, rand_int_32\n"
			"flw.ps f18, 0(x20)\n"
			"fpackrepb.pi f12, f18\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f23, 768(x28)\n"
			"flw.ps f18, 96(x28)\n"
			"flw.ps f8, 192(x28)\n"
			"fmsub.ps f1, f23, f18, f8, rne\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f21, 992(x22)\n"
			"flw.ps f22, 928(x22)\n"
			"fsgnjx.ps f3, f21, f22\n" 
			"li x5, 0x6bb3\n"
			"csrw tensor_mask, x5\n"	
			"LBL_SEQID_0_M0_WRITE_ID_3_HID_12:\n"
			"mov.m.x m4, x11, 0x37\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x19, rand_ieee754_32\n"
			"flw.ps f10, 992(x19)\n"
			"flw.ps f23, 288(x19)\n"
			"fcmov.ps f23, f10, f23, f10\n" 
			"mova.x.m x22\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f1, 544(x6)\n"
			"flw.ps f10, 480(x6)\n"
			"fdiv.s f23, f1, f10, rmm\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_0_RDX_ID_0_SHIRE_0_HID_12:\n"
			"li x12, 0x2c00000002110001\n"
			"csrr x6, mhartid\n"
			"srli x6, x6, 4\n"
			"slli x6, x6, 6\n"
			"add x12, x12, x6\n"
			"csrw tensor_reduce, x12\n" 
			"csrwi tensor_wait, 9\n" 
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=0); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"li x5, 0xabc3\n"
			"csrw tensor_mask, x5\n"	
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ITER_0_HID_12:\n"
			"li x6, ((0x1a << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0x9 << TENSOR_QUANT_QUANT_ROW) | (0x18 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x7 << TENSOR_QUANT_TRANSF6) | (0x7 << TENSOR_QUANT_TRANSF5) | (0x7 << TENSOR_QUANT_TRANSF4) | (0x7 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ITER_1_HID_12:\n"
			"li x6, ((0x17 << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0x8 << TENSOR_QUANT_QUANT_ROW) | (0x2d << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0xa << TENSOR_QUANT_TRANSF1) | (0x9 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ITER_2_HID_12:\n"
			"li x6, ((0x14 << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0xa << TENSOR_QUANT_QUANT_ROW) | (0x18 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x1 << TENSOR_QUANT_TRANSF6) | (0xa << TENSOR_QUANT_TRANSF5) | (0x3 << TENSOR_QUANT_TRANSF4) | (0x5 << TENSOR_QUANT_TRANSF3) | (0x3 << TENSOR_QUANT_TRANSF2) | (0x9 << TENSOR_QUANT_TRANSF1) | (0x5 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_12\n"
			"csrw tensor_error, x0\n"
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_12\n"
			"addi x30, x30, 1\n"
			"la x12, rand_int_32\n"
			"flw.ps f29, 480(x12)\n"
			"flw.ps f18, 608(x12)\n"
			"fsll.pi f7, f29, f18\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f21, 64(x11)\n"
			"fcvt.pw.ps f11, f21, rup\n" 
			"LBL_SEQID_1_M0_WRITE_ID_4_HID_12:\n"
			"flem.ps m4, f23, f8\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x20, rand_ieee754_32\n"
			"flw.ps f14, 384(x20)\n"
			"flw.ps f11, 704(x20)\n"
			"fcmov.ps f7, f14, f11, f11\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f1, 512(x6)\n"
			"flw.ps f0, 768(x6)\n"
			"fadd.ps f18, f1, f0, rtz\n" 
			"la x6, rand_int_32\n"
			"flw.ps f29, 64(x6)\n"
			"fpackreph.pi f3, f29\n" 
			"li x11, 0xff5121bc15752464\n"
			"fcvt.s.w f12, x11, rmm\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f14, 160(x26)\n"
			"fcvt.lu.s x20, f14, rtz\n" 
			"maskpopcz x20, m2\n" 
			"la x11, rand_int_32\n"
			"flw.ps f14, 576(x11)\n"
			"flw.ps f29, 288(x11)\n"
			"fmaxu.pi f3, f14, f29\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f3, 608(x6)\n"
			"frsq.ps f23, f3\n" 
			"LBL_SEQID_1_M0_WRITE_ID_5_HID_12:\n"
			"maskxor m0, m0, m4\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0x9e90d2b7d0\n"
			"flw.ps f5, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x23, rand_int_32\n"
			"flw.ps f12, 864(x23)\n"
			"flw.ps f14, 992(x23)\n"
			"fremu.pi f14, f12, f14\n" 
			"la x19, rand_int_32\n"
			"flw.ps f10, 736(x19)\n"
			"flw.ps f28, 576(x19)\n"
			"fmin.pi f12, f10, f28\n" 
			"fbci.pi f3, 0x74a32\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f11, 960(x26)\n"
			"flw.ps f0, 640(x26)\n"
			"flw.ps f7, 192(x26)\n"
			"fnmadd.s f22, f11, f0, f7, rtz\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ID_503692_HID_12:\n"
			"li x6, ((0x15 << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0xf << TENSOR_QUANT_QUANT_ROW) | (0x32 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x6 << TENSOR_QUANT_TRANSF5) | (0x6 << TENSOR_QUANT_TRANSF4) | (0x7 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ID_380772_HID_12:\n"
			"li x6, ((0x0 << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0xd << TENSOR_QUANT_QUANT_ROW) | (0x2a << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x2 << TENSOR_QUANT_TRANSF5) | (0x7 << TENSOR_QUANT_TRANSF4) | (0x6 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_12\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_1_RDX_ID_1_SHIRE_0_HID_12:\n"
			"li x20, 0xa0000000010000a\n"
			"csrw tensor_reduce, x20\n" 
			"csrwi tensor_wait, 9\n" 
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=0); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ITER_0_HID_12:\n"
			"li x5, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x0 << TENSOR_FMA16_B_NUM_COLS) | (0x8 << TENSOR_FMA16_A_NUM_ROWS) | (0xc << TENSOR_FMA16_A_NUM_COLS) | (0x9 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x5b << TENSOR_FMA16_SCP_LOC_B) | (0x14 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ITER_1_HID_12:\n"
			"li x5, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x2 << TENSOR_IMA_B_NUM_COLS) | (0x7 << TENSOR_IMA_A_NUM_ROWS) | (0xd << TENSOR_IMA_A_NUM_COLS) | (0x9 << TENSOR_IMA_OFFSET) | (0x0 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x8b << TENSOR_IMA_SCP_LOC_B) | (0x20 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x0 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_12\n"
			"csrw tensor_error, x0\n"
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_12\n"
			"addi x30, x30, 1\n"
			"la x23, rand_ieee754_32\n"
			"flw.ps f18, 480(x23)\n"
			"fcvt.pw.ps f10, f18, rdn\n" 
			"la x11, rand_int_32\n"
			"flw.ps f1, 928(x11)\n"
			"flw.ps f10, 96(x11)\n"
			"fsub.pi f0, f1, f10\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f29, 256(x12)\n"
			"flw.ps f21, 448(x12)\n"
			"flw.ps f0, 512(x12)\n"
			"fnmsub.ps f7, f29, f21, f0, rtz\n" 
			"la x22, rand_int_32\n"
			"flw.ps f21, 992(x22)\n"
			"faddi.pi f21, f21, 0x5e\n" 
			"la x26, rand_int_32\n"
			"flw.ps f11, 320(x26)\n"
			"feq.pi f28, f11, f11\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f29, 640(x22)\n"
			"flw.ps f28, 96(x22)\n"
			"fdiv.ps f7, f29, f28, rmm\n" 
			"la x28, rand_int_32\n"
			"flw.ps f7, 832(x28)\n"
			"fnot.pi f21, f7\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f29, 64(x6)\n"
			"fswizz.ps f12, f29, 0x15\n" 
			"li x5, 0x9fd\n"
			"csrw tensor_mask, x5\n"	
			"la x28, rand_ieee754_32\n"
			"flw.ps f11, 576(x28)\n"
			"flw.ps f3, 512(x28)\n"
			"flw.ps f28, 224(x28)\n"
			"fmadd.ps f28, f11, f3, f28, rne\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f28, 256(x6)\n"
			"flw.ps f10, 800(x6)\n"
			"fsgnjx.s f14, f28, f10\n" 
			"la x19, rand_int_32\n"
			"flw.ps f23, 64(x19)\n"
			"flw.ps f14, 0(x19)\n"
			"fminu.pi f14, f23, f14\n" 
			"la x20, rand_int_32\n"
			"flw.ps f29, 480(x20)\n"
			"flw.ps f8, 992(x20)\n"
			"fmaxu.pi f1, f29, f8\n" 
			"LBL_SEQID_2_FP_TRANS_ID_6_HID_12:\n"
			"la x20, rand_ieee754_32\n"
			"flw.ps f21, 608(x20)\n"
			"fexp.ps f18, f21\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f28, 768(x22)\n"
			"flw.ps f1, 576(x22)\n"
			"flw.ps f8, 448(x22)\n"
			"fmadd.s f8, f28, f1, f8, rmm\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f8, 672(x10)\n"
			"flw.ps f3, 416(x10)\n"
			"fsgnjn.s f0, f8, f3\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f28, 608(x22)\n"
			"fcvt.w.s x20, f28, rdn\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_2_TLD_XMA_STR_SEQ_ID_999362_HID_12:\n"
			"la x31, self_check_16\n"
			"li x5, 0x8000000000000a\n"
			"add x5, x5, x31\n"
			"li x6, 0x36000000000000d\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x1 << TENSOR_FMA16_B_NUM_COLS) | (0xa << TENSOR_FMA16_A_NUM_ROWS) | (0xd << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1b << TENSOR_FMA16_SCP_LOC_B) | (0x4 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x18 << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0xa << TENSOR_QUANT_QUANT_ROW) | (0x1b << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x2 << TENSOR_QUANT_TRANSF8) | (0x7 << TENSOR_QUANT_TRANSF7) | (0x7 << TENSOR_QUANT_TRANSF6) | (0x6 << TENSOR_QUANT_TRANSF5) | (0x7 << TENSOR_QUANT_TRANSF4) | (0x7 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0x1c\n"
			"bne x5, x31, LBL_FAIL_HID_12\n"
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_12\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_2_TLD_XMA_STR_SEQ_ITER_0_HID_12:\n"
			"la x31, self_check_16\n"
			"li x5, 0xe0000000000009\n"
			"add x5, x5, x31\n"
			"li x6, 0x10000000000005\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x0 << TENSOR_FMA16_B_NUM_COLS) | (0x9 << TENSOR_FMA16_A_NUM_ROWS) | (0x5 << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x1 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x19 << TENSOR_FMA16_SCP_LOC_B) | (0x7 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x12 << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0x9 << TENSOR_QUANT_QUANT_ROW) | (0x10 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x2 << TENSOR_QUANT_TRANSF7) | (0x7 << TENSOR_QUANT_TRANSF6) | (0x7 << TENSOR_QUANT_TRANSF5) | (0x6 << TENSOR_QUANT_TRANSF4) | (0x7 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0xc\n"
			"bne x5, x31, LBL_FAIL_HID_12\n"
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_12\n"
			"csrw tensor_error, x0\n"
			"li x5, 3\n" 
			"bne x30, x5, LBL_FAIL_HID_12\n"
			"nop\n"          
		       VSNIP_RSV     
		);
		load_vpu_regs();
		__asm__ __volatile__ ( 
			"nop\n"          
			"addi x30, x30, 1\n"
			"la x19, rand_ieee754_32\n"
			"flw.ps f3, 768(x19)\n"
			"fmax.s f22, f3, f3\n" 
			"la x6, rand_int_32\n"
			"flw.ps f10, 96(x6)\n"
			"flw.ps f7, 448(x6)\n"
			"fand.pi f1, f10, f7\n" 
			"la x26, rand_int_32\n"
			"flw.ps f29, 480(x26)\n"
			"flw.ps f28, 928(x26)\n"
			"fadd.pi f23, f29, f28\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f10, 256(x26)\n"
			"fsqrt.ps f0, f10\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f21, 192(x6)\n"
			"flw.ps f1, 288(x6)\n"
			"fsub.s f7, f21, f1, rup\n" 
			"la x11, rand_int_32\n"
			"flw.ps f14, 0(x11)\n"
			"fpackrepb.pi f28, f14\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f22, 384(x12)\n"
			"flw.ps f23, 512(x12)\n"
			"fsub.s f7, f22, f23, rdn\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f20, 544(x10)\n"
			"fclass.ps f20, f20\n" 
			"LBL_SEQID_3_M0_WRITE_ID_7_HID_12:\n"
			"maskxor m7, m0, m0\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x10, rand_ieee754_32\n"
			"flw.ps f20, 96(x10)\n"
			"flw.ps f18, 352(x10)\n"
			"fnmsub.s f23, f20, f18, f18, rtz\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f29, 992(x20)\n"
			"flw.ps f8, 288(x20)\n"
			"flw.ps f20, 224(x20)\n"
			"fmadd.s f0, f29, f8, f20, rne\n" 
			"la x28, rand_int_32\n"
			"flw.ps f14, 448(x28)\n"
			"flw.ps f11, 288(x28)\n"
			"flt.pi f10, f14, f11\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f20, 864(x12)\n"
			"flw.ps f21, 384(x12)\n"
			"fsub.s f22, f20, f21, rdn\n" 
			"LBL_SEQID_3_M0_WRITE_ID_8_HID_12:\n"
			"masknot m4, m7\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"fbci.ps f3, 0x22c2b\n" 
			"LBL_SEQID_3_M0_WRITE_ID_9_HID_12:\n"
			"mov.m.x m4, x28, 0x70\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_3_TLD_XMA_STR_SEQ_ITER_0_HID_12:\n"
			"la x31, self_check_16\n"
			"li x5, 0x60000000000002\n"
			"add x5, x5, x31\n"
			"li x6, 0x360000000000007\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x0 << TENSOR_FMA16_B_NUM_COLS) | (0x2 << TENSOR_FMA16_A_NUM_ROWS) | (0x7 << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1b << TENSOR_FMA16_SCP_LOC_B) | (0x3 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0x10\n"
			"bne x5, x31, LBL_FAIL_HID_12\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_12\n"
			"csrw tensor_error, x0\n"
			"li x5, 4\n" 
			"bne x30, x5, LBL_FAIL_HID_12\n"
			"nop\n"          
		       VSNIP_RSV     
		);
		trans();
		__asm__ __volatile__ ( 
			"nop\n"          
			"addi x30, x30, 1\n"
			"la x6, rand_ieee754_32\n"
			"flw.ps f8, 256(x6)\n"
			"flw.ps f20, 320(x6)\n"
			"feq.ps f22, f8, f20\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f0, 384(x20)\n"
			"fcvt.lu.s x20, f0, rup\n" 
			"la x23, rand_int_32\n"
			"flw.ps f12, 928(x23)\n"
			"flw.ps f3, 608(x23)\n"
			"fsub.pi f29, f12, f3\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f7, 768(x12)\n"
			"fsgnjn.ps f12, f7, f7\n" 
			"LBL_SEQID_4_M0_WRITE_ID_10_HID_12:\n"
			"mov.m.x m7, x23, 0x26\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x6, rand_ieee754_32\n"
			"flw.ps f10, 576(x6)\n"
			"flw.ps f1, 96(x6)\n"
			"fsgnj.ps f18, f10, f1\n" 
			"LBL_SEQID_4_M0_WRITE_ID_11_HID_12:\n"
			"flem.ps m2, f18, f12\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_4_FP_TRANS_ID_12_HID_12:\n"
			"la x22, rand_ieee754_32\n"
			"flw.ps f10, 512(x22)\n"
			"frcp.ps f1, f10\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f7, 352(x11)\n"
			"fcvt.w.s x12, f7, rne\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f22, 224(x11)\n"
			"fcvt.pw.ps f10, f22, rmm\n" 
			"li x5, 0x7c74\n"
			"csrw tensor_mask, x5\n"	
			"la x20, rand_int_32\n"
			"flw.ps f0, 192(x20)\n"
			"flw.ps f14, 960(x20)\n"
			"fltu.pi f28, f0, f14\n" 
			"la x19, rand_int_32\n"
			"flw.ps f23, 832(x19)\n"
			"faddi.pi f23, f23, 0x8\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f1, 512(x23)\n"
			"flw.ps f0, 416(x23)\n"
			"feq.s x12, f1, f0\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f12, 768(x12)\n"
			"flw.ps f20, 832(x12)\n"
			"fsgnjn.ps f23, f12, f20\n" 
			"li x5, 0xa281b214fd74353d\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0x4f92df3f9c600c6d\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"li x23, 0x809f752d8e492308\n"
			"fcvt.s.wu f8, x23, rup\n" 
			"LBL_SEQID_4_M0_WRITE_ID_13_HID_12:\n"
			"feqm.ps m4, f21, f11\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_4_XMA_QNT_STR_SEQ_ID_41972_HID_12:\n"
			"li x5, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x3 << TENSOR_FMA16_B_NUM_COLS) | (0xf << TENSOR_FMA16_A_NUM_ROWS) | (0x5 << TENSOR_FMA16_A_NUM_COLS) | (0x1 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x2f << TENSOR_FMA16_SCP_LOC_B) | (0x7 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_12\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_4_TLD_XMA_STR_SEQ_ITER_0_HID_12:\n"
			"la x31, self_check_8\n"
			"li x5, 0xa0000000000004\n"
			"add x5, x5, x31\n"
			"li x6, 0x1000000000000b\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x0 << TENSOR_IMA_B_NUM_COLS) | (0x4 << TENSOR_IMA_A_NUM_ROWS) | (0xb << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x1 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1d << TENSOR_IMA_SCP_LOC_B) | (0x5 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x9, ((0x9 << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0x4 << TENSOR_QUANT_QUANT_ROW) | (0x3 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x1 << TENSOR_QUANT_TRANSF1) | (0x5 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fmvz.x.ps x31, f0, 0\n"
			"li x5, 0x30\n"
			"bne x5, x31, LBL_FAIL_HID_12\n"
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_12\n"
			"csrw tensor_error, x0\n"
			"li x5, 5\n" 
			"bne x30, x5, LBL_FAIL_HID_12\n"
			"nop\n"          
		       VSNIP_RSV     
		);
		load_vpu_regs();
		__asm__ __volatile__ ( 
			"nop\n"          
			"addi x30, x30, 1\n"
			"la x6, rand_int_32\n"
			"flw.ps f8, 448(x6)\n"
			"flw.ps f23, 576(x6)\n"
			"fmax.pi f11, f8, f23\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f3, 352(x6)\n"
			"fcvt.f16.ps f18, f3\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x12, rand_ieee754_32\n"
			"flw.ps f14, 192(x12)\n"
			"flw.ps f11, 832(x12)\n"
			"flw.ps f7, 256(x12)\n"
			"fmsub.s f1, f14, f11, f7, rdn\n" 
			"la x6, rand_int_32\n"
			"flw.ps f1, 640(x6)\n"
			"fslli.pi f18, f1, 0xb\n" 
			"fbci.ps f28, 0x5a5d9\n" 
			"LBL_SEQID_5_FP_TRANS_ID_14_HID_12:\n"
			"la x11, rand_ieee754_32\n"
			"flw.ps f23, 352(x11)\n"
			"flog.ps f14, f23\n" 
			"LBL_SEQID_5_FP_TRANS_ID_15_HID_12:\n"
			"la x20, rand_ieee754_32\n"
			"flw.ps f14, 960(x20)\n"
			"flog.ps f7, f14\n" 
			"li x5, 0x3732\n"
			"csrw tensor_mask, x5\n"	
			"la x23, rand_int_32\n"
			"flw.ps f14, 64(x23)\n"
			"flw.ps f18, 928(x23)\n"
			"fmulh.pi f28, f14, f18\n" 
			"la x26, rand_int_32\n"
			"flw.ps f22, 352(x26)\n"
			"flw.ps f3, 672(x26)\n"
			"fltu.pi f8, f22, f3\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f20, 480(x12)\n"
			"flw.ps f11, 32(x12)\n"
			"fadd.ps f29, f20, f11, rtz\n" 
			"la x28, rand_int_32\n"
			"flw.ps f12, 832(x28)\n"
			"flw.ps f7, 896(x28)\n"
			"fminu.pi f14, f12, f7\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x26, rand_ieee754_32\n"
			"flw.ps f29, 224(x26)\n"
			"flw.ps f0, 672(x26)\n"
			"fmul.ps f3, f29, f0, rne\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f8, 672(x22)\n"
			"flw.ps f11, 160(x22)\n"
			"fmin.s f23, f8, f11\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f10, 384(x26)\n"
			"frsq.ps f7, f10\n" 
			"LBL_SEQID_5_M0_WRITE_ID_16_HID_12:\n"
			"feqm.ps m0, f21, f14\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x22, rand_int_32\n"
			"flw.ps f21, 704(x22)\n"
			"flw.ps f20, 704(x22)\n"
			"fadd.pi f18, f21, f20\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 0x303851e767a69d51\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0x34a72255a59faa8b\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"LBL_SEQID_5_TLD_XMA_STR_SEQ_ITER_0_HID_12:\n"
			"la x31, self_check_16\n"
			"li x5, 0x2000000000000c\n"
			"add x5, x5, x31\n"
			"li x6, 0x360000000000003\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x1 << TENSOR_FMA16_B_NUM_COLS) | (0xc << TENSOR_FMA16_A_NUM_ROWS) | (0x3 << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1b << TENSOR_FMA16_SCP_LOC_B) | (0x1 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0x8\n"
			"bne x5, x31, LBL_FAIL_HID_12\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_12\n"
			"csrw tensor_error, x0\n"
			"li x5, 6\n" 
			"bne x30, x5, LBL_FAIL_HID_12\n"
			"addi x30, x30, 1\n"
			"la x28, rand_ieee754_32\n"
			"flw.ps f18, 0(x28)\n"
			"flw.ps f23, 96(x28)\n"
			"fmax.ps f14, f18, f23\n" 
			"la x22, rand_int_32\n"
			"flw.ps f3, 448(x22)\n"
			"fcvt.ps.pw f23, f3, rdn\n" 
			"li x5, 0xde0c\n"
			"csrw tensor_mask, x5\n"	
			"la x11, rand_ieee754_32\n"
			"flw.ps f11, 512(x11)\n"
			"flw.ps f28, 288(x11)\n"
			"fmin.s f18, f11, f28\n" 
			"la x6, rand_int_32\n"
			"flw.ps f11, 672(x6)\n"
			"fcvt.ps.pw f28, f11, rmm\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x28, rand_ieee754_32\n"
			"flw.ps f12, 224(x28)\n"
			"flw.ps f18, 96(x28)\n"
			"fsub.s f1, f12, f18, rup\n" 
			"mova.x.m x23\n" 
			"li x5, 0x6c40\n"
			"csrw tensor_mask, x5\n"	
			"la x28, rand_ieee754_32\n"
			"flw.ps f3, 832(x28)\n"
			"flw.ps f14, 992(x28)\n"
			"fle.ps f29, f3, f14\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"LBL_SEQID_6_M0_WRITE_ID_17_HID_12:\n"
			"masknot m0, m0\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x10, rand_ieee754_32\n"
			"flw.ps f20, 224(x10)\n"
			"fcvt.w.s x28, f20, rtz\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f10, 864(x26)\n"
			"fround.ps f11, f10, rup\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f3, 128(x26)\n"
			"fsub.ps f22, f3, f3, rdn\n" 
			"LBL_SEQID_6_FP_TRANS_ID_18_HID_12:\n"
			"la x10, rand_ieee754_32\n"
			"flw.ps f0, 64(x10)\n"
			"frcp.ps f12, f0\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x11, rand_int_32\n"
			"flw.ps f11, 32(x11)\n"
			"fsatu8.pi f1, f11\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f21, 96(x22)\n"
			"fcvt.wu.s x11, f21, rne\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f21, 320(x12)\n"
			"flw.ps f3, 576(x12)\n"
			"fsub.ps f7, f21, f3, rdn\n" 
			"la x26, rand_int_32\n"
			"flw.ps f3, 640(x26)\n"
			"fnot.pi f3, f3\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_6_XMA_QNT_STR_SEQ_ITER_0_HID_12:\n"
			"li x5, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x0 << TENSOR_IMA_B_NUM_COLS) | (0xe << TENSOR_IMA_A_NUM_ROWS) | (0x2 << TENSOR_IMA_A_NUM_COLS) | (0x3 << TENSOR_IMA_OFFSET) | (0x0 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x27 << TENSOR_IMA_SCP_LOC_B) | (0x70 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x0 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"LBL_SEQID_6_XMA_QNT_STR_SEQ_ITER_1_HID_12:\n"
			"li x6, ((0x9 << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0x9 << TENSOR_QUANT_QUANT_ROW) | (0x2c << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x0 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_6_XMA_QNT_STR_SEQ_ITER_2_HID_12:\n"
			"li x6, ((0x5 << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0x9 << TENSOR_QUANT_QUANT_ROW) | (0x31 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x2 << TENSOR_QUANT_TRANSF5) | (0x6 << TENSOR_QUANT_TRANSF4) | (0x7 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 7\n"
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_12\n"
			"csrw tensor_error, x0\n"
			"li x5, 7\n" 
			"bne x30, x5, LBL_FAIL_HID_12\n"
			"nop \n"
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=1); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"nop \n"
			"addi x29, x29, -1\n"
			"bne x29, x0, LBL_OUTER_LOOP_SEQ_HID_12\n"
			"nop\n"          
		       VSNIP_RSV     
		);
		load_vpu_regs();
		__asm__ __volatile__ ( 
			"nop\n"          
			"nop\n"          
		       VSNIP_RSV     
		);
		trans();
		__asm__ __volatile__ ( 
			"nop\n"          
			"la x31, check_sum_hid_12\n"
			"lw x5, (x31)\n"
			"bne x30, x5, LBL_FAIL_HID_12\n"
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
		c_sync_point(sync_odd_harts=1); 
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
	if ( hid%16 == 13 ) { 
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
			"csrwi mhpmevent3, 11\n"   
			"csrwi mhpmevent4, 1\n"   
			"csrwi mhpmevent5, 6\n"   
			"csrwi mhpmevent6, 12\n"   
			"csrwi mhpmevent7, 23\n"   
			"csrwi mhpmevent8, 21\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f24,  832 (x5)\n"
			"flw.ps f9,  160 (x5)\n"
			"flw.ps f25,  352 (x5)\n"
			"flw.ps f29,  0 (x5)\n"
			"flw.ps f16,  192 (x5)\n"
			"flw.ps f4,  928 (x5)\n"
			"flw.ps f11,  480 (x5)\n"
			"flw.ps f15,  960 (x5)\n"
			"flw.ps f7,  896 (x5)\n"
			"flw.ps f20,  672 (x5)\n"
			"flw.ps f3,  224 (x5)\n"
			"flw.ps f6,  64 (x5)\n"
			"flw.ps f10,  32 (x5)\n"
			"flw.ps f31,  320 (x5)\n"
			"flw.ps f17,  704 (x5)\n"
			"flw.ps f13,  800 (x5)\n"
			"flw.ps f27,  992 (x5)\n"
			"flw.ps f14,  736 (x5)\n"
			"flw.ps f28,  768 (x5)\n"
			"flw.ps f18,  544 (x5)\n"
			"flw.ps f1,  448 (x5)\n"
			"flw.ps f30,  256 (x5)\n"
			"flw.ps f22,  512 (x5)\n"
			"flw.ps f23,  608 (x5)\n"
			"flw.ps f26,  576 (x5)\n"
			"flw.ps f0,  128 (x5)\n"
			"flw.ps f12,  288 (x5)\n"
			"flw.ps f5,  864 (x5)\n"
			"flw.ps f8,  96 (x5)\n"
			"flw.ps f21,  384 (x5)\n"
			"flw.ps f2,  640 (x5)\n"
			"flw.ps f19,  416 (x5)\n"
			"csrw fflags, zero\n"
			"csrwi frm, 0x0\n"
			"li x31, 100\n"
			"1:\n" 
			"addi x31, x31, -1\n"
			"bne x31, x0, 1b\n" 
			"csrw fcc, x0\n" 
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=1); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"LBL_KERNEL_HID_13:\n"          
			"li x5, 0xFEEDFEED\n"
			"slti x0, x0, 0x7f2\n"
			"la x31, check_sum_hid_13\n"
			"li x5, 0x7\n" 
			"sw x5, (x31)\n"
			"li x29, 0x7\n"
			"LBL_OUTER_LOOP_SEQ_HID_13:\n"
			"nop \n"
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=1); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"nop \n"
			"li x30, 0\n"
			"addi x30, x30, 1\n"
			"fbci.pi f25, 0x6ed67\n" 
			"la x28, rand_int_32\n"
			"flw.ps f18, 928(x28)\n"
			"flw.ps f8, 928(x28)\n"
			"fsll.pi f7, f18, f8\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f17, 736(x22)\n"
			"flw.ps f9, 800(x22)\n"
			"fsub.ps f15, f17, f9, rtz\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f31, 768(x23)\n"
			"fcvt.w.s x6, f31, rne\n" 
			"LBL_SEQID_0_FP_TRANS_ID_0_HID_13:\n"
			"la x14, rand_ieee754_32\n"
			"flw.ps f29, 160(x14)\n"
			"fexp.ps f9, f29\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_13:\n"
			"fltm.ps m4, f18, f25\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x7, rand_ieee754_32\n"
			"flw.ps f13, 0(x7)\n"
			"flw.ps f29, 32(x7)\n"
			"fmsub.ps f5, f13, f29, f13, rmm\n" 
			"la x9, rand_int_32\n"
			"flw.ps f10, 416(x9)\n"
			"flw.ps f0, 96(x9)\n"
			"fsrl.pi f9, f10, f0\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f7, 576(x14)\n"
			"flw.ps f15, 160(x14)\n"
			"flw.ps f23, 96(x14)\n"
			"fcmov.ps f29, f7, f15, f23\n" 
			"la x23, rand_int_32\n"
			"flw.ps f11, 96(x23)\n"
			"flw.ps f31, 352(x23)\n"
			"feq.pi f17, f11, f31\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f17, 704(x17)\n"
			"flw.ps f9, 992(x17)\n"
			"fmin.ps f10, f17, f9\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f0, 576(x7)\n"
			"flw.ps f8, 576(x7)\n"
			"fmax.s f18, f0, f8\n" 
			"la x11, rand_ieee754_16\n"
			"flw.ps f10, 512(x11)\n"
			"fcvt.ps.f16 f17, f10\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f7, 928(x6)\n"
			"fsqrt.s f0, f7, rtz\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f29, 128(x17)\n"
			"flw.ps f15, 480(x17)\n"
			"fmin.s f10, f29, f15\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f25, 448(x7)\n"
			"flw.ps f13, 64(x7)\n"
			"flt.ps f31, f25, f13\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_13\n"
			"addi x30, x30, 1\n"
			"la x15, rand_ieee754_32\n"
			"flw.ps f10, 896(x15)\n"
			"flw.ps f11, 0(x15)\n"
			"flt.ps f21, f10, f11\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f7, 96(x20)\n"
			"flw.ps f9, 288(x20)\n"
			"fle.s x20, f7, f9\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f21, 800(x7)\n"
			"flw.ps f18, 576(x7)\n"
			"fadd.s f18, f21, f18, rdn\n" 
			"LBL_SEQID_1_M0_WRITE_ID_2_HID_13:\n"
			"flem.ps m3, f5, f5\n" 
			"li x5, CREDINC2\n"
			"ld x5, 0(x5)\n"
			"la x11, rand_ieee754_32\n"
			"flw.ps f25, 672(x11)\n"
			"flw.ps f21, 608(x11)\n"
			"flt.s x23, f25, f21\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f7, 128(x22)\n"
			"flw.ps f0, 832(x22)\n"
			"fsgnjx.s f5, f7, f0\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f0, 736(x28)\n"
			"fcvt.pwu.ps f18, f0, rmm\n" 
			"LBL_SEQID_1_M0_WRITE_ID_3_HID_13:\n"
			"mova.m.x x7\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x28, rand_ieee754_32\n"
			"flw.ps f21, 192(x28)\n"
			"flw.ps f17, 224(x28)\n"
			"flw.ps f9, 544(x28)\n"
			"fmadd.ps f25, f21, f17, f9, rdn\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f8, 672(x11)\n"
			"flw.ps f10, 608(x11)\n"
			"fdiv.ps f5, f8, f10, rmm\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f0, 608(x22)\n"
			"flw.ps f25, 192(x22)\n"
			"feq.s x14, f0, f25\n" 
			"la x23, rand_int_32\n"
			"flw.ps f18, 960(x23)\n"
			"fmax.pi f9, f18, f18\n" 
			"la x7, rand_int_32\n"
			"flw.ps f15, 32(x7)\n"
			"flw.ps f13, 704(x7)\n"
			"for.pi f29, f15, f13\n" 
			"LBL_SEQID_1_M0_WRITE_ID_4_HID_13:\n"
			"maskand m5, m4, m4\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_1_M0_WRITE_ID_5_HID_13:\n"
			"flem.ps m3, f13, f5\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x23, rand_ieee754_32\n"
			"flw.ps f10, 224(x23)\n"
			"ffrc.ps f7, f10\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_13\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_2_M0_WRITE_ID_6_HID_13:\n"
			"flem.ps m4, f7, f8\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x23, rand_ieee754_32\n"
			"flw.ps f10, 64(x23)\n"
			"flw.ps f23, 160(x23)\n"
			"fsgnjn.s f13, f10, f23\n" 
			"la x17, rand_int_32\n"
			"flw.ps f9, 192(x17)\n"
			"flw.ps f5, 800(x17)\n"
			"fmaxu.pi f5, f9, f5\n" 
			"la x28, rand_int_32\n"
			"flw.ps f11, 608(x28)\n"
			"fsrai.pi f31, f11, 0x7\n" 
			"li x28, 0xeca5610ed344bfcb\n"
			"fcvt.s.w f5, x28, rmm\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f31, 448(x17)\n"
			"flw.ps f17, 352(x17)\n"
			"fsub.ps f10, f31, f17, rtz\n" 
			"la x15, rand_int_32\n"
			"flw.ps f23, 672(x15)\n"
			"flw.ps f17, 736(x15)\n"
			"frem.pi f5, f23, f17\n" 
			"la x9, rand_int_32\n"
			"flw.ps f29, 896(x9)\n"
			"flw.ps f7, 480(x9)\n"
			"fltu.pi f0, f29, f7\n" 
			"LBL_SEQID_2_M0_WRITE_ID_7_HID_13:\n"
			"fltm.ps m3, f11, f31\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x11, rand_int_32\n"
			"flw.ps f21, 736(x11)\n"
			"fcvt.ps.pwu f0, f21, rup\n" 
			"la x22, rand_int_32\n"
			"flw.ps f29, 32(x22)\n"
			"flw.ps f18, 704(x22)\n"
			"fmulh.pi f21, f29, f18\n" 
			"la x11, rand_int_32\n"
			"flw.ps f15, 512(x11)\n"
			"flw.ps f17, 832(x11)\n"
			"fsrl.pi f21, f15, f17\n" 
			"la x22, rand_int_32\n"
			"flw.ps f13, 416(x22)\n"
			"flw.ps f7, 576(x22)\n"
			"fsll.pi f10, f13, f7\n" 
			"la x20, rand_int_32\n"
			"flw.ps f8, 992(x20)\n"
			"fpackreph.pi f8, f8\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f11, 576(x9)\n"
			"flw.ps f29, 320(x9)\n"
			"flw.ps f10, 736(x9)\n"
			"fnmadd.s f5, f11, f29, f10, rmm\n" 
			"la x22, rand_int_32\n"
			"flw.ps f11, 672(x22)\n"
			"flw.ps f31, 32(x22)\n"
			"fminu.pi f8, f11, f31\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 3\n" 
			"bne x30, x5, LBL_FAIL_HID_13\n"
			"nop\n"          
		       VSNIP_RSV     
		);
		load_vpu_regs();
		__asm__ __volatile__ ( 
			"nop\n"          
			"addi x30, x30, 1\n"
			"la x15, rand_int_32\n"
			"flw.ps f11, 992(x15)\n"
			"flw.ps f17, 416(x15)\n"
			"fsra.pi f17, f11, f17\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f23, 832(x23)\n"
			"flw.ps f10, 128(x23)\n"
			"fsub.ps f7, f23, f10, rup\n" 
			"la x9, rand_int_32\n"
			"flw.ps f29, 704(x9)\n"
			"fsatu8.pi f11, f29\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f10, 928(x15)\n"
			"flw.ps f25, 608(x15)\n"
			"fmin.s f18, f10, f25\n" 
			"la x14, rand_ieee754_16\n"
			"flw.ps f23, 832(x14)\n"
			"fcvt.ps.f16 f8, f23\n" 
			"la x6, rand_int_32\n"
			"flw.ps f17, 480(x6)\n"
			"flw.ps f29, 448(x6)\n"
			"fxor.pi f18, f17, f29\n" 
			"la x9, rand_int_32\n"
			"flw.ps f31, 416(x9)\n"
			"flw.ps f8, 896(x9)\n"
			"flt.pi f21, f31, f8\n" 
			"la x14, rand_int_32\n"
			"flw.ps f5, 672(x14)\n"
			"flw.ps f29, 544(x14)\n"
			"fxor.pi f10, f5, f29\n" 
			"LBL_SEQID_3_FP_TRANS_ID_8_HID_13:\n"
			"la x22, rand_ieee754_32\n"
			"flw.ps f29, 224(x22)\n"
			"fexp.ps f10, f29\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f8, 160(x9)\n"
			"fclass.s x20, f8\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f7, 128(x28)\n"
			"flw.ps f5, 544(x28)\n"
			"fsgnjn.s f29, f7, f5\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f25, 704(x14)\n"
			"fcvt.pwu.ps f17, f25, rmm\n" 
			"LBL_SEQID_3_M0_WRITE_ID_9_HID_13:\n"
			"feqm.ps m5, f13, f29\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x7, rand_ieee754_32\n"
			"flw.ps f21, 0(x7)\n"
			"flw.ps f17, 32(x7)\n"
			"fsgnj.s f31, f21, f17\n" 
			"LBL_SEQID_3_M0_WRITE_ID_10_HID_13:\n"
			"feqm.ps m3, f11, f9\n" 
			"li x5, 0x803401B8\n"
			"ld x5, 0(x5)\n"
			"la x15, rand_int_32\n"
			"flw.ps f8, 384(x15)\n"
			"flw.ps f9, 832(x15)\n"
			"fxor.pi f0, f8, f9\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 4\n" 
			"bne x30, x5, LBL_FAIL_HID_13\n"
			"nop\n"          
		       VSNIP_RSV     
		);
		trans();
		__asm__ __volatile__ ( 
			"nop\n"          
			"addi x30, x30, 1\n"
			"la x23, rand_ieee754_32\n"
			"flw.ps f5, 512(x23)\n"
			"flw.ps f31, 448(x23)\n"
			"flw.ps f11, 768(x23)\n"
			"fmadd.s f13, f5, f31, f11, rmm\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f11, 64(x23)\n"
			"flw.ps f8, 832(x23)\n"
			"fmul.ps f13, f11, f8, rne\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f17, 448(x20)\n"
			"flw.ps f8, 256(x20)\n"
			"fle.ps f18, f17, f8\n" 
			"la x14, rand_int_32\n"
			"flw.ps f23, 288(x14)\n"
			"flw.ps f13, 288(x14)\n"
			"fsra.pi f21, f23, f13\n" 
			"la x22, rand_int_32\n"
			"flw.ps f7, 480(x22)\n"
			"flw.ps f15, 736(x22)\n"
			"fand.pi f11, f7, f15\n" 
			"la x23, rand_int_32\n"
			"flw.ps f15, 576(x23)\n"
			"flw.ps f0, 832(x23)\n"
			"fmin.pi f21, f15, f0\n" 
			"la x9, rand_int_32\n"
			"flw.ps f18, 160(x9)\n"
			"flw.ps f23, 544(x9)\n"
			"fltu.pi f11, f18, f23\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f17, 352(x22)\n"
			"fcvt.wu.s x6, f17, rdn\n" 
			"la x11, rand_int_32\n"
			"flw.ps f15, 128(x11)\n"
			"flw.ps f0, 288(x11)\n"
			"feq.pi f9, f15, f0\n" 
			"la x9, rand_int_32\n"
			"flw.ps f8, 800(x9)\n"
			"flw.ps f15, 928(x9)\n"
			"fmulhu.pi f7, f8, f15\n" 
			"fbci.ps f11, 0x704d1\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f25, 512(x22)\n"
			"flw.ps f18, 480(x22)\n"
			"fsgnj.s f31, f25, f18\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f13, 96(x20)\n"
			"flw.ps f0, 768(x20)\n"
			"fsgnjx.s f31, f13, f0\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f23, 736(x9)\n"
			"fcvt.pw.ps f0, f23, rdn\n" 
			"LBL_SEQID_4_M0_WRITE_ID_11_HID_13:\n"
			"maskor m3, m4, m3\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x15, rand_ieee754_32\n"
			"flw.ps f0, 480(x15)\n"
			"fcvt.wu.s x7, f0, rne\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 5\n" 
			"bne x30, x5, LBL_FAIL_HID_13\n"
			"nop\n"          
		       VSNIP_RSV     
		);
		load_vpu_regs();
		__asm__ __volatile__ ( 
			"nop\n"          
			"addi x30, x30, 1\n"
			"la x11, rand_ieee754_32\n"
			"flw.ps f17, 992(x11)\n"
			"fswizz.ps f31, f17, 0xd\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f11, 96(x14)\n"
			"flw.ps f5, 256(x14)\n"
			"flw.ps f25, 864(x14)\n"
			"fcmov.ps f15, f11, f5, f25\n" 
			"LBL_SEQID_5_M0_WRITE_ID_12_HID_13:\n"
			"fsetm.pi m3, f21\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x14, rand_ieee754_32\n"
			"flw.ps f7, 544(x14)\n"
			"flw.ps f5, 160(x14)\n"
			"fmin.s f11, f7, f5\n" 
			"la x22, rand_int_32\n"
			"flw.ps f31, 192(x22)\n"
			"fpackrepb.pi f31, f31\n" 
			"la x11, rand_int_32\n"
			"flw.ps f9, 608(x11)\n"
			"fnot.pi f0, f9\n" 
			"la x7, rand_int_32\n"
			"flw.ps f7, 448(x7)\n"
			"fsrli.pi f15, f7, 0x4\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f0, 832(x14)\n"
			"fclass.s x23, f0\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f21, 96(x17)\n"
			"flw.ps f11, 960(x17)\n"
			"feq.s x7, f21, f11\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f13, 288(x9)\n"
			"flw.ps f0, 416(x9)\n"
			"fmul.s f29, f13, f0, rmm\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f9, 768(x14)\n"
			"flw.ps f13, 928(x14)\n"
			"fcmov.ps f13, f9, f9, f13\n" 
			"fbci.pi f17, 0x4e2cf\n" 
			"la x6, rand_int_32\n"
			"flw.ps f15, 576(x6)\n"
			"flw.ps f7, 608(x6)\n"
			"fsra.pi f8, f15, f7\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f25, 32(x7)\n"
			"flw.ps f13, 160(x7)\n"
			"flw.ps f5, 352(x7)\n"
			"fmadd.ps f15, f25, f13, f5, rmm\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f17, 512(x7)\n"
			"fsin.ps f17, f17\n" 
			"la x23, rand_int_32\n"
			"flw.ps f8, 256(x23)\n"
			"flw.ps f31, 192(x23)\n"
			"for.pi f11, f8, f31\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 6\n" 
			"bne x30, x5, LBL_FAIL_HID_13\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_6_FP_TRANS_ID_13_HID_13:\n"
			"la x15, rand_ieee754_32\n"
			"flw.ps f18, 928(x15)\n"
			"flog.ps f7, f18\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f29, 768(x28)\n"
			"flw.ps f21, 384(x28)\n"
			"flw.ps f8, 0(x28)\n"
			"fnmsub.ps f21, f29, f21, f8, rtz\n" 
			"LBL_SEQID_6_FP_TRANS_ID_14_HID_13:\n"
			"la x22, rand_ieee754_32\n"
			"flw.ps f31, 160(x22)\n"
			"fexp.ps f23, f31\n" 
			"la x17, rand_int_32\n"
			"flw.ps f0, 0(x17)\n"
			"flw.ps f8, 384(x17)\n"
			"fmax.pi f8, f0, f8\n" 
			"la x11, rand_int_32\n"
			"flw.ps f23, 896(x11)\n"
			"flw.ps f17, 672(x11)\n"
			"feq.pi f15, f23, f17\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f5, 160(x6)\n"
			"flw.ps f29, 704(x6)\n"
			"fsub.ps f21, f5, f29, rne\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f25, 384(x28)\n"
			"flw.ps f31, 928(x28)\n"
			"flw.ps f11, 96(x28)\n"
			"fnmsub.s f15, f25, f31, f11, rne\n" 
			"li x11, 0x8c8a64cc6fcb4a23\n"
			"fcvt.s.w f5, x11, rup\n" 
			"LBL_SEQID_6_M0_WRITE_ID_15_HID_13:\n"
			"fsetm.pi m3, f29\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x14, rand_ieee754_32\n"
			"flw.ps f5, 544(x14)\n"
			"fswizz.ps f15, f5, 0x1\n" 
			"LBL_SEQID_6_M0_WRITE_ID_16_HID_13:\n"
			"fltm.ps m5, f10, f5\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x23, rand_ieee754_32\n"
			"flw.ps f25, 448(x23)\n"
			"flw.ps f17, 224(x23)\n"
			"flw.ps f29, 192(x23)\n"
			"fnmsub.s f17, f25, f17, f29, rdn\n" 
			"la x14, rand_int_32\n"
			"flw.ps f5, 448(x14)\n"
			"fcvt.ps.pwu f17, f5, rne\n" 
			"la x22, rand_int_32\n"
			"flw.ps f10, 864(x22)\n"
			"fcvt.ps.pw f18, f10, rtz\n" 
			"la x22, rand_ieee754_16\n"
			"flw.ps f7, 352(x22)\n"
			"fcvt.ps.f16 f11, f7\n" 
			"la x22, rand_int_32\n"
			"flw.ps f7, 800(x22)\n"
			"fsat8.pi f25, f7\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 7\n" 
			"bne x30, x5, LBL_FAIL_HID_13\n"
			"nop \n"
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=1); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"nop \n"
			"addi x29, x29, -1\n"
			"bne x29, x0, LBL_OUTER_LOOP_SEQ_HID_13\n"
			"nop\n"          
		       VSNIP_RSV     
		);
		load_vpu_regs();
		__asm__ __volatile__ ( 
			"nop\n"          
			"nop\n"          
		       VSNIP_RSV     
		);
		trans();
		__asm__ __volatile__ ( 
			"nop\n"          
			"la x31, check_sum_hid_13\n"
			"lw x5, (x31)\n"
			"bne x30, x5, LBL_FAIL_HID_13\n"
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
		c_sync_point(sync_odd_harts=1); 
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
	if ( hid%16 == 14 ) { 
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
			"csrwi mhpmevent3, 16\n"   
			"csrwi mhpmevent4, 6\n"   
			"csrwi mhpmevent5, 4\n"   
			"csrwi mhpmevent6, 27\n"   
			"csrwi mhpmevent7, 6\n"   
			"csrwi mhpmevent8, 3\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f24,  192 (x5)\n"
			"flw.ps f22,  288 (x5)\n"
			"flw.ps f23,  768 (x5)\n"
			"flw.ps f1,  448 (x5)\n"
			"flw.ps f20,  96 (x5)\n"
			"flw.ps f21,  480 (x5)\n"
			"flw.ps f25,  960 (x5)\n"
			"flw.ps f12,  512 (x5)\n"
			"flw.ps f26,  32 (x5)\n"
			"flw.ps f30,  864 (x5)\n"
			"flw.ps f27,  704 (x5)\n"
			"flw.ps f5,  128 (x5)\n"
			"flw.ps f2,  64 (x5)\n"
			"flw.ps f15,  640 (x5)\n"
			"flw.ps f8,  256 (x5)\n"
			"flw.ps f17,  992 (x5)\n"
			"flw.ps f13,  0 (x5)\n"
			"flw.ps f9,  896 (x5)\n"
			"flw.ps f31,  544 (x5)\n"
			"flw.ps f14,  832 (x5)\n"
			"flw.ps f10,  736 (x5)\n"
			"flw.ps f16,  320 (x5)\n"
			"flw.ps f3,  384 (x5)\n"
			"flw.ps f28,  352 (x5)\n"
			"flw.ps f11,  672 (x5)\n"
			"flw.ps f4,  800 (x5)\n"
			"flw.ps f7,  224 (x5)\n"
			"flw.ps f18,  928 (x5)\n"
			"flw.ps f6,  576 (x5)\n"
			"flw.ps f29,  160 (x5)\n"
			"flw.ps f19,  608 (x5)\n"
			"flw.ps f0,  416 (x5)\n"
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
			"csrw fcc, x0\n" 
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=1); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"LBL_KERNEL_HID_14:\n"          
			"li x5, 0xFEEDFEED\n"
			"slti x0, x0, 0x7f2\n"
			"la x31, check_sum_hid_14\n"
			"li x5, 0x7\n" 
			"sw x5, (x31)\n"
			"li x29, 0x7\n"
			"LBL_OUTER_LOOP_SEQ_HID_14:\n"
			"nop \n"
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=1); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"nop \n"
			"li x30, 0\n"
			"addi x30, x30, 1\n"
			"la x17, rand_int_32\n"
			"flw.ps f30, 768(x17)\n"
			"flw.ps f7, 704(x17)\n"
			"fadd.pi f25, f30, f7\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f5, 992(x17)\n"
			"fround.ps f15, f5, rdn\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f9, 128(x7)\n"
			"flw.ps f15, 768(x7)\n"
			"flw.ps f8, 192(x7)\n"
			"fnmadd.s f15, f9, f15, f8, rmm\n" 
			"LBL_SEQID_0_FP_TRANS_ID_0_HID_14:\n"
			"la x13, rand_ieee754_32\n"
			"flw.ps f9, 736(x13)\n"
			"flog.ps f30, f9\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_14:\n"
			"feqm.ps m4, f0, f11\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x17, rand_ieee754_32\n"
			"flw.ps f14, 864(x17)\n"
			"fmax.ps f0, f14, f14\n" 
			"la x9, rand_int_32\n"
			"flw.ps f0, 256(x9)\n"
			"flw.ps f16, 0(x9)\n"
			"fmaxu.pi f6, f0, f16\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f14, 672(x7)\n"
			"flw.ps f6, 128(x7)\n"
			"flw.ps f12, 576(x7)\n"
			"fnmsub.s f12, f14, f6, f12, rmm\n" 
			"la x19, rand_int_32\n"
			"flw.ps f30, 480(x19)\n"
			"fnot.pi f7, f30\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f12, 640(x15)\n"
			"flw.ps f5, 352(x15)\n"
			"flw.ps f9, 256(x15)\n"
			"fmsub.ps f12, f12, f5, f9, rtz\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f0, 352(x22)\n"
			"flw.ps f16, 736(x22)\n"
			"fsub.s f24, f0, f16, rup\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f24, 480(x17)\n"
			"flw.ps f15, 768(x17)\n"
			"fmadd.s f5, f24, f15, f15, rup\n" 
			"la x22, rand_int_32\n"
			"flw.ps f7, 768(x22)\n"
			"fslli.pi f11, f7, 0xf\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f30, 160(x14)\n"
			"flw.ps f16, 640(x14)\n"
			"flw.ps f7, 992(x14)\n"
			"fnmadd.s f24, f30, f16, f7, rdn\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f6, 576(x17)\n"
			"flw.ps f9, 896(x17)\n"
			"fsgnjn.ps f11, f6, f9\n" 
			"la x14, rand_int_32\n"
			"flw.ps f1, 768(x14)\n"
			"fsatu8.pi f9, f1\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ID_316082_HID_14:\n"
			"li x5, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x2 << TENSOR_IMA_B_NUM_COLS) | (0x0 << TENSOR_IMA_A_NUM_ROWS) | (0xd << TENSOR_IMA_A_NUM_COLS) | (0x7 << TENSOR_IMA_OFFSET) | (0x0 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1b << TENSOR_IMA_SCP_LOC_B) | (0x50 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x0 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_14\n"
			"csrw tensor_error, x0\n"
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=0); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"li x5, 0x1cdf\n"
			"csrw tensor_mask, x5\n"	
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ITER_0_HID_14:\n"
			"la x31, self_check_16\n"
			"li x5, 0xa000000000000f\n"
			"add x5, x5, x31\n"
			"li x6, 0x320000000000000\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x2 << TENSOR_FMA16_B_NUM_COLS) | (0xf << TENSOR_FMA16_A_NUM_ROWS) | (0x0 << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x19 << TENSOR_FMA16_SCP_LOC_B) | (0x5 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x1d << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0xf << TENSOR_QUANT_QUANT_ROW) | (0x1b << TENSOR_QUANT_QUANT_SCP_LOC) | (0x2 << TENSOR_QUANT_TRANSF9) | (0x6 << TENSOR_QUANT_TRANSF8) | (0x6 << TENSOR_QUANT_TRANSF7) | (0x6 << TENSOR_QUANT_TRANSF6) | (0x6 << TENSOR_QUANT_TRANSF5) | (0x6 << TENSOR_QUANT_TRANSF4) | (0x7 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0x2\n"
			"bne x5, x31, LBL_FAIL_HID_14\n"
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_14\n"
			"csrw tensor_error, x0\n"
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_14\n"
			"addi x30, x30, 1\n"
			"la x5, rand_int_32\n"
			"flw.ps f16, 896(x5)\n"
			"flw.ps f24, 416(x5)\n"
			"fmulhu.pi f15, f16, f24\n" 
			"li x5, 0xbe430a9f1fbad990\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0xadcf69e0fb922ac0\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"LBL_SEQID_1_M0_WRITE_ID_2_HID_14:\n"
			"fltm.ps m0, f14, f30\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x18, rand_ieee754_32\n"
			"flw.ps f12, 576(x18)\n"
			"flw.ps f14, 448(x18)\n"
			"fmul.s f30, f12, f14, rmm\n" 
			"la x22, rand_int_32\n"
			"flw.ps f12, 192(x22)\n"
			"fnot.pi f30, f12\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f11, 448(x18)\n"
			"flw.ps f14, 512(x18)\n"
			"feq.s x18, f11, f14\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f15, 896(x17)\n"
			"ffrc.ps f7, f15\n" 
			"LBL_SEQID_1_M0_WRITE_ID_3_HID_14:\n"
			"maskxor m0, m2, m4\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x17, rand_ieee754_32\n"
			"flw.ps f15, 192(x17)\n"
			"fcvt.w.s x22, f15, rmm\n" 
			"la x17, rand_int_32\n"
			"flw.ps f25, 64(x17)\n"
			"flw.ps f11, 352(x17)\n"
			"fremu.pi f9, f25, f11\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f0, 224(x22)\n"
			"flw.ps f11, 288(x22)\n"
			"flw.ps f8, 768(x22)\n"
			"fnmadd.s f30, f0, f11, f8, rmm\n" 
			"LBL_SEQID_1_M0_WRITE_ID_4_HID_14:\n"
			"fltm.ps m0, f12, f8\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x5, rand_int_32\n"
			"flw.ps f16, 0(x5)\n"
			"flw.ps f24, 64(x5)\n"
			"fsll.pi f9, f16, f24\n" 
			"li x5, 0x388\n"
			"csrw tensor_mask, x5\n"	
			"la x22, rand_int_32\n"
			"flw.ps f16, 352(x22)\n"
			"flw.ps f9, 896(x22)\n"
			"feq.pi f11, f16, f9\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f16, 640(x17)\n"
			"fsqrt.ps f8, f16\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f7, 288(x18)\n"
			"frsq.ps f24, f7\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f6, 640(x17)\n"
			"flw.ps f14, 800(x17)\n"
			"flt.s x15, f6, f14\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_1_RDX_ID_1_SHIRE_0_HID_14:\n"
			"li x13, 0xa0000000010000a\n"
			"csrw tensor_reduce, x13\n" 
			"csrwi tensor_wait, 9\n" 
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ID_841168_HID_14:\n"
			"li x5, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x3 << TENSOR_IMA_B_NUM_COLS) | (0x0 << TENSOR_IMA_A_NUM_ROWS) | (0x3 << TENSOR_IMA_A_NUM_COLS) | (0x2 << TENSOR_IMA_OFFSET) | (0x0 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x90 << TENSOR_IMA_SCP_LOC_B) | (0x2b << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x0 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_14\n"
			"csrw tensor_error, x0\n"
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=0); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ITER_0_HID_14:\n"
			"li x6, ((0x4 << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0x0 << TENSOR_QUANT_QUANT_ROW) | (0x2e << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_14\n"
			"csrw tensor_error, x0\n"
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_14\n"
			"addi x30, x30, 1\n"
			"la x17, rand_int_32\n"
			"flw.ps f30, 384(x17)\n"
			"flw.ps f11, 416(x17)\n"
			"fxor.pi f16, f30, f11\n" 
			"la x19, rand_int_32\n"
			"flw.ps f3, 832(x19)\n"
			"flw.ps f8, 544(x19)\n"
			"fmulhu.pi f15, f3, f8\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f3, 832(x18)\n"
			"flw.ps f6, 640(x18)\n"
			"flw.ps f1, 960(x18)\n"
			"fnmadd.s f24, f3, f6, f1, rtz\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f3, 608(x18)\n"
			"fsin.ps f6, f3\n" 
			"la x18, rand_int_32\n"
			"flw.ps f15, 256(x18)\n"
			"flw.ps f6, 928(x18)\n"
			"for.pi f9, f15, f6\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f25, 768(x22)\n"
			"flw.ps f0, 32(x22)\n"
			"flw.ps f9, 352(x22)\n"
			"fmadd.ps f25, f25, f0, f9, rdn\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f14, 640(x22)\n"
			"flw.ps f16, 352(x22)\n"
			"fmin.ps f3, f14, f16\n" 
			"la x18, rand_int_32\n"
			"flw.ps f5, 512(x18)\n"
			"flw.ps f16, 256(x18)\n"
			"fmaxu.pi f15, f5, f16\n" 
			"LBL_SEQID_2_M0_WRITE_ID_5_HID_14:\n"
			"feqm.ps m5, f25, f8\n" 
			"li x5, CREDINC3\n"
			"ld x5, 0(x5)\n"
			"la x5, rand_ieee754_32\n"
			"flw.ps f0, 704(x5)\n"
			"flw.ps f5, 992(x5)\n"
			"fsgnj.ps f24, f0, f5\n" 
			"la x9, rand_int_32\n"
			"flw.ps f14, 608(x9)\n"
			"flw.ps f8, 256(x9)\n"
			"fadd.pi f8, f14, f8\n" 
			"la x14, rand_int_32\n"
			"flw.ps f1, 32(x14)\n"
			"fpackrepb.pi f30, f1\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x9, rand_ieee754_32\n"
			"flw.ps f25, 928(x9)\n"
			"flw.ps f9, 288(x9)\n"
			"flw.ps f15, 608(x9)\n"
			"fmsub.ps f11, f25, f9, f15, rup\n" 
			"la x19, rand_int_32\n"
			"flw.ps f5, 992(x19)\n"
			"fsrai.pi f9, f5, 0x1\n" 
			"la x19, rand_int_32\n"
			"flw.ps f8, 0(x19)\n"
			"flw.ps f24, 96(x19)\n"
			"fmulhu.pi f11, f8, f24\n" 
			"la x17, rand_int_32\n"
			"flw.ps f25, 864(x17)\n"
			"flw.ps f6, 256(x17)\n"
			"fmax.pi f0, f25, f6\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_2_XMA_QNT_STR_SEQ_ID_533967_HID_14:\n"
			"li x5, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x1 << TENSOR_FMA32_B_NUM_COLS) | (0x7 << TENSOR_FMA32_A_NUM_ROWS) | (0x2 << TENSOR_FMA32_A_NUM_COLS) | (0x9 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x83 << TENSOR_FMA32_SCP_LOC_B) | (0xe9 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x6, ((0x11 << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0x7 << TENSOR_QUANT_QUANT_ROW) | (0xe << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x7 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_2_XMA_QNT_STR_SEQ_ID_108492_HID_14:\n"
			"li x6, ((0x10 << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0x9 << TENSOR_QUANT_QUANT_ROW) | (0x3b << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x0 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 7\n"
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_14\n"
			"csrw tensor_error, x0\n"
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"LBL_SEQID_2_XMA_QNT_STR_SEQ_ITER_0_HID_14:\n"
			"li x6, ((0x11 << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0x0 << TENSOR_QUANT_QUANT_ROW) | (0xe << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_14\n"
			"csrw tensor_error, x0\n"
			"li x5, 3\n" 
			"bne x30, x5, LBL_FAIL_HID_14\n"
			"nop\n"          
		       VSNIP_RSV     
		);
		load_vpu_regs();
		__asm__ __volatile__ ( 
			"nop\n"          
			"addi x30, x30, 1\n"
			"la x18, rand_int_32\n"
			"flw.ps f15, 480(x18)\n"
			"fslli.pi f0, f15, 0x2\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f11, 448(x5)\n"
			"flw.ps f9, 512(x5)\n"
			"fsgnjx.ps f1, f11, f9\n" 
			"la x17, rand_int_32\n"
			"flw.ps f5, 160(x17)\n"
			"flw.ps f1, 544(x17)\n"
			"feq.pi f30, f5, f1\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f24, 384(x7)\n"
			"flw.ps f8, 768(x7)\n"
			"fsgnjn.s f0, f24, f8\n" 
			"LBL_SEQID_3_M0_WRITE_ID_6_HID_14:\n"
			"feqm.ps m2, f25, f25\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x7, rand_ieee754_32\n"
			"flw.ps f7, 64(x7)\n"
			"flw.ps f24, 576(x7)\n"
			"fadd.ps f16, f7, f24, rup\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f12, 800(x15)\n"
			"flw.ps f30, 96(x15)\n"
			"fmin.s f3, f12, f30\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f3, 64(x7)\n"
			"flw.ps f14, 544(x7)\n"
			"fmax.ps f11, f3, f14\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x15, rand_ieee754_32\n"
			"flw.ps f5, 704(x15)\n"
			"flw.ps f30, 160(x15)\n"
			"flw.ps f24, 992(x15)\n"
			"fnmadd.ps f15, f5, f30, f24, rup\n" 
			"LBL_SEQID_3_M0_WRITE_ID_7_HID_14:\n"
			"fsetm.pi m5, f1\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x19, rand_ieee754_32\n"
			"flw.ps f7, 800(x19)\n"
			"flw.ps f15, 864(x19)\n"
			"fsgnjx.s f8, f7, f15\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f8, 0(x18)\n"
			"flw.ps f16, 960(x18)\n"
			"flw.ps f24, 96(x18)\n"
			"fmadd.s f8, f8, f16, f24, rmm\n" 
			"li x5, 0x694f380e3ebe794d\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0x6607efde3bfd7a3c\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"la x17, rand_int_32\n"
			"flw.ps f11, 352(x17)\n"
			"fsatu8.pi f8, f11\n" 
			"li x5, 0xdb8d60890075b3c\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0x687f2780ca42a0ff\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"la x14, rand_ieee754_16\n"
			"flw.ps f16, 864(x14)\n"
			"fcvt.ps.f16 f11, f16\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f6, 352(x14)\n"
			"fsin.ps f6, f6\n" 
			"li x5, 0xae28\n"
			"csrw tensor_mask, x5\n"	
			"la x17, rand_ieee754_32\n"
			"flw.ps f15, 544(x17)\n"
			"flw.ps f30, 0(x17)\n"
			"fle.ps f7, f15, f30\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_3_TLD_XMA_STR_SEQ_ITER_0_HID_14:\n"
			"la x31, self_check_16\n"
			"li x5, 0xa000000000000d\n"
			"add x5, x5, x31\n"
			"li x6, 0x10000000000001\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x3 << TENSOR_FMA16_B_NUM_COLS) | (0xd << TENSOR_FMA16_A_NUM_ROWS) | (0x1 << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x1 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x18 << TENSOR_FMA16_SCP_LOC_B) | (0x5 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0x4\n"
			"bne x5, x31, LBL_FAIL_HID_14\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_14\n"
			"csrw tensor_error, x0\n"
			"li x5, 4\n" 
			"bne x30, x5, LBL_FAIL_HID_14\n"
			"nop\n"          
		       VSNIP_RSV     
		);
		trans();
		__asm__ __volatile__ ( 
			"nop\n"          
			"addi x30, x30, 1\n"
			"la x7, rand_ieee754_32\n"
			"flw.ps f30, 480(x7)\n"
			"flw.ps f14, 640(x7)\n"
			"fmax.s f14, f30, f14\n" 
			"la x17, rand_int_32\n"
			"flw.ps f15, 192(x17)\n"
			"flw.ps f9, 448(x17)\n"
			"fmin.pi f1, f15, f9\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f15, 640(x18)\n"
			"flw.ps f7, 800(x18)\n"
			"fnmadd.ps f25, f15, f7, f7, rup\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f15, 864(x5)\n"
			"flw.ps f6, 64(x5)\n"
			"fmadd.s f6, f15, f6, f6, rmm\n" 
			"la x9, rand_int_32\n"
			"flw.ps f0, 800(x9)\n"
			"flw.ps f12, 64(x9)\n"
			"fand.pi f7, f0, f12\n" 
			"LBL_SEQID_4_FP_TRANS_ID_8_HID_14:\n"
			"la x15, rand_ieee754_32\n"
			"flw.ps f12, 768(x15)\n"
			"fexp.ps f12, f12\n" 
			"li x5, 0x5949708f38d64c95\n"
			"fcvt.s.lu f7, x5, rmm\n" 
			"la x9, rand_int_32\n"
			"flw.ps f6, 384(x9)\n"
			"flw.ps f1, 576(x9)\n"
			"fmin.pi f30, f6, f1\n" 
			"la x15, rand_int_32\n"
			"flw.ps f0, 32(x15)\n"
			"fcvt.ps.pw f6, f0, rmm\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f16, 576(x5)\n"
			"flw.ps f11, 672(x5)\n"
			"fsgnjx.ps f0, f16, f11\n" 
			"maskpopcz x13, m5\n" 
			"la x14, rand_int_32\n"
			"flw.ps f8, 544(x14)\n"
			"flw.ps f14, 192(x14)\n"
			"fmulh.pi f30, f8, f14\n" 
			"la x15, rand_int_32\n"
			"flw.ps f1, 800(x15)\n"
			"fsrli.pi f6, f1, 0x9\n" 
			"la x5, rand_int_32\n"
			"flw.ps f15, 736(x5)\n"
			"flw.ps f12, 128(x5)\n"
			"fmaxu.pi f11, f15, f12\n" 
			"la x17, rand_int_32\n"
			"flw.ps f1, 0(x17)\n"
			"faddi.pi f9, f1, 0x105\n" 
			"LBL_SEQID_4_FP_TRANS_ID_9_HID_14:\n"
			"la x17, rand_ieee754_32\n"
			"flw.ps f16, 896(x17)\n"
			"frcp.ps f6, f16\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 0xee1572fd57610664\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0x9589cfc3ed042536\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"LBL_SEQID_4_TLD_XMA_STR_SEQ_ID_730249_HID_14:\n"
			"la x31, self_check_16\n"
			"li x5, 0xc000000000000b\n"
			"add x5, x5, x31\n"
			"li x6, 0x1000000000000e\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x3 << TENSOR_FMA16_B_NUM_COLS) | (0xb << TENSOR_FMA16_A_NUM_ROWS) | (0xe << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x1 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1a << TENSOR_FMA16_SCP_LOC_B) | (0x6 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x0 << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0xb << TENSOR_QUANT_QUANT_ROW) | (0x3e << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x2 << TENSOR_QUANT_TRANSF7) | (0x6 << TENSOR_QUANT_TRANSF6) | (0x7 << TENSOR_QUANT_TRANSF5) | (0x6 << TENSOR_QUANT_TRANSF4) | (0x7 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0x1e\n"
			"bne x5, x31, LBL_FAIL_HID_14\n"
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 10\n"
			"LBL_SEQID_4_TLD_XMA_STR_SEQ_ID_952592_HID_14:\n"
			"la x31, self_check_16\n"
			"li x5, 0x80000000000001\n"
			"add x5, x5, x31\n"
			"li x6, 0x3c0000000000003\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x1 << TENSOR_FMA16_B_NUM_COLS) | (0x1 << TENSOR_FMA16_A_NUM_ROWS) | (0x3 << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1e << TENSOR_FMA16_SCP_LOC_B) | (0x4 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0x8\n"
			"bne x5, x31, LBL_FAIL_HID_14\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_14\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_4_TLD_XMA_STR_SEQ_ITER_0_HID_14:\n"
			"la x31, self_check_8\n"
			"li x5, 0xc0000000000009\n"
			"add x5, x5, x31\n"
			"li x6, 0x1000000000000a\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x1 << TENSOR_IMA_B_NUM_COLS) | (0x9 << TENSOR_IMA_A_NUM_ROWS) | (0xa << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x1 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1e << TENSOR_IMA_SCP_LOC_B) | (0x6 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x9, ((0x17 << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0x9 << TENSOR_QUANT_QUANT_ROW) | (0x35 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x0 << TENSOR_QUANT_TRANSF1) | (0x0 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fmvz.x.ps x31, f0, 0\n"
			"li x5, 0x2c\n"
			"bne x5, x31, LBL_FAIL_HID_14\n"
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 10\n"
			"LBL_SEQID_4_TLD_XMA_STR_SEQ_ITER_1_HID_14:\n"
			"la x31, self_check_16\n"
			"li x5, 0xe0000000000001\n"
			"add x5, x5, x31\n"
			"li x6, 0x10000000000001\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x2 << TENSOR_FMA16_B_NUM_COLS) | (0x1 << TENSOR_FMA16_A_NUM_ROWS) | (0x1 << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x1 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1a << TENSOR_FMA16_SCP_LOC_B) | (0x7 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0x4\n"
			"bne x5, x31, LBL_FAIL_HID_14\n"
			"LBL_SEQID_4_TLD_XMA_STR_SEQ_ITER_2_HID_14:\n"
			"la x31, self_check_16\n"
			"li x5, 0x4000000000000f\n"
			"add x5, x5, x31\n"
			"li x6, 0x1000000000000e\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x3 << TENSOR_FMA16_B_NUM_COLS) | (0xf << TENSOR_FMA16_A_NUM_ROWS) | (0xe << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x1 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1c << TENSOR_FMA16_SCP_LOC_B) | (0x2 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0x1e\n"
			"bne x5, x31, LBL_FAIL_HID_14\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_14\n"
			"csrw tensor_error, x0\n"
			"li x5, 5\n" 
			"bne x30, x5, LBL_FAIL_HID_14\n"
			"nop\n"          
		       VSNIP_RSV     
		);
		load_vpu_regs();
		__asm__ __volatile__ ( 
			"nop\n"          
			"addi x30, x30, 1\n"
			"la x14, rand_ieee754_32\n"
			"flw.ps f30, 224(x14)\n"
			"flw.ps f11, 256(x14)\n"
			"fsgnj.s f0, f30, f11\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f25, 64(x9)\n"
			"flw.ps f16, 96(x9)\n"
			"fsgnj.ps f5, f25, f16\n" 
			"LBL_SEQID_5_M0_WRITE_ID_10_HID_14:\n"
			"feqm.ps m0, f12, f12\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x15, rand_ieee754_32\n"
			"flw.ps f7, 128(x15)\n"
			"flw.ps f25, 896(x15)\n"
			"fmax.ps f11, f7, f25\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f6, 416(x5)\n"
			"flw.ps f7, 256(x5)\n"
			"fsgnj.ps f15, f6, f7\n" 
			"la x17, rand_int_32\n"
			"flw.ps f0, 160(x17)\n"
			"flw.ps f11, 416(x17)\n"
			"fsrl.pi f8, f0, f11\n" 
			"la x17, rand_int_32\n"
			"flw.ps f6, 480(x17)\n"
			"flw.ps f1, 480(x17)\n"
			"fdiv.pi f8, f6, f1\n" 
			"la x17, rand_int_32\n"
			"flw.ps f14, 96(x17)\n"
			"fcvt.ps.pw f0, f14, rdn\n" 
			"la x5, rand_int_32\n"
			"flw.ps f30, 704(x5)\n"
			"fsatu8.pi f5, f30\n" 
			"la x19, rand_int_32\n"
			"flw.ps f15, 672(x19)\n"
			"flw.ps f5, 128(x19)\n"
			"fmaxu.pi f3, f15, f5\n" 
			"la x22, rand_int_32\n"
			"flw.ps f7, 256(x22)\n"
			"flw.ps f16, 704(x22)\n"
			"fand.pi f9, f7, f16\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f6, 256(x17)\n"
			"flw.ps f3, 928(x17)\n"
			"flw.ps f24, 992(x17)\n"
			"fnmadd.ps f16, f6, f3, f24, rne\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f7, 448(x18)\n"
			"flw.ps f6, 288(x18)\n"
			"flw.ps f3, 0(x18)\n"
			"fmadd.s f12, f7, f6, f3, rmm\n" 
			"la x7, rand_int_32\n"
			"flw.ps f1, 576(x7)\n"
			"flw.ps f11, 800(x7)\n"
			"fmulhu.pi f5, f1, f11\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f30, 800(x18)\n"
			"fclass.ps f6, f30\n" 
			"LBL_SEQID_5_M0_WRITE_ID_11_HID_14:\n"
			"fltm.pi m5, f1, f11\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_5_XMA_QNT_STR_SEQ_ITER_0_HID_14:\n"
			"li x6, ((0x11 << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0x1 << TENSOR_QUANT_QUANT_ROW) | (0x1b << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x1 << TENSOR_QUANT_TRANSF1) | (0x5 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_14\n"
			"csrw tensor_error, x0\n"
			"li x5, 6\n" 
			"bne x30, x5, LBL_FAIL_HID_14\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_6_M0_WRITE_ID_12_HID_14:\n"
			"fltm.ps m0, f30, f14\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x22, rand_ieee754_32\n"
			"flw.ps f9, 800(x22)\n"
			"flw.ps f14, 928(x22)\n"
			"flt.s x5, f9, f14\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x5, rand_ieee754_32\n"
			"flw.ps f12, 640(x5)\n"
			"fsin.ps f3, f12\n" 
			"la x9, rand_int_32\n"
			"flw.ps f14, 384(x9)\n"
			"flw.ps f8, 480(x9)\n"
			"fsra.pi f1, f14, f8\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f11, 320(x15)\n"
			"flw.ps f7, 640(x15)\n"
			"fsub.ps f24, f11, f7, rtz\n" 
			"LBL_SEQID_6_M0_WRITE_ID_13_HID_14:\n"
			"fsetm.pi m5, f30\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"LBL_SEQID_6_M0_WRITE_ID_14_HID_14:\n"
			"fsetm.pi m0, f11\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_6_M0_WRITE_ID_15_HID_14:\n"
			"fltm.pi m5, f9, f9\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_6_FP_TRANS_ID_16_HID_14:\n"
			"la x17, rand_ieee754_32\n"
			"flw.ps f16, 0(x17)\n"
			"fexp.ps f6, f16\n" 
			"la x22, rand_int_32\n"
			"flw.ps f25, 192(x22)\n"
			"fsatu8.pi f16, f25\n" 
			"LBL_SEQID_6_M0_WRITE_ID_17_HID_14:\n"
			"fsetm.pi m4, f5\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x22, rand_ieee754_32\n"
			"flw.ps f12, 64(x22)\n"
			"ffrc.ps f9, f12\n" 
			"li x5, 0xb37875d31feea9be\n"
			"fcvt.s.wu f11, x5, rne\n" 
			"la x13, rand_int_32\n"
			"flw.ps f7, 224(x13)\n"
			"fcvt.ps.pwu f30, f7, rne\n" 
			"la x19, rand_int_32\n"
			"flw.ps f12, 224(x19)\n"
			"flw.ps f8, 672(x19)\n"
			"fsub.pi f9, f12, f8\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f5, 32(x7)\n"
			"flw.ps f7, 672(x7)\n"
			"fsgnjn.ps f14, f5, f7\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_6_TLD_XMA_STR_SEQ_ITER_0_HID_14:\n"
			"la x31, self_check_8\n"
			"li x5, 0xe0000000000006\n"
			"add x5, x5, x31\n"
			"li x6, 0x3c0000000000005\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x1 << TENSOR_IMA_B_NUM_COLS) | (0x6 << TENSOR_IMA_A_NUM_ROWS) | (0x5 << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1e << TENSOR_IMA_SCP_LOC_B) | (0x7 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x9, ((0xb << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0x6 << TENSOR_QUANT_QUANT_ROW) | (0x27 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x4 << TENSOR_QUANT_TRANSF6) | (0x8 << TENSOR_QUANT_TRANSF5) | (0x9 << TENSOR_QUANT_TRANSF4) | (0x8 << TENSOR_QUANT_TRANSF3) | (0x9 << TENSOR_QUANT_TRANSF2) | (0x9 << TENSOR_QUANT_TRANSF1) | (0xa << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fmvz.x.ps x31, f0, 0\n"
			"li x5, 0x18\n"
			"bne x5, x31, LBL_FAIL_HID_14\n"
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 10\n"
			"LBL_SEQID_6_TLD_XMA_STR_SEQ_ITER_1_HID_14:\n"
			"la x31, self_check_16\n"
			"li x5, 0x20000000000007\n"
			"add x5, x5, x31\n"
			"li x6, 0x10000000000003\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x1 << TENSOR_FMA16_B_NUM_COLS) | (0x7 << TENSOR_FMA16_A_NUM_ROWS) | (0x3 << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x1 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x18 << TENSOR_FMA16_SCP_LOC_B) | (0x1 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x1 << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0x7 << TENSOR_QUANT_QUANT_ROW) | (0x10 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x7 << TENSOR_QUANT_TRANSF6) | (0x7 << TENSOR_QUANT_TRANSF5) | (0x6 << TENSOR_QUANT_TRANSF4) | (0x6 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0x8\n"
			"bne x5, x31, LBL_FAIL_HID_14\n"
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_14\n"
			"csrw tensor_error, x0\n"
			"li x5, 7\n" 
			"bne x30, x5, LBL_FAIL_HID_14\n"
			"nop \n"
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=1); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"nop \n"
			"addi x29, x29, -1\n"
			"bne x29, x0, LBL_OUTER_LOOP_SEQ_HID_14\n"
			"nop\n"          
		       VSNIP_RSV     
		);
		load_vpu_regs();
		__asm__ __volatile__ ( 
			"nop\n"          
			"nop\n"          
		       VSNIP_RSV     
		);
		trans();
		__asm__ __volatile__ ( 
			"nop\n"          
			"la x31, check_sum_hid_14\n"
			"lw x5, (x31)\n"
			"bne x30, x5, LBL_FAIL_HID_14\n"
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
			"csrrci x0, mcache_control, 3\n"
			"csrwi tensor_wait, 0x6\n" 
			"fence\n"
			"csrwi tensor_wait, 0x6\n" 
			"fence\n"
			"csrwi excl_mode, 0\n"
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=1); 
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
	if ( hid%16 == 15 ) { 
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
			"csrwi mhpmevent3, 11\n"   
			"csrwi mhpmevent4, 1\n"   
			"csrwi mhpmevent5, 6\n"   
			"csrwi mhpmevent6, 12\n"   
			"csrwi mhpmevent7, 23\n"   
			"csrwi mhpmevent8, 21\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f29,  320 (x5)\n"
			"flw.ps f8,  992 (x5)\n"
			"flw.ps f31,  640 (x5)\n"
			"flw.ps f20,  288 (x5)\n"
			"flw.ps f16,  96 (x5)\n"
			"flw.ps f22,  576 (x5)\n"
			"flw.ps f30,  384 (x5)\n"
			"flw.ps f5,  448 (x5)\n"
			"flw.ps f14,  224 (x5)\n"
			"flw.ps f4,  128 (x5)\n"
			"flw.ps f17,  480 (x5)\n"
			"flw.ps f27,  896 (x5)\n"
			"flw.ps f11,  736 (x5)\n"
			"flw.ps f7,  256 (x5)\n"
			"flw.ps f9,  608 (x5)\n"
			"flw.ps f19,  928 (x5)\n"
			"flw.ps f23,  352 (x5)\n"
			"flw.ps f6,  960 (x5)\n"
			"flw.ps f13,  768 (x5)\n"
			"flw.ps f15,  192 (x5)\n"
			"flw.ps f25,  416 (x5)\n"
			"flw.ps f3,  832 (x5)\n"
			"flw.ps f26,  704 (x5)\n"
			"flw.ps f18,  32 (x5)\n"
			"flw.ps f1,  160 (x5)\n"
			"flw.ps f2,  544 (x5)\n"
			"flw.ps f12,  800 (x5)\n"
			"flw.ps f21,  672 (x5)\n"
			"flw.ps f28,  864 (x5)\n"
			"flw.ps f0,  0 (x5)\n"
			"flw.ps f10,  512 (x5)\n"
			"flw.ps f24,  64 (x5)\n"
			"csrw fflags, zero\n"
			"csrwi frm, 0x0\n"
			"li x31, 100\n"
			"1:\n" 
			"addi x31, x31, -1\n"
			"bne x31, x0, 1b\n" 
			"csrw fcc, x0\n" 
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=1); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"LBL_KERNEL_HID_15:\n"          
			"li x5, 0xFEEDFEED\n"
			"slti x0, x0, 0x7f2\n"
			"la x31, check_sum_hid_15\n"
			"li x5, 0x7\n" 
			"sw x5, (x31)\n"
			"li x29, 0x7\n"
			"LBL_OUTER_LOOP_SEQ_HID_15:\n"
			"nop \n"
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=1); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"nop \n"
			"li x30, 0\n"
			"addi x30, x30, 1\n"
			"la x9, rand_int_32\n"
			"flw.ps f27, 96(x9)\n"
			"fslli.pi f12, f27, 0xa\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f7, 480(x9)\n"
			"fle.ps f7, f7, f7\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f27, 640(x16)\n"
			"flw.ps f15, 512(x16)\n"
			"flt.ps f21, f27, f15\n" 
			"la x25, rand_int_32\n"
			"flw.ps f16, 416(x25)\n"
			"fmaxu.pi f20, f16, f16\n" 
			"la x25, rand_int_32\n"
			"flw.ps f8, 576(x25)\n"
			"flw.ps f15, 576(x25)\n"
			"fsub.pi f16, f8, f15\n" 
			"la x9, rand_int_32\n"
			"flw.ps f6, 352(x9)\n"
			"flw.ps f22, 128(x9)\n"
			"fxor.pi f2, f6, f22\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_15:\n"
			"maskor m4, m5, m3\n" 
			"li x5, CREDINC2\n"
			"ld x5, 0(x5)\n"
			"la x25, rand_int_32\n"
			"flw.ps f27, 800(x25)\n"
			"flw.ps f12, 448(x25)\n"
			"fsra.pi f7, f27, f12\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f7, 512(x20)\n"
			"flw.ps f10, 320(x20)\n"
			"flw.ps f27, 736(x20)\n"
			"fmadd.ps f26, f7, f10, f27, rne\n" 
			"la x22, rand_int_32\n"
			"flw.ps f21, 480(x22)\n"
			"fpackrepb.pi f22, f21\n" 
			"LBL_SEQID_0_FP_TRANS_ID_1_HID_15:\n"
			"la x16, rand_ieee754_32\n"
			"flw.ps f22, 416(x16)\n"
			"flog.ps f16, f22\n" 
			"li x18, 0x5568635d6b8414d0\n"
			"fcvt.s.wu f6, x18, rmm\n" 
			"mova.x.m x13\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f6, 256(x7)\n"
			"flw.ps f15, 0(x7)\n"
			"flw.ps f7, 992(x7)\n"
			"fnmadd.s f8, f6, f15, f7, rdn\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f19, 96(x25)\n"
			"flw.ps f2, 480(x25)\n"
			"fsgnj.s f15, f19, f2\n" 
			"LBL_SEQID_0_M0_WRITE_ID_2_HID_15:\n"
			"feqm.ps m0, f15, f15\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"csrw tensor_error, zero\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_15\n"
			"addi x30, x30, 1\n"
			"la x9, rand_int_32\n"
			"flw.ps f2, 864(x9)\n"
			"fpackrepb.pi f24, f2\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f24, 64(x9)\n"
			"flw.ps f8, 0(x9)\n"
			"fmul.s f15, f24, f8, rtz\n" 
			"LBL_SEQID_1_M0_WRITE_ID_3_HID_15:\n"
			"mov.m.x m5, x22, 0x7d\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x16, rand_ieee754_32\n"
			"flw.ps f21, 416(x16)\n"
			"flw.ps f22, 768(x16)\n"
			"fmin.ps f27, f21, f22\n" 
			"la x9, rand_int_32\n"
			"flw.ps f27, 448(x9)\n"
			"flw.ps f16, 672(x9)\n"
			"fminu.pi f7, f27, f16\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f21, 320(x9)\n"
			"flw.ps f20, 0(x9)\n"
			"fsub.s f27, f21, f20, rtz\n" 
			"la x20, rand_int_32\n"
			"flw.ps f26, 640(x20)\n"
			"flw.ps f24, 672(x20)\n"
			"fsub.pi f20, f26, f24\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f8, 256(x10)\n"
			"flw.ps f22, 128(x10)\n"
			"fmsub.ps f15, f8, f22, f22, rup\n" 
			"la x23, rand_int_32\n"
			"flw.ps f21, 416(x23)\n"
			"fsrai.pi f7, f21, 0xe\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f2, 480(x18)\n"
			"fcvt.f16.ps f15, f2\n" 
			"la x7, rand_int_32\n"
			"flw.ps f15, 96(x7)\n"
			"flw.ps f20, 992(x7)\n"
			"fmin.pi f12, f15, f20\n" 
			"la x13, rand_int_32\n"
			"flw.ps f11, 256(x13)\n"
			"flw.ps f7, 96(x13)\n"
			"feq.pi f16, f11, f7\n" 
			"la x18, rand_int_32\n"
			"flw.ps f15, 0(x18)\n"
			"flw.ps f12, 768(x18)\n"
			"fxor.pi f7, f15, f12\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f21, 64(x10)\n"
			"flw.ps f20, 256(x10)\n"
			"fsgnjn.s f16, f21, f20\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f10, 416(x23)\n"
			"flw.ps f2, 608(x23)\n"
			"flw.ps f21, 352(x23)\n"
			"fmadd.ps f16, f10, f2, f21, rup\n" 
			"la x15, rand_int_32\n"
			"flw.ps f20, 32(x15)\n"
			"fpackrepb.pi f16, f20\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_15\n"
			"addi x30, x30, 1\n"
			"la x15, rand_int_32\n"
			"flw.ps f2, 992(x15)\n"
			"fslli.pi f20, f2, 0x0\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f22, 928(x15)\n"
			"flw.ps f21, 736(x15)\n"
			"flw.ps f10, 736(x15)\n"
			"fmadd.s f8, f22, f21, f10, rne\n" 
			"la x15, rand_int_32\n"
			"flw.ps f2, 64(x15)\n"
			"flw.ps f24, 896(x15)\n"
			"fxor.pi f19, f2, f24\n" 
			"maskpopcz x22, m0\n" 
			"mov.m.x m0, x0, 0xff\n"
			"la x22, rand_int_32\n"
			"flw.ps f27, 128(x22)\n"
			"fsrai.pi f11, f27, 0x5\n" 
			"LBL_SEQID_2_M0_WRITE_ID_4_HID_15:\n"
			"maskxor m3, m3, m5\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x23, rand_int_32\n"
			"flw.ps f24, 768(x23)\n"
			"flw.ps f15, 992(x23)\n"
			"flt.pi f19, f24, f15\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f21, 384(x20)\n"
			"fcvt.f16.ps f12, f21\n" 
			"la x13, rand_int_32\n"
			"flw.ps f15, 832(x13)\n"
			"faddi.pi f12, f15, 0x188\n" 
			"li x10, 0x518d51f8d020d905\n"
			"fcvt.s.wu f16, x10, rmm\n" 
			"LBL_SEQID_2_M0_WRITE_ID_5_HID_15:\n"
			"fltm.pi m0, f16, f22\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x15, rand_ieee754_32\n"
			"flw.ps f10, 864(x15)\n"
			"flw.ps f26, 416(x15)\n"
			"flw.ps f7, 864(x15)\n"
			"fcmov.ps f11, f10, f26, f7\n" 
			"la x9, rand_int_32\n"
			"flw.ps f12, 576(x9)\n"
			"flw.ps f21, 864(x9)\n"
			"feq.pi f26, f12, f21\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f19, 640(x15)\n"
			"flw.ps f6, 32(x15)\n"
			"fsgnj.ps f8, f19, f6\n" 
			"la x10, rand_int_32\n"
			"flw.ps f8, 928(x10)\n"
			"fcvt.ps.pw f27, f8, rdn\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f20, 512(x15)\n"
			"fswizz.ps f7, f20, 0x1e\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 3\n" 
			"bne x30, x5, LBL_FAIL_HID_15\n"
			"nop\n"          
		       VSNIP_RSV     
		);
		load_vpu_regs();
		__asm__ __volatile__ ( 
			"nop\n"          
			"addi x30, x30, 1\n"
			"la x23, rand_ieee754_32\n"
			"flw.ps f26, 800(x23)\n"
			"flw.ps f7, 672(x23)\n"
			"fle.ps f8, f26, f7\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f2, 576(x15)\n"
			"flw.ps f11, 288(x15)\n"
			"fcmovm.ps f8, f2, f11\n" 
			"la x22, rand_int_32\n"
			"flw.ps f16, 128(x22)\n"
			"flw.ps f11, 768(x22)\n"
			"fltu.pi f24, f16, f11\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f10, 128(x15)\n"
			"fle.ps f26, f10, f10\n" 
			"la x16, rand_int_32\n"
			"flw.ps f20, 160(x16)\n"
			"fcvt.ps.pwu f11, f20, rne\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f12, 608(x15)\n"
			"frsq.ps f10, f12\n" 
			"maskpopcz x13, m4\n" 
			"la x15, rand_int_32\n"
			"flw.ps f8, 384(x15)\n"
			"fsub.pi f6, f8, f8\n" 
			"LBL_SEQID_3_FP_TRANS_ID_6_HID_15:\n"
			"la x25, rand_ieee754_32\n"
			"flw.ps f26, 960(x25)\n"
			"frcp.ps f19, f26\n" 
			"la x22, rand_int_32\n"
			"flw.ps f10, 64(x22)\n"
			"flw.ps f19, 192(x22)\n"
			"fxor.pi f8, f10, f19\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f6, 864(x15)\n"
			"flw.ps f20, 640(x15)\n"
			"fsub.s f11, f6, f20, rup\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f16, 832(x23)\n"
			"flw.ps f12, 352(x23)\n"
			"flt.s x10, f16, f12\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f7, 32(x9)\n"
			"fcvt.pw.ps f6, f7, rne\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f6, 416(x20)\n"
			"frsq.ps f22, f6\n" 
			"LBL_SEQID_3_M0_WRITE_ID_7_HID_15:\n"
			"mova.m.x x9\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x20, rand_int_32\n"
			"flw.ps f10, 96(x20)\n"
			"fpackreph.pi f16, f10\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 4\n" 
			"bne x30, x5, LBL_FAIL_HID_15\n"
			"nop\n"          
		       VSNIP_RSV     
		);
		trans();
		__asm__ __volatile__ ( 
			"nop\n"          
			"addi x30, x30, 1\n"
			"la x25, rand_ieee754_32\n"
			"flw.ps f24, 960(x25)\n"
			"flw.ps f20, 608(x25)\n"
			"fadd.ps f7, f24, f20, rdn\n" 
			"LBL_SEQID_4_M0_WRITE_ID_8_HID_15:\n"
			"maskand m3, m0, m0\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x25, rand_ieee754_32\n"
			"flw.ps f2, 128(x25)\n"
			"flw.ps f27, 928(x25)\n"
			"fsgnjx.s f6, f2, f27\n" 
			"la x13, rand_int_32\n"
			"flw.ps f16, 928(x13)\n"
			"flw.ps f21, 224(x13)\n"
			"fremu.pi f16, f16, f21\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f20, 896(x25)\n"
			"flw.ps f6, 512(x25)\n"
			"fsgnjx.s f15, f20, f6\n" 
			"la x13, rand_int_32\n"
			"flw.ps f19, 480(x13)\n"
			"fslli.pi f12, f19, 0x8\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f11, 544(x22)\n"
			"fclass.ps f10, f11\n" 
			"la x15, rand_int_32\n"
			"flw.ps f21, 896(x15)\n"
			"flw.ps f2, 352(x15)\n"
			"fadd.pi f20, f21, f2\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f8, 832(x9)\n"
			"flw.ps f19, 800(x9)\n"
			"fnmsub.ps f16, f8, f8, f19, rmm\n" 
			"la x25, rand_int_32\n"
			"flw.ps f12, 768(x25)\n"
			"fsrli.pi f20, f12, 0xc\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f11, 224(x10)\n"
			"flw.ps f24, 512(x10)\n"
			"flw.ps f8, 608(x10)\n"
			"fnmsub.ps f22, f11, f24, f8, rdn\n" 
			"maskpopcz x25, m0\n" 
			"mov.m.x m0, x0, 0xff\n"
			"la x22, rand_ieee754_32\n"
			"flw.ps f2, 0(x22)\n"
			"flw.ps f7, 128(x22)\n"
			"fmax.s f19, f2, f7\n" 
			"la x16, rand_int_32\n"
			"flw.ps f22, 288(x16)\n"
			"faddi.pi f16, f22, 0x6d\n" 
			"la x13, rand_int_32\n"
			"flw.ps f24, 672(x13)\n"
			"fnot.pi f7, f24\n" 
			"LBL_SEQID_4_M0_WRITE_ID_9_HID_15:\n"
			"mov.m.x m5, x23, 0x4d\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"csrw tensor_error, zero\n" 
			"li x5, 5\n" 
			"bne x30, x5, LBL_FAIL_HID_15\n"
			"nop\n"          
		       VSNIP_RSV     
		);
		load_vpu_regs();
		__asm__ __volatile__ ( 
			"nop\n"          
			"addi x30, x30, 1\n"
			"la x16, rand_ieee754_32\n"
			"flw.ps f2, 160(x16)\n"
			"flw.ps f7, 192(x16)\n"
			"flt.s x23, f2, f7\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f19, 288(x10)\n"
			"fcvt.w.s x15, f19, rne\n" 
			"la x9, rand_int_32\n"
			"flw.ps f12, 704(x9)\n"
			"fmin.pi f7, f12, f12\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f26, 384(x7)\n"
			"fcvt.pwu.ps f27, f26, rup\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f12, 672(x10)\n"
			"flw.ps f15, 704(x10)\n"
			"fmul.ps f21, f12, f15, rmm\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f7, 448(x20)\n"
			"flw.ps f27, 128(x20)\n"
			"feq.ps f15, f7, f27\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f6, 928(x10)\n"
			"fswizz.ps f24, f6, 0x1c\n" 
			"la x16, rand_int_32\n"
			"flw.ps f20, 288(x16)\n"
			"fcvt.ps.pw f24, f20, rne\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f22, 608(x7)\n"
			"flw.ps f10, 64(x7)\n"
			"fsgnjn.s f7, f22, f10\n" 
			"la x23, rand_int_32\n"
			"flw.ps f21, 800(x23)\n"
			"fslli.pi f2, f21, 0xd\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f24, 32(x23)\n"
			"flw.ps f6, 896(x23)\n"
			"feq.ps f2, f24, f6\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f6, 896(x15)\n"
			"flw.ps f16, 64(x15)\n"
			"fsub.s f11, f6, f16, rmm\n" 
			"la x23, rand_int_32\n"
			"flw.ps f16, 416(x23)\n"
			"flw.ps f8, 128(x23)\n"
			"fmul.pi f27, f16, f8\n" 
			"la x15, rand_int_32\n"
			"flw.ps f7, 32(x15)\n"
			"flw.ps f27, 608(x15)\n"
			"fadd.pi f22, f7, f27\n" 
			"la x25, rand_int_32\n"
			"flw.ps f11, 160(x25)\n"
			"flw.ps f19, 192(x25)\n"
			"fltu.pi f10, f11, f19\n" 
			"LBL_SEQID_5_FP_TRANS_ID_10_HID_15:\n"
			"la x13, rand_ieee754_32\n"
			"flw.ps f2, 576(x13)\n"
			"fexp.ps f26, f2\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 6\n" 
			"bne x30, x5, LBL_FAIL_HID_15\n"
			"addi x30, x30, 1\n"
			"la x22, rand_ieee754_32\n"
			"flw.ps f16, 736(x22)\n"
			"flw.ps f21, 832(x22)\n"
			"fmul.s f20, f16, f21, rne\n" 
			"la x16, rand_int_32\n"
			"flw.ps f2, 704(x16)\n"
			"flw.ps f10, 864(x16)\n"
			"fsrl.pi f15, f2, f10\n" 
			"la x16, rand_int_32\n"
			"flw.ps f16, 32(x16)\n"
			"flw.ps f19, 32(x16)\n"
			"feq.pi f21, f16, f19\n" 
			"la x22, rand_int_32\n"
			"flw.ps f24, 192(x22)\n"
			"flw.ps f26, 160(x22)\n"
			"fle.pi f16, f24, f26\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f16, 832(x15)\n"
			"flw.ps f22, 704(x15)\n"
			"fadd.ps f27, f16, f22, rne\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f12, 416(x22)\n"
			"fcvt.pw.ps f20, f12, rne\n" 
			"la x18, rand_int_32\n"
			"flw.ps f20, 416(x18)\n"
			"fcvt.ps.pw f22, f20, rtz\n" 
			"maskpopcz x10, m5\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f26, 320(x25)\n"
			"ffrc.ps f7, f26\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f16, 544(x9)\n"
			"flw.ps f7, 320(x9)\n"
			"feq.s x10, f16, f7\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f16, 544(x25)\n"
			"flw.ps f7, 512(x25)\n"
			"fmul.s f10, f16, f7, rup\n" 
			"li x13, 0x546591c5b7db4570\n"
			"fcvt.s.w f7, x13, rup\n" 
			"la x10, rand_int_32\n"
			"flw.ps f2, 192(x10)\n"
			"flw.ps f27, 544(x10)\n"
			"fsrl.pi f24, f2, f27\n" 
			"la x7, rand_int_32\n"
			"flw.ps f12, 704(x7)\n"
			"flw.ps f10, 512(x7)\n"
			"fltu.pi f19, f12, f10\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f7, 800(x10)\n"
			"fcvt.wu.s x7, f7, rne\n" 
			"la x22, rand_int_32\n"
			"flw.ps f12, 576(x22)\n"
			"flw.ps f16, 64(x22)\n"
			"fle.pi f15, f12, f16\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 7\n" 
			"bne x30, x5, LBL_FAIL_HID_15\n"
			"nop \n"
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=1); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"nop \n"
			"addi x29, x29, -1\n"
			"bne x29, x0, LBL_OUTER_LOOP_SEQ_HID_15\n"
			"nop\n"          
		       VSNIP_RSV     
		);
		load_vpu_regs();
		__asm__ __volatile__ ( 
			"nop\n"          
			"nop\n"          
		       VSNIP_RSV     
		);
		trans();
		__asm__ __volatile__ ( 
			"nop\n"          
			"la x31, check_sum_hid_15\n"
			"lw x5, (x31)\n"
			"bne x30, x5, LBL_FAIL_HID_15\n"
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
		c_sync_point(sync_odd_harts=1); 
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
		0xbec96351, 
		0x1c37399d, 
		0x139f1ecc, 
		0x2c2f3762, 
		0x82d3175e, 
		0xcc9ede4f, 
		0x4906e884, 
		0x839107c3, 
		0xb4f4c661, 
		0xb3d71fde, 
		0xa0840bb6, 
		0x9aea6bf6, 
		0xaa9705f6, 
		0x7d804a6c, 
		0xc61f5dbf, 
		0xc8e8425b, 
		0xbf2ba231, 
		0x86240f78, 
		0x290e1a45, 
		0x92a06d43, 
		0xd485e6a7, 
		0x49cb996e, 
		0x7c5f8676, 
		0xed5b9c9a, 
		0x3496bbef, 
		0xd1b98336, 
		0x4d7c5192, 
		0xf5791d7d, 
		0x35fa8ab7, 
		0xa44aa5fd, 
		0xff0e8b5b, 
		0xf7dbdf25, 
		0xfa300d15, 
		0x35e07088, 
		0x41eb4ad3, 
		0x4f11cba6, 
		0xb3b1b410, 
		0xc591a182, 
		0x9a86ed3d, 
		0x85bbc69f, 
		0xf69490c5, 
		0x08059d2e, 
		0x97fcf2cf, 
		0x83eaeeba, 
		0x26bf8109, 
		0xd059ab8c, 
		0xc4fecc8f, 
		0x1da5a195, 
		0x1d36d7e7, 
		0xc83df0ad, 
		0x6425ab4b, 
		0x23f1e436, 
		0x64aa5cff, 
		0xbb87cdaf, 
		0x16b32a32, 
		0xf42a23b5, 
		0xd9197d71, 
		0x6e34d0c3, 
		0x457d4f93, 
		0xe98dd9c8, 
		0x3a7df6cd, 
		0x1c758113, 
		0x0c16007a, 
		0x1c8936ea, 
		0x19b510df, 
		0x557bca99, 
		0x9bd7f4e9, 
		0x6218293d, 
		0xf7cadc93, 
		0xbb247df5, 
		0x0055c277, 
		0x4add0d75, 
		0xf87cf0d9, 
		0x191b078b, 
		0x76369246, 
		0xdb1c7c3f, 
		0xa31ccdf4, 
		0x08bde0b0, 
		0x1377324a, 
		0x3476f0da, 
		0x6dd9fd8c, 
		0x334024f7, 
		0x74c8dedd, 
		0x46bec5c0, 
		0x58df92da, 
		0x090fb578, 
		0x3f23fad0, 
		0x03d3da7d, 
		0x9904ba4f, 
		0xcfe40351, 
		0x16682932, 
		0xdeb4eeff, 
		0xe7ec89ed, 
		0x23ce9f1c, 
		0x195987a5, 
		0xa737ef24, 
		0x99c21614, 
		0x092df730, 
		0xb19d545d, 
		0x16da6e93, 
		0x67499eea, 
		0xe3eb5bbc, 
		0x8734616d, 
		0x5bc7e14e, 
		0x454ef275, 
		0x174502c1, 
		0x7156d694, 
		0x2e28ddca, 
		0xd6944119, 
		0x9114dbec, 
		0x484c4c21, 
		0x48592cc8, 
		0xc2932860, 
		0xc9e17645, 
		0x6816460e, 
		0xed484bc1, 
		0xdc5f6d8f, 
		0x6a17d7d3, 
		0xdafd9f41, 
		0x28598cc2, 
		0x8743a073, 
		0x2c7a7faa, 
		0x6384bd3c, 
		0xc2ce4a2d, 
		0x7329699d, 
		0x73dfd15b, 
		0xe0c5e13b, 
		0xb035dfc9, 
		0x407f268e, 
		0x9f629116, 
		0x6e8e5489, 
		0xdd98b69f, 
		0x34867055, 
		0xb9683e00, 
		0xe718b876, 
		0xbffeb38a, 
		0x84bf0c02, 
		0x1505f4ad, 
		0x48220b4d, 
		0x846fc4ee, 
		0xe72af63e, 
		0xdec28e04, 
		0x17d76d37, 
		0xe687af2e, 
		0xd7056e67, 
		0x6a441e95, 
		0xd27a8cd0, 
		0xf56ee3a9, 
		0x4fe75856, 
		0x2442293f, 
		0x019cc03b, 
		0xac935216, 
		0xa00f3551, 
		0xcbb3e82b, 
		0xd09b8926, 
		0xb35cacee, 
		0x2b37e9ef, 
		0xeed638e5, 
		0x406f8e73, 
		0xaa2b5d65, 
		0xecba79c7, 
		0xa0f14e27, 
		0xa731e3b8, 
		0x70215213, 
		0x983a99f9, 
		0xc8e16255, 
		0xe92816f9, 
		0xf89bfa20, 
		0x9e7e1759, 
		0x5b87a4d6, 
		0x867cb103, 
		0xa289da48, 
		0xa7da9cc0, 
		0x99ec9d33, 
		0x74924400, 
		0x0f9798d4, 
		0x8b3ffc66, 
		0x90dcdf7e, 
		0xc775ec6e, 
		0xcd7c65b0, 
		0x107460b2, 
		0x650c719c, 
		0x9dd04fa5, 
		0x71ae26cd, 
		0xd63db5f5, 
		0xf3a4d06d, 
		0x706452de, 
		0x028182c7, 
		0xacaeb44d, 
		0xfc6d0a6f, 
		0xb3f29cbd, 
		0xdb599525, 
		0xf6bde7fc, 
		0x072ed989, 
		0x6ca93e14, 
		0x1158cb70, 
		0xdf37ba0f, 
		0x7522bea4, 
		0x5d1a84a9, 
		0xd3cdb849, 
		0x3d3e8eb2, 
		0xa6c66f84, 
		0xc46fea23, 
		0x1fbec76f, 
		0x5cd3480e, 
		0xc17891fa, 
		0xc06bb93b, 
		0x95f427ec, 
		0x6a6cd843, 
		0x9a0e4908, 
		0x789c1747, 
		0xe20c0624, 
		0xd3c38bb9, 
		0xcc249ab4, 
		0x59966476, 
		0xc7085dea, 
		0x705fc1da, 
		0x70744ad6, 
		0xf90f8917, 
		0x9c45ff12, 
		0xad74b8d0, 
		0x040446c9, 
		0xdd09d890, 
		0xca1ac4bc, 
		0xf7a3fe06, 
		0x5a5d50d0, 
		0x0b68bb88, 
		0xdf909dd5, 
		0x6da9e87b, 
		0x2fa323d0, 
		0x4e34b63a, 
		0x4f92d99d, 
		0x6946daad, 
		0xf4d7f92c, 
		0xef2c97a7, 
		0xf9311e1a, 
		0x6ad30baa, 
		0x045c4942, 
		0x7c2b5103, 
		0x4e266466, 
		0x23c6c191, 
		0x6660c380, 
		0x4544dc72, 
		0x69e5fae6, 
		0x6907d0a6, 
		0x5147b765, 
		0xdaa0abb5, 
		0x85994996, 
		0xc51c0f00, 
		0xc4f5a2e3, 
		0xc14b122b, 
		0xb43d47dc, 
		0xf1eeb1c3, 
		0x94997f9d, 
		0xebbc63a3, 
		0x16a02d55  
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
		0x4a80, 
		0x52a0, 
		0x2745, 
		0x50c0, 
		0x9688, 
		0x8b2b, 
		0xd300, 
		0xcb00, 
		0xf1c6, 
		0xd160, 
		0x8000, 
		0x83a8, 
		0xcc80, 
		0x9c60, 
		0x4f80, 
		0xfe14, 
		0x0000, 
		0x50a0, 
		0x7c00, 
		0xcd40, 
		0xac31, 
		0x16aa, 
		0x4800, 
		0x4c40, 
		0xba75, 
		0x067d, 
		0xd0a0, 
		0x52c0, 
		0x5120, 
		0x5060, 
		0x20ab, 
		0x53e0, 
		0x5060, 
		0xd180, 
		0x14c2, 
		0x5220, 
		0x5220, 
		0x1f25, 
		0xd140, 
		0x355f, 
		0xbc00, 
		0x0cb1, 
		0x71fe, 
		0x5140, 
		0x5430, 
		0xb7a4, 
		0x7c00, 
		0xd1e0, 
		0x53e0, 
		0x8000, 
		0xd160, 
		0x5220, 
		0x5380, 
		0x5060, 
		0x5320, 
		0xd3a0, 
		0xcec0, 
		0xd220, 
		0xec99, 
		0x0000, 
		0x7db5, 
		0x4a00, 
		0x9b3b, 
		0xd340, 
		0x5200, 
		0x4e40, 
		0xd3c0, 
		0x2d63, 
		0xca00, 
		0x4500, 
		0x588b, 
		0x5340, 
		0xd1a0, 
		0xd360, 
		0xcfc0, 
		0xc7cb, 
		0x3c84, 
		0xfc00, 
		0xc700, 
		0xfcb7, 
		0xccc0, 
		0x5180, 
		0x5240, 
		0xc400, 
		0xd0e0, 
		0x84d0, 
		0xfe10, 
		0x4c40, 
		0xd0c0, 
		0xda5c, 
		0x5000, 
		0x5060, 
		0x4d40, 
		0x0000, 
		0xd240, 
		0x4f80, 
		0x4e40, 
		0xd000, 
		0xb01a, 
		0xcdc0, 
		0x5220, 
		0xd340, 
		0xcc40, 
		0xcc80, 
		0x4c40, 
		0x646d, 
		0x6573, 
		0x5240, 
		0xcec0, 
		0xa05f, 
		0xd3a0, 
		0x8000, 
		0x4600, 
		0x8000, 
		0xccc0, 
		0x5ade, 
		0x5060, 
		0xd320, 
		0xce80, 
		0x0000, 
		0xc600, 
		0xd1e0, 
		0x4400, 
		0x4d80, 
		0xc5fb, 
		0xd020, 
		0x51a0, 
		0x4500, 
		0x7c00, 
		0x8000, 
		0x351d, 
		0x5af6, 
		0x53a0, 
		0xa058, 
		0x51c0, 
		0x4980, 
		0x4200, 
		0xd200, 
		0xcf40, 
		0xcd80, 
		0xce00, 
		0xfdc3, 
		0xe071, 
		0x6d9f, 
		0xfc00, 
		0x0799, 
		0x51e0, 
		0xb379, 
		0xd020, 
		0xc700, 
		0x53a0, 
		0xcf00, 
		0xc800, 
		0x4f80, 
		0x5160, 
		0xc500, 
		0x134f, 
		0x7087, 
		0x4ec7, 
		0x433a, 
		0xd200, 
		0x4c40, 
		0xd080, 
		0x53a0, 
		0xd160, 
		0x5260, 
		0xcb00, 
		0x4dc0, 
		0x53a0, 
		0xfc00, 
		0xd060, 
		0xd0a0, 
		0xcc40, 
		0x52e0, 
		0xcc00, 
		0x5000, 
		0x4e40, 
		0x4700, 
		0xd2e0, 
		0x5180, 
		0x4000, 
		0x53a0, 
		0x2692, 
		0xd3e0, 
		0x2b34, 
		0xd2c0, 
		0xd140, 
		0x6b55, 
		0x7079, 
		0xd2c0, 
		0xcc40, 
		0xd2a0, 
		0x5260, 
		0xcc80, 
		0x4980, 
		0x4dc0, 
		0xd493, 
		0xc2c5, 
		0x8d49, 
		0xfc00, 
		0xccc0, 
		0xd180, 
		0x4e80, 
		0xd080, 
		0xce00, 
		0x7b24, 
		0x5bdc, 
		0x86d2, 
		0x7c00, 
		0xd040, 
		0x51c0, 
		0xcf00, 
		0x5240, 
		0x5300, 
		0xd1e0, 
		0xd0a0, 
		0x7c00, 
		0x50e0, 
		0x432c, 
		0x52a0, 
		0x4900, 
		0x5180, 
		0x4f40, 
		0x4a00, 
		0x9d4d, 
		0xaf0a, 
		0x50c0, 
		0x01bc, 
		0x4ac9, 
		0xeaf5, 
		0xd1e0, 
		0xd360, 
		0x8000, 
		0x5844, 
		0xa737, 
		0xcc40, 
		0xc700, 
		0xd0a0, 
		0x8000, 
		0x7c08, 
		0x5200, 
		0xfc54, 
		0x50c0, 
		0x29bb, 
		0xd3e0, 
		0xc700, 
		0x2a21, 
		0x4c00, 
		0x4e40, 
		0xc600, 
		0x4cc0, 
		0xc880, 
		0x53c0, 
		0x9b45, 
		0x6673, 
		0x5080, 
		0x4900, 
		0x52b4, 
		0xbc9f, 
		0xce80, 
		0xc5b3, 
		0xc66c, 
		0x53e0, 
		0xd080, 
		0xd830, 
		0x4b80, 
		0x9f07, 
		0xc700, 
		0xfc00, 
		0x0000, 
		0x7c9c, 
		0x5340, 
		0xcb00, 
		0x4e40, 
		0x5220, 
		0x0303, 
		0x5120, 
		0xc600, 
		0x4f80, 
		0x4336, 
		0x4880, 
		0x5160, 
		0x0000, 
		0x5060, 
		0xc1ac, 
		0x5360, 
		0x9eb7, 
		0x5320, 
		0xd180, 
		0xccc0, 
		0xf3e1, 
		0xccc0, 
		0xc200, 
		0xc0a6, 
		0x4172, 
		0x53c0, 
		0x5280, 
		0xcf40, 
		0x5340, 
		0xd300, 
		0x4d80, 
		0x7c1b, 
		0xd3c0, 
		0xca80, 
		0xd0c0, 
		0xd000, 
		0x4e40, 
		0xcc80, 
		0x1cb2, 
		0xd040, 
		0xca00, 
		0x056a, 
		0xd000, 
		0x1276, 
		0x0000, 
		0x9c87, 
		0x51c0, 
		0x23fd, 
		0x5360, 
		0xca00, 
		0x4c00, 
		0x47c3, 
		0x0ab9, 
		0xca00, 
		0x846c, 
		0x4e40, 
		0x0174, 
		0x3cd5, 
		0x9ce6, 
		0xa04b, 
		0x4f40, 
		0xfc00, 
		0x4980, 
		0xbc00, 
		0x5080, 
		0xd200, 
		0xcdc0, 
		0xcfc0, 
		0x50c0, 
		0xc980, 
		0xd320, 
		0xa8c9, 
		0x4c40, 
		0x57c6, 
		0x8000, 
		0xfc00, 
		0xf928, 
		0x4dc0, 
		0x4e00, 
		0x2832, 
		0x0000, 
		0xd040, 
		0x0b6e, 
		0xfc00, 
		0x4e40, 
		0x53fb, 
		0xc000, 
		0x4f40, 
		0x8ffd, 
		0x2427, 
		0x4d40, 
		0xc9c0, 
		0x5320, 
		0xbda7, 
		0x4400, 
		0xcf00, 
		0xe17f, 
		0xc800, 
		0x4c00, 
		0x4cc0, 
		0x5160, 
		0x845a, 
		0xc413, 
		0x89db, 
		0x51e0, 
		0x89c2, 
		0xd2c0, 
		0xce80, 
		0xc800, 
		0x2cac, 
		0xcbcf, 
		0xd000, 
		0xac7f, 
		0x1283, 
		0xd240, 
		0xcf80, 
		0xd300, 
		0xcea2, 
		0xcf80, 
		0xc600, 
		0x691a, 
		0x5320, 
		0xd3c0, 
		0xc080, 
		0xd180, 
		0xcc05, 
		0xd360, 
		0x5120, 
		0xc400, 
		0xd3c0, 
		0xf91d, 
		0xd160, 
		0xbc00, 
		0x8000, 
		0x191c, 
		0xd3c0, 
		0x4cc0, 
		0xfd58, 
		0xd0e0, 
		0xaaaa, 
		0x5380, 
		0x8000, 
		0x1019, 
		0xd260, 
		0x93a1, 
		0x4d00, 
		0xd360, 
		0xfc00, 
		0xd1e0, 
		0x7989, 
		0x0000, 
		0x4600, 
		0xc880, 
		0xce80, 
		0xa85a, 
		0x5180, 
		0x8000, 
		0x4200, 
		0xd020, 
		0xd280, 
		0x4500, 
		0xd240, 
		0x7aa7, 
		0x5240, 
		0xca00, 
		0xab68, 
		0xce80, 
		0xd360, 
		0xd0c0, 
		0xc700, 
		0xbc5a, 
		0xcd80, 
		0xd340, 
		0x5280, 
		0x7c00, 
		0xfbaf, 
		0x5140, 
		0x35f4, 
		0x5140, 
		0x4000, 
		0x667c, 
		0x4dc0, 
		0xcf40, 
		0x4400, 
		0x4200, 
		0x4b80, 
		0xfc00, 
		0x4b80, 
		0x7c00, 
		0x446b, 
		0x4ec0, 
		0xca00, 
		0x4fc0, 
		0x4c00, 
		0x5140, 
		0x5040, 
		0xbc00, 
		0x0000, 
		0x4b80, 
		0xca80, 
		0x5260, 
		0xd3e0, 
		0x53e0, 
		0x4900, 
		0x8000, 
		0x4c80, 
		0x19d5, 
		0xcd40, 
		0xd240, 
		0x53e0, 
		0x5260, 
		0xd020, 
		0x4880, 
		0x23e9, 
		0xf76a, 
		0x4880, 
		0x7194, 
		0xd220, 
		0x5240, 
		0xd3a0, 
		0xb861, 
		0xa797, 
		0x4f00, 
		0xd0a0, 
		0x0611, 
		0xc200, 
		0x5320, 
		0xca80, 
		0x5040, 
		0x53c0, 
		0x4ec0, 
		0xd320, 
		0xfc7b, 
		0x4d00, 
		0x8ec6, 
		0x4e80, 
		0xcd00, 
		0xae1f, 
		0xcd80, 
		0x51e0, 
		0x8bce, 
		0x0000, 
		0xce80, 
		0x0000, 
		0x5000, 
		0xcd00, 
		0x8000, 
		0xcd80, 
		0xc200, 
		0xd2e0, 
		0x5040, 
		0x4400, 
		0x51c0, 
		0x4a80, 
		0xfc00, 
		0xd260, 
		0x50e0, 
		0xd076, 
		0xcc00, 
		0x4e00, 
		0x4f40, 
		0x0000, 
		0x5300, 
		0xd160, 
		0x4900, 
		0x5320, 
		0xce80, 
		0xd180, 
		0x4e00, 
		0xd360, 
		0x51e0, 
		0x98ff, 
		0xc900, 
		0xca19, 
		0xd3c0, 
		0x3965, 
		0xd521, 
		0x4d40, 
		0x9836, 
		0x50a0, 
		0xcc00, 
		0xd320, 
		0x2d13, 
		0xfda6, 
		0xccc0, 
		0xc600, 
		0x4e80, 
		0xcd80, 
		0x5100, 
		0xd0c0, 
		0x7d9a, 
		0x204f, 
		0x6c18, 
		0xd0e0, 
		0x5340, 
		0x4ec0, 
		0x4d40, 
		0x5060, 
		0x0000, 
		0xd753, 
		0xd060, 
		0x4880, 
		0xe083, 
		0xfc00, 
		0xb431, 
		0xafe5, 
		0x7c00, 
		0xcf40, 
		0x5280, 
		0xc980, 
		0x5060, 
		0x9c42, 
		0x51e0, 
		0x53c0, 
		0xfc00, 
		0x4e00, 
		0xe57d, 
		0x5200, 
		0xce40, 
		0x53c0, 
		0x7cb4, 
		0x5020, 
		0x3a3b, 
		0xc90b, 
		0xd0e0, 
		0x7a3a, 
		0xca00, 
		0x6931, 
		0x53a0, 
		0x5360, 
		0xf867, 
		0x53e9, 
		0xd1a0, 
		0x4b00, 
		0x3c7b, 
		0xd260, 
		0x7c00, 
		0x0000, 
		0xd140, 
		0x4fc0, 
		0xfc00, 
		0x5060, 
		0x4fc0, 
		0xc400, 
		0xd060, 
		0xd320, 
		0xccc0, 
		0x3eb8, 
		0x3c00, 
		0xd000, 
		0xce80, 
		0xa6b1, 
		0x369f, 
		0xa7b6, 
		0x4900, 
		0xd160, 
		0xd0c0, 
		0x4f00, 
		0xd0c0, 
		0x5133, 
		0x51a0, 
		0xec31, 
		0xcf00, 
		0xc297, 
		0xd683, 
		0xd3a0, 
		0xd3c0, 
		0x1d7e, 
		0x2d92, 
		0x3181, 
		0xc880, 
		0xd380, 
		0x4d00, 
		0xeea4, 
		0xd0e0, 
		0x9dbc, 
		0x8000, 
		0x7c00, 
		0xce40, 
		0x5020, 
		0x8848, 
		0x5356, 
		0xd0c0, 
		0xd1c0, 
		0x4e80, 
		0x4000, 
		0x4ec0, 
		0x4e80, 
		0x50c0, 
		0xd344, 
		0x1c4f, 
		0xca80, 
		0x53a0, 
		0xd2c0, 
		0xfc00, 
		0x75c1, 
		0xe3b9, 
		0x759f, 
		0xc980, 
		0x5200, 
		0x5280, 
		0xd2e0, 
		0xd260, 
		0xd040, 
		0x8000, 
		0x5100, 
		0xcf80, 
		0x4800, 
		0xc400, 
		0x4cc0, 
		0xc800, 
		0x5320, 
		0x4900, 
		0xc290, 
		0x5360, 
		0xcb00, 
		0xc800, 
		0xc900, 
		0xc400, 
		0xd000, 
		0xcc80, 
		0xf3ba, 
		0x8000, 
		0xd1e0, 
		0x360f, 
		0xc800, 
		0x8000, 
		0xc900, 
		0xd380, 
		0x4f40, 
		0xc800, 
		0xd2e0, 
		0x66b3, 
		0x3587, 
		0x7c00, 
		0x52e0, 
		0x50e0, 
		0x4e80, 
		0xac48, 
		0xd1e0, 
		0x0000, 
		0x8000, 
		0x3a7c, 
		0xd0e0, 
		0xd180, 
		0xcb00, 
		0x558b, 
		0x5100, 
		0x4d40, 
		0xcb00, 
		0xfc00, 
		0xd1a0, 
		0xd2e0, 
		0xd080, 
		0x0000, 
		0x5040, 
		0xcb80, 
		0xf814, 
		0xd3e0, 
		0x8000, 
		0xcdc0, 
		0xd0a0, 
		0x4e80, 
		0x23ea, 
		0x0dfb, 
		0xca00, 
		0x6667, 
		0xd100, 
		0x784d, 
		0x4fc0, 
		0x50e0, 
		0xfdf8, 
		0xcd00, 
		0x6202, 
		0xcd00, 
		0xd3a0, 
		0x186a, 
		0x508d, 
		0xce80, 
		0xcec0, 
		0xd040, 
		0xd340, 
		0x4e80, 
		0xfaa1, 
		0xfc77, 
		0x5160, 
		0xd360, 
		0x0ec3, 
		0x9b70, 
		0xd020, 
		0x5220, 
		0x710f, 
		0xe420, 
		0xc200, 
		0xc500, 
		0x3fe4, 
		0x360e, 
		0x4245, 
		0x2218, 
		0xc200, 
		0xcec0, 
		0xd060, 
		0x0000, 
		0x5180, 
		0x4500, 
		0xc700, 
		0x8000, 
		0xd140, 
		0x4b80, 
		0xd260, 
		0x7cf5, 
		0x5340, 
		0x5380, 
		0x4200, 
		0x8000, 
		0xef75, 
		0xcfc0, 
		0x7c00, 
		0x7024, 
		0xd0c0, 
		0x5060, 
		0x8140, 
		0xd1a0, 
		0x5c4d, 
		0xbec9, 
		0xcf09, 
		0x4a17, 
		0x5180, 
		0x5220, 
		0xd000, 
		0xd2a0, 
		0xd2a0, 
		0x0d2b, 
		0x0000, 
		0x4c40, 
		0x82d3, 
		0x1d89, 
		0x4b00, 
		0xd3c0, 
		0xd0a0, 
		0xd360, 
		0xa54b, 
		0xd70e, 
		0x6749, 
		0x7d51, 
		0xfc00, 
		0x4000, 
		0x4800, 
		0x2ef1, 
		0xa015, 
		0x51c0, 
		0x4f80, 
		0xc500, 
		0xfebc, 
		0x67bb, 
		0xd360, 
		0x8043, 
		0xc91e, 
		0x52a0, 
		0x5a4b, 
		0x4000, 
		0xd380, 
		0x421e, 
		0x5000, 
		0x4c40, 
		0x50c0, 
		0x4c80, 
		0xff9a, 
		0xcfc0, 
		0xcc40, 
		0x3c00, 
		0x4980, 
		0x5200, 
		0x4980, 
		0x8000, 
		0x5340, 
		0x4400, 
		0xc000, 
		0xd1a0, 
		0x9f3b, 
		0xca00, 
		0xbdc2, 
		0x1ff4, 
		0x0818, 
		0xd120, 
		0xd3a0, 
		0x5240, 
		0xce00, 
		0x9b0c, 
		0x59a5, 
		0x8000, 
		0x154e, 
		0xc5a8, 
		0x7c00, 
		0x4f00, 
		0x7c00, 
		0x2b2a, 
		0x8ef5, 
		0x556b, 
		0x5140, 
		0x51a0, 
		0x54f6, 
		0x8000, 
		0xcf40, 
		0x4452, 
		0xcd80, 
		0xcec0, 
		0xc200, 
		0x4d00, 
		0xd0a0, 
		0x854c, 
		0xfc00, 
		0xd254, 
		0x5320, 
		0x3a6a, 
		0x4980, 
		0x3847, 
		0x4700, 
		0x5280, 
		0x7c00, 
		0xd3a0, 
		0x8000, 
		0x5040, 
		0xc400, 
		0xc980, 
		0x713a, 
		0xd060, 
		0x5140, 
		0xc600, 
		0xd180, 
		0x8086, 
		0xf27b, 
		0x730b, 
		0xc900, 
		0x9fbb, 
		0x5060, 
		0x5fae, 
		0x7d36, 
		0xcc00, 
		0xcc80, 
		0x3d32, 
		0x4980, 
		0xf7ec, 
		0xce80, 
		0xd300, 
		0xe731, 
		0x5120, 
		0xc400, 
		0xe781, 
		0x5180, 
		0xcd40, 
		0x7c00, 
		0x1298, 
		0x2178, 
		0xd080, 
		0x0000, 
		0xd887, 
		0x8fad, 
		0x4f80, 
		0x4c40, 
		0x4700, 
		0x4a6b, 
		0x55b8, 
		0x52e0, 
		0xbc00, 
		0x4cc0, 
		0xd020, 
		0xc53f, 
		0x5000, 
		0x216c, 
		0xd1a0, 
		0xc800, 
		0xcf80, 
		0x53a0, 
		0x7c00, 
		0xc500, 
		0x927b, 
		0x4b80, 
		0xd380, 
		0xc67f, 
		0x53e0, 
		0xc200, 
		0x4600, 
		0x50a0, 
		0x0c94, 
		0x52e0, 
		0x5120, 
		0xd160, 
		0x5280, 
		0x52d4, 
		0xce40, 
		0x4000, 
		0x4cc0, 
		0x6c52, 
		0xa236, 
		0xc500, 
		0xd200, 
		0x4800, 
		0xd120, 
		0x5000, 
		0xcc80, 
		0xcc00, 
		0x9ed7, 
		0x5b97, 
		0x44a4, 
		0x4980, 
		0xe7b5, 
		0x5260, 
		0xc980, 
		0xa473, 
		0xfc00, 
		0xd360, 
		0xed5e, 
		0xc700, 
		0x7c00, 
		0x58ed, 
		0x4e40, 
		0xccea, 
		0x8000, 
		0xc400, 
		0xd220, 
		0x4c00, 
		0xd000, 
		0x5360, 
		0x4500, 
		0xd0a0, 
		0xe828, 
		0x4f80, 
		0x6947, 
		0x460e, 
		0xca80, 
		0xd2a0, 
		0x7d27, 
		0x4400, 
		0xd2c0, 
		0x707e, 
		0xcf40, 
		0xd260, 
		0x4c80, 
		0x8000, 
		0x4400, 
		0xcf00, 
		0x2802, 
		0xca80, 
		0x5360, 
		0xcf40, 
		0xeb74, 
		0xd000, 
		0x7c3e, 
		0xbc00, 
		0xf3e3, 
		0xd260, 
		0xd1c0, 
		0xf6e8, 
		0xcdc0, 
		0x4000, 
		0x5080, 
		0xc000, 
		0x6d8b, 
		0xc412, 
		0x88f1, 
		0x0335, 
		0x5040, 
		0x323d, 
		0x53a0, 
		0xca80, 
		0x5100, 
		0xd932, 
		0xc600, 
		0x85cc, 
		0xc880, 
		0xc500, 
		0xc864, 
		0xd2a0, 
		0x4800, 
		0x4f80, 
		0x80b6, 
		0x5160, 
		0xcec0, 
		0x4f40, 
		0x4c00, 
		0xfc00, 
		0x7c00, 
		0x5380, 
		0xd360, 
		0x52c0, 
		0x4ec0, 
		0x5220, 
		0x0000, 
		0xfc00, 
		0xd380, 
		0xc070, 
		0xd340, 
		0x18c9, 
		0x49cd, 
		0x65e5, 
		0xcd00, 
		0x5080, 
		0x5340, 
		0x5280, 
		0xc700, 
		0x28bd, 
		0x3aae, 
		0xa2ca, 
		0xd000, 
		0x9afb, 
		0xd1e0, 
		0x0000, 
		0x969a, 
		0xc200, 
		0xfc00, 
		0xcc80, 
		0x4400, 
		0x713c, 
		0x5160, 
		0xd160, 
		0x536e, 
		0x53a0, 
		0x4a80, 
		0x7320, 
		0x2046, 
		0xd360, 
		0x53e0, 
		0x4880, 
		0xcf40, 
		0xc600, 
		0xd0c0, 
		0xd1c0, 
		0xd0a0, 
		0xb51f, 
		0x8000, 
		0xd340, 
		0x5140, 
		0x7a0f, 
		0xce00, 
		0x0000, 
		0x4200, 
		0xbddd, 
		0x5060, 
		0x8d37, 
		0xd1c0, 
		0x5120, 
		0xa98d, 
		0x2370, 
		0xd300, 
		0x4a80, 
		0xaac2, 
		0x6c6f, 
		0x3f34, 
		0xd612, 
		0xd3a0, 
		0x1f01, 
		0x4880, 
		0x5200, 
		0xcd00, 
		0x034b, 
		0x8000, 
		0x52e0, 
		0x4e80, 
		0x7fe2, 
		0xd040, 
		0x8000, 
		0xc200, 
		0x4600, 
		0xaa3e, 
		0x5000, 
		0x3b41, 
		0x4e00, 
		0xf48a, 
		0x52a0, 
		0x4b00, 
		0xd160, 
		0x5380, 
		0x8000, 
		0x53a0, 
		0x5000, 
		0xcc40, 
		0x2033, 
		0x5300, 
		0xe452, 
		0x4b00, 
		0x868f, 
		0x4900, 
		0xa571, 
		0xc21d, 
		0xd220, 
		0x5200, 
		0xb1ca, 
		0xcd80, 
		0x4000, 
		0xdb16, 
		0xcb80, 
		0xd120, 
		0x4e80, 
		0xd120, 
		0x51a0, 
		0x4dc0, 
		0xc700, 
		0x2740, 
		0x4d00, 
		0xd220, 
		0xd240, 
		0x5c49, 
		0x5200, 
		0x5040, 
		0xd1c0, 
		0x5220, 
		0xc000, 
		0xdf96, 
		0x1dcc, 
		0x50a0, 
		0xcec0, 
		0x4bb1, 
		0x8000, 
		0xd2c0, 
		0xcd00, 
		0x0000, 
		0xfc00, 
		0xcf00, 
		0xd080, 
		0x4700, 
		0x5220, 
		0x2051, 
		0x4600, 
		0xd300, 
		0x4b5a, 
		0xd080, 
		0x4e40, 
		0xd180, 
		0x3c00, 
		0xd2e0, 
		0xcd80, 
		0x4b80, 
		0x1789, 
		0x4600, 
		0xd0c0, 
		0xa01d, 
		0x4cc0, 
		0x51a0, 
		0xba46, 
		0xfc00, 
		0x0000, 
		0xf4e9, 
		0xa600, 
		0x7f01, 
		0x5340, 
		0x5340, 
		0x5240, 
		0x52a0, 
		0x4ec0, 
		0xa195, 
		0xcd80, 
		0x4391, 
		0x4268, 
		0x8000, 
		0x3c00, 
		0xd280, 
		0x4e00, 
		0x0127, 
		0xd3c0, 
		0xd280, 
		0xef2f, 
		0xbf9c, 
		0x4c40, 
		0x7311, 
		0xfcc8, 
		0xb35f, 
		0xd2a0, 
		0x5320, 
		0xd0c0, 
		0x4a00, 
		0x69f0, 
		0xfbe6, 
		0x170d, 
		0x2db2, 
		0xd100, 
		0xcdc0, 
		0xce40, 
		0xd3c0, 
		0x5020, 
		0xd1e0, 
		0xcc80, 
		0x51a0, 
		0x50a0, 
		0x52a0, 
		0x5120, 
		0x821a, 
		0xd0e0, 
		0x0acc, 
		0xcf00, 
		0x0807, 
		0xcf40, 
		0x4600, 
		0xcfc0, 
		0xc200, 
		0xa1bd, 
		0xa0ad, 
		0x590c, 
		0x4c00, 
		0xc839, 
		0xd1a0, 
		0x3cc6, 
		0x2646, 
		0xe6ff, 
		0x0000, 
		0xd060, 
		0x2511, 
		0x51a0, 
		0x4700, 
		0xfc00, 
		0xc800, 
		0xfc00, 
		0x7d15, 
		0x7570, 
		0x8273, 
		0x4d00, 
		0x4880, 
		0x4fc0, 
		0xc600, 
		0x4dc0, 
		0xc400, 
		0x8000, 
		0x5360, 
		0x5578, 
		0xe97b, 
		0xcf00, 
		0x0000, 
		0x4700, 
		0x4600, 
		0x4dc0, 
		0x5100, 
		0x4400, 
		0xc800, 
		0xb18a, 
		0x5100, 
		0xd71c, 
		0xce80, 
		0xcf00, 
		0x794d, 
		0xcb00, 
		0xd2c0, 
		0x6b03, 
		0xd300, 
		0x5698, 
		0x5180, 
		0x4000, 
		0x7c00, 
		0x50e0, 
		0xfca5, 
		0xd0c0, 
		0x4600, 
		0xb051, 
		0x50e0, 
		0x15e3, 
		0x4d00, 
		0xcd40, 
		0xf1b8, 
		0x50c0, 
		0x8167, 
		0x5280, 
		0xd160, 
		0x51a0, 
		0x4b80, 
		0xc500, 
		0x8000, 
		0xd160, 
		0x4e00, 
		0xcb00, 
		0x1315, 
		0x3c00, 
		0xd080, 
		0x52c0, 
		0x5080, 
		0xc900, 
		0xcc80, 
		0x4000, 
		0x0a13, 
		0x5380, 
		0x50e0, 
		0x122e, 
		0x4e80, 
		0x0000, 
		0xd160, 
		0x8000, 
		0x0000, 
		0xcd80, 
		0x51c0, 
		0xad63, 
		0x5080, 
		0xd0a0, 
		0x9ba2, 
		0xce00, 
		0x3c38, 
		0x82cd, 
		0x4e00, 
		0xb4b2, 
		0xc800, 
		0xc500, 
		0xe774, 
		0xd260, 
		0xa8be, 
		0x4a80, 
		0xd140, 
		0xc900, 
		0xd260, 
		0x4f40, 
		0xb98a, 
		0xd2c0, 
		0x4c40, 
		0x4fa2, 
		0x5020, 
		0xeb2a, 
		0xcc00, 
		0x5040, 
		0x1731, 
		0x5160, 
		0xd300, 
		0x50e0, 
		0x4d80, 
		0x52c0, 
		0x5020, 
		0x4980, 
		0xd280, 
		0x4d40, 
		0x4f40, 
		0xd240, 
		0xf403, 
		0xcf00, 
		0x53e0, 
		0xd0c0, 
		0xcfc0, 
		0x64b1, 
		0xce00, 
		0x4a80, 
		0x4900, 
		0x5220, 
		0x5260, 
		0xd260, 
		0x8000, 
		0xc500, 
		0x8ee2, 
		0x9a96, 
		0xcd00, 
		0x33ec, 
		0xd1a0, 
		0x0000, 
		0x81ae, 
		0x775e, 
		0x52e0, 
		0xd020, 
		0x4c40, 
		0x2a23, 
		0xd020, 
		0xc900, 
		0xd240, 
		0x7989, 
		0xc700, 
		0xd320, 
		0xf68f, 
		0x787e, 
		0xabad, 
		0x0769, 
		0x7c00, 
		0x3a2e, 
		0x7d05, 
		0xcced, 
		0x8749, 
		0x8000, 
		0x5040, 
		0x4da8, 
		0xcc80, 
		0x5160, 
		0x5140, 
		0xe5f4, 
		0xd040, 
		0xd320, 
		0x4800, 
		0xf9a1, 
		0xd100, 
		0xd020, 
		0x4cd5, 
		0xce80, 
		0xd0c0, 
		0x4b80, 
		0xd120, 
		0xd2c0, 
		0xc500, 
		0xd180, 
		0x5f0b, 
		0x4cc0, 
		0x4600, 
		0x50a0, 
		0x5240, 
		0x5e73, 
		0x5bcc, 
		0x5360, 
		0x5220, 
		0x5040, 
		0xcb00, 
		0x51c0, 
		0x4d80, 
		0x69d4, 
		0xcd00, 
		0xd1c0, 
		0xcf00, 
		0xcf80, 
		0x4cc0, 
		0x4f40, 
		0x0000, 
		0xd240, 
		0x4400, 
		0x4400, 
		0xd200, 
		0x52e0, 
		0x8000, 
		0xd000, 
		0x5200, 
		0x7c00, 
		0x0b2a, 
		0xa9ef, 
		0xca80, 
		0x4c00, 
		0x0000, 
		0xfc00, 
		0x0000, 
		0xcb00, 
		0xcd40, 
		0x8000, 
		0x8000, 
		0x4e40, 
		0x5340, 
		0xc200, 
		0x0000, 
		0xd0ec, 
		0x8000, 
		0x5280, 
		0x9fe4, 
		0x5180, 
		0xcb00, 
		0x0000, 
		0xd3c0, 
		0x8000, 
		0x6d05, 
		0x4f00, 
		0x7c00, 
		0x35cf, 
		0x543d, 
		0x2b0a, 
		0x5260, 
		0xd5d4, 
		0xd340, 
		0xdd8a, 
		0x3bc3, 
		0xfc00, 
		0xba3d, 
		0x7cba, 
		0x4fc0, 
		0xe6b8, 
		0xca20, 
		0xd0c0, 
		0xcec0, 
		0x1c38, 
		0xc980, 
		0x51e0, 
		0xcc40, 
		0x4d00  
};
volatile uint32_t rand_ieee754_32[1536] __attribute__ ((aligned (2048))) = {
		0x0c317027, 
		0x0e3fffff, 
		0x7f800000, 
		0xc1980000, 
		0x80800000, 
		0xc2580000, 
		0x7f7ffffe, 
		0x00080000, 
		0xff7fffff, 
		0x42140000, 
		0x41e80000, 
		0x0c7ffff0, 
		0xbf800000, 
		0x00800000, 
		0x80080000, 
		0x40400000, 
		0x00000008, 
		0x8f7ffffe, 
		0x7ffcd7ca, 
		0x00733d31, 
		0x80000000, 
		0x00400000, 
		0x2a0da657, 
		0xffc00001, 
		0x0c7fc000, 
		0xffc00001, 
		0xcccccccc, 
		0xff800000, 
		0xff9d2bde, 
		0xb8613e46, 
		0x0c7ffffe, 
		0x00000000, 
		0x7f7ffffe, 
		0x80000000, 
		0x8f7ffffd, 
		0x0f7ffffe, 
		0x00000080, 
		0xbf1d27c6, 
		0x43061026, 
		0x00000000, 
		0x00008000, 
		0x0e0fffff, 
		0x00000000, 
		0x80000000, 
		0xb755f580, 
		0xc0e00000, 
		0x388b447f, 
		0x427c0000, 
		0x7f800000, 
		0xc1c00000, 
		0x41100000, 
		0x42200000, 
		0x00000002, 
		0x80800001, 
		0xc1000000, 
		0x0057fd3d, 
		0x0555c54a, 
		0x7f7ffffe, 
		0xc1f80000, 
		0x00080000, 
		0x0e1fffff, 
		0xff800000, 
		0x00000200, 
		0x8e9698a7, 
		0xff800000, 
		0xc1b80000, 
		0xe16a83ce, 
		0x00800000, 
		0xfc3704f4, 
		0x247061bc, 
		0x170bd310, 
		0x0c7fffff, 
		0x2d8a8170, 
		0x00001000, 
		0xff7ffffe, 
		0x5f6eff14, 
		0x41800000, 
		0x434bcaba, 
		0xff800001, 
		0x45c3fce7, 
		0x23c46755, 
		0x00000000, 
		0x1f0bc919, 
		0x00002000, 
		0x380370d7, 
		0xa13abc24, 
		0x00000001, 
		0x00400000, 
		0x0e00000f, 
		0x0f7ffffe, 
		0xc2600000, 
		0xc2180000, 
		0x0c7ffffc, 
		0x33333333, 
		0x10a303e7, 
		0x80000000, 
		0x0c7c0000, 
		0x8f21d440, 
		0xbf028f5c, 
		0x0e003fff, 
		0xff800000, 
		0xd54a3f74, 
		0x0e7fffff, 
		0x17780b3e, 
		0x42540000, 
		0x9c6e5658, 
		0xc2280000, 
		0x7f7ffffe, 
		0x0c7fffe0, 
		0x728a1f0f, 
		0x00000000, 
		0xff800000, 
		0xadf48f31, 
		0xcd295168, 
		0x007fffff, 
		0x80800001, 
		0xc1f80000, 
		0x42680000, 
		0x80000000, 
		0x7f7fffff, 
		0x0e00ffff, 
		0xff7ffffe, 
		0x59653417, 
		0x80000000, 
		0xba325ab4, 
		0x80000000, 
		0x80000010, 
		0xe4ff741e, 
		0x8d790537, 
		0x80000010, 
		0x00002000, 
		0x00000000, 
		0x41200000, 
		0xffbd2ea5, 
		0x8056e300, 
		0x42440000, 
		0x7f800000, 
		0x40800000, 
		0x9bd97b67, 
		0x42200000, 
		0x3f800000, 
		0x80000000, 
		0x7f7ffffe, 
		0xc27c0000, 
		0x0e00ffff, 
		0x94df29f7, 
		0x423c0000, 
		0xc22c0000, 
		0xc22c0000, 
		0x00000001, 
		0x7f800000, 
		0x0c7e0000, 
		0x42440000, 
		0x0e0fffff, 
		0x80000000, 
		0xffc00001, 
		0x80000800, 
		0x88b64448, 
		0x42380000, 
		0x227f1a2c, 
		0x33333333, 
		0x0c7fffff, 
		0xac2781cc, 
		0x0c400000, 
		0x7fc00001, 
		0x42200000, 
		0x55555555, 
		0xff800000, 
		0x00080000, 
		0xd13fe2aa, 
		0x0c7ffffe, 
		0x00400000, 
		0x00800000, 
		0x0e7fffff, 
		0xffa7de2c, 
		0xef7a2dd9, 
		0x7f7ffffe, 
		0x4deadedc, 
		0x00800003, 
		0x00800003, 
		0x00020000, 
		0xa08b10c3, 
		0x42480000, 
		0xb2ab063d, 
		0x7f8fa84d, 
		0xc25c0000, 
		0x20988488, 
		0x00000040, 
		0xc2500000, 
		0x0c7fffe0, 
		0x09a18957, 
		0x69b36e72, 
		0x0c7ffe00, 
		0x7f800000, 
		0x40890e64, 
		0x00800003, 
		0xc21c0000, 
		0x80000000, 
		0x421c0000, 
		0x80800003, 
		0x253060e8, 
		0xc21c0000, 
		0xff800000, 
		0x7fffffff, 
		0x0e0fffff, 
		0xb3b17801, 
		0x42440000, 
		0x0e07ffff, 
		0x000bd6b0, 
		0x80800002, 
		0xb0867fca, 
		0x42380000, 
		0x40800000, 
		0xff800000, 
		0x7f800000, 
		0x7a9ce19d, 
		0x1f2b328e, 
		0x645080c0, 
		0xc1100000, 
		0x00000001, 
		0x00011111, 
		0x80000000, 
		0xeb7deae6, 
		0x41b00000, 
		0x00000000, 
		0x8053c5c6, 
		0xc0800000, 
		0x7fd363d3, 
		0x80000000, 
		0x00000000, 
		0x80ffffff, 
		0xc2640000, 
		0x80800001, 
		0x0f7ffffe, 
		0xc1b80000, 
		0x3708ef39, 
		0x0e0001ff, 
		0x60caaf7f, 
		0xabad337e, 
		0x4f6b362c, 
		0x0c700000, 
		0x80800000, 
		0x80200000, 
		0xc1800000, 
		0x30585513, 
		0x902b409b, 
		0x2f7a33b6, 
		0x42340000, 
		0x0c7ffffe, 
		0x0ba8587e, 
		0x80080000, 
		0xffaef46f, 
		0xff7ffffe, 
		0x41e00000, 
		0x80010000, 
		0x00010000, 
		0x00000000, 
		0x42580000, 
		0x8e6c5236, 
		0xff7fffff, 
		0x40400000, 
		0xd0a1d541, 
		0x00000004, 
		0x41f80000, 
		0x00000000, 
		0xc2300000, 
		0x80000000, 
		0x900979f7, 
		0x1f5aab79, 
		0x4b8c4b40, 
		0xea3d731f, 
		0x002399be, 
		0x0d5b0d1b, 
		0x00509b5a, 
		0x0c7ffff8, 
		0x0e00001f, 
		0x00000001, 
		0x06f43e78, 
		0xff000000, 
		0x00000020, 
		0x79ab3194, 
		0x6cd0c8c8, 
		0xc9bad133, 
		0xff912490, 
		0xc1700000, 
		0xc2080000, 
		0x6678c64f, 
		0x0c7fff00, 
		0x0c7ff800, 
		0xff800000, 
		0x849439de, 
		0x4b8c4b40, 
		0x0e07ffff, 
		0x4b22ef8a, 
		0x80000000, 
		0xaefaac6d, 
		0xa0e7fe05, 
		0x00800003, 
		0x0e000001, 
		0xc1500000, 
		0x80000000, 
		0x40a00000, 
		0x0e3fffff, 
		0x596ff9dd, 
		0x80800000, 
		0xc1c00000, 
		0xc1880000, 
		0xd13b0657, 
		0x00000100, 
		0x42780000, 
		0xfefb2e76, 
		0xc2500000, 
		0x80000000, 
		0xc2180000, 
		0xff7ffffe, 
		0x426747a5, 
		0x00000002, 
		0x76c5539a, 
		0x86268fd2, 
		0x00100000, 
		0x4b000000, 
		0xefb2fcd0, 
		0x33333333, 
		0x0bc63c47, 
		0x7fc00000, 
		0x40a00000, 
		0xc0e00000, 
		0x80000001, 
		0xff800000, 
		0x080ac2f2, 
		0xb364ca8a, 
		0xb23ac7c9, 
		0x80800000, 
		0x422c0000, 
		0x5e221b8b, 
		0x0c7ffc00, 
		0x0c7f8000, 
		0x80020000, 
		0x0f2a7e14, 
		0x8ba16e50, 
		0xc0c00000, 
		0x7fbfffff, 
		0x2416740b, 
		0x80000800, 
		0x9cebce5d, 
		0x31eca591, 
		0x1b4f6eac, 
		0x0c7fffc0, 
		0xc20c0000, 
		0x7e8a71d5, 
		0x166f5b8e, 
		0x41300000, 
		0xd4107c2a, 
		0x7f800000, 
		0x00000080, 
		0x80000001, 
		0xc55ae383, 
		0x41700000, 
		0x80000020, 
		0x0e07ffff, 
		0xc45c6363, 
		0x80ffffff, 
		0x00000200, 
		0x676fe03c, 
		0xc2400000, 
		0x920dffc7, 
		0x80800001, 
		0x427c0000, 
		0xc2200000, 
		0xc2300000, 
		0x5b8a4082, 
		0x41300000, 
		0x80000001, 
		0xc2680000, 
		0x85b19856, 
		0x4b8c4b40, 
		0x80200000, 
		0xd2cba03c, 
		0x531d9bb9, 
		0xd7748db6, 
		0x00008000, 
		0x80800000, 
		0xff800000, 
		0x00080000, 
		0x30c5c084, 
		0x36048cdb, 
		0xc1500000, 
		0x80000000, 
		0x686163e0, 
		0x97b8a18d, 
		0xff800000, 
		0xc2680000, 
		0x00000000, 
		0xc707d7a0, 
		0xd2216344, 
		0x36150c0d, 
		0x22d5f8e0, 
		0x625da80e, 
		0x0c7c0000, 
		0x86fbf216, 
		0xc1100000, 
		0xb3bfdc50, 
		0xc23c0000, 
		0x0e03ffff, 
		0x80000001, 
		0x00000000, 
		0x00000000, 
		0x00000000, 
		0x80ffffff, 
		0xff800000, 
		0x80000080, 
		0x7fc00001, 
		0x41100000, 
		0x7f800001, 
		0xff800000, 
		0xa9bd9d0b, 
		0xbdd3070d, 
		0x00000001, 
		0x28b3fdff, 
		0x40e00000, 
		0x80000200, 
		0x0e00ffff, 
		0x00011111, 
		0x7f000000, 
		0x85fc8d1d, 
		0x00001000, 
		0xf09b0def, 
		0x80000000, 
		0x6732590f, 
		0xc2600000, 
		0xc2080000, 
		0xc1c80000, 
		0x0c780000, 
		0x80000400, 
		0x80000000, 
		0xc2100000, 
		0x00000020, 
		0xa1b1d6e6, 
		0x80000400, 
		0xb4cc12b4, 
		0x00000100, 
		0x33333333, 
		0x0c7fffc0, 
		0x80001000, 
		0x80000100, 
		0x0c7fffc0, 
		0xc2140000, 
		0x41a80000, 
		0x42000000, 
		0x80000002, 
		0x42100000, 
		0xc0400000, 
		0x0d00fff0, 
		0x0c7f8000, 
		0xff7ffffe, 
		0xc26c0000, 
		0x0f7fffff, 
		0x5504ba69, 
		0x80000000, 
		0x80000000, 
		0x80000020, 
		0x00000000, 
		0x7fc00000, 
		0xa8929b1b, 
		0x0ce717c1, 
		0x40a00000, 
		0x807fffff, 
		0x97475650, 
		0xc2340000, 
		0x0e003fff, 
		0xb37ccbc4, 
		0x80800002, 
		0x42780000, 
		0xc2500000, 
		0x0c7fe000, 
		0x00000000, 
		0x40b5f7eb, 
		0x80000002, 
		0x3f800001, 
		0x0f7ffffe, 
		0x0e0007ff, 
		0xc1880000, 
		0xaa3b67e6, 
		0x894adf76, 
		0x1b051d63, 
		0xb135f054, 
		0xc1300000, 
		0x8046c452, 
		0xc2440000, 
		0xff000000, 
		0x0c7ffffe, 
		0x71bb484c, 
		0x80000200, 
		0xa204267d, 
		0x62051941, 
		0x423c0000, 
		0x80200000, 
		0x42700000, 
		0xff800000, 
		0x7f800001, 
		0x6d76de71, 
		0x00000000, 
		0xc2500000, 
		0x00000010, 
		0x87f1e1ef, 
		0x00000080, 
		0x49030919, 
		0xd8965f7d, 
		0x41980000, 
		0x00008000, 
		0xf2449bc9, 
		0xc2700000, 
		0x0e01ffff, 
		0x00000100, 
		0x00004000, 
		0x0d7a7e8a, 
		0x00800002, 
		0x0e00000f, 
		0x685fc9c6, 
		0x13e10599, 
		0x6f30a211, 
		0xa4f2b8d8, 
		0x80000001, 
		0x80001000, 
		0x0f7ffffe, 
		0x00000000, 
		0x80000002, 
		0x50d1164d, 
		0xf3340c39, 
		0xc27c0000, 
		0x41880000, 
		0x41300000, 
		0x06308b41, 
		0x426c0000, 
		0xc2100000, 
		0x00000000, 
		0xc2440000, 
		0x00000002, 
		0xf93358e0, 
		0x42540000, 
		0x007ffffe, 
		0xffc00000, 
		0x80000000, 
		0xdbb7e47a, 
		0x80ffffff, 
		0x3a6927ea, 
		0x9366674e, 
		0x2bf0f6e8, 
		0x8c6837f7, 
		0x0f7ffffe, 
		0x40800000, 
		0x7f800000, 
		0x80000010, 
		0xad39d3dc, 
		0x80000000, 
		0x0c600000, 
		0x41400000, 
		0xff800000, 
		0x41700000, 
		0xff800000, 
		0x80000000, 
		0x908cf37c, 
		0xc2200000, 
		0x41a00000, 
		0xad3c4e8a, 
		0x7f7fffff, 
		0x41b00000, 
		0x42280000, 
		0xffffffff, 
		0xaaaaaaaa, 
		0x8010db14, 
		0xc2380000, 
		0xc0a00000, 
		0x42340000, 
		0x0e007fff, 
		0x30dc5c8c, 
		0x41980000, 
		0x41b00000, 
		0x00000000, 
		0xc1800000, 
		0x00000080, 
		0xe128fcb6, 
		0xd03e8a9a, 
		0x07767305, 
		0x80000002, 
		0x4b000000, 
		0xc26c0000, 
		0xff8a4880, 
		0xcbb50fb3, 
		0xb2be8146, 
		0xff800000, 
		0xc2140000, 
		0x422c0000, 
		0x0e0003ff, 
		0x8e5dcaf6, 
		0x00000400, 
		0xc116ac24, 
		0x0c7e0000, 
		0xc1400000, 
		0x80000008, 
		0x80000100, 
		0x00000000, 
		0x00200000, 
		0x5602b964, 
		0x0e001fff, 
		0xc2000000, 
		0x42740000, 
		0x00000010, 
		0x42580000, 
		0x5bbd645f, 
		0xc2080000, 
		0x80000000, 
		0x7ea50ca1, 
		0x0e0000ff, 
		0xad7702d5, 
		0xffc00000, 
		0xbf800000, 
		0x42600000, 
		0x00000400, 
		0xae26020a, 
		0x80000008, 
		0x9f2911f4, 
		0xdaf5f8b1, 
		0xb60067bd, 
		0x00800003, 
		0xcb9a0e89, 
		0xc2140000, 
		0xb3457b05, 
		0xffc00000, 
		0xb2c883fc, 
		0x80040000, 
		0x538736d8, 
		0x7f7ffffe, 
		0x7f8e669a, 
		0x198e2e22, 
		0x80800002, 
		0xfa532490, 
		0x00000004, 
		0x80100000, 
		0x42080000, 
		0x7f800000, 
		0x0c7f8000, 
		0xff800000, 
		0x16e4f4a6, 
		0xa6d84654, 
		0x0e0001ff, 
		0x0c7ffc00, 
		0x74902db7, 
		0x0f7ffffd, 
		0x00000000, 
		0x7fffffff, 
		0xc20c0000, 
		0x00000040, 
		0xb778db2a, 
		0xff7ffffe, 
		0x00000000, 
		0x80000001, 
		0x802b3383, 
		0x0c7f0000, 
		0x41b00000, 
		0x80432fb8, 
		0x80000000, 
		0x7ff60616, 
		0xff800000, 
		0xc2640000, 
		0xff800000, 
		0x42540000, 
		0x0c7ffc00, 
		0x80040000, 
		0x0e00000f, 
		0x80000000, 
		0x42100000, 
		0xc1a00000, 
		0x4b000000, 
		0xc0c00000, 
		0x80000000, 
		0x8f7ffffe, 
		0x00800003, 
		0x00800001, 
		0xaaaaaaaa, 
		0x55c4454a, 
		0x41800000, 
		0x0e0fffff, 
		0x7c56baf7, 
		0xf37f49ef, 
		0x00000004, 
		0xc0800000, 
		0xe4b2ac71, 
		0x0e01ffff, 
		0x40800000, 
		0x42380000, 
		0x0f7ffffd, 
		0x00000000, 
		0x42240000, 
		0xc1f00000, 
		0x00000000, 
		0xc2480000, 
		0x0e0001ff, 
		0x3ab7a41b, 
		0x00010000, 
		0x0c7fff00, 
		0x29a651a0, 
		0x80000001, 
		0x0ed66cb5, 
		0xbf028f5c, 
		0x395eedc6, 
		0x5544d78a, 
		0xbc40fcd3, 
		0x0c7fffe0, 
		0x42240000, 
		0x7f7ffffe, 
		0x7fb4bba8, 
		0xc1900000, 
		0xbf800000, 
		0x7faf897a, 
		0x80000100, 
		0x41800000, 
		0xff800000, 
		0x7f800000, 
		0x41a00000, 
		0x7f800000, 
		0x80010000, 
		0x0c780000, 
		0xc2280000, 
		0x7f7ffffe, 
		0xc1900000, 
		0xe15efb42, 
		0xc2500000, 
		0x00000010, 
		0x80000000, 
		0xc2700000, 
		0x80004000, 
		0x27688ad3, 
		0xff9ee2d6, 
		0x80000002, 
		0xd907cdc9, 
		0xc2700000, 
		0xccf0cb98, 
		0xb1de7e34, 
		0x7cf8cbf5, 
		0x80800001, 
		0x4b8c4b40, 
		0x7ffca4b2, 
		0xaa5f2f37, 
		0xc1880000, 
		0xc1900000, 
		0x00000001, 
		0x0e7fffff, 
		0x6946650f, 
		0x80000002, 
		0x80000800, 
		0x7f9ca22c, 
		0x05cf7fb9, 
		0xc1200000, 
		0xcbed9a49, 
		0x40a00000, 
		0x32622faf, 
		0xd8339aa5, 
		0xfabc658f, 
		0x41e80000, 
		0x00000010, 
		0x42680000, 
		0x42700000, 
		0x42440000, 
		0xc22c2a14, 
		0x42780000, 
		0x0e00007f, 
		0x7f800000, 
		0x0482f6cb, 
		0x40e00000, 
		0x00000000, 
		0x00000000, 
		0x0c400000, 
		0x80000000, 
		0xc1a00000, 
		0xa4e7ce12, 
		0x0e003fff, 
		0xc2040000, 
		0xc1300000, 
		0xb2cb1ab9, 
		0x41800000, 
		0x41d00000, 
		0x426c0000, 
		0x00000000, 
		0x7f800000, 
		0x80002000, 
		0xc2380000, 
		0xba18d204, 
		0xa8ecd62a, 
		0x40a00000, 
		0x00008000, 
		0x42400000, 
		0x42480000, 
		0x00800000, 
		0xc1d80000, 
		0x00100000, 
		0xc25c0000, 
		0x00000000, 
		0x3c210bf9, 
		0x00000400, 
		0x80000000, 
		0xc1980000, 
		0x7fc00001, 
		0x41d80000, 
		0xffffffff, 
		0x00800001, 
		0x00000001, 
		0x80000008, 
		0x00593717, 
		0x00000001, 
		0xc1e80000, 
		0xc2500000, 
		0x45c457a7, 
		0x54d40f83, 
		0x486902f8, 
		0x00000001, 
		0x7f800000, 
		0xb17ded91, 
		0x0c7f0000, 
		0x40800000, 
		0xbc1dfe56, 
		0x00004000, 
		0xff800000, 
		0x7f800000, 
		0x80000020, 
		0x41300000, 
		0x80800000, 
		0xbb19c2d3, 
		0xa4b47213, 
		0x242be3a9, 
		0x819c6106, 
		0x734d24e9, 
		0x41f00000, 
		0x00000000, 
		0x3e40d913, 
		0xc1600000, 
		0x00002000, 
		0x90cbc055, 
		0x425c0000, 
		0x0c7e0000, 
		0x7f800000, 
		0x4012cde6, 
		0xc61e0d49, 
		0xd162e0f4, 
		0xbe5c3d90, 
		0xc23c0000, 
		0x298a9d9e, 
		0x00000000, 
		0xc2780000, 
		0x41800000, 
		0x00010000, 
		0xc0000000, 
		0x41f00000, 
		0x80000000, 
		0x00000000, 
		0xc2240000, 
		0x00010000, 
		0xc1100000, 
		0x09e9fe75, 
		0xf4c631d1, 
		0x80ffffff, 
		0x00000001, 
		0x0c7ff800, 
		0x80000800, 
		0x24fe824e, 
		0x7f800000, 
		0xc1900000, 
		0x7a670186, 
		0xffbace94, 
		0x2bdd3975, 
		0xff893cac, 
		0x423774cd, 
		0xec5d485c, 
		0x807fffff, 
		0xff7fffff, 
		0x0e0003ff, 
		0x80800001, 
		0xc2100000, 
		0xcccccccc, 
		0xc2080000, 
		0xcf9741a3, 
		0x9cddaca2, 
		0x00002000, 
		0x00020000, 
		0x80000000, 
		0x0c7ffffc, 
		0xf7ad1912, 
		0x0e0001ff, 
		0x7feeda92, 
		0xcb8c4b40, 
		0x80000008, 
		0xc2780000, 
		0x0e07ffff, 
		0x7f7ffffe, 
		0x00000000, 
		0x80000000, 
		0xa2579a01, 
		0x00001000, 
		0x420c0000, 
		0x2478eb00, 
		0x0d000ff0, 
		0x00004000, 
		0x7f800000, 
		0x77253df7, 
		0x00800002, 
		0x80000000, 
		0x7f800000, 
		0xff800000, 
		0x00000400, 
		0x7f7fffff, 
		0x00000000, 
		0x40a00000, 
		0x3f800000, 
		0x42740000, 
		0x007fffff, 
		0x8e315358, 
		0xc0800000, 
		0x00000000, 
		0x8e536992, 
		0x463bca71, 
		0x7ec5b259, 
		0x8f7ffffe, 
		0xf60b7de7, 
		0x0f7fffff, 
		0x1852f09c, 
		0x7f7fffff, 
		0xc1c00000, 
		0x91f3c4da, 
		0xc23c0000, 
		0xc2240000, 
		0x55e87c63, 
		0x3f800000, 
		0xc3d50a8c, 
		0x0c7ff000, 
		0xc2acb765, 
		0x39e2bc07, 
		0xc1d80000, 
		0x8c3d42a0, 
		0xc0a00000, 
		0x7e8415a1, 
		0x00000000, 
		0x80100000, 
		0x00000080, 
		0x0e3fffff, 
		0x00000000, 
		0x80000200, 
		0x80011111, 
		0x80000001, 
		0xc0800000, 
		0xff800000, 
		0xf214ad07, 
		0x23efde74, 
		0x5cdab9d6, 
		0xc20c0000, 
		0x0e1fffff, 
		0x42240000, 
		0x007fffff, 
		0x464c1499, 
		0x41000000, 
		0x80000000, 
		0x80000040, 
		0x7f53f781, 
		0xff800001, 
		0x0c400000, 
		0x0e0001ff, 
		0xa9aa4012, 
		0x8f7fffff, 
		0x00400000, 
		0xc0c00000, 
		0x0e000001, 
		0x42200000, 
		0x0e7fffff, 
		0xaaaaaaaa, 
		0x4eecbbbc, 
		0x0e00ffff, 
		0xec468148, 
		0x00000800, 
		0x42600000, 
		0xc0c00000, 
		0xcb8c4b40, 
		0x80800001, 
		0x0c7ffe00, 
		0xbf028f5c, 
		0xc2400000, 
		0x0c7ffffc, 
		0xb2c337d3, 
		0x42540000, 
		0x7f800000, 
		0x49c26436, 
		0x80000001, 
		0x00000000, 
		0x41f00000, 
		0xc1200000, 
		0x00400000, 
		0x7f7ffffe, 
		0xff800000, 
		0xc1d80000, 
		0x7f7ffffe, 
		0x0c7fffc0, 
		0xa6044f55, 
		0xc1000000, 
		0x42400000, 
		0x00000000, 
		0xc0e00000, 
		0x80306898, 
		0xcb8c4b40, 
		0x00000008, 
		0x03e1733c, 
		0x014ad4db, 
		0x0e000fff, 
		0x41e80000, 
		0x97c66d65, 
		0xd3d22a6d, 
		0x6b6607fc, 
		0x0e00001f, 
		0x51daba10, 
		0xc6ccd610, 
		0xa6559c59, 
		0xc1980000, 
		0x7f7ffffe, 
		0x7cc1feb2, 
		0xc1b9c70f, 
		0xe5aab434, 
		0x2eb8c776, 
		0x41200000, 
		0x0f7ffffd, 
		0x00020000, 
		0x00000002, 
		0xc0800000, 
		0xc1a00000, 
		0x7f7fffff, 
		0xc1500000, 
		0x80000020, 
		0x007e2a31, 
		0x8f7ffffd, 
		0x8f7fffff, 
		0x0c7fffe0, 
		0x00800000, 
		0x6beae6e5, 
		0xc8672436, 
		0xff800000, 
		0xc1000000, 
		0x46e9d7b9, 
		0x80000002, 
		0x80800001, 
		0x0c7fffff, 
		0x7fc00001, 
		0xbf028f5c, 
		0xd670a02f, 
		0xed54ac4f, 
		0x801442ca, 
		0x0e1fffff, 
		0x0e01ffff, 
		0x00011111, 
		0xf3d422b2, 
		0x80000200, 
		0x7f7ffffe, 
		0x00000800, 
		0x41c80000, 
		0xe39beb16, 
		0x00000100, 
		0x420c0000, 
		0xc25c0000, 
		0x80100000, 
		0x0e00ffff, 
		0x00100000, 
		0x8cc20409, 
		0x807fffff, 
		0xffc00000, 
		0x80000010, 
		0xc2700000, 
		0x42080000, 
		0x00000800, 
		0xe845c950, 
		0x0c7fc000, 
		0x80800003, 
		0x80ffffff, 
		0x9b9d6c3d, 
		0xffffffff, 
		0xcfa13fae, 
		0xff7fffff, 
		0x7fbc5120, 
		0xfcb36900, 
		0x362b0f61, 
		0x0c7ff800, 
		0xc1a00000, 
		0x003f2d7f, 
		0x00800003, 
		0x00000002, 
		0xc2300000, 
		0x80000000, 
		0x1adde074, 
		0x5beaa973, 
		0x42480000, 
		0x00800001, 
		0xc21c0000, 
		0x00800002, 
		0x8f7ffffc, 
		0x0c7ff800, 
		0xc2640000, 
		0xff800000, 
		0xc1c00000, 
		0xfd6c1c66, 
		0x41c80000, 
		0x00000001, 
		0x7f800000, 
		0xe0e7db73, 
		0x00000010, 
		0xc1880000, 
		0x80001000, 
		0xc2380000, 
		0x007ffffe, 
		0xffffffff, 
		0x80011111, 
		0x1aa955bc, 
		0x80000000, 
		0x0c7ffffe, 
		0xa24cee21, 
		0x00000100, 
		0xc1e00000, 
		0x41980000, 
		0x9151561d, 
		0x7f800000, 
		0x7b5b237b, 
		0x00040000, 
		0x0e7fffff, 
		0x80000000, 
		0x1298d097, 
		0x42340000, 
		0x0e0003ff, 
		0xff800000, 
		0x41600000, 
		0x41300000, 
		0x80800001, 
		0xd13fffbe, 
		0xff800001, 
		0xc2100000, 
		0xaa781527, 
		0x0e0003ff, 
		0x0e07ffff, 
		0x00000000, 
		0x41c00000, 
		0x0023fe93, 
		0x0c7ff800, 
		0x41100000, 
		0x0f7ffffe, 
		0x7f7fffff, 
		0xf9f1ce90, 
		0x00000008, 
		0x80000000, 
		0x7f902f4a, 
		0x42340000, 
		0x00000000, 
		0xefa296c6, 
		0x80000800, 
		0xc1e80000, 
		0x0c7c0000, 
		0xc21c0000, 
		0x98b12b3c, 
		0x00000000, 
		0x46bfdff5, 
		0xc2740000, 
		0x0e0000ff, 
		0x00000100, 
		0xfc80ba66, 
		0x69dc9e9a, 
		0xf4a484ce, 
		0x426c0000, 
		0xad49c83f, 
		0x00800000, 
		0x7255a8c2, 
		0x00000000, 
		0xefbf22c9, 
		0x8f7ffffe, 
		0x4a8c1e53, 
		0x88e0b36c, 
		0x0e0fffff, 
		0x00800003, 
		0x80000000, 
		0x80800001, 
		0xc1f80000, 
		0xc1d91538, 
		0x04653cdc, 
		0x80000400, 
		0x40400000, 
		0x42200000, 
		0x00800001, 
		0x0f7ffffd, 
		0x80000000, 
		0x6e37e328, 
		0x4fbeab44, 
		0x0e1fffff, 
		0x80000002, 
		0x00000000, 
		0x55555555, 
		0x0e0007ff, 
		0x7f7ffffe, 
		0xc1000000, 
		0x80800001, 
		0x41d00000, 
		0x41400000, 
		0x5ac00dfa, 
		0x00000000, 
		0x80040000, 
		0xbfd5e03f, 
		0x0e000003, 
		0x9babe80c, 
		0x80800002, 
		0x7f81285c, 
		0x807fffff, 
		0xb02bcac7, 
		0xc1d80000, 
		0x9738c041, 
		0xc0e00000, 
		0x8f7ffffd, 
		0x00000004, 
		0x64b90dc4, 
		0x80000000, 
		0x0c7e0000, 
		0x00100000, 
		0xc1c80000, 
		0x80000001, 
		0x80000002, 
		0x5ce61a52, 
		0x7f800000, 
		0xc1c80000, 
		0xa32617cd, 
		0x00400000, 
		0xff800001, 
		0x00000200, 
		0x80800001, 
		0x59cafbf4, 
		0xfff1fd53, 
		0x2c071bd3, 
		0x007ffffe, 
		0xff000000, 
		0xc1b00000, 
		0x40000000, 
		0xc22c0000, 
		0x3027beba, 
		0x80100000, 
		0xc2480000, 
		0xd968bcd6, 
		0x80000002, 
		0x7f000000, 
		0x0c600000, 
		0x0e0001ff, 
		0x0003f209, 
		0x0a5baeec, 
		0x00000000, 
		0x40c00000, 
		0xc2600000, 
		0x09ee6e3e, 
		0x00000010, 
		0x0e001fff, 
		0x00080000, 
		0xcfa041fe, 
		0xa6b0ba0b, 
		0x00000000, 
		0xb21d2686, 
		0x0e007fff, 
		0xffac47b5, 
		0x80800003, 
		0x6e3db436, 
		0x0c600000, 
		0x407a5989, 
		0x25a242b4, 
		0x083f91e1, 
		0xc5568948, 
		0x2e8e82fd, 
		0x00000000, 
		0x422c0000, 
		0x07f8406e, 
		0x420c0000, 
		0xcb000000, 
		0x80800001, 
		0x80800003, 
		0xc2480000, 
		0x7d1a9e85, 
		0xc2200000, 
		0xe1eec64d, 
		0x42740000, 
		0x80000800, 
		0x4b610d18, 
		0x41300000, 
		0x7f7ffffe, 
		0x80080000, 
		0x80200000, 
		0x0e1fffff, 
		0xc5215dc3, 
		0x056daed5, 
		0x89334b8a, 
		0x427c0000, 
		0x80000002, 
		0x424c0000, 
		0x80800001, 
		0x00000000, 
		0xc3e2b506, 
		0x00000002, 
		0x86e1e4ac, 
		0x0e00ffff, 
		0x9c64e661, 
		0xfaab9481, 
		0xc2400000, 
		0xc24c0000, 
		0x00000000, 
		0x80010000, 
		0x42100000, 
		0x3460e86d, 
		0xa48cd1a8, 
		0x00800002, 
		0x0e0001ff, 
		0x00000000, 
		0x42340000, 
		0x41600000, 
		0xc2540000, 
		0x7f800000, 
		0x42680000, 
		0x41d80000, 
		0x7f000000, 
		0x80000000, 
		0x80004000, 
		0x7f800000, 
		0xc21c0000, 
		0x80800000, 
		0x554907a4, 
		0x51e5eedf, 
		0x7f7fffff, 
		0x7f800000, 
		0x80800002, 
		0xc2500000, 
		0x38bbc00e, 
		0x41c80000, 
		0xf2815cea, 
		0x807fffff, 
		0x6afb5879, 
		0x3fe85163, 
		0x930c77a4, 
		0x7f800000, 
		0xff800000, 
		0x00200000, 
		0x5c606cad, 
		0x0e1fffff, 
		0x719aa80c, 
		0x9b40031c, 
		0x25a3b34d, 
		0x424c0000, 
		0xde77a2ac, 
		0x00000001, 
		0x80000008, 
		0xf5c60435, 
		0x4cb9bcb3, 
		0xc2040000, 
		0xc1400000, 
		0x7159f7f8, 
		0x7f8810d6, 
		0x96c3ecb9, 
		0x1b2ef070, 
		0x393cc26d, 
		0xc1200000, 
		0x00800000, 
		0x00000080, 
		0x80040000, 
		0xafc72f97, 
		0x0e003fff, 
		0xd4eac08d, 
		0xef2f1000, 
		0x7f800000, 
		0x3f800001, 
		0xc22c0000, 
		0x8f7ffffe, 
		0x00800000, 
		0x80000010, 
		0xc0e00000, 
		0xc1300000, 
		0x80800002, 
		0x00000200, 
		0x7f82c956, 
		0xc0400000, 
		0xc2280000, 
		0xb7e6efbd, 
		0x7f800000, 
		0xc0e00000, 
		0x00800001, 
		0x5c8dcc67, 
		0x0c7ffe00, 
		0x00000800, 
		0xff800000, 
		0x69ef400d, 
		0xa8621bac, 
		0x41a80000, 
		0x728e1144, 
		0xc1e00000, 
		0xff800000, 
		0xc2780000, 
		0x0e7fffff, 
		0xff800000, 
		0x93eb8bc8, 
		0x80010000, 
		0x4c02401d, 
		0x271218bd, 
		0x00000000, 
		0x51d364c0, 
		0x42780000, 
		0x7fbfffff, 
		0x0f7fffff, 
		0x41d80000, 
		0x00400000, 
		0x0c7f8000, 
		0xff800000, 
		0x00000000, 
		0xc2700000, 
		0x426c0000, 
		0xac364f7f, 
		0x2f91c261, 
		0x0c7c0000, 
		0x00000080, 
		0x80000100, 
		0x800d707f, 
		0x4db7b42f, 
		0x823ae4be, 
		0x42740000, 
		0x73df3237, 
		0x80000002, 
		0x2bf8b488, 
		0x007ffffe, 
		0xe3585fd1, 
		0x36876b98, 
		0x6e07ceca, 
		0xc2300000, 
		0x807ffffe, 
		0xeae7c1e9, 
		0xc79dd876, 
		0x7faabfb8, 
		0xc2400000, 
		0xc1a80000, 
		0x00000001, 
		0x007fffff, 
		0x8f84fec7, 
		0xeaba9956, 
		0x42c9d246, 
		0x80000000, 
		0x80040000, 
		0x80000000, 
		0x8f7ffffc, 
		0x7f8885a2, 
		0x00020000, 
		0x0286e2d6, 
		0xa3c73a33, 
		0x0e0007ff, 
		0x7f800000, 
		0x73033fa9, 
		0x40000000, 
		0x3e2b1663, 
		0x24154f5b, 
		0x4a05385a, 
		0x646de388, 
		0x80800001, 
		0x0e000003, 
		0xc2480000, 
		0xef37d4bb, 
		0x42780000, 
		0x12f08886, 
		0xc21c0000, 
		0x80000000, 
		0x7ff1ef8c, 
		0x0c7fffc0, 
		0x87697ca8, 
		0x0c7fffc0, 
		0xc1c00000, 
		0xc2580000, 
		0x65831e86, 
		0x0c700000, 
		0x0c700000, 
		0xc1f80000, 
		0x001e13e2, 
		0xd21f248b, 
		0x7fc00000, 
		0x00000001, 
		0xadab0945, 
		0xff7ffffe, 
		0x7fc00000, 
		0x002b1601, 
		0x7f800000, 
		0x0c7c0000, 
		0xc27c0000, 
		0x6555bf7d, 
		0xc2040000, 
		0x42100000, 
		0x42300000, 
		0xff800000, 
		0x12395303, 
		0x00000400, 
		0x41600000, 
		0x8c64ac61, 
		0x62d35837, 
		0x41f80000, 
		0x98ea1a69, 
		0x42140000, 
		0xc1a80000, 
		0xc2080000, 
		0x68902008, 
		0x8f7fffff, 
		0xf9bda259, 
		0x424c0000, 
		0xc27c0000, 
		0x700eb647, 
		0xffc00001, 
		0xdd6612af, 
		0xa8a42ba2, 
		0xff937b24, 
		0x40800000, 
		0xc2480000, 
		0x80000000, 
		0x7f800001, 
		0x41e00000, 
		0x80189188, 
		0x423c0000, 
		0x4f0c8565, 
		0x227aad31, 
		0x00000000, 
		0x41b80000, 
		0x0e00ffff, 
		0x00000000, 
		0x35e70b56, 
		0x427c0000, 
		0x80800001, 
		0xc2400000, 
		0x52ce4f58, 
		0x00800003, 
		0x00000000, 
		0x0c600000, 
		0x41900000, 
		0xff800000, 
		0x30f7b63b, 
		0xebb798d2  
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
		0x000003ef, 
		0x00000159, 
		0x000000b3, 
		0x00000359, 
		0x000003e3, 
		0x00000207, 
		0x00000355, 
		0x00000117, 
		0x00000205, 
		0x00000197, 
		0x00000221, 
		0x000003af, 
		0x000003f9, 
		0x0000012d, 
		0x000003a7, 
		0x00000343, 
		0x000002a9, 
		0x0000037d, 
		0x0000007d, 
		0x000003cf, 
		0x00000039, 
		0x00000051, 
		0x000001c3, 
		0x00000389, 
		0x00000009, 
		0x0000000d, 
		0x00000191, 
		0x00000389, 
		0x000002c3, 
		0x000002ed, 
		0x0000010d, 
		0x00000399, 
		0x0000028f, 
		0x0000004f, 
		0x0000016b, 
		0x000003fd, 
		0x000000a5, 
		0x00000321, 
		0x00000113, 
		0x000002bd, 
		0x000002a3, 
		0x000000db, 
		0x000002fd, 
		0x00000389, 
		0x00000399, 
		0x000002eb, 
		0x000002d7, 
		0x000000df, 
		0x000000d5, 
		0x00000061, 
		0x00000091, 
		0x0000009f, 
		0x000001a3, 
		0x0000017b, 
		0x00000155, 
		0x00000221, 
		0x000003ab, 
		0x000002c5, 
		0x0000027f, 
		0x00000335, 
		0x0000011f, 
		0x000001af, 
		0x0000031f, 
		0x000003d1, 
		0x000002d7, 
		0x000002c3, 
		0x00000269, 
		0x00000209, 
		0x00000047, 
		0x00000179, 
		0x00000035, 
		0x000001fb, 
		0x00000375, 
		0x000001b1, 
		0x00000113, 
		0x00000203, 
		0x00000083, 
		0x000003a9, 
		0x000003fd, 
		0x00000127, 
		0x0000006d, 
		0x000001c5, 
		0x0000015f, 
		0x000003fb, 
		0x00000283, 
		0x00000307, 
		0x000001d1, 
		0x0000010b, 
		0x00000121, 
		0x000000f1, 
		0x0000025d, 
		0x00000047, 
		0x000003d5, 
		0x000003f5, 
		0x00000049, 
		0x0000031b, 
		0x0000020b, 
		0x0000007f, 
		0x000002ff, 
		0x0000021d, 
		0x000002b9, 
		0x00000079, 
		0x00000205, 
		0x000003bf, 
		0x000000ff, 
		0x00000109, 
		0x00000083, 
		0x00000203, 
		0x000000f3, 
		0x00000157, 
		0x00000033, 
		0x000002cb, 
		0x000003fd, 
		0x000001b7, 
		0x00000289, 
		0x00000261, 
		0x000000e3, 
		0x0000010f, 
		0x00000255, 
		0x00000143, 
		0x00000039, 
		0x00000283, 
		0x000003fd, 
		0x000000a5, 
		0x0000021b, 
		0x000003d3, 
		0x0000000f, 
		0x0000017d, 
		0x00000073, 
		0x000002c7, 
		0x000003c7, 
		0x00000237, 
		0x000001f1, 
		0x00000209, 
		0x000003f5, 
		0x00000085, 
		0x000001b5, 
		0x00000163, 
		0x000002c1, 
		0x0000027b, 
		0x000002c1, 
		0x00000369, 
		0x000000f5, 
		0x00000269, 
		0x00000037, 
		0x00000165, 
		0x000002b7, 
		0x0000036d, 
		0x00000303, 
		0x00000293, 
		0x000000f3, 
		0x0000019d, 
		0x0000015d, 
		0x000002b9, 
		0x0000030b, 
		0x0000008d, 
		0x00000131, 
		0x0000018b, 
		0x000003af, 
		0x0000020f, 
		0x000003b1, 
		0x00000119, 
		0x00000351, 
		0x000002d9, 
		0x00000275, 
		0x00000043, 
		0x00000299, 
		0x00000055, 
		0x0000016f, 
		0x000003d9, 
		0x00000275, 
		0x00000177, 
		0x00000285, 
		0x000001eb, 
		0x00000361, 
		0x000003fb, 
		0x000003c7, 
		0x000003eb, 
		0x00000251, 
		0x00000089, 
		0x000001bd, 
		0x000002b5, 
		0x00000207, 
		0x000001d7, 
		0x000000d5, 
		0x00000339, 
		0x00000041, 
		0x0000015d, 
		0x000002dd, 
		0x000003cd, 
		0x000001c9, 
		0x000000c1, 
		0x0000021f, 
		0x0000027b, 
		0x000003eb, 
		0x0000009b, 
		0x00000147, 
		0x000001db, 
		0x000003d1, 
		0x00000245, 
		0x000002cb, 
		0x00000287, 
		0x000003b1, 
		0x0000031f, 
		0x00000345, 
		0x00000021, 
		0x00000063, 
		0x00000219, 
		0x00000295, 
		0x00000033, 
		0x00000141, 
		0x000001b1, 
		0x0000008b, 
		0x0000026f, 
		0x000002bf, 
		0x000003bf, 
		0x0000031b, 
		0x0000020f, 
		0x000003cb, 
		0x000001b9, 
		0x000000fb, 
		0x00000363, 
		0x000001ad, 
		0x00000299, 
		0x0000017b, 
		0x00000005, 
		0x00000317, 
		0x000002e3, 
		0x000000ad, 
		0x00000151, 
		0x00000367, 
		0x00000119, 
		0x00000043, 
		0x0000026f, 
		0x0000023f, 
		0x000002b3, 
		0x000002fd, 
		0x0000020b, 
		0x00000339, 
		0x0000036d, 
		0x00000051, 
		0x00000265, 
		0x0000032f, 
		0x0000026f, 
		0x00000097, 
		0x00000287, 
		0x000003d9, 
		0x0000033d, 
		0x000002c3, 
		0x00000115, 
		0x000003c3, 
		0x0000033f, 
		0x00000191, 
		0x00000175, 
		0x0000025b, 
		0x000002b9  
};

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
			"li x5, 155\n"
			"faddi.pi f2, f0, 220\n"
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
			"fexp.ps f18, f2\n"
			"fexp.ps f19, f3\n"
			"fexp.ps f20, f4\n"
			"fexp.ps f21, f5\n"
			"fexp.ps f22, f6\n"
			"fexp.ps f23, f7\n"
			"fexp.ps f24, f8\n"
			"fexp.ps f25, f9\n"
			"fexp.ps f26, f10\n"
			"fexp.ps f27, f11\n"
			"fexp.ps f28, f12\n"
			"fexp.ps f29, f13\n"
			"fexp.ps f30, f14\n"
			"fexp.ps f31, f15\n"
			"fexp.ps f0, f16\n"
			"fexp.ps f1, f17\n"
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
			"fexp.ps f18, f2\n"
			"fexp.ps f19, f3\n"
			"fexp.ps f20, f4\n"
			"fexp.ps f21, f5\n"
			"fexp.ps f22, f6\n"
			"fexp.ps f23, f7\n"
			"fexp.ps f24, f8\n"
			"fexp.ps f25, f9\n"
			"fexp.ps f26, f10\n"
			"fexp.ps f27, f11\n"
			"fexp.ps f28, f12\n"
			"fexp.ps f29, f13\n"
			"fexp.ps f30, f14\n"
			"fexp.ps f31, f15\n"
			"fexp.ps f0, f16\n"
			"fexp.ps f1, f17\n"
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
			"csrwi mhpmevent3, 17\n"   
			"csrwi mhpmevent4, 28\n"   
			"csrwi mhpmevent5, 19\n"   
			"csrwi mhpmevent6, 16\n"   
			"csrwi mhpmevent7, 4\n"   
			"csrwi mhpmevent8, 7\n"   
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
			"flw.ps f17,  992 (x5)\n"
			"flw.ps f27,  672 (x5)\n"
			"flw.ps f25,  768 (x5)\n"
			"flw.ps f18,  928 (x5)\n"
			"flw.ps f29,  416 (x5)\n"
			"flw.ps f26,  352 (x5)\n"
			"flw.ps f22,  576 (x5)\n"
			"flw.ps f13,  160 (x5)\n"
			"flw.ps f7,  640 (x5)\n"
			"flw.ps f4,  0 (x5)\n"
			"flw.ps f19,  96 (x5)\n"
			"flw.ps f16,  128 (x5)\n"
			"flw.ps f21,  64 (x5)\n"
			"flw.ps f15,  448 (x5)\n"
			"flw.ps f5,  608 (x5)\n"
			"flw.ps f28,  736 (x5)\n"
			"flw.ps f24,  320 (x5)\n"
			"flw.ps f20,  224 (x5)\n"
			"flw.ps f31,  544 (x5)\n"
			"flw.ps f3,  896 (x5)\n"
			"flw.ps f14,  256 (x5)\n"
			"flw.ps f6,  512 (x5)\n"
			"flw.ps f23,  192 (x5)\n"
			"flw.ps f11,  480 (x5)\n"
			"flw.ps f12,  832 (x5)\n"
			"flw.ps f30,  384 (x5)\n"
			"flw.ps f2,  288 (x5)\n"
			"flw.ps f9,  32 (x5)\n"
			"flw.ps f10,  864 (x5)\n"
			"flw.ps f8,  800 (x5)\n"
			"flw.ps f0,  704 (x5)\n"
			"flw.ps f1,  960 (x5)\n"
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
			"li x0, 0x64\n" 
			"slti x0, x0, 0x7f2\n"
			"la x31, check_sum_hid_0\n"
			"li x5, 0x5\n" 
			"sw x5, (x31)\n"
			"li x29, 0x5\n"
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
			"LBL_SEQID_0_FP_TRANS_ID_0_HID_0:\n"
			"la x16, rand_ieee754_32\n"
			"flw.ps f6, 800(x16)\n"
			"frcp.ps f7, f6\n" 
			"la x28, rand_int_32\n"
			"flw.ps f13, 32(x28)\n"
			"fmaxu.pi f7, f13, f13\n" 
			"la x28, rand_int_32\n"
			"flw.ps f19, 224(x28)\n"
			"flw.ps f28, 32(x28)\n"
			"fmulhu.pi f20, f19, f28\n" 
			"la x10, rand_int_32\n"
			"flw.ps f26, 640(x10)\n"
			"flw.ps f6, 480(x10)\n"
			"fsra.pi f19, f26, f6\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f11, 992(x22)\n"
			"flw.ps f13, 288(x22)\n"
			"fsgnj.ps f15, f11, f13\n" 
			"la x16, rand_int_32\n"
			"flw.ps f11, 288(x16)\n"
			"flw.ps f28, 800(x16)\n"
			"fmax.pi f18, f11, f28\n" 
			"la x23, rand_int_32\n"
			"flw.ps f3, 896(x23)\n"
			"flw.ps f20, 320(x23)\n"
			"fmax.pi f17, f3, f20\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f28, 256(x20)\n"
			"flw.ps f14, 32(x20)\n"
			"fadd.ps f15, f28, f14, rmm\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f26, 224(x28)\n"
			"flw.ps f5, 320(x28)\n"
			"fsub.s f20, f26, f5, rmm\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f5, 704(x22)\n"
			"flw.ps f13, 192(x22)\n"
			"fsgnjx.s f11, f5, f13\n" 
			"la x6, rand_int_32\n"
			"flw.ps f5, 576(x6)\n"
			"flw.ps f7, 192(x6)\n"
			"fsub.pi f5, f5, f7\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f27, 512(x14)\n"
			"flw.ps f13, 448(x14)\n"
			"flw.ps f17, 128(x14)\n"
			"fmadd.s f12, f27, f13, f17, rup\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_0:\n"
			"maskor m4, m4, m2\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0x8ed5e795e9\n"
			"flw.ps f5, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x13, rand_ieee754_32\n"
			"flw.ps f14, 480(x13)\n"
			"flw.ps f12, 928(x13)\n"
			"fcmov.ps f15, f14, f12, f12\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f28, 128(x27)\n"
			"flw.ps f12, 608(x27)\n"
			"flw.ps f11, 576(x27)\n"
			"fnmsub.ps f27, f28, f12, f11, rup\n" 
			"la x27, rand_int_32\n"
			"flw.ps f5, 864(x27)\n"
			"flw.ps f26, 608(x27)\n"
			"fminu.pi f5, f5, f26\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_0_RDX_ID_0_SHIRE_0_HID_0:\n"
			"li x6, 0x1000000000790021\n"
			"csrr x7, mhartid\n"
			"srli x7, x7, 4\n"
			"slli x7, x7, 6\n"
			"add x6, x6, x7\n"
			"csrw tensor_reduce, x6\n" 
			"csrwi tensor_wait, 9\n" 
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=0); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ITER_0_HID_0:\n"
			"la x31, self_check_32\n"
			"li x5, 0xc000000000000d\n"
			"add x5, x5, x31\n"
			"li x6, 0x10000000000002\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x3 << TENSOR_FMA32_B_NUM_COLS) | (0xd << TENSOR_FMA32_A_NUM_ROWS) | (0x2 << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x1 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1c << TENSOR_FMA32_SCP_LOC_B) | (0x6 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0x3\n"
			"bne x5, x31, LBL_FAIL_HID_0\n"
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ITER_1_HID_0:\n"
			"la x31, self_check_16\n"
			"li x5, 0xa000000000000e\n"
			"add x5, x5, x31\n"
			"li x6, 0x1000000000000a\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x3 << TENSOR_FMA16_B_NUM_COLS) | (0xe << TENSOR_FMA16_A_NUM_ROWS) | (0xa << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x1 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x19 << TENSOR_FMA16_SCP_LOC_B) | (0x5 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x10 << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0xe << TENSOR_QUANT_QUANT_ROW) | (0x23 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x2 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_0\n"
			"csrw tensor_error, x0\n"
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_0\n"
			"addi x30, x30, 1\n"
			"la x28, rand_ieee754_32\n"
			"flw.ps f26, 576(x28)\n"
			"flw.ps f15, 448(x28)\n"
			"fsgnj.ps f17, f26, f15\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f7, 224(x27)\n"
			"fcvt.pw.ps f19, f7, rtz\n" 
			"la x14, rand_int_32\n"
			"flw.ps f18, 480(x14)\n"
			"flw.ps f11, 320(x14)\n"
			"fdivu.pi f28, f18, f11\n" 
			"LBL_SEQID_1_M0_WRITE_ID_2_HID_0:\n"
			"fltm.pi m6, f20, f13\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x27, rand_int_32\n"
			"flw.ps f26, 160(x27)\n"
			"fsat8.pi f18, f26\n" 
			"li x5, 0xf35ee9fbdd3f39d6\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0x75f52cedef0fd305\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"la x10, rand_ieee754_32\n"
			"flw.ps f3, 864(x10)\n"
			"flw.ps f7, 288(x10)\n"
			"fsgnjn.ps f6, f3, f7\n" 
			"li x5, 0xcc0334c09dad426a\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0x8cc83b23ee77407d\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"la x10, rand_int_32\n"
			"flw.ps f26, 992(x10)\n"
			"flw.ps f28, 96(x10)\n"
			"fmin.pi f27, f26, f28\n" 
			"la x13, rand_int_32\n"
			"flw.ps f19, 800(x13)\n"
			"flw.ps f20, 352(x13)\n"
			"fxor.pi f27, f19, f20\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f20, 576(x14)\n"
			"ffrc.ps f15, f20\n" 
			"LBL_SEQID_1_M0_WRITE_ID_3_HID_0:\n"
			"mova.m.x x23\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x13, rand_ieee754_32\n"
			"flw.ps f11, 544(x13)\n"
			"fround.ps f27, f11, rne\n" 
			"li x5, 0xea0f\n"
			"csrw tensor_mask, x5\n"	
			"la x16, rand_int_32\n"
			"flw.ps f3, 320(x16)\n"
			"fcvt.ps.pw f6, f3, rne\n" 
			"la x10, rand_int_32\n"
			"flw.ps f5, 544(x10)\n"
			"flw.ps f11, 992(x10)\n"
			"feq.pi f5, f5, f11\n" 
			"la x6, rand_int_32\n"
			"flw.ps f5, 832(x6)\n"
			"faddi.pi f11, f5, 0x1cb\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f18, 128(x22)\n"
			"fclass.s x20, f18\n" 
			"la x13, rand_int_32\n"
			"flw.ps f17, 192(x13)\n"
			"fcvt.ps.pwu f7, f17, rdn\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_1_RDX_ID_1_SHIRE_0_HID_0:\n"
			"li x14, 0x2c00000000700020\n"
			"csrr x5, mhartid\n"
			"srli x5, x5, 4\n"
			"slli x5, x5, 6\n"
			"add x14, x14, x5\n"
			"csrw tensor_reduce, x14\n" 
			"csrwi tensor_wait, 9\n" 
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ID_796335_HID_0:\n"
			"li x5, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x3 << TENSOR_FMA16_B_NUM_COLS) | (0x9 << TENSOR_FMA16_A_NUM_ROWS) | (0xc << TENSOR_FMA16_A_NUM_COLS) | (0xc << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0xe2 << TENSOR_FMA16_SCP_LOC_B) | (0x25 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x0 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x6, ((0x16 << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0x9 << TENSOR_QUANT_QUANT_ROW) | (0x3f << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x7 << TENSOR_QUANT_TRANSF4) | (0x7 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ID_919921_HID_0:\n"
			"li x6, ((0x12 << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0xf << TENSOR_QUANT_QUANT_ROW) | (0xf << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x2 << TENSOR_QUANT_TRANSF5) | (0x7 << TENSOR_QUANT_TRANSF4) | (0x6 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ID_958149_HID_0:\n"
			"li x6, ((0xf << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0x2 << TENSOR_QUANT_QUANT_ROW) | (0x25 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x1 << TENSOR_QUANT_TRANSF5) | (0x4 << TENSOR_QUANT_TRANSF4) | (0xa << TENSOR_QUANT_TRANSF3) | (0xa << TENSOR_QUANT_TRANSF2) | (0xa << TENSOR_QUANT_TRANSF1) | (0x3 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
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
			"la x31, self_check_16\n"
			"li x5, 0x2\n"
			"add x5, x5, x31\n"
			"li x6, 0x10000000000004\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x3 << TENSOR_FMA16_B_NUM_COLS) | (0x2 << TENSOR_FMA16_A_NUM_ROWS) | (0x4 << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x1 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1d << TENSOR_FMA16_SCP_LOC_B) | (0x0 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x9, ((0xe << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0x2 << TENSOR_QUANT_QUANT_ROW) | (0x35 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x0 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 10\n"
			"LBL_SEQID_1_TLD_XMA_STR_SEQ_ITER_1_HID_0:\n"
			"la x31, self_check_16\n"
			"li x5, 0xa0000000000005\n"
			"add x5, x5, x31\n"
			"li x6, 0x380000000000008\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x1 << TENSOR_FMA16_B_NUM_COLS) | (0x5 << TENSOR_FMA16_A_NUM_ROWS) | (0x8 << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1c << TENSOR_FMA16_SCP_LOC_B) | (0x5 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x6 << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0x5 << TENSOR_QUANT_QUANT_ROW) | (0x1d << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x6 << TENSOR_QUANT_TRANSF4) | (0x6 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0x12\n"
			"bne x5, x31, LBL_FAIL_HID_0\n"
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 10\n"
			"LBL_SEQID_1_TLD_XMA_STR_SEQ_ITER_2_HID_0:\n"
			"la x31, self_check_32\n"
			"li x5, 0x2000000000000f\n"
			"add x5, x5, x31\n"
			"li x6, 0x10000000000009\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x1 << TENSOR_FMA32_B_NUM_COLS) | (0xf << TENSOR_FMA32_A_NUM_ROWS) | (0x9 << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x1 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1d << TENSOR_FMA32_SCP_LOC_B) | (0x1 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x9, ((0xd << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0xf << TENSOR_QUANT_QUANT_ROW) | (0x30 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x0 << TENSOR_QUANT_TRANSF1) | (0x0 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_0\n"
			"csrw tensor_error, x0\n"
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_0\n"
			"addi x30, x30, 1\n"
			"la x10, rand_ieee754_32\n"
			"flw.ps f7, 608(x10)\n"
			"flt.ps f14, f7, f7\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f28, 224(x20)\n"
			"flw.ps f5, 384(x20)\n"
			"feq.s x28, f28, f5\n" 
			"la x27, rand_int_32\n"
			"flw.ps f11, 352(x27)\n"
			"flw.ps f12, 512(x27)\n"
			"fmulhu.pi f20, f11, f12\n" 
			"fbci.ps f27, 0x1d173\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f28, 448(x6)\n"
			"fcvt.w.s x10, f28, rtz\n" 
			"la x27, rand_int_32\n"
			"flw.ps f11, 416(x27)\n"
			"flw.ps f19, 608(x27)\n"
			"frem.pi f6, f11, f19\n" 
			"la x28, rand_int_32\n"
			"flw.ps f13, 224(x28)\n"
			"fandi.pi f20, f13, 0xe1\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f13, 416(x16)\n"
			"flw.ps f26, 64(x16)\n"
			"fcmovm.ps f26, f13, f26\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f3, 896(x6)\n"
			"flw.ps f7, 896(x6)\n"
			"fsgnjn.ps f28, f3, f7\n" 
			"LBL_SEQID_2_M0_WRITE_ID_4_HID_0:\n"
			"masknot m2, m4\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_2_FP_TRANS_ID_5_HID_0:\n"
			"la x28, rand_ieee754_32\n"
			"flw.ps f26, 224(x28)\n"
			"frcp.ps f19, f26\n" 
			"mova.x.m x6\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f18, 384(x12)\n"
			"flw.ps f14, 640(x12)\n"
			"fsgnjn.ps f13, f18, f14\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x20, rand_ieee754_32\n"
			"flw.ps f15, 64(x20)\n"
			"frsq.ps f7, f15\n" 
			"LBL_SEQID_2_M0_WRITE_ID_6_HID_0:\n"
			"fsetm.pi m6, f17\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x28, rand_ieee754_32\n"
			"flw.ps f27, 128(x28)\n"
			"flw.ps f18, 800(x28)\n"
			"flw.ps f19, 576(x28)\n"
			"fmsub.ps f7, f27, f18, f19, rmm\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_2_XMA_QNT_STR_SEQ_ID_130083_HID_0:\n"
			"li x6, ((0x9 << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0x9 << TENSOR_QUANT_QUANT_ROW) | (0xa << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0xa << TENSOR_QUANT_TRANSF6) | (0xa << TENSOR_QUANT_TRANSF5) | (0x3 << TENSOR_QUANT_TRANSF4) | (0x3 << TENSOR_QUANT_TRANSF3) | (0x5 << TENSOR_QUANT_TRANSF2) | (0x9 << TENSOR_QUANT_TRANSF1) | (0x9 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_0\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_2_XMA_QNT_STR_SEQ_ITER_0_HID_0:\n"
			"li x5, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x2 << TENSOR_FMA32_B_NUM_COLS) | (0xc << TENSOR_FMA32_A_NUM_ROWS) | (0x6 << TENSOR_FMA32_A_NUM_COLS) | (0xd << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0xd0 << TENSOR_FMA32_SCP_LOC_B) | (0xc5 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
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
			"la x13, rand_int_32\n"
			"flw.ps f19, 224(x13)\n"
			"flw.ps f18, 96(x13)\n"
			"feq.pi f17, f19, f18\n" 
			"la x10, rand_int_32\n"
			"flw.ps f11, 736(x10)\n"
			"fcvt.ps.pwu f12, f11, rtz\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f28, 544(x23)\n"
			"flw.ps f12, 352(x23)\n"
			"flt.s x23, f28, f12\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f17, 864(x14)\n"
			"flw.ps f15, 0(x14)\n"
			"flw.ps f6, 0(x14)\n"
			"fmadd.s f14, f17, f15, f6, rdn\n" 
			"la x10, rand_int_32\n"
			"flw.ps f7, 32(x10)\n"
			"fcvt.ps.pw f19, f7, rmm\n" 
			"li x20, 0x9fff864d9de9da73\n"
			"fcvt.s.w f19, x20, rtz\n" 
			"fbci.ps f7, 0x53c42\n" 
			"la x28, rand_int_32\n"
			"flw.ps f26, 832(x28)\n"
			"fsrl.pi f13, f26, f26\n" 
			"LBL_SEQID_3_M0_WRITE_ID_7_HID_0:\n"
			"flem.ps m6, f20, f5\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x14, rand_ieee754_32\n"
			"flw.ps f11, 32(x14)\n"
			"ffrc.ps f5, f11\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f15, 352(x14)\n"
			"flw.ps f3, 96(x14)\n"
			"fdiv.ps f17, f15, f3, rne\n" 
			"LBL_SEQID_3_FP_TRANS_ID_8_HID_0:\n"
			"la x13, rand_ieee754_32\n"
			"flw.ps f18, 960(x13)\n"
			"flog.ps f7, f18\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f11, 736(x28)\n"
			"fswizz.ps f11, f11, 0x3\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f17, 256(x14)\n"
			"flw.ps f7, 320(x14)\n"
			"flt.s x16, f17, f7\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f11, 640(x28)\n"
			"flw.ps f15, 416(x28)\n"
			"fmax.s f13, f11, f15\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f27, 896(x12)\n"
			"flw.ps f11, 896(x12)\n"
			"flt.ps f7, f27, f11\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_3_XMA_QNT_STR_SEQ_ITER_0_HID_0:\n"
			"li x6, ((0x5 << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0x6 << TENSOR_QUANT_QUANT_ROW) | (0x13 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x2 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 10\n"
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
			"la x27, rand_int_32\n"
			"flw.ps f20, 0(x27)\n"
			"fsrai.pi f11, f20, 0x7\n" 
			"li x5, 0x9d5e\n"
			"csrw tensor_mask, x5\n"	
			"LBL_SEQID_4_M0_WRITE_ID_9_HID_0:\n"
			"mova.m.x x6\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_4_M0_WRITE_ID_10_HID_0:\n"
			"maskor m6, m6, m2\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_4_FP_TRANS_ID_11_HID_0:\n"
			"la x12, rand_ieee754_32\n"
			"flw.ps f26, 448(x12)\n"
			"fexp.ps f5, f26\n" 
			"la x20, rand_int_32\n"
			"flw.ps f13, 608(x20)\n"
			"flw.ps f20, 352(x20)\n"
			"fmax.pi f12, f13, f20\n" 
			"la x10, rand_int_32\n"
			"flw.ps f26, 992(x10)\n"
			"flw.ps f3, 864(x10)\n"
			"fmaxu.pi f6, f26, f3\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x28, rand_ieee754_32\n"
			"flw.ps f27, 160(x28)\n"
			"fcvt.pw.ps f11, f27, rdn\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f20, 288(x16)\n"
			"fmul.ps f12, f20, f20, rmm\n" 
			"la x28, rand_int_32\n"
			"flw.ps f6, 224(x28)\n"
			"fsrli.pi f20, f6, 0xd\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f18, 352(x16)\n"
			"fcvt.wu.s x16, f18, rne\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f11, 992(x27)\n"
			"fcvt.lu.s x23, f11, rne\n" 
			"la x27, rand_int_32\n"
			"flw.ps f15, 768(x27)\n"
			"fsrli.pi f5, f15, 0x5\n" 
			"la x13, rand_int_32\n"
			"flw.ps f27, 192(x13)\n"
			"fsatu8.pi f17, f27\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f17, 864(x6)\n"
			"fcvt.wu.s x20, f17, rmm\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f11, 64(x22)\n"
			"flw.ps f5, 864(x22)\n"
			"fadd.ps f19, f11, f5, rmm\n" 
			"LBL_SEQID_4_FP_TRANS_ID_12_HID_0:\n"
			"la x10, rand_ieee754_32\n"
			"flw.ps f11, 832(x10)\n"
			"fexp.ps f15, f11\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 0x3fee\n"
			"csrw tensor_mask, x5\n"	
			"LBL_SEQID_4_XMA_QNT_STR_SEQ_ITER_0_HID_0:\n"
			"li x5, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x2 << TENSOR_FMA32_B_NUM_COLS) | (0xc << TENSOR_FMA32_A_NUM_ROWS) | (0x2 << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0xeb << TENSOR_FMA32_SCP_LOC_B) | (0x2f << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x6, ((0x1b << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0xc << TENSOR_QUANT_QUANT_ROW) | (0x16 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x6 << TENSOR_QUANT_TRANSF6) | (0x7 << TENSOR_QUANT_TRANSF5) | (0x7 << TENSOR_QUANT_TRANSF4) | (0x7 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"LBL_HPM_3_CORE_TS_OPS_FAIL_HID_0:\n"          
			"csrr x5, mhpmcounter3\n"
			"sd x5, 32 (x31)\n"
			"LBL_HPM_4_CORE_TQUANT_INST_FAIL_HID_0:\n"          
			"csrr x5, mhpmcounter4\n"
			"sd x5, 40 (x31)\n"
			"LBL_HPM_5_CORE_TIMA_OPS_FAIL_HID_0:\n"          
			"csrr x5, mhpmcounter5\n"
			"sd x5, 48 (x31)\n"
			"LBL_HPM_6_CORE_TS_INST_FAIL_HID_0:\n"          
			"csrr x5, mhpmcounter6\n"
			"sd x5, 56 (x31)\n"
			"LBL_HPM_7_NEIGH_TX_NEIGH_COOP_TLD_REQ_FAIL_HID_0:\n"          
			"csrr x5, mhpmcounter7\n"
			"sd x5, 64 (x31)\n"
			"LBL_HPM_8_NEIGH_RX_COOP_TST_RSP_FAIL_HID_0:\n"          
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
			"LBL_HPM_3_CORE_TS_OPS_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter3\n"
			"sd x5, 32 (x31)\n"
			"LBL_HPM_4_CORE_TQUANT_INST_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter4\n"
			"sd x5, 40 (x31)\n"
			"LBL_HPM_5_CORE_TIMA_OPS_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter5\n"
			"sd x5, 48 (x31)\n"
			"LBL_HPM_6_CORE_TS_INST_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter6\n"
			"sd x5, 56 (x31)\n"
			"LBL_HPM_7_NEIGH_TX_NEIGH_COOP_TLD_REQ_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter7\n"
			"sd x5, 64 (x31)\n"
			"LBL_HPM_8_NEIGH_RX_COOP_TST_RSP_PASS_HID_0:\n"          
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
			"csrwi mhpmevent3, 4\n"   
			"csrwi mhpmevent4, 8\n"   
			"csrwi mhpmevent5, 6\n"   
			"csrwi mhpmevent6, 9\n"   
			"csrwi mhpmevent7, 21\n"   
			"csrwi mhpmevent8, 12\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f2,  320 (x5)\n"
			"flw.ps f20,  992 (x5)\n"
			"flw.ps f24,  192 (x5)\n"
			"flw.ps f19,  224 (x5)\n"
			"flw.ps f8,  576 (x5)\n"
			"flw.ps f18,  256 (x5)\n"
			"flw.ps f27,  96 (x5)\n"
			"flw.ps f28,  480 (x5)\n"
			"flw.ps f17,  64 (x5)\n"
			"flw.ps f9,  928 (x5)\n"
			"flw.ps f25,  832 (x5)\n"
			"flw.ps f10,  704 (x5)\n"
			"flw.ps f12,  640 (x5)\n"
			"flw.ps f30,  160 (x5)\n"
			"flw.ps f21,  608 (x5)\n"
			"flw.ps f4,  512 (x5)\n"
			"flw.ps f6,  416 (x5)\n"
			"flw.ps f15,  960 (x5)\n"
			"flw.ps f26,  672 (x5)\n"
			"flw.ps f1,  768 (x5)\n"
			"flw.ps f11,  448 (x5)\n"
			"flw.ps f3,  288 (x5)\n"
			"flw.ps f13,  896 (x5)\n"
			"flw.ps f29,  736 (x5)\n"
			"flw.ps f5,  0 (x5)\n"
			"flw.ps f0,  128 (x5)\n"
			"flw.ps f7,  544 (x5)\n"
			"flw.ps f16,  32 (x5)\n"
			"flw.ps f22,  384 (x5)\n"
			"flw.ps f31,  352 (x5)\n"
			"flw.ps f23,  800 (x5)\n"
			"flw.ps f14,  864 (x5)\n"
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
			"li x5, 0x5\n" 
			"sw x5, (x31)\n"
			"li x29, 0x5\n"
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
			"la x27, rand_int_32\n"
			"flw.ps f1, 128(x27)\n"
			"fsrli.pi f26, f1, 0x2\n" 
			"maskpopcz x22, m7\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f4, 512(x27)\n"
			"flw.ps f26, 992(x27)\n"
			"fsgnjx.s f27, f4, f26\n" 
			"la x13, rand_int_32\n"
			"flw.ps f9, 896(x13)\n"
			"fslli.pi f12, f9, 0x8\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f24, 736(x14)\n"
			"flw.ps f12, 736(x14)\n"
			"fadd.s f27, f24, f12, rup\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_1:\n"
			"fltm.pi m7, f26, f23\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x14, rand_int_32\n"
			"flw.ps f28, 224(x14)\n"
			"fnot.pi f23, f28\n" 
			"la x7, rand_int_32\n"
			"flw.ps f4, 576(x7)\n"
			"flw.ps f21, 768(x7)\n"
			"fle.pi f9, f4, f21\n" 
			"LBL_SEQID_0_FP_TRANS_ID_1_HID_1:\n"
			"la x14, rand_ieee754_32\n"
			"flw.ps f1, 544(x14)\n"
			"flog.ps f7, f1\n" 
			"la x12, rand_int_32\n"
			"flw.ps f0, 768(x12)\n"
			"flw.ps f17, 896(x12)\n"
			"fmul.pi f9, f0, f17\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f26, 672(x9)\n"
			"flw.ps f17, 992(x9)\n"
			"flw.ps f7, 128(x9)\n"
			"fnmsub.s f23, f26, f17, f7, rdn\n" 
			"la x14, rand_int_32\n"
			"flw.ps f27, 768(x14)\n"
			"flw.ps f24, 864(x14)\n"
			"fmulh.pi f24, f27, f24\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f24, 672(x11)\n"
			"flw.ps f1, 640(x11)\n"
			"fsub.s f23, f24, f1, rmm\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f28, 896(x27)\n"
			"flw.ps f4, 384(x27)\n"
			"fmin.ps f7, f28, f4\n" 
			"la x12, rand_int_32\n"
			"flw.ps f27, 544(x12)\n"
			"fnot.pi f26, f27\n" 
			"la x7, rand_ieee754_16\n"
			"flw.ps f21, 160(x7)\n"
			"fcvt.ps.f16 f23, f21\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_1\n"
			"addi x30, x30, 1\n"
			"la x27, rand_ieee754_32\n"
			"flw.ps f13, 128(x27)\n"
			"flw.ps f4, 864(x27)\n"
			"fmadd.s f28, f13, f4, f4, rdn\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f24, 448(x27)\n"
			"fcvt.wu.s x23, f24, rup\n" 
			"LBL_SEQID_1_M0_WRITE_ID_2_HID_1:\n"
			"flem.ps m2, f17, f7\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x22, rand_ieee754_32\n"
			"flw.ps f9, 576(x22)\n"
			"flw.ps f17, 832(x22)\n"
			"fsub.ps f17, f9, f17, rup\n" 
			"la x14, rand_int_32\n"
			"flw.ps f12, 928(x14)\n"
			"flw.ps f1, 416(x14)\n"
			"fand.pi f9, f12, f1\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f1, 864(x18)\n"
			"flw.ps f28, 256(x18)\n"
			"fle.s x11, f1, f28\n" 
			"la x23, rand_int_32\n"
			"flw.ps f2, 672(x23)\n"
			"fadd.pi f9, f2, f2\n" 
			"la x14, rand_int_32\n"
			"flw.ps f28, 288(x14)\n"
			"faddi.pi f4, f28, 0x103\n" 
			"la x18, rand_int_32\n"
			"flw.ps f23, 480(x18)\n"
			"flw.ps f0, 736(x18)\n"
			"fmax.pi f0, f23, f0\n" 
			"la x18, rand_int_32\n"
			"flw.ps f24, 608(x18)\n"
			"fslli.pi f27, f24, 0xf\n" 
			"la x13, rand_int_32\n"
			"flw.ps f4, 96(x13)\n"
			"flw.ps f0, 800(x13)\n"
			"fmax.pi f2, f4, f0\n" 
			"mova.x.m x9\n" 
			"LBL_SEQID_1_FP_TRANS_ID_3_HID_1:\n"
			"la x9, rand_ieee754_32\n"
			"flw.ps f21, 448(x9)\n"
			"frcp.ps f9, f21\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f27, 416(x22)\n"
			"flw.ps f28, 512(x22)\n"
			"fsgnj.ps f4, f27, f28\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f0, 992(x14)\n"
			"fcvt.pwu.ps f4, f0, rmm\n" 
			"la x18, rand_int_32\n"
			"flw.ps f21, 224(x18)\n"
			"flw.ps f2, 896(x18)\n"
			"fmaxu.pi f1, f21, f2\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_1\n"
			"addi x30, x30, 1\n"
			"la x14, rand_ieee754_32\n"
			"flw.ps f2, 672(x14)\n"
			"flw.ps f17, 384(x14)\n"
			"flt.ps f21, f2, f17\n" 
			"la x12, rand_int_32\n"
			"flw.ps f28, 448(x12)\n"
			"flw.ps f27, 864(x12)\n"
			"frem.pi f12, f28, f27\n" 
			"la x22, rand_int_32\n"
			"flw.ps f1, 800(x22)\n"
			"flw.ps f4, 32(x22)\n"
			"fsub.pi f13, f1, f4\n" 
			"LBL_SEQID_2_M0_WRITE_ID_4_HID_1:\n"
			"flem.ps m7, f1, f17\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x9, rand_ieee754_32\n"
			"flw.ps f12, 736(x9)\n"
			"flw.ps f0, 256(x9)\n"
			"flw.ps f24, 256(x9)\n"
			"fcmov.ps f9, f12, f0, f24\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f17, 64(x7)\n"
			"flw.ps f26, 352(x7)\n"
			"flw.ps f27, 224(x7)\n"
			"fmsub.s f0, f17, f26, f27, rup\n" 
			"LBL_SEQID_2_M0_WRITE_ID_5_HID_1:\n"
			"maskand m7, m4, m7\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"fbci.ps f23, 0x6c3ca\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f26, 320(x9)\n"
			"flw.ps f24, 384(x9)\n"
			"fmax.s f21, f26, f24\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f23, 32(x23)\n"
			"flw.ps f0, 992(x23)\n"
			"fsgnjx.ps f13, f23, f0\n" 
			"la x13, rand_int_32\n"
			"flw.ps f0, 256(x13)\n"
			"flw.ps f23, 384(x13)\n"
			"fmin.pi f9, f0, f23\n" 
			"LBL_SEQID_2_M0_WRITE_ID_6_HID_1:\n"
			"fsetm.pi m7, f2\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x9, rand_int_32\n"
			"flw.ps f23, 480(x9)\n"
			"flw.ps f2, 384(x9)\n"
			"fminu.pi f21, f23, f2\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f21, 672(x14)\n"
			"flw.ps f23, 544(x14)\n"
			"fmin.ps f0, f21, f23\n" 
			"la x13, rand_int_32\n"
			"flw.ps f21, 992(x13)\n"
			"flw.ps f15, 608(x13)\n"
			"fand.pi f28, f21, f15\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f23, 480(x22)\n"
			"flw.ps f2, 736(x22)\n"
			"fsgnj.s f2, f23, f2\n" 
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
			"la x12, rand_ieee754_32\n"
			"flw.ps f1, 608(x12)\n"
			"flw.ps f4, 256(x12)\n"
			"fsgnjn.ps f0, f1, f4\n" 
			"LBL_SEQID_3_FP_TRANS_ID_7_HID_1:\n"
			"la x7, rand_ieee754_32\n"
			"flw.ps f23, 320(x7)\n"
			"fexp.ps f23, f23\n" 
			"la x14, rand_int_32\n"
			"flw.ps f1, 384(x14)\n"
			"fpackrepb.pi f26, f1\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f26, 64(x14)\n"
			"ffrc.ps f0, f26\n" 
			"la x12, rand_int_32\n"
			"flw.ps f9, 160(x12)\n"
			"fandi.pi f2, f9, 0x185\n" 
			"maskpopc x7, m7\n" 
			"la x22, rand_int_32\n"
			"flw.ps f24, 352(x22)\n"
			"fcvt.ps.pw f2, f24, rtz\n" 
			"fbci.pi f26, 0x7721e\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f15, 96(x12)\n"
			"flw.ps f1, 128(x12)\n"
			"flw.ps f12, 352(x12)\n"
			"fnmadd.s f9, f15, f1, f12, rmm\n" 
			"la x11, rand_int_32\n"
			"flw.ps f0, 640(x11)\n"
			"fslli.pi f15, f0, 0x1\n" 
			"LBL_SEQID_3_FP_TRANS_ID_8_HID_1:\n"
			"la x23, rand_ieee754_32\n"
			"flw.ps f28, 256(x23)\n"
			"flog.ps f23, f28\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f2, 384(x11)\n"
			"flw.ps f24, 128(x11)\n"
			"fcmovm.ps f24, f2, f24\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f1, 608(x14)\n"
			"flw.ps f27, 896(x14)\n"
			"fsgnjx.s f15, f1, f27\n" 
			"la x9, rand_int_32\n"
			"flw.ps f2, 32(x9)\n"
			"flw.ps f26, 704(x9)\n"
			"fmulhu.pi f27, f2, f26\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f9, 448(x12)\n"
			"flw.ps f0, 480(x12)\n"
			"flw.ps f27, 928(x12)\n"
			"fcmov.ps f9, f9, f0, f27\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f23, 224(x13)\n"
			"fswizz.ps f0, f23, 0x1f\n" 
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
			"la x13, rand_ieee754_32\n"
			"flw.ps f7, 224(x13)\n"
			"flw.ps f28, 352(x13)\n"
			"fsgnj.ps f15, f7, f28\n" 
			"la x27, rand_int_32\n"
			"flw.ps f9, 544(x27)\n"
			"fsrai.pi f7, f9, 0x4\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f0, 96(x14)\n"
			"fclass.s x22, f0\n" 
			"la x27, rand_int_32\n"
			"flw.ps f28, 320(x27)\n"
			"fnot.pi f2, f28\n" 
			"fbci.ps f1, 0x18f3c\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f12, 928(x18)\n"
			"flw.ps f26, 192(x18)\n"
			"fle.s x27, f12, f26\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f24, 288(x23)\n"
			"ffrc.ps f1, f24\n" 
			"la x27, rand_int_32\n"
			"flw.ps f15, 864(x27)\n"
			"flw.ps f27, 992(x27)\n"
			"fminu.pi f15, f15, f27\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f15, 0(x9)\n"
			"flw.ps f24, 0(x9)\n"
			"fle.ps f26, f15, f24\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f2, 32(x13)\n"
			"flw.ps f15, 704(x13)\n"
			"fsub.ps f4, f2, f15, rmm\n" 
			"li x9, 0x20b9a97559ca9ea5\n"
			"fcvt.s.wu f12, x9, rup\n" 
			"la x23, rand_int_32\n"
			"flw.ps f9, 800(x23)\n"
			"flw.ps f7, 704(x23)\n"
			"fmaxu.pi f7, f9, f7\n" 
			"la x22, rand_int_32\n"
			"flw.ps f21, 288(x22)\n"
			"flw.ps f7, 32(x22)\n"
			"fadd.pi f2, f21, f7\n" 
			"LBL_SEQID_4_M0_WRITE_ID_9_HID_1:\n"
			"masknot m7, m4\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_4_M0_WRITE_ID_10_HID_1:\n"
			"fltm.ps m4, f28, f28\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x18, rand_int_32\n"
			"flw.ps f2, 0(x18)\n"
			"flw.ps f26, 128(x18)\n"
			"fsrl.pi f15, f2, f26\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 5\n" 
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
			"LBL_HPM_3_CORE_BRANCHES0_FAIL_HID_1:\n"          
			"csrr x5, mhpmcounter3\n"
			"sd x5, 32 (x31)\n"
			"LBL_HPM_4_CORE_DCACHE_MISSES0_FAIL_HID_1:\n"          
			"csrr x5, mhpmcounter4\n"
			"sd x5, 40 (x31)\n"
			"LBL_HPM_5_CORE_DCACHE_ACCESS0_FAIL_HID_1:\n"          
			"csrr x5, mhpmcounter5\n"
			"sd x5, 48 (x31)\n"
			"LBL_HPM_6_CORE_DCACHE_MISSES1_FAIL_HID_1:\n"          
			"csrr x5, mhpmcounter6\n"
			"sd x5, 56 (x31)\n"
			"LBL_HPM_7_NEIGH_ETL_REQ_TO_INTERMEDIATE_FIFO_FAIL_HID_1:\n"          
			"csrr x5, mhpmcounter7\n"
			"sd x5, 64 (x31)\n"
			"LBL_HPM_8_NEIGH_INTER_CORE_MSG_TX_FAIL_HID_1:\n"          
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
			"LBL_HPM_3_CORE_BRANCHES0_PASS_HID_1:\n"          
			"csrr x5, mhpmcounter3\n"
			"sd x5, 32 (x31)\n"
			"LBL_HPM_4_CORE_DCACHE_MISSES0_PASS_HID_1:\n"          
			"csrr x5, mhpmcounter4\n"
			"sd x5, 40 (x31)\n"
			"LBL_HPM_5_CORE_DCACHE_ACCESS0_PASS_HID_1:\n"          
			"csrr x5, mhpmcounter5\n"
			"sd x5, 48 (x31)\n"
			"LBL_HPM_6_CORE_DCACHE_MISSES1_PASS_HID_1:\n"          
			"csrr x5, mhpmcounter6\n"
			"sd x5, 56 (x31)\n"
			"LBL_HPM_7_NEIGH_ETL_REQ_TO_INTERMEDIATE_FIFO_PASS_HID_1:\n"          
			"csrr x5, mhpmcounter7\n"
			"sd x5, 64 (x31)\n"
			"LBL_HPM_8_NEIGH_INTER_CORE_MSG_TX_PASS_HID_1:\n"          
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
			"csrwi mhpmevent3, 17\n"   
			"csrwi mhpmevent4, 28\n"   
			"csrwi mhpmevent5, 19\n"   
			"csrwi mhpmevent6, 16\n"   
			"csrwi mhpmevent7, 4\n"   
			"csrwi mhpmevent8, 7\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f9,  288 (x5)\n"
			"flw.ps f14,  32 (x5)\n"
			"flw.ps f22,  224 (x5)\n"
			"flw.ps f26,  992 (x5)\n"
			"flw.ps f4,  384 (x5)\n"
			"flw.ps f3,  480 (x5)\n"
			"flw.ps f8,  448 (x5)\n"
			"flw.ps f31,  704 (x5)\n"
			"flw.ps f21,  608 (x5)\n"
			"flw.ps f13,  960 (x5)\n"
			"flw.ps f12,  672 (x5)\n"
			"flw.ps f18,  768 (x5)\n"
			"flw.ps f27,  736 (x5)\n"
			"flw.ps f19,  576 (x5)\n"
			"flw.ps f25,  64 (x5)\n"
			"flw.ps f29,  352 (x5)\n"
			"flw.ps f30,  256 (x5)\n"
			"flw.ps f5,  928 (x5)\n"
			"flw.ps f16,  160 (x5)\n"
			"flw.ps f28,  320 (x5)\n"
			"flw.ps f6,  512 (x5)\n"
			"flw.ps f24,  800 (x5)\n"
			"flw.ps f0,  0 (x5)\n"
			"flw.ps f15,  192 (x5)\n"
			"flw.ps f17,  832 (x5)\n"
			"flw.ps f2,  416 (x5)\n"
			"flw.ps f1,  128 (x5)\n"
			"flw.ps f23,  896 (x5)\n"
			"flw.ps f20,  864 (x5)\n"
			"flw.ps f7,  544 (x5)\n"
			"flw.ps f11,  96 (x5)\n"
			"flw.ps f10,  640 (x5)\n"
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
			"li x5, 0x5\n" 
			"sw x5, (x31)\n"
			"li x29, 0x5\n"
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
			"la x15, rand_ieee754_32\n"
			"flw.ps f16, 352(x15)\n"
			"flw.ps f28, 640(x15)\n"
			"fadd.ps f31, f16, f28, rup\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f22, 960(x15)\n"
			"flw.ps f9, 64(x15)\n"
			"fsgnj.s f5, f22, f9\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f9, 704(x20)\n"
			"flw.ps f13, 576(x20)\n"
			"feq.ps f8, f9, f13\n" 
			"fbci.pi f6, 0x4b063\n" 
			"li x5, 0x7efd\n"
			"csrw tensor_mask, x5\n"	
			"la x15, rand_ieee754_32\n"
			"flw.ps f18, 512(x15)\n"
			"flw.ps f23, 608(x15)\n"
			"flt.s x10, f18, f23\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_2:\n"
			"maskor m1, m4, m5\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_2:\n"
			"fltm.ps m0, f23, f22\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0xFF\n"
			"mova.x.m x13\n" 
			"LBL_SEQID_0_FP_TRANS_ID_2_HID_2:\n"
			"la x15, rand_ieee754_32\n"
			"flw.ps f1, 320(x15)\n"
			"flog.ps f30, f1\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f13, 672(x15)\n"
			"flw.ps f25, 576(x15)\n"
			"fmin.s f30, f13, f25\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f22, 992(x26)\n"
			"fsin.ps f1, f22\n" 
			"LBL_SEQID_0_M0_WRITE_ID_3_HID_2:\n"
			"masknot m0, m0\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0xFF\n"
			"li x5, 0xbe1cf2ffa5efabbd\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0x8f1a183f75877e0a\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"la x26, rand_ieee754_16\n"
			"flw.ps f8, 32(x26)\n"
			"fcvt.ps.f16 f30, f8\n" 
			"la x22, rand_int_32\n"
			"flw.ps f23, 288(x22)\n"
			"flw.ps f13, 0(x22)\n"
			"fsll.pi f8, f23, f13\n" 
			"maskpopcz x13, m4\n" 
			"LBL_SEQID_0_M0_WRITE_ID_4_HID_2:\n"
			"maskxor m5, m1, m4\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"csrw tensor_error, zero\n" 
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=0); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ITER_0_HID_2:\n"
			"li x5, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x0 << TENSOR_FMA16_B_NUM_COLS) | (0x6 << TENSOR_FMA16_A_NUM_ROWS) | (0x9 << TENSOR_FMA16_A_NUM_COLS) | (0xd << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0xef << TENSOR_FMA16_SCP_LOC_B) | (0xad << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x0 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x6, ((0x17 << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0x6 << TENSOR_QUANT_QUANT_ROW) | (0xa << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x2 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ITER_1_HID_2:\n"
			"li x5, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x0 << TENSOR_FMA32_B_NUM_COLS) | (0x2 << TENSOR_FMA32_A_NUM_ROWS) | (0xb << TENSOR_FMA32_A_NUM_COLS) | (0x3 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x46 << TENSOR_FMA32_SCP_LOC_B) | (0xdc << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x0 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_2\n"
			"csrw tensor_error, x0\n"
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_2\n"
			"addi x30, x30, 1\n"
			"la x10, rand_int_32\n"
			"flw.ps f29, 160(x10)\n"
			"fpackrepb.pi f18, f29\n" 
			"LBL_SEQID_1_FP_TRANS_ID_5_HID_2:\n"
			"la x18, rand_ieee754_32\n"
			"flw.ps f28, 224(x18)\n"
			"frcp.ps f16, f28\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f31, 64(x22)\n"
			"fclass.s x26, f31\n" 
			"LBL_SEQID_1_M0_WRITE_ID_6_HID_2:\n"
			"maskxor m1, m4, m4\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x27, rand_ieee754_32\n"
			"flw.ps f5, 768(x27)\n"
			"fround.ps f5, f5, rmm\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f18, 896(x15)\n"
			"flw.ps f31, 832(x15)\n"
			"feq.ps f31, f18, f31\n" 
			"li x5, 0xf926\n"
			"csrw tensor_mask, x5\n"	
			"la x13, rand_ieee754_32\n"
			"flw.ps f23, 672(x13)\n"
			"fclass.s x10, f23\n" 
			"la x13, rand_int_32\n"
			"flw.ps f25, 704(x13)\n"
			"flw.ps f28, 32(x13)\n"
			"flt.pi f13, f25, f28\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f9, 192(x18)\n"
			"flw.ps f29, 640(x18)\n"
			"fsub.ps f8, f9, f29, rup\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f28, 864(x19)\n"
			"fcvt.pw.ps f16, f28, rup\n" 
			"la x12, rand_int_32\n"
			"flw.ps f16, 672(x12)\n"
			"fnot.pi f22, f16\n" 
			"li x5, 0x1af8\n"
			"csrw tensor_mask, x5\n"	
			"la x10, rand_ieee754_32\n"
			"flw.ps f23, 672(x10)\n"
			"flw.ps f5, 128(x10)\n"
			"flw.ps f13, 672(x10)\n"
			"fnmsub.s f22, f23, f5, f13, rup\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f5, 864(x28)\n"
			"flw.ps f8, 832(x28)\n"
			"fsgnjn.s f9, f5, f8\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f9, 160(x26)\n"
			"flw.ps f18, 320(x26)\n"
			"fadd.ps f22, f9, f18, rdn\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f1, 320(x26)\n"
			"fclass.s x10, f1\n" 
			"la x19, rand_int_32\n"
			"flw.ps f16, 768(x19)\n"
			"fsat8.pi f22, f16\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"csrw tensor_error, zero\n" 
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=0); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"LBL_SEQID_1_TLD_XMA_STR_SEQ_ITER_0_HID_2:\n"
			"la x31, self_check_32\n"
			"li x5, 0xa0000000000004\n"
			"add x5, x5, x31\n"
			"li x6, 0x10000000000004\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x3 << TENSOR_FMA32_B_NUM_COLS) | (0x4 << TENSOR_FMA32_A_NUM_ROWS) | (0x4 << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x1 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1c << TENSOR_FMA32_SCP_LOC_B) | (0x5 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0x5\n"
			"bne x5, x31, LBL_FAIL_HID_2\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_2\n"
			"csrw tensor_error, x0\n"
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_2\n"
			"addi x30, x30, 1\n"
			"la x20, rand_int_32\n"
			"flw.ps f31, 256(x20)\n"
			"flw.ps f12, 0(x20)\n"
			"fdiv.pi f18, f31, f12\n" 
			"la x15, rand_int_32\n"
			"flw.ps f6, 288(x15)\n"
			"faddi.pi f18, f6, 0x162\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f13, 32(x12)\n"
			"flw.ps f28, 256(x12)\n"
			"fnmadd.ps f6, f13, f13, f28, rmm\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f5, 960(x28)\n"
			"flw.ps f16, 288(x28)\n"
			"flw.ps f18, 96(x28)\n"
			"fcmov.ps f13, f5, f16, f18\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f18, 576(x27)\n"
			"flw.ps f9, 288(x27)\n"
			"flw.ps f23, 768(x27)\n"
			"fnmadd.ps f13, f18, f9, f23, rmm\n" 
			"la x26, rand_int_32\n"
			"flw.ps f13, 544(x26)\n"
			"flw.ps f22, 320(x26)\n"
			"flt.pi f30, f13, f22\n" 
			"maskpopc x20, m4\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f18, 288(x19)\n"
			"flw.ps f29, 192(x19)\n"
			"fmul.ps f5, f18, f29, rtz\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f9, 320(x28)\n"
			"flw.ps f12, 672(x28)\n"
			"fdiv.s f25, f9, f12, rtz\n" 
			"la x18, rand_int_32\n"
			"flw.ps f23, 224(x18)\n"
			"fpackreph.pi f25, f23\n" 
			"la x28, rand_int_32\n"
			"flw.ps f22, 256(x28)\n"
			"fpackrepb.pi f6, f22\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f16, 288(x20)\n"
			"fsin.ps f30, f16\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f25, 832(x19)\n"
			"flw.ps f5, 416(x19)\n"
			"feq.s x26, f25, f5\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f6, 672(x20)\n"
			"flw.ps f18, 960(x20)\n"
			"fsgnj.s f22, f6, f18\n" 
			"la x13, rand_int_32\n"
			"flw.ps f5, 768(x13)\n"
			"fsatu8.pi f31, f5\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f29, 512(x18)\n"
			"fclass.s x19, f29\n" 
			"li x5, 0x8288\n"
			"csrw tensor_mask, x5\n"	
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_2_TLD_XMA_STR_SEQ_ITER_0_HID_2:\n"
			"la x31, self_check_16\n"
			"li x5, 0x80000000000007\n"
			"add x5, x5, x31\n"
			"li x6, 0x10000000000003\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x3 << TENSOR_FMA16_B_NUM_COLS) | (0x7 << TENSOR_FMA16_A_NUM_ROWS) | (0x3 << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x1 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1b << TENSOR_FMA16_SCP_LOC_B) | (0x4 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
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
			"LBL_SEQID_3_FP_TRANS_ID_7_HID_2:\n"
			"la x27, rand_ieee754_32\n"
			"flw.ps f28, 224(x27)\n"
			"fexp.ps f31, f28\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f16, 64(x18)\n"
			"flw.ps f25, 576(x18)\n"
			"fle.s x19, f16, f25\n" 
			"la x28, rand_int_32\n"
			"flw.ps f23, 800(x28)\n"
			"flw.ps f29, 704(x28)\n"
			"for.pi f13, f23, f29\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x10, rand_ieee754_32\n"
			"flw.ps f18, 608(x10)\n"
			"flw.ps f9, 736(x10)\n"
			"feq.s x27, f18, f9\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f8, 256(x13)\n"
			"fround.ps f1, f8, rne\n" 
			"fbci.pi f9, 0x4db1c\n" 
			"la x10, rand_int_32\n"
			"flw.ps f28, 224(x10)\n"
			"flw.ps f6, 320(x10)\n"
			"fsll.pi f16, f28, f6\n" 
			"li x13, 0x690aba4f8ccfe58d\n"
			"fcvt.s.wu f31, x13, rtz\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f29, 704(x19)\n"
			"flw.ps f12, 192(x19)\n"
			"fmax.s f22, f29, f12\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f23, 192(x28)\n"
			"flw.ps f22, 608(x28)\n"
			"flw.ps f8, 96(x28)\n"
			"fnmadd.s f18, f23, f22, f8, rne\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f1, 448(x27)\n"
			"flw.ps f23, 256(x27)\n"
			"fsgnj.s f30, f1, f23\n" 
			"fbci.pi f8, 0x2de36\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f1, 512(x20)\n"
			"ffrc.ps f29, f1\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f16, 544(x22)\n"
			"flw.ps f25, 384(x22)\n"
			"fsgnj.s f5, f16, f25\n" 
			"maskpopc x22, m0\n" 
			"mov.m.x m0, x0, 0xff\n"
			"la x26, rand_int_32\n"
			"flw.ps f8, 768(x26)\n"
			"flw.ps f5, 32(x26)\n"
			"fmin.pi f6, f8, f5\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_3_XMA_QNT_STR_SEQ_ID_906523_HID_2:\n"
			"li x6, ((0x10 << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0x3 << TENSOR_QUANT_QUANT_ROW) | (0x1c << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0xa << TENSOR_QUANT_TRANSF1) | (0x3 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_3_XMA_QNT_STR_SEQ_ID_682581_HID_2:\n"
			"li x6, ((0x11 << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0x8 << TENSOR_QUANT_QUANT_ROW) | (0x14 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x9 << TENSOR_QUANT_TRANSF8) | (0x9 << TENSOR_QUANT_TRANSF7) | (0x4 << TENSOR_QUANT_TRANSF6) | (0x3 << TENSOR_QUANT_TRANSF5) | (0x8 << TENSOR_QUANT_TRANSF4) | (0x3 << TENSOR_QUANT_TRANSF3) | (0x3 << TENSOR_QUANT_TRANSF2) | (0x5 << TENSOR_QUANT_TRANSF1) | (0x8 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_2\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_3_TLD_XMA_STR_SEQ_ITER_0_HID_2:\n"
			"la x31, self_check_8\n"
			"li x5, 0x80000000000008\n"
			"add x5, x5, x31\n"
			"li x6, 0x1000000000000d\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x0 << TENSOR_IMA_B_NUM_COLS) | (0x8 << TENSOR_IMA_A_NUM_ROWS) | (0xd << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x1 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1c << TENSOR_IMA_SCP_LOC_B) | (0x4 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fmvz.x.ps x31, f0, 0\n"
			"li x5, 0x38\n"
			"bne x5, x31, LBL_FAIL_HID_2\n"
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
			"la x15, rand_ieee754_32\n"
			"flw.ps f22, 256(x15)\n"
			"flw.ps f12, 416(x15)\n"
			"fsgnjn.ps f22, f22, f12\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f30, 160(x26)\n"
			"flw.ps f31, 320(x26)\n"
			"fmax.ps f31, f30, f31\n" 
			"li x28, 0x2d3c755a5626bc68\n"
			"fcvt.s.l f13, x28, rne\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f8, 576(x22)\n"
			"flw.ps f9, 800(x22)\n"
			"fsgnj.ps f5, f8, f9\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f5, 928(x15)\n"
			"flw.ps f1, 256(x15)\n"
			"fmax.s f23, f5, f1\n" 
			"la x10, rand_int_32\n"
			"flw.ps f6, 320(x10)\n"
			"fcvt.ps.pw f13, f6, rne\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f23, 448(x27)\n"
			"flw.ps f13, 128(x27)\n"
			"flt.s x12, f23, f13\n" 
			"la x10, rand_int_32\n"
			"flw.ps f5, 384(x10)\n"
			"fcvt.ps.pw f8, f5, rne\n" 
			"la x28, rand_int_32\n"
			"flw.ps f5, 544(x28)\n"
			"flw.ps f25, 32(x28)\n"
			"fmax.pi f12, f5, f25\n" 
			"LBL_SEQID_4_FP_TRANS_ID_8_HID_2:\n"
			"la x18, rand_ieee754_32\n"
			"flw.ps f31, 256(x18)\n"
			"flog.ps f16, f31\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f6, 96(x13)\n"
			"flw.ps f18, 352(x13)\n"
			"flt.s x19, f6, f18\n" 
			"LBL_SEQID_4_M0_WRITE_ID_9_HID_2:\n"
			"feqm.ps m4, f28, f25\n" 
			"li x5, CREDINC0\n"
			"ld x5, 0(x5)\n"
			"la x10, rand_int_32\n"
			"flw.ps f18, 448(x10)\n"
			"flw.ps f1, 768(x10)\n"
			"fminu.pi f1, f18, f1\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f1, 256(x12)\n"
			"flw.ps f13, 64(x12)\n"
			"fsub.s f29, f1, f13, rup\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f23, 704(x28)\n"
			"flw.ps f1, 992(x28)\n"
			"feq.ps f9, f23, f1\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f23, 704(x27)\n"
			"fsin.ps f8, f23\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_4_XMA_QNT_STR_SEQ_ITER_0_HID_2:\n"
			"li x5, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x3 << TENSOR_IMA_B_NUM_COLS) | (0xb << TENSOR_IMA_A_NUM_ROWS) | (0x7 << TENSOR_IMA_A_NUM_COLS) | (0x2 << TENSOR_IMA_OFFSET) | (0x0 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1e << TENSOR_IMA_SCP_LOC_B) | (0x36 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_2\n"
			"csrw tensor_error, x0\n"
			"li x5, 5\n" 
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
			"csrrci x0, mcache_control, 1\n"
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
			"csrwi mhpmevent3, 4\n"   
			"csrwi mhpmevent4, 8\n"   
			"csrwi mhpmevent5, 6\n"   
			"csrwi mhpmevent6, 9\n"   
			"csrwi mhpmevent7, 21\n"   
			"csrwi mhpmevent8, 12\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f10,  608 (x5)\n"
			"flw.ps f30,  448 (x5)\n"
			"flw.ps f19,  256 (x5)\n"
			"flw.ps f18,  64 (x5)\n"
			"flw.ps f20,  640 (x5)\n"
			"flw.ps f2,  800 (x5)\n"
			"flw.ps f24,  192 (x5)\n"
			"flw.ps f27,  672 (x5)\n"
			"flw.ps f11,  96 (x5)\n"
			"flw.ps f13,  224 (x5)\n"
			"flw.ps f31,  288 (x5)\n"
			"flw.ps f5,  832 (x5)\n"
			"flw.ps f17,  704 (x5)\n"
			"flw.ps f14,  160 (x5)\n"
			"flw.ps f8,  768 (x5)\n"
			"flw.ps f9,  352 (x5)\n"
			"flw.ps f16,  128 (x5)\n"
			"flw.ps f12,  416 (x5)\n"
			"flw.ps f22,  896 (x5)\n"
			"flw.ps f7,  864 (x5)\n"
			"flw.ps f1,  928 (x5)\n"
			"flw.ps f25,  0 (x5)\n"
			"flw.ps f21,  480 (x5)\n"
			"flw.ps f0,  32 (x5)\n"
			"flw.ps f6,  384 (x5)\n"
			"flw.ps f23,  736 (x5)\n"
			"flw.ps f15,  512 (x5)\n"
			"flw.ps f3,  320 (x5)\n"
			"flw.ps f28,  992 (x5)\n"
			"flw.ps f4,  576 (x5)\n"
			"flw.ps f29,  544 (x5)\n"
			"flw.ps f26,  960 (x5)\n"
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
			"li x5, 0x5\n" 
			"sw x5, (x31)\n"
			"li x29, 0x5\n"
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
			"li x28, 0xddcaf532f395f4f6\n"
			"fcvt.s.lu f23, x28, rup\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f8, 256(x16)\n"
			"flw.ps f3, 416(x16)\n"
			"fadd.ps f1, f8, f3, rup\n" 
			"la x16, rand_int_32\n"
			"flw.ps f9, 480(x16)\n"
			"fcvt.ps.pwu f4, f9, rtz\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f23, 96(x10)\n"
			"fcvt.wu.s x19, f23, rmm\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f3, 608(x27)\n"
			"flw.ps f23, 416(x27)\n"
			"fdiv.s f1, f3, f23, rmm\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f22, 768(x18)\n"
			"flw.ps f14, 128(x18)\n"
			"flw.ps f3, 384(x18)\n"
			"fmadd.ps f5, f22, f14, f3, rne\n" 
			"la x10, rand_int_32\n"
			"flw.ps f3, 832(x10)\n"
			"flw.ps f4, 96(x10)\n"
			"fand.pi f4, f3, f4\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f9, 384(x18)\n"
			"flw.ps f5, 160(x18)\n"
			"feq.s x20, f9, f5\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f3, 832(x20)\n"
			"flw.ps f9, 448(x20)\n"
			"feq.ps f3, f3, f9\n" 
			"la x27, rand_ieee754_16\n"
			"flw.ps f11, 672(x27)\n"
			"fcvt.ps.f16 f12, f11\n" 
			"la x25, rand_int_32\n"
			"flw.ps f6, 64(x25)\n"
			"flw.ps f9, 160(x25)\n"
			"fmulh.pi f4, f6, f9\n" 
			"LBL_SEQID_0_FP_TRANS_ID_0_HID_3:\n"
			"la x25, rand_ieee754_32\n"
			"flw.ps f26, 992(x25)\n"
			"frcp.ps f2, f26\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f5, 128(x18)\n"
			"flw.ps f30, 32(x18)\n"
			"fle.s x27, f5, f30\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_3:\n"
			"maskor m1, m1, m3\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x19, rand_int_32\n"
			"flw.ps f6, 704(x19)\n"
			"fpackreph.pi f14, f6\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f24, 192(x15)\n"
			"flw.ps f5, 416(x15)\n"
			"fsgnj.ps f26, f24, f5\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_3\n"
			"addi x30, x30, 1\n"
			"la x25, rand_ieee754_32\n"
			"flw.ps f9, 416(x25)\n"
			"flw.ps f8, 256(x25)\n"
			"fsgnj.ps f6, f9, f8\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f14, 768(x18)\n"
			"flw.ps f2, 576(x18)\n"
			"fmax.ps f26, f14, f2\n" 
			"LBL_SEQID_1_M0_WRITE_ID_2_HID_3:\n"
			"maskxor m3, m1, m7\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x9, rand_int_32\n"
			"flw.ps f12, 704(x9)\n"
			"flw.ps f11, 128(x9)\n"
			"fsra.pi f3, f12, f11\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f24, 992(x9)\n"
			"flw.ps f6, 384(x9)\n"
			"fsub.ps f26, f24, f6, rne\n" 
			"maskpopc x9, m3\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f30, 224(x10)\n"
			"fcvt.lu.s x18, f30, rmm\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f30, 64(x18)\n"
			"fcvt.wu.s x10, f30, rne\n" 
			"LBL_SEQID_1_FP_TRANS_ID_3_HID_3:\n"
			"la x16, rand_ieee754_32\n"
			"flw.ps f2, 384(x16)\n"
			"fexp.ps f12, f2\n" 
			"la x20, rand_int_32\n"
			"flw.ps f11, 32(x20)\n"
			"flw.ps f30, 640(x20)\n"
			"fmul.pi f23, f11, f30\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f3, 576(x19)\n"
			"flw.ps f14, 480(x19)\n"
			"fsgnjn.ps f5, f3, f14\n" 
			"LBL_SEQID_1_M0_WRITE_ID_4_HID_3:\n"
			"maskor m3, m0, m1\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x16, rand_ieee754_32\n"
			"flw.ps f26, 160(x16)\n"
			"flw.ps f5, 704(x16)\n"
			"flw.ps f12, 512(x16)\n"
			"fnmadd.s f5, f26, f5, f12, rmm\n" 
			"LBL_SEQID_1_FP_TRANS_ID_5_HID_3:\n"
			"la x28, rand_ieee754_32\n"
			"flw.ps f26, 224(x28)\n"
			"frcp.ps f12, f26\n" 
			"LBL_SEQID_1_M0_WRITE_ID_6_HID_3:\n"
			"maskor m7, m1, m7\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0x5620cbe0ba\n"
			"flw.ps f5, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x15, rand_int_32\n"
			"flw.ps f6, 672(x15)\n"
			"faddi.pi f4, f6, 0x1ff\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_3\n"
			"addi x30, x30, 1\n"
			"la x9, rand_int_32\n"
			"flw.ps f24, 960(x9)\n"
			"fpackrepb.pi f3, f24\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f11, 736(x25)\n"
			"flw.ps f9, 352(x25)\n"
			"flw.ps f6, 896(x25)\n"
			"fmsub.ps f22, f11, f9, f6, rmm\n" 
			"la x19, rand_int_32\n"
			"flw.ps f4, 352(x19)\n"
			"fsat8.pi f26, f4\n" 
			"maskpopc x15, m0\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f1, 480(x16)\n"
			"flw.ps f5, 768(x16)\n"
			"fle.ps f8, f1, f5\n" 
			"la x9, rand_int_32\n"
			"flw.ps f26, 352(x9)\n"
			"faddi.pi f14, f26, 0x16b\n" 
			"maskpopc x18, m3\n" 
			"la x10, rand_int_32\n"
			"flw.ps f2, 608(x10)\n"
			"fpackreph.pi f3, f2\n" 
			"la x20, rand_int_32\n"
			"flw.ps f3, 416(x20)\n"
			"flw.ps f9, 352(x20)\n"
			"fmax.pi f4, f3, f9\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f14, 768(x20)\n"
			"flw.ps f4, 128(x20)\n"
			"fsgnj.ps f26, f14, f4\n" 
			"la x25, rand_int_32\n"
			"flw.ps f9, 864(x25)\n"
			"fpackrepb.pi f2, f9\n" 
			"LBL_SEQID_2_M0_WRITE_ID_7_HID_3:\n"
			"feqm.ps m3, f5, f1\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0xf17ff0c8dd\n"
			"flw.ps f5, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x15, rand_int_32\n"
			"flw.ps f14, 384(x15)\n"
			"fpackreph.pi f22, f14\n" 
			"mova.x.m x10\n" 
			"la x18, rand_int_32\n"
			"flw.ps f26, 448(x18)\n"
			"flw.ps f12, 960(x18)\n"
			"fmulh.pi f12, f26, f12\n" 
			"maskpopc x27, m0\n" 
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
			"la x15, rand_ieee754_16\n"
			"flw.ps f12, 64(x15)\n"
			"fcvt.ps.f16 f23, f12\n" 
			"la x10, rand_int_32\n"
			"flw.ps f24, 832(x10)\n"
			"flw.ps f12, 288(x10)\n"
			"fminu.pi f6, f24, f12\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f2, 96(x27)\n"
			"flw.ps f14, 864(x27)\n"
			"fsgnj.ps f30, f2, f14\n" 
			"la x28, rand_int_32\n"
			"flw.ps f23, 32(x28)\n"
			"flw.ps f3, 320(x28)\n"
			"fsra.pi f11, f23, f3\n" 
			"fbci.ps f6, 0x55b50\n" 
			"la x20, rand_int_32\n"
			"flw.ps f14, 64(x20)\n"
			"fpackrepb.pi f30, f14\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f9, 608(x15)\n"
			"fcvt.wu.s x28, f9, rmm\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f14, 384(x28)\n"
			"flw.ps f5, 576(x28)\n"
			"fsgnjx.s f1, f14, f5\n" 
			"LBL_SEQID_3_M0_WRITE_ID_8_HID_3:\n"
			"fsetm.pi m3, f1\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x20, rand_int_32\n"
			"flw.ps f11, 320(x20)\n"
			"fslli.pi f2, f11, 0x4\n" 
			"LBL_SEQID_3_FP_TRANS_ID_9_HID_3:\n"
			"la x27, rand_ieee754_32\n"
			"flw.ps f11, 64(x27)\n"
			"frcp.ps f4, f11\n" 
			"la x28, rand_int_32\n"
			"flw.ps f22, 0(x28)\n"
			"flw.ps f1, 544(x28)\n"
			"fle.pi f8, f22, f1\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f6, 32(x18)\n"
			"ffrc.ps f4, f6\n" 
			"la x28, rand_int_32\n"
			"flw.ps f1, 480(x28)\n"
			"fcvt.ps.pwu f1, f1, rtz\n" 
			"la x25, rand_ieee754_16\n"
			"flw.ps f6, 576(x25)\n"
			"fcvt.ps.f16 f12, f6\n" 
			"LBL_SEQID_3_M0_WRITE_ID_10_HID_3:\n"
			"flem.ps m1, f4, f8\n" 
			"mov.m.x m0, x0, 0xFF\n"
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
			"la x15, rand_ieee754_32\n"
			"flw.ps f24, 224(x15)\n"
			"flw.ps f14, 448(x15)\n"
			"flt.s x9, f24, f14\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f5, 384(x10)\n"
			"flw.ps f30, 256(x10)\n"
			"fadd.s f26, f5, f30, rup\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f1, 928(x27)\n"
			"flw.ps f9, 64(x27)\n"
			"fle.s x27, f1, f9\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f1, 512(x27)\n"
			"fcvt.pwu.ps f14, f1, rtz\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f11, 864(x19)\n"
			"fround.ps f12, f11, rdn\n" 
			"la x16, rand_int_32\n"
			"flw.ps f26, 32(x16)\n"
			"flw.ps f22, 576(x16)\n"
			"fsll.pi f11, f26, f22\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f26, 608(x10)\n"
			"flw.ps f6, 480(x10)\n"
			"feq.ps f24, f26, f6\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f26, 32(x15)\n"
			"flw.ps f6, 832(x15)\n"
			"fmin.s f8, f26, f6\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f5, 448(x18)\n"
			"flw.ps f4, 320(x18)\n"
			"fle.ps f26, f5, f4\n" 
			"la x27, rand_int_32\n"
			"flw.ps f1, 64(x27)\n"
			"fsat8.pi f26, f1\n" 
			"LBL_SEQID_4_M0_WRITE_ID_11_HID_3:\n"
			"fltm.ps m1, f12, f24\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x27, rand_ieee754_32\n"
			"flw.ps f24, 608(x27)\n"
			"flw.ps f6, 896(x27)\n"
			"fsgnjn.ps f30, f24, f6\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f4, 0(x28)\n"
			"fcvt.w.s x25, f4, rup\n" 
			"la x10, rand_ieee754_16\n"
			"flw.ps f5, 256(x10)\n"
			"fcvt.ps.f16 f11, f5\n" 
			"li x28, 0x2599b1c8dc5d7027\n"
			"fcvt.s.lu f8, x28, rne\n" 
			"LBL_SEQID_4_M0_WRITE_ID_12_HID_3:\n"
			"fltm.pi m1, f14, f8\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"csrw tensor_error, zero\n" 
			"li x5, 5\n" 
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
			"csrwi mhpmevent3, 17\n"   
			"csrwi mhpmevent4, 28\n"   
			"csrwi mhpmevent5, 19\n"   
			"csrwi mhpmevent6, 16\n"   
			"csrwi mhpmevent7, 4\n"   
			"csrwi mhpmevent8, 7\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f5,  864 (x5)\n"
			"flw.ps f27,  928 (x5)\n"
			"flw.ps f11,  832 (x5)\n"
			"flw.ps f25,  288 (x5)\n"
			"flw.ps f8,  576 (x5)\n"
			"flw.ps f0,  416 (x5)\n"
			"flw.ps f24,  64 (x5)\n"
			"flw.ps f7,  608 (x5)\n"
			"flw.ps f15,  512 (x5)\n"
			"flw.ps f30,  0 (x5)\n"
			"flw.ps f23,  544 (x5)\n"
			"flw.ps f29,  320 (x5)\n"
			"flw.ps f21,  384 (x5)\n"
			"flw.ps f12,  224 (x5)\n"
			"flw.ps f9,  800 (x5)\n"
			"flw.ps f3,  768 (x5)\n"
			"flw.ps f13,  704 (x5)\n"
			"flw.ps f4,  448 (x5)\n"
			"flw.ps f1,  256 (x5)\n"
			"flw.ps f31,  672 (x5)\n"
			"flw.ps f28,  352 (x5)\n"
			"flw.ps f10,  192 (x5)\n"
			"flw.ps f14,  640 (x5)\n"
			"flw.ps f2,  32 (x5)\n"
			"flw.ps f22,  896 (x5)\n"
			"flw.ps f18,  128 (x5)\n"
			"flw.ps f16,  160 (x5)\n"
			"flw.ps f20,  96 (x5)\n"
			"flw.ps f19,  992 (x5)\n"
			"flw.ps f26,  736 (x5)\n"
			"flw.ps f17,  480 (x5)\n"
			"flw.ps f6,  960 (x5)\n"
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
			"li x5, 0x5\n" 
			"sw x5, (x31)\n"
			"li x29, 0x5\n"
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
			"la x19, rand_ieee754_32\n"
			"flw.ps f9, 32(x19)\n"
			"flw.ps f29, 960(x19)\n"
			"fsgnj.s f22, f9, f29\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f29, 544(x12)\n"
			"flw.ps f3, 288(x12)\n"
			"fmul.ps f9, f29, f3, rne\n" 
			"la x19, rand_int_32\n"
			"flw.ps f10, 736(x19)\n"
			"flw.ps f14, 928(x19)\n"
			"fminu.pi f22, f10, f14\n" 
			"la x22, rand_int_32\n"
			"flw.ps f1, 96(x22)\n"
			"fsatu8.pi f1, f1\n" 
			"la x14, rand_int_32\n"
			"flw.ps f24, 608(x14)\n"
			"fsatu8.pi f29, f24\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f1, 512(x14)\n"
			"flw.ps f7, 288(x14)\n"
			"flw.ps f18, 32(x14)\n"
			"fmsub.s f30, f1, f7, f18, rtz\n" 
			"la x14, rand_int_32\n"
			"flw.ps f22, 288(x14)\n"
			"fsatu8.pi f3, f22\n" 
			"li x5, 0xe80f\n"
			"csrw tensor_mask, x5\n"	
			"la x16, rand_ieee754_32\n"
			"flw.ps f23, 736(x16)\n"
			"flw.ps f14, 800(x16)\n"
			"fsgnj.ps f29, f23, f14\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f24, 160(x27)\n"
			"flw.ps f21, 32(x27)\n"
			"flw.ps f7, 480(x27)\n"
			"fnmsub.s f10, f24, f21, f7, rne\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f23, 864(x27)\n"
			"flw.ps f20, 608(x27)\n"
			"flw.ps f18, 352(x27)\n"
			"fnmsub.ps f21, f23, f20, f18, rup\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x12, rand_int_32\n"
			"flw.ps f1, 320(x12)\n"
			"fsat8.pi f2, f1\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f9, 960(x15)\n"
			"flw.ps f1, 320(x15)\n"
			"flw.ps f22, 288(x15)\n"
			"fmadd.s f29, f9, f1, f22, rne\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f21, 448(x19)\n"
			"flw.ps f14, 672(x19)\n"
			"flw.ps f16, 672(x19)\n"
			"fnmadd.ps f1, f21, f14, f16, rtz\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"LBL_SEQID_0_FP_TRANS_ID_0_HID_4:\n"
			"la x16, rand_ieee754_32\n"
			"flw.ps f1, 352(x16)\n"
			"fexp.ps f30, f1\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_4:\n"
			"mov.m.x m1, x27, 0x1f\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x22, rand_int_32\n"
			"flw.ps f10, 960(x22)\n"
			"flw.ps f16, 640(x22)\n"
			"feq.pi f16, f10, f16\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ID_560291_HID_4:\n"
			"li x6, ((0x19 << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0xd << TENSOR_QUANT_QUANT_ROW) | (0x14 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x0 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ID_753835_HID_4:\n"
			"li x5, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x0 << TENSOR_FMA16_B_NUM_COLS) | (0x3 << TENSOR_FMA16_A_NUM_ROWS) | (0x1 << TENSOR_FMA16_A_NUM_COLS) | (0xa << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x2d << TENSOR_FMA16_SCP_LOC_B) | (0xc1 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x6, ((0xe << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0x3 << TENSOR_QUANT_QUANT_ROW) | (0x3c << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x0 << TENSOR_QUANT_TRANSF1) | (0x0 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 7\n"
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_4\n"
			"csrw tensor_error, x0\n"
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=0); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ITER_0_HID_4:\n"
			"li x5, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x3 << TENSOR_IMA_B_NUM_COLS) | (0x7 << TENSOR_IMA_A_NUM_ROWS) | (0xd << TENSOR_IMA_A_NUM_COLS) | (0xd << TENSOR_IMA_OFFSET) | (0x0 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x67 << TENSOR_IMA_SCP_LOC_B) | (0x13 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x0 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_4\n"
			"csrw tensor_error, x0\n"
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_4\n"
			"addi x30, x30, 1\n"
			"la x14, rand_ieee754_32\n"
			"flw.ps f3, 256(x14)\n"
			"flw.ps f21, 96(x14)\n"
			"fsub.ps f3, f3, f21, rne\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x16, rand_ieee754_32\n"
			"flw.ps f20, 384(x16)\n"
			"fcvt.f16.ps f24, f20\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f10, 864(x27)\n"
			"flw.ps f18, 672(x27)\n"
			"fsgnjn.ps f18, f10, f18\n" 
			"la x9, rand_int_32\n"
			"flw.ps f20, 768(x9)\n"
			"flw.ps f7, 192(x9)\n"
			"fmax.pi f18, f20, f7\n" 
			"la x12, rand_ieee754_16\n"
			"flw.ps f18, 96(x12)\n"
			"fcvt.ps.f16 f20, f18\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f9, 0(x9)\n"
			"flw.ps f14, 256(x9)\n"
			"fsgnjx.s f9, f9, f14\n" 
			"la x16, rand_int_32\n"
			"flw.ps f21, 512(x16)\n"
			"flw.ps f23, 32(x16)\n"
			"fsrl.pi f9, f21, f23\n" 
			"LBL_SEQID_1_M0_WRITE_ID_2_HID_4:\n"
			"mov.m.x m6, x12, 0x26\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_1_M0_WRITE_ID_3_HID_4:\n"
			"mov.m.x m6, x13, 0x16\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x22, rand_ieee754_32\n"
			"flw.ps f10, 896(x22)\n"
			"flw.ps f9, 832(x22)\n"
			"fsub.ps f1, f10, f9, rdn\n" 
			"maskpopcz x19, m2\n" 
			"la x10, rand_int_32\n"
			"flw.ps f22, 160(x10)\n"
			"flw.ps f29, 0(x10)\n"
			"fdiv.pi f3, f22, f29\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f23, 640(x9)\n"
			"frsq.ps f30, f23\n" 
			"li x5, 0xcb250752302e9b4e\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0x14266d9d697cbd4d\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"li x14, 0x6c125456521fc552\n"
			"fcvt.s.l f30, x14, rdn\n" 
			"fbci.ps f3, 0x1e1d5\n" 
			"la x12, rand_int_32\n"
			"flw.ps f9, 608(x12)\n"
			"flw.ps f22, 384(x12)\n"
			"fmax.pi f21, f9, f22\n" 
			"csrw tensor_error, zero\n" 
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=0); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ITER_0_HID_4:\n"
			"li x5, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x1 << TENSOR_IMA_B_NUM_COLS) | (0x6 << TENSOR_IMA_A_NUM_ROWS) | (0xc << TENSOR_IMA_A_NUM_COLS) | (0xb << TENSOR_IMA_OFFSET) | (0x0 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0xbd << TENSOR_IMA_SCP_LOC_B) | (0xa9 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x0 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ITER_1_HID_4:\n"
			"li x5, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x2 << TENSOR_FMA32_B_NUM_COLS) | (0x2 << TENSOR_FMA32_A_NUM_ROWS) | (0x1 << TENSOR_FMA32_A_NUM_COLS) | (0xd << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x67 << TENSOR_FMA32_SCP_LOC_B) | (0x3b << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x0 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_4\n"
			"csrw tensor_error, x0\n"
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_4\n"
			"addi x30, x30, 1\n"
			"la x27, rand_ieee754_32\n"
			"flw.ps f30, 672(x27)\n"
			"flw.ps f21, 896(x27)\n"
			"flw.ps f22, 192(x27)\n"
			"fnmsub.s f22, f30, f21, f22, rne\n" 
			"la x13, rand_int_32\n"
			"flw.ps f10, 416(x13)\n"
			"fpackrepb.pi f3, f10\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f9, 32(x28)\n"
			"fcvt.wu.s x15, f9, rup\n" 
			"la x12, rand_int_32\n"
			"flw.ps f3, 864(x12)\n"
			"fcvt.ps.pw f1, f3, rup\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f22, 544(x10)\n"
			"fsgnj.s f30, f22, f22\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f22, 96(x16)\n"
			"fadd.ps f18, f22, f22, rdn\n" 
			"la x22, rand_ieee754_16\n"
			"flw.ps f7, 224(x22)\n"
			"fcvt.ps.f16 f22, f7\n" 
			"li x5, 0xff3a0c3d2fabed74\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0x6bba9b80d6c7437b\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"la x15, rand_ieee754_32\n"
			"flw.ps f3, 160(x15)\n"
			"flw.ps f18, 0(x15)\n"
			"flw.ps f23, 224(x15)\n"
			"fmsub.ps f9, f3, f18, f23, rup\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f21, 544(x13)\n"
			"flw.ps f14, 0(x13)\n"
			"fsgnj.s f16, f21, f14\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f1, 192(x14)\n"
			"flw.ps f30, 928(x14)\n"
			"fmul.ps f7, f1, f30, rtz\n" 
			"LBL_SEQID_2_FP_TRANS_ID_4_HID_4:\n"
			"la x28, rand_ieee754_32\n"
			"flw.ps f7, 320(x28)\n"
			"fexp.ps f24, f7\n" 
			"li x5, 0xe2cb\n"
			"csrw tensor_mask, x5\n"	
			"la x28, rand_ieee754_32\n"
			"flw.ps f18, 224(x28)\n"
			"flw.ps f29, 448(x28)\n"
			"flw.ps f21, 576(x28)\n"
			"fnmsub.s f29, f18, f29, f21, rmm\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f20, 640(x9)\n"
			"flw.ps f21, 896(x9)\n"
			"fmin.s f10, f20, f21\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f9, 160(x27)\n"
			"ffrc.ps f22, f9\n" 
			"la x15, rand_int_32\n"
			"flw.ps f29, 800(x15)\n"
			"fsatu8.pi f16, f29\n" 
			"la x10, rand_int_32\n"
			"flw.ps f18, 704(x10)\n"
			"flw.ps f7, 256(x10)\n"
			"flt.pi f10, f18, f7\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_2_XMA_QNT_STR_SEQ_ID_575558_HID_4:\n"
			"li x5, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x2 << TENSOR_FMA16_B_NUM_COLS) | (0xb << TENSOR_FMA16_A_NUM_ROWS) | (0xb << TENSOR_FMA16_A_NUM_COLS) | (0xe << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x3d << TENSOR_FMA16_SCP_LOC_B) | (0xf1 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x0 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_4\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_2_XMA_QNT_STR_SEQ_ITER_0_HID_4:\n"
			"li x5, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x0 << TENSOR_IMA_B_NUM_COLS) | (0x8 << TENSOR_IMA_A_NUM_ROWS) | (0x4 << TENSOR_IMA_A_NUM_COLS) | (0x9 << TENSOR_IMA_OFFSET) | (0x0 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x38 << TENSOR_IMA_SCP_LOC_B) | (0x60 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
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
			"li x16, 0xac25fff7d7462a1d\n"
			"fcvt.s.w f29, x16, rup\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f9, 672(x15)\n"
			"flw.ps f3, 704(x15)\n"
			"flw.ps f21, 160(x15)\n"
			"fmadd.ps f18, f9, f3, f21, rup\n" 
			"la x22, rand_int_32\n"
			"flw.ps f22, 736(x22)\n"
			"fsatu8.pi f20, f22\n" 
			"la x15, rand_int_32\n"
			"flw.ps f22, 480(x15)\n"
			"flw.ps f23, 64(x15)\n"
			"fminu.pi f20, f22, f23\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f23, 96(x15)\n"
			"flw.ps f1, 384(x15)\n"
			"feq.ps f23, f23, f1\n" 
			"la x27, rand_int_32\n"
			"flw.ps f2, 32(x27)\n"
			"flw.ps f16, 64(x27)\n"
			"fmulhu.pi f10, f2, f16\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f1, 96(x22)\n"
			"fcvt.pwu.ps f29, f1, rtz\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f10, 544(x22)\n"
			"fsqrt.ps f21, f10\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x28, rand_ieee754_32\n"
			"flw.ps f29, 64(x28)\n"
			"fcvt.lu.s x13, f29, rup\n" 
			"la x19, rand_int_32\n"
			"flw.ps f29, 992(x19)\n"
			"fpackrepb.pi f23, f29\n" 
			"LBL_SEQID_3_M0_WRITE_ID_5_HID_4:\n"
			"flem.ps m6, f29, f7\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0x94e2fe3042\n"
			"flw.ps f5, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x12, rand_int_32\n"
			"flw.ps f24, 608(x12)\n"
			"flw.ps f14, 736(x12)\n"
			"fadd.pi f21, f24, f14\n" 
			"li x9, 0x68080360ed915c2c\n"
			"fcvt.s.w f14, x9, rup\n" 
			"LBL_SEQID_3_M0_WRITE_ID_6_HID_4:\n"
			"fsetm.pi m3, f9\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x10, rand_ieee754_32\n"
			"flw.ps f21, 256(x10)\n"
			"flw.ps f10, 512(x10)\n"
			"fmul.s f2, f21, f10, rtz\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f7, 992(x22)\n"
			"fswizz.ps f3, f7, 0x12\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_3_XMA_QNT_STR_SEQ_ITER_0_HID_4:\n"
			"li x6, ((0x16 << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0x7 << TENSOR_QUANT_QUANT_ROW) | (0x26 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x6 << TENSOR_QUANT_TRANSF4) | (0x7 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_3_XMA_QNT_STR_SEQ_ITER_1_HID_4:\n"
			"li x6, ((0x11 << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0x4 << TENSOR_QUANT_QUANT_ROW) | (0x4 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_3_XMA_QNT_STR_SEQ_ITER_2_HID_4:\n"
			"li x6, ((0x15 << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0x1 << TENSOR_QUANT_QUANT_ROW) | (0x3c << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x2 << TENSOR_QUANT_TRANSF4) | (0x7 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 10\n"
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
			"la x27, rand_ieee754_32\n"
			"flw.ps f24, 320(x27)\n"
			"flw.ps f16, 736(x27)\n"
			"flw.ps f7, 864(x27)\n"
			"fnmsub.s f30, f24, f16, f7, rmm\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f14, 608(x19)\n"
			"flw.ps f9, 800(x19)\n"
			"fnmsub.s f16, f14, f9, f14, rdn\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f23, 928(x15)\n"
			"flw.ps f29, 704(x15)\n"
			"fle.s x19, f23, f29\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f1, 544(x16)\n"
			"fsin.ps f22, f1\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f3, 992(x27)\n"
			"flw.ps f21, 832(x27)\n"
			"flw.ps f10, 0(x27)\n"
			"fmsub.ps f10, f3, f21, f10, rdn\n" 
			"maskpopc x13, m2\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f24, 256(x13)\n"
			"flw.ps f14, 608(x13)\n"
			"flw.ps f1, 64(x13)\n"
			"fcmov.ps f23, f24, f14, f1\n" 
			"mova.x.m x19\n" 
			"li x12, 0x41cfc1da76d52742\n"
			"fcvt.s.w f24, x12, rtz\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x27, rand_ieee754_32\n"
			"flw.ps f30, 224(x27)\n"
			"flw.ps f21, 672(x27)\n"
			"fsgnj.s f30, f30, f21\n" 
			"la x14, rand_int_32\n"
			"flw.ps f7, 64(x14)\n"
			"flw.ps f1, 960(x14)\n"
			"fadd.pi f9, f7, f1\n" 
			"la x14, rand_int_32\n"
			"flw.ps f29, 896(x14)\n"
			"flw.ps f23, 128(x14)\n"
			"fxor.pi f7, f29, f23\n" 
			"li x5, 0xec1c\n"
			"csrw tensor_mask, x5\n"	
			"la x10, rand_ieee754_32\n"
			"flw.ps f23, 960(x10)\n"
			"flw.ps f10, 64(x10)\n"
			"fadd.s f21, f23, f10, rne\n" 
			"la x14, rand_int_32\n"
			"flw.ps f23, 928(x14)\n"
			"fsrli.pi f7, f23, 0xd\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f3, 960(x10)\n"
			"flw.ps f30, 736(x10)\n"
			"flw.ps f23, 128(x10)\n"
			"fmadd.s f22, f3, f30, f23, rdn\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f2, 384(x10)\n"
			"flw.ps f30, 480(x10)\n"
			"fsgnj.s f22, f2, f30\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_4_XMA_QNT_STR_SEQ_ID_711741_HID_4:\n"
			"li x6, ((0x16 << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0x3 << TENSOR_QUANT_QUANT_ROW) | (0x2e << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x8 << TENSOR_QUANT_TRANSF1) | (0x3 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_4\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_4_TLD_XMA_STR_SEQ_ITER_0_HID_4:\n"
			"la x31, self_check_16\n"
			"li x5, 0xe000000000000d\n"
			"add x5, x5, x31\n"
			"li x6, 0x360000000000003\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x3 << TENSOR_FMA16_B_NUM_COLS) | (0xd << TENSOR_FMA16_A_NUM_ROWS) | (0x3 << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1b << TENSOR_FMA16_SCP_LOC_B) | (0x7 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x16 << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0xd << TENSOR_QUANT_QUANT_ROW) | (0x3f << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x2 << TENSOR_QUANT_TRANSF4) | (0x6 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_4\n"
			"csrw tensor_error, x0\n"
			"li x5, 5\n" 
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
			"csrrci x0, mcache_control, 1\n"
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
			"csrwi mhpmevent3, 4\n"   
			"csrwi mhpmevent4, 8\n"   
			"csrwi mhpmevent5, 6\n"   
			"csrwi mhpmevent6, 9\n"   
			"csrwi mhpmevent7, 21\n"   
			"csrwi mhpmevent8, 12\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f20,  416 (x5)\n"
			"flw.ps f29,  928 (x5)\n"
			"flw.ps f25,  992 (x5)\n"
			"flw.ps f14,  704 (x5)\n"
			"flw.ps f8,  320 (x5)\n"
			"flw.ps f17,  640 (x5)\n"
			"flw.ps f28,  576 (x5)\n"
			"flw.ps f18,  224 (x5)\n"
			"flw.ps f31,  96 (x5)\n"
			"flw.ps f1,  384 (x5)\n"
			"flw.ps f22,  960 (x5)\n"
			"flw.ps f12,  736 (x5)\n"
			"flw.ps f13,  160 (x5)\n"
			"flw.ps f15,  864 (x5)\n"
			"flw.ps f23,  608 (x5)\n"
			"flw.ps f16,  544 (x5)\n"
			"flw.ps f19,  512 (x5)\n"
			"flw.ps f26,  352 (x5)\n"
			"flw.ps f0,  448 (x5)\n"
			"flw.ps f2,  0 (x5)\n"
			"flw.ps f7,  256 (x5)\n"
			"flw.ps f6,  288 (x5)\n"
			"flw.ps f5,  896 (x5)\n"
			"flw.ps f3,  768 (x5)\n"
			"flw.ps f21,  64 (x5)\n"
			"flw.ps f9,  32 (x5)\n"
			"flw.ps f24,  672 (x5)\n"
			"flw.ps f30,  128 (x5)\n"
			"flw.ps f11,  832 (x5)\n"
			"flw.ps f10,  192 (x5)\n"
			"flw.ps f27,  800 (x5)\n"
			"flw.ps f4,  480 (x5)\n"
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
			"li x5, 0x5\n" 
			"sw x5, (x31)\n"
			"li x29, 0x5\n"
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
			"la x12, rand_int_32\n"
			"flw.ps f9, 0(x12)\n"
			"fsat8.pi f2, f9\n" 
			"la x11, rand_int_32\n"
			"flw.ps f6, 96(x11)\n"
			"fpackreph.pi f15, f6\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_5:\n"
			"maskand m2, m1, m2\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"li x22, 0x9c684979817535c\n"
			"fcvt.s.lu f17, x22, rne\n" 
			"LBL_SEQID_0_FP_TRANS_ID_1_HID_5:\n"
			"la x15, rand_ieee754_32\n"
			"flw.ps f30, 224(x15)\n"
			"flog.ps f16, f30\n" 
			"la x11, rand_int_32\n"
			"flw.ps f24, 288(x11)\n"
			"flw.ps f3, 384(x11)\n"
			"fmulh.pi f9, f24, f3\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f31, 704(x6)\n"
			"flw.ps f28, 576(x6)\n"
			"fle.s x27, f31, f28\n" 
			"LBL_SEQID_0_M0_WRITE_ID_2_HID_5:\n"
			"masknot m4, m4\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x17, rand_ieee754_32\n"
			"flw.ps f3, 736(x17)\n"
			"flw.ps f1, 192(x17)\n"
			"fsgnjx.s f24, f3, f1\n" 
			"la x7, rand_int_32\n"
			"flw.ps f3, 32(x7)\n"
			"flw.ps f15, 0(x7)\n"
			"fmul.pi f27, f3, f15\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f28, 224(x7)\n"
			"ffrc.ps f30, f28\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f28, 256(x11)\n"
			"fsqrt.ps f10, f28\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f28, 608(x27)\n"
			"flw.ps f2, 192(x27)\n"
			"fadd.s f1, f28, f2, rup\n" 
			"LBL_SEQID_0_FP_TRANS_ID_3_HID_5:\n"
			"la x26, rand_ieee754_32\n"
			"flw.ps f1, 736(x26)\n"
			"frcp.ps f30, f1\n" 
			"la x11, rand_int_32\n"
			"flw.ps f30, 448(x11)\n"
			"flw.ps f6, 576(x11)\n"
			"fsll.pi f24, f30, f6\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f28, 640(x6)\n"
			"flw.ps f0, 224(x6)\n"
			"fsub.s f10, f28, f0, rne\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_5\n"
			"addi x30, x30, 1\n"
			"la x6, rand_ieee754_32\n"
			"flw.ps f10, 320(x6)\n"
			"flw.ps f0, 608(x6)\n"
			"flw.ps f6, 704(x6)\n"
			"fnmsub.ps f17, f10, f0, f6, rtz\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f6, 64(x27)\n"
			"flw.ps f4, 64(x27)\n"
			"fle.s x7, f6, f4\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f1, 672(x15)\n"
			"flw.ps f28, 544(x15)\n"
			"fmin.s f0, f1, f28\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f31, 576(x11)\n"
			"flw.ps f9, 64(x11)\n"
			"fmin.s f6, f31, f9\n" 
			"la x26, rand_int_32\n"
			"flw.ps f0, 544(x26)\n"
			"flw.ps f2, 320(x26)\n"
			"fmulhu.pi f28, f0, f2\n" 
			"la x26, rand_int_32\n"
			"flw.ps f27, 928(x26)\n"
			"flw.ps f9, 672(x26)\n"
			"fmulhu.pi f28, f27, f9\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f28, 96(x11)\n"
			"flw.ps f30, 128(x11)\n"
			"fadd.ps f17, f28, f30, rup\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f27, 960(x13)\n"
			"fclass.s x26, f27\n" 
			"la x7, rand_int_32\n"
			"flw.ps f1, 96(x7)\n"
			"flw.ps f6, 416(x7)\n"
			"fsll.pi f10, f1, f6\n" 
			"LBL_SEQID_1_M0_WRITE_ID_4_HID_5:\n"
			"maskor m2, m4, m6\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x7, rand_int_32\n"
			"flw.ps f16, 608(x7)\n"
			"fcvt.ps.pw f30, f16, rup\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f4, 192(x13)\n"
			"fswizz.ps f0, f4, 0x14\n" 
			"la x13, rand_int_32\n"
			"flw.ps f0, 128(x13)\n"
			"fsatu8.pi f27, f0\n" 
			"LBL_SEQID_1_M0_WRITE_ID_5_HID_5:\n"
			"maskxor m1, m6, m6\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x11, rand_ieee754_32\n"
			"flw.ps f3, 224(x11)\n"
			"flw.ps f16, 960(x11)\n"
			"fmin.ps f0, f3, f16\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f27, 864(x11)\n"
			"frsq.ps f9, f27\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_5\n"
			"addi x30, x30, 1\n"
			"la x27, rand_int_32\n"
			"flw.ps f3, 672(x27)\n"
			"flw.ps f24, 416(x27)\n"
			"fle.pi f9, f3, f24\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f10, 224(x7)\n"
			"flw.ps f4, 64(x7)\n"
			"fcmovm.ps f0, f10, f4\n" 
			"LBL_SEQID_2_M0_WRITE_ID_6_HID_5:\n"
			"fltm.pi m6, f31, f28\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x26, rand_ieee754_32\n"
			"flw.ps f31, 896(x26)\n"
			"flw.ps f24, 896(x26)\n"
			"fle.ps f10, f31, f24\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f6, 704(x6)\n"
			"fcvt.pwu.ps f0, f6, rne\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f15, 576(x11)\n"
			"flw.ps f3, 704(x11)\n"
			"feq.ps f30, f15, f3\n" 
			"la x11, rand_ieee754_16\n"
			"flw.ps f27, 192(x11)\n"
			"fcvt.ps.f16 f6, f27\n" 
			"la x27, rand_int_32\n"
			"flw.ps f9, 768(x27)\n"
			"flw.ps f17, 128(x27)\n"
			"fltu.pi f30, f9, f17\n" 
			"li x7, 0xe3c25779b5533eb\n"
			"fcvt.s.lu f2, x7, rmm\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f9, 864(x26)\n"
			"fadd.ps f1, f9, f9, rne\n" 
			"la x13, rand_int_32\n"
			"flw.ps f6, 448(x13)\n"
			"fsatu8.pi f17, f6\n" 
			"la x22, rand_int_32\n"
			"flw.ps f31, 128(x22)\n"
			"fcvt.ps.pwu f17, f31, rmm\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f27, 0(x7)\n"
			"ffrc.ps f10, f27\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f31, 960(x26)\n"
			"fcvt.wu.s x15, f31, rtz\n" 
			"la x7, rand_int_32\n"
			"flw.ps f4, 640(x7)\n"
			"flw.ps f6, 32(x7)\n"
			"fxor.pi f30, f4, f6\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f6, 544(x22)\n"
			"flw.ps f24, 224(x22)\n"
			"fsub.s f0, f6, f24, rtz\n" 
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
			"la x13, rand_int_32\n"
			"flw.ps f4, 512(x13)\n"
			"flw.ps f30, 992(x13)\n"
			"fmin.pi f4, f4, f30\n" 
			"la x12, rand_int_32\n"
			"flw.ps f24, 800(x12)\n"
			"fsrl.pi f15, f24, f24\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f4, 320(x11)\n"
			"flw.ps f15, 608(x11)\n"
			"fsgnjx.ps f28, f4, f15\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f27, 672(x11)\n"
			"flw.ps f17, 416(x11)\n"
			"flw.ps f30, 96(x11)\n"
			"fmsub.s f2, f27, f17, f30, rdn\n" 
			"maskpopc x7, m2\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f30, 128(x17)\n"
			"flw.ps f27, 256(x17)\n"
			"flw.ps f24, 832(x17)\n"
			"fnmsub.ps f17, f30, f27, f24, rdn\n" 
			"la x27, rand_int_32\n"
			"flw.ps f3, 416(x27)\n"
			"flw.ps f2, 800(x27)\n"
			"fsra.pi f9, f3, f2\n" 
			"fbci.ps f9, 0x35816\n" 
			"fbci.pi f24, 0x2b55f\n" 
			"la x7, rand_int_32\n"
			"flw.ps f28, 736(x7)\n"
			"flw.ps f0, 224(x7)\n"
			"fsra.pi f28, f28, f0\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f27, 448(x6)\n"
			"fcvt.wu.s x27, f27, rtz\n" 
			"la x7, rand_int_32\n"
			"flw.ps f6, 640(x7)\n"
			"fsat8.pi f17, f6\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f1, 384(x7)\n"
			"flw.ps f15, 448(x7)\n"
			"fsub.ps f9, f1, f15, rmm\n" 
			"LBL_SEQID_3_M0_WRITE_ID_7_HID_5:\n"
			"fsetm.pi m2, f0\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x6, rand_ieee754_32\n"
			"flw.ps f2, 768(x6)\n"
			"fcvt.pwu.ps f24, f2, rdn\n" 
			"la x13, rand_int_32\n"
			"flw.ps f2, 256(x13)\n"
			"fnot.pi f6, f2\n" 
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
			"la x12, rand_ieee754_32\n"
			"flw.ps f6, 640(x12)\n"
			"flw.ps f17, 384(x12)\n"
			"fmadd.ps f2, f6, f6, f17, rup\n" 
			"LBL_SEQID_4_M0_WRITE_ID_8_HID_5:\n"
			"maskand m6, m1, m6\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x27, rand_ieee754_32\n"
			"flw.ps f16, 352(x27)\n"
			"fcvt.w.s x7, f16, rne\n" 
			"la x7, rand_int_32\n"
			"flw.ps f6, 288(x7)\n"
			"fsrai.pi f9, f6, 0xd\n" 
			"la x11, rand_int_32\n"
			"flw.ps f17, 448(x11)\n"
			"fsatu8.pi f15, f17\n" 
			"LBL_SEQID_4_M0_WRITE_ID_9_HID_5:\n"
			"masknot m2, m2\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x15, rand_ieee754_32\n"
			"flw.ps f28, 0(x15)\n"
			"flw.ps f3, 32(x15)\n"
			"fsgnj.ps f6, f28, f3\n" 
			"LBL_SEQID_4_FP_TRANS_ID_10_HID_5:\n"
			"la x13, rand_ieee754_32\n"
			"flw.ps f15, 672(x13)\n"
			"flog.ps f27, f15\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f31, 32(x7)\n"
			"flw.ps f16, 96(x7)\n"
			"fmin.s f28, f31, f16\n" 
			"LBL_SEQID_4_M0_WRITE_ID_11_HID_5:\n"
			"flem.ps m2, f3, f0\n" 
			"li x5, CREDINC2\n"
			"ld x5, 0(x5)\n"
			"la x15, rand_int_32\n"
			"flw.ps f17, 544(x15)\n"
			"fsrai.pi f2, f17, 0x8\n" 
			"LBL_SEQID_4_M0_WRITE_ID_12_HID_5:\n"
			"fltm.ps m2, f6, f30\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x12, rand_ieee754_32\n"
			"flw.ps f24, 96(x12)\n"
			"flw.ps f2, 320(x12)\n"
			"fsgnjn.ps f0, f24, f2\n" 
			"LBL_SEQID_4_M0_WRITE_ID_13_HID_5:\n"
			"maskxor m1, m2, m4\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x6, rand_int_32\n"
			"flw.ps f15, 544(x6)\n"
			"fsrai.pi f15, f15, 0xc\n" 
			"la x7, rand_int_32\n"
			"flw.ps f0, 512(x7)\n"
			"flw.ps f2, 32(x7)\n"
			"fltu.pi f3, f0, f2\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 5\n" 
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
			"csrwi mhpmevent3, 17\n"   
			"csrwi mhpmevent4, 28\n"   
			"csrwi mhpmevent5, 19\n"   
			"csrwi mhpmevent6, 16\n"   
			"csrwi mhpmevent7, 4\n"   
			"csrwi mhpmevent8, 7\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f2,  736 (x5)\n"
			"flw.ps f11,  96 (x5)\n"
			"flw.ps f25,  896 (x5)\n"
			"flw.ps f20,  768 (x5)\n"
			"flw.ps f8,  928 (x5)\n"
			"flw.ps f27,  512 (x5)\n"
			"flw.ps f14,  192 (x5)\n"
			"flw.ps f30,  0 (x5)\n"
			"flw.ps f15,  352 (x5)\n"
			"flw.ps f0,  672 (x5)\n"
			"flw.ps f12,  448 (x5)\n"
			"flw.ps f4,  576 (x5)\n"
			"flw.ps f17,  320 (x5)\n"
			"flw.ps f3,  800 (x5)\n"
			"flw.ps f31,  128 (x5)\n"
			"flw.ps f10,  480 (x5)\n"
			"flw.ps f28,  544 (x5)\n"
			"flw.ps f5,  224 (x5)\n"
			"flw.ps f18,  992 (x5)\n"
			"flw.ps f9,  832 (x5)\n"
			"flw.ps f19,  288 (x5)\n"
			"flw.ps f7,  864 (x5)\n"
			"flw.ps f23,  160 (x5)\n"
			"flw.ps f13,  960 (x5)\n"
			"flw.ps f21,  32 (x5)\n"
			"flw.ps f16,  64 (x5)\n"
			"flw.ps f6,  608 (x5)\n"
			"flw.ps f29,  416 (x5)\n"
			"flw.ps f26,  704 (x5)\n"
			"flw.ps f24,  384 (x5)\n"
			"flw.ps f1,  640 (x5)\n"
			"flw.ps f22,  256 (x5)\n"
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
			"li x5, 0x5\n" 
			"sw x5, (x31)\n"
			"li x29, 0x5\n"
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
			"fbci.pi f28, 0xe454\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_6:\n"
			"masknot m1, m1\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0x18b8fd566c\n"
			"fsw.ps f0, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x17, rand_int_32\n"
			"flw.ps f0, 128(x17)\n"
			"flw.ps f8, 704(x17)\n"
			"fand.pi f7, f0, f8\n" 
			"la x20, rand_int_32\n"
			"flw.ps f0, 864(x20)\n"
			"flw.ps f21, 864(x20)\n"
			"fmulh.pi f29, f0, f21\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f14, 96(x10)\n"
			"flw.ps f23, 224(x10)\n"
			"fle.s x11, f14, f23\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f9, 960(x22)\n"
			"flw.ps f18, 608(x22)\n"
			"fle.s x10, f9, f18\n" 
			"la x11, rand_int_32\n"
			"flw.ps f7, 64(x11)\n"
			"fsat8.pi f18, f7\n" 
			"la x19, rand_int_32\n"
			"flw.ps f13, 0(x19)\n"
			"fsrli.pi f5, f13, 0x3\n" 
			"li x5, 0x5440\n"
			"csrw tensor_mask, x5\n"	
			"la x20, rand_ieee754_32\n"
			"flw.ps f9, 192(x20)\n"
			"fsin.ps f23, f9\n" 
			"la x11, rand_int_32\n"
			"flw.ps f14, 192(x11)\n"
			"flw.ps f2, 0(x11)\n"
			"feq.pi f8, f14, f2\n" 
			"la x23, rand_ieee754_16\n"
			"flw.ps f26, 480(x23)\n"
			"fcvt.ps.f16 f6, f26\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f6, 128(x19)\n"
			"flw.ps f9, 640(x19)\n"
			"fmin.ps f9, f6, f9\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f2, 512(x17)\n"
			"frsq.ps f23, f2\n" 
			"la x11, rand_int_32\n"
			"flw.ps f6, 352(x11)\n"
			"fnot.pi f6, f6\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f28, 672(x19)\n"
			"fswizz.ps f5, f28, 0x1\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x19, rand_ieee754_32\n"
			"flw.ps f3, 736(x19)\n"
			"flw.ps f5, 832(x19)\n"
			"flw.ps f28, 352(x19)\n"
			"fcmov.ps f13, f3, f5, f28\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"csrw tensor_error, zero\n" 
			"li x5, 0xfd1e764a5f0a0bf0\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0xbb06de30ef14d685\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ID_513673_HID_6:\n"
			"la x31, self_check_32\n"
			"li x5, 0xc0000000000009\n"
			"add x5, x5, x31\n"
			"li x6, 0x320000000000003\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x2 << TENSOR_FMA32_B_NUM_COLS) | (0x9 << TENSOR_FMA32_A_NUM_ROWS) | (0x3 << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x19 << TENSOR_FMA32_SCP_LOC_B) | (0x6 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0x4\n"
			"bne x5, x31, LBL_FAIL_HID_6\n"
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
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ITER_0_HID_6:\n"
			"la x31, self_check_32\n"
			"li x5, 0xc0000000000002\n"
			"add x5, x5, x31\n"
			"li x6, 0x380000000000006\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x1 << TENSOR_FMA32_B_NUM_COLS) | (0x2 << TENSOR_FMA32_A_NUM_ROWS) | (0x6 << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1c << TENSOR_FMA32_SCP_LOC_B) | (0x6 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
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
			"bne x5, x31, LBL_FAIL_HID_6\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_6\n"
			"csrw tensor_error, x0\n"
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_6\n"
			"addi x30, x30, 1\n"
			"la x14, rand_int_32\n"
			"flw.ps f6, 512(x14)\n"
			"flw.ps f26, 736(x14)\n"
			"flt.pi f8, f6, f26\n" 
			"LBL_SEQID_1_FP_TRANS_ID_1_HID_6:\n"
			"la x23, rand_ieee754_32\n"
			"flw.ps f6, 736(x23)\n"
			"frcp.ps f9, f6\n" 
			"la x22, rand_int_32\n"
			"flw.ps f28, 64(x22)\n"
			"fandi.pi f21, f28, 0x127\n" 
			"la x17, rand_int_32\n"
			"flw.ps f26, 960(x17)\n"
			"flw.ps f14, 992(x17)\n"
			"fsra.pi f23, f26, f14\n" 
			"la x28, rand_int_32\n"
			"flw.ps f5, 96(x28)\n"
			"fnot.pi f28, f5\n" 
			"la x22, rand_int_32\n"
			"flw.ps f13, 640(x22)\n"
			"flw.ps f5, 352(x22)\n"
			"feq.pi f7, f13, f5\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f26, 384(x17)\n"
			"flw.ps f18, 896(x17)\n"
			"fmax.ps f21, f26, f18\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f28, 512(x17)\n"
			"flw.ps f7, 736(x17)\n"
			"fsgnjx.s f3, f28, f7\n" 
			"fbci.pi f0, 0x1c554\n" 
			"la x23, rand_int_32\n"
			"flw.ps f2, 416(x23)\n"
			"flw.ps f9, 32(x23)\n"
			"fmulhu.pi f6, f2, f9\n" 
			"la x10, rand_int_32\n"
			"flw.ps f13, 416(x10)\n"
			"flw.ps f0, 512(x10)\n"
			"fmax.pi f23, f13, f0\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f3, 384(x23)\n"
			"flw.ps f13, 992(x23)\n"
			"fmin.ps f5, f3, f13\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f26, 224(x23)\n"
			"flw.ps f0, 896(x23)\n"
			"flw.ps f6, 192(x23)\n"
			"fnmadd.s f7, f26, f0, f6, rtz\n" 
			"LBL_SEQID_1_M0_WRITE_ID_2_HID_6:\n"
			"mov.m.x m4, x19, 0x22\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_1_M0_WRITE_ID_3_HID_6:\n"
			"maskand m3, m3, m5\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x22, rand_int_32\n"
			"flw.ps f5, 960(x22)\n"
			"flw.ps f6, 672(x22)\n"
			"feq.pi f29, f5, f6\n" 
			"csrw tensor_error, zero\n" 
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=0); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"LBL_SEQID_1_TLD_XMA_STR_SEQ_ITER_0_HID_6:\n"
			"la x31, self_check_32\n"
			"li x5, 0xc0000000000006\n"
			"add x5, x5, x31\n"
			"li x6, 0x3e0000000000000\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x2 << TENSOR_FMA32_B_NUM_COLS) | (0x6 << TENSOR_FMA32_A_NUM_ROWS) | (0x0 << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1f << TENSOR_FMA32_SCP_LOC_B) | (0x6 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x16 << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0x6 << TENSOR_QUANT_QUANT_ROW) | (0x30 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x7 << TENSOR_QUANT_TRANSF8) | (0x6 << TENSOR_QUANT_TRANSF7) | (0x7 << TENSOR_QUANT_TRANSF6) | (0x6 << TENSOR_QUANT_TRANSF5) | (0x6 << TENSOR_QUANT_TRANSF4) | (0x7 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0x1\n"
			"bne x5, x31, LBL_FAIL_HID_6\n"
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_6\n"
			"csrw tensor_error, x0\n"
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_6\n"
			"addi x30, x30, 1\n"
			"li x10, 0x7b6fada74f2326ba\n"
			"fcvt.s.wu f29, x10, rdn\n" 
			"la x20, rand_int_32\n"
			"flw.ps f21, 224(x20)\n"
			"fpackrepb.pi f14, f21\n" 
			"la x17, rand_int_32\n"
			"flw.ps f2, 704(x17)\n"
			"flw.ps f29, 256(x17)\n"
			"fand.pi f6, f2, f29\n" 
			"la x10, rand_int_32\n"
			"flw.ps f3, 928(x10)\n"
			"fpackreph.pi f5, f3\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f14, 832(x20)\n"
			"flw.ps f3, 672(x20)\n"
			"fadd.ps f0, f14, f3, rmm\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f28, 192(x28)\n"
			"flw.ps f0, 864(x28)\n"
			"fmin.ps f6, f28, f0\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f18, 32(x14)\n"
			"flw.ps f8, 192(x14)\n"
			"flw.ps f7, 288(x14)\n"
			"fmadd.s f2, f18, f8, f7, rup\n" 
			"la x19, rand_int_32\n"
			"flw.ps f2, 512(x19)\n"
			"fcvt.ps.pw f3, f2, rdn\n" 
			"LBL_SEQID_2_M0_WRITE_ID_4_HID_6:\n"
			"masknot m1, m5\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x14, rand_int_32\n"
			"flw.ps f3, 320(x14)\n"
			"fsrai.pi f2, f3, 0x7\n" 
			"LBL_SEQID_2_M0_WRITE_ID_5_HID_6:\n"
			"fltm.ps m1, f26, f26\n" 
			"li x5, 0x80340140\n"
			"ld x5, 0(x5)\n"
			"li x5, 0xa393\n"
			"csrw tensor_mask, x5\n"	
			"la x19, rand_ieee754_32\n"
			"flw.ps f28, 640(x19)\n"
			"feq.s x17, f28, f28\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f9, 384(x20)\n"
			"flw.ps f13, 960(x20)\n"
			"fmax.s f5, f9, f13\n" 
			"LBL_SEQID_2_M0_WRITE_ID_6_HID_6:\n"
			"maskxor m5, m1, m3\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x19, rand_ieee754_32\n"
			"flw.ps f14, 896(x19)\n"
			"fcvt.pw.ps f2, f14, rmm\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f0, 128(x23)\n"
			"flw.ps f29, 32(x23)\n"
			"fmax.s f7, f0, f29\n" 
			"li x5, 0x45ce1e901c6060c\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0xaec56df78d02f98c\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_2_XMA_QNT_STR_SEQ_ITER_0_HID_6:\n"
			"li x5, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x1 << TENSOR_FMA16_B_NUM_COLS) | (0xd << TENSOR_FMA16_A_NUM_ROWS) | (0x4 << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0xcf << TENSOR_FMA16_SCP_LOC_B) | (0x4c << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x0 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
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
			"la x28, rand_ieee754_32\n"
			"flw.ps f5, 576(x28)\n"
			"flw.ps f13, 224(x28)\n"
			"fle.ps f26, f5, f13\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f5, 768(x11)\n"
			"flw.ps f9, 384(x11)\n"
			"fmax.ps f23, f5, f9\n" 
			"li x22, 0xb63d2d84b67d2e66\n"
			"fcvt.s.wu f5, x22, rmm\n" 
			"LBL_SEQID_3_M0_WRITE_ID_7_HID_6:\n"
			"flem.ps m4, f0, f13\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x25, rand_int_32\n"
			"flw.ps f13, 64(x25)\n"
			"flw.ps f9, 448(x25)\n"
			"for.pi f8, f13, f9\n" 
			"maskpopc x17, m5\n" 
			"LBL_SEQID_3_M0_WRITE_ID_8_HID_6:\n"
			"fsetm.pi m1, f29\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x11, rand_int_32\n"
			"flw.ps f28, 224(x11)\n"
			"fslli.pi f23, f28, 0x3\n" 
			"LBL_SEQID_3_FP_TRANS_ID_9_HID_6:\n"
			"la x20, rand_ieee754_32\n"
			"flw.ps f26, 384(x20)\n"
			"fexp.ps f23, f26\n" 
			"la x10, rand_int_32\n"
			"flw.ps f21, 704(x10)\n"
			"flw.ps f7, 864(x10)\n"
			"fsra.pi f13, f21, f7\n" 
			"LBL_SEQID_3_M0_WRITE_ID_10_HID_6:\n"
			"maskand m3, m3, m4\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"li x5, 0xf7e8\n"
			"csrw tensor_mask, x5\n"	
			"LBL_SEQID_3_M0_WRITE_ID_11_HID_6:\n"
			"maskxor m4, m4, m5\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0x14ce6c085f\n"
			"fsw.ps f0, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x25, rand_ieee754_32\n"
			"flw.ps f18, 992(x25)\n"
			"fclass.ps f2, f18\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f6, 800(x19)\n"
			"fcvt.w.s x14, f6, rtz\n" 
			"la x17, rand_int_32\n"
			"flw.ps f9, 768(x17)\n"
			"flw.ps f6, 256(x17)\n"
			"fle.pi f7, f9, f6\n" 
			"la x25, rand_int_32\n"
			"flw.ps f3, 928(x25)\n"
			"flw.ps f26, 704(x25)\n"
			"fmax.pi f23, f3, f26\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_3_XMA_QNT_STR_SEQ_ID_361905_HID_6:\n"
			"li x6, ((0x19 << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0x3 << TENSOR_QUANT_QUANT_ROW) | (0x28 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x0 << TENSOR_QUANT_TRANSF1) | (0x1 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_3_XMA_QNT_STR_SEQ_ID_58068_HID_6:\n"
			"li x6, ((0xa << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0xf << TENSOR_QUANT_QUANT_ROW) | (0x18 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x6 << TENSOR_QUANT_TRANSF5) | (0x7 << TENSOR_QUANT_TRANSF4) | (0x6 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_6\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_3_TLD_XMA_STR_SEQ_ITER_0_HID_6:\n"
			"la x31, self_check_32\n"
			"li x5, 0x4000000000000b\n"
			"add x5, x5, x31\n"
			"li x6, 0x1000000000000d\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x1 << TENSOR_FMA32_B_NUM_COLS) | (0xb << TENSOR_FMA32_A_NUM_ROWS) | (0xd << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x1 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x18 << TENSOR_FMA32_SCP_LOC_B) | (0x2 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0xe\n"
			"bne x5, x31, LBL_FAIL_HID_6\n"
			"LBL_SEQID_3_TLD_XMA_STR_SEQ_ITER_1_HID_6:\n"
			"la x31, self_check_8\n"
			"li x5, 0x40000000000004\n"
			"add x5, x5, x31\n"
			"li x6, 0x10000000000007\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x1 << TENSOR_IMA_B_NUM_COLS) | (0x4 << TENSOR_IMA_A_NUM_ROWS) | (0x7 << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x1 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1f << TENSOR_IMA_SCP_LOC_B) | (0x2 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x9, ((0xa << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0x4 << TENSOR_QUANT_QUANT_ROW) | (0x3a << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x8 << TENSOR_QUANT_TRANSF3) | (0x8 << TENSOR_QUANT_TRANSF2) | (0x8 << TENSOR_QUANT_TRANSF1) | (0xa << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fmvz.x.ps x31, f0, 0\n"
			"li x5, 0x20\n"
			"bne x5, x31, LBL_FAIL_HID_6\n"
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 10\n"
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
			"la x20, rand_ieee754_32\n"
			"flw.ps f6, 192(x20)\n"
			"flw.ps f9, 32(x20)\n"
			"fmin.ps f23, f6, f9\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f6, 96(x11)\n"
			"flw.ps f7, 928(x11)\n"
			"fsgnjx.ps f23, f6, f7\n" 
			"la x19, rand_int_32\n"
			"flw.ps f26, 640(x19)\n"
			"flw.ps f7, 384(x19)\n"
			"fmax.pi f13, f26, f7\n" 
			"LBL_SEQID_4_M0_WRITE_ID_12_HID_6:\n"
			"maskand m5, m5, m5\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0x90d8810b11\n"
			"fsw.ps f0, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x11, rand_ieee754_32\n"
			"flw.ps f28, 672(x11)\n"
			"flw.ps f26, 896(x11)\n"
			"fle.ps f14, f28, f26\n" 
			"la x20, rand_int_32\n"
			"flw.ps f3, 448(x20)\n"
			"fandi.pi f9, f3, 0xe2\n" 
			"la x28, rand_int_32\n"
			"flw.ps f0, 480(x28)\n"
			"flw.ps f3, 96(x28)\n"
			"fdiv.pi f6, f0, f3\n" 
			"la x22, rand_int_32\n"
			"flw.ps f18, 224(x22)\n"
			"flw.ps f6, 800(x22)\n"
			"fmin.pi f28, f18, f6\n" 
			"li x17, 0xbf83a6bf1ba682fa\n"
			"fcvt.s.wu f13, x17, rdn\n" 
			"la x17, rand_int_32\n"
			"flw.ps f8, 992(x17)\n"
			"for.pi f9, f8, f8\n" 
			"la x17, rand_int_32\n"
			"flw.ps f8, 672(x17)\n"
			"fminu.pi f7, f8, f8\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f29, 384(x10)\n"
			"flw.ps f13, 128(x10)\n"
			"flw.ps f21, 320(x10)\n"
			"fnmadd.s f8, f29, f13, f21, rne\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f18, 224(x19)\n"
			"flw.ps f29, 640(x19)\n"
			"fadd.s f26, f18, f29, rtz\n" 
			"LBL_SEQID_4_M0_WRITE_ID_13_HID_6:\n"
			"masknot m1, m5\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"li x22, 0x1ee7a43c785f41da\n"
			"fcvt.s.wu f6, x22, rdn\n" 
			"fbci.pi f0, 0x41a9c\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_4_XMA_QNT_STR_SEQ_ITER_0_HID_6:\n"
			"li x5, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x2 << TENSOR_FMA32_B_NUM_COLS) | (0xa << TENSOR_FMA32_A_NUM_ROWS) | (0xd << TENSOR_FMA32_A_NUM_COLS) | (0xc << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0xe9 << TENSOR_FMA32_SCP_LOC_B) | (0xc8 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_6\n"
			"csrw tensor_error, x0\n"
			"li x5, 5\n" 
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
			"csrwi mhpmevent3, 4\n"   
			"csrwi mhpmevent4, 8\n"   
			"csrwi mhpmevent5, 6\n"   
			"csrwi mhpmevent6, 9\n"   
			"csrwi mhpmevent7, 21\n"   
			"csrwi mhpmevent8, 12\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f28,  32 (x5)\n"
			"flw.ps f29,  416 (x5)\n"
			"flw.ps f27,  640 (x5)\n"
			"flw.ps f16,  864 (x5)\n"
			"flw.ps f30,  64 (x5)\n"
			"flw.ps f10,  768 (x5)\n"
			"flw.ps f31,  384 (x5)\n"
			"flw.ps f13,  256 (x5)\n"
			"flw.ps f9,  832 (x5)\n"
			"flw.ps f19,  224 (x5)\n"
			"flw.ps f0,  608 (x5)\n"
			"flw.ps f11,  192 (x5)\n"
			"flw.ps f18,  288 (x5)\n"
			"flw.ps f25,  96 (x5)\n"
			"flw.ps f22,  960 (x5)\n"
			"flw.ps f6,  160 (x5)\n"
			"flw.ps f7,  672 (x5)\n"
			"flw.ps f23,  128 (x5)\n"
			"flw.ps f8,  800 (x5)\n"
			"flw.ps f1,  352 (x5)\n"
			"flw.ps f2,  576 (x5)\n"
			"flw.ps f12,  448 (x5)\n"
			"flw.ps f5,  736 (x5)\n"
			"flw.ps f4,  992 (x5)\n"
			"flw.ps f20,  928 (x5)\n"
			"flw.ps f21,  320 (x5)\n"
			"flw.ps f26,  0 (x5)\n"
			"flw.ps f14,  480 (x5)\n"
			"flw.ps f15,  544 (x5)\n"
			"flw.ps f3,  896 (x5)\n"
			"flw.ps f17,  704 (x5)\n"
			"flw.ps f24,  512 (x5)\n"
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
			"li x5, 0x5\n" 
			"sw x5, (x31)\n"
			"li x29, 0x5\n"
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
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_7:\n"
			"mov.m.x m1, x28, 0x69\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x10, rand_int_32\n"
			"flw.ps f2, 672(x10)\n"
			"flw.ps f19, 96(x10)\n"
			"fltu.pi f2, f2, f19\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f31, 672(x12)\n"
			"flw.ps f22, 128(x12)\n"
			"fsgnjx.ps f4, f31, f22\n" 
			"la x9, rand_int_32\n"
			"flw.ps f2, 160(x9)\n"
			"flw.ps f9, 448(x9)\n"
			"fsrl.pi f4, f2, f9\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f2, 416(x28)\n"
			"flw.ps f19, 320(x28)\n"
			"fmax.s f20, f2, f19\n" 
			"la x25, rand_int_32\n"
			"flw.ps f21, 896(x25)\n"
			"fandi.pi f21, f21, 0x69\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f9, 224(x12)\n"
			"flw.ps f20, 512(x12)\n"
			"flw.ps f29, 64(x12)\n"
			"fmsub.s f19, f9, f20, f29, rdn\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f2, 352(x28)\n"
			"flw.ps f31, 480(x28)\n"
			"fmin.s f12, f2, f31\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f8, 896(x11)\n"
			"flw.ps f20, 640(x11)\n"
			"fmin.ps f10, f8, f20\n" 
			"la x17, rand_int_32\n"
			"flw.ps f1, 736(x17)\n"
			"flw.ps f4, 640(x17)\n"
			"fsra.pi f8, f1, f4\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f31, 32(x9)\n"
			"flw.ps f1, 736(x9)\n"
			"fmul.ps f8, f31, f1, rmm\n" 
			"la x10, rand_int_32\n"
			"flw.ps f12, 192(x10)\n"
			"fandi.pi f19, f12, 0x1c\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f20, 320(x13)\n"
			"flw.ps f3, 0(x13)\n"
			"flw.ps f22, 512(x13)\n"
			"fmadd.ps f1, f20, f3, f22, rne\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f19, 832(x12)\n"
			"fround.ps f31, f19, rtz\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f3, 256(x11)\n"
			"flw.ps f20, 416(x11)\n"
			"fsgnjx.s f19, f3, f20\n" 
			"la x9, rand_int_32\n"
			"flw.ps f8, 704(x9)\n"
			"flw.ps f22, 256(x9)\n"
			"fsrl.pi f9, f8, f22\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_7\n"
			"addi x30, x30, 1\n"
			"la x23, rand_int_32\n"
			"flw.ps f5, 64(x23)\n"
			"flw.ps f12, 224(x23)\n"
			"fsll.pi f10, f5, f12\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f31, 288(x10)\n"
			"flw.ps f0, 704(x10)\n"
			"fsub.s f5, f31, f0, rdn\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f4, 128(x13)\n"
			"fclass.ps f4, f4\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f10, 256(x23)\n"
			"fadd.ps f1, f10, f10, rmm\n" 
			"la x13, rand_int_32\n"
			"flw.ps f29, 384(x13)\n"
			"fadd.pi f1, f29, f29\n" 
			"la x17, rand_int_32\n"
			"flw.ps f9, 32(x17)\n"
			"flw.ps f22, 0(x17)\n"
			"fmax.pi f0, f9, f22\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f4, 736(x9)\n"
			"flw.ps f31, 416(x9)\n"
			"fcmovm.ps f0, f4, f31\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f0, 928(x6)\n"
			"flw.ps f3, 832(x6)\n"
			"fsgnjn.s f21, f0, f3\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f21, 928(x13)\n"
			"flw.ps f31, 864(x13)\n"
			"flw.ps f19, 512(x13)\n"
			"fmsub.s f2, f21, f31, f19, rup\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f21, 512(x17)\n"
			"flw.ps f0, 448(x17)\n"
			"flw.ps f9, 960(x17)\n"
			"fmsub.ps f21, f21, f0, f9, rtz\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f9, 0(x9)\n"
			"flw.ps f31, 640(x9)\n"
			"fmax.s f31, f9, f31\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f22, 160(x28)\n"
			"fround.ps f5, f22, rmm\n" 
			"la x10, rand_int_32\n"
			"flw.ps f19, 640(x10)\n"
			"fnot.pi f31, f19\n" 
			"la x12, rand_int_32\n"
			"flw.ps f5, 608(x12)\n"
			"faddi.pi f19, f5, 0xd3\n" 
			"la x9, rand_ieee754_16\n"
			"flw.ps f29, 256(x9)\n"
			"fcvt.ps.f16 f12, f29\n" 
			"la x19, rand_int_32\n"
			"flw.ps f0, 736(x19)\n"
			"fpackrepb.pi f2, f0\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_7\n"
			"addi x30, x30, 1\n"
			"la x19, rand_ieee754_32\n"
			"flw.ps f2, 320(x19)\n"
			"flw.ps f4, 384(x19)\n"
			"flw.ps f29, 384(x19)\n"
			"fcmov.ps f1, f2, f4, f29\n" 
			"la x17, rand_int_32\n"
			"flw.ps f29, 192(x17)\n"
			"flw.ps f1, 832(x17)\n"
			"fmaxu.pi f1, f29, f1\n" 
			"mova.x.m x12\n" 
			"LBL_SEQID_2_M0_WRITE_ID_1_HID_7:\n"
			"flem.ps m2, f5, f31\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0xc83f1d8206\n"
			"flw.ps f5, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x13, rand_ieee754_32\n"
			"flw.ps f0, 192(x13)\n"
			"flw.ps f12, 896(x13)\n"
			"fsgnj.ps f5, f0, f12\n" 
			"la x12, rand_int_32\n"
			"flw.ps f20, 96(x12)\n"
			"flw.ps f0, 864(x12)\n"
			"fxor.pi f8, f20, f0\n" 
			"la x10, rand_ieee754_16\n"
			"flw.ps f21, 480(x10)\n"
			"fcvt.ps.f16 f1, f21\n" 
			"li x25, 0x6577dcddd236b9fc\n"
			"fcvt.s.w f21, x25, rne\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f29, 512(x17)\n"
			"fswizz.ps f22, f29, 0x1c\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f4, 256(x9)\n"
			"fcvt.w.s x25, f4, rdn\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f21, 896(x23)\n"
			"flw.ps f10, 256(x23)\n"
			"fsgnjn.s f5, f21, f10\n" 
			"la x10, rand_int_32\n"
			"flw.ps f3, 384(x10)\n"
			"fandi.pi f2, f3, 0xe1\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f9, 288(x28)\n"
			"flw.ps f21, 960(x28)\n"
			"fmsub.s f10, f9, f21, f9, rup\n" 
			"la x9, rand_int_32\n"
			"flw.ps f3, 0(x9)\n"
			"flw.ps f20, 352(x9)\n"
			"fdivu.pi f20, f3, f20\n" 
			"la x19, rand_int_32\n"
			"flw.ps f20, 256(x19)\n"
			"flw.ps f31, 992(x19)\n"
			"fmax.pi f1, f20, f31\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f10, 736(x11)\n"
			"fcvt.pw.ps f31, f10, rmm\n" 
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
			"LBL_SEQID_3_FP_TRANS_ID_2_HID_7:\n"
			"la x13, rand_ieee754_32\n"
			"flw.ps f31, 576(x13)\n"
			"fexp.ps f29, f31\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f1, 352(x9)\n"
			"frsq.ps f3, f1\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f20, 576(x17)\n"
			"ffrc.ps f5, f20\n" 
			"la x19, rand_int_32\n"
			"flw.ps f5, 96(x19)\n"
			"fpackrepb.pi f20, f5\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f20, 960(x19)\n"
			"flw.ps f29, 608(x19)\n"
			"fsgnjn.s f1, f20, f29\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f21, 928(x13)\n"
			"flw.ps f5, 256(x13)\n"
			"fsub.s f29, f21, f5, rmm\n" 
			"la x23, rand_int_32\n"
			"flw.ps f2, 256(x23)\n"
			"flw.ps f8, 928(x23)\n"
			"fxor.pi f4, f2, f8\n" 
			"la x11, rand_int_32\n"
			"flw.ps f10, 928(x11)\n"
			"fcvt.ps.pw f10, f10, rdn\n" 
			"la x19, rand_int_32\n"
			"flw.ps f4, 576(x19)\n"
			"fmulhu.pi f8, f4, f4\n" 
			"LBL_SEQID_3_M0_WRITE_ID_3_HID_7:\n"
			"mov.m.x m5, x17, 0x60\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x13, rand_ieee754_32\n"
			"flw.ps f22, 832(x13)\n"
			"flw.ps f29, 896(x13)\n"
			"fmin.s f12, f22, f29\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f20, 512(x11)\n"
			"ffrc.ps f20, f20\n" 
			"LBL_SEQID_3_M0_WRITE_ID_4_HID_7:\n"
			"fltm.pi m1, f10, f5\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x11, rand_ieee754_32\n"
			"flw.ps f22, 640(x11)\n"
			"flw.ps f5, 256(x11)\n"
			"fmsub.s f22, f22, f5, f22, rne\n" 
			"la x28, rand_int_32\n"
			"flw.ps f29, 256(x28)\n"
			"fsrai.pi f4, f29, 0x6\n" 
			"la x12, rand_int_32\n"
			"flw.ps f5, 0(x12)\n"
			"flw.ps f2, 800(x12)\n"
			"fsll.pi f22, f5, f2\n" 
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
			"la x19, rand_ieee754_32\n"
			"flw.ps f5, 0(x19)\n"
			"fcvt.pwu.ps f0, f5, rdn\n" 
			"la x23, rand_int_32\n"
			"flw.ps f20, 704(x23)\n"
			"fmul.pi f3, f20, f20\n" 
			"la x13, rand_int_32\n"
			"flw.ps f10, 576(x13)\n"
			"fpackreph.pi f3, f10\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f0, 896(x28)\n"
			"fmin.s f8, f0, f0\n" 
			"maskpopc x6, m0\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f21, 160(x25)\n"
			"flw.ps f10, 0(x25)\n"
			"flw.ps f12, 0(x25)\n"
			"fnmsub.ps f22, f21, f10, f12, rtz\n" 
			"la x10, rand_ieee754_16\n"
			"flw.ps f22, 832(x10)\n"
			"fcvt.ps.f16 f29, f22\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f21, 832(x13)\n"
			"flw.ps f9, 288(x13)\n"
			"fsgnjn.s f20, f21, f9\n" 
			"LBL_SEQID_4_FP_TRANS_ID_5_HID_7:\n"
			"la x9, rand_ieee754_32\n"
			"flw.ps f29, 928(x9)\n"
			"flog.ps f0, f29\n" 
			"la x9, rand_int_32\n"
			"flw.ps f3, 608(x9)\n"
			"flw.ps f2, 896(x9)\n"
			"fmul.pi f9, f3, f2\n" 
			"LBL_SEQID_4_M0_WRITE_ID_6_HID_7:\n"
			"fltm.pi m1, f4, f22\n" 
			"li x5, 0x80340118\n"
			"ld x5, 0(x5)\n"
			"la x19, rand_ieee754_32\n"
			"flw.ps f31, 864(x19)\n"
			"flw.ps f0, 992(x19)\n"
			"fmul.s f4, f31, f0, rtz\n" 
			"la x23, rand_int_32\n"
			"flw.ps f4, 832(x23)\n"
			"fcvt.ps.pw f0, f4, rtz\n" 
			"la x13, rand_int_32\n"
			"flw.ps f5, 32(x13)\n"
			"flw.ps f9, 224(x13)\n"
			"fsll.pi f9, f5, f9\n" 
			"LBL_SEQID_4_FP_TRANS_ID_7_HID_7:\n"
			"la x25, rand_ieee754_32\n"
			"flw.ps f20, 736(x25)\n"
			"frcp.ps f4, f20\n" 
			"la x17, rand_int_32\n"
			"flw.ps f31, 384(x17)\n"
			"flw.ps f4, 928(x17)\n"
			"fand.pi f3, f31, f4\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 5\n" 
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
			"csrwi mhpmevent3, 17\n"   
			"csrwi mhpmevent4, 28\n"   
			"csrwi mhpmevent5, 19\n"   
			"csrwi mhpmevent6, 16\n"   
			"csrwi mhpmevent7, 4\n"   
			"csrwi mhpmevent8, 7\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f14,  576 (x5)\n"
			"flw.ps f27,  480 (x5)\n"
			"flw.ps f19,  608 (x5)\n"
			"flw.ps f13,  832 (x5)\n"
			"flw.ps f26,  96 (x5)\n"
			"flw.ps f3,  64 (x5)\n"
			"flw.ps f21,  864 (x5)\n"
			"flw.ps f1,  672 (x5)\n"
			"flw.ps f18,  224 (x5)\n"
			"flw.ps f15,  352 (x5)\n"
			"flw.ps f10,  128 (x5)\n"
			"flw.ps f22,  416 (x5)\n"
			"flw.ps f7,  544 (x5)\n"
			"flw.ps f8,  384 (x5)\n"
			"flw.ps f4,  928 (x5)\n"
			"flw.ps f30,  0 (x5)\n"
			"flw.ps f5,  448 (x5)\n"
			"flw.ps f9,  192 (x5)\n"
			"flw.ps f20,  736 (x5)\n"
			"flw.ps f31,  704 (x5)\n"
			"flw.ps f25,  320 (x5)\n"
			"flw.ps f16,  160 (x5)\n"
			"flw.ps f11,  640 (x5)\n"
			"flw.ps f2,  768 (x5)\n"
			"flw.ps f0,  992 (x5)\n"
			"flw.ps f23,  256 (x5)\n"
			"flw.ps f29,  800 (x5)\n"
			"flw.ps f28,  32 (x5)\n"
			"flw.ps f6,  512 (x5)\n"
			"flw.ps f24,  896 (x5)\n"
			"flw.ps f12,  960 (x5)\n"
			"flw.ps f17,  288 (x5)\n"
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
			"li x5, 0x5\n" 
			"sw x5, (x31)\n"
			"li x29, 0x5\n"
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
			"la x5, rand_ieee754_32\n"
			"flw.ps f3, 544(x5)\n"
			"flw.ps f11, 160(x5)\n"
			"fsgnj.s f30, f3, f11\n" 
			"la x27, rand_int_32\n"
			"flw.ps f27, 288(x27)\n"
			"flw.ps f5, 512(x27)\n"
			"fmulh.pi f6, f27, f5\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f28, 896(x5)\n"
			"fswizz.ps f11, f28, 0x12\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f27, 704(x27)\n"
			"fswizz.ps f1, f27, 0x1e\n" 
			"li x20, 0x8333c49031754f8b\n"
			"fcvt.s.w f29, x20, rmm\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f22, 160(x12)\n"
			"flw.ps f19, 256(x12)\n"
			"flw.ps f11, 832(x12)\n"
			"fcmov.ps f26, f22, f19, f11\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f18, 224(x28)\n"
			"flw.ps f28, 192(x28)\n"
			"fle.s x20, f18, f28\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x27, rand_ieee754_32\n"
			"flw.ps f15, 544(x27)\n"
			"flw.ps f22, 544(x27)\n"
			"flw.ps f11, 416(x27)\n"
			"fmadd.ps f3, f15, f22, f11, rne\n" 
			"la x18, rand_int_32\n"
			"flw.ps f1, 0(x18)\n"
			"flw.ps f19, 128(x18)\n"
			"fmulh.pi f15, f1, f19\n" 
			"la x14, rand_int_32\n"
			"flw.ps f27, 992(x14)\n"
			"fsrai.pi f19, f27, 0x4\n" 
			"la x12, rand_int_32\n"
			"flw.ps f5, 192(x12)\n"
			"fsatu8.pi f28, f5\n" 
			"la x11, rand_int_32\n"
			"flw.ps f27, 576(x11)\n"
			"flw.ps f6, 544(x11)\n"
			"feq.pi f26, f27, f6\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f6, 576(x11)\n"
			"fcvt.pwu.ps f3, f6, rup\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f18, 992(x18)\n"
			"flw.ps f6, 352(x18)\n"
			"fle.s x11, f18, f6\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f27, 960(x5)\n"
			"fround.ps f3, f27, rmm\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f3, 576(x11)\n"
			"flw.ps f18, 0(x11)\n"
			"flw.ps f22, 512(x11)\n"
			"fnmadd.s f27, f3, f18, f22, rtz\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_0_RDX_ID_0_SHIRE_0_HID_8:\n"
			"li x11, 0x1000000000790000\n"
			"csrr x7, mhartid\n"
			"srli x7, x7, 4\n"
			"slli x7, x7, 6\n"
			"add x11, x11, x7\n"
			"csrw tensor_reduce, x11\n" 
			"csrwi tensor_wait, 9\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ID_442679_HID_8:\n"
			"la x31, self_check_32\n"
			"li x5, 0xa000000000000c\n"
			"add x5, x5, x31\n"
			"li x6, 0x10000000000004\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x2 << TENSOR_FMA32_B_NUM_COLS) | (0xc << TENSOR_FMA32_A_NUM_ROWS) | (0x4 << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x1 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1b << TENSOR_FMA32_SCP_LOC_B) | (0x5 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x9, ((0xb << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0xc << TENSOR_QUANT_QUANT_ROW) | (0x20 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0x5\n"
			"bne x5, x31, LBL_FAIL_HID_8\n"
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 10\n"
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
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ITER_0_HID_8:\n"
			"li x6, ((0x9 << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0x5 << TENSOR_QUANT_QUANT_ROW) | (0x2c << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_8\n"
			"csrw tensor_error, x0\n"
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_8\n"
			"addi x30, x30, 1\n"
			"la x27, rand_ieee754_32\n"
			"flw.ps f28, 352(x27)\n"
			"fcvt.pwu.ps f1, f28, rdn\n" 
			"li x5, 0x15c6\n"
			"csrw tensor_mask, x5\n"	
			"la x13, rand_int_32\n"
			"flw.ps f15, 928(x13)\n"
			"flw.ps f19, 480(x13)\n"
			"fxor.pi f3, f15, f19\n" 
			"la x18, rand_int_32\n"
			"flw.ps f11, 736(x18)\n"
			"flw.ps f28, 864(x18)\n"
			"fmax.pi f3, f11, f28\n" 
			"LBL_SEQID_1_M0_WRITE_ID_0_HID_8:\n"
			"masknot m7, m0\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_1_M0_WRITE_ID_1_HID_8:\n"
			"fsetm.pi m7, f19\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x27, rand_int_32\n"
			"flw.ps f11, 416(x27)\n"
			"flw.ps f22, 672(x27)\n"
			"fmax.pi f11, f11, f22\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f5, 608(x7)\n"
			"frsq.ps f17, f5\n" 
			"fbci.ps f3, 0x2f782\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x14, rand_ieee754_32\n"
			"flw.ps f15, 608(x14)\n"
			"flw.ps f1, 160(x14)\n"
			"flt.ps f19, f15, f1\n" 
			"la x6, rand_int_32\n"
			"flw.ps f17, 128(x6)\n"
			"faddi.pi f30, f17, 0x1a5\n" 
			"LBL_SEQID_1_M0_WRITE_ID_2_HID_8:\n"
			"flem.ps m4, f28, f17\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x28, rand_int_32\n"
			"flw.ps f17, 0(x28)\n"
			"fnot.pi f6, f17\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f27, 96(x20)\n"
			"flw.ps f11, 32(x20)\n"
			"fle.ps f29, f27, f11\n" 
			"LBL_SEQID_1_M0_WRITE_ID_3_HID_8:\n"
			"fltm.ps m7, f18, f5\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"fbci.pi f11, 0x34945\n" 
			"LBL_SEQID_1_FP_TRANS_ID_4_HID_8:\n"
			"la x28, rand_ieee754_32\n"
			"flw.ps f31, 672(x28)\n"
			"flog.ps f28, f31\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_1_RDX_ID_1_SHIRE_0_HID_8:\n"
			"li x14, 0x2c00000004700001\n"
			"csrr x5, mhartid\n"
			"srli x5, x5, 4\n"
			"slli x5, x5, 6\n"
			"add x14, x14, x5\n"
			"csrw tensor_reduce, x14\n" 
			"csrwi tensor_wait, 9\n" 
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
			"li x5, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x1 << TENSOR_FMA32_B_NUM_COLS) | (0x0 << TENSOR_FMA32_A_NUM_ROWS) | (0xf << TENSOR_FMA32_A_NUM_COLS) | (0x9 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x81 << TENSOR_FMA32_SCP_LOC_B) | (0x65 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x0 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x6, ((0x4 << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0x0 << TENSOR_QUANT_QUANT_ROW) | (0x26 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x7 << TENSOR_QUANT_TRANSF6) | (0x7 << TENSOR_QUANT_TRANSF5) | (0x6 << TENSOR_QUANT_TRANSF4) | (0x6 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 7\n"
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_8\n"
			"csrw tensor_error, x0\n"
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_8\n"
			"addi x30, x30, 1\n"
			"la x18, rand_ieee754_32\n"
			"flw.ps f11, 512(x18)\n"
			"flw.ps f29, 480(x18)\n"
			"fcmovm.ps f6, f11, f29\n" 
			"LBL_SEQID_2_M0_WRITE_ID_5_HID_8:\n"
			"mova.m.x x20\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x11, rand_ieee754_32\n"
			"flw.ps f1, 800(x11)\n"
			"flw.ps f19, 352(x11)\n"
			"flw.ps f26, 0(x11)\n"
			"fmsub.ps f19, f1, f19, f26, rup\n" 
			"maskpopc x11, m1\n" 
			"LBL_SEQID_2_M0_WRITE_ID_6_HID_8:\n"
			"feqm.ps m1, f1, f29\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x11, rand_int_32\n"
			"flw.ps f18, 256(x11)\n"
			"flw.ps f28, 96(x11)\n"
			"feq.pi f3, f18, f28\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f28, 448(x14)\n"
			"frsq.ps f19, f28\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f30, 416(x18)\n"
			"flw.ps f26, 800(x18)\n"
			"feq.ps f17, f30, f26\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f27, 672(x14)\n"
			"fcvt.w.s x18, f27, rne\n" 
			"la x27, rand_int_32\n"
			"flw.ps f3, 640(x27)\n"
			"flw.ps f15, 224(x27)\n"
			"fsub.pi f1, f3, f15\n" 
			"li x5, 0xf12409f1914d8ee5\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0x53b2f15c37f27c86\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"LBL_SEQID_2_M0_WRITE_ID_7_HID_8:\n"
			"feqm.ps m7, f6, f29\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x27, rand_ieee754_32\n"
			"flw.ps f3, 640(x27)\n"
			"flw.ps f17, 992(x27)\n"
			"fmax.s f3, f3, f17\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f1, 640(x14)\n"
			"flw.ps f22, 832(x14)\n"
			"flw.ps f28, 736(x14)\n"
			"fmsub.s f27, f1, f22, f28, rtz\n" 
			"li x13, 0x9a3c61d2f68cb6b3\n"
			"fcvt.s.w f11, x13, rne\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f3, 832(x7)\n"
			"fsqrt.s f27, f3, rmm\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f26, 64(x20)\n"
			"flw.ps f19, 64(x20)\n"
			"fmul.s f31, f26, f19, rdn\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_2_TLD_XMA_STR_SEQ_ITER_0_HID_8:\n"
			"la x31, self_check_8\n"
			"li x5, 0xe000000000000f\n"
			"add x5, x5, x31\n"
			"li x6, 0x10000000000006\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x3 << TENSOR_IMA_B_NUM_COLS) | (0xf << TENSOR_IMA_A_NUM_ROWS) | (0x6 << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x1 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1a << TENSOR_IMA_SCP_LOC_B) | (0x7 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fmvz.x.ps x31, f0, 0\n"
			"li x5, 0x1c\n"
			"bne x5, x31, LBL_FAIL_HID_8\n"
			"LBL_SEQID_2_TLD_XMA_STR_SEQ_ITER_1_HID_8:\n"
			"la x31, self_check_8\n"
			"li x5, 0xc000000000000b\n"
			"add x5, x5, x31\n"
			"li x6, 0x340000000000001\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x3 << TENSOR_IMA_B_NUM_COLS) | (0xb << TENSOR_IMA_A_NUM_ROWS) | (0x1 << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1a << TENSOR_IMA_SCP_LOC_B) | (0x6 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fmvz.x.ps x31, f0, 0\n"
			"li x5, 0x8\n"
			"bne x5, x31, LBL_FAIL_HID_8\n"
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
			"LBL_SEQID_3_M0_WRITE_ID_8_HID_8:\n"
			"maskand m4, m7, m0\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x14, rand_ieee754_32\n"
			"flw.ps f3, 544(x14)\n"
			"fcvt.pw.ps f28, f3, rne\n" 
			"LBL_SEQID_3_M0_WRITE_ID_9_HID_8:\n"
			"flem.ps m0, f11, f6\n" 
			"li x5, CREDINC3\n"
			"ld x5, 0(x5)\n"
			"la x14, rand_ieee754_32\n"
			"flw.ps f15, 32(x14)\n"
			"flw.ps f11, 736(x14)\n"
			"fsgnjn.s f18, f15, f11\n" 
			"LBL_SEQID_3_M0_WRITE_ID_10_HID_8:\n"
			"fltm.pi m4, f31, f26\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x14, rand_int_32\n"
			"flw.ps f17, 384(x14)\n"
			"flw.ps f15, 576(x14)\n"
			"fmulh.pi f22, f17, f15\n" 
			"la x18, rand_int_32\n"
			"flw.ps f11, 832(x18)\n"
			"flw.ps f31, 896(x18)\n"
			"fmul.pi f19, f11, f31\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x7, rand_int_32\n"
			"flw.ps f29, 832(x7)\n"
			"flw.ps f6, 352(x7)\n"
			"fsll.pi f26, f29, f6\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f22, 384(x11)\n"
			"flw.ps f26, 672(x11)\n"
			"flw.ps f28, 384(x11)\n"
			"fcmov.ps f11, f22, f26, f28\n" 
			"la x18, rand_int_32\n"
			"flw.ps f5, 672(x18)\n"
			"flw.ps f27, 864(x18)\n"
			"flt.pi f29, f5, f27\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f28, 480(x27)\n"
			"flw.ps f19, 992(x27)\n"
			"fmax.ps f28, f28, f19\n" 
			"la x7, rand_ieee754_16\n"
			"flw.ps f5, 736(x7)\n"
			"fcvt.ps.f16 f19, f5\n" 
			"la x27, rand_int_32\n"
			"flw.ps f5, 736(x27)\n"
			"fcvt.ps.pw f30, f5, rmm\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f3, 800(x11)\n"
			"fswizz.ps f19, f3, 0x6\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f3, 288(x28)\n"
			"flw.ps f1, 576(x28)\n"
			"feq.s x20, f3, f1\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f22, 928(x5)\n"
			"flw.ps f1, 416(x5)\n"
			"fle.s x11, f22, f1\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_3_TLD_XMA_STR_SEQ_ITER_0_HID_8:\n"
			"la x31, self_check_8\n"
			"li x5, 0x9\n"
			"add x5, x5, x31\n"
			"li x6, 0x380000000000004\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x3 << TENSOR_IMA_B_NUM_COLS) | (0x9 << TENSOR_IMA_A_NUM_ROWS) | (0x4 << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1c << TENSOR_IMA_SCP_LOC_B) | (0x0 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x9, ((0x1a << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0x9 << TENSOR_QUANT_QUANT_ROW) | (0x0 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x0 << TENSOR_QUANT_TRANSF1) | (0x0 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fmvz.x.ps x31, f0, 0\n"
			"li x5, 0x14\n"
			"bne x5, x31, LBL_FAIL_HID_8\n"
			"csrw tensor_quant, x9\n"	
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
			"LBL_SEQID_4_M0_WRITE_ID_11_HID_8:\n"
			"mov.m.x m4, x14, 0x56\n" 
			"li x5, CREDINC2\n"
			"ld x5, 0(x5)\n"
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x13, rand_ieee754_32\n"
			"flw.ps f22, 576(x13)\n"
			"flw.ps f28, 32(x13)\n"
			"fsub.ps f3, f22, f28, rup\n" 
			"LBL_SEQID_4_M0_WRITE_ID_12_HID_8:\n"
			"mova.m.x x27\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x28, rand_int_32\n"
			"flw.ps f30, 576(x28)\n"
			"flw.ps f18, 288(x28)\n"
			"fminu.pi f27, f30, f18\n" 
			"LBL_SEQID_4_M0_WRITE_ID_13_HID_8:\n"
			"fltm.pi m7, f31, f11\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x28, rand_ieee754_32\n"
			"flw.ps f27, 512(x28)\n"
			"flw.ps f15, 672(x28)\n"
			"flw.ps f28, 0(x28)\n"
			"fcmov.ps f22, f27, f15, f28\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f1, 0(x6)\n"
			"flw.ps f31, 0(x6)\n"
			"fsgnj.s f27, f1, f31\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f1, 128(x5)\n"
			"flw.ps f28, 96(x5)\n"
			"fmin.ps f19, f1, f28\n" 
			"la x20, rand_int_32\n"
			"flw.ps f26, 384(x20)\n"
			"flw.ps f30, 128(x20)\n"
			"fadd.pi f29, f26, f30\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f11, 192(x11)\n"
			"fcvt.pwu.ps f19, f11, rmm\n" 
			"la x14, rand_int_32\n"
			"flw.ps f3, 512(x14)\n"
			"flw.ps f19, 736(x14)\n"
			"fmin.pi f17, f3, f19\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x5, rand_ieee754_32\n"
			"flw.ps f29, 96(x5)\n"
			"fswizz.ps f19, f29, 0x0\n" 
			"la x28, rand_int_32\n"
			"flw.ps f29, 640(x28)\n"
			"flw.ps f26, 512(x28)\n"
			"fminu.pi f6, f29, f26\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f26, 384(x27)\n"
			"ffrc.ps f29, f26\n" 
			"la x6, rand_int_32\n"
			"flw.ps f27, 128(x6)\n"
			"flw.ps f15, 800(x6)\n"
			"fminu.pi f15, f27, f15\n" 
			"LBL_SEQID_4_M0_WRITE_ID_14_HID_8:\n"
			"maskand m4, m7, m7\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_4_TLD_XMA_STR_SEQ_ID_731256_HID_8:\n"
			"la x31, self_check_32\n"
			"li x5, 0x60000000000005\n"
			"add x5, x5, x31\n"
			"li x6, 0x380000000000002\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x2 << TENSOR_FMA32_B_NUM_COLS) | (0x5 << TENSOR_FMA32_A_NUM_ROWS) | (0x2 << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1c << TENSOR_FMA32_SCP_LOC_B) | (0x3 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0x3\n"
			"bne x5, x31, LBL_FAIL_HID_8\n"
			"LBL_SEQID_4_TLD_XMA_STR_SEQ_ID_37721_HID_8:\n"
			"la x31, self_check_16\n"
			"li x5, 0xa0000000000001\n"
			"add x5, x5, x31\n"
			"li x6, 0x10000000000001\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x2 << TENSOR_FMA16_B_NUM_COLS) | (0x1 << TENSOR_FMA16_A_NUM_ROWS) | (0x1 << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x1 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1f << TENSOR_FMA16_SCP_LOC_B) | (0x5 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x1a << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0x1 << TENSOR_QUANT_QUANT_ROW) | (0x19 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x7 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0x4\n"
			"bne x5, x31, LBL_FAIL_HID_8\n"
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_8\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_4_XMA_QNT_STR_SEQ_ITER_0_HID_8:\n"
			"li x5, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x0 << TENSOR_IMA_B_NUM_COLS) | (0x5 << TENSOR_IMA_A_NUM_ROWS) | (0xc << TENSOR_IMA_A_NUM_COLS) | (0xc << TENSOR_IMA_OFFSET) | (0x0 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x5e << TENSOR_IMA_SCP_LOC_B) | (0x13 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"LBL_SEQID_4_XMA_QNT_STR_SEQ_ITER_1_HID_8:\n"
			"li x5, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x3 << TENSOR_IMA_B_NUM_COLS) | (0x8 << TENSOR_IMA_A_NUM_ROWS) | (0xf << TENSOR_IMA_A_NUM_COLS) | (0x5 << TENSOR_IMA_OFFSET) | (0x0 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x4a << TENSOR_IMA_SCP_LOC_B) | (0x4b << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x0 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_8\n"
			"csrw tensor_error, x0\n"
			"li x5, 5\n" 
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
			"csrwi mhpmevent3, 4\n"   
			"csrwi mhpmevent4, 8\n"   
			"csrwi mhpmevent5, 6\n"   
			"csrwi mhpmevent6, 9\n"   
			"csrwi mhpmevent7, 21\n"   
			"csrwi mhpmevent8, 12\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f6,  608 (x5)\n"
			"flw.ps f2,  160 (x5)\n"
			"flw.ps f9,  576 (x5)\n"
			"flw.ps f29,  64 (x5)\n"
			"flw.ps f21,  512 (x5)\n"
			"flw.ps f24,  544 (x5)\n"
			"flw.ps f5,  960 (x5)\n"
			"flw.ps f17,  640 (x5)\n"
			"flw.ps f19,  928 (x5)\n"
			"flw.ps f28,  224 (x5)\n"
			"flw.ps f22,  352 (x5)\n"
			"flw.ps f3,  864 (x5)\n"
			"flw.ps f30,  192 (x5)\n"
			"flw.ps f20,  896 (x5)\n"
			"flw.ps f11,  832 (x5)\n"
			"flw.ps f16,  32 (x5)\n"
			"flw.ps f31,  800 (x5)\n"
			"flw.ps f26,  384 (x5)\n"
			"flw.ps f18,  128 (x5)\n"
			"flw.ps f0,  0 (x5)\n"
			"flw.ps f1,  992 (x5)\n"
			"flw.ps f8,  96 (x5)\n"
			"flw.ps f14,  448 (x5)\n"
			"flw.ps f13,  288 (x5)\n"
			"flw.ps f23,  672 (x5)\n"
			"flw.ps f25,  256 (x5)\n"
			"flw.ps f27,  320 (x5)\n"
			"flw.ps f15,  736 (x5)\n"
			"flw.ps f4,  416 (x5)\n"
			"flw.ps f12,  768 (x5)\n"
			"flw.ps f7,  704 (x5)\n"
			"flw.ps f10,  480 (x5)\n"
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
			"li x5, 0x5\n" 
			"sw x5, (x31)\n"
			"li x29, 0x5\n"
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
			"la x23, rand_int_32\n"
			"flw.ps f4, 224(x23)\n"
			"fandi.pi f28, f4, 0x41\n" 
			"LBL_SEQID_0_FP_TRANS_ID_0_HID_9:\n"
			"la x11, rand_ieee754_32\n"
			"flw.ps f4, 672(x11)\n"
			"frcp.ps f15, f4\n" 
			"li x10, 0xe06f7f2ba0934c6e\n"
			"fcvt.s.w f1, x10, rdn\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f5, 672(x23)\n"
			"flw.ps f4, 832(x23)\n"
			"fcmovm.ps f6, f5, f4\n" 
			"la x10, rand_int_32\n"
			"flw.ps f13, 864(x10)\n"
			"fcvt.ps.pwu f15, f13, rup\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f26, 608(x13)\n"
			"flw.ps f15, 864(x13)\n"
			"fsgnjx.s f26, f26, f15\n" 
			"la x5, rand_int_32\n"
			"flw.ps f0, 640(x5)\n"
			"flw.ps f6, 768(x5)\n"
			"fadd.pi f8, f0, f6\n" 
			"la x28, rand_int_32\n"
			"flw.ps f15, 800(x28)\n"
			"flw.ps f4, 672(x28)\n"
			"fdivu.pi f16, f15, f4\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_9:\n"
			"fsetm.pi m0, f11\n" 
			"mov.m.x m0, x0, 0xff\n"
			"li x5, CREDINC0\n"
			"ld x5, 0(x5)\n"
			"la x26, rand_int_32\n"
			"flw.ps f25, 32(x26)\n"
			"flw.ps f0, 576(x26)\n"
			"fmax.pi f26, f25, f0\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f25, 352(x26)\n"
			"frsq.ps f7, f25\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f28, 864(x11)\n"
			"fclass.ps f11, f28\n" 
			"la x23, rand_int_32\n"
			"flw.ps f5, 224(x23)\n"
			"fpackreph.pi f9, f5\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f0, 320(x28)\n"
			"flw.ps f26, 288(x28)\n"
			"fmin.s f23, f0, f26\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f7, 864(x13)\n"
			"fsqrt.ps f0, f7\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f28, 384(x5)\n"
			"flw.ps f11, 640(x5)\n"
			"flw.ps f23, 256(x5)\n"
			"fnmadd.s f0, f28, f11, f23, rtz\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_9\n"
			"addi x30, x30, 1\n"
			"la x13, rand_ieee754_32\n"
			"flw.ps f13, 192(x13)\n"
			"flw.ps f23, 192(x13)\n"
			"fle.ps f26, f13, f23\n" 
			"la x10, rand_int_32\n"
			"flw.ps f28, 96(x10)\n"
			"faddi.pi f0, f28, 0x35\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f0, 480(x12)\n"
			"fcvt.f16.ps f25, f0\n" 
			"LBL_SEQID_1_FP_TRANS_ID_2_HID_9:\n"
			"la x10, rand_ieee754_32\n"
			"flw.ps f4, 736(x10)\n"
			"flog.ps f16, f4\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f8, 800(x20)\n"
			"fsqrt.s f0, f8, rdn\n" 
			"mova.x.m x23\n" 
			"la x20, rand_int_32\n"
			"flw.ps f1, 512(x20)\n"
			"fnot.pi f23, f1\n" 
			"LBL_SEQID_1_M0_WRITE_ID_3_HID_9:\n"
			"maskxor m0, m7, m6\n" 
			"li x5, CREDINC1\n"
			"ld x5, 0(x5)\n"
			"LBL_SEQID_1_M0_WRITE_ID_4_HID_9:\n"
			"fltm.ps m5, f5, f1\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x23, rand_ieee754_32\n"
			"flw.ps f11, 608(x23)\n"
			"fcvt.f16.ps f28, f11\n" 
			"la x16, rand_int_32\n"
			"flw.ps f15, 256(x16)\n"
			"flw.ps f25, 416(x16)\n"
			"feq.pi f11, f15, f25\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f11, 640(x26)\n"
			"fcvt.w.s x10, f11, rmm\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f5, 736(x13)\n"
			"flw.ps f0, 992(x13)\n"
			"fle.s x26, f5, f0\n" 
			"la x10, rand_int_32\n"
			"flw.ps f11, 32(x10)\n"
			"fsatu8.pi f1, f11\n" 
			"la x5, rand_int_32\n"
			"flw.ps f7, 224(x5)\n"
			"flw.ps f11, 736(x5)\n"
			"fxor.pi f5, f7, f11\n" 
			"LBL_SEQID_1_M0_WRITE_ID_5_HID_9:\n"
			"maskand m0, m7, m6\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0xFF\n"
			"csrw tensor_error, zero\n" 
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_9\n"
			"addi x30, x30, 1\n"
			"la x10, rand_ieee754_32\n"
			"flw.ps f13, 768(x10)\n"
			"flw.ps f28, 544(x10)\n"
			"fsgnjx.s f23, f13, f28\n" 
			"fbci.ps f7, 0x622f3\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f13, 352(x26)\n"
			"fcvt.f16.ps f7, f13\n" 
			"la x10, rand_int_32\n"
			"flw.ps f9, 480(x10)\n"
			"flw.ps f28, 192(x10)\n"
			"fminu.pi f28, f9, f28\n" 
			"LBL_SEQID_2_M0_WRITE_ID_6_HID_9:\n"
			"maskor m6, m0, m7\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x13, rand_int_32\n"
			"flw.ps f6, 480(x13)\n"
			"flw.ps f8, 288(x13)\n"
			"feq.pi f5, f6, f8\n" 
			"LBL_SEQID_2_M0_WRITE_ID_7_HID_9:\n"
			"fltm.pi m0, f9, f7\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x13, rand_ieee754_32\n"
			"flw.ps f6, 672(x13)\n"
			"fsgnjn.s f7, f6, f6\n" 
			"LBL_SEQID_2_M0_WRITE_ID_8_HID_9:\n"
			"maskand m5, m0, m7\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x16, rand_ieee754_32\n"
			"flw.ps f16, 256(x16)\n"
			"flw.ps f5, 640(x16)\n"
			"fmax.ps f23, f16, f5\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f1, 288(x16)\n"
			"flw.ps f5, 576(x16)\n"
			"fsgnjx.s f1, f1, f5\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f28, 448(x11)\n"
			"flw.ps f6, 64(x11)\n"
			"flt.s x28, f28, f6\n" 
			"la x20, rand_int_32\n"
			"flw.ps f11, 352(x20)\n"
			"fpackrepb.pi f9, f11\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f1, 480(x10)\n"
			"flw.ps f5, 320(x10)\n"
			"feq.s x28, f1, f5\n" 
			"la x20, rand_int_32\n"
			"flw.ps f9, 320(x20)\n"
			"flw.ps f16, 896(x20)\n"
			"fmul.pi f7, f9, f16\n" 
			"LBL_SEQID_2_M0_WRITE_ID_9_HID_9:\n"
			"masknot m0, m6\n" 
			"mov.m.x m0, x0, 0xff\n"
			"li x5, CREDINC3\n"
			"ld x5, 0(x5)\n"
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
			"la x5, rand_int_32\n"
			"flw.ps f13, 416(x5)\n"
			"fpackreph.pi f13, f13\n" 
			"la x26, rand_int_32\n"
			"flw.ps f6, 640(x26)\n"
			"flw.ps f26, 768(x26)\n"
			"fsrl.pi f5, f6, f26\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f9, 0(x11)\n"
			"flw.ps f25, 352(x11)\n"
			"fsgnj.ps f28, f9, f25\n" 
			"LBL_SEQID_3_M0_WRITE_ID_10_HID_9:\n"
			"mov.m.x m6, x5, 0x31\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x5, rand_int_32\n"
			"flw.ps f26, 672(x5)\n"
			"flw.ps f16, 576(x5)\n"
			"fmin.pi f28, f26, f16\n" 
			"la x26, rand_int_32\n"
			"flw.ps f28, 576(x26)\n"
			"fsatu8.pi f1, f28\n" 
			"la x17, rand_int_32\n"
			"flw.ps f11, 960(x17)\n"
			"fsrai.pi f4, f11, 0xe\n" 
			"la x12, rand_int_32\n"
			"flw.ps f1, 640(x12)\n"
			"flw.ps f9, 384(x12)\n"
			"frem.pi f25, f1, f9\n" 
			"la x5, rand_int_32\n"
			"flw.ps f5, 192(x5)\n"
			"flw.ps f9, 96(x5)\n"
			"fminu.pi f13, f5, f9\n" 
			"la x11, rand_int_32\n"
			"flw.ps f26, 736(x11)\n"
			"flw.ps f6, 128(x11)\n"
			"fmax.pi f5, f26, f6\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f7, 160(x26)\n"
			"flw.ps f11, 320(x26)\n"
			"flw.ps f5, 640(x26)\n"
			"fmadd.s f7, f7, f11, f5, rup\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f5, 640(x16)\n"
			"flw.ps f23, 384(x16)\n"
			"fsgnjn.s f7, f5, f23\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f28, 928(x10)\n"
			"flw.ps f15, 832(x10)\n"
			"fmul.ps f9, f28, f15, rtz\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f5, 128(x16)\n"
			"flw.ps f7, 384(x16)\n"
			"fadd.s f4, f5, f7, rup\n" 
			"LBL_SEQID_3_FP_TRANS_ID_11_HID_9:\n"
			"la x13, rand_ieee754_32\n"
			"flw.ps f4, 768(x13)\n"
			"flog.ps f8, f4\n" 
			"la x28, rand_int_32\n"
			"flw.ps f0, 672(x28)\n"
			"fsrai.pi f28, f0, 0x0\n" 
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
			"fbci.ps f7, 0x60273\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f4, 256(x11)\n"
			"flw.ps f15, 672(x11)\n"
			"fmsub.s f8, f4, f15, f4, rup\n" 
			"la x5, rand_int_32\n"
			"flw.ps f6, 736(x5)\n"
			"flw.ps f8, 416(x5)\n"
			"fsrl.pi f8, f6, f8\n" 
			"li x16, 0x188caf764a403543\n"
			"fcvt.s.wu f0, x16, rdn\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f23, 864(x26)\n"
			"flw.ps f11, 128(x26)\n"
			"fmsub.ps f4, f23, f11, f23, rdn\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f11, 864(x20)\n"
			"fmul.ps f5, f11, f11, rup\n" 
			"la x16, rand_int_32\n"
			"flw.ps f6, 160(x16)\n"
			"flw.ps f5, 832(x16)\n"
			"fltu.pi f0, f6, f5\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f23, 288(x12)\n"
			"flw.ps f9, 576(x12)\n"
			"fadd.s f8, f23, f9, rdn\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f15, 320(x5)\n"
			"flw.ps f7, 896(x5)\n"
			"fdiv.ps f26, f15, f7, rup\n" 
			"la x20, rand_int_32\n"
			"flw.ps f6, 512(x20)\n"
			"flw.ps f5, 0(x20)\n"
			"fadd.pi f7, f6, f5\n" 
			"la x12, rand_int_32\n"
			"flw.ps f4, 512(x12)\n"
			"flw.ps f6, 320(x12)\n"
			"fsub.pi f1, f4, f6\n" 
			"la x23, rand_int_32\n"
			"flw.ps f1, 864(x23)\n"
			"fcvt.ps.pw f28, f1, rup\n" 
			"la x10, rand_int_32\n"
			"flw.ps f25, 64(x10)\n"
			"fpackrepb.pi f5, f25\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f15, 128(x28)\n"
			"fcvt.w.s x23, f15, rdn\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f13, 576(x28)\n"
			"flw.ps f23, 352(x28)\n"
			"fle.ps f26, f13, f23\n" 
			"la x10, rand_int_32\n"
			"flw.ps f4, 768(x10)\n"
			"flw.ps f6, 192(x10)\n"
			"fand.pi f7, f4, f6\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 5\n" 
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
			"csrwi mhpmevent3, 17\n"   
			"csrwi mhpmevent4, 28\n"   
			"csrwi mhpmevent5, 19\n"   
			"csrwi mhpmevent6, 16\n"   
			"csrwi mhpmevent7, 4\n"   
			"csrwi mhpmevent8, 7\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f0,  768 (x5)\n"
			"flw.ps f15,  320 (x5)\n"
			"flw.ps f28,  352 (x5)\n"
			"flw.ps f8,  448 (x5)\n"
			"flw.ps f19,  896 (x5)\n"
			"flw.ps f2,  544 (x5)\n"
			"flw.ps f24,  64 (x5)\n"
			"flw.ps f9,  0 (x5)\n"
			"flw.ps f29,  32 (x5)\n"
			"flw.ps f27,  640 (x5)\n"
			"flw.ps f14,  928 (x5)\n"
			"flw.ps f18,  480 (x5)\n"
			"flw.ps f17,  96 (x5)\n"
			"flw.ps f7,  736 (x5)\n"
			"flw.ps f16,  864 (x5)\n"
			"flw.ps f12,  608 (x5)\n"
			"flw.ps f21,  192 (x5)\n"
			"flw.ps f1,  288 (x5)\n"
			"flw.ps f22,  384 (x5)\n"
			"flw.ps f6,  800 (x5)\n"
			"flw.ps f5,  512 (x5)\n"
			"flw.ps f4,  224 (x5)\n"
			"flw.ps f10,  128 (x5)\n"
			"flw.ps f26,  672 (x5)\n"
			"flw.ps f30,  160 (x5)\n"
			"flw.ps f20,  416 (x5)\n"
			"flw.ps f11,  576 (x5)\n"
			"flw.ps f3,  960 (x5)\n"
			"flw.ps f13,  256 (x5)\n"
			"flw.ps f31,  704 (x5)\n"
			"flw.ps f23,  832 (x5)\n"
			"flw.ps f25,  992 (x5)\n"
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
			"li x5, 0x5\n" 
			"sw x5, (x31)\n"
			"li x29, 0x5\n"
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
			"la x28, rand_int_32\n"
			"flw.ps f4, 704(x28)\n"
			"flw.ps f27, 512(x28)\n"
			"fmulh.pi f20, f4, f27\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x9, rand_int_32\n"
			"flw.ps f30, 480(x9)\n"
			"flw.ps f14, 544(x9)\n"
			"for.pi f3, f30, f14\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f18, 544(x19)\n"
			"flw.ps f4, 960(x19)\n"
			"fsgnj.s f4, f18, f4\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x12, rand_ieee754_32\n"
			"flw.ps f27, 576(x12)\n"
			"fcvt.l.s x19, f27, rdn\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f12, 576(x18)\n"
			"fclass.ps f26, f12\n" 
			"la x20, rand_int_32\n"
			"flw.ps f27, 544(x20)\n"
			"flw.ps f20, 480(x20)\n"
			"fxor.pi f28, f27, f20\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f30, 32(x23)\n"
			"flw.ps f8, 416(x23)\n"
			"fsub.ps f27, f30, f8, rne\n" 
			"la x20, rand_int_32\n"
			"flw.ps f13, 352(x20)\n"
			"flw.ps f28, 992(x20)\n"
			"fmaxu.pi f13, f13, f28\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_10:\n"
			"maskand m2, m5, m5\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"mova.x.m x23\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f30, 448(x13)\n"
			"frsq.ps f27, f30\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f18, 416(x19)\n"
			"flw.ps f2, 320(x19)\n"
			"fsgnj.s f4, f18, f2\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f20, 928(x18)\n"
			"flw.ps f28, 256(x18)\n"
			"fsgnjx.ps f3, f20, f28\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f2, 192(x13)\n"
			"fsqrt.ps f7, f2\n" 
			"la x19, rand_int_32\n"
			"flw.ps f27, 512(x19)\n"
			"fsat8.pi f14, f27\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f8, 768(x5)\n"
			"flw.ps f18, 224(x5)\n"
			"fsub.ps f26, f8, f18, rtz\n" 
			"csrw tensor_error, zero\n" 
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=0); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ITER_0_HID_10:\n"
			"li x6, ((0x4 << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0x3 << TENSOR_QUANT_QUANT_ROW) | (0xa << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x0 << TENSOR_QUANT_TRANSF1) | (0x4 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_10\n"
			"csrw tensor_error, x0\n"
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_10\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_1_M0_WRITE_ID_1_HID_10:\n"
			"fltm.ps m1, f5, f10\n" 
			"li x5, 0x803401C0\n"
			"ld x5, 0(x5)\n"
			"la x9, rand_int_32\n"
			"flw.ps f12, 512(x9)\n"
			"flw.ps f8, 704(x9)\n"
			"fand.pi f2, f12, f8\n" 
			"la x28, rand_int_32\n"
			"flw.ps f27, 64(x28)\n"
			"fnot.pi f27, f27\n" 
			"la x16, rand_int_32\n"
			"flw.ps f27, 0(x16)\n"
			"flw.ps f28, 352(x16)\n"
			"fand.pi f30, f27, f28\n" 
			"la x13, rand_int_32\n"
			"flw.ps f27, 320(x13)\n"
			"flw.ps f5, 832(x13)\n"
			"fmax.pi f18, f27, f5\n" 
			"la x13, rand_int_32\n"
			"flw.ps f2, 704(x13)\n"
			"flw.ps f3, 96(x13)\n"
			"fmul.pi f10, f2, f3\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f26, 800(x13)\n"
			"flw.ps f28, 832(x13)\n"
			"fmin.s f4, f26, f28\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x18, rand_int_32\n"
			"flw.ps f4, 800(x18)\n"
			"flw.ps f28, 224(x18)\n"
			"for.pi f20, f4, f28\n" 
			"la x28, rand_int_32\n"
			"flw.ps f2, 480(x28)\n"
			"flw.ps f4, 416(x28)\n"
			"fxor.pi f5, f2, f4\n" 
			"la x12, rand_int_32\n"
			"flw.ps f20, 864(x12)\n"
			"fmax.pi f20, f20, f20\n" 
			"li x5, 0xdbc7\n"
			"csrw tensor_mask, x5\n"	
			"la x23, rand_ieee754_32\n"
			"flw.ps f13, 896(x23)\n"
			"fcvt.pw.ps f8, f13, rmm\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f10, 992(x9)\n"
			"flw.ps f3, 832(x9)\n"
			"flw.ps f14, 992(x9)\n"
			"fmsub.s f20, f10, f3, f14, rmm\n" 
			"LBL_SEQID_1_M0_WRITE_ID_2_HID_10:\n"
			"mov.m.x m2, x13, 0x76\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x7, rand_int_32\n"
			"flw.ps f30, 352(x7)\n"
			"fcvt.ps.pw f8, f30, rtz\n" 
			"la x18, rand_int_32\n"
			"flw.ps f3, 480(x18)\n"
			"fslli.pi f3, f3, 0x4\n" 
			"la x20, rand_int_32\n"
			"flw.ps f30, 288(x20)\n"
			"flw.ps f10, 416(x20)\n"
			"fadd.pi f20, f30, f10\n" 
			"csrw tensor_error, zero\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"LBL_SEQID_1_TLD_XMA_STR_SEQ_ID_252635_HID_10:\n"
			"la x31, self_check_8\n"
			"li x5, 0xc0000000000004\n"
			"add x5, x5, x31\n"
			"li x6, 0x32000000000000e\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x3 << TENSOR_IMA_B_NUM_COLS) | (0x4 << TENSOR_IMA_A_NUM_ROWS) | (0xe << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x19 << TENSOR_IMA_SCP_LOC_B) | (0x6 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
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
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ITER_0_HID_10:\n"
			"li x6, ((0x2 << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0xc << TENSOR_QUANT_QUANT_ROW) | (0x5 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ITER_1_HID_10:\n"
			"li x5, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x2 << TENSOR_FMA16_B_NUM_COLS) | (0x8 << TENSOR_FMA16_A_NUM_ROWS) | (0x4 << TENSOR_FMA16_A_NUM_COLS) | (0x6 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x97 << TENSOR_FMA16_SCP_LOC_B) | (0x74 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x6, ((0x19 << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0x8 << TENSOR_QUANT_QUANT_ROW) | (0x6 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x6 << TENSOR_QUANT_TRANSF6) | (0x6 << TENSOR_QUANT_TRANSF5) | (0x6 << TENSOR_QUANT_TRANSF4) | (0x6 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 7\n"
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_10\n"
			"csrw tensor_error, x0\n"
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_10\n"
			"addi x30, x30, 1\n"
			"la x12, rand_int_32\n"
			"flw.ps f2, 288(x12)\n"
			"fcvt.ps.pw f30, f2, rup\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f7, 864(x20)\n"
			"flw.ps f12, 480(x20)\n"
			"fsgnjn.s f5, f7, f12\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f5, 864(x5)\n"
			"flw.ps f27, 896(x5)\n"
			"flt.s x18, f5, f27\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f30, 512(x5)\n"
			"flw.ps f12, 0(x5)\n"
			"fsgnj.ps f14, f30, f12\n" 
			"la x7, rand_int_32\n"
			"flw.ps f18, 544(x7)\n"
			"fcvt.ps.pwu f12, f18, rmm\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f20, 160(x23)\n"
			"fclass.ps f10, f20\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f4, 576(x16)\n"
			"flw.ps f28, 608(x16)\n"
			"fcmovm.ps f2, f4, f28\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x5, rand_int_32\n"
			"flw.ps f2, 576(x5)\n"
			"fandi.pi f3, f2, 0xce\n" 
			"li x5, 0xb881\n"
			"csrw tensor_mask, x5\n"	
			"la x12, rand_int_32\n"
			"flw.ps f13, 896(x12)\n"
			"flw.ps f18, 448(x12)\n"
			"fsub.pi f10, f13, f18\n" 
			"la x23, rand_int_32\n"
			"flw.ps f27, 800(x23)\n"
			"flw.ps f13, 512(x23)\n"
			"fmax.pi f20, f27, f13\n" 
			"la x16, rand_int_32\n"
			"flw.ps f3, 64(x16)\n"
			"fpackreph.pi f5, f3\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f2, 256(x16)\n"
			"flw.ps f30, 0(x16)\n"
			"fsgnjn.ps f5, f2, f30\n" 
			"la x7, rand_int_32\n"
			"flw.ps f12, 448(x7)\n"
			"flw.ps f14, 416(x7)\n"
			"fmul.pi f27, f12, f14\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f20, 576(x5)\n"
			"flw.ps f8, 576(x5)\n"
			"feq.s x18, f20, f8\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f10, 224(x18)\n"
			"flw.ps f12, 352(x18)\n"
			"fle.ps f20, f10, f12\n" 
			"li x12, 0x750cffa7c4447e5b\n"
			"fcvt.s.wu f5, x12, rne\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 0x5bc8\n"
			"csrw tensor_mask, x5\n"	
			"LBL_SEQID_2_XMA_QNT_STR_SEQ_ID_788672_HID_10:\n"
			"li x6, ((0x0 << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0x1 << TENSOR_QUANT_QUANT_ROW) | (0x6 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x2 << TENSOR_QUANT_TRANSF8) | (0x7 << TENSOR_QUANT_TRANSF7) | (0x6 << TENSOR_QUANT_TRANSF6) | (0x6 << TENSOR_QUANT_TRANSF5) | (0x7 << TENSOR_QUANT_TRANSF4) | (0x7 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_10\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_2_XMA_QNT_STR_SEQ_ITER_0_HID_10:\n"
			"li x5, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x2 << TENSOR_FMA16_B_NUM_COLS) | (0xb << TENSOR_FMA16_A_NUM_ROWS) | (0x7 << TENSOR_FMA16_A_NUM_COLS) | (0xf << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0xe6 << TENSOR_FMA16_SCP_LOC_B) | (0x48 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x0 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
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
			"la x18, rand_int_32\n"
			"flw.ps f13, 736(x18)\n"
			"faddi.pi f26, f13, 0xaf\n" 
			"LBL_SEQID_3_M0_WRITE_ID_3_HID_10:\n"
			"fsetm.pi m2, f30\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"li x23, 0xca868e2ee90e586f\n"
			"fcvt.s.w f18, x23, rne\n" 
			"la x7, rand_int_32\n"
			"flw.ps f4, 320(x7)\n"
			"fsrai.pi f28, f4, 0xc\n" 
			"la x5, rand_int_32\n"
			"flw.ps f2, 448(x5)\n"
			"flw.ps f5, 0(x5)\n"
			"feq.pi f18, f2, f5\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x16, rand_ieee754_32\n"
			"flw.ps f5, 416(x16)\n"
			"flw.ps f13, 576(x16)\n"
			"flt.s x7, f5, f13\n" 
			"LBL_SEQID_3_FP_TRANS_ID_4_HID_10:\n"
			"la x18, rand_ieee754_32\n"
			"flw.ps f3, 256(x18)\n"
			"frcp.ps f30, f3\n" 
			"mova.x.m x16\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f20, 800(x13)\n"
			"flw.ps f26, 864(x13)\n"
			"fmin.ps f30, f20, f26\n" 
			"fbci.ps f30, 0x1b092\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f28, 896(x18)\n"
			"fround.ps f8, f28, rne\n" 
			"li x5, 0x428882f386657b8\n"
			"fcvt.s.wu f2, x5, rtz\n" 
			"LBL_SEQID_3_M0_WRITE_ID_5_HID_10:\n"
			"feqm.ps m2, f2, f14\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"li x5, 0xef6c\n"
			"csrw tensor_mask, x5\n"	
			"la x28, rand_ieee754_32\n"
			"flw.ps f3, 512(x28)\n"
			"flw.ps f14, 608(x28)\n"
			"fsub.s f14, f3, f14, rne\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f13, 672(x5)\n"
			"flw.ps f4, 960(x5)\n"
			"fadd.s f30, f13, f4, rdn\n" 
			"la x23, rand_int_32\n"
			"flw.ps f2, 160(x23)\n"
			"fsat8.pi f3, f2\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_3_XMA_QNT_STR_SEQ_ITER_0_HID_10:\n"
			"li x6, ((0x14 << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0x9 << TENSOR_QUANT_QUANT_ROW) | (0x1b << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x1 << TENSOR_QUANT_TRANSF2) | (0x9 << TENSOR_QUANT_TRANSF1) | (0x4 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 10\n"
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
			"la x13, rand_ieee754_32\n"
			"flw.ps f20, 160(x13)\n"
			"flw.ps f13, 0(x13)\n"
			"fmin.s f5, f20, f13\n" 
			"la x7, rand_int_32\n"
			"flw.ps f5, 800(x7)\n"
			"flw.ps f28, 832(x7)\n"
			"for.pi f28, f5, f28\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f5, 896(x9)\n"
			"flw.ps f8, 416(x9)\n"
			"fsub.ps f5, f5, f8, rdn\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f20, 128(x9)\n"
			"flw.ps f26, 320(x9)\n"
			"fsub.s f10, f20, f26, rup\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f30, 224(x5)\n"
			"flw.ps f3, 512(x5)\n"
			"fsgnj.ps f7, f30, f3\n" 
			"la x18, rand_int_32\n"
			"flw.ps f28, 288(x18)\n"
			"for.pi f12, f28, f28\n" 
			"la x19, rand_int_32\n"
			"flw.ps f13, 416(x19)\n"
			"flw.ps f10, 736(x19)\n"
			"feq.pi f10, f13, f10\n" 
			"la x20, rand_int_32\n"
			"flw.ps f30, 352(x20)\n"
			"flw.ps f28, 160(x20)\n"
			"fltu.pi f5, f30, f28\n" 
			"la x18, rand_int_32\n"
			"flw.ps f3, 160(x18)\n"
			"flw.ps f10, 672(x18)\n"
			"for.pi f8, f3, f10\n" 
			"fbci.pi f12, 0x6c011\n" 
			"la x16, rand_int_32\n"
			"flw.ps f20, 544(x16)\n"
			"fpackreph.pi f27, f20\n" 
			"la x23, rand_int_32\n"
			"flw.ps f27, 288(x23)\n"
			"faddi.pi f12, f27, 0x91\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f10, 672(x9)\n"
			"fle.ps f14, f10, f10\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f7, 288(x5)\n"
			"flw.ps f18, 384(x5)\n"
			"flw.ps f3, 992(x5)\n"
			"fnmadd.ps f3, f7, f18, f3, rdn\n" 
			"LBL_SEQID_4_M0_WRITE_ID_6_HID_10:\n"
			"flem.ps m0, f4, f13\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_4_FP_TRANS_ID_7_HID_10:\n"
			"la x12, rand_ieee754_32\n"
			"flw.ps f12, 640(x12)\n"
			"fexp.ps f20, f12\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_4_TLD_XMA_STR_SEQ_ID_34250_HID_10:\n"
			"la x31, self_check_16\n"
			"li x5, 0xa\n"
			"add x5, x5, x31\n"
			"li x6, 0x320000000000007\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x1 << TENSOR_FMA16_B_NUM_COLS) | (0xa << TENSOR_FMA16_A_NUM_ROWS) | (0x7 << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x19 << TENSOR_FMA16_SCP_LOC_B) | (0x0 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
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
			"bne x5, x31, LBL_FAIL_HID_10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_10\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_4_XMA_QNT_STR_SEQ_ITER_0_HID_10:\n"
			"li x5, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x1 << TENSOR_FMA16_B_NUM_COLS) | (0x0 << TENSOR_FMA16_A_NUM_ROWS) | (0x4 << TENSOR_FMA16_A_NUM_COLS) | (0x1 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0xa << TENSOR_FMA16_SCP_LOC_B) | (0xc2 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x0 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_10\n"
			"csrw tensor_error, x0\n"
			"li x5, 5\n" 
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
			"csrwi mhpmevent3, 4\n"   
			"csrwi mhpmevent4, 8\n"   
			"csrwi mhpmevent5, 6\n"   
			"csrwi mhpmevent6, 9\n"   
			"csrwi mhpmevent7, 21\n"   
			"csrwi mhpmevent8, 12\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f24,  96 (x5)\n"
			"flw.ps f31,  960 (x5)\n"
			"flw.ps f15,  768 (x5)\n"
			"flw.ps f10,  800 (x5)\n"
			"flw.ps f7,  64 (x5)\n"
			"flw.ps f19,  992 (x5)\n"
			"flw.ps f13,  544 (x5)\n"
			"flw.ps f6,  928 (x5)\n"
			"flw.ps f14,  704 (x5)\n"
			"flw.ps f0,  320 (x5)\n"
			"flw.ps f29,  864 (x5)\n"
			"flw.ps f17,  32 (x5)\n"
			"flw.ps f16,  896 (x5)\n"
			"flw.ps f25,  448 (x5)\n"
			"flw.ps f30,  0 (x5)\n"
			"flw.ps f26,  832 (x5)\n"
			"flw.ps f4,  416 (x5)\n"
			"flw.ps f28,  512 (x5)\n"
			"flw.ps f9,  192 (x5)\n"
			"flw.ps f23,  288 (x5)\n"
			"flw.ps f3,  576 (x5)\n"
			"flw.ps f21,  128 (x5)\n"
			"flw.ps f18,  736 (x5)\n"
			"flw.ps f12,  672 (x5)\n"
			"flw.ps f5,  224 (x5)\n"
			"flw.ps f2,  384 (x5)\n"
			"flw.ps f1,  352 (x5)\n"
			"flw.ps f22,  160 (x5)\n"
			"flw.ps f27,  256 (x5)\n"
			"flw.ps f20,  480 (x5)\n"
			"flw.ps f8,  640 (x5)\n"
			"flw.ps f11,  608 (x5)\n"
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
			"li x5, 0x5\n" 
			"sw x5, (x31)\n"
			"li x29, 0x5\n"
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
			"la x26, rand_ieee754_32\n"
			"flw.ps f20, 608(x26)\n"
			"fclass.s x26, f20\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f2, 416(x16)\n"
			"flw.ps f17, 160(x16)\n"
			"fsub.ps f20, f2, f17, rdn\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f20, 352(x27)\n"
			"flw.ps f0, 896(x27)\n"
			"fmsub.s f2, f20, f0, f0, rne\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_11:\n"
			"maskxor m7, m0, m6\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x26, rand_ieee754_32\n"
			"flw.ps f27, 0(x26)\n"
			"flw.ps f1, 608(x26)\n"
			"fsgnjx.ps f1, f27, f1\n" 
			"fbci.ps f17, 0x6637c\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f30, 608(x27)\n"
			"flw.ps f12, 0(x27)\n"
			"fsgnjn.s f17, f30, f12\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f13, 64(x23)\n"
			"flw.ps f30, 352(x23)\n"
			"fle.s x15, f13, f30\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_11:\n"
			"maskxor m0, m2, m0\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x15, rand_ieee754_32\n"
			"flw.ps f16, 64(x15)\n"
			"fcvt.pwu.ps f4, f16, rne\n" 
			"la x10, rand_int_32\n"
			"flw.ps f2, 384(x10)\n"
			"fsrli.pi f17, f2, 0x2\n" 
			"LBL_SEQID_0_M0_WRITE_ID_2_HID_11:\n"
			"feqm.ps m7, f27, f1\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0x387e00e11a\n"
			"flw.ps f5, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x16, rand_ieee754_32\n"
			"flw.ps f1, 448(x16)\n"
			"fcvt.f16.ps f13, f1\n" 
			"la x27, rand_int_32\n"
			"flw.ps f1, 800(x27)\n"
			"fmul.pi f5, f1, f1\n" 
			"LBL_SEQID_0_M0_WRITE_ID_3_HID_11:\n"
			"mov.m.x m0, x14, 0xc\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x14, rand_ieee754_32\n"
			"flw.ps f1, 864(x14)\n"
			"fsin.ps f10, f1\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_11\n"
			"addi x30, x30, 1\n"
			"la x23, rand_ieee754_32\n"
			"flw.ps f12, 672(x23)\n"
			"flw.ps f30, 320(x23)\n"
			"fnmsub.ps f12, f12, f12, f30, rtz\n" 
			"maskpopcz x10, m0\n" 
			"mov.m.x m0, x0, 0xff\n"
			"la x10, rand_ieee754_32\n"
			"flw.ps f0, 192(x10)\n"
			"flw.ps f22, 32(x10)\n"
			"flw.ps f10, 960(x10)\n"
			"fmsub.s f10, f0, f22, f10, rmm\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f10, 768(x17)\n"
			"flw.ps f27, 480(x17)\n"
			"fadd.ps f13, f10, f27, rup\n" 
			"la x17, rand_int_32\n"
			"flw.ps f22, 576(x17)\n"
			"fpackrepb.pi f5, f22\n" 
			"LBL_SEQID_1_M0_WRITE_ID_4_HID_11:\n"
			"fsetm.pi m6, f28\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x17, rand_ieee754_32\n"
			"flw.ps f0, 416(x17)\n"
			"flw.ps f2, 960(x17)\n"
			"fle.s x14, f0, f2\n" 
			"la x28, rand_int_32\n"
			"flw.ps f14, 736(x28)\n"
			"flw.ps f27, 448(x28)\n"
			"feq.pi f4, f14, f27\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f22, 960(x16)\n"
			"flw.ps f13, 256(x16)\n"
			"fsgnj.s f1, f22, f13\n" 
			"la x15, rand_int_32\n"
			"flw.ps f17, 32(x15)\n"
			"fminu.pi f12, f17, f17\n" 
			"la x28, rand_int_32\n"
			"flw.ps f17, 800(x28)\n"
			"flw.ps f2, 32(x28)\n"
			"fxor.pi f2, f17, f2\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f28, 256(x26)\n"
			"fclass.s x15, f28\n" 
			"la x17, rand_int_32\n"
			"flw.ps f17, 704(x17)\n"
			"fsat8.pi f27, f17\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f27, 544(x15)\n"
			"fcvt.w.s x26, f27, rne\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f12, 160(x14)\n"
			"fcvt.l.s x23, f12, rmm\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f2, 256(x23)\n"
			"flw.ps f27, 384(x23)\n"
			"fsub.ps f16, f2, f27, rne\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_11\n"
			"addi x30, x30, 1\n"
			"mova.x.m x10\n" 
			"LBL_SEQID_2_M0_WRITE_ID_5_HID_11:\n"
			"mov.m.x m7, x25, 0x73\n" 
			"li x5, CREDINC1\n"
			"ld x5, 0(x5)\n"
			"la x10, rand_int_32\n"
			"flw.ps f4, 320(x10)\n"
			"faddi.pi f2, f4, 0x13c\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f28, 352(x27)\n"
			"flw.ps f10, 608(x27)\n"
			"fsub.s f28, f28, f10, rne\n" 
			"LBL_SEQID_2_M0_WRITE_ID_6_HID_11:\n"
			"masknot m0, m0\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_2_FP_TRANS_ID_7_HID_11:\n"
			"la x25, rand_ieee754_32\n"
			"flw.ps f4, 896(x25)\n"
			"fexp.ps f5, f4\n" 
			"la x14, rand_int_32\n"
			"flw.ps f4, 160(x14)\n"
			"feq.pi f4, f4, f4\n" 
			"LBL_SEQID_2_M0_WRITE_ID_8_HID_11:\n"
			"mova.m.x x23\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x26, rand_ieee754_32\n"
			"flw.ps f28, 96(x26)\n"
			"flw.ps f12, 448(x26)\n"
			"fmadd.ps f4, f28, f12, f12, rdn\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f30, 128(x17)\n"
			"flw.ps f0, 736(x17)\n"
			"fmin.ps f27, f30, f0\n" 
			"la x27, rand_int_32\n"
			"flw.ps f13, 384(x27)\n"
			"flw.ps f12, 928(x27)\n"
			"fsll.pi f16, f13, f12\n" 
			"la x16, rand_int_32\n"
			"flw.ps f27, 960(x16)\n"
			"flw.ps f1, 736(x16)\n"
			"feq.pi f5, f27, f1\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f17, 608(x23)\n"
			"flw.ps f16, 32(x23)\n"
			"flw.ps f30, 352(x23)\n"
			"fnmsub.s f13, f17, f16, f30, rtz\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f10, 992(x25)\n"
			"fcvt.wu.s x25, f10, rmm\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f20, 960(x17)\n"
			"flw.ps f5, 576(x17)\n"
			"flt.s x26, f20, f5\n" 
			"la x15, rand_int_32\n"
			"flw.ps f4, 512(x15)\n"
			"fsrli.pi f27, f4, 0xe\n" 
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
			"la x16, rand_ieee754_32\n"
			"flw.ps f0, 608(x16)\n"
			"flw.ps f4, 352(x16)\n"
			"fnmadd.s f27, f0, f4, f4, rup\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f28, 256(x14)\n"
			"fcvt.f16.ps f13, f28\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f5, 448(x10)\n"
			"flw.ps f2, 416(x10)\n"
			"fnmadd.ps f17, f5, f2, f5, rup\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f12, 608(x10)\n"
			"flw.ps f10, 416(x10)\n"
			"flw.ps f30, 704(x10)\n"
			"fnmsub.s f17, f12, f10, f30, rtz\n" 
			"la x15, rand_int_32\n"
			"flw.ps f27, 800(x15)\n"
			"fcvt.ps.pw f28, f27, rdn\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f22, 544(x25)\n"
			"flw.ps f12, 64(x25)\n"
			"fdiv.s f0, f22, f12, rne\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f16, 128(x17)\n"
			"flw.ps f2, 928(x17)\n"
			"fmax.s f28, f16, f2\n" 
			"la x17, rand_int_32\n"
			"flw.ps f10, 288(x17)\n"
			"fsll.pi f0, f10, f10\n" 
			"la x23, rand_int_32\n"
			"flw.ps f13, 64(x23)\n"
			"fandi.pi f4, f13, 0xd3\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f20, 704(x17)\n"
			"flw.ps f12, 896(x17)\n"
			"fmax.ps f14, f20, f12\n" 
			"li x28, 0x97611a7ee8b5ce3b\n"
			"fcvt.s.wu f10, x28, rtz\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f13, 864(x15)\n"
			"fmul.ps f28, f13, f13, rtz\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f30, 256(x28)\n"
			"flw.ps f4, 128(x28)\n"
			"fmul.ps f0, f30, f4, rup\n" 
			"maskpopcz x14, m6\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f27, 288(x27)\n"
			"flw.ps f22, 896(x27)\n"
			"flw.ps f2, 608(x27)\n"
			"fmadd.s f0, f27, f22, f2, rmm\n" 
			"la x16, rand_int_32\n"
			"flw.ps f28, 160(x16)\n"
			"fcvt.ps.pw f20, f28, rup\n" 
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
			"la x23, rand_ieee754_32\n"
			"flw.ps f30, 32(x23)\n"
			"flw.ps f1, 224(x23)\n"
			"fsub.s f22, f30, f1, rup\n" 
			"LBL_SEQID_4_M0_WRITE_ID_9_HID_11:\n"
			"flem.ps m2, f16, f22\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_4_FP_TRANS_ID_10_HID_11:\n"
			"la x16, rand_ieee754_32\n"
			"flw.ps f14, 992(x16)\n"
			"frcp.ps f17, f14\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f14, 640(x16)\n"
			"flw.ps f2, 64(x16)\n"
			"fmax.ps f10, f14, f2\n" 
			"LBL_SEQID_4_M0_WRITE_ID_11_HID_11:\n"
			"maskxor m0, m2, m0\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x23, rand_ieee754_32\n"
			"flw.ps f17, 576(x23)\n"
			"fswizz.ps f16, f17, 0x9\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f1, 672(x17)\n"
			"fround.ps f22, f1, rtz\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f10, 128(x15)\n"
			"fsgnjx.s f14, f10, f10\n" 
			"LBL_SEQID_4_M0_WRITE_ID_12_HID_11:\n"
			"fltm.pi m6, f12, f30\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x16, rand_ieee754_32\n"
			"flw.ps f22, 736(x16)\n"
			"flw.ps f0, 704(x16)\n"
			"fcmovm.ps f27, f22, f0\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f14, 128(x15)\n"
			"flw.ps f10, 864(x15)\n"
			"fmadd.ps f2, f14, f14, f10, rdn\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f14, 608(x26)\n"
			"flw.ps f30, 448(x26)\n"
			"fadd.s f5, f14, f30, rdn\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f10, 256(x17)\n"
			"flw.ps f16, 992(x17)\n"
			"fmin.ps f14, f10, f16\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f14, 960(x16)\n"
			"flw.ps f13, 448(x16)\n"
			"fsgnjx.ps f14, f14, f13\n" 
			"la x23, rand_int_32\n"
			"flw.ps f17, 864(x23)\n"
			"flw.ps f1, 320(x23)\n"
			"feq.pi f0, f17, f1\n" 
			"LBL_SEQID_4_M0_WRITE_ID_13_HID_11:\n"
			"masknot m7, m6\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"csrw tensor_error, zero\n" 
			"li x5, 5\n" 
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
			"csrwi mhpmevent3, 17\n"   
			"csrwi mhpmevent4, 28\n"   
			"csrwi mhpmevent5, 19\n"   
			"csrwi mhpmevent6, 16\n"   
			"csrwi mhpmevent7, 4\n"   
			"csrwi mhpmevent8, 7\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f1,  448 (x5)\n"
			"flw.ps f25,  512 (x5)\n"
			"flw.ps f15,  480 (x5)\n"
			"flw.ps f27,  896 (x5)\n"
			"flw.ps f12,  0 (x5)\n"
			"flw.ps f4,  736 (x5)\n"
			"flw.ps f16,  192 (x5)\n"
			"flw.ps f20,  64 (x5)\n"
			"flw.ps f14,  704 (x5)\n"
			"flw.ps f26,  960 (x5)\n"
			"flw.ps f30,  160 (x5)\n"
			"flw.ps f23,  96 (x5)\n"
			"flw.ps f3,  992 (x5)\n"
			"flw.ps f0,  576 (x5)\n"
			"flw.ps f7,  384 (x5)\n"
			"flw.ps f22,  288 (x5)\n"
			"flw.ps f8,  864 (x5)\n"
			"flw.ps f10,  768 (x5)\n"
			"flw.ps f9,  256 (x5)\n"
			"flw.ps f13,  544 (x5)\n"
			"flw.ps f2,  832 (x5)\n"
			"flw.ps f29,  800 (x5)\n"
			"flw.ps f5,  416 (x5)\n"
			"flw.ps f11,  32 (x5)\n"
			"flw.ps f17,  672 (x5)\n"
			"flw.ps f24,  352 (x5)\n"
			"flw.ps f31,  224 (x5)\n"
			"flw.ps f19,  608 (x5)\n"
			"flw.ps f6,  128 (x5)\n"
			"flw.ps f18,  640 (x5)\n"
			"flw.ps f21,  928 (x5)\n"
			"flw.ps f28,  320 (x5)\n"
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
			"li x5, 0x5\n" 
			"sw x5, (x31)\n"
			"li x29, 0x5\n"
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
			"la x27, rand_int_32\n"
			"flw.ps f8, 128(x27)\n"
			"fsll.pi f14, f8, f8\n" 
			"la x26, rand_int_32\n"
			"flw.ps f23, 288(x26)\n"
			"fandi.pi f0, f23, 0xf6\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_12:\n"
			"mova.m.x x23\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x17, rand_int_32\n"
			"flw.ps f19, 544(x17)\n"
			"flw.ps f1, 128(x17)\n"
			"fand.pi f8, f19, f1\n" 
			"la x14, rand_int_32\n"
			"flw.ps f19, 640(x14)\n"
			"fpackreph.pi f14, f19\n" 
			"la x20, rand_int_32\n"
			"flw.ps f23, 384(x20)\n"
			"flw.ps f27, 960(x20)\n"
			"fmaxu.pi f27, f23, f27\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f15, 864(x28)\n"
			"flw.ps f27, 32(x28)\n"
			"fmax.ps f22, f15, f27\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x17, rand_ieee754_32\n"
			"flw.ps f2, 768(x17)\n"
			"flw.ps f28, 608(x17)\n"
			"fsgnj.s f10, f2, f28\n" 
			"LBL_SEQID_0_FP_TRANS_ID_1_HID_12:\n"
			"la x26, rand_ieee754_32\n"
			"flw.ps f1, 96(x26)\n"
			"flog.ps f1, f1\n" 
			"la x10, rand_int_32\n"
			"flw.ps f27, 352(x10)\n"
			"flw.ps f8, 288(x10)\n"
			"fle.pi f18, f27, f8\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f21, 512(x28)\n"
			"flw.ps f15, 64(x28)\n"
			"flw.ps f27, 224(x28)\n"
			"fmadd.s f14, f21, f15, f27, rne\n" 
			"la x20, rand_int_32\n"
			"flw.ps f23, 96(x20)\n"
			"fcvt.ps.pw f22, f23, rtz\n" 
			"la x20, rand_int_32\n"
			"flw.ps f30, 608(x20)\n"
			"fmin.pi f19, f30, f30\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f2, 288(x27)\n"
			"flw.ps f8, 288(x27)\n"
			"fmax.ps f19, f2, f8\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x26, rand_ieee754_32\n"
			"flw.ps f1, 224(x26)\n"
			"flw.ps f28, 416(x26)\n"
			"fmax.ps f1, f1, f28\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f21, 704(x23)\n"
			"fcvt.pwu.ps f18, f21, rtz\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ID_128499_HID_12:\n"
			"li x6, ((0x1c << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0x8 << TENSOR_QUANT_QUANT_ROW) | (0x1e << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_12\n"
			"csrw tensor_error, x0\n"
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=0); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ITER_0_HID_12:\n"
			"la x31, self_check_32\n"
			"li x5, 0x80000000000002\n"
			"add x5, x5, x31\n"
			"li x6, 0x3c0000000000001\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x3 << TENSOR_FMA32_B_NUM_COLS) | (0x2 << TENSOR_FMA32_A_NUM_ROWS) | (0x1 << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1e << TENSOR_FMA32_SCP_LOC_B) | (0x4 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x9, ((0xa << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0x2 << TENSOR_QUANT_QUANT_ROW) | (0x6 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x6 << TENSOR_QUANT_TRANSF6) | (0x6 << TENSOR_QUANT_TRANSF5) | (0x7 << TENSOR_QUANT_TRANSF4) | (0x7 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0x2\n"
			"bne x5, x31, LBL_FAIL_HID_12\n"
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_12\n"
			"csrw tensor_error, x0\n"
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_12\n"
			"addi x30, x30, 1\n"
			"fbci.pi f0, 0x4210\n" 
			"la x5, rand_int_32\n"
			"flw.ps f23, 992(x5)\n"
			"fsrli.pi f14, f23, 0x0\n" 
			"LBL_SEQID_1_M0_WRITE_ID_2_HID_12:\n"
			"maskxor m0, m5, m7\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x10, rand_ieee754_32\n"
			"flw.ps f22, 960(x10)\n"
			"flw.ps f2, 352(x10)\n"
			"feq.ps f0, f22, f2\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f0, 192(x20)\n"
			"flw.ps f23, 416(x20)\n"
			"fsub.ps f18, f0, f23, rmm\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f2, 256(x23)\n"
			"ffrc.ps f30, f2\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x10, rand_int_32\n"
			"flw.ps f28, 960(x10)\n"
			"flw.ps f8, 576(x10)\n"
			"flt.pi f23, f28, f8\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f2, 0(x17)\n"
			"flw.ps f16, 352(x17)\n"
			"fmin.ps f8, f2, f16\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f27, 0(x10)\n"
			"flw.ps f0, 672(x10)\n"
			"fadd.s f15, f27, f0, rup\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f10, 864(x23)\n"
			"flw.ps f19, 384(x23)\n"
			"flw.ps f23, 448(x23)\n"
			"fmadd.s f1, f10, f19, f23, rdn\n" 
			"mova.x.m x10\n" 
			"li x5, 0x635c\n"
			"csrw tensor_mask, x5\n"	
			"la x23, rand_ieee754_32\n"
			"flw.ps f19, 672(x23)\n"
			"flw.ps f2, 736(x23)\n"
			"flw.ps f1, 384(x23)\n"
			"fmadd.s f1, f19, f2, f1, rne\n" 
			"la x16, rand_int_32\n"
			"flw.ps f30, 704(x16)\n"
			"flw.ps f8, 544(x16)\n"
			"fxor.pi f28, f30, f8\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f21, 320(x23)\n"
			"flw.ps f15, 96(x23)\n"
			"fsub.ps f22, f21, f15, rup\n" 
			"LBL_SEQID_1_M0_WRITE_ID_3_HID_12:\n"
			"feqm.ps m5, f16, f2\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0x9745ba0755\n"
			"fsw.ps f0, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x14, rand_int_32\n"
			"flw.ps f30, 192(x14)\n"
			"flw.ps f10, 960(x14)\n"
			"fltu.pi f27, f30, f10\n" 
			"li x5, 0x9fec8e4e44d82ea0\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0xbce52cb9f2abcda9\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"csrw tensor_error, zero\n" 
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=0); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"LBL_SEQID_1_TLD_XMA_STR_SEQ_ITER_0_HID_12:\n"
			"la x31, self_check_8\n"
			"li x5, 0xe000000000000e\n"
			"add x5, x5, x31\n"
			"li x6, 0x300000000000005\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x1 << TENSOR_IMA_B_NUM_COLS) | (0xe << TENSOR_IMA_A_NUM_ROWS) | (0x5 << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x18 << TENSOR_IMA_SCP_LOC_B) | (0x7 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
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
			"bne x5, x31, LBL_FAIL_HID_12\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_12\n"
			"csrw tensor_error, x0\n"
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_12\n"
			"addi x30, x30, 1\n"
			"la x20, rand_int_32\n"
			"flw.ps f23, 256(x20)\n"
			"flw.ps f22, 768(x20)\n"
			"fsrl.pi f1, f23, f22\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f0, 160(x26)\n"
			"flw.ps f15, 96(x26)\n"
			"fmul.ps f1, f0, f15, rtz\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f16, 736(x28)\n"
			"flw.ps f8, 32(x28)\n"
			"fmax.ps f10, f16, f8\n" 
			"li x28, 0xeff44ed55f41876a\n"
			"fcvt.s.wu f22, x28, rne\n" 
			"LBL_SEQID_2_M0_WRITE_ID_4_HID_12:\n"
			"maskor m5, m7, m7\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x11, rand_ieee754_32\n"
			"flw.ps f10, 768(x11)\n"
			"flw.ps f16, 800(x11)\n"
			"fmax.ps f8, f10, f16\n" 
			"fbci.ps f0, 0x3946a\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f15, 256(x10)\n"
			"flw.ps f2, 768(x10)\n"
			"flw.ps f16, 640(x10)\n"
			"fmadd.s f22, f15, f2, f16, rup\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f16, 384(x20)\n"
			"flw.ps f0, 736(x20)\n"
			"fsub.s f15, f16, f0, rmm\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f28, 96(x17)\n"
			"flw.ps f30, 352(x17)\n"
			"fmul.ps f22, f28, f30, rtz\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f16, 32(x17)\n"
			"flw.ps f27, 448(x17)\n"
			"fcmovm.ps f8, f16, f27\n" 
			"la x16, rand_int_32\n"
			"flw.ps f21, 896(x16)\n"
			"flw.ps f1, 832(x16)\n"
			"fsll.pi f16, f21, f1\n" 
			"la x10, rand_int_32\n"
			"flw.ps f18, 96(x10)\n"
			"faddi.pi f18, f18, 0x133\n" 
			"la x11, rand_int_32\n"
			"flw.ps f14, 896(x11)\n"
			"flw.ps f22, 640(x11)\n"
			"flt.pi f16, f14, f22\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f22, 256(x26)\n"
			"fclass.ps f27, f22\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f0, 512(x5)\n"
			"fadd.ps f2, f0, f0, rmm\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_2_TLD_XMA_STR_SEQ_ITER_0_HID_12:\n"
			"la x31, self_check_16\n"
			"li x5, 0x4000000000000a\n"
			"add x5, x5, x31\n"
			"li x6, 0x10000000000002\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x3 << TENSOR_FMA16_B_NUM_COLS) | (0xa << TENSOR_FMA16_A_NUM_ROWS) | (0x2 << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x1 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1c << TENSOR_FMA16_SCP_LOC_B) | (0x2 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x17 << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0xa << TENSOR_QUANT_QUANT_ROW) | (0x20 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x6 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0x6\n"
			"bne x5, x31, LBL_FAIL_HID_12\n"
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 10\n"
			"LBL_SEQID_2_TLD_XMA_STR_SEQ_ITER_1_HID_12:\n"
			"la x31, self_check_8\n"
			"li x5, 0x6000000000000b\n"
			"add x5, x5, x31\n"
			"li x6, 0x1000000000000a\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x3 << TENSOR_IMA_B_NUM_COLS) | (0xb << TENSOR_IMA_A_NUM_ROWS) | (0xa << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x1 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1d << TENSOR_IMA_SCP_LOC_B) | (0x3 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fmvz.x.ps x31, f0, 0\n"
			"li x5, 0x2c\n"
			"bne x5, x31, LBL_FAIL_HID_12\n"
			"LBL_SEQID_2_TLD_XMA_STR_SEQ_ITER_2_HID_12:\n"
			"la x31, self_check_8\n"
			"li x5, 0x20000000000007\n"
			"add x5, x5, x31\n"
			"li x6, 0x10000000000000\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x0 << TENSOR_IMA_B_NUM_COLS) | (0x7 << TENSOR_IMA_A_NUM_ROWS) | (0x0 << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x1 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1b << TENSOR_IMA_SCP_LOC_B) | (0x1 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fmvz.x.ps x31, f0, 0\n"
			"li x5, 0x4\n"
			"bne x5, x31, LBL_FAIL_HID_12\n"
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
			"la x26, rand_ieee754_32\n"
			"flw.ps f18, 128(x26)\n"
			"fcvt.wu.s x28, f18, rtz\n" 
			"li x5, 0x14beb6753a2a33e\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0x2fab862cc014de04\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"la x11, rand_int_32\n"
			"flw.ps f1, 768(x11)\n"
			"fsrai.pi f0, f1, 0xa\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x27, rand_ieee754_32\n"
			"flw.ps f2, 576(x27)\n"
			"flw.ps f16, 32(x27)\n"
			"fadd.ps f8, f2, f16, rdn\n" 
			"la x28, rand_int_32\n"
			"flw.ps f14, 896(x28)\n"
			"flw.ps f28, 864(x28)\n"
			"fand.pi f8, f14, f28\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x23, rand_int_32\n"
			"flw.ps f18, 800(x23)\n"
			"fcvt.ps.pwu f0, f18, rup\n" 
			"la x10, rand_int_32\n"
			"flw.ps f21, 224(x10)\n"
			"flw.ps f14, 192(x10)\n"
			"fxor.pi f18, f21, f14\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f28, 32(x17)\n"
			"flw.ps f8, 544(x17)\n"
			"flw.ps f14, 96(x17)\n"
			"fmsub.s f10, f28, f8, f14, rup\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f10, 672(x5)\n"
			"feq.s x10, f10, f10\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f0, 320(x10)\n"
			"fcvt.wu.s x27, f0, rdn\n" 
			"la x23, rand_int_32\n"
			"flw.ps f18, 992(x23)\n"
			"fnot.pi f2, f18\n" 
			"LBL_SEQID_3_M0_WRITE_ID_5_HID_12:\n"
			"feqm.ps m5, f19, f18\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x14, rand_ieee754_32\n"
			"flw.ps f21, 672(x14)\n"
			"flw.ps f1, 640(x14)\n"
			"fsub.ps f10, f21, f1, rne\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f1, 544(x14)\n"
			"flw.ps f8, 576(x14)\n"
			"flw.ps f15, 800(x14)\n"
			"fmadd.ps f14, f1, f8, f15, rup\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f28, 832(x16)\n"
			"flw.ps f23, 576(x16)\n"
			"flw.ps f1, 704(x16)\n"
			"fmadd.ps f28, f28, f23, f1, rmm\n" 
			"la x11, rand_int_32\n"
			"flw.ps f16, 288(x11)\n"
			"flw.ps f15, 608(x11)\n"
			"fsra.pi f15, f16, f15\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f27, 608(x20)\n"
			"fround.ps f2, f27, rmm\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 0x5492\n"
			"csrw tensor_mask, x5\n"	
			"LBL_SEQID_3_XMA_QNT_STR_SEQ_ITER_0_HID_12:\n"
			"li x6, ((0xf << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0xf << TENSOR_QUANT_QUANT_ROW) | (0x2a << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0xa << TENSOR_QUANT_TRANSF3) | (0x9 << TENSOR_QUANT_TRANSF2) | (0x9 << TENSOR_QUANT_TRANSF1) | (0xa << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_3_XMA_QNT_STR_SEQ_ITER_1_HID_12:\n"
			"li x6, ((0xc << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0x8 << TENSOR_QUANT_QUANT_ROW) | (0x16 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x6 << TENSOR_QUANT_TRANSF6) | (0x6 << TENSOR_QUANT_TRANSF5) | (0x7 << TENSOR_QUANT_TRANSF4) | (0x7 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 10\n"
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
			"la x26, rand_int_32\n"
			"flw.ps f22, 608(x26)\n"
			"flw.ps f27, 736(x26)\n"
			"fmulh.pi f0, f22, f27\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f30, 768(x27)\n"
			"fcvt.w.s x17, f30, rtz\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f19, 832(x28)\n"
			"flw.ps f15, 960(x28)\n"
			"fsub.ps f15, f19, f15, rdn\n" 
			"la x17, rand_int_32\n"
			"flw.ps f27, 32(x17)\n"
			"flw.ps f2, 928(x17)\n"
			"fsll.pi f15, f27, f2\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f19, 704(x16)\n"
			"flw.ps f23, 672(x16)\n"
			"flw.ps f15, 384(x16)\n"
			"fmadd.s f16, f19, f23, f15, rdn\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f14, 736(x17)\n"
			"fcvt.pwu.ps f1, f14, rmm\n" 
			"maskpopcz x5, m0\n" 
			"la x17, rand_int_32\n"
			"flw.ps f19, 384(x17)\n"
			"fsrli.pi f22, f19, 0xb\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f0, 0(x23)\n"
			"flw.ps f2, 768(x23)\n"
			"fcmovm.ps f1, f0, f2\n" 
			"la x5, rand_int_32\n"
			"flw.ps f18, 384(x5)\n"
			"fsrli.pi f0, f18, 0x2\n" 
			"la x10, rand_int_32\n"
			"flw.ps f30, 288(x10)\n"
			"fsrli.pi f30, f30, 0x4\n" 
			"maskpopc x10, m2\n" 
			"la x23, rand_int_32\n"
			"flw.ps f22, 704(x23)\n"
			"fpackreph.pi f1, f22\n" 
			"li x5, 0x2464bfc5960e0b3a\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0x808cc392721bc1ca\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"la x17, rand_int_32\n"
			"flw.ps f21, 96(x17)\n"
			"fsrai.pi f1, f21, 0x3\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f23, 480(x11)\n"
			"flw.ps f2, 928(x11)\n"
			"fmax.ps f27, f23, f2\n" 
			"la x26, rand_int_32\n"
			"flw.ps f18, 832(x26)\n"
			"flw.ps f27, 256(x26)\n"
			"flt.pi f28, f18, f27\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_4_XMA_QNT_STR_SEQ_ITER_0_HID_12:\n"
			"li x5, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x2 << TENSOR_IMA_B_NUM_COLS) | (0xc << TENSOR_IMA_A_NUM_ROWS) | (0x5 << TENSOR_IMA_A_NUM_COLS) | (0x7 << TENSOR_IMA_OFFSET) | (0x0 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0xbf << TENSOR_IMA_SCP_LOC_B) | (0xb4 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"LBL_SEQID_4_XMA_QNT_STR_SEQ_ITER_1_HID_12:\n"
			"li x5, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x2 << TENSOR_FMA32_B_NUM_COLS) | (0xd << TENSOR_FMA32_A_NUM_ROWS) | (0xf << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1e << TENSOR_FMA32_SCP_LOC_B) | (0x42 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_12\n"
			"csrw tensor_error, x0\n"
			"li x5, 5\n" 
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
			"csrwi mhpmevent3, 4\n"   
			"csrwi mhpmevent4, 8\n"   
			"csrwi mhpmevent5, 6\n"   
			"csrwi mhpmevent6, 9\n"   
			"csrwi mhpmevent7, 21\n"   
			"csrwi mhpmevent8, 12\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f14,  288 (x5)\n"
			"flw.ps f7,  352 (x5)\n"
			"flw.ps f0,  384 (x5)\n"
			"flw.ps f30,  736 (x5)\n"
			"flw.ps f26,  224 (x5)\n"
			"flw.ps f19,  416 (x5)\n"
			"flw.ps f24,  480 (x5)\n"
			"flw.ps f23,  576 (x5)\n"
			"flw.ps f10,  896 (x5)\n"
			"flw.ps f12,  32 (x5)\n"
			"flw.ps f28,  0 (x5)\n"
			"flw.ps f31,  640 (x5)\n"
			"flw.ps f9,  768 (x5)\n"
			"flw.ps f6,  992 (x5)\n"
			"flw.ps f11,  64 (x5)\n"
			"flw.ps f22,  448 (x5)\n"
			"flw.ps f16,  960 (x5)\n"
			"flw.ps f17,  704 (x5)\n"
			"flw.ps f27,  544 (x5)\n"
			"flw.ps f8,  800 (x5)\n"
			"flw.ps f13,  864 (x5)\n"
			"flw.ps f3,  512 (x5)\n"
			"flw.ps f29,  672 (x5)\n"
			"flw.ps f5,  96 (x5)\n"
			"flw.ps f25,  256 (x5)\n"
			"flw.ps f20,  832 (x5)\n"
			"flw.ps f4,  192 (x5)\n"
			"flw.ps f2,  160 (x5)\n"
			"flw.ps f15,  320 (x5)\n"
			"flw.ps f18,  128 (x5)\n"
			"flw.ps f1,  608 (x5)\n"
			"flw.ps f21,  928 (x5)\n"
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
			"li x5, 0x5\n" 
			"sw x5, (x31)\n"
			"li x29, 0x5\n"
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
			"la x5, rand_int_32\n"
			"flw.ps f30, 576(x5)\n"
			"faddi.pi f22, f30, 0x157\n" 
			"la x18, rand_int_32\n"
			"flw.ps f7, 64(x18)\n"
			"flw.ps f13, 192(x18)\n"
			"fmaxu.pi f10, f7, f13\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f19, 672(x28)\n"
			"flw.ps f2, 352(x28)\n"
			"flt.s x18, f19, f2\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f10, 768(x25)\n"
			"flw.ps f6, 832(x25)\n"
			"fsgnj.ps f14, f10, f6\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f14, 608(x5)\n"
			"ffrc.ps f19, f14\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f6, 960(x23)\n"
			"flw.ps f13, 960(x23)\n"
			"fcmovm.ps f22, f6, f13\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f13, 928(x20)\n"
			"flw.ps f30, 544(x20)\n"
			"fle.s x14, f13, f30\n" 
			"la x28, rand_int_32\n"
			"flw.ps f13, 96(x28)\n"
			"flw.ps f30, 992(x28)\n"
			"fsrl.pi f10, f13, f30\n" 
			"la x14, rand_int_32\n"
			"flw.ps f10, 128(x14)\n"
			"fsat8.pi f23, f10\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f20, 224(x26)\n"
			"flw.ps f25, 864(x26)\n"
			"fmadd.ps f13, f20, f20, f25, rup\n" 
			"la x22, rand_int_32\n"
			"flw.ps f13, 672(x22)\n"
			"fsatu8.pi f25, f13\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f26, 896(x23)\n"
			"fcvt.f16.ps f20, f26\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f14, 128(x9)\n"
			"flw.ps f26, 832(x9)\n"
			"fsgnj.s f11, f14, f26\n" 
			"la x20, rand_int_32\n"
			"flw.ps f6, 640(x20)\n"
			"fsrai.pi f20, f6, 0xb\n" 
			"la x20, rand_int_32\n"
			"flw.ps f18, 512(x20)\n"
			"flw.ps f30, 224(x20)\n"
			"fmax.pi f30, f18, f30\n" 
			"la x11, rand_int_32\n"
			"flw.ps f23, 800(x11)\n"
			"flw.ps f11, 64(x11)\n"
			"fsra.pi f22, f23, f11\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_13\n"
			"addi x30, x30, 1\n"
			"la x22, rand_ieee754_32\n"
			"flw.ps f26, 544(x22)\n"
			"fround.ps f6, f26, rdn\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f2, 320(x11)\n"
			"flw.ps f22, 352(x11)\n"
			"flw.ps f7, 384(x11)\n"
			"fmadd.ps f19, f2, f22, f7, rdn\n" 
			"la x5, rand_int_32\n"
			"flw.ps f23, 224(x5)\n"
			"fsatu8.pi f7, f23\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f10, 736(x25)\n"
			"flw.ps f26, 256(x25)\n"
			"fsgnjx.s f30, f10, f26\n" 
			"LBL_SEQID_1_FP_TRANS_ID_0_HID_13:\n"
			"la x20, rand_ieee754_32\n"
			"flw.ps f23, 32(x20)\n"
			"flog.ps f7, f23\n" 
			"LBL_SEQID_1_M0_WRITE_ID_1_HID_13:\n"
			"fltm.pi m2, f7, f20\n" 
			"li x5, CREDINC1\n"
			"ld x5, 0(x5)\n"
			"LBL_SEQID_1_M0_WRITE_ID_2_HID_13:\n"
			"mova.m.x x18\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"li x9, 0xd5a1747408bf6613\n"
			"fcvt.s.w f19, x9, rdn\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f18, 864(x18)\n"
			"ffrc.ps f25, f18\n" 
			"fbci.ps f18, 0x619d5\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f11, 96(x25)\n"
			"fcvt.f16.ps f11, f11\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f10, 544(x9)\n"
			"flw.ps f23, 480(x9)\n"
			"flt.s x23, f10, f23\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f25, 992(x22)\n"
			"fle.s x20, f25, f25\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f18, 576(x22)\n"
			"flw.ps f19, 160(x22)\n"
			"flw.ps f15, 288(x22)\n"
			"fcmov.ps f7, f18, f19, f15\n" 
			"la x20, rand_int_32\n"
			"flw.ps f19, 448(x20)\n"
			"fsrli.pi f18, f19, 0x8\n" 
			"LBL_SEQID_1_M0_WRITE_ID_3_HID_13:\n"
			"feqm.ps m0, f11, f26\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"csrw tensor_error, zero\n" 
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_13\n"
			"addi x30, x30, 1\n"
			"la x25, rand_ieee754_32\n"
			"flw.ps f13, 896(x25)\n"
			"flw.ps f18, 352(x25)\n"
			"fsgnjx.ps f10, f13, f18\n" 
			"la x18, rand_int_32\n"
			"flw.ps f20, 576(x18)\n"
			"flw.ps f6, 224(x18)\n"
			"fmin.pi f25, f20, f6\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f19, 416(x23)\n"
			"fcvt.f16.ps f22, f19\n" 
			"la x23, rand_ieee754_16\n"
			"flw.ps f20, 800(x23)\n"
			"fcvt.ps.f16 f22, f20\n" 
			"la x22, rand_int_32\n"
			"flw.ps f18, 256(x22)\n"
			"flw.ps f15, 480(x22)\n"
			"fand.pi f25, f18, f15\n" 
			"maskpopc x14, m0\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f2, 832(x25)\n"
			"fclass.s x26, f2\n" 
			"la x23, rand_int_32\n"
			"flw.ps f20, 736(x23)\n"
			"flw.ps f6, 224(x23)\n"
			"fle.pi f22, f20, f6\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f26, 256(x9)\n"
			"flw.ps f10, 800(x9)\n"
			"flt.s x11, f26, f10\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f25, 480(x25)\n"
			"flw.ps f10, 96(x25)\n"
			"flt.ps f6, f25, f10\n" 
			"la x28, rand_int_32\n"
			"flw.ps f14, 320(x28)\n"
			"fsat8.pi f22, f14\n" 
			"LBL_SEQID_2_M0_WRITE_ID_4_HID_13:\n"
			"maskxor m2, m4, m0\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0x34a5abceff\n"
			"flw.ps f5, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x14, rand_ieee754_32\n"
			"flw.ps f18, 448(x14)\n"
			"flw.ps f30, 992(x14)\n"
			"flw.ps f25, 96(x14)\n"
			"fcmov.ps f19, f18, f30, f25\n" 
			"la x14, rand_int_32\n"
			"flw.ps f19, 288(x14)\n"
			"fcvt.ps.pw f14, f19, rtz\n" 
			"la x20, rand_int_32\n"
			"flw.ps f18, 480(x20)\n"
			"fslli.pi f2, f18, 0x6\n" 
			"LBL_SEQID_2_M0_WRITE_ID_5_HID_13:\n"
			"mov.m.x m3, x26, 0x75\n" 
			"mov.m.x m0, x0, 0xFF\n"
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
			"la x9, rand_ieee754_32\n"
			"flw.ps f11, 672(x9)\n"
			"flw.ps f30, 448(x9)\n"
			"flw.ps f7, 864(x9)\n"
			"fcmov.ps f10, f11, f30, f7\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f2, 320(x18)\n"
			"fclass.s x25, f2\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f19, 480(x18)\n"
			"flw.ps f20, 608(x18)\n"
			"fsgnjx.ps f23, f19, f20\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f23, 992(x28)\n"
			"fcvt.f16.ps f14, f23\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f18, 576(x26)\n"
			"fcvt.lu.s x11, f18, rdn\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f20, 352(x11)\n"
			"fcvt.pwu.ps f13, f20, rmm\n" 
			"LBL_SEQID_3_M0_WRITE_ID_6_HID_13:\n"
			"fltm.ps m3, f10, f10\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x25, rand_int_32\n"
			"flw.ps f14, 64(x25)\n"
			"flw.ps f20, 768(x25)\n"
			"fltu.pi f20, f14, f20\n" 
			"LBL_SEQID_3_M0_WRITE_ID_7_HID_13:\n"
			"mova.m.x x22\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0x875c86a2f\n"
			"flw.ps f5, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x18, rand_int_32\n"
			"flw.ps f19, 736(x18)\n"
			"flw.ps f6, 960(x18)\n"
			"fmulhu.pi f23, f19, f6\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f22, 96(x9)\n"
			"flw.ps f7, 64(x9)\n"
			"fsub.ps f26, f22, f7, rtz\n" 
			"la x9, rand_int_32\n"
			"flw.ps f13, 960(x9)\n"
			"fpackreph.pi f22, f13\n" 
			"la x11, rand_int_32\n"
			"flw.ps f22, 192(x11)\n"
			"fsrli.pi f30, f22, 0x5\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f6, 992(x14)\n"
			"fclass.s x18, f6\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f18, 64(x26)\n"
			"ffrc.ps f2, f18\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f18, 928(x11)\n"
			"flw.ps f26, 736(x11)\n"
			"fsgnjn.s f13, f18, f26\n" 
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
			"la x25, rand_int_32\n"
			"flw.ps f11, 0(x25)\n"
			"fpackreph.pi f22, f11\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f15, 448(x18)\n"
			"flw.ps f14, 544(x18)\n"
			"fmin.s f15, f15, f14\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f26, 736(x9)\n"
			"flw.ps f23, 992(x9)\n"
			"fcmovm.ps f20, f26, f23\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f2, 608(x22)\n"
			"flw.ps f30, 32(x22)\n"
			"fsgnj.s f15, f2, f30\n" 
			"LBL_SEQID_4_FP_TRANS_ID_8_HID_13:\n"
			"la x23, rand_ieee754_32\n"
			"flw.ps f26, 160(x23)\n"
			"flog.ps f13, f26\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f18, 480(x20)\n"
			"flw.ps f13, 288(x20)\n"
			"fmax.s f22, f18, f13\n" 
			"la x18, rand_int_32\n"
			"flw.ps f18, 224(x18)\n"
			"flw.ps f19, 992(x18)\n"
			"fmax.pi f7, f18, f19\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f30, 768(x14)\n"
			"ffrc.ps f6, f30\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f13, 672(x28)\n"
			"flw.ps f6, 416(x28)\n"
			"fsgnjn.ps f19, f13, f6\n" 
			"la x25, rand_int_32\n"
			"flw.ps f15, 320(x25)\n"
			"flw.ps f19, 480(x25)\n"
			"fdiv.pi f13, f15, f19\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f11, 32(x22)\n"
			"fsgnjx.s f7, f11, f11\n" 
			"LBL_SEQID_4_M0_WRITE_ID_9_HID_13:\n"
			"masknot m3, m3\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x22, rand_int_32\n"
			"flw.ps f13, 96(x22)\n"
			"flw.ps f11, 544(x22)\n"
			"fminu.pi f30, f13, f11\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f11, 352(x26)\n"
			"flw.ps f15, 608(x26)\n"
			"flw.ps f2, 704(x26)\n"
			"fmadd.s f14, f11, f15, f2, rne\n" 
			"maskpopc x22, m4\n" 
			"la x20, rand_int_32\n"
			"flw.ps f2, 544(x20)\n"
			"flw.ps f6, 800(x20)\n"
			"fmulh.pi f10, f2, f6\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 5\n" 
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
			"csrwi mhpmevent3, 17\n"   
			"csrwi mhpmevent4, 28\n"   
			"csrwi mhpmevent5, 19\n"   
			"csrwi mhpmevent6, 16\n"   
			"csrwi mhpmevent7, 4\n"   
			"csrwi mhpmevent8, 7\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f16,  0 (x5)\n"
			"flw.ps f0,  672 (x5)\n"
			"flw.ps f25,  384 (x5)\n"
			"flw.ps f13,  64 (x5)\n"
			"flw.ps f20,  448 (x5)\n"
			"flw.ps f23,  512 (x5)\n"
			"flw.ps f22,  128 (x5)\n"
			"flw.ps f29,  480 (x5)\n"
			"flw.ps f24,  288 (x5)\n"
			"flw.ps f26,  96 (x5)\n"
			"flw.ps f4,  224 (x5)\n"
			"flw.ps f15,  896 (x5)\n"
			"flw.ps f11,  416 (x5)\n"
			"flw.ps f17,  256 (x5)\n"
			"flw.ps f19,  32 (x5)\n"
			"flw.ps f2,  800 (x5)\n"
			"flw.ps f21,  768 (x5)\n"
			"flw.ps f31,  832 (x5)\n"
			"flw.ps f10,  736 (x5)\n"
			"flw.ps f5,  992 (x5)\n"
			"flw.ps f6,  608 (x5)\n"
			"flw.ps f3,  960 (x5)\n"
			"flw.ps f28,  576 (x5)\n"
			"flw.ps f27,  864 (x5)\n"
			"flw.ps f7,  928 (x5)\n"
			"flw.ps f1,  160 (x5)\n"
			"flw.ps f30,  192 (x5)\n"
			"flw.ps f9,  352 (x5)\n"
			"flw.ps f18,  704 (x5)\n"
			"flw.ps f8,  544 (x5)\n"
			"flw.ps f14,  640 (x5)\n"
			"flw.ps f12,  320 (x5)\n"
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
			"li x5, 0x5\n" 
			"sw x5, (x31)\n"
			"li x29, 0x5\n"
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
			"la x20, rand_int_32\n"
			"flw.ps f13, 640(x20)\n"
			"flw.ps f1, 256(x20)\n"
			"fmax.pi f10, f13, f1\n" 
			"la x19, rand_int_32\n"
			"flw.ps f12, 640(x19)\n"
			"fpackrepb.pi f0, f12\n" 
			"la x25, rand_int_32\n"
			"flw.ps f2, 736(x25)\n"
			"flw.ps f3, 448(x25)\n"
			"fltu.pi f5, f2, f3\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f17, 64(x26)\n"
			"flw.ps f19, 832(x26)\n"
			"fmax.ps f16, f17, f19\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f23, 992(x23)\n"
			"flw.ps f1, 128(x23)\n"
			"fmul.s f20, f23, f1, rup\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f10, 768(x20)\n"
			"flw.ps f15, 320(x20)\n"
			"fsgnj.ps f23, f10, f15\n" 
			"LBL_SEQID_0_FP_TRANS_ID_0_HID_14:\n"
			"la x23, rand_ieee754_32\n"
			"flw.ps f0, 192(x23)\n"
			"frcp.ps f1, f0\n" 
			"la x19, rand_int_32\n"
			"flw.ps f10, 448(x19)\n"
			"fpackreph.pi f7, f10\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f2, 320(x10)\n"
			"fcvt.f16.ps f23, f2\n" 
			"la x14, rand_int_32\n"
			"flw.ps f2, 32(x14)\n"
			"flw.ps f17, 448(x14)\n"
			"fmax.pi f0, f2, f17\n" 
			"fbci.ps f0, 0x78ef6\n" 
			"la x22, rand_int_32\n"
			"flw.ps f0, 576(x22)\n"
			"flw.ps f13, 160(x22)\n"
			"for.pi f23, f0, f13\n" 
			"la x25, rand_int_32\n"
			"flw.ps f17, 704(x25)\n"
			"faddi.pi f0, f17, 0x1ce\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x16, rand_ieee754_32\n"
			"flw.ps f7, 896(x16)\n"
			"flw.ps f23, 928(x16)\n"
			"fmul.ps f13, f7, f23, rmm\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f10, 256(x19)\n"
			"fsin.ps f2, f10\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f16, 800(x17)\n"
			"fround.ps f10, f16, rmm\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 0xbcca9bc4691c846b\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0x772fd3f31e1a1c05\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ID_817533_HID_14:\n"
			"la x31, self_check_32\n"
			"li x5, 0x20000000000002\n"
			"add x5, x5, x31\n"
			"li x6, 0x10000000000008\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x2 << TENSOR_FMA32_B_NUM_COLS) | (0x2 << TENSOR_FMA32_A_NUM_ROWS) | (0x8 << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x1 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1b << TENSOR_FMA32_SCP_LOC_B) | (0x1 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0x9\n"
			"bne x5, x31, LBL_FAIL_HID_14\n"
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
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ITER_0_HID_14:\n"
			"li x5, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x0 << TENSOR_FMA32_B_NUM_COLS) | (0x7 << TENSOR_FMA32_A_NUM_ROWS) | (0x4 << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0xb7 << TENSOR_FMA32_SCP_LOC_B) | (0xd7 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ITER_1_HID_14:\n"
			"li x5, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x0 << TENSOR_FMA16_B_NUM_COLS) | (0x4 << TENSOR_FMA16_A_NUM_ROWS) | (0x3 << TENSOR_FMA16_A_NUM_COLS) | (0x3 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x2 << TENSOR_FMA16_SCP_LOC_B) | (0x59 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_14\n"
			"csrw tensor_error, x0\n"
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_14\n"
			"addi x30, x30, 1\n"
			"la x17, rand_ieee754_32\n"
			"flw.ps f20, 384(x17)\n"
			"flw.ps f15, 544(x17)\n"
			"fmul.ps f12, f20, f15, rup\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f17, 224(x10)\n"
			"flw.ps f5, 992(x10)\n"
			"fsgnj.s f12, f17, f5\n" 
			"la x19, rand_int_32\n"
			"flw.ps f10, 864(x19)\n"
			"fsat8.pi f2, f10\n" 
			"maskpopcz x14, m2\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f2, 704(x16)\n"
			"fle.s x17, f2, f2\n" 
			"LBL_SEQID_1_M0_WRITE_ID_1_HID_14:\n"
			"maskand m1, m3, m2\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"li x5, 0x3cf8\n"
			"csrw tensor_mask, x5\n"	
			"li x25, 0xc850f96ee0236b73\n"
			"fcvt.s.wu f15, x25, rup\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x25, rand_ieee754_16\n"
			"flw.ps f17, 768(x25)\n"
			"fcvt.ps.f16 f23, f17\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f13, 832(x23)\n"
			"flw.ps f7, 864(x23)\n"
			"fmul.ps f10, f13, f7, rne\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x27, rand_ieee754_32\n"
			"flw.ps f15, 736(x27)\n"
			"flw.ps f0, 160(x27)\n"
			"flt.s x10, f15, f0\n" 
			"la x25, rand_int_32\n"
			"flw.ps f20, 480(x25)\n"
			"flw.ps f23, 96(x25)\n"
			"fmul.pi f12, f20, f23\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f7, 480(x14)\n"
			"flw.ps f20, 704(x14)\n"
			"fsgnjn.ps f5, f7, f20\n" 
			"li x19, 0x63ed912fd442791b\n"
			"fcvt.s.wu f16, x19, rmm\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f13, 192(x10)\n"
			"flw.ps f5, 352(x10)\n"
			"fmax.s f12, f13, f5\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f15, 960(x17)\n"
			"flw.ps f12, 384(x17)\n"
			"fsgnj.ps f13, f15, f12\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f15, 416(x10)\n"
			"flw.ps f3, 320(x10)\n"
			"fadd.s f15, f15, f3, rne\n" 
			"csrw tensor_error, zero\n" 
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=0); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ITER_0_HID_14:\n"
			"li x6, ((0xc << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0x8 << TENSOR_QUANT_QUANT_ROW) | (0x1e << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x1 << TENSOR_QUANT_TRANSF1) | (0x9 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_14\n"
			"csrw tensor_error, x0\n"
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_14\n"
			"addi x30, x30, 1\n"
			"la x22, rand_ieee754_32\n"
			"flw.ps f17, 96(x22)\n"
			"flw.ps f16, 32(x22)\n"
			"fsub.ps f2, f17, f16, rdn\n" 
			"la x27, rand_int_32\n"
			"flw.ps f26, 768(x27)\n"
			"fsrli.pi f2, f26, 0xf\n" 
			"la x17, rand_int_32\n"
			"flw.ps f13, 576(x17)\n"
			"fpackrepb.pi f12, f13\n" 
			"LBL_SEQID_2_M0_WRITE_ID_2_HID_14:\n"
			"flem.ps m1, f2, f16\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x20, rand_ieee754_32\n"
			"flw.ps f26, 288(x20)\n"
			"flw.ps f12, 352(x20)\n"
			"flw.ps f23, 64(x20)\n"
			"fcmov.ps f3, f26, f12, f23\n" 
			"LBL_SEQID_2_M0_WRITE_ID_3_HID_14:\n"
			"mova.m.x x25\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x17, rand_ieee754_32\n"
			"flw.ps f15, 128(x17)\n"
			"frsq.ps f10, f15\n" 
			"li x26, 0x9ab2c874f73bf092\n"
			"fcvt.s.wu f19, x26, rne\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f26, 832(x14)\n"
			"fcvt.lu.s x17, f26, rtz\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f5, 32(x26)\n"
			"fcvt.w.s x23, f5, rne\n" 
			"LBL_SEQID_2_FP_TRANS_ID_4_HID_14:\n"
			"la x19, rand_ieee754_32\n"
			"flw.ps f5, 992(x19)\n"
			"fexp.ps f15, f5\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f23, 128(x19)\n"
			"fclass.s x19, f23\n" 
			"la x10, rand_int_32\n"
			"flw.ps f15, 448(x10)\n"
			"fsat8.pi f0, f15\n" 
			"LBL_SEQID_2_M0_WRITE_ID_5_HID_14:\n"
			"flem.ps m2, f15, f13\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"fbci.ps f26, 0x6fa8a\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f23, 320(x22)\n"
			"flw.ps f17, 896(x22)\n"
			"flw.ps f20, 864(x22)\n"
			"fnmsub.s f3, f23, f17, f20, rne\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_2_TLD_XMA_STR_SEQ_ID_79821_HID_14:\n"
			"la x31, self_check_16\n"
			"li x5, 0x20000000000004\n"
			"add x5, x5, x31\n"
			"li x6, 0x3a0000000000005\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x3 << TENSOR_FMA16_B_NUM_COLS) | (0x4 << TENSOR_FMA16_A_NUM_ROWS) | (0x5 << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1d << TENSOR_FMA16_SCP_LOC_B) | (0x1 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0xc\n"
			"bne x5, x31, LBL_FAIL_HID_14\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_14\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_2_XMA_QNT_STR_SEQ_ITER_0_HID_14:\n"
			"li x5, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x1 << TENSOR_FMA32_B_NUM_COLS) | (0x5 << TENSOR_FMA32_A_NUM_ROWS) | (0x6 << TENSOR_FMA32_A_NUM_COLS) | (0x9 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x80 << TENSOR_FMA32_SCP_LOC_B) | (0x38 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x0 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
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
			"la x10, rand_ieee754_32\n"
			"flw.ps f3, 832(x10)\n"
			"flw.ps f23, 448(x10)\n"
			"flw.ps f0, 672(x10)\n"
			"fcmov.ps f13, f3, f23, f0\n" 
			"li x5, 0xeb59\n"
			"csrw tensor_mask, x5\n"	
			"la x26, rand_ieee754_32\n"
			"flw.ps f17, 576(x26)\n"
			"flw.ps f20, 704(x26)\n"
			"fadd.ps f26, f17, f20, rdn\n" 
			"li x5, 0xf8cb154de17ae854\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0xa8b263af9c36bc67\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"la x16, rand_int_32\n"
			"flw.ps f20, 992(x16)\n"
			"flw.ps f15, 576(x16)\n"
			"fmul.pi f7, f20, f15\n" 
			"la x17, rand_int_32\n"
			"flw.ps f19, 544(x17)\n"
			"flw.ps f10, 0(x17)\n"
			"for.pi f15, f19, f10\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f7, 128(x10)\n"
			"fcvt.pw.ps f19, f7, rdn\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f15, 384(x27)\n"
			"flw.ps f13, 96(x27)\n"
			"fmin.s f0, f15, f13\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x26, rand_ieee754_32\n"
			"flw.ps f26, 0(x26)\n"
			"flw.ps f10, 448(x26)\n"
			"fsub.ps f10, f26, f10, rup\n" 
			"la x10, rand_int_32\n"
			"flw.ps f1, 352(x10)\n"
			"flw.ps f2, 320(x10)\n"
			"fmulhu.pi f17, f1, f2\n" 
			"LBL_SEQID_3_M0_WRITE_ID_6_HID_14:\n"
			"mova.m.x x25\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_3_M0_WRITE_ID_7_HID_14:\n"
			"mova.m.x x23\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0xf5d620fec2\n"
			"flw.ps f5, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x10, rand_ieee754_32\n"
			"flw.ps f19, 416(x10)\n"
			"fcvt.pw.ps f10, f19, rtz\n" 
			"la x26, rand_int_32\n"
			"flw.ps f19, 608(x26)\n"
			"fsrai.pi f0, f19, 0xc\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f3, 768(x26)\n"
			"ffrc.ps f0, f3\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f0, 480(x16)\n"
			"flw.ps f16, 864(x16)\n"
			"flw.ps f12, 224(x16)\n"
			"fmsub.s f2, f0, f16, f12, rtz\n" 
			"la x19, rand_int_32\n"
			"flw.ps f2, 64(x19)\n"
			"fcvt.ps.pwu f15, f2, rne\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f10, 992(x22)\n"
			"flw.ps f0, 928(x22)\n"
			"fdiv.s f26, f10, f0, rdn\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_3_XMA_QNT_STR_SEQ_ITER_0_HID_14:\n"
			"li x5, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x1 << TENSOR_FMA32_B_NUM_COLS) | (0x2 << TENSOR_FMA32_A_NUM_ROWS) | (0x5 << TENSOR_FMA32_A_NUM_COLS) | (0x7 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x7b << TENSOR_FMA32_SCP_LOC_B) | (0xa5 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
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
			"la x10, rand_ieee754_32\n"
			"flw.ps f7, 832(x10)\n"
			"flw.ps f3, 704(x10)\n"
			"fsub.s f12, f7, f3, rdn\n" 
			"la x27, rand_int_32\n"
			"flw.ps f17, 32(x27)\n"
			"flw.ps f7, 832(x27)\n"
			"fmulhu.pi f23, f17, f7\n" 
			"la x14, rand_int_32\n"
			"flw.ps f13, 96(x14)\n"
			"fcvt.ps.pwu f13, f13, rtz\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f13, 288(x25)\n"
			"flw.ps f2, 480(x25)\n"
			"flw.ps f0, 192(x25)\n"
			"fmadd.s f26, f13, f2, f0, rup\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x26, rand_ieee754_32\n"
			"flw.ps f3, 896(x26)\n"
			"fcvt.pwu.ps f26, f3, rdn\n" 
			"LBL_SEQID_4_M0_WRITE_ID_8_HID_14:\n"
			"fsetm.pi m1, f1\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_4_M0_WRITE_ID_9_HID_14:\n"
			"mova.m.x x17\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x23, rand_ieee754_32\n"
			"flw.ps f0, 128(x23)\n"
			"fcvt.wu.s x26, f0, rne\n" 
			"la x27, rand_int_32\n"
			"flw.ps f17, 288(x27)\n"
			"flw.ps f15, 192(x27)\n"
			"fsub.pi f13, f17, f15\n" 
			"la x23, rand_int_32\n"
			"flw.ps f0, 288(x23)\n"
			"flw.ps f5, 544(x23)\n"
			"fsll.pi f10, f0, f5\n" 
			"la x14, rand_int_32\n"
			"flw.ps f0, 608(x14)\n"
			"fslli.pi f5, f0, 0x8\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f10, 832(x22)\n"
			"flw.ps f13, 640(x22)\n"
			"fmul.s f12, f10, f13, rdn\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f2, 384(x23)\n"
			"flw.ps f12, 768(x23)\n"
			"flw.ps f10, 672(x23)\n"
			"fcmov.ps f3, f2, f12, f10\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f20, 320(x17)\n"
			"flw.ps f15, 736(x17)\n"
			"flw.ps f1, 256(x17)\n"
			"fmadd.s f3, f20, f15, f1, rdn\n" 
			"la x20, rand_int_32\n"
			"flw.ps f26, 160(x20)\n"
			"flw.ps f3, 864(x20)\n"
			"fltu.pi f26, f26, f3\n" 
			"la x14, rand_int_32\n"
			"flw.ps f12, 512(x14)\n"
			"flw.ps f16, 544(x14)\n"
			"fmin.pi f16, f12, f16\n" 
			"li x5, 0x1e68\n"
			"csrw tensor_mask, x5\n"	
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_4_XMA_QNT_STR_SEQ_ID_869031_HID_14:\n"
			"li x6, ((0x1b << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0x8 << TENSOR_QUANT_QUANT_ROW) | (0x27 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x2 << TENSOR_QUANT_TRANSF7) | (0x6 << TENSOR_QUANT_TRANSF6) | (0x7 << TENSOR_QUANT_TRANSF5) | (0x6 << TENSOR_QUANT_TRANSF4) | (0x7 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_14\n"
			"csrw tensor_error, x0\n"
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"LBL_SEQID_4_XMA_QNT_STR_SEQ_ITER_0_HID_14:\n"
			"li x6, ((0x19 << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0x0 << TENSOR_QUANT_QUANT_ROW) | (0x37 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x7 << TENSOR_QUANT_TRANSF6) | (0x7 << TENSOR_QUANT_TRANSF5) | (0x7 << TENSOR_QUANT_TRANSF4) | (0x7 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_4_XMA_QNT_STR_SEQ_ITER_1_HID_14:\n"
			"li x6, ((0x1b << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0xc << TENSOR_QUANT_QUANT_ROW) | (0x13 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x2 << TENSOR_QUANT_TRANSF5) | (0x7 << TENSOR_QUANT_TRANSF4) | (0x7 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_14\n"
			"csrw tensor_error, x0\n"
			"li x5, 5\n" 
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
			"csrwi mhpmevent3, 4\n"   
			"csrwi mhpmevent4, 8\n"   
			"csrwi mhpmevent5, 6\n"   
			"csrwi mhpmevent6, 9\n"   
			"csrwi mhpmevent7, 21\n"   
			"csrwi mhpmevent8, 12\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f11,  352 (x5)\n"
			"flw.ps f16,  224 (x5)\n"
			"flw.ps f20,  512 (x5)\n"
			"flw.ps f3,  896 (x5)\n"
			"flw.ps f15,  64 (x5)\n"
			"flw.ps f23,  32 (x5)\n"
			"flw.ps f10,  992 (x5)\n"
			"flw.ps f31,  832 (x5)\n"
			"flw.ps f28,  416 (x5)\n"
			"flw.ps f5,  480 (x5)\n"
			"flw.ps f8,  928 (x5)\n"
			"flw.ps f18,  544 (x5)\n"
			"flw.ps f21,  288 (x5)\n"
			"flw.ps f22,  960 (x5)\n"
			"flw.ps f7,  864 (x5)\n"
			"flw.ps f29,  736 (x5)\n"
			"flw.ps f25,  128 (x5)\n"
			"flw.ps f0,  0 (x5)\n"
			"flw.ps f24,  256 (x5)\n"
			"flw.ps f12,  448 (x5)\n"
			"flw.ps f30,  800 (x5)\n"
			"flw.ps f17,  768 (x5)\n"
			"flw.ps f13,  576 (x5)\n"
			"flw.ps f26,  672 (x5)\n"
			"flw.ps f9,  608 (x5)\n"
			"flw.ps f27,  704 (x5)\n"
			"flw.ps f14,  640 (x5)\n"
			"flw.ps f1,  96 (x5)\n"
			"flw.ps f4,  192 (x5)\n"
			"flw.ps f19,  320 (x5)\n"
			"flw.ps f6,  160 (x5)\n"
			"flw.ps f2,  384 (x5)\n"
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
			"li x5, 0x5\n" 
			"sw x5, (x31)\n"
			"li x29, 0x5\n"
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
			"la x7, rand_ieee754_32\n"
			"flw.ps f9, 896(x7)\n"
			"flw.ps f13, 0(x7)\n"
			"flw.ps f12, 768(x7)\n"
			"fnmsub.ps f9, f9, f13, f12, rup\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_15:\n"
			"fsetm.pi m2, f29\n" 
			"li x5, CREDINC2\n"
			"ld x5, 0(x5)\n"
			"la x5, rand_ieee754_32\n"
			"flw.ps f8, 320(x5)\n"
			"flw.ps f12, 320(x5)\n"
			"fmul.s f9, f8, f12, rmm\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f10, 800(x16)\n"
			"ffrc.ps f12, f10\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f2, 768(x23)\n"
			"flw.ps f15, 384(x23)\n"
			"fadd.s f30, f2, f15, rup\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f8, 992(x11)\n"
			"frsq.ps f23, f8\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_15:\n"
			"fsetm.pi m3, f15\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0xf7543fba09\n"
			"fsw.ps f0, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x7, rand_ieee754_32\n"
			"flw.ps f13, 704(x7)\n"
			"flw.ps f5, 448(x7)\n"
			"flw.ps f23, 640(x7)\n"
			"fmadd.ps f15, f13, f5, f23, rne\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f10, 320(x10)\n"
			"fswizz.ps f5, f10, 0x0\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f10, 512(x7)\n"
			"fswizz.ps f8, f10, 0x1b\n" 
			"la x5, rand_int_32\n"
			"flw.ps f8, 192(x5)\n"
			"flw.ps f26, 288(x5)\n"
			"fminu.pi f30, f8, f26\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f5, 608(x23)\n"
			"fcvt.w.s x16, f5, rup\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f26, 64(x17)\n"
			"flw.ps f6, 608(x17)\n"
			"flw.ps f1, 896(x17)\n"
			"fnmsub.s f30, f26, f6, f1, rmm\n" 
			"la x17, rand_int_32\n"
			"flw.ps f13, 64(x17)\n"
			"fsat8.pi f1, f13\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f13, 800(x11)\n"
			"fround.ps f20, f13, rne\n" 
			"LBL_SEQID_0_FP_TRANS_ID_2_HID_15:\n"
			"la x11, rand_ieee754_32\n"
			"flw.ps f10, 736(x11)\n"
			"flog.ps f8, f10\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_15\n"
			"addi x30, x30, 1\n"
			"la x11, rand_int_32\n"
			"flw.ps f6, 256(x11)\n"
			"flw.ps f1, 640(x11)\n"
			"flt.pi f2, f6, f1\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f9, 544(x17)\n"
			"flw.ps f29, 224(x17)\n"
			"fmul.ps f12, f9, f29, rdn\n" 
			"la x10, rand_int_32\n"
			"flw.ps f9, 384(x10)\n"
			"fslli.pi f23, f9, 0x8\n" 
			"la x5, rand_int_32\n"
			"flw.ps f1, 352(x5)\n"
			"flw.ps f10, 608(x5)\n"
			"flt.pi f5, f1, f10\n" 
			"la x16, rand_int_32\n"
			"flw.ps f10, 0(x16)\n"
			"flw.ps f18, 224(x16)\n"
			"fsra.pi f15, f10, f18\n" 
			"la x18, rand_int_32\n"
			"flw.ps f1, 64(x18)\n"
			"fcvt.ps.pwu f15, f1, rtz\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f6, 800(x16)\n"
			"flw.ps f2, 672(x16)\n"
			"fnmsub.s f15, f6, f2, f6, rmm\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f12, 736(x10)\n"
			"flw.ps f6, 640(x10)\n"
			"flw.ps f10, 672(x10)\n"
			"fnmsub.s f15, f12, f6, f10, rdn\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f5, 640(x11)\n"
			"flw.ps f20, 192(x11)\n"
			"flw.ps f26, 768(x11)\n"
			"fnmadd.s f6, f5, f20, f26, rne\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f26, 704(x7)\n"
			"flw.ps f5, 928(x7)\n"
			"fsub.ps f15, f26, f5, rne\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f2, 256(x10)\n"
			"fcvt.f16.ps f1, f2\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f20, 416(x11)\n"
			"fround.ps f2, f20, rne\n" 
			"la x9, rand_int_32\n"
			"flw.ps f8, 608(x9)\n"
			"fsatu8.pi f12, f8\n" 
			"li x7, 0x9e199e01d8ebea87\n"
			"fcvt.s.w f12, x7, rup\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f5, 736(x9)\n"
			"flw.ps f18, 96(x9)\n"
			"fmin.s f8, f5, f18\n" 
			"la x11, rand_int_32\n"
			"flw.ps f2, 288(x11)\n"
			"flw.ps f12, 832(x11)\n"
			"for.pi f10, f2, f12\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_15\n"
			"addi x30, x30, 1\n"
			"li x5, 0xf5680697de690950\n"
			"fcvt.s.w f29, x5, rne\n" 
			"la x16, rand_int_32\n"
			"flw.ps f18, 960(x16)\n"
			"fpackrepb.pi f15, f18\n" 
			"la x18, rand_int_32\n"
			"flw.ps f15, 704(x18)\n"
			"fpackreph.pi f6, f15\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f29, 576(x23)\n"
			"fswizz.ps f23, f29, 0x4\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f26, 256(x10)\n"
			"fcvt.pw.ps f13, f26, rne\n" 
			"la x9, rand_int_32\n"
			"flw.ps f2, 800(x9)\n"
			"flw.ps f29, 480(x9)\n"
			"flt.pi f23, f2, f29\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f6, 192(x10)\n"
			"flw.ps f13, 896(x10)\n"
			"fsgnjn.ps f5, f6, f13\n" 
			"la x10, rand_int_32\n"
			"flw.ps f26, 864(x10)\n"
			"flw.ps f2, 576(x10)\n"
			"fadd.pi f18, f26, f2\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f12, 448(x7)\n"
			"ffrc.ps f5, f12\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f13, 992(x7)\n"
			"flw.ps f10, 640(x7)\n"
			"fmax.ps f26, f13, f10\n" 
			"li x7, 0xc789ae719f8cf98a\n"
			"fcvt.s.w f1, x7, rtz\n" 
			"la x16, rand_int_32\n"
			"flw.ps f1, 320(x16)\n"
			"fsrai.pi f10, f1, 0x4\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f23, 928(x23)\n"
			"fsin.ps f26, f23\n" 
			"LBL_SEQID_2_FP_TRANS_ID_3_HID_15:\n"
			"la x5, rand_ieee754_32\n"
			"flw.ps f6, 128(x5)\n"
			"fexp.ps f29, f6\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f8, 384(x10)\n"
			"flw.ps f10, 352(x10)\n"
			"fsgnjn.ps f2, f8, f10\n" 
			"LBL_SEQID_2_M0_WRITE_ID_4_HID_15:\n"
			"fltm.pi m3, f30, f23\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0xc7d65a2ff\n"
			"flw.ps f5, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
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
			"la x9, rand_int_32\n"
			"flw.ps f5, 480(x9)\n"
			"flw.ps f6, 224(x9)\n"
			"fxor.pi f1, f5, f6\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f23, 480(x11)\n"
			"flw.ps f5, 992(x11)\n"
			"fsgnjx.s f12, f23, f5\n" 
			"la x18, rand_int_32\n"
			"flw.ps f6, 0(x18)\n"
			"flw.ps f20, 160(x18)\n"
			"fmax.pi f10, f6, f20\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f8, 768(x28)\n"
			"fround.ps f29, f8, rmm\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f9, 256(x16)\n"
			"flw.ps f26, 992(x16)\n"
			"fmax.ps f8, f9, f26\n" 
			"la x9, rand_int_32\n"
			"flw.ps f2, 64(x9)\n"
			"flw.ps f12, 832(x9)\n"
			"fand.pi f26, f2, f12\n" 
			"la x7, rand_int_32\n"
			"flw.ps f26, 224(x7)\n"
			"flw.ps f23, 352(x7)\n"
			"fmulh.pi f2, f26, f23\n" 
			"la x7, rand_int_32\n"
			"flw.ps f13, 608(x7)\n"
			"fandi.pi f1, f13, 0x1f6\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f5, 960(x28)\n"
			"fround.ps f15, f5, rtz\n" 
			"li x9, 0x4952bfcc7f5d771c\n"
			"fcvt.s.l f10, x9, rdn\n" 
			"la x7, rand_int_32\n"
			"flw.ps f9, 576(x7)\n"
			"fcvt.ps.pw f26, f9, rdn\n" 
			"fbci.pi f20, 0x11d48\n" 
			"la x23, rand_int_32\n"
			"flw.ps f29, 0(x23)\n"
			"fadd.pi f10, f29, f29\n" 
			"la x9, rand_int_32\n"
			"flw.ps f6, 416(x9)\n"
			"flw.ps f10, 768(x9)\n"
			"fand.pi f15, f6, f10\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f2, 512(x10)\n"
			"flw.ps f1, 160(x10)\n"
			"fadd.s f9, f2, f1, rmm\n" 
			"la x28, rand_int_32\n"
			"flw.ps f23, 96(x28)\n"
			"flw.ps f6, 800(x28)\n"
			"fmax.pi f15, f23, f6\n" 
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
			"la x17, rand_int_32\n"
			"flw.ps f12, 224(x17)\n"
			"flw.ps f1, 672(x17)\n"
			"fmulh.pi f18, f12, f1\n" 
			"LBL_SEQID_4_FP_TRANS_ID_5_HID_15:\n"
			"la x7, rand_ieee754_32\n"
			"flw.ps f20, 960(x7)\n"
			"flog.ps f23, f20\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f1, 448(x7)\n"
			"fcvt.pw.ps f8, f1, rmm\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f1, 160(x17)\n"
			"flw.ps f15, 416(x17)\n"
			"fsgnjx.ps f30, f1, f15\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f10, 352(x18)\n"
			"flw.ps f2, 768(x18)\n"
			"feq.ps f23, f10, f2\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f13, 352(x18)\n"
			"flw.ps f8, 384(x18)\n"
			"feq.ps f2, f13, f8\n" 
			"la x28, rand_int_32\n"
			"flw.ps f30, 960(x28)\n"
			"flw.ps f13, 608(x28)\n"
			"fltu.pi f29, f30, f13\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f20, 256(x16)\n"
			"flw.ps f30, 448(x16)\n"
			"flw.ps f29, 192(x16)\n"
			"fnmadd.s f5, f20, f30, f29, rup\n" 
			"la x7, rand_int_32\n"
			"flw.ps f29, 96(x7)\n"
			"fsat8.pi f18, f29\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f8, 96(x23)\n"
			"flw.ps f2, 576(x23)\n"
			"fsub.s f9, f8, f2, rmm\n" 
			"LBL_SEQID_4_M0_WRITE_ID_6_HID_15:\n"
			"fsetm.pi m3, f8\n" 
			"li x5, 0x80340160\n"
			"ld x5, 0(x5)\n"
			"la x11, rand_ieee754_32\n"
			"flw.ps f10, 800(x11)\n"
			"flw.ps f26, 960(x11)\n"
			"fle.ps f5, f10, f26\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f13, 800(x17)\n"
			"flw.ps f18, 0(x17)\n"
			"fadd.s f20, f13, f18, rup\n" 
			"fbci.ps f13, 0x7d80\n" 
			"la x16, rand_int_32\n"
			"flw.ps f26, 992(x16)\n"
			"flw.ps f18, 704(x16)\n"
			"fmulh.pi f26, f26, f18\n" 
			"li x11, 0xee13cf2ccadca2fc\n"
			"fcvt.s.wu f1, x11, rup\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 5\n" 
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
		0x90b409c3, 
		0xec875c2b, 
		0x91a0122c, 
		0xfadb762f, 
		0x55478b03, 
		0xf4ebd23b, 
		0xfe6c0230, 
		0x8e7554b9, 
		0xeed45e31, 
		0x5a7902db, 
		0x1ee0f787, 
		0xb94cd814, 
		0x049e09a7, 
		0x5a676401, 
		0x34256338, 
		0xdcbed24e, 
		0xd242dc4d, 
		0x41bae055, 
		0xc2e4aff8, 
		0x928ac92e, 
		0xb6a93103, 
		0x678c60a7, 
		0x999f92f8, 
		0x2c6a295e, 
		0xb79a914c, 
		0xbd9f0c02, 
		0x2cce7d18, 
		0xc5921332, 
		0x2cb48519, 
		0xf9f9fb04, 
		0xe961ce5d, 
		0xf0543baa, 
		0xad43755e, 
		0x2754306e, 
		0xe4f9c381, 
		0xb0619af6, 
		0x03491246, 
		0x6e86a6c4, 
		0xa330b428, 
		0xc352a1bc, 
		0xd4996fb7, 
		0x099152c8, 
		0xc6f03a22, 
		0xbebc24f1, 
		0xc438ad71, 
		0xf6ae0912, 
		0xbd35af1f, 
		0x0dedc8e2, 
		0x29a4a629, 
		0x9e260b43, 
		0x1b76a010, 
		0xa5b72926, 
		0x4aa8fc5a, 
		0x76cfc6e3, 
		0x026a31c5, 
		0xf7563b69, 
		0xb59d76ea, 
		0xea5f025f, 
		0x5211f316, 
		0x580351eb, 
		0xf21e4026, 
		0x7b2befa9, 
		0x5b1fbd76, 
		0xc83786b3, 
		0x9bf4d9b4, 
		0x3472cf73, 
		0x3d7b2a39, 
		0xfe19205c, 
		0x7346de8e, 
		0x9bc64d96, 
		0xef771f0f, 
		0x66d633e0, 
		0x0b297258, 
		0x3fd70829, 
		0xf2e47386, 
		0x76ad57bc, 
		0xd52a337e, 
		0xf3af153d, 
		0xa8b7fe83, 
		0x14cce894, 
		0x770e9fc7, 
		0xcbc151ae, 
		0x1e84a563, 
		0xa42d37e6, 
		0xcdd39505, 
		0x16f8ee52, 
		0x6eb46cd2, 
		0xa00d3a2f, 
		0x8a12804e, 
		0x1e7dc81b, 
		0x1e47785d, 
		0xc8e7e981, 
		0x35ead004, 
		0x8db3cd0b, 
		0x2fe45850, 
		0x1867c28c, 
		0xbf2c3f53, 
		0x9702ad6e, 
		0xd7a899bc, 
		0x7a36721e, 
		0x0e7dc3c0, 
		0xa1c9cdad, 
		0x4b0422ce, 
		0x1b07cb16, 
		0x7500123f, 
		0xf0d12124, 
		0xb67b47f5, 
		0xdd53d231, 
		0x443534ef, 
		0xe84a9b28, 
		0xff1f2c78, 
		0x2147ae40, 
		0xc267dc16, 
		0x5351facb, 
		0x5ca9dd28, 
		0x1c6e6cb4, 
		0x6cdaec7d, 
		0xd12856e0, 
		0xdc127f3e, 
		0x1ccf59a2, 
		0x548fc88a, 
		0x662e03ba, 
		0xfd0c979b, 
		0x7ad0cad8, 
		0x72928d3d, 
		0x6ecae4bf, 
		0xd2b3b9c7, 
		0x1a7a324d, 
		0xfd5fcdfc, 
		0x8ef091cd, 
		0x005b047b, 
		0x623f4ebd, 
		0xfb6b9d84, 
		0x3b2b5b5f, 
		0xa2196ea1, 
		0x5bc9b637, 
		0x4c72eb7f, 
		0x6c087016, 
		0x8b8ccc2c, 
		0xe51226e0, 
		0xe52e2781, 
		0x89e5844a, 
		0xc4d299dc, 
		0x74f60118, 
		0xa2e0f5d6, 
		0x57a24b21, 
		0x5a6f8842, 
		0xe3f769f5, 
		0xe3137016, 
		0xb69ed16c, 
		0x634721e2, 
		0x54fd2678, 
		0x8e183793, 
		0x540f3469, 
		0x2ff9f5f7, 
		0x862d15a0, 
		0x6076bbcd, 
		0x4265c08e, 
		0x771a1591, 
		0x1f506c47, 
		0x1099874c, 
		0xfe9c7ffa, 
		0xc907a703, 
		0xc2cfc7e8, 
		0x7629a9e1, 
		0xd1dde8ab, 
		0x791d9500, 
		0x870237f9, 
		0x1e612b71, 
		0x62bbfb51, 
		0x220c3926, 
		0x35042101, 
		0xc03c3454, 
		0x45bce122, 
		0x42e9bf7a, 
		0x4824977b, 
		0x0c49a984, 
		0xb2a9b172, 
		0x16913bd9, 
		0x204ee10f, 
		0x19d3a398, 
		0x0e4bc7a9, 
		0xa0874587, 
		0x80c02fef, 
		0x22248c50, 
		0x7fc02f86, 
		0xc0f7def6, 
		0x3a8f13ec, 
		0x0a037ada, 
		0x7ace263e, 
		0xd3cefa4c, 
		0xa86a13e6, 
		0x999737d2, 
		0x6631a459, 
		0x066a08ad, 
		0xeade361a, 
		0x1ea6fd05, 
		0x4eefce23, 
		0x1adde7e8, 
		0x89d38e38, 
		0xe45e58db, 
		0x324db09a, 
		0xce729550, 
		0xa22d11f6, 
		0x0aeb3312, 
		0x70cd3459, 
		0x3990e642, 
		0xc2813c89, 
		0xb2795e37, 
		0x58ed7625, 
		0x8cbd7e10, 
		0xe5b4a308, 
		0x02f9fa57, 
		0x256e734d, 
		0x1b3f88b6, 
		0x4d624e5e, 
		0xdd41a2d9, 
		0x0b99cc87, 
		0x21033432, 
		0x4ba97f19, 
		0xe7379589, 
		0x6c0a2f2a, 
		0xb47c162e, 
		0x8a703072, 
		0xaf8569a0, 
		0x385997d7, 
		0xabc3cd46, 
		0x6f0b10a7, 
		0xcda8a3aa, 
		0x5d75cca0, 
		0x80a6baaa, 
		0xa4d0a41d, 
		0x732d682b, 
		0x1d0cee9c, 
		0xa14a0e5e, 
		0xe9cb2e7d, 
		0xfaca5667, 
		0xf870be4e, 
		0xe9b37351, 
		0x5d2ab8e1, 
		0x16eb8d44, 
		0x722ce78c, 
		0x77b46847, 
		0x5439a63a, 
		0x74a87691, 
		0x8baa2cde, 
		0xa2ee1f99, 
		0xa667868a, 
		0x306cb3d3, 
		0x12e16b16, 
		0x8b591e52, 
		0x798efe84, 
		0x7a5ce974, 
		0x823ec45d, 
		0xd024a7cc, 
		0x7956360b  
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
		0x4c80, 
		0xc200, 
		0xce00, 
		0x6cb4, 
		0xbc00, 
		0x5040, 
		0xb8bd, 
		0x0000, 
		0xc700, 
		0xcec0, 
		0xcf80, 
		0x8000, 
		0x4980, 
		0xd0a0, 
		0xe7b7, 
		0xd060, 
		0x4d40, 
		0xc700, 
		0x7790, 
		0x4700, 
		0x52a0, 
		0x5020, 
		0xb562, 
		0x53a0, 
		0xd220, 
		0x1305, 
		0x4d40, 
		0x4600, 
		0xd380, 
		0xd000, 
		0xad16, 
		0xa035, 
		0x2eef, 
		0xc400, 
		0x0000, 
		0x4c17, 
		0x0000, 
		0x25fa, 
		0xd300, 
		0x4cc0, 
		0xbee1, 
		0xa140, 
		0xcc80, 
		0xd060, 
		0x8000, 
		0xa9c2, 
		0x4600, 
		0xcc80, 
		0xd3e0, 
		0x5260, 
		0x4900, 
		0x4800, 
		0xd060, 
		0x51c0, 
		0xd3e0, 
		0xcc80, 
		0x5d5c, 
		0x0000, 
		0x5020, 
		0x7c00, 
		0x90e6, 
		0x5020, 
		0xd240, 
		0xce40, 
		0x7c00, 
		0xc971, 
		0x4e00, 
		0x5280, 
		0xd040, 
		0x4e40, 
		0xc15f, 
		0xc400, 
		0x5cb4, 
		0x234d, 
		0x51a0, 
		0x4980, 
		0x9ee4, 
		0x5100, 
		0xcb00, 
		0x145d, 
		0x4e80, 
		0xcdc0, 
		0x8000, 
		0xd1c0, 
		0xfc00, 
		0x5340, 
		0xd140, 
		0x5060, 
		0xfc4d, 
		0xd0e0, 
		0x5060, 
		0xd380, 
		0x3c00, 
		0xd0a0, 
		0x5080, 
		0x1d98, 
		0xcf00, 
		0xfd46, 
		0xd240, 
		0x51e0, 
		0xd340, 
		0xcdae, 
		0x5140, 
		0x0000, 
		0x4f80, 
		0x52a0, 
		0xcc80, 
		0xd5ab, 
		0xd2a0, 
		0x5020, 
		0x5060, 
		0x4c80, 
		0x4e80, 
		0x51e0, 
		0x5320, 
		0x426c, 
		0xf7f5, 
		0xd3c0, 
		0x4200, 
		0x50e0, 
		0x0000, 
		0x4a80, 
		0x4664, 
		0xf04d, 
		0xd0c0, 
		0x654a, 
		0xc700, 
		0xf8ad, 
		0x0000, 
		0xd300, 
		0xbc00, 
		0xf4bb, 
		0xce00, 
		0x1df9, 
		0x773f, 
		0x4a00, 
		0x7c00, 
		0xb589, 
		0xd200, 
		0x52c0, 
		0xcc80, 
		0xa98f, 
		0x6471, 
		0xcc00, 
		0x4fc0, 
		0xd0a0, 
		0xcf40, 
		0x8000, 
		0xf97a, 
		0x52a0, 
		0x4f80, 
		0x4700, 
		0xa47d, 
		0xc4a5, 
		0x51e0, 
		0xd3c0, 
		0xdb83, 
		0x4c00, 
		0x5380, 
		0xd1c0, 
		0xd2c0, 
		0xc200, 
		0x914e, 
		0x6f5d, 
		0x4500, 
		0xd200, 
		0x537a, 
		0xd2a0, 
		0x7815, 
		0xde96, 
		0x190f, 
		0xcd00, 
		0x0320, 
		0x64fc, 
		0xcc40, 
		0x47f6, 
		0xc880, 
		0x5260, 
		0xc0a6, 
		0x5060, 
		0x14a3, 
		0x4fc0, 
		0x4900, 
		0x5340, 
		0xcb00, 
		0x0000, 
		0x2dec, 
		0xd7f9, 
		0x5300, 
		0xc000, 
		0x53a0, 
		0x52c0, 
		0xd260, 
		0x66ae, 
		0x4600, 
		0xcf40, 
		0xe297, 
		0x6a69, 
		0x5240, 
		0xf72e, 
		0x9374, 
		0xd140, 
		0xd120, 
		0xfd78, 
		0x4d40, 
		0xbedc, 
		0xcdc0, 
		0xd3c0, 
		0xce80, 
		0xce00, 
		0x3905, 
		0x9c0a, 
		0x5080, 
		0x8000, 
		0xcd00, 
		0x8000, 
		0xcc00, 
		0x4a00, 
		0xc800, 
		0xd2e0, 
		0x5320, 
		0xc980, 
		0x2758, 
		0x0a9d, 
		0x89d7, 
		0x50c0, 
		0xcec0, 
		0xf142, 
		0xd0c0, 
		0xed61, 
		0xce00, 
		0x50c0, 
		0x3ef1, 
		0x955e, 
		0x7c00, 
		0x0000, 
		0xd000, 
		0x4e00, 
		0x7866, 
		0xd3c0, 
		0xcd40, 
		0x4500, 
		0x0000, 
		0x0da9, 
		0x85ba, 
		0x620c, 
		0x92a2, 
		0xd180, 
		0xd260, 
		0xb96e, 
		0xcb00, 
		0x80f2, 
		0xd360, 
		0xd020, 
		0x7c00, 
		0x5a71, 
		0xc3d9, 
		0xca00, 
		0x4e40, 
		0xd040, 
		0x5220, 
		0x4200, 
		0xf132, 
		0x53c0, 
		0x4b00, 
		0xc000, 
		0x904a, 
		0xa177, 
		0x50a0, 
		0x23a5, 
		0x0000, 
		0xd92e, 
		0xd3a0, 
		0x4400, 
		0xce80, 
		0x09e9, 
		0xd2a0, 
		0x4f40, 
		0xd280, 
		0x83c3, 
		0xf89d, 
		0x5100, 
		0x5200, 
		0x5a5e, 
		0xd300, 
		0xfc00, 
		0x4f80, 
		0x837e, 
		0x4900, 
		0xd300, 
		0x4e40, 
		0xd360, 
		0xbbce, 
		0xcc80, 
		0x0000, 
		0xfd10, 
		0xcb00, 
		0x0000, 
		0xccc0, 
		0xd3c0, 
		0x243f, 
		0x2be5, 
		0x4a80, 
		0x4700, 
		0x605a, 
		0x0a0a, 
		0xc980, 
		0x4fc2, 
		0xc347, 
		0x5100, 
		0xfc00, 
		0x1ea2, 
		0x256b, 
		0xd220, 
		0x5020, 
		0x51a0, 
		0x53e0, 
		0x4980, 
		0x5200, 
		0xce40, 
		0x9182, 
		0x4a80, 
		0x62fa, 
		0x7733, 
		0xd100, 
		0x4800, 
		0x4c40, 
		0x4fc0, 
		0xd1e0, 
		0x4d00, 
		0xd2e0, 
		0xce80, 
		0x739b, 
		0xb9c0, 
		0xc800, 
		0x5100, 
		0x31b5, 
		0xce80, 
		0x4c00, 
		0x4e40, 
		0xbc00, 
		0xd2a0, 
		0xd1e0, 
		0x4800, 
		0xc980, 
		0x53e0, 
		0xcd40, 
		0x8000, 
		0xf68c, 
		0x8000, 
		0x5080, 
		0x5060, 
		0x780b, 
		0xcd80, 
		0xfc00, 
		0x4000, 
		0x824d, 
		0x8000, 
		0x4e40, 
		0xcc00, 
		0x5140, 
		0x77a8, 
		0x5300, 
		0xd140, 
		0x1619, 
		0x5ae8, 
		0x8000, 
		0xcf00, 
		0xfc00, 
		0x5220, 
		0xd180, 
		0x8e40, 
		0x7dd7, 
		0x8a9f, 
		0x7c00, 
		0xc700, 
		0x4d80, 
		0x7177, 
		0x5180, 
		0x4400, 
		0xcf40, 
		0x0000, 
		0xd56f, 
		0xd0e0, 
		0xc700, 
		0x5100, 
		0xaae7, 
		0xd100, 
		0x4d80, 
		0xfe9c, 
		0x4d40, 
		0x7d94, 
		0x3ff9, 
		0xc900, 
		0xd200, 
		0xec51, 
		0x4700, 
		0x3c00, 
		0x4d80, 
		0x4700, 
		0xd36f, 
		0xc000, 
		0x4ec0, 
		0x52e0, 
		0xce62, 
		0xd3ab, 
		0xc700, 
		0xd300, 
		0x51c0, 
		0x7c00, 
		0xeb34, 
		0xd380, 
		0xd1e0, 
		0x4200, 
		0xca80, 
		0x4b80, 
		0x4b00, 
		0xcdc0, 
		0xd1e0, 
		0xd3c0, 
		0x7c00, 
		0xfc00, 
		0xb53c, 
		0x5060, 
		0xd0a0, 
		0x4b00, 
		0x4700, 
		0xe88a, 
		0xd3a0, 
		0x5280, 
		0x5240, 
		0x51e0, 
		0xcfc0, 
		0xdd2f, 
		0x5d6a, 
		0x0000, 
		0x5020, 
		0xb740, 
		0xa233, 
		0x51e0, 
		0xc11c, 
		0x7a33, 
		0x1a66, 
		0xc800, 
		0xd1a0, 
		0x5020, 
		0x8078, 
		0xcfc0, 
		0xd0c0, 
		0x5220, 
		0xd380, 
		0xc847, 
		0x4d40, 
		0x0000, 
		0x52c0, 
		0xbc00, 
		0x4f00, 
		0xcd40, 
		0xcdc0, 
		0x69d4, 
		0xf5b4, 
		0x5280, 
		0x4f40, 
		0xa4e5, 
		0xc400, 
		0xc500, 
		0xd1e0, 
		0xcdc0, 
		0xd380, 
		0x1605, 
		0x4c40, 
		0x4980, 
		0xe879, 
		0xfc00, 
		0x4e40, 
		0xcc80, 
		0xd180, 
		0xcec0, 
		0x1aa6, 
		0xa9e3, 
		0x4980, 
		0xc600, 
		0x0000, 
		0x3311, 
		0xcb00, 
		0x5040, 
		0x4fc0, 
		0xfc0e, 
		0xd320, 
		0xd1e0, 
		0x8dc5, 
		0xc35b, 
		0xc900, 
		0xb93a, 
		0xc600, 
		0xc400, 
		0xd1c0, 
		0x932f, 
		0x97d9, 
		0xd2a0, 
		0xd080, 
		0x0000, 
		0x51c0, 
		0xbc00, 
		0x4e96, 
		0x12b5, 
		0x5280, 
		0x5100, 
		0xc700, 
		0xd200, 
		0x5020, 
		0x5060, 
		0xd360, 
		0xce00, 
		0x4e80, 
		0x5360, 
		0x0640, 
		0xcd00, 
		0x4700, 
		0xd000, 
		0xcd80, 
		0x6591, 
		0x8000, 
		0xed66, 
		0x5200, 
		0x4880, 
		0x50c0, 
		0x8d25, 
		0xd380, 
		0x8000, 
		0x4fc0, 
		0x5380, 
		0x53a0, 
		0x6c78, 
		0x7c00, 
		0x5380, 
		0xa290, 
		0x50a0, 
		0x0000, 
		0xb9f5, 
		0x5140, 
		0xc600, 
		0xd120, 
		0x5240, 
		0xe9a1, 
		0x5300, 
		0xca00, 
		0xa2d4, 
		0x5120, 
		0x5020, 
		0xd260, 
		0xc980, 
		0x5b2c, 
		0xc400, 
		0x8000, 
		0x5140, 
		0x4f40, 
		0x79b1, 
		0x4b00, 
		0x7c00, 
		0x4980, 
		0x4f00, 
		0x4fc0, 
		0xc000, 
		0x5140, 
		0x52e0, 
		0x6857, 
		0x5040, 
		0x4880, 
		0x09f5, 
		0x3372, 
		0xfc00, 
		0xd0c0, 
		0x7cf7, 
		0x0000, 
		0xc600, 
		0xd200, 
		0x4e40, 
		0xd280, 
		0x4dc0, 
		0xcb00, 
		0x0000, 
		0x5020, 
		0xfc00, 
		0x8000, 
		0x85c9, 
		0x53a0, 
		0x53c0, 
		0x4836, 
		0xcf00, 
		0x4500, 
		0x7c00, 
		0x8000, 
		0x55d4, 
		0x39e7, 
		0xf9eb, 
		0xd200, 
		0xe28c, 
		0xce3a, 
		0xbffc, 
		0x27cd, 
		0xfffc, 
		0x4b80, 
		0x765b, 
		0x1dcd, 
		0x52a0, 
		0x4f40, 
		0xa308, 
		0x4dc0, 
		0xd2e0, 
		0x4b80, 
		0xc034, 
		0x4f74, 
		0xc600, 
		0xd1e0, 
		0xb62d, 
		0xd3e0, 
		0xd300, 
		0xc800, 
		0x4980, 
		0x50c0, 
		0xbc00, 
		0xcb80, 
		0x50c0, 
		0x7c31, 
		0xcf6b, 
		0xca80, 
		0x4000, 
		0x1a6c, 
		0x52c0, 
		0x4f00, 
		0xe6d0, 
		0xaf0a, 
		0x5220, 
		0xd100, 
		0xd1a0, 
		0xd320, 
		0x53c0, 
		0x2ab1, 
		0x0000, 
		0xd080, 
		0x44d1, 
		0x6295, 
		0x5020, 
		0xcc40, 
		0xd340, 
		0x4900, 
		0xcf80, 
		0x7c00, 
		0x2c93, 
		0xd000, 
		0xb080, 
		0x4c33, 
		0xcc80, 
		0x4900, 
		0x12e7, 
		0xce40, 
		0x3a1f, 
		0xac1c, 
		0xce40, 
		0xc000, 
		0xbc00, 
		0xd1c0, 
		0x4fc0, 
		0xd220, 
		0x5180, 
		0x0000, 
		0x4d00, 
		0x5160, 
		0x70a9, 
		0x4e40, 
		0xd522, 
		0xd080, 
		0x0000, 
		0xd1c0, 
		0x459d, 
		0xcd40, 
		0xce40, 
		0xd280, 
		0xd020, 
		0xf407, 
		0x5000, 
		0x4e00, 
		0xcc00, 
		0x50a0, 
		0xd380, 
		0xd220, 
		0x98bc, 
		0xd020, 
		0xe2f9, 
		0x5280, 
		0xcb00, 
		0x4dc0, 
		0xfc00, 
		0x53e0, 
		0xc400, 
		0x53e0, 
		0x4c40, 
		0x8000, 
		0xb140, 
		0xa712, 
		0xaf79, 
		0xbc00, 
		0x58d2, 
		0x51c0, 
		0x4500, 
		0x972e, 
		0x50e0, 
		0x0000, 
		0x51c0, 
		0x4980, 
		0x4d00, 
		0x4d40, 
		0xc880, 
		0x50c0, 
		0x64b8, 
		0x7c00, 
		0x5080, 
		0xf77f, 
		0x4880, 
		0x33e2, 
		0x5320, 
		0x8000, 
		0xd100, 
		0x8000, 
		0xedb1, 
		0x4fc0, 
		0x5280, 
		0x0000, 
		0x2871, 
		0x4200, 
		0x9472, 
		0x4d40, 
		0x3c00, 
		0xc700, 
		0xa1cb, 
		0x50e0, 
		0x5200, 
		0x0000, 
		0x5a44, 
		0x1a8a, 
		0x0000, 
		0x9f47, 
		0x50e0, 
		0x5340, 
		0x8000, 
		0x7565, 
		0x5020, 
		0xa30b, 
		0x8da9, 
		0x5040, 
		0x5243, 
		0x345b, 
		0xc980, 
		0xc900, 
		0x0073, 
		0x0000, 
		0x6a96, 
		0x5b9c, 
		0xcb00, 
		0xd3a0, 
		0x3fe0, 
		0x4c00, 
		0xd200, 
		0xce40, 
		0xc400, 
		0x52a0, 
		0xd000, 
		0xf6e4, 
		0x7893, 
		0xacfe, 
		0xa614, 
		0x671d, 
		0x51c0, 
		0xb269, 
		0x102b, 
		0xcd80, 
		0xfd90, 
		0xd846, 
		0x965b, 
		0x0000, 
		0x3c00, 
		0xcc40, 
		0xd300, 
		0xc600, 
		0x4d80, 
		0x5100, 
		0x4ec0, 
		0xd340, 
		0x5020, 
		0x4f00, 
		0xd3e0, 
		0x7059, 
		0x0a57, 
		0x5160, 
		0x9b7d, 
		0xd200, 
		0xd0e0, 
		0x4f80, 
		0x9493, 
		0x4c80, 
		0x54d6, 
		0xfcd9, 
		0xfc00, 
		0xd0e0, 
		0xff3a, 
		0xcb80, 
		0x72ea, 
		0x7c00, 
		0x5e4e, 
		0xd180, 
		0x8000, 
		0xca00, 
		0x5320, 
		0x8000, 
		0x8442, 
		0xc200, 
		0x0000, 
		0x7747, 
		0xd380, 
		0x4800, 
		0x8f71, 
		0xc600, 
		0x4ec0, 
		0x4880, 
		0xc075, 
		0x5300, 
		0xcd40, 
		0x5240, 
		0xd240, 
		0xda22, 
		0x53e0, 
		0xd360, 
		0x4000, 
		0x4600, 
		0x3b33, 
		0x4f40, 
		0x0000, 
		0x0000, 
		0xce00, 
		0xce80, 
		0xb660, 
		0xd300, 
		0xd3c0, 
		0x5140, 
		0x5240, 
		0x5180, 
		0x5000, 
		0xd2e0, 
		0xccb6, 
		0xc800, 
		0x82d6, 
		0x5220, 
		0xabe5, 
		0x3490, 
		0x5260, 
		0x4d00, 
		0x0117, 
		0xabc4, 
		0xc600, 
		0xd0c0, 
		0x4b85, 
		0xa711, 
		0x1acf, 
		0xcf80, 
		0xd380, 
		0xc987, 
		0xcc80, 
		0x52e0, 
		0xd1e0, 
		0x8000, 
		0x0c10, 
		0x5220, 
		0xcec0, 
		0x5260, 
		0xfc00, 
		0x78be, 
		0x5020, 
		0x8fe5, 
		0x53e0, 
		0x5000, 
		0xd000, 
		0x590a, 
		0x9818, 
		0x2e08, 
		0x4b80, 
		0xd2e0, 
		0x2fe4, 
		0x4400, 
		0x4dc0, 
		0x8000, 
		0xe8e3, 
		0xd1c0, 
		0x5280, 
		0xfc00, 
		0x5300, 
		0xaae4, 
		0xd280, 
		0x51a0, 
		0x9c15, 
		0x51c0, 
		0xc296, 
		0xccc0, 
		0x4a80, 
		0xd200, 
		0xd320, 
		0x4d40, 
		0x5100, 
		0x7d4a, 
		0x52a0, 
		0x4800, 
		0xcf80, 
		0xcf00, 
		0x1e9f, 
		0xd144, 
		0xca00, 
		0x8000, 
		0x543f, 
		0x50c0, 
		0x911b, 
		0xd260, 
		0x4acc, 
		0x4d00, 
		0xab6e, 
		0x6f8a, 
		0xd060, 
		0x5140, 
		0xa649, 
		0x4853, 
		0xce40, 
		0x5220, 
		0x4f40, 
		0x7c00, 
		0x4345, 
		0xcec0, 
		0xc400, 
		0x1212, 
		0x4e21, 
		0x5260, 
		0x5220, 
		0x4cc0, 
		0x0000, 
		0x8000, 
		0x5340, 
		0x8c4d, 
		0xcf80, 
		0xa871, 
		0xd3e0, 
		0x4d80, 
		0xf2bb, 
		0xd000, 
		0x5247, 
		0xfdb5, 
		0x4e80, 
		0xcd80, 
		0xc980, 
		0x4cc0, 
		0xd0e0, 
		0x5380, 
		0x5e2a, 
		0xd160, 
		0xd120, 
		0x53a0, 
		0xb797, 
		0x50e0, 
		0x7c00, 
		0x4000, 
		0x5200, 
		0xcf00, 
		0xc200, 
		0xd020, 
		0x8000, 
		0xfc00, 
		0x4d00, 
		0xd2a0, 
		0x5000, 
		0x9677, 
		0x4ec0, 
		0xcfc0, 
		0xc778, 
		0xd1a0, 
		0x4b00, 
		0xce80, 
		0xcfc0, 
		0x4d00, 
		0x5160, 
		0x5080, 
		0x7c00, 
		0x5100, 
		0x5532, 
		0x8b7e, 
		0xfc00, 
		0x76ad, 
		0x7d7b, 
		0xd1a0, 
		0x4400, 
		0xa063, 
		0xfc00, 
		0x3542, 
		0x4500, 
		0xc3ba, 
		0x5180, 
		0x6a37, 
		0x11f4, 
		0x8348, 
		0xcc00, 
		0xdb84, 
		0x5040, 
		0x51a0, 
		0x9281, 
		0xbc00, 
		0x4d00, 
		0x4700, 
		0x4400, 
		0xc600, 
		0x5100, 
		0xbc00, 
		0x5240, 
		0x20b9, 
		0xce80, 
		0x4900, 
		0x75d2, 
		0xca80, 
		0xbb78, 
		0xfaaa, 
		0xd020, 
		0x4e40, 
		0x7c00, 
		0x495f, 
		0x5100, 
		0x8000, 
		0x9363, 
		0xd360, 
		0x13d4, 
		0x0000, 
		0xfd0a, 
		0xcf40, 
		0xcc80, 
		0xabe1, 
		0x4d40, 
		0x5380, 
		0xe5f9, 
		0x4e00, 
		0x52c0, 
		0xfc95, 
		0x2e9b, 
		0x568e, 
		0xf6c4, 
		0xd380, 
		0x0000, 
		0xd1c0, 
		0x6922, 
		0x8000, 
		0x3c00, 
		0x8000, 
		0x5060, 
		0x8b2a, 
		0xcd80, 
		0x4fc0, 
		0xcb80, 
		0x4000, 
		0xd120, 
		0x5120, 
		0x4f00, 
		0xcd80, 
		0x4f00, 
		0xbc00, 
		0xd320, 
		0xd200, 
		0x497b, 
		0xfc00, 
		0x5340, 
		0x52a0, 
		0x4200, 
		0xcf80, 
		0xef88, 
		0xd0a0, 
		0x7c00, 
		0xaaf9, 
		0x53a0, 
		0x50e0, 
		0xd2e0, 
		0xcc00, 
		0x5000, 
		0x2dab, 
		0xc500, 
		0xce40, 
		0x4200, 
		0xfdf0, 
		0x5140, 
		0x5280, 
		0x4a00, 
		0x7cac, 
		0xf5b0, 
		0xfccf, 
		0x0000, 
		0x4cc0, 
		0xd3c0, 
		0x6142, 
		0xce80, 
		0xcf80, 
		0x1007, 
		0xbb2e, 
		0x3ee9, 
		0xc400, 
		0x3c00, 
		0x6c16, 
		0xd0e0, 
		0xf0e4, 
		0x4000, 
		0xfc00, 
		0x819c, 
		0x0000, 
		0x4400, 
		0xc880, 
		0xc600, 
		0x0000, 
		0x5240, 
		0x860f, 
		0x4800, 
		0x4c00, 
		0x53c1, 
		0x0000, 
		0x4cc0, 
		0x5300, 
		0x80e5, 
		0x840c, 
		0x46d0, 
		0x5140, 
		0xd000, 
		0x4a00, 
		0xd140, 
		0x79c1, 
		0xfc00, 
		0xd120, 
		0x51e0, 
		0xd42f, 
		0xc900, 
		0x7994, 
		0x72f4, 
		0xcec0, 
		0x4e80, 
		0x5100, 
		0x4f80, 
		0xb858, 
		0xcc40, 
		0xd3c0, 
		0xcdc0, 
		0x0000, 
		0xd3e0, 
		0xca00, 
		0x78b7, 
		0x4b00, 
		0x525c, 
		0x7c00, 
		0x41d7, 
		0xd280, 
		0x5140, 
		0x37ce, 
		0xd2a0, 
		0x9ec2, 
		0x0000, 
		0x78a2, 
		0x0be0, 
		0x22ed, 
		0xd0e0, 
		0x4455, 
		0x4e80, 
		0x4f80, 
		0xfc00, 
		0x9656, 
		0x50c0, 
		0xf9d8, 
		0xd260, 
		0x52c0, 
		0xa004, 
		0x4e80, 
		0xde98, 
		0xd2e0, 
		0x5380, 
		0x5240, 
		0x0000, 
		0x51a0, 
		0xc500, 
		0xcb80, 
		0x5eec, 
		0xfc00, 
		0xcb80, 
		0xce80, 
		0xd847, 
		0x95e2, 
		0xc200, 
		0xd2a0, 
		0x7c6e, 
		0xfbf2, 
		0xa4a7, 
		0xce40, 
		0x59c5, 
		0x0b43, 
		0x3c00, 
		0x4900, 
		0x30cc, 
		0x5000, 
		0x52e0, 
		0xd426, 
		0x9473, 
		0xc700, 
		0x5180, 
		0xcc00, 
		0x4600, 
		0xbc00, 
		0x8941, 
		0xb74b, 
		0x9004, 
		0x5aaa, 
		0xc5e4, 
		0xd3a0, 
		0x4600, 
		0x9e65, 
		0xd2c0, 
		0x11bc, 
		0x4b00, 
		0xccc0, 
		0xc800, 
		0xd240, 
		0xc500, 
		0x1e69, 
		0xce40, 
		0x4d80, 
		0x5320, 
		0x8000, 
		0xd180, 
		0x4880, 
		0xd020, 
		0x9da2, 
		0x51e0, 
		0x3c00, 
		0xd020, 
		0x064f, 
		0x4e00, 
		0xcec0, 
		0x7c00, 
		0x820c, 
		0x363d, 
		0xcc74, 
		0xd300, 
		0x4fc0, 
		0x5a5e, 
		0x2a5f, 
		0xd020, 
		0x5280, 
		0x5060, 
		0x5300, 
		0x4d00, 
		0x4d00, 
		0x4c40, 
		0x9b87, 
		0xc900, 
		0xcdc0, 
		0xcffd, 
		0xc600, 
		0x5100, 
		0x032d, 
		0xd380, 
		0x5040, 
		0xfc00, 
		0xd020, 
		0x4d80, 
		0xb59b, 
		0xcd00, 
		0x0000, 
		0x3b7d, 
		0xcf40, 
		0x12ad, 
		0xd280, 
		0xbc00, 
		0xd100, 
		0x4e00, 
		0x0000, 
		0xe7a8, 
		0x4d40, 
		0xd1c0, 
		0x8000, 
		0xaf0c, 
		0xd300, 
		0x51c0, 
		0x3016, 
		0xd1c0, 
		0x4dc0, 
		0xd3a0, 
		0x4b00, 
		0x7c00, 
		0xfc4f, 
		0x5260, 
		0x853f, 
		0xca00, 
		0xce00, 
		0xd3a0, 
		0x4f40, 
		0x526b, 
		0xf22d, 
		0x3c8e, 
		0x4880, 
		0x4e80, 
		0x7db7, 
		0x52c0, 
		0xce40, 
		0xc633, 
		0x2b7d, 
		0xc400, 
		0x4400, 
		0x5300, 
		0x5200, 
		0x3a59, 
		0x5180, 
		0x7c00, 
		0xf4eb, 
		0xc000, 
		0x4000, 
		0xa9fa, 
		0xcf00, 
		0x52c0, 
		0x5020, 
		0x5360, 
		0x5200, 
		0xd2e0, 
		0x35c2, 
		0xd100, 
		0xce40, 
		0x73f3, 
		0xf8f6, 
		0x3e08, 
		0xd240, 
		0x4500, 
		0xf19a, 
		0x6a02, 
		0x5080, 
		0x4e40, 
		0xc800, 
		0xfcdf, 
		0x8000, 
		0x2f7d, 
		0xfc00, 
		0xa149, 
		0xd1c0, 
		0xc000, 
		0xd140, 
		0xd360, 
		0x8000, 
		0x41f5, 
		0x1162, 
		0xac38, 
		0xca23, 
		0xcabc, 
		0x7c00, 
		0xd040, 
		0x5120, 
		0xd0a0, 
		0xcb80, 
		0x2866, 
		0xce80, 
		0xac93, 
		0x8b8f, 
		0xd080, 
		0x52a0, 
		0x50a0, 
		0xd883, 
		0x3c37, 
		0xcc00, 
		0x0000, 
		0xd3e0, 
		0x4400, 
		0xcc00, 
		0x980f, 
		0x4fc0, 
		0xd040, 
		0xcc80, 
		0x53e0, 
		0x4880, 
		0x5220, 
		0xd2c0, 
		0xd260, 
		0x4000, 
		0x452e, 
		0x4cc0, 
		0xd3c0, 
		0x4dc0, 
		0x3c00, 
		0x5140, 
		0x5260, 
		0x4d40, 
		0xcd80, 
		0xd020, 
		0x50c0, 
		0xef74, 
		0x4000, 
		0xb93d, 
		0x0000, 
		0xcb00, 
		0x905a, 
		0x0000, 
		0x7bca, 
		0x9189, 
		0x4200, 
		0x52c0, 
		0xcf40, 
		0x5000, 
		0x3b13, 
		0x4d40, 
		0x200f, 
		0xcf00, 
		0x7861, 
		0xcec0, 
		0x5360, 
		0x584f, 
		0xcf00, 
		0xcdc0, 
		0xcb00, 
		0x60bc, 
		0x53e0, 
		0xfe70, 
		0x53a0, 
		0x52a0, 
		0x0000, 
		0x3f9f, 
		0xd0e0, 
		0x4900, 
		0xbc00, 
		0x8065, 
		0xc400, 
		0x4c00, 
		0xcc00, 
		0x4a00, 
		0x5060, 
		0x5260, 
		0x4e40, 
		0x4800, 
		0xcd80, 
		0x4c80, 
		0xce80, 
		0x5040, 
		0x5b64, 
		0x4b00, 
		0x3662, 
		0xce40, 
		0xd160, 
		0x7c00, 
		0x4f00, 
		0x0000, 
		0x0e23, 
		0xd260, 
		0xfc00, 
		0xc880, 
		0xc980, 
		0xfc00, 
		0x4ec0, 
		0x5160, 
		0xd360, 
		0x8278, 
		0xfbf0, 
		0xd2a0, 
		0xd080, 
		0x53a0, 
		0x51e0, 
		0x5200, 
		0xb107, 
		0x51c0, 
		0x4b00, 
		0xd100, 
		0xd260, 
		0xcf40, 
		0x8000, 
		0x4b94, 
		0xcc80, 
		0x34c2, 
		0x4cc0, 
		0x287e, 
		0x5160, 
		0x6cf1, 
		0x0e91, 
		0x1da2, 
		0xd1e0, 
		0xeda6, 
		0xc700, 
		0x4700, 
		0x8450, 
		0x9859, 
		0x4d40, 
		0xd2c0, 
		0x8000, 
		0xde7b, 
		0x51c0, 
		0x7a2a, 
		0xd1c0, 
		0xd380, 
		0x5140, 
		0x4a00, 
		0xcf9a, 
		0xc000, 
		0xd260, 
		0x5060, 
		0xcfc0, 
		0x51a0, 
		0xb992, 
		0x4980, 
		0x51a0, 
		0x5120, 
		0xfd82, 
		0x4000, 
		0x5380, 
		0xd1a0, 
		0x4a80, 
		0x4c40, 
		0x1894, 
		0x396d, 
		0x5380, 
		0x4e80, 
		0xd0e0, 
		0xace3, 
		0xd2a0, 
		0xd0a0, 
		0x5000, 
		0xe679, 
		0x656d, 
		0xfafb, 
		0x56b3, 
		0x5100, 
		0xc23c, 
		0xd3a0, 
		0xd240, 
		0xc200, 
		0x4900, 
		0xc800, 
		0x8000, 
		0xc500, 
		0x50ec  
};
volatile uint32_t rand_ieee754_32[1536] __attribute__ ((aligned (2048))) = {
		0x807313bc, 
		0x00000000, 
		0x42740000, 
		0x00000002, 
		0xd7909924, 
		0x80000000, 
		0x72917a61, 
		0xc2480000, 
		0x49362172, 
		0x5c1261e2, 
		0xff11f2f1, 
		0xdad73ad4, 
		0xff7fffff, 
		0x80000002, 
		0x50077926, 
		0xff7fffff, 
		0x5d781923, 
		0x80002000, 
		0x0c7ffffe, 
		0x55555555, 
		0x80000000, 
		0x003a2f7f, 
		0x56dc881a, 
		0xc1f80000, 
		0x3f028f5c, 
		0x7f800000, 
		0x4b000000, 
		0x7f92cc53, 
		0x7faa7ec3, 
		0x7f7ffffe, 
		0xc2c07dfd, 
		0xff800000, 
		0x80004000, 
		0x41d00000, 
		0x73b35c85, 
		0xc1e00000, 
		0x00800003, 
		0x94c74537, 
		0x42500000, 
		0x42080000, 
		0x09312f15, 
		0x7fc00001, 
		0x42480000, 
		0x80800003, 
		0xc2400000, 
		0x2312d050, 
		0x10a059dc, 
		0x42340000, 
		0xd97aeb0d, 
		0x5a632be9, 
		0x00000000, 
		0x41a80000, 
		0xc2640000, 
		0x41e80000, 
		0xff8ba2e3, 
		0x0e001fff, 
		0x7f800000, 
		0x0e000001, 
		0x0e000007, 
		0x7f800000, 
		0xbb090119, 
		0x3f800000, 
		0x0e000003, 
		0x80800001, 
		0x0e00000f, 
		0x451ccf43, 
		0x00000000, 
		0x41b00000, 
		0xbf800001, 
		0xffc01b76, 
		0x40000000, 
		0x8f7ffffd, 
		0x0c400000, 
		0x424c0000, 
		0x40a00000, 
		0x70cb98dc, 
		0x24b9e404, 
		0xe2ccb5b0, 
		0x78a46c32, 
		0xc26c0000, 
		0x7f800000, 
		0x00400000, 
		0x20823eb7, 
		0xc1a80000, 
		0x0c7ffffe, 
		0x80011111, 
		0x7c1e501d, 
		0x00200000, 
		0xffc00001, 
		0x7f800000, 
		0xc2680000, 
		0x4dd31748, 
		0x002059fc, 
		0x00000000, 
		0x00ffffff, 
		0xc1e80000, 
		0x80000001, 
		0x0e000007, 
		0xc2700000, 
		0x80800001, 
		0x3ddbdbbd, 
		0xc21c0000, 
		0xc2040000, 
		0xff800000, 
		0x00000001, 
		0x0e7fffff, 
		0x80000000, 
		0x8f7fffff, 
		0x41880000, 
		0x33333333, 
		0x5432dd6f, 
		0x33333333, 
		0xbf800001, 
		0x80000000, 
		0xa3a89035, 
		0xff7ffffe, 
		0x80000004, 
		0xc23c0000, 
		0x422c0000, 
		0x0e007fff, 
		0x95f1f61d, 
		0x42380000, 
		0x7fef5062, 
		0x0c7ffc00, 
		0x93e84d5a, 
		0x007fffff, 
		0xc1f00000, 
		0x00000800, 
		0xaea45d81, 
		0xcc6caa49, 
		0xff800000, 
		0x33333333, 
		0x1d8d8414, 
		0x42600000, 
		0x7fbf38d4, 
		0x00000040, 
		0x00011111, 
		0xc23c0000, 
		0xcb8c4b40, 
		0x00800002, 
		0x0c7fffe0, 
		0x80000000, 
		0x41c00000, 
		0x42440000, 
		0x41600000, 
		0x80000000, 
		0x0e0003ff, 
		0x6aee956a, 
		0xc2080000, 
		0xff7fffff, 
		0xc1980000, 
		0x0e00000f, 
		0x0e0007ff, 
		0x7f800000, 
		0xff800000, 
		0x807fffff, 
		0xcdb83789, 
		0x7f800000, 
		0x325cbbc1, 
		0xc1f00000, 
		0x4f84a2fa, 
		0x0c7c0000, 
		0xd8478849, 
		0x7f800000, 
		0x80000200, 
		0x40a00000, 
		0xc1900000, 
		0x7f7ffffe, 
		0xc2480000, 
		0x41d80000, 
		0x41600000, 
		0x0e1fffff, 
		0xc2380000, 
		0x80000000, 
		0x00000000, 
		0x7f800000, 
		0x41500000, 
		0xff2785d5, 
		0xc2240000, 
		0xff800000, 
		0x6b5133e2, 
		0x8f7ffffe, 
		0xff7ffffe, 
		0x42180000, 
		0x80010000, 
		0x80186c95, 
		0xff800000, 
		0xa397e627, 
		0x9ad8e72d, 
		0x80020000, 
		0x0c700000, 
		0x80800001, 
		0xb893d564, 
		0x0e0003ff, 
		0x22476a37, 
		0x7f800000, 
		0xc1d80000, 
		0x00000000, 
		0xff800000, 
		0x7d738021, 
		0x3f800000, 
		0x80000000, 
		0x42400000, 
		0x00000000, 
		0xfc99cf9d, 
		0x9f7ed794, 
		0xc2740000, 
		0x80000000, 
		0x74e31411, 
		0x1bbf55cc, 
		0xcccccccc, 
		0x00000002, 
		0x7dd7b13e, 
		0x80000080, 
		0xc2580000, 
		0xc1980000, 
		0x00000002, 
		0x0c7ffe00, 
		0x80800000, 
		0x0e3fffff, 
		0x41980000, 
		0xc2240000, 
		0x36835f95, 
		0x42680000, 
		0x42140000, 
		0xc2eb7910, 
		0xc1600000, 
		0x8044003e, 
		0xc24c0000, 
		0xc1900000, 
		0x80800000, 
		0x00004000, 
		0x7f000000, 
		0x0c7fff00, 
		0x0c7ffe00, 
		0xff800000, 
		0x80800000, 
		0x42100000, 
		0x42140000, 
		0x40a00000, 
		0x80000400, 
		0x42080000, 
		0x80200000, 
		0x420c0000, 
		0x0e3fffff, 
		0xaaaaaaaa, 
		0xcb8c4b40, 
		0xffc00001, 
		0x80000080, 
		0x35fbc1ce, 
		0x7fbfffff, 
		0x42100000, 
		0x8f7ffffd, 
		0x254d1924, 
		0x00000008, 
		0xff800000, 
		0x00400000, 
		0x00010000, 
		0x80000000, 
		0xf0bb7bc9, 
		0x80000080, 
		0x00000000, 
		0x7f7ffffe, 
		0x7089fd07, 
		0xc1980000, 
		0x0d362dd2, 
		0xc2000000, 
		0x00000200, 
		0x9f0cb9dc, 
		0xc1b00000, 
		0xc1300000, 
		0x896dee06, 
		0x42580000, 
		0x80000400, 
		0x00000008, 
		0x00800001, 
		0xc2340000, 
		0x4fc0eff6, 
		0xc2180000, 
		0x00800001, 
		0x0c7fffc0, 
		0x80000040, 
		0x00000000, 
		0x00008000, 
		0x04535050, 
		0x80000000, 
		0x7f800000, 
		0x321f0b00, 
		0x00000001, 
		0x00c77338, 
		0x00000020, 
		0x0c7e0000, 
		0xff7ffffe, 
		0xc1980000, 
		0x00000000, 
		0x42700000, 
		0x00000010, 
		0x80000001, 
		0x41400000, 
		0x42040000, 
		0x9a9e9cea, 
		0xc1000000, 
		0x0e0001ff, 
		0x80000000, 
		0xfa4215af, 
		0x0e000fff, 
		0x426c0000, 
		0xc23c0000, 
		0xfe703fb7, 
		0xff800000, 
		0xff800001, 
		0x41100000, 
		0xff7ffffe, 
		0x18846b55, 
		0x8e44813d, 
		0xc1000000, 
		0x8013540f, 
		0xff800000, 
		0x7f000000, 
		0x0e007fff, 
		0x00000001, 
		0x80800003, 
		0x0c7ffe00, 
		0xc2540000, 
		0x8804a346, 
		0x41c00000, 
		0x41900000, 
		0x7f800000, 
		0xe74e69c3, 
		0x79562400, 
		0x8036dbe2, 
		0x80000000, 
		0xc0800000, 
		0xbf800001, 
		0x42040000, 
		0x7f800000, 
		0xc1600000, 
		0x57bf70d8, 
		0x80200000, 
		0x0e000007, 
		0xc21c0000, 
		0xffa64dee, 
		0x80000001, 
		0x80800001, 
		0x0e00000f, 
		0x80000000, 
		0x00000000, 
		0x0f7ffffc, 
		0x41a80000, 
		0x00ffffff, 
		0x0be5baaf, 
		0xc2200000, 
		0x7c3d2dbc, 
		0x80000020, 
		0x00400000, 
		0x1c581e6b, 
		0xbcac888e, 
		0x80008000, 
		0xc2280000, 
		0x00000020, 
		0xc24c0000, 
		0x42180000, 
		0x80800000, 
		0x13175789, 
		0x7fff2498, 
		0x245625fa, 
		0x7f800000, 
		0x73179fd9, 
		0x425c0000, 
		0x80000001, 
		0xa9f0e448, 
		0xff7fffff, 
		0x0c7ffffc, 
		0x0e001fff, 
		0xbf800001, 
		0x0f7ffffc, 
		0x7fbfffff, 
		0x00000000, 
		0x00200000, 
		0x80000000, 
		0x7fffffff, 
		0xcccccccc, 
		0x00800002, 
		0x00800002, 
		0x0c7ffffe, 
		0x0c7fe000, 
		0xc2140000, 
		0x42580000, 
		0x0f7ffffd, 
		0x422c0000, 
		0x83127bff, 
		0x3ea253bf, 
		0x7f7ffffe, 
		0x3de99c5d, 
		0xc4a5eb85, 
		0x00000200, 
		0x80000001, 
		0x7fbfffff, 
		0x42040000, 
		0xc1f00000, 
		0xc4a95aa4, 
		0x00100000, 
		0x80040000, 
		0x8f7fffff, 
		0x4f4344d9, 
		0xc1f00000, 
		0xc2440000, 
		0xff800000, 
		0x1dce1f6d, 
		0x00800001, 
		0x00040000, 
		0x7f7ffffe, 
		0x7fffffff, 
		0x007fffff, 
		0xe0aa20e4, 
		0x4b8c4b40, 
		0x80000000, 
		0x007fffff, 
		0x9148c897, 
		0xff9629f1, 
		0xe523526f, 
		0x58e20716, 
		0x80000004, 
		0x0e00001f, 
		0x4881a4d3, 
		0x42400000, 
		0x86bbcc9a, 
		0x0c7ffff8, 
		0xc1f00000, 
		0x0f7ffffc, 
		0x00800000, 
		0x00000000, 
		0x80080000, 
		0x7fbfffff, 
		0x0e001fff, 
		0x0e00ffff, 
		0x80000001, 
		0x0c7ffffc, 
		0x00001000, 
		0x00000000, 
		0xff7fffff, 
		0xc1600000, 
		0x80000020, 
		0x8673938c, 
		0x257e7521, 
		0x7510126b, 
		0xff800000, 
		0xc0c00000, 
		0x41100000, 
		0x1abf5769, 
		0x421c0000, 
		0x80000000, 
		0xde273031, 
		0x9469122e, 
		0x7f7fffff, 
		0x0c7fffe0, 
		0x7f7ffffe, 
		0x7d67f536, 
		0x0c7fffc0, 
		0xc1500000, 
		0xe17d57c8, 
		0x41700000, 
		0x41a80000, 
		0x56e7d665, 
		0x80000000, 
		0x42700000, 
		0x41900000, 
		0x80000000, 
		0x0c7f0000, 
		0x02f1dfbe, 
		0x00008000, 
		0xc2600000, 
		0x6274cd3a, 
		0x41b00000, 
		0xb0e903d3, 
		0xbec0d257, 
		0x18b8498d, 
		0xffbfffff, 
		0x9f62cef9, 
		0xff7fffff, 
		0x093526da, 
		0x847bb5e4, 
		0x00ffffff, 
		0xc1e80000, 
		0x9aaf8fb7, 
		0x00080000, 
		0x0e07ffff, 
		0x80800003, 
		0xb22f3a7f, 
		0xace758ef, 
		0x40c00000, 
		0x423c0000, 
		0x0e0000ff, 
		0x80002000, 
		0x007fffff, 
		0x0c7ff800, 
		0x8fb721e3, 
		0xc2340000, 
		0xff800000, 
		0x7f800000, 
		0xc1e80000, 
		0x80000100, 
		0x0e000007, 
		0x83982fe1, 
		0x00004000, 
		0x80002000, 
		0x807ffffe, 
		0x80000001, 
		0x007fffff, 
		0x80000000, 
		0x00000000, 
		0x420c0000, 
		0xc24c0000, 
		0x00800002, 
		0xc26c0000, 
		0xb05d7cdb, 
		0x80800002, 
		0x4bf77cad, 
		0x80000020, 
		0xafd19701, 
		0x7f800000, 
		0x80000000, 
		0xcccccccc, 
		0x80000000, 
		0xecc7e8be, 
		0x0e000001, 
		0xffb04e7f, 
		0x6f29de3d, 
		0xa5535775, 
		0x2e846016, 
		0x80011111, 
		0x80601f62, 
		0xce6c2871, 
		0xcb8c4b40, 
		0xc0400000, 
		0xff7ffffe, 
		0xd56f0a96, 
		0xcb000000, 
		0x42780000, 
		0xf4bf1e94, 
		0x00000008, 
		0x41f00000, 
		0xc1980000, 
		0x41000000, 
		0x80ffffff, 
		0x42580000, 
		0x80000040, 
		0x41600000, 
		0x42080000, 
		0xffc00000, 
		0xfcf0b069, 
		0x0c7fffe0, 
		0x000fde76, 
		0x42540000, 
		0x0e00003f, 
		0x31337ab8, 
		0x9d828ebf, 
		0x42400000, 
		0x00000000, 
		0x7f800000, 
		0x7f800000, 
		0x41c00000, 
		0x0aa3af9e, 
		0x41e80000, 
		0x7f800000, 
		0x0c7ff000, 
		0x41a80000, 
		0xa6dcae0c, 
		0x80000000, 
		0x41200000, 
		0x59ede08d, 
		0x00000000, 
		0x7f87ce3b, 
		0x5ec00b69, 
		0xac4fda2e, 
		0xe19c270d, 
		0x80000001, 
		0xc20c0000, 
		0xa8d8a86b, 
		0x80800003, 
		0x7b718910, 
		0x80000000, 
		0x7fc00001, 
		0x7f800000, 
		0x80000001, 
		0xb3a58bd4, 
		0xc1900000, 
		0x007fffff, 
		0x00000000, 
		0x7f800001, 
		0x00040000, 
		0x42200000, 
		0x42480000, 
		0xff7ffffe, 
		0x42000000, 
		0x0f7ffffe, 
		0x4c76feef, 
		0x11df2f13, 
		0xc1500000, 
		0x1cd24990, 
		0x9ae47abf, 
		0x4b8c4b40, 
		0x55555555, 
		0xd545ad2e, 
		0xc2080000, 
		0x80200000, 
		0xe64fedef, 
		0xeb0d01c9, 
		0x7fb5afcb, 
		0xcb000000, 
		0x80000000, 
		0xb094702b, 
		0x80080000, 
		0xc1700000, 
		0xbe7ba485, 
		0xa4b3535a, 
		0xff800000, 
		0x42640000, 
		0xb52057ae, 
		0xc357d69a, 
		0x0c7f8000, 
		0x8dfb4d0f, 
		0x0c7f8000, 
		0x00011111, 
		0x41a80000, 
		0x87c15e9e, 
		0x80000000, 
		0x615d66dd, 
		0x0c7fff80, 
		0x42240000, 
		0xd02d4961, 
		0x40400000, 
		0xffcd98bb, 
		0xc5b55239, 
		0x00800003, 
		0x80000002, 
		0x426c0000, 
		0xad4e4d04, 
		0xff800000, 
		0xd5a0537d, 
		0x55e2b6a3, 
		0xf7c3732c, 
		0x41b80000, 
		0xff800000, 
		0xff800000, 
		0x42480000, 
		0x40c00000, 
		0x42780000, 
		0x1b8e6d57, 
		0x0e003fff, 
		0x00040000, 
		0x9110716e, 
		0x7f000000, 
		0x80800001, 
		0x80100000, 
		0x0e00007f, 
		0x422c0000, 
		0x00000040, 
		0x99268962, 
		0x80000000, 
		0x00001000, 
		0x00800002, 
		0x00800001, 
		0x9fa23c0f, 
		0x3cafde17, 
		0x801d6cc5, 
		0x0e000fff, 
		0x52421730, 
		0x7555d7c6, 
		0x00000100, 
		0x47ccecc3, 
		0xbf028f5c, 
		0xbf028f5c, 
		0x007fffff, 
		0x3432a68f, 
		0x0c7ffff0, 
		0x30bd4804, 
		0xc0e00000, 
		0x42680000, 
		0xc2200000, 
		0x0f7ffffc, 
		0x0e1fffff, 
		0x803a3586, 
		0x40400000, 
		0x0c7ffffc, 
		0x2647551c, 
		0xc2100000, 
		0x41880000, 
		0xc24c0000, 
		0xf69ed3e9, 
		0xcbcd2a01, 
		0x17fdcfde, 
		0x00000000, 
		0x00000080, 
		0x00000002, 
		0x41a80000, 
		0xff7fffff, 
		0x0e000007, 
		0x41c80000, 
		0x5c889fdc, 
		0xc26c0000, 
		0x00000002, 
		0xe0d62331, 
		0x00000000, 
		0xff89198e, 
		0x606c9a77, 
		0x41a00000, 
		0x0025faf4, 
		0x41100000, 
		0x41e00000, 
		0x7476a405, 
		0xc2700000, 
		0x0e00ffff, 
		0x007fffff, 
		0x80000000, 
		0x7f800000, 
		0x80100000, 
		0xc1c00000, 
		0x42180000, 
		0xc72db03d, 
		0x427c0000, 
		0xa791e00b, 
		0xd0e1df76, 
		0x7a0fc10f, 
		0x7fbfffff, 
		0xbcee9da8, 
		0x7f000000, 
		0xd79fc643, 
		0x0c7fffc0, 
		0xff7ffffe, 
		0x00011111, 
		0x00000000, 
		0x3932dece, 
		0x0c7fff80, 
		0xff800000, 
		0x7f800000, 
		0x7f800000, 
		0x0e7fffff, 
		0x0e00003f, 
		0xc1a00000, 
		0x80800003, 
		0xff800000, 
		0x00ffffff, 
		0x7f800000, 
		0x41800000, 
		0x0e0001ff, 
		0x00f12a67, 
		0x00000001, 
		0x00800002, 
		0x78f7e21f, 
		0x512672f5, 
		0xd3d6c0c4, 
		0x663b2780, 
		0x18effccb, 
		0x80000000, 
		0x41900000, 
		0xc1980000, 
		0x00400000, 
		0xc2080000, 
		0xc1300000, 
		0xff801c53, 
		0x42640000, 
		0xc21547e8, 
		0x8f7ffffd, 
		0xc2200000, 
		0x41900000, 
		0x0e01ffff, 
		0x887853eb, 
		0xe3890609, 
		0x0c7fe000, 
		0x80800003, 
		0xa37b5e11, 
		0x00000000, 
		0x00800002, 
		0xffd809fb, 
		0x4b8c4b40, 
		0x80030d69, 
		0x0c780000, 
		0xc2580000, 
		0xab964b5f, 
		0x00000000, 
		0xc1f00000, 
		0x00000400, 
		0xc2780000, 
		0x8f7ffffe, 
		0x42780000, 
		0x42480000, 
		0x00010000, 
		0x003227a9, 
		0x00ffffff, 
		0xe276a44d, 
		0x00800002, 
		0x2be77862, 
		0x00000100, 
		0xc398a167, 
		0xffc00001, 
		0x7f7ffffe, 
		0x00000000, 
		0xc27c0000, 
		0x00800000, 
		0x695c0725, 
		0xc2000000, 
		0x1638b74a, 
		0x422c0000, 
		0x40e00000, 
		0x80800003, 
		0x42100000, 
		0xc1d00000, 
		0x42640000, 
		0x42cbffdb, 
		0x42140000, 
		0xc25c0000, 
		0x40800000, 
		0x41300000, 
		0xf3721ca9, 
		0x7fbfffff, 
		0x80004000, 
		0x425c0000, 
		0xc1000000, 
		0x508ebe15, 
		0xc1f80000, 
		0xbf800001, 
		0x80800001, 
		0x0c7ffff0, 
		0xb7b55224, 
		0x80001000, 
		0x0e000003, 
		0xc2780000, 
		0xe29c713f, 
		0xff7ffffe, 
		0x0e000003, 
		0x00011111, 
		0x00000000, 
		0x41c80000, 
		0x9069abdc, 
		0x6e033288, 
		0xc2540000, 
		0xfa3972d1, 
		0x8f7ffffe, 
		0x3bfe092a, 
		0x00000002, 
		0x800270ef, 
		0x40a00000, 
		0x3d25265b, 
		0x0e00007f, 
		0xdaed0375, 
		0x0e01ffff, 
		0x0e00007f, 
		0x80800001, 
		0x00000000, 
		0x562955ce, 
		0xc2340000, 
		0x2466afe8, 
		0xff9dcf12, 
		0x00000000, 
		0x822f12a3, 
		0x43a42aa7, 
		0xc2540000, 
		0x80000001, 
		0x00040000, 
		0xc2580000, 
		0xc0800000, 
		0x42540000, 
		0x4002431f, 
		0x59d14096, 
		0x80000000, 
		0x0e0fffff, 
		0x7ffbb91d, 
		0x0f7ffffc, 
		0x41e00000, 
		0x0c7fff80, 
		0xf29f2b68, 
		0x00000000, 
		0x00000020, 
		0x41200000, 
		0x80ffffff, 
		0xcac1a127, 
		0x0c7fc000, 
		0x41000000, 
		0xff800000, 
		0x80200000, 
		0x806e8094, 
		0x00004000, 
		0x7f800000, 
		0x0c7fc000, 
		0x80000002, 
		0xff800000, 
		0x6ede1d95, 
		0x00000000, 
		0x572921ed, 
		0xffffffff, 
		0x4b000000, 
		0xc1b80000, 
		0xa81c5218, 
		0x80008000, 
		0x80000040, 
		0xc23c0000, 
		0x00040000, 
		0xc1800000, 
		0xf835363f, 
		0x00000020, 
		0x42000000, 
		0x00000000, 
		0x73ba933a, 
		0x35ce928b, 
		0xc840fccb, 
		0x80080000, 
		0xc2340000, 
		0xff800000, 
		0x00000000, 
		0x80020000, 
		0x2f16ec77, 
		0x545a4357, 
		0x800f95b7, 
		0x42300000, 
		0x42480000, 
		0xbb573f6d, 
		0x0c7fc000, 
		0x80000000, 
		0x6504ce06, 
		0x00000000, 
		0xc1a80000, 
		0x7f893fca, 
		0xc1880000, 
		0x0e7fffff, 
		0xc1100000, 
		0xc1800000, 
		0xbf800000, 
		0x41700000, 
		0x8e6a63fa, 
		0xa05858bf, 
		0x80004000, 
		0x033ed79c, 
		0x9094a389, 
		0x80000080, 
		0xff83e08b, 
		0x80000000, 
		0xc0c00000, 
		0xff7fffff, 
		0x0e003fff, 
		0x84ceb8fe, 
		0x00467784, 
		0x0c7f0000, 
		0x6d184bdd, 
		0xa89f3130, 
		0xa809db25, 
		0x421c0000, 
		0x0e0000ff, 
		0xff800000, 
		0x13f4cebc, 
		0x80000001, 
		0x0c7ffffc, 
		0x420c0000, 
		0x80800003, 
		0x00020000, 
		0x4b000000, 
		0x41000000, 
		0x778fb3b4, 
		0xc1300000, 
		0x80000200, 
		0x42600000, 
		0xbaf795fa, 
		0x8d8b8259, 
		0x0e40ad8c, 
		0xc2580000, 
		0x8f7ffffd, 
		0x80ffffff, 
		0x80000020, 
		0x9a581a87, 
		0xc1a80000, 
		0x40400000, 
		0xdd59268b, 
		0x54290f86, 
		0x42140000, 
		0x426c0000, 
		0x80000008, 
		0x96cc81e4, 
		0x944d8635, 
		0x46c7f219, 
		0x80000000, 
		0xffc00001, 
		0x4c8db334, 
		0x00000800, 
		0x00000000, 
		0x00000000, 
		0x58fabe6a, 
		0x0e000fff, 
		0x8f7ffffc, 
		0x00000001, 
		0xbf028f5c, 
		0x41e80000, 
		0x7f9b4204, 
		0x1446a566, 
		0x0058a346, 
		0x42300000, 
		0xcfaee40f, 
		0x6dd6378d, 
		0x807fffff, 
		0x27ea3fe9, 
		0xc1f80000, 
		0x7f7fffff, 
		0x4e93648d, 
		0xff7ffffe, 
		0xb5855492, 
		0xc2080000, 
		0xc7879538, 
		0x40000000, 
		0x42080000, 
		0x00000000, 
		0xdd52b946, 
		0x00000000, 
		0x40e00000, 
		0x0c400000, 
		0x0c7ffff0, 
		0x0e001fff, 
		0x7fc00001, 
		0xe37d2bba, 
		0x7f800000, 
		0x007fffff, 
		0x7f7ffffe, 
		0x79575331, 
		0x00011111, 
		0x0e1fffff, 
		0x0e0fffff, 
		0x41d80000, 
		0xc1700000, 
		0x64a9b021, 
		0x803650e9, 
		0xc2480000, 
		0xfb143887, 
		0xc1c00000, 
		0x80011111, 
		0x42280000, 
		0x80000000, 
		0xc1800000, 
		0x41600000, 
		0x80000002, 
		0xc7c7d80c, 
		0xc2fc1d1f, 
		0x00800002, 
		0x8b2123e8, 
		0x0c7ffffc, 
		0x7f800000, 
		0x80800000, 
		0x42680000, 
		0x42040000, 
		0xc1f00000, 
		0x0c7f8000, 
		0x00000400, 
		0xeac3a4e2, 
		0x001b8461, 
		0xc8a1112a, 
		0x00010000, 
		0x80000000, 
		0x0c7ffffc, 
		0x59c5d9f8, 
		0x0e007fff, 
		0x66ac4b1b, 
		0xaaaaaaaa, 
		0x00800000, 
		0x7fbfffff, 
		0x00800000, 
		0x8161a25f, 
		0xc1200000, 
		0x0e000001, 
		0xe6958f32, 
		0xc1000000, 
		0x85a7afae, 
		0xff800000, 
		0x0d000ff0, 
		0x0e000001, 
		0x80000200, 
		0x86b91371, 
		0x00000000, 
		0xc288d561, 
		0x00800002, 
		0xc2780000, 
		0x80080000, 
		0xdc48030a, 
		0x40e00000, 
		0xc1980000, 
		0x0c7f8000, 
		0x40c00000, 
		0xff8df66c, 
		0x41880000, 
		0x94df6bf0, 
		0x00000000, 
		0x41b80000, 
		0x80020000, 
		0x42c0c0ff, 
		0x7f800000, 
		0x0e0007ff, 
		0xc1100000, 
		0x80002000, 
		0x00624466, 
		0x760baf0d, 
		0x7f800000, 
		0xe93e134b, 
		0x7fa88cad, 
		0x00000001, 
		0xb7fe662b, 
		0x00020000, 
		0x0e00000f, 
		0x5ea40e0c, 
		0x80000000, 
		0xcdeed52b, 
		0xc1f00000, 
		0xc1200000, 
		0xc1100000, 
		0x00000000, 
		0x73cba3b0, 
		0x41500000, 
		0x0c7ffff0, 
		0x40000000, 
		0xbd7adf3c, 
		0xada67912, 
		0x80008000, 
		0x80000000, 
		0x59617753, 
		0xc1e00000, 
		0xc0c00000, 
		0x00000000, 
		0x7fb3b1be, 
		0xffc00000, 
		0x00000000, 
		0x80000000, 
		0x41d00000, 
		0xcccccccc, 
		0x80000000, 
		0xc1d80000, 
		0x5c8713f5, 
		0xff800000, 
		0x00800000, 
		0xc1f00000, 
		0x00000200, 
		0xc1400000, 
		0xc1300000, 
		0x01772476, 
		0x00000020, 
		0xff800000, 
		0x41400000, 
		0x7f800000, 
		0x80000000, 
		0x80000000, 
		0x00000000, 
		0x0c780000, 
		0x0e00001f, 
		0x00ffffff, 
		0x50eeb1b6, 
		0x00800001, 
		0xe6d254e7, 
		0x7f800000, 
		0x7f9dacad, 
		0x7f800001, 
		0x00000008, 
		0x42280000, 
		0x0c780000, 
		0x0c7c0000, 
		0x8030ef8c, 
		0x7f7ffffe, 
		0x0e003fff, 
		0xc0400000, 
		0x2177231b, 
		0xff800000, 
		0x42600000, 
		0xd9647a7b, 
		0x3f800000, 
		0x00010000, 
		0x0e0000ff, 
		0x0e1fffff, 
		0x3bad3c72, 
		0x8933d1f5, 
		0x2a0e27a7, 
		0x425c0000, 
		0x00200000, 
		0xef4eb966, 
		0x7f7fffff, 
		0x0c7fff00, 
		0x0f7ffffe, 
		0x068f3a40, 
		0xa7018dcf, 
		0x7df2575b, 
		0x0c7ffffe, 
		0xc0c00000, 
		0x42480000, 
		0xc27c0000, 
		0x5d8a64d0, 
		0x0e003fff, 
		0x42100000, 
		0x00000000, 
		0x80000000, 
		0x1bf7e6cf, 
		0xe4fde7f3, 
		0x888b5548, 
		0x42180000, 
		0xd743e7b2, 
		0x80040000, 
		0xbc5e1e66, 
		0xdbbbe2a6, 
		0x0e000003, 
		0x2c481931, 
		0x173410f4, 
		0x0c7ffe00, 
		0xb0e386f0, 
		0x80000001, 
		0xf643841b, 
		0x0c7fff00, 
		0x3f800000, 
		0xd3e73a6d, 
		0xff7ffffe, 
		0x3f2c2d55, 
		0x422c0000, 
		0x0e1fffff, 
		0xc2140000, 
		0x80800002, 
		0xcccccccc, 
		0x423c0000, 
		0x0e003fff, 
		0x80800001, 
		0x0c7ff800, 
		0x00800002, 
		0x00000008, 
		0x00000001, 
		0x523209b0, 
		0xff800000, 
		0x00400000, 
		0x8f7fffff, 
		0x41880000, 
		0x0f7ffffe, 
		0xa77f7c36, 
		0x849c70d7, 
		0xc2740000, 
		0xcde98742, 
		0x00000020, 
		0x00128c01, 
		0xbf800001, 
		0x806bcf03, 
		0x41900000, 
		0x0f7ffffc, 
		0x00800000, 
		0xa2e11b12, 
		0x00400000, 
		0x0d000ff0, 
		0xffbfffff, 
		0xa251629f, 
		0x00000000, 
		0x80200000, 
		0xc2580000, 
		0xc1200000, 
		0x8f7ffffc, 
		0xe04ca8c4, 
		0xd194cd06, 
		0x7f800000, 
		0x4b8c4b40, 
		0x80040000, 
		0x41200000, 
		0xc2080000, 
		0x80002000, 
		0xf734ab6d, 
		0x7f7fffff, 
		0xc0c00000, 
		0xc1200000, 
		0xc2200000, 
		0x00800000, 
		0x80000020, 
		0x0c7ffc00, 
		0x00000001, 
		0x41c00000, 
		0xff872473, 
		0xc1f80000, 
		0x00800000, 
		0x00000001, 
		0x41e80000, 
		0x80000000, 
		0x80002000, 
		0x0e00007f, 
		0x7fbfffff, 
		0x50094ac0, 
		0xefd2eab9, 
		0x80040000, 
		0x7fa0a6c3, 
		0x42380000, 
		0xc2000000, 
		0x0b8231e3, 
		0x6d57841e, 
		0x786910d8, 
		0x15021dbe, 
		0x41c00000, 
		0x0d00fff0, 
		0xc2380000, 
		0x740ad7f0, 
		0xc2380000, 
		0xc1f00000, 
		0xc2240000, 
		0x1287299d, 
		0x42040000, 
		0x7451e62a, 
		0x7f800000, 
		0x0f7fffff, 
		0xc1f80000, 
		0xcb43d7e3, 
		0x80000008, 
		0x80000000, 
		0x8576eb32, 
		0x425c0000, 
		0x80000100, 
		0xc1e80000, 
		0x19eb217e, 
		0x94901471, 
		0x80000800, 
		0xc1600000, 
		0x006d7061, 
		0x0e003fff, 
		0x0c7b8ef8, 
		0x3631bb0e, 
		0x41200000, 
		0x3f028f5c, 
		0x7f9f83bd, 
		0x0e003fff, 
		0x0e1fffff, 
		0x80080000, 
		0x426c0000, 
		0xbf028f5c, 
		0xc4c93fc5, 
		0x94214921, 
		0x7fbc4b6d, 
		0xc2000000, 
		0xc2500000, 
		0xc21c0000, 
		0xff7fffff, 
		0x42500000, 
		0x0bee04a8, 
		0x7a63e626, 
		0x7427e630, 
		0xc1e80000, 
		0x80000000, 
		0x80000000, 
		0x60fc1fc1, 
		0x0c7fffff, 
		0x41d80000, 
		0x00000000, 
		0x80000000, 
		0xff800000, 
		0x0e00001f, 
		0x421c0000, 
		0x00000002, 
		0x33333333, 
		0xfe0a2871, 
		0x0e07ffff, 
		0x42280000, 
		0x70d32845, 
		0x9ffcbc09, 
		0x42700000, 
		0x8142d05c, 
		0xc543747c, 
		0x41700000, 
		0x80000000, 
		0x82009410, 
		0x00000002, 
		0x2fd6ec30, 
		0x00000010, 
		0x4b000000, 
		0x2c1c005e, 
		0xfb94b32d, 
		0x80000000, 
		0x4d86ba4b, 
		0x42700000, 
		0xdd04b3a2, 
		0x5c02d0b6, 
		0x80000000, 
		0x15e3ecb3, 
		0x80800001, 
		0x6dadb4d4, 
		0x00004000, 
		0x41d00000, 
		0x00800001, 
		0x00800001, 
		0x80008000, 
		0x7c8fb5e9, 
		0xc1a80000, 
		0x40400000, 
		0x00200000, 
		0x07c1fc6a, 
		0x3f028f5c, 
		0x0e00000f, 
		0x41c80000, 
		0xc5aa107e, 
		0xa560a458, 
		0xf3d3f404, 
		0x80002000, 
		0x68ef0907, 
		0x81b6dfdf, 
		0xc1300000, 
		0xc1200000, 
		0xe7569fb7, 
		0xff800000, 
		0xff800000, 
		0x0c400000, 
		0xc2300000, 
		0x0e00ffff, 
		0x1c7dfded, 
		0x00000000, 
		0x0047d201, 
		0x80800002, 
		0x42380000, 
		0x40400000, 
		0x7ec3cacd, 
		0x20084055, 
		0x0e03ffff, 
		0x80800001, 
		0x41800000, 
		0x7f7fffff, 
		0x41800000, 
		0x42300000, 
		0x7f800000, 
		0x0ba44918, 
		0x42200000, 
		0x00002000, 
		0x00000400, 
		0x59c46746, 
		0x80000008, 
		0x80000000, 
		0x00000000, 
		0x0e007fff, 
		0x00800001, 
		0x21cac85d, 
		0xc1980000, 
		0xc2240000, 
		0x41000000, 
		0x00040000, 
		0xc2580000, 
		0xffc00001, 
		0x7f800000, 
		0xbbfc4e08, 
		0xc2040000, 
		0x00000000, 
		0x010013f3, 
		0x26e87908, 
		0x46e315c3, 
		0x09641ee2, 
		0x00000000, 
		0x52892217, 
		0x42380000, 
		0x2668346c, 
		0xd922de56, 
		0x80000004, 
		0xc24c0000, 
		0xc2680000, 
		0xc0e00000, 
		0x0e0003ff, 
		0x41c00000, 
		0x281804cb, 
		0x80011111, 
		0x2222e8e9, 
		0x536fcc16, 
		0xc2240000, 
		0x42700000, 
		0x00000002, 
		0xe5e56902, 
		0xecb6b29c, 
		0x80020000, 
		0x6e67498b, 
		0xff800000, 
		0x422c0000, 
		0xaf382e68, 
		0x55555555, 
		0x80000001, 
		0xb7b030e4, 
		0x88c4bb9e, 
		0x80010000, 
		0x424c0000, 
		0x795b8469, 
		0x42400000, 
		0x4e283d9c, 
		0xb92f14c6, 
		0xc24c0000, 
		0x00800003, 
		0x00800000, 
		0xffc00000, 
		0x0c7fffc0, 
		0x7f800000, 
		0x24c69d5a, 
		0xff800000, 
		0x80000020, 
		0x41c80000, 
		0x42700000, 
		0xffc00000, 
		0x80008000, 
		0xf1c9bc3f, 
		0x3f800000, 
		0x422c0000, 
		0xcd0d56ce, 
		0xb82d5b34, 
		0x0e000003, 
		0x42480000, 
		0x007fffff, 
		0x307c142a, 
		0x80000080, 
		0xff800000, 
		0x80040000, 
		0xd491639b, 
		0x29e18e06, 
		0x7fbfffff, 
		0x100e56c2, 
		0xab8d8dab, 
		0xce925275, 
		0x7f800000, 
		0x408e3af9, 
		0x40400000, 
		0x0e3fffff, 
		0x3c74a4e3, 
		0xc1600000, 
		0xff000000, 
		0x8079dcbb, 
		0x42500000, 
		0x80020000, 
		0xff800000, 
		0x42000000, 
		0x00008000, 
		0x42180000, 
		0x1b372f71, 
		0x42700000, 
		0xff800000, 
		0x00000000, 
		0x80008000, 
		0xffb4628f, 
		0x00002000, 
		0xd0922d97, 
		0x0c7fff00, 
		0x80800001, 
		0x80008000, 
		0xcccccccc, 
		0xff800000, 
		0x42500000, 
		0xffb19d01, 
		0x80001000  
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
		0x00000307, 
		0x00000279, 
		0x00000041, 
		0x00000037, 
		0x0000032b, 
		0x0000008b, 
		0x00000381, 
		0x00000211, 
		0x00000147, 
		0x0000024f, 
		0x0000035f, 
		0x00000155, 
		0x00000289, 
		0x00000339, 
		0x000003c9, 
		0x000003b1, 
		0x000002db, 
		0x00000127, 
		0x000002eb, 
		0x0000008b, 
		0x00000389, 
		0x000003d5, 
		0x000003d7, 
		0x0000032b, 
		0x0000010d, 
		0x00000321, 
		0x0000023b, 
		0x000000e1, 
		0x0000000f, 
		0x000001df, 
		0x00000113, 
		0x000001ab, 
		0x000002c3, 
		0x0000013b, 
		0x00000151, 
		0x00000167, 
		0x00000075, 
		0x0000024b, 
		0x000003bd, 
		0x00000267, 
		0x000002bf, 
		0x0000025f, 
		0x00000269, 
		0x0000000f, 
		0x000000c3, 
		0x00000241, 
		0x00000163, 
		0x00000047, 
		0x00000049, 
		0x000000af, 
		0x000002e3, 
		0x0000007f, 
		0x000000d5, 
		0x00000055, 
		0x0000024d, 
		0x000002a9, 
		0x00000381, 
		0x0000024f, 
		0x00000395, 
		0x0000025b, 
		0x0000033d, 
		0x00000157, 
		0x0000035b, 
		0x000001cd, 
		0x00000195, 
		0x000001c1, 
		0x000001ed, 
		0x000003f1, 
		0x00000381, 
		0x00000309, 
		0x000000b5, 
		0x000003bd, 
		0x0000021d, 
		0x00000085, 
		0x00000217, 
		0x0000033d, 
		0x000002bd, 
		0x00000297, 
		0x000002af, 
		0x00000337, 
		0x000002c1, 
		0x00000395, 
		0x000002e3, 
		0x000003db, 
		0x000000bd, 
		0x0000007f, 
		0x000000e9, 
		0x000003f7, 
		0x000001eb, 
		0x0000003f, 
		0x0000006f, 
		0x000001f9, 
		0x000000a3, 
		0x000002e9, 
		0x00000045, 
		0x0000004d, 
		0x00000051, 
		0x000000ed, 
		0x0000007b, 
		0x000002b3, 
		0x0000007d, 
		0x00000399, 
		0x0000036d, 
		0x000000bb, 
		0x000003b3, 
		0x000001a9, 
		0x00000249, 
		0x0000039d, 
		0x000003d9, 
		0x00000345, 
		0x000003e7, 
		0x00000013, 
		0x000002b3, 
		0x0000007b, 
		0x0000015b, 
		0x00000191, 
		0x000003f7, 
		0x00000065, 
		0x000000c5, 
		0x000002af, 
		0x000003ad, 
		0x000002b5, 
		0x000003d5, 
		0x000001bf, 
		0x000003af, 
		0x00000207, 
		0x0000004b, 
		0x000002a5, 
		0x00000001, 
		0x0000017b, 
		0x000001ad, 
		0x000000b9, 
		0x000002eb, 
		0x000000c9, 
		0x000002b9, 
		0x000000e5, 
		0x000003a3, 
		0x000001db, 
		0x00000283, 
		0x00000397, 
		0x00000215, 
		0x000002d9, 
		0x0000019f, 
		0x00000285, 
		0x00000299, 
		0x000001b9, 
		0x0000011d, 
		0x000001df, 
		0x00000067, 
		0x000001a9, 
		0x00000077, 
		0x00000335, 
		0x000003e7, 
		0x000003f7, 
		0x00000129, 
		0x000002e5, 
		0x000002df, 
		0x0000008b, 
		0x000002d1, 
		0x0000039b, 
		0x000001db, 
		0x00000105, 
		0x00000267, 
		0x00000079, 
		0x00000303, 
		0x00000011, 
		0x000003d1, 
		0x00000393, 
		0x0000010b, 
		0x000002d3, 
		0x0000007d, 
		0x000003a1, 
		0x00000139, 
		0x00000341, 
		0x000001bd, 
		0x00000309, 
		0x00000265, 
		0x00000119, 
		0x0000034f, 
		0x000002f1, 
		0x0000027f, 
		0x000001e7, 
		0x0000015f, 
		0x000000b5, 
		0x00000209, 
		0x000001c9, 
		0x000000d3, 
		0x00000129, 
		0x00000297, 
		0x0000039b, 
		0x0000023b, 
		0x000002e3, 
		0x000001d1, 
		0x00000031, 
		0x00000163, 
		0x00000211, 
		0x000001bd, 
		0x00000357, 
		0x00000301, 
		0x00000311, 
		0x000002a5, 
		0x000002ad, 
		0x00000337, 
		0x0000039b, 
		0x0000018b, 
		0x00000267, 
		0x000000bb, 
		0x000000a3, 
		0x0000033b, 
		0x000001d1, 
		0x00000237, 
		0x0000001b, 
		0x000000db, 
		0x00000003, 
		0x0000034b, 
		0x00000347, 
		0x000000c3, 
		0x00000263, 
		0x00000179, 
		0x000002e7, 
		0x0000001f, 
		0x000002cb, 
		0x000002e1, 
		0x0000023d, 
		0x00000285, 
		0x0000021d, 
		0x0000019b, 
		0x00000015, 
		0x000002bd, 
		0x000001df, 
		0x00000393, 
		0x0000007d, 
		0x00000087, 
		0x000003f5, 
		0x0000005b, 
		0x0000014f, 
		0x00000113, 
		0x000002f7, 
		0x000001ad, 
		0x000000a9, 
		0x00000229, 
		0x00000033, 
		0x00000387, 
		0x00000219, 
		0x0000009b, 
		0x0000011b, 
		0x000000f3, 
		0x0000019b, 
		0x00000037, 
		0x0000016b, 
		0x0000010f, 
		0x0000033d, 
		0x0000016f, 
		0x00000031, 
		0x000001c7, 
		0x00000399  
};

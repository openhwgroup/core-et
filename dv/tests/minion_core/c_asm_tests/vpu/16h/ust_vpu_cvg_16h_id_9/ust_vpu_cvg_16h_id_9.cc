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
			"li x5, 312\n"
			"faddi.pi f2, f0, 24\n"
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
			"csrwi mhpmevent3, 8\n"   
			"csrwi mhpmevent4, 14\n"   
			"csrwi mhpmevent5, 4\n"   
			"csrwi mhpmevent6, 17\n"   
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
			"flw.ps f18,  96 (x5)\n"
			"flw.ps f13,  768 (x5)\n"
			"flw.ps f11,  640 (x5)\n"
			"flw.ps f3,  672 (x5)\n"
			"flw.ps f19,  224 (x5)\n"
			"flw.ps f16,  0 (x5)\n"
			"flw.ps f9,  128 (x5)\n"
			"flw.ps f22,  64 (x5)\n"
			"flw.ps f21,  704 (x5)\n"
			"flw.ps f8,  192 (x5)\n"
			"flw.ps f6,  448 (x5)\n"
			"flw.ps f31,  160 (x5)\n"
			"flw.ps f7,  256 (x5)\n"
			"flw.ps f0,  960 (x5)\n"
			"flw.ps f4,  864 (x5)\n"
			"flw.ps f10,  928 (x5)\n"
			"flw.ps f1,  608 (x5)\n"
			"flw.ps f12,  512 (x5)\n"
			"flw.ps f24,  416 (x5)\n"
			"flw.ps f23,  896 (x5)\n"
			"flw.ps f29,  384 (x5)\n"
			"flw.ps f14,  576 (x5)\n"
			"flw.ps f27,  736 (x5)\n"
			"flw.ps f5,  32 (x5)\n"
			"flw.ps f2,  800 (x5)\n"
			"flw.ps f30,  288 (x5)\n"
			"flw.ps f20,  832 (x5)\n"
			"flw.ps f15,  320 (x5)\n"
			"flw.ps f25,  544 (x5)\n"
			"flw.ps f26,  992 (x5)\n"
			"flw.ps f28,  480 (x5)\n"
			"flw.ps f17,  352 (x5)\n"
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
			"li x0, 0x3e8\n" 
			"slti x0, x0, 0x7f2\n"
			"la x31, check_sum_hid_0\n"
			"li x5, 0x7\n" 
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
			"mova.x.m x5\n" 
			"la x28, rand_int_32\n"
			"flw.ps f31, 192(x28)\n"
			"fcvt.ps.pwu f22, f31, rtz\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f12, 736(x11)\n"
			"flw.ps f21, 736(x11)\n"
			"fmax.ps f8, f12, f21\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f9, 768(x25)\n"
			"fcvt.pwu.ps f13, f9, rmm\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f1, 64(x5)\n"
			"flw.ps f15, 800(x5)\n"
			"fsgnjx.s f15, f1, f15\n" 
			"la x23, rand_int_32\n"
			"flw.ps f21, 352(x23)\n"
			"flw.ps f8, 448(x23)\n"
			"fmax.pi f1, f21, f8\n" 
			"li x5, 0x80ad\n"
			"csrw tensor_mask, x5\n"	
			"la x12, rand_int_32\n"
			"flw.ps f20, 800(x12)\n"
			"flw.ps f22, 384(x12)\n"
			"fmul.pi f28, f20, f22\n" 
			"la x11, rand_int_32\n"
			"flw.ps f19, 960(x11)\n"
			"flw.ps f28, 288(x11)\n"
			"fxor.pi f8, f19, f28\n" 
			"la x23, rand_int_32\n"
			"flw.ps f31, 0(x23)\n"
			"flw.ps f7, 160(x23)\n"
			"fmul.pi f8, f31, f7\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_0:\n"
			"mova.m.x x25\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x11, rand_ieee754_32\n"
			"flw.ps f8, 672(x11)\n"
			"flw.ps f7, 576(x11)\n"
			"fdiv.ps f20, f8, f7, rup\n" 
			"la x12, rand_int_32\n"
			"flw.ps f1, 640(x12)\n"
			"flw.ps f21, 736(x12)\n"
			"fand.pi f14, f1, f21\n" 
			"la x23, rand_ieee754_16\n"
			"flw.ps f6, 32(x23)\n"
			"fcvt.ps.f16 f30, f6\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_0:\n"
			"fsetm.pi m6, f19\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x5, rand_ieee754_32\n"
			"flw.ps f9, 672(x5)\n"
			"flw.ps f13, 128(x5)\n"
			"flw.ps f20, 128(x5)\n"
			"fcmov.ps f13, f9, f13, f20\n" 
			"la x5, rand_int_32\n"
			"flw.ps f28, 864(x5)\n"
			"flw.ps f9, 864(x5)\n"
			"fsrl.pi f1, f28, f9\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 0x96\n"
			"csrw tensor_mask, x5\n"	
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ID_179381_HID_0:\n"
			"li x5, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x3 << TENSOR_FMA16_B_NUM_COLS) | (0x5 << TENSOR_FMA16_A_NUM_ROWS) | (0x7 << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0xe2 << TENSOR_FMA16_SCP_LOC_B) | (0x13 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ID_637880_HID_0:\n"
			"li x5, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x3 << TENSOR_IMA_B_NUM_COLS) | (0x2 << TENSOR_IMA_A_NUM_ROWS) | (0xa << TENSOR_IMA_A_NUM_COLS) | (0xb << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x38 << TENSOR_IMA_SCP_LOC_B) | (0x80 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x0 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x6, ((0xa << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0x2 << TENSOR_QUANT_QUANT_ROW) | (0x39 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x0 << TENSOR_QUANT_TRANSF1) | (0x1 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 7\n"
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_0\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_0_RDX_ID_0_SHIRE_0_HID_0:\n"
			"li x28, 0x3a0000000353000b\n"
			"csrw tensor_reduce, x28\n" 
			"csrwi tensor_wait, 9\n" 
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ID_824491_HID_0:\n"
			"li x5, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x1 << TENSOR_FMA16_B_NUM_COLS) | (0x0 << TENSOR_FMA16_A_NUM_ROWS) | (0x3 << TENSOR_FMA16_A_NUM_COLS) | (0x6 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0xae << TENSOR_FMA16_SCP_LOC_B) | (0x1 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x6, ((0xc << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0x0 << TENSOR_QUANT_QUANT_ROW) | (0x1f << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x2 << TENSOR_QUANT_TRANSF4) | (0x7 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
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
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ITER_0_HID_0:\n"
			"la x31, self_check_16\n"
			"li x5, 0x6\n"
			"add x5, x5, x31\n"
			"li x6, 0x10000000000005\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x1 << TENSOR_FMA16_B_NUM_COLS) | (0x6 << TENSOR_FMA16_A_NUM_ROWS) | (0x5 << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x1 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1d << TENSOR_FMA16_SCP_LOC_B) | (0x0 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x5 << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0x6 << TENSOR_QUANT_QUANT_ROW) | (0x5 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x0 << TENSOR_QUANT_TRANSF1) | (0x2 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0xc\n"
			"bne x5, x31, LBL_FAIL_HID_0\n"
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 10\n"
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ITER_1_HID_0:\n"
			"la x31, self_check_16\n"
			"li x5, 0x4000000000000a\n"
			"add x5, x5, x31\n"
			"li x6, 0x3c000000000000f\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x2 << TENSOR_FMA16_B_NUM_COLS) | (0xa << TENSOR_FMA16_A_NUM_ROWS) | (0xf << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1e << TENSOR_FMA16_SCP_LOC_B) | (0x2 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0x20\n"
			"bne x5, x31, LBL_FAIL_HID_0\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_0\n"
			"csrw tensor_error, x0\n"
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_0\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_1_FP_TRANS_ID_2_HID_0:\n"
			"la x11, rand_ieee754_32\n"
			"flw.ps f30, 352(x11)\n"
			"fexp.ps f14, f30\n" 
			"la x11, rand_int_32\n"
			"flw.ps f1, 416(x11)\n"
			"flw.ps f12, 128(x11)\n"
			"fdivu.pi f30, f1, f12\n" 
			"la x22, rand_int_32\n"
			"flw.ps f12, 960(x22)\n"
			"fsat8.pi f1, f12\n" 
			"la x25, rand_int_32\n"
			"flw.ps f12, 768(x25)\n"
			"fcvt.ps.pw f6, f12, rup\n" 
			"LBL_SEQID_1_M0_WRITE_ID_3_HID_0:\n"
			"maskxor m3, m4, m6\n" 
			"li x5, CREDINC0\n"
			"ld x5, 0(x5)\n"
			"maskpopc x13, m6\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f22, 800(x22)\n"
			"fcvt.pwu.ps f31, f22, rne\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f21, 960(x12)\n"
			"fcvt.f16.ps f28, f21\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f28, 64(x19)\n"
			"fcvt.pw.ps f30, f28, rdn\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f7, 320(x13)\n"
			"flw.ps f1, 544(x13)\n"
			"fsgnjn.s f21, f7, f1\n" 
			"LBL_SEQID_1_FP_TRANS_ID_4_HID_0:\n"
			"la x19, rand_ieee754_32\n"
			"flw.ps f13, 576(x19)\n"
			"fexp.ps f30, f13\n" 
			"LBL_SEQID_1_M0_WRITE_ID_5_HID_0:\n"
			"maskxor m6, m4, m3\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x12, rand_ieee754_32\n"
			"flw.ps f31, 928(x12)\n"
			"flw.ps f15, 256(x12)\n"
			"fsgnjn.ps f1, f31, f15\n" 
			"LBL_SEQID_1_M0_WRITE_ID_6_HID_0:\n"
			"maskor m0, m6, m4\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_1_FP_TRANS_ID_7_HID_0:\n"
			"la x12, rand_ieee754_32\n"
			"flw.ps f22, 544(x12)\n"
			"flog.ps f31, f22\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f22, 704(x13)\n"
			"flw.ps f14, 736(x13)\n"
			"flt.ps f6, f22, f14\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_1_RDX_ID_1_SHIRE_0_HID_0:\n"
			"li x5, 0x300000000738000b\n"
			"csrw tensor_reduce, x5\n" 
			"csrwi tensor_wait, 9\n" 
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=0); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"li x5, 0x6d3d3745f787b75\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0xb89aa7c0993e4dbf\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ITER_0_HID_0:\n"
			"li x5, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x0 << TENSOR_IMA_B_NUM_COLS) | (0xd << TENSOR_IMA_A_NUM_ROWS) | (0x1 << TENSOR_IMA_A_NUM_COLS) | (0x7 << TENSOR_IMA_OFFSET) | (0x0 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x49 << TENSOR_IMA_SCP_LOC_B) | (0xab << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x0 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ITER_1_HID_0:\n"
			"li x5, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x3 << TENSOR_FMA16_B_NUM_COLS) | (0x5 << TENSOR_FMA16_A_NUM_ROWS) | (0xf << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x67 << TENSOR_FMA16_SCP_LOC_B) | (0x2d << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_0\n"
			"csrw tensor_error, x0\n"
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_0\n"
			"addi x30, x30, 1\n"
			"la x20, rand_int_32\n"
			"flw.ps f31, 704(x20)\n"
			"flw.ps f22, 832(x20)\n"
			"fsll.pi f13, f31, f22\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f15, 320(x13)\n"
			"flw.ps f22, 352(x13)\n"
			"fmul.s f9, f15, f22, rmm\n" 
			"LBL_SEQID_2_M0_WRITE_ID_8_HID_0:\n"
			"masknot m3, m4\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x12, rand_int_32\n"
			"flw.ps f30, 640(x12)\n"
			"fslli.pi f20, f30, 0xe\n" 
			"li x5, 0x2487\n"
			"csrw tensor_mask, x5\n"	
			"la x5, rand_int_32\n"
			"flw.ps f28, 608(x5)\n"
			"fsrai.pi f12, f28, 0x7\n" 
			"LBL_SEQID_2_M0_WRITE_ID_9_HID_0:\n"
			"maskand m4, m0, m3\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x20, rand_int_32\n"
			"flw.ps f31, 0(x20)\n"
			"feq.pi f6, f31, f31\n" 
			"la x25, rand_int_32\n"
			"flw.ps f15, 224(x25)\n"
			"fandi.pi f21, f15, 0x5f\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f21, 128(x19)\n"
			"flw.ps f20, 608(x19)\n"
			"fmul.s f22, f21, f20, rmm\n" 
			"LBL_SEQID_2_FP_TRANS_ID_10_HID_0:\n"
			"la x23, rand_ieee754_32\n"
			"flw.ps f13, 832(x23)\n"
			"frcp.ps f1, f13\n" 
			"la x19, rand_int_32\n"
			"flw.ps f15, 544(x19)\n"
			"fnot.pi f12, f15\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f12, 192(x22)\n"
			"fswizz.ps f28, f12, 0x13\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f21, 608(x14)\n"
			"fswizz.ps f22, f21, 0x13\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f19, 704(x5)\n"
			"fswizz.ps f8, f19, 0x1e\n" 
			"la x14, rand_int_32\n"
			"flw.ps f21, 960(x14)\n"
			"fpackreph.pi f21, f21\n" 
			"LBL_SEQID_2_M0_WRITE_ID_11_HID_0:\n"
			"maskor m0, m6, m4\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_2_XMA_QNT_STR_SEQ_ITER_0_HID_0:\n"
			"li x5, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x3 << TENSOR_FMA16_B_NUM_COLS) | (0x3 << TENSOR_FMA16_A_NUM_ROWS) | (0x7 << TENSOR_FMA16_A_NUM_COLS) | (0xd << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0xa0 << TENSOR_FMA16_SCP_LOC_B) | (0xd3 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"LBL_SEQID_2_XMA_QNT_STR_SEQ_ITER_1_HID_0:\n"
			"li x5, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x2 << TENSOR_IMA_B_NUM_COLS) | (0x1 << TENSOR_IMA_A_NUM_ROWS) | (0xd << TENSOR_IMA_A_NUM_COLS) | (0xb << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0xff << TENSOR_IMA_SCP_LOC_B) | (0xc9 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
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
			"LBL_SEQID_3_M0_WRITE_ID_12_HID_0:\n"
			"fltm.ps m6, f6, f20\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x20, rand_ieee754_32\n"
			"flw.ps f31, 864(x20)\n"
			"flw.ps f20, 224(x20)\n"
			"feq.ps f1, f31, f20\n" 
			"la x22, rand_int_32\n"
			"flw.ps f28, 992(x22)\n"
			"flw.ps f9, 736(x22)\n"
			"fsra.pi f20, f28, f9\n" 
			"mova.x.m x11\n" 
			"la x22, rand_int_32\n"
			"flw.ps f9, 0(x22)\n"
			"flw.ps f20, 800(x22)\n"
			"fsub.pi f12, f9, f20\n" 
			"LBL_SEQID_3_M0_WRITE_ID_13_HID_0:\n"
			"fsetm.pi m0, f15\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x20, rand_ieee754_32\n"
			"flw.ps f19, 672(x20)\n"
			"ffrc.ps f20, f19\n" 
			"la x22, rand_int_32\n"
			"flw.ps f1, 512(x22)\n"
			"fsrli.pi f30, f1, 0x3\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f19, 192(x23)\n"
			"fclass.s x13, f19\n" 
			"LBL_SEQID_3_FP_TRANS_ID_14_HID_0:\n"
			"la x14, rand_ieee754_32\n"
			"flw.ps f15, 224(x14)\n"
			"frcp.ps f13, f15\n" 
			"la x12, rand_int_32\n"
			"flw.ps f6, 64(x12)\n"
			"flw.ps f13, 768(x12)\n"
			"fmulh.pi f22, f6, f13\n" 
			"LBL_SEQID_3_FP_TRANS_ID_15_HID_0:\n"
			"la x12, rand_ieee754_32\n"
			"flw.ps f22, 192(x12)\n"
			"flog.ps f12, f22\n" 
			"maskpopc x28, m4\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f6, 192(x22)\n"
			"fcvt.f16.ps f14, f6\n" 
			"maskpopcz x12, m3\n" 
			"la x19, rand_int_32\n"
			"flw.ps f31, 416(x19)\n"
			"fandi.pi f8, f31, 0x155\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 0xcb2fee1246078305\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0x4473ed17022fa367\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"LBL_SEQID_3_XMA_QNT_STR_SEQ_ITER_0_HID_0:\n"
			"li x5, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x1 << TENSOR_FMA32_B_NUM_COLS) | (0xa << TENSOR_FMA32_A_NUM_ROWS) | (0xc << TENSOR_FMA32_A_NUM_COLS) | (0x1 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0xe2 << TENSOR_FMA32_SCP_LOC_B) | (0xd3 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x0 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
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
			"la x12, rand_ieee754_32\n"
			"flw.ps f15, 544(x12)\n"
			"fclass.ps f9, f15\n" 
			"la x22, rand_int_32\n"
			"flw.ps f13, 800(x22)\n"
			"flw.ps f20, 544(x22)\n"
			"fmulhu.pi f22, f13, f20\n" 
			"LBL_SEQID_4_FP_TRANS_ID_16_HID_0:\n"
			"la x11, rand_ieee754_32\n"
			"flw.ps f12, 352(x11)\n"
			"frcp.ps f20, f12\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f22, 608(x11)\n"
			"flw.ps f8, 160(x11)\n"
			"fmul.ps f13, f22, f8, rmm\n" 
			"la x11, rand_int_32\n"
			"flw.ps f19, 160(x11)\n"
			"flw.ps f31, 704(x11)\n"
			"fminu.pi f13, f19, f31\n" 
			"la x11, rand_int_32\n"
			"flw.ps f20, 160(x11)\n"
			"flw.ps f21, 352(x11)\n"
			"fle.pi f15, f20, f21\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f7, 384(x11)\n"
			"flw.ps f20, 704(x11)\n"
			"flw.ps f13, 896(x11)\n"
			"fnmadd.ps f19, f7, f20, f13, rdn\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f31, 832(x28)\n"
			"fcvt.l.s x13, f31, rtz\n" 
			"LBL_SEQID_4_FP_TRANS_ID_17_HID_0:\n"
			"la x22, rand_ieee754_32\n"
			"flw.ps f13, 320(x22)\n"
			"fexp.ps f14, f13\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f30, 576(x25)\n"
			"flw.ps f6, 128(x25)\n"
			"fmadd.ps f28, f30, f6, f30, rmm\n" 
			"li x5, 0xeb63\n"
			"csrw tensor_mask, x5\n"	
			"la x20, rand_ieee754_32\n"
			"flw.ps f31, 736(x20)\n"
			"fcvt.f16.ps f15, f31\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f14, 512(x23)\n"
			"flw.ps f20, 448(x23)\n"
			"fmax.ps f8, f14, f20\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f30, 96(x28)\n"
			"flw.ps f21, 192(x28)\n"
			"fsgnjn.s f20, f30, f21\n" 
			"li x5, 0x2d9c\n"
			"csrw tensor_mask, x5\n"	
			"la x23, rand_int_32\n"
			"flw.ps f19, 192(x23)\n"
			"flw.ps f31, 640(x23)\n"
			"fmaxu.pi f9, f19, f31\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f30, 832(x19)\n"
			"fsin.ps f6, f30\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f15, 864(x14)\n"
			"fround.ps f20, f15, rdn\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_4_TLD_XMA_STR_SEQ_ID_366250_HID_0:\n"
			"la x31, self_check_32\n"
			"li x5, 0xe0000000000007\n"
			"add x5, x5, x31\n"
			"li x6, 0x1000000000000f\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x2 << TENSOR_FMA32_B_NUM_COLS) | (0x7 << TENSOR_FMA32_A_NUM_ROWS) | (0xf << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x1 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1d << TENSOR_FMA32_SCP_LOC_B) | (0x7 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x14 << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0x7 << TENSOR_QUANT_QUANT_ROW) | (0x1a << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x6 << TENSOR_QUANT_TRANSF7) | (0x6 << TENSOR_QUANT_TRANSF6) | (0x6 << TENSOR_QUANT_TRANSF5) | (0x6 << TENSOR_QUANT_TRANSF4) | (0x6 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0x10\n"
			"bne x5, x31, LBL_FAIL_HID_0\n"
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 10\n"
			"LBL_SEQID_4_TLD_XMA_STR_SEQ_ID_573351_HID_0:\n"
			"la x31, self_check_32\n"
			"li x5, 0xe0000000000000\n"
			"add x5, x5, x31\n"
			"li x6, 0x3a000000000000a\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x3 << TENSOR_FMA32_B_NUM_COLS) | (0x0 << TENSOR_FMA32_A_NUM_ROWS) | (0xa << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1d << TENSOR_FMA32_SCP_LOC_B) | (0x7 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0xb\n"
			"bne x5, x31, LBL_FAIL_HID_0\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_0\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_4_TLD_XMA_STR_SEQ_ITER_0_HID_0:\n"
			"la x31, self_check_32\n"
			"li x5, 0x40000000000007\n"
			"add x5, x5, x31\n"
			"li x6, 0x1000000000000f\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x3 << TENSOR_FMA32_B_NUM_COLS) | (0x7 << TENSOR_FMA32_A_NUM_ROWS) | (0xf << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x1 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1b << TENSOR_FMA32_SCP_LOC_B) | (0x2 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x9, ((0xb << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0x7 << TENSOR_QUANT_QUANT_ROW) | (0x22 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x2 << TENSOR_QUANT_TRANSF4) | (0x6 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0x10\n"
			"bne x5, x31, LBL_FAIL_HID_0\n"
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 10\n"
			"LBL_SEQID_4_TLD_XMA_STR_SEQ_ITER_1_HID_0:\n"
			"la x31, self_check_8\n"
			"li x5, 0x2000000000000f\n"
			"add x5, x5, x31\n"
			"li x6, 0x10000000000003\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x3 << TENSOR_IMA_B_NUM_COLS) | (0xf << TENSOR_IMA_A_NUM_ROWS) | (0x3 << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x1 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1e << TENSOR_IMA_SCP_LOC_B) | (0x1 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x9, ((0x14 << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0xf << TENSOR_QUANT_QUANT_ROW) | (0x35 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x4 << TENSOR_QUANT_TRANSF1) | (0x9 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fmvz.x.ps x31, f0, 0\n"
			"li x5, 0x10\n"
			"bne x5, x31, LBL_FAIL_HID_0\n"
			"csrw tensor_quant, x9\n"	
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
			"la x19, rand_ieee754_32\n"
			"flw.ps f19, 544(x19)\n"
			"fround.ps f14, f19, rmm\n" 
			"li x5, 0x852d9c4ebe541485\n"
			"fcvt.s.lu f14, x5, rtz\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f14, 288(x14)\n"
			"fclass.s x13, f14\n" 
			"la x23, rand_int_32\n"
			"flw.ps f7, 960(x23)\n"
			"flw.ps f28, 704(x23)\n"
			"fxor.pi f22, f7, f28\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f6, 512(x5)\n"
			"flw.ps f7, 320(x5)\n"
			"fmul.ps f8, f6, f7, rtz\n" 
			"li x5, 0xcf77\n"
			"csrw tensor_mask, x5\n"	
			"LBL_SEQID_5_M0_WRITE_ID_18_HID_0:\n"
			"fltm.ps m4, f13, f14\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x22, rand_int_32\n"
			"flw.ps f22, 416(x22)\n"
			"flw.ps f13, 928(x22)\n"
			"frem.pi f13, f22, f13\n" 
			"LBL_SEQID_5_M0_WRITE_ID_19_HID_0:\n"
			"mova.m.x x14\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x22, rand_ieee754_32\n"
			"flw.ps f14, 224(x22)\n"
			"flw.ps f13, 992(x22)\n"
			"fmul.ps f20, f14, f13, rtz\n" 
			"la x14, rand_int_32\n"
			"flw.ps f1, 320(x14)\n"
			"flw.ps f15, 480(x14)\n"
			"feq.pi f8, f1, f15\n" 
			"LBL_SEQID_5_M0_WRITE_ID_20_HID_0:\n"
			"fsetm.pi m3, f31\n" 
			"li x5, CREDINC3\n"
			"ld x5, 0(x5)\n"
			"la x23, rand_ieee754_32\n"
			"flw.ps f22, 352(x23)\n"
			"fcvt.pwu.ps f20, f22, rmm\n" 
			"la x25, rand_int_32\n"
			"flw.ps f31, 96(x25)\n"
			"fcvt.ps.pwu f30, f31, rtz\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"li x19, 0xa29af08ed981c82\n"
			"fcvt.s.wu f20, x19, rtz\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f7, 928(x12)\n"
			"flw.ps f30, 992(x12)\n"
			"fmax.ps f1, f7, f30\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f30, 544(x28)\n"
			"flw.ps f1, 288(x28)\n"
			"feq.ps f15, f30, f1\n" 
			"csrw tensor_error, zero\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"LBL_SEQID_5_XMA_QNT_STR_SEQ_ID_795178_HID_0:\n"
			"li x6, ((0x4 << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0xf << TENSOR_QUANT_QUANT_ROW) | (0x6 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_5_XMA_QNT_STR_SEQ_ID_542508_HID_0:\n"
			"li x6, ((0x5 << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0x1 << TENSOR_QUANT_QUANT_ROW) | (0x6 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x2 << TENSOR_QUANT_TRANSF9) | (0x7 << TENSOR_QUANT_TRANSF8) | (0x7 << TENSOR_QUANT_TRANSF7) | (0x6 << TENSOR_QUANT_TRANSF6) | (0x6 << TENSOR_QUANT_TRANSF5) | (0x6 << TENSOR_QUANT_TRANSF4) | (0x6 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_0\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_5_XMA_QNT_STR_SEQ_ITER_0_HID_0:\n"
			"li x6, ((0x15 << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0x6 << TENSOR_QUANT_QUANT_ROW) | (0x2d << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x1 << TENSOR_QUANT_TRANSF5) | (0x9 << TENSOR_QUANT_TRANSF4) | (0x9 << TENSOR_QUANT_TRANSF3) | (0x8 << TENSOR_QUANT_TRANSF2) | (0x5 << TENSOR_QUANT_TRANSF1) | (0x5 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_5_XMA_QNT_STR_SEQ_ITER_1_HID_0:\n"
			"li x6, ((0x17 << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0xd << TENSOR_QUANT_QUANT_ROW) | (0x4 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x0 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_0\n"
			"csrw tensor_error, x0\n"
			"li x5, 6\n" 
			"bne x30, x5, LBL_FAIL_HID_0\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_6_M0_WRITE_ID_21_HID_0:\n"
			"mova.m.x x14\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x20, rand_ieee754_32\n"
			"flw.ps f7, 448(x20)\n"
			"flw.ps f31, 704(x20)\n"
			"flw.ps f22, 160(x20)\n"
			"fmsub.ps f31, f7, f31, f22, rdn\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f9, 192(x20)\n"
			"flw.ps f20, 896(x20)\n"
			"fsgnj.s f6, f9, f20\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x19, rand_ieee754_32\n"
			"flw.ps f12, 224(x19)\n"
			"flw.ps f28, 64(x19)\n"
			"fmax.ps f19, f12, f28\n" 
			"la x14, rand_int_32\n"
			"flw.ps f15, 448(x14)\n"
			"faddi.pi f13, f15, 0x163\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f30, 224(x23)\n"
			"fclass.ps f21, f30\n" 
			"la x23, rand_int_32\n"
			"flw.ps f12, 384(x23)\n"
			"flw.ps f20, 512(x23)\n"
			"fsll.pi f14, f12, f20\n" 
			"mova.x.m x23\n" 
			"LBL_SEQID_6_M0_WRITE_ID_22_HID_0:\n"
			"mova.m.x x23\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x19, rand_int_32\n"
			"flw.ps f19, 352(x19)\n"
			"flw.ps f28, 672(x19)\n"
			"fmax.pi f14, f19, f28\n" 
			"la x23, rand_int_32\n"
			"flw.ps f31, 160(x23)\n"
			"flw.ps f9, 0(x23)\n"
			"fsll.pi f31, f31, f9\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x19, rand_ieee754_32\n"
			"flw.ps f14, 96(x19)\n"
			"fmin.s f31, f14, f14\n" 
			"la x25, rand_int_32\n"
			"flw.ps f22, 448(x25)\n"
			"fpackreph.pi f9, f22\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f30, 512(x14)\n"
			"fcvt.pwu.ps f13, f30, rup\n" 
			"li x5, 0x8113\n"
			"csrw tensor_mask, x5\n"	
			"la x22, rand_ieee754_32\n"
			"flw.ps f8, 576(x22)\n"
			"flw.ps f14, 704(x22)\n"
			"fmul.s f28, f8, f14, rne\n" 
			"fbci.ps f13, 0x4367d\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_6_TLD_XMA_STR_SEQ_ID_787893_HID_0:\n"
			"la x31, self_check_16\n"
			"li x5, 0xc0000000000009\n"
			"add x5, x5, x31\n"
			"li x6, 0x3e0000000000002\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x0 << TENSOR_FMA16_B_NUM_COLS) | (0x9 << TENSOR_FMA16_A_NUM_ROWS) | (0x2 << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1f << TENSOR_FMA16_SCP_LOC_B) | (0x6 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
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
			"LBL_SEQID_6_TLD_XMA_STR_SEQ_ID_532533_HID_0:\n"
			"la x31, self_check_32\n"
			"li x5, 0xa0000000000006\n"
			"add x5, x5, x31\n"
			"li x6, 0x380000000000001\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x3 << TENSOR_FMA32_B_NUM_COLS) | (0x6 << TENSOR_FMA32_A_NUM_ROWS) | (0x1 << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1c << TENSOR_FMA32_SCP_LOC_B) | (0x5 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x1b << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0x6 << TENSOR_QUANT_QUANT_ROW) | (0x2a << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x2 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"bne x5, x31, LBL_FAIL_HID_0\n"
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_0\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_6_XMA_QNT_STR_SEQ_ITER_0_HID_0:\n"
			"li x6, ((0x0 << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0x3 << TENSOR_QUANT_QUANT_ROW) | (0x1c << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x7 << TENSOR_QUANT_TRANSF6) | (0x7 << TENSOR_QUANT_TRANSF5) | (0x7 << TENSOR_QUANT_TRANSF4) | (0x7 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
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
			"LBL_HPM_3_CORE_DCACHE_MISSES0_FAIL_HID_0:\n"          
			"csrr x5, mhpmcounter3\n"
			"sd x5, 32 (x31)\n"
			"LBL_HPM_4_CORE_TL_INST_FAIL_HID_0:\n"          
			"csrr x5, mhpmcounter4\n"
			"sd x5, 40 (x31)\n"
			"LBL_HPM_5_CORE_BRANCHES0_FAIL_HID_0:\n"          
			"csrr x5, mhpmcounter5\n"
			"sd x5, 48 (x31)\n"
			"LBL_HPM_6_CORE_TS_OPS_FAIL_HID_0:\n"          
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
			"LBL_HPM_3_CORE_DCACHE_MISSES0_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter3\n"
			"sd x5, 32 (x31)\n"
			"LBL_HPM_4_CORE_TL_INST_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter4\n"
			"sd x5, 40 (x31)\n"
			"LBL_HPM_5_CORE_BRANCHES0_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter5\n"
			"sd x5, 48 (x31)\n"
			"LBL_HPM_6_CORE_TS_OPS_PASS_HID_0:\n"          
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
			"csrwi mhpmevent3, 2\n"   
			"csrwi mhpmevent4, 1\n"   
			"csrwi mhpmevent5, 3\n"   
			"csrwi mhpmevent6, 6\n"   
			"csrwi mhpmevent7, 22\n"   
			"csrwi mhpmevent8, 2\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f3,  256 (x5)\n"
			"flw.ps f11,  544 (x5)\n"
			"flw.ps f4,  384 (x5)\n"
			"flw.ps f14,  416 (x5)\n"
			"flw.ps f18,  192 (x5)\n"
			"flw.ps f22,  288 (x5)\n"
			"flw.ps f19,  512 (x5)\n"
			"flw.ps f9,  704 (x5)\n"
			"flw.ps f27,  96 (x5)\n"
			"flw.ps f17,  0 (x5)\n"
			"flw.ps f28,  320 (x5)\n"
			"flw.ps f26,  864 (x5)\n"
			"flw.ps f23,  768 (x5)\n"
			"flw.ps f1,  896 (x5)\n"
			"flw.ps f6,  224 (x5)\n"
			"flw.ps f0,  640 (x5)\n"
			"flw.ps f30,  32 (x5)\n"
			"flw.ps f5,  608 (x5)\n"
			"flw.ps f24,  448 (x5)\n"
			"flw.ps f20,  64 (x5)\n"
			"flw.ps f7,  832 (x5)\n"
			"flw.ps f2,  352 (x5)\n"
			"flw.ps f25,  992 (x5)\n"
			"flw.ps f10,  128 (x5)\n"
			"flw.ps f21,  736 (x5)\n"
			"flw.ps f31,  480 (x5)\n"
			"flw.ps f16,  160 (x5)\n"
			"flw.ps f15,  960 (x5)\n"
			"flw.ps f13,  928 (x5)\n"
			"flw.ps f29,  672 (x5)\n"
			"flw.ps f8,  800 (x5)\n"
			"flw.ps f12,  576 (x5)\n"
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
			"la x17, rand_int_32\n"
			"flw.ps f27, 256(x17)\n"
			"fle.pi f18, f27, f27\n" 
			"la x23, rand_int_32\n"
			"flw.ps f30, 192(x23)\n"
			"flw.ps f6, 640(x23)\n"
			"fsra.pi f18, f30, f6\n" 
			"la x28, rand_int_32\n"
			"flw.ps f17, 896(x28)\n"
			"flw.ps f27, 544(x28)\n"
			"for.pi f6, f17, f27\n" 
			"la x28, rand_int_32\n"
			"flw.ps f17, 192(x28)\n"
			"flw.ps f19, 96(x28)\n"
			"feq.pi f24, f17, f19\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f17, 704(x12)\n"
			"flw.ps f19, 704(x12)\n"
			"fsgnjn.s f2, f17, f19\n" 
			"la x17, rand_int_32\n"
			"flw.ps f23, 512(x17)\n"
			"flw.ps f24, 960(x17)\n"
			"fmulh.pi f31, f23, f24\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f30, 320(x12)\n"
			"ffrc.ps f24, f30\n" 
			"la x12, rand_int_32\n"
			"flw.ps f8, 992(x12)\n"
			"flw.ps f30, 864(x12)\n"
			"fsll.pi f24, f8, f30\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f8, 288(x27)\n"
			"flw.ps f9, 768(x27)\n"
			"fsgnjn.s f11, f8, f9\n" 
			"la x6, rand_int_32\n"
			"flw.ps f6, 960(x6)\n"
			"flw.ps f31, 992(x6)\n"
			"fsrl.pi f15, f6, f31\n" 
			"la x10, rand_int_32\n"
			"flw.ps f9, 160(x10)\n"
			"flw.ps f31, 928(x10)\n"
			"fmin.pi f6, f9, f31\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f23, 512(x6)\n"
			"flw.ps f11, 288(x6)\n"
			"fdiv.s f30, f23, f11, rdn\n" 
			"la x15, rand_int_32\n"
			"flw.ps f19, 704(x15)\n"
			"fandi.pi f9, f19, 0x12\n" 
			"la x6, rand_int_32\n"
			"flw.ps f8, 96(x6)\n"
			"flw.ps f14, 608(x6)\n"
			"fsra.pi f14, f8, f14\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f22, 192(x14)\n"
			"flw.ps f17, 32(x14)\n"
			"fadd.s f18, f22, f17, rtz\n" 
			"la x10, rand_ieee754_16\n"
			"flw.ps f17, 928(x10)\n"
			"fcvt.ps.f16 f19, f17\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_1\n"
			"addi x30, x30, 1\n"
			"la x27, rand_int_32\n"
			"flw.ps f6, 544(x27)\n"
			"flw.ps f31, 192(x27)\n"
			"fsra.pi f31, f6, f31\n" 
			"LBL_SEQID_1_M0_WRITE_ID_0_HID_1:\n"
			"maskand m4, m2, m3\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0x65a65b1180\n"
			"flw.ps f5, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_1_M0_WRITE_ID_1_HID_1:\n"
			"mov.m.x m3, x15, 0x23\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"li x15, 0x99e70cdcfed6e0eb\n"
			"fcvt.s.wu f23, x15, rne\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f11, 672(x12)\n"
			"fclass.ps f11, f11\n" 
			"la x17, rand_int_32\n"
			"flw.ps f14, 160(x17)\n"
			"flw.ps f11, 800(x17)\n"
			"fmulhu.pi f6, f14, f11\n" 
			"LBL_SEQID_1_M0_WRITE_ID_2_HID_1:\n"
			"maskand m6, m4, m3\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x27, rand_ieee754_32\n"
			"flw.ps f6, 224(x27)\n"
			"fround.ps f2, f6, rup\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f8, 960(x14)\n"
			"flw.ps f27, 608(x14)\n"
			"flw.ps f30, 896(x14)\n"
			"fmadd.ps f23, f8, f27, f30, rtz\n" 
			"la x12, rand_int_32\n"
			"flw.ps f14, 896(x12)\n"
			"flw.ps f15, 704(x12)\n"
			"feq.pi f2, f14, f15\n" 
			"la x28, rand_int_32\n"
			"flw.ps f9, 512(x28)\n"
			"fpackreph.pi f27, f9\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f15, 672(x17)\n"
			"flw.ps f27, 352(x17)\n"
			"fadd.ps f30, f15, f27, rmm\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f18, 960(x18)\n"
			"fsin.ps f6, f18\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f15, 256(x12)\n"
			"flw.ps f31, 416(x12)\n"
			"fle.ps f8, f15, f31\n" 
			"la x28, rand_int_32\n"
			"flw.ps f27, 320(x28)\n"
			"flw.ps f23, 64(x28)\n"
			"fltu.pi f30, f27, f23\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f6, 864(x6)\n"
			"fmax.ps f2, f6, f6\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_1\n"
			"addi x30, x30, 1\n"
			"la x15, rand_int_32\n"
			"flw.ps f22, 128(x15)\n"
			"fsat8.pi f27, f22\n" 
			"la x15, rand_int_32\n"
			"flw.ps f8, 864(x15)\n"
			"flw.ps f19, 288(x15)\n"
			"fmaxu.pi f30, f8, f19\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f15, 448(x10)\n"
			"flw.ps f22, 128(x10)\n"
			"flw.ps f23, 64(x10)\n"
			"fmsub.ps f14, f15, f22, f23, rne\n" 
			"LBL_SEQID_2_M0_WRITE_ID_3_HID_1:\n"
			"mov.m.x m4, x18, 0x28\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_2_M0_WRITE_ID_4_HID_1:\n"
			"masknot m3, m4\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x10, rand_int_32\n"
			"flw.ps f27, 64(x10)\n"
			"flw.ps f18, 640(x10)\n"
			"fminu.pi f8, f27, f18\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f24, 608(x6)\n"
			"fcvt.w.s x27, f24, rup\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f19, 800(x6)\n"
			"fcvt.pwu.ps f22, f19, rdn\n" 
			"LBL_SEQID_2_M0_WRITE_ID_5_HID_1:\n"
			"feqm.ps m4, f8, f27\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0xb00aa828c0\n"
			"fsw.ps f0, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x17, rand_ieee754_32\n"
			"flw.ps f15, 480(x17)\n"
			"flw.ps f9, 288(x17)\n"
			"fmul.ps f17, f15, f9, rup\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f15, 544(x14)\n"
			"flw.ps f27, 288(x14)\n"
			"flw.ps f2, 736(x14)\n"
			"fmsub.ps f11, f15, f27, f2, rup\n" 
			"la x6, rand_int_32\n"
			"flw.ps f8, 704(x6)\n"
			"flw.ps f9, 736(x6)\n"
			"fxor.pi f11, f8, f9\n" 
			"la x28, rand_int_32\n"
			"flw.ps f23, 448(x28)\n"
			"flw.ps f6, 768(x28)\n"
			"fadd.pi f8, f23, f6\n" 
			"mova.x.m x15\n" 
			"LBL_SEQID_2_M0_WRITE_ID_6_HID_1:\n"
			"mov.m.x m6, x6, 0x6d\n" 
			"li x5, CREDINC0\n"
			"ld x5, 0(x5)\n"
			"la x18, rand_ieee754_32\n"
			"flw.ps f19, 864(x18)\n"
			"flw.ps f8, 416(x18)\n"
			"fmax.ps f30, f19, f8\n" 
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
			"flw.ps f8, 672(x12)\n"
			"flw.ps f6, 32(x12)\n"
			"flw.ps f15, 352(x12)\n"
			"fmadd.ps f31, f8, f6, f15, rup\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f30, 736(x15)\n"
			"flw.ps f14, 704(x15)\n"
			"fmul.s f14, f30, f14, rmm\n" 
			"la x17, rand_int_32\n"
			"flw.ps f19, 832(x17)\n"
			"fcvt.ps.pwu f17, f19, rup\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f19, 96(x18)\n"
			"flw.ps f2, 448(x18)\n"
			"fmsub.ps f27, f19, f2, f19, rdn\n" 
			"la x18, rand_ieee754_16\n"
			"flw.ps f6, 352(x18)\n"
			"fcvt.ps.f16 f30, f6\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f14, 160(x10)\n"
			"fsin.ps f19, f14\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f30, 832(x28)\n"
			"flw.ps f8, 800(x28)\n"
			"fsgnj.s f2, f30, f8\n" 
			"la x15, rand_int_32\n"
			"flw.ps f15, 736(x15)\n"
			"flw.ps f11, 320(x15)\n"
			"flt.pi f22, f15, f11\n" 
			"LBL_SEQID_3_M0_WRITE_ID_7_HID_1:\n"
			"fsetm.pi m4, f27\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x17, rand_int_32\n"
			"flw.ps f22, 512(x17)\n"
			"flw.ps f23, 640(x17)\n"
			"fle.pi f19, f22, f23\n" 
			"la x15, rand_int_32\n"
			"flw.ps f11, 384(x15)\n"
			"flw.ps f27, 608(x15)\n"
			"for.pi f6, f11, f27\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f18, 672(x18)\n"
			"flw.ps f24, 608(x18)\n"
			"fcmovm.ps f2, f18, f24\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f27, 0(x18)\n"
			"flw.ps f18, 224(x18)\n"
			"flt.ps f6, f27, f18\n" 
			"la x17, rand_int_32\n"
			"flw.ps f24, 832(x17)\n"
			"fsatu8.pi f9, f24\n" 
			"LBL_SEQID_3_M0_WRITE_ID_8_HID_1:\n"
			"fltm.pi m4, f14, f19\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_3_M0_WRITE_ID_9_HID_1:\n"
			"maskand m4, m6, m4\n" 
			"mov.m.x m0, x0, 0xFF\n"
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
			"fbci.ps f23, 0x6e98e\n" 
			"li x15, 0xae7be9801cc59f7f\n"
			"fcvt.s.l f9, x15, rdn\n" 
			"maskpopcz x23, m6\n" 
			"la x14, rand_int_32\n"
			"flw.ps f24, 352(x14)\n"
			"fpackrepb.pi f17, f24\n" 
			"la x28, rand_int_32\n"
			"flw.ps f14, 32(x28)\n"
			"flw.ps f2, 992(x28)\n"
			"fminu.pi f19, f14, f2\n" 
			"la x14, rand_int_32\n"
			"flw.ps f17, 960(x14)\n"
			"fsrli.pi f27, f17, 0x4\n" 
			"fbci.ps f19, 0x764c5\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f30, 480(x14)\n"
			"flw.ps f2, 736(x14)\n"
			"flw.ps f27, 288(x14)\n"
			"fcmov.ps f14, f30, f2, f27\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f17, 224(x17)\n"
			"fsin.ps f22, f17\n" 
			"la x10, rand_int_32\n"
			"flw.ps f19, 544(x10)\n"
			"fandi.pi f19, f19, 0x5b\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f14, 96(x12)\n"
			"fclass.ps f6, f14\n" 
			"la x18, rand_int_32\n"
			"flw.ps f24, 480(x18)\n"
			"flw.ps f14, 96(x18)\n"
			"flt.pi f23, f24, f14\n" 
			"la x14, rand_int_32\n"
			"flw.ps f23, 64(x14)\n"
			"flw.ps f22, 640(x14)\n"
			"fmax.pi f14, f23, f22\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f15, 64(x23)\n"
			"flw.ps f11, 224(x23)\n"
			"fsgnjn.s f27, f15, f11\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f19, 416(x17)\n"
			"flw.ps f31, 672(x17)\n"
			"fmul.ps f15, f19, f31, rtz\n" 
			"la x12, rand_int_32\n"
			"flw.ps f14, 896(x12)\n"
			"fsatu8.pi f9, f14\n" 
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
			"maskpopc x23, m3\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f24, 544(x17)\n"
			"fswizz.ps f9, f24, 0x15\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f24, 832(x23)\n"
			"fclass.ps f30, f24\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f15, 928(x23)\n"
			"flw.ps f11, 32(x23)\n"
			"fsgnjx.s f8, f15, f11\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f8, 416(x6)\n"
			"flw.ps f2, 736(x6)\n"
			"fsgnjx.ps f17, f8, f2\n" 
			"la x27, rand_int_32\n"
			"flw.ps f2, 160(x27)\n"
			"fsat8.pi f6, f2\n" 
			"la x15, rand_int_32\n"
			"flw.ps f24, 544(x15)\n"
			"flw.ps f11, 416(x15)\n"
			"fmulh.pi f2, f24, f11\n" 
			"li x23, 0xf01d4086e5b82150\n"
			"fcvt.s.wu f6, x23, rne\n" 
			"la x6, rand_int_32\n"
			"flw.ps f24, 768(x6)\n"
			"flw.ps f9, 736(x6)\n"
			"feq.pi f11, f24, f9\n" 
			"la x28, rand_int_32\n"
			"flw.ps f11, 704(x28)\n"
			"fcvt.ps.pwu f27, f11, rne\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f24, 736(x17)\n"
			"flw.ps f31, 192(x17)\n"
			"feq.s x17, f24, f31\n" 
			"LBL_SEQID_5_M0_WRITE_ID_10_HID_1:\n"
			"masknot m6, m4\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x17, rand_ieee754_32\n"
			"flw.ps f11, 96(x17)\n"
			"feq.s x6, f11, f11\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f23, 576(x23)\n"
			"fcvt.wu.s x18, f23, rtz\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f15, 832(x15)\n"
			"flw.ps f11, 704(x15)\n"
			"flw.ps f2, 384(x15)\n"
			"fnmsub.ps f17, f15, f11, f2, rdn\n" 
			"mova.x.m x18\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 6\n" 
			"bne x30, x5, LBL_FAIL_HID_1\n"
			"addi x30, x30, 1\n"
			"la x15, rand_ieee754_32\n"
			"flw.ps f2, 160(x15)\n"
			"flw.ps f19, 832(x15)\n"
			"fle.ps f31, f2, f19\n" 
			"la x12, rand_int_32\n"
			"flw.ps f2, 448(x12)\n"
			"fsrai.pi f22, f2, 0xc\n" 
			"la x14, rand_int_32\n"
			"flw.ps f31, 800(x14)\n"
			"flw.ps f19, 224(x14)\n"
			"flt.pi f11, f31, f19\n" 
			"la x15, rand_int_32\n"
			"flw.ps f2, 224(x15)\n"
			"flw.ps f15, 352(x15)\n"
			"fsra.pi f17, f2, f15\n" 
			"la x6, rand_int_32\n"
			"flw.ps f19, 512(x6)\n"
			"flw.ps f2, 128(x6)\n"
			"fmulh.pi f27, f19, f2\n" 
			"la x28, rand_int_32\n"
			"flw.ps f23, 352(x28)\n"
			"fsrli.pi f15, f23, 0x6\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f23, 832(x10)\n"
			"flw.ps f24, 768(x10)\n"
			"fmin.s f27, f23, f24\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f18, 416(x12)\n"
			"ffrc.ps f17, f18\n" 
			"mova.x.m x10\n" 
			"LBL_SEQID_6_M0_WRITE_ID_11_HID_1:\n"
			"flem.ps m2, f2, f31\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x28, rand_ieee754_32\n"
			"flw.ps f11, 224(x28)\n"
			"frsq.ps f2, f11\n" 
			"LBL_SEQID_6_M0_WRITE_ID_12_HID_1:\n"
			"mov.m.x m4, x23, 0x55\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x23, rand_int_32\n"
			"flw.ps f19, 448(x23)\n"
			"fandi.pi f22, f19, 0x15d\n" 
			"la x12, rand_int_32\n"
			"flw.ps f15, 160(x12)\n"
			"fcvt.ps.pwu f8, f15, rup\n" 
			"la x12, rand_int_32\n"
			"flw.ps f24, 800(x12)\n"
			"flw.ps f17, 480(x12)\n"
			"fadd.pi f31, f24, f17\n" 
			"LBL_SEQID_6_M0_WRITE_ID_13_HID_1:\n"
			"maskor m6, m3, m6\n" 
			"mov.m.x m0, x0, 0xFF\n"
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
			"LBL_HPM_3_CORE_RETIRED_INST0_FAIL_HID_1:\n"          
			"csrr x5, mhpmcounter3\n"
			"sd x5, 32 (x31)\n"
			"LBL_HPM_4_CORE_MCYCLES_FAIL_HID_1:\n"          
			"csrr x5, mhpmcounter4\n"
			"sd x5, 40 (x31)\n"
			"LBL_HPM_5_CORE_RETIRED_INST1_FAIL_HID_1:\n"          
			"csrr x5, mhpmcounter5\n"
			"sd x5, 48 (x31)\n"
			"LBL_HPM_6_CORE_DCACHE_ACCESS0_FAIL_HID_1:\n"          
			"csrr x5, mhpmcounter6\n"
			"sd x5, 56 (x31)\n"
			"LBL_HPM_7_NEIGH_ETL_REQ_TO_SC_UC_FIFO_FAIL_HID_1:\n"          
			"csrr x5, mhpmcounter7\n"
			"sd x5, 64 (x31)\n"
			"LBL_HPM_8_NEIGH_ANY_CORE_RX_ETL_RSP_FAIL_HID_1:\n"          
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
			"LBL_HPM_3_CORE_RETIRED_INST0_PASS_HID_1:\n"          
			"csrr x5, mhpmcounter3\n"
			"sd x5, 32 (x31)\n"
			"LBL_HPM_4_CORE_MCYCLES_PASS_HID_1:\n"          
			"csrr x5, mhpmcounter4\n"
			"sd x5, 40 (x31)\n"
			"LBL_HPM_5_CORE_RETIRED_INST1_PASS_HID_1:\n"          
			"csrr x5, mhpmcounter5\n"
			"sd x5, 48 (x31)\n"
			"LBL_HPM_6_CORE_DCACHE_ACCESS0_PASS_HID_1:\n"          
			"csrr x5, mhpmcounter6\n"
			"sd x5, 56 (x31)\n"
			"LBL_HPM_7_NEIGH_ETL_REQ_TO_SC_UC_FIFO_PASS_HID_1:\n"          
			"csrr x5, mhpmcounter7\n"
			"sd x5, 64 (x31)\n"
			"LBL_HPM_8_NEIGH_ANY_CORE_RX_ETL_RSP_PASS_HID_1:\n"          
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
			"csrwi mhpmevent3, 8\n"   
			"csrwi mhpmevent4, 14\n"   
			"csrwi mhpmevent5, 4\n"   
			"csrwi mhpmevent6, 17\n"   
			"csrwi mhpmevent7, 6\n"   
			"csrwi mhpmevent8, 3\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f15,  512 (x5)\n"
			"flw.ps f20,  288 (x5)\n"
			"flw.ps f23,  32 (x5)\n"
			"flw.ps f25,  0 (x5)\n"
			"flw.ps f29,  736 (x5)\n"
			"flw.ps f8,  64 (x5)\n"
			"flw.ps f26,  480 (x5)\n"
			"flw.ps f2,  160 (x5)\n"
			"flw.ps f14,  448 (x5)\n"
			"flw.ps f0,  864 (x5)\n"
			"flw.ps f1,  256 (x5)\n"
			"flw.ps f6,  544 (x5)\n"
			"flw.ps f11,  960 (x5)\n"
			"flw.ps f19,  896 (x5)\n"
			"flw.ps f31,  832 (x5)\n"
			"flw.ps f7,  416 (x5)\n"
			"flw.ps f22,  704 (x5)\n"
			"flw.ps f28,  352 (x5)\n"
			"flw.ps f4,  128 (x5)\n"
			"flw.ps f5,  672 (x5)\n"
			"flw.ps f12,  992 (x5)\n"
			"flw.ps f13,  640 (x5)\n"
			"flw.ps f27,  768 (x5)\n"
			"flw.ps f17,  608 (x5)\n"
			"flw.ps f30,  192 (x5)\n"
			"flw.ps f3,  384 (x5)\n"
			"flw.ps f9,  576 (x5)\n"
			"flw.ps f16,  928 (x5)\n"
			"flw.ps f10,  800 (x5)\n"
			"flw.ps f21,  96 (x5)\n"
			"flw.ps f24,  320 (x5)\n"
			"flw.ps f18,  224 (x5)\n"
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
			"LBL_SEQID_0_FP_TRANS_ID_0_HID_2:\n"
			"la x26, rand_ieee754_32\n"
			"flw.ps f0, 992(x26)\n"
			"flog.ps f30, f0\n" 
			"la x10, rand_int_32\n"
			"flw.ps f15, 768(x10)\n"
			"flw.ps f9, 224(x10)\n"
			"fltu.pi f18, f15, f9\n" 
			"la x16, rand_int_32\n"
			"flw.ps f23, 192(x16)\n"
			"fpackrepb.pi f27, f23\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f31, 800(x11)\n"
			"flw.ps f5, 576(x11)\n"
			"fle.ps f0, f31, f5\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f31, 544(x11)\n"
			"fsin.ps f26, f31\n" 
			"la x26, rand_int_32\n"
			"flw.ps f14, 800(x26)\n"
			"fcvt.ps.pwu f27, f14, rdn\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f19, 192(x6)\n"
			"fround.ps f12, f19, rdn\n" 
			"la x9, rand_int_32\n"
			"flw.ps f27, 96(x9)\n"
			"flw.ps f9, 672(x9)\n"
			"fsll.pi f26, f27, f9\n" 
			"la x28, rand_int_32\n"
			"flw.ps f15, 224(x28)\n"
			"flw.ps f14, 352(x28)\n"
			"fand.pi f0, f15, f14\n" 
			"li x5, 0x714a\n"
			"csrw tensor_mask, x5\n"	
			"la x10, rand_ieee754_32\n"
			"flw.ps f30, 0(x10)\n"
			"flw.ps f0, 160(x10)\n"
			"fmax.s f0, f30, f0\n" 
			"mova.x.m x11\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f0, 512(x9)\n"
			"flw.ps f9, 992(x9)\n"
			"flw.ps f23, 768(x9)\n"
			"fcmov.ps f31, f0, f9, f23\n" 
			"li x16, 0x91bdc1e11257e52c\n"
			"fcvt.s.wu f23, x16, rdn\n" 
			"la x6, rand_int_32\n"
			"flw.ps f12, 352(x6)\n"
			"fcvt.ps.pw f30, f12, rup\n" 
			"la x17, rand_int_32\n"
			"flw.ps f14, 992(x17)\n"
			"fpackreph.pi f5, f14\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f27, 64(x19)\n"
			"fclass.ps f9, f27\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ID_935290_HID_2:\n"
			"la x31, self_check_8\n"
			"li x5, 0x6000000000000b\n"
			"add x5, x5, x31\n"
			"li x6, 0x1000000000000c\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x0 << TENSOR_IMA_B_NUM_COLS) | (0xb << TENSOR_IMA_A_NUM_ROWS) | (0xc << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x1 << TENSOR_IMA_TEN_B_IN_MEM) | (0x18 << TENSOR_IMA_SCP_LOC_B) | (0x3 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fmvz.x.ps x31, f0, 0\n"
			"li x5, 0x34\n"
			"bne x5, x31, LBL_FAIL_HID_2\n"
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ID_74120_HID_2:\n"
			"la x31, self_check_16\n"
			"li x5, 0x2000000000000e\n"
			"add x5, x5, x31\n"
			"li x6, 0x3c000000000000e\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x2 << TENSOR_FMA16_B_NUM_COLS) | (0xe << TENSOR_FMA16_A_NUM_ROWS) | (0xe << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1e << TENSOR_FMA16_SCP_LOC_B) | (0x1 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x1e << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0xe << TENSOR_QUANT_QUANT_ROW) | (0x4 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x2 << TENSOR_QUANT_TRANSF7) | (0x7 << TENSOR_QUANT_TRANSF6) | (0x7 << TENSOR_QUANT_TRANSF5) | (0x7 << TENSOR_QUANT_TRANSF4) | (0x6 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"bne x5, x31, LBL_FAIL_HID_2\n"
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_2\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_0_RDX_ID_0_SHIRE_0_HID_2:\n"
			"li x20, 0x3a0000000353000b\n"
			"csrw tensor_reduce, x20\n" 
			"csrwi tensor_wait, 9\n" 
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=0); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ITER_0_HID_2:\n"
			"la x31, self_check_32\n"
			"li x5, 0x20000000000006\n"
			"add x5, x5, x31\n"
			"li x6, 0x36000000000000a\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x2 << TENSOR_FMA32_B_NUM_COLS) | (0x6 << TENSOR_FMA32_A_NUM_ROWS) | (0xa << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1b << TENSOR_FMA32_SCP_LOC_B) | (0x1 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0xb\n"
			"bne x5, x31, LBL_FAIL_HID_2\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_2\n"
			"csrw tensor_error, x0\n"
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_2\n"
			"addi x30, x30, 1\n"
			"la x16, rand_ieee754_32\n"
			"flw.ps f4, 864(x16)\n"
			"flw.ps f9, 64(x16)\n"
			"fmul.s f0, f4, f9, rne\n" 
			"la x16, rand_int_32\n"
			"flw.ps f0, 800(x16)\n"
			"flw.ps f26, 704(x16)\n"
			"flt.pi f5, f0, f26\n" 
			"LBL_SEQID_1_FP_TRANS_ID_1_HID_2:\n"
			"la x6, rand_ieee754_32\n"
			"flw.ps f19, 896(x6)\n"
			"fexp.ps f12, f19\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x26, rand_ieee754_32\n"
			"flw.ps f9, 320(x26)\n"
			"fcvt.w.s x9, f9, rup\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f9, 320(x17)\n"
			"flw.ps f15, 640(x17)\n"
			"flw.ps f30, 544(x17)\n"
			"fmsub.ps f4, f9, f15, f30, rdn\n" 
			"maskpopcz x17, m3\n" 
			"la x6, rand_int_32\n"
			"flw.ps f12, 672(x6)\n"
			"flw.ps f0, 256(x6)\n"
			"fsra.pi f14, f12, f0\n" 
			"LBL_SEQID_1_M0_WRITE_ID_2_HID_2:\n"
			"feqm.ps m3, f14, f26\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_1_M0_WRITE_ID_3_HID_2:\n"
			"mov.m.x m5, x16, 0x65\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x10, rand_ieee754_32\n"
			"flw.ps f19, 96(x10)\n"
			"flw.ps f12, 896(x10)\n"
			"flt.ps f18, f19, f12\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f15, 928(x19)\n"
			"flw.ps f14, 800(x19)\n"
			"feq.s x17, f15, f14\n" 
			"la x11, rand_int_32\n"
			"flw.ps f19, 192(x11)\n"
			"fadd.pi f17, f19, f19\n" 
			"li x6, 0xb9b67f67e35a0f77\n"
			"fcvt.s.wu f9, x6, rdn\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f0, 224(x20)\n"
			"flw.ps f17, 192(x20)\n"
			"fmax.ps f26, f0, f17\n" 
			"la x6, rand_int_32\n"
			"flw.ps f31, 128(x6)\n"
			"flw.ps f5, 704(x6)\n"
			"fadd.pi f17, f31, f5\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f23, 448(x26)\n"
			"flw.ps f19, 32(x26)\n"
			"fsgnjn.s f30, f23, f19\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_1_RDX_ID_1_SHIRE_0_HID_2:\n"
			"li x6, 0x300000000738000b\n"
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
			"LBL_SEQID_1_TLD_XMA_STR_SEQ_ITER_0_HID_2:\n"
			"la x31, self_check_16\n"
			"li x5, 0xe000000000000b\n"
			"add x5, x5, x31\n"
			"li x6, 0x3c000000000000e\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x3 << TENSOR_FMA16_B_NUM_COLS) | (0xb << TENSOR_FMA16_A_NUM_ROWS) | (0xe << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1e << TENSOR_FMA16_SCP_LOC_B) | (0x7 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x18 << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0xb << TENSOR_QUANT_QUANT_ROW) | (0x18 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x0 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"bne x5, x31, LBL_FAIL_HID_2\n"
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 10\n"
			"LBL_SEQID_1_TLD_XMA_STR_SEQ_ITER_1_HID_2:\n"
			"la x31, self_check_16\n"
			"li x5, 0x9\n"
			"add x5, x5, x31\n"
			"li x6, 0x360000000000008\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x0 << TENSOR_FMA16_B_NUM_COLS) | (0x9 << TENSOR_FMA16_A_NUM_ROWS) | (0x8 << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1b << TENSOR_FMA16_SCP_LOC_B) | (0x0 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x9, ((0xf << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0x9 << TENSOR_QUANT_QUANT_ROW) | (0x7 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x0 << TENSOR_QUANT_TRANSF1) | (0x2 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0x12\n"
			"bne x5, x31, LBL_FAIL_HID_2\n"
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_2\n"
			"csrw tensor_error, x0\n"
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_2\n"
			"addi x30, x30, 1\n"
			"la x26, rand_int_32\n"
			"flw.ps f27, 160(x26)\n"
			"flw.ps f17, 960(x26)\n"
			"fltu.pi f18, f27, f17\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f26, 0(x16)\n"
			"flw.ps f19, 256(x16)\n"
			"flw.ps f4, 480(x16)\n"
			"fnmadd.s f0, f26, f19, f4, rtz\n" 
			"la x19, rand_int_32\n"
			"flw.ps f0, 512(x19)\n"
			"flw.ps f7, 416(x19)\n"
			"for.pi f30, f0, f7\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f30, 384(x9)\n"
			"flw.ps f27, 352(x9)\n"
			"fmax.s f19, f30, f27\n" 
			"li x17, 0x321e59d93b2d272c\n"
			"fcvt.s.wu f14, x17, rdn\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f30, 64(x26)\n"
			"flw.ps f15, 224(x26)\n"
			"fmul.s f31, f30, f15, rne\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f4, 96(x10)\n"
			"flw.ps f12, 0(x10)\n"
			"fmadd.s f12, f4, f12, f12, rup\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f19, 960(x6)\n"
			"flw.ps f31, 320(x6)\n"
			"fsgnj.ps f19, f19, f31\n" 
			"la x17, rand_int_32\n"
			"flw.ps f9, 896(x17)\n"
			"fnot.pi f30, f9\n" 
			"LBL_SEQID_2_M0_WRITE_ID_4_HID_2:\n"
			"fltm.ps m3, f12, f7\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x11, rand_int_32\n"
			"flw.ps f14, 960(x11)\n"
			"fsat8.pi f5, f14\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f31, 160(x17)\n"
			"fclass.ps f14, f31\n" 
			"LBL_SEQID_2_FP_TRANS_ID_5_HID_2:\n"
			"la x28, rand_ieee754_32\n"
			"flw.ps f30, 64(x28)\n"
			"fexp.ps f15, f30\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f27, 448(x16)\n"
			"fswizz.ps f23, f27, 0x11\n" 
			"LBL_SEQID_2_M0_WRITE_ID_6_HID_2:\n"
			"fltm.pi m7, f15, f23\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_2_M0_WRITE_ID_7_HID_2:\n"
			"fltm.pi m3, f19, f18\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_2_XMA_QNT_STR_SEQ_ITER_0_HID_2:\n"
			"li x6, ((0x19 << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0xe << TENSOR_QUANT_QUANT_ROW) | (0x9 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x6 << TENSOR_QUANT_TRANSF8) | (0x6 << TENSOR_QUANT_TRANSF7) | (0x6 << TENSOR_QUANT_TRANSF6) | (0x6 << TENSOR_QUANT_TRANSF5) | (0x6 << TENSOR_QUANT_TRANSF4) | (0x6 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"LBL_SEQID_3_M0_WRITE_ID_8_HID_2:\n"
			"mov.m.x m7, x28, 0x1\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0x486840a35\n"
			"fsw.ps f0, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x6, rand_int_32\n"
			"flw.ps f27, 576(x6)\n"
			"flw.ps f23, 128(x6)\n"
			"fle.pi f12, f27, f23\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f4, 800(x17)\n"
			"flw.ps f23, 672(x17)\n"
			"fmul.ps f18, f4, f23, rne\n" 
			"li x5, 0xc900\n"
			"csrw tensor_mask, x5\n"	
			"LBL_SEQID_3_M0_WRITE_ID_9_HID_2:\n"
			"maskxor m3, m7, m5\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x17, rand_ieee754_32\n"
			"flw.ps f23, 608(x17)\n"
			"flt.s x10, f23, f23\n" 
			"la x10, rand_int_32\n"
			"flw.ps f12, 128(x10)\n"
			"flw.ps f30, 800(x10)\n"
			"fadd.pi f27, f12, f30\n" 
			"la x28, rand_int_32\n"
			"flw.ps f23, 992(x28)\n"
			"flw.ps f0, 64(x28)\n"
			"fmin.pi f0, f23, f0\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f5, 544(x11)\n"
			"flw.ps f23, 992(x11)\n"
			"flw.ps f26, 576(x11)\n"
			"fmsub.ps f15, f5, f23, f26, rtz\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f12, 320(x10)\n"
			"flw.ps f31, 704(x10)\n"
			"fsgnjx.ps f9, f12, f31\n" 
			"la x17, rand_int_32\n"
			"flw.ps f31, 640(x17)\n"
			"flw.ps f30, 224(x17)\n"
			"feq.pi f23, f31, f30\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f18, 864(x19)\n"
			"flw.ps f14, 896(x19)\n"
			"flw.ps f17, 384(x19)\n"
			"fmadd.s f26, f18, f14, f17, rup\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f14, 640(x19)\n"
			"flw.ps f12, 352(x19)\n"
			"fmin.s f23, f14, f12\n" 
			"la x9, rand_int_32\n"
			"flw.ps f9, 0(x9)\n"
			"flw.ps f7, 960(x9)\n"
			"fltu.pi f17, f9, f7\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f17, 352(x20)\n"
			"flw.ps f15, 128(x20)\n"
			"feq.s x26, f17, f15\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f26, 352(x10)\n"
			"flw.ps f15, 960(x10)\n"
			"fsgnjx.s f26, f26, f15\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f18, 704(x10)\n"
			"flw.ps f30, 384(x10)\n"
			"flw.ps f17, 256(x10)\n"
			"fnmadd.s f31, f18, f30, f17, rne\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_3_TLD_XMA_STR_SEQ_ITER_0_HID_2:\n"
			"la x31, self_check_8\n"
			"li x5, 0x6000000000000f\n"
			"add x5, x5, x31\n"
			"li x6, 0x10000000000009\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x3 << TENSOR_IMA_B_NUM_COLS) | (0xf << TENSOR_IMA_A_NUM_ROWS) | (0x9 << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x1 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1e << TENSOR_IMA_SCP_LOC_B) | (0x3 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x9, ((0xe << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0xf << TENSOR_QUANT_QUANT_ROW) | (0x15 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x1 << TENSOR_QUANT_TRANSF1) | (0xa << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fmvz.x.ps x31, f0, 0\n"
			"li x5, 0x28\n"
			"bne x5, x31, LBL_FAIL_HID_2\n"
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 10\n"
			"LBL_SEQID_3_TLD_XMA_STR_SEQ_ITER_1_HID_2:\n"
			"la x31, self_check_8\n"
			"li x5, 0xe0000000000000\n"
			"add x5, x5, x31\n"
			"li x6, 0x10000000000006\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x1 << TENSOR_IMA_B_NUM_COLS) | (0x0 << TENSOR_IMA_A_NUM_ROWS) | (0x6 << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x1 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1f << TENSOR_IMA_SCP_LOC_B) | (0x7 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x9, ((0xc << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0x0 << TENSOR_QUANT_QUANT_ROW) | (0xa << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x1 << TENSOR_QUANT_TRANSF3) | (0x3 << TENSOR_QUANT_TRANSF2) | (0xa << TENSOR_QUANT_TRANSF1) | (0xa << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fmvz.x.ps x31, f0, 0\n"
			"li x5, 0x1c\n"
			"bne x5, x31, LBL_FAIL_HID_2\n"
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 10\n"
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
			"la x9, rand_ieee754_32\n"
			"flw.ps f15, 704(x9)\n"
			"fround.ps f31, f15, rmm\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f12, 544(x20)\n"
			"flw.ps f19, 384(x20)\n"
			"flw.ps f17, 576(x20)\n"
			"fcmov.ps f15, f12, f19, f17\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f9, 192(x19)\n"
			"flw.ps f19, 640(x19)\n"
			"fmin.ps f12, f9, f19\n" 
			"li x20, 0xbaa5cab2d71c85d8\n"
			"fcvt.s.wu f4, x20, rup\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f4, 480(x17)\n"
			"flw.ps f19, 256(x17)\n"
			"flt.s x19, f4, f19\n" 
			"la x6, rand_int_32\n"
			"flw.ps f4, 608(x6)\n"
			"fandi.pi f15, f4, 0x61\n" 
			"la x11, rand_int_32\n"
			"flw.ps f27, 32(x11)\n"
			"flw.ps f14, 608(x11)\n"
			"fminu.pi f15, f27, f14\n" 
			"LBL_SEQID_4_FP_TRANS_ID_10_HID_2:\n"
			"la x26, rand_ieee754_32\n"
			"flw.ps f5, 928(x26)\n"
			"frcp.ps f14, f5\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f31, 608(x6)\n"
			"fle.s x28, f31, f31\n" 
			"LBL_SEQID_4_M0_WRITE_ID_11_HID_2:\n"
			"maskor m7, m1, m1\n" 
			"li x5, 0x80340150\n"
			"ld x5, 0(x5)\n"
			"LBL_SEQID_4_FP_TRANS_ID_12_HID_2:\n"
			"la x16, rand_ieee754_32\n"
			"flw.ps f4, 640(x16)\n"
			"flog.ps f5, f4\n" 
			"la x28, rand_int_32\n"
			"flw.ps f7, 288(x28)\n"
			"fsrli.pi f9, f7, 0x8\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x16, rand_ieee754_32\n"
			"flw.ps f18, 576(x16)\n"
			"flw.ps f31, 192(x16)\n"
			"fsgnj.ps f7, f18, f31\n" 
			"li x16, 0x6aadf5b1f33f4d7f\n"
			"fcvt.s.w f0, x16, rmm\n" 
			"maskpopcz x20, m5\n" 
			"la x17, rand_int_32\n"
			"flw.ps f12, 352(x17)\n"
			"flw.ps f17, 832(x17)\n"
			"fdiv.pi f15, f12, f17\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_4_XMA_QNT_STR_SEQ_ID_214548_HID_2:\n"
			"li x6, ((0x6 << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0x9 << TENSOR_QUANT_QUANT_ROW) | (0x23 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x3 << TENSOR_QUANT_TRANSF7) | (0x9 << TENSOR_QUANT_TRANSF6) | (0x3 << TENSOR_QUANT_TRANSF5) | (0x9 << TENSOR_QUANT_TRANSF4) | (0x9 << TENSOR_QUANT_TRANSF3) | (0x5 << TENSOR_QUANT_TRANSF2) | (0x3 << TENSOR_QUANT_TRANSF1) | (0x5 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_2\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_4_XMA_QNT_STR_SEQ_ITER_0_HID_2:\n"
			"li x6, ((0x1c << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0xf << TENSOR_QUANT_QUANT_ROW) | (0x3d << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x5 << TENSOR_QUANT_TRANSF5) | (0x3 << TENSOR_QUANT_TRANSF4) | (0xa << TENSOR_QUANT_TRANSF3) | (0x3 << TENSOR_QUANT_TRANSF2) | (0x5 << TENSOR_QUANT_TRANSF1) | (0x3 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_4_XMA_QNT_STR_SEQ_ITER_1_HID_2:\n"
			"li x6, ((0x7 << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0xf << TENSOR_QUANT_QUANT_ROW) | (0x3d << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x1 << TENSOR_QUANT_TRANSF5) | (0x5 << TENSOR_QUANT_TRANSF4) | (0xa << TENSOR_QUANT_TRANSF3) | (0x8 << TENSOR_QUANT_TRANSF2) | (0x8 << TENSOR_QUANT_TRANSF1) | (0xa << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_4_XMA_QNT_STR_SEQ_ITER_2_HID_2:\n"
			"li x6, ((0x16 << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0x6 << TENSOR_QUANT_QUANT_ROW) | (0x14 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x5 << TENSOR_QUANT_TRANSF5) | (0x5 << TENSOR_QUANT_TRANSF4) | (0x4 << TENSOR_QUANT_TRANSF3) | (0x8 << TENSOR_QUANT_TRANSF2) | (0x4 << TENSOR_QUANT_TRANSF1) | (0x4 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 10\n"
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
			"la x10, rand_ieee754_32\n"
			"flw.ps f14, 800(x10)\n"
			"fround.ps f31, f14, rup\n" 
			"la x9, rand_int_32\n"
			"flw.ps f19, 576(x9)\n"
			"fnot.pi f27, f19\n" 
			"li x16, 0x95111e63f07a1c02\n"
			"fcvt.s.w f19, x16, rup\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f23, 32(x10)\n"
			"flw.ps f27, 288(x10)\n"
			"fmin.s f15, f23, f27\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f23, 992(x20)\n"
			"fcvt.pwu.ps f14, f23, rmm\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f0, 576(x6)\n"
			"flw.ps f27, 64(x6)\n"
			"fsub.s f17, f0, f27, rtz\n" 
			"la x6, rand_int_32\n"
			"flw.ps f31, 192(x6)\n"
			"flw.ps f0, 160(x6)\n"
			"fle.pi f5, f31, f0\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f12, 608(x10)\n"
			"fswizz.ps f14, f12, 0x4\n" 
			"la x20, rand_int_32\n"
			"flw.ps f12, 928(x20)\n"
			"flw.ps f0, 960(x20)\n"
			"fsrl.pi f17, f12, f0\n" 
			"la x20, rand_int_32\n"
			"flw.ps f15, 800(x20)\n"
			"flw.ps f27, 384(x20)\n"
			"fmaxu.pi f0, f15, f27\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f4, 32(x9)\n"
			"fsgnj.s f0, f4, f4\n" 
			"la x16, rand_int_32\n"
			"flw.ps f27, 224(x16)\n"
			"flw.ps f5, 736(x16)\n"
			"for.pi f18, f27, f5\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f9, 0(x10)\n"
			"flw.ps f14, 256(x10)\n"
			"flw.ps f4, 896(x10)\n"
			"fmadd.s f5, f9, f14, f4, rdn\n" 
			"la x10, rand_int_32\n"
			"flw.ps f12, 0(x10)\n"
			"flw.ps f30, 544(x10)\n"
			"fsll.pi f4, f12, f30\n" 
			"la x19, rand_ieee754_16\n"
			"flw.ps f9, 160(x19)\n"
			"fcvt.ps.f16 f5, f9\n" 
			"la x17, rand_int_32\n"
			"flw.ps f12, 576(x17)\n"
			"fmul.pi f9, f12, f12\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 0xb7d5\n"
			"csrw tensor_mask, x5\n"	
			"LBL_SEQID_5_XMA_QNT_STR_SEQ_ITER_0_HID_2:\n"
			"li x6, ((0x7 << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0xf << TENSOR_QUANT_QUANT_ROW) | (0x23 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_2\n"
			"csrw tensor_error, x0\n"
			"li x5, 6\n" 
			"bne x30, x5, LBL_FAIL_HID_2\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_6_M0_WRITE_ID_13_HID_2:\n"
			"maskand m1, m7, m1\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x9, rand_ieee754_32\n"
			"flw.ps f14, 896(x9)\n"
			"flw.ps f0, 928(x9)\n"
			"feq.s x9, f14, f0\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f14, 544(x28)\n"
			"fcvt.pwu.ps f23, f14, rmm\n" 
			"la x10, rand_int_32\n"
			"flw.ps f27, 832(x10)\n"
			"fandi.pi f14, f27, 0x1e0\n" 
			"la x17, rand_int_32\n"
			"flw.ps f15, 704(x17)\n"
			"flw.ps f5, 576(x17)\n"
			"fxor.pi f15, f15, f5\n" 
			"la x28, rand_int_32\n"
			"flw.ps f23, 672(x28)\n"
			"flw.ps f31, 960(x28)\n"
			"fmulhu.pi f31, f23, f31\n" 
			"LBL_SEQID_6_M0_WRITE_ID_14_HID_2:\n"
			"mova.m.x x20\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x19, rand_ieee754_32\n"
			"flw.ps f27, 0(x19)\n"
			"flw.ps f23, 544(x19)\n"
			"fmax.ps f31, f27, f23\n" 
			"la x10, rand_int_32\n"
			"flw.ps f26, 576(x10)\n"
			"flw.ps f14, 992(x10)\n"
			"fsub.pi f27, f26, f14\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x11, rand_ieee754_32\n"
			"flw.ps f14, 672(x11)\n"
			"frsq.ps f12, f14\n" 
			"la x9, rand_int_32\n"
			"flw.ps f26, 864(x9)\n"
			"fsrai.pi f9, f26, 0x7\n" 
			"LBL_SEQID_6_M0_WRITE_ID_15_HID_2:\n"
			"maskor m1, m1, m5\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0xff0616698\n"
			"flw.ps f5, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x20, rand_ieee754_32\n"
			"flw.ps f18, 768(x20)\n"
			"fswizz.ps f26, f18, 0x1f\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f23, 928(x16)\n"
			"flw.ps f5, 928(x16)\n"
			"fmin.ps f27, f23, f5\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f5, 576(x9)\n"
			"flw.ps f30, 320(x9)\n"
			"fsgnj.s f0, f5, f30\n" 
			"la x10, rand_int_32\n"
			"flw.ps f30, 928(x10)\n"
			"fnot.pi f26, f30\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_6_XMA_QNT_STR_SEQ_ID_489234_HID_2:\n"
			"li x6, ((0x17 << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0x0 << TENSOR_QUANT_QUANT_ROW) | (0x2f << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x2 << TENSOR_QUANT_TRANSF5) | (0x6 << TENSOR_QUANT_TRANSF4) | (0x7 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_2\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_6_TLD_XMA_STR_SEQ_ITER_0_HID_2:\n"
			"la x31, self_check_16\n"
			"li x5, 0xa000000000000e\n"
			"add x5, x5, x31\n"
			"li x6, 0x10000000000007\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x2 << TENSOR_FMA16_B_NUM_COLS) | (0xe << TENSOR_FMA16_A_NUM_ROWS) | (0x7 << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x1 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x18 << TENSOR_FMA16_SCP_LOC_B) | (0x5 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x1b << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0xe << TENSOR_QUANT_QUANT_ROW) | (0x31 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x7 << TENSOR_QUANT_TRANSF8) | (0x7 << TENSOR_QUANT_TRANSF7) | (0x7 << TENSOR_QUANT_TRANSF6) | (0x7 << TENSOR_QUANT_TRANSF5) | (0x7 << TENSOR_QUANT_TRANSF4) | (0x6 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0x10\n"
			"bne x5, x31, LBL_FAIL_HID_2\n"
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 10\n"
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
			"csrwi mhpmevent3, 2\n"   
			"csrwi mhpmevent4, 1\n"   
			"csrwi mhpmevent5, 3\n"   
			"csrwi mhpmevent6, 6\n"   
			"csrwi mhpmevent7, 22\n"   
			"csrwi mhpmevent8, 2\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f21,  736 (x5)\n"
			"flw.ps f30,  352 (x5)\n"
			"flw.ps f8,  64 (x5)\n"
			"flw.ps f19,  256 (x5)\n"
			"flw.ps f16,  800 (x5)\n"
			"flw.ps f5,  896 (x5)\n"
			"flw.ps f12,  320 (x5)\n"
			"flw.ps f18,  448 (x5)\n"
			"flw.ps f7,  416 (x5)\n"
			"flw.ps f6,  960 (x5)\n"
			"flw.ps f0,  544 (x5)\n"
			"flw.ps f3,  768 (x5)\n"
			"flw.ps f15,  224 (x5)\n"
			"flw.ps f31,  928 (x5)\n"
			"flw.ps f2,  96 (x5)\n"
			"flw.ps f26,  608 (x5)\n"
			"flw.ps f10,  864 (x5)\n"
			"flw.ps f24,  192 (x5)\n"
			"flw.ps f1,  384 (x5)\n"
			"flw.ps f29,  992 (x5)\n"
			"flw.ps f28,  32 (x5)\n"
			"flw.ps f20,  288 (x5)\n"
			"flw.ps f25,  128 (x5)\n"
			"flw.ps f17,  480 (x5)\n"
			"flw.ps f4,  576 (x5)\n"
			"flw.ps f14,  672 (x5)\n"
			"flw.ps f13,  640 (x5)\n"
			"flw.ps f23,  704 (x5)\n"
			"flw.ps f11,  160 (x5)\n"
			"flw.ps f27,  0 (x5)\n"
			"flw.ps f9,  512 (x5)\n"
			"flw.ps f22,  832 (x5)\n"
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
			"la x18, rand_ieee754_32\n"
			"flw.ps f27, 160(x18)\n"
			"flw.ps f22, 64(x18)\n"
			"fsgnjn.s f17, f27, f22\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f26, 128(x19)\n"
			"frsq.ps f26, f26\n" 
			"la x16, rand_int_32\n"
			"flw.ps f22, 864(x16)\n"
			"fpackrepb.pi f14, f22\n" 
			"li x5, 0xd6e133d5243e5891\n"
			"fcvt.s.wu f18, x5, rup\n" 
			"fbci.pi f2, 0x43886\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_3:\n"
			"mov.m.x m6, x11, 0x78\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x11, rand_ieee754_32\n"
			"flw.ps f20, 320(x11)\n"
			"flw.ps f2, 160(x11)\n"
			"fnmsub.s f27, f20, f2, f20, rdn\n" 
			"la x5, rand_int_32\n"
			"flw.ps f22, 864(x5)\n"
			"flw.ps f2, 736(x5)\n"
			"feq.pi f26, f22, f2\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f11, 736(x11)\n"
			"flw.ps f31, 480(x11)\n"
			"fadd.s f23, f11, f31, rdn\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f1, 448(x28)\n"
			"fcvt.wu.s x17, f1, rne\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f27, 96(x16)\n"
			"fclass.s x17, f27\n" 
			"la x17, rand_int_32\n"
			"flw.ps f27, 320(x17)\n"
			"flw.ps f14, 64(x17)\n"
			"fmin.pi f25, f27, f14\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f2, 992(x28)\n"
			"flw.ps f20, 960(x28)\n"
			"fsgnjx.s f18, f2, f20\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f11, 800(x19)\n"
			"flw.ps f22, 704(x19)\n"
			"fsgnjn.ps f20, f11, f22\n" 
			"maskpopcz x11, m0\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f20, 0(x20)\n"
			"flw.ps f25, 160(x20)\n"
			"flw.ps f1, 192(x20)\n"
			"fmsub.s f17, f20, f25, f1, rmm\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_3\n"
			"addi x30, x30, 1\n"
			"la x20, rand_ieee754_32\n"
			"flw.ps f26, 768(x20)\n"
			"flw.ps f21, 64(x20)\n"
			"flw.ps f14, 0(x20)\n"
			"fmsub.ps f8, f26, f21, f14, rup\n" 
			"la x17, rand_int_32\n"
			"flw.ps f11, 928(x17)\n"
			"flw.ps f22, 608(x17)\n"
			"fsub.pi f17, f11, f22\n" 
			"LBL_SEQID_1_M0_WRITE_ID_1_HID_3:\n"
			"maskor m0, m1, m5\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x19, rand_ieee754_32\n"
			"flw.ps f31, 608(x19)\n"
			"flw.ps f8, 288(x19)\n"
			"feq.ps f11, f31, f8\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f22, 256(x19)\n"
			"fle.ps f23, f22, f22\n" 
			"li x5, 0xf6b22c5494a4dd4d\n"
			"fcvt.s.wu f14, x5, rup\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f14, 160(x5)\n"
			"flw.ps f8, 160(x5)\n"
			"flt.s x16, f14, f8\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f23, 736(x7)\n"
			"flw.ps f8, 736(x7)\n"
			"fsgnj.ps f18, f23, f8\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f8, 992(x19)\n"
			"flw.ps f23, 608(x19)\n"
			"fsgnjx.ps f23, f8, f23\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f25, 736(x11)\n"
			"flw.ps f8, 800(x11)\n"
			"fadd.s f17, f25, f8, rmm\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f0, 32(x28)\n"
			"fsin.ps f0, f0\n" 
			"la x18, rand_ieee754_16\n"
			"flw.ps f21, 576(x18)\n"
			"fcvt.ps.f16 f11, f21\n" 
			"la x18, rand_int_32\n"
			"flw.ps f20, 64(x18)\n"
			"flw.ps f27, 96(x18)\n"
			"fand.pi f27, f20, f27\n" 
			"LBL_SEQID_1_FP_TRANS_ID_2_HID_3:\n"
			"la x18, rand_ieee754_32\n"
			"flw.ps f14, 704(x18)\n"
			"frcp.ps f2, f14\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f0, 448(x16)\n"
			"flw.ps f17, 448(x16)\n"
			"fmul.s f26, f0, f17, rmm\n" 
			"LBL_SEQID_1_M0_WRITE_ID_3_HID_3:\n"
			"masknot m1, m5\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"csrw tensor_error, zero\n" 
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_3\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_2_M0_WRITE_ID_4_HID_3:\n"
			"flem.ps m6, f22, f21\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"maskpopc x7, m6\n" 
			"LBL_SEQID_2_M0_WRITE_ID_5_HID_3:\n"
			"maskor m5, m5, m1\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x7, rand_int_32\n"
			"flw.ps f22, 448(x7)\n"
			"fpackrepb.pi f18, f22\n" 
			"la x19, rand_int_32\n"
			"flw.ps f20, 64(x19)\n"
			"flw.ps f1, 896(x19)\n"
			"fmaxu.pi f27, f20, f1\n" 
			"LBL_SEQID_2_M0_WRITE_ID_6_HID_3:\n"
			"mova.m.x x7\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x11, rand_ieee754_32\n"
			"flw.ps f2, 704(x11)\n"
			"flw.ps f1, 224(x11)\n"
			"flw.ps f27, 64(x11)\n"
			"fnmadd.s f27, f2, f1, f27, rmm\n" 
			"la x18, rand_int_32\n"
			"flw.ps f8, 384(x18)\n"
			"fsrai.pi f31, f8, 0xa\n" 
			"LBL_SEQID_2_FP_TRANS_ID_7_HID_3:\n"
			"la x23, rand_ieee754_32\n"
			"flw.ps f14, 896(x23)\n"
			"flog.ps f25, f14\n" 
			"LBL_SEQID_2_M0_WRITE_ID_8_HID_3:\n"
			"fsetm.pi m0, f26\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0xFF\n"
			"maskpopcz x5, m6\n" 
			"LBL_SEQID_2_M0_WRITE_ID_9_HID_3:\n"
			"mov.m.x m5, x23, 0x49\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"fbci.ps f8, 0x43a8c\n" 
			"la x28, rand_int_32\n"
			"flw.ps f26, 576(x28)\n"
			"flw.ps f1, 480(x28)\n"
			"fdiv.pi f22, f26, f1\n" 
			"mova.x.m x19\n" 
			"la x28, rand_int_32\n"
			"flw.ps f25, 512(x28)\n"
			"flw.ps f22, 0(x28)\n"
			"fltu.pi f31, f25, f22\n" 
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
			"la x18, rand_int_32\n"
			"flw.ps f17, 384(x18)\n"
			"flw.ps f20, 480(x18)\n"
			"fdiv.pi f0, f17, f20\n" 
			"la x18, rand_int_32\n"
			"flw.ps f0, 576(x18)\n"
			"fsrai.pi f14, f0, 0xc\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f18, 800(x18)\n"
			"fsqrt.s f11, f18, rtz\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f26, 608(x19)\n"
			"fclass.s x17, f26\n" 
			"li x16, 0xc015ddec280b14c0\n"
			"fcvt.s.w f27, x16, rtz\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f18, 608(x23)\n"
			"flw.ps f20, 320(x23)\n"
			"fsgnjx.ps f8, f18, f20\n" 
			"la x17, rand_int_32\n"
			"flw.ps f21, 416(x17)\n"
			"flw.ps f20, 640(x17)\n"
			"frem.pi f22, f21, f20\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f2, 160(x5)\n"
			"fmin.s f20, f2, f2\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f2, 384(x18)\n"
			"flw.ps f8, 768(x18)\n"
			"fcmovm.ps f21, f2, f8\n" 
			"la x28, rand_int_32\n"
			"flw.ps f17, 800(x28)\n"
			"flw.ps f21, 384(x28)\n"
			"fsll.pi f8, f17, f21\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f21, 288(x19)\n"
			"flw.ps f18, 288(x19)\n"
			"flt.ps f21, f21, f18\n" 
			"la x23, rand_int_32\n"
			"flw.ps f17, 832(x23)\n"
			"flw.ps f26, 576(x23)\n"
			"fsll.pi f20, f17, f26\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f2, 704(x23)\n"
			"flw.ps f22, 832(x23)\n"
			"flw.ps f21, 832(x23)\n"
			"fnmadd.ps f14, f2, f22, f21, rdn\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f1, 800(x28)\n"
			"fcvt.l.s x7, f1, rtz\n" 
			"la x11, rand_int_32\n"
			"flw.ps f23, 736(x11)\n"
			"flw.ps f18, 384(x11)\n"
			"fmul.pi f8, f23, f18\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f0, 736(x28)\n"
			"flw.ps f18, 800(x28)\n"
			"flw.ps f1, 224(x28)\n"
			"fcmov.ps f21, f0, f18, f1\n" 
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
			"la x17, rand_ieee754_32\n"
			"flw.ps f17, 416(x17)\n"
			"flw.ps f31, 256(x17)\n"
			"flw.ps f18, 512(x17)\n"
			"fmsub.ps f31, f17, f31, f18, rtz\n" 
			"la x11, rand_int_32\n"
			"flw.ps f25, 960(x11)\n"
			"feq.pi f27, f25, f25\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f8, 544(x17)\n"
			"flw.ps f25, 320(x17)\n"
			"flw.ps f20, 288(x17)\n"
			"fnmadd.s f1, f8, f25, f20, rtz\n" 
			"la x16, rand_int_32\n"
			"flw.ps f1, 480(x16)\n"
			"flw.ps f11, 352(x16)\n"
			"fmulh.pi f2, f1, f11\n" 
			"maskpopcz x7, m1\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f26, 448(x28)\n"
			"fcvt.wu.s x28, f26, rne\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f27, 128(x18)\n"
			"fclass.ps f1, f27\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f0, 768(x23)\n"
			"flw.ps f17, 992(x23)\n"
			"feq.ps f20, f0, f17\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f11, 480(x28)\n"
			"flw.ps f18, 448(x28)\n"
			"fmax.s f20, f11, f18\n" 
			"LBL_SEQID_4_M0_WRITE_ID_10_HID_3:\n"
			"flem.ps m6, f2, f23\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x5, rand_ieee754_32\n"
			"flw.ps f26, 544(x5)\n"
			"flw.ps f31, 32(x5)\n"
			"fmax.s f18, f26, f31\n" 
			"la x23, rand_int_32\n"
			"flw.ps f0, 96(x23)\n"
			"fandi.pi f1, f0, 0x154\n" 
			"la x18, rand_ieee754_16\n"
			"flw.ps f23, 384(x18)\n"
			"fcvt.ps.f16 f20, f23\n" 
			"la x16, rand_int_32\n"
			"flw.ps f25, 384(x16)\n"
			"fsrai.pi f20, f25, 0xb\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f8, 96(x5)\n"
			"fsin.ps f8, f8\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f8, 768(x17)\n"
			"flw.ps f11, 352(x17)\n"
			"flt.ps f23, f8, f11\n" 
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
			"la x28, rand_ieee754_32\n"
			"flw.ps f21, 448(x28)\n"
			"fcvt.w.s x16, f21, rdn\n" 
			"la x19, rand_int_32\n"
			"flw.ps f14, 960(x19)\n"
			"flw.ps f20, 832(x19)\n"
			"fmin.pi f21, f14, f20\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f25, 608(x23)\n"
			"flw.ps f31, 736(x23)\n"
			"fsgnj.s f31, f25, f31\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f27, 448(x28)\n"
			"flw.ps f21, 352(x28)\n"
			"fsub.ps f21, f27, f21, rup\n" 
			"la x28, rand_int_32\n"
			"flw.ps f31, 800(x28)\n"
			"flw.ps f1, 0(x28)\n"
			"fsub.pi f0, f31, f1\n" 
			"la x11, rand_int_32\n"
			"flw.ps f18, 608(x11)\n"
			"fandi.pi f18, f18, 0x12f\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f23, 384(x11)\n"
			"fadd.s f2, f23, f23, rne\n" 
			"LBL_SEQID_5_FP_TRANS_ID_11_HID_3:\n"
			"la x11, rand_ieee754_32\n"
			"flw.ps f21, 32(x11)\n"
			"flog.ps f26, f21\n" 
			"LBL_SEQID_5_FP_TRANS_ID_12_HID_3:\n"
			"la x5, rand_ieee754_32\n"
			"flw.ps f22, 160(x5)\n"
			"flog.ps f11, f22\n" 
			"LBL_SEQID_5_M0_WRITE_ID_13_HID_3:\n"
			"fltm.ps m0, f18, f27\n" 
			"li x5, CREDINC2\n"
			"ld x5, 0(x5)\n"
			"li x20, 0x61e762d024d3061\n"
			"fcvt.s.w f18, x20, rup\n" 
			"la x7, rand_int_32\n"
			"flw.ps f17, 128(x7)\n"
			"faddi.pi f25, f17, 0x47\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f8, 672(x23)\n"
			"fcvt.w.s x20, f8, rup\n" 
			"la x23, rand_int_32\n"
			"flw.ps f25, 544(x23)\n"
			"flw.ps f1, 576(x23)\n"
			"fdivu.pi f21, f25, f1\n" 
			"la x28, rand_int_32\n"
			"flw.ps f17, 896(x28)\n"
			"fslli.pi f18, f17, 0x9\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f1, 960(x7)\n"
			"fcvt.pwu.ps f14, f1, rtz\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 6\n" 
			"bne x30, x5, LBL_FAIL_HID_3\n"
			"addi x30, x30, 1\n"
			"la x19, rand_int_32\n"
			"flw.ps f11, 128(x19)\n"
			"flw.ps f17, 96(x19)\n"
			"fmulhu.pi f14, f11, f17\n" 
			"li x16, 0xcddde5b60e8a0fd4\n"
			"fcvt.s.w f20, x16, rdn\n" 
			"la x11, rand_int_32\n"
			"flw.ps f8, 960(x11)\n"
			"fpackrepb.pi f14, f8\n" 
			"la x5, rand_int_32\n"
			"flw.ps f25, 960(x5)\n"
			"flw.ps f27, 864(x5)\n"
			"fle.pi f26, f25, f27\n" 
			"LBL_SEQID_6_M0_WRITE_ID_14_HID_3:\n"
			"mov.m.x m1, x23, 0x7d\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_6_M0_WRITE_ID_15_HID_3:\n"
			"fltm.pi m1, f18, f23\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0x11e0eaafee\n"
			"fsw.ps f0, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x7, rand_ieee754_32\n"
			"flw.ps f11, 864(x7)\n"
			"flw.ps f25, 416(x7)\n"
			"flw.ps f14, 768(x7)\n"
			"fcmov.ps f8, f11, f25, f14\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f0, 928(x23)\n"
			"flw.ps f31, 800(x23)\n"
			"flw.ps f11, 64(x23)\n"
			"fnmsub.s f11, f0, f31, f11, rup\n" 
			"la x19, rand_ieee754_16\n"
			"flw.ps f2, 128(x19)\n"
			"fcvt.ps.f16 f23, f2\n" 
			"fbci.ps f0, 0xc9ef\n" 
			"la x20, rand_int_32\n"
			"flw.ps f17, 320(x20)\n"
			"flw.ps f11, 128(x20)\n"
			"fadd.pi f20, f17, f11\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f1, 800(x23)\n"
			"fround.ps f18, f1, rup\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f21, 992(x19)\n"
			"flw.ps f8, 992(x19)\n"
			"fsgnjn.s f21, f21, f8\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f2, 832(x28)\n"
			"fle.ps f2, f2, f2\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f8, 928(x28)\n"
			"flw.ps f17, 512(x28)\n"
			"flw.ps f23, 832(x28)\n"
			"fmsub.s f1, f8, f17, f23, rdn\n" 
			"LBL_SEQID_6_M0_WRITE_ID_16_HID_3:\n"
			"mova.m.x x18\n" 
			"li x5, CREDINC3\n"
			"ld x5, 0(x5)\n"
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
			"csrwi mhpmevent3, 8\n"   
			"csrwi mhpmevent4, 14\n"   
			"csrwi mhpmevent5, 4\n"   
			"csrwi mhpmevent6, 17\n"   
			"csrwi mhpmevent7, 6\n"   
			"csrwi mhpmevent8, 3\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f30,  704 (x5)\n"
			"flw.ps f1,  256 (x5)\n"
			"flw.ps f6,  736 (x5)\n"
			"flw.ps f5,  480 (x5)\n"
			"flw.ps f28,  64 (x5)\n"
			"flw.ps f16,  672 (x5)\n"
			"flw.ps f23,  288 (x5)\n"
			"flw.ps f14,  800 (x5)\n"
			"flw.ps f11,  320 (x5)\n"
			"flw.ps f27,  576 (x5)\n"
			"flw.ps f20,  416 (x5)\n"
			"flw.ps f15,  832 (x5)\n"
			"flw.ps f29,  32 (x5)\n"
			"flw.ps f12,  640 (x5)\n"
			"flw.ps f21,  768 (x5)\n"
			"flw.ps f0,  960 (x5)\n"
			"flw.ps f4,  352 (x5)\n"
			"flw.ps f26,  608 (x5)\n"
			"flw.ps f7,  512 (x5)\n"
			"flw.ps f3,  160 (x5)\n"
			"flw.ps f31,  928 (x5)\n"
			"flw.ps f13,  224 (x5)\n"
			"flw.ps f8,  128 (x5)\n"
			"flw.ps f22,  192 (x5)\n"
			"flw.ps f10,  0 (x5)\n"
			"flw.ps f19,  96 (x5)\n"
			"flw.ps f9,  448 (x5)\n"
			"flw.ps f2,  864 (x5)\n"
			"flw.ps f25,  544 (x5)\n"
			"flw.ps f24,  896 (x5)\n"
			"flw.ps f18,  992 (x5)\n"
			"flw.ps f17,  384 (x5)\n"
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
			"la x13, rand_ieee754_32\n"
			"flw.ps f29, 768(x13)\n"
			"flw.ps f14, 704(x13)\n"
			"flw.ps f3, 800(x13)\n"
			"fmsub.ps f4, f29, f14, f3, rup\n" 
			"la x17, rand_int_32\n"
			"flw.ps f22, 704(x17)\n"
			"flw.ps f18, 192(x17)\n"
			"fle.pi f30, f22, f18\n" 
			"LBL_SEQID_0_FP_TRANS_ID_0_HID_4:\n"
			"la x11, rand_ieee754_32\n"
			"flw.ps f13, 32(x11)\n"
			"flog.ps f18, f13\n" 
			"la x12, rand_int_32\n"
			"flw.ps f13, 320(x12)\n"
			"flw.ps f30, 704(x12)\n"
			"fsrl.pi f12, f13, f30\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_4:\n"
			"maskand m4, m6, m4\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"mova.x.m x15\n" 
			"la x15, rand_int_32\n"
			"flw.ps f29, 704(x15)\n"
			"flw.ps f13, 384(x15)\n"
			"fle.pi f18, f29, f13\n" 
			"maskpopcz x12, m6\n" 
			"li x5, 0xa002\n"
			"csrw tensor_mask, x5\n"	
			"la x28, rand_int_32\n"
			"flw.ps f4, 96(x28)\n"
			"flw.ps f3, 768(x28)\n"
			"feq.pi f3, f4, f3\n" 
			"LBL_SEQID_0_M0_WRITE_ID_2_HID_4:\n"
			"mov.m.x m2, x27, 0xc\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x27, rand_ieee754_32\n"
			"flw.ps f12, 576(x27)\n"
			"flw.ps f18, 608(x27)\n"
			"flw.ps f10, 256(x27)\n"
			"fnmsub.ps f18, f12, f18, f10, rmm\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f12, 672(x11)\n"
			"flw.ps f22, 800(x11)\n"
			"fsgnj.ps f13, f12, f22\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f3, 160(x20)\n"
			"fsin.ps f13, f3\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f12, 832(x11)\n"
			"fcvt.f16.ps f16, f12\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f0, 96(x15)\n"
			"flw.ps f10, 768(x15)\n"
			"fcmovm.ps f22, f0, f10\n" 
			"li x28, 0xa8a17a8d3acf845\n"
			"fcvt.s.l f12, x28, rmm\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_0_RDX_ID_0_SHIRE_0_HID_4:\n"
			"li x26, 0x3a0000000353000b\n"
			"csrw tensor_reduce, x26\n" 
			"csrwi tensor_wait, 9\n" 
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ID_711205_HID_4:\n"
			"li x5, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x1 << TENSOR_FMA32_B_NUM_COLS) | (0x3 << TENSOR_FMA32_A_NUM_ROWS) | (0x3 << TENSOR_FMA32_A_NUM_COLS) | (0x6 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x9c << TENSOR_FMA32_SCP_LOC_B) | (0x30 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
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
			"li x5, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x0 << TENSOR_FMA16_B_NUM_COLS) | (0x3 << TENSOR_FMA16_A_NUM_ROWS) | (0x4 << TENSOR_FMA16_A_NUM_COLS) | (0x9 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0xd3 << TENSOR_FMA16_SCP_LOC_B) | (0xa1 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x0 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x6, ((0x2 << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0x3 << TENSOR_QUANT_QUANT_ROW) | (0xc << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x2 << TENSOR_QUANT_TRANSF4) | (0x6 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 7\n"
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_4\n"
			"csrw tensor_error, x0\n"
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_4\n"
			"addi x30, x30, 1\n"
			"la x17, rand_int_32\n"
			"flw.ps f16, 192(x17)\n"
			"flw.ps f28, 576(x17)\n"
			"fmulhu.pi f22, f16, f28\n" 
			"la x19, rand_int_32\n"
			"flw.ps f25, 512(x19)\n"
			"flw.ps f30, 64(x19)\n"
			"for.pi f12, f25, f30\n" 
			"la x28, rand_int_32\n"
			"flw.ps f10, 320(x28)\n"
			"flw.ps f19, 64(x28)\n"
			"fdiv.pi f3, f10, f19\n" 
			"la x17, rand_int_32\n"
			"flw.ps f15, 512(x17)\n"
			"flw.ps f22, 512(x17)\n"
			"fmin.pi f29, f15, f22\n" 
			"la x11, rand_int_32\n"
			"flw.ps f16, 256(x11)\n"
			"flw.ps f18, 992(x11)\n"
			"fle.pi f25, f16, f18\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f15, 256(x11)\n"
			"flw.ps f12, 896(x11)\n"
			"feq.ps f30, f15, f12\n" 
			"la x17, rand_int_32\n"
			"flw.ps f29, 224(x17)\n"
			"flw.ps f14, 224(x17)\n"
			"fremu.pi f12, f29, f14\n" 
			"LBL_SEQID_1_M0_WRITE_ID_3_HID_4:\n"
			"maskand m4, m6, m6\n" 
			"li x5, 0x803401A0\n"
			"ld x5, 0(x5)\n"
			"LBL_SEQID_1_FP_TRANS_ID_4_HID_4:\n"
			"la x20, rand_ieee754_32\n"
			"flw.ps f13, 544(x20)\n"
			"flog.ps f16, f13\n" 
			"li x5, 0xf033\n"
			"csrw tensor_mask, x5\n"	
			"la x27, rand_int_32\n"
			"flw.ps f3, 320(x27)\n"
			"flw.ps f25, 768(x27)\n"
			"for.pi f22, f3, f25\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f4, 96(x17)\n"
			"flw.ps f13, 0(x17)\n"
			"flw.ps f3, 640(x17)\n"
			"fmsub.ps f29, f4, f13, f3, rtz\n" 
			"LBL_SEQID_1_M0_WRITE_ID_5_HID_4:\n"
			"flem.ps m6, f16, f29\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x20, rand_int_32\n"
			"flw.ps f4, 544(x20)\n"
			"flw.ps f18, 736(x20)\n"
			"fmaxu.pi f4, f4, f18\n" 
			"li x5, 0x280f4c1d9d0a0a9\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0x518135119e93ba46\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"la x15, rand_int_32\n"
			"flw.ps f29, 128(x15)\n"
			"fcvt.ps.pw f16, f29, rtz\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x17, rand_int_32\n"
			"flw.ps f19, 736(x17)\n"
			"fpackreph.pi f29, f19\n" 
			"LBL_SEQID_1_M0_WRITE_ID_6_HID_4:\n"
			"fltm.ps m4, f12, f29\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ID_789366_HID_4:\n"
			"li x5, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x3 << TENSOR_IMA_B_NUM_COLS) | (0xb << TENSOR_IMA_A_NUM_ROWS) | (0xa << TENSOR_IMA_A_NUM_COLS) | (0x2 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0xbb << TENSOR_IMA_SCP_LOC_B) | (0x95 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ID_285754_HID_4:\n"
			"li x5, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x1 << TENSOR_FMA32_B_NUM_COLS) | (0x9 << TENSOR_FMA32_A_NUM_ROWS) | (0x0 << TENSOR_FMA32_A_NUM_COLS) | (0x6 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x51 << TENSOR_FMA32_SCP_LOC_B) | (0x17 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ID_239591_HID_4:\n"
			"li x5, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x1 << TENSOR_FMA32_B_NUM_COLS) | (0x5 << TENSOR_FMA32_A_NUM_ROWS) | (0xe << TENSOR_FMA32_A_NUM_COLS) | (0xe << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0xfb << TENSOR_FMA32_SCP_LOC_B) | (0x2c << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x0 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x6, ((0x1a << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0x5 << TENSOR_QUANT_QUANT_ROW) | (0x20 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 7\n"
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_4\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_1_RDX_ID_1_SHIRE_0_HID_4:\n"
			"li x28, 0x300000000738000b\n"
			"csrw tensor_reduce, x28\n" 
			"csrwi tensor_wait, 9\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ID_780627_HID_4:\n"
			"li x6, ((0x7 << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0x1 << TENSOR_QUANT_QUANT_ROW) | (0x6 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x1 << TENSOR_QUANT_TRANSF3) | (0x8 << TENSOR_QUANT_TRANSF2) | (0x3 << TENSOR_QUANT_TRANSF1) | (0x8 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ID_172081_HID_4:\n"
			"li x6, ((0xb << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0x0 << TENSOR_QUANT_QUANT_ROW) | (0x3c << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
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
			"LBL_SEQID_1_TLD_XMA_STR_SEQ_ITER_0_HID_4:\n"
			"la x31, self_check_16\n"
			"li x5, 0x4000000000000f\n"
			"add x5, x5, x31\n"
			"li x6, 0x36000000000000f\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x2 << TENSOR_FMA16_B_NUM_COLS) | (0xf << TENSOR_FMA16_A_NUM_ROWS) | (0xf << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1b << TENSOR_FMA16_SCP_LOC_B) | (0x2 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0x20\n"
			"bne x5, x31, LBL_FAIL_HID_4\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_4\n"
			"csrw tensor_error, x0\n"
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_4\n"
			"addi x30, x30, 1\n"
			"la x20, rand_int_32\n"
			"flw.ps f28, 512(x20)\n"
			"fnot.pi f25, f28\n" 
			"LBL_SEQID_2_M0_WRITE_ID_7_HID_4:\n"
			"fltm.ps m5, f30, f29\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x15, rand_ieee754_32\n"
			"flw.ps f28, 224(x15)\n"
			"flw.ps f29, 672(x15)\n"
			"flw.ps f15, 576(x15)\n"
			"fnmadd.s f3, f28, f29, f15, rup\n" 
			"la x19, rand_int_32\n"
			"flw.ps f16, 96(x19)\n"
			"flw.ps f10, 0(x19)\n"
			"fmulhu.pi f18, f16, f10\n" 
			"la x20, rand_int_32\n"
			"flw.ps f29, 640(x20)\n"
			"fcvt.ps.pwu f4, f29, rne\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f4, 256(x17)\n"
			"flw.ps f28, 704(x17)\n"
			"feq.s x13, f4, f28\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f14, 192(x15)\n"
			"flw.ps f18, 832(x15)\n"
			"flw.ps f13, 416(x15)\n"
			"fmadd.s f0, f14, f18, f13, rdn\n" 
			"LBL_SEQID_2_M0_WRITE_ID_8_HID_4:\n"
			"fltm.ps m5, f13, f22\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x15, rand_ieee754_32\n"
			"flw.ps f16, 96(x15)\n"
			"frsq.ps f22, f16\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f16, 544(x12)\n"
			"flw.ps f30, 192(x12)\n"
			"fmax.s f19, f16, f30\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f28, 640(x17)\n"
			"fsub.ps f30, f28, f28, rne\n" 
			"la x27, rand_int_32\n"
			"flw.ps f13, 32(x27)\n"
			"flw.ps f12, 704(x27)\n"
			"fsrl.pi f3, f13, f12\n" 
			"la x20, rand_int_32\n"
			"flw.ps f16, 672(x20)\n"
			"flw.ps f30, 576(x20)\n"
			"fle.pi f13, f16, f30\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f4, 352(x11)\n"
			"flw.ps f25, 768(x11)\n"
			"flw.ps f29, 608(x11)\n"
			"fnmsub.ps f4, f4, f25, f29, rne\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f15, 576(x15)\n"
			"flw.ps f18, 832(x15)\n"
			"flw.ps f13, 736(x15)\n"
			"fmadd.ps f16, f15, f18, f13, rdn\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f30, 544(x13)\n"
			"flw.ps f14, 704(x13)\n"
			"fmax.s f30, f30, f14\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_2_XMA_QNT_STR_SEQ_ITER_0_HID_4:\n"
			"li x6, ((0x17 << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0xd << TENSOR_QUANT_QUANT_ROW) | (0x16 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x2 << TENSOR_QUANT_TRANSF6) | (0x6 << TENSOR_QUANT_TRANSF5) | (0x6 << TENSOR_QUANT_TRANSF4) | (0x6 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_2_XMA_QNT_STR_SEQ_ITER_1_HID_4:\n"
			"li x6, ((0xc << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0xc << TENSOR_QUANT_QUANT_ROW) | (0xc << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x6 << TENSOR_QUANT_TRANSF6) | (0x6 << TENSOR_QUANT_TRANSF5) | (0x7 << TENSOR_QUANT_TRANSF4) | (0x7 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 10\n"
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
			"la x19, rand_ieee754_32\n"
			"flw.ps f19, 960(x19)\n"
			"flw.ps f14, 352(x19)\n"
			"fsub.ps f4, f19, f14, rmm\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f3, 736(x15)\n"
			"fclass.s x27, f3\n" 
			"la x15, rand_int_32\n"
			"flw.ps f10, 864(x15)\n"
			"fsatu8.pi f13, f10\n" 
			"LBL_SEQID_3_M0_WRITE_ID_9_HID_4:\n"
			"fsetm.pi m2, f22\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x28, rand_ieee754_32\n"
			"flw.ps f15, 704(x28)\n"
			"flw.ps f19, 768(x28)\n"
			"flt.ps f4, f15, f19\n" 
			"li x5, 0xb6c5\n"
			"csrw tensor_mask, x5\n"	
			"fbci.pi f18, 0xb88e\n" 
			"la x15, rand_ieee754_16\n"
			"flw.ps f29, 864(x15)\n"
			"fcvt.ps.f16 f19, f29\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f19, 672(x28)\n"
			"flw.ps f10, 0(x28)\n"
			"fsgnjn.s f13, f19, f10\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f13, 256(x13)\n"
			"flw.ps f25, 928(x13)\n"
			"fsgnjn.s f0, f13, f25\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f14, 672(x28)\n"
			"flw.ps f28, 384(x28)\n"
			"fle.s x13, f14, f28\n" 
			"la x19, rand_int_32\n"
			"flw.ps f30, 64(x19)\n"
			"flw.ps f16, 512(x19)\n"
			"fand.pi f18, f30, f16\n" 
			"la x11, rand_int_32\n"
			"flw.ps f30, 640(x11)\n"
			"flw.ps f19, 384(x11)\n"
			"for.pi f18, f30, f19\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f12, 320(x27)\n"
			"flw.ps f16, 768(x27)\n"
			"fsgnjn.ps f12, f12, f16\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f13, 992(x27)\n"
			"flw.ps f25, 608(x27)\n"
			"flt.s x19, f13, f25\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x20, rand_ieee754_32\n"
			"flw.ps f4, 192(x20)\n"
			"fcvt.wu.s x13, f4, rup\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f22, 224(x19)\n"
			"fswizz.ps f22, f22, 0xf\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_3_XMA_QNT_STR_SEQ_ID_866020_HID_4:\n"
			"li x5, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x0 << TENSOR_FMA32_B_NUM_COLS) | (0x9 << TENSOR_FMA32_A_NUM_ROWS) | (0x2 << TENSOR_FMA32_A_NUM_COLS) | (0x2 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0xad << TENSOR_FMA32_SCP_LOC_B) | (0x9c << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"LBL_SEQID_3_XMA_QNT_STR_SEQ_ID_763611_HID_4:\n"
			"li x5, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x2 << TENSOR_IMA_B_NUM_COLS) | (0x0 << TENSOR_IMA_A_NUM_ROWS) | (0x9 << TENSOR_IMA_A_NUM_COLS) | (0xe << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x4e << TENSOR_IMA_SCP_LOC_B) | (0x7d << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x6, ((0xe << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0x0 << TENSOR_QUANT_QUANT_ROW) | (0x3a << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x1 << TENSOR_QUANT_TRANSF8) | (0x5 << TENSOR_QUANT_TRANSF7) | (0x8 << TENSOR_QUANT_TRANSF6) | (0x8 << TENSOR_QUANT_TRANSF5) | (0x5 << TENSOR_QUANT_TRANSF4) | (0xa << TENSOR_QUANT_TRANSF3) | (0xa << TENSOR_QUANT_TRANSF2) | (0x9 << TENSOR_QUANT_TRANSF1) | (0x5 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 7\n"
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_4\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_3_XMA_QNT_STR_SEQ_ITER_0_HID_4:\n"
			"li x6, ((0xf << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0xa << TENSOR_QUANT_QUANT_ROW) | (0x17 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x7 << TENSOR_QUANT_TRANSF5) | (0x7 << TENSOR_QUANT_TRANSF4) | (0x6 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"la x26, rand_int_32\n"
			"flw.ps f28, 704(x26)\n"
			"faddi.pi f16, f28, 0x85\n" 
			"maskpopc x19, m5\n" 
			"la x26, rand_int_32\n"
			"flw.ps f16, 992(x26)\n"
			"fsll.pi f16, f16, f16\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f3, 96(x12)\n"
			"flw.ps f29, 896(x12)\n"
			"flw.ps f30, 864(x12)\n"
			"fmsub.s f13, f3, f29, f30, rup\n" 
			"la x26, rand_ieee754_16\n"
			"flw.ps f25, 640(x26)\n"
			"fcvt.ps.f16 f19, f25\n" 
			"maskpopcz x13, m6\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f29, 896(x17)\n"
			"flw.ps f25, 544(x17)\n"
			"fmin.s f10, f29, f25\n" 
			"la x11, rand_int_32\n"
			"flw.ps f4, 992(x11)\n"
			"flw.ps f19, 576(x11)\n"
			"fxor.pi f0, f4, f19\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f19, 0(x11)\n"
			"flw.ps f14, 992(x11)\n"
			"fmax.s f29, f19, f14\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f15, 320(x13)\n"
			"ffrc.ps f28, f15\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x26, rand_ieee754_32\n"
			"flw.ps f0, 864(x26)\n"
			"flw.ps f3, 32(x26)\n"
			"fcmovm.ps f19, f0, f3\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f14, 160(x26)\n"
			"flw.ps f10, 288(x26)\n"
			"fsgnj.s f25, f14, f10\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f19, 800(x19)\n"
			"flw.ps f3, 704(x19)\n"
			"feq.s x19, f19, f3\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f30, 864(x15)\n"
			"flw.ps f28, 0(x15)\n"
			"fle.s x15, f30, f28\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f19, 224(x12)\n"
			"flw.ps f10, 832(x12)\n"
			"flw.ps f29, 416(x12)\n"
			"fnmsub.ps f16, f19, f10, f29, rdn\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f22, 800(x11)\n"
			"flw.ps f12, 832(x11)\n"
			"fsgnj.s f13, f22, f12\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_4_TLD_XMA_STR_SEQ_ITER_0_HID_4:\n"
			"la x31, self_check_32\n"
			"li x5, 0xe0000000000007\n"
			"add x5, x5, x31\n"
			"li x6, 0x1000000000000f\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x3 << TENSOR_FMA32_B_NUM_COLS) | (0x7 << TENSOR_FMA32_A_NUM_ROWS) | (0xf << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x1 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x19 << TENSOR_FMA32_SCP_LOC_B) | (0x7 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x9, ((0xe << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0x7 << TENSOR_QUANT_QUANT_ROW) | (0x31 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x2 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0x10\n"
			"bne x5, x31, LBL_FAIL_HID_4\n"
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 10\n"
			"LBL_SEQID_4_TLD_XMA_STR_SEQ_ITER_1_HID_4:\n"
			"la x31, self_check_16\n"
			"li x5, 0xa000000000000b\n"
			"add x5, x5, x31\n"
			"li x6, 0x30000000000000a\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x1 << TENSOR_FMA16_B_NUM_COLS) | (0xb << TENSOR_FMA16_A_NUM_ROWS) | (0xa << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x18 << TENSOR_FMA16_SCP_LOC_B) | (0x5 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x5 << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0xb << TENSOR_QUANT_QUANT_ROW) | (0x18 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x7 << TENSOR_QUANT_TRANSF7) | (0x6 << TENSOR_QUANT_TRANSF6) | (0x6 << TENSOR_QUANT_TRANSF5) | (0x7 << TENSOR_QUANT_TRANSF4) | (0x7 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0x16\n"
			"bne x5, x31, LBL_FAIL_HID_4\n"
			"csrw tensor_quant, x9\n"	
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
			"la x12, rand_int_32\n"
			"flw.ps f19, 384(x12)\n"
			"flw.ps f3, 480(x12)\n"
			"fxor.pi f16, f19, f3\n" 
			"LBL_SEQID_5_M0_WRITE_ID_10_HID_4:\n"
			"maskand m5, m6, m6\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x19, rand_ieee754_32\n"
			"flw.ps f13, 96(x19)\n"
			"flw.ps f30, 608(x19)\n"
			"fsgnj.ps f28, f13, f30\n" 
			"la x13, rand_int_32\n"
			"flw.ps f4, 384(x13)\n"
			"flw.ps f28, 0(x13)\n"
			"fremu.pi f22, f4, f28\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x28, rand_int_32\n"
			"flw.ps f3, 576(x28)\n"
			"flw.ps f22, 800(x28)\n"
			"fadd.pi f12, f3, f22\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f3, 896(x11)\n"
			"fcvt.pw.ps f3, f3, rmm\n" 
			"la x28, rand_int_32\n"
			"flw.ps f22, 160(x28)\n"
			"flw.ps f19, 288(x28)\n"
			"fsrl.pi f14, f22, f19\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f14, 320(x17)\n"
			"fcvt.pwu.ps f4, f14, rmm\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x20, rand_ieee754_32\n"
			"flw.ps f22, 352(x20)\n"
			"flw.ps f3, 96(x20)\n"
			"fle.s x26, f22, f3\n" 
			"la x17, rand_ieee754_16\n"
			"flw.ps f18, 256(x17)\n"
			"fcvt.ps.f16 f30, f18\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f30, 160(x19)\n"
			"flw.ps f29, 544(x19)\n"
			"fmsub.ps f12, f30, f30, f29, rtz\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f10, 736(x12)\n"
			"flw.ps f29, 928(x12)\n"
			"fsgnjx.ps f4, f10, f29\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f28, 896(x27)\n"
			"flw.ps f13, 160(x27)\n"
			"feq.s x13, f28, f13\n" 
			"la x17, rand_int_32\n"
			"flw.ps f12, 64(x17)\n"
			"flw.ps f29, 32(x17)\n"
			"fsub.pi f15, f12, f29\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f29, 320(x12)\n"
			"flw.ps f4, 832(x12)\n"
			"fmin.s f25, f29, f4\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f25, 288(x15)\n"
			"flw.ps f0, 832(x15)\n"
			"fsub.s f14, f25, f0, rne\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_5_TLD_XMA_STR_SEQ_ID_761493_HID_4:\n"
			"la x31, self_check_16\n"
			"li x5, 0x20000000000006\n"
			"add x5, x5, x31\n"
			"li x6, 0x300000000000006\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x3 << TENSOR_FMA16_B_NUM_COLS) | (0x6 << TENSOR_FMA16_A_NUM_ROWS) | (0x6 << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x18 << TENSOR_FMA16_SCP_LOC_B) | (0x1 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x15 << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0x6 << TENSOR_QUANT_QUANT_ROW) | (0x24 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x0 << TENSOR_QUANT_TRANSF1) | (0x2 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0xe\n"
			"bne x5, x31, LBL_FAIL_HID_4\n"
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_4\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_5_TLD_XMA_STR_SEQ_ITER_0_HID_4:\n"
			"la x31, self_check_8\n"
			"li x5, 0xa0000000000007\n"
			"add x5, x5, x31\n"
			"li x6, 0x3a0000000000001\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x1 << TENSOR_IMA_B_NUM_COLS) | (0x7 << TENSOR_IMA_A_NUM_ROWS) | (0x1 << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1d << TENSOR_IMA_SCP_LOC_B) | (0x5 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fmvz.x.ps x31, f0, 0\n"
			"li x5, 0x8\n"
			"bne x5, x31, LBL_FAIL_HID_4\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_4\n"
			"csrw tensor_error, x0\n"
			"li x5, 6\n" 
			"bne x30, x5, LBL_FAIL_HID_4\n"
			"addi x30, x30, 1\n"
			"la x12, rand_int_32\n"
			"flw.ps f3, 64(x12)\n"
			"fslli.pi f13, f3, 0xb\n" 
			"LBL_SEQID_6_M0_WRITE_ID_11_HID_4:\n"
			"maskand m2, m6, m2\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_6_FP_TRANS_ID_12_HID_4:\n"
			"la x11, rand_ieee754_32\n"
			"flw.ps f30, 224(x11)\n"
			"fexp.ps f14, f30\n" 
			"LBL_SEQID_6_M0_WRITE_ID_13_HID_4:\n"
			"fltm.ps m6, f15, f29\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x20, rand_ieee754_32\n"
			"flw.ps f28, 448(x20)\n"
			"flw.ps f25, 896(x20)\n"
			"flw.ps f30, 384(x20)\n"
			"fmadd.ps f13, f28, f25, f30, rdn\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f25, 672(x11)\n"
			"flw.ps f4, 288(x11)\n"
			"feq.ps f30, f25, f4\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f18, 160(x15)\n"
			"fcvt.wu.s x27, f18, rdn\n" 
			"la x19, rand_int_32\n"
			"flw.ps f12, 224(x19)\n"
			"flw.ps f29, 32(x19)\n"
			"fxor.pi f30, f12, f29\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f14, 928(x20)\n"
			"flw.ps f0, 0(x20)\n"
			"flw.ps f4, 128(x20)\n"
			"fmsub.s f19, f14, f0, f4, rmm\n" 
			"la x11, rand_int_32\n"
			"flw.ps f4, 608(x11)\n"
			"flw.ps f28, 512(x11)\n"
			"fmul.pi f22, f4, f28\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f4, 544(x19)\n"
			"frsq.ps f12, f4\n" 
			"la x20, rand_int_32\n"
			"flw.ps f29, 256(x20)\n"
			"flw.ps f22, 640(x20)\n"
			"fdivu.pi f29, f29, f22\n" 
			"la x12, rand_int_32\n"
			"flw.ps f4, 288(x12)\n"
			"flw.ps f14, 352(x12)\n"
			"flt.pi f13, f4, f14\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f4, 608(x12)\n"
			"fclass.s x26, f4\n" 
			"la x19, rand_int_32\n"
			"flw.ps f15, 480(x19)\n"
			"flw.ps f18, 736(x19)\n"
			"fmulh.pi f30, f15, f18\n" 
			"la x13, rand_int_32\n"
			"flw.ps f19, 192(x13)\n"
			"flw.ps f22, 0(x13)\n"
			"fminu.pi f22, f19, f22\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_6_TLD_XMA_STR_SEQ_ITER_0_HID_4:\n"
			"la x31, self_check_16\n"
			"li x5, 0xc0000000000009\n"
			"add x5, x5, x31\n"
			"li x6, 0x1000000000000f\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x0 << TENSOR_FMA16_B_NUM_COLS) | (0x9 << TENSOR_FMA16_A_NUM_ROWS) | (0xf << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x1 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1a << TENSOR_FMA16_SCP_LOC_B) | (0x6 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x1a << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0x9 << TENSOR_QUANT_QUANT_ROW) | (0x30 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x2 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0x20\n"
			"bne x5, x31, LBL_FAIL_HID_4\n"
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 10\n"
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
			"csrwi mhpmevent3, 2\n"   
			"csrwi mhpmevent4, 1\n"   
			"csrwi mhpmevent5, 3\n"   
			"csrwi mhpmevent6, 6\n"   
			"csrwi mhpmevent7, 22\n"   
			"csrwi mhpmevent8, 2\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f9,  0 (x5)\n"
			"flw.ps f29,  896 (x5)\n"
			"flw.ps f11,  416 (x5)\n"
			"flw.ps f24,  256 (x5)\n"
			"flw.ps f8,  768 (x5)\n"
			"flw.ps f17,  608 (x5)\n"
			"flw.ps f16,  864 (x5)\n"
			"flw.ps f15,  544 (x5)\n"
			"flw.ps f23,  320 (x5)\n"
			"flw.ps f19,  192 (x5)\n"
			"flw.ps f0,  32 (x5)\n"
			"flw.ps f25,  448 (x5)\n"
			"flw.ps f2,  736 (x5)\n"
			"flw.ps f3,  992 (x5)\n"
			"flw.ps f28,  384 (x5)\n"
			"flw.ps f21,  928 (x5)\n"
			"flw.ps f14,  640 (x5)\n"
			"flw.ps f30,  800 (x5)\n"
			"flw.ps f20,  832 (x5)\n"
			"flw.ps f4,  288 (x5)\n"
			"flw.ps f22,  160 (x5)\n"
			"flw.ps f31,  960 (x5)\n"
			"flw.ps f10,  128 (x5)\n"
			"flw.ps f5,  480 (x5)\n"
			"flw.ps f7,  96 (x5)\n"
			"flw.ps f6,  224 (x5)\n"
			"flw.ps f18,  704 (x5)\n"
			"flw.ps f26,  352 (x5)\n"
			"flw.ps f1,  576 (x5)\n"
			"flw.ps f12,  512 (x5)\n"
			"flw.ps f13,  64 (x5)\n"
			"flw.ps f27,  672 (x5)\n"
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
			"la x20, rand_ieee754_32\n"
			"flw.ps f26, 544(x20)\n"
			"fcvt.w.s x26, f26, rup\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f20, 320(x22)\n"
			"flw.ps f22, 448(x22)\n"
			"fsgnjx.ps f20, f20, f22\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f1, 480(x25)\n"
			"flw.ps f3, 736(x25)\n"
			"fmax.ps f0, f1, f3\n" 
			"la x22, rand_int_32\n"
			"flw.ps f20, 544(x22)\n"
			"fmax.pi f0, f20, f20\n" 
			"fbci.ps f2, 0x16714\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f20, 448(x23)\n"
			"flw.ps f2, 384(x23)\n"
			"fsub.ps f29, f20, f2, rdn\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f28, 992(x14)\n"
			"flw.ps f0, 416(x14)\n"
			"fnmsub.ps f3, f28, f0, f28, rmm\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f3, 672(x23)\n"
			"flw.ps f14, 960(x23)\n"
			"fsgnjn.s f11, f3, f14\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f2, 448(x27)\n"
			"flw.ps f20, 352(x27)\n"
			"fadd.s f14, f2, f20, rne\n" 
			"la x16, rand_int_32\n"
			"flw.ps f27, 896(x16)\n"
			"flw.ps f22, 32(x16)\n"
			"for.pi f21, f27, f22\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f21, 128(x25)\n"
			"flw.ps f20, 128(x25)\n"
			"flw.ps f22, 960(x25)\n"
			"fmsub.s f28, f21, f20, f22, rup\n" 
			"la x26, rand_int_32\n"
			"flw.ps f29, 128(x26)\n"
			"fsrai.pi f3, f29, 0xb\n" 
			"la x25, rand_int_32\n"
			"flw.ps f16, 704(x25)\n"
			"flw.ps f22, 576(x25)\n"
			"fxor.pi f20, f16, f22\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f22, 672(x23)\n"
			"flw.ps f9, 512(x23)\n"
			"flw.ps f20, 384(x23)\n"
			"fnmadd.s f2, f22, f9, f20, rdn\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f27, 256(x19)\n"
			"flw.ps f26, 832(x19)\n"
			"flw.ps f29, 448(x19)\n"
			"fnmadd.ps f3, f27, f26, f29, rup\n" 
			"la x17, rand_int_32\n"
			"flw.ps f26, 448(x17)\n"
			"flw.ps f3, 768(x17)\n"
			"fmul.pi f29, f26, f3\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_5\n"
			"addi x30, x30, 1\n"
			"la x19, rand_int_32\n"
			"flw.ps f1, 608(x19)\n"
			"flw.ps f16, 0(x19)\n"
			"fmin.pi f1, f1, f16\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f21, 160(x17)\n"
			"flw.ps f2, 992(x17)\n"
			"fmin.ps f14, f21, f2\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f10, 928(x17)\n"
			"flw.ps f0, 960(x17)\n"
			"fcmovm.ps f22, f10, f0\n" 
			"LBL_SEQID_1_M0_WRITE_ID_0_HID_5:\n"
			"maskxor m4, m6, m6\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"li x27, 0xa2929cde7af03e57\n"
			"fcvt.s.lu f20, x27, rmm\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f29, 896(x17)\n"
			"fclass.s x16, f29\n" 
			"la x19, rand_int_32\n"
			"flw.ps f21, 32(x19)\n"
			"fpackrepb.pi f28, f21\n" 
			"LBL_SEQID_1_FP_TRANS_ID_1_HID_5:\n"
			"la x14, rand_ieee754_32\n"
			"flw.ps f16, 288(x14)\n"
			"frcp.ps f10, f16\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f0, 384(x25)\n"
			"flw.ps f11, 320(x25)\n"
			"flw.ps f20, 736(x25)\n"
			"fcmov.ps f21, f0, f11, f20\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f14, 896(x22)\n"
			"flw.ps f11, 320(x22)\n"
			"flw.ps f10, 800(x22)\n"
			"fmsub.s f26, f14, f11, f10, rmm\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f14, 416(x14)\n"
			"flw.ps f0, 352(x14)\n"
			"fdiv.s f14, f14, f0, rup\n" 
			"LBL_SEQID_1_M0_WRITE_ID_2_HID_5:\n"
			"fsetm.pi m4, f21\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x19, rand_ieee754_32\n"
			"flw.ps f20, 768(x19)\n"
			"fswizz.ps f14, f20, 0x2\n" 
			"LBL_SEQID_1_M0_WRITE_ID_3_HID_5:\n"
			"flem.ps m2, f20, f10\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_1_M0_WRITE_ID_4_HID_5:\n"
			"maskand m6, m4, m1\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x25, rand_ieee754_32\n"
			"flw.ps f21, 0(x25)\n"
			"flw.ps f1, 128(x25)\n"
			"flw.ps f28, 224(x25)\n"
			"fcmov.ps f16, f21, f1, f28\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_5\n"
			"addi x30, x30, 1\n"
			"la x17, rand_ieee754_32\n"
			"flw.ps f3, 288(x17)\n"
			"flw.ps f16, 448(x17)\n"
			"fnmsub.ps f3, f3, f16, f16, rmm\n" 
			"la x25, rand_int_32\n"
			"flw.ps f3, 224(x25)\n"
			"fcvt.ps.pwu f1, f3, rdn\n" 
			"la x27, rand_int_32\n"
			"flw.ps f1, 64(x27)\n"
			"flw.ps f21, 768(x27)\n"
			"fltu.pi f0, f1, f21\n" 
			"LBL_SEQID_2_M0_WRITE_ID_5_HID_5:\n"
			"flem.ps m1, f29, f16\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x16, rand_ieee754_32\n"
			"flw.ps f2, 256(x16)\n"
			"flw.ps f28, 480(x16)\n"
			"fadd.s f21, f2, f28, rmm\n" 
			"la x25, rand_int_32\n"
			"flw.ps f1, 224(x25)\n"
			"flw.ps f27, 224(x25)\n"
			"fmul.pi f10, f1, f27\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f14, 544(x23)\n"
			"fclass.s x20, f14\n" 
			"la x25, rand_int_32\n"
			"flw.ps f9, 320(x25)\n"
			"flw.ps f27, 512(x25)\n"
			"fmax.pi f14, f9, f27\n" 
			"li x16, 0x2da100f00c3865fd\n"
			"fcvt.s.wu f2, x16, rtz\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f26, 320(x22)\n"
			"fcvt.f16.ps f3, f26\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f1, 480(x22)\n"
			"fsgnjn.ps f20, f1, f1\n" 
			"la x19, rand_int_32\n"
			"flw.ps f27, 288(x19)\n"
			"flw.ps f28, 96(x19)\n"
			"fsll.pi f22, f27, f28\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f21, 288(x26)\n"
			"flw.ps f11, 960(x26)\n"
			"fsgnjx.ps f22, f21, f11\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f1, 672(x23)\n"
			"fcvt.pw.ps f3, f1, rmm\n" 
			"la x14, rand_int_32\n"
			"flw.ps f22, 576(x14)\n"
			"fcvt.ps.pwu f11, f22, rtz\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f14, 640(x22)\n"
			"flw.ps f20, 512(x22)\n"
			"flw.ps f11, 192(x22)\n"
			"fmadd.s f26, f14, f20, f11, rmm\n" 
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
			"LBL_SEQID_3_M0_WRITE_ID_6_HID_5:\n"
			"maskand m6, m1, m4\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x14, rand_int_32\n"
			"flw.ps f21, 224(x14)\n"
			"flw.ps f3, 512(x14)\n"
			"fltu.pi f29, f21, f3\n" 
			"LBL_SEQID_3_FP_TRANS_ID_7_HID_5:\n"
			"la x16, rand_ieee754_32\n"
			"flw.ps f11, 512(x16)\n"
			"fexp.ps f9, f11\n" 
			"LBL_SEQID_3_M0_WRITE_ID_8_HID_5:\n"
			"flem.ps m2, f0, f28\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x17, rand_ieee754_32\n"
			"flw.ps f14, 512(x17)\n"
			"flw.ps f22, 128(x17)\n"
			"fmax.s f26, f14, f22\n" 
			"la x19, rand_int_32\n"
			"flw.ps f21, 64(x19)\n"
			"flw.ps f3, 96(x19)\n"
			"fsrl.pi f14, f21, f3\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f14, 288(x19)\n"
			"flw.ps f10, 224(x19)\n"
			"flt.ps f11, f14, f10\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f27, 288(x26)\n"
			"fsqrt.s f3, f27, rtz\n" 
			"la x22, rand_int_32\n"
			"flw.ps f27, 480(x22)\n"
			"fpackrepb.pi f10, f27\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f26, 448(x26)\n"
			"flw.ps f0, 768(x26)\n"
			"flt.ps f14, f26, f0\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f3, 288(x23)\n"
			"flw.ps f21, 864(x23)\n"
			"fcmovm.ps f28, f3, f21\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f11, 96(x20)\n"
			"flw.ps f14, 256(x20)\n"
			"feq.s x19, f11, f14\n" 
			"la x14, rand_int_32\n"
			"flw.ps f3, 800(x14)\n"
			"flw.ps f29, 704(x14)\n"
			"fmul.pi f21, f3, f29\n" 
			"LBL_SEQID_3_M0_WRITE_ID_9_HID_5:\n"
			"mova.m.x x22\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x27, rand_int_32\n"
			"flw.ps f27, 160(x27)\n"
			"fadd.pi f3, f27, f27\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f21, 800(x14)\n"
			"flw.ps f11, 128(x14)\n"
			"fsub.ps f10, f21, f11, rne\n" 
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
			"la x14, rand_int_32\n"
			"flw.ps f14, 64(x14)\n"
			"fandi.pi f21, f14, 0x1c7\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f3, 992(x19)\n"
			"flw.ps f10, 480(x19)\n"
			"flw.ps f21, 736(x19)\n"
			"fmadd.s f26, f3, f10, f21, rmm\n" 
			"maskpopcz x17, m6\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f22, 864(x19)\n"
			"flw.ps f1, 288(x19)\n"
			"fmax.s f29, f22, f1\n" 
			"la x14, rand_int_32\n"
			"flw.ps f2, 960(x14)\n"
			"flw.ps f22, 960(x14)\n"
			"fltu.pi f2, f2, f22\n" 
			"la x16, rand_int_32\n"
			"flw.ps f10, 928(x16)\n"
			"fpackreph.pi f21, f10\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f10, 992(x26)\n"
			"fcvt.pwu.ps f28, f10, rne\n" 
			"la x19, rand_int_32\n"
			"flw.ps f3, 352(x19)\n"
			"flw.ps f26, 800(x19)\n"
			"fmax.pi f29, f3, f26\n" 
			"la x17, rand_int_32\n"
			"flw.ps f22, 96(x17)\n"
			"flw.ps f27, 448(x17)\n"
			"fsra.pi f2, f22, f27\n" 
			"mova.x.m x25\n" 
			"fbci.ps f3, 0x51bd8\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f11, 512(x23)\n"
			"flw.ps f26, 512(x23)\n"
			"flt.s x16, f11, f26\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f10, 544(x25)\n"
			"flw.ps f11, 224(x25)\n"
			"fmul.s f29, f10, f11, rne\n" 
			"la x23, rand_int_32\n"
			"flw.ps f14, 480(x23)\n"
			"flw.ps f26, 864(x23)\n"
			"flt.pi f16, f14, f26\n" 
			"la x26, rand_int_32\n"
			"flw.ps f1, 544(x26)\n"
			"fsrai.pi f16, f1, 0x5\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f28, 96(x26)\n"
			"flw.ps f20, 512(x26)\n"
			"flw.ps f1, 64(x26)\n"
			"fnmadd.ps f27, f28, f20, f1, rup\n" 
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
			"la x14, rand_int_32\n"
			"flw.ps f21, 704(x14)\n"
			"flw.ps f29, 160(x14)\n"
			"flt.pi f11, f21, f29\n" 
			"la x19, rand_int_32\n"
			"flw.ps f2, 832(x19)\n"
			"fsrli.pi f9, f2, 0x1\n" 
			"la x14, rand_int_32\n"
			"flw.ps f26, 64(x14)\n"
			"flw.ps f20, 896(x14)\n"
			"fdiv.pi f29, f26, f20\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f9, 32(x23)\n"
			"flw.ps f28, 512(x23)\n"
			"fle.ps f21, f9, f28\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f2, 64(x23)\n"
			"flw.ps f20, 960(x23)\n"
			"flw.ps f21, 768(x23)\n"
			"fnmsub.ps f0, f2, f20, f21, rdn\n" 
			"LBL_SEQID_5_M0_WRITE_ID_10_HID_5:\n"
			"maskxor m1, m6, m6\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x17, rand_int_32\n"
			"flw.ps f28, 800(x17)\n"
			"flw.ps f3, 352(x17)\n"
			"fand.pi f14, f28, f3\n" 
			"la x14, rand_int_32\n"
			"flw.ps f20, 288(x14)\n"
			"flw.ps f26, 448(x14)\n"
			"fmul.pi f0, f20, f26\n" 
			"LBL_SEQID_5_M0_WRITE_ID_11_HID_5:\n"
			"feqm.ps m2, f1, f9\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_5_M0_WRITE_ID_12_HID_5:\n"
			"flem.ps m6, f14, f22\n" 
			"li x5, 0x803401F8\n"
			"ld x5, 0(x5)\n"
			"LBL_SEQID_5_M0_WRITE_ID_13_HID_5:\n"
			"feqm.ps m6, f3, f28\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x16, rand_int_32\n"
			"flw.ps f11, 576(x16)\n"
			"flw.ps f2, 544(x16)\n"
			"flt.pi f22, f11, f2\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f20, 992(x25)\n"
			"flw.ps f1, 576(x25)\n"
			"feq.s x14, f20, f1\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f3, 192(x22)\n"
			"flw.ps f16, 640(x22)\n"
			"flt.s x14, f3, f16\n" 
			"la x23, rand_int_32\n"
			"flw.ps f1, 512(x23)\n"
			"fsat8.pi f26, f1\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f2, 320(x16)\n"
			"fsub.s f16, f2, f2, rtz\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 6\n" 
			"bne x30, x5, LBL_FAIL_HID_5\n"
			"addi x30, x30, 1\n"
			"la x20, rand_ieee754_32\n"
			"flw.ps f26, 288(x20)\n"
			"flw.ps f27, 96(x20)\n"
			"flw.ps f2, 96(x20)\n"
			"fmsub.s f29, f26, f27, f2, rmm\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f26, 32(x16)\n"
			"flw.ps f20, 928(x16)\n"
			"flw.ps f22, 672(x16)\n"
			"fnmsub.s f29, f26, f20, f22, rmm\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f14, 800(x26)\n"
			"fcvt.pw.ps f0, f14, rdn\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f26, 960(x16)\n"
			"fcvt.pw.ps f0, f26, rdn\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f16, 832(x22)\n"
			"flw.ps f3, 224(x22)\n"
			"flt.s x27, f16, f3\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f14, 192(x23)\n"
			"fround.ps f27, f14, rup\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f20, 288(x17)\n"
			"fclass.ps f28, f20\n" 
			"la x20, rand_int_32\n"
			"flw.ps f16, 224(x20)\n"
			"flw.ps f22, 832(x20)\n"
			"feq.pi f20, f16, f22\n" 
			"LBL_SEQID_6_FP_TRANS_ID_14_HID_5:\n"
			"la x19, rand_ieee754_32\n"
			"flw.ps f22, 64(x19)\n"
			"frcp.ps f3, f22\n" 
			"LBL_SEQID_6_M0_WRITE_ID_15_HID_5:\n"
			"maskxor m6, m1, m4\n" 
			"li x5, CREDINC3\n"
			"ld x5, 0(x5)\n"
			"la x16, rand_ieee754_32\n"
			"flw.ps f26, 608(x16)\n"
			"fcvt.f16.ps f21, f26\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f16, 928(x26)\n"
			"flw.ps f27, 544(x26)\n"
			"fmin.s f14, f16, f27\n" 
			"la x19, rand_int_32\n"
			"flw.ps f28, 96(x19)\n"
			"fpackreph.pi f9, f28\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f22, 768(x17)\n"
			"fclass.ps f22, f22\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f27, 512(x25)\n"
			"flw.ps f2, 64(x25)\n"
			"flw.ps f22, 512(x25)\n"
			"fnmsub.s f10, f27, f2, f22, rdn\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f10, 640(x25)\n"
			"flw.ps f11, 32(x25)\n"
			"feq.ps f2, f10, f11\n" 
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
			"csrwi mhpmevent3, 8\n"   
			"csrwi mhpmevent4, 14\n"   
			"csrwi mhpmevent5, 4\n"   
			"csrwi mhpmevent6, 17\n"   
			"csrwi mhpmevent7, 6\n"   
			"csrwi mhpmevent8, 3\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f16,  192 (x5)\n"
			"flw.ps f11,  448 (x5)\n"
			"flw.ps f0,  416 (x5)\n"
			"flw.ps f9,  0 (x5)\n"
			"flw.ps f5,  960 (x5)\n"
			"flw.ps f12,  672 (x5)\n"
			"flw.ps f28,  992 (x5)\n"
			"flw.ps f2,  224 (x5)\n"
			"flw.ps f8,  160 (x5)\n"
			"flw.ps f4,  608 (x5)\n"
			"flw.ps f13,  288 (x5)\n"
			"flw.ps f24,  32 (x5)\n"
			"flw.ps f23,  512 (x5)\n"
			"flw.ps f31,  704 (x5)\n"
			"flw.ps f17,  928 (x5)\n"
			"flw.ps f10,  864 (x5)\n"
			"flw.ps f25,  352 (x5)\n"
			"flw.ps f19,  128 (x5)\n"
			"flw.ps f6,  800 (x5)\n"
			"flw.ps f22,  96 (x5)\n"
			"flw.ps f18,  832 (x5)\n"
			"flw.ps f21,  896 (x5)\n"
			"flw.ps f27,  768 (x5)\n"
			"flw.ps f29,  320 (x5)\n"
			"flw.ps f15,  640 (x5)\n"
			"flw.ps f3,  256 (x5)\n"
			"flw.ps f26,  480 (x5)\n"
			"flw.ps f20,  736 (x5)\n"
			"flw.ps f1,  64 (x5)\n"
			"flw.ps f7,  576 (x5)\n"
			"flw.ps f14,  384 (x5)\n"
			"flw.ps f30,  544 (x5)\n"
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
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_6:\n"
			"fltm.ps m6, f8, f7\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x26, rand_ieee754_32\n"
			"flw.ps f11, 576(x26)\n"
			"fcvt.f16.ps f29, f11\n" 
			"li x5, 0xc05c2b2e160c73d7\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0x97ce8f127f665e19\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"la x9, rand_ieee754_32\n"
			"flw.ps f18, 288(x9)\n"
			"flw.ps f2, 256(x9)\n"
			"fdiv.ps f14, f18, f2, rmm\n" 
			"la x10, rand_int_32\n"
			"flw.ps f14, 224(x10)\n"
			"flw.ps f5, 928(x10)\n"
			"fle.pi f30, f14, f5\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f18, 608(x11)\n"
			"flw.ps f30, 896(x11)\n"
			"feq.ps f18, f18, f30\n" 
			"la x25, rand_int_32\n"
			"flw.ps f16, 160(x25)\n"
			"fnot.pi f7, f16\n" 
			"la x26, rand_int_32\n"
			"flw.ps f20, 960(x26)\n"
			"flw.ps f14, 192(x26)\n"
			"fmax.pi f30, f20, f14\n" 
			"la x19, rand_int_32\n"
			"flw.ps f7, 928(x19)\n"
			"fsat8.pi f14, f7\n" 
			"li x5, 0x9b4d\n"
			"csrw tensor_mask, x5\n"	
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_6:\n"
			"masknot m6, m5\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0x6b196296fc\n"
			"fsw.ps f0, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x9, rand_int_32\n"
			"flw.ps f16, 896(x9)\n"
			"flw.ps f30, 640(x9)\n"
			"fdiv.pi f20, f16, f30\n" 
			"LBL_SEQID_0_M0_WRITE_ID_2_HID_6:\n"
			"fltm.pi m5, f5, f11\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x11, rand_ieee754_32\n"
			"flw.ps f3, 352(x11)\n"
			"fround.ps f5, f3, rne\n" 
			"LBL_SEQID_0_M0_WRITE_ID_3_HID_6:\n"
			"maskxor m7, m7, m7\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x13, rand_int_32\n"
			"flw.ps f20, 448(x13)\n"
			"flw.ps f17, 352(x13)\n"
			"fle.pi f14, f20, f17\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f26, 832(x19)\n"
			"fclass.ps f30, f26\n" 
			"la x19, rand_int_32\n"
			"flw.ps f14, 288(x19)\n"
			"fsrli.pi f29, f14, 0xe\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ID_730874_HID_6:\n"
			"la x31, self_check_16\n"
			"li x5, 0x20000000000006\n"
			"add x5, x5, x31\n"
			"li x6, 0x1000000000000b\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x3 << TENSOR_FMA16_B_NUM_COLS) | (0x6 << TENSOR_FMA16_A_NUM_ROWS) | (0xb << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x1 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x18 << TENSOR_FMA16_SCP_LOC_B) | (0x1 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x4 << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0x6 << TENSOR_QUANT_QUANT_ROW) | (0x19 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x2 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0x18\n"
			"bne x5, x31, LBL_FAIL_HID_6\n"
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 10\n"
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ID_814008_HID_6:\n"
			"la x31, self_check_32\n"
			"li x5, 0xa0000000000006\n"
			"add x5, x5, x31\n"
			"li x6, 0x10000000000008\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x1 << TENSOR_FMA32_B_NUM_COLS) | (0x6 << TENSOR_FMA32_A_NUM_ROWS) | (0x8 << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x1 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1a << TENSOR_FMA32_SCP_LOC_B) | (0x5 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0x9\n"
			"bne x5, x31, LBL_FAIL_HID_6\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_6\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_0_RDX_ID_0_SHIRE_0_HID_6:\n"
			"li x5, 0x3a0000000353000b\n"
			"csrw tensor_reduce, x5\n" 
			"csrwi tensor_wait, 9\n" 
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ID_515472_HID_6:\n"
			"la x31, self_check_16\n"
			"li x5, 0xe0000000000008\n"
			"add x5, x5, x31\n"
			"li x6, 0x34000000000000f\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x0 << TENSOR_FMA16_B_NUM_COLS) | (0x8 << TENSOR_FMA16_A_NUM_ROWS) | (0xf << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1a << TENSOR_FMA16_SCP_LOC_B) | (0x7 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0x20\n"
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
			"li x5, 0x80000000000002\n"
			"add x5, x5, x31\n"
			"li x6, 0x1000000000000d\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x1 << TENSOR_FMA32_B_NUM_COLS) | (0x2 << TENSOR_FMA32_A_NUM_ROWS) | (0xd << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x1 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1f << TENSOR_FMA32_SCP_LOC_B) | (0x4 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
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
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_6\n"
			"csrw tensor_error, x0\n"
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_6\n"
			"addi x30, x30, 1\n"
			"la x7, rand_ieee754_32\n"
			"flw.ps f18, 576(x7)\n"
			"flw.ps f26, 992(x7)\n"
			"fsgnj.ps f29, f18, f26\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f5, 832(x7)\n"
			"flw.ps f2, 672(x7)\n"
			"fadd.ps f29, f5, f2, rup\n" 
			"la x5, rand_int_32\n"
			"flw.ps f11, 992(x5)\n"
			"flw.ps f18, 384(x5)\n"
			"fsrl.pi f20, f11, f18\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f29, 992(x11)\n"
			"fcvt.lu.s x25, f29, rup\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f14, 256(x16)\n"
			"flw.ps f11, 736(x16)\n"
			"feq.s x19, f14, f11\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f2, 0(x16)\n"
			"flw.ps f14, 64(x16)\n"
			"flw.ps f15, 128(x16)\n"
			"fnmsub.ps f2, f2, f14, f15, rdn\n" 
			"LBL_SEQID_1_M0_WRITE_ID_4_HID_6:\n"
			"maskxor m3, m6, m7\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0xf84d7d3e28\n"
			"flw.ps f5, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x7, rand_int_32\n"
			"flw.ps f17, 992(x7)\n"
			"fcvt.ps.pwu f18, f17, rtz\n" 
			"la x9, rand_int_32\n"
			"flw.ps f2, 160(x9)\n"
			"flw.ps f20, 128(x9)\n"
			"fsub.pi f7, f2, f20\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f7, 32(x9)\n"
			"flw.ps f8, 928(x9)\n"
			"feq.s x9, f7, f8\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f3, 992(x11)\n"
			"flw.ps f30, 800(x11)\n"
			"flw.ps f17, 96(x11)\n"
			"fmadd.ps f18, f3, f30, f17, rne\n" 
			"LBL_SEQID_1_M0_WRITE_ID_5_HID_6:\n"
			"fltm.pi m7, f5, f14\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x10, rand_ieee754_32\n"
			"flw.ps f26, 256(x10)\n"
			"flw.ps f30, 384(x10)\n"
			"fsub.s f18, f26, f30, rup\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f8, 992(x10)\n"
			"flw.ps f7, 896(x10)\n"
			"feq.s x5, f8, f7\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x7, rand_int_32\n"
			"flw.ps f5, 640(x7)\n"
			"fnot.pi f30, f5\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f3, 832(x5)\n"
			"flw.ps f20, 928(x5)\n"
			"fmul.ps f15, f3, f20, rmm\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ID_939271_HID_6:\n"
			"li x5, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x2 << TENSOR_IMA_B_NUM_COLS) | (0x3 << TENSOR_IMA_A_NUM_ROWS) | (0xa << TENSOR_IMA_A_NUM_COLS) | (0x6 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x88 << TENSOR_IMA_SCP_LOC_B) | (0xca << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x0 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_6\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_1_RDX_ID_1_SHIRE_0_HID_6:\n"
			"li x25, 0x300000000738000b\n"
			"csrw tensor_reduce, x25\n" 
			"csrwi tensor_wait, 9\n" 
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=0); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"LBL_SEQID_1_TLD_XMA_STR_SEQ_ITER_0_HID_6:\n"
			"la x31, self_check_8\n"
			"li x5, 0xe000000000000e\n"
			"add x5, x5, x31\n"
			"li x6, 0x300000000000000\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x3 << TENSOR_IMA_B_NUM_COLS) | (0xe << TENSOR_IMA_A_NUM_ROWS) | (0x0 << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x18 << TENSOR_IMA_SCP_LOC_B) | (0x7 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fmvz.x.ps x31, f0, 0\n"
			"li x5, 0x4\n"
			"bne x5, x31, LBL_FAIL_HID_6\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_6\n"
			"csrw tensor_error, x0\n"
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_6\n"
			"addi x30, x30, 1\n"
			"la x16, rand_ieee754_32\n"
			"flw.ps f2, 800(x16)\n"
			"fcvt.pwu.ps f11, f2, rmm\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f26, 704(x10)\n"
			"flw.ps f30, 160(x10)\n"
			"fadd.s f7, f26, f30, rtz\n" 
			"maskpopcz x5, m3\n" 
			"la x26, rand_int_32\n"
			"flw.ps f26, 672(x26)\n"
			"fnot.pi f26, f26\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f30, 192(x26)\n"
			"fclass.s x10, f30\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x10, rand_int_32\n"
			"flw.ps f30, 128(x10)\n"
			"flw.ps f14, 480(x10)\n"
			"fmin.pi f7, f30, f14\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f11, 0(x7)\n"
			"flw.ps f17, 800(x7)\n"
			"fmul.s f18, f11, f17, rne\n" 
			"fbci.ps f27, 0x4d4e0\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f30, 32(x19)\n"
			"fcvt.l.s x26, f30, rne\n" 
			"la x10, rand_int_32\n"
			"flw.ps f7, 544(x10)\n"
			"flw.ps f15, 96(x10)\n"
			"fltu.pi f15, f7, f15\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x16, rand_int_32\n"
			"flw.ps f27, 672(x16)\n"
			"flw.ps f16, 800(x16)\n"
			"feq.pi f3, f27, f16\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f2, 704(x9)\n"
			"flw.ps f14, 928(x9)\n"
			"fsgnj.ps f30, f2, f14\n" 
			"LBL_SEQID_2_M0_WRITE_ID_6_HID_6:\n"
			"feqm.ps m3, f11, f17\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x25, rand_ieee754_32\n"
			"flw.ps f26, 384(x25)\n"
			"fcvt.w.s x13, f26, rmm\n" 
			"la x19, rand_int_32\n"
			"flw.ps f5, 160(x19)\n"
			"flw.ps f26, 416(x19)\n"
			"fmul.pi f15, f5, f26\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f5, 384(x10)\n"
			"flw.ps f15, 416(x10)\n"
			"fsgnjx.s f5, f5, f15\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_2_TLD_XMA_STR_SEQ_ID_447044_HID_6:\n"
			"la x31, self_check_32\n"
			"li x5, 0xe0000000000002\n"
			"add x5, x5, x31\n"
			"li x6, 0x1000000000000b\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x3 << TENSOR_FMA32_B_NUM_COLS) | (0x2 << TENSOR_FMA32_A_NUM_ROWS) | (0xb << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x1 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x19 << TENSOR_FMA32_SCP_LOC_B) | (0x7 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0xc\n"
			"bne x5, x31, LBL_FAIL_HID_6\n"
			"LBL_SEQID_2_TLD_XMA_STR_SEQ_ID_546953_HID_6:\n"
			"la x31, self_check_8\n"
			"li x5, 0x20000000000002\n"
			"add x5, x5, x31\n"
			"li x6, 0x10000000000005\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x3 << TENSOR_IMA_B_NUM_COLS) | (0x2 << TENSOR_IMA_A_NUM_ROWS) | (0x5 << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x1 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1a << TENSOR_IMA_SCP_LOC_B) | (0x1 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x9, ((0x0 << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0x2 << TENSOR_QUANT_QUANT_ROW) | (0x28 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x0 << TENSOR_QUANT_TRANSF1) | (0x0 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fmvz.x.ps x31, f0, 0\n"
			"li x5, 0x18\n"
			"bne x5, x31, LBL_FAIL_HID_6\n"
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_6\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_2_XMA_QNT_STR_SEQ_ITER_0_HID_6:\n"
			"li x5, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x3 << TENSOR_FMA16_B_NUM_COLS) | (0x1 << TENSOR_FMA16_A_NUM_ROWS) | (0x7 << TENSOR_FMA16_A_NUM_COLS) | (0xd << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0xa2 << TENSOR_FMA16_SCP_LOC_B) | (0xad << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
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
			"la x19, rand_ieee754_32\n"
			"flw.ps f15, 384(x19)\n"
			"flw.ps f30, 416(x19)\n"
			"fsgnjn.ps f14, f15, f30\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f11, 160(x16)\n"
			"flw.ps f8, 800(x16)\n"
			"fmul.s f29, f11, f8, rtz\n" 
			"LBL_SEQID_3_M0_WRITE_ID_7_HID_6:\n"
			"flem.ps m3, f15, f26\n" 
			"li x5, 0x80340188\n"
			"ld x5, 0(x5)\n"
			"la x26, rand_int_32\n"
			"flw.ps f7, 64(x26)\n"
			"flw.ps f5, 192(x26)\n"
			"fminu.pi f14, f7, f5\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"LBL_SEQID_3_M0_WRITE_ID_8_HID_6:\n"
			"mova.m.x x10\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"li x5, 0x97e\n"
			"csrw tensor_mask, x5\n"	
			"la x9, rand_int_32\n"
			"flw.ps f17, 928(x9)\n"
			"fpackrepb.pi f26, f17\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f30, 992(x10)\n"
			"flw.ps f29, 480(x10)\n"
			"flw.ps f26, 672(x10)\n"
			"fnmadd.ps f29, f30, f29, f26, rup\n" 
			"la x11, rand_int_32\n"
			"flw.ps f7, 64(x11)\n"
			"flw.ps f17, 0(x11)\n"
			"for.pi f17, f7, f17\n" 
			"LBL_SEQID_3_FP_TRANS_ID_9_HID_6:\n"
			"la x26, rand_ieee754_32\n"
			"flw.ps f26, 64(x26)\n"
			"flog.ps f20, f26\n" 
			"la x16, rand_int_32\n"
			"flw.ps f20, 416(x16)\n"
			"flw.ps f7, 928(x16)\n"
			"flt.pi f11, f20, f7\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f20, 128(x16)\n"
			"flw.ps f16, 64(x16)\n"
			"feq.ps f29, f20, f16\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f7, 32(x19)\n"
			"flw.ps f14, 960(x19)\n"
			"flw.ps f5, 544(x19)\n"
			"fnmsub.s f30, f7, f14, f5, rdn\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f18, 32(x13)\n"
			"fcvt.pw.ps f29, f18, rdn\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f11, 832(x26)\n"
			"flw.ps f26, 768(x26)\n"
			"flw.ps f5, 32(x26)\n"
			"fnmadd.ps f2, f11, f26, f5, rup\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f17, 416(x16)\n"
			"ffrc.ps f5, f17\n" 
			"la x7, rand_int_32\n"
			"flw.ps f29, 512(x7)\n"
			"faddi.pi f18, f29, 0x140\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 0x9b3ae2b9ad2b2664\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0xea132952a5a76240\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"LBL_SEQID_3_TLD_XMA_STR_SEQ_ID_620812_HID_6:\n"
			"la x31, self_check_32\n"
			"li x5, 0x20000000000004\n"
			"add x5, x5, x31\n"
			"li x6, 0x340000000000004\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x0 << TENSOR_FMA32_B_NUM_COLS) | (0x4 << TENSOR_FMA32_A_NUM_ROWS) | (0x4 << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1a << TENSOR_FMA32_SCP_LOC_B) | (0x1 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x16 << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0x4 << TENSOR_QUANT_QUANT_ROW) | (0x12 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x2 << TENSOR_QUANT_TRANSF5) | (0x6 << TENSOR_QUANT_TRANSF4) | (0x7 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0x5\n"
			"bne x5, x31, LBL_FAIL_HID_6\n"
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_6\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_3_XMA_QNT_STR_SEQ_ITER_0_HID_6:\n"
			"li x5, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x3 << TENSOR_FMA16_B_NUM_COLS) | (0x8 << TENSOR_FMA16_A_NUM_ROWS) | (0xc << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x9a << TENSOR_FMA16_SCP_LOC_B) | (0x7 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x6, ((0x19 << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0x8 << TENSOR_QUANT_QUANT_ROW) | (0x20 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x7 << TENSOR_QUANT_TRANSF5) | (0x7 << TENSOR_QUANT_TRANSF4) | (0x6 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 7\n"
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
			"la x7, rand_int_32\n"
			"flw.ps f29, 608(x7)\n"
			"fnot.pi f17, f29\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f16, 832(x16)\n"
			"flw.ps f18, 320(x16)\n"
			"flw.ps f17, 128(x16)\n"
			"fmadd.s f7, f16, f18, f17, rtz\n" 
			"LBL_SEQID_4_M0_WRITE_ID_10_HID_6:\n"
			"flem.ps m3, f29, f27\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0xf433c40e7a\n"
			"flw.ps f5, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"fbci.ps f8, 0x35254\n" 
			"LBL_SEQID_4_M0_WRITE_ID_11_HID_6:\n"
			"feqm.ps m6, f18, f8\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x7, rand_ieee754_32\n"
			"flw.ps f2, 640(x7)\n"
			"frsq.ps f11, f2\n" 
			"LBL_SEQID_4_M0_WRITE_ID_12_HID_6:\n"
			"feqm.ps m6, f15, f17\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x19, rand_ieee754_32\n"
			"flw.ps f18, 672(x19)\n"
			"flw.ps f20, 672(x19)\n"
			"fmin.s f29, f18, f20\n" 
			"li x13, 0xafebd684b46be0a9\n"
			"fcvt.s.wu f30, x13, rdn\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f8, 288(x7)\n"
			"frsq.ps f20, f8\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f2, 288(x13)\n"
			"flw.ps f16, 992(x13)\n"
			"fmax.s f15, f2, f16\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f18, 32(x25)\n"
			"flw.ps f11, 320(x25)\n"
			"fsgnjn.ps f11, f18, f11\n" 
			"LBL_SEQID_4_FP_TRANS_ID_13_HID_6:\n"
			"la x11, rand_ieee754_32\n"
			"flw.ps f3, 224(x11)\n"
			"flog.ps f8, f3\n" 
			"li x5, 0x5322a7c4fe3b61b1\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0xddf70fad5831f0d\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"la x16, rand_int_32\n"
			"flw.ps f17, 704(x16)\n"
			"flw.ps f3, 832(x16)\n"
			"flt.pi f3, f17, f3\n" 
			"la x11, rand_int_32\n"
			"flw.ps f16, 800(x11)\n"
			"fsat8.pi f2, f16\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f11, 672(x13)\n"
			"flw.ps f30, 256(x13)\n"
			"fsgnj.ps f29, f11, f30\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_4_XMA_QNT_STR_SEQ_ID_472367_HID_6:\n"
			"li x6, ((0x0 << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0x2 << TENSOR_QUANT_QUANT_ROW) | (0x36 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x9 << TENSOR_QUANT_TRANSF6) | (0x3 << TENSOR_QUANT_TRANSF5) | (0x3 << TENSOR_QUANT_TRANSF4) | (0x4 << TENSOR_QUANT_TRANSF3) | (0xa << TENSOR_QUANT_TRANSF2) | (0x5 << TENSOR_QUANT_TRANSF1) | (0x5 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_4_XMA_QNT_STR_SEQ_ID_290589_HID_6:\n"
			"li x6, ((0xf << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0xf << TENSOR_QUANT_QUANT_ROW) | (0x8 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x1 << TENSOR_QUANT_TRANSF3) | (0xa << TENSOR_QUANT_TRANSF2) | (0x8 << TENSOR_QUANT_TRANSF1) | (0xa << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_6\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_4_XMA_QNT_STR_SEQ_ITER_0_HID_6:\n"
			"li x6, ((0x18 << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0x1 << TENSOR_QUANT_QUANT_ROW) | (0x1a << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x1 << TENSOR_QUANT_TRANSF3) | (0xa << TENSOR_QUANT_TRANSF2) | (0x4 << TENSOR_QUANT_TRANSF1) | (0x9 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_4_XMA_QNT_STR_SEQ_ITER_1_HID_6:\n"
			"li x5, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x2 << TENSOR_IMA_B_NUM_COLS) | (0x7 << TENSOR_IMA_A_NUM_ROWS) | (0x5 << TENSOR_IMA_A_NUM_COLS) | (0xd << TENSOR_IMA_OFFSET) | (0x0 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x6 << TENSOR_IMA_SCP_LOC_B) | (0xb4 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x6, ((0x18 << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0x7 << TENSOR_QUANT_QUANT_ROW) | (0x3b << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0xa << TENSOR_QUANT_TRANSF4) | (0x5 << TENSOR_QUANT_TRANSF3) | (0x5 << TENSOR_QUANT_TRANSF2) | (0x8 << TENSOR_QUANT_TRANSF1) | (0x9 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_4_XMA_QNT_STR_SEQ_ITER_2_HID_6:\n"
			"li x5, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x0 << TENSOR_FMA16_B_NUM_COLS) | (0x0 << TENSOR_FMA16_A_NUM_ROWS) | (0x5 << TENSOR_FMA16_A_NUM_COLS) | (0x7 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x20 << TENSOR_FMA16_SCP_LOC_B) | (0xd6 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
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
			"la x19, rand_ieee754_32\n"
			"flw.ps f2, 448(x19)\n"
			"flw.ps f16, 288(x19)\n"
			"fsgnjx.ps f16, f2, f16\n" 
			"la x10, rand_int_32\n"
			"flw.ps f16, 928(x10)\n"
			"flw.ps f2, 160(x10)\n"
			"fltu.pi f2, f16, f2\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x7, rand_ieee754_32\n"
			"flw.ps f29, 768(x7)\n"
			"flw.ps f3, 960(x7)\n"
			"flw.ps f15, 288(x7)\n"
			"fmadd.ps f14, f29, f3, f15, rup\n" 
			"LBL_SEQID_5_M0_WRITE_ID_14_HID_6:\n"
			"masknot m3, m3\n" 
			"li x5, 0x80340188\n"
			"ld x5, 0(x5)\n"
			"la x11, rand_ieee754_16\n"
			"flw.ps f27, 320(x11)\n"
			"fcvt.ps.f16 f26, f27\n" 
			"la x10, rand_int_32\n"
			"flw.ps f7, 288(x10)\n"
			"flw.ps f16, 992(x10)\n"
			"fminu.pi f15, f7, f16\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f18, 480(x19)\n"
			"flw.ps f15, 544(x19)\n"
			"fcmovm.ps f11, f18, f15\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f8, 320(x25)\n"
			"flw.ps f17, 928(x25)\n"
			"flw.ps f16, 768(x25)\n"
			"fnmadd.ps f17, f8, f17, f16, rmm\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f11, 704(x11)\n"
			"flw.ps f20, 768(x11)\n"
			"fnmsub.s f29, f11, f20, f11, rne\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f8, 704(x13)\n"
			"flw.ps f2, 928(x13)\n"
			"fle.s x13, f8, f2\n" 
			"LBL_SEQID_5_M0_WRITE_ID_15_HID_6:\n"
			"mov.m.x m7, x5, 0x2e\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0xacee29c36e\n"
			"fsw.ps f0, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x19, rand_int_32\n"
			"flw.ps f11, 320(x19)\n"
			"flw.ps f5, 64(x19)\n"
			"flt.pi f18, f11, f5\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f29, 736(x9)\n"
			"flw.ps f20, 256(x9)\n"
			"fsgnjn.s f27, f29, f20\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f11, 384(x9)\n"
			"ffrc.ps f17, f11\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f3, 160(x13)\n"
			"flw.ps f26, 928(x13)\n"
			"flw.ps f20, 960(x13)\n"
			"fnmsub.s f18, f3, f26, f20, rup\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f30, 0(x19)\n"
			"flw.ps f14, 512(x19)\n"
			"fsgnjx.ps f5, f30, f14\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_5_TLD_XMA_STR_SEQ_ITER_0_HID_6:\n"
			"la x31, self_check_8\n"
			"li x5, 0x6\n"
			"add x5, x5, x31\n"
			"li x6, 0x30000000000000e\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x1 << TENSOR_IMA_B_NUM_COLS) | (0x6 << TENSOR_IMA_A_NUM_ROWS) | (0xe << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x18 << TENSOR_IMA_SCP_LOC_B) | (0x0 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
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
			"bne x5, x31, LBL_FAIL_HID_6\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_6\n"
			"csrw tensor_error, x0\n"
			"li x5, 6\n" 
			"bne x30, x5, LBL_FAIL_HID_6\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_6_M0_WRITE_ID_16_HID_6:\n"
			"masknot m6, m7\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"maskpopc x13, m7\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f26, 320(x25)\n"
			"ffrc.ps f2, f26\n" 
			"li x5, 0x87ca\n"
			"csrw tensor_mask, x5\n"	
			"la x26, rand_ieee754_32\n"
			"flw.ps f30, 960(x26)\n"
			"flw.ps f7, 672(x26)\n"
			"fcmovm.ps f16, f30, f7\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f8, 992(x9)\n"
			"fcvt.f16.ps f18, f8\n" 
			"la x10, rand_int_32\n"
			"flw.ps f27, 416(x10)\n"
			"fslli.pi f7, f27, 0x1\n" 
			"la x5, rand_int_32\n"
			"flw.ps f2, 928(x5)\n"
			"flw.ps f8, 192(x5)\n"
			"fmulh.pi f30, f2, f8\n" 
			"LBL_SEQID_6_M0_WRITE_ID_17_HID_6:\n"
			"maskor m5, m3, m5\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0xe9512f558b\n"
			"fsw.ps f0, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x19, rand_int_32\n"
			"flw.ps f11, 64(x19)\n"
			"fsrli.pi f7, f11, 0xe\n" 
			"LBL_SEQID_6_FP_TRANS_ID_18_HID_6:\n"
			"la x10, rand_ieee754_32\n"
			"flw.ps f18, 512(x10)\n"
			"flog.ps f17, f18\n" 
			"LBL_SEQID_6_M0_WRITE_ID_19_HID_6:\n"
			"maskxor m3, m3, m7\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_6_FP_TRANS_ID_20_HID_6:\n"
			"la x5, rand_ieee754_32\n"
			"flw.ps f11, 224(x5)\n"
			"fexp.ps f2, f11\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f30, 192(x16)\n"
			"flw.ps f20, 96(x16)\n"
			"flt.s x9, f30, f20\n" 
			"la x7, rand_int_32\n"
			"flw.ps f26, 256(x7)\n"
			"flw.ps f29, 864(x7)\n"
			"fsra.pi f26, f26, f29\n" 
			"la x25, rand_int_32\n"
			"flw.ps f8, 928(x25)\n"
			"flw.ps f7, 576(x25)\n"
			"fmulhu.pi f11, f8, f7\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f27, 576(x13)\n"
			"frsq.ps f2, f27\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_6_XMA_QNT_STR_SEQ_ITER_0_HID_6:\n"
			"li x5, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x0 << TENSOR_FMA16_B_NUM_COLS) | (0xf << TENSOR_FMA16_A_NUM_ROWS) | (0x7 << TENSOR_FMA16_A_NUM_COLS) | (0x2 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x43 << TENSOR_FMA16_SCP_LOC_B) | (0x79 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x0 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"LBL_SEQID_6_XMA_QNT_STR_SEQ_ITER_1_HID_6:\n"
			"li x5, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x3 << TENSOR_FMA16_B_NUM_COLS) | (0x7 << TENSOR_FMA16_A_NUM_ROWS) | (0x2 << TENSOR_FMA16_A_NUM_COLS) | (0xa << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x9c << TENSOR_FMA16_SCP_LOC_B) | (0xb0 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
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
			"csrwi mhpmevent3, 2\n"   
			"csrwi mhpmevent4, 1\n"   
			"csrwi mhpmevent5, 3\n"   
			"csrwi mhpmevent6, 6\n"   
			"csrwi mhpmevent7, 22\n"   
			"csrwi mhpmevent8, 2\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f9,  768 (x5)\n"
			"flw.ps f17,  64 (x5)\n"
			"flw.ps f18,  928 (x5)\n"
			"flw.ps f22,  32 (x5)\n"
			"flw.ps f14,  672 (x5)\n"
			"flw.ps f21,  960 (x5)\n"
			"flw.ps f20,  224 (x5)\n"
			"flw.ps f4,  992 (x5)\n"
			"flw.ps f8,  832 (x5)\n"
			"flw.ps f12,  448 (x5)\n"
			"flw.ps f25,  704 (x5)\n"
			"flw.ps f15,  896 (x5)\n"
			"flw.ps f28,  512 (x5)\n"
			"flw.ps f5,  384 (x5)\n"
			"flw.ps f30,  320 (x5)\n"
			"flw.ps f2,  800 (x5)\n"
			"flw.ps f19,  160 (x5)\n"
			"flw.ps f31,  352 (x5)\n"
			"flw.ps f3,  256 (x5)\n"
			"flw.ps f0,  640 (x5)\n"
			"flw.ps f10,  608 (x5)\n"
			"flw.ps f23,  192 (x5)\n"
			"flw.ps f1,  480 (x5)\n"
			"flw.ps f16,  864 (x5)\n"
			"flw.ps f7,  96 (x5)\n"
			"flw.ps f24,  736 (x5)\n"
			"flw.ps f27,  576 (x5)\n"
			"flw.ps f26,  0 (x5)\n"
			"flw.ps f6,  288 (x5)\n"
			"flw.ps f29,  416 (x5)\n"
			"flw.ps f11,  128 (x5)\n"
			"flw.ps f13,  544 (x5)\n"
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
			"la x5, rand_int_32\n"
			"flw.ps f6, 352(x5)\n"
			"flw.ps f29, 448(x5)\n"
			"feq.pi f24, f6, f29\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f24, 576(x14)\n"
			"flw.ps f27, 416(x14)\n"
			"fsgnjn.s f6, f24, f27\n" 
			"la x5, rand_int_32\n"
			"flw.ps f2, 320(x5)\n"
			"flw.ps f10, 384(x5)\n"
			"fsll.pi f23, f2, f10\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f12, 192(x11)\n"
			"fclass.ps f10, f12\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_7:\n"
			"maskand m3, m5, m7\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x15, rand_ieee754_32\n"
			"flw.ps f15, 512(x15)\n"
			"feq.ps f13, f15, f15\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f15, 160(x13)\n"
			"flw.ps f29, 832(x13)\n"
			"fsgnjn.s f13, f15, f29\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_7:\n"
			"fltm.pi m7, f12, f29\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_0_M0_WRITE_ID_2_HID_7:\n"
			"mova.m.x x28\n" 
			"li x5, CREDINC2\n"
			"ld x5, 0(x5)\n"
			"la x18, rand_int_32\n"
			"flw.ps f29, 800(x18)\n"
			"flw.ps f10, 832(x18)\n"
			"fsrl.pi f3, f29, f10\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f29, 128(x23)\n"
			"flw.ps f9, 0(x23)\n"
			"flw.ps f15, 608(x23)\n"
			"fnmsub.s f10, f29, f9, f15, rmm\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f16, 192(x11)\n"
			"flw.ps f2, 608(x11)\n"
			"fnmadd.s f2, f16, f2, f2, rne\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f9, 800(x6)\n"
			"flw.ps f12, 928(x6)\n"
			"flw.ps f2, 256(x6)\n"
			"fcmov.ps f27, f9, f12, f2\n" 
			"li x15, 0x6f8386e664e9695d\n"
			"fcvt.s.w f2, x15, rup\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f22, 192(x22)\n"
			"flw.ps f31, 160(x22)\n"
			"fsgnjx.s f6, f22, f31\n" 
			"li x10, 0xb3b9fcac5aed8886\n"
			"fcvt.s.wu f2, x10, rtz\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_7\n"
			"addi x30, x30, 1\n"
			"la x14, rand_int_32\n"
			"flw.ps f2, 384(x14)\n"
			"flw.ps f3, 800(x14)\n"
			"fsrl.pi f13, f2, f3\n" 
			"la x15, rand_int_32\n"
			"flw.ps f12, 768(x15)\n"
			"faddi.pi f12, f12, 0x1c5\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f10, 32(x23)\n"
			"flw.ps f9, 928(x23)\n"
			"fsgnj.s f27, f10, f9\n" 
			"la x22, rand_int_32\n"
			"flw.ps f29, 832(x22)\n"
			"fpackrepb.pi f19, f29\n" 
			"fbci.ps f27, 0x7c098\n" 
			"la x5, rand_int_32\n"
			"flw.ps f29, 64(x5)\n"
			"flw.ps f2, 928(x5)\n"
			"feq.pi f6, f29, f2\n" 
			"la x22, rand_int_32\n"
			"flw.ps f22, 864(x22)\n"
			"fcvt.ps.pw f13, f22, rtz\n" 
			"la x5, rand_int_32\n"
			"flw.ps f10, 288(x5)\n"
			"fpackreph.pi f6, f10\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f19, 160(x5)\n"
			"flw.ps f13, 0(x5)\n"
			"flw.ps f2, 576(x5)\n"
			"fmsub.s f22, f19, f13, f2, rtz\n" 
			"la x5, rand_int_32\n"
			"flw.ps f10, 160(x5)\n"
			"flw.ps f3, 672(x5)\n"
			"fmax.pi f9, f10, f3\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f24, 32(x5)\n"
			"flw.ps f23, 480(x5)\n"
			"fcmov.ps f24, f24, f23, f23\n" 
			"la x11, rand_int_32\n"
			"flw.ps f10, 224(x11)\n"
			"fsat8.pi f10, f10\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f12, 96(x28)\n"
			"flw.ps f3, 448(x28)\n"
			"flw.ps f10, 0(x28)\n"
			"fnmsub.ps f29, f12, f3, f10, rtz\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f16, 416(x11)\n"
			"flw.ps f19, 32(x11)\n"
			"flw.ps f13, 480(x11)\n"
			"fmsub.s f2, f16, f19, f13, rup\n" 
			"la x22, rand_int_32\n"
			"flw.ps f2, 384(x22)\n"
			"for.pi f12, f2, f2\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f22, 0(x13)\n"
			"flw.ps f16, 768(x13)\n"
			"fadd.s f16, f22, f16, rup\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_7\n"
			"addi x30, x30, 1\n"
			"la x11, rand_int_32\n"
			"flw.ps f15, 96(x11)\n"
			"fmulhu.pi f16, f15, f15\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f3, 768(x18)\n"
			"flw.ps f6, 480(x18)\n"
			"feq.ps f2, f3, f6\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f16, 960(x28)\n"
			"frsq.ps f22, f16\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f23, 32(x23)\n"
			"fcvt.pwu.ps f29, f23, rup\n" 
			"la x6, rand_int_32\n"
			"flw.ps f10, 32(x6)\n"
			"flw.ps f29, 160(x6)\n"
			"fmin.pi f24, f10, f29\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f31, 576(x22)\n"
			"flw.ps f15, 640(x22)\n"
			"fle.s x18, f31, f15\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f24, 544(x15)\n"
			"flw.ps f9, 288(x15)\n"
			"fmax.s f2, f24, f9\n" 
			"la x13, rand_int_32\n"
			"flw.ps f3, 320(x13)\n"
			"flw.ps f6, 448(x13)\n"
			"fmulhu.pi f3, f3, f6\n" 
			"la x23, rand_int_32\n"
			"flw.ps f2, 960(x23)\n"
			"flw.ps f24, 960(x23)\n"
			"fltu.pi f22, f2, f24\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f22, 768(x10)\n"
			"frsq.ps f12, f22\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f22, 608(x10)\n"
			"flw.ps f13, 416(x10)\n"
			"fle.s x18, f22, f13\n" 
			"la x6, rand_ieee754_16\n"
			"flw.ps f15, 640(x6)\n"
			"fcvt.ps.f16 f3, f15\n" 
			"LBL_SEQID_2_FP_TRANS_ID_3_HID_7:\n"
			"la x18, rand_ieee754_32\n"
			"flw.ps f24, 64(x18)\n"
			"frcp.ps f23, f24\n" 
			"la x14, rand_int_32\n"
			"flw.ps f13, 864(x14)\n"
			"fxor.pi f27, f13, f13\n" 
			"la x23, rand_int_32\n"
			"flw.ps f31, 480(x23)\n"
			"flw.ps f16, 768(x23)\n"
			"fadd.pi f24, f31, f16\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f22, 288(x23)\n"
			"flw.ps f10, 64(x23)\n"
			"feq.ps f9, f22, f10\n" 
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
			"la x6, rand_int_32\n"
			"flw.ps f6, 992(x6)\n"
			"fslli.pi f31, f6, 0x6\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f31, 800(x23)\n"
			"flw.ps f15, 512(x23)\n"
			"fmul.ps f6, f31, f15, rdn\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f24, 448(x13)\n"
			"fclass.s x22, f24\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f9, 864(x6)\n"
			"flw.ps f29, 832(x6)\n"
			"feq.s x11, f9, f29\n" 
			"la x28, rand_int_32\n"
			"flw.ps f12, 288(x28)\n"
			"flw.ps f2, 640(x28)\n"
			"fxor.pi f29, f12, f2\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f6, 928(x5)\n"
			"flw.ps f27, 736(x5)\n"
			"fsgnjx.ps f9, f6, f27\n" 
			"la x23, rand_int_32\n"
			"flw.ps f19, 64(x23)\n"
			"fsat8.pi f22, f19\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f13, 288(x28)\n"
			"flw.ps f9, 864(x28)\n"
			"fsub.ps f23, f13, f9, rtz\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f9, 928(x23)\n"
			"flw.ps f27, 736(x23)\n"
			"fmin.s f31, f9, f27\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f15, 960(x6)\n"
			"flw.ps f3, 96(x6)\n"
			"flw.ps f31, 384(x6)\n"
			"fnmsub.s f19, f15, f3, f31, rtz\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f19, 928(x15)\n"
			"flw.ps f24, 832(x15)\n"
			"flw.ps f13, 256(x15)\n"
			"fnmadd.ps f15, f19, f24, f13, rtz\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f16, 320(x22)\n"
			"fcvt.pw.ps f15, f16, rmm\n" 
			"LBL_SEQID_3_M0_WRITE_ID_4_HID_7:\n"
			"maskor m7, m7, m4\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0xdf8112bc6d\n"
			"fsw.ps f0, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x11, rand_int_32\n"
			"flw.ps f13, 608(x11)\n"
			"flw.ps f12, 320(x11)\n"
			"fminu.pi f24, f13, f12\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f23, 480(x23)\n"
			"frsq.ps f10, f23\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f6, 96(x18)\n"
			"fcvt.w.s x13, f6, rup\n" 
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
			"la x13, rand_ieee754_32\n"
			"flw.ps f12, 800(x13)\n"
			"flw.ps f9, 32(x13)\n"
			"fadd.ps f22, f12, f9, rdn\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f27, 704(x28)\n"
			"flw.ps f16, 928(x28)\n"
			"flw.ps f13, 736(x28)\n"
			"fmsub.ps f27, f27, f16, f13, rne\n" 
			"fbci.pi f24, 0x19c97\n" 
			"la x18, rand_int_32\n"
			"flw.ps f19, 448(x18)\n"
			"flw.ps f2, 416(x18)\n"
			"fltu.pi f9, f19, f2\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f12, 608(x6)\n"
			"flw.ps f10, 160(x6)\n"
			"fsgnjn.s f10, f12, f10\n" 
			"la x18, rand_int_32\n"
			"flw.ps f10, 96(x18)\n"
			"flw.ps f31, 128(x18)\n"
			"fmulhu.pi f15, f10, f31\n" 
			"LBL_SEQID_4_M0_WRITE_ID_5_HID_7:\n"
			"mov.m.x m4, x5, 0x31\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0x60b7879b81\n"
			"fsw.ps f0, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"li x23, 0x93cf92dbf19dff82\n"
			"fcvt.s.wu f16, x23, rmm\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f27, 352(x23)\n"
			"flw.ps f16, 640(x23)\n"
			"fsgnjx.ps f12, f27, f16\n" 
			"fbci.ps f16, 0x28a59\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f29, 64(x22)\n"
			"fcvt.w.s x6, f29, rtz\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f24, 800(x14)\n"
			"flw.ps f6, 448(x14)\n"
			"flt.s x28, f24, f6\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f15, 128(x22)\n"
			"flw.ps f23, 512(x22)\n"
			"fsgnjn.ps f16, f15, f23\n" 
			"maskpopcz x18, m5\n" 
			"li x23, 0x5dde8896b5b8e8ff\n"
			"fcvt.s.w f29, x23, rne\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f13, 928(x28)\n"
			"fswizz.ps f29, f13, 0x14\n" 
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
			"fbci.pi f24, 0x2a31d\n" 
			"la x28, rand_int_32\n"
			"flw.ps f2, 0(x28)\n"
			"flw.ps f23, 448(x28)\n"
			"fmulhu.pi f29, f2, f23\n" 
			"la x23, rand_int_32\n"
			"flw.ps f13, 928(x23)\n"
			"flw.ps f16, 800(x23)\n"
			"fmul.pi f3, f13, f16\n" 
			"la x11, rand_int_32\n"
			"flw.ps f15, 32(x11)\n"
			"fsatu8.pi f6, f15\n" 
			"LBL_SEQID_5_M0_WRITE_ID_6_HID_7:\n"
			"masknot m3, m5\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_5_M0_WRITE_ID_7_HID_7:\n"
			"flem.ps m5, f31, f24\n" 
			"li x5, 0x803401D8\n"
			"ld x5, 0(x5)\n"
			"LBL_SEQID_5_FP_TRANS_ID_8_HID_7:\n"
			"la x13, rand_ieee754_32\n"
			"flw.ps f31, 576(x13)\n"
			"flog.ps f15, f31\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f3, 352(x14)\n"
			"flw.ps f22, 672(x14)\n"
			"fsub.s f23, f3, f22, rne\n" 
			"la x28, rand_ieee754_16\n"
			"flw.ps f24, 64(x28)\n"
			"fcvt.ps.f16 f22, f24\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f13, 512(x23)\n"
			"flw.ps f10, 576(x23)\n"
			"fmul.ps f23, f13, f10, rup\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f6, 832(x10)\n"
			"flw.ps f15, 256(x10)\n"
			"fle.s x13, f6, f15\n" 
			"LBL_SEQID_5_M0_WRITE_ID_9_HID_7:\n"
			"fsetm.pi m7, f2\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x28, rand_ieee754_32\n"
			"flw.ps f6, 832(x28)\n"
			"flw.ps f9, 704(x28)\n"
			"fsgnjn.s f19, f6, f9\n" 
			"LBL_SEQID_5_M0_WRITE_ID_10_HID_7:\n"
			"masknot m3, m5\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x10, rand_int_32\n"
			"flw.ps f19, 384(x10)\n"
			"fpackreph.pi f2, f19\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f29, 800(x11)\n"
			"frsq.ps f9, f29\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 6\n" 
			"bne x30, x5, LBL_FAIL_HID_7\n"
			"addi x30, x30, 1\n"
			"la x6, rand_int_32\n"
			"flw.ps f12, 160(x6)\n"
			"flw.ps f16, 992(x6)\n"
			"fmul.pi f12, f12, f16\n" 
			"la x10, rand_int_32\n"
			"flw.ps f27, 64(x10)\n"
			"flw.ps f24, 960(x10)\n"
			"fsra.pi f22, f27, f24\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f19, 32(x15)\n"
			"frsq.ps f27, f19\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f19, 800(x23)\n"
			"fcvt.pw.ps f12, f19, rtz\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f24, 640(x13)\n"
			"flw.ps f3, 608(x13)\n"
			"fmul.ps f9, f24, f3, rmm\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f2, 608(x13)\n"
			"frsq.ps f19, f2\n" 
			"LBL_SEQID_6_M0_WRITE_ID_11_HID_7:\n"
			"maskor m7, m4, m5\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x22, rand_int_32\n"
			"flw.ps f9, 320(x22)\n"
			"fandi.pi f10, f9, 0x67\n" 
			"LBL_SEQID_6_M0_WRITE_ID_12_HID_7:\n"
			"fltm.pi m7, f9, f9\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x10, rand_int_32\n"
			"flw.ps f19, 672(x10)\n"
			"flw.ps f15, 960(x10)\n"
			"fand.pi f22, f19, f15\n" 
			"la x10, rand_int_32\n"
			"flw.ps f15, 192(x10)\n"
			"flw.ps f12, 640(x10)\n"
			"fmin.pi f10, f15, f12\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f13, 224(x6)\n"
			"fcvt.pwu.ps f9, f13, rdn\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f15, 352(x11)\n"
			"flw.ps f13, 768(x11)\n"
			"flw.ps f16, 704(x11)\n"
			"fcmov.ps f27, f15, f13, f16\n" 
			"LBL_SEQID_6_M0_WRITE_ID_13_HID_7:\n"
			"fltm.pi m7, f23, f9\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x15, rand_ieee754_32\n"
			"flw.ps f15, 32(x15)\n"
			"ffrc.ps f15, f15\n" 
			"LBL_SEQID_6_M0_WRITE_ID_14_HID_7:\n"
			"fltm.ps m7, f16, f3\n" 
			"mov.m.x m0, x0, 0xFF\n"
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
			"csrwi mhpmevent3, 8\n"   
			"csrwi mhpmevent4, 14\n"   
			"csrwi mhpmevent5, 4\n"   
			"csrwi mhpmevent6, 17\n"   
			"csrwi mhpmevent7, 6\n"   
			"csrwi mhpmevent8, 3\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f2,  832 (x5)\n"
			"flw.ps f20,  256 (x5)\n"
			"flw.ps f0,  32 (x5)\n"
			"flw.ps f12,  736 (x5)\n"
			"flw.ps f30,  896 (x5)\n"
			"flw.ps f24,  160 (x5)\n"
			"flw.ps f1,  640 (x5)\n"
			"flw.ps f8,  768 (x5)\n"
			"flw.ps f7,  480 (x5)\n"
			"flw.ps f29,  992 (x5)\n"
			"flw.ps f15,  704 (x5)\n"
			"flw.ps f10,  576 (x5)\n"
			"flw.ps f14,  608 (x5)\n"
			"flw.ps f18,  224 (x5)\n"
			"flw.ps f3,  672 (x5)\n"
			"flw.ps f27,  96 (x5)\n"
			"flw.ps f23,  512 (x5)\n"
			"flw.ps f21,  320 (x5)\n"
			"flw.ps f13,  544 (x5)\n"
			"flw.ps f6,  864 (x5)\n"
			"flw.ps f26,  384 (x5)\n"
			"flw.ps f31,  288 (x5)\n"
			"flw.ps f28,  192 (x5)\n"
			"flw.ps f9,  64 (x5)\n"
			"flw.ps f11,  800 (x5)\n"
			"flw.ps f5,  448 (x5)\n"
			"flw.ps f19,  128 (x5)\n"
			"flw.ps f22,  960 (x5)\n"
			"flw.ps f16,  416 (x5)\n"
			"flw.ps f17,  928 (x5)\n"
			"flw.ps f25,  0 (x5)\n"
			"flw.ps f4,  352 (x5)\n"
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
			"LBL_SEQID_0_FP_TRANS_ID_0_HID_8:\n"
			"la x5, rand_ieee754_32\n"
			"flw.ps f10, 736(x5)\n"
			"flog.ps f21, f10\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f28, 576(x13)\n"
			"ffrc.ps f28, f28\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f4, 576(x13)\n"
			"fsqrt.s f14, f4, rne\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_8:\n"
			"fltm.ps m3, f21, f19\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x15, rand_ieee754_32\n"
			"flw.ps f3, 928(x15)\n"
			"fsin.ps f7, f3\n" 
			"la x20, rand_int_32\n"
			"flw.ps f24, 672(x20)\n"
			"fsrai.pi f14, f24, 0x7\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f23, 320(x5)\n"
			"flw.ps f24, 128(x5)\n"
			"flw.ps f12, 128(x5)\n"
			"fcmov.ps f7, f23, f24, f12\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f19, 576(x5)\n"
			"ffrc.ps f31, f19\n" 
			"la x5, rand_int_32\n"
			"flw.ps f21, 704(x5)\n"
			"flw.ps f4, 928(x5)\n"
			"fmul.pi f4, f21, f4\n" 
			"la x15, rand_int_32\n"
			"flw.ps f4, 768(x15)\n"
			"flw.ps f24, 288(x15)\n"
			"fsll.pi f0, f4, f24\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f7, 256(x15)\n"
			"flw.ps f14, 64(x15)\n"
			"flt.ps f4, f7, f14\n" 
			"la x7, rand_int_32\n"
			"flw.ps f19, 896(x7)\n"
			"fpackrepb.pi f21, f19\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x23, rand_ieee754_32\n"
			"flw.ps f3, 448(x23)\n"
			"flw.ps f4, 96(x23)\n"
			"fle.s x23, f3, f4\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f4, 288(x9)\n"
			"flw.ps f10, 320(x9)\n"
			"feq.s x9, f4, f10\n" 
			"la x9, rand_int_32\n"
			"flw.ps f12, 960(x9)\n"
			"flw.ps f31, 224(x9)\n"
			"fmulh.pi f14, f12, f31\n" 
			"la x23, rand_int_32\n"
			"flw.ps f5, 160(x23)\n"
			"flw.ps f2, 800(x23)\n"
			"fand.pi f24, f5, f2\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ID_890380_HID_8:\n"
			"la x31, self_check_16\n"
			"li x5, 0xc0000000000009\n"
			"add x5, x5, x31\n"
			"li x6, 0x32000000000000c\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x2 << TENSOR_FMA16_B_NUM_COLS) | (0x9 << TENSOR_FMA16_A_NUM_ROWS) | (0xc << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x19 << TENSOR_FMA16_SCP_LOC_B) | (0x6 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0x1a\n"
			"bne x5, x31, LBL_FAIL_HID_8\n"
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ID_705487_HID_8:\n"
			"la x31, self_check_32\n"
			"li x5, 0x60000000000001\n"
			"add x5, x5, x31\n"
			"li x6, 0x1000000000000a\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x3 << TENSOR_FMA32_B_NUM_COLS) | (0x1 << TENSOR_FMA32_A_NUM_ROWS) | (0xa << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x1 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1c << TENSOR_FMA32_SCP_LOC_B) | (0x3 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x8 << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0x1 << TENSOR_QUANT_QUANT_ROW) | (0x3f << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x7 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0xb\n"
			"bne x5, x31, LBL_FAIL_HID_8\n"
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_8\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_0_RDX_ID_0_SHIRE_0_HID_8:\n"
			"li x28, 0x3a0000000353000b\n"
			"csrw tensor_reduce, x28\n" 
			"csrwi tensor_wait, 9\n" 
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ID_328788_HID_8:\n"
			"li x6, ((0x0 << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0x4 << TENSOR_QUANT_QUANT_ROW) | (0x1d << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x3 << TENSOR_QUANT_TRANSF6) | (0x5 << TENSOR_QUANT_TRANSF5) | (0x9 << TENSOR_QUANT_TRANSF4) | (0x3 << TENSOR_QUANT_TRANSF3) | (0x4 << TENSOR_QUANT_TRANSF2) | (0xa << TENSOR_QUANT_TRANSF1) | (0x5 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
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
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ITER_0_HID_8:\n"
			"la x31, self_check_8\n"
			"li x5, 0x40000000000000\n"
			"add x5, x5, x31\n"
			"li x6, 0x3c000000000000b\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x2 << TENSOR_IMA_B_NUM_COLS) | (0x0 << TENSOR_IMA_A_NUM_ROWS) | (0xb << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1e << TENSOR_IMA_SCP_LOC_B) | (0x2 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fmvz.x.ps x31, f0, 0\n"
			"li x5, 0x30\n"
			"bne x5, x31, LBL_FAIL_HID_8\n"
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ITER_1_HID_8:\n"
			"la x31, self_check_32\n"
			"li x5, 0x60000000000008\n"
			"add x5, x5, x31\n"
			"li x6, 0x34000000000000a\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x3 << TENSOR_FMA32_B_NUM_COLS) | (0x8 << TENSOR_FMA32_A_NUM_ROWS) | (0xa << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1a << TENSOR_FMA32_SCP_LOC_B) | (0x3 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0xb\n"
			"bne x5, x31, LBL_FAIL_HID_8\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_8\n"
			"csrw tensor_error, x0\n"
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_8\n"
			"addi x30, x30, 1\n"
			"maskpopcz x28, m3\n" 
			"la x5, rand_int_32\n"
			"flw.ps f20, 960(x5)\n"
			"flw.ps f19, 160(x5)\n"
			"fsll.pi f2, f20, f19\n" 
			"la x20, rand_int_32\n"
			"flw.ps f7, 960(x20)\n"
			"flw.ps f23, 0(x20)\n"
			"fminu.pi f3, f7, f23\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f28, 416(x9)\n"
			"flw.ps f21, 896(x9)\n"
			"fmul.ps f31, f28, f21, rdn\n" 
			"la x9, rand_int_32\n"
			"flw.ps f14, 192(x9)\n"
			"flw.ps f21, 128(x9)\n"
			"fadd.pi f12, f14, f21\n" 
			"la x7, rand_int_32\n"
			"flw.ps f20, 384(x7)\n"
			"faddi.pi f23, f20, 0x147\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f23, 160(x27)\n"
			"fcvt.f16.ps f20, f23\n" 
			"la x23, rand_int_32\n"
			"flw.ps f0, 96(x23)\n"
			"fadd.pi f7, f0, f0\n" 
			"la x7, rand_int_32\n"
			"flw.ps f7, 832(x7)\n"
			"flw.ps f20, 192(x7)\n"
			"fltu.pi f10, f7, f20\n" 
			"maskpopc x13, m7\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f19, 672(x19)\n"
			"fswizz.ps f23, f19, 0x9\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f4, 928(x15)\n"
			"flw.ps f23, 288(x15)\n"
			"flw.ps f0, 864(x15)\n"
			"fmsub.s f19, f4, f23, f0, rup\n" 
			"la x5, rand_int_32\n"
			"flw.ps f19, 128(x5)\n"
			"flw.ps f28, 992(x5)\n"
			"fremu.pi f31, f19, f28\n" 
			"la x5, rand_int_32\n"
			"flw.ps f31, 320(x5)\n"
			"fnot.pi f24, f31\n" 
			"la x9, rand_int_32\n"
			"flw.ps f2, 64(x9)\n"
			"flw.ps f21, 512(x9)\n"
			"fsra.pi f28, f2, f21\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x5, rand_ieee754_32\n"
			"flw.ps f28, 960(x5)\n"
			"fsin.ps f10, f28\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_1_RDX_ID_1_SHIRE_0_HID_8:\n"
			"li x9, 0x300000000738000b\n"
			"csrw tensor_reduce, x9\n" 
			"csrwi tensor_wait, 9\n" 
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ID_542565_HID_8:\n"
			"li x6, ((0x13 << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0xc << TENSOR_QUANT_QUANT_ROW) | (0x18 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x7 << TENSOR_QUANT_TRANSF7) | (0x6 << TENSOR_QUANT_TRANSF6) | (0x7 << TENSOR_QUANT_TRANSF5) | (0x6 << TENSOR_QUANT_TRANSF4) | (0x6 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
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
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ITER_0_HID_8:\n"
			"li x6, ((0x13 << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0x2 << TENSOR_QUANT_QUANT_ROW) | (0xc << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x1 << TENSOR_QUANT_TRANSF3) | (0x3 << TENSOR_QUANT_TRANSF2) | (0x4 << TENSOR_QUANT_TRANSF1) | (0x8 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_8\n"
			"csrw tensor_error, x0\n"
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_8\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_2_M0_WRITE_ID_2_HID_8:\n"
			"maskxor m3, m2, m6\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x7, rand_ieee754_32\n"
			"flw.ps f10, 352(x7)\n"
			"fclass.s x15, f10\n" 
			"la x28, rand_int_32\n"
			"flw.ps f14, 768(x28)\n"
			"fsrai.pi f3, f14, 0x9\n" 
			"la x28, rand_int_32\n"
			"flw.ps f31, 672(x28)\n"
			"faddi.pi f20, f31, 0xa2\n" 
			"mova.x.m x15\n" 
			"la x19, rand_int_32\n"
			"flw.ps f20, 0(x19)\n"
			"fsrli.pi f5, f20, 0xa\n" 
			"la x9, rand_int_32\n"
			"flw.ps f7, 160(x9)\n"
			"flw.ps f23, 192(x9)\n"
			"fminu.pi f7, f7, f23\n" 
			"la x20, rand_int_32\n"
			"flw.ps f21, 448(x20)\n"
			"flw.ps f19, 96(x20)\n"
			"for.pi f4, f21, f19\n" 
			"la x27, rand_int_32\n"
			"flw.ps f14, 832(x27)\n"
			"flw.ps f31, 384(x27)\n"
			"fmulh.pi f7, f14, f31\n" 
			"la x27, rand_int_32\n"
			"flw.ps f10, 512(x27)\n"
			"flw.ps f14, 192(x27)\n"
			"fltu.pi f12, f10, f14\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f7, 960(x7)\n"
			"flw.ps f12, 544(x7)\n"
			"fmin.ps f31, f7, f12\n" 
			"la x15, rand_int_32\n"
			"flw.ps f5, 800(x15)\n"
			"flw.ps f24, 320(x15)\n"
			"fdivu.pi f28, f5, f24\n" 
			"la x9, rand_int_32\n"
			"flw.ps f0, 512(x9)\n"
			"fsrli.pi f21, f0, 0x6\n" 
			"fbci.pi f10, 0x2fca0\n" 
			"la x5, rand_int_32\n"
			"flw.ps f5, 480(x5)\n"
			"flw.ps f28, 576(x5)\n"
			"fsub.pi f0, f5, f28\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f28, 800(x23)\n"
			"flw.ps f21, 992(x23)\n"
			"flw.ps f31, 608(x23)\n"
			"fnmadd.s f23, f28, f21, f31, rdn\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_2_XMA_QNT_STR_SEQ_ITER_0_HID_8:\n"
			"li x6, ((0x1 << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0x2 << TENSOR_QUANT_QUANT_ROW) | (0xa << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x2 << TENSOR_QUANT_TRANSF8) | (0x7 << TENSOR_QUANT_TRANSF7) | (0x7 << TENSOR_QUANT_TRANSF6) | (0x7 << TENSOR_QUANT_TRANSF5) | (0x7 << TENSOR_QUANT_TRANSF4) | (0x7 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 10\n"
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
			"la x15, rand_ieee754_32\n"
			"flw.ps f7, 576(x15)\n"
			"fcvt.l.s x7, f7, rmm\n" 
			"LBL_SEQID_3_M0_WRITE_ID_3_HID_8:\n"
			"mova.m.x x19\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x13, rand_ieee754_32\n"
			"flw.ps f28, 320(x13)\n"
			"flw.ps f21, 128(x13)\n"
			"flw.ps f7, 832(x13)\n"
			"fmadd.ps f7, f28, f21, f7, rtz\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f24, 736(x5)\n"
			"flw.ps f4, 96(x5)\n"
			"fle.s x28, f24, f4\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f10, 64(x27)\n"
			"flw.ps f23, 576(x27)\n"
			"flw.ps f31, 704(x27)\n"
			"fnmadd.s f14, f10, f23, f31, rne\n" 
			"li x5, 0xffb4\n"
			"csrw tensor_mask, x5\n"	
			"la x9, rand_int_32\n"
			"flw.ps f21, 416(x9)\n"
			"flt.pi f14, f21, f21\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f20, 832(x23)\n"
			"flw.ps f5, 672(x23)\n"
			"fnmadd.ps f21, f20, f20, f5, rne\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x20, rand_int_32\n"
			"flw.ps f2, 864(x20)\n"
			"flw.ps f24, 992(x20)\n"
			"fsrl.pi f14, f2, f24\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f3, 704(x23)\n"
			"frsq.ps f4, f3\n" 
			"la x19, rand_int_32\n"
			"flw.ps f5, 352(x19)\n"
			"flw.ps f2, 64(x19)\n"
			"fmulhu.pi f10, f5, f2\n" 
			"LBL_SEQID_3_M0_WRITE_ID_4_HID_8:\n"
			"maskxor m2, m7, m2\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x7, rand_ieee754_32\n"
			"flw.ps f4, 96(x7)\n"
			"fclass.s x9, f4\n" 
			"la x27, rand_int_32\n"
			"flw.ps f7, 160(x27)\n"
			"fsat8.pi f5, f7\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f24, 96(x20)\n"
			"fsin.ps f2, f24\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f21, 832(x19)\n"
			"flw.ps f4, 416(x19)\n"
			"fle.ps f4, f21, f4\n" 
			"la x9, rand_int_32\n"
			"flw.ps f14, 608(x9)\n"
			"fcvt.ps.pwu f28, f14, rmm\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_3_XMA_QNT_STR_SEQ_ITER_0_HID_8:\n"
			"li x5, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x3 << TENSOR_IMA_B_NUM_COLS) | (0x1 << TENSOR_IMA_A_NUM_ROWS) | (0x2 << TENSOR_IMA_A_NUM_COLS) | (0xa << TENSOR_IMA_OFFSET) | (0x0 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0xe6 << TENSOR_IMA_SCP_LOC_B) | (0xbb << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"LBL_SEQID_3_XMA_QNT_STR_SEQ_ITER_1_HID_8:\n"
			"li x6, ((0x2 << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0x8 << TENSOR_QUANT_QUANT_ROW) | (0x8 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x1 << TENSOR_QUANT_TRANSF7) | (0x5 << TENSOR_QUANT_TRANSF6) | (0x8 << TENSOR_QUANT_TRANSF5) | (0x9 << TENSOR_QUANT_TRANSF4) | (0x9 << TENSOR_QUANT_TRANSF3) | (0x8 << TENSOR_QUANT_TRANSF2) | (0x8 << TENSOR_QUANT_TRANSF1) | (0x9 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 7\n"
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
			"la x23, rand_int_32\n"
			"flw.ps f12, 288(x23)\n"
			"flw.ps f23, 640(x23)\n"
			"fsub.pi f3, f12, f23\n" 
			"li x5, 0x6d4f\n"
			"csrw tensor_mask, x5\n"	
			"la x7, rand_int_32\n"
			"flw.ps f3, 992(x7)\n"
			"flw.ps f19, 896(x7)\n"
			"fmaxu.pi f10, f3, f19\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f5, 768(x27)\n"
			"flw.ps f24, 768(x27)\n"
			"flw.ps f10, 96(x27)\n"
			"fnmadd.s f21, f5, f24, f10, rmm\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f31, 800(x9)\n"
			"flw.ps f14, 544(x9)\n"
			"fmin.s f7, f31, f14\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f0, 320(x27)\n"
			"ffrc.ps f5, f0\n" 
			"la x23, rand_int_32\n"
			"flw.ps f21, 640(x23)\n"
			"fnot.pi f7, f21\n" 
			"la x19, rand_int_32\n"
			"flw.ps f20, 608(x19)\n"
			"flw.ps f0, 288(x19)\n"
			"fremu.pi f10, f20, f0\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f12, 480(x7)\n"
			"fclass.s x28, f12\n" 
			"LBL_SEQID_4_FP_TRANS_ID_5_HID_8:\n"
			"la x15, rand_ieee754_32\n"
			"flw.ps f31, 320(x15)\n"
			"fexp.ps f2, f31\n" 
			"la x27, rand_int_32\n"
			"flw.ps f0, 448(x27)\n"
			"fcvt.ps.pwu f21, f0, rmm\n" 
			"LBL_SEQID_4_M0_WRITE_ID_6_HID_8:\n"
			"fltm.pi m7, f24, f12\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_4_M0_WRITE_ID_7_HID_8:\n"
			"mova.m.x x9\n" 
			"li x5, CREDINC2\n"
			"ld x5, 0(x5)\n"
			"la x27, rand_int_32\n"
			"flw.ps f20, 416(x27)\n"
			"fsrl.pi f21, f20, f20\n" 
			"la x9, rand_int_32\n"
			"flw.ps f21, 992(x9)\n"
			"flw.ps f5, 448(x9)\n"
			"fxor.pi f12, f21, f5\n" 
			"la x13, rand_int_32\n"
			"flw.ps f10, 288(x13)\n"
			"flw.ps f12, 864(x13)\n"
			"fsrl.pi f19, f10, f12\n" 
			"li x5, 0x8d1d\n"
			"csrw tensor_mask, x5\n"	
			"la x27, rand_ieee754_32\n"
			"flw.ps f24, 192(x27)\n"
			"fsin.ps f19, f24\n" 
			"csrw tensor_error, zero\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"LBL_SEQID_4_XMA_QNT_STR_SEQ_ITER_0_HID_8:\n"
			"li x5, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x2 << TENSOR_IMA_B_NUM_COLS) | (0x1 << TENSOR_IMA_A_NUM_ROWS) | (0x9 << TENSOR_IMA_A_NUM_COLS) | (0xc << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x65 << TENSOR_IMA_SCP_LOC_B) | (0xf7 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"LBL_SEQID_4_XMA_QNT_STR_SEQ_ITER_1_HID_8:\n"
			"li x5, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x2 << TENSOR_FMA32_B_NUM_COLS) | (0x4 << TENSOR_FMA32_A_NUM_ROWS) | (0x8 << TENSOR_FMA32_A_NUM_COLS) | (0x1 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x35 << TENSOR_FMA32_SCP_LOC_B) | (0x9c << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x0 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
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
			"la x27, rand_ieee754_32\n"
			"flw.ps f23, 928(x27)\n"
			"flw.ps f2, 736(x27)\n"
			"flw.ps f19, 960(x27)\n"
			"fmsub.ps f10, f23, f2, f19, rmm\n" 
			"LBL_SEQID_5_FP_TRANS_ID_8_HID_8:\n"
			"la x5, rand_ieee754_32\n"
			"flw.ps f12, 320(x5)\n"
			"fexp.ps f28, f12\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f7, 960(x19)\n"
			"fcvt.pwu.ps f19, f7, rtz\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f2, 384(x15)\n"
			"flw.ps f3, 160(x15)\n"
			"fmin.s f0, f2, f3\n" 
			"maskpopc x23, m2\n" 
			"la x23, rand_int_32\n"
			"flw.ps f21, 928(x23)\n"
			"fcvt.ps.pw f4, f21, rmm\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f20, 960(x5)\n"
			"flw.ps f19, 384(x5)\n"
			"flw.ps f28, 672(x5)\n"
			"fnmadd.s f20, f20, f19, f28, rne\n" 
			"la x20, rand_int_32\n"
			"flw.ps f7, 448(x20)\n"
			"flw.ps f19, 384(x20)\n"
			"fsll.pi f23, f7, f19\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f14, 480(x15)\n"
			"fcvt.w.s x19, f14, rup\n" 
			"la x15, rand_int_32\n"
			"flw.ps f21, 736(x15)\n"
			"flw.ps f23, 640(x15)\n"
			"fxor.pi f24, f21, f23\n" 
			"la x28, rand_int_32\n"
			"flw.ps f14, 160(x28)\n"
			"flw.ps f7, 576(x28)\n"
			"fand.pi f0, f14, f7\n" 
			"la x7, rand_int_32\n"
			"flw.ps f4, 96(x7)\n"
			"flw.ps f10, 416(x7)\n"
			"frem.pi f20, f4, f10\n" 
			"la x9, rand_int_32\n"
			"flw.ps f2, 160(x9)\n"
			"fpackreph.pi f4, f2\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f2, 736(x9)\n"
			"fclass.s x13, f2\n" 
			"LBL_SEQID_5_M0_WRITE_ID_9_HID_8:\n"
			"flem.ps m2, f5, f31\n" 
			"li x5, CREDINC3\n"
			"ld x5, 0(x5)\n"
			"la x20, rand_ieee754_32\n"
			"flw.ps f19, 608(x20)\n"
			"flw.ps f5, 768(x20)\n"
			"fmin.s f31, f19, f5\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 0x2ce5\n"
			"csrw tensor_mask, x5\n"	
			"LBL_SEQID_5_XMA_QNT_STR_SEQ_ITER_0_HID_8:\n"
			"li x5, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x0 << TENSOR_FMA16_B_NUM_COLS) | (0xe << TENSOR_FMA16_A_NUM_ROWS) | (0x5 << TENSOR_FMA16_A_NUM_COLS) | (0x3 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x28 << TENSOR_FMA16_SCP_LOC_B) | (0xd3 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_8\n"
			"csrw tensor_error, x0\n"
			"li x5, 6\n" 
			"bne x30, x5, LBL_FAIL_HID_8\n"
			"addi x30, x30, 1\n"
			"la x5, rand_int_32\n"
			"flw.ps f19, 224(x5)\n"
			"fsat8.pi f10, f19\n" 
			"la x5, rand_int_32\n"
			"flw.ps f23, 704(x5)\n"
			"flw.ps f21, 608(x5)\n"
			"fand.pi f14, f23, f21\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x23, rand_int_32\n"
			"flw.ps f31, 768(x23)\n"
			"flw.ps f23, 544(x23)\n"
			"for.pi f12, f31, f23\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f4, 992(x28)\n"
			"fmul.ps f21, f4, f4, rtz\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f12, 448(x19)\n"
			"fcvt.f16.ps f20, f12\n" 
			"la x15, rand_int_32\n"
			"flw.ps f12, 160(x15)\n"
			"flw.ps f0, 512(x15)\n"
			"fmulh.pi f10, f12, f0\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f14, 832(x9)\n"
			"flw.ps f4, 256(x9)\n"
			"fsub.s f10, f14, f4, rmm\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f14, 192(x15)\n"
			"flw.ps f4, 576(x15)\n"
			"fsgnjx.ps f7, f14, f4\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f10, 64(x13)\n"
			"flw.ps f28, 320(x13)\n"
			"fmin.s f20, f10, f28\n" 
			"la x9, rand_int_32\n"
			"flw.ps f31, 992(x9)\n"
			"flw.ps f28, 32(x9)\n"
			"fand.pi f4, f31, f28\n" 
			"la x20, rand_int_32\n"
			"flw.ps f24, 512(x20)\n"
			"fcvt.ps.pwu f24, f24, rup\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f4, 832(x9)\n"
			"flw.ps f14, 0(x9)\n"
			"fsgnj.ps f10, f4, f14\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f2, 608(x28)\n"
			"flw.ps f0, 0(x28)\n"
			"fsgnjn.s f14, f2, f0\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f2, 992(x19)\n"
			"fcvt.pwu.ps f12, f2, rmm\n" 
			"la x27, rand_int_32\n"
			"flw.ps f5, 32(x27)\n"
			"fcvt.ps.pwu f24, f5, rne\n" 
			"la x23, rand_int_32\n"
			"flw.ps f24, 768(x23)\n"
			"flw.ps f19, 192(x23)\n"
			"fsrl.pi f2, f24, f19\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_6_TLD_XMA_STR_SEQ_ITER_0_HID_8:\n"
			"la x31, self_check_32\n"
			"li x5, 0x80000000000005\n"
			"add x5, x5, x31\n"
			"li x6, 0x1000000000000d\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x0 << TENSOR_FMA32_B_NUM_COLS) | (0x5 << TENSOR_FMA32_A_NUM_ROWS) | (0xd << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x1 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x18 << TENSOR_FMA32_SCP_LOC_B) | (0x4 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x14 << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0x5 << TENSOR_QUANT_QUANT_ROW) | (0xa << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x2 << TENSOR_QUANT_TRANSF7) | (0x6 << TENSOR_QUANT_TRANSF6) | (0x6 << TENSOR_QUANT_TRANSF5) | (0x6 << TENSOR_QUANT_TRANSF4) | (0x7 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0xe\n"
			"bne x5, x31, LBL_FAIL_HID_8\n"
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 10\n"
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
			"csrwi mhpmevent3, 2\n"   
			"csrwi mhpmevent4, 1\n"   
			"csrwi mhpmevent5, 3\n"   
			"csrwi mhpmevent6, 6\n"   
			"csrwi mhpmevent7, 22\n"   
			"csrwi mhpmevent8, 2\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f6,  992 (x5)\n"
			"flw.ps f23,  800 (x5)\n"
			"flw.ps f13,  640 (x5)\n"
			"flw.ps f14,  448 (x5)\n"
			"flw.ps f16,  736 (x5)\n"
			"flw.ps f29,  832 (x5)\n"
			"flw.ps f31,  384 (x5)\n"
			"flw.ps f12,  544 (x5)\n"
			"flw.ps f22,  864 (x5)\n"
			"flw.ps f19,  768 (x5)\n"
			"flw.ps f11,  672 (x5)\n"
			"flw.ps f7,  416 (x5)\n"
			"flw.ps f3,  288 (x5)\n"
			"flw.ps f8,  32 (x5)\n"
			"flw.ps f21,  928 (x5)\n"
			"flw.ps f10,  192 (x5)\n"
			"flw.ps f28,  576 (x5)\n"
			"flw.ps f4,  512 (x5)\n"
			"flw.ps f25,  704 (x5)\n"
			"flw.ps f30,  320 (x5)\n"
			"flw.ps f27,  960 (x5)\n"
			"flw.ps f17,  96 (x5)\n"
			"flw.ps f18,  256 (x5)\n"
			"flw.ps f1,  608 (x5)\n"
			"flw.ps f24,  128 (x5)\n"
			"flw.ps f20,  480 (x5)\n"
			"flw.ps f0,  224 (x5)\n"
			"flw.ps f5,  0 (x5)\n"
			"flw.ps f15,  352 (x5)\n"
			"flw.ps f26,  896 (x5)\n"
			"flw.ps f9,  64 (x5)\n"
			"flw.ps f2,  160 (x5)\n"
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
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_9:\n"
			"maskxor m5, m0, m3\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_9:\n"
			"flem.ps m3, f20, f15\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x5, rand_int_32\n"
			"flw.ps f29, 992(x5)\n"
			"flw.ps f21, 480(x5)\n"
			"fmaxu.pi f19, f29, f21\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f19, 736(x17)\n"
			"flw.ps f23, 0(x17)\n"
			"flw.ps f22, 448(x17)\n"
			"fnmsub.s f31, f19, f23, f22, rtz\n" 
			"la x15, rand_int_32\n"
			"flw.ps f29, 0(x15)\n"
			"flw.ps f21, 864(x15)\n"
			"fand.pi f21, f29, f21\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f23, 544(x25)\n"
			"ffrc.ps f9, f23\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f27, 544(x11)\n"
			"flw.ps f7, 224(x11)\n"
			"fsgnjn.ps f21, f27, f7\n" 
			"la x22, rand_int_32\n"
			"flw.ps f22, 928(x22)\n"
			"flw.ps f19, 96(x22)\n"
			"fltu.pi f23, f22, f19\n" 
			"la x15, rand_int_32\n"
			"flw.ps f8, 896(x15)\n"
			"fslli.pi f10, f8, 0xd\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f3, 32(x17)\n"
			"flw.ps f19, 736(x17)\n"
			"flw.ps f23, 864(x17)\n"
			"fmsub.s f19, f3, f19, f23, rup\n" 
			"la x22, rand_int_32\n"
			"flw.ps f8, 864(x22)\n"
			"flw.ps f10, 768(x22)\n"
			"fsub.pi f17, f8, f10\n" 
			"la x17, rand_int_32\n"
			"flw.ps f10, 256(x17)\n"
			"flw.ps f27, 800(x17)\n"
			"fminu.pi f21, f10, f27\n" 
			"la x11, rand_int_32\n"
			"flw.ps f21, 64(x11)\n"
			"flw.ps f30, 32(x11)\n"
			"fle.pi f23, f21, f30\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f10, 512(x11)\n"
			"flw.ps f7, 544(x11)\n"
			"flt.ps f23, f10, f7\n" 
			"fbci.ps f31, 0x3bafa\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f8, 928(x28)\n"
			"flw.ps f27, 704(x28)\n"
			"flw.ps f17, 352(x28)\n"
			"fnmadd.ps f8, f8, f27, f17, rdn\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_9\n"
			"addi x30, x30, 1\n"
			"la x28, rand_ieee754_32\n"
			"flw.ps f23, 448(x28)\n"
			"fcvt.pw.ps f19, f23, rtz\n" 
			"la x16, rand_int_32\n"
			"flw.ps f20, 192(x16)\n"
			"fandi.pi f30, f20, 0x1f5\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f19, 224(x25)\n"
			"flw.ps f9, 672(x25)\n"
			"fmadd.s f22, f19, f9, f19, rne\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f30, 928(x10)\n"
			"fsin.ps f31, f30\n" 
			"la x10, rand_int_32\n"
			"flw.ps f3, 352(x10)\n"
			"fnot.pi f29, f3\n" 
			"LBL_SEQID_1_M0_WRITE_ID_2_HID_9:\n"
			"fltm.ps m5, f29, f29\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x10, rand_ieee754_32\n"
			"flw.ps f23, 256(x10)\n"
			"flw.ps f15, 704(x10)\n"
			"fsgnj.s f21, f23, f15\n" 
			"la x11, rand_int_32\n"
			"flw.ps f31, 896(x11)\n"
			"flw.ps f15, 640(x11)\n"
			"feq.pi f20, f31, f15\n" 
			"LBL_SEQID_1_M0_WRITE_ID_3_HID_9:\n"
			"masknot m3, m1\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_1_M0_WRITE_ID_4_HID_9:\n"
			"flem.ps m0, f31, f10\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x22, rand_int_32\n"
			"flw.ps f29, 960(x22)\n"
			"flw.ps f7, 640(x22)\n"
			"fsll.pi f20, f29, f7\n" 
			"la x22, rand_int_32\n"
			"flw.ps f30, 224(x22)\n"
			"flw.ps f23, 544(x22)\n"
			"fle.pi f20, f30, f23\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f23, 928(x10)\n"
			"flw.ps f15, 640(x10)\n"
			"fsub.ps f20, f23, f15, rmm\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f29, 576(x25)\n"
			"flw.ps f22, 832(x25)\n"
			"feq.s x17, f29, f22\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f9, 704(x10)\n"
			"flw.ps f31, 256(x10)\n"
			"flw.ps f19, 672(x10)\n"
			"fnmsub.ps f31, f9, f31, f19, rne\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f27, 320(x22)\n"
			"flw.ps f21, 960(x22)\n"
			"flw.ps f8, 992(x22)\n"
			"fmadd.s f17, f27, f21, f8, rdn\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_9\n"
			"addi x30, x30, 1\n"
			"la x18, rand_ieee754_32\n"
			"flw.ps f19, 608(x18)\n"
			"flw.ps f10, 672(x18)\n"
			"fdiv.ps f3, f19, f10, rup\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f15, 224(x16)\n"
			"fcvt.pw.ps f29, f15, rdn\n" 
			"la x22, rand_int_32\n"
			"flw.ps f23, 736(x22)\n"
			"flw.ps f9, 288(x22)\n"
			"fmul.pi f19, f23, f9\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f27, 576(x28)\n"
			"flw.ps f15, 736(x28)\n"
			"fadd.s f7, f27, f15, rmm\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f3, 736(x5)\n"
			"flw.ps f7, 576(x5)\n"
			"flt.ps f17, f3, f7\n" 
			"LBL_SEQID_2_FP_TRANS_ID_5_HID_9:\n"
			"la x18, rand_ieee754_32\n"
			"flw.ps f31, 0(x18)\n"
			"fexp.ps f10, f31\n" 
			"maskpopc x11, m5\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f21, 96(x15)\n"
			"flw.ps f22, 384(x15)\n"
			"flt.s x10, f21, f22\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f10, 576(x11)\n"
			"flw.ps f22, 768(x11)\n"
			"flw.ps f9, 32(x11)\n"
			"fmsub.ps f8, f10, f22, f9, rup\n" 
			"la x25, rand_int_32\n"
			"flw.ps f9, 416(x25)\n"
			"flw.ps f8, 320(x25)\n"
			"frem.pi f9, f9, f8\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f20, 32(x5)\n"
			"flw.ps f7, 640(x5)\n"
			"fsgnjn.s f3, f20, f7\n" 
			"la x25, rand_int_32\n"
			"flw.ps f10, 288(x25)\n"
			"flw.ps f9, 832(x25)\n"
			"fsub.pi f3, f10, f9\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f23, 352(x16)\n"
			"flw.ps f17, 608(x16)\n"
			"flw.ps f19, 128(x16)\n"
			"fmadd.ps f9, f23, f17, f19, rne\n" 
			"la x17, rand_int_32\n"
			"flw.ps f10, 384(x17)\n"
			"flw.ps f21, 832(x17)\n"
			"fsrl.pi f30, f10, f21\n" 
			"LBL_SEQID_2_FP_TRANS_ID_6_HID_9:\n"
			"la x18, rand_ieee754_32\n"
			"flw.ps f19, 256(x18)\n"
			"flog.ps f23, f19\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f21, 352(x17)\n"
			"flw.ps f17, 704(x17)\n"
			"flw.ps f20, 0(x17)\n"
			"fmsub.s f22, f21, f17, f20, rdn\n" 
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
			"la x11, rand_ieee754_32\n"
			"flw.ps f29, 0(x11)\n"
			"fclass.s x5, f29\n" 
			"LBL_SEQID_3_M0_WRITE_ID_7_HID_9:\n"
			"mova.m.x x25\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x15, rand_ieee754_32\n"
			"flw.ps f15, 0(x15)\n"
			"flw.ps f21, 800(x15)\n"
			"flw.ps f3, 0(x15)\n"
			"fnmadd.s f31, f15, f21, f3, rup\n" 
			"la x5, rand_int_32\n"
			"flw.ps f21, 704(x5)\n"
			"fcvt.ps.pw f31, f21, rne\n" 
			"LBL_SEQID_3_M0_WRITE_ID_8_HID_9:\n"
			"maskand m3, m3, m5\n" 
			"li x5, 0x80340118\n"
			"ld x5, 0(x5)\n"
			"la x16, rand_ieee754_32\n"
			"flw.ps f20, 320(x16)\n"
			"flw.ps f31, 64(x16)\n"
			"fsgnjn.ps f15, f20, f31\n" 
			"LBL_SEQID_3_M0_WRITE_ID_9_HID_9:\n"
			"mova.m.x x18\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_3_M0_WRITE_ID_10_HID_9:\n"
			"fltm.pi m5, f9, f15\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x5, rand_ieee754_32\n"
			"flw.ps f29, 992(x5)\n"
			"flw.ps f20, 768(x5)\n"
			"feq.s x28, f29, f20\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f23, 128(x10)\n"
			"flw.ps f17, 832(x10)\n"
			"fadd.s f22, f23, f17, rtz\n" 
			"la x17, rand_int_32\n"
			"flw.ps f21, 640(x17)\n"
			"fslli.pi f7, f21, 0x9\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f10, 192(x16)\n"
			"flw.ps f23, 352(x16)\n"
			"fmax.s f3, f10, f23\n" 
			"la x25, rand_int_32\n"
			"flw.ps f20, 512(x25)\n"
			"flw.ps f10, 0(x25)\n"
			"fdiv.pi f3, f20, f10\n" 
			"la x22, rand_int_32\n"
			"flw.ps f9, 32(x22)\n"
			"fsrli.pi f31, f9, 0x0\n" 
			"la x11, rand_int_32\n"
			"flw.ps f23, 576(x11)\n"
			"flw.ps f29, 0(x11)\n"
			"fle.pi f27, f23, f29\n" 
			"LBL_SEQID_3_M0_WRITE_ID_11_HID_9:\n"
			"mov.m.x m1, x22, 0x62\n" 
			"mov.m.x m0, x0, 0xFF\n"
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
			"LBL_SEQID_4_M0_WRITE_ID_12_HID_9:\n"
			"feqm.ps m5, f9, f30\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x15, rand_int_32\n"
			"flw.ps f21, 256(x15)\n"
			"flw.ps f17, 224(x15)\n"
			"fsrl.pi f29, f21, f17\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f10, 864(x5)\n"
			"flw.ps f27, 928(x5)\n"
			"fmax.ps f20, f10, f27\n" 
			"la x15, rand_int_32\n"
			"flw.ps f21, 672(x15)\n"
			"fslli.pi f3, f21, 0xf\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f3, 992(x5)\n"
			"flw.ps f17, 960(x5)\n"
			"fmax.s f17, f3, f17\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f10, 512(x22)\n"
			"fround.ps f31, f10, rdn\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f3, 480(x15)\n"
			"flw.ps f19, 800(x15)\n"
			"flw.ps f27, 672(x15)\n"
			"fcmov.ps f29, f3, f19, f27\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f17, 544(x16)\n"
			"ffrc.ps f15, f17\n" 
			"li x11, 0xc1b8356217629c1b\n"
			"fcvt.s.wu f8, x11, rtz\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f9, 192(x17)\n"
			"fsqrt.ps f17, f9\n" 
			"LBL_SEQID_4_M0_WRITE_ID_13_HID_9:\n"
			"fsetm.pi m5, f21\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x22, rand_ieee754_32\n"
			"flw.ps f8, 640(x22)\n"
			"fsin.ps f15, f8\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f23, 832(x11)\n"
			"flw.ps f9, 512(x11)\n"
			"fsub.ps f8, f23, f9, rmm\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f8, 320(x25)\n"
			"flw.ps f30, 448(x25)\n"
			"feq.ps f21, f8, f30\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f31, 96(x17)\n"
			"flw.ps f22, 544(x17)\n"
			"flt.ps f29, f31, f22\n" 
			"LBL_SEQID_4_M0_WRITE_ID_14_HID_9:\n"
			"fsetm.pi m0, f15\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0xFF\n"
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
			"LBL_SEQID_5_M0_WRITE_ID_15_HID_9:\n"
			"fltm.pi m3, f19, f27\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0x6c7a639d51\n"
			"fsw.ps f0, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x28, rand_int_32\n"
			"flw.ps f30, 512(x28)\n"
			"flw.ps f23, 960(x28)\n"
			"fsub.pi f21, f30, f23\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f9, 736(x22)\n"
			"flw.ps f3, 480(x22)\n"
			"fsgnj.ps f27, f9, f3\n" 
			"la x18, rand_int_32\n"
			"flw.ps f21, 224(x18)\n"
			"flw.ps f8, 64(x18)\n"
			"fmulhu.pi f3, f21, f8\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f21, 64(x18)\n"
			"flw.ps f23, 736(x18)\n"
			"flw.ps f20, 576(x18)\n"
			"fnmsub.s f17, f21, f23, f20, rne\n" 
			"la x5, rand_int_32\n"
			"flw.ps f7, 896(x5)\n"
			"flw.ps f17, 576(x5)\n"
			"fsub.pi f29, f7, f17\n" 
			"li x18, 0x24456dab307f10dd\n"
			"fcvt.s.w f21, x18, rup\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f10, 512(x18)\n"
			"flw.ps f29, 288(x18)\n"
			"feq.ps f15, f10, f29\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f8, 864(x5)\n"
			"flw.ps f21, 224(x5)\n"
			"fmax.ps f29, f8, f21\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f20, 608(x15)\n"
			"flw.ps f19, 672(x15)\n"
			"fsgnjx.ps f17, f20, f19\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f29, 384(x25)\n"
			"fle.ps f17, f29, f29\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f31, 992(x17)\n"
			"ffrc.ps f7, f31\n" 
			"la x16, rand_int_32\n"
			"flw.ps f7, 832(x16)\n"
			"fsrl.pi f3, f7, f7\n" 
			"fbci.pi f19, 0x24279\n" 
			"li x10, 0x4be16a992f7e0da0\n"
			"fcvt.s.w f19, x10, rup\n" 
			"LBL_SEQID_5_FP_TRANS_ID_16_HID_9:\n"
			"la x25, rand_ieee754_32\n"
			"flw.ps f8, 672(x25)\n"
			"fexp.ps f8, f8\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 6\n" 
			"bne x30, x5, LBL_FAIL_HID_9\n"
			"addi x30, x30, 1\n"
			"la x28, rand_ieee754_32\n"
			"flw.ps f10, 768(x28)\n"
			"flw.ps f17, 576(x28)\n"
			"flt.s x22, f10, f17\n" 
			"la x16, rand_int_32\n"
			"flw.ps f15, 672(x16)\n"
			"flw.ps f20, 448(x16)\n"
			"fmul.pi f22, f15, f20\n" 
			"la x16, rand_int_32\n"
			"flw.ps f9, 960(x16)\n"
			"faddi.pi f23, f9, 0x1f6\n" 
			"LBL_SEQID_6_M0_WRITE_ID_17_HID_9:\n"
			"maskxor m3, m1, m0\n" 
			"li x5, CREDINC3\n"
			"ld x5, 0(x5)\n"
			"la x17, rand_int_32\n"
			"flw.ps f21, 960(x17)\n"
			"flw.ps f19, 992(x17)\n"
			"fsll.pi f8, f21, f19\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f30, 736(x18)\n"
			"fclass.s x11, f30\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f20, 32(x15)\n"
			"flw.ps f8, 128(x15)\n"
			"fmin.s f31, f20, f8\n" 
			"LBL_SEQID_6_FP_TRANS_ID_18_HID_9:\n"
			"la x28, rand_ieee754_32\n"
			"flw.ps f22, 96(x28)\n"
			"flog.ps f21, f22\n" 
			"LBL_SEQID_6_FP_TRANS_ID_19_HID_9:\n"
			"la x18, rand_ieee754_32\n"
			"flw.ps f30, 64(x18)\n"
			"frcp.ps f19, f30\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f9, 96(x11)\n"
			"flw.ps f19, 896(x11)\n"
			"flw.ps f10, 128(x11)\n"
			"fnmsub.ps f10, f9, f19, f10, rtz\n" 
			"la x10, rand_int_32\n"
			"flw.ps f23, 128(x10)\n"
			"flw.ps f10, 384(x10)\n"
			"fxor.pi f30, f23, f10\n" 
			"la x15, rand_int_32\n"
			"flw.ps f15, 352(x15)\n"
			"flw.ps f19, 448(x15)\n"
			"for.pi f19, f15, f19\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f10, 896(x17)\n"
			"fcvt.f16.ps f22, f10\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f10, 160(x18)\n"
			"ffrc.ps f17, f10\n" 
			"la x16, rand_int_32\n"
			"flw.ps f31, 928(x16)\n"
			"fpackreph.pi f20, f31\n" 
			"la x22, rand_int_32\n"
			"flw.ps f20, 608(x22)\n"
			"flw.ps f29, 128(x22)\n"
			"fadd.pi f3, f20, f29\n" 
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
			"csrwi mhpmevent3, 8\n"   
			"csrwi mhpmevent4, 14\n"   
			"csrwi mhpmevent5, 4\n"   
			"csrwi mhpmevent6, 17\n"   
			"csrwi mhpmevent7, 6\n"   
			"csrwi mhpmevent8, 3\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f15,  800 (x5)\n"
			"flw.ps f13,  608 (x5)\n"
			"flw.ps f21,  192 (x5)\n"
			"flw.ps f11,  448 (x5)\n"
			"flw.ps f25,  160 (x5)\n"
			"flw.ps f7,  352 (x5)\n"
			"flw.ps f18,  320 (x5)\n"
			"flw.ps f6,  0 (x5)\n"
			"flw.ps f3,  832 (x5)\n"
			"flw.ps f28,  224 (x5)\n"
			"flw.ps f24,  736 (x5)\n"
			"flw.ps f31,  416 (x5)\n"
			"flw.ps f29,  480 (x5)\n"
			"flw.ps f23,  960 (x5)\n"
			"flw.ps f4,  992 (x5)\n"
			"flw.ps f17,  576 (x5)\n"
			"flw.ps f19,  512 (x5)\n"
			"flw.ps f30,  288 (x5)\n"
			"flw.ps f8,  928 (x5)\n"
			"flw.ps f26,  128 (x5)\n"
			"flw.ps f1,  704 (x5)\n"
			"flw.ps f27,  64 (x5)\n"
			"flw.ps f10,  768 (x5)\n"
			"flw.ps f16,  32 (x5)\n"
			"flw.ps f9,  896 (x5)\n"
			"flw.ps f14,  864 (x5)\n"
			"flw.ps f12,  384 (x5)\n"
			"flw.ps f2,  256 (x5)\n"
			"flw.ps f0,  672 (x5)\n"
			"flw.ps f22,  640 (x5)\n"
			"flw.ps f5,  96 (x5)\n"
			"flw.ps f20,  544 (x5)\n"
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
			"la x15, rand_ieee754_32\n"
			"flw.ps f20, 736(x15)\n"
			"flw.ps f23, 448(x15)\n"
			"fmadd.ps f27, f20, f20, f23, rtz\n" 
			"la x19, rand_int_32\n"
			"flw.ps f13, 832(x19)\n"
			"flw.ps f6, 768(x19)\n"
			"fsll.pi f23, f13, f6\n" 
			"la x19, rand_int_32\n"
			"flw.ps f8, 800(x19)\n"
			"flw.ps f21, 288(x19)\n"
			"fand.pi f8, f8, f21\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_10:\n"
			"mov.m.x m3, x6, 0x10\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x10, rand_int_32\n"
			"flw.ps f21, 288(x10)\n"
			"fpackreph.pi f14, f21\n" 
			"fbci.pi f27, 0x71764\n" 
			"li x5, 0x8afe\n"
			"csrw tensor_mask, x5\n"	
			"la x10, rand_int_32\n"
			"flw.ps f13, 320(x10)\n"
			"fsatu8.pi f29, f13\n" 
			"la x15, rand_int_32\n"
			"flw.ps f21, 288(x15)\n"
			"feq.pi f13, f21, f21\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_10:\n"
			"fltm.ps m3, f0, f20\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"maskpopc x13, m3\n" 
			"la x6, rand_int_32\n"
			"flw.ps f19, 768(x6)\n"
			"flw.ps f2, 224(x6)\n"
			"fmulh.pi f12, f19, f2\n" 
			"la x23, rand_int_32\n"
			"flw.ps f8, 384(x23)\n"
			"fltu.pi f12, f8, f8\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f11, 992(x12)\n"
			"flw.ps f13, 0(x12)\n"
			"fmax.s f2, f11, f13\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f8, 192(x19)\n"
			"ffrc.ps f8, f8\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f8, 960(x10)\n"
			"flw.ps f24, 32(x10)\n"
			"flw.ps f19, 864(x10)\n"
			"fmsub.ps f24, f8, f24, f19, rtz\n" 
			"fbci.pi f8, 0x69b39\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_0_RDX_ID_0_SHIRE_0_HID_10:\n"
			"li x9, 0x3a0000000353000b\n"
			"csrw tensor_reduce, x9\n" 
			"csrwi tensor_wait, 9\n" 
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=0); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ITER_0_HID_10:\n"
			"la x31, self_check_16\n"
			"li x5, 0xe000000000000a\n"
			"add x5, x5, x31\n"
			"li x6, 0x340000000000002\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x2 << TENSOR_FMA16_B_NUM_COLS) | (0xa << TENSOR_FMA16_A_NUM_ROWS) | (0x2 << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1a << TENSOR_FMA16_SCP_LOC_B) | (0x7 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x2 << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0xa << TENSOR_QUANT_QUANT_ROW) | (0x3c << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x2 << TENSOR_QUANT_TRANSF4) | (0x6 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0x6\n"
			"bne x5, x31, LBL_FAIL_HID_10\n"
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 10\n"
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ITER_1_HID_10:\n"
			"la x31, self_check_16\n"
			"li x5, 0x4000000000000c\n"
			"add x5, x5, x31\n"
			"li x6, 0x10000000000004\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x1 << TENSOR_FMA16_B_NUM_COLS) | (0xc << TENSOR_FMA16_A_NUM_ROWS) | (0x4 << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x1 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x19 << TENSOR_FMA16_SCP_LOC_B) | (0x2 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
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
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_10\n"
			"addi x30, x30, 1\n"
			"la x22, rand_ieee754_32\n"
			"flw.ps f29, 512(x22)\n"
			"fround.ps f12, f29, rdn\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f20, 448(x9)\n"
			"flw.ps f23, 0(x9)\n"
			"fsgnjn.s f23, f20, f23\n" 
			"la x12, rand_int_32\n"
			"flw.ps f29, 256(x12)\n"
			"fsrai.pi f20, f29, 0xa\n" 
			"li x22, 0x6aa2f87fd57727d\n"
			"fcvt.s.w f0, x22, rdn\n" 
			"la x19, rand_int_32\n"
			"flw.ps f14, 640(x19)\n"
			"fsatu8.pi f24, f14\n" 
			"LBL_SEQID_1_FP_TRANS_ID_2_HID_10:\n"
			"la x26, rand_ieee754_32\n"
			"flw.ps f11, 736(x26)\n"
			"frcp.ps f0, f11\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f14, 224(x15)\n"
			"flw.ps f6, 96(x15)\n"
			"flw.ps f12, 288(x15)\n"
			"fcmov.ps f19, f14, f6, f12\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f29, 0(x25)\n"
			"flw.ps f24, 288(x25)\n"
			"flw.ps f20, 960(x25)\n"
			"fmsub.ps f6, f29, f24, f20, rmm\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f19, 128(x15)\n"
			"flw.ps f11, 160(x15)\n"
			"flw.ps f23, 480(x15)\n"
			"fmadd.s f21, f19, f11, f23, rup\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f8, 128(x25)\n"
			"fsqrt.s f8, f8, rdn\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x12, rand_int_32\n"
			"flw.ps f22, 896(x12)\n"
			"flw.ps f29, 384(x12)\n"
			"for.pi f12, f22, f29\n" 
			"li x12, 0xdd46fe7402fb2ded\n"
			"fcvt.s.wu f20, x12, rne\n" 
			"la x10, rand_int_32\n"
			"flw.ps f29, 864(x10)\n"
			"flw.ps f19, 576(x10)\n"
			"feq.pi f24, f29, f19\n" 
			"la x13, rand_int_32\n"
			"flw.ps f22, 384(x13)\n"
			"fcvt.ps.pwu f21, f22, rtz\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f11, 192(x23)\n"
			"flw.ps f19, 704(x23)\n"
			"flt.ps f12, f11, f19\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f6, 672(x22)\n"
			"fcvt.pwu.ps f27, f6, rdn\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_1_RDX_ID_1_SHIRE_0_HID_10:\n"
			"li x26, 0x300000000738000b\n"
			"csrw tensor_reduce, x26\n" 
			"csrwi tensor_wait, 9\n" 
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ID_270704_HID_10:\n"
			"li x5, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x1 << TENSOR_FMA32_B_NUM_COLS) | (0xc << TENSOR_FMA32_A_NUM_ROWS) | (0x1 << TENSOR_FMA32_A_NUM_COLS) | (0xd << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0xb8 << TENSOR_FMA32_SCP_LOC_B) | (0x85 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x0 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
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
			"li x5, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x0 << TENSOR_FMA16_B_NUM_COLS) | (0x0 << TENSOR_FMA16_A_NUM_ROWS) | (0xa << TENSOR_FMA16_A_NUM_COLS) | (0x1 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x27 << TENSOR_FMA16_SCP_LOC_B) | (0x9b << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_10\n"
			"csrw tensor_error, x0\n"
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_10\n"
			"addi x30, x30, 1\n"
			"la x22, rand_ieee754_32\n"
			"flw.ps f23, 544(x22)\n"
			"flw.ps f27, 736(x22)\n"
			"fsub.ps f12, f23, f27, rtz\n" 
			"la x15, rand_int_32\n"
			"flw.ps f8, 256(x15)\n"
			"fpackreph.pi f13, f8\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f24, 768(x10)\n"
			"flw.ps f2, 608(x10)\n"
			"flw.ps f12, 960(x10)\n"
			"fnmadd.ps f14, f24, f2, f12, rne\n" 
			"la x26, rand_int_32\n"
			"flw.ps f14, 864(x26)\n"
			"flw.ps f27, 160(x26)\n"
			"fmax.pi f12, f14, f27\n" 
			"maskpopcz x25, m0\n" 
			"mov.m.x m0, x0, 0xff\n"
			"la x26, rand_int_32\n"
			"flw.ps f0, 576(x26)\n"
			"flw.ps f8, 160(x26)\n"
			"fsrl.pi f22, f0, f8\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f23, 544(x19)\n"
			"flw.ps f2, 576(x19)\n"
			"flw.ps f22, 96(x19)\n"
			"fmsub.ps f27, f23, f2, f22, rmm\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x9, rand_int_32\n"
			"flw.ps f23, 608(x9)\n"
			"flw.ps f22, 416(x9)\n"
			"fsrl.pi f11, f23, f22\n" 
			"li x6, 0xa25e2b8e1061b719\n"
			"fcvt.s.wu f2, x6, rtz\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f11, 0(x25)\n"
			"fround.ps f6, f11, rdn\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f8, 384(x10)\n"
			"flw.ps f23, 256(x10)\n"
			"flw.ps f2, 960(x10)\n"
			"fmsub.s f13, f8, f23, f2, rdn\n" 
			"la x6, rand_int_32\n"
			"flw.ps f29, 320(x6)\n"
			"flw.ps f14, 0(x6)\n"
			"fand.pi f8, f29, f14\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f23, 96(x9)\n"
			"fclass.s x25, f23\n" 
			"la x25, rand_int_32\n"
			"flw.ps f6, 704(x25)\n"
			"flw.ps f12, 576(x25)\n"
			"fand.pi f0, f6, f12\n" 
			"la x13, rand_int_32\n"
			"flw.ps f2, 576(x13)\n"
			"fsrai.pi f12, f2, 0x4\n" 
			"la x19, rand_int_32\n"
			"flw.ps f23, 64(x19)\n"
			"fcvt.ps.pwu f12, f23, rmm\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_2_XMA_QNT_STR_SEQ_ITER_0_HID_10:\n"
			"li x5, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x1 << TENSOR_IMA_B_NUM_COLS) | (0x9 << TENSOR_IMA_A_NUM_ROWS) | (0x3 << TENSOR_IMA_A_NUM_COLS) | (0x3 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x8c << TENSOR_IMA_SCP_LOC_B) | (0xad << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
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
			"la x15, rand_int_32\n"
			"flw.ps f12, 608(x15)\n"
			"fsatu8.pi f21, f12\n" 
			"la x15, rand_int_32\n"
			"flw.ps f24, 672(x15)\n"
			"fnot.pi f11, f24\n" 
			"la x12, rand_int_32\n"
			"flw.ps f20, 896(x12)\n"
			"flw.ps f27, 704(x12)\n"
			"fminu.pi f22, f20, f27\n" 
			"la x19, rand_int_32\n"
			"flw.ps f21, 832(x19)\n"
			"flw.ps f2, 608(x19)\n"
			"frem.pi f29, f21, f2\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f2, 352(x12)\n"
			"frsq.ps f19, f2\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f19, 544(x9)\n"
			"flw.ps f23, 320(x9)\n"
			"fcmovm.ps f22, f19, f23\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x19, rand_ieee754_32\n"
			"flw.ps f21, 96(x19)\n"
			"flw.ps f11, 480(x19)\n"
			"fmul.s f24, f21, f11, rmm\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f0, 288(x26)\n"
			"flw.ps f22, 416(x26)\n"
			"fsgnj.s f13, f0, f22\n" 
			"LBL_SEQID_3_FP_TRANS_ID_3_HID_10:\n"
			"la x10, rand_ieee754_32\n"
			"flw.ps f23, 416(x10)\n"
			"fexp.ps f6, f23\n" 
			"la x22, rand_int_32\n"
			"flw.ps f22, 64(x22)\n"
			"flw.ps f19, 544(x22)\n"
			"fdivu.pi f29, f22, f19\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x25, rand_ieee754_32\n"
			"flw.ps f27, 672(x25)\n"
			"fmul.ps f20, f27, f27, rmm\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f23, 288(x23)\n"
			"flw.ps f24, 320(x23)\n"
			"fsub.ps f6, f23, f24, rdn\n" 
			"la x6, rand_int_32\n"
			"flw.ps f20, 384(x6)\n"
			"flw.ps f8, 480(x6)\n"
			"fsrl.pi f11, f20, f8\n" 
			"la x6, rand_int_32\n"
			"flw.ps f13, 832(x6)\n"
			"flw.ps f21, 32(x6)\n"
			"fremu.pi f0, f13, f21\n" 
			"li x5, 0x85a0b7648225b617\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0xa593cc0cbe8fecb3\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"la x15, rand_ieee754_32\n"
			"flw.ps f19, 384(x15)\n"
			"flw.ps f14, 736(x15)\n"
			"fcmovm.ps f0, f19, f14\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f12, 160(x25)\n"
			"fswizz.ps f0, f12, 0x15\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 0x8721\n"
			"csrw tensor_mask, x5\n"	
			"LBL_SEQID_3_TLD_XMA_STR_SEQ_ITER_0_HID_10:\n"
			"la x31, self_check_32\n"
			"li x5, 0x60000000000009\n"
			"add x5, x5, x31\n"
			"li x6, 0x3e0000000000004\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x3 << TENSOR_FMA32_B_NUM_COLS) | (0x9 << TENSOR_FMA32_A_NUM_ROWS) | (0x4 << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1f << TENSOR_FMA32_SCP_LOC_B) | (0x3 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0x5\n"
			"bne x5, x31, LBL_FAIL_HID_10\n"
			"LBL_SEQID_3_TLD_XMA_STR_SEQ_ITER_1_HID_10:\n"
			"la x31, self_check_16\n"
			"li x5, 0x20000000000004\n"
			"add x5, x5, x31\n"
			"li x6, 0x10000000000007\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x3 << TENSOR_FMA16_B_NUM_COLS) | (0x4 << TENSOR_FMA16_A_NUM_ROWS) | (0x7 << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x1 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1f << TENSOR_FMA16_SCP_LOC_B) | (0x1 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x9 << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0x4 << TENSOR_QUANT_QUANT_ROW) | (0x23 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x6 << TENSOR_QUANT_TRANSF5) | (0x7 << TENSOR_QUANT_TRANSF4) | (0x6 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0x10\n"
			"bne x5, x31, LBL_FAIL_HID_10\n"
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 10\n"
			"LBL_SEQID_3_TLD_XMA_STR_SEQ_ITER_2_HID_10:\n"
			"la x31, self_check_32\n"
			"li x5, 0x3\n"
			"add x5, x5, x31\n"
			"li x6, 0x1000000000000a\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x0 << TENSOR_FMA32_B_NUM_COLS) | (0x3 << TENSOR_FMA32_A_NUM_ROWS) | (0xa << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x1 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x18 << TENSOR_FMA32_SCP_LOC_B) | (0x0 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x10 << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0x3 << TENSOR_QUANT_QUANT_ROW) | (0x21 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0xb\n"
			"bne x5, x31, LBL_FAIL_HID_10\n"
			"csrw tensor_quant, x9\n"	
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
			"la x10, rand_int_32\n"
			"flw.ps f13, 544(x10)\n"
			"flw.ps f20, 800(x10)\n"
			"fsrl.pi f6, f13, f20\n" 
			"li x5, 0xafcbd0c1480751d3\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0x4fb6294d4bdce113\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"la x23, rand_ieee754_32\n"
			"flw.ps f29, 256(x23)\n"
			"flw.ps f6, 704(x23)\n"
			"fsgnjn.s f21, f29, f6\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f11, 736(x22)\n"
			"ffrc.ps f13, f11\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f12, 256(x9)\n"
			"flw.ps f19, 288(x9)\n"
			"flw.ps f20, 480(x9)\n"
			"fmsub.s f19, f12, f19, f20, rup\n" 
			"LBL_SEQID_4_FP_TRANS_ID_4_HID_10:\n"
			"la x22, rand_ieee754_32\n"
			"flw.ps f2, 512(x22)\n"
			"fexp.ps f14, f2\n" 
			"LBL_SEQID_4_M0_WRITE_ID_5_HID_10:\n"
			"mova.m.x x10\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x12, rand_int_32\n"
			"flw.ps f0, 0(x12)\n"
			"flw.ps f13, 224(x12)\n"
			"fmaxu.pi f24, f0, f13\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f22, 832(x25)\n"
			"flw.ps f13, 576(x25)\n"
			"fle.ps f23, f22, f13\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f13, 544(x10)\n"
			"flw.ps f14, 384(x10)\n"
			"fsgnjx.s f14, f13, f14\n" 
			"LBL_SEQID_4_FP_TRANS_ID_6_HID_10:\n"
			"la x10, rand_ieee754_32\n"
			"flw.ps f23, 256(x10)\n"
			"frcp.ps f29, f23\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f29, 800(x25)\n"
			"fcvt.pw.ps f13, f29, rup\n" 
			"la x9, rand_int_32\n"
			"flw.ps f22, 320(x9)\n"
			"flw.ps f8, 192(x9)\n"
			"fsrl.pi f0, f22, f8\n" 
			"LBL_SEQID_4_M0_WRITE_ID_7_HID_10:\n"
			"maskxor m0, m3, m6\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x10, rand_ieee754_32\n"
			"flw.ps f23, 864(x10)\n"
			"fcvt.pwu.ps f2, f23, rtz\n" 
			"LBL_SEQID_4_M0_WRITE_ID_8_HID_10:\n"
			"maskxor m4, m0, m3\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x15, rand_ieee754_32\n"
			"flw.ps f27, 288(x15)\n"
			"flw.ps f6, 416(x15)\n"
			"fsgnj.ps f2, f27, f6\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_4_XMA_QNT_STR_SEQ_ITER_0_HID_10:\n"
			"li x6, ((0x10 << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0xf << TENSOR_QUANT_QUANT_ROW) | (0xf << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x7 << TENSOR_QUANT_TRANSF6) | (0x7 << TENSOR_QUANT_TRANSF5) | (0x7 << TENSOR_QUANT_TRANSF4) | (0x6 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
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
			"la x9, rand_int_32\n"
			"flw.ps f21, 672(x9)\n"
			"flw.ps f29, 0(x9)\n"
			"fmul.pi f0, f21, f29\n" 
			"la x10, rand_int_32\n"
			"flw.ps f11, 640(x10)\n"
			"fpackreph.pi f20, f11\n" 
			"fbci.ps f27, 0x455b1\n" 
			"LBL_SEQID_5_FP_TRANS_ID_9_HID_10:\n"
			"la x10, rand_ieee754_32\n"
			"flw.ps f19, 512(x10)\n"
			"frcp.ps f29, f19\n" 
			"la x6, rand_int_32\n"
			"flw.ps f20, 480(x6)\n"
			"flw.ps f0, 192(x6)\n"
			"fle.pi f6, f20, f0\n" 
			"la x6, rand_int_32\n"
			"flw.ps f21, 224(x6)\n"
			"fmax.pi f0, f21, f21\n" 
			"la x13, rand_int_32\n"
			"flw.ps f12, 896(x13)\n"
			"fandi.pi f11, f12, 0x15f\n" 
			"la x9, rand_int_32\n"
			"flw.ps f24, 96(x9)\n"
			"flw.ps f0, 576(x9)\n"
			"fxor.pi f23, f24, f0\n" 
			"la x25, rand_int_32\n"
			"flw.ps f12, 288(x25)\n"
			"fpackreph.pi f20, f12\n" 
			"la x23, rand_int_32\n"
			"flw.ps f29, 96(x23)\n"
			"flw.ps f23, 96(x23)\n"
			"fmulhu.pi f6, f29, f23\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f19, 768(x25)\n"
			"flw.ps f22, 608(x25)\n"
			"fsgnjx.ps f14, f19, f22\n" 
			"LBL_SEQID_5_M0_WRITE_ID_10_HID_10:\n"
			"flem.ps m3, f23, f20\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x10, rand_int_32\n"
			"flw.ps f12, 64(x10)\n"
			"flw.ps f20, 704(x10)\n"
			"fmin.pi f23, f12, f20\n" 
			"la x6, rand_int_32\n"
			"flw.ps f24, 800(x6)\n"
			"flw.ps f21, 128(x6)\n"
			"fltu.pi f24, f24, f21\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f21, 0(x19)\n"
			"flw.ps f0, 96(x19)\n"
			"fmul.ps f13, f21, f0, rup\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f0, 672(x23)\n"
			"flw.ps f19, 320(x23)\n"
			"flw.ps f8, 64(x23)\n"
			"fmadd.ps f23, f0, f19, f8, rdn\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"csrw tensor_error, zero\n" 
			"li x5, 0x3b63\n"
			"csrw tensor_mask, x5\n"	
			"LBL_SEQID_5_XMA_QNT_STR_SEQ_ITER_0_HID_10:\n"
			"li x6, ((0x1b << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0x5 << TENSOR_QUANT_QUANT_ROW) | (0x3d << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x0 << TENSOR_QUANT_TRANSF1) | (0x0 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_10\n"
			"csrw tensor_error, x0\n"
			"li x5, 6\n" 
			"bne x30, x5, LBL_FAIL_HID_10\n"
			"addi x30, x30, 1\n"
			"fbci.ps f11, 0x6c9b6\n" 
			"la x9, rand_int_32\n"
			"flw.ps f12, 352(x9)\n"
			"flw.ps f2, 192(x9)\n"
			"fmulhu.pi f29, f12, f2\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f0, 224(x10)\n"
			"fcvt.wu.s x19, f0, rup\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f13, 864(x26)\n"
			"flw.ps f14, 512(x26)\n"
			"feq.ps f0, f13, f14\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f8, 672(x19)\n"
			"fsgnjn.ps f21, f8, f8\n" 
			"LBL_SEQID_6_M0_WRITE_ID_11_HID_10:\n"
			"fsetm.pi m4, f0\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x22, rand_ieee754_32\n"
			"flw.ps f6, 960(x22)\n"
			"flw.ps f2, 768(x22)\n"
			"fnmsub.s f24, f6, f2, f6, rup\n" 
			"la x6, rand_int_32\n"
			"flw.ps f2, 736(x6)\n"
			"flw.ps f19, 896(x6)\n"
			"fmax.pi f6, f2, f19\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f2, 576(x15)\n"
			"flw.ps f20, 960(x15)\n"
			"fle.ps f27, f2, f20\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f6, 448(x12)\n"
			"flw.ps f27, 512(x12)\n"
			"fmul.s f6, f6, f27, rdn\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f23, 576(x12)\n"
			"flw.ps f22, 64(x12)\n"
			"fsgnjn.s f13, f23, f22\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f11, 736(x13)\n"
			"flw.ps f13, 64(x13)\n"
			"flt.ps f13, f11, f13\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f27, 448(x22)\n"
			"flw.ps f12, 0(x22)\n"
			"flw.ps f2, 704(x22)\n"
			"fmadd.ps f11, f27, f12, f2, rdn\n" 
			"li x26, 0xcac9e9e214caa572\n"
			"fcvt.s.wu f27, x26, rtz\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f19, 768(x9)\n"
			"flw.ps f14, 832(x9)\n"
			"fle.ps f24, f19, f14\n" 
			"la x9, rand_int_32\n"
			"flw.ps f11, 0(x9)\n"
			"flw.ps f14, 224(x9)\n"
			"fmaxu.pi f8, f11, f14\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_6_XMA_QNT_STR_SEQ_ITER_0_HID_10:\n"
			"li x6, ((0xd << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0xc << TENSOR_QUANT_QUANT_ROW) | (0x1f << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x7 << TENSOR_QUANT_TRANSF7) | (0x7 << TENSOR_QUANT_TRANSF6) | (0x6 << TENSOR_QUANT_TRANSF5) | (0x7 << TENSOR_QUANT_TRANSF4) | (0x6 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_6_XMA_QNT_STR_SEQ_ITER_1_HID_10:\n"
			"li x6, ((0xb << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0xc << TENSOR_QUANT_QUANT_ROW) | (0x34 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x2 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 10\n"
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
			"csrwi mhpmevent3, 2\n"   
			"csrwi mhpmevent4, 1\n"   
			"csrwi mhpmevent5, 3\n"   
			"csrwi mhpmevent6, 6\n"   
			"csrwi mhpmevent7, 22\n"   
			"csrwi mhpmevent8, 2\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f30,  96 (x5)\n"
			"flw.ps f18,  608 (x5)\n"
			"flw.ps f21,  128 (x5)\n"
			"flw.ps f12,  800 (x5)\n"
			"flw.ps f28,  928 (x5)\n"
			"flw.ps f4,  992 (x5)\n"
			"flw.ps f16,  32 (x5)\n"
			"flw.ps f27,  160 (x5)\n"
			"flw.ps f17,  256 (x5)\n"
			"flw.ps f13,  832 (x5)\n"
			"flw.ps f24,  320 (x5)\n"
			"flw.ps f10,  448 (x5)\n"
			"flw.ps f29,  0 (x5)\n"
			"flw.ps f5,  288 (x5)\n"
			"flw.ps f15,  640 (x5)\n"
			"flw.ps f25,  64 (x5)\n"
			"flw.ps f1,  704 (x5)\n"
			"flw.ps f6,  416 (x5)\n"
			"flw.ps f22,  768 (x5)\n"
			"flw.ps f31,  672 (x5)\n"
			"flw.ps f0,  224 (x5)\n"
			"flw.ps f14,  896 (x5)\n"
			"flw.ps f19,  544 (x5)\n"
			"flw.ps f3,  864 (x5)\n"
			"flw.ps f11,  192 (x5)\n"
			"flw.ps f9,  384 (x5)\n"
			"flw.ps f7,  960 (x5)\n"
			"flw.ps f2,  512 (x5)\n"
			"flw.ps f8,  480 (x5)\n"
			"flw.ps f20,  736 (x5)\n"
			"flw.ps f26,  352 (x5)\n"
			"flw.ps f23,  576 (x5)\n"
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
			"la x10, rand_ieee754_32\n"
			"flw.ps f6, 448(x10)\n"
			"fround.ps f19, f6, rdn\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_11:\n"
			"fsetm.pi m4, f3\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x26, rand_int_32\n"
			"flw.ps f25, 736(x26)\n"
			"flw.ps f24, 800(x26)\n"
			"fsra.pi f12, f25, f24\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f29, 32(x9)\n"
			"flw.ps f23, 192(x9)\n"
			"fadd.ps f31, f29, f23, rtz\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f3, 96(x12)\n"
			"flw.ps f11, 192(x12)\n"
			"fmul.ps f27, f3, f11, rne\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f30, 352(x19)\n"
			"fclass.ps f23, f30\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f5, 128(x9)\n"
			"flw.ps f3, 608(x9)\n"
			"fcmovm.ps f6, f5, f3\n" 
			"la x14, rand_int_32\n"
			"flw.ps f12, 384(x14)\n"
			"flw.ps f25, 480(x14)\n"
			"fsub.pi f6, f12, f25\n" 
			"la x19, rand_int_32\n"
			"flw.ps f6, 576(x19)\n"
			"flw.ps f25, 928(x19)\n"
			"fltu.pi f29, f6, f25\n" 
			"la x12, rand_int_32\n"
			"flw.ps f18, 192(x12)\n"
			"fsrli.pi f30, f18, 0x0\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_11:\n"
			"maskand m7, m0, m7\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x12, rand_ieee754_32\n"
			"flw.ps f11, 256(x12)\n"
			"frsq.ps f23, f11\n" 
			"la x7, rand_ieee754_16\n"
			"flw.ps f12, 384(x7)\n"
			"fcvt.ps.f16 f30, f12\n" 
			"la x19, rand_int_32\n"
			"flw.ps f9, 288(x19)\n"
			"fsra.pi f6, f9, f9\n" 
			"la x19, rand_int_32\n"
			"flw.ps f31, 736(x19)\n"
			"fsrli.pi f6, f31, 0x8\n" 
			"la x26, rand_int_32\n"
			"flw.ps f6, 800(x26)\n"
			"fsat8.pi f24, f6\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_11\n"
			"addi x30, x30, 1\n"
			"la x12, rand_int_32\n"
			"flw.ps f5, 992(x12)\n"
			"fnot.pi f25, f5\n" 
			"maskpopc x19, m0\n" 
			"mov.m.x m0, x0, 0xff\n"
			"maskpopcz x19, m7\n" 
			"la x15, rand_int_32\n"
			"flw.ps f25, 64(x15)\n"
			"fpackreph.pi f18, f25\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f31, 928(x14)\n"
			"flw.ps f18, 672(x14)\n"
			"fsgnjn.ps f9, f31, f18\n" 
			"la x15, rand_int_32\n"
			"flw.ps f23, 288(x15)\n"
			"flw.ps f12, 736(x15)\n"
			"fsll.pi f25, f23, f12\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f12, 864(x20)\n"
			"flw.ps f29, 864(x20)\n"
			"fcmovm.ps f6, f12, f29\n" 
			"la x19, rand_int_32\n"
			"flw.ps f12, 224(x19)\n"
			"flw.ps f29, 928(x19)\n"
			"feq.pi f3, f12, f29\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f9, 224(x26)\n"
			"flw.ps f11, 0(x26)\n"
			"fle.ps f25, f9, f11\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f12, 224(x10)\n"
			"fclass.ps f12, f12\n" 
			"fbci.ps f30, 0x7b716\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f9, 32(x19)\n"
			"flw.ps f25, 224(x19)\n"
			"fle.s x26, f9, f25\n" 
			"la x19, rand_int_32\n"
			"flw.ps f18, 576(x19)\n"
			"flw.ps f25, 192(x19)\n"
			"fmulhu.pi f30, f18, f25\n" 
			"LBL_SEQID_1_M0_WRITE_ID_2_HID_11:\n"
			"mov.m.x m4, x12, 0x1b\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x26, rand_ieee754_32\n"
			"flw.ps f25, 448(x26)\n"
			"flw.ps f29, 640(x26)\n"
			"flt.s x20, f25, f29\n" 
			"mova.x.m x7\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_11\n"
			"addi x30, x30, 1\n"
			"la x14, rand_ieee754_32\n"
			"flw.ps f3, 544(x14)\n"
			"flw.ps f5, 800(x14)\n"
			"fsgnj.ps f30, f3, f5\n" 
			"la x15, rand_int_32\n"
			"flw.ps f12, 160(x15)\n"
			"fnot.pi f5, f12\n" 
			"LBL_SEQID_2_M0_WRITE_ID_3_HID_11:\n"
			"maskand m0, m4, m0\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x19, rand_int_32\n"
			"flw.ps f9, 160(x19)\n"
			"fsrli.pi f24, f9, 0xb\n" 
			"LBL_SEQID_2_FP_TRANS_ID_4_HID_11:\n"
			"la x7, rand_ieee754_32\n"
			"flw.ps f5, 64(x7)\n"
			"fexp.ps f28, f5\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f11, 128(x9)\n"
			"flw.ps f23, 800(x9)\n"
			"fmax.ps f6, f11, f23\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f23, 192(x15)\n"
			"flw.ps f19, 864(x15)\n"
			"fnmsub.ps f3, f23, f19, f23, rup\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f6, 608(x15)\n"
			"flw.ps f11, 192(x15)\n"
			"fadd.s f19, f6, f11, rdn\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f19, 224(x26)\n"
			"flw.ps f3, 640(x26)\n"
			"flw.ps f28, 544(x26)\n"
			"fnmsub.s f19, f19, f3, f28, rmm\n" 
			"la x10, rand_int_32\n"
			"flw.ps f23, 608(x10)\n"
			"for.pi f28, f23, f23\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f28, 736(x23)\n"
			"flw.ps f23, 832(x23)\n"
			"fnmadd.ps f6, f28, f23, f23, rdn\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f31, 192(x19)\n"
			"fswizz.ps f24, f31, 0x2\n" 
			"la x20, rand_int_32\n"
			"flw.ps f3, 928(x20)\n"
			"flw.ps f31, 128(x20)\n"
			"fsrl.pi f28, f3, f31\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f9, 288(x19)\n"
			"flw.ps f31, 224(x19)\n"
			"fadd.ps f5, f9, f31, rtz\n" 
			"la x14, rand_int_32\n"
			"flw.ps f30, 192(x14)\n"
			"faddi.pi f24, f30, 0x7c\n" 
			"la x20, rand_int_32\n"
			"flw.ps f25, 512(x20)\n"
			"fpackrepb.pi f11, f25\n" 
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
			"la x9, rand_ieee754_32\n"
			"flw.ps f18, 864(x9)\n"
			"flw.ps f25, 928(x9)\n"
			"fmax.s f25, f18, f25\n" 
			"LBL_SEQID_3_M0_WRITE_ID_5_HID_11:\n"
			"feqm.ps m7, f6, f31\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0x794596a009\n"
			"fsw.ps f0, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_3_M0_WRITE_ID_6_HID_11:\n"
			"flem.ps m7, f3, f25\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x23, rand_ieee754_32\n"
			"flw.ps f19, 864(x23)\n"
			"flw.ps f23, 768(x23)\n"
			"fmax.ps f23, f19, f23\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f29, 64(x14)\n"
			"flw.ps f11, 544(x14)\n"
			"fsgnjx.s f6, f29, f11\n" 
			"la x9, rand_int_32\n"
			"flw.ps f29, 96(x9)\n"
			"flw.ps f3, 544(x9)\n"
			"flt.pi f23, f29, f3\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f25, 416(x7)\n"
			"flw.ps f29, 256(x7)\n"
			"fmadd.s f27, f25, f29, f25, rup\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f19, 0(x14)\n"
			"fswizz.ps f28, f19, 0x17\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f27, 896(x26)\n"
			"flw.ps f3, 768(x26)\n"
			"fsgnjn.s f31, f27, f3\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f5, 128(x15)\n"
			"flw.ps f6, 736(x15)\n"
			"fsub.s f9, f5, f6, rup\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f3, 128(x19)\n"
			"flt.s x9, f3, f3\n" 
			"la x7, rand_int_32\n"
			"flw.ps f23, 0(x7)\n"
			"flw.ps f9, 896(x7)\n"
			"fadd.pi f5, f23, f9\n" 
			"la x15, rand_int_32\n"
			"flw.ps f11, 672(x15)\n"
			"flw.ps f5, 480(x15)\n"
			"flt.pi f31, f11, f5\n" 
			"la x20, rand_int_32\n"
			"flw.ps f3, 480(x20)\n"
			"fremu.pi f29, f3, f3\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f28, 512(x26)\n"
			"flw.ps f27, 0(x26)\n"
			"flw.ps f25, 192(x26)\n"
			"fmadd.s f24, f28, f27, f25, rdn\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f23, 832(x26)\n"
			"fclass.ps f31, f23\n" 
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
			"fbci.ps f5, 0x61ab5\n" 
			"LBL_SEQID_4_M0_WRITE_ID_7_HID_11:\n"
			"fltm.ps m3, f29, f6\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0xf34814e1e8\n"
			"flw.ps f5, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x7, rand_ieee754_32\n"
			"flw.ps f5, 480(x7)\n"
			"flw.ps f19, 768(x7)\n"
			"fsub.s f27, f5, f19, rdn\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f31, 640(x20)\n"
			"flw.ps f23, 352(x20)\n"
			"fsgnjn.s f28, f31, f23\n" 
			"la x14, rand_int_32\n"
			"flw.ps f27, 160(x14)\n"
			"flw.ps f11, 224(x14)\n"
			"fsrl.pi f28, f27, f11\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f29, 256(x15)\n"
			"fdiv.s f31, f29, f29, rne\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f9, 480(x10)\n"
			"flw.ps f25, 992(x10)\n"
			"flw.ps f29, 480(x10)\n"
			"fnmsub.ps f3, f9, f25, f29, rup\n" 
			"la x20, rand_int_32\n"
			"flw.ps f25, 96(x20)\n"
			"fpackreph.pi f19, f25\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f27, 864(x20)\n"
			"ffrc.ps f27, f27\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f31, 768(x9)\n"
			"flw.ps f19, 320(x9)\n"
			"fnmsub.ps f28, f31, f19, f19, rtz\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f25, 672(x23)\n"
			"flw.ps f23, 736(x23)\n"
			"fsub.ps f24, f25, f23, rup\n" 
			"la x7, rand_int_32\n"
			"flw.ps f18, 512(x7)\n"
			"flw.ps f3, 192(x7)\n"
			"flt.pi f25, f18, f3\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f23, 800(x12)\n"
			"flw.ps f25, 96(x12)\n"
			"flw.ps f29, 768(x12)\n"
			"fmsub.ps f24, f23, f25, f29, rtz\n" 
			"la x23, rand_int_32\n"
			"flw.ps f5, 128(x23)\n"
			"flw.ps f27, 320(x23)\n"
			"feq.pi f24, f5, f27\n" 
			"la x23, rand_int_32\n"
			"flw.ps f12, 448(x23)\n"
			"flw.ps f25, 384(x23)\n"
			"fxor.pi f24, f12, f25\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f9, 704(x12)\n"
			"flw.ps f31, 128(x12)\n"
			"fmin.s f11, f9, f31\n" 
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
			"la x7, rand_ieee754_32\n"
			"flw.ps f19, 96(x7)\n"
			"frsq.ps f27, f19\n" 
			"mova.x.m x26\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f24, 544(x12)\n"
			"fsin.ps f11, f24\n" 
			"LBL_SEQID_5_M0_WRITE_ID_8_HID_11:\n"
			"fltm.ps m7, f6, f12\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x26, rand_ieee754_32\n"
			"flw.ps f12, 160(x26)\n"
			"flw.ps f31, 416(x26)\n"
			"fsgnjx.s f30, f12, f31\n" 
			"LBL_SEQID_5_M0_WRITE_ID_9_HID_11:\n"
			"masknot m4, m4\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x26, rand_int_32\n"
			"flw.ps f19, 160(x26)\n"
			"fcvt.ps.pwu f18, f19, rdn\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f9, 896(x12)\n"
			"flw.ps f11, 960(x12)\n"
			"feq.s x9, f9, f11\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f3, 448(x15)\n"
			"flw.ps f30, 928(x15)\n"
			"fle.s x10, f3, f30\n" 
			"mova.x.m x23\n" 
			"la x14, rand_int_32\n"
			"flw.ps f29, 800(x14)\n"
			"faddi.pi f25, f29, 0x1e2\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f18, 928(x7)\n"
			"flw.ps f11, 288(x7)\n"
			"fsgnjx.ps f3, f18, f11\n" 
			"la x9, rand_int_32\n"
			"flw.ps f11, 608(x9)\n"
			"flw.ps f23, 576(x9)\n"
			"flt.pi f18, f11, f23\n" 
			"la x10, rand_int_32\n"
			"flw.ps f11, 0(x10)\n"
			"flw.ps f30, 928(x10)\n"
			"fmulhu.pi f3, f11, f30\n" 
			"LBL_SEQID_5_M0_WRITE_ID_10_HID_11:\n"
			"fltm.ps m7, f11, f12\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0x36eeda5d21\n"
			"flw.ps f5, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x15, rand_ieee754_32\n"
			"flw.ps f27, 160(x15)\n"
			"flw.ps f24, 256(x15)\n"
			"fsgnj.s f24, f27, f24\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 6\n" 
			"bne x30, x5, LBL_FAIL_HID_11\n"
			"addi x30, x30, 1\n"
			"la x26, rand_ieee754_32\n"
			"flw.ps f9, 384(x26)\n"
			"flw.ps f18, 640(x26)\n"
			"flw.ps f24, 224(x26)\n"
			"fnmsub.s f12, f9, f18, f24, rne\n" 
			"la x14, rand_int_32\n"
			"flw.ps f19, 672(x14)\n"
			"faddi.pi f24, f19, 0xff\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f6, 384(x20)\n"
			"flw.ps f29, 384(x20)\n"
			"fsgnj.ps f3, f6, f29\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f27, 832(x19)\n"
			"flw.ps f28, 704(x19)\n"
			"fmax.ps f9, f27, f28\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f29, 224(x12)\n"
			"flw.ps f24, 992(x12)\n"
			"fmin.ps f11, f29, f24\n" 
			"la x20, rand_int_32\n"
			"flw.ps f23, 896(x20)\n"
			"flw.ps f3, 32(x20)\n"
			"for.pi f19, f23, f3\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f31, 576(x10)\n"
			"fcvt.w.s x10, f31, rdn\n" 
			"fbci.ps f28, 0xc575\n" 
			"li x20, 0x8617ab31c583f6b0\n"
			"fcvt.s.l f23, x20, rne\n" 
			"LBL_SEQID_6_FP_TRANS_ID_11_HID_11:\n"
			"la x20, rand_ieee754_32\n"
			"flw.ps f12, 608(x20)\n"
			"fexp.ps f12, f12\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f3, 992(x12)\n"
			"flw.ps f29, 288(x12)\n"
			"fle.ps f27, f3, f29\n" 
			"LBL_SEQID_6_M0_WRITE_ID_12_HID_11:\n"
			"feqm.ps m3, f5, f23\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0x5a8cdeb7f\n"
			"fsw.ps f0, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x10, rand_ieee754_32\n"
			"flw.ps f9, 800(x10)\n"
			"fround.ps f29, f9, rup\n" 
			"la x26, rand_int_32\n"
			"flw.ps f3, 640(x26)\n"
			"fpackrepb.pi f9, f3\n" 
			"LBL_SEQID_6_M0_WRITE_ID_13_HID_11:\n"
			"mova.m.x x7\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0xc9a2ccce3c\n"
			"fsw.ps f0, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x12, rand_ieee754_32\n"
			"flw.ps f30, 416(x12)\n"
			"flw.ps f19, 544(x12)\n"
			"flw.ps f24, 512(x12)\n"
			"fnmadd.s f5, f30, f19, f24, rup\n" 
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
			"csrwi mhpmevent3, 8\n"   
			"csrwi mhpmevent4, 14\n"   
			"csrwi mhpmevent5, 4\n"   
			"csrwi mhpmevent6, 17\n"   
			"csrwi mhpmevent7, 6\n"   
			"csrwi mhpmevent8, 3\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f23,  512 (x5)\n"
			"flw.ps f18,  288 (x5)\n"
			"flw.ps f28,  448 (x5)\n"
			"flw.ps f29,  736 (x5)\n"
			"flw.ps f7,  256 (x5)\n"
			"flw.ps f0,  576 (x5)\n"
			"flw.ps f16,  352 (x5)\n"
			"flw.ps f15,  128 (x5)\n"
			"flw.ps f27,  64 (x5)\n"
			"flw.ps f17,  832 (x5)\n"
			"flw.ps f26,  608 (x5)\n"
			"flw.ps f12,  480 (x5)\n"
			"flw.ps f10,  992 (x5)\n"
			"flw.ps f13,  768 (x5)\n"
			"flw.ps f1,  672 (x5)\n"
			"flw.ps f6,  416 (x5)\n"
			"flw.ps f19,  32 (x5)\n"
			"flw.ps f14,  896 (x5)\n"
			"flw.ps f30,  224 (x5)\n"
			"flw.ps f4,  192 (x5)\n"
			"flw.ps f11,  864 (x5)\n"
			"flw.ps f31,  640 (x5)\n"
			"flw.ps f20,  928 (x5)\n"
			"flw.ps f3,  0 (x5)\n"
			"flw.ps f9,  960 (x5)\n"
			"flw.ps f25,  96 (x5)\n"
			"flw.ps f22,  320 (x5)\n"
			"flw.ps f21,  800 (x5)\n"
			"flw.ps f5,  160 (x5)\n"
			"flw.ps f2,  544 (x5)\n"
			"flw.ps f24,  384 (x5)\n"
			"flw.ps f8,  704 (x5)\n"
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
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_12:\n"
			"fltm.ps m0, f9, f3\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0xfec3cb21d3\n"
			"fsw.ps f0, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x22, rand_ieee754_32\n"
			"flw.ps f9, 832(x22)\n"
			"flw.ps f3, 672(x22)\n"
			"fsgnjn.s f2, f9, f3\n" 
			"la x18, rand_int_32\n"
			"flw.ps f15, 992(x18)\n"
			"fsrai.pi f6, f15, 0x3\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f9, 960(x10)\n"
			"flw.ps f25, 128(x10)\n"
			"fsub.ps f19, f9, f25, rmm\n" 
			"la x20, rand_int_32\n"
			"flw.ps f31, 928(x20)\n"
			"fpackrepb.pi f2, f31\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f27, 896(x7)\n"
			"flw.ps f14, 864(x7)\n"
			"fsgnjx.s f4, f27, f14\n" 
			"la x6, rand_int_32\n"
			"flw.ps f12, 992(x6)\n"
			"flw.ps f4, 416(x6)\n"
			"for.pi f19, f12, f4\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f31, 416(x11)\n"
			"flw.ps f23, 384(x11)\n"
			"fmul.s f4, f31, f23, rup\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f4, 416(x28)\n"
			"flw.ps f1, 352(x28)\n"
			"fsub.ps f6, f4, f1, rup\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_12:\n"
			"fltm.pi m2, f2, f14\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"li x5, 0x74ec\n"
			"csrw tensor_mask, x5\n"	
			"LBL_SEQID_0_FP_TRANS_ID_2_HID_12:\n"
			"la x25, rand_ieee754_32\n"
			"flw.ps f25, 896(x25)\n"
			"frcp.ps f2, f25\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f4, 768(x25)\n"
			"flw.ps f3, 128(x25)\n"
			"fsgnjn.s f9, f4, f3\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f25, 608(x6)\n"
			"flw.ps f19, 960(x6)\n"
			"fsgnj.s f3, f25, f19\n" 
			"LBL_SEQID_0_M0_WRITE_ID_3_HID_12:\n"
			"masknot m5, m2\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_0_M0_WRITE_ID_4_HID_12:\n"
			"maskxor m1, m0, m2\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x7, rand_ieee754_32\n"
			"flw.ps f4, 704(x7)\n"
			"flw.ps f31, 704(x7)\n"
			"flw.ps f19, 320(x7)\n"
			"fmadd.s f15, f4, f31, f19, rne\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_0_RDX_ID_0_SHIRE_0_HID_12:\n"
			"li x27, 0x3a0000000353000b\n"
			"csrw tensor_reduce, x27\n" 
			"csrwi tensor_wait, 9\n" 
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=0); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ITER_0_HID_12:\n"
			"li x5, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x3 << TENSOR_IMA_B_NUM_COLS) | (0x4 << TENSOR_IMA_A_NUM_ROWS) | (0x4 << TENSOR_IMA_A_NUM_COLS) | (0x3 << TENSOR_IMA_OFFSET) | (0x0 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x72 << TENSOR_IMA_SCP_LOC_B) | (0xaf << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_12\n"
			"csrw tensor_error, x0\n"
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_12\n"
			"addi x30, x30, 1\n"
			"la x11, rand_ieee754_32\n"
			"flw.ps f22, 832(x11)\n"
			"flw.ps f2, 800(x11)\n"
			"flw.ps f25, 224(x11)\n"
			"fnmsub.s f14, f22, f2, f25, rup\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f4, 608(x11)\n"
			"flw.ps f13, 512(x11)\n"
			"fle.ps f31, f4, f13\n" 
			"la x25, rand_int_32\n"
			"flw.ps f27, 672(x25)\n"
			"flw.ps f14, 896(x25)\n"
			"fmulhu.pi f2, f27, f14\n" 
			"la x22, rand_int_32\n"
			"flw.ps f12, 704(x22)\n"
			"flw.ps f9, 448(x22)\n"
			"fsra.pi f12, f12, f9\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f3, 384(x6)\n"
			"fcvt.w.s x27, f3, rne\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f1, 0(x18)\n"
			"flt.ps f23, f1, f1\n" 
			"fbci.pi f22, 0x6cb88\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f3, 32(x25)\n"
			"fcmovm.ps f19, f3, f3\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f1, 288(x7)\n"
			"fcvt.pwu.ps f1, f1, rne\n" 
			"la x7, rand_int_32\n"
			"flw.ps f27, 480(x7)\n"
			"flw.ps f1, 672(x7)\n"
			"for.pi f13, f27, f1\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f1, 800(x27)\n"
			"flw.ps f9, 64(x27)\n"
			"fsgnjn.ps f15, f1, f9\n" 
			"li x5, 0x96cf\n"
			"csrw tensor_mask, x5\n"	
			"la x20, rand_ieee754_32\n"
			"flw.ps f23, 288(x20)\n"
			"flw.ps f14, 768(x20)\n"
			"fmin.s f22, f23, f14\n" 
			"la x7, rand_int_32\n"
			"flw.ps f9, 672(x7)\n"
			"fsrli.pi f23, f9, 0xc\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f14, 896(x11)\n"
			"flw.ps f9, 832(x11)\n"
			"fadd.ps f19, f14, f9, rup\n" 
			"la x6, rand_int_32\n"
			"flw.ps f22, 224(x6)\n"
			"fcvt.ps.pw f12, f22, rmm\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f23, 544(x28)\n"
			"flw.ps f9, 672(x28)\n"
			"feq.ps f12, f23, f9\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_1_TLD_XMA_STR_SEQ_ID_567657_HID_12:\n"
			"la x31, self_check_8\n"
			"li x5, 0x40000000000002\n"
			"add x5, x5, x31\n"
			"li x6, 0x340000000000002\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x0 << TENSOR_IMA_B_NUM_COLS) | (0x2 << TENSOR_IMA_A_NUM_ROWS) | (0x2 << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1a << TENSOR_IMA_SCP_LOC_B) | (0x2 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fmvz.x.ps x31, f0, 0\n"
			"li x5, 0xc\n"
			"bne x5, x31, LBL_FAIL_HID_12\n"
			"LBL_SEQID_1_TLD_XMA_STR_SEQ_ID_75856_HID_12:\n"
			"la x31, self_check_16\n"
			"li x5, 0x4\n"
			"add x5, x5, x31\n"
			"li x6, 0x30000000000000c\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x0 << TENSOR_FMA16_B_NUM_COLS) | (0x4 << TENSOR_FMA16_A_NUM_ROWS) | (0xc << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x18 << TENSOR_FMA16_SCP_LOC_B) | (0x0 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0x1a\n"
			"bne x5, x31, LBL_FAIL_HID_12\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_12\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_1_RDX_ID_1_SHIRE_0_HID_12:\n"
			"li x7, 0x300000000738000b\n"
			"csrw tensor_reduce, x7\n" 
			"csrwi tensor_wait, 9\n" 
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ID_702098_HID_12:\n"
			"li x6, ((0x1c << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0x9 << TENSOR_QUANT_QUANT_ROW) | (0x32 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x1 << TENSOR_QUANT_TRANSF1) | (0x4 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ITER_0_HID_12:\n"
			"li x6, ((0x1d << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0xe << TENSOR_QUANT_QUANT_ROW) | (0xd << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x6 << TENSOR_QUANT_TRANSF8) | (0x7 << TENSOR_QUANT_TRANSF7) | (0x7 << TENSOR_QUANT_TRANSF6) | (0x6 << TENSOR_QUANT_TRANSF5) | (0x6 << TENSOR_QUANT_TRANSF4) | (0x7 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_12\n"
			"csrw tensor_error, x0\n"
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_12\n"
			"addi x30, x30, 1\n"
			"la x18, rand_int_32\n"
			"flw.ps f31, 0(x18)\n"
			"fpackreph.pi f25, f31\n" 
			"la x11, rand_int_32\n"
			"flw.ps f15, 864(x11)\n"
			"flw.ps f2, 352(x11)\n"
			"fmin.pi f3, f15, f2\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f15, 160(x18)\n"
			"flw.ps f6, 256(x18)\n"
			"fsub.ps f14, f15, f6, rne\n" 
			"la x22, rand_int_32\n"
			"flw.ps f13, 896(x22)\n"
			"fslli.pi f27, f13, 0x6\n" 
			"la x6, rand_int_32\n"
			"flw.ps f1, 224(x6)\n"
			"flw.ps f15, 288(x6)\n"
			"fminu.pi f22, f1, f15\n" 
			"la x18, rand_int_32\n"
			"flw.ps f6, 416(x18)\n"
			"flw.ps f14, 160(x18)\n"
			"fxor.pi f1, f6, f14\n" 
			"la x6, rand_int_32\n"
			"flw.ps f22, 0(x6)\n"
			"fsrl.pi f23, f22, f22\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f27, 736(x11)\n"
			"flw.ps f15, 416(x11)\n"
			"fle.ps f15, f27, f15\n" 
			"fbci.ps f2, 0x61441\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f19, 32(x11)\n"
			"fnmadd.s f27, f19, f19, f19, rdn\n" 
			"la x7, rand_int_32\n"
			"flw.ps f12, 320(x7)\n"
			"flw.ps f6, 768(x7)\n"
			"fmulhu.pi f19, f12, f6\n" 
			"la x6, rand_int_32\n"
			"flw.ps f19, 672(x6)\n"
			"flw.ps f4, 288(x6)\n"
			"fmax.pi f1, f19, f4\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f4, 96(x7)\n"
			"flw.ps f14, 160(x7)\n"
			"fle.s x28, f4, f14\n" 
			"maskpopc x11, m0\n" 
			"mov.m.x m0, x0, 0xff\n"
			"la x18, rand_ieee754_32\n"
			"flw.ps f13, 96(x18)\n"
			"flw.ps f27, 576(x18)\n"
			"fdiv.ps f6, f13, f27, rup\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f19, 768(x28)\n"
			"flw.ps f14, 384(x28)\n"
			"fmin.s f3, f19, f14\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_2_TLD_XMA_STR_SEQ_ID_517274_HID_12:\n"
			"la x31, self_check_8\n"
			"li x5, 0xc0000000000006\n"
			"add x5, x5, x31\n"
			"li x6, 0x1000000000000c\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x1 << TENSOR_IMA_B_NUM_COLS) | (0x6 << TENSOR_IMA_A_NUM_ROWS) | (0xc << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x1 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1c << TENSOR_IMA_SCP_LOC_B) | (0x6 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fmvz.x.ps x31, f0, 0\n"
			"li x5, 0x34\n"
			"bne x5, x31, LBL_FAIL_HID_12\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_12\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_2_TLD_XMA_STR_SEQ_ITER_0_HID_12:\n"
			"la x31, self_check_8\n"
			"li x5, 0x3\n"
			"add x5, x5, x31\n"
			"li x6, 0x3e000000000000b\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x1 << TENSOR_IMA_B_NUM_COLS) | (0x3 << TENSOR_IMA_A_NUM_ROWS) | (0xb << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1f << TENSOR_IMA_SCP_LOC_B) | (0x0 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x9, ((0x1d << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0x3 << TENSOR_QUANT_QUANT_ROW) | (0x29 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x1 << TENSOR_QUANT_TRANSF8) | (0x9 << TENSOR_QUANT_TRANSF7) | (0x5 << TENSOR_QUANT_TRANSF6) | (0x3 << TENSOR_QUANT_TRANSF5) | (0x8 << TENSOR_QUANT_TRANSF4) | (0xa << TENSOR_QUANT_TRANSF3) | (0x4 << TENSOR_QUANT_TRANSF2) | (0x8 << TENSOR_QUANT_TRANSF1) | (0x9 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrwi tensor_wait, 0\n"
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
			"li x5, 3\n" 
			"bne x30, x5, LBL_FAIL_HID_12\n"
			"nop\n"          
		       VSNIP_RSV     
		);
		load_vpu_regs();
		__asm__ __volatile__ ( 
			"nop\n"          
			"addi x30, x30, 1\n"
			"la x11, rand_ieee754_16\n"
			"flw.ps f31, 992(x11)\n"
			"fcvt.ps.f16 f27, f31\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f23, 32(x25)\n"
			"flw.ps f1, 960(x25)\n"
			"fmul.s f2, f23, f1, rdn\n" 
			"li x5, 0x5d89\n"
			"csrw tensor_mask, x5\n"	
			"la x10, rand_ieee754_32\n"
			"flw.ps f3, 928(x10)\n"
			"flw.ps f27, 288(x10)\n"
			"feq.ps f4, f3, f27\n" 
			"la x27, rand_int_32\n"
			"flw.ps f3, 288(x27)\n"
			"flw.ps f27, 576(x27)\n"
			"feq.pi f4, f3, f27\n" 
			"la x25, rand_int_32\n"
			"flw.ps f2, 160(x25)\n"
			"flw.ps f12, 256(x25)\n"
			"fmin.pi f6, f2, f12\n" 
			"la x10, rand_int_32\n"
			"flw.ps f14, 224(x10)\n"
			"flw.ps f1, 896(x10)\n"
			"fle.pi f15, f14, f1\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f6, 768(x27)\n"
			"flw.ps f2, 0(x27)\n"
			"fadd.s f15, f6, f2, rdn\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f13, 128(x10)\n"
			"flw.ps f1, 384(x10)\n"
			"fdiv.s f2, f13, f1, rdn\n" 
			"la x11, rand_int_32\n"
			"flw.ps f3, 256(x11)\n"
			"flw.ps f19, 544(x11)\n"
			"for.pi f19, f3, f19\n" 
			"la x27, rand_int_32\n"
			"flw.ps f4, 608(x27)\n"
			"fpackreph.pi f9, f4\n" 
			"LBL_SEQID_3_M0_WRITE_ID_5_HID_12:\n"
			"flem.ps m5, f15, f6\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x25, rand_int_32\n"
			"flw.ps f1, 512(x25)\n"
			"flw.ps f13, 576(x25)\n"
			"fminu.pi f15, f1, f13\n" 
			"LBL_SEQID_3_FP_TRANS_ID_6_HID_12:\n"
			"la x11, rand_ieee754_32\n"
			"flw.ps f9, 96(x11)\n"
			"frcp.ps f12, f9\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f14, 960(x6)\n"
			"flw.ps f12, 416(x6)\n"
			"flw.ps f23, 640(x6)\n"
			"fmsub.ps f22, f14, f12, f23, rmm\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f4, 832(x25)\n"
			"flw.ps f15, 544(x25)\n"
			"flw.ps f22, 800(x25)\n"
			"fcmov.ps f31, f4, f15, f22\n" 
			"la x6, rand_int_32\n"
			"flw.ps f27, 448(x6)\n"
			"fsra.pi f6, f27, f27\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_3_TLD_XMA_STR_SEQ_ID_635605_HID_12:\n"
			"la x31, self_check_32\n"
			"li x5, 0xc0000000000001\n"
			"add x5, x5, x31\n"
			"li x6, 0x10000000000001\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x3 << TENSOR_FMA32_B_NUM_COLS) | (0x1 << TENSOR_FMA32_A_NUM_ROWS) | (0x1 << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x1 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1e << TENSOR_FMA32_SCP_LOC_B) | (0x6 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x1b << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0x1 << TENSOR_QUANT_QUANT_ROW) | (0x3 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x0 << TENSOR_QUANT_TRANSF1) | (0x0 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
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
			"LBL_SEQID_3_XMA_QNT_STR_SEQ_ITER_0_HID_12:\n"
			"li x5, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x3 << TENSOR_FMA32_B_NUM_COLS) | (0x6 << TENSOR_FMA32_A_NUM_ROWS) | (0xc << TENSOR_FMA32_A_NUM_COLS) | (0x1 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0xbc << TENSOR_FMA32_SCP_LOC_B) | (0x7c << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
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
			"mova.x.m x10\n" 
			"la x25, rand_int_32\n"
			"flw.ps f15, 64(x25)\n"
			"fpackreph.pi f6, f15\n" 
			"li x5, 0x6aed\n"
			"csrw tensor_mask, x5\n"	
			"la x27, rand_ieee754_32\n"
			"flw.ps f13, 896(x27)\n"
			"fmax.s f31, f13, f13\n" 
			"la x20, rand_int_32\n"
			"flw.ps f12, 416(x20)\n"
			"flw.ps f14, 352(x20)\n"
			"fltu.pi f9, f12, f14\n" 
			"LBL_SEQID_4_M0_WRITE_ID_7_HID_12:\n"
			"maskxor m2, m5, m5\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0xb9333cfac4\n"
			"fsw.ps f0, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x11, rand_int_32\n"
			"flw.ps f25, 160(x11)\n"
			"fnot.pi f25, f25\n" 
			"la x10, rand_int_32\n"
			"flw.ps f14, 896(x10)\n"
			"fsrli.pi f14, f14, 0xa\n" 
			"la x10, rand_int_32\n"
			"flw.ps f3, 384(x10)\n"
			"flw.ps f14, 608(x10)\n"
			"feq.pi f27, f3, f14\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f1, 640(x25)\n"
			"flw.ps f14, 928(x25)\n"
			"fmul.s f31, f1, f14, rup\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f6, 736(x6)\n"
			"flw.ps f19, 608(x6)\n"
			"flw.ps f3, 768(x6)\n"
			"fnmsub.ps f12, f6, f19, f3, rdn\n" 
			"maskpopcz x11, m5\n" 
			"LBL_SEQID_4_FP_TRANS_ID_8_HID_12:\n"
			"la x18, rand_ieee754_32\n"
			"flw.ps f22, 736(x18)\n"
			"frcp.ps f2, f22\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f25, 864(x22)\n"
			"flw.ps f19, 608(x22)\n"
			"flw.ps f15, 608(x22)\n"
			"fmadd.ps f2, f25, f19, f15, rtz\n" 
			"fbci.pi f3, 0x4cda2\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f15, 512(x27)\n"
			"flw.ps f14, 704(x27)\n"
			"flw.ps f23, 224(x27)\n"
			"fnmadd.ps f14, f15, f14, f23, rmm\n" 
			"la x20, rand_int_32\n"
			"flw.ps f31, 160(x20)\n"
			"fandi.pi f13, f31, 0x19d\n" 
			"csrw tensor_error, zero\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"LBL_SEQID_4_XMA_QNT_STR_SEQ_ITER_0_HID_12:\n"
			"li x6, ((0xd << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0x6 << TENSOR_QUANT_QUANT_ROW) | (0x1 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x5 << TENSOR_QUANT_TRANSF5) | (0x4 << TENSOR_QUANT_TRANSF4) | (0x8 << TENSOR_QUANT_TRANSF3) | (0x5 << TENSOR_QUANT_TRANSF2) | (0x9 << TENSOR_QUANT_TRANSF1) | (0x9 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_4_XMA_QNT_STR_SEQ_ITER_1_HID_12:\n"
			"li x6, ((0x14 << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0xb << TENSOR_QUANT_QUANT_ROW) | (0x29 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x0 << TENSOR_QUANT_TRANSF1) | (0x2 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
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
			"la x7, rand_int_32\n"
			"flw.ps f31, 320(x7)\n"
			"flw.ps f14, 544(x7)\n"
			"fle.pi f27, f31, f14\n" 
			"LBL_SEQID_5_M0_WRITE_ID_9_HID_12:\n"
			"mova.m.x x18\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x11, rand_ieee754_32\n"
			"flw.ps f27, 96(x11)\n"
			"fcvt.pwu.ps f13, f27, rtz\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f13, 960(x25)\n"
			"flw.ps f6, 128(x25)\n"
			"fmul.s f15, f13, f6, rup\n" 
			"la x10, rand_int_32\n"
			"flw.ps f19, 608(x10)\n"
			"fandi.pi f19, f19, 0xfb\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"fbci.ps f15, 0x15516\n" 
			"LBL_SEQID_5_M0_WRITE_ID_10_HID_12:\n"
			"masknot m2, m0\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0x9df0211c77\n"
			"flw.ps f5, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x10, rand_int_32\n"
			"flw.ps f25, 960(x10)\n"
			"flw.ps f9, 480(x10)\n"
			"fltu.pi f31, f25, f9\n" 
			"LBL_SEQID_5_M0_WRITE_ID_11_HID_12:\n"
			"mov.m.x m2, x6, 0x46\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x28, rand_ieee754_32\n"
			"flw.ps f1, 832(x28)\n"
			"flw.ps f22, 928(x28)\n"
			"fsub.ps f4, f1, f22, rmm\n" 
			"LBL_SEQID_5_M0_WRITE_ID_12_HID_12:\n"
			"mova.m.x x20\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_5_M0_WRITE_ID_13_HID_12:\n"
			"maskxor m1, m1, m1\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x6, rand_ieee754_32\n"
			"flw.ps f4, 928(x6)\n"
			"fcvt.f16.ps f6, f4\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f6, 352(x10)\n"
			"fcvt.w.s x6, f6, rtz\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f19, 544(x7)\n"
			"flw.ps f25, 64(x7)\n"
			"fsgnjx.ps f23, f19, f25\n" 
			"li x5, 0xf224a276a48b033f\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0x46cad4ba98a30b23\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"la x22, rand_ieee754_32\n"
			"flw.ps f2, 256(x22)\n"
			"flw.ps f19, 224(x22)\n"
			"fle.ps f13, f2, f19\n" 
			"li x5, 0xc2d4e80310c1963f\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0x5b01219a922749b3\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"csrw tensor_error, zero\n" 
			"li x5, 0xa934\n"
			"csrw tensor_mask, x5\n"	
			"LBL_SEQID_5_XMA_QNT_STR_SEQ_ID_562310_HID_12:\n"
			"li x5, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x0 << TENSOR_FMA16_B_NUM_COLS) | (0xa << TENSOR_FMA16_A_NUM_ROWS) | (0x6 << TENSOR_FMA16_A_NUM_COLS) | (0xc << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x36 << TENSOR_FMA16_SCP_LOC_B) | (0x26 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_12\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_5_TLD_XMA_STR_SEQ_ITER_0_HID_12:\n"
			"la x31, self_check_32\n"
			"li x5, 0xa0000000000009\n"
			"add x5, x5, x31\n"
			"li x6, 0x10000000000009\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x0 << TENSOR_FMA32_B_NUM_COLS) | (0x9 << TENSOR_FMA32_A_NUM_ROWS) | (0x9 << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x1 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x18 << TENSOR_FMA32_SCP_LOC_B) | (0x5 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0xa\n"
			"bne x5, x31, LBL_FAIL_HID_12\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_12\n"
			"csrw tensor_error, x0\n"
			"li x5, 6\n" 
			"bne x30, x5, LBL_FAIL_HID_12\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_6_M0_WRITE_ID_14_HID_12:\n"
			"fltm.pi m2, f6, f1\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0xceebd7cb88\n"
			"fsw.ps f0, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x28, rand_ieee754_32\n"
			"flw.ps f6, 224(x28)\n"
			"flw.ps f2, 736(x28)\n"
			"flt.s x10, f6, f2\n" 
			"LBL_SEQID_6_M0_WRITE_ID_15_HID_12:\n"
			"mova.m.x x27\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0xd252b60e99\n"
			"fsw.ps f0, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x20, rand_ieee754_32\n"
			"flw.ps f25, 800(x20)\n"
			"flw.ps f1, 896(x20)\n"
			"fmax.s f22, f25, f1\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f14, 544(x27)\n"
			"flw.ps f12, 864(x27)\n"
			"fmin.s f4, f14, f12\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f1, 32(x25)\n"
			"flw.ps f13, 192(x25)\n"
			"feq.ps f13, f1, f13\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x28, rand_int_32\n"
			"flw.ps f14, 0(x28)\n"
			"fnot.pi f19, f14\n" 
			"LBL_SEQID_6_M0_WRITE_ID_16_HID_12:\n"
			"maskor m5, m0, m1\n" 
			"mov.m.x m0, x0, 0xff\n"
			"li x5, 0x80340158\n"
			"ld x5, 0(x5)\n"
			"maskpopcz x10, m2\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f2, 384(x25)\n"
			"flw.ps f14, 640(x25)\n"
			"flw.ps f6, 448(x25)\n"
			"fnmsub.ps f27, f2, f14, f6, rdn\n" 
			"la x22, rand_int_32\n"
			"flw.ps f3, 448(x22)\n"
			"flw.ps f13, 896(x22)\n"
			"fmulhu.pi f25, f3, f13\n" 
			"li x5, 0x2848\n"
			"csrw tensor_mask, x5\n"	
			"la x22, rand_ieee754_32\n"
			"flw.ps f27, 0(x22)\n"
			"fcvt.w.s x10, f27, rne\n" 
			"la x11, rand_int_32\n"
			"flw.ps f13, 864(x11)\n"
			"flw.ps f14, 832(x11)\n"
			"fmaxu.pi f14, f13, f14\n" 
			"la x28, rand_int_32\n"
			"flw.ps f12, 800(x28)\n"
			"flw.ps f15, 288(x28)\n"
			"fsll.pi f23, f12, f15\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f6, 928(x25)\n"
			"flw.ps f13, 736(x25)\n"
			"fsub.ps f23, f6, f13, rmm\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f13, 608(x10)\n"
			"frsq.ps f1, f13\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_6_TLD_XMA_STR_SEQ_ID_293013_HID_12:\n"
			"la x31, self_check_16\n"
			"li x5, 0x4000000000000b\n"
			"add x5, x5, x31\n"
			"li x6, 0x1000000000000b\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x0 << TENSOR_FMA16_B_NUM_COLS) | (0xb << TENSOR_FMA16_A_NUM_ROWS) | (0xb << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x1 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1e << TENSOR_FMA16_SCP_LOC_B) | (0x2 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x1c << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0xb << TENSOR_QUANT_QUANT_ROW) | (0x23 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x2 << TENSOR_QUANT_TRANSF7) | (0x7 << TENSOR_QUANT_TRANSF6) | (0x6 << TENSOR_QUANT_TRANSF5) | (0x7 << TENSOR_QUANT_TRANSF4) | (0x6 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0x18\n"
			"bne x5, x31, LBL_FAIL_HID_12\n"
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_12\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_6_TLD_XMA_STR_SEQ_ITER_0_HID_12:\n"
			"la x31, self_check_32\n"
			"li x5, 0x6000000000000a\n"
			"add x5, x5, x31\n"
			"li x6, 0x10000000000001\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x1 << TENSOR_FMA32_B_NUM_COLS) | (0xa << TENSOR_FMA32_A_NUM_ROWS) | (0x1 << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x1 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1a << TENSOR_FMA32_SCP_LOC_B) | (0x3 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x9 << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0xa << TENSOR_QUANT_QUANT_ROW) | (0x2e << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x6 << TENSOR_QUANT_TRANSF4) | (0x6 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
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
			"csrwi mhpmevent3, 2\n"   
			"csrwi mhpmevent4, 1\n"   
			"csrwi mhpmevent5, 3\n"   
			"csrwi mhpmevent6, 6\n"   
			"csrwi mhpmevent7, 22\n"   
			"csrwi mhpmevent8, 2\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f20,  224 (x5)\n"
			"flw.ps f7,  192 (x5)\n"
			"flw.ps f26,  992 (x5)\n"
			"flw.ps f27,  864 (x5)\n"
			"flw.ps f31,  320 (x5)\n"
			"flw.ps f24,  288 (x5)\n"
			"flw.ps f30,  640 (x5)\n"
			"flw.ps f29,  896 (x5)\n"
			"flw.ps f6,  960 (x5)\n"
			"flw.ps f18,  160 (x5)\n"
			"flw.ps f13,  928 (x5)\n"
			"flw.ps f1,  480 (x5)\n"
			"flw.ps f21,  800 (x5)\n"
			"flw.ps f3,  32 (x5)\n"
			"flw.ps f8,  256 (x5)\n"
			"flw.ps f28,  544 (x5)\n"
			"flw.ps f22,  768 (x5)\n"
			"flw.ps f15,  672 (x5)\n"
			"flw.ps f14,  96 (x5)\n"
			"flw.ps f16,  416 (x5)\n"
			"flw.ps f23,  704 (x5)\n"
			"flw.ps f10,  512 (x5)\n"
			"flw.ps f12,  448 (x5)\n"
			"flw.ps f5,  64 (x5)\n"
			"flw.ps f25,  608 (x5)\n"
			"flw.ps f19,  832 (x5)\n"
			"flw.ps f4,  0 (x5)\n"
			"flw.ps f9,  384 (x5)\n"
			"flw.ps f0,  128 (x5)\n"
			"flw.ps f17,  576 (x5)\n"
			"flw.ps f11,  352 (x5)\n"
			"flw.ps f2,  736 (x5)\n"
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
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_13:\n"
			"flem.ps m4, f2, f26\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x17, rand_ieee754_32\n"
			"flw.ps f19, 800(x17)\n"
			"flw.ps f18, 928(x17)\n"
			"fadd.s f28, f19, f18, rmm\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f25, 160(x9)\n"
			"fcvt.pw.ps f6, f25, rtz\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f27, 896(x18)\n"
			"flw.ps f30, 864(x18)\n"
			"flw.ps f19, 384(x18)\n"
			"fnmadd.s f29, f27, f30, f19, rne\n" 
			"la x28, rand_int_32\n"
			"flw.ps f18, 672(x28)\n"
			"flw.ps f19, 832(x28)\n"
			"fmax.pi f14, f18, f19\n" 
			"la x5, rand_int_32\n"
			"flw.ps f3, 256(x5)\n"
			"flw.ps f30, 960(x5)\n"
			"fsrl.pi f28, f3, f30\n" 
			"maskpopc x28, m6\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f9, 992(x23)\n"
			"flw.ps f14, 512(x23)\n"
			"fmul.s f28, f9, f14, rne\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f12, 160(x16)\n"
			"flw.ps f26, 448(x16)\n"
			"fmul.ps f18, f12, f26, rne\n" 
			"la x17, rand_int_32\n"
			"flw.ps f25, 512(x17)\n"
			"flw.ps f10, 544(x17)\n"
			"fmulhu.pi f29, f25, f10\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_13:\n"
			"flem.ps m6, f3, f19\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_0_M0_WRITE_ID_2_HID_13:\n"
			"mov.m.x m5, x9, 0x1\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x18, rand_ieee754_32\n"
			"flw.ps f16, 288(x18)\n"
			"fround.ps f18, f16, rdn\n" 
			"fbci.pi f10, 0x70392\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f29, 224(x12)\n"
			"flw.ps f12, 928(x12)\n"
			"fadd.s f10, f29, f12, rdn\n" 
			"la x23, rand_int_32\n"
			"flw.ps f6, 608(x23)\n"
			"flw.ps f9, 960(x23)\n"
			"fsub.pi f19, f6, f9\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_13\n"
			"addi x30, x30, 1\n"
			"la x7, rand_ieee754_32\n"
			"flw.ps f3, 896(x7)\n"
			"fcvt.wu.s x12, f3, rdn\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f9, 96(x17)\n"
			"flw.ps f6, 992(x17)\n"
			"fadd.s f6, f9, f6, rtz\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f16, 608(x17)\n"
			"fcvt.wu.s x13, f16, rne\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f18, 672(x12)\n"
			"flw.ps f3, 768(x12)\n"
			"fadd.s f26, f18, f3, rmm\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f14, 512(x23)\n"
			"flt.ps f10, f14, f14\n" 
			"la x28, rand_int_32\n"
			"flw.ps f14, 672(x28)\n"
			"fsrli.pi f14, f14, 0xa\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f26, 576(x16)\n"
			"flw.ps f29, 928(x16)\n"
			"fmul.s f12, f26, f29, rne\n" 
			"la x12, rand_int_32\n"
			"flw.ps f2, 352(x12)\n"
			"fnot.pi f9, f2\n" 
			"fbci.ps f12, 0x58747\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f3, 896(x28)\n"
			"fswizz.ps f29, f3, 0xb\n" 
			"la x17, rand_int_32\n"
			"flw.ps f25, 160(x17)\n"
			"flw.ps f26, 192(x17)\n"
			"feq.pi f26, f25, f26\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f29, 608(x14)\n"
			"flw.ps f3, 192(x14)\n"
			"fnmsub.ps f12, f29, f29, f3, rdn\n" 
			"la x13, rand_int_32\n"
			"flw.ps f25, 960(x13)\n"
			"fpackreph.pi f6, f25\n" 
			"la x13, rand_int_32\n"
			"flw.ps f28, 576(x13)\n"
			"fnot.pi f10, f28\n" 
			"la x12, rand_int_32\n"
			"flw.ps f19, 320(x12)\n"
			"fandi.pi f25, f19, 0x1f\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f25, 800(x12)\n"
			"flw.ps f19, 672(x12)\n"
			"feq.s x7, f25, f19\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_13\n"
			"addi x30, x30, 1\n"
			"la x28, rand_int_32\n"
			"flw.ps f10, 832(x28)\n"
			"flw.ps f9, 992(x28)\n"
			"fmax.pi f18, f10, f9\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f16, 0(x5)\n"
			"flw.ps f12, 256(x5)\n"
			"fsgnjx.s f30, f16, f12\n" 
			"la x16, rand_int_32\n"
			"flw.ps f10, 576(x16)\n"
			"fsrli.pi f29, f10, 0x4\n" 
			"la x5, rand_int_32\n"
			"flw.ps f19, 64(x5)\n"
			"flw.ps f28, 608(x5)\n"
			"fmul.pi f30, f19, f28\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f12, 192(x23)\n"
			"flw.ps f25, 544(x23)\n"
			"flw.ps f28, 736(x23)\n"
			"fmsub.s f14, f12, f25, f28, rdn\n" 
			"la x28, rand_int_32\n"
			"flw.ps f10, 864(x28)\n"
			"flw.ps f12, 736(x28)\n"
			"fmaxu.pi f12, f10, f12\n" 
			"LBL_SEQID_2_M0_WRITE_ID_3_HID_13:\n"
			"maskor m6, m4, m6\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x5, rand_int_32\n"
			"flw.ps f19, 544(x5)\n"
			"flw.ps f30, 608(x5)\n"
			"fmax.pi f28, f19, f30\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f18, 512(x23)\n"
			"flw.ps f14, 320(x23)\n"
			"fmax.ps f9, f18, f14\n" 
			"la x17, rand_int_32\n"
			"flw.ps f3, 32(x17)\n"
			"flw.ps f28, 192(x17)\n"
			"fmin.pi f6, f3, f28\n" 
			"LBL_SEQID_2_M0_WRITE_ID_4_HID_13:\n"
			"fltm.pi m4, f2, f9\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x14, rand_int_32\n"
			"flw.ps f10, 800(x14)\n"
			"flw.ps f14, 704(x14)\n"
			"fxor.pi f29, f10, f14\n" 
			"la x12, rand_int_32\n"
			"flw.ps f16, 928(x12)\n"
			"flw.ps f18, 288(x12)\n"
			"fle.pi f30, f16, f18\n" 
			"mova.x.m x16\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f12, 992(x14)\n"
			"flw.ps f25, 384(x14)\n"
			"flw.ps f29, 224(x14)\n"
			"fnmadd.ps f18, f12, f25, f29, rmm\n" 
			"LBL_SEQID_2_FP_TRANS_ID_5_HID_13:\n"
			"la x7, rand_ieee754_32\n"
			"flw.ps f28, 928(x7)\n"
			"fexp.ps f2, f28\n" 
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
			"flw.ps f26, 768(x9)\n"
			"fcvt.wu.s x12, f26, rup\n" 
			"la x23, rand_int_32\n"
			"flw.ps f9, 896(x23)\n"
			"flw.ps f16, 608(x23)\n"
			"fmaxu.pi f19, f9, f16\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f3, 736(x16)\n"
			"flw.ps f2, 640(x16)\n"
			"fmin.s f30, f3, f2\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f9, 32(x18)\n"
			"flw.ps f29, 928(x18)\n"
			"fle.s x12, f9, f29\n" 
			"LBL_SEQID_3_FP_TRANS_ID_6_HID_13:\n"
			"la x23, rand_ieee754_32\n"
			"flw.ps f10, 512(x23)\n"
			"flog.ps f12, f10\n" 
			"li x18, 0x32db5dde1ac4f410\n"
			"fcvt.s.w f26, x18, rmm\n" 
			"la x12, rand_int_32\n"
			"flw.ps f28, 0(x12)\n"
			"fmax.pi f16, f28, f28\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f3, 928(x5)\n"
			"fclass.ps f27, f3\n" 
			"la x16, rand_int_32\n"
			"flw.ps f2, 288(x16)\n"
			"flw.ps f29, 736(x16)\n"
			"fltu.pi f25, f2, f29\n" 
			"la x5, rand_int_32\n"
			"flw.ps f14, 32(x5)\n"
			"flw.ps f3, 224(x5)\n"
			"fle.pi f18, f14, f3\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f16, 864(x13)\n"
			"flw.ps f18, 384(x13)\n"
			"flw.ps f29, 992(x13)\n"
			"fmsub.s f28, f16, f18, f29, rne\n" 
			"la x28, rand_int_32\n"
			"flw.ps f18, 544(x28)\n"
			"flw.ps f27, 320(x28)\n"
			"fsll.pi f30, f18, f27\n" 
			"la x5, rand_int_32\n"
			"flw.ps f16, 480(x5)\n"
			"fsrai.pi f25, f16, 0x8\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f3, 128(x23)\n"
			"fcvt.pwu.ps f19, f3, rtz\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f14, 192(x18)\n"
			"flw.ps f27, 992(x18)\n"
			"flw.ps f28, 704(x18)\n"
			"fnmadd.ps f27, f14, f27, f28, rne\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f26, 320(x12)\n"
			"fcvt.wu.s x18, f26, rup\n" 
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
			"la x9, rand_ieee754_32\n"
			"flw.ps f6, 256(x9)\n"
			"flw.ps f16, 608(x9)\n"
			"flt.s x5, f6, f16\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f29, 864(x23)\n"
			"flw.ps f25, 864(x23)\n"
			"feq.ps f19, f29, f25\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f27, 224(x12)\n"
			"flw.ps f3, 768(x12)\n"
			"fmul.s f9, f27, f3, rtz\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f27, 32(x9)\n"
			"flw.ps f30, 864(x9)\n"
			"fmul.s f26, f27, f30, rup\n" 
			"la x12, rand_int_32\n"
			"flw.ps f29, 288(x12)\n"
			"flw.ps f19, 96(x12)\n"
			"fmin.pi f10, f29, f19\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f27, 992(x28)\n"
			"fcvt.w.s x12, f27, rtz\n" 
			"la x7, rand_int_32\n"
			"flw.ps f25, 896(x7)\n"
			"flw.ps f26, 96(x7)\n"
			"fadd.pi f12, f25, f26\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f2, 480(x28)\n"
			"flw.ps f9, 512(x28)\n"
			"fmin.s f6, f2, f9\n" 
			"la x23, rand_int_32\n"
			"flw.ps f19, 704(x23)\n"
			"flw.ps f9, 768(x23)\n"
			"fxor.pi f16, f19, f9\n" 
			"fbci.ps f9, 0x458c9\n" 
			"LBL_SEQID_4_FP_TRANS_ID_7_HID_13:\n"
			"la x18, rand_ieee754_32\n"
			"flw.ps f9, 896(x18)\n"
			"flog.ps f12, f9\n" 
			"maskpopcz x16, m5\n" 
			"maskpopcz x16, m5\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f14, 832(x17)\n"
			"flw.ps f10, 288(x17)\n"
			"flw.ps f25, 896(x17)\n"
			"fmsub.ps f9, f14, f10, f25, rne\n" 
			"la x13, rand_int_32\n"
			"flw.ps f14, 800(x13)\n"
			"fpackrepb.pi f16, f14\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f26, 896(x17)\n"
			"flw.ps f12, 928(x17)\n"
			"flw.ps f9, 192(x17)\n"
			"fcmov.ps f30, f26, f12, f9\n" 
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
			"la x28, rand_ieee754_32\n"
			"flw.ps f3, 352(x28)\n"
			"flw.ps f14, 128(x28)\n"
			"flw.ps f10, 320(x28)\n"
			"fnmadd.ps f18, f3, f14, f10, rmm\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f25, 768(x23)\n"
			"flw.ps f18, 512(x23)\n"
			"flt.s x12, f25, f18\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f29, 128(x17)\n"
			"flw.ps f18, 384(x17)\n"
			"flw.ps f10, 704(x17)\n"
			"fmsub.s f18, f29, f18, f10, rne\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f2, 512(x12)\n"
			"flw.ps f9, 512(x12)\n"
			"fsgnj.ps f28, f2, f9\n" 
			"LBL_SEQID_5_M0_WRITE_ID_8_HID_13:\n"
			"feqm.ps m4, f27, f26\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x17, rand_int_32\n"
			"flw.ps f2, 192(x17)\n"
			"fsrli.pi f14, f2, 0x4\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f14, 576(x9)\n"
			"flw.ps f18, 96(x9)\n"
			"flw.ps f9, 672(x9)\n"
			"fnmsub.s f9, f14, f18, f9, rmm\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f26, 832(x9)\n"
			"flw.ps f18, 352(x9)\n"
			"feq.s x28, f26, f18\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f16, 448(x5)\n"
			"fsub.s f2, f16, f16, rdn\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f9, 448(x13)\n"
			"fcvt.wu.s x5, f9, rdn\n" 
			"LBL_SEQID_5_M0_WRITE_ID_9_HID_13:\n"
			"maskor m6, m6, m5\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_5_M0_WRITE_ID_10_HID_13:\n"
			"fsetm.pi m4, f28\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x12, rand_ieee754_32\n"
			"flw.ps f18, 352(x12)\n"
			"flw.ps f14, 736(x12)\n"
			"flt.ps f16, f18, f14\n" 
			"LBL_SEQID_5_M0_WRITE_ID_11_HID_13:\n"
			"flem.ps m4, f3, f9\n" 
			"li x5, CREDINC3\n"
			"ld x5, 0(x5)\n"
			"la x14, rand_int_32\n"
			"flw.ps f12, 576(x14)\n"
			"flw.ps f2, 128(x14)\n"
			"fmul.pi f12, f12, f2\n" 
			"fbci.pi f26, 0x45fc2\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 6\n" 
			"bne x30, x5, LBL_FAIL_HID_13\n"
			"addi x30, x30, 1\n"
			"la x18, rand_int_32\n"
			"flw.ps f10, 992(x18)\n"
			"fnot.pi f16, f10\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f10, 736(x12)\n"
			"flw.ps f14, 192(x12)\n"
			"flw.ps f19, 256(x12)\n"
			"fnmsub.s f26, f10, f14, f19, rtz\n" 
			"la x28, rand_int_32\n"
			"flw.ps f26, 288(x28)\n"
			"fnot.pi f27, f26\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f30, 928(x12)\n"
			"fcvt.pwu.ps f19, f30, rne\n" 
			"la x12, rand_int_32\n"
			"flw.ps f18, 0(x12)\n"
			"flw.ps f28, 0(x12)\n"
			"fmulh.pi f14, f18, f28\n" 
			"la x5, rand_int_32\n"
			"flw.ps f9, 352(x5)\n"
			"flw.ps f29, 512(x5)\n"
			"fremu.pi f27, f9, f29\n" 
			"la x18, rand_int_32\n"
			"flw.ps f29, 480(x18)\n"
			"flw.ps f18, 32(x18)\n"
			"fminu.pi f16, f29, f18\n" 
			"maskpopcz x14, m3\n" 
			"la x16, rand_int_32\n"
			"flw.ps f6, 736(x16)\n"
			"fsrli.pi f28, f6, 0x1\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f16, 896(x7)\n"
			"flw.ps f12, 832(x7)\n"
			"fmul.ps f19, f16, f12, rup\n" 
			"la x28, rand_int_32\n"
			"flw.ps f16, 288(x28)\n"
			"fsat8.pi f25, f16\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f16, 928(x23)\n"
			"fadd.s f29, f16, f16, rdn\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f18, 928(x7)\n"
			"fclass.ps f27, f18\n" 
			"la x23, rand_int_32\n"
			"flw.ps f9, 736(x23)\n"
			"fsat8.pi f29, f9\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f19, 576(x14)\n"
			"flw.ps f2, 96(x14)\n"
			"fsgnjn.s f9, f19, f2\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f12, 448(x7)\n"
			"fle.ps f6, f12, f12\n" 
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
			"csrwi mhpmevent3, 8\n"   
			"csrwi mhpmevent4, 14\n"   
			"csrwi mhpmevent5, 4\n"   
			"csrwi mhpmevent6, 17\n"   
			"csrwi mhpmevent7, 6\n"   
			"csrwi mhpmevent8, 3\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f30,  192 (x5)\n"
			"flw.ps f25,  448 (x5)\n"
			"flw.ps f19,  32 (x5)\n"
			"flw.ps f26,  832 (x5)\n"
			"flw.ps f18,  576 (x5)\n"
			"flw.ps f5,  352 (x5)\n"
			"flw.ps f2,  64 (x5)\n"
			"flw.ps f8,  928 (x5)\n"
			"flw.ps f24,  960 (x5)\n"
			"flw.ps f17,  896 (x5)\n"
			"flw.ps f28,  864 (x5)\n"
			"flw.ps f21,  608 (x5)\n"
			"flw.ps f13,  256 (x5)\n"
			"flw.ps f6,  992 (x5)\n"
			"flw.ps f3,  384 (x5)\n"
			"flw.ps f0,  512 (x5)\n"
			"flw.ps f27,  800 (x5)\n"
			"flw.ps f1,  96 (x5)\n"
			"flw.ps f10,  768 (x5)\n"
			"flw.ps f4,  224 (x5)\n"
			"flw.ps f9,  288 (x5)\n"
			"flw.ps f14,  544 (x5)\n"
			"flw.ps f11,  640 (x5)\n"
			"flw.ps f12,  480 (x5)\n"
			"flw.ps f29,  704 (x5)\n"
			"flw.ps f16,  0 (x5)\n"
			"flw.ps f31,  416 (x5)\n"
			"flw.ps f23,  160 (x5)\n"
			"flw.ps f20,  128 (x5)\n"
			"flw.ps f15,  736 (x5)\n"
			"flw.ps f7,  320 (x5)\n"
			"flw.ps f22,  672 (x5)\n"
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
			"la x27, rand_ieee754_32\n"
			"flw.ps f6, 288(x27)\n"
			"frsq.ps f9, f6\n" 
			"la x14, rand_int_32\n"
			"flw.ps f6, 128(x14)\n"
			"faddi.pi f5, f6, 0x5e\n" 
			"la x19, rand_int_32\n"
			"flw.ps f17, 288(x19)\n"
			"fnot.pi f31, f17\n" 
			"li x5, 0x17a8\n"
			"csrw tensor_mask, x5\n"	
			"la x18, rand_ieee754_32\n"
			"flw.ps f22, 896(x18)\n"
			"flw.ps f18, 160(x18)\n"
			"fsgnjx.s f13, f22, f18\n" 
			"la x20, rand_int_32\n"
			"flw.ps f7, 320(x20)\n"
			"flw.ps f23, 64(x20)\n"
			"fminu.pi f6, f7, f23\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_14:\n"
			"fsetm.pi m4, f6\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x27, rand_ieee754_32\n"
			"flw.ps f31, 320(x27)\n"
			"flw.ps f3, 224(x27)\n"
			"fmax.ps f25, f31, f3\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f4, 512(x9)\n"
			"flw.ps f7, 672(x9)\n"
			"fmul.ps f13, f4, f7, rne\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f25, 288(x23)\n"
			"flw.ps f4, 0(x23)\n"
			"fmin.ps f13, f25, f4\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f6, 32(x14)\n"
			"fcvt.f16.ps f15, f6\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f17, 768(x9)\n"
			"flw.ps f23, 576(x9)\n"
			"feq.ps f14, f17, f23\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f4, 704(x14)\n"
			"flw.ps f13, 640(x14)\n"
			"fsgnjn.s f22, f4, f13\n" 
			"li x18, 0x8f6fb760c3e0ea88\n"
			"fcvt.s.w f25, x18, rne\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x17, rand_int_32\n"
			"flw.ps f14, 224(x17)\n"
			"fcvt.ps.pwu f23, f14, rtz\n" 
			"la x20, rand_int_32\n"
			"flw.ps f15, 480(x20)\n"
			"flw.ps f14, 160(x20)\n"
			"fxor.pi f6, f15, f14\n" 
			"la x18, rand_int_32\n"
			"flw.ps f23, 704(x18)\n"
			"flw.ps f4, 640(x18)\n"
			"fxor.pi f4, f23, f4\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_0_RDX_ID_0_SHIRE_0_HID_14:\n"
			"li x14, 0x3a0000000353000b\n"
			"csrw tensor_reduce, x14\n" 
			"csrwi tensor_wait, 9\n" 
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=0); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ITER_0_HID_14:\n"
			"la x31, self_check_32\n"
			"li x5, 0x60000000000006\n"
			"add x5, x5, x31\n"
			"li x6, 0x10000000000004\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x2 << TENSOR_FMA32_B_NUM_COLS) | (0x6 << TENSOR_FMA32_A_NUM_ROWS) | (0x4 << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x1 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1d << TENSOR_FMA32_SCP_LOC_B) | (0x3 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x19 << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0x6 << TENSOR_QUANT_QUANT_ROW) | (0x2b << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x2 << TENSOR_QUANT_TRANSF8) | (0x6 << TENSOR_QUANT_TRANSF7) | (0x7 << TENSOR_QUANT_TRANSF6) | (0x7 << TENSOR_QUANT_TRANSF5) | (0x7 << TENSOR_QUANT_TRANSF4) | (0x7 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0x5\n"
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
			"la x19, rand_ieee754_32\n"
			"flw.ps f13, 256(x19)\n"
			"fcvt.pw.ps f7, f13, rmm\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f23, 960(x9)\n"
			"flw.ps f17, 608(x9)\n"
			"fsub.ps f25, f23, f17, rtz\n" 
			"la x7, rand_int_32\n"
			"flw.ps f18, 64(x7)\n"
			"flw.ps f31, 832(x7)\n"
			"fmulhu.pi f25, f18, f31\n" 
			"la x17, rand_int_32\n"
			"flw.ps f17, 896(x17)\n"
			"flw.ps f5, 864(x17)\n"
			"fand.pi f13, f17, f5\n" 
			"la x10, rand_int_32\n"
			"flw.ps f15, 928(x10)\n"
			"flw.ps f13, 992(x10)\n"
			"fminu.pi f7, f15, f13\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f6, 928(x19)\n"
			"flw.ps f15, 736(x19)\n"
			"fsub.ps f3, f6, f15, rne\n" 
			"la x14, rand_int_32\n"
			"flw.ps f18, 160(x14)\n"
			"flw.ps f15, 736(x14)\n"
			"fmulh.pi f9, f18, f15\n" 
			"la x18, rand_int_32\n"
			"flw.ps f22, 608(x18)\n"
			"flw.ps f25, 416(x18)\n"
			"fmulh.pi f31, f22, f25\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f15, 736(x7)\n"
			"flw.ps f9, 448(x7)\n"
			"fle.s x23, f15, f9\n" 
			"la x19, rand_int_32\n"
			"flw.ps f18, 480(x19)\n"
			"fsat8.pi f31, f18\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f3, 992(x19)\n"
			"flw.ps f9, 512(x19)\n"
			"flw.ps f17, 608(x19)\n"
			"fmadd.s f3, f3, f9, f17, rmm\n" 
			"fbci.ps f6, 0x606ac\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f22, 960(x27)\n"
			"flw.ps f13, 384(x27)\n"
			"fmin.ps f5, f22, f13\n" 
			"la x7, rand_int_32\n"
			"flw.ps f14, 608(x7)\n"
			"fsatu8.pi f18, f14\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f17, 992(x14)\n"
			"fsgnjn.ps f23, f17, f17\n" 
			"LBL_SEQID_1_FP_TRANS_ID_1_HID_14:\n"
			"la x9, rand_ieee754_32\n"
			"flw.ps f5, 672(x9)\n"
			"fexp.ps f7, f5\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_1_RDX_ID_1_SHIRE_0_HID_14:\n"
			"li x14, 0x300000000738000b\n"
			"csrw tensor_reduce, x14\n" 
			"csrwi tensor_wait, 9\n" 
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ID_958370_HID_14:\n"
			"li x5, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x2 << TENSOR_FMA16_B_NUM_COLS) | (0x9 << TENSOR_FMA16_A_NUM_ROWS) | (0xe << TENSOR_FMA16_A_NUM_COLS) | (0x2 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x24 << TENSOR_FMA16_SCP_LOC_B) | (0x37 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ID_155170_HID_14:\n"
			"li x5, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x1 << TENSOR_FMA16_B_NUM_COLS) | (0xf << TENSOR_FMA16_A_NUM_ROWS) | (0x7 << TENSOR_FMA16_A_NUM_COLS) | (0xe << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1a << TENSOR_FMA16_SCP_LOC_B) | (0x42 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x0 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
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
			"LBL_SEQID_1_TLD_XMA_STR_SEQ_ITER_0_HID_14:\n"
			"la x31, self_check_32\n"
			"li x5, 0x6000000000000b\n"
			"add x5, x5, x31\n"
			"li x6, 0x1000000000000d\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x1 << TENSOR_FMA32_B_NUM_COLS) | (0xb << TENSOR_FMA32_A_NUM_ROWS) | (0xd << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x1 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1d << TENSOR_FMA32_SCP_LOC_B) | (0x3 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x1 << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0xb << TENSOR_QUANT_QUANT_ROW) | (0x10 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x2 << TENSOR_QUANT_TRANSF6) | (0x7 << TENSOR_QUANT_TRANSF5) | (0x7 << TENSOR_QUANT_TRANSF4) | (0x7 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0xe\n"
			"bne x5, x31, LBL_FAIL_HID_14\n"
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_14\n"
			"csrw tensor_error, x0\n"
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_14\n"
			"addi x30, x30, 1\n"
			"la x20, rand_int_32\n"
			"flw.ps f18, 544(x20)\n"
			"flw.ps f6, 224(x20)\n"
			"fmax.pi f27, f18, f6\n" 
			"la x10, rand_int_32\n"
			"flw.ps f23, 32(x10)\n"
			"fsub.pi f31, f23, f23\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x14, rand_ieee754_32\n"
			"flw.ps f9, 160(x14)\n"
			"flw.ps f7, 32(x14)\n"
			"fle.s x20, f9, f7\n" 
			"la x10, rand_int_32\n"
			"flw.ps f23, 960(x10)\n"
			"flw.ps f3, 704(x10)\n"
			"fxor.pi f5, f23, f3\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f22, 32(x14)\n"
			"fsin.ps f27, f22\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x20, rand_ieee754_32\n"
			"flw.ps f15, 288(x20)\n"
			"flw.ps f22, 672(x20)\n"
			"fsgnjn.ps f17, f15, f22\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x17, rand_int_32\n"
			"flw.ps f18, 32(x17)\n"
			"flw.ps f3, 672(x17)\n"
			"fsub.pi f25, f18, f3\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f14, 640(x7)\n"
			"ffrc.ps f27, f14\n" 
			"la x27, rand_int_32\n"
			"flw.ps f6, 928(x27)\n"
			"flw.ps f25, 0(x27)\n"
			"fmulhu.pi f15, f6, f25\n" 
			"la x19, rand_int_32\n"
			"flw.ps f27, 64(x19)\n"
			"faddi.pi f22, f27, 0x1c7\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f31, 672(x14)\n"
			"flw.ps f27, 512(x14)\n"
			"flw.ps f13, 704(x14)\n"
			"fnmadd.ps f14, f31, f27, f13, rmm\n" 
			"la x5, rand_int_32\n"
			"flw.ps f27, 192(x5)\n"
			"faddi.pi f7, f27, 0x8d\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f14, 736(x19)\n"
			"fcvt.wu.s x7, f14, rmm\n" 
			"la x9, rand_int_32\n"
			"flw.ps f25, 384(x9)\n"
			"fpackreph.pi f7, f25\n" 
			"LBL_SEQID_2_M0_WRITE_ID_2_HID_14:\n"
			"flem.ps m4, f4, f13\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x17, rand_int_32\n"
			"flw.ps f18, 960(x17)\n"
			"flw.ps f5, 864(x17)\n"
			"fremu.pi f7, f18, f5\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_2_TLD_XMA_STR_SEQ_ID_372243_HID_14:\n"
			"la x31, self_check_32\n"
			"li x5, 0x8000000000000d\n"
			"add x5, x5, x31\n"
			"li x6, 0x10000000000007\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x1 << TENSOR_FMA32_B_NUM_COLS) | (0xd << TENSOR_FMA32_A_NUM_ROWS) | (0x7 << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x1 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1c << TENSOR_FMA32_SCP_LOC_B) | (0x4 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
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
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_14\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_2_TLD_XMA_STR_SEQ_ITER_0_HID_14:\n"
			"la x31, self_check_16\n"
			"li x5, 0xe0000000000006\n"
			"add x5, x5, x31\n"
			"li x6, 0x360000000000001\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x0 << TENSOR_FMA16_B_NUM_COLS) | (0x6 << TENSOR_FMA16_A_NUM_ROWS) | (0x1 << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1b << TENSOR_FMA16_SCP_LOC_B) | (0x7 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x9 << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0x6 << TENSOR_QUANT_QUANT_ROW) | (0x1b << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x2 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0x4\n"
			"bne x5, x31, LBL_FAIL_HID_14\n"
			"csrw tensor_quant, x9\n"	
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
			"flw.ps f15, 576(x18)\n"
			"flw.ps f31, 32(x18)\n"
			"fminu.pi f23, f15, f31\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x5, rand_int_32\n"
			"flw.ps f7, 64(x5)\n"
			"flw.ps f23, 224(x5)\n"
			"fadd.pi f5, f7, f23\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f13, 608(x17)\n"
			"fclass.s x10, f13\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f6, 672(x5)\n"
			"fswizz.ps f4, f6, 0x1a\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f18, 160(x18)\n"
			"flw.ps f6, 64(x18)\n"
			"fnmsub.s f4, f18, f18, f6, rdn\n" 
			"LBL_SEQID_3_M0_WRITE_ID_3_HID_14:\n"
			"mova.m.x x5\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0x3386af576b\n"
			"flw.ps f5, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x9, rand_ieee754_32\n"
			"flw.ps f18, 672(x9)\n"
			"flw.ps f7, 832(x9)\n"
			"feq.ps f31, f18, f7\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f5, 960(x9)\n"
			"flw.ps f18, 608(x9)\n"
			"flw.ps f3, 608(x9)\n"
			"fnmadd.s f31, f5, f18, f3, rdn\n" 
			"la x27, rand_int_32\n"
			"flw.ps f14, 224(x27)\n"
			"flw.ps f6, 832(x27)\n"
			"fand.pi f31, f14, f6\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f18, 864(x17)\n"
			"fsin.ps f31, f18\n" 
			"LBL_SEQID_3_M0_WRITE_ID_4_HID_14:\n"
			"maskor m1, m2, m1\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_3_M0_WRITE_ID_5_HID_14:\n"
			"fsetm.pi m2, f14\n" 
			"li x5, CREDINC2\n"
			"ld x5, 0(x5)\n"
			"la x7, rand_ieee754_32\n"
			"flw.ps f25, 768(x7)\n"
			"flw.ps f22, 896(x7)\n"
			"fsub.s f23, f25, f22, rne\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x18, rand_ieee754_32\n"
			"flw.ps f27, 960(x18)\n"
			"fclass.s x9, f27\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f22, 320(x17)\n"
			"flw.ps f4, 32(x17)\n"
			"fmul.ps f14, f22, f4, rmm\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f5, 640(x19)\n"
			"fround.ps f6, f5, rmm\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_3_XMA_QNT_STR_SEQ_ITER_0_HID_14:\n"
			"li x5, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x2 << TENSOR_IMA_B_NUM_COLS) | (0x6 << TENSOR_IMA_A_NUM_ROWS) | (0x8 << TENSOR_IMA_A_NUM_COLS) | (0x4 << TENSOR_IMA_OFFSET) | (0x0 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0xd6 << TENSOR_IMA_SCP_LOC_B) | (0x72 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x0 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x6, ((0x1f << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0x6 << TENSOR_QUANT_QUANT_ROW) | (0x29 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x1 << TENSOR_QUANT_TRANSF1) | (0x8 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 7\n"
			"csrwi tensor_wait, 10\n"
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
			"la x18, rand_ieee754_32\n"
			"flw.ps f9, 864(x18)\n"
			"flw.ps f18, 288(x18)\n"
			"fadd.ps f9, f9, f18, rup\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f13, 96(x10)\n"
			"flw.ps f31, 128(x10)\n"
			"flw.ps f7, 256(x10)\n"
			"fmsub.ps f3, f13, f31, f7, rup\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f7, 896(x23)\n"
			"flw.ps f15, 384(x23)\n"
			"fsub.ps f7, f7, f15, rne\n" 
			"la x9, rand_int_32\n"
			"flw.ps f18, 128(x9)\n"
			"fsrai.pi f5, f18, 0x3\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f5, 992(x5)\n"
			"fswizz.ps f22, f5, 0x2\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f17, 448(x23)\n"
			"flw.ps f31, 0(x23)\n"
			"feq.s x23, f17, f31\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f15, 800(x17)\n"
			"flw.ps f25, 192(x17)\n"
			"fsub.ps f31, f15, f25, rdn\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f22, 768(x10)\n"
			"flw.ps f18, 576(x10)\n"
			"flw.ps f9, 32(x10)\n"
			"fmadd.s f7, f22, f18, f9, rne\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f3, 128(x10)\n"
			"fround.ps f7, f3, rtz\n" 
			"la x19, rand_int_32\n"
			"flw.ps f4, 544(x19)\n"
			"flw.ps f17, 480(x19)\n"
			"fand.pi f6, f4, f17\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f31, 320(x17)\n"
			"flw.ps f18, 64(x17)\n"
			"fsgnj.s f5, f31, f18\n" 
			"LBL_SEQID_4_FP_TRANS_ID_6_HID_14:\n"
			"la x19, rand_ieee754_32\n"
			"flw.ps f3, 32(x19)\n"
			"fexp.ps f23, f3\n" 
			"la x7, rand_int_32\n"
			"flw.ps f23, 0(x7)\n"
			"flw.ps f18, 896(x7)\n"
			"fxor.pi f18, f23, f18\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f23, 0(x7)\n"
			"fcvt.w.s x23, f23, rup\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f6, 512(x10)\n"
			"flw.ps f17, 992(x10)\n"
			"fsgnjn.ps f27, f6, f17\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f27, 672(x18)\n"
			"flw.ps f23, 896(x18)\n"
			"fmul.ps f13, f27, f23, rtz\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_4_XMA_QNT_STR_SEQ_ITER_0_HID_14:\n"
			"li x6, ((0x1b << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0x0 << TENSOR_QUANT_QUANT_ROW) | (0x18 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x8 << TENSOR_QUANT_TRANSF4) | (0x3 << TENSOR_QUANT_TRANSF3) | (0x4 << TENSOR_QUANT_TRANSF2) | (0x8 << TENSOR_QUANT_TRANSF1) | (0x4 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_4_XMA_QNT_STR_SEQ_ITER_1_HID_14:\n"
			"li x6, ((0xc << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0xe << TENSOR_QUANT_QUANT_ROW) | (0x31 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x2 << TENSOR_QUANT_TRANSF8) | (0x6 << TENSOR_QUANT_TRANSF7) | (0x7 << TENSOR_QUANT_TRANSF6) | (0x6 << TENSOR_QUANT_TRANSF5) | (0x7 << TENSOR_QUANT_TRANSF4) | (0x6 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_4_XMA_QNT_STR_SEQ_ITER_2_HID_14:\n"
			"li x6, ((0x1a << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0xd << TENSOR_QUANT_QUANT_ROW) | (0x23 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 10\n"
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
			"la x9, rand_ieee754_32\n"
			"flw.ps f22, 864(x9)\n"
			"fsin.ps f13, f22\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"maskpopc x14, m4\n" 
			"la x27, rand_int_32\n"
			"flw.ps f23, 512(x27)\n"
			"fsrli.pi f31, f23, 0x5\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f17, 384(x5)\n"
			"flw.ps f25, 192(x5)\n"
			"fmax.ps f22, f17, f25\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f14, 928(x10)\n"
			"flw.ps f7, 800(x10)\n"
			"fadd.ps f9, f14, f7, rup\n" 
			"LBL_SEQID_5_M0_WRITE_ID_7_HID_14:\n"
			"fsetm.pi m7, f13\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x18, rand_ieee754_32\n"
			"flw.ps f3, 352(x18)\n"
			"fcvt.pwu.ps f6, f3, rtz\n" 
			"la x7, rand_int_32\n"
			"flw.ps f23, 736(x7)\n"
			"flw.ps f3, 704(x7)\n"
			"fminu.pi f17, f23, f3\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f4, 896(x23)\n"
			"flw.ps f9, 928(x23)\n"
			"fsgnjx.s f18, f4, f9\n" 
			"LBL_SEQID_5_M0_WRITE_ID_8_HID_14:\n"
			"mova.m.x x18\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x20, rand_int_32\n"
			"flw.ps f5, 224(x20)\n"
			"flw.ps f27, 96(x20)\n"
			"fmax.pi f7, f5, f27\n" 
			"LBL_SEQID_5_M0_WRITE_ID_9_HID_14:\n"
			"feqm.ps m7, f31, f14\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_5_M0_WRITE_ID_10_HID_14:\n"
			"fsetm.pi m7, f22\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x14, rand_int_32\n"
			"flw.ps f13, 768(x14)\n"
			"flw.ps f6, 224(x14)\n"
			"fminu.pi f23, f13, f6\n" 
			"la x19, rand_int_32\n"
			"flw.ps f9, 832(x19)\n"
			"flw.ps f5, 512(x19)\n"
			"fand.pi f25, f9, f5\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f5, 832(x19)\n"
			"fcvt.wu.s x5, f5, rdn\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_5_TLD_XMA_STR_SEQ_ITER_0_HID_14:\n"
			"la x31, self_check_32\n"
			"li x5, 0x6\n"
			"add x5, x5, x31\n"
			"li x6, 0x320000000000008\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x3 << TENSOR_FMA32_B_NUM_COLS) | (0x6 << TENSOR_FMA32_A_NUM_ROWS) | (0x8 << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x19 << TENSOR_FMA32_SCP_LOC_B) | (0x0 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
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
			"li x5, 6\n" 
			"bne x30, x5, LBL_FAIL_HID_14\n"
			"addi x30, x30, 1\n"
			"la x20, rand_ieee754_32\n"
			"flw.ps f7, 800(x20)\n"
			"flw.ps f14, 128(x20)\n"
			"fadd.s f22, f7, f14, rdn\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f27, 0(x20)\n"
			"flw.ps f31, 992(x20)\n"
			"fmin.ps f25, f27, f31\n" 
			"LBL_SEQID_6_M0_WRITE_ID_11_HID_14:\n"
			"flem.ps m4, f6, f6\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x18, rand_ieee754_32\n"
			"flw.ps f25, 96(x18)\n"
			"flw.ps f31, 480(x18)\n"
			"flw.ps f13, 256(x18)\n"
			"fnmadd.s f18, f25, f31, f13, rdn\n" 
			"la x19, rand_int_32\n"
			"flw.ps f7, 672(x19)\n"
			"flw.ps f25, 480(x19)\n"
			"fsra.pi f15, f7, f25\n" 
			"LBL_SEQID_6_M0_WRITE_ID_12_HID_14:\n"
			"flem.ps m2, f17, f23\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x5, rand_ieee754_32\n"
			"flw.ps f25, 704(x5)\n"
			"flw.ps f7, 64(x5)\n"
			"fsgnj.s f15, f25, f7\n" 
			"la x17, rand_int_32\n"
			"flw.ps f13, 800(x17)\n"
			"flw.ps f7, 800(x17)\n"
			"fmul.pi f7, f13, f7\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f7, 128(x5)\n"
			"flw.ps f9, 896(x5)\n"
			"flw.ps f15, 576(x5)\n"
			"fmsub.s f3, f7, f9, f15, rne\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f7, 96(x18)\n"
			"flw.ps f4, 608(x18)\n"
			"flt.ps f22, f7, f4\n" 
			"la x14, rand_int_32\n"
			"flw.ps f25, 192(x14)\n"
			"fsrli.pi f6, f25, 0x8\n" 
			"maskpopcz x23, m2\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f3, 640(x17)\n"
			"fround.ps f3, f3, rup\n" 
			"mova.x.m x20\n" 
			"LBL_SEQID_6_M0_WRITE_ID_13_HID_14:\n"
			"maskxor m2, m1, m1\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_6_M0_WRITE_ID_14_HID_14:\n"
			"fltm.ps m7, f25, f9\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_6_XMA_QNT_STR_SEQ_ID_406488_HID_14:\n"
			"li x5, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x2 << TENSOR_IMA_B_NUM_COLS) | (0x4 << TENSOR_IMA_A_NUM_ROWS) | (0xa << TENSOR_IMA_A_NUM_COLS) | (0x7 << TENSOR_IMA_OFFSET) | (0x0 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0xe9 << TENSOR_IMA_SCP_LOC_B) | (0xee << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x0 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_14\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_6_TLD_XMA_STR_SEQ_ITER_0_HID_14:\n"
			"la x31, self_check_8\n"
			"li x5, 0xa0000000000003\n"
			"add x5, x5, x31\n"
			"li x6, 0x300000000000004\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x3 << TENSOR_IMA_B_NUM_COLS) | (0x3 << TENSOR_IMA_A_NUM_ROWS) | (0x4 << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x18 << TENSOR_IMA_SCP_LOC_B) | (0x5 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fmvz.x.ps x31, f0, 0\n"
			"li x5, 0x14\n"
			"bne x5, x31, LBL_FAIL_HID_14\n"
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
			"csrwi mhpmevent3, 2\n"   
			"csrwi mhpmevent4, 1\n"   
			"csrwi mhpmevent5, 3\n"   
			"csrwi mhpmevent6, 6\n"   
			"csrwi mhpmevent7, 22\n"   
			"csrwi mhpmevent8, 2\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f6,  736 (x5)\n"
			"flw.ps f14,  896 (x5)\n"
			"flw.ps f15,  0 (x5)\n"
			"flw.ps f31,  832 (x5)\n"
			"flw.ps f5,  928 (x5)\n"
			"flw.ps f1,  32 (x5)\n"
			"flw.ps f19,  800 (x5)\n"
			"flw.ps f8,  352 (x5)\n"
			"flw.ps f11,  448 (x5)\n"
			"flw.ps f12,  992 (x5)\n"
			"flw.ps f24,  544 (x5)\n"
			"flw.ps f25,  672 (x5)\n"
			"flw.ps f3,  64 (x5)\n"
			"flw.ps f4,  640 (x5)\n"
			"flw.ps f27,  160 (x5)\n"
			"flw.ps f2,  704 (x5)\n"
			"flw.ps f20,  608 (x5)\n"
			"flw.ps f28,  416 (x5)\n"
			"flw.ps f18,  864 (x5)\n"
			"flw.ps f0,  576 (x5)\n"
			"flw.ps f26,  320 (x5)\n"
			"flw.ps f9,  512 (x5)\n"
			"flw.ps f30,  224 (x5)\n"
			"flw.ps f23,  480 (x5)\n"
			"flw.ps f13,  256 (x5)\n"
			"flw.ps f10,  192 (x5)\n"
			"flw.ps f17,  768 (x5)\n"
			"flw.ps f22,  128 (x5)\n"
			"flw.ps f21,  384 (x5)\n"
			"flw.ps f16,  288 (x5)\n"
			"flw.ps f29,  960 (x5)\n"
			"flw.ps f7,  96 (x5)\n"
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
			"la x15, rand_ieee754_32\n"
			"flw.ps f14, 448(x15)\n"
			"flw.ps f21, 768(x15)\n"
			"fle.s x7, f14, f21\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f10, 256(x15)\n"
			"flw.ps f7, 736(x15)\n"
			"flw.ps f9, 928(x15)\n"
			"fmadd.s f24, f10, f7, f9, rtz\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f21, 0(x28)\n"
			"flw.ps f2, 896(x28)\n"
			"fmax.s f1, f21, f2\n" 
			"mova.x.m x12\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f10, 544(x28)\n"
			"fcvt.wu.s x27, f10, rmm\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f6, 608(x15)\n"
			"fsqrt.ps f21, f6\n" 
			"la x5, rand_int_32\n"
			"flw.ps f30, 224(x5)\n"
			"flw.ps f10, 64(x5)\n"
			"fmin.pi f21, f30, f10\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f8, 320(x19)\n"
			"flw.ps f1, 704(x19)\n"
			"flw.ps f27, 832(x19)\n"
			"fmsub.ps f21, f8, f1, f27, rdn\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_15:\n"
			"maskand m7, m3, m6\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x5, rand_int_32\n"
			"flw.ps f9, 480(x5)\n"
			"fdivu.pi f9, f9, f9\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_15:\n"
			"maskor m1, m6, m1\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x7, rand_ieee754_32\n"
			"flw.ps f27, 736(x7)\n"
			"fcvt.pw.ps f6, f27, rne\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f10, 320(x19)\n"
			"flw.ps f13, 160(x19)\n"
			"fsub.s f9, f10, f13, rup\n" 
			"la x15, rand_int_32\n"
			"flw.ps f1, 480(x15)\n"
			"fnot.pi f8, f1\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f2, 192(x9)\n"
			"fcvt.f16.ps f21, f2\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f7, 800(x19)\n"
			"flw.ps f28, 192(x19)\n"
			"fsgnj.ps f24, f7, f28\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_15\n"
			"addi x30, x30, 1\n"
			"la x5, rand_ieee754_32\n"
			"flw.ps f6, 128(x5)\n"
			"fcvt.f16.ps f12, f6\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f9, 960(x20)\n"
			"flw.ps f28, 768(x20)\n"
			"flw.ps f14, 448(x20)\n"
			"fcmov.ps f7, f9, f28, f14\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f24, 704(x7)\n"
			"flw.ps f9, 576(x7)\n"
			"flw.ps f14, 384(x7)\n"
			"fmsub.ps f8, f24, f9, f14, rdn\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f21, 672(x12)\n"
			"flw.ps f2, 128(x12)\n"
			"fmadd.ps f2, f21, f2, f2, rup\n" 
			"la x9, rand_int_32\n"
			"flw.ps f8, 768(x9)\n"
			"fslli.pi f10, f8, 0xc\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f6, 64(x18)\n"
			"fcvt.f16.ps f13, f6\n" 
			"LBL_SEQID_1_M0_WRITE_ID_2_HID_15:\n"
			"mov.m.x m6, x5, 0x2a\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x5, rand_int_32\n"
			"flw.ps f8, 800(x5)\n"
			"flw.ps f27, 384(x5)\n"
			"fminu.pi f21, f8, f27\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f2, 800(x19)\n"
			"flw.ps f24, 128(x19)\n"
			"fsgnjn.ps f27, f2, f24\n" 
			"la x19, rand_ieee754_16\n"
			"flw.ps f30, 384(x19)\n"
			"fcvt.ps.f16 f8, f30\n" 
			"maskpopcz x19, m7\n" 
			"LBL_SEQID_1_FP_TRANS_ID_3_HID_15:\n"
			"la x13, rand_ieee754_32\n"
			"flw.ps f10, 32(x13)\n"
			"frcp.ps f28, f10\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f7, 544(x9)\n"
			"flw.ps f1, 256(x9)\n"
			"fsgnj.s f28, f7, f1\n" 
			"li x9, 0xd7bf20228c9c3afe\n"
			"fcvt.s.wu f24, x9, rmm\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f6, 928(x27)\n"
			"fcvt.pw.ps f9, f6, rdn\n" 
			"maskpopc x5, m7\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_15\n"
			"addi x30, x30, 1\n"
			"la x12, rand_ieee754_32\n"
			"flw.ps f12, 32(x12)\n"
			"flw.ps f20, 736(x12)\n"
			"fsub.s f24, f12, f20, rup\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f20, 736(x18)\n"
			"frsq.ps f10, f20\n" 
			"la x7, rand_int_32\n"
			"flw.ps f28, 864(x7)\n"
			"flw.ps f10, 128(x7)\n"
			"frem.pi f13, f28, f10\n" 
			"la x28, rand_int_32\n"
			"flw.ps f1, 992(x28)\n"
			"fcvt.ps.pwu f10, f1, rtz\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f27, 416(x28)\n"
			"flw.ps f13, 288(x28)\n"
			"fmul.ps f30, f27, f13, rtz\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f20, 224(x13)\n"
			"fsqrt.s f21, f20, rne\n" 
			"LBL_SEQID_2_M0_WRITE_ID_4_HID_15:\n"
			"masknot m3, m6\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x7, rand_int_32\n"
			"flw.ps f20, 704(x7)\n"
			"flw.ps f7, 576(x7)\n"
			"fremu.pi f30, f20, f7\n" 
			"la x13, rand_int_32\n"
			"flw.ps f14, 352(x13)\n"
			"flw.ps f2, 160(x13)\n"
			"for.pi f27, f14, f2\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f20, 64(x13)\n"
			"flw.ps f1, 32(x13)\n"
			"flw.ps f21, 512(x13)\n"
			"fcmov.ps f8, f20, f1, f21\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f10, 224(x7)\n"
			"flw.ps f20, 320(x7)\n"
			"fsgnjx.ps f2, f10, f20\n" 
			"LBL_SEQID_2_FP_TRANS_ID_5_HID_15:\n"
			"la x12, rand_ieee754_32\n"
			"flw.ps f28, 256(x12)\n"
			"fexp.ps f10, f28\n" 
			"li x28, 0xffb34b6b0c7dc1a3\n"
			"fcvt.s.w f12, x28, rmm\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f2, 992(x15)\n"
			"flw.ps f10, 512(x15)\n"
			"flw.ps f27, 288(x15)\n"
			"fnmsub.ps f27, f2, f10, f27, rmm\n" 
			"LBL_SEQID_2_FP_TRANS_ID_6_HID_15:\n"
			"la x15, rand_ieee754_32\n"
			"flw.ps f7, 512(x15)\n"
			"frcp.ps f28, f7\n" 
			"maskpopcz x20, m6\n" 
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
			"la x9, rand_ieee754_32\n"
			"flw.ps f20, 736(x9)\n"
			"flw.ps f1, 992(x9)\n"
			"fsgnj.ps f30, f20, f1\n" 
			"la x19, rand_int_32\n"
			"flw.ps f10, 672(x19)\n"
			"fslli.pi f8, f10, 0x3\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f10, 320(x5)\n"
			"flw.ps f30, 480(x5)\n"
			"flw.ps f12, 288(x5)\n"
			"fcmov.ps f6, f10, f30, f12\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f24, 64(x12)\n"
			"flw.ps f21, 608(x12)\n"
			"flw.ps f13, 800(x12)\n"
			"fnmsub.ps f30, f24, f21, f13, rdn\n" 
			"li x18, 0xccef8fcfb281b08e\n"
			"fcvt.s.wu f6, x18, rmm\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f12, 0(x27)\n"
			"flw.ps f20, 896(x27)\n"
			"flt.ps f21, f12, f20\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f2, 704(x15)\n"
			"flw.ps f8, 992(x15)\n"
			"flt.s x15, f2, f8\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f6, 480(x18)\n"
			"flw.ps f21, 480(x18)\n"
			"flt.ps f30, f6, f21\n" 
			"la x19, rand_int_32\n"
			"flw.ps f27, 928(x19)\n"
			"flw.ps f24, 576(x19)\n"
			"fadd.pi f20, f27, f24\n" 
			"la x5, rand_int_32\n"
			"flw.ps f14, 288(x5)\n"
			"faddi.pi f27, f14, 0xb\n" 
			"la x20, rand_int_32\n"
			"flw.ps f1, 32(x20)\n"
			"fandi.pi f8, f1, 0x107\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f20, 0(x27)\n"
			"flw.ps f7, 640(x27)\n"
			"flw.ps f10, 704(x27)\n"
			"fmadd.ps f28, f20, f7, f10, rtz\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f24, 384(x9)\n"
			"flt.s x28, f24, f24\n" 
			"la x15, rand_int_32\n"
			"flw.ps f12, 480(x15)\n"
			"faddi.pi f30, f12, 0x192\n" 
			"la x28, rand_ieee754_16\n"
			"flw.ps f10, 224(x28)\n"
			"fcvt.ps.f16 f1, f10\n" 
			"la x15, rand_int_32\n"
			"flw.ps f6, 800(x15)\n"
			"faddi.pi f1, f6, 0x15c\n" 
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
			"LBL_SEQID_4_M0_WRITE_ID_7_HID_15:\n"
			"maskxor m6, m6, m6\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x20, rand_ieee754_32\n"
			"flw.ps f27, 896(x20)\n"
			"fswizz.ps f9, f27, 0x6\n" 
			"la x19, rand_int_32\n"
			"flw.ps f9, 608(x19)\n"
			"fsrli.pi f21, f9, 0x7\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f2, 736(x12)\n"
			"flw.ps f30, 192(x12)\n"
			"fsgnjx.ps f6, f2, f30\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f24, 0(x15)\n"
			"flw.ps f6, 96(x15)\n"
			"flw.ps f28, 736(x15)\n"
			"fnmadd.ps f24, f24, f6, f28, rmm\n" 
			"LBL_SEQID_4_M0_WRITE_ID_8_HID_15:\n"
			"fltm.pi m7, f28, f30\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0xca2d3da3da\n"
			"fsw.ps f0, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"mova.x.m x20\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f6, 0(x5)\n"
			"fsin.ps f27, f6\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f14, 736(x12)\n"
			"flw.ps f30, 96(x12)\n"
			"fsgnj.ps f30, f14, f30\n" 
			"li x15, 0xd3255ed9948a726e\n"
			"fcvt.s.wu f21, x15, rmm\n" 
			"LBL_SEQID_4_M0_WRITE_ID_9_HID_15:\n"
			"flem.ps m6, f12, f6\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0x4eddf2018\n"
			"flw.ps f5, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x9, rand_int_32\n"
			"flw.ps f20, 448(x9)\n"
			"flw.ps f14, 128(x9)\n"
			"fmin.pi f14, f20, f14\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f1, 576(x9)\n"
			"flw.ps f24, 576(x9)\n"
			"fle.ps f2, f1, f24\n" 
			"la x12, rand_int_32\n"
			"flw.ps f24, 512(x12)\n"
			"fle.pi f1, f24, f24\n" 
			"LBL_SEQID_4_FP_TRANS_ID_10_HID_15:\n"
			"la x19, rand_ieee754_32\n"
			"flw.ps f2, 608(x19)\n"
			"flog.ps f2, f2\n" 
			"la x20, rand_int_32\n"
			"flw.ps f13, 0(x20)\n"
			"flw.ps f24, 352(x20)\n"
			"fsra.pi f8, f13, f24\n" 
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
			"la x15, rand_ieee754_32\n"
			"flw.ps f28, 256(x15)\n"
			"flw.ps f12, 512(x15)\n"
			"fsgnj.ps f6, f28, f12\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f6, 256(x28)\n"
			"flw.ps f1, 0(x28)\n"
			"fsgnj.s f7, f6, f1\n" 
			"maskpopcz x7, m1\n" 
			"la x20, rand_int_32\n"
			"flw.ps f20, 672(x20)\n"
			"fcvt.ps.pwu f14, f20, rtz\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f7, 544(x27)\n"
			"fsin.ps f12, f7\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f21, 832(x5)\n"
			"flw.ps f24, 768(x5)\n"
			"fcmovm.ps f14, f21, f24\n" 
			"la x28, rand_int_32\n"
			"flw.ps f12, 96(x28)\n"
			"flw.ps f27, 224(x28)\n"
			"fsll.pi f10, f12, f27\n" 
			"maskpopc x13, m7\n" 
			"la x27, rand_int_32\n"
			"flw.ps f7, 800(x27)\n"
			"flw.ps f21, 480(x27)\n"
			"fand.pi f13, f7, f21\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f1, 448(x28)\n"
			"flw.ps f2, 320(x28)\n"
			"fsub.s f21, f1, f2, rmm\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f28, 384(x9)\n"
			"ffrc.ps f10, f28\n" 
			"LBL_SEQID_5_M0_WRITE_ID_11_HID_15:\n"
			"fltm.pi m1, f1, f12\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x20, rand_ieee754_32\n"
			"flw.ps f8, 256(x20)\n"
			"flw.ps f13, 800(x20)\n"
			"fmax.s f20, f8, f13\n" 
			"la x7, rand_int_32\n"
			"flw.ps f6, 64(x7)\n"
			"fsat8.pi f6, f6\n" 
			"la x19, rand_int_32\n"
			"flw.ps f13, 512(x19)\n"
			"flw.ps f12, 160(x19)\n"
			"fand.pi f13, f13, f12\n" 
			"la x18, rand_int_32\n"
			"flw.ps f10, 832(x18)\n"
			"flw.ps f1, 640(x18)\n"
			"fmulh.pi f13, f10, f1\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 6\n" 
			"bne x30, x5, LBL_FAIL_HID_15\n"
			"addi x30, x30, 1\n"
			"la x15, rand_int_32\n"
			"flw.ps f7, 96(x15)\n"
			"fsrai.pi f9, f7, 0x0\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f30, 96(x12)\n"
			"flw.ps f8, 480(x12)\n"
			"fadd.s f21, f30, f8, rmm\n" 
			"la x12, rand_int_32\n"
			"flw.ps f12, 352(x12)\n"
			"flw.ps f21, 224(x12)\n"
			"fmin.pi f13, f12, f21\n" 
			"la x7, rand_int_32\n"
			"flw.ps f21, 864(x7)\n"
			"flw.ps f28, 384(x7)\n"
			"fsra.pi f28, f21, f28\n" 
			"la x15, rand_int_32\n"
			"flw.ps f14, 864(x15)\n"
			"flw.ps f2, 0(x15)\n"
			"fminu.pi f2, f14, f2\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f13, 672(x27)\n"
			"flw.ps f1, 576(x27)\n"
			"fmax.ps f10, f13, f1\n" 
			"LBL_SEQID_6_M0_WRITE_ID_12_HID_15:\n"
			"maskand m7, m1, m6\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x7, rand_ieee754_32\n"
			"flw.ps f24, 736(x7)\n"
			"fswizz.ps f1, f24, 0x10\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f6, 0(x9)\n"
			"flw.ps f8, 224(x9)\n"
			"fle.s x20, f6, f8\n" 
			"la x27, rand_int_32\n"
			"flw.ps f21, 192(x27)\n"
			"flw.ps f6, 480(x27)\n"
			"fxor.pi f14, f21, f6\n" 
			"maskpopcz x18, m1\n" 
			"fbci.pi f8, 0xb4bb\n" 
			"LBL_SEQID_6_FP_TRANS_ID_13_HID_15:\n"
			"la x28, rand_ieee754_32\n"
			"flw.ps f8, 224(x28)\n"
			"flog.ps f8, f8\n" 
			"la x5, rand_int_32\n"
			"flw.ps f1, 960(x5)\n"
			"fnot.pi f24, f1\n" 
			"la x15, rand_ieee754_16\n"
			"flw.ps f6, 704(x15)\n"
			"fcvt.ps.f16 f24, f6\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f28, 160(x18)\n"
			"flw.ps f10, 352(x18)\n"
			"fmax.s f14, f28, f10\n" 
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
		0x5ba11679, 
		0x72f6ac32, 
		0xf173028c, 
		0x7e2b07f8, 
		0x545cd141, 
		0xe48b5bf5, 
		0xf3e96afe, 
		0x7f26440d, 
		0x21fe7533, 
		0xab57be55, 
		0x7a6fe1be, 
		0x00f0c0f6, 
		0x85bf6045, 
		0x3099e9cd, 
		0x4ceb08c2, 
		0x5d7f5ee7, 
		0x89e029e1, 
		0x8a54039a, 
		0xb7c5df13, 
		0x2a637e40, 
		0x327f13a5, 
		0x7f3bfc5e, 
		0x16d59fb6, 
		0x09e8966b, 
		0x3e5c21d7, 
		0xf54bd00d, 
		0x4ca1a4c3, 
		0x3756b4ad, 
		0xf3437536, 
		0xcecd54a2, 
		0x89ef09aa, 
		0xb7ebdf3d, 
		0xa0264cf6, 
		0x654a4cab, 
		0x6697c4a1, 
		0x0428adcb, 
		0x13e3ced1, 
		0x441214c8, 
		0xf606cefe, 
		0x926a6420, 
		0x81835b5a, 
		0x8b146cd4, 
		0x9b534a00, 
		0xc596b524, 
		0xe0cba1f4, 
		0x921469da, 
		0xf7e4c661, 
		0x66cbc7bc, 
		0x72696f56, 
		0xab6b96b4, 
		0xc1e80cc2, 
		0x7133d44e, 
		0x29521453, 
		0x7ea1cb4b, 
		0x3c2c35e6, 
		0xf37b2fe2, 
		0xd12088a5, 
		0x5980fc4a, 
		0xe3cae476, 
		0x1fd41af5, 
		0xda4d1318, 
		0x73610b28, 
		0x2635f487, 
		0x5ad93c72, 
		0xad26f1b7, 
		0x9fd9034c, 
		0x126d8f3e, 
		0xeaff552c, 
		0xe84a5b50, 
		0x1473cd8a, 
		0xe95f4016, 
		0x943f30b9, 
		0x6b191ec4, 
		0x6f67a2ca, 
		0x80fe5308, 
		0x1f019481, 
		0x69f4aef2, 
		0x47f44b5f, 
		0xe890c53b, 
		0x1ff2dee7, 
		0xb0334862, 
		0x11fea68b, 
		0xea654e2c, 
		0x602ccf37, 
		0x82db667e, 
		0x319ed5c9, 
		0x50103b37, 
		0xe421ab75, 
		0xd8008d0d, 
		0x1c0c76b1, 
		0xc38edf2d, 
		0x6a2c4c2f, 
		0xf610a7ef, 
		0x05ca77d1, 
		0xdbec0b22, 
		0xd8fad670, 
		0x4808a84c, 
		0xa8435684, 
		0xff8aa60a, 
		0xdd2bc39a, 
		0xd2180f58, 
		0x5cc1aeee, 
		0xbdc80615, 
		0x1283ba76, 
		0x21089eac, 
		0xf483409b, 
		0xf40e2e65, 
		0x9f98e1ab, 
		0xb12718d9, 
		0x31f1a15f, 
		0x2806870d, 
		0x792a3518, 
		0x37c55bf0, 
		0xefd3b03f, 
		0xbdb593b0, 
		0xdaffd32e, 
		0xb82e332a, 
		0x9687a985, 
		0xd90fac46, 
		0xc4279452, 
		0x9c8f3437, 
		0xefd53c9d, 
		0x0cc321a8, 
		0x50d00c23, 
		0xbcdc46eb, 
		0x18b4b42a, 
		0x15fd6f21, 
		0x53dd3ea1, 
		0xa9336237, 
		0x9beecb67, 
		0xaf7331cc, 
		0x7f97418e, 
		0x30aa612f, 
		0x9485033b, 
		0xc30962e5, 
		0x5cba8a40, 
		0x36bc066d, 
		0x318a790b, 
		0x9dfe30ca, 
		0x41ee780c, 
		0x58168c56, 
		0x184b9df8, 
		0xb3bb1612, 
		0x8dff6861, 
		0xc5e16a82, 
		0x703e90fc, 
		0x2589040c, 
		0x02210b88, 
		0x51d97833, 
		0xd9a29c96, 
		0xe7fb3fb7, 
		0xc892a417, 
		0x6595c202, 
		0x0f592e05, 
		0x2b073baf, 
		0xad25d9b5, 
		0x464c1bcb, 
		0x206acd75, 
		0xa8aea581, 
		0x37e5cada, 
		0x5e91966c, 
		0xb3e902ba, 
		0xef1c7bd2, 
		0xa29ac12e, 
		0x69535ef6, 
		0xbbdfd58a, 
		0xa1726619, 
		0x24cb77ab, 
		0x5fe2c5ea, 
		0xbaad3a54, 
		0x2ce55216, 
		0x3168b907, 
		0xa98d00e2, 
		0x4b81457e, 
		0xb198002d, 
		0x7880cf08, 
		0x60c78083, 
		0xf3c31abb, 
		0x6c299719, 
		0xdcf1101f, 
		0x2a05e39f, 
		0x188464af, 
		0xd7909b9b, 
		0x395f5bbe, 
		0x5866bfd6, 
		0xfa068714, 
		0xa466928b, 
		0xdcdbdffe, 
		0x63e8ba68, 
		0x9cdd7188, 
		0xee78a325, 
		0x58a26d41, 
		0x1bdc5ba5, 
		0x79bba4e0, 
		0x2c37fad9, 
		0x90ab11ed, 
		0xbc5bc031, 
		0xc5b7d705, 
		0x36c51613, 
		0x76497cad, 
		0xeb706470, 
		0xd449d08d, 
		0x7eb472e3, 
		0x5f2ec05c, 
		0x3d18cf60, 
		0xab6563f0, 
		0x65b0e304, 
		0x4f191959, 
		0x039d8896, 
		0x464f5daa, 
		0xe4def867, 
		0x5b32d648, 
		0xe0cde239, 
		0xfa0ccfad, 
		0x9866ac71, 
		0x09b09e63, 
		0xf0906d8e, 
		0x7bff7d2b, 
		0x074ef4f9, 
		0xa14d51dc, 
		0xfe6aa29a, 
		0xaab95d08, 
		0x3158c77c, 
		0x3d0a8782, 
		0xf10d2017, 
		0x0d535ffd, 
		0x3c4147ca, 
		0x2058d913, 
		0x2965fcf6, 
		0x80a04ae7, 
		0x16dd3596, 
		0xacc11b6f, 
		0x19667aa7, 
		0x66578605, 
		0x6a812070, 
		0x79f4877a, 
		0xc6aa0813, 
		0x078353ac, 
		0x4b3827ef, 
		0xb460aaa3, 
		0xc23f5b40, 
		0xcbb7bd7e, 
		0xad031dd7, 
		0x65736bb7, 
		0x35dadae9, 
		0xe50a92fa, 
		0x4bf3737d, 
		0x792b3670, 
		0x20c8ed5e, 
		0x757f6174, 
		0x5cf8549a, 
		0xcbfca612, 
		0xc1897f86, 
		0x41873a0d, 
		0xb197d017, 
		0xdde27e54  
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
		0xcb00, 
		0xcc00, 
		0xc980, 
		0x5260, 
		0x0000, 
		0xc980, 
		0xfc00, 
		0x5280, 
		0xd380, 
		0xdd97, 
		0xd2c0, 
		0x5020, 
		0x8000, 
		0xf41d, 
		0xd140, 
		0x5060, 
		0x1123, 
		0x5080, 
		0xd240, 
		0x52a0, 
		0x0000, 
		0x8000, 
		0xd240, 
		0x897f, 
		0x0000, 
		0xe0a2, 
		0xc000, 
		0xd100, 
		0x4e80, 
		0x0b02, 
		0x5280, 
		0x4e00, 
		0xf84c, 
		0xd060, 
		0x8000, 
		0x5300, 
		0x8000, 
		0xcf00, 
		0x5340, 
		0x5040, 
		0xcf80, 
		0x5000, 
		0x5120, 
		0xcc80, 
		0x409b, 
		0x4800, 
		0x4f80, 
		0xd1e0, 
		0x4dc0, 
		0x50e0, 
		0x5b31, 
		0xaeb4, 
		0x2c96, 
		0xf5d7, 
		0xce00, 
		0x4e40, 
		0x45b5, 
		0xcec0, 
		0xc900, 
		0x5340, 
		0xd0c0, 
		0xb224, 
		0x8000, 
		0x4fc0, 
		0x0000, 
		0x5040, 
		0x3c00, 
		0x9706, 
		0x4a00, 
		0x5020, 
		0xd040, 
		0x52e0, 
		0xd320, 
		0xc4e8, 
		0xd260, 
		0xc600, 
		0xc980, 
		0xd0a0, 
		0xc7a5, 
		0x8000, 
		0x5060, 
		0xcf40, 
		0x4980, 
		0xd300, 
		0xd2a0, 
		0x5080, 
		0x55b5, 
		0xec6d, 
		0x53c0, 
		0x5360, 
		0xc590, 
		0x52ec, 
		0x4400, 
		0xd3a0, 
		0x52a0, 
		0x7d72, 
		0xcc40, 
		0xcc40, 
		0x7b28, 
		0x4c00, 
		0xc800, 
		0x95c7, 
		0xd3e0, 
		0x5220, 
		0xbfc0, 
		0x71a4, 
		0xd1c0, 
		0x4cc0, 
		0x52c0, 
		0x4500, 
		0x4c80, 
		0x4d00, 
		0x8000, 
		0xb915, 
		0x24ae, 
		0x9246, 
		0xcad7, 
		0xc500, 
		0x452a, 
		0x8000, 
		0xd320, 
		0x4abe, 
		0x53a0, 
		0xccc0, 
		0xd1c0, 
		0x53c0, 
		0xce00, 
		0x4700, 
		0x4e40, 
		0xc980, 
		0xb522, 
		0xd160, 
		0xcc80, 
		0x94c9, 
		0x0000, 
		0x5360, 
		0x62cf, 
		0x4f80, 
		0xfc00, 
		0xd2a0, 
		0x52c0, 
		0xcfc0, 
		0x4c80, 
		0xd060, 
		0x0000, 
		0x79d3, 
		0xce40, 
		0xbc00, 
		0xc000, 
		0x85ae, 
		0xce40, 
		0x8000, 
		0xa82c, 
		0x5080, 
		0x4500, 
		0x7c00, 
		0xcdc0, 
		0xdc7b, 
		0x4000, 
		0x34c4, 
		0x8797, 
		0x89d2, 
		0x7a87, 
		0xd1c0, 
		0xcd80, 
		0x0f05, 
		0xd0c0, 
		0x0cec, 
		0xfd00, 
		0x3c00, 
		0x4b80, 
		0x53c0, 
		0xdf86, 
		0x4ec0, 
		0x3306, 
		0x7764, 
		0xce80, 
		0xd180, 
		0xdc48, 
		0x4500, 
		0x4f40, 
		0x4a80, 
		0xd040, 
		0x0000, 
		0x4b00, 
		0xb3bc, 
		0x5160, 
		0x50e0, 
		0xee22, 
		0x52e0, 
		0xadeb, 
		0x4f00, 
		0x51a0, 
		0x50e0, 
		0xce90, 
		0x0000, 
		0xd0a0, 
		0x12e6, 
		0x5180, 
		0x5240, 
		0xfc00, 
		0x5ee3, 
		0x52c0, 
		0xd080, 
		0xcd40, 
		0x7c00, 
		0xd0e0, 
		0x0000, 
		0x5c05, 
		0xc880, 
		0x5240, 
		0x1bb8, 
		0x8000, 
		0xc800, 
		0x4e00, 
		0x2274, 
		0xa30d, 
		0x48cd, 
		0xaff2, 
		0xd3c0, 
		0x4d00, 
		0xd549, 
		0xfca1, 
		0x8000, 
		0x4f00, 
		0x7c00, 
		0x22d7, 
		0xbc91, 
		0xcd40, 
		0x8000, 
		0x52c0, 
		0x11c6, 
		0x534f, 
		0x64ab, 
		0x51e0, 
		0xcc00, 
		0x1a0d, 
		0xcc00, 
		0x0000, 
		0xaf18, 
		0x5300, 
		0xd020, 
		0xc400, 
		0x4000, 
		0xd240, 
		0xd140, 
		0x4e80, 
		0x3c00, 
		0x6ce4, 
		0x8000, 
		0x4980, 
		0xcf40, 
		0x51a0, 
		0x5080, 
		0x39e8, 
		0x0000, 
		0x5360, 
		0xd280, 
		0x50c0, 
		0xd040, 
		0xd3e0, 
		0x8000, 
		0xbc00, 
		0x4d00, 
		0x3081, 
		0x2fc5, 
		0xd2a0, 
		0x8000, 
		0xd0e0, 
		0xd3e0, 
		0xce00, 
		0x4800, 
		0xd3a0, 
		0xfc15, 
		0x6f71, 
		0xd2c0, 
		0xd120, 
		0xd120, 
		0x9728, 
		0x8000, 
		0xcb80, 
		0xd320, 
		0x53a0, 
		0x5060, 
		0xd0c0, 
		0x0074, 
		0x3a78, 
		0x52c0, 
		0x4f00, 
		0x5160, 
		0x5320, 
		0x8000, 
		0xd1c0, 
		0xd280, 
		0x6959, 
		0x4e80, 
		0x7f4f, 
		0x4c80, 
		0x5320, 
		0x5300, 
		0x50e0, 
		0xce00, 
		0x4f00, 
		0x52e0, 
		0x4c40, 
		0xcec0, 
		0xcc00, 
		0xcd80, 
		0x7675, 
		0xd320, 
		0x4d80, 
		0xef1d, 
		0x2a5c, 
		0x5320, 
		0x0000, 
		0xb855, 
		0x4980, 
		0x4900, 
		0xd080, 
		0x4d00, 
		0x8000, 
		0xcf80, 
		0x61e5, 
		0x7b3e, 
		0xd120, 
		0x4ec0, 
		0x3ccd, 
		0x50a0, 
		0x5240, 
		0x8000, 
		0xd0a0, 
		0xcc40, 
		0x7c00, 
		0xea5b, 
		0xd0e0, 
		0x21e9, 
		0x4880, 
		0x4d00, 
		0xc700, 
		0x4f80, 
		0x8375, 
		0xfc00, 
		0x4ec0, 
		0x7d40, 
		0xd997, 
		0xca80, 
		0x5100, 
		0xae2d, 
		0x5340, 
		0xf12c, 
		0x5320, 
		0xc880, 
		0x5220, 
		0xcec0, 
		0x4980, 
		0x0000, 
		0x53e0, 
		0x53c0, 
		0xa009, 
		0x80f2, 
		0xae19, 
		0x0000, 
		0xc880, 
		0x5425, 
		0xcb00, 
		0xab41, 
		0x5360, 
		0xa0e1, 
		0x1f0c, 
		0x3c00, 
		0xfc00, 
		0x51c0, 
		0xfe64, 
		0xab68, 
		0xb3f3, 
		0x08b5, 
		0x51e0, 
		0x4f40, 
		0x943f, 
		0xc511, 
		0x8000, 
		0xc000, 
		0x4b4a, 
		0xcf40, 
		0xbb09, 
		0x7202, 
		0x4c00, 
		0x4980, 
		0xd200, 
		0xbc00, 
		0xd0a0, 
		0x0000, 
		0x5000, 
		0xcd40, 
		0xd320, 
		0xcdc0, 
		0xcdc0, 
		0xb798, 
		0xd380, 
		0xc500, 
		0xfc00, 
		0x4200, 
		0x8000, 
		0x7c00, 
		0x140b, 
		0x53a0, 
		0xd0c0, 
		0x4880, 
		0x4b71, 
		0x4200, 
		0xe9bc, 
		0xcd40, 
		0x5180, 
		0xdbfb, 
		0xc880, 
		0x8000, 
		0xd020, 
		0x4700, 
		0xf895, 
		0xa3c6, 
		0x0bef, 
		0x9fd2, 
		0x4f80, 
		0x5020, 
		0xc900, 
		0xa082, 
		0x56ee, 
		0xd020, 
		0xd052, 
		0xd240, 
		0xf72f, 
		0x5100, 
		0x4600, 
		0x4f40, 
		0x7d86, 
		0x3c00, 
		0xd440, 
		0xedd2, 
		0x8000, 
		0x5565, 
		0x0110, 
		0x4cc0, 
		0x4b00, 
		0xcdc0, 
		0x42db, 
		0x52d1, 
		0xfbf6, 
		0x4e40, 
		0xc400, 
		0xcf40, 
		0xd0a0, 
		0xd2a0, 
		0xeef3, 
		0xd060, 
		0xdba5, 
		0x0917, 
		0xfc00, 
		0xe4e7, 
		0x3273, 
		0xce40, 
		0x7c3b, 
		0x50e0, 
		0xd1e0, 
		0x5220, 
		0xc500, 
		0xd060, 
		0x2acf, 
		0xd200, 
		0xaeeb, 
		0xcd00, 
		0xc646, 
		0xd320, 
		0xcf00, 
		0x810b, 
		0x38f7, 
		0xcd40, 
		0x3c00, 
		0xfc00, 
		0x5080, 
		0x4000, 
		0x1e1f, 
		0x0000, 
		0xaaa9, 
		0x3f7e, 
		0xccc0, 
		0x4f80, 
		0x4b00, 
		0x5000, 
		0xfc00, 
		0x0d2d, 
		0x5320, 
		0xd2e0, 
		0x5280, 
		0x2ce2, 
		0xf6cb, 
		0x5060, 
		0xd080, 
		0x0bc2, 
		0x4dc0, 
		0xcec0, 
		0xd140, 
		0xc657, 
		0xd1c0, 
		0xd320, 
		0x51c0, 
		0xfa88, 
		0x4b80, 
		0xc200, 
		0x3bce, 
		0x7c00, 
		0xcd80, 
		0x4c80, 
		0x8df3, 
		0xd120, 
		0xd120, 
		0x4e40, 
		0x0000, 
		0x7d3b, 
		0x5040, 
		0x50a0, 
		0xd3a0, 
		0x4cc0, 
		0x4800, 
		0x34ae, 
		0x8000, 
		0x53e0, 
		0x7c0e, 
		0xd54f, 
		0xcb80, 
		0x0000, 
		0xc700, 
		0x0000, 
		0xdc6a, 
		0xdd62, 
		0xcc80, 
		0x4fc0, 
		0xbd0f, 
		0x4200, 
		0xb9b1, 
		0xfc00, 
		0x3bf6, 
		0x4e80, 
		0xccc0, 
		0x51a0, 
		0xb7f4, 
		0xcc00, 
		0x52a0, 
		0x4880, 
		0x4f80, 
		0xb63c, 
		0x50c0, 
		0x8000, 
		0x4726, 
		0x4800, 
		0x23a3, 
		0x57de, 
		0x329a, 
		0xd1c0, 
		0xd320, 
		0xbbf6, 
		0x4500, 
		0xd2c0, 
		0xc971, 
		0x5340, 
		0x71b1, 
		0x3c00, 
		0xd060, 
		0x7b18, 
		0xd240, 
		0xd160, 
		0x18b3, 
		0x50a0, 
		0xd060, 
		0x5240, 
		0xfcab, 
		0x0000, 
		0x3c00, 
		0x5200, 
		0x4a00, 
		0x4000, 
		0xac61, 
		0x0000, 
		0x3c4c, 
		0x4377, 
		0xce80, 
		0x53c0, 
		0x4b80, 
		0x4900, 
		0x13a6, 
		0xdbe7, 
		0xc2d1, 
		0x5380, 
		0xc700, 
		0xcc40, 
		0xe3b1, 
		0xc200, 
		0x8000, 
		0xcd00, 
		0x7c00, 
		0x4f00, 
		0x5220, 
		0x4400, 
		0x394b, 
		0xd000, 
		0x3ee2, 
		0xcdc0, 
		0x5380, 
		0x7c00, 
		0xca00, 
		0xb482, 
		0x538b, 
		0xd360, 
		0xcfc0, 
		0x4b00, 
		0xcb80, 
		0x1fb3, 
		0x37dc, 
		0x51a0, 
		0xfc00, 
		0xcc80, 
		0xd040, 
		0xaa70, 
		0x50e0, 
		0xd360, 
		0xe40a, 
		0x4e00, 
		0x4980, 
		0xc880, 
		0x7c1f, 
		0xd040, 
		0xd000, 
		0xd340, 
		0x5260, 
		0x7c00, 
		0x45e8, 
		0xaa22, 
		0xc400, 
		0xd2a0, 
		0x5100, 
		0xce00, 
		0xd1c0, 
		0x627e, 
		0xd3a0, 
		0xb29e, 
		0xd340, 
		0x5c55, 
		0x05f9, 
		0xba71, 
		0xc900, 
		0x5140, 
		0x4dc0, 
		0x1002, 
		0xc880, 
		0x8000, 
		0x00d3, 
		0x3c89, 
		0x52a0, 
		0xd040, 
		0x4b80, 
		0x152e, 
		0x4b00, 
		0x7c00, 
		0x5140, 
		0x9ed0, 
		0x4600, 
		0x5060, 
		0x53e0, 
		0x7c00, 
		0x47d9, 
		0x4c00, 
		0x5000, 
		0x9778, 
		0x4980, 
		0xd3c0, 
		0xab9c, 
		0xcb80, 
		0xcec0, 
		0x1ad9, 
		0x5e3d, 
		0xcc00, 
		0xa6b7, 
		0xda92, 
		0x449c, 
		0xd120, 
		0x9946, 
		0x0d13, 
		0x8000, 
		0xce40, 
		0x1c57, 
		0x51c0, 
		0x01f0, 
		0x4e00, 
		0xcd80, 
		0xcf80, 
		0xd0c0, 
		0x9212, 
		0xd2a0, 
		0xcfc0, 
		0xcd80, 
		0xcb3b, 
		0x4800, 
		0xd360, 
		0x66bf, 
		0xbd23, 
		0x4a00, 
		0x2e28, 
		0x7c00, 
		0x4d80, 
		0x50e0, 
		0x2722, 
		0xc880, 
		0x5300, 
		0xc800, 
		0xccc0, 
		0x51c0, 
		0x5140, 
		0xd000, 
		0x52c0, 
		0x1460, 
		0xd020, 
		0x4ec0, 
		0x4fc0, 
		0x9fd4, 
		0xcf80, 
		0xd1a0, 
		0x7c00, 
		0xed4b, 
		0xd160, 
		0x7d67, 
		0x4c40, 
		0xa1af, 
		0x5140, 
		0x8000, 
		0x4ef6, 
		0x4c80, 
		0x0000, 
		0xc980, 
		0xcd40, 
		0xcc00, 
		0xcd00, 
		0xc700, 
		0xcc40, 
		0x4880, 
		0x5fad, 
		0x2be3, 
		0xc200, 
		0x05c3, 
		0xd320, 
		0xcd80, 
		0x4a00, 
		0x5060, 
		0x52e0, 
		0xd260, 
		0x4880, 
		0xd2a0, 
		0x85eb, 
		0x5160, 
		0x5380, 
		0x4e00, 
		0xc500, 
		0x684c, 
		0xc9ac, 
		0xc656, 
		0x5080, 
		0x4980, 
		0x566b, 
		0xe449, 
		0x4cc0, 
		0x4e80, 
		0x52e0, 
		0x8108, 
		0x8000, 
		0x0000, 
		0x4e00, 
		0x6e3e, 
		0xd000, 
		0x4b00, 
		0x5240, 
		0x5240, 
		0xd180, 
		0x0000, 
		0x2103, 
		0xd468, 
		0x5340, 
		0x52c0, 
		0xc000, 
		0x4980, 
		0xd320, 
		0xce00, 
		0xd200, 
		0xd2c0, 
		0xd000, 
		0x5080, 
		0xeeda, 
		0x51e0, 
		0x7c00, 
		0xd3a0, 
		0xd3e0, 
		0x4d40, 
		0xd3a0, 
		0xd120, 
		0x4d80, 
		0x6c1a, 
		0xcd00, 
		0x5080, 
		0x5200, 
		0x5340, 
		0xeaf9, 
		0xcb00, 
		0x5080, 
		0x0f18, 
		0xb723, 
		0x3c00, 
		0xd1a0, 
		0x4980, 
		0xcc00, 
		0x12b4, 
		0x4e80, 
		0xd3c0, 
		0xce00, 
		0x5360, 
		0x42b5, 
		0x5180, 
		0x225c, 
		0xbd16, 
		0xd3a0, 
		0xd3c0, 
		0xad99, 
		0xd0c0, 
		0xb1ab, 
		0x53c0, 
		0x8271, 
		0xcf80, 
		0xcb00, 
		0x164e, 
		0xfc00, 
		0xce80, 
		0x53bb, 
		0xd380, 
		0x8d24, 
		0x0000, 
		0x5260, 
		0x5360, 
		0x52e0, 
		0x5340, 
		0xd3e0, 
		0xc880, 
		0x8000, 
		0x5320, 
		0x8000, 
		0xcf40, 
		0x4fc0, 
		0xfcb7, 
		0x5060, 
		0x187d, 
		0x4000, 
		0x8000, 
		0xcf40, 
		0x78c6, 
		0x4a80, 
		0x5140, 
		0xd180, 
		0xd140, 
		0x4000, 
		0x51e0, 
		0x0a8c, 
		0x4000, 
		0x5360, 
		0x64a5, 
		0x4cc0, 
		0x3049, 
		0x3b3e, 
		0xb768, 
		0xc215, 
		0xd360, 
		0xa16c, 
		0xfc48, 
		0x3625, 
		0xc200, 
		0x51c0, 
		0x4cc0, 
		0xc880, 
		0x8000, 
		0xd280, 
		0x4fc0, 
		0xcb80, 
		0x5120, 
		0x5080, 
		0x5200, 
		0xaeb5, 
		0x59be, 
		0x724b, 
		0x5200, 
		0x0000, 
		0x5240, 
		0xb205, 
		0xa9ec, 
		0x4b80, 
		0x425c, 
		0x5380, 
		0xcf00, 
		0x51c0, 
		0xc200, 
		0x53e0, 
		0x50e8, 
		0xe35b, 
		0xcc40, 
		0x4f00, 
		0xce80, 
		0xd3e0, 
		0xfc00, 
		0x4800, 
		0xc600, 
		0x14a4, 
		0xc611, 
		0x6afd, 
		0x5daa, 
		0xd1e0, 
		0x4a00, 
		0xd100, 
		0x5200, 
		0x53a0, 
		0xca00, 
		0xd360, 
		0xd0e0, 
		0x5280, 
		0xd280, 
		0xada3, 
		0x5080, 
		0xccc0, 
		0xbc00, 
		0x4a54, 
		0x3c00, 
		0x8530, 
		0x5200, 
		0xfc00, 
		0xc600, 
		0xd2a0, 
		0x4789, 
		0xd0c0, 
		0x6a66, 
		0xcdc0, 
		0x51e0, 
		0xd1e0, 
		0x83b3, 
		0x5320, 
		0xd2a0, 
		0xd200, 
		0x4880, 
		0x4f80, 
		0xae6a, 
		0xcd80, 
		0xfa6f, 
		0xd200, 
		0xfd65, 
		0x79f9, 
		0xc600, 
		0x52c0, 
		0xd360, 
		0xd1c0, 
		0xd180, 
		0xd140, 
		0x5360, 
		0x50a0, 
		0xbc00, 
		0xcf00, 
		0xd000, 
		0xcc84, 
		0x4600, 
		0xd340, 
		0x0107, 
		0x28c1, 
		0xf36f, 
		0x8000, 
		0x52c0, 
		0xd619, 
		0xd3c0, 
		0x52c0, 
		0x239d, 
		0xd0e0, 
		0x5280, 
		0x4900, 
		0xc5db, 
		0xd140, 
		0xce00, 
		0xd160, 
		0x4f40, 
		0xc700, 
		0xdab0, 
		0x5320, 
		0x5310, 
		0x604d, 
		0xccc0, 
		0x4a00, 
		0xd280, 
		0x53e0, 
		0xce80, 
		0xd220, 
		0xd3c0, 
		0x4800, 
		0xc386, 
		0xf19f, 
		0x4b00, 
		0xd080, 
		0x29cf, 
		0xd020, 
		0x0137, 
		0x3c93, 
		0xd220, 
		0xf78e, 
		0xd0e0, 
		0x0000, 
		0x2cb3, 
		0x4800, 
		0xfc00, 
		0x98fd, 
		0xb561, 
		0x54bd, 
		0x4e80, 
		0xf1c1, 
		0x7c00, 
		0x7661, 
		0x182d, 
		0xcf80, 
		0x4f00, 
		0x0000, 
		0xfc00, 
		0x58a6, 
		0x5320, 
		0x4dc0, 
		0x4a34, 
		0x5040, 
		0x52e0, 
		0x5360, 
		0xc000, 
		0x3c00, 
		0xdd81, 
		0x3af3, 
		0xf49c, 
		0xd340, 
		0x451b, 
		0x3a54, 
		0x4dc0, 
		0xd040, 
		0xcec0, 
		0xce40, 
		0xcfc0, 
		0x923c, 
		0x8000, 
		0xd0c0, 
		0xdb20, 
		0xd180, 
		0xc400, 
		0x52a0, 
		0x53c0, 
		0xc200, 
		0x50a0, 
		0x7d49, 
		0xc700, 
		0xbde5, 
		0xd0c0, 
		0xd0a0, 
		0x575f, 
		0x4a80, 
		0xf72d, 
		0x7c00, 
		0xd040, 
		0xd3e0, 
		0xcd49, 
		0xd1e0, 
		0xfce4, 
		0x8f09, 
		0xc600, 
		0xd1a0, 
		0x5340, 
		0x4cc0, 
		0xcb80, 
		0x4f80, 
		0xc400, 
		0xca80, 
		0xdd04, 
		0xf95c, 
		0xfc4f, 
		0xd060, 
		0xd0e0, 
		0x53e0, 
		0x50a0, 
		0xce00, 
		0x2675, 
		0xd3e0, 
		0xc0d5, 
		0x4000, 
		0xd320, 
		0x4445, 
		0xd360, 
		0x1849, 
		0x4e40, 
		0x52a0, 
		0xcd40, 
		0x72cc, 
		0x5220, 
		0x5300, 
		0xe03f, 
		0x2b95, 
		0x4400, 
		0x4c80, 
		0x2a9c, 
		0x7c00, 
		0xc000, 
		0x4000, 
		0x4dc0, 
		0xc500, 
		0x52c0, 
		0x0000, 
		0x36bf, 
		0x03f5, 
		0x5340, 
		0xcb00, 
		0xc500, 
		0x0000, 
		0xd320, 
		0x4f40, 
		0xd360, 
		0xd2a0, 
		0xcec0, 
		0x8000, 
		0x4880, 
		0x68eb, 
		0xc236, 
		0xcf00, 
		0x5069, 
		0xd200, 
		0xc800, 
		0xf4fd, 
		0x5368, 
		0xc6db, 
		0x4e80, 
		0xe7df, 
		0x5340, 
		0x5280, 
		0x53a0, 
		0x51c0, 
		0x50a0, 
		0xc700, 
		0x4d80, 
		0x5260, 
		0xe102, 
		0x4e00, 
		0x326a, 
		0xd140, 
		0x4700, 
		0x50e0, 
		0xcdc0, 
		0x9594, 
		0xce80, 
		0x8000, 
		0xc600, 
		0x50a0, 
		0x50a0, 
		0xd340, 
		0x5280, 
		0x3820, 
		0xf7e3, 
		0xd2a0, 
		0x4980, 
		0xd080, 
		0x4400, 
		0xd340, 
		0xd220, 
		0xd000, 
		0xd3a0, 
		0xcc80, 
		0x1260, 
		0x4980, 
		0xcd00, 
		0xd180, 
		0x5360, 
		0x7291, 
		0x50a0, 
		0xd180, 
		0x6818, 
		0xfce3, 
		0xdaeb, 
		0xce00, 
		0xcd00, 
		0xfc00, 
		0xd0a0, 
		0xc900, 
		0xbcc7, 
		0xf9c3, 
		0xfd62, 
		0x7c00, 
		0x2010, 
		0x4900, 
		0x4400, 
		0x8cbb, 
		0xd120, 
		0x6e6e, 
		0xcd40, 
		0xd1e0, 
		0x0000, 
		0xd1e0, 
		0xd380, 
		0xeb2e, 
		0x5260, 
		0x1ead, 
		0xf6d4, 
		0x8364, 
		0xcc80, 
		0x69ff, 
		0xd140, 
		0xd2c0, 
		0xcb80, 
		0x3996, 
		0x4400, 
		0x51e0, 
		0x4e78, 
		0xcd40, 
		0xd300, 
		0xc400, 
		0x0000, 
		0xcc40, 
		0x8dbf, 
		0x4d80, 
		0x4a80, 
		0x5535, 
		0xcf00, 
		0xd060, 
		0xccc0, 
		0xcb80, 
		0xd1a0, 
		0x8e59, 
		0x7ce8, 
		0xd160, 
		0xcc40, 
		0x5000, 
		0x4f40, 
		0x4dc0, 
		0x85c8, 
		0x4400, 
		0x8000, 
		0xca80, 
		0x50a0, 
		0x4c00, 
		0xd1a0, 
		0xd2c0, 
		0x7c09, 
		0xcf40, 
		0x5823, 
		0xe221, 
		0xd3c0, 
		0x51a0, 
		0xcf00, 
		0x011d, 
		0x4500, 
		0x9b8c, 
		0xd360, 
		0x4700, 
		0x5140, 
		0x4dc0, 
		0xbc00, 
		0x5260, 
		0x5380, 
		0xcb80, 
		0x53e0, 
		0xece2, 
		0xc1dd, 
		0x273f, 
		0x3459, 
		0xeb9e, 
		0xef00, 
		0x50a0, 
		0x4c80, 
		0x4e40, 
		0x74fc, 
		0x46cf, 
		0x2b94, 
		0x7cc0, 
		0x51c0, 
		0xfd97, 
		0xcb80, 
		0x4fc0, 
		0x4200, 
		0x5c81, 
		0x8000, 
		0xcc80, 
		0x5380, 
		0x4c40, 
		0x4400, 
		0x0dff, 
		0xcdc0, 
		0xd2a0, 
		0xd2e0, 
		0xcdc0, 
		0xae26, 
		0x4800, 
		0x5320, 
		0xab07, 
		0xcf40, 
		0xc9c8, 
		0xd240, 
		0xbc00, 
		0xd300, 
		0xfd01, 
		0x8878, 
		0xbf3c, 
		0x5200, 
		0x4200, 
		0xfc74, 
		0xd3c0, 
		0xd300, 
		0x5160, 
		0x52c0, 
		0x35bc, 
		0xa17d, 
		0xd120, 
		0x4200, 
		0x8000, 
		0x4c40, 
		0x4f80, 
		0xfc00, 
		0x7c00, 
		0x7135, 
		0x7473, 
		0xd0c0, 
		0xc000, 
		0x32a7, 
		0x5a53, 
		0xd0e0, 
		0xd3e0, 
		0xd120, 
		0xd200, 
		0x0000, 
		0x3619, 
		0x1c70, 
		0x8000, 
		0x393e, 
		0x8000, 
		0xd060, 
		0x52a0, 
		0x5360, 
		0x4a6b, 
		0xd080, 
		0xd160, 
		0xd200, 
		0x8552, 
		0xd060, 
		0xf4ff, 
		0x8000, 
		0x8000, 
		0xd100, 
		0xcd40, 
		0xd140, 
		0xd180, 
		0xd240, 
		0xd220, 
		0xc200, 
		0x2215, 
		0x51c0, 
		0xd200, 
		0xf07a, 
		0xce00, 
		0x0000, 
		0xcc00, 
		0x535c, 
		0x52a0, 
		0xc980, 
		0x5020, 
		0x0000, 
		0x0ae5, 
		0xfc00, 
		0x8479, 
		0xcd00, 
		0x5240, 
		0x52a0, 
		0x5120, 
		0x6698, 
		0xcfc0, 
		0x4f4a, 
		0xfc00, 
		0x2a32, 
		0x4a80, 
		0xd160, 
		0xd000, 
		0xa37a, 
		0xd220, 
		0x5260, 
		0xb71c, 
		0x50e0, 
		0x53c0, 
		0xd360, 
		0xcc40, 
		0x5220, 
		0xd160, 
		0xcc80, 
		0xce00, 
		0x4980, 
		0xcf69, 
		0xcd40, 
		0x3704, 
		0xd2a0, 
		0xcf40, 
		0xd7fb, 
		0x5340, 
		0x5180, 
		0x4400, 
		0x4dc0, 
		0x58bc, 
		0x4634, 
		0x314a, 
		0x8000, 
		0x9d7d, 
		0x52a0, 
		0xe37a, 
		0x4d40, 
		0x910b, 
		0x9bb9, 
		0x5280, 
		0x51a0, 
		0xd3e0, 
		0xf1d5, 
		0xe661, 
		0x5320, 
		0xf6cc, 
		0x0000, 
		0x6b4c, 
		0xfd8f, 
		0xd380, 
		0xd200, 
		0x8000, 
		0x51c0, 
		0xc200, 
		0xd260, 
		0x52a0, 
		0x5060, 
		0xd140, 
		0x4a00, 
		0x0000, 
		0x38b9, 
		0xcee6, 
		0xd220, 
		0xc500, 
		0xcc80, 
		0xd160, 
		0xa73b, 
		0x8000, 
		0xd3e0, 
		0x7580, 
		0x7c00, 
		0x8000, 
		0x8000, 
		0x9e0c, 
		0x52a0, 
		0xd260, 
		0x4cc0, 
		0x5140, 
		0xf182, 
		0x4b00, 
		0xd0b7, 
		0x3c5b, 
		0xfc00, 
		0x1be3, 
		0xaf9c, 
		0x4f00, 
		0x5260, 
		0x30a3, 
		0x7810, 
		0x4c40, 
		0xeb33, 
		0x52e0, 
		0x4c40, 
		0x5340, 
		0xd260, 
		0x5080, 
		0x661d, 
		0xc400, 
		0x4500, 
		0x5080, 
		0x7234, 
		0xcd80, 
		0x1425, 
		0xd360, 
		0xcc40, 
		0xe630, 
		0xcf40, 
		0x0c48, 
		0x8000, 
		0x60bf, 
		0x904c, 
		0x53c0, 
		0x88da, 
		0xd040, 
		0xcc80, 
		0x0000, 
		0x4e80, 
		0x5020, 
		0x4227, 
		0xcd40, 
		0x5200, 
		0xd240, 
		0x4dc0, 
		0xcdc0, 
		0x183a, 
		0xabc8, 
		0x5280, 
		0x5889, 
		0xcf80, 
		0x4b00, 
		0x3fbf, 
		0x4e00, 
		0x4c00, 
		0xd120, 
		0x5220, 
		0xd300, 
		0x0537, 
		0xcc00, 
		0x4cc0, 
		0x53c0, 
		0x5220, 
		0x5100, 
		0x7c00, 
		0xbda6, 
		0x53a0, 
		0x81e2, 
		0x14dc, 
		0xd0a0, 
		0xabef, 
		0xd240, 
		0x8918, 
		0xc600, 
		0xc400, 
		0x4200, 
		0xd380, 
		0x53e0, 
		0xca80, 
		0xcfc0  
};
volatile uint32_t rand_ieee754_32[1536] __attribute__ ((aligned (2048))) = {
		0xf8dfa564, 
		0x80100000, 
		0x42500000, 
		0xf6975f5e, 
		0x41d80000, 
		0x41880000, 
		0x0e0007ff, 
		0x42000000, 
		0xc0c00000, 
		0x41300000, 
		0x3f800001, 
		0x80010000, 
		0x00000000, 
		0x80800003, 
		0x00002000, 
		0x7f000000, 
		0xc1980000, 
		0xc2200000, 
		0x00020000, 
		0x806797b9, 
		0xa80f0115, 
		0x7f800001, 
		0xff800000, 
		0xc24c0000, 
		0xc21c0000, 
		0x0f791616, 
		0x0e000003, 
		0xcd36cadd, 
		0x0d00fff0, 
		0x0e0001ff, 
		0x80800003, 
		0xff7ffffe, 
		0xc1e80000, 
		0xc0c00000, 
		0x2e7e45fe, 
		0x409b3696, 
		0x00800002, 
		0x80318428, 
		0x42280000, 
		0x80000080, 
		0x048e9f46, 
		0xfa47d930, 
		0xc0800000, 
		0x634441cf, 
		0x80000000, 
		0x00000000, 
		0x003ba491, 
		0x00000000, 
		0xab6c69d9, 
		0x00800002, 
		0x4ab1e268, 
		0xc1b80000, 
		0x42480000, 
		0xc2680000, 
		0x00000000, 
		0x00200000, 
		0x00100000, 
		0x615a078c, 
		0xc26c0000, 
		0xc26c0000, 
		0x37d124f0, 
		0xff800000, 
		0x42080000, 
		0x00000400, 
		0xffffffff, 
		0xb833dffd, 
		0x80000080, 
		0x0e0fffff, 
		0x00000040, 
		0xc1b00000, 
		0xaa8f1213, 
		0x80000000, 
		0x41000000, 
		0x423c0000, 
		0xfdf30beb, 
		0xc2200000, 
		0x381fa22b, 
		0x0e0007ff, 
		0x0c7fc000, 
		0x80000400, 
		0x80000000, 
		0x7f800000, 
		0xc2440000, 
		0xeea8f278, 
		0x80000000, 
		0xc1c80000, 
		0xf1fb0958, 
		0x697db4a6, 
		0x434d464c, 
		0xc1c00000, 
		0x00000100, 
		0x80000001, 
		0x42400000, 
		0x80000010, 
		0xd55e874a, 
		0x0e07ffff, 
		0xc2200000, 
		0xc1b80000, 
		0x00000000, 
		0x00800001, 
		0x80040000, 
		0x80000000, 
		0x2d8bd0ab, 
		0x7f975b46, 
		0x41d80000, 
		0x00000000, 
		0x0c7fffc0, 
		0xc1e00000, 
		0x0e00000f, 
		0x00200000, 
		0x8027ab1a, 
		0x00800000, 
		0x6125f803, 
		0x41a80000, 
		0x0e00003f, 
		0xc1f80000, 
		0xff8b58c6, 
		0xff800000, 
		0xc8ed115d, 
		0x268b5993, 
		0x0115474e, 
		0x42580000, 
		0x4b8c4b40, 
		0x80000400, 
		0x54daa448, 
		0xc2180000, 
		0x80000001, 
		0x40800000, 
		0x0c7fffff, 
		0x42080000, 
		0x0e01ffff, 
		0x0f7ffffc, 
		0xca6b544a, 
		0x7f9820fc, 
		0x005b7930, 
		0x7f7ffffe, 
		0x0c7f8000, 
		0x80800000, 
		0xffc00000, 
		0x0ee34b90, 
		0x0c7e0000, 
		0x80000000, 
		0xc1400000, 
		0x80000800, 
		0x67312f4e, 
		0x42240000, 
		0x76472434, 
		0x8555e371, 
		0xc1980000, 
		0x40c00000, 
		0xde954402, 
		0xc24c0000, 
		0xe2f81103, 
		0x41d00000, 
		0xc1600000, 
		0xaf5ffd1b, 
		0x0f7ffffe, 
		0x40800000, 
		0x0e01ffff, 
		0xc2640000, 
		0x42680000, 
		0x186e1fe5, 
		0x426c0000, 
		0xc2480000, 
		0xffbfffff, 
		0x937ab712, 
		0x80800000, 
		0x0f7ffffe, 
		0xa20a7032, 
		0xc1900000, 
		0x80000000, 
		0x6c8384f0, 
		0x0c7fffc0, 
		0x80000000, 
		0xb0d9464c, 
		0x00800003, 
		0xc2200000, 
		0x7fbfffff, 
		0x80000800, 
		0x3c35cdd2, 
		0xc2140000, 
		0x923669ce, 
		0x7f800000, 
		0xaefa0f87, 
		0x4b8c4b40, 
		0x41f80000, 
		0x0e001fff, 
		0x00000001, 
		0x41b00000, 
		0xffffffff, 
		0x00000000, 
		0x00000008, 
		0x0e0fffff, 
		0x80001000, 
		0x80800000, 
		0x00800001, 
		0x422c0000, 
		0xc0875d59, 
		0x41100000, 
		0xc1900000, 
		0xbf028f5c, 
		0x7f7ffffe, 
		0x6bc23101, 
		0xe74dc83d, 
		0xc97e7dfd, 
		0x41a00000, 
		0x7f7fffff, 
		0x60969c75, 
		0xbd23e39d, 
		0xc1e80000, 
		0xa8ac2962, 
		0xff9e0721, 
		0x40400000, 
		0x80040000, 
		0x55958fb3, 
		0x41a80000, 
		0xa8659055, 
		0xc0c00000, 
		0x007ffffe, 
		0xca89e022, 
		0x76428bab, 
		0x4754d81d, 
		0x55555555, 
		0xc2240000, 
		0x0e00000f, 
		0x07c0587f, 
		0x00000000, 
		0xffc00000, 
		0x41e80000, 
		0x00004000, 
		0xc2340000, 
		0x64881d83, 
		0x41a80000, 
		0xffffffff, 
		0x42040000, 
		0x33333333, 
		0x8f7fffff, 
		0xc2780000, 
		0xf04ec427, 
		0xc0000000, 
		0x00800001, 
		0xf4e6fd86, 
		0x3f800001, 
		0xe2df71e7, 
		0xc0e00000, 
		0x3c6f623b, 
		0x0c7c0000, 
		0x15bbc559, 
		0xc1d80000, 
		0xf60a6d42, 
		0x0e00001f, 
		0x80000002, 
		0xff7ffffe, 
		0xc0800000, 
		0x78353e07, 
		0x7f7fffff, 
		0xe5fd5c0f, 
		0x7fc00001, 
		0xc1900000, 
		0x00000100, 
		0x42280000, 
		0x00000200, 
		0xc0c00000, 
		0x00800002, 
		0x67548a12, 
		0x00000000, 
		0x7f000000, 
		0x98ed0da7, 
		0xff800000, 
		0x2e031f10, 
		0xbd589c1c, 
		0xd3cc322a, 
		0x80000000, 
		0x42240000, 
		0x00800001, 
		0x7f800001, 
		0x807fffff, 
		0xc2040000, 
		0x1acac5f4, 
		0x7f800000, 
		0x80000000, 
		0x7f7ffffe, 
		0x00000000, 
		0x00002000, 
		0x0e01ffff, 
		0x42140000, 
		0x0c7fffc0, 
		0xbf028f5c, 
		0xdf48ee2a, 
		0x0d00fff0, 
		0x80001000, 
		0xff7ffffe, 
		0xaf516c44, 
		0xbf9a5a18, 
		0x00000400, 
		0x00010000, 
		0x41600000, 
		0xc24c0000, 
		0x00000008, 
		0x7f800000, 
		0xff800000, 
		0x981a8734, 
		0x00400000, 
		0x80000000, 
		0xff800001, 
		0xff800000, 
		0xc2200000, 
		0x00000000, 
		0x426c0000, 
		0x0c7ffff0, 
		0x42600000, 
		0xb542a1e1, 
		0x80000040, 
		0x00800003, 
		0x80000008, 
		0x6dedeefa, 
		0x41000000, 
		0xcb8c4b40, 
		0x0af90d23, 
		0x00000000, 
		0xe2d5b729, 
		0xf43be75e, 
		0x0e00ffff, 
		0xe2472ac1, 
		0x80000001, 
		0x67784815, 
		0x7fc00001, 
		0x80000000, 
		0x2836038a, 
		0x80000200, 
		0x7f800000, 
		0xc1b00000, 
		0x9481581c, 
		0xc2280000, 
		0xc1900000, 
		0x3f238bb4, 
		0xc2080000, 
		0x4b000000, 
		0x423c0000, 
		0x41000000, 
		0xff7ffffe, 
		0xff800000, 
		0x9b6b51a2, 
		0x42680000, 
		0x80000000, 
		0x045a1a8f, 
		0x80000000, 
		0x42416016, 
		0x00000008, 
		0x4b000000, 
		0xa90e1414, 
		0x31976128, 
		0x0c780000, 
		0x00020000, 
		0x00400000, 
		0x03a5d8ea, 
		0xff864b15, 
		0x8f7fffff, 
		0x00000000, 
		0x80100000, 
		0x0f7ffffd, 
		0x7fd840df, 
		0xc1980000, 
		0x00004000, 
		0x80000000, 
		0x41200000, 
		0x4b000000, 
		0x42000000, 
		0x00400000, 
		0x41a80000, 
		0x425c0000, 
		0x0e01ffff, 
		0x7f90bd47, 
		0xba7fb94f, 
		0xccc93e01, 
		0x7c0974a9, 
		0x0e03ffff, 
		0xbde6f617, 
		0xc2780000, 
		0x80000800, 
		0x807ffffe, 
		0xc2280000, 
		0x8f7ffffc, 
		0x00800003, 
		0x93fd2e16, 
		0x00000000, 
		0x48407faa, 
		0x0c7f8000, 
		0xb8e954dd, 
		0x80000000, 
		0x41300000, 
		0x283952a6, 
		0x0c7fffc0, 
		0xb0d354ea, 
		0x9c855baa, 
		0x00000080, 
		0xa1b02b20, 
		0xc2440000, 
		0xeef900e9, 
		0x0e1fffff, 
		0x80000400, 
		0x6006ee87, 
		0x00000040, 
		0x0e00003f, 
		0xff7fffff, 
		0x42300000, 
		0xff7ffffe, 
		0xb573c41f, 
		0x00800000, 
		0x80004000, 
		0xc27c0000, 
		0x80800001, 
		0x8373e0ce, 
		0x0c780000, 
		0xc1b00000, 
		0xc24c0000, 
		0xe9d7d180, 
		0x00800000, 
		0xc2000000, 
		0xffb314da, 
		0xff800000, 
		0x42500000, 
		0x010a6da5, 
		0x80000000, 
		0xc2100000, 
		0xc2380000, 
		0x1e4219df, 
		0xff800001, 
		0x0c7ff800, 
		0x80000000, 
		0x081b57db, 
		0x0f7ffffd, 
		0x80603d65, 
		0x717291c8, 
		0x0e0001ff, 
		0x0e0001ff, 
		0x41200000, 
		0x80000000, 
		0xc1a00000, 
		0x435a5526, 
		0x0e01ffff, 
		0xc0a00000, 
		0x294123d5, 
		0x80800003, 
		0xd4086228, 
		0x4724051c, 
		0xcccccccc, 
		0x4b000000, 
		0x0e0000ff, 
		0x1ac95323, 
		0x41900000, 
		0xc1d00000, 
		0x7fa8b50b, 
		0xc1100000, 
		0xa6d56c69, 
		0x0c7fe000, 
		0xc1e80000, 
		0x2449a3fa, 
		0x0c7ffff8, 
		0x7fc00000, 
		0xc2080000, 
		0x9f2e0a1c, 
		0xff800000, 
		0x41300000, 
		0x0e000fff, 
		0xc0000000, 
		0x2ccd6631, 
		0x18eb6308, 
		0x0c7ffc00, 
		0x00000008, 
		0xff800000, 
		0x00000200, 
		0x00001000, 
		0x0c7ffff0, 
		0x207e6740, 
		0xc1b00000, 
		0x7f7fffff, 
		0xd467b4ba, 
		0x00800001, 
		0xc2680000, 
		0xc0c00000, 
		0x7f7fffff, 
		0x1a0b860f, 
		0x7f800000, 
		0x42780000, 
		0x80100000, 
		0x7f7ffffe, 
		0x1220e71b, 
		0x0e001fff, 
		0x80000000, 
		0xc1900000, 
		0x3dbdfce9, 
		0xb157283d, 
		0x41e00000, 
		0x80000000, 
		0x007fffff, 
		0xc1200000, 
		0x7fc00000, 
		0x00800000, 
		0x42600000, 
		0xd81f1d1e, 
		0x7f7ffffe, 
		0x615a9ed3, 
		0x6aaf2cc5, 
		0x8f7fffff, 
		0xc25c0000, 
		0x7f800000, 
		0xc2180000, 
		0xc1d80000, 
		0xc9f4153a, 
		0x7f800000, 
		0x7fc00001, 
		0x7f7ffffe, 
		0x0e00000f, 
		0xc2700000, 
		0x00000000, 
		0x94fc94b6, 
		0x0c7fffff, 
		0xc2480000, 
		0x80001000, 
		0xaff413aa, 
		0x803fadca, 
		0x0e000007, 
		0xff800000, 
		0xe7ca440b, 
		0x70efad2e, 
		0xc1900000, 
		0x1662bcb8, 
		0x40000000, 
		0x0d5efb45, 
		0x80002000, 
		0x00400000, 
		0x7373aea5, 
		0xc1f80000, 
		0x8f7fffff, 
		0x0e0003ff, 
		0x0e000007, 
		0x80000000, 
		0x00000000, 
		0x41f80000, 
		0x0000278e, 
		0x00800000, 
		0x001d72b8, 
		0x7fb5667b, 
		0x7f7ffffe, 
		0x0bbed915, 
		0x00000000, 
		0xc2700000, 
		0xf6d7a70e, 
		0x006681fc, 
		0x80000000, 
		0x0c780000, 
		0x0e00000f, 
		0x98bd1288, 
		0xbf5d5c77, 
		0x80800001, 
		0x11448234, 
		0xcdfeaae3, 
		0xc0000000, 
		0x7fffffff, 
		0x41700000, 
		0x00020000, 
		0xff800000, 
		0x42580000, 
		0xcdb0b4ce, 
		0xbf926076, 
		0x4685bc8e, 
		0x80000010, 
		0x0e03ffff, 
		0x5b8a836e, 
		0x41e00000, 
		0xff7ffffe, 
		0x80000002, 
		0x0022144a, 
		0x1f2f0440, 
		0x3153bb2b, 
		0x00000000, 
		0x41200000, 
		0x7f800000, 
		0x7f7ffffe, 
		0xc1100000, 
		0x00000000, 
		0x00000040, 
		0x427c0000, 
		0x8582c5cb, 
		0x00000000, 
		0x8095d572, 
		0x7fafd8ae, 
		0x0c7ffffc, 
		0x80800001, 
		0x7fbadf92, 
		0x87c761df, 
		0x2b7d0435, 
		0x00800001, 
		0x00800003, 
		0x00000000, 
		0x80800003, 
		0x42180000, 
		0x7f7ffffe, 
		0xfc0eaeb4, 
		0x8008db44, 
		0x7f7fffff, 
		0x0f7fffff, 
		0xcb000000, 
		0xc0c00000, 
		0x422c0000, 
		0xe505d7d4, 
		0xc1f80000, 
		0x00400000, 
		0x80800001, 
		0x30745cd9, 
		0x15d64ba8, 
		0x061806ad, 
		0x0e00001f, 
		0x40c00000, 
		0x80000000, 
		0x00400000, 
		0x00800002, 
		0x40800000, 
		0x7db6439e, 
		0x00000000, 
		0x00800001, 
		0x41200000, 
		0x00800003, 
		0x41600000, 
		0xc1300000, 
		0x00000000, 
		0x0e112b41, 
		0x00400000, 
		0x2d2c7d04, 
		0xc26c0000, 
		0x80000000, 
		0x00000200, 
		0x80800000, 
		0xc2280000, 
		0xf38561dd, 
		0xff800000, 
		0x80000020, 
		0x0e003fff, 
		0x5b065dda, 
		0x33333333, 
		0x00000000, 
		0xe9b1352b, 
		0x94e1bad9, 
		0x80000080, 
		0x423c0000, 
		0x56d6f238, 
		0x0e0fffff, 
		0x42040000, 
		0xff800000, 
		0x41700000, 
		0x00000004, 
		0xff800000, 
		0x80fbd691, 
		0x0c7ff800, 
		0x7f7fffff, 
		0xdf575b83, 
		0xd2012c29, 
		0xc2440000, 
		0x41f00000, 
		0x7fc00000, 
		0x42000000, 
		0x80800002, 
		0x7869777d, 
		0x7e404fee, 
		0x80000001, 
		0x2326bfbc, 
		0xff800000, 
		0x00000002, 
		0xc0a00000, 
		0x80000001, 
		0x7fbe4c24, 
		0xc1c80000, 
		0xc2280000, 
		0x00000000, 
		0x42300000, 
		0x41a00000, 
		0x40e24108, 
		0xe76dbccb, 
		0xffaf5c50, 
		0x2db6544a, 
		0x40e00000, 
		0x00000000, 
		0x0e00000f, 
		0x2ace51f1, 
		0x0c7ffff8, 
		0x80000001, 
		0xc2600000, 
		0x80000000, 
		0x80000000, 
		0x80000002, 
		0xc2400000, 
		0x0e000fff, 
		0xc1200000, 
		0x423c0000, 
		0x03dde9fe, 
		0x80002000, 
		0x41800000, 
		0x41b80000, 
		0x97658431, 
		0x40e00000, 
		0xffc00000, 
		0xbf800000, 
		0x42640000, 
		0x80000000, 
		0xeecb80e3, 
		0xff800000, 
		0x00000010, 
		0x0e00001f, 
		0xbdf5aa9a, 
		0xc9d53428, 
		0x7f800000, 
		0xc1d00000, 
		0x80000000, 
		0x41c80000, 
		0x41c80000, 
		0x7f9a9ed6, 
		0x80000000, 
		0xbd514d57, 
		0xc21c0000, 
		0xc2680000, 
		0x0f03c34a, 
		0x4b8c4b40, 
		0x80000000, 
		0x04fae7b8, 
		0x42000000, 
		0x961ec7a4, 
		0xc1880000, 
		0x00200000, 
		0xff7fffff, 
		0x80040000, 
		0xc2080000, 
		0x0e0fffff, 
		0xe4a9ae10, 
		0x04e702d1, 
		0xfe758793, 
		0x76315f87, 
		0x1e8145e1, 
		0xa8a06845, 
		0x00000002, 
		0xc1700000, 
		0x7f7fffff, 
		0xc1600000, 
		0x00000002, 
		0x0e3fffff, 
		0x41b00000, 
		0x0f7ffffd, 
		0x00000002, 
		0x41300000, 
		0x4875482b, 
		0x7fc00000, 
		0x7f800000, 
		0xc24c0000, 
		0xffc00000, 
		0xe57a52e4, 
		0x00000000, 
		0x3f028f5c, 
		0xcccccccc, 
		0x0c7ffc00, 
		0x26975ea4, 
		0x02b50275, 
		0xc1b80000, 
		0xb958dc0c, 
		0x42000000, 
		0x0e3fffff, 
		0x0e0007ff, 
		0xfd2d0844, 
		0xfef5ece8, 
		0x00011111, 
		0xff800000, 
		0x2d0d77d2, 
		0x0e0001ff, 
		0xc2280000, 
		0x42700000, 
		0x41600000, 
		0xc2180000, 
		0x0e000003, 
		0x42540000, 
		0xff800000, 
		0x7f800000, 
		0xc2380000, 
		0x0e00ffff, 
		0x00000080, 
		0xff7ffffe, 
		0xc2480000, 
		0xc64439ed, 
		0x42340000, 
		0x0e03ffff, 
		0x279b11a0, 
		0x774aba6e, 
		0xff7fffff, 
		0x80200000, 
		0xa28abb68, 
		0xc2200000, 
		0x41400000, 
		0x00040000, 
		0x41e80000, 
		0x41100000, 
		0xffc00001, 
		0x41d00000, 
		0xff800000, 
		0x80010000, 
		0xc1600000, 
		0x0c7f0000, 
		0xffbd2069, 
		0xff800000, 
		0x426c0000, 
		0xb6302000, 
		0xc26c0000, 
		0x42100000, 
		0x80000000, 
		0x1bae8c25, 
		0x0f7ffffc, 
		0xff7fffff, 
		0x7fbfffff, 
		0x0ba4a437, 
		0xba8d5cd2, 
		0xc0000000, 
		0x00000000, 
		0x0c7fffc0, 
		0x00000000, 
		0x006c2ea1, 
		0xc1400000, 
		0xc21c0000, 
		0x42280000, 
		0x41880000, 
		0x41400000, 
		0x0e00007f, 
		0x0c7fffff, 
		0x80040000, 
		0x87bd05bf, 
		0x40400000, 
		0x00000001, 
		0xc1200000, 
		0x00000000, 
		0xff851f5a, 
		0x427c0000, 
		0xa214918b, 
		0xc1a80000, 
		0x3a785fdf, 
		0x42600000, 
		0x8f7ffffe, 
		0x42400000, 
		0x41a80000, 
		0x4734bc03, 
		0x80000020, 
		0xff7ffffe, 
		0xc2000000, 
		0xc1800000, 
		0x202e13e1, 
		0xc2080000, 
		0x42380000, 
		0x00000000, 
		0x0c7f8000, 
		0xc0800000, 
		0x41559074, 
		0x42480000, 
		0xe7fdefa9, 
		0x42040000, 
		0x0e7fffff, 
		0x7f800000, 
		0x80000800, 
		0xc24c0000, 
		0x500db39c, 
		0x80000000, 
		0x42400000, 
		0x80000200, 
		0x80000000, 
		0x007fffff, 
		0x7fa21e5f, 
		0xc2300000, 
		0x00800002, 
		0x0c7fffff, 
		0x00000000, 
		0x00001000, 
		0x96e19159, 
		0x807bb27a, 
		0x80000100, 
		0x16d47d90, 
		0x00000100, 
		0x7fe9448b, 
		0xcccccccc, 
		0x00001000, 
		0x80000020, 
		0x00800001, 
		0x7f7ffffe, 
		0x41b00000, 
		0x487059c4, 
		0x80000000, 
		0x4345809f, 
		0x42480000, 
		0xbf800000, 
		0xc1200000, 
		0x3f800000, 
		0x0c7ff800, 
		0x81c49044, 
		0x3a32899e, 
		0x0e001fff, 
		0xc1a00000, 
		0x00000000, 
		0xc1f00000, 
		0xe0497fc6, 
		0xc20c0000, 
		0xbe4728c5, 
		0x00000000, 
		0x00000400, 
		0xafca11d1, 
		0x0e000fff, 
		0xe0fb740d, 
		0x80000200, 
		0x7f800000, 
		0x425c0000, 
		0xd65abc18, 
		0x41800000, 
		0xffaf0301, 
		0x7f800000, 
		0x3fd08005, 
		0x9f404076, 
		0xc1500000, 
		0x0d00fff0, 
		0x0e0fffff, 
		0x80800000, 
		0xc2680000, 
		0x80800001, 
		0xffbfffff, 
		0x0e1fffff, 
		0xff7ffffe, 
		0x7f7ffffe, 
		0x9953a7d4, 
		0x00000020, 
		0xf2ca4d08, 
		0xcd4ca2f4, 
		0x8db985ff, 
		0xc23c0000, 
		0x454c69e2, 
		0xc1300000, 
		0x2ee6c63f, 
		0x002a0405, 
		0xc83d78ae, 
		0x00000000, 
		0x6df75237, 
		0x420c0000, 
		0xbc518d16, 
		0x00000040, 
		0x80040000, 
		0x7fa3b2d6, 
		0xc2540000, 
		0xffc88c94, 
		0x00000001, 
		0x259ea977, 
		0x0d82256d, 
		0xfb7de988, 
		0xc1200000, 
		0x86f60d78, 
		0x0e0fffff, 
		0x421c0000, 
		0x7f800000, 
		0x0c7fc000, 
		0x80000002, 
		0xc25c0000, 
		0x68e8908d, 
		0x00001000, 
		0x80000002, 
		0x3d756e88, 
		0xff800000, 
		0xff800000, 
		0x80000000, 
		0xc1f80000, 
		0x91c4a981, 
		0x00000000, 
		0x80000000, 
		0xcccccccc, 
		0x0f7fffff, 
		0xc1200000, 
		0x0c7fffff, 
		0x41c00000, 
		0xc25c0000, 
		0xc2100000, 
		0xc2340000, 
		0x0e3fffff, 
		0x0c7fffc0, 
		0x0e7fffff, 
		0xd4c87fc3, 
		0xcc2f1436, 
		0x420c0000, 
		0x00200000, 
		0x4f06faec, 
		0xc2340000, 
		0xc1b80000, 
		0x0f7fffff, 
		0xc0400000, 
		0x00000080, 
		0x04521092, 
		0x3f800000, 
		0x423c0000, 
		0xc2100000, 
		0x80004000, 
		0x1000c71b, 
		0x7211d6a9, 
		0x00800001, 
		0x00000000, 
		0x0e0000ff, 
		0x7f800000, 
		0x00000000, 
		0x80011111, 
		0x00800003, 
		0x00000000, 
		0xfab9f371, 
		0x0f7ffffe, 
		0x00000002, 
		0xcf2335fc, 
		0x14923d15, 
		0x00000001, 
		0x007fffff, 
		0x91886d5c, 
		0xcadf73fc, 
		0xc1980000, 
		0x00000000, 
		0x00800001, 
		0x6663f574, 
		0x0c7ffff0, 
		0x0c7ffff0, 
		0x80800002, 
		0xffd150e8, 
		0xff800000, 
		0x1e6cba31, 
		0xbb515399, 
		0x80000004, 
		0x00000000, 
		0xff800000, 
		0x00000000, 
		0x42480000, 
		0x855ccd41, 
		0x3144a334, 
		0xf490fa4b, 
		0x9fa162ae, 
		0x42480000, 
		0x8038ef99, 
		0x42400000, 
		0x16d6a14d, 
		0x00000001, 
		0x80800003, 
		0xdf44c615, 
		0xca5c2a93, 
		0x80000000, 
		0x00000400, 
		0x8f7ffffc, 
		0x42080000, 
		0x0c7ffe00, 
		0x424c0000, 
		0x1baca546, 
		0x80000000, 
		0xc2680000, 
		0xff7fffff, 
		0x33afebe0, 
		0x7f7fffff, 
		0xb8fa5185, 
		0x427c0000, 
		0x41a80000, 
		0xc2240000, 
		0x8013ea18, 
		0x6253b05b, 
		0x5b483325, 
		0x4b8c4b40, 
		0x00ffffff, 
		0x0e00003f, 
		0x0c7ff800, 
		0x42140000, 
		0x8fab44cb, 
		0x41c80000, 
		0xd9d8e85b, 
		0x00100000, 
		0x081acafc, 
		0x03e0619c, 
		0x41b00000, 
		0x42040000, 
		0x41100000, 
		0xc2180000, 
		0x10913d5a, 
		0x55555555, 
		0x14bde87e, 
		0xc2680000, 
		0x56ab8f98, 
		0x7f7ffffe, 
		0xa2399b6d, 
		0x00010000, 
		0x7fffffff, 
		0x0c780000, 
		0x99eb489e, 
		0xc2700000, 
		0xc1f00000, 
		0xff7ffffe, 
		0xff7ffffe, 
		0x4cb63432, 
		0x00000040, 
		0xc1a00000, 
		0x46a00154, 
		0x0008da72, 
		0x0e0001ff, 
		0xcb8c4b40, 
		0x40000000, 
		0xff800000, 
		0x0e1fffff, 
		0xc1100000, 
		0x80000000, 
		0x5e6a5007, 
		0x42240000, 
		0xca7c073b, 
		0x80001000, 
		0xd4e0bdd3, 
		0x06e53258, 
		0x42200000, 
		0x7fc00000, 
		0x7f800000, 
		0x41400000, 
		0xc2540000, 
		0x7fc00001, 
		0x00800000, 
		0x0c7c0000, 
		0x80040000, 
		0xe73187e1, 
		0xc1100000, 
		0x42100000, 
		0x2b8af3ca, 
		0xc1c80000, 
		0xfbf548b3, 
		0x00000000, 
		0x0e0003ff, 
		0x60431fac, 
		0x3bf56283, 
		0xc1c80000, 
		0xf5a86204, 
		0x120a5fd9, 
		0x3709e87b, 
		0x7f7ffffe, 
		0x6dcfd210, 
		0xd5312d7d, 
		0x5b0f63ab, 
		0x80200000, 
		0x00002000, 
		0x80800000, 
		0x427c0000, 
		0xb8f5db64, 
		0x00800000, 
		0x41d80000, 
		0xc16ec0f8, 
		0x9e9566a8, 
		0x00040000, 
		0xb944196d, 
		0x7f800000, 
		0x8ec6b4fd, 
		0x80000004, 
		0xde0c5a3a, 
		0x7fd4920d, 
		0xc26c0000, 
		0x80080000, 
		0x8064e7d3, 
		0xc2780000, 
		0xc2500000, 
		0x41000000, 
		0x637286f4, 
		0xe3ef3450, 
		0x40a00000, 
		0xa7a618d6, 
		0x80040000, 
		0x00000400, 
		0x0e0fffff, 
		0x7f800000, 
		0x75b3a714, 
		0xbcc3634a, 
		0x0e1fffff, 
		0x6435ca8a, 
		0xbf028f5c, 
		0x00000000, 
		0x8f7fffff, 
		0x0e1fffff, 
		0x0c600000, 
		0x1b8be759, 
		0x1f9acbb7, 
		0x4964d7ec, 
		0xffc00000, 
		0x80000008, 
		0x0e1fffff, 
		0x41a00000, 
		0x3c104ab8, 
		0x80800001, 
		0xc1c80000, 
		0x807f4c43, 
		0x00000001, 
		0xff800000, 
		0x7f800000, 
		0xe95ede0c, 
		0x42600000, 
		0x0c7ff800, 
		0x80000002, 
		0x42540000, 
		0xff7ffffe, 
		0x0e000fff, 
		0x0e0000ff, 
		0x80000000, 
		0x0c400000, 
		0x80020000, 
		0x80000000, 
		0xffc00001, 
		0x80800003, 
		0x0c7f8000, 
		0x80000000, 
		0xc2600000, 
		0xf4c3997f, 
		0xcb000000, 
		0xff7fffff, 
		0x7f800000, 
		0x29a9b59e, 
		0x0ba061cc, 
		0x42340000, 
		0x0c7fffe0, 
		0x80ffffff, 
		0x4b8c4b40, 
		0x41200000, 
		0x80004000, 
		0x80800003, 
		0x80000002, 
		0x80000000, 
		0xffc00000, 
		0x41100000, 
		0x0e0fffff, 
		0x0f7fffff, 
		0x80000000, 
		0x80000000, 
		0x00001000, 
		0x00000000, 
		0x80010000, 
		0xc1a00000, 
		0x80000000, 
		0x68fa9b6d, 
		0x8dd83c82, 
		0xf07e97b8, 
		0x41000000, 
		0x80800001, 
		0x55555555, 
		0xff7ffffe, 
		0x0e000fff, 
		0x00000001, 
		0x00000000, 
		0x7fbff5a8, 
		0x00000000, 
		0x421c0000, 
		0xbb7a1e7e, 
		0xc1f80000, 
		0x0f7ffffe, 
		0x00800000, 
		0xc24c0000, 
		0xac9107ab, 
		0xc2140000, 
		0xc23c0000, 
		0x80008000, 
		0x00008000, 
		0x0e0000ff, 
		0x807ffffe, 
		0x00000001, 
		0xe2421cd1, 
		0x44e4a70d, 
		0x7fa8409d, 
		0xda531354, 
		0xc1e00000, 
		0x6c49ce9c, 
		0x0c7ffff0, 
		0x7f7fffff, 
		0x002a6257, 
		0xff800000, 
		0xc0800000, 
		0xe3a302e0, 
		0x42500000, 
		0xff7fffff, 
		0x00000020, 
		0x00800003, 
		0x41c80000, 
		0x0e00ffff, 
		0x00000000, 
		0x96c58e83, 
		0x078be0e5, 
		0x00000000, 
		0xa74ec15b, 
		0xff800000, 
		0x0c3a5e67, 
		0xe6e046b4, 
		0x83d26bb1, 
		0xf4a9b820, 
		0x00000000, 
		0x80010000, 
		0x42680000, 
		0x8f7fffff, 
		0x00020000, 
		0x7fc00000, 
		0x80800001, 
		0x42080000, 
		0x28f86d1b, 
		0xffb08634, 
		0x03c8100b, 
		0x0e7fffff, 
		0x424c0000, 
		0x41c80000, 
		0x4b000000, 
		0xc1980000, 
		0x7f800000, 
		0x00000000, 
		0x80000000, 
		0x00000000, 
		0x7f800000, 
		0xf11a685c, 
		0xc2540000, 
		0x0c7fc000, 
		0x00200000, 
		0x00800003, 
		0x80002000, 
		0x7f800000, 
		0xe9302d43, 
		0x00800002, 
		0x91a6a198, 
		0x00800000, 
		0x425c0000, 
		0x0f7ffffc, 
		0x0e0007ff, 
		0x7f000000, 
		0x42600000, 
		0x80000020, 
		0x00000010, 
		0xc2000000, 
		0x090d2b73, 
		0x0c7ffff0, 
		0x40a00000, 
		0xffbdc4ee, 
		0x73e14431, 
		0x40800000, 
		0x7f800000, 
		0xc2180000, 
		0x8ec31598, 
		0x41f80000, 
		0xff800000, 
		0x0c7fc000, 
		0xc2040000, 
		0x63e1e297, 
		0x0c7ffff0, 
		0x0c600000, 
		0x80000040, 
		0x41100000, 
		0x00000000, 
		0xc00e36be, 
		0x42540000, 
		0xd097d427, 
		0x0e3fffff, 
		0x7fc00000, 
		0x0c7fffe0, 
		0xffc00000, 
		0x007fffff, 
		0x42140000, 
		0x41c00000, 
		0x42400000, 
		0xc2440000, 
		0x072237f6, 
		0x7b4a9a94, 
		0x9eaf8102, 
		0xc22c0000, 
		0x0c7fff00, 
		0xaaaaaaaa, 
		0x807fffff, 
		0x00000000, 
		0x80800000, 
		0x80000010, 
		0x0068f005, 
		0x80000000, 
		0x1e08168f, 
		0x7f800001, 
		0x306f49fd, 
		0x5f209573, 
		0xaaaaaaaa, 
		0xf467ae13, 
		0xff800000, 
		0x80000040, 
		0x00001000, 
		0x0c7ffffe, 
		0x80000000, 
		0x7d678fcc, 
		0x80000000, 
		0x00000100, 
		0x40800000, 
		0x41500000, 
		0xc20c0000, 
		0x00800003, 
		0x0e07ffff, 
		0x80000002, 
		0xffffffff, 
		0x41400000, 
		0x8ac2f77e, 
		0x25bf3941, 
		0x41a00000, 
		0x63b68f84, 
		0x596ebe1a, 
		0x00800001, 
		0xc26c0000, 
		0x374494a5, 
		0x7f7fffff, 
		0x7fc00001, 
		0x00000100, 
		0xc1e80000, 
		0xb286d9d8, 
		0x37cc2e2b, 
		0x7f800000, 
		0xc1880000, 
		0x80000010, 
		0xc2600000, 
		0xb49cf078, 
		0x807ffffe, 
		0x0c7fffe0, 
		0x42100000, 
		0xc0800000, 
		0xbf800000, 
		0x0ca0aa25, 
		0x422c0000, 
		0x00800001, 
		0x41f80000, 
		0x80000001, 
		0x7f800000, 
		0x80000000, 
		0x0d000ff0, 
		0xfba43c59, 
		0xff800000, 
		0xc2500000, 
		0xa994024b, 
		0x80800003, 
		0x7fc00000, 
		0xe1b5c1e6, 
		0x42480000, 
		0xaa17eeb7, 
		0x80ffffff, 
		0xff7ffffe, 
		0x807fffff, 
		0xc2600000, 
		0x2cb2ec06, 
		0xc2300000, 
		0xff800000, 
		0x40000000, 
		0x42340000, 
		0xffa22a1a, 
		0x80000100, 
		0xa765b141, 
		0x0c7ff800, 
		0xc1600000, 
		0x55555555, 
		0xffba5702, 
		0x00000000, 
		0x80000000, 
		0x80000100, 
		0x00000000, 
		0x80002000, 
		0xc1700000, 
		0x0e007fff, 
		0x7f800000, 
		0x629a7bcd, 
		0x0e1fffff, 
		0x80000000, 
		0x80000080, 
		0x80004000, 
		0x203d9f6b, 
		0x80000800, 
		0x0f67df69, 
		0x7ea1d395, 
		0x80000040, 
		0xcda8f5d1, 
		0x40c00000, 
		0x40a00000, 
		0x8ed978eb, 
		0x00100000, 
		0xee705f27, 
		0xd9a1f1f6, 
		0x80ffffff, 
		0x6b4eb2cf, 
		0xc51894ad, 
		0x46262406, 
		0x00000000, 
		0x9f67e1bf, 
		0x40400000, 
		0xe486456a, 
		0x42140000, 
		0x0fa996c9, 
		0x80800000, 
		0x45559564, 
		0x00000001, 
		0x7f7fffff, 
		0xf0024cac, 
		0x089cbc23, 
		0x80011111, 
		0x6edd7922, 
		0x80000000, 
		0xc2400000, 
		0x5cc5401b, 
		0x00040000, 
		0x80000001, 
		0xc0400000, 
		0x7f7ffffe, 
		0xc2740000, 
		0x0e000001, 
		0xd1b0b1f4, 
		0x42280000, 
		0x80000002, 
		0x80040000, 
		0x26d58716, 
		0x80000000, 
		0x0e7fffff, 
		0xaf03b62a, 
		0xc26c0000, 
		0xc1c80000, 
		0x80040000, 
		0x80000000, 
		0xc1100000, 
		0xfe5387ce, 
		0x7f800000, 
		0x42440000, 
		0x42500000, 
		0x7fa58b13, 
		0x42140000, 
		0xc1ef2b3f, 
		0xc8f48519, 
		0xa42f15cc, 
		0x7d812dc4, 
		0x0e00003f, 
		0xc0400000, 
		0xc1b80000, 
		0x80000008, 
		0x80000002, 
		0x266b4d7c, 
		0x41200000, 
		0x0d000ff0, 
		0x7f800000, 
		0x41800000, 
		0x0c7ff000  
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
		0x000000ed, 
		0x0000016f, 
		0x00000385, 
		0x000003ab, 
		0x000003a5, 
		0x00000057, 
		0x0000014b, 
		0x000002d7, 
		0x0000012f, 
		0x000003e3, 
		0x000000db, 
		0x000001af, 
		0x00000301, 
		0x00000025, 
		0x00000003, 
		0x000000d3, 
		0x000001fb, 
		0x0000010d, 
		0x000001bb, 
		0x00000091, 
		0x00000007, 
		0x000001ff, 
		0x00000247, 
		0x00000039, 
		0x0000031d, 
		0x00000383, 
		0x00000007, 
		0x00000117, 
		0x0000002d, 
		0x000002cb, 
		0x000003bd, 
		0x00000137, 
		0x000003fb, 
		0x00000021, 
		0x00000381, 
		0x000003b9, 
		0x00000069, 
		0x000003fd, 
		0x00000111, 
		0x000003fb, 
		0x00000043, 
		0x0000003f, 
		0x00000107, 
		0x000000c1, 
		0x000001e7, 
		0x0000037f, 
		0x0000017d, 
		0x000003c9, 
		0x00000393, 
		0x00000235, 
		0x00000369, 
		0x00000071, 
		0x0000008b, 
		0x000001e1, 
		0x0000032d, 
		0x00000383, 
		0x000000ab, 
		0x00000309, 
		0x0000011b, 
		0x000000a9, 
		0x000003bb, 
		0x00000291, 
		0x00000175, 
		0x000001e5, 
		0x0000017b, 
		0x00000045, 
		0x000001f1, 
		0x00000331, 
		0x00000067, 
		0x00000079, 
		0x00000009, 
		0x00000315, 
		0x000000f9, 
		0x0000005b, 
		0x00000005, 
		0x000002d5, 
		0x00000053, 
		0x00000313, 
		0x000003d9, 
		0x00000043, 
		0x00000251, 
		0x0000036f, 
		0x000003bb, 
		0x00000065, 
		0x000003f9, 
		0x0000014d, 
		0x0000032f, 
		0x000001e3, 
		0x00000345, 
		0x00000011, 
		0x000002dd, 
		0x0000024d, 
		0x0000016d, 
		0x000001ad, 
		0x0000018b, 
		0x00000143, 
		0x00000097, 
		0x0000014f, 
		0x000003e1, 
		0x00000379, 
		0x000001a7, 
		0x00000315, 
		0x000000f7, 
		0x00000103, 
		0x000001fb, 
		0x00000005, 
		0x000001e9, 
		0x000000ed, 
		0x00000141, 
		0x00000153, 
		0x0000038f, 
		0x0000006f, 
		0x00000165, 
		0x0000019f, 
		0x0000023d, 
		0x000003fd, 
		0x000003c7, 
		0x00000289, 
		0x00000205, 
		0x0000020d, 
		0x000003a1, 
		0x000002a7, 
		0x00000367, 
		0x00000159, 
		0x0000028d, 
		0x00000047, 
		0x00000343, 
		0x00000027, 
		0x00000183, 
		0x0000014b, 
		0x00000081, 
		0x0000031f, 
		0x00000213, 
		0x0000018b, 
		0x00000007, 
		0x00000379, 
		0x00000289, 
		0x00000283, 
		0x00000321, 
		0x000002bb, 
		0x00000031, 
		0x0000033b, 
		0x0000038b, 
		0x000000a7, 
		0x00000319, 
		0x00000133, 
		0x00000303, 
		0x000003af, 
		0x00000373, 
		0x0000039d, 
		0x000003bb, 
		0x00000307, 
		0x000000d7, 
		0x000001cb, 
		0x00000225, 
		0x00000151, 
		0x000000cd, 
		0x00000213, 
		0x000002ed, 
		0x000000f7, 
		0x0000001b, 
		0x000003ad, 
		0x000003ab, 
		0x00000167, 
		0x0000003b, 
		0x0000000f, 
		0x000001e7, 
		0x000003d7, 
		0x0000007d, 
		0x000000cd, 
		0x000002d9, 
		0x0000005b, 
		0x000000ef, 
		0x00000301, 
		0x00000139, 
		0x0000011b, 
		0x0000025f, 
		0x000002f9, 
		0x00000217, 
		0x0000004b, 
		0x000001ad, 
		0x000001d5, 
		0x00000111, 
		0x00000245, 
		0x000001d3, 
		0x0000026f, 
		0x0000026f, 
		0x00000051, 
		0x000003b5, 
		0x000000f7, 
		0x0000019b, 
		0x000002a5, 
		0x000000ad, 
		0x00000217, 
		0x000000c3, 
		0x000001b7, 
		0x00000181, 
		0x00000069, 
		0x000003ff, 
		0x00000273, 
		0x0000025d, 
		0x00000351, 
		0x0000032b, 
		0x000000c5, 
		0x000002cb, 
		0x00000127, 
		0x00000151, 
		0x000000f5, 
		0x00000265, 
		0x000001cb, 
		0x0000017f, 
		0x00000205, 
		0x00000219, 
		0x000001b3, 
		0x000003ff, 
		0x000001e5, 
		0x00000079, 
		0x000000ad, 
		0x0000012f, 
		0x0000033d, 
		0x00000181, 
		0x0000033f, 
		0x0000011d, 
		0x00000081, 
		0x00000397, 
		0x000000fd, 
		0x000003ab, 
		0x00000173, 
		0x00000239, 
		0x00000249, 
		0x000002c7, 
		0x0000036d, 
		0x000003cf, 
		0x00000103, 
		0x0000033f, 
		0x00000145, 
		0x00000363, 
		0x000003a9, 
		0x0000028f, 
		0x00000055, 
		0x000001f7, 
		0x00000095, 
		0x000001fd, 
		0x00000013, 
		0x0000007b, 
		0x00000387, 
		0x00000149, 
		0x000003fd, 
		0x0000032f, 
		0x0000033f, 
		0x000002c5, 
		0x0000024d, 
		0x000002dd, 
		0x00000139, 
		0x000002af, 
		0x000001cb  
};

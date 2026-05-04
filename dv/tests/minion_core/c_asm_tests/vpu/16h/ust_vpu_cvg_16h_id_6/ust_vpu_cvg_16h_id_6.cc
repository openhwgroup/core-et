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
			"li x5, 278\n"
			"faddi.pi f2, f0, 335\n"
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
			"csrwi mhpmevent3, 4\n"   
			"csrwi mhpmevent4, 8\n"   
			"csrwi mhpmevent5, 18\n"   
			"csrwi mhpmevent6, 21\n"   
			"csrwi mhpmevent7, 4\n"   
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
			"flw.ps f22,  576 (x5)\n"
			"flw.ps f29,  992 (x5)\n"
			"flw.ps f0,  320 (x5)\n"
			"flw.ps f8,  864 (x5)\n"
			"flw.ps f25,  896 (x5)\n"
			"flw.ps f12,  608 (x5)\n"
			"flw.ps f13,  736 (x5)\n"
			"flw.ps f9,  96 (x5)\n"
			"flw.ps f1,  352 (x5)\n"
			"flw.ps f3,  480 (x5)\n"
			"flw.ps f15,  704 (x5)\n"
			"flw.ps f16,  160 (x5)\n"
			"flw.ps f31,  544 (x5)\n"
			"flw.ps f30,  640 (x5)\n"
			"flw.ps f23,  288 (x5)\n"
			"flw.ps f17,  800 (x5)\n"
			"flw.ps f28,  416 (x5)\n"
			"flw.ps f19,  672 (x5)\n"
			"flw.ps f4,  64 (x5)\n"
			"flw.ps f20,  128 (x5)\n"
			"flw.ps f2,  32 (x5)\n"
			"flw.ps f6,  0 (x5)\n"
			"flw.ps f10,  512 (x5)\n"
			"flw.ps f21,  832 (x5)\n"
			"flw.ps f27,  256 (x5)\n"
			"flw.ps f11,  960 (x5)\n"
			"flw.ps f24,  192 (x5)\n"
			"flw.ps f26,  384 (x5)\n"
			"flw.ps f18,  768 (x5)\n"
			"flw.ps f14,  928 (x5)\n"
			"flw.ps f5,  224 (x5)\n"
			"flw.ps f7,  448 (x5)\n"
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
			"li x0, 0x2bc\n" 
			"slti x0, x0, 0x7f2\n"
			"la x31, check_sum_hid_0\n"
			"li x5, 0x2\n" 
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
			"la x5, rand_ieee754_32\n"
			"flw.ps f23, 992(x5)\n"
			"flw.ps f13, 256(x5)\n"
			"flw.ps f18, 192(x5)\n"
			"fmadd.s f4, f23, f13, f18, rup\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f1, 256(x19)\n"
			"flw.ps f10, 128(x19)\n"
			"fsgnjx.s f7, f1, f10\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_0:\n"
			"mov.m.x m6, x12, 0x37\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_0:\n"
			"feqm.ps m6, f19, f18\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x19, rand_ieee754_32\n"
			"flw.ps f25, 256(x19)\n"
			"flw.ps f17, 672(x19)\n"
			"flw.ps f15, 896(x19)\n"
			"fcmov.ps f1, f25, f17, f15\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f3, 512(x12)\n"
			"flw.ps f10, 416(x12)\n"
			"fmadd.s f25, f3, f3, f10, rup\n" 
			"la x11, rand_int_32\n"
			"flw.ps f26, 224(x11)\n"
			"flw.ps f27, 320(x11)\n"
			"fsll.pi f23, f26, f27\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f13, 832(x19)\n"
			"flw.ps f26, 160(x19)\n"
			"fmax.ps f25, f13, f26\n" 
			"LBL_SEQID_0_M0_WRITE_ID_2_HID_0:\n"
			"fltm.ps m7, f15, f27\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"li x5, 0x8a9d8c64f1ceb8d8\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0x40a45584fe98a802\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"la x14, rand_int_32\n"
			"flw.ps f2, 928(x14)\n"
			"flw.ps f1, 416(x14)\n"
			"fand.pi f13, f2, f1\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f29, 832(x6)\n"
			"fcvt.pw.ps f19, f29, rdn\n" 
			"li x5, 0xaaf4\n"
			"csrw tensor_mask, x5\n"	
			"la x14, rand_ieee754_32\n"
			"flw.ps f27, 352(x14)\n"
			"flw.ps f23, 768(x14)\n"
			"flt.s x12, f27, f23\n" 
			"la x6, rand_int_32\n"
			"flw.ps f1, 160(x6)\n"
			"fandi.pi f26, f1, 0x16e\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f25, 928(x11)\n"
			"flw.ps f2, 96(x11)\n"
			"fadd.s f27, f25, f2, rdn\n" 
			"la x15, rand_int_32\n"
			"flw.ps f17, 960(x15)\n"
			"flw.ps f13, 512(x15)\n"
			"fand.pi f7, f17, f13\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f19, 320(x12)\n"
			"flw.ps f27, 672(x12)\n"
			"flw.ps f29, 384(x12)\n"
			"fmadd.s f4, f19, f27, f29, rtz\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ID_224838_HID_0:\n"
			"la x31, self_check_32\n"
			"li x5, 0xe0000000000003\n"
			"add x5, x5, x31\n"
			"li x6, 0x3c0000000000002\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x3 << TENSOR_FMA32_B_NUM_COLS) | (0x3 << TENSOR_FMA32_A_NUM_ROWS) | (0x2 << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1e << TENSOR_FMA32_SCP_LOC_B) | (0x7 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
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
			"bne x5, x31, LBL_FAIL_HID_0\n"
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ID_96367_HID_0:\n"
			"la x31, self_check_16\n"
			"li x5, 0xa0000000000003\n"
			"add x5, x5, x31\n"
			"li x6, 0x340000000000000\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x3 << TENSOR_FMA16_B_NUM_COLS) | (0x3 << TENSOR_FMA16_A_NUM_ROWS) | (0x0 << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1a << TENSOR_FMA16_SCP_LOC_B) | (0x5 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
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
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_0\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_0_RDX_ID_0_SHIRE_0_HID_0:\n"
			"li x27, 0x3e00000000040018\n"
			"csrr x7, mhartid\n"
			"srli x7, x7, 4\n"
			"slli x7, x7, 6\n"
			"add x27, x27, x7\n"
			"csrw tensor_reduce, x27\n" 
			"csrwi tensor_wait, 9\n" 
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ID_555686_HID_0:\n"
			"la x31, self_check_32\n"
			"li x5, 0xe0000000000009\n"
			"add x5, x5, x31\n"
			"li x6, 0x340000000000005\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x3 << TENSOR_FMA32_B_NUM_COLS) | (0x9 << TENSOR_FMA32_A_NUM_ROWS) | (0x5 << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1a << TENSOR_FMA32_SCP_LOC_B) | (0x7 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x1a << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0x9 << TENSOR_QUANT_QUANT_ROW) | (0x31 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x2 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ID_475685_HID_0:\n"
			"la x31, self_check_8\n"
			"li x5, 0x8000000000000d\n"
			"add x5, x5, x31\n"
			"li x6, 0x10000000000008\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x1 << TENSOR_IMA_B_NUM_COLS) | (0xd << TENSOR_IMA_A_NUM_ROWS) | (0x8 << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x1 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1c << TENSOR_IMA_SCP_LOC_B) | (0x4 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fmvz.x.ps x31, f0, 0\n"
			"li x5, 0x24\n"
			"bne x5, x31, LBL_FAIL_HID_0\n"
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
			"la x31, self_check_32\n"
			"li x5, 0xe0000000000005\n"
			"add x5, x5, x31\n"
			"li x6, 0x380000000000006\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x0 << TENSOR_FMA32_B_NUM_COLS) | (0x5 << TENSOR_FMA32_A_NUM_ROWS) | (0x6 << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1c << TENSOR_FMA32_SCP_LOC_B) | (0x7 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x1a << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0x5 << TENSOR_QUANT_QUANT_ROW) | (0x36 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x7 << TENSOR_QUANT_TRANSF6) | (0x7 << TENSOR_QUANT_TRANSF5) | (0x6 << TENSOR_QUANT_TRANSF4) | (0x6 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0x7\n"
			"bne x5, x31, LBL_FAIL_HID_0\n"
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 10\n"
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ITER_1_HID_0:\n"
			"la x31, self_check_8\n"
			"li x5, 0xa0000000000004\n"
			"add x5, x5, x31\n"
			"li x6, 0x340000000000000\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x0 << TENSOR_IMA_B_NUM_COLS) | (0x4 << TENSOR_IMA_A_NUM_ROWS) | (0x0 << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1a << TENSOR_IMA_SCP_LOC_B) | (0x5 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x9, ((0xf << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0x4 << TENSOR_QUANT_QUANT_ROW) | (0x14 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x0 << TENSOR_QUANT_TRANSF1) | (0x3 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fmvz.x.ps x31, f0, 0\n"
			"li x5, 0x4\n"
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
			"la x23, rand_ieee754_32\n"
			"flw.ps f2, 256(x23)\n"
			"ffrc.ps f7, f2\n" 
			"la x23, rand_int_32\n"
			"flw.ps f13, 192(x23)\n"
			"fsrli.pi f10, f13, 0xf\n" 
			"la x11, rand_int_32\n"
			"flw.ps f23, 96(x11)\n"
			"flw.ps f26, 480(x11)\n"
			"fadd.pi f17, f23, f26\n" 
			"la x15, rand_int_32\n"
			"flw.ps f15, 0(x15)\n"
			"flw.ps f1, 832(x15)\n"
			"fxor.pi f2, f15, f1\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f4, 128(x26)\n"
			"fsgnjn.s f10, f4, f4\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x23, rand_ieee754_32\n"
			"flw.ps f2, 640(x23)\n"
			"flw.ps f18, 288(x23)\n"
			"flw.ps f19, 768(x23)\n"
			"fmadd.ps f1, f2, f18, f19, rtz\n" 
			"LBL_SEQID_1_FP_TRANS_ID_3_HID_0:\n"
			"la x19, rand_ieee754_32\n"
			"flw.ps f7, 992(x19)\n"
			"flog.ps f4, f7\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x11, rand_ieee754_32\n"
			"flw.ps f29, 928(x11)\n"
			"flw.ps f23, 640(x11)\n"
			"fsgnjn.ps f10, f29, f23\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f23, 480(x11)\n"
			"fcvt.pw.ps f23, f23, rmm\n" 
			"fbci.pi f23, 0x50cad\n" 
			"la x11, rand_int_32\n"
			"flw.ps f15, 416(x11)\n"
			"fnot.pi f7, f15\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f3, 576(x11)\n"
			"flw.ps f7, 512(x11)\n"
			"fmin.s f19, f3, f7\n" 
			"la x19, rand_int_32\n"
			"flw.ps f1, 960(x19)\n"
			"fpackrepb.pi f25, f1\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f19, 192(x5)\n"
			"flw.ps f2, 832(x5)\n"
			"flw.ps f13, 128(x5)\n"
			"fnmadd.s f2, f19, f2, f13, rne\n" 
			"la x6, rand_int_32\n"
			"flw.ps f10, 512(x6)\n"
			"flw.ps f29, 0(x6)\n"
			"fle.pi f2, f10, f29\n" 
			"la x5, rand_int_32\n"
			"flw.ps f13, 416(x5)\n"
			"flw.ps f4, 992(x5)\n"
			"fand.pi f13, f13, f4\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_1_RDX_ID_1_SHIRE_0_HID_0:\n"
			"li x15, 0x120000000435000b\n"
			"csrw tensor_reduce, x15\n" 
			"csrwi tensor_wait, 9\n" 
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=0); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"LBL_SEQID_1_TLD_XMA_STR_SEQ_ITER_0_HID_0:\n"
			"la x31, self_check_8\n"
			"li x5, 0xc0000000000003\n"
			"add x5, x5, x31\n"
			"li x6, 0x3a000000000000f\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x0 << TENSOR_IMA_B_NUM_COLS) | (0x3 << TENSOR_IMA_A_NUM_ROWS) | (0xf << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1d << TENSOR_IMA_SCP_LOC_B) | (0x6 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x9, ((0x10 << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0x3 << TENSOR_QUANT_QUANT_ROW) | (0x5 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x1 << TENSOR_QUANT_TRANSF1) | (0x9 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fmvz.x.ps x31, f0, 0\n"
			"li x5, 0x40\n"
			"bne x5, x31, LBL_FAIL_HID_0\n"
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_0\n"
			"csrw tensor_error, x0\n"
			"li x5, 2\n" 
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
			"LBL_HPM_3_CORE_BRANCHES0_FAIL_HID_0:\n"          
			"csrr x5, mhpmcounter3\n"
			"sd x5, 32 (x31)\n"
			"LBL_HPM_4_CORE_DCACHE_MISSES0_FAIL_HID_0:\n"          
			"csrr x5, mhpmcounter4\n"
			"sd x5, 40 (x31)\n"
			"LBL_HPM_5_CORE_TFMA_WAIT_TENB_FAIL_HID_0:\n"          
			"csrr x5, mhpmcounter5\n"
			"sd x5, 48 (x31)\n"
			"LBL_HPM_6_CORE_TXFMA_OPS32_FAIL_HID_0:\n"          
			"csrr x5, mhpmcounter6\n"
			"sd x5, 56 (x31)\n"
			"LBL_HPM_7_NEIGH_TX_NEIGH_COOP_TLD_REQ_FAIL_HID_0:\n"          
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
			"LBL_HPM_3_CORE_BRANCHES0_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter3\n"
			"sd x5, 32 (x31)\n"
			"LBL_HPM_4_CORE_DCACHE_MISSES0_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter4\n"
			"sd x5, 40 (x31)\n"
			"LBL_HPM_5_CORE_TFMA_WAIT_TENB_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter5\n"
			"sd x5, 48 (x31)\n"
			"LBL_HPM_6_CORE_TXFMA_OPS32_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter6\n"
			"sd x5, 56 (x31)\n"
			"LBL_HPM_7_NEIGH_TX_NEIGH_COOP_TLD_REQ_PASS_HID_0:\n"          
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
			"csrwi mhpmevent3, 6\n"   
			"csrwi mhpmevent4, 1\n"   
			"csrwi mhpmevent5, 3\n"   
			"csrwi mhpmevent6, 2\n"   
			"csrwi mhpmevent7, 23\n"   
			"csrwi mhpmevent8, 22\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f9,  960 (x5)\n"
			"flw.ps f6,  736 (x5)\n"
			"flw.ps f0,  288 (x5)\n"
			"flw.ps f30,  768 (x5)\n"
			"flw.ps f7,  480 (x5)\n"
			"flw.ps f18,  384 (x5)\n"
			"flw.ps f27,  544 (x5)\n"
			"flw.ps f25,  64 (x5)\n"
			"flw.ps f13,  320 (x5)\n"
			"flw.ps f21,  608 (x5)\n"
			"flw.ps f17,  0 (x5)\n"
			"flw.ps f28,  96 (x5)\n"
			"flw.ps f10,  704 (x5)\n"
			"flw.ps f2,  832 (x5)\n"
			"flw.ps f3,  32 (x5)\n"
			"flw.ps f23,  128 (x5)\n"
			"flw.ps f29,  928 (x5)\n"
			"flw.ps f11,  640 (x5)\n"
			"flw.ps f16,  448 (x5)\n"
			"flw.ps f5,  192 (x5)\n"
			"flw.ps f22,  800 (x5)\n"
			"flw.ps f14,  416 (x5)\n"
			"flw.ps f26,  992 (x5)\n"
			"flw.ps f15,  672 (x5)\n"
			"flw.ps f1,  576 (x5)\n"
			"flw.ps f12,  352 (x5)\n"
			"flw.ps f19,  512 (x5)\n"
			"flw.ps f4,  256 (x5)\n"
			"flw.ps f31,  160 (x5)\n"
			"flw.ps f20,  864 (x5)\n"
			"flw.ps f8,  896 (x5)\n"
			"flw.ps f24,  224 (x5)\n"
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
			"li x5, 0x2\n" 
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
			"la x7, rand_ieee754_32\n"
			"flw.ps f27, 896(x7)\n"
			"flw.ps f6, 224(x7)\n"
			"fcmovm.ps f27, f27, f6\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f14, 640(x26)\n"
			"fclass.s x9, f14\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_1:\n"
			"fltm.pi m3, f27, f19\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x10, rand_ieee754_32\n"
			"flw.ps f29, 160(x10)\n"
			"fcvt.f16.ps f25, f29\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f31, 544(x11)\n"
			"flw.ps f4, 416(x11)\n"
			"fsub.ps f13, f31, f4, rtz\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_1:\n"
			"maskand m3, m6, m3\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x7, rand_ieee754_32\n"
			"flw.ps f19, 800(x7)\n"
			"flw.ps f13, 608(x7)\n"
			"flw.ps f27, 128(x7)\n"
			"fnmsub.s f6, f19, f13, f27, rtz\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f6, 544(x7)\n"
			"flw.ps f17, 800(x7)\n"
			"flw.ps f27, 512(x7)\n"
			"fmadd.s f13, f6, f17, f27, rne\n" 
			"LBL_SEQID_0_M0_WRITE_ID_2_HID_1:\n"
			"mov.m.x m2, x11, 0x11\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x7, rand_ieee754_32\n"
			"flw.ps f17, 32(x7)\n"
			"fcvt.wu.s x9, f17, rne\n" 
			"li x26, 0xccb73b71685aa932\n"
			"fcvt.s.w f4, x26, rmm\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f31, 928(x6)\n"
			"flw.ps f13, 960(x6)\n"
			"fle.ps f1, f31, f13\n" 
			"LBL_SEQID_0_FP_TRANS_ID_3_HID_1:\n"
			"la x26, rand_ieee754_32\n"
			"flw.ps f22, 96(x26)\n"
			"frcp.ps f19, f22\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f25, 192(x10)\n"
			"flw.ps f29, 480(x10)\n"
			"flw.ps f27, 608(x10)\n"
			"fcmov.ps f14, f25, f29, f27\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f31, 160(x9)\n"
			"flw.ps f30, 256(x9)\n"
			"fadd.s f22, f31, f30, rtz\n" 
			"LBL_SEQID_0_FP_TRANS_ID_4_HID_1:\n"
			"la x9, rand_ieee754_32\n"
			"flw.ps f4, 160(x9)\n"
			"flog.ps f4, f4\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_1\n"
			"addi x30, x30, 1\n"
			"la x15, rand_int_32\n"
			"flw.ps f17, 928(x15)\n"
			"flw.ps f7, 992(x15)\n"
			"fsll.pi f13, f17, f7\n" 
			"la x14, rand_int_32\n"
			"flw.ps f17, 32(x14)\n"
			"fpackrepb.pi f13, f17\n" 
			"la x25, rand_ieee754_16\n"
			"flw.ps f6, 736(x25)\n"
			"fcvt.ps.f16 f27, f6\n" 
			"la x15, rand_int_32\n"
			"flw.ps f6, 352(x15)\n"
			"flw.ps f7, 416(x15)\n"
			"fand.pi f9, f6, f7\n" 
			"la x14, rand_int_32\n"
			"flw.ps f14, 320(x14)\n"
			"fminu.pi f31, f14, f14\n" 
			"la x10, rand_ieee754_16\n"
			"flw.ps f6, 0(x10)\n"
			"fcvt.ps.f16 f27, f6\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f29, 32(x26)\n"
			"fclass.ps f31, f29\n" 
			"la x11, rand_int_32\n"
			"flw.ps f30, 832(x11)\n"
			"fsrl.pi f7, f30, f30\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f30, 672(x25)\n"
			"flw.ps f19, 608(x25)\n"
			"fsgnj.ps f9, f30, f19\n" 
			"la x23, rand_int_32\n"
			"flw.ps f7, 640(x23)\n"
			"fslli.pi f31, f7, 0xd\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f29, 352(x11)\n"
			"fsgnjn.ps f27, f29, f29\n" 
			"la x6, rand_int_32\n"
			"flw.ps f30, 608(x6)\n"
			"flw.ps f20, 128(x6)\n"
			"fsrl.pi f9, f30, f20\n" 
			"la x14, rand_int_32\n"
			"flw.ps f25, 960(x14)\n"
			"flw.ps f17, 416(x14)\n"
			"fsll.pi f4, f25, f17\n" 
			"la x26, rand_int_32\n"
			"flw.ps f22, 384(x26)\n"
			"flw.ps f30, 352(x26)\n"
			"flt.pi f13, f22, f30\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f6, 960(x9)\n"
			"feq.ps f22, f6, f6\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f19, 512(x11)\n"
			"flw.ps f29, 320(x11)\n"
			"fnmsub.ps f6, f19, f19, f29, rtz\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 2\n" 
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
			"LBL_HPM_3_CORE_DCACHE_ACCESS0_FAIL_HID_1:\n"          
			"csrr x5, mhpmcounter3\n"
			"sd x5, 32 (x31)\n"
			"LBL_HPM_4_CORE_MCYCLES_FAIL_HID_1:\n"          
			"csrr x5, mhpmcounter4\n"
			"sd x5, 40 (x31)\n"
			"LBL_HPM_5_CORE_RETIRED_INST1_FAIL_HID_1:\n"          
			"csrr x5, mhpmcounter5\n"
			"sd x5, 48 (x31)\n"
			"LBL_HPM_6_CORE_RETIRED_INST0_FAIL_HID_1:\n"          
			"csrr x5, mhpmcounter6\n"
			"sd x5, 56 (x31)\n"
			"LBL_HPM_7_NEIGH_ETL_RSP_FROM_SC_UC_FAIL_HID_1:\n"          
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
			"LBL_HPM_3_CORE_DCACHE_ACCESS0_PASS_HID_1:\n"          
			"csrr x5, mhpmcounter3\n"
			"sd x5, 32 (x31)\n"
			"LBL_HPM_4_CORE_MCYCLES_PASS_HID_1:\n"          
			"csrr x5, mhpmcounter4\n"
			"sd x5, 40 (x31)\n"
			"LBL_HPM_5_CORE_RETIRED_INST1_PASS_HID_1:\n"          
			"csrr x5, mhpmcounter5\n"
			"sd x5, 48 (x31)\n"
			"LBL_HPM_6_CORE_RETIRED_INST0_PASS_HID_1:\n"          
			"csrr x5, mhpmcounter6\n"
			"sd x5, 56 (x31)\n"
			"LBL_HPM_7_NEIGH_ETL_RSP_FROM_SC_UC_PASS_HID_1:\n"          
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
			"csrwi mhpmevent3, 4\n"   
			"csrwi mhpmevent4, 8\n"   
			"csrwi mhpmevent5, 18\n"   
			"csrwi mhpmevent6, 21\n"   
			"csrwi mhpmevent7, 4\n"   
			"csrwi mhpmevent8, 3\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f30,  448 (x5)\n"
			"flw.ps f16,  32 (x5)\n"
			"flw.ps f15,  0 (x5)\n"
			"flw.ps f28,  800 (x5)\n"
			"flw.ps f1,  928 (x5)\n"
			"flw.ps f3,  832 (x5)\n"
			"flw.ps f6,  96 (x5)\n"
			"flw.ps f31,  960 (x5)\n"
			"flw.ps f10,  544 (x5)\n"
			"flw.ps f13,  736 (x5)\n"
			"flw.ps f19,  672 (x5)\n"
			"flw.ps f18,  864 (x5)\n"
			"flw.ps f25,  576 (x5)\n"
			"flw.ps f0,  320 (x5)\n"
			"flw.ps f22,  768 (x5)\n"
			"flw.ps f7,  480 (x5)\n"
			"flw.ps f12,  608 (x5)\n"
			"flw.ps f26,  192 (x5)\n"
			"flw.ps f4,  704 (x5)\n"
			"flw.ps f14,  512 (x5)\n"
			"flw.ps f2,  352 (x5)\n"
			"flw.ps f27,  416 (x5)\n"
			"flw.ps f21,  896 (x5)\n"
			"flw.ps f24,  384 (x5)\n"
			"flw.ps f5,  256 (x5)\n"
			"flw.ps f20,  128 (x5)\n"
			"flw.ps f9,  160 (x5)\n"
			"flw.ps f11,  992 (x5)\n"
			"flw.ps f29,  640 (x5)\n"
			"flw.ps f23,  288 (x5)\n"
			"flw.ps f17,  224 (x5)\n"
			"flw.ps f8,  64 (x5)\n"
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
			"li x5, 0x2\n" 
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
			"la x15, rand_ieee754_32\n"
			"flw.ps f5, 416(x15)\n"
			"flw.ps f6, 928(x15)\n"
			"fsgnj.ps f9, f5, f6\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f16, 512(x25)\n"
			"flw.ps f29, 256(x25)\n"
			"fcmov.ps f25, f16, f29, f16\n" 
			"li x5, 0x88f8\n"
			"csrw tensor_mask, x5\n"	
			"la x17, rand_int_32\n"
			"flw.ps f25, 352(x17)\n"
			"flw.ps f5, 192(x17)\n"
			"fand.pi f27, f25, f5\n" 
			"li x28, 0x83d9ba78c541d159\n"
			"fcvt.s.wu f15, x28, rtz\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f25, 352(x25)\n"
			"flw.ps f5, 672(x25)\n"
			"flw.ps f12, 768(x25)\n"
			"fnmadd.ps f16, f25, f5, f12, rtz\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_2:\n"
			"masknot m3, m4\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x23, rand_ieee754_32\n"
			"flw.ps f29, 32(x23)\n"
			"flw.ps f5, 160(x23)\n"
			"flw.ps f7, 480(x23)\n"
			"fcmov.ps f4, f29, f5, f7\n" 
			"la x14, rand_int_32\n"
			"flw.ps f7, 192(x14)\n"
			"fsat8.pi f27, f7\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f12, 576(x28)\n"
			"fadd.ps f15, f12, f12, rtz\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_2:\n"
			"fltm.pi m3, f14, f6\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x15, rand_int_32\n"
			"flw.ps f6, 704(x15)\n"
			"faddi.pi f2, f6, 0x1a8\n" 
			"la x17, rand_int_32\n"
			"flw.ps f15, 800(x17)\n"
			"faddi.pi f6, f15, 0x1c8\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f14, 640(x5)\n"
			"flw.ps f27, 160(x5)\n"
			"flw.ps f16, 448(x5)\n"
			"fnmsub.ps f12, f14, f27, f16, rmm\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f29, 288(x7)\n"
			"flw.ps f14, 64(x7)\n"
			"fsgnj.s f9, f29, f14\n" 
			"la x28, rand_int_32\n"
			"flw.ps f12, 320(x28)\n"
			"flw.ps f15, 96(x28)\n"
			"fxor.pi f15, f12, f15\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f7, 576(x27)\n"
			"flw.ps f29, 736(x27)\n"
			"flw.ps f2, 256(x27)\n"
			"fnmsub.s f7, f7, f29, f2, rup\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ID_169204_HID_2:\n"
			"la x31, self_check_32\n"
			"li x5, 0x40000000000008\n"
			"add x5, x5, x31\n"
			"li x6, 0x320000000000005\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x1 << TENSOR_FMA32_B_NUM_COLS) | (0x8 << TENSOR_FMA32_A_NUM_ROWS) | (0x5 << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x19 << TENSOR_FMA32_SCP_LOC_B) | (0x2 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
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
			"bne x5, x31, LBL_FAIL_HID_2\n"
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ID_323527_HID_2:\n"
			"la x31, self_check_16\n"
			"li x5, 0xc0000000000002\n"
			"add x5, x5, x31\n"
			"li x6, 0x340000000000003\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x0 << TENSOR_FMA16_B_NUM_COLS) | (0x2 << TENSOR_FMA16_A_NUM_ROWS) | (0x3 << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1a << TENSOR_FMA16_SCP_LOC_B) | (0x6 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
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
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=0); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ITER_0_HID_2:\n"
			"la x31, self_check_16\n"
			"li x5, 0x8000000000000a\n"
			"add x5, x5, x31\n"
			"li x6, 0x3e0000000000003\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x2 << TENSOR_FMA16_B_NUM_COLS) | (0xa << TENSOR_FMA16_A_NUM_ROWS) | (0x3 << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1f << TENSOR_FMA16_SCP_LOC_B) | (0x4 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x19 << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0xa << TENSOR_QUANT_QUANT_ROW) | (0xb << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x2 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 10\n"
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ITER_1_HID_2:\n"
			"la x31, self_check_8\n"
			"li x5, 0x80000000000004\n"
			"add x5, x5, x31\n"
			"li x6, 0x3c000000000000c\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x1 << TENSOR_IMA_B_NUM_COLS) | (0x4 << TENSOR_IMA_A_NUM_ROWS) | (0xc << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1e << TENSOR_IMA_SCP_LOC_B) | (0x4 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fmvz.x.ps x31, f0, 0\n"
			"li x5, 0x34\n"
			"bne x5, x31, LBL_FAIL_HID_2\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_2\n"
			"csrw tensor_error, x0\n"
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_2\n"
			"addi x30, x30, 1\n"
			"la x28, rand_ieee754_32\n"
			"flw.ps f15, 288(x28)\n"
			"flw.ps f16, 32(x28)\n"
			"fsub.s f12, f15, f16, rtz\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f14, 96(x25)\n"
			"flw.ps f27, 768(x25)\n"
			"fsgnj.s f5, f14, f27\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f10, 800(x7)\n"
			"flw.ps f9, 896(x7)\n"
			"fcmovm.ps f15, f10, f9\n" 
			"LBL_SEQID_1_M0_WRITE_ID_2_HID_2:\n"
			"masknot m1, m1\n" 
			"li x5, CREDINC2\n"
			"ld x5, 0(x5)\n"
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x25, rand_int_32\n"
			"flw.ps f2, 512(x25)\n"
			"flw.ps f29, 416(x25)\n"
			"fsrl.pi f10, f2, f29\n" 
			"la x13, rand_int_32\n"
			"flw.ps f2, 704(x13)\n"
			"flw.ps f29, 288(x13)\n"
			"fmin.pi f24, f2, f29\n" 
			"la x27, rand_int_32\n"
			"flw.ps f27, 352(x27)\n"
			"fmul.pi f16, f27, f27\n" 
			"li x5, 0x1022e85faf21f81f\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0xa903b3658d30d502\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"maskpopcz x28, m6\n" 
			"LBL_SEQID_1_FP_TRANS_ID_3_HID_2:\n"
			"la x28, rand_ieee754_32\n"
			"flw.ps f16, 448(x28)\n"
			"frcp.ps f0, f16\n" 
			"mova.x.m x17\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f16, 544(x15)\n"
			"flw.ps f15, 224(x15)\n"
			"fsub.ps f27, f16, f15, rne\n" 
			"LBL_SEQID_1_M0_WRITE_ID_4_HID_2:\n"
			"flem.ps m4, f16, f5\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x7, rand_int_32\n"
			"flw.ps f16, 416(x7)\n"
			"fpackrepb.pi f25, f16\n" 
			"la x13, rand_int_32\n"
			"flw.ps f29, 256(x13)\n"
			"faddi.pi f4, f29, 0x7f\n" 
			"la x17, rand_int_32\n"
			"flw.ps f10, 224(x17)\n"
			"fpackreph.pi f25, f10\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f0, 160(x28)\n"
			"flw.ps f6, 32(x28)\n"
			"flw.ps f24, 960(x28)\n"
			"fmadd.s f24, f0, f6, f24, rdn\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_1_RDX_ID_1_SHIRE_0_HID_2:\n"
			"li x14, 0x120000000435000b\n"
			"csrw tensor_reduce, x14\n" 
			"csrwi tensor_wait, 9\n" 
			"LBL_SEQID_1_TLD_XMA_STR_SEQ_ID_400701_HID_2:\n"
			"la x31, self_check_8\n"
			"li x5, 0xf\n"
			"add x5, x5, x31\n"
			"li x6, 0x3c0000000000001\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x0 << TENSOR_IMA_B_NUM_COLS) | (0xf << TENSOR_IMA_A_NUM_ROWS) | (0x1 << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1e << TENSOR_IMA_SCP_LOC_B) | (0x0 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
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
			"bne x5, x31, LBL_FAIL_HID_2\n"
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
			"LBL_SEQID_1_TLD_XMA_STR_SEQ_ITER_0_HID_2:\n"
			"la x31, self_check_16\n"
			"li x5, 0x7\n"
			"add x5, x5, x31\n"
			"li x6, 0x3e000000000000c\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x1 << TENSOR_FMA16_B_NUM_COLS) | (0x7 << TENSOR_FMA16_A_NUM_ROWS) | (0xc << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1f << TENSOR_FMA16_SCP_LOC_B) | (0x0 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
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
			"bne x5, x31, LBL_FAIL_HID_2\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_2\n"
			"csrw tensor_error, x0\n"
			"li x5, 2\n" 
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
			"csrwi mhpmevent3, 6\n"   
			"csrwi mhpmevent4, 1\n"   
			"csrwi mhpmevent5, 3\n"   
			"csrwi mhpmevent6, 2\n"   
			"csrwi mhpmevent7, 23\n"   
			"csrwi mhpmevent8, 22\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f21,  352 (x5)\n"
			"flw.ps f29,  192 (x5)\n"
			"flw.ps f11,  640 (x5)\n"
			"flw.ps f13,  576 (x5)\n"
			"flw.ps f19,  896 (x5)\n"
			"flw.ps f17,  832 (x5)\n"
			"flw.ps f5,  960 (x5)\n"
			"flw.ps f6,  128 (x5)\n"
			"flw.ps f22,  672 (x5)\n"
			"flw.ps f4,  32 (x5)\n"
			"flw.ps f23,  928 (x5)\n"
			"flw.ps f14,  288 (x5)\n"
			"flw.ps f20,  480 (x5)\n"
			"flw.ps f18,  800 (x5)\n"
			"flw.ps f7,  64 (x5)\n"
			"flw.ps f30,  384 (x5)\n"
			"flw.ps f26,  864 (x5)\n"
			"flw.ps f24,  160 (x5)\n"
			"flw.ps f2,  608 (x5)\n"
			"flw.ps f15,  992 (x5)\n"
			"flw.ps f28,  736 (x5)\n"
			"flw.ps f12,  0 (x5)\n"
			"flw.ps f1,  512 (x5)\n"
			"flw.ps f31,  448 (x5)\n"
			"flw.ps f27,  704 (x5)\n"
			"flw.ps f0,  320 (x5)\n"
			"flw.ps f3,  768 (x5)\n"
			"flw.ps f8,  224 (x5)\n"
			"flw.ps f10,  544 (x5)\n"
			"flw.ps f16,  416 (x5)\n"
			"flw.ps f25,  96 (x5)\n"
			"flw.ps f9,  256 (x5)\n"
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
			"li x5, 0x2\n" 
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
			"LBL_SEQID_0_FP_TRANS_ID_0_HID_3:\n"
			"la x26, rand_ieee754_32\n"
			"flw.ps f0, 576(x26)\n"
			"frcp.ps f10, f0\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f3, 704(x26)\n"
			"flw.ps f19, 128(x26)\n"
			"fcmovm.ps f21, f3, f19\n" 
			"la x14, rand_int_32\n"
			"flw.ps f13, 544(x14)\n"
			"fandi.pi f13, f13, 0x1bd\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f6, 992(x26)\n"
			"flw.ps f20, 352(x26)\n"
			"flw.ps f21, 224(x26)\n"
			"fmsub.ps f13, f6, f20, f21, rtz\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f2, 64(x23)\n"
			"flw.ps f3, 448(x23)\n"
			"fsgnj.s f22, f2, f3\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f20, 512(x28)\n"
			"flw.ps f15, 32(x28)\n"
			"fadd.s f6, f20, f15, rdn\n" 
			"la x26, rand_int_32\n"
			"flw.ps f4, 864(x26)\n"
			"flw.ps f2, 960(x26)\n"
			"fadd.pi f6, f4, f2\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f0, 704(x14)\n"
			"flw.ps f7, 896(x14)\n"
			"fcmovm.ps f24, f0, f7\n" 
			"la x26, rand_int_32\n"
			"flw.ps f0, 288(x26)\n"
			"fnot.pi f18, f0\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f24, 480(x28)\n"
			"flw.ps f13, 32(x28)\n"
			"flw.ps f22, 480(x28)\n"
			"fnmadd.ps f22, f24, f13, f22, rup\n" 
			"la x26, rand_int_32\n"
			"flw.ps f13, 800(x26)\n"
			"flw.ps f12, 992(x26)\n"
			"fle.pi f24, f13, f12\n" 
			"la x27, rand_int_32\n"
			"flw.ps f19, 768(x27)\n"
			"flw.ps f3, 288(x27)\n"
			"fand.pi f21, f19, f3\n" 
			"la x14, rand_int_32\n"
			"flw.ps f13, 736(x14)\n"
			"flw.ps f24, 416(x14)\n"
			"fminu.pi f6, f13, f24\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_3:\n"
			"fltm.pi m7, f10, f6\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x5, rand_ieee754_32\n"
			"flw.ps f21, 832(x5)\n"
			"flw.ps f7, 288(x5)\n"
			"fmax.ps f15, f21, f7\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f18, 960(x19)\n"
			"flw.ps f21, 672(x19)\n"
			"flw.ps f7, 128(x19)\n"
			"fmsub.ps f4, f18, f21, f7, rne\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_3\n"
			"addi x30, x30, 1\n"
			"la x20, rand_ieee754_32\n"
			"flw.ps f21, 704(x20)\n"
			"frsq.ps f18, f21\n" 
			"la x5, rand_int_32\n"
			"flw.ps f13, 224(x5)\n"
			"flw.ps f21, 352(x5)\n"
			"fsub.pi f7, f13, f21\n" 
			"la x28, rand_int_32\n"
			"flw.ps f19, 864(x28)\n"
			"flw.ps f22, 512(x28)\n"
			"fmax.pi f4, f19, f22\n" 
			"LBL_SEQID_1_M0_WRITE_ID_2_HID_3:\n"
			"masknot m7, m2\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"mova.x.m x20\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f2, 480(x27)\n"
			"fcvt.f16.ps f4, f2\n" 
			"la x23, rand_int_32\n"
			"flw.ps f2, 672(x23)\n"
			"flw.ps f20, 736(x23)\n"
			"fdivu.pi f10, f2, f20\n" 
			"la x14, rand_int_32\n"
			"flw.ps f10, 160(x14)\n"
			"flw.ps f20, 832(x14)\n"
			"fmaxu.pi f3, f10, f20\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f13, 0(x26)\n"
			"fround.ps f24, f13, rup\n" 
			"li x23, 0x8b8f38d50f3bdda\n"
			"fcvt.s.w f15, x23, rtz\n" 
			"mova.x.m x23\n" 
			"la x10, rand_int_32\n"
			"flw.ps f20, 384(x10)\n"
			"flw.ps f2, 448(x10)\n"
			"for.pi f10, f20, f2\n" 
			"la x19, rand_int_32\n"
			"flw.ps f2, 544(x19)\n"
			"fcvt.ps.pw f2, f2, rtz\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f22, 352(x20)\n"
			"flw.ps f12, 736(x20)\n"
			"fmax.s f13, f22, f12\n" 
			"la x23, rand_int_32\n"
			"flw.ps f22, 384(x23)\n"
			"flw.ps f13, 608(x23)\n"
			"fsll.pi f15, f22, f13\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f22, 0(x7)\n"
			"fcvt.wu.s x27, f22, rup\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 2\n" 
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
			"csrwi mhpmevent3, 4\n"   
			"csrwi mhpmevent4, 8\n"   
			"csrwi mhpmevent5, 18\n"   
			"csrwi mhpmevent6, 21\n"   
			"csrwi mhpmevent7, 4\n"   
			"csrwi mhpmevent8, 3\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f0,  544 (x5)\n"
			"flw.ps f16,  320 (x5)\n"
			"flw.ps f17,  896 (x5)\n"
			"flw.ps f26,  960 (x5)\n"
			"flw.ps f9,  480 (x5)\n"
			"flw.ps f5,  992 (x5)\n"
			"flw.ps f1,  512 (x5)\n"
			"flw.ps f6,  768 (x5)\n"
			"flw.ps f4,  672 (x5)\n"
			"flw.ps f7,  96 (x5)\n"
			"flw.ps f18,  704 (x5)\n"
			"flw.ps f14,  864 (x5)\n"
			"flw.ps f20,  352 (x5)\n"
			"flw.ps f31,  64 (x5)\n"
			"flw.ps f27,  608 (x5)\n"
			"flw.ps f8,  224 (x5)\n"
			"flw.ps f2,  288 (x5)\n"
			"flw.ps f23,  832 (x5)\n"
			"flw.ps f21,  576 (x5)\n"
			"flw.ps f30,  800 (x5)\n"
			"flw.ps f11,  256 (x5)\n"
			"flw.ps f3,  736 (x5)\n"
			"flw.ps f28,  160 (x5)\n"
			"flw.ps f12,  928 (x5)\n"
			"flw.ps f10,  384 (x5)\n"
			"flw.ps f15,  32 (x5)\n"
			"flw.ps f13,  416 (x5)\n"
			"flw.ps f22,  640 (x5)\n"
			"flw.ps f19,  0 (x5)\n"
			"flw.ps f29,  192 (x5)\n"
			"flw.ps f24,  448 (x5)\n"
			"flw.ps f25,  128 (x5)\n"
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
			"li x5, 0x2\n" 
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
			"la x20, rand_ieee754_32\n"
			"flw.ps f12, 736(x20)\n"
			"flw.ps f13, 512(x20)\n"
			"flw.ps f17, 384(x20)\n"
			"fnmsub.ps f28, f12, f13, f17, rup\n" 
			"la x5, rand_int_32\n"
			"flw.ps f12, 768(x5)\n"
			"flw.ps f16, 352(x5)\n"
			"flt.pi f9, f12, f16\n" 
			"la x27, rand_int_32\n"
			"flw.ps f16, 576(x27)\n"
			"flw.ps f30, 64(x27)\n"
			"fmax.pi f24, f16, f30\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_4:\n"
			"masknot m6, m2\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"fbci.pi f29, 0x37f9d\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f27, 416(x14)\n"
			"fcvt.w.s x20, f27, rne\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"li x23, 0x2b535bf784d960bf\n"
			"fcvt.s.w f15, x23, rmm\n" 
			"la x14, rand_int_32\n"
			"flw.ps f17, 224(x14)\n"
			"flw.ps f22, 704(x14)\n"
			"fsll.pi f16, f17, f22\n" 
			"la x14, rand_int_32\n"
			"flw.ps f17, 480(x14)\n"
			"flw.ps f12, 352(x14)\n"
			"fadd.pi f8, f17, f12\n" 
			"li x11, 0x767f0732aea53b48\n"
			"fcvt.s.w f9, x11, rtz\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f27, 224(x27)\n"
			"flw.ps f29, 320(x27)\n"
			"fle.s x23, f27, f29\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f4, 128(x11)\n"
			"flw.ps f9, 288(x11)\n"
			"fadd.ps f21, f4, f9, rdn\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f9, 32(x27)\n"
			"flw.ps f4, 960(x27)\n"
			"feq.ps f15, f9, f4\n" 
			"mova.x.m x20\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f27, 32(x13)\n"
			"flw.ps f12, 384(x13)\n"
			"flw.ps f29, 960(x13)\n"
			"fmsub.ps f8, f27, f12, f29, rdn\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f15, 864(x17)\n"
			"flw.ps f9, 960(x17)\n"
			"fmul.ps f30, f15, f9, rne\n" 
			"li x5, 0x966f\n"
			"csrw tensor_mask, x5\n"	
			"csrw tensor_error, zero\n" 
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=0); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ITER_0_HID_4:\n"
			"la x31, self_check_32\n"
			"li x5, 0xe000000000000d\n"
			"add x5, x5, x31\n"
			"li x6, 0x10000000000000\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x2 << TENSOR_FMA32_B_NUM_COLS) | (0xd << TENSOR_FMA32_A_NUM_ROWS) | (0x0 << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x1 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1e << TENSOR_FMA32_SCP_LOC_B) | (0x7 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0x1\n"
			"bne x5, x31, LBL_FAIL_HID_4\n"
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ITER_1_HID_4:\n"
			"la x31, self_check_16\n"
			"li x5, 0x80000000000008\n"
			"add x5, x5, x31\n"
			"li x6, 0x36000000000000a\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x1 << TENSOR_FMA16_B_NUM_COLS) | (0x8 << TENSOR_FMA16_A_NUM_ROWS) | (0xa << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1b << TENSOR_FMA16_SCP_LOC_B) | (0x4 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
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
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_4\n"
			"csrw tensor_error, x0\n"
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_4\n"
			"addi x30, x30, 1\n"
			"la x13, rand_int_32\n"
			"flw.ps f27, 640(x13)\n"
			"flw.ps f20, 928(x13)\n"
			"fmin.pi f15, f27, f20\n" 
			"la x23, rand_int_32\n"
			"flw.ps f8, 416(x23)\n"
			"flw.ps f4, 704(x23)\n"
			"for.pi f15, f8, f4\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f24, 288(x17)\n"
			"flw.ps f12, 416(x17)\n"
			"flw.ps f21, 352(x17)\n"
			"fnmsub.ps f30, f24, f12, f21, rne\n" 
			"la x23, rand_int_32\n"
			"flw.ps f16, 864(x23)\n"
			"flw.ps f8, 320(x23)\n"
			"fmax.pi f8, f16, f8\n" 
			"la x13, rand_int_32\n"
			"flw.ps f29, 32(x13)\n"
			"fsatu8.pi f16, f29\n" 
			"la x23, rand_int_32\n"
			"flw.ps f4, 480(x23)\n"
			"fsrli.pi f20, f4, 0x2\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f16, 896(x14)\n"
			"fround.ps f16, f16, rmm\n" 
			"la x13, rand_int_32\n"
			"flw.ps f8, 128(x13)\n"
			"fcvt.ps.pw f24, f8, rne\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f27, 288(x27)\n"
			"fsqrt.ps f29, f27\n" 
			"la x17, rand_int_32\n"
			"flw.ps f30, 768(x17)\n"
			"fand.pi f4, f30, f30\n" 
			"la x7, rand_int_32\n"
			"flw.ps f12, 64(x7)\n"
			"flw.ps f16, 608(x7)\n"
			"fsrl.pi f13, f12, f16\n" 
			"LBL_SEQID_1_M0_WRITE_ID_1_HID_4:\n"
			"flem.ps m1, f13, f16\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x11, rand_ieee754_32\n"
			"flw.ps f29, 736(x11)\n"
			"fclass.s x27, f29\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f9, 512(x7)\n"
			"flw.ps f21, 64(x7)\n"
			"fmul.s f22, f9, f21, rup\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f12, 416(x11)\n"
			"fcvt.pw.ps f13, f12, rne\n" 
			"la x9, rand_int_32\n"
			"flw.ps f24, 576(x9)\n"
			"fsat8.pi f21, f24\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_1_RDX_ID_1_SHIRE_0_HID_4:\n"
			"li x20, 0x120000000435000b\n"
			"csrw tensor_reduce, x20\n" 
			"csrwi tensor_wait, 9\n" 
			"LBL_SEQID_1_TLD_XMA_STR_SEQ_ID_942151_HID_4:\n"
			"la x31, self_check_8\n"
			"li x5, 0x20000000000008\n"
			"add x5, x5, x31\n"
			"li x6, 0x1000000000000d\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x3 << TENSOR_IMA_B_NUM_COLS) | (0x8 << TENSOR_IMA_A_NUM_ROWS) | (0xd << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x1 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1c << TENSOR_IMA_SCP_LOC_B) | (0x1 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x9, ((0x15 << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0x8 << TENSOR_QUANT_QUANT_ROW) | (0x2f << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x0 << TENSOR_QUANT_TRANSF1) | (0x1 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fmvz.x.ps x31, f0, 0\n"
			"li x5, 0x38\n"
			"bne x5, x31, LBL_FAIL_HID_4\n"
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 10\n"
			"LBL_SEQID_1_TLD_XMA_STR_SEQ_ID_280421_HID_4:\n"
			"la x31, self_check_16\n"
			"li x5, 0x6000000000000b\n"
			"add x5, x5, x31\n"
			"li x6, 0x340000000000008\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x3 << TENSOR_FMA16_B_NUM_COLS) | (0xb << TENSOR_FMA16_A_NUM_ROWS) | (0x8 << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1a << TENSOR_FMA16_SCP_LOC_B) | (0x3 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
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
			"bne x5, x31, LBL_FAIL_HID_4\n"
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
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ITER_0_HID_4:\n"
			"li x5, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x2 << TENSOR_IMA_B_NUM_COLS) | (0x3 << TENSOR_IMA_A_NUM_ROWS) | (0x2 << TENSOR_IMA_A_NUM_COLS) | (0xa << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x23 << TENSOR_IMA_SCP_LOC_B) | (0x28 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x0 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ITER_1_HID_4:\n"
			"li x5, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x2 << TENSOR_FMA16_B_NUM_COLS) | (0xb << TENSOR_FMA16_A_NUM_ROWS) | (0x4 << TENSOR_FMA16_A_NUM_COLS) | (0x9 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0xab << TENSOR_FMA16_SCP_LOC_B) | (0xfd << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_4\n"
			"csrw tensor_error, x0\n"
			"li x5, 2\n" 
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
			"csrwi mhpmevent3, 6\n"   
			"csrwi mhpmevent4, 1\n"   
			"csrwi mhpmevent5, 3\n"   
			"csrwi mhpmevent6, 2\n"   
			"csrwi mhpmevent7, 23\n"   
			"csrwi mhpmevent8, 22\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f8,  992 (x5)\n"
			"flw.ps f4,  832 (x5)\n"
			"flw.ps f21,  0 (x5)\n"
			"flw.ps f7,  672 (x5)\n"
			"flw.ps f5,  384 (x5)\n"
			"flw.ps f26,  256 (x5)\n"
			"flw.ps f13,  160 (x5)\n"
			"flw.ps f27,  320 (x5)\n"
			"flw.ps f16,  544 (x5)\n"
			"flw.ps f10,  192 (x5)\n"
			"flw.ps f19,  448 (x5)\n"
			"flw.ps f14,  288 (x5)\n"
			"flw.ps f2,  928 (x5)\n"
			"flw.ps f29,  128 (x5)\n"
			"flw.ps f12,  800 (x5)\n"
			"flw.ps f24,  96 (x5)\n"
			"flw.ps f15,  864 (x5)\n"
			"flw.ps f30,  512 (x5)\n"
			"flw.ps f28,  960 (x5)\n"
			"flw.ps f9,  480 (x5)\n"
			"flw.ps f6,  768 (x5)\n"
			"flw.ps f22,  416 (x5)\n"
			"flw.ps f1,  64 (x5)\n"
			"flw.ps f17,  704 (x5)\n"
			"flw.ps f25,  224 (x5)\n"
			"flw.ps f11,  640 (x5)\n"
			"flw.ps f20,  896 (x5)\n"
			"flw.ps f23,  352 (x5)\n"
			"flw.ps f31,  32 (x5)\n"
			"flw.ps f18,  608 (x5)\n"
			"flw.ps f0,  736 (x5)\n"
			"flw.ps f3,  576 (x5)\n"
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
			"li x5, 0x2\n" 
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
			"la x13, rand_int_32\n"
			"flw.ps f24, 640(x13)\n"
			"flw.ps f11, 320(x13)\n"
			"fsra.pi f11, f24, f11\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f11, 896(x20)\n"
			"flw.ps f29, 0(x20)\n"
			"fmin.s f10, f11, f29\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_5:\n"
			"maskxor m0, m2, m3\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0xf4f051964d\n"
			"fsw.ps f0, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x27, rand_ieee754_32\n"
			"flw.ps f7, 992(x27)\n"
			"flw.ps f17, 480(x27)\n"
			"fsgnj.s f1, f7, f17\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f7, 640(x25)\n"
			"fcvt.f16.ps f30, f7\n" 
			"la x16, rand_int_32\n"
			"flw.ps f5, 992(x16)\n"
			"flw.ps f11, 512(x16)\n"
			"feq.pi f17, f5, f11\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f23, 640(x12)\n"
			"flw.ps f19, 64(x12)\n"
			"fadd.s f1, f23, f19, rtz\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f24, 928(x11)\n"
			"flw.ps f29, 160(x11)\n"
			"fmsub.ps f5, f24, f24, f29, rup\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_5:\n"
			"masknot m2, m0\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0x7b91c289e2\n"
			"fsw.ps f0, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x27, rand_ieee754_32\n"
			"flw.ps f30, 896(x27)\n"
			"fswizz.ps f11, f30, 0x17\n" 
			"la x5, rand_int_32\n"
			"flw.ps f5, 64(x5)\n"
			"flw.ps f25, 704(x5)\n"
			"fand.pi f3, f5, f25\n" 
			"li x13, 0x245f8cfb8add648f\n"
			"fcvt.s.w f24, x13, rup\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f21, 544(x11)\n"
			"fswizz.ps f1, f21, 0x17\n" 
			"la x11, rand_int_32\n"
			"flw.ps f24, 192(x11)\n"
			"flw.ps f1, 768(x11)\n"
			"fadd.pi f10, f24, f1\n" 
			"fbci.pi f19, 0x24763\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f23, 288(x13)\n"
			"fcvt.pwu.ps f30, f23, rtz\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_5\n"
			"addi x30, x30, 1\n"
			"la x11, rand_int_32\n"
			"flw.ps f1, 800(x11)\n"
			"fsrai.pi f21, f1, 0x4\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f21, 704(x20)\n"
			"flw.ps f30, 224(x20)\n"
			"fsgnjn.ps f7, f21, f30\n" 
			"LBL_SEQID_1_M0_WRITE_ID_2_HID_5:\n"
			"fltm.ps m2, f17, f7\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x16, rand_ieee754_32\n"
			"flw.ps f29, 32(x16)\n"
			"fsgnjn.ps f30, f29, f29\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f30, 704(x28)\n"
			"flw.ps f5, 672(x28)\n"
			"fle.s x12, f30, f5\n" 
			"LBL_SEQID_1_M0_WRITE_ID_3_HID_5:\n"
			"maskor m0, m7, m0\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x15, rand_ieee754_32\n"
			"flw.ps f24, 416(x15)\n"
			"flw.ps f23, 960(x15)\n"
			"flw.ps f5, 480(x15)\n"
			"fmsub.s f23, f24, f23, f5, rdn\n" 
			"la x6, rand_int_32\n"
			"flw.ps f13, 96(x6)\n"
			"flw.ps f25, 64(x6)\n"
			"fltu.pi f7, f13, f25\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f1, 128(x15)\n"
			"frsq.ps f21, f1\n" 
			"fbci.pi f30, 0x2a2f6\n" 
			"LBL_SEQID_1_M0_WRITE_ID_4_HID_5:\n"
			"feqm.ps m7, f17, f21\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x27, rand_ieee754_32\n"
			"flw.ps f5, 576(x27)\n"
			"fcvt.pwu.ps f13, f5, rtz\n" 
			"LBL_SEQID_1_FP_TRANS_ID_5_HID_5:\n"
			"la x13, rand_ieee754_32\n"
			"flw.ps f5, 736(x13)\n"
			"fexp.ps f24, f5\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f8, 320(x25)\n"
			"flw.ps f19, 544(x25)\n"
			"flw.ps f25, 608(x25)\n"
			"fnmsub.ps f10, f8, f19, f25, rmm\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f1, 832(x16)\n"
			"fswizz.ps f5, f1, 0x1e\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f30, 736(x11)\n"
			"flw.ps f10, 672(x11)\n"
			"fsgnjn.ps f17, f30, f10\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 2\n" 
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
			"csrwi mhpmevent3, 4\n"   
			"csrwi mhpmevent4, 8\n"   
			"csrwi mhpmevent5, 18\n"   
			"csrwi mhpmevent6, 21\n"   
			"csrwi mhpmevent7, 4\n"   
			"csrwi mhpmevent8, 3\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f5,  832 (x5)\n"
			"flw.ps f16,  192 (x5)\n"
			"flw.ps f9,  256 (x5)\n"
			"flw.ps f27,  544 (x5)\n"
			"flw.ps f18,  320 (x5)\n"
			"flw.ps f7,  32 (x5)\n"
			"flw.ps f6,  64 (x5)\n"
			"flw.ps f12,  0 (x5)\n"
			"flw.ps f1,  640 (x5)\n"
			"flw.ps f29,  288 (x5)\n"
			"flw.ps f19,  896 (x5)\n"
			"flw.ps f11,  768 (x5)\n"
			"flw.ps f31,  960 (x5)\n"
			"flw.ps f13,  704 (x5)\n"
			"flw.ps f10,  416 (x5)\n"
			"flw.ps f20,  800 (x5)\n"
			"flw.ps f25,  480 (x5)\n"
			"flw.ps f28,  512 (x5)\n"
			"flw.ps f3,  864 (x5)\n"
			"flw.ps f0,  608 (x5)\n"
			"flw.ps f21,  448 (x5)\n"
			"flw.ps f2,  352 (x5)\n"
			"flw.ps f15,  160 (x5)\n"
			"flw.ps f24,  224 (x5)\n"
			"flw.ps f4,  992 (x5)\n"
			"flw.ps f30,  384 (x5)\n"
			"flw.ps f23,  576 (x5)\n"
			"flw.ps f17,  928 (x5)\n"
			"flw.ps f14,  128 (x5)\n"
			"flw.ps f22,  672 (x5)\n"
			"flw.ps f26,  736 (x5)\n"
			"flw.ps f8,  96 (x5)\n"
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
			"li x5, 0x2\n" 
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
			"masknot m3, m3\n" 
			"li x5, 0x80340168\n"
			"ld x5, 0(x5)\n"
			"la x9, rand_ieee754_32\n"
			"flw.ps f27, 672(x9)\n"
			"fsgnjn.s f9, f27, f27\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f9, 96(x22)\n"
			"fcvt.w.s x13, f9, rtz\n" 
			"la x6, rand_int_32\n"
			"flw.ps f16, 160(x6)\n"
			"fsat8.pi f31, f16\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f3, 704(x12)\n"
			"flw.ps f16, 32(x12)\n"
			"fdiv.s f29, f3, f16, rdn\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f1, 64(x20)\n"
			"fcvt.pwu.ps f27, f1, rdn\n" 
			"la x22, rand_int_32\n"
			"flw.ps f6, 448(x22)\n"
			"fnot.pi f18, f6\n" 
			"la x11, rand_int_32\n"
			"flw.ps f3, 256(x11)\n"
			"fsat8.pi f22, f3\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f1, 544(x6)\n"
			"flw.ps f16, 544(x6)\n"
			"fadd.ps f9, f1, f16, rne\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f1, 320(x16)\n"
			"flw.ps f2, 896(x16)\n"
			"fadd.s f1, f1, f2, rmm\n" 
			"fbci.ps f9, 0x1277c\n" 
			"la x20, rand_int_32\n"
			"flw.ps f10, 736(x20)\n"
			"fcvt.ps.pwu f16, f10, rup\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x18, rand_ieee754_32\n"
			"flw.ps f9, 480(x18)\n"
			"flw.ps f25, 160(x18)\n"
			"fmul.ps f22, f9, f25, rtz\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f10, 224(x20)\n"
			"fcvt.f16.ps f11, f10\n" 
			"la x16, rand_int_32\n"
			"flw.ps f18, 480(x16)\n"
			"fadd.pi f31, f18, f18\n" 
			"la x18, rand_int_32\n"
			"flw.ps f9, 352(x18)\n"
			"flw.ps f18, 608(x18)\n"
			"fand.pi f22, f9, f18\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_0_RDX_ID_0_SHIRE_0_HID_6:\n"
			"li x11, 0x3e00000000040001\n"
			"csrr x7, mhartid\n"
			"srli x7, x7, 4\n"
			"slli x7, x7, 6\n"
			"add x11, x11, x7\n"
			"csrw tensor_reduce, x11\n" 
			"csrwi tensor_wait, 9\n" 
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=0); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"li x5, 0xca52\n"
			"csrw tensor_mask, x5\n"	
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ITER_0_HID_6:\n"
			"la x31, self_check_32\n"
			"li x5, 0xe0000000000004\n"
			"add x5, x5, x31\n"
			"li x6, 0x1000000000000d\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x2 << TENSOR_FMA32_B_NUM_COLS) | (0x4 << TENSOR_FMA32_A_NUM_ROWS) | (0xd << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x1 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1e << TENSOR_FMA32_SCP_LOC_B) | (0x7 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x1c << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0x4 << TENSOR_QUANT_QUANT_ROW) | (0x27 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 10\n"
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ITER_1_HID_6:\n"
			"la x31, self_check_32\n"
			"li x5, 0x60000000000005\n"
			"add x5, x5, x31\n"
			"li x6, 0x10000000000001\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x3 << TENSOR_FMA32_B_NUM_COLS) | (0x5 << TENSOR_FMA32_A_NUM_ROWS) | (0x1 << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x1 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1b << TENSOR_FMA32_SCP_LOC_B) | (0x3 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x9, ((0xd << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0x5 << TENSOR_QUANT_QUANT_ROW) | (0x6 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x7 << TENSOR_QUANT_TRANSF8) | (0x7 << TENSOR_QUANT_TRANSF7) | (0x6 << TENSOR_QUANT_TRANSF6) | (0x7 << TENSOR_QUANT_TRANSF5) | (0x7 << TENSOR_QUANT_TRANSF4) | (0x6 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0x2\n"
			"bne x5, x31, LBL_FAIL_HID_6\n"
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_6\n"
			"csrw tensor_error, x0\n"
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_6\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_1_M0_WRITE_ID_1_HID_6:\n"
			"fltm.pi m3, f25, f10\n" 
			"li x5, 0x80340178\n"
			"ld x5, 0(x5)\n"
			"la x22, rand_int_32\n"
			"flw.ps f27, 832(x22)\n"
			"flw.ps f28, 64(x22)\n"
			"fle.pi f27, f27, f28\n" 
			"la x16, rand_int_32\n"
			"flw.ps f2, 288(x16)\n"
			"flw.ps f3, 384(x16)\n"
			"fminu.pi f3, f2, f3\n" 
			"fbci.ps f6, 0x1c6d7\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f28, 832(x7)\n"
			"flw.ps f29, 224(x7)\n"
			"fmin.s f10, f28, f29\n" 
			"LBL_SEQID_1_FP_TRANS_ID_2_HID_6:\n"
			"la x7, rand_ieee754_32\n"
			"flw.ps f16, 64(x7)\n"
			"fexp.ps f22, f16\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f25, 384(x20)\n"
			"flw.ps f29, 896(x20)\n"
			"fle.s x9, f25, f29\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f18, 0(x12)\n"
			"flw.ps f28, 320(x12)\n"
			"fsgnjn.ps f29, f18, f28\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f0, 0(x9)\n"
			"flw.ps f31, 288(x9)\n"
			"fsub.s f9, f0, f31, rup\n" 
			"la x23, rand_int_32\n"
			"flw.ps f29, 96(x23)\n"
			"flw.ps f31, 64(x23)\n"
			"fand.pi f28, f29, f31\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f0, 672(x12)\n"
			"flw.ps f27, 512(x12)\n"
			"fsgnj.s f2, f0, f27\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f27, 0(x22)\n"
			"flw.ps f29, 672(x22)\n"
			"fmin.s f22, f27, f29\n" 
			"li x5, 0xd074\n"
			"csrw tensor_mask, x5\n"	
			"LBL_SEQID_1_M0_WRITE_ID_3_HID_6:\n"
			"mova.m.x x11\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x16, rand_ieee754_32\n"
			"flw.ps f25, 224(x16)\n"
			"flw.ps f27, 352(x16)\n"
			"fsub.s f27, f25, f27, rtz\n" 
			"LBL_SEQID_1_M0_WRITE_ID_4_HID_6:\n"
			"mov.m.x m4, x22, 0x4\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x20, rand_ieee754_32\n"
			"flw.ps f11, 160(x20)\n"
			"flw.ps f31, 320(x20)\n"
			"fcmovm.ps f22, f11, f31\n" 
			"li x5, 0xc74f\n"
			"csrw tensor_mask, x5\n"	
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_1_RDX_ID_1_SHIRE_0_HID_6:\n"
			"li x12, 0x120000000435000b\n"
			"csrw tensor_reduce, x12\n" 
			"csrwi tensor_wait, 9\n" 
			"LBL_SEQID_1_TLD_XMA_STR_SEQ_ID_174578_HID_6:\n"
			"la x31, self_check_16\n"
			"li x5, 0x6\n"
			"add x5, x5, x31\n"
			"li x6, 0x3e0000000000002\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x0 << TENSOR_FMA16_B_NUM_COLS) | (0x6 << TENSOR_FMA16_A_NUM_ROWS) | (0x2 << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1f << TENSOR_FMA16_SCP_LOC_B) | (0x0 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
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
			"LBL_SEQID_1_TLD_XMA_STR_SEQ_ITER_0_HID_6:\n"
			"la x31, self_check_16\n"
			"li x5, 0x2000000000000b\n"
			"add x5, x5, x31\n"
			"li x6, 0x32000000000000f\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x3 << TENSOR_FMA16_B_NUM_COLS) | (0xb << TENSOR_FMA16_A_NUM_ROWS) | (0xf << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x19 << TENSOR_FMA16_SCP_LOC_B) | (0x1 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
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
			"bne x5, x31, LBL_FAIL_HID_6\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_6\n"
			"csrw tensor_error, x0\n"
			"li x5, 2\n" 
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
			"csrwi mhpmevent3, 6\n"   
			"csrwi mhpmevent4, 1\n"   
			"csrwi mhpmevent5, 3\n"   
			"csrwi mhpmevent6, 2\n"   
			"csrwi mhpmevent7, 23\n"   
			"csrwi mhpmevent8, 22\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f9,  256 (x5)\n"
			"flw.ps f17,  0 (x5)\n"
			"flw.ps f3,  160 (x5)\n"
			"flw.ps f20,  224 (x5)\n"
			"flw.ps f13,  640 (x5)\n"
			"flw.ps f16,  576 (x5)\n"
			"flw.ps f1,  544 (x5)\n"
			"flw.ps f5,  608 (x5)\n"
			"flw.ps f22,  384 (x5)\n"
			"flw.ps f21,  736 (x5)\n"
			"flw.ps f19,  896 (x5)\n"
			"flw.ps f18,  480 (x5)\n"
			"flw.ps f24,  320 (x5)\n"
			"flw.ps f2,  288 (x5)\n"
			"flw.ps f28,  672 (x5)\n"
			"flw.ps f8,  32 (x5)\n"
			"flw.ps f14,  832 (x5)\n"
			"flw.ps f11,  992 (x5)\n"
			"flw.ps f25,  128 (x5)\n"
			"flw.ps f7,  64 (x5)\n"
			"flw.ps f4,  800 (x5)\n"
			"flw.ps f27,  512 (x5)\n"
			"flw.ps f29,  192 (x5)\n"
			"flw.ps f10,  864 (x5)\n"
			"flw.ps f30,  352 (x5)\n"
			"flw.ps f0,  448 (x5)\n"
			"flw.ps f6,  96 (x5)\n"
			"flw.ps f15,  768 (x5)\n"
			"flw.ps f26,  928 (x5)\n"
			"flw.ps f12,  704 (x5)\n"
			"flw.ps f31,  960 (x5)\n"
			"flw.ps f23,  416 (x5)\n"
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
			"li x5, 0x2\n" 
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
			"la x10, rand_int_32\n"
			"flw.ps f28, 576(x10)\n"
			"flw.ps f9, 640(x10)\n"
			"fltu.pi f0, f28, f9\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f13, 832(x26)\n"
			"flw.ps f8, 992(x26)\n"
			"flw.ps f9, 768(x26)\n"
			"fnmsub.ps f26, f13, f8, f9, rmm\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f31, 160(x23)\n"
			"flw.ps f5, 512(x23)\n"
			"flw.ps f17, 192(x23)\n"
			"fnmadd.ps f28, f31, f5, f17, rtz\n" 
			"la x14, rand_int_32\n"
			"flw.ps f11, 160(x14)\n"
			"fpackreph.pi f5, f11\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f25, 864(x23)\n"
			"fmul.ps f12, f25, f25, rtz\n" 
			"fbci.ps f22, 0x2c305\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f9, 992(x15)\n"
			"flw.ps f22, 640(x15)\n"
			"fsub.s f20, f9, f22, rtz\n" 
			"la x25, rand_int_32\n"
			"flw.ps f18, 672(x25)\n"
			"fsat8.pi f8, f18\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f5, 224(x14)\n"
			"flw.ps f9, 480(x14)\n"
			"fmul.s f11, f5, f9, rup\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f28, 128(x15)\n"
			"fsqrt.s f22, f28, rmm\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f5, 896(x10)\n"
			"flw.ps f26, 640(x10)\n"
			"fmin.s f21, f5, f26\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f9, 160(x13)\n"
			"flw.ps f20, 96(x13)\n"
			"fmul.ps f31, f9, f20, rtz\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f0, 992(x17)\n"
			"flw.ps f26, 864(x17)\n"
			"flw.ps f25, 256(x17)\n"
			"fnmadd.s f20, f0, f26, f25, rmm\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f26, 704(x25)\n"
			"flw.ps f28, 64(x25)\n"
			"fmul.s f5, f26, f28, rdn\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f31, 384(x14)\n"
			"flw.ps f0, 864(x14)\n"
			"fle.ps f21, f31, f0\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f9, 928(x13)\n"
			"fswizz.ps f28, f9, 0xb\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_7\n"
			"addi x30, x30, 1\n"
			"la x13, rand_int_32\n"
			"flw.ps f8, 256(x13)\n"
			"fcvt.ps.pw f22, f8, rup\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f11, 864(x14)\n"
			"fclass.s x26, f11\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f8, 320(x10)\n"
			"flw.ps f11, 832(x10)\n"
			"fnmadd.ps f28, f8, f11, f11, rne\n" 
			"la x15, rand_int_32\n"
			"flw.ps f8, 160(x15)\n"
			"flw.ps f31, 448(x15)\n"
			"feq.pi f0, f8, f31\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f12, 480(x10)\n"
			"fmin.s f31, f12, f12\n" 
			"la x10, rand_int_32\n"
			"flw.ps f21, 352(x10)\n"
			"flw.ps f17, 736(x10)\n"
			"fadd.pi f17, f21, f17\n" 
			"la x25, rand_int_32\n"
			"flw.ps f12, 96(x25)\n"
			"fpackrepb.pi f21, f12\n" 
			"la x11, rand_int_32\n"
			"flw.ps f9, 64(x11)\n"
			"flw.ps f12, 992(x11)\n"
			"for.pi f26, f9, f12\n" 
			"LBL_SEQID_1_M0_WRITE_ID_0_HID_7:\n"
			"flem.ps m6, f26, f8\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_1_FP_TRANS_ID_1_HID_7:\n"
			"la x10, rand_ieee754_32\n"
			"flw.ps f5, 96(x10)\n"
			"flog.ps f22, f5\n" 
			"LBL_SEQID_1_M0_WRITE_ID_2_HID_7:\n"
			"mova.m.x x14\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0x979676b789\n"
			"flw.ps f5, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x25, rand_int_32\n"
			"flw.ps f0, 768(x25)\n"
			"flw.ps f28, 992(x25)\n"
			"fadd.pi f5, f0, f28\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f20, 864(x26)\n"
			"flw.ps f12, 128(x26)\n"
			"feq.ps f26, f20, f12\n" 
			"la x14, rand_int_32\n"
			"flw.ps f25, 832(x14)\n"
			"faddi.pi f21, f25, 0x3b\n" 
			"la x28, rand_int_32\n"
			"flw.ps f11, 64(x28)\n"
			"fslli.pi f8, f11, 0x8\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f9, 0(x17)\n"
			"fcvt.l.s x28, f9, rdn\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 2\n" 
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
			"csrwi mhpmevent3, 4\n"   
			"csrwi mhpmevent4, 8\n"   
			"csrwi mhpmevent5, 18\n"   
			"csrwi mhpmevent6, 21\n"   
			"csrwi mhpmevent7, 4\n"   
			"csrwi mhpmevent8, 3\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f14,  320 (x5)\n"
			"flw.ps f23,  800 (x5)\n"
			"flw.ps f1,  960 (x5)\n"
			"flw.ps f31,  96 (x5)\n"
			"flw.ps f11,  32 (x5)\n"
			"flw.ps f29,  160 (x5)\n"
			"flw.ps f3,  672 (x5)\n"
			"flw.ps f30,  928 (x5)\n"
			"flw.ps f9,  480 (x5)\n"
			"flw.ps f24,  704 (x5)\n"
			"flw.ps f19,  416 (x5)\n"
			"flw.ps f0,  288 (x5)\n"
			"flw.ps f4,  448 (x5)\n"
			"flw.ps f5,  832 (x5)\n"
			"flw.ps f26,  864 (x5)\n"
			"flw.ps f21,  608 (x5)\n"
			"flw.ps f18,  64 (x5)\n"
			"flw.ps f15,  192 (x5)\n"
			"flw.ps f2,  768 (x5)\n"
			"flw.ps f10,  896 (x5)\n"
			"flw.ps f8,  352 (x5)\n"
			"flw.ps f27,  576 (x5)\n"
			"flw.ps f17,  992 (x5)\n"
			"flw.ps f28,  512 (x5)\n"
			"flw.ps f13,  544 (x5)\n"
			"flw.ps f12,  736 (x5)\n"
			"flw.ps f25,  128 (x5)\n"
			"flw.ps f6,  384 (x5)\n"
			"flw.ps f22,  640 (x5)\n"
			"flw.ps f7,  224 (x5)\n"
			"flw.ps f20,  0 (x5)\n"
			"flw.ps f16,  256 (x5)\n"
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
			"li x5, 0x2\n" 
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
			"la x7, rand_ieee754_32\n"
			"flw.ps f5, 384(x7)\n"
			"frsq.ps f27, f5\n" 
			"li x5, 0x832c2357c3dc8432\n"
			"fcvt.s.w f23, x5, rne\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_8:\n"
			"mov.m.x m6, x5, 0x52\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0xf0afdbe1a3\n"
			"flw.ps f5, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_8:\n"
			"maskxor m6, m1, m6\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"li x5, 0x162e\n"
			"csrw tensor_mask, x5\n"	
			"la x7, rand_ieee754_32\n"
			"flw.ps f18, 320(x7)\n"
			"flw.ps f14, 288(x7)\n"
			"fmin.s f29, f18, f14\n" 
			"LBL_SEQID_0_M0_WRITE_ID_2_HID_8:\n"
			"feqm.ps m5, f20, f13\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x5, rand_ieee754_32\n"
			"flw.ps f26, 992(x5)\n"
			"flw.ps f5, 224(x5)\n"
			"fdiv.s f10, f26, f5, rtz\n" 
			"LBL_SEQID_0_M0_WRITE_ID_3_HID_8:\n"
			"feqm.ps m5, f23, f27\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x9, rand_ieee754_32\n"
			"flw.ps f5, 608(x9)\n"
			"flw.ps f13, 416(x9)\n"
			"fsgnjx.s f2, f5, f13\n" 
			"la x14, rand_int_32\n"
			"flw.ps f27, 192(x14)\n"
			"fpackrepb.pi f14, f27\n" 
			"la x7, rand_int_32\n"
			"flw.ps f26, 96(x7)\n"
			"fandi.pi f27, f26, 0x1ae\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f30, 928(x7)\n"
			"flw.ps f1, 416(x7)\n"
			"fadd.s f30, f30, f1, rne\n" 
			"li x5, 0x805b\n"
			"csrw tensor_mask, x5\n"	
			"la x7, rand_ieee754_32\n"
			"flw.ps f20, 224(x7)\n"
			"flw.ps f27, 672(x7)\n"
			"flw.ps f14, 224(x7)\n"
			"fnmsub.ps f1, f20, f27, f14, rmm\n" 
			"la x14, rand_int_32\n"
			"flw.ps f5, 608(x14)\n"
			"fsrli.pi f29, f5, 0x2\n" 
			"LBL_SEQID_0_FP_TRANS_ID_4_HID_8:\n"
			"la x15, rand_ieee754_32\n"
			"flw.ps f2, 864(x15)\n"
			"flog.ps f5, f2\n" 
			"LBL_SEQID_0_FP_TRANS_ID_5_HID_8:\n"
			"la x14, rand_ieee754_32\n"
			"flw.ps f14, 704(x14)\n"
			"flog.ps f7, f14\n" 
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
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ITER_0_HID_8:\n"
			"la x31, self_check_32\n"
			"li x5, 0xa0000000000007\n"
			"add x5, x5, x31\n"
			"li x6, 0x34000000000000e\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x0 << TENSOR_FMA32_B_NUM_COLS) | (0x7 << TENSOR_FMA32_A_NUM_ROWS) | (0xe << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1a << TENSOR_FMA32_SCP_LOC_B) | (0x5 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x1 << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0x7 << TENSOR_QUANT_QUANT_ROW) | (0x27 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x0 << TENSOR_QUANT_TRANSF1) | (0x0 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0xf\n"
			"bne x5, x31, LBL_FAIL_HID_8\n"
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_8\n"
			"csrw tensor_error, x0\n"
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_8\n"
			"addi x30, x30, 1\n"
			"la x14, rand_ieee754_32\n"
			"flw.ps f25, 896(x14)\n"
			"fswizz.ps f20, f25, 0x7\n" 
			"LBL_SEQID_1_FP_TRANS_ID_6_HID_8:\n"
			"la x9, rand_ieee754_32\n"
			"flw.ps f20, 480(x9)\n"
			"frcp.ps f14, f20\n" 
			"LBL_SEQID_1_M0_WRITE_ID_7_HID_8:\n"
			"feqm.ps m1, f14, f27\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x23, rand_int_32\n"
			"flw.ps f25, 384(x23)\n"
			"flw.ps f10, 288(x23)\n"
			"flt.pi f10, f25, f10\n" 
			"la x14, rand_int_32\n"
			"flw.ps f27, 416(x14)\n"
			"flw.ps f20, 512(x14)\n"
			"fmul.pi f18, f27, f20\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f25, 256(x13)\n"
			"fcvt.pw.ps f27, f25, rup\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f27, 416(x26)\n"
			"flw.ps f20, 448(x26)\n"
			"fsgnjx.s f26, f27, f20\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f2, 352(x26)\n"
			"flw.ps f23, 960(x26)\n"
			"fsgnj.ps f27, f2, f23\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f5, 128(x13)\n"
			"flw.ps f1, 288(x13)\n"
			"fle.ps f20, f5, f1\n" 
			"LBL_SEQID_1_M0_WRITE_ID_8_HID_8:\n"
			"feqm.ps m3, f5, f28\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_1_M0_WRITE_ID_9_HID_8:\n"
			"fsetm.pi m3, f2\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x26, rand_ieee754_32\n"
			"flw.ps f29, 576(x26)\n"
			"flw.ps f14, 160(x26)\n"
			"flw.ps f25, 736(x26)\n"
			"fnmadd.ps f1, f29, f14, f25, rne\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f5, 672(x9)\n"
			"fcvt.wu.s x6, f5, rtz\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f1, 704(x6)\n"
			"fround.ps f18, f1, rmm\n" 
			"li x16, 0xe48d93a1b52fc9c7\n"
			"fcvt.s.wu f1, x16, rmm\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f7, 352(x26)\n"
			"fclass.s x26, f7\n" 
			"li x5, 0x121f\n"
			"csrw tensor_mask, x5\n"	
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_1_RDX_ID_1_SHIRE_0_HID_8:\n"
			"li x13, 0x120000000435000b\n"
			"csrw tensor_reduce, x13\n" 
			"csrwi tensor_wait, 9\n" 
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=0); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"LBL_SEQID_1_TLD_XMA_STR_SEQ_ITER_0_HID_8:\n"
			"la x31, self_check_32\n"
			"li x5, 0xc000000000000e\n"
			"add x5, x5, x31\n"
			"li x6, 0x3e0000000000002\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x3 << TENSOR_FMA32_B_NUM_COLS) | (0xe << TENSOR_FMA32_A_NUM_ROWS) | (0x2 << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1f << TENSOR_FMA32_SCP_LOC_B) | (0x6 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x18 << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0xe << TENSOR_QUANT_QUANT_ROW) | (0x2f << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x2 << TENSOR_QUANT_TRANSF7) | (0x6 << TENSOR_QUANT_TRANSF6) | (0x7 << TENSOR_QUANT_TRANSF5) | (0x7 << TENSOR_QUANT_TRANSF4) | (0x7 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0x3\n"
			"bne x5, x31, LBL_FAIL_HID_8\n"
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_8\n"
			"csrw tensor_error, x0\n"
			"li x5, 2\n" 
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
			"csrwi mhpmevent3, 6\n"   
			"csrwi mhpmevent4, 1\n"   
			"csrwi mhpmevent5, 3\n"   
			"csrwi mhpmevent6, 2\n"   
			"csrwi mhpmevent7, 23\n"   
			"csrwi mhpmevent8, 22\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f30,  800 (x5)\n"
			"flw.ps f6,  512 (x5)\n"
			"flw.ps f15,  544 (x5)\n"
			"flw.ps f4,  928 (x5)\n"
			"flw.ps f28,  192 (x5)\n"
			"flw.ps f29,  608 (x5)\n"
			"flw.ps f3,  576 (x5)\n"
			"flw.ps f5,  128 (x5)\n"
			"flw.ps f20,  864 (x5)\n"
			"flw.ps f24,  96 (x5)\n"
			"flw.ps f8,  480 (x5)\n"
			"flw.ps f9,  832 (x5)\n"
			"flw.ps f27,  896 (x5)\n"
			"flw.ps f21,  32 (x5)\n"
			"flw.ps f26,  256 (x5)\n"
			"flw.ps f0,  704 (x5)\n"
			"flw.ps f17,  960 (x5)\n"
			"flw.ps f18,  0 (x5)\n"
			"flw.ps f1,  64 (x5)\n"
			"flw.ps f10,  416 (x5)\n"
			"flw.ps f25,  384 (x5)\n"
			"flw.ps f11,  640 (x5)\n"
			"flw.ps f7,  672 (x5)\n"
			"flw.ps f19,  352 (x5)\n"
			"flw.ps f13,  160 (x5)\n"
			"flw.ps f14,  992 (x5)\n"
			"flw.ps f23,  736 (x5)\n"
			"flw.ps f16,  288 (x5)\n"
			"flw.ps f22,  768 (x5)\n"
			"flw.ps f31,  448 (x5)\n"
			"flw.ps f12,  224 (x5)\n"
			"flw.ps f2,  320 (x5)\n"
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
			"li x5, 0x2\n" 
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
			"la x15, rand_int_32\n"
			"flw.ps f18, 480(x15)\n"
			"flw.ps f3, 160(x15)\n"
			"fsra.pi f12, f18, f3\n" 
			"la x15, rand_int_32\n"
			"flw.ps f27, 448(x15)\n"
			"fpackrepb.pi f27, f27\n" 
			"la x6, rand_int_32\n"
			"flw.ps f16, 960(x6)\n"
			"fsat8.pi f10, f16\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f3, 928(x19)\n"
			"flw.ps f25, 832(x19)\n"
			"fmax.ps f16, f3, f25\n" 
			"la x14, rand_int_32\n"
			"flw.ps f8, 672(x14)\n"
			"flw.ps f28, 288(x14)\n"
			"fltu.pi f11, f8, f28\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f10, 32(x12)\n"
			"fcvt.lu.s x12, f10, rdn\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f22, 96(x12)\n"
			"flw.ps f8, 544(x12)\n"
			"fsgnjn.ps f14, f22, f8\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f24, 576(x11)\n"
			"fmul.s f24, f24, f24, rne\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f11, 64(x12)\n"
			"fmax.ps f16, f11, f11\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f7, 864(x7)\n"
			"flw.ps f22, 832(x7)\n"
			"flw.ps f24, 224(x7)\n"
			"fmadd.s f17, f7, f22, f24, rdn\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f28, 128(x12)\n"
			"ffrc.ps f3, f28\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_9:\n"
			"maskxor m0, m0, m2\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_0_FP_TRANS_ID_1_HID_9:\n"
			"la x18, rand_ieee754_32\n"
			"flw.ps f3, 704(x18)\n"
			"frcp.ps f7, f3\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f10, 384(x12)\n"
			"flw.ps f24, 768(x12)\n"
			"fmul.ps f18, f10, f24, rtz\n" 
			"la x15, rand_int_32\n"
			"flw.ps f11, 768(x15)\n"
			"flw.ps f28, 288(x15)\n"
			"fsra.pi f11, f11, f28\n" 
			"LBL_SEQID_0_M0_WRITE_ID_2_HID_9:\n"
			"maskor m2, m6, m0\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0xFF\n"
			"csrw tensor_error, zero\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_9\n"
			"addi x30, x30, 1\n"
			"la x20, rand_int_32\n"
			"flw.ps f17, 544(x20)\n"
			"flw.ps f3, 736(x20)\n"
			"fmulh.pi f17, f17, f3\n" 
			"LBL_SEQID_1_FP_TRANS_ID_3_HID_9:\n"
			"la x19, rand_ieee754_32\n"
			"flw.ps f10, 352(x19)\n"
			"fexp.ps f8, f10\n" 
			"la x23, rand_int_32\n"
			"flw.ps f11, 64(x23)\n"
			"flw.ps f25, 32(x23)\n"
			"feq.pi f25, f11, f25\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f25, 96(x20)\n"
			"flw.ps f7, 416(x20)\n"
			"feq.s x6, f25, f7\n" 
			"la x6, rand_int_32\n"
			"flw.ps f7, 352(x6)\n"
			"flw.ps f30, 352(x6)\n"
			"fmaxu.pi f11, f7, f30\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f18, 224(x7)\n"
			"flw.ps f12, 32(x7)\n"
			"fnmadd.s f27, f18, f18, f12, rdn\n" 
			"la x14, rand_int_32\n"
			"flw.ps f7, 896(x14)\n"
			"flw.ps f25, 832(x14)\n"
			"fsub.pi f27, f7, f25\n" 
			"la x11, rand_int_32\n"
			"flw.ps f25, 384(x11)\n"
			"flw.ps f8, 544(x11)\n"
			"fltu.pi f11, f25, f8\n" 
			"li x11, 0x4f61867440f353f8\n"
			"fcvt.s.l f22, x11, rdn\n" 
			"la x23, rand_int_32\n"
			"flw.ps f8, 576(x23)\n"
			"fpackreph.pi f27, f8\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f14, 672(x11)\n"
			"fcvt.wu.s x20, f14, rtz\n" 
			"la x7, rand_int_32\n"
			"flw.ps f17, 928(x7)\n"
			"fcvt.ps.pwu f16, f17, rtz\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f10, 480(x6)\n"
			"flw.ps f8, 896(x6)\n"
			"flt.s x14, f10, f8\n" 
			"maskpopcz x20, m2\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f25, 800(x14)\n"
			"flw.ps f3, 992(x14)\n"
			"fdiv.s f8, f25, f3, rdn\n" 
			"la x18, rand_int_32\n"
			"flw.ps f10, 0(x18)\n"
			"fle.pi f30, f10, f10\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 2\n" 
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
			"csrwi mhpmevent3, 4\n"   
			"csrwi mhpmevent4, 8\n"   
			"csrwi mhpmevent5, 18\n"   
			"csrwi mhpmevent6, 21\n"   
			"csrwi mhpmevent7, 4\n"   
			"csrwi mhpmevent8, 3\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f23,  320 (x5)\n"
			"flw.ps f11,  96 (x5)\n"
			"flw.ps f26,  544 (x5)\n"
			"flw.ps f3,  128 (x5)\n"
			"flw.ps f10,  160 (x5)\n"
			"flw.ps f27,  64 (x5)\n"
			"flw.ps f18,  736 (x5)\n"
			"flw.ps f15,  288 (x5)\n"
			"flw.ps f31,  576 (x5)\n"
			"flw.ps f13,  864 (x5)\n"
			"flw.ps f24,  768 (x5)\n"
			"flw.ps f25,  960 (x5)\n"
			"flw.ps f29,  480 (x5)\n"
			"flw.ps f1,  224 (x5)\n"
			"flw.ps f9,  32 (x5)\n"
			"flw.ps f20,  640 (x5)\n"
			"flw.ps f30,  416 (x5)\n"
			"flw.ps f28,  992 (x5)\n"
			"flw.ps f5,  832 (x5)\n"
			"flw.ps f8,  704 (x5)\n"
			"flw.ps f4,  672 (x5)\n"
			"flw.ps f6,  896 (x5)\n"
			"flw.ps f19,  448 (x5)\n"
			"flw.ps f16,  384 (x5)\n"
			"flw.ps f2,  352 (x5)\n"
			"flw.ps f0,  0 (x5)\n"
			"flw.ps f7,  800 (x5)\n"
			"flw.ps f21,  512 (x5)\n"
			"flw.ps f14,  256 (x5)\n"
			"flw.ps f17,  192 (x5)\n"
			"flw.ps f22,  608 (x5)\n"
			"flw.ps f12,  928 (x5)\n"
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
			"li x5, 0x2\n" 
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
			"la x18, rand_ieee754_32\n"
			"flw.ps f31, 416(x18)\n"
			"flw.ps f0, 416(x18)\n"
			"fsgnj.s f31, f31, f0\n" 
			"LBL_SEQID_0_FP_TRANS_ID_0_HID_10:\n"
			"la x26, rand_ieee754_32\n"
			"flw.ps f31, 896(x26)\n"
			"frcp.ps f21, f31\n" 
			"la x16, rand_int_32\n"
			"flw.ps f4, 224(x16)\n"
			"flw.ps f12, 96(x16)\n"
			"feq.pi f12, f4, f12\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_10:\n"
			"maskand m3, m7, m3\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x13, rand_ieee754_32\n"
			"flw.ps f21, 576(x13)\n"
			"fcvt.pw.ps f24, f21, rtz\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f0, 544(x18)\n"
			"flw.ps f10, 736(x18)\n"
			"fnmadd.s f29, f0, f10, f10, rtz\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f13, 32(x9)\n"
			"flw.ps f24, 960(x9)\n"
			"flt.ps f13, f13, f24\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f6, 608(x13)\n"
			"fclass.s x14, f6\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f10, 64(x25)\n"
			"flw.ps f13, 864(x25)\n"
			"fsub.s f20, f10, f13, rtz\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f18, 0(x25)\n"
			"fcvt.f16.ps f18, f18\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f4, 352(x11)\n"
			"flw.ps f0, 160(x11)\n"
			"fnmsub.ps f22, f4, f0, f4, rne\n" 
			"la x16, rand_int_32\n"
			"flw.ps f20, 640(x16)\n"
			"fnot.pi f20, f20\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x9, rand_int_32\n"
			"flw.ps f31, 800(x9)\n"
			"flw.ps f17, 832(x9)\n"
			"fminu.pi f10, f31, f17\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f12, 32(x26)\n"
			"flw.ps f18, 160(x26)\n"
			"flw.ps f16, 544(x26)\n"
			"fmsub.s f0, f12, f18, f16, rtz\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f17, 128(x10)\n"
			"flw.ps f27, 640(x10)\n"
			"fmin.s f17, f17, f27\n" 
			"la x13, rand_int_32\n"
			"flw.ps f0, 288(x13)\n"
			"flw.ps f4, 896(x13)\n"
			"fmulh.pi f0, f0, f4\n" 
			"csrw tensor_error, zero\n" 
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=0); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ITER_0_HID_10:\n"
			"li x5, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x1 << TENSOR_FMA32_B_NUM_COLS) | (0xb << TENSOR_FMA32_A_NUM_ROWS) | (0x7 << TENSOR_FMA32_A_NUM_COLS) | (0xf << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x4a << TENSOR_FMA32_SCP_LOC_B) | (0xab << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x0 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_10\n"
			"csrw tensor_error, x0\n"
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_10\n"
			"addi x30, x30, 1\n"
			"la x18, rand_int_32\n"
			"flw.ps f12, 768(x18)\n"
			"fcvt.ps.pwu f13, f12, rup\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f29, 608(x10)\n"
			"fcvt.pwu.ps f4, f29, rne\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f21, 448(x9)\n"
			"fsin.ps f6, f21\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f13, 128(x11)\n"
			"flw.ps f0, 544(x11)\n"
			"fmin.ps f21, f13, f0\n" 
			"la x25, rand_int_32\n"
			"flw.ps f29, 640(x25)\n"
			"flw.ps f16, 96(x25)\n"
			"fltu.pi f27, f29, f16\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f4, 96(x18)\n"
			"flw.ps f22, 800(x18)\n"
			"fmin.s f4, f4, f22\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f17, 704(x9)\n"
			"flw.ps f21, 992(x9)\n"
			"flw.ps f16, 288(x9)\n"
			"fmadd.s f20, f17, f21, f16, rmm\n" 
			"fbci.pi f21, 0x1132e\n" 
			"la x12, rand_int_32\n"
			"flw.ps f29, 832(x12)\n"
			"flw.ps f10, 896(x12)\n"
			"fxor.pi f24, f29, f10\n" 
			"la x10, rand_int_32\n"
			"flw.ps f21, 640(x10)\n"
			"fsat8.pi f24, f21\n" 
			"la x11, rand_int_32\n"
			"flw.ps f20, 768(x11)\n"
			"flw.ps f4, 416(x11)\n"
			"fmaxu.pi f24, f20, f4\n" 
			"li x15, 0x7eb8558e02f5b843\n"
			"fcvt.s.w f13, x15, rup\n" 
			"la x18, rand_int_32\n"
			"flw.ps f10, 96(x18)\n"
			"flw.ps f12, 416(x18)\n"
			"fmaxu.pi f29, f10, f12\n" 
			"li x16, 0x6b3b6544262ccb5b\n"
			"fcvt.s.w f17, x16, rup\n" 
			"la x25, rand_int_32\n"
			"flw.ps f22, 128(x25)\n"
			"flw.ps f29, 832(x25)\n"
			"fmin.pi f16, f22, f29\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f24, 704(x18)\n"
			"flw.ps f6, 32(x18)\n"
			"fmul.ps f13, f24, f6, rdn\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_1_RDX_ID_1_SHIRE_0_HID_10:\n"
			"li x14, 0x120000000435000b\n"
			"csrw tensor_reduce, x14\n" 
			"csrwi tensor_wait, 9\n" 
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=0); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"LBL_SEQID_1_TLD_XMA_STR_SEQ_ITER_0_HID_10:\n"
			"la x31, self_check_8\n"
			"li x5, 0xc0000000000007\n"
			"add x5, x5, x31\n"
			"li x6, 0x1000000000000a\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x3 << TENSOR_IMA_B_NUM_COLS) | (0x7 << TENSOR_IMA_A_NUM_ROWS) | (0xa << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x1 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1e << TENSOR_IMA_SCP_LOC_B) | (0x6 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fmvz.x.ps x31, f0, 0\n"
			"li x5, 0x2c\n"
			"bne x5, x31, LBL_FAIL_HID_10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_10\n"
			"csrw tensor_error, x0\n"
			"li x5, 2\n" 
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
			"csrwi mhpmevent3, 6\n"   
			"csrwi mhpmevent4, 1\n"   
			"csrwi mhpmevent5, 3\n"   
			"csrwi mhpmevent6, 2\n"   
			"csrwi mhpmevent7, 23\n"   
			"csrwi mhpmevent8, 22\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f26,  864 (x5)\n"
			"flw.ps f1,  832 (x5)\n"
			"flw.ps f10,  96 (x5)\n"
			"flw.ps f22,  160 (x5)\n"
			"flw.ps f5,  608 (x5)\n"
			"flw.ps f2,  576 (x5)\n"
			"flw.ps f19,  0 (x5)\n"
			"flw.ps f6,  896 (x5)\n"
			"flw.ps f31,  128 (x5)\n"
			"flw.ps f29,  480 (x5)\n"
			"flw.ps f16,  928 (x5)\n"
			"flw.ps f0,  64 (x5)\n"
			"flw.ps f27,  800 (x5)\n"
			"flw.ps f17,  224 (x5)\n"
			"flw.ps f11,  672 (x5)\n"
			"flw.ps f13,  288 (x5)\n"
			"flw.ps f30,  640 (x5)\n"
			"flw.ps f8,  384 (x5)\n"
			"flw.ps f12,  256 (x5)\n"
			"flw.ps f3,  992 (x5)\n"
			"flw.ps f18,  448 (x5)\n"
			"flw.ps f15,  736 (x5)\n"
			"flw.ps f7,  352 (x5)\n"
			"flw.ps f4,  512 (x5)\n"
			"flw.ps f21,  704 (x5)\n"
			"flw.ps f20,  320 (x5)\n"
			"flw.ps f23,  768 (x5)\n"
			"flw.ps f24,  960 (x5)\n"
			"flw.ps f25,  192 (x5)\n"
			"flw.ps f14,  544 (x5)\n"
			"flw.ps f28,  32 (x5)\n"
			"flw.ps f9,  416 (x5)\n"
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
			"li x5, 0x2\n" 
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
			"la x5, rand_int_32\n"
			"flw.ps f30, 192(x5)\n"
			"flw.ps f13, 544(x5)\n"
			"fmulhu.pi f2, f30, f13\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f19, 448(x9)\n"
			"flw.ps f2, 992(x9)\n"
			"flw.ps f1, 0(x9)\n"
			"fnmsub.ps f30, f19, f2, f1, rdn\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f23, 928(x6)\n"
			"flw.ps f6, 352(x6)\n"
			"fle.s x14, f23, f6\n" 
			"la x28, rand_int_32\n"
			"flw.ps f17, 736(x28)\n"
			"flw.ps f8, 960(x28)\n"
			"fsrl.pi f18, f17, f8\n" 
			"la x6, rand_int_32\n"
			"flw.ps f29, 128(x6)\n"
			"fsrai.pi f11, f29, 0xa\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f18, 928(x9)\n"
			"fadd.ps f23, f18, f18, rup\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f11, 768(x22)\n"
			"fcvt.pw.ps f30, f11, rup\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f9, 512(x12)\n"
			"flw.ps f1, 672(x12)\n"
			"fadd.ps f29, f9, f1, rne\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f0, 320(x22)\n"
			"flw.ps f29, 992(x22)\n"
			"fmax.s f11, f0, f29\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f26, 864(x22)\n"
			"flw.ps f2, 960(x22)\n"
			"flw.ps f19, 640(x22)\n"
			"fmadd.s f2, f26, f2, f19, rne\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f19, 160(x26)\n"
			"flw.ps f6, 64(x26)\n"
			"fmadd.s f18, f19, f19, f6, rdn\n" 
			"la x12, rand_int_32\n"
			"flw.ps f13, 672(x12)\n"
			"fpackreph.pi f26, f13\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f0, 384(x18)\n"
			"flw.ps f26, 928(x18)\n"
			"fmin.ps f13, f0, f26\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_11:\n"
			"fltm.ps m4, f1, f26\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x26, rand_ieee754_32\n"
			"flw.ps f3, 192(x26)\n"
			"flw.ps f26, 480(x26)\n"
			"fsgnjn.ps f30, f3, f26\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f23, 96(x18)\n"
			"flw.ps f17, 928(x18)\n"
			"feq.ps f6, f23, f17\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_11\n"
			"addi x30, x30, 1\n"
			"la x18, rand_int_32\n"
			"flw.ps f23, 288(x18)\n"
			"flw.ps f29, 0(x18)\n"
			"fltu.pi f0, f23, f29\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f17, 0(x28)\n"
			"flw.ps f13, 864(x28)\n"
			"flw.ps f9, 320(x28)\n"
			"fcmov.ps f26, f17, f13, f9\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f3, 640(x26)\n"
			"fclass.ps f13, f3\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f26, 448(x6)\n"
			"flw.ps f1, 320(x6)\n"
			"flw.ps f0, 96(x6)\n"
			"fnmsub.ps f1, f26, f1, f0, rne\n" 
			"la x22, rand_int_32\n"
			"flw.ps f18, 832(x22)\n"
			"flw.ps f0, 96(x22)\n"
			"fmax.pi f19, f18, f0\n" 
			"la x26, rand_int_32\n"
			"flw.ps f1, 544(x26)\n"
			"flw.ps f30, 576(x26)\n"
			"fmulh.pi f26, f1, f30\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f0, 512(x12)\n"
			"flt.ps f11, f0, f0\n" 
			"la x12, rand_int_32\n"
			"flw.ps f1, 864(x12)\n"
			"feq.pi f18, f1, f1\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f17, 672(x11)\n"
			"fcvt.pw.ps f9, f17, rmm\n" 
			"LBL_SEQID_1_FP_TRANS_ID_1_HID_11:\n"
			"la x14, rand_ieee754_32\n"
			"flw.ps f0, 608(x14)\n"
			"flog.ps f26, f0\n" 
			"la x14, rand_int_32\n"
			"flw.ps f8, 928(x14)\n"
			"flw.ps f17, 896(x14)\n"
			"fxor.pi f18, f8, f17\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f26, 224(x12)\n"
			"flw.ps f11, 736(x12)\n"
			"fmin.ps f11, f26, f11\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f26, 160(x14)\n"
			"fcvt.f16.ps f8, f26\n" 
			"la x9, rand_int_32\n"
			"flw.ps f1, 416(x9)\n"
			"flw.ps f30, 128(x9)\n"
			"flt.pi f9, f1, f30\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f11, 512(x11)\n"
			"flw.ps f1, 896(x11)\n"
			"feq.s x12, f11, f1\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f30, 992(x5)\n"
			"fcvt.w.s x14, f30, rtz\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 2\n" 
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
			"csrwi mhpmevent3, 4\n"   
			"csrwi mhpmevent4, 8\n"   
			"csrwi mhpmevent5, 18\n"   
			"csrwi mhpmevent6, 21\n"   
			"csrwi mhpmevent7, 4\n"   
			"csrwi mhpmevent8, 3\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f5,  512 (x5)\n"
			"flw.ps f2,  640 (x5)\n"
			"flw.ps f1,  704 (x5)\n"
			"flw.ps f13,  672 (x5)\n"
			"flw.ps f20,  448 (x5)\n"
			"flw.ps f3,  320 (x5)\n"
			"flw.ps f15,  992 (x5)\n"
			"flw.ps f27,  384 (x5)\n"
			"flw.ps f30,  288 (x5)\n"
			"flw.ps f18,  128 (x5)\n"
			"flw.ps f12,  864 (x5)\n"
			"flw.ps f17,  576 (x5)\n"
			"flw.ps f10,  480 (x5)\n"
			"flw.ps f9,  160 (x5)\n"
			"flw.ps f31,  192 (x5)\n"
			"flw.ps f8,  0 (x5)\n"
			"flw.ps f16,  224 (x5)\n"
			"flw.ps f22,  768 (x5)\n"
			"flw.ps f29,  32 (x5)\n"
			"flw.ps f21,  608 (x5)\n"
			"flw.ps f26,  96 (x5)\n"
			"flw.ps f25,  928 (x5)\n"
			"flw.ps f0,  416 (x5)\n"
			"flw.ps f6,  736 (x5)\n"
			"flw.ps f7,  352 (x5)\n"
			"flw.ps f23,  896 (x5)\n"
			"flw.ps f14,  960 (x5)\n"
			"flw.ps f28,  544 (x5)\n"
			"flw.ps f11,  800 (x5)\n"
			"flw.ps f24,  64 (x5)\n"
			"flw.ps f4,  256 (x5)\n"
			"flw.ps f19,  832 (x5)\n"
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
			"li x5, 0x2\n" 
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
			"la x9, rand_ieee754_32\n"
			"flw.ps f25, 416(x9)\n"
			"flw.ps f22, 576(x9)\n"
			"flw.ps f29, 896(x9)\n"
			"fcmov.ps f24, f25, f22, f29\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f7, 896(x7)\n"
			"flw.ps f1, 320(x7)\n"
			"feq.s x7, f7, f1\n" 
			"LBL_SEQID_0_FP_TRANS_ID_0_HID_12:\n"
			"la x22, rand_ieee754_32\n"
			"flw.ps f24, 736(x22)\n"
			"fexp.ps f22, f24\n" 
			"maskpopcz x7, m2\n" 
			"maskpopc x17, m4\n" 
			"la x17, rand_int_32\n"
			"flw.ps f20, 992(x17)\n"
			"fcvt.ps.pw f14, f20, rtz\n" 
			"LBL_SEQID_0_FP_TRANS_ID_1_HID_12:\n"
			"la x27, rand_ieee754_32\n"
			"flw.ps f18, 480(x27)\n"
			"frcp.ps f29, f18\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f29, 768(x5)\n"
			"fcvt.w.s x7, f29, rtz\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x5, rand_int_32\n"
			"flw.ps f13, 224(x5)\n"
			"fpackreph.pi f1, f13\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f28, 736(x11)\n"
			"fswizz.ps f15, f28, 0x11\n" 
			"LBL_SEQID_0_M0_WRITE_ID_2_HID_12:\n"
			"feqm.ps m4, f24, f25\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x9, rand_ieee754_32\n"
			"flw.ps f31, 992(x9)\n"
			"flw.ps f13, 736(x9)\n"
			"fadd.s f28, f31, f13, rtz\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f5, 736(x22)\n"
			"fswizz.ps f24, f5, 0x17\n" 
			"la x7, rand_int_32\n"
			"flw.ps f29, 896(x7)\n"
			"flw.ps f31, 64(x7)\n"
			"fxor.pi f31, f29, f31\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f22, 96(x9)\n"
			"flw.ps f24, 96(x9)\n"
			"feq.s x7, f22, f24\n" 
			"li x5, 0xcbb5740e70d00f02\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0x2b119a42671fd221\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"la x17, rand_int_32\n"
			"flw.ps f17, 800(x17)\n"
			"flw.ps f1, 352(x17)\n"
			"feq.pi f25, f17, f1\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ID_390160_HID_12:\n"
			"li x6, ((0x2 << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0x3 << TENSOR_QUANT_QUANT_ROW) | (0x2a << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0xa << TENSOR_QUANT_TRANSF4) | (0x5 << TENSOR_QUANT_TRANSF3) | (0x9 << TENSOR_QUANT_TRANSF2) | (0x4 << TENSOR_QUANT_TRANSF1) | (0x5 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ITER_0_HID_12:\n"
			"li x5, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x0 << TENSOR_FMA16_B_NUM_COLS) | (0x5 << TENSOR_FMA16_A_NUM_ROWS) | (0x0 << TENSOR_FMA16_A_NUM_COLS) | (0xb << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0xa << TENSOR_FMA16_SCP_LOC_B) | (0xf7 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_12\n"
			"csrw tensor_error, x0\n"
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_12\n"
			"addi x30, x30, 1\n"
			"la x27, rand_int_32\n"
			"flw.ps f17, 64(x27)\n"
			"flw.ps f1, 32(x27)\n"
			"for.pi f8, f17, f1\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f5, 384(x7)\n"
			"fcvt.pw.ps f17, f5, rdn\n" 
			"la x7, rand_int_32\n"
			"flw.ps f14, 32(x7)\n"
			"flw.ps f31, 192(x7)\n"
			"fmulhu.pi f28, f14, f31\n" 
			"LBL_SEQID_1_FP_TRANS_ID_3_HID_12:\n"
			"la x9, rand_ieee754_32\n"
			"flw.ps f8, 64(x9)\n"
			"flog.ps f18, f8\n" 
			"la x17, rand_int_32\n"
			"flw.ps f22, 800(x17)\n"
			"fnot.pi f22, f22\n" 
			"la x11, rand_int_32\n"
			"flw.ps f20, 864(x11)\n"
			"flw.ps f22, 512(x11)\n"
			"feq.pi f24, f20, f22\n" 
			"la x18, rand_int_32\n"
			"flw.ps f15, 128(x18)\n"
			"flw.ps f31, 544(x18)\n"
			"fand.pi f24, f15, f31\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f5, 224(x17)\n"
			"flw.ps f8, 64(x17)\n"
			"fsgnj.s f28, f5, f8\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f29, 512(x5)\n"
			"flw.ps f14, 448(x5)\n"
			"fsgnjn.ps f13, f29, f14\n" 
			"la x27, rand_int_32\n"
			"flw.ps f14, 768(x27)\n"
			"flw.ps f8, 32(x27)\n"
			"fremu.pi f18, f14, f8\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f13, 736(x19)\n"
			"flw.ps f18, 672(x19)\n"
			"flw.ps f28, 352(x19)\n"
			"fnmadd.ps f5, f13, f18, f28, rtz\n" 
			"la x13, rand_int_32\n"
			"flw.ps f1, 544(x13)\n"
			"flw.ps f13, 480(x13)\n"
			"fsub.pi f5, f1, f13\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f29, 416(x22)\n"
			"flw.ps f7, 256(x22)\n"
			"feq.ps f22, f29, f7\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f8, 896(x5)\n"
			"flw.ps f28, 416(x5)\n"
			"flw.ps f29, 224(x5)\n"
			"fmsub.s f17, f8, f28, f29, rdn\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x7, rand_int_32\n"
			"flw.ps f18, 320(x7)\n"
			"fpackreph.pi f29, f18\n" 
			"li x5, 0x4988018872d99402\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0x37492372186331ec\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"la x9, rand_int_32\n"
			"flw.ps f1, 64(x9)\n"
			"flw.ps f8, 992(x9)\n"
			"fmulhu.pi f31, f1, f8\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_1_RDX_ID_1_SHIRE_0_HID_12:\n"
			"li x18, 0x120000000435000b\n"
			"csrw tensor_reduce, x18\n" 
			"csrwi tensor_wait, 9\n" 
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=0); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"LBL_SEQID_1_TLD_XMA_STR_SEQ_ITER_0_HID_12:\n"
			"la x31, self_check_8\n"
			"li x5, 0xa0000000000003\n"
			"add x5, x5, x31\n"
			"li x6, 0x1000000000000b\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x0 << TENSOR_IMA_B_NUM_COLS) | (0x3 << TENSOR_IMA_A_NUM_ROWS) | (0xb << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x1 << TENSOR_IMA_TEN_B_IN_MEM) | (0x18 << TENSOR_IMA_SCP_LOC_B) | (0x5 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x9, ((0xb << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0x3 << TENSOR_QUANT_QUANT_ROW) | (0x2b << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x9 << TENSOR_QUANT_TRANSF1) | (0x8 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
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
			"li x5, 2\n" 
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
			"csrwi mhpmevent3, 6\n"   
			"csrwi mhpmevent4, 1\n"   
			"csrwi mhpmevent5, 3\n"   
			"csrwi mhpmevent6, 2\n"   
			"csrwi mhpmevent7, 23\n"   
			"csrwi mhpmevent8, 22\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f24,  384 (x5)\n"
			"flw.ps f3,  448 (x5)\n"
			"flw.ps f17,  192 (x5)\n"
			"flw.ps f30,  224 (x5)\n"
			"flw.ps f0,  416 (x5)\n"
			"flw.ps f22,  928 (x5)\n"
			"flw.ps f13,  544 (x5)\n"
			"flw.ps f15,  160 (x5)\n"
			"flw.ps f21,  608 (x5)\n"
			"flw.ps f1,  352 (x5)\n"
			"flw.ps f4,  992 (x5)\n"
			"flw.ps f11,  704 (x5)\n"
			"flw.ps f5,  256 (x5)\n"
			"flw.ps f12,  896 (x5)\n"
			"flw.ps f27,  864 (x5)\n"
			"flw.ps f31,  128 (x5)\n"
			"flw.ps f9,  736 (x5)\n"
			"flw.ps f28,  800 (x5)\n"
			"flw.ps f8,  640 (x5)\n"
			"flw.ps f14,  960 (x5)\n"
			"flw.ps f18,  320 (x5)\n"
			"flw.ps f2,  576 (x5)\n"
			"flw.ps f23,  672 (x5)\n"
			"flw.ps f10,  512 (x5)\n"
			"flw.ps f25,  96 (x5)\n"
			"flw.ps f29,  480 (x5)\n"
			"flw.ps f7,  288 (x5)\n"
			"flw.ps f6,  32 (x5)\n"
			"flw.ps f26,  64 (x5)\n"
			"flw.ps f16,  0 (x5)\n"
			"flw.ps f19,  768 (x5)\n"
			"flw.ps f20,  832 (x5)\n"
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
			"li x5, 0x2\n" 
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
			"la x23, rand_ieee754_32\n"
			"flw.ps f2, 64(x23)\n"
			"fcvt.pwu.ps f22, f2, rdn\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f3, 160(x27)\n"
			"flw.ps f27, 672(x27)\n"
			"fcmov.ps f19, f3, f27, f3\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f2, 256(x25)\n"
			"fcvt.pwu.ps f15, f2, rmm\n" 
			"maskpopc x25, m0\n" 
			"mov.m.x m0, x0, 0xff\n"
			"la x27, rand_ieee754_32\n"
			"flw.ps f3, 608(x27)\n"
			"flw.ps f27, 736(x27)\n"
			"fsub.s f22, f3, f27, rmm\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f23, 320(x20)\n"
			"flw.ps f2, 960(x20)\n"
			"fnmadd.ps f8, f23, f2, f23, rtz\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_13:\n"
			"masknot m0, m7\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x13, rand_ieee754_32\n"
			"flw.ps f19, 320(x13)\n"
			"flw.ps f16, 256(x13)\n"
			"fsub.s f15, f19, f16, rtz\n" 
			"li x17, 0x4dbd7fcdc2479352\n"
			"fcvt.s.wu f20, x17, rdn\n" 
			"la x27, rand_int_32\n"
			"flw.ps f2, 608(x27)\n"
			"fsrli.pi f3, f2, 0x3\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f7, 544(x17)\n"
			"flw.ps f20, 416(x17)\n"
			"fsub.ps f14, f7, f20, rtz\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f8, 288(x12)\n"
			"fswizz.ps f15, f8, 0x1c\n" 
			"la x18, rand_int_32\n"
			"flw.ps f17, 448(x18)\n"
			"flw.ps f14, 736(x18)\n"
			"fxor.pi f16, f17, f14\n" 
			"LBL_SEQID_0_FP_TRANS_ID_1_HID_13:\n"
			"la x13, rand_ieee754_32\n"
			"flw.ps f7, 448(x13)\n"
			"flog.ps f22, f7\n" 
			"la x7, rand_int_32\n"
			"flw.ps f7, 224(x7)\n"
			"flw.ps f15, 832(x7)\n"
			"fmulhu.pi f3, f7, f15\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f14, 128(x27)\n"
			"fcvt.l.s x25, f14, rup\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_13\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_1_M0_WRITE_ID_2_HID_13:\n"
			"fltm.ps m7, f2, f22\n" 
			"li x5, CREDINC1\n"
			"ld x5, 0(x5)\n"
			"la x18, rand_ieee754_32\n"
			"flw.ps f17, 512(x18)\n"
			"flw.ps f15, 32(x18)\n"
			"flw.ps f11, 448(x18)\n"
			"fmadd.ps f2, f17, f15, f11, rmm\n" 
			"la x7, rand_int_32\n"
			"flw.ps f2, 800(x7)\n"
			"flw.ps f0, 576(x7)\n"
			"fxor.pi f16, f2, f0\n" 
			"la x27, rand_int_32\n"
			"flw.ps f3, 640(x27)\n"
			"flw.ps f16, 672(x27)\n"
			"fadd.pi f6, f3, f16\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f3, 800(x25)\n"
			"fcvt.w.s x27, f3, rdn\n" 
			"la x10, rand_int_32\n"
			"flw.ps f0, 896(x10)\n"
			"fsatu8.pi f23, f0\n" 
			"la x20, rand_int_32\n"
			"flw.ps f22, 416(x20)\n"
			"flw.ps f2, 896(x20)\n"
			"for.pi f19, f22, f2\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f17, 128(x18)\n"
			"flw.ps f0, 960(x18)\n"
			"flw.ps f20, 448(x18)\n"
			"fmsub.ps f20, f17, f0, f20, rdn\n" 
			"mova.x.m x20\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f17, 576(x23)\n"
			"flw.ps f20, 704(x23)\n"
			"fmin.ps f7, f17, f20\n" 
			"LBL_SEQID_1_M0_WRITE_ID_3_HID_13:\n"
			"maskand m0, m0, m5\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x12, rand_int_32\n"
			"flw.ps f6, 480(x12)\n"
			"flw.ps f23, 416(x12)\n"
			"fltu.pi f17, f6, f23\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f11, 416(x25)\n"
			"fcvt.pw.ps f6, f11, rtz\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f20, 0(x13)\n"
			"fcvt.wu.s x12, f20, rne\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f27, 160(x20)\n"
			"fswizz.ps f6, f27, 0x1d\n" 
			"la x17, rand_int_32\n"
			"flw.ps f2, 960(x17)\n"
			"fcvt.ps.pwu f20, f2, rne\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 2\n" 
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
			"csrwi mhpmevent3, 4\n"   
			"csrwi mhpmevent4, 8\n"   
			"csrwi mhpmevent5, 18\n"   
			"csrwi mhpmevent6, 21\n"   
			"csrwi mhpmevent7, 4\n"   
			"csrwi mhpmevent8, 3\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f29,  224 (x5)\n"
			"flw.ps f11,  384 (x5)\n"
			"flw.ps f5,  608 (x5)\n"
			"flw.ps f30,  128 (x5)\n"
			"flw.ps f13,  928 (x5)\n"
			"flw.ps f2,  192 (x5)\n"
			"flw.ps f14,  896 (x5)\n"
			"flw.ps f21,  800 (x5)\n"
			"flw.ps f17,  416 (x5)\n"
			"flw.ps f22,  544 (x5)\n"
			"flw.ps f27,  576 (x5)\n"
			"flw.ps f4,  640 (x5)\n"
			"flw.ps f25,  352 (x5)\n"
			"flw.ps f23,  960 (x5)\n"
			"flw.ps f18,  96 (x5)\n"
			"flw.ps f31,  832 (x5)\n"
			"flw.ps f10,  448 (x5)\n"
			"flw.ps f6,  672 (x5)\n"
			"flw.ps f15,  0 (x5)\n"
			"flw.ps f19,  768 (x5)\n"
			"flw.ps f1,  256 (x5)\n"
			"flw.ps f26,  704 (x5)\n"
			"flw.ps f8,  864 (x5)\n"
			"flw.ps f0,  320 (x5)\n"
			"flw.ps f3,  160 (x5)\n"
			"flw.ps f7,  992 (x5)\n"
			"flw.ps f28,  480 (x5)\n"
			"flw.ps f24,  512 (x5)\n"
			"flw.ps f16,  736 (x5)\n"
			"flw.ps f9,  64 (x5)\n"
			"flw.ps f12,  288 (x5)\n"
			"flw.ps f20,  32 (x5)\n"
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
			"li x5, 0x2\n" 
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
			"li x20, 0x7ddf1ee38ae4bbf9\n"
			"fcvt.s.lu f22, x20, rne\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f26, 640(x20)\n"
			"flw.ps f5, 320(x20)\n"
			"fsgnj.s f19, f26, f5\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f27, 448(x22)\n"
			"flw.ps f10, 288(x22)\n"
			"flw.ps f18, 800(x22)\n"
			"fmsub.s f26, f27, f10, f18, rdn\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f18, 128(x9)\n"
			"fround.ps f18, f18, rtz\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f22, 480(x5)\n"
			"flw.ps f24, 640(x5)\n"
			"fadd.ps f15, f22, f24, rmm\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f17, 448(x20)\n"
			"flw.ps f24, 576(x20)\n"
			"fle.s x17, f17, f24\n" 
			"la x26, rand_int_32\n"
			"flw.ps f27, 608(x26)\n"
			"fnot.pi f22, f27\n" 
			"li x5, 0x11b7\n"
			"csrw tensor_mask, x5\n"	
			"maskpopcz x10, m3\n" 
			"la x17, rand_int_32\n"
			"flw.ps f22, 992(x17)\n"
			"flw.ps f0, 128(x17)\n"
			"fmin.pi f26, f22, f0\n" 
			"li x5, 0x883e\n"
			"csrw tensor_mask, x5\n"	
			"la x19, rand_int_32\n"
			"flw.ps f17, 320(x19)\n"
			"frem.pi f26, f17, f17\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_14:\n"
			"masknot m3, m6\n" 
			"li x5, CREDINC3\n"
			"ld x5, 0(x5)\n"
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_14:\n"
			"maskxor m6, m6, m0\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0xd9ea460dc5\n"
			"fsw.ps f0, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x20, rand_int_32\n"
			"flw.ps f17, 320(x20)\n"
			"flw.ps f18, 128(x20)\n"
			"fsrl.pi f18, f17, f18\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f24, 480(x9)\n"
			"flw.ps f22, 384(x9)\n"
			"flw.ps f14, 608(x9)\n"
			"fmadd.ps f21, f24, f22, f14, rdn\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f14, 960(x5)\n"
			"flw.ps f24, 896(x5)\n"
			"flw.ps f5, 576(x5)\n"
			"fmsub.s f2, f14, f24, f5, rne\n" 
			"LBL_SEQID_0_FP_TRANS_ID_2_HID_14:\n"
			"la x27, rand_ieee754_32\n"
			"flw.ps f10, 224(x27)\n"
			"flog.ps f21, f10\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 0x92d4d587244609d0\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0xd8ea30cc87e524af\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ID_276748_HID_14:\n"
			"la x31, self_check_16\n"
			"li x5, 0xa0000000000003\n"
			"add x5, x5, x31\n"
			"li x6, 0x1000000000000d\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x1 << TENSOR_FMA16_B_NUM_COLS) | (0x3 << TENSOR_FMA16_A_NUM_ROWS) | (0xd << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x1 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1b << TENSOR_FMA16_SCP_LOC_B) | (0x5 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0x1c\n"
			"bne x5, x31, LBL_FAIL_HID_14\n"
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ID_675887_HID_14:\n"
			"la x31, self_check_8\n"
			"li x5, 0x60000000000008\n"
			"add x5, x5, x31\n"
			"li x6, 0x10000000000000\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x2 << TENSOR_IMA_B_NUM_COLS) | (0x8 << TENSOR_IMA_A_NUM_ROWS) | (0x0 << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x1 << TENSOR_IMA_TEN_B_IN_MEM) | (0x19 << TENSOR_IMA_SCP_LOC_B) | (0x3 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fmvz.x.ps x31, f0, 0\n"
			"li x5, 0x4\n"
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
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ITER_0_HID_14:\n"
			"la x31, self_check_32\n"
			"li x5, 0xa0000000000000\n"
			"add x5, x5, x31\n"
			"li x6, 0x3c000000000000b\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x0 << TENSOR_FMA32_B_NUM_COLS) | (0x0 << TENSOR_FMA32_A_NUM_ROWS) | (0xb << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1e << TENSOR_FMA32_SCP_LOC_B) | (0x5 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
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
			"bne x5, x31, LBL_FAIL_HID_14\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_14\n"
			"csrw tensor_error, x0\n"
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_14\n"
			"addi x30, x30, 1\n"
			"la x20, rand_int_32\n"
			"flw.ps f15, 256(x20)\n"
			"flt.pi f10, f15, f15\n" 
			"li x5, 0xd852\n"
			"csrw tensor_mask, x5\n"	
			"la x26, rand_ieee754_32\n"
			"flw.ps f27, 288(x26)\n"
			"fswizz.ps f15, f27, 0x6\n" 
			"li x28, 0x72fe3d2cb1406541\n"
			"fcvt.s.l f10, x28, rtz\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f14, 480(x28)\n"
			"flw.ps f15, 0(x28)\n"
			"fmax.s f24, f14, f15\n" 
			"maskpopc x20, m3\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f15, 0(x5)\n"
			"flw.ps f13, 704(x5)\n"
			"fmadd.s f21, f15, f13, f13, rdn\n" 
			"la x17, rand_int_32\n"
			"flw.ps f24, 384(x17)\n"
			"flw.ps f2, 64(x17)\n"
			"for.pi f13, f24, f2\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x26, rand_int_32\n"
			"flw.ps f27, 800(x26)\n"
			"fpackreph.pi f21, f27\n" 
			"la x22, rand_int_32\n"
			"flw.ps f24, 288(x22)\n"
			"fsll.pi f12, f24, f24\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f19, 160(x7)\n"
			"flw.ps f27, 256(x7)\n"
			"flt.ps f26, f19, f27\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"mova.x.m x22\n" 
			"LBL_SEQID_1_M0_WRITE_ID_3_HID_14:\n"
			"masknot m6, m6\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x27, rand_ieee754_32\n"
			"flw.ps f12, 736(x27)\n"
			"fround.ps f13, f12, rne\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f17, 0(x7)\n"
			"fclass.s x7, f17\n" 
			"maskpopcz x20, m0\n" 
			"mov.m.x m0, x0, 0xff\n"
			"la x10, rand_ieee754_32\n"
			"flw.ps f2, 256(x10)\n"
			"frsq.ps f14, f2\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_1_RDX_ID_1_SHIRE_0_HID_14:\n"
			"li x28, 0x120000000435000b\n"
			"csrw tensor_reduce, x28\n" 
			"csrwi tensor_wait, 9\n" 
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=0); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ITER_0_HID_14:\n"
			"li x5, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x2 << TENSOR_FMA16_B_NUM_COLS) | (0x8 << TENSOR_FMA16_A_NUM_ROWS) | (0x3 << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0xba << TENSOR_FMA16_SCP_LOC_B) | (0x9 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_14\n"
			"csrw tensor_error, x0\n"
			"li x5, 2\n" 
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
			"csrwi mhpmevent3, 6\n"   
			"csrwi mhpmevent4, 1\n"   
			"csrwi mhpmevent5, 3\n"   
			"csrwi mhpmevent6, 2\n"   
			"csrwi mhpmevent7, 23\n"   
			"csrwi mhpmevent8, 22\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f6,  192 (x5)\n"
			"flw.ps f18,  64 (x5)\n"
			"flw.ps f30,  0 (x5)\n"
			"flw.ps f26,  128 (x5)\n"
			"flw.ps f11,  576 (x5)\n"
			"flw.ps f16,  96 (x5)\n"
			"flw.ps f31,  256 (x5)\n"
			"flw.ps f1,  512 (x5)\n"
			"flw.ps f23,  960 (x5)\n"
			"flw.ps f14,  640 (x5)\n"
			"flw.ps f12,  864 (x5)\n"
			"flw.ps f10,  992 (x5)\n"
			"flw.ps f7,  480 (x5)\n"
			"flw.ps f27,  416 (x5)\n"
			"flw.ps f9,  224 (x5)\n"
			"flw.ps f24,  448 (x5)\n"
			"flw.ps f8,  320 (x5)\n"
			"flw.ps f15,  160 (x5)\n"
			"flw.ps f28,  544 (x5)\n"
			"flw.ps f2,  32 (x5)\n"
			"flw.ps f0,  288 (x5)\n"
			"flw.ps f29,  928 (x5)\n"
			"flw.ps f19,  672 (x5)\n"
			"flw.ps f21,  768 (x5)\n"
			"flw.ps f4,  608 (x5)\n"
			"flw.ps f25,  352 (x5)\n"
			"flw.ps f3,  800 (x5)\n"
			"flw.ps f17,  896 (x5)\n"
			"flw.ps f20,  832 (x5)\n"
			"flw.ps f5,  384 (x5)\n"
			"flw.ps f22,  736 (x5)\n"
			"flw.ps f13,  704 (x5)\n"
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
			"li x5, 0x2\n" 
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
			"la x6, rand_ieee754_32\n"
			"flw.ps f28, 352(x6)\n"
			"flw.ps f18, 320(x6)\n"
			"flw.ps f26, 736(x6)\n"
			"fmsub.s f26, f28, f18, f26, rtz\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f18, 32(x25)\n"
			"flw.ps f29, 320(x25)\n"
			"flw.ps f12, 128(x25)\n"
			"fmadd.s f18, f18, f29, f12, rne\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f27, 608(x15)\n"
			"flw.ps f10, 704(x15)\n"
			"feq.ps f25, f27, f10\n" 
			"LBL_SEQID_0_FP_TRANS_ID_0_HID_15:\n"
			"la x12, rand_ieee754_32\n"
			"flw.ps f5, 480(x12)\n"
			"flog.ps f26, f5\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f1, 800(x17)\n"
			"fclass.s x12, f1\n" 
			"la x13, rand_int_32\n"
			"flw.ps f29, 416(x13)\n"
			"fpackrepb.pi f7, f29\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f5, 480(x28)\n"
			"frsq.ps f1, f5\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f18, 736(x13)\n"
			"fcvt.l.s x26, f18, rne\n" 
			"la x28, rand_int_32\n"
			"flw.ps f25, 224(x28)\n"
			"flw.ps f1, 608(x28)\n"
			"for.pi f29, f25, f1\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f3, 768(x5)\n"
			"flw.ps f7, 416(x5)\n"
			"fle.s x17, f3, f7\n" 
			"la x17, rand_int_32\n"
			"flw.ps f12, 384(x17)\n"
			"flw.ps f29, 928(x17)\n"
			"fmax.pi f10, f12, f29\n" 
			"la x28, rand_int_32\n"
			"flw.ps f0, 288(x28)\n"
			"fslli.pi f0, f0, 0x5\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f7, 320(x28)\n"
			"flw.ps f5, 960(x28)\n"
			"fle.ps f7, f7, f5\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f29, 544(x6)\n"
			"fle.ps f7, f29, f29\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f16, 64(x6)\n"
			"flw.ps f27, 128(x6)\n"
			"fmin.s f0, f16, f27\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f28, 480(x13)\n"
			"flw.ps f27, 864(x13)\n"
			"flw.ps f12, 544(x13)\n"
			"fmsub.s f5, f28, f27, f12, rtz\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_15\n"
			"addi x30, x30, 1\n"
			"la x6, rand_ieee754_32\n"
			"flw.ps f28, 544(x6)\n"
			"fswizz.ps f31, f28, 0x18\n" 
			"LBL_SEQID_1_M0_WRITE_ID_1_HID_15:\n"
			"flem.ps m1, f5, f27\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0x3a731b0d21\n"
			"flw.ps f5, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x22, rand_ieee754_32\n"
			"flw.ps f3, 288(x22)\n"
			"fcvt.w.s x13, f3, rup\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f27, 160(x25)\n"
			"flw.ps f16, 928(x25)\n"
			"flw.ps f1, 320(x25)\n"
			"fmsub.ps f16, f27, f16, f1, rdn\n" 
			"la x17, rand_int_32\n"
			"flw.ps f10, 320(x17)\n"
			"fpackrepb.pi f7, f10\n" 
			"fbci.ps f7, 0x7d0b\n" 
			"fbci.pi f25, 0x34a21\n" 
			"la x17, rand_int_32\n"
			"flw.ps f1, 896(x17)\n"
			"flw.ps f3, 320(x17)\n"
			"flt.pi f18, f1, f3\n" 
			"LBL_SEQID_1_M0_WRITE_ID_2_HID_15:\n"
			"flem.ps m6, f12, f5\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_1_FP_TRANS_ID_3_HID_15:\n"
			"la x19, rand_ieee754_32\n"
			"flw.ps f0, 32(x19)\n"
			"flog.ps f25, f0\n" 
			"la x12, rand_int_32\n"
			"flw.ps f21, 160(x12)\n"
			"flt.pi f26, f21, f21\n" 
			"la x28, rand_int_32\n"
			"flw.ps f16, 896(x28)\n"
			"fcvt.ps.pwu f7, f16, rdn\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f12, 672(x12)\n"
			"flw.ps f31, 768(x12)\n"
			"fmin.ps f16, f12, f31\n" 
			"la x13, rand_int_32\n"
			"flw.ps f10, 800(x13)\n"
			"fpackreph.pi f10, f10\n" 
			"la x13, rand_int_32\n"
			"flw.ps f21, 928(x13)\n"
			"fsat8.pi f12, f21\n" 
			"la x28, rand_int_32\n"
			"flw.ps f26, 608(x28)\n"
			"flw.ps f27, 960(x28)\n"
			"fmin.pi f31, f26, f27\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 2\n" 
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
		0x3e5e3523, 
		0xba699080, 
		0xf869157a, 
		0x788788b4, 
		0x1f5be987, 
		0xd995fdd2, 
		0xdfc1a4f2, 
		0x5c3507b8, 
		0x29d808cb, 
		0x39d745af, 
		0xa0215369, 
		0xfe46cca8, 
		0x3652ec1e, 
		0x5b1b57e4, 
		0xbdd317b9, 
		0xb79eed4f, 
		0x0fa6c849, 
		0xbe6d8117, 
		0x632e18cd, 
		0x9a807723, 
		0x94f1f832, 
		0x0e664f27, 
		0x00c3f7fa, 
		0x46e6f893, 
		0x29cf1ea1, 
		0xdfc936ac, 
		0x3812e49f, 
		0xdb799a78, 
		0xe3749be0, 
		0x7831e043, 
		0x42bb5bcc, 
		0x7288d76a, 
		0x253de450, 
		0x1565fa97, 
		0xff4a0a18, 
		0x985200c4, 
		0x34b63d75, 
		0xddefd08b, 
		0xcbf1dabc, 
		0xb18e2337, 
		0x4a649803, 
		0x05d1bc2e, 
		0xe09c95e5, 
		0x726a3b92, 
		0x56ca22ba, 
		0x8080422f, 
		0x50f196a2, 
		0x08a3e0b7, 
		0x8313ae8a, 
		0xd327c1d5, 
		0xeaca7987, 
		0xc0eb7360, 
		0xcdedd89a, 
		0x88b47352, 
		0x15560735, 
		0x3cf5b4db, 
		0xec9176ae, 
		0x21fe6116, 
		0x15b77173, 
		0x596a8454, 
		0x2b19330a, 
		0xc258e69c, 
		0xf110eb13, 
		0xe91c4336, 
		0x058207ee, 
		0xf6f0a090, 
		0x9b314576, 
		0xdefdba12, 
		0x0071e5a1, 
		0xbe10c337, 
		0xdffbbe73, 
		0xd18958af, 
		0xa71e442f, 
		0x81c0edc8, 
		0xe0ac8dc9, 
		0xd84f5b33, 
		0x63e3b033, 
		0x3fe51066, 
		0x2b5a0844, 
		0xa8d637dd, 
		0x34ac4880, 
		0x3b89dab8, 
		0x4d5314f4, 
		0x2c8e6f6c, 
		0x28ca469b, 
		0x644204ce, 
		0x221964a3, 
		0x0608bbdb, 
		0x5fb8e635, 
		0x742b126b, 
		0x8e22a161, 
		0x6a3fce0a, 
		0x087219aa, 
		0x628302a5, 
		0xbf4ee2fc, 
		0xc99717ac, 
		0xf532cf71, 
		0x7208248c, 
		0xf9548987, 
		0x0aff4b17, 
		0xd78ecf18, 
		0x60d408bd, 
		0x4f3475e6, 
		0x47bddb7a, 
		0xf630e4c5, 
		0x1c69035b, 
		0x72c2d38a, 
		0x657592ac, 
		0xb211f6c8, 
		0x4a2e3580, 
		0x4fd566e6, 
		0x00e77a85, 
		0xc75ebcfb, 
		0x0c186adc, 
		0xc2244c6a, 
		0xe2d495a7, 
		0x01d3b15c, 
		0xc7c69ded, 
		0x485b6bcc, 
		0x4d14d6d7, 
		0xce73f8f9, 
		0xe5c8ac15, 
		0x4e2bebc4, 
		0x8344a40a, 
		0x7ae94eff, 
		0x5151c64a, 
		0x45035b85, 
		0xbd431a39, 
		0x5b5b58f6, 
		0x036ee281, 
		0x7a107a82, 
		0x4baa6b58, 
		0x93f79e8c, 
		0x6f1a6a01, 
		0xc1ab042e, 
		0x77ace7b4, 
		0x9c57cc0e, 
		0x6a6823f9, 
		0xaa49f597, 
		0xf97563da, 
		0x6c0ed701, 
		0xe9ef2af4, 
		0x2d593489, 
		0x237badfd, 
		0x789e04b4, 
		0x1a6b9447, 
		0xc47b1020, 
		0xaddfd41a, 
		0x61d8275f, 
		0x1c501391, 
		0x6011a231, 
		0x06b63b83, 
		0x64929650, 
		0x5c5cd3af, 
		0x76f15edb, 
		0x0a711750, 
		0xf08f40eb, 
		0xdaa9385d, 
		0xf571753d, 
		0x22dccbd6, 
		0xbe45ac6b, 
		0xb2d82acb, 
		0x7653ffc9, 
		0xcd43c2a0, 
		0x8d1ae090, 
		0x78f90078, 
		0xcb5417c1, 
		0x355a673a, 
		0x396bee5b, 
		0x185fe15f, 
		0x8e7fcbbd, 
		0x16dcfea7, 
		0x054d9024, 
		0xa3bafe43, 
		0x774ffe4f, 
		0xdaa71d20, 
		0x1879b29c, 
		0xcb839baa, 
		0x960af2dd, 
		0xc5ca06d4, 
		0x6ed58944, 
		0x9ed7d0d9, 
		0x17ff9c64, 
		0x5f3b3a47, 
		0x44eb01ba, 
		0x1bca9fa8, 
		0xe93cb964, 
		0x30a52d26, 
		0x874bf488, 
		0x51aececf, 
		0x7c72da91, 
		0xcbca2d5f, 
		0x3c3056bf, 
		0xa2196657, 
		0xf6b86aac, 
		0x1f77cdce, 
		0x84fdf23a, 
		0x17794f6b, 
		0x4f8e7085, 
		0x22163320, 
		0x699559e7, 
		0x79bee483, 
		0x079e92b8, 
		0x5c3d4526, 
		0xf0559a1c, 
		0x569e15a1, 
		0x73cdf719, 
		0xeb1d5603, 
		0xcc871e49, 
		0xf2301a01, 
		0x25975d1c, 
		0x5f43996b, 
		0xde12b945, 
		0x63dc0349, 
		0x0b948b2d, 
		0xa1885e88, 
		0x436524c7, 
		0x89e976e2, 
		0x49cb3cf1, 
		0x24a62d75, 
		0x91dd551b, 
		0xa83b4f0c, 
		0x35c57c99, 
		0xca09f2a8, 
		0x24fb72a1, 
		0x2117cf65, 
		0x721f3aad, 
		0x9b85cdc6, 
		0x8f8e634c, 
		0x74e1985d, 
		0x8403b350, 
		0xc02b3b5d, 
		0x4a23b98b, 
		0x64975110, 
		0x0bada371, 
		0xb77ff2b9, 
		0xfd45af00, 
		0x32d88bec, 
		0xfda5bb20, 
		0x07b16831, 
		0x5d98a17e, 
		0x0734e1f2, 
		0x776f0276, 
		0x00f108fa, 
		0x6ae41248, 
		0xc19ca8ee, 
		0x44fb0091, 
		0x8730041b, 
		0xfff0b937, 
		0xb965034c, 
		0x81004ccb, 
		0xaa263b79, 
		0xa62b463e, 
		0x448fea24, 
		0x6012a14a, 
		0x013509f8  
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
		0xcc00, 
		0x0d1a, 
		0x4980, 
		0xd200, 
		0x3142, 
		0xd1c0, 
		0xd2a0, 
		0x5060, 
		0xc681, 
		0x4681, 
		0x78a6, 
		0xf86e, 
		0x51e0, 
		0x4f40, 
		0x50e0, 
		0x4400, 
		0x700f, 
		0x4200, 
		0x8000, 
		0x4980, 
		0x4e80, 
		0x95e7, 
		0xce40, 
		0x655e, 
		0xc800, 
		0x0000, 
		0x8000, 
		0xc880, 
		0xcec0, 
		0xcf63, 
		0x3879, 
		0xe006, 
		0xcd00, 
		0x434c, 
		0xcc40, 
		0x4f80, 
		0x59e1, 
		0x4500, 
		0xa2b8, 
		0xd380, 
		0x5340, 
		0xcdc0, 
		0xcfc0, 
		0xc0db, 
		0xcc00, 
		0x4d00, 
		0xd340, 
		0xca00, 
		0x4b80, 
		0xc400, 
		0xfdba, 
		0x5140, 
		0xcdc0, 
		0xd120, 
		0xbc28, 
		0x5080, 
		0x0000, 
		0xdc39, 
		0x4f40, 
		0xd2e0, 
		0x51e0, 
		0xcf40, 
		0xcd80, 
		0x4980, 
		0xce40, 
		0x0449, 
		0xfc00, 
		0x51a0, 
		0x047e, 
		0xd060, 
		0x5476, 
		0xf538, 
		0xd120, 
		0x0000, 
		0xcb00, 
		0x7c00, 
		0xd1e0, 
		0xc1ef, 
		0x0000, 
		0xf66d, 
		0xc900, 
		0xc2c5, 
		0x602b, 
		0x808c, 
		0x477f, 
		0x6cd2, 
		0x5080, 
		0x7c00, 
		0xb1d4, 
		0x53c0, 
		0x5040, 
		0xd1e0, 
		0x50c0, 
		0xd060, 
		0x8000, 
		0xd000, 
		0x4f40, 
		0x4d00, 
		0x5dc9, 
		0xdc36, 
		0x0000, 
		0x5360, 
		0x8000, 
		0xc500, 
		0xcc40, 
		0x51a0, 
		0x4f3d, 
		0xc200, 
		0x4dc0, 
		0x44de, 
		0x0000, 
		0xd0e0, 
		0xd0a0, 
		0x4880, 
		0x3c00, 
		0x1513, 
		0xca00, 
		0x7c00, 
		0x0000, 
		0x4e00, 
		0x6573, 
		0xd1c0, 
		0x5380, 
		0xd1e0, 
		0x4f40, 
		0xeeb7, 
		0x6aa7, 
		0x4b80, 
		0xc200, 
		0x4a80, 
		0x52e0, 
		0x845b, 
		0xfc00, 
		0x51c0, 
		0xf0ba, 
		0xd2a0, 
		0xd020, 
		0x0015, 
		0xcd40, 
		0x4880, 
		0x5320, 
		0x4cc0, 
		0x0000, 
		0xdd7c, 
		0xd1e0, 
		0xd140, 
		0x5080, 
		0x74c6, 
		0x0dd1, 
		0xc000, 
		0x5200, 
		0x6140, 
		0x256c, 
		0xc600, 
		0xcfc0, 
		0x82e5, 
		0xd180, 
		0x7ac5, 
		0x5dd2, 
		0x4d40, 
		0x3bdb, 
		0xa304, 
		0x5200, 
		0xd220, 
		0xc433, 
		0x4c00, 
		0x0758, 
		0x51a0, 
		0xd0c0, 
		0xd0c0, 
		0x7afd, 
		0x4500, 
		0x8000, 
		0xcf80, 
		0xb1c9, 
		0xfa67, 
		0xca80, 
		0xd100, 
		0x4600, 
		0xa670, 
		0x7ddc, 
		0x3056, 
		0xa425, 
		0xb99e, 
		0x51a0, 
		0xc767, 
		0x4e00, 
		0x3ee8, 
		0x50d1, 
		0x51e0, 
		0x5100, 
		0x5280, 
		0x5180, 
		0x50a0, 
		0x9ff5, 
		0xcc00, 
		0x4880, 
		0x5380, 
		0x2a55, 
		0xd380, 
		0x7c00, 
		0xd160, 
		0x0840, 
		0xc200, 
		0x948c, 
		0xd000, 
		0x5413, 
		0xd180, 
		0xd3a0, 
		0x5300, 
		0x53a0, 
		0xd140, 
		0xd2c0, 
		0x53a0, 
		0xa768, 
		0xaece, 
		0x50e0, 
		0xfc00, 
		0xbf5d, 
		0xcb00, 
		0x55ce, 
		0x4f00, 
		0xcec0, 
		0xfd49, 
		0xd1c0, 
		0x5e87, 
		0xd3a0, 
		0x9895, 
		0xfc80, 
		0x4c00, 
		0xb9dd, 
		0xe7a4, 
		0x8000, 
		0x6b36, 
		0xe0dd, 
		0xd100, 
		0xcf80, 
		0x46a1, 
		0xa3a7, 
		0x4e00, 
		0xc800, 
		0xbc00, 
		0xa4bd, 
		0x4980, 
		0x52e0, 
		0xcb80, 
		0xd1c0, 
		0x52e0, 
		0xeaa3, 
		0x6b83, 
		0x4900, 
		0x50a0, 
		0xb050, 
		0xd1e0, 
		0x18b2, 
		0x16cb, 
		0xce40, 
		0x5100, 
		0x4e40, 
		0xc600, 
		0x417c, 
		0xd200, 
		0xfc00, 
		0xd2a0, 
		0x2559, 
		0xd280, 
		0x53e0, 
		0xc400, 
		0x5300, 
		0xd88b, 
		0x53c0, 
		0x74b8, 
		0xccc0, 
		0xd434, 
		0xed32, 
		0xca00, 
		0xc900, 
		0x4b00, 
		0xae0e, 
		0xe010, 
		0x4880, 
		0x5340, 
		0xd3e0, 
		0x4584, 
		0x1b93, 
		0x0a30, 
		0x4ec0, 
		0x53f8, 
		0x5180, 
		0x51c0, 
		0x4b00, 
		0x3587, 
		0xcb00, 
		0x5000, 
		0xc400, 
		0x5060, 
		0x03ce, 
		0x4200, 
		0xd0c0, 
		0x4500, 
		0x4500, 
		0xc700, 
		0x5140, 
		0x5100, 
		0xd160, 
		0xc800, 
		0x4d00, 
		0xfc00, 
		0x4a80, 
		0x32ec, 
		0xc400, 
		0x4dc0, 
		0x7c00, 
		0x4d00, 
		0x38af, 
		0x453c, 
		0xd308, 
		0xc800, 
		0x5080, 
		0xd1c0, 
		0xb6cd, 
		0xd360, 
		0xcfc0, 
		0xe235, 
		0xc900, 
		0x8000, 
		0x7c00, 
		0xd300, 
		0xd260, 
		0x4f80, 
		0x4000, 
		0xcc80, 
		0x4500, 
		0x4980, 
		0x6676, 
		0xa8ee, 
		0x4c80, 
		0x7c00, 
		0x60d9, 
		0x4d00, 
		0xc800, 
		0xcc80, 
		0x4ec0, 
		0x5200, 
		0xce40, 
		0xd0e0, 
		0xcf80, 
		0x3333, 
		0x53e0, 
		0x4b80, 
		0xd180, 
		0xe72f, 
		0x8000, 
		0x5140, 
		0xa4e3, 
		0x2776, 
		0xd140, 
		0x9cc0, 
		0x5380, 
		0xcf80, 
		0xfc00, 
		0xd3c0, 
		0x51e0, 
		0x64ed, 
		0x36d3, 
		0xd0c0, 
		0xc700, 
		0xcd40, 
		0xcf80, 
		0xd380, 
		0x4274, 
		0x4880, 
		0xd2e0, 
		0xd120, 
		0x51c0, 
		0xc200, 
		0x5160, 
		0x4dc0, 
		0xd3c0, 
		0x5240, 
		0x4200, 
		0x4f80, 
		0x8217, 
		0xc200, 
		0xd140, 
		0xce40, 
		0x5120, 
		0x4c00, 
		0xd120, 
		0x0000, 
		0xcd40, 
		0x5100, 
		0x8000, 
		0xd200, 
		0xcd00, 
		0x327c, 
		0x52a0, 
		0x4600, 
		0xd240, 
		0x5080, 
		0xd220, 
		0xd220, 
		0xc614, 
		0x53e0, 
		0x5060, 
		0xcd40, 
		0xf878, 
		0x5340, 
		0x4a00, 
		0xd300, 
		0xd4de, 
		0x3c00, 
		0x5140, 
		0xfc00, 
		0x3a15, 
		0x8000, 
		0xcc80, 
		0x9572, 
		0xd2e0, 
		0x9a6b, 
		0x043f, 
		0x8000, 
		0xc880, 
		0xe8ca, 
		0x4c00, 
		0xfad8, 
		0xd2a0, 
		0xd3c0, 
		0x4b80, 
		0xd020, 
		0xd360, 
		0xfc26, 
		0x9b53, 
		0x5260, 
		0x7d1b, 
		0xca00, 
		0x4000, 
		0x8000, 
		0x51c0, 
		0xce00, 
		0x53e0, 
		0xce00, 
		0x5280, 
		0x3c00, 
		0x9e93, 
		0x4800, 
		0xd340, 
		0x4dc0, 
		0xc436, 
		0x4600, 
		0xc162, 
		0xd160, 
		0x4e00, 
		0x50c0, 
		0xc800, 
		0x39ab, 
		0xce40, 
		0x4c40, 
		0xd000, 
		0x53e0, 
		0xbc00, 
		0x0000, 
		0xc600, 
		0xbc00, 
		0xd060, 
		0x8000, 
		0xd240, 
		0xd120, 
		0x4700, 
		0x0000, 
		0xfd8e, 
		0xcf00, 
		0x50a0, 
		0x5200, 
		0xcc40, 
		0x5f5d, 
		0xd0e0, 
		0x7c00, 
		0xd200, 
		0xd2e0, 
		0xc800, 
		0xb7cd, 
		0xd200, 
		0x9f93, 
		0x5160, 
		0x0848, 
		0xd100, 
		0xcc00, 
		0x51a0, 
		0x4d40, 
		0x5220, 
		0x4c40, 
		0xc000, 
		0xd280, 
		0x4c00, 
		0x5360, 
		0x51c0, 
		0x5240, 
		0x1cd5, 
		0x4980, 
		0xcfc0, 
		0x017b, 
		0x8000, 
		0x4700, 
		0x5040, 
		0x0000, 
		0xd240, 
		0x50c0, 
		0x0000, 
		0xecd7, 
		0xcd80, 
		0x95e0, 
		0x41c8, 
		0xd140, 
		0x5761, 
		0xfc87, 
		0xd020, 
		0x4dc0, 
		0xc400, 
		0x5320, 
		0xd300, 
		0xfc00, 
		0x8000, 
		0xe90b, 
		0xfc00, 
		0x4b00, 
		0x6ffa, 
		0x50c0, 
		0xa10e, 
		0xd2c0, 
		0x5000, 
		0xcc80, 
		0xd080, 
		0xcc40, 
		0x5360, 
		0x80a4, 
		0x4f00, 
		0x3990, 
		0x8840, 
		0x4e40, 
		0x4ec0, 
		0x893f, 
		0xd1e0, 
		0xc900, 
		0x8000, 
		0x5160, 
		0x4d80, 
		0x20f0, 
		0x1588, 
		0x5806, 
		0x8085, 
		0x0000, 
		0x52e0, 
		0x5320, 
		0x4f00, 
		0xca00, 
		0xd300, 
		0x4dc0, 
		0xcd40, 
		0x4980, 
		0xcd80, 
		0x2ed0, 
		0x4d40, 
		0x50c0, 
		0x4500, 
		0x5260, 
		0x5040, 
		0xd1e0, 
		0x7d35, 
		0x5360, 
		0xcf80, 
		0x7c00, 
		0xaa15, 
		0x4f40, 
		0x6cbd, 
		0xd2a0, 
		0x9a5d, 
		0x4253, 
		0xd060, 
		0x4cc0, 
		0x0000, 
		0xce00, 
		0x6a1b, 
		0x4400, 
		0xd1c0, 
		0x8000, 
		0x4a00, 
		0x4880, 
		0xce40, 
		0x7a60, 
		0xb32a, 
		0xcb00, 
		0xcd00, 
		0xc700, 
		0xd000, 
		0x0000, 
		0xb471, 
		0xc800, 
		0x5300, 
		0xcc80, 
		0xd180, 
		0xfd6a, 
		0x5100, 
		0x8000, 
		0xcdc0, 
		0x4d00, 
		0x53a0, 
		0x50e0, 
		0xd060, 
		0xd0c0, 
		0xd040, 
		0xa4ed, 
		0x3dc2, 
		0xce00, 
		0xc800, 
		0xd220, 
		0xbad1, 
		0xc880, 
		0x5d86, 
		0x4d80, 
		0x4fc0, 
		0x4f00, 
		0xfc00, 
		0x5280, 
		0x6215, 
		0xd2c0, 
		0x4ec0, 
		0x8000, 
		0x5000, 
		0xcd80, 
		0x08ac, 
		0xd160, 
		0x8000, 
		0xc200, 
		0xdb1c, 
		0x4d40, 
		0x3c00, 
		0xd140, 
		0x4265, 
		0x795d, 
		0x5120, 
		0xd54f, 
		0x3c00, 
		0xcd00, 
		0x0000, 
		0xd000, 
		0x52c0, 
		0x1224, 
		0x4dc0, 
		0x4e00, 
		0x5080, 
		0x8212, 
		0x2661, 
		0xd320, 
		0x4ec0, 
		0x8000, 
		0xd220, 
		0xce40, 
		0xf314, 
		0xd280, 
		0x8057, 
		0xd000, 
		0xcb80, 
		0x5060, 
		0x03bd, 
		0xd260, 
		0xce00, 
		0xd160, 
		0xd140, 
		0x3c00, 
		0x4700, 
		0x308c, 
		0xcb80, 
		0xd260, 
		0xd1e0, 
		0x5200, 
		0x5320, 
		0x0487, 
		0xc800, 
		0x7dda, 
		0x7c2b, 
		0xd140, 
		0x4a00, 
		0x9492, 
		0xd240, 
		0xd0a0, 
		0x0bb1, 
		0x5140, 
		0x1aa9, 
		0x4ae9, 
		0x0000, 
		0x51e0, 
		0xfc00, 
		0xe8c9, 
		0xfc00, 
		0x0000, 
		0x537e, 
		0xbb4b, 
		0xd040, 
		0xd220, 
		0x51c0, 
		0xcd40, 
		0x7c9d, 
		0x7c00, 
		0x3450, 
		0xc200, 
		0x97f5, 
		0x4e80, 
		0xcc80, 
		0x4ec0, 
		0x9fad, 
		0x5160, 
		0x180c, 
		0xcec0, 
		0x7c10, 
		0xec4e, 
		0xc200, 
		0xcfc0, 
		0x74d4, 
		0x52c0, 
		0x4880, 
		0x5320, 
		0x51e0, 
		0x4dc0, 
		0x5320, 
		0xca00, 
		0x4e00, 
		0xc980, 
		0x51e0, 
		0x344f, 
		0xc200, 
		0x0000, 
		0xc980, 
		0x5220, 
		0xc880, 
		0x0000, 
		0x15d5, 
		0x5020, 
		0xfda0, 
		0x0c57, 
		0xc500, 
		0xc700, 
		0xba00, 
		0x5000, 
		0x45bb, 
		0xd2e0, 
		0x210d, 
		0x5000, 
		0x5320, 
		0x5699, 
		0xd3a0, 
		0x49d4, 
		0x53a0, 
		0x7c10, 
		0x4a80, 
		0xd2a0, 
		0x2a55, 
		0x0aa2, 
		0x2eb7, 
		0xd260, 
		0xd300, 
		0x8000, 
		0x5060, 
		0xd220, 
		0x8000, 
		0x0000, 
		0x4f80, 
		0xdf0e, 
		0x53c0, 
		0xf14e, 
		0xd30f, 
		0x8672, 
		0x5340, 
		0x4ec0, 
		0x5220, 
		0x4800, 
		0xd220, 
		0xd180, 
		0x49cc, 
		0x4000, 
		0xd080, 
		0x0005, 
		0xe6ce, 
		0x5240, 
		0x269a, 
		0x5dc8, 
		0xd120, 
		0xfc00, 
		0x37c5, 
		0x5220, 
		0x4ec0, 
		0xd1e0, 
		0xca80, 
		0x718a, 
		0xc880, 
		0xcc80, 
		0x230b, 
		0xd2e0, 
		0x7c00, 
		0x4c40, 
		0x4f80, 
		0xd3c0, 
		0xd140, 
		0x5120, 
		0x8000, 
		0x2b8a, 
		0xfc00, 
		0x5870, 
		0x8000, 
		0xf8cf, 
		0x19cb, 
		0x8000, 
		0x0000, 
		0x5020, 
		0xf949, 
		0xd3a0, 
		0x7c00, 
		0xcb80, 
		0x231d, 
		0xd100, 
		0x4b80, 
		0x7cea, 
		0xc800, 
		0x1269, 
		0xd320, 
		0x999e, 
		0xc700, 
		0xcf00, 
		0x4f80, 
		0xcc40, 
		0xe42b, 
		0x47d2, 
		0xd300, 
		0x2f2d, 
		0xbc87, 
		0x3f1e, 
		0xd1b9, 
		0xeb5e, 
		0xaa5b, 
		0xce80, 
		0x4c00, 
		0xcc80, 
		0x5340, 
		0x0211, 
		0xfc00, 
		0xd160, 
		0xd2e0, 
		0x5120, 
		0xd180, 
		0x0061, 
		0x76d7, 
		0x0000, 
		0xf15e, 
		0x24e7, 
		0x51e0, 
		0xfa49, 
		0x4000, 
		0x8000, 
		0xcfc0, 
		0xacaa, 
		0x0000, 
		0x5220, 
		0xd000, 
		0xcd40, 
		0x4c80, 
		0xd220, 
		0xcb00, 
		0x4200, 
		0x4f00, 
		0x8000, 
		0x5000, 
		0x5100, 
		0xb640, 
		0x7c00, 
		0x4b80, 
		0xd1a0, 
		0xccc0, 
		0xcb00, 
		0x8000, 
		0x4cc0, 
		0x50e0, 
		0x1058, 
		0x4f80, 
		0x8f97, 
		0x5300, 
		0xd160, 
		0xd240, 
		0x8000, 
		0x8000, 
		0x5060, 
		0x045b, 
		0x5380, 
		0xb24f, 
		0xc700, 
		0x4f00, 
		0x34b1, 
		0x8000, 
		0xc000, 
		0xc67b, 
		0x4f80, 
		0xc800, 
		0xd380, 
		0xc6ec, 
		0x81d2, 
		0xd1c0, 
		0x555b, 
		0xef50, 
		0x4e80, 
		0xabfb, 
		0xc400, 
		0xd260, 
		0x0000, 
		0x3abb, 
		0x2443, 
		0x5240, 
		0x5160, 
		0xd1e0, 
		0xfda7, 
		0x4fc0, 
		0xc000, 
		0x2e40, 
		0x743f, 
		0x52a0, 
		0x4400, 
		0xd360, 
		0xcf00, 
		0x3d41, 
		0x5d29, 
		0x0000, 
		0x52c0, 
		0xd000, 
		0x0000, 
		0xca80, 
		0x8000, 
		0xcd00, 
		0xca16, 
		0xa963, 
		0xcc40, 
		0xd2c0, 
		0x53a0, 
		0xcb00, 
		0x5fc1, 
		0xfc00, 
		0xd220, 
		0xcc40, 
		0x0219, 
		0xc880, 
		0x8efd, 
		0x8000, 
		0x1a52, 
		0x4f00, 
		0x4900, 
		0xd2e0, 
		0x288d, 
		0xfc00, 
		0x52c0, 
		0xbc00, 
		0x4a00, 
		0xcf80, 
		0xfc00, 
		0x7c00, 
		0x4a00, 
		0xd1c0, 
		0xf882, 
		0x3ee7, 
		0x7573, 
		0x0733, 
		0x4a00, 
		0x4ec0, 
		0xd160, 
		0x4f80, 
		0xa0d8, 
		0x6d0e, 
		0xcfc0, 
		0x4600, 
		0x5140, 
		0x32e8, 
		0xfc00, 
		0xc880, 
		0xdb7e, 
		0x60c5, 
		0x5120, 
		0x04f3, 
		0xea76, 
		0xd240, 
		0x8fc8, 
		0x6a67, 
		0x9205, 
		0x8000, 
		0xbed4, 
		0x3c00, 
		0x4600, 
		0xebd2, 
		0xcfc0, 
		0xc700, 
		0xcd80, 
		0x4c00, 
		0x4b80, 
		0xd1e0, 
		0xd2e0, 
		0x0000, 
		0xcd80, 
		0x4e00, 
		0xfe7c, 
		0x51e0, 
		0x0b21, 
		0xd060, 
		0x458a, 
		0x5040, 
		0x4a80, 
		0xd060, 
		0x7f72, 
		0x5220, 
		0x4a80, 
		0x5320, 
		0x0000, 
		0xcfc0, 
		0xfc00, 
		0x5de1, 
		0x88da, 
		0x50a0, 
		0x4600, 
		0x1e63, 
		0x5e42, 
		0x3c00, 
		0xd320, 
		0xcc40, 
		0x4980, 
		0x5100, 
		0x5000, 
		0x4d80, 
		0x5020, 
		0xa2ba, 
		0xcb00, 
		0x7163, 
		0xfc31, 
		0xcfc0, 
		0x51a0, 
		0x5140, 
		0x4ec0, 
		0x7dec, 
		0xd1c0, 
		0x5300, 
		0xe690, 
		0xcab4, 
		0x3c00, 
		0xfd59, 
		0xe321, 
		0x8000, 
		0xc880, 
		0xd140, 
		0x53c0, 
		0x4f40, 
		0x8a6e, 
		0xa5b6, 
		0x4c00, 
		0x0000, 
		0xcd40, 
		0xd380, 
		0x4340, 
		0x8e3c, 
		0xd3c0, 
		0xcf00, 
		0xee35, 
		0x53f7, 
		0x7626, 
		0xd040, 
		0x4c80, 
		0xd160, 
		0x8534, 
		0x4e7d, 
		0x4880, 
		0x3c00, 
		0x4e53, 
		0x383c, 
		0xd160, 
		0xcd00, 
		0xc880, 
		0x9b1e, 
		0xc1fd, 
		0x5467, 
		0x5000, 
		0xcd80, 
		0x52a0, 
		0x8000, 
		0xd180, 
		0x5140, 
		0xd200, 
		0xca80, 
		0x645e, 
		0x7c00, 
		0x86e0, 
		0x7c00, 
		0x50c0, 
		0xd3e0, 
		0xc500, 
		0x4b57, 
		0xd380, 
		0xcec0, 
		0x52c0, 
		0xc6da, 
		0x5240, 
		0x29be, 
		0x00ad, 
		0x0996, 
		0x294e, 
		0x4e00, 
		0xd360, 
		0x5a8e, 
		0xfc00, 
		0x044f, 
		0x2dcb, 
		0x5d17, 
		0x4980, 
		0x4200, 
		0xfc36, 
		0xce40, 
		0x4f00, 
		0x65d8, 
		0xa11f, 
		0x50a0, 
		0x52e0, 
		0x4c00, 
		0x497c, 
		0xd220, 
		0xbc00, 
		0xcce0, 
		0xb404, 
		0xc600, 
		0x7c00, 
		0x8778, 
		0x6f27, 
		0xe54b, 
		0xb7a4, 
		0x2b8d, 
		0x0000, 
		0xd320, 
		0xa8bd, 
		0xcb00, 
		0xd1e0, 
		0xd020, 
		0xd140, 
		0xcb00, 
		0x0b1f, 
		0x3c00, 
		0xd180, 
		0x9405, 
		0x4f00, 
		0x8000, 
		0x4500, 
		0xca80, 
		0x50e0, 
		0x9650, 
		0xdeb8, 
		0xaa21, 
		0xd320, 
		0x50e0, 
		0x0000, 
		0x4600, 
		0x0000, 
		0xfc00, 
		0xd040, 
		0x4dc0, 
		0x4dab, 
		0x044a, 
		0x0000, 
		0x5080, 
		0x4880, 
		0x5060, 
		0x8000, 
		0xc900, 
		0x4980, 
		0x51c0, 
		0xccc0, 
		0x46e3, 
		0x4c00, 
		0xd220, 
		0xf72a, 
		0x4700, 
		0x2bd8, 
		0xbdb4, 
		0xce00, 
		0x4c40, 
		0x6de9, 
		0x4700, 
		0x552c, 
		0x7c00, 
		0x4e00, 
		0xccc0, 
		0xc535, 
		0xd507, 
		0xc600, 
		0xd3c0, 
		0xfc83, 
		0xf999, 
		0x5517, 
		0x4c80, 
		0x46d7, 
		0x4a80, 
		0xcec0, 
		0xaa7e, 
		0x2591, 
		0xf75a, 
		0xbb96, 
		0x4c00, 
		0xd2a0, 
		0x5140, 
		0x4b0e, 
		0x9177, 
		0xd2e0, 
		0x8000, 
		0x20fc, 
		0x5300, 
		0x12f2, 
		0x52c0, 
		0xd0a0, 
		0x4ec0, 
		0xcc40, 
		0xcf40, 
		0x4d80, 
		0x4980, 
		0xbb6e, 
		0x7c00, 
		0x5240, 
		0x4fc0, 
		0x8000, 
		0xc600, 
		0x7036, 
		0xc880, 
		0xd000, 
		0xd300, 
		0x0000, 
		0x8076, 
		0xd2e0, 
		0xd1e0, 
		0x4f40, 
		0xd140, 
		0xd100, 
		0x4500, 
		0xc900, 
		0x4800, 
		0xc200, 
		0x52e0, 
		0x7dc4, 
		0x4f00, 
		0x4fc0, 
		0x7c38, 
		0xd2a0, 
		0xfc00, 
		0xa31e, 
		0x4400, 
		0xce00, 
		0x5160, 
		0xc200, 
		0x8000, 
		0xd240, 
		0xcd80, 
		0x4b80, 
		0x9dc1, 
		0xccc0, 
		0xfc40, 
		0x20ac, 
		0xcc00, 
		0x8000, 
		0x0cdf, 
		0x7c00, 
		0xc800, 
		0x53c0, 
		0xcef5, 
		0x0453, 
		0x4500, 
		0x8516, 
		0xc400, 
		0x4c40, 
		0xfd4e, 
		0xd1e0, 
		0x69ec, 
		0x4b00, 
		0x5120, 
		0x22f8, 
		0x0dba, 
		0xd340, 
		0x4200, 
		0xfc00, 
		0x4e40, 
		0xd2e0, 
		0x0000, 
		0xcd40, 
		0xd3a0, 
		0xe0ad, 
		0xd140, 
		0xfd7d, 
		0xfdf5, 
		0x7cda, 
		0x4ec0, 
		0x7cf3, 
		0x0000, 
		0xd120, 
		0xcb80, 
		0x8000, 
		0x5000, 
		0xfbae, 
		0x4a00, 
		0x51a0, 
		0x3bf7, 
		0x4000, 
		0xd340, 
		0x4500, 
		0x5020, 
		0x0000, 
		0x5240, 
		0x4d00, 
		0xa2e7, 
		0x29e9, 
		0xc880, 
		0xcfc0, 
		0xcb80, 
		0x4200, 
		0xd873, 
		0xd100, 
		0x17bc, 
		0xd0a0, 
		0x0000, 
		0x5040, 
		0xd3e0, 
		0x0000, 
		0xcade, 
		0x682a, 
		0x0000, 
		0x4900, 
		0x050f, 
		0x8000, 
		0x5160, 
		0x5340, 
		0xadf1, 
		0x853b, 
		0x4600, 
		0x5060, 
		0xce80, 
		0x5240, 
		0xfc00, 
		0x5e1c, 
		0xcd00, 
		0x1b59, 
		0x4000, 
		0x11ad, 
		0x8000, 
		0xeba4, 
		0x9b07, 
		0x0000, 
		0xd2c0, 
		0xe2b6, 
		0x6404, 
		0xfc00, 
		0x4f40, 
		0x843b, 
		0xd340, 
		0x6ec8, 
		0xccc0, 
		0x07fc, 
		0x2fbf, 
		0xcdfa, 
		0x5200, 
		0xcd80, 
		0x4a00, 
		0x4880, 
		0xcf80, 
		0x0000, 
		0xd3f8, 
		0xce00, 
		0xd380, 
		0xd2a0, 
		0x8000, 
		0x7c00, 
		0xcd40, 
		0xaccd, 
		0xd24d, 
		0xbe96, 
		0x8000, 
		0xd320, 
		0x5380, 
		0xd040, 
		0x4500, 
		0xd1e0, 
		0x114d, 
		0xbc00, 
		0x8000, 
		0x7c00, 
		0xbc00, 
		0xc900, 
		0xa374, 
		0x51c0, 
		0xca80, 
		0x8000, 
		0xfc00, 
		0xc397, 
		0x63c1, 
		0x5131, 
		0xd2c0, 
		0x53c0, 
		0xe7e4, 
		0x8000, 
		0x4e40, 
		0xcc00, 
		0x23be, 
		0x7d31, 
		0xcdc0, 
		0x024e, 
		0x8000, 
		0xd27c, 
		0xd220, 
		0xcb00, 
		0xd260, 
		0x5020, 
		0x4d40, 
		0x4ec0, 
		0xb6ad, 
		0xeab3, 
		0xdd7f, 
		0xd3e0, 
		0x71c6, 
		0xd020, 
		0xaf51, 
		0xfc00, 
		0xe9e0, 
		0x7cbd, 
		0x4c80, 
		0x5260, 
		0x81da, 
		0xd180, 
		0xdadd, 
		0x51e0, 
		0x906b, 
		0xc880, 
		0xc500, 
		0xc600, 
		0xd2c0, 
		0x5ec5, 
		0x6aaf, 
		0x071e, 
		0x5220, 
		0xc600, 
		0x0000, 
		0x0000, 
		0xc880, 
		0x4e00, 
		0xae5c, 
		0x0fd9, 
		0xd260, 
		0xd3e0, 
		0x2b7b, 
		0x69ce, 
		0xd020, 
		0x4880, 
		0xd120, 
		0xd2a0, 
		0xd000, 
		0xb780, 
		0x8000, 
		0x0000, 
		0x4d80, 
		0xd080, 
		0x4200, 
		0xfca5, 
		0xfc00, 
		0x4880, 
		0x4dfd, 
		0x4a80, 
		0x51a0, 
		0x8000, 
		0xd260, 
		0x7c00, 
		0xbc4d, 
		0xcdc0, 
		0xcf80, 
		0x8000, 
		0xd0a0, 
		0x2574, 
		0xd360, 
		0xd120, 
		0x4d00, 
		0x4846, 
		0x77b3, 
		0x8000, 
		0x4400, 
		0x51a0, 
		0xb7c3, 
		0x0000, 
		0x4e80, 
		0xfc92, 
		0x51e0, 
		0x1e28, 
		0x7da5, 
		0xc1e3, 
		0x4dc0, 
		0x32e5, 
		0x31c2, 
		0x2dfb, 
		0xcf80, 
		0xcb00, 
		0xc500, 
		0xd180, 
		0xd040, 
		0xccc0, 
		0xd360, 
		0x9f7b, 
		0xc1ed, 
		0x50e0, 
		0x52a0, 
		0xf328, 
		0xf94a, 
		0xcdc0, 
		0x5140, 
		0xafd2, 
		0x0921, 
		0x1573, 
		0x4a80, 
		0xb446, 
		0x4e80, 
		0xce40, 
		0x0000, 
		0x5381, 
		0x52a0, 
		0x5d8c, 
		0x4d40, 
		0xc000, 
		0xc800, 
		0xd914, 
		0x067d, 
		0x5260, 
		0xc200, 
		0xcc00, 
		0x51c0, 
		0xcd80, 
		0x53c0, 
		0xd340  
};
volatile uint32_t rand_ieee754_32[1536] __attribute__ ((aligned (2048))) = {
		0x0c7fff00, 
		0x7f800000, 
		0x007ffffe, 
		0x80359bdc, 
		0xc2300000, 
		0x0c7e0000, 
		0xb4370e0a, 
		0x00000020, 
		0x7f800000, 
		0x15a9833c, 
		0xc2580000, 
		0x42500000, 
		0x00080000, 
		0xde0542c3, 
		0xc2600000, 
		0x7f800000, 
		0xff809b3a, 
		0xff93d3fd, 
		0x007fffff, 
		0x0d00fff0, 
		0xffc00000, 
		0xd7cc0c48, 
		0x42480000, 
		0x761613e3, 
		0x42100000, 
		0x0e001fff, 
		0xc1500000, 
		0x41880000, 
		0x41e80000, 
		0x80000000, 
		0xc21c0000, 
		0x98bf8bbf, 
		0x8f7ffffd, 
		0x14efdf60, 
		0x0c7fffc0, 
		0xff800000, 
		0xc2600000, 
		0xc2700000, 
		0x7f800000, 
		0x1c9cd57e, 
		0xc1f80000, 
		0x79733bf5, 
		0xd130d9a1, 
		0x00002000, 
		0x7f800000, 
		0x426c0000, 
		0x80000002, 
		0x80000000, 
		0x30001646, 
		0x41a80000, 
		0xff7ffffe, 
		0x00800000, 
		0x40a00000, 
		0x7fc00000, 
		0x793be2c3, 
		0x1a5fa72e, 
		0x00000000, 
		0x0c7ff000, 
		0x0c7ff800, 
		0x00080000, 
		0x42180000, 
		0x42780000, 
		0x0c7c0000, 
		0xff800000, 
		0x1a2ca397, 
		0x00000000, 
		0x773742fb, 
		0xdf691273, 
		0x0c7ffe00, 
		0xff800000, 
		0x42580000, 
		0x00ffffff, 
		0x7f7ffffe, 
		0x7f7fffff, 
		0x4b000000, 
		0x41200000, 
		0x00800001, 
		0xa33975a0, 
		0x00800001, 
		0x422c0000, 
		0xc1d80000, 
		0x779a3caf, 
		0x5592f1e7, 
		0x00000000, 
		0x80000000, 
		0x80000001, 
		0x41c80000, 
		0x0f7ffffc, 
		0x75f1eda8, 
		0xc3c69327, 
		0xe7c2f5ef, 
		0x775eb304, 
		0x41980000, 
		0x8f7ffffc, 
		0x0c780000, 
		0xff9ee245, 
		0x0e07ffff, 
		0x41980000, 
		0x2d4a8642, 
		0xccce0559, 
		0x4c541464, 
		0x0e000007, 
		0x80800001, 
		0x00800001, 
		0x00004000, 
		0x0f7ffffd, 
		0xb3ca8d74, 
		0xd7096855, 
		0x42240000, 
		0xc1500000, 
		0x41e80000, 
		0x80040000, 
		0xe7de790c, 
		0x42440000, 
		0x90b3d66f, 
		0x00000001, 
		0x3f800000, 
		0x406db6bb, 
		0x7fadcd44, 
		0x80000800, 
		0x33333333, 
		0xff800000, 
		0x0e3fffff, 
		0xc2380000, 
		0xa8df7c3c, 
		0x00000000, 
		0x42340000, 
		0xc2040000, 
		0x0e0fffff, 
		0x03365dfd, 
		0x00000100, 
		0xc2180000, 
		0x48a83fdb, 
		0x3f028f5c, 
		0x0e0003ff, 
		0xc1880000, 
		0x40c00000, 
		0x7f7ffffe, 
		0x41100000, 
		0xc2100000, 
		0xe72bfea5, 
		0x41700000, 
		0x80800000, 
		0xb6967cc8, 
		0x24abdad1, 
		0x42380000, 
		0x0e0000ff, 
		0x807fffff, 
		0xeeeb3a23, 
		0xff8ebed1, 
		0xc4644560, 
		0x40400000, 
		0x17e7b5d3, 
		0xff800000, 
		0x80000000, 
		0x80000100, 
		0x42480000, 
		0x421c0000, 
		0xc0e00000, 
		0x80800003, 
		0x42340000, 
		0x007ffffe, 
		0x0f7fffff, 
		0x33333333, 
		0x11323fb4, 
		0x0c7ff800, 
		0x7f800000, 
		0x80000008, 
		0xc2440000, 
		0x0c7fffc0, 
		0x80000002, 
		0x65ee6cfe, 
		0x7fa1c7cb, 
		0x00800002, 
		0x889286f8, 
		0xc2780000, 
		0x39c281b6, 
		0x80800003, 
		0x7cb01dfb, 
		0x007fffff, 
		0x0c7ffffe, 
		0x00100000, 
		0xc2080000, 
		0x0e1fffff, 
		0x420c0000, 
		0xc2340000, 
		0x66922cb6, 
		0xd044e225, 
		0x41c80000, 
		0x00000002, 
		0x42240000, 
		0xff7ffffe, 
		0xffaac952, 
		0x7fc00001, 
		0x42740000, 
		0x00800001, 
		0x2c37362e, 
		0x12f3ff64, 
		0x42040000, 
		0xfa554f57, 
		0x80800000, 
		0x5ece516d, 
		0x00800001, 
		0xa2f73129, 
		0x42440000, 
		0x80000040, 
		0x1525abc3, 
		0xe975bb58, 
		0xfa334970, 
		0xf5124c9b, 
		0x7f7ffffe, 
		0x81220616, 
		0x0c7c0000, 
		0x00400000, 
		0xffffffff, 
		0x40400000, 
		0x00800002, 
		0xc2180000, 
		0xff800000, 
		0x0e000007, 
		0x06c5ad6c, 
		0x00b6976d, 
		0x3352902c, 
		0x0c7ffff0, 
		0xd7597b99, 
		0x00000000, 
		0x12c593c2, 
		0x00800001, 
		0x6f5d24f9, 
		0xff7ffffe, 
		0x33333333, 
		0xbfab4fb4, 
		0x41200000, 
		0xdf590c0d, 
		0x41c80000, 
		0x12018074, 
		0xe2fdcb63, 
		0x420c0000, 
		0xa7b00abc, 
		0x41d80000, 
		0x7f800000, 
		0xc2540000, 
		0x00000000, 
		0xc0c00000, 
		0x8f7ffffd, 
		0xbf028f5c, 
		0x7fb8cbd0, 
		0x0a116e6c, 
		0x80000008, 
		0xc1700000, 
		0x80000800, 
		0xc0400000, 
		0x00000800, 
		0x3f800001, 
		0xcb000000, 
		0xff800001, 
		0x5acb628d, 
		0x0c7f8000, 
		0xdd69835e, 
		0xffbfffff, 
		0x68626d6e, 
		0x00800002, 
		0x80080000, 
		0x74fc96dd, 
		0x7f800000, 
		0x0e007fff, 
		0x80800000, 
		0x4e3e7a54, 
		0x41200000, 
		0xfa1d5a1b, 
		0x80000004, 
		0x80000010, 
		0x3ec69bbd, 
		0x3f800000, 
		0x0ecb36f8, 
		0x7f83776d, 
		0xc2300000, 
		0x7f800001, 
		0x8e035134, 
		0x7f800000, 
		0x7f7ffffe, 
		0x007ffffe, 
		0x00000004, 
		0x91730602, 
		0xc1800000, 
		0x0c7ffff8, 
		0xbf800001, 
		0xc2080000, 
		0x55555555, 
		0x0e03ffff, 
		0xefcc9811, 
		0x0e03ffff, 
		0x00080000, 
		0x943f5e9e, 
		0x0e007fff, 
		0xc2700000, 
		0x00001000, 
		0x00800001, 
		0x263fe005, 
		0x7fe9149f, 
		0xff800000, 
		0x40400000, 
		0xc1a80000, 
		0x00000000, 
		0xcccccccc, 
		0xfc174de7, 
		0x0e0001ff, 
		0xb575d346, 
		0x7f800000, 
		0xffc00001, 
		0x7fffffff, 
		0x8f7ffffc, 
		0x4b8c4b40, 
		0x00002000, 
		0x00000008, 
		0xc1b00000, 
		0x0c7f8000, 
		0x7f800000, 
		0x0b1ebf79, 
		0x0e001fff, 
		0x807fffff, 
		0x40f173e5, 
		0x427c0000, 
		0x40800000, 
		0xc0800000, 
		0x80000000, 
		0x42080000, 
		0x97780168, 
		0x00ffffff, 
		0x7f800000, 
		0xff800001, 
		0x7f800000, 
		0x80000000, 
		0x8009900f, 
		0xc22c0000, 
		0x1689659f, 
		0x8aefd72f, 
		0x3f028f5c, 
		0x80000001, 
		0x80020000, 
		0xf6214299, 
		0xff800000, 
		0x0e01ffff, 
		0x007fffff, 
		0xe97c6b17, 
		0xc0a00000, 
		0x80000000, 
		0x7f800000, 
		0x0c7ffc00, 
		0xc23c0000, 
		0xff7ffffe, 
		0xc1b00000, 
		0xc1f80000, 
		0x23ded5b3, 
		0xad99e056, 
		0x0827e900, 
		0x877b2af1, 
		0x7f800000, 
		0x6519a05a, 
		0x80000000, 
		0xdef24746, 
		0x52958974, 
		0xc1900000, 
		0x0e0007ff, 
		0x41500000, 
		0xee2591dd, 
		0xa0bf28fa, 
		0x8bf37e91, 
		0x0c7ffe00, 
		0xcc57e4b5, 
		0x80100000, 
		0xff000000, 
		0x80800000, 
		0x6fe9cf8b, 
		0x827d21f0, 
		0x56a947de, 
		0x0c7ff800, 
		0xbf800000, 
		0x02afae77, 
		0x4b8c4b40, 
		0x80000002, 
		0xaf118f9b, 
		0x80000000, 
		0xff7ffffe, 
		0x3f028f5c, 
		0xfe5f3063, 
		0x00000000, 
		0x00000200, 
		0xac793fb4, 
		0x42740000, 
		0xdb1c9f85, 
		0xff7ffffe, 
		0xc2600000, 
		0xdc97a3fe, 
		0x00000000, 
		0x42680000, 
		0x42440000, 
		0x41000000, 
		0x9454c9f3, 
		0x7f7ffffe, 
		0x7f000000, 
		0x0e001fff, 
		0xff878def, 
		0x00000000, 
		0x58f10c42, 
		0xcb8c4b40, 
		0x7fc00000, 
		0x41c80000, 
		0xff97bc79, 
		0xc1e80000, 
		0x5e7f123e, 
		0x425c0000, 
		0x642c8c88, 
		0x40c00000, 
		0xbf800001, 
		0x469f7b69, 
		0x00800001, 
		0xc2700000, 
		0xb6484715, 
		0x007fffff, 
		0xffc00000, 
		0x42440000, 
		0x42640000, 
		0xc20c0000, 
		0x41980000, 
		0xc2400000, 
		0x80000400, 
		0x007fffff, 
		0xc2080000, 
		0x426c0000, 
		0x41000000, 
		0x0f7fffff, 
		0xc1700000, 
		0xff7ffffe, 
		0xc1e00000, 
		0x7fcca1bf, 
		0xd32dfe18, 
		0x80000002, 
		0x4ca8854b, 
		0x0e001fff, 
		0x3f800000, 
		0x7f800001, 
		0x80000001, 
		0x00800001, 
		0x7ce06c6c, 
		0xaf536b9a, 
		0x80800003, 
		0xffa85b94, 
		0x0c7ff000, 
		0x7fc00000, 
		0x42080000, 
		0x003ef0fd, 
		0x7f7ffffe, 
		0xc1100000, 
		0x80000100, 
		0xffa5d421, 
		0x207c3ab1, 
		0x0e07ffff, 
		0x6b105be4, 
		0x00000000, 
		0xc2200000, 
		0xc1e80000, 
		0x7f7ffffe, 
		0xc0590a77, 
		0xf6206fc8, 
		0xc1000000, 
		0x42280000, 
		0xc2080000, 
		0x007fffff, 
		0x4084e290, 
		0x00000010, 
		0x426c0000, 
		0xc1d80000, 
		0xff7fffff, 
		0x1e42bba2, 
		0x12d3f2da, 
		0x7f7fffff, 
		0xc0000000, 
		0x32703d01, 
		0x0c7fc000, 
		0x0e001fff, 
		0x2502eb44, 
		0x422c0000, 
		0x8efff2d8, 
		0x7fae1e8c, 
		0x80008000, 
		0x42000000, 
		0x80000000, 
		0x00000000, 
		0x00001000, 
		0x80000000, 
		0x0c7fff80, 
		0x7f800000, 
		0x672ed223, 
		0x0104e6c1, 
		0x7f9153e2, 
		0x0038a994, 
		0xc21c0000, 
		0xc1e00000, 
		0xc1900000, 
		0x00000010, 
		0x36290481, 
		0x41c80000, 
		0x80000000, 
		0x335d6934, 
		0x80000000, 
		0xff800000, 
		0x8610242b, 
		0x80ffffff, 
		0x00200000, 
		0x9a884dde, 
		0xc1e80000, 
		0xc0b6ade2, 
		0x00000020, 
		0xf0e1dc22, 
		0x41f00000, 
		0x773ffd9a, 
		0x8e7c60ae, 
		0x45d39edf, 
		0x649bf851, 
		0x421c0000, 
		0x4b8c4b40, 
		0x80000080, 
		0x46b91b15, 
		0x0c7fc000, 
		0x00000001, 
		0x01912f23, 
		0xffd08538, 
		0x0c7ffff0, 
		0x422c0000, 
		0x41a80000, 
		0xa680faf0, 
		0xc2700000, 
		0x80080000, 
		0x420c0000, 
		0xe78a1155, 
		0x9747e32b, 
		0x4518d6e2, 
		0xc1a80000, 
		0xc1f00000, 
		0x0c780000, 
		0xc24c0000, 
		0x80000400, 
		0x2be59e76, 
		0x7f7fffff, 
		0xe653cc89, 
		0x42000000, 
		0x00800001, 
		0x00040000, 
		0x42500000, 
		0x0c7ffe00, 
		0x41000000, 
		0xc1e00000, 
		0x7f7ffffe, 
		0x95a7ae3c, 
		0x00000000, 
		0xffbae08b, 
		0xc2400000, 
		0x005b256a, 
		0x3c7daa20, 
		0x0c7ff000, 
		0x00800003, 
		0x11782599, 
		0xcc8f499b, 
		0x00000000, 
		0xc1980000, 
		0x80000000, 
		0x423c0000, 
		0xc0000000, 
		0x7fb5ef6e, 
		0x80000000, 
		0x0c7fffff, 
		0xc2480000, 
		0x58bce3e8, 
		0x41b00000, 
		0x80000000, 
		0xc2240000, 
		0x9336de39, 
		0x055db2c5, 
		0x0f7ffffc, 
		0x80000000, 
		0xc2480000, 
		0x7f800000, 
		0xcccccccc, 
		0x80200000, 
		0x0c7e0000, 
		0x7f7fffff, 
		0x4fbd9347, 
		0x40a00000, 
		0x00000400, 
		0x80008000, 
		0x00000000, 
		0x7fc00000, 
		0x218af9fa, 
		0xc20c0000, 
		0x42000000, 
		0x3f028f5c, 
		0x0c780000, 
		0x807fffff, 
		0x41880000, 
		0x00000000, 
		0xc2740000, 
		0x00011111, 
		0x00011111, 
		0x42680000, 
		0xd9411359, 
		0x80000000, 
		0xf41d1a7d, 
		0x80008000, 
		0xffbfffff, 
		0x42240000, 
		0x80000800, 
		0xc1500000, 
		0x4bc04b63, 
		0x94e15dd2, 
		0x41980000, 
		0x0e0001ff, 
		0xc2180000, 
		0x00000001, 
		0x6602dc73, 
		0x1c7b5902, 
		0x80000080, 
		0x8932cd74, 
		0x64b1df04, 
		0x0c7ffff0, 
		0x57dc9b56, 
		0x80000000, 
		0x00000000, 
		0x41d00000, 
		0x80000000, 
		0xc22c0000, 
		0x80000000, 
		0x00000008, 
		0x7b16ee0d, 
		0xffeac1de, 
		0x00000002, 
		0x80000001, 
		0xfc1102ea, 
		0x0c7ffc00, 
		0xff7fffff, 
		0x0c600000, 
		0xc1a80000, 
		0x082fdb23, 
		0x7fbfffff, 
		0x807ffffe, 
		0x5a35148c, 
		0xc1200000, 
		0x00800000, 
		0x7fc00000, 
		0x41b80000, 
		0x0c7fffc0, 
		0x19d5a2b9, 
		0x41f80000, 
		0xff7ffffe, 
		0x070b860b, 
		0x11550a67, 
		0x0c7c0000, 
		0x97e66736, 
		0x80800001, 
		0x807ffffe, 
		0x0e000003, 
		0xc1300000, 
		0xc2280000, 
		0x643f064f, 
		0x0c780000, 
		0x2af57cc2, 
		0x80800002, 
		0xc1a00000, 
		0x0e00007f, 
		0x41880000, 
		0x41400000, 
		0x7f800000, 
		0x6006f0a4, 
		0xc1100000, 
		0x80800000, 
		0x40800000, 
		0x0e007fff, 
		0x6d324bd1, 
		0x00800003, 
		0x42740000, 
		0xf7d7b967, 
		0xfe36706b, 
		0x0c600000, 
		0x00011111, 
		0x891c4eb4, 
		0xa69e528b, 
		0x0c780000, 
		0x80800000, 
		0x42500000, 
		0x00000001, 
		0xff7ffffe, 
		0xbae68639, 
		0x0f7ffffe, 
		0x3603b924, 
		0x7441693e, 
		0xff800000, 
		0x40e00000, 
		0x42180000, 
		0xff800000, 
		0xff800000, 
		0x0c7ffe00, 
		0x0e1fffff, 
		0x8f7ffffc, 
		0x0e000fff, 
		0x40e00000, 
		0x7f800000, 
		0x0f7ffffd, 
		0x80800003, 
		0x80000000, 
		0x0c7ffc00, 
		0x00000001, 
		0xae0801d1, 
		0x7fc00001, 
		0xc26c0000, 
		0x0f7ffffe, 
		0xf24606ad, 
		0x80040000, 
		0xc0e00000, 
		0x00800002, 
		0xc22c0000, 
		0xc0000000, 
		0x7f800000, 
		0xeaa2fb01, 
		0xffffffff, 
		0x5d50e5eb, 
		0x80100000, 
		0x41800000, 
		0x2b41142f, 
		0x0e7fffff, 
		0x5915a3ea, 
		0x1897e0ae, 
		0x284d7047, 
		0xb493292d, 
		0x00800002, 
		0xff800000, 
		0x80000800, 
		0x80000200, 
		0x80000080, 
		0xff800000, 
		0x5cf11e52, 
		0x80000001, 
		0x50d82ee5, 
		0x80000010, 
		0xaaaaaaaa, 
		0x0019e5d1, 
		0x42280000, 
		0x0e03ffff, 
		0x0c7fffc0, 
		0x9030bca3, 
		0xe4615bb2, 
		0x5f95f332, 
		0x42300000, 
		0xf28caa33, 
		0xff800000, 
		0xbf800000, 
		0x00000000, 
		0x00000002, 
		0x63be7310, 
		0xff985798, 
		0x7f800000, 
		0x41c80000, 
		0x80000010, 
		0x7aeb8a5e, 
		0x42580000, 
		0x40979ae0, 
		0x40a00000, 
		0xc2200000, 
		0x80000002, 
		0xa3ddbec2, 
		0x5fb10d42, 
		0x6ccf0db1, 
		0xc1c80000, 
		0xc1100000, 
		0x802148a9, 
		0x80001000, 
		0xbf028f5c, 
		0x7f800000, 
		0xf5f817ff, 
		0x8016bc79, 
		0x40a00000, 
		0x00400000, 
		0x40c00000, 
		0xc22c0000, 
		0x1f8eaeb5, 
		0xff800000, 
		0xcccccccc, 
		0x4ff06450, 
		0xb74dea74, 
		0x0e03ffff, 
		0x80000000, 
		0xc1800000, 
		0xff7ffffe, 
		0x1c813055, 
		0x0e00001f, 
		0x00000001, 
		0x0c7ffc00, 
		0x423c0000, 
		0x2a508886, 
		0xf1d20ad8, 
		0x63f247d0, 
		0xc70f37b9, 
		0x80011111, 
		0x6fc0c449, 
		0x00000080, 
		0x58faf4e9, 
		0x00800002, 
		0x71057123, 
		0x7f800000, 
		0x41700000, 
		0x80000400, 
		0x41e00000, 
		0x42200000, 
		0x00ffffff, 
		0xff7fffff, 
		0xf0931315, 
		0x66e18c31, 
		0x7fbfffff, 
		0x80000000, 
		0xc1c80000, 
		0xff800000, 
		0x7f800000, 
		0xe946b54d, 
		0x80000000, 
		0x510e562c, 
		0x80000000, 
		0xff8379d0, 
		0xc1000000, 
		0x97aeb510, 
		0x6f7009d2, 
		0xfe83d941, 
		0x0e7fffff, 
		0x0c600000, 
		0x00000002, 
		0x0b693b70, 
		0x00800001, 
		0x4e2cd975, 
		0x41a80000, 
		0xc2640000, 
		0x84994790, 
		0x80800000, 
		0x80008000, 
		0xc2140000, 
		0x80004000, 
		0x0e01ffff, 
		0x7a0a8867, 
		0xaddc807f, 
		0x420c0000, 
		0x80800000, 
		0x04ebe2db, 
		0xc1e00000, 
		0x0c7ffffe, 
		0xb62af2ae, 
		0x00002000, 
		0xc23c0000, 
		0xff7ffffe, 
		0xe801248b, 
		0x56156b5f, 
		0xc1e00000, 
		0x80040000, 
		0x0cca2673, 
		0x80000020, 
		0xc1800000, 
		0x0e001fff, 
		0x00000010, 
		0xc2040000, 
		0x7f800000, 
		0x7f7ffffe, 
		0x21ffb339, 
		0xc0e00000, 
		0x7a93c03d, 
		0x420c0000, 
		0x0c7f8000, 
		0x00000000, 
		0x967e128b, 
		0x80000002, 
		0xff7ffffe, 
		0x7f85a805, 
		0xc1800000, 
		0x00800003, 
		0x8040c97d, 
		0x8f7ffffc, 
		0x70266724, 
		0x80011111, 
		0x6b6a0f2b, 
		0x29170aec, 
		0xcd709039, 
		0x80800003, 
		0xc22c0000, 
		0xc1b80000, 
		0xc2780000, 
		0x2befdb1c, 
		0x4f133493, 
		0x425c0000, 
		0xf16477f3, 
		0xeb039fe1, 
		0x0053dc66, 
		0xc2700000, 
		0x41880000, 
		0xc0800000, 
		0x0e0000ff, 
		0x55555555, 
		0x0c7ff800, 
		0x422c0000, 
		0x0036b1d0, 
		0x7fc00001, 
		0xc1d199a5, 
		0xc1000000, 
		0x7f800001, 
		0xb81aea02, 
		0x80000100, 
		0x2274ce69, 
		0x80000000, 
		0x80008000, 
		0xd7c6d58c, 
		0x00000040, 
		0x0c7fffff, 
		0x0e1fffff, 
		0x07c438e0, 
		0xc0400000, 
		0xff7ffffe, 
		0xc3b15fbd, 
		0x00040000, 
		0x0c7ffff8, 
		0xc21c0000, 
		0x0c7ffe00, 
		0x0d000ff0, 
		0x0e0007ff, 
		0x424c0000, 
		0x00000001, 
		0x5a5d2601, 
		0x80800003, 
		0x0d00fff0, 
		0xff7fffff, 
		0x42200000, 
		0x7fc00001, 
		0x147121c8, 
		0x42040000, 
		0x52b5cd2e, 
		0x41100000, 
		0x59866428, 
		0x80000001, 
		0x41980000, 
		0x44608975, 
		0x80000800, 
		0xde389cf5, 
		0x8328a528, 
		0x420c0000, 
		0xff7fffff, 
		0x00000001, 
		0x7fc6e679, 
		0x6f35e75f, 
		0x8c95db33, 
		0x00800003, 
		0xc2540000, 
		0x66c09e9b, 
		0x26d56d11, 
		0xe6719764, 
		0x0d00fff0, 
		0xfe7569a7, 
		0x80ffffff, 
		0x80000008, 
		0xb54f2a7e, 
		0xaaaaaaaa, 
		0xc2480000, 
		0x634d6569, 
		0x42300000, 
		0x0c7ff000, 
		0x00001000, 
		0xfba9d298, 
		0xdd288397, 
		0x0c7fffc0, 
		0x0e0001ff, 
		0xcbf92740, 
		0xdf08b10f, 
		0x41880000, 
		0x98830502, 
		0x41b80000, 
		0x9873292f, 
		0x80000001, 
		0x0e00007f, 
		0x80100000, 
		0x00000008, 
		0x8f7ffffd, 
		0xc0c00000, 
		0x76321395, 
		0xf0cc0457, 
		0x41e80000, 
		0xc1a00000, 
		0x0e3fffff, 
		0x0c7f0000, 
		0x0e03ffff, 
		0xc1d80000, 
		0x00000000, 
		0x7f000000, 
		0xc6cfb34d, 
		0x40800000, 
		0x1d125cbc, 
		0xff800000, 
		0xc1c80000, 
		0xc1f80000, 
		0x7fffffff, 
		0x00100000, 
		0x421c0000, 
		0x00000001, 
		0x7f800000, 
		0xc2100000, 
		0x0c7fffff, 
		0x42400000, 
		0x37fe4a60, 
		0x80000000, 
		0xc1980000, 
		0xac823b53, 
		0x0e000003, 
		0xc2340000, 
		0x41d80000, 
		0x0c7ff800, 
		0x03816d76, 
		0x5b4fd464, 
		0xebd36df6, 
		0x420c0000, 
		0xc200bdee, 
		0x80000000, 
		0x80000000, 
		0xb9d75277, 
		0x213442ab, 
		0xc1f80000, 
		0x00000000, 
		0x80000000, 
		0x80001000, 
		0xff7ffffe, 
		0x80800001, 
		0xff800000, 
		0xc0400000, 
		0x00000000, 
		0xe3e8d495, 
		0x857a7667, 
		0xc0800000, 
		0x0f7fffff, 
		0x00000400, 
		0xc1c00000, 
		0x1fd5a445, 
		0xac7cdc0d, 
		0x42640000, 
		0x42000000, 
		0xff800000, 
		0xe26d89c6, 
		0xc0400000, 
		0xad01d127, 
		0x00020000, 
		0x0e00007f, 
		0x805a728e, 
		0x00000000, 
		0x427c0000, 
		0x80040000, 
		0xc1f00000, 
		0x41f80000, 
		0x8319b12a, 
		0x7959e50c, 
		0x80200000, 
		0xbd272fa5, 
		0x7f000000, 
		0xff9ae9f9, 
		0x41700000, 
		0x2fff03cf, 
		0x41e80000, 
		0x3de28689, 
		0x41600000, 
		0x24040302, 
		0x1e88347a, 
		0xc2700000, 
		0xa6523dcc, 
		0xc0a00000, 
		0x7f7ffffe, 
		0x3c9f5fc6, 
		0x6d92c079, 
		0xae552b17, 
		0x00000000, 
		0x0c7f0000, 
		0x42040000, 
		0xf49f9def, 
		0x105d5a8e, 
		0x41d00000, 
		0xffc00001, 
		0x0c7fc000, 
		0xff800000, 
		0x00800003, 
		0x7f982df5, 
		0x41300000, 
		0x80800002, 
		0x33333333, 
		0xffc00000, 
		0xc0e00000, 
		0x0c7ffffc, 
		0xffffffff, 
		0x807c036a, 
		0x0e0000ff, 
		0x27dd64f3, 
		0x3f800000, 
		0x0e007fff, 
		0xaa3faedf, 
		0xff800000, 
		0x0c400000, 
		0x450654d1, 
		0x41d80000, 
		0x0c600000, 
		0x42240000, 
		0x0c700000, 
		0x7a343e30, 
		0xa80e76fe, 
		0x6dd453b4, 
		0x42380000, 
		0x00800002, 
		0x80080000, 
		0xc2740000, 
		0x4f5eeb38, 
		0xc1000000, 
		0xc2540000, 
		0xff800000, 
		0x00000200, 
		0xff800000, 
		0xff800000, 
		0x0c7c0000, 
		0x0c7fe000, 
		0x42180000, 
		0xc1400000, 
		0x6bbc8f0b, 
		0xc2040000, 
		0xff7fffff, 
		0xc2040000, 
		0x41900000, 
		0x8764ceeb, 
		0x0c7fc000, 
		0x00100000, 
		0x94f86abd, 
		0x0e003fff, 
		0x00000008, 
		0x00000002, 
		0x7f7ffffe, 
		0xe2e3a78e, 
		0x21d19b36, 
		0x41980000, 
		0x87984518, 
		0x8f7fffff, 
		0x41d00000, 
		0x0e000001, 
		0x426c0000, 
		0x42340000, 
		0x0c7fffe0, 
		0x80000000, 
		0x0c780000, 
		0xbf800000, 
		0x41e80000, 
		0x427c0000, 
		0xff7ffffe, 
		0xa70127a6, 
		0x0e00003f, 
		0x00800000, 
		0xc2300000, 
		0x00000000, 
		0xc1600000, 
		0x40800000, 
		0x00000000, 
		0x8ede8c72, 
		0xc2300000, 
		0xc2040000, 
		0x80000000, 
		0x00004000, 
		0x80800001, 
		0xff800000, 
		0x80020000, 
		0xff92e185, 
		0x42600000, 
		0xc2380000, 
		0x422c0000, 
		0x0d00fff0, 
		0x0c7ffff8, 
		0x80004000, 
		0x443c89c6, 
		0x0c7fffff, 
		0x40000000, 
		0x45be4d61, 
		0x0f7fffff, 
		0xc2780000, 
		0x00000001, 
		0xc0a00000, 
		0x426c0000, 
		0x00400000, 
		0x0e000007, 
		0xc2380000, 
		0x0c700000, 
		0x0d000ff0, 
		0x36aad39a, 
		0x0e00000f, 
		0x41f80000, 
		0x3f8ae7c6, 
		0x7a53ddc2, 
		0xeeff061f, 
		0x80000010, 
		0x4a420e81, 
		0xe74f83ac, 
		0x80000000, 
		0xc2780000, 
		0x17c96956, 
		0xff40c683, 
		0xc23c0000, 
		0x0630291f, 
		0x41200000, 
		0x31e4971b, 
		0xc23c0000, 
		0x80000002, 
		0x00800000, 
		0x25024f7e, 
		0x00000002, 
		0x80000000, 
		0x42700000, 
		0x41f80000, 
		0x80000000, 
		0xc2140000, 
		0xc1400000, 
		0x42600000, 
		0x80011111, 
		0x80800001, 
		0xc1d00000, 
		0x80080000, 
		0x0444b83b, 
		0x848706ad, 
		0xc2580000, 
		0xc2780000, 
		0xff800000, 
		0xc1980000, 
		0x41900000, 
		0x0e0fffff, 
		0x851a07bf, 
		0xbf028f5c, 
		0x56248231, 
		0x8f7ffffd, 
		0x80800003, 
		0x00000000, 
		0x427c0000, 
		0x42400000, 
		0x42540000, 
		0xf4c55b28, 
		0x00000800, 
		0x41b00000, 
		0xc2600000, 
		0x00400000, 
		0x00002000, 
		0x0e03ffff, 
		0x80000000, 
		0x00000000, 
		0xc2500000, 
		0x00400000, 
		0x7f800000, 
		0x0e00007f, 
		0xc1100000, 
		0x0baf909a, 
		0xdeae2dda, 
		0x00000000, 
		0xf8101092, 
		0x41700000, 
		0x0c7c0000, 
		0x80000001, 
		0xb68127b3, 
		0xc0000000, 
		0xbf028f5c, 
		0xca7e9dca, 
		0x10871032, 
		0x7f9b0a46, 
		0x6d7bd6f8, 
		0x60de9914, 
		0x80000800, 
		0xa1e543e4, 
		0x33333333, 
		0xc2680000, 
		0x80000400, 
		0x4b8c4b40, 
		0xb64e7633, 
		0x00000010, 
		0x0c7ffff0, 
		0x41800000, 
		0x80000000, 
		0x00000000, 
		0xc1d9c7bc, 
		0x700e1060, 
		0x4e7eb0de, 
		0xc2780000, 
		0xcb8c4b40, 
		0x42080000, 
		0xdb4d4f90, 
		0x42080000, 
		0x40800000, 
		0xe2c3cb48, 
		0x6f3fd328, 
		0x47d94ae6, 
		0x00a93c50, 
		0xff800000, 
		0x0c7fffc0, 
		0x80000001, 
		0x80000000, 
		0x7079aca5, 
		0xc0e00000, 
		0x80000000, 
		0x80002000, 
		0x007d6450, 
		0x0f7ffffc, 
		0x80080000, 
		0xff800000, 
		0x7f000000, 
		0x00ffffff, 
		0x00000080, 
		0x00000000, 
		0xe640a2bf, 
		0xecb63bea, 
		0xc1d00000, 
		0x80020000, 
		0x41c80000, 
		0xe8b0d046, 
		0xaa979676, 
		0x598b0da5, 
		0xab6d4abf, 
		0x007fffff, 
		0x321ab0b0, 
		0x0d000ff0, 
		0xc2280000, 
		0x80011111, 
		0x00800000, 
		0xc78357c7, 
		0x0f7ffffc, 
		0x82338c81, 
		0x0c7f8000, 
		0x40800000, 
		0x0e003fff, 
		0x00000000, 
		0x42140000, 
		0x28560e7d, 
		0xb27eafec, 
		0x4263923d, 
		0xc20c0000, 
		0xff7ffffe, 
		0xc2440000, 
		0xff7ffffe, 
		0xa194277c, 
		0xa6757ff9, 
		0x00800000, 
		0x00000000, 
		0xc24c0000, 
		0x8ea21f07, 
		0x40400000, 
		0x0c7ffe00, 
		0xc2340000, 
		0x98d001d7, 
		0x80800001, 
		0xc21c0000, 
		0x42080000, 
		0x5124f63b, 
		0x364f5b0c, 
		0xc1c80000, 
		0x80000000, 
		0x7f7ffffe, 
		0x736b3e56, 
		0x80000100, 
		0x7fc00000, 
		0x0d000ff0, 
		0x00000000, 
		0x8390ea2c, 
		0xff9acef3, 
		0xbdf328c3, 
		0xc2040000, 
		0x80000000, 
		0xff7fffff, 
		0x0c7fffc0, 
		0x0c7ffc00, 
		0x80080000, 
		0x80000000, 
		0xff000000, 
		0xc1980000, 
		0x38dc6e7e, 
		0x8bbee048, 
		0xc2440000, 
		0xee412d8d, 
		0x00002000, 
		0xc1c00000, 
		0xff7ffffe, 
		0x0401b22d, 
		0x00000001, 
		0x41700000, 
		0x51bfcaec, 
		0x0e0007ff, 
		0x0e000007, 
		0x7fb688af, 
		0x80000040, 
		0x0e0003ff, 
		0x7f800000, 
		0x80800003, 
		0x42000000, 
		0xca3f89ac, 
		0x9a6e63ca, 
		0x00000000, 
		0xd5811aab, 
		0x80800001, 
		0xffb0e506, 
		0x00000080, 
		0x42740000, 
		0x93f8f9f0, 
		0x0e0001ff, 
		0xc2040000, 
		0x80000000, 
		0x00080000, 
		0xbd6f7940, 
		0x0c7ffe00, 
		0xa6d16390, 
		0x80000000, 
		0x7f7ffffe, 
		0x42280000, 
		0x00000000, 
		0x7f800000, 
		0x80000040, 
		0x0c32e0f2, 
		0x0e00007f, 
		0xff7fffff, 
		0x80000000, 
		0x00000000, 
		0x80800003, 
		0x0c7ffe00, 
		0x0e00000f, 
		0x0f7ffffd, 
		0x4835ad58, 
		0xc1d00000, 
		0xa05c9ecd, 
		0xffc00001, 
		0x7fc00000, 
		0x861974db, 
		0x3fd5436d, 
		0x0e07ffff, 
		0x80397d97, 
		0x422c0000, 
		0x3fd2e0c7, 
		0x2b1fe788, 
		0x7fbfffff, 
		0x00001000, 
		0xff800000, 
		0x00000100, 
		0xc0400000, 
		0x42400000, 
		0x42740000, 
		0x00000002, 
		0xbf028f5c, 
		0x80100000, 
		0x7fdafeed, 
		0x0c7f0000, 
		0x14fcc2e7, 
		0x00000002, 
		0xe0ca6155, 
		0x80000000, 
		0x0e00000f, 
		0x07d82e36, 
		0x0f7ffffd, 
		0x0e03ffff, 
		0x7f7ffffe, 
		0x8f7ffffe, 
		0xc2740000, 
		0x80000001, 
		0xc2300000, 
		0x00000000, 
		0x83396667, 
		0x04545e35, 
		0x41a00000, 
		0x80800001, 
		0x00ffffff, 
		0x00400000, 
		0x80000000, 
		0xc1400000, 
		0x80000000, 
		0x83f8dcaa, 
		0x8a93a4f6, 
		0x3a39dc9d, 
		0x1c91a025, 
		0x814f8ce8, 
		0x7f7ffffe, 
		0xc1400000, 
		0x0e00007f, 
		0x7f7ffffe, 
		0x0e1fffff, 
		0x42380000, 
		0xc23c0000, 
		0x421c0000, 
		0x528cf1bb, 
		0xff7fffff, 
		0x00400000, 
		0x80008000, 
		0x7fa9e507, 
		0x74cfca35, 
		0xc1980000, 
		0x9a7fd304, 
		0x0c7ffff0, 
		0xffb5cc56, 
		0x5a36cf4a, 
		0xc1500000, 
		0x80800003, 
		0xc2380000, 
		0x80800003, 
		0x64466757, 
		0xf5f4ccc3, 
		0xa0bb2927, 
		0x00800000, 
		0x80000002, 
		0xb5cb2760, 
		0xff7ffffe, 
		0xe195124e, 
		0x0dfd38af, 
		0x80000008, 
		0x00010000, 
		0xc22c0000, 
		0x00040000, 
		0x00000010, 
		0x7be2c167, 
		0x3f800001, 
		0x32ee4003, 
		0x42380000, 
		0x41880000, 
		0x0c7c0000, 
		0x0c7fff00, 
		0x75299a47, 
		0x423c0000, 
		0x0c7ff800, 
		0x0e00ffff, 
		0x67ad4337, 
		0x7f7fffff, 
		0x00000001, 
		0xdc713a4c, 
		0xc1d80000, 
		0xc0800000, 
		0xc0c3e2ef, 
		0x372a4efb, 
		0xff800000, 
		0x00800001  
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
		0x00000127, 
		0x000001ab, 
		0x000001d9, 
		0x00000363, 
		0x0000021d, 
		0x000001af, 
		0x000000ad, 
		0x000003cb, 
		0x00000179, 
		0x00000379, 
		0x0000028f, 
		0x00000127, 
		0x0000038d, 
		0x000002c3, 
		0x000003b9, 
		0x0000028d, 
		0x0000015b, 
		0x00000043, 
		0x0000038d, 
		0x00000329, 
		0x000002f5, 
		0x00000327, 
		0x00000057, 
		0x00000125, 
		0x00000013, 
		0x00000085, 
		0x000000bb, 
		0x0000036d, 
		0x000001b3, 
		0x0000020d, 
		0x000000b3, 
		0x000001f9, 
		0x0000012b, 
		0x0000011b, 
		0x00000233, 
		0x00000165, 
		0x00000319, 
		0x0000009f, 
		0x000002e9, 
		0x00000209, 
		0x0000003d, 
		0x0000013b, 
		0x00000047, 
		0x000000b1, 
		0x00000291, 
		0x0000036b, 
		0x00000383, 
		0x00000329, 
		0x00000265, 
		0x000003d5, 
		0x0000011f, 
		0x00000009, 
		0x000000ad, 
		0x0000031f, 
		0x000003e3, 
		0x00000351, 
		0x00000197, 
		0x000001ad, 
		0x000000b3, 
		0x0000000b, 
		0x0000003d, 
		0x000003ad, 
		0x000003a9, 
		0x00000209, 
		0x00000329, 
		0x000000e5, 
		0x000002bb, 
		0x0000025b, 
		0x00000157, 
		0x0000034b, 
		0x000002af, 
		0x000003d5, 
		0x000000e7, 
		0x00000171, 
		0x00000261, 
		0x000000e9, 
		0x00000085, 
		0x000002e7, 
		0x0000002d, 
		0x00000225, 
		0x000001a7, 
		0x000001f7, 
		0x00000157, 
		0x000003e1, 
		0x000003e7, 
		0x000003bb, 
		0x000003b5, 
		0x000003fb, 
		0x00000033, 
		0x0000036d, 
		0x000003d1, 
		0x00000337, 
		0x00000025, 
		0x000002b7, 
		0x000002cf, 
		0x0000021f, 
		0x0000022d, 
		0x00000351, 
		0x0000007d, 
		0x000001b3, 
		0x000001b5, 
		0x0000015d, 
		0x000003b7, 
		0x00000337, 
		0x0000034f, 
		0x00000067, 
		0x0000008d, 
		0x000001c7, 
		0x0000001f, 
		0x00000047, 
		0x000003b9, 
		0x000002c7, 
		0x00000051, 
		0x00000159, 
		0x0000028b, 
		0x000001fb, 
		0x00000151, 
		0x0000034d, 
		0x000003c9, 
		0x00000083, 
		0x0000023b, 
		0x00000155, 
		0x000001f7, 
		0x00000045, 
		0x0000025b, 
		0x00000243, 
		0x000000a7, 
		0x000002c3, 
		0x000002a5, 
		0x00000249, 
		0x0000015d, 
		0x00000365, 
		0x0000003f, 
		0x000003f7, 
		0x0000008d, 
		0x0000006d, 
		0x00000175, 
		0x000002c5, 
		0x00000077, 
		0x000001a5, 
		0x00000003, 
		0x0000038b, 
		0x0000005b, 
		0x00000335, 
		0x000003c3, 
		0x000000b9, 
		0x0000002d, 
		0x00000173, 
		0x0000004b, 
		0x00000101, 
		0x0000019d, 
		0x000003ed, 
		0x0000020b, 
		0x00000297, 
		0x00000125, 
		0x00000193, 
		0x000002db, 
		0x000002c3, 
		0x000003eb, 
		0x000002ad, 
		0x00000323, 
		0x000000a3, 
		0x000002df, 
		0x000003e7, 
		0x00000169, 
		0x00000311, 
		0x000003ed, 
		0x000002b1, 
		0x000001f3, 
		0x000000c9, 
		0x00000211, 
		0x000002ff, 
		0x0000010b, 
		0x00000003, 
		0x0000025d, 
		0x000001fd, 
		0x000000e9, 
		0x0000017d, 
		0x000001c9, 
		0x00000193, 
		0x00000215, 
		0x000002e5, 
		0x000002fd, 
		0x0000006f, 
		0x000000ef, 
		0x00000271, 
		0x00000199, 
		0x000000c5, 
		0x00000195, 
		0x00000367, 
		0x000003d9, 
		0x00000097, 
		0x000002a1, 
		0x000002eb, 
		0x0000001d, 
		0x0000025d, 
		0x0000009b, 
		0x00000249, 
		0x000002b3, 
		0x0000017b, 
		0x0000023b, 
		0x00000327, 
		0x00000181, 
		0x000000b1, 
		0x000000bd, 
		0x000002a5, 
		0x00000263, 
		0x00000247, 
		0x00000291, 
		0x00000161, 
		0x00000177, 
		0x0000004b, 
		0x0000014b, 
		0x000001ad, 
		0x0000014f, 
		0x00000329, 
		0x0000012d, 
		0x000001db, 
		0x00000233, 
		0x000002b7, 
		0x0000018d, 
		0x00000269, 
		0x0000025d, 
		0x000000b5, 
		0x0000017f, 
		0x000002db, 
		0x000000e5, 
		0x0000032f, 
		0x00000271, 
		0x000003b7, 
		0x000002c9, 
		0x00000251, 
		0x00000359, 
		0x0000027d, 
		0x000002fd, 
		0x0000018f, 
		0x00000257, 
		0x00000387, 
		0x000000e9, 
		0x000001c5, 
		0x00000131, 
		0x0000005f, 
		0x000003b1, 
		0x00000165, 
		0x000001f7, 
		0x00000243, 
		0x00000297, 
		0x000001eb, 
		0x000000d9, 
		0x00000271, 
		0x0000019f, 
		0x00000225, 
		0x000003e5, 
		0x00000377, 
		0x00000321, 
		0x0000032d  
};

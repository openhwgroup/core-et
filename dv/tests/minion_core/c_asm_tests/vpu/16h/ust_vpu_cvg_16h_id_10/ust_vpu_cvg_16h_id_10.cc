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
			"li x5, 430\n"
			"faddi.pi f2, f0, -53\n"
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
			"csrwi mhpmevent4, 4\n"   
			"csrwi mhpmevent5, 21\n"   
			"csrwi mhpmevent6, 6\n"   
			"csrwi mhpmevent7, 4\n"   
			"csrwi mhpmevent8, 5\n"   
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
			"flw.ps f29,  704 (x5)\n"
			"flw.ps f6,  608 (x5)\n"
			"flw.ps f25,  128 (x5)\n"
			"flw.ps f1,  160 (x5)\n"
			"flw.ps f19,  480 (x5)\n"
			"flw.ps f21,  800 (x5)\n"
			"flw.ps f30,  832 (x5)\n"
			"flw.ps f4,  864 (x5)\n"
			"flw.ps f10,  256 (x5)\n"
			"flw.ps f5,  736 (x5)\n"
			"flw.ps f18,  64 (x5)\n"
			"flw.ps f22,  640 (x5)\n"
			"flw.ps f12,  928 (x5)\n"
			"flw.ps f28,  992 (x5)\n"
			"flw.ps f0,  544 (x5)\n"
			"flw.ps f15,  224 (x5)\n"
			"flw.ps f3,  320 (x5)\n"
			"flw.ps f7,  896 (x5)\n"
			"flw.ps f2,  32 (x5)\n"
			"flw.ps f8,  672 (x5)\n"
			"flw.ps f13,  352 (x5)\n"
			"flw.ps f17,  576 (x5)\n"
			"flw.ps f24,  0 (x5)\n"
			"flw.ps f16,  512 (x5)\n"
			"flw.ps f23,  96 (x5)\n"
			"flw.ps f31,  192 (x5)\n"
			"flw.ps f11,  448 (x5)\n"
			"flw.ps f27,  960 (x5)\n"
			"flw.ps f14,  288 (x5)\n"
			"flw.ps f9,  416 (x5)\n"
			"flw.ps f26,  384 (x5)\n"
			"flw.ps f20,  768 (x5)\n"
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
			"li x0, 0x44c\n" 
			"slti x0, x0, 0x7f2\n"
			"la x31, check_sum_hid_0\n"
			"li x5, 0x5\n" 
			"sw x5, (x31)\n"
			"li x29, 0x4\n"
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
			"la x28, rand_ieee754_32\n"
			"flw.ps f12, 128(x28)\n"
			"flw.ps f1, 448(x28)\n"
			"flw.ps f31, 224(x28)\n"
			"fmsub.ps f24, f12, f1, f31, rne\n" 
			"la x17, rand_int_32\n"
			"flw.ps f2, 800(x17)\n"
			"flw.ps f8, 64(x17)\n"
			"fmaxu.pi f28, f2, f8\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f26, 32(x17)\n"
			"flw.ps f16, 576(x17)\n"
			"flw.ps f1, 736(x17)\n"
			"fmadd.s f25, f26, f16, f1, rup\n" 
			"la x13, rand_int_32\n"
			"flw.ps f2, 320(x13)\n"
			"fslli.pi f8, f2, 0xf\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f12, 736(x28)\n"
			"flw.ps f28, 256(x28)\n"
			"fmax.s f24, f12, f28\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f12, 864(x23)\n"
			"flw.ps f15, 928(x23)\n"
			"fmax.s f31, f12, f15\n" 
			"la x9, rand_int_32\n"
			"flw.ps f19, 224(x9)\n"
			"fandi.pi f22, f19, 0x59\n" 
			"li x5, 0x29a6\n"
			"csrw tensor_mask, x5\n"	
			"la x18, rand_ieee754_32\n"
			"flw.ps f1, 896(x18)\n"
			"flw.ps f7, 128(x18)\n"
			"flw.ps f15, 864(x18)\n"
			"fnmadd.ps f28, f1, f7, f15, rup\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f26, 160(x25)\n"
			"flw.ps f8, 864(x25)\n"
			"fmul.s f2, f26, f8, rne\n" 
			"maskpopcz x10, m7\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f19, 384(x25)\n"
			"ffrc.ps f22, f19\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f24, 864(x18)\n"
			"fcvt.w.s x17, f24, rne\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f12, 128(x18)\n"
			"fclass.ps f1, f12\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f15, 224(x23)\n"
			"flw.ps f16, 320(x23)\n"
			"fmin.ps f13, f15, f16\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f31, 832(x27)\n"
			"fsin.ps f2, f31\n" 
			"la x15, rand_int_32\n"
			"flw.ps f8, 224(x15)\n"
			"fpackrepb.pi f19, f8\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_0_RDX_ID_0_SHIRE_0_HID_0:\n"
			"li x15, 0xc00000002640002\n"
			"csrw tensor_reduce, x15\n" 
			"csrwi tensor_wait, 9\n" 
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=0); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ITER_0_HID_0:\n"
			"la x31, self_check_16\n"
			"li x5, 0x40000000000006\n"
			"add x5, x5, x31\n"
			"li x6, 0x10000000000007\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x0 << TENSOR_FMA16_B_NUM_COLS) | (0x6 << TENSOR_FMA16_A_NUM_ROWS) | (0x7 << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x1 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x19 << TENSOR_FMA16_SCP_LOC_B) | (0x2 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x9 << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0x6 << TENSOR_QUANT_QUANT_ROW) | (0x29 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x0 << TENSOR_QUANT_TRANSF1) | (0x0 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ITER_1_HID_0:\n"
			"la x31, self_check_8\n"
			"li x5, 0x80000000000005\n"
			"add x5, x5, x31\n"
			"li x6, 0x1000000000000f\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x0 << TENSOR_IMA_B_NUM_COLS) | (0x5 << TENSOR_IMA_A_NUM_ROWS) | (0xf << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x1 << TENSOR_IMA_TEN_B_IN_MEM) | (0x18 << TENSOR_IMA_SCP_LOC_B) | (0x4 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fmvz.x.ps x31, f0, 0\n"
			"li x5, 0x40\n"
			"bne x5, x31, LBL_FAIL_HID_0\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_0\n"
			"csrw tensor_error, x0\n"
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_0\n"
			"addi x30, x30, 1\n"
			"la x25, rand_ieee754_32\n"
			"flw.ps f26, 192(x25)\n"
			"flw.ps f8, 128(x25)\n"
			"fmul.s f1, f26, f8, rmm\n" 
			"LBL_SEQID_1_M0_WRITE_ID_0_HID_0:\n"
			"feqm.ps m2, f28, f28\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x27, rand_ieee754_32\n"
			"flw.ps f13, 352(x27)\n"
			"flw.ps f2, 96(x27)\n"
			"fmul.ps f2, f13, f2, rtz\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f19, 320(x9)\n"
			"flw.ps f25, 672(x9)\n"
			"fsgnjn.s f19, f19, f25\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f1, 160(x25)\n"
			"flw.ps f26, 32(x25)\n"
			"fsgnj.s f12, f1, f26\n" 
			"LBL_SEQID_1_M0_WRITE_ID_1_HID_0:\n"
			"mova.m.x x9\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x23, rand_int_32\n"
			"flw.ps f19, 288(x23)\n"
			"flw.ps f28, 384(x23)\n"
			"fle.pi f17, f19, f28\n" 
			"la x27, rand_int_32\n"
			"flw.ps f17, 736(x27)\n"
			"flw.ps f16, 832(x27)\n"
			"fmin.pi f7, f17, f16\n" 
			"LBL_SEQID_1_FP_TRANS_ID_2_HID_0:\n"
			"la x27, rand_ieee754_32\n"
			"flw.ps f1, 544(x27)\n"
			"frcp.ps f25, f1\n" 
			"la x25, rand_int_32\n"
			"flw.ps f17, 416(x25)\n"
			"flw.ps f7, 736(x25)\n"
			"fle.pi f8, f17, f7\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f13, 32(x27)\n"
			"flw.ps f17, 224(x27)\n"
			"fsgnj.s f25, f13, f17\n" 
			"la x15, rand_int_32\n"
			"flw.ps f24, 960(x15)\n"
			"fsat8.pi f15, f24\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f31, 384(x27)\n"
			"flw.ps f24, 288(x27)\n"
			"fmul.ps f15, f31, f24, rup\n" 
			"la x27, rand_int_32\n"
			"flw.ps f24, 64(x27)\n"
			"fpackrepb.pi f1, f24\n" 
			"maskpopcz x27, m2\n" 
			"la x28, rand_int_32\n"
			"flw.ps f12, 32(x28)\n"
			"fpackrepb.pi f22, f12\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_1_RDX_ID_1_SHIRE_0_HID_0:\n"
			"li x15, 0x2800000006650031\n"
			"csrr x5, mhartid\n"
			"srli x5, x5, 4\n"
			"slli x5, x5, 6\n"
			"add x15, x15, x5\n"
			"csrw tensor_reduce, x15\n" 
			"csrwi tensor_wait, 9\n" 
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=0); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ITER_0_HID_0:\n"
			"li x6, ((0x19 << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0xa << TENSOR_QUANT_QUANT_ROW) | (0x33 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x7 << TENSOR_QUANT_TRANSF7) | (0x7 << TENSOR_QUANT_TRANSF6) | (0x6 << TENSOR_QUANT_TRANSF5) | (0x6 << TENSOR_QUANT_TRANSF4) | (0x7 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ITER_1_HID_0:\n"
			"li x5, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x1 << TENSOR_FMA16_B_NUM_COLS) | (0xb << TENSOR_FMA16_A_NUM_ROWS) | (0x9 << TENSOR_FMA16_A_NUM_COLS) | (0x5 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0xe2 << TENSOR_FMA16_SCP_LOC_B) | (0x9b << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x6, ((0x1f << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0xb << TENSOR_QUANT_QUANT_ROW) | (0x1c << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x2 << TENSOR_QUANT_TRANSF8) | (0x7 << TENSOR_QUANT_TRANSF7) | (0x7 << TENSOR_QUANT_TRANSF6) | (0x6 << TENSOR_QUANT_TRANSF5) | (0x6 << TENSOR_QUANT_TRANSF4) | (0x6 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 7\n"
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_0\n"
			"csrw tensor_error, x0\n"
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_0\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_2_M0_WRITE_ID_3_HID_0:\n"
			"feqm.ps m2, f25, f17\n" 
			"li x5, CREDINC2\n"
			"ld x5, 0(x5)\n"
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x9, rand_ieee754_32\n"
			"flw.ps f8, 544(x9)\n"
			"fcvt.pwu.ps f12, f8, rtz\n" 
			"la x18, rand_int_32\n"
			"flw.ps f15, 992(x18)\n"
			"fnot.pi f25, f15\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f15, 128(x28)\n"
			"flw.ps f1, 704(x28)\n"
			"fsub.ps f17, f15, f1, rtz\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f1, 192(x10)\n"
			"flw.ps f16, 800(x10)\n"
			"fadd.ps f16, f1, f16, rne\n" 
			"la x25, rand_int_32\n"
			"flw.ps f13, 896(x25)\n"
			"flw.ps f17, 672(x25)\n"
			"fand.pi f15, f13, f17\n" 
			"li x5, 0xa421\n"
			"csrw tensor_mask, x5\n"	
			"la x13, rand_ieee754_32\n"
			"flw.ps f17, 736(x13)\n"
			"flw.ps f2, 672(x13)\n"
			"fsub.ps f28, f17, f2, rup\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f15, 992(x9)\n"
			"fmin.s f26, f15, f15\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x28, rand_int_32\n"
			"flw.ps f7, 256(x28)\n"
			"flw.ps f16, 864(x28)\n"
			"fsll.pi f12, f7, f16\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f22, 864(x23)\n"
			"flw.ps f12, 640(x23)\n"
			"fadd.s f22, f22, f12, rne\n" 
			"la x17, rand_int_32\n"
			"flw.ps f2, 96(x17)\n"
			"flw.ps f15, 672(x17)\n"
			"fmin.pi f15, f2, f15\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f28, 352(x17)\n"
			"flw.ps f16, 384(x17)\n"
			"feq.ps f12, f28, f16\n" 
			"LBL_SEQID_2_M0_WRITE_ID_4_HID_0:\n"
			"fltm.pi m2, f16, f25\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x23, rand_ieee754_32\n"
			"flw.ps f2, 480(x23)\n"
			"fclass.s x10, f2\n" 
			"LBL_SEQID_2_M0_WRITE_ID_5_HID_0:\n"
			"maskand m4, m7, m4\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0xcdf06bddb8\n"
			"fsw.ps f0, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"li x13, 0x24e7bc82b4d4c2e8\n"
			"fcvt.s.wu f24, x13, rtz\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_2_XMA_QNT_STR_SEQ_ID_189427_HID_0:\n"
			"li x6, ((0xc << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0x7 << TENSOR_QUANT_QUANT_ROW) | (0xf << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_0\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_2_XMA_QNT_STR_SEQ_ITER_0_HID_0:\n"
			"li x5, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x0 << TENSOR_FMA16_B_NUM_COLS) | (0xd << TENSOR_FMA16_A_NUM_ROWS) | (0x9 << TENSOR_FMA16_A_NUM_COLS) | (0x5 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x53 << TENSOR_FMA16_SCP_LOC_B) | (0x35 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x0 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
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
			"LBL_SEQID_3_M0_WRITE_ID_6_HID_0:\n"
			"maskxor m2, m6, m7\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x25, rand_ieee754_32\n"
			"flw.ps f8, 608(x25)\n"
			"fsin.ps f24, f8\n" 
			"la x23, rand_int_32\n"
			"flw.ps f31, 320(x23)\n"
			"fsrli.pi f8, f31, 0xd\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f13, 352(x23)\n"
			"flw.ps f24, 320(x23)\n"
			"fmul.s f28, f13, f24, rtz\n" 
			"li x5, 0xfee2939fa81c67a6\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0x77cf3bc54b2b8bb4\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"LBL_SEQID_3_FP_TRANS_ID_7_HID_0:\n"
			"la x27, rand_ieee754_32\n"
			"flw.ps f31, 768(x27)\n"
			"flog.ps f17, f31\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f31, 64(x23)\n"
			"fcvt.w.s x18, f31, rne\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f12, 992(x10)\n"
			"flw.ps f25, 608(x10)\n"
			"fle.s x23, f12, f25\n" 
			"LBL_SEQID_3_M0_WRITE_ID_8_HID_0:\n"
			"fltm.ps m6, f24, f17\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_3_M0_WRITE_ID_9_HID_0:\n"
			"flem.ps m4, f16, f19\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x27, rand_int_32\n"
			"flw.ps f1, 512(x27)\n"
			"flw.ps f2, 288(x27)\n"
			"fmulh.pi f13, f1, f2\n" 
			"la x9, rand_int_32\n"
			"flw.ps f8, 960(x9)\n"
			"flw.ps f15, 320(x9)\n"
			"feq.pi f1, f8, f15\n" 
			"LBL_SEQID_3_FP_TRANS_ID_10_HID_0:\n"
			"la x27, rand_ieee754_32\n"
			"flw.ps f22, 896(x27)\n"
			"fexp.ps f19, f22\n" 
			"la x15, rand_int_32\n"
			"flw.ps f31, 864(x15)\n"
			"flw.ps f12, 608(x15)\n"
			"frem.pi f22, f31, f12\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f25, 512(x23)\n"
			"flw.ps f31, 416(x23)\n"
			"flw.ps f13, 576(x23)\n"
			"fnmadd.s f24, f25, f31, f13, rne\n" 
			"la x27, rand_int_32\n"
			"flw.ps f25, 256(x27)\n"
			"fandi.pi f15, f25, 0xd3\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f15, 992(x15)\n"
			"fmax.ps f25, f15, f15\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_3_XMA_QNT_STR_SEQ_ITER_0_HID_0:\n"
			"li x5, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x0 << TENSOR_FMA32_B_NUM_COLS) | (0x9 << TENSOR_FMA32_A_NUM_ROWS) | (0x1 << TENSOR_FMA32_A_NUM_COLS) | (0x6 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x2 << TENSOR_FMA32_SCP_LOC_B) | (0x81 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x0 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"LBL_SEQID_3_XMA_QNT_STR_SEQ_ITER_1_HID_0:\n"
			"li x5, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x2 << TENSOR_FMA32_B_NUM_COLS) | (0x0 << TENSOR_FMA32_A_NUM_ROWS) | (0xb << TENSOR_FMA32_A_NUM_COLS) | (0x9 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x43 << TENSOR_FMA32_SCP_LOC_B) | (0x6e << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x0 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
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
			"la x15, rand_int_32\n"
			"flw.ps f22, 576(x15)\n"
			"flw.ps f7, 736(x15)\n"
			"fminu.pi f16, f22, f7\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f15, 448(x10)\n"
			"flw.ps f8, 736(x10)\n"
			"fsgnjx.ps f24, f15, f8\n" 
			"fbci.pi f24, 0x422a9\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f12, 64(x10)\n"
			"fcvt.w.s x15, f12, rdn\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f2, 384(x15)\n"
			"flw.ps f25, 416(x15)\n"
			"fmax.s f26, f2, f25\n" 
			"maskpopcz x13, m7\n" 
			"li x5, 0xee691a55edc1e6b0\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0x4177a287e65ca33e\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"la x18, rand_ieee754_32\n"
			"flw.ps f31, 320(x18)\n"
			"flw.ps f15, 320(x18)\n"
			"flw.ps f19, 320(x18)\n"
			"fmadd.ps f19, f31, f15, f19, rtz\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f25, 928(x27)\n"
			"flw.ps f2, 64(x27)\n"
			"fsgnjx.s f12, f25, f2\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f12, 0(x25)\n"
			"fcvt.pw.ps f16, f12, rup\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f2, 448(x28)\n"
			"flw.ps f17, 64(x28)\n"
			"fmul.ps f28, f2, f17, rup\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f25, 640(x9)\n"
			"flw.ps f2, 384(x9)\n"
			"flw.ps f15, 992(x9)\n"
			"fnmadd.s f17, f25, f2, f15, rne\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f8, 64(x18)\n"
			"flw.ps f19, 160(x18)\n"
			"fsgnjx.ps f26, f8, f19\n" 
			"la x18, rand_int_32\n"
			"flw.ps f26, 32(x18)\n"
			"fsrli.pi f31, f26, 0xc\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f22, 192(x17)\n"
			"flw.ps f13, 352(x17)\n"
			"feq.s x25, f22, f13\n" 
			"LBL_SEQID_4_M0_WRITE_ID_11_HID_0:\n"
			"masknot m2, m4\n" 
			"li x5, 0x80340120\n"
			"ld x5, 0(x5)\n"
			"la x27, rand_ieee754_32\n"
			"flw.ps f25, 480(x27)\n"
			"fround.ps f16, f25, rmm\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_4_TLD_XMA_STR_SEQ_ITER_0_HID_0:\n"
			"la x31, self_check_16\n"
			"li x5, 0xe0000000000005\n"
			"add x5, x5, x31\n"
			"li x6, 0x1000000000000f\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x3 << TENSOR_FMA16_B_NUM_COLS) | (0x5 << TENSOR_FMA16_A_NUM_ROWS) | (0xf << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x1 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1c << TENSOR_FMA16_SCP_LOC_B) | (0x7 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x14 << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0x5 << TENSOR_QUANT_QUANT_ROW) | (0x3d << TENSOR_QUANT_QUANT_SCP_LOC) | (0x2 << TENSOR_QUANT_TRANSF9) | (0x7 << TENSOR_QUANT_TRANSF8) | (0x7 << TENSOR_QUANT_TRANSF7) | (0x7 << TENSOR_QUANT_TRANSF6) | (0x7 << TENSOR_QUANT_TRANSF5) | (0x6 << TENSOR_QUANT_TRANSF4) | (0x6 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0x20\n"
			"bne x5, x31, LBL_FAIL_HID_0\n"
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 10\n"
			"LBL_SEQID_4_TLD_XMA_STR_SEQ_ITER_1_HID_0:\n"
			"la x31, self_check_16\n"
			"li x5, 0xe000000000000c\n"
			"add x5, x5, x31\n"
			"li x6, 0x3c0000000000005\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x0 << TENSOR_FMA16_B_NUM_COLS) | (0xc << TENSOR_FMA16_A_NUM_ROWS) | (0x5 << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1e << TENSOR_FMA16_SCP_LOC_B) | (0x7 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
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
			"bne x5, x31, LBL_FAIL_HID_0\n"
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
			"LBL_HPM_3_CORE_DCACHE_MISSES0_FAIL_HID_0:\n"          
			"csrr x5, mhpmcounter3\n"
			"sd x5, 32 (x31)\n"
			"LBL_HPM_4_CORE_BRANCHES0_FAIL_HID_0:\n"          
			"csrr x5, mhpmcounter4\n"
			"sd x5, 40 (x31)\n"
			"LBL_HPM_5_CORE_TXFMA_OPS32_FAIL_HID_0:\n"          
			"csrr x5, mhpmcounter5\n"
			"sd x5, 48 (x31)\n"
			"LBL_HPM_6_CORE_DCACHE_ACCESS0_FAIL_HID_0:\n"          
			"csrr x5, mhpmcounter6\n"
			"sd x5, 56 (x31)\n"
			"LBL_HPM_7_NEIGH_TX_NEIGH_COOP_TLD_REQ_FAIL_HID_0:\n"          
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
			"LBL_HPM_3_CORE_DCACHE_MISSES0_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter3\n"
			"sd x5, 32 (x31)\n"
			"LBL_HPM_4_CORE_BRANCHES0_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter4\n"
			"sd x5, 40 (x31)\n"
			"LBL_HPM_5_CORE_TXFMA_OPS32_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter5\n"
			"sd x5, 48 (x31)\n"
			"LBL_HPM_6_CORE_DCACHE_ACCESS0_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter6\n"
			"sd x5, 56 (x31)\n"
			"LBL_HPM_7_NEIGH_TX_NEIGH_COOP_TLD_REQ_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter7\n"
			"sd x5, 64 (x31)\n"
			"LBL_HPM_8_NEIGH_RX_COOP_TLD_RSP_PASS_HID_0:\n"          
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
			"csrwi mhpmevent3, 24\n"   
			"csrwi mhpmevent4, 1\n"   
			"csrwi mhpmevent5, 2\n"   
			"csrwi mhpmevent6, 8\n"   
			"csrwi mhpmevent7, 1\n"   
			"csrwi mhpmevent8, 2\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f0,  992 (x5)\n"
			"flw.ps f11,  576 (x5)\n"
			"flw.ps f7,  896 (x5)\n"
			"flw.ps f10,  832 (x5)\n"
			"flw.ps f1,  256 (x5)\n"
			"flw.ps f12,  928 (x5)\n"
			"flw.ps f4,  352 (x5)\n"
			"flw.ps f2,  32 (x5)\n"
			"flw.ps f26,  544 (x5)\n"
			"flw.ps f18,  224 (x5)\n"
			"flw.ps f3,  704 (x5)\n"
			"flw.ps f29,  384 (x5)\n"
			"flw.ps f28,  64 (x5)\n"
			"flw.ps f22,  416 (x5)\n"
			"flw.ps f27,  128 (x5)\n"
			"flw.ps f5,  0 (x5)\n"
			"flw.ps f20,  960 (x5)\n"
			"flw.ps f16,  192 (x5)\n"
			"flw.ps f30,  96 (x5)\n"
			"flw.ps f8,  768 (x5)\n"
			"flw.ps f15,  512 (x5)\n"
			"flw.ps f25,  864 (x5)\n"
			"flw.ps f19,  800 (x5)\n"
			"flw.ps f6,  672 (x5)\n"
			"flw.ps f21,  480 (x5)\n"
			"flw.ps f23,  320 (x5)\n"
			"flw.ps f9,  160 (x5)\n"
			"flw.ps f17,  640 (x5)\n"
			"flw.ps f14,  448 (x5)\n"
			"flw.ps f24,  736 (x5)\n"
			"flw.ps f31,  288 (x5)\n"
			"flw.ps f13,  608 (x5)\n"
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
			"li x29, 0x4\n"
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
			"la x7, rand_int_32\n"
			"flw.ps f16, 800(x7)\n"
			"fpackrepb.pi f13, f16\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_1:\n"
			"fsetm.pi m4, f4\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x25, rand_int_32\n"
			"flw.ps f5, 896(x25)\n"
			"flw.ps f17, 352(x25)\n"
			"fadd.pi f26, f5, f17\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f31, 96(x15)\n"
			"flw.ps f14, 768(x15)\n"
			"feq.ps f26, f31, f14\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f31, 224(x23)\n"
			"ffrc.ps f16, f31\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f19, 224(x25)\n"
			"fcvt.f16.ps f10, f19\n" 
			"fbci.ps f5, 0x4102a\n" 
			"la x27, rand_int_32\n"
			"flw.ps f12, 448(x27)\n"
			"flw.ps f14, 96(x27)\n"
			"fmin.pi f31, f12, f14\n" 
			"la x25, rand_int_32\n"
			"flw.ps f12, 288(x25)\n"
			"flw.ps f19, 160(x25)\n"
			"fsra.pi f4, f12, f19\n" 
			"la x9, rand_int_32\n"
			"flw.ps f20, 0(x9)\n"
			"flw.ps f13, 480(x9)\n"
			"fmulhu.pi f22, f20, f13\n" 
			"la x9, rand_int_32\n"
			"flw.ps f26, 672(x9)\n"
			"flw.ps f22, 864(x9)\n"
			"fmul.pi f31, f26, f22\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_1:\n"
			"maskor m4, m1, m1\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x9, rand_ieee754_32\n"
			"flw.ps f17, 768(x9)\n"
			"flw.ps f15, 320(x9)\n"
			"fsgnjn.ps f19, f17, f15\n" 
			"LBL_SEQID_0_M0_WRITE_ID_2_HID_1:\n"
			"maskand m1, m1, m3\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x7, rand_int_32\n"
			"flw.ps f4, 704(x7)\n"
			"flw.ps f5, 32(x7)\n"
			"feq.pi f20, f4, f5\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f13, 576(x25)\n"
			"flt.ps f11, f13, f13\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_1\n"
			"addi x30, x30, 1\n"
			"la x7, rand_int_32\n"
			"flw.ps f15, 0(x7)\n"
			"fsat8.pi f4, f15\n" 
			"maskpopc x9, m3\n" 
			"la x25, rand_int_32\n"
			"flw.ps f31, 288(x25)\n"
			"fsat8.pi f5, f31\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f17, 192(x27)\n"
			"flw.ps f14, 32(x27)\n"
			"fmax.s f16, f17, f14\n" 
			"la x6, rand_int_32\n"
			"flw.ps f22, 256(x6)\n"
			"flw.ps f10, 640(x6)\n"
			"fle.pi f17, f22, f10\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f16, 608(x6)\n"
			"flw.ps f15, 992(x6)\n"
			"fmul.s f11, f16, f15, rdn\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f16, 480(x9)\n"
			"fswizz.ps f13, f16, 0x1b\n" 
			"la x25, rand_int_32\n"
			"flw.ps f19, 128(x25)\n"
			"faddi.pi f14, f19, 0x16\n" 
			"la x11, rand_int_32\n"
			"flw.ps f11, 320(x11)\n"
			"flw.ps f13, 832(x11)\n"
			"fsra.pi f17, f11, f13\n" 
			"la x11, rand_int_32\n"
			"flw.ps f3, 192(x11)\n"
			"fsat8.pi f5, f3\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f11, 704(x23)\n"
			"flw.ps f12, 320(x23)\n"
			"fle.ps f15, f11, f12\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f10, 384(x15)\n"
			"flw.ps f26, 192(x15)\n"
			"fmul.s f10, f10, f26, rdn\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f15, 640(x13)\n"
			"flw.ps f11, 896(x13)\n"
			"flw.ps f22, 96(x13)\n"
			"fmadd.ps f16, f15, f11, f22, rne\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f31, 224(x12)\n"
			"flw.ps f4, 320(x12)\n"
			"fle.s x25, f31, f4\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f20, 160(x12)\n"
			"flw.ps f12, 640(x12)\n"
			"fmadd.ps f13, f20, f12, f20, rmm\n" 
			"la x11, rand_int_32\n"
			"flw.ps f11, 832(x11)\n"
			"flw.ps f13, 256(x11)\n"
			"fxor.pi f19, f11, f13\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_1\n"
			"addi x30, x30, 1\n"
			"la x9, rand_int_32\n"
			"flw.ps f14, 736(x9)\n"
			"flw.ps f3, 512(x9)\n"
			"fmul.pi f15, f14, f3\n" 
			"LBL_SEQID_2_M0_WRITE_ID_3_HID_1:\n"
			"masknot m3, m1\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"maskpopc x25, m4\n" 
			"la x27, rand_int_32\n"
			"flw.ps f17, 192(x27)\n"
			"flw.ps f26, 384(x27)\n"
			"fmin.pi f15, f17, f26\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f11, 288(x13)\n"
			"flw.ps f19, 96(x13)\n"
			"fmax.s f13, f11, f19\n" 
			"li x9, 0x77fc7c7805c7b994\n"
			"fcvt.s.w f22, x9, rup\n" 
			"li x15, 0x27613dbfcaedf425\n"
			"fcvt.s.wu f14, x15, rtz\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f22, 928(x12)\n"
			"flw.ps f4, 544(x12)\n"
			"fle.ps f5, f22, f4\n" 
			"maskpopc x6, m1\n" 
			"fbci.ps f16, 0x4af1d\n" 
			"LBL_SEQID_2_FP_TRANS_ID_4_HID_1:\n"
			"la x27, rand_ieee754_32\n"
			"flw.ps f12, 480(x27)\n"
			"flog.ps f15, f12\n" 
			"la x27, rand_int_32\n"
			"flw.ps f5, 960(x27)\n"
			"fpackrepb.pi f20, f5\n" 
			"la x25, rand_int_32\n"
			"flw.ps f16, 480(x25)\n"
			"fsrai.pi f26, f16, 0xb\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f14, 928(x9)\n"
			"flw.ps f13, 736(x9)\n"
			"fcmovm.ps f19, f14, f13\n" 
			"li x6, 0x18dc8c00435a3c5a\n"
			"fcvt.s.wu f22, x6, rne\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f19, 832(x15)\n"
			"flw.ps f13, 544(x15)\n"
			"fle.s x25, f19, f13\n" 
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
			"li x9, 0x7196c138c96bb796\n"
			"fcvt.s.w f5, x9, rup\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f4, 0(x23)\n"
			"flw.ps f12, 512(x23)\n"
			"flw.ps f10, 512(x23)\n"
			"fcmov.ps f12, f4, f12, f10\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f12, 864(x27)\n"
			"fclass.ps f31, f12\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f16, 352(x6)\n"
			"flw.ps f12, 0(x6)\n"
			"fsgnjn.s f15, f16, f12\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f17, 512(x23)\n"
			"flw.ps f19, 160(x23)\n"
			"feq.s x7, f17, f19\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f15, 288(x15)\n"
			"flw.ps f17, 0(x15)\n"
			"flw.ps f4, 160(x15)\n"
			"fnmadd.ps f13, f15, f17, f4, rup\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f19, 96(x25)\n"
			"flw.ps f3, 192(x25)\n"
			"fmin.s f14, f19, f3\n" 
			"la x9, rand_int_32\n"
			"flw.ps f19, 320(x9)\n"
			"flw.ps f5, 256(x9)\n"
			"fadd.pi f17, f19, f5\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f11, 0(x15)\n"
			"flw.ps f16, 416(x15)\n"
			"flt.s x13, f11, f16\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f26, 928(x7)\n"
			"flw.ps f31, 192(x7)\n"
			"flw.ps f14, 864(x7)\n"
			"fnmsub.ps f16, f26, f31, f14, rdn\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f22, 288(x9)\n"
			"fround.ps f20, f22, rmm\n" 
			"la x23, rand_int_32\n"
			"flw.ps f10, 768(x23)\n"
			"fcvt.ps.pw f14, f10, rdn\n" 
			"la x27, rand_int_32\n"
			"flw.ps f10, 768(x27)\n"
			"flw.ps f11, 576(x27)\n"
			"fsra.pi f11, f10, f11\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f20, 448(x11)\n"
			"flw.ps f31, 416(x11)\n"
			"flw.ps f3, 736(x11)\n"
			"fnmadd.s f15, f20, f31, f3, rtz\n" 
			"la x9, rand_int_32\n"
			"flw.ps f19, 896(x9)\n"
			"flw.ps f17, 160(x9)\n"
			"fsll.pi f16, f19, f17\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f31, 448(x12)\n"
			"flw.ps f11, 160(x12)\n"
			"flt.ps f31, f31, f11\n" 
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
			"la x6, rand_ieee754_32\n"
			"flw.ps f26, 224(x6)\n"
			"flw.ps f11, 96(x6)\n"
			"flw.ps f31, 384(x6)\n"
			"fnmadd.ps f19, f26, f11, f31, rup\n" 
			"la x11, rand_int_32\n"
			"flw.ps f14, 672(x11)\n"
			"flw.ps f4, 736(x11)\n"
			"fmax.pi f16, f14, f4\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f20, 704(x7)\n"
			"flw.ps f5, 192(x7)\n"
			"feq.s x6, f20, f5\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f5, 640(x6)\n"
			"fclass.s x23, f5\n" 
			"LBL_SEQID_4_M0_WRITE_ID_5_HID_1:\n"
			"masknot m1, m1\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"maskpopc x7, m6\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f26, 928(x6)\n"
			"flw.ps f14, 32(x6)\n"
			"fmin.ps f4, f26, f14\n" 
			"la x11, rand_int_32\n"
			"flw.ps f31, 704(x11)\n"
			"fsrai.pi f3, f31, 0xe\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f4, 96(x9)\n"
			"fmax.ps f20, f4, f4\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f31, 512(x11)\n"
			"fsin.ps f3, f31\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f13, 32(x15)\n"
			"flw.ps f14, 352(x15)\n"
			"fmax.s f11, f13, f14\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f17, 480(x27)\n"
			"flw.ps f15, 480(x27)\n"
			"fsgnjn.s f22, f17, f15\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f12, 64(x9)\n"
			"flw.ps f5, 192(x9)\n"
			"fmul.s f19, f12, f5, rne\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f5, 416(x25)\n"
			"flw.ps f11, 128(x25)\n"
			"fle.ps f11, f5, f11\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f13, 224(x7)\n"
			"flw.ps f5, 864(x7)\n"
			"fle.ps f31, f13, f5\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f20, 256(x25)\n"
			"flw.ps f11, 288(x25)\n"
			"flw.ps f26, 320(x25)\n"
			"fnmsub.s f14, f20, f11, f26, rne\n" 
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
			"LBL_HPM_3_CORE_SHORT_OPS_FAIL_HID_1:\n"          
			"csrr x5, mhpmcounter3\n"
			"sd x5, 32 (x31)\n"
			"LBL_HPM_4_CORE_MCYCLES_FAIL_HID_1:\n"          
			"csrr x5, mhpmcounter4\n"
			"sd x5, 40 (x31)\n"
			"LBL_HPM_5_CORE_RETIRED_INST0_FAIL_HID_1:\n"          
			"csrr x5, mhpmcounter5\n"
			"sd x5, 48 (x31)\n"
			"LBL_HPM_6_CORE_DCACHE_MISSES0_FAIL_HID_1:\n"          
			"csrr x5, mhpmcounter6\n"
			"sd x5, 56 (x31)\n"
			"LBL_HPM_7_NEIGH_ANY_CORE_TX_ETL_REQ_FAIL_HID_1:\n"          
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
			"LBL_HPM_3_CORE_SHORT_OPS_PASS_HID_1:\n"          
			"csrr x5, mhpmcounter3\n"
			"sd x5, 32 (x31)\n"
			"LBL_HPM_4_CORE_MCYCLES_PASS_HID_1:\n"          
			"csrr x5, mhpmcounter4\n"
			"sd x5, 40 (x31)\n"
			"LBL_HPM_5_CORE_RETIRED_INST0_PASS_HID_1:\n"          
			"csrr x5, mhpmcounter5\n"
			"sd x5, 48 (x31)\n"
			"LBL_HPM_6_CORE_DCACHE_MISSES0_PASS_HID_1:\n"          
			"csrr x5, mhpmcounter6\n"
			"sd x5, 56 (x31)\n"
			"LBL_HPM_7_NEIGH_ANY_CORE_TX_ETL_REQ_PASS_HID_1:\n"          
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
			"csrwi mhpmevent4, 4\n"   
			"csrwi mhpmevent5, 21\n"   
			"csrwi mhpmevent6, 6\n"   
			"csrwi mhpmevent7, 4\n"   
			"csrwi mhpmevent8, 5\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f25,  96 (x5)\n"
			"flw.ps f11,  992 (x5)\n"
			"flw.ps f0,  0 (x5)\n"
			"flw.ps f23,  512 (x5)\n"
			"flw.ps f20,  480 (x5)\n"
			"flw.ps f30,  704 (x5)\n"
			"flw.ps f21,  544 (x5)\n"
			"flw.ps f6,  736 (x5)\n"
			"flw.ps f29,  768 (x5)\n"
			"flw.ps f16,  384 (x5)\n"
			"flw.ps f13,  896 (x5)\n"
			"flw.ps f24,  224 (x5)\n"
			"flw.ps f15,  448 (x5)\n"
			"flw.ps f19,  416 (x5)\n"
			"flw.ps f12,  288 (x5)\n"
			"flw.ps f10,  320 (x5)\n"
			"flw.ps f18,  928 (x5)\n"
			"flw.ps f8,  352 (x5)\n"
			"flw.ps f14,  64 (x5)\n"
			"flw.ps f26,  256 (x5)\n"
			"flw.ps f3,  32 (x5)\n"
			"flw.ps f1,  192 (x5)\n"
			"flw.ps f17,  160 (x5)\n"
			"flw.ps f4,  800 (x5)\n"
			"flw.ps f9,  672 (x5)\n"
			"flw.ps f2,  864 (x5)\n"
			"flw.ps f31,  128 (x5)\n"
			"flw.ps f7,  832 (x5)\n"
			"flw.ps f28,  608 (x5)\n"
			"flw.ps f27,  576 (x5)\n"
			"flw.ps f22,  960 (x5)\n"
			"flw.ps f5,  640 (x5)\n"
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
			"li x29, 0x4\n"
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
			"la x20, rand_ieee754_32\n"
			"flw.ps f9, 896(x20)\n"
			"flw.ps f28, 0(x20)\n"
			"flw.ps f2, 288(x20)\n"
			"fmsub.s f1, f9, f28, f2, rne\n" 
			"la x5, rand_int_32\n"
			"flw.ps f29, 352(x5)\n"
			"flw.ps f8, 352(x5)\n"
			"fsrl.pi f9, f29, f8\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f0, 800(x10)\n"
			"flw.ps f26, 128(x10)\n"
			"feq.ps f30, f0, f26\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f26, 672(x11)\n"
			"flw.ps f21, 192(x11)\n"
			"fmin.s f30, f26, f21\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f26, 224(x28)\n"
			"fmin.s f31, f26, f26\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f29, 512(x20)\n"
			"flw.ps f2, 672(x20)\n"
			"fmin.ps f17, f29, f2\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f29, 480(x20)\n"
			"flw.ps f28, 768(x20)\n"
			"fsub.ps f23, f29, f28, rup\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f30, 0(x28)\n"
			"flw.ps f1, 544(x28)\n"
			"flw.ps f26, 128(x28)\n"
			"fnmsub.ps f6, f30, f1, f26, rtz\n" 
			"la x23, rand_int_32\n"
			"flw.ps f21, 736(x23)\n"
			"flw.ps f26, 608(x23)\n"
			"flt.pi f12, f21, f26\n" 
			"mova.x.m x20\n" 
			"la x20, rand_int_32\n"
			"flw.ps f29, 864(x20)\n"
			"fsrl.pi f17, f29, f29\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_2:\n"
			"fltm.pi m4, f0, f30\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x28, rand_int_32\n"
			"flw.ps f8, 192(x28)\n"
			"flw.ps f30, 928(x28)\n"
			"fadd.pi f30, f8, f30\n" 
			"la x16, rand_int_32\n"
			"flw.ps f31, 640(x16)\n"
			"flw.ps f9, 512(x16)\n"
			"fmaxu.pi f6, f31, f9\n" 
			"la x26, rand_int_32\n"
			"flw.ps f30, 864(x26)\n"
			"flw.ps f26, 288(x26)\n"
			"fand.pi f18, f30, f26\n" 
			"la x15, rand_int_32\n"
			"flw.ps f21, 64(x15)\n"
			"flw.ps f31, 672(x15)\n"
			"fle.pi f8, f21, f31\n" 
			"li x5, 0xcf7a00762fcad6a1\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0xe0c246e520ce49f3\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_0_RDX_ID_0_SHIRE_0_HID_2:\n"
			"li x25, 0xc00000002640002\n"
			"csrw tensor_reduce, x25\n" 
			"csrwi tensor_wait, 9\n" 
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=0); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ITER_0_HID_2:\n"
			"la x31, self_check_32\n"
			"li x5, 0x4000000000000a\n"
			"add x5, x5, x31\n"
			"li x6, 0x320000000000006\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x3 << TENSOR_FMA32_B_NUM_COLS) | (0xa << TENSOR_FMA32_A_NUM_ROWS) | (0x6 << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x19 << TENSOR_FMA32_SCP_LOC_B) | (0x2 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
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
			"bne x5, x31, LBL_FAIL_HID_2\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_2\n"
			"csrw tensor_error, x0\n"
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_2\n"
			"addi x30, x30, 1\n"
			"la x25, rand_int_32\n"
			"flw.ps f0, 224(x25)\n"
			"flw.ps f17, 800(x25)\n"
			"fsra.pi f9, f0, f17\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f0, 928(x16)\n"
			"flw.ps f12, 160(x16)\n"
			"fsgnjx.s f1, f0, f12\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f28, 352(x15)\n"
			"fcvt.wu.s x25, f28, rtz\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x5, rand_ieee754_32\n"
			"flw.ps f26, 864(x5)\n"
			"flw.ps f8, 192(x5)\n"
			"fsub.s f21, f26, f8, rtz\n" 
			"fbci.ps f1, 0x23cb4\n" 
			"li x5, 0x4b09\n"
			"csrw tensor_mask, x5\n"	
			"la x25, rand_ieee754_32\n"
			"flw.ps f21, 480(x25)\n"
			"flw.ps f6, 32(x25)\n"
			"flw.ps f28, 928(x25)\n"
			"fcmov.ps f21, f21, f6, f28\n" 
			"la x15, rand_int_32\n"
			"flw.ps f9, 192(x15)\n"
			"flw.ps f28, 448(x15)\n"
			"fsra.pi f26, f9, f28\n" 
			"LBL_SEQID_1_M0_WRITE_ID_1_HID_2:\n"
			"maskxor m7, m3, m4\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x10, rand_ieee754_32\n"
			"flw.ps f26, 0(x10)\n"
			"flw.ps f1, 416(x10)\n"
			"fsub.s f28, f26, f1, rmm\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f6, 448(x28)\n"
			"flw.ps f29, 736(x28)\n"
			"fsub.ps f8, f6, f29, rmm\n" 
			"la x20, rand_int_32\n"
			"flw.ps f1, 32(x20)\n"
			"flw.ps f26, 192(x20)\n"
			"feq.pi f30, f1, f26\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f2, 320(x20)\n"
			"fcvt.w.s x16, f2, rdn\n" 
			"la x16, rand_int_32\n"
			"flw.ps f8, 192(x16)\n"
			"flw.ps f29, 384(x16)\n"
			"fle.pi f9, f8, f29\n" 
			"la x20, rand_int_32\n"
			"flw.ps f9, 224(x20)\n"
			"flw.ps f0, 384(x20)\n"
			"fsll.pi f1, f9, f0\n" 
			"la x28, rand_int_32\n"
			"flw.ps f1, 352(x28)\n"
			"flw.ps f6, 96(x28)\n"
			"fle.pi f18, f1, f6\n" 
			"la x23, rand_int_32\n"
			"flw.ps f23, 832(x23)\n"
			"flw.ps f6, 192(x23)\n"
			"fle.pi f17, f23, f6\n" 
			"csrw tensor_error, zero\n" 
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=0); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ITER_0_HID_2:\n"
			"li x6, ((0x7 << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0x8 << TENSOR_QUANT_QUANT_ROW) | (0x1b << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x7 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_2\n"
			"csrw tensor_error, x0\n"
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_2\n"
			"addi x30, x30, 1\n"
			"mova.x.m x5\n" 
			"LBL_SEQID_2_M0_WRITE_ID_2_HID_2:\n"
			"masknot m4, m3\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x15, rand_ieee754_32\n"
			"flw.ps f6, 160(x15)\n"
			"fcmovm.ps f0, f6, f6\n" 
			"LBL_SEQID_2_M0_WRITE_ID_3_HID_2:\n"
			"masknot m7, m4\n" 
			"li x5, 0x80340118\n"
			"ld x5, 0(x5)\n"
			"la x25, rand_int_32\n"
			"flw.ps f28, 736(x25)\n"
			"flw.ps f30, 832(x25)\n"
			"fle.pi f23, f28, f30\n" 
			"LBL_SEQID_2_M0_WRITE_ID_4_HID_2:\n"
			"maskand m7, m7, m2\n" 
			"li x5, CREDINC0\n"
			"ld x5, 0(x5)\n"
			"LBL_SEQID_2_M0_WRITE_ID_5_HID_2:\n"
			"mova.m.x x28\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x15, rand_ieee754_32\n"
			"flw.ps f17, 960(x15)\n"
			"flw.ps f23, 0(x15)\n"
			"fle.ps f30, f17, f23\n" 
			"la x25, rand_int_32\n"
			"flw.ps f6, 736(x25)\n"
			"flw.ps f29, 512(x25)\n"
			"fsll.pi f26, f6, f29\n" 
			"LBL_SEQID_2_M0_WRITE_ID_6_HID_2:\n"
			"fltm.pi m2, f28, f8\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x26, rand_int_32\n"
			"flw.ps f29, 672(x26)\n"
			"flw.ps f30, 192(x26)\n"
			"fsra.pi f23, f29, f30\n" 
			"LBL_SEQID_2_M0_WRITE_ID_7_HID_2:\n"
			"mova.m.x x28\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x10, rand_ieee754_32\n"
			"flw.ps f23, 512(x10)\n"
			"frsq.ps f31, f23\n" 
			"LBL_SEQID_2_M0_WRITE_ID_8_HID_2:\n"
			"fltm.pi m4, f26, f29\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x5, rand_ieee754_32\n"
			"flw.ps f23, 896(x5)\n"
			"flw.ps f31, 0(x5)\n"
			"flw.ps f17, 64(x5)\n"
			"fmsub.ps f1, f23, f31, f17, rup\n" 
			"la x15, rand_int_32\n"
			"flw.ps f18, 160(x15)\n"
			"flw.ps f30, 320(x15)\n"
			"feq.pi f1, f18, f30\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_2_XMA_QNT_STR_SEQ_ITER_0_HID_2:\n"
			"li x5, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x2 << TENSOR_IMA_B_NUM_COLS) | (0xa << TENSOR_IMA_A_NUM_ROWS) | (0x1 << TENSOR_IMA_A_NUM_COLS) | (0x5 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0xa0 << TENSOR_IMA_SCP_LOC_B) | (0x7d << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x0 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
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
			"la x5, rand_ieee754_32\n"
			"flw.ps f6, 576(x5)\n"
			"fsqrt.s f2, f6, rne\n" 
			"la x11, rand_int_32\n"
			"flw.ps f8, 288(x11)\n"
			"fandi.pi f17, f8, 0x19a\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f29, 992(x28)\n"
			"fswizz.ps f21, f29, 0x13\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f6, 288(x10)\n"
			"flw.ps f21, 96(x10)\n"
			"fmax.s f2, f6, f21\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f8, 832(x23)\n"
			"ffrc.ps f1, f8\n" 
			"li x5, 0x84b8\n"
			"csrw tensor_mask, x5\n"	
			"la x28, rand_ieee754_32\n"
			"flw.ps f29, 160(x28)\n"
			"flw.ps f18, 352(x28)\n"
			"fsgnj.s f18, f29, f18\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f31, 32(x28)\n"
			"flw.ps f21, 0(x28)\n"
			"fsgnjx.ps f23, f31, f21\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f0, 928(x28)\n"
			"flw.ps f12, 928(x28)\n"
			"fle.s x23, f0, f12\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"LBL_SEQID_3_FP_TRANS_ID_9_HID_2:\n"
			"la x16, rand_ieee754_32\n"
			"flw.ps f23, 992(x16)\n"
			"flog.ps f28, f23\n" 
			"la x11, rand_int_32\n"
			"flw.ps f1, 576(x11)\n"
			"fmulhu.pi f30, f1, f1\n" 
			"li x5, 0x55bb\n"
			"csrw tensor_mask, x5\n"	
			"LBL_SEQID_3_M0_WRITE_ID_10_HID_2:\n"
			"fltm.pi m2, f0, f26\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x26, rand_ieee754_32\n"
			"flw.ps f23, 416(x26)\n"
			"fcvt.wu.s x11, f23, rne\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f29, 288(x5)\n"
			"flw.ps f31, 896(x5)\n"
			"fmul.ps f6, f29, f31, rdn\n" 
			"la x26, rand_int_32\n"
			"flw.ps f9, 640(x26)\n"
			"flw.ps f1, 224(x26)\n"
			"fand.pi f21, f9, f1\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f21, 96(x11)\n"
			"flw.ps f29, 224(x11)\n"
			"flw.ps f9, 448(x11)\n"
			"fmsub.ps f26, f21, f29, f9, rne\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f30, 992(x16)\n"
			"flw.ps f0, 992(x16)\n"
			"fsgnj.ps f6, f30, f0\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_3_XMA_QNT_STR_SEQ_ID_121081_HID_2:\n"
			"li x6, ((0x8 << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0x9 << TENSOR_QUANT_QUANT_ROW) | (0x1f << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x2 << TENSOR_QUANT_TRANSF8) | (0x7 << TENSOR_QUANT_TRANSF7) | (0x7 << TENSOR_QUANT_TRANSF6) | (0x6 << TENSOR_QUANT_TRANSF5) | (0x7 << TENSOR_QUANT_TRANSF4) | (0x6 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_3_XMA_QNT_STR_SEQ_ID_923276_HID_2:\n"
			"li x6, ((0xf << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0xb << TENSOR_QUANT_QUANT_ROW) | (0x2b << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x2 << TENSOR_QUANT_TRANSF7) | (0x7 << TENSOR_QUANT_TRANSF6) | (0x6 << TENSOR_QUANT_TRANSF5) | (0x7 << TENSOR_QUANT_TRANSF4) | (0x6 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_2\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_3_TLD_XMA_STR_SEQ_ITER_0_HID_2:\n"
			"la x31, self_check_8\n"
			"li x5, 0x8\n"
			"add x5, x5, x31\n"
			"li x6, 0x36000000000000a\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x2 << TENSOR_IMA_B_NUM_COLS) | (0x8 << TENSOR_IMA_A_NUM_ROWS) | (0xa << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1b << TENSOR_IMA_SCP_LOC_B) | (0x0 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fmvz.x.ps x31, f0, 0\n"
			"li x5, 0x2c\n"
			"bne x5, x31, LBL_FAIL_HID_2\n"
			"LBL_SEQID_3_TLD_XMA_STR_SEQ_ITER_1_HID_2:\n"
			"la x31, self_check_8\n"
			"li x5, 0x80000000000009\n"
			"add x5, x5, x31\n"
			"li x6, 0x10000000000008\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x3 << TENSOR_IMA_B_NUM_COLS) | (0x9 << TENSOR_IMA_A_NUM_ROWS) | (0x8 << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x1 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1e << TENSOR_IMA_SCP_LOC_B) | (0x4 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x9, ((0x1b << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0x9 << TENSOR_QUANT_QUANT_ROW) | (0x25 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x1 << TENSOR_QUANT_TRANSF3) | (0x5 << TENSOR_QUANT_TRANSF2) | (0x4 << TENSOR_QUANT_TRANSF1) | (0x3 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fmvz.x.ps x31, f0, 0\n"
			"li x5, 0x24\n"
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
			"la x25, rand_int_32\n"
			"flw.ps f18, 224(x25)\n"
			"fcvt.ps.pw f9, f18, rup\n" 
			"la x10, rand_int_32\n"
			"flw.ps f28, 608(x10)\n"
			"flw.ps f29, 896(x10)\n"
			"for.pi f18, f28, f29\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f0, 256(x28)\n"
			"fsin.ps f23, f0\n" 
			"LBL_SEQID_4_M0_WRITE_ID_11_HID_2:\n"
			"fltm.ps m4, f31, f18\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x23, rand_int_32\n"
			"flw.ps f8, 448(x23)\n"
			"flw.ps f30, 384(x23)\n"
			"fmulh.pi f17, f8, f30\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f17, 384(x15)\n"
			"flw.ps f0, 448(x15)\n"
			"fsub.ps f28, f17, f0, rdn\n" 
			"LBL_SEQID_4_M0_WRITE_ID_12_HID_2:\n"
			"fltm.pi m7, f6, f12\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x23, rand_int_32\n"
			"flw.ps f8, 928(x23)\n"
			"flw.ps f21, 32(x23)\n"
			"feq.pi f17, f8, f21\n" 
			"maskpopc x10, m2\n" 
			"la x25, rand_int_32\n"
			"flw.ps f12, 96(x25)\n"
			"fcvt.ps.pwu f9, f12, rdn\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f12, 352(x5)\n"
			"flw.ps f18, 736(x5)\n"
			"flt.ps f30, f12, f18\n" 
			"la x26, rand_int_32\n"
			"flw.ps f28, 224(x26)\n"
			"flw.ps f18, 416(x26)\n"
			"fmulh.pi f28, f28, f18\n" 
			"li x11, 0x32eafbcc172c98b8\n"
			"fcvt.s.l f0, x11, rmm\n" 
			"la x26, rand_int_32\n"
			"flw.ps f12, 416(x26)\n"
			"flw.ps f21, 32(x26)\n"
			"fadd.pi f0, f12, f21\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f18, 384(x28)\n"
			"fswizz.ps f21, f18, 0x1a\n" 
			"LBL_SEQID_4_M0_WRITE_ID_13_HID_2:\n"
			"feqm.ps m2, f0, f23\n" 
			"li x5, CREDINC0\n"
			"ld x5, 0(x5)\n"
			"csrw tensor_error, zero\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"LBL_SEQID_4_XMA_QNT_STR_SEQ_ITER_0_HID_2:\n"
			"li x5, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x1 << TENSOR_FMA16_B_NUM_COLS) | (0x1 << TENSOR_FMA16_A_NUM_ROWS) | (0xd << TENSOR_FMA16_A_NUM_COLS) | (0x6 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0xd9 << TENSOR_FMA16_SCP_LOC_B) | (0x6b << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x0 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
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
			"csrwi mhpmevent3, 24\n"   
			"csrwi mhpmevent4, 1\n"   
			"csrwi mhpmevent5, 2\n"   
			"csrwi mhpmevent6, 8\n"   
			"csrwi mhpmevent7, 1\n"   
			"csrwi mhpmevent8, 2\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f23,  928 (x5)\n"
			"flw.ps f12,  992 (x5)\n"
			"flw.ps f18,  32 (x5)\n"
			"flw.ps f26,  416 (x5)\n"
			"flw.ps f31,  512 (x5)\n"
			"flw.ps f28,  0 (x5)\n"
			"flw.ps f13,  192 (x5)\n"
			"flw.ps f14,  576 (x5)\n"
			"flw.ps f6,  704 (x5)\n"
			"flw.ps f15,  384 (x5)\n"
			"flw.ps f25,  352 (x5)\n"
			"flw.ps f8,  832 (x5)\n"
			"flw.ps f9,  480 (x5)\n"
			"flw.ps f22,  128 (x5)\n"
			"flw.ps f1,  640 (x5)\n"
			"flw.ps f11,  544 (x5)\n"
			"flw.ps f2,  448 (x5)\n"
			"flw.ps f19,  608 (x5)\n"
			"flw.ps f24,  800 (x5)\n"
			"flw.ps f4,  960 (x5)\n"
			"flw.ps f29,  288 (x5)\n"
			"flw.ps f27,  64 (x5)\n"
			"flw.ps f7,  320 (x5)\n"
			"flw.ps f5,  736 (x5)\n"
			"flw.ps f3,  768 (x5)\n"
			"flw.ps f0,  864 (x5)\n"
			"flw.ps f16,  96 (x5)\n"
			"flw.ps f30,  896 (x5)\n"
			"flw.ps f10,  256 (x5)\n"
			"flw.ps f17,  160 (x5)\n"
			"flw.ps f21,  672 (x5)\n"
			"flw.ps f20,  224 (x5)\n"
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
			"li x29, 0x4\n"
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
			"li x10, 0x96d141b12272dd37\n"
			"fcvt.s.l f25, x10, rne\n" 
			"la x10, rand_int_32\n"
			"flw.ps f25, 256(x10)\n"
			"fslli.pi f30, f25, 0xd\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f22, 288(x13)\n"
			"flw.ps f26, 704(x13)\n"
			"fsgnjx.s f6, f22, f26\n" 
			"la x26, rand_int_32\n"
			"flw.ps f7, 160(x26)\n"
			"fcvt.ps.pw f22, f7, rmm\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f24, 928(x9)\n"
			"flw.ps f25, 672(x9)\n"
			"flw.ps f4, 352(x9)\n"
			"fnmsub.s f6, f24, f25, f4, rmm\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f22, 992(x28)\n"
			"flw.ps f15, 704(x28)\n"
			"fmul.s f5, f22, f15, rdn\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f28, 416(x27)\n"
			"flw.ps f6, 992(x27)\n"
			"flw.ps f11, 480(x27)\n"
			"fnmsub.s f3, f28, f6, f11, rne\n" 
			"la x14, rand_int_32\n"
			"flw.ps f14, 480(x14)\n"
			"fsat8.pi f7, f14\n" 
			"la x9, rand_int_32\n"
			"flw.ps f4, 896(x9)\n"
			"fnot.pi f26, f4\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f8, 864(x5)\n"
			"flw.ps f15, 96(x5)\n"
			"flw.ps f22, 768(x5)\n"
			"fnmadd.s f3, f8, f15, f22, rmm\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f11, 448(x26)\n"
			"fcvt.f16.ps f0, f11\n" 
			"li x13, 0xd015f85deb8a36bd\n"
			"fcvt.s.wu f6, x13, rup\n" 
			"mova.x.m x23\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f15, 544(x13)\n"
			"flw.ps f25, 416(x13)\n"
			"flw.ps f14, 288(x13)\n"
			"fmadd.s f28, f15, f25, f14, rtz\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f3, 672(x14)\n"
			"flw.ps f4, 832(x14)\n"
			"fsub.s f0, f3, f4, rmm\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f26, 864(x19)\n"
			"fcvt.w.s x5, f26, rtz\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_3\n"
			"addi x30, x30, 1\n"
			"la x27, rand_ieee754_32\n"
			"flw.ps f26, 864(x27)\n"
			"fswizz.ps f30, f26, 0x1b\n" 
			"la x23, rand_int_32\n"
			"flw.ps f6, 96(x23)\n"
			"flw.ps f3, 672(x23)\n"
			"fsub.pi f26, f6, f3\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f24, 64(x19)\n"
			"flw.ps f30, 928(x19)\n"
			"fnmsub.s f5, f24, f30, f30, rmm\n" 
			"LBL_SEQID_1_M0_WRITE_ID_0_HID_3:\n"
			"fltm.pi m7, f15, f28\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x10, rand_int_32\n"
			"flw.ps f11, 32(x10)\n"
			"fsat8.pi f11, f11\n" 
			"LBL_SEQID_1_M0_WRITE_ID_1_HID_3:\n"
			"maskand m7, m5, m7\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x10, rand_ieee754_32\n"
			"flw.ps f30, 736(x10)\n"
			"flw.ps f28, 320(x10)\n"
			"fcmovm.ps f24, f30, f28\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f0, 704(x13)\n"
			"flw.ps f25, 416(x13)\n"
			"flw.ps f15, 672(x13)\n"
			"fmsub.s f7, f0, f25, f15, rtz\n" 
			"fbci.pi f30, 0x63b60\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f15, 704(x19)\n"
			"flw.ps f24, 704(x19)\n"
			"flw.ps f8, 864(x19)\n"
			"fcmov.ps f7, f15, f24, f8\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f15, 288(x14)\n"
			"flw.ps f22, 448(x14)\n"
			"flw.ps f14, 320(x14)\n"
			"fmadd.s f7, f15, f22, f14, rmm\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f11, 256(x10)\n"
			"frsq.ps f15, f11\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f6, 352(x19)\n"
			"fcvt.pw.ps f5, f6, rup\n" 
			"la x26, rand_int_32\n"
			"flw.ps f11, 672(x26)\n"
			"flw.ps f4, 96(x26)\n"
			"fxor.pi f3, f11, f4\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f3, 832(x19)\n"
			"flw.ps f28, 896(x19)\n"
			"flw.ps f8, 896(x19)\n"
			"fnmsub.ps f5, f3, f28, f8, rtz\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f26, 320(x19)\n"
			"flw.ps f5, 576(x19)\n"
			"fmax.s f25, f26, f5\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_3\n"
			"addi x30, x30, 1\n"
			"la x23, rand_int_32\n"
			"flw.ps f26, 512(x23)\n"
			"flw.ps f30, 0(x23)\n"
			"fsra.pi f15, f26, f30\n" 
			"la x23, rand_int_32\n"
			"flw.ps f28, 448(x23)\n"
			"flw.ps f4, 800(x23)\n"
			"fltu.pi f5, f28, f4\n" 
			"la x14, rand_int_32\n"
			"flw.ps f15, 928(x14)\n"
			"flw.ps f11, 704(x14)\n"
			"for.pi f15, f15, f11\n" 
			"la x14, rand_int_32\n"
			"flw.ps f11, 32(x14)\n"
			"fsrli.pi f28, f11, 0xd\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f7, 992(x9)\n"
			"fclass.s x23, f7\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f8, 64(x5)\n"
			"flw.ps f26, 800(x5)\n"
			"fadd.ps f7, f8, f26, rmm\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f6, 864(x28)\n"
			"flw.ps f4, 480(x28)\n"
			"fdiv.s f3, f6, f4, rup\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f26, 928(x23)\n"
			"fmax.s f0, f26, f26\n" 
			"LBL_SEQID_2_M0_WRITE_ID_2_HID_3:\n"
			"flem.ps m3, f7, f6\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x13, rand_int_32\n"
			"flw.ps f22, 0(x13)\n"
			"fandi.pi f30, f22, 0x98\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f4, 896(x28)\n"
			"fswizz.ps f26, f4, 0x18\n" 
			"la x13, rand_int_32\n"
			"flw.ps f30, 320(x13)\n"
			"faddi.pi f14, f30, 0x1d2\n" 
			"la x10, rand_int_32\n"
			"flw.ps f6, 480(x10)\n"
			"fsat8.pi f30, f6\n" 
			"la x5, rand_int_32\n"
			"flw.ps f7, 960(x5)\n"
			"flw.ps f3, 480(x5)\n"
			"fand.pi f22, f7, f3\n" 
			"LBL_SEQID_2_M0_WRITE_ID_3_HID_3:\n"
			"mova.m.x x28\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x28, rand_ieee754_32\n"
			"flw.ps f30, 736(x28)\n"
			"flw.ps f0, 320(x28)\n"
			"flw.ps f22, 416(x28)\n"
			"fnmsub.ps f14, f30, f0, f22, rtz\n" 
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
			"la x23, rand_ieee754_32\n"
			"flw.ps f26, 768(x23)\n"
			"fclass.ps f4, f26\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f3, 384(x23)\n"
			"fmax.ps f0, f3, f3\n" 
			"la x9, rand_int_32\n"
			"flw.ps f22, 736(x9)\n"
			"fsatu8.pi f15, f22\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f25, 800(x27)\n"
			"flw.ps f30, 960(x27)\n"
			"fsub.s f7, f25, f30, rup\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f5, 832(x10)\n"
			"flw.ps f30, 992(x10)\n"
			"feq.ps f28, f5, f30\n" 
			"la x28, rand_int_32\n"
			"flw.ps f15, 640(x28)\n"
			"flw.ps f30, 672(x28)\n"
			"fsra.pi f5, f15, f30\n" 
			"la x28, rand_int_32\n"
			"flw.ps f26, 352(x28)\n"
			"flw.ps f8, 0(x28)\n"
			"fmin.pi f8, f26, f8\n" 
			"la x26, rand_int_32\n"
			"flw.ps f25, 96(x26)\n"
			"flw.ps f5, 864(x26)\n"
			"feq.pi f4, f25, f5\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f22, 512(x14)\n"
			"ffrc.ps f25, f22\n" 
			"la x26, rand_int_32\n"
			"flw.ps f7, 96(x26)\n"
			"flw.ps f15, 96(x26)\n"
			"for.pi f24, f7, f15\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f8, 352(x28)\n"
			"flw.ps f4, 800(x28)\n"
			"feq.ps f0, f8, f4\n" 
			"LBL_SEQID_3_M0_WRITE_ID_4_HID_3:\n"
			"maskor m5, m3, m7\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0xb8b893ef1\n"
			"fsw.ps f0, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x9, rand_int_32\n"
			"flw.ps f28, 384(x9)\n"
			"fpackreph.pi f30, f28\n" 
			"la x19, rand_int_32\n"
			"flw.ps f6, 928(x19)\n"
			"fsrli.pi f30, f6, 0x8\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f11, 224(x13)\n"
			"fclass.ps f4, f11\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f28, 672(x26)\n"
			"flw.ps f7, 768(x26)\n"
			"flw.ps f0, 448(x26)\n"
			"fnmsub.ps f22, f28, f7, f0, rup\n" 
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
			"la x19, rand_int_32\n"
			"flw.ps f30, 320(x19)\n"
			"flw.ps f7, 640(x19)\n"
			"fsra.pi f28, f30, f7\n" 
			"la x27, rand_int_32\n"
			"flw.ps f8, 640(x27)\n"
			"fsatu8.pi f15, f8\n" 
			"fbci.ps f6, 0xddd\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f28, 640(x28)\n"
			"flw.ps f24, 640(x28)\n"
			"flt.s x19, f28, f24\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f15, 384(x14)\n"
			"flw.ps f8, 640(x14)\n"
			"fmax.s f11, f15, f8\n" 
			"li x5, 0x61f492c711573a05\n"
			"fcvt.s.l f6, x5, rtz\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f8, 416(x23)\n"
			"fsqrt.s f25, f8, rne\n" 
			"LBL_SEQID_4_M0_WRITE_ID_5_HID_3:\n"
			"fltm.ps m5, f30, f4\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0x2ed79f516d\n"
			"fsw.ps f0, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x23, rand_int_32\n"
			"flw.ps f24, 288(x23)\n"
			"flw.ps f25, 224(x23)\n"
			"fmulh.pi f4, f24, f25\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f4, 192(x14)\n"
			"fcvt.pw.ps f14, f4, rmm\n" 
			"la x14, rand_int_32\n"
			"flw.ps f22, 544(x14)\n"
			"fsrai.pi f26, f22, 0x5\n" 
			"la x26, rand_int_32\n"
			"flw.ps f6, 448(x26)\n"
			"flw.ps f28, 384(x26)\n"
			"fmin.pi f11, f6, f28\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f7, 576(x28)\n"
			"flw.ps f4, 704(x28)\n"
			"flt.ps f14, f7, f4\n" 
			"la x27, rand_int_32\n"
			"flw.ps f11, 672(x27)\n"
			"flw.ps f7, 0(x27)\n"
			"fltu.pi f25, f11, f7\n" 
			"la x28, rand_int_32\n"
			"flw.ps f4, 864(x28)\n"
			"flw.ps f30, 672(x28)\n"
			"feq.pi f30, f4, f30\n" 
			"fbci.pi f30, 0x2a294\n" 
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
			"csrwi mhpmevent3, 8\n"   
			"csrwi mhpmevent4, 4\n"   
			"csrwi mhpmevent5, 21\n"   
			"csrwi mhpmevent6, 6\n"   
			"csrwi mhpmevent7, 4\n"   
			"csrwi mhpmevent8, 5\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f26,  672 (x5)\n"
			"flw.ps f6,  320 (x5)\n"
			"flw.ps f23,  192 (x5)\n"
			"flw.ps f14,  896 (x5)\n"
			"flw.ps f21,  256 (x5)\n"
			"flw.ps f16,  512 (x5)\n"
			"flw.ps f9,  96 (x5)\n"
			"flw.ps f15,  608 (x5)\n"
			"flw.ps f17,  64 (x5)\n"
			"flw.ps f8,  448 (x5)\n"
			"flw.ps f30,  224 (x5)\n"
			"flw.ps f10,  736 (x5)\n"
			"flw.ps f13,  576 (x5)\n"
			"flw.ps f27,  384 (x5)\n"
			"flw.ps f25,  960 (x5)\n"
			"flw.ps f11,  704 (x5)\n"
			"flw.ps f7,  800 (x5)\n"
			"flw.ps f31,  992 (x5)\n"
			"flw.ps f12,  640 (x5)\n"
			"flw.ps f20,  288 (x5)\n"
			"flw.ps f18,  128 (x5)\n"
			"flw.ps f0,  768 (x5)\n"
			"flw.ps f28,  32 (x5)\n"
			"flw.ps f1,  928 (x5)\n"
			"flw.ps f3,  0 (x5)\n"
			"flw.ps f29,  480 (x5)\n"
			"flw.ps f5,  544 (x5)\n"
			"flw.ps f4,  864 (x5)\n"
			"flw.ps f19,  352 (x5)\n"
			"flw.ps f24,  160 (x5)\n"
			"flw.ps f22,  416 (x5)\n"
			"flw.ps f2,  832 (x5)\n"
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
			"li x29, 0x4\n"
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
			"la x12, rand_int_32\n"
			"flw.ps f7, 256(x12)\n"
			"fsrai.pi f7, f7, 0x7\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f17, 96(x26)\n"
			"flw.ps f1, 64(x26)\n"
			"flw.ps f7, 96(x26)\n"
			"fmadd.ps f16, f17, f1, f7, rne\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_4:\n"
			"mov.m.x m5, x27, 0x55\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"li x5, 0xd67\n"
			"csrw tensor_mask, x5\n"	
			"LBL_SEQID_0_FP_TRANS_ID_1_HID_4:\n"
			"la x7, rand_ieee754_32\n"
			"flw.ps f13, 416(x7)\n"
			"frcp.ps f13, f13\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f13, 672(x7)\n"
			"flw.ps f7, 928(x7)\n"
			"flw.ps f29, 128(x7)\n"
			"fmadd.s f24, f13, f7, f29, rup\n" 
			"li x5, 0x23935d90edb1a3f1\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0x6f30eed903f508f1\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"la x13, rand_ieee754_32\n"
			"flw.ps f18, 512(x13)\n"
			"flw.ps f8, 672(x13)\n"
			"fsgnj.ps f24, f18, f8\n" 
			"la x19, rand_int_32\n"
			"flw.ps f24, 384(x19)\n"
			"flw.ps f30, 96(x19)\n"
			"fand.pi f8, f24, f30\n" 
			"la x12, rand_int_32\n"
			"flw.ps f1, 608(x12)\n"
			"fsatu8.pi f29, f1\n" 
			"la x26, rand_ieee754_16\n"
			"flw.ps f13, 128(x26)\n"
			"fcvt.ps.f16 f6, f13\n" 
			"la x12, rand_int_32\n"
			"flw.ps f29, 224(x12)\n"
			"fpackrepb.pi f24, f29\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f15, 704(x17)\n"
			"flw.ps f29, 320(x17)\n"
			"feq.ps f24, f15, f29\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f8, 64(x16)\n"
			"flw.ps f26, 256(x16)\n"
			"fsub.ps f18, f8, f26, rmm\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f29, 672(x14)\n"
			"fcvt.wu.s x19, f29, rtz\n" 
			"la x13, rand_int_32\n"
			"flw.ps f15, 224(x13)\n"
			"flw.ps f17, 992(x13)\n"
			"fmax.pi f1, f15, f17\n" 
			"la x6, rand_int_32\n"
			"flw.ps f15, 192(x6)\n"
			"flw.ps f17, 128(x6)\n"
			"fle.pi f29, f15, f17\n" 
			"la x26, rand_int_32\n"
			"flw.ps f13, 704(x26)\n"
			"flw.ps f3, 224(x26)\n"
			"fxor.pi f6, f13, f3\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ID_447893_HID_4:\n"
			"la x31, self_check_8\n"
			"li x5, 0x40000000000008\n"
			"add x5, x5, x31\n"
			"li x6, 0x10000000000001\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x1 << TENSOR_IMA_B_NUM_COLS) | (0x8 << TENSOR_IMA_A_NUM_ROWS) | (0x1 << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x1 << TENSOR_IMA_TEN_B_IN_MEM) | (0x19 << TENSOR_IMA_SCP_LOC_B) | (0x2 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fmvz.x.ps x31, f0, 0\n"
			"li x5, 0x8\n"
			"bne x5, x31, LBL_FAIL_HID_4\n"
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ID_745572_HID_4:\n"
			"la x31, self_check_16\n"
			"li x5, 0xe0000000000001\n"
			"add x5, x5, x31\n"
			"li x6, 0x1000000000000a\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x1 << TENSOR_FMA16_B_NUM_COLS) | (0x1 << TENSOR_FMA16_A_NUM_ROWS) | (0xa << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x1 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1f << TENSOR_FMA16_SCP_LOC_B) | (0x7 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x3 << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0x1 << TENSOR_QUANT_QUANT_ROW) | (0x3a << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x0 << TENSOR_QUANT_TRANSF1) | (0x2 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0x16\n"
			"bne x5, x31, LBL_FAIL_HID_4\n"
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 10\n"
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ID_52118_HID_4:\n"
			"la x31, self_check_32\n"
			"li x5, 0x20000000000001\n"
			"add x5, x5, x31\n"
			"li x6, 0x1000000000000c\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x2 << TENSOR_FMA32_B_NUM_COLS) | (0x1 << TENSOR_FMA32_A_NUM_ROWS) | (0xc << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x1 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x19 << TENSOR_FMA32_SCP_LOC_B) | (0x1 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x4 << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0x1 << TENSOR_QUANT_QUANT_ROW) | (0x3f << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x7 << TENSOR_QUANT_TRANSF5) | (0x7 << TENSOR_QUANT_TRANSF4) | (0x6 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0xd\n"
			"bne x5, x31, LBL_FAIL_HID_4\n"
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_4\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_0_RDX_ID_0_SHIRE_0_HID_4:\n"
			"li x17, 0xc00000002640002\n"
			"csrw tensor_reduce, x17\n" 
			"csrwi tensor_wait, 9\n" 
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=0); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ITER_0_HID_4:\n"
			"li x5, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x2 << TENSOR_FMA32_B_NUM_COLS) | (0x9 << TENSOR_FMA32_A_NUM_ROWS) | (0x2 << TENSOR_FMA32_A_NUM_COLS) | (0xb << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x16 << TENSOR_FMA32_SCP_LOC_B) | (0xab << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x0 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ITER_1_HID_4:\n"
			"li x5, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x3 << TENSOR_IMA_B_NUM_COLS) | (0xf << TENSOR_IMA_A_NUM_ROWS) | (0xc << TENSOR_IMA_A_NUM_COLS) | (0x7 << TENSOR_IMA_OFFSET) | (0x0 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x63 << TENSOR_IMA_SCP_LOC_B) | (0x17 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ITER_2_HID_4:\n"
			"li x5, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x2 << TENSOR_FMA16_B_NUM_COLS) | (0x5 << TENSOR_FMA16_A_NUM_ROWS) | (0xe << TENSOR_FMA16_A_NUM_COLS) | (0x8 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0xb7 << TENSOR_FMA16_SCP_LOC_B) | (0x4d << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x0 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_4\n"
			"csrw tensor_error, x0\n"
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_4\n"
			"addi x30, x30, 1\n"
			"la x14, rand_int_32\n"
			"flw.ps f8, 992(x14)\n"
			"fpackreph.pi f29, f8\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x12, rand_ieee754_32\n"
			"flw.ps f6, 640(x12)\n"
			"flw.ps f5, 96(x12)\n"
			"fmul.s f16, f6, f5, rdn\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f8, 896(x14)\n"
			"flw.ps f10, 832(x14)\n"
			"fmadd.s f7, f8, f10, f10, rmm\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f24, 96(x17)\n"
			"flw.ps f8, 224(x17)\n"
			"fmul.s f5, f24, f8, rtz\n" 
			"LBL_SEQID_1_M0_WRITE_ID_2_HID_4:\n"
			"mov.m.x m5, x19, 0x77\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x12, rand_ieee754_32\n"
			"flw.ps f30, 992(x12)\n"
			"flw.ps f24, 928(x12)\n"
			"flw.ps f6, 960(x12)\n"
			"fcmov.ps f26, f30, f24, f6\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f15, 608(x23)\n"
			"fcvt.w.s x17, f15, rne\n" 
			"li x5, 0xd5cd\n"
			"csrw tensor_mask, x5\n"	
			"la x19, rand_ieee754_32\n"
			"flw.ps f13, 32(x19)\n"
			"flw.ps f3, 256(x19)\n"
			"fmin.ps f26, f13, f3\n" 
			"li x5, 0x2f9e\n"
			"csrw tensor_mask, x5\n"	
			"li x27, 0x30603a5453a6ea76\n"
			"fcvt.s.wu f24, x27, rup\n" 
			"li x5, 0xbe0\n"
			"csrw tensor_mask, x5\n"	
			"la x23, rand_ieee754_32\n"
			"flw.ps f3, 832(x23)\n"
			"ffrc.ps f8, f3\n" 
			"LBL_SEQID_1_M0_WRITE_ID_3_HID_4:\n"
			"flem.ps m1, f29, f3\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x17, rand_ieee754_32\n"
			"flw.ps f15, 416(x17)\n"
			"fsqrt.s f6, f15, rtz\n" 
			"la x6, rand_int_32\n"
			"flw.ps f8, 384(x6)\n"
			"flw.ps f29, 128(x6)\n"
			"fsra.pi f8, f8, f29\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f29, 704(x12)\n"
			"flw.ps f24, 480(x12)\n"
			"fsub.s f8, f29, f24, rup\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f18, 992(x7)\n"
			"flw.ps f30, 448(x7)\n"
			"flw.ps f8, 704(x7)\n"
			"fcmov.ps f16, f18, f30, f8\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f6, 512(x16)\n"
			"fsub.s f3, f6, f6, rtz\n" 
			"csrw tensor_error, zero\n" 
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=0); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"LBL_SEQID_1_TLD_XMA_STR_SEQ_ITER_0_HID_4:\n"
			"la x31, self_check_32\n"
			"li x5, 0x6000000000000b\n"
			"add x5, x5, x31\n"
			"li x6, 0x38000000000000f\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x1 << TENSOR_FMA32_B_NUM_COLS) | (0xb << TENSOR_FMA32_A_NUM_ROWS) | (0xf << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1c << TENSOR_FMA32_SCP_LOC_B) | (0x3 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0x10\n"
			"bne x5, x31, LBL_FAIL_HID_4\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_4\n"
			"csrw tensor_error, x0\n"
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_4\n"
			"addi x30, x30, 1\n"
			"la x27, rand_ieee754_32\n"
			"flw.ps f10, 96(x27)\n"
			"flw.ps f6, 96(x27)\n"
			"flw.ps f8, 0(x27)\n"
			"fcmov.ps f6, f10, f6, f8\n" 
			"la x13, rand_int_32\n"
			"flw.ps f26, 960(x13)\n"
			"flw.ps f29, 704(x13)\n"
			"fdiv.pi f17, f26, f29\n" 
			"la x19, rand_int_32\n"
			"flw.ps f13, 384(x19)\n"
			"fsat8.pi f16, f13\n" 
			"la x12, rand_int_32\n"
			"flw.ps f8, 64(x12)\n"
			"flw.ps f10, 128(x12)\n"
			"fdiv.pi f15, f8, f10\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f7, 288(x16)\n"
			"flw.ps f10, 672(x16)\n"
			"feq.s x16, f7, f10\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f26, 608(x14)\n"
			"flw.ps f17, 0(x14)\n"
			"fmul.s f17, f26, f17, rtz\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"LBL_SEQID_2_M0_WRITE_ID_4_HID_4:\n"
			"feqm.ps m1, f6, f26\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"li x5, 0xf165\n"
			"csrw tensor_mask, x5\n"	
			"la x12, rand_ieee754_32\n"
			"flw.ps f7, 512(x12)\n"
			"flw.ps f18, 288(x12)\n"
			"fmax.s f29, f7, f18\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f5, 960(x6)\n"
			"fsgnjx.s f6, f5, f5\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f8, 288(x27)\n"
			"flw.ps f26, 736(x27)\n"
			"flw.ps f29, 96(x27)\n"
			"fmadd.s f10, f8, f26, f29, rne\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f3, 480(x7)\n"
			"flw.ps f16, 96(x7)\n"
			"fsub.ps f15, f3, f16, rmm\n" 
			"la x6, rand_int_32\n"
			"flw.ps f5, 256(x6)\n"
			"fandi.pi f17, f5, 0x106\n" 
			"la x26, rand_int_32\n"
			"flw.ps f15, 640(x26)\n"
			"fnot.pi f18, f15\n" 
			"li x13, 0x163b8a6f266e0053\n"
			"fcvt.s.wu f29, x13, rtz\n" 
			"la x12, rand_int_32\n"
			"flw.ps f15, 960(x12)\n"
			"fandi.pi f26, f15, 0x1b6\n" 
			"maskpopc x16, m5\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_2_TLD_XMA_STR_SEQ_ITER_0_HID_4:\n"
			"la x31, self_check_16\n"
			"li x5, 0x20000000000009\n"
			"add x5, x5, x31\n"
			"li x6, 0x360000000000006\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x3 << TENSOR_FMA16_B_NUM_COLS) | (0x9 << TENSOR_FMA16_A_NUM_ROWS) | (0x6 << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1b << TENSOR_FMA16_SCP_LOC_B) | (0x1 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
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
			"la x16, rand_int_32\n"
			"flw.ps f7, 416(x16)\n"
			"fmaxu.pi f18, f7, f7\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f18, 768(x26)\n"
			"flw.ps f10, 832(x26)\n"
			"fmax.s f7, f18, f10\n" 
			"maskpopcz x19, m1\n" 
			"LBL_SEQID_3_M0_WRITE_ID_5_HID_4:\n"
			"maskxor m5, m6, m1\n" 
			"li x5, 0x80340140\n"
			"ld x5, 0(x5)\n"
			"la x26, rand_int_32\n"
			"flw.ps f15, 448(x26)\n"
			"fnot.pi f15, f15\n" 
			"la x17, rand_int_32\n"
			"flw.ps f16, 256(x17)\n"
			"flw.ps f6, 480(x17)\n"
			"flt.pi f13, f16, f6\n" 
			"li x5, 0xc4a63b24df334d9a\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0xb4b799e9bb4e6420\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"LBL_SEQID_3_FP_TRANS_ID_6_HID_4:\n"
			"la x7, rand_ieee754_32\n"
			"flw.ps f17, 96(x7)\n"
			"fexp.ps f8, f17\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f3, 96(x27)\n"
			"flw.ps f18, 384(x27)\n"
			"fsub.s f18, f3, f18, rup\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f26, 704(x6)\n"
			"fround.ps f5, f26, rdn\n" 
			"li x5, 0x925402a792621a5e\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0xb4f04c0a8a6e55bc\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"la x14, rand_int_32\n"
			"flw.ps f6, 992(x14)\n"
			"fcvt.ps.pw f13, f6, rmm\n" 
			"mova.x.m x26\n" 
			"la x6, rand_int_32\n"
			"flw.ps f1, 96(x6)\n"
			"flw.ps f15, 736(x6)\n"
			"flt.pi f13, f1, f15\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f15, 0(x7)\n"
			"flw.ps f17, 96(x7)\n"
			"fsgnjn.s f30, f15, f17\n" 
			"la x16, rand_int_32\n"
			"flw.ps f26, 480(x16)\n"
			"flw.ps f13, 768(x16)\n"
			"flt.pi f26, f26, f13\n" 
			"LBL_SEQID_3_M0_WRITE_ID_7_HID_4:\n"
			"maskand m4, m5, m1\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x14, rand_int_32\n"
			"flw.ps f17, 672(x14)\n"
			"flw.ps f8, 256(x14)\n"
			"fltu.pi f8, f17, f8\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_3_XMA_QNT_STR_SEQ_ID_483805_HID_4:\n"
			"li x5, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x0 << TENSOR_FMA16_B_NUM_COLS) | (0x9 << TENSOR_FMA16_A_NUM_ROWS) | (0x0 << TENSOR_FMA16_A_NUM_COLS) | (0xe << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x65 << TENSOR_FMA16_SCP_LOC_B) | (0x9c << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x0 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_4\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_3_XMA_QNT_STR_SEQ_ITER_0_HID_4:\n"
			"li x5, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x1 << TENSOR_IMA_B_NUM_COLS) | (0xd << TENSOR_IMA_A_NUM_ROWS) | (0xa << TENSOR_IMA_A_NUM_COLS) | (0x3 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0xb6 << TENSOR_IMA_SCP_LOC_B) | (0x98 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x0 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
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
			"LBL_SEQID_4_M0_WRITE_ID_8_HID_4:\n"
			"maskxor m5, m5, m6\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0x735d455cf5\n"
			"flw.ps f5, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x26, rand_int_32\n"
			"flw.ps f13, 160(x26)\n"
			"fcvt.ps.pwu f13, f13, rne\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f3, 32(x26)\n"
			"fsin.ps f8, f3\n" 
			"la x19, rand_int_32\n"
			"flw.ps f13, 832(x19)\n"
			"fcvt.ps.pwu f8, f13, rmm\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f8, 672(x12)\n"
			"flw.ps f3, 96(x12)\n"
			"fnmsub.s f1, f8, f8, f3, rdn\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f26, 480(x13)\n"
			"flw.ps f18, 480(x13)\n"
			"flw.ps f10, 224(x13)\n"
			"fnmsub.ps f3, f26, f18, f10, rtz\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f29, 896(x17)\n"
			"flw.ps f3, 64(x17)\n"
			"flw.ps f8, 672(x17)\n"
			"fmsub.s f5, f29, f3, f8, rdn\n" 
			"la x27, rand_int_32\n"
			"flw.ps f5, 864(x27)\n"
			"fandi.pi f16, f5, 0x1ea\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f7, 512(x17)\n"
			"fcvt.pwu.ps f8, f7, rup\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f16, 576(x6)\n"
			"fcvt.w.s x23, f16, rdn\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f18, 768(x27)\n"
			"flw.ps f26, 896(x27)\n"
			"fmul.ps f29, f18, f26, rtz\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f16, 32(x19)\n"
			"flw.ps f10, 544(x19)\n"
			"fsgnj.s f3, f16, f10\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f10, 96(x7)\n"
			"fcvt.wu.s x16, f10, rmm\n" 
			"LBL_SEQID_4_M0_WRITE_ID_9_HID_4:\n"
			"masknot m6, m1\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x27, rand_int_32\n"
			"flw.ps f18, 448(x27)\n"
			"flw.ps f7, 704(x27)\n"
			"fmin.pi f7, f18, f7\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f16, 384(x16)\n"
			"flw.ps f18, 64(x16)\n"
			"fsgnjn.ps f17, f16, f18\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_4_XMA_QNT_STR_SEQ_ITER_0_HID_4:\n"
			"li x6, ((0x1e << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0xf << TENSOR_QUANT_QUANT_ROW) | (0x16 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_4_XMA_QNT_STR_SEQ_ITER_1_HID_4:\n"
			"li x6, ((0x16 << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0xe << TENSOR_QUANT_QUANT_ROW) | (0x7 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x0 << TENSOR_QUANT_TRANSF1) | (0x1 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
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
			"csrwi mhpmevent3, 24\n"   
			"csrwi mhpmevent4, 1\n"   
			"csrwi mhpmevent5, 2\n"   
			"csrwi mhpmevent6, 8\n"   
			"csrwi mhpmevent7, 1\n"   
			"csrwi mhpmevent8, 2\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f22,  864 (x5)\n"
			"flw.ps f3,  608 (x5)\n"
			"flw.ps f4,  32 (x5)\n"
			"flw.ps f23,  224 (x5)\n"
			"flw.ps f12,  480 (x5)\n"
			"flw.ps f19,  640 (x5)\n"
			"flw.ps f31,  736 (x5)\n"
			"flw.ps f30,  448 (x5)\n"
			"flw.ps f29,  544 (x5)\n"
			"flw.ps f28,  800 (x5)\n"
			"flw.ps f18,  832 (x5)\n"
			"flw.ps f24,  416 (x5)\n"
			"flw.ps f1,  384 (x5)\n"
			"flw.ps f0,  64 (x5)\n"
			"flw.ps f8,  896 (x5)\n"
			"flw.ps f16,  96 (x5)\n"
			"flw.ps f5,  288 (x5)\n"
			"flw.ps f14,  576 (x5)\n"
			"flw.ps f6,  192 (x5)\n"
			"flw.ps f21,  0 (x5)\n"
			"flw.ps f26,  512 (x5)\n"
			"flw.ps f9,  768 (x5)\n"
			"flw.ps f11,  992 (x5)\n"
			"flw.ps f13,  352 (x5)\n"
			"flw.ps f7,  928 (x5)\n"
			"flw.ps f25,  256 (x5)\n"
			"flw.ps f15,  128 (x5)\n"
			"flw.ps f17,  160 (x5)\n"
			"flw.ps f10,  672 (x5)\n"
			"flw.ps f27,  320 (x5)\n"
			"flw.ps f2,  704 (x5)\n"
			"flw.ps f20,  960 (x5)\n"
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
			"li x29, 0x4\n"
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
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_5:\n"
			"feqm.ps m4, f24, f19\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x15, rand_ieee754_32\n"
			"flw.ps f14, 128(x15)\n"
			"flw.ps f15, 416(x15)\n"
			"flw.ps f10, 672(x15)\n"
			"fnmsub.ps f12, f14, f15, f10, rtz\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f30, 512(x14)\n"
			"fcvt.w.s x16, f30, rmm\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f18, 864(x20)\n"
			"flw.ps f10, 448(x20)\n"
			"fcmovm.ps f23, f18, f10\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f17, 608(x28)\n"
			"flw.ps f19, 256(x28)\n"
			"flw.ps f23, 832(x28)\n"
			"fnmadd.s f19, f17, f19, f23, rdn\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f23, 896(x17)\n"
			"fcvt.f16.ps f1, f23\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f23, 224(x19)\n"
			"flw.ps f24, 608(x19)\n"
			"flw.ps f14, 864(x19)\n"
			"fnmsub.ps f18, f23, f24, f14, rdn\n" 
			"la x13, rand_int_32\n"
			"flw.ps f14, 96(x13)\n"
			"fnot.pi f12, f14\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_5:\n"
			"mov.m.x m7, x25, 0x60\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0x9fcfa12aa7\n"
			"flw.ps f5, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x25, rand_ieee754_32\n"
			"flw.ps f10, 704(x25)\n"
			"flw.ps f24, 544(x25)\n"
			"flt.ps f24, f10, f24\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f23, 256(x28)\n"
			"fcvt.f16.ps f10, f23\n" 
			"la x13, rand_int_32\n"
			"flw.ps f10, 352(x13)\n"
			"fcvt.ps.pw f14, f10, rne\n" 
			"LBL_SEQID_0_M0_WRITE_ID_2_HID_5:\n"
			"masknot m3, m7\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x17, rand_ieee754_32\n"
			"flw.ps f12, 864(x17)\n"
			"fcvt.pwu.ps f12, f12, rtz\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f22, 576(x14)\n"
			"flw.ps f16, 608(x14)\n"
			"fle.ps f19, f22, f16\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f23, 256(x15)\n"
			"flw.ps f30, 384(x15)\n"
			"fmin.s f14, f23, f30\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_5\n"
			"addi x30, x30, 1\n"
			"la x20, rand_ieee754_32\n"
			"flw.ps f14, 224(x20)\n"
			"flw.ps f6, 32(x20)\n"
			"fcmovm.ps f1, f14, f6\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f19, 800(x5)\n"
			"flw.ps f17, 992(x5)\n"
			"fdiv.ps f6, f19, f17, rup\n" 
			"la x16, rand_int_32\n"
			"flw.ps f16, 896(x16)\n"
			"flw.ps f22, 992(x16)\n"
			"for.pi f23, f16, f22\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f30, 960(x12)\n"
			"flw.ps f14, 352(x12)\n"
			"feq.s x13, f30, f14\n" 
			"LBL_SEQID_1_FP_TRANS_ID_3_HID_5:\n"
			"la x17, rand_ieee754_32\n"
			"flw.ps f19, 160(x17)\n"
			"frcp.ps f1, f19\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f12, 736(x28)\n"
			"fle.s x25, f12, f12\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f6, 768(x19)\n"
			"flw.ps f22, 480(x19)\n"
			"fadd.ps f15, f6, f22, rup\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f14, 864(x15)\n"
			"flw.ps f27, 896(x15)\n"
			"fsgnjx.ps f22, f14, f27\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f22, 64(x5)\n"
			"fcvt.f16.ps f12, f22\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f27, 224(x5)\n"
			"flw.ps f18, 864(x5)\n"
			"fsgnjn.ps f10, f27, f18\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f15, 608(x19)\n"
			"frsq.ps f14, f15\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f14, 320(x28)\n"
			"flw.ps f12, 416(x28)\n"
			"fsgnj.s f12, f14, f12\n" 
			"la x20, rand_int_32\n"
			"flw.ps f6, 992(x20)\n"
			"flw.ps f12, 416(x20)\n"
			"fadd.pi f10, f6, f12\n" 
			"LBL_SEQID_1_M0_WRITE_ID_4_HID_5:\n"
			"maskxor m4, m4, m0\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x20, rand_ieee754_32\n"
			"flw.ps f24, 608(x20)\n"
			"flw.ps f5, 608(x20)\n"
			"fle.ps f6, f24, f5\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f5, 224(x17)\n"
			"flw.ps f12, 288(x17)\n"
			"fcmovm.ps f18, f5, f12\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_5\n"
			"addi x30, x30, 1\n"
			"maskpopc x14, m0\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f14, 96(x13)\n"
			"flw.ps f16, 32(x13)\n"
			"flw.ps f30, 0(x13)\n"
			"fmadd.s f1, f14, f16, f30, rtz\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f18, 512(x20)\n"
			"flw.ps f16, 480(x20)\n"
			"flt.ps f17, f18, f16\n" 
			"LBL_SEQID_2_M0_WRITE_ID_5_HID_5:\n"
			"maskor m0, m0, m0\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_2_FP_TRANS_ID_6_HID_5:\n"
			"la x14, rand_ieee754_32\n"
			"flw.ps f19, 128(x14)\n"
			"fexp.ps f19, f19\n" 
			"la x19, rand_int_32\n"
			"flw.ps f18, 992(x19)\n"
			"fnot.pi f30, f18\n" 
			"mova.x.m x19\n" 
			"la x19, rand_ieee754_16\n"
			"flw.ps f18, 384(x19)\n"
			"fcvt.ps.f16 f19, f18\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f16, 960(x28)\n"
			"fcvt.wu.s x28, f16, rne\n" 
			"la x12, rand_int_32\n"
			"flw.ps f1, 832(x12)\n"
			"flw.ps f10, 416(x12)\n"
			"fmax.pi f14, f1, f10\n" 
			"LBL_SEQID_2_FP_TRANS_ID_7_HID_5:\n"
			"la x14, rand_ieee754_32\n"
			"flw.ps f30, 448(x14)\n"
			"frcp.ps f5, f30\n" 
			"la x20, rand_int_32\n"
			"flw.ps f22, 608(x20)\n"
			"fsat8.pi f15, f22\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f24, 544(x13)\n"
			"flw.ps f14, 800(x13)\n"
			"fmul.s f23, f24, f14, rtz\n" 
			"la x12, rand_int_32\n"
			"flw.ps f30, 480(x12)\n"
			"faddi.pi f14, f30, 0x93\n" 
			"la x20, rand_int_32\n"
			"flw.ps f10, 288(x20)\n"
			"fnot.pi f30, f10\n" 
			"LBL_SEQID_2_M0_WRITE_ID_8_HID_5:\n"
			"feqm.ps m4, f19, f24\n" 
			"mov.m.x m0, x0, 0xFF\n"
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
			"la x20, rand_ieee754_32\n"
			"flw.ps f16, 896(x20)\n"
			"flw.ps f30, 416(x20)\n"
			"fmul.s f27, f16, f30, rtz\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f1, 928(x15)\n"
			"flw.ps f6, 512(x15)\n"
			"feq.s x14, f1, f6\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f19, 640(x28)\n"
			"flw.ps f23, 512(x28)\n"
			"flw.ps f6, 928(x28)\n"
			"fnmsub.ps f30, f19, f23, f6, rmm\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f17, 416(x15)\n"
			"flw.ps f22, 288(x15)\n"
			"fsub.s f14, f17, f22, rtz\n" 
			"la x28, rand_int_32\n"
			"flw.ps f14, 832(x28)\n"
			"flw.ps f12, 672(x28)\n"
			"fmulh.pi f17, f14, f12\n" 
			"fbci.ps f12, 0x556e0\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f27, 832(x13)\n"
			"flw.ps f6, 576(x13)\n"
			"fdiv.s f19, f27, f6, rne\n" 
			"LBL_SEQID_3_M0_WRITE_ID_9_HID_5:\n"
			"feqm.ps m3, f18, f23\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x17, rand_ieee754_32\n"
			"flw.ps f27, 128(x17)\n"
			"flw.ps f18, 192(x17)\n"
			"fsgnjn.ps f24, f27, f18\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f24, 768(x17)\n"
			"flw.ps f14, 576(x17)\n"
			"flw.ps f1, 416(x17)\n"
			"fmadd.ps f27, f24, f14, f1, rdn\n" 
			"LBL_SEQID_3_M0_WRITE_ID_10_HID_5:\n"
			"mov.m.x m4, x16, 0x21\n" 
			"li x5, CREDINC2\n"
			"ld x5, 0(x5)\n"
			"la x19, rand_ieee754_32\n"
			"flw.ps f6, 768(x19)\n"
			"fsqrt.ps f27, f6\n" 
			"la x25, rand_int_32\n"
			"flw.ps f19, 928(x25)\n"
			"fandi.pi f1, f19, 0x1f0\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f1, 480(x15)\n"
			"flw.ps f17, 736(x15)\n"
			"fle.s x13, f1, f17\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f12, 672(x12)\n"
			"flw.ps f15, 736(x12)\n"
			"flw.ps f6, 192(x12)\n"
			"fmsub.s f17, f12, f15, f6, rdn\n" 
			"la x25, rand_int_32\n"
			"flw.ps f14, 896(x25)\n"
			"flw.ps f15, 544(x25)\n"
			"fadd.pi f27, f14, f15\n" 
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
			"LBL_SEQID_4_M0_WRITE_ID_11_HID_5:\n"
			"mov.m.x m3, x13, 0x2\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x25, rand_ieee754_32\n"
			"flw.ps f27, 928(x25)\n"
			"flw.ps f5, 640(x25)\n"
			"fmax.ps f14, f27, f5\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f16, 448(x25)\n"
			"fswizz.ps f5, f16, 0x1a\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f6, 736(x12)\n"
			"fround.ps f15, f6, rup\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f16, 256(x5)\n"
			"fcvt.w.s x19, f16, rup\n" 
			"la x16, rand_int_32\n"
			"flw.ps f5, 928(x16)\n"
			"flw.ps f30, 544(x16)\n"
			"fsrl.pi f16, f5, f30\n" 
			"LBL_SEQID_4_M0_WRITE_ID_12_HID_5:\n"
			"fltm.pi m7, f12, f15\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0x7bafa68f8c\n"
			"fsw.ps f0, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_4_FP_TRANS_ID_13_HID_5:\n"
			"la x20, rand_ieee754_32\n"
			"flw.ps f15, 800(x20)\n"
			"flog.ps f15, f15\n" 
			"la x5, rand_int_32\n"
			"flw.ps f17, 480(x5)\n"
			"flw.ps f14, 832(x5)\n"
			"fsra.pi f17, f17, f14\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f5, 704(x17)\n"
			"flw.ps f15, 448(x17)\n"
			"feq.ps f22, f5, f15\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f6, 384(x16)\n"
			"flw.ps f12, 160(x16)\n"
			"fmul.ps f16, f6, f12, rtz\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f22, 928(x15)\n"
			"flw.ps f24, 736(x15)\n"
			"flt.s x19, f22, f24\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f15, 320(x15)\n"
			"flw.ps f5, 64(x15)\n"
			"flw.ps f24, 480(x15)\n"
			"fnmsub.s f12, f15, f5, f24, rmm\n" 
			"la x16, rand_int_32\n"
			"flw.ps f1, 864(x16)\n"
			"fnot.pi f12, f1\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f19, 800(x20)\n"
			"fcvt.f16.ps f16, f19\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f30, 224(x15)\n"
			"fcvt.pwu.ps f18, f30, rne\n" 
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
			"csrwi mhpmevent3, 8\n"   
			"csrwi mhpmevent4, 4\n"   
			"csrwi mhpmevent5, 21\n"   
			"csrwi mhpmevent6, 6\n"   
			"csrwi mhpmevent7, 4\n"   
			"csrwi mhpmevent8, 5\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f13,  320 (x5)\n"
			"flw.ps f24,  704 (x5)\n"
			"flw.ps f14,  640 (x5)\n"
			"flw.ps f6,  480 (x5)\n"
			"flw.ps f5,  224 (x5)\n"
			"flw.ps f2,  0 (x5)\n"
			"flw.ps f29,  192 (x5)\n"
			"flw.ps f8,  960 (x5)\n"
			"flw.ps f18,  32 (x5)\n"
			"flw.ps f3,  928 (x5)\n"
			"flw.ps f26,  352 (x5)\n"
			"flw.ps f30,  672 (x5)\n"
			"flw.ps f9,  608 (x5)\n"
			"flw.ps f4,  128 (x5)\n"
			"flw.ps f0,  384 (x5)\n"
			"flw.ps f17,  288 (x5)\n"
			"flw.ps f22,  512 (x5)\n"
			"flw.ps f11,  832 (x5)\n"
			"flw.ps f7,  992 (x5)\n"
			"flw.ps f1,  544 (x5)\n"
			"flw.ps f19,  416 (x5)\n"
			"flw.ps f15,  256 (x5)\n"
			"flw.ps f23,  768 (x5)\n"
			"flw.ps f28,  736 (x5)\n"
			"flw.ps f25,  64 (x5)\n"
			"flw.ps f31,  896 (x5)\n"
			"flw.ps f16,  576 (x5)\n"
			"flw.ps f12,  96 (x5)\n"
			"flw.ps f10,  864 (x5)\n"
			"flw.ps f27,  448 (x5)\n"
			"flw.ps f21,  800 (x5)\n"
			"flw.ps f20,  160 (x5)\n"
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
			"li x29, 0x4\n"
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
			"la x5, rand_ieee754_32\n"
			"flw.ps f21, 576(x5)\n"
			"flw.ps f10, 896(x5)\n"
			"fsgnjn.ps f0, f21, f10\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f31, 768(x15)\n"
			"flw.ps f3, 960(x15)\n"
			"fmax.s f3, f31, f3\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f11, 832(x22)\n"
			"fcvt.l.s x11, f11, rup\n" 
			"la x15, rand_ieee754_16\n"
			"flw.ps f21, 448(x15)\n"
			"fcvt.ps.f16 f9, f21\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_6:\n"
			"flem.ps m4, f13, f10\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x14, rand_ieee754_32\n"
			"flw.ps f7, 832(x14)\n"
			"fsin.ps f21, f7\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_6:\n"
			"fltm.pi m1, f23, f3\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x15, rand_int_32\n"
			"flw.ps f9, 96(x15)\n"
			"faddi.pi f24, f9, 0xb\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f10, 32(x25)\n"
			"flw.ps f14, 320(x25)\n"
			"fsgnj.s f25, f10, f14\n" 
			"LBL_SEQID_0_M0_WRITE_ID_2_HID_6:\n"
			"fltm.ps m6, f1, f31\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x5, rand_ieee754_32\n"
			"flw.ps f20, 192(x5)\n"
			"flw.ps f24, 960(x5)\n"
			"fmul.ps f21, f20, f24, rtz\n" 
			"la x16, rand_int_32\n"
			"flw.ps f25, 960(x16)\n"
			"flw.ps f11, 416(x16)\n"
			"fmul.pi f0, f25, f11\n" 
			"la x23, rand_int_32\n"
			"flw.ps f21, 512(x23)\n"
			"flw.ps f23, 672(x23)\n"
			"fand.pi f14, f21, f23\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f24, 544(x15)\n"
			"flw.ps f0, 832(x15)\n"
			"flw.ps f7, 96(x15)\n"
			"fcmov.ps f26, f24, f0, f7\n" 
			"LBL_SEQID_0_M0_WRITE_ID_3_HID_6:\n"
			"fltm.pi m4, f21, f20\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x15, rand_int_32\n"
			"flw.ps f13, 448(x15)\n"
			"fsrai.pi f31, f13, 0xe\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ID_669049_HID_6:\n"
			"la x31, self_check_32\n"
			"li x5, 0x5\n"
			"add x5, x5, x31\n"
			"li x6, 0x300000000000000\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x1 << TENSOR_FMA32_B_NUM_COLS) | (0x5 << TENSOR_FMA32_A_NUM_ROWS) | (0x0 << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x18 << TENSOR_FMA32_SCP_LOC_B) | (0x0 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x9, ((0xc << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0x5 << TENSOR_QUANT_QUANT_ROW) | (0x1 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x7 << TENSOR_QUANT_TRANSF7) | (0x6 << TENSOR_QUANT_TRANSF6) | (0x7 << TENSOR_QUANT_TRANSF5) | (0x7 << TENSOR_QUANT_TRANSF4) | (0x7 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrwi tensor_wait, 0\n"
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
			"LBL_SEQID_0_RDX_ID_0_SHIRE_0_HID_6:\n"
			"li x16, 0xc00000002640002\n"
			"csrw tensor_reduce, x16\n" 
			"csrwi tensor_wait, 9\n" 
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=0); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ITER_0_HID_6:\n"
			"li x5, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x3 << TENSOR_IMA_B_NUM_COLS) | (0xd << TENSOR_IMA_A_NUM_ROWS) | (0x0 << TENSOR_IMA_A_NUM_COLS) | (0x6 << TENSOR_IMA_OFFSET) | (0x0 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0xd3 << TENSOR_IMA_SCP_LOC_B) | (0xab << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_6\n"
			"csrw tensor_error, x0\n"
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_6\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_1_M0_WRITE_ID_4_HID_6:\n"
			"flem.ps m4, f20, f23\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"li x16, 0x43ed9cc5e8e815fc\n"
			"fcvt.s.wu f31, x16, rmm\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f1, 672(x20)\n"
			"flw.ps f7, 992(x20)\n"
			"fle.s x5, f1, f7\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f13, 224(x6)\n"
			"fcvt.pw.ps f1, f13, rtz\n" 
			"la x11, rand_int_32\n"
			"flw.ps f3, 224(x11)\n"
			"flw.ps f14, 160(x11)\n"
			"fle.pi f0, f3, f14\n" 
			"la x20, rand_int_32\n"
			"flw.ps f11, 128(x20)\n"
			"flw.ps f14, 0(x20)\n"
			"fand.pi f31, f11, f14\n" 
			"li x16, 0x728b8b361f4a9f96\n"
			"fcvt.s.wu f26, x16, rdn\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f7, 704(x15)\n"
			"fcvt.wu.s x22, f7, rup\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f26, 800(x20)\n"
			"flw.ps f0, 864(x20)\n"
			"fmin.s f1, f26, f0\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f3, 608(x20)\n"
			"flw.ps f10, 352(x20)\n"
			"flw.ps f20, 416(x20)\n"
			"fmadd.s f13, f3, f10, f20, rup\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f10, 352(x22)\n"
			"fcvt.pwu.ps f9, f10, rne\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f31, 704(x20)\n"
			"flw.ps f26, 960(x20)\n"
			"feq.ps f11, f31, f26\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f26, 896(x22)\n"
			"flw.ps f14, 832(x22)\n"
			"fsgnjn.s f9, f26, f14\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f24, 448(x14)\n"
			"flw.ps f20, 416(x14)\n"
			"fsgnjx.s f23, f24, f20\n" 
			"li x22, 0x8c4598312fe52b87\n"
			"fcvt.s.wu f11, x22, rtz\n" 
			"maskpopc x6, m4\n" 
			"csrw tensor_error, zero\n" 
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=0); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ITER_0_HID_6:\n"
			"li x5, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x3 << TENSOR_IMA_B_NUM_COLS) | (0xb << TENSOR_IMA_A_NUM_ROWS) | (0x6 << TENSOR_IMA_A_NUM_COLS) | (0xa << TENSOR_IMA_OFFSET) | (0x0 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x42 << TENSOR_IMA_SCP_LOC_B) | (0x2 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x0 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ITER_1_HID_6:\n"
			"li x5, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x0 << TENSOR_FMA32_B_NUM_COLS) | (0xe << TENSOR_FMA32_A_NUM_ROWS) | (0x9 << TENSOR_FMA32_A_NUM_COLS) | (0xa << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x69 << TENSOR_FMA32_SCP_LOC_B) | (0xbe << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_6\n"
			"csrw tensor_error, x0\n"
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_6\n"
			"addi x30, x30, 1\n"
			"la x15, rand_ieee754_32\n"
			"flw.ps f26, 224(x15)\n"
			"flw.ps f3, 480(x15)\n"
			"flw.ps f13, 288(x15)\n"
			"fnmadd.s f7, f26, f3, f13, rne\n" 
			"la x25, rand_int_32\n"
			"flw.ps f10, 256(x25)\n"
			"flw.ps f31, 64(x25)\n"
			"fmin.pi f31, f10, f31\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f0, 576(x14)\n"
			"fswizz.ps f3, f0, 0x13\n" 
			"mova.x.m x23\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f23, 576(x25)\n"
			"frsq.ps f1, f23\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f25, 32(x22)\n"
			"fsin.ps f23, f25\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f0, 832(x16)\n"
			"flw.ps f1, 448(x16)\n"
			"flt.s x6, f0, f1\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f26, 896(x23)\n"
			"ffrc.ps f1, f26\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f9, 960(x15)\n"
			"flw.ps f20, 192(x15)\n"
			"fsgnj.s f23, f9, f20\n" 
			"LBL_SEQID_2_M0_WRITE_ID_5_HID_6:\n"
			"maskand m6, m2, m6\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x6, rand_int_32\n"
			"flw.ps f26, 960(x6)\n"
			"flw.ps f11, 992(x6)\n"
			"fmax.pi f10, f26, f11\n" 
			"li x5, 0x2e9c\n"
			"csrw tensor_mask, x5\n"	
			"li x14, 0xdbe55f9ed72f4640\n"
			"fcvt.s.lu f7, x14, rdn\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f10, 928(x20)\n"
			"flw.ps f25, 64(x20)\n"
			"fsgnjx.s f0, f10, f25\n" 
			"mova.x.m x22\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x23, rand_ieee754_32\n"
			"flw.ps f25, 32(x23)\n"
			"flw.ps f23, 192(x23)\n"
			"fmax.ps f0, f25, f23\n" 
			"la x20, rand_int_32\n"
			"flw.ps f13, 608(x20)\n"
			"flw.ps f24, 256(x20)\n"
			"fdivu.pi f9, f13, f24\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_2_TLD_XMA_STR_SEQ_ID_165647_HID_6:\n"
			"la x31, self_check_16\n"
			"li x5, 0xa000000000000a\n"
			"add x5, x5, x31\n"
			"li x6, 0x1000000000000f\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x3 << TENSOR_FMA16_B_NUM_COLS) | (0xa << TENSOR_FMA16_A_NUM_ROWS) | (0xf << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x1 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1c << TENSOR_FMA16_SCP_LOC_B) | (0x5 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x1d << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0xa << TENSOR_QUANT_QUANT_ROW) | (0x3a << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0x20\n"
			"bne x5, x31, LBL_FAIL_HID_6\n"
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_6\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_2_TLD_XMA_STR_SEQ_ITER_0_HID_6:\n"
			"la x31, self_check_8\n"
			"li x5, 0xc0000000000004\n"
			"add x5, x5, x31\n"
			"li x6, 0x3e0000000000006\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x3 << TENSOR_IMA_B_NUM_COLS) | (0x4 << TENSOR_IMA_A_NUM_ROWS) | (0x6 << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1f << TENSOR_IMA_SCP_LOC_B) | (0x6 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fmvz.x.ps x31, f0, 0\n"
			"li x5, 0x1c\n"
			"bne x5, x31, LBL_FAIL_HID_6\n"
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
			"LBL_SEQID_3_FP_TRANS_ID_6_HID_6:\n"
			"la x23, rand_ieee754_32\n"
			"flw.ps f31, 896(x23)\n"
			"frcp.ps f26, f31\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x22, rand_int_32\n"
			"flw.ps f3, 480(x22)\n"
			"flw.ps f1, 704(x22)\n"
			"fsub.pi f24, f3, f1\n" 
			"LBL_SEQID_3_FP_TRANS_ID_7_HID_6:\n"
			"la x6, rand_ieee754_32\n"
			"flw.ps f25, 800(x6)\n"
			"flog.ps f25, f25\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f25, 32(x22)\n"
			"flw.ps f1, 96(x22)\n"
			"feq.s x5, f25, f1\n" 
			"la x14, rand_int_32\n"
			"flw.ps f24, 192(x14)\n"
			"flw.ps f14, 160(x14)\n"
			"fxor.pi f11, f24, f14\n" 
			"la x15, rand_int_32\n"
			"flw.ps f31, 480(x15)\n"
			"flw.ps f26, 352(x15)\n"
			"fltu.pi f24, f31, f26\n" 
			"LBL_SEQID_3_M0_WRITE_ID_8_HID_6:\n"
			"flem.ps m1, f13, f14\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x6, rand_int_32\n"
			"flw.ps f9, 768(x6)\n"
			"flw.ps f7, 704(x6)\n"
			"fltu.pi f24, f9, f7\n" 
			"li x16, 0x16a435a632469e4d\n"
			"fcvt.s.lu f13, x16, rup\n" 
			"la x5, rand_int_32\n"
			"flw.ps f9, 928(x5)\n"
			"flw.ps f26, 0(x5)\n"
			"fand.pi f9, f9, f26\n" 
			"la x14, rand_int_32\n"
			"flw.ps f23, 128(x14)\n"
			"fnot.pi f23, f23\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f23, 32(x16)\n"
			"flw.ps f1, 576(x16)\n"
			"flw.ps f31, 576(x16)\n"
			"fcmov.ps f10, f23, f1, f31\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x11, rand_ieee754_32\n"
			"flw.ps f3, 768(x11)\n"
			"frsq.ps f7, f3\n" 
			"la x15, rand_int_32\n"
			"flw.ps f21, 800(x15)\n"
			"flw.ps f0, 224(x15)\n"
			"fdivu.pi f25, f21, f0\n" 
			"LBL_SEQID_3_M0_WRITE_ID_9_HID_6:\n"
			"maskand m2, m2, m6\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x23, rand_ieee754_32\n"
			"flw.ps f31, 416(x23)\n"
			"flw.ps f1, 512(x23)\n"
			"fnmsub.s f9, f31, f31, f1, rne\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 0x24\n"
			"csrw tensor_mask, x5\n"	
			"LBL_SEQID_3_TLD_XMA_STR_SEQ_ID_514778_HID_6:\n"
			"la x31, self_check_32\n"
			"li x5, 0xe000000000000f\n"
			"add x5, x5, x31\n"
			"li x6, 0x3a0000000000003\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x0 << TENSOR_FMA32_B_NUM_COLS) | (0xf << TENSOR_FMA32_A_NUM_ROWS) | (0x3 << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1d << TENSOR_FMA32_SCP_LOC_B) | (0x7 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
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
			"bne x5, x31, LBL_FAIL_HID_6\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_6\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_3_XMA_QNT_STR_SEQ_ITER_0_HID_6:\n"
			"li x5, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x3 << TENSOR_IMA_B_NUM_COLS) | (0xb << TENSOR_IMA_A_NUM_ROWS) | (0xc << TENSOR_IMA_A_NUM_COLS) | (0x6 << TENSOR_IMA_OFFSET) | (0x0 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x7d << TENSOR_IMA_SCP_LOC_B) | (0xc7 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
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
			"la x16, rand_int_32\n"
			"flw.ps f9, 640(x16)\n"
			"fsra.pi f1, f9, f9\n" 
			"LBL_SEQID_4_M0_WRITE_ID_10_HID_6:\n"
			"fltm.ps m4, f10, f13\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_4_M0_WRITE_ID_11_HID_6:\n"
			"maskor m1, m6, m1\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x16, rand_ieee754_32\n"
			"flw.ps f9, 352(x16)\n"
			"flw.ps f24, 288(x16)\n"
			"fdiv.s f23, f9, f24, rup\n" 
			"la x5, rand_int_32\n"
			"flw.ps f20, 32(x5)\n"
			"fsrai.pi f9, f20, 0xe\n" 
			"LBL_SEQID_4_FP_TRANS_ID_12_HID_6:\n"
			"la x23, rand_ieee754_32\n"
			"flw.ps f0, 64(x23)\n"
			"frcp.ps f26, f0\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f20, 864(x11)\n"
			"flw.ps f7, 320(x11)\n"
			"flt.s x20, f20, f7\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f20, 256(x5)\n"
			"flw.ps f24, 672(x5)\n"
			"flt.s x6, f20, f24\n" 
			"la x20, rand_int_32\n"
			"flw.ps f31, 608(x20)\n"
			"fsrai.pi f25, f31, 0x0\n" 
			"la x11, rand_int_32\n"
			"flw.ps f9, 992(x11)\n"
			"flw.ps f3, 160(x11)\n"
			"fsra.pi f20, f9, f3\n" 
			"la x6, rand_int_32\n"
			"flw.ps f7, 640(x6)\n"
			"flw.ps f10, 256(x6)\n"
			"fand.pi f1, f7, f10\n" 
			"la x23, rand_int_32\n"
			"flw.ps f9, 704(x23)\n"
			"flw.ps f21, 736(x23)\n"
			"fltu.pi f13, f9, f21\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f31, 96(x5)\n"
			"flw.ps f7, 768(x5)\n"
			"flw.ps f25, 896(x5)\n"
			"fnmsub.ps f26, f31, f7, f25, rdn\n" 
			"la x25, rand_int_32\n"
			"flw.ps f14, 160(x25)\n"
			"fcvt.ps.pwu f25, f14, rdn\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f26, 928(x25)\n"
			"fcvt.pwu.ps f14, f26, rup\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f14, 448(x5)\n"
			"flw.ps f24, 800(x5)\n"
			"fmax.s f0, f14, f24\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 0x925b\n"
			"csrw tensor_mask, x5\n"	
			"LBL_SEQID_4_TLD_XMA_STR_SEQ_ID_83707_HID_6:\n"
			"la x31, self_check_8\n"
			"li x5, 0x80000000000003\n"
			"add x5, x5, x31\n"
			"li x6, 0x10000000000001\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x0 << TENSOR_IMA_B_NUM_COLS) | (0x3 << TENSOR_IMA_A_NUM_ROWS) | (0x1 << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x1 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1b << TENSOR_IMA_SCP_LOC_B) | (0x4 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x9, ((0x1c << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0x3 << TENSOR_QUANT_QUANT_ROW) | (0x3e << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x1 << TENSOR_QUANT_TRANSF4) | (0x4 << TENSOR_QUANT_TRANSF3) | (0x8 << TENSOR_QUANT_TRANSF2) | (0x9 << TENSOR_QUANT_TRANSF1) | (0xa << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"LBL_SEQID_4_TLD_XMA_STR_SEQ_ID_930459_HID_6:\n"
			"la x31, self_check_16\n"
			"li x5, 0xe000000000000a\n"
			"add x5, x5, x31\n"
			"li x6, 0x36000000000000d\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x1 << TENSOR_FMA16_B_NUM_COLS) | (0xa << TENSOR_FMA16_A_NUM_ROWS) | (0xd << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1b << TENSOR_FMA16_SCP_LOC_B) | (0x7 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
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
			"LBL_SEQID_4_TLD_XMA_STR_SEQ_ID_7498_HID_6:\n"
			"la x31, self_check_8\n"
			"li x5, 0xa0000000000008\n"
			"add x5, x5, x31\n"
			"li x6, 0x10000000000006\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x0 << TENSOR_IMA_B_NUM_COLS) | (0x8 << TENSOR_IMA_A_NUM_ROWS) | (0x6 << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x1 << TENSOR_IMA_TEN_B_IN_MEM) | (0x18 << TENSOR_IMA_SCP_LOC_B) | (0x5 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x9, ((0xa << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0x8 << TENSOR_QUANT_QUANT_ROW) | (0x21 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x5 << TENSOR_QUANT_TRANSF3) | (0x4 << TENSOR_QUANT_TRANSF2) | (0x3 << TENSOR_QUANT_TRANSF1) | (0x4 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fmvz.x.ps x31, f0, 0\n"
			"li x5, 0x1c\n"
			"bne x5, x31, LBL_FAIL_HID_6\n"
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_6\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_4_XMA_QNT_STR_SEQ_ITER_0_HID_6:\n"
			"li x6, ((0x3 << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0xc << TENSOR_QUANT_QUANT_ROW) | (0x2b << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_4_XMA_QNT_STR_SEQ_ITER_1_HID_6:\n"
			"li x6, ((0x10 << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0x6 << TENSOR_QUANT_QUANT_ROW) | (0x2e << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x0 << TENSOR_QUANT_TRANSF1) | (0x0 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 10\n"
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
			"csrwi mhpmevent3, 24\n"   
			"csrwi mhpmevent4, 1\n"   
			"csrwi mhpmevent5, 2\n"   
			"csrwi mhpmevent6, 8\n"   
			"csrwi mhpmevent7, 1\n"   
			"csrwi mhpmevent8, 2\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f5,  288 (x5)\n"
			"flw.ps f18,  128 (x5)\n"
			"flw.ps f8,  64 (x5)\n"
			"flw.ps f1,  0 (x5)\n"
			"flw.ps f6,  960 (x5)\n"
			"flw.ps f19,  800 (x5)\n"
			"flw.ps f17,  928 (x5)\n"
			"flw.ps f12,  864 (x5)\n"
			"flw.ps f25,  992 (x5)\n"
			"flw.ps f13,  608 (x5)\n"
			"flw.ps f22,  768 (x5)\n"
			"flw.ps f11,  256 (x5)\n"
			"flw.ps f9,  384 (x5)\n"
			"flw.ps f21,  832 (x5)\n"
			"flw.ps f10,  896 (x5)\n"
			"flw.ps f30,  192 (x5)\n"
			"flw.ps f15,  640 (x5)\n"
			"flw.ps f29,  576 (x5)\n"
			"flw.ps f4,  160 (x5)\n"
			"flw.ps f20,  512 (x5)\n"
			"flw.ps f3,  544 (x5)\n"
			"flw.ps f24,  352 (x5)\n"
			"flw.ps f27,  448 (x5)\n"
			"flw.ps f7,  32 (x5)\n"
			"flw.ps f23,  320 (x5)\n"
			"flw.ps f16,  736 (x5)\n"
			"flw.ps f0,  416 (x5)\n"
			"flw.ps f31,  96 (x5)\n"
			"flw.ps f28,  704 (x5)\n"
			"flw.ps f14,  480 (x5)\n"
			"flw.ps f2,  672 (x5)\n"
			"flw.ps f26,  224 (x5)\n"
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
			"li x29, 0x4\n"
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
			"fltm.pi m1, f2, f24\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_0_FP_TRANS_ID_1_HID_7:\n"
			"la x28, rand_ieee754_32\n"
			"flw.ps f9, 64(x28)\n"
			"fexp.ps f1, f9\n" 
			"mova.x.m x20\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f18, 832(x26)\n"
			"flw.ps f24, 512(x26)\n"
			"fmin.ps f13, f18, f24\n" 
			"la x13, rand_ieee754_16\n"
			"flw.ps f24, 832(x13)\n"
			"fcvt.ps.f16 f12, f24\n" 
			"li x7, 0x48e316bf035e287c\n"
			"fcvt.s.w f2, x7, rdn\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f24, 352(x28)\n"
			"flw.ps f1, 512(x28)\n"
			"fle.ps f1, f24, f1\n" 
			"la x19, rand_int_32\n"
			"flw.ps f14, 0(x19)\n"
			"fpackreph.pi f12, f14\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f8, 800(x15)\n"
			"fsgnjx.ps f10, f8, f8\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f6, 800(x26)\n"
			"flw.ps f18, 416(x26)\n"
			"fmin.ps f14, f6, f18\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f27, 608(x28)\n"
			"flw.ps f10, 928(x28)\n"
			"fmin.ps f1, f27, f10\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f1, 544(x13)\n"
			"flw.ps f2, 832(x13)\n"
			"fmax.ps f10, f1, f2\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f12, 640(x12)\n"
			"flw.ps f27, 832(x12)\n"
			"fmul.ps f1, f12, f27, rup\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f12, 192(x13)\n"
			"flw.ps f24, 352(x13)\n"
			"flw.ps f21, 32(x13)\n"
			"fnmsub.ps f9, f12, f24, f21, rdn\n" 
			"LBL_SEQID_0_M0_WRITE_ID_2_HID_7:\n"
			"mova.m.x x15\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0x947cfc9a97\n"
			"fsw.ps f0, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x15, rand_int_32\n"
			"flw.ps f11, 480(x15)\n"
			"flw.ps f24, 0(x15)\n"
			"fand.pi f4, f11, f24\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_7\n"
			"addi x30, x30, 1\n"
			"la x19, rand_int_32\n"
			"flw.ps f11, 992(x19)\n"
			"fsat8.pi f12, f11\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f24, 192(x28)\n"
			"flw.ps f8, 800(x28)\n"
			"fadd.s f4, f24, f8, rtz\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f27, 160(x12)\n"
			"fcvt.w.s x12, f27, rmm\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f18, 384(x11)\n"
			"flw.ps f21, 736(x11)\n"
			"flw.ps f1, 480(x11)\n"
			"fmadd.ps f6, f18, f21, f1, rtz\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f6, 896(x28)\n"
			"flw.ps f21, 960(x28)\n"
			"flw.ps f11, 736(x28)\n"
			"fnmadd.s f26, f6, f21, f11, rmm\n" 
			"maskpopc x26, m2\n" 
			"mova.x.m x13\n" 
			"LBL_SEQID_1_M0_WRITE_ID_3_HID_7:\n"
			"maskor m1, m2, m3\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x13, rand_ieee754_32\n"
			"flw.ps f24, 672(x13)\n"
			"fcvt.f16.ps f14, f24\n" 
			"LBL_SEQID_1_FP_TRANS_ID_4_HID_7:\n"
			"la x11, rand_ieee754_32\n"
			"flw.ps f12, 864(x11)\n"
			"flog.ps f26, f12\n" 
			"LBL_SEQID_1_M0_WRITE_ID_5_HID_7:\n"
			"masknot m1, m1\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x27, rand_int_32\n"
			"flw.ps f27, 448(x27)\n"
			"fsat8.pi f24, f27\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f2, 192(x20)\n"
			"fround.ps f9, f2, rup\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f14, 256(x11)\n"
			"flw.ps f11, 896(x11)\n"
			"flw.ps f12, 832(x11)\n"
			"fmsub.ps f6, f14, f11, f12, rup\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f4, 608(x28)\n"
			"flw.ps f9, 352(x28)\n"
			"fmax.s f12, f4, f9\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f21, 992(x14)\n"
			"flw.ps f9, 512(x14)\n"
			"fmax.ps f11, f21, f9\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_7\n"
			"addi x30, x30, 1\n"
			"la x27, rand_ieee754_32\n"
			"flw.ps f4, 288(x27)\n"
			"fcvt.pw.ps f10, f4, rmm\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f4, 960(x26)\n"
			"flw.ps f13, 640(x26)\n"
			"flt.ps f10, f4, f13\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f2, 864(x20)\n"
			"fsgnjx.ps f14, f2, f2\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f8, 320(x15)\n"
			"flw.ps f12, 288(x15)\n"
			"fsgnjx.s f9, f8, f12\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f11, 480(x15)\n"
			"flw.ps f27, 480(x15)\n"
			"fmsub.ps f8, f11, f27, f27, rup\n" 
			"LBL_SEQID_2_M0_WRITE_ID_6_HID_7:\n"
			"maskxor m2, m3, m1\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x28, rand_int_32\n"
			"flw.ps f13, 320(x28)\n"
			"flw.ps f9, 960(x28)\n"
			"fmax.pi f6, f13, f9\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f24, 960(x13)\n"
			"flw.ps f18, 224(x13)\n"
			"flw.ps f2, 352(x13)\n"
			"fcmov.ps f8, f24, f18, f2\n" 
			"la x13, rand_int_32\n"
			"flw.ps f2, 608(x13)\n"
			"flw.ps f14, 768(x13)\n"
			"flt.pi f21, f2, f14\n" 
			"la x12, rand_int_32\n"
			"flw.ps f13, 928(x12)\n"
			"fcvt.ps.pw f10, f13, rup\n" 
			"LBL_SEQID_2_FP_TRANS_ID_7_HID_7:\n"
			"la x26, rand_ieee754_32\n"
			"flw.ps f1, 768(x26)\n"
			"fexp.ps f18, f1\n" 
			"la x27, rand_int_32\n"
			"flw.ps f2, 960(x27)\n"
			"flw.ps f13, 96(x27)\n"
			"fdivu.pi f1, f2, f13\n" 
			"la x7, rand_int_32\n"
			"flw.ps f12, 320(x7)\n"
			"flw.ps f4, 320(x7)\n"
			"fsub.pi f11, f12, f4\n" 
			"LBL_SEQID_2_M0_WRITE_ID_8_HID_7:\n"
			"masknot m1, m3\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x7, rand_ieee754_32\n"
			"flw.ps f12, 832(x7)\n"
			"flw.ps f14, 864(x7)\n"
			"flw.ps f9, 864(x7)\n"
			"fmadd.ps f4, f12, f14, f9, rmm\n" 
			"la x19, rand_int_32\n"
			"flw.ps f13, 64(x19)\n"
			"faddi.pi f26, f13, 0x125\n" 
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
			"la x11, rand_ieee754_32\n"
			"flw.ps f9, 160(x11)\n"
			"flw.ps f8, 480(x11)\n"
			"fsgnj.s f8, f9, f8\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f2, 704(x11)\n"
			"flw.ps f13, 384(x11)\n"
			"flw.ps f26, 512(x11)\n"
			"fnmsub.ps f10, f2, f13, f26, rne\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f9, 704(x27)\n"
			"flw.ps f24, 672(x27)\n"
			"fcmovm.ps f14, f9, f24\n" 
			"la x26, rand_int_32\n"
			"flw.ps f1, 608(x26)\n"
			"flw.ps f4, 512(x26)\n"
			"fmaxu.pi f26, f1, f4\n" 
			"LBL_SEQID_3_FP_TRANS_ID_9_HID_7:\n"
			"la x15, rand_ieee754_32\n"
			"flw.ps f11, 992(x15)\n"
			"frcp.ps f11, f11\n" 
			"LBL_SEQID_3_M0_WRITE_ID_10_HID_7:\n"
			"fltm.pi m3, f11, f8\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"maskpopc x7, m4\n" 
			"LBL_SEQID_3_M0_WRITE_ID_11_HID_7:\n"
			"maskor m1, m4, m3\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x11, rand_ieee754_32\n"
			"flw.ps f8, 512(x11)\n"
			"flw.ps f1, 64(x11)\n"
			"flt.s x14, f8, f1\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f10, 832(x19)\n"
			"fsgnj.s f26, f10, f10\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f2, 768(x20)\n"
			"ffrc.ps f21, f2\n" 
			"LBL_SEQID_3_M0_WRITE_ID_12_HID_7:\n"
			"mova.m.x x26\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x14, rand_int_32\n"
			"flw.ps f24, 480(x14)\n"
			"flw.ps f13, 864(x14)\n"
			"fmax.pi f13, f24, f13\n" 
			"LBL_SEQID_3_M0_WRITE_ID_13_HID_7:\n"
			"flem.ps m4, f8, f9\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x13, rand_ieee754_32\n"
			"flw.ps f11, 960(x13)\n"
			"flw.ps f14, 480(x13)\n"
			"flw.ps f10, 576(x13)\n"
			"fmadd.s f2, f11, f14, f10, rdn\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f6, 768(x7)\n"
			"flw.ps f10, 800(x7)\n"
			"fcmov.ps f6, f6, f6, f10\n" 
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
			"la x27, rand_ieee754_32\n"
			"flw.ps f11, 512(x27)\n"
			"fsin.ps f13, f11\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f1, 192(x14)\n"
			"flw.ps f9, 320(x14)\n"
			"fcmovm.ps f1, f1, f9\n" 
			"la x19, rand_int_32\n"
			"flw.ps f2, 128(x19)\n"
			"fsrli.pi f4, f2, 0x0\n" 
			"la x28, rand_int_32\n"
			"flw.ps f12, 704(x28)\n"
			"flw.ps f6, 608(x28)\n"
			"fmax.pi f8, f12, f6\n" 
			"la x13, rand_int_32\n"
			"flw.ps f8, 64(x13)\n"
			"flw.ps f10, 896(x13)\n"
			"fsub.pi f1, f8, f10\n" 
			"LBL_SEQID_4_M0_WRITE_ID_14_HID_7:\n"
			"maskor m4, m1, m2\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x12, rand_ieee754_32\n"
			"flw.ps f4, 0(x12)\n"
			"flw.ps f11, 864(x12)\n"
			"fmin.ps f1, f4, f11\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f11, 512(x20)\n"
			"fclass.ps f6, f11\n" 
			"la x20, rand_int_32\n"
			"flw.ps f2, 128(x20)\n"
			"fsrli.pi f8, f2, 0xc\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f1, 832(x7)\n"
			"flw.ps f10, 672(x7)\n"
			"flw.ps f9, 736(x7)\n"
			"fnmsub.s f21, f1, f10, f9, rup\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f21, 256(x13)\n"
			"flw.ps f9, 928(x13)\n"
			"fsgnj.s f1, f21, f9\n" 
			"la x13, rand_int_32\n"
			"flw.ps f4, 288(x13)\n"
			"flw.ps f27, 576(x13)\n"
			"fle.pi f12, f4, f27\n" 
			"la x27, rand_int_32\n"
			"flw.ps f8, 768(x27)\n"
			"flw.ps f12, 352(x27)\n"
			"flt.pi f9, f8, f12\n" 
			"LBL_SEQID_4_M0_WRITE_ID_15_HID_7:\n"
			"mov.m.x m1, x15, 0x10\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x11, rand_ieee754_32\n"
			"flw.ps f12, 224(x11)\n"
			"flw.ps f4, 992(x11)\n"
			"fsgnjx.ps f27, f12, f4\n" 
			"la x11, rand_int_32\n"
			"flw.ps f6, 256(x11)\n"
			"flw.ps f13, 64(x11)\n"
			"fmin.pi f21, f6, f13\n" 
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
			"csrwi mhpmevent3, 8\n"   
			"csrwi mhpmevent4, 4\n"   
			"csrwi mhpmevent5, 21\n"   
			"csrwi mhpmevent6, 6\n"   
			"csrwi mhpmevent7, 4\n"   
			"csrwi mhpmevent8, 5\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f13,  928 (x5)\n"
			"flw.ps f25,  640 (x5)\n"
			"flw.ps f7,  384 (x5)\n"
			"flw.ps f27,  320 (x5)\n"
			"flw.ps f19,  160 (x5)\n"
			"flw.ps f14,  576 (x5)\n"
			"flw.ps f28,  864 (x5)\n"
			"flw.ps f12,  288 (x5)\n"
			"flw.ps f16,  0 (x5)\n"
			"flw.ps f10,  512 (x5)\n"
			"flw.ps f29,  704 (x5)\n"
			"flw.ps f5,  832 (x5)\n"
			"flw.ps f6,  352 (x5)\n"
			"flw.ps f26,  480 (x5)\n"
			"flw.ps f8,  32 (x5)\n"
			"flw.ps f24,  64 (x5)\n"
			"flw.ps f15,  416 (x5)\n"
			"flw.ps f2,  192 (x5)\n"
			"flw.ps f20,  544 (x5)\n"
			"flw.ps f22,  896 (x5)\n"
			"flw.ps f3,  992 (x5)\n"
			"flw.ps f17,  96 (x5)\n"
			"flw.ps f1,  256 (x5)\n"
			"flw.ps f9,  448 (x5)\n"
			"flw.ps f23,  736 (x5)\n"
			"flw.ps f0,  768 (x5)\n"
			"flw.ps f21,  608 (x5)\n"
			"flw.ps f4,  224 (x5)\n"
			"flw.ps f18,  128 (x5)\n"
			"flw.ps f30,  960 (x5)\n"
			"flw.ps f11,  672 (x5)\n"
			"flw.ps f31,  800 (x5)\n"
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
			"li x29, 0x4\n"
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
			"maskor m7, m4, m3\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_8:\n"
			"mova.m.x x9\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x6, rand_int_32\n"
			"flw.ps f5, 608(x6)\n"
			"fsatu8.pi f13, f5\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f1, 800(x9)\n"
			"flw.ps f18, 704(x9)\n"
			"flw.ps f13, 0(x9)\n"
			"fnmadd.ps f26, f1, f18, f13, rmm\n" 
			"la x19, rand_int_32\n"
			"flw.ps f25, 224(x19)\n"
			"fsrli.pi f19, f25, 0x0\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f21, 32(x11)\n"
			"flw.ps f4, 320(x11)\n"
			"fmax.ps f1, f21, f4\n" 
			"LBL_SEQID_0_FP_TRANS_ID_2_HID_8:\n"
			"la x11, rand_ieee754_32\n"
			"flw.ps f18, 416(x11)\n"
			"fexp.ps f25, f18\n" 
			"la x9, rand_int_32\n"
			"flw.ps f4, 416(x9)\n"
			"flw.ps f11, 480(x9)\n"
			"fsub.pi f0, f4, f11\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f19, 928(x22)\n"
			"fsqrt.ps f8, f19\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f16, 512(x18)\n"
			"flw.ps f14, 832(x18)\n"
			"flw.ps f21, 544(x18)\n"
			"fmadd.s f27, f16, f14, f21, rup\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f6, 320(x9)\n"
			"flw.ps f11, 320(x9)\n"
			"flw.ps f18, 32(x9)\n"
			"fnmsub.s f27, f6, f11, f18, rne\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f0, 256(x19)\n"
			"fclass.s x27, f0\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f26, 128(x15)\n"
			"flw.ps f18, 512(x15)\n"
			"fmin.s f27, f26, f18\n" 
			"la x6, rand_int_32\n"
			"flw.ps f1, 320(x6)\n"
			"faddi.pi f5, f1, 0x1d4\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x15, rand_ieee754_32\n"
			"flw.ps f19, 256(x15)\n"
			"flw.ps f1, 160(x15)\n"
			"fsgnjx.ps f25, f19, f1\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f14, 640(x11)\n"
			"flw.ps f11, 128(x11)\n"
			"fsgnjn.s f0, f14, f11\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ID_593588_HID_8:\n"
			"la x31, self_check_8\n"
			"li x5, 0x8000000000000a\n"
			"add x5, x5, x31\n"
			"li x6, 0x3e0000000000000\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x0 << TENSOR_IMA_B_NUM_COLS) | (0xa << TENSOR_IMA_A_NUM_ROWS) | (0x0 << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1f << TENSOR_IMA_SCP_LOC_B) | (0x4 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
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
			"bne x5, x31, LBL_FAIL_HID_8\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_8\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_0_RDX_ID_0_SHIRE_0_HID_8:\n"
			"li x19, 0xc00000002640002\n"
			"csrw tensor_reduce, x19\n" 
			"csrwi tensor_wait, 9\n" 
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=0); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ITER_0_HID_8:\n"
			"la x31, self_check_16\n"
			"li x5, 0xe0000000000008\n"
			"add x5, x5, x31\n"
			"li x6, 0x10000000000008\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x2 << TENSOR_FMA16_B_NUM_COLS) | (0x8 << TENSOR_FMA16_A_NUM_ROWS) | (0x8 << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x1 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1f << TENSOR_FMA16_SCP_LOC_B) | (0x7 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x9, ((0xd << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0x8 << TENSOR_QUANT_QUANT_ROW) | (0x6 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x2 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_8\n"
			"addi x30, x30, 1\n"
			"la x22, rand_ieee754_32\n"
			"flw.ps f19, 480(x22)\n"
			"flw.ps f11, 992(x22)\n"
			"fsub.ps f16, f19, f11, rtz\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f21, 160(x19)\n"
			"fswizz.ps f6, f21, 0x1d\n" 
			"la x25, rand_int_32\n"
			"flw.ps f4, 320(x25)\n"
			"flw.ps f0, 704(x25)\n"
			"fmin.pi f26, f4, f0\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f0, 800(x6)\n"
			"fsgnjx.s f4, f0, f0\n" 
			"LBL_SEQID_1_M0_WRITE_ID_3_HID_8:\n"
			"maskxor m5, m3, m4\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x19, rand_int_32\n"
			"flw.ps f18, 832(x19)\n"
			"flw.ps f25, 640(x19)\n"
			"fsra.pi f4, f18, f25\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f19, 0(x25)\n"
			"flw.ps f27, 768(x25)\n"
			"feq.ps f5, f19, f27\n" 
			"la x15, rand_int_32\n"
			"flw.ps f13, 672(x15)\n"
			"fpackreph.pi f6, f13\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f0, 832(x18)\n"
			"flw.ps f11, 992(x18)\n"
			"flw.ps f5, 864(x18)\n"
			"fnmsub.s f0, f0, f11, f5, rne\n" 
			"LBL_SEQID_1_M0_WRITE_ID_4_HID_8:\n"
			"flem.ps m3, f14, f21\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x25, rand_int_32\n"
			"flw.ps f18, 672(x25)\n"
			"flw.ps f8, 224(x25)\n"
			"fltu.pi f6, f18, f8\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f0, 640(x6)\n"
			"flw.ps f6, 128(x6)\n"
			"flw.ps f4, 384(x6)\n"
			"fmadd.s f13, f0, f6, f4, rtz\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"li x19, 0xba5105072c66df24\n"
			"fcvt.s.wu f25, x19, rdn\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f18, 0(x27)\n"
			"fcvt.w.s x19, f18, rup\n" 
			"LBL_SEQID_1_M0_WRITE_ID_5_HID_8:\n"
			"maskand m7, m5, m3\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x27, rand_int_32\n"
			"flw.ps f4, 352(x27)\n"
			"fsat8.pi f16, f4\n" 
			"csrw tensor_error, zero\n" 
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=0); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"li x5, 0xf9742d0b3ddbecbf\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0x1ff6ffad00ce37a3\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ITER_0_HID_8:\n"
			"li x6, ((0x1f << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0x2 << TENSOR_QUANT_QUANT_ROW) | (0xf << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x5 << TENSOR_QUANT_TRANSF6) | (0x5 << TENSOR_QUANT_TRANSF5) | (0x4 << TENSOR_QUANT_TRANSF4) | (0x4 << TENSOR_QUANT_TRANSF3) | (0x3 << TENSOR_QUANT_TRANSF2) | (0x8 << TENSOR_QUANT_TRANSF1) | (0x9 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_8\n"
			"csrw tensor_error, x0\n"
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_8\n"
			"addi x30, x30, 1\n"
			"la x25, rand_ieee754_32\n"
			"flw.ps f14, 512(x25)\n"
			"fcvt.w.s x15, f14, rne\n" 
			"LBL_SEQID_2_M0_WRITE_ID_6_HID_8:\n"
			"fltm.pi m4, f8, f5\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0x37f345fa88\n"
			"fsw.ps f0, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x11, rand_ieee754_32\n"
			"flw.ps f21, 928(x11)\n"
			"flw.ps f4, 96(x11)\n"
			"flw.ps f16, 352(x11)\n"
			"fcmov.ps f16, f21, f4, f16\n" 
			"LBL_SEQID_2_M0_WRITE_ID_7_HID_8:\n"
			"maskand m7, m7, m4\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x19, rand_ieee754_32\n"
			"flw.ps f27, 0(x19)\n"
			"flw.ps f19, 704(x19)\n"
			"flw.ps f1, 832(x19)\n"
			"fmadd.s f16, f27, f19, f1, rdn\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f27, 256(x11)\n"
			"fcvt.w.s x27, f27, rmm\n" 
			"la x18, rand_int_32\n"
			"flw.ps f1, 704(x18)\n"
			"flw.ps f4, 224(x18)\n"
			"fand.pi f19, f1, f4\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f5, 128(x6)\n"
			"fclass.ps f0, f5\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f14, 864(x14)\n"
			"fcvt.f16.ps f14, f14\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f26, 640(x25)\n"
			"flw.ps f8, 800(x25)\n"
			"fadd.ps f13, f26, f8, rdn\n" 
			"LBL_SEQID_2_FP_TRANS_ID_8_HID_8:\n"
			"la x14, rand_ieee754_32\n"
			"flw.ps f11, 864(x14)\n"
			"fexp.ps f6, f11\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f18, 896(x22)\n"
			"flw.ps f6, 640(x22)\n"
			"flw.ps f19, 896(x22)\n"
			"fnmsub.ps f6, f18, f6, f19, rdn\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f4, 576(x19)\n"
			"flw.ps f16, 992(x19)\n"
			"fnmadd.ps f19, f4, f16, f16, rtz\n" 
			"li x5, 0x9c07abeb9cc82f9\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0xe3dad9104e5ed4bf\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"la x11, rand_ieee754_32\n"
			"flw.ps f1, 640(x11)\n"
			"ffrc.ps f1, f1\n" 
			"la x27, rand_int_32\n"
			"flw.ps f25, 320(x27)\n"
			"faddi.pi f1, f25, 0x142\n" 
			"la x14, rand_int_32\n"
			"flw.ps f27, 480(x14)\n"
			"flw.ps f18, 608(x14)\n"
			"for.pi f4, f27, f18\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_2_XMA_QNT_STR_SEQ_ITER_0_HID_8:\n"
			"li x6, ((0xc << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0x2 << TENSOR_QUANT_QUANT_ROW) | (0x2a << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x0 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_2_XMA_QNT_STR_SEQ_ITER_1_HID_8:\n"
			"li x6, ((0x7 << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0xe << TENSOR_QUANT_QUANT_ROW) | (0x8 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"la x14, rand_ieee754_32\n"
			"flw.ps f26, 576(x14)\n"
			"fcvt.wu.s x9, f26, rne\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x25, rand_int_32\n"
			"flw.ps f8, 832(x25)\n"
			"fsat8.pi f19, f8\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f18, 896(x27)\n"
			"flw.ps f4, 608(x27)\n"
			"fsgnjn.ps f25, f18, f4\n" 
			"LBL_SEQID_3_M0_WRITE_ID_9_HID_8:\n"
			"mova.m.x x22\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x25, rand_int_32\n"
			"flw.ps f25, 768(x25)\n"
			"fsat8.pi f11, f25\n" 
			"la x19, rand_ieee754_16\n"
			"flw.ps f25, 480(x19)\n"
			"fcvt.ps.f16 f19, f25\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f1, 320(x19)\n"
			"flw.ps f11, 256(x19)\n"
			"fmax.ps f26, f1, f11\n" 
			"li x5, 0x10a20ecec9397987\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0x57cce6a74b6dc39a\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"la x19, rand_ieee754_32\n"
			"flw.ps f6, 896(x19)\n"
			"fsgnjx.s f13, f6, f6\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f18, 544(x9)\n"
			"fclass.s x19, f18\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f0, 480(x14)\n"
			"flw.ps f13, 672(x14)\n"
			"flw.ps f18, 960(x14)\n"
			"fmadd.s f14, f0, f13, f18, rtz\n" 
			"la x25, rand_int_32\n"
			"flw.ps f6, 480(x25)\n"
			"flw.ps f0, 704(x25)\n"
			"fsrl.pi f8, f6, f0\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f21, 0(x9)\n"
			"flw.ps f0, 928(x9)\n"
			"fnmsub.s f5, f21, f0, f21, rtz\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f26, 800(x18)\n"
			"flw.ps f19, 960(x18)\n"
			"flw.ps f14, 480(x18)\n"
			"fnmadd.ps f5, f26, f19, f14, rdn\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f16, 864(x19)\n"
			"fsin.ps f27, f16\n" 
			"LBL_SEQID_3_M0_WRITE_ID_10_HID_8:\n"
			"feqm.ps m3, f5, f16\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x18, rand_ieee754_32\n"
			"flw.ps f16, 704(x18)\n"
			"flw.ps f27, 672(x18)\n"
			"fmul.s f13, f16, f27, rne\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_3_TLD_XMA_STR_SEQ_ITER_0_HID_8:\n"
			"la x31, self_check_16\n"
			"li x5, 0xa000000000000c\n"
			"add x5, x5, x31\n"
			"li x6, 0x34000000000000e\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x1 << TENSOR_FMA16_B_NUM_COLS) | (0xc << TENSOR_FMA16_A_NUM_ROWS) | (0xe << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1a << TENSOR_FMA16_SCP_LOC_B) | (0x5 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x14 << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0xc << TENSOR_QUANT_QUANT_ROW) | (0x32 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x2 << TENSOR_QUANT_TRANSF6) | (0x7 << TENSOR_QUANT_TRANSF5) | (0x6 << TENSOR_QUANT_TRANSF4) | (0x6 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"la x11, rand_ieee754_32\n"
			"flw.ps f16, 160(x11)\n"
			"flw.ps f6, 416(x11)\n"
			"fmsub.s f14, f16, f16, f6, rdn\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f6, 576(x9)\n"
			"flw.ps f0, 0(x9)\n"
			"fsgnjx.s f25, f6, f0\n" 
			"la x9, rand_int_32\n"
			"flw.ps f11, 672(x9)\n"
			"flw.ps f25, 896(x9)\n"
			"fmin.pi f0, f11, f25\n" 
			"LBL_SEQID_4_M0_WRITE_ID_11_HID_8:\n"
			"mov.m.x m5, x15, 0x7b\n" 
			"li x5, CREDINC3\n"
			"ld x5, 0(x5)\n"
			"la x6, rand_ieee754_32\n"
			"flw.ps f21, 864(x6)\n"
			"fround.ps f8, f21, rne\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f5, 0(x11)\n"
			"flw.ps f8, 864(x11)\n"
			"fmul.s f0, f5, f8, rdn\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f27, 192(x18)\n"
			"flw.ps f1, 864(x18)\n"
			"fcmovm.ps f27, f27, f1\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f6, 160(x27)\n"
			"flw.ps f14, 992(x27)\n"
			"flw.ps f1, 608(x27)\n"
			"fmsub.s f4, f6, f14, f1, rmm\n" 
			"LBL_SEQID_4_M0_WRITE_ID_12_HID_8:\n"
			"maskxor m4, m4, m7\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x25, rand_int_32\n"
			"flw.ps f26, 960(x25)\n"
			"fmulh.pi f5, f26, f26\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f0, 768(x9)\n"
			"flw.ps f16, 96(x9)\n"
			"flw.ps f14, 192(x9)\n"
			"fmsub.ps f5, f0, f16, f14, rtz\n" 
			"li x19, 0xea89ce47ef58bd10\n"
			"fcvt.s.w f6, x19, rtz\n" 
			"la x22, rand_int_32\n"
			"flw.ps f16, 640(x22)\n"
			"flw.ps f25, 32(x22)\n"
			"fle.pi f5, f16, f25\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f25, 96(x11)\n"
			"flw.ps f19, 832(x11)\n"
			"fsgnj.s f13, f25, f19\n" 
			"la x18, rand_int_32\n"
			"flw.ps f8, 288(x18)\n"
			"fandi.pi f11, f8, 0x1ac\n" 
			"la x14, rand_int_32\n"
			"flw.ps f16, 960(x14)\n"
			"fnot.pi f21, f16\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_4_TLD_XMA_STR_SEQ_ITER_0_HID_8:\n"
			"la x31, self_check_16\n"
			"li x5, 0xc0000000000006\n"
			"add x5, x5, x31\n"
			"li x6, 0x10000000000007\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x3 << TENSOR_FMA16_B_NUM_COLS) | (0x6 << TENSOR_FMA16_A_NUM_ROWS) | (0x7 << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x1 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1d << TENSOR_FMA16_SCP_LOC_B) | (0x6 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0x10\n"
			"bne x5, x31, LBL_FAIL_HID_8\n"
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
			"csrwi mhpmevent3, 24\n"   
			"csrwi mhpmevent4, 1\n"   
			"csrwi mhpmevent5, 2\n"   
			"csrwi mhpmevent6, 8\n"   
			"csrwi mhpmevent7, 1\n"   
			"csrwi mhpmevent8, 2\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f2,  256 (x5)\n"
			"flw.ps f29,  480 (x5)\n"
			"flw.ps f6,  448 (x5)\n"
			"flw.ps f25,  128 (x5)\n"
			"flw.ps f0,  96 (x5)\n"
			"flw.ps f21,  608 (x5)\n"
			"flw.ps f28,  768 (x5)\n"
			"flw.ps f27,  736 (x5)\n"
			"flw.ps f10,  832 (x5)\n"
			"flw.ps f31,  224 (x5)\n"
			"flw.ps f16,  512 (x5)\n"
			"flw.ps f14,  800 (x5)\n"
			"flw.ps f1,  320 (x5)\n"
			"flw.ps f7,  160 (x5)\n"
			"flw.ps f18,  0 (x5)\n"
			"flw.ps f12,  928 (x5)\n"
			"flw.ps f22,  864 (x5)\n"
			"flw.ps f3,  288 (x5)\n"
			"flw.ps f17,  544 (x5)\n"
			"flw.ps f9,  960 (x5)\n"
			"flw.ps f23,  352 (x5)\n"
			"flw.ps f8,  32 (x5)\n"
			"flw.ps f4,  896 (x5)\n"
			"flw.ps f26,  576 (x5)\n"
			"flw.ps f5,  640 (x5)\n"
			"flw.ps f20,  64 (x5)\n"
			"flw.ps f30,  416 (x5)\n"
			"flw.ps f13,  192 (x5)\n"
			"flw.ps f11,  384 (x5)\n"
			"flw.ps f19,  704 (x5)\n"
			"flw.ps f24,  672 (x5)\n"
			"flw.ps f15,  992 (x5)\n"
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
			"li x29, 0x4\n"
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
			"masknot m0, m6\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x22, rand_ieee754_32\n"
			"flw.ps f6, 832(x22)\n"
			"flw.ps f29, 768(x22)\n"
			"fle.ps f24, f6, f29\n" 
			"la x23, rand_int_32\n"
			"flw.ps f28, 992(x23)\n"
			"fsat8.pi f29, f28\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f19, 736(x9)\n"
			"flw.ps f31, 32(x9)\n"
			"flt.s x17, f19, f31\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f16, 672(x16)\n"
			"flw.ps f1, 288(x16)\n"
			"feq.ps f13, f16, f1\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f25, 768(x17)\n"
			"flw.ps f2, 960(x17)\n"
			"fsgnjx.ps f1, f25, f2\n" 
			"li x26, 0xbef696b88411bebd\n"
			"fcvt.s.w f13, x26, rdn\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f9, 352(x15)\n"
			"flw.ps f29, 960(x15)\n"
			"flw.ps f3, 608(x15)\n"
			"fmsub.s f6, f9, f29, f3, rmm\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f24, 160(x22)\n"
			"flw.ps f19, 768(x22)\n"
			"flw.ps f16, 64(x22)\n"
			"fnmadd.ps f13, f24, f19, f16, rdn\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_9:\n"
			"mova.m.x x25\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x22, rand_int_32\n"
			"flw.ps f1, 544(x22)\n"
			"flw.ps f13, 192(x22)\n"
			"fltu.pi f1, f1, f13\n" 
			"fbci.ps f28, 0x635b5\n" 
			"la x28, rand_int_32\n"
			"flw.ps f3, 544(x28)\n"
			"flw.ps f29, 448(x28)\n"
			"flt.pi f25, f3, f29\n" 
			"la x26, rand_int_32\n"
			"flw.ps f6, 832(x26)\n"
			"fcvt.ps.pw f29, f6, rmm\n" 
			"la x26, rand_int_32\n"
			"flw.ps f28, 672(x26)\n"
			"fsatu8.pi f22, f28\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f9, 416(x22)\n"
			"flw.ps f25, 416(x22)\n"
			"flw.ps f3, 928(x22)\n"
			"fmadd.s f6, f9, f25, f3, rmm\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_9\n"
			"addi x30, x30, 1\n"
			"la x9, rand_int_32\n"
			"flw.ps f31, 64(x9)\n"
			"flw.ps f24, 576(x9)\n"
			"fmul.pi f9, f31, f24\n" 
			"LBL_SEQID_1_FP_TRANS_ID_2_HID_9:\n"
			"la x16, rand_ieee754_32\n"
			"flw.ps f24, 96(x16)\n"
			"flog.ps f2, f24\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f6, 992(x15)\n"
			"flw.ps f19, 192(x15)\n"
			"fadd.s f9, f6, f19, rne\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f28, 160(x26)\n"
			"flw.ps f24, 672(x26)\n"
			"fmax.ps f13, f28, f24\n" 
			"la x13, rand_int_32\n"
			"flw.ps f31, 96(x13)\n"
			"flw.ps f22, 0(x13)\n"
			"fadd.pi f13, f31, f22\n" 
			"la x23, rand_int_32\n"
			"flw.ps f24, 0(x23)\n"
			"fsrli.pi f22, f24, 0xd\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f25, 608(x17)\n"
			"flw.ps f19, 736(x17)\n"
			"fmax.ps f30, f25, f19\n" 
			"la x26, rand_ieee754_16\n"
			"flw.ps f6, 256(x26)\n"
			"fcvt.ps.f16 f2, f6\n" 
			"la x28, rand_int_32\n"
			"flw.ps f29, 32(x28)\n"
			"fltu.pi f24, f29, f29\n" 
			"LBL_SEQID_1_M0_WRITE_ID_3_HID_9:\n"
			"maskor m4, m6, m5\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x16, rand_ieee754_32\n"
			"flw.ps f16, 224(x16)\n"
			"flw.ps f25, 896(x16)\n"
			"flw.ps f6, 416(x16)\n"
			"fcmov.ps f25, f16, f25, f6\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f13, 32(x17)\n"
			"flw.ps f19, 704(x17)\n"
			"flw.ps f16, 480(x17)\n"
			"fmsub.s f3, f13, f19, f16, rne\n" 
			"la x26, rand_int_32\n"
			"flw.ps f19, 416(x26)\n"
			"fsrai.pi f9, f19, 0x6\n" 
			"la x13, rand_int_32\n"
			"flw.ps f16, 672(x13)\n"
			"fnot.pi f30, f16\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f19, 128(x13)\n"
			"flw.ps f25, 512(x13)\n"
			"flw.ps f21, 288(x13)\n"
			"fcmov.ps f9, f19, f25, f21\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f19, 832(x17)\n"
			"flw.ps f6, 128(x17)\n"
			"fmin.s f2, f19, f6\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_9\n"
			"addi x30, x30, 1\n"
			"la x25, rand_ieee754_32\n"
			"flw.ps f25, 992(x25)\n"
			"flw.ps f3, 768(x25)\n"
			"fsgnjx.ps f29, f25, f3\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f16, 32(x22)\n"
			"fsgnj.ps f25, f16, f16\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f31, 320(x16)\n"
			"flw.ps f25, 672(x16)\n"
			"flw.ps f3, 256(x16)\n"
			"fnmadd.ps f19, f31, f25, f3, rup\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f21, 736(x28)\n"
			"flw.ps f19, 640(x28)\n"
			"feq.s x25, f21, f19\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f29, 416(x22)\n"
			"fcvt.wu.s x13, f29, rdn\n" 
			"LBL_SEQID_2_FP_TRANS_ID_4_HID_9:\n"
			"la x23, rand_ieee754_32\n"
			"flw.ps f30, 544(x23)\n"
			"flog.ps f1, f30\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f24, 640(x6)\n"
			"flw.ps f21, 512(x6)\n"
			"fmax.ps f1, f24, f21\n" 
			"la x17, rand_int_32\n"
			"flw.ps f3, 64(x17)\n"
			"flw.ps f22, 32(x17)\n"
			"fmax.pi f24, f3, f22\n" 
			"la x15, rand_ieee754_16\n"
			"flw.ps f22, 96(x15)\n"
			"fcvt.ps.f16 f2, f22\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f19, 832(x13)\n"
			"flw.ps f2, 128(x13)\n"
			"feq.ps f22, f19, f2\n" 
			"la x25, rand_int_32\n"
			"flw.ps f31, 992(x25)\n"
			"flw.ps f2, 576(x25)\n"
			"fsub.pi f16, f31, f2\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f30, 960(x28)\n"
			"flw.ps f22, 128(x28)\n"
			"flw.ps f6, 608(x28)\n"
			"fnmsub.ps f2, f30, f22, f6, rne\n" 
			"la x28, rand_int_32\n"
			"flw.ps f24, 96(x28)\n"
			"flw.ps f6, 768(x28)\n"
			"fle.pi f25, f24, f6\n" 
			"la x25, rand_int_32\n"
			"flw.ps f3, 0(x25)\n"
			"flw.ps f19, 64(x25)\n"
			"fmin.pi f21, f3, f19\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f29, 96(x22)\n"
			"flw.ps f9, 576(x22)\n"
			"fmul.s f1, f29, f9, rdn\n" 
			"la x25, rand_int_32\n"
			"flw.ps f2, 832(x25)\n"
			"flw.ps f22, 352(x25)\n"
			"fminu.pi f22, f2, f22\n" 
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
			"la x17, rand_int_32\n"
			"flw.ps f19, 512(x17)\n"
			"faddi.pi f31, f19, 0x9f\n" 
			"la x28, rand_int_32\n"
			"flw.ps f30, 352(x28)\n"
			"flw.ps f19, 448(x28)\n"
			"fmulhu.pi f6, f30, f19\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f25, 672(x9)\n"
			"flw.ps f31, 672(x9)\n"
			"fmul.s f28, f25, f31, rmm\n" 
			"LBL_SEQID_3_M0_WRITE_ID_5_HID_9:\n"
			"maskor m5, m4, m6\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"maskpopcz x28, m5\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f9, 64(x22)\n"
			"fcvt.pw.ps f3, f9, rtz\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f29, 224(x6)\n"
			"flw.ps f30, 320(x6)\n"
			"fsgnjn.ps f16, f29, f30\n" 
			"LBL_SEQID_3_M0_WRITE_ID_6_HID_9:\n"
			"maskor m0, m0, m0\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x6, rand_int_32\n"
			"flw.ps f9, 576(x6)\n"
			"fsrli.pi f21, f9, 0x6\n" 
			"la x6, rand_int_32\n"
			"flw.ps f21, 960(x6)\n"
			"fpackrepb.pi f29, f21\n" 
			"li x17, 0x3ce80d7b4688f945\n"
			"fcvt.s.wu f21, x17, rtz\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f28, 928(x25)\n"
			"flw.ps f31, 608(x25)\n"
			"fsub.ps f21, f28, f31, rmm\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f25, 256(x9)\n"
			"flw.ps f31, 672(x9)\n"
			"fmin.ps f22, f25, f31\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f1, 544(x15)\n"
			"fcvt.w.s x9, f1, rmm\n" 
			"la x6, rand_int_32\n"
			"flw.ps f30, 896(x6)\n"
			"fpackrepb.pi f25, f30\n" 
			"la x25, rand_int_32\n"
			"flw.ps f9, 384(x25)\n"
			"flw.ps f13, 192(x25)\n"
			"fsll.pi f21, f9, f13\n" 
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
			"LBL_SEQID_4_M0_WRITE_ID_7_HID_9:\n"
			"fltm.ps m4, f3, f2\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x9, rand_ieee754_32\n"
			"flw.ps f30, 448(x9)\n"
			"flw.ps f9, 608(x9)\n"
			"flw.ps f6, 640(x9)\n"
			"fmadd.ps f2, f30, f9, f6, rtz\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f3, 448(x23)\n"
			"flw.ps f22, 832(x23)\n"
			"flw.ps f24, 544(x23)\n"
			"fmadd.ps f25, f3, f22, f24, rtz\n" 
			"LBL_SEQID_4_M0_WRITE_ID_8_HID_9:\n"
			"fltm.pi m6, f1, f29\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x23, rand_ieee754_32\n"
			"flw.ps f2, 896(x23)\n"
			"flw.ps f29, 960(x23)\n"
			"fsgnj.s f16, f2, f29\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f19, 992(x16)\n"
			"fcvt.pwu.ps f31, f19, rup\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f25, 32(x13)\n"
			"frsq.ps f25, f25\n" 
			"LBL_SEQID_4_M0_WRITE_ID_9_HID_9:\n"
			"masknot m4, m6\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x22, rand_ieee754_32\n"
			"flw.ps f31, 992(x22)\n"
			"fcvt.pwu.ps f28, f31, rne\n" 
			"LBL_SEQID_4_M0_WRITE_ID_10_HID_9:\n"
			"fsetm.pi m5, f2\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x15, rand_int_32\n"
			"flw.ps f21, 768(x15)\n"
			"flw.ps f29, 512(x15)\n"
			"fsub.pi f29, f21, f29\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f2, 128(x28)\n"
			"flw.ps f29, 416(x28)\n"
			"fsgnj.s f19, f2, f29\n" 
			"LBL_SEQID_4_M0_WRITE_ID_11_HID_9:\n"
			"mova.m.x x17\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x25, rand_int_32\n"
			"flw.ps f22, 736(x25)\n"
			"faddi.pi f22, f22, 0xfd\n" 
			"li x13, 0x20a05a495a230b37\n"
			"fcvt.s.w f9, x13, rtz\n" 
			"la x23, rand_int_32\n"
			"flw.ps f9, 672(x23)\n"
			"flw.ps f16, 704(x23)\n"
			"fadd.pi f6, f9, f16\n" 
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
			"csrwi mhpmevent3, 8\n"   
			"csrwi mhpmevent4, 4\n"   
			"csrwi mhpmevent5, 21\n"   
			"csrwi mhpmevent6, 6\n"   
			"csrwi mhpmevent7, 4\n"   
			"csrwi mhpmevent8, 5\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f11,  864 (x5)\n"
			"flw.ps f27,  512 (x5)\n"
			"flw.ps f19,  96 (x5)\n"
			"flw.ps f20,  160 (x5)\n"
			"flw.ps f30,  960 (x5)\n"
			"flw.ps f28,  0 (x5)\n"
			"flw.ps f1,  640 (x5)\n"
			"flw.ps f16,  832 (x5)\n"
			"flw.ps f24,  736 (x5)\n"
			"flw.ps f22,  896 (x5)\n"
			"flw.ps f17,  256 (x5)\n"
			"flw.ps f21,  64 (x5)\n"
			"flw.ps f23,  672 (x5)\n"
			"flw.ps f9,  224 (x5)\n"
			"flw.ps f7,  416 (x5)\n"
			"flw.ps f5,  352 (x5)\n"
			"flw.ps f25,  576 (x5)\n"
			"flw.ps f13,  480 (x5)\n"
			"flw.ps f15,  928 (x5)\n"
			"flw.ps f2,  128 (x5)\n"
			"flw.ps f4,  192 (x5)\n"
			"flw.ps f29,  288 (x5)\n"
			"flw.ps f0,  544 (x5)\n"
			"flw.ps f26,  448 (x5)\n"
			"flw.ps f3,  384 (x5)\n"
			"flw.ps f31,  32 (x5)\n"
			"flw.ps f14,  608 (x5)\n"
			"flw.ps f6,  800 (x5)\n"
			"flw.ps f8,  704 (x5)\n"
			"flw.ps f12,  992 (x5)\n"
			"flw.ps f10,  320 (x5)\n"
			"flw.ps f18,  768 (x5)\n"
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
			"li x29, 0x4\n"
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
			"la x16, rand_int_32\n"
			"flw.ps f9, 800(x16)\n"
			"flw.ps f5, 288(x16)\n"
			"fsll.pi f28, f9, f5\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f5, 256(x7)\n"
			"flw.ps f9, 512(x7)\n"
			"fsgnjx.s f17, f5, f9\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_10:\n"
			"maskand m4, m2, m4\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_0_FP_TRANS_ID_1_HID_10:\n"
			"la x7, rand_ieee754_32\n"
			"flw.ps f7, 800(x7)\n"
			"fexp.ps f7, f7\n" 
			"la x16, rand_int_32\n"
			"flw.ps f28, 256(x16)\n"
			"flw.ps f7, 448(x16)\n"
			"fand.pi f3, f28, f7\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f3, 32(x14)\n"
			"flw.ps f10, 576(x14)\n"
			"flw.ps f6, 960(x14)\n"
			"fmsub.ps f9, f3, f10, f6, rne\n" 
			"LBL_SEQID_0_M0_WRITE_ID_2_HID_10:\n"
			"fsetm.pi m3, f21\n" 
			"li x5, CREDINC2\n"
			"ld x5, 0(x5)\n"
			"la x14, rand_ieee754_16\n"
			"flw.ps f6, 544(x14)\n"
			"fcvt.ps.f16 f14, f6\n" 
			"LBL_SEQID_0_M0_WRITE_ID_3_HID_10:\n"
			"feqm.ps m2, f6, f3\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0x75e50d6ea4\n"
			"flw.ps f5, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x7, rand_ieee754_32\n"
			"flw.ps f18, 448(x7)\n"
			"flw.ps f21, 864(x7)\n"
			"fsub.ps f26, f18, f21, rne\n" 
			"LBL_SEQID_0_M0_WRITE_ID_4_HID_10:\n"
			"flem.ps m1, f4, f17\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x7, rand_ieee754_32\n"
			"flw.ps f30, 0(x7)\n"
			"fclass.ps f4, f30\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f6, 320(x10)\n"
			"flw.ps f9, 736(x10)\n"
			"flw.ps f30, 256(x10)\n"
			"fmadd.s f17, f6, f9, f30, rmm\n" 
			"LBL_SEQID_0_M0_WRITE_ID_5_HID_10:\n"
			"maskor m1, m1, m4\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x18, rand_int_32\n"
			"flw.ps f14, 384(x18)\n"
			"fsra.pi f18, f14, f14\n" 
			"li x5, 0xc528\n"
			"csrw tensor_mask, x5\n"	
			"la x22, rand_ieee754_32\n"
			"flw.ps f21, 576(x22)\n"
			"fround.ps f4, f21, rtz\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_0_RDX_ID_0_SHIRE_0_HID_10:\n"
			"li x11, 0xc00000002640002\n"
			"csrw tensor_reduce, x11\n" 
			"csrwi tensor_wait, 9\n" 
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=0); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"li x5, 0x2dd9\n"
			"csrw tensor_mask, x5\n"	
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ITER_0_HID_10:\n"
			"li x5, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x2 << TENSOR_FMA16_B_NUM_COLS) | (0xc << TENSOR_FMA16_A_NUM_ROWS) | (0x8 << TENSOR_FMA16_A_NUM_COLS) | (0x6 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0xfd << TENSOR_FMA16_SCP_LOC_B) | (0xf1 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x6, ((0x1a << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0xc << TENSOR_QUANT_QUANT_ROW) | (0x2f << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x6 << TENSOR_QUANT_TRANSF8) | (0x6 << TENSOR_QUANT_TRANSF7) | (0x7 << TENSOR_QUANT_TRANSF6) | (0x6 << TENSOR_QUANT_TRANSF5) | (0x7 << TENSOR_QUANT_TRANSF4) | (0x6 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ITER_1_HID_10:\n"
			"li x5, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x3 << TENSOR_IMA_B_NUM_COLS) | (0xa << TENSOR_IMA_A_NUM_ROWS) | (0x0 << TENSOR_IMA_A_NUM_COLS) | (0x8 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x13 << TENSOR_IMA_SCP_LOC_B) | (0x1e << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ITER_2_HID_10:\n"
			"li x5, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x3 << TENSOR_FMA16_B_NUM_COLS) | (0x8 << TENSOR_FMA16_A_NUM_ROWS) | (0x5 << TENSOR_FMA16_A_NUM_COLS) | (0xf << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0xbb << TENSOR_FMA16_SCP_LOC_B) | (0x92 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x6, ((0x2 << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0x8 << TENSOR_QUANT_QUANT_ROW) | (0x2e << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x2 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 7\n"
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_10\n"
			"csrw tensor_error, x0\n"
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_10\n"
			"addi x30, x30, 1\n"
			"mova.x.m x10\n" 
			"LBL_SEQID_1_M0_WRITE_ID_6_HID_10:\n"
			"maskxor m1, m1, m1\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x16, rand_int_32\n"
			"flw.ps f21, 960(x16)\n"
			"flw.ps f10, 608(x16)\n"
			"fand.pi f10, f21, f10\n" 
			"la x22, rand_int_32\n"
			"flw.ps f9, 768(x22)\n"
			"flw.ps f23, 320(x22)\n"
			"fxor.pi f9, f9, f23\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f23, 96(x11)\n"
			"flw.ps f9, 704(x11)\n"
			"flt.s x7, f23, f9\n" 
			"li x5, 0x16df\n"
			"csrw tensor_mask, x5\n"	
			"la x10, rand_int_32\n"
			"flw.ps f14, 384(x10)\n"
			"fsatu8.pi f25, f14\n" 
			"li x10, 0x2ccb2b6a47a8dccf\n"
			"fcvt.s.w f3, x10, rup\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f21, 800(x18)\n"
			"flw.ps f23, 0(x18)\n"
			"fsub.ps f30, f21, f23, rtz\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f25, 864(x16)\n"
			"flw.ps f3, 672(x16)\n"
			"fmin.s f26, f25, f3\n" 
			"la x22, rand_int_32\n"
			"flw.ps f14, 608(x22)\n"
			"flw.ps f5, 576(x22)\n"
			"fand.pi f14, f14, f5\n" 
			"li x5, 0x1871\n"
			"csrw tensor_mask, x5\n"	
			"la x22, rand_ieee754_32\n"
			"flw.ps f14, 384(x22)\n"
			"fcvt.pw.ps f23, f14, rmm\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f9, 704(x7)\n"
			"flw.ps f23, 160(x7)\n"
			"fsgnjn.s f9, f9, f23\n" 
			"LBL_SEQID_1_M0_WRITE_ID_7_HID_10:\n"
			"feqm.ps m2, f26, f4\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x26, rand_int_32\n"
			"flw.ps f6, 672(x26)\n"
			"flw.ps f17, 864(x26)\n"
			"fmaxu.pi f4, f6, f17\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f5, 416(x6)\n"
			"flw.ps f18, 96(x6)\n"
			"fmax.ps f21, f5, f18\n" 
			"la x14, rand_int_32\n"
			"flw.ps f25, 320(x14)\n"
			"flw.ps f17, 960(x14)\n"
			"fsrl.pi f26, f25, f17\n" 
			"csrw tensor_error, zero\n" 
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=0); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"LBL_SEQID_1_TLD_XMA_STR_SEQ_ITER_0_HID_10:\n"
			"la x31, self_check_16\n"
			"li x5, 0xe0000000000007\n"
			"add x5, x5, x31\n"
			"li x6, 0x1000000000000f\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x1 << TENSOR_FMA16_B_NUM_COLS) | (0x7 << TENSOR_FMA16_A_NUM_ROWS) | (0xf << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x1 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x19 << TENSOR_FMA16_SCP_LOC_B) | (0x7 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0x20\n"
			"bne x5, x31, LBL_FAIL_HID_10\n"
			"LBL_SEQID_1_TLD_XMA_STR_SEQ_ITER_1_HID_10:\n"
			"la x31, self_check_32\n"
			"li x5, 0xe000000000000b\n"
			"add x5, x5, x31\n"
			"li x6, 0x10000000000001\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x0 << TENSOR_FMA32_B_NUM_COLS) | (0xb << TENSOR_FMA32_A_NUM_ROWS) | (0x1 << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x1 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x18 << TENSOR_FMA32_SCP_LOC_B) | (0x7 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
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
			"LBL_SEQID_1_TLD_XMA_STR_SEQ_ITER_2_HID_10:\n"
			"la x31, self_check_8\n"
			"li x5, 0x6000000000000e\n"
			"add x5, x5, x31\n"
			"li x6, 0x360000000000001\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x0 << TENSOR_IMA_B_NUM_COLS) | (0xe << TENSOR_IMA_A_NUM_ROWS) | (0x1 << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1b << TENSOR_IMA_SCP_LOC_B) | (0x3 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x9, ((0x7 << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0xe << TENSOR_QUANT_QUANT_ROW) | (0x11 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x1 << TENSOR_QUANT_TRANSF2) | (0x4 << TENSOR_QUANT_TRANSF1) | (0x4 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"bne x5, x31, LBL_FAIL_HID_10\n"
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_10\n"
			"csrw tensor_error, x0\n"
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_10\n"
			"addi x30, x30, 1\n"
			"la x22, rand_int_32\n"
			"flw.ps f17, 448(x22)\n"
			"flw.ps f5, 736(x22)\n"
			"fadd.pi f18, f17, f5\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f21, 544(x7)\n"
			"fcvt.wu.s x11, f21, rne\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f26, 160(x15)\n"
			"flw.ps f6, 512(x15)\n"
			"flw.ps f4, 864(x15)\n"
			"fmadd.s f23, f26, f6, f4, rup\n" 
			"li x5, 0x8a80\n"
			"csrw tensor_mask, x5\n"	
			"la x10, rand_ieee754_32\n"
			"flw.ps f18, 352(x10)\n"
			"flw.ps f4, 192(x10)\n"
			"flw.ps f26, 416(x10)\n"
			"fmadd.s f30, f18, f4, f26, rmm\n" 
			"la x26, rand_int_32\n"
			"flw.ps f25, 832(x26)\n"
			"flw.ps f28, 384(x26)\n"
			"fmulhu.pi f10, f25, f28\n" 
			"la x22, rand_int_32\n"
			"flw.ps f17, 512(x22)\n"
			"flw.ps f5, 288(x22)\n"
			"for.pi f9, f17, f5\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f30, 416(x22)\n"
			"flw.ps f18, 64(x22)\n"
			"fsgnjn.s f18, f30, f18\n" 
			"la x16, rand_int_32\n"
			"flw.ps f17, 960(x16)\n"
			"flw.ps f9, 96(x16)\n"
			"fmaxu.pi f18, f17, f9\n" 
			"li x18, 0x47dde0e856332222\n"
			"fcvt.s.w f28, x18, rup\n" 
			"LBL_SEQID_2_FP_TRANS_ID_8_HID_10:\n"
			"la x26, rand_ieee754_32\n"
			"flw.ps f10, 0(x26)\n"
			"flog.ps f17, f10\n" 
			"la x7, rand_int_32\n"
			"flw.ps f28, 192(x7)\n"
			"flw.ps f17, 416(x7)\n"
			"fsra.pi f14, f28, f17\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f14, 928(x18)\n"
			"flw.ps f23, 928(x18)\n"
			"flw.ps f9, 224(x18)\n"
			"fmadd.ps f9, f14, f23, f9, rmm\n" 
			"li x5, 0x97ec\n"
			"csrw tensor_mask, x5\n"	
			"fbci.pi f30, 0x740d7\n" 
			"LBL_SEQID_2_FP_TRANS_ID_9_HID_10:\n"
			"la x15, rand_ieee754_32\n"
			"flw.ps f10, 896(x15)\n"
			"fexp.ps f18, f10\n" 
			"la x15, rand_int_32\n"
			"flw.ps f18, 32(x15)\n"
			"flw.ps f23, 928(x15)\n"
			"fsll.pi f14, f18, f23\n" 
			"LBL_SEQID_2_FP_TRANS_ID_10_HID_10:\n"
			"la x7, rand_ieee754_32\n"
			"flw.ps f21, 704(x7)\n"
			"fexp.ps f10, f21\n" 
			"csrw tensor_error, zero\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"LBL_SEQID_2_TLD_XMA_STR_SEQ_ITER_0_HID_10:\n"
			"la x31, self_check_32\n"
			"li x5, 0xe000000000000b\n"
			"add x5, x5, x31\n"
			"li x6, 0x1000000000000a\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x2 << TENSOR_FMA32_B_NUM_COLS) | (0xb << TENSOR_FMA32_A_NUM_ROWS) | (0xa << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x1 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1c << TENSOR_FMA32_SCP_LOC_B) | (0x7 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x9, ((0xf << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0xb << TENSOR_QUANT_QUANT_ROW) | (0x3e << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x7 << TENSOR_QUANT_TRANSF8) | (0x6 << TENSOR_QUANT_TRANSF7) | (0x7 << TENSOR_QUANT_TRANSF6) | (0x7 << TENSOR_QUANT_TRANSF5) | (0x7 << TENSOR_QUANT_TRANSF4) | (0x7 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"li x5, 3\n" 
			"bne x30, x5, LBL_FAIL_HID_10\n"
			"nop\n"          
		       VSNIP_RSV     
		);
		load_vpu_regs();
		__asm__ __volatile__ ( 
			"nop\n"          
			"addi x30, x30, 1\n"
			"LBL_SEQID_3_FP_TRANS_ID_11_HID_10:\n"
			"la x18, rand_ieee754_32\n"
			"flw.ps f26, 32(x18)\n"
			"fexp.ps f23, f26\n" 
			"la x10, rand_int_32\n"
			"flw.ps f3, 224(x10)\n"
			"flw.ps f23, 960(x10)\n"
			"fxor.pi f14, f3, f23\n" 
			"la x16, rand_int_32\n"
			"flw.ps f25, 736(x16)\n"
			"faddi.pi f28, f25, 0x7f\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f26, 96(x16)\n"
			"flw.ps f3, 800(x16)\n"
			"fdiv.s f6, f26, f3, rdn\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f9, 960(x26)\n"
			"flw.ps f21, 64(x26)\n"
			"feq.s x26, f9, f21\n" 
			"la x16, rand_int_32\n"
			"flw.ps f21, 224(x16)\n"
			"fpackreph.pi f4, f21\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f25, 64(x10)\n"
			"flw.ps f18, 128(x10)\n"
			"flw.ps f3, 544(x10)\n"
			"fnmadd.s f3, f25, f18, f3, rdn\n" 
			"fbci.pi f7, 0x126b\n" 
			"la x7, rand_int_32\n"
			"flw.ps f21, 128(x7)\n"
			"fpackreph.pi f14, f21\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f30, 672(x16)\n"
			"flw.ps f18, 0(x16)\n"
			"flw.ps f23, 416(x16)\n"
			"fmsub.ps f7, f30, f18, f23, rtz\n" 
			"la x15, rand_int_32\n"
			"flw.ps f14, 800(x15)\n"
			"faddi.pi f5, f14, 0x42\n" 
			"LBL_SEQID_3_M0_WRITE_ID_12_HID_10:\n"
			"feqm.ps m1, f5, f14\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0xfb57b12dd4\n"
			"fsw.ps f0, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x11, rand_ieee754_32\n"
			"flw.ps f18, 928(x11)\n"
			"fclass.ps f30, f18\n" 
			"LBL_SEQID_3_M0_WRITE_ID_13_HID_10:\n"
			"maskand m1, m2, m3\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_3_M0_WRITE_ID_14_HID_10:\n"
			"flem.ps m1, f30, f4\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0x91f013efc1\n"
			"flw.ps f5, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x18, rand_int_32\n"
			"flw.ps f28, 384(x18)\n"
			"flw.ps f17, 992(x18)\n"
			"fsll.pi f30, f28, f17\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_3_XMA_QNT_STR_SEQ_ITER_0_HID_10:\n"
			"li x5, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x2 << TENSOR_FMA32_B_NUM_COLS) | (0xd << TENSOR_FMA32_A_NUM_ROWS) | (0x6 << TENSOR_FMA32_A_NUM_COLS) | (0xc << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x8e << TENSOR_FMA32_SCP_LOC_B) | (0x50 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x0 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
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
			"la x15, rand_int_32\n"
			"flw.ps f5, 128(x15)\n"
			"flw.ps f28, 640(x15)\n"
			"flt.pi f7, f5, f28\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f18, 224(x15)\n"
			"flw.ps f5, 608(x15)\n"
			"fle.ps f14, f18, f5\n" 
			"la x11, rand_int_32\n"
			"flw.ps f25, 736(x11)\n"
			"fsra.pi f25, f25, f25\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f18, 320(x7)\n"
			"fcvt.l.s x9, f18, rne\n" 
			"la x11, rand_int_32\n"
			"flw.ps f30, 0(x11)\n"
			"flw.ps f9, 320(x11)\n"
			"fmin.pi f18, f30, f9\n" 
			"LBL_SEQID_4_M0_WRITE_ID_15_HID_10:\n"
			"maskor m3, m4, m3\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x14, rand_int_32\n"
			"flw.ps f30, 704(x14)\n"
			"flw.ps f7, 128(x14)\n"
			"fxor.pi f23, f30, f7\n" 
			"LBL_SEQID_4_M0_WRITE_ID_16_HID_10:\n"
			"flem.ps m1, f21, f10\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x10, rand_ieee754_16\n"
			"flw.ps f18, 256(x10)\n"
			"fcvt.ps.f16 f17, f18\n" 
			"LBL_SEQID_4_M0_WRITE_ID_17_HID_10:\n"
			"feqm.ps m2, f18, f5\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x22, rand_int_32\n"
			"flw.ps f3, 896(x22)\n"
			"flw.ps f6, 576(x22)\n"
			"fmin.pi f10, f3, f6\n" 
			"li x6, 0x83758b65b5674a50\n"
			"fcvt.s.wu f3, x6, rmm\n" 
			"la x11, rand_int_32\n"
			"flw.ps f5, 256(x11)\n"
			"faddi.pi f25, f5, 0x1fc\n" 
			"la x7, rand_int_32\n"
			"flw.ps f7, 160(x7)\n"
			"flw.ps f18, 736(x7)\n"
			"flt.pi f23, f7, f18\n" 
			"la x26, rand_int_32\n"
			"flw.ps f5, 480(x26)\n"
			"flw.ps f7, 288(x26)\n"
			"flt.pi f18, f5, f7\n" 
			"la x14, rand_int_32\n"
			"flw.ps f5, 544(x14)\n"
			"fsrai.pi f25, f5, 0x3\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_4_TLD_XMA_STR_SEQ_ITER_0_HID_10:\n"
			"la x31, self_check_8\n"
			"li x5, 0xc0000000000006\n"
			"add x5, x5, x31\n"
			"li x6, 0x380000000000004\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x3 << TENSOR_IMA_B_NUM_COLS) | (0x6 << TENSOR_IMA_A_NUM_ROWS) | (0x4 << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1c << TENSOR_IMA_SCP_LOC_B) | (0x6 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x9, ((0x5 << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0x6 << TENSOR_QUANT_QUANT_ROW) | (0x1 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x9 << TENSOR_QUANT_TRANSF8) | (0x9 << TENSOR_QUANT_TRANSF7) | (0x4 << TENSOR_QUANT_TRANSF6) | (0xa << TENSOR_QUANT_TRANSF5) | (0x5 << TENSOR_QUANT_TRANSF4) | (0x4 << TENSOR_QUANT_TRANSF3) | (0x5 << TENSOR_QUANT_TRANSF2) | (0x5 << TENSOR_QUANT_TRANSF1) | (0x4 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"bne x5, x31, LBL_FAIL_HID_10\n"
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 10\n"
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
			"csrwi mhpmevent3, 24\n"   
			"csrwi mhpmevent4, 1\n"   
			"csrwi mhpmevent5, 2\n"   
			"csrwi mhpmevent6, 8\n"   
			"csrwi mhpmevent7, 1\n"   
			"csrwi mhpmevent8, 2\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f27,  192 (x5)\n"
			"flw.ps f18,  672 (x5)\n"
			"flw.ps f29,  512 (x5)\n"
			"flw.ps f5,  480 (x5)\n"
			"flw.ps f25,  960 (x5)\n"
			"flw.ps f23,  256 (x5)\n"
			"flw.ps f8,  288 (x5)\n"
			"flw.ps f9,  768 (x5)\n"
			"flw.ps f3,  160 (x5)\n"
			"flw.ps f28,  0 (x5)\n"
			"flw.ps f14,  608 (x5)\n"
			"flw.ps f26,  928 (x5)\n"
			"flw.ps f30,  416 (x5)\n"
			"flw.ps f24,  576 (x5)\n"
			"flw.ps f21,  864 (x5)\n"
			"flw.ps f17,  96 (x5)\n"
			"flw.ps f31,  224 (x5)\n"
			"flw.ps f6,  32 (x5)\n"
			"flw.ps f20,  448 (x5)\n"
			"flw.ps f12,  352 (x5)\n"
			"flw.ps f1,  640 (x5)\n"
			"flw.ps f11,  736 (x5)\n"
			"flw.ps f0,  992 (x5)\n"
			"flw.ps f2,  320 (x5)\n"
			"flw.ps f13,  64 (x5)\n"
			"flw.ps f19,  704 (x5)\n"
			"flw.ps f22,  128 (x5)\n"
			"flw.ps f4,  896 (x5)\n"
			"flw.ps f16,  832 (x5)\n"
			"flw.ps f10,  800 (x5)\n"
			"flw.ps f7,  384 (x5)\n"
			"flw.ps f15,  544 (x5)\n"
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
			"li x29, 0x4\n"
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
			"la x22, rand_ieee754_32\n"
			"flw.ps f22, 0(x22)\n"
			"flw.ps f20, 832(x22)\n"
			"fadd.s f20, f22, f20, rtz\n" 
			"fbci.pi f20, 0x4ba7f\n" 
			"la x5, rand_int_32\n"
			"flw.ps f27, 544(x5)\n"
			"fpackreph.pi f29, f27\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f14, 64(x22)\n"
			"fcvt.pwu.ps f23, f14, rne\n" 
			"la x6, rand_int_32\n"
			"flw.ps f12, 160(x6)\n"
			"fslli.pi f20, f12, 0x2\n" 
			"la x6, rand_int_32\n"
			"flw.ps f22, 256(x6)\n"
			"flw.ps f0, 416(x6)\n"
			"flt.pi f10, f22, f0\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f12, 384(x28)\n"
			"fcvt.pw.ps f30, f12, rmm\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f10, 544(x25)\n"
			"fclass.s x9, f10\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f25, 416(x13)\n"
			"fcvt.f16.ps f4, f25\n" 
			"la x14, rand_int_32\n"
			"flw.ps f9, 32(x14)\n"
			"fpackreph.pi f9, f9\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f12, 320(x6)\n"
			"flw.ps f24, 800(x6)\n"
			"flw.ps f4, 224(x6)\n"
			"fcmov.ps f24, f12, f24, f4\n" 
			"la x28, rand_int_32\n"
			"flw.ps f23, 224(x28)\n"
			"fandi.pi f0, f23, 0x19d\n" 
			"la x9, rand_int_32\n"
			"flw.ps f23, 384(x9)\n"
			"flw.ps f16, 896(x9)\n"
			"fmin.pi f16, f23, f16\n" 
			"fbci.ps f16, 0x7f63e\n" 
			"la x5, rand_int_32\n"
			"flw.ps f29, 640(x5)\n"
			"faddi.pi f16, f29, 0x12f\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f20, 256(x17)\n"
			"flw.ps f27, 480(x17)\n"
			"fsgnj.s f16, f20, f27\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_11\n"
			"addi x30, x30, 1\n"
			"la x13, rand_ieee754_32\n"
			"flw.ps f20, 512(x13)\n"
			"fsin.ps f20, f20\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f24, 64(x14)\n"
			"frsq.ps f24, f24\n" 
			"LBL_SEQID_1_M0_WRITE_ID_0_HID_11:\n"
			"fltm.pi m2, f24, f30\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x25, rand_ieee754_32\n"
			"flw.ps f16, 736(x25)\n"
			"flw.ps f8, 64(x25)\n"
			"flw.ps f24, 704(x25)\n"
			"fnmsub.s f22, f16, f8, f24, rup\n" 
			"la x17, rand_int_32\n"
			"flw.ps f4, 736(x17)\n"
			"flw.ps f27, 64(x17)\n"
			"fxor.pi f16, f4, f27\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f0, 32(x22)\n"
			"fcvt.pw.ps f30, f0, rdn\n" 
			"la x26, rand_int_32\n"
			"flw.ps f23, 576(x26)\n"
			"flw.ps f16, 928(x26)\n"
			"fmax.pi f23, f23, f16\n" 
			"la x9, rand_int_32\n"
			"flw.ps f16, 576(x9)\n"
			"flw.ps f30, 480(x9)\n"
			"fsrl.pi f27, f16, f30\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f0, 256(x22)\n"
			"fmul.ps f27, f0, f0, rmm\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f10, 64(x13)\n"
			"flw.ps f4, 224(x13)\n"
			"flt.s x14, f10, f4\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f30, 832(x13)\n"
			"flw.ps f25, 512(x13)\n"
			"flt.ps f25, f30, f25\n" 
			"li x5, 0xce24d260cde594dc\n"
			"fcvt.s.l f20, x5, rup\n" 
			"la x25, rand_ieee754_16\n"
			"flw.ps f8, 32(x25)\n"
			"fcvt.ps.f16 f0, f8\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f8, 384(x6)\n"
			"flw.ps f16, 32(x6)\n"
			"fle.s x26, f8, f16\n" 
			"LBL_SEQID_1_M0_WRITE_ID_1_HID_11:\n"
			"maskand m7, m7, m0\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0xFF\n"
			"fbci.pi f10, 0x512da\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_11\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_2_FP_TRANS_ID_2_HID_11:\n"
			"la x9, rand_ieee754_32\n"
			"flw.ps f27, 352(x9)\n"
			"flog.ps f4, f27\n" 
			"LBL_SEQID_2_M0_WRITE_ID_3_HID_11:\n"
			"flem.ps m0, f25, f0\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x5, rand_int_32\n"
			"flw.ps f14, 928(x5)\n"
			"fsll.pi f24, f14, f14\n" 
			"la x5, rand_int_32\n"
			"flw.ps f30, 352(x5)\n"
			"flw.ps f23, 288(x5)\n"
			"fmul.pi f29, f30, f23\n" 
			"la x17, rand_int_32\n"
			"flw.ps f20, 576(x17)\n"
			"faddi.pi f24, f20, 0x1ce\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f24, 928(x6)\n"
			"flw.ps f30, 608(x6)\n"
			"fsgnjx.ps f12, f24, f30\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f10, 224(x6)\n"
			"flw.ps f22, 768(x6)\n"
			"flt.s x14, f10, f22\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f24, 960(x25)\n"
			"fnmadd.s f12, f24, f24, f24, rtz\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f20, 928(x25)\n"
			"fsin.ps f14, f20\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f27, 704(x14)\n"
			"flw.ps f8, 992(x14)\n"
			"flw.ps f4, 128(x14)\n"
			"fmsub.ps f4, f27, f8, f4, rup\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f22, 384(x9)\n"
			"flw.ps f30, 768(x9)\n"
			"fsgnjn.ps f9, f22, f30\n" 
			"LBL_SEQID_2_M0_WRITE_ID_4_HID_11:\n"
			"flem.ps m1, f4, f4\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x5, rand_ieee754_32\n"
			"flw.ps f4, 256(x5)\n"
			"fcvt.wu.s x9, f4, rne\n" 
			"la x25, rand_int_32\n"
			"flw.ps f22, 224(x25)\n"
			"fcvt.ps.pw f23, f22, rdn\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f20, 608(x14)\n"
			"flw.ps f22, 160(x14)\n"
			"fnmsub.ps f16, f20, f22, f20, rup\n" 
			"maskpopcz x26, m1\n" 
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
			"LBL_SEQID_3_M0_WRITE_ID_5_HID_11:\n"
			"maskor m7, m1, m2\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x5, rand_ieee754_32\n"
			"flw.ps f24, 864(x5)\n"
			"flw.ps f27, 576(x5)\n"
			"fadd.ps f16, f24, f27, rdn\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f10, 320(x9)\n"
			"flw.ps f30, 928(x9)\n"
			"flw.ps f16, 512(x9)\n"
			"fcmov.ps f27, f10, f30, f16\n" 
			"la x13, rand_int_32\n"
			"flw.ps f9, 288(x13)\n"
			"flw.ps f29, 288(x13)\n"
			"fmaxu.pi f16, f9, f29\n" 
			"la x22, rand_int_32\n"
			"flw.ps f30, 832(x22)\n"
			"flw.ps f25, 96(x22)\n"
			"fmulhu.pi f12, f30, f25\n" 
			"la x25, rand_int_32\n"
			"flw.ps f14, 672(x25)\n"
			"flw.ps f25, 832(x25)\n"
			"fand.pi f9, f14, f25\n" 
			"la x28, rand_ieee754_16\n"
			"flw.ps f23, 960(x28)\n"
			"fcvt.ps.f16 f4, f23\n" 
			"la x22, rand_int_32\n"
			"flw.ps f22, 736(x22)\n"
			"fsrli.pi f4, f22, 0x5\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f0, 896(x13)\n"
			"flw.ps f25, 864(x13)\n"
			"feq.s x25, f0, f25\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f9, 32(x13)\n"
			"flw.ps f8, 608(x13)\n"
			"fnmadd.s f0, f9, f8, f9, rne\n" 
			"la x13, rand_int_32\n"
			"flw.ps f22, 800(x13)\n"
			"flw.ps f4, 96(x13)\n"
			"fmaxu.pi f9, f22, f4\n" 
			"mova.x.m x5\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f16, 512(x6)\n"
			"fcvt.wu.s x17, f16, rtz\n" 
			"la x22, rand_int_32\n"
			"flw.ps f24, 928(x22)\n"
			"flw.ps f29, 0(x22)\n"
			"fmax.pi f29, f24, f29\n" 
			"la x9, rand_int_32\n"
			"flw.ps f22, 992(x9)\n"
			"flw.ps f14, 448(x9)\n"
			"fsll.pi f27, f22, f14\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f0, 576(x6)\n"
			"fclass.s x25, f0\n" 
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
			"LBL_SEQID_4_M0_WRITE_ID_6_HID_11:\n"
			"maskor m0, m1, m2\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x22, rand_int_32\n"
			"flw.ps f9, 32(x22)\n"
			"flw.ps f20, 288(x22)\n"
			"fminu.pi f4, f9, f20\n" 
			"la x17, rand_int_32\n"
			"flw.ps f12, 992(x17)\n"
			"fcvt.ps.pwu f24, f12, rtz\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f4, 96(x17)\n"
			"flw.ps f16, 736(x17)\n"
			"flw.ps f24, 32(x17)\n"
			"fmadd.ps f23, f4, f16, f24, rdn\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f23, 384(x17)\n"
			"flw.ps f27, 320(x17)\n"
			"fmadd.s f8, f23, f23, f27, rtz\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f24, 448(x26)\n"
			"flw.ps f27, 224(x26)\n"
			"flt.ps f9, f24, f27\n" 
			"li x5, 0x3e02534e3e533e14\n"
			"fcvt.s.wu f22, x5, rne\n" 
			"LBL_SEQID_4_M0_WRITE_ID_7_HID_11:\n"
			"mov.m.x m7, x14, 0x3\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x14, rand_int_32\n"
			"flw.ps f30, 288(x14)\n"
			"flw.ps f22, 800(x14)\n"
			"flt.pi f30, f30, f22\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f27, 928(x28)\n"
			"frsq.ps f16, f27\n" 
			"LBL_SEQID_4_M0_WRITE_ID_8_HID_11:\n"
			"fltm.pi m0, f23, f0\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x22, rand_int_32\n"
			"flw.ps f23, 160(x22)\n"
			"fsat8.pi f25, f23\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f16, 288(x25)\n"
			"fcvt.f16.ps f30, f16\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f30, 192(x25)\n"
			"flw.ps f22, 448(x25)\n"
			"fdiv.ps f14, f30, f22, rup\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f9, 0(x25)\n"
			"fsub.ps f29, f9, f9, rdn\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f16, 896(x9)\n"
			"fsqrt.ps f20, f16\n" 
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
			"csrwi mhpmevent3, 8\n"   
			"csrwi mhpmevent4, 4\n"   
			"csrwi mhpmevent5, 21\n"   
			"csrwi mhpmevent6, 6\n"   
			"csrwi mhpmevent7, 4\n"   
			"csrwi mhpmevent8, 5\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f12,  864 (x5)\n"
			"flw.ps f5,  992 (x5)\n"
			"flw.ps f23,  160 (x5)\n"
			"flw.ps f21,  352 (x5)\n"
			"flw.ps f31,  448 (x5)\n"
			"flw.ps f24,  960 (x5)\n"
			"flw.ps f1,  32 (x5)\n"
			"flw.ps f19,  704 (x5)\n"
			"flw.ps f7,  512 (x5)\n"
			"flw.ps f4,  224 (x5)\n"
			"flw.ps f17,  928 (x5)\n"
			"flw.ps f16,  608 (x5)\n"
			"flw.ps f29,  288 (x5)\n"
			"flw.ps f20,  320 (x5)\n"
			"flw.ps f22,  384 (x5)\n"
			"flw.ps f18,  480 (x5)\n"
			"flw.ps f26,  640 (x5)\n"
			"flw.ps f3,  96 (x5)\n"
			"flw.ps f27,  832 (x5)\n"
			"flw.ps f8,  128 (x5)\n"
			"flw.ps f30,  256 (x5)\n"
			"flw.ps f14,  672 (x5)\n"
			"flw.ps f10,  576 (x5)\n"
			"flw.ps f28,  0 (x5)\n"
			"flw.ps f25,  192 (x5)\n"
			"flw.ps f15,  768 (x5)\n"
			"flw.ps f0,  896 (x5)\n"
			"flw.ps f11,  64 (x5)\n"
			"flw.ps f6,  416 (x5)\n"
			"flw.ps f2,  800 (x5)\n"
			"flw.ps f13,  544 (x5)\n"
			"flw.ps f9,  736 (x5)\n"
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
			"li x29, 0x4\n"
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
			"maskxor m3, m1, m2\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x26, rand_ieee754_32\n"
			"flw.ps f30, 640(x26)\n"
			"fcvt.f16.ps f30, f30\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f26, 448(x11)\n"
			"fcvt.wu.s x7, f26, rdn\n" 
			"la x11, rand_int_32\n"
			"flw.ps f10, 832(x11)\n"
			"fnot.pi f29, f10\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f3, 992(x7)\n"
			"flw.ps f24, 640(x7)\n"
			"fsgnjn.ps f29, f3, f24\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f13, 288(x6)\n"
			"fcvt.pwu.ps f29, f13, rdn\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x22, rand_int_32\n"
			"flw.ps f8, 320(x22)\n"
			"fnot.pi f10, f8\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f26, 384(x10)\n"
			"flw.ps f3, 352(x10)\n"
			"fsgnjx.ps f21, f26, f3\n" 
			"LBL_SEQID_0_FP_TRANS_ID_1_HID_12:\n"
			"la x16, rand_ieee754_32\n"
			"flw.ps f10, 768(x16)\n"
			"fexp.ps f10, f10\n" 
			"la x19, rand_int_32\n"
			"flw.ps f19, 704(x19)\n"
			"fdivu.pi f25, f19, f19\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f19, 640(x25)\n"
			"flw.ps f3, 704(x25)\n"
			"fadd.ps f30, f19, f3, rmm\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f23, 0(x27)\n"
			"flw.ps f24, 192(x27)\n"
			"fsgnj.s f17, f23, f24\n" 
			"LBL_SEQID_0_M0_WRITE_ID_2_HID_12:\n"
			"maskor m2, m1, m6\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"li x5, 0x3c81\n"
			"csrw tensor_mask, x5\n"	
			"LBL_SEQID_0_FP_TRANS_ID_3_HID_12:\n"
			"la x19, rand_ieee754_32\n"
			"flw.ps f14, 832(x19)\n"
			"fexp.ps f14, f14\n" 
			"li x7, 0xbd4354c0a5d7e2c3\n"
			"fcvt.s.w f30, x7, rup\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f29, 256(x6)\n"
			"flw.ps f23, 960(x6)\n"
			"flt.s x16, f29, f23\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 0xc60907be97289b35\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0x260c946537754546\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ID_554164_HID_12:\n"
			"li x6, ((0xc << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0xd << TENSOR_QUANT_QUANT_ROW) | (0x9 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x7 << TENSOR_QUANT_TRANSF6) | (0x7 << TENSOR_QUANT_TRANSF5) | (0x7 << TENSOR_QUANT_TRANSF4) | (0x6 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_12\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_0_RDX_ID_0_SHIRE_0_HID_12:\n"
			"li x6, 0xc00000002640002\n"
			"csrw tensor_reduce, x6\n" 
			"csrwi tensor_wait, 9\n" 
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=0); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ITER_0_HID_12:\n"
			"la x31, self_check_16\n"
			"li x5, 0xa000000000000e\n"
			"add x5, x5, x31\n"
			"li x6, 0x36000000000000c\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x2 << TENSOR_FMA16_B_NUM_COLS) | (0xe << TENSOR_FMA16_A_NUM_ROWS) | (0xc << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1b << TENSOR_FMA16_SCP_LOC_B) | (0x5 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x1b << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0xe << TENSOR_QUANT_QUANT_ROW) | (0x1a << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x2 << TENSOR_QUANT_TRANSF5) | (0x7 << TENSOR_QUANT_TRANSF4) | (0x7 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_12\n"
			"csrw tensor_error, x0\n"
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_12\n"
			"addi x30, x30, 1\n"
			"la x11, rand_ieee754_32\n"
			"flw.ps f26, 864(x11)\n"
			"fcvt.pwu.ps f25, f26, rdn\n" 
			"maskpopcz x16, m3\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f23, 96(x27)\n"
			"flw.ps f3, 640(x27)\n"
			"fle.s x25, f23, f3\n" 
			"la x25, rand_int_32\n"
			"flw.ps f3, 992(x25)\n"
			"fslli.pi f29, f3, 0x0\n" 
			"la x27, rand_int_32\n"
			"flw.ps f21, 128(x27)\n"
			"flw.ps f25, 544(x27)\n"
			"frem.pi f10, f21, f25\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f19, 320(x26)\n"
			"flw.ps f25, 512(x26)\n"
			"flw.ps f10, 320(x26)\n"
			"fnmsub.s f23, f19, f25, f10, rmm\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f10, 768(x10)\n"
			"fcvt.lu.s x7, f10, rtz\n" 
			"la x27, rand_int_32\n"
			"flw.ps f10, 0(x27)\n"
			"flw.ps f26, 672(x27)\n"
			"for.pi f19, f10, f26\n" 
			"la x26, rand_int_32\n"
			"flw.ps f26, 768(x26)\n"
			"fcvt.ps.pw f25, f26, rne\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f14, 224(x19)\n"
			"flw.ps f8, 576(x19)\n"
			"fmax.s f8, f14, f8\n" 
			"li x5, 0x41c7\n"
			"csrw tensor_mask, x5\n"	
			"la x10, rand_ieee754_32\n"
			"flw.ps f10, 224(x10)\n"
			"flw.ps f12, 0(x10)\n"
			"flt.s x26, f10, f12\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f21, 608(x25)\n"
			"flw.ps f13, 416(x25)\n"
			"flw.ps f14, 320(x25)\n"
			"fnmadd.ps f15, f21, f13, f14, rtz\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f29, 224(x25)\n"
			"fcvt.pw.ps f12, f29, rmm\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f12, 512(x16)\n"
			"flw.ps f17, 224(x16)\n"
			"fsub.ps f23, f12, f17, rne\n" 
			"la x7, rand_int_32\n"
			"flw.ps f17, 352(x7)\n"
			"flw.ps f14, 96(x7)\n"
			"fand.pi f14, f17, f14\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f10, 352(x7)\n"
			"flw.ps f3, 416(x7)\n"
			"flw.ps f29, 192(x7)\n"
			"fmsub.ps f25, f10, f3, f29, rne\n" 
			"csrw tensor_error, zero\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ID_676144_HID_12:\n"
			"li x5, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x2 << TENSOR_IMA_B_NUM_COLS) | (0xb << TENSOR_IMA_A_NUM_ROWS) | (0x3 << TENSOR_IMA_A_NUM_COLS) | (0x3 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x2d << TENSOR_IMA_SCP_LOC_B) | (0xc5 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x6, ((0x6 << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0xb << TENSOR_QUANT_QUANT_ROW) | (0x2e << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0xa << TENSOR_QUANT_TRANSF5) | (0x3 << TENSOR_QUANT_TRANSF4) | (0xa << TENSOR_QUANT_TRANSF3) | (0x8 << TENSOR_QUANT_TRANSF2) | (0x5 << TENSOR_QUANT_TRANSF1) | (0x3 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 7\n"
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_12\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_1_RDX_ID_1_SHIRE_0_HID_12:\n"
			"li x10, 0x2800000000650000\n"
			"csrr x7, mhartid\n"
			"srli x7, x7, 4\n"
			"slli x7, x7, 6\n"
			"add x10, x10, x7\n"
			"csrw tensor_reduce, x10\n" 
			"csrwi tensor_wait, 9\n" 
			"LBL_SEQID_1_TLD_XMA_STR_SEQ_ID_188864_HID_12:\n"
			"la x31, self_check_8\n"
			"li x5, 0x80000000000008\n"
			"add x5, x5, x31\n"
			"li x6, 0x10000000000009\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x1 << TENSOR_IMA_B_NUM_COLS) | (0x8 << TENSOR_IMA_A_NUM_ROWS) | (0x9 << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x1 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1a << TENSOR_IMA_SCP_LOC_B) | (0x4 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fmvz.x.ps x31, f0, 0\n"
			"li x5, 0x28\n"
			"bne x5, x31, LBL_FAIL_HID_12\n"
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
			"LBL_SEQID_1_TLD_XMA_STR_SEQ_ITER_0_HID_12:\n"
			"la x31, self_check_32\n"
			"li x5, 0xe0000000000002\n"
			"add x5, x5, x31\n"
			"li x6, 0x1000000000000c\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x2 << TENSOR_FMA32_B_NUM_COLS) | (0x2 << TENSOR_FMA32_A_NUM_ROWS) | (0xc << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x1 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1e << TENSOR_FMA32_SCP_LOC_B) | (0x7 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0xd\n"
			"bne x5, x31, LBL_FAIL_HID_12\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_12\n"
			"csrw tensor_error, x0\n"
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_12\n"
			"addi x30, x30, 1\n"
			"la x25, rand_ieee754_32\n"
			"flw.ps f19, 96(x25)\n"
			"ffrc.ps f29, f19\n" 
			"LBL_SEQID_2_M0_WRITE_ID_4_HID_12:\n"
			"maskxor m6, m1, m6\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_2_M0_WRITE_ID_5_HID_12:\n"
			"maskxor m3, m6, m1\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"mova.x.m x16\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f14, 384(x7)\n"
			"flw.ps f26, 256(x7)\n"
			"fsgnjx.ps f24, f14, f26\n" 
			"la x22, rand_int_32\n"
			"flw.ps f24, 320(x22)\n"
			"fltu.pi f14, f24, f24\n" 
			"LBL_SEQID_2_M0_WRITE_ID_6_HID_12:\n"
			"fltm.ps m3, f12, f13\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x26, rand_int_32\n"
			"flw.ps f19, 672(x26)\n"
			"flw.ps f23, 544(x26)\n"
			"fand.pi f24, f19, f23\n" 
			"la x7, rand_int_32\n"
			"flw.ps f21, 64(x7)\n"
			"fpackrepb.pi f21, f21\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f29, 736(x26)\n"
			"flw.ps f25, 288(x26)\n"
			"fmax.s f21, f29, f25\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f12, 992(x25)\n"
			"flw.ps f30, 192(x25)\n"
			"fmul.ps f30, f12, f30, rmm\n" 
			"la x19, rand_int_32\n"
			"flw.ps f13, 832(x19)\n"
			"flw.ps f12, 992(x19)\n"
			"for.pi f10, f13, f12\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f10, 704(x16)\n"
			"fcvt.lu.s x7, f10, rdn\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f10, 384(x16)\n"
			"fcvt.pw.ps f21, f10, rmm\n" 
			"la x19, rand_int_32\n"
			"flw.ps f13, 736(x19)\n"
			"fxor.pi f30, f13, f13\n" 
			"la x6, rand_int_32\n"
			"flw.ps f12, 288(x6)\n"
			"flw.ps f17, 992(x6)\n"
			"fand.pi f30, f12, f17\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 0x46bbc6136f001472\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0x2078d82d958fb114\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"LBL_SEQID_2_TLD_XMA_STR_SEQ_ID_441466_HID_12:\n"
			"la x31, self_check_32\n"
			"li x5, 0xc0000000000000\n"
			"add x5, x5, x31\n"
			"li x6, 0x10000000000001\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x1 << TENSOR_FMA32_B_NUM_COLS) | (0x0 << TENSOR_FMA32_A_NUM_ROWS) | (0x1 << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x1 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1a << TENSOR_FMA32_SCP_LOC_B) | (0x6 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x12 << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0x0 << TENSOR_QUANT_QUANT_ROW) | (0x0 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"LBL_SEQID_2_XMA_QNT_STR_SEQ_ITER_0_HID_12:\n"
			"li x6, ((0x2 << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0x5 << TENSOR_QUANT_QUANT_ROW) | (0x26 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x2 << TENSOR_QUANT_TRANSF9) | (0x7 << TENSOR_QUANT_TRANSF8) | (0x6 << TENSOR_QUANT_TRANSF7) | (0x6 << TENSOR_QUANT_TRANSF6) | (0x6 << TENSOR_QUANT_TRANSF5) | (0x6 << TENSOR_QUANT_TRANSF4) | (0x7 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
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
			"LBL_SEQID_3_M0_WRITE_ID_7_HID_12:\n"
			"fltm.pi m2, f17, f3\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x27, rand_ieee754_32\n"
			"flw.ps f15, 640(x27)\n"
			"flw.ps f10, 800(x27)\n"
			"fsgnjx.s f17, f15, f10\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f26, 224(x26)\n"
			"flw.ps f24, 384(x26)\n"
			"fsgnjn.ps f12, f26, f24\n" 
			"la x6, rand_int_32\n"
			"flw.ps f14, 224(x6)\n"
			"flw.ps f19, 128(x6)\n"
			"fmulh.pi f14, f14, f19\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f10, 704(x27)\n"
			"flw.ps f24, 896(x27)\n"
			"flw.ps f17, 32(x27)\n"
			"fmsub.ps f14, f10, f24, f17, rdn\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f30, 0(x16)\n"
			"flw.ps f10, 352(x16)\n"
			"fmax.ps f17, f30, f10\n" 
			"li x11, 0x1ee4ea2f850791df\n"
			"fcvt.s.wu f3, x11, rmm\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f8, 768(x6)\n"
			"flw.ps f26, 0(x6)\n"
			"fmul.s f21, f8, f26, rtz\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x10, rand_ieee754_32\n"
			"flw.ps f15, 320(x10)\n"
			"flw.ps f26, 288(x10)\n"
			"fsgnj.s f15, f15, f26\n" 
			"la x22, rand_int_32\n"
			"flw.ps f3, 960(x22)\n"
			"flw.ps f8, 800(x22)\n"
			"fmin.pi f24, f3, f8\n" 
			"la x19, rand_int_32\n"
			"flw.ps f23, 544(x19)\n"
			"fpackrepb.pi f10, f23\n" 
			"la x6, rand_int_32\n"
			"flw.ps f17, 480(x6)\n"
			"faddi.pi f19, f17, 0x12f\n" 
			"la x16, rand_int_32\n"
			"flw.ps f15, 704(x16)\n"
			"flw.ps f26, 896(x16)\n"
			"fsrl.pi f26, f15, f26\n" 
			"la x7, rand_ieee754_16\n"
			"flw.ps f15, 544(x7)\n"
			"fcvt.ps.f16 f12, f15\n" 
			"la x10, rand_int_32\n"
			"flw.ps f13, 224(x10)\n"
			"flw.ps f17, 864(x10)\n"
			"fminu.pi f14, f13, f17\n" 
			"la x26, rand_int_32\n"
			"flw.ps f30, 576(x26)\n"
			"flw.ps f29, 128(x26)\n"
			"fsub.pi f14, f30, f29\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_3_XMA_QNT_STR_SEQ_ITER_0_HID_12:\n"
			"li x5, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x3 << TENSOR_IMA_B_NUM_COLS) | (0x9 << TENSOR_IMA_A_NUM_ROWS) | (0x2 << TENSOR_IMA_A_NUM_COLS) | (0xa << TENSOR_IMA_OFFSET) | (0x0 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x75 << TENSOR_IMA_SCP_LOC_B) | (0x8c << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
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
			"la x10, rand_int_32\n"
			"flw.ps f12, 0(x10)\n"
			"flw.ps f19, 32(x10)\n"
			"fand.pi f14, f12, f19\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x22, rand_int_32\n"
			"flw.ps f24, 608(x22)\n"
			"flw.ps f14, 448(x22)\n"
			"fand.pi f30, f24, f14\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f29, 832(x6)\n"
			"flw.ps f14, 608(x6)\n"
			"fsgnj.ps f14, f29, f14\n" 
			"li x25, 0x30b18161d7e0e958\n"
			"fcvt.s.l f19, x25, rtz\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f12, 544(x25)\n"
			"fcvt.wu.s x19, f12, rtz\n" 
			"LBL_SEQID_4_M0_WRITE_ID_8_HID_12:\n"
			"fsetm.pi m1, f21\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x26, rand_ieee754_32\n"
			"flw.ps f24, 32(x26)\n"
			"fcvt.wu.s x25, f24, rdn\n" 
			"LBL_SEQID_4_FP_TRANS_ID_9_HID_12:\n"
			"la x11, rand_ieee754_32\n"
			"flw.ps f15, 0(x11)\n"
			"flog.ps f13, f15\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f10, 512(x25)\n"
			"flw.ps f13, 64(x25)\n"
			"flw.ps f26, 416(x25)\n"
			"fmsub.s f10, f10, f13, f26, rup\n" 
			"LBL_SEQID_4_M0_WRITE_ID_10_HID_12:\n"
			"mova.m.x x7\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x22, rand_ieee754_32\n"
			"flw.ps f14, 288(x22)\n"
			"flw.ps f17, 192(x22)\n"
			"fmadd.ps f10, f14, f17, f14, rmm\n" 
			"LBL_SEQID_4_M0_WRITE_ID_11_HID_12:\n"
			"mov.m.x m6, x19, 0x56\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x10, rand_ieee754_32\n"
			"flw.ps f8, 640(x10)\n"
			"flw.ps f12, 992(x10)\n"
			"fmadd.s f13, f8, f12, f12, rmm\n" 
			"mova.x.m x19\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f3, 544(x26)\n"
			"fclass.s x19, f3\n" 
			"la x19, rand_int_32\n"
			"flw.ps f24, 896(x19)\n"
			"flw.ps f3, 512(x19)\n"
			"fmulhu.pi f24, f24, f3\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 0x9bce\n"
			"csrw tensor_mask, x5\n"	
			"LBL_SEQID_4_XMA_QNT_STR_SEQ_ITER_0_HID_12:\n"
			"li x5, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x1 << TENSOR_FMA32_B_NUM_COLS) | (0xb << TENSOR_FMA32_A_NUM_ROWS) | (0xc << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0xc6 << TENSOR_FMA32_SCP_LOC_B) | (0xd3 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x0 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x6, ((0xe << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0xb << TENSOR_QUANT_QUANT_ROW) | (0x1e << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x2 << TENSOR_QUANT_TRANSF5) | (0x6 << TENSOR_QUANT_TRANSF4) | (0x6 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 7\n"
			"csrwi tensor_wait, 10\n"
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
			"csrwi mhpmevent3, 24\n"   
			"csrwi mhpmevent4, 1\n"   
			"csrwi mhpmevent5, 2\n"   
			"csrwi mhpmevent6, 8\n"   
			"csrwi mhpmevent7, 1\n"   
			"csrwi mhpmevent8, 2\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f19,  928 (x5)\n"
			"flw.ps f31,  736 (x5)\n"
			"flw.ps f8,  224 (x5)\n"
			"flw.ps f12,  800 (x5)\n"
			"flw.ps f5,  96 (x5)\n"
			"flw.ps f13,  832 (x5)\n"
			"flw.ps f28,  576 (x5)\n"
			"flw.ps f9,  512 (x5)\n"
			"flw.ps f17,  320 (x5)\n"
			"flw.ps f29,  256 (x5)\n"
			"flw.ps f2,  128 (x5)\n"
			"flw.ps f16,  0 (x5)\n"
			"flw.ps f26,  704 (x5)\n"
			"flw.ps f3,  288 (x5)\n"
			"flw.ps f7,  608 (x5)\n"
			"flw.ps f4,  768 (x5)\n"
			"flw.ps f11,  384 (x5)\n"
			"flw.ps f14,  416 (x5)\n"
			"flw.ps f24,  32 (x5)\n"
			"flw.ps f10,  448 (x5)\n"
			"flw.ps f1,  672 (x5)\n"
			"flw.ps f21,  352 (x5)\n"
			"flw.ps f23,  896 (x5)\n"
			"flw.ps f18,  160 (x5)\n"
			"flw.ps f22,  992 (x5)\n"
			"flw.ps f15,  960 (x5)\n"
			"flw.ps f6,  640 (x5)\n"
			"flw.ps f20,  864 (x5)\n"
			"flw.ps f30,  192 (x5)\n"
			"flw.ps f25,  480 (x5)\n"
			"flw.ps f27,  544 (x5)\n"
			"flw.ps f0,  64 (x5)\n"
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
			"li x29, 0x4\n"
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
			"la x19, rand_ieee754_32\n"
			"flw.ps f27, 544(x19)\n"
			"fcvt.wu.s x9, f27, rtz\n" 
			"la x9, rand_int_32\n"
			"flw.ps f2, 512(x9)\n"
			"flw.ps f0, 288(x9)\n"
			"fand.pi f20, f2, f0\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f13, 768(x27)\n"
			"fcvt.f16.ps f19, f13\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f19, 320(x9)\n"
			"flw.ps f24, 160(x9)\n"
			"flw.ps f9, 640(x9)\n"
			"fnmadd.s f8, f19, f24, f9, rtz\n" 
			"li x27, 0x84b3006a84a9dfc4\n"
			"fcvt.s.wu f24, x27, rtz\n" 
			"la x16, rand_int_32\n"
			"flw.ps f8, 160(x16)\n"
			"fslli.pi f20, f8, 0x9\n" 
			"la x19, rand_int_32\n"
			"flw.ps f23, 768(x19)\n"
			"flw.ps f19, 960(x19)\n"
			"feq.pi f8, f23, f19\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_13:\n"
			"maskand m3, m7, m6\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x9, rand_ieee754_32\n"
			"flw.ps f4, 768(x9)\n"
			"ffrc.ps f23, f4\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_13:\n"
			"maskand m6, m6, m6\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x27, rand_int_32\n"
			"flw.ps f0, 672(x27)\n"
			"fsat8.pi f2, f0\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f24, 32(x19)\n"
			"flw.ps f2, 64(x19)\n"
			"flt.ps f6, f24, f2\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f24, 384(x23)\n"
			"flw.ps f6, 384(x23)\n"
			"fdiv.ps f8, f24, f6, rmm\n" 
			"la x18, rand_int_32\n"
			"flw.ps f0, 192(x18)\n"
			"flw.ps f4, 256(x18)\n"
			"fadd.pi f0, f0, f4\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f19, 128(x17)\n"
			"fclass.s x15, f19\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f25, 960(x15)\n"
			"flw.ps f13, 128(x15)\n"
			"fadd.s f13, f25, f13, rdn\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_13\n"
			"addi x30, x30, 1\n"
			"la x14, rand_ieee754_32\n"
			"flw.ps f7, 576(x14)\n"
			"flw.ps f23, 0(x14)\n"
			"flt.ps f25, f7, f23\n" 
			"la x23, rand_int_32\n"
			"flw.ps f13, 256(x23)\n"
			"flw.ps f8, 640(x23)\n"
			"fsra.pi f0, f13, f8\n" 
			"LBL_SEQID_1_M0_WRITE_ID_2_HID_13:\n"
			"fltm.ps m7, f23, f24\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x9, rand_ieee754_32\n"
			"flw.ps f25, 768(x9)\n"
			"fcvt.w.s x17, f25, rmm\n" 
			"LBL_SEQID_1_M0_WRITE_ID_3_HID_13:\n"
			"flem.ps m2, f13, f6\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0x3f872e4a59\n"
			"flw.ps f5, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x18, rand_ieee754_32\n"
			"flw.ps f6, 384(x18)\n"
			"flw.ps f18, 224(x18)\n"
			"flw.ps f20, 0(x18)\n"
			"fcmov.ps f8, f6, f18, f20\n" 
			"la x15, rand_int_32\n"
			"flw.ps f2, 224(x15)\n"
			"flw.ps f4, 160(x15)\n"
			"fmul.pi f27, f2, f4\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f6, 544(x15)\n"
			"flw.ps f9, 960(x15)\n"
			"flt.ps f8, f6, f9\n" 
			"LBL_SEQID_1_M0_WRITE_ID_4_HID_13:\n"
			"maskand m6, m3, m7\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x17, rand_ieee754_32\n"
			"flw.ps f7, 768(x17)\n"
			"flw.ps f10, 0(x17)\n"
			"flw.ps f8, 320(x17)\n"
			"fmsub.ps f27, f7, f10, f8, rne\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f7, 992(x15)\n"
			"flw.ps f9, 672(x15)\n"
			"fsub.s f4, f7, f9, rmm\n" 
			"LBL_SEQID_1_M0_WRITE_ID_5_HID_13:\n"
			"maskor m6, m3, m3\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x27, rand_int_32\n"
			"flw.ps f6, 896(x27)\n"
			"flw.ps f9, 320(x27)\n"
			"fand.pi f10, f6, f9\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f19, 960(x17)\n"
			"flw.ps f8, 704(x17)\n"
			"fnmsub.s f9, f19, f8, f8, rtz\n" 
			"LBL_SEQID_1_M0_WRITE_ID_6_HID_13:\n"
			"feqm.ps m6, f13, f10\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x15, rand_int_32\n"
			"flw.ps f24, 896(x15)\n"
			"flw.ps f9, 480(x15)\n"
			"fxor.pi f25, f24, f9\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_13\n"
			"addi x30, x30, 1\n"
			"la x15, rand_ieee754_32\n"
			"flw.ps f20, 320(x15)\n"
			"flw.ps f24, 928(x15)\n"
			"fmin.ps f8, f20, f24\n" 
			"la x15, rand_int_32\n"
			"flw.ps f18, 864(x15)\n"
			"flw.ps f10, 320(x15)\n"
			"feq.pi f19, f18, f10\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f24, 576(x18)\n"
			"flw.ps f0, 832(x18)\n"
			"flw.ps f25, 896(x18)\n"
			"fmadd.s f8, f24, f0, f25, rdn\n" 
			"la x25, rand_int_32\n"
			"flw.ps f13, 0(x25)\n"
			"fnot.pi f13, f13\n" 
			"la x23, rand_int_32\n"
			"flw.ps f23, 224(x23)\n"
			"flw.ps f24, 160(x23)\n"
			"fsll.pi f20, f23, f24\n" 
			"la x23, rand_int_32\n"
			"flw.ps f2, 224(x23)\n"
			"flw.ps f0, 608(x23)\n"
			"fle.pi f23, f2, f0\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f4, 32(x18)\n"
			"fcvt.pwu.ps f10, f4, rmm\n" 
			"LBL_SEQID_2_M0_WRITE_ID_7_HID_13:\n"
			"fltm.ps m2, f23, f19\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x16, rand_int_32\n"
			"flw.ps f8, 736(x16)\n"
			"flw.ps f27, 512(x16)\n"
			"frem.pi f27, f8, f27\n" 
			"la x27, rand_int_32\n"
			"flw.ps f23, 992(x27)\n"
			"flw.ps f27, 288(x27)\n"
			"fmin.pi f0, f23, f27\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f7, 96(x17)\n"
			"flw.ps f18, 736(x17)\n"
			"fsgnjx.s f27, f7, f18\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f19, 736(x15)\n"
			"fcvt.pwu.ps f18, f19, rne\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f4, 160(x23)\n"
			"flw.ps f23, 960(x23)\n"
			"fmin.s f9, f4, f23\n" 
			"la x23, rand_int_32\n"
			"flw.ps f8, 736(x23)\n"
			"flw.ps f27, 0(x23)\n"
			"fmax.pi f7, f8, f27\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f2, 800(x18)\n"
			"fcvt.w.s x16, f2, rdn\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f23, 160(x18)\n"
			"flw.ps f25, 448(x18)\n"
			"flw.ps f10, 384(x18)\n"
			"fnmsub.ps f24, f23, f25, f10, rdn\n" 
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
			"la x18, rand_int_32\n"
			"flw.ps f18, 544(x18)\n"
			"fsub.pi f4, f18, f18\n" 
			"li x16, 0xbcf6ba142cee3db8\n"
			"fcvt.s.wu f27, x16, rmm\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f8, 800(x27)\n"
			"flw.ps f13, 544(x27)\n"
			"flt.ps f2, f8, f13\n" 
			"la x9, rand_int_32\n"
			"flw.ps f6, 512(x9)\n"
			"flw.ps f25, 64(x9)\n"
			"fmin.pi f0, f6, f25\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f10, 576(x25)\n"
			"flw.ps f25, 608(x25)\n"
			"fmul.s f6, f10, f25, rne\n" 
			"la x27, rand_int_32\n"
			"flw.ps f9, 64(x27)\n"
			"fandi.pi f7, f9, 0xb5\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f24, 800(x25)\n"
			"flw.ps f0, 192(x25)\n"
			"fmax.s f13, f24, f0\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f13, 768(x19)\n"
			"flw.ps f20, 768(x19)\n"
			"fle.s x27, f13, f20\n" 
			"LBL_SEQID_3_M0_WRITE_ID_8_HID_13:\n"
			"fsetm.pi m7, f19\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x18, rand_ieee754_32\n"
			"flw.ps f23, 0(x18)\n"
			"flw.ps f4, 640(x18)\n"
			"fsgnjn.ps f18, f23, f4\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f0, 320(x19)\n"
			"flw.ps f27, 896(x19)\n"
			"feq.ps f18, f0, f27\n" 
			"la x17, rand_int_32\n"
			"flw.ps f18, 448(x17)\n"
			"fmul.pi f10, f18, f18\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f4, 320(x25)\n"
			"flw.ps f27, 832(x25)\n"
			"fmin.ps f7, f4, f27\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f25, 576(x14)\n"
			"flw.ps f4, 704(x14)\n"
			"fsub.ps f19, f25, f4, rtz\n" 
			"LBL_SEQID_3_FP_TRANS_ID_9_HID_13:\n"
			"la x27, rand_ieee754_32\n"
			"flw.ps f25, 320(x27)\n"
			"fexp.ps f2, f25\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f19, 64(x18)\n"
			"flw.ps f4, 480(x18)\n"
			"fmin.s f13, f19, f4\n" 
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
			"LBL_SEQID_4_M0_WRITE_ID_10_HID_13:\n"
			"maskor m6, m7, m2\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x16, rand_int_32\n"
			"flw.ps f18, 704(x16)\n"
			"fandi.pi f25, f18, 0x162\n" 
			"la x9, rand_int_32\n"
			"flw.ps f7, 832(x9)\n"
			"flw.ps f13, 0(x9)\n"
			"flt.pi f20, f7, f13\n" 
			"la x23, rand_int_32\n"
			"flw.ps f19, 192(x23)\n"
			"fsrli.pi f7, f19, 0xd\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f13, 448(x17)\n"
			"fround.ps f4, f13, rup\n" 
			"LBL_SEQID_4_M0_WRITE_ID_11_HID_13:\n"
			"mova.m.x x23\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x15, rand_ieee754_32\n"
			"flw.ps f10, 288(x15)\n"
			"fcvt.w.s x19, f10, rmm\n" 
			"la x19, rand_int_32\n"
			"flw.ps f18, 192(x19)\n"
			"fsat8.pi f4, f18\n" 
			"la x17, rand_int_32\n"
			"flw.ps f27, 256(x17)\n"
			"flw.ps f4, 640(x17)\n"
			"fadd.pi f27, f27, f4\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f18, 128(x9)\n"
			"fle.s x16, f18, f18\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f25, 384(x9)\n"
			"flw.ps f0, 320(x9)\n"
			"fsgnjx.s f4, f25, f0\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f25, 128(x17)\n"
			"flw.ps f10, 576(x17)\n"
			"flw.ps f6, 128(x17)\n"
			"fcmov.ps f25, f25, f10, f6\n" 
			"LBL_SEQID_4_M0_WRITE_ID_12_HID_13:\n"
			"feqm.ps m7, f25, f4\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"li x27, 0x31d1f4782c13640c\n"
			"fcvt.s.w f19, x27, rtz\n" 
			"li x27, 0x61dc7b7e15702c70\n"
			"fcvt.s.w f25, x27, rup\n" 
			"la x14, rand_int_32\n"
			"flw.ps f6, 160(x14)\n"
			"flw.ps f4, 512(x14)\n"
			"fadd.pi f8, f6, f4\n" 
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
			"csrwi mhpmevent3, 8\n"   
			"csrwi mhpmevent4, 4\n"   
			"csrwi mhpmevent5, 21\n"   
			"csrwi mhpmevent6, 6\n"   
			"csrwi mhpmevent7, 4\n"   
			"csrwi mhpmevent8, 5\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f5,  672 (x5)\n"
			"flw.ps f20,  480 (x5)\n"
			"flw.ps f9,  512 (x5)\n"
			"flw.ps f24,  96 (x5)\n"
			"flw.ps f13,  0 (x5)\n"
			"flw.ps f21,  608 (x5)\n"
			"flw.ps f26,  192 (x5)\n"
			"flw.ps f25,  704 (x5)\n"
			"flw.ps f12,  768 (x5)\n"
			"flw.ps f14,  288 (x5)\n"
			"flw.ps f7,  544 (x5)\n"
			"flw.ps f11,  960 (x5)\n"
			"flw.ps f15,  224 (x5)\n"
			"flw.ps f16,  448 (x5)\n"
			"flw.ps f3,  384 (x5)\n"
			"flw.ps f29,  256 (x5)\n"
			"flw.ps f17,  896 (x5)\n"
			"flw.ps f6,  576 (x5)\n"
			"flw.ps f18,  864 (x5)\n"
			"flw.ps f0,  160 (x5)\n"
			"flw.ps f31,  992 (x5)\n"
			"flw.ps f30,  736 (x5)\n"
			"flw.ps f28,  128 (x5)\n"
			"flw.ps f1,  416 (x5)\n"
			"flw.ps f19,  928 (x5)\n"
			"flw.ps f4,  352 (x5)\n"
			"flw.ps f8,  64 (x5)\n"
			"flw.ps f10,  640 (x5)\n"
			"flw.ps f23,  800 (x5)\n"
			"flw.ps f2,  32 (x5)\n"
			"flw.ps f27,  832 (x5)\n"
			"flw.ps f22,  320 (x5)\n"
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
			"li x29, 0x4\n"
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
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_14:\n"
			"fltm.ps m0, f14, f14\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x27, rand_ieee754_32\n"
			"flw.ps f0, 384(x27)\n"
			"flw.ps f19, 864(x27)\n"
			"fsgnj.s f0, f0, f19\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f0, 128(x27)\n"
			"flw.ps f19, 544(x27)\n"
			"flw.ps f31, 544(x27)\n"
			"fmadd.ps f8, f0, f19, f31, rup\n" 
			"la x23, rand_int_32\n"
			"flw.ps f0, 704(x23)\n"
			"fnot.pi f23, f0\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f1, 352(x26)\n"
			"flw.ps f3, 0(x26)\n"
			"fmadd.ps f17, f1, f3, f1, rne\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f20, 800(x27)\n"
			"flw.ps f26, 768(x27)\n"
			"fmin.ps f4, f20, f26\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f31, 736(x22)\n"
			"fsin.ps f8, f31\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_14:\n"
			"masknot m0, m2\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x17, rand_ieee754_32\n"
			"flw.ps f28, 128(x17)\n"
			"flw.ps f30, 864(x17)\n"
			"fsub.s f3, f28, f30, rmm\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f1, 832(x15)\n"
			"flw.ps f30, 256(x15)\n"
			"fdiv.s f31, f1, f30, rtz\n" 
			"la x12, rand_int_32\n"
			"flw.ps f28, 0(x12)\n"
			"fandi.pi f17, f28, 0x1ca\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f20, 192(x22)\n"
			"fround.ps f25, f20, rne\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f28, 224(x17)\n"
			"flw.ps f3, 160(x17)\n"
			"fsgnj.s f31, f28, f3\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f0, 640(x17)\n"
			"fclass.ps f8, f0\n" 
			"la x6, rand_int_32\n"
			"flw.ps f30, 960(x6)\n"
			"flw.ps f14, 800(x6)\n"
			"fsll.pi f4, f30, f14\n" 
			"la x23, rand_int_32\n"
			"flw.ps f10, 64(x23)\n"
			"flw.ps f23, 448(x23)\n"
			"fmul.pi f23, f10, f23\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_0_RDX_ID_0_SHIRE_0_HID_14:\n"
			"li x23, 0xc00000002640002\n"
			"csrw tensor_reduce, x23\n" 
			"csrwi tensor_wait, 9\n" 
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=0); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ITER_0_HID_14:\n"
			"la x31, self_check_32\n"
			"li x5, 0x40000000000005\n"
			"add x5, x5, x31\n"
			"li x6, 0x1000000000000c\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x2 << TENSOR_FMA32_B_NUM_COLS) | (0x5 << TENSOR_FMA32_A_NUM_ROWS) | (0xc << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x1 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1a << TENSOR_FMA32_SCP_LOC_B) | (0x2 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x9, ((0xe << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0x5 << TENSOR_QUANT_QUANT_ROW) | (0x1a << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x7 << TENSOR_QUANT_TRANSF4) | (0x6 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0xd\n"
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
			"la x7, rand_ieee754_32\n"
			"flw.ps f4, 0(x7)\n"
			"fsgnjn.s f30, f4, f4\n" 
			"la x23, rand_ieee754_16\n"
			"flw.ps f31, 384(x23)\n"
			"fcvt.ps.f16 f30, f31\n" 
			"la x6, rand_int_32\n"
			"flw.ps f1, 192(x6)\n"
			"flw.ps f23, 352(x6)\n"
			"fmul.pi f19, f1, f23\n" 
			"fbci.pi f26, 0x1e801\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f28, 960(x26)\n"
			"fcvt.wu.s x23, f28, rmm\n" 
			"li x12, 0x73682ccd71ac1705\n"
			"fcvt.s.l f28, x12, rup\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f3, 128(x26)\n"
			"flw.ps f26, 512(x26)\n"
			"fsgnjx.s f8, f3, f26\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f4, 544(x12)\n"
			"flw.ps f1, 96(x12)\n"
			"flw.ps f25, 896(x12)\n"
			"fmsub.ps f30, f4, f1, f25, rtz\n" 
			"la x15, rand_int_32\n"
			"flw.ps f31, 128(x15)\n"
			"fandi.pi f30, f31, 0x21\n" 
			"la x27, rand_int_32\n"
			"flw.ps f26, 320(x27)\n"
			"flw.ps f0, 320(x27)\n"
			"for.pi f31, f26, f0\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f10, 448(x12)\n"
			"fcvt.l.s x7, f10, rmm\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f25, 928(x7)\n"
			"flw.ps f23, 352(x7)\n"
			"flw.ps f10, 800(x7)\n"
			"fcmov.ps f4, f25, f23, f10\n" 
			"la x7, rand_int_32\n"
			"flw.ps f3, 768(x7)\n"
			"flw.ps f1, 672(x7)\n"
			"fltu.pi f8, f3, f1\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f8, 640(x20)\n"
			"flw.ps f0, 768(x20)\n"
			"flt.s x27, f8, f0\n" 
			"LBL_SEQID_1_M0_WRITE_ID_2_HID_14:\n"
			"mov.m.x m7, x25, 0x7d\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x15, rand_ieee754_32\n"
			"flw.ps f0, 128(x15)\n"
			"fcvt.f16.ps f26, f0\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_1_TLD_XMA_STR_SEQ_ID_959636_HID_14:\n"
			"la x31, self_check_16\n"
			"li x5, 0x60000000000000\n"
			"add x5, x5, x31\n"
			"li x6, 0x3c000000000000e\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x3 << TENSOR_FMA16_B_NUM_COLS) | (0x0 << TENSOR_FMA16_A_NUM_ROWS) | (0xe << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1e << TENSOR_FMA16_SCP_LOC_B) | (0x3 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x1a << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0x0 << TENSOR_QUANT_QUANT_ROW) | (0x35 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x7 << TENSOR_QUANT_TRANSF6) | (0x6 << TENSOR_QUANT_TRANSF5) | (0x6 << TENSOR_QUANT_TRANSF4) | (0x7 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0x1e\n"
			"bne x5, x31, LBL_FAIL_HID_14\n"
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 10\n"
			"LBL_SEQID_1_TLD_XMA_STR_SEQ_ID_138744_HID_14:\n"
			"la x31, self_check_16\n"
			"li x5, 0x8\n"
			"add x5, x5, x31\n"
			"li x6, 0x1000000000000f\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x1 << TENSOR_FMA16_B_NUM_COLS) | (0x8 << TENSOR_FMA16_A_NUM_ROWS) | (0xf << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x1 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1a << TENSOR_FMA16_SCP_LOC_B) | (0x0 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x17 << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0x8 << TENSOR_QUANT_QUANT_ROW) | (0x18 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x7 << TENSOR_QUANT_TRANSF6) | (0x7 << TENSOR_QUANT_TRANSF5) | (0x6 << TENSOR_QUANT_TRANSF4) | (0x7 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0x20\n"
			"bne x5, x31, LBL_FAIL_HID_14\n"
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 10\n"
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
			"la x31, self_check_16\n"
			"li x5, 0x40000000000004\n"
			"add x5, x5, x31\n"
			"li x6, 0x10000000000006\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x1 << TENSOR_FMA16_B_NUM_COLS) | (0x4 << TENSOR_FMA16_A_NUM_ROWS) | (0x6 << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x1 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1c << TENSOR_FMA16_SCP_LOC_B) | (0x2 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x9, ((0xc << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0x4 << TENSOR_QUANT_QUANT_ROW) | (0x1e << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x7 << TENSOR_QUANT_TRANSF8) | (0x6 << TENSOR_QUANT_TRANSF7) | (0x7 << TENSOR_QUANT_TRANSF6) | (0x7 << TENSOR_QUANT_TRANSF5) | (0x7 << TENSOR_QUANT_TRANSF4) | (0x7 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
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
			"la x25, rand_ieee754_32\n"
			"flw.ps f0, 352(x25)\n"
			"flw.ps f8, 352(x25)\n"
			"fmul.ps f20, f0, f8, rmm\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f17, 480(x27)\n"
			"flw.ps f28, 352(x27)\n"
			"fmin.s f0, f17, f28\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f0, 352(x23)\n"
			"flw.ps f17, 480(x23)\n"
			"fmul.s f3, f0, f17, rtz\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f4, 160(x7)\n"
			"ffrc.ps f25, f4\n" 
			"la x6, rand_int_32\n"
			"flw.ps f30, 640(x6)\n"
			"fandi.pi f31, f30, 0xc1\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f30, 640(x26)\n"
			"flw.ps f20, 960(x26)\n"
			"fdiv.s f19, f30, f20, rmm\n" 
			"la x15, rand_int_32\n"
			"flw.ps f20, 64(x15)\n"
			"flw.ps f8, 288(x15)\n"
			"fmaxu.pi f30, f20, f8\n" 
			"li x5, 0x2aa6b83d06d01b31\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0xe8ad125c1beaca2c\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"la x7, rand_ieee754_32\n"
			"flw.ps f25, 512(x7)\n"
			"flw.ps f14, 768(x7)\n"
			"fmax.s f31, f25, f14\n" 
			"li x5, 0x8e6204d20e39c927\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0xd930b241c4f199d4\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"la x26, rand_ieee754_32\n"
			"flw.ps f8, 256(x26)\n"
			"fcvt.wu.s x6, f8, rup\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f1, 448(x6)\n"
			"flw.ps f23, 192(x6)\n"
			"fmul.s f25, f1, f23, rtz\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f1, 384(x22)\n"
			"fswizz.ps f26, f1, 0xd\n" 
			"la x22, rand_int_32\n"
			"flw.ps f31, 416(x22)\n"
			"flw.ps f28, 928(x22)\n"
			"fle.pi f1, f31, f28\n" 
			"LBL_SEQID_2_M0_WRITE_ID_3_HID_14:\n"
			"mova.m.x x20\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x22, rand_ieee754_32\n"
			"flw.ps f26, 128(x22)\n"
			"flw.ps f20, 480(x22)\n"
			"fadd.s f4, f26, f20, rup\n" 
			"li x25, 0x13c45a10cc894961\n"
			"fcvt.s.wu f0, x25, rdn\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f31, 192(x26)\n"
			"flw.ps f25, 224(x26)\n"
			"fsgnjn.ps f4, f31, f25\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_2_TLD_XMA_STR_SEQ_ITER_0_HID_14:\n"
			"la x31, self_check_16\n"
			"li x5, 0x4000000000000a\n"
			"add x5, x5, x31\n"
			"li x6, 0x36000000000000d\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x2 << TENSOR_FMA16_B_NUM_COLS) | (0xa << TENSOR_FMA16_A_NUM_ROWS) | (0xd << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1b << TENSOR_FMA16_SCP_LOC_B) | (0x2 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x14 << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0xa << TENSOR_QUANT_QUANT_ROW) | (0x25 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x2 << TENSOR_QUANT_TRANSF5) | (0x7 << TENSOR_QUANT_TRANSF4) | (0x7 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"bne x5, x31, LBL_FAIL_HID_14\n"
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 10\n"
			"LBL_SEQID_2_TLD_XMA_STR_SEQ_ITER_1_HID_14:\n"
			"la x31, self_check_16\n"
			"li x5, 0x8000000000000e\n"
			"add x5, x5, x31\n"
			"li x6, 0x32000000000000a\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x0 << TENSOR_FMA16_B_NUM_COLS) | (0xe << TENSOR_FMA16_A_NUM_ROWS) | (0xa << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x19 << TENSOR_FMA16_SCP_LOC_B) | (0x4 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
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
			"bne x5, x31, LBL_FAIL_HID_14\n"
			"LBL_SEQID_2_TLD_XMA_STR_SEQ_ITER_2_HID_14:\n"
			"la x31, self_check_16\n"
			"li x5, 0x4000000000000e\n"
			"add x5, x5, x31\n"
			"li x6, 0x10000000000007\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x3 << TENSOR_FMA16_B_NUM_COLS) | (0xe << TENSOR_FMA16_A_NUM_ROWS) | (0x7 << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x1 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1b << TENSOR_FMA16_SCP_LOC_B) | (0x2 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0x10\n"
			"bne x5, x31, LBL_FAIL_HID_14\n"
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
			"la x17, rand_ieee754_32\n"
			"flw.ps f0, 896(x17)\n"
			"flw.ps f28, 160(x17)\n"
			"feq.s x12, f0, f28\n" 
			"LBL_SEQID_3_M0_WRITE_ID_4_HID_14:\n"
			"mov.m.x m7, x25, 0x71\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0x52a438eb98\n"
			"flw.ps f5, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_3_M0_WRITE_ID_5_HID_14:\n"
			"mov.m.x m2, x7, 0x60\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x17, rand_ieee754_32\n"
			"flw.ps f1, 544(x17)\n"
			"flw.ps f23, 832(x17)\n"
			"fadd.ps f19, f1, f23, rup\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f28, 864(x17)\n"
			"flw.ps f19, 352(x17)\n"
			"fmax.s f1, f28, f19\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f1, 416(x27)\n"
			"flw.ps f25, 320(x27)\n"
			"feq.ps f30, f1, f25\n" 
			"li x23, 0xb77642aa4b35a84\n"
			"fcvt.s.w f1, x23, rup\n" 
			"LBL_SEQID_3_M0_WRITE_ID_6_HID_14:\n"
			"masknot m2, m2\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x25, rand_ieee754_32\n"
			"flw.ps f8, 576(x25)\n"
			"fcvt.f16.ps f20, f8\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f20, 352(x20)\n"
			"flw.ps f8, 352(x20)\n"
			"fmsub.ps f14, f20, f8, f8, rdn\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f25, 960(x22)\n"
			"flw.ps f26, 416(x22)\n"
			"fmul.ps f0, f25, f26, rmm\n" 
			"LBL_SEQID_3_M0_WRITE_ID_7_HID_14:\n"
			"maskand m3, m2, m2\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0x8bd02212a2\n"
			"flw.ps f5, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x25, rand_int_32\n"
			"flw.ps f20, 32(x25)\n"
			"fsrli.pi f10, f20, 0x6\n" 
			"la x7, rand_int_32\n"
			"flw.ps f8, 0(x7)\n"
			"flw.ps f25, 192(x7)\n"
			"fmulhu.pi f25, f8, f25\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f1, 544(x27)\n"
			"fcvt.pwu.ps f10, f1, rmm\n" 
			"la x12, rand_int_32\n"
			"flw.ps f0, 800(x12)\n"
			"flw.ps f30, 96(x12)\n"
			"fmaxu.pi f0, f0, f30\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 0x7e3b\n"
			"csrw tensor_mask, x5\n"	
			"LBL_SEQID_3_XMA_QNT_STR_SEQ_ITER_0_HID_14:\n"
			"li x6, ((0x4 << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0x7 << TENSOR_QUANT_QUANT_ROW) | (0x31 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x7 << TENSOR_QUANT_TRANSF6) | (0x6 << TENSOR_QUANT_TRANSF5) | (0x6 << TENSOR_QUANT_TRANSF4) | (0x7 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_3_XMA_QNT_STR_SEQ_ITER_1_HID_14:\n"
			"li x5, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x1 << TENSOR_IMA_B_NUM_COLS) | (0xb << TENSOR_IMA_A_NUM_ROWS) | (0x5 << TENSOR_IMA_A_NUM_COLS) | (0xe << TENSOR_IMA_OFFSET) | (0x0 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0xa5 << TENSOR_IMA_SCP_LOC_B) | (0x15 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x0 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x6, ((0x1 << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0xb << TENSOR_QUANT_QUANT_ROW) | (0x21 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x5 << TENSOR_QUANT_TRANSF1) | (0x3 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"la x15, rand_int_32\n"
			"flw.ps f30, 96(x15)\n"
			"flw.ps f19, 352(x15)\n"
			"fsll.pi f31, f30, f19\n" 
			"mova.x.m x7\n" 
			"la x22, rand_int_32\n"
			"flw.ps f17, 320(x22)\n"
			"flw.ps f14, 640(x22)\n"
			"fsll.pi f28, f17, f14\n" 
			"li x20, 0x135a0ae5dbe45244\n"
			"fcvt.s.wu f14, x20, rdn\n" 
			"li x5, 0x903d4b042f3e1217\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0xf0ebc0ee104644e2\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"la x6, rand_int_32\n"
			"flw.ps f20, 704(x6)\n"
			"fmulh.pi f3, f20, f20\n" 
			"la x25, rand_int_32\n"
			"flw.ps f17, 32(x25)\n"
			"flw.ps f25, 224(x25)\n"
			"fmul.pi f23, f17, f25\n" 
			"LBL_SEQID_4_M0_WRITE_ID_8_HID_14:\n"
			"mova.m.x x17\n" 
			"li x5, CREDINC3\n"
			"ld x5, 0(x5)\n"
			"LBL_SEQID_4_M0_WRITE_ID_9_HID_14:\n"
			"maskor m2, m0, m7\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_4_M0_WRITE_ID_10_HID_14:\n"
			"maskand m3, m2, m3\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0x9e238019e7\n"
			"fsw.ps f0, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_4_M0_WRITE_ID_11_HID_14:\n"
			"mov.m.x m2, x17, 0x6e\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x27, rand_ieee754_32\n"
			"flw.ps f19, 832(x27)\n"
			"flw.ps f0, 544(x27)\n"
			"flw.ps f3, 960(x27)\n"
			"fcmov.ps f30, f19, f0, f3\n" 
			"la x26, rand_int_32\n"
			"flw.ps f0, 960(x26)\n"
			"flw.ps f10, 512(x26)\n"
			"fsra.pi f20, f0, f10\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f4, 0(x25)\n"
			"flw.ps f10, 224(x25)\n"
			"feq.ps f25, f4, f10\n" 
			"la x23, rand_int_32\n"
			"flw.ps f1, 512(x23)\n"
			"flw.ps f28, 608(x23)\n"
			"fadd.pi f10, f1, f28\n" 
			"LBL_SEQID_4_M0_WRITE_ID_12_HID_14:\n"
			"mova.m.x x22\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x7, rand_int_32\n"
			"flw.ps f17, 0(x7)\n"
			"flw.ps f20, 704(x7)\n"
			"fmulhu.pi f26, f17, f20\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 0xeea0\n"
			"csrw tensor_mask, x5\n"	
			"LBL_SEQID_4_TLD_XMA_STR_SEQ_ID_761883_HID_14:\n"
			"la x31, self_check_16\n"
			"li x5, 0x60000000000006\n"
			"add x5, x5, x31\n"
			"li x6, 0x1000000000000c\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x3 << TENSOR_FMA16_B_NUM_COLS) | (0x6 << TENSOR_FMA16_A_NUM_ROWS) | (0xc << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x1 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x18 << TENSOR_FMA16_SCP_LOC_B) | (0x3 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0x1a\n"
			"bne x5, x31, LBL_FAIL_HID_14\n"
			"LBL_SEQID_4_TLD_XMA_STR_SEQ_ID_912117_HID_14:\n"
			"la x31, self_check_8\n"
			"li x5, 0x5\n"
			"add x5, x5, x31\n"
			"li x6, 0x360000000000002\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x3 << TENSOR_IMA_B_NUM_COLS) | (0x5 << TENSOR_IMA_A_NUM_ROWS) | (0x2 << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1b << TENSOR_IMA_SCP_LOC_B) | (0x0 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
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
			"bne x5, x31, LBL_FAIL_HID_14\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_14\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_4_XMA_QNT_STR_SEQ_ITER_0_HID_14:\n"
			"li x6, ((0x19 << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0x3 << TENSOR_QUANT_QUANT_ROW) | (0x3 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x5 << TENSOR_QUANT_TRANSF2) | (0x3 << TENSOR_QUANT_TRANSF1) | (0x8 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_4_XMA_QNT_STR_SEQ_ITER_1_HID_14:\n"
			"li x6, ((0x1c << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0xb << TENSOR_QUANT_QUANT_ROW) | (0x11 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x2 << TENSOR_QUANT_TRANSF4) | (0x6 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"csrwi mhpmevent3, 24\n"   
			"csrwi mhpmevent4, 1\n"   
			"csrwi mhpmevent5, 2\n"   
			"csrwi mhpmevent6, 8\n"   
			"csrwi mhpmevent7, 1\n"   
			"csrwi mhpmevent8, 2\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f29,  448 (x5)\n"
			"flw.ps f13,  288 (x5)\n"
			"flw.ps f4,  608 (x5)\n"
			"flw.ps f23,  224 (x5)\n"
			"flw.ps f16,  640 (x5)\n"
			"flw.ps f1,  32 (x5)\n"
			"flw.ps f8,  320 (x5)\n"
			"flw.ps f7,  576 (x5)\n"
			"flw.ps f22,  512 (x5)\n"
			"flw.ps f28,  800 (x5)\n"
			"flw.ps f18,  864 (x5)\n"
			"flw.ps f17,  768 (x5)\n"
			"flw.ps f25,  0 (x5)\n"
			"flw.ps f9,  192 (x5)\n"
			"flw.ps f27,  160 (x5)\n"
			"flw.ps f6,  96 (x5)\n"
			"flw.ps f31,  544 (x5)\n"
			"flw.ps f15,  832 (x5)\n"
			"flw.ps f5,  672 (x5)\n"
			"flw.ps f24,  736 (x5)\n"
			"flw.ps f2,  64 (x5)\n"
			"flw.ps f10,  928 (x5)\n"
			"flw.ps f21,  256 (x5)\n"
			"flw.ps f20,  128 (x5)\n"
			"flw.ps f11,  992 (x5)\n"
			"flw.ps f12,  384 (x5)\n"
			"flw.ps f3,  704 (x5)\n"
			"flw.ps f14,  416 (x5)\n"
			"flw.ps f0,  480 (x5)\n"
			"flw.ps f26,  896 (x5)\n"
			"flw.ps f19,  960 (x5)\n"
			"flw.ps f30,  352 (x5)\n"
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
			"li x29, 0x4\n"
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
			"la x18, rand_ieee754_32\n"
			"flw.ps f8, 384(x18)\n"
			"flw.ps f2, 896(x18)\n"
			"fsgnjn.s f14, f8, f2\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_15:\n"
			"flem.ps m7, f20, f2\n" 
			"li x5, 0x80340158\n"
			"ld x5, 0(x5)\n"
			"la x17, rand_ieee754_32\n"
			"flw.ps f5, 32(x17)\n"
			"flw.ps f12, 416(x17)\n"
			"flw.ps f21, 448(x17)\n"
			"fmadd.ps f14, f5, f12, f21, rmm\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f1, 896(x23)\n"
			"flw.ps f21, 128(x23)\n"
			"flw.ps f8, 160(x23)\n"
			"fmsub.s f29, f1, f21, f8, rdn\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f19, 352(x12)\n"
			"fclass.ps f8, f19\n" 
			"la x5, rand_int_32\n"
			"flw.ps f29, 896(x5)\n"
			"flw.ps f8, 640(x5)\n"
			"fremu.pi f20, f29, f8\n" 
			"fbci.ps f20, 0x12c7f\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f18, 704(x11)\n"
			"fmul.s f0, f18, f18, rtz\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f31, 928(x18)\n"
			"flw.ps f16, 64(x18)\n"
			"fmin.ps f0, f31, f16\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f14, 448(x12)\n"
			"flw.ps f29, 704(x12)\n"
			"flt.s x16, f14, f29\n" 
			"li x25, 0xf4c555188d910438\n"
			"fcvt.s.l f1, x25, rup\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f31, 256(x7)\n"
			"flw.ps f16, 544(x7)\n"
			"fmax.s f14, f31, f16\n" 
			"LBL_SEQID_0_FP_TRANS_ID_1_HID_15:\n"
			"la x12, rand_ieee754_32\n"
			"flw.ps f5, 224(x12)\n"
			"flog.ps f0, f5\n" 
			"li x23, 0xae06ba2cb70cac94\n"
			"fcvt.s.wu f25, x23, rdn\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f29, 192(x12)\n"
			"ffrc.ps f1, f29\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f11, 544(x11)\n"
			"fcvt.pwu.ps f31, f11, rtz\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_15\n"
			"addi x30, x30, 1\n"
			"la x11, rand_int_32\n"
			"flw.ps f25, 736(x11)\n"
			"fsll.pi f21, f25, f25\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f16, 768(x16)\n"
			"fcvt.pwu.ps f20, f16, rdn\n" 
			"maskpopcz x23, m4\n" 
			"LBL_SEQID_1_M0_WRITE_ID_2_HID_15:\n"
			"feqm.ps m3, f14, f29\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0xef398511ce\n"
			"flw.ps f5, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x25, rand_int_32\n"
			"flw.ps f0, 640(x25)\n"
			"fcvt.ps.pwu f2, f0, rdn\n" 
			"fbci.ps f18, 0x511a2\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f1, 800(x28)\n"
			"flw.ps f29, 320(x28)\n"
			"fadd.ps f19, f1, f29, rdn\n" 
			"la x25, rand_int_32\n"
			"flw.ps f8, 384(x25)\n"
			"fnot.pi f19, f8\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f1, 448(x18)\n"
			"flw.ps f5, 480(x18)\n"
			"fsgnjx.ps f16, f1, f5\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f20, 352(x18)\n"
			"flw.ps f1, 576(x18)\n"
			"fsgnjn.ps f1, f20, f1\n" 
			"LBL_SEQID_1_M0_WRITE_ID_3_HID_15:\n"
			"maskand m0, m7, m3\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0x87600669e4\n"
			"fsw.ps f0, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x12, rand_ieee754_32\n"
			"flw.ps f20, 896(x12)\n"
			"flw.ps f16, 32(x12)\n"
			"fsgnjn.s f11, f20, f16\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f19, 224(x7)\n"
			"flw.ps f25, 320(x7)\n"
			"fmul.s f31, f19, f25, rtz\n" 
			"la x16, rand_int_32\n"
			"flw.ps f11, 416(x16)\n"
			"flw.ps f12, 160(x16)\n"
			"fmulh.pi f1, f11, f12\n" 
			"la x18, rand_int_32\n"
			"flw.ps f8, 672(x18)\n"
			"fminu.pi f2, f8, f8\n" 
			"LBL_SEQID_1_M0_WRITE_ID_4_HID_15:\n"
			"maskxor m7, m7, m0\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0xFF\n"
			"csrw tensor_error, zero\n" 
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_15\n"
			"addi x30, x30, 1\n"
			"la x7, rand_int_32\n"
			"flw.ps f21, 896(x7)\n"
			"flw.ps f1, 768(x7)\n"
			"fsub.pi f31, f21, f1\n" 
			"LBL_SEQID_2_M0_WRITE_ID_5_HID_15:\n"
			"fltm.pi m0, f11, f5\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x17, rand_ieee754_32\n"
			"flw.ps f8, 448(x17)\n"
			"frsq.ps f12, f8\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f0, 0(x16)\n"
			"flw.ps f5, 32(x16)\n"
			"flw.ps f12, 992(x16)\n"
			"fnmsub.ps f12, f0, f5, f12, rup\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f21, 576(x18)\n"
			"flw.ps f1, 768(x18)\n"
			"feq.ps f14, f21, f1\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f12, 832(x12)\n"
			"flw.ps f1, 448(x12)\n"
			"fsub.ps f11, f12, f1, rmm\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f16, 512(x25)\n"
			"flw.ps f1, 992(x25)\n"
			"flw.ps f11, 672(x25)\n"
			"fnmsub.s f18, f16, f1, f11, rne\n" 
			"LBL_SEQID_2_M0_WRITE_ID_6_HID_15:\n"
			"maskand m3, m7, m3\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x7, rand_int_32\n"
			"flw.ps f21, 544(x7)\n"
			"flw.ps f14, 608(x7)\n"
			"fmulhu.pi f31, f21, f14\n" 
			"li x25, 0x629112727fa15a23\n"
			"fcvt.s.wu f21, x25, rmm\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f12, 224(x17)\n"
			"flw.ps f11, 256(x17)\n"
			"fmsub.ps f0, f12, f11, f11, rtz\n" 
			"la x16, rand_int_32\n"
			"flw.ps f25, 128(x16)\n"
			"flw.ps f11, 544(x16)\n"
			"fadd.pi f2, f25, f11\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f8, 928(x7)\n"
			"fmin.ps f5, f8, f8\n" 
			"la x28, rand_ieee754_16\n"
			"flw.ps f25, 960(x28)\n"
			"fcvt.ps.f16 f21, f25\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f21, 864(x13)\n"
			"fround.ps f12, f21, rne\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f1, 704(x18)\n"
			"flw.ps f21, 896(x18)\n"
			"fadd.s f29, f1, f21, rtz\n" 
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
			"la x28, rand_int_32\n"
			"flw.ps f2, 608(x28)\n"
			"flw.ps f21, 704(x28)\n"
			"fsub.pi f1, f2, f21\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f11, 576(x18)\n"
			"flw.ps f0, 640(x18)\n"
			"fmul.ps f20, f11, f0, rtz\n" 
			"mova.x.m x17\n" 
			"li x18, 0x8009c98f296d6bd0\n"
			"fcvt.s.wu f14, x18, rne\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f12, 960(x25)\n"
			"fclass.s x5, f12\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f19, 544(x16)\n"
			"flw.ps f8, 928(x16)\n"
			"flw.ps f18, 64(x16)\n"
			"fmsub.s f11, f19, f8, f18, rdn\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f11, 960(x12)\n"
			"flw.ps f8, 512(x12)\n"
			"flw.ps f14, 608(x12)\n"
			"fnmadd.s f31, f11, f8, f14, rmm\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f1, 672(x13)\n"
			"flw.ps f20, 96(x13)\n"
			"flt.ps f19, f1, f20\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f11, 928(x28)\n"
			"flw.ps f25, 864(x28)\n"
			"flw.ps f2, 32(x28)\n"
			"fnmadd.ps f25, f11, f25, f2, rdn\n" 
			"li x28, 0xb6eb12e2ceecbdcd\n"
			"fcvt.s.lu f2, x28, rdn\n" 
			"la x5, rand_int_32\n"
			"flw.ps f14, 544(x5)\n"
			"flw.ps f12, 288(x5)\n"
			"fsrl.pi f12, f14, f12\n" 
			"la x13, rand_int_32\n"
			"flw.ps f16, 288(x13)\n"
			"flw.ps f18, 640(x13)\n"
			"fmaxu.pi f19, f16, f18\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f1, 480(x11)\n"
			"fround.ps f2, f1, rtz\n" 
			"la x12, rand_int_32\n"
			"flw.ps f19, 960(x12)\n"
			"flw.ps f20, 448(x12)\n"
			"fmax.pi f1, f19, f20\n" 
			"LBL_SEQID_3_M0_WRITE_ID_7_HID_15:\n"
			"maskand m7, m7, m3\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x25, rand_ieee754_32\n"
			"flw.ps f8, 192(x25)\n"
			"flw.ps f16, 576(x25)\n"
			"feq.s x17, f8, f16\n" 
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
			"la x12, rand_ieee754_32\n"
			"flw.ps f8, 480(x12)\n"
			"flw.ps f16, 256(x12)\n"
			"fmul.s f25, f8, f16, rmm\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f8, 672(x18)\n"
			"flw.ps f11, 416(x18)\n"
			"flw.ps f19, 928(x18)\n"
			"fnmadd.s f29, f8, f11, f19, rtz\n" 
			"la x11, rand_int_32\n"
			"flw.ps f21, 896(x11)\n"
			"fandi.pi f0, f21, 0x114\n" 
			"LBL_SEQID_4_M0_WRITE_ID_8_HID_15:\n"
			"maskor m4, m7, m4\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x25, rand_ieee754_32\n"
			"flw.ps f1, 96(x25)\n"
			"flw.ps f18, 160(x25)\n"
			"fsgnjn.ps f11, f1, f18\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f20, 544(x5)\n"
			"fcvt.pw.ps f18, f20, rdn\n" 
			"la x5, rand_int_32\n"
			"flw.ps f8, 704(x5)\n"
			"flw.ps f25, 160(x5)\n"
			"for.pi f25, f8, f25\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f1, 64(x13)\n"
			"flw.ps f21, 960(x13)\n"
			"fmin.s f1, f1, f21\n" 
			"LBL_SEQID_4_FP_TRANS_ID_9_HID_15:\n"
			"la x17, rand_ieee754_32\n"
			"flw.ps f21, 352(x17)\n"
			"frcp.ps f18, f21\n" 
			"maskpopcz x23, m7\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f20, 896(x5)\n"
			"flw.ps f5, 320(x5)\n"
			"fle.ps f16, f20, f5\n" 
			"LBL_SEQID_4_M0_WRITE_ID_10_HID_15:\n"
			"maskor m4, m4, m0\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x23, rand_ieee754_32\n"
			"flw.ps f31, 768(x23)\n"
			"flw.ps f16, 288(x23)\n"
			"fadd.ps f25, f31, f16, rdn\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f21, 992(x11)\n"
			"ffrc.ps f14, f21\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f29, 288(x12)\n"
			"flw.ps f20, 896(x12)\n"
			"fsub.ps f29, f29, f20, rdn\n" 
			"la x13, rand_int_32\n"
			"flw.ps f31, 960(x13)\n"
			"fnot.pi f11, f31\n" 
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
		0xed8ba059, 
		0xe09b9fb7, 
		0x99c16a90, 
		0xeac5f300, 
		0xb0bbfa93, 
		0xbaea3700, 
		0x683f4312, 
		0x1ac121d8, 
		0x65bf3be2, 
		0x04c2d0e0, 
		0x94fb4b92, 
		0x09cba440, 
		0x41d2e5a5, 
		0x5ea8ee9a, 
		0x24af8edc, 
		0x8c557093, 
		0x84c1a5c4, 
		0x104ccf71, 
		0xcd4c116e, 
		0x66465293, 
		0xbfa4c290, 
		0x43f0f4af, 
		0x99828287, 
		0x0f1c82ea, 
		0xfe75b008, 
		0x66d82734, 
		0x786af0d6, 
		0x24407ff3, 
		0x1b9bfa17, 
		0xd91edb8d, 
		0xc7fda70d, 
		0xba47f2eb, 
		0xef3440e6, 
		0xc2390484, 
		0x9d2ec041, 
		0xf72a9a90, 
		0xafa65283, 
		0xdc8e7d0c, 
		0xd0c01d53, 
		0x0d1b0b60, 
		0xd7937c7d, 
		0x6d4d62b5, 
		0x61909eb5, 
		0x90a8303e, 
		0xfd4d983f, 
		0xefe5d64d, 
		0x620b0854, 
		0xed48f902, 
		0x22480b9d, 
		0x8d906464, 
		0x934f7924, 
		0xaa9f26ae, 
		0x4c35a328, 
		0xb645f402, 
		0xdcd4f4ae, 
		0x8a4fcf33, 
		0x5fcab493, 
		0x47af47af, 
		0x12f91045, 
		0xd4303278, 
		0x9a0501a0, 
		0x3f26ded0, 
		0x1647e744, 
		0xb37141e5, 
		0x2cac955a, 
		0x3de5c8af, 
		0x2cdefd3e, 
		0x383088d0, 
		0x614c0e36, 
		0x312fb745, 
		0xbac979e8, 
		0x611a1197, 
		0x8364c109, 
		0x60ef3ebc, 
		0xa26c6830, 
		0x8ed11722, 
		0x17587792, 
		0xf355ccb7, 
		0xb8e2246d, 
		0x30dddf89, 
		0x2a09db47, 
		0xf3a96c38, 
		0x1b3fe716, 
		0xb1203d2f, 
		0x1d5d48a9, 
		0x37f963ba, 
		0x4076904f, 
		0x794a11b8, 
		0x50ab9826, 
		0x7e262660, 
		0xb8e12f5e, 
		0xbb12e603, 
		0xedf6285e, 
		0xda728c02, 
		0xfb0d3b17, 
		0x1c3e0ba5, 
		0xbab9f017, 
		0x249eeaad, 
		0xea02e657, 
		0x9c4e4f31, 
		0x34427597, 
		0xd58b4827, 
		0xb37f604a, 
		0xb3ea1c02, 
		0x96189e3a, 
		0x02f9ea7e, 
		0xe15d7883, 
		0xf55f24eb, 
		0xe6059eb2, 
		0xea72a997, 
		0x112c701f, 
		0xd8b58d81, 
		0xaa5457e5, 
		0x82c0264a, 
		0xd0a69230, 
		0x6a0eb438, 
		0xa8fa67dd, 
		0xb9a22cc2, 
		0xd5bc00d4, 
		0x1495139b, 
		0x6fb78839, 
		0x1461d102, 
		0xe4cadc5c, 
		0xc10861b5, 
		0x478f76c1, 
		0xbbbd9f68, 
		0xbf744640, 
		0xc6f884aa, 
		0x778d39ad, 
		0xaeb2a290, 
		0x975a0d26, 
		0x89cd2625, 
		0xee5d8ed6, 
		0xa6f27964, 
		0x0f0cd686, 
		0xee257eee, 
		0x61f23a0b, 
		0x1eee1113, 
		0x861809bf, 
		0x6aa306c4, 
		0xdd533192, 
		0xed1f0242, 
		0x453eee0c, 
		0x00129ac6, 
		0x551caac3, 
		0x871aad47, 
		0xba2ce97e, 
		0xaabaffce, 
		0xcf7a4d50, 
		0x89f1f029, 
		0xa11f089c, 
		0x9295c9c1, 
		0x33c48f2f, 
		0xd1938f84, 
		0x632a9ceb, 
		0x7c185ea5, 
		0xacdfce8a, 
		0xacbd373f, 
		0x3ede666f, 
		0x6fca46f0, 
		0xa51cf7e3, 
		0xeec00c20, 
		0x6fd13fa0, 
		0xdeb25faf, 
		0x2962a3f5, 
		0x544443b6, 
		0xd1ee6828, 
		0x3a87afc8, 
		0xd814869b, 
		0x3ee4597a, 
		0xc9fbf170, 
		0x72b53886, 
		0x2a61b258, 
		0x24812de4, 
		0xd3ada4fa, 
		0x67f4a772, 
		0xbfb25c07, 
		0x3841f3f1, 
		0x643e57f6, 
		0xdf977b0e, 
		0xfd652c4c, 
		0xd561dae8, 
		0x59690603, 
		0xca174d58, 
		0xfe685794, 
		0xb45cdc5a, 
		0xb6bb3d84, 
		0xe1c58d73, 
		0x40d3e97a, 
		0x95a13feb, 
		0x34470052, 
		0x2c2fcae8, 
		0x0e783ad8, 
		0xe69da375, 
		0x8ab6a40c, 
		0x67713f6d, 
		0xfb39fccf, 
		0x45a7bc00, 
		0xdc3b2edc, 
		0x6e41c43a, 
		0x93f3df60, 
		0x6623ac99, 
		0xea7c889e, 
		0xaf0b4ee9, 
		0xf2bf2ebb, 
		0x93a2f99a, 
		0xa42ad2c2, 
		0x593e6089, 
		0x16dfaab4, 
		0x6f0e0389, 
		0x987f3ad5, 
		0x6669425a, 
		0x0e6b5d54, 
		0x693175f7, 
		0x98853b94, 
		0x30923a5e, 
		0x9b054361, 
		0xd4715328, 
		0xf03cdcdf, 
		0x834cfe0f, 
		0xf5b26cd9, 
		0x1856efd1, 
		0x572b6863, 
		0x4a86dcb7, 
		0xeb3f4f5b, 
		0x01091068, 
		0x6c71e789, 
		0x0b1ba147, 
		0xc8432074, 
		0xc512d8d4, 
		0x46b4a75e, 
		0x774e5512, 
		0x21bbb8ec, 
		0x95bbc0fa, 
		0x397ed06e, 
		0x85e2b07c, 
		0xcb3e459c, 
		0x988ca101, 
		0x8c0bb708, 
		0x851a5b19, 
		0x3e039457, 
		0x6aee57c2, 
		0x372fc74a, 
		0x6a656633, 
		0xe77df835, 
		0x89049494, 
		0xac50265e, 
		0x18fc51b7, 
		0xea12a881, 
		0x850a4c48, 
		0xeed51276, 
		0xfd8374d3, 
		0x69c12288, 
		0x996e3145, 
		0xf44581c7, 
		0x4161b2d8  
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
		0x5300, 
		0xcd80, 
		0x5140, 
		0xd340, 
		0xa624, 
		0xce40, 
		0x4d00, 
		0xd32a, 
		0x8b8a, 
		0x4f00, 
		0x5307, 
		0xca80, 
		0xb642, 
		0x3b30, 
		0x61d7, 
		0x4c00, 
		0xc006, 
		0x4d40, 
		0x0000, 
		0xcec0, 
		0x92b1, 
		0xcd40, 
		0xc900, 
		0xd35d, 
		0x5807, 
		0xd060, 
		0x5080, 
		0x0000, 
		0xd1c0, 
		0x2ea8, 
		0xef56, 
		0xb751, 
		0x4e00, 
		0x7fd0, 
		0x4200, 
		0x7c00, 
		0xfcb0, 
		0xcc40, 
		0x23a7, 
		0xc938, 
		0x4b80, 
		0x4e00, 
		0xd020, 
		0x8000, 
		0xc880, 
		0x1327, 
		0xce80, 
		0xcd80, 
		0x53e0, 
		0x0000, 
		0xcc00, 
		0x4fc0, 
		0x53a0, 
		0xf210, 
		0x50c0, 
		0x0d47, 
		0xd000, 
		0x4400, 
		0x0000, 
		0xd0a0, 
		0x8000, 
		0x39a3, 
		0x5320, 
		0xc900, 
		0xca80, 
		0x8000, 
		0x1b37, 
		0x4d04, 
		0x7c00, 
		0xd340, 
		0x4ec0, 
		0x53e0, 
		0x4c40, 
		0x5220, 
		0xd3e0, 
		0x5300, 
		0xd300, 
		0xc500, 
		0xc980, 
		0x0000, 
		0x0c77, 
		0x4d80, 
		0x0be1, 
		0x5200, 
		0xd0a0, 
		0xd3a0, 
		0xcf00, 
		0x4400, 
		0x0000, 
		0xd320, 
		0xd0e0, 
		0xad08, 
		0x52a0, 
		0x51a0, 
		0x52a0, 
		0xd3a0, 
		0x5200, 
		0xe231, 
		0x2837, 
		0x50e0, 
		0x2f0b, 
		0x52a0, 
		0x8000, 
		0x4a00, 
		0x5060, 
		0x0c40, 
		0x119a, 
		0xcb00, 
		0xc500, 
		0x4dc0, 
		0x4600, 
		0x5c99, 
		0x5300, 
		0xd0a0, 
		0xc200, 
		0xd020, 
		0xc506, 
		0xc5b3, 
		0xcf40, 
		0x0000, 
		0x5240, 
		0x4d40, 
		0x32e7, 
		0xd340, 
		0xcf00, 
		0x5100, 
		0x4900, 
		0xd1e0, 
		0xd340, 
		0x771a, 
		0x210a, 
		0xc7b3, 
		0xfaac, 
		0xd292, 
		0xd1e0, 
		0x5040, 
		0x4e40, 
		0xce80, 
		0x50a0, 
		0x53a0, 
		0x90ed, 
		0xc000, 
		0x50c0, 
		0xcd40, 
		0x0000, 
		0xfc00, 
		0xbe24, 
		0xd040, 
		0xd0e0, 
		0x1c0f, 
		0x4e60, 
		0x51a0, 
		0xcc00, 
		0xd160, 
		0x22da, 
		0xccc0, 
		0x7c5e, 
		0x41b2, 
		0xd3a0, 
		0x5060, 
		0xb21a, 
		0x51a0, 
		0xca80, 
		0xd3e0, 
		0xfc00, 
		0x95f2, 
		0x4c80, 
		0x53e0, 
		0x7d43, 
		0x53a0, 
		0x5360, 
		0xc200, 
		0x4a00, 
		0x5240, 
		0x53c0, 
		0xc500, 
		0x3c71, 
		0x653e, 
		0xd0c0, 
		0xc000, 
		0x6c5a, 
		0x0000, 
		0xbfbb, 
		0xc8aa, 
		0xf685, 
		0xc600, 
		0xd0c0, 
		0xd0e0, 
		0xc5ad, 
		0xcc40, 
		0x0124, 
		0x0215, 
		0x81fe, 
		0x0822, 
		0x4d40, 
		0x8000, 
		0x4200, 
		0xc638, 
		0xd16d, 
		0xd1c0, 
		0x5120, 
		0x50c0, 
		0x4700, 
		0x4b00, 
		0x4f40, 
		0xfc00, 
		0xcdc0, 
		0xd280, 
		0x5140, 
		0xd020, 
		0x4f40, 
		0x0000, 
		0x4400, 
		0x9b16, 
		0x4f80, 
		0x4f80, 
		0x4e80, 
		0x7c00, 
		0x5220, 
		0xcc00, 
		0xd060, 
		0xd180, 
		0x51c0, 
		0x5340, 
		0x4b00, 
		0x4b80, 
		0x3c00, 
		0x5320, 
		0x5180, 
		0x5000, 
		0x7cb4, 
		0x52c0, 
		0x5080, 
		0xce80, 
		0xd3a0, 
		0xd080, 
		0x4800, 
		0xceba, 
		0x8000, 
		0xe569, 
		0x5f8e, 
		0x5240, 
		0xcc00, 
		0xc700, 
		0x5280, 
		0xc500, 
		0x51a0, 
		0x5280, 
		0x4cc0, 
		0x4cc0, 
		0xee97, 
		0xccc0, 
		0x5180, 
		0xcf40, 
		0xd080, 
		0xea11, 
		0x4880, 
		0x8000, 
		0x5280, 
		0x656d, 
		0xd1c0, 
		0x7c00, 
		0xd1a0, 
		0xcb80, 
		0xd280, 
		0xd3c0, 
		0xd0c0, 
		0x8000, 
		0xd300, 
		0x4500, 
		0xd260, 
		0x51a0, 
		0x4f80, 
		0xd3c0, 
		0x4c00, 
		0x8000, 
		0xd0c0, 
		0xd1a0, 
		0x5180, 
		0x4980, 
		0x5180, 
		0x4600, 
		0xd100, 
		0xfc00, 
		0x942c, 
		0x1f4d, 
		0x5180, 
		0x5320, 
		0x5380, 
		0xd360, 
		0x9cfd, 
		0xcd00, 
		0x4f40, 
		0x4d80, 
		0xcf80, 
		0x39aa, 
		0x9e3e, 
		0x5260, 
		0xcc80, 
		0x5160, 
		0x52e0, 
		0x026d, 
		0x5140, 
		0x5160, 
		0x4e80, 
		0x5517, 
		0xcd00, 
		0xd0c0, 
		0xbc00, 
		0x9faa, 
		0xd200, 
		0x0000, 
		0x50a0, 
		0xcfda, 
		0x4a80, 
		0xcf40, 
		0xd320, 
		0x8993, 
		0xa9a9, 
		0x5100, 
		0x7d63, 
		0xc880, 
		0x7c00, 
		0xc500, 
		0x4c00, 
		0x4b00, 
		0x5080, 
		0x0f45, 
		0x4b80, 
		0xce00, 
		0xfa1c, 
		0x5000, 
		0x5060, 
		0xd340, 
		0x4a00, 
		0x5300, 
		0xd140, 
		0xd140, 
		0x4d80, 
		0xbdca, 
		0xd120, 
		0x4d40, 
		0x5340, 
		0x4f00, 
		0x4fc0, 
		0x4c80, 
		0x8000, 
		0x4500, 
		0xc904, 
		0x448c, 
		0x1bee, 
		0x7c00, 
		0xcf00, 
		0x4a00, 
		0x8000, 
		0x5140, 
		0x8c44, 
		0xc2ca, 
		0x4fc0, 
		0xc900, 
		0xd2c0, 
		0xce00, 
		0xd0e0, 
		0xcc00, 
		0x0000, 
		0x51a0, 
		0x4c80, 
		0xc13d, 
		0xd260, 
		0xcf00, 
		0x6bb1, 
		0x7c94, 
		0x5340, 
		0xca00, 
		0x4d00, 
		0x50c0, 
		0xd885, 
		0xe28b, 
		0x0000, 
		0x52a0, 
		0x0000, 
		0xd260, 
		0x3a42, 
		0x8000, 
		0x4a80, 
		0xcdc0, 
		0x51a0, 
		0xcc00, 
		0x5300, 
		0xd2e0, 
		0xd3a0, 
		0x4e40, 
		0xd3e0, 
		0x4600, 
		0x5662, 
		0x0000, 
		0xce80, 
		0x5340, 
		0x8000, 
		0x5220, 
		0x20e9, 
		0x5020, 
		0x5200, 
		0xce40, 
		0x52e0, 
		0xed66, 
		0x5000, 
		0x0000, 
		0x53a0, 
		0x4800, 
		0xd1e0, 
		0x41c2, 
		0x26d8, 
		0x5060, 
		0x86e3, 
		0x4800, 
		0xcc40, 
		0x98db, 
		0xd120, 
		0xc800, 
		0xcfc0, 
		0x7cb9, 
		0x1827, 
		0x1395, 
		0xd3a0, 
		0xa91f, 
		0xc880, 
		0x4800, 
		0x5220, 
		0x5120, 
		0xdf13, 
		0x72f2, 
		0xd340, 
		0xcd80, 
		0xd000, 
		0x4b00, 
		0x5694, 
		0x6df3, 
		0x4f40, 
		0x4f80, 
		0xd140, 
		0xeac9, 
		0xdd25, 
		0x8000, 
		0x5100, 
		0xcc80, 
		0xd320, 
		0x7c00, 
		0xd300, 
		0x50c0, 
		0x4a00, 
		0xd140, 
		0x52e0, 
		0x0000, 
		0xbc00, 
		0x8000, 
		0x4e00, 
		0x5060, 
		0x0260, 
		0xd1e0, 
		0xce80, 
		0x5100, 
		0x4fc0, 
		0x4b00, 
		0x34c6, 
		0xfc00, 
		0x8000, 
		0x4c80, 
		0xcf80, 
		0x4f80, 
		0xd140, 
		0x0357, 
		0x2894, 
		0x4980, 
		0xcb80, 
		0xd340, 
		0x4e80, 
		0x4700, 
		0x076e, 
		0x4200, 
		0xcf80, 
		0xd180, 
		0x4dc0, 
		0x8000, 
		0x5140, 
		0x8000, 
		0xd380, 
		0xd100, 
		0x1469, 
		0x831d, 
		0x4700, 
		0x4e40, 
		0xfc00, 
		0xd1c0, 
		0xc800, 
		0xfc00, 
		0x8121, 
		0xb1a7, 
		0x8000, 
		0xd200, 
		0x4e00, 
		0x2daa, 
		0xf033, 
		0x4f00, 
		0xc500, 
		0x8af9, 
		0xd100, 
		0xcd80, 
		0x50e0, 
		0xd180, 
		0xfc00, 
		0x4c40, 
		0x0000, 
		0xb017, 
		0x4700, 
		0xcdc0, 
		0xc600, 
		0x4880, 
		0xd140, 
		0x7d90, 
		0x7961, 
		0xcc00, 
		0x4f80, 
		0x716c, 
		0xd320, 
		0x4d00, 
		0xd080, 
		0x198a, 
		0x4f00, 
		0xcd40, 
		0x4b00, 
		0x0000, 
		0xfc00, 
		0x5e63, 
		0x5320, 
		0x4f40, 
		0x0000, 
		0x5100, 
		0xe09c, 
		0xde25, 
		0x4e00, 
		0x8000, 
		0x50c0, 
		0xf652, 
		0x0fc4, 
		0x4fc0, 
		0x5140, 
		0x50a0, 
		0xa315, 
		0xc200, 
		0x8000, 
		0x5120, 
		0xecf1, 
		0xcfc0, 
		0xd1e0, 
		0x4c40, 
		0x52e0, 
		0xb623, 
		0xd160, 
		0xd2a0, 
		0x4a00, 
		0x5380, 
		0xd200, 
		0x4e80, 
		0xea57, 
		0xd260, 
		0xfc00, 
		0xfecc, 
		0x52c0, 
		0x51c0, 
		0xce40, 
		0xd3e0, 
		0xf48b, 
		0xcb00, 
		0xd240, 
		0x260f, 
		0x5140, 
		0x50c0, 
		0x0b12, 
		0x4d00, 
		0x8000, 
		0xc880, 
		0xfc00, 
		0x8cd2, 
		0xd0c0, 
		0xccc0, 
		0xdcdb, 
		0xc000, 
		0x50a0, 
		0x8000, 
		0xbb77, 
		0x4fc0, 
		0xc000, 
		0xc000, 
		0x8000, 
		0x6a0b, 
		0x05f1, 
		0x5200, 
		0x5060, 
		0xd2e0, 
		0xcb26, 
		0xcc00, 
		0x5040, 
		0xc600, 
		0xca80, 
		0x7964, 
		0x84fa, 
		0xd2c0, 
		0x4e40, 
		0x53a0, 
		0x4c00, 
		0x4f40, 
		0x5000, 
		0xcf00, 
		0xcc80, 
		0x4000, 
		0xd180, 
		0x51e0, 
		0xd020, 
		0x4d80, 
		0xcf80, 
		0xd220, 
		0xd3a0, 
		0x833f, 
		0x53bd, 
		0xca80, 
		0x3c00, 
		0x54af, 
		0xd180, 
		0x5260, 
		0x5080, 
		0xd360, 
		0xcec0, 
		0x8000, 
		0x4ec0, 
		0xfc52, 
		0x5183, 
		0x4c80, 
		0x5200, 
		0x5040, 
		0xfc76, 
		0x4ec0, 
		0xc900, 
		0x4f00, 
		0xce00, 
		0xd7e8, 
		0xe4c6, 
		0xd871, 
		0x8000, 
		0x0000, 
		0x053b, 
		0xd47a, 
		0x4e00, 
		0x8000, 
		0xd060, 
		0xd280, 
		0x4a00, 
		0xc980, 
		0x5380, 
		0x50e0, 
		0xcc00, 
		0x50e0, 
		0x4800, 
		0x5300, 
		0x0fc8, 
		0x4a80, 
		0x0b7a, 
		0x5120, 
		0xcd00, 
		0x5200, 
		0x1f8e, 
		0x8000, 
		0xf527, 
		0x4f40, 
		0x9a66, 
		0x5200, 
		0x8000, 
		0x4500, 
		0x7d3c, 
		0x4600, 
		0x4f00, 
		0xbf0d, 
		0xfc00, 
		0x4c40, 
		0x9aa4, 
		0x5140, 
		0x4700, 
		0x4f40, 
		0xb906, 
		0xc800, 
		0xce80, 
		0x8e23, 
		0xc1e4, 
		0xd1e0, 
		0xa5b6, 
		0xc000, 
		0x5180, 
		0x8000, 
		0xf62a, 
		0xcf40, 
		0xcf80, 
		0x5220, 
		0x8000, 
		0x4b00, 
		0x50c0, 
		0x4d00, 
		0x7c00, 
		0xbc00, 
		0x4600, 
		0x9087, 
		0x976c, 
		0x2252, 
		0x8000, 
		0x0000, 
		0x8000, 
		0x4c00, 
		0xfc00, 
		0xfcc6, 
		0xb427, 
		0x4d40, 
		0x4400, 
		0x1bfb, 
		0xce54, 
		0x4e8d, 
		0x4d40, 
		0x267f, 
		0x2f8d, 
		0xbc00, 
		0xd3a0, 
		0x4200, 
		0xd0a0, 
		0x0cdf, 
		0x4817, 
		0xfc00, 
		0x5380, 
		0xd2a0, 
		0x4880, 
		0xc800, 
		0x5200, 
		0x4c80, 
		0xd120, 
		0x3d79, 
		0x5ca7, 
		0xc200, 
		0xca80, 
		0x5080, 
		0xa105, 
		0x52e0, 
		0x4880, 
		0x4a80, 
		0x4000, 
		0x5120, 
		0xfbdf, 
		0x50c0, 
		0x4600, 
		0xcdc0, 
		0x8000, 
		0x53a0, 
		0x5320, 
		0xd764, 
		0x72f6, 
		0xfc00, 
		0xd120, 
		0xf02b, 
		0x5140, 
		0xcf40, 
		0xcb80, 
		0x4600, 
		0xd040, 
		0x46be, 
		0x2058, 
		0x882c, 
		0xc400, 
		0x5240, 
		0xd080, 
		0x67f5, 
		0xd120, 
		0x5000, 
		0xaf3a, 
		0xcaf4, 
		0x680f, 
		0x8d84, 
		0xc500, 
		0xda8a, 
		0x03a1, 
		0x8601, 
		0x7586, 
		0xef38, 
		0x7c00, 
		0xd040, 
		0x3c00, 
		0x57c9, 
		0xcc80, 
		0xbc00, 
		0x5320, 
		0x5000, 
		0x5140, 
		0xd2a0, 
		0xa7d1, 
		0xd0c0, 
		0xd320, 
		0x1c97, 
		0xec9c, 
		0xd4a4, 
		0xcfc0, 
		0x8000, 
		0x4700, 
		0xd300, 
		0x4ab4, 
		0x5380, 
		0x4ec0, 
		0x4e40, 
		0x0000, 
		0x0000, 
		0x1592, 
		0xcd40, 
		0xd2e0, 
		0xca80, 
		0x52e0, 
		0x4c40, 
		0x4c00, 
		0xd380, 
		0x4f00, 
		0x5080, 
		0x51e0, 
		0x5260, 
		0x5320, 
		0xd6c6, 
		0x4d00, 
		0xc400, 
		0xcdc0, 
		0x7c4d, 
		0xd042, 
		0xd100, 
		0x5380, 
		0x9a6d, 
		0xe1df, 
		0x40b6, 
		0x5140, 
		0x4700, 
		0x9d94, 
		0xd000, 
		0xe905, 
		0x8971, 
		0x6f3c, 
		0x8000, 
		0xd240, 
		0xd000, 
		0xd260, 
		0x4000, 
		0xc000, 
		0xdcd1, 
		0xfd51, 
		0x4a80, 
		0x0000, 
		0x7c00, 
		0x73db, 
		0x4f80, 
		0xcb00, 
		0xfcd1, 
		0x4700, 
		0x4b00, 
		0xd2a0, 
		0x4600, 
		0xd380, 
		0x51c0, 
		0x13ac, 
		0xc800, 
		0x11d9, 
		0xcb00, 
		0x0c47, 
		0x4d81, 
		0x7cbe, 
		0x4880, 
		0x8e1b, 
		0x1b2c, 
		0xca80, 
		0xcf00, 
		0x7c00, 
		0x8000, 
		0x4fc0, 
		0x4f80, 
		0xd1a0, 
		0x32ff, 
		0xd100, 
		0x1435, 
		0xc400, 
		0x53c0, 
		0x5260, 
		0x4a80, 
		0x8000, 
		0x52c0, 
		0x4f80, 
		0xce80, 
		0xc880, 
		0x8000, 
		0x4000, 
		0xba78, 
		0xd260, 
		0x5280, 
		0xc200, 
		0x8000, 
		0xa172, 
		0x6801, 
		0xd040, 
		0x5000, 
		0xad54, 
		0x4e40, 
		0x5140, 
		0x5100, 
		0xf8a9, 
		0x5380, 
		0x51a0, 
		0x3cf6, 
		0x9061, 
		0x2af9, 
		0xcd40, 
		0xd020, 
		0xc600, 
		0x5180, 
		0x5240, 
		0x0000, 
		0x4700, 
		0x5300, 
		0xd3a0, 
		0x0000, 
		0xd2e0, 
		0x7c00, 
		0xd200, 
		0x3dc3, 
		0x8000, 
		0xc000, 
		0xd2e0, 
		0x3c00, 
		0x4b00, 
		0x50c0, 
		0x5380, 
		0xd380, 
		0xe68e, 
		0x1152, 
		0xd000, 
		0x4d00, 
		0xd120, 
		0xd2c0, 
		0xd100, 
		0x4f40, 
		0x422b, 
		0xd040, 
		0xcdc0, 
		0x5300, 
		0x803c, 
		0x5100, 
		0x49ae, 
		0x7dd1, 
		0x52c0, 
		0x4f80, 
		0x53e0, 
		0xf12b, 
		0x4e80, 
		0x51e0, 
		0xd260, 
		0x4b80, 
		0x5d5f, 
		0x4b80, 
		0x9662, 
		0x53e0, 
		0x0000, 
		0xa69a, 
		0xfc00, 
		0x4800, 
		0x4a80, 
		0x5200, 
		0xcc00, 
		0x71d6, 
		0x4d40, 
		0x8000, 
		0xc200, 
		0x7670, 
		0xc500, 
		0x7f45, 
		0x5000, 
		0x5360, 
		0x4900, 
		0x8f44, 
		0x3b53, 
		0xc900, 
		0xd2e0, 
		0x5adf, 
		0x0000, 
		0x4900, 
		0xd2a0, 
		0xd120, 
		0x52e0, 
		0xd160, 
		0x8000, 
		0xd5f8, 
		0xd200, 
		0x5220, 
		0x343a, 
		0xfd77, 
		0x6cb5, 
		0x50e0, 
		0x4c40, 
		0xcdc0, 
		0x4f00, 
		0x2d9c, 
		0xaf7b, 
		0xc700, 
		0x7de4, 
		0x7b7e, 
		0x5260, 
		0x5160, 
		0x8af7, 
		0x52c0, 
		0xd340, 
		0x52c0, 
		0x51a0, 
		0x5200, 
		0x91c2, 
		0x53c0, 
		0xd0a0, 
		0x2637, 
		0xd360, 
		0x52a0, 
		0xcec0, 
		0x0000, 
		0xd060, 
		0x4c80, 
		0x7c00, 
		0x1723, 
		0x5360, 
		0x6f1d, 
		0xcf80, 
		0xce40, 
		0x50e0, 
		0x51e0, 
		0x0000, 
		0xfc00, 
		0x5040, 
		0x51e0, 
		0x4000, 
		0x1b22, 
		0x4dc0, 
		0x4a00, 
		0xc900, 
		0xd300, 
		0x4200, 
		0xe41c, 
		0xaf60, 
		0x7c00, 
		0x5200, 
		0x4d40, 
		0xd160, 
		0x8000, 
		0xf72d, 
		0xce80, 
		0xf846, 
		0xcdc0, 
		0xbc18, 
		0x5080, 
		0x50e0, 
		0x4980, 
		0x4a80, 
		0x51e0, 
		0x5100, 
		0x5040, 
		0xcd00, 
		0x5360, 
		0x8000, 
		0xd2e0, 
		0x0000, 
		0x52c0, 
		0x7845, 
		0x1234, 
		0x1e27, 
		0x4cc0, 
		0x10ee, 
		0xd140, 
		0xd300, 
		0xfc00, 
		0x4c80, 
		0x2796, 
		0x5260, 
		0xd0a0, 
		0xd220, 
		0xafbc, 
		0x5380, 
		0x4e00, 
		0xb321, 
		0x53c0, 
		0xccc0, 
		0xfc00, 
		0x52c0, 
		0xd95e, 
		0xc980, 
		0x043d, 
		0x4200, 
		0x4d00, 
		0x7c00, 
		0xffb2, 
		0x5180, 
		0x4fc0, 
		0x4200, 
		0x94f8, 
		0x4c00, 
		0xfc00, 
		0xd160, 
		0xffea, 
		0xa4d5, 
		0x50df, 
		0x4500, 
		0x4600, 
		0x4c40, 
		0x7c68, 
		0x5200, 
		0x9fc9, 
		0x9b99, 
		0x5112, 
		0x4817, 
		0x5a78, 
		0xd2c0, 
		0xd160, 
		0x4980, 
		0xd200, 
		0xd1e0, 
		0x4dc0, 
		0xfc00, 
		0x5360, 
		0x4cc0, 
		0x8000, 
		0xc05d, 
		0x8000, 
		0x3d67, 
		0x21d5, 
		0x4dc0, 
		0x27a1, 
		0xc880, 
		0x4600, 
		0x5260, 
		0xca00, 
		0xfc00, 
		0xcf00, 
		0x5280, 
		0x51e0, 
		0x4e00, 
		0xfd27, 
		0xd0a0, 
		0x4c00, 
		0x9317, 
		0x6711, 
		0xf55b, 
		0x4f00, 
		0x17bc, 
		0xd0c0, 
		0x9fa6, 
		0xc500, 
		0x4ec0, 
		0x868c, 
		0xca00, 
		0xc600, 
		0xf036, 
		0x4880, 
		0x8000, 
		0xd180, 
		0x4500, 
		0xcd40, 
		0xf6a6, 
		0x37e1, 
		0xcf40, 
		0x4700, 
		0x5100, 
		0xd3c0, 
		0x51c0, 
		0xfc00, 
		0x8a79, 
		0x99cc, 
		0xd3a0, 
		0x1d6c, 
		0x4f80, 
		0xce80, 
		0xa997, 
		0x4000, 
		0x8000, 
		0xd320, 
		0xccc0, 
		0x4e40, 
		0x3749, 
		0x7d55, 
		0x53a0, 
		0xd2c0, 
		0x5080, 
		0x52e0, 
		0x02b7, 
		0x5140, 
		0x8000, 
		0x4c80, 
		0x4f00, 
		0xaaaa, 
		0x779d, 
		0xabd1, 
		0xcc80, 
		0xbc13, 
		0xc200, 
		0x253e, 
		0x4700, 
		0x7c00, 
		0x4e40, 
		0x5140, 
		0x5160, 
		0xc200, 
		0x4c40, 
		0x50a0, 
		0x0000, 
		0x1fa4, 
		0x7121, 
		0xc600, 
		0xa146, 
		0xb651, 
		0x53a0, 
		0xc880, 
		0x5741, 
		0x4d40, 
		0xc895, 
		0xfa2e, 
		0xbcb8, 
		0xc600, 
		0x2db3, 
		0xe23a, 
		0xa33a, 
		0x8000, 
		0x72ea, 
		0x50c0, 
		0xd1e0, 
		0xcec0, 
		0x52c0, 
		0xbc00, 
		0x4a00, 
		0x5bca, 
		0x5140, 
		0x05b5, 
		0xab9f, 
		0x0000, 
		0xc500, 
		0x4ec0, 
		0xd240, 
		0xfee2, 
		0x4400, 
		0xfc00, 
		0x4900, 
		0xb273, 
		0x8000, 
		0xec99, 
		0xcfc0, 
		0x4d00, 
		0x0614, 
		0x4980, 
		0xfbfb, 
		0xe7e1, 
		0x5360, 
		0x53a0, 
		0xd0a0, 
		0x4e00, 
		0xd140, 
		0xd200, 
		0xcec0, 
		0x495a, 
		0x639d, 
		0xcc00, 
		0x8000, 
		0x50c0, 
		0xd100, 
		0xc700, 
		0xfc00, 
		0xd200, 
		0xc200, 
		0x8000, 
		0xce80, 
		0x40cc, 
		0xfc00, 
		0xd280, 
		0x4400, 
		0x4500, 
		0x8000, 
		0x4e00, 
		0x7c00, 
		0xd1c0, 
		0x0aac, 
		0x4ec0, 
		0xd300, 
		0x67a4, 
		0x50a0, 
		0xd0e0, 
		0x20bd, 
		0x4500, 
		0xccc0, 
		0xcf00, 
		0xbc00, 
		0xac51, 
		0xcc00, 
		0x4400, 
		0x4c00, 
		0x8a46, 
		0x53a0, 
		0x709c, 
		0xd1e0, 
		0xd1c0, 
		0x7c00, 
		0xd524, 
		0x22a9, 
		0xcebd, 
		0xd260, 
		0xce80, 
		0xcfc0, 
		0xc700, 
		0x5020, 
		0xcf00, 
		0x4c00, 
		0x7c00, 
		0x9016, 
		0xfc00, 
		0x4000, 
		0xd0e0, 
		0x50c0, 
		0xd2e0, 
		0xecca, 
		0xfdee, 
		0xcb00, 
		0x8000, 
		0x57a6, 
		0x9858, 
		0x4500, 
		0x4e00, 
		0x4ec0, 
		0xb9ef, 
		0x5040, 
		0xd220, 
		0x52a0, 
		0xf818, 
		0xfbed, 
		0x4fc0, 
		0x5240, 
		0x3c00, 
		0xc900, 
		0x504e, 
		0xfcb3, 
		0x4e40, 
		0xa58a, 
		0xd340, 
		0xd040, 
		0x5000, 
		0x7c00, 
		0x53e0, 
		0x4d00, 
		0x2920, 
		0xc500, 
		0xce80, 
		0x0000, 
		0xd1a0, 
		0x4500, 
		0xa0f7, 
		0x827f, 
		0x4253, 
		0x8000, 
		0xcace, 
		0x4d00, 
		0x9cb4, 
		0x4880, 
		0xd180, 
		0x4cc0, 
		0xca00, 
		0x4c00, 
		0x396d, 
		0x203e, 
		0xcec0, 
		0xcd80, 
		0x350c, 
		0x4700, 
		0x91ac, 
		0xc200, 
		0x4b80, 
		0xc23c, 
		0xfd25, 
		0xcec0, 
		0x8000, 
		0xd120, 
		0x9793, 
		0xd2c0, 
		0xcc00, 
		0xcf00, 
		0x5260, 
		0xd1a0, 
		0x51c0, 
		0x0000, 
		0xd120, 
		0xcd80, 
		0xa373, 
		0xd220, 
		0xd340, 
		0x4f80, 
		0x5200, 
		0x7c00, 
		0x5360, 
		0xce00, 
		0x53a0, 
		0xcc00, 
		0x53c0, 
		0xc200, 
		0xd1e0, 
		0xfc00, 
		0xd080, 
		0x5060, 
		0x0000, 
		0x5260, 
		0xd3a0, 
		0xcdc0, 
		0x4800, 
		0x2d78, 
		0xc200, 
		0x1f83, 
		0xc200, 
		0x41c7, 
		0xc000, 
		0x839e, 
		0x7428, 
		0x5220, 
		0x4c40, 
		0x8000, 
		0x53e0, 
		0x7c00, 
		0xf2f8, 
		0x4c80, 
		0x4980, 
		0xd3e0, 
		0xd040, 
		0xc77a, 
		0x4e40, 
		0xa68d, 
		0x51c0, 
		0xd3c0, 
		0x50e0, 
		0x6a3f, 
		0xd35c, 
		0xca80, 
		0xcf40, 
		0xc980, 
		0xa462, 
		0x4400, 
		0xf5e9, 
		0x45b1, 
		0x5020, 
		0xcf00, 
		0x8000, 
		0xca00, 
		0xd020, 
		0xd320, 
		0x5020, 
		0x4ae5, 
		0x5000, 
		0x050a, 
		0xcd80, 
		0x8d0c, 
		0xc200, 
		0xce80, 
		0x4700, 
		0x784a, 
		0xcf80, 
		0x4cc0, 
		0xd160, 
		0x85ca, 
		0xcc40, 
		0x9866, 
		0x6b94, 
		0x13d2, 
		0x4b00, 
		0x1707, 
		0xfc00, 
		0x7e10, 
		0xf95d, 
		0xd29d, 
		0x4800, 
		0x52c0, 
		0x6dde, 
		0xcf00, 
		0xca80, 
		0xb275, 
		0xce80, 
		0xce00, 
		0xca00, 
		0xc500, 
		0x283f, 
		0xd100, 
		0xd080, 
		0x4d40, 
		0xfd44, 
		0x4800, 
		0xc880, 
		0xcf80, 
		0xd060, 
		0x4ec0, 
		0xd060, 
		0x4ede, 
		0xd5d5, 
		0x52a0, 
		0xe213, 
		0x6447, 
		0x5140, 
		0x4c80, 
		0xd000, 
		0x0000, 
		0x1d99, 
		0xcd40, 
		0x4f40, 
		0x25b8, 
		0xd320, 
		0x50a0, 
		0x51a0, 
		0xcf00, 
		0x2bd6, 
		0x5260, 
		0x1a69, 
		0x4fc0, 
		0x52a0, 
		0x4e40, 
		0xcec0, 
		0xcf80, 
		0x51da, 
		0x5532, 
		0x4d80, 
		0x0000, 
		0x5000, 
		0xac9f, 
		0x1919, 
		0xcf40, 
		0xb378, 
		0x72a2, 
		0x8720  
};
volatile uint32_t rand_ieee754_32[1536] __attribute__ ((aligned (2048))) = {
		0xff800000, 
		0x00000000, 
		0x73b80cb8, 
		0xc52e8e32, 
		0x860c4594, 
		0x0c7ffffc, 
		0xfb0d1d8c, 
		0x2783eb97, 
		0x00000020, 
		0x41000000, 
		0x80000002, 
		0x80800002, 
		0x88430b41, 
		0x426c0000, 
		0x40000000, 
		0xa9b40881, 
		0x0e03ffff, 
		0xc07fc36a, 
		0xff000000, 
		0xdbba13cf, 
		0x4f0ddf4b, 
		0xc2780000, 
		0x13e80678, 
		0xff000000, 
		0x80000100, 
		0x9c71ab78, 
		0xbf028f5c, 
		0x423c0000, 
		0x00000000, 
		0xf5abe019, 
		0x41980000, 
		0x1eb29f68, 
		0x41980000, 
		0x42080000, 
		0x0c7fff00, 
		0xc22c0000, 
		0xa150bb78, 
		0xc2040000, 
		0x80800001, 
		0x427c0000, 
		0xff7ffffe, 
		0xc1100000, 
		0x80011111, 
		0x00001000, 
		0x42200000, 
		0x00000002, 
		0xefb60299, 
		0xbf800001, 
		0xc1c80000, 
		0x00000002, 
		0x134875fa, 
		0x0e00003f, 
		0x00000080, 
		0x00000008, 
		0x52fdf0af, 
		0x42140000, 
		0xc2100000, 
		0x0c7ffff0, 
		0x0c7fff00, 
		0x0c7fe000, 
		0x73e299ef, 
		0x42040000, 
		0x80000000, 
		0xf01522e1, 
		0x0e01ffff, 
		0xff800000, 
		0x0c700000, 
		0x0e00ffff, 
		0x00000100, 
		0xff800000, 
		0x00000001, 
		0x1acc3b86, 
		0x7fc8cd05, 
		0xc25c0000, 
		0xc1d00000, 
		0x7f800000, 
		0xbd0c5289, 
		0xff800000, 
		0x5934326c, 
		0xc1200000, 
		0x00000000, 
		0xcd2d1453, 
		0x0e007fff, 
		0xc27c0000, 
		0xcb5f1a98, 
		0x8f7ffffe, 
		0x00ffffff, 
		0xe7783341, 
		0xc2200000, 
		0x5b687e18, 
		0x0e03ffff, 
		0x00000020, 
		0xc23c0000, 
		0x0f7ffffe, 
		0x40800000, 
		0x0e000fff, 
		0x00010000, 
		0x80000001, 
		0xc2180000, 
		0x0e7fffff, 
		0x7edf0b1f, 
		0xc1b00000, 
		0x422c0000, 
		0x80000000, 
		0xc1a00000, 
		0x42140000, 
		0x0c7ffffc, 
		0xff7ffffe, 
		0xff834b3a, 
		0x9f89bf9d, 
		0xe9979299, 
		0xc1e00000, 
		0x80100000, 
		0xc2340000, 
		0xc2100000, 
		0x0c7ffffc, 
		0x58ffe7a1, 
		0xc2080000, 
		0x80000001, 
		0x0c780000, 
		0x80000000, 
		0x31ba8547, 
		0xbf800001, 
		0x9383cc2f, 
		0xff835141, 
		0xc2000000, 
		0x80000000, 
		0x00000020, 
		0x41600000, 
		0x80080000, 
		0xc26c0000, 
		0xc2480000, 
		0xffcbaa72, 
		0x2c159957, 
		0x00200000, 
		0x00000001, 
		0x18a77d7d, 
		0x4ded62e7, 
		0x0c7e0000, 
		0xdd6f92c2, 
		0x42340000, 
		0xfb72c5eb, 
		0x7f800000, 
		0x7f7fffff, 
		0x60439b09, 
		0x80800001, 
		0x80000000, 
		0x0c7fff00, 
		0xc4e4edf0, 
		0x94d69b33, 
		0xfa8f3da2, 
		0x003d5f52, 
		0x00000020, 
		0x41c00000, 
		0xc58129fb, 
		0xa238f81a, 
		0x00000001, 
		0x00000000, 
		0x00000000, 
		0x2ecf5a79, 
		0xc25c0000, 
		0x42380000, 
		0x80000100, 
		0x00000020, 
		0xff9869af, 
		0xc2240000, 
		0xc0a00000, 
		0xc0000000, 
		0x80000000, 
		0xc9d41e00, 
		0x42480000, 
		0x42400000, 
		0x7fffffff, 
		0x41a00000, 
		0x00000000, 
		0x42140000, 
		0x80000000, 
		0x0010ede5, 
		0x0c7fff80, 
		0x80001000, 
		0xe93c7183, 
		0xee6d1803, 
		0x00000000, 
		0xc0000000, 
		0x416ca06d, 
		0x01093078, 
		0x00000200, 
		0x00080000, 
		0xff800000, 
		0x0c400000, 
		0x80000000, 
		0x0e00007f, 
		0x80800000, 
		0xc0400000, 
		0x427c0000, 
		0x80655493, 
		0xbf800001, 
		0x1c25a413, 
		0x4b000000, 
		0x007ffffe, 
		0x42100000, 
		0x80000000, 
		0x403af14e, 
		0x8d57b12f, 
		0x41100000, 
		0xff7ffffe, 
		0x41a00000, 
		0xb8c3fe16, 
		0x41980000, 
		0x7b02d17d, 
		0x00001000, 
		0x40000000, 
		0xf44ddaa7, 
		0x93e9c81e, 
		0x01a273c3, 
		0x5db55f31, 
		0x00000008, 
		0x0c7fffc0, 
		0xff7fffff, 
		0xff800000, 
		0x9dfe0e9e, 
		0x6c1f7c1e, 
		0x0e00000f, 
		0x7f7ffffe, 
		0x4b000000, 
		0x80000000, 
		0x7fc00000, 
		0x4b8c4b40, 
		0x3eca8ea6, 
		0xcccccccc, 
		0x40000000, 
		0x425c0000, 
		0xc2700000, 
		0x41d80000, 
		0xb3759c21, 
		0xff800000, 
		0x42480000, 
		0xff000000, 
		0x00000000, 
		0xc1a80000, 
		0x714af00b, 
		0x0066d57f, 
		0x0e01ffff, 
		0xbb660b98, 
		0x425c0000, 
		0x748c3e04, 
		0x80800003, 
		0x00000000, 
		0x4a33ab13, 
		0x0c7ff800, 
		0x80000000, 
		0x41b00000, 
		0x5cd029e3, 
		0x00000000, 
		0xe360c039, 
		0xcccccccc, 
		0x80200000, 
		0x42440000, 
		0xec5d6689, 
		0xc0a00000, 
		0xb40d5f9e, 
		0x7f000000, 
		0x00004000, 
		0xff7ffffe, 
		0x41600000, 
		0x0e003fff, 
		0x007ffffe, 
		0x0c7ff800, 
		0xe4095260, 
		0xffc00000, 
		0x7f7fffff, 
		0xff7fffff, 
		0x0c7ffffc, 
		0x80000000, 
		0x47a980da, 
		0x00000000, 
		0x80000000, 
		0x042ee20c, 
		0x70c4c4f8, 
		0x55555555, 
		0x221ef181, 
		0x4b000000, 
		0x807ffffe, 
		0x0c7e0000, 
		0xa08b1374, 
		0x427c0000, 
		0x7ff19df0, 
		0x7fbfffff, 
		0x0c7e0000, 
		0x98e5dc91, 
		0xc1980000, 
		0x00000000, 
		0x0e00007f, 
		0xc1880000, 
		0xb5374325, 
		0x1ec34987, 
		0x51b98b64, 
		0x0e000003, 
		0x00000000, 
		0x00000000, 
		0x420c0000, 
		0xeb5417ee, 
		0x5ea0eb97, 
		0x80000000, 
		0x73db3fe5, 
		0x6b09155c, 
		0xc23c0000, 
		0xebf42e80, 
		0x0c7ffe00, 
		0x6f4aabde, 
		0xc1980000, 
		0xc22c0000, 
		0x42640000, 
		0xc1b80000, 
		0x0e000007, 
		0xc3c9d65e, 
		0x3bd4fa1d, 
		0xff800000, 
		0xc0c00000, 
		0xe3c976b1, 
		0xff7ffffe, 
		0xc0a00000, 
		0x00000000, 
		0x427c0000, 
		0x0c7fff00, 
		0xb726144b, 
		0x41c80000, 
		0x00800000, 
		0x41900000, 
		0xc1880000, 
		0x7f800000, 
		0x8b88373d, 
		0xf45d067e, 
		0xe7be6caa, 
		0x00000008, 
		0x80000000, 
		0x41800000, 
		0xc2000000, 
		0x80000000, 
		0x41c80000, 
		0x41600000, 
		0x41d80000, 
		0x7f7ffffe, 
		0xff800000, 
		0x00800003, 
		0x41d00000, 
		0x1209d16c, 
		0x00000400, 
		0x9be5a1b3, 
		0xc0000000, 
		0x80000400, 
		0x80000000, 
		0xffc00001, 
		0xf0bd82b8, 
		0x42300000, 
		0xc1100000, 
		0x0f7fffff, 
		0xc2140000, 
		0x00000100, 
		0xc2040000, 
		0x52ebdde0, 
		0x0e0000ff, 
		0x007fffff, 
		0x8e7bac68, 
		0xc2580000, 
		0x00000200, 
		0x8f7fffff, 
		0x0e000001, 
		0xd78e852e, 
		0x42440000, 
		0x7f89c3ec, 
		0x80000000, 
		0x12b443ec, 
		0x0e00007f, 
		0xc0000000, 
		0xc1900000, 
		0x80080000, 
		0xc24c0000, 
		0x9ff1823b, 
		0x4d8d4053, 
		0x80008000, 
		0x80010000, 
		0x0c7e0000, 
		0xcccccccc, 
		0x00000000, 
		0x00000000, 
		0x8523a2e7, 
		0x00002000, 
		0x00000080, 
		0x00400000, 
		0x0e000007, 
		0x80800001, 
		0x0c7e0000, 
		0x00001000, 
		0x41900000, 
		0xf19ff147, 
		0x0e000007, 
		0x0c600000, 
		0x80000400, 
		0xd1f548ae, 
		0x0d00fff0, 
		0x00000000, 
		0x40e00000, 
		0x7fffffff, 
		0x3f028f5c, 
		0x4b4369e9, 
		0x41700000, 
		0x7f800000, 
		0x8f7ffffd, 
		0x70f27cfa, 
		0xc2280000, 
		0xc1d00000, 
		0xe8fa7a24, 
		0x0e000003, 
		0x0c780000, 
		0x00000001, 
		0x00800000, 
		0x07e8d494, 
		0x0e1fffff, 
		0xf606d957, 
		0x01d3178b, 
		0x0f7ffffe, 
		0x80000000, 
		0xff7ffffe, 
		0x42500000, 
		0x41b00000, 
		0x00040000, 
		0x804b1f4e, 
		0xb70b53d0, 
		0x00ffffff, 
		0x97615122, 
		0x3f18d7d6, 
		0x0c7f8000, 
		0x00000000, 
		0x425c0000, 
		0x1236fd5f, 
		0x7f800000, 
		0x0c7f0000, 
		0x0c7c0000, 
		0x0e0003ff, 
		0x007fffff, 
		0xc2740000, 
		0x40800000, 
		0x41100000, 
		0x80000000, 
		0xb85c6d1b, 
		0x41d00000, 
		0x42240000, 
		0xc2300000, 
		0x3f800001, 
		0x82f61a7d, 
		0x8f7ffffe, 
		0x9e6bf8fb, 
		0x0c7ffffe, 
		0x00000000, 
		0x3568769c, 
		0x7fa8efd3, 
		0x80000000, 
		0x00080000, 
		0xc24c0000, 
		0x49b912f7, 
		0xc1a80000, 
		0x80200000, 
		0x41500000, 
		0xd298662c, 
		0x00000002, 
		0x00010000, 
		0x00000008, 
		0xbf800000, 
		0x22d6d2d8, 
		0xf56b8265, 
		0xc1c00000, 
		0x42180000, 
		0x0e01ffff, 
		0x6f83c3c0, 
		0x00000100, 
		0xff7ffffe, 
		0xac3137af, 
		0xbaa85873, 
		0x425c0000, 
		0x80000080, 
		0x007a2bb9, 
		0x42700000, 
		0x00000000, 
		0x007fffff, 
		0x00000000, 
		0x5a040c5a, 
		0xf03e7132, 
		0x00800002, 
		0xf3d68eb2, 
		0x00000000, 
		0x80002000, 
		0x913674c1, 
		0x8f7ffffc, 
		0xb479a62a, 
		0x0c7ffc00, 
		0x41f00000, 
		0x5f549b3b, 
		0x7f800000, 
		0x00000100, 
		0xc1e00000, 
		0x2ee5da62, 
		0xc1e80000, 
		0x66ec5d24, 
		0x42700000, 
		0x7e28d048, 
		0x517d0bda, 
		0xd578b56d, 
		0xc2200000, 
		0x7f800000, 
		0x42340000, 
		0x0e7fffff, 
		0xc1300000, 
		0x0e1fffff, 
		0x6727f7b6, 
		0xc1f80000, 
		0x9d4da4b4, 
		0x00800002, 
		0x807ffffe, 
		0x00080000, 
		0x7f800000, 
		0xab27a7a4, 
		0xa837a4c6, 
		0xc1880000, 
		0xc25c0000, 
		0x1a42a111, 
		0x80020000, 
		0x41500000, 
		0x41700000, 
		0xdb8c86c9, 
		0x92dde4c5, 
		0x41d00000, 
		0x0c7e0000, 
		0x7f800000, 
		0xc1600000, 
		0x80100000, 
		0x42240000, 
		0xa8dfe282, 
		0x80800000, 
		0x007f2932, 
		0x0c7fff00, 
		0xff8fadfd, 
		0x0e001fff, 
		0xc1700000, 
		0xffa0e8d7, 
		0x807ffffe, 
		0xc2780000, 
		0xc27c0000, 
		0x80000001, 
		0xffbeb6fc, 
		0x8f7ffffc, 
		0x0e000fff, 
		0x80000008, 
		0x42500000, 
		0x98c71d55, 
		0x00000000, 
		0x0c7fe000, 
		0x7d94d507, 
		0x68f074d5, 
		0x8db7697e, 
		0xc2700000, 
		0x80000008, 
		0x80800002, 
		0x00004000, 
		0x427c0000, 
		0x42700000, 
		0xf75b0e7c, 
		0x00004000, 
		0xbf800001, 
		0x9e6315f4, 
		0x41000000, 
		0x00000020, 
		0xf1a63322, 
		0xc1100000, 
		0x80800001, 
		0x80000000, 
		0x7f7fffff, 
		0x00008000, 
		0x7f800000, 
		0x80000001, 
		0x41880000, 
		0x2857c4f1, 
		0x8f7ffffd, 
		0x80800003, 
		0x80000002, 
		0x16bf9321, 
		0x0c7f8000, 
		0xc2240000, 
		0xc1500000, 
		0xb3f5d6c5, 
		0x00000100, 
		0x33333333, 
		0x00000000, 
		0xc1e80000, 
		0xc1f00000, 
		0x80517b61, 
		0x41900000, 
		0x80000000, 
		0xa984beab, 
		0x424c0000, 
		0x7f800000, 
		0xc1d00000, 
		0x34d94145, 
		0x8f7fffff, 
		0x010462f1, 
		0x00400000, 
		0x00000080, 
		0xff800000, 
		0xc2580000, 
		0x0c7fff80, 
		0xc647c469, 
		0x0e07ffff, 
		0x42580000, 
		0xffc0f389, 
		0xdfad07c8, 
		0xf4ef3b7c, 
		0xf5d36063, 
		0xe03d3eae, 
		0x0e00003f, 
		0x00ffffff, 
		0x41880000, 
		0x94a58f42, 
		0xff96febc, 
		0x41d80000, 
		0x0c7fff80, 
		0x00800002, 
		0x55555555, 
		0x00010000, 
		0x41300000, 
		0x41000000, 
		0xe11a1351, 
		0x80100000, 
		0x0e07ffff, 
		0xc2740000, 
		0xc1400000, 
		0xbf028f5c, 
		0x007fffff, 
		0xa26d1e58, 
		0xea909c62, 
		0xc2000000, 
		0x80000100, 
		0x7f800000, 
		0x7f800000, 
		0x00010000, 
		0x00008000, 
		0x0e000007, 
		0x00080000, 
		0xc1900000, 
		0x188d8ae8, 
		0x42540000, 
		0x00000008, 
		0x7f7ffffe, 
		0xfcfd85ac, 
		0xb79e46fb, 
		0x196f8c8a, 
		0x701188d7, 
		0x5746cf48, 
		0x80000000, 
		0x42080000, 
		0x80000000, 
		0x42140000, 
		0xd59d5926, 
		0xff800000, 
		0x42300000, 
		0x33333333, 
		0x7f7ffffe, 
		0x40c00000, 
		0xc0a00000, 
		0xb5bf5458, 
		0x80000002, 
		0x3f800001, 
		0x1fae2a44, 
		0xc2280000, 
		0x42461767, 
		0x40e00000, 
		0x7fc00001, 
		0xffbfffff, 
		0x18517068, 
		0x0e003fff, 
		0x42380000, 
		0x80000200, 
		0x80080000, 
		0xffc00000, 
		0x4a343993, 
		0x00011111, 
		0x42100000, 
		0x424c0000, 
		0xc2500000, 
		0x4275f278, 
		0xc2300000, 
		0x0c7f0000, 
		0x00000000, 
		0xebe0689e, 
		0x80000001, 
		0x80000000, 
		0x40a00000, 
		0x7f800000, 
		0x7f800000, 
		0x00000000, 
		0x00800001, 
		0xff7fffff, 
		0x7f7fffff, 
		0x00800000, 
		0xff800000, 
		0x00000000, 
		0x42100000, 
		0x7fbfffff, 
		0x0e000007, 
		0xff800000, 
		0x41e80000, 
		0x0d524c85, 
		0xaaaaaaaa, 
		0x80010000, 
		0x9ce61e6a, 
		0xc2240000, 
		0x80000000, 
		0xd0132588, 
		0x41c80000, 
		0x00000010, 
		0x00000800, 
		0x1dffd0c6, 
		0x79a935a6, 
		0x7fc00001, 
		0x00008000, 
		0x99babbf3, 
		0xc26c0000, 
		0xff800000, 
		0x41e80000, 
		0x44307629, 
		0x00000020, 
		0x0c600000, 
		0x0c400000, 
		0xc1c00000, 
		0x3f800000, 
		0x77e536de, 
		0x6d4ca0e6, 
		0xff800000, 
		0xc1d00000, 
		0x41a80000, 
		0x00000000, 
		0x425c0000, 
		0x80800002, 
		0x00000000, 
		0x0e01ffff, 
		0x55555555, 
		0xe16af5e0, 
		0x007a375b, 
		0x7f800001, 
		0xc2740000, 
		0x007fffff, 
		0x2801826f, 
		0x41300000, 
		0x0f7ffffc, 
		0x661a1c7f, 
		0xc2740000, 
		0xaaaaaaaa, 
		0x42340000, 
		0x15b85c14, 
		0xa3e7e287, 
		0xb2de25fb, 
		0x7f800000, 
		0x42780000, 
		0x28a7594a, 
		0xbeb769ed, 
		0x1d55fc4c, 
		0x00800000, 
		0x0e00001f, 
		0x80000000, 
		0x80000000, 
		0x42000000, 
		0x40e00000, 
		0x42780000, 
		0x00800000, 
		0x7fb496a0, 
		0x00000000, 
		0x42480000, 
		0xd258f944, 
		0x00000020, 
		0x2107ac67, 
		0x00800001, 
		0x80000004, 
		0xffb09269, 
		0x80000001, 
		0x9d74449c, 
		0x42240000, 
		0x41e00000, 
		0x5f36ed88, 
		0x00ffffff, 
		0x41d00000, 
		0x1c636ee7, 
		0x427c0000, 
		0x00800001, 
		0xffaa5a9e, 
		0x42180000, 
		0x7f9932af, 
		0x505e632f, 
		0xc2400000, 
		0x1e15025e, 
		0xc1400000, 
		0x80020000, 
		0x80000000, 
		0x7fbc721c, 
		0xc2780000, 
		0xff7ffffe, 
		0x7f8e6231, 
		0x7f800001, 
		0x0e7fffff, 
		0xc7e520fc, 
		0x80000000, 
		0x80200000, 
		0x80000000, 
		0x7fc00000, 
		0x7f7ffffe, 
		0x0c7fff00, 
		0xc2680000, 
		0xff800000, 
		0x55e36500, 
		0xc2040000, 
		0x7f800000, 
		0x00000000, 
		0x41b00000, 
		0xb100ef91, 
		0x7f800000, 
		0x2429923e, 
		0x80800000, 
		0x42340000, 
		0x421c0000, 
		0x42400000, 
		0xc0000000, 
		0x42140000, 
		0x0e0fffff, 
		0x7e150ecc, 
		0x007fffff, 
		0x64186612, 
		0x0c7ff000, 
		0x00800003, 
		0x80000200, 
		0x80200000, 
		0xdcc58890, 
		0xe2d4e0b0, 
		0x41e00000, 
		0x7fc00000, 
		0xf16fd962, 
		0xdbf7546f, 
		0x9b8d28ba, 
		0x41d80000, 
		0x80000000, 
		0x00100000, 
		0x7db9d911, 
		0x80000000, 
		0xc0c00000, 
		0xcace042a, 
		0xc1a80000, 
		0x3e20bff2, 
		0x959a79e7, 
		0x3d590067, 
		0x48b0325b, 
		0xcccccccc, 
		0x7c9c0832, 
		0xc1e80000, 
		0x2ab9642b, 
		0x9e601e88, 
		0x40e00000, 
		0xffffffff, 
		0x00000000, 
		0x00000001, 
		0xc1900000, 
		0xff800000, 
		0x40c00000, 
		0x80800000, 
		0xff7ffffe, 
		0x980cd7f8, 
		0xc2140000, 
		0x42340000, 
		0x7db55340, 
		0x25f1fc6c, 
		0x41b80000, 
		0xc1400000, 
		0x29e595d8, 
		0x58eae806, 
		0x6a03b4a8, 
		0x7fc00001, 
		0x80000000, 
		0xc1800000, 
		0x25ca4414, 
		0x80000001, 
		0xcccccccc, 
		0xffffffff, 
		0xff7ffffe, 
		0xe0b306b5, 
		0xc23c0000, 
		0xc2300000, 
		0x80800002, 
		0xc2240000, 
		0xff7fffff, 
		0xc1980000, 
		0x0e7fffff, 
		0x522cadc2, 
		0xc1a00000, 
		0x42300000, 
		0xc3ecb0e5, 
		0x00000001, 
		0xa82a6c40, 
		0x9a045489, 
		0x80800003, 
		0x978bfd2e, 
		0x0c7ffc00, 
		0x04e8a2e9, 
		0x42100000, 
		0x42780000, 
		0x00000000, 
		0xe49cb82f, 
		0x58fd3d8c, 
		0x6f15bc07, 
		0x03750ef9, 
		0x72368e99, 
		0x80000000, 
		0x0c7ffff0, 
		0x9d95c09c, 
		0x311f9847, 
		0xaa0e448c, 
		0xaaaaaaaa, 
		0x7fab53be, 
		0xb5131415, 
		0x2aaebe22, 
		0x7f800000, 
		0x7f800000, 
		0xa98ff91b, 
		0x42600000, 
		0x62aad91f, 
		0x00000100, 
		0x41a00000, 
		0xa7d9ee60, 
		0x7f800000, 
		0x0c6d8038, 
		0x0e3fffff, 
		0xc20c0000, 
		0xcccccccc, 
		0x8f7fffff, 
		0xc1d00000, 
		0xc20c0000, 
		0x732814c7, 
		0x00000000, 
		0x82e159d4, 
		0xbf800000, 
		0xc2040000, 
		0x6cc048e2, 
		0x42240000, 
		0x3c32860b, 
		0x1c0989c8, 
		0xdf39a6ad, 
		0x0c7ffff8, 
		0x80000000, 
		0x00800002, 
		0x00000000, 
		0x80200000, 
		0x8f7ffffd, 
		0x7f800000, 
		0x0c7fc000, 
		0x80ffffff, 
		0x00020000, 
		0x41500000, 
		0xcb8c4b40, 
		0x30790cad, 
		0xff800000, 
		0x88d4a554, 
		0xd4ff1779, 
		0x6d6f1881, 
		0x00008000, 
		0x00000000, 
		0xc2700000, 
		0x80800000, 
		0x0f7ffffe, 
		0x18ddae99, 
		0x00800001, 
		0xc24c0000, 
		0x80800000, 
		0xc1a00000, 
		0xe6de8578, 
		0x00040000, 
		0x0c7fc000, 
		0xc2440000, 
		0x0b351fcd, 
		0x80000000, 
		0x00000100, 
		0xc2340000, 
		0x0d00fff0, 
		0x007fffff, 
		0xc0400000, 
		0xcccccccc, 
		0xab3c05e4, 
		0x653ede67, 
		0x990b4943, 
		0x0013d4c6, 
		0xc1600000, 
		0xffa3b418, 
		0x00e8e82d, 
		0x80000000, 
		0xc1980000, 
		0x8f7ffffd, 
		0x42480000, 
		0x00000008, 
		0x60f05323, 
		0xefa084f8, 
		0x7312b1bd, 
		0xc2300000, 
		0x3f800001, 
		0x6038e4b7, 
		0x80000020, 
		0x0c7fff80, 
		0xcb000000, 
		0x9423b948, 
		0x00000000, 
		0x40c00000, 
		0xbf028f5c, 
		0x44618d4d, 
		0x93d853a5, 
		0xffa79bad, 
		0x41f80000, 
		0x690392a1, 
		0xff897441, 
		0x478aa203, 
		0x269f2d6a, 
		0x7f000000, 
		0x1b5c6566, 
		0x579ce354, 
		0x4b8c4b40, 
		0xc1300000, 
		0x42400000, 
		0x41400000, 
		0x00000000, 
		0x7828c48b, 
		0xcfaa4e1d, 
		0xb2875c9a, 
		0xc776c883, 
		0x80000000, 
		0xb73b0062, 
		0xd966f608, 
		0x42200000, 
		0xff7ffffe, 
		0xc1c80000, 
		0x80800001, 
		0x42600000, 
		0x41f80000, 
		0x0e00003f, 
		0x0e00007f, 
		0x80000000, 
		0x00400000, 
		0x42380000, 
		0x0e00007f, 
		0x7fc00000, 
		0x41c00000, 
		0xc1100000, 
		0x8f7fffff, 
		0xc0c00000, 
		0x0c7fffe0, 
		0x0e00007f, 
		0x4be1bc20, 
		0xc3de7634, 
		0xd31a1c96, 
		0xfe8d0b30, 
		0xff7ffffe, 
		0x7f86fae7, 
		0x00000000, 
		0xc2600000, 
		0x80000400, 
		0x7f8b4f42, 
		0x0c7fffc0, 
		0x00010000, 
		0x0e0000ff, 
		0x007fffff, 
		0x6a4e7ac9, 
		0x00000000, 
		0x8af0efa3, 
		0xe9387c3f, 
		0x8ae21266, 
		0x7f7ffffe, 
		0x807fffff, 
		0x00000800, 
		0x0d000ff0, 
		0x41400000, 
		0xf7ea1ea4, 
		0x41d00000, 
		0xc2580000, 
		0xc22c0000, 
		0x41800000, 
		0x110a2c57, 
		0xc2540000, 
		0xc21c0000, 
		0x61882e48, 
		0x41e00000, 
		0x24957ce7, 
		0x00080000, 
		0x0e07ffff, 
		0x0c600000, 
		0x7f7fffff, 
		0x7fbfffff, 
		0xc1900000, 
		0x396a1477, 
		0x421c0000, 
		0x42440000, 
		0x41f80000, 
		0x80080000, 
		0x00000000, 
		0x8e113f20, 
		0x41200000, 
		0x80000400, 
		0xff869e9c, 
		0x40800000, 
		0x80800001, 
		0x80000002, 
		0x42600000, 
		0xc1d80000, 
		0xc1980000, 
		0x42680000, 
		0xc2040000, 
		0x42440000, 
		0xd8f241bf, 
		0x42780000, 
		0xc0c00000, 
		0xc2640000, 
		0x8f7fffff, 
		0x7de0754b, 
		0xc1880000, 
		0x00001000, 
		0xe3ffce31, 
		0xc0e00000, 
		0xff800000, 
		0xd28e2206, 
		0x9ec68ab2, 
		0x0c7ffffc, 
		0x42680000, 
		0x80000080, 
		0xba115c50, 
		0x80000002, 
		0x80800000, 
		0x0285054e, 
		0x0c7ffff0, 
		0x7dcfcbdd, 
		0x8063a70a, 
		0x0f7ffffc, 
		0x3510df52, 
		0x41000000, 
		0x80000080, 
		0x00000000, 
		0x03f4804a, 
		0x41a80000, 
		0x00000000, 
		0xc2700000, 
		0xffbfffff, 
		0x3feb6ef2, 
		0xf3e79e97, 
		0x42500000, 
		0x11f92e5c, 
		0x1072085a, 
		0x41980000, 
		0x0e003fff, 
		0xaf63aa3a, 
		0xd7daba4b, 
		0xc27c0000, 
		0x422c0000, 
		0x80008000, 
		0x7f7ffffe, 
		0x80000080, 
		0xff800000, 
		0xff800000, 
		0x40e00000, 
		0x7fc00000, 
		0xc0000000, 
		0xfda91233, 
		0x0c780000, 
		0x0f7ffffd, 
		0xffc00000, 
		0x41880000, 
		0x0e0000ff, 
		0xfce1fac9, 
		0x843c9e79, 
		0xc1600000, 
		0x80000002, 
		0x0e003fff, 
		0x43954c00, 
		0x9fc23245, 
		0x7f800001, 
		0xc27c0000, 
		0x7f800000, 
		0x41800000, 
		0x80800002, 
		0x0e3fffff, 
		0x42780000, 
		0x0c7ff800, 
		0xb7638ba8, 
		0x0c7ffffc, 
		0x42740000, 
		0x00000010, 
		0x5aa5f5de, 
		0x7f800000, 
		0xff800000, 
		0x41700000, 
		0x99a83212, 
		0xd3ddd803, 
		0xf492355b, 
		0x0c7fc000, 
		0x9ba6de32, 
		0x7194d9bf, 
		0x8bcf94ff, 
		0x0e03ffff, 
		0x2cc30f0b, 
		0x0c780000, 
		0x40000000, 
		0x7f7fffff, 
		0x80000008, 
		0x39025aa5, 
		0x42180000, 
		0xe6c66330, 
		0xff800001, 
		0x427c0000, 
		0xc2400000, 
		0x80000001, 
		0xffc00000, 
		0xed654b87, 
		0xa66a576e, 
		0x0c780000, 
		0x7f8b0d2d, 
		0x7fe4b379, 
		0x00000002, 
		0xc2140000, 
		0x40800000, 
		0xff000000, 
		0x0e0001ff, 
		0x0c600000, 
		0x421c0000, 
		0x6a5bf35a, 
		0x0e0fffff, 
		0xff80c86d, 
		0x7f7ffffe, 
		0x41980000, 
		0x00000800, 
		0xc2100000, 
		0x3f028f5c, 
		0x00010000, 
		0x42640000, 
		0x41800000, 
		0x2cb62fe4, 
		0x0c7ffff8, 
		0xc1b00000, 
		0x00200000, 
		0x00800000, 
		0xc20c0000, 
		0x41429ecf, 
		0x1fb80231, 
		0x5316b570, 
		0x7f9eec25, 
		0xbe302b35, 
		0x41000000, 
		0x00000000, 
		0xcb000000, 
		0x80011111, 
		0x00ffffff, 
		0x7f7ffffe, 
		0x00000080, 
		0x00000002, 
		0x00000008, 
		0xc15dbac7, 
		0xc2780000, 
		0x42140000, 
		0x80000008, 
		0x0e0fffff, 
		0x5445e399, 
		0xc1300000, 
		0x8bebd04f, 
		0xc1980000, 
		0x7f800000, 
		0x80000800, 
		0x80000000, 
		0xe87ba33d, 
		0xc2200000, 
		0x4b8c4b40, 
		0x41d80000, 
		0x41d00000, 
		0x42100000, 
		0xff902067, 
		0x01a1ddae, 
		0xff000000, 
		0x0e001fff, 
		0x807a9ab8, 
		0xb1497b23, 
		0x0c7c0000, 
		0x53b71f98, 
		0x80000020, 
		0xc0a00000, 
		0x41500000, 
		0xc2400000, 
		0x421c0000, 
		0x00000002, 
		0x9787b09e, 
		0xff800000, 
		0xc2380000, 
		0x0e3fffff, 
		0xc0e00000, 
		0x41b80000, 
		0xff91dd47, 
		0xc2640000, 
		0xc2440000, 
		0x41b00000, 
		0xd83936f4, 
		0x8fdb7ff9, 
		0xc1a80000, 
		0x40000000, 
		0x6fc24c6e, 
		0x0e001fff, 
		0xc0400000, 
		0x80000000, 
		0x51135d68, 
		0x7f000000, 
		0x423c0000, 
		0x0c7ffe00, 
		0xffffffff, 
		0x0c7fff00, 
		0xff7fffff, 
		0xc7b508f8, 
		0x5e1dff57, 
		0x0e7fffff, 
		0x40a00000, 
		0x9516fa7d, 
		0xc21c0000, 
		0x80200000, 
		0x80000000, 
		0xc2780000, 
		0xa7fb623e, 
		0x0e0003ff, 
		0x7fc00000, 
		0xf016d05d, 
		0x80ffffff, 
		0x423c0000, 
		0x80000000, 
		0x00000000, 
		0x80000400, 
		0x41a00000, 
		0x63d49f2d, 
		0x0c7ff800, 
		0xc1d80000, 
		0x807ffffe, 
		0x7f7ffffe, 
		0xffc24418, 
		0x80001000, 
		0x0e000001, 
		0x42780000, 
		0xfbf265ad, 
		0x3f800000, 
		0xff800000, 
		0xc22c0000, 
		0xc1600000, 
		0x7d91b4a2, 
		0xffc00000, 
		0xb0a257a6, 
		0x8d715592, 
		0x42580000, 
		0xff7ffffe, 
		0x00000000, 
		0x83f83d8f, 
		0x7d697dd6, 
		0xc1100000, 
		0x41d00000, 
		0x80020000, 
		0xc25c0000, 
		0x41c00000, 
		0xc2380000, 
		0x0c7ffff8, 
		0xc1100000, 
		0x513d62de, 
		0x7f800000, 
		0x80000000, 
		0xc1500000, 
		0x423c0000, 
		0xc2040000, 
		0x97681c6b, 
		0x3911eaad, 
		0x0e7fffff, 
		0xbade5c15, 
		0x960075ee, 
		0x41d00000, 
		0x5b4af13a, 
		0xd0ad2fdf, 
		0xc2580000, 
		0xc0e00000, 
		0xc1c80000, 
		0x7f81da0a, 
		0x80040000, 
		0xf8561e1e, 
		0x9b741bdb, 
		0x42540000, 
		0x41d00000, 
		0xd852325c, 
		0xc1a80000, 
		0x0e000001, 
		0x80000800, 
		0xc24c0000, 
		0x80000000, 
		0xc1880000, 
		0xc1d00000, 
		0x80000002, 
		0x0c7ff000, 
		0xbf800001, 
		0xc1f80000, 
		0x23ff0312, 
		0xc2480000, 
		0x387907ff, 
		0xc1c00000, 
		0x0c7f8000, 
		0x00800003, 
		0xc1000000, 
		0xb0467b2d, 
		0xff800000, 
		0x40000000, 
		0x880691cf, 
		0x00000000, 
		0x42640000, 
		0x80800002, 
		0x0e0007ff, 
		0x00000200, 
		0x00008000, 
		0xc1e80000, 
		0x0e0007ff, 
		0x212df2ad, 
		0xcccccccc, 
		0x0e00003f, 
		0x00000000, 
		0x00000000, 
		0x0c7ff000, 
		0x7faa91a0, 
		0x0c7fffff, 
		0x42640000, 
		0xffbfffff, 
		0x5ea7a7bc, 
		0xc2200000, 
		0x8f7ffffd, 
		0x0f7ffffc, 
		0x38743e42, 
		0x000c6b29, 
		0xc2a3c6d4, 
		0x00000000, 
		0xc0000000, 
		0x7fc00000, 
		0x7f7ffffe, 
		0x00400000, 
		0x0c7f0000, 
		0x0d000ff0, 
		0xda6df03a, 
		0x0e9a923b, 
		0xa1ce8065, 
		0x41800000, 
		0x9c2eba3f, 
		0xff7ffffe, 
		0x7f7fffff, 
		0x0cfef9ef, 
		0x6f1606d5, 
		0x80080000, 
		0x7f7fffff, 
		0x80011111, 
		0xf7fbdc6c, 
		0xc2340000, 
		0xffa23e7c, 
		0x80010000, 
		0x9dcdf88a, 
		0xd784bb40, 
		0xff000000, 
		0x422c0000, 
		0x80000000, 
		0x0e000fff, 
		0x006764fd, 
		0xfc7af9d6, 
		0x80800002, 
		0x41d80000, 
		0x75d94e1c, 
		0xc2240000, 
		0xc2780000, 
		0xf4d3278f, 
		0x426c0000, 
		0x00000008, 
		0x42640000, 
		0x33613520, 
		0x0c7ffffe, 
		0x0c7fff80, 
		0xf42d72d5, 
		0x80000400, 
		0x0e007fff, 
		0x03e47a1b, 
		0xe4120396, 
		0xc1400000, 
		0x4e037051, 
		0xc1e00000, 
		0xc2480000, 
		0x7f800000, 
		0x80008000, 
		0x00001000, 
		0x7f800000, 
		0xcdb69135, 
		0x4b000000, 
		0xf34e3f40, 
		0x06d64a8f, 
		0x76aa1f4e, 
		0x00000000, 
		0xbbe79d8d, 
		0x6907454f, 
		0x40c00000, 
		0x7f7ffffe, 
		0x42580000, 
		0x145c2566, 
		0x8c89e2d7, 
		0xc2540000, 
		0xc22c0000, 
		0x0f7ffffd, 
		0x2beeddf4, 
		0xea7ecf7c, 
		0x68b78dcc, 
		0x41100000, 
		0xff800000, 
		0x80000000, 
		0x41f00000, 
		0xc22c0000, 
		0x0e07ffff, 
		0x0c98a17c, 
		0xc0400000, 
		0x00000010, 
		0xa3c047cd, 
		0x00010000, 
		0x00800000, 
		0x41880000, 
		0xc0e00000, 
		0x40400000, 
		0x00000400, 
		0x41a00000, 
		0x00800001, 
		0x103defcc, 
		0x00ffffff, 
		0x240340bf  
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
		0x00000155, 
		0x00000379, 
		0x000001b7, 
		0x000001bd, 
		0x000001f5, 
		0x00000293, 
		0x00000085, 
		0x000000cd, 
		0x0000004f, 
		0x000003af, 
		0x00000159, 
		0x00000123, 
		0x00000145, 
		0x0000016d, 
		0x00000307, 
		0x00000059, 
		0x000002d5, 
		0x000001d5, 
		0x0000035f, 
		0x000000e1, 
		0x00000275, 
		0x00000223, 
		0x00000119, 
		0x0000001f, 
		0x00000399, 
		0x00000331, 
		0x00000227, 
		0x00000107, 
		0x000003b3, 
		0x00000321, 
		0x00000157, 
		0x00000341, 
		0x000002ff, 
		0x000003dd, 
		0x00000371, 
		0x000000fb, 
		0x00000225, 
		0x000002bf, 
		0x000001f3, 
		0x00000307, 
		0x0000031b, 
		0x0000016b, 
		0x00000071, 
		0x00000233, 
		0x000002d9, 
		0x000001b5, 
		0x00000161, 
		0x00000391, 
		0x000002cf, 
		0x0000022d, 
		0x0000017d, 
		0x00000259, 
		0x000000bb, 
		0x0000024f, 
		0x00000261, 
		0x000000c7, 
		0x000003cd, 
		0x0000002d, 
		0x0000014b, 
		0x000001b9, 
		0x00000381, 
		0x000001c1, 
		0x0000002d, 
		0x000001d7, 
		0x00000151, 
		0x000000c7, 
		0x0000030f, 
		0x00000091, 
		0x0000037d, 
		0x00000025, 
		0x000002f3, 
		0x000002fd, 
		0x000003f7, 
		0x000003ab, 
		0x000002f3, 
		0x000002dd, 
		0x00000043, 
		0x00000055, 
		0x00000051, 
		0x0000030f, 
		0x0000019f, 
		0x00000349, 
		0x000003af, 
		0x000000cd, 
		0x0000011d, 
		0x00000147, 
		0x000003b9, 
		0x00000103, 
		0x0000008b, 
		0x000000ad, 
		0x000000a3, 
		0x00000383, 
		0x000002b9, 
		0x00000261, 
		0x0000006b, 
		0x00000225, 
		0x0000019f, 
		0x000000c5, 
		0x000001fb, 
		0x000003ef, 
		0x000001e9, 
		0x000003fd, 
		0x000003af, 
		0x00000087, 
		0x0000028f, 
		0x000003e7, 
		0x000003b5, 
		0x00000353, 
		0x0000012d, 
		0x000001db, 
		0x00000329, 
		0x00000309, 
		0x000002c9, 
		0x000001d3, 
		0x000003c9, 
		0x0000023f, 
		0x000002e7, 
		0x0000030f, 
		0x00000111, 
		0x000003db, 
		0x00000233, 
		0x000001ef, 
		0x0000024d, 
		0x00000341, 
		0x000002cb, 
		0x00000123, 
		0x00000371, 
		0x00000115, 
		0x00000031, 
		0x000001ad, 
		0x000000e9, 
		0x000003a7, 
		0x00000141, 
		0x00000377, 
		0x00000281, 
		0x0000008d, 
		0x000000ef, 
		0x0000010d, 
		0x00000231, 
		0x0000002b, 
		0x00000387, 
		0x000002df, 
		0x000003e3, 
		0x0000017b, 
		0x00000397, 
		0x000002fb, 
		0x00000323, 
		0x000001a5, 
		0x0000021f, 
		0x000001bd, 
		0x0000006f, 
		0x0000018d, 
		0x000003d5, 
		0x00000117, 
		0x0000030b, 
		0x000000ef, 
		0x0000000b, 
		0x000000a1, 
		0x00000397, 
		0x00000135, 
		0x00000239, 
		0x000003e7, 
		0x00000051, 
		0x00000079, 
		0x00000007, 
		0x000003f9, 
		0x0000009b, 
		0x00000095, 
		0x00000151, 
		0x00000307, 
		0x00000375, 
		0x00000159, 
		0x00000109, 
		0x000003c5, 
		0x00000203, 
		0x00000231, 
		0x00000387, 
		0x000001d7, 
		0x00000293, 
		0x000000f5, 
		0x00000203, 
		0x000001bf, 
		0x00000197, 
		0x00000217, 
		0x000000d9, 
		0x000001b1, 
		0x00000347, 
		0x00000239, 
		0x00000133, 
		0x0000002b, 
		0x000002f1, 
		0x000002e3, 
		0x000000e1, 
		0x0000000f, 
		0x00000319, 
		0x000001f9, 
		0x000001bd, 
		0x00000301, 
		0x00000249, 
		0x000001d5, 
		0x000000cf, 
		0x000001c7, 
		0x000002cb, 
		0x00000393, 
		0x00000091, 
		0x0000011f, 
		0x000000db, 
		0x0000019b, 
		0x00000175, 
		0x00000345, 
		0x0000016b, 
		0x00000197, 
		0x0000023f, 
		0x000002a1, 
		0x00000151, 
		0x00000257, 
		0x00000371, 
		0x00000185, 
		0x0000007f, 
		0x000000eb, 
		0x00000123, 
		0x000003b1, 
		0x000002cd, 
		0x000002ab, 
		0x0000017d, 
		0x000002c5, 
		0x0000003f, 
		0x00000361, 
		0x000002cf, 
		0x0000005b, 
		0x000001df, 
		0x00000173, 
		0x000003ef, 
		0x0000014d, 
		0x0000017f, 
		0x0000010f, 
		0x0000036b, 
		0x00000383, 
		0x00000317, 
		0x00000049, 
		0x0000030b, 
		0x00000313, 
		0x00000107, 
		0x000003fb, 
		0x00000135, 
		0x000000fd, 
		0x000002ed, 
		0x000001b5, 
		0x00000205, 
		0x000000e9, 
		0x00000277, 
		0x0000003f, 
		0x00000089, 
		0x0000038d, 
		0x000002bf, 
		0x00000249  
};

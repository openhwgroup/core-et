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
			"li x5, 470\n"
			"faddi.pi f2, f0, -246\n"
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
			"csrwi mhpmevent4, 26\n"   
			"csrwi mhpmevent5, 27\n"   
			"csrwi mhpmevent6, 19\n"   
			"csrwi mhpmevent7, 5\n"   
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
			"flw.ps f23,  416 (x5)\n"
			"flw.ps f12,  480 (x5)\n"
			"flw.ps f18,  736 (x5)\n"
			"flw.ps f30,  704 (x5)\n"
			"flw.ps f21,  544 (x5)\n"
			"flw.ps f28,  800 (x5)\n"
			"flw.ps f4,  192 (x5)\n"
			"flw.ps f7,  256 (x5)\n"
			"flw.ps f9,  960 (x5)\n"
			"flw.ps f0,  768 (x5)\n"
			"flw.ps f17,  864 (x5)\n"
			"flw.ps f10,  352 (x5)\n"
			"flw.ps f27,  224 (x5)\n"
			"flw.ps f25,  320 (x5)\n"
			"flw.ps f26,  288 (x5)\n"
			"flw.ps f19,  512 (x5)\n"
			"flw.ps f22,  448 (x5)\n"
			"flw.ps f15,  608 (x5)\n"
			"flw.ps f16,  32 (x5)\n"
			"flw.ps f6,  928 (x5)\n"
			"flw.ps f3,  384 (x5)\n"
			"flw.ps f1,  96 (x5)\n"
			"flw.ps f11,  576 (x5)\n"
			"flw.ps f24,  640 (x5)\n"
			"flw.ps f31,  0 (x5)\n"
			"flw.ps f20,  64 (x5)\n"
			"flw.ps f14,  128 (x5)\n"
			"flw.ps f29,  160 (x5)\n"
			"flw.ps f2,  832 (x5)\n"
			"flw.ps f5,  672 (x5)\n"
			"flw.ps f8,  992 (x5)\n"
			"flw.ps f13,  896 (x5)\n"
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
			"li x0, 0x76c\n" 
			"slti x0, x0, 0x7f2\n"
			"la x31, check_sum_hid_0\n"
			"li x5, 0x2\n" 
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
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_0:\n"
			"mov.m.x m4, x28, 0x60\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x11, rand_ieee754_32\n"
			"flw.ps f22, 544(x11)\n"
			"flw.ps f5, 992(x11)\n"
			"fsgnj.s f20, f22, f5\n" 
			"la x25, rand_int_32\n"
			"flw.ps f30, 64(x25)\n"
			"flw.ps f7, 576(x25)\n"
			"fltu.pi f13, f30, f7\n" 
			"LBL_SEQID_0_FP_TRANS_ID_1_HID_0:\n"
			"la x7, rand_ieee754_32\n"
			"flw.ps f2, 160(x7)\n"
			"fexp.ps f26, f2\n" 
			"la x18, rand_int_32\n"
			"flw.ps f5, 576(x18)\n"
			"flw.ps f0, 768(x18)\n"
			"fsrl.pi f8, f5, f0\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f29, 416(x11)\n"
			"flw.ps f31, 192(x11)\n"
			"fmadd.ps f26, f29, f31, f29, rtz\n" 
			"la x27, rand_int_32\n"
			"flw.ps f13, 224(x27)\n"
			"flw.ps f18, 896(x27)\n"
			"for.pi f18, f13, f18\n" 
			"LBL_SEQID_0_M0_WRITE_ID_2_HID_0:\n"
			"mova.m.x x9\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_0_M0_WRITE_ID_3_HID_0:\n"
			"mova.m.x x27\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x6, rand_int_32\n"
			"flw.ps f24, 128(x6)\n"
			"flw.ps f3, 256(x6)\n"
			"flt.pi f14, f24, f3\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f4, 896(x5)\n"
			"flw.ps f8, 224(x5)\n"
			"fmul.s f0, f4, f8, rmm\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x5, rand_int_32\n"
			"flw.ps f20, 704(x5)\n"
			"flw.ps f0, 64(x5)\n"
			"feq.pi f6, f20, f0\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f5, 960(x9)\n"
			"fsqrt.ps f22, f5\n" 
			"la x23, rand_int_32\n"
			"flw.ps f3, 640(x23)\n"
			"fnot.pi f11, f3\n" 
			"la x23, rand_int_32\n"
			"flw.ps f3, 576(x23)\n"
			"fpackrepb.pi f10, f3\n" 
			"fbci.ps f29, 0x21c81\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_0_RDX_ID_0_SHIRE_0_HID_0:\n"
			"li x25, 0x320000000741000a\n"
			"csrw tensor_reduce, x25\n" 
			"csrwi tensor_wait, 9\n" 
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=0); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ITER_0_HID_0:\n"
			"li x5, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x2 << TENSOR_FMA32_B_NUM_COLS) | (0xf << TENSOR_FMA32_A_NUM_ROWS) | (0xf << TENSOR_FMA32_A_NUM_COLS) | (0xc << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0xb3 << TENSOR_FMA32_SCP_LOC_B) | (0x21 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x0 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_0\n"
			"csrw tensor_error, x0\n"
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_0\n"
			"addi x30, x30, 1\n"
			"la x16, rand_ieee754_32\n"
			"flw.ps f16, 256(x16)\n"
			"flw.ps f18, 416(x16)\n"
			"fadd.ps f19, f16, f18, rtz\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f25, 288(x18)\n"
			"fcvt.w.s x5, f25, rtz\n" 
			"la x17, rand_int_32\n"
			"flw.ps f19, 832(x17)\n"
			"flw.ps f14, 352(x17)\n"
			"fsub.pi f1, f19, f14\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f23, 160(x26)\n"
			"flw.ps f2, 512(x26)\n"
			"flw.ps f7, 224(x26)\n"
			"fcmov.ps f15, f23, f2, f7\n" 
			"li x5, 0x9355\n"
			"csrw tensor_mask, x5\n"	
			"la x22, rand_ieee754_32\n"
			"flw.ps f0, 960(x22)\n"
			"flw.ps f2, 288(x22)\n"
			"flw.ps f25, 672(x22)\n"
			"fmadd.s f4, f0, f2, f25, rtz\n" 
			"fbci.ps f5, 0x4ceb4\n" 
			"LBL_SEQID_1_FP_TRANS_ID_4_HID_0:\n"
			"la x7, rand_ieee754_32\n"
			"flw.ps f22, 992(x7)\n"
			"fexp.ps f3, f22\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f23, 160(x6)\n"
			"flw.ps f25, 96(x6)\n"
			"flw.ps f5, 832(x6)\n"
			"fmadd.ps f7, f23, f25, f5, rmm\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f10, 416(x5)\n"
			"flw.ps f6, 640(x5)\n"
			"flt.s x16, f10, f6\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f26, 384(x22)\n"
			"flw.ps f18, 96(x22)\n"
			"fmax.s f13, f26, f18\n" 
			"li x13, 0xffec175ad6e304bd\n"
			"fcvt.s.w f19, x13, rne\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f21, 960(x22)\n"
			"fround.ps f24, f21, rtz\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f7, 864(x17)\n"
			"fclass.ps f25, f7\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f21, 480(x20)\n"
			"flw.ps f5, 416(x20)\n"
			"fadd.s f31, f21, f5, rdn\n" 
			"LBL_SEQID_1_M0_WRITE_ID_5_HID_0:\n"
			"maskor m4, m2, m4\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x20, rand_int_32\n"
			"flw.ps f28, 320(x20)\n"
			"fandi.pi f15, f28, 0x105\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ID_281685_HID_0:\n"
			"li x6, ((0x1b << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0xb << TENSOR_QUANT_QUANT_ROW) | (0xf << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x0 << TENSOR_QUANT_TRANSF1) | (0x2 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_0\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_1_RDX_ID_1_SHIRE_0_HID_0:\n"
			"li x13, 0x800000002280019\n"
			"csrr x6, mhartid\n"
			"srli x6, x6, 4\n"
			"slli x6, x6, 6\n"
			"add x13, x13, x6\n"
			"csrw tensor_reduce, x13\n" 
			"csrwi tensor_wait, 9\n" 
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=0); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ITER_0_HID_0:\n"
			"li x6, ((0x13 << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0xc << TENSOR_QUANT_QUANT_ROW) | (0x3 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x6 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
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
			"LBL_HPM_3_CORE_DCACHE_MISSES0_FAIL_HID_0:\n"          
			"csrr x5, mhpmcounter3\n"
			"sd x5, 32 (x31)\n"
			"LBL_HPM_4_CORE_TFMA_INST_FAIL_HID_0:\n"          
			"csrr x5, mhpmcounter4\n"
			"sd x5, 40 (x31)\n"
			"LBL_HPM_5_CORE_TREDUCE_INST_FAIL_HID_0:\n"          
			"csrr x5, mhpmcounter5\n"
			"sd x5, 48 (x31)\n"
			"LBL_HPM_6_CORE_TIMA_OPS_FAIL_HID_0:\n"          
			"csrr x5, mhpmcounter6\n"
			"sd x5, 56 (x31)\n"
			"LBL_HPM_7_NEIGH_RX_COOP_TLD_RSP_FAIL_HID_0:\n"          
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
			"LBL_HPM_3_CORE_DCACHE_MISSES0_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter3\n"
			"sd x5, 32 (x31)\n"
			"LBL_HPM_4_CORE_TFMA_INST_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter4\n"
			"sd x5, 40 (x31)\n"
			"LBL_HPM_5_CORE_TREDUCE_INST_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter5\n"
			"sd x5, 48 (x31)\n"
			"LBL_HPM_6_CORE_TIMA_OPS_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter6\n"
			"sd x5, 56 (x31)\n"
			"LBL_HPM_7_NEIGH_RX_COOP_TLD_RSP_PASS_HID_0:\n"          
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
			"csrwi mhpmevent3, 25\n"   
			"csrwi mhpmevent4, 11\n"   
			"csrwi mhpmevent5, 10\n"   
			"csrwi mhpmevent6, 6\n"   
			"csrwi mhpmevent7, 12\n"   
			"csrwi mhpmevent8, 1\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f16,  768 (x5)\n"
			"flw.ps f14,  192 (x5)\n"
			"flw.ps f26,  320 (x5)\n"
			"flw.ps f28,  672 (x5)\n"
			"flw.ps f29,  640 (x5)\n"
			"flw.ps f0,  544 (x5)\n"
			"flw.ps f15,  32 (x5)\n"
			"flw.ps f2,  160 (x5)\n"
			"flw.ps f4,  576 (x5)\n"
			"flw.ps f1,  288 (x5)\n"
			"flw.ps f3,  704 (x5)\n"
			"flw.ps f8,  960 (x5)\n"
			"flw.ps f25,  128 (x5)\n"
			"flw.ps f7,  864 (x5)\n"
			"flw.ps f19,  224 (x5)\n"
			"flw.ps f23,  512 (x5)\n"
			"flw.ps f6,  896 (x5)\n"
			"flw.ps f22,  480 (x5)\n"
			"flw.ps f11,  384 (x5)\n"
			"flw.ps f17,  928 (x5)\n"
			"flw.ps f27,  736 (x5)\n"
			"flw.ps f24,  992 (x5)\n"
			"flw.ps f10,  832 (x5)\n"
			"flw.ps f12,  800 (x5)\n"
			"flw.ps f13,  416 (x5)\n"
			"flw.ps f20,  96 (x5)\n"
			"flw.ps f18,  0 (x5)\n"
			"flw.ps f21,  448 (x5)\n"
			"flw.ps f31,  256 (x5)\n"
			"flw.ps f30,  352 (x5)\n"
			"flw.ps f9,  64 (x5)\n"
			"flw.ps f5,  608 (x5)\n"
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
			"la x25, rand_ieee754_32\n"
			"flw.ps f11, 736(x25)\n"
			"flw.ps f2, 96(x25)\n"
			"fmax.ps f7, f11, f2\n" 
			"fbci.pi f30, 0x132d1\n" 
			"li x22, 0x17770ce3bb8627fa\n"
			"fcvt.s.wu f11, x22, rne\n" 
			"la x7, rand_int_32\n"
			"flw.ps f29, 416(x7)\n"
			"flw.ps f3, 576(x7)\n"
			"feq.pi f28, f29, f3\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_1:\n"
			"fltm.pi m0, f10, f23\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_1:\n"
			"mova.m.x x6\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x9, rand_ieee754_32\n"
			"flw.ps f31, 928(x9)\n"
			"fclass.s x23, f31\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f30, 960(x7)\n"
			"fsin.ps f31, f30\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f10, 800(x17)\n"
			"flw.ps f14, 352(x17)\n"
			"flt.s x10, f10, f14\n" 
			"la x11, rand_int_32\n"
			"flw.ps f13, 768(x11)\n"
			"fsat8.pi f16, f13\n" 
			"la x12, rand_int_32\n"
			"flw.ps f27, 224(x12)\n"
			"flw.ps f14, 992(x12)\n"
			"fmul.pi f6, f27, f14\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f4, 64(x20)\n"
			"flw.ps f20, 960(x20)\n"
			"flw.ps f21, 0(x20)\n"
			"fnmsub.ps f18, f4, f20, f21, rmm\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f12, 416(x10)\n"
			"fround.ps f7, f12, rmm\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f31, 640(x26)\n"
			"fle.s x10, f31, f31\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f18, 160(x28)\n"
			"fsgnjx.ps f21, f18, f18\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f5, 224(x20)\n"
			"flw.ps f31, 768(x20)\n"
			"flt.ps f17, f5, f31\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_1\n"
			"addi x30, x30, 1\n"
			"la x17, rand_ieee754_32\n"
			"flw.ps f15, 928(x17)\n"
			"flw.ps f9, 480(x17)\n"
			"fmul.s f20, f15, f9, rtz\n" 
			"LBL_SEQID_1_FP_TRANS_ID_2_HID_1:\n"
			"la x15, rand_ieee754_32\n"
			"flw.ps f19, 192(x15)\n"
			"fexp.ps f6, f19\n" 
			"LBL_SEQID_1_FP_TRANS_ID_3_HID_1:\n"
			"la x19, rand_ieee754_32\n"
			"flw.ps f9, 512(x19)\n"
			"flog.ps f7, f9\n" 
			"la x7, rand_int_32\n"
			"flw.ps f26, 448(x7)\n"
			"fandi.pi f16, f26, 0x199\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f29, 448(x14)\n"
			"flw.ps f11, 352(x14)\n"
			"fmax.ps f6, f29, f11\n" 
			"li x14, 0xb0e2e94b6aa4a2f5\n"
			"fcvt.s.w f3, x14, rdn\n" 
			"maskpopc x19, m3\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f4, 608(x10)\n"
			"flw.ps f13, 576(x10)\n"
			"fmax.s f6, f4, f13\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f25, 864(x28)\n"
			"flw.ps f11, 256(x28)\n"
			"fsgnjn.ps f2, f25, f11\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f7, 544(x27)\n"
			"flw.ps f11, 960(x27)\n"
			"fsgnj.s f1, f7, f11\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f15, 64(x13)\n"
			"flw.ps f3, 800(x13)\n"
			"fdiv.ps f22, f15, f3, rdn\n" 
			"la x10, rand_int_32\n"
			"flw.ps f31, 928(x10)\n"
			"fsatu8.pi f5, f31\n" 
			"LBL_SEQID_1_M0_WRITE_ID_4_HID_1:\n"
			"masknot m2, m4\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x6, rand_ieee754_32\n"
			"flw.ps f24, 736(x6)\n"
			"flw.ps f1, 704(x6)\n"
			"flw.ps f3, 800(x6)\n"
			"fmadd.s f9, f24, f1, f3, rup\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f4, 704(x5)\n"
			"fcvt.pwu.ps f28, f4, rne\n" 
			"la x27, rand_int_32\n"
			"flw.ps f21, 288(x27)\n"
			"fnot.pi f7, f21\n" 
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
			"LBL_HPM_3_CORE_MASK_OPS_FAIL_HID_1:\n"          
			"csrr x5, mhpmcounter3\n"
			"sd x5, 32 (x31)\n"
			"LBL_HPM_4_CORE_L2_MISS_REQ_REJ_FAIL_HID_1:\n"          
			"csrr x5, mhpmcounter4\n"
			"sd x5, 40 (x31)\n"
			"LBL_HPM_5_CORE_L2_MISS_REQ_FAIL_HID_1:\n"          
			"csrr x5, mhpmcounter5\n"
			"sd x5, 48 (x31)\n"
			"LBL_HPM_6_CORE_DCACHE_ACCESS0_FAIL_HID_1:\n"          
			"csrr x5, mhpmcounter6\n"
			"sd x5, 56 (x31)\n"
			"LBL_HPM_7_NEIGH_INTER_CORE_MSG_TX_FAIL_HID_1:\n"          
			"csrr x5, mhpmcounter7\n"
			"sd x5, 64 (x31)\n"
			"LBL_HPM_8_NEIGH_ANY_CORE_TX_ETL_REQ_FAIL_HID_1:\n"          
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
			"LBL_HPM_3_CORE_MASK_OPS_PASS_HID_1:\n"          
			"csrr x5, mhpmcounter3\n"
			"sd x5, 32 (x31)\n"
			"LBL_HPM_4_CORE_L2_MISS_REQ_REJ_PASS_HID_1:\n"          
			"csrr x5, mhpmcounter4\n"
			"sd x5, 40 (x31)\n"
			"LBL_HPM_5_CORE_L2_MISS_REQ_PASS_HID_1:\n"          
			"csrr x5, mhpmcounter5\n"
			"sd x5, 48 (x31)\n"
			"LBL_HPM_6_CORE_DCACHE_ACCESS0_PASS_HID_1:\n"          
			"csrr x5, mhpmcounter6\n"
			"sd x5, 56 (x31)\n"
			"LBL_HPM_7_NEIGH_INTER_CORE_MSG_TX_PASS_HID_1:\n"          
			"csrr x5, mhpmcounter7\n"
			"sd x5, 64 (x31)\n"
			"LBL_HPM_8_NEIGH_ANY_CORE_TX_ETL_REQ_PASS_HID_1:\n"          
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
			"csrwi mhpmevent4, 26\n"   
			"csrwi mhpmevent5, 27\n"   
			"csrwi mhpmevent6, 19\n"   
			"csrwi mhpmevent7, 5\n"   
			"csrwi mhpmevent8, 4\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f15,  672 (x5)\n"
			"flw.ps f17,  832 (x5)\n"
			"flw.ps f18,  864 (x5)\n"
			"flw.ps f31,  96 (x5)\n"
			"flw.ps f12,  288 (x5)\n"
			"flw.ps f9,  576 (x5)\n"
			"flw.ps f11,  768 (x5)\n"
			"flw.ps f26,  160 (x5)\n"
			"flw.ps f4,  416 (x5)\n"
			"flw.ps f20,  544 (x5)\n"
			"flw.ps f10,  704 (x5)\n"
			"flw.ps f19,  480 (x5)\n"
			"flw.ps f21,  224 (x5)\n"
			"flw.ps f7,  896 (x5)\n"
			"flw.ps f14,  736 (x5)\n"
			"flw.ps f23,  192 (x5)\n"
			"flw.ps f24,  352 (x5)\n"
			"flw.ps f5,  512 (x5)\n"
			"flw.ps f13,  320 (x5)\n"
			"flw.ps f3,  960 (x5)\n"
			"flw.ps f1,  128 (x5)\n"
			"flw.ps f8,  384 (x5)\n"
			"flw.ps f22,  928 (x5)\n"
			"flw.ps f16,  448 (x5)\n"
			"flw.ps f30,  608 (x5)\n"
			"flw.ps f0,  256 (x5)\n"
			"flw.ps f6,  640 (x5)\n"
			"flw.ps f2,  800 (x5)\n"
			"flw.ps f27,  64 (x5)\n"
			"flw.ps f29,  992 (x5)\n"
			"flw.ps f25,  32 (x5)\n"
			"flw.ps f28,  0 (x5)\n"
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
			"la x15, rand_ieee754_32\n"
			"flw.ps f31, 576(x15)\n"
			"flw.ps f4, 928(x15)\n"
			"fmin.s f17, f31, f4\n" 
			"la x9, rand_int_32\n"
			"flw.ps f11, 416(x9)\n"
			"fnot.pi f3, f11\n" 
			"la x25, rand_int_32\n"
			"flw.ps f28, 928(x25)\n"
			"flw.ps f15, 576(x25)\n"
			"fminu.pi f13, f28, f15\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f17, 992(x28)\n"
			"flw.ps f6, 32(x28)\n"
			"fadd.s f3, f17, f6, rtz\n" 
			"la x5, rand_int_32\n"
			"flw.ps f21, 288(x5)\n"
			"fcvt.ps.pw f20, f21, rup\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f0, 960(x12)\n"
			"flw.ps f9, 288(x12)\n"
			"fadd.s f28, f0, f9, rtz\n" 
			"li x10, 0x35924d39e64f8a8e\n"
			"fcvt.s.l f4, x10, rmm\n" 
			"LBL_SEQID_0_FP_TRANS_ID_0_HID_2:\n"
			"la x12, rand_ieee754_32\n"
			"flw.ps f30, 896(x12)\n"
			"frcp.ps f23, f30\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f23, 672(x20)\n"
			"fcvt.f16.ps f14, f23\n" 
			"LBL_SEQID_0_FP_TRANS_ID_1_HID_2:\n"
			"la x15, rand_ieee754_32\n"
			"flw.ps f26, 992(x15)\n"
			"flog.ps f27, f26\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f14, 640(x19)\n"
			"fsin.ps f11, f14\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f14, 896(x6)\n"
			"fclass.s x18, f14\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f27, 992(x6)\n"
			"flw.ps f4, 128(x6)\n"
			"feq.ps f2, f27, f4\n" 
			"la x19, rand_int_32\n"
			"flw.ps f9, 736(x19)\n"
			"flw.ps f22, 800(x19)\n"
			"fxor.pi f22, f9, f22\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f27, 544(x28)\n"
			"flw.ps f21, 768(x28)\n"
			"feq.ps f4, f27, f21\n" 
			"la x5, rand_int_32\n"
			"flw.ps f26, 768(x5)\n"
			"fpackrepb.pi f27, f26\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_0_RDX_ID_0_SHIRE_0_HID_2:\n"
			"li x15, 0x320000000741000a\n"
			"csrw tensor_reduce, x15\n" 
			"csrwi tensor_wait, 9\n" 
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=0); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ITER_0_HID_2:\n"
			"la x31, self_check_16\n"
			"li x5, 0xa0000000000007\n"
			"add x5, x5, x31\n"
			"li x6, 0x10000000000005\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x2 << TENSOR_FMA16_B_NUM_COLS) | (0x7 << TENSOR_FMA16_A_NUM_ROWS) | (0x5 << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x1 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1c << TENSOR_FMA16_SCP_LOC_B) | (0x5 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0xc\n"
			"bne x5, x31, LBL_FAIL_HID_2\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_2\n"
			"csrw tensor_error, x0\n"
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_2\n"
			"addi x30, x30, 1\n"
			"la x11, rand_ieee754_32\n"
			"flw.ps f7, 448(x11)\n"
			"flw.ps f20, 352(x11)\n"
			"fsgnjn.s f4, f7, f20\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f20, 800(x10)\n"
			"flw.ps f6, 576(x10)\n"
			"flw.ps f2, 128(x10)\n"
			"fnmsub.s f13, f20, f6, f2, rtz\n" 
			"la x15, rand_int_32\n"
			"flw.ps f2, 128(x15)\n"
			"fsrai.pi f26, f2, 0x5\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x6, rand_ieee754_32\n"
			"flw.ps f23, 0(x6)\n"
			"flw.ps f18, 288(x6)\n"
			"fadd.ps f5, f23, f18, rdn\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f15, 448(x19)\n"
			"flw.ps f29, 128(x19)\n"
			"fmin.ps f13, f15, f29\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f16, 896(x15)\n"
			"flw.ps f23, 640(x15)\n"
			"fmin.ps f15, f16, f23\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f27, 192(x28)\n"
			"flw.ps f13, 992(x28)\n"
			"flw.ps f31, 224(x28)\n"
			"fnmsub.s f1, f27, f13, f31, rne\n" 
			"la x9, rand_int_32\n"
			"flw.ps f5, 448(x9)\n"
			"fcvt.ps.pw f18, f5, rup\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f19, 640(x9)\n"
			"fsin.ps f14, f19\n" 
			"la x18, rand_int_32\n"
			"flw.ps f15, 736(x18)\n"
			"fsrai.pi f3, f15, 0xc\n" 
			"la x15, rand_int_32\n"
			"flw.ps f22, 576(x15)\n"
			"fpackrepb.pi f8, f22\n" 
			"li x5, 0x55e786b6466bad94\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0xa579ae9203ec8fe2\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"la x6, rand_ieee754_32\n"
			"flw.ps f14, 480(x6)\n"
			"ffrc.ps f29, f14\n" 
			"LBL_SEQID_1_FP_TRANS_ID_2_HID_2:\n"
			"la x15, rand_ieee754_32\n"
			"flw.ps f6, 832(x15)\n"
			"frcp.ps f9, f6\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f23, 480(x28)\n"
			"flw.ps f22, 320(x28)\n"
			"fmax.ps f22, f23, f22\n" 
			"LBL_SEQID_1_M0_WRITE_ID_3_HID_2:\n"
			"mova.m.x x5\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0x30b4683cd1\n"
			"flw.ps f5, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x6, rand_int_32\n"
			"flw.ps f0, 544(x6)\n"
			"flw.ps f25, 768(x6)\n"
			"feq.pi f12, f0, f25\n" 
			"csrw tensor_error, zero\n" 
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=0); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"LBL_SEQID_1_TLD_XMA_STR_SEQ_ITER_0_HID_2:\n"
			"la x31, self_check_16\n"
			"li x5, 0x6000000000000e\n"
			"add x5, x5, x31\n"
			"li x6, 0x36000000000000d\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x1 << TENSOR_FMA16_B_NUM_COLS) | (0xe << TENSOR_FMA16_A_NUM_ROWS) | (0xd << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1b << TENSOR_FMA16_SCP_LOC_B) | (0x3 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0x1c\n"
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
			"csrwi mhpmevent3, 25\n"   
			"csrwi mhpmevent4, 11\n"   
			"csrwi mhpmevent5, 10\n"   
			"csrwi mhpmevent6, 6\n"   
			"csrwi mhpmevent7, 12\n"   
			"csrwi mhpmevent8, 1\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f21,  384 (x5)\n"
			"flw.ps f27,  448 (x5)\n"
			"flw.ps f17,  672 (x5)\n"
			"flw.ps f5,  640 (x5)\n"
			"flw.ps f12,  768 (x5)\n"
			"flw.ps f22,  832 (x5)\n"
			"flw.ps f19,  416 (x5)\n"
			"flw.ps f31,  160 (x5)\n"
			"flw.ps f10,  896 (x5)\n"
			"flw.ps f28,  256 (x5)\n"
			"flw.ps f13,  288 (x5)\n"
			"flw.ps f1,  960 (x5)\n"
			"flw.ps f30,  544 (x5)\n"
			"flw.ps f15,  320 (x5)\n"
			"flw.ps f20,  224 (x5)\n"
			"flw.ps f0,  992 (x5)\n"
			"flw.ps f26,  352 (x5)\n"
			"flw.ps f14,  96 (x5)\n"
			"flw.ps f3,  0 (x5)\n"
			"flw.ps f7,  480 (x5)\n"
			"flw.ps f6,  736 (x5)\n"
			"flw.ps f24,  864 (x5)\n"
			"flw.ps f11,  576 (x5)\n"
			"flw.ps f9,  928 (x5)\n"
			"flw.ps f16,  704 (x5)\n"
			"flw.ps f8,  608 (x5)\n"
			"flw.ps f23,  800 (x5)\n"
			"flw.ps f18,  192 (x5)\n"
			"flw.ps f4,  32 (x5)\n"
			"flw.ps f2,  128 (x5)\n"
			"flw.ps f29,  512 (x5)\n"
			"flw.ps f25,  64 (x5)\n"
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
			"la x17, rand_int_32\n"
			"flw.ps f28, 608(x17)\n"
			"faddi.pi f26, f28, 0xfc\n" 
			"la x5, rand_int_32\n"
			"flw.ps f26, 256(x5)\n"
			"flw.ps f12, 192(x5)\n"
			"fltu.pi f12, f26, f12\n" 
			"la x20, rand_int_32\n"
			"flw.ps f6, 192(x20)\n"
			"fnot.pi f29, f6\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f14, 256(x23)\n"
			"flw.ps f12, 448(x23)\n"
			"fdiv.s f28, f14, f12, rne\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f9, 224(x27)\n"
			"ffrc.ps f3, f9\n" 
			"LBL_SEQID_0_FP_TRANS_ID_0_HID_3:\n"
			"la x5, rand_ieee754_32\n"
			"flw.ps f14, 448(x5)\n"
			"frcp.ps f11, f14\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f2, 256(x27)\n"
			"flw.ps f11, 576(x27)\n"
			"flt.s x26, f2, f11\n" 
			"LBL_SEQID_0_FP_TRANS_ID_1_HID_3:\n"
			"la x16, rand_ieee754_32\n"
			"flw.ps f5, 768(x16)\n"
			"flog.ps f13, f5\n" 
			"la x17, rand_ieee754_16\n"
			"flw.ps f8, 992(x17)\n"
			"fcvt.ps.f16 f26, f8\n" 
			"la x26, rand_int_32\n"
			"flw.ps f26, 352(x26)\n"
			"flw.ps f11, 608(x26)\n"
			"fsrl.pi f12, f26, f11\n" 
			"fbci.pi f14, 0x4f7e7\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f18, 736(x16)\n"
			"flw.ps f19, 224(x16)\n"
			"fmin.ps f25, f18, f19\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f26, 928(x22)\n"
			"flw.ps f28, 320(x22)\n"
			"fsgnjn.s f21, f26, f28\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f0, 512(x9)\n"
			"fswizz.ps f29, f0, 0x19\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f29, 352(x25)\n"
			"fround.ps f31, f29, rne\n" 
			"la x5, rand_int_32\n"
			"flw.ps f20, 576(x5)\n"
			"flw.ps f28, 576(x5)\n"
			"fadd.pi f23, f20, f28\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_3\n"
			"addi x30, x30, 1\n"
			"la x6, rand_ieee754_32\n"
			"flw.ps f6, 544(x6)\n"
			"fmin.s f2, f6, f6\n" 
			"li x22, 0x52a911b81d6c24c0\n"
			"fcvt.s.w f25, x22, rne\n" 
			"la x17, rand_int_32\n"
			"flw.ps f12, 992(x17)\n"
			"fnot.pi f17, f12\n" 
			"la x14, rand_int_32\n"
			"flw.ps f1, 640(x14)\n"
			"fsrli.pi f6, f1, 0x9\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f14, 992(x7)\n"
			"flw.ps f5, 576(x7)\n"
			"flw.ps f20, 384(x7)\n"
			"fmsub.ps f31, f14, f5, f20, rmm\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f24, 384(x25)\n"
			"flw.ps f19, 928(x25)\n"
			"fadd.ps f29, f24, f19, rne\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f29, 448(x10)\n"
			"flw.ps f3, 800(x10)\n"
			"flw.ps f22, 640(x10)\n"
			"fnmadd.ps f24, f29, f3, f22, rup\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f20, 192(x7)\n"
			"fswizz.ps f25, f20, 0x1a\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f10, 864(x9)\n"
			"flw.ps f15, 832(x9)\n"
			"feq.ps f28, f10, f15\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f0, 256(x28)\n"
			"flw.ps f26, 352(x28)\n"
			"flw.ps f16, 896(x28)\n"
			"fnmsub.s f21, f0, f26, f16, rtz\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f2, 0(x19)\n"
			"flw.ps f11, 896(x19)\n"
			"fmax.s f27, f2, f11\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f13, 704(x7)\n"
			"flw.ps f27, 160(x7)\n"
			"fnmadd.ps f14, f13, f27, f27, rne\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f28, 704(x11)\n"
			"fcvt.f16.ps f20, f28\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f13, 448(x23)\n"
			"fsgnjn.ps f9, f13, f13\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f14, 224(x26)\n"
			"flw.ps f13, 576(x26)\n"
			"fcmovm.ps f9, f14, f13\n" 
			"la x10, rand_int_32\n"
			"flw.ps f29, 992(x10)\n"
			"flw.ps f24, 288(x10)\n"
			"fsll.pi f19, f29, f24\n" 
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
			"csrwi mhpmevent3, 8\n"   
			"csrwi mhpmevent4, 26\n"   
			"csrwi mhpmevent5, 27\n"   
			"csrwi mhpmevent6, 19\n"   
			"csrwi mhpmevent7, 5\n"   
			"csrwi mhpmevent8, 4\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f13,  544 (x5)\n"
			"flw.ps f14,  288 (x5)\n"
			"flw.ps f29,  128 (x5)\n"
			"flw.ps f22,  192 (x5)\n"
			"flw.ps f4,  224 (x5)\n"
			"flw.ps f27,  672 (x5)\n"
			"flw.ps f28,  64 (x5)\n"
			"flw.ps f30,  416 (x5)\n"
			"flw.ps f24,  704 (x5)\n"
			"flw.ps f23,  384 (x5)\n"
			"flw.ps f0,  800 (x5)\n"
			"flw.ps f19,  832 (x5)\n"
			"flw.ps f26,  576 (x5)\n"
			"flw.ps f11,  896 (x5)\n"
			"flw.ps f3,  352 (x5)\n"
			"flw.ps f2,  928 (x5)\n"
			"flw.ps f5,  640 (x5)\n"
			"flw.ps f21,  608 (x5)\n"
			"flw.ps f12,  160 (x5)\n"
			"flw.ps f10,  512 (x5)\n"
			"flw.ps f18,  768 (x5)\n"
			"flw.ps f20,  448 (x5)\n"
			"flw.ps f16,  256 (x5)\n"
			"flw.ps f9,  736 (x5)\n"
			"flw.ps f25,  32 (x5)\n"
			"flw.ps f8,  992 (x5)\n"
			"flw.ps f17,  960 (x5)\n"
			"flw.ps f1,  0 (x5)\n"
			"flw.ps f31,  320 (x5)\n"
			"flw.ps f7,  864 (x5)\n"
			"flw.ps f15,  480 (x5)\n"
			"flw.ps f6,  96 (x5)\n"
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
			"la x28, rand_ieee754_32\n"
			"flw.ps f3, 224(x28)\n"
			"flw.ps f27, 768(x28)\n"
			"fle.ps f24, f3, f27\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f13, 128(x26)\n"
			"fcvt.wu.s x11, f13, rup\n" 
			"li x5, 0x8301\n"
			"csrw tensor_mask, x5\n"	
			"la x5, rand_int_32\n"
			"flw.ps f23, 960(x5)\n"
			"flw.ps f19, 864(x5)\n"
			"fmulhu.pi f11, f23, f19\n" 
			"la x12, rand_int_32\n"
			"flw.ps f20, 576(x12)\n"
			"flw.ps f8, 960(x12)\n"
			"fmax.pi f31, f20, f8\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x6, rand_int_32\n"
			"flw.ps f10, 672(x6)\n"
			"flw.ps f4, 512(x6)\n"
			"fmulhu.pi f1, f10, f4\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f7, 384(x25)\n"
			"flw.ps f19, 768(x25)\n"
			"fmax.s f9, f7, f19\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f2, 32(x13)\n"
			"flw.ps f14, 160(x13)\n"
			"flw.ps f4, 160(x13)\n"
			"fnmadd.ps f8, f2, f14, f4, rup\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f14, 320(x28)\n"
			"flw.ps f8, 32(x28)\n"
			"fadd.s f8, f14, f8, rtz\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_4:\n"
			"maskxor m3, m1, m1\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x14, rand_int_32\n"
			"flw.ps f7, 544(x14)\n"
			"fsat8.pi f5, f7\n" 
			"la x18, rand_int_32\n"
			"flw.ps f28, 704(x18)\n"
			"fsrai.pi f0, f28, 0x1\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f26, 64(x7)\n"
			"fcvt.f16.ps f29, f26\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f19, 448(x17)\n"
			"fcvt.pw.ps f1, f19, rup\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f29, 768(x27)\n"
			"flw.ps f0, 864(x27)\n"
			"feq.ps f23, f29, f0\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f13, 416(x17)\n"
			"ffrc.ps f29, f13\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f21, 448(x28)\n"
			"flw.ps f6, 736(x28)\n"
			"fmul.s f3, f21, f6, rdn\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_0_RDX_ID_0_SHIRE_0_HID_4:\n"
			"li x9, 0x320000000741000a\n"
			"csrw tensor_reduce, x9\n" 
			"csrwi tensor_wait, 9\n" 
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ID_154010_HID_4:\n"
			"li x6, ((0x9 << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0x8 << TENSOR_QUANT_QUANT_ROW) | (0x39 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x3 << TENSOR_QUANT_TRANSF1) | (0x4 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ITER_0_HID_4:\n"
			"li x6, ((0xf << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0x5 << TENSOR_QUANT_QUANT_ROW) | (0x13 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x1 << TENSOR_QUANT_TRANSF3) | (0x8 << TENSOR_QUANT_TRANSF2) | (0x8 << TENSOR_QUANT_TRANSF1) | (0x5 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_4\n"
			"csrw tensor_error, x0\n"
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_4\n"
			"addi x30, x30, 1\n"
			"la x5, rand_ieee754_32\n"
			"flw.ps f24, 864(x5)\n"
			"flw.ps f3, 928(x5)\n"
			"fmax.ps f31, f24, f3\n" 
			"LBL_SEQID_1_M0_WRITE_ID_1_HID_4:\n"
			"fsetm.pi m2, f3\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x5, rand_ieee754_32\n"
			"flw.ps f25, 160(x5)\n"
			"fcvt.pwu.ps f30, f25, rup\n" 
			"la x9, rand_int_32\n"
			"flw.ps f17, 640(x9)\n"
			"fsatu8.pi f17, f17\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x18, rand_int_32\n"
			"flw.ps f3, 608(x18)\n"
			"flw.ps f7, 544(x18)\n"
			"fmaxu.pi f20, f3, f7\n" 
			"la x11, rand_int_32\n"
			"flw.ps f11, 288(x11)\n"
			"fsatu8.pi f11, f11\n" 
			"LBL_SEQID_1_M0_WRITE_ID_2_HID_4:\n"
			"fltm.ps m4, f12, f3\n" 
			"li x5, 0x803401D0\n"
			"ld x5, 0(x5)\n"
			"LBL_SEQID_1_M0_WRITE_ID_3_HID_4:\n"
			"masknot m3, m6\n" 
			"li x5, CREDINC1\n"
			"ld x5, 0(x5)\n"
			"la x20, rand_int_32\n"
			"flw.ps f15, 544(x20)\n"
			"fcvt.ps.pwu f7, f15, rne\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x20, rand_ieee754_32\n"
			"flw.ps f17, 960(x20)\n"
			"flw.ps f21, 256(x20)\n"
			"fmax.ps f1, f17, f21\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f20, 416(x16)\n"
			"frsq.ps f23, f20\n" 
			"la x28, rand_int_32\n"
			"flw.ps f21, 480(x28)\n"
			"flw.ps f30, 160(x28)\n"
			"for.pi f12, f21, f30\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f24, 864(x23)\n"
			"flw.ps f3, 896(x23)\n"
			"fadd.ps f24, f24, f3, rne\n" 
			"la x17, rand_int_32\n"
			"flw.ps f25, 128(x17)\n"
			"fcvt.ps.pwu f22, f25, rne\n" 
			"la x23, rand_int_32\n"
			"flw.ps f20, 512(x23)\n"
			"flw.ps f10, 928(x23)\n"
			"fmulh.pi f15, f20, f10\n" 
			"LBL_SEQID_1_M0_WRITE_ID_4_HID_4:\n"
			"feqm.ps m6, f8, f6\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"csrw tensor_error, zero\n" 
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=0); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ITER_0_HID_4:\n"
			"li x5, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x1 << TENSOR_FMA16_B_NUM_COLS) | (0x0 << TENSOR_FMA16_A_NUM_ROWS) | (0x7 << TENSOR_FMA16_A_NUM_COLS) | (0x3 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0xf6 << TENSOR_FMA16_SCP_LOC_B) | (0x76 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
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
			"csrwi mhpmevent3, 25\n"   
			"csrwi mhpmevent4, 11\n"   
			"csrwi mhpmevent5, 10\n"   
			"csrwi mhpmevent6, 6\n"   
			"csrwi mhpmevent7, 12\n"   
			"csrwi mhpmevent8, 1\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f21,  960 (x5)\n"
			"flw.ps f29,  128 (x5)\n"
			"flw.ps f23,  160 (x5)\n"
			"flw.ps f27,  800 (x5)\n"
			"flw.ps f4,  544 (x5)\n"
			"flw.ps f31,  64 (x5)\n"
			"flw.ps f25,  832 (x5)\n"
			"flw.ps f14,  864 (x5)\n"
			"flw.ps f1,  576 (x5)\n"
			"flw.ps f22,  640 (x5)\n"
			"flw.ps f3,  256 (x5)\n"
			"flw.ps f16,  768 (x5)\n"
			"flw.ps f7,  704 (x5)\n"
			"flw.ps f17,  96 (x5)\n"
			"flw.ps f19,  384 (x5)\n"
			"flw.ps f10,  416 (x5)\n"
			"flw.ps f15,  672 (x5)\n"
			"flw.ps f9,  320 (x5)\n"
			"flw.ps f26,  512 (x5)\n"
			"flw.ps f13,  736 (x5)\n"
			"flw.ps f18,  992 (x5)\n"
			"flw.ps f5,  352 (x5)\n"
			"flw.ps f30,  32 (x5)\n"
			"flw.ps f12,  608 (x5)\n"
			"flw.ps f0,  0 (x5)\n"
			"flw.ps f8,  480 (x5)\n"
			"flw.ps f2,  896 (x5)\n"
			"flw.ps f28,  448 (x5)\n"
			"flw.ps f20,  288 (x5)\n"
			"flw.ps f6,  224 (x5)\n"
			"flw.ps f24,  928 (x5)\n"
			"flw.ps f11,  192 (x5)\n"
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
			"la x25, rand_ieee754_32\n"
			"flw.ps f30, 0(x25)\n"
			"flw.ps f24, 704(x25)\n"
			"fsgnj.s f14, f30, f24\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f15, 352(x14)\n"
			"flw.ps f27, 288(x14)\n"
			"fmul.s f8, f15, f27, rup\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f16, 864(x25)\n"
			"fclass.ps f31, f16\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_5:\n"
			"mova.m.x x23\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x16, rand_ieee754_32\n"
			"flw.ps f14, 608(x16)\n"
			"flw.ps f20, 128(x16)\n"
			"feq.ps f13, f14, f20\n" 
			"fbci.pi f22, 0x3a990\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f8, 384(x17)\n"
			"flw.ps f1, 736(x17)\n"
			"fmax.s f1, f8, f1\n" 
			"la x23, rand_int_32\n"
			"flw.ps f16, 608(x23)\n"
			"fsatu8.pi f5, f16\n" 
			"la x10, rand_int_32\n"
			"flw.ps f4, 160(x10)\n"
			"fcvt.ps.pw f15, f4, rmm\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_5:\n"
			"mov.m.x m7, x9, 0x4f\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x20, rand_ieee754_32\n"
			"flw.ps f23, 704(x20)\n"
			"flw.ps f22, 704(x20)\n"
			"fsgnjx.s f17, f23, f22\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f28, 736(x25)\n"
			"flt.s x11, f28, f28\n" 
			"LBL_SEQID_0_M0_WRITE_ID_2_HID_5:\n"
			"maskxor m0, m4, m5\n" 
			"li x5, 0x80340160\n"
			"ld x5, 0(x5)\n"
			"la x18, rand_ieee754_32\n"
			"flw.ps f20, 832(x18)\n"
			"ffrc.ps f24, f20\n" 
			"la x13, rand_int_32\n"
			"flw.ps f14, 992(x13)\n"
			"flw.ps f12, 672(x13)\n"
			"fmulhu.pi f26, f14, f12\n" 
			"la x6, rand_int_32\n"
			"flw.ps f2, 160(x6)\n"
			"fnot.pi f3, f2\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_5\n"
			"addi x30, x30, 1\n"
			"la x19, rand_ieee754_32\n"
			"flw.ps f6, 96(x19)\n"
			"ffrc.ps f12, f6\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f11, 0(x20)\n"
			"flw.ps f25, 192(x20)\n"
			"flw.ps f13, 96(x20)\n"
			"fnmadd.ps f6, f11, f25, f13, rdn\n" 
			"la x14, rand_int_32\n"
			"flw.ps f17, 448(x14)\n"
			"fandi.pi f28, f17, 0x5e\n" 
			"la x6, rand_int_32\n"
			"flw.ps f21, 928(x6)\n"
			"flw.ps f23, 352(x6)\n"
			"fmax.pi f22, f21, f23\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f31, 64(x28)\n"
			"flw.ps f25, 672(x28)\n"
			"flw.ps f12, 288(x28)\n"
			"fmadd.s f13, f31, f25, f12, rdn\n" 
			"la x27, rand_int_32\n"
			"flw.ps f15, 416(x27)\n"
			"flw.ps f23, 960(x27)\n"
			"fmax.pi f26, f15, f23\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f15, 864(x28)\n"
			"flw.ps f18, 320(x28)\n"
			"fsgnjn.ps f8, f15, f18\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f23, 864(x18)\n"
			"fcvt.pw.ps f27, f23, rmm\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f30, 704(x14)\n"
			"fround.ps f31, f30, rtz\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f24, 864(x5)\n"
			"flw.ps f0, 416(x5)\n"
			"fsgnjn.ps f3, f24, f0\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f4, 896(x18)\n"
			"flw.ps f30, 0(x18)\n"
			"flw.ps f18, 832(x18)\n"
			"fmsub.s f24, f4, f30, f18, rdn\n" 
			"la x23, rand_int_32\n"
			"flw.ps f25, 864(x23)\n"
			"flw.ps f13, 992(x23)\n"
			"fmaxu.pi f11, f25, f13\n" 
			"la x19, rand_int_32\n"
			"flw.ps f0, 0(x19)\n"
			"flw.ps f17, 864(x19)\n"
			"fltu.pi f6, f0, f17\n" 
			"la x17, rand_int_32\n"
			"flw.ps f25, 64(x17)\n"
			"flw.ps f8, 224(x17)\n"
			"fmin.pi f20, f25, f8\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f21, 992(x11)\n"
			"flw.ps f29, 608(x11)\n"
			"fsgnjn.s f12, f21, f29\n" 
			"la x6, rand_int_32\n"
			"flw.ps f25, 128(x6)\n"
			"fslli.pi f21, f25, 0x0\n" 
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
			"csrwi mhpmevent3, 8\n"   
			"csrwi mhpmevent4, 26\n"   
			"csrwi mhpmevent5, 27\n"   
			"csrwi mhpmevent6, 19\n"   
			"csrwi mhpmevent7, 5\n"   
			"csrwi mhpmevent8, 4\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f29,  672 (x5)\n"
			"flw.ps f17,  800 (x5)\n"
			"flw.ps f11,  224 (x5)\n"
			"flw.ps f12,  320 (x5)\n"
			"flw.ps f18,  992 (x5)\n"
			"flw.ps f25,  192 (x5)\n"
			"flw.ps f14,  576 (x5)\n"
			"flw.ps f19,  832 (x5)\n"
			"flw.ps f21,  288 (x5)\n"
			"flw.ps f10,  64 (x5)\n"
			"flw.ps f5,  160 (x5)\n"
			"flw.ps f1,  768 (x5)\n"
			"flw.ps f23,  448 (x5)\n"
			"flw.ps f8,  960 (x5)\n"
			"flw.ps f27,  96 (x5)\n"
			"flw.ps f0,  736 (x5)\n"
			"flw.ps f31,  512 (x5)\n"
			"flw.ps f13,  928 (x5)\n"
			"flw.ps f15,  352 (x5)\n"
			"flw.ps f26,  128 (x5)\n"
			"flw.ps f20,  32 (x5)\n"
			"flw.ps f7,  640 (x5)\n"
			"flw.ps f9,  896 (x5)\n"
			"flw.ps f4,  416 (x5)\n"
			"flw.ps f30,  544 (x5)\n"
			"flw.ps f24,  704 (x5)\n"
			"flw.ps f3,  864 (x5)\n"
			"flw.ps f28,  0 (x5)\n"
			"flw.ps f2,  608 (x5)\n"
			"flw.ps f6,  480 (x5)\n"
			"flw.ps f16,  384 (x5)\n"
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
			"li x5, 0x2\n" 
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
			"la x15, rand_ieee754_32\n"
			"flw.ps f22, 96(x15)\n"
			"flw.ps f30, 448(x15)\n"
			"fle.s x27, f22, f30\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f25, 864(x6)\n"
			"fclass.ps f29, f25\n" 
			"la x27, rand_int_32\n"
			"flw.ps f13, 224(x27)\n"
			"flw.ps f11, 512(x27)\n"
			"fmulhu.pi f24, f13, f11\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f2, 576(x23)\n"
			"fcvt.w.s x18, f2, rne\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_6:\n"
			"maskand m1, m6, m3\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x18, rand_ieee754_32\n"
			"flw.ps f2, 288(x18)\n"
			"fcvt.pw.ps f29, f2, rne\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_6:\n"
			"fltm.pi m6, f31, f17\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x10, rand_int_32\n"
			"flw.ps f16, 352(x10)\n"
			"flw.ps f24, 608(x10)\n"
			"fmaxu.pi f29, f16, f24\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f29, 224(x14)\n"
			"flw.ps f8, 64(x14)\n"
			"flw.ps f16, 352(x14)\n"
			"fmadd.s f4, f29, f8, f16, rne\n" 
			"fbci.pi f18, 0x5c387\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f6, 160(x25)\n"
			"flw.ps f1, 448(x25)\n"
			"fsgnj.s f9, f6, f1\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f5, 192(x17)\n"
			"flw.ps f28, 576(x17)\n"
			"fsgnjn.s f26, f5, f28\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f3, 960(x17)\n"
			"flw.ps f5, 96(x17)\n"
			"feq.s x12, f3, f5\n" 
			"LBL_SEQID_0_M0_WRITE_ID_2_HID_6:\n"
			"flem.ps m6, f2, f0\n" 
			"li x5, CREDINC3\n"
			"ld x5, 0(x5)\n"
			"la x18, rand_int_32\n"
			"flw.ps f9, 928(x18)\n"
			"flw.ps f30, 352(x18)\n"
			"feq.pi f25, f9, f30\n" 
			"la x20, rand_int_32\n"
			"flw.ps f19, 928(x20)\n"
			"flw.ps f29, 64(x20)\n"
			"fminu.pi f31, f19, f29\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ID_916271_HID_6:\n"
			"la x31, self_check_8\n"
			"li x5, 0x4000000000000b\n"
			"add x5, x5, x31\n"
			"li x6, 0x30000000000000c\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x3 << TENSOR_IMA_B_NUM_COLS) | (0xb << TENSOR_IMA_A_NUM_ROWS) | (0xc << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x18 << TENSOR_IMA_SCP_LOC_B) | (0x2 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x9, ((0x17 << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0xb << TENSOR_QUANT_QUANT_ROW) | (0x16 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x9 << TENSOR_QUANT_TRANSF7) | (0x3 << TENSOR_QUANT_TRANSF6) | (0x8 << TENSOR_QUANT_TRANSF5) | (0xa << TENSOR_QUANT_TRANSF4) | (0x8 << TENSOR_QUANT_TRANSF3) | (0x5 << TENSOR_QUANT_TRANSF2) | (0x5 << TENSOR_QUANT_TRANSF1) | (0x9 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"bne x5, x31, LBL_FAIL_HID_6\n"
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_6\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_0_RDX_ID_0_SHIRE_0_HID_6:\n"
			"li x11, 0x320000000741000a\n"
			"csrw tensor_reduce, x11\n" 
			"csrwi tensor_wait, 9\n" 
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ID_28486_HID_6:\n"
			"li x5, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x2 << TENSOR_FMA32_B_NUM_COLS) | (0x5 << TENSOR_FMA32_A_NUM_ROWS) | (0x8 << TENSOR_FMA32_A_NUM_COLS) | (0xc << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0xe9 << TENSOR_FMA32_SCP_LOC_B) | (0xc1 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x0 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
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
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ITER_0_HID_6:\n"
			"la x31, self_check_16\n"
			"li x5, 0x40000000000000\n"
			"add x5, x5, x31\n"
			"li x6, 0x36000000000000d\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x1 << TENSOR_FMA16_B_NUM_COLS) | (0x0 << TENSOR_FMA16_A_NUM_ROWS) | (0xd << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1b << TENSOR_FMA16_SCP_LOC_B) | (0x2 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x9, ((0xc << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0x0 << TENSOR_QUANT_QUANT_ROW) | (0x22 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x7 << TENSOR_QUANT_TRANSF5) | (0x6 << TENSOR_QUANT_TRANSF4) | (0x7 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 10\n"
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ITER_1_HID_6:\n"
			"la x31, self_check_32\n"
			"li x5, 0x80000000000005\n"
			"add x5, x5, x31\n"
			"li x6, 0x10000000000009\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x0 << TENSOR_FMA32_B_NUM_COLS) | (0x5 << TENSOR_FMA32_A_NUM_ROWS) | (0x9 << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x1 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1e << TENSOR_FMA32_SCP_LOC_B) | (0x4 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0xa\n"
			"bne x5, x31, LBL_FAIL_HID_6\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_6\n"
			"csrw tensor_error, x0\n"
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_6\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_1_M0_WRITE_ID_3_HID_6:\n"
			"feqm.ps m0, f17, f12\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x13, rand_int_32\n"
			"flw.ps f4, 704(x13)\n"
			"flw.ps f13, 224(x13)\n"
			"fltu.pi f3, f4, f13\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f15, 512(x17)\n"
			"flw.ps f14, 224(x17)\n"
			"flw.ps f12, 288(x17)\n"
			"fmsub.s f6, f15, f14, f12, rdn\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f10, 96(x28)\n"
			"flw.ps f19, 768(x28)\n"
			"feq.ps f1, f10, f19\n" 
			"la x12, rand_int_32\n"
			"flw.ps f23, 896(x12)\n"
			"flw.ps f5, 256(x12)\n"
			"fltu.pi f19, f23, f5\n" 
			"la x9, rand_int_32\n"
			"flw.ps f18, 288(x9)\n"
			"flw.ps f19, 320(x9)\n"
			"fmul.pi f26, f18, f19\n" 
			"la x7, rand_int_32\n"
			"flw.ps f9, 832(x7)\n"
			"flw.ps f12, 576(x7)\n"
			"fsrl.pi f21, f9, f12\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f20, 608(x26)\n"
			"flw.ps f13, 832(x26)\n"
			"fle.ps f27, f20, f13\n" 
			"la x5, rand_int_32\n"
			"flw.ps f30, 992(x5)\n"
			"faddi.pi f18, f30, 0x1a2\n" 
			"la x15, rand_int_32\n"
			"flw.ps f26, 608(x15)\n"
			"flw.ps f8, 128(x15)\n"
			"fremu.pi f21, f26, f8\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f8, 352(x26)\n"
			"fcvt.l.s x28, f8, rdn\n" 
			"la x9, rand_int_32\n"
			"flw.ps f29, 320(x9)\n"
			"flw.ps f27, 512(x9)\n"
			"fmulhu.pi f10, f29, f27\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f19, 544(x5)\n"
			"flw.ps f25, 832(x5)\n"
			"fdiv.s f23, f19, f25, rdn\n" 
			"LBL_SEQID_1_M0_WRITE_ID_4_HID_6:\n"
			"fsetm.pi m4, f20\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x22, rand_int_32\n"
			"flw.ps f5, 128(x22)\n"
			"flw.ps f25, 640(x22)\n"
			"fdiv.pi f9, f5, f25\n" 
			"la x19, rand_int_32\n"
			"flw.ps f19, 640(x19)\n"
			"flw.ps f26, 160(x19)\n"
			"fltu.pi f0, f19, f26\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_1_RDX_ID_1_SHIRE_0_HID_6:\n"
			"li x18, 0x800000000280000\n"
			"csrr x5, mhartid\n"
			"srli x5, x5, 4\n"
			"slli x5, x5, 6\n"
			"add x18, x18, x5\n"
			"csrw tensor_reduce, x18\n" 
			"csrwi tensor_wait, 9\n" 
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=0); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"li x5, 0x444f\n"
			"csrw tensor_mask, x5\n"	
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ITER_0_HID_6:\n"
			"li x5, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x2 << TENSOR_FMA32_B_NUM_COLS) | (0xc << TENSOR_FMA32_A_NUM_ROWS) | (0x8 << TENSOR_FMA32_A_NUM_COLS) | (0x1 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x20 << TENSOR_FMA32_SCP_LOC_B) | (0xf0 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x0 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
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
			"csrwi mhpmevent3, 25\n"   
			"csrwi mhpmevent4, 11\n"   
			"csrwi mhpmevent5, 10\n"   
			"csrwi mhpmevent6, 6\n"   
			"csrwi mhpmevent7, 12\n"   
			"csrwi mhpmevent8, 1\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f9,  544 (x5)\n"
			"flw.ps f26,  96 (x5)\n"
			"flw.ps f10,  416 (x5)\n"
			"flw.ps f22,  832 (x5)\n"
			"flw.ps f0,  352 (x5)\n"
			"flw.ps f1,  704 (x5)\n"
			"flw.ps f14,  320 (x5)\n"
			"flw.ps f3,  256 (x5)\n"
			"flw.ps f11,  64 (x5)\n"
			"flw.ps f13,  160 (x5)\n"
			"flw.ps f28,  608 (x5)\n"
			"flw.ps f23,  448 (x5)\n"
			"flw.ps f31,  480 (x5)\n"
			"flw.ps f21,  672 (x5)\n"
			"flw.ps f5,  288 (x5)\n"
			"flw.ps f25,  224 (x5)\n"
			"flw.ps f8,  928 (x5)\n"
			"flw.ps f15,  192 (x5)\n"
			"flw.ps f27,  576 (x5)\n"
			"flw.ps f6,  0 (x5)\n"
			"flw.ps f12,  768 (x5)\n"
			"flw.ps f2,  32 (x5)\n"
			"flw.ps f7,  736 (x5)\n"
			"flw.ps f24,  128 (x5)\n"
			"flw.ps f20,  800 (x5)\n"
			"flw.ps f30,  512 (x5)\n"
			"flw.ps f18,  640 (x5)\n"
			"flw.ps f29,  960 (x5)\n"
			"flw.ps f4,  384 (x5)\n"
			"flw.ps f19,  896 (x5)\n"
			"flw.ps f16,  864 (x5)\n"
			"flw.ps f17,  992 (x5)\n"
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
			"la x15, rand_int_32\n"
			"flw.ps f28, 704(x15)\n"
			"fsatu8.pi f13, f28\n" 
			"la x28, rand_int_32\n"
			"flw.ps f19, 640(x28)\n"
			"flw.ps f20, 32(x28)\n"
			"for.pi f20, f19, f20\n" 
			"la x6, rand_int_32\n"
			"flw.ps f9, 288(x6)\n"
			"fcvt.ps.pw f28, f9, rmm\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f3, 64(x11)\n"
			"frsq.ps f26, f3\n" 
			"la x17, rand_int_32\n"
			"flw.ps f7, 576(x17)\n"
			"flw.ps f2, 352(x17)\n"
			"fsll.pi f6, f7, f2\n" 
			"la x5, rand_int_32\n"
			"flw.ps f13, 288(x5)\n"
			"flw.ps f23, 512(x5)\n"
			"flt.pi f17, f13, f23\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f18, 480(x9)\n"
			"fcvt.pwu.ps f11, f18, rmm\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f27, 640(x6)\n"
			"fcvt.pwu.ps f26, f27, rne\n" 
			"maskpopc x22, m6\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f18, 672(x19)\n"
			"flw.ps f27, 640(x19)\n"
			"fmul.ps f30, f18, f27, rne\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f27, 928(x27)\n"
			"flw.ps f2, 512(x27)\n"
			"fadd.ps f13, f27, f2, rup\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f6, 928(x13)\n"
			"flw.ps f1, 640(x13)\n"
			"fadd.s f22, f6, f1, rup\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_7:\n"
			"fltm.ps m4, f9, f17\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x10, rand_int_32\n"
			"flw.ps f19, 512(x10)\n"
			"flw.ps f3, 864(x10)\n"
			"fmulh.pi f16, f19, f3\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f9, 256(x5)\n"
			"fsgnjn.ps f26, f9, f9\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f0, 320(x5)\n"
			"flw.ps f13, 160(x5)\n"
			"fle.ps f24, f0, f13\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_7\n"
			"addi x30, x30, 1\n"
			"fbci.ps f26, 0x43918\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f22, 384(x22)\n"
			"flw.ps f3, 896(x22)\n"
			"fadd.s f14, f22, f3, rne\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f10, 224(x14)\n"
			"flw.ps f15, 512(x14)\n"
			"flw.ps f27, 384(x14)\n"
			"fnmsub.s f30, f10, f15, f27, rmm\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f12, 32(x15)\n"
			"ffrc.ps f12, f12\n" 
			"li x9, 0x222ae22e80601d68\n"
			"fcvt.s.wu f17, x9, rmm\n" 
			"la x7, rand_int_32\n"
			"flw.ps f16, 992(x7)\n"
			"flw.ps f1, 448(x7)\n"
			"fmul.pi f18, f16, f1\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f2, 928(x14)\n"
			"fcvt.wu.s x10, f2, rne\n" 
			"LBL_SEQID_1_M0_WRITE_ID_1_HID_7:\n"
			"mova.m.x x6\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_1_M0_WRITE_ID_2_HID_7:\n"
			"fltm.pi m4, f15, f26\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x26, rand_ieee754_32\n"
			"flw.ps f6, 0(x26)\n"
			"flw.ps f2, 416(x26)\n"
			"fmul.s f16, f6, f2, rmm\n" 
			"la x18, rand_int_32\n"
			"flw.ps f13, 64(x18)\n"
			"flw.ps f2, 0(x18)\n"
			"for.pi f29, f13, f2\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f29, 128(x19)\n"
			"flw.ps f12, 960(x19)\n"
			"flt.ps f29, f29, f12\n" 
			"maskpopc x26, m2\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f27, 864(x13)\n"
			"fclass.ps f14, f27\n" 
			"la x14, rand_int_32\n"
			"flw.ps f31, 544(x14)\n"
			"fcvt.ps.pw f24, f31, rdn\n" 
			"la x23, rand_int_32\n"
			"flw.ps f7, 672(x23)\n"
			"fnot.pi f17, f7\n" 
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
			"csrwi mhpmevent3, 8\n"   
			"csrwi mhpmevent4, 26\n"   
			"csrwi mhpmevent5, 27\n"   
			"csrwi mhpmevent6, 19\n"   
			"csrwi mhpmevent7, 5\n"   
			"csrwi mhpmevent8, 4\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f16,  288 (x5)\n"
			"flw.ps f24,  736 (x5)\n"
			"flw.ps f4,  608 (x5)\n"
			"flw.ps f6,  0 (x5)\n"
			"flw.ps f14,  32 (x5)\n"
			"flw.ps f15,  544 (x5)\n"
			"flw.ps f26,  256 (x5)\n"
			"flw.ps f13,  576 (x5)\n"
			"flw.ps f23,  800 (x5)\n"
			"flw.ps f11,  768 (x5)\n"
			"flw.ps f20,  448 (x5)\n"
			"flw.ps f31,  352 (x5)\n"
			"flw.ps f22,  192 (x5)\n"
			"flw.ps f12,  928 (x5)\n"
			"flw.ps f1,  480 (x5)\n"
			"flw.ps f10,  128 (x5)\n"
			"flw.ps f3,  512 (x5)\n"
			"flw.ps f0,  384 (x5)\n"
			"flw.ps f7,  864 (x5)\n"
			"flw.ps f5,  320 (x5)\n"
			"flw.ps f21,  832 (x5)\n"
			"flw.ps f27,  672 (x5)\n"
			"flw.ps f2,  160 (x5)\n"
			"flw.ps f30,  640 (x5)\n"
			"flw.ps f9,  960 (x5)\n"
			"flw.ps f17,  704 (x5)\n"
			"flw.ps f18,  896 (x5)\n"
			"flw.ps f8,  224 (x5)\n"
			"flw.ps f29,  416 (x5)\n"
			"flw.ps f28,  96 (x5)\n"
			"flw.ps f19,  992 (x5)\n"
			"flw.ps f25,  64 (x5)\n"
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
			"la x20, rand_int_32\n"
			"flw.ps f25, 800(x20)\n"
			"flw.ps f1, 320(x20)\n"
			"fand.pi f16, f25, f1\n" 
			"li x5, 0xb1d5\n"
			"csrw tensor_mask, x5\n"	
			"la x13, rand_int_32\n"
			"flw.ps f18, 0(x13)\n"
			"fsat8.pi f1, f18\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f2, 640(x20)\n"
			"flw.ps f13, 256(x20)\n"
			"flw.ps f17, 320(x20)\n"
			"fnmsub.ps f21, f2, f13, f17, rdn\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f21, 32(x6)\n"
			"flw.ps f8, 160(x6)\n"
			"fsub.ps f22, f21, f8, rne\n" 
			"la x19, rand_int_32\n"
			"flw.ps f18, 480(x19)\n"
			"flw.ps f31, 288(x19)\n"
			"fmin.pi f19, f18, f31\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f0, 992(x26)\n"
			"flw.ps f20, 320(x26)\n"
			"flw.ps f26, 736(x26)\n"
			"fmadd.ps f20, f0, f20, f26, rdn\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x13, rand_ieee754_32\n"
			"flw.ps f19, 928(x13)\n"
			"flw.ps f10, 864(x13)\n"
			"fsgnjn.ps f17, f19, f10\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f16, 0(x7)\n"
			"flw.ps f30, 0(x7)\n"
			"flw.ps f28, 448(x7)\n"
			"fnmadd.s f28, f16, f30, f28, rmm\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f27, 640(x7)\n"
			"flw.ps f25, 672(x7)\n"
			"flw.ps f18, 544(x7)\n"
			"fmsub.s f16, f27, f25, f18, rup\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_8:\n"
			"mov.m.x m1, x10, 0x17\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0xe36c428069\n"
			"flw.ps f5, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x6, rand_int_32\n"
			"flw.ps f6, 672(x6)\n"
			"flw.ps f22, 480(x6)\n"
			"fand.pi f2, f6, f22\n" 
			"fbci.ps f15, 0x66e6a\n" 
			"li x14, 0x3b453af8167c41a3\n"
			"fcvt.s.wu f25, x14, rmm\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_8:\n"
			"mov.m.x m6, x15, 0x53\n" 
			"li x5, CREDINC1\n"
			"ld x5, 0(x5)\n"
			"la x14, rand_int_32\n"
			"flw.ps f29, 832(x14)\n"
			"flw.ps f6, 640(x14)\n"
			"feq.pi f31, f29, f6\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f0, 384(x14)\n"
			"fswizz.ps f8, f0, 0x4\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_0_RDX_ID_0_SHIRE_0_HID_8:\n"
			"li x14, 0x320000000741000a\n"
			"csrw tensor_reduce, x14\n" 
			"csrwi tensor_wait, 9\n" 
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ID_889976_HID_8:\n"
			"li x5, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x1 << TENSOR_IMA_B_NUM_COLS) | (0x3 << TENSOR_IMA_A_NUM_ROWS) | (0x2 << TENSOR_IMA_A_NUM_COLS) | (0x6 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x35 << TENSOR_IMA_SCP_LOC_B) | (0x99 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x6, ((0x14 << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0x3 << TENSOR_QUANT_QUANT_ROW) | (0x28 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x1 << TENSOR_QUANT_TRANSF6) | (0x8 << TENSOR_QUANT_TRANSF5) | (0x3 << TENSOR_QUANT_TRANSF4) | (0xa << TENSOR_QUANT_TRANSF3) | (0xa << TENSOR_QUANT_TRANSF2) | (0x3 << TENSOR_QUANT_TRANSF1) | (0xa << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 7\n"
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
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ITER_0_HID_8:\n"
			"li x5, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x1 << TENSOR_FMA16_B_NUM_COLS) | (0xd << TENSOR_FMA16_A_NUM_ROWS) | (0xb << TENSOR_FMA16_A_NUM_COLS) | (0xe << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0xbf << TENSOR_FMA16_SCP_LOC_B) | (0x94 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_8\n"
			"csrw tensor_error, x0\n"
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_8\n"
			"addi x30, x30, 1\n"
			"maskpopcz x13, m1\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f20, 736(x17)\n"
			"fswizz.ps f20, f20, 0xf\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f30, 960(x16)\n"
			"frsq.ps f9, f30\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x12, rand_int_32\n"
			"flw.ps f9, 672(x12)\n"
			"fpackreph.pi f7, f9\n" 
			"la x25, rand_int_32\n"
			"flw.ps f5, 608(x25)\n"
			"fsrai.pi f15, f5, 0xf\n" 
			"la x27, rand_int_32\n"
			"flw.ps f2, 224(x27)\n"
			"flw.ps f12, 576(x27)\n"
			"fand.pi f22, f2, f12\n" 
			"la x13, rand_int_32\n"
			"flw.ps f27, 736(x13)\n"
			"flw.ps f11, 768(x13)\n"
			"fadd.pi f24, f27, f11\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f10, 576(x27)\n"
			"flw.ps f0, 32(x27)\n"
			"fsgnjn.ps f13, f10, f0\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f0, 384(x23)\n"
			"flw.ps f4, 224(x23)\n"
			"fsgnj.s f24, f0, f4\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f9, 96(x13)\n"
			"ffrc.ps f1, f9\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f10, 608(x13)\n"
			"fcvt.pwu.ps f9, f10, rup\n" 
			"la x25, rand_int_32\n"
			"flw.ps f29, 0(x25)\n"
			"flw.ps f2, 0(x25)\n"
			"for.pi f17, f29, f2\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f8, 288(x22)\n"
			"flw.ps f17, 928(x22)\n"
			"fadd.ps f17, f8, f17, rdn\n" 
			"LBL_SEQID_1_M0_WRITE_ID_2_HID_8:\n"
			"maskand m6, m6, m0\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0xFF\n"
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x28, rand_ieee754_32\n"
			"flw.ps f5, 896(x28)\n"
			"fcvt.pw.ps f8, f5, rdn\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f14, 864(x25)\n"
			"fclass.s x11, f14\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_1_TLD_XMA_STR_SEQ_ID_493532_HID_8:\n"
			"la x31, self_check_32\n"
			"li x5, 0xc000000000000a\n"
			"add x5, x5, x31\n"
			"li x6, 0x3a0000000000001\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x1 << TENSOR_FMA32_B_NUM_COLS) | (0xa << TENSOR_FMA32_A_NUM_ROWS) | (0x1 << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1d << TENSOR_FMA32_SCP_LOC_B) | (0x6 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x9 << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0xa << TENSOR_QUANT_QUANT_ROW) | (0x4 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x2 << TENSOR_QUANT_TRANSF9) | (0x7 << TENSOR_QUANT_TRANSF8) | (0x6 << TENSOR_QUANT_TRANSF7) | (0x6 << TENSOR_QUANT_TRANSF6) | (0x6 << TENSOR_QUANT_TRANSF5) | (0x6 << TENSOR_QUANT_TRANSF4) | (0x7 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ITER_0_HID_8:\n"
			"li x6, ((0x11 << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0x2 << TENSOR_QUANT_QUANT_ROW) | (0x26 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x1 << TENSOR_QUANT_TRANSF2) | (0x8 << TENSOR_QUANT_TRANSF1) | (0x3 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
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
			"csrwi mhpmevent3, 25\n"   
			"csrwi mhpmevent4, 11\n"   
			"csrwi mhpmevent5, 10\n"   
			"csrwi mhpmevent6, 6\n"   
			"csrwi mhpmevent7, 12\n"   
			"csrwi mhpmevent8, 1\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f13,  832 (x5)\n"
			"flw.ps f18,  416 (x5)\n"
			"flw.ps f10,  128 (x5)\n"
			"flw.ps f6,  960 (x5)\n"
			"flw.ps f16,  352 (x5)\n"
			"flw.ps f27,  320 (x5)\n"
			"flw.ps f29,  704 (x5)\n"
			"flw.ps f11,  672 (x5)\n"
			"flw.ps f17,  288 (x5)\n"
			"flw.ps f21,  768 (x5)\n"
			"flw.ps f2,  448 (x5)\n"
			"flw.ps f1,  640 (x5)\n"
			"flw.ps f26,  608 (x5)\n"
			"flw.ps f28,  96 (x5)\n"
			"flw.ps f23,  736 (x5)\n"
			"flw.ps f31,  192 (x5)\n"
			"flw.ps f25,  160 (x5)\n"
			"flw.ps f30,  0 (x5)\n"
			"flw.ps f19,  224 (x5)\n"
			"flw.ps f14,  800 (x5)\n"
			"flw.ps f8,  864 (x5)\n"
			"flw.ps f0,  896 (x5)\n"
			"flw.ps f4,  544 (x5)\n"
			"flw.ps f7,  992 (x5)\n"
			"flw.ps f20,  64 (x5)\n"
			"flw.ps f22,  32 (x5)\n"
			"flw.ps f15,  928 (x5)\n"
			"flw.ps f24,  512 (x5)\n"
			"flw.ps f9,  384 (x5)\n"
			"flw.ps f12,  256 (x5)\n"
			"flw.ps f3,  480 (x5)\n"
			"flw.ps f5,  576 (x5)\n"
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
			"la x5, rand_ieee754_32\n"
			"flw.ps f26, 32(x5)\n"
			"flw.ps f2, 224(x5)\n"
			"fsgnj.ps f19, f26, f2\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f13, 352(x7)\n"
			"frsq.ps f18, f13\n" 
			"la x12, rand_int_32\n"
			"flw.ps f3, 192(x12)\n"
			"flw.ps f30, 32(x12)\n"
			"fsll.pi f26, f3, f30\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f29, 160(x19)\n"
			"flw.ps f30, 800(x19)\n"
			"flw.ps f3, 64(x19)\n"
			"fmsub.ps f3, f29, f30, f3, rmm\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f3, 288(x25)\n"
			"flw.ps f13, 832(x25)\n"
			"fadd.s f28, f3, f13, rtz\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_9:\n"
			"maskxor m5, m3, m3\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x19, rand_ieee754_32\n"
			"flw.ps f23, 608(x19)\n"
			"fcvt.pw.ps f31, f23, rmm\n" 
			"la x5, rand_int_32\n"
			"flw.ps f24, 832(x5)\n"
			"flw.ps f2, 128(x5)\n"
			"fmin.pi f10, f24, f2\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f13, 672(x20)\n"
			"flw.ps f27, 640(x20)\n"
			"fsgnj.s f27, f13, f27\n" 
			"la x18, rand_int_32\n"
			"flw.ps f10, 352(x18)\n"
			"flw.ps f12, 288(x18)\n"
			"for.pi f27, f10, f12\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f31, 352(x17)\n"
			"flw.ps f19, 768(x17)\n"
			"fsub.ps f25, f31, f19, rne\n" 
			"la x26, rand_int_32\n"
			"flw.ps f12, 992(x26)\n"
			"flw.ps f23, 576(x26)\n"
			"fmin.pi f27, f12, f23\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f14, 640(x9)\n"
			"fsub.s f16, f14, f14, rup\n" 
			"la x6, rand_int_32\n"
			"flw.ps f7, 224(x6)\n"
			"faddi.pi f28, f7, 0x1cc\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f9, 512(x9)\n"
			"flw.ps f23, 128(x9)\n"
			"flw.ps f29, 128(x9)\n"
			"fmadd.s f2, f9, f23, f29, rtz\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f4, 736(x9)\n"
			"flw.ps f27, 928(x9)\n"
			"fcmovm.ps f7, f4, f27\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_9\n"
			"addi x30, x30, 1\n"
			"la x26, rand_ieee754_32\n"
			"flw.ps f0, 672(x26)\n"
			"fcvt.pwu.ps f26, f0, rtz\n" 
			"LBL_SEQID_1_M0_WRITE_ID_1_HID_9:\n"
			"fltm.pi m6, f22, f29\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x10, rand_ieee754_32\n"
			"flw.ps f15, 608(x10)\n"
			"flw.ps f16, 576(x10)\n"
			"flw.ps f8, 736(x10)\n"
			"fnmsub.s f22, f15, f16, f8, rmm\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f31, 288(x25)\n"
			"flw.ps f2, 352(x25)\n"
			"fsgnj.ps f19, f31, f2\n" 
			"la x15, rand_int_32\n"
			"flw.ps f12, 928(x15)\n"
			"flw.ps f23, 896(x15)\n"
			"flt.pi f4, f12, f23\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f22, 544(x6)\n"
			"flw.ps f25, 960(x6)\n"
			"flw.ps f8, 992(x6)\n"
			"fmadd.s f29, f22, f25, f8, rmm\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f27, 320(x11)\n"
			"flw.ps f31, 704(x11)\n"
			"fadd.ps f6, f27, f31, rtz\n" 
			"la x22, rand_int_32\n"
			"flw.ps f19, 224(x22)\n"
			"flw.ps f5, 800(x22)\n"
			"fsub.pi f22, f19, f5\n" 
			"la x22, rand_int_32\n"
			"flw.ps f25, 0(x22)\n"
			"flw.ps f18, 992(x22)\n"
			"fsll.pi f11, f25, f18\n" 
			"la x11, rand_int_32\n"
			"flw.ps f9, 64(x11)\n"
			"flw.ps f26, 608(x11)\n"
			"fadd.pi f8, f9, f26\n" 
			"la x6, rand_int_32\n"
			"flw.ps f27, 448(x6)\n"
			"flw.ps f5, 736(x6)\n"
			"fltu.pi f12, f27, f5\n" 
			"maskpopc x17, m4\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f20, 32(x28)\n"
			"flw.ps f2, 704(x28)\n"
			"fsgnj.ps f28, f20, f2\n" 
			"LBL_SEQID_1_M0_WRITE_ID_2_HID_9:\n"
			"masknot m2, m6\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x13, rand_int_32\n"
			"flw.ps f22, 288(x13)\n"
			"fandi.pi f5, f22, 0x107\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f5, 160(x11)\n"
			"flw.ps f30, 480(x11)\n"
			"flw.ps f13, 224(x11)\n"
			"fmsub.ps f10, f5, f30, f13, rmm\n" 
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
			"csrwi mhpmevent3, 8\n"   
			"csrwi mhpmevent4, 26\n"   
			"csrwi mhpmevent5, 27\n"   
			"csrwi mhpmevent6, 19\n"   
			"csrwi mhpmevent7, 5\n"   
			"csrwi mhpmevent8, 4\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f24,  576 (x5)\n"
			"flw.ps f28,  960 (x5)\n"
			"flw.ps f20,  32 (x5)\n"
			"flw.ps f18,  480 (x5)\n"
			"flw.ps f10,  544 (x5)\n"
			"flw.ps f3,  864 (x5)\n"
			"flw.ps f25,  992 (x5)\n"
			"flw.ps f30,  672 (x5)\n"
			"flw.ps f16,  96 (x5)\n"
			"flw.ps f12,  224 (x5)\n"
			"flw.ps f8,  640 (x5)\n"
			"flw.ps f21,  736 (x5)\n"
			"flw.ps f17,  192 (x5)\n"
			"flw.ps f6,  832 (x5)\n"
			"flw.ps f9,  320 (x5)\n"
			"flw.ps f29,  256 (x5)\n"
			"flw.ps f5,  384 (x5)\n"
			"flw.ps f22,  512 (x5)\n"
			"flw.ps f14,  448 (x5)\n"
			"flw.ps f1,  128 (x5)\n"
			"flw.ps f15,  0 (x5)\n"
			"flw.ps f0,  160 (x5)\n"
			"flw.ps f11,  352 (x5)\n"
			"flw.ps f2,  416 (x5)\n"
			"flw.ps f13,  800 (x5)\n"
			"flw.ps f27,  64 (x5)\n"
			"flw.ps f26,  928 (x5)\n"
			"flw.ps f23,  896 (x5)\n"
			"flw.ps f7,  704 (x5)\n"
			"flw.ps f19,  288 (x5)\n"
			"flw.ps f4,  768 (x5)\n"
			"flw.ps f31,  608 (x5)\n"
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
			"la x27, rand_int_32\n"
			"flw.ps f7, 736(x27)\n"
			"flw.ps f0, 608(x27)\n"
			"fsub.pi f28, f7, f0\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f29, 768(x19)\n"
			"fcvt.f16.ps f14, f29\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f3, 0(x26)\n"
			"flw.ps f9, 928(x26)\n"
			"feq.s x11, f3, f9\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f3, 352(x7)\n"
			"flw.ps f0, 928(x7)\n"
			"fmin.ps f21, f3, f0\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f21, 640(x5)\n"
			"flw.ps f30, 320(x5)\n"
			"fmul.ps f6, f21, f30, rdn\n" 
			"la x9, rand_int_32\n"
			"flw.ps f19, 768(x9)\n"
			"fcvt.ps.pwu f30, f19, rne\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_10:\n"
			"fltm.ps m3, f11, f12\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x11, rand_int_32\n"
			"flw.ps f18, 672(x11)\n"
			"flw.ps f28, 512(x11)\n"
			"feq.pi f27, f18, f28\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f25, 64(x25)\n"
			"flw.ps f5, 736(x25)\n"
			"flt.ps f10, f25, f5\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f24, 224(x26)\n"
			"flw.ps f13, 352(x26)\n"
			"fle.s x28, f24, f13\n" 
			"la x10, rand_int_32\n"
			"flw.ps f24, 736(x10)\n"
			"flw.ps f20, 512(x10)\n"
			"fminu.pi f20, f24, f20\n" 
			"LBL_SEQID_0_FP_TRANS_ID_1_HID_10:\n"
			"la x14, rand_ieee754_32\n"
			"flw.ps f6, 448(x14)\n"
			"frcp.ps f21, f6\n" 
			"la x12, rand_int_32\n"
			"flw.ps f21, 128(x12)\n"
			"flw.ps f15, 480(x12)\n"
			"fsll.pi f26, f21, f15\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f30, 992(x17)\n"
			"flw.ps f28, 800(x17)\n"
			"fmul.ps f20, f30, f28, rne\n" 
			"LBL_SEQID_0_M0_WRITE_ID_2_HID_10:\n"
			"maskor m3, m3, m2\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x15, rand_int_32\n"
			"flw.ps f31, 768(x15)\n"
			"fcvt.ps.pwu f6, f31, rmm\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_0_RDX_ID_0_SHIRE_0_HID_10:\n"
			"li x11, 0x320000000741000a\n"
			"csrw tensor_reduce, x11\n" 
			"csrwi tensor_wait, 9\n" 
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=0); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ITER_0_HID_10:\n"
			"li x6, ((0x1 << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0x3 << TENSOR_QUANT_QUANT_ROW) | (0x14 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x5 << TENSOR_QUANT_TRANSF3) | (0x3 << TENSOR_QUANT_TRANSF2) | (0xa << TENSOR_QUANT_TRANSF1) | (0x9 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_10\n"
			"csrw tensor_error, x0\n"
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_10\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_1_FP_TRANS_ID_3_HID_10:\n"
			"la x25, rand_ieee754_32\n"
			"flw.ps f4, 64(x25)\n"
			"fexp.ps f8, f4\n" 
			"LBL_SEQID_1_M0_WRITE_ID_4_HID_10:\n"
			"fsetm.pi m6, f11\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x13, rand_ieee754_32\n"
			"flw.ps f11, 288(x13)\n"
			"flw.ps f17, 576(x13)\n"
			"fsgnj.s f30, f11, f17\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f27, 608(x9)\n"
			"flw.ps f3, 128(x9)\n"
			"flw.ps f15, 32(x9)\n"
			"fmadd.ps f13, f27, f3, f15, rmm\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f25, 640(x13)\n"
			"flw.ps f6, 576(x13)\n"
			"fsgnjx.ps f23, f25, f6\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f18, 448(x22)\n"
			"fclass.ps f11, f18\n" 
			"la x17, rand_int_32\n"
			"flw.ps f28, 416(x17)\n"
			"fsrai.pi f15, f28, 0x4\n" 
			"la x17, rand_int_32\n"
			"flw.ps f9, 0(x17)\n"
			"flw.ps f6, 704(x17)\n"
			"fmulhu.pi f7, f9, f6\n" 
			"la x10, rand_int_32\n"
			"flw.ps f27, 672(x10)\n"
			"flw.ps f5, 128(x10)\n"
			"fmul.pi f19, f27, f5\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f8, 416(x11)\n"
			"flw.ps f17, 320(x11)\n"
			"fle.ps f3, f8, f17\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f27, 0(x16)\n"
			"flw.ps f19, 992(x16)\n"
			"flw.ps f8, 416(x16)\n"
			"fnmsub.s f9, f27, f19, f8, rdn\n" 
			"la x23, rand_int_32\n"
			"flw.ps f9, 0(x23)\n"
			"flw.ps f0, 448(x23)\n"
			"for.pi f13, f9, f0\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f26, 416(x28)\n"
			"flw.ps f15, 0(x28)\n"
			"fsgnjn.ps f23, f26, f15\n" 
			"la x27, rand_int_32\n"
			"flw.ps f8, 448(x27)\n"
			"flw.ps f31, 64(x27)\n"
			"flt.pi f5, f8, f31\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f21, 448(x15)\n"
			"fcvt.pw.ps f9, f21, rtz\n" 
			"li x5, 0x61af\n"
			"csrw tensor_mask, x5\n"	
			"la x19, rand_ieee754_32\n"
			"flw.ps f21, 960(x19)\n"
			"flw.ps f22, 384(x19)\n"
			"fmin.ps f31, f21, f22\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ID_494833_HID_10:\n"
			"li x6, ((0x1f << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0xc << TENSOR_QUANT_QUANT_ROW) | (0xd << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x2 << TENSOR_QUANT_TRANSF8) | (0x7 << TENSOR_QUANT_TRANSF7) | (0x6 << TENSOR_QUANT_TRANSF6) | (0x6 << TENSOR_QUANT_TRANSF5) | (0x7 << TENSOR_QUANT_TRANSF4) | (0x6 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ITER_0_HID_10:\n"
			"li x5, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x0 << TENSOR_FMA32_B_NUM_COLS) | (0xd << TENSOR_FMA32_A_NUM_ROWS) | (0xa << TENSOR_FMA32_A_NUM_COLS) | (0x3 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x3 << TENSOR_FMA32_SCP_LOC_B) | (0x54 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x0 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
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
			"csrwi mhpmevent3, 25\n"   
			"csrwi mhpmevent4, 11\n"   
			"csrwi mhpmevent5, 10\n"   
			"csrwi mhpmevent6, 6\n"   
			"csrwi mhpmevent7, 12\n"   
			"csrwi mhpmevent8, 1\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f12,  352 (x5)\n"
			"flw.ps f9,  544 (x5)\n"
			"flw.ps f28,  576 (x5)\n"
			"flw.ps f27,  992 (x5)\n"
			"flw.ps f17,  768 (x5)\n"
			"flw.ps f7,  288 (x5)\n"
			"flw.ps f31,  128 (x5)\n"
			"flw.ps f5,  928 (x5)\n"
			"flw.ps f13,  896 (x5)\n"
			"flw.ps f16,  608 (x5)\n"
			"flw.ps f11,  480 (x5)\n"
			"flw.ps f30,  416 (x5)\n"
			"flw.ps f26,  256 (x5)\n"
			"flw.ps f29,  864 (x5)\n"
			"flw.ps f8,  384 (x5)\n"
			"flw.ps f2,  960 (x5)\n"
			"flw.ps f22,  672 (x5)\n"
			"flw.ps f25,  704 (x5)\n"
			"flw.ps f20,  800 (x5)\n"
			"flw.ps f14,  736 (x5)\n"
			"flw.ps f15,  224 (x5)\n"
			"flw.ps f24,  32 (x5)\n"
			"flw.ps f3,  640 (x5)\n"
			"flw.ps f0,  192 (x5)\n"
			"flw.ps f4,  512 (x5)\n"
			"flw.ps f19,  64 (x5)\n"
			"flw.ps f6,  96 (x5)\n"
			"flw.ps f23,  448 (x5)\n"
			"flw.ps f18,  320 (x5)\n"
			"flw.ps f1,  832 (x5)\n"
			"flw.ps f21,  0 (x5)\n"
			"flw.ps f10,  160 (x5)\n"
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
			"la x25, rand_ieee754_32\n"
			"flw.ps f1, 768(x25)\n"
			"frsq.ps f5, f1\n" 
			"la x6, rand_int_32\n"
			"flw.ps f10, 992(x6)\n"
			"flw.ps f19, 608(x6)\n"
			"fmin.pi f27, f10, f19\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_11:\n"
			"mova.m.x x27\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0x7c403723b8\n"
			"fsw.ps f0, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x17, rand_ieee754_32\n"
			"flw.ps f21, 32(x17)\n"
			"fcvt.pwu.ps f11, f21, rdn\n" 
			"la x26, rand_int_32\n"
			"flw.ps f18, 544(x26)\n"
			"flw.ps f4, 896(x26)\n"
			"fmax.pi f2, f18, f4\n" 
			"la x23, rand_int_32\n"
			"flw.ps f19, 160(x23)\n"
			"fandi.pi f24, f19, 0x56\n" 
			"la x9, rand_int_32\n"
			"flw.ps f31, 416(x9)\n"
			"flw.ps f1, 768(x9)\n"
			"feq.pi f15, f31, f1\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f24, 352(x18)\n"
			"flw.ps f30, 160(x18)\n"
			"flw.ps f13, 352(x18)\n"
			"fnmadd.ps f23, f24, f30, f13, rne\n" 
			"la x13, rand_int_32\n"
			"flw.ps f6, 672(x13)\n"
			"fandi.pi f4, f6, 0xbc\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_11:\n"
			"maskxor m6, m6, m1\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x12, rand_int_32\n"
			"flw.ps f16, 960(x12)\n"
			"flw.ps f7, 256(x12)\n"
			"fadd.pi f11, f16, f7\n" 
			"li x17, 0x92e7b2d84510e305\n"
			"fcvt.s.w f28, x17, rtz\n" 
			"la x25, rand_int_32\n"
			"flw.ps f12, 544(x25)\n"
			"fnot.pi f14, f12\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f14, 416(x16)\n"
			"flw.ps f0, 384(x16)\n"
			"flw.ps f26, 704(x16)\n"
			"fmsub.ps f12, f14, f0, f26, rmm\n" 
			"LBL_SEQID_0_M0_WRITE_ID_2_HID_11:\n"
			"fsetm.pi m4, f6\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x13, rand_ieee754_32\n"
			"flw.ps f21, 960(x13)\n"
			"flw.ps f18, 0(x13)\n"
			"fsgnjx.ps f9, f21, f18\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_11\n"
			"addi x30, x30, 1\n"
			"la x26, rand_int_32\n"
			"flw.ps f27, 416(x26)\n"
			"flw.ps f28, 192(x26)\n"
			"fsrl.pi f8, f27, f28\n" 
			"maskpopcz x19, m5\n" 
			"la x26, rand_int_32\n"
			"flw.ps f12, 32(x26)\n"
			"flw.ps f16, 512(x26)\n"
			"fand.pi f29, f12, f16\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f30, 96(x15)\n"
			"flw.ps f26, 960(x15)\n"
			"flw.ps f12, 128(x15)\n"
			"fnmadd.s f1, f30, f26, f12, rtz\n" 
			"la x20, rand_int_32\n"
			"flw.ps f5, 128(x20)\n"
			"flw.ps f11, 896(x20)\n"
			"fmulhu.pi f6, f5, f11\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f30, 608(x22)\n"
			"flw.ps f25, 544(x22)\n"
			"fle.s x9, f30, f25\n" 
			"LBL_SEQID_1_M0_WRITE_ID_3_HID_11:\n"
			"mova.m.x x12\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x27, rand_int_32\n"
			"flw.ps f18, 128(x27)\n"
			"flw.ps f25, 352(x27)\n"
			"feq.pi f12, f18, f25\n" 
			"la x19, rand_int_32\n"
			"flw.ps f9, 256(x19)\n"
			"fnot.pi f11, f9\n" 
			"la x15, rand_int_32\n"
			"flw.ps f5, 0(x15)\n"
			"fnot.pi f18, f5\n" 
			"la x10, rand_int_32\n"
			"flw.ps f5, 224(x10)\n"
			"flw.ps f29, 800(x10)\n"
			"fxor.pi f3, f5, f29\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f29, 128(x22)\n"
			"frsq.ps f6, f29\n" 
			"LBL_SEQID_1_M0_WRITE_ID_4_HID_11:\n"
			"fltm.ps m0, f3, f14\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x5, rand_ieee754_32\n"
			"flw.ps f18, 288(x5)\n"
			"flw.ps f6, 832(x5)\n"
			"fle.ps f5, f18, f6\n" 
			"LBL_SEQID_1_M0_WRITE_ID_5_HID_11:\n"
			"fltm.ps m6, f6, f23\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x13, rand_int_32\n"
			"flw.ps f4, 896(x13)\n"
			"fpackrepb.pi f5, f4\n" 
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
			"csrwi mhpmevent3, 8\n"   
			"csrwi mhpmevent4, 26\n"   
			"csrwi mhpmevent5, 27\n"   
			"csrwi mhpmevent6, 19\n"   
			"csrwi mhpmevent7, 5\n"   
			"csrwi mhpmevent8, 4\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f27,  416 (x5)\n"
			"flw.ps f18,  352 (x5)\n"
			"flw.ps f3,  544 (x5)\n"
			"flw.ps f31,  160 (x5)\n"
			"flw.ps f2,  288 (x5)\n"
			"flw.ps f4,  224 (x5)\n"
			"flw.ps f8,  384 (x5)\n"
			"flw.ps f21,  64 (x5)\n"
			"flw.ps f6,  928 (x5)\n"
			"flw.ps f19,  480 (x5)\n"
			"flw.ps f12,  608 (x5)\n"
			"flw.ps f11,  32 (x5)\n"
			"flw.ps f26,  640 (x5)\n"
			"flw.ps f24,  256 (x5)\n"
			"flw.ps f13,  0 (x5)\n"
			"flw.ps f0,  512 (x5)\n"
			"flw.ps f20,  864 (x5)\n"
			"flw.ps f9,  832 (x5)\n"
			"flw.ps f22,  128 (x5)\n"
			"flw.ps f17,  320 (x5)\n"
			"flw.ps f25,  896 (x5)\n"
			"flw.ps f23,  448 (x5)\n"
			"flw.ps f29,  992 (x5)\n"
			"flw.ps f14,  960 (x5)\n"
			"flw.ps f7,  576 (x5)\n"
			"flw.ps f15,  800 (x5)\n"
			"flw.ps f10,  704 (x5)\n"
			"flw.ps f5,  768 (x5)\n"
			"flw.ps f28,  192 (x5)\n"
			"flw.ps f30,  96 (x5)\n"
			"flw.ps f1,  736 (x5)\n"
			"flw.ps f16,  672 (x5)\n"
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
			"la x26, rand_int_32\n"
			"flw.ps f26, 64(x26)\n"
			"fpackrepb.pi f2, f26\n" 
			"LBL_SEQID_0_FP_TRANS_ID_0_HID_12:\n"
			"la x26, rand_ieee754_32\n"
			"flw.ps f31, 544(x26)\n"
			"fexp.ps f24, f31\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f8, 960(x16)\n"
			"fcvt.f16.ps f6, f8\n" 
			"la x28, rand_int_32\n"
			"flw.ps f16, 576(x28)\n"
			"flw.ps f17, 640(x28)\n"
			"frem.pi f20, f16, f17\n" 
			"la x6, rand_int_32\n"
			"flw.ps f21, 448(x6)\n"
			"flw.ps f19, 736(x6)\n"
			"fsub.pi f21, f21, f19\n" 
			"la x19, rand_int_32\n"
			"flw.ps f6, 448(x19)\n"
			"fcvt.ps.pw f8, f6, rmm\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f5, 160(x28)\n"
			"flw.ps f1, 768(x28)\n"
			"fadd.ps f16, f5, f1, rup\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_12:\n"
			"flem.ps m5, f23, f3\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x15, rand_int_32\n"
			"flw.ps f24, 992(x15)\n"
			"flw.ps f13, 800(x15)\n"
			"fmul.pi f5, f24, f13\n" 
			"la x23, rand_int_32\n"
			"flw.ps f27, 832(x23)\n"
			"flw.ps f17, 544(x23)\n"
			"fmax.pi f6, f27, f17\n" 
			"la x23, rand_int_32\n"
			"flw.ps f13, 320(x23)\n"
			"flw.ps f5, 192(x23)\n"
			"fmulhu.pi f3, f13, f5\n" 
			"la x14, rand_int_32\n"
			"flw.ps f4, 416(x14)\n"
			"flw.ps f13, 992(x14)\n"
			"fand.pi f24, f4, f13\n" 
			"mova.x.m x20\n" 
			"la x27, rand_int_32\n"
			"flw.ps f23, 192(x27)\n"
			"fsat8.pi f23, f23\n" 
			"la x7, rand_int_32\n"
			"flw.ps f25, 736(x7)\n"
			"fpackreph.pi f3, f25\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f31, 544(x20)\n"
			"flw.ps f6, 512(x20)\n"
			"flw.ps f20, 480(x20)\n"
			"fmadd.ps f18, f31, f6, f20, rne\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ID_106170_HID_12:\n"
			"li x5, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x3 << TENSOR_IMA_B_NUM_COLS) | (0x9 << TENSOR_IMA_A_NUM_ROWS) | (0xd << TENSOR_IMA_A_NUM_COLS) | (0x1 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x8c << TENSOR_IMA_SCP_LOC_B) | (0xda << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x0 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x6, ((0x13 << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0x9 << TENSOR_QUANT_QUANT_ROW) | (0x6 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0xa << TENSOR_QUANT_TRANSF2) | (0xa << TENSOR_QUANT_TRANSF1) | (0xa << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 7\n"
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_12\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_0_RDX_ID_0_SHIRE_0_HID_12:\n"
			"li x9, 0x320000000741000a\n"
			"csrw tensor_reduce, x9\n" 
			"csrwi tensor_wait, 9\n" 
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ID_625669_HID_12:\n"
			"li x5, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x2 << TENSOR_FMA16_B_NUM_COLS) | (0xd << TENSOR_FMA16_A_NUM_ROWS) | (0x5 << TENSOR_FMA16_A_NUM_COLS) | (0x6 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0xed << TENSOR_FMA16_SCP_LOC_B) | (0x81 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x0 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
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
			"li x6, ((0x17 << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0x1 << TENSOR_QUANT_QUANT_ROW) | (0x2d << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x1 << TENSOR_QUANT_TRANSF5) | (0x4 << TENSOR_QUANT_TRANSF4) | (0x5 << TENSOR_QUANT_TRANSF3) | (0x8 << TENSOR_QUANT_TRANSF2) | (0x5 << TENSOR_QUANT_TRANSF1) | (0xa << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_12\n"
			"csrw tensor_error, x0\n"
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_12\n"
			"addi x30, x30, 1\n"
			"fbci.pi f18, 0x63b3b\n" 
			"la x18, rand_ieee754_16\n"
			"flw.ps f22, 320(x18)\n"
			"fcvt.ps.f16 f5, f22\n" 
			"la x12, rand_int_32\n"
			"flw.ps f18, 384(x12)\n"
			"fsat8.pi f23, f18\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x16, rand_ieee754_32\n"
			"flw.ps f11, 288(x16)\n"
			"flw.ps f12, 256(x16)\n"
			"fadd.ps f21, f11, f12, rmm\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f24, 32(x6)\n"
			"flw.ps f21, 736(x6)\n"
			"feq.ps f5, f24, f21\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f1, 832(x7)\n"
			"flw.ps f8, 352(x7)\n"
			"flw.ps f4, 224(x7)\n"
			"fnmsub.ps f17, f1, f8, f4, rup\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f31, 864(x28)\n"
			"fclass.s x16, f31\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f6, 224(x27)\n"
			"flw.ps f4, 256(x27)\n"
			"fmul.s f17, f6, f4, rup\n" 
			"la x23, rand_int_32\n"
			"flw.ps f28, 480(x23)\n"
			"flw.ps f31, 544(x23)\n"
			"fsub.pi f25, f28, f31\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f24, 992(x11)\n"
			"flw.ps f12, 896(x11)\n"
			"fsgnj.s f17, f24, f12\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f5, 128(x28)\n"
			"flw.ps f15, 608(x28)\n"
			"fmin.ps f9, f5, f15\n" 
			"la x9, rand_int_32\n"
			"flw.ps f2, 192(x9)\n"
			"flw.ps f5, 96(x9)\n"
			"fmaxu.pi f1, f2, f5\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f19, 96(x5)\n"
			"flw.ps f12, 576(x5)\n"
			"fmin.ps f20, f19, f12\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f8, 768(x11)\n"
			"flw.ps f30, 96(x11)\n"
			"fle.s x15, f8, f30\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f6, 416(x18)\n"
			"fcvt.f16.ps f15, f6\n" 
			"la x23, rand_int_32\n"
			"flw.ps f30, 960(x23)\n"
			"flw.ps f20, 320(x23)\n"
			"for.pi f20, f30, f20\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 0xf39b\n"
			"csrw tensor_mask, x5\n"	
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ID_607001_HID_12:\n"
			"li x6, ((0x5 << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0xa << TENSOR_QUANT_QUANT_ROW) | (0x35 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x2 << TENSOR_QUANT_TRANSF7) | (0x6 << TENSOR_QUANT_TRANSF6) | (0x7 << TENSOR_QUANT_TRANSF5) | (0x6 << TENSOR_QUANT_TRANSF4) | (0x7 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ID_4167_HID_12:\n"
			"li x6, ((0x7 << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0xe << TENSOR_QUANT_QUANT_ROW) | (0xc << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"LBL_SEQID_1_TLD_XMA_STR_SEQ_ITER_0_HID_12:\n"
			"la x31, self_check_32\n"
			"li x5, 0x7\n"
			"add x5, x5, x31\n"
			"li x6, 0x320000000000005\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x3 << TENSOR_FMA32_B_NUM_COLS) | (0x7 << TENSOR_FMA32_A_NUM_ROWS) | (0x5 << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x19 << TENSOR_FMA32_SCP_LOC_B) | (0x0 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x13 << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0x7 << TENSOR_QUANT_QUANT_ROW) | (0x3e << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"csrwi mhpmevent3, 25\n"   
			"csrwi mhpmevent4, 11\n"   
			"csrwi mhpmevent5, 10\n"   
			"csrwi mhpmevent6, 6\n"   
			"csrwi mhpmevent7, 12\n"   
			"csrwi mhpmevent8, 1\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f2,  544 (x5)\n"
			"flw.ps f1,  96 (x5)\n"
			"flw.ps f31,  480 (x5)\n"
			"flw.ps f10,  64 (x5)\n"
			"flw.ps f9,  384 (x5)\n"
			"flw.ps f4,  192 (x5)\n"
			"flw.ps f27,  288 (x5)\n"
			"flw.ps f28,  512 (x5)\n"
			"flw.ps f20,  992 (x5)\n"
			"flw.ps f6,  672 (x5)\n"
			"flw.ps f22,  960 (x5)\n"
			"flw.ps f12,  704 (x5)\n"
			"flw.ps f30,  768 (x5)\n"
			"flw.ps f8,  928 (x5)\n"
			"flw.ps f29,  0 (x5)\n"
			"flw.ps f5,  224 (x5)\n"
			"flw.ps f26,  800 (x5)\n"
			"flw.ps f3,  448 (x5)\n"
			"flw.ps f15,  128 (x5)\n"
			"flw.ps f23,  608 (x5)\n"
			"flw.ps f14,  864 (x5)\n"
			"flw.ps f24,  320 (x5)\n"
			"flw.ps f25,  256 (x5)\n"
			"flw.ps f19,  832 (x5)\n"
			"flw.ps f16,  640 (x5)\n"
			"flw.ps f18,  736 (x5)\n"
			"flw.ps f17,  32 (x5)\n"
			"flw.ps f11,  576 (x5)\n"
			"flw.ps f0,  416 (x5)\n"
			"flw.ps f13,  160 (x5)\n"
			"flw.ps f21,  896 (x5)\n"
			"flw.ps f7,  352 (x5)\n"
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
			"maskpopc x13, m0\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f21, 960(x18)\n"
			"fclass.ps f1, f21\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f7, 320(x20)\n"
			"flw.ps f0, 32(x20)\n"
			"flw.ps f27, 352(x20)\n"
			"fmadd.s f24, f7, f0, f27, rtz\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f1, 736(x20)\n"
			"flw.ps f29, 960(x20)\n"
			"fmax.s f20, f1, f29\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_13:\n"
			"feqm.ps m6, f4, f1\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x27, rand_int_32\n"
			"flw.ps f5, 544(x27)\n"
			"flw.ps f13, 320(x27)\n"
			"fxor.pi f21, f5, f13\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f0, 32(x25)\n"
			"flw.ps f15, 640(x25)\n"
			"fsub.ps f24, f0, f15, rmm\n" 
			"li x12, 0xf71e20a21928f92f\n"
			"fcvt.s.wu f1, x12, rne\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f20, 192(x19)\n"
			"flw.ps f9, 192(x19)\n"
			"fsgnjn.ps f13, f20, f9\n" 
			"la x22, rand_int_32\n"
			"flw.ps f8, 480(x22)\n"
			"flw.ps f28, 320(x22)\n"
			"fsll.pi f31, f8, f28\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f18, 384(x27)\n"
			"flw.ps f29, 768(x27)\n"
			"flw.ps f22, 800(x27)\n"
			"fmsub.ps f9, f18, f29, f22, rmm\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f15, 800(x7)\n"
			"fclass.ps f14, f15\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_13:\n"
			"masknot m4, m3\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0x62901eea6a\n"
			"flw.ps f5, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x18, rand_ieee754_32\n"
			"flw.ps f0, 480(x18)\n"
			"fcvt.pw.ps f27, f0, rmm\n" 
			"la x22, rand_int_32\n"
			"flw.ps f16, 672(x22)\n"
			"flw.ps f29, 576(x22)\n"
			"fmulh.pi f29, f16, f29\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f28, 896(x22)\n"
			"fcvt.w.s x28, f28, rup\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_13\n"
			"addi x30, x30, 1\n"
			"la x20, rand_ieee754_32\n"
			"flw.ps f4, 384(x20)\n"
			"fswizz.ps f19, f4, 0x9\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f13, 704(x14)\n"
			"flw.ps f19, 608(x14)\n"
			"flw.ps f17, 128(x14)\n"
			"fnmsub.s f8, f13, f19, f17, rdn\n" 
			"LBL_SEQID_1_M0_WRITE_ID_2_HID_13:\n"
			"maskxor m6, m6, m0\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x7, rand_ieee754_32\n"
			"flw.ps f22, 416(x7)\n"
			"flw.ps f28, 352(x7)\n"
			"flw.ps f29, 640(x7)\n"
			"fcmov.ps f13, f22, f28, f29\n" 
			"LBL_SEQID_1_M0_WRITE_ID_3_HID_13:\n"
			"mova.m.x x26\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_1_M0_WRITE_ID_4_HID_13:\n"
			"fltm.ps m2, f30, f17\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"li x23, 0xfe23675ce9cf5a0d\n"
			"fcvt.s.wu f31, x23, rmm\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f29, 704(x7)\n"
			"flw.ps f27, 544(x7)\n"
			"fmin.ps f22, f29, f27\n" 
			"LBL_SEQID_1_M0_WRITE_ID_5_HID_13:\n"
			"mova.m.x x16\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x26, rand_ieee754_32\n"
			"flw.ps f3, 256(x26)\n"
			"flw.ps f22, 800(x26)\n"
			"fnmsub.ps f10, f3, f22, f3, rdn\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f22, 544(x10)\n"
			"flw.ps f5, 928(x10)\n"
			"fle.ps f20, f22, f5\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f16, 864(x17)\n"
			"flw.ps f14, 832(x17)\n"
			"fsub.ps f2, f16, f14, rmm\n" 
			"la x20, rand_int_32\n"
			"flw.ps f31, 320(x20)\n"
			"flw.ps f13, 448(x20)\n"
			"fle.pi f27, f31, f13\n" 
			"la x26, rand_int_32\n"
			"flw.ps f27, 416(x26)\n"
			"faddi.pi f24, f27, 0xf5\n" 
			"la x16, rand_int_32\n"
			"flw.ps f3, 448(x16)\n"
			"flw.ps f31, 192(x16)\n"
			"fdiv.pi f13, f3, f31\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f18, 960(x5)\n"
			"flw.ps f31, 800(x5)\n"
			"fmin.ps f15, f18, f31\n" 
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
			"csrwi mhpmevent3, 8\n"   
			"csrwi mhpmevent4, 26\n"   
			"csrwi mhpmevent5, 27\n"   
			"csrwi mhpmevent6, 19\n"   
			"csrwi mhpmevent7, 5\n"   
			"csrwi mhpmevent8, 4\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f3,  416 (x5)\n"
			"flw.ps f23,  352 (x5)\n"
			"flw.ps f15,  128 (x5)\n"
			"flw.ps f7,  64 (x5)\n"
			"flw.ps f9,  864 (x5)\n"
			"flw.ps f17,  160 (x5)\n"
			"flw.ps f10,  448 (x5)\n"
			"flw.ps f19,  576 (x5)\n"
			"flw.ps f31,  960 (x5)\n"
			"flw.ps f20,  992 (x5)\n"
			"flw.ps f25,  192 (x5)\n"
			"flw.ps f2,  96 (x5)\n"
			"flw.ps f13,  224 (x5)\n"
			"flw.ps f6,  800 (x5)\n"
			"flw.ps f0,  640 (x5)\n"
			"flw.ps f4,  288 (x5)\n"
			"flw.ps f11,  672 (x5)\n"
			"flw.ps f26,  0 (x5)\n"
			"flw.ps f24,  832 (x5)\n"
			"flw.ps f18,  896 (x5)\n"
			"flw.ps f12,  704 (x5)\n"
			"flw.ps f14,  320 (x5)\n"
			"flw.ps f22,  480 (x5)\n"
			"flw.ps f1,  32 (x5)\n"
			"flw.ps f5,  608 (x5)\n"
			"flw.ps f30,  736 (x5)\n"
			"flw.ps f16,  512 (x5)\n"
			"flw.ps f27,  768 (x5)\n"
			"flw.ps f28,  928 (x5)\n"
			"flw.ps f8,  384 (x5)\n"
			"flw.ps f21,  256 (x5)\n"
			"flw.ps f29,  544 (x5)\n"
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
			"la x13, rand_ieee754_32\n"
			"flw.ps f11, 224(x13)\n"
			"flw.ps f24, 416(x13)\n"
			"fmin.s f0, f11, f24\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_14:\n"
			"fsetm.pi m5, f2\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x14, rand_ieee754_32\n"
			"flw.ps f1, 800(x14)\n"
			"fcvt.w.s x17, f1, rmm\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f29, 64(x20)\n"
			"fcvt.lu.s x25, f29, rmm\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f26, 832(x7)\n"
			"flw.ps f28, 384(x7)\n"
			"fmax.s f23, f26, f28\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f14, 768(x20)\n"
			"flw.ps f4, 32(x20)\n"
			"flt.s x12, f14, f4\n" 
			"la x5, rand_int_32\n"
			"flw.ps f14, 32(x5)\n"
			"flw.ps f23, 896(x5)\n"
			"fand.pi f25, f14, f23\n" 
			"la x23, rand_int_32\n"
			"flw.ps f1, 128(x23)\n"
			"fslli.pi f30, f1, 0x1\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f12, 64(x7)\n"
			"fswizz.ps f19, f12, 0x9\n" 
			"la x28, rand_int_32\n"
			"flw.ps f23, 416(x28)\n"
			"faddi.pi f9, f23, 0x1ed\n" 
			"la x5, rand_int_32\n"
			"flw.ps f7, 672(x5)\n"
			"flw.ps f19, 608(x5)\n"
			"fle.pi f29, f7, f19\n" 
			"li x5, 0xad722a48ed84c2fc\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0xfbc1ec41bb3d5b65\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"la x15, rand_int_32\n"
			"flw.ps f21, 160(x15)\n"
			"fpackrepb.pi f4, f21\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f10, 384(x12)\n"
			"flw.ps f5, 736(x12)\n"
			"flt.ps f31, f10, f5\n" 
			"la x12, rand_int_32\n"
			"flw.ps f17, 992(x12)\n"
			"fsrai.pi f8, f17, 0x5\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_14:\n"
			"maskor m6, m7, m4\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x15, rand_ieee754_32\n"
			"flw.ps f8, 960(x15)\n"
			"flw.ps f11, 736(x15)\n"
			"flw.ps f1, 576(x15)\n"
			"fmsub.s f10, f8, f11, f1, rup\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ID_712936_HID_14:\n"
			"li x6, ((0x1d << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0x7 << TENSOR_QUANT_QUANT_ROW) | (0x1d << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x0 << TENSOR_QUANT_TRANSF1) | (0x0 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_14\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_0_RDX_ID_0_SHIRE_0_HID_14:\n"
			"li x25, 0x320000000741000a\n"
			"csrw tensor_reduce, x25\n" 
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
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ITER_0_HID_14:\n"
			"la x31, self_check_16\n"
			"li x5, 0x4000000000000d\n"
			"add x5, x5, x31\n"
			"li x6, 0x1000000000000b\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x0 << TENSOR_FMA16_B_NUM_COLS) | (0xd << TENSOR_FMA16_A_NUM_ROWS) | (0xb << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x1 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1c << TENSOR_FMA16_SCP_LOC_B) | (0x2 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x14 << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0xd << TENSOR_QUANT_QUANT_ROW) | (0x3f << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0x18\n"
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
			"flw.ps f20, 192(x5)\n"
			"fnot.pi f26, f20\n" 
			"la x18, rand_int_32\n"
			"flw.ps f12, 0(x18)\n"
			"flw.ps f30, 384(x18)\n"
			"fmulhu.pi f20, f12, f30\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f28, 128(x23)\n"
			"flw.ps f12, 448(x23)\n"
			"fsgnj.ps f14, f28, f12\n" 
			"la x23, rand_int_32\n"
			"flw.ps f19, 320(x23)\n"
			"flw.ps f22, 416(x23)\n"
			"flt.pi f29, f19, f22\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f21, 224(x14)\n"
			"flw.ps f15, 192(x14)\n"
			"fsub.s f28, f21, f15, rmm\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f8, 480(x12)\n"
			"flw.ps f20, 320(x12)\n"
			"fadd.s f15, f8, f20, rup\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f30, 64(x6)\n"
			"flw.ps f22, 896(x6)\n"
			"fadd.ps f23, f30, f22, rne\n" 
			"la x6, rand_int_32\n"
			"flw.ps f13, 544(x6)\n"
			"flw.ps f26, 992(x6)\n"
			"fadd.pi f14, f13, f26\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f27, 192(x12)\n"
			"flw.ps f2, 576(x12)\n"
			"fmin.s f27, f27, f2\n" 
			"la x18, rand_ieee754_16\n"
			"flw.ps f8, 640(x18)\n"
			"fcvt.ps.f16 f9, f8\n" 
			"LBL_SEQID_1_M0_WRITE_ID_2_HID_14:\n"
			"feqm.ps m7, f13, f21\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x13, rand_ieee754_32\n"
			"flw.ps f30, 128(x13)\n"
			"frsq.ps f8, f30\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f23, 160(x15)\n"
			"fcvt.l.s x13, f23, rup\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f8, 512(x6)\n"
			"flw.ps f9, 928(x6)\n"
			"flw.ps f5, 0(x6)\n"
			"fmsub.ps f7, f8, f9, f5, rne\n" 
			"LBL_SEQID_1_M0_WRITE_ID_3_HID_14:\n"
			"feqm.ps m0, f3, f14\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_1_FP_TRANS_ID_4_HID_14:\n"
			"la x10, rand_ieee754_32\n"
			"flw.ps f16, 384(x10)\n"
			"fexp.ps f12, f16\n" 
			"li x5, 0x4b9ef18c58f66531\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0xc9cf538ace831489\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"csrw tensor_error, zero\n" 
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=0); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ITER_0_HID_14:\n"
			"li x6, ((0x16 << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0x4 << TENSOR_QUANT_QUANT_ROW) | (0x21 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x4 << TENSOR_QUANT_TRANSF5) | (0x3 << TENSOR_QUANT_TRANSF4) | (0x3 << TENSOR_QUANT_TRANSF3) | (0xa << TENSOR_QUANT_TRANSF2) | (0x9 << TENSOR_QUANT_TRANSF1) | (0x9 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ITER_1_HID_14:\n"
			"li x6, ((0xa << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0x9 << TENSOR_QUANT_QUANT_ROW) | (0x4 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x7 << TENSOR_QUANT_TRANSF8) | (0x7 << TENSOR_QUANT_TRANSF7) | (0x6 << TENSOR_QUANT_TRANSF6) | (0x6 << TENSOR_QUANT_TRANSF5) | (0x6 << TENSOR_QUANT_TRANSF4) | (0x7 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ITER_2_HID_14:\n"
			"li x6, ((0x3 << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0x3 << TENSOR_QUANT_QUANT_ROW) | (0x3d << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x2 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 10\n"
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
			"csrwi mhpmevent3, 25\n"   
			"csrwi mhpmevent4, 11\n"   
			"csrwi mhpmevent5, 10\n"   
			"csrwi mhpmevent6, 6\n"   
			"csrwi mhpmevent7, 12\n"   
			"csrwi mhpmevent8, 1\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f27,  32 (x5)\n"
			"flw.ps f1,  448 (x5)\n"
			"flw.ps f2,  160 (x5)\n"
			"flw.ps f3,  736 (x5)\n"
			"flw.ps f6,  672 (x5)\n"
			"flw.ps f0,  0 (x5)\n"
			"flw.ps f14,  224 (x5)\n"
			"flw.ps f11,  640 (x5)\n"
			"flw.ps f19,  64 (x5)\n"
			"flw.ps f28,  896 (x5)\n"
			"flw.ps f5,  256 (x5)\n"
			"flw.ps f4,  544 (x5)\n"
			"flw.ps f9,  576 (x5)\n"
			"flw.ps f31,  768 (x5)\n"
			"flw.ps f16,  480 (x5)\n"
			"flw.ps f21,  960 (x5)\n"
			"flw.ps f17,  288 (x5)\n"
			"flw.ps f26,  352 (x5)\n"
			"flw.ps f24,  96 (x5)\n"
			"flw.ps f10,  928 (x5)\n"
			"flw.ps f7,  608 (x5)\n"
			"flw.ps f13,  704 (x5)\n"
			"flw.ps f30,  128 (x5)\n"
			"flw.ps f25,  320 (x5)\n"
			"flw.ps f18,  832 (x5)\n"
			"flw.ps f23,  512 (x5)\n"
			"flw.ps f12,  416 (x5)\n"
			"flw.ps f29,  992 (x5)\n"
			"flw.ps f20,  384 (x5)\n"
			"flw.ps f22,  800 (x5)\n"
			"flw.ps f8,  192 (x5)\n"
			"flw.ps f15,  864 (x5)\n"
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
			"la x27, rand_ieee754_32\n"
			"flw.ps f0, 480(x27)\n"
			"ffrc.ps f4, f0\n" 
			"la x11, rand_int_32\n"
			"flw.ps f11, 608(x11)\n"
			"fcvt.ps.pw f23, f11, rne\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f5, 704(x22)\n"
			"flw.ps f14, 352(x22)\n"
			"flw.ps f31, 64(x22)\n"
			"fnmadd.s f30, f5, f14, f31, rdn\n" 
			"la x28, rand_int_32\n"
			"flw.ps f22, 352(x28)\n"
			"fsrli.pi f9, f22, 0x0\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f11, 224(x19)\n"
			"fcvt.pw.ps f4, f11, rtz\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f9, 736(x9)\n"
			"flw.ps f19, 928(x9)\n"
			"flw.ps f6, 960(x9)\n"
			"fnmadd.s f2, f9, f19, f6, rmm\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f16, 480(x10)\n"
			"flw.ps f27, 224(x10)\n"
			"fle.ps f27, f16, f27\n" 
			"LBL_SEQID_0_FP_TRANS_ID_0_HID_15:\n"
			"la x19, rand_ieee754_32\n"
			"flw.ps f5, 992(x19)\n"
			"fexp.ps f19, f5\n" 
			"la x18, rand_int_32\n"
			"flw.ps f8, 672(x18)\n"
			"fslli.pi f5, f8, 0x4\n" 
			"la x25, rand_int_32\n"
			"flw.ps f3, 608(x25)\n"
			"flw.ps f1, 896(x25)\n"
			"fltu.pi f0, f3, f1\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f22, 416(x6)\n"
			"flt.ps f9, f22, f22\n" 
			"fbci.ps f27, 0x6f18\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_15:\n"
			"maskand m3, m3, m3\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x18, rand_int_32\n"
			"flw.ps f27, 96(x18)\n"
			"flw.ps f8, 896(x18)\n"
			"fmin.pi f31, f27, f8\n" 
			"fbci.pi f13, 0x3b3a5\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f20, 960(x22)\n"
			"flw.ps f28, 832(x22)\n"
			"fadd.s f26, f20, f28, rdn\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_15\n"
			"addi x30, x30, 1\n"
			"la x15, rand_ieee754_32\n"
			"flw.ps f18, 928(x15)\n"
			"flw.ps f23, 64(x15)\n"
			"flw.ps f7, 288(x15)\n"
			"fnmsub.ps f13, f18, f23, f7, rtz\n" 
			"LBL_SEQID_1_M0_WRITE_ID_2_HID_15:\n"
			"mov.m.x m1, x19, 0x24\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0x9352dd8f8e\n"
			"fsw.ps f0, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"maskpopcz x6, m4\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f27, 512(x6)\n"
			"flw.ps f25, 288(x6)\n"
			"flw.ps f15, 672(x6)\n"
			"fnmsub.ps f1, f27, f25, f15, rne\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f13, 832(x6)\n"
			"flw.ps f14, 32(x6)\n"
			"fmin.ps f0, f13, f14\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f4, 608(x22)\n"
			"frsq.ps f0, f4\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f20, 544(x14)\n"
			"flw.ps f19, 992(x14)\n"
			"flw.ps f21, 64(x14)\n"
			"fcmov.ps f8, f20, f19, f21\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f26, 640(x13)\n"
			"flw.ps f25, 480(x13)\n"
			"feq.s x9, f26, f25\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f17, 704(x9)\n"
			"flw.ps f15, 32(x9)\n"
			"flw.ps f19, 224(x9)\n"
			"fmsub.ps f10, f17, f15, f19, rtz\n" 
			"la x25, rand_int_32\n"
			"flw.ps f12, 256(x25)\n"
			"flw.ps f2, 128(x25)\n"
			"fand.pi f10, f12, f2\n" 
			"li x12, 0xd2af7ad264fc3af7\n"
			"fcvt.s.w f30, x12, rne\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f30, 256(x11)\n"
			"flw.ps f23, 640(x11)\n"
			"flw.ps f5, 64(x11)\n"
			"fmadd.ps f28, f30, f23, f5, rne\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f22, 160(x18)\n"
			"flw.ps f1, 0(x18)\n"
			"flw.ps f17, 384(x18)\n"
			"fmsub.ps f1, f22, f1, f17, rdn\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f23, 480(x12)\n"
			"fclass.s x25, f23\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f4, 32(x26)\n"
			"flw.ps f31, 96(x26)\n"
			"fmin.s f25, f4, f31\n" 
			"li x6, 0x2b4664db29324d87\n"
			"fcvt.s.w f27, x6, rtz\n" 
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
		0xf8b6713a, 
		0x4a17ee09, 
		0xc1972609, 
		0x97a51d3d, 
		0xcae6fedc, 
		0xee4f8296, 
		0xc318d25d, 
		0x1ac40d0a, 
		0x89b9c046, 
		0x858d2e75, 
		0xbd95eac8, 
		0xb6d0ede3, 
		0xa40b7961, 
		0x5db1070c, 
		0x9482616a, 
		0x433bdf55, 
		0x4a1604b7, 
		0x2e0d05ff, 
		0x3d8105db, 
		0x9e443abc, 
		0xdd7bee34, 
		0xdce04e64, 
		0x406749c6, 
		0x8722cac3, 
		0x3a8f5e5b, 
		0xf1ae3c0b, 
		0x1748f124, 
		0x4a331952, 
		0x71fcc736, 
		0xab526f0f, 
		0xa99eb825, 
		0xa2ad6d9e, 
		0xff5e7232, 
		0xdbc4bbed, 
		0x0da0efb9, 
		0x7fe5e836, 
		0xa121dac6, 
		0x95523e7f, 
		0x04ef6a0d, 
		0x1b932590, 
		0x599bd6fa, 
		0x98d43bd4, 
		0x583f2f58, 
		0x4f8e28fa, 
		0x4ce246b4, 
		0xe3e37487, 
		0xcf869281, 
		0x2d8c04f8, 
		0xdf721e95, 
		0x0d7c9915, 
		0x9d650184, 
		0xb5d8ff18, 
		0x4b9942f6, 
		0x73445e4d, 
		0x69dbee69, 
		0x861bb1cf, 
		0xb93b600d, 
		0x85e2231c, 
		0x172ef049, 
		0x53cfb8ee, 
		0xf05ee94f, 
		0xe34fc674, 
		0x470bf4dd, 
		0xaa0cd7f3, 
		0x34f313bf, 
		0xd1efea6b, 
		0xbcca77bb, 
		0xf8975a65, 
		0xc29d1c7d, 
		0x8e2c0cf6, 
		0x38c201de, 
		0xdb2ec435, 
		0x637862fe, 
		0xa4ea1b49, 
		0xb5c6c0cc, 
		0x0ca3a456, 
		0x8f855ad3, 
		0x5e9d59af, 
		0xbdc4b8f0, 
		0x0495a3de, 
		0x12e5ec94, 
		0x244d8614, 
		0xd74504b0, 
		0x624ff6ee, 
		0xe9a842de, 
		0xaa0c63eb, 
		0x228c247f, 
		0x41d20a9d, 
		0xa3c6048a, 
		0xec35f6d0, 
		0x137670a9, 
		0xe0eb135f, 
		0xab74aadf, 
		0xc96ac928, 
		0x4c0e91c4, 
		0x401ea660, 
		0xa725146e, 
		0x7a6689c3, 
		0x66fa5e6d, 
		0x0ebf9f95, 
		0x2cbda7ae, 
		0x5055737d, 
		0x9c645391, 
		0xbfb2b3f8, 
		0x5de3ea43, 
		0xf8f7c04f, 
		0xe63ccfa9, 
		0x19adf6d1, 
		0xd95149b9, 
		0xc50e9628, 
		0xb4d05144, 
		0xa8102200, 
		0xacd99ecb, 
		0x0ed9afd1, 
		0x7f2b0967, 
		0x5f820fcd, 
		0xf886640b, 
		0xc67501f9, 
		0xbee4b18c, 
		0xdd5bb6a1, 
		0xb44b8195, 
		0xb2710905, 
		0xd8c85c0c, 
		0x9dfef9ce, 
		0x9060f7f8, 
		0x5dc1d0a9, 
		0x6f8566f1, 
		0x40e4938f, 
		0x13eb0b7d, 
		0x4c348224, 
		0xeac41968, 
		0x8fdb6380, 
		0x8f7e5a59, 
		0x78f67896, 
		0xa26e6870, 
		0x3e1f4f5e, 
		0x95183128, 
		0xaac6a1ae, 
		0x92f800b6, 
		0xbf2337d0, 
		0xe61f8b9e, 
		0x1cfc6d7d, 
		0xb83b4bd5, 
		0xcd3f84c8, 
		0xdc042503, 
		0x600bde4d, 
		0x5d819d5f, 
		0x944739f2, 
		0x8798256a, 
		0x1e0221b0, 
		0xb04ce849, 
		0x2478893d, 
		0x80dab668, 
		0x5d1a3735, 
		0xfb732ad6, 
		0x1a048fb2, 
		0x4212a406, 
		0xd30163c9, 
		0x5b3b2412, 
		0x60f28f7f, 
		0x4ec45fc6, 
		0x820b7856, 
		0x09566952, 
		0x93878b99, 
		0x589a5013, 
		0x06174134, 
		0x5707e781, 
		0x10ad78d6, 
		0xf30a3402, 
		0xf22621f1, 
		0xb2ebd976, 
		0xff43cfbb, 
		0x9c3da643, 
		0x6a669c7a, 
		0x5464ff49, 
		0x7e6c33d6, 
		0xa3edfa59, 
		0x70ecb757, 
		0x8b4b4103, 
		0x0743398b, 
		0x9cc608f8, 
		0x8b54306f, 
		0x7f8117fa, 
		0xc9b04a3e, 
		0x792fb9fd, 
		0xe95dd842, 
		0xd3dae8cc, 
		0xf6b4276e, 
		0xfe670b06, 
		0x69b73ba6, 
		0x5754ed5b, 
		0x2f539d4e, 
		0xc59ed298, 
		0xf3046d3e, 
		0x8988abdc, 
		0xd96339fe, 
		0x7b44df22, 
		0x30d4184c, 
		0x02eeac93, 
		0xfe477e72, 
		0x5486782e, 
		0x805e7405, 
		0xb475298a, 
		0x35bde6d0, 
		0xb818c2b2, 
		0x90520e5b, 
		0x18fd85c4, 
		0x65a0d0ed, 
		0xae4c5b37, 
		0xc7db23a4, 
		0xf8b51a5b, 
		0x27d43843, 
		0xd5e0084d, 
		0x97921d41, 
		0xda7d8903, 
		0x8b082bb4, 
		0x54574e68, 
		0x81901c10, 
		0x6c4b4d1f, 
		0x6a441a7f, 
		0x8661c0ea, 
		0xa1f9fde6, 
		0x9674b861, 
		0x90e2fe3a, 
		0x916a8ec3, 
		0x3e2801cf, 
		0xcb1f7e9c, 
		0x8eb8da9e, 
		0x8dd3fb28, 
		0xf24d7044, 
		0x8b3d4001, 
		0xbe0a41b1, 
		0xf545c49d, 
		0x86a592ce, 
		0xcb3fa098, 
		0xfb578dbf, 
		0x1a954fba, 
		0x3f47f763, 
		0x67feccb3, 
		0xac5a9b3c, 
		0xcfcd8b5b, 
		0x04e5491e, 
		0xaeed8076, 
		0xb9c8fc8d, 
		0x5bf87082, 
		0x07de2d20, 
		0x0f39f972, 
		0xee46abf4, 
		0x38f03426, 
		0xb71f130f, 
		0x5683e8bd, 
		0x5ce2c0f2, 
		0xd0622354, 
		0xd8d91223, 
		0xced3470c, 
		0x6fed4f47  
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
		0xc4f8, 
		0xd3c0, 
		0x5240, 
		0xd240, 
		0xecac, 
		0x6d3a, 
		0xa086, 
		0x20dd, 
		0x4cc0, 
		0x4880, 
		0x53c0, 
		0x3c00, 
		0xd300, 
		0xd1a0, 
		0x5180, 
		0xcec0, 
		0x2ae8, 
		0x4e00, 
		0xd300, 
		0xd140, 
		0xb52d, 
		0x5260, 
		0xb0ab, 
		0x4b80, 
		0xfc76, 
		0x0000, 
		0xc900, 
		0x4dc0, 
		0x51e0, 
		0x50c0, 
		0x4f80, 
		0x4b80, 
		0x7c32, 
		0x6ed8, 
		0x0a33, 
		0x4500, 
		0x4d40, 
		0x51a0, 
		0x72aa, 
		0x30a5, 
		0x3e77, 
		0x50cc, 
		0x9a36, 
		0x4400, 
		0xfc00, 
		0x4cc0, 
		0x5080, 
		0xd320, 
		0xfc00, 
		0x4d80, 
		0x5100, 
		0x7d61, 
		0xcf00, 
		0xcdc0, 
		0xa9d8, 
		0xcd80, 
		0x4134, 
		0x7c00, 
		0xd360, 
		0xcc00, 
		0xce40, 
		0x4800, 
		0xd200, 
		0x425d, 
		0xc900, 
		0xc880, 
		0x53e0, 
		0x8bd2, 
		0x5180, 
		0xc400, 
		0xd040, 
		0x8000, 
		0xc000, 
		0x52a0, 
		0xd0c0, 
		0x4f00, 
		0xc880, 
		0x4fc0, 
		0xfc00, 
		0xd120, 
		0xd3a0, 
		0x44c7, 
		0x4df5, 
		0x7c00, 
		0x5180, 
		0x5120, 
		0xd3e0, 
		0xc400, 
		0x0000, 
		0x2671, 
		0x0000, 
		0xdb72, 
		0x6a5d, 
		0x6bfd, 
		0x2718, 
		0x4dc0, 
		0xc86a, 
		0xbd25, 
		0xd5f9, 
		0x4a00, 
		0xd1a0, 
		0xf342, 
		0x4400, 
		0xd380, 
		0x0000, 
		0x53c0, 
		0x50c0, 
		0x851b, 
		0x0000, 
		0xc980, 
		0x4b00, 
		0xbbae, 
		0x5040, 
		0xb19d, 
		0xe96e, 
		0xcd78, 
		0xa0b2, 
		0x4dc0, 
		0xd0e0, 
		0xcd40, 
		0xe655, 
		0x4d40, 
		0x894e, 
		0xd1e0, 
		0x81f8, 
		0xc685, 
		0xc000, 
		0x30a8, 
		0x50a0, 
		0x4e80, 
		0x8b84, 
		0xca3a, 
		0x5040, 
		0xd3e0, 
		0x2706, 
		0xe407, 
		0x4880, 
		0x869d, 
		0x4000, 
		0xcd00, 
		0x7c00, 
		0x4f00, 
		0x0b8b, 
		0xf2f2, 
		0x83ff, 
		0xcc40, 
		0x8000, 
		0x0000, 
		0x606d, 
		0xd120, 
		0x7011, 
		0x8000, 
		0xcc40, 
		0x4d40, 
		0x51e0, 
		0x52a0, 
		0xd340, 
		0xd280, 
		0x6d56, 
		0x6461, 
		0xfcb2, 
		0xd1c0, 
		0xd120, 
		0x4d80, 
		0xd4f0, 
		0x5140, 
		0x51e0, 
		0xe499, 
		0xfcda, 
		0xfc00, 
		0xce40, 
		0x2a0e, 
		0x4980, 
		0x4500, 
		0x0000, 
		0x31fe, 
		0xbc49, 
		0x6908, 
		0x0000, 
		0xc638, 
		0xd3a0, 
		0x00b4, 
		0xd200, 
		0xedb2, 
		0x0c19, 
		0xc980, 
		0xd240, 
		0x4d00, 
		0xd1e0, 
		0x8000, 
		0x5080, 
		0xafc0, 
		0x0000, 
		0xd0c0, 
		0xd140, 
		0x463b, 
		0xfc00, 
		0x4b80, 
		0xbc00, 
		0xd060, 
		0x9c91, 
		0x1052, 
		0x5140, 
		0x5160, 
		0x5af4, 
		0x4cc0, 
		0x5340, 
		0xcfc0, 
		0x4d00, 
		0xfc00, 
		0xfc00, 
		0x4e00, 
		0xd020, 
		0x5180, 
		0xdeec, 
		0x0000, 
		0x4b00, 
		0xa05f, 
		0xd3a0, 
		0x5300, 
		0xd0a0, 
		0xa6de, 
		0xb759, 
		0x2125, 
		0xd3e0, 
		0x8000, 
		0x0000, 
		0x5240, 
		0x8000, 
		0x9b4a, 
		0xdf12, 
		0xdcf8, 
		0x73e8, 
		0x8dcb, 
		0xbbf2, 
		0xa3fa, 
		0xcd00, 
		0xcf40, 
		0x7c00, 
		0xf1cb, 
		0xd340, 
		0xc500, 
		0xd2e0, 
		0xd140, 
		0xce40, 
		0x8330, 
		0xd6a0, 
		0xd0c0, 
		0x5300, 
		0xcc00, 
		0x4f00, 
		0x4e40, 
		0xd060, 
		0x0d2e, 
		0xd140, 
		0x8000, 
		0x8000, 
		0xd0e0, 
		0x4f80, 
		0xcdc0, 
		0x9224, 
		0xca00, 
		0x7c00, 
		0x8ee2, 
		0x4d1e, 
		0xd1e0, 
		0x7db6, 
		0x5160, 
		0x5240, 
		0x7c00, 
		0xc000, 
		0xe6cc, 
		0x1dff, 
		0x4f40, 
		0xd200, 
		0x4000, 
		0xca80, 
		0x4cc0, 
		0x4a00, 
		0x4880, 
		0x51c0, 
		0x4b00, 
		0x5080, 
		0x5040, 
		0xd200, 
		0xd1c0, 
		0x474f, 
		0xcd80, 
		0xc400, 
		0xccc0, 
		0x0000, 
		0xb81e, 
		0xcb00, 
		0x4c80, 
		0x3c85, 
		0xd3a0, 
		0x3c00, 
		0xd2e0, 
		0xdcd5, 
		0x4c00, 
		0xcd40, 
		0xd100, 
		0x4b00, 
		0x4d00, 
		0x4cc0, 
		0x085a, 
		0x9f91, 
		0xaa17, 
		0x5040, 
		0x5162, 
		0xcec0, 
		0x8000, 
		0x50a0, 
		0x4900, 
		0x859b, 
		0xd3e0, 
		0xcc80, 
		0x9706, 
		0x53c0, 
		0x4d00, 
		0x7c00, 
		0xc800, 
		0x4700, 
		0x8000, 
		0x5280, 
		0xd280, 
		0x4400, 
		0x5000, 
		0xfc00, 
		0xc600, 
		0xfc00, 
		0x4600, 
		0xc800, 
		0xd100, 
		0xc980, 
		0x7c00, 
		0x642c, 
		0x4a00, 
		0xca80, 
		0xe5f1, 
		0xd2a0, 
		0xd080, 
		0x8000, 
		0x4f40, 
		0x4cc0, 
		0xc500, 
		0x5260, 
		0xce80, 
		0xd160, 
		0x7de9, 
		0x5040, 
		0xd806, 
		0xd1c0, 
		0xd2e0, 
		0xbc4d, 
		0x5260, 
		0x4dc0, 
		0x4d40, 
		0xcf40, 
		0x51c0, 
		0xfc00, 
		0x8367, 
		0x7c00, 
		0x4f40, 
		0x5220, 
		0xfc00, 
		0x0000, 
		0x4f80, 
		0x7da2, 
		0xccc0, 
		0xd380, 
		0x3ab8, 
		0x52e0, 
		0x4e40, 
		0xfc76, 
		0xcc40, 
		0xc200, 
		0xd120, 
		0x50c0, 
		0x5080, 
		0xd2e0, 
		0x6dd2, 
		0xc980, 
		0x4400, 
		0x4b00, 
		0xd060, 
		0x4900, 
		0xcb80, 
		0xd300, 
		0x9f7a, 
		0xcf00, 
		0xf4d9, 
		0x51a0, 
		0xb674, 
		0xd2c0, 
		0x4900, 
		0x5300, 
		0xd2e0, 
		0x3aef, 
		0xcc00, 
		0x5220, 
		0x689c, 
		0x4c00, 
		0x5160, 
		0x5080, 
		0x1bd2, 
		0xf724, 
		0x53a0, 
		0x5380, 
		0x4500, 
		0xce80, 
		0xbc00, 
		0x4b80, 
		0x3fd9, 
		0xf8ae, 
		0xb185, 
		0xcc40, 
		0x4b80, 
		0x50c0, 
		0x56d0, 
		0xc600, 
		0xcd00, 
		0xd120, 
		0xd01d, 
		0xf381, 
		0x0000, 
		0x4f80, 
		0xd3a0, 
		0x8000, 
		0x059c, 
		0xd360, 
		0xdfcc, 
		0x1d3d, 
		0x4900, 
		0xca00, 
		0xd040, 
		0xbc00, 
		0x4200, 
		0x25bc, 
		0x9066, 
		0x1b5c, 
		0x6bdc, 
		0xd0c0, 
		0xcc00, 
		0xcec0, 
		0x5000, 
		0xd2e0, 
		0xc500, 
		0xd6d8, 
		0x6529, 
		0xd2a0, 
		0xd0e0, 
		0x9b71, 
		0x4900, 
		0xd596, 
		0xcc80, 
		0x4f80, 
		0x0000, 
		0x8e1d, 
		0x5080, 
		0xc400, 
		0x49fb, 
		0xf7cb, 
		0xadc7, 
		0xc980, 
		0xd000, 
		0xdeed, 
		0xaa8e, 
		0xcb00, 
		0x4400, 
		0xc600, 
		0x5b81, 
		0x5380, 
		0x0906, 
		0xd2c0, 
		0xf96c, 
		0xd2e0, 
		0x53a0, 
		0xce00, 
		0x936e, 
		0xd380, 
		0x4b00, 
		0xcdc0, 
		0x73bb, 
		0xd280, 
		0xd260, 
		0x5080, 
		0x4b80, 
		0x4ebd, 
		0xa022, 
		0x4c40, 
		0x50e0, 
		0xcf80, 
		0xcc40, 
		0x4e7a, 
		0x5360, 
		0xcd00, 
		0x5160, 
		0xd300, 
		0xcc40, 
		0x5000, 
		0xd220, 
		0x50e0, 
		0xc700, 
		0xd1c0, 
		0x4f40, 
		0x8000, 
		0x5060, 
		0x8000, 
		0xd3a0, 
		0xd2c0, 
		0x5240, 
		0x4c00, 
		0xcf40, 
		0x5220, 
		0x51e0, 
		0x4c40, 
		0x0000, 
		0x4500, 
		0xd260, 
		0x5360, 
		0x4200, 
		0x5120, 
		0x50a0, 
		0xd8d5, 
		0xb4d1, 
		0x7cfa, 
		0x45ec, 
		0x4800, 
		0x4900, 
		0x5100, 
		0x1880, 
		0xc997, 
		0x53c0, 
		0x7c00, 
		0xaf4b, 
		0xd000, 
		0xcf00, 
		0x46ed, 
		0x8943, 
		0xd0c0, 
		0xd300, 
		0x4fc0, 
		0x4fc0, 
		0x5160, 
		0xcfc0, 
		0xebfd, 
		0x2e4d, 
		0xc700, 
		0xfc00, 
		0xcc40, 
		0x7710, 
		0xcd80, 
		0xbc00, 
		0x5160, 
		0x7839, 
		0xd3c0, 
		0xd0e0, 
		0xc200, 
		0xd220, 
		0x5200, 
		0x4e80, 
		0xd3a0, 
		0xfc00, 
		0xf60f, 
		0xd1a0, 
		0xd060, 
		0xd340, 
		0x8000, 
		0x48bd, 
		0xd020, 
		0xcd29, 
		0xcfc0, 
		0xfc00, 
		0xcec0, 
		0xd2a0, 
		0x53a0, 
		0x5080, 
		0xcf00, 
		0xd2a0, 
		0x4a80, 
		0xd180, 
		0xeaab, 
		0xe397, 
		0x1389, 
		0x0754, 
		0xc318, 
		0x50c0, 
		0x8000, 
		0xcf40, 
		0x9a10, 
		0x4d80, 
		0x5000, 
		0x6992, 
		0xd100, 
		0x8353, 
		0x0000, 
		0x51c0, 
		0x8000, 
		0x1900, 
		0x94f5, 
		0x8f4c, 
		0x5240, 
		0x4b00, 
		0x7026, 
		0xd100, 
		0xb1cc, 
		0x4b80, 
		0x4980, 
		0x4c80, 
		0x4980, 
		0x0000, 
		0x5360, 
		0xfdf6, 
		0xca80, 
		0xef0f, 
		0xc400, 
		0xd1e0, 
		0x50e0, 
		0xc400, 
		0x51a0, 
		0xce80, 
		0x4f00, 
		0xd240, 
		0x1c56, 
		0x4d80, 
		0x50e0, 
		0xcfc0, 
		0x51c0, 
		0xc980, 
		0x50e0, 
		0x4700, 
		0x97d9, 
		0x1bbe, 
		0xcb80, 
		0xc900, 
		0xd0e0, 
		0xc980, 
		0xd160, 
		0x49db, 
		0xab33, 
		0x5280, 
		0x8e5b, 
		0xc880, 
		0x5140, 
		0x4700, 
		0x3ae4, 
		0xcd40, 
		0x4d00, 
		0x4a80, 
		0x5020, 
		0x632a, 
		0x7c00, 
		0x7ac7, 
		0x8000, 
		0xcdb9, 
		0x0000, 
		0x5180, 
		0x4c40, 
		0x00f5, 
		0x5260, 
		0x0291, 
		0xca00, 
		0x8000, 
		0xca00, 
		0x5200, 
		0xce80, 
		0xd240, 
		0x4d80, 
		0x0000, 
		0xd000, 
		0xfd65, 
		0x5120, 
		0xd200, 
		0x5040, 
		0xd2a0, 
		0xfcaf, 
		0xcb80, 
		0x3e39, 
		0xe325, 
		0xc980, 
		0x332a, 
		0x4600, 
		0x0000, 
		0xcb80, 
		0xfcbc, 
		0x5240, 
		0x0000, 
		0xcf00, 
		0x8000, 
		0xbc00, 
		0xd3a0, 
		0x93a0, 
		0x4b80, 
		0x2fe3, 
		0x4d00, 
		0xd160, 
		0x0000, 
		0x4e80, 
		0x5180, 
		0x1d6e, 
		0x4700, 
		0x2c25, 
		0xfc00, 
		0x4f40, 
		0xcf80, 
		0x4e00, 
		0x1b4d, 
		0x4ec0, 
		0x4800, 
		0x51e0, 
		0xd0e0, 
		0xce80, 
		0xc25b, 
		0x5280, 
		0xca80, 
		0xd360, 
		0xf9ea, 
		0xcfc0, 
		0x8ad0, 
		0xcdc0, 
		0x8115, 
		0x8000, 
		0xcb00, 
		0xcb00, 
		0xca80, 
		0x4500, 
		0xfcad, 
		0xc600, 
		0x7d64, 
		0x9436, 
		0x5220, 
		0xcfc0, 
		0xe65c, 
		0x4b00, 
		0xd320, 
		0xd300, 
		0x4c80, 
		0x6470, 
		0xcd40, 
		0xc500, 
		0xce00, 
		0xace2, 
		0xd260, 
		0x0000, 
		0xa311, 
		0xd000, 
		0x0000, 
		0x8000, 
		0x1bcf, 
		0xbc00, 
		0x59ca, 
		0x0000, 
		0x4000, 
		0xd300, 
		0x4a00, 
		0x52c0, 
		0x5160, 
		0x96f3, 
		0xd3e0, 
		0x4a00, 
		0x9466, 
		0x5380, 
		0x5200, 
		0x5fb7, 
		0x4f40, 
		0x4400, 
		0xfc00, 
		0xca80, 
		0x8000, 
		0xcc80, 
		0xbf67, 
		0xd180, 
		0x4700, 
		0xb1c7, 
		0xb6ea, 
		0x4d00, 
		0xc880, 
		0xfc00, 
		0x948b, 
		0xbc00, 
		0x5380, 
		0x9c94, 
		0xd080, 
		0x3a31, 
		0x5040, 
		0xcec0, 
		0x9bb9, 
		0x4700, 
		0x4e80, 
		0xc84a, 
		0xcb80, 
		0xd2c0, 
		0xf995, 
		0x8000, 
		0xcf00, 
		0xcec0, 
		0xa90e, 
		0x23a2, 
		0xcdc0, 
		0xbd8d, 
		0xd160, 
		0x01e0, 
		0x53a0, 
		0xd100, 
		0x0000, 
		0x5140, 
		0xd2a0, 
		0xde79, 
		0xd2e0, 
		0x2255, 
		0x5422, 
		0xd180, 
		0x4880, 
		0x53c0, 
		0xc800, 
		0xd100, 
		0xf9f2, 
		0xa7d6, 
		0x5060, 
		0x8000, 
		0x1cbb, 
		0x9cfc, 
		0x4cc0, 
		0xd240, 
		0x7cfc, 
		0xc126, 
		0xdc5b, 
		0xce40, 
		0xe4a9, 
		0x5080, 
		0xd260, 
		0xd0a0, 
		0xd300, 
		0xbc00, 
		0x4880, 
		0x4500, 
		0x4e40, 
		0x4dc0, 
		0x5360, 
		0xd21c, 
		0x0109, 
		0x4e80, 
		0x5340, 
		0xd2c0, 
		0x8000, 
		0x3ff8, 
		0x0000, 
		0x7396, 
		0x6942, 
		0x4f40, 
		0x7f7c, 
		0xc200, 
		0x4400, 
		0xb9f7, 
		0xcc80, 
		0x51a0, 
		0x53a0, 
		0xb186, 
		0xce00, 
		0xd2e0, 
		0xd160, 
		0x5180, 
		0xd080, 
		0x0000, 
		0x5240, 
		0xfc00, 
		0x4f40, 
		0x4a00, 
		0x68ab, 
		0xd360, 
		0x5220, 
		0x4500, 
		0x4600, 
		0xcfc0, 
		0xd000, 
		0xce40, 
		0x7a49, 
		0x4700, 
		0xca80, 
		0xd060, 
		0xd20d, 
		0x7c00, 
		0x4800, 
		0x1b73, 
		0xd3a0, 
		0xd180, 
		0x52e0, 
		0xb1b8, 
		0xcc40, 
		0xce80, 
		0xcc80, 
		0x5260, 
		0xce40, 
		0x0000, 
		0x5100, 
		0xd05f, 
		0x38e8, 
		0x4b00, 
		0x5060, 
		0x4a00, 
		0x8384, 
		0x50c0, 
		0xdc84, 
		0x50c0, 
		0x58f7, 
		0xd0a0, 
		0xd300, 
		0x4e80, 
		0xd180, 
		0x51c0, 
		0x4ea1, 
		0x1bf5, 
		0x4fc0, 
		0xd976, 
		0x7382, 
		0xd200, 
		0xd1c0, 
		0x5060, 
		0x4880, 
		0xcd80, 
		0xcf00, 
		0x7c38, 
		0xcf40, 
		0xfc00, 
		0xa923, 
		0x8000, 
		0x0033, 
		0xd300, 
		0x3ecd, 
		0xfdf9, 
		0xd2e0, 
		0xb40a, 
		0x8000, 
		0x4880, 
		0x7de5, 
		0x0000, 
		0x5340, 
		0x604e, 
		0x5380, 
		0x3f9b, 
		0x4f40, 
		0xca80, 
		0xd100, 
		0x4e80, 
		0x76fd, 
		0xc980, 
		0xca00, 
		0xcf80, 
		0xb040, 
		0xd000, 
		0xce40, 
		0x8160, 
		0x04fa, 
		0x5080, 
		0x5060, 
		0x8000, 
		0x4e80, 
		0x4b00, 
		0x4880, 
		0xd0a0, 
		0xd340, 
		0x5100, 
		0xfdd0, 
		0xc0c9, 
		0x169d, 
		0xfa9c, 
		0xc600, 
		0xd0e0, 
		0x4600, 
		0x5000, 
		0xcf80, 
		0x604c, 
		0x5300, 
		0x6f49, 
		0x1054, 
		0xfc00, 
		0x87d5, 
		0xdbe5, 
		0xd000, 
		0xfcd7, 
		0x4000, 
		0x5040, 
		0xa04a, 
		0xd100, 
		0xca00, 
		0x4b00, 
		0x5380, 
		0xc800, 
		0x4f80, 
		0x74a9, 
		0xc800, 
		0x0000, 
		0x51a0, 
		0x4f40, 
		0x361d, 
		0xd353, 
		0xd1e0, 
		0xc980, 
		0x5bee, 
		0xd300, 
		0x7a61, 
		0xfc00, 
		0x4980, 
		0xfc00, 
		0xbc00, 
		0x12dd, 
		0x0000, 
		0xf45c, 
		0x50a0, 
		0x52a0, 
		0xd200, 
		0x1169, 
		0xd0e0, 
		0x51e0, 
		0xd340, 
		0x1391, 
		0xdd85, 
		0x50a0, 
		0xca00, 
		0x5060, 
		0x5a0b, 
		0x4000, 
		0xd340, 
		0x4c40, 
		0x4700, 
		0x4e80, 
		0xa61f, 
		0xd040, 
		0x971b, 
		0x4d80, 
		0xd100, 
		0xd2e0, 
		0xcdc0, 
		0xe481, 
		0xd340, 
		0xd180, 
		0x7c42, 
		0x0000, 
		0x4200, 
		0xce00, 
		0x1d1b, 
		0x6c85, 
		0xc700, 
		0x8000, 
		0x5c84, 
		0xc800, 
		0xbc1a, 
		0xc980, 
		0x5140, 
		0x9a1e, 
		0xcf40, 
		0xc880, 
		0x8000, 
		0x5000, 
		0xc400, 
		0x71b3, 
		0xca80, 
		0xd0c0, 
		0x53c0, 
		0x4b00, 
		0x4880, 
		0xccc0, 
		0x50a0, 
		0xce40, 
		0xcc80, 
		0xd300, 
		0x9ce4, 
		0x5340, 
		0x50a0, 
		0xa4e9, 
		0xcc40, 
		0xd380, 
		0x8000, 
		0x0d0e, 
		0xd120, 
		0x4200, 
		0x02ee, 
		0xd340, 
		0xcec0, 
		0x9946, 
		0x0000, 
		0x8000, 
		0x02dc, 
		0xf4c8, 
		0xcf80, 
		0x9414, 
		0x6aa6, 
		0x3e5c, 
		0x4a00, 
		0x5000, 
		0x0dd2, 
		0xd3e0, 
		0xc700, 
		0x1ed4, 
		0xd160, 
		0xcd40, 
		0x223d, 
		0xd1a0, 
		0xd220, 
		0x5120, 
		0x8000, 
		0x69e6, 
		0x4500, 
		0xc880, 
		0xd120, 
		0xb1f8, 
		0x0000, 
		0x0000, 
		0xd360, 
		0x83ea, 
		0x8f12, 
		0x366e, 
		0xe2e8, 
		0xd3c0, 
		0xc900, 
		0xcd00, 
		0x8000, 
		0x4e40, 
		0x4200, 
		0xd020, 
		0x4b00, 
		0x52e9, 
		0x5340, 
		0x4200, 
		0xc990, 
		0xd3e0, 
		0x448d, 
		0x08a7, 
		0x4a80, 
		0x922c, 
		0x8000, 
		0x4d40, 
		0x5040, 
		0x2f54, 
		0x4980, 
		0x3a8a, 
		0x4900, 
		0xd120, 
		0x5300, 
		0x5bad, 
		0x0000, 
		0xcf80, 
		0xc0a2, 
		0x4b00, 
		0x51a0, 
		0x901b, 
		0x2285, 
		0x7c95, 
		0xd1a0, 
		0x7fca, 
		0x579b, 
		0xc880, 
		0xf30f, 
		0xd160, 
		0x440b, 
		0x8000, 
		0xd380, 
		0xb9a4, 
		0xc600, 
		0x04b0, 
		0x4880, 
		0xd2e7, 
		0xcb80, 
		0xa9d4, 
		0xcfc0, 
		0x5140, 
		0x5e39, 
		0x7c00, 
		0x50c0, 
		0x4cc0, 
		0xc000, 
		0x51a0, 
		0xd100, 
		0xd040, 
		0xd160, 
		0xcf40, 
		0x5140, 
		0x0000, 
		0x52c0, 
		0x5100, 
		0xd231, 
		0xc800, 
		0x8d38, 
		0x4880, 
		0x5280, 
		0x5340, 
		0xcd80, 
		0xd394, 
		0xfc00, 
		0xc600, 
		0xcfc0, 
		0x05c3, 
		0xd020, 
		0x94cd, 
		0xfc00, 
		0xd160, 
		0x7c00, 
		0xd0c0, 
		0xcf40, 
		0xd1a0, 
		0x0e81, 
		0xd300, 
		0x453c, 
		0xc200, 
		0x50e0, 
		0x8000, 
		0xc880, 
		0xd1a0, 
		0x4e00, 
		0x5340, 
		0xc000, 
		0x0000, 
		0xd3e0, 
		0x4200, 
		0x52c0, 
		0x5060, 
		0x4b80, 
		0x2898, 
		0x5260, 
		0x5320, 
		0x0000, 
		0x7c00, 
		0x4c17, 
		0x4d40, 
		0xb295, 
		0xd280, 
		0xce00, 
		0x5200, 
		0x7c00, 
		0x3f90, 
		0x7c00, 
		0x58ef, 
		0x3d5d, 
		0xf2fe, 
		0xc600, 
		0xac81, 
		0xd0e0, 
		0xc800, 
		0x52c0, 
		0x5320, 
		0xcdc0, 
		0x4f00, 
		0x0000, 
		0x4cc0, 
		0x812f, 
		0x4d00, 
		0xd160, 
		0x1052, 
		0x7c00, 
		0xcec0, 
		0xd3c0, 
		0x3d4f, 
		0x5100, 
		0x4c40, 
		0x51a0, 
		0xd240, 
		0xd1e0, 
		0xfc48, 
		0x5380, 
		0xc200, 
		0xeb69, 
		0xd080, 
		0x4980, 
		0x4000, 
		0xcf00, 
		0xcb80, 
		0xcb00, 
		0x5280, 
		0x0000, 
		0x5340, 
		0x50a0, 
		0xd240, 
		0xf51c, 
		0xd300, 
		0xd220, 
		0x5140, 
		0xd000, 
		0xcd80, 
		0x970d, 
		0x4b80, 
		0x911c, 
		0x7c00, 
		0xcd40, 
		0x5120, 
		0xc800, 
		0xd340, 
		0x53a0, 
		0xcd40, 
		0xd2e0, 
		0xd100, 
		0x51bf, 
		0x5020, 
		0x5260, 
		0xd380, 
		0x9bf7, 
		0xd000, 
		0x4b80, 
		0x4d80, 
		0x4000, 
		0x5380, 
		0xe307, 
		0xca00, 
		0x5380, 
		0x495d, 
		0xd1e0, 
		0x672a, 
		0x8fce, 
		0x1f13, 
		0xc800, 
		0x2066, 
		0x5280, 
		0x8000, 
		0x4f00, 
		0x5360, 
		0xfc85, 
		0x6c01, 
		0xd2e0, 
		0xcf00, 
		0xcc80, 
		0x1015, 
		0x5260, 
		0x569a, 
		0x4500, 
		0xd360, 
		0x4e80, 
		0x150b, 
		0xd160, 
		0xd000, 
		0x56e1, 
		0x5040, 
		0x52e0, 
		0x0c2e, 
		0xc800, 
		0x423e, 
		0x52c0, 
		0x4d40, 
		0x0000, 
		0x0c55, 
		0x0000, 
		0x15b9, 
		0xf677, 
		0x9639, 
		0x52c0, 
		0xd000, 
		0xd040, 
		0x4e00, 
		0xc880, 
		0x4000, 
		0xc9cd, 
		0x5080, 
		0x4980, 
		0x52a0, 
		0x6c49, 
		0x1ac7, 
		0xf2d6, 
		0xea9a, 
		0x4b83, 
		0xd362, 
		0x3d4b, 
		0x8310, 
		0xc000, 
		0x5320, 
		0xd320, 
		0x0df3, 
		0xca80, 
		0xfc37, 
		0xd300, 
		0x8000, 
		0xc600, 
		0xd2c0, 
		0x1fa6, 
		0x41b8, 
		0x7320, 
		0xcaf1, 
		0xc980, 
		0x5380, 
		0xfc00, 
		0xcd40, 
		0xa297, 
		0x8000, 
		0x4cc0, 
		0x4980, 
		0xe608, 
		0xd080, 
		0x4c80, 
		0x2074, 
		0xabc9, 
		0x0000, 
		0xe696, 
		0x5140, 
		0xd83f, 
		0x52a0, 
		0x6e68, 
		0xc500, 
		0x4b80, 
		0xd212, 
		0x4c80, 
		0xcd80, 
		0x05d3, 
		0x6e6a, 
		0x5120, 
		0x4880, 
		0x52c0, 
		0xdde0, 
		0x3ca9, 
		0x5280, 
		0x5140, 
		0xd1a0, 
		0x4900, 
		0xd380, 
		0xc900, 
		0xadd8, 
		0x4800, 
		0xd3a0, 
		0x93e8, 
		0x85cb, 
		0xccc0, 
		0xc900, 
		0x7c00, 
		0xa3c7, 
		0x50a0, 
		0xd120, 
		0xcec0, 
		0xaa75, 
		0x52e0, 
		0x4000, 
		0x0000, 
		0x5020, 
		0x52e0, 
		0xcfc0, 
		0x51e0, 
		0xcd00, 
		0xd060, 
		0x5340, 
		0x7c8f, 
		0x8000, 
		0xd260, 
		0x53c0, 
		0xd380, 
		0x940d, 
		0x9445, 
		0x2415, 
		0xfc00, 
		0x8000, 
		0x7a6b, 
		0xd3c0, 
		0x4e40, 
		0xe9f8, 
		0x214c, 
		0x59ee, 
		0x5260, 
		0x5320, 
		0x51a0, 
		0x4274, 
		0x8450, 
		0x5180, 
		0x5220, 
		0xfd45, 
		0x5140, 
		0x4d40, 
		0xc248, 
		0x51c0, 
		0x3c00, 
		0xd020, 
		0xd3c0, 
		0xcc80, 
		0xd020, 
		0xc800, 
		0x8000, 
		0x2e4c, 
		0x50a0, 
		0xc980, 
		0xe3f8, 
		0xcb00, 
		0xd0a0, 
		0x4b80, 
		0x5260, 
		0xcf80, 
		0x0000, 
		0xd0e0, 
		0x50c0, 
		0xc600, 
		0x169f, 
		0xce40, 
		0xd200, 
		0x4400, 
		0xcc80, 
		0x16f5, 
		0x340c, 
		0xdcd4, 
		0xd320, 
		0xd0c0, 
		0xd0a0, 
		0x4b00, 
		0xc200, 
		0x53a0, 
		0x0bf1, 
		0x7cd6, 
		0xc400, 
		0xa0d6, 
		0x4ec0, 
		0x6565, 
		0x4400, 
		0xcd00, 
		0x2d0c, 
		0x2824, 
		0x5240, 
		0x4b80, 
		0x580c, 
		0xd1c0, 
		0xfeae, 
		0x0000, 
		0xf2cd, 
		0xd380, 
		0x4d00, 
		0x52c0, 
		0xc980, 
		0xccc0, 
		0x9732, 
		0x50e0, 
		0xcfc0, 
		0x7c00, 
		0xc739, 
		0x50a0, 
		0x4c40, 
		0x8000, 
		0x5380, 
		0xbe09, 
		0xf28e, 
		0x6d74, 
		0xcc40, 
		0xe0de, 
		0xa1c4, 
		0xce80, 
		0xd240  
};
volatile uint32_t rand_ieee754_32[1536] __attribute__ ((aligned (2048))) = {
		0xcb000000, 
		0x13a5d6b2, 
		0xc1a00000, 
		0x0f7ffffc, 
		0x0e0001ff, 
		0x42500000, 
		0xc0a00000, 
		0x0f7ffffd, 
		0x7f800001, 
		0xaa90ec24, 
		0x733f7e24, 
		0x00100000, 
		0xad2085b6, 
		0xffbfffff, 
		0x0a2b64be, 
		0xb2c01b4e, 
		0xc2000000, 
		0x424c0000, 
		0xff800000, 
		0xff7ffffe, 
		0xc0c00000, 
		0xc52d2894, 
		0x42200000, 
		0x54c3ccd3, 
		0x00000002, 
		0xc22c0000, 
		0x80000010, 
		0x80000100, 
		0x0e7fffff, 
		0x137bfac7, 
		0xc2600000, 
		0xe613f9cf, 
		0x80ffffff, 
		0x0e000003, 
		0x41b00000, 
		0x3f800001, 
		0x00000000, 
		0x80000400, 
		0x0e07ffff, 
		0x7cad7d99, 
		0xc2380000, 
		0xc2600000, 
		0x41000000, 
		0xf94a1cce, 
		0x807ffffe, 
		0xe24c1d8d, 
		0x706c8baf, 
		0x21fcb4dc, 
		0x17c44e14, 
		0x7d300964, 
		0xc1a80000, 
		0x41400000, 
		0x13a9e7a6, 
		0xaa1a3ad1, 
		0xf1676e91, 
		0x00000800, 
		0x00000080, 
		0xff800000, 
		0x00000000, 
		0x80000200, 
		0x80000000, 
		0xdfd02501, 
		0x40a00000, 
		0x0e01ffff, 
		0x9c004965, 
		0x0cb8fa1a, 
		0xc26c0000, 
		0x42540000, 
		0xc453ff5a, 
		0x80000000, 
		0xffd48d7c, 
		0xec4d7a24, 
		0x802d9782, 
		0x00000020, 
		0xc1200000, 
		0x16d0d142, 
		0x80800002, 
		0xc26c0000, 
		0x3c071256, 
		0xd0be79bf, 
		0xfd4b6cf5, 
		0x7b60d883, 
		0xc1900000, 
		0x00000000, 
		0x80000020, 
		0x00800001, 
		0x0c600000, 
		0xc1d00000, 
		0x65938618, 
		0xc2200000, 
		0x00000000, 
		0x0c7e0000, 
		0x42680000, 
		0xe1476d0f, 
		0xff000000, 
		0x0e00007f, 
		0x00000000, 
		0x0d000ff0, 
		0x00000008, 
		0x80000200, 
		0x425c0000, 
		0xa67933e5, 
		0x41980000, 
		0xc1100000, 
		0x31f8f436, 
		0x7f9a05ea, 
		0x41c00000, 
		0x2d11ee6f, 
		0x00ad0844, 
		0x40400000, 
		0x8f1f9612, 
		0x0d000ff0, 
		0x0e0fffff, 
		0x9a6a3229, 
		0x39d153ca, 
		0x00000000, 
		0xc2540000, 
		0x0c7fffc0, 
		0x8045f5a7, 
		0xc1c80000, 
		0xa250f298, 
		0x80000000, 
		0x30b35734, 
		0x00004000, 
		0x80800001, 
		0x4b000000, 
		0x307ba541, 
		0x41a00000, 
		0xbf800001, 
		0x80011111, 
		0x80480beb, 
		0xc1880000, 
		0xc0c00000, 
		0x054822c9, 
		0x41880000, 
		0x80000000, 
		0x420c0000, 
		0x00000040, 
		0xff800001, 
		0x30e6681a, 
		0x427c0000, 
		0x807ffffe, 
		0xc20c0000, 
		0x00800001, 
		0x00000010, 
		0x00000002, 
		0x00800002, 
		0x41200000, 
		0xcb000000, 
		0xc089088f, 
		0x00000000, 
		0xc0800000, 
		0x00000000, 
		0x239b3149, 
		0x1aaa0035, 
		0x80000000, 
		0xc2680000, 
		0x41d80000, 
		0x00000010, 
		0x00000000, 
		0x0c7f8000, 
		0x0c7ffff8, 
		0xc2040000, 
		0x2cfc0ba5, 
		0xc47727a8, 
		0xc1e80000, 
		0x786384e3, 
		0x7f800000, 
		0xb8e711a1, 
		0xe8a30f02, 
		0x0e1fffff, 
		0xc1700000, 
		0x1b38104d, 
		0x0c7fe000, 
		0xc2280000, 
		0x40e00000, 
		0x0f7ffffc, 
		0xc2200000, 
		0x75ce89df, 
		0x807fffff, 
		0xa4048fc2, 
		0xc2440000, 
		0x80800000, 
		0x0d96e29b, 
		0xc2700000, 
		0xff82b3b4, 
		0xff000000, 
		0xc2180000, 
		0x80020000, 
		0x80006bcf, 
		0x0e0000ff, 
		0x80000400, 
		0x80000000, 
		0x7fab1a10, 
		0x0c700000, 
		0x09f1daba, 
		0xc24c0000, 
		0xa01235b3, 
		0xc2680000, 
		0xa50a9096, 
		0x6fb6c87d, 
		0x4b000000, 
		0x21e00a32, 
		0xff800000, 
		0x8f7ffffc, 
		0x00800002, 
		0x80000001, 
		0xdd23bb49, 
		0x00020000, 
		0x4de023a6, 
		0x807fffff, 
		0x41d80000, 
		0x41800000, 
		0xbf028f5c, 
		0x9d891f8c, 
		0x3f800000, 
		0x8018b554, 
		0x807fffff, 
		0x0c7fff80, 
		0x7f800000, 
		0xc1980000, 
		0x80ffffff, 
		0x41c80000, 
		0x42180000, 
		0x42500000, 
		0x0b0c45a1, 
		0x80000000, 
		0xc0a00000, 
		0xbf800001, 
		0xc2740000, 
		0xff7fffff, 
		0x42580000, 
		0x42040000, 
		0x7f800000, 
		0xbf028f5c, 
		0x6f48c7ca, 
		0x7fffd6b2, 
		0xe099b505, 
		0xc1000000, 
		0xc2680000, 
		0x8f7fffff, 
		0xff800000, 
		0x0fcf0b92, 
		0xb257b6e6, 
		0xff800001, 
		0x7f800000, 
		0x7f8b4ab8, 
		0x0e3fffff, 
		0x80073821, 
		0x40a00000, 
		0x80100000, 
		0xc3d2c310, 
		0x4e22b931, 
		0xc1700000, 
		0x0e00001f, 
		0xc1900000, 
		0x41400000, 
		0x80800001, 
		0x00020000, 
		0xffc00000, 
		0xc1300000, 
		0xce431db1, 
		0x80000010, 
		0x42280000, 
		0x80000000, 
		0x007ffffe, 
		0x00800002, 
		0x1beceaf9, 
		0x80000400, 
		0xc46eb518, 
		0x00000002, 
		0x7f800000, 
		0x42000000, 
		0x42300000, 
		0x00000001, 
		0x9364b01f, 
		0xc2780000, 
		0x8c2beb53, 
		0xae89ef13, 
		0x31131ff1, 
		0x0e1fffff, 
		0x00800000, 
		0x80000000, 
		0x00000001, 
		0x0c7fffff, 
		0x80004000, 
		0xc1c00000, 
		0x421c0000, 
		0x0c7ffff8, 
		0x80800001, 
		0x80080000, 
		0x00008000, 
		0xc21c0000, 
		0xf29f90ae, 
		0x41900000, 
		0xc1d80000, 
		0x41d80000, 
		0xeeaf6211, 
		0xc0400000, 
		0xc2700000, 
		0x0e00003f, 
		0x0c7fff00, 
		0x41900000, 
		0x7f7ffffe, 
		0x7897d250, 
		0xff7fffff, 
		0x41b80000, 
		0xaf957017, 
		0x00040000, 
		0x55555555, 
		0xc1200000, 
		0x0c7ffffc, 
		0xc1f00000, 
		0xffad8833, 
		0xb01d66b2, 
		0xc1400000, 
		0x029688ed, 
		0x58bfb5ff, 
		0x00000200, 
		0x00800001, 
		0x7f800001, 
		0x7f800000, 
		0xc1200000, 
		0xd03ed47c, 
		0x00080000, 
		0x007f34cf, 
		0x31da2e09, 
		0xd5be613e, 
		0x42540000, 
		0x00000020, 
		0x7fc00000, 
		0x80000000, 
		0x001791e0, 
		0xff800000, 
		0xc1880000, 
		0xb788c69c, 
		0x5a6bbacd, 
		0x93ce1e79, 
		0x0f7fffff, 
		0xffbb3839, 
		0xc1300000, 
		0xff7ffffe, 
		0xe2f37e3d, 
		0xbf800000, 
		0xff7ffffe, 
		0xc0bc2fcb, 
		0x0e00ffff, 
		0x4e4a58a0, 
		0x007fffff, 
		0x3c2cee5d, 
		0x41b00000, 
		0x7f800000, 
		0x0d00fff0, 
		0xffcb8e3b, 
		0xc36e7cb5, 
		0xc2480000, 
		0x00000020, 
		0x80000400, 
		0x7f7fffff, 
		0xffc00000, 
		0xba399894, 
		0xd451870a, 
		0x0d00fff0, 
		0xc1980000, 
		0x00000000, 
		0xc21c0000, 
		0x0e000007, 
		0xc0800000, 
		0xc1f80000, 
		0x80800000, 
		0x7f000000, 
		0x41880000, 
		0xbcd1f87d, 
		0xe8ea9496, 
		0x7f7ffffe, 
		0x0d00fff0, 
		0xc2400000, 
		0x41800000, 
		0x0e0003ff, 
		0x1475b4ed, 
		0xa636f44e, 
		0x7f800000, 
		0x0c7ffffc, 
		0x80011111, 
		0x7f800000, 
		0x42540000, 
		0x6b6512ab, 
		0x41b00000, 
		0x0054ac8a, 
		0x00011111, 
		0xc2180000, 
		0xfd1680c2, 
		0x00800003, 
		0xe985183a, 
		0xff7ffffe, 
		0x40e00000, 
		0x80004000, 
		0x001f7bb5, 
		0x57855519, 
		0xc9face2a, 
		0x80011111, 
		0x0b9a10f5, 
		0xc26c0000, 
		0x80200000, 
		0xe513dcc6, 
		0xff800000, 
		0xc1700000, 
		0x7fbfffff, 
		0x3614e58b, 
		0x80000000, 
		0x57963abd, 
		0x0f7ffffe, 
		0x97083ef0, 
		0xb0d9a11e, 
		0xc2100000, 
		0x3f800000, 
		0xc21c0000, 
		0xc1c80000, 
		0x00000800, 
		0x3f028f5c, 
		0x80000100, 
		0x7fb35f82, 
		0x41e00000, 
		0x00000200, 
		0x80000000, 
		0x42540000, 
		0x423c0000, 
		0x0c7c0000, 
		0xcb000000, 
		0x41880000, 
		0x00010000, 
		0x80800000, 
		0x0e003fff, 
		0x0c7fe000, 
		0xc1900000, 
		0xff7ffffe, 
		0x41300000, 
		0xcec44853, 
		0x0c780000, 
		0xc2700000, 
		0x0c7ffffc, 
		0x82749db9, 
		0xbbbaf52a, 
		0x00800000, 
		0x423c0000, 
		0x42400000, 
		0xff800000, 
		0x7fffffff, 
		0x41a80000, 
		0x41900000, 
		0xffdeffbe, 
		0x77b186b1, 
		0xc2600000, 
		0xa81b2b8e, 
		0x00400000, 
		0xd00fbf4b, 
		0xff800000, 
		0xaf746ad4, 
		0x0e00000f, 
		0xb9828f05, 
		0x7f800000, 
		0x00000002, 
		0xe6c05ba0, 
		0x53a4ea8d, 
		0x00000004, 
		0x0c7e0000, 
		0x0c600000, 
		0x80011111, 
		0xdd1a860a, 
		0xff800000, 
		0xbf800001, 
		0x146ebc41, 
		0x40a00000, 
		0xc2640000, 
		0x42140000, 
		0x7f800000, 
		0x805f661e, 
		0x01681a53, 
		0xead9d0b7, 
		0xc2600000, 
		0xb621dc60, 
		0x168b08b2, 
		0xc1300000, 
		0xc24c0000, 
		0x40800000, 
		0x00400000, 
		0x0f7ffffd, 
		0x1e8045cd, 
		0xc1b00000, 
		0x80800001, 
		0xc1bb8f71, 
		0xffc00000, 
		0x7fbdd6a7, 
		0x80000000, 
		0x80000000, 
		0xc2000000, 
		0xe368b553, 
		0xff000000, 
		0x40400000, 
		0x7f800000, 
		0xc0e00000, 
		0xffbfffff, 
		0xd15d39e3, 
		0x003a0b35, 
		0x40e00000, 
		0xc1000000, 
		0x7fba9cfe, 
		0x00053ad9, 
		0x42100000, 
		0xc2480000, 
		0x0e000fff, 
		0x00000000, 
		0x0c7ffc00, 
		0xd315a984, 
		0x7f7ffffe, 
		0x00000000, 
		0xc9c0b8f4, 
		0x0f7ffffe, 
		0x0f7ffffe, 
		0xb76e7613, 
		0x0c780000, 
		0xf993896b, 
		0xe32c50b7, 
		0x0c700000, 
		0xc2140000, 
		0xa538786e, 
		0x5e060354, 
		0x0c31f5b8, 
		0x00000000, 
		0x0f7ffffd, 
		0xff800000, 
		0xff7fffff, 
		0x00000100, 
		0x7f800000, 
		0xfe60a5ac, 
		0x3f028f5c, 
		0xffc00001, 
		0xf38f92fb, 
		0x80008000, 
		0xc2740000, 
		0x682346d0, 
		0x00020000, 
		0xc1e00000, 
		0x0e01ffff, 
		0x55555555, 
		0x0f7ffffd, 
		0xcb8c4b40, 
		0x6dcbec65, 
		0x00000000, 
		0x4036969f, 
		0xc1b80000, 
		0xc1500000, 
		0xc2580000, 
		0x339f20e2, 
		0x80000000, 
		0x80000000, 
		0xe74647f0, 
		0x0c7e0000, 
		0x40a00000, 
		0x41a80000, 
		0x94b44af3, 
		0x68b17e55, 
		0x9b7aaac4, 
		0x0c7fc000, 
		0xe2f76808, 
		0xc0a00000, 
		0x04b99b60, 
		0x36e2532c, 
		0x9721cc2b, 
		0x00002000, 
		0xf079790e, 
		0xb29529c9, 
		0xc1d00000, 
		0x7f7fffff, 
		0xdf6feb01, 
		0xa26d70f3, 
		0x0c7fffc0, 
		0x00800000, 
		0x80000000, 
		0xff800000, 
		0x80001000, 
		0x42400000, 
		0x41700000, 
		0xc21c0000, 
		0x0c7ffff8, 
		0x7f800000, 
		0x0625fada, 
		0xc2600000, 
		0xff000000, 
		0xc2400000, 
		0x1b337464, 
		0x00080000, 
		0x7f800000, 
		0x00000000, 
		0x80000000, 
		0xbf800000, 
		0x3a2ebd47, 
		0x00000002, 
		0xc2300000, 
		0x41000000, 
		0x897d0165, 
		0xc2580000, 
		0x80000000, 
		0x17cb3ce9, 
		0xe3d1177b, 
		0x00ffffff, 
		0x00800001, 
		0x423c0000, 
		0xcc63d745, 
		0x7fffffff, 
		0x0e00003f, 
		0xffc00000, 
		0x7f800000, 
		0x80000000, 
		0x0e00003f, 
		0x0c7fe000, 
		0x0c7ffffc, 
		0xc2180000, 
		0x44427aee, 
		0x173a0afc, 
		0xbc17a2f5, 
		0xff800000, 
		0xd63259ac, 
		0x8067a67b, 
		0xff800000, 
		0xc22c0000, 
		0x80000010, 
		0x42640000, 
		0xc2100000, 
		0x9db70d8e, 
		0x80100000, 
		0xc1700000, 
		0x42040000, 
		0x7f800000, 
		0x80000000, 
		0x80800001, 
		0x1b8e862a, 
		0x7f800000, 
		0xff800000, 
		0x00000000, 
		0x42740000, 
		0x46bb88e0, 
		0x00100000, 
		0x80800001, 
		0xcccccccc, 
		0x422c0000, 
		0x628be019, 
		0xb36fe1b4, 
		0xc1e80000, 
		0xc2300000, 
		0x00000000, 
		0xb37b92cb, 
		0xf43ac36b, 
		0xc2240000, 
		0x7f7fffff, 
		0xae5d7aae, 
		0x41600000, 
		0xe4f19557, 
		0x057028bb, 
		0xc2140000, 
		0xff7fffff, 
		0xc1400000, 
		0xc1d80000, 
		0x3d53869c, 
		0x0c7fffff, 
		0x5e85ff94, 
		0x199f3b3a, 
		0x5fdd6b1b, 
		0x80000000, 
		0xc0000000, 
		0x01610de1, 
		0x423c0000, 
		0xa8728667, 
		0x42580000, 
		0x00000000, 
		0x80000002, 
		0x00000000, 
		0xc1a80000, 
		0xc1b00000, 
		0x0c7f0000, 
		0xffd33400, 
		0x02421144, 
		0x0e0000ff, 
		0xebc1686c, 
		0x0c7ffffc, 
		0x00000000, 
		0xc1400000, 
		0x84a395b1, 
		0x423091fa, 
		0x7422b4ac, 
		0x7fb4f5e7, 
		0xc21c0000, 
		0x80000040, 
		0xc1c80000, 
		0x80800001, 
		0x80000000, 
		0x3a414197, 
		0x0e000007, 
		0x0c7ff800, 
		0x27cc9ee1, 
		0x0c7c0000, 
		0x0c7ffffc, 
		0x0f7ffffe, 
		0x80000001, 
		0x00000800, 
		0xff000000, 
		0x55555555, 
		0x80800001, 
		0xff89963e, 
		0x42180000, 
		0x00800001, 
		0x4b000000, 
		0x1e6b7fda, 
		0x80000020, 
		0x80040000, 
		0x40000000, 
		0x01b0b52f, 
		0xc1e80000, 
		0x0c7ff000, 
		0x9b656fbd, 
		0x42200000, 
		0xbb6355ad, 
		0xff000000, 
		0xc2640000, 
		0x0f7ffffc, 
		0xc1400000, 
		0x7f800001, 
		0x41f00000, 
		0x00020000, 
		0x41400000, 
		0xc1f80000, 
		0xc2700000, 
		0x46a13da6, 
		0x00800003, 
		0x0e0001ff, 
		0x80000000, 
		0x00000000, 
		0x974e355a, 
		0x0c7f0000, 
		0xff800000, 
		0x425c0000, 
		0xff7fffff, 
		0xff800000, 
		0x00400000, 
		0x33ee324c, 
		0xc2000000, 
		0x0e0003ff, 
		0xa9fef71b, 
		0x1e241a6a, 
		0x90e43d15, 
		0xc1f80000, 
		0x0e00ffff, 
		0x4b8c4b40, 
		0x42780000, 
		0x80800003, 
		0x0c151d13, 
		0xb340bf36, 
		0xc0e00000, 
		0xc2140000, 
		0x00000400, 
		0xcccccccc, 
		0x00000000, 
		0x5ffbc5b8, 
		0x80000200, 
		0x80696bcf, 
		0x00200000, 
		0x422c0000, 
		0xbf028f5c, 
		0x42300000, 
		0xc891766a, 
		0x7f800000, 
		0x80800002, 
		0x41200000, 
		0x007fffff, 
		0x0f7ffffc, 
		0xa46f2e59, 
		0x9164b8df, 
		0xc20c0000, 
		0x80000008, 
		0x80800001, 
		0xe34b0aef, 
		0x40c00000, 
		0xc7fa8a49, 
		0x0062ecf5, 
		0x7fab2489, 
		0xa66d8500, 
		0x7f800000, 
		0x00800002, 
		0x0e0000ff, 
		0xcb8c4b40, 
		0xc1500000, 
		0x80ffffff, 
		0x983e0c1b, 
		0x0006ca81, 
		0x2079f77c, 
		0xa8c6b8f3, 
		0x00000000, 
		0x80800000, 
		0x0c780000, 
		0x84910643, 
		0xffab74e0, 
		0xc1900000, 
		0x41b00000, 
		0x8f3b1e83, 
		0x0c7ffc00, 
		0x00000000, 
		0x0b12bbd6, 
		0xd09822dc, 
		0xdf30f246, 
		0xff7ffffe, 
		0xff94ebc8, 
		0x55555555, 
		0x7fc00001, 
		0xc1a80000, 
		0x425c0000, 
		0x7b807c94, 
		0x41600000, 
		0x423c0000, 
		0x80100000, 
		0x0e0003ff, 
		0x807437af, 
		0xd2dd5462, 
		0x00800003, 
		0x0f833865, 
		0x004049a6, 
		0x80000000, 
		0xc8d798d1, 
		0x80040000, 
		0xe82ed08c, 
		0xc23c0000, 
		0xf3932370, 
		0xc1000000, 
		0x0e3fffff, 
		0x80000010, 
		0x0e000001, 
		0x7f96ee76, 
		0x42440000, 
		0x22471940, 
		0x80000020, 
		0x4b8c4b40, 
		0xc376505f, 
		0xc2680000, 
		0xff800000, 
		0xa9a762d6, 
		0x5dc7de88, 
		0x7f7ffffe, 
		0xb0513ce4, 
		0x0e003fff, 
		0x0c7ffff0, 
		0x422c0000, 
		0xff7ffffe, 
		0xc765cecd, 
		0xc1200000, 
		0x24735e55, 
		0x9f8ff23e, 
		0xe82af570, 
		0x0e7fffff, 
		0x007fffff, 
		0x00000000, 
		0x426c0000, 
		0xf63bfe1d, 
		0x6a7b3822, 
		0x41500000, 
		0x4b8c4b40, 
		0x0e00ffff, 
		0x42340000, 
		0xa8e3684e, 
		0x40000000, 
		0x42040000, 
		0x6074c723, 
		0x41600000, 
		0x7faf6618, 
		0xffb9ee7a, 
		0x0005e2c7, 
		0x1d94a637, 
		0xcb8c4b40, 
		0xff800000, 
		0xc1880000, 
		0x0e003fff, 
		0xff7ffffe, 
		0x00800000, 
		0xc0a00000, 
		0xaa11ccec, 
		0xbd8a3726, 
		0xc26c0000, 
		0xff000000, 
		0x084ccb3a, 
		0x0c7fe000, 
		0x00000000, 
		0xb7a65c9f, 
		0xf7d22d06, 
		0x0c780000, 
		0x0c7ffffc, 
		0x80100000, 
		0x80000000, 
		0x7f800000, 
		0xc1100000, 
		0xff7ffffe, 
		0x00000200, 
		0x7f7fffff, 
		0x55555555, 
		0x0e07ffff, 
		0x807ffffe, 
		0x73d061c7, 
		0x00002000, 
		0x40400000, 
		0xd86bdffc, 
		0xbf028f5c, 
		0xc1700000, 
		0x81c4097d, 
		0xc2580000, 
		0x0f7ffffd, 
		0x80000000, 
		0x42380000, 
		0xb1ad36ae, 
		0x019f811a, 
		0x7fa38413, 
		0xc1000000, 
		0x42040000, 
		0x64dc8155, 
		0xe7e9febb, 
		0xc1a80000, 
		0xc1a80000, 
		0x40c00000, 
		0x80000000, 
		0xffee49de, 
		0x00000080, 
		0x7f800000, 
		0xff7ffffe, 
		0x00000000, 
		0xbf800000, 
		0x12e02634, 
		0xc2200000, 
		0xbf028f5c, 
		0x00400000, 
		0xff800000, 
		0xff931b17, 
		0x8fc45881, 
		0xff800000, 
		0x42100000, 
		0x298055a1, 
		0x0c7fff00, 
		0xffffffff, 
		0x0502adb4, 
		0x7f9994eb, 
		0xb5692bdc, 
		0xc1f00000, 
		0x0e003fff, 
		0x42680000, 
		0x1072ac39, 
		0xc2240000, 
		0x0e00ffff, 
		0x00020000, 
		0xb46a316f, 
		0x0c7ffffe, 
		0x0e00007f, 
		0x0d000ff0, 
		0xc27c0000, 
		0x80008000, 
		0xc48e5696, 
		0x1e333204, 
		0x80000080, 
		0xd394304b, 
		0x0e000007, 
		0xfb3295b4, 
		0x0e001fff, 
		0x41a00000, 
		0xff800000, 
		0x8f7ffffc, 
		0x00400000, 
		0x00000002, 
		0xffc00000, 
		0xcb000000, 
		0xc25c0000, 
		0xc1900000, 
		0x00800002, 
		0xe4cf71c5, 
		0xff800000, 
		0x00800003, 
		0x41800000, 
		0x00000008, 
		0xc1c80000, 
		0x80800000, 
		0x00000000, 
		0x0c7fff80, 
		0x80000000, 
		0xc2240000, 
		0x80100000, 
		0xb23d3ad8, 
		0x0e03ffff, 
		0xc1700000, 
		0x80000000, 
		0x41e00000, 
		0x540e1833, 
		0xd0e013cc, 
		0x427c0000, 
		0xdd8369cb, 
		0xff800001, 
		0x42340000, 
		0x7fe3bb27, 
		0xc2140000, 
		0xb493c188, 
		0xc2240000, 
		0xc2140000, 
		0xdd6827ce, 
		0x00800000, 
		0x80800003, 
		0xc2100000, 
		0x80800003, 
		0xff967499, 
		0xc1300000, 
		0xc2480000, 
		0x30f8c991, 
		0xff800000, 
		0xff7fffff, 
		0x7f800000, 
		0x41980000, 
		0xff7ffffe, 
		0x00800003, 
		0x41e00000, 
		0xf46356e1, 
		0x32b01c14, 
		0x80000001, 
		0x80000000, 
		0x00000000, 
		0x4b000000, 
		0xc1400000, 
		0x00ffffff, 
		0x0e3fffff, 
		0x41c00000, 
		0xff800000, 
		0xff800000, 
		0x68f2efb8, 
		0x42680000, 
		0x80000002, 
		0xc2640000, 
		0x5d840e89, 
		0xff800000, 
		0x41700000, 
		0x0e00003f, 
		0x0c7e0000, 
		0xff800000, 
		0xff7fffff, 
		0x80000000, 
		0x7fd86d6a, 
		0x80000000, 
		0x80040000, 
		0x00800000, 
		0x6e68c182, 
		0x41c80000, 
		0x7f800000, 
		0xff7ffffe, 
		0x80002000, 
		0x80b773ef, 
		0xc2580000, 
		0x0e00ffff, 
		0xc2640000, 
		0x42680000, 
		0x80800000, 
		0x8f7ffffc, 
		0x41a80000, 
		0x33333333, 
		0xffc00001, 
		0xff82a3e7, 
		0x00000400, 
		0xd4ffb1f1, 
		0x7fa6b3e6, 
		0xff90fb26, 
		0xaaaaaaaa, 
		0x41880000, 
		0x7fb0e7b9, 
		0x00200000, 
		0x2f848d09, 
		0x80000200, 
		0x80800003, 
		0xbab30955, 
		0x8f7ffffe, 
		0x80040000, 
		0x80000000, 
		0xbf028f5c, 
		0x7f800000, 
		0x40e00000, 
		0x7d319d21, 
		0x7fc00000, 
		0x42380000, 
		0xc1400000, 
		0x7fc00001, 
		0x42240000, 
		0x80000000, 
		0x42500000, 
		0x7f000000, 
		0x80000000, 
		0x41c80000, 
		0x16f514d7, 
		0x00000004, 
		0x00020000, 
		0x80000000, 
		0x80000020, 
		0x80000000, 
		0xc27c0000, 
		0x00000000, 
		0xffbfb13f, 
		0xc2380000, 
		0xc2540000, 
		0x42580000, 
		0xc25c0000, 
		0x55555555, 
		0x7f800000, 
		0x00000100, 
		0x0c7fff80, 
		0xfab99857, 
		0x41500000, 
		0x80800002, 
		0x80000001, 
		0xc0c00000, 
		0x0e000001, 
		0x41400000, 
		0xb3e08d06, 
		0x00400000, 
		0x424c0000, 
		0xc1719aa0, 
		0x0c7fffff, 
		0x0e000007, 
		0x00000800, 
		0xff7fffff, 
		0xdcb9b844, 
		0x00800000, 
		0x2deb2398, 
		0x80000020, 
		0xc27c0000, 
		0x00000000, 
		0x427c0000, 
		0x7f800000, 
		0x80020000, 
		0xff800001, 
		0x80225ec9, 
		0xc2600000, 
		0x41f80000, 
		0x7f7fffff, 
		0x42780000, 
		0xc33241d5, 
		0xff800000, 
		0x426c0000, 
		0x4743f2f6, 
		0x7fba02f8, 
		0x0c400000, 
		0x0c7fc000, 
		0xff800000, 
		0xbeb9cb4c, 
		0xc2600000, 
		0x7bc40681, 
		0x0f7ffffc, 
		0xeab26a33, 
		0x40b0d843, 
		0x473acdaf, 
		0x80000001, 
		0x0e00000f, 
		0xaf0f36bb, 
		0x00000000, 
		0x80000020, 
		0xc0000000, 
		0xc0c00000, 
		0x8f7ffffc, 
		0x2452ed6b, 
		0xaaaaaaaa, 
		0xf7511809, 
		0x42340000, 
		0x59665fa8, 
		0xc22c0000, 
		0x00000000, 
		0x80004000, 
		0x0c7ffe00, 
		0x42440000, 
		0xc1900000, 
		0xc2580000, 
		0xff800000, 
		0xc1f00000, 
		0x0c7ff800, 
		0x80000002, 
		0x00011111, 
		0x7f804be3, 
		0xff7ffffe, 
		0x00800001, 
		0xc2640000, 
		0xff800000, 
		0x7f800000, 
		0x80000000, 
		0x28762ad0, 
		0xff800000, 
		0x78e7a69d, 
		0x00000200, 
		0x1046ecd6, 
		0xffc85f06, 
		0x41d00000, 
		0xcc57578d, 
		0x6e1de116, 
		0x41c00000, 
		0x00010000, 
		0x80040000, 
		0xa1f92a38, 
		0x55adee5c, 
		0xffe9cf86, 
		0xc20c0000, 
		0x7f7fffff, 
		0x7f800001, 
		0xc1700000, 
		0x00000002, 
		0xe07ba312, 
		0x42440000, 
		0xc1d80000, 
		0x42280000, 
		0xbbee989d, 
		0x0c7fff80, 
		0xc1c00000, 
		0xd1041554, 
		0x80000001, 
		0x7f800000, 
		0x7fa326d0, 
		0xff800000, 
		0x42580000, 
		0x007ffffe, 
		0xf3c74e29, 
		0x00800000, 
		0x00000000, 
		0x42080000, 
		0x42100000, 
		0x42580000, 
		0xa52ebee4, 
		0x0c7ff800, 
		0x42080000, 
		0x0e03ffff, 
		0x80040000, 
		0xbf800000, 
		0x40e00000, 
		0xc2280000, 
		0x80000020, 
		0x7fffffff, 
		0xa186de9f, 
		0xff7fffff, 
		0xace3e512, 
		0xff7ffffe, 
		0xff800001, 
		0x00020000, 
		0xdc786673, 
		0x0009b4bd, 
		0x00ffffff, 
		0x0e003fff, 
		0x0ee683ad, 
		0xba64167e, 
		0x41d00000, 
		0x44af40c9, 
		0x80000001, 
		0x0c400000, 
		0xcb000000, 
		0xff7fffff, 
		0x80000001, 
		0x42140000, 
		0xb127c333, 
		0x60ac03cc, 
		0xd17d8c51, 
		0x5a05e20f, 
		0x42140000, 
		0x00000000, 
		0x0f7ffffc, 
		0xe43a5585, 
		0x80000400, 
		0x40800000, 
		0x0e000007, 
		0x80010000, 
		0x7f7fffff, 
		0x80040000, 
		0xe4f42eeb, 
		0x80000000, 
		0xc513482d, 
		0xaff9bff6, 
		0x00800000, 
		0xc0a00000, 
		0xc1e80000, 
		0x80000200, 
		0xc1b80000, 
		0x00000800, 
		0x41200000, 
		0x0e0fffff, 
		0x40a00000, 
		0x00000000, 
		0x42380000, 
		0xc27c0000, 
		0x7f800000, 
		0x37f984e3, 
		0xff7ffffe, 
		0x7f80c9d2, 
		0x7f7ffffe, 
		0x80000008, 
		0x0d000ff0, 
		0x0f7ffffc, 
		0xff7ffffe, 
		0x0c7ffffe, 
		0xa30d6d33, 
		0x80000000, 
		0x0c7f8000, 
		0x00800003, 
		0x80800001, 
		0xc1700000, 
		0x00800000, 
		0x0e000001, 
		0xbf800001, 
		0x80000100, 
		0x00000080, 
		0x0e007fff, 
		0x7fbdc858, 
		0x8cf91b2d, 
		0xc0800000, 
		0x0c7fc000, 
		0x426c0000, 
		0x00ffffff, 
		0x41c80000, 
		0xcb8c4b40, 
		0x420c0000, 
		0x7fada2ef, 
		0x425c0000, 
		0x41e80000, 
		0xaaaaaaaa, 
		0x0c7fffff, 
		0x0c400000, 
		0xc2000000, 
		0xc2040000, 
		0x0e0000ff, 
		0xc2080000, 
		0x00004000, 
		0x00800000, 
		0xff800001, 
		0x7fffffff, 
		0x7f800001, 
		0x7f800000, 
		0x424c0000, 
		0xbf028f5c, 
		0x0e00003f, 
		0x7fffffff, 
		0x7f7ffffe, 
		0x41880000, 
		0x42300000, 
		0x00800000, 
		0x0e03ffff, 
		0x7f7fffff, 
		0x84971446, 
		0x3accfb58, 
		0x6b204af1, 
		0x80000100, 
		0x0c7fffc0, 
		0x42040000, 
		0x80800001, 
		0x0e00001f, 
		0x00020000, 
		0xc1600000, 
		0xffc45691, 
		0x4653a8a2, 
		0xc2440000, 
		0x2256a5fe, 
		0x0e3fffff, 
		0xc1700000, 
		0xc2540000, 
		0x0e0003ff, 
		0x42640000, 
		0x89c50332, 
		0x0e000003, 
		0xc1200000, 
		0x3f800000, 
		0xc2340000, 
		0x00114894, 
		0x3cc4490c, 
		0xc1600000, 
		0x0c7ffffe, 
		0x1715a56a, 
		0x0e000007, 
		0x80800001, 
		0x7fc00000, 
		0x42240000, 
		0x3ac52625, 
		0x00800000, 
		0xfff5def9, 
		0x41c00000, 
		0x00800002, 
		0xa499308c, 
		0x80000100, 
		0x6aa4f3d0, 
		0xffffffff, 
		0x7f7ffffe, 
		0x7f800000, 
		0x41c00000, 
		0xc2780000, 
		0x0c7fffff, 
		0x7f7ffffe, 
		0x00800000, 
		0xc23c0000, 
		0x41d80000, 
		0xc0800000, 
		0x0c55b34c, 
		0x47aaa4bb, 
		0x5be04609, 
		0x7630f8bd, 
		0x1b5d2e7d, 
		0x6f14572c, 
		0x40a00000, 
		0x7fae2a2a, 
		0x0e000fff, 
		0x41300000, 
		0xc2180000, 
		0x220095ce, 
		0x0e01ffff, 
		0x251d622d, 
		0xc6da1e4a, 
		0xeb20c0bb, 
		0xff800000, 
		0x80004000, 
		0x7f800000, 
		0xffab3daa, 
		0x425c0000, 
		0xe8f4beb5, 
		0x40800000, 
		0x0e0000ff, 
		0x42340000, 
		0xc22c0000, 
		0x0e7fffff, 
		0x80800000, 
		0x8f7fffff, 
		0x80080000, 
		0xe941944f, 
		0x80800002, 
		0x807ffffe, 
		0xc1b80000, 
		0x84b0cf13, 
		0x00000000, 
		0xc2780000, 
		0x80000000, 
		0x00000000, 
		0xff000000, 
		0x424c0000, 
		0x0611ab55, 
		0x41200000, 
		0x0c7ffffe, 
		0xc2480000, 
		0x80200000, 
		0x97a5d13e, 
		0x807fffff, 
		0x0c7f8000, 
		0x4104cc60, 
		0xbe5925d7, 
		0x7fc00000, 
		0x41700000, 
		0x8f7fffff, 
		0xd61f72d9, 
		0x0c700000, 
		0x00000000, 
		0x63befa29, 
		0x41d80000, 
		0xff800000, 
		0xffc00001, 
		0xc1d80000, 
		0x426c0000, 
		0x00000004, 
		0x00000000, 
		0x42503b5b, 
		0x40d3a466, 
		0x41e00000, 
		0x00000004, 
		0xf831aa98, 
		0x80100000, 
		0x55555555, 
		0x426c0000, 
		0xe058df4e, 
		0x41800000, 
		0xc1880000, 
		0x4b8c4b40, 
		0xc0a00000, 
		0x0e007fff, 
		0xc1400000, 
		0x1df9dcd3, 
		0x0c7ffffc, 
		0x80001000, 
		0x00000010, 
		0x42240000, 
		0x80002000, 
		0x41200000, 
		0x7f7ffffe, 
		0x75452cf8, 
		0xffe684e2, 
		0xc0ada6d1, 
		0x00000080, 
		0x19f43c60, 
		0x80000001, 
		0xc20c0000, 
		0x3f800000, 
		0x80800000, 
		0x915eaf63, 
		0x40e00000, 
		0xc2780000, 
		0xc25c0000, 
		0x17b4eca1, 
		0x00008000, 
		0x41c00000, 
		0x00000010, 
		0x0c7ff800, 
		0xcba5dc85, 
		0x204814c3, 
		0x7f800000, 
		0x00800002, 
		0xf2336fbf, 
		0xbe7fb5c8, 
		0xc0800000, 
		0xc2440000, 
		0xc0c00000, 
		0x7bebdb65, 
		0xc1a80000, 
		0x5c74747a, 
		0x7f800000, 
		0x80800000, 
		0x80000080, 
		0xc1600000, 
		0x42180000, 
		0xff800000, 
		0x80800002, 
		0x80800002, 
		0x2de4edfe, 
		0x41f00000, 
		0xe8dea4df, 
		0xc22c0000, 
		0xc1800000, 
		0x423c0000, 
		0x7a149db5, 
		0xc5812f39, 
		0x80000080, 
		0xe63efc11, 
		0x7e8dad54, 
		0xc26c0000, 
		0x00000000, 
		0x006e0390  
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
		0x000002b3, 
		0x00000313, 
		0x0000012b, 
		0x000000cd, 
		0x000002b3, 
		0x000002c9, 
		0x000003ff, 
		0x0000025f, 
		0x000000d3, 
		0x00000003, 
		0x000003ad, 
		0x00000263, 
		0x00000201, 
		0x00000355, 
		0x0000031d, 
		0x000003ff, 
		0x000000ed, 
		0x0000031d, 
		0x00000035, 
		0x00000153, 
		0x0000020f, 
		0x000000f3, 
		0x00000161, 
		0x00000319, 
		0x0000000f, 
		0x000000db, 
		0x000003e1, 
		0x00000201, 
		0x00000135, 
		0x0000015d, 
		0x00000117, 
		0x0000016f, 
		0x0000020f, 
		0x00000359, 
		0x00000065, 
		0x0000032b, 
		0x00000213, 
		0x00000147, 
		0x00000095, 
		0x0000027b, 
		0x000000b5, 
		0x000001db, 
		0x000002c3, 
		0x000002e1, 
		0x00000291, 
		0x00000225, 
		0x0000034d, 
		0x00000195, 
		0x0000000d, 
		0x000001db, 
		0x00000241, 
		0x000003cb, 
		0x000001b7, 
		0x000003c3, 
		0x00000327, 
		0x000000cd, 
		0x000000ff, 
		0x0000035b, 
		0x0000017f, 
		0x00000289, 
		0x000001a9, 
		0x00000213, 
		0x000000c9, 
		0x00000339, 
		0x0000039d, 
		0x00000333, 
		0x000003df, 
		0x000000c5, 
		0x000003eb, 
		0x00000047, 
		0x000002af, 
		0x0000022b, 
		0x00000055, 
		0x00000157, 
		0x0000009f, 
		0x00000117, 
		0x000003a9, 
		0x00000291, 
		0x00000351, 
		0x00000165, 
		0x0000032f, 
		0x000002d3, 
		0x0000001b, 
		0x0000011d, 
		0x000000c3, 
		0x000002bf, 
		0x000000ff, 
		0x000000d3, 
		0x0000024f, 
		0x000001cb, 
		0x000002cf, 
		0x000003e9, 
		0x00000269, 
		0x0000027b, 
		0x000002b7, 
		0x000000d7, 
		0x000000d5, 
		0x0000035b, 
		0x00000389, 
		0x0000029f, 
		0x0000035d, 
		0x000001a1, 
		0x000003ef, 
		0x000002bb, 
		0x0000033d, 
		0x00000043, 
		0x00000211, 
		0x00000101, 
		0x0000029f, 
		0x000001f5, 
		0x00000155, 
		0x0000004f, 
		0x000000af, 
		0x00000049, 
		0x000003cd, 
		0x00000149, 
		0x000001ab, 
		0x000000d1, 
		0x00000235, 
		0x00000159, 
		0x000003a9, 
		0x000002fb, 
		0x00000327, 
		0x000003fd, 
		0x000000c1, 
		0x00000039, 
		0x000000f3, 
		0x000001a9, 
		0x000001bb, 
		0x00000369, 
		0x00000051, 
		0x000002fb, 
		0x00000283, 
		0x00000147, 
		0x00000319, 
		0x0000003b, 
		0x0000015d, 
		0x00000113, 
		0x00000307, 
		0x0000014d, 
		0x0000003f, 
		0x00000227, 
		0x00000305, 
		0x000001a9, 
		0x000001dd, 
		0x00000381, 
		0x000001dd, 
		0x000000af, 
		0x00000299, 
		0x0000011d, 
		0x00000337, 
		0x0000012d, 
		0x000002b7, 
		0x00000241, 
		0x000003df, 
		0x000003b7, 
		0x0000019b, 
		0x00000313, 
		0x0000013b, 
		0x000001d9, 
		0x000002bf, 
		0x0000035d, 
		0x00000205, 
		0x000000c5, 
		0x00000201, 
		0x00000235, 
		0x0000031b, 
		0x00000001, 
		0x0000017f, 
		0x00000197, 
		0x000001af, 
		0x0000009b, 
		0x00000303, 
		0x00000115, 
		0x00000179, 
		0x00000097, 
		0x00000037, 
		0x000001fb, 
		0x00000311, 
		0x0000039b, 
		0x000000ad, 
		0x0000000d, 
		0x00000023, 
		0x00000327, 
		0x00000227, 
		0x0000026f, 
		0x00000015, 
		0x000002ab, 
		0x0000015f, 
		0x000000f7, 
		0x00000397, 
		0x00000307, 
		0x000002c7, 
		0x00000163, 
		0x0000021d, 
		0x00000249, 
		0x00000345, 
		0x00000323, 
		0x000002cb, 
		0x0000029d, 
		0x000001ed, 
		0x00000007, 
		0x000002d5, 
		0x00000297, 
		0x00000195, 
		0x00000123, 
		0x000002c1, 
		0x0000008d, 
		0x0000000b, 
		0x00000195, 
		0x000000f7, 
		0x00000127, 
		0x0000028b, 
		0x0000038d, 
		0x0000032f, 
		0x000000bf, 
		0x00000069, 
		0x00000187, 
		0x0000035d, 
		0x0000030b, 
		0x0000004b, 
		0x0000006f, 
		0x0000024f, 
		0x000002a3, 
		0x00000015, 
		0x00000237, 
		0x00000063, 
		0x00000289, 
		0x0000001f, 
		0x00000017, 
		0x000003c3, 
		0x00000163, 
		0x00000341, 
		0x000003fb, 
		0x000003e3, 
		0x0000008f, 
		0x0000037f, 
		0x000002b3, 
		0x00000033, 
		0x0000029b, 
		0x0000024b, 
		0x000001cd, 
		0x00000203, 
		0x0000018b, 
		0x0000027b, 
		0x000000e5, 
		0x0000004f, 
		0x0000014d, 
		0x000002b3, 
		0x00000021, 
		0x00000121, 
		0x0000030d, 
		0x0000007b, 
		0x000000cb, 
		0x000001e5, 
		0x00000197  
};

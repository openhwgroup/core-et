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
			"li x5, 177\n"
			"faddi.pi f2, f0, -82\n"
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
			"frcp.ps f18, f2\n"
			"frcp.ps f19, f3\n"
			"frcp.ps f20, f4\n"
			"frcp.ps f21, f5\n"
			"frcp.ps f22, f6\n"
			"frcp.ps f23, f7\n"
			"frcp.ps f24, f8\n"
			"frcp.ps f25, f9\n"
			"frcp.ps f26, f10\n"
			"frcp.ps f27, f11\n"
			"frcp.ps f28, f12\n"
			"frcp.ps f29, f13\n"
			"frcp.ps f30, f14\n"
			"frcp.ps f31, f15\n"
			"frcp.ps f0, f16\n"
			"frcp.ps f1, f17\n"
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
			"csrwi mhpmevent3, 26\n"   
			"csrwi mhpmevent4, 17\n"   
			"csrwi mhpmevent5, 19\n"   
			"csrwi mhpmevent6, 15\n"   
			"csrwi mhpmevent7, 7\n"   
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
			"flw.ps f3,  576 (x5)\n"
			"flw.ps f29,  416 (x5)\n"
			"flw.ps f30,  800 (x5)\n"
			"flw.ps f10,  32 (x5)\n"
			"flw.ps f1,  896 (x5)\n"
			"flw.ps f28,  768 (x5)\n"
			"flw.ps f24,  480 (x5)\n"
			"flw.ps f17,  160 (x5)\n"
			"flw.ps f31,  128 (x5)\n"
			"flw.ps f21,  608 (x5)\n"
			"flw.ps f26,  0 (x5)\n"
			"flw.ps f20,  704 (x5)\n"
			"flw.ps f16,  864 (x5)\n"
			"flw.ps f12,  224 (x5)\n"
			"flw.ps f8,  736 (x5)\n"
			"flw.ps f14,  992 (x5)\n"
			"flw.ps f23,  448 (x5)\n"
			"flw.ps f25,  256 (x5)\n"
			"flw.ps f0,  192 (x5)\n"
			"flw.ps f5,  384 (x5)\n"
			"flw.ps f18,  544 (x5)\n"
			"flw.ps f13,  640 (x5)\n"
			"flw.ps f2,  288 (x5)\n"
			"flw.ps f11,  512 (x5)\n"
			"flw.ps f6,  352 (x5)\n"
			"flw.ps f4,  96 (x5)\n"
			"flw.ps f9,  960 (x5)\n"
			"flw.ps f7,  672 (x5)\n"
			"flw.ps f15,  320 (x5)\n"
			"flw.ps f22,  832 (x5)\n"
			"flw.ps f27,  64 (x5)\n"
			"flw.ps f19,  928 (x5)\n"
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
			"li x0, 0x258\n" 
			"slti x0, x0, 0x7f2\n"
			"la x31, check_sum_hid_0\n"
			"li x5, 0x3\n" 
			"sw x5, (x31)\n"
			"li x29, 0x6\n"
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
			"la x20, rand_ieee754_32\n"
			"flw.ps f0, 0(x20)\n"
			"flw.ps f2, 800(x20)\n"
			"fadd.ps f12, f0, f2, rne\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f28, 160(x9)\n"
			"flw.ps f12, 224(x9)\n"
			"flw.ps f23, 800(x9)\n"
			"fnmsub.s f23, f28, f12, f23, rmm\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f30, 448(x28)\n"
			"flw.ps f19, 128(x28)\n"
			"fle.ps f9, f30, f19\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x25, rand_ieee754_32\n"
			"flw.ps f6, 704(x25)\n"
			"flw.ps f1, 224(x25)\n"
			"fmax.ps f1, f6, f1\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f12, 0(x9)\n"
			"fclass.ps f12, f12\n" 
			"la x12, rand_int_32\n"
			"flw.ps f11, 0(x12)\n"
			"flw.ps f12, 736(x12)\n"
			"fle.pi f12, f11, f12\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f9, 928(x20)\n"
			"flw.ps f19, 320(x20)\n"
			"fmin.ps f12, f9, f19\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f27, 64(x9)\n"
			"flw.ps f9, 256(x9)\n"
			"fmul.ps f9, f27, f9, rdn\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x22, rand_ieee754_32\n"
			"flw.ps f28, 128(x22)\n"
			"flw.ps f12, 800(x22)\n"
			"fnmsub.s f2, f28, f12, f12, rne\n" 
			"la x13, rand_int_32\n"
			"flw.ps f19, 128(x13)\n"
			"fcvt.ps.pwu f13, f19, rup\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_0:\n"
			"flem.ps m6, f6, f13\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x9, rand_ieee754_32\n"
			"flw.ps f27, 288(x9)\n"
			"fswizz.ps f7, f27, 0x4\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f23, 640(x13)\n"
			"flw.ps f27, 160(x13)\n"
			"fmul.s f28, f23, f27, rne\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f7, 960(x22)\n"
			"flw.ps f27, 672(x22)\n"
			"flw.ps f6, 32(x22)\n"
			"fmsub.s f11, f7, f27, f6, rdn\n" 
			"la x13, rand_int_32\n"
			"flw.ps f9, 576(x13)\n"
			"fslli.pi f7, f9, 0x3\n" 
			"la x10, rand_int_32\n"
			"flw.ps f18, 384(x10)\n"
			"fsat8.pi f7, f18\n" 
			"li x5, 0xd635\n"
			"csrw tensor_mask, x5\n"	
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ID_95728_HID_0:\n"
			"li x5, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x2 << TENSOR_FMA16_B_NUM_COLS) | (0xe << TENSOR_FMA16_A_NUM_ROWS) | (0xb << TENSOR_FMA16_A_NUM_COLS) | (0x9 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1d << TENSOR_FMA16_SCP_LOC_B) | (0xf8 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x0 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x6, ((0x1 << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0xe << TENSOR_QUANT_QUANT_ROW) | (0x25 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x7 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 7\n"
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_0\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_0_RDX_ID_0_SHIRE_0_HID_0:\n"
			"li x20, 0x240000000707000a\n"
			"csrw tensor_reduce, x20\n" 
			"csrwi tensor_wait, 9\n" 
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=0); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"li x5, 0x7f954b9c4bac5448\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0xe799c7bf76d8893a\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ITER_0_HID_0:\n"
			"la x31, self_check_32\n"
			"li x5, 0x8\n"
			"add x5, x5, x31\n"
			"li x6, 0x1000000000000b\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x0 << TENSOR_FMA32_B_NUM_COLS) | (0x8 << TENSOR_FMA32_A_NUM_ROWS) | (0xb << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x1 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x19 << TENSOR_FMA32_SCP_LOC_B) | (0x0 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x18 << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0x8 << TENSOR_QUANT_QUANT_ROW) | (0x36 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x0 << TENSOR_QUANT_TRANSF1) | (0x2 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0xc\n"
			"bne x5, x31, LBL_FAIL_HID_0\n"
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 10\n"
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ITER_1_HID_0:\n"
			"la x31, self_check_16\n"
			"li x5, 0x20000000000000\n"
			"add x5, x5, x31\n"
			"li x6, 0x10000000000000\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x1 << TENSOR_FMA16_B_NUM_COLS) | (0x0 << TENSOR_FMA16_A_NUM_ROWS) | (0x0 << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x1 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x19 << TENSOR_FMA16_SCP_LOC_B) | (0x1 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
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
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_0\n"
			"addi x30, x30, 1\n"
			"la x22, rand_ieee754_32\n"
			"flw.ps f30, 96(x22)\n"
			"flw.ps f0, 992(x22)\n"
			"fsgnj.ps f9, f30, f0\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f28, 512(x13)\n"
			"flw.ps f6, 640(x13)\n"
			"fmin.ps f7, f28, f6\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f7, 448(x9)\n"
			"frsq.ps f7, f7\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f1, 544(x20)\n"
			"flw.ps f28, 576(x20)\n"
			"fsgnjx.s f0, f1, f28\n" 
			"maskpopc x25, m3\n" 
			"LBL_SEQID_1_M0_WRITE_ID_1_HID_0:\n"
			"flem.ps m2, f11, f2\n" 
			"li x5, CREDINC2\n"
			"ld x5, 0(x5)\n"
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x15, rand_ieee754_16\n"
			"flw.ps f2, 704(x15)\n"
			"fcvt.ps.f16 f23, f2\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f19, 992(x9)\n"
			"fclass.ps f12, f19\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f3, 480(x7)\n"
			"flw.ps f23, 832(x7)\n"
			"flt.ps f19, f3, f23\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f28, 768(x20)\n"
			"flw.ps f19, 160(x20)\n"
			"fle.ps f2, f28, f19\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f0, 352(x12)\n"
			"fsgnjn.ps f18, f0, f0\n" 
			"la x15, rand_int_32\n"
			"flw.ps f6, 768(x15)\n"
			"fsrai.pi f3, f6, 0x3\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x9, rand_ieee754_32\n"
			"flw.ps f6, 736(x9)\n"
			"flw.ps f27, 224(x9)\n"
			"fsgnj.ps f27, f6, f27\n" 
			"la x20, rand_int_32\n"
			"flw.ps f12, 256(x20)\n"
			"fcvt.ps.pwu f2, f12, rdn\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f28, 928(x20)\n"
			"flw.ps f19, 512(x20)\n"
			"fmul.ps f30, f28, f19, rne\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x12, rand_ieee754_32\n"
			"flw.ps f7, 32(x12)\n"
			"flw.ps f11, 64(x12)\n"
			"fmin.s f27, f7, f11\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_1_RDX_ID_1_SHIRE_0_HID_0:\n"
			"li x25, 0x140000000330000b\n"
			"csrw tensor_reduce, x25\n" 
			"csrwi tensor_wait, 9\n" 
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ID_124824_HID_0:\n"
			"li x6, ((0xe << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0xe << TENSOR_QUANT_QUANT_ROW) | (0x20 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x1 << TENSOR_QUANT_TRANSF9) | (0x3 << TENSOR_QUANT_TRANSF8) | (0x4 << TENSOR_QUANT_TRANSF7) | (0x8 << TENSOR_QUANT_TRANSF6) | (0x4 << TENSOR_QUANT_TRANSF5) | (0x9 << TENSOR_QUANT_TRANSF4) | (0x3 << TENSOR_QUANT_TRANSF3) | (0xa << TENSOR_QUANT_TRANSF2) | (0x9 << TENSOR_QUANT_TRANSF1) | (0x3 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
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
			"la x31, self_check_32\n"
			"li x5, 0xe0000000000009\n"
			"add x5, x5, x31\n"
			"li x6, 0x10000000000007\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x2 << TENSOR_FMA32_B_NUM_COLS) | (0x9 << TENSOR_FMA32_A_NUM_ROWS) | (0x7 << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x1 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x19 << TENSOR_FMA32_SCP_LOC_B) | (0x7 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x5 << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0x9 << TENSOR_QUANT_QUANT_ROW) | (0x10 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x7 << TENSOR_QUANT_TRANSF8) | (0x7 << TENSOR_QUANT_TRANSF7) | (0x7 << TENSOR_QUANT_TRANSF6) | (0x6 << TENSOR_QUANT_TRANSF5) | (0x6 << TENSOR_QUANT_TRANSF4) | (0x7 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0x8\n"
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
			"la x12, rand_int_32\n"
			"flw.ps f6, 160(x12)\n"
			"flw.ps f2, 960(x12)\n"
			"fsub.pi f12, f6, f2\n" 
			"la x10, rand_int_32\n"
			"flw.ps f9, 448(x10)\n"
			"flw.ps f19, 832(x10)\n"
			"fxor.pi f11, f9, f19\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f27, 992(x9)\n"
			"flw.ps f12, 256(x9)\n"
			"feq.ps f13, f27, f12\n" 
			"LBL_SEQID_2_M0_WRITE_ID_2_HID_0:\n"
			"maskxor m3, m2, m3\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0xd1a47be8e6\n"
			"flw.ps f5, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x22, rand_ieee754_32\n"
			"flw.ps f1, 416(x22)\n"
			"flw.ps f3, 416(x22)\n"
			"flw.ps f12, 512(x22)\n"
			"fnmsub.ps f19, f1, f3, f12, rmm\n" 
			"la x15, rand_int_32\n"
			"flw.ps f1, 320(x15)\n"
			"fsat8.pi f12, f1\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f11, 352(x28)\n"
			"flw.ps f13, 800(x28)\n"
			"flt.ps f11, f11, f13\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f6, 736(x20)\n"
			"fsin.ps f18, f6\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f23, 64(x15)\n"
			"fcvt.pw.ps f9, f23, rmm\n" 
			"la x28, rand_int_32\n"
			"flw.ps f0, 512(x28)\n"
			"flw.ps f2, 96(x28)\n"
			"fmaxu.pi f0, f0, f2\n" 
			"LBL_SEQID_2_M0_WRITE_ID_3_HID_0:\n"
			"masknot m6, m0\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_2_M0_WRITE_ID_4_HID_0:\n"
			"maskxor m2, m6, m2\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x15, rand_int_32\n"
			"flw.ps f19, 256(x15)\n"
			"flw.ps f18, 224(x15)\n"
			"for.pi f12, f19, f18\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f9, 0(x15)\n"
			"flw.ps f6, 864(x15)\n"
			"fsub.s f3, f9, f6, rup\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f23, 672(x28)\n"
			"fcvt.w.s x13, f23, rmm\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f23, 384(x10)\n"
			"flw.ps f19, 608(x10)\n"
			"fsgnjx.s f18, f23, f19\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 0x4ca63e9b5595f964\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0x65275d1782868885\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"LBL_SEQID_2_XMA_QNT_STR_SEQ_ITER_0_HID_0:\n"
			"li x6, ((0x16 << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0x2 << TENSOR_QUANT_QUANT_ROW) | (0xd << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x2 << TENSOR_QUANT_TRANSF7) | (0x6 << TENSOR_QUANT_TRANSF6) | (0x6 << TENSOR_QUANT_TRANSF5) | (0x6 << TENSOR_QUANT_TRANSF4) | (0x7 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_0\n"
			"csrw tensor_error, x0\n"
			"li x5, 3\n" 
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
			"LBL_HPM_3_CORE_TFMA_INST_FAIL_HID_0:\n"          
			"csrr x5, mhpmcounter3\n"
			"sd x5, 32 (x31)\n"
			"LBL_HPM_4_CORE_TS_OPS_FAIL_HID_0:\n"          
			"csrr x5, mhpmcounter4\n"
			"sd x5, 40 (x31)\n"
			"LBL_HPM_5_CORE_TIMA_OPS_FAIL_HID_0:\n"          
			"csrr x5, mhpmcounter5\n"
			"sd x5, 48 (x31)\n"
			"LBL_HPM_6_CORE_TL_OPS_FAIL_HID_0:\n"          
			"csrr x5, mhpmcounter6\n"
			"sd x5, 56 (x31)\n"
			"LBL_HPM_7_NEIGH_RX_COOP_TST_RSP_FAIL_HID_0:\n"          
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
			"LBL_HPM_3_CORE_TFMA_INST_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter3\n"
			"sd x5, 32 (x31)\n"
			"LBL_HPM_4_CORE_TS_OPS_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter4\n"
			"sd x5, 40 (x31)\n"
			"LBL_HPM_5_CORE_TIMA_OPS_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter5\n"
			"sd x5, 48 (x31)\n"
			"LBL_HPM_6_CORE_TL_OPS_PASS_HID_0:\n"          
			"csrr x5, mhpmcounter6\n"
			"sd x5, 56 (x31)\n"
			"LBL_HPM_7_NEIGH_RX_COOP_TST_RSP_PASS_HID_0:\n"          
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
			"csrwi mhpmevent3, 11\n"   
			"csrwi mhpmevent4, 6\n"   
			"csrwi mhpmevent5, 13\n"   
			"csrwi mhpmevent6, 23\n"   
			"csrwi mhpmevent7, 2\n"   
			"csrwi mhpmevent8, 1\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f10,  704 (x5)\n"
			"flw.ps f29,  224 (x5)\n"
			"flw.ps f11,  128 (x5)\n"
			"flw.ps f30,  480 (x5)\n"
			"flw.ps f21,  800 (x5)\n"
			"flw.ps f16,  928 (x5)\n"
			"flw.ps f25,  512 (x5)\n"
			"flw.ps f13,  160 (x5)\n"
			"flw.ps f20,  416 (x5)\n"
			"flw.ps f24,  608 (x5)\n"
			"flw.ps f2,  320 (x5)\n"
			"flw.ps f5,  832 (x5)\n"
			"flw.ps f0,  288 (x5)\n"
			"flw.ps f7,  992 (x5)\n"
			"flw.ps f22,  576 (x5)\n"
			"flw.ps f23,  192 (x5)\n"
			"flw.ps f14,  256 (x5)\n"
			"flw.ps f12,  352 (x5)\n"
			"flw.ps f28,  448 (x5)\n"
			"flw.ps f9,  32 (x5)\n"
			"flw.ps f19,  736 (x5)\n"
			"flw.ps f6,  896 (x5)\n"
			"flw.ps f1,  544 (x5)\n"
			"flw.ps f27,  384 (x5)\n"
			"flw.ps f31,  864 (x5)\n"
			"flw.ps f26,  96 (x5)\n"
			"flw.ps f4,  672 (x5)\n"
			"flw.ps f18,  768 (x5)\n"
			"flw.ps f17,  960 (x5)\n"
			"flw.ps f3,  0 (x5)\n"
			"flw.ps f8,  640 (x5)\n"
			"flw.ps f15,  64 (x5)\n"
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
			"li x5, 0x3\n" 
			"sw x5, (x31)\n"
			"li x29, 0x6\n"
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
			"fbci.pi f13, 0x77e5a\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f21, 384(x28)\n"
			"flw.ps f8, 256(x28)\n"
			"fmul.ps f0, f21, f8, rup\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f27, 224(x15)\n"
			"flw.ps f6, 896(x15)\n"
			"flw.ps f13, 672(x15)\n"
			"fnmsub.s f22, f27, f6, f13, rmm\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f11, 352(x17)\n"
			"flw.ps f20, 960(x17)\n"
			"fle.s x15, f11, f20\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f4, 672(x18)\n"
			"flw.ps f1, 896(x18)\n"
			"fdiv.ps f29, f4, f1, rtz\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f0, 672(x27)\n"
			"fcmovm.ps f6, f0, f0\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f4, 992(x28)\n"
			"flw.ps f8, 320(x28)\n"
			"fmin.s f19, f4, f8\n" 
			"fbci.pi f11, 0x58366\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f13, 64(x28)\n"
			"flw.ps f4, 288(x28)\n"
			"flt.ps f11, f13, f4\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f13, 128(x18)\n"
			"flw.ps f27, 608(x18)\n"
			"fmin.ps f6, f13, f27\n" 
			"la x18, rand_int_32\n"
			"flw.ps f22, 288(x18)\n"
			"flw.ps f4, 160(x18)\n"
			"fmin.pi f29, f22, f4\n" 
			"la x23, rand_int_32\n"
			"flw.ps f29, 544(x23)\n"
			"flw.ps f0, 608(x23)\n"
			"fminu.pi f8, f29, f0\n" 
			"la x18, rand_int_32\n"
			"flw.ps f21, 992(x18)\n"
			"fsrli.pi f8, f21, 0x1\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f13, 192(x15)\n"
			"flw.ps f0, 384(x15)\n"
			"fsgnjx.s f12, f13, f0\n" 
			"LBL_SEQID_0_FP_TRANS_ID_0_HID_1:\n"
			"la x28, rand_ieee754_32\n"
			"flw.ps f4, 128(x28)\n"
			"fexp.ps f8, f4\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f8, 0(x19)\n"
			"fcvt.pwu.ps f21, f8, rne\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_1\n"
			"addi x30, x30, 1\n"
			"mova.x.m x28\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f21, 736(x11)\n"
			"ffrc.ps f19, f21\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f21, 672(x9)\n"
			"flw.ps f25, 768(x9)\n"
			"flw.ps f15, 0(x9)\n"
			"fmadd.ps f1, f21, f25, f15, rup\n" 
			"la x17, rand_int_32\n"
			"flw.ps f19, 256(x17)\n"
			"fsat8.pi f1, f19\n" 
			"la x28, rand_int_32\n"
			"flw.ps f6, 128(x28)\n"
			"fcvt.ps.pw f27, f6, rne\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f22, 608(x19)\n"
			"flw.ps f27, 672(x19)\n"
			"flt.s x9, f22, f27\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f25, 32(x12)\n"
			"fcvt.pwu.ps f8, f25, rmm\n" 
			"LBL_SEQID_1_FP_TRANS_ID_1_HID_1:\n"
			"la x23, rand_ieee754_32\n"
			"flw.ps f12, 928(x23)\n"
			"frcp.ps f13, f12\n" 
			"la x9, rand_int_32\n"
			"flw.ps f12, 256(x9)\n"
			"fsrli.pi f15, f12, 0x0\n" 
			"la x23, rand_int_32\n"
			"flw.ps f11, 128(x23)\n"
			"flw.ps f6, 192(x23)\n"
			"fmulh.pi f29, f11, f6\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f15, 128(x28)\n"
			"fmsub.ps f4, f15, f15, f15, rne\n" 
			"LBL_SEQID_1_M0_WRITE_ID_2_HID_1:\n"
			"maskand m5, m5, m6\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x15, rand_ieee754_32\n"
			"flw.ps f8, 416(x15)\n"
			"fcvt.w.s x15, f8, rdn\n" 
			"la x17, rand_int_32\n"
			"flw.ps f15, 480(x17)\n"
			"fcvt.ps.pwu f29, f15, rtz\n" 
			"la x15, rand_int_32\n"
			"flw.ps f19, 448(x15)\n"
			"fsrli.pi f13, f19, 0x9\n" 
			"la x11, rand_int_32\n"
			"flw.ps f20, 960(x11)\n"
			"fcvt.ps.pwu f4, f20, rtz\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_1\n"
			"addi x30, x30, 1\n"
			"la x23, rand_ieee754_32\n"
			"flw.ps f27, 928(x23)\n"
			"flw.ps f13, 928(x23)\n"
			"fsgnjx.s f13, f27, f13\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f29, 256(x11)\n"
			"flw.ps f0, 928(x11)\n"
			"flt.s x23, f29, f0\n" 
			"LBL_SEQID_2_M0_WRITE_ID_3_HID_1:\n"
			"fsetm.pi m7, f22\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0xa9aa76a957\n"
			"flw.ps f5, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x19, rand_ieee754_32\n"
			"flw.ps f1, 672(x19)\n"
			"flw.ps f12, 992(x19)\n"
			"fdiv.s f0, f1, f12, rup\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f13, 480(x28)\n"
			"fround.ps f1, f13, rtz\n" 
			"la x18, rand_ieee754_16\n"
			"flw.ps f25, 352(x18)\n"
			"fcvt.ps.f16 f20, f25\n" 
			"LBL_SEQID_2_FP_TRANS_ID_4_HID_1:\n"
			"la x27, rand_ieee754_32\n"
			"flw.ps f6, 896(x27)\n"
			"frcp.ps f13, f6\n" 
			"li x17, 0xeaab6026893c635f\n"
			"fcvt.s.lu f8, x17, rtz\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f6, 320(x9)\n"
			"flw.ps f21, 160(x9)\n"
			"fle.s x28, f6, f21\n" 
			"LBL_SEQID_2_M0_WRITE_ID_5_HID_1:\n"
			"maskxor m7, m1, m5\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x11, rand_ieee754_32\n"
			"flw.ps f27, 224(x11)\n"
			"flw.ps f1, 0(x11)\n"
			"fsgnjx.s f19, f27, f1\n" 
			"LBL_SEQID_2_M0_WRITE_ID_6_HID_1:\n"
			"masknot m5, m5\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x17, rand_ieee754_32\n"
			"flw.ps f4, 704(x17)\n"
			"flw.ps f6, 64(x17)\n"
			"fsgnjn.s f6, f4, f6\n" 
			"la x18, rand_int_32\n"
			"flw.ps f27, 320(x18)\n"
			"fsat8.pi f21, f27\n" 
			"LBL_SEQID_2_M0_WRITE_ID_7_HID_1:\n"
			"mova.m.x x11\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0xbd234d130b\n"
			"flw.ps f5, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x17, rand_ieee754_32\n"
			"flw.ps f6, 480(x17)\n"
			"fcvt.f16.ps f11, f6\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 3\n" 
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
			"LBL_HPM_3_CORE_L2_MISS_REQ_REJ_FAIL_HID_1:\n"          
			"csrr x5, mhpmcounter3\n"
			"sd x5, 32 (x31)\n"
			"LBL_HPM_4_CORE_DCACHE_ACCESS0_FAIL_HID_1:\n"          
			"csrr x5, mhpmcounter4\n"
			"sd x5, 40 (x31)\n"
			"LBL_HPM_5_CORE_L2_EVICT_REQ_REJ_FAIL_HID_1:\n"          
			"csrr x5, mhpmcounter5\n"
			"sd x5, 48 (x31)\n"
			"LBL_HPM_6_CORE_FTRANS_OPS_FAIL_HID_1:\n"          
			"csrr x5, mhpmcounter6\n"
			"sd x5, 56 (x31)\n"
			"LBL_HPM_7_NEIGH_ANY_CORE_RX_ETL_RSP_FAIL_HID_1:\n"          
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
			"LBL_HPM_3_CORE_L2_MISS_REQ_REJ_PASS_HID_1:\n"          
			"csrr x5, mhpmcounter3\n"
			"sd x5, 32 (x31)\n"
			"LBL_HPM_4_CORE_DCACHE_ACCESS0_PASS_HID_1:\n"          
			"csrr x5, mhpmcounter4\n"
			"sd x5, 40 (x31)\n"
			"LBL_HPM_5_CORE_L2_EVICT_REQ_REJ_PASS_HID_1:\n"          
			"csrr x5, mhpmcounter5\n"
			"sd x5, 48 (x31)\n"
			"LBL_HPM_6_CORE_FTRANS_OPS_PASS_HID_1:\n"          
			"csrr x5, mhpmcounter6\n"
			"sd x5, 56 (x31)\n"
			"LBL_HPM_7_NEIGH_ANY_CORE_RX_ETL_RSP_PASS_HID_1:\n"          
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
			"csrwi mhpmevent3, 26\n"   
			"csrwi mhpmevent4, 17\n"   
			"csrwi mhpmevent5, 19\n"   
			"csrwi mhpmevent6, 15\n"   
			"csrwi mhpmevent7, 7\n"   
			"csrwi mhpmevent8, 4\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f7,  192 (x5)\n"
			"flw.ps f31,  128 (x5)\n"
			"flw.ps f19,  672 (x5)\n"
			"flw.ps f1,  608 (x5)\n"
			"flw.ps f27,  96 (x5)\n"
			"flw.ps f8,  928 (x5)\n"
			"flw.ps f26,  736 (x5)\n"
			"flw.ps f24,  320 (x5)\n"
			"flw.ps f11,  384 (x5)\n"
			"flw.ps f28,  480 (x5)\n"
			"flw.ps f20,  544 (x5)\n"
			"flw.ps f13,  288 (x5)\n"
			"flw.ps f4,  704 (x5)\n"
			"flw.ps f17,  960 (x5)\n"
			"flw.ps f21,  576 (x5)\n"
			"flw.ps f5,  448 (x5)\n"
			"flw.ps f15,  352 (x5)\n"
			"flw.ps f30,  0 (x5)\n"
			"flw.ps f29,  416 (x5)\n"
			"flw.ps f14,  992 (x5)\n"
			"flw.ps f25,  768 (x5)\n"
			"flw.ps f3,  832 (x5)\n"
			"flw.ps f2,  32 (x5)\n"
			"flw.ps f23,  864 (x5)\n"
			"flw.ps f16,  512 (x5)\n"
			"flw.ps f22,  160 (x5)\n"
			"flw.ps f12,  224 (x5)\n"
			"flw.ps f9,  640 (x5)\n"
			"flw.ps f18,  800 (x5)\n"
			"flw.ps f10,  896 (x5)\n"
			"flw.ps f6,  64 (x5)\n"
			"flw.ps f0,  256 (x5)\n"
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
			"li x5, 0x3\n" 
			"sw x5, (x31)\n"
			"li x29, 0x6\n"
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
			"la x25, rand_int_32\n"
			"flw.ps f16, 480(x25)\n"
			"faddi.pi f6, f16, 0xb0\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x25, rand_int_32\n"
			"flw.ps f29, 64(x25)\n"
			"flw.ps f18, 960(x25)\n"
			"fmin.pi f1, f29, f18\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f1, 288(x6)\n"
			"flw.ps f31, 224(x6)\n"
			"fcmovm.ps f10, f1, f31\n" 
			"la x25, rand_int_32\n"
			"flw.ps f16, 832(x25)\n"
			"flw.ps f29, 128(x25)\n"
			"flt.pi f31, f16, f29\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f12, 928(x28)\n"
			"flw.ps f31, 672(x28)\n"
			"fcmovm.ps f24, f12, f31\n" 
			"LBL_SEQID_0_FP_TRANS_ID_0_HID_2:\n"
			"la x6, rand_ieee754_32\n"
			"flw.ps f16, 32(x6)\n"
			"flog.ps f25, f16\n" 
			"la x25, rand_int_32\n"
			"flw.ps f24, 224(x25)\n"
			"flw.ps f28, 64(x25)\n"
			"fle.pi f10, f24, f28\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_2:\n"
			"maskxor m1, m1, m1\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"li x5, 0xb6d1\n"
			"csrw tensor_mask, x5\n"	
			"la x22, rand_ieee754_32\n"
			"flw.ps f28, 64(x22)\n"
			"ffrc.ps f13, f28\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f29, 96(x11)\n"
			"flw.ps f25, 800(x11)\n"
			"flw.ps f15, 928(x11)\n"
			"fnmadd.s f13, f29, f25, f15, rup\n" 
			"mova.x.m x20\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f16, 896(x26)\n"
			"flw.ps f24, 448(x26)\n"
			"flw.ps f27, 192(x26)\n"
			"fmsub.s f27, f16, f24, f27, rup\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f11, 0(x18)\n"
			"flw.ps f16, 768(x18)\n"
			"fmul.ps f21, f11, f16, rup\n" 
			"la x20, rand_int_32\n"
			"flw.ps f25, 640(x20)\n"
			"fle.pi f1, f25, f25\n" 
			"fbci.pi f13, 0x4778b\n" 
			"LBL_SEQID_0_FP_TRANS_ID_2_HID_2:\n"
			"la x19, rand_ieee754_32\n"
			"flw.ps f1, 416(x19)\n"
			"frcp.ps f29, f1\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ID_503605_HID_2:\n"
			"la x31, self_check_32\n"
			"li x5, 0x8\n"
			"add x5, x5, x31\n"
			"li x6, 0x10000000000005\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x0 << TENSOR_FMA32_B_NUM_COLS) | (0x8 << TENSOR_FMA32_A_NUM_ROWS) | (0x5 << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x1 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x18 << TENSOR_FMA32_SCP_LOC_B) | (0x0 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0x6\n"
			"bne x5, x31, LBL_FAIL_HID_2\n"
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ID_768084_HID_2:\n"
			"la x31, self_check_16\n"
			"li x5, 0x60000000000009\n"
			"add x5, x5, x31\n"
			"li x6, 0x10000000000002\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x1 << TENSOR_FMA16_B_NUM_COLS) | (0x9 << TENSOR_FMA16_A_NUM_ROWS) | (0x2 << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x1 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1c << TENSOR_FMA16_SCP_LOC_B) | (0x3 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0x6\n"
			"bne x5, x31, LBL_FAIL_HID_2\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_2\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_0_RDX_ID_0_SHIRE_0_HID_2:\n"
			"li x9, 0x240000000707000a\n"
			"csrw tensor_reduce, x9\n" 
			"csrwi tensor_wait, 9\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ID_646571_HID_2:\n"
			"la x31, self_check_32\n"
			"li x5, 0x80000000000009\n"
			"add x5, x5, x31\n"
			"li x6, 0x1000000000000d\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x3 << TENSOR_FMA32_B_NUM_COLS) | (0x9 << TENSOR_FMA32_A_NUM_ROWS) | (0xd << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x1 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x18 << TENSOR_FMA32_SCP_LOC_B) | (0x4 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x1e << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0x9 << TENSOR_QUANT_QUANT_ROW) | (0x2b << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x0 << TENSOR_QUANT_TRANSF1) | (0x0 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0xe\n"
			"bne x5, x31, LBL_FAIL_HID_2\n"
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 10\n"
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ID_271208_HID_2:\n"
			"la x31, self_check_8\n"
			"li x5, 0xa000000000000f\n"
			"add x5, x5, x31\n"
			"li x6, 0x3e0000000000006\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x0 << TENSOR_IMA_B_NUM_COLS) | (0xf << TENSOR_IMA_A_NUM_ROWS) | (0x6 << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1f << TENSOR_IMA_SCP_LOC_B) | (0x5 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x9, ((0xe << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0xf << TENSOR_QUANT_QUANT_ROW) | (0x3f << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x0 << TENSOR_QUANT_TRANSF1) | (0xa << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrwi tensor_wait, 0\n"
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
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=0); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ITER_0_HID_2:\n"
			"la x31, self_check_32\n"
			"li x5, 0xc0000000000008\n"
			"add x5, x5, x31\n"
			"li x6, 0x3c000000000000c\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x0 << TENSOR_FMA32_B_NUM_COLS) | (0x8 << TENSOR_FMA32_A_NUM_ROWS) | (0xc << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1e << TENSOR_FMA32_SCP_LOC_B) | (0x6 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0xd\n"
			"bne x5, x31, LBL_FAIL_HID_2\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_2\n"
			"csrw tensor_error, x0\n"
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_2\n"
			"addi x30, x30, 1\n"
			"la x18, rand_ieee754_32\n"
			"flw.ps f25, 576(x18)\n"
			"flw.ps f29, 32(x18)\n"
			"fmin.ps f1, f25, f29\n" 
			"LBL_SEQID_1_FP_TRANS_ID_3_HID_2:\n"
			"la x18, rand_ieee754_32\n"
			"flw.ps f15, 480(x18)\n"
			"frcp.ps f31, f15\n" 
			"la x28, rand_int_32\n"
			"flw.ps f25, 800(x28)\n"
			"flw.ps f24, 640(x28)\n"
			"fmulh.pi f21, f25, f24\n" 
			"la x26, rand_int_32\n"
			"flw.ps f10, 608(x26)\n"
			"flw.ps f6, 672(x26)\n"
			"fxor.pi f29, f10, f6\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f10, 800(x22)\n"
			"fclass.ps f25, f10\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f29, 832(x20)\n"
			"flw.ps f16, 256(x20)\n"
			"fcmovm.ps f6, f29, f16\n" 
			"maskpopcz x19, m1\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f31, 96(x11)\n"
			"flw.ps f11, 0(x11)\n"
			"flw.ps f27, 160(x11)\n"
			"fmsub.s f25, f31, f11, f27, rmm\n" 
			"fbci.pi f31, 0x2096a\n" 
			"la x5, rand_int_32\n"
			"flw.ps f21, 832(x5)\n"
			"fcvt.ps.pw f1, f21, rmm\n" 
			"LBL_SEQID_1_FP_TRANS_ID_4_HID_2:\n"
			"la x25, rand_ieee754_32\n"
			"flw.ps f27, 832(x25)\n"
			"flog.ps f16, f27\n" 
			"la x9, rand_ieee754_16\n"
			"flw.ps f12, 416(x9)\n"
			"fcvt.ps.f16 f24, f12\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f31, 64(x5)\n"
			"fround.ps f25, f31, rne\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f11, 992(x26)\n"
			"flw.ps f12, 512(x26)\n"
			"fmul.ps f11, f11, f12, rmm\n" 
			"LBL_SEQID_1_M0_WRITE_ID_5_HID_2:\n"
			"fltm.ps m4, f18, f29\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x11, rand_int_32\n"
			"flw.ps f24, 800(x11)\n"
			"fpackreph.pi f10, f24\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_1_RDX_ID_1_SHIRE_0_HID_2:\n"
			"li x19, 0x140000000330000b\n"
			"csrw tensor_reduce, x19\n" 
			"csrwi tensor_wait, 9\n" 
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=0); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"LBL_SEQID_1_TLD_XMA_STR_SEQ_ITER_0_HID_2:\n"
			"la x31, self_check_32\n"
			"li x5, 0xe0000000000006\n"
			"add x5, x5, x31\n"
			"li x6, 0x380000000000008\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x2 << TENSOR_FMA32_B_NUM_COLS) | (0x6 << TENSOR_FMA32_A_NUM_ROWS) | (0x8 << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1c << TENSOR_FMA32_SCP_LOC_B) | (0x7 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
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
			"bne x5, x31, LBL_FAIL_HID_2\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_2\n"
			"csrw tensor_error, x0\n"
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_2\n"
			"addi x30, x30, 1\n"
			"la x19, rand_ieee754_32\n"
			"flw.ps f28, 960(x19)\n"
			"flw.ps f18, 320(x19)\n"
			"fcmovm.ps f24, f28, f18\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f15, 224(x20)\n"
			"flw.ps f24, 96(x20)\n"
			"fsub.ps f6, f15, f24, rne\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f18, 896(x18)\n"
			"ffrc.ps f25, f18\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f12, 64(x9)\n"
			"flw.ps f28, 864(x9)\n"
			"fadd.s f29, f12, f28, rdn\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f12, 288(x26)\n"
			"flw.ps f10, 608(x26)\n"
			"flw.ps f25, 256(x26)\n"
			"fnmsub.ps f11, f12, f10, f25, rup\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f25, 192(x11)\n"
			"flw.ps f27, 672(x11)\n"
			"fsub.s f13, f25, f27, rmm\n" 
			"la x28, rand_int_32\n"
			"flw.ps f10, 288(x28)\n"
			"fpackreph.pi f15, f10\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f21, 320(x26)\n"
			"flw.ps f15, 640(x26)\n"
			"fcmovm.ps f16, f21, f15\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f27, 384(x25)\n"
			"flw.ps f6, 192(x25)\n"
			"fsgnjn.ps f12, f27, f6\n" 
			"LBL_SEQID_2_M0_WRITE_ID_6_HID_2:\n"
			"mova.m.x x11\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x6, rand_ieee754_32\n"
			"flw.ps f16, 32(x6)\n"
			"flw.ps f11, 192(x6)\n"
			"fadd.ps f31, f16, f11, rne\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f6, 576(x11)\n"
			"flw.ps f12, 320(x11)\n"
			"flw.ps f10, 256(x11)\n"
			"fnmsub.ps f18, f6, f12, f10, rdn\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f27, 864(x9)\n"
			"flw.ps f6, 448(x9)\n"
			"fsgnjx.ps f28, f27, f6\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f25, 608(x5)\n"
			"flw.ps f16, 128(x5)\n"
			"fadd.s f10, f25, f16, rne\n" 
			"la x20, rand_int_32\n"
			"flw.ps f11, 736(x20)\n"
			"flw.ps f12, 960(x20)\n"
			"fsll.pi f31, f11, f12\n" 
			"la x25, rand_int_32\n"
			"flw.ps f21, 416(x25)\n"
			"fsrl.pi f31, f21, f21\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_2_XMA_QNT_STR_SEQ_ID_532836_HID_2:\n"
			"li x5, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x1 << TENSOR_FMA16_B_NUM_COLS) | (0x0 << TENSOR_FMA16_A_NUM_ROWS) | (0x8 << TENSOR_FMA16_A_NUM_COLS) | (0x7 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x77 << TENSOR_FMA16_SCP_LOC_B) | (0x28 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x0 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_2\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_2_XMA_QNT_STR_SEQ_ITER_0_HID_2:\n"
			"li x5, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x2 << TENSOR_FMA32_B_NUM_COLS) | (0x2 << TENSOR_FMA32_A_NUM_ROWS) | (0x4 << TENSOR_FMA32_A_NUM_COLS) | (0x6 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0xbf << TENSOR_FMA32_SCP_LOC_B) | (0xc4 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x0 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"LBL_SEQID_2_XMA_QNT_STR_SEQ_ITER_1_HID_2:\n"
			"li x5, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x0 << TENSOR_FMA16_B_NUM_COLS) | (0xe << TENSOR_FMA16_A_NUM_ROWS) | (0xb << TENSOR_FMA16_A_NUM_COLS) | (0x7 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0xa4 << TENSOR_FMA16_SCP_LOC_B) | (0x79 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_2\n"
			"csrw tensor_error, x0\n"
			"li x5, 3\n" 
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
			"csrwi mhpmevent3, 11\n"   
			"csrwi mhpmevent4, 6\n"   
			"csrwi mhpmevent5, 13\n"   
			"csrwi mhpmevent6, 23\n"   
			"csrwi mhpmevent7, 2\n"   
			"csrwi mhpmevent8, 1\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f0,  896 (x5)\n"
			"flw.ps f15,  928 (x5)\n"
			"flw.ps f9,  320 (x5)\n"
			"flw.ps f6,  864 (x5)\n"
			"flw.ps f8,  224 (x5)\n"
			"flw.ps f4,  288 (x5)\n"
			"flw.ps f12,  960 (x5)\n"
			"flw.ps f23,  0 (x5)\n"
			"flw.ps f27,  160 (x5)\n"
			"flw.ps f14,  384 (x5)\n"
			"flw.ps f1,  736 (x5)\n"
			"flw.ps f31,  32 (x5)\n"
			"flw.ps f25,  832 (x5)\n"
			"flw.ps f28,  576 (x5)\n"
			"flw.ps f30,  448 (x5)\n"
			"flw.ps f19,  992 (x5)\n"
			"flw.ps f11,  96 (x5)\n"
			"flw.ps f16,  128 (x5)\n"
			"flw.ps f29,  800 (x5)\n"
			"flw.ps f21,  352 (x5)\n"
			"flw.ps f10,  608 (x5)\n"
			"flw.ps f3,  544 (x5)\n"
			"flw.ps f7,  64 (x5)\n"
			"flw.ps f17,  640 (x5)\n"
			"flw.ps f18,  416 (x5)\n"
			"flw.ps f2,  192 (x5)\n"
			"flw.ps f13,  768 (x5)\n"
			"flw.ps f5,  704 (x5)\n"
			"flw.ps f22,  256 (x5)\n"
			"flw.ps f24,  512 (x5)\n"
			"flw.ps f20,  480 (x5)\n"
			"flw.ps f26,  672 (x5)\n"
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
			"li x5, 0x3\n" 
			"sw x5, (x31)\n"
			"li x29, 0x6\n"
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
			"la x13, rand_int_32\n"
			"flw.ps f16, 288(x13)\n"
			"flw.ps f5, 544(x13)\n"
			"fand.pi f27, f16, f5\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f7, 288(x13)\n"
			"flw.ps f3, 512(x13)\n"
			"feq.s x23, f7, f3\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f31, 32(x23)\n"
			"fcvt.w.s x17, f31, rmm\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_3:\n"
			"fltm.pi m4, f12, f7\n" 
			"li x5, 0x80340168\n"
			"ld x5, 0(x5)\n"
			"la x13, rand_ieee754_32\n"
			"flw.ps f7, 256(x13)\n"
			"flw.ps f16, 800(x13)\n"
			"fadd.ps f24, f7, f16, rne\n" 
			"la x12, rand_int_32\n"
			"flw.ps f1, 96(x12)\n"
			"flw.ps f21, 384(x12)\n"
			"fremu.pi f25, f1, f21\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f21, 224(x17)\n"
			"flw.ps f22, 32(x17)\n"
			"fsgnjx.s f22, f21, f22\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f11, 864(x13)\n"
			"flw.ps f22, 416(x13)\n"
			"feq.s x15, f11, f22\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f13, 896(x27)\n"
			"fcvt.f16.ps f19, f13\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f19, 320(x22)\n"
			"fcvt.wu.s x27, f19, rtz\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f7, 704(x23)\n"
			"flw.ps f22, 224(x23)\n"
			"fmul.s f5, f7, f22, rtz\n" 
			"la x20, rand_int_32\n"
			"flw.ps f11, 864(x20)\n"
			"flw.ps f13, 768(x20)\n"
			"fsub.pi f21, f11, f13\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f13, 224(x7)\n"
			"flw.ps f1, 512(x7)\n"
			"fadd.s f22, f13, f1, rne\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f21, 800(x17)\n"
			"flw.ps f3, 256(x17)\n"
			"fdiv.ps f25, f21, f3, rne\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_3:\n"
			"maskxor m4, m4, m7\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x13, rand_int_32\n"
			"flw.ps f22, 448(x13)\n"
			"flw.ps f7, 384(x13)\n"
			"fand.pi f19, f22, f7\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_3\n"
			"addi x30, x30, 1\n"
			"la x27, rand_int_32\n"
			"flw.ps f11, 672(x27)\n"
			"flw.ps f16, 480(x27)\n"
			"for.pi f5, f11, f16\n" 
			"la x7, rand_int_32\n"
			"flw.ps f12, 256(x7)\n"
			"flw.ps f3, 448(x7)\n"
			"fmul.pi f19, f12, f3\n" 
			"la x13, rand_int_32\n"
			"flw.ps f12, 160(x13)\n"
			"flw.ps f25, 800(x13)\n"
			"fmulh.pi f22, f12, f25\n" 
			"LBL_SEQID_1_FP_TRANS_ID_2_HID_3:\n"
			"la x12, rand_ieee754_32\n"
			"flw.ps f3, 96(x12)\n"
			"fexp.ps f19, f3\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f13, 544(x9)\n"
			"flw.ps f12, 544(x9)\n"
			"fsgnjn.s f21, f13, f12\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f7, 736(x9)\n"
			"flw.ps f3, 928(x9)\n"
			"feq.s x7, f7, f3\n" 
			"la x12, rand_int_32\n"
			"flw.ps f7, 320(x12)\n"
			"flw.ps f25, 352(x12)\n"
			"fsub.pi f19, f7, f25\n" 
			"la x13, rand_int_32\n"
			"flw.ps f5, 288(x13)\n"
			"flw.ps f1, 64(x13)\n"
			"for.pi f19, f5, f1\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f29, 992(x23)\n"
			"fmax.s f27, f29, f29\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f19, 832(x27)\n"
			"flw.ps f16, 352(x27)\n"
			"fsub.ps f27, f19, f16, rdn\n" 
			"la x12, rand_int_32\n"
			"flw.ps f24, 352(x12)\n"
			"flw.ps f13, 992(x12)\n"
			"fmax.pi f7, f24, f13\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f22, 960(x27)\n"
			"fcvt.f16.ps f24, f22\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f5, 160(x20)\n"
			"fcvt.f16.ps f29, f5\n" 
			"LBL_SEQID_1_FP_TRANS_ID_3_HID_3:\n"
			"la x17, rand_ieee754_32\n"
			"flw.ps f5, 800(x17)\n"
			"frcp.ps f31, f5\n" 
			"la x13, rand_int_32\n"
			"flw.ps f25, 480(x13)\n"
			"flw.ps f31, 544(x13)\n"
			"fmul.pi f19, f25, f31\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f12, 576(x20)\n"
			"flw.ps f27, 704(x20)\n"
			"fsgnjx.ps f3, f12, f27\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_3\n"
			"addi x30, x30, 1\n"
			"la x13, rand_ieee754_32\n"
			"flw.ps f24, 192(x13)\n"
			"fcvt.f16.ps f1, f24\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f29, 128(x22)\n"
			"flw.ps f16, 672(x22)\n"
			"fsub.s f13, f29, f16, rmm\n" 
			"la x12, rand_int_32\n"
			"flw.ps f21, 64(x12)\n"
			"flw.ps f29, 672(x12)\n"
			"fmul.pi f1, f21, f29\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f27, 0(x13)\n"
			"flw.ps f3, 128(x13)\n"
			"fsub.s f5, f27, f3, rmm\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f25, 800(x7)\n"
			"flw.ps f24, 448(x7)\n"
			"fsub.ps f22, f25, f24, rmm\n" 
			"la x12, rand_int_32\n"
			"flw.ps f25, 960(x12)\n"
			"fsub.pi f12, f25, f25\n" 
			"la x23, rand_int_32\n"
			"flw.ps f12, 832(x23)\n"
			"flw.ps f31, 672(x23)\n"
			"fmin.pi f24, f12, f31\n" 
			"la x13, rand_ieee754_16\n"
			"flw.ps f1, 896(x13)\n"
			"fcvt.ps.f16 f22, f1\n" 
			"la x23, rand_int_32\n"
			"flw.ps f21, 480(x23)\n"
			"flw.ps f24, 800(x23)\n"
			"fmin.pi f27, f21, f24\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f16, 896(x7)\n"
			"fsqrt.s f24, f16, rdn\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f13, 832(x27)\n"
			"flw.ps f1, 192(x27)\n"
			"feq.s x20, f13, f1\n" 
			"la x23, rand_int_32\n"
			"flw.ps f29, 832(x23)\n"
			"fsrli.pi f31, f29, 0x5\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f1, 160(x13)\n"
			"frsq.ps f1, f1\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f22, 192(x13)\n"
			"flw.ps f5, 576(x13)\n"
			"flw.ps f24, 96(x13)\n"
			"fmadd.ps f7, f22, f5, f24, rtz\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f27, 32(x20)\n"
			"flw.ps f25, 288(x20)\n"
			"feq.ps f16, f27, f25\n" 
			"la x12, rand_int_32\n"
			"flw.ps f3, 160(x12)\n"
			"flw.ps f31, 928(x12)\n"
			"flt.pi f7, f3, f31\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 3\n" 
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
			"csrwi mhpmevent3, 26\n"   
			"csrwi mhpmevent4, 17\n"   
			"csrwi mhpmevent5, 19\n"   
			"csrwi mhpmevent6, 15\n"   
			"csrwi mhpmevent7, 7\n"   
			"csrwi mhpmevent8, 4\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f30,  352 (x5)\n"
			"flw.ps f2,  416 (x5)\n"
			"flw.ps f22,  864 (x5)\n"
			"flw.ps f20,  832 (x5)\n"
			"flw.ps f7,  288 (x5)\n"
			"flw.ps f21,  160 (x5)\n"
			"flw.ps f8,  384 (x5)\n"
			"flw.ps f18,  544 (x5)\n"
			"flw.ps f26,  896 (x5)\n"
			"flw.ps f16,  960 (x5)\n"
			"flw.ps f1,  640 (x5)\n"
			"flw.ps f28,  576 (x5)\n"
			"flw.ps f10,  480 (x5)\n"
			"flw.ps f4,  928 (x5)\n"
			"flw.ps f25,  608 (x5)\n"
			"flw.ps f13,  256 (x5)\n"
			"flw.ps f17,  96 (x5)\n"
			"flw.ps f6,  736 (x5)\n"
			"flw.ps f27,  768 (x5)\n"
			"flw.ps f23,  64 (x5)\n"
			"flw.ps f24,  192 (x5)\n"
			"flw.ps f14,  32 (x5)\n"
			"flw.ps f11,  0 (x5)\n"
			"flw.ps f19,  320 (x5)\n"
			"flw.ps f9,  448 (x5)\n"
			"flw.ps f31,  800 (x5)\n"
			"flw.ps f5,  224 (x5)\n"
			"flw.ps f12,  512 (x5)\n"
			"flw.ps f0,  672 (x5)\n"
			"flw.ps f29,  992 (x5)\n"
			"flw.ps f15,  704 (x5)\n"
			"flw.ps f3,  128 (x5)\n"
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
			"li x5, 0x3\n" 
			"sw x5, (x31)\n"
			"li x29, 0x6\n"
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
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_4:\n"
			"maskxor m2, m0, m3\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0xFF\n"
			"li x22, 0xc48019765ab9841e\n"
			"fcvt.s.wu f31, x22, rne\n" 
			"la x10, rand_int_32\n"
			"flw.ps f1, 320(x10)\n"
			"fsrai.pi f21, f1, 0x2\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f7, 704(x7)\n"
			"flw.ps f13, 0(x7)\n"
			"fmax.s f30, f7, f13\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x25, rand_int_32\n"
			"flw.ps f24, 96(x25)\n"
			"flw.ps f3, 320(x25)\n"
			"fltu.pi f31, f24, f3\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f24, 32(x16)\n"
			"flw.ps f1, 192(x16)\n"
			"fnmadd.ps f5, f24, f1, f1, rmm\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f10, 416(x13)\n"
			"flw.ps f11, 800(x13)\n"
			"fmin.ps f10, f10, f11\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x16, rand_ieee754_32\n"
			"flw.ps f3, 32(x16)\n"
			"fround.ps f11, f3, rdn\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f1, 480(x7)\n"
			"flw.ps f3, 672(x7)\n"
			"fmsub.s f21, f1, f1, f3, rup\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f15, 576(x22)\n"
			"flw.ps f2, 32(x22)\n"
			"fle.s x22, f15, f2\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_4:\n"
			"masknot m5, m0\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x20, rand_int_32\n"
			"flw.ps f7, 800(x20)\n"
			"fsat8.pi f31, f7\n" 
			"fbci.pi f21, 0x9315\n" 
			"la x7, rand_int_32\n"
			"flw.ps f31, 288(x7)\n"
			"flw.ps f26, 0(x7)\n"
			"fltu.pi f31, f31, f26\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f3, 992(x25)\n"
			"fcvt.pwu.ps f1, f3, rdn\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f13, 608(x16)\n"
			"flw.ps f26, 864(x16)\n"
			"fsgnjn.ps f27, f13, f26\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ID_141535_HID_4:\n"
			"la x31, self_check_32\n"
			"li x5, 0xe0000000000003\n"
			"add x5, x5, x31\n"
			"li x6, 0x1000000000000a\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x0 << TENSOR_FMA32_B_NUM_COLS) | (0x3 << TENSOR_FMA32_A_NUM_ROWS) | (0xa << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x1 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1a << TENSOR_FMA32_SCP_LOC_B) | (0x7 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0xb\n"
			"bne x5, x31, LBL_FAIL_HID_4\n"
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ID_30526_HID_4:\n"
			"la x31, self_check_8\n"
			"li x5, 0x2000000000000b\n"
			"add x5, x5, x31\n"
			"li x6, 0x3c000000000000f\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x0 << TENSOR_IMA_B_NUM_COLS) | (0xb << TENSOR_IMA_A_NUM_ROWS) | (0xf << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1e << TENSOR_IMA_SCP_LOC_B) | (0x1 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x9, ((0x10 << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0xb << TENSOR_QUANT_QUANT_ROW) | (0x1 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x5 << TENSOR_QUANT_TRANSF7) | (0x3 << TENSOR_QUANT_TRANSF6) | (0x4 << TENSOR_QUANT_TRANSF5) | (0x5 << TENSOR_QUANT_TRANSF4) | (0x4 << TENSOR_QUANT_TRANSF3) | (0x9 << TENSOR_QUANT_TRANSF2) | (0xa << TENSOR_QUANT_TRANSF1) | (0x3 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"bne x5, x31, LBL_FAIL_HID_4\n"
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_4\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_0_RDX_ID_0_SHIRE_0_HID_4:\n"
			"li x25, 0x240000000707000a\n"
			"csrw tensor_reduce, x25\n" 
			"csrwi tensor_wait, 9\n" 
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=0); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ITER_0_HID_4:\n"
			"li x6, ((0x3 << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0x4 << TENSOR_QUANT_QUANT_ROW) | (0x24 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x2 << TENSOR_QUANT_TRANSF6) | (0x6 << TENSOR_QUANT_TRANSF5) | (0x6 << TENSOR_QUANT_TRANSF4) | (0x6 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ITER_1_HID_4:\n"
			"li x6, ((0xf << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0x1 << TENSOR_QUANT_QUANT_ROW) | (0x35 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x8 << TENSOR_QUANT_TRANSF5) | (0x9 << TENSOR_QUANT_TRANSF4) | (0x4 << TENSOR_QUANT_TRANSF3) | (0xa << TENSOR_QUANT_TRANSF2) | (0x5 << TENSOR_QUANT_TRANSF1) | (0x8 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_4\n"
			"csrw tensor_error, x0\n"
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_4\n"
			"addi x30, x30, 1\n"
			"la x7, rand_ieee754_32\n"
			"flw.ps f23, 128(x7)\n"
			"fround.ps f24, f23, rtz\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f15, 96(x15)\n"
			"flw.ps f13, 960(x15)\n"
			"fsgnjx.ps f24, f15, f13\n" 
			"la x13, rand_int_32\n"
			"flw.ps f10, 288(x13)\n"
			"flw.ps f15, 0(x13)\n"
			"frem.pi f27, f10, f15\n" 
			"li x5, 0x3c4699e205b1582e\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0x7359a3ca68d04545\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"la x16, rand_int_32\n"
			"flw.ps f11, 416(x16)\n"
			"faddi.pi f21, f11, 0x136\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f1, 928(x15)\n"
			"flw.ps f13, 128(x15)\n"
			"feq.ps f31, f1, f13\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f31, 288(x15)\n"
			"flw.ps f15, 128(x15)\n"
			"fle.s x22, f31, f15\n" 
			"la x16, rand_ieee754_16\n"
			"flw.ps f26, 160(x16)\n"
			"fcvt.ps.f16 f24, f26\n" 
			"LBL_SEQID_1_M0_WRITE_ID_2_HID_4:\n"
			"fltm.pi m3, f24, f30\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"li x5, 0xdb90\n"
			"csrw tensor_mask, x5\n"	
			"LBL_SEQID_1_M0_WRITE_ID_3_HID_4:\n"
			"mov.m.x m3, x17, 0x2e\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_1_M0_WRITE_ID_4_HID_4:\n"
			"fsetm.pi m5, f31\n" 
			"li x5, CREDINC0\n"
			"ld x5, 0(x5)\n"
			"la x17, rand_ieee754_32\n"
			"flw.ps f5, 416(x17)\n"
			"flw.ps f11, 96(x17)\n"
			"fnmadd.ps f7, f5, f11, f5, rmm\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x7, rand_int_32\n"
			"flw.ps f23, 896(x7)\n"
			"flw.ps f3, 704(x7)\n"
			"fmax.pi f26, f23, f3\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f1, 832(x22)\n"
			"ffrc.ps f23, f1\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f7, 352(x25)\n"
			"fswizz.ps f10, f7, 0x2\n" 
			"la x25, rand_int_32\n"
			"flw.ps f26, 960(x25)\n"
			"faddi.pi f7, f26, 0xd7\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x22, rand_int_32\n"
			"flw.ps f23, 352(x22)\n"
			"flw.ps f24, 800(x22)\n"
			"fmul.pi f24, f23, f24\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_1_RDX_ID_1_SHIRE_0_HID_4:\n"
			"li x25, 0x140000000330000b\n"
			"csrw tensor_reduce, x25\n" 
			"csrwi tensor_wait, 9\n" 
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ID_723935_HID_4:\n"
			"li x6, ((0x6 << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0x2 << TENSOR_QUANT_QUANT_ROW) | (0x2a << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x0 << TENSOR_QUANT_TRANSF1) | (0xa << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ID_8049_HID_4:\n"
			"li x6, ((0xf << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0x2 << TENSOR_QUANT_QUANT_ROW) | (0x35 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x2 << TENSOR_QUANT_TRANSF7) | (0x6 << TENSOR_QUANT_TRANSF6) | (0x6 << TENSOR_QUANT_TRANSF5) | (0x7 << TENSOR_QUANT_TRANSF4) | (0x7 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ID_280425_HID_4:\n"
			"li x6, ((0xb << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0xe << TENSOR_QUANT_QUANT_ROW) | (0x37 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x1 << TENSOR_QUANT_TRANSF2) | (0x3 << TENSOR_QUANT_TRANSF1) | (0x9 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"li x5, 0xf370\n"
			"csrw tensor_mask, x5\n"	
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ITER_0_HID_4:\n"
			"li x5, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x1 << TENSOR_FMA32_B_NUM_COLS) | (0x2 << TENSOR_FMA32_A_NUM_ROWS) | (0xd << TENSOR_FMA32_A_NUM_COLS) | (0xb << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0xd7 << TENSOR_FMA32_SCP_LOC_B) | (0x8d << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ITER_1_HID_4:\n"
			"li x5, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x3 << TENSOR_IMA_B_NUM_COLS) | (0x0 << TENSOR_IMA_A_NUM_ROWS) | (0x2 << TENSOR_IMA_A_NUM_COLS) | (0xb << TENSOR_IMA_OFFSET) | (0x0 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x36 << TENSOR_IMA_SCP_LOC_B) | (0xec << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x0 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ITER_2_HID_4:\n"
			"li x5, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x0 << TENSOR_FMA16_B_NUM_COLS) | (0xa << TENSOR_FMA16_A_NUM_ROWS) | (0xb << TENSOR_FMA16_A_NUM_COLS) | (0x2 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x22 << TENSOR_FMA16_SCP_LOC_B) | (0xd4 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_4\n"
			"csrw tensor_error, x0\n"
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_4\n"
			"addi x30, x30, 1\n"
			"la x16, rand_ieee754_32\n"
			"flw.ps f30, 64(x16)\n"
			"flw.ps f3, 224(x16)\n"
			"feq.s x9, f30, f3\n" 
			"li x5, 0x76de\n"
			"csrw tensor_mask, x5\n"	
			"la x10, rand_ieee754_32\n"
			"flw.ps f11, 896(x10)\n"
			"flw.ps f30, 608(x10)\n"
			"fsgnjx.ps f23, f11, f30\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f2, 32(x17)\n"
			"flw.ps f24, 960(x17)\n"
			"fcmovm.ps f27, f2, f24\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f31, 896(x25)\n"
			"flw.ps f24, 576(x25)\n"
			"fmul.ps f10, f31, f24, rmm\n" 
			"la x16, rand_int_32\n"
			"flw.ps f3, 448(x16)\n"
			"flw.ps f5, 480(x16)\n"
			"fadd.pi f24, f3, f5\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f2, 0(x25)\n"
			"flw.ps f30, 352(x25)\n"
			"flt.s x20, f2, f30\n" 
			"la x13, rand_int_32\n"
			"flw.ps f21, 384(x13)\n"
			"flw.ps f31, 160(x13)\n"
			"frem.pi f5, f21, f31\n" 
			"la x9, rand_int_32\n"
			"flw.ps f10, 384(x9)\n"
			"flw.ps f15, 544(x9)\n"
			"fminu.pi f11, f10, f15\n" 
			"LBL_SEQID_2_FP_TRANS_ID_5_HID_4:\n"
			"la x10, rand_ieee754_32\n"
			"flw.ps f11, 160(x10)\n"
			"frcp.ps f24, f11\n" 
			"la x13, rand_int_32\n"
			"flw.ps f1, 224(x13)\n"
			"fpackreph.pi f1, f1\n" 
			"LBL_SEQID_2_M0_WRITE_ID_6_HID_4:\n"
			"fltm.ps m2, f30, f10\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x10, rand_ieee754_32\n"
			"flw.ps f1, 448(x10)\n"
			"flw.ps f13, 960(x10)\n"
			"fmul.ps f21, f1, f13, rmm\n" 
			"la x22, rand_int_32\n"
			"flw.ps f24, 736(x22)\n"
			"fpackreph.pi f10, f24\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f23, 96(x10)\n"
			"ffrc.ps f15, f23\n" 
			"la x9, rand_int_32\n"
			"flw.ps f13, 544(x9)\n"
			"flw.ps f3, 992(x9)\n"
			"fadd.pi f3, f13, f3\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f27, 96(x13)\n"
			"flw.ps f10, 320(x13)\n"
			"fadd.ps f7, f27, f10, rne\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_2_XMA_QNT_STR_SEQ_ID_303158_HID_4:\n"
			"li x6, ((0xb << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0x2 << TENSOR_QUANT_QUANT_ROW) | (0x29 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x1 << TENSOR_QUANT_TRANSF3) | (0x3 << TENSOR_QUANT_TRANSF2) | (0x3 << TENSOR_QUANT_TRANSF1) | (0x3 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_4\n"
			"csrw tensor_error, x0\n"
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"LBL_SEQID_2_TLD_XMA_STR_SEQ_ITER_0_HID_4:\n"
			"la x31, self_check_8\n"
			"li x5, 0x60000000000005\n"
			"add x5, x5, x31\n"
			"li x6, 0x10000000000007\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x2 << TENSOR_IMA_B_NUM_COLS) | (0x5 << TENSOR_IMA_A_NUM_ROWS) | (0x7 << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x1 << TENSOR_IMA_TEN_B_IN_MEM) | (0x18 << TENSOR_IMA_SCP_LOC_B) | (0x3 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fmvz.x.ps x31, f0, 0\n"
			"li x5, 0x20\n"
			"bne x5, x31, LBL_FAIL_HID_4\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_4\n"
			"csrw tensor_error, x0\n"
			"li x5, 3\n" 
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
			"csrwi mhpmevent3, 11\n"   
			"csrwi mhpmevent4, 6\n"   
			"csrwi mhpmevent5, 13\n"   
			"csrwi mhpmevent6, 23\n"   
			"csrwi mhpmevent7, 2\n"   
			"csrwi mhpmevent8, 1\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f19,  320 (x5)\n"
			"flw.ps f15,  224 (x5)\n"
			"flw.ps f0,  832 (x5)\n"
			"flw.ps f7,  384 (x5)\n"
			"flw.ps f29,  512 (x5)\n"
			"flw.ps f16,  544 (x5)\n"
			"flw.ps f3,  352 (x5)\n"
			"flw.ps f18,  256 (x5)\n"
			"flw.ps f2,  992 (x5)\n"
			"flw.ps f5,  0 (x5)\n"
			"flw.ps f31,  160 (x5)\n"
			"flw.ps f17,  864 (x5)\n"
			"flw.ps f30,  128 (x5)\n"
			"flw.ps f25,  928 (x5)\n"
			"flw.ps f12,  96 (x5)\n"
			"flw.ps f24,  768 (x5)\n"
			"flw.ps f22,  480 (x5)\n"
			"flw.ps f4,  736 (x5)\n"
			"flw.ps f14,  448 (x5)\n"
			"flw.ps f28,  960 (x5)\n"
			"flw.ps f21,  704 (x5)\n"
			"flw.ps f23,  608 (x5)\n"
			"flw.ps f26,  416 (x5)\n"
			"flw.ps f20,  896 (x5)\n"
			"flw.ps f10,  64 (x5)\n"
			"flw.ps f9,  800 (x5)\n"
			"flw.ps f27,  576 (x5)\n"
			"flw.ps f1,  192 (x5)\n"
			"flw.ps f6,  288 (x5)\n"
			"flw.ps f13,  32 (x5)\n"
			"flw.ps f11,  672 (x5)\n"
			"flw.ps f8,  640 (x5)\n"
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
			"li x5, 0x3\n" 
			"sw x5, (x31)\n"
			"li x29, 0x6\n"
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
			"la x7, rand_int_32\n"
			"flw.ps f12, 800(x7)\n"
			"faddi.pi f10, f12, 0x60\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f17, 416(x27)\n"
			"fcvt.w.s x27, f17, rdn\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f22, 448(x27)\n"
			"fround.ps f27, f22, rup\n" 
			"la x18, rand_int_32\n"
			"flw.ps f31, 352(x18)\n"
			"fpackreph.pi f27, f31\n" 
			"la x25, rand_int_32\n"
			"flw.ps f3, 384(x25)\n"
			"fsrai.pi f12, f3, 0x2\n" 
			"la x19, rand_int_32\n"
			"flw.ps f12, 128(x19)\n"
			"flw.ps f19, 896(x19)\n"
			"fsrl.pi f22, f12, f19\n" 
			"la x28, rand_int_32\n"
			"flw.ps f27, 608(x28)\n"
			"fcvt.ps.pwu f7, f27, rup\n" 
			"maskpopc x27, m2\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f8, 416(x28)\n"
			"fcvt.wu.s x17, f8, rup\n" 
			"la x23, rand_int_32\n"
			"flw.ps f24, 736(x23)\n"
			"flw.ps f17, 672(x23)\n"
			"for.pi f21, f24, f17\n" 
			"la x10, rand_int_32\n"
			"flw.ps f27, 352(x10)\n"
			"flw.ps f24, 256(x10)\n"
			"fmax.pi f22, f27, f24\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_5:\n"
			"flem.ps m0, f26, f22\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x19, rand_int_32\n"
			"flw.ps f12, 992(x19)\n"
			"flw.ps f26, 800(x19)\n"
			"fminu.pi f8, f12, f26\n" 
			"la x28, rand_int_32\n"
			"flw.ps f26, 960(x28)\n"
			"fsrli.pi f31, f26, 0x8\n" 
			"la x18, rand_int_32\n"
			"flw.ps f19, 512(x18)\n"
			"fpackreph.pi f27, f19\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_5:\n"
			"maskxor m6, m0, m1\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0xFF\n"
			"csrw tensor_error, zero\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_5\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_1_FP_TRANS_ID_2_HID_5:\n"
			"la x10, rand_ieee754_32\n"
			"flw.ps f17, 896(x10)\n"
			"frcp.ps f24, f17\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f22, 288(x25)\n"
			"flw.ps f19, 672(x25)\n"
			"fle.s x25, f22, f19\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f31, 32(x19)\n"
			"flw.ps f21, 608(x19)\n"
			"flw.ps f20, 32(x19)\n"
			"fmadd.s f24, f31, f21, f20, rtz\n" 
			"mova.x.m x15\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f17, 96(x10)\n"
			"ffrc.ps f26, f17\n" 
			"LBL_SEQID_1_M0_WRITE_ID_3_HID_5:\n"
			"mov.m.x m1, x28, 0x9\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_1_FP_TRANS_ID_4_HID_5:\n"
			"la x15, rand_ieee754_32\n"
			"flw.ps f19, 896(x15)\n"
			"frcp.ps f19, f19\n" 
			"la x10, rand_int_32\n"
			"flw.ps f22, 64(x10)\n"
			"flw.ps f31, 320(x10)\n"
			"fsll.pi f19, f22, f31\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f18, 352(x27)\n"
			"flw.ps f17, 832(x27)\n"
			"fmul.ps f21, f18, f17, rne\n" 
			"LBL_SEQID_1_FP_TRANS_ID_5_HID_5:\n"
			"la x7, rand_ieee754_32\n"
			"flw.ps f18, 896(x7)\n"
			"frcp.ps f19, f18\n" 
			"la x15, rand_int_32\n"
			"flw.ps f31, 160(x15)\n"
			"fsrai.pi f20, f31, 0xd\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f10, 512(x25)\n"
			"flw.ps f26, 192(x25)\n"
			"fadd.s f24, f10, f26, rtz\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f26, 736(x18)\n"
			"flw.ps f17, 64(x18)\n"
			"flw.ps f21, 928(x18)\n"
			"fmsub.ps f18, f26, f17, f21, rmm\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f20, 64(x7)\n"
			"flw.ps f10, 928(x7)\n"
			"feq.ps f18, f20, f10\n" 
			"LBL_SEQID_1_M0_WRITE_ID_6_HID_5:\n"
			"maskxor m0, m6, m2\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_1_M0_WRITE_ID_7_HID_5:\n"
			"fsetm.pi m6, f22\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"csrw tensor_error, zero\n" 
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_5\n"
			"addi x30, x30, 1\n"
			"la x18, rand_ieee754_16\n"
			"flw.ps f27, 832(x18)\n"
			"fcvt.ps.f16 f8, f27\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f26, 96(x15)\n"
			"fcvt.w.s x23, f26, rtz\n" 
			"la x15, rand_int_32\n"
			"flw.ps f8, 448(x15)\n"
			"flw.ps f16, 192(x15)\n"
			"for.pi f16, f8, f16\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f8, 864(x10)\n"
			"flw.ps f19, 800(x10)\n"
			"fdiv.ps f10, f8, f19, rtz\n" 
			"LBL_SEQID_2_M0_WRITE_ID_8_HID_5:\n"
			"flem.ps m6, f12, f8\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_2_M0_WRITE_ID_9_HID_5:\n"
			"maskand m1, m2, m2\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x10, rand_ieee754_32\n"
			"flw.ps f7, 64(x10)\n"
			"flw.ps f27, 288(x10)\n"
			"fsgnjn.ps f22, f7, f27\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f7, 416(x23)\n"
			"flw.ps f24, 768(x23)\n"
			"fsub.ps f27, f7, f24, rtz\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f17, 128(x15)\n"
			"flw.ps f26, 288(x15)\n"
			"flw.ps f7, 320(x15)\n"
			"fnmadd.s f31, f17, f26, f7, rtz\n" 
			"maskpopc x27, m6\n" 
			"li x27, 0xf3757d710372a83f\n"
			"fcvt.s.wu f18, x27, rtz\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f10, 96(x28)\n"
			"fsub.ps f12, f10, f10, rdn\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f26, 608(x15)\n"
			"flw.ps f7, 384(x15)\n"
			"fsub.s f24, f26, f7, rdn\n" 
			"la x7, rand_int_32\n"
			"flw.ps f19, 352(x7)\n"
			"fsrli.pi f8, f19, 0x7\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f22, 928(x7)\n"
			"fswizz.ps f8, f22, 0x18\n" 
			"la x17, rand_int_32\n"
			"flw.ps f22, 576(x17)\n"
			"fsatu8.pi f16, f22\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 3\n" 
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
			"csrwi mhpmevent3, 26\n"   
			"csrwi mhpmevent4, 17\n"   
			"csrwi mhpmevent5, 19\n"   
			"csrwi mhpmevent6, 15\n"   
			"csrwi mhpmevent7, 7\n"   
			"csrwi mhpmevent8, 4\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f18,  96 (x5)\n"
			"flw.ps f13,  800 (x5)\n"
			"flw.ps f24,  352 (x5)\n"
			"flw.ps f19,  32 (x5)\n"
			"flw.ps f14,  384 (x5)\n"
			"flw.ps f25,  288 (x5)\n"
			"flw.ps f0,  416 (x5)\n"
			"flw.ps f30,  128 (x5)\n"
			"flw.ps f31,  640 (x5)\n"
			"flw.ps f21,  576 (x5)\n"
			"flw.ps f20,  864 (x5)\n"
			"flw.ps f27,  256 (x5)\n"
			"flw.ps f3,  704 (x5)\n"
			"flw.ps f5,  832 (x5)\n"
			"flw.ps f29,  320 (x5)\n"
			"flw.ps f23,  736 (x5)\n"
			"flw.ps f9,  608 (x5)\n"
			"flw.ps f1,  160 (x5)\n"
			"flw.ps f10,  448 (x5)\n"
			"flw.ps f26,  480 (x5)\n"
			"flw.ps f28,  0 (x5)\n"
			"flw.ps f8,  928 (x5)\n"
			"flw.ps f11,  672 (x5)\n"
			"flw.ps f7,  512 (x5)\n"
			"flw.ps f17,  896 (x5)\n"
			"flw.ps f2,  224 (x5)\n"
			"flw.ps f16,  960 (x5)\n"
			"flw.ps f4,  768 (x5)\n"
			"flw.ps f22,  64 (x5)\n"
			"flw.ps f15,  544 (x5)\n"
			"flw.ps f6,  992 (x5)\n"
			"flw.ps f12,  192 (x5)\n"
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
			"li x5, 0x3\n" 
			"sw x5, (x31)\n"
			"li x29, 0x6\n"
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
			"la x27, rand_int_32\n"
			"flw.ps f9, 256(x27)\n"
			"flw.ps f12, 704(x27)\n"
			"fsll.pi f8, f9, f12\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f31, 864(x20)\n"
			"flw.ps f24, 512(x20)\n"
			"fsgnj.ps f1, f31, f24\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f14, 0(x26)\n"
			"flw.ps f18, 448(x26)\n"
			"fnmsub.s f29, f14, f18, f18, rdn\n" 
			"la x20, rand_ieee754_16\n"
			"flw.ps f24, 608(x20)\n"
			"fcvt.ps.f16 f29, f24\n" 
			"li x5, 0x3f8cc0782a730513\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0x6a4c324a5f1d9a33\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"la x6, rand_int_32\n"
			"flw.ps f30, 64(x6)\n"
			"flw.ps f29, 736(x6)\n"
			"fminu.pi f8, f30, f29\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f5, 192(x25)\n"
			"frsq.ps f24, f5\n" 
			"la x12, rand_int_32\n"
			"flw.ps f24, 384(x12)\n"
			"flw.ps f9, 640(x12)\n"
			"fmaxu.pi f31, f24, f9\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_6:\n"
			"fltm.pi m2, f15, f1\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x10, rand_int_32\n"
			"flw.ps f8, 352(x10)\n"
			"fsrai.pi f20, f8, 0xa\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f8, 864(x10)\n"
			"flw.ps f3, 768(x10)\n"
			"flw.ps f18, 736(x10)\n"
			"fnmadd.ps f9, f8, f3, f18, rup\n" 
			"la x6, rand_int_32\n"
			"flw.ps f30, 928(x6)\n"
			"flw.ps f18, 832(x6)\n"
			"fmin.pi f29, f30, f18\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f3, 704(x5)\n"
			"flw.ps f29, 448(x5)\n"
			"fsgnjx.ps f20, f3, f29\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x11, rand_int_32\n"
			"flw.ps f15, 576(x11)\n"
			"flw.ps f12, 864(x11)\n"
			"fsra.pi f30, f15, f12\n" 
			"la x11, rand_int_32\n"
			"flw.ps f16, 480(x11)\n"
			"flw.ps f15, 288(x11)\n"
			"fmulhu.pi f15, f16, f15\n" 
			"la x11, rand_int_32\n"
			"flw.ps f16, 704(x11)\n"
			"fsrai.pi f3, f16, 0xb\n" 
			"li x5, 0xa26\n"
			"csrw tensor_mask, x5\n"	
			"la x10, rand_ieee754_32\n"
			"flw.ps f30, 32(x10)\n"
			"flw.ps f18, 416(x10)\n"
			"flw.ps f5, 576(x10)\n"
			"fnmsub.ps f30, f30, f18, f5, rmm\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_0_RDX_ID_0_SHIRE_0_HID_6:\n"
			"li x23, 0x240000000707000a\n"
			"csrw tensor_reduce, x23\n" 
			"csrwi tensor_wait, 9\n" 
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=0); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ITER_0_HID_6:\n"
			"la x31, self_check_32\n"
			"li x5, 0xe0000000000003\n"
			"add x5, x5, x31\n"
			"li x6, 0x10000000000005\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x1 << TENSOR_FMA32_B_NUM_COLS) | (0x3 << TENSOR_FMA32_A_NUM_ROWS) | (0x5 << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x1 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1d << TENSOR_FMA32_SCP_LOC_B) | (0x7 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x16 << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0x3 << TENSOR_QUANT_QUANT_ROW) | (0x16 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x6 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0x6\n"
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
			"la x5, rand_ieee754_32\n"
			"flw.ps f12, 864(x5)\n"
			"fcvt.l.s x20, f12, rne\n" 
			"li x5, 0x7c772cdebc84531f\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0x721244229411cf0b\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"la x25, rand_int_32\n"
			"flw.ps f18, 736(x25)\n"
			"fsrai.pi f29, f18, 0x0\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f3, 960(x5)\n"
			"fmax.s f20, f3, f3\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f5, 704(x12)\n"
			"fswizz.ps f5, f5, 0x15\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x5, rand_int_32\n"
			"flw.ps f16, 256(x5)\n"
			"flw.ps f20, 64(x5)\n"
			"fadd.pi f30, f16, f20\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f16, 288(x26)\n"
			"flw.ps f8, 480(x26)\n"
			"fsgnjn.ps f11, f16, f8\n" 
			"LBL_SEQID_1_M0_WRITE_ID_1_HID_6:\n"
			"masknot m0, m0\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x23, rand_ieee754_32\n"
			"flw.ps f3, 576(x23)\n"
			"fclass.s x26, f3\n" 
			"li x5, 0x8164\n"
			"csrw tensor_mask, x5\n"	
			"la x25, rand_int_32\n"
			"flw.ps f1, 640(x25)\n"
			"flw.ps f14, 960(x25)\n"
			"for.pi f29, f1, f14\n" 
			"LBL_SEQID_1_M0_WRITE_ID_2_HID_6:\n"
			"maskand m7, m4, m0\n" 
			"mov.m.x m0, x0, 0xff\n"
			"li x5, 0x80340190\n"
			"ld x5, 0(x5)\n"
			"LBL_SEQID_1_M0_WRITE_ID_3_HID_6:\n"
			"maskxor m2, m7, m0\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0xc971cb0a50\n"
			"fsw.ps f0, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x5, rand_int_32\n"
			"flw.ps f16, 576(x5)\n"
			"flw.ps f5, 416(x5)\n"
			"fmin.pi f3, f16, f5\n" 
			"la x20, rand_int_32\n"
			"flw.ps f5, 160(x20)\n"
			"faddi.pi f31, f5, 0x190\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"LBL_SEQID_1_M0_WRITE_ID_4_HID_6:\n"
			"fltm.pi m4, f15, f12\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x10, rand_ieee754_32\n"
			"flw.ps f31, 480(x10)\n"
			"flw.ps f14, 384(x10)\n"
			"fmul.s f29, f31, f14, rmm\n" 
			"LBL_SEQID_1_M0_WRITE_ID_5_HID_6:\n"
			"mova.m.x x6\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_1_RDX_ID_1_SHIRE_0_HID_6:\n"
			"li x25, 0x140000000330000b\n"
			"csrw tensor_reduce, x25\n" 
			"csrwi tensor_wait, 9\n" 
			"LBL_SEQID_1_TLD_XMA_STR_SEQ_ID_797634_HID_6:\n"
			"la x31, self_check_16\n"
			"li x5, 0x80000000000004\n"
			"add x5, x5, x31\n"
			"li x6, 0x1000000000000c\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x1 << TENSOR_FMA16_B_NUM_COLS) | (0x4 << TENSOR_FMA16_A_NUM_ROWS) | (0xc << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x1 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1e << TENSOR_FMA16_SCP_LOC_B) | (0x4 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x9, ((0xc << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0x4 << TENSOR_QUANT_QUANT_ROW) | (0xd << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x0 << TENSOR_QUANT_TRANSF1) | (0x2 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
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
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=0); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"LBL_SEQID_1_TLD_XMA_STR_SEQ_ITER_0_HID_6:\n"
			"la x31, self_check_16\n"
			"li x5, 0x60000000000001\n"
			"add x5, x5, x31\n"
			"li x6, 0x340000000000007\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x3 << TENSOR_FMA16_B_NUM_COLS) | (0x1 << TENSOR_FMA16_A_NUM_ROWS) | (0x7 << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1a << TENSOR_FMA16_SCP_LOC_B) | (0x3 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x0 << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0x1 << TENSOR_QUANT_QUANT_ROW) | (0x17 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x7 << TENSOR_QUANT_TRANSF6) | (0x7 << TENSOR_QUANT_TRANSF5) | (0x6 << TENSOR_QUANT_TRANSF4) | (0x7 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"la x20, rand_ieee754_32\n"
			"flw.ps f5, 512(x20)\n"
			"flt.ps f30, f5, f5\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f11, 896(x27)\n"
			"flw.ps f16, 800(x27)\n"
			"flw.ps f14, 608(x27)\n"
			"fnmsub.s f1, f11, f16, f14, rtz\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f8, 928(x26)\n"
			"flw.ps f12, 0(x26)\n"
			"fsub.s f31, f8, f12, rne\n" 
			"la x6, rand_int_32\n"
			"flw.ps f9, 832(x6)\n"
			"flw.ps f1, 64(x6)\n"
			"fmaxu.pi f31, f9, f1\n" 
			"li x5, 0x8205da19a9d787d\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0xa064a5f561bed4ef\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"la x11, rand_int_32\n"
			"flw.ps f8, 32(x11)\n"
			"fsatu8.pi f11, f8\n" 
			"la x5, rand_int_32\n"
			"flw.ps f9, 352(x5)\n"
			"flw.ps f31, 256(x5)\n"
			"fsrl.pi f9, f9, f31\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f31, 928(x23)\n"
			"flw.ps f11, 800(x23)\n"
			"fmax.s f12, f31, f11\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f5, 96(x10)\n"
			"flw.ps f12, 160(x10)\n"
			"flw.ps f15, 0(x10)\n"
			"fmadd.s f14, f5, f12, f15, rup\n" 
			"la x27, rand_int_32\n"
			"flw.ps f9, 704(x27)\n"
			"flw.ps f12, 192(x27)\n"
			"fmul.pi f1, f9, f12\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f20, 288(x12)\n"
			"flw.ps f31, 992(x12)\n"
			"flw.ps f9, 32(x12)\n"
			"fnmsub.ps f31, f20, f31, f9, rdn\n" 
			"li x5, 0xd0d4\n"
			"csrw tensor_mask, x5\n"	
			"la x12, rand_ieee754_16\n"
			"flw.ps f24, 32(x12)\n"
			"fcvt.ps.f16 f29, f24\n" 
			"maskpopc x5, m2\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f16, 320(x27)\n"
			"flw.ps f8, 128(x27)\n"
			"fsgnjx.ps f29, f16, f8\n" 
			"LBL_SEQID_2_FP_TRANS_ID_6_HID_6:\n"
			"la x23, rand_ieee754_32\n"
			"flw.ps f18, 160(x23)\n"
			"flog.ps f5, f18\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x6, rand_ieee754_32\n"
			"flw.ps f9, 768(x6)\n"
			"frsq.ps f30, f9\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f1, 0(x20)\n"
			"flw.ps f18, 640(x20)\n"
			"fsgnjn.s f16, f1, f18\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_2_XMA_QNT_STR_SEQ_ITER_0_HID_6:\n"
			"li x5, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x1 << TENSOR_FMA32_B_NUM_COLS) | (0x7 << TENSOR_FMA32_A_NUM_ROWS) | (0xd << TENSOR_FMA32_A_NUM_COLS) | (0x1 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x10 << TENSOR_FMA32_SCP_LOC_B) | (0xd1 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x0 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"LBL_SEQID_2_XMA_QNT_STR_SEQ_ITER_1_HID_6:\n"
			"li x5, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x2 << TENSOR_FMA32_B_NUM_COLS) | (0x2 << TENSOR_FMA32_A_NUM_ROWS) | (0x4 << TENSOR_FMA32_A_NUM_COLS) | (0x7 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x53 << TENSOR_FMA32_SCP_LOC_B) | (0xae << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x0 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_6\n"
			"csrw tensor_error, x0\n"
			"li x5, 3\n" 
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
			"csrwi mhpmevent4, 6\n"   
			"csrwi mhpmevent5, 13\n"   
			"csrwi mhpmevent6, 23\n"   
			"csrwi mhpmevent7, 2\n"   
			"csrwi mhpmevent8, 1\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f19,  448 (x5)\n"
			"flw.ps f17,  864 (x5)\n"
			"flw.ps f29,  160 (x5)\n"
			"flw.ps f22,  608 (x5)\n"
			"flw.ps f4,  672 (x5)\n"
			"flw.ps f28,  0 (x5)\n"
			"flw.ps f3,  192 (x5)\n"
			"flw.ps f7,  896 (x5)\n"
			"flw.ps f10,  384 (x5)\n"
			"flw.ps f2,  256 (x5)\n"
			"flw.ps f30,  32 (x5)\n"
			"flw.ps f21,  928 (x5)\n"
			"flw.ps f26,  96 (x5)\n"
			"flw.ps f23,  832 (x5)\n"
			"flw.ps f20,  128 (x5)\n"
			"flw.ps f16,  480 (x5)\n"
			"flw.ps f6,  512 (x5)\n"
			"flw.ps f13,  288 (x5)\n"
			"flw.ps f0,  352 (x5)\n"
			"flw.ps f14,  992 (x5)\n"
			"flw.ps f8,  704 (x5)\n"
			"flw.ps f18,  576 (x5)\n"
			"flw.ps f1,  768 (x5)\n"
			"flw.ps f27,  640 (x5)\n"
			"flw.ps f11,  64 (x5)\n"
			"flw.ps f9,  800 (x5)\n"
			"flw.ps f5,  544 (x5)\n"
			"flw.ps f12,  736 (x5)\n"
			"flw.ps f15,  320 (x5)\n"
			"flw.ps f24,  416 (x5)\n"
			"flw.ps f31,  960 (x5)\n"
			"flw.ps f25,  224 (x5)\n"
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
			"li x5, 0x3\n" 
			"sw x5, (x31)\n"
			"li x29, 0x6\n"
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
			"la x18, rand_ieee754_32\n"
			"flw.ps f22, 544(x18)\n"
			"flw.ps f5, 896(x18)\n"
			"fcmovm.ps f7, f22, f5\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f25, 448(x5)\n"
			"flw.ps f19, 832(x5)\n"
			"fsgnjn.ps f0, f25, f19\n" 
			"LBL_SEQID_0_FP_TRANS_ID_0_HID_7:\n"
			"la x9, rand_ieee754_32\n"
			"flw.ps f21, 928(x9)\n"
			"fexp.ps f12, f21\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f19, 512(x11)\n"
			"fsqrt.ps f31, f19\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f21, 928(x9)\n"
			"fcvt.pwu.ps f31, f21, rdn\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f31, 704(x9)\n"
			"fcvt.l.s x7, f31, rmm\n" 
			"la x7, rand_int_32\n"
			"flw.ps f21, 544(x7)\n"
			"fsat8.pi f7, f21\n" 
			"la x19, rand_ieee754_16\n"
			"flw.ps f28, 832(x19)\n"
			"fcvt.ps.f16 f9, f28\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f9, 480(x19)\n"
			"fcvt.pw.ps f19, f9, rne\n" 
			"la x22, rand_int_32\n"
			"flw.ps f28, 832(x22)\n"
			"fandi.pi f13, f28, 0x17b\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f17, 320(x9)\n"
			"fsin.ps f17, f17\n" 
			"la x7, rand_int_32\n"
			"flw.ps f17, 928(x7)\n"
			"flw.ps f25, 832(x7)\n"
			"fmul.pi f12, f17, f25\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f21, 256(x5)\n"
			"flw.ps f31, 768(x5)\n"
			"flt.ps f19, f21, f31\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f23, 832(x22)\n"
			"flw.ps f13, 128(x22)\n"
			"fadd.ps f28, f23, f13, rup\n" 
			"LBL_SEQID_0_FP_TRANS_ID_1_HID_7:\n"
			"la x23, rand_ieee754_32\n"
			"flw.ps f21, 832(x23)\n"
			"frcp.ps f7, f21\n" 
			"LBL_SEQID_0_M0_WRITE_ID_2_HID_7:\n"
			"maskand m0, m0, m5\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0xFF\n"
			"csrw tensor_error, zero\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_7\n"
			"addi x30, x30, 1\n"
			"la x23, rand_int_32\n"
			"flw.ps f19, 480(x23)\n"
			"fpackreph.pi f31, f19\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f28, 96(x22)\n"
			"flw.ps f12, 672(x22)\n"
			"feq.s x20, f28, f12\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f21, 224(x5)\n"
			"fmax.ps f20, f21, f21\n" 
			"la x5, rand_int_32\n"
			"flw.ps f17, 32(x5)\n"
			"fsrli.pi f20, f17, 0x1\n" 
			"la x20, rand_int_32\n"
			"flw.ps f2, 704(x20)\n"
			"fsub.pi f13, f2, f2\n" 
			"la x14, rand_int_32\n"
			"flw.ps f23, 960(x14)\n"
			"flw.ps f7, 96(x14)\n"
			"fsra.pi f31, f23, f7\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f23, 992(x20)\n"
			"flw.ps f5, 736(x20)\n"
			"fsgnj.s f2, f23, f5\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f9, 448(x17)\n"
			"fcvt.wu.s x7, f9, rup\n" 
			"fbci.pi f23, 0x7d837\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f23, 512(x17)\n"
			"flw.ps f13, 800(x17)\n"
			"fadd.s f7, f23, f13, rdn\n" 
			"LBL_SEQID_1_FP_TRANS_ID_3_HID_7:\n"
			"la x22, rand_ieee754_32\n"
			"flw.ps f21, 544(x22)\n"
			"frcp.ps f19, f21\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f22, 640(x23)\n"
			"fmul.ps f12, f22, f22, rne\n" 
			"la x20, rand_int_32\n"
			"flw.ps f7, 992(x20)\n"
			"fsrai.pi f31, f7, 0xb\n" 
			"LBL_SEQID_1_M0_WRITE_ID_4_HID_7:\n"
			"flem.ps m0, f9, f7\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0xfb1e9ea4ad\n"
			"fsw.ps f0, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x11, rand_int_32\n"
			"flw.ps f12, 448(x11)\n"
			"fmax.pi f0, f12, f12\n" 
			"la x18, rand_int_32\n"
			"flw.ps f12, 704(x18)\n"
			"flw.ps f7, 320(x18)\n"
			"fmul.pi f13, f12, f7\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_7\n"
			"addi x30, x30, 1\n"
			"la x7, rand_ieee754_32\n"
			"flw.ps f28, 480(x7)\n"
			"flw.ps f20, 128(x7)\n"
			"flw.ps f31, 160(x7)\n"
			"fnmsub.ps f5, f28, f20, f31, rtz\n" 
			"la x23, rand_int_32\n"
			"flw.ps f31, 512(x23)\n"
			"fcvt.ps.pw f31, f31, rup\n" 
			"la x20, rand_int_32\n"
			"flw.ps f22, 416(x20)\n"
			"flw.ps f0, 992(x20)\n"
			"fsll.pi f13, f22, f0\n" 
			"LBL_SEQID_2_M0_WRITE_ID_5_HID_7:\n"
			"fltm.pi m2, f22, f2\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x11, rand_ieee754_16\n"
			"flw.ps f21, 384(x11)\n"
			"fcvt.ps.f16 f13, f21\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f31, 864(x11)\n"
			"flw.ps f21, 64(x11)\n"
			"fsgnjn.ps f22, f31, f21\n" 
			"LBL_SEQID_2_M0_WRITE_ID_6_HID_7:\n"
			"mov.m.x m1, x17, 0x60\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x9, rand_int_32\n"
			"flw.ps f19, 992(x9)\n"
			"flw.ps f12, 160(x9)\n"
			"fltu.pi f5, f19, f12\n" 
			"fbci.ps f12, 0x2970a\n" 
			"fbci.pi f0, 0x2f52f\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f5, 576(x22)\n"
			"flw.ps f25, 832(x22)\n"
			"flt.s x18, f5, f25\n" 
			"la x19, rand_int_32\n"
			"flw.ps f21, 640(x19)\n"
			"fandi.pi f13, f21, 0x13a\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f9, 384(x22)\n"
			"flw.ps f21, 384(x22)\n"
			"flw.ps f31, 800(x22)\n"
			"fmsub.ps f25, f9, f21, f31, rne\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f25, 512(x7)\n"
			"fclass.ps f21, f25\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f31, 960(x19)\n"
			"fclass.s x19, f31\n" 
			"maskpopc x19, m5\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 3\n" 
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
			"csrwi mhpmevent3, 26\n"   
			"csrwi mhpmevent4, 17\n"   
			"csrwi mhpmevent5, 19\n"   
			"csrwi mhpmevent6, 15\n"   
			"csrwi mhpmevent7, 7\n"   
			"csrwi mhpmevent8, 4\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f23,  384 (x5)\n"
			"flw.ps f9,  416 (x5)\n"
			"flw.ps f12,  608 (x5)\n"
			"flw.ps f26,  576 (x5)\n"
			"flw.ps f24,  224 (x5)\n"
			"flw.ps f1,  736 (x5)\n"
			"flw.ps f7,  128 (x5)\n"
			"flw.ps f18,  768 (x5)\n"
			"flw.ps f11,  672 (x5)\n"
			"flw.ps f16,  32 (x5)\n"
			"flw.ps f20,  896 (x5)\n"
			"flw.ps f13,  992 (x5)\n"
			"flw.ps f31,  96 (x5)\n"
			"flw.ps f17,  640 (x5)\n"
			"flw.ps f6,  192 (x5)\n"
			"flw.ps f14,  960 (x5)\n"
			"flw.ps f30,  480 (x5)\n"
			"flw.ps f21,  800 (x5)\n"
			"flw.ps f0,  320 (x5)\n"
			"flw.ps f25,  288 (x5)\n"
			"flw.ps f28,  704 (x5)\n"
			"flw.ps f10,  544 (x5)\n"
			"flw.ps f15,  64 (x5)\n"
			"flw.ps f27,  160 (x5)\n"
			"flw.ps f22,  512 (x5)\n"
			"flw.ps f5,  928 (x5)\n"
			"flw.ps f2,  832 (x5)\n"
			"flw.ps f8,  448 (x5)\n"
			"flw.ps f4,  864 (x5)\n"
			"flw.ps f3,  256 (x5)\n"
			"flw.ps f19,  352 (x5)\n"
			"flw.ps f29,  0 (x5)\n"
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
			"li x5, 0x3\n" 
			"sw x5, (x31)\n"
			"li x29, 0x6\n"
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
			"la x14, rand_int_32\n"
			"flw.ps f16, 384(x14)\n"
			"fsat8.pi f17, f16\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f9, 704(x26)\n"
			"flw.ps f26, 320(x26)\n"
			"fsgnjx.s f17, f9, f26\n" 
			"la x13, rand_int_32\n"
			"flw.ps f20, 96(x13)\n"
			"faddi.pi f28, f20, 0x140\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f14, 736(x20)\n"
			"flw.ps f21, 608(x20)\n"
			"flw.ps f19, 448(x20)\n"
			"fmsub.s f16, f14, f21, f19, rup\n" 
			"LBL_SEQID_0_FP_TRANS_ID_0_HID_8:\n"
			"la x6, rand_ieee754_32\n"
			"flw.ps f14, 256(x6)\n"
			"frcp.ps f17, f14\n" 
			"la x14, rand_int_32\n"
			"flw.ps f9, 352(x14)\n"
			"flw.ps f26, 512(x14)\n"
			"for.pi f14, f9, f26\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f20, 640(x18)\n"
			"fcvt.w.s x5, f20, rmm\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f0, 384(x26)\n"
			"fclass.s x14, f0\n" 
			"la x5, rand_int_32\n"
			"flw.ps f21, 288(x5)\n"
			"fcvt.ps.pwu f13, f21, rne\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f19, 928(x12)\n"
			"flw.ps f2, 64(x12)\n"
			"fnmadd.ps f14, f19, f19, f2, rdn\n" 
			"la x12, rand_int_32\n"
			"flw.ps f21, 512(x12)\n"
			"fandi.pi f17, f21, 0xa7\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f13, 832(x20)\n"
			"flw.ps f28, 992(x20)\n"
			"flw.ps f2, 96(x20)\n"
			"fcmov.ps f11, f13, f28, f2\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f13, 672(x13)\n"
			"flw.ps f8, 64(x13)\n"
			"fsgnj.ps f17, f13, f8\n" 
			"la x23, rand_int_32\n"
			"flw.ps f17, 832(x23)\n"
			"fpackrepb.pi f14, f17\n" 
			"la x26, rand_int_32\n"
			"flw.ps f20, 192(x26)\n"
			"fnot.pi f16, f20\n" 
			"la x15, rand_int_32\n"
			"flw.ps f26, 672(x15)\n"
			"flw.ps f0, 608(x15)\n"
			"fmin.pi f21, f26, f0\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_0_RDX_ID_0_SHIRE_0_HID_8:\n"
			"li x15, 0x240000000707000a\n"
			"csrw tensor_reduce, x15\n" 
			"csrwi tensor_wait, 9\n" 
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=0); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"li x5, 0x78576cb448cb50cf\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0x752d03feaddf1ae2\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ITER_0_HID_8:\n"
			"li x5, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x0 << TENSOR_FMA16_B_NUM_COLS) | (0x1 << TENSOR_FMA16_A_NUM_ROWS) | (0x8 << TENSOR_FMA16_A_NUM_COLS) | (0xc << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0xce << TENSOR_FMA16_SCP_LOC_B) | (0x6c << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x0 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ITER_1_HID_8:\n"
			"li x5, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x3 << TENSOR_IMA_B_NUM_COLS) | (0xb << TENSOR_IMA_A_NUM_ROWS) | (0x8 << TENSOR_IMA_A_NUM_COLS) | (0x2 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x5c << TENSOR_IMA_SCP_LOC_B) | (0x7e << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x0 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x6, ((0x1d << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0xb << TENSOR_QUANT_QUANT_ROW) | (0x16 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x3 << TENSOR_QUANT_TRANSF3) | (0x8 << TENSOR_QUANT_TRANSF2) | (0x5 << TENSOR_QUANT_TRANSF1) | (0x8 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 7\n"
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_8\n"
			"csrw tensor_error, x0\n"
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_8\n"
			"addi x30, x30, 1\n"
			"la x26, rand_ieee754_32\n"
			"flw.ps f28, 864(x26)\n"
			"flw.ps f20, 448(x26)\n"
			"fmsub.s f5, f28, f20, f20, rup\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f5, 992(x12)\n"
			"fcvt.pw.ps f14, f5, rne\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f1, 160(x12)\n"
			"fsin.ps f20, f1\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f9, 128(x20)\n"
			"flw.ps f0, 352(x20)\n"
			"flt.ps f16, f9, f0\n" 
			"li x5, 0x5fdc18d279e6858a\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0x8051ce6557956fa4\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"la x5, rand_int_32\n"
			"flw.ps f19, 32(x5)\n"
			"fsatu8.pi f19, f19\n" 
			"LBL_SEQID_1_M0_WRITE_ID_1_HID_8:\n"
			"masknot m2, m3\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_1_M0_WRITE_ID_2_HID_8:\n"
			"mov.m.x m1, x18, 0x6c\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0x75bd21dad4\n"
			"flw.ps f5, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"maskpopc x23, m3\n" 
			"la x13, rand_int_32\n"
			"flw.ps f11, 448(x13)\n"
			"fsrai.pi f14, f11, 0x1\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f9, 448(x14)\n"
			"flw.ps f8, 800(x14)\n"
			"fmin.ps f19, f9, f8\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f1, 544(x12)\n"
			"flw.ps f26, 768(x12)\n"
			"flt.s x20, f1, f26\n" 
			"la x6, rand_int_32\n"
			"flw.ps f13, 0(x6)\n"
			"flw.ps f20, 704(x6)\n"
			"fadd.pi f11, f13, f20\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f14, 160(x13)\n"
			"flw.ps f20, 640(x13)\n"
			"fsub.ps f17, f14, f20, rdn\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f1, 288(x20)\n"
			"fcvt.wu.s x13, f1, rtz\n" 
			"la x23, rand_int_32\n"
			"flw.ps f5, 672(x23)\n"
			"fnot.pi f0, f5\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f17, 224(x18)\n"
			"fsqrt.s f0, f17, rdn\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_1_RDX_ID_1_SHIRE_0_HID_8:\n"
			"li x15, 0x140000000330000b\n"
			"csrw tensor_reduce, x15\n" 
			"csrwi tensor_wait, 9\n" 
			"LBL_SEQID_1_TLD_XMA_STR_SEQ_ID_263227_HID_8:\n"
			"la x31, self_check_32\n"
			"li x5, 0x60000000000009\n"
			"add x5, x5, x31\n"
			"li x6, 0x1000000000000a\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x1 << TENSOR_FMA32_B_NUM_COLS) | (0x9 << TENSOR_FMA32_A_NUM_ROWS) | (0xa << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x1 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1f << TENSOR_FMA32_SCP_LOC_B) | (0x3 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x12 << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0x9 << TENSOR_QUANT_QUANT_ROW) | (0x13 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x2 << TENSOR_QUANT_TRANSF9) | (0x6 << TENSOR_QUANT_TRANSF8) | (0x6 << TENSOR_QUANT_TRANSF7) | (0x7 << TENSOR_QUANT_TRANSF6) | (0x7 << TENSOR_QUANT_TRANSF5) | (0x7 << TENSOR_QUANT_TRANSF4) | (0x6 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
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
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=0); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"LBL_SEQID_1_TLD_XMA_STR_SEQ_ITER_0_HID_8:\n"
			"la x31, self_check_32\n"
			"li x5, 0x3\n"
			"add x5, x5, x31\n"
			"li x6, 0x1000000000000b\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x2 << TENSOR_FMA32_B_NUM_COLS) | (0x3 << TENSOR_FMA32_A_NUM_ROWS) | (0xb << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x1 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x19 << TENSOR_FMA32_SCP_LOC_B) | (0x0 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0xc\n"
			"bne x5, x31, LBL_FAIL_HID_8\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_8\n"
			"csrw tensor_error, x0\n"
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_8\n"
			"addi x30, x30, 1\n"
			"la x12, rand_ieee754_32\n"
			"flw.ps f16, 288(x12)\n"
			"flw.ps f11, 576(x12)\n"
			"fsgnjx.s f9, f16, f11\n" 
			"la x18, rand_int_32\n"
			"flw.ps f9, 480(x18)\n"
			"fcvt.ps.pw f26, f9, rtz\n" 
			"LBL_SEQID_2_M0_WRITE_ID_3_HID_8:\n"
			"maskand m2, m3, m3\n" 
			"li x5, CREDINC0\n"
			"ld x5, 0(x5)\n"
			"la x23, rand_ieee754_32\n"
			"flw.ps f9, 576(x23)\n"
			"fsin.ps f0, f9\n" 
			"la x18, rand_int_32\n"
			"flw.ps f28, 64(x18)\n"
			"flw.ps f26, 416(x18)\n"
			"flt.pi f21, f28, f26\n" 
			"li x5, 0x6002\n"
			"csrw tensor_mask, x5\n"	
			"la x6, rand_int_32\n"
			"flw.ps f26, 288(x6)\n"
			"fsrai.pi f5, f26, 0x1\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f0, 704(x14)\n"
			"flw.ps f14, 992(x14)\n"
			"flw.ps f19, 640(x14)\n"
			"fnmadd.s f28, f0, f14, f19, rmm\n" 
			"LBL_SEQID_2_FP_TRANS_ID_4_HID_8:\n"
			"la x23, rand_ieee754_32\n"
			"flw.ps f16, 544(x23)\n"
			"flog.ps f9, f16\n" 
			"LBL_SEQID_2_M0_WRITE_ID_5_HID_8:\n"
			"flem.ps m6, f21, f28\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x13, rand_ieee754_32\n"
			"flw.ps f5, 480(x13)\n"
			"flw.ps f21, 512(x13)\n"
			"fle.s x14, f5, f21\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f0, 256(x23)\n"
			"flw.ps f8, 736(x23)\n"
			"flw.ps f20, 768(x23)\n"
			"fmadd.ps f5, f0, f8, f20, rup\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f11, 64(x23)\n"
			"flw.ps f20, 320(x23)\n"
			"flw.ps f21, 640(x23)\n"
			"fmsub.ps f19, f11, f20, f21, rmm\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f26, 672(x26)\n"
			"fsqrt.s f28, f26, rup\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f1, 480(x6)\n"
			"flw.ps f17, 800(x6)\n"
			"fsgnjx.ps f13, f1, f17\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f11, 416(x20)\n"
			"flw.ps f19, 832(x20)\n"
			"flt.s x6, f11, f19\n" 
			"la x18, rand_int_32\n"
			"flw.ps f0, 160(x18)\n"
			"flw.ps f17, 448(x18)\n"
			"fsrl.pi f8, f0, f17\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_2_TLD_XMA_STR_SEQ_ID_169883_HID_8:\n"
			"la x31, self_check_32\n"
			"li x5, 0x6000000000000c\n"
			"add x5, x5, x31\n"
			"li x6, 0x10000000000009\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x2 << TENSOR_FMA32_B_NUM_COLS) | (0xc << TENSOR_FMA32_A_NUM_ROWS) | (0x9 << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x1 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x18 << TENSOR_FMA32_SCP_LOC_B) | (0x3 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0xa\n"
			"bne x5, x31, LBL_FAIL_HID_8\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_8\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_2_XMA_QNT_STR_SEQ_ITER_0_HID_8:\n"
			"li x6, ((0xa << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0xc << TENSOR_QUANT_QUANT_ROW) | (0x18 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x9 << TENSOR_QUANT_TRANSF8) | (0x3 << TENSOR_QUANT_TRANSF7) | (0x5 << TENSOR_QUANT_TRANSF6) | (0x3 << TENSOR_QUANT_TRANSF5) | (0x8 << TENSOR_QUANT_TRANSF4) | (0x4 << TENSOR_QUANT_TRANSF3) | (0x8 << TENSOR_QUANT_TRANSF2) | (0x9 << TENSOR_QUANT_TRANSF1) | (0x5 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_8\n"
			"csrw tensor_error, x0\n"
			"li x5, 3\n" 
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
			"csrwi mhpmevent3, 11\n"   
			"csrwi mhpmevent4, 6\n"   
			"csrwi mhpmevent5, 13\n"   
			"csrwi mhpmevent6, 23\n"   
			"csrwi mhpmevent7, 2\n"   
			"csrwi mhpmevent8, 1\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f27,  480 (x5)\n"
			"flw.ps f10,  160 (x5)\n"
			"flw.ps f2,  320 (x5)\n"
			"flw.ps f25,  128 (x5)\n"
			"flw.ps f3,  960 (x5)\n"
			"flw.ps f21,  768 (x5)\n"
			"flw.ps f12,  864 (x5)\n"
			"flw.ps f30,  896 (x5)\n"
			"flw.ps f11,  96 (x5)\n"
			"flw.ps f5,  992 (x5)\n"
			"flw.ps f6,  32 (x5)\n"
			"flw.ps f9,  704 (x5)\n"
			"flw.ps f13,  832 (x5)\n"
			"flw.ps f17,  640 (x5)\n"
			"flw.ps f20,  544 (x5)\n"
			"flw.ps f22,  448 (x5)\n"
			"flw.ps f23,  928 (x5)\n"
			"flw.ps f29,  224 (x5)\n"
			"flw.ps f14,  0 (x5)\n"
			"flw.ps f4,  416 (x5)\n"
			"flw.ps f31,  736 (x5)\n"
			"flw.ps f26,  288 (x5)\n"
			"flw.ps f24,  672 (x5)\n"
			"flw.ps f16,  192 (x5)\n"
			"flw.ps f7,  256 (x5)\n"
			"flw.ps f1,  64 (x5)\n"
			"flw.ps f19,  384 (x5)\n"
			"flw.ps f28,  576 (x5)\n"
			"flw.ps f15,  800 (x5)\n"
			"flw.ps f0,  512 (x5)\n"
			"flw.ps f8,  608 (x5)\n"
			"flw.ps f18,  352 (x5)\n"
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
			"li x5, 0x3\n" 
			"sw x5, (x31)\n"
			"li x29, 0x6\n"
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
			"la x15, rand_ieee754_32\n"
			"flw.ps f18, 96(x15)\n"
			"fcvt.pw.ps f7, f18, rdn\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f23, 768(x23)\n"
			"fcvt.l.s x16, f23, rup\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f26, 800(x6)\n"
			"fcvt.pw.ps f2, f26, rup\n" 
			"la x22, rand_int_32\n"
			"flw.ps f26, 672(x22)\n"
			"flw.ps f31, 544(x22)\n"
			"feq.pi f29, f26, f31\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f0, 160(x6)\n"
			"flw.ps f18, 992(x6)\n"
			"flw.ps f2, 64(x6)\n"
			"fnmsub.ps f17, f0, f18, f2, rmm\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f10, 96(x20)\n"
			"flw.ps f31, 640(x20)\n"
			"flw.ps f22, 160(x20)\n"
			"fnmsub.s f2, f10, f31, f22, rne\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_9:\n"
			"mov.m.x m2, x5, 0x2b\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x22, rand_int_32\n"
			"flw.ps f18, 32(x22)\n"
			"flw.ps f2, 864(x22)\n"
			"fmulh.pi f13, f18, f2\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_9:\n"
			"maskand m2, m7, m5\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"fbci.pi f0, 0x41929\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f10, 64(x25)\n"
			"flw.ps f1, 736(x25)\n"
			"fmsub.ps f1, f10, f10, f1, rup\n" 
			"la x16, rand_int_32\n"
			"flw.ps f10, 320(x16)\n"
			"fcvt.ps.pwu f29, f10, rtz\n" 
			"la x14, rand_int_32\n"
			"flw.ps f5, 96(x14)\n"
			"fpackreph.pi f9, f5\n" 
			"LBL_SEQID_0_M0_WRITE_ID_2_HID_9:\n"
			"fltm.ps m7, f26, f31\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0x87f9052b50\n"
			"flw.ps f5, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x16, rand_ieee754_32\n"
			"flw.ps f5, 384(x16)\n"
			"flw.ps f26, 320(x16)\n"
			"flw.ps f10, 576(x16)\n"
			"fnmadd.ps f31, f5, f26, f10, rtz\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f14, 224(x22)\n"
			"flw.ps f31, 0(x22)\n"
			"flw.ps f18, 704(x22)\n"
			"fnmsub.ps f14, f14, f31, f18, rup\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_9\n"
			"addi x30, x30, 1\n"
			"la x22, rand_ieee754_32\n"
			"flw.ps f17, 224(x22)\n"
			"flw.ps f1, 800(x22)\n"
			"fle.s x14, f17, f1\n" 
			"LBL_SEQID_1_M0_WRITE_ID_3_HID_9:\n"
			"mova.m.x x6\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x27, rand_int_32\n"
			"flw.ps f22, 128(x27)\n"
			"fsrai.pi f10, f22, 0xb\n" 
			"la x20, rand_int_32\n"
			"flw.ps f31, 96(x20)\n"
			"flw.ps f14, 544(x20)\n"
			"fmaxu.pi f13, f31, f14\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f1, 704(x16)\n"
			"flw.ps f31, 672(x16)\n"
			"flw.ps f23, 0(x16)\n"
			"fmsub.s f29, f1, f31, f23, rup\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f29, 256(x14)\n"
			"flw.ps f2, 800(x14)\n"
			"fmax.ps f31, f29, f2\n" 
			"la x28, rand_int_32\n"
			"flw.ps f26, 608(x28)\n"
			"flw.ps f10, 384(x28)\n"
			"fltu.pi f17, f26, f10\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f2, 800(x22)\n"
			"fcvt.w.s x25, f2, rup\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f5, 704(x16)\n"
			"fcvt.f16.ps f2, f5\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f2, 320(x22)\n"
			"fcvt.wu.s x15, f2, rmm\n" 
			"la x25, rand_int_32\n"
			"flw.ps f14, 800(x25)\n"
			"flw.ps f1, 672(x25)\n"
			"fsll.pi f17, f14, f1\n" 
			"la x14, rand_int_32\n"
			"flw.ps f9, 704(x14)\n"
			"flw.ps f23, 704(x14)\n"
			"fadd.pi f13, f9, f23\n" 
			"la x28, rand_int_32\n"
			"flw.ps f0, 800(x28)\n"
			"flw.ps f29, 832(x28)\n"
			"fand.pi f0, f0, f29\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f0, 0(x27)\n"
			"flw.ps f14, 0(x27)\n"
			"flw.ps f5, 960(x27)\n"
			"fmadd.ps f18, f0, f14, f5, rup\n" 
			"la x27, rand_int_32\n"
			"flw.ps f13, 0(x27)\n"
			"flw.ps f26, 864(x27)\n"
			"fsrl.pi f9, f13, f26\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f5, 416(x6)\n"
			"flw.ps f29, 352(x6)\n"
			"fsgnjx.s f23, f5, f29\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_9\n"
			"addi x30, x30, 1\n"
			"la x20, rand_ieee754_32\n"
			"flw.ps f1, 192(x20)\n"
			"flw.ps f17, 640(x20)\n"
			"fdiv.ps f23, f1, f17, rtz\n" 
			"LBL_SEQID_2_FP_TRANS_ID_4_HID_9:\n"
			"la x5, rand_ieee754_32\n"
			"flw.ps f13, 256(x5)\n"
			"frcp.ps f23, f13\n" 
			"la x20, rand_int_32\n"
			"flw.ps f17, 896(x20)\n"
			"fdivu.pi f23, f17, f17\n" 
			"la x20, rand_int_32\n"
			"flw.ps f9, 992(x20)\n"
			"flw.ps f23, 992(x20)\n"
			"fmaxu.pi f23, f9, f23\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f14, 512(x16)\n"
			"flw.ps f31, 864(x16)\n"
			"flw.ps f1, 576(x16)\n"
			"fcmov.ps f13, f14, f31, f1\n" 
			"LBL_SEQID_2_FP_TRANS_ID_5_HID_9:\n"
			"la x22, rand_ieee754_32\n"
			"flw.ps f23, 960(x22)\n"
			"frcp.ps f7, f23\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f2, 832(x14)\n"
			"flw.ps f22, 512(x14)\n"
			"flw.ps f0, 832(x14)\n"
			"fmadd.s f9, f2, f22, f0, rup\n" 
			"la x16, rand_int_32\n"
			"flw.ps f10, 512(x16)\n"
			"fcvt.ps.pw f31, f10, rmm\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f13, 416(x14)\n"
			"flw.ps f7, 480(x14)\n"
			"fsgnj.s f5, f13, f7\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f23, 768(x6)\n"
			"ffrc.ps f17, f23\n" 
			"LBL_SEQID_2_M0_WRITE_ID_6_HID_9:\n"
			"maskor m2, m5, m6\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0xad42dca9bb\n"
			"fsw.ps f0, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x20, rand_ieee754_32\n"
			"flw.ps f23, 0(x20)\n"
			"fcvt.pw.ps f0, f23, rmm\n" 
			"LBL_SEQID_2_M0_WRITE_ID_7_HID_9:\n"
			"fltm.ps m7, f17, f5\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"maskpopcz x20, m6\n" 
			"la x14, rand_int_32\n"
			"flw.ps f10, 608(x14)\n"
			"flw.ps f5, 416(x14)\n"
			"fmin.pi f17, f10, f5\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f13, 288(x22)\n"
			"flw.ps f29, 96(x22)\n"
			"flw.ps f14, 544(x22)\n"
			"fmadd.ps f14, f13, f29, f14, rne\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 3\n" 
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
			"csrwi mhpmevent3, 26\n"   
			"csrwi mhpmevent4, 17\n"   
			"csrwi mhpmevent5, 19\n"   
			"csrwi mhpmevent6, 15\n"   
			"csrwi mhpmevent7, 7\n"   
			"csrwi mhpmevent8, 4\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f14,  96 (x5)\n"
			"flw.ps f29,  288 (x5)\n"
			"flw.ps f30,  576 (x5)\n"
			"flw.ps f31,  224 (x5)\n"
			"flw.ps f2,  992 (x5)\n"
			"flw.ps f22,  544 (x5)\n"
			"flw.ps f17,  704 (x5)\n"
			"flw.ps f7,  672 (x5)\n"
			"flw.ps f15,  480 (x5)\n"
			"flw.ps f26,  416 (x5)\n"
			"flw.ps f6,  352 (x5)\n"
			"flw.ps f13,  640 (x5)\n"
			"flw.ps f8,  0 (x5)\n"
			"flw.ps f10,  256 (x5)\n"
			"flw.ps f18,  864 (x5)\n"
			"flw.ps f0,  768 (x5)\n"
			"flw.ps f28,  64 (x5)\n"
			"flw.ps f21,  448 (x5)\n"
			"flw.ps f27,  928 (x5)\n"
			"flw.ps f11,  32 (x5)\n"
			"flw.ps f24,  384 (x5)\n"
			"flw.ps f9,  832 (x5)\n"
			"flw.ps f5,  800 (x5)\n"
			"flw.ps f25,  192 (x5)\n"
			"flw.ps f16,  160 (x5)\n"
			"flw.ps f3,  320 (x5)\n"
			"flw.ps f20,  512 (x5)\n"
			"flw.ps f1,  960 (x5)\n"
			"flw.ps f19,  128 (x5)\n"
			"flw.ps f4,  608 (x5)\n"
			"flw.ps f23,  896 (x5)\n"
			"flw.ps f12,  736 (x5)\n"
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
			"li x5, 0x3\n" 
			"sw x5, (x31)\n"
			"li x29, 0x6\n"
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
			"la x6, rand_ieee754_32\n"
			"flw.ps f20, 96(x6)\n"
			"fcvt.pwu.ps f10, f20, rmm\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f27, 832(x27)\n"
			"flw.ps f26, 576(x27)\n"
			"fsgnjn.ps f29, f27, f26\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_10:\n"
			"flem.ps m4, f30, f20\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"maskpopc x9, m4\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f27, 704(x17)\n"
			"flw.ps f19, 704(x17)\n"
			"flw.ps f10, 544(x17)\n"
			"fnmsub.s f31, f27, f19, f10, rtz\n" 
			"maskpopcz x17, m7\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f20, 768(x23)\n"
			"fcvt.w.s x17, f20, rdn\n" 
			"la x22, rand_int_32\n"
			"flw.ps f19, 480(x22)\n"
			"flw.ps f22, 160(x22)\n"
			"fxor.pi f0, f19, f22\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f7, 992(x17)\n"
			"flw.ps f10, 704(x17)\n"
			"fmax.s f10, f7, f10\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_10:\n"
			"masknot m7, m4\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x12, rand_ieee754_32\n"
			"flw.ps f10, 0(x12)\n"
			"flw.ps f29, 64(x12)\n"
			"flw.ps f26, 544(x12)\n"
			"fcmov.ps f19, f10, f29, f26\n" 
			"la x22, rand_int_32\n"
			"flw.ps f7, 768(x22)\n"
			"flw.ps f27, 576(x22)\n"
			"fmul.pi f4, f7, f27\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f0, 832(x5)\n"
			"fdiv.ps f31, f0, f0, rtz\n" 
			"la x27, rand_int_32\n"
			"flw.ps f10, 512(x27)\n"
			"fcvt.ps.pw f30, f10, rup\n" 
			"la x23, rand_int_32\n"
			"flw.ps f0, 128(x23)\n"
			"flw.ps f20, 224(x23)\n"
			"fxor.pi f8, f0, f20\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f10, 640(x6)\n"
			"flw.ps f4, 992(x6)\n"
			"fnmadd.ps f0, f10, f4, f4, rup\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_0_RDX_ID_0_SHIRE_0_HID_10:\n"
			"li x7, 0x240000000707000a\n"
			"csrw tensor_reduce, x7\n" 
			"csrwi tensor_wait, 9\n" 
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=0); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"LBL_SEQID_0_XMA_QNT_STR_SEQ_ITER_0_HID_10:\n"
			"li x5, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x1 << TENSOR_IMA_B_NUM_COLS) | (0x8 << TENSOR_IMA_A_NUM_ROWS) | (0xa << TENSOR_IMA_A_NUM_COLS) | (0x6 << TENSOR_IMA_OFFSET) | (0x0 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x9a << TENSOR_IMA_SCP_LOC_B) | (0xf << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_10\n"
			"csrw tensor_error, x0\n"
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_10\n"
			"addi x30, x30, 1\n"
			"la x7, rand_ieee754_32\n"
			"flw.ps f10, 96(x7)\n"
			"flw.ps f14, 736(x7)\n"
			"flt.ps f2, f10, f14\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f2, 384(x17)\n"
			"fround.ps f27, f2, rtz\n" 
			"la x5, rand_int_32\n"
			"flw.ps f14, 928(x5)\n"
			"flw.ps f10, 544(x5)\n"
			"fmax.pi f10, f14, f10\n" 
			"LBL_SEQID_1_M0_WRITE_ID_2_HID_10:\n"
			"fltm.pi m4, f27, f13\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_1_M0_WRITE_ID_3_HID_10:\n"
			"maskxor m4, m7, m1\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x6, rand_int_32\n"
			"flw.ps f0, 992(x6)\n"
			"flw.ps f4, 960(x6)\n"
			"flt.pi f7, f0, f4\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f26, 192(x12)\n"
			"fcvt.pwu.ps f2, f26, rup\n" 
			"LBL_SEQID_1_M0_WRITE_ID_4_HID_10:\n"
			"mova.m.x x7\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x22, rand_int_32\n"
			"flw.ps f4, 960(x22)\n"
			"flw.ps f31, 320(x22)\n"
			"fminu.pi f20, f4, f31\n" 
			"la x6, rand_int_32\n"
			"flw.ps f26, 96(x6)\n"
			"flw.ps f27, 0(x6)\n"
			"fand.pi f14, f26, f27\n" 
			"la x12, rand_int_32\n"
			"flw.ps f26, 448(x12)\n"
			"fnot.pi f10, f26\n" 
			"la x6, rand_int_32\n"
			"flw.ps f10, 192(x6)\n"
			"fpackreph.pi f26, f10\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f7, 544(x15)\n"
			"flw.ps f27, 800(x15)\n"
			"fsub.s f10, f7, f27, rtz\n" 
			"la x9, rand_ieee754_32\n"
			"flw.ps f10, 640(x9)\n"
			"fcvt.f16.ps f31, f10\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f30, 928(x17)\n"
			"flw.ps f2, 0(x17)\n"
			"flw.ps f19, 128(x17)\n"
			"fnmadd.s f7, f30, f2, f19, rne\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f31, 928(x27)\n"
			"flw.ps f19, 128(x27)\n"
			"fmax.ps f13, f31, f19\n" 
			"li x5, 0x3483816825f9e6c2\n" 
			"csrw tensor_conv_size, x5\n"	
			"li x5, 0xe61d8ac61866da76\n"
			"csrw tensor_conv_ctrl, x5\n"	
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ID_828644_HID_10:\n"
			"li x5, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x0 << TENSOR_FMA32_B_NUM_COLS) | (0xe << TENSOR_FMA32_A_NUM_ROWS) | (0x2 << TENSOR_FMA32_A_NUM_COLS) | (0xb << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x40 << TENSOR_FMA32_SCP_LOC_B) | (0x90 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ID_351528_HID_10:\n"
			"li x5, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x0 << TENSOR_IMA_B_NUM_COLS) | (0x3 << TENSOR_IMA_A_NUM_ROWS) | (0xd << TENSOR_IMA_A_NUM_COLS) | (0xd << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x1 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x81 << TENSOR_IMA_SCP_LOC_B) | (0xea << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x0 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_10\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_1_RDX_ID_1_SHIRE_0_HID_10:\n"
			"li x9, 0x140000000330000b\n"
			"csrw tensor_reduce, x9\n" 
			"csrwi tensor_wait, 9\n" 
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=0); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"li x5, 0x51e\n"
			"csrw tensor_mask, x5\n"	
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ITER_0_HID_10:\n"
			"li x6, ((0x10 << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0x9 << TENSOR_QUANT_QUANT_ROW) | (0x2b << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x7 << TENSOR_QUANT_TRANSF4) | (0x6 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ITER_1_HID_10:\n"
			"li x6, ((0xe << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0x3 << TENSOR_QUANT_QUANT_ROW) | (0x3 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x2 << TENSOR_QUANT_TRANSF9) | (0x7 << TENSOR_QUANT_TRANSF8) | (0x7 << TENSOR_QUANT_TRANSF7) | (0x7 << TENSOR_QUANT_TRANSF6) | (0x7 << TENSOR_QUANT_TRANSF5) | (0x6 << TENSOR_QUANT_TRANSF4) | (0x7 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_10\n"
			"csrw tensor_error, x0\n"
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_10\n"
			"addi x30, x30, 1\n"
			"la x7, rand_ieee754_32\n"
			"flw.ps f31, 992(x7)\n"
			"fsgnj.s f19, f31, f31\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f13, 32(x15)\n"
			"flw.ps f22, 736(x15)\n"
			"fcmovm.ps f2, f13, f22\n" 
			"la x12, rand_int_32\n"
			"flw.ps f2, 800(x12)\n"
			"flw.ps f30, 960(x12)\n"
			"for.pi f0, f2, f30\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f2, 448(x12)\n"
			"flw.ps f4, 32(x12)\n"
			"fcmov.ps f26, f2, f4, f4\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f2, 864(x15)\n"
			"flw.ps f29, 224(x15)\n"
			"fadd.ps f19, f2, f29, rtz\n" 
			"la x6, rand_int_32\n"
			"flw.ps f20, 384(x6)\n"
			"flw.ps f8, 832(x6)\n"
			"fsra.pi f0, f20, f8\n" 
			"la x27, rand_int_32\n"
			"flw.ps f19, 32(x27)\n"
			"fpackreph.pi f10, f19\n" 
			"la x23, rand_ieee754_32\n"
			"flw.ps f26, 544(x23)\n"
			"fnmadd.s f0, f26, f26, f26, rtz\n" 
			"la x15, rand_int_32\n"
			"flw.ps f27, 512(x15)\n"
			"flw.ps f2, 736(x15)\n"
			"fand.pi f13, f27, f2\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x17, rand_int_32\n"
			"flw.ps f22, 320(x17)\n"
			"flw.ps f19, 384(x17)\n"
			"fmul.pi f4, f22, f19\n" 
			"la x15, rand_int_32\n"
			"flw.ps f20, 608(x15)\n"
			"flw.ps f30, 896(x15)\n"
			"fand.pi f13, f20, f30\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f0, 160(x5)\n"
			"flw.ps f31, 448(x5)\n"
			"fsub.s f27, f0, f31, rdn\n" 
			"fbci.pi f13, 0x21181\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f19, 96(x17)\n"
			"fcvt.pwu.ps f10, f19, rmm\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f30, 864(x17)\n"
			"flw.ps f31, 32(x17)\n"
			"fmul.ps f22, f30, f31, rne\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x12, rand_int_32\n"
			"flw.ps f31, 704(x12)\n"
			"fsatu8.pi f8, f31\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_2_TLD_XMA_STR_SEQ_ITER_0_HID_10:\n"
			"la x31, self_check_32\n"
			"li x5, 0xc000000000000b\n"
			"add x5, x5, x31\n"
			"li x6, 0x340000000000002\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x2 << TENSOR_FMA32_B_NUM_COLS) | (0xb << TENSOR_FMA32_A_NUM_ROWS) | (0x2 << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1a << TENSOR_FMA32_SCP_LOC_B) | (0x6 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
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
			"bne x5, x31, LBL_FAIL_HID_10\n"
			"LBL_SEQID_2_TLD_XMA_STR_SEQ_ITER_1_HID_10:\n"
			"la x31, self_check_32\n"
			"li x5, 0x1\n"
			"add x5, x5, x31\n"
			"li x6, 0x1000000000000e\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x2 << TENSOR_FMA32_B_NUM_COLS) | (0x1 << TENSOR_FMA32_A_NUM_ROWS) | (0xe << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x1 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1e << TENSOR_FMA32_SCP_LOC_B) | (0x0 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0xf\n"
			"bne x5, x31, LBL_FAIL_HID_10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_10\n"
			"csrw tensor_error, x0\n"
			"li x5, 3\n" 
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
			"csrwi mhpmevent3, 11\n"   
			"csrwi mhpmevent4, 6\n"   
			"csrwi mhpmevent5, 13\n"   
			"csrwi mhpmevent6, 23\n"   
			"csrwi mhpmevent7, 2\n"   
			"csrwi mhpmevent8, 1\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f8,  480 (x5)\n"
			"flw.ps f17,  640 (x5)\n"
			"flw.ps f2,  960 (x5)\n"
			"flw.ps f16,  896 (x5)\n"
			"flw.ps f9,  352 (x5)\n"
			"flw.ps f12,  32 (x5)\n"
			"flw.ps f24,  800 (x5)\n"
			"flw.ps f31,  864 (x5)\n"
			"flw.ps f21,  416 (x5)\n"
			"flw.ps f30,  544 (x5)\n"
			"flw.ps f29,  192 (x5)\n"
			"flw.ps f13,  704 (x5)\n"
			"flw.ps f25,  384 (x5)\n"
			"flw.ps f5,  608 (x5)\n"
			"flw.ps f11,  256 (x5)\n"
			"flw.ps f19,  288 (x5)\n"
			"flw.ps f3,  448 (x5)\n"
			"flw.ps f20,  0 (x5)\n"
			"flw.ps f4,  224 (x5)\n"
			"flw.ps f1,  160 (x5)\n"
			"flw.ps f28,  832 (x5)\n"
			"flw.ps f27,  320 (x5)\n"
			"flw.ps f10,  128 (x5)\n"
			"flw.ps f7,  992 (x5)\n"
			"flw.ps f22,  512 (x5)\n"
			"flw.ps f26,  64 (x5)\n"
			"flw.ps f23,  768 (x5)\n"
			"flw.ps f6,  672 (x5)\n"
			"flw.ps f15,  96 (x5)\n"
			"flw.ps f0,  928 (x5)\n"
			"flw.ps f18,  736 (x5)\n"
			"flw.ps f14,  576 (x5)\n"
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
			"li x5, 0x3\n" 
			"sw x5, (x31)\n"
			"li x29, 0x6\n"
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
			"LBL_SEQID_0_FP_TRANS_ID_0_HID_11:\n"
			"la x5, rand_ieee754_32\n"
			"flw.ps f0, 640(x5)\n"
			"flog.ps f31, f0\n" 
			"li x14, 0xe666beb0820a9466\n"
			"fcvt.s.lu f15, x14, rtz\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f8, 768(x5)\n"
			"fcvt.pwu.ps f0, f8, rne\n" 
			"la x11, rand_int_32\n"
			"flw.ps f11, 896(x11)\n"
			"fpackrepb.pi f1, f11\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f6, 448(x11)\n"
			"flw.ps f0, 800(x11)\n"
			"fmin.s f6, f6, f0\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f12, 672(x20)\n"
			"flw.ps f25, 256(x20)\n"
			"flw.ps f4, 576(x20)\n"
			"fnmsub.s f0, f12, f25, f4, rup\n" 
			"la x18, rand_int_32\n"
			"flw.ps f4, 800(x18)\n"
			"flw.ps f12, 0(x18)\n"
			"fminu.pi f6, f4, f12\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_11:\n"
			"fltm.pi m2, f26, f25\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x15, rand_int_32\n"
			"flw.ps f15, 224(x15)\n"
			"flw.ps f20, 768(x15)\n"
			"fmulhu.pi f11, f15, f20\n" 
			"LBL_SEQID_0_M0_WRITE_ID_2_HID_11:\n"
			"masknot m7, m2\n" 
			"li x5, CREDINC1\n"
			"ld x5, 0(x5)\n"
			"la x11, rand_ieee754_32\n"
			"flw.ps f18, 640(x11)\n"
			"flw.ps f8, 608(x11)\n"
			"flw.ps f13, 640(x11)\n"
			"fmsub.ps f19, f18, f8, f13, rdn\n" 
			"la x11, rand_ieee754_32\n"
			"flw.ps f8, 736(x11)\n"
			"flw.ps f31, 64(x11)\n"
			"fsgnjn.ps f15, f8, f31\n" 
			"LBL_SEQID_0_M0_WRITE_ID_3_HID_11:\n"
			"maskxor m4, m4, m2\n" 
			"li x5, 0x80340138\n"
			"ld x5, 0(x5)\n"
			"la x15, rand_ieee754_32\n"
			"flw.ps f12, 320(x15)\n"
			"fcvt.lu.s x12, f12, rdn\n" 
			"la x26, rand_int_32\n"
			"flw.ps f0, 992(x26)\n"
			"fandi.pi f20, f0, 0x19d\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f11, 576(x18)\n"
			"flw.ps f15, 224(x18)\n"
			"fmin.s f20, f11, f15\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_11\n"
			"addi x30, x30, 1\n"
			"la x26, rand_ieee754_32\n"
			"flw.ps f4, 0(x26)\n"
			"flw.ps f25, 288(x26)\n"
			"flw.ps f24, 640(x26)\n"
			"fnmsub.ps f31, f4, f25, f24, rup\n" 
			"la x15, rand_int_32\n"
			"flw.ps f24, 320(x15)\n"
			"fcvt.ps.pw f1, f24, rtz\n" 
			"la x12, rand_int_32\n"
			"flw.ps f24, 64(x12)\n"
			"flw.ps f13, 480(x12)\n"
			"fsrl.pi f13, f24, f13\n" 
			"LBL_SEQID_1_M0_WRITE_ID_4_HID_11:\n"
			"feqm.ps m4, f20, f26\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x7, rand_ieee754_32\n"
			"flw.ps f0, 672(x7)\n"
			"flw.ps f13, 512(x7)\n"
			"fmul.s f8, f0, f13, rdn\n" 
			"la x11, rand_int_32\n"
			"flw.ps f6, 576(x11)\n"
			"flw.ps f26, 320(x11)\n"
			"fmax.pi f31, f6, f26\n" 
			"LBL_SEQID_1_M0_WRITE_ID_5_HID_11:\n"
			"fltm.ps m5, f11, f0\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x7, rand_ieee754_32\n"
			"flw.ps f6, 832(x7)\n"
			"fcvt.f16.ps f11, f6\n" 
			"la x20, rand_int_32\n"
			"flw.ps f4, 224(x20)\n"
			"flw.ps f26, 64(x20)\n"
			"fminu.pi f26, f4, f26\n" 
			"maskpopc x18, m2\n" 
			"la x20, rand_int_32\n"
			"flw.ps f18, 288(x20)\n"
			"faddi.pi f31, f18, 0x7d\n" 
			"LBL_SEQID_1_M0_WRITE_ID_6_HID_11:\n"
			"maskand m7, m2, m7\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_1_M0_WRITE_ID_7_HID_11:\n"
			"flem.ps m5, f1, f24\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x18, rand_ieee754_32\n"
			"flw.ps f12, 288(x18)\n"
			"flw.ps f18, 928(x18)\n"
			"fmin.s f6, f12, f18\n" 
			"LBL_SEQID_1_FP_TRANS_ID_8_HID_11:\n"
			"la x18, rand_ieee754_32\n"
			"flw.ps f11, 32(x18)\n"
			"frcp.ps f13, f11\n" 
			"la x7, rand_int_32\n"
			"flw.ps f12, 448(x7)\n"
			"fsat8.pi f19, f12\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_11\n"
			"addi x30, x30, 1\n"
			"la x12, rand_int_32\n"
			"flw.ps f26, 832(x12)\n"
			"fcvt.ps.pw f15, f26, rtz\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f24, 64(x26)\n"
			"fclass.s x7, f24\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f26, 256(x14)\n"
			"fround.ps f15, f26, rne\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f19, 576(x18)\n"
			"flw.ps f8, 896(x18)\n"
			"fdiv.ps f12, f19, f8, rmm\n" 
			"LBL_SEQID_2_M0_WRITE_ID_9_HID_11:\n"
			"maskand m5, m2, m4\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x10, rand_ieee754_32\n"
			"flw.ps f8, 576(x10)\n"
			"flw.ps f26, 832(x10)\n"
			"fmul.ps f13, f8, f26, rtz\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f13, 288(x20)\n"
			"flw.ps f4, 544(x20)\n"
			"fle.ps f26, f13, f4\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f18, 384(x15)\n"
			"flw.ps f8, 224(x15)\n"
			"fsub.s f8, f18, f8, rne\n" 
			"LBL_SEQID_2_M0_WRITE_ID_10_HID_11:\n"
			"maskand m7, m7, m2\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_2_M0_WRITE_ID_11_HID_11:\n"
			"fltm.pi m4, f20, f15\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x26, rand_ieee754_32\n"
			"flw.ps f24, 960(x26)\n"
			"flw.ps f8, 896(x26)\n"
			"flw.ps f11, 160(x26)\n"
			"fmsub.ps f31, f24, f8, f11, rmm\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f1, 960(x10)\n"
			"flw.ps f12, 32(x10)\n"
			"flw.ps f11, 672(x10)\n"
			"fnmadd.ps f31, f1, f12, f11, rmm\n" 
			"la x20, rand_int_32\n"
			"flw.ps f11, 160(x20)\n"
			"fmaxu.pi f1, f11, f11\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f6, 192(x7)\n"
			"flw.ps f13, 704(x7)\n"
			"flw.ps f31, 320(x7)\n"
			"fmsub.s f13, f6, f13, f31, rmm\n" 
			"la x14, rand_int_32\n"
			"flw.ps f19, 896(x14)\n"
			"fsrai.pi f0, f19, 0x6\n" 
			"li x26, 0x47db6f81dfe56b01\n"
			"fcvt.s.lu f12, x26, rup\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 3\n" 
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
			"csrwi mhpmevent3, 26\n"   
			"csrwi mhpmevent4, 17\n"   
			"csrwi mhpmevent5, 19\n"   
			"csrwi mhpmevent6, 15\n"   
			"csrwi mhpmevent7, 7\n"   
			"csrwi mhpmevent8, 4\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f2,  512 (x5)\n"
			"flw.ps f25,  704 (x5)\n"
			"flw.ps f18,  0 (x5)\n"
			"flw.ps f12,  576 (x5)\n"
			"flw.ps f10,  192 (x5)\n"
			"flw.ps f8,  672 (x5)\n"
			"flw.ps f24,  800 (x5)\n"
			"flw.ps f20,  768 (x5)\n"
			"flw.ps f7,  288 (x5)\n"
			"flw.ps f14,  448 (x5)\n"
			"flw.ps f29,  864 (x5)\n"
			"flw.ps f6,  64 (x5)\n"
			"flw.ps f13,  384 (x5)\n"
			"flw.ps f9,  992 (x5)\n"
			"flw.ps f31,  256 (x5)\n"
			"flw.ps f26,  928 (x5)\n"
			"flw.ps f3,  544 (x5)\n"
			"flw.ps f5,  320 (x5)\n"
			"flw.ps f15,  224 (x5)\n"
			"flw.ps f17,  832 (x5)\n"
			"flw.ps f28,  128 (x5)\n"
			"flw.ps f11,  480 (x5)\n"
			"flw.ps f22,  736 (x5)\n"
			"flw.ps f4,  96 (x5)\n"
			"flw.ps f27,  32 (x5)\n"
			"flw.ps f16,  416 (x5)\n"
			"flw.ps f21,  160 (x5)\n"
			"flw.ps f19,  960 (x5)\n"
			"flw.ps f23,  352 (x5)\n"
			"flw.ps f30,  896 (x5)\n"
			"flw.ps f1,  608 (x5)\n"
			"flw.ps f0,  640 (x5)\n"
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
			"li x5, 0x3\n" 
			"sw x5, (x31)\n"
			"li x29, 0x6\n"
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
			"la x15, rand_ieee754_32\n"
			"flw.ps f19, 480(x15)\n"
			"flw.ps f21, 32(x15)\n"
			"fcmovm.ps f18, f19, f21\n" 
			"mova.x.m x23\n" 
			"la x15, rand_int_32\n"
			"flw.ps f19, 704(x15)\n"
			"fsrai.pi f23, f19, 0x2\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_12:\n"
			"maskand m5, m4, m4\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x18, rand_int_32\n"
			"flw.ps f25, 640(x18)\n"
			"flw.ps f26, 128(x18)\n"
			"fltu.pi f1, f25, f26\n" 
			"la x23, rand_int_32\n"
			"flw.ps f5, 32(x23)\n"
			"fcvt.ps.pwu f25, f5, rne\n" 
			"maskpopcz x23, m0\n" 
			"la x23, rand_int_32\n"
			"flw.ps f0, 736(x23)\n"
			"fsatu8.pi f1, f0\n" 
			"li x5, 0x591871566b3b3bb\n"
			"fcvt.s.w f1, x5, rtz\n" 
			"la x22, rand_int_32\n"
			"flw.ps f19, 160(x22)\n"
			"fsatu8.pi f23, f19\n" 
			"la x26, rand_int_32\n"
			"flw.ps f16, 416(x26)\n"
			"flw.ps f1, 96(x26)\n"
			"fmax.pi f18, f16, f1\n" 
			"la x22, rand_int_32\n"
			"flw.ps f13, 512(x22)\n"
			"fslli.pi f6, f13, 0x4\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f1, 736(x22)\n"
			"flw.ps f9, 800(x22)\n"
			"feq.ps f23, f1, f9\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f5, 64(x5)\n"
			"fcvt.pw.ps f6, f5, rmm\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f1, 416(x22)\n"
			"fcvt.l.s x23, f1, rtz\n" 
			"li x23, 0x10395b3b15c050e1\n"
			"fcvt.s.lu f23, x23, rne\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ID_804398_HID_12:\n"
			"la x31, self_check_8\n"
			"li x5, 0xa0000000000004\n"
			"add x5, x5, x31\n"
			"li x6, 0x3e0000000000008\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x0 << TENSOR_IMA_B_NUM_COLS) | (0x4 << TENSOR_IMA_A_NUM_ROWS) | (0x8 << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x0 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1f << TENSOR_IMA_SCP_LOC_B) | (0x5 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fmvz.x.ps x31, f0, 0\n"
			"li x5, 0x24\n"
			"bne x5, x31, LBL_FAIL_HID_12\n"
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ID_540743_HID_12:\n"
			"la x31, self_check_16\n"
			"li x5, 0xd\n"
			"add x5, x5, x31\n"
			"li x6, 0x340000000000002\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x1 << TENSOR_FMA16_B_NUM_COLS) | (0xd << TENSOR_FMA16_A_NUM_ROWS) | (0x2 << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1a << TENSOR_FMA16_SCP_LOC_B) | (0x0 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x9, ((0xe << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0xd << TENSOR_QUANT_QUANT_ROW) | (0x27 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x0 << TENSOR_QUANT_TRANSF1) | (0x2 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"LBL_SEQID_0_RDX_ID_0_SHIRE_0_HID_12:\n"
			"li x15, 0x240000000707000a\n"
			"csrw tensor_reduce, x15\n" 
			"csrwi tensor_wait, 9\n" 
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ID_993891_HID_12:\n"
			"la x31, self_check_16\n"
			"li x5, 0xc0000000000003\n"
			"add x5, x5, x31\n"
			"li x6, 0x10000000000003\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x0 << TENSOR_FMA16_B_NUM_COLS) | (0x3 << TENSOR_FMA16_A_NUM_ROWS) | (0x3 << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x1 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1d << TENSOR_FMA16_SCP_LOC_B) | (0x6 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x15 << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0x3 << TENSOR_QUANT_QUANT_ROW) | (0x23 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x0 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0x8\n"
			"bne x5, x31, LBL_FAIL_HID_12\n"
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 10\n"
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ID_165639_HID_12:\n"
			"la x31, self_check_16\n"
			"li x5, 0x8000000000000d\n"
			"add x5, x5, x31\n"
			"li x6, 0x38000000000000d\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x0 << TENSOR_FMA16_B_NUM_COLS) | (0xd << TENSOR_FMA16_A_NUM_ROWS) | (0xd << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1c << TENSOR_FMA16_SCP_LOC_B) | (0x4 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x12 << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0xd << TENSOR_QUANT_QUANT_ROW) | (0x1d << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x6 << TENSOR_QUANT_TRANSF4) | (0x7 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=0); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ITER_0_HID_12:\n"
			"la x31, self_check_16\n"
			"li x5, 0x80000000000003\n"
			"add x5, x5, x31\n"
			"li x6, 0x10000000000001\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x1 << TENSOR_FMA16_B_NUM_COLS) | (0x3 << TENSOR_FMA16_A_NUM_ROWS) | (0x1 << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x1 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1c << TENSOR_FMA16_SCP_LOC_B) | (0x4 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x17 << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0x3 << TENSOR_QUANT_QUANT_ROW) | (0x0 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x2 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0x4\n"
			"bne x5, x31, LBL_FAIL_HID_12\n"
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 10\n"
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ITER_1_HID_12:\n"
			"la x31, self_check_16\n"
			"li x5, 0xe0000000000000\n"
			"add x5, x5, x31\n"
			"li x6, 0x34000000000000e\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x1 << TENSOR_FMA16_B_NUM_COLS) | (0x0 << TENSOR_FMA16_A_NUM_ROWS) | (0xe << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1a << TENSOR_FMA16_SCP_LOC_B) | (0x7 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
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
			"bne x5, x31, LBL_FAIL_HID_12\n"
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ITER_2_HID_12:\n"
			"la x31, self_check_8\n"
			"li x5, 0x8000000000000a\n"
			"add x5, x5, x31\n"
			"li x6, 0x1000000000000c\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x3 << TENSOR_IMA_B_NUM_COLS) | (0xa << TENSOR_IMA_A_NUM_ROWS) | (0xc << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x1 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1d << TENSOR_IMA_SCP_LOC_B) | (0x4 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x9, ((0x7 << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0xa << TENSOR_QUANT_QUANT_ROW) | (0x3f << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x1 << TENSOR_QUANT_TRANSF1) | (0x9 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fmvz.x.ps x31, f0, 0\n"
			"li x5, 0x34\n"
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
			"la x26, rand_ieee754_32\n"
			"flw.ps f21, 160(x26)\n"
			"fclass.s x27, f21\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f23, 320(x22)\n"
			"flw.ps f13, 736(x22)\n"
			"feq.s x23, f23, f13\n" 
			"la x10, rand_int_32\n"
			"flw.ps f20, 832(x10)\n"
			"fslli.pi f0, f20, 0xb\n" 
			"la x9, rand_ieee754_16\n"
			"flw.ps f23, 288(x9)\n"
			"fcvt.ps.f16 f4, f23\n" 
			"LBL_SEQID_1_FP_TRANS_ID_1_HID_12:\n"
			"la x23, rand_ieee754_32\n"
			"flw.ps f21, 800(x23)\n"
			"fexp.ps f5, f21\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f20, 160(x14)\n"
			"fcvt.w.s x12, f20, rmm\n" 
			"LBL_SEQID_1_M0_WRITE_ID_2_HID_12:\n"
			"maskand m4, m5, m5\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x18, rand_ieee754_32\n"
			"flw.ps f19, 736(x18)\n"
			"flw.ps f21, 480(x18)\n"
			"fsgnj.s f18, f19, f21\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"LBL_SEQID_1_M0_WRITE_ID_3_HID_12:\n"
			"flem.ps m0, f19, f16\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"maskpopc x10, m0\n" 
			"la x10, rand_int_32\n"
			"flw.ps f23, 288(x10)\n"
			"fpackrepb.pi f20, f23\n" 
			"maskpopcz x5, m5\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f6, 864(x27)\n"
			"feq.s x26, f6, f6\n" 
			"la x18, rand_int_32\n"
			"flw.ps f26, 224(x18)\n"
			"flw.ps f19, 672(x18)\n"
			"for.pi f3, f26, f19\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x14, rand_ieee754_32\n"
			"flw.ps f26, 960(x14)\n"
			"fcvt.pwu.ps f18, f26, rtz\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f3, 992(x12)\n"
			"flw.ps f20, 96(x12)\n"
			"fsgnj.s f21, f3, f20\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_1_RDX_ID_1_SHIRE_0_HID_12:\n"
			"li x23, 0x140000000330000b\n"
			"csrw tensor_reduce, x23\n" 
			"csrwi tensor_wait, 9\n" 
			"nop\n"          
		       VSNIP_RSV     
		);
		c_sync_point(sync_odd_harts=0); 
		__asm__ __volatile__ ( 
			"nop\n"          
			"LBL_SEQID_1_TLD_XMA_STR_SEQ_ITER_0_HID_12:\n"
			"la x31, self_check_8\n"
			"li x5, 0x6000000000000e\n"
			"add x5, x5, x31\n"
			"li x6, 0x10000000000007\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x3 << TENSOR_IMA_B_NUM_COLS) | (0xe << TENSOR_IMA_A_NUM_ROWS) | (0x7 << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x0 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x1 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1a << TENSOR_IMA_SCP_LOC_B) | (0x3 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x9, ((0x1b << TENSOR_QUANT_START_REG) | (0x3 << TENSOR_QUANT_QUANT_COL) | (0xe << TENSOR_QUANT_QUANT_ROW) | (0x11 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x1 << TENSOR_QUANT_TRANSF8) | (0x8 << TENSOR_QUANT_TRANSF7) | (0x3 << TENSOR_QUANT_TRANSF6) | (0x9 << TENSOR_QUANT_TRANSF5) | (0x9 << TENSOR_QUANT_TRANSF4) | (0x4 << TENSOR_QUANT_TRANSF3) | (0x9 << TENSOR_QUANT_TRANSF2) | (0xa << TENSOR_QUANT_TRANSF1) | (0x4 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fmvz.x.ps x31, f0, 0\n"
			"li x5, 0x20\n"
			"bne x5, x31, LBL_FAIL_HID_12\n"
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_12\n"
			"csrw tensor_error, x0\n"
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_12\n"
			"addi x30, x30, 1\n"
			"la x18, rand_int_32\n"
			"flw.ps f20, 128(x18)\n"
			"fpackreph.pi f16, f20\n" 
			"la x23, rand_int_32\n"
			"flw.ps f23, 0(x23)\n"
			"flw.ps f16, 96(x23)\n"
			"feq.pi f19, f23, f16\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f5, 32(x14)\n"
			"flw.ps f4, 256(x14)\n"
			"flw.ps f16, 512(x14)\n"
			"fnmsub.ps f19, f5, f4, f16, rne\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f1, 800(x18)\n"
			"flw.ps f21, 800(x18)\n"
			"flt.ps f26, f1, f21\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f4, 896(x27)\n"
			"flw.ps f21, 352(x27)\n"
			"fsub.s f19, f4, f21, rdn\n" 
			"la x12, rand_int_32\n"
			"flw.ps f26, 768(x12)\n"
			"flw.ps f5, 128(x12)\n"
			"fsll.pi f26, f26, f5\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"LBL_SEQID_2_M0_WRITE_ID_4_HID_12:\n"
			"fltm.pi m6, f26, f21\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x27, rand_int_32\n"
			"flw.ps f13, 704(x27)\n"
			"fcvt.ps.pw f20, f13, rne\n" 
			"la x23, rand_int_32\n"
			"flw.ps f21, 544(x23)\n"
			"flw.ps f6, 544(x23)\n"
			"fsrl.pi f19, f21, f6\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f5, 192(x12)\n"
			"flw.ps f20, 256(x12)\n"
			"flw.ps f25, 480(x12)\n"
			"fnmsub.ps f19, f5, f20, f25, rdn\n" 
			"li x9, 0xeee35a717bc32ff8\n"
			"fcvt.s.wu f0, x9, rdn\n" 
			"LBL_SEQID_2_M0_WRITE_ID_5_HID_12:\n"
			"maskxor m4, m0, m0\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x27, rand_ieee754_32\n"
			"flw.ps f18, 992(x27)\n"
			"flw.ps f5, 864(x27)\n"
			"fle.s x18, f18, f5\n" 
			"mova.x.m x9\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f4, 224(x15)\n"
			"flw.ps f6, 928(x15)\n"
			"fmax.ps f19, f4, f6\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f0, 576(x5)\n"
			"frsq.ps f25, f0\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_2_TLD_XMA_STR_SEQ_ITER_0_HID_12:\n"
			"la x31, self_check_32\n"
			"li x5, 0x80000000000001\n"
			"add x5, x5, x31\n"
			"li x6, 0x34000000000000e\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x2 << TENSOR_FMA32_B_NUM_COLS) | (0x1 << TENSOR_FMA32_A_NUM_ROWS) | (0xe << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1a << TENSOR_FMA32_SCP_LOC_B) | (0x4 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0xf\n"
			"bne x5, x31, LBL_FAIL_HID_12\n"
			"LBL_SEQID_2_TLD_XMA_STR_SEQ_ITER_1_HID_12:\n"
			"la x31, self_check_8\n"
			"li x5, 0x2000000000000a\n"
			"add x5, x5, x31\n"
			"li x6, 0x10000000000001\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_IMA_USE_TMASK) | (0x3 << TENSOR_IMA_B_NUM_COLS) | (0xa << TENSOR_IMA_A_NUM_ROWS) | (0x1 << TENSOR_IMA_A_NUM_COLS) | (0x0 << TENSOR_IMA_OFFSET) | (0x1 << TENSOR_IMA_TO_VRF) | (0x1 << TENSOR_IMA_U_B) | (0x0 << TENSOR_IMA_U_A) | (0x1 << TENSOR_IMA_TEN_B_IN_MEM) | (0x1f << TENSOR_IMA_SCP_LOC_B) | (0x1 << TENSOR_IMA_SCP_LOC_A) | (0x3 << TENSOR_IMA_OPC_011) | (0x1 << TENSOR_IMA_RESET_TENC) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fmvz.x.ps x31, f0, 0\n"
			"li x5, 0x8\n"
			"bne x5, x31, LBL_FAIL_HID_12\n"
			"LBL_SEQID_2_TLD_XMA_STR_SEQ_ITER_2_HID_12:\n"
			"la x31, self_check_16\n"
			"li x5, 0x60000000000009\n"
			"add x5, x5, x31\n"
			"li x6, 0x10000000000004\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x1 << TENSOR_FMA16_B_NUM_COLS) | (0x9 << TENSOR_FMA16_A_NUM_ROWS) | (0x4 << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x1 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1d << TENSOR_FMA16_SCP_LOC_B) | (0x3 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x17 << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0x9 << TENSOR_QUANT_QUANT_ROW) | (0x31 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x2 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 0\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0xa\n"
			"bne x5, x31, LBL_FAIL_HID_12\n"
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_12\n"
			"csrw tensor_error, x0\n"
			"li x5, 3\n" 
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
			"csrwi mhpmevent4, 6\n"   
			"csrwi mhpmevent5, 13\n"   
			"csrwi mhpmevent6, 23\n"   
			"csrwi mhpmevent7, 2\n"   
			"csrwi mhpmevent8, 1\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f14,  256 (x5)\n"
			"flw.ps f4,  544 (x5)\n"
			"flw.ps f3,  128 (x5)\n"
			"flw.ps f22,  672 (x5)\n"
			"flw.ps f18,  768 (x5)\n"
			"flw.ps f20,  32 (x5)\n"
			"flw.ps f17,  864 (x5)\n"
			"flw.ps f2,  640 (x5)\n"
			"flw.ps f30,  704 (x5)\n"
			"flw.ps f5,  736 (x5)\n"
			"flw.ps f9,  960 (x5)\n"
			"flw.ps f28,  928 (x5)\n"
			"flw.ps f27,  352 (x5)\n"
			"flw.ps f19,  480 (x5)\n"
			"flw.ps f16,  64 (x5)\n"
			"flw.ps f25,  320 (x5)\n"
			"flw.ps f13,  800 (x5)\n"
			"flw.ps f10,  512 (x5)\n"
			"flw.ps f24,  992 (x5)\n"
			"flw.ps f7,  96 (x5)\n"
			"flw.ps f12,  0 (x5)\n"
			"flw.ps f15,  832 (x5)\n"
			"flw.ps f23,  160 (x5)\n"
			"flw.ps f11,  896 (x5)\n"
			"flw.ps f29,  224 (x5)\n"
			"flw.ps f8,  288 (x5)\n"
			"flw.ps f21,  576 (x5)\n"
			"flw.ps f31,  608 (x5)\n"
			"flw.ps f0,  416 (x5)\n"
			"flw.ps f6,  384 (x5)\n"
			"flw.ps f1,  448 (x5)\n"
			"flw.ps f26,  192 (x5)\n"
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
			"li x5, 0x3\n" 
			"sw x5, (x31)\n"
			"li x29, 0x6\n"
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
			"la x14, rand_int_32\n"
			"flw.ps f1, 352(x14)\n"
			"for.pi f31, f1, f1\n" 
			"la x25, rand_int_32\n"
			"flw.ps f1, 800(x25)\n"
			"flw.ps f30, 576(x25)\n"
			"fmulhu.pi f18, f1, f30\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f0, 480(x14)\n"
			"flw.ps f13, 160(x14)\n"
			"fsgnjn.s f28, f0, f13\n" 
			"la x5, rand_int_32\n"
			"flw.ps f28, 768(x5)\n"
			"fmulhu.pi f4, f28, f28\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f30, 512(x13)\n"
			"flw.ps f13, 256(x13)\n"
			"fsub.s f25, f30, f13, rdn\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f31, 64(x16)\n"
			"flw.ps f20, 704(x16)\n"
			"fadd.ps f20, f31, f20, rmm\n" 
			"la x17, rand_int_32\n"
			"flw.ps f1, 64(x17)\n"
			"fsrli.pi f28, f1, 0x9\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f10, 640(x12)\n"
			"fclass.ps f4, f10\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_13:\n"
			"feqm.ps m7, f18, f8\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0xb6c671c328\n"
			"flw.ps f5, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x13, rand_ieee754_32\n"
			"flw.ps f31, 896(x13)\n"
			"flw.ps f13, 928(x13)\n"
			"fle.s x14, f31, f13\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f4, 480(x17)\n"
			"frsq.ps f25, f4\n" 
			"la x17, rand_int_32\n"
			"flw.ps f27, 448(x17)\n"
			"flw.ps f0, 128(x17)\n"
			"fand.pi f4, f27, f0\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f10, 800(x12)\n"
			"fcvt.pwu.ps f21, f10, rup\n" 
			"la x25, rand_int_32\n"
			"flw.ps f0, 672(x25)\n"
			"flw.ps f3, 608(x25)\n"
			"fmax.pi f0, f0, f3\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f1, 576(x26)\n"
			"fround.ps f0, f1, rtz\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f0, 160(x13)\n"
			"flw.ps f18, 768(x13)\n"
			"fdiv.s f8, f0, f18, rup\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_13\n"
			"addi x30, x30, 1\n"
			"la x17, rand_ieee754_16\n"
			"flw.ps f18, 352(x17)\n"
			"fcvt.ps.f16 f4, f18\n" 
			"la x15, rand_int_32\n"
			"flw.ps f10, 864(x15)\n"
			"flw.ps f25, 896(x15)\n"
			"fxor.pi f3, f10, f25\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f4, 640(x5)\n"
			"flw.ps f18, 896(x5)\n"
			"fle.ps f25, f4, f18\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f30, 0(x26)\n"
			"flw.ps f1, 128(x26)\n"
			"flw.ps f28, 896(x26)\n"
			"fmadd.ps f4, f30, f1, f28, rup\n" 
			"mova.x.m x17\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f20, 64(x26)\n"
			"fcvt.pwu.ps f20, f20, rup\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f30, 224(x14)\n"
			"frsq.ps f25, f30\n" 
			"LBL_SEQID_1_M0_WRITE_ID_1_HID_13:\n"
			"fltm.pi m7, f29, f25\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_1_M0_WRITE_ID_2_HID_13:\n"
			"mov.m.x m5, x13, 0xa\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x25, rand_int_32\n"
			"flw.ps f21, 832(x25)\n"
			"fslli.pi f20, f21, 0x6\n" 
			"LBL_SEQID_1_FP_TRANS_ID_3_HID_13:\n"
			"la x17, rand_ieee754_32\n"
			"flw.ps f27, 960(x17)\n"
			"fexp.ps f3, f27\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f31, 320(x16)\n"
			"fcvt.wu.s x25, f31, rmm\n" 
			"la x14, rand_ieee754_16\n"
			"flw.ps f10, 960(x14)\n"
			"fcvt.ps.f16 f30, f10\n" 
			"la x16, rand_int_32\n"
			"flw.ps f20, 224(x16)\n"
			"fsrl.pi f13, f20, f20\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f27, 800(x25)\n"
			"flw.ps f21, 64(x25)\n"
			"fsub.s f27, f27, f21, rtz\n" 
			"la x14, rand_int_32\n"
			"flw.ps f29, 0(x14)\n"
			"flw.ps f4, 384(x14)\n"
			"for.pi f0, f29, f4\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_13\n"
			"addi x30, x30, 1\n"
			"la x5, rand_ieee754_16\n"
			"flw.ps f30, 256(x5)\n"
			"fcvt.ps.f16 f10, f30\n" 
			"la x25, rand_ieee754_32\n"
			"flw.ps f1, 896(x25)\n"
			"flw.ps f31, 352(x25)\n"
			"fle.ps f1, f1, f31\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f13, 480(x20)\n"
			"flw.ps f31, 864(x20)\n"
			"fmin.s f31, f13, f31\n" 
			"maskpopc x5, m0\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f31, 832(x20)\n"
			"flw.ps f8, 160(x20)\n"
			"flw.ps f4, 768(x20)\n"
			"fmadd.ps f21, f31, f8, f4, rdn\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f4, 224(x5)\n"
			"flw.ps f0, 320(x5)\n"
			"flw.ps f25, 320(x5)\n"
			"fcmov.ps f10, f4, f0, f25\n" 
			"maskpopc x15, m5\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f3, 832(x13)\n"
			"flw.ps f1, 864(x13)\n"
			"feq.s x12, f3, f1\n" 
			"la x16, rand_ieee754_32\n"
			"flw.ps f10, 128(x16)\n"
			"fclass.s x20, f10\n" 
			"la x14, rand_ieee754_16\n"
			"flw.ps f8, 448(x14)\n"
			"fcvt.ps.f16 f20, f8\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f31, 608(x20)\n"
			"fsin.ps f28, f31\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f25, 768(x17)\n"
			"fcvt.pw.ps f8, f25, rdn\n" 
			"mova.x.m x15\n" 
			"la x15, rand_ieee754_32\n"
			"flw.ps f27, 864(x15)\n"
			"flw.ps f1, 320(x15)\n"
			"fmin.s f1, f27, f1\n" 
			"la x26, rand_ieee754_32\n"
			"flw.ps f0, 800(x26)\n"
			"flw.ps f28, 384(x26)\n"
			"fcmovm.ps f25, f0, f28\n" 
			"la x12, rand_ieee754_32\n"
			"flw.ps f10, 640(x12)\n"
			"ffrc.ps f25, f10\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 3\n" 
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
			"csrwi mhpmevent3, 26\n"   
			"csrwi mhpmevent4, 17\n"   
			"csrwi mhpmevent5, 19\n"   
			"csrwi mhpmevent6, 15\n"   
			"csrwi mhpmevent7, 7\n"   
			"csrwi mhpmevent8, 4\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f12,  224 (x5)\n"
			"flw.ps f15,  96 (x5)\n"
			"flw.ps f23,  160 (x5)\n"
			"flw.ps f8,  768 (x5)\n"
			"flw.ps f19,  608 (x5)\n"
			"flw.ps f26,  64 (x5)\n"
			"flw.ps f4,  960 (x5)\n"
			"flw.ps f28,  992 (x5)\n"
			"flw.ps f6,  416 (x5)\n"
			"flw.ps f29,  704 (x5)\n"
			"flw.ps f17,  736 (x5)\n"
			"flw.ps f18,  32 (x5)\n"
			"flw.ps f22,  192 (x5)\n"
			"flw.ps f24,  256 (x5)\n"
			"flw.ps f16,  480 (x5)\n"
			"flw.ps f14,  448 (x5)\n"
			"flw.ps f21,  128 (x5)\n"
			"flw.ps f5,  864 (x5)\n"
			"flw.ps f27,  352 (x5)\n"
			"flw.ps f31,  544 (x5)\n"
			"flw.ps f3,  0 (x5)\n"
			"flw.ps f11,  832 (x5)\n"
			"flw.ps f2,  896 (x5)\n"
			"flw.ps f13,  928 (x5)\n"
			"flw.ps f1,  512 (x5)\n"
			"flw.ps f7,  384 (x5)\n"
			"flw.ps f20,  800 (x5)\n"
			"flw.ps f0,  672 (x5)\n"
			"flw.ps f25,  288 (x5)\n"
			"flw.ps f9,  640 (x5)\n"
			"flw.ps f30,  576 (x5)\n"
			"flw.ps f10,  320 (x5)\n"
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
			"li x5, 0x3\n" 
			"sw x5, (x31)\n"
			"li x29, 0x6\n"
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
			"la x19, rand_int_32\n"
			"flw.ps f11, 960(x19)\n"
			"flw.ps f28, 0(x19)\n"
			"fsrl.pi f25, f11, f28\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f2, 832(x19)\n"
			"flw.ps f27, 832(x19)\n"
			"flw.ps f9, 832(x19)\n"
			"fmadd.s f29, f2, f27, f9, rup\n" 
			"la x19, rand_ieee754_32\n"
			"flw.ps f3, 640(x19)\n"
			"flw.ps f20, 480(x19)\n"
			"fsgnjx.ps f2, f3, f20\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f2, 192(x28)\n"
			"flw.ps f25, 0(x28)\n"
			"fmax.ps f11, f2, f25\n" 
			"la x17, rand_int_32\n"
			"flw.ps f31, 832(x17)\n"
			"flw.ps f13, 576(x17)\n"
			"fand.pi f24, f31, f13\n" 
			"la x14, rand_int_32\n"
			"flw.ps f30, 448(x14)\n"
			"flw.ps f20, 992(x14)\n"
			"for.pi f26, f30, f20\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_14:\n"
			"feqm.ps m4, f27, f12\n" 
			"mov.m.x m0, x0, 0x0\n"
			"li x5, 0x656b02edcd\n"
			"fsw.ps f0, 0(x5)\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x17, rand_int_32\n"
			"flw.ps f25, 448(x17)\n"
			"flw.ps f31, 928(x17)\n"
			"fsrl.pi f25, f25, f31\n" 
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_14:\n"
			"fsetm.pi m4, f31\n" 
			"li x5, 0x80340128\n"
			"ld x5, 0(x5)\n"
			"la x23, rand_ieee754_32\n"
			"flw.ps f9, 768(x23)\n"
			"flw.ps f11, 256(x23)\n"
			"fcmovm.ps f27, f9, f11\n" 
			"maskpopc x9, m4\n" 
			"LBL_SEQID_0_FP_TRANS_ID_2_HID_14:\n"
			"la x16, rand_ieee754_32\n"
			"flw.ps f11, 192(x16)\n"
			"flog.ps f30, f11\n" 
			"la x14, rand_ieee754_32\n"
			"flw.ps f30, 192(x14)\n"
			"flw.ps f24, 384(x14)\n"
			"flt.s x9, f30, f24\n" 
			"LBL_SEQID_0_M0_WRITE_ID_3_HID_14:\n"
			"flem.ps m1, f11, f3\n" 
			"li x5, 0x80340110\n"
			"ld x5, 0(x5)\n"
			"LBL_SEQID_0_M0_WRITE_ID_4_HID_14:\n"
			"fltm.pi m1, f25, f13\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x23, rand_int_32\n"
			"flw.ps f0, 704(x23)\n"
			"flw.ps f9, 224(x23)\n"
			"fsll.pi f26, f0, f9\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_0_RDX_ID_0_SHIRE_0_HID_14:\n"
			"li x9, 0x240000000707000a\n"
			"csrw tensor_reduce, x9\n" 
			"csrwi tensor_wait, 9\n" 
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ID_104840_HID_14:\n"
			"la x31, self_check_32\n"
			"li x5, 0xe0000000000004\n"
			"add x5, x5, x31\n"
			"li x6, 0x1000000000000f\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x2 << TENSOR_FMA32_B_NUM_COLS) | (0x4 << TENSOR_FMA32_A_NUM_ROWS) | (0xf << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x1 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1b << TENSOR_FMA32_SCP_LOC_B) | (0x7 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x17 << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0x4 << TENSOR_QUANT_QUANT_ROW) | (0x4 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x2 << TENSOR_QUANT_TRANSF5) | (0x7 << TENSOR_QUANT_TRANSF4) | (0x7 << TENSOR_QUANT_TRANSF3) | (0x7 << TENSOR_QUANT_TRANSF2) | (0x6 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 10\n"
			"LBL_SEQID_0_TLD_XMA_STR_SEQ_ID_543995_HID_14:\n"
			"la x31, self_check_32\n"
			"li x5, 0x20000000000009\n"
			"add x5, x5, x31\n"
			"li x6, 0x1000000000000a\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x3 << TENSOR_FMA32_B_NUM_COLS) | (0x9 << TENSOR_FMA32_A_NUM_ROWS) | (0xa << TENSOR_FMA32_A_NUM_COLS) | (0x0 << TENSOR_FMA32_OFFSET) | (0x1 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x1b << TENSOR_FMA32_SCP_LOC_B) | (0x1 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"li x31, (0x1 << 6) | 0x1\n"
			"csrw tensor_load, x5\n"	
			"li x31, (0x1 << 6) | 0x0\n"
			"csrw tensor_load, x6\n"	
			"csrwi tensor_wait, 1\n"
			"csrw tensor_fma, x7\n"	
			"csrwi tensor_wait, 7\n"
			"fcvt.w.s x31, f0\n"
			"li x5, 0xb\n"
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
			"li x5, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x1 << TENSOR_FMA32_B_NUM_COLS) | (0xa << TENSOR_FMA32_A_NUM_ROWS) | (0x6 << TENSOR_FMA32_A_NUM_COLS) | (0x8 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x98 << TENSOR_FMA32_SCP_LOC_B) | (0x2d << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_14\n"
			"csrw tensor_error, x0\n"
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_14\n"
			"addi x30, x30, 1\n"
			"la x19, rand_int_32\n"
			"flw.ps f13, 64(x19)\n"
			"fcvt.ps.pwu f27, f13, rne\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x14, rand_int_32\n"
			"flw.ps f9, 992(x14)\n"
			"flw.ps f24, 384(x14)\n"
			"fltu.pi f0, f9, f24\n" 
			"la x22, rand_int_32\n"
			"flw.ps f0, 864(x22)\n"
			"flw.ps f28, 896(x22)\n"
			"fsub.pi f27, f0, f28\n" 
			"la x5, rand_int_32\n"
			"flw.ps f13, 544(x5)\n"
			"flw.ps f25, 960(x5)\n"
			"fmaxu.pi f27, f13, f25\n" 
			"la x9, rand_int_32\n"
			"flw.ps f24, 288(x9)\n"
			"flw.ps f29, 192(x9)\n"
			"fmax.pi f26, f24, f29\n" 
			"la x22, rand_int_32\n"
			"flw.ps f12, 928(x22)\n"
			"fandi.pi f24, f12, 0xa4\n" 
			"la x6, rand_int_32\n"
			"flw.ps f24, 256(x6)\n"
			"flw.ps f2, 960(x6)\n"
			"fmaxu.pi f24, f24, f2\n" 
			"la x17, rand_int_32\n"
			"flw.ps f13, 576(x17)\n"
			"fslli.pi f11, f13, 0x9\n" 
			"la x22, rand_int_32\n"
			"flw.ps f20, 832(x22)\n"
			"fpackreph.pi f24, f20\n" 
			"la x14, rand_int_32\n"
			"flw.ps f27, 800(x14)\n"
			"flw.ps f25, 768(x14)\n"
			"fmul.pi f25, f27, f25\n" 
			"la x6, rand_int_32\n"
			"flw.ps f0, 288(x6)\n"
			"flw.ps f28, 992(x6)\n"
			"fadd.pi f13, f0, f28\n" 
			"la x10, rand_ieee754_32\n"
			"flw.ps f2, 32(x10)\n"
			"fcvt.w.s x10, f2, rne\n" 
			"la x5, rand_int_32\n"
			"flw.ps f31, 608(x5)\n"
			"flw.ps f28, 384(x5)\n"
			"fadd.pi f2, f31, f28\n" 
			"LBL_SEQID_1_M0_WRITE_ID_5_HID_14:\n"
			"fltm.ps m3, f29, f9\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x5, rand_ieee754_32\n"
			"flw.ps f2, 480(x5)\n"
			"flw.ps f11, 896(x5)\n"
			"flt.s x19, f2, f11\n" 
			"LBL_SEQID_1_M0_WRITE_ID_6_HID_14:\n"
			"feqm.ps m2, f3, f3\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_1_TLD_XMA_STR_SEQ_ID_893713_HID_14:\n"
			"la x31, self_check_16\n"
			"li x5, 0xe0000000000006\n"
			"add x5, x5, x31\n"
			"li x6, 0x38000000000000c\n"
			"add x6, x6, x31\n"
			"li x7, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x1 << TENSOR_FMA16_B_NUM_COLS) | (0x6 << TENSOR_FMA16_A_NUM_ROWS) | (0xc << TENSOR_FMA16_A_NUM_COLS) | (0x0 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x1c << TENSOR_FMA16_SCP_LOC_B) | (0x7 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x9, ((0x8 << TENSOR_QUANT_START_REG) | (0x1 << TENSOR_QUANT_QUANT_COL) | (0x6 << TENSOR_QUANT_QUANT_ROW) | (0x19 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x0 << TENSOR_QUANT_TRANSF4) | (0x0 << TENSOR_QUANT_TRANSF3) | (0x0 << TENSOR_QUANT_TRANSF2) | (0x0 << TENSOR_QUANT_TRANSF1) | (0x6 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
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
			"bne x5, x31, LBL_FAIL_HID_14\n"
			"csrw tensor_quant, x9\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_14\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_1_RDX_ID_1_SHIRE_0_HID_14:\n"
			"li x9, 0x140000000330000b\n"
			"csrw tensor_reduce, x9\n" 
			"csrwi tensor_wait, 9\n" 
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ID_988705_HID_14:\n"
			"li x5, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x2 << TENSOR_FMA16_B_NUM_COLS) | (0xe << TENSOR_FMA16_A_NUM_ROWS) | (0xd << TENSOR_FMA16_A_NUM_COLS) | (0xb << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0xdc << TENSOR_FMA16_SCP_LOC_B) | (0xc6 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x0 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"li x6, ((0x15 << TENSOR_QUANT_START_REG) | (0x2 << TENSOR_QUANT_QUANT_COL) | (0xe << TENSOR_QUANT_QUANT_ROW) | (0x30 << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0x0 << TENSOR_QUANT_TRANSF6) | (0x0 << TENSOR_QUANT_TRANSF5) | (0x2 << TENSOR_QUANT_TRANSF4) | (0x6 << TENSOR_QUANT_TRANSF3) | (0x6 << TENSOR_QUANT_TRANSF2) | (0x7 << TENSOR_QUANT_TRANSF1) | (0x7 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 7\n"
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
			"LBL_SEQID_1_XMA_QNT_STR_SEQ_ITER_0_HID_14:\n"
			"li x6, ((0x1b << TENSOR_QUANT_START_REG) | (0x0 << TENSOR_QUANT_QUANT_COL) | (0x2 << TENSOR_QUANT_QUANT_ROW) | (0x2d << TENSOR_QUANT_QUANT_SCP_LOC) | (0x0 << TENSOR_QUANT_TRANSF9) | (0x0 << TENSOR_QUANT_TRANSF8) | (0x0 << TENSOR_QUANT_TRANSF7) | (0xa << TENSOR_QUANT_TRANSF6) | (0x3 << TENSOR_QUANT_TRANSF5) | (0x9 << TENSOR_QUANT_TRANSF4) | (0x5 << TENSOR_QUANT_TRANSF3) | (0x9 << TENSOR_QUANT_TRANSF2) | (0x3 << TENSOR_QUANT_TRANSF1) | (0x9 << TENSOR_QUANT_TRANSF0) | 0 )\n" 
			"csrw tensor_quant, x6\n"	
			"csrwi tensor_wait, 10\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_14\n"
			"csrw tensor_error, x0\n"
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_14\n"
			"addi x30, x30, 1\n"
			"la x19, rand_ieee754_32\n"
			"flw.ps f25, 448(x19)\n"
			"flw.ps f3, 544(x19)\n"
			"fle.s x17, f25, f3\n" 
			"LBL_SEQID_2_M0_WRITE_ID_7_HID_14:\n"
			"fsetm.pi m2, f27\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"li x16, 0x4f1cdc10e324216e\n"
			"fcvt.s.wu f29, x16, rtz\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f30, 704(x22)\n"
			"fcvt.pw.ps f26, f30, rne\n" 
			"la x28, rand_ieee754_32\n"
			"flw.ps f29, 192(x28)\n"
			"fsgnj.s f0, f29, f29\n" 
			"mova.x.m x16\n" 
			"maskpopcz x6, m3\n" 
			"la x19, rand_int_32\n"
			"flw.ps f11, 576(x19)\n"
			"flw.ps f27, 96(x19)\n"
			"fsra.pi f28, f11, f27\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f31, 704(x22)\n"
			"fclass.s x28, f31\n" 
			"csrw tensor_conv_size, zero\n"
			"csrw tensor_conv_ctrl, zero\n"
			"csrw tensor_mask, zero\n"
			"la x16, rand_ieee754_32\n"
			"flw.ps f20, 448(x16)\n"
			"frsq.ps f29, f20\n" 
			"la x28, rand_int_32\n"
			"flw.ps f24, 800(x28)\n"
			"flw.ps f28, 736(x28)\n"
			"feq.pi f26, f24, f28\n" 
			"la x17, rand_int_32\n"
			"flw.ps f28, 128(x17)\n"
			"flw.ps f24, 480(x17)\n"
			"fxor.pi f28, f28, f24\n" 
			"la x14, rand_int_32\n"
			"flw.ps f2, 192(x14)\n"
			"fcvt.ps.pw f12, f2, rne\n" 
			"LBL_SEQID_2_M0_WRITE_ID_8_HID_14:\n"
			"masknot m2, m1\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x16, rand_int_32\n"
			"flw.ps f24, 480(x16)\n"
			"fnot.pi f2, f24\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f31, 704(x22)\n"
			"flw.ps f25, 736(x22)\n"
			"fmul.ps f26, f31, f25, rup\n" 
			"csrw tensor_error, zero\n" 
			"LBL_SEQID_2_XMA_QNT_STR_SEQ_ID_313722_HID_14:\n"
			"li x5, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x3 << TENSOR_FMA16_B_NUM_COLS) | (0x5 << TENSOR_FMA16_A_NUM_ROWS) | (0xe << TENSOR_FMA16_A_NUM_COLS) | (0x6 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0xbf << TENSOR_FMA16_SCP_LOC_B) | (0x8e << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x0 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"LBL_SEQID_2_XMA_QNT_STR_SEQ_ID_409917_HID_14:\n"
			"li x5, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x3 << TENSOR_FMA32_B_NUM_COLS) | (0x2 << TENSOR_FMA32_A_NUM_ROWS) | (0xc << TENSOR_FMA32_A_NUM_COLS) | (0x3 << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0xd << TENSOR_FMA32_SCP_LOC_B) | (0x27 << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_14\n"
			"csrw tensor_error, x0\n"
			"LBL_SEQID_2_XMA_QNT_STR_SEQ_ITER_0_HID_14:\n"
			"li x5, ((0x0 << TENSOR_FMA32_USE_TMASK) | (0x0 << TENSOR_FMA32_B_NUM_COLS) | (0xa << TENSOR_FMA32_A_NUM_ROWS) | (0x2 << TENSOR_FMA32_A_NUM_COLS) | (0xa << TENSOR_FMA32_OFFSET) | (0x0 << TENSOR_FMA32_TEN_B_IN_MEM) | (0x64 << TENSOR_FMA32_SCP_LOC_B) | (0x8e << TENSOR_FMA32_SCP_LOC_A) | (0x0 << TENSOR_FMA32_OPC_000) | (0x1 << TENSOR_FMA32_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"LBL_SEQID_2_XMA_QNT_STR_SEQ_ITER_1_HID_14:\n"
			"li x5, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x3 << TENSOR_FMA16_B_NUM_COLS) | (0xe << TENSOR_FMA16_A_NUM_ROWS) | (0xa << TENSOR_FMA16_A_NUM_COLS) | (0x6 << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0xbb << TENSOR_FMA16_SCP_LOC_B) | (0x51 << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"LBL_SEQID_2_XMA_QNT_STR_SEQ_ITER_2_HID_14:\n"
			"li x5, ((0x0 << TENSOR_FMA16_USE_TMASK) | (0x2 << TENSOR_FMA16_B_NUM_COLS) | (0xe << TENSOR_FMA16_A_NUM_ROWS) | (0xb << TENSOR_FMA16_A_NUM_COLS) | (0xb << TENSOR_FMA16_OFFSET) | (0x0 << TENSOR_FMA16_TEN_B_IN_MEM) | (0x2f << TENSOR_FMA16_SCP_LOC_B) | (0x2a << TENSOR_FMA16_SCP_LOC_A) | (0x1 << TENSOR_FMA16_OPC_001) | (0x1 << TENSOR_FMA16_FIRST_PASS) | 0 )\n" 
			"csrw tensor_fma, x5\n"	
			"csrwi tensor_wait, 7\n"
			"csrr x5, tensor_error\n"
			"andi x5, x5, 0x88\n"
			"bne x5, x0, LBL_FAIL_HID_14\n"
			"csrw tensor_error, x0\n"
			"li x5, 3\n" 
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
			"csrwi mhpmevent3, 11\n"   
			"csrwi mhpmevent4, 6\n"   
			"csrwi mhpmevent5, 13\n"   
			"csrwi mhpmevent6, 23\n"   
			"csrwi mhpmevent7, 2\n"   
			"csrwi mhpmevent8, 1\n"   
			"li x31, 0x40\n"                 
			"mov.m.x m0, x0, 0xff\n"
			"la x5, rand_ieee754_32\n"
			"add x6, x0, x29\n"
			"slli x6, x6, 5\n"
			"add x5, x6, x5\n"
			"flw.ps f14,  832 (x5)\n"
			"flw.ps f31,  864 (x5)\n"
			"flw.ps f11,  256 (x5)\n"
			"flw.ps f5,  64 (x5)\n"
			"flw.ps f27,  608 (x5)\n"
			"flw.ps f8,  672 (x5)\n"
			"flw.ps f9,  928 (x5)\n"
			"flw.ps f7,  128 (x5)\n"
			"flw.ps f29,  32 (x5)\n"
			"flw.ps f3,  480 (x5)\n"
			"flw.ps f24,  0 (x5)\n"
			"flw.ps f17,  800 (x5)\n"
			"flw.ps f23,  96 (x5)\n"
			"flw.ps f21,  960 (x5)\n"
			"flw.ps f4,  448 (x5)\n"
			"flw.ps f10,  416 (x5)\n"
			"flw.ps f25,  352 (x5)\n"
			"flw.ps f20,  512 (x5)\n"
			"flw.ps f26,  576 (x5)\n"
			"flw.ps f13,  320 (x5)\n"
			"flw.ps f0,  736 (x5)\n"
			"flw.ps f15,  992 (x5)\n"
			"flw.ps f6,  640 (x5)\n"
			"flw.ps f19,  384 (x5)\n"
			"flw.ps f30,  288 (x5)\n"
			"flw.ps f28,  704 (x5)\n"
			"flw.ps f18,  768 (x5)\n"
			"flw.ps f12,  160 (x5)\n"
			"flw.ps f16,  224 (x5)\n"
			"flw.ps f1,  544 (x5)\n"
			"flw.ps f22,  896 (x5)\n"
			"flw.ps f2,  192 (x5)\n"
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
			"li x5, 0x3\n" 
			"sw x5, (x31)\n"
			"li x29, 0x6\n"
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
			"la x28, rand_ieee754_32\n"
			"flw.ps f10, 320(x28)\n"
			"fround.ps f4, f10, rdn\n" 
			"la x13, rand_int_32\n"
			"flw.ps f15, 928(x13)\n"
			"flw.ps f11, 32(x13)\n"
			"fmulh.pi f1, f15, f11\n" 
			"la x27, rand_ieee754_16\n"
			"flw.ps f11, 960(x27)\n"
			"fcvt.ps.f16 f18, f11\n" 
			"la x17, rand_int_32\n"
			"flw.ps f3, 992(x17)\n"
			"flw.ps f29, 736(x17)\n"
			"flt.pi f3, f3, f29\n" 
			"la x7, rand_ieee754_32\n"
			"flw.ps f13, 416(x7)\n"
			"flw.ps f2, 672(x7)\n"
			"fmul.ps f14, f13, f2, rup\n" 
			"LBL_SEQID_0_M0_WRITE_ID_0_HID_15:\n"
			"mova.m.x x20\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"LBL_SEQID_0_M0_WRITE_ID_1_HID_15:\n"
			"maskor m0, m2, m4\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0xFF\n"
			"la x12, rand_int_32\n"
			"flw.ps f1, 576(x12)\n"
			"flw.ps f29, 96(x12)\n"
			"fmul.pi f13, f1, f29\n" 
			"la x13, rand_int_32\n"
			"flw.ps f4, 416(x13)\n"
			"flw.ps f30, 736(x13)\n"
			"fmulhu.pi f13, f4, f30\n" 
			"la x5, rand_ieee754_32\n"
			"flw.ps f29, 928(x5)\n"
			"fsqrt.ps f28, f29\n" 
			"LBL_SEQID_0_M0_WRITE_ID_2_HID_15:\n"
			"maskand m4, m4, m2\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x12, rand_int_32\n"
			"flw.ps f4, 320(x12)\n"
			"flw.ps f20, 256(x12)\n"
			"fmul.pi f1, f4, f20\n" 
			"la x7, rand_int_32\n"
			"flw.ps f2, 160(x7)\n"
			"fsatu8.pi f4, f2\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f18, 992(x17)\n"
			"fcvt.pw.ps f13, f18, rtz\n" 
			"la x18, rand_ieee754_32\n"
			"flw.ps f1, 576(x18)\n"
			"flw.ps f2, 416(x18)\n"
			"fsgnjn.s f3, f1, f2\n" 
			"LBL_SEQID_0_M0_WRITE_ID_3_HID_15:\n"
			"maskand m6, m6, m4\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"csrw tensor_error, zero\n" 
			"li x5, 1\n" 
			"bne x30, x5, LBL_FAIL_HID_15\n"
			"addi x30, x30, 1\n"
			"la x6, rand_ieee754_32\n"
			"flw.ps f28, 448(x6)\n"
			"fcvt.pwu.ps f1, f28, rup\n" 
			"la x22, rand_int_32\n"
			"flw.ps f13, 864(x22)\n"
			"flw.ps f14, 800(x22)\n"
			"fmaxu.pi f15, f13, f14\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f13, 96(x22)\n"
			"flw.ps f16, 192(x22)\n"
			"flw.ps f4, 256(x22)\n"
			"fcmov.ps f20, f13, f16, f4\n" 
			"la x5, rand_int_32\n"
			"flw.ps f16, 480(x5)\n"
			"flw.ps f3, 288(x5)\n"
			"fle.pi f27, f16, f3\n" 
			"la x13, rand_ieee754_32\n"
			"flw.ps f28, 640(x13)\n"
			"flw.ps f20, 32(x13)\n"
			"fmax.s f13, f28, f20\n" 
			"la x18, rand_int_32\n"
			"flw.ps f3, 672(x18)\n"
			"flw.ps f15, 32(x18)\n"
			"fminu.pi f16, f3, f15\n" 
			"LBL_SEQID_1_M0_WRITE_ID_4_HID_15:\n"
			"maskand m6, m0, m0\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"maskpopc x12, m0\n" 
			"mov.m.x m0, x0, 0xff\n"
			"la x20, rand_ieee754_32\n"
			"flw.ps f29, 640(x20)\n"
			"fcvt.pw.ps f28, f29, rdn\n" 
			"la x18, rand_int_32\n"
			"flw.ps f15, 320(x18)\n"
			"fandi.pi f2, f15, 0x125\n" 
			"la x17, rand_ieee754_32\n"
			"flw.ps f16, 928(x17)\n"
			"fadd.ps f13, f16, f16, rtz\n" 
			"LBL_SEQID_1_M0_WRITE_ID_5_HID_15:\n"
			"fsetm.pi m4, f13\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"li x13, 0x573626e4bcfab55f\n"
			"fcvt.s.wu f28, x13, rtz\n" 
			"la x6, rand_int_32\n"
			"flw.ps f14, 192(x6)\n"
			"fsat8.pi f28, f14\n" 
			"la x6, rand_ieee754_32\n"
			"flw.ps f11, 896(x6)\n"
			"flw.ps f27, 96(x6)\n"
			"feq.ps f30, f11, f27\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f16, 864(x22)\n"
			"flw.ps f15, 928(x22)\n"
			"fsgnj.s f11, f16, f15\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 2\n" 
			"bne x30, x5, LBL_FAIL_HID_15\n"
			"addi x30, x30, 1\n"
			"LBL_SEQID_2_FP_TRANS_ID_6_HID_15:\n"
			"la x17, rand_ieee754_32\n"
			"flw.ps f2, 960(x17)\n"
			"frcp.ps f14, f2\n" 
			"la x5, rand_int_32\n"
			"flw.ps f1, 704(x5)\n"
			"flw.ps f14, 768(x5)\n"
			"fmul.pi f27, f1, f14\n" 
			"LBL_SEQID_2_M0_WRITE_ID_7_HID_15:\n"
			"fsetm.pi m4, f27\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x28, rand_int_32\n"
			"flw.ps f13, 960(x28)\n"
			"flw.ps f4, 64(x28)\n"
			"fsll.pi f3, f13, f4\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f29, 320(x27)\n"
			"fcvt.pwu.ps f3, f29, rne\n" 
			"la x13, rand_int_32\n"
			"flw.ps f3, 480(x13)\n"
			"flw.ps f13, 320(x13)\n"
			"feq.pi f3, f3, f13\n" 
			"la x22, rand_ieee754_32\n"
			"flw.ps f28, 160(x22)\n"
			"fcvt.w.s x20, f28, rne\n" 
			"la x20, rand_ieee754_32\n"
			"flw.ps f15, 288(x20)\n"
			"flw.ps f4, 320(x20)\n"
			"fcmovm.ps f13, f15, f4\n" 
			"la x17, rand_int_32\n"
			"flw.ps f27, 128(x17)\n"
			"flw.ps f18, 448(x17)\n"
			"fltu.pi f18, f27, f18\n" 
			"la x17, rand_int_32\n"
			"flw.ps f13, 576(x17)\n"
			"flw.ps f28, 864(x17)\n"
			"fmul.pi f16, f13, f28\n" 
			"LBL_SEQID_2_M0_WRITE_ID_8_HID_15:\n"
			"maskxor m2, m0, m0\n" 
			"mov.m.x m0, x0, 0xff\n"
			"mov.m.x m0, x0, 0xFF\n"
			"li x7, 0xc8d87f59384d6b5f\n"
			"fcvt.s.w f10, x7, rtz\n" 
			"LBL_SEQID_2_M0_WRITE_ID_9_HID_15:\n"
			"maskand m2, m4, m6\n" 
			"mov.m.x m0, x0, 0xFF\n"
			"la x13, rand_ieee754_32\n"
			"flw.ps f14, 192(x13)\n"
			"flw.ps f2, 352(x13)\n"
			"flt.ps f28, f14, f2\n" 
			"la x5, rand_int_32\n"
			"flw.ps f27, 224(x5)\n"
			"flw.ps f11, 192(x5)\n"
			"fmax.pi f10, f27, f11\n" 
			"la x27, rand_ieee754_32\n"
			"flw.ps f15, 768(x27)\n"
			"fsin.ps f16, f15\n" 
			"csrw tensor_error, zero\n" 
			"li x5, 3\n" 
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
		0x4f8637d9, 
		0x0ba59f9d, 
		0xa132c8d7, 
		0x50114d05, 
		0xa5c760a0, 
		0x8c5d1e0b, 
		0x25c42f39, 
		0x18c6a480, 
		0xaee7f2ce, 
		0xfa5d58c7, 
		0x68f3ce03, 
		0xddf746fc, 
		0x40333f9e, 
		0x32203826, 
		0x2f82a70b, 
		0x33550e55, 
		0x81529136, 
		0xc94f1286, 
		0x0878f3ce, 
		0x5b277a44, 
		0xc1f5043c, 
		0x6c37ac41, 
		0xbc4331f7, 
		0x700c17d8, 
		0xb540dad4, 
		0x9ab6b010, 
		0xe8d9f49e, 
		0x12171e03, 
		0x07d0c814, 
		0xacf6981d, 
		0x47d097e0, 
		0x0b634478, 
		0x39d971ea, 
		0xb7e57461, 
		0x15c14808, 
		0xc33d3dcd, 
		0xc3a4d1de, 
		0xe06aa6c6, 
		0x1a8fa04d, 
		0xf29c74f7, 
		0x95245781, 
		0x63fea1d4, 
		0xa33be61f, 
		0x38ad0ce9, 
		0x99b611b1, 
		0xea0ffbed, 
		0x9030427d, 
		0x4d4d1cc3, 
		0x2d284b66, 
		0xadc1ba20, 
		0x5e5f1277, 
		0x3fc8c828, 
		0x1fa4c067, 
		0xe25f7bad, 
		0x122d86b0, 
		0xfd83510d, 
		0x6b391ba2, 
		0xfa2ff692, 
		0x15f1aed3, 
		0x33e471ba, 
		0x8591513f, 
		0x715a8eba, 
		0x2ae16dee, 
		0x413cced5, 
		0x5c3f2fc7, 
		0xf5208038, 
		0x7e5c48cc, 
		0x0afa0a8b, 
		0xd246166e, 
		0xf3e0752f, 
		0xfa7594bc, 
		0x21ab7757, 
		0x488018dd, 
		0xe3b2f3f2, 
		0x5a61c564, 
		0x425d9069, 
		0x21b7b6fd, 
		0xc74ac3d9, 
		0x3707f671, 
		0x34664611, 
		0xa793f6a3, 
		0x124e3bb9, 
		0x0c9d4c50, 
		0x3aa254fc, 
		0x11bf5f2f, 
		0x60f4a8ed, 
		0x4009499b, 
		0x13708a7e, 
		0x6d1c3da8, 
		0xb37f80b8, 
		0x45813e35, 
		0xbbaf8351, 
		0x2283ec5a, 
		0x37d3133f, 
		0xa4e6edec, 
		0xa76d834b, 
		0xd40f3709, 
		0x4d3af4a5, 
		0xa3c2f7c6, 
		0x30767ed6, 
		0xfa876ca2, 
		0xa86bbc90, 
		0x89b8e515, 
		0x1b503acf, 
		0x670f47d9, 
		0xba80cd63, 
		0x0d1d8bb2, 
		0xc976f522, 
		0xd9a97415, 
		0x4ceddc11, 
		0x6efd02df, 
		0x5204b820, 
		0x0dd31009, 
		0x74458f38, 
		0x666c01c0, 
		0xca9aa989, 
		0x994cd542, 
		0x51b865a2, 
		0x6ff8c6e8, 
		0x499b99c0, 
		0x1178aa44, 
		0x42b39f59, 
		0xdd1bd6c8, 
		0xe1b6e009, 
		0xdb56dc33, 
		0x482b68a8, 
		0x0a9a5bf0, 
		0x70ddb1af, 
		0xfd6de28f, 
		0x4c8edbb0, 
		0xec47d62f, 
		0x4f16b98b, 
		0xee3bc58c, 
		0x58c87eda, 
		0x6b3b87a1, 
		0x4dd5ba0f, 
		0x77cb0083, 
		0x3d3f0997, 
		0x1e44b5fd, 
		0x1f86d147, 
		0xf4cf74f3, 
		0x1202dffe, 
		0xb8fd38ce, 
		0xa7d68386, 
		0x677b3a73, 
		0x02acfc59, 
		0x4396834d, 
		0x2b3560db, 
		0xa98acb18, 
		0x9ce3c042, 
		0xc239000d, 
		0x51f0806a, 
		0xe1895594, 
		0x99d3825e, 
		0x15d8334e, 
		0x97bdc78f, 
		0x7ae53b07, 
		0x01e77ae4, 
		0x694bd3b6, 
		0x643d4a08, 
		0x1ff17b36, 
		0x8af609ea, 
		0x72f97fce, 
		0x99a82680, 
		0x67732035, 
		0x356caa80, 
		0x06e0f37d, 
		0x625ded64, 
		0x93ca04b6, 
		0xb8c1505c, 
		0xcc012410, 
		0x2cdbde8e, 
		0x67a99a44, 
		0x551c60cd, 
		0x17ed6ef2, 
		0x3095869e, 
		0x1642d173, 
		0x9eeacb94, 
		0xdda6031f, 
		0x31a92983, 
		0x1c5f6015, 
		0x1c80958d, 
		0x566608b7, 
		0x0b07b0a5, 
		0xa0af522b, 
		0x146440bf, 
		0x147cdbdb, 
		0xd7075c6c, 
		0x18f4223e, 
		0xa78ae283, 
		0x6efad3d2, 
		0x9f80e315, 
		0x1ed23909, 
		0xf2ee83de, 
		0xa7778d96, 
		0x9dfba7c7, 
		0x713af10c, 
		0xa95f638f, 
		0xb9ae926e, 
		0x2353e1a0, 
		0xc43b014c, 
		0x8ac30bba, 
		0xa62e7e91, 
		0x90fc8531, 
		0xe3c85724, 
		0xc13dd7de, 
		0x4bc0b4d0, 
		0xc94f7f27, 
		0x9cf08510, 
		0xc8a2e9c2, 
		0xaadfbeff, 
		0x9d919ad0, 
		0x0ef3aee2, 
		0x40f13651, 
		0x74230f10, 
		0xda6d0b08, 
		0x39ee8236, 
		0x60f23450, 
		0xa9b94b74, 
		0xcfea12c6, 
		0x124184e8, 
		0x955843c6, 
		0x32674f6f, 
		0x11411a68, 
		0x17bc2fde, 
		0x8e68ad28, 
		0x0ac2f92e, 
		0x8a0285b8, 
		0x6958625a, 
		0xa8afd047, 
		0x301e7169, 
		0xec2fe7e0, 
		0x6eb9c7eb, 
		0xe4688bcf, 
		0xc59ed78e, 
		0x24f221b5, 
		0x6b5ecbf7, 
		0xdfcf2c0d, 
		0xd5a2c249, 
		0x5e807259, 
		0xe26c1322, 
		0x5786a4af, 
		0xa0ed707f, 
		0x618716ae, 
		0xa0171ff9, 
		0xc88d62db, 
		0x5a0eda91, 
		0xdb063675, 
		0x2f99d57b, 
		0x993fcb50, 
		0x9b38062c, 
		0xeeb3e93d, 
		0x4876d84f, 
		0x9f90af0a, 
		0xa2d583ec, 
		0xc0298ea7  
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
		0xd1a0, 
		0xd3e0, 
		0x7c3d, 
		0x4400, 
		0x5220, 
		0xfc00, 
		0xbc00, 
		0xd260, 
		0x7fa1, 
		0x4400, 
		0x5120, 
		0xcf40, 
		0xcc80, 
		0xd040, 
		0xd380, 
		0x5320, 
		0x8000, 
		0x53e0, 
		0x4f00, 
		0x0000, 
		0xfd58, 
		0x53c0, 
		0x1929, 
		0x4e80, 
		0x65e3, 
		0xcf00, 
		0x3d32, 
		0x4d80, 
		0xd1a0, 
		0x5140, 
		0xc400, 
		0x4800, 
		0x5260, 
		0xd100, 
		0xd300, 
		0x0000, 
		0x5db2, 
		0xd320, 
		0xd100, 
		0x5240, 
		0xcfc0, 
		0x192c, 
		0xfc00, 
		0xf7aa, 
		0x8000, 
		0xd220, 
		0xd3a0, 
		0x55e1, 
		0xa323, 
		0x53a0, 
		0x8000, 
		0x5280, 
		0xd140, 
		0xd020, 
		0xcf00, 
		0xd1a0, 
		0xf6d6, 
		0x5260, 
		0x9b87, 
		0xdffd, 
		0x5200, 
		0xe683, 
		0xd360, 
		0x54d3, 
		0xcd40, 
		0xcf56, 
		0x53a0, 
		0xcb80, 
		0x17e5, 
		0x53a0, 
		0xd2d2, 
		0xc200, 
		0x7c0a, 
		0x8000, 
		0xa1bc, 
		0xefb3, 
		0xcc80, 
		0x527c, 
		0x5020, 
		0x1d9a, 
		0x027e, 
		0x4a00, 
		0x4dc0, 
		0x8000, 
		0x5240, 
		0x52a0, 
		0xcf74, 
		0xd2e0, 
		0xd060, 
		0x4a80, 
		0xfd09, 
		0x7c00, 
		0xc980, 
		0xcd40, 
		0xd1e0, 
		0x4ec0, 
		0x6879, 
		0x8000, 
		0x5000, 
		0x430b, 
		0x8000, 
		0xd140, 
		0x5020, 
		0x82bc, 
		0xcec0, 
		0x7c00, 
		0xd040, 
		0x52a0, 
		0xf4c8, 
		0xcd00, 
		0xd1e0, 
		0xd180, 
		0x4b00, 
		0x8000, 
		0x5180, 
		0xfcff, 
		0xda67, 
		0x4f80, 
		0xc880, 
		0x5180, 
		0x485e, 
		0x579d, 
		0xc8e6, 
		0xc000, 
		0xce80, 
		0xd2bb, 
		0xfc00, 
		0x5320, 
		0xd000, 
		0xd200, 
		0x8912, 
		0x1f8e, 
		0x0b41, 
		0xc400, 
		0x8000, 
		0x4e00, 
		0x4d80, 
		0xcf40, 
		0xcd00, 
		0x1c45, 
		0x4880, 
		0x7cad, 
		0x4200, 
		0x8000, 
		0xd2a0, 
		0x5040, 
		0x5020, 
		0x4f40, 
		0xf153, 
		0xd040, 
		0xd3a0, 
		0x4f80, 
		0xd080, 
		0xc500, 
		0xd1a0, 
		0x4e00, 
		0x4800, 
		0xd020, 
		0x4a80, 
		0x4cc0, 
		0xd0c0, 
		0xd9b1, 
		0xc980, 
		0x4e40, 
		0x87dd, 
		0xebd3, 
		0xeb22, 
		0x4e00, 
		0x4cc0, 
		0x5280, 
		0xd3a0, 
		0x7c00, 
		0xc700, 
		0xd1e0, 
		0xcf00, 
		0xd360, 
		0x52a0, 
		0x960f, 
		0x8000, 
		0x4880, 
		0xc000, 
		0xfc58, 
		0x1339, 
		0x5b0e, 
		0xc900, 
		0xd100, 
		0xd1c0, 
		0x80b6, 
		0xca00, 
		0x53a0, 
		0xc800, 
		0xd120, 
		0x4a00, 
		0xcf80, 
		0x4200, 
		0x4b80, 
		0x8000, 
		0x5360, 
		0xd100, 
		0x9ad6, 
		0x72fe, 
		0xd160, 
		0x0858, 
		0x4b00, 
		0x8000, 
		0x4f80, 
		0xd200, 
		0x5220, 
		0x7c00, 
		0xd080, 
		0xcd9a, 
		0x4b00, 
		0xe0ce, 
		0xc500, 
		0xd100, 
		0xfc00, 
		0x0000, 
		0x4c80, 
		0x5340, 
		0xd1a0, 
		0x4e40, 
		0x4e00, 
		0x4a80, 
		0x15c3, 
		0x4d5d, 
		0x0000, 
		0x5100, 
		0x7d65, 
		0x1848, 
		0x176c, 
		0xcf40, 
		0xd380, 
		0x4fc0, 
		0xce40, 
		0xd060, 
		0x4a00, 
		0xead6, 
		0xd280, 
		0x4400, 
		0xdb30, 
		0xd2a0, 
		0x3914, 
		0xce80, 
		0xd3a0, 
		0xaa57, 
		0xd020, 
		0x52c0, 
		0x4000, 
		0x5040, 
		0x5080, 
		0xc400, 
		0x52a0, 
		0xfaa3, 
		0xc400, 
		0xeecf, 
		0x7c00, 
		0xbc00, 
		0x5220, 
		0xc500, 
		0xfc00, 
		0x6d51, 
		0x51c0, 
		0xcc40, 
		0xd120, 
		0xce80, 
		0x3cec, 
		0xd320, 
		0xa772, 
		0x4500, 
		0x9e99, 
		0xd0e0, 
		0xcd62, 
		0x4980, 
		0xb5e5, 
		0x8000, 
		0x3aad, 
		0x4e40, 
		0x677f, 
		0x6b16, 
		0x1638, 
		0x4980, 
		0x5120, 
		0x7c00, 
		0x7c3c, 
		0x7c00, 
		0x832c, 
		0x4e40, 
		0xcb80, 
		0x3a11, 
		0x52a0, 
		0x1061, 
		0x5dac, 
		0x8b33, 
		0x11da, 
		0x5b79, 
		0x8000, 
		0xb470, 
		0xd000, 
		0x4e00, 
		0xce80, 
		0xc000, 
		0x5300, 
		0x4700, 
		0x4f80, 
		0x8000, 
		0xcf40, 
		0x0000, 
		0x0884, 
		0xd3a0, 
		0xce80, 
		0xcb80, 
		0x4fc0, 
		0x0000, 
		0x1421, 
		0xe715, 
		0x0000, 
		0x611b, 
		0xfc00, 
		0xcefc, 
		0x6309, 
		0xd040, 
		0xc365, 
		0xd180, 
		0xd360, 
		0xa486, 
		0xba30, 
		0x111e, 
		0xd951, 
		0x4d40, 
		0xcd80, 
		0x406a, 
		0x5e6d, 
		0x4f00, 
		0x9220, 
		0xacb4, 
		0xbc00, 
		0xcf00, 
		0xc400, 
		0xd2c0, 
		0xce80, 
		0x4dc0, 
		0xf1f1, 
		0x5180, 
		0xb266, 
		0x5340, 
		0x5060, 
		0x5100, 
		0xe631, 
		0x5360, 
		0x53c0, 
		0xd140, 
		0x4700, 
		0xd280, 
		0xcfbd, 
		0xd300, 
		0x59c8, 
		0xcec0, 
		0x8000, 
		0x0000, 
		0xccc0, 
		0xfc00, 
		0x51e0, 
		0x5000, 
		0x4e00, 
		0x8000, 
		0xd120, 
		0x9b97, 
		0x7c9e, 
		0xd280, 
		0xc900, 
		0xc800, 
		0x50e0, 
		0xd2c0, 
		0x3fa3, 
		0x4f00, 
		0x5080, 
		0x52e0, 
		0x8958, 
		0x5300, 
		0xcc40, 
		0x14d4, 
		0xc800, 
		0x4f80, 
		0x5380, 
		0x53c0, 
		0xfc2e, 
		0xd6d1, 
		0x3512, 
		0x4f40, 
		0xd3a0, 
		0x66f6, 
		0x5100, 
		0x643b, 
		0xd340, 
		0xd220, 
		0x4e00, 
		0xd020, 
		0x44b0, 
		0xabd6, 
		0x5100, 
		0xcfc0, 
		0x9573, 
		0xc30c, 
		0xc600, 
		0x1e8f, 
		0xce80, 
		0x8000, 
		0x84c1, 
		0x5160, 
		0xd120, 
		0x5100, 
		0xd060, 
		0xd160, 
		0xd200, 
		0xd300, 
		0xc571, 
		0x52a0, 
		0xd260, 
		0xd040, 
		0xcc00, 
		0xfabc, 
		0x8000, 
		0x5220, 
		0x7894, 
		0xd280, 
		0x53e0, 
		0xcec0, 
		0xd1c0, 
		0xc880, 
		0x8adf, 
		0xca00, 
		0x8541, 
		0x0000, 
		0xc880, 
		0x83af, 
		0xd360, 
		0x4200, 
		0xbc00, 
		0xe129, 
		0x5000, 
		0xd100, 
		0xd140, 
		0xdf5b, 
		0x4700, 
		0x8048, 
		0xd000, 
		0xd0e0, 
		0xce00, 
		0xead5, 
		0x7c00, 
		0xc400, 
		0x4d00, 
		0xcf40, 
		0x57a9, 
		0x96d5, 
		0x5000, 
		0x5000, 
		0xfc00, 
		0xcb80, 
		0xc400, 
		0x6cf1, 
		0x8000, 
		0xc1ad, 
		0xd040, 
		0x5220, 
		0x253d, 
		0x50a0, 
		0x9312, 
		0x5280, 
		0x5160, 
		0x7090, 
		0x9e32, 
		0xca80, 
		0x50c0, 
		0xcd40, 
		0x584d, 
		0x5120, 
		0xdb92, 
		0x5240, 
		0x52a0, 
		0x4880, 
		0x56dd, 
		0x3c00, 
		0xb87b, 
		0x4200, 
		0xfc00, 
		0xd380, 
		0xd080, 
		0x3917, 
		0x4e40, 
		0x4a80, 
		0xfdb4, 
		0xd97d, 
		0xcc00, 
		0x2312, 
		0xac52, 
		0xd260, 
		0xce0b, 
		0xce00, 
		0x4c00, 
		0xc200, 
		0x8000, 
		0x5080, 
		0xd080, 
		0x4d40, 
		0xcd7b, 
		0x33d2, 
		0xfc4f, 
		0xb57f, 
		0xa3e9, 
		0x64bd, 
		0x3c00, 
		0x4cc0, 
		0xd200, 
		0x53e0, 
		0x4400, 
		0x1f74, 
		0x3c00, 
		0xc000, 
		0xd3e0, 
		0xd000, 
		0x5240, 
		0xc200, 
		0x7e25, 
		0xc600, 
		0x0e0e, 
		0x0000, 
		0xd100, 
		0xfc00, 
		0xcc00, 
		0xd200, 
		0x4600, 
		0xd6c7, 
		0x5280, 
		0x46bf, 
		0xcf0b, 
		0x3c00, 
		0x038e, 
		0x4c40, 
		0x2cef, 
		0x4b00, 
		0x0000, 
		0x4d80, 
		0x5080, 
		0x4c80, 
		0x4400, 
		0x7c40, 
		0x515a, 
		0x4700, 
		0xd3e0, 
		0xcf80, 
		0xc880, 
		0xcd00, 
		0x4e80, 
		0xfc00, 
		0xba53, 
		0x8d2e, 
		0xd060, 
		0xb6b1, 
		0x0000, 
		0xcec0, 
		0xc442, 
		0xd200, 
		0x53e0, 
		0x4000, 
		0x0000, 
		0xce40, 
		0xaa03, 
		0x2eb4, 
		0x5260, 
		0xcfc0, 
		0x4fc0, 
		0xc7f6, 
		0xd380, 
		0x4d80, 
		0xd060, 
		0xd000, 
		0x1426, 
		0x4c40, 
		0x4d57, 
		0x87a6, 
		0xe813, 
		0x1d4a, 
		0xc980, 
		0x76e8, 
		0xd280, 
		0x8000, 
		0x3c00, 
		0x6b5c, 
		0x7d73, 
		0xcec0, 
		0xd2c0, 
		0xf979, 
		0x6c0c, 
		0x5300, 
		0xc980, 
		0x53fc, 
		0x964e, 
		0x5140, 
		0x4900, 
		0x4dc0, 
		0x6de5, 
		0x5100, 
		0x8000, 
		0x51e0, 
		0xd0a0, 
		0xcf40, 
		0xce40, 
		0x36d3, 
		0x4e00, 
		0xcfc0, 
		0xc880, 
		0x5280, 
		0xcd40, 
		0xa0ae, 
		0x41ff, 
		0xd3a0, 
		0xcd40, 
		0x4600, 
		0x4c80, 
		0xd140, 
		0xaaf5, 
		0x8000, 
		0x5add, 
		0x51a0, 
		0xcd40, 
		0xd140, 
		0x0000, 
		0xe8bf, 
		0xd300, 
		0x8220, 
		0x5280, 
		0x5180, 
		0x53a0, 
		0xd260, 
		0xc000, 
		0xc900, 
		0x4cc0, 
		0x4400, 
		0x4d03, 
		0xd260, 
		0x2e75, 
		0xc900, 
		0xd200, 
		0x4900, 
		0x5000, 
		0x5120, 
		0xaeb2, 
		0x022d, 
		0x5160, 
		0xc700, 
		0xc400, 
		0x4f80, 
		0x50e0, 
		0xcd80, 
		0xcb80, 
		0x11dd, 
		0x00fd, 
		0x4b80, 
		0x5100, 
		0x8000, 
		0x5120, 
		0x5180, 
		0x4200, 
		0x280e, 
		0x4cbe, 
		0xfcbf, 
		0x8e07, 
		0xd260, 
		0x4c00, 
		0x058a, 
		0xdb81, 
		0x4e00, 
		0xd280, 
		0xfd8d, 
		0xc500, 
		0xa002, 
		0x0ff8, 
		0xea50, 
		0xaf01, 
		0xd0a0, 
		0x7c00, 
		0x50c0, 
		0x6e15, 
		0x330a, 
		0x5240, 
		0x3c00, 
		0xf832, 
		0x5c67, 
		0x5080, 
		0x0000, 
		0x51e0, 
		0xd160, 
		0x4703, 
		0x10b0, 
		0x4f00, 
		0x37ee, 
		0xd100, 
		0xfc00, 
		0x7c00, 
		0x4700, 
		0xa817, 
		0xc600, 
		0x4900, 
		0x5160, 
		0xd2c0, 
		0x1241, 
		0x2696, 
		0x4cc0, 
		0x5060, 
		0x0000, 
		0x5220, 
		0x8113, 
		0x5879, 
		0x4a00, 
		0xff57, 
		0xd020, 
		0x52c0, 
		0xd3a0, 
		0xb3e9, 
		0xcf00, 
		0x8000, 
		0xc880, 
		0xcd80, 
		0xd160, 
		0xd0c0, 
		0x5380, 
		0xfc00, 
		0xfb63, 
		0xd180, 
		0x50a0, 
		0x50c0, 
		0xa0df, 
		0x9c4c, 
		0xd1c0, 
		0x7c00, 
		0xd180, 
		0x8000, 
		0x4600, 
		0x5320, 
		0xd200, 
		0x5200, 
		0xd000, 
		0x0138, 
		0xce40, 
		0x9db7, 
		0xcf80, 
		0xdd24, 
		0x8000, 
		0x5340, 
		0x4c00, 
		0x6e61, 
		0x4dc0, 
		0xc000, 
		0x4e40, 
		0x4b00, 
		0x4e80, 
		0xcb00, 
		0x8000, 
		0x4600, 
		0xcf40, 
		0x4800, 
		0xc800, 
		0x52a0, 
		0xd0e0, 
		0xce00, 
		0x4880, 
		0x4000, 
		0xd6e9, 
		0xbddd, 
		0x8000, 
		0xcd80, 
		0x843b, 
		0x7ca0, 
		0x8000, 
		0x0000, 
		0x4980, 
		0xcf3d, 
		0x4f80, 
		0xcd00, 
		0x53c0, 
		0x26be, 
		0xce00, 
		0x4e40, 
		0xd140, 
		0xd040, 
		0x3cdf, 
		0xd060, 
		0x5100, 
		0xfc7a, 
		0x4980, 
		0x50a0, 
		0x4700, 
		0x4880, 
		0x4600, 
		0x4900, 
		0x52e0, 
		0x5100, 
		0x4d40, 
		0x4dc0, 
		0xcf00, 
		0x7c00, 
		0x8000, 
		0xd380, 
		0x5080, 
		0x8000, 
		0x55c0, 
		0xd000, 
		0xc880, 
		0x4f40, 
		0x5140, 
		0x4e00, 
		0xd280, 
		0x8000, 
		0x0000, 
		0x8000, 
		0xcdc0, 
		0x4fc0, 
		0xb2de, 
		0x4fc8, 
		0xc700, 
		0x8518, 
		0x4d80, 
		0x4b00, 
		0xf8ee, 
		0xbc6c, 
		0x50e0, 
		0xcc40, 
		0xfc00, 
		0xd000, 
		0xc822, 
		0x4e80, 
		0x4e80, 
		0x5280, 
		0xc880, 
		0xc800, 
		0x4c40, 
		0xfc00, 
		0x4b80, 
		0x4200, 
		0xda5f, 
		0xd1a0, 
		0x5000, 
		0x0000, 
		0xd080, 
		0x4f80, 
		0x56e9, 
		0x4400, 
		0x5380, 
		0xd000, 
		0xe471, 
		0x50a0, 
		0xd000, 
		0x13f8, 
		0xc880, 
		0xd340, 
		0xcf79, 
		0xd3a0, 
		0x5360, 
		0xce40, 
		0xd100, 
		0x6bda, 
		0x4900, 
		0xc900, 
		0x5060, 
		0x0d70, 
		0xca00, 
		0x2ec9, 
		0x4c40, 
		0xfc00, 
		0x4c40, 
		0xc600, 
		0x5bd7, 
		0x1caf, 
		0xd260, 
		0xc980, 
		0xd2e0, 
		0x3527, 
		0xd080, 
		0xcf40, 
		0xcfc0, 
		0x4f00, 
		0x4880, 
		0x5080, 
		0xca80, 
		0xc600, 
		0xaa0e, 
		0xcc40, 
		0xd020, 
		0x0000, 
		0x0000, 
		0xd120, 
		0xd140, 
		0xcf00, 
		0x52e0, 
		0xd200, 
		0xd300, 
		0x47aa, 
		0x8000, 
		0xfc00, 
		0x1371, 
		0xd040, 
		0x0000, 
		0xd100, 
		0x8212, 
		0xfc00, 
		0xd1c0, 
		0x4b80, 
		0x4900, 
		0xd160, 
		0x4a80, 
		0x8dce, 
		0x505a, 
		0x53c0, 
		0x4500, 
		0x58ee, 
		0x4f00, 
		0xa218, 
		0x4c40, 
		0xd220, 
		0x4e40, 
		0x5280, 
		0xcf00, 
		0x8917, 
		0xc900, 
		0xc700, 
		0x1f98, 
		0xd200, 
		0xcf40, 
		0x4c8b, 
		0xd1e0, 
		0x4a58, 
		0xd2e0, 
		0xcdc0, 
		0x6a8a, 
		0xa1dd, 
		0xd080, 
		0x5140, 
		0xd100, 
		0xd3e0, 
		0x5380, 
		0x4000, 
		0x5200, 
		0xfd2f, 
		0xcb80, 
		0x51e0, 
		0xdef1, 
		0x3d80, 
		0x049f, 
		0x50a0, 
		0xcb80, 
		0x95bf, 
		0xd0e0, 
		0xd060, 
		0xc400, 
		0xfc00, 
		0x4d53, 
		0xcc80, 
		0xcf40, 
		0xc400, 
		0x4f00, 
		0x0000, 
		0x0000, 
		0xbed1, 
		0x5160, 
		0xc700, 
		0xaafb, 
		0x53a0, 
		0x5fe4, 
		0xd000, 
		0x8000, 
		0x655f, 
		0xc9f1, 
		0x4e00, 
		0x52c0, 
		0xfc00, 
		0xce80, 
		0x5100, 
		0x00ea, 
		0xd000, 
		0x8000, 
		0xd2c0, 
		0x986c, 
		0x8000, 
		0x5240, 
		0xeee6, 
		0x0945, 
		0xcd00, 
		0x4b80, 
		0xcf00, 
		0x8000, 
		0x5f6d, 
		0xd140, 
		0x4e80, 
		0x4e40, 
		0xd2c0, 
		0x58af, 
		0xc000, 
		0x0000, 
		0x79da, 
		0x4d80, 
		0xd320, 
		0xce00, 
		0xd160, 
		0x4f80, 
		0x51e0, 
		0xd1a0, 
		0x0883, 
		0x5340, 
		0x52c0, 
		0xc578, 
		0xd340, 
		0xce00, 
		0x5060, 
		0x2250, 
		0x5380, 
		0x4d57, 
		0x51e0, 
		0x3e02, 
		0xfc00, 
		0x5525, 
		0x0cda, 
		0xd120, 
		0x3cf8, 
		0x4e80, 
		0x40c2, 
		0x4700, 
		0x8000, 
		0x5007, 
		0xa962, 
		0x4a00, 
		0x8bd9, 
		0x4000, 
		0xcb80, 
		0xd2e0, 
		0x4e40, 
		0xd020, 
		0xd040, 
		0xd3a0, 
		0xb339, 
		0xcec0, 
		0xd2c0, 
		0xd320, 
		0xa085, 
		0x01dd, 
		0xe468, 
		0xc400, 
		0xd000, 
		0x513d, 
		0x53e0, 
		0x4200, 
		0x4880, 
		0xfde2, 
		0x5380, 
		0xd320, 
		0x4b80, 
		0x5180, 
		0xd3e0, 
		0x4e40, 
		0x5100, 
		0x1c74, 
		0x7c00, 
		0xbedc, 
		0xcf00, 
		0x4700, 
		0x2cd8, 
		0x4b00, 
		0xcd00, 
		0x5200, 
		0x80dc, 
		0xd2e0, 
		0xcd40, 
		0x5160, 
		0x5020, 
		0x9672, 
		0xd0e0, 
		0x4c80, 
		0x8000, 
		0x52a0, 
		0xd000, 
		0x53c0, 
		0x8761, 
		0x5360, 
		0x4d80, 
		0x7829, 
		0xcc80, 
		0xd140, 
		0xdb71, 
		0xf8cd, 
		0xd320, 
		0x7ba7, 
		0xfc00, 
		0x0000, 
		0x8ac2, 
		0x4f80, 
		0xd240, 
		0xca00, 
		0xcdc0, 
		0xccc0, 
		0x51c2, 
		0xd1a0, 
		0x0000, 
		0x4500, 
		0xa9dc, 
		0x50c0, 
		0xf263, 
		0xd2a0, 
		0xcec0, 
		0x4d00, 
		0x8d22, 
		0x7c00, 
		0x4dc0, 
		0x5200, 
		0xc200, 
		0xd3c0, 
		0x5060, 
		0x4fc0, 
		0xc000, 
		0xd2a0, 
		0x5340, 
		0x0000, 
		0x4a80, 
		0xd0c0, 
		0x7c00, 
		0x4cf6, 
		0xcc40, 
		0x4e40, 
		0x4200, 
		0xd2a0, 
		0x485a, 
		0xdbed, 
		0xfc00, 
		0xd380, 
		0x5000, 
		0xd020, 
		0x4980, 
		0x7786, 
		0x50a0, 
		0xce80, 
		0xca80, 
		0x219e, 
		0xcf80, 
		0x5040, 
		0x1063, 
		0x5280, 
		0x16fd, 
		0x7c00, 
		0x0000, 
		0xc980, 
		0xc980, 
		0xd000, 
		0xd160, 
		0x4fc0, 
		0xcb00, 
		0xc900, 
		0xfe6b, 
		0x7c00, 
		0x5080, 
		0x4fc0, 
		0xfc00, 
		0xcd80, 
		0x98ef, 
		0x2f1d, 
		0x52a0, 
		0x7c00, 
		0x4fc0, 
		0x826e, 
		0xcc80, 
		0x5000, 
		0x3e95, 
		0x3c00, 
		0x1ec6, 
		0x5240, 
		0x4f80, 
		0x08ce, 
		0xe9b2, 
		0x09ce, 
		0x4d00, 
		0x346d, 
		0x8170, 
		0x4fc0, 
		0x0000, 
		0x5240, 
		0xd240, 
		0x7d82, 
		0x4700, 
		0x4f00, 
		0x3841, 
		0xcc00, 
		0x0939, 
		0x4200, 
		0x4e00, 
		0x4ec0, 
		0xd3c0, 
		0xce00, 
		0x5280, 
		0xa008, 
		0xd260, 
		0xd1c0, 
		0x4400, 
		0xc880, 
		0x4500, 
		0xda46, 
		0xc700, 
		0x95b9, 
		0xcfc0, 
		0x8000, 
		0xc900, 
		0x4c80, 
		0x6009, 
		0x54f7, 
		0x5060, 
		0xce40, 
		0x8000, 
		0xcca2, 
		0x4e00, 
		0x4a00, 
		0x5060, 
		0x5140, 
		0x5060, 
		0x3c00, 
		0x55c4, 
		0x8836, 
		0x53c0, 
		0x870c, 
		0x8000, 
		0x53e0, 
		0x0e79, 
		0xfc00, 
		0x4800, 
		0x7154, 
		0x4a80, 
		0x990c, 
		0x49f5, 
		0xd380, 
		0x9240, 
		0x4f00, 
		0x8000, 
		0x4dc0, 
		0x4c00, 
		0xfc00, 
		0x4033, 
		0x8241, 
		0x0000, 
		0x8000, 
		0x7c00, 
		0xd360, 
		0x52e0, 
		0x53a0, 
		0x4200, 
		0x0000, 
		0x1ea8, 
		0xbad6, 
		0x53a0, 
		0x9dd9, 
		0xd240, 
		0xbcb7, 
		0xd1e0, 
		0x4e40, 
		0xd1a0, 
		0x4f80, 
		0x4f40, 
		0x6a14, 
		0x53e0, 
		0xc400, 
		0x4800, 
		0x5020, 
		0x9309, 
		0xc700, 
		0xbf2d, 
		0x0000, 
		0xf8b2, 
		0x5260, 
		0x4400, 
		0x5300, 
		0xc291, 
		0x4c40, 
		0x5200, 
		0x81fd, 
		0x3c00, 
		0xd100, 
		0xd3e0, 
		0x7c00, 
		0x4e00, 
		0xd300, 
		0xce80, 
		0xf2b6, 
		0xc980, 
		0x2df2, 
		0x1ab8, 
		0xd3c0, 
		0x4c40, 
		0x5320, 
		0x7763, 
		0xf454, 
		0xefa1, 
		0xcf00, 
		0x16f0, 
		0xd1e0, 
		0xd020, 
		0xfd2a, 
		0x5380, 
		0xd0e0, 
		0xce80, 
		0xca80, 
		0x53e0, 
		0x5260, 
		0x51c0, 
		0x4a00, 
		0x0000, 
		0x5220, 
		0xd1a0, 
		0xcb00, 
		0x4f00, 
		0xc400, 
		0x9fe6, 
		0x97fa, 
		0xc400, 
		0x5240, 
		0x91e5, 
		0x5fa9, 
		0x3c00, 
		0x4e00, 
		0xd6c9, 
		0x0f14, 
		0x4fc0, 
		0x4500, 
		0x4c00, 
		0x8000, 
		0xd360, 
		0x5fef, 
		0xe669, 
		0x5fce, 
		0x4c00, 
		0xd300, 
		0x5060, 
		0xcf4e, 
		0x0000, 
		0xd2e0, 
		0xcf80, 
		0xd237, 
		0x79dc, 
		0xd180, 
		0x8000, 
		0x4d40, 
		0xce40, 
		0xd200, 
		0x5320, 
		0xca00, 
		0xd297, 
		0x935b, 
		0xca80, 
		0xcd00, 
		0x4c40, 
		0x02d8, 
		0x3af0, 
		0x50a0, 
		0x5380, 
		0xd439, 
		0x336e, 
		0xd260, 
		0x4932, 
		0x4b6c, 
		0x32a6, 
		0xc400, 
		0x5240, 
		0xd040, 
		0x52e0, 
		0x30e9, 
		0xd280, 
		0x3abb, 
		0x8000, 
		0x5280, 
		0x0000, 
		0x8000, 
		0x4b00, 
		0x5ba7, 
		0xc200, 
		0x4500, 
		0xc200, 
		0x4600, 
		0xc000, 
		0x5451, 
		0xd3e0, 
		0x699b, 
		0xccc0, 
		0x5120, 
		0x5e6b, 
		0xcd00, 
		0x36fd, 
		0x5060, 
		0x0000, 
		0xd260, 
		0x8000, 
		0x0000, 
		0xc600, 
		0x4cc0, 
		0x53a0, 
		0x4000, 
		0xd9f8, 
		0x4200, 
		0x1cc5, 
		0x187b, 
		0x5280, 
		0xc980, 
		0x4b6b, 
		0x50e0, 
		0x5120, 
		0xd140, 
		0xd280, 
		0x4a00, 
		0xd080, 
		0x4dc0, 
		0x4a00, 
		0xd240, 
		0x9671, 
		0x4800, 
		0xdcdb, 
		0x52a0, 
		0xa1ca, 
		0xd320, 
		0x8000, 
		0x8000, 
		0x51e0, 
		0x87ae, 
		0x5360, 
		0xce40, 
		0x4700, 
		0x7c00, 
		0x785a, 
		0x5340, 
		0xe4ae, 
		0xce40, 
		0x4d80, 
		0xc200, 
		0x3243, 
		0xa173, 
		0x4dc0, 
		0x4500, 
		0x4cc0, 
		0x66a0, 
		0x5dbc, 
		0x5360, 
		0x4b00, 
		0x4800, 
		0x10b3, 
		0xfc00, 
		0x5380, 
		0xea5c, 
		0x5000, 
		0x5380, 
		0xd140, 
		0x5360, 
		0x4fc0, 
		0x4800, 
		0xcc69, 
		0x5300, 
		0xd140, 
		0xccbe, 
		0x8000, 
		0xc800, 
		0xce80, 
		0xd120, 
		0xd040, 
		0x4880, 
		0xd260, 
		0xd360, 
		0xd120, 
		0x3dbf, 
		0xa27c, 
		0xd3c0, 
		0x5f54, 
		0x5380, 
		0xd320, 
		0x0000, 
		0xd2a0, 
		0xd2c0, 
		0xc4a2, 
		0x122e, 
		0x2e7a, 
		0x5020, 
		0x70ed, 
		0xb920, 
		0x4c80, 
		0x5240, 
		0xcec0, 
		0xbda4, 
		0x5220, 
		0x5220, 
		0x7c00, 
		0xbbe6, 
		0xd2e0, 
		0x52a0, 
		0x4000, 
		0x51a0, 
		0x52a0, 
		0xb027, 
		0x1164, 
		0xb90e, 
		0x4600, 
		0x528c, 
		0x2c90, 
		0xd000, 
		0x4000, 
		0xcb00, 
		0xfc00, 
		0x4400, 
		0x4880, 
		0x53e0, 
		0x4e80, 
		0x5320, 
		0x50c0, 
		0x4237, 
		0xd340, 
		0x8a58, 
		0x53a0, 
		0xc600, 
		0xf74c, 
		0x50a0, 
		0x71de, 
		0x4a80, 
		0x4800, 
		0x014d, 
		0xbc4c, 
		0x4980, 
		0x5240, 
		0xc900, 
		0x4c00, 
		0x7c00, 
		0x4c40, 
		0x119f, 
		0xce00, 
		0x8000, 
		0x8000, 
		0xf8ab, 
		0x52c0  
};
volatile uint32_t rand_ieee754_32[1536] __attribute__ ((aligned (2048))) = {
		0x0c7ff000, 
		0x0d00fff0, 
		0x00400000, 
		0xaaaaaaaa, 
		0x00000000, 
		0x80000200, 
		0x00000002, 
		0x7ac5629b, 
		0x80008000, 
		0x807ffffe, 
		0x80000000, 
		0x80000020, 
		0x0e3fffff, 
		0x905e1473, 
		0x41800000, 
		0x80000000, 
		0xff800000, 
		0xc2740000, 
		0x80800000, 
		0x8b5dd09a, 
		0x80000080, 
		0xc1a00000, 
		0x7fc00001, 
		0x3f800000, 
		0x80000010, 
		0xff800000, 
		0x2fdf1229, 
		0x7f800000, 
		0x80000008, 
		0x73b5cd8c, 
		0xc2700000, 
		0x00000400, 
		0x2b62e41e, 
		0x00000000, 
		0x78f2246a, 
		0x945160b1, 
		0x00ffffff, 
		0x007ffffe, 
		0x14a2c3d1, 
		0x00000000, 
		0x00800002, 
		0x56177ab2, 
		0x00800001, 
		0x000bb814, 
		0x80000000, 
		0x0c7ff800, 
		0xcccccccc, 
		0xc23c0000, 
		0xe54ac8a0, 
		0xfe1c2769, 
		0x42740000, 
		0x8c073a88, 
		0x0c7fc000, 
		0xba77f078, 
		0x40000000, 
		0x8f7ffffc, 
		0xc2400000, 
		0x0c7fe000, 
		0x42340000, 
		0xbd382c8e, 
		0x80000000, 
		0x80000000, 
		0x35faa571, 
		0xc1400000, 
		0xff800000, 
		0x80800003, 
		0x42680000, 
		0x00011111, 
		0x73b9f6d6, 
		0xc2080000, 
		0x00100000, 
		0xff800000, 
		0xc2540000, 
		0x92010a0f, 
		0xc86002ec, 
		0xbf3adc42, 
		0xff9ce1d9, 
		0xc23c0000, 
		0xc0e00000, 
		0x421c0000, 
		0x7f800000, 
		0x41f80000, 
		0xcc7cc1d5, 
		0x0e0003ff, 
		0xff800001, 
		0xc2780000, 
		0x80000000, 
		0x42340000, 
		0xc1400000, 
		0x88a0cea1, 
		0x7fbfffff, 
		0x00800001, 
		0x80080000, 
		0x6d1bbe97, 
		0x42080000, 
		0x6217682c, 
		0x00020000, 
		0x00800002, 
		0x0342c29f, 
		0xc26c0000, 
		0xf952a677, 
		0xbf800001, 
		0xc1000000, 
		0xc1d80000, 
		0x5f88f2a9, 
		0x80000001, 
		0x1b4dbf3b, 
		0x0e0000ff, 
		0xc1100000, 
		0xba326e16, 
		0x0f7ffffd, 
		0x244645c4, 
		0xff000000, 
		0x0c7fff00, 
		0xfffb5f8e, 
		0x42200000, 
		0x42240000, 
		0x00000040, 
		0x7cfd1387, 
		0x0e01ffff, 
		0x8ef58200, 
		0x9169f627, 
		0x0c7fff80, 
		0x8ffd9af0, 
		0x80080000, 
		0xaba7f8a5, 
		0xff800000, 
		0x42040000, 
		0x80000000, 
		0x41300000, 
		0x7f800000, 
		0x0d000ff0, 
		0x7f800000, 
		0x0469c4bd, 
		0x7f800000, 
		0xff800000, 
		0x80008000, 
		0x26e25a89, 
		0x00000000, 
		0xd9605016, 
		0x7f800000, 
		0xfa611c89, 
		0x0e00007f, 
		0x1e8c1159, 
		0x0c7ffff0, 
		0x134149d1, 
		0xd053fb85, 
		0x41d80000, 
		0x80ffffff, 
		0xc2540000, 
		0xc1000000, 
		0xc22c0000, 
		0x37390eae, 
		0x41c80000, 
		0xcdb81e62, 
		0x424c0000, 
		0xc1d00000, 
		0x5d09447b, 
		0x9a45e0f3, 
		0x80463a3a, 
		0xffb46a09, 
		0xfaada6e1, 
		0x80800003, 
		0x7888747b, 
		0x0c7ffff0, 
		0x80000000, 
		0x73cde918, 
		0xc1e00000, 
		0x00ffffff, 
		0xe8801f3c, 
		0x80000100, 
		0x42580000, 
		0xc2400000, 
		0xcda59b5c, 
		0x00800000, 
		0x03d7a15d, 
		0xe7a21a94, 
		0x8e8cef06, 
		0x42480000, 
		0x0c7ff800, 
		0xff800000, 
		0x80040000, 
		0x42780000, 
		0x33333333, 
		0x80011111, 
		0x00080000, 
		0xc2280000, 
		0x80000000, 
		0x00000000, 
		0x41c80000, 
		0xff921e10, 
		0x3e9d3d44, 
		0x0c7ff000, 
		0x67ca3a8a, 
		0x00000100, 
		0x0e01ffff, 
		0x80bec8e9, 
		0x6b5e2118, 
		0xc0800000, 
		0xffa9edcd, 
		0x7f800000, 
		0x80000040, 
		0xe58a8ba0, 
		0x53e156e0, 
		0x0c7ffff8, 
		0x0e01ffff, 
		0x6ead61bf, 
		0x7b670644, 
		0xf9380696, 
		0x00000040, 
		0x353ac8dc, 
		0x41b80000, 
		0x80608c75, 
		0x41600000, 
		0xff800000, 
		0x7fc00001, 
		0x00000000, 
		0xff800000, 
		0xc1a00000, 
		0x00000000, 
		0x7f800000, 
		0x80800001, 
		0xdf0c2003, 
		0x80000020, 
		0x0f7ffffd, 
		0x0c7ffff0, 
		0x75a0112a, 
		0x9ea5a8af, 
		0x291c1121, 
		0x4977cc4c, 
		0xff7ffffe, 
		0x2bc7642c, 
		0xc22c0000, 
		0x3f800001, 
		0x7f800000, 
		0xaaaaaaaa, 
		0x0c7fff80, 
		0x80000040, 
		0x00008000, 
		0xc2280000, 
		0x3a05a4c4, 
		0xc1f00000, 
		0xff800000, 
		0x0f7fffff, 
		0xcb8c4b40, 
		0x42500000, 
		0xc24c0000, 
		0xfd86d47c, 
		0x80000000, 
		0x7fffffff, 
		0x00020000, 
		0xeb1734b2, 
		0xd424657c, 
		0x80011111, 
		0x40c00000, 
		0x0c700000, 
		0xca6ec735, 
		0x80800000, 
		0xeadff551, 
		0x7fa5059d, 
		0xfd52bb2d, 
		0x00011111, 
		0x18cfe547, 
		0xc3e16ba4, 
		0x80001000, 
		0x00000000, 
		0x80000200, 
		0xf34b7bcd, 
		0x00011111, 
		0x7fc00001, 
		0x80800001, 
		0x80000000, 
		0x8f7ffffe, 
		0x4bc9107f, 
		0x5bebd8bc, 
		0x2426301a, 
		0x7f800000, 
		0xffc00000, 
		0x0e0003ff, 
		0x7f800000, 
		0x80000000, 
		0xa6e0713a, 
		0x40e00000, 
		0x41f80000, 
		0x80800003, 
		0x0c7ff000, 
		0x80000000, 
		0x3f800000, 
		0x7927151d, 
		0x3f800001, 
		0x80579c52, 
		0x7f800000, 
		0x80774cda, 
		0x0e0001ff, 
		0xc2700000, 
		0x42740000, 
		0x00004000, 
		0x3492e9c1, 
		0x26223a9a, 
		0xc2640000, 
		0x00800001, 
		0x0c7ffc00, 
		0x88d297c0, 
		0x41e00000, 
		0x6b49857d, 
		0xffb5cb97, 
		0x78147839, 
		0xbbfdb623, 
		0x80000001, 
		0x00000000, 
		0x5500417d, 
		0x7f800000, 
		0xcccccccc, 
		0x0c600000, 
		0xc2700000, 
		0x0c600000, 
		0x40c00000, 
		0xffaf4b34, 
		0x0f35f2ae, 
		0x41800000, 
		0x80000001, 
		0x42180000, 
		0xffefc7bd, 
		0x00011111, 
		0xc0c00000, 
		0xc28300ef, 
		0x41a80000, 
		0x0c7fffc0, 
		0x00000200, 
		0x0c7fffff, 
		0x426c0000, 
		0xffc00001, 
		0x48b711e3, 
		0x80412d0c, 
		0x0c7ffffc, 
		0x80800003, 
		0x00000000, 
		0x00000000, 
		0xffbf8e15, 
		0xc26c0000, 
		0x857c0f1e, 
		0x41d80000, 
		0x41a80000, 
		0x426c0000, 
		0xc1d00000, 
		0x3b163dec, 
		0xddd9eaef, 
		0x7f800000, 
		0x2001c591, 
		0xc2400000, 
		0x0e0003ff, 
		0x0e3fffff, 
		0xff800000, 
		0x730c4a9a, 
		0x80000000, 
		0xff800000, 
		0x80800000, 
		0xe824aec5, 
		0x45b20387, 
		0xf87c1c3d, 
		0x0d00fff0, 
		0x41e00000, 
		0x80000200, 
		0xc1200000, 
		0xc1400000, 
		0x427c0000, 
		0x80800000, 
		0xc2400000, 
		0x00000004, 
		0x426c0000, 
		0x5b7099ac, 
		0x4efed41b, 
		0xffacd8d9, 
		0x33263b76, 
		0x427c0000, 
		0x42580000, 
		0xffbffc74, 
		0xc1c80000, 
		0x0c7f0000, 
		0x80080000, 
		0x9e4081eb, 
		0x7fa006b9, 
		0x0965ef3d, 
		0x0e03ffff, 
		0x7fbfffff, 
		0x59764f60, 
		0x41b00000, 
		0x80000000, 
		0xc1500000, 
		0x42500000, 
		0x80000008, 
		0xf573bb3a, 
		0x421c0000, 
		0x3f800000, 
		0x0e0000ff, 
		0xd29b8ad1, 
		0x42540000, 
		0xc6f4091b, 
		0x80000008, 
		0xfe54f98f, 
		0xbdfabbf9, 
		0x7f800000, 
		0xff7fffff, 
		0x0e001fff, 
		0x8d7f6a98, 
		0xff000000, 
		0xc22c0000, 
		0x41f00000, 
		0xcccccccc, 
		0x55555555, 
		0x7fb46b76, 
		0x52f1351d, 
		0xbb1ef370, 
		0xaf25ce43, 
		0xfb92caf0, 
		0x00000001, 
		0x0e00ffff, 
		0xc1980000, 
		0x00800000, 
		0x6f69932e, 
		0xc2680000, 
		0x807fffff, 
		0x3f800000, 
		0x41880000, 
		0x80000010, 
		0x0e007fff, 
		0xc1980000, 
		0xd6db50a8, 
		0x44fd9519, 
		0x42960182, 
		0x00000000, 
		0x00000001, 
		0x12115ece, 
		0x80000004, 
		0x7f7ffffe, 
		0x7708fba2, 
		0xbf028f5c, 
		0x80000000, 
		0x41880000, 
		0x00000010, 
		0x00000000, 
		0xde61c6db, 
		0x00ffffff, 
		0xc0e00000, 
		0xc1d80000, 
		0x5581e555, 
		0xc1e00000, 
		0x80000008, 
		0xc2480000, 
		0xc1900000, 
		0x7f800001, 
		0xff800000, 
		0xe1bee225, 
		0x80000008, 
		0x42240000, 
		0xf110c1af, 
		0xa450a76e, 
		0x0f7ffffd, 
		0x87761fb6, 
		0x0f7fffff, 
		0xff7ffffe, 
		0x00282035, 
		0x00040000, 
		0x00000004, 
		0x00000000, 
		0xc1b80000, 
		0x00001000, 
		0xfed0ec16, 
		0x7f803f33, 
		0xc1c80000, 
		0xda99d4f1, 
		0xc2000000, 
		0x6d9b02d6, 
		0x42700000, 
		0x0c7f0000, 
		0x0c7fe000, 
		0x007ffffe, 
		0x41c80000, 
		0x42080000, 
		0x80800000, 
		0xc2780000, 
		0x41600000, 
		0x42580000, 
		0x689b5f77, 
		0xc23c0000, 
		0x03490497, 
		0xecd768f7, 
		0x00000000, 
		0x27007aad, 
		0x6aefdafa, 
		0xc2040000, 
		0x7f800000, 
		0x7f7ffffe, 
		0xc41d4156, 
		0x80000200, 
		0xc5088908, 
		0x80800001, 
		0xb0738abb, 
		0xff800000, 
		0xc2380000, 
		0xcf3201b5, 
		0x7f852536, 
		0x8f7ffffe, 
		0xcb8c4b40, 
		0xd5e8ae78, 
		0x38d07a47, 
		0x00000000, 
		0xebeb0515, 
		0x6c0d5329, 
		0xe5b3a232, 
		0xc2640000, 
		0x00000000, 
		0x2d52234f, 
		0x49dd939c, 
		0x42100000, 
		0xcd7fc6bb, 
		0x41a00000, 
		0x5b9c6a98, 
		0xbd1b551e, 
		0x00000020, 
		0x41c00000, 
		0x80000010, 
		0x00000000, 
		0xcb000000, 
		0x4b000000, 
		0x2b3b93f4, 
		0x922eff2c, 
		0x66513f8a, 
		0x80000000, 
		0xbf028f5c, 
		0x2d1fafbe, 
		0xc1d00000, 
		0x41980000, 
		0x3445c993, 
		0x80000000, 
		0x42040000, 
		0xd0756d25, 
		0x8c89b987, 
		0x0f7ffffc, 
		0x421c0000, 
		0xcb000000, 
		0x7fa29c71, 
		0x1c302977, 
		0xe439108a, 
		0x422c0000, 
		0x0e03ffff, 
		0x41e80000, 
		0xc2180000, 
		0xc25c0000, 
		0xeda852f4, 
		0x2d72ee57, 
		0x80000000, 
		0x0e003fff, 
		0x239e9e99, 
		0xc25c0000, 
		0x42200000, 
		0x0e00ffff, 
		0x41d00000, 
		0x00000000, 
		0xc26c0000, 
		0x7fc00000, 
		0x42500000, 
		0x8623bc69, 
		0x41a80000, 
		0xfc471098, 
		0x00400000, 
		0x80800000, 
		0xc2080000, 
		0xbadf226d, 
		0x802b819e, 
		0xc23c0000, 
		0x42480000, 
		0x00800001, 
		0xc2400000, 
		0x00000001, 
		0x2058de88, 
		0x80000001, 
		0x41500000, 
		0x0c7c0000, 
		0x7f800000, 
		0x7f800000, 
		0xc1d80000, 
		0x0c7ff800, 
		0x00020000, 
		0x80000020, 
		0x2540babd, 
		0xfbaddbcc, 
		0x80000001, 
		0x7f7fffff, 
		0x0e0007ff, 
		0x80100000, 
		0xc1400000, 
		0x0c7fe000, 
		0x80001000, 
		0x9cdac1b3, 
		0x00800000, 
		0x62b0d256, 
		0x9544461e, 
		0x00000000, 
		0x42040000, 
		0xc0a00000, 
		0xc1800000, 
		0xc2380000, 
		0x00000200, 
		0xda66ffb9, 
		0x1208f7eb, 
		0x0cfaca35, 
		0x80000100, 
		0x007fffff, 
		0xe258e9a2, 
		0x41f80000, 
		0x1a161077, 
		0xc20c0000, 
		0x5ecdb141, 
		0x42080000, 
		0x80000080, 
		0x422c0000, 
		0x00000000, 
		0x7f800000, 
		0xe42f9f11, 
		0xffc00000, 
		0x329a6486, 
		0x00000200, 
		0xc2380000, 
		0x7f97c6b4, 
		0x2b986806, 
		0xc24c0000, 
		0x00000000, 
		0x01655f96, 
		0xc0e00000, 
		0x80000001, 
		0xf9b9fe20, 
		0x0f7ffffe, 
		0x0d000ff0, 
		0x80ffffff, 
		0x63d24fb5, 
		0x72143113, 
		0x80000000, 
		0x41c00000, 
		0xc2080000, 
		0x0e0fffff, 
		0xd67434fd, 
		0x423c0000, 
		0x0e7fffff, 
		0x42140000, 
		0xc0c00000, 
		0x0e0007ff, 
		0x007fffff, 
		0x26b7337a, 
		0xc1f80000, 
		0x00800001, 
		0x804a9680, 
		0x00000000, 
		0xc2600000, 
		0xdc62fbd3, 
		0x0b03f82b, 
		0xc26c0000, 
		0x423c0000, 
		0x87977c4f, 
		0x0c7ffe00, 
		0xc3168327, 
		0x00000020, 
		0xf750e659, 
		0x1ecf0b5f, 
		0x00000000, 
		0x55555555, 
		0x40c00000, 
		0x42140000, 
		0x80004000, 
		0xc2700000, 
		0x41c00000, 
		0x7f7ffffe, 
		0xb91f570f, 
		0xc1c80000, 
		0xdd8ebc97, 
		0xc1200000, 
		0x80800000, 
		0x80000000, 
		0x7f800000, 
		0x80200000, 
		0xac290dca, 
		0x41900000, 
		0x80000000, 
		0xc1900000, 
		0x0c700000, 
		0xff800000, 
		0x41f80000, 
		0xec759bf2, 
		0x00000000, 
		0x0c780000, 
		0x1545314c, 
		0x01afd09a, 
		0x80000800, 
		0xaf65e99a, 
		0xc1300000, 
		0x8d454e85, 
		0x41c80000, 
		0xff800000, 
		0x41400000, 
		0xc25c0000, 
		0x00000001, 
		0xff800001, 
		0xefe0d17f, 
		0x6574f19f, 
		0x80000800, 
		0x09b12bd4, 
		0x80800002, 
		0x41800000, 
		0x41b00000, 
		0xff000000, 
		0xfc059f4e, 
		0x41500000, 
		0xffb8546c, 
		0x7fc00000, 
		0xc1880000, 
		0x80000002, 
		0x0e000003, 
		0x00000000, 
		0x80100000, 
		0x4f3ef59e, 
		0xff800000, 
		0x80000004, 
		0x42440000, 
		0x0c7e0000, 
		0x00800000, 
		0x80800000, 
		0xc22c0000, 
		0xa49dd0b0, 
		0xc2180000, 
		0x30fd322a, 
		0x42440000, 
		0x42200000, 
		0xff800000, 
		0x0c7ffff0, 
		0x7a33925f, 
		0x42740000, 
		0x7f7ffffe, 
		0x00002000, 
		0x00000040, 
		0x0e0001ff, 
		0x43bbe2de, 
		0x54a8f82b, 
		0xefd8aa97, 
		0x7f7ffffe, 
		0x007fffff, 
		0x00800000, 
		0x80800003, 
		0xe54f5792, 
		0x40c00000, 
		0x9b4bef3b, 
		0xcccccccc, 
		0xe71cb27f, 
		0x007fffff, 
		0x80000000, 
		0xc0e00000, 
		0x41b80000, 
		0xeba65439, 
		0x872805c8, 
		0xf0259364, 
		0x0e03ffff, 
		0x00400000, 
		0x42780000, 
		0x1f5d0a40, 
		0xfff699d3, 
		0x42100000, 
		0x42140000, 
		0xf2e2d7fa, 
		0x7f800000, 
		0x607efdba, 
		0x424c0000, 
		0x00000800, 
		0x0c7fff00, 
		0x0e07ffff, 
		0x9ef14095, 
		0x6efb0592, 
		0x00000000, 
		0x887a6b52, 
		0x0c7ffffc, 
		0xc4001dcf, 
		0x1dc1f861, 
		0x0c700000, 
		0xfe67fc80, 
		0x00040000, 
		0x10895540, 
		0xc2340000, 
		0x2cd45600, 
		0x80000002, 
		0x8a626f7c, 
		0x0b1f0359, 
		0xceff6c94, 
		0xfb1c1660, 
		0x717de348, 
		0xd5c83b66, 
		0x80000000, 
		0x0e07ffff, 
		0x67fb43e6, 
		0x80000000, 
		0x80000001, 
		0x0c7fff00, 
		0x8f7ffffd, 
		0xc2580000, 
		0x80000000, 
		0xa0691920, 
		0x4ffa101b, 
		0xc1a00000, 
		0x423c0000, 
		0xdb705f60, 
		0xc1a00000, 
		0x80000000, 
		0xcb000000, 
		0x04b3d9f9, 
		0xc2700000, 
		0x2af3cfe4, 
		0x41e80000, 
		0x0c7ffff8, 
		0x807fffff, 
		0x0c700000, 
		0x40800000, 
		0x3f028f5c, 
		0x00000000, 
		0x80000100, 
		0x7a9b2bac, 
		0xff800000, 
		0x7f7ffffe, 
		0x42180000, 
		0x33333333, 
		0x80200000, 
		0x7f000000, 
		0x80800001, 
		0xaaaaaaaa, 
		0xa2276473, 
		0x91e4fe07, 
		0xff000000, 
		0x88565dd9, 
		0x42500000, 
		0x004ba040, 
		0x2bab8799, 
		0x0b489b25, 
		0xff9d0abb, 
		0x6d8006d3, 
		0xc1e80000, 
		0x3d28ce73, 
		0x41e00000, 
		0xff800000, 
		0xffbfffff, 
		0x3d976307, 
		0x42480000, 
		0x00000001, 
		0x3f584f83, 
		0x61b96ec5, 
		0xe1e12812, 
		0x7f7fffff, 
		0x80000010, 
		0xac16b6c8, 
		0x80000000, 
		0x7f98b1c1, 
		0xfaff3fdf, 
		0xc2780000, 
		0x0f7ffffc, 
		0xf98075a9, 
		0x00000200, 
		0x80080000, 
		0xc2700000, 
		0xc2780000, 
		0xb53f6b0e, 
		0x42300000, 
		0xa7973da4, 
		0x41a80000, 
		0x7228f9f2, 
		0x0e0003ff, 
		0x3f028f5c, 
		0x7fa1b29e, 
		0x42300000, 
		0xac7f4bd5, 
		0x8109cd2d, 
		0x80000000, 
		0x80800002, 
		0xc2380000, 
		0x589f741f, 
		0xdd6506e4, 
		0xc1f80000, 
		0x80000010, 
		0x7cd29079, 
		0x35ce036c, 
		0x42480000, 
		0xc1100000, 
		0xc2480000, 
		0xb76b876f, 
		0x80080000, 
		0xd19a1f7b, 
		0x42540000, 
		0x41300000, 
		0x42200000, 
		0x80000000, 
		0xc1a00000, 
		0x42300000, 
		0x00800002, 
		0xc0e00000, 
		0x4e0dbe71, 
		0xc2380000, 
		0xf0dca165, 
		0xc2180000, 
		0xc26c0000, 
		0x80000800, 
		0x0e07ffff, 
		0x0e0003ff, 
		0xc2140000, 
		0xbf800000, 
		0x80800001, 
		0x00800003, 
		0xffbfffff, 
		0x00400000, 
		0x2936a3e8, 
		0x80000800, 
		0xaaaaaaaa, 
		0x0c700000, 
		0x7fc00001, 
		0x8f5283c5, 
		0x0e00003f, 
		0x80000040, 
		0x3432d75c, 
		0x41c80000, 
		0x80800001, 
		0xc2440000, 
		0x444c8a08, 
		0x80008000, 
		0x42000000, 
		0xc2080000, 
		0x00000000, 
		0x00800003, 
		0x00000000, 
		0xffbfffff, 
		0x0dc667da, 
		0x426c0000, 
		0x80040000, 
		0x80000000, 
		0x0f7fffff, 
		0x42500000, 
		0x8f7fffff, 
		0x80000000, 
		0x0c7ff800, 
		0x4bf6c103, 
		0xb3370a90, 
		0xba664e24, 
		0x7c0f8fb3, 
		0xefeaedef, 
		0xc23c0000, 
		0xb0fa6a58, 
		0x67249c97, 
		0xc2500000, 
		0x9d9625cd, 
		0xbab8b8fd, 
		0x425c0000, 
		0x425c0000, 
		0xd7197755, 
		0x17e65d8f, 
		0xffba8f60, 
		0x41000000, 
		0xc1b80000, 
		0x41f80000, 
		0x799b8c00, 
		0x0c7ffe00, 
		0x2210eafc, 
		0x807ffffe, 
		0x40400000, 
		0xa5da1621, 
		0xc2380000, 
		0xffffffff, 
		0x02927755, 
		0x00000001, 
		0xd3e0b8c6, 
		0xc1880000, 
		0x7f800000, 
		0xf6faa741, 
		0x356943ea, 
		0xc2200000, 
		0xe655b2d6, 
		0x80011111, 
		0x80000800, 
		0x7fb81847, 
		0xcf9eec4a, 
		0x16659572, 
		0x41880000, 
		0x0e0000ff, 
		0x80800001, 
		0xc2300000, 
		0x0e0003ff, 
		0xd053451f, 
		0xc0a00000, 
		0x0c7fffff, 
		0x80000002, 
		0x00000002, 
		0x00200000, 
		0x00000200, 
		0xfff46f2b, 
		0x7aaf2adf, 
		0x80000000, 
		0x7f800000, 
		0xc24c0000, 
		0xc1300000, 
		0x14506f43, 
		0xff800001, 
		0x80000000, 
		0xc1a00000, 
		0x33333333, 
		0xff8770b6, 
		0x00800001, 
		0xffbfffff, 
		0xd6d5d95f, 
		0x00800000, 
		0x00000080, 
		0x41800000, 
		0xdee63f50, 
		0x0c7f0000, 
		0x33333333, 
		0xff800000, 
		0x42580000, 
		0xc0a00000, 
		0xc27c0000, 
		0xc1a80000, 
		0x8f7ffffc, 
		0x42140000, 
		0x6eebdaff, 
		0x00400000, 
		0x00000000, 
		0x42200000, 
		0x00000000, 
		0x7d7de082, 
		0x80800001, 
		0x41100000, 
		0x7fbfffff, 
		0xc20c0000, 
		0xc2180000, 
		0x96602913, 
		0x00415dfb, 
		0x00800001, 
		0x41d00000, 
		0x80800003, 
		0x00000001, 
		0x80004000, 
		0xff800000, 
		0xd833ea8c, 
		0x8f7ffffc, 
		0x00000002, 
		0x614bf574, 
		0x41700000, 
		0xc1800000, 
		0x2f977e51, 
		0x426c0000, 
		0x9927c8aa, 
		0x80800000, 
		0x73cf4f99, 
		0x0e0007ff, 
		0x00010000, 
		0xbb075b6b, 
		0xc629f861, 
		0x00008000, 
		0xc23c0000, 
		0x00200000, 
		0x41980000, 
		0xf3483683, 
		0x00000000, 
		0x00000000, 
		0x2421f64c, 
		0x424c0000, 
		0xc2000000, 
		0x6f278ba0, 
		0x42000000, 
		0x7f9b5673, 
		0x00000020, 
		0x0c7ffff0, 
		0x42440000, 
		0x0c7fc000, 
		0xc0e00000, 
		0x25ddd083, 
		0x80080000, 
		0xbf028f5c, 
		0x5763fb96, 
		0xc21c0000, 
		0x41200000, 
		0x9e7569e3, 
		0x00000001, 
		0x41980000, 
		0x00000000, 
		0x516dc8c3, 
		0x41880000, 
		0x80020000, 
		0x7fc00001, 
		0x7fc00001, 
		0x0c7fe000, 
		0x8e1230c0, 
		0x33901bce, 
		0xff800000, 
		0x0e00003f, 
		0x54564edc, 
		0xc2680000, 
		0x80000000, 
		0x40e00000, 
		0x3a9af95b, 
		0x80800000, 
		0xc1700000, 
		0x0e0001ff, 
		0xd8f2c1cc, 
		0xc27c0000, 
		0x80000000, 
		0xda99305d, 
		0x32c12f12, 
		0x4abeb37d, 
		0xc1f80000, 
		0xfa1c9bba, 
		0x1772ffc9, 
		0x0e3fffff, 
		0x7f800000, 
		0x804ed484, 
		0x4d45a8cb, 
		0x00000000, 
		0x003ee7e6, 
		0x00ffffff, 
		0x42580000, 
		0xffffffff, 
		0xcf84effd, 
		0x7f7ffffe, 
		0x0c7c0000, 
		0x0e07ffff, 
		0x9e034d2f, 
		0x7fcfce1f, 
		0x807fffff, 
		0x80053d2c, 
		0x80000040, 
		0x0d000ff0, 
		0x1a40f39a, 
		0x00000100, 
		0x00000000, 
		0xbd5102ca, 
		0x0e0fffff, 
		0x80800001, 
		0xc1d00000, 
		0x147ba756, 
		0x0e3fffff, 
		0x80002000, 
		0x974d2aa8, 
		0x0e0003ff, 
		0x79447787, 
		0x42740000, 
		0x0e00ffff, 
		0x80002000, 
		0x425c0000, 
		0x00000008, 
		0xd096f337, 
		0x094717c2, 
		0x80000000, 
		0xff800000, 
		0x0e0000ff, 
		0x3667904c, 
		0x41900000, 
		0x00800000, 
		0x0c7ff000, 
		0x42280000, 
		0x0e00000f, 
		0x0039edf9, 
		0x9d87f765, 
		0x41c00000, 
		0x00020000, 
		0x7f800000, 
		0xc1800000, 
		0x423c0000, 
		0x3ed5ec39, 
		0xc2380000, 
		0x0e0003ff, 
		0x79c0d02c, 
		0xc1a80000, 
		0x0e000001, 
		0x807ffffe, 
		0x0c7ffffc, 
		0x0035b76f, 
		0x58faabff, 
		0x80000002, 
		0x41e80000, 
		0x7b6f1ccf, 
		0xff800001, 
		0x41c00000, 
		0xc21c0000, 
		0xad066897, 
		0xc1f00000, 
		0x8fc3066b, 
		0x80000002, 
		0x60619fca, 
		0x27d85114, 
		0x7a5ef9fb, 
		0x00800003, 
		0xee16f2ee, 
		0x0c600000, 
		0x806f4a4b, 
		0xe0142311, 
		0xc151973e, 
		0x7f8b09f5, 
		0x7f7fffff, 
		0xbf800001, 
		0x0f7fffff, 
		0x84e4fe30, 
		0x80ffffff, 
		0xcbb2c7c3, 
		0x00000000, 
		0x00000000, 
		0x2af33ee2, 
		0x41000000, 
		0x08205514, 
		0x80000002, 
		0x36021032, 
		0x41500000, 
		0x0c7ffc00, 
		0x80000000, 
		0x89014266, 
		0xb1615c33, 
		0x42700000, 
		0x7763db0c, 
		0xd7175ab2, 
		0xeadc7a5d, 
		0x4f20ad5a, 
		0x80000001, 
		0x630bb761, 
		0x89f95f9c, 
		0x00800002, 
		0x42340000, 
		0x41d00000, 
		0xffc00001, 
		0x00800003, 
		0x0c7ffff8, 
		0x0e000007, 
		0x00800001, 
		0x00000010, 
		0x00000000, 
		0x6ac75e9c, 
		0xc2000000, 
		0x0d000ff0, 
		0x42000000, 
		0xff800000, 
		0x00000000, 
		0x40800000, 
		0x42180000, 
		0x7f7ffffe, 
		0xff800000, 
		0x871e421d, 
		0x3f028f5c, 
		0x00040000, 
		0xcb8c4b40, 
		0x6bf17e45, 
		0x0e000001, 
		0xffbc0abe, 
		0x0c7ffffc, 
		0x80020000, 
		0x41900000, 
		0x0c780000, 
		0xc2540000, 
		0x3f800000, 
		0x00000000, 
		0xc0c00000, 
		0x0e00003f, 
		0xff7fffff, 
		0x91bb8c4d, 
		0xe07de4a0, 
		0x40000000, 
		0x0c600000, 
		0x42380000, 
		0xa99ec750, 
		0x7f7fffff, 
		0x70a29f7f, 
		0xbfc65ecb, 
		0xa02d67e7, 
		0x80000800, 
		0x40000000, 
		0x6d01aca1, 
		0x80000200, 
		0x5a943366, 
		0xf9a4cd81, 
		0x80200000, 
		0xc2340000, 
		0x03fa06bd, 
		0x0c600000, 
		0x7f800000, 
		0x41d80000, 
		0x0c7f0000, 
		0xfba7f02d, 
		0x30df1cf2, 
		0x41c80000, 
		0x00000004, 
		0x3f800001, 
		0x04b351b1, 
		0x0c7fc000, 
		0x0e0000ff, 
		0xad291b06, 
		0xc1100000, 
		0xff7fffff, 
		0x00000000, 
		0x80000004, 
		0x33333333, 
		0xc1f80000, 
		0x7f800000, 
		0x80000001, 
		0x0c600000, 
		0x0e0003ff, 
		0x79bc4cd1, 
		0x33fa101e, 
		0x0e01ffff, 
		0x47022e04, 
		0x00800000, 
		0x28005166, 
		0x00000001, 
		0x80010000, 
		0x80000004, 
		0x00000002, 
		0x0c780000, 
		0x00000004, 
		0x80800002, 
		0x656ed09f, 
		0xee3e7bd0, 
		0xff800001, 
		0x7f7fffff, 
		0x0e3fffff, 
		0x00000000, 
		0x297372ab, 
		0x8f4c4c6d, 
		0x41500000, 
		0x80000000, 
		0x41880000, 
		0xff800000, 
		0x80000004, 
		0x0e000007, 
		0x7fb41115, 
		0x420c0000, 
		0x80040000, 
		0x7fc00001, 
		0x00000000, 
		0xb591705b, 
		0x80000800, 
		0x4b8c4b40, 
		0x41b80000, 
		0xc2580000, 
		0x3dbd8aee, 
		0x8207557c, 
		0x41a5886f, 
		0xbc694efa, 
		0xffc00001, 
		0x420c0000, 
		0xbf6dcad2, 
		0xff800001, 
		0x007fffff, 
		0x7f800000, 
		0xc20c0000, 
		0x8f7ffffd, 
		0x41d00000, 
		0x42080000, 
		0x7c773890, 
		0x4354dbc2, 
		0x00000001, 
		0x0c7ffffc, 
		0x932cee3f, 
		0x33333333, 
		0xc2300000, 
		0x7f800000, 
		0x80000000, 
		0x8f7ffffd, 
		0x43e329d5, 
		0x4b8c4b40, 
		0x80800002, 
		0x0d000ff0, 
		0x7f800000, 
		0x421c0000, 
		0x420c0000, 
		0xff800000, 
		0xc2180000, 
		0xf9684d85, 
		0x7fc00001, 
		0xff85dac3, 
		0x71281a00, 
		0x42700000, 
		0x4761d1ce, 
		0x803ae6b4, 
		0x80000000, 
		0x00400000, 
		0x544e23bf, 
		0x4fc40cb9, 
		0x80270182, 
		0x4f9e301b, 
		0xd5ad2a42, 
		0x21f2e4fc, 
		0x0e7fffff, 
		0x0e00003f, 
		0x6c9cddc6, 
		0x00800003, 
		0x80000000, 
		0x8f7ffffe, 
		0x07f5b326, 
		0x4b000000, 
		0x80800002, 
		0x80a80a35, 
		0xc1b80000, 
		0x69840d19, 
		0xa66e733c, 
		0x007fffff, 
		0x80000200, 
		0x80000000, 
		0x7f000000, 
		0xb125ff59, 
		0x0f7fffff, 
		0xdf4e1a51, 
		0x1c00e26d, 
		0x0e000007, 
		0x356a7649, 
		0x7fc00001, 
		0x80800000, 
		0xff8db7ea, 
		0x8be9a9b4, 
		0x80687f9d, 
		0xeccbc2f6, 
		0xff800000, 
		0x1aaada72, 
		0xc2480000, 
		0x7f800000, 
		0xc1b00000, 
		0x0c7fc000, 
		0x42100000, 
		0x0c7fe000, 
		0xff800000, 
		0x78e08302, 
		0xff7ffffe, 
		0xb9d362bc, 
		0xc1c00000, 
		0x0e0003ff, 
		0xc1f00000, 
		0x80000002, 
		0x42740000, 
		0x909e58c5, 
		0x42440000, 
		0x63c35f11, 
		0x3ddf4e66, 
		0x80000000, 
		0x0e000001, 
		0x7f7ffffe, 
		0x80000002, 
		0xbb4ee598, 
		0x80800003, 
		0x80040000, 
		0xd6eba8de, 
		0x41e00000, 
		0xff7ffffe, 
		0x7f7fffff, 
		0xc0f2b011, 
		0xd050e532, 
		0xff800000, 
		0x0e0007ff, 
		0x80000000, 
		0x80001000, 
		0x40800000, 
		0x80000100, 
		0x00000000, 
		0x0e001fff, 
		0x8f7fffff, 
		0x255f4438, 
		0x0e03ffff, 
		0xcf786cb7, 
		0x80800002, 
		0x00000000, 
		0xc1e00000, 
		0x5898422f, 
		0xaff165df, 
		0x8f7ffffd, 
		0xff800000, 
		0xcb8c4b40, 
		0x40400000, 
		0x00000000, 
		0xc2480000, 
		0xff7ffffe, 
		0x80000000, 
		0xb5046233, 
		0x4efdd353, 
		0xc0000000, 
		0xb9e7e9e9, 
		0x483fbb99, 
		0x80ffffff, 
		0xff800000, 
		0x80000000, 
		0x80574a03, 
		0x0e01ffff, 
		0x00000000, 
		0x6b68a073, 
		0x807fffff, 
		0xc1000000, 
		0x42180000, 
		0x5f5633f6, 
		0xff800000, 
		0x41b00000, 
		0x5487feae, 
		0x96d7a207, 
		0x7fb7d382, 
		0x80800002, 
		0xc2400000, 
		0x42480000, 
		0x80080000, 
		0x04b7fda6, 
		0x80000000, 
		0xfdc05ebb, 
		0x40000000, 
		0xff000000, 
		0xc3720b3f, 
		0x199d34df, 
		0x80800003, 
		0x80000080, 
		0x51b82cfd, 
		0xc2100000, 
		0x00011111, 
		0xc2000000, 
		0xffffffff, 
		0x8f7ffffd, 
		0xd8028855, 
		0x00800000, 
		0x80000040, 
		0x0c7fe000, 
		0x0e00003f, 
		0xc2200000, 
		0x794bd4ae, 
		0x00010000, 
		0x00200000, 
		0x80001000, 
		0x80100000, 
		0x00000001, 
		0x3007fd3f, 
		0x80000000, 
		0x42640000, 
		0x7f7fffff, 
		0x40c00000, 
		0x89ce812d, 
		0x7f800000, 
		0xae46e82f, 
		0xbf800000, 
		0x00000000  
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
		0x00000217, 
		0x000000e7, 
		0x0000004d, 
		0x000000cb, 
		0x0000031b, 
		0x0000039f, 
		0x00000133, 
		0x000001e5, 
		0x0000012b, 
		0x000003b9, 
		0x000001af, 
		0x000001bb, 
		0x0000037b, 
		0x0000018f, 
		0x00000363, 
		0x000002a3, 
		0x00000113, 
		0x000000a9, 
		0x00000319, 
		0x0000026f, 
		0x000002b9, 
		0x0000019d, 
		0x000000fb, 
		0x0000024b, 
		0x00000073, 
		0x000001fb, 
		0x0000031b, 
		0x00000341, 
		0x000002c1, 
		0x00000055, 
		0x000001ef, 
		0x00000303, 
		0x000000ab, 
		0x000001eb, 
		0x00000247, 
		0x000002f5, 
		0x000000c5, 
		0x0000020f, 
		0x000001c5, 
		0x0000003f, 
		0x000002fd, 
		0x000001e7, 
		0x000000d7, 
		0x0000038d, 
		0x000001ff, 
		0x000001db, 
		0x000002cd, 
		0x0000021f, 
		0x0000013b, 
		0x0000005b, 
		0x0000016d, 
		0x0000002f, 
		0x000000df, 
		0x00000069, 
		0x000003a3, 
		0x00000065, 
		0x0000002f, 
		0x0000035f, 
		0x0000013d, 
		0x0000035d, 
		0x0000032b, 
		0x0000009f, 
		0x0000029b, 
		0x000002a3, 
		0x0000006d, 
		0x00000201, 
		0x0000024d, 
		0x000002ab, 
		0x00000075, 
		0x000001a1, 
		0x00000087, 
		0x000000ed, 
		0x0000037f, 
		0x0000018f, 
		0x00000227, 
		0x00000173, 
		0x00000273, 
		0x000003eb, 
		0x00000331, 
		0x0000039b, 
		0x0000010f, 
		0x00000009, 
		0x0000019d, 
		0x00000089, 
		0x000000f3, 
		0x0000032d, 
		0x00000123, 
		0x000002a7, 
		0x0000004d, 
		0x0000017b, 
		0x00000337, 
		0x00000193, 
		0x0000020d, 
		0x0000038d, 
		0x000003ff, 
		0x00000325, 
		0x00000371, 
		0x000000a1, 
		0x0000017f, 
		0x000003ef, 
		0x00000283, 
		0x000003a9, 
		0x000002ed, 
		0x00000293, 
		0x0000010b, 
		0x000003bd, 
		0x00000053, 
		0x00000019, 
		0x00000357, 
		0x00000327, 
		0x000002ab, 
		0x00000051, 
		0x0000036b, 
		0x0000024d, 
		0x00000155, 
		0x000002b3, 
		0x000000d1, 
		0x00000153, 
		0x00000237, 
		0x00000245, 
		0x00000139, 
		0x00000159, 
		0x000000b9, 
		0x0000017b, 
		0x00000195, 
		0x00000061, 
		0x0000001d, 
		0x000000d7, 
		0x000002e7, 
		0x0000039b, 
		0x00000285, 
		0x0000000b, 
		0x000000cb, 
		0x0000033b, 
		0x00000209, 
		0x000003eb, 
		0x0000031b, 
		0x000003d1, 
		0x0000002f, 
		0x00000271, 
		0x000001cb, 
		0x00000181, 
		0x000001d3, 
		0x00000325, 
		0x000002b3, 
		0x0000021f, 
		0x0000035b, 
		0x00000289, 
		0x000003c9, 
		0x000003b9, 
		0x0000001f, 
		0x00000337, 
		0x000002f1, 
		0x000003c3, 
		0x0000017f, 
		0x00000189, 
		0x000001ab, 
		0x00000247, 
		0x00000015, 
		0x0000022f, 
		0x000002dd, 
		0x0000039b, 
		0x0000011b, 
		0x0000025d, 
		0x0000029f, 
		0x000001bd, 
		0x00000305, 
		0x00000019, 
		0x000003ad, 
		0x00000125, 
		0x0000034d, 
		0x00000305, 
		0x00000047, 
		0x00000083, 
		0x00000151, 
		0x000002bb, 
		0x0000015f, 
		0x00000397, 
		0x00000043, 
		0x0000012b, 
		0x0000025d, 
		0x000000c9, 
		0x00000063, 
		0x000002f3, 
		0x00000303, 
		0x00000375, 
		0x0000023d, 
		0x0000016f, 
		0x0000022b, 
		0x00000215, 
		0x0000017f, 
		0x0000031f, 
		0x0000024d, 
		0x00000315, 
		0x00000035, 
		0x00000395, 
		0x0000030b, 
		0x000002bb, 
		0x00000175, 
		0x00000117, 
		0x000002e9, 
		0x00000377, 
		0x00000267, 
		0x00000189, 
		0x000001e7, 
		0x00000297, 
		0x0000038b, 
		0x000001eb, 
		0x00000307, 
		0x00000127, 
		0x00000151, 
		0x0000024d, 
		0x000003d3, 
		0x0000027b, 
		0x00000131, 
		0x0000015b, 
		0x000000b1, 
		0x000001b7, 
		0x000003d9, 
		0x0000036d, 
		0x000002e9, 
		0x000002d1, 
		0x00000023, 
		0x0000005b, 
		0x00000261, 
		0x0000014d, 
		0x000002a1, 
		0x000000e9, 
		0x0000004b, 
		0x000002b1, 
		0x0000029b, 
		0x00000207, 
		0x000002c1, 
		0x00000009, 
		0x00000011, 
		0x00000397, 
		0x00000015, 
		0x00000355, 
		0x000001eb, 
		0x000003b9, 
		0x00000351, 
		0x0000016b, 
		0x00000375, 
		0x0000038d, 
		0x00000319, 
		0x00000277, 
		0x000002db, 
		0x00000265, 
		0x00000173, 
		0x0000036b, 
		0x000002f7, 
		0x000000f7, 
		0x00000343, 
		0x000002e1, 
		0x0000036b, 
		0x000000f7  
};
